#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

// idk sounds good to me
#define NUM_ITERATIONS 1000000

int main() {
  struct timeval start, end;
  int fd = open("/dev/null", O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 1;
  }

  // Warm-up loop to ensure any startup overhead is not measured
  for (int i = 0; i < 1000; i++) {
    read(fd, NULL, 0);
  }

  // Measure time before performing system calls
  gettimeofday(&start, NULL);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    read(fd, NULL, 0);
  }

  // Measure time after performing system calls
  gettimeofday(&end, NULL);

  close(fd);

  // Calculate the elapsed time in microseconds
  long elapsed_time =
      (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
  double avg_time = (double)elapsed_time / NUM_ITERATIONS;

  printf("Average time per system call: %.2f microseconds\n", avg_time);

  return 0;
}
