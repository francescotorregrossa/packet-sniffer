#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;
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

struct eth_header
{
    mac_address destination_host;
    mac_address source_host;
    word type_code;

    packet next;
};

typedef struct eth_header * eth_header;

// IP Header

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

    void* options;

    packet next;
};

typedef struct ip_header * ip_header;

// ICMP Header

#define ICMP_HEADER_SIZE 8

struct icmp_header
{
    byte type;
    byte code;
    word checksum;

    dword options;

    packet next;
};

typedef struct icmp_header * icmp_header;

// TCP Header

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
        data_offset: 4;
    tcp_flags flags;
    word window_size;

    word checksum;
    word urgent;

    void* options;

    packet next;
};

typedef struct tcp_header * tcp_header;

// UDP Header

#define UDP_HEADER_SIZE 8

struct udp_header
{
    word source_port;
    word destination_port;

    word length;
    word checksum;

    packet next;
};

typedef struct udp_header * udp_header;

eth_header prepare_ethernet_header(packet data);
ip_header prepare_ip_header(packet data);
icmp_header prepare_icmp_header(packet data);
tcp_header prepare_tcp_header(packet data);
udp_header prepare_udp_header(packet data);

string get_mac_address(mac_address address);
string get_ip_address(ip_address address);
word switch_encoding(word w);

void describe_eth_header(eth_header header);
void describe_ip_header(ip_header ip_header);
void describe_icmp_header(icmp_header ip_header);
void describe_tcp_header(tcp_header tcp_header);
void describe_udp_header(udp_header udp_header);

void free_eth_header(eth_header header);
void free_ip_header(ip_header header);
void free_icmp_header(icmp_header header);
void free_tcp_header(tcp_header header);
void free_udp_header(udp_header header);
