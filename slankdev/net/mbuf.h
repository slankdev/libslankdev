

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
 * @file   slankdev/net/mbuf.h
 * @brief  sample mbuf implementation
 * @author Hiroki SHIROKURA
 * @date   2017.4.2
 */


#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

namespace slankdev {


class mbuf_interface {
 public:
  virtual ~mbuf_interface() {}
  virtual const uint8_t* mtod() const = 0;
  virtual uint8_t* mtod()             = 0;
  virtual size_t dlen() const         = 0;
  virtual void prepend(size_t len)    = 0;
  virtual void append(size_t len)     = 0;
  virtual void adj(size_t len)        = 0;
  virtual void trim(size_t len)       = 0;
  virtual void dump(FILE* s) const    = 0;
};


class mbuf : public mbuf_interface {
  enum {
    def_data_size = 2000,
    def_head_size = 500 ,
    def_pack_size = 0   ,
  };
  uint8_t  data_[def_data_size];
  size_t   head_; // offset to head_ptr
  size_t   tail_; // offset to tail_ptr
  void clear();

 public:
  mbuf();
  virtual const uint8_t* mtod() const override;
  virtual uint8_t* mtod() override;
  virtual size_t dlen() const override;
  virtual void prepend(size_t len) override;
  virtual void append(size_t len) override;
  virtual void adj(size_t len) override;
  virtual void trim(size_t len) override;
  virtual void dump(FILE* s) const override;
};


} /* namespace slankdev */





/*
 * Function Implementation
 */

#include <string.h>
#include <slankdev/exception.h>
#include <slankdev/hexdump.h>


namespace slankdev {


inline mbuf::mbuf()
  : head_(def_head_size)
  , tail_(def_head_size + def_pack_size) { clear(); }
inline const uint8_t* mbuf::mtod() const { return data_ + head_; }
inline uint8_t* mbuf::mtod() { return data_ + head_; }
inline size_t mbuf::dlen() const { return tail_ - head_; }
inline void mbuf::prepend(size_t len)
{
  if (head_ < len)
    throw slankdev::exception("miss");
  head_ -= len;
}
inline void mbuf::append (size_t len)
{
  if (tail_ + len > sizeof(data_))
    throw slankdev::exception("miss");
  tail_ += len;
}
inline void mbuf::adj(size_t len)
{
  if (head_ + len > tail_)
    throw slankdev::exception("miss");
  head_ += len;
}
inline void mbuf::trim(size_t len)
{
  if (tail_ < head_ + len)
    throw slankdev::exception("miss");
  tail_ -= len;
}
inline void mbuf::dump(FILE* s) const
{
  fprintf(s, "Mbuf\n");
  fprintf(s, "- data address: %p \n", data_);
  fprintf(s, "- head address: %p (data+%zd)\n", data_ + head_, head_);
  fprintf(s, "- tail address: %p (data+%zd)\n", data_ + tail_, tail_);
  fprintf(s, "- total length: %zd \n", sizeof(data_));
  fprintf(s, "- pack length : %zd \n", dlen());
  slankdev::hexdump(s, mtod(), dlen());
}
inline void mbuf::clear() { memset(data_, 0, sizeof(data_)); }


} /* namespace slankdev */


