// comando per visualizzare se la porta 8080 e'occupate "netstat -nap | grep :8080
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define LOCAL_SERVER_PORT 8580
#define MAX_MSG 100

int main(int argc, char *argv[])
{
  int sd, rc, n, cliLen;
  struct sockaddr_in cliAddr, remoteServAddr;
  char msg[MAX_MSG];
  sd = socket(AF_INET, SOCK_DGRAM, 0); /* socket creation */
  if (sd < 0)
  {
    printf("%s: cannot open socket \n", argv[0]);
    exit(1);
  } /* bind any port */
  cliAddr.sin_family = AF_INET;
  cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  cliAddr.sin_port = htons(LOCAL_SERVER_PORT);
  rc = bind(sd, (struct sockaddr *)&cliAddr, sizeof(cliAddr));
  if (rc < 0)
  {
    printf("%s: cannot bind port %d \n", argv[0], LOCAL_SERVER_PORT);
    exit(1);
  }
  while (1)
  {
    memset(msg, 0x0, MAX_MSG); // init buffer
    /* receive message */
    cliLen = sizeof(cliAddr);
    n = recvfrom(sd, msg, MAX_MSG, 0, (struct sockaddr *)&cliAddr, &cliLen);
    if (n < 0)
    {
      printf("%s: cannot receive data \n", argv[0]);
      continue;
    }
    printf("Message %s\n", msg);
    printf("from %s:PORT UDP%u \n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));
  } /* end of server infinite loop */
  return 1;
}