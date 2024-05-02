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
    error_and_exit("Error creating thread");
  }
  puts("Handler Assigned");
}

/**
 * Relays a message to all connected clients except the sender.
 *
 * Relays the provided message to all connected clients except the sender
 * identified by 'self_socket'. It locks the clients_mutex to ensure
 * thread safety while accessing the client_sockets array. Upon successful
 * relay of the message to all clients, the mutex is unlocked. In case of any
 * error during message transmission, the program exits with an error message.
 *
 * @param message The message to relay.
 * @param read_size The size of the message.
 * @param client_sockets Array of client sockets.
 * @param self_socket The socket of the sender.
 * @param n_clients Pointer to the number of clients.
 * @param clients_mutex Pointer to the mutex for accessing the clients array.
 */
static void relay_message(char* message, ssize_t read_size, int* client_sockets,
                          int self_socket, const int* n_clients,
                          pthread_mutex_t* clients_mutex) {
  pthread_mutex_lock(clients_mutex);
  message[read_size] = '\0';  // Null terminate the message
  // Lock the clients array
  for (int i = 0; i < *n_clients; i++) {
    if (client_sockets[i] != self_socket) {
      if (send(client_sockets[i], message, strlen(message), 0) < 0) {
        error_and_exit("Error sending message to client");
      }
    }
  }
  pthread_mutex_unlock(clients_mutex);
}

/**
 * Handles a failed message receive operation.
 *
 * @param read_size The size of the received message.
 */
static void handle_failed_receive(ssize_t read_size) {
  if (read_size == 0) {
    puts("Client disconnected");
    (void)fflush(stdout);
  } else if (read_size == -1) {
    error_and_exit("Error receiving message from client");
  }
}

/**
 * Removes a client from the list of connected clients.
 * 
 * This function removes the client identified by 'self_socket' from the array
 * of client sockets. It locks the clients_mutex to ensure thread safety while
 * accessing and modifying the client_sockets array. Once the client is removed,
 * the mutex is unlocked. Also closes the socket associated with the
 * removed client.
 * 
 * @param client_sockets Array of client sockets.
 * @param self_socket The socket of the client to be removed.
 * @param n_clients Pointer to the number of clients.
 * @param clients_mutex Pointer to the mutex for accessing the clients array.
 */
static void remove_client(int* client_sockets, int self_socket, int* n_clients,
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
