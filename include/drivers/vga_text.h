#ifndef VGA_TEXT_H
#define VGA_TEXT_H

#include "stdint.h"

void vga_text_init();
void vga_text_write(uint32_t tty_id);

#endif // #ifndef VGA_TEXT_H
