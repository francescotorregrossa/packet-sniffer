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

ip_header prepare_ip_header(packet data)
{
  ip_header header = malloc(sizeof(struct ip_header));
  memcpy(header, data, IP_HEADER_SIZE);

  unsigned int options_length = header->header_length * 4 - IP_HEADER_SIZE;
  if (options_length)
  {
    header->options = malloc(options_length);
    memcpy(header->options, data + IP_HEADER_SIZE, options_length);
  }
  else
    header->options = NULL;

  header->next = data + IP_HEADER_SIZE + options_length;

  return header;
}

void free_ip_header(ip_header header)
{
  if (header->options != NULL)
    free(header->options);
  free(header);
}

// Utility functions

string get_ip_address(ip_address address)
{
  int len = 8;

  if (address.a >= 10)
    len++;
  if (address.b >= 10)
    len++;
  if (address.c >= 10)
    len++;
  if (address.d >= 10)
    len++;

  if (address.a >= 100)
    len++;
  if (address.b >= 100)
    len++;
  if (address.c >= 100)
    len++;
  if (address.d >= 100)
    len++;

  string output = malloc(len);

  sprintf(output,
          "%d.%d.%d.%d",
          address.a, address.b,
          address.c, address.d);

  output[len - 1] = '\0';
  return output;
}

void describe_ip_header(ip_header header)
{
  string dh = get_ip_address(header->destination_address);
  string sh = get_ip_address(header->source_address);

  printf("\tIP Header:\n");
  printf("\t\t- Destination: %s, Source: %s, Protocol: %d\n", dh, sh, header->protocol);
  printf("\t\t- Version: %d, Header Length: %d, TTL: %d\n", header->version, header->header_length, header->time_to_live);

  free(dh);
  free(sh);
}