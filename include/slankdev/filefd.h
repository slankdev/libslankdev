

#pragma onece

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <string>

namespace slankdev {
    
class filefd {
    private:
        FILE* fp;
        std::string name;

    public:
        filefd() : fp(nullptr) {}
        ~filefd() 
        {
            close();
        }


        void open(const char* path, const char* mode)
        {
            if (fp)
                close();

            fp = ::fopen(path, mode);
            if (!fp) {
                perror("fopen");
                exit(-1);
            }
            name = path;
        }
        void close()
        {
            if (fp)
                ::fclose(fp);
            name = "";
        }
        void write(const void* ptr, size_t size, size_t nmemb)
        {
            size_t res = ::fwrite(ptr, size, nmemb, fp);
            if (res != nmemb) {
                perror("fwrite");
                exit(-1);
            }
        }
        size_t read(void* ptr, size_t size, size_t nmemb)
        {
            size_t res = ::fread(ptr, size, nmemb, fp);
            if (res != nmemb) {
                if (errno == 0)
                    return res;
                else
                    perror("fread");
            }
            return res;
        }
        void flush()
        {
            int res = ::fflush(fp);
            if (res == EOF) {
                perror("fflush");
                exit(-1);
            }
        }



        template<typename... ARG>
        void printf(const char* fmt, const ARG&... arg)
        {
            ::fprintf(fp, fmt, arg...);
        }
        // void printf(const char* fmt)
        // {
        //     ::fprintf(fp, fmt);
        // }
};


} /* namespace slankdev */


