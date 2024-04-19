#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

enum { BACKLOG_SIZE = 10, BUFFER_SIZE = 1024 };

typedef struct {
  int accepted_socketFD;    // client socket FD
  struct sockaddr_in addr;  // client addr
  int error;
  int accept_success;
  int client_list[BACKLOG_SIZE];  // List of client socket FDs
  int* client_count;              // Number of clients in chat
} accepted_socket;

accepted_socket* accept_connection(int server_socketFD);

void start_accepting(int server_socketFD);

void* recv_and_print(void* arg);

void create_client_thread(accepted_socket* accepted);
