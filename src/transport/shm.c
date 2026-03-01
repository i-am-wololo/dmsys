#include <sys/types.h>
#include "../common.h"
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "./transport.h"
#include "../utils.h"


// presque pareil que pipe
void serialize_server_shm(Header *h, uint32_t *data, char *shmptr, int sem_ctrl) {
	size_t taille = sizeof(ServerHeader) + (h->length * sizeof(uint32_t));
	char* buffer = malloc(taille);

	ServerHeader sh;
	sh.datanumber = h->datanumber;
	sh.checksum = compute_server_checksum(data, h->length);
	sh.length = h->length;

	if (buffer) {
		memcpy(buffer, &sh, sizeof(ServerHeader));
		memcpy(buffer+sizeof(ServerHeader), data, sh.length*sizeof(uint32_t));
		memcpy(shmptr, buffer, taille);
	}

	Vmult(sem_ctrl, h->portnumber);
	free(buffer);
}
