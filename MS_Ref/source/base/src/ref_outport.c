/******************************************************************************
 ** File Name:      ref__outport.c                                            *
 ** Author:         hanjun.liu                                                *
 ** DATE:           08/14/2002                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file realize the functions defined at **_import.h    *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 02/18/2004     hanjun.liu       Create.                                   *
 ** 2006/09/11     Daniel Luo       add certain memory dump method            *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "ms_ref_base_trc.h"
#include "sio.h"
#include "nvitem.h"
#include "nv_item_id.h"
#include "power.h"
#include "cmddef.h"
#include "task_monitor.h"
#include "fiq_drv.h"
#include "vcom_usb.h"
#include "dal_chr.h" 
#include "threadx_os.h"
#include "os_apiext.h"
#include "os_api.h"
#include "mem_dump.h" 
#include "chip.h"
#include "timer_drv.h"
#include "mem_prod.h"
#include "os_param.h" 
#include "power_manager.h"
#include "bus_monitor.h"
#include "vb_drv.h"
#include "threadx_assert.h"
#include "auto_download.h"
#ifdef CARD_LOG
#include "logsave_output.h"
#endif
#include "ref_outport.h"
#include "ref_param.h"
#include "flash.h"
#include "sci_ftl_api.h"
#include "spload_cfg.h"
#include "bsd.h"
#include "adi_hal_internal.h"
#include "cp_clkrst.h"

#if defined(PLATFORM_UWS6121E)
#include "load_modem.h"
#include "bt_abs.h"
#endif

#include "vcom_usb.h"
#include "ucom_api.h"
//#include "sim_drv.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

#ifndef WIN32 // In THUMB Target Mode
    #define   DUMP_MEM_PACKET_SIZE    (16 * 1024)
    #define   DUMP_MAX_PACKET_SIZE    (64 * 1024)
#endif  //end WIN32    

//define word endian swap
#define WORD_ENDIAN_SWAP(word) (((word & 0xFF)<<8) |((word & 0xFF00) >>8))

//define dword endian swap
#define DWORD_ENDIAN_SWAP(DWord) \
    (((DWord & 0x0FF00)<< 8)|((DWord & 0x0FF)<<24) |\
     ((DWord & 0xFF000000)>>24)|((DWord & 0xFF0000)>>8))

#define CONFIRM_POWER_DOMAIN_TURNON(_POWER_DOMAIN)                  \
    while(!(hwp_pwrctrl->_POWER_DOMAIN##_STAT & (1<<1)));           \
    if (hwp_pwrctrl->_POWER_DOMAIN##_STAT != 0x3)                   \
    {                                                               \
        hwp_pwrctrl->_POWER_DOMAIN##_CTRL = (1<<1);                 \
    }                                                               \
    while(hwp_pwrctrl->_POWER_DOMAIN##_STAT != 0x3);

#define CONFIRM_POWER_DOMAIN_TURNON_POLL(_POWER_DOMAIN)             \
    while(!(hwp_pwrctrl->_POWER_DOMAIN##_STAT & (1<<1)));           \
    if (hwp_pwrctrl->_POWER_DOMAIN##_STAT != 0x3)                   \
    {                                                               \
        hwp_pwrctrl->ZSP_PD_POLL_CLR = (1<<0);                     \
    }                                                               \
    while(hwp_pwrctrl->_POWER_DOMAIN##_STAT != 0x3);


/**---------------------------------------------------------------------------*
 **                         Global variables                                  *
 **---------------------------------------------------------------------------*/
// Here crc_data is crc code caculated for normal_info_head.These variable are used in RTOS.
#ifdef CHIP_ENDIAN_LITTLE
    LOCAL const uint8 normal_info_head[8] = {0, 0, 0, 0, // seq num and dummy word
                                                8, 0,       // size
                                                MSG_BOARD_ASSERT, NORMAL_INFO};
    const uint8 all_assert_info_mem_end[8]   = {0, 0, 0, 0, // seq num and dummy word
        8, 0,MSG_BOARD_ASSERT, DUMP_ALL_ASSERT_INFO_END};
#else

    LOCAL const uint8 normal_info_head[8] = {0, 0, 0, 0, // seq num and dummy word
                                                0, 8,       // size
                                                MSG_BOARD_ASSERT, NORMAL_INFO};
    const uint8 all_assert_info_mem_end[8]   = {0, 0, 0, 0, // seq num and dummy word
        0, 8,MSG_BOARD_ASSERT, DUMP_ALL_ASSERT_INFO_END};
#endif

uint8 dump_mem_data[8]   = {0, 0, 0, 0, // seq num and dummy word
                                           0, 0,
                                           MSG_BOARD_ASSERT, DUMP_MEM_DATA};
    
const uint8 dump_mem_end[8]    = {0, 0, 0, 0, // seq num and dummy word
                                           0, 0,
                                           MSG_BOARD_ASSERT, DUMP_MEM_END};

//LOCAL BOOLEAN s_usb_is_reconfiged = FALSE;

uint32 g_value_v = 0;
uint32 g_count_v = 0;

LOCAL uint32 s_mem_seq_num = 0;
LOCAL uint32 s_dsp_tick_count   = 0;                     //for dc isp disturb noise check 
LOCAL uint32 s_isp_cap_sof_tick   = 0;                      //for dc isp disturb noise check 

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
#ifndef WIN32
extern void WDG_TimerStop(void);
#endif
/*****************************************************************************/
// Description :    This function is used to reset MCU which used in OS.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
#ifdef WIN32
PUBLIC uint32 SCI_TraceLowly(const char * x_format, ...)
{
    return 1;
}
PUBLIC int32 VCOM_GetChar (void)
{
    return 1;
}
PUBLIC void VCOM_usb_boot(void)
{
}
PUBLIC uint32 VCOM_GetDevState (void)
{
    return 1;
}
PUBLIC BOOLEAN drvUsbIsConnected(void)
{
    return TRUE;
}
PUBLIC uint32 SIO_GetPhyPortNo (uint32 port)
{
    return 1;
}


#endif
PUBLIC void OS_ResetMCU(void)
{
#ifndef WIN32
    CHIP_ResetMCU(); 
#endif
}

 /*****************************************************************************/
// Description :    This function is used to get escaped seconds form power on.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/

PUBLIC uint32 OS_GetCurrentTime(void)
{
#ifndef WIN32
    return (TIMER_GetTickCount() / 1000);
#else
    return 0;
#endif
 }

PUBLIC uint32 OS_GetCurrentMilSec(void)/*lint -esym(765, OS_GetCurrentMilSec)*/
{
#ifndef WIN32
    // 20100704: will.jiang: x / 1000  --> x ,  x return ms, no need "/1000"
    return TIMER_GetTickCount();
#else
    return 0;
#endif
}

 /*****************************************************************************/
// Description : This function scanf a char from uart receive register at debug.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/

