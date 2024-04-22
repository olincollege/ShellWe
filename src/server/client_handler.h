#pragma once
#include <pthread.h>

enum { BUFFER_SIZE = 1024, MAX_CLIENTS = 100, PORT = 12345 };

// A set of arguments to pass to each client
typedef struct {
  int sock;                        // Socket number of current client
  int* n_clients;                  // Number of connected clients
  int* client_sockets;             // Array of socket FD
  pthread_mutex_t* clients_mutex;  // Mutex for client socket FD array
} client_handler_args_t;

/**
 *
 *
 * @param arg A client_handler_args_t pointer to the function arguments.
 * @returns The null pointer
 */
void* handle_client(void* arg);
