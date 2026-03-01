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
#include <sys/msg.h>


// ces variables sont necessaire pour les 3 programmes
int shmid;
int semid;

#if SHMBUILD
int server_shms[SERVERNUMBER];
int server_sems;

void initialize_sems() {
	// c'est pour shm
	// plutot que de faire plusieurs groupes de semaphore unaire,
	// il est plus judicieux d'en faire un seul avec plusieurs sempahores dedans
	
	server_sems = semget(IPC_PRIVATE, SERVERNUMBER, IPC_CREAT|0666);
	
}

void initialize_shms() {
	for (int i = 0; i < SERVERNUMBER; i++) {
		server_shms[i] = shmget(IPC_PRIVATE, 2048, IPC_CREAT| 0666);
	}
}

void clean_shms() {
	for (int i = 0; i <SERVERNUMBER; i++) {
		shmctl(server_shms[i], IPC_RMID, NULL);
	}
}

#endif

int16_t* initialize_ports() {
	printf("initialisation tableau de ports\n");
	shmid = shmget(IPC_PRIVATE, sizeof(int16_t)*(SERVERNUMBER+10), IPC_CREAT|0777);
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
	semctl(semid, 0, SETVAL, 1);  // ← add this
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
	init_server(config, semid, config.ports);
#endif

#ifdef SHMBUILD
	printf("demarrage serveurs shm\n");
	initialize_shms();
	initialize_sems();
	config.shmids = server_shms;
	config.sem_sync = server_sems;
	init_servers(config, SERVERNUMBER, config.ports);
#endif

#ifdef MSGQUEUEBUILD
	printf("demarrage serveurs MessageQueue\n");
	config.mqid = msgget(IPC_PRIVATE, IPC_CREAT|0666);
	init_servers(config, SERVERNUMBER);
#endif

	init_transport(config);
	init_clients(config, CLIENTNUMBER);
	close(transportpipe[1]);

#ifdef SHMBUILD
	wait(NULL);
	printf("nettoyage serveurs SHM\n");
	clean_shms();
#endif
}
