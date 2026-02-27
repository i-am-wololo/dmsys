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
		printf("TRANSPORT: reading from clients2transport pipe...\n");
		Header* header;
		while ((header = deserialize(config.c2t)) != NULL) {
			printf("TRANSPORT: received header (and data)\n");
			dynbuffer = malloc(sizeof(uint32_t)*header->length);
			print_header(header);
			t2c = config.t2c[header->source]; // response pipe
			close(t2c[0]); 
			read(config.c2t[0], dynbuffer, sizeof(uint32_t)*header->length);
			printf("computing checksum...\n");
			if(compute_checksum(header, dynbuffer) == header->checksum) {
				printf("TRANSPORT: checksum ok!\n");
				if (config.ports[header->portnumber] == -1) {
					printf("TRANSPORT: port non occupé\n");
					response = 2;
				}
			} else {
				printf("TRANSPORT: checksum not ok\n");
				response = 1;
			}
			write(t2c[1], &response, sizeof(int));
			printf("TRANSPORT: response %d to client %d", response, header->source);
			free(header);
			free(dynbuffer);
		}
		printf("fin de transport\n");

		close(config.c2t[0]);
		shmdt(config.ports);
		exit(0);
	}
}
