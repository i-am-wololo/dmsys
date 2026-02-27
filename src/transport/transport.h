#ifndef TRANSPORT
#define TRANSPORT

typedef struct {
	int (*t2c)[2]; // tableau de tubes transport client 
	int* c2t; // tube client transport
	int* ports;
} transport_config;


void init_transport(transport_config config);
#endif

