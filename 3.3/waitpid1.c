#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char**argv) {
  pid_t childpid;
//  int i, n;
//  if (argc != 2) {
//     fprintf(stderr, "Usage: %s n\n", argv[0]);
//     return 1;
//  }
//
//  n = atoi(argv[1]);
//  for (i = 1; i < n; i++){
    if ((childpid = fork()) == 0)
      wait(NULL);
    else 
      wait(NULL);

//  }

//  while(wait(NULL) > 0);

  fprintf(stderr, "process ID: %ld parent ID: %ld child ID: %ld\n", (long)getpid(), (long)getppid(), (long)childpid);
//  fprintf(stderr, "i: %d process ID: %ld parent ID: %ld child ID: %ld\n", i, (long)getpid(), (long)getppid(), (long)childpid);
  return 0;
}
