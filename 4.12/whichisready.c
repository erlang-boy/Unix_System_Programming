#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>

int whichisready(int fd1, int fd2)
{
     int maxfd;
     int nfds;

     fd_set readset;

     if ((fd1 < 0) || fd1 >= FD_SETSIZE || (fd1 < 0) || fd1 >= FD_SETSIZE) {
          errno = EINVAL;   
          return -1;
     }

     maxfd = (fd1 > fd2) ? fd1: fd2;
     FD_ZERO(&readset); // 初始化

     FD_SET(fd1, &readset);
     FD_SET(fd2, &readset);
     nfds = select(maxfd+1, &readset, NULL, NULL, NULL);
     if (nfds == -1)
         return -1;

     if (FD_ISSET(fd1, &readset))
         return fd1;
              
     if (FD_ISSET(fd1, &readset))
         return fd2;

     errno = EINVAL;
     return -1;
}

int 
main(int argc, char **argv)
{
 //   int bytesread;
 //   int childpid;
 //   int fd, fd1, fd2;

      int fd;
 //   if (argc != 3)
 //   {
 //       fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
 //       return 1;
 //   }

 //   if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
 //       fprintf(stderr, "Failed to open file %s: %s\n", argv[1], strerror(errno));
 //       return 1;
 //   }

 //   if ((fd2 = open(argv[2], O_RDONLY)) == -1) {
 //       fprintf(stderr, "Failed to open file %s: %s\n", argv[2], strerror(errno));
 //       return 1;
 //   }

 //   whichisready(fd1, fd2);
      fprintf(stderr, "write stdout haha\n");
      for(;;)
      {
        fd = whichisready(STDIN_FILENO, STDERR_FILENO);
        fprintf(stderr, "ready fd: %d\n", fd);
      }
//    fprintf(stderr, "1 fd1: %d fd2: %d, return fd: %d\n", fd1, fd2, whichisready(fd1, fd2));

    return 0;
}
