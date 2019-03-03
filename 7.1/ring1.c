#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int clone(int nprocs)
{
  int fd[2];
  int error = 0;
  pid_t childpid;
  if (nprocs == 0) {
    return 0;
  }


  if (pipe(fd) == -1) {
    fprintf(stderr, "[%ld]:failed to create pipe %d: %s\n", (long)getpid(), nprocs, strerror(errno));
    return 1;
  }
  if ((childpid = fork()) == -1) {
    fprintf(stderr, "[%ld]:failed to create child %d: %s\n", (long)getpid(), nprocs, strerror(errno));
    return 1;
  }

  if (childpid > 0) { // 父进程
    error = dup2(fd[1], STDOUT_FILENO);
  } else {
    error = dup2(fd[0], STDIN_FILENO);
  }

  if (error == -1) {
    fprintf(stderr, "[%ld]:failed to dup pipes for iteration %d: %s\n", (long)getpid(), nprocs, strerror(errno));
    return 1;
  }

  if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
    fprintf(stderr, "[%ld]:failed to close extra descriptors %d: %s\n", (long)getpid(), nprocs, strerror(errno));
    return 1;
  }

  if (childpid == 0) {
    clone(nprocs-1);
  } else {
    wait(NULL);
    fprintf(stderr, "this is process %d with ID %ld and parent id %ld\n", nprocs, (long)getpid(), (long)getppid());
   // wait(NULL);
  }
  return 0;
}

int 
main(int argc, char **argv)
{
  int fd[2];
  int i;
  int nprocs;

  if ((argc != 2) || ((nprocs = atoi(argv[1])) <= 0)) {
    fprintf(stderr, "Usage: %s <nprocs>\n", argv[0]);
    return 1;
  }

  // 将父进程的标准输入输出重定向为管道的读写端
  if (pipe(fd) == -1) {
    perror("Failed to create starting pipe");
    return 1;
  }

  if ((dup2(fd[0], STDIN_FILENO) == -1) ||
     (dup2(fd[1], STDOUT_FILENO) == -1)) {
    perror("Failed to connect pipe");
    return 1;
  }

  if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
    perror("Failed to close extra descriptors");
    return 1;
  }

  // 
  clone(nprocs);

//  char buf[12] = {0};
//  write(STDOUT_FILENO, "hello world\n", 12);
//  read(STDIN_FILENO, buf, 12);
//  fprintf(stderr, "%s", buf);
  return 0;
}
