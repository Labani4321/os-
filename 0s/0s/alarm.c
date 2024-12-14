#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// Global variable to count iterations
int iteration = 0;

// Signal handler for SIGALRM
void alarm_handler(int sig) {
    // Print "hello world" every 3 seconds
    printf("hello world\n");
    iteration++;

    // Exit after 11 iterations
    if (iteration == 11) {
        exit(0);
    }

    // Set the alarm again for 3 seconds
    alarm(3);
}

int main() {
    // Set up signal handler for SIGALRM
    signal(SIGALRM, alarm_handler);

    // Set the initial alarm for 3 seconds
    alarm(3);

    // Loop indefinitely (will exit on 11th iteration due to exit(0) in handler)
    while(1) {
        // Waiting for signals
        pause();
    }

    return 0;
}
