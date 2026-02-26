#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

// envoie l'entête au tube
void serialize(Header* h, int* data, int* pipe) {
	write(pipe[1], &h->portnumber, sizeof(uint16_t));
	write(pipe[1], &h->datanumber, sizeof(uint16_t));
	write(pipe[1], &h->length, sizeof(uint16_t));
	write(pipe[1], &h->checksum, sizeof(uint16_t));
	write(pipe[1], data, sizeof(int)*h->length);
}

// gère seulement la lecture d'entête!
Header* deserialize(int *pipe) {
	Header* header = malloc(sizeof(Header));
	read(pipe[0], &header->portnumber, sizeof(uint16_t));
	read(pipe[0], &header->datanumber, sizeof(uint16_t));
	read(pipe[0], &header->length, sizeof(uint16_t));
	read(pipe[0], &header->checksum, sizeof(uint16_t));
	return header;
}


void print_header(Header* h) {
	printf(
			"lecture d'entête:"
			"portnumber: %hu"
			"datanumber: %hu"
			"length: %hu"
			"checksum: %hu"
			"source: %hu",
			h->portnumber, h->datanumber, h->length, h->checksum, h->source
		);
}
