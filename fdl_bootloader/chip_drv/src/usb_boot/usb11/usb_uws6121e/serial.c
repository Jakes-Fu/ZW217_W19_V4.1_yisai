
#include "global_macros.h"
//#include "arm_uart.h"
#include "sys_ctrl_6121e.h"
#include "serial.h"
#include "uart_reg_uws6121e.h"

#include "ap_apb.h"                     //w217

#include "iomux_reg_uws6121e.h"         //w217
#include "ap_clk.h"                     //w217
#include "lps_apb.h"                    //w217
#include "slv_fw_ap_ifc.h"              //w217

//#define hwp_uart hwp_uart1
#define hwp_uart hwp_uart1

#define DIVISOR_MODE_16 (0)
#define DIVISOR_MODE_4 (1)

#if 1
//#define OSI_ABS(type, a) ({ type _a = (type)(a); _a > 0? _a : -_a; })
#define UART1_SRC_FREQ (26000000)
#define UART2_SRC_FREQ (26000000)
#define UART3_SRC_FREQ (26000000)
#define UART4_SRC_FREQ (26000000)

typedef unsigned short uint16_t;
typedef unsigned long long uint64_t;

typedef struct
{
    uint16_t num;
    uint16_t denom;
} romFracDividerValue_t;

int builtin_clz(unsigned int x)
{
	int n = 0;
	x |=1;
	while(!(x & 0x80000000))
	{
		n++;
		x <<= 1;
	}
	return n;
}

romFracDividerValue_t romCalcFracDivider(unsigned input, unsigned output,
                                         unsigned num_bits,
                                         unsigned denom_bits)
{

	int delta = -1U;
	int rnum = 0, rdenom = 0;
	romFracDividerValue_t divider;
	int inavail_bits;
	unsigned int in_up=0;
	int num=0;
	unsigned int denom;
	unsigned int out_up;
	unsigned int diff;


	divider.denom = 0;
	divider.num = 0;
	inavail_bits = builtin_clz(input);

    if (input == 0 || output == 0 || output > input)
        return divider;

    // Ensure no overflow: input*num, output*denom

    if (inavail_bits < num_bits + 1)
    {
        input >>= (num_bits + 1 - inavail_bits);
        output >>= (num_bits + 1 - inavail_bits);
    }


    for (num = 1; num < (1 << num_bits); num++)
    {
         in_up = (unsigned int)input * num;
         denom = (in_up + output / 2) / output;
        if (denom >= (1 << denom_bits))
            break;

        // Maybe a little over-complex. If the difference is
        // calculated by (input*num/denom), sub-optimized value
        // may be choosen when output is small.
         out_up = (unsigned int)output * denom;
         //diff = OSI_ABS(int, out_up - in_up);
		diff = (out_up - in_up)>0 ? (out_up - in_up):(in_up-out_up);
        diff = ((uint64_t)diff << denom_bits) / denom;
        if (diff < delta)
        {
            delta = diff;
            rnum = num;
            rdenom = denom;
        }
        if (diff == 0)
            break;
    }

    if (delta == -1U)
        return divider;

    divider.num = rnum;
    divider.denom = rdenom;
    return divider;
}

void uart4_SetBuad(unsigned int Buad)
{
	REG_AP_APB_CFG_CLK_UART4_T AP_APB_CFG_CLK_UART4;
	romFracDividerValue_t divider = romCalcFracDivider(UART4_SRC_FREQ / 4, Buad, 13, 17);

	AP_APB_CFG_CLK_UART4.v = hwp_apApb->cfg_clk_uart4;
	AP_APB_CFG_CLK_UART4.b.uart4_div_num = divider.num;
	AP_APB_CFG_CLK_UART4.b.uart4_div_denom = divider.denom;
	hwp_apApb->cfg_clk_uart4 = AP_APB_CFG_CLK_UART4.v;
}

void uart3_SetBuad(unsigned int Buad)
{
	REG_SYS_CTRL_CFG_CLK_UART3_T SYS_CTRL_CFG_CLK_UART3;
	romFracDividerValue_t divider = romCalcFracDivider(UART3_SRC_FREQ / 4, Buad, 10, 14);

	SYS_CTRL_CFG_CLK_UART3.v = hwp_sysCtrl->cfg_clk_uart3;
	SYS_CTRL_CFG_CLK_UART3.b.cfg_clk_uart3_num = divider.num;
	SYS_CTRL_CFG_CLK_UART3.b.cfg_clk_uart3_demod = divider.denom;
	hwp_sysCtrl->cfg_clk_uart3 = SYS_CTRL_CFG_CLK_UART3.v;

}

