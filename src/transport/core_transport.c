#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "transport.h"
#include "../common.h"
#include "../utils.h"


void init_transport(transport_config config) {
	printf("initializing transport process...\n");
	if (fork() == 0)  {
		int* dynbuffer;
		printf("TRANSPORT: reading from clients2transport pipe...\n");
		Header* header;
		while ((header = deserialize(config.c2t)) != NULL) {
			printf("TRANSPORT: received header (and data)\n");
			dynbuffer = malloc(sizeof(int)*header->length);
			print_header(header);
			read(config.c2t[0], dynbuffer, sizeof(int)*header->length);
			printf("computing checksum...\n");
			if(compute_checksum(header, dynbuffer) == header->checksum) {
				printf("TRANSPORT: checksum ok!\n");
			} else printf("TRANSPORT: checksum not ok\n");

			free(header);
			free(dynbuffer);
		}
		printf("fin de transport\n");
		exit(0);
	}
}
