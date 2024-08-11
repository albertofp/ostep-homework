package main

import (
	"fmt"
	"os"
	"syscall"
)

func main() {
	x := 100
	fmt.Println("Start: x = ", x)
	pid, _, errno := syscall.Syscall(syscall.SYS_FORK, 0, 0, 0)
	if errno != 0 {
		fmt.Fprintf(os.Stderr, "Fork failed: %v\n", errno)
		os.Exit(1)
	}
	if pid == 0 {
		// Child process
		fmt.Printf("This is the child process - pid: %d\n", pid)
		fmt.Printf("Initial value of x in the child process: %d\n", x)
		x = 200
		fmt.Printf("Modified value of x in the child process: %d\n", x)
	} else {
		// Parent process
		fmt.Printf("This is the parent process - pid: %d\n", pid)
		syscall.Wait4(int(pid), nil, 0, nil)
		fmt.Printf("Value of x in the parent process after fork: %d\n", x)
		x = 300
		fmt.Printf("Modified value of x in the parent process: %d\n", x)
	}

	syscall.Wait4(int(pid), nil, 0, nil)
}
