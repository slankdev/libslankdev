

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <slankdev.h>
using namespace slankdev;

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s port \n", argv[0]);
        return -1;
    }

    slankdev::socketfd fd;
    fd.socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    memset((char *) &server, 0, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = slankdev::htons(atoi(argv[1]));
    fd.bind((struct sockaddr*)&server, sizeof(server));
    fd.listen(5);

    struct sockaddr_in client;
    size_t client_len = sizeof(client);
    int new_sockfd = fd.accept((struct sockaddr*)&client, (socklen_t*)&client_len);
    if (new_sockfd < 0) {
        perror("accept");
        exit(1);
    }
    
    slankdev::socketfd c;
    c.set_fd(new_sockfd);
    uint8_t buf[10000];
    while (1) {
        memset(buf, 0, sizeof buf);
        size_t rsize = c.read(buf, sizeof(buf));
        if (rsize == 0) {
            printf("connection closed\n");
            break;
        }  else {
            hexdump("Recved", buf, rsize);
        }
    }
}
