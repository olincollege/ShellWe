#pragma once
#include <pthread.h>

enum { BUFFER_SIZE = 1024, MAX_CLIENTS = 100 };

typedef struct {
  int sock;                        // Socket number of current client
  int* n_clients;                  // Number of connected clients
  int* client_sockets;             // Array of socket FD
  pthread_mutex_t* clients_mutex;  // Mutex for client socket FD array
} client_handler_args_t;

void* handle_client(void* arg);
