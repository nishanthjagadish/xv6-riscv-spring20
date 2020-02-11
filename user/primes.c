#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  int numbers[100], count = 0, i;
  int fd[2];
  for (i = 2; i <= 35; i++) {
    numbers[count++] = i;
  }
  while (count > 0) {
    pipe(fd);
    if (fork() == 0) {
      int prime, this_prime = 0;
      close(fd[1]);
      count = -1;
      while (read(fd[0], &prime, sizeof(prime)) != 0) {
        if (count == -1) {
          this_prime = prime;
          count = 0;
        } else {
          if (prime % this_prime != 0) numbers[count++] = prime;
        }
      }
      printf("prime %d\n", this_prime);
      close(fd[0]);
    } else {
      close(fd[0]);
      for (i = 0; i < count; i++) {
        write(fd[1], &numbers[i], sizeof(numbers[0]));
      }
      close(fd[1]);
      wait();
      break;
    }
  }
  exit();
}
