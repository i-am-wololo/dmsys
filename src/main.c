#include <stdio.h>
#include <stdlib.h>
#include "transport/transport.h"
#include <string.h>
#include "client/client.h"
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#define CLIENTNUMBER 5
#define SERVERNUMBER 20

int* initialize_ports() {
	printf("initialisation tableau de ports\n");
	int id = shmget(IPC_PRIVATE, sizeof(int)*SERVERNUMBER, IPC_CREAT|0777);
	int* portsptr = shmat(id, 0, 0);
	if (portsptr == (void*) -1) {
		printf("allocation du tableau de ports echoué\n");
		return NULL;
	}
	memset(portsptr, 0, SERVERNUMBER*sizeof(int));
	return portsptr;
}


int main(int argc, char* argv[]) {
	int* ports = initialize_ports();
	int clientpipes[CLIENTNUMBER][2];
	int transportpipe[2];
	printf("creation des tubes\n");
	for (int i = 0; i<CLIENTNUMBER; i++) {
		pipe(clientpipes[i]);
	}
	pipe(transportpipe);
	transport_config config;
	config.c2t = transportpipe;
	config.t2c = clientpipes;
	config.ports = ports;
	init_transport(config);
	init_clients(config, 5);
	

}
