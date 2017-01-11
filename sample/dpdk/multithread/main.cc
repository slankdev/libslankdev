

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <slankdev/dpdk.h>
#include <slankdev/exception.h>
#include <vector>

#define UNUSED(x) (void)(x)


#define RX_RING_SIZE 128
#define TX_RING_SIZE 512
#define NUM_MBUFS 8192
#define MBUF_CACHE_SIZE 0
#define BURST_SIZE 32
#define MBUF_SIZ RTE_MBUF_DEFAULT_BUF_SIZE


template <class T>
class ring {
    struct rte_ring* ring_;
public:
    ring() : ring_(nullptr) {}
    void init(const char* name, size_t count, uint16_t socket_id)
    {
        ring_ = rte_ring_create(name, count, socket_id, 0);
        if (!ring_)
            throw slankdev::exception("rte_ring_create");
    }
    ~ring()
    {
        if (!ring_)
            rte_ring_free(ring_);
    }
    void push(T* data)
    {
        int ret = rte_ring_enqueue(ring_, data);
        if (ret < 0)
            throw slankdev::exception("rte_ring_enqueue");
    }
    void push_bulk(T** data_table, size_t n)
    {
        int ret = rte_ring_enqueue_bulk(ring_, data_table, n);
        if (ret < 0)
            throw slankdev::exception("rte_ring_enqueue_bulk");
    }
    void pop(T** data)
    {
        int ret = rte_ring_dequeue(ring_, reinterpret_cast<void**>(data));
        if (ret < 0)
            throw slankdev::exception("rte_ring_dequeue");
    }
    void pop_bulk(T** data_table, size_t n)
    {
        int ret = rte_ring_dequeue_bulk(ring_, reinterpret_cast<void**>(data_table), n);
        if (ret < 0)
            throw slankdev::exception("rte_ring_dequeue_bulk");
    }
    size_t size()
    {
        return rte_ring_count(ring_);
    }
};


size_t rx_burst(uint8_t port, rte_mbuf** bufs, size_t nb_bufs)
{
    const uint16_t nb_rx = rte_eth_rx_burst(port, 0, bufs, nb_bufs);
    return nb_rx;
}

void tx_burst(uint8_t port, rte_mbuf** bufs, size_t nb_bufs)
{
    const uint16_t nb_tx = rte_eth_tx_burst(port, 0, bufs, nb_bufs);
    if (nb_tx != nb_bufs)
        throw slankdev::exception("rte_eth_tx_burst");
}


ring<rte_mbuf> ring0;

int thread_worker(void* arg)
{
    UNUSED(arg);
    while (1) {
        sleep(1);
        printf("fa\n");
    }
    return 0;
}

int thread_txrx(void* arg)
{
    UNUSED(arg);
    const uint8_t nb_ports = rte_eth_dev_count();
	for (;;) {
        for (uint8_t port = 0; port < nb_ports; port++) {
            struct rte_mbuf *bufs[BURST_SIZE];
            const uint16_t nb_rx = rx_burst(port, bufs, BURST_SIZE);
            if (nb_rx == 0) continue;

            static uint8_t C = 0x01;
            uint8_t* p = rte_pktmbuf_mtod(bufs[0], uint8_t*);
            for (int i=0; i<6; i++) p[i] = C;
            C++;

            ring0.push(bufs[0]);
            printf("enque %p \n", bufs[0]);

            if (ring0.size() > 4) {
                struct rte_mbuf *bb[100];
                ring0.pop_bulk(bb, 5);
                tx_burst(port, bb, 5);
                printf("deque_bulk\n");
            }
	    }
	}
    return 0;
}

int thread_tx(void* arg)
{
    UNUSED(arg);
    return 0;
}
int thread_rx(void* arg)
{
    UNUSED(arg);
    return 0;
}


int main(int argc, char** argv)
{
    slankdev::dpdk_boot(argc, argv);

    slankdev::pool mp;
    mp.create("Pool0", NUM_MBUFS*rte_eth_dev_count(), MBUF_CACHE_SIZE, MBUF_SIZ, rte_socket_id());

    slankdev::port_conf conf;
    uint16_t nb_ports = rte_eth_dev_count();
	for (uint16_t portid = 0; portid < nb_ports; portid++) {
        port_init(portid, &mp, &conf, 1, 1, RX_RING_SIZE, TX_RING_SIZE);

        slankdev::ether_addr addr;
        rte_eth_macaddr_get(portid, &addr);

        printf("Port %u MAC: ", (unsigned)portid);
        addr.print(stdout);
        printf("\n");
    }

    ring0.init("SLANKDEV", 16, 0);
#if 1
    rte_eal_remote_launch(thread_txrx  , nullptr, 1);
    rte_eal_remote_launch(thread_worker, nullptr, 2);
#else
    rte_eal_remote_launch(thread_rx    , nullptr, 1);
    rte_eal_remote_launch(thread_worker, nullptr, 2);
    rte_eal_remote_launch(thread_tx    , nullptr, 3);
#endif
    rte_eal_mp_wait_lcore();
}

