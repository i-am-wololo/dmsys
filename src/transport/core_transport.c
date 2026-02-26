#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "transport.h"
#include "../common.h"


void init_transport(transport_config config) {
	printf("initializing transport process...\n");
	if (fork())  {
		printf("TRANSPORT: reading from clients2transport pipe...\n");
		Header* header;
		while ((header = deserialize(config.c2t)) != NULL) {
			printf("TRANSPORT: received header\n");
			print_header(header);
			// ignorer les données qui viennent (a enlever plus tard)
			read(config.c2t[0], NULL, sizeof(int)*header->length);
			free(header);
		}
		printf("fin de transport\n");
		exit(0);
	}
}