PUBLIC int OS_DebugGetChar(void)
{
#ifndef WIN32

        int32 retval;
        uint32 phyport;
        uint32 old_tick, new_tick, last_tick;
        uint32 count=0, value=0;

        do{
                phyport = SIO_GetPhyPortNo(COM_DEBUG);
   
#if 1
                /*if ucom is ready,use ucom port*/
                /*if ucom is not ready,then re-enumerate vcom for transfer*/

                if (/*!drvUsbIsConnected() &&*/ VCOM_DEV_NO_PRESENT == VCOM_GetDevState() 
#ifndef WIN32
                    && UCOM_GetUcomType()!= UCOM_CREATE_COM_NPI
#endif
                    )
                {
                    //SCI_TraceLow("VCOM is not present!\n");
                    if (CHGMNG_IsChargeConnect() /*&&  CHGMNG_ADP_USB == CHGMNG_IdentifyAdpType()*/)
                    {
                        VCOM_usb_boot();
                        old_tick = new_tick = SCI_GetTickCount();
                        last_tick = new_tick;

                        while ((new_tick - old_tick) < 3000) // wait 3s for enumerate
                        {
                            new_tick = SCI_GetTickCount();
                            if (last_tick != new_tick)
                            {
                                last_tick = new_tick;
                                value++;
                                g_value_v = value;
                            }

                            if (-1 != VCOM_GetChar() /*|| VCOM_DEV_PRESENT == VCOM_GetDevState()*/)
                            {
                                count++;
                                g_count_v = count;
                                //SCI_TraceLowly("VCOM is ready!\n");
                                /*如果assert的时候没有插usb线，那么phy port是uart，当usb dump过程中usb不小心被拔掉了，
                                *那么剩下还未dump出去的数据就会从uart走，而uart dump数据速度很慢，
                                *必须等uart dump过程走完再插入usb才可以枚举成功(polling模式枚举必须在while循环中调用VCOM_GetChar)
                                *这里将COM_DEBUG状态设置为关闭状态，可以缩短剩下的dump流程的时间*/
                                SIO_DumpSetPhyPortNo(COM_DEBUG,0xff);
                                break;
                            }
                        }
                        
                        if (VCOM_DEV_PRESENT == VCOM_GetDevState())
                        {
#ifndef WIN32
                            VCOM_Open(TRUE);
                            SIO_DumpSetPhyPortNo(COM_DEBUG,0xff);
#endif
                        }
                        
                    }
                }
#endif

                if(1)//(VIR_COM0 == phyport)||(VIR_COM1 == phyport)
                {
                    if(VCOM_DEV_NO_PRESENT == VCOM_GetDevState())         //SIO(UCOM or uart)
                    {
                        /*
                        *  usb is in stage of connecting, no data should be received and sent
                        *  the valid data is got from uart still
                        */
                        //VCOM_GetChar();
#ifndef WIN32
                        retval = SIO_DumpGetChar(COM_DEBUG);
#endif

                    }
                    else                                                //VCOM
                    {
                        /*
                        *  usb conneted,virtual com is open
                        *  the valid data is got from usb virtual com only
                        */
                        retval = VCOM_GetChar();

                    }
                }
                else
                {
#ifndef WIN32
                    retval = SIO_DumpGetChar(COM_DEBUG);
#endif
                }
    /* used for auto-test reset mcu */
            //got valid char from uart or usb
            if(-1 != retval)
            {
                //SCI_TraceLowly("get a char: 0x%x(%c)\n",retval,retval);
                return retval;
            }

        }while(TRUE); /*lint !e506*/
        return retval;
#else
    return 0 ;
#endif
}

 /*****************************************************************************/
// Description : This function send a char to uart or usb virtual com at assert debug
// Global resource dependence :
// Author :      weihua.wang
// Note :           
/*****************************************************************************/
PUBLIC void OS_DebugPutChar(uint8 ch)
{
#ifndef WIN32
    
    if( VCOM_DEV_READY == VCOM_GetDevState())
    {
            VCOM_PutTxChar(ch);
    }
    else
    {
        SIO_DumpPutChar(COM_DEBUG, ch);
    }
#endif
}

 /*****************************************************************************/
// Description : This function send chars to uart or usb virtual com at assert debug
// Global resource dependence :
// Author :      weihua.wang
// Note :        called in memdump only!   
/*****************************************************************************/
PUBLIC void OS_DebugPutChars(uint8* data_ptr, int32 len)
{
#ifndef WIN32
    
    if(VCOM_DEV_READY == VCOM_GetDevState())
    {
            VCOM_PutTxChars(data_ptr, len);
    }
    else
    {
        SIO_DumpPutChars(COM_DEBUG, data_ptr,len);
    }
#endif
}

/*****************************************************************************/
//  Description:    The OS_DumpPrint function is used to print string by 
//                  serial port.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
PUBLIC  void OS_DumpFlush(void)
{
#ifndef WIN32

    uint32 vcom_mode;
    
    vcom_mode = VCOM_GetDevState();
    
    if( VCOM_DEV_READY == vcom_mode)
    {
        VCOM_Flush();
    }
    else
    {
        SIO_DumpPutCharEnd(COM_DEBUG);
    }    

#endif

}

 /*****************************************************************************/
// Description : This function is used in the RTOS
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
void OS_SendRemainLogMessage(void)
{
#ifndef WIN32
    uint32 phyport;
    SIO_SendRemainLogMessage();
#if defined(MODEM_LOG)
    SIO_SendRemainCPLog();
 #endif   
 
    phyport = SIO_GetPhyPortNo(COM_DEBUG);
    
    if((VIR_COM0 != phyport)&&(VIR_COM1 != phyport))
    {
        /*
        * current debug port is by uart, so we should init usb first
        * else do null
        */
#if !defined(PLATFORM_UWS6121E) //temp_stub_sgq
       VCOM_usb_boot();
#endif
    }
#endif
}
 /*****************************************************************************/
// Description :    This function is used to set the next tick use in RTOS.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC void OS_SetNextTick(
            uint32 mil_sec,         // Time value to be set.
            void (* func_ptr)() // When timer is expired, call this function.
                              )
{
#ifndef WIN32
#if defined(PLATFORM_UWS6121E)
    TIMER_Set(OS_TIMER, mil_sec, TIMER_ONESHOT_MODE, func_ptr);
#else
    TIMER_Set(TIMER_0, mil_sec, TIMER_ONESHOT_MODE, func_ptr);
#endif
#endif
}
/*****************************************************************************/
// Description :    This function is used to get escaped ticks form power on.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC uint32 OS_GetTickCount(void)  // return the number of tick count.
{
#ifndef WIN32
#if defined(PLATFORM_UWS6121E)
	return timer_TimTickGetHwVal(OS_TIMER);
#else
    return TIMER_GetTickCount();
#endif
#else
    return 0;
#endif
}

#if defined(PLATFORM_UWS6121E)
PUBLIC uint32 OS_TimTickEnable(void)
{
#ifndef WIN32
    return timer_TimTickEnable(OS_TIMER);
#else
    return 0;
#endif
}

PUBLIC uint32 OS_TimTickIsEnabled(void)
{
#ifndef WIN32
    return timer_TimTickIsEnabled(OS_TIMER);
#else
    return 0;
#endif
}

PUBLIC uint32 OS_TimTickStop(void)
{
#ifndef WIN32
    return timer_phy_disable(OS_TIMER);
#else
    return 0;
#endif
}

PUBLIC uint32 OS_TimCurValue(void)
{
#ifndef WIN32
    return timer_TimTickGetVal(OS_TIMER);
#else
    return 0;
#endif
}

#else
PUBLIC uint32 OS_TimTickEnable(void)
{
    return 0;
}

PUBLIC uint32 OS_TimTickIsEnabled(void)
{
    return 1;
}

PUBLIC uint32 OS_TimTickStop(void)
{
    return 0;
}
#endif

/*****************************************************************************/
// Description :    This function delay some ticks .
// Global resource dependence :
// Author :         Daniel.ding
// Note :           
/*****************************************************************************/
PUBLIC uint32 OS_TickDelay(uint32 ticks)
{
#ifndef WIN32
#if !defined(PLATFORM_UWS6121E)
    volatile uint32  des_tick = TIMER_GetSystemCounterReg() + ticks;
    while (TIMER_GetSystemCounterReg() < des_tick ) ;
#else
    volatile uint32 time_1 = 0, time_2 = 0;
    volatile uint32 delay_count = 0;
    delay_count = (ticks * 16384) / 1000;
    time_1 = timer_TimRealTickGet(OS_TIMER);
    time_2 = time_1 + delay_count;
    if (time_2 < time_1)
    {
        delay_count -= ((0xFFFFFFFF - time_1) + 1);
        while(timer_TimRealTickGet(OS_TIMER) >= time_1);
        while(delay_count > timer_TimRealTickGet(OS_TIMER));
    }
    else
    {
        while(time_2 >= timer_TimRealTickGet(OS_TIMER));
    }
#endif
#endif
    return ticks;

}
/*****************************************************************************/
// Description :    This function is used to read OS parameter.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/
PUBLIC BOOLEAN OS_ReadOsParameter(
                                uint16      length,
                                uint8       *buf_ptr
                                )
{
    BOOLEAN status = FALSE;

    // Read RTOS parameter initial values from NVItem.
    status = EFS_NvitemRead(NV_OS_PARAMETER, length, buf_ptr);
    
    // If the initial values don't exist, set default value to NVItem.
    if (NVERR_NOT_EXIST != status)
    {
        status = TRUE;
    }
    return status;
}

 /*****************************************************************************/
