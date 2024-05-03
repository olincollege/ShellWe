#pragma once
#include "../tui/tui_handler.h"

enum { DEFAULT_CURSOR_COL = 7 };

typedef struct {
  int socket;
  int* next_msg_pos;
  window_info_t* input;
  window_info_t* output;
} recv_args_t;

/**
 * Receive messages from a socket and print them to stdout.
 *
 * Continuously receieves messages from the provided socket, then prints it out
 * to stdout. Terminates if there is an error during receiving or the socket is
 * closed.
 *
 * @param socket_desc A void pointer to the socket descriptor.
 * @returns The Null pointer.
 */

void* receive_message(void* recv_args);
