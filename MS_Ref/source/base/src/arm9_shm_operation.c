/******************************************************************************
 ** File Name:      bsc_device.c                                                    *
 ** Author:                                                       *
 ** DATE:                                                         *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 bsc data. It manages bootloader write and read.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME             	DESCRIPTION                               *
 ** 										                                  *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "bt_abs.h"
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include <string.h>
#include "arm9_shm_operation.h"
#include "isr_drvapi.h"
#if !defined(PLATFORM_UWS6121E)
#include "sc6531efm_int_cfg.h"
#else
#include "uws6121e_int_cfg.h"
#endif
#include "doidleonpage.h"
#include "priority_system.h"
#include "tasks_id.h"
#include "task_monitor.h"
/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

/******************************************************************************
 * Static Global Variables
 ******************************************************************************/
#define ARM9_CM4_SHM_ADDR	0x30100400
#define SHM_FIFO_SIZE		500

#define ARM9_SHM_WRITE_START_ADDR		(ARM9_CM4_SHM_ADDR)
#define ARM9_SHM_READ_START_ADDR		(ARM9_SHM_WRITE_START_ADDR+SHM_FIFO_SIZE)

#define ARM9_SHM_WRITE_LENGTH_ADDR	(ARM9_CM4_SHM_ADDR -0x4)
#define ARM9_SHM_READ_LENGTH_ADDR		(ARM9_CM4_SHM_ADDR -0x8)

#define CLR_CM4_INT             				0X8B000160
#define CLR_CM4_TO_ARM9_IRQ_INT0		BIT_15
#define CLR_CM4_TO_ARM9_IRQ_INT1		BIT_14
#define CLR_CM4_TO_ARM9_IRQ_INT2		BIT_18
#define SET_ARM9_TO_CM4_IRQ_INT0		BIT_13
#define SET_ARM9_TO_CM4_IRQ_INT1		BIT_12
#define SET_ARM9_TO_CM4_IRQ_INT2		BIT_16

#define SHM_TIMER_INTERVAL      50

typedef enum{
	SHM_DATA_TO_WRITE,
	SHM_DATA_TO_READ,
	SHM_LOG_TO_READ,
	SHM_WRITE_COMPLETE ,
	SHM_INIT_COMPLETE ,
	SHM_SHUTDOWN_COMPLETE ,
	MAX_EVENT
}SHM_EVENT_T;

SHM_CB_T shm_cb;

typedef uint32                TIME;
typedef uint32                  scheduler_identifier   ;
typedef scheduler_identifier    tid;

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :
// Global resource dependence :
// Author :
// Note :
/*****************************************************************************/
static void Arm9_Shm_Data_Write(uint8 *buf,uint32 length,BOOLEAN flag);
static void Arm9_Shm_Data_Read(uint8 *buf,uint32 length,BOOLEAN flag);
static void ShmDrv_Rx(void);
static BOOLEAN ShmDrv_Tx(char *buf, uint16 num_to_send, uint16 *numSend);
static BOOLEAN __ShmTxIsBusy(void);
static void __ShmTxData(void);
static uint32 OSShmDrv_EventCallBack(uint32 event);
static void _SHM_DataReadTimerHandler (uint32 state);
static ISR_EXE_T Shm_Data_Write_ISR_Handle (uint32 param);
static ISR_EXE_T Shm_Data_Read_ISR_Handle (uint32 param);
static void  ShmDataWriteHisrFunc (uint32 i, void *d);
static void  ShmDataReadHisrFunc (uint32 i, void *d);
static void Shm_Send_Msg(SHM_CHANNEL_T channel);
extern void OS_SendMsgToSchedTask(uint32 event, void *msg_ptr);
extern BOOLEAN OS_IsSchedRunning(void);
extern tid timed_event_in(TIME delay, void (*fn)(uint16, void *), uint16 fniarg, void *fnvarg);
extern uint32 uartReadData(char *buffer, uint32 count);
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 PUBLIC SHM_HW_CONFIG_T shm_hw_config =
{
    {(uint8 *)ARM9_SHM_WRITE_START_ADDR, (uint8 *)ARM9_SHM_READ_START_ADDR},
    {(uint32 *)ARM9_SHM_WRITE_LENGTH_ADDR, (uint32 *)ARM9_SHM_READ_LENGTH_ADDR},
    {   {TB_CM4_TO_ARM9_INT0, Shm_Data_Write_ISR_Handle},
         {TB_CM4_TO_ARM9_INT1, Shm_Data_Read_ISR_Handle},
    },
    SHM_FIFO_SIZE,
};

