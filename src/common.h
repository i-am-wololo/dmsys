#ifndef DM_COMMON
#define DM_COMMON
#include <sys/sem.h>
#include <stdint.h>
#define CLIENTNUMBER 3
#define SERVERNUMBER 2
#define MAX_SIZE 2048

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

typedef struct {
	uint16_t datanumber;
	uint16_t length;
	uint16_t checksum;
} ServerHeader;

// pour msgqueue, msgsnd et msgrcv s'attendent a un buffer ou les premiers bytes est le type.
// ce buffer peut etre organise de plusieurs facons, ici j'ai fait un struct
typedef struct {
	long mtype; // servira de port
	ServerHeader sh;
	uint32_t data[MAX_SIZE];
} MQPacket;

// fonctions sémaphore
void P(int semid);
void V(int semid);

void Pmult(int semid, int sem_index);
void Vmult(int semid, int sem_index);


void print_header(Header* h);
void serialize(Header* h, uint32_t* data, int pipe[2]);
Header* deserialize(int pipe[2]);


#endif



