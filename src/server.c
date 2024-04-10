#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
#define _GNU_SOURCE

#include "server.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

echo_server* make_echo_server(struct sockaddr_in ip_addr, int max_backlog) {
  echo_server* server = malloc(sizeof(echo_server));
  server->listener = open_tcp_socket();
  server->addr = ip_addr;
  server->max_backlog = max_backlog;
  return server;
}

void free_echo_server(echo_server* server) {
  close_tcp_socket(server->listener);
  free(server);
}

void listen_for_connections(echo_server* server) {
  if (bind(server->listener, (struct sockaddr*)&server->addr,
           sizeof(server->addr))) {
    error_and_exit("Failed to bind to socket");
  }
  if (listen(server->listener, server->max_backlog) == -1) {
    error_and_exit("Failed to listen for connections");
  }
}

int accept_client(echo_server* server) {
  struct sockaddr_storage client_addr;
  unsigned int addr_size = sizeof(client_addr);
  int connect_d = accept4(server->listener, (struct sockaddr*)&client_addr,
                          &addr_size, SOCK_CLOEXEC);
  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("Error forking process");
  }
  if (!pid) {  // child process
    while (1) {
      echo(connect_d);
    }
    return -1;
  }
  return 0;
}

const int BUFFER = 255;

void echo(int socket_descriptor) {
  char buf[BUFFER];
  char* msg = buf;
  size_t slen = sizeof(msg);
  ssize_t chars = recv(socket_descriptor, msg, slen, 0);
  while ((chars > 0) && (msg[chars - 1] != '\n')) {
    msg += chars;
    slen -= (size_t)chars;
    chars = recv(socket_descriptor, msg, slen, 0);
  }
  if (chars < 0) {
    error_and_exit("Error recieving from socket");
  } else if (chars == 0) {
    buf[0] = '\n';
  } else {
    msg[chars - 1] = '\n';
  }

  send(socket_descriptor, msg, (size_t)strlen(msg), 0);
}
