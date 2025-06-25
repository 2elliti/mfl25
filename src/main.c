#include "poola.h"
#include <stdio.h>

void func(poola_thread_output *output) {
  printf("Created a new thread\n");

  output->index = 69420;

  return;
}

int main() {
  if (poola_init(func)) return -1;

  if (poola_run()) return -1;

  poola_thread_output *output = poola_out();
  if (output == NULL) return -1;

  size_t i;
  for (i = 0; i < POOLA_THREAD_COUNT; ++i) {
    printf("%d\n", output[i].index);
  }

  poola_free();
  return 0;
}
