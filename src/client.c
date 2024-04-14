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

FILE* get_socket_file(int client_socket) {
  FILE* socket_file = fdopen(client_socket, "w+");
  if (socket_file == NULL) {
    error_and_exit("Couldn't open socket as file stream");
  }
  return socket_file;
}

int echo(FILE* socket_file) {
  char* send_line = NULL;
  size_t send_line_size = 0;
  if (getline(&send_line, &send_line_size, stdin) == -1) {
    return -1;
  }
  // If we can't send the line on the socket, the connection is broken and we
  // have to exit.
  if (fputs(send_line, socket_file) == EOF) {
    free(send_line);
    error_and_exit("Couldn't send line");
  }
  free(send_line);
  char* recv_line = NULL;
  size_t recv_line_size = 0;
  if (getline(&recv_line, &recv_line_size, socket_file) == -1) {
    return -1;
  }
  fputs(recv_line, stdout);
  free(recv_line);
  return 0;
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