// Description :    This function is used to write OS parameter.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/

PUBLIC void OS_WriteOsParameter(
                                uint16      length,
                                uint8       *buf_ptr
                                )
{
    EFS_NvitemWrite(NV_OS_PARAMETER, length, buf_ptr, 1);
} 

 /*****************************************************************************/
// Description :    This function is used in the RTOS.
// Global resource dependence :
// Author :         hanjun.liu
// Note :           
/*****************************************************************************/

void OS_SendTestPointRequest(uint32 param1, uint32 param2)
{
#ifndef WIN32
    TM_SendTestPointRequest(param1,param2);
#endif
}
/*****************************************************************************/
// Description :    This function is used to reset in RTOS.
// Global resource dependence : 
// Author :         hanjun.liu
// Note :
/*****************************************************************************/
PUBLIC void  OS_Reset(void)
{
    POWER_Reset();
}

#ifndef WIN32
/*****************************************************************************/
//  Description:    The XF_SavePrintTraceToMem function .
//  Global resource dependence:
//  Author:        
//  Note:
/*****************************************************************************/
PUBLIC void XF_SavePrintTraceToMem(
    char * string,
    uint32 size,
    uint32 type    
)
{
//#ifdef RELEASE_INFO  
#if 0
	if( SCI_TRUE == Get_XfileEnableEndFlag())
	{
		if(NULL != string)
		{ 
			if((size >= (XFILE_LOG_SAVE_BUF_SIZE)))
			return;

			if(0 == xf_trace_log_save_len)
			{    
			xf_trace_log_save_len = size;
			SCI_MEMSET((xf_trace_Log_buf), '\0' ,(XFILE_LOG_SAVE_BUF_SIZE));
			}
			else    
			{
			xf_trace_log_save_len += size;
			}

			if(xf_trace_log_save_len >= (XFILE_LOG_SAVE_BUF_SIZE))
			{
			xf_trace_log_save_len = size;
			SCI_MEMSET((xf_trace_Log_buf), '\0' ,(XFILE_LOG_SAVE_BUF_SIZE));
			}

			strcat((char*)(xf_trace_Log_buf), (char*)string );
		}
	}
#endif	
    return;

}
#endif

PUBLIC void  OS_ResetEntryDownload(void)
{
    /* CR:555820 2016-06-01 */
    POWER_SetMsStatus(POWER_MS_STATUS_HANDLE_POWER_OFF);

    //turn off backlight before LCD Sleep
    GPIO_SetLcdBackLight (SCI_FALSE);/*lint !e718 !e18 */

    LCD_Close();
#ifndef WIN32
    /*close wdg,????λ??????д????????????Ч????????*/
    WDG_TimerStop();
#endif
    // Change MCU mode.
    BOOT_SetResetMode (RESET_MODE);
	
    USB_Connect();
	
    CHIP_ResetMCUSoft();
}

/*****************************************************************************/
//  Description:    The OS_DumpPrint function is used to print string by 
//                  serial port.
//  Global resource dependence:
//  Author:         Richard.Yang
//  Note:
/*****************************************************************************/
PUBLIC  void OS_DumpPrint(
    char * string,      // String to be printed out
    uint32 size         // Size of string to be printed out
    )
{
#ifndef WIN32
    int32 i;
       

    OS_DebugPutChar(0x7E);

    for (i = 0; i < (int32)sizeof(normal_info_head); i++) 
    {
        OS_DebugPutChar(normal_info_head[i]);
    }
        
    while (size--)
    {
        OS_DebugPutChar(*string++);
    }

    OS_DebugPutChar(0x7E);

    return;
    
#endif
}
/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/

PUBLIC void OS_SendPacket (uint32 sendpacketmode)
{
#ifndef WIN32
    uint8 i;
    //MSG_HEAD_T all_assert_info_mem_end;
    switch(sendpacketmode)
    {
        case SEND_PACKET_END:
/*
    all_assert_info_mem_end.seq_num = 0;
    all_assert_info_mem_end.len = sizeof(MSG_HEAD_T);
    all_assert_info_mem_end.type = MSG_BOARD_ASSERT;
    all_assert_info_mem_end.subtype = DUMP_ALL_ASSERT_INFO_END;
     SIO_SendPacket((uint8*)&all_assert_info_mem_end, sizeof(MSG_HEAD_T));
*/      
        OS_DebugPutChar(0x7E);
        
        for (i = 0; i < sizeof(all_assert_info_mem_end); i++)
            {       
                OS_DebugPutChar(all_assert_info_mem_end[i]); 
            }
        
    //  OS_DebugPutChar(0x9);
        OS_DebugPutChar(0x7E);

            break;

        case SEND_PACKET_CUSTOM0:

            break;
            
        case SEND_PACKET_CUSTOM1:

            break;
            
        default :
            break;
    }    
#endif
}

#ifndef WIN32
static uint32 DumpReadReg(
                       uint32 addr // Must word aligned
                       )                  
{
    uint32    mcu_data= 0;
    uint32    mcu_addr= addr&(~0x03);
    
    if(!ANA_IS_ANA_REG(addr))
    {
        mcu_data = CHIP_REG_GET(mcu_addr);
    }
    else
    {
        mcu_data = ANA_REG_GET (mcu_addr);
    }

    return mcu_data;
}

static __inline void PutUint8Chars(uint8* dataptr, int32 len)
{
    OS_DebugPutChars(dataptr, len);
}

static __inline void PutUint8(uint8 data)
{
    if ((0x7E == data) || (0x7D == data))  
    {
        OS_DebugPutChar(0x7D);
        OS_DebugPutChar((data ^ 0x20));
    }
    else
    {
        OS_DebugPutChar(data);
    }
}

static __inline void PutUint16(uint16 data)
{
    uint8 data_low = data & 0xff;
    uint8 data_high = (data & 0xff00) >> 8;
    PutUint8(data_high);
    PutUint8(data_low);     
}

static __inline void PutUint32(uint32 data)
{
    int i;
    
    for (i = 0; i < 4; ++i) 
    {
        uint8 c = (data & 0xFF000000) >> 24;
        
        PutUint8(c);        
        
        data <<= 8;
    }
} 

/*****************************************************************************/
// Description :    This function is used to print dupm mem header info
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
LOCAL void _OS_PrintHeaderInfo(void)
{
    int32 i;
    
    for (i = 0; i < (int32)sizeof(dump_mem_data); i++)
    {
        PutUint8(dump_mem_data[i]);
    }
}

/*****************************************************************************/
// Description :    This function is used to update the seqnumber and len info when dupm memoy
// Global resource dependence :
// Author :         
// Note :           
/*****************************************************************************/
LOCAL void _OS_UpdateDumpHeader(uint32 seq_num, uint16 len)
{
    MSG_HEAD_T* data_ptr = NULL;    

    data_ptr = (MSG_HEAD_T*)(dump_mem_data);

#if defined(CHIP_ENDIAN_LITTLE) && defined(PLATFORM_SC8800H)
    data_ptr->seq_num = DWORD_ENDIAN_SWAP(seq_num);
    data_ptr->len     = WORD_ENDIAN_SWAP(len);
#else
    data_ptr->seq_num = seq_num;
    data_ptr->len     = len;
#endif    

    _OS_PrintHeaderInfo();
}


