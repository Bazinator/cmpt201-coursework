#define _POSIX_C_SOURCE 200809L
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

int main() {

  // Loop until the the return token is print.
  // For each line we get the size of it and store it in an array of char*
  // free the memory, then reinit and continue

  char *input = NULL;
  size_t size = 0;
  ssize_t num_char = 1;

  char *inputs[5] = {NULL, NULL, NULL, NULL, NULL};
  int pos = 0;
  int total_entries = 0;

  while (1) {
    printf("Enter input: ");
    num_char = getline(&input, &size, stdin);

    if (strcmp(input, "print\n") == 0) {
      int items_to_print = total_entries < 5 ? total_entries : 5;

      int start_idx = total_entries < 5 ? 0 : pos;

      for (int i = 0; i < items_to_print; i++) {
        int idx = (start_idx + i) % 5;

        printf("%s", inputs[idx]);

        free(inputs[idx]);
        inputs[idx] = NULL;
      }
      pos = 0;
      total_entries = 0;
      continue;
    }

    if (inputs[pos] != NULL) {
      free(inputs[pos]);
    }

    inputs[pos] = strdup(input);
    pos = (pos + 1) % 5;
    total_entries++;
  }

  free(input);

  return 0;
}
