#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../util/util.h"
#include "receiver.h"
#include "sys/_pthread/_pthread_t.h"


int main(void) {
  char* message = NULL;
  pthread_t recv_thread = NULL;

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
    // Create a new buffer to hold the username, prefix, and message
    char* full_message = malloc(strlen(username) + strlen(prefix) + strlen(message) + 1);
    strcpy(full_message, username);
    strcat(full_message, prefix);
    strcat(full_message, message);

    remove_newline(full_message);

    if (send(sock, full_message, strlen(full_message), 0) < 0) {
      close_tcp_socket(sock);
      error_and_exit("Send failed");
    }

    free(full_message);
  }
}
