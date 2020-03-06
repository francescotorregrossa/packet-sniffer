#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#define PKT_LEN 8192 //lunghezza pacchetto
int main()
{
  struct iphdr *ip;   //IP header
  struct udphdr *udp; //UDP header
  struct in_addr s_ind;
  int sd;
  char *buffer; //buffer pacchetti IP + UDP
  char *dati;   //buffer dati pacchetto
  char *IpOrigin, *IpDest;
  IpOrigin = (char *)malloc(21);
  IpDest = (char *)malloc(21);
  buffer = (char *)malloc(PKT_LEN);
  memset(buffer, 0, PKT_LEN);
  sd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
  if (sd < 0)
    perror("Errore socket()");

  while (read(sd, buffer, 8192) > 0)
  {
    ip = (struct iphdr *)buffer; //header IP = inizio
    printf("-- IP Header --\n");
    printf("Lunghezza header: %d bytes\n", ip->ihl * 4);
    //32 bitoctets
    printf("Versione: %d\n", ip->version);
    printf("Priorità: %d\n", ip->tos);
    printf("Lunghezza pacchetto: %d bytes\n", ntohs(ip->tot_len));
    printf("ID: %d\n", ntohs(ip->id));
    printf("Offset: %d\n", ip->frag_off);
    printf("Time to live: %d\n", ip->ttl);
    printf("Protocollo trasporto: %d\n", ip->protocol);
    printf("Checksum: %d\n", ip->check);
    printf("Indirizzo origine in long int: %ul\n", ip->saddr);
    s_ind.s_addr = ip->saddr;
    IpOrigin = inet_ntoa(s_ind);
    printf("Indirizzo origine: %s\n", IpOrigin);
    s_ind.s_addr = ip->daddr;
    IpDest = inet_ntoa(s_ind);
    printf("Indirizzo destinazione in long int:%ul\n", ip->daddr);
    printf("Indirizzo destinazione: %s\n", IpDest);
    udp = (struct udphdr *)(buffer + ip->ihl * 4); //inizio header TCP = buffer spazio header IP
    printf("-- UDP Header --\n");
    printf("Porta sorgente: %d\n", ntohs(udp->source));
    printf("Porta destinazione: %d\n", ntohs(udp->dest));
    printf("Lunghezza pacchetto UDP: %d bytes\n", ntohs(udp->len));
    printf("Checksum: %d\n", udp->check);
    dati = (buffer + (ip->ihl * 4) + (sizeof(struct udphdr)));
    //inizio dati = header IP + header UDP
    printf("%s\n", dati);
    printf("**********************************************\n");
    memset(buffer, 0, PKT_LEN);
  }
  return 0;
}