#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  pid_t pid = fork();

  // CHILD
  if (pid == 0) {

    printf("Hi there I a mmichael and I am the child");

    sleep(5);
    execlp("/bin/ls", "ls", "-a", "-l", "-h", (char *)NULL);
    printf("Hi there I a mmichael and I am the child");
    sleep(5);

  } else {
    // PARENT
    // execlp("/bin/ls", "ls", "-a", (char *)NULL);

    int status;
    if (waitpid(pid, &status, 0) == -1) {
      perror("PID Failure");
      exit(EXIT_FAILURE);
    }
    if (WIFEXITED(status)) {
      // WEXITSTATUS
      printf("child exited normally with status %d\n", WEXITSTATUS(status));
    } else {
      printf("Child did not exit normally\n");
    }

    //    perror("exec failed");
    //   printf("%d\n", getpid());
    return 1;
  }
}
