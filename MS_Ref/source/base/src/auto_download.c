/******************************************************************************
 ** File Name:      auto_download.c                                           *
 ** Author:         Haifeng.Yang                                              *
 ** DATE:           2010.08.01                                                *
 ** Copyright:      2010 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2010.08.01   Haifeng.Yang          Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "auto_download.h"
#include "cmd_def.h"
#include "sc_reg.h"
#include "driver_export.h"
#include "threadx_os.h"
#include "threadx_assert.h"
#include "tb_hal.h"
#include "fiq_drv.h"
#include "ref_outport.h"
#include "chip.h"
#include "usb_drv.h"
#include "ucom_api.h"
//#include "wdg_drvapi.h"
#include "chip_internal.h"/*lint -esym(766, chip_internal.h) */
#include "syscnt_drvapi.h"/*lint -esym(766, syscnt_drvapi.h) */
#include "gpio_prod_api.h"
#include "ldo_drvapi.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
typedef struct 
{  
   int        sio_state;
   int        pkt_state;
   int        baud_rate;   
   int        buf_size;
   int        data_size;
   char       *buf_ptr;
   char       *write_ptr;
   char       *read_ptr;
}SIO_HANDLE_T, *SIO_HANDLE_PTR_T;

typedef enum
{
    PKT_NONE = 0,
    PKT_HEAD,
    PKT_GATHER,
    PKT_RECV,
    PKT_ERROR
} PKT_FLAG_E;

typedef enum
{
    SYS_STAGE_NONE,
    SYS_STAGE_CONNECTED,
    SYS_STAGE_START,
    SYS_STAGE_GATHER,
    SYS_STAGE_END,
    SYS_STAGE_ERROR
}DOWN_FLAG_E;

typedef struct 
{
    DOWN_FLAG_E      stage;
    unsigned long    file_size;
    unsigned long    recv_size;
    unsigned long    start_addr;
    unsigned long    write_addr;
}DATA_DOWNLOAD_T, *DATA_DOWNLOAD_PTR_T;

typedef struct 
{
    ret_status       data_verify;
    DATA_DOWNLOAD_T  data;
}SYS_STAGE_T, *SYS_STAGE_PTR_T;

#define AUTODL_RETRY_TIMES 			10
#define USB_RECV_LIMIT  			0x400 

#define CRC_16_POLYNOMIAL       0x1021
#define CRC_16_L_POLYNOMIAL     0x8000
#define CRC_16_L_SEED           0x80
#define HDLC_FLAG               0x7E
#define HDLC_ESCAPE             0x7D
#define HDLC_ESCAPE_MASK        0x20

#ifdef PLATFORM_SC8501C 
#define GR_HWRST                0x8B0000B8  
#endif

#ifdef PLATFORM_SC6530
#define GR_HWRST                0x8B000228  
#endif

#ifdef PLATFORM_SC6531EFM
#define GR_HWRST                0x8B000228  
#endif

#ifdef PLATFORM_UWS6121E
#define GR_HWRST                0x8B000228  
#endif


#ifdef PLATFORM_SC6531
#define GR_HWRST                0x8B000228  
#endif



#define NOR_FDL_SIZE            (40*1024)
/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/
LOCAL void _AutoDL_sys_connect(char *pkt_buf, int pkt_len);
LOCAL void _AutoDL_data_start(char *pkt_buf, int pkt_len);
LOCAL void _AutoDL_data_midst(char *pkt_buf, int pkt_len);
LOCAL void _AutoDL_data_end(char *pkt_buf, int pkt_len);
LOCAL void _AutoDL_data_exec(char *pkt_buf, int pkt_len);

LOCAL uint16 _crc_16_l_calc(uint8 const *buffer, uint32 len);
/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/
typedef void (*pkt_proc_fun)(char*, int);
LOCAL const pkt_proc_fun packet_proc_tab[] = 
{    
    _AutoDL_sys_connect, /*BSL_CMD_CONNECT,*/ 
    _AutoDL_data_start,  /*BSL_CMD_START_DATA,*/
    _AutoDL_data_midst,  /*BSL_CMD_MIDST_DATA,*/
    _AutoDL_data_end,    /*BSL_CMD_END_DATA,*/  
    _AutoDL_data_exec, 	/*BSL_CMD_EXEC_DATA,*/ 
};

