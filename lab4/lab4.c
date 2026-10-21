#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct header {
  uint64_t size;
  struct header *next;
};

static int BUF_SIZE = 64;
void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  write(STDOUT_FILENO, buf, len);
}

int main() {
  void *address = sbrk(256);

  if (address == (void *)-1) {
    perror("sbrk failed\n");
    return 1;
  }
  struct header *b1 = (struct header *)address;
  struct header *b2 = (address + 128);

  b1->next = NULL;
  b2->next = b1;
  b1->size = 128;
  b2->size = 128;
  // Print out the adress of each block, values in the header and the data in each block
  print_out("first block: %p\n", &b1, sizeof(b1));
  print_out("second block: %p\n", &b2, sizeof(b2));
  print_out("first block size: %d\n", &b1->size, sizeof(b1->size));
  print_out("first block next: %p\n", &b1->next, sizeof(b1->next));
  print_out("second block size: %d\n", &b2->size, sizeof(b2->size));
  print_out("second block next: %p\n", &b2->next, sizeof(b2->next));

  memset(b1 + 1, 0, 112);
  memset(b2 + 1, 1, 112);

  // print out data in block
  unsigned char *bytesb1 = (unsigned char *)b1 + sizeof(struct header);
  unsigned char *bytesb2 = (unsigned char *)b2 + sizeof(struct header);

  for (int i = 0; i < 112; i++) {
    printf("%d\n", bytesb1[i]);
  }

  for (int i = 0; i < 112; i++) {
    printf("%d\n", bytesb2[i]);
  }
}
