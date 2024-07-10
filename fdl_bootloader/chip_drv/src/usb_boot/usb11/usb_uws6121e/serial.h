#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdarg.h>
#include "sci_types.h"
#include "fdl_channel.h"

/*
typedef struct FDL_ChannelHandlerUart
{
	unsigned int Regbase;
	unsigned int baudRate;
    int (*Open) (struct FDL_ChannelHandlerUart *channel, unsigned int  baudrate);
    int (*Read) (struct FDL_ChannelHandlerUart *channel, const unsigned char *buf, unsigned int  len);
    char (*GetChar) (struct FDL_ChannelHandlerUart *channel);
    int (*GetSingleChar) (struct FDL_ChannelHandlerUart *channel);
    int (*Write) (struct FDL_ChannelHandlerUart, const unsigned char *buf, unsigned int  len);
    int (*PutChar) (struct FDL_ChannelHandlerUart, const unsigned char ch);
    int (*SetBaudrate) (struct FDL_ChannelHandlerUart, unsigned int  baudrate);
    int (*Close) (struct FDL_ChannelHandlerUart *channel);

} FDL_ChannelHandlerUart_T;
*/

typedef struct UartPort
{
    unsigned int regBase;
    unsigned int baudRate;
} UartPort_T;

/*
typedef struct FDL_ChannelHandler
{
    int (*Open) (struct FDL_ChannelHandler *channel, unsigned int  baudrate);
    int (*Read) (struct FDL_ChannelHandler *channel, const unsigned char *buf, unsigned int  len);
    char (*GetChar) (struct FDL_ChannelHandler *channel);
    int (*GetSingleChar) (struct FDL_ChannelHandler *channel);
    int (*Write) (struct FDL_ChannelHandler *channel, const unsigned char *buf, unsigned int  len);
    int (*PutChar) (struct FDL_ChannelHandler *channel, const unsigned char ch);
    int (*SetBaudrate) (struct FDL_ChannelHandler *channel, unsigned int  baudrate);
    int (*Close) (struct FDL_ChannelHandler *channel);
    void   *priv;
} FDL_ChannelHandler_T;
*/

typedef enum uart_instance
{
	uart1_instance = 1,
	uart2_instance,
	uart3_instance,
	uart4_instance,
}uart_instance_t;

void serial_init(uart_instance_t uart);
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

