#include "headers.h"

eth_header prepare_ethernet_header(packet data)
{
    eth_header header = malloc(ETH_HEADER_SIZE + sizeof(packet));
    memcpy(header, data, ETH_HEADER_SIZE);
    header->next = data + ETH_HEADER_SIZE;
    return header;
}

ip_header prepare_ip_header(packet data)
{
    ip_header header = malloc(IP_HEADER_SIZE + sizeof(packet));
    memcpy(header, data, IP_HEADER_SIZE);
    header->next = data + header->header_length * 4;
    return header;
}

icmp_header prepare_icmp_header(packet data)
{
    icmp_header header = malloc(ICMP_HEADER_SIZE + sizeof(packet));
    memcpy(header, data, ICMP_HEADER_SIZE);
    header->next = data + ICMP_HEADER_SIZE;
    return header;
}

tcp_header prepare_tcp_header(packet data)
{
    tcp_header header = malloc(TCP_HEADER_SIZE + sizeof(packet));
    memcpy(header, data, TCP_HEADER_SIZE);
    header->next = data + header->data_offset * 4;
    return header;
}

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

void describe_eth_header(eth_header header)
{
    string dh = get_mac_address(header->destination_host);
    string sh = get_mac_address(header->source_host);

    printf("Ethernet Header:\n");
    printf("\t- Destination: %s, Source: %s, Protocol: %d\n", dh, sh, header->type_code);

    free(dh);
    free(sh);
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

void describe_icmp_header(icmp_header header)
{
    printf("\t\tICMP Header:\n");
    printf("\t\t\t- Type: %d, Code: %d\n", header->type, header->code);
}

void describe_tcp_header(tcp_header header)
{
    printf("\t\tTCP Header:\n");
    printf("\t\t\t- Source Port: %hu, Destination Port: %hu\n", header->source_port, header->destination_port);
    printf("\t\t\t- Sequence Number: %u, Acknowledgment: %u\n", header->sequence_number, header->acknowldge_number);
    printf("\t\t\t- URG: %u, ACK: %u, FIN: %u\n", header->flags.urg, header->flags.ack, header->flags.fin);
}
