#pragma once
#include "../tui/window_handler.h"

typedef struct {
  int socket;
  int* next_msg_pos;
  window_info_t* input;
  window_info_t* output;
} recv_args_t;

void* receive_message(void* recv_args);
