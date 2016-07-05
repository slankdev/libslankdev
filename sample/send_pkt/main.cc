

#include <slankdev.h>
int main(int argc, char** argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s interface count\n", argv[0]);
        return -1;
    }
    slankdev::safe_intfd fd;
    fd.open_if(argv[1]);

    slankdev::hexdump("send packet", slankdev::raw, sizeof(slankdev::raw));
    for (int i=0; i<atoi(argv[2]); i++)
        fd.write(slankdev::raw, sizeof(slankdev::raw));
}