LOCAL uint8 *rep_ack_array=NULL;
LOCAL uint8 *recv_data_buffer_array=NULL; 
LOCAL const uint8  version_string[] =  {"SPRD_AUTODL"};
LOCAL SYS_STAGE_T  sys_stage;
LOCAL SIO_HANDLE_T s_sio_handle; 
LOCAL BOOLEAN s_is_download_mode = SCI_FALSE;


/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/
 
#pragma arm 
#pragma arm section code= "AUTODL_IRAM_RW"
extern void remap_and_reboot(unsigned long boot_addr);
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
void copy_norfdl_bin(unsigned long addr)
{
    remap_and_reboot(addr);
}
#else
void copy_norfdl_bin(unsigned long addr)
{
    uint32 length = 0;
    uint8* dest = (uint8*)0x04000000;
    uint8* src = (uint8*)addr;
    while(length < (40*1024))
    {
        *(dest++) = *(src++);
        length++;
    }
    remap_and_reboot(addr);
}
#endif
#pragma arm section code
#pragma thumb 


LOCAL unsigned short EndianConv_16 (unsigned short value)
{
#ifdef CHIP_ENDIAN_LITTLE
    return (value >> 8 | value << 8);
#else
    return value;
#endif
}

LOCAL unsigned int EndianConv_32 (unsigned int value)
{
#ifdef CHIP_ENDIAN_LITTLE
    unsigned int nTmp = (value >> 24 | value << 24);

    nTmp |= ( (value >> 8) & 0x0000FF00);
    nTmp |= ( (value << 8) & 0x00FF0000);
    return nTmp;
#else
    return value;
#endif
}
/******************************************************************************/
// Description: 
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void _AutoDL_Delay(uint32 ms)
{
	uint32 end = SCI_GetTickCount() + ms;
	
	while(SCI_GetTickCount() < end){;}
}
/******************************************************************************/
// Description: 
// Dependence: 
// Author:        
// Note:
/******************************************************************************/
LOCAL uint16 _crc_16_l_calc(uint8 const *buffer, uint32 len)
{
    unsigned int i;
    unsigned short crc = 0;
    
    while (len--!=0)
    {
        for(i = CRC_16_L_SEED; i !=0 ; i = i>>1)
        {
           if((crc & CRC_16_L_POLYNOMIAL) !=0 )
           {
               crc = crc << 1 ;
               crc = crc ^ CRC_16_POLYNOMIAL;
           }
           else
           {
               crc = crc << 1 ;
           } 
        
           if(((unsigned char)*buffer & i) != 0 )
           {
              crc = crc ^ CRC_16_POLYNOMIAL;
           }
        }
        buffer++;
    }
    
    return (crc);
}
/******************************************************************************/
// Description:   WRITE CHARATERS TO USB/UART PORT
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL int32 _AutoDL_Write(char *write_buf,  int write_len)
{
    unsigned short crc;
    
    crc = _crc_16_l_calc((uint8*)write_buf, write_len - 2);
    *(write_buf + write_len - 2) = (crc >> 8) & 0x0FF;
    *(write_buf + write_len - 1) = crc & 0x0FF;
    
    OS_DebugPutChar(HDLC_FLAG);
    while (write_len--)
    {
  	    OS_DebugPutChar(*write_buf++);
    }
    OS_DebugPutChar(HDLC_FLAG);
    OS_DumpFlush(); 

	return 0;
}

