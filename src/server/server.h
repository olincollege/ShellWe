#pragma once
#include <netinet/in.h>
#include <sys/socket.h>

enum { MAX_BACKLOG = 10, PORT = 12345 };

typedef struct {
  int listener;
  struct sockaddr_in addr;
  int max_backlog;
} server_t;

/**
 * Initializes a server structure with the provided IP address and max backlog.
 *
 * Allocates memory for the server structure, creates a TCP socket using
 * open_tcp_socket(), and assigns the IP address and maximum backlog
 * to the server. The initialized server structure is returned.
 *
 * @param ip_addr The IP address and port number to bind the server to.
 * @param max_backlog The maximum length of the queue of pending connections.
 * @return A pointer to the initialized server structure.
 */
server_t* init_server(struct sockaddr_in ip_addr, int max_backlog);

/**
 * Listens for incoming connections on the server's socket.
 *
 * Binds the server's listener socket to the provided address
 * and starts listening for incoming connections. If binding or setting the
 * socket to listen fails, the program exits with an error message.
 *
 * @param server Pointer to the server structure.
 */
void listen_for_connections(server_t* server);

/**
 * Accepts a client connection on the server's listener socket.
 *
 * Retrieves information about the connected client and prints it using
 * print_connection(). If accepting the client connection fails, the program
 * exits with an error message.
 *
 * @param server Pointer to the server structure.
 * @return The socket descriptor for the connected client.
 */

int accept_client(server_t* server);

/**
 * Logs a newly connected client.
 *
 * This function logs a newly connected client by adding its socket descriptor
 * to the list of client sockets. It locks the clients_mutex to ensure thread
 * safety while accessing and modifying the client_sockets array. Once the
 * client is logged, the mutex is unlocked.
 *
 * @param socket_fd The socket descriptor of the newly connected client.
 * @param n_clients Pointer to the number of clients.
 * @param client_sockets Array of client sockets.
 * @param clients_mutex Pointer to the mutex for accessing the clients array.
 */
void log_client(int socket_fd, int* n_clients, int* client_sockets,
                pthread_mutex_t* clients_mutex);
