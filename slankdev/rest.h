
#include <slankdev/http.h>
#include <slankdev/util.h>
#include <string.h>
#include <string>
#include <map>

namespace slankdev {

std::string getline(const void* buf, size_t len);

class rest_server : public slankdev::http {
  std::map<std::string,slankdev::http_callback_t> cbs;
 public:
  rest_server(uint32_t addr, uint16_t port) : slankdev::http(addr, port) {}
  void add_route(const char* path, slankdev::http_callback_t callback)
  { cbs[std::string(path)] = callback; }
  void dispatch(void* arg)
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

      /* Analyze */
      std::string line = getline(buf, recvlen);
      printf(" \"%s\" \n", line.c_str());
      char method[256];
      char uri[256];
      sscanf(line.c_str(), "%s %s", method, uri);

      if (cbs.count(uri) == 0) {
        slankdev::fdprintf(fd, "HTTP/1.1 404 Not Found\n");
      } else {
        cbs[uri](fd, buf, recvlen, arg);
      }
    } // while
  }
};

std::string getline(const void* buf, size_t len)
{
  std::string s = "";
  const char* p = reinterpret_cast<const char*>(buf);
  for (size_t i=0; i<len; i++) {
    if (p[i] == '\n' || p[i] == EOF || p[i] == '\r') break;
    s += p[i];
  }
  return s;
}

} /* namespace slankdev */
