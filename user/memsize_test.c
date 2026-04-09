#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int before, after_malloc, after_free;

    before = memsize();
    printf("Before malloc: %d bytes\n", before);

    char *arr = malloc(20000);

    after_malloc = memsize();
    printf("After malloc: %d bytes\n", after_malloc);

    free(arr);

    after_free = memsize();
    printf("After free: %d bytes\n", after_free);

    return 0;
}