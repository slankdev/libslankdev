
#pragma once

#include <slankdev/unused.h>
#include <slankdev/string.h>
#include <slankdev/exception.h>

#include <slankdev/extra/dpdk_header.h>
#include <slankdev/extra/dpdk_struct.h>
#include <slankdev/extra/dpdk_wrap.h>




namespace slankdev {



static inline void errhandle(const char* str)
{
  std::string e = str;
  e += ": ";
  e += rte_strerror(rte_errno);
  throw slankdev::exception(e.c_str());
}



inline void dpdk_boot(int argc, char** argv)
{
  int ret = rte_eal_init(argc, argv);
  if (ret < 0) {
    std::string e = "rte_eal_init: ";
    e += rte_strerror(rte_errno);
    throw slankdev::exception(e.c_str());
  }
}


inline void set_mbuf_raw(rte_mbuf* mbuf, const void* data, size_t len)
{
  mbuf->data_len = len;
  mbuf->pkt_len  = len;
  uint8_t* p = reinterpret_cast<uint8_t*>(rte_pktmbuf_mtod(mbuf, uint8_t*));
  memcpy(p, data, len);
}

inline rte_mempool* mp_alloc(const char* name)
{
  constexpr size_t NUM_MBUFS       = 8191;
  constexpr size_t MBUF_CACHE_SIZE = 0;
  size_t nb_ports = rte_eth_dev_count();

	struct rte_mempool* mp = rte_pktmbuf_pool_create(
      name,
      NUM_MBUFS * (nb_ports!=0?nb_ports:1),
      MBUF_CACHE_SIZE,
      0,
      RTE_MBUF_DEFAULT_BUF_SIZE,
      rte_socket_id());
  if (!mp) {
    std::string e = "rte_pktmbuf_pool_create: ";
    e += rte_strerror(rte_errno);
    throw slankdev::exception(e.c_str());
  }
  return mp;
}

inline rte_ring* ring_alloc(const char* name, size_t sizeofring)
{
  /*
   * 2nd argument, sizeofringm must be power of 2
   * Example
   *   good: 64, 128, 1024
   *   bad : 1000
   */
  int socket_id  = rte_socket_id();
  uint32_t flags = 0;
  rte_ring* r = slankdev::ring_create(name, sizeofring, socket_id, flags);
  return r;
}


inline void init_portconf(struct rte_eth_conf* conf)
{
  memset(conf, 0, sizeof(rte_eth_conf));

  /*********************************************\
   * Rx Mode
  \*********************************************/
  auto& rxmode = conf->rxmode;
  rxmode.mq_mode        = ETH_MQ_RX_NONE;
  rxmode.max_rx_pkt_len = ETHER_MAX_LEN;
  rxmode.split_hdr_size = 0;

  /* bit fields */
  rxmode.header_split   = 0;
  rxmode.hw_ip_checksum = 0;
  rxmode.hw_vlan_filter = 0;
  rxmode.hw_vlan_strip  = 0;
  rxmode.hw_vlan_extend = 0;
  rxmode.jumbo_frame    = 0;
  rxmode.hw_strip_crc   = 0;
  rxmode.enable_scatter = 0;
  rxmode.enable_lro     = 0;


  /*********************************************\
   * Tx Mode
  \*********************************************/
  auto& txmode = conf->txmode;
  txmode.mq_mode                 = ETH_MQ_TX_NONE;
  txmode.pvid                    = 0;  /* only I40E? */
  txmode.hw_vlan_reject_tagged   = 0;  /* only I40E? */
  txmode.hw_vlan_reject_untagged = 0;  /* only I40E? */
  txmode.hw_vlan_insert_pvid     = 0;  /* only I40E? */


  /*********************************************\
   * Rx Adv Conf
  \*********************************************/
  auto& rx_adv_conf = conf->rx_adv_conf;
  UNUSED(rx_adv_conf);


  /*********************************************\
   * Tx Adv Conf
  \*********************************************/
  auto& tx_adv_conf = conf->tx_adv_conf;
  UNUSED(tx_adv_conf);


  /*********************************************\
   * FDIR conf
  \*********************************************/
  auto& fdir_conf = conf->fdir_conf;
  UNUSED(fdir_conf);
}



inline void port_configure(uint8_t port, size_t nb_rxq, size_t nb_txq,
                  const struct rte_eth_conf* port_conf, struct rte_mempool* mp)
{
  int ret;
  constexpr size_t RX_RING_SIZE = 128;
  constexpr size_t TX_RING_SIZE = 512;

  ret = rte_eth_dev_configure(port, nb_rxq, nb_txq, port_conf);
  if (ret < 0) errhandle("rte_eth_dev_configure");

  for (size_t q=0; q<nb_rxq; q++) {
    ret = rte_eth_rx_queue_setup(port, q, RX_RING_SIZE,
        rte_eth_dev_socket_id(port), NULL, mp);
    if (ret < 0) {
      std::string e = "rte_eth_rx_queue_setup: ";
      e += rte_strerror(rte_errno);
      switch (ret) {
        case -EINVAL:
          e += "(-EINVAL)";
          break;
        case -ENOMEM:
          e += "(-ENOMEM)";
          break;
        default:
          e += "unknown error";
          throw slankdev::exception(e.c_str());
          break;
      }
    }
  }

  for (size_t q=0; q<nb_txq; q++) {
    ret = rte_eth_tx_queue_setup(port, q, TX_RING_SIZE,
        rte_eth_dev_socket_id(port), NULL);
    if (ret < 0) {
      std::string e = "rte_eth_tx_queue_setup: ";
      switch (ret) {
        case -ENOMEM:
          e += "(-ENOMEM)";
          break;
        default:
          e += "unknown error";
          throw slankdev::exception(e.c_str());
          break;
      }
    }
  }

  ret = rte_eth_dev_start(port);
  if (ret < 0) {
    throw slankdev::exception("rte_eth_dev_start");
  }

  /* this function always success because of HARDWARECALL */
  rte_eth_promiscuous_enable(port);
}


inline void safe_ring_enqueue(rte_ring* ring, void* data)
{
  int ret = rte_ring_enqueue(ring, data);
  if (ret < 0) throw slankdev::exception("rte_ring_enqueue: no space in ring");
}


inline void safe_ring_dequeue(rte_ring* ring, void** data)
{
  int ret = rte_ring_dequeue(ring, data);
  if (ret < 0) throw slankdev::exception("rte_ring_dequeue: no entry in ring");
}


inline void safe_ring_enqueue_bulk(rte_ring* ring, void* const* objs, size_t n)
{
  int ret = rte_ring_enqueue_bulk(ring, objs, n, nullptr);
  if (ret != n) throw slankdev::exception("rte_ring_enqueue_bulk: miss");
}


inline void safe_ring_dequeue_bulk(rte_ring* ring, void** objs, size_t n)
{
  int ret = rte_ring_dequeue_bulk(ring, objs, n, nullptr);
  if (ret < 0) throw slankdev::exception("rte_ring_dequeue_bulk: miss");
}


} /* namespace slankdev */




