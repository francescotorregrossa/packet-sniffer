#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

typedef char byte;
typedef unsigned short word;
typedef byte *packet;
typedef char *string;

// Ethernet Header

typedef struct
{

    byte destination_host[6];
    byte source_host[6];
    word type_code;

} eth_header;

// IP Header

typedef struct
{

    byte
        header_length : 4,
        version : 4;
    byte type_of_service;
    word total_length;

    word id;
    word fragment_offset;

    byte time_to_live;
    byte protocol;
    word checksum;

    byte source_address[4];
    byte destination_address[4];

} ip_header;

eth_header *prepare_ethernet_header(packet data);
ip_header *prepare_ip_header(packet data);

string get_mac_address(byte address[6]);
string get_ip_address(byte address[4]);

void describe_eth_header(eth_header *header);
void describe_ip_header(ip_header *ip_header);
