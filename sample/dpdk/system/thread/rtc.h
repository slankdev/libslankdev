
#pragma once


int thread_txrxwk(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);

    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            dpdk::Port& in_port = sys->ports[pid];
            dpdk::Port& out_port = sys->ports[pid^1];

            in_port.rx_burst();

            rte_mbuf* m;
            in_port.rxq[0].pop(&m);
            if (m) {
                out_port.txq[0].push(m);
            }

            out_port.tx_burst();
	    }
	}
    return 0;
}


struct thread_txrxwk_bulk_arg {
    dpdk::System* sys;
    size_t bulk_size;

    thread_txrxwk_bulk_arg(dpdk::System* s, size_t t)
        : sys(s), bulk_size(t) {}
};
int thread_txrxwk_bulk(void* _arg)
{
    thread_wk_bulk_arg* arg = reinterpret_cast<thread_wk_bulk_arg*>(_arg);

    dpdk::System* sys = arg->sys;
    size_t bulk_size  = arg->bulk_size;

    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            dpdk::Port& in_port = sys->ports[pid];
            dpdk::Port& out_port = sys->ports[pid^1];

            in_port.rx_burst_bulk(bulk_size);

            struct rte_mbuf* pkts[bulk_size];
            in_port.rxq[0].pop_bulk(pkts, bulk_size);
            out_port.txq[0].push_bulk(pkts, bulk_size);

            out_port.hk(bulk_size);
	    }
	}
    return 0;
}