static __inline int PutThreadInfo(const TX_THREAD *thread)
{
    int i;
    
    /* dump tcb ptr, 4 bytes */
    PutUint32((uint32)thread);
    
    /* dump thread name, 32 bytes */
    for (i = 0; i < 31; ++i)
    {
        if (thread->tx_thread_name[i])
            PutUint8(thread->tx_thread_name[i]);
        else
            break;
    }
    
    for (; i < 32; ++i)
    {
        PutUint8(0);
    }
    
    return 36; /* total 36 bytes dumped */ 
}
 
static __inline void DumpThreadInfo(void)
{
   // int32         i;
    TX_THREAD   *start_thread = NULL;
    TX_THREAD   *thread = NULL;
    TX_THREAD   *prev_thread = NULL;
    int         count = 0;
    
    /* head */
    OS_DebugPutChar(0x7E);
    
    // update and print Head info.
    _OS_UpdateDumpHeader(++s_mem_seq_num, DUMP_MEM_PACKET_SIZE);        
    
    /* data */
    start_thread = (TX_THREAD *)SCI_ThreadListHeadPtr();
    if(start_thread)
    {
        count += PutThreadInfo(start_thread);
        thread = start_thread->tx_created_next;
       prev_thread = start_thread;
        while ((thread != start_thread) && (thread != NULL)  && (thread == prev_thread->tx_created_next))
        {
            prev_thread = thread;
            count += PutThreadInfo(thread);
            thread = thread->tx_created_next;
        }       
    }
       
    /* padding */
    for (; count < DUMP_MEM_PACKET_SIZE; ++count)
        PutUint8(0);
    //tail
    OS_DebugPutChar(0x7E);
}

static __inline void DumpMemSection(uint32 mem_begin_addr, uint32 mem_lenth)
{   
    uint32  i;
    uint32  num;
    uint32  packet_num;
    uint32  residual_num = 0;
    uint32  seq_num;
    
    uint8   mem_data = 0;
    uint8 *mem_ptr = (uint8 *)mem_begin_addr;
    
    if(0 == mem_lenth)
    {
        return;
    }

    packet_num = (mem_lenth + DUMP_MEM_PACKET_SIZE - 1)/ DUMP_MEM_PACKET_SIZE;
    residual_num = mem_lenth % DUMP_MEM_PACKET_SIZE;

    seq_num = s_mem_seq_num;
    // Dump memory data.
    for (num = 0; num < (packet_num - 1); num++)
    {
        OS_DebugPutChar(0x7E);
        
        // update and print Head info.
        _OS_UpdateDumpHeader(++seq_num, DUMP_MEM_PACKET_SIZE);        

        // Data.
        PutUint8Chars(mem_ptr, DUMP_MEM_PACKET_SIZE);
        mem_ptr += DUMP_MEM_PACKET_SIZE;

        OS_DebugPutChar(0x7E);
    }
    
    if (0 == residual_num)
    {
        residual_num = DUMP_MEM_PACKET_SIZE;
    }
    
    
    OS_DebugPutChar(0x7E);
        
    // update and print Head info.
    _OS_UpdateDumpHeader(++seq_num, residual_num);        
    s_mem_seq_num = seq_num ;
    
    // Data.
    for (i = 0; i < residual_num; i++)
    {
        
        mem_data = *mem_ptr;
        PutUint8(mem_data);
        mem_ptr++;
    }

    OS_DebugPutChar(0x7E);
}

static __inline uint32 DumpIRAMDsp( void )
{
#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)

    uint32 dump_count=0;
    uint32 count =0;
    uint32 mem_lenth = 0;
    uint32 mem_addr = 0;

    IRAM_AREA* iram_cfg_ptr = (IRAM_AREA*) MEM_GetIRAMDsp(&count);

    //Anyway dump iram for dsp.
    {
        //Switch all iram to ARM domain
        Chip_SwitchIRAM2Arm();

        while( count-- )
        {
            mem_addr  = iram_cfg_ptr->iram_addr;
            mem_lenth = iram_cfg_ptr->iram_length;
            
            DumpMemSection(mem_addr, mem_lenth);

            dump_count = dump_count+mem_lenth;
            iram_cfg_ptr++;
        }

        //Switch iram2 iram3 to dsp domain
        Chip_SwitchIRAMDefault();
    }
    return dump_count;    

#else 
    return 0;
#endif 
}

static __inline void DumpSharedMem(void)
{
        
    uint32  i;
    uint32  num;
    uint32  packet_num;
    uint32  residual_num = 0;
    uint32  seq_num;
    uint16  mem_data = 0;
    uint32  mem_lenth = CHIP_GetSharedMemLen();
    volatile uint16 *mem_ptr = (volatile uint16 * ) CHIP_GetSharedMemBase();
    
    if(0 == mem_lenth)
    {
        return;
    }
    
    packet_num = (mem_lenth + DUMP_MEM_PACKET_SIZE - 1)/ DUMP_MEM_PACKET_SIZE;
    residual_num = mem_lenth % DUMP_MEM_PACKET_SIZE;
    seq_num = s_mem_seq_num;

    // Dump memory data.
    for (num = 0; num < (packet_num - 1); num++)
    {
        OS_DebugPutChar(0x7E);
        
        // update and print Head info.
        _OS_UpdateDumpHeader(++seq_num, DUMP_MEM_PACKET_SIZE);        
        
        // Data.
        for (i = 0; i < DUMP_MEM_PACKET_SIZE; i+=2)
        {
#ifdef CHIP_ENDIAN_LITTLE            
            mem_data = WORD_ENDIAN_SWAP(*mem_ptr);/*lint !e564*/
#else
            mem_data = *mem_ptr;
#endif
            PutUint16(mem_data);
            mem_ptr++;
        }

        OS_DebugPutChar(0x7E);
    }
    
    if (0 == residual_num)
    {
        residual_num = DUMP_MEM_PACKET_SIZE;
    }
    
    
    OS_DebugPutChar(0x7E);
        
    // update and print Head info.
    _OS_UpdateDumpHeader(++seq_num, residual_num);        
    s_mem_seq_num = seq_num ;
    
    // Data.
    for (i = 0; i < residual_num; i+=2)
    {
#ifdef CHIP_ENDIAN_LITTLE        
        mem_data = WORD_ENDIAN_SWAP(*mem_ptr);/*lint !e564*/
#else
        mem_data = *mem_ptr;
#endif
        PutUint16(mem_data);
        mem_ptr++;
    }

    OS_DebugPutChar(0x7E);
}

static __inline uint32 DumpRegisterGroup(void)
{
    uint32  i = 0;   
    uint32 reg_group_num = CHIP_GetNumCtlRegGrp();
    CTL_REG_INFO_T  *IpRegInfoPtr = (CTL_REG_INFO_T *)CHIP_GetInfoOfCtlReg();
    uint32 dump_count =0;
    uint32 ip_reg_size = 0;

    for(i=0; i<reg_group_num; i++)
    {
        if(IpRegInfoPtr[i].reg_address_begin != 0xffffffff)
        {
            ip_reg_size = IpRegInfoPtr[i].reg_address_end - IpRegInfoPtr[i].reg_address_begin;
            SCI_PrintAssertString("\n Region:Offset=0x%08X, Length=0x%08X",
                    IpRegInfoPtr[i].reg_address_begin,
                    ip_reg_size);
            DumpMemSection(IpRegInfoPtr[i].reg_address_begin, ip_reg_size);
            dump_count+=ip_reg_size;
        }
    }

    return dump_count;
}

