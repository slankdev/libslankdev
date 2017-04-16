

#include <slankdev/socketfd.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        return -1;
    }

    slankdev::socketfd sock;
    sock.socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(atoi(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    sock.bind((sockaddr*)&addr, sizeof(addr));

    sock.listen(5);

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int fd = sock.accept((sockaddr*)&client, &client_len);

    slankdev::socketfd client_sock;
    client_sock.set_fd(fd);

    while (1) {
        uint8_t buf[1000];
        size_t recvlen = client_sock.read(buf, sizeof(buf));
        client_sock.write(buf, recvlen);
    }
    return 0;
}


int client()
{
  slankdev::socketfd sock;
  uint32_t addr = 0x7f000001;
  uint16_t port = 22;
  sock.open_connect(addr, port);

  char data[] = "port configure";
  sock.write(data, sizeof(data));
}
