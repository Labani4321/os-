#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>

int count = 1;
int max_count = 11;
struct sigaction old_action;
pid_t proid;

void sigalrm_handler(int signum) {
    printf("Alarm triggered %d times by pid = %d\n", count, proid);
    count++;

    if (count >= max_count) {
        sigaction(SIGALRM, &old_action, NULL);
    }

    alarm(1);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = &sigalrm_handler;
    sigaction(SIGALRM, &sa, &old_action);

    proid = fork();
    if(proid == 0){
        alarm(1);

        while(1){
            pause();
        }
    }

    alarm(1);

    while (1) {
        pause();
    }

    return 0;
}
