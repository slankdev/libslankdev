


#include <stdio.h>
#include "dpdk/system.h"
#include "thread/txrx.h"
#include "thread/worker.h"
#include "thread/rtc.h"
#include "thread/omake.h"


/*
 * TODO: Erase below
 * impelement cpu class startly.
 */
thread_txrx_arg port0_arg;
thread_txrx_arg port1_arg;


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

enum thread_pattern {
    TXRXWK,
    TXRX_WK,
    TX_RX_WK,
    TXRX0_TXRX1_WK,
};
void configure(dpdk::System* sys, thread_pattern pattern)
{
    sys->cpus[1].thrd = {thread_viewer, sys};
    switch (pattern) {
        case TXRXWK:
            sys->cpus[2].thrd = {thread_txrxwk_RTC, sys};
            break;

        case TXRX_WK:
            sys->cpus[2].thrd = {thread_txrx_AP   , sys};
            sys->cpus[3].thrd = {thread_wk        , sys};
            break;

        case TX_RX_WK:
            sys->cpus[2].thrd = {thread_tx_AP     , sys};
            sys->cpus[3].thrd = {thread_rx_AP     , sys};
            sys->cpus[4].thrd = {thread_wk        , sys};
            break;

        case TXRX0_TXRX1_WK:
        {
            port0_arg = {sys, 0};
            port1_arg = {sys, 1};
            sys->cpus[2].thrd = {thread_txrx, &port0_arg};
            sys->cpus[3].thrd = {thread_txrx, &port1_arg};
            sys->cpus[4].thrd = {thread_wk  , sys};
            break;
        }
        default:
            throw slankdev::exception("FAAAAAAA!!!!!");
            break;
    }
}

int main(int argc, char** argv)
{
    dpdk::System::rx_ring_size   = 128;
    dpdk::System::tx_ring_size   = 512;
    dpdk::System::port_bulk_size = 32;

    dpdk::System sys(argc, argv);
    if (sys.ports.size()%2 != 0) return -1;

    configure(&sys, TXRX0_TXRX1_WK);
    sys.launch();
}

