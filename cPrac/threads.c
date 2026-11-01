#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *threadFunction(void *arg) {
  char *msg = arg;
  printf("%s\n", msg);
  return (void *)strlen(msg);
}

int main() {

  void *ret_val = 0;
  pthread_t tid;
  if (pthread_create(&tid, NULL, threadFunction, "Hello") != 0) {
    perror("Unable to create thread.");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(tid, &ret_val) != 0) {
    perror("Unable to join");
    exit(EXIT_FAILURE);
  }
  printf("%lu\n", (uint64_t)ret_val);
  return 0;
}
