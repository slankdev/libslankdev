
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <slankdev/filefd.h>
#include <slankdev/singleton.h>



namespace slankdev {
    

enum loglevel {
    DEBUG,
    INFO ,
    WARN ,
    ERROR,
    FATAL
};
static const char* lv2str(loglevel lv)
{
    static char str[8];
    switch (lv) {
        case loglevel::DEBUG: strcpy(str, "DEBUG"); break;
        case loglevel::INFO : strcpy(str, "INFO "); break;
        case loglevel::WARN : strcpy(str, "WARN "); break;
        case loglevel::ERROR: strcpy(str, "ERROR"); break;
        case loglevel::FATAL: strcpy(str, "FATAL"); break;
    }
    return str;
}


class log : public slankdev::singleton<log> {
    friend slankdev::singleton<log>;
    private:
        std::string name;
        slankdev::filefd fd;
    public:
        void open(const char* path)
        {
            fd.open(path, "a");
            name = path;
        }
        template<typename ... ARG>
        void write(loglevel lv, const char* fmt, const ARG&... arg)
        {
            fd.printf("%s: ", lv2str(lv));
            fd.printf(fmt, arg...);
            fd.printf("\n");
            fd.flush();
        }
        void export_to_file(const char* output_path)
        {
            fd.flush();

            slankdev::filefd read_fd;
            slankdev::filefd write_fd;
            read_fd.open(name.c_str(), "r");
            write_fd.open(output_path, "w");

            char c;
            while (read_fd.read(&c, sizeof(c), 1)!=0) {
                write_fd.write(&c, sizeof(c), 1);
            }
        }
};



} /* namespace slankdev */
