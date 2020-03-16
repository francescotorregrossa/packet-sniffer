#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"

#define IP_HEADER_SIZE 20

typedef struct
{
  byte a;
  byte b;
  byte c;
  byte d;
} ip_address;

struct ip_header
{
  byte
      header_length : 4,
      version : 4;
  byte type_of_service;
  word total_length;

  word id;
  word : 1,
      flag_do_not_fragment : 1,
      flag_more_fragments : 1,
      fragment_offset : 13;

  byte time_to_live;
  byte protocol;
  word checksum;

  ip_address source_address;
  ip_address destination_address;

  void *options;

  packet next;
};

typedef struct ip_header *ip_header;

ip_header prepare_ip_header(packet data);
dword size_ip_header(ip_header header);
void free_ip_header(ip_header header);

string get_ip_address(ip_address address);
void describe_ip_header(ip_header header);