static __inline uint32 DumpDSPMem(void)
{
    if(SCI_IsDspRunInAssertMode())
    {
        uint32  i;
        uint32  seq_num;
        volatile unsigned short * shared_mem_ptr = (volatile unsigned short *) CHIP_GetSharedMemBase();
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		volatile unsigned short * temp_reg_ptr   = (volatile unsigned short *) 0x30000FA0;
#else
        volatile unsigned short * temp_reg_ptr   = (volatile unsigned short *) 0x10000FA0;
#endif
        uint16 dsp_addr = 0x0000;
        unsigned short k;
        volatile unsigned short * tmp_ptr = NULL;
    
        unsigned short data;
        unsigned char data_low;
        unsigned char data_high;
        uint32 dump_count = 0;

        seq_num = s_mem_seq_num;
        // dump dsp internal register first
         OS_DebugPutChar(0x7E);
    
        // Head.
        _OS_UpdateDumpHeader(++seq_num, 64);        


        tmp_ptr = temp_reg_ptr;
        for(k=0; k<32; k++)
        {
            data = *tmp_ptr++;
            data_low    = data & 0xff;
            data_high   = (data & 0xff00) >> 8;

                      dump_count=dump_count+2;
            // Dump high bit
            if ((data_high == 0x7E) || (data_high == 0x7D))
            {
                OS_DebugPutChar(0x7D);
                OS_DebugPutChar((data_high ^ 0x20));
            }
            else
            {
                OS_DebugPutChar(data_high);
            }

            // Dump low bit
            if ((data_low == 0x7E) || (data_low == 0x7D))
            {
                OS_DebugPutChar(0x7D);
                OS_DebugPutChar((data_low ^ 0x20));
            }
            else
            {
                OS_DebugPutChar(data_low);
            }
        }
    
        OS_DebugPutChar(0x7E);
    
        // Data
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
		for (i = 0; i < 24; i++)  //dsp memory 24K halfword
#else
        for (i = 0; i < 64; i++)
#endif
        {
            cmd_ptr->RefCmd_block.SeqNr = dsp_addr;
            cmd_ptr->RefCmd_block.reserved2 = 1024;
            cmd_ptr->RefCmd_block.cmd_done = 0;
            dsp_addr += 1024;
            while( 0 == cmd_ptr->RefCmd_block.cmd_done ){};
        
            OS_DebugPutChar(0x7E);
        
            // Head.
            _OS_UpdateDumpHeader(++seq_num, 2048);        
    
            tmp_ptr = shared_mem_ptr;
            for(k=0; k<1024; k++)
            {
                data = *tmp_ptr++;
                data_low    = data & 0xff;
                data_high   = (data & 0xff00) >> 8;

                    dump_count=dump_count+2;
                // Dump high bit
                if ((data_high == 0x7E) || (data_high == 0x7D))
                {
                    OS_DebugPutChar(0x7D);
                    OS_DebugPutChar((data_high ^ 0x20));
                }
                else
                {
                    OS_DebugPutChar(data_high);
                }

                // Dump low bit
                if ((data_low == 0x7E) || (data_low == 0x7D))
                {
                    OS_DebugPutChar(0x7D);
                    OS_DebugPutChar((data_low ^ 0x20));
                }
                else
                {
                    OS_DebugPutChar(data_low);
                }
            }
        
            OS_DebugPutChar(0x7E);
        }

        s_mem_seq_num = seq_num;
        
        return dump_count;
    }
        
    return 0;
}
#ifdef CARD_LOG
LOCAL void OS_DumpLogSaveBuf(void)
{
    uint8* addr;
    uint32 len,i;

    do
    {
        SCI_PrintAssertString("\n=============== Dump LogSave ArmLog Memory==============\n");
        LogSaveOutput_GetArmLogBufInfoAfterAssert(&addr, &len);
        if((0 == len)||(0 == addr))
        {
            SCI_PrintAssertString("no logsave armlog");
            break;
        }
        DumpMemSection((uint32)addr, len); 
        OS_DebugPutChar(0x7E);
        for (i = 0; i < sizeof(dump_mem_end); i++)
        {       
           PutUint8(dump_mem_end[i]); 
        }
        OS_DebugPutChar(0x9);
        OS_DebugPutChar(0x7E);
        SCI_PrintAssertString( "\nArmLog Memory Dumping Finished:begin addr=0x%08X,total size=%ldByte(0x%08x)\n",(uint32)addr, len,len);
    }while(0);

    do
    {
        SCI_PrintAssertString("\n=============== Dump LogSave DspLog Memory==============\n");
        LogSaveOutput_GetDspLogBufInfoAfterAssert(&addr, &len);
        if((0 == len)||(0 == addr))
        {
            SCI_PrintAssertString("no logsave DspLog");
            break;
        }
        DumpMemSection((uint32)addr, len); 
        OS_DebugPutChar(0x7E);
        for (i = 0; i < sizeof(dump_mem_end); i++)
        {       
           PutUint8(dump_mem_end[i]); 
        }
        OS_DebugPutChar(0x9);
        OS_DebugPutChar(0x7E);
        SCI_PrintAssertString( "\nArmLog Memory Dumping Finished:begin addr=0x%08X,total size=%ldByte(0x%08x)\n",(uint32)addr, len,len);
    }while(0);

    do
    {
        SCI_PrintAssertString("\n=============== Dump LogSave IQ Memory==============\n");
        LogSaveOutput_GetDspIqBufInfoAfterAssert(&addr, &len);
        if((0 == len)||(0 == addr))
        {
            SCI_PrintAssertString("no logsave IQ");
            break;
        }
        DumpMemSection((uint32)addr, len); 
        OS_DebugPutChar(0x7E);
        for (i = 0; i < sizeof(dump_mem_end); i++)
        {       
           PutUint8(dump_mem_end[i]); 
        }
        OS_DebugPutChar(0x9);
        OS_DebugPutChar(0x7E);
        SCI_PrintAssertString( "\nArmLog Memory Dumping Finished:begin addr=0x%08X,total size=%ldByte(0x%08x)\n",(uint32)addr, len,len);
    }while(0);
}
#endif
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
#define  UART_READ_TIMEOUT      8000
#define  DUMP_GE2MEM_READ_SIZE      (1024)
#define  DUMP_GE2MEM_PACKET_SIZE      (4 * 1024)
#define DUMP_TOTAL_SIZE   (0x88000)
uint8 ge2_dump_memory[DUMP_GE2MEM_PACKET_SIZE] = {0};
extern uint8 assertbuf[];
extern uint32 assertpos;
/*****************************************************************************/
//  Description:   DumpGe2Memory
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
static  void DumpGe2Memory(uint32 mem_begin_addr, uint32 mem_lenth)
{	
	uint32	i;
	uint32	num;
	uint32	packet_num;
	uint32	residual_num = 0;
	uint32	seq_num;
	
	uint8	mem_data = 0;
	uint8 *mem_ptr = (uint8 *)mem_begin_addr;
	
	if(0 == mem_lenth)
	{
		return;
	}

	packet_num = (mem_lenth + DUMP_GE2MEM_PACKET_SIZE - 1)/ DUMP_GE2MEM_PACKET_SIZE;
	residual_num = mem_lenth % DUMP_GE2MEM_PACKET_SIZE;

	seq_num = s_mem_seq_num;
	// Dump memory data.
	for (num = 0; num < (packet_num - 1); num++)
	{
		OS_DebugPutChar(0x7E);
		
		// update and print Head info.
		_OS_UpdateDumpHeader(++seq_num, DUMP_GE2MEM_PACKET_SIZE);		  

		// Data.
		PutUint8Chars(mem_ptr, DUMP_GE2MEM_PACKET_SIZE);
		mem_ptr += DUMP_GE2MEM_PACKET_SIZE;

		OS_DebugPutChar(0x7E);
	}
	
	if (0 == residual_num)
	{
		residual_num = DUMP_GE2MEM_PACKET_SIZE;
	}
	
	
	OS_DebugPutChar(0x7E);
		
	// update and print Head info.
	_OS_UpdateDumpHeader(++seq_num, residual_num);		  
	s_mem_seq_num = seq_num ;
	
	// Data.
	for (i = 0; i < residual_num; i++)
	{
		
		mem_data = *mem_ptr;
		PutUint8(mem_data);
		mem_ptr++;
	}

	OS_DebugPutChar(0x7E);
}

/*****************************************************************************/
//  Description:   OutputGe2Memory
//  Global resource dependence:
//  Author:         
//  Note:
/*****************************************************************************/
static uint32 OutputGe2Memory(uint8 gps_com)
{
	uint32	i, count;
	uint32	tick_old, tick_new;
	uint8	rx_buffer[DUMP_GE2MEM_READ_SIZE];
	uint8	*ptr_rx_buffer = NULL;
	uint32    read_len =0;
	uint8 *ptr_ge2_dump = ge2_dump_memory;
	uint32 data_length = 0;
	uint32 total_length = 0;
	int ret = 0;
	
	memset(rx_buffer, 0, sizeof(rx_buffer));
	ret = gps_get_assert_mode();

	if(ret == 0)
		return 0;
	if(2 == ret)
	{
		memset(rx_buffer, 0x8e,64);
		data_length = UART_PHY_WriteData(gps_com, rx_buffer, 8);
		ptr_rx_buffer = ptr_ge2_dump;
		read_len = DUMP_GE2MEM_PACKET_SIZE;
		if(data_length <=  2)
		{   
			return 0;
		}
	}
	else
	{
		ptr_rx_buffer = rx_buffer;
		read_len = DUMP_GE2MEM_READ_SIZE-1;
		DumpGe2Memory(assertbuf, assertpos);
		total_length += assertpos;
	}
	tick_old = tick_new = SCI_GetTickCount();
	 // Rx fifo count
	while( (tick_new-tick_old) < UART_READ_TIMEOUT)
	{
		count = UART_PHY_ReadData(gps_com,ptr_rx_buffer,read_len);
		if(count > 0)
		{
			data_length = gps_assert_entry(ptr_rx_buffer,ptr_ge2_dump,count);
			if(data_length != 0 )
			{
				DumpGe2Memory(ptr_ge2_dump, data_length);
				memset(ptr_ge2_dump, 0x0, data_length);
				memset(ptr_rx_buffer, 0x0, read_len);
				total_length += data_length;
			}
				
			tick_old = SCI_GetTickCount();
		}
		tick_new = SCI_GetTickCount();
	}
	return total_length;

}
#endif
#endif /*WIN32*/

/*****************************************************************************/
//  Description:    The command function
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
 #ifdef  NANDBOOT_SUPPORT
PUBLIC void  DUMP_NV_Nand (const char *Name)
{
#ifndef WIN32
    uint32  rRet,i,j,num,k;
    uint32  nandId;
    uint32  size,secnum_per_packet;
    BSD_DEVICE      dev;
    BSD_STATUS      status;
    BSD_HANDLE      pHandle;
    uint8  buf[512];
    uint8   mem_data = 0;
    uint32  packet_num,s_mem_seq_num;   
    SCI_FTL_HANDLE  ftlHandle;
    SCI_FTL_PARTITION_INFO  partitionInfo ;
    nandId = 0;
    
    status = BSD_Open(Name, BSD_GENERIC_READ,&pHandle);
       status = BSD_GetDevice(Name, &dev);
       size = ( dev.SectorSize ) * ( dev.TotScts );
    SCI_PrintAssertString("\n NV sctSize :sctSize=0x%0x", dev.SectorSize ); 
    SCI_PrintAssertString("\n NV sctTotalNum :sctTotalNum=0x%0x", dev.TotScts);     
    SCI_PrintAssertString("\n NV totalSize =0x%0x", size); 
    
       packet_num = size / DUMP_MEM_PACKET_SIZE;
       s_mem_seq_num = 0;  
    j=0;
    secnum_per_packet = DUMP_MEM_PACKET_SIZE /512;
    // Dump memory data.
       for (num = 0; num < packet_num; num++)
     {
        OS_DebugPutChar(0x7E);

        // Head.
        // update and print Head info.
        _OS_UpdateDumpHeader(++s_mem_seq_num, DUMP_MEM_PACKET_SIZE);        

        // Send 1 Packet Data.
        for (i = 0; i < secnum_per_packet ; i++)            
        {       
               // read 1 sec data.
             status = BSD_Read(pHandle, j, 1, buf);
             j++;
              //  send 1 sec data.
            for (k = 0; k< 512 ; k++)           
               {     
                    mem_data = buf[k];
                    PutUint8(mem_data);
             }
        }
        
        OS_DebugPutChar(0x7E);
     }
       
     // Dump end packet.
     OS_DebugPutChar(0x7E);
     for (i = 0; i < sizeof(dump_mem_end); i++)
     {       
        PutUint8(dump_mem_end[i]); 
     }    
     OS_DebugPutChar(0x9);
     OS_DebugPutChar(0x7E);
     
     status=BSD_Close(pHandle);   
    
/*
    if(SCI_FTL_IsOpen(nandId, partid, &ftlHandle, 0))
        {
            SCI_PrintAssertString("\n NV has been Open");  
        }
    else
        {
            rRet = SCI_FTL_Open(nandId, partid, &ftlHandle, 0);
            SCI_PrintAssertString("\n NV SCI_FTL_Open ftlHandle=0x%0x", ftlHandle);  
        }

    rRet = SCI_FTL_GetPartInfo(ftlHandle, &partitionInfo);
    SCI_PrintAssertString("\n  PartitionInfo addr =0x%0x", &partitionInfo);  
    
       size = ( partitionInfo.sctSize ) * ( partitionInfo.sctTotalNum );
    SCI_PrintAssertString("\n NV sctSize :sctSize=0x%0x", partitionInfo.sctSize); 
    SCI_PrintAssertString("\n NV sctTotalNum :sctTotalNum=0x%0x", partitionInfo.sctTotalNum);   
    SCI_PrintAssertString("\n NV totalSize =0x%0x", size); 
    
       packet_num = size / DUMP_MEM_PACKET_SIZE;
       s_mem_seq_num = 0;  
    j=0;
    secnum_per_packet = DUMP_MEM_PACKET_SIZE /512;
    // Dump memory data.
       for (num = 0; num < packet_num; num++)
     {
        OS_DebugPutChar(0x7E);

        // Head.
        // update and print Head info.
        _OS_UpdateDumpHeader(++s_mem_seq_num, DUMP_MEM_PACKET_SIZE);        

        // Send 1 Packet Data.
        for (i = 0; i < secnum_per_packet ; i++)            
        {       
               // read 1 sec data.
             rRet = SCI_FTL_Read(ftlHandle, j, 1, buf); 
             j++;
              //  send 1 sec data.
            for (k = 0; k< 512 ; k++)           
               {     
                    mem_data = buf[k];
                    PutUint8(mem_data);
             }
        }
        
        OS_DebugPutChar(0x7E);
     }
       
     // Dump end packet.
     OS_DebugPutChar(0x7E);
     for (i = 0; i < sizeof(dump_mem_end); i++)
     {       
        PutUint8(dump_mem_end[i]); 
     }    
     OS_DebugPutChar(0x9);
     OS_DebugPutChar(0x7E);
     
     SCI_FTL_Close(ftlHandle);   
*/
#endif
}
#endif

/*****************************************************************************/
//  Description:    The DumpMemory function is used to dump out len bytes
//                  memory from begin address.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC void OS_DumpMemory(void)
{
#ifndef WIN32
    uint32  dump_len = 0;
    uint16  i,num_of_dump_mem;
    uint32 size_of_dump_mem = 0;

	CTL_MEM_INFO_T *CtlMemInfoPtr = NULL;

    CtlMemInfoPtr = (CTL_MEM_INFO_T *)CHIP_GetInfoOfCtlMem();
	num_of_dump_mem = CHIP_GetNumCtlMemGrp();
	
   SCI_PrintAssertString("\n=============== Dump All Memory To One File==============\n");
   s_mem_seq_num = 0;
   for(i=0;i<num_of_dump_mem;i++){
		if(CtlMemInfoPtr[i].mem_address_begin != 0xffffffff){
			SCI_PrintAssertString("\n Region:Offset=0x%08X, Length=0x%08X",CtlMemInfoPtr[i].mem_address_begin,CtlMemInfoPtr[i].mem_size);
			DumpMemSection(CtlMemInfoPtr[i].mem_address_begin, CtlMemInfoPtr[i].mem_size);
			size_of_dump_mem+=CtlMemInfoPtr[i].mem_size;
		}
   }

   // Dump registers 
   dump_len = DumpRegisterGroup();  
   SCI_PrintAssertString("\nRegister Data:Offset=0x%08X, Length=0x%08X", (int)size_of_dump_mem, (int)dump_len);
   size_of_dump_mem=size_of_dump_mem+ dump_len; 

    // Dump end packet.
    OS_DebugPutChar(0x7E);
    for (i = 0; i < sizeof(dump_mem_end); i++)
    {       
       PutUint8(dump_mem_end[i]); 
    }
    OS_DebugPutChar(0x9);
    OS_DebugPutChar(0x7E);
    
    SCI_PrintAssertString( "\nMemory Dumping Finished:begin addr=0x%08X,total size=%ldByte(0x%08x)\n",
        (int)CtlMemInfoPtr[0].mem_address_begin,size_of_dump_mem, size_of_dump_mem);
#ifdef CARD_LOG
    OS_DumpLogSaveBuf();
#endif
#endif
}

