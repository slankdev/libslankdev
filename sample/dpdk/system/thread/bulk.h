
#pragma once



struct thread_txrx_bulk_arg {
    dpdk::System* sys;
    size_t rx_bulk_size;
    size_t tx_bulk_size;

    thread_txrx_bulk_arg(dpdk::System* s, size_t r, size_t t)
        : sys(s), rx_bulk_size(r), tx_bulk_size(t) {}
};
int thread_txrx_bulk(void* _arg)
{
    thread_txrx_bulk_arg* arg = reinterpret_cast<thread_txrx_bulk_arg*>(_arg);

    dpdk::System* sys = arg->sys;
    size_t tx_bulk_size = arg->tx_bulk_size;
    size_t rx_bulk_size = arg->rx_bulk_size;

    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].rx_burst_bulk(rx_bulk_size);
            sys->ports[pid].tx_burst_bulk(tx_bulk_size);
	    }
	}
    return 0;
}
struct thread_tx_bulk_arg {
    dpdk::System* sys;
    size_t tx_bulk_size;

    thread_tx_bulk_arg(dpdk::System* s, size_t t)
        : sys(s), tx_bulk_size(t) {}
};
int thread_tx_bulk(void* _arg)
{
    thread_tx_bulk_arg* arg = reinterpret_cast<thread_tx_bulk_arg*>(_arg);

    dpdk::System* sys   = arg->sys;
    size_t tx_bulk_size = arg->tx_bulk_size;

    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].tx_burst_bulk(tx_bulk_size);
	    }
	}
}
struct thread_rx_bulk_arg {
    dpdk::System* sys;
    size_t rx_bulk_size;

    thread_rx_bulk_arg(dpdk::System* s, size_t t)
        : sys(s), rx_bulk_size(t) {}
};
int thread_rx_bulk(void* _arg)
{
    thread_rx_bulk_arg* arg = reinterpret_cast<thread_rx_bulk_arg*>(_arg);

    dpdk::System* sys   = arg->sys;
    size_t rx_bulk_size = arg->rx_bulk_size;

    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            sys->ports[pid].rx_burst_bulk(rx_bulk_size);
	    }
	}
    return 0;
}

struct thread_wk_bulk_arg {
    dpdk::System* sys;
    size_t bulk_size;

    thread_wk_bulk_arg(dpdk::System* s, size_t b)
        : sys(s), bulk_size(b) {}
};
int thread_wk_bulk(void* _arg)
{
    thread_wk_bulk_arg* arg = reinterpret_cast<thread_wk_bulk_arg*>(_arg);

    dpdk::System* sys = arg->sys;
    size_t bulk_size  = arg->bulk_size;

	for (;;) {
		const uint8_t nb_ports = sys->ports.size();
		for (uint8_t pid=0; pid<nb_ports; pid++) {

            dpdk::Port& in_port  = sys->ports[pid];
            dpdk::Port& out_port = sys->ports[pid^1];

            struct rte_mbuf* pkts[bulk_size];
            in_port.rxq[0].pop_bulk(pkts, bulk_size);
            out_port.txq[0].push_bulk(pkts, bulk_size);
	    }
	}
    return 0;
}


