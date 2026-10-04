#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

int main() {

  // Loop until the the return token is print.
  // For each line we get the size of it and store it in an array of char*
  // free the memory, then reinit and continue

  char *input = "test";
  ssize_t size = 0;
  ssize_t num_char = 1;

  char *print = "print";
  char *inputs[5];

  while (num_char > 0) {
    input = "test";
    int pos = 0;
    while (*input != *print) {
      if (pos == 5) {
        pos = 0;
      }
      printf("Enter input: ");
      num_char = getline(&input, &size, stdin);

      inputs[pos] = input;
      pos++;
    }

    // Print out the last 5
    while (pos >= 0) {
      printf("we had a line\n");
      pos--;
    }
  }
}
