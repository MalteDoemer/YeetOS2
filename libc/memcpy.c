
#include "stdint.h"
#include "stddef.h"

void* memcpy(void* dest, const void* src, size_t num)
{
    uint8_t* d = dest;
    const uint8_t* s = src;

    while (num--){
        *d++ = *s++;
    }

    return dest;
}