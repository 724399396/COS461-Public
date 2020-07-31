/*****************************************************************************
 * server-c.c                                                                 
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

#define QUEUE_LENGTH 10
#define RECV_BUFFER_SIZE 2048

/* server()
 * Open socket and wait for client to connect
 * Print received message to stdout
 * Return 0 on success, non-zero on failure
*/
int server(char *server_port) {
  int status, sockfd, numbytes, newfd;
  unsigned char buf[RECV_BUFFER_SIZE];
  struct sockaddr_storage their_addr;
  socklen_t sin_size;
  struct addrinfo hints, *servinfo, *p;
  int yes=1;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, server_port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %d\n", status);
    return 1;
  }

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
        perror("server: socket");
        continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  freeaddrinfo(servinfo);

  if (p == NULL) {
    fprintf(stderr, "server : failed to bind\n");
    return 1;
  }

  if (listen(sockfd, QUEUE_LENGTH) == -1) {
    perror("listen");
    return 1;
  }

  while (1) {
    sin_size = sizeof their_addr;
    newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    if (newfd == -1) {
      perror("accept");
      continue;
    }
    while (numbytes = recv(newfd, buf, RECV_BUFFER_SIZE-1, 0)) {
      fwrite(buf, sizeof(char), numbytes, stdout);
      fflush(stdout);
    }
    close(newfd);
  }
  close(sockfd);

  return 0;
}

/*
 * main():
 * Parse command-line arguments and call server function
*/
int main(int argc, char **argv) {
  char *server_port;

  if (argc != 2) {
    fprintf(stderr, "Usage: ./server-c [server port]\n");
    exit(EXIT_FAILURE);
  }

  server_port = argv[1];
  return server(server_port);
}
