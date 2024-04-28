#include <ncurses.h>
#include <string.h>

int main() {
  char* mesg = "Type your message: ";
  char str[80];
  int row, col;
  int message_row = 0;  // Track the row for printing messages
  const int MESSAGE_LIMIT = 5;

  initscr();

  while (1) {
    getmaxyx(stdscr, row, col);
    mvprintw(row - 2, 0, "%s", mesg);
    refresh();

    getstr(str);
    if (strcmp(str, "exit") == 0) {
      printw("Exiting");
      break;
    }

    // Print the user's message beneath the previous message
    mvprintw(message_row, 0, "You: %s", str);
    refresh();  // Update the screen

    // Increment the message row for the next message
    message_row++;

    // Clear the input area
    move(row - 2, 0);  // Move cursor to the input area
    clrtoeol();        // Clear the line from cursor position to end of line

    // If message_row reaches limit, clear the screen and reset message_row
    if (message_row == MESSAGE_LIMIT) {
      clear();          // Clear the screen
      refresh();        // Update the screen
      message_row = 0;  // Reset message_row to start from the top
      mvprintw(message_row, 0, "You: %s", str);
      message_row++;
    }
  }

  endwin();

  return 0;
}
