#include "../transport/transport.h"
#include <stdint.h>


uint16_t compute_server_checksum(uint32_t* databuf, uint32_t data_length);

#ifdef PIPEBUILD
void init_server(
		transport_config config,
		int semid,
		int16_t* ports
);

#endif

#ifdef SHMBUILD
void init_servers(transport_config config, int N, int16_t* port_map);

#endif


#ifdef MSGQUEUEBUILD
void init_servers(transport_config config, int N);
#endif


