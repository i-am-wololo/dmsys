#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "./common.h"

// envoie l'entête et les données au tube
void serialize(Header* h, uint32_t* data, int pipe[2]) {
	write(pipe[1], &h->portnumber, sizeof(uint16_t));
	write(pipe[1], &h->datanumber, sizeof(uint16_t));
	write(pipe[1], &h->length, sizeof(uint16_t));
	write(pipe[1], &h->source, sizeof(uint16_t));
	write(pipe[1], &h->checksum, sizeof(uint16_t));
	write(pipe[1], data, sizeof(uint32_t)*h->length);
}

// gère seulement la lecture d'entête!
// le ramassage de données doit se faire manuellement après
Header* deserialize(int pipe[2]) {
	Header* header = malloc(sizeof(Header));
	read(pipe[0], &header->portnumber, sizeof(uint16_t));
	read(pipe[0], &header->datanumber, sizeof(uint16_t));
	read(pipe[0], &header->length, sizeof(uint16_t));
	read(pipe[0], &header->source, sizeof(uint16_t));
	read(pipe[0], &header->checksum, sizeof(uint16_t));
	return header;
}


void print_header(Header* h) {
	printf(
			"lecture d'entête:\n"
			"portnumber: %hu\n"
			"datanumber: %hu\n"
			"length: %hu\n"
			"checksum: %hu\n"
			"source: %hu\n",
			h->portnumber, h->datanumber, h->length, h->checksum, h->source
		);
}
