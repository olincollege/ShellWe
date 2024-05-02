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

/**
 * Prints information about an accepted connection.
 * 
 * Prints information about a client connection accepted by the server. 
 * Extracts and converts the IP address and port number from the
 * provided sockaddr_in structure and prints them to stdout.
 * 
 * @param client The sockaddr_in containing information about the client.
 */
static void print_connection(struct sockaddr_in client) {
  char ip_str[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(client.sin_addr), ip_str, INET_ADDRSTRLEN);
  printf("Connection accepted from %s:%d\n", ip_str, ntohs(client.sin_port));
}

int accept_client(server_t* server) {
  struct sockaddr_in client;
  socklen_t addr_size = sizeof(client);
  int connected_socket = accept(server->listener, (struct sockaddr*)&client,
                                 &addr_size);
  if (connected_socket < 0) {
    error_and_exit("Error accepting client");
  }
  print_connection(client);
  return connected_socket;
}

void log_client(int socket_fd, int* n_clients, int* client_sockets,
                pthread_mutex_t* clients_mutex) {
  pthread_mutex_lock(clients_mutex);
  client_sockets[(*n_clients)++] = socket_fd;
  pthread_mutex_unlock(clients_mutex);
}
