#ifndef COMMON_H
#define COMMON_H
#ifdef PROFILING
#include <time.h>
#ifdef  __x86_64__
#include <x86intrin.h>
#endif
#endif

#ifdef PROFILING
#ifdef __x86_64__
#define BEGIN_PROFILED_FUNCTION()                                              \
  struct timespec wall_start;                                                  \
  clock_gettime(CLOCK_MONOTONIC, &wall_start);                                 \
  clock_t start = clock(); \
  unsigned long long start_cycle = __rdtsc()

#define END_PROFILED_FUNCTION()                                                \
  do {                                                                         \
    unsigned long long end_cycle = __rdtsc(); \
    clock_t end = clock();                                                     \
    struct timespec wall_end;                                                  \
    clock_gettime(CLOCK_MONOTONIC, &wall_end);                                 \
    double wall_clock_time =                                                   \
        (wall_end.tv_sec - wall_start.tv_sec) * 1000.0 +                       \
        (wall_end.tv_nsec - wall_start.tv_nsec) / 1000000.0;                   \
    double cpu_time = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;       \
    unsigned long long cycle_count = end_cycle - start_cycle; \
    LOG("PROFILE: %s() cpu_time: %fms, wall_clock: %fms, cycle count: %llu", __func__, cpu_time,  \
        wall_clock_time, cycle_count);                                                      \
  } while (0)
#else
#define BEGIN_PROFILED_FUNCTION()                                              \
  struct timespec wall_start;                                                  \
  clock_gettime(CLOCK_MONOTONIC, &wall_start);                                 \
  clock_t start = clock()

#define END_PROFILED_FUNCTION()                                                \
  do {                                                                         \
    clock_t end = clock();                                                     \
    struct timespec wall_end;                                                  \
    clock_gettime(CLOCK_MONOTONIC, &wall_end);                                 \
    double wall_clock_time =                                                   \
        (wall_end.tv_sec - wall_start.tv_sec) * 1000.0 +                       \
        (wall_end.tv_nsec - wall_start.tv_nsec) / 1000000.0;                   \
    double cpu_time = ((double)(end - start)) * 1000.0 / CLOCKS_PER_SEC;       \
    LOG("PROFILE: %s() cpu_time: %fms, wall_clock: %fms", __func__, cpu_time,  \
        wall_clock_time);                                                      \
  } while (0)
#endif // __x86_64__
#else
#define BEGIN_PROFILED_FUNCTION() do {} while (0)
#define END_PROFILED_FUNCTION() do {} while (0)
#endif // PROFILING

#define LOG(...)                                                               \
  do {                                                                         \
    fprintf(stderr, __VA_ARGS__);                                              \
    fprintf(stderr, "\n");                                                     \
  } while (0)

#endif // COMMON_H
