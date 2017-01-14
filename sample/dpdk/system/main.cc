


#include "system.h"
#include "thread.h"


int main(int argc, char** argv)
{
    dpdk::System sys;
    sys.boot(argc, argv);

    sys.configure(2, 1, 128, 512);
    sys.cpus[1].func = thread_txrx;
    sys.cpus[1].arg  = &sys;
    sys.cpus[3].func = thread_worker;
    sys.cpus[3].arg  = &sys;
    sys.cpus[4].func = thread_viewer;
    sys.cpus[4].arg  = &sys;

    // sys.launch();
}


