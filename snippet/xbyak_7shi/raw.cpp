#include <windows.h>
#include <stdio.h>
#include <string.h>

unsigned char testf[] = {
    0x8b, 0x44, 0x24, 0x04, // mov eax, [esp+4]
    0x03, 0x44, 0x24, 0x08, // add eax, [esp+8]
    0xc3 // ret
};

int main() {
    auto page = VirtualAlloc(nullptr, sizeof(testf), MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(page, testf, sizeof(testf));
    auto test = reinterpret_cast<int(*)(int, int)>(page);
    printf("test(2, 3): %d\n", test(2, 3));
    VirtualFree(page, 0, MEM_RELEASE);
}
