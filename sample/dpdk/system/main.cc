


#include <stdio.h>
#include "dpdk/system.h"
#include "thread/shot.h"
#include "thread/bulk.h"
#include "thread/omake.h"


static void ifconfig(dpdk::System* sys)
{
    for (dpdk::Port& port : sys->ports) {
        port.stats.update();

        printf("%s\n", port.name.c_str());
        printf("  HWaddr %s \n", port.addr.toString().c_str());

        auto& stats = port.stats;
        printf("  RX packets:%lu errors:%lu dropped:%lu allocmiss:%lu \n",
                    stats.raw.ipackets, stats.raw.ierrors,
                    stats.raw.imissed, stats.raw.rx_nombuf);
        printf("  TX packets:%lu errors:%lu  \n",
                stats.raw.opackets, stats.raw.oerrors);
        printf("  RX bytes:%lu TX bytes:%lu \n", stats.raw.ibytes, stats.raw.obytes);
        printf("  RX ring:%zd/%zd TX ring:%zd/%zd\n",
                port.rxq[0].count(), port.rxq[0].size(),
                port.txq[0].count(), port.txq[0].size());

    }
}


int thread_viewer(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
	for (;;) {
        slankdev::clear_screen();
        ifconfig(sys);
		usleep(50000);
	}
	return 0;
}


int main(int argc, char** argv)
{
    dpdk::System::rx_ring_size = 128;
    dpdk::System::tx_ring_size = 512;
    dpdk::System sys(argc, argv);

    sys.cpus[1].func = thread_txrx;
    sys.cpus[1].arg  = &sys;
    sys.cpus[2].func = thread_wk;
    sys.cpus[2].arg  = &sys;
    sys.cpus[3].func = thread_viewer;
    sys.cpus[3].arg  = &sys;
    if (sys.ports.size() != 2) {
        fprintf(stderr, "number of ports is not 2 \n");
        return -1;
    }

    sys.launch();
}

