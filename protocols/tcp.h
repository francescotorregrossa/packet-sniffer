#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"

#define TCP_HEADER_SIZE 20

typedef struct
{
  byte
      cwr : 1,
      ece : 1,
      urg : 1,
      ack : 1,
      psh : 1,
      rst : 1,
      syn : 1,
      fin : 1;
} tcp_flags;

struct tcp_header
{
  word source_port;
  word destination_port;

  dword sequence_number;
  dword acknowldge_number;

  byte : 4,
      data_offset : 4;
  tcp_flags flags;
  word window_size;

  word checksum;
  word urgent;

  void *options;

  packet next;
};

typedef struct tcp_header *tcp_header;

tcp_header prepare_tcp_header(packet data);
void free_tcp_header(tcp_header header);

void describe_tcp_header(tcp_header header);