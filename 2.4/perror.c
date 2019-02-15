#include <unistd.h>
#include <stdio.h>
#include <string.h>

int fields = 5;

int main(){
  //printf("fields: %d\n", fields);
  if (close(fields) == -1) {
     perror("Failed to close the file");
     printf("Failed to close the file %d: %s\n", fields, strerror(22));
  }
  return 0;
}