/******************************************************************************/
// Description:   READ CHARATERS FROM USB/UART PORT
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 _AutoDL_Read(char **read_buf, int *read_len)
{
	int ch;
	int crc;

	s_sio_handle.buf_ptr   = (char*)recv_data_buffer_array;
    s_sio_handle.write_ptr = s_sio_handle.buf_ptr ;
    s_sio_handle.data_size = 0;
    s_sio_handle.pkt_state = PKT_NONE;

    /* wait for that Now have received a complete packet */
    while(s_sio_handle.pkt_state != PKT_RECV )
    {   
        ch = OS_DebugGetChar();
        
        switch (s_sio_handle.pkt_state )
        {
        case PKT_NONE:
            if(HDLC_FLAG == ch)
            {
                s_sio_handle.pkt_state      = PKT_HEAD;
            }
            break;
        case PKT_HEAD:
            if(HDLC_FLAG != ch)
            {
                if (HDLC_ESCAPE == ch)
                {                    
                    ch = OS_DebugGetChar();
         
                    ch = ch ^ HDLC_ESCAPE_MASK;
                }
                s_sio_handle.pkt_state      = PKT_GATHER;                    
                *(s_sio_handle.write_ptr++) = ch;
                s_sio_handle.data_size      += 1;
            }
            break;
        case PKT_GATHER:
            if(HDLC_FLAG == ch)
            {                    
                s_sio_handle.pkt_state      =  PKT_RECV;                    
            }
            else 
            {
                if (HDLC_ESCAPE == ch)
                {                    
                    ch = OS_DebugGetChar();
         
                       ch = ch ^ HDLC_ESCAPE_MASK;
                }
                *(s_sio_handle.write_ptr++)  = ch;
                s_sio_handle.data_size      += 1;
            }
            break;
        default:
            break;
        }
    }	

	crc = _crc_16_l_calc((uint8*)s_sio_handle.buf_ptr, s_sio_handle.data_size); 
   
    if (0 != crc)
    {
        return BSL_REP_VERIFY_ERROR;
    }
    else
    {
        *read_buf = s_sio_handle.buf_ptr;
        *read_len = s_sio_handle.data_size;
        return BSL_PHONE_SUCCEED;
    }	

}

/******************************************************************************/
// Description:   Send message to host-pc
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void  _AutoDL_SendAck(cmd_pkt_type pkt_type)
{ 
    ((pkt_header_ptr)rep_ack_array)->pkt_type = EndianConv_16(pkt_type);
    ((pkt_header_ptr)rep_ack_array)->pkt_size = 0;
    _AutoDL_Write((char*)rep_ack_array, REP_PKT_SIZE + PKT_CRC_SIZE);
}
/******************************************************************************/
// Description:   The handler of BSL_CMD_CONNECT command
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void _AutoDL_sys_connect(char *pkt_buf, int pkt_len)
{
     if(SYS_STAGE_CONNECTED != sys_stage.data.stage)
     {
        _AutoDL_SendAck(BSL_REP_ACK);
        sys_stage.data.stage = SYS_STAGE_CONNECTED;
     }
     
     memset(&sys_stage, 0, sizeof(sys_stage));   /*lint  !e718 -e718*/
}

/******************************************************************************/
// Description:   The handler of BSL_CMD_START_DATA command
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void _AutoDL_data_start(char *pkt_buf, int pkt_len)
{
    unsigned long  start_addr = 0;
    unsigned long  file_size = 0;    
#ifdef NANDBOOT_SUPPORT
    start_addr   = *(unsigned long *)&pkt_buf[(int)PKT_HEADER_SIZE];
    file_size    = *(unsigned long *)&pkt_buf[(int)PKT_HEADER_SIZE + sizeof(unsigned long)];
    
    sys_stage.data.start_addr  = EndianConv_32(start_addr);
    sys_stage.data.write_addr  = sys_stage.data.start_addr;
   
    (*(volatile uint32 *)(0xA090021C) &= (uint32)(0xFFFFFFFE)); //remap IRAM->0x40000000;     
	   
#else
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    unsigned long *data = (unsigned long *) ((char *)pkt_buf+4);
    start_addr = EndianConv_32 (*data);
    file_size = * (data + 1);
#else
    start_addr = (unsigned long*)SCI_ALLOC(NOR_FDL_SIZE);   
    if(start_addr == NULL)
    {
        SCI_TRACE_LOW("_AutoDL_data_start alloc memory fail!");
        return;
    }
    file_size    = *(unsigned long *)&pkt_buf[(int)PKT_HEADER_SIZE + sizeof(unsigned long)];
#endif

    //sys_stage.data.start_addr  = EndianConv_32(start_addr);
    //sys_stage.data.write_addr  = sys_stage.data.start_addr ;    
    sys_stage.data.start_addr = start_addr;    
    sys_stage.data.write_addr = start_addr; 	
#endif    

    sys_stage.data.file_size   = EndianConv_32(file_size);           
    _AutoDL_SendAck(BSL_REP_ACK);
}

