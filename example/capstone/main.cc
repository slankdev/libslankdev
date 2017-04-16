
#include <stdio.h>
#include <stdint.h>

#include <slankdev/exception.h>
#include <slankdev/extra/capstone.h>

#define CODE "\x55\x48\x8b\x05\xb8\x13\x00\x00"


int main(void)
{
  slankdev::capstone c;
  c.disasm(CODE, sizeof(CODE)-1, 0x1000, 0);
  for (size_t j = 0; j < c.insn_len(); j++) {
    printf("0x%lu:\t%s\t\t%s\n",
        c.get_insn()[j].address,
        c.get_insn()[j].mnemonic,
        c.get_insn()[j].op_str);
  }
}





