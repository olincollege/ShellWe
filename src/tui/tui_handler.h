#pragma once
#include <ncurses.h>

typedef struct {
  WINDOW* window;
  int row;
  int col;
} window_info_t;

/**
 * Initializes the ncurses screen, setting it to the cbreak mode, and retrieves
 * the maximum row and column dimensions.
 *
 * @param row Pointer to an integer variable to store the maximum number of
 * rows.
 * @param col Pointer to an integer variable to store the maximum number of
 * columns.
 */
void init_scr(int* row, int* col);

/**
 * Initializes a new ncurses window with the specified dimensions and starting
 * position, retrieves its maximum row and column dimensions, and returns a
 * pointer to a structure containing information about the created window.
 *
 * @param height The height of the window.
 * @param width The width of the window.
 * @param start_row The starting row position of the window.
 * @param start_col The starting column position of the window.
 * @return A pointer to a window_info_t structure containing information about
 * the created window.
 */
window_info_t* init_win(int height, int width, int start_row, int start_col);

/**
 * Draws a box around the specified ncurses window, refreshes the window to
 * display the changes, and updates the screen.
 *
 * @param win The ncurses window to draw the box around.
 */
void make_box(WINDOW* win);
