#import <stdio.h>
#import <signal.h>
#import <unistd.h>
#import <uistlib.h>

int iteration = 0;

void alarmhadler(int sig)

printf ("hello world") ;
iteration++;

if (iteraion==10){
    exit (0)
}
int main ()
signal(SIGALARM,alarm handler);
alarm (3)
while (1){
    pause();
}
return 0;
}