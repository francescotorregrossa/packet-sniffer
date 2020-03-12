#include "headers.h"

eth_header* prepare_ethernet_header(packet data) {
    eth_header* header = malloc(sizeof(eth_header));
    memcpy(header, data, sizeof(eth_header));
    return header;
}

ip_header* prepare_ip_header(packet data) {
    ip_header* header = malloc(sizeof(ip_header));
    memcpy(header, data, sizeof(ip_header));
    return header;
}

string get_mac_address(byte address[6]) {
    string output = malloc(18);

    string pin = address;
    string hex = "0123456789ABCDEF";
    string pout = output;

    for (int i = 0; i < 5; i++){
        *pout++ = hex[(*pin>>4)&0xF];
        *pout++ = hex[(*pin++)&0xF];
        *pout++ = ':';
    }
    *pout++ = hex[(*pin>>4)&0xF];
    *pout++ = hex[(*pin)&0xF];
    *pout = 0;

    output[17] = '\0';
    return output;
}

string get_ip_address(ip_address address) {
    int len = 8; // 3 dots + 4 numbers + \0
    
    if (address.a >= 10) len++;
    if (address.b >= 10) len++;
    if (address.c >= 10) len++;
    if (address.d >= 10) len++;

    if (address.a >= 100) len++;
    if (address.b >= 100) len++;
    if (address.c >= 100) len++;
    if (address.d >= 100) len++;

    string output = malloc(len);

    sprintf(output,
        "%d.%d.%d.%d", 
        address.a, address.b, 
        address.c, address.d);

    output[len - 1] = '\0';
    return output;
}


void describe_eth_header(eth_header* header) {
    string dh = get_mac_address(header->destination_host);
    string sh = get_mac_address(header->source_host);

    printf("Ethernet Header:\n");
    printf("\t- Destination: %s, Source: %s, Protocol: %d\n", dh, sh, header->type_code);
    
    free(dh);
    free(sh);
}

void describe_ip_header(ip_header* header) {
    string dh = get_ip_address(header->destination_address);
    string sh = get_ip_address(header->source_address);

    printf("\tIP Header:\n");
    printf("\t\t- Destination: %s, Source: %s\n", dh, sh);

    free(dh);
    free(sh);
}
