#include <stdio.h>
#include <stdlib.h>
#include "../transport/transport.h"
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>


void init_client(transport_config config, int id) {
	int clientpipe = config.c2t[id]; // lecture transport vers client
	sleep(8);
	exit(0);
};

void init_clients(transport_config config, uint8_t clientsnumber) {
	for (int i = 0; i < clientsnumber; i++) {
		int clientnumber = fork(); // le numéro retourné par le fork servira d'identifiant pour client
		printf("client %d créé, en attente", clientnumber);
		init_client(config, clientnumber);
	}
	printf("fin de création des clients\n");
	for (int i = 0; i < clientsnumber; i++) {
			wait(NULL);
			printf("un client récupéré \n");
	}
}

