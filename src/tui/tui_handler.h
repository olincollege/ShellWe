#pragma once
#include <ncurses.h>

typedef struct {
  WINDOW* window;
  int row;
  int col;
} window_info_t;

void init_scr(int* row, int* col);

window_info_t* init_win(int height, int width, int start_row, int start_col);

void make_box(WINDOW* win);
