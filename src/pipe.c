#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

void spawn_child(int child_id, int* pipe_fd) {
  pid_t pid = fork();

  if (pid < 0) {
    error_and_exit("Can't fork process");
  }
  if (pid == 0) {
    char msg[30];
    if (snprintf(msg, sizeof(msg), "I am child %i", child_id) < 0) {
      error_and_exit("Error making message string");
    }
    write(pipe_fd[1], msg, strlen(msg));
    exit(EXIT_SUCCESS);
  }
}

int main(void) {
  const int NUM_CHILDREN = 3;
  int pipe_to_parent[NUM_CHILDREN][2];

  for (int i = 0; i < NUM_CHILDREN; ++i) {
    if (pipe(pipe_to_parent[i]) == -1) {
      error_and_exit("Can't create pipe");
    }
  }

  for (int i = 0; i < NUM_CHILDREN; ++i) {
    spawn_child(i, pipe_to_parent[i]);
  }

  for (int i = 0; i < NUM_CHILDREN; ++i) {
    const int BUFFER_SIZE = 80;
    char buffer[BUFFER_SIZE];
    read(pipe_to_parent[i][0], buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
  }

  return 0;
}
