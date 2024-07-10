#include "common.h"
#include "bdl_packet.h"
#include "fdl_crc.h"
#include "fdl_stdio.h"
#include "sio_drv.h"
#ifdef SECURE_BOOT_SUPPORT
#include "sha1_32.h"
#endif

#ifdef __cplusplus
    extern   "C"
    {
#endif

//download flag for fdl
#define BOOT_FLAG_UART1                 	(0x6A)
#define BOOT_FLAG_UART0                 	(0x7A)

//BOOT mode Strapping Pin value
#define NOR_BOOT_MODE                   0x4
#define NAND_BOOT_MODE                  0x3
#define USB_BOOT_MODE                   0x2
#define UART_BOOT_MODE  		        0x1
#define USB_BOOT_MODE_2                 0x0

#define UART_IDENTIFY_COUNT             (3)

#define HEART_BEAT_CHAR				0x55

//Set uart getchar timeout 80ms
#define UART_TIMEOUT			       80

#ifdef SECURE_BOOT_SUPPORT
static volatile uint32 loop_var=0;
static uint32 fdl_file_size = 0;
#endif


extern sio_handle global_sio_handle;
extern unsigned int global_uart_base;

extern sys_stage_s      global_sys_stage;
extern char 			*rep_ack_ptr;
extern char 			*recv_data_buffer_ptr;
extern unsigned int 	rep_ack_array[];
extern unsigned int 	recv_data_buffer_array[];		   //Initialize the global variable   
extern const char       version_string[];
/**---------------------------------------------------------------------------*
 ** The Follow defines the packet processed function entry table              *
 **---------------------------------------------------------------------------*/
static void  process_packets(void);
static void  send_ack(cmd_pkt_type pkt_type);
static void sys_connect(char *pkt_buf, int pkt_len);
static void data_start(char *pkt_buf, int pkt_len);
static void data_midst(char *pkt_buf, int pkt_len);
static void data_end(char *pkt_buf, int pkt_len);
static void data_exec(char *pkt_buf, int pkt_len);
static void reg_access (char *pkt_buf, int pkt_len);
void sio_hw_init( unsigned int uart_base);
void sio_hw_open(unsigned int uart_base);
void sio_rx_handle(void);

const pkt_proc_fun_t proc_tab_t[] = {
  /* For Line Control */
  { /*BSL_CMD_CONNECT, */ sys_connect},

  /*For data download command */
  { /*BSL_CMD_START_DATA, */ data_start},
  { /*BSL_CMD_MIDST_DATA, */ data_midst},
  { /*BSL_CMD_END_DATA, */ data_end},
  { /*BSL_CMD_EXEC_DATA, */ data_exec},
  { /*BSL_CMD_REG_ACCESS, */ reg_access}
};

/*******************************************************************************/
void sio_open  (void)
{    
    //default baudrate is BAUD_115200      
    //set the uart state    
    global_sio_handle.sio_state   = BSL_REP_NOT_SUPPORT_BAUDRATE;
	global_sio_handle.baud_rate   = BAUD_115200_26M;    
    global_sio_handle.buf_size    = (int)   SIO_RECV_LIMIT;
    global_sio_handle.buf_ptr     = recv_data_buffer_ptr;
       
    //clear the receive buffer
    //FDL_memset((char *)global_sio_handle.buf_ptr, 0xCC, global_sio_handle.buf_size);

	sio_hw_open(global_uart_base);

    /* Adjust Baudrate automatically*/
    while(global_sio_handle.sio_state  != BSL_PHONE_VALID_BAUDRATE)
    {        
        unsigned int baudrate = global_sio_handle.baud_rate;              
    
	    /* change the baudrate double previous baudrate */
	    global_sio_handle.baud_rate = baudrate >> 1;
	    
		sio_hw_open(global_uart_base);	
    }    
}

void sio_write (char *write_buf,  int write_len)
{
	unsigned short crc;
	
  
    // @Richard
    // NOTE(Important):
    // We do not do mask because the message including crc do not have mask word!
    // So if there is mask word, we have to modify below codes.
    // 
    crc = crc_16_l_calc(write_buf, write_len - 2);
    
    *(write_buf + write_len - 2) = (crc >> 8) & 0x0FF;
    *(write_buf + write_len - 1) = crc & 0x0FF;
    *(write_buf + write_len )    = HDLC_FLAG;
    write_len++;
    
    /* Check if tx port is ready.*/
	while (SIO_TX_READY_E(SIO_GET_TX_STATUS(global_uart_base)))
    {
           //delay(10);
           ;
    }
      
    SIO_PUT_CHAR(global_uart_base, HDLC_FLAG);
    
        
    while(0 != (write_len))
	{
		/* Check if tx port is ready.*/
		while (SIO_TX_READY_E(SIO_GET_TX_STATUS(global_uart_base)))
        {
            //delay(10);
            ;
        }
        
		/* Send a char to the tx port.*/
		SIO_PUT_CHAR(global_uart_base, *write_buf++);
		
 	    write_len--;
	}	
	
	/* Ensure the last byte is output success*/
	while (SIO_TX_READY_E(SIO_GET_TX_STATUS(global_uart_base)))
    {
      //delay(10);
      ;
    }
}

ret_status sio_read  (char **read_buf, int *read_len)
{
    int  crc;
    
    //Reset the SIO receive status
    global_sio_handle.write_ptr = global_sio_handle.buf_ptr ;
	global_sio_handle.data_size = 0;
	global_sio_handle.pkt_state = PKT_NONE;

    //receive data from uart
    sio_rx_handle();    
    
    //ret_code = decode_msg(global_sio_handle.buf_ptr , global_sio_handle.data_size , read_buf,read_len);
    crc = crc_16_l_calc(global_sio_handle.buf_ptr, global_sio_handle.data_size); 
   
	if (CRC_16_L_OK != crc)
	{
		return BSL_REP_VERIFY_ERROR;
	}
	else
	{
	    *read_buf = global_sio_handle.buf_ptr;
	    *read_len = global_sio_handle.data_size;
	    return BSL_PHONE_SUCCEED;
	}
}

void sio_rx_handle(void)
{
     int ch;
     
     // Clear Uart Rx FIFO
     while(SIO_RX_READY_E(SIO_GET_RX_STATUS(global_uart_base)))
     {       
       ch = SIO_GET_CHAR(global_uart_base);
     }
     
     if(BSL_REP_NOT_SUPPORT_BAUDRATE == global_sio_handle.sio_state)
     {
         // @Xueliang.wang modify
         while(1)
         {
	         // Wait for UART inputing forever
	         while(!SIO_RX_READY_E(SIO_GET_RX_STATUS(global_uart_base)));
	         
	         ch = SIO_GET_CHAR(global_uart_base);
	         
	         // Check if it is break.
	         // If it is , continue do loop; else break.
			if(SIO_GET_BREAK_STATUS(global_uart_base))
			{
				(*(volatile unsigned int*) (global_uart_base + (unsigned int)ARM_UART_ICLR)) |= (unsigned int)UARTICLR_BREAK_DETECT;
			}
			else
			{
				break;
			}
         }
         
         // the input keyboard value is right that indicates the 
         //baudrate is matched 
         if(HDLC_FLAG == ch)
         {
             global_sio_handle.sio_state  = BSL_PHONE_VALID_BAUDRATE;
         }  
         else
         {
	        // @Xueliang.wang modify
	        // Add delay 2 ms.
         	int	i = 15000;
         	while( i-- );
	     }    
     }
     else
     {
         /* wait for that Now have received a complete packet */
         while(global_sio_handle.pkt_state != PKT_RECV )
         {   
             
             // @Xueliang.wang modify
	         while(1)
	         {
		         // Wait for UART inputing forever
		         while(!SIO_RX_READY_E(SIO_GET_RX_STATUS(global_uart_base)));
		         
		         ch = SIO_GET_CHAR(global_uart_base);
		         
		         // Check if it is break.
		         // If it is , continue do loop; else break.
				if(SIO_GET_BREAK_STATUS(global_uart_base))
				{
					(*(volatile unsigned int*) (global_uart_base + (unsigned int)ARM_UART_ICLR)) |= (unsigned int)UARTICLR_BREAK_DETECT;
				}
				else
				{
					break;
				}
	         }
             
            switch (global_sio_handle.pkt_state )
            {
            case PKT_NONE:
                if(HDLC_FLAG == ch)
                {
                    global_sio_handle.pkt_state      = PKT_HEAD;
                    //*(global_sio_handle.write_ptr++) = ch;
                    //global_sio_handle.data_size      += 1;
                }
                break;
            case PKT_HEAD:
                if(HDLC_FLAG != ch)
                {
                	if (HDLC_ESCAPE == ch)
                	{                    
                   		while(!SIO_RX_READY_E(SIO_GET_RX_STATUS(global_uart_base)));
             
		                ch = SIO_GET_CHAR(global_uart_base);
             
                		ch = ch ^ HDLC_ESCAPE_MASK;
        	        }
                    global_sio_handle.pkt_state      = PKT_GATHER;                    
	                *(global_sio_handle.write_ptr++) = ch;
    	            global_sio_handle.data_size      += 1;
                }
                break;
            case PKT_GATHER:
                if(HDLC_FLAG == ch)
                {                    
                    global_sio_handle.pkt_state      =  PKT_RECV;                    
                }
                else 
                {
                	if (HDLC_ESCAPE == ch)
                	{                    
                   		while(!SIO_RX_READY_E(SIO_GET_RX_STATUS(global_uart_base)));
             
                   		ch = SIO_GET_CHAR(global_uart_base);
             
                   		ch = ch ^ HDLC_ESCAPE_MASK;
                	}
                    *(global_sio_handle.write_ptr++) = ch;
                    global_sio_handle.data_size      += 1;
                }
                break;
            }
             
         }
     }
}

void UartConfig (void)
{
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E) 
    /*Purpose:to select uart1 clock source 26M*/
    REG_OR(APB_CLK_CFG1, CLK_UART1_SEL_MCU);
#endif
    /*Purpose:enable the uart0 and uart1 clock*/
    //REG_WRITE(APB_EB0, UART0_EB | UART1_EB);

    /*Purpose:enable the uart0 and uart1 */
    REG_WRITE(APB_EB0_SET, UART0_EB | UART1_EB);

    /*Init Uart */
    sio_hw_init ( ARM_UART0_Base);
    sio_hw_init ( ARM_UART1_Base);
}

