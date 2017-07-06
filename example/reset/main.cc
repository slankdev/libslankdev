

#include <string.h>
#include <slankdev/util.h>
#include <slankdev/rest.h>


void callback_root(int fd, const void* buf, size_t len, void*)
{ slankdev::fdprintf(fd, "{ \"kanailab\" : \"Wonderful OBAKA Laboratory\" }\n"); }

void callback_slankdev(int fd, const void* buf, size_t len, void*)
{ slankdev::fdprintf(fd, "{ \"slankdev\" : \"Hiroki SHIROKURA\" }\n"); }

void callback_yukaribonk(int fd, const void* buf, size_t len, void*)
{ slankdev::fdprintf(fd, "{ \"yukaribonk\" : \"Yukari KUBO\" }\n"); }

int main(int argc, char** argv)
{
  slankdev::rest_server serv(INADDR_ANY, 80);
  serv.add_route("/", callback_root);
  serv.add_route("/slankdev", callback_slankdev);
  serv.add_route("/yukaribonk", callback_yukaribonk);
  serv.dispatch(nullptr);
}


