#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"

#define ETH_HEADER_SIZE 14

typedef struct
{
  byte a;
  byte b;
  byte c;
  byte d;
  byte e;
  byte f;
} mac_address;

struct eth_header
{
  mac_address destination_host;
  mac_address source_host;
  word type_code;

  packet next;
};

typedef struct eth_header *eth_header;

eth_header prepare_eth_header(packet data);
dword size_eth_header(eth_header header);
void free_eth_header(eth_header header);

string get_mac_address(mac_address address);
void describe_eth_header(eth_header header);