#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    pid_t pid;
    int status;
    int child_pids[5];  // Store child PIDs

    // Seed the random number generator
    srand(time(NULL));

    // Create 5 child processes
    for (int i = 0; i < 5; i++) {
        pid = fork();
        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process
            int sleep_time = rand() % 4 + 2; // Random sleep time between 2 and 5 seconds
            int return_value = rand() % 16 + 10; // Random number between 10 and 25

            printf("Child PID: %d, Sleep Time: %d seconds, Return Value: %d\n", getpid(), sleep_time, return_value);

            sleep(sleep_time);
            exit(return_value); // Exit with the random number
        } else {
            // Parent process
            child_pids[i] = pid; // Store child PID
        }
    }

    // Parent waits for all child processes
    for (int i = 0; i < 5; i++) {
        pid_t cpid = wait(&status);

        if (cpid == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            int ret_val = WEXITSTATUS(status);
            printf("Child with PID %d exited with return value %d\n", cpid, ret_val);
        } else {
            printf("Child with PID %d did not exit normally\n", cpid);
        }
    }

    printf("Parent PID: %d is exiting\n", getpid());

    return 0;
}
