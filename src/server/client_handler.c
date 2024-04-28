#include "client_handler.h"

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/util.h"

void assign_handler(client_handler_args_t* handler_args) {
  pthread_t thread_id = 0;
  if (pthread_create(&thread_id, NULL, handle_client, (void*)handler_args) !=
      0) {
    puts("Issue creating handler thread, continuing");
  }
  puts("Handler Assigned");
}

void relay_message(char* message, ssize_t read_size, int* client_sockets,
                   int self_socket, const int* n_clients,
                   pthread_mutex_t* clients_mutex) {
  pthread_mutex_lock(clients_mutex);
  message[read_size] = '\0';  // Null terminate the message
  // Lock the clients array
  for (int i = 0; i < *n_clients; i++) {
    if (client_sockets[i] != self_socket) {
      if (send(client_sockets[i], message, strlen(message), 0) < 0) {
        puts("Issue sending message to client, continuing");
      }
    }
  }
  pthread_mutex_unlock(clients_mutex);
}

void handle_failed_receive(ssize_t read_size) {
  if (read_size == 0) {
    puts("Client disconnected");
    (void)fflush(stdout);
  } else if (read_size == -1) {
    puts("Issue receiving message from client, disconnecting client");
  }
}

void remove_client(int* client_sockets, int self_socket, int* n_clients,
                   pthread_mutex_t* clients_mutex) {
  pthread_mutex_lock(clients_mutex);
  for (int i = 0; i < *n_clients; i++) {
    if (client_sockets[i] == self_socket) {
      while (i < *n_clients - 1) {
        client_sockets[i] = client_sockets[i + 1];
        i++;
      }
      (*n_clients)--;
      break;
    }
  }
  pthread_mutex_unlock(clients_mutex);
  close(self_socket);
}

void* handle_client(void* arg) {
  // Detach the current thread.
  pthread_detach(pthread_self());

  // Unpack thread args
  client_handler_args_t* handler_args = (client_handler_args_t*)arg;

  int* client_sockets = handler_args->client_sockets;
  int sock = handler_args->sock;
  int* n_clients = handler_args->n_clients;
  pthread_mutex_t* clients_mutex = handler_args->clients_mutex;

  // Init message buffer
  char buffer[BUFFER_SIZE];
  ssize_t read_size = 0;

  // Receive messages from client
  while ((read_size = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
    relay_message(buffer, read_size, client_sockets, sock, n_clients,
                  clients_mutex);
  }

  handle_failed_receive(read_size);
  remove_client(client_sockets, sock, n_clients, clients_mutex);
  return NULL;
}
