#include "client.h"

#include <netinet/in.h>  // sockaddr_in
#include <pthread.h>
#include <stdio.h>       // getline
#include <stdlib.h>      // free
#include <sys/socket.h>  // connect, sockaddr

#include "util.h"

void try_connect(int client_socket, struct sockaddr_in server_addr) {
  if (connect(client_socket, (const struct sockaddr*)&server_addr,
              sizeof(server_addr)) == -1) {
    error_and_exit("Couldn't connect to server.");
  }
}

void create_listener(int client_socketFD) {
  pthread_t id = 0;
  pthread_create(&id, NULL, listen_and_print, client_socketFD);
}

void listen_and_print(int client_socketFD) {
  char buffer[BUFFER_SIZE];
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
}
