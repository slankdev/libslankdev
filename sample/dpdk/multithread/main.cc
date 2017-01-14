


#include "system.h"
#include "thread.h"


dpdk::System sys;

int main(int argc, char** argv)
{
    sys.boot(argc, argv);

    sys.configure(2, 1, 128, 512);
    sys.cpus[1].func = thread_txrx;
    sys.cpus[3].func = thread_worker;
    sys.cpus[4].func = thread_viewer;

    sys.launch();
}


