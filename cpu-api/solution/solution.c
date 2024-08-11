#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int x = 100;
  printf("Start: x = %d\n", x);

  // Fork the process
  pid_t pid = fork();

  if (pid < 0) {
    // Fork failed
    fprintf(stderr, "Fork failed\n");
    return 1;
  } else if (pid == 0) {
    // Child process
    printf("This is the child process - pid: %d\n", getpid());
    printf("Initial value of x in the child process: %d\n", x);
    x = 200;
    printf("Modified value of x in the child process: %d\n", x);
    exit(0);
  } else {
    // Parent process
    printf("This is the parent process - pid: %d\n", getpid());
    // Wait for the child process to finish
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
      printf("Child process exited with status: %d\n", WEXITSTATUS(status));
    }
    printf("Value of x in the parent process after fork: %d\n", x);
    x = 300;
    printf("Modified value of x in the parent process: %d\n", x);
  }

  return 0;
}
