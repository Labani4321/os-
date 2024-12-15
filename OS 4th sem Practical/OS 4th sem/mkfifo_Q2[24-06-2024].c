// Replicate these -[ ls -l > myfifo & wc -l myfifo ] using C by making use of (pipe, mkfifo, open, write, close) and any other required system calls such as fork, exec, etc.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

#define FIFO_NAME "/tmp/myfifo"

int main() 
{
    // Create the FIFO (named pipe)
    if (mkfifo(FIFO_NAME, 0666) == -1) 
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // Fork the process
    pid_t pid = fork();
    if (pid < 0) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) 
    { // Child process
        // Open the FIFO for writing
        int fd = open(FIFO_NAME, O_WRONLY);
        if (fd == -1) 
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Redirect standard output to the FIFO
        if (dup2(fd, STDOUT_FILENO) == -1) 
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd);

        // Execute the "ls -l" command
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);

    } 
    else 
    { // Parent process
        // Open the FIFO for reading
        int fd = open(FIFO_NAME, O_RDONLY);
        if (fd == -1) 
        {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Redirect standard input to the FIFO
        if (dup2(fd, STDIN_FILENO) == -1) 
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(fd);

        // Execute the "wc -l" command
        execlp("wc", "wc", "-l", (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    // Wait for the child process to finish
    wait(NULL);

    // Remove the FIFO
    unlink(FIFO_NAME);

    return 0;
}

