#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../util/util.h"
#include "receiver.h"


int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <IP address>\n", argv[0]);
    return 1;
  }

  char *ip_address = argv[1];  // IP address passed as command-line argument

  char* message = NULL;
  pthread_t recv_thread = 0;

  int sock = open_tcp_socket();

  struct sockaddr_in server = socket_address(ip_address, SERVER_PORT);

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
  char* username = NULL; // replace with actual username
  size_t username_len = 0;
  char* prefix = " >> ";
  printf("Enter username : ");
  if (getline(&username, &username_len, stdin) == -1) {
    close_tcp_socket(sock);
    error_and_exit("Setting username failed");
  }

  while (1) {
    printf(">> ");
    (void)fflush(stdout);
    if (getline(&message, &len, stdin) == -1) {
      close_tcp_socket(sock);
      error_and_exit("Read failed");
    }

    size_t full_message_size = strlen(username) + strlen(prefix) + strlen(message) + 1;
    char* full_message = malloc(full_message_size);

    if (full_message == NULL) {
      close_tcp_socket(sock);
      error_and_exit("Failed to allocate memory for message");
    }

    if(snprintf(full_message, full_message_size, "%s%s%s", username, prefix,
              message) == -1) {
      close_tcp_socket(sock);
      free(full_message);
      error_and_exit("Failed to concatenate for full message");
    }

    remove_newline(full_message);

    if (send(sock, full_message, strlen(full_message), 0) < 0) {
      close_tcp_socket(sock);
      error_and_exit("Send failed");
    }

    free(full_message);
  }
}
