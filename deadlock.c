#include <pthread.h>
#include <stdio.h>
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

static void *func(void *arg) {
  pthread_mutex_lock(&lock1);
  printf("Hello from t1 we have locked lock1\n");
  pthread_mutex_lock(&lock2);
  printf("Hello we from t1 have now locked lock2\n");

  pthread_mutex_unlock(&lock2);

  pthread_mutex_unlock(&lock1);
  printf("Hi from t1 we have unlocked!\n");

  pthread_exit(0);
}

static void *func2(void *arg) {
  pthread_mutex_lock(&lock2);
  printf("Hi from t2 we have locked lock2\n");
  pthread_mutex_lock(&lock1);
  printf("Hi from t2 we have locked lock1\n");

  pthread_mutex_unlock(&lock1);
  pthread_mutex_unlock(&lock2);
  printf("Hi from t2 we have unlocked!\n");
  pthread_exit(0);
}

int main(int argc, char *argv[]) {
  pthread_t t1;
  pthread_t t2;

  pthread_create(&t1, NULL, func, NULL);
  pthread_create(&t2, NULL, func2, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
}
