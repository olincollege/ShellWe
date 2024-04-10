#include "util.h"

#include <stdint.h>  // uint16_t
#include <stdio.h>   // perror
#include <stdlib.h>  // exit, EXIT_FAILURE
#include <unistd.h>  // close

const uint16_t PORT = 4242;

void error_and_exit(const char* error_msg) {
  perror(error_msg);
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  exit(EXIT_FAILURE);
}

int open_tcp_socket(void) {
  int listener_d = socket(AF_INET, SOCK_STREAM, 0);
  if (listener_d == -1) {
    error_and_exit("Failed to create socket");
  }
  return listener_d;
}

void close_tcp_socket(int socket_) {
  if (close(socket_) == -1) {
    error_and_exit("Failed to close socket");
  }
}

struct sockaddr_in socket_address(in_addr_t addr, in_port_t port) {
  struct sockaddr_in sockaddr = {
      .sin_family = PF_INET,
      .sin_port = (in_port_t)htons(port),
      .sin_addr = {.s_addr = htonl(addr)},
  };
  return sockaddr;
}
