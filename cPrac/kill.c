#define _POSIX_C_SOURCE 200809
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void sigint_handler(int signum) { write(STDOUT_FILENO, "CTRL-C Pressed\n", 16); }

int main() {

  int ret = fork();

  if (ret == 0) {
    // This is child process

    // sleep to allow handler to be processed
    sleep(3);
    pid_t ppid = getppid();
    while (1) {
      sleep(2);
      // Send signal to parent of SIGINT
      kill(ppid, SIGINT);
    }
  } else if (ret != -1) {
    // This is the parent process
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
      // print error
    }
    while (1) {
      sleep(1);
    }
  }
}
