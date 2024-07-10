/******************************************************************************
 ** File Name:    usb_boot.c                                                  *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         4/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "usb_boot.h"
//#include "bsl_main.h"
//#include "bsl_crc.h"
#include "DRV_usb.h"
#include "virtual_com.h"
#include "sio_drv.h"
//#include "bsl_system.h"
//#include "bsl_stdio.h"
#include "common.h"
#include "bdl_packet.h"
#include "fdl_stdio.h"

#ifdef SECURE_BOOT_SUPPORT
#include "sha1_32.h"
#endif

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/

#ifdef SECURE_BOOT_SUPPORT
static volatile uint32 loop_var=0;
static uint32 fdl_file_size = 0;
#endif

const  pkt_proc_fun_t usb_proc_tab[] = {
    /* For Line Control*/
    {/*BSL_CMD_CONNECT,*/       usb_sys_connect},
    
    /*For data download command */
    {/*BSL_CMD_START_DATA,*/    usb_data_start},
    {/*BSL_CMD_MIDST_DATA,*/    usb_data_midst},
    {/*BSL_CMD_END_DATA,*/      usb_data_end}, 
    {/*BSL_CMD_EXEC_DATA,*/     usb_data_exec},
    {/*BSL_CMD_REG_ACCESS,*/     usb_reg_access} 
};

/*--------------------------- Local Data ------------------------------------*/
/*--------------------------- Global Data -----------------------------------*/
sio_handle   global_usb_handle;
/*--------------------------- External Data ---------------------------------*/
extern sys_stage_s      global_sys_stage;
extern char 			*rep_ack_ptr;
extern char 			*recv_data_buffer_ptr;
extern unsigned int 	rep_ack_array[];
extern unsigned int 	recv_data_buffer_array[];		   //Initialize the global variable   
extern const char       version_string[];

extern USB_rx_buf_T 	buf_manager ;
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
//void usb_write (unsigned char *write_buf,  int write_len);
//int USB_EPxSendData  (char ep_id ,unsigned int * pBuf,short len);
//void JumpTo(unsigned int );
void fdl_delay(uint32 ms);
int get_ver_len(void);
BOOLEAN USB_EP0Config  (void);
BOOLEAN USB_EP2Config  (void);
BOOLEAN USB_EP3Config  (void);

#define REG_READ(reg_addr,value)			do{\
												value = *(volatile uint32 *)(reg_addr);\
											}while(0)

#define REG_WRITE(reg_addr,value)			do{\
												*(volatile uint32 *)(reg_addr) = (value);\
											}while(0)

#define REG_OR(reg_addr,or_msk)				do{\
												*(volatile uint32 *)(reg_addr) |= (or_msk);\
											}while(0)

