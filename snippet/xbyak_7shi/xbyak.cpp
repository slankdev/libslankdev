#include <stdio.h>
#include "xbyak/xbyak.h"

struct TestFunc : public Xbyak::CodeGenerator {
    TestFunc() {
        mov(eax, ptr[esp+4]);
        add(eax, ptr[esp+8]);
        ret();
    }
};

int main() {
    TestFunc testf;
    auto test = reinterpret_cast<int(*)(int, int)>(
        const_cast<Xbyak::uint8 *>(testf.getCode()));
    printf("test(2, 3): %d\n", test(2, 3));
}
