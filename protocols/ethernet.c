#include "ethernet.h"

eth_header prepare_ethernet_header(packet data)
{
  eth_header header = malloc(sizeof(struct eth_header));
  memcpy(header, data, ETH_HEADER_SIZE);
  header->next = data + ETH_HEADER_SIZE;
  return header;
}

void free_eth_header(eth_header header)
{
  free(header);
}

// Utility functions

string get_mac_address(mac_address address)
{
  string output = malloc(18);

  sprintf(output,
          "%02X:%02X:%02X:%02X:%02X:%02X",
          address.a, address.b,
          address.c, address.d,
          address.e, address.f);

  output[17] = '\0';
  return output;
}

void describe_eth_header(eth_header header)
{
  string dh = get_mac_address(header->destination_host);
  string sh = get_mac_address(header->source_host);

  printf("Ethernet Header:\n");
  printf("\t- Destination: %s, Source: %s, Protocol: %d\n", dh, sh, header->type_code);

  free(dh);
  free(sh);
}