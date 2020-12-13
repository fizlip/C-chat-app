#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

int main(){

  int fd, newsockfd, n;
  char buffer[256];
  int clilen;
  struct sockaddr_in serv_addr, cli_addr;
  
  // Initialize server socket
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(20001);
 
  // Create new socket
  fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *) &serv_addr, sizeof(serv_addr));

  // Bind server socket to fd
  if(bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
    perror("Error on bind!");
    exit(0);
  };

  listen(fd, 5);

  clilen = sizeof(cli_addr);
  newsockfd = accept(fd, (struct sockaddr *)&cli_addr, &clilen);

  if(newsockfd < 0){
    perror("ERROR on accept");
    exit(0);
  }
  printf("New client connected\r\n");

  bzero(buffer, 256);
  n = read(newsockfd, buffer, 256);
  if(n < 0){
    perror("ERROR reading from the socket");
    exit(0);
  }

  printf("Message: %s", buffer);

  n = write(newsockfd, "Got it", strlen("Got it"));
  if(n < 0){
    perror("ERROR when writing to socket");
  }
  close(newsockfd);
  close(fd);
  return 0;
}
