
#pragma once

#include <slankdev/exception.h>

#include "types.h"


inline const char* txmqmode2str(enum rte_eth_tx_mq_mode mode)
{
    switch (mode) {
        case ETH_MQ_TX_NONE     : return "ETH_MQ_TX_NONE     ";
        case ETH_MQ_TX_DCB      : return "ETH_MQ_TX_DCB      ";
        case ETH_MQ_TX_VMDQ_DCB : return "ETH_MQ_TX_VMDQ_DCB ";
        case ETH_MQ_TX_VMDQ_ONLY: return "ETH_MQ_TX_VMDQ_ONLY";
        default: throw slankdev::exception("txmqmode2str: unknow mq_mode");
    }
}



inline const char* rxmqmode2str(enum rte_eth_rx_mq_mode mode)
{
    switch (mode) {
        case ETH_MQ_RX_NONE        : return "NONE        ";
        case ETH_MQ_RX_RSS         : return "RSS         ";
        case ETH_MQ_RX_DCB         : return "DCB         ";
        case ETH_MQ_RX_DCB_RSS     : return "DCB_RSS     ";
        case ETH_MQ_RX_VMDQ_ONLY   : return "VMDQ_ONLY   ";
        case ETH_MQ_RX_VMDQ_RSS    : return "VMDQ_RSS    ";
        case ETH_MQ_RX_VMDQ_DCB    : return "VMDQ_DCB    ";
        case ETH_MQ_RX_VMDQ_DCB_RSS: return "VMDQ_DCB_RSS";
        default: throw slankdev::exception("rxmqmode2str: unknow mq_mode");
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
		default: throw slankdev::exception("linkspeed2str: unknow dpdk link speed");
	}
}
