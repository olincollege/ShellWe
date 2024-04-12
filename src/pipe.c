#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

int main(void) {
  const int BUFFER_SIZE = 80;
  char buffer[BUFFER_SIZE];
  int pipe_to_parent[2];
  int pipe_to_child[2];
  if (pipe(pipe_to_parent) == -1 || pipe(pipe_to_child) == -1) {
    error_and_exit("Can't create pipe");
  }

  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("Can't fork process");
  }

  if (!pid) {
    // child
    char msg[] = "from child to parent";
    write(pipe_to_parent[1], msg, strlen(msg));

    read(pipe_to_child[0], buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
  } else {
    // parent
    char msg[] = "from parent to child";
    write(pipe_to_child[1], msg, strlen(msg));

    read(pipe_to_parent[0], buffer, BUFFER_SIZE);
    printf("%s\n", buffer);
  }

  return 0;
}
