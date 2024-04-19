#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../util/util.h"
#include "client_handler.h"

int main(void) {
  int client_sockets[MAX_CLIENTS];
  int* n_clients = malloc(sizeof(int));
  *n_clients = 0;
  pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_t thread_id;

  struct sockaddr_in client;
  int socket_desc = open_tcp_socket();
  puts("Socket created");

  struct sockaddr_in server = socket_address(INADDR_LOOPBACK, PORT);

  if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("bind failed. Error");
    return 1;
  }
  puts("bind done");

  listen(socket_desc, 3);

  puts("Waiting for incoming connections...");
  socklen_t addr_size = sizeof(client);

  while (1) {
    int new_socket = accept(socket_desc, (struct sockaddr*)&client, &addr_size);
    if (new_socket < 0) {
      perror("accept failed");
      break;
    }
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client.sin_addr), ip_str, INET_ADDRSTRLEN);
    printf("Connection accepted from %s:%d\n", ip_str, ntohs(client.sin_port));

    pthread_mutex_lock(&clients_mutex);
    client_sockets[(*n_clients)++] = new_socket;
    pthread_mutex_unlock(&clients_mutex);
    client_handler_args_t args = {new_socket, n_clients, client_sockets,
                                  &clients_mutex};
    if (pthread_create(&thread_id, NULL, handle_client, (void*)&args) != 0) {
      perror("could not create thread");
      free(n_clients);
      return 1;
    }

    puts("Handler assigned");
  }
  free(n_clients);
  return 0;
}
