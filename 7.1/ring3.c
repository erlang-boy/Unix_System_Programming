#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

int 
main(int argc, char **argv)
{
  pid_t childpid;
  int fd[2];
  int error;
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

  long procs[nprocs];
  for (i = 1; i < nprocs; i++) {
    if (pipe(fd) == -1) {
      fprintf(stderr, "[%ld]:failed to create pipe %d: %s\n", (long)getpid(), i, strerror(errno));
      return 1;
    }
    if ((childpid = fork()) == -1) {
      fprintf(stderr, "[%ld]:failed to create child %d: %s\n", (long)getpid(), i, strerror(errno));
      return 1;
    }

    if (childpid > 0) { // 父进程
      error = dup2(fd[1], STDOUT_FILENO);
    } else {
      error = dup2(fd[0], STDIN_FILENO);
    }

    if (error == -1) {
      fprintf(stderr, "[%ld]:failed to dup pipes for iteration %d: %s\n", (long)getpid(), i, strerror(errno));
      return 1;
    }

    if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
      fprintf(stderr, "[%ld]:failed to close extra descriptors %d: %s\n", (long)getpid(), i, strerror(errno));
      return 1;
    }

    if (childpid != 0) {
      break;
    } 
  }

  procs[0] = (long)getpid();
  char buf[BUF_SIZE] = {0};
  char *nextID = NULL;
  int readbytes = 0;

  sprintf(buf, "%ld", (long)getpid());
  write(STDOUT_FILENO, buf, strlen(buf)+1);


  readbytes = read(STDIN_FILENO, buf, BUF_SIZE);
  if (readbytes == -1) {
    fprintf(stderr, "[%ld]: failed to read next id, proc: %d\n", (long)getpid(), i);
    return 1;
  }

  procs[i] = atoi(buf);
  wait(NULL);
  for (i = 0; i< nprocs; i++)
  {
     sprintf(buf+i, "%ld ", procs[i]);
  }

  fprintf(stderr, "this is process %d with ID %ld and parent id %ld, buf: %s\n", i, (long)getpid(), (long)getppid(), buf);
  // wait(NULL);
  return 0;
}
