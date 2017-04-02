
/*
 * MIT License
 *
 * Copyright (c) 2017 Susanoo G
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file   slankdev/filelogger.h
 * @brief  filelogger
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once


#include <string>
#include <slankdev/filefd.h>



namespace slankdev {



/*
 * SAMPLE CODE
 *
 *     using slankdev::filelogger;
 *     std::string filelogger::path = "log.out";
 *     filelogger::get_instance().write("test\n");
 *     filelogger::get_instance().write("%s is pro\n", "palloc");
 *
 */


/*
 * Logger Singlethon class
 */
class filelogger {
    filefd fd;
    filelogger()
    {
        fd.fopen(path.c_str(), "w");
    }
    ~filelogger() {}

public:
    static std::string path;
    filelogger(const filelogger&) = delete;
    filelogger& operator=(const filelogger&) = delete;
    filelogger(filelogger&&) = delete;
    filelogger& operator=(filelogger&&) = delete;

    template <class... ARGS>
    void write(const char* fmt, ARGS... args) { fd.fprintf(fmt, args...); }
    static filelogger& get_instance() {
        static filelogger inst;
        return inst;
    }
};



} /* namespace slankdev */



