
#pragma once


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



