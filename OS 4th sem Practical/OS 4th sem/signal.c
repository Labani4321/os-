#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int count = 0;

void alarm_handler(int signum) {
    // Print the message
    printf("Alarm %d: Message printed after 15 seconds.\n", count + 1);
    
    // Increment the count
    count++;
    
    // Check if it's the 11th time and terminate if so
    if (count >= 10) {
        printf("Terminating program after 10 alarms.\n");
        exit(0);
    }
    
    // Reset the alarm for 15 seconds
    alarm(15);
}

int main() {
    // Set the alarm handler for SIGALRM
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        perror("Failed to set signal handler");
        return 1;
    }
    
    // Initial alarm set for 15 seconds
    alarm(15);
    
    // Loop indefinitely
    while (1) {
        // Sleep to allow the signal handler to execute
        sleep(1);
    }
    
    return 0;
}

