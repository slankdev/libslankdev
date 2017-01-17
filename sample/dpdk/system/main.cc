


#include "system.h"
#include "thread.h"


int main(int argc, char** argv)
{
    dpdk::System sys(argc, argv);
    sys.configure(1, 2, 128, 512);

    sys.cpus[1].func = thread_txrx;
    sys.cpus[1].arg  = &sys;
    sys.cpus[2].func = thread_worker;
    sys.cpus[2].arg  = &sys;
    sys.cpus[3].func = thread_viewer;
    sys.cpus[3].arg  = &sys;

    sys.launch();
}


