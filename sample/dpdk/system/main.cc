


#include <stdio.h>
#include "system.h"
#include "types.h"


int thread_worker_1shot(void* arg);
int thread_worker_bulk(void* arg);
int thread_txrx(void* arg);
int thread_viewer(void* arg);

size_t dpdk::System::rx_ring_size = 128;
size_t dpdk::System::tx_ring_size = 512;


int main(int argc, char** argv)
{
    dpdk::System::rx_ring_size = 128;
    dpdk::System::tx_ring_size = 512;
    dpdk::System sys(argc, argv);

    sys.cpus[1].func = thread_txrx;
    sys.cpus[1].arg  = &sys;
    sys.cpus[2].func = thread_worker_bulk;
    sys.cpus[2].arg  = &sys;
    sys.cpus[3].func = thread_viewer;
    sys.cpus[3].arg  = &sys;
    if (sys.ports.size() != 2) {
        fprintf(stderr, "number of ports is not 2 \n");
        return -1;
    }

    sys.launch();
}


int thread_worker_1shot(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
		for (uint8_t pid=0; pid<nb_ports; pid++) {
            dpdk::Port& in_port  = sys->ports[pid];
            dpdk::Port& out_port = sys->ports[pid^1];

            rte_mbuf* m;
            in_port.rxq[0].pop(&m);
            if (m) {
                out_port.txq[0].push(m);
            }
	    }
	}
    return 0;
}
int thread_worker_bulk(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
	for (;;) {
		const uint8_t nb_ports = sys->ports.size();
		for (uint8_t pid=0; pid<nb_ports; pid++) {

            dpdk::Port& in_port  = sys->ports[pid];
            dpdk::Port& out_port = sys->ports[pid^1];

            rte_mbuf* m = nullptr;
            in_port.rxq[0].pop(&m);
            if (m) {
                out_port.txq[0].push(m);
            }
	    }
	}
    return 0;
}
int thread_txrx(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].rx_burst();
            sys->ports[pid].tx_burst();
	    }
	}
    return 0;
}
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



#if 0
int thread_tx(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].tx_burst();
	    }
	}
}
int thread_rx(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);
    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].rx_burst();
	    }
	}
    return 0;
}
#endif

