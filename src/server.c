#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
#define _GNU_SOURCE

#include "server.h"

#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

accepted_socket* accept_connection(int server_socketFD) {
  struct sockaddr_in client_addr;
  socklen_t addr_size = sizeof(struct sockaddr_in);
  int client_socketFD =
      accept4(server_socketFD, &client_addr, &addr_size, SOCK_CLOEXEC);

  accepted_socket* accepted = malloc(sizeof(accepted_socket));
  accepted->addr = client_addr;
  accepted->accepted_socketFD = client_socketFD;
  accepted->accept_success = client_socketFD > 0;

  if (!accepted->accept_success) {
    accepted->error = client_socketFD;
  }
  return accepted;
}

void* recv_and_print(void* arg) {
  printf("void version\n");
  char buffer[BUFFER_SIZE];

  int client_socketFD = *(int*)arg;

  while (1) {
    ssize_t txt_received = recv(client_socketFD, buffer, BUFFER_SIZE, 0);

    if (txt_received > 0) {
      buffer[txt_received] = 0;
      printf("%s\n", buffer);
    }

    if (txt_received == 0) {
      break;
    }
  }
  close(client_socketFD);
  return NULL;
}

void create_client_thread(accepted_socket* accepted) {
  pthread_t idx = 0;
  pthread_create(&idx, NULL, recv_and_print, (void*)accepted);
}

void start_accepting(int server_socketFD) {
  while (1) {
    accepted_socket* client_socket = accept_connection(server_socketFD);
    create_client_thread(client_socket);
  }
}
