
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <slankdev/socketfd.h>

namespace slankdev {

typedef void (*http_callback_t)(int,const void*,size_t,void*);

class http {
 protected:
  slankdev::socketfd sock;
 public:
  http(uint32_t addr, uint16_t port)
  {
    int yes = 1;
    sock.socket(AF_INET, SOCK_STREAM, 0);
    sock.setsockopt(SOL_SOCKET, SO_REUSEADDR,
        (const char *)&yes, sizeof(yes));
    sock.bind(addr, port);
  }
  void dispatch(http_callback_t callback, void* arg)
  {
    sock.listen(1);
    while (true) {
      struct sockaddr_in client;
      socklen_t client_len = sizeof(client);
      int fd = sock.accept((sockaddr*)&client, &client_len);
      slankdev::socketfd client_sock;
      client_sock.set_fd(fd);

      uint8_t buf[1000];
      size_t recvlen = client_sock.read(buf, sizeof(buf));

      callback(fd, buf, recvlen, arg);
    }
  }
};

} /* namespace slankdev */
