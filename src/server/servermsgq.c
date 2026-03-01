#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdint.h>

#include "server.h"
#include "../common.h"
#include "../utils.h"

#ifdef MSGQUEUEBUILD
void init_server(transport_config config, int server_id, int sem_ctrl, int N) {
		MQPacket buf;
		ServerHeader bufsh;
    srand(123456 + server_id);
		int16_t* port_map = config.ports;	
    int my_port = -1;
		
    while (my_port == -1) {
        int target = rand() % N;
        P(sem_ctrl);
        if (port_map[target] == -1) {
            port_map[target] = server_id;
            my_port = target;
        }
        V(sem_ctrl);
			if (my_port == -1) sleep(1);
    }
    printf("SERVER %d: Port %d\n", server_id, my_port + 1);

		while (msgrcv(config.mqid, &buf, sizeof(MQPacket)-sizeof(long), my_port, 0) != -1) {
			printf("MSGQ serv: recu message\n");
			bufsh = buf.sh;
			if (compute_server_checksum(buf.data, bufsh.length) == bufsh.checksum) {
            printf("SERVER %d: OK (%hu entiers)\n", server_id, bufsh.length);
			}
        sleep(rand() % 5);
		}
}

void init_servers(transport_config config, int N) {
	int sem_ctrl = semget(IPC_PRIVATE, 1, IPC_CREAT|0666);
	semctl(sem_ctrl, 0, SETVAL, 1);

	if (fork() == 0) {
		for (int i = 0; i < N; i++) {
			if (fork() == 0) {
				printf("lancement serveur %d\n", i);
				init_server(config, i, sem_ctrl, N);
			}
		}
	}
}
#endif
