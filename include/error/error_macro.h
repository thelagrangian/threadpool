#include <stdio.h>
#include <errno.h>


#define POSIX_CHECK(code) do {                                                             \
    int retval = (code);                                                                   \
    if (retval != 0) {                                                                     \
        fprintf(stderr, "POSIX error returned %d at %s:%d\n", retval, __FILE__, __LINE__); \
        exit(EXIT_FAILURE);                                                                \
    }                                                                                      \
} while(0)
