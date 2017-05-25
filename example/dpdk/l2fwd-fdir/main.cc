

#include <slankdev/errno.h>
#include <slankdev/extra/dpdk.h>
#define DEBUG

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
  // auto& fdir_conf = conf->fdir_conf;
  // fdir_conf.mode       = RTE_FDIR_MODE_PERFECT_TUNNEL;
  // fdir_conf.pballoc    = RTE_FDIR_PBALLOC_64K;
  // struct rte_eth_fdir_masks mask;
  // memset(&mask, 0, sizeof(mask));
  // mask.vlan_tci_mask = 0xffff;
  // // fdir_conf.mask       = mask;
  // fdir_conf.status     = RTE_FDIR_REPORT_STATUS_ALWAYS;
  // fdir_conf.drop_queue = 127;

}

void Flow_validate(uint8_t port, struct rte_flow_attr* attr,
    struct rte_flow_item* pattern, struct rte_flow_action* actions)
{
  struct rte_flow_error error;
  int ret = rte_flow_validate(port, attr, pattern, actions, &error);

  if (ret < 0) {
    std::string e = slankdev::format(
            "rte_flow_validate: %s (cause=%p,type=%s) %s",
            error.message, error.cause,
            slankdev::rte_flow_error_type2str(error.type),
            slankdev::errno2str(ret));
    // switch (ret) {
    //   case -ENOSYS:
    //     e += "(-ENOSYS)";
    //     break;
    //   case -ENOTSUP:
    //     e += "(-ENOTSUP)";
    //     break;
    //   case -EEXIST:
    //     e += "(-EEXIST)";
    //     break;
    //   case -ENOMEM:
    //     e += "(-ENOMEM)";
    //     break;
    //   case -EBUSY:
    //     e += "(-EBUSY)";
    //     break;
    //   default:
    //     e += "(unknown error)";
    // }
    throw slankdev::exception(e.c_str());
  }
}




void fdir_conf() {
  struct rte_flow_attr attr;
  attr.group    = 0;
  attr.priority = 0;
  attr.ingress  = 1;
  attr.egress   = 0;
  attr.reserved = 0;

  struct rte_flow_item   pattern[2];
  uint64_t mask = 0;
//   struct rte_flow_item_eth item;
//   item.type = 0x0008;
//   size_t   last = sizeof(item);
//   pattern[0].type = RTE_FLOW_ITEM_TYPE_ETH;
//   pattern[0].spec = &item;
//   pattern[0].last = &last;
//   pattern[0].mask = &mask;

//  {
//
//         .dst.addr_bytes = "\xff\xff\xff\xff\xff\xff",
//         .src.addr_bytes = "\xff\xff\xff\xff\xff\xff",
//         .type = 0x0000,
// }

  pattern[0].type = RTE_FLOW_ITEM_TYPE_END;
  pattern[0].spec = NULL;
  pattern[0].last = NULL;
  pattern[0].mask = NULL;

  struct rte_flow_action actions[2];
  // actions[0].type = RTE_FLOW_ACTION_TYPE_DROP;
  // actions[0].conf = NULL;
  actions[0].type = RTE_FLOW_ACTION_TYPE_END;
  actions[0].conf = NULL;


#ifdef DEBUG
  printf("DEBUG||  port   : %u \n", 0   );
  printf("DEBUG||  attr   : %p \n", &attr   );
  printf("DEBUG||  pattern: %p \n", pattern);
  printf("DEBUG||  actions: %p \n", actions);
#endif

  Flow_validate(0, &attr, pattern, actions);
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

  fdir_conf();

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


