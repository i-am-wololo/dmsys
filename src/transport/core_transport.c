#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include "transport.h"
#include "../common.h"
#include "../utils.h"


void init_transport(transport_config config) {
	printf("initializing transport process...\n");
	if (fork() == 0)  {
		uint32_t* dynbuffer;
		int* t2c; 
		int response;
		close(config.c2t[1]);
		for (int i = 0; i<CLIENTNUMBER; close(config.t2c[i++][0]));

		printf("TRANSPORT: reading from clients2transport pipe...\n");
		Header* header;
		while ((header = deserialize(config.c2t)) != NULL) {
			printf("TRANSPORT: received header (and data)\n");
			dynbuffer = malloc(sizeof(uint32_t)*header->length);
			print_header(header);
			t2c = config.t2c[header->source]; // response pipe
			read(config.c2t[0], dynbuffer, sizeof(uint32_t)*header->length);
			printf("computing checksum...\n");
			uint16_t checksum = compute_checksum(header, dynbuffer);
			if( checksum == header->checksum) {
				printf("TRANSPORT: checksum ok!\n");
				if (config.ports[header->portnumber] == -1) {
					printf("TRANSPORT: port non occupé\n");
					response = 2;

				} else {
					// bloc d'envoie
#ifdef PIPEBUILD
					printf("TRANSPORT: pipe version\n");
					serialize_server_pipe(header, dynbuffer, config.t2s[header->portnumber]);
#endif
#ifdef SHMBUILD
					printf("shmbuild\n");
						// serialize_server_shm();
#endif
#ifdef MSGQUEUEBUILD
						printf("msgqueue a faire\n");
#endif
				}
			} else {
				printf("TRANSPORT: checksum not ok, expected checksum %u\n", checksum);
				response = 1;
			}
			write(t2c[1], &response, sizeof(int));
			printf("TRANSPORT: response %d to client %d\n", response, header->source);
			free(header);
			free(dynbuffer);
		}
		printf("fin de transport\n");

		close(config.c2t[0]);
		shmdt(config.ports);
		exit(0);
	}
}
