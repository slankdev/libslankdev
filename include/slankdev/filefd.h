

#pragma onece

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


namespace slankdev {
    
class filefd {
    private:
        FILE* fp;

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
        }
        void close()
        {
            if (fp)
                ::fclose(fp);
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
                perror("fread");
            }
            return res;
        }
        template<typename... ARG>
        void printf(const char* const fmt, const ARG&... arg)
        {
            ::fprintf(fp, fmt, arg...);
        }
        void printf(const char* const fmt)
        {
            ::fprintf(fp, fmt);
        }
};


} /* namespace slankdev */


