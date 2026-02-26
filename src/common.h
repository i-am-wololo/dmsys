#ifndef DM_COMMON
#define DM_COMMON
#include <stdint.h>

// definition de l'entête
// pendant les communication, les champs devront être envoyés dans cette ordre!
typedef struct {
	uint16_t portnumber; 
	uint16_t datanumber;
	uint16_t length;
	uint16_t source; //  id du client
	uint16_t checksum;
} Header;

void print_header(Header* h);
void serialize(Header* h, int* data, int pipe[2]);
Header* deserialize(int pipe[2]);

#endif
