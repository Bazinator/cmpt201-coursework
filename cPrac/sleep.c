#include <stdio.h>
#include <unistd.h>

int main() {

  int ret = fork();
  for (int i = 0; i < 20; i++) {
    if (ret > 0) {
      printf("I am a child");
    } else {
      printf("I am a parent");
    }

    sleep(i);
    printf("Sleeping for %i\n", i);
  }

  return 1;
}
