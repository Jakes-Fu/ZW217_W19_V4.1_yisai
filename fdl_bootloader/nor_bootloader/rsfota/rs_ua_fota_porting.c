#include "sci_types.h"
#include "rs_ua_fota_porting.h"
#include "spiflash.h"

#include "bdl_cmd_def.h"
#include "usb_boot.h"
#include "drv_usb.h"
#include "virtual_com.h"
#include "sio_drv.h"

#define RS_FOTA_RAM_BUFFER_SIZE   			0x180000
#define RS_FOTA_PORTING_RAM_BUFFER_SIZE  0X8000              // block size

#define RS_MAX_PRINT_SIZE	250

#define PROGRESS_EXTEND_PRE  (20)
#define PROGRESS_NORMAL_PRE (100 - PROGRESS_EXTEND_PRE)

#pragma arm section zidata = "ZIDATA_AREA_5"
rs_u8   rs_ram_buffer[RS_FOTA_RAM_BUFFER_SIZE];
rs_u8   rs_porting_ram_buffer[RS_FOTA_PORTING_RAM_BUFFER_SIZE];
#pragma arm section zidata 

extern void sio_trace( char* str, uint32 len );

uint32 g_romStartAddr = 0;
uint32 g_deltaPartitionAddr = 0;


/**
 * rs_ua_postProgress
 *
 *
 */
void rs_ua_postUpdateProgress(rs_u32 current , rs_u32 total)
{

	rs_u32 present = current;
	if(total == PROGRESS_EXTEND_PRE)
	{
		present = (rs_u32)(PROGRESS_NORMAL_PRE + (current* total/100));
	}
	else if(total == PROGRESS_NORMAL_PRE)
	{
		present = (rs_u32)(current* total/100);
	}
	
	rs_trace("updating show progress, present:%d, current:%d, total:%d \r\n", present , current, total);
}

/**
 * 
 * rs_ua_kick_watchdog
 */
void rs_ua_kick_watchdog(void)
{
	// do nothing
}

rs_u32 rs_ua_sdk_heap_addr()
{
	return (rs_u32)rs_ram_buffer;
}

rs_u32 rs_ua_sdk_heap_size()
{
  return RS_FOTA_RAM_BUFFER_SIZE;
}

rs_u32 rs_ua_ram_block()
{
	return (rs_u32 )rs_porting_ram_buffer;
}

rs_u32 rs_ua_ram_block_size()
{
	return RS_FOTA_PORTING_RAM_BUFFER_SIZE;
}

rs_u32 rs_ua_getStartAddr(rs_u32 addr)
{
	return g_romStartAddr;
}

rs_u32 rs_ua_setStartAddr(rs_u32 addr)
{
	 g_romStartAddr = addr;
}

rs_u32 rs_ua_setDeltaPartitionAddr(rs_u32 addr)
{
	g_deltaPartitionAddr = addr;
}

rs_u32 rs_ua_getPackageAddr()
{
	return g_deltaPartitionAddr + 2*RS_FOTA_PORTING_RAM_BUFFER_SIZE;
}

rs_u32 rs_ua_getUpdateInfo1Addr()
{
	return g_deltaPartitionAddr;
}

rs_u32 rs_ua_getUpdateInfo2Addr()
{
	return g_deltaPartitionAddr + RS_FOTA_PORTING_RAM_BUFFER_SIZE;
}

rs_u32 rs_ua_saveExtendData(rs_u8* buff,rs_u32 size, rs_u32 offset,rs_bool isFinal)
{
	return 0;
}


rs_u32 rs_ua_processExtendData()
{
	return 0;
}

rs_s32 rs_ua_flash_init()
{
	return 0;
}

rs_bool rs_ua_flash_read(rs_u8 *destination,
                       volatile rs_u8 *address,
                       rs_u32 size)
{
	uint32 ret;
	ret = FDL_FlashRead(address, destination, size);
	rs_trace("exit rs_ua_flash_read address = 0x%X, size = %d, ret=%d .\r\n", (rs_u32)address,  size,  ret);

	if (ret == size)
		return rs_true;
	else
		return rs_false;
}

