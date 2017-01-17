


#pragma once
#include "types.h"
#include "log.h"


namespace dpdk {



class Ring {
    struct rte_ring* ring_;
	size_t ring_depth;
public:
    Ring() : ring_(nullptr), ring_depth(0) {}
    void init(const char* n, size_t count, uint16_t socket_id)
    {
		ring_depth = count;
        ring_ = rte_ring_create(n, count, socket_id, 0);
        if (!ring_)
            throw slankdev::exception("rte_ring_create");

        kernel_log(SYSTEM, "init ring %s ... done\n", name());
    }
    ~Ring()
    {
        if (!ring_)
            rte_ring_free(ring_);
    }

    /*
     * If ring is already full, this container
     * frees 10 element and re-enqueue.
     */
    void push(rte_mbuf* data)
    {
        int ret = rte_ring_enqueue(ring_, data);
		if (ret < 0) {
			if (ret == -EDQUOT) {
                /*
                 * Quota exceeded.
                 * The objects have been enqueued,
                 * but the high water mark is exceeded.
                 */
			} else if (ret == -ENOBUFS) {
                /*
                 * Not enough room in the ring to enqueue;
                 * no object is enqueued.
                 */
                rte_mbuf* m;
                for (size_t i=0; i<10; i++) {
                    pop(&m);
                    rte_pktmbuf_free(m);
                }
                push(data);
			} else {
				throw slankdev::exception("rte_ring_enqueue: unknown");
			}
		}
    }
    void push_bulk(rte_mbuf** obj_table, size_t n)
    {
        int ret = rte_ring_enqueue_bulk(ring_, reinterpret_cast<void**>(obj_table), n);
        if (ret < 0) {
            if (ret == -EDQUOT ) {
                /*
                 * Quota exceeded.
                 * The objects have been enqueued,
                 * but the high water mark is exceeded.
                 */
            }
            else if (ret == -ENOBUFS) {
                /*
                 * Not enough room in the ring to enqueue;
                 * no object is enqueued.
                 */
                rte_mbuf* free_buf[n];
                pop_bulk(free_buf, n);
                for (size_t i=0; i<n; i++) {
                    rte_pktmbuf_free(free_buf[i]);
                }
                push_bulk(obj_table, n);
            } else {
                throw slankdev::exception("rte_ring_enqueue_bulk: unknown");
            }
        }
    }


    /*
     * If ring is empty, *data = nullptr;
     */
    void pop(rte_mbuf** data)
    {
        int ret = rte_ring_dequeue(ring_, reinterpret_cast<void**>(data));
        if (ret < 0) {
			if (ret == -ENOENT) {
                /*
                 * Not enough entries in the ring to dequeue,
                 * no object is dequeued.
                 */
                *data = nullptr;
			} else {
				throw slankdev::exception("rte_ring_dequeue: unknown");
			}
		}
    }

    void pop_bulk(rte_mbuf** obj_table, size_t n)
    {
        int ret = rte_ring_dequeue_bulk(ring_, reinterpret_cast<void**>(obj_table), n);
        if (ret < 0) {
            if (ret == -ENOENT) {
                /*
                 * Not enough entries in the ring to dequeue,
                 * no object is dequeued.
                 */
                *obj_table = nullptr;
            } else {
				throw slankdev::exception("rte_ring_dequeue_bulk: unknown");
            }
        }
    }


    /*
     * Return number of elements in the ring.
     */
	size_t count() const
	{
        return rte_ring_count(ring_);
	}

    /*
     * Return depth of ring.
     */
    size_t size() const
    {
		return ring_depth;
    }

    const char* name() const
    {
        return (const char*)ring_->name;
    }
};


} /* namespace dpdk */
