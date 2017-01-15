


#include "system.h"
#include "thread.h"


int main(int argc, char** argv)
{
    dpdk::System sys;
    sys.boot(argc, argv);

    sys.ports[0].conf.raw.rxmode.mq_mode = ETH_MQ_RX_RSS;
    sys.ports[0].conf.raw.rx_adv_conf.rss_conf.rss_hf = ETH_RSS_IP;

    printf("\n\n");
    dpdk::util::print(&sys.ports[0].conf.raw.rxmode);
    dpdk::util::print(&sys.ports[0].conf.raw.rx_adv_conf.rss_conf);
    dpdk::util::print(&sys.ports[0].info.raw);
    printf("\n\n");

    sys.configure(2, 1, 128, 512);

    sys.cpus[1].func = thread_txrx;
    sys.cpus[1].arg  = &sys;
    sys.cpus[3].func = thread_worker;
    sys.cpus[3].arg  = &sys;
    // sys.cpus[4].func = thread_viewer;
    // sys.cpus[4].arg  = &sys;

    // sys.launch();
}


