#include "../common.h"
#include "transport.h"
#include "../utils.h"
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>

void serialize_server_msgq(Header* h, uint32_t* data, int mqid) {
	MQPacket paq;
	paq.mtype = h->portnumber+1; // dans certains cas, probleme quand le port est 0
	paq.sh.length = h->length;
	paq.sh.checksum = compute_server_checksum(data, paq.sh.length);
	memcpy(paq.data, data, h->length*sizeof(uint32_t));

	msgsnd(mqid, &paq, sizeof(ServerHeader) + h->length * sizeof(uint32_t), 0);
}
