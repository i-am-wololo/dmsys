#include "client.h"
#include "../common.c"
#include <stdint.h>
#include <unistd.h>
#include "../utils.h"

int dummy_data[][8] = {
	{1, 3, 4, 5, 8, 0, 1, 9},
	{1, 4},
	{9, 1, 8, 2, 3},
	{1, 0, 8, 3, 4, 9, 21},
	{12, 331, 90318, 121, 12}
};



void init_client(transport_config config, int id) {
	int* sendpipe = config.c2t; // envoie
	int* recvpipe = config.t2c[id]; // lecture transport vers client
	close(sendpipe[0]);
	sleep(2);
	printf("commencement d'envoie de données pour client %d\n", id);
	Header header;
	header.portnumber = 0;
	header.datanumber = 1;
	header.length = 8;
	header.checksum = 0; 
	header.source = id;
	header.checksum = compute_checksum(&header, dummy_data[id]);
	serialize(&header, dummy_data[id], sendpipe);
	close(sendpipe[1]);
	exit(0);
};

void init_clients(transport_config config, uint8_t clientsnumber) {
	for (int i = 0; i < clientsnumber; i++) {
		if (fork() == 0) {
			printf("client %d créé, en attente\n", i);
			init_client(config, i);
		}
	}
	printf("fin de création des clients\n");
	for (int i = 0; i < clientsnumber; i++) {
			wait(NULL);
			printf("un client récupéré \n");
	}
	printf("fin des clients, fermeture du tube transport");
	close(config.c2t[1]);
}