/******************************************************************************/
// Description:   The handler of BSL_CMD_MIDST_DATA command
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void _AutoDL_data_midst(char *pkt_buf, int pkt_len)
{
    unsigned short     data_len;
    
    // analyse the packet    
    data_len     = EndianConv_16(((pkt_header_ptr)pkt_buf)->pkt_size);

    //copy data to destination address
    memcpy((void *)sys_stage.data.write_addr, &pkt_buf[DATA_ADDR ], data_len);

    sys_stage.data.write_addr = sys_stage.data.write_addr + data_len;
    sys_stage.data.recv_size  = sys_stage.data.recv_size + data_len;
	_AutoDL_SendAck(BSL_REP_ACK);
}

/******************************************************************************/
// Description:   The handler of BSL_CMD_END_DATA command
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL void _AutoDL_data_end(char *pkt_buf, int pkt_len)
{
    if(sys_stage.data.recv_size == sys_stage.data.file_size )
    {
        sys_stage.data_verify      = BSL_PHONE_SUCCEED;
        _AutoDL_SendAck(BSL_REP_ACK); 
    }
    else
    { 
        sys_stage.data.stage      = SYS_STAGE_ERROR;
        _AutoDL_SendAck(BSL_REP_VERIFY_ERROR);           
    }
}

/******************************************************************************/
// Description:   The handler of BSL_CMD_EXEC_DATA command
// Dependence: 
// Author:        Haifeng.Yang
// Note:          This function will never return.
/******************************************************************************/
LOCAL void _AutoDL_data_exec(char *pkt_buf, int pkt_len)
{
#ifdef NANDBOOT_SUPPORT
	typedef void (*BOOT_FDL_F)(void);
	BOOT_FDL_F boot_fdl = (BOOT_FDL_F)(sys_stage.data.start_addr);		
    _AutoDL_SendAck(BSL_REP_ACK);
    //_AutoDL_Delay(1);   
    //MMU_Disable();
	//MAKE autodownload flag	    
    *(volatile uint32 *)GR_HWRST = 0x8A00;      //nor_fdl:#define BOOT_FLAG_AUTODOWNLOAD  (0x8A)    
   	boot_fdl();   	
#else
    _AutoDL_SendAck(BSL_REP_ACK);
    _AutoDL_Delay(1);       
	//MAKE autodownload flag	
    *(volatile uint32 *)GR_HWRST = 0x8A00;      //nor_fdl:#define BOOT_FLAG_AUTODOWNLOAD  (0x8A)
    
    copy_norfdl_bin(sys_stage.data.start_addr);   
#endif
}

/******************************************************************************/
// Description:   Handler packets according CMD
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
LOCAL uint32 _AutoDL_ProcessPackets(void)
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
        ret_code = _AutoDL_Read(&pkt_buf, &pkt_len);       
        if(ret_code != BSL_PHONE_SUCCEED)
        {
            // if check result failed 
            _AutoDL_SendAck(ret_code);
            //memset(&sys_stage, 0, sizeof(sys_stage));   //in order to download fdl1 succcess.
            continue;
        }
        
        pkt_type = (cmd_pkt_type)(*(short *)pkt_buf);
        pkt_type = EndianConv_16(pkt_type);/*lint !e64*/
        pkt_len  = EndianConv_16(pkt_len);	
		if(packet_proc_tab[pkt_type])
		{
        	packet_proc_tab[pkt_type](pkt_buf, pkt_len); 
		}
		else
		{
            _AutoDL_SendAck(BSL_REP_UNKNOW_CMD);
            memset(&sys_stage, 0, sizeof(sys_stage));		
		}

    }        
    
    return 0;
}

