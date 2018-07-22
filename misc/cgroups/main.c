
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#define MB 1024*1024

int main(int argc, char *argv[])
{
    char *p;
    int malloc_size = 10 * MB;
    size_t tot = 0;
    printf("pid: %d \n", getpid());
    printf("waiting to start [press enter]:");
    getchar();

    while(1)
    {
        p = (char *)malloc(malloc_size);
        if (!p) {
          fprintf(stderr, "can't allocate memory\n");
          exit(1);
        }
        tot += malloc_size;
        memset(p, 0, malloc_size);
        if (tot > 1000000000)   printf("\rmem: %.3lf GByte", (double)tot / 1000000000);
        else if (tot > 1000000) printf("\rmem: %.3lf MByte", (double)tot / 1000000);
        else if (tot > 1000)    printf("\rmem: %.3lf MByte", (double)tot / 1000);
        else                    printf("\rmem: %zd Byte   ", tot);
        fflush(stdout);
        usleep(100000);
    }


    return 0;
}

