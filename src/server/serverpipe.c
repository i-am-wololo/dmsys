#ifdef PIPEBUILD

#include "./server.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>

#define M 5
#define N 5

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void P(int semid) {
    struct sembuf op = {0, -1, 0}; // Décrémente (attend que le sémaphore soit > 0)
    semop(semid, &op, 1);
}

void V(int semid) {
    struct sembuf op = {0, 1, 0};  // Incrémente (libère)
    semop(semid, &op, 1);
}


void init_server(transport_config config,int semid, int16_t* port_map) {
    for(int i=0; i<M; i++) 
        port_map[i] = -1;
    printf("Server processes initializing.\n");
    for(int i=0;i<M;i++) {
        if (fork() == 0) {
            srand(123456+i);
            sleep(rand()%5);
            printf("Server process %d is ready.\n",i);
            int my_port = -1;
            while (my_port == -1) {
                int target = rand() % N; 
                P(semid);
                if (port_map[target] == -1) {
                    port_map[target] = 1;
                    my_port = target;
								}
                V(semid);
						}
                printf("Serveur registered on port %d.\n", i);
        int read_fd = config.t2s[i][0];
        uint16_t data_length;
        uint16_t received_checksum;
        while (read(read_fd, &data_length, sizeof(uint16_t)) > 0) {
            uint32_t* data_buffer = malloc(sizeof(uint32_t) * data_length);
            int bytes_to_read = sizeof(uint32_t) * data_length;
            int total_received = 0;
            while (total_received < bytes_to_read) {
                int n = read(read_fd, ((char*)data_buffer) + total_received, bytes_to_read - total_received);
                if (n <= 0) break;
                total_received += n;
            }
            read(read_fd, &received_checksum, sizeof(uint16_t));
            if (compute_server_checksum(data_buffer, data_length) == received_checksum) {
                printf("SERVEUR %d: Paquet reçu (%hu entiers). Checksum OK.\n", i, data_length);
            } else {
                printf("SERVEUR %d: Erreur de Checksum !\n", i);
            }
            free(data_buffer);
        }
			}
        exit(0);
  	}
	}
}
#endif

