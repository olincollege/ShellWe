#include <arpa/inet.h>  // sockaddr_in
#include <stdio.h>

#include "server.h"  // echo_server, related functions
#include "util.h"    // socket_address, PORT

int main(void) {
  printf("Welcome to ShellWe!\n");
  accepted_socket acceptedSockets[BACKLOG_SIZE];
  int acceptedSocketsCount = 0;

  struct sockaddr_in server_addr = socket_address(INADDR_LOOPBACK, PORT);

  int server_socketFD = open_tcp_socket();

  if (bind(server_socketFD, (struct sockaddr*)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("Socket bound failed");
  }

  if (listen(server_socketFD, BACKLOG_SIZE) == -1) {
    perror("Socket listen failed");
  }

  start_accepting(server_socketFD);

  shutdown(server_socketFD, SHUT_RDWR);

  return 0;
}
