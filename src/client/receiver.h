#pragma once
#include <ncurses.h>

typedef struct {
  WINDOW* window;
  int row;
  int col;
} window_info_t;

typedef struct {
  int socket;
  int* next_msg_pos;
  window_info_t* input;
  window_info_t* output;
} recv_args_t;

void* receive_message(void* recv_args);
