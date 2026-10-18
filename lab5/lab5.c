#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement first fit
  while (free_list_ptr->size < size && free_list_ptr->next != NULL) {
    free_list_ptr = free_list_ptr->next;
  }
  if (free_list_ptr->next == NULL) {
    return -1;
  }
  return free_list_ptr->id;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t arr[100];
  // TODO: Implement best fit
  struct header *pntr = free_list_ptr;
  int i = 0;
  while (pntr->next != NULL) {
    if (pntr->size >= size) {
      arr[i] = pntr->size;
      i++;
    }
    pntr = pntr->next;
  }

  int min = INT32_MAX;
  while (i >= 0) {
    if (arr[i - 1] < min) {
      min = arr[i - 1];
    }
    i--;
  }

  struct header *ptr2 = free_list_ptr;
  while (ptr2->next != NULL) {
    if (ptr2->size == min) {
      return ptr2->id;
    }
    ptr2 = ptr2->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement worst fit
  int maxSize = 0;
  struct header *pntr = free_list_ptr;
  while (pntr->next != NULL) {
    if (pntr->size > maxSize) {
      maxSize = pntr->size;
    }
    pntr = pntr->next;
  }
  while (free_list_ptr->next != NULL) {
    if (free_list_ptr->size == maxSize) {
      return free_list_ptr->id;
    }
    free_list_ptr = free_list_ptr->next;
  }
  return -1;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  // TODO: Print out the IDs
  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);

  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);
  return 0;
}

/*
function coalesce(freed_block):
    // 1. Check next block
    next_block = freed_block->next_physical
    if next_block is not NULL and next_block->is_free == TRUE:
        // Merge next_block into freed_block
        freed_block->size = freed_block->size + next_block->size + sizeof(header)
        freed_block->next_physical = next_block->next_physical

        if next_block->next_physical is not NULL:
            next_block->next_physical->prev_physical = freed_block

    // 2. Check previous
    prev_block = freed_block->prev_physical
    if prev_block is not NULL and prev_block->is_free == TRUE:
        // Merge freed_block into prev_block
        prev_block->size = prev_block->size + freed_block->size + sizeof(header)
        prev_block->next_physical = freed_block->next_physical

        if freed_block->next_physical is not NULL:
            freed_block->next_physical->prev_physical = prev_block

        // The resulting coalesced block is now prev_block
        target_block = prev_block
    else:
        target_block = freed_block
*/