void uart2_SetBuad(unsigned int Buad)
{
	REG_SYS_CTRL_CFG_CLK_UART2_T SYS_CTRL_CFG_CLK_UART2;
	romFracDividerValue_t divider = romCalcFracDivider(UART2_SRC_FREQ / 4, Buad, 10, 14);

	SYS_CTRL_CFG_CLK_UART2.v = hwp_sysCtrl->cfg_clk_uart2;
	SYS_CTRL_CFG_CLK_UART2.b.cfg_clk_uart2_num = divider.num;
	SYS_CTRL_CFG_CLK_UART2.b.cfg_clk_uart2_demod = divider.denom;
	hwp_sysCtrl->cfg_clk_uart2 = SYS_CTRL_CFG_CLK_UART2.v;
}

void uart1_SetBuad(unsigned int Buad)
{
	REG_LPS_APB_CFG_CLK_UART1_T LPS_APB_CFG_CLK_UART1;
	romFracDividerValue_t divider = romCalcFracDivider(UART1_SRC_FREQ / 4, Buad, 10, 14);

	LPS_APB_CFG_CLK_UART1.v = hwp_lpsApb->cfg_clk_uart1;
	LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_num = divider.num;
	LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_demod = divider.denom;
	LPS_APB_CFG_CLK_UART1.b.cfg_clk_uart1_update = 1;
	hwp_lpsApb->cfg_clk_uart1 = LPS_APB_CFG_CLK_UART1.v;
}

void uart4_io_init(void)
{
	// UART4 is not default IO
    hwp_iomux_6121e->uart_2_cts = 0x10; // UART_4_RX
    hwp_iomux_6121e->uart_2_rts = 0x10; // UART_4_TX
    hwp_iomux_6121e->pad_uart_2_cts &= ~(IOMUX_PAD_UART_2_CTS_DRV(0x3));
    hwp_iomux_6121e->pad_uart_2_cts |= IOMUX_PAD_UART_2_CTS_DRV(2);
    hwp_iomux_6121e->pad_uart_2_cts &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_2_cts |= IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_2_rts &= ~(IOMUX_PAD_UART_2_RTS_DRV(0x3));
    hwp_iomux_6121e->pad_uart_2_rts |= IOMUX_PAD_UART_2_RTS_DRV(2);
}

void uart3_io_init(void)
{
    // UART3 is not default IO
    hwp_iomux_6121e->gpio_17 = 0x4; // UART_3_RX
    hwp_iomux_6121e->gpio_16 = 0x4; // UART_3_TX
    hwp_iomux_6121e->pad_gpio_17 &= ~(IOMUX_PAD_GPIO_17_DRV(0xf));
    hwp_iomux_6121e->pad_gpio_17 |= IOMUX_PAD_GPIO_17_DRV(2);
    hwp_iomux_6121e->pad_gpio_17 &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_gpio_17 |= IOMUX_WPU;
    hwp_iomux_6121e->pad_gpio_16 &= ~(IOMUX_PAD_GPIO_16_DRV(0xf));
    hwp_iomux_6121e->pad_gpio_16 |= IOMUX_PAD_GPIO_16_DRV(2);
}

void uart2_io_init(void)
{	
	hwp_iomux_6121e->gpio_0 = 0x1c;//UART2_RX
	hwp_iomux_6121e->gpio_1 = 0x1c;//UART2_TX
	hwp_iomux_6121e->pad_gpio_0 &= ~(IOMUX_PAD_GPIO_0_DRV(0xf));
	hwp_iomux_6121e->pad_gpio_0 |= IOMUX_PAD_GPIO_0_DRV(2);
	hwp_iomux_6121e->pad_gpio_0 &= ~IOMUX_WPU;
	hwp_iomux_6121e->pad_gpio_0 |= IOMUX_WPU;
	hwp_iomux_6121e->pad_gpio_1 &= ~(IOMUX_PAD_GPIO_1_DRV(0xf));
	hwp_iomux_6121e->pad_gpio_1 |= IOMUX_PAD_GPIO_1_DRV(2);
}

