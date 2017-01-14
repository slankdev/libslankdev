
#pragma once

#include <string>
#include <exception>
#include <sstream>
#include <ostream>

#include <rte_ring.h>
#include <rte_config.h>
#include <rte_version.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_ether.h>
#include <rte_cycles.h>
#include <rte_lcore.h>
#include <rte_mbuf.h>
#include <rte_hexdump.h>
#include <rte_ip.h>
#include <rte_ip_frag.h>

#include <slankdev/exception.h>


namespace dpdk {



class ether_addr : public ::ether_addr {
public:
    void print(FILE* fd) const
    {
        fprintf(fd, "%s", toString().c_str());
    }
    std::string toString() const
    {
        char buf[32];
        snprintf(buf, sizeof(buf),
                "%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8
                   ":%02" PRIx8 ":%02" PRIx8 ":%02" PRIx8,
                addr_bytes[0], addr_bytes[1],
                addr_bytes[2], addr_bytes[3],
                addr_bytes[4], addr_bytes[5]);

        return buf;
    }
};



class pool {
    rte_mempool* raw_;
public:
    pool() : raw_(nullptr) {}
    pool(const char* name,
            size_t nb_seg,
            size_t cache_siz,
            size_t mbuf_siz,
            uint16_t sock_id) : pool()
    {
        create(name,
            nb_seg, cache_siz,
            mbuf_siz, sock_id);
    }
    ~pool()
    {
        free();
    }
    void create(const char* name,
            size_t nb_seg,
            size_t cache_siz,
            size_t mbuf_siz,
            uint16_t sock_id)
    {
        if (raw_) {
            throw slankdev::exception("already created");
        }

        raw_ = rte_pktmbuf_pool_create(name,
                        nb_seg, cache_siz, 0,
                        mbuf_siz, sock_id);
        if (!raw_) {
            fprintf(stderr, "name     : %s  \n", name     );
            fprintf(stderr, "nb_seg   : %zd \n", nb_seg   );
            fprintf(stderr, "cache_siz: %zd \n", cache_siz);
            fprintf(stderr, "mbuf_siz : %zd \n", mbuf_siz );
            fprintf(stderr, "sock_id  : %u  \n", sock_id  );

            throw slankdev::exception("can not create pool");
        }
    }
    void free()
    {
        if (raw_) {
            rte_mempool_free(raw_);
            raw_ = nullptr;
        }
    }


    rte_mempool* get_raw() { return raw_; }
};




} /* namespace dpdk */
