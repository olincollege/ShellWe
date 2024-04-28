#include "receiver.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "../util/util.h"
#include "sys/socket.h"

void* receive_message(void* recv_args) {
  recv_args_t* args = (recv_args_t*)recv_args;
  int sock = args->socket;
  int* next_msg_pos = args->next_msg_pos;

  window_info_t* input = args->input;
  WINDOW* input_w = input->window;
  int input_row = input->row;
  int input_col = input->col;

  window_info_t* output = args->output;
  WINDOW* output_w = output->window;
  int output_row = output->row;
  int output_col = output->col;

  char buffer[RECV_BUFFER_SIZE];
  const int MESSAGE_LIMIT = output_row - 1;

  while (1) {
    ssize_t len = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (len > 0) {
      buffer[len] = '\0';

      // print received message to output window
      wmove(output_w, 1, 1);
      mvwprintw(output_w, *next_msg_pos, 1, "%s %d", buffer, *next_msg_pos);
      (*next_msg_pos)++;
      wrefresh(output_w);

      if (*next_msg_pos >= MESSAGE_LIMIT) {
        for (int i = 1; i < MESSAGE_LIMIT; i++) {
          wmove(output_w, i, 0);
          wclrtoeol(output_w);
        }
        *next_msg_pos = 1;
        wmove(output_w, 1, 1);
        mvwprintw(output_w, *next_msg_pos, 1, "%s", buffer);
        (*next_msg_pos)++;
        wrefresh(output_w);
      }

      // send cursor back to input window
      wmove(input_w, input_row - 2, 7);
      wrefresh(input_w);

    } else {
      perror("recv failed");
      break;
    }
  }

  return NULL;
}
