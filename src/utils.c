#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "./common.h"
#include "./utils.h"

uint16_t compute_checksum(Header h, int* data) {
	/*
	 * le TCP calcule le checksum comme tel:
	 * res = somme de tout les mots de 16 bits
	 * res = ~res
	 * puis itérer dessus pour additioner la somme, et enfin envoyer la negation bitwise
	 */

	uint32_t sum = 0;
	sum += h.portnumber+h.datanumber+h.length;
	
	uint16_t* u16ptr = (uint16_t*) data;
	int u16length = (h.length*sizeof(int))/sizeof(uint16_t);

	for (int i = 0; i<u16length; i++) {
		sum += u16ptr[i];
	}

	return (uint16_t)~sum;
}
