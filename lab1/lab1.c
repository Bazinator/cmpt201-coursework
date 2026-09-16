#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  char *input = NULL;
  size_t size = 0;

  ssize_t num_char = 1;
  while (num_char > 0) {
    printf("Please enter a string to tokenize: ");
    num_char = getline(&input, &size, stdin);
    char *saveptr;
    char *ret = strtok_r(input, " ", &saveptr);
    printf("%s\n", ret);
    while (ret != NULL) {
      ret = strtok_r(NULL, " ", &saveptr);
      printf("%s\n", ret);
    }
  }
  free(input);

  return 0;
}
