
#include <stdio.h>
#include <rte_eal.h>
volatile bool force_quit;
#include <lthread_api.h>

static void lthread_start(void* arg)
{
    int* i = reinterpret_cast<int*>(arg);
    while (1) {
        printf("test %d\n", *i);
        lthread_yield ();
    }
    lthread_exit (NULL);
}

int main(int argc, char** argv)
{
    rte_eal_init(argc, argv);

    struct lthread *lt[2];
    int one = 1, two = 2;
    lthread_create (&lt[0], -1, lthread_start, &one);
    lthread_create (&lt[1], -1, lthread_start, &two);
    lthread_run ();
    printf("lthread finished \n");
}
