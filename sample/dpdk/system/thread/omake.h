
#pragma once

int thread_rxwk(void* arg)
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
	    }
	}
    return 0;
}

int thread_txwk(void* arg)
{
    dpdk::System* sys = reinterpret_cast<dpdk::System*>(arg);

    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            dpdk::Port& in_port = sys->ports[pid];
            dpdk::Port& out_port = sys->ports[pid^1];

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


struct thread_rxwk_bulk_arg {
    dpdk::System* sys;
    size_t bulk_size;

    thread_rxwk_bulk_arg(dpdk::System* s, size_t t)
        : sys(s), bulk_size(t) {}
};
int thread_rxwk_bulk(void* _arg)
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
	    }
	}
    return 0;
}


struct thread_txwk_bulk_arg {
    dpdk::System* sys;
    size_t bulk_size;

    thread_txwk_bulk_arg(dpdk::System* s, size_t t)
        : sys(s), bulk_size(t) {}
};
int thread_txwk_bulk(void* _arg)
{
    thread_wk_bulk_arg* arg = reinterpret_cast<thread_wk_bulk_arg*>(_arg);

    dpdk::System* sys = arg->sys;
    size_t bulk_size  = arg->bulk_size;

    const uint8_t nb_ports = sys->ports.size();
	for (;;) {
        for (uint8_t pid = 0; pid < nb_ports; pid++) {
            dpdk::Port& in_port = sys->ports[pid];
            dpdk::Port& out_port = sys->ports[pid^1];

            struct rte_mbuf* pkts[bulk_size];
            in_port.rxq[0].pop_bulk(pkts, bulk_size);
            out_port.txq[0].push_bulk(pkts, bulk_size);

            out_port.tx_burst_bulk(bulk_size);
	    }
	}
    return 0;
}


