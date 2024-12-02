#include "common.h"
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define MAX_REPORT_LENGTH 8
#define REPORT_COUNT 1000
#define LINE_BUFFER_CAPACITY 128

bool parse_reports(const char *file_path,
                   int reports[REPORT_COUNT][MAX_REPORT_LENGTH],
                   int report_lengths[REPORT_COUNT]) {
  BEGIN_PROFILED_FUNCTION();
  FILE *handle = fopen(file_path, "r");
  if (!handle) {
    LOG("Couldn't open file %s: %s", file_path, strerror(errno));
    return false;
  }

  char line[LINE_BUFFER_CAPACITY];
  int index = 0;
  ssize_t read;
  while ((read = fgets(line, LINE_BUFFER_CAPACITY, handle) != NULL)) {
    char *first_value_str = strtok(line, " ");
    if (!first_value_str) {
      LOG("Missing first value");
      goto close_file;
    }

    reports[index][0] = atoi(first_value_str);
    report_lengths[index]++;

    int level_index = 1;
    char *token;
    while ((token = strtok(NULL, " ")) != NULL) {
      reports[index][level_index++] = atoi(token);
      report_lengths[index]++;
    }

    index++;
  }

  fclose(handle);
  END_PROFILED_FUNCTION();
  return true;
close_file:
  fclose(handle);
  return false;
}

bool is_report_safe(int levels[MAX_REPORT_LENGTH], int report_length) {
  int prev_d = 0;
  int d = 0;
  for (int level_index = 1; level_index < report_length; level_index++) {
    d = levels[level_index] - levels[level_index - 1];
    if ((prev_d > 0 && d < 0) || (prev_d < 0 && d > 0))
      return false;
    int abs_d = abs(d);
    if (abs_d < 1 || abs_d > 3)
      return false;

    prev_d = d;
  }
  return true;
}

bool is_report_safe_2(int levels[MAX_REPORT_LENGTH], int report_length) {
  if (is_report_safe(levels, report_length)) {
    return true;
  }

  for (int skip_index = 0; skip_index < report_length; skip_index++) {
    int modified_levels[MAX_REPORT_LENGTH];
    int new_length = 0;

    for (int i = 0; i < report_length; i++) {
      if (i != skip_index) {
        modified_levels[new_length++] = levels[i];
      }
    }

    if (is_report_safe(modified_levels, new_length)) {
      return true;
    }
  }

  return false;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    LOG("Usage: ./02 <input_file>");
    return 1;
  }

  int reports[REPORT_COUNT][MAX_REPORT_LENGTH];
  int report_lengths[REPORT_COUNT] = {0};

  if (!parse_reports(argv[1], reports, report_lengths)) {
    LOG("Couldn't parse reports");
    return 1;
  }

  int safe_reports = 0;
  for (int report_index = 0; report_index < REPORT_COUNT; report_index++) {
    safe_reports += (int)is_report_safe(reports[report_index],
                                        report_lengths[report_index]);
  }
  printf("Safe reports: %d\n", safe_reports);

  safe_reports = 0;
  for (int report_index = 0; report_index < REPORT_COUNT; report_index++) {
    safe_reports += (int)is_report_safe_2(reports[report_index],
                                          report_lengths[report_index]);
  }
  printf("Safe reports 2: %d\n", safe_reports);

  return 0;
}