void uart1_io_init(void)
{
    // UART1: set iomux
    hwp_iomux_6121e->pad_uart_1_rxd &= ~(IOMUX_PAD_UART_1_RXD_DRV(0xf));
    hwp_iomux_6121e->pad_uart_1_rxd |= IOMUX_PAD_UART_1_RXD_DRV(7); //rx_drving = 2
    hwp_iomux_6121e->pad_uart_1_rxd &= ~IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_1_rxd |= IOMUX_WPU;
    hwp_iomux_6121e->pad_uart_1_txd &= ~(IOMUX_PAD_UART_1_TXD_DRV(0xf));
    hwp_iomux_6121e->pad_uart_1_txd |= IOMUX_PAD_UART_1_TXD_DRV(7); //rx_drving = 2
}


#define MASTER_ID_APA5_CODE (0)
#define MASTER_ID_APA5_DATA (1)
#define MASTER_ID_CE_SEC (4)
#define MASTER_ID_EMMC (6)
#define MASTER_ID_USB (9)
#define MASTER_ID_DAP (60)
#define MASTER_ID_FDMA (61)

#define SLAVE_ACCESS_NO (0)
#define SLAVE_ACCESS_SECURE_ONLY (1)
#define SLAVE_ACCESS_NONSECURE_ONLY (2)
#define SLAVE_ACCESS_SECURE_NONESECURE (3)

static void romFirewallEnableMaster(volatile uint32 *reg, unsigned id)
{
    unsigned regn = id / 32;
    unsigned mask = 1 << (id % 32);
    reg[regn] |= mask;
}

void SetUart4Firewall(void)
{
	REG_SLV_FW_AP_IFC_RD_SEC_0_T SLV_FW_AP_IFC_RD_SEC_0;
	REG_SLV_FW_AP_IFC_WR_SEC_0_T SLV_FW_AP_IFC_WR_SEC_0;

    // slave firewall for uart4 registers
    romFirewallEnableMaster(&hwp_slvFwApIfc->id0_mstid_0, MASTER_ID_APA5_CODE);
    romFirewallEnableMaster(&hwp_slvFwApIfc->id0_mstid_0, MASTER_ID_APA5_DATA);
    romFirewallEnableMaster(&hwp_slvFwApIfc->id0_mstid_0, MASTER_ID_DAP);
    romFirewallEnableMaster(&hwp_slvFwApIfc->id0_mstid_0, MASTER_ID_FDMA);
    hwp_slvFwApIfc->id0_first_addr_0 = 0;
    hwp_slvFwApIfc->id0_last_addr_0 = 0xffffffff;

	SLV_FW_AP_IFC_RD_SEC_0.v = hwp_slvFwApIfc->rd_sec_0;
	SLV_FW_AP_IFC_RD_SEC_0.b.uart4_rd_sec = SLAVE_ACCESS_SECURE_ONLY;
	hwp_slvFwApIfc->rd_sec_0 = SLV_FW_AP_IFC_RD_SEC_0.v;

	SLV_FW_AP_IFC_WR_SEC_0.v = hwp_slvFwApIfc->wr_sec_0;
	SLV_FW_AP_IFC_WR_SEC_0.b.uart4_wr_sec = SLAVE_ACCESS_SECURE_ONLY;
	hwp_slvFwApIfc->wr_sec_0 = SLV_FW_AP_IFC_WR_SEC_0.v;
}

#endif

HWP_UART_T * p_uart = hwp_uart;

static void _serial_init(uart_instance_t uart)
{
	REG_UART_CTRL_T ctrl;
	REG_UART_CMD_SET_T cmd_set;
	REG_SYS_CTRL_AON_CLOCK_EN0_T clken23;
	REG_AP_APB_CLK_AP_EN0_T clken;

	switch(uart)
	{
		case uart1_instance:
			uart1_io_init();
			uart1_SetBuad(921600);
			p_uart = hwp_uart1;
			break;

		case uart2_instance:
			clken23.v = hwp_sysCtrl->aon_clock_en0;
			clken23.b.uart2_en = 1;

			uart2_io_init();
			uart2_SetBuad(921600);
			p_uart = hwp_uart2;
			hwp_sysCtrl->aon_clock_en0 = clken23.v;
			break;

		case uart3_instance:
			clken23.v = hwp_sysCtrl->aon_clock_en0;
			clken23.b.uart3_en = 1;

			uart3_io_init();
			uart3_SetBuad(921600);
			p_uart = hwp_uart3;
			hwp_sysCtrl->aon_clock_en0 = clken23.v;
			break;

		case uart4_instance:
			clken.v = hwp_apApb->clk_ap_en0;
			clken.b.clk_en_uart4_bf_div_fr = 1;
			hwp_apApb->clk_ap_en0 = clken.v;
			uart4_io_init();
			uart4_SetBuad(921600);
			p_uart = hwp_uart4;
			break;

		default:
			break;
	}

	p_uart->ctrl = 0XF0080003;
	//p_uart->status = 0X82001000;
	//p_uart->rxtx_buffer = 0X0000006F;
	p_uart->irq_mask = 0X00000000;
	p_uart->irq_cause = 0X00040000;
	p_uart->triggers = 0X00000000;
	p_uart->cmd_set = 0X00000000;
	p_uart->cmd_clr = 0X00000000;
	p_uart->auto_ratio = 0X00000000;
	p_uart->xon = 0X00000011;
	p_uart->xoff= 0X00000013;

#if 0
	ctrl.b.enable = 1;
	ctrl.b.data_bits = 1;
	ctrl.b.data_bits_56 = 0;
	ctrl.b.tx_stop_bits = UART_TX_STOP_BITS_V_1_BIT;
	ctrl.b.parity_enable = UART_PARITY_ENABLE_V_NO;
	ctrl.b.parity_select = 0;
	ctrl.b.soft_flow_ctrl_enable = 0;
	ctrl.b.auto_enable = 0;
	ctrl.b.divisor_mode = DIVISOR_MODE_4;
	ctrl.b.irda_enable = 0;
	ctrl.b.dma_mode = 0;
	ctrl.b.auto_flow_control = 0;
	ctrl.b.loop_back_mode = 0;
	ctrl.b.rx_lock_err = 0;
	ctrl.b.rx_break_length = 15;

	cmd_set.b.rx_fifo_reset = 1;
	cmd_set.b.tx_fifo_reset = 1;

	hwp_uart->irq_mask = 0;
	hwp_uart->ctrl = ctrl.v;

	hwp_uart->cmd_set = cmd_set.v;
	hwp_uart->cmd_set = cmd_set.v;
#endif

}

static void _serial_enable_rtscts(void)
{
	//hwp_uart->UART_CONF |= ARM_UART_HWFC;
}

/*
 * Test whether a character is in the RX buffer
 */
static int _serial_tstc(void)
{
	//return (GET_BITFIELD(hwp_uart->uart_status, ARM_UART_RX_FIFO_CNT));
	//return (hwp_uart->UART_RXFIFO_STAT > ARM_UART_RX_FIFO_CNT(0));
	return 0;
}

#define UART_TIMEOUT  10000
static int _serial_getc(void)
{
	REG_UART_STATUS_T status;

	status.v = p_uart->status;
	do
	{
		status.v = p_uart->status;
	}while(status.b.rx_fifo_level == 0);

	return ((p_uart->rxtx_buffer) & 0xff);
}


#if 0  //uart4 debug_demo
typedef unsigned int uint32_t;
// status
typedef union {
    uint32_t v;
    struct
    {
        uint32_t rx_fifo_level : 8;        // [7:0], read only
        uint32_t tx_fifo_space : 5;        // [12:8], read only
        uint32_t at_match_flag : 1;        // [13], read only
        uint32_t tx_active : 1;            // [14], read only
        uint32_t rx_active : 1;            // [15], read only
        uint32_t rx_overflow_err : 1;      // [16], read only
        uint32_t tx_overflow_err : 1;      // [17], read only
        uint32_t rx_parity_err : 1;        // [18], read only
        uint32_t rx_framing_err : 1;       // [19], read only
        uint32_t rx_break_int : 1;         // [20], read only
        uint32_t character_miscompare : 1; // [21], read only
        uint32_t auto_baud_locked : 1;     // [22], read only
        uint32_t __23_23 : 1;              // [23]
        uint32_t dcts : 1;                 // [24], read only
        uint32_t cts : 1;                  // [25], read only
        uint32_t auto_ratio_flag : 1;      // [26], read only
        uint32_t mask_tx_enable_flag : 1;  // [27], read only
        uint32_t dtr : 1;                  // [28], read only
        uint32_t __30_29 : 2;              // [30:29]
        uint32_t clk_enabled : 1;          // [31], read only
    } b;
} DEBUG_REG_UART_STATUS_T;

typedef volatile struct
{
    uint32_t ctrl;        // 0x00000000
    uint32_t status;      // 0x00000004
    uint32_t rxtx_buffer; // 0x00000008
    uint32_t irq_mask;    // 0x0000000c
    uint32_t irq_cause;   // 0x00000010
    uint32_t triggers;    // 0x00000014
    uint32_t cmd_set;     // 0x00000018
    uint32_t cmd_clr;     // 0x0000001c
    uint32_t auto_ratio;  // 0x00000020
    uint32_t xon;         // 0x00000024
    uint32_t xoff;        // 0x00000028
} DEBUG_HWP_UART_T;

#define DEBUG_REG_UART4_BASE (0x04400000)
#define DEBUG_hwp_uart4 ((DEBUG_HWP_UART_T *)(DEBUG_REG_UART4_BASE))
//extern void Debug_Uart4_puts(char *Str);

void Debug_Uart4_puts(char *Str)
{
	int i=0;
	DEBUG_REG_UART_STATUS_T status;

	while(Str[i]!=0)
	{
		status.v = DEBUG_hwp_uart4->status;
		do{
			status.v = DEBUG_hwp_uart4->status;
		}while(status.b.tx_fifo_space == 0);
		DEBUG_hwp_uart4->rxtx_buffer = c;
	}
	DEBUG_hwp_uart4->rxtx_buffer = '\r';
}

#endif



static void _serial_putc_hw(const char c)
{

	REG_UART_STATUS_T status;

	status.v = p_uart->status;

	do{
		status.v = p_uart->status;
	}while(status.b.tx_fifo_space == 0);

	p_uart->rxtx_buffer = c;
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

void serial_init(uart_instance_t uart)
{
	_serial_init(uart);
	//_serial_puts("_serial_init\n");
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


//------ UART 1 PAC DOWNLOAD Download_ -----
LOCAL UartPort_T gUart1DownloadPortInfo = //UART1
{
    0x51700000,
    115200
};



#define CHANNEL_DEFAULT_REGBASE 0x51700000   //UART1

unsigned int Download_SIO_GetHwDivider (unsigned int baudrate)
{
    //return (unsigned int) ( (ARM_APB_CLK + baudrate / 2) / baudrate);
	/*UART1 and UART4 (115200,baudrete has been configured in RomCode)*/
	return 0;
}

void Download_SIO_HwOpen (struct FDL_ChannelHandler *channel, unsigned int divider)
{

	REG_UART_CTRL_T ctrl;
	REG_UART_CMD_SET_T cmd_set;

	HWP_UART_T *Regbase;
    UartPort_T *port;

	if(channel != NULL)
	{
		port = (UartPort_T *) channel->priv;
		Regbase = (HWP_UART_T *)port->regBase;
	}
	else
	{
		Regbase = (HWP_UART_T *)(CHANNEL_DEFAULT_REGBASE);
	}

	ctrl.b.enable = 0;
	ctrl.b.data_bits = 1;
	ctrl.b.data_bits_56 = 0;
	ctrl.b.tx_stop_bits = UART_TX_STOP_BITS_V_1_BIT;
	ctrl.b.parity_enable = UART_PARITY_ENABLE_V_NO;
	ctrl.b.parity_select = 0;
	ctrl.b.soft_flow_ctrl_enable = 0;
	ctrl.b.auto_enable = 0;
	ctrl.b.divisor_mode = 1; //DIVISOR_MODE_4;
	ctrl.b.irda_enable = 0;
	ctrl.b.dma_mode = 0;
	ctrl.b.auto_flow_control = 0;
	ctrl.b.loop_back_mode = 0;
	ctrl.b.rx_lock_err = 0;
	ctrl.b.rx_break_length = 15;
	cmd_set.b.rx_fifo_reset = 1;
	cmd_set.b.tx_fifo_reset = 1;
	Regbase->irq_mask = 0;
	Regbase->ctrl = ctrl.v;
	Regbase->cmd_set = cmd_set.v;
	Regbase->cmd_set = cmd_set.v;
}

int Download_SIO_Open (struct FDL_ChannelHandler *channel, unsigned int baudrate)
{
    unsigned int divider;
    divider = Download_SIO_GetHwDivider (baudrate);
    Download_SIO_HwOpen (channel, divider);
    return 0;

}

int Download_SIO_Read (struct FDL_ChannelHandler *channel, const unsigned char *buf, unsigned int len)
{
	unsigned int i;
	unsigned char *pstart = (unsigned char *) buf;
	REG_UART_STATUS_T status;
	HWP_UART_T *Regbase;
    UartPort_T *port ;

	if(channel != NULL)
	{
		port = (UartPort_T *) channel->priv;
		Regbase = (HWP_UART_T *)port->regBase;
	}
	else
	{
		Regbase = (HWP_UART_T *)(CHANNEL_DEFAULT_REGBASE);
	}

	for(i=0;i<len;i++)
	{
		do
		{
			status.v = Regbase->status;
		}while(status.b.rx_fifo_level == 0);
		pstart[i] = (char)(((Regbase->rxtx_buffer) & 0xff));
	}
	return len;
}

char Download_SIO_GetChar (struct FDL_ChannelHandler *channel)
{
	REG_UART_STATUS_T status;
	HWP_UART_T *Regbase;
    UartPort_T *port ;

	if(channel != NULL)
	{
		port = (UartPort_T *) channel->priv;
		Regbase = (HWP_UART_T *)port->regBase;
		rprintf("Download_SIO_GetChar Regbase=0x%x\n",(unsigned int)Regbase);
	}
	else
	{
		Regbase = (HWP_UART_T *)(CHANNEL_DEFAULT_REGBASE);
	}

	//_serial_puts("GetChar");

	//Regbase = (HWP_UART_T *)port->regBase;
	status.v = Regbase->status;
	do	{
		status.v = Regbase->status;
	}while(status.b.rx_fifo_level == 0);
	return ((Regbase->rxtx_buffer) & 0xff);


	return ((p_uart->rxtx_buffer) & 0xff);
}

int Download_SIO_GetSingleChar(struct FDL_ChannelHandler  *channel)
{

	REG_UART_STATUS_T status;

	HWP_UART_T *Regbase;
    UartPort_T *port ;

	if(channel != NULL)
	{
		port = (UartPort_T *) channel->priv;
		Regbase = (HWP_UART_T *)port->regBase;
	}
	else
	{
		Regbase = (HWP_UART_T *)(CHANNEL_DEFAULT_REGBASE);
	}


	//_serial_puts("GetSingleChar");

	status.v = Regbase->status;

	if(status.b.rx_fifo_level == 0)
	{
		return -1;
	}

	return ((Regbase->rxtx_buffer) & 0xff);
}

int Download_SIO_Write (struct FDL_ChannelHandler  *channel, const unsigned char *buf, unsigned int len)
{
	unsigned int i;

	REG_UART_STATUS_T status;
	unsigned char *pstart = (unsigned char *) buf;

	HWP_UART_T *Regbase;
    UartPort_T *port ;

	if(channel != NULL)
	{
		port = (UartPort_T *) channel->priv;
		Regbase = (HWP_UART_T *)port->regBase;
	}
	else
	{
		Regbase = (HWP_UART_T *)(CHANNEL_DEFAULT_REGBASE);
	}

	status.v = Regbase->status;
	for(i=0;i<len;i++)
	{
		do{
		status.v = Regbase->status;
		}while(status.b.tx_fifo_space == 0);
		Regbase->rxtx_buffer = pstart[i];
	}
	return i;
}

int Download_SIO_PutChar (struct FDL_ChannelHandler  *channel, const unsigned char ch)
{
    REG_UART_STATUS_T status;
	HWP_UART_T *Regbase;
    UartPort_T *port ;

	if(channel != NULL)
	{
		port = (UartPort_T *) channel->priv;
		Regbase = (HWP_UART_T *)port->regBase;
	}
	else
	{
		Regbase = (HWP_UART_T *)(CHANNEL_DEFAULT_REGBASE);
	}
	status.v = Regbase->status;

	do{
		status.v = Regbase->status;
	}while(status.b.tx_fifo_space == 0);
	Regbase->rxtx_buffer = ch;
	return 0;
}
int Download_SIO_SetBaudrate (struct FDL_ChannelHandler  *channel,  unsigned int baudrate)
{
    /* Not implemented yet,follow-up debgging */
	return 0;
}

int Download_SIO_Close (struct FDL_ChannelHandler  *channel)
{
    return 0;
}

unsigned int Download_GetRegBase(struct FDL_ChannelHandler *channel)
{

	HWP_UART_T *Regbase;
    UartPort_T *port  = (UartPort_T *) channel->priv;

	Regbase = (HWP_UART_T *)port->regBase;
	return (unsigned int)Regbase;
}


FDL_ChannelHandler_T gUart1_DownloadChannel =
{
    Download_SIO_Open,
    Download_SIO_Read,
    Download_SIO_GetChar,
    Download_SIO_GetSingleChar,
    Download_SIO_Write,
    Download_SIO_PutChar,
    Download_SIO_SetBaudrate,
    Download_SIO_Close,
    &gUart1DownloadPortInfo
};

















