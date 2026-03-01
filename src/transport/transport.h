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
#ifdef SHMBUILD
	char** shmptrs // tableau de pointeurs vers des zones shm (un par serveur)
#endif
} transport_config;

void init_transport(transport_config config);
#endif

void serialize_server_pipe(Header* h, uint32_t* data, int* pipe);
void serialize_server_shm(Header* h, uint32_t* data, char* shmptr);


