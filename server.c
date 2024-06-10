#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "utils.h"
 
void receive_file(int socket_fd){
  int bytes_received;
  FILE *file_pointer;
  const char *output_file = "output.txt";
  char buffer[1024];
 
 //Open the target file for writing (always write to output.txt)
  file_pointer = fopen(output_file, "w");
  
  while (1) {
    bytes_received = recv(socket_fd, buffer, 1024, 0);
    if (bytes_received <= 0){
      break;
      return;
    }
    fprintf(file_pointer, "%s", buffer);
    memset(buffer, 0, 1024);
  }
  return;
}
 
int main(){
  const char *server_ip = "127.0.0.1";
  int server_port = 8080;
 
  int server_socket, client_socket;
  struct sockaddr_in server_address, client_server;
  socklen_t client_address_size;
  char buffer[1024];
 
  //Create UDP socket 
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(server_socket < 0) {
    perror("[-] Error creating socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");
 
 //Configure the server address structure
  server_address.sin_family = AF_INET;
  server_address.sin_port = server_port;
  server_address.sin_addr.s_addr = inet_addr(server_ip);
 
 //Bind the listen socket to the server address
  if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    perror("[-] Error binding socket");
    exit(1);
  }
  printf("[+] Socket binding successfull.\n");
 
  if(listen(server_socket, 10) == 0){
 printf("[+] Listening for incoming connections....\n");
 }else{
 perror("[-] Error in listening");
    exit(1);
 }
 
  client_address_size = sizeof(client_server);
  client_socket = accept(server_socket, (struct sockaddr*)&client_server, &client_address_size);
  
  if (client_socket < 0){
     perror("[-] Error accepting connection");
     exit(1);
  }
      perror("[-] Connection accepted.\n");
  
  receive_file(client_socket);
  printf("[+]Data written to file successfully.\n");
 
  return 0;
}
