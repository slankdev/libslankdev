

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
 * @file   slankdev/exception.h
 * @brief  exception class
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once

#include <string>
#include <exception>
#include <sstream>
#include <ostream>


namespace slankdev {

class exception : public std::exception {
 private:
  std::string str;
 public:
  explicit exception(const char* s="") noexcept {
    str = s;
  }

  template<class T>
    exception& operator<<(const T& t) noexcept {
      std::ostringstream os;
      os << " " << t ;
      str += os.str();
      return *this;
    }
  const char* what() const noexcept {
    return str.c_str();
  }
};

} /* namespace slankdev */
