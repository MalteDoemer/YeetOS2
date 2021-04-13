#include "Types.hpp"
#include "Platform.hpp"

#include "Kernel/Kernel.hpp"

#include "Arch/x86/Asm.hpp"

C_LINKAGE void* memcpy(void* dest, const void* src, size_t count)
{
    Uint8* d = static_cast<Uint8*>(dest);
    const Uint8* s = static_cast<const Uint8*>(src);

    movsd(d, s, count / 4);

    d += (count / 4) * 4;
    s += (count / 4) * 4;

    count = count & 0b11;
    while (count--) {
        *d++ = *s++;
    }

    return dest;
}
