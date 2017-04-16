
#pragma once
#include <capstone/capstone.h>


namespace slankdev {

class capstone {
  csh handle;
  cs_insn *insn;
  size_t count;
 public:
  capstone()
  {
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &handle) != CS_ERR_OK) {
      throw slankdev::exception("cs_open");
    }
  }
  void disasm(const void* code, size_t len, uint32_t a, uint32_t b)
  {
    count = cs_disasm(handle, (const uint8_t*)code, len, a, b, &insn);
    if (count <= 0)
      throw slankdev::exception("cs_disasm");
  }
  const cs_insn* get_insn() const { return insn; }
  size_t insn_len() const { return count; }
  ~capstone()
  {
    cs_free(insn, count);
    cs_close(&handle);
  }
};

} /* namespace slankdev */
