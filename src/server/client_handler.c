#include "client_handler.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/util.h"

void assign_handler(client_handler_args_t* handler_args) {
  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, handle_client, (void*)handler_args) !=
      0) {
    error_and_exit("Error creating thread");
  }
  puts("Handler Assigned");
}

void* handle_client(void* arg) {
  // Detach the current thread.
  pthread_detach(pthread_self());

  client_handler_args_t* handler_args = (client_handler_args_t*)arg;

  int* client_sockets = handler_args->client_sockets;
  int sock = handler_args->sock;
  int* n_clients = handler_args->n_clients;
  pthread_mutex_t* clients_mutex = handler_args->clients_mutex;

  char buffer[BUFFER_SIZE];
  ssize_t read_size = 0;

  // Receive messages from client
  while ((read_size = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
    buffer[read_size] = '\0';  // Null terminate the message

    // Lock the clients array
    pthread_mutex_lock(clients_mutex);
    for (int i = 0; i < *n_clients; i++) {
      if (client_sockets[i] != sock) {
        if (send(client_sockets[i], buffer, strlen(buffer), 0) < 0) {
          perror("send failed");
          continue;
        }
      }
    }
    pthread_mutex_unlock(clients_mutex);
  }

  if (read_size == 0) {
    puts("Client disconnected");
    (void)fflush(stdout);
  } else if (read_size == -1) {
    perror("recv failed");
  }

  // Remove the client
  pthread_mutex_lock(clients_mutex);
  for (int i = 0; i < *n_clients; i++) {
    if (client_sockets[i] == sock) {
      while (i < *n_clients - 1) {
        client_sockets[i] = client_sockets[i + 1];
        i++;
      }
      (*n_clients)--;
      break;
    }
  }
  pthread_mutex_unlock(clients_mutex);

  close(sock);
  return NULL;
}
