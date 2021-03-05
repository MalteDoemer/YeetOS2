#ifndef TTY_H
#define TTY_H

#include "stdint.h"
#include "stddef.h"

#define TTY_BUF_SIZE 1024

#define TTY_INC(a) ((a) = ((a) + 1) & (TTY_BUF_SIZE - 1))
#define TTY_NUM(a) (((a).head-(a).tail)&(TTY_BUF_SIZE-1))
#define TTY_EMPTY(a) ((a).head == (a).tail)
#define TTY_GETCH(queue, c) \
    (void)({c=(queue).buf[(queue).tail];TTY_INC((queue).tail); })
#define TTY_PUTCH(c, queue) \
    (void)({(queue).buf[(queue).head]=(c);TTY_INC((queue).head); })

typedef struct tty_queue_t {
    uint32_t head;
    uint32_t tail;
    char buf[TTY_BUF_SIZE];
} tty_queue_t;

typedef struct tty_t {
    uint32_t flags;
    void (*write)(struct tty_t* tty);
    tty_queue_t write_q;
    tty_queue_t read_q;
} tty_t;


tty_t* get_tty(uint32_t id);

int tty_read(uint32_t id, char* buffer, size_t num);
int tty_write(uint32_t id, char* buffer, size_t num);

#endif // #ifndef TTY_H
