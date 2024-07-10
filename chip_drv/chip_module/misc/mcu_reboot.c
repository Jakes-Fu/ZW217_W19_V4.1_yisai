/******************************************************************************
 ** File Name:  mcu_reboot.c                                *
 ** Author:                                                       *
 ** DATE:                                                           *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    .                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 **                         Create.                                   *
 ******************************************************************************/
#include "os_api.h"
#include "chip_plf_export.h"
#include "mcu_reboot.h"
#include "fdl_siodrv.h"
#include "fdl_cmddef.h"
#include "ref_outport.h"

#define BUG() \
    do { \
        SCI_ASSERT(0); /*assert verified*/ /* The phone will be restarted */ \
        /* We should not get here */ \
        for (;;) {} \
    } while (0)

#define CRC_16_POLYNOMIAL       0x1021
#define CRC_16_L_POLYNOMIAL     0x8000
#define CRC_16_L_SEED           0x80
#define CRC_16_L_OK             0x00
#define HDLC_FLAG               0x7E
#define HDLC_ESCAPE             0x7D
#define HDLC_ESCAPE_MASK        0x20
#define CRC_CHECK_SIZE          0x02

#define BUF_SIZE            0x40//0x800 not used now, so reduce the memory size
static __align (8) unsigned char g_tmp[BUF_SIZE];
static const char version_string[] = {"Spreadtrum Boot Block version 3.0"};

extern void remap_and_reboot (unsigned long boot_addr); /* defined in tx_deepsleep.s */


static void sio_open (unsigned int baudrate);
static void sio_hw_open (unsigned int baudrate);
static unsigned int hw_baudrate (unsigned int baudrate);
static void check_baudrate (void);
static void connect (void);
static void receive_packet (unsigned char *buf, int size, int *recv);
static void send_packet (unsigned char *buf, int size);
static void send_ack (DLSTATUS status);
static void start_write (unsigned long *start_addr, unsigned long *size);
static void write_data (unsigned long start_addr, unsigned long size);
static void execute (unsigned long entry_addr);
static unsigned short crc_16_l_calc (const unsigned char *buf, int len);

void mcu_nand_reboot (void)
{
    remap_and_reboot (0);
}

void mcu_uart_reboot (void)
{
    unsigned long start_addr;
    unsigned long size;

    sio_open (115200);
    check_baudrate();

    connect();

    start_write (&start_addr, &size);
    write_data (start_addr, size);
    execute (start_addr);

    BUG();
}
void sio_open (unsigned int baudrate)
{
    sio_hw_open (hw_baudrate (baudrate));
}

void sio_hw_open (unsigned int baudrate)
{
    /*Disable Interrupt */
    REG32 (ARM_UART0_Base + ARM_UART_IEN) = 0;

    /* Set baud rate  */
    REG32 (ARM_UART0_Base + ARM_UART_CLKD0) = LWORD (baudrate);
    REG32 (ARM_UART0_Base + ARM_UART_CLKD1) = HWORD (baudrate);


    /* Set port for 8 bit, one stop, no parity  */
    REG32 (ARM_UART0_Base + ARM_UART_CTL0) = UARTCTL_BL8BITS | UARTCTL_SL1BITS;
    REG32 (ARM_UART0_Base + ARM_UART_CTL1) = 0;
    REG32 (ARM_UART0_Base + ARM_UART_CTL2) = 0;
}

static unsigned int hw_baudrate (unsigned int baudrate)
{
    unsigned long apb_clk;

    apb_clk = CHIP_GetAPBClk();
    return (unsigned int) ( (apb_clk + baudrate / 2) / baudrate);
}

void check_baudrate (void)
{
    unsigned char   c;
    PKT_HEADER      *header = (PKT_HEADER *) g_tmp;

    do
    {
        c = sio_get_char();
    }
    while (HDLC_FLAG != c);

    header->type = BSL_REP_VER;
    header->size = sizeof (version_string);
    SCI_MEMCPY (g_tmp + sizeof (*header), version_string, sizeof (version_string));

    send_packet (g_tmp, sizeof (*header) + sizeof (version_string));
}

void receive_packet (unsigned char *buf, int size, int *recv)
{
    unsigned char  c;
    unsigned short crc;
    unsigned char *tmp = buf;
    int            total;

    do
    {
        c = sio_get_char();
    }
    while (HDLC_FLAG == c);

    total = 0;

    for (;;)
    {
        if (HDLC_FLAG == c)
        {
            break;
        }

        if (size == total)
        {
            send_ack (BSL_REP_VERIFY_ERROR);
            BUG();
        }

        if (HDLC_ESCAPE == c)
        {
            c = sio_get_char() ^ HDLC_ESCAPE_MASK;
        }

        *buf++ = c;
        total++;

        c = sio_get_char();
    }

    crc = crc_16_l_calc (tmp, total);

    if (0 != crc)
    {
        send_ack (BSL_REP_VERIFY_ERROR);
        BUG();
    }

    *recv = total - CRC_CHECK_SIZE; /* subtract the size of crc */
    return;
}

void send_packet (unsigned char *buf, int size)
{
    unsigned short crc;

    crc = crc_16_l_calc (buf, size);
    * (buf + size) = (crc >> 8) & 0x0FF;
    * (buf + size + 1) = crc & 0x0FF;
    size += CRC_CHECK_SIZE;

    sio_put_char (HDLC_FLAG);

    while (size--)
    {
        if ( (HDLC_FLAG == *buf) || (HDLC_ESCAPE == *buf))
        {
            sio_put_char (HDLC_ESCAPE);
            sio_put_char (~HDLC_ESCAPE_MASK & *buf);
        }
        else
        {
            sio_put_char (*buf);
        }

        ++buf;
    }

    sio_put_char (HDLC_FLAG);
}

void send_ack (DLSTATUS status)
{
    PKT_HEADER *header = (PKT_HEADER *) g_tmp;
    header->type = status;
    header->size = 0;

    send_packet (g_tmp, sizeof (*header));
}

void connect (void)
{
    PKT_HEADER  *header;
    int         recv;

    receive_packet (g_tmp, sizeof (g_tmp), &recv);
    header = (PKT_HEADER *) g_tmp;

    if (BSL_CMD_CONNECT != header->type)
    {
        send_ack (BSL_REP_INVALID_CMD);
        BUG();
    }

    send_ack (BSL_REP_ACK);
}

unsigned short crc_16_l_calc (const unsigned char *buf, int len)
{
    unsigned int    i;
    unsigned short  crc = 0;

    while (len--)
    {
        for (i = CRC_16_L_SEED; i != 0 ; i >>= 1)
        {
            if (crc & CRC_16_L_POLYNOMIAL)
            {
                crc <<= 1;
                crc ^= CRC_16_POLYNOMIAL;
            }
            else
            {
                crc <<= 1;
            }

            if (*buf & i)
            {
                crc ^= CRC_16_POLYNOMIAL;
            }
        }

        ++buf;
    }

    return (crc);
}

void start_write (unsigned long *start_addr, unsigned long *size)
{
    PKT_HEADER  *header;
    int         recv;

    receive_packet (g_tmp, sizeof (g_tmp), &recv);
    header = (PKT_HEADER *) g_tmp;

    if (BSL_CMD_START_DATA != header->type)
    {
        send_ack (BSL_REP_INVALID_CMD);
        BUG();
    }

    *start_addr = * (unsigned long *) (g_tmp + sizeof (*header));
    *size = * (unsigned long *) (g_tmp + sizeof (*header) + sizeof (unsigned long));
    send_ack (BSL_REP_ACK);
}

void write_data (unsigned long start_addr, unsigned long size)
{
    PKT_HEADER      *header;
    int             recv;
    unsigned long   total = 0;

    for (;;)
    {
        receive_packet (g_tmp, sizeof (g_tmp), &recv);
        header = (PKT_HEADER *) g_tmp;

        if (BSL_CMD_END_DATA == header->type)
        {
            break;
        }

        if (BSL_CMD_MIDST_DATA != header->type)
        {
            send_ack (BSL_REP_INVALID_CMD);
            BUG();
        }

        SCI_MEMCPY ( (void *) start_addr, g_tmp + sizeof (*header), header->size);
        total += header->size;
        start_addr += header->size;
        send_ack (BSL_REP_ACK);
    }

    if (total != size)
    {
        send_ack (BSL_REP_VERIFY_ERROR);
        BUG();
    }

    send_ack (BSL_REP_ACK);
}

void execute (unsigned long entry_addr)
{
    int recv;

    receive_packet (g_tmp, sizeof (g_tmp), &recv);

    if (BSL_CMD_EXEC_DATA != ( (PKT_HEADER *) g_tmp)->type)
    {
        send_ack (BSL_REP_INVALID_CMD);
        BUG();
    }

    send_ack (BSL_REP_ACK);
    remap_and_reboot (entry_addr);
}
