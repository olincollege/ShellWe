#include "receiver.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../util/util.h"
#include "sys/socket.h"

void* receive_message(void* recv_args) {
  recv_args_t* args = (recv_args_t*)recv_args;
  int sock = (int)args->socket;
  int* next_msg_pos = args->next_msg_pos;

  char buffer[RECV_BUFFER_SIZE];
  const int MESSAGE_LIMIT = 5;
  int row = 0;
  int col = 0;

  while (1) {
    getmaxyx(stdscr, row, col);
    ssize_t len = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
      buffer[len] = '\0';
      move(0, 0);
      clrtoeol();
      refresh();

      mvprintw(*next_msg_pos, 0, "%s", buffer);
      *next_msg_pos++;
      move(row - 2, 6);
      clrtoeol();
      refresh();
    } else {
      perror("recv failed");
      break;
    }
  }

  return NULL;
}
