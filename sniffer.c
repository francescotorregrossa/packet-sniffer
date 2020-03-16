#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "protocols/ethernet.h"
#include "protocols/ip.h"
#include "protocols/icmp.h"
#include "protocols/tcp.h"
#include "protocols/udp.h"

#define PKT_LEN 8192
#define MAGIC1 65536
#define MAGIC2 768

#define MAX_TERMINAL_LINE_LENGTH 100

void analyze(packet buffer);
void print_separator();
void print_plaintext(packet data, dword size);

int main(int argc, char *argv[])
{

  packet buffer = malloc(PKT_LEN);
  memset(buffer, 0, PKT_LEN);

  int sock = socket(AF_PACKET, SOCK_RAW, MAGIC2);
  if (sock < 0)
    perror("Errore socket()");

  while (read(sock, buffer, PKT_LEN) > 0)
    analyze(buffer);

  return 0;
}

void analyze(packet buffer)
{
  print_separator();

  eth_header eh = prepare_eth_header(buffer);
  describe_eth_header(eh);

  if (eh->type_code == 8)
  {

    ip_header iph = prepare_ip_header(eh->next);
    describe_ip_header(iph);

    switch (iph->protocol)
    {
    case 1:
    {
      icmp_header icmph = prepare_icmp_header(iph->next);
      describe_icmp_header(icmph);

      print_separator();
      print_plaintext(icmph->next, iph->total_length - size_ip_header(iph) - size_icmp_header(icmph));

      free_icmp_header(icmph);
      break;
    }
    case 6:
    {
      tcp_header tcph = prepare_tcp_header(iph->next);
      describe_tcp_header(tcph);

      print_separator();
      print_plaintext(tcph->next, iph->total_length - size_ip_header(iph) - size_tcp_header(tcph));

      free_tcp_header(tcph);
      break;
    }
    case 17:
    {
      udp_header udph = prepare_udp_header(iph->next);
      describe_udp_header(udph);

      print_separator();
      print_plaintext(udph->next, iph->total_length - size_ip_header(iph) - size_udp_header(udph));

      free_udp_header(udph);
      break;
    }
    default:
      print_separator();
      print_plaintext(iph->next, iph->total_length - size_ip_header(iph));
      break;
    }

    free_ip_header(iph);
  }

  print_separator();
  printf("\n\n\n");

  free_eth_header(eh);
}

void print_separator()
{
  for (int i = 0; i < MAX_TERMINAL_LINE_LENGTH; i++)
    printf("-");
  printf("\n");
}

void print_plaintext(packet data, dword size)
{
  printf("Data Length: %d, Raw Data:\n", size);
  for (dword i = 0; i < size; i++)
  {
    if (i % 100 == 0 && i != 0)
      printf("\n");
    if (32 <= data[i] && data[i] <= 126)
      printf("%c", data[i]);
    else
      printf(".");
  }
  if (size > 0)
    printf("\n");
}