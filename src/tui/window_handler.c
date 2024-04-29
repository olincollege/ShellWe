#include "window_handler.h"

#include <ncurses.h>
#include <stdlib.h>

void init_scr(int* row, int* col) {
  initscr();
  cbreak();
  getmaxyx(stdscr, *row, *col);
}

window_info_t* init_win(int height, int width, int start_row, int start_col) {
  WINDOW* win = newwin(height, width, start_row, start_col);
  int win_row = 0;
  int win_col = 0;
  getmaxyx(win, win_row, win_col);

  window_info_t* win_info = malloc(sizeof(window_info_t));
  win_info->window = win;
  win_info->row = win_row;
  win_info->col = win_col;

  return win_info;
}

void make_box(WINDOW* win) {
  box(win, ' ', '-');
  touchwin(win);
  wrefresh(win);
}
