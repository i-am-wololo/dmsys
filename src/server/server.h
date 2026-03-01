#include "../transport/transport.h"
#include <stdint.h>


uint16_t compute_server_checksum(uint32_t* databuf, uint32_t data_length);

void init_server(
		transport_config config,
#ifdef PIPEBUILD
		int semid,
#endif
		int16_t* ports
);

