#include "kernel/types.h"
#include "user/user.h"

void
test_errors()
{
  int pid = getpid();
  int cpid;
  int r;

  printf("\n[TEST] co_yield error cases\n");

  r = co_yield(0, 10);
  printf("pid 0 returned: %d\n", r);

  r = co_yield(-1, 10);
  printf("negative pid returned: %d\n", r);

  r = co_yield(pid, 10);
  printf("self yield returned: %d\n", r);

  r = co_yield(9999, 10);
  printf("non-existing pid returned: %d\n", r);

  r = co_yield(pid + 1, -5);
  printf("negative value returned: %d\n", r);

  cpid = fork();

  if(cpid == 0){
    exit(0);
  }

  /*
   * Wait a bit so the child exits and becomes ZOMBIE.
   * We do not call wait yet, because wait would remove it.
   */
  sleep(10);

  r = co_yield(cpid, 10);
  printf("zombie pid returned: %d\n", r);

  wait(0);

  printf("\nerror tests done\n");
}

int
main()
{
  test_errors();
  exit(0);
}