/*****************************************************************************
 * client-c.c                                                                 
 * Name:
 * NetId:
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

#define SEND_BUFFER_SIZE 2048

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  } 
  return &(((struct sockaddr_in6*)sa)->sin6_addr); 
}

/* client()
 * Open socket and send message from stdin.
 * Return 0 on success, non-zero on failure
*/
int client(char *server_ip, char *server_port)
{
  int sockfd, status, numbytes;
  struct addrinfo hints, *servinfo, *p;
  unsigned int buf[SEND_BUFFER_SIZE];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(server_ip, server_port, &hints, &servinfo)) != 0)
  {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next)
  {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
    {
      fprintf(stderr, "client socket error");
      continue;
    }
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
    {
      close(sockfd);
      fprintf(stderr, "client connect error");
      continue;
    }
    break;
  }

  if (p == NULL)
  {
    fprintf(stderr, "client: failed to connect\n");
    return 2;
  }

  freeaddrinfo(servinfo);

  while (numbytes = fread(buf, 1, SEND_BUFFER_SIZE, stdin)) {
    if (send(sockfd, buf, numbytes, 0) == -1) {
        perror("send");
    }
  }
  close(sockfd);

  return 0;
}

/*
 * main()
 * Parse command-line arguments and call client function
*/
int main(int argc, char **argv)
{
  char *server_ip;
  char *server_port;

  if (argc != 3)
  {
    fprintf(stderr, "Usage: ./client-c [server IP] [server port] < [message]\n");
    exit(EXIT_FAILURE);
  }

  server_ip = argv[1];
  server_port = argv[2];
  return client(server_ip, server_port);
}
