#ifndef ASM_H
#define ASM_H

#include "stdint.h"
#include "stddef.h"

static inline void outb(uint16_t port, uint8_t data)
{
    __asm volatile("outb %1, %0" ::"dN"(port), "a"(data));
}

static inline void outw(uint16_t port, uint16_t data)
{
    __asm volatile("outw %1, %0" ::"dN"(port), "a"(data));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    __asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

static inline void cli()
{
    __asm("cli");
}

static inline void sti()
{
    __asm("sti");
}

static inline void hlt()
{
    __asm("hlt");
}

#endif // #ifndef ASM_H