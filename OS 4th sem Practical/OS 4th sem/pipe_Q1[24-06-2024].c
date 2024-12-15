// Replicate these - ls -l | wc -l ] using C by making use of (pipe, mkfifo, open, write, close) and any other required system calls such as fork, exec, etc.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() 
{
    int pipe_fd[2];
    pid_t pid;

    // Create a pipe
    if (pipe(pipe_fd) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) 
    {
        // Child process
        // Close Unused Read End
        close(pipe_fd[0]);

        // Redirect stdout to the write end of the pipe
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) // 'dup2' duplicates 'pipe_fd[1]' to STDOUT_FILENO (file descriptor for standard output).
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Close the write end of the pipe (it is already duplicated)
        close(pipe_fd[1]);

        // Execute "ls -l"
        execlp("ls", "ls", "-l", (char *)NULL);
        // If execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    } 
    else 
    {
        // Parent process
        // Close Unused Write End
        close(pipe_fd[1]);

        // Redirect stdin to the read end of the pipe
        if (dup2(pipe_fd[0], STDIN_FILENO) == -1)  // 'dup2' duplicates 'pipe_fd[0]' to STDIN_FILENO (file descriptor for standard input).
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Close the read end of the pipe (it is already duplicated)
        close(pipe_fd[0]);

        // Execute "wc -l"
        execlp("wc", "wc", "-l", (char *)NULL);
        // If execlp fails
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    // Wait for child process to complete
    wait(NULL);
    return 0;
}


// Another Way

/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
    int pipefd[2];
    pid_t pid;
    
    // Create pipe
    if (pipe(pipefd) == -1) 
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    // Fork a child process
    pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) 
    {
        // Child process (wc -l)
        close(pipefd[1]);  // Close unused write end
        
        // Redirect stdin to read from pipe
        if (dup2(pipefd[0], STDIN_FILENO) == -1) 
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Close pipe read end (it's already duplicated to stdin)
        close(pipefd[0]);
        
        // Execute wc -l
        execlp("wc", "wc", "-l", NULL);
        
        // execlp failed if we reach here
        perror("execlp");
        exit(EXIT_FAILURE);
    } 
    else 
    {
        // Parent process (ls -l)
        close(pipefd[0]);  // Close unused read end
        
        // Redirect stdout to write to pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) 
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        
        // Close pipe write end (it's already duplicated to stdout)
        close(pipefd[1]);
        
        // Execute ls -l
        execlp("ls", "ls", "-l", NULL);
        
        // execlp failed if we reach here
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    
    return 0;
} */


