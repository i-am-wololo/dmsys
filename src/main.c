#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <transport.h>
#include <server.h>
#include <string.h>
#include <client.h>
#include <unistd.h>
#include "./common.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int shmid;
int semid;

int16_t* initialize_ports() {
	printf("initialisation tableau de ports\n");
	shmid = shmget(IPC_PRIVATE, sizeof(int16_t)*SERVERNUMBER, IPC_CREAT|0777);
	int16_t* portsptr = shmat(shmid, 0, 0);
	if (portsptr == (void*) -1) {
		printf("allocation du tableau de ports echoué\n");
		return NULL;
	}
	memset(portsptr, -1, SERVERNUMBER*sizeof(int));
	return portsptr;
}

void init_serverpipes(int** pipes, uint n) {
	printf("creation des pipes serveurs");
	for (int i = 0; i<n; i++) pipe(pipes[i]);
};


void clean_ipc() {
	semctl(semid, 0, IPC_RMID);

	shmctl(shmid, IPC_RMID, NULL);
}

int main(int argc, char* argv[]) {
	int16_t* ports = initialize_ports();
	semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
	int clientpipes[CLIENTNUMBER][2];
	int transportpipe[2];
	transport_config config;

#ifdef PIPEBUILD
	int serveurpipe[SERVERNUMBER][2];
	config.t2s = serveurpipe;
#endif

	printf("creation des tubes\n");
	for (int i = 0; i<CLIENTNUMBER; i++) {
		pipe(clientpipes[i]);
	}
	pipe(transportpipe);
	config.c2t = transportpipe;
	config.t2c = clientpipes;
	config.ports = ports;
#ifdef PIPEBUILD
	printf("demarrage serveur tubes\n");
	init_server(config, semid, ports);
#endif
	init_transport(config);
	init_clients(config, 5);
	close(transportpipe[1]);
}
