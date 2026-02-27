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

// je viens de m'en rendre compte
// le champs source est redondant car un client N envoie à un port N
// donc portnumber aurait très bien pu être utilisé par le transport pour savoir où envoyer la réponse

void print_header(Header* h);
void serialize(Header* h, uint32_t* data, int pipe[2]);
Header* deserialize(int pipe[2]);

#endif
