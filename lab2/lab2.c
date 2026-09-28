#define POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  // Get input from user
  //
  //
  //
  //
  char *input = NULL;
  size_t size = 0;

  ssize_t num_char = 1;

  while (num_char > 0) {
    printf("Please enter a command to execute: ");
    fflush(stdout);
    num_char = getline(&input, &size, stdin);

    input[strcspn(input, "\r\n")] = '\0';

    pid_t pid = fork();

    if (pid == 0) {
      // CHILD

      char *argv[] = {input, NULL};

      execv(input, argv);

      printf("Exec failure\n");
      free(input);

    } else {
      int status;
      if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
      }
    }
  }
  free(input);
}
