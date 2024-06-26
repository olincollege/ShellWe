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
  // init server
  struct sockaddr_in server_addr = socket_address(SERVER_IP, PORT);
  server_t* server = init_server(server_addr, MAX_BACKLOG);
  listen_for_connections(server);

  // init variables for tracking clients
  int client_sockets[MAX_CLIENTS];
  int* n_clients = malloc(sizeof(int));
  *n_clients = 0;
  pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

  while (1) {
    int new_socket = accept_client(server);
    if (new_socket < 0) {
      continue;
    }
    log_client(new_socket, n_clients, client_sockets, &clients_mutex);

    // spawn thread
    client_handler_args_t args = {new_socket, n_clients, client_sockets,
                                  &clients_mutex};
    assign_handler(&args);
  }
  return 0;
}
