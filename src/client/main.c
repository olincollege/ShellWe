#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "../util/util.h"

#define SERVER_IP 0x7f000001
#define SERVER_PORT 12345
#define BUFFER_SIZE 1024

void* receive_message(void* socket_desc) {
  int sock = *(int*)socket_desc;
  char server_msg[BUFFER_SIZE];
  ssize_t read_size;

  while ((read_size = recv(sock, server_msg, BUFFER_SIZE - 1, 0)) > 0) {
    server_msg[read_size] = '\0';
    printf("Server reply : %s\n", server_msg);
  }

  if (read_size == 0) {
    puts("Server disconnected");
    fflush(stdout);
  } else if (read_size == -1) {
    perror("recv failed");
  }

  return 0;
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

  while (1) {
    puts("Enter message : ");
    if (getline(&message, &len, stdin) == -1) {
      close_tcp_socket(sock);
      error_and_exit("Read failed");
    }

    if (send(sock, message, strlen(message), 0) < 0) {
      close_tcp_socket(sock);
      error_and_exit("Send failed");
    }
  }
}
