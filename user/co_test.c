#include "kernel/types.h"
#include "user/user.h"

int
main()
{
  int pid1 = getpid();
  int pid2 = fork();

  if(pid2 == 0){
    // child
    for(;;){
      int value = co_yield(pid1, 1);
      printf("Child received: %d\n", value);
    }
  } else {
    // parent
    for(;;){
      int value = co_yield(pid2, 2);
      printf("Parent received: %d\n", value);
    }
  }

  exit(0);
}