/*****************************************************************************/
//  Description:    Dump size_in_byte bytes definite memory out which begin from
//                  start_addr address.
//  Global resource dependence:
//  Author:         Daniel.Luo  2006/09/11
//  Note:
/*****************************************************************************/
PUBLIC void OS_DumpDefiniteMemory(
                                    uint32 start_addr,      //start address
                                    uint32 size_in_byte)    //how many bytes needed to be dumped
{
#ifndef WIN32
    uint32  i= 0;
    uint32  num;
    char    *mem_ptr = (char *)start_addr;
    uint8   mem_data = 0;
    uint32  packet_num;
    uint32  seq_num;
    uint32 residual_num;

    
    SCI_PrintAssertString("\nDumping memory to a file, which from 0x%lx with %lu bytes, please wait ......\n",
        start_addr, size_in_byte);

    packet_num = size_in_byte;
    packet_num = packet_num / DUMP_MEM_PACKET_SIZE;
    residual_num = size_in_byte % DUMP_MEM_PACKET_SIZE;

    s_mem_seq_num = 0;

    seq_num = s_mem_seq_num;

    
    // Dump memory data.
    for (num = 0; num < packet_num; num++)
    {
        OS_DebugPutChar(0x7E);

        // Head.
        // update and print Head info.
        _OS_UpdateDumpHeader(++seq_num, DUMP_MEM_PACKET_SIZE);        

        // Data.
        for (i = 0; i < DUMP_MEM_PACKET_SIZE ; i++)
        {
            mem_data = *mem_ptr;
            PutUint8(mem_data);
            mem_ptr++;
        }

        OS_DebugPutChar(0x7E);
    }

    
    
    OS_DebugPutChar(0x7E);
        
    // update and print Head info.
    _OS_UpdateDumpHeader(++seq_num, residual_num);        
    
    // Data.
    for (i = 0; i < residual_num; i++)
    {
        
        mem_data = *mem_ptr;
        PutUint8(mem_data);
        mem_ptr++;
    }

    OS_DebugPutChar(0x7E);
	

    // Dump end packet.
    OS_DebugPutChar(0x7E);
    for (i = 0; i < sizeof(dump_mem_end); i++)
    {       
       PutUint8(dump_mem_end[i]); 
    }
    
    OS_DebugPutChar(0x9);
    OS_DebugPutChar(0x7E);

    SCI_PrintAssertString("\nDumping memory completed !!!\n");
#endif
}


/*****************************************************************************/
//  Description:    OS_CheckCodeRegion
//  Global resource dependence: 
//  Author:        Younger.yang
//  Note:           This function  provide system code region
/*****************************************************************************/
BOOLEAN OS_CheckCodeRegion(uint32 addr)
{
    return MEM_IsRORegion(addr);

}

/*****************************************************************************/
//  Description:    OS_CheckStackRegion
//  Global resource dependence: 
//  Author:        Younger.yang
//  Note:           This function  provide system stack region
/*****************************************************************************/

BOOLEAN OS_CheckStackRegion(uint32 addr)
{
    return MEM_IsRWRegion(addr);

}

void osiReset(osiResetMode_t mode)
{
    WDG_TimerStop();
    if (mode == OSI_RESET_FORCE_DOWNLOAD)
    {
#ifndef WIN32
        CHIP_PHY_SetBootMode();
#endif
    }
    
    OS_ResetEntryDownload();
    
    while (1)
        ;
}

/******************************************************************************/
// Description:   Handler AT Command
// Dependence:    None
// Author:        Haifeng.Yang
// Note:
/******************************************************************************/
PUBLIC uint32 REF_HandleATCommand(uint8* at_string)
{
#ifndef WIN32
    typedef uint32 (*CMD_HANDLER)(uint32);
    typedef struct 
    {
        uint8*      p_cmd_str;
        CMD_HANDLER cmd_handler;
    }AT_CMD_ENTRY;

    //CMD TABLE
    static const AT_CMD_ENTRY s_cmd_entry[] = 
    {
        {(uint8*)"AUTODLOADER", AutoDL_StartDownLoad}
    };
    uint32 i;
    uint8  *temp_p0, *temp_p1, ch;
    
    if(at_string == NULL)
    {
        return REF_AT_INVLID_PARA;
    }

    //Convert strings to upper case and delete redundant space
    temp_p0 = at_string;
    temp_p1 = at_string;
    ch = *temp_p1++;
    while(ch)
    {
        if(ch == ' ')
        {
            while(*temp_p1 == ' '){temp_p1++;};
            *temp_p0++ = ' ';
            ch = *temp_p1++;
            continue;
        }
        else if('a' <= ch && ch <= 'z')
        {
            ch -= 32;
        }
        *temp_p0++ = ch;
        ch = *temp_p1++;
    }
    *temp_p0++ = 0;

    //Search CMD table. 
    for(i = 0; i < sizeof(s_cmd_entry)/sizeof(AT_CMD_ENTRY); i++)
    {
        if(strncmp((int8*)at_string, (int8*)s_cmd_entry[i].p_cmd_str, /*lint !e746 */
            strlen((int8*)s_cmd_entry[i].p_cmd_str)) == 0)
        {
            return s_cmd_entry[i].cmd_handler((uint32)at_string);
        }
    }   
#endif

    return REF_AT_INVLID_CMD;
}
/******************************************************************************/
// Description:   REF_SetModeForRatSwitch
// Dependence:    None
// Note: 
//     net_mode: 0  -2G,    1   -3G
/******************************************************************************/

PUBLIC uint32 REF_SetModeForRatSwitch(uint32 net_mode)
{   
  #ifndef WIN32
    // set chip clk
    #if defined(PLATFORM_SC8800G)
        CHIP_ClkForNetSwitch(net_mode);
    #endif
    // set others...
  #endif
    return 0;
}
/*****************************************************************************/
//  Description:    REF_RestoreFactorySetting
//  Global resource dependence: 
//  Author:        
//  Note:          add for REF reset during mmi reset factory setting 
/*****************************************************************************/

PUBLIC uint32 REF_RestoreFactorySetting(void)
{
    //SCI_TRACE_LOW:"[REF] REF_RestoreFactorySetting() entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,REF_OUTPORT_1524_112_2_18_1_5_32_55,(uint8*)"");
    // disable USB Auto Mode, will be checked before writing
    REFPARAM_SetUsbAutoModeFlag(0);
    
    return 0;
}

