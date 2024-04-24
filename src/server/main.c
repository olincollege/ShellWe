#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../util/util.h"
#include "client_handler.h"
#include "server.h"

int main(void) {
  int client_sockets[MAX_CLIENTS];
  int* n_clients = malloc(sizeof(int));
  *n_clients = 0;
  pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_t thread_id;

  struct sockaddr_in client;

  // init server
  struct sockaddr_in server_addr = socket_address(SERVER_IP, PORT);
  server_t* server = init_server(server_addr, MAX_BACKLOG);
  listen_for_connections(server);

  socklen_t addr_size = sizeof(client);

  while (1) {
    int new_socket = accept_client(server);
    log_client(new_socket, client_sockets, &clients_mutex, n_clients);
    client_handler_args_t args = {new_socket, n_clients, client_sockets,
                                  &clients_mutex};
    assign_handler(&args);
  }
  free(n_clients);
  return 0;
}
