#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
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
  pthread_create(&tid, NULL, threadFunction, "Hello");

  pthread_join(tid, &ret_val);

  printf("%lu\n", (uint64_t)ret_val);
  return 0;
}
