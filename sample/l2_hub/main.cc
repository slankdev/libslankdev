
#include <stdio.h>
#include <string>
#include <slankdev.h>

const char* if0 = "enp3s0";
const char* if1 = "enp8s0";

int main()
{
    
    slankdev::pollfd base;
    base.add_if(if0);
    base.add_if(if1);

    while (1) {
        std::string name;
        uint8_t buf[10000];
        size_t recvlen = base.recv_any(name, buf, sizeof buf);
        printf("interface=%s length=%zd   ", name.c_str(), recvlen);

        if (name == if0) {
            printf("send to eth1 \n");
            base.send(if1, buf, recvlen);
        } else if (name == if1) {
            printf("send to eth0 \n");
            base.send(if0, buf, recvlen);
        } else {
            printf("unknown interface \n");
        }
    }

}


