#pragma once

enum {
  RECV_BUFFER_SIZE = 1024,
  SERVER_IP = 0x7f000001,
  SERVER_PORT = 12345,
};

void* receive_message(void* socket_desc);
