#pragma once
#include <netinet/in.h>
#include <sys/socket.h>

enum { MAX_BACKLOG = 10, PORT = 12345 };

typedef struct {
  int listener;
  struct sockaddr_in addr;
  int max_backlog;
} server_t;

server_t* init_server(struct sockaddr_in ip_addr, int max_backlog);

void listen_for_connections(server_t* server);

int accept_client(server_t* server);

void log_client(int socket_fd, int* n_clients, int* client_sockets,
                pthread_mutex_t* clients_mutex);
