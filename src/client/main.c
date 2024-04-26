#include <arpa/inet.h>
#include <ncurses.h>  // Include ncurses header
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "../util/util.h"
#include "receiver.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    (void)fprintf(stderr, "Usage: %s <IP address>\n", argv[0]);
    return 1;
  }

  int* next_msg_pos = malloc(sizeof(int));
  *next_msg_pos = 0;
  const int MSGSIZE = 100;
  initscr();
  cbreak();
  scrollok(stdscr, TRUE);
  int row = 0;
  int col = 0;
  getmaxyx(stdscr, row, col);  // get screen size

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

  printw("Connected to server\n");
  refresh();

  recv_args_t* args = malloc(sizeof(recv_args_t));
  args->next_msg_pos = next_msg_pos;
  args->socket = sock;

  if (pthread_create(&recv_thread, NULL, receive_message, (void*)args) != 0) {
    close_tcp_socket(sock);
    error_and_exit("could not create thread");
  }

  int type_start = row - 2;
  char* username = malloc(MSGSIZE * sizeof(char));
  char* prefix = " >> ";
  printw("Plesae enter a username : ");
  if (getstr(username) == -1) {
    close_tcp_socket(sock);
    error_and_exit("Setting username failed");
  }

  move(0, 0);
  clrtoeol();
  move(1, 0);
  clrtoeol();

  refresh();
  mvprintw(0, 0, "Welcome to ShellWe, %s! Messages will be displayed below",
           username);

  while (1) {
    mvprintw(type_start, 0, "%s%s", "Me", prefix);
    refresh();
    getstr(message);  // Get message from user
    if (strcmp(message, "exit") == 0) {
      break;
    }

    move(type_start, 0);  // Move cursor to the input area
    clrtoeol();           // Clear what user typed previously

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
    // printw(full_message);

    free(full_message);
  }
  endwin();  // Cleanup ncurses before exiting
  free(next_msg_pos);
  printf("Successfully Exited ShellWe\n");
  return 0;
}
