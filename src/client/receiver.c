#include "receiver.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../util/util.h"
#include "sys/socket.h"

void* receive_message(void* socket_desc) {
  int sock = *(int*)socket_desc;
  char buffer[RECV_BUFFER_SIZE];

  while (1) {
    ssize_t len = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
      buffer[len] = '\0';

      printw("%s\n", buffer);
      printw(">> ");
      refresh();
    } else {
      perror("recv failed");
      break;
    }
  }

  return NULL;
}
