
#include <slankdev/dpdk.h>



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