void sio_hw_init( unsigned int uart_base)
{    
  /*Purpose:Disable Interrupt */
  REG_WRITE(uart_base + (unsigned int)ARM_UART_IEN, 0);
  
  /*Purpose:Set baud rate  */
  REG_WRITE(uart_base + (unsigned int)ARM_UART_CLKD0, LWORD(BAUD_115200_26M));
  
  /*Purpose:Set port for 8 bit, one stop, no parity  */
  REG_WRITE(uart_base + (unsigned int)ARM_UART_CTL0, (unsigned int)(UARTCTL_BL8BITS | UARTCTL_SL1BITS));
}

void sio_hw_open(unsigned int uart_base)
{    
  /*Purpose:Disable Interrupt */
  REG_WRITE(uart_base + (unsigned int)ARM_UART_IEN, 0);
  
   /*Purpose:Set baud rate  */
  REG_WRITE(uart_base + (unsigned int)ARM_UART_CLKD0, LWORD(global_sio_handle.baud_rate));
  
  /*Purpose:Set port for 8 bit, one stop, no parity  */
  REG_WRITE(uart_base + (unsigned int)ARM_UART_CTL0, (unsigned int)(UARTCTL_BL8BITS | UARTCTL_SL1BITS));
  
  /* Wait for external inputting */
  sio_rx_handle();
}

