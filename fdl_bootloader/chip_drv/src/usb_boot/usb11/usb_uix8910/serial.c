
#include "global_macros.h"
//#include "arm_uart.h"
#include "sys_ctrl.h"
#include "serial.h"


#define hwp_uart hwp_uart2

static void _serial_init(void)
{
	/* baudrate use HW default 921600 */
	hwp_uart->UART_TXTRIG = ARM_UART_TX_TRIG(1);	//7 ?
	hwp_uart->UART_RXTRIG = ARM_UART_RX_TRIG(1);	//7 ?

	/* Don't block UART even if peer is not connected */
	hwp_uart->UART_CONF &= ~(ARM_UART_STOP_BIT |
				ARM_UART_CHECK |
				ARM_UART_ST_CHECK |
				ARM_UART_RX_IE |
				ARM_UART_TOUT_IE);

	/* Allow reception */
	hwp_uart->UART_CONF |= ARM_UART_RX_TRIG_HWFC;
}

static void _serial_enable_rtscts(void)
{
	hwp_uart->UART_CONF |= ARM_UART_HWFC;
}

/*
 * Test whether a character is in the RX buffer
 */
static int _serial_tstc(void)
{
	//return (GET_BITFIELD(hwp_uart->uart_status, ARM_UART_RX_FIFO_CNT));
	return (hwp_uart->UART_RXFIFO_STAT > ARM_UART_RX_FIFO_CNT(0));
}

#define UART_TIMEOUT  10000
static int _serial_getc(void)
{
	int timeout = 0;
	/* wait for character to arrive */ ;
	//while (!(GET_BITFIELD(hwp_uart->uart_status, ARM_UART_RX_FIFO_CNT)))
	while (!(hwp_uart->UART_RXFIFO_STAT > ARM_UART_RX_FIFO_CNT(0))) ;

	return (hwp_uart->UART_RX & 0xff);
}

static void _serial_putc_hw(const char c)
{
	int timeout = 0;
	// Place in the TX Fifo ?
	//while (!(GET_BITFIELD(hwp_uart->status, ARM_UART_TX_FIFO_CNT)))
	while (!(hwp_uart->UART_TXFIFO_STAT < ARM_UART_TX_FIFO_CNT(16))) ;

	hwp_uart->UART_TX = (uint32) c;
    while (hwp_uart->UART_TXFIFO_STAT);
}

static void _serial_putc(const char c)
{
	if (c == '\n') {
		_serial_putc_hw('\r');
	}
	_serial_putc_hw(c);
}

static void _serial_puts(const char *s)
{
	while (*s) {
		_serial_putc(*s++);
	}
}

void serial_init(void)
{
	hwp_sysCtrl->Cfg_MISC_CFG |= SYS_CTRL_AP_UART_OUT_SEL;
	_serial_init();
}

void serial_enable_rtscts(void)
{
	_serial_enable_rtscts();
}

int serial_getc(void)
{
	return _serial_getc();
}

int serial_tstc(void)
{
	return _serial_tstc();
}

void serial_putc(const char c)
{
	_serial_putc(c);
}

void serial_puts(const char *s)
{
	_serial_puts(s);
}

static void backspace(int cnts)
{
	int i;

	for (i = 0; i < cnts; i++) {
		serial_putc('\b');
		serial_putc(' ');
		serial_putc('\b');
	}
}

int serial_gets(uint8 * pstr)
{
	uint32 length;

	length = 0;
	while (1) {
		pstr[length] = serial_getc();
		if (pstr[length] == '\r') {
			pstr[length] = 0x00;
			break;
		} else if (pstr[length] == '\b') {
			if (length > 0) {
				length--;
				backspace(1);
			}
		} else {
			serial_putc(pstr[length]);
			length++;
		}
		if (length > 16)
			return -1;
	}
	return 0;
}

static char *flex_i2a(unsigned num, char *pstr, int radix, int len, int digits)
{
	char ch;
	char *p;
	int sign = 0;

	if (radix < 0) {
		radix = -radix;
		sign = 1;
	}
	// limit the string and number of displayed digits
	if (digits > len - 1)
		digits = len - 1;
	p = pstr + len;
	*--p = 0;

	// print the number including sign
	while (pstr != p) {
		unsigned int n = num / radix;
		ch = num - n * radix;
		num = n;
		ch += (ch < 10) ? '0' : 'A' - 10;
		*--p = ch;
		if (num == 0)
			break;
	}

	// output the sign
	if (sign && pstr != p) {
		*--p = '-';
	}

	if (digits == 0)
		return p;

	ch = (radix == 16) ? '0' : ' ';
	len = digits - (pstr + len - p - 1);	// target_digits - current_chars
	for (; len > 0; len--)
		*--p = ch;

	return p;
}

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

void rprintf(const char *fmt, ...)
{
	va_list ap;

	char str[16];
	int iarg;
	const char *s;

	va_start(ap, fmt);

	s = fmt;
	while (*s) {
		if (*s == '%') {
			int size = 0;
			int radix = 10;
			const char *p = 0;

			++s;
			if (*s == '0')
				s++;
			if (*s >= '0' && *s <= '9') {
				size = *s++ - '0';
			};

			switch (*s) {
			case 's':
				p = va_arg(ap, const char *);
				break;
			case 'x':
				radix = 16;
			case 'd':
			case 'u':
				iarg = va_arg(ap, int);
				if (*s == 'd' && iarg < 0) {
					radix = -10;
					iarg = -iarg;
				}
				p = flex_i2a(iarg, str, radix, sizeof(str),
					     size);
				break;
			case '%':
				_serial_putc('%');
				break;
			case 'c':
				iarg = va_arg(ap, int);
				str[0] = iarg;
				str[1] = 0;
				p = str;
			default:;
			}

			if (p)
				_serial_puts(p);
		} else {
			_serial_putc(*s);
		}
		++s;
	}

	va_end(ap);
}

static uint64 asc2hex(uint8 * pstr, uint8 len)
{
	uint8 i, ch, mylen;
	uint64 hexvalue;

	for (mylen = 0, i = 0; i < 16; i++) {
		if (pstr[i] == 0)
			break;
		mylen++;
	}
	if (len != 0) {
		if (mylen > len)
			mylen = len;
	}
	if (mylen > 16)
		mylen = 16;

	hexvalue = 0;
	for (i = 0; i < mylen; i++) {
		hexvalue <<= 4;
		ch = *(pstr + i);
		if ((ch >= '0') && (ch <= '9'))
			hexvalue |= ch - '0';
		else if ((ch >= 'A') && (ch <= 'F'))
			hexvalue |= ch - ('A' - 10);
		else if ((ch >= 'a') && (ch <= 'f'))
			hexvalue |= ch - ('a' - 10);
		else;
	}
	return (hexvalue);
}

void dump_buf(uint8 *buf, uint32 size)
{
	uint32 i, j;

	size += (size % 16);
	for (i = 0; i < size; i += 16) {
		for (j = 0; j < 16; j++) {
			rprintf("%d ", buf[i + j]);
		}
		serial_puts("\n");
	}
}

PUBLIC uint32 SCI_TraceLow(
    const char *x_format, ...)
{
	va_list ap;

	char str[16];
	int iarg;
	const char *s;

	va_start(ap, x_format);

	s = x_format;
	while (*s) {
		if (*s == '%') {
			int size = 0;
			int radix = 10;
			const char *p = 0;

			++s;
			if (*s == '0')
				s++;
			if (*s >= '0' && *s <= '9') {
				size = *s++ - '0';
			};

			switch (*s) {
			case 's':
				p = va_arg(ap, const char *);
				break;
			case 'x':
				radix = 16;
			case 'd':
			case 'u':
				iarg = va_arg(ap, int);
				if (*s == 'd' && iarg < 0) {
					radix = -10;
					iarg = -iarg;
				}
				p = flex_i2a(iarg, str, radix, sizeof(str), size);
				break;
			case '%':
				_serial_putc('%');
				break;
			case 'c':
				iarg = va_arg(ap, int);
				str[0] = iarg;
				str[1] = 0;
				p = str;
			default:;
			}

			if (p)
				_serial_puts(p);
		} else {
			_serial_putc(*s);
		}
		++s;
	}

	va_end(ap);

    rprintf("\n");

    return 0;
}

