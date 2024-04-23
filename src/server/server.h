#pragma once
#include <netinet/in.h>
#include <sys/socket.h>

enum { MAX_BACKLOG = 10 };

typedef struct {
  int listener;
  struct sockaddr_in addr;
  int max_backlog;
} server_t;

server_t* init_server(struct sockaddr_in ip_addr, int max_backlog);

void listen_for_connections(server_t* server);
