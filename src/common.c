#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./common.h"

// envoie l'entête et les données au tube
// on a remarqué que sans synchronisation, plusieurs clients peuvent écrire en même temps
// ainsi corrompant les données. deux solutions:
// 1- condenser tout les writes en un seul (solution choisie)
// 2- introduire des semaphores pour bloquer les autres clients quand un est en train d'écrire
// on a pas besoin de faire pareil pour deserialize
void serialize(Header* h, uint32_t* data, int pipe[2]) {
	size_t taille = sizeof(Header) + (h->length*sizeof(uint32_t));
	char* buffer = malloc(taille);
	if (buffer) {
		memcpy(buffer, h, sizeof(Header));
		memcpy(buffer+sizeof(Header), data, h->length*sizeof(uint32_t));
	}
	write(pipe[1], buffer, taille);
	// write(pipe[1], &h->portnumber, sizeof(uint16_t));
	// write(pipe[1], &h->datanumber, sizeof(uint16_t));
	// write(pipe[1], &h->length, sizeof(uint16_t));
	// write(pipe[1], &h->source, sizeof(uint16_t));
	// write(pipe[1], &h->checksum, sizeof(uint16_t));
	// write(pipe[1], data, sizeof(uint32_t)*h->length);
	free(buffer);
}

// gère seulement la lecture d'entête!
// le ramassage de données doit se faire manuellement après
Header* deserialize(int pipe[2]) {
	Header* header = malloc(sizeof(Header));
	int n = read(pipe[0], &header->portnumber, sizeof(uint16_t));
	if (n <= 0) {
			free(header);
			return NULL;
	}
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


void P(int semid) {
    struct sembuf op = {0, -1, 0}; // Décrémente (attend que le sémaphore soit > 0)
    semop(semid, &op, 1);
}

void V(int semid) {
    struct sembuf op = {0, 1, 0};  // Incrémente (libère)
    semop(semid, &op, 1);
}

void Pmult(int semid, int sem_index) {
	struct sembuf op = {
		.sem_num = sem_index,
		.sem_op = -1,
		.sem_flg = 0
	};
	semop(semid, &op, 1);
}

void Vmult(int semid, int sem_index) {
	struct sembuf op = {
		.sem_num = sem_index,
		.sem_op = +1,
		.sem_flg = 0
	};
	semop(semid, &op, 1);
}
