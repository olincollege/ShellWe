#pragma once

typedef struct {
  int socket;
  int* next_msg_pos
} recv_args_t;

void* receive_message(void* recv_args);
