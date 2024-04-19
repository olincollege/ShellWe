#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../util/util.h"

#define SERVER_IP "127.0.0.1"
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
  struct sockaddr_in server;
  char* message = NULL;
  pthread_t recv_thread;

  int sock = open_tcp_socket();

  server.sin_addr.s_addr = inet_addr(SERVER_IP);
  server.sin_family = AF_INET;
  server.sin_port = htons(SERVER_PORT);

  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("connect failed. Error");
    return 1;
  }

  puts("Connected\n");

  if (pthread_create(&recv_thread, NULL, receive_message, (void*)&sock) != 0) {
    perror("could not create thread");
    return 1;
  }

  while (1) {
    printf("Enter message : ");
    size_t len = 0;
    ssize_t read = getline(&message, &len, stdin);

    if (read == -1) {
      puts("Read failed");
      return 1;
    }
    if (send(sock, message, strlen(message), 0) < 0) {
      puts("Send failed");
      return 1;
    }
  }
  close(sock);
  return 0;
}
