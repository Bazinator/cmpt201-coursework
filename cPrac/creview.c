#include <stdio.h>
#include <stdlib.h>

int main() {
  // Dynamically make an array of integers
  int *p = (int *)malloc(sizeof(int) * 5);

  if (p == NULL) {
    return 1;
  }

  p[1] = 10;
  printf("%d\n", p[1]);

  free(p);

  return 0;
}