/******************************************************************************/
// Description: clear uart0 and uart1 FIFO
// Parameter:
// NONE
// Return:
// NONE
/******************************************************************************/
void
ClearUartFifo (void)
{
    int             ch;

    /*Purpose:To get UART FIFO status,SIO_GET_RX_STATUS (uart_base) is same as
      REG_READ(global_uart_base+ARM_UART_STS1),
      two address may be read,either 0x8300_000C for UART0 or 0x8400_000C
      for UART1
     */
    while (SIO_RX_READY_E (SIO_GET_RX_STATUS (ARM_UART1_Base))) {

    	/*Purpose:To get a char from UART FIFO status,SIO_GET_CHAR (uart_base) is same as
	      REG_READ(global_uart_base+ARM_UART_RXD),
	      two address may be read,either 0x8300_0004 for UART0 or 0x8400_0004
	      for UART1
	     */
        ch = SIO_GET_CHAR (ARM_UART1_Base);

    }

    /*Purpose:To get UART FIFO status,SIO_GET_RX_STATUS (uart_base) is same as
      REG_READ(global_uart_base+ARM_UART_STS1),
      two address may be read,either 0x8300_000C for UART0 or 0x8400_000C
      for UART1
     */
    while (SIO_RX_READY_E (SIO_GET_RX_STATUS (ARM_UART0_Base))) {

	    /*Purpose:To get a char from UART FIFO status,SIO_GET_CHAR (uart_base) is same as
	      REG_READ(global_uart_base+ARM_UART_RXD),
	      two address may be read,either 0x8300_0004 for UART0 or 0x8400_0004
	      for UART1
	     */
        ch = SIO_GET_CHAR (ARM_UART0_Base);
    }
}

