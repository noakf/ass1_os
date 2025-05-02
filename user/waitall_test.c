#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pids[3];
    int n = 0;
    int statuses[3];
   // printf("before forkn\n");
    int res = forkn(3, pids);
    //printf("after forkn\n");
    if (res == 0) {
        // Parent process
        for (int i = 0; i < 3; i++) {
            printf("Parent created child %d with PID %d\n", i + 1, pids[i]);
        }

        int rc = waitall(&n, statuses);
        if (rc < 0) {
            printf("waitall failed\n");
        } else {
            printf("waitall returned %d children:\n", n);
            for (int i = 0; i < n; i++) {
                printf("Child #%d exited with status %d\n", i + 1, statuses[i]);
            }
        }
    } else {
        // Child process
        printf("Child #%d (pid=%d) is exiting with status %d\n", res, getpid(), res * 10);
        exit(res * 10, "bye from child");
    }

    exit(0, "bye parent");
}