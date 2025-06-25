#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define THREAD_COUNT 8

pthread_t threads[THREAD_COUNT];

struct output {
  int index;
};

void *test(void *arg) {
  struct output *out = (struct output *)arg;

  out->index = 25565;

  printf("Created a new thread\n");
  return NULL;
}

int main() {
  struct output *output_buffer = malloc(THREAD_COUNT * sizeof(struct output));
  if (output_buffer == NULL) return 1;

  int i;
  for (i = 0; i < THREAD_COUNT; i++) {
    output_buffer[i].index = i + 1;
  }

  for (i = 0; i < THREAD_COUNT; i++) {
    printf("%d\n", output_buffer[i].index);
  } printf("\n");

  for (i = 0; i < THREAD_COUNT; i++) {
    pthread_create(&threads[i], NULL, &test, (void *)(&(output_buffer[i])));
  }

  for (i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
  } printf("\n");

  for (i = 0; i < THREAD_COUNT; i++) {
    printf("%d\n", output_buffer[i].index);
  } printf("\n");

  return 0;
}
