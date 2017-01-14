


#pragma once
#include "types.h"


namespace dpdk {



class Ring {
    struct rte_ring* ring_;
	size_t ring_depth;
public:
    Ring() : ring_(nullptr), ring_depth(0) {}
    void init(const char* name, size_t count, uint16_t socket_id)
    {
		ring_depth = count;
        ring_ = rte_ring_create(name, count, socket_id, 0);
        if (!ring_)
            throw slankdev::exception("rte_ring_create");
    }
    ~Ring()
    {
        if (!ring_)
            rte_ring_free(ring_);
    }
    void push(rte_mbuf* data)
    {
        int ret = rte_ring_enqueue(ring_, data);
		if (ret < 0) {
			if (ret == -EDQUOT) {
				throw slankdev::exception("rte_ring_enqueue: quota exceeded");
			} else if (ret == -ENOBUFS) {
				// throw slankdev::exception("rte_ring_enqueue: no buf");
			} else {
				throw slankdev::exception("rte_ring_enqueue: unknown");
			}
		}
    }
    void pop(rte_mbuf** data)
    {
        int ret = rte_ring_dequeue(ring_, reinterpret_cast<void**>(data));
        if (ret < 0) {
			if (ret == -ENOENT) {
				// throw slankdev::exception("rte_ring_dequeue: no ent");
			} else {
				throw slankdev::exception("rte_ring_dequeue: unknown");
			}
		}

    }
	size_t count() const
	{
        return rte_ring_count(ring_);
	}
    size_t size() const
    {
		return ring_depth;
    }
    const char* name() const
    {
        return (const char*)ring_->name;
    }
    void stats() const
    {
        printf("     - %s: %zu/%zu \n", name(), count(), size());
    }
};


} /* namespace dpdk */
