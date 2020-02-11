#include "kernel/types.h"
#include "user/user.h"

int main()
{
    /* Create the pipe */
    int parent_fd[2], child_fd[2];
    pipe(parent_fd);
    pipe(child_fd);

    int pid = fork();

    if (pid < 0)
    {
        printf ("Fork Failed\n");
        return -1;
    }
    else if(pid == 0)
    {
	char x;
        read(parent_fd[0], &x, sizeof(x));
	printf("%d : Received ping\n",getpid());
        write(child_fd[1], &x, sizeof(x));
    }
    else 
    {
	char y='b';
	write(parent_fd[1], &y, sizeof(y));
	read(child_fd[0], &y,sizeof(y));
	printf("%d : Received pong\n",getpid());
    }

    exit();
}
