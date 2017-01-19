


#pragma once
#include "mempool.h"
#include "log.h"


namespace dpdk {



class Ring {
    struct rte_ring* ring_;
	size_t ring_depth;
public:
    Ring(const char* n, size_t count, uint16_t socket_id)
        : ring_depth(count)
    {
        ring_ = rte_ring_create(n, count, socket_id, 0);
        if (!ring_) {
            char errstr[256];
            snprintf(errstr, sizeof(errstr),
                    "rte_ring_create(%s, %zd, %u)",
                    n, count, socket_id);
            throw slankdev::exception(errstr);
        }

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
        if (!data) return;

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


	size_t count() const { return rte_ring_count(ring_);    }
    size_t size()  const { return ring_depth;               }
    bool   empty() const { return rte_ring_empty(ring_)==1; }
    bool   full()  const { return rte_ring_full(ring_)==1;  }
    const char* name() const { return (const char*)ring_->name; }
};


} /* namespace dpdk */