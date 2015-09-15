#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/*
Madison McHam 
Question 3.21 for OS CS426

*/

int main(int argc, char * argv[])
{
    int num =0;

    //Validate input
    if ( 2 > argc)
    {
        printf("Not enough arguments. \n");
        return 1;    
        }

    num = atoi(argv[1]);
    if (0 >= num) 
    {
        printf("Number must be positive. \n");
        return 1;
        }

    // Valid input    
    int pid = fork();
    if (0 > pid) 
    {
        printf("Unable to fork child process. Exiting.\n");
        }

    else if (pid == 0)
    {
        //parent process
        wait(NULL);
        //printf("Parent process is done.\n");
        }
    else {

        //printf("Parents is waiting on child process...\n");
        //printf("Child is working...\n");
        while (num > 1)
        {
            printf("%1d, ",num);
        
            if (num%2 == 0)
            {
                num = num/2;
                }
            else
            {
                    num = 3 * (num) + 1;
                } 
            }
            
            printf("%1d\n",num);
           // printf("Child process is done.\n");
            }
        
        return 0; 
    }
        
    
