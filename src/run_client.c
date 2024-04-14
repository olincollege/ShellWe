#include <arpa/inet.h>  // INADDR_LOOPBACK, inet_ntop
#include <stdio.h>      // fprintf
#include <string.h>

#include "client.h"
#include "util.h"

const socklen_t MAX_IP_ADDR_LEN = 16;

int main(void) {
  // Open a TCP socket to connect to the server.
  int socket_descriptor = open_tcp_socket();
  (void)fprintf(stderr, "Client socket descriptor: %d\n", socket_descriptor);

  // Connect to the server running on this machine (127.0.0.1).
  struct sockaddr_in server_addr = socket_address(INADDR_LOOPBACK, PORT);
  try_connect(socket_descriptor, server_addr);
  // To print an IP address as a string, we need to allocate a buffer.
  char addr_string[MAX_IP_ADDR_LEN];
  (void)fprintf(
      stderr, "Connected to server at %s, port %d\n",
      inet_ntop(AF_INET, &server_addr.sin_addr, addr_string, MAX_IP_ADDR_LEN),
      PORT);

  char* line = NULL;
  size_t line_size = 0;
  while (1) {
    ssize_t char_count = getline(&line, &line_size, stdin);

    if (char_count > 0) {
      if (strcmp(line, "exit\n") == 0) {
        break;
      }

      ssize_t txt_sent = send(socket_descriptor, line, line_size, 0);
    }
  }

  // Clean up and exit.
  close_tcp_socket(socket_descriptor);
  return 0;
}