/*****************************************************************************/
//  Description: REF_ioctl
//  Note:        data_ptr should be ready before call it       
/*****************************************************************************/
PUBLIC uint32 REF_ioctl(uint32 cmd, void * data_ptr)
{
    uint32 ret = 0;
    
    SCI_TRACE_LOW("[REF] REF_ioctl: cmd = %d, data_ptr = 0x%X", cmd, data_ptr);
    if(NULL==data_ptr)
    {
        SCI_TRACE_LOW("[REF]<ERROR> REF_ioctl: data_ptr is invalid !!");
        return 2;
    }
  #ifndef WIN32
    switch(cmd)
    {
    
        case REF_IOCTL_SET_CHIP_SDRAM_CLK:
        {   /*
            #if defined(PLATFORM_SC8800G)
                ret = CHIP_SetSdramClk(*(uint32 *) data_ptr);
            #endif
            */
            break;
        }
        case REF_IOCTL_SET_RFOUTPUT:
        {   /*
            REF_IOCTL_RFOUTPUT_DATA_T * rfoutput_data_ptr = 0;
            rfoutput_data_ptr = (REF_IOCTL_RFOUTPUT_DATA_T *) data_ptr;
            
            SCI_TRACE_LOW("[REF] REF_ioctl: SET_RFOUTPUT: switch_on = %d, value = %d",
                rfoutput_data_ptr->switch_on,
                rfoutput_data_ptr->value);
            L1API_DSPSetRfoutput(rfoutput_data_ptr->switch_on, 
                                 rfoutput_data_ptr->value);

           */
            break;
        }
        case REF_IOCTL_GET_RFOUTPUT:
        {   
            /*
            REF_IOCTL_RFOUTPUT_DATA_T * rfoutput_data_ptr = 0;
            rfoutput_data_ptr = (REF_IOCTL_RFOUTPUT_DATA_T *) data_ptr;
            
            PROD_GetRFOUTCfg(rfoutput_data_ptr);
            SCI_TRACE_LOW("[REF] REF_ioctl: GET_RFOUTPUT: switch_on = %d, value = %d",
                rfoutput_data_ptr->switch_on,
                rfoutput_data_ptr->value);
            */
            break;
        }        

        case REF_IOCTL_SET_DC_DSPTICK:
        {
            //set the dsp time tick for dsp 
            s_dsp_tick_count = SCI_GetTickCount();
            break;
        }

        case REF_IOCTL_SET_DC_CAPSOF:
        {
            //set the dsp time tick for dsp 
            s_isp_cap_sof_tick = SCI_GetTickCount();
            break;
        }

        case REF_IOCTL_CHK_DC_CAPTICK:
        {
            //check if the capture time is in abnormal range
            SCI_TRACE_LOW("_ISP_DSP_check in isp txdone service: cap sof tick is %d, dsp sign tick is %d", s_isp_cap_sof_tick, s_dsp_tick_count);

            if((s_isp_cap_sof_tick-22) <= s_dsp_tick_count)
            {
                *(BOOLEAN *)data_ptr = SCI_FALSE;
            }
            else
            {
                *(BOOLEAN *)data_ptr = SCI_TRUE;
            }
            break;
        }
        
        default:
            SCI_TRACE_LOW("[REF]<WARNING> REF_ioctl: CMD is NOT not support");
            ret = 0;
            break;
    }
  #endif
    return ret;
}


#ifdef RF_HW_SELFADAPTIVE_ENABLE


int s_ph_adaptive_gpio_map[] =
{ 
	14,   /*GPIO 14*/
	15    /*GPIO 15*/
};

/*****************************************************************************/
//  Description:   PH_GetAdaptiveGpioValue
//	Global resource dependence:
//  Author:  tiger.han       
//	Note:
/*****************************************************************************/
PUBLIC uint8 PH_GetAdaptiveGpioValue(void)
{
	int i;
	int adaptive_gpio_cnt = 0;
	BOOLEAN info = 0;
	uint8 result = 0;

	adaptive_gpio_cnt = sizeof(s_ph_adaptive_gpio_map) / sizeof(s_ph_adaptive_gpio_map[0]);

	for(i = 0; i < adaptive_gpio_cnt; i++)
	{
		// Get GPIO value
		info = GPIO_GetValue(s_ph_adaptive_gpio_map[i]);
		result = (result << 1) | info;
	}

	return result;
}

/*****************************************************************************/
//  Description:   SCI_Get_HW_VERSION
//	Global resource dependence:
//  Author:       
//	Note:
/*****************************************************************************/
PUBLIC PH_HW_VERSION  SCI_Get_HW_VERSION(void)
{
    uint8 result = 0;
    PH_HW_VERSION hw_version = PH_HW_MAX;

#ifdef RF_HW_SELFADAPTIVE_ENABLE
     result = PH_GetAdaptiveGpioValue();
#endif

    SCI_TRACE_LOW("SCI_Get_HW_VERSION result=%d",result);

    switch(result){
        case 0:
            hw_version = PH_HW_VER_00;
            break;
        case 1:
            hw_version = PH_HW_VER_01;
            break;           
        case 2:
            hw_version = PH_HW_VER_02;
            break;         
        case 3:
            hw_version = PH_HW_VER_03;    
            break;
         default:
            break;            
    }
    return hw_version;
}

//houlili add for RF ID
PUBLIC char* SCI_GET_RF_ID(void)
{
    uint8 result = 0;
    char* rf_id = (char*)"RFPA ID:000";
#ifdef RF_HW_SELFADAPTIVE_ENABLE
    result = PH_GetAdaptiveGpioValue();
#endif
    SCI_TRACE_LOW("SCI_GET_RF_ID result=%d",result);
    switch(result){
        case 0:
            rf_id = (char*)"RFPA ID:00";
            break;
        case 1:
            rf_id = (char*)"RFPA ID:01";
            break;           
        case 2:
            rf_id = (char*)"RFPA ID:10";
            break;         
        case 3:
            rf_id = (char*)"RFPA ID:11";    
            break;
         default:
            break;            
    }
    return rf_id;
}
/********************************************************************
bit     0--1--2--3--4--5--6--7--8--9--10-11-12-13-14-15
Band    1  2  3  4  5  7  8  12 13 17 20 28 38 39 40 41
bit     16-17-18-19-20-21-22-23-24-25-26-27-28-29-30-31
Band    66 xx xx xx xx xx xx xx |---MAGIC CODE:0x5e---|
*********************************************************************/
PUBLIC char* SCI_GET_RF_BAND_ID(void)
{
    uint8 result = 0;
    uint32 rf_id = 0;
#ifdef RF_HW_SELFADAPTIVE_ENABLE
    result = PH_GetAdaptiveGpioValue();
#endif
    SCI_TRACE_LOW("SCI_GET_RF_ID result=%d",result);
    if (result & 0x1) {
        /*RF Band: G-850-900-1800-L-1-3-5-8-38-39-40-41*/
        rf_id |= (0x1<<0 |   //BAND1
                  0x1<<2 |   //BAND3
                  0x1<<4 |   //BAND5
                  0x1<<6 |   //BAND8
                  0x1<<12|   //BAND38
                  0x1<<13|   //BAND39
                  0x1<<14|   //BAND40
                  0x1<<15);  //BAND41

    } else {
        /*RF Band: G-850-900-1800-L-1-3-5-7-8-20-28-38-40-41*/
        rf_id |= (0x1<<0 |   //BAND1
                  0x1<<2 |   //BAND3
                  0x1<<4 |   //BAND5
                  0x1<<5 |   //BAND7
                  0x1<<6 |   //BAND8
                  0x1<<10|   //BAND20
                  0x1<<11|   //BAND28
                  0x1<<12|   //BAND38
                  0x1<<14|   //BAND40
                  0x1<<15);  //BAND41
    }
    return (rf_id | (0x5e << 24));
}


#endif



#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
PUBLIC void REF_7sResetEnable(BOOLEAN enable)
{
	#ifndef WIN32
	if(enable)
	{
		CHIP_Enable7sRstMode();
	}
	else
	{
		CHIP_Disable7sRstMode();
	}
	#endif
}

#ifndef RF_HW_SELFADAPTIVE_ENABLE
PUBLIC char* SCI_GET_RF_ID(void)
{
    //TODO -- MMI_COMPILE_E
    return "RFPA ID:000";
}
#endif
#else
PUBLIC void REF_7sResetEnable(BOOLEAN enable)
{
	return ;
}


#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

