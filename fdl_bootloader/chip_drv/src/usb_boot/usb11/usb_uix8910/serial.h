#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdarg.h>
#include "sci_types.h"

void serial_init(void);
void serial_enable_rtscts(void);
int serial_getc(void);
int serial_tstc(void);
void serial_putc(const char c);
void serial_puts(const char *s);
int serial_gets(uint8 *pstr);

void dump_buf(uint8 *buf, uint32 size);

/*
 rprintf() is a tiny version of printf() for use in the boot_rom.
 The format-string is very limited subset of printf():
 	%<size><format>
 where <size> is a single digit size specifier between 0 and 9. <size> is
 accepted for all <formats> but ignored for strings.

 The following <format> codes is supported:
    s    - string
    d    - decimal number, signed
    u    - decimal number, unsigned
    x    - hexadecimal number, unsigned
    %    - literal '%'

 Examples:
        rprintf("The answer to life, universe and %s is %d!\n", "everything", 42);
        rprintf("%d is %u in unsigned (hex: 0x%8x)\n", -42, -42, -42);
    Output:
        "The answer to life, universe and everything is 42!"
        "-42 is 4294967254 in unsigned (hex: 0xFFFFFFD6)"

*/
void rprintf(const char *fmt, ...);

#endif /* _SERIAL_H_ */

