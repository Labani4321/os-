#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>

void main()
{
	for(int i=0;i<5;i++) // loop will run n times (n=5)
	{
		if(fork() == 0)
		{
            printf("The Random Number is %d\n",(rand()%16));
            printf("\nMY PID: %d MY DAD's PID : %d\n ",getpid(),getppid());
            sleep(10);			
            exit(0);
		}
	}
	for(int i=0;i<5;i++){ // loop will run n times (n=5)
        wait(NULL);
    }
    printf("\n I am the AllFather , MY PID: %d & MY DAD/Session PID: %d\n\nExiting ..\n",getpid(),getppid());
	
}