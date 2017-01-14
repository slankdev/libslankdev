


#include "system.h"
#include "thread.h"


dpdk::System sys;

int main(int argc, char** argv)
{
    rte_eth_conf c;
    memset(&c, 0, sizeof(c));
    printf("\n\n\n\n");
    dpdk_struct_utils::print(&c);
    return 0;


    sys.boot(argc, argv);

    sys.configure(2, 1, 128, 512);
    sys.cpus[1].func = thread_txrx;
    sys.cpus[3].func = thread_worker;
    sys.cpus[4].func = thread_viewer;

    sys.launch();
}


