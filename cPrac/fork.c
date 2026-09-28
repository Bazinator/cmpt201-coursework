#include <stdio.h>
#include <unistd.h>

int main() {

  int processID = getpid();
  int parentProcess = getppid();
  printf("Start PID=%d Parent=%d\n", processID, parentProcess);

  int pID = fork();
  parentProcess = getppid();
  processID = getpid();
  if (pID == 0) {

    printf("CHILD PID=%d Parent=%d\n", processID, parentProcess);
  } else {

    printf("Parent PID=%d Child=%d\n", processID, pID);
  }
}