static void
UpdateFlagRxCount (unsigned int uart_base, uint32 * count)
{
    int ch;
    if (SIO_RX_READY_E (SIO_GET_RX_STATUS (uart_base))) {
        ch = SIO_GET_CHAR (uart_base);
        if (HDLC_FLAG == ch) {
            (*count)++;
        }
    }
}

/******************************************************************************/
// Description: uart send heart beat characte to AP
// Parameter:
// None
// Return:
// None
/******************************************************************************/
static void _UartSendHeartBeat(unsigned int uartRegBase)
{
    /*Purpose:To  check if tx port is ready,SIO_TX_READY_E (uart_base) is same as
      REG_READ(global_uart_base+ARM_UART_STS1) >> 8,
      two address may be read,either 0x8300_000C for UART0 or 0x8400_000C
      for UART1
     */
    while (SIO_TX_READY_E (SIO_GET_TX_STATUS (uartRegBase)))
      {
        //delay(10);
        ;
      }

	/*Purpose:To send a char to UART FIFO status,SIO_PUT_CHAR (uart_base) is same as
      REG_WRITE(global_uart_base+ARM_UART_TXD),
      two address may be read,either 0x8300_0000 for UART0 or 0x8400_0000
      for UART1
    */
    SIO_PUT_CHAR (uartRegBase, HEART_BEAT_CHAR);
}

static void UartSendHeartBeat(unsigned int delayTime)
{
	static unsigned int sendHeartBeatBitMask = 0;
	unsigned int times;
	times = delayTime / 20;
	if(times >= 31) //only support 640ms when uart identify
	{
		return ;
	}
	if((sendHeartBeatBitMask & (1 << times)) == 0)
	{
		_UartSendHeartBeat(ARM_UART0_Base);
		_UartSendHeartBeat(ARM_UART1_Base);
		sendHeartBeatBitMask |= (1 << times);
	}
}

