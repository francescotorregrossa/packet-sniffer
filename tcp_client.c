#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>  // system defined identifiers.
#include <netinet/in.h> // internet address structure.
#include <sys/socket.h> // Need 4 socket(), bind(),
#include <netdb.h>
int main(int argc, char *argv[])
{
  int clientsocket, rispconect;
  struct sockaddr_in addr;
  char messaggio[80];
  short port;
  struct hostent *h;
  port = 8081;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  h = gethostbyname("localhost");
  if (h == 0)
  {
    printf("Gethostbyname fallito\n");
    exit(1);
  }
  bcopy(h->h_addr, &addr.sin_addr, h->h_length);
  printf("Server Address %s\n", inet_ntoa(addr.sin_addr));
  clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (clientsocket < 0)
  {
    printf("Client:errore creazione socket.\n");
    exit(0);
  }
  rispconect = connect(clientsocket, (struct sockaddr *)&addr, sizeof(addr));
  if (rispconect < 0)
  {
    printf("Errore nella connessione con il Server");
    exit(0);
  }
  printf("Messaggio da Inviare: ");
  gets(messaggio);
  send(clientsocket, messaggio, sizeof(messaggio), 0);
  return 0;
}