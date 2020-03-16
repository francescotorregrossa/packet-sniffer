#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"

#define UDP_HEADER_SIZE 8

struct udp_header
{
  word source_port;
  word destination_port;

  word length;
  word checksum;

  packet next;
};

typedef struct udp_header *udp_header;

udp_header prepare_udp_header(packet data);
dword size_udp_header(udp_header header);
void free_udp_header(udp_header header);

void describe_udp_header(udp_header header);