/*****************************************************************************/
// Description :    Arm9&Cm4 Share Memory Init.
// PUBLIC resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void Shm_Data_Init()
{
	shm_cb.p_hw = &shm_hw_config;

	ISR_RegHandler_Ex(shm_cb.p_hw->inter[SHM_DATA_WRITE].int_num,shm_cb.p_hw->inter[SHM_DATA_WRITE].func, ShmDataWriteHisrFunc,CHIPDRV_HISR_PRIO_1,NULL);

	ISR_RegHandler_Ex(shm_cb.p_hw->inter[SHM_DATA_READ].int_num,shm_cb.p_hw->inter[SHM_DATA_READ].func, ShmDataReadHisrFunc,CHIPDRV_HISR_PRIO_1,NULL);
}

/*****************************************************************************/
// Description :    Arm9&Cm4 Share Memory Close.
// PUBLIC resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void Shm_Close()
{
         *(uint32*)(CLR_CM4_INT) |= CLR_CM4_TO_ARM9_IRQ_INT0;

	 *(uint32*)(CLR_CM4_INT) |= CLR_CM4_TO_ARM9_IRQ_INT1;

	_CHIPDRV_DisableIRQINT(shm_cb.p_hw->inter[SHM_DATA_WRITE].int_num);

	_CHIPDRV_DisableIRQINT(shm_cb.p_hw->inter[SHM_DATA_READ].int_num);

	SCI_FREE(shm_cb.write_info.tx_buf);
	SCI_FREE(shm_cb.read_info.rx_buf);

	shm_cb.write_info.tx_buf = NULL;
	shm_cb.read_info.rx_buf = NULL;
}

/*****************************************************************************/
// Description :    Arm9&Cm4 Share Memory Open.
// PUBLIC resource dependence :
// Author :
// Note :
/*****************************************************************************/
PUBLIC void Shm_Open()
{
	uint8 i = 0;

	for(i = 0; i < 2;i++)
	{
		SCI_MEMSET(shm_cb.p_hw->baseAddr[i],0,shm_cb.p_hw->max_size);
		SCI_MEMSET(shm_cb.p_hw->fifoSize[i],0,0x4);
	}

    	SCI_MEMSET(&shm_cb.write_info,0,sizeof(SHM_Write_INFO_S));
    	SCI_MEMSET(&shm_cb.read_info,0,sizeof(SHM_Read_INFO_S));

	shm_cb.write_info.tx_buf = SCI_ALLOC_APP(SHM_TX_BUF_MAX_SIZE);
	shm_cb.read_info.rx_buf = SCI_ALLOC_APP(SHM_RX_BUF_MAX_SIZE);

	if((NULL == shm_cb.write_info.tx_buf) || (NULL == shm_cb.read_info.rx_buf))
	{
		SCI_PASSERT(0,("bt share memory alloc fail!"));
	}

	SCI_MEMSET(shm_cb.write_info.tx_buf,0,SHM_TX_BUF_MAX_SIZE);
	SCI_MEMSET(shm_cb.read_info.rx_buf,0,SHM_RX_BUF_MAX_SIZE);

	shm_cb.read_info.b_need_rx_data = TRUE;

	shm_cb.read_info.shm_data_read_timer = SCI_CreatePeriodTimer ("SHMDATAREADTIMERDECT",
                            _SHM_DataReadTimerHandler,
                            0,
                            SHM_TIMER_INTERVAL,
                            SCI_NO_ACTIVATE);

	_CHIPDRV_EnableIRQINT(shm_cb.p_hw->inter[SHM_DATA_WRITE].int_num);

	_CHIPDRV_EnableIRQINT(shm_cb.p_hw->inter[SHM_DATA_READ].int_num);

}


