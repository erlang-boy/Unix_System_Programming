#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    pid_t childpid = 0;
    int i, n;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s processes\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    for (i = 1; i < n; i++)
    {
        if (childpid = fork() > 0)
          break;
    }
    fprintf(stdout, "i: %d  %ld ---> %ld ---> %ld\n\n", i,  (long)getppid(), (long)getpid(), (long)childpid);
    sleep(100);

    return 0;
}
