#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_ITERATIONS 10000

int main() {
  int pipe1[2], pipe2[2];
  if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
    perror("pipe");
    return 1;
  }

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork");
    return 1;
  }

  if (pid == 0) {
    // Child process
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) {
      perror("sched_setaffinity");
      exit(1);
    }

    char buf;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      // Wait for data from the parent
      read(pipe1[0], &buf, 1);
      // Send data to the parent
      write(pipe2[1], "c", 1);
    }

    exit(0);
  } else {
    // Parent process
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);
    if (sched_setaffinity(0, sizeof(cpu_set_t), &set) == -1) {
      perror("sched_setaffinity");
      return 1;
    }

    struct timeval start, end;
    char buf;

    // Start the time measurement
    gettimeofday(&start, NULL);

    for (int i = 0; i < NUM_ITERATIONS; i++) {
      // Send data to the child
      write(pipe1[1], "p", 1);
      // Wait for data from the child
      read(pipe2[0], &buf, 1);
    }

    // End the time measurement
    gettimeofday(&end, NULL);

    // Calculate the elapsed time in microseconds
    long elapsed_time =
        (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
    double avg_time = (double)elapsed_time / (2 * NUM_ITERATIONS);

    printf("Average time per context switch: %.2f microseconds\n", avg_time);

    wait(NULL); // Wait for the child process to finish
  }

  return 0;
}
