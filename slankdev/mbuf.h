


#pragma once
#include <stdint.h>
#include <stddef.h>
#include <slankdev/exception.h>
#include <slankdev/hexdump.h>


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



/*
 * Function Implementation
 */
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


