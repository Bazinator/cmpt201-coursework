#include <stdio.h>
#include <unistd.h>

int main() {

  for (int i = 0; i < 20; i++) {
    sleep(i);
    printf("Sleeping for %i\n", i);
  }

  return 1;
}
