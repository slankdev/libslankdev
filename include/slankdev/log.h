
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <slankdev/filefd.h>
#include <slankdev/singleton.h>
#include <slankdev/exception.h>



namespace slankdev {
    

#define DEBUG 0
#define INFO  1
#define WARN  2
#define ERROR 3
#define FATAL 4
typedef uint8_t loglevel_t;

static const char* lv2str(loglevel_t lv)
{
    static char str[8];
    switch (lv) {
        case DEBUG: strcpy(str, "DEBUG"); break;
        case INFO : strcpy(str, "INFO "); break;
        case WARN : strcpy(str, "WARN "); break;
        case ERROR: strcpy(str, "ERROR"); break;
        case FATAL: strcpy(str, "FATAL"); break;
    }
    return str;
}


class log : public slankdev::singleton<log> {
    friend slankdev::singleton<log>;
    private:
        static bool inited;
        std::string name;
        slankdev::filefd fd;

        std::vector<std::string> depth;
    public:
        void open(const char* path)
        {
            if (inited)
                throw slankdev::exception("Inited yet");
            inited = true;
            fd.open(path, "a");
            name = path;
        }
        template<typename ... ARG>
        void write(loglevel_t lv, const char* fmt, const ARG&... arg)
        {
            if (!inited)
                throw slankdev::exception("Not nited yes");

            fd.printf("%s: ", lv2str(lv));
            for (size_t i=0; i<depth.size(); i++)
                fd.printf("%s:", depth[i].c_str());
            fd.printf(" ");
            fd.printf(fmt, arg...);
            fd.printf("\n");
            fd.flush();
        }
        void export_to_file(const char* output_path)
        {
            if (!inited)
                throw slankdev::exception("Not nited yes");

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

        void push(const char* name)
        {
            std::string str = name;
            depth.push_back(str);
        }
        void pop()
        {
            depth.pop_back();
        }
};



} /* namespace slankdev */
