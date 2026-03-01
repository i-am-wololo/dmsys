#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdint.h>

#include "server.h"
#include "../common.h"
#include "../utils.h"

#ifdef SHMBUILD

void init_server(transport_config config, int server_id, int N, int sem_ctrl, int16_t* port_map) {
    srand(123456 + server_id);
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

    uint32_t* shm_ptr = shmat(config.shmids[my_port], NULL, 0);

    while (1) {
				Pmult(config.sem_sync, my_port);

				// il est plus judicieux (bien que peut etre pas safe)  de caster directement en entete 
				ServerHeader* sh = (ServerHeader*)shm_ptr;
				uint16_t len = sh->length;
				uint16_t received_chk = sh->checksum;
				uint32_t* data = (uint32_t*)(sh + 1);

        if (compute_server_checksum(data, len) == received_chk) {
            printf("SERVER %d: OK (%hu entiers)\n", server_id, len);
        } else {
            printf("SERVER %d: Erreur Checksum\n", server_id);
        }

				// les serveurs sont immortel, jsp si on devrait laisser comme ca
				// mais sinon ca marche, ils arrive a communiquer avec les clients
        sleep(rand() % 5);
    }
}

void init_servers(transport_config config, int N, int16_t* port_map) {
	// c'est dans un fork pour ne pas bloquer le fil d'execution principal
	if (fork() == 0) {
		// semaphore propre aux serveurs afin de faire en sorte qu'ils prennent a leur tout un port
		int sem_ctrl = semget(IPC_PRIVATE, 1, IPC_CREAT|0666); 
		semctl(sem_ctrl, 0, SETVAL, 1);
		for (int i = 0; i < N; i++) {
			if (fork() == 0) {
				printf("lancement serveur %d\n", i);
				init_server(config, i, N, sem_ctrl, port_map);
			}

		}
		
		for (int i = 0; i < N; i++) {
			wait(NULL);
			printf("un serveur a terminé\n");
		}

		printf("terminaison des serveurs, nettoyage IPC\n");
		semctl(sem_ctrl, 0, IPC_RMID);
		exit(0);
	}
}

#endif