rs_bool rs_ua_flash_write(volatile rs_u8 *address,
                       rs_u8 *source,
                       rs_u32 size)
{
	uint32 ret;
	ret = FDL_FlashWriteArea(address, source, size);
	rs_trace("exit rs_ua_flash_write address = 0x%X, size = %d, ret=%d .\r\n", (rs_u32)address,  size,  ret);

	if (ret == size)
		return rs_true;
	else
		return rs_false;
}

rs_bool rs_ua_flash_erase(volatile rs_u8 *address,
                        rs_u32 size)
{
	uint32 ret;
  	ret = FDL_FlashErase(address,FDL_FlashCheckSectorSize(1));
  	rs_trace("exit rs_ua_flash_erase address = 0x%X, size = %d \r\n", (rs_u32)address,  size);

	/*
	if (ret == size)
		return rs_true;
	else
		return rs_false;
		*/

	return rs_true;
}

static int32 s_rs_bl_debug_switch = 1;//bootloader trace is open by default
static int32 s_rs_debug_port_switch = 1;//0:usb, other:uart
extern char 			*rep_ack_ptr;
extern char 			*recv_data_buffer_ptr;
extern unsigned int 	rep_ack_array[];
extern unsigned int 	recv_data_buffer_array[];//Initialize the global variable   
extern sio_handle   global_usb_handle;
extern USB_rx_buf_T    buf_manager;

void rs_usb_init(void)
{    
	USB_DOWNLOAD_STATUS_E download_status = USB_DOWNLOAD_OK;
	char usb_download_char = 0;

	rep_ack_ptr = (char*)rep_ack_array;
	//recv_data_buffer_ptr = (char*)recv_data_buffer_array;

	global_usb_handle.buf_size	= (int)   USB_RECV_LIMIT;
	global_usb_handle.buf_ptr	= recv_data_buffer_ptr;

	buf_manager.read	= 0 ;
	buf_manager.write	= 0 ;
	usb_init(1);

#if 0//for test
	{			
	    uint32 rs_idx=0;
	    while(1)
	    {
	        for(rs_idx=0;rs_idx<10000;rs_idx++)
	        {
	            rs_trace("rs-abcd-1234 = %04d\r\n", rs_idx);
	        }
	    }        
	}
#endif
}

void rs_usb_write (unsigned char *write_buf,  unsigned int write_len)
{
	unsigned short crc;
	int i;
#if 0
	//USB_EPxSendData  (USB_EP3 ,(unsigned int * )write_buf,write_len);//
	USB_EPxSendData_EX((unsigned int * )write_buf,write_len);
#else	
	//Ep3 max pack size is 64 bytes .
    if ((write_len >> 6)>0)
	{
    	for (i=0; i<(write_len >> 6); i++)
		{
			USB_EPxSendData_EX((unsigned int * )(write_buf + (i<<6)),64);
    	}
		USB_EPxSendData_EX((unsigned int * )(write_buf + (i<<6)),(write_len - (i<<6)));
    }
    else
	{
	    USB_EPxSendData_EX((unsigned int * )write_buf,write_len);
    } 
#endif
}

rs_u32 rs_trace(const char *format, ...)
{
	if(s_rs_bl_debug_switch)
	{

		USB_DOWNLOAD_STATUS_E download_status = USB_DOWNLOAD_OK;
		int32 len=0, idx=0;
		va_list ap;
		va_start (ap, format);

		FDL_memset(rep_ack_ptr, 0, MAX_PKT_SIZE_B);
		len = 0;
		len = vsnprintf(rep_ack_ptr,MAX_PKT_SIZE_B, format, ap);
		
		if((len+2) < MAX_PKT_SIZE_B)
		{
			rep_ack_ptr[len] = '\r';
			rep_ack_ptr[len+1] = '\n';
			len += 2;
		}
		//USB 	
		if(s_rs_debug_port_switch == 0)
		{
			VCOM_GetChar_EX(&download_status);
			rs_usb_write(rep_ack_ptr, len); 
		}	
		else//UART
		{
			sio_trace(rep_ack_ptr, len);
		}	
		va_end (ap);	
	}	
	return 0;
}

