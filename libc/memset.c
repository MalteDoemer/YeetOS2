#include "stdint.h"
#include "stddef.h"

void* memset(void* dest, int value, size_t n)
{
    uint8_t* d = dest;

    while (n--) {
        *d++ = (uint8_t)value;
    }

    return dest;
}