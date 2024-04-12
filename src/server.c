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
  if (bind(server->listener, (const struct sockaddr*)&server->addr,
           sizeof(server->addr)) == -1) {
    error_and_exit("Couldn't bind listener socket.");
  }
  if (listen(server->listener, server->max_backlog) == -1) {
    error_and_exit("Couldn't set the socket to listen.");
  }
}

int accept_client(echo_server* server) {
  struct sockaddr_in client_addr;
  socklen_t addr_size = sizeof(client_addr);
  int accepted_sock_fd =
      accept4(server->listener, (struct sockaddr*)&client_addr, &addr_size,
              SOCK_CLOEXEC);
  if (accepted_sock_fd == -1) {
    error_and_exit("Couldn't accept incoming connection request from client");
  }
  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("Couldn't fork process");
  } else if (pid == 0) {
    echo(accepted_sock_fd);
    return -1;
  }
  return 0;
}

void echo(int socket_descriptor) {
  FILE* socket_file = fdopen(socket_descriptor, "r+");
  if (socket_file == NULL) {
    error_and_exit("Couldn't open socket as file stream");
  }
  char* recv_line = NULL;
  size_t recv_line_size = 0;
  while (getline(&recv_line, &recv_line_size, socket_file) != -1) {
    if (fputs(recv_line, socket_file) == EOF) {
      free(recv_line);
      error_and_exit("Couldn't send line");
    }
  }
  free(recv_line);
  if (fclose(socket_file) != 0) {
    error_and_exit("Couldn't close socket file");
  }
  error_and_exit("Connection closed");
}
