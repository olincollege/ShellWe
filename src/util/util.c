#include "util.h"

#include <stdio.h>   // perror
#include <stdlib.h>  // exit, EXIT_FAILURE
#include <unistd.h>  // close

void error_and_exit(const char* error_msg) {
  perror(error_msg);
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  exit(EXIT_FAILURE);
}

int open_tcp_socket(void) {
  int socket_d = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket_d == -1) {
    error_and_exit("Couldn't open TCP socket.");
  }
  return socket_d;
}

void close_tcp_socket(int socket_) {
  if (close(socket_) == -1) {
    error_and_exit("Couldn't close TCP socket.");
  }
}

struct sockaddr_in socket_address(in_addr_t addr, in_port_t port) {
  struct sockaddr_in sockaddr = {.sin_addr = {htonl(addr)},
                                 .sin_port = htons(port),
                                 .sin_family = AF_INET};
  return sockaddr;
}

FILE* get_socket_file(int socket) {
  FILE* socket_file = fdopen(socket, "w+");
  if (socket_file == NULL) {
    error_and_exit("Couldn't open socket as file stream");
  }
  return socket_file;
}

void remove_newline(char* str) {
  int newIdx = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] != '\n') {
      str[newIdx++] = str[i];
    }
  }
  str[newIdx] = '\0';
}