unsigned char UartIdentify(void)
{
    uint32 uart0_count, uart1_count;
    uint32 boot_mode = NOR_BOOT_MODE;
    uint32 old_tick, tick_count;

#if defined(PLATFORM_UWS6121E)
    return FALSE;
#else
    uart0_count = 0;
    uart1_count = 0;
    old_tick = fdl_GetTickCount ();

    UartSendHeartBeat(0);

    //Wait for at least three continuous 0x7e until timeout
    while (1)
    {
        UpdateFlagRxCount(ARM_UART1_Base,&uart1_count);
        UpdateFlagRxCount(ARM_UART0_Base,&uart0_count);

        //If we get 0x7E from Uart1,goto Uart1 Boot
        if (uart1_count >= UART_IDENTIFY_COUNT)
        {
            boot_mode = UART_BOOT_MODE;
            global_uart_base = ARM_UART1_Base;

            /*Purpose:Tell fdl Uart1 Boot*/
            REG32(APB_MISC_CTL2) &= ~HWRST1_REG_MSK;	
            REG32(APB_MISC_CTL2) |= (BOOT_FLAG_UART1 << HWRST1_REG_SHIFT);
            break;
        }

        //If we get 0x7E from Uart0,goto Uart0 Boot
        if (uart0_count >= UART_IDENTIFY_COUNT)
        {
            boot_mode = UART_BOOT_MODE;
            global_uart_base = ARM_UART0_Base;

            /*Purpose:Tell fdl Uart0 Boot*/
            REG32(APB_MISC_CTL2) &= ~HWRST1_REG_MSK;	
            REG32(APB_MISC_CTL2) |= (BOOT_FLAG_UART0 << HWRST1_REG_SHIFT);
            break;
        }

        /* If Uart pooling timeout,goto Nor Boot */
        tick_count = fdl_GetTickCount () - old_tick;

        if ( tick_count  > UART_TIMEOUT)
        {
                break;
        }

        UartSendHeartBeat(tick_count);
    }
    return (UART_BOOT_MODE==boot_mode);
#endif
}

void
UartDownload (void)
{
#if !defined(PLATFORM_UWS6121E)
    /*Initialize the global variable    */
    rep_ack_ptr = (char *) rep_ack_array;
    recv_data_buffer_ptr = (char *) recv_data_buffer_array;

    FDL_memset (&global_sio_handle, 0, sizeof (global_sio_handle));

    sio_open ();

    ((pkt_header_ptr) rep_ack_ptr)->pkt_type = EndianConv_16(BSL_REP_VER);
    ((pkt_header_ptr) rep_ack_ptr)->pkt_size = EndianConv_16(get_ver_len ());
    FDL_memcpy (rep_ack_ptr + DATA_ADDR, (void *)&version_string, get_ver_len ());

    sio_write (rep_ack_ptr, get_ver_len () + (unsigned int)PKT_HEADER_SIZE + (unsigned int)PKT_CRC_SIZE);

    //7E 00 81 00 22 53 70 CRC 7E
    //sio_write((char *)rep_ack, 2 + PKT_HEADER_SIZE + PKT_CRC_SIZE);

    process_packets ();
#endif
}

void
process_packets ()
{
    /*
    * length of th ereceive packet
    */
    int             pkt_len;
    ret_status      ret_code;
    ret_status      repeat_status;
    cmd_pkt_type    pkt_type;
    char           *pkt_buf;

    repeat_status = BSL_PHONE_REPEAT_CONTINUE;
    while (repeat_status == BSL_PHONE_REPEAT_CONTINUE)
    {

        FDL_memset(recv_data_buffer_array, 0, 0x400);
        /*
        * receive packet
        */
        ret_code = sio_read (&pkt_buf, &pkt_len);

        if (ret_code != BSL_PHONE_SUCCEED)
        {
            /*
            * if check result failed
            */
            send_ack (ret_code);
            FDL_memset (&global_sys_stage, 0, sizeof (global_sys_stage));

            continue;
        }

        pkt_type = (cmd_pkt_type)EndianConv_16((cmd_pkt_type) (*(short *) pkt_buf));

        proc_tab_t[pkt_type] (pkt_buf, pkt_len);

    }

    return;
}

static void
send_ack (cmd_pkt_type pkt_type)
{
    ((pkt_header_ptr) rep_ack_ptr)->pkt_type = EndianConv_16(pkt_type);
    ((pkt_header_ptr) rep_ack_ptr)->pkt_size = 0;
    sio_write (rep_ack_ptr, REP_PKT_SIZE + PKT_CRC_SIZE);

}

