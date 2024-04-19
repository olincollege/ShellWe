#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../util/util.h"

#define SERVER_IP 0x7f000001
#define SERVER_PORT 12345

void* receive_message(void* socket_desc) {
  int sock = *(int*)socket_desc;
  FILE* socket_file = get_socket_file(sock);

  char* recv_line = NULL;
  size_t recv_line_size = 0;

  while (getline(&recv_line, &recv_line_size, socket_file) != -1) {
    puts(recv_line);
  }

  free(recv_line);
  perror("recv failed");
  return NULL;
}

int main(void) {
  char* message = NULL;
  pthread_t recv_thread;

  int sock = open_tcp_socket();

  struct sockaddr_in server = socket_address(SERVER_IP, SERVER_PORT);

  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    close_tcp_socket(sock);
    error_and_exit("connect failed. Error");
  }

  puts("Connected to server\n");

  if (pthread_create(&recv_thread, NULL, receive_message, (void*)&sock) != 0) {
    close_tcp_socket(sock);
    error_and_exit("could not create thread");
  }

  size_t len = 0;
  char* username = "username"; // replace with actual username
  char* prefix = ">> ";

  while (1) {
    puts("Enter message : ");
    if (getline(&message, &len, stdin) == -1) {
      close_tcp_socket(sock);
      error_and_exit("Read failed");
    }
    // Create a new buffer to hold the username, prefix, and message
    char* full_message = malloc(strlen(username) + strlen(prefix) + strlen(message) + 1);
    strcpy(full_message, username);
    strcat(full_message, prefix);
    strcat(full_message, message);

    if (send(sock, full_message, strlen(full_message), 0) < 0) {
      close_tcp_socket(sock);
      error_and_exit("Send failed");
    }

    free(full_message);
  }
}
