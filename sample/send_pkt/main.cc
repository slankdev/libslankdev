

#include <slankdev.h>
int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s interface \n", argv[0]);
        return -1;
    }
    slankdev::safe_intfd fd;
    fd.open_if(argv[1]);
    fd.write(slankdev::raw, sizeof(slankdev::raw));
}
