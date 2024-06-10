#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"


void transmit_file(FILE *file, int socket_fd){
  int n;
  char buffer[1024] = {0};
 
  while(fgets(buffer, 1024, file) != NULL) {
    if (send(socket_fd, buffer, sizeof(buffer), 0) == -1) {
      perror("[-]Error while sending file.");
      exit(1);
    }
    memset(buffer, 0, 1024);
  }
}

 int main(){
  const char *server_ip = "127.0.0.1";
  int server_port = 8080;
  int socket_fd;

  struct sockaddr_in server_address;
  FILE *file;
  const char *filename = "input.txt";
 
 //Create a UDP socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(socket_fd < 0) {
    perror("Could not create socket");
    exit(1);
  }
  printf("Server socket created successfully.\n");
 
 //configure the server adress structure to which will send data
  server_address.sin_family = AF_INET;
  server_address.sin_port = (server_port);
  server_address.sin_addr.s_addr = inet_addr(server_ip);
 
 //Bind the listen socket to the client address
  if(connect(socket_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
 printf("[+]Connected to Server.\n");
 
 //open file for reading
  file = fopen(filename, "r");
  if (file == NULL) {
    perror("[-]Unable to open file");
    exit(1);
  }
 
 //transmits file
  transmit_file(file, socket_fd);
  printf("[+]File data transmitted successfully.\n");
 
  printf("[+]Closing the socket connection.\n");
  close(socket_fd);
 
  return 0;
}