/******************************************************************************/
// Description:  Starting download
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC uint32 AutoDL_StartDownLoad(uint32 mode)
{
#ifdef AUTO_DOWNLOAD_SUPPORT
	TX_INTERRUPT_SAVE_AREA	
	
	mode = 0;	//AVOID WARNINGS	
	
    rep_ack_array = SCI_ALLOC(MAX_PKT_SIZE);
    if(rep_ack_array == NULL)
    {
        return 0;    
    }

    recv_data_buffer_array= SCI_ALLOC(USB_RECV_LIMIT); 
    if(recv_data_buffer_array == NULL)
    {
        SCI_FREE(rep_ack_array);
        return 0;    
    }
    
    GPIO_SetLcdBackLight(SCI_FALSE);
    
 	SCI_Sleep(3000);	    //Wait for HOST-PC to close AT PORT
	//stop watch-dog and hold dsp
    HAL_WDG_STOP ();	
    Syscnt_ClearTimer(); 
    
    CHIP_ActiveSMCLK(SCI_TRUE);
    TX_DISABLE_FIQ_IRQ
    
	CHIP_ResetDspInAssertMode();	
	_AutoDL_Delay(20);
	OS_DumpFlush();		     
    SCI_InitLogSwitch(SCI_FALSE); //disable trace log       
	//USB_LDO_Control(FALSE);	 //POWER DOWN USB TO DISCONNECT TO PC
	LDO_TurnOffLDO(LDO_LDO_USB);
	s_is_download_mode = SCI_TRUE;
	//UCOM_SetDloaderEndpointMap();
	_AutoDL_Delay(2000);      //WAIT FOR PC TO DISCONNECT SUCCESSFULLY

    Chip_SwitchIRAM2Arm();       //ARM can use IRAM freely now. 
    SCI_EnterAssertMode();  
    
    MMU_Disable();     
    
	for(;;)
	{	
		uint32 i;
	
		OS_DebugPutChar(HDLC_FLAG);
		//OS_DumpFlush();
		for(i = 0; i < AUTODL_RETRY_TIMES; i++)
		{
			_AutoDL_Delay(1000);
			if(OS_DebugGetChar() == HDLC_FLAG)
				break;
		}
		//HOST PC makes no respondings.
		if(i >= AUTODL_RETRY_TIMES)
		{
			_AutoDL_Delay(1000);
			continue;
		}

		//SEND VERSION STRING TO HOST PC
    	((pkt_header_ptr)rep_ack_array)->pkt_type = EndianConv_16(BSL_REP_VER);
    	((pkt_header_ptr)rep_ack_array)->pkt_size = EndianConv_16(sizeof(version_string));
    	memcpy((uint8*)rep_ack_array + DATA_ADDR, &version_string, sizeof(version_string));
    	_AutoDL_Write((char*)rep_ack_array, (sizeof(version_string) + PKT_HEADER_SIZE + PKT_CRC_SIZE));
		OS_DumpFlush();
		
		_AutoDL_ProcessPackets();
	}
#endif /*#ifdef AUTO_DOWNLOAD_SUPPORT*/

	return 0; /*lint !e527*/
}

/******************************************************************************/
// Description:  retrieve the status of auto_download.
// Dependence: 
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN AutoDL_IsDloaderMode(void)
{
    return s_is_download_mode;
}
/******************************************************************************/
// Description:  when reboot make sure whether reboot after autodownload
// Dependence: 
// Author:        Jackey.ling
// Note:
/******************************************************************************/

uint32 AutoDL_GetBootModeval = 0;

PUBLIC BOOLEAN AutoDL_GetBootMode(void)
{
    BOOLEAN bootMode = SCI_FALSE;   
    bootMode = (* (volatile uint32 *) (GR_HWRST) >> 8) & 0xff; //read HWRST reg bit[15:8]    
    if(0x8A == bootMode)  /*lint !e774 */ //refer to nor_fdl
    {
       bootMode = SCI_TRUE; 
    }
    return bootMode;
}

/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif


