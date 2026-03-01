#include "../common.h"
#include <stdint.h>
#ifndef TRANSPORT
#define TRANSPORT


typedef struct {
	int (*t2c)[2]; // tableau de tubes transport client 
	int* c2t; // tube client transport
	int16_t* ports;
#ifdef PIPEBUILD
	int (*t2s)[2];
#endif

#ifdef MSGQUEUEBUILD
	int mqid;
#endif

#ifdef SHMBUILD
	int* shmids; // tableau de pointeurs vers des zones shm (un par serveur)
		int sem_sync; // id du jeu de semaphores
#endif
} transport_config;

void init_transport(transport_config config);
#endif

void serialize_server_pipe(Header* h, uint32_t* data, int* pipe);
void serialize_server_shm(Header* h, uint32_t* data, char* shmptr, int sem_ctrl);
void serialize_server_msgq(Header* h, uint32_t* data, int mqid);

