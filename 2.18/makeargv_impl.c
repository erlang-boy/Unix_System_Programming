
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
makeargv(s, delim, argvp)
  const char *s;
  const char *delim;
  char ***argvp;
{
  int error = 0;
  int i = 0;
  int numtokens = 0;
  const char *snew = NULL;
  char *t = NULL;

  if ((s == NULL) || (delim == NULL) || (argvp == NULL)) {
    errno = EINVAL;
    return -1;
  }

  *argvp = NULL;
  snew = s + strspn(s, delim); // 字符串前几个有可能是delimter

  if ((t = malloc(strlen(snew) + 1)) == NULL) {
    return -1;
  }

  strcpy(t, snew);
  printf("before calc t: %s\n", t);

  // 计算s中子字符串的数量
  if (strtok(t, delim) != NULL) {
    for (numtokens = 1; strtok(NULL, delim) != NULL; numtokens++) {
    }
  }

  if ((*argvp = malloc((numtokens + 1) * sizeof(char*))) == NULL) {
    error = errno;
    free(t);
    errno = error;
    return -1;
  }

  printf("after calc t: %s\n", t);

  if (numtokens == 0) {
    free(t);
  } else {
    strcpy(t, snew); // 这里是否需要进行cp?  需要，strtok计算完子字符数量后，t的值发生了变化
    **argvp = strtok(t, delim); // 第0个
    for (i = 1; i < numtokens; i++) {
      *(*argvp+i) = strtok(NULL, delim);
    }
  }

  *(*argvp + numtokens) = NULL;
//  free(t); // 书中少了一个free(t); 此处增加free(t)后，系统报重复释放错误
  return numtokens;
}
/*
 * strtok(t, delim)
 *    1. 会修改t的值
 *    2. 会释放t对应的mem
 */

void freemakeargv(argv)
  char **argv;
{
  if (argv == NULL)
    return;
  if (*argv != NULL) 
    free(*argv);
  free(argv);
}

