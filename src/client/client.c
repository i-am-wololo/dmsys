#include "client.h"
#include "../common.c"
#include <stdint.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "../utils.h"

struct stats {
	int loss;
	int fchecksum; // normalement si tout ce passe bien ce champs devrait tout le temps être a 0
	int ok;
};

// int dummy_data[][8] = {
// 	{1, 3, 4, 5, 8, 0, 1, 9},
// 	{1, 4},
// 	{9, 1, 8, 2, 3},
// 	{1, 0, 8, 3, 4, 9, 21},
// 	{12, 331, 90318, 121, 12}
// };


uint32_t* create_data(Header* header, uint len, int id) {
	uint32_t* data = malloc(sizeof(uint32_t)*len);
	srand(12345+id);
	for (int i = 0; i<len; i++) {
		data[i] = rand()%300;
	}
	header->length = len;
	return data;
}

void init_client(transport_config config, int id, uint datalen, uint msgnum, struct stats* stats) {

	uint32_t* data;
	int* sendpipe = config.c2t; // envoie
	int* recvpipe = config.t2c[id]; // lecture transport vers client
	int response;

	close(sendpipe[0]);
	close(recvpipe[1]);

	sleep(rand()%5);
	printf("commencement d'envoie de données pour client %d\n", id);
	for (int i = 0; i < msgnum; i++) {
		Header header;

		data = create_data(&header, datalen, id);
		header.datanumber = i;
		header.portnumber = id;
		header.source = id;
		// header.length = 8;
		
		header.checksum = compute_checksum(&header, data);
		serialize(&header, data, sendpipe);
		read(recvpipe[0], &response, sizeof(int));
		switch (response) {
			case 0:
				stats->ok++;

			case 1:
				stats->fchecksum++;

			case 2:
				stats->loss++;
		}
	}
	close(sendpipe[1]);
	exit(0);
};

void init_clients(transport_config config, uint8_t clientsnumber) {
	int statid = shmget(IPC_PRIVATE, sizeof(struct stats), IPC_CREAT|0666);
	struct stats* stats = shmat(statid, 0, 0);

	stats->loss = 0;
	stats->fchecksum = 0;
	stats->ok = 0;

	for (int i = 0; i < clientsnumber; i++) {
		if (fork() == 0) {
			shmdt(config.ports);
			printf("client %d créé, en attente\n", i);
			sleep(1);
			init_client(config, i, 2000, 10, stats);
		}
	}
	printf("fin de création des clients\n");
	for (int i = 0; i < clientsnumber; i++) {
			wait(NULL);
			printf("un client récupéré \n");
	}
	printf("fin des clients, fermeture du tube transport");
	printf(
			"!! stats clients !!\n"
			"paquets ok: %d"
			"paquets avec checksum faux: %d"
			"paquets perdus: %d",
			stats->ok, stats->fchecksum, stats->loss
				
			);
	close(config.c2t[1]);
}

