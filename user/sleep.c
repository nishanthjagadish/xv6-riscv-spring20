#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{

 int count;

 if (argc!=2)
  {
   printf("Error! Please write the command properly!");
  }
else{
   count=atoi(argv[1]);
   sleep(count);
}

exit();
}
