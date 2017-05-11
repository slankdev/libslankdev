
#pragma once

#include <stdint.h>
#include <stdio.h>

namespace slankdev {

enum {
 ei_nident = 16,
};

struct elf64_Ehdr {
  unsigned char   e_ident[slankdev::ei_nident];   /* Id bytes */
  uint16_t   e_type     ;   /* file type */
  uint16_t   e_machine  ;   /* machine type */
  uint32_t   e_version  ;   /* version number */
  uint64_t   e_entry    ;   /* entry point */
  uint64_t   e_phoff    ;   /* Program hdr offset */
  uint64_t   e_shoff    ;   /* Section hdr offset */
  uint32_t   e_flags    ;   /* Processor flags */
  uint16_t   e_ehsize   ;   /* sizeof ehdr */
  uint16_t   e_phentsize;   /* Program header entry size */
  uint16_t   e_phnum    ;   /* Number of program headers */
  uint16_t   e_shentsize;   /* Section header entry size */
  uint16_t   e_shnum    ;   /* Number of section headers */
  uint16_t   e_shstrndx ;   /* String table index */

  void print() const
  {
    printf("  e_type     : %u \n", e_type     );
    printf("  e_machine  : %u \n", e_machine  );
    printf("  e_version  : %u \n", e_version  );
    printf("  e_entry    : %lu \n", e_entry    );
    printf("  e_phoff    : %lu \n", e_phoff    );
    printf("  e_shoff    : %lu \n", e_shoff    );
    printf("  e_flags    : %u \n", e_flags    );
    printf("  e_ehsize   : %u \n", e_ehsize   );
    printf("  e_phentsize: %u \n", e_phentsize);
    printf("  e_phnum    : %u \n", e_phnum    );
    printf("  e_shentsize: %u \n", e_shentsize);
    printf("  e_shnum    : %u \n", e_shnum    );
    printf("  e_shstrndx : %u \n", e_shstrndx );
  }
};


} /* namespace slankdev */


