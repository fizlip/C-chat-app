#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h> 
#include <string.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(){
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    perror("Could not create socket!");
    exit(0);
  }

  server = gethostbyname("localhost");
  if(server == NULL){
    perror("No such host");
    exit(0);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
  serv_addr.sin_port = htons(49145);

  int in = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  printf("conn: %d \r\n", in);
  if (in < 0){
    perror("ERROR");
    exit(0);
  }

  printf("Please enter a message: ");
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);
  n = write(sockfd, buffer, strlen(buffer));
  if(n < 0){
    perror("ERROR writing to socket");
    exit(0);
  }

  bzero(buffer,256);

  n = read(sockfd, buffer, 255);
  if(n < 0){
    perror("ERROR reading from socket");
    exit(0);
  }

  printf("%s\n", buffer);
  close(sockfd);

  exit(0);
}
