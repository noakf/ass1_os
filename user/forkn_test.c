
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pids[10];
    int res = forkn(3, pids);
    if (res == 0) {
      printf("Parent here! Created: %d children\n", 3);
      for (int i = 0; i < 3; i++) {
        printf("Child %d has PID %d\n", i + 1, pids[i]);
     }
    } else {
      printf("Child number %d here!\n", res);
    }
    exit(0, "");
  }