/*****************************************************************************/
// Description :  the timer to check data full or not
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
LOCAL void _SHM_DataReadTimerHandler (uint32 state)
{
    	uint32 length = SHM_RX_BUF_MAX_SIZE - shm_cb.read_info.rx_numbers;
	if(*shm_cb.p_hw->fifoSize[SHM_DATA_READ] > length)
	{
		if(shm_cb.read_info.b_need_rx_data)
		{
			Shm_Send_Msg(SHM_DATA_READ);
			shm_cb.read_info.b_need_rx_data = FALSE;
		}
		return;
	}
	else
	{
		SCI_DeactiveTimer(shm_cb.read_info.shm_data_read_timer);
		ShmDrv_Rx();
	}
}

/*****************************************************************************/
// Description :  when int happened,the callback will be called
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
LOCAL uint32 OSShmDrv_EventCallBack(uint32 event)
{
    switch(event)
	{
	    case SHM_DATA_TO_WRITE:
		{
                        if(!__ShmTxIsBusy())
                        {
				__ShmTxData();
                        }
			break;
		}
		case SHM_DATA_TO_READ:
		{
			ShmDrv_Rx();
			if((shm_cb.read_info.b_need_rx_data)&&(shm_cb.read_info.rx_numbers>0))
			{
				Shm_Send_Msg(SHM_DATA_READ);
				shm_cb.read_info.b_need_rx_data = FALSE;
			}
	           break;
		}
		default:
		{
			break;
		}
	}

    return 0;
}

/*****************************************************************************/
// Description :  when write data int happened,first callback handle
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
static ISR_EXE_T Shm_Data_Write_ISR_Handle (uint32 param)
{
	 *(uint32*)(CLR_CM4_INT) |= CLR_CM4_TO_ARM9_IRQ_INT0;
	 shm_cb.write_info.tx_is_busy = SCI_FALSE;

    	TM_SendTestPointRequest ((uint32)ARM9_SHM_WRITE_START_ADDR,(uint32)SHM_DATA_WRITE);

	return CALL_HISR;
}

/*****************************************************************************/
// Description :  when read data int happened,first callback handle
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
static ISR_EXE_T Shm_Data_Read_ISR_Handle (uint32 param)
{
	*(uint32*)(CLR_CM4_INT) |= CLR_CM4_TO_ARM9_IRQ_INT1;

	TM_SendTestPointRequest ((uint32)ARM9_SHM_READ_START_ADDR,(uint32)SHM_DATA_READ);

	if(*shm_cb.p_hw->fifoSize[SHM_DATA_READ] > 0)
	{
		return CALL_HISR;
	}
	else
	{
		(*(volatile uint32*)(0x8B000160)) |= SET_ARM9_TO_CM4_IRQ_INT1;//enable arm9 write end irq int
		return ISR_DONE;
	}
}

/*****************************************************************************/
// Description :  when write data int happened,second callback handle
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
static void  ShmDataWriteHisrFunc (uint32 i, void *d)
{
	OSShmDrv_EventCallBack(SHM_DATA_TO_WRITE);
}

/*****************************************************************************/
// Description :  when read data int happened,second callback handle
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
static void  ShmDataReadHisrFunc (uint32 i, void *d)
{
	OSShmDrv_EventCallBack(SHM_DATA_TO_READ);
}

/*****************************************************************************/
// Description :  write data to share memory
// PUBLIC resource dependence :
// PARAMETERS: flag:continue write
// Author :
// Note :
/*****************************************************************************/
LOCAL void Arm9_Shm_Data_Write(uint8 *buf,uint32 length,BOOLEAN flag)
{
	SCI_MEMCPY(shm_cb.p_hw->baseAddr[SHM_DATA_WRITE],buf,length);

	if(!flag)
	{
		shm_cb.p_hw->baseAddr[SHM_DATA_WRITE] += length;
	}
	else
	{
		shm_cb.p_hw->baseAddr[SHM_DATA_WRITE] = (uint8 *)ARM9_SHM_WRITE_START_ADDR;
	}
}

/*****************************************************************************/
// Description :  read data to share memory
// PUBLIC resource dependence :
// PARAMETERS: flag:continue read
// Author :
// Note :
/*****************************************************************************/
LOCAL void Arm9_Shm_Data_Read(uint8 *buf,uint32 length,BOOLEAN flag)
{
	SCI_MEMCPY(buf,shm_cb.p_hw->baseAddr[SHM_DATA_READ],length);

	if(!flag)
	{
		shm_cb.p_hw->baseAddr[SHM_DATA_READ] += length;
	}
	else
	{
		shm_cb.p_hw->baseAddr[SHM_DATA_READ] = (uint8 *)ARM9_SHM_READ_START_ADDR;
	}

}

/*****************************************************************************/
// Description :  send msg to bt host
// PUBLIC resource dependence :
// PARAMETERS:
// Return:
// Author :
// Note :
/*****************************************************************************/
static uint32  s_shm_read_buff_timer = 0;

LOCAL void Shm_RxReTrig(uint16 a, void *data)
{
    if(OS_IsSchedRunning()
            && shm_cb.read_info.b_need_rx_data
            && (shm_cb.read_info.rx_numbers > 0))
    {
        SHM_LOG_TRACE(("Shm_RxReTrig, send msg to bT task   222..."));
        Shm_Send_Msg(SHM_DATA_READ);
	shm_cb.read_info.b_need_rx_data = FALSE;
    }
    s_shm_read_buff_timer = 0;
}

/*****************************************************************************/
// Description :  API interface,user can use to read data from cache
// PUBLIC resource dependence :
// PARAMETERS:buffer:to store read data;length:read length
// Return: return had read length
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint32 Shm_Read()//(uint8 *buffer,uint32 length)
{
    uint32 rx_ret = 0;

    SCI_ASSERT(shm_cb.read_info.rx_out_ptr <= SHM_RX_BUF_MAX_SIZE);/*assert verified*/

    SHM_LOG_TRACE(("Shm_Read, s_shm_rx_buf=%d, s_rx_numbers=%d", shm_cb.read_info.rx_out_ptr, shm_cb.read_info.rx_numbers ));

    if(shm_cb.read_info.rx_numbers  >= SHM_RX_BUF_MAX_SIZE)
    {
        SHM_LOG_TRACE(("[BT] critical error error error buffer over"));
    }

    if(shm_cb.read_info.rx_numbers  > 0)
    {
        uint32 total = shm_cb.read_info.rx_numbers ;
        uint32 to_send = total;

        do
        {
            uint8 *buf = &shm_cb.read_info.rx_buf[shm_cb.read_info.rx_out_ptr];
            uint32 rx_cnt = SHM_RX_BUF_MAX_SIZE - shm_cb.read_info.rx_out_ptr;

            if(rx_cnt > total)
            {
                rx_cnt = total;
            }

            rx_ret = uartReadData((char *)buf, rx_cnt);

            shm_cb.read_info.rx_out_ptr += rx_ret;

            if(shm_cb.read_info.rx_out_ptr >= SHM_RX_BUF_MAX_SIZE)
            {
                shm_cb.read_info.rx_out_ptr -= SHM_RX_BUF_MAX_SIZE;
            }

            total -= rx_ret;

        }
        while(total && rx_ret);

        SCI_DisableIRQ();
        shm_cb.read_info.rx_numbers  -= (to_send - total);
        SCI_RestoreIRQ();

        SHM_LOG_TRACE(("shm_Read, s_rx_numbers = %d,to_send=%d,total=%d,rx_ret=%d",
                        shm_cb.read_info.rx_numbers ,to_send, total,rx_ret));

    }

    shm_cb.read_info.b_need_rx_data = TRUE;

    if(OS_IsSchedRunning() && (shm_cb.read_info.rx_numbers > 0))
    {
        if (rx_ret)
        {
            SHM_LOG_TRACE(("shm_read: send msg to BT task.... 111"));
            Shm_Send_Msg(SHM_DATA_READ);
	    shm_cb.read_info.b_need_rx_data = FALSE;
        }
        else
        {
            SHM_LOG_TRACE(("shm_read: rx_ret==%d",rx_ret));
            if(0 == s_shm_read_buff_timer)
            {
                s_shm_read_buff_timer = timed_event_in(1000*50, Shm_RxReTrig, 0, NULL);
            }
        }
    }

    return 0;
}

#if 0
/*****************************************************************************/
// Description :  API interface,user can use to read data from cache
// PUBLIC resource dependence :
// PARAMETERS:buffer:to store read data;length:read length
// Return: return had read length
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint32 Shm_Read(uint8 *buffer,uint32 length)
{
	uint8 *buf = &shm_cb.read_info.rx_buf[shm_cb.read_info.rx_out_ptr];
	uint32 rx_cnt= 0;
	uint32 min_rx_len = 0;

	shm_cb.read_info.b_need_rx_data = TRUE;

	if(shm_cb.read_info.rx_numbers == 0)
		return min_rx_len;

	min_rx_len = (shm_cb.read_info.rx_numbers < length) ? shm_cb.read_info.rx_numbers : length;

	if((shm_cb.read_info.rx_out_ptr + min_rx_len) > SHM_RX_BUF_MAX_SIZE)
	{
		rx_cnt = SHM_RX_BUF_MAX_SIZE - shm_cb.read_info.rx_out_ptr;
		memcpy(buffer,buf,rx_cnt);
		memcpy(buffer+rx_cnt,&shm_cb.read_info.rx_buf[0],min_rx_len-rx_cnt);
	}
	else
	{
		memcpy(buffer,buf,min_rx_len);
	}

	shm_cb.read_info.rx_numbers -= min_rx_len;

	shm_cb.read_info.rx_out_ptr += min_rx_len;

	if(shm_cb.read_info.rx_out_ptr >= SHM_RX_BUF_MAX_SIZE)
	{
		shm_cb.read_info.rx_out_ptr -= SHM_RX_BUF_MAX_SIZE;
	}

	SCI_TRACE_LOW("BT Shm_Read s_shm_rx_numbers = 0x%x,s_shm_rx_out_ptr=0x%x,min_rx_len=0x%x",shm_cb.read_info.rx_numbers,shm_cb.read_info.rx_out_ptr,min_rx_len);
	return min_rx_len;
}
#endif

/*****************************************************************************/
// Description :  use to read data from share memory.
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
LOCAL void ShmDrv_Rx(void)
{
	uint8 *buf = &shm_cb.read_info.rx_buf[shm_cb.read_info.rx_in_ptr];
	uint32 rx_cnt = SHM_RX_BUF_MAX_SIZE - shm_cb.read_info.rx_in_ptr;
	uint32 length = 0;

	SCI_ASSERT(shm_cb.read_info.rx_in_ptr <= SHM_RX_BUF_MAX_SIZE);/*assert verified*/

	length = *shm_cb.p_hw->fifoSize[SHM_DATA_READ];

	SHM_LOG_TRACE(("BT ShmDrv_Rx length = 0x%x",length));

	if((shm_cb.read_info.rx_numbers + length) > SHM_RX_BUF_MAX_SIZE)
	{
		SCI_ChangeTimer (shm_cb.read_info.shm_data_read_timer, _SHM_DataReadTimerHandler, SHM_TIMER_INTERVAL);
		SCI_ActiveTimer(shm_cb.read_info.shm_data_read_timer);
		SHM_LOG_TRACE(("[BT] critical error error error data buffer over"));
		return;
	}

	if((shm_cb.read_info.rx_in_ptr + length) >  SHM_RX_BUF_MAX_SIZE)
	{
		Arm9_Shm_Data_Read((uint8 *)buf,rx_cnt,FALSE);
		Arm9_Shm_Data_Read((uint8 *)&shm_cb.read_info.rx_buf[0],length-rx_cnt,TRUE);
	}
	else
	{
		Arm9_Shm_Data_Read((uint8 *)buf,length,TRUE);
	}

#ifdef CHIP_VER_6531EFM
    {
        char *vendor_cmd_enable_evt  = "040e0601a1fc000001";
        char *vendor_cmd_disable_evt = "040e0601a1fc000000";
        char *reset_evt              = "040e0401030c00";
        char str_rcv[50] = {0};
        int  i = 0;

        for(i=0; i<14; i++)
            sprintf(str_rcv+(i*2), "%02x", buf[i]);


        if(!strncmp(str_rcv, vendor_cmd_enable_evt, strlen(vendor_cmd_enable_evt)))
        {
            BT_SetCmdEventFlag(BT_VENDOR_ENABLE_EVENT, TRUE);

        }

        if(!strncmp(str_rcv, reset_evt, strlen(reset_evt)))
        {
        BT_SetCmdEventFlag(BT_RESET_EVENT, TRUE);
        }

        if(!strncmp(str_rcv, vendor_cmd_disable_evt, strlen(vendor_cmd_disable_evt)))
        {
            BT_SetCmdEventFlag(BT_VENDOR_DISABLE_EVENT, TRUE);
        }
    }
#endif

	shm_cb.read_info.rx_in_ptr += length;

	shm_cb.read_info.rx_numbers += length;

	if(shm_cb.read_info.rx_in_ptr >= SHM_RX_BUF_MAX_SIZE)
	{
		shm_cb.read_info.rx_in_ptr -= SHM_RX_BUF_MAX_SIZE;
	}

	(*(volatile uint32*)(0x8B000160)) |= SET_ARM9_TO_CM4_IRQ_INT1;//enable arm9 write end irq int

	SHM_LOG_TRACE(("BT ShmDrv_Rx s_shm_rx_numbers = 0x%x,s_shm_rx_in_ptr=0x%x,length = 0x%x",shm_cb.read_info.rx_numbers,shm_cb.read_info.rx_in_ptr,length));
}

/*****************************************************************************/
// Description :  API interface,user can write data to cache
// PUBLIC resource dependence :
// PARAMETERS: buffer:write data;length:write length
// Return: 0:write fail,cache full need to send again.
// Author :
// Note :
/*****************************************************************************/
PUBLIC uint16 Shm_Write(const uint8 *buffer, uint16 length)
{
    uint16 written_len = 0;

    ShmDrv_Tx((char *)buffer, (uint16)length, &written_len);

    return written_len;
}

/*****************************************************************************/
// Description :  write data to cache
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
LOCAL BOOLEAN ShmDrv_Tx(char *buf, uint16 num_to_send, uint16 *numSend)
{
    uint8 *buf_ptr = (uint8 *)buf;
    uint32 length = (SHM_TX_BUF_MAX_SIZE - shm_cb.write_info.tx_in_ptr);
    uint8 *write_buf = &shm_cb.write_info.tx_buf[shm_cb.write_info.tx_in_ptr];

    SCI_ASSERT(SCI_NULL != buf_ptr);/*assert verified*/
    SCI_ASSERT(0 != num_to_send);/*assert verified*/
    SCI_ASSERT(shm_cb.write_info.tx_in_ptr <= SHM_TX_BUF_MAX_SIZE);/*assert verified*/

    if((shm_cb.write_info.tx_numbers + num_to_send) > SHM_TX_BUF_MAX_SIZE)
    {
        *numSend = 0;

	shm_cb.write_info.tx_is_full = TRUE;

        if(!__ShmTxIsBusy())
        {
            __ShmTxData();
        }

        return TRUE;
    }

    if((shm_cb.write_info.tx_in_ptr + num_to_send) <= SHM_TX_BUF_MAX_SIZE)
    {
        SCI_MEMCPY((uint8 *)write_buf, (uint8 *)buf_ptr, num_to_send);
    }
    else
    {
        SCI_MEMCPY((uint8 *)write_buf, (uint8 *)buf_ptr, length);
        SCI_MEMCPY((uint8 *)&shm_cb.write_info.tx_buf[0], (uint8 *)(buf_ptr + length), (num_to_send - length));
    }

    shm_cb.write_info.tx_in_ptr += num_to_send;

    if(shm_cb.write_info.tx_in_ptr >= SHM_TX_BUF_MAX_SIZE)
    {
        shm_cb.write_info.tx_in_ptr -= SHM_TX_BUF_MAX_SIZE;
    }

    *numSend = num_to_send;

    SCI_DisableIRQ();

    shm_cb.write_info.tx_numbers += num_to_send;

    if(!__ShmTxIsBusy())
    {
        __ShmTxData();
    }

    SCI_RestoreIRQ();

    return TRUE;
}

/*****************************************************************************/
// Description :  write busy or not
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
LOCAL BOOLEAN __ShmTxIsBusy(void)
{
    return  shm_cb.write_info.tx_is_busy;
}

/*****************************************************************************/
// Description :  write data to share memory
// PUBLIC resource dependence :
// PARAMETERS:
// Author :
// Note :
/*****************************************************************************/
LOCAL void __ShmTxData(void)
{
    uint32 min_send_len = 0;
    uint32 length = (SHM_TX_BUF_MAX_SIZE - shm_cb.write_info.tx_out_ptr);
    uint8 *buf = &shm_cb.write_info.tx_buf[shm_cb.write_info.tx_out_ptr];

    if(shm_cb.write_info.tx_numbers == 0)
    {
        return;
    }

    shm_cb.write_info.tx_is_busy = TRUE;

    min_send_len = (shm_cb.write_info.tx_numbers < SHM_FIFO_SIZE) ? shm_cb.write_info.tx_numbers : SHM_FIFO_SIZE;

    *shm_cb.p_hw->fifoSize[SHM_DATA_WRITE] = min_send_len;

    if((shm_cb.write_info.tx_out_ptr + min_send_len)  >=   SHM_TX_BUF_MAX_SIZE)
    {
         Arm9_Shm_Data_Write((uint8 *)buf, length,FALSE);
	 Arm9_Shm_Data_Write(&shm_cb.write_info.tx_buf[0], min_send_len-length,TRUE);
    }
    else
    {
        Arm9_Shm_Data_Write((uint8 *)buf, min_send_len,TRUE);
    }
    shm_cb.write_info.tx_out_ptr += min_send_len;

    if(shm_cb.write_info.tx_out_ptr >= SHM_TX_BUF_MAX_SIZE)
    {
	    shm_cb.write_info.tx_out_ptr -= SHM_TX_BUF_MAX_SIZE;
    }

    shm_cb.write_info.tx_numbers -= min_send_len;

    if(shm_cb.write_info.tx_is_full)
    {
	shm_cb.write_info.tx_is_full = FALSE;
	Shm_Send_Msg(SHM_DATA_WRITE);
    }

    (*(volatile uint32*)(0x8B000160)) |= SET_ARM9_TO_CM4_IRQ_INT0;//enable arm9 write end irq int

    SHM_LOG_TRACE(("BT __ShmTxData tx_out_ptr = 0x%x,tx_numbers=0x%x,min_send_len=0x%x,tx_is_full=0x%x",shm_cb.write_info.tx_out_ptr,shm_cb.write_info.tx_numbers,min_send_len,shm_cb.write_info.tx_is_full));

}

LOCAL void Shm_Send_Msg(SHM_CHANNEL_T channel)  //for shm recv
{
    OS_INT_MSG_T *msg_ptr = NULL;
    msg_ptr = (OS_INT_MSG_T *)SCI_ALLOC(sizeof(OS_INT_MSG_T));
    SCI_ASSERT(NULL != msg_ptr);/*assert verified*/


   switch(channel)
   {
	case SHM_DATA_WRITE:
		msg_ptr->message[0] = 0;
		msg_ptr->message[1] = OS_MSG_EXTEND_TRIG_DATA;
		msg_ptr->message[2] = channel;
		msg_ptr->message[3] = (SHM_TX_BUF_MAX_SIZE - shm_cb.write_info.tx_numbers);
		OS_SendMsgToSchedTask(OS_INT_HANDLE_MSG, msg_ptr);
		break;
	case SHM_DATA_READ:
		msg_ptr->message[0] = 0;
    		msg_ptr->message[1] = OS_MSG_EXTEND_TRIG_DATA;
		msg_ptr->message[2] = channel;
		msg_ptr->message[3] = shm_cb.read_info.rx_numbers;
		OS_SendMsgToSchedTask(OS_INT_HANDLE_MSG, msg_ptr);
		break;
	default:
		break;
   }

   SHM_LOG_TRACE(("BT Shm_Send_Msg 0x%x,0x%x,0x%x,0x%x",msg_ptr->message[0],msg_ptr->message[1],msg_ptr->message[2],msg_ptr->message[3]));

}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //

