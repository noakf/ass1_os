#include "kernel/types.h"
#include "user/user.h"

    int main() {
    printf("Initial memsize: %d bytes\n", memsize());

    void* ptr = malloc(20000);  
    printf("After malloc: %d bytes\n", memsize());

    free(ptr);
    printf("After free: %d bytes\n", memsize());
    exit(0,"");
    }