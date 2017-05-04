

#include <slankdev/extra/dpdk.h>

inline void INIT_PORTCONF(struct rte_eth_conf* conf)
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

int main(int argc, char** argv)
{
  slankdev::dpdk_boot(argc, argv);
  struct rte_eth_conf port_conf;
  INIT_PORTCONF(&port_conf);
  struct rte_mempool* mp = slankdev::mp_alloc("RXMBUFMP");

  size_t nb_ports = rte_eth_dev_count();
  for (size_t i=0; i<nb_ports; i++) {
    slankdev::port_configure(i, 1, 1, &port_conf, mp);
  }

  while (1) {
    constexpr size_t BURSTSZ = 32;
    rte_mbuf* mbufs[BURSTSZ];

    size_t nb_ports = rte_eth_dev_count();
    for (size_t port=0; port<nb_ports; port++) {
      constexpr size_t nb_q = 1;
      for (size_t q=0; q<nb_q; q++) {
        size_t nb_recv = rte_eth_rx_burst(port, q, mbufs, BURSTSZ);
        slankdev::rte_pktmbuf_free_bulk(mbufs, nb_recv);
      }
    }
  }
}