#define REG_AND(reg_addr,and_msk)			do{\
												*(volatile uint32 *)(reg_addr) &= (and_msk);\
											}while(0)
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_boot (unsigned long ext_clk26M)
{    
    USB_DOWNLOAD_STATUS_E download_status = USB_DOWNLOAD_OK;
    char usb_download_char = 0;
	rep_ack_ptr = (char*)rep_ack_array;
	recv_data_buffer_ptr = (char*)recv_data_buffer_array;

    global_usb_handle.buf_size  = (int)   USB_RECV_LIMIT;
    global_usb_handle.buf_ptr   = recv_data_buffer_ptr;

	buf_manager.read 	= 0 ;
	buf_manager.write 	= 0 ;
	usb_init(ext_clk26M);
	
	//Waite for first 0x7e ;
    while (1)
    {
        usb_download_char = VCOM_GetChar(&download_status);
        if ( ( (HDLC_FLAG == usb_download_char) && (USB_DOWNLOAD_OK == download_status) ) || 
              (USB_DOWNLOAD_TIMEOUT == download_status) )
        {
            break;
        }
    }

    if (USB_DOWNLOAD_OK == download_status)
    {
        ((pkt_header_ptr) rep_ack_ptr)->pkt_type = EndianConv_16(BSL_REP_VER);
        ((pkt_header_ptr) rep_ack_ptr)->pkt_size = EndianConv_16(get_ver_len ());
    	FDL_memcpy(rep_ack_ptr + DATA_ADDR, &version_string, get_ver_len ());

    	usb_write(rep_ack_ptr, (get_ver_len () + PKT_HEADER_SIZE + PKT_CRC_SIZE));

    	//7E 00 81 00 22 53 70 CRC 7E
    	//usb_write((char *)rep_ack, 2 + PKT_HEADER_SIZE + PKT_CRC_SIZE);
      
    	usb_process_packets();
    }
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_process_packets(void)
{
    // length of th ereceive packet
    int          pkt_len;
    ret_status   ret_code;
    ret_status   repeat_status;
    cmd_pkt_type pkt_type;
    char         *pkt_buf;
    
    repeat_status = BSL_PHONE_REPEAT_CONTINUE;
    while(repeat_status == BSL_PHONE_REPEAT_CONTINUE)
    {
        //receive packet
        ret_code = usb_read(&pkt_buf, &pkt_len);       
        if(ret_code != BSL_PHONE_SUCCEED)
        {
            // if check result failed 
            usb_send_ack(ret_code);//virsual com
 			FDL_memset(&global_sys_stage, 0, sizeof(global_sys_stage));

            continue;
        }

        pkt_type = EndianConv_16((cmd_pkt_type)(*(short *)pkt_buf));

        usb_proc_tab[pkt_type](pkt_buf, pkt_len); 

    }        
    
    return ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void  usb_send_ack(cmd_pkt_type pkt_type)
{
    ((pkt_header_ptr) rep_ack_ptr)->pkt_type = EndianConv_16(pkt_type);
     //SCI_TraceLow("packet type: %x\n\r",pkt_type);
    ((pkt_header_ptr) rep_ack_ptr)->pkt_size = 0;
    usb_write(rep_ack_ptr, REP_PKT_SIZE + PKT_CRC_SIZE);
    
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_sys_connect(char *pkt_buf, int pkt_len)
{
     if(SYS_STAGE_CONNECTED != global_sys_stage.data.stage)
     {
        usb_send_ack(BSL_REP_ACK);
        global_sys_stage.data.stage = SYS_STAGE_CONNECTED;
     }

     FDL_memset(&global_sys_stage, 0, sizeof(global_sys_stage));
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Yi.Qiu
//	Note:           
/*****************************************************************************/
void usb_reg_access(char *pkt_buf, int pkt_len)
{    
    unsigned long  reg_addr;
    unsigned long  value;
    
    reg_addr = *(unsigned long *)&pkt_buf[(int)PKT_HEADER_SIZE];
    value = *(unsigned long *)&pkt_buf[(unsigned int)PKT_HEADER_SIZE + (unsigned int)sizeof(unsigned long)];
    
	*(unsigned long *)reg_addr = value;  
    
	usb_send_ack(BSL_REP_ACK);
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_data_start(char *pkt_buf, int pkt_len)
{    
    unsigned long  start_addr;
    unsigned long  file_size;
    
    start_addr = EndianConv_32(*(unsigned long *) &pkt_buf[(unsigned int) PKT_HEADER_SIZE]);
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    start_addr = start_addr - 0x30000000;
#endif
    file_size = EndianConv_32(*(unsigned long *) &pkt_buf[(unsigned int) PKT_HEADER_SIZE + (unsigned int)sizeof (unsigned long)]);
    global_sys_stage.data .start_addr  = start_addr;
    global_sys_stage.data .write_addr  = global_sys_stage.data .start_addr ;
    global_sys_stage.data .file_size   = file_size;
    //global_sys_stage.data .stage       = SYS_STAGE_START;    
    
	usb_send_ack(BSL_REP_ACK);
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_data_midst(char *pkt_buf, int pkt_len)
{
    unsigned short     data_len;
    
    // analyse the packet    
    data_len = EndianConv_16(((pkt_header_ptr) pkt_buf)->pkt_size);

    //copy data to destination address
    FDL_memcpy((void *)global_sys_stage.data .write_addr, &pkt_buf[DATA_ADDR ], data_len);

    global_sys_stage.data .write_addr = global_sys_stage.data .write_addr + data_len;
    global_sys_stage.data .recv_size  = global_sys_stage.data .recv_size + data_len;
    //global_sys_stage.data .stage      = SYS_STAGE_GATHER;

    usb_send_ack(BSL_REP_ACK);
    
    return;
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_data_end(char *pkt_buf, int pkt_len)
{

#ifdef SECURE_BOOT_SUPPORT
    uint8	*memAddr = (uint8 *)global_sys_stage.data .start_addr;
    uint8 	*fdl_vlr_ptr = NULL;
    fdl_vlr_ptr = memAddr + global_sys_stage.data.file_size;
    fdl_vlr_ptr = fdl_vlr_ptr - 512;
    
    
    fdl_file_size = global_sys_stage.data.file_size;
#endif
    if(global_sys_stage.data .recv_size == global_sys_stage.data .file_size )
    {
        //global_sys_stage.data .stage      = SYS_STAGE_END;         
        global_sys_stage.data_verify      = BSL_PHONE_SUCCEED;
#ifdef SECURE_BOOT_SUPPORT	
        if (0 == download_secure_check((uint8 *)memAddr, (uint8 *)fdl_vlr_ptr))			
        {
            loop_var = 1;
            usb_send_ack(BSL_REP_SECURITY_VERIFICATION_FAIL);
            while(loop_var == 1)
            {
                ; //Do Nothing
            }
        }
#endif
        usb_send_ack(BSL_REP_ACK); 
    }
    else
    { 
       global_sys_stage.data .stage      = SYS_STAGE_ERROR;
       usb_send_ack(BSL_REP_VERIFY_ERROR);    
    }
    return;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/

void usb_data_exec(char *pkt_buf, int pkt_len)
{

  usb_send_ack(BSL_REP_ACK);
  
  //Please wait for a moment ;
  fdl_delay(300);
  MMU_DisableIDCM();

  REG32(AHB_SYS_CTL6) |= REMAP;//added it to remap in bootloader before jumping to fdl1
  
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
  JumpToTarget(global_sys_stage.data.start_addr);
#else
  JumpToTarget(global_sys_stage.data.start_addr + 0x30000000);
#endif

  
  return;
} 
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/

void usb_init(unsigned long ext_clk26M)
{
  /*Purpose:Enable USB AHB CLK*/
  int i;
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
  REG_WRITE(AHB_EB0_SET, USBD_EB_SET);
  /*Purpose:Reset USB PHY Logic*/
  REG_WRITE(AHB_RST0_SET,USBD_SOFT_RST_SET);
  fdl_delay(10);//50
  REG_WRITE(AHB_RST0_CLR,USBD_SOFT_RST_CLR);
  /*Enable USB device */
  REG_OR(USB_DEV_CTRL,BIT_0);
  /*Purpose:USB_TIME_SET*/
  REG_WRITE(USB_TIME_SET, 15);
  USB_EP0Config ();
  USB_EP2Config ();
  USB_EP3Config ();
  UsbLdoEnable(1);
#else
  REG_WRITE(AHB_EB0_SET, USBD_EB);
  /*Purpose:Reset USB PHY Logic*/
  REG_WRITE(AHB_RST0_SET, USBD_SOFT_RST);
  for (i = 0; i < 30000; i++);
  REG_WRITE(AHB_RST0_CLR, USBD_SOFT_RST);

  /*Enable USB device */
  REG_OR(USB_DEV_CTRL,BIT_0);

  /*Purpose:USB_TIME_SET*/
  REG_WRITE(USB_TIME_SET, 15);

  //reset fifo and data toggle for every endpoints.
  REG_OR(USB_EPx_RESET, 15);

  USB_EP0Config ();
  USB_EP2Config ();
  USB_EP3Config ();
  /* open 1.5k pullup resister */
  REG_OR(APB_MISC_CTL0, USB_1K5_EN);
#endif
  
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
ret_status usb_read  (char **read_buf, int *read_len)
{
    int  crc;
    
    //Reset the USB receive status
    global_usb_handle.write_ptr = global_usb_handle.buf_ptr ;
	global_usb_handle.data_size = 0;
	global_usb_handle.pkt_state = PKT_NONE;

    //receive data from uart
    usb_rx_handle();    
    
    //ret_code = decode_msg(global_usb_handle.buf_ptr , global_usb_handle.data_size , read_buf,read_len);
    crc = crc_16_l_calc(global_usb_handle.buf_ptr, global_usb_handle.data_size); 
   
	if (CRC_16_L_OK != crc)
	{
		return BSL_REP_VERIFY_ERROR;
	}
	else
	{
	    *read_buf = global_usb_handle.buf_ptr;
	    *read_len = global_usb_handle.data_size;
	    return BSL_PHONE_SUCCEED;
	}
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_write (unsigned char *write_buf,  unsigned int write_len)
{
	unsigned short crc;
	int i;
	
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

    write_len++;
    for (i = write_len ; i>0; i--)
    {
    	*(write_buf + i) = *(write_buf + i -1);
    }
    
    *write_buf = HDLC_FLAG ;
    
	//Ep3 max pack size is 64 bytes .
    if ((write_len >> 6)>0){
    	for (i=0; i<(write_len >> 6); i++){
	    	USB_EPxSendData  (USB_EP3 ,(unsigned int * )(write_buf + (i<<6)),64);
    	}
    	USB_EPxSendData  (USB_EP3 ,(unsigned int * )(write_buf + (i<<6)),(write_len - (i<<6)));
    }
    else{
	    USB_EPxSendData  (USB_EP3 ,(unsigned int * )write_buf,write_len);
    } 
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
void usb_rx_handle(void)
{
    int ch;
    USB_DOWNLOAD_STATUS_E download_status = USB_DOWNLOAD_OK;
	/* wait for that Now have received a complete packet */
	while(global_usb_handle.pkt_state != PKT_RECV )
	{   
		ch = VCOM_GetChar(&download_status);
		switch (global_usb_handle.pkt_state )
		{
		case PKT_NONE:
		    if(HDLC_FLAG == ch)
		    {
		        global_usb_handle.pkt_state      = PKT_HEAD;
		    }
		    break;
		case PKT_HEAD:
		    if(HDLC_FLAG != ch)
		    {
		    	if (HDLC_ESCAPE == ch)
		    	{                    
		            ch = VCOM_GetChar(&download_status);
		 
		    		ch = ch ^ HDLC_ESCAPE_MASK;
		        }
		        global_usb_handle.pkt_state      = PKT_GATHER;                    
		        *(global_usb_handle.write_ptr++) = ch;
		        global_usb_handle.data_size      += 1;
		    }
		    break;
		case PKT_GATHER:
		    if(HDLC_FLAG == ch)
		    {                    
		        global_usb_handle.pkt_state      =  PKT_RECV;                    
		    }
		    else 
		    {
		    	if (HDLC_ESCAPE == ch)
		    	{                    
					ch = VCOM_GetChar(&download_status);
		 
		       		ch = ch ^ HDLC_ESCAPE_MASK;
		    	}
		        *(global_usb_handle.write_ptr++)  = ch;
		        global_usb_handle.data_size      += 1;
		    }
		    break;
		}
	}
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
