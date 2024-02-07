#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8000
#define BUFFER_SIZE 1024
#define IPADDR "127.0.0.1"

int main() {
  // create char buffer
  char buffer[BUFFER_SIZE] = {0};

  // try to create client socket, exit on failure
  int client_socket;
  if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket creation failed.");
    exit(EXIT_FAILURE);
  }

  // create server address struct for connection string
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  
  // convert standard IP address to binary 
  if (inet_pton(AF_INET, IPADDR, &server_address.sin_addr) <= 0) {
    perror("inet_pton failed.");
    exit(EXIT_FAILURE);
  }

  // try to connect to addr
  if ((connect(client_socket, (struct sockaddr_in*)&server_address, sizeof(server_address))) < 0) {
    perror("Connection failed.");
    exit(EXIT_FAILURE);
  }
  // print connection success
  printf("Connected to server on port %d\n", PORT);

  // Loop for communication
  while (1) {
      // Get user input
      printf("Client: ");
      fgets(buffer, sizeof(buffer), stdin);

      // Send data to server
      send(client_socket, buffer, strlen(buffer), 0);

      // Read data from server
      if (recv(client_socket, buffer, sizeof(buffer), 0) <= 0) {
          perror("Receive failed");
          exit(EXIT_FAILURE);
      }

      printf("Server: %s\n", buffer);
  }

  // clean up, close socket and return
  close(client_socket);
  return 0;
}

