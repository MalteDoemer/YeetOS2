#include "stdint.h"

#include "yeetos/kernel.h"
#include "yeetos/tty.h"

tty_t tty_table[2];

tty_t* get_tty(uint32_t id)
{
    return tty_table + id;
}

int tty_read(uint32_t id, char* buffer, size_t num)
{
    char c;
    char* b;
    tty_t* tty;

    tty = get_tty(id);

    for (b = buffer; num; num--, b++) {

        if (TTY_EMPTY(tty->read_q))
            break;

        TTY_GETCH(tty->read_q, c);
        *b = c;
    }
}

int tty_write(uint32_t id, char* buffer, size_t num)
{
    char* b;
    tty_t* tty;

    tty = get_tty(id);
    for (b = buffer; num; num--, b++) {
        TTY_PUTCH(*buffer, tty->write_q);
    }

    tty->write(tty);

    return b - buffer;
}

int tty_init()
{


    
    return 0;
}
