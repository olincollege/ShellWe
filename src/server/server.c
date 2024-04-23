#include "server.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/util.h"

server_t* init_server(struct sockaddr_in ip_addr, int max_backlog) {
  server_t* server = malloc(sizeof(server_t));
  server->listener = open_tcp_socket();
  server->addr = ip_addr;
  server->max_backlog = max_backlog;
  return server;
}

void listen_for_connections(server_t* server) {
  if (bind(server->listener, (const struct sockaddr*)&server->addr,
           sizeof(server->addr)) == -1) {
    error_and_exit("Bind failed");
  }
  if (listen(server->listener, server->max_backlog) == -1) {
    error_and_exit("Couldn't set socket to listen");
  }
}
