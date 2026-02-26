#include <stdio.h>
#include <stdlib.h>
#include "../transport/transport.h"
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>


void init_client(transport_config config, int id);
void init_clients(transport_config config, uint8_t clientsnumber);
