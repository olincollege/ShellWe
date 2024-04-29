#include <arpa/inet.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../tui/tui_handler.h"
#include "../util/util.h"
#include "receiver.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    (void)fprintf(stderr, "Usage: %s <IP address>\n", argv[0]);
    return 1;
  }

  int* next_msg_pos = malloc(sizeof(int));
  *next_msg_pos = 1;
  const size_t MSGSIZE = 100;

  // initialize ncurses screen
  int row = 0;
  int col = 0;
  init_scr(&row, &col);

  // make 2 windows and generate window_info_t structs
  window_info_t* output_info = init_win(row - 3, col, 0, 0);
  WINDOW* output = output_info->window;

  window_info_t* input_info = init_win(3, col, row - 3, 0);
  WINDOW* input = input_info->window;

  char* ip_address = argv[1];  // IP address passed as command-line argument

  char* message = malloc(MSGSIZE * sizeof(char));
  pthread_t recv_thread = 0;

  int sock = open_tcp_socket();
  uint32_t ip_addr_converted = ntohl(inet_addr(ip_address));
  struct sockaddr_in server = socket_address(ip_addr_converted, SERVER_PORT);

  if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
    close_tcp_socket(sock);
    error_and_exit("connect failed. Error");
  }

  printw("Successfully connected to server\n");
  refresh();

  recv_args_t* args = malloc(sizeof(recv_args_t));
  args->next_msg_pos = next_msg_pos;
  args->socket = sock;
  args->input = input_info;
  args->output = output_info;

  if (pthread_create(&recv_thread, NULL, receive_message, (void*)args) != 0) {
    close_tcp_socket(sock);
    error_and_exit("could not create thread");
  }

  char* username = malloc(MSGSIZE * sizeof(char));
  char* prefix = " >> ";
  printw("Enter a username to continue : ");
  refresh();
  if (getstr(username) == -1) {
    close_tcp_socket(sock);
    error_and_exit("Setting username failed");
  }

  make_box(input);
  make_box(output);

  char* title = "Welcome to ShellWe";
  mvwprintw(
      output, 0,
      (output_info->col - ((int)strlen(username) + (int)strlen(title))) / 2,
      "Hi %s, %s", username, title);
  wrefresh(output);

  int type_start = input_info->row - 2;

  while (1) {
    mvwprintw(input, type_start, 1, "%s%s", "Me", prefix);
    wrefresh(input);
    wgetstr(input, message);  // Get message from user

    if (strcmp(message, "exit") == 0) {
      endwin();  // Cleanup ncurses
      break;
    }
    wmove(input, type_start, 7);  // Move cursor to the input area
    wclrtoeol(input);             // Clear what user typed previously
    wrefresh(input);

    size_t full_message_size =
        strlen(username) + strlen(prefix) + strlen(message) + 1;
    char* full_message = malloc(full_message_size);

    if (full_message == NULL) {
      close_tcp_socket(sock);
      error_and_exit("Failed to allocate memory for message");
    }

    if (snprintf(full_message, full_message_size, "%s%s%s", username, prefix,
                 message) == -1) {
      close_tcp_socket(sock);
      free(full_message);
      error_and_exit("Failed to concatenate for full message");
    }

    remove_newline(full_message);

    if (send(sock, full_message, strlen(full_message), 0) < 0) {
      close_tcp_socket(sock);
      error_and_exit("Send failed");
    }

    free(full_message);
  }
  free(message);
  free(username);
  free(next_msg_pos);
  printf("Successfully Exited ShellWe\n");
  return 0;
}
