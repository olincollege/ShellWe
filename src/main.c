#include <stdio.h>
#include <string.h>

#include "util.h"

#define FILEPATH "../../files/"

int main(void) {
  const int NUM_FILES = 3;
  const int FILENAME_LEN = 20;

  char filenames[NUM_FILES][FILENAME_LEN];

  strcpy(filenames[0], "a.txt");
  strcpy(filenames[1], "b.txt");
  strcpy(filenames[2], "c.txt");

  FILE* files[NUM_FILES];

  for (int i = 0; i < NUM_FILES; ++i) {
    char full_path[30] = FILEPATH;
    strcat(full_path, filenames[i]);
    files[i] = fopen(full_path, "w+e");
    if (!files[i]) {
      error_and_exit("Couldn't open file");
    }
  }

  char* line = "hello";

  for (int i = 0; i < NUM_FILES; ++i) {
    if (fputs(line, files[i]) == EOF) {
      error_and_exit("Couldn't write to file");
    }
  }

  for (int i = 0; i < NUM_FILES; ++i) {
    if (fclose(files[i])) {
      error_and_exit("Couldn't close file");
    }
  }

  return 0;
}
