#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cnt = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *increment_cnt(void *arg) {
  int i = 0;
  pthread_mutex_lock(&mutex);

  while (i < 100000000) {
    cnt++;
    i++;
  }

  pthread_mutex_unlock(&mutex);
  return (void *)NULL;
}

int main() {

  pthread_t tid1;
  pthread_t tid2;

  if (pthread_create(&tid1, NULL, increment_cnt, NULL) != 0) {
    perror("Unable to create thread.");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&tid2, NULL, increment_cnt, NULL) != 0) {
    perror("Unable to create thread.");
    exit(EXIT_FAILURE);
  }

  if (pthread_join(tid1, NULL) != 0) {
    perror("Unable to join");
    exit(EXIT_FAILURE);
  }
  if (pthread_join(tid2, NULL) != 0) {
    perror("Unable to join");
    exit(EXIT_FAILURE);
  }
  printf("%d\n", cnt);
}
