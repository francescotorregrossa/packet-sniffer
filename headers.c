#include "headers.h"

eth_header* prepare_ethernet_header(packet data) {
    eth_header* header = malloc(sizeof(eth_header));
    memcpy(header, data, sizeof(eth_header));
    // header->type_code = htons(header->type_code); // fa buggare tutto così
    return header;
}

ip_header* prepare_ip_header(packet data) {
    ip_header* header = malloc(sizeof(ip_header));
    memcpy(header, data, sizeof(ip_header));
    return header;
}


string get_mac_address(byte address[6]) {
    string output = malloc(18);
    sprintf(output,
        "%02x:%02x:%02x:%02x:%02x:%02x", 
        address[0], address[1], 
        address[2], address[3], 
        address[4], address[5]);
    output[17] = '\0';
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
