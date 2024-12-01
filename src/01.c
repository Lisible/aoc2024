#include "common.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_LINE_COUNT 1000
#define MAX_VALUE 99999
#define HISTOGRAM_LENGTH (MAX_VALUE + 1)
#define LINE_BUFFER_CAPACITY 128

int int_cmp(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

bool read_lists_from_file(int left[INPUT_LINE_COUNT],
                          int right[INPUT_LINE_COUNT],
                          int right_histogram[HISTOGRAM_LENGTH],
                          const char *file_path) {
  BEGIN_PROFILED_FUNCTION();
  FILE *file_handle = fopen(file_path, "r");
  if (!file_handle) {
    LOG("Couldn't open file %s: %s", file_path, strerror(errno));
    goto err;
  }

  char line[LINE_BUFFER_CAPACITY];
  int index = 0;
  ssize_t read;
  while ((read = fgets(line, LINE_BUFFER_CAPACITY, file_handle) != NULL)) {
    char *left_value_str = strtok(line, " ");
    if (!left_value_str) {
      LOG("Missing left value");
      goto close_file;
    }

    char *right_value_str = strtok(NULL, " ");
    if (!right_value_str) {
      LOG("Missing right value");
      goto close_file;
    }

    left[index] = atoi(left_value_str);
    right[index] = atoi(right_value_str);
    right_histogram[right[index]]++;

    index++;
  }
  if (!feof(file_handle)) {
    LOG("An error occured while reading the file");
    goto close_file;
  }
  fclose(file_handle);
  qsort(left, INPUT_LINE_COUNT, sizeof(int), int_cmp);
  qsort(right, INPUT_LINE_COUNT, sizeof(int), int_cmp);
  END_PROFILED_FUNCTION();
  return true;

close_file:
  fclose(file_handle);
err:
  return false;
}

int compute_total_distance(int left[INPUT_LINE_COUNT],
                           int right[INPUT_LINE_COUNT]) {
  BEGIN_PROFILED_FUNCTION();
  int total_distance = 0;
  for (int i = 0; i < INPUT_LINE_COUNT; i++) {
    total_distance += abs(left[i] - right[i]);
  }
  END_PROFILED_FUNCTION();
  return total_distance;
}

int compute_similarity_score(int left[INPUT_LINE_COUNT],
                             int right_histogram[HISTOGRAM_LENGTH]) {
  BEGIN_PROFILED_FUNCTION();
  int similarity_score = 0;
  for (int i = 0; i < INPUT_LINE_COUNT; i++) {
    similarity_score += left[i] * right_histogram[left[i]];
  }
  END_PROFILED_FUNCTION();
  return similarity_score;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    LOG("Usage: ./01 <input_file_path>");
    goto err;
  }

  int left[INPUT_LINE_COUNT];
  int right[INPUT_LINE_COUNT];
  int right_histogram[HISTOGRAM_LENGTH] = {0};
  if (!read_lists_from_file(left, right, right_histogram, argv[1])) {
    LOG("Couldn't read list from file: %s", argv[1]);
    goto err;
  }

  printf("Total distance: %d\n", compute_total_distance(left, right));
  printf("Similarity score: %d\n",
         compute_similarity_score(left, right_histogram));

  return 0;
err:
  return 1;
}
