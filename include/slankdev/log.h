
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <slankdev/intfd.h>



namespace slankdev {
    

enum class loglevel {
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


/* 
 * Format
 * [rdtcp clock] host: msg.. 
 */
class logger {
    private:
        slankdev::safe_intfd fd;
    public:

        void open(const char* n)
        {
            fd.open(n, 0666);
        }
        template<typename ... ARG>
        void write(loglevel lv, const char* fmt, const ARG&... arg)
        {
            fd.printf("%s: ", lv2str(lv));
            fd.printf(fmt, arg...);
        }
};




} /* namespace slankdev */
