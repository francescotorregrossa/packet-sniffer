#include "headers.h"

eth_header* prepare_ethernet_header(packet data) {
    eth_header* header = malloc(sizeof(eth_header));
    memcpy(header, data, sizeof(eth_header));
    header->type_code = htons(header->type_code);
    return header;
}

ip_header* prepare_ip_header(packet data) {
    ip_header* header = malloc(sizeof(ip_header));
    memcpy(header, data, sizeof(ip_header));
    return header;
}


string describe_eth_header(eth_header* header) {
    printf("dest: ");
    for (int i = 0; i < 6; i++)
        printf("%04x", header->destination_host[i]);
    printf("\n");
    return "";
}
