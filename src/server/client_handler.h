#pragma once
#include <pthread.h>

enum { BUFFER_SIZE = 1024, MAX_CLIENTS = 100 };

// A set of arguments to pass to each client
typedef struct {
  int sock;                        // Socket number of current client
  int* n_clients;                  // Number of connected clients
  int* client_sockets;             // Array of socket FD
  pthread_mutex_t* clients_mutex;  // Mutex for client socket FD array
} client_handler_args_t;

/**
 * Assigns a client handler to a separate thread.
 *
 * Creates a new pthread to handle a client using the provided
 * client handler arguments. Upon successful creation of the thread, a message
 * "Handler Assigned" is printed to stdout. In case of failure to create the
 * thread, the program exits with an error message.
 *
 * @param handler_args Pointer to client_handler_args_t.
 */
void assign_handler(client_handler_args_t* handler_args);

/**
 * Handle messages from a client and broadcast them to other clients.
 *
 * Handles messages received from a client socket, broadcasting
 * the messages to all other connected client sockets. It also manages the
 * removal of the client from the list of connected clients when the client
 * disconnects. Terminates when the client socket is closed or an error occurs
 * during receiving.
 *
 * @param arg A client_handler_args_t pointer to the function arguments.
 * @returns The null pointer
 */
void* handle_client(void* arg);
