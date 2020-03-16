#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"

#define ICMP_HEADER_SIZE 8

struct icmp_header
{
  byte type;
  byte code;
  word checksum;

  dword options;

  packet next;
};

typedef struct icmp_header *icmp_header;

icmp_header prepare_icmp_header(packet data);
dword size_icmp_header(icmp_header header);
void free_icmp_header(icmp_header header);

void describe_icmp_header(icmp_header header);