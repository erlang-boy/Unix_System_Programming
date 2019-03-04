#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BLKSIZE 1024

int 
copyfile(int fromfd, int tofd) 
{
    char* bp = NULL;
    char buf[BLKSIZE] = {0};
    int bytesread, byteswritten = 0;
    int totalbytes = 0;

    for (;;) {
         while(((bytesread = read(fromfd, buf, BLKSIZE)) == -1) && errno == EINTR);
         if (byteswritten < 0)
              break;
         bp = buf;

         while(bytesread > 0) {
             while(((byteswritten = write(tofd, bp, bytesread)) == -1) && errno == EINTR);
             if(byteswritten <= 0)
                  break;
             totalbytes+=byteswritten;
             bytesread -=byteswritten;
             bp += byteswritten;
         }

         if (byteswritten == -1)
              break;
    }
    return totalbytes;
}


int 
main(int argc, char **argv)
{
    int bytesread;
    int childpid;
    int fd, fd1, fd2;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        return 1;
    }

    if ((fd1 = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "Failed to open file %s: %s\n", argv[1], strerror(errno));
        return 1;
    }

    if ((fd2 = open(argv[2], O_RDONLY)) == -1) {
        fprintf(stderr, "Failed to open file %s: %s\n", argv[2], strerror(errno));
        return 1;
    }

    if((childpid = fork()) == -1) {
        perror("Failed to create child process");
        return 1;
    }

    if (childpid > 0) {
        fd = fd1;
    } else {
        fd = fd2;
    }
    
    bytesread = copyfile(fd, STDOUT_FILENO);
    fprintf(stderr, "Bytes read: %d\n", bytesread);
    return 0;
}
