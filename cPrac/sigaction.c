#define _POSIX_C_SOURCE 200809
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sleeplongtime(int signum) { write(STDOUT_FILENO, "CTRL-C Pressed\n", 14); }

int main() {

  struct sigaction sa;
  sa.sa_handler = sleeplongtime;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  // Register the signal handler
  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("Sigaction() failed");
    exit(EXIT_FAILURE);
  }
  while (1) {
    sleep(1);
  }
}
