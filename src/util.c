#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

noreturn void error_and_exit(const char* error_msg) {
  perror(error_msg);
  // NOLINTNEXTLINE(concurrency-mt-unsafe)
  exit(EXIT_FAILURE);
}
