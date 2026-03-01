
#include <stdint.h>
uint16_t compute_server_checksum(uint32_t* databuf, uint32_t data_length) {
	/*
	 * le TCP calcule le checksum comme tel:
	 * res = somme de tout les mots de 16 bits
	 * addition des bits qui dépassent
	 * res = ~res
	 */

	uint32_t sum = 0;
	
	uint16_t* u16ptr = (uint16_t*) databuf;
	int u16length =	data_length*2; 

	for (int i = 0; i<u16length; i++) {
		sum += u16ptr[i];
	}

	// retenue
	// tant qu'il y a quelque chose après les 16 bits, 
	// les additionner dans les premiers 16 bits de la somme.
	while (sum >> 16) sum = (sum & 0xFFFF) + (sum>>16);
	return (uint16_t)~sum;
}
