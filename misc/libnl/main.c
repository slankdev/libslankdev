
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netlink/netlink.h>
#include <netlink/socket.h>
#include <netlink/version.h>
#include <netlink/cache.h>
#include <netlink/route/link.h>

int main(int argc, char** argv)
{
  struct nl_sock* sk = nl_socket_alloc();
  if (!sk) return 1;

  uint32_t groups = ~0U;
  nl_join_groups(sk, groups);

  int32_t proto = NETLINK_ROUTE;
  int ret = nl_connect(sk, proto);
  if (ret < 0) return 1;

  nl_socket_free(sk);
}

