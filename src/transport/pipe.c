#include "../common.h"
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// converti le header client en header server, puis l'écrit
void serialize_server_pipe(Header *h, uint32_t *data, int* pipe) {
	size_t taille = sizeof(ServerHeader) + (h->length * sizeof(uint32_t));
	char* buffer = malloc(taille);

	ServerHeader sh;
	sh.datanumber = h->datanumber;
	sh.checksum = h->checksum;
	sh.length = h->length;

	if (buffer) {
		memcpy(buffer, &sh, sizeof(ServerHeader));
		memcpy(buffer+sizeof(ServerHeader), data, sh.length*sizeof(uint32_t));
		write(pipe[1], buffer, taille);
	}
	free(buffet)
}
