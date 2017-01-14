
#pragma once

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


namespace dpdk {
namespace util {

#define DEPTHSTR "    "
#define for_i(begin, end) for(size_t i=begin; i<end; i++)



// TODO Prease Review
template<class... Args>
inline void printf_depth(size_t depth, const char* const fmt, Args... args)
{
    for_i (0, depth) printf(DEPTHSTR);
    printf(fmt, args...);
}




inline const char* rte_eth_nb_tcs2str(rte_eth_nb_tcs e)
{
    switch (e) {
        case ETH_4_TCS: return "4_TCS";
        case ETH_8_TCS: return "8_TCS";
        default: return "UNKNOWN-ERROR";
    }
}


inline const char* nb_queue_pools2str(rte_eth_nb_pools e)
{
    switch (e) {
        case ETH_8_POOLS : return "8_POOLS ";
        case ETH_16_POOLS: return "16_POOLS";
        case ETH_32_POOLS: return "32_POOLS";
        case ETH_64_POOLS: return "64_POOLS";
        default: return "UNKNOWN-ERROR";
    }
}


inline const char* rte_eth_payload_type2str(enum rte_eth_payload_type e)
{
    switch (e) {
        case RTE_FDIR_MODE_NONE 	       : return "NONE 	         ";
        case RTE_FDIR_MODE_SIGNATURE 	   : return "SIGNATURE 	     ";
        case RTE_FDIR_MODE_PERFECT 	       : return "PERFECT 	     ";
        case RTE_FDIR_MODE_PERFECT_MAC_VLAN: return "PERFECT_MAC_VLAN";
        case RTE_FDIR_MODE_PERFECT_TUNNEL  : return "PERFECT_TUNNEL  ";
        default: return "UNKNOWN-ERROR";
    }
}




inline const char* rte_eth_tx_mq_mode2str(enum rte_eth_tx_mq_mode mode)
{
    switch (mode) {
        case ETH_MQ_TX_NONE     : return "ETH_MQ_TX_NONE     ";
        case ETH_MQ_TX_DCB      : return "ETH_MQ_TX_DCB      ";
        case ETH_MQ_TX_VMDQ_DCB : return "ETH_MQ_TX_VMDQ_DCB ";
        case ETH_MQ_TX_VMDQ_ONLY: return "ETH_MQ_TX_VMDQ_ONLY";
        default: return "UNKNOWN-ERROR";
    }
}



inline const char* rte_eth_rx_mq_mode2str(enum rte_eth_rx_mq_mode mode)
{
    switch (mode) {
        case ETH_MQ_RX_NONE        : return "NONE ";
        case ETH_MQ_RX_RSS         : return "RSS  ";
        case ETH_MQ_RX_DCB         : return "DCB  ";
        case ETH_MQ_RX_DCB_RSS     : return "DCB_RSS     ";
        case ETH_MQ_RX_VMDQ_ONLY   : return "VMDQ_ONLY   ";
        case ETH_MQ_RX_VMDQ_RSS    : return "VMDQ_RSS    ";
        case ETH_MQ_RX_VMDQ_DCB    : return "VMDQ_DCB    ";
        case ETH_MQ_RX_VMDQ_DCB_RSS: return "VMDQ_DCB_RSS";
        default: return "UNKNOWN-ERROR";
    }
}


inline const char* linkspeed2str(uint32_t speed)
{
	switch (speed) {
        case ETH_LINK_SPEED_AUTONEG:  return "AUTONEG";
        case ETH_LINK_SPEED_FIXED  :  return "FIXED  ";
        case ETH_LINK_SPEED_10M_HD :  return "10M_HD ";
        case ETH_LINK_SPEED_10M    :  return "10M    ";
        case ETH_LINK_SPEED_100M_HD:  return "100M_HD";
        case ETH_LINK_SPEED_100M   :  return "100M   ";
        case ETH_LINK_SPEED_1G     :  return "1G     ";
        case ETH_LINK_SPEED_2_5G   :  return "2_5G   ";
        case ETH_LINK_SPEED_5G     :  return "5G     ";
        case ETH_LINK_SPEED_10G    :  return "10G    ";
        case ETH_LINK_SPEED_20G    :  return "20G    ";
        case ETH_LINK_SPEED_25G    :  return "25G    ";
        case ETH_LINK_SPEED_40G    :  return "40G    ";
        case ETH_LINK_SPEED_50G    :  return "50G    ";
        case ETH_LINK_SPEED_56G    :  return "56G    ";
        case ETH_LINK_SPEED_100G   :  return "100G   ";
        default: return "UNKNOWN-ERROR";
	}
}


inline const char* rte_eth_nb_pools2str(enum rte_eth_nb_pools e)
{
    switch (e) {
		case ETH_8_POOLS : return "ETH_8_POOLS ";
		case ETH_16_POOLS: return "ETH_16_POOLS";
		case ETH_32_POOLS: return "ETH_32_POOLS";
		case ETH_64_POOLS: return "ETH_64_POOLS";
        default: return "UNKNOWN-ERROR";
    }
}


inline const char* rte_fdir_mode2str(enum rte_fdir_mode e)
{
    switch (e) {
        case RTE_FDIR_MODE_NONE            : return "NONE            ";
        case RTE_FDIR_MODE_SIGNATURE       : return "SIGNATURE       ";
        case RTE_FDIR_MODE_PERFECT         : return "PERFECT         ";
        case RTE_FDIR_MODE_PERFECT_MAC_VLAN: return "PERFECT_MAC_VLAN";
        case RTE_FDIR_MODE_PERFECT_TUNNEL  : return "PERFECT_TUNNEL  ";
        default: return "UNKNOWN-ERROR";
    }
}


inline const char* rte_fdir_pballoc_type2str(enum rte_fdir_pballoc_type e)
{
    switch (e) {
        case RTE_FDIR_PBALLOC_64K : return "64K ";
        case RTE_FDIR_PBALLOC_128K: return "128K";
        case RTE_FDIR_PBALLOC_256K: return "256K";
        default: return "UNKNOWN-ERROR";
    }
}


inline const char* rte_fdir_status_mode2str(enum rte_fdir_status_mode e)
{
    switch (e) {
        case RTE_FDIR_NO_REPORT_STATUS    : return "NO_REPORT_STATUS    ";
        case RTE_FDIR_REPORT_STATUS       : return "REPORT_STATUS       ";
        case RTE_FDIR_REPORT_STATUS_ALWAYS: return "REPORT_STATUS_ALWAYS";
        default: return "UNKNOWN-ERROR";
    }
}


inline void print(const struct rte_eth_stats* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_stats %s {\n", name);
    printf_depth(depth, "ipackets  : %lu \n", raw->ipackets );
    printf_depth(depth, "opackets  : %lu \n", raw->opackets );
    printf_depth(depth, "ibytes    : %lu \n", raw->ibytes   );
    printf_depth(depth, "obytes    : %lu \n", raw->obytes   );
    printf_depth(depth, "imissed   : %lu \n", raw->imissed  );
    printf_depth(depth, "ierrors   : %lu \n", raw->ierrors  );
    printf_depth(depth, "oerrors   : %lu \n", raw->oerrors  );
    printf_depth(depth, "rx_nombuf : %lu \n", raw->rx_nombuf);

    for_i (0, RTE_ETHDEV_QUEUE_STAT_CNTRS) {
        printf_depth(depth, "q_ipackets[%2zd]   : %lu \n", i, raw->q_ipackets[i]);
        printf_depth(depth, "q_ibytes[%2zd]     : %lu \n", i, raw->q_ibytes  [i]);
    }
    for_i (0, RTE_ETHDEV_QUEUE_STAT_CNTRS) {
        printf_depth(depth, "q_opackets[%2zd]   : %lu \n", i, raw->q_opackets[i]);
        printf_depth(depth, "q_obytes  [%2zd]   : %lu \n", i, raw->q_obytes  [i]);
        printf_depth(depth, "q_errors  [%2zd]   : %lu \n", i, raw->q_errors  [i]);
    }
    printf_depth(depth, "}\n");
}


inline void print(const struct rte_eth_rxmode* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_rxmode %s {\n", name);

    depth++;
    printf_depth(depth, "mq_mode        : %s\n", rte_eth_rx_mq_mode2str(raw->mq_mode));
    printf_depth(depth, "max_rx_pkt_len : %u\n", raw->max_rx_pkt_len);
    printf_depth(depth, "split_hdr_size : %u\n", raw->split_hdr_size);
    printf_depth(depth, "header_split   : %s\n", (raw->header_split  ==1)?"Yes":"No");
    printf_depth(depth, "hw_ip_checksum : %s\n", (raw->hw_ip_checksum==1)?"Yes":"No");
    printf_depth(depth, "hw_vlan_filter : %s\n", (raw->hw_vlan_filter==1)?"Yes":"No");
    printf_depth(depth, "hw_vlan_strip  : %s\n", (raw->hw_vlan_strip ==1)?"Yes":"No");
    printf_depth(depth, "hw_vlan_extend : %s\n", (raw->hw_vlan_extend==1)?"Yes":"No");
    printf_depth(depth, "jumbo_frame    : %s\n", (raw->jumbo_frame   ==1)?"Yes":"No");
    printf_depth(depth, "hw_strip_crc   : %s\n", (raw->hw_strip_crc  ==1)?"Yes":"No");
    printf_depth(depth, "enable_scatter : %s\n", (raw->enable_scatter==1)?"Yes":"No");
    printf_depth(depth, "enable_lro     : %s\n", (raw->enable_lro    ==1)?"Yes":"No");
    depth--;

    printf_depth(depth, "}\n");
}

inline void print(const struct rte_eth_txmode* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_txmode %s {\n", name);

    depth++;
    printf_depth(depth, "mq_mode                 : %s\n", rte_eth_tx_mq_mode2str(raw->mq_mode)   );
    printf_depth(depth, "hw_vlan_reject_tagged   : %s\n", (raw->hw_vlan_reject_tagged  ==1)?"Yes":"No");
    printf_depth(depth, "hw_vlan_reject_untagged : %s\n", (raw->hw_vlan_reject_untagged==1)?"Yes":"No");
    printf_depth(depth, "hw_vlan_insert_pvid     : %s\n", (raw->hw_vlan_insert_pvid    ==1)?"Yes":"No");
    depth--;

    printf_depth(depth, "}\n");
}


inline void print(const struct rte_eth_rss_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_rss_conf %s {\n", name);

    depth++;
    printf_depth(depth, "rss_key     : %p \n", raw->rss_key);
    if (raw->rss_key) {
        printf_depth(depth, "val         : ");
        for_i (0, 40) printf_depth(depth, "%02x", raw->rss_key[i]);
        printf_depth(depth, "\n");
    }
    printf_depth(depth, "rss_key_len : %u\n", raw->rss_key_len   );
    printf_depth(depth, "rss_hf      : %lu\n", raw->rss_hf);
    depth--;

    printf_depth(depth, "}\n");
}




inline void print(const struct rte_eth_vmdq_dcb_conf* raw, const char* name="", size_t depth=0)
{

    printf_depth(depth, "rte_eth_vmdq_dcb_conf %s {\n", name);

	depth++;
	printf_depth(depth, "nb_queue_pools      : %s \n", rte_eth_nb_pools2str(raw->nb_queue_pools));
	printf_depth(depth, "enable_default_pool : %u \n", raw->enable_default_pool);
	printf_depth(depth, "default_pool        : %u \n", raw->default_pool       );
	printf_depth(depth, "nb_pool_maps        : %u \n", raw->nb_pool_maps       );

    for (size_t i=0; i<1; i++) {
        printf_depth(depth, "pool_map[0] {\n", ETH_VMDQ_MAX_VLAN_FILTERS);
        printf_depth(depth, DEPTHSTR "vlan_id : %u \n", raw->pool_map[i].vlan_id);
        printf_depth(depth, DEPTHSTR "pools   : %u \n", raw->pool_map[i].pools  );
        printf_depth(depth, "}\n");
    }
	printf_depth(depth, "pool_map[1-%zd] {} --> EXPANDABLE: see source \n", ETH_VMDQ_MAX_VLAN_FILTERS);

	printf_depth(depth, "dcb_tc : {%u, %u, %u, %u, %u, %u, %u, %u} \n",
            raw->dcb_tc[0], raw->dcb_tc[1], raw->dcb_tc[2], raw->dcb_tc[3],
            raw->dcb_tc[4], raw->dcb_tc[5], raw->dcb_tc[6], raw->dcb_tc[7]);
	depth--;

    printf_depth(depth, "}\n");
}





inline void print(const struct rte_eth_dcb_rx_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_dcb_rx_conf %s {\n", name);

	depth++;
	printf_depth(depth, "nb_tcs : %s \n", rte_eth_nb_tcs2str(raw->nb_tcs));
	printf_depth(depth, "dcb_tc : {%u, %u, %u, %u, %u, %u, %u, %u} \n",
            raw->dcb_tc[0], raw->dcb_tc[1], raw->dcb_tc[2], raw->dcb_tc[3],
            raw->dcb_tc[4], raw->dcb_tc[5], raw->dcb_tc[6], raw->dcb_tc[7]);
	depth--;

    printf_depth(depth, "}\n");
}



inline void print(const struct rte_eth_vmdq_rx_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_vmdq_rx_conf %s {\n", name);

	depth++;
	printf_depth(depth, "nb_queue_pools : %s \n", rte_eth_nb_pools2str(raw->nb_queue_pools));
	printf_depth(depth, "enable_default_pool : %u \n", raw->enable_default_pool);
	printf_depth(depth, "default_pool        : %u \n", raw->default_pool       );
	printf_depth(depth, "enable_loop_back    : %u \n", raw->enable_loop_back   );
	printf_depth(depth, "nb_pool_maps        : %u \n", raw->nb_pool_maps       );
	printf_depth(depth, "rx_mode             : %u \n", raw->rx_mode            );

	printf_depth(depth, "struct {\n");
	printf_depth(depth, DEPTHSTR "vlan_id : %u \n", raw->pool_map[0].vlan_id);
	printf_depth(depth, DEPTHSTR "pools   : %u \n", raw->pool_map[0].pools  );
	printf_depth(depth, "} pool_map[0]\n");
	printf_depth(depth, "struct {} pool_map[1-%zd]\n", ETH_VMDQ_MAX_VLAN_FILTERS);
	depth--;

    printf_depth(depth, "}\n");
}


inline void print(const struct rte_eth_vmdq_dcb_tx_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_vmdq_dcb_tx_conf %s {\n", name);

	depth++;
	printf_depth(depth, "nb_queue_pools : %s \n", rte_eth_nb_pools2str(raw->nb_queue_pools));
	printf_depth(depth, "dcb_tc : {%u, %u, %u, %u, %u, %u, %u, %u} \n",
            raw->dcb_tc[0], raw->dcb_tc[1], raw->dcb_tc[2], raw->dcb_tc[3],
            raw->dcb_tc[4], raw->dcb_tc[5], raw->dcb_tc[6], raw->dcb_tc[7]);
	depth--;

    printf_depth(depth, "}\n");
}


inline void print(const struct rte_eth_dcb_tx_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_dcb_tx_conf %s {\n", name);

	depth++;
	printf_depth(depth, "nb_tcs : %s \n", rte_eth_nb_tcs2str(raw->nb_tcs));
	printf_depth(depth, "dcb_tc[] %u \n", raw->dcb_tc[0]);
	printf_depth(depth, "dcb_tc[1-%zd] {} \n", ETH_DCB_NUM_USER_PRIORITIES);
	depth--;

    printf_depth(depth, "}\n");
}

inline void print(const struct rte_eth_vmdq_tx_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_vmdq_tx_conf %s {\n", name);

	depth++;
	printf_depth(depth, "nb_queue_pools : %s \n", raw->nb_queue_pools);
	depth--;

    printf_depth(depth, "}\n");
}



inline void print(const struct rte_eth_flex_payload_cfg* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_flex_payload_cfg %s {\n", name);

	depth++;
	printf_depth(depth, "type : %s \n", rte_eth_payload_type2str(raw->type));
	printf_depth(depth,
            "src_offset[%zd] : {%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u} \n",
            RTE_ETH_FDIR_MAX_FLEXLEN,
            raw->src_offset[0] , raw->src_offset[1] , raw->src_offset[2] , raw->src_offset[3] ,
            raw->src_offset[4] , raw->src_offset[5] , raw->src_offset[6] , raw->src_offset[7] ,
            raw->src_offset[8] , raw->src_offset[9] , raw->src_offset[10], raw->src_offset[11],
            raw->src_offset[12], raw->src_offset[13], raw->src_offset[14], raw->src_offset[15]);
	depth--;

    printf_depth(depth, "}\n");
}
inline void print(const struct rte_eth_fdir_flex_mask* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_fdir_flex_mask %s {\n", name);

	depth++;
	printf_depth(depth,
            "mask[%zd] : {%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u} \n",
            RTE_ETH_FDIR_MAX_FLEXLEN,
            raw->mask[0] , raw->mask[1] , raw->mask[2] , raw->mask[3] ,
            raw->mask[4] , raw->mask[5] , raw->mask[6] , raw->mask[7] ,
            raw->mask[8] , raw->mask[9] , raw->mask[10], raw->mask[11],
            raw->mask[12], raw->mask[13], raw->mask[14], raw->mask[15]);
	depth--;

    printf_depth(depth, "}\n");
}
inline void print(const struct rte_eth_fdir_flex_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_fdir_flex_conf %s {\n", name);

	depth++;
	printf_depth(depth, "nb_payloads  : %u \n", raw->nb_payloads );
	printf_depth(depth, "nb_flexmasks : %u \n", raw->nb_flexmasks);

    print(&raw->flex_set[0], "flex_set[0]", depth);
    printf_depth(depth, "rte_eth_flex_payload_cfg flex_set[1-%zd] {} \n", RTE_ETH_PAYLOAD_MAX);
    print(&raw->flex_mask[0], "flex_mask[0]", depth);
    printf_depth(depth, "rte_eth_fdir_flex_mask flex_mask[1-%zd] {} \n", RTE_ETH_FLOW_MAX);

	depth--;

    printf_depth(depth, "}\n");
}
inline void print(const struct rte_fdir_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_fdir_conf %s {\n", name);

	depth++;
	printf_depth(depth, "mode       : %s \n", rte_fdir_mode2str(raw->mode   ));
	printf_depth(depth, "pballoc    : %s \n", rte_fdir_pballoc_type2str(raw->pballoc));
	printf_depth(depth, "status     : %s \n", rte_fdir_status_mode2str(raw->status ));
	printf_depth(depth, "drop_queue : %u \n", raw->drop_queue);
	print(&raw->flex_conf, "flex_conf", depth);
	depth--;

    printf_depth(depth, "}\n");
}



inline void print(const struct rte_intr_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_intr_conf %s {\n", name);

	depth++;
	printf_depth(depth, "lsc : %u \n", raw->lsc);
	printf_depth(depth, "rxq : %u \n", raw->rxq);
	depth--;

    printf_depth(depth, "}\n");
}



inline void print(const struct rte_eth_conf* raw, const char* name="", size_t depth=0)
{
    printf_depth(depth, "rte_eth_conf %s {\n", name);

    depth++;
    printf_depth(depth, "link_speeds : %u(%s) \n", raw->link_speeds, linkspeed2str(raw->link_speeds));
    print(&raw->rxmode, "rxmode", depth);
    print(&raw->txmode, "txmode", depth);
    printf_depth(depth, "lpbk_mode   : %u \n", raw->lpbk_mode   );
    printf_depth(depth, "struct {\n");
    print(&raw->rx_adv_conf.rss_conf     , "rss_conf"     , depth+1);
	print(&raw->rx_adv_conf.vmdq_dcb_conf, "vmdq_dcb_conf", depth+1);
    print(&raw->rx_adv_conf.dcb_rx_conf  , "dcb_rx_conf"  , depth+1);
    print(&raw->rx_adv_conf.vmdq_rx_conf , "vmdq_rx_conf" , depth+1);
    printf_depth(depth, "} rx_adv_conf\n");
    printf_depth(depth, "struct {\n");
	printf_depth(depth, DEPTHSTR "[UNION]\n");
    print(&raw->tx_adv_conf.vmdq_dcb_tx_conf, "vmdq_dcb_tx_conf", depth+1);
    print(&raw->tx_adv_conf.dcb_tx_conf     , "dcb_tx_conf"     , depth+1);
    print(&raw->tx_adv_conf.vmdq_tx_conf    , "vmdq_tx_conf"    , depth+1);
    printf_depth(depth, "} tx_adv_conf\n");
    printf_depth(depth, "dcb_capability_en : %u \n", raw->dcb_capability_en  );
    print(&raw->fdir_conf, "fdir_conf", depth);
    print(&raw->intr_conf, "intr_conf", depth);
    depth--;

    printf_depth(depth, "}\n");
}


} /* namespace util */
} /* namespace dpdk */
