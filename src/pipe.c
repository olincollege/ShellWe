#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "util.h"

int main(void) {
  int pipe_fd[2];
  if (pipe(pipe_fd) == -1) {
    error_and_exit("Can't create pipe");
  }
  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("Can't fork process");
  }
  if (!pid) {
    // child
    char msg[] = "hello";
    write(pipe_fd[1], msg, strlen(msg));
  }
  // parent
  const int BUFFER_SIZE = 80;
  char buffer[BUFFER_SIZE];
  read(pipe_fd[0], buffer, BUFFER_SIZE);
  printf("%s\n", buffer);

  return 0;
}
