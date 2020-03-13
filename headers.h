#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef byte *packet;
typedef char *string;

// Ethernet Header

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

typedef struct
{

    mac_address destination_host;
    mac_address source_host;
    word type_code;

    packet next;

} eth_header;

// IP Header

#define IP_HEADER_SIZE 20

typedef struct
{

    byte a;
    byte b;
    byte c;
    byte d;

} ip_address;

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

    ip_address source_address;
    ip_address destination_address;

    packet next;

} ip_header;

eth_header *prepare_ethernet_header(packet data);
ip_header *prepare_ip_header(packet data);

string get_mac_address(mac_address address);
string get_ip_address(ip_address address);

void describe_eth_header(eth_header *header);
void describe_ip_header(ip_header *ip_header);
