#include "poola.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

poola_thread_output *output_buffer;
void (*thread_function)(poola_thread_output *);
pthread_t threads[POOLA_THREAD_COUNT];
poola_thread_output *big_output;
const size_t n = POOLA_THREAD_COUNT * sizeof(poola_thread_output);
size_t big_output_count;

int poola_init(void (*poola_thread_function)(poola_thread_output *)) {
  // Allocate the buffer for the output of the threads
  output_buffer = NULL;
  {
    output_buffer = malloc(n);

    if (output_buffer == NULL) {
      fprintf(stderr, "Fatal: failed to allocate %zu bytes for the thread output buffer.\n", n);
      fprintf(stderr, "errno Information: %s\n", strerror(errno));

      return -1;
    }
  }

  thread_function = poola_thread_function;
  if (poola_thread_function == NULL) {
    fprintf(stderr, "Fatal: NULL function provided for the thread function\n");

    return -1;
  }

  big_output = NULL;
  big_output_count = 0;

  return 0;
}

void *thread_function_complete(void *arg) {
  poola_thread_output *output = (poola_thread_output *)arg;

  thread_function(output);

  return NULL;
}

int poola_run(void) {
  size_t i;
  
  for (i = 0; i < POOLA_THREAD_COUNT; ++i) {
    pthread_create(&threads[i], NULL, thread_function_complete, &output_buffer[i]);
  }

  return 0;
}

poola_thread_output *poola_out(void) {
  // finish the work on all threads
  size_t i;
  for (i = 0; i < POOLA_THREAD_COUNT; ++i) {
    pthread_join(threads[i], NULL);
  }

  // allocate a completely new list
  if (big_output == NULL) {
    big_output = malloc(n);
  } else {
    big_output = realloc(big_output, n * (big_output_count + 1));
  }

  if (big_output == NULL) {
    fprintf(stderr, "Error: failed to allocate %zu bytes for the big output buffer.\n", n * (big_output_count + 1));
    fprintf(stderr, "errno Information: %s\n", strerror(errno));

    return NULL;
  }

  for (i = n * big_output_count; i < n * (big_output_count + 1); i++) {
    big_output[i] = output_buffer[i];
  }

  big_output_count++;

  return big_output;
}

void poola_free(void) {
  big_output_count = 0;
  if (big_output != NULL) { free(big_output); big_output = NULL; }
  thread_function = NULL;
  free(output_buffer);
}
