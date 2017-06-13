
#pragma once

namespace slankdev {


inline void rte_pktmbuf_free_bulk(struct rte_mbuf* m_list[], size_t npkts)
{
  while (npkts--)
    rte_pktmbuf_free(*m_list++);
}


inline struct rte_mbuf* pktmbuf_clone(struct rte_mbuf* md, struct rte_mempool* mp)
{
  struct rte_mbuf* ret = ::rte_pktmbuf_clone(md, mp);
  if (!ret) {
    throw slankdev::exception("rte_pktmbuf_clone");
  }
  return ret;
}

inline struct rte_ring* ring_create(const char* name, size_t count, int socket_id, unsigned flags)
{
  struct rte_ring* ring = ::rte_ring_create(name, count, socket_id, flags);
  if (!ring) {
    std::string e = "rte_ring_create: ";
    switch (rte_errno) {
      case E_RTE_NO_CONFIG: e += "E_RTE_NO_CONFIG"; break;
      case E_RTE_SECONDARY: e += "E_RTE_SECONDARY"; break;
      case EINVAL         : e += "EINVAL         "; break;
      case ENOSPC         : e += "ENOSPC         "; break;
      case EEXIST         : e += "EEXIST         "; break;
      case ENOMEM         : e += "ENOMEM         "; break;
      default: e += "unknown error"; break;
    }
    throw slankdev::exception(e.c_str());
  }
}

} /* namespace slankdev */


