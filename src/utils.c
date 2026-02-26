#include "./utils.h"

uint16_t compute_checksum(Header* h, int* data) {
	/*
	 * le TCP calcule le checksum comme tel:
	 * res = somme de tout les mots de 16 bits
	 * addition des bits qui dépassent
	 * res = ~res
	 */

	uint32_t sum = 0;
	sum += h->portnumber+h->datanumber+h->length+h->source;
	
	uint16_t* u16ptr = (uint16_t*) data;
	int u16length = (h->length*sizeof(int))/sizeof(uint16_t);

	for (int i = 0; i<u16length; i++) {
		sum += u16ptr[i];
	}

	// retenue
	// tant qu'il y a quelque chose après les 16 bits, 
	// les additionner dans les premiers 16 bits de la somme.
	while (sum >> 16) sum = (sum & 0xFFFF) + (sum>>16);
	return (uint16_t)~sum;
}
