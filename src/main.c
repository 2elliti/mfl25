#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

FILE *fopen_safe(const char *filename, const char *modes) {
  FILE *file = fopen(filename, modes);
  if (file == NULL) {
    fprintf(stderr, "Error: failed to open file %s\n", filename);
    fprintf(stderr, "errno Information: %s\n", strerror(errno));

    return NULL;
  }

  return file;
}
int fclose_safe(FILE *stream) {
  int out = fclose(stream);
  if (out) {
    fprintf(stderr, "Error: failed to close file stream\n");
    fprintf(stderr, "errno Information: %s\n", strerror(errno));
  }

  return out;
}

#define THREAD_COUNT 12
pthread_t thread_pool[THREAD_COUNT];

typedef struct { size_t index; const char *filename; size_t filesize; } thread_interface_t;
thread_interface_t thread_interface[THREAD_COUNT];

bool active = 1;
bool start[THREAD_COUNT] = { 0 };
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg) {
  thread_interface_t *interface = (thread_interface_t *)arg;

  printf("Thread Created\n");

  while (active) {
    pthread_mutex_lock(&mutex);
    start[interface->index] = 0;
    pthread_mutex_unlock(&mutex);

    while (active && !start[interface->index]);
    if (!active) break;

    //printf("Opening File: %s\n", interface->filename);
    FILE *file = fopen_safe(interface->filename, "rb");
    if (file == NULL) {
      fprintf(stderr, "Error: failed to open necessary input file; skipping file\n");
      continue;
    }

    //printf("Calculating the number of bytes in the file...\n");
    fseek(file, 0, SEEK_END);
    interface->filesize = ftell(file);
    //printf("The file size is: %lu\n", interface->filesize);

    fclose_safe(file);
  }

  return NULL;
}

int main() {
  FILE *file_input, *file_output;
  { // open files and error checking
  file_input = fopen_safe("filenames.txt", "r");
  file_output = fopen_safe("linked.out", "w");

  if (file_input == NULL || file_output == NULL) {
    fprintf(stderr, "Fatal: failed to open necessary input/output files\n");
    return -1;
  }
  }

  size_t i;

  /// prepare the thread stuff
  pthread_t threads[THREAD_COUNT];
  for (i = 0; i < THREAD_COUNT; i++) {
    thread_interface[i].index = i;
    thread_interface[i].filename = NULL;
    thread_interface[i].filesize = 0;

    pthread_create(&threads[i], NULL, thread_function, &thread_interface[i]);
  }

  /// read the input file and prepare a list of filenames, a maximum of 'THREAD_COUNT'
  char *lines[THREAD_COUNT];

  for (i = 0; i < THREAD_COUNT; i++) lines[i] = NULL;

  char c = fgetc(file_input);
  size_t line_start = 0, line_end = 0;

  bool eof = 0;
  while (!eof) {

    bool ok = 1;
    i = 0;

    while (ok && !eof) {
      if (c == '\n' || feof(file_input)) {
        if (feof(file_input)) eof = 1;

        size_t len = line_end - line_start;

        if (lines[i] == NULL) {
          lines[i] = malloc(len * sizeof(char));
        } else {
          lines[i] = realloc(lines[i], len * sizeof(char));
        }

        fseek(file_input, line_start, SEEK_SET);

        fread(lines[i], len, 1, file_input); lines[i][len] = '\0';

        fseek(file_input, line_end + 1, SEEK_SET);

        if (++i == THREAD_COUNT) ok = 0;

        line_start = line_end + 1;
      }

      line_end++;
      c = fgetc(file_input);
    }

    if (eof && ok) {
      for (; i < THREAD_COUNT; i++) {
        free(lines[i]);
        lines[i] = NULL;
      }
    }

    /// we now have a new list of names every time this loop runs
    /// lets get thready!
    for (i = 0; i < THREAD_COUNT; i++) {
      if (lines[i] != NULL) {
        pthread_mutex_lock(&mutex);
        thread_interface[i].filename = lines[i];
        start[i] = 1;
        pthread_mutex_unlock(&mutex);
      }
    }

    for (i = 0; i < THREAD_COUNT; i++) {
      while (start[i]);

      if (thread_interface[i].filename != NULL) {
        fprintf(file_output, "%s: %lu\n", thread_interface[i].filename, thread_interface[i].filesize);
      }

      pthread_mutex_lock(&mutex);
      thread_interface[i].filename = NULL;
      thread_interface[i].filesize = 0;
      pthread_mutex_unlock(&mutex);
    }
  }

  pthread_mutex_lock(&mutex);
  active = 0;
  pthread_mutex_unlock(&mutex);

  for (i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&mutex);

  for (i = 0; i < THREAD_COUNT; ++i) {
    if (lines[i] != NULL) {
      free(lines[i]);
    }
  }

  { // closing the files; only really a problem if the output file fails
  fclose_safe(file_input);
  if (fclose_safe(file_output)) {
    fprintf(stderr, "Fatal: failed to close output file; the output may be incorrect\n");
    return -1;
  }
  }

  return 0;
}
