#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile int count = 0;

void alarm_handler(int signum) {

    printf("Alarm %d: Message printed after 15 seconds.\n", count + 1);
    count++;
    
    if (count >= 10) {
        printf("Terminating program after 10 alarms.\n");
        exit(0);
    }
    alarm(15);
}

int main() {

    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        perror("Failed to set signal handler");
        return 1;
    }
    alarm(15);
    while (1) {
        sleep(1);
    }
    
    return 0;
}

