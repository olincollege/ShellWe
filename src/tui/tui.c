#include <ncurses.h>
#include <stdio.h>

int main(void) {
  // initialize screen; clear screen, get memory, etc
  initscr();

  int height, width, start_y, start_x;
  height = 10;
  width = 20;
  start_y = 10;
  start_x = 10;

  WINDOW* win = newwin(height, width, start_y, start_x);
  refresh();

  box(win, 0, 0);
  wrefresh(win);

  int input = getch();

  endwin();

  return 0;
}
