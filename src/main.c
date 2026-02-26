#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "transport/transport.h"
#include "client/client.h"
#include <unistd.h>
#define CLIENTNUMBER 5


int main(int argc, char* argv[]) {
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
	init_transport(config);
	init_clients(config, 5);

}
