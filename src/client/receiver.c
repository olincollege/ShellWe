#include "receiver.h"
#include "sys/socket.h"
#include <stdio.h>
#include <stdlib.h>
#include "../util/util.h"

void* receive_message(void* socket_desc) {
  int sock = *(int*)socket_desc;
  char buffer[RECV_BUFFER_SIZE];
  char* recv_line = NULL;

  while (1) {
    ssize_t len = recv(sock, buffer + 1, sizeof(buffer) - 2, 0);
    if (len > 0) {
      buffer[0] = '\n';
      buffer[len + 1] = '\0';
      printf("%s\n", buffer);
      printf(">> ");
      (void)fflush(stdout);
    } else {
      perror("recv failed");
      break;
    }
  }

  free(recv_line);
  perror("recv failed");
  return NULL;
}
