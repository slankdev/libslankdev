


#include <stdio.h>
#include "system.h"
#include "types.h"


int thread_worker_1shot(void* arg);
int thread_worker_bulk(void* arg);
int thread_txrx(void* arg);
int thread_viewer(void* arg);



int main(int argc, char** argv)
{
    dpdk::System sys(argc, argv);
    sys.configure(128, 512);

    sys.cpus[1].func = thread_txrx;
    sys.cpus[1].arg  = &sys;
    // sys.cpus[2].func = thread_worker_1shot;
    sys.cpus[2].func = thread_worker_bulk;
    sys.cpus[2].arg  = &sys;
    sys.cpus[3].func = thread_viewer;
    sys.cpus[3].arg  = &sys;

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
            in_port.rxq.pop(&m);
            if (m) {
                out_port.txq.push(m);
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
            in_port.rxq.pop(&m);
            if (m) {
                out_port.txq.push(m);
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
                port.rxq.count(), port.rxq.size(),
                port.txq.count(), port.txq.size());

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