static void
sys_connect (char *pkt_buf, int pkt_len)
{
    if (SYS_STAGE_CONNECTED != global_sys_stage.data.stage)
    {
        send_ack (BSL_REP_ACK);
        global_sys_stage.data.stage = SYS_STAGE_CONNECTED;
    }

    FDL_memset (&global_sys_stage, 0, sizeof (global_sys_stage));
}

static void
reg_access (char *pkt_buf, int pkt_len)
{
    unsigned long reg_addr;
    unsigned long value;

    reg_addr = *(unsigned long *) &pkt_buf[(int) PKT_HEADER_SIZE];
    value = *(unsigned long *) &pkt_buf[(unsigned int) PKT_HEADER_SIZE + (unsigned int)sizeof (unsigned long)];

    *(unsigned long *) reg_addr = value;

    send_ack (BSL_REP_ACK);
}

static void
data_start (char *pkt_buf, int pkt_len)
{
    unsigned int   start_addr;
    unsigned int   file_size;

    start_addr = EndianConv_32(*(unsigned int *) &pkt_buf[(unsigned int) PKT_HEADER_SIZE]);
    file_size = EndianConv_32(*(unsigned int *) &pkt_buf[(unsigned int) PKT_HEADER_SIZE + (unsigned int)sizeof (unsigned long)]);
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E) 
    start_addr = start_addr - 0x30000000;
#endif
    global_sys_stage.data.start_addr = start_addr;
    global_sys_stage.data.write_addr = global_sys_stage.data.start_addr;
    global_sys_stage.data.file_size = file_size;
    //global_sys_stage.data .stage       = SYS_STAGE_START;

    send_ack (BSL_REP_ACK);
}

static void
data_midst (char *pkt_buf, int pkt_len)
{
    unsigned short data_len;

    // analyse the packet
    data_len = EndianConv_16(((pkt_header_ptr) pkt_buf)->pkt_size);

    //copy data to destination address
    FDL_memcpy ((void *) global_sys_stage.data.write_addr, &pkt_buf[DATA_ADDR], data_len);

    global_sys_stage.data.write_addr = global_sys_stage.data.write_addr + data_len;
    global_sys_stage.data.recv_size = global_sys_stage.data.recv_size + data_len;
    //global_sys_stage.data .stage      = SYS_STAGE_GATHER;

    send_ack (BSL_REP_ACK);

    return;
}

static void
data_end (char *pkt_buf, int pkt_len)
{
#ifdef SECURE_BOOT_SUPPORT
    uint8	*memAddr = (uint8 *)global_sys_stage.data .start_addr;
    uint8 	*fdl_vlr_ptr = NULL;
    fdl_vlr_ptr = memAddr + global_sys_stage.data.file_size;
    fdl_vlr_ptr = fdl_vlr_ptr - 512;
    
    
    fdl_file_size = global_sys_stage.data.file_size;
#endif
    if (global_sys_stage.data.recv_size == global_sys_stage.data.file_size)
    {
        //global_sys_stage.data .stage      = SYS_STAGE_END;
        global_sys_stage.data_verify = BSL_PHONE_SUCCEED;
#ifdef SECURE_BOOT_SUPPORT	
        if (0 == download_secure_check((uint8 *)memAddr, (uint8 *)fdl_vlr_ptr))			
        {
            loop_var = 1;
            send_ack(BSL_REP_SECURITY_VERIFICATION_FAIL);
            while(loop_var == 1)
            {
                ; //Do Nothing
            }
        }
#endif
        send_ack (BSL_REP_ACK);
    }
    else
    {
        global_sys_stage.data.stage = SYS_STAGE_ERROR;
        send_ack (BSL_REP_VERIFY_ERROR);
    }

    return;
}

static void
data_exec (char *pkt_buf, int pkt_len)
{
    send_ack (BSL_REP_ACK);

  MMU_DisableIDCM();

  REG32(AHB_SYS_CTL6) |= REMAP;//added it to remap in bootloader before jumping to fdl1
  
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E) 
  JumpToTarget(global_sys_stage.data.start_addr);
#else
  JumpToTarget(global_sys_stage.data.start_addr + 0x30000000);
#endif

    return;
}

#ifdef __cplusplus
    }
#endif

