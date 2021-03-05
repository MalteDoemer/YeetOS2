
#include "yeetos/kernel.h"
#include "yeetos/tty.h"
#include "arch/x86/arch.h"

#define WIDTH 80
#define HEIGHT 25
#define TAB_SIZE 4

#define VRAM_START (0xB8000 + KERNEL_BASE)
#define VRAM_END (0xC0000 + KERNEL_BASE)

static uint32_t origin = (uint32_t)VRAM_START;
static uint32_t cpos = 0;
static uint8_t attrs = 0x0F;

static inline void gotoxy(uint32_t x, uint32_t y)
{
    if (x >= WIDTH || y >= HEIGHT)
        return;

    cpos = y * WIDTH + x;
}

static inline void update_origin(void)
{
    cli();
    outb(0x3d4, 12);
    outb(0x3d5, 0xff & ((origin - VRAM_START) >> 9));
    outb(0x3d4, 13);
    outb(0x3d5, 0xff & ((origin - VRAM_START) >> 1));
    sti();
}

static inline void update_cursor()
{
    outb(0x3D4, 14);
    outb(0x3D5, cpos >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cpos);
}

static void scroll_up()
{
}

static void scroll_down()
{
}

void vga_text_init()
{
}

void vga_text_write(tty_t* tty)
{
    size_t num;
    char c;

    num = TTY_NUM(tty->write_q);

    while (num--) {
        TTY_GETCH(tty->write_q, c);

        if (c == '\b') {
            cpos--;
        } else if (c == '\t') {
            for (int i = 0; i < TAB_SIZE; i++) {
                ((uint16_t*)origin)[cpos + i] = (attrs << 8) | ' ';
            }
            cpos = (cpos + TAB_SIZE) & ~(TAB_SIZE - 1);
        } else if (c == '\n') {
            cpos += WIDTH;
            cpos = (cpos / WIDTH) * WIDTH;
        } else if (c == '\r') {
            cpos = (cpos / WIDTH) * WIDTH;
        } else {
            ((uint16_t*)origin)[cpos] = (attrs << 8) | c;
            cpos++;
        }
    }

    update_cursor();
}
