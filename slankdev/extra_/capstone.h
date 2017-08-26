
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


inline void disas_x86(const void* ptr, size_t len)
{
  slankdev::capstone c;
  c.disasm(ptr, len, 0x000, 0);
  for (size_t i = 0; i < c.insn_len(); i++) {
    const cs_insn* insn = c.get_insn();
    printf("0x%04lx:    %-5s %-20s   ",
        insn[i].address,
        insn[i].mnemonic,
        insn[i].op_str
    );
    for (size_t j=0; j<insn[i].size; j++) {
      printf("%02x ", insn[i].bytes[j]);
    }
    printf("\n");

    if (strcmp(insn[i].mnemonic, "nop") == 0) break;
  }
}





} /* namespace slankdev */
