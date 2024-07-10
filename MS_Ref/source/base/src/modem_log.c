/******************************************************************************
 ** File Name:      dsp_log.c                                                     *
 ** Author:         Fei Zhang                                              *
 ** DATE:           07/04/2011                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 serrial device. It manages create, send, receive          *
 **                 interface of serrial port.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 07/04/2010     fei.zhang        Add DSP USB Log function       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "sci_api.h"
#include "modem_log.h"
#include "chip_plf_export.h"
#include "ref_param.h"
#include "mem_prod.h"
#include "ref_outport.h"
#include "sfs.h"
#include "priority_system.h"
#include "load_modem.h"
#include "modem_log.h"
#include "cfw.h"

/**---------------------------------------------------------------------------*
 **                                                 Compiler Flag                                                               *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/*-----------------------------SIO DEBUG MACOR-------------------------------*/

#ifdef MODEM_LOG_TRA
#define SIO_MODEM_TRACE   SCI_TraceLow
#else
#define SIO_MODEM_TRACE(...)
#endif

#define  TASK_MODEM_CP_LOG_PRI   (PRI_APP)   //31

/*-----------------------------END--------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  Static Variables & functions
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **  external Variables
 **---------------------------------------------------------------------------*/
extern SCI_MUTEX_PTR   comm_mutex;
extern uint32 s_sio_tx_dma_is_start;
extern SIO_INFO_S      sio_port[MAX_LOGICAL_SIO_PORT_NUM];           // SIO information

PUBLIC uint32 SIO_GetTxFifoCnt (uint32 port);

/**---------------------------------------------------------------------------*
 **  Protocol functions
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **  User port variables & functions
 **---------------------------------------------------------------------------*/
#define MODME_LOG_COM_PORT      COM_DEBUG2
#define MODEM_LOG_USB_VIR_COM         VIR_COM4
#define SIO_MODEM_LOG_SEND_CP  0

#define MODEM_CP_LOG_MIN_SIZE  1024
#define MODEM_CP_LOG_MAX_SIZE  (16*1024)
#define MODEM_LOG_ALIGN_4   0xFFFFFFFC

#define MODEM_USB_DISCARD_THRESHOLD  100

#define MODEM_LOG_USB_TIMER             20
#define MODEM_LOG_SDC_TIMER             80

LOCAL uint32  v_sio_modem_log_timer_value = 0;


PUBLIC SCI_EVENT_GROUP_PTR modem_cp_log_evt = SCI_NULL;
LOCAL BLOCK_ID modem_cp_log_thread_id = SCI_INVALID_BLOCK_ID;

LOCAL uint16 modem_cp_logfile_num = 0, modem_cp_deletefile_num = 0;

LOCAL SFS_HANDLE  sfs_modem_cp_log = NULL;
PUBLIC T_TRA_MODEM_LOG * p_modem_cp_log_buff_info = NULL;


LOCAL uint8  v_sio_modem_log_tx_is_start = SCI_FALSE;
LOCAL uint8  v_sio_modem_cp_log_sdc_tx_is_start = SCI_FALSE;
LOCAL uint32  v_sio_modem_cp_log_lost_cnt = 0;
LOCAL uint8  v_sio_modem_log_usb_cp_threshold_cnt = 0;

LOCAL uint8  v_sio_modem_tx_type = SIO_MODEM_LOG_SEND_CP;  //0,send cp;1 send dsp log

#if 1//defined(MODEM_LOG)
LOCAL uint8   v_modem_debug_logic_port = 0xff;
LOCAL uint8   v_modem_debug_phy_port = SIO_MODEM_PORT_NULL;
LOCAL uint8   v_modem_debug_phy_port_flag = 0;
LOCAL uint16  cp_phy_port = SIO_MODEM_UART;
#ifdef CARD_LOG
LOCAL BOOLEAN  v_card_log_pause_flag = SCI_FALSE;
#endif
#endif


#ifdef MODEM_LOG_USB_ZERO_COPY

#define MODEM_LOG_USB_FINISH    0x10
#define MODEM_LOG_USB_WORK    0x1
#define MODEM_LOG_USB_IDLE    0x0
//0: usb-send is idle ; 1: usb-send  is working; 0x10: usb-send  has finished
//avoid the send api to be reentried in the critical region when not completed  the last time .
LOCAL uint8  v_sio_modem_log_usb_flag = MODEM_LOG_USB_IDLE;
LOCAL uint32  v_sio_modem_cp_log_sending_sz = 0;
LOCAL uint32  v_sio_modem_cp_log_sending_total_sz = 0;

#endif
LOCAL uint32  v_sio_modem_log_usb_fifo_full_cnt = 0;   // when usb fifo is full, and disacrd the log to free the log buffer


PUBLIC uint8  v_sio_modem_log_tx_status = SIO_MODEM_LOG_USB_TX_DISABLE;
PUBLIC uint8  v_sio_modem_log_sdc_tx_status = SIO_MODEM_LOG_SDC_TX_DISABLE;



static SCI_TIMER_PTR s_sio_modem_log_usb_check_timer = PNULL;

LOCAL void SIO_SendModemCpLog(void);

LOCAL void SIO_SdcSendModemCpLog(uint32 send_size);

extern uint32 UCOM_TxIsIdle(uint32 port_num);

#if 1//defined(MODEM_LOG)
PUBLIC int SIO_SendModemTrace(
    uint32 type
)
{
	//SCI_TraceLow("modemLog:type 0x%x", type);

     //if((0x1CCC == type) || (0x1DDD == type))
	  SIO_SendModemLogNotify();

	 return TRUE;
}

void SIO_CloseDskComNotifyModem(void)
{
	v_sio_modem_log_sdc_tx_status = SIO_MODEM_LOG_SDC_TX_DISABLE;
	SIO_ModemLogReset(0);
	if((DSK_COM0 == v_modem_debug_logic_port) || (DSK_COM1 == v_modem_debug_logic_port))
        SIO_SetModemTracePort(COM3);
}

void SIO_OpenDskComNotifyModem(void)
{
    SIO_SetModemTracePort(DSK_COM0);
	v_sio_modem_log_sdc_tx_status = SIO_MODEM_LOG_SDC_TX_ENABLE;
	SIO_ModemLogReset(1);
}


void SIO_CloseUsbComNotifyModem(void)
{

	v_sio_modem_log_tx_status = SIO_MODEM_LOG_USB_TX_DISABLE;
	SIO_ModemLogReset(0);
	SIO_SetModemTracePort(cp_phy_port);
    SIO_MDLogSendPutEvt();
}

void SIO_OpenUsbComNotifyModem(void)
{
    cp_phy_port = GetModemLogCpPhyPort();
	SIO_SetModemTracePort(SIO_MODEM_AP_USB);
    SIO_MDLogSendPutEvt();
	v_sio_modem_log_tx_status = SIO_MODEM_LOG_USB_TX_ENABLE;
	SIO_ModemLogReset(1);
}

void SIO_SetModemTracePort(uint8 port)
{
     if(v_modem_debug_phy_port != port)
     {

	    v_modem_debug_phy_port_flag = 1;  //port changed
	    v_modem_debug_phy_port = port;
        SCI_TraceLow("SIO_SetModemTracePort:phy port 0x%x", v_modem_debug_phy_port);

     }
}


uint8  SIO_GetModemTracePort(void)
{
    return v_modem_debug_phy_port;
}

uint8 SIO_ModemTracePortNotify(void)
{

    if(v_modem_debug_phy_port_flag)
    {
        v_modem_debug_phy_port_flag = 0;

        if(SIO_MODEM_UART == v_modem_debug_phy_port)
        {
            CFW_EmodSetTracePort(0);//call rpc api
        }
        else if(SIO_MODEM_AP_USB == v_modem_debug_phy_port) //usb port
        {
            CFW_EmodSetTracePort(5);//call rpc api
        }
        else if(SIO_MODEM_AP_DSK == v_modem_debug_phy_port) //sdcard port
        {
            CFW_EmodSetTracePort(6);//call rpc api
        }
        else if(SIO_MODEM_CLOSE == v_modem_debug_phy_port) //sdcard port
        {
            CFW_EmodSetTracePort(2);//call rpc api
        }
        else
        {
            CFW_EmodSetTracePort(0);//call rpc api
        }

        SCI_TraceLow("SIO_ModemTracePortNotify:phy port 0x%x", v_modem_debug_phy_port);
    }

    return v_modem_debug_phy_port;
}



void SIO_SendRemainCPLog (void)
{
    uint32 phy_port;
    uint32 i;
    uint8 *p_head=NULL;
    uint32 v_readIndex=0;
    T_TRA_MODEM_LOG * p_log_buff_info = NULL;

    if (SIO_ALREADY_OPEN != sio_port[COM_DEBUG2].open_flag)
    {
        SCI_TraceLow("[USB]: SIO_ALREADY_OPEN\n");
    }

    phy_port = SIO_PhyPortfromIndex (sio_port[COM_DEBUG2].phy_port);

	p_log_buff_info = p_modem_cp_log_buff_info;

	if( p_log_buff_info)
	{
		if(MODEM_LOG_BUFF_OK == p_log_buff_info->status)
		 {
			 i = 0;
			 p_head = (uint8 *)(p_log_buff_info->head);
			 v_readIndex= ((p_log_buff_info->rdIndex) & (p_log_buff_info->length - 1));
			 while(( i < p_log_buff_info->length) && (v_readIndex != p_log_buff_info->wrIndex))
			 {
				 (sio_port[COM_DEBUG2].sio_op.put_char) (phy_port, *(uint8 *)(p_head + v_readIndex));
				 v_readIndex= ((p_log_buff_info->rdIndex + i) & (p_log_buff_info->length - 1));
				 i++;
			 }
		}
	}

    (sio_port[COM_DEBUG2].sio_op.flush) (phy_port);
}


#ifdef CARD_LOG
void SIO_SetCardLogPauseFlag(BOOLEAN flag)
{
     v_card_log_pause_flag = flag;
}

BOOLEAN SIO_GetCardLogPauseFlag(void)
{
     return v_card_log_pause_flag;
}
#endif

#endif

void SIO_MDLogSendPutEvt(void)
{
    if(modem_cp_log_evt)
    {
	    SCI_SetEvent(modem_cp_log_evt,1,SCI_OR);	//	set event
    }
}

LOCAL uint32 SIO_GetModemLogSize (void *p_ctrl_info)
{
	T_TRA_MODEM_LOG *p_tra_info = (T_TRA_MODEM_LOG *)p_ctrl_info;
	uint32 v_len = 0;

	//bug1112466, assure the rdIdex is 4Bytes alignment
       if((p_tra_info->rdIndex) & (~(MODEM_LOG_ALIGN_4)))
       {
  		SCI_TraceLow("modemLog:rdInx unaligned 4B,port %d, head 0x%x, wr0x%x,rd0x%x,ofi0x%x",SIO_GetModemTracePort(),p_tra_info->head,p_tra_info->wrIndex,p_tra_info->rdIndex,p_tra_info->overflowIndex);
       		p_tra_info->rdIndex = p_tra_info->rdIndex & MODEM_LOG_ALIGN_4;
    	}

	if(p_tra_info->wrIndex >= p_tra_info->rdIndex)
		return  ((p_tra_info->wrIndex -  p_tra_info->rdIndex) & MODEM_LOG_ALIGN_4);
	else
	{
		v_len = (p_tra_info->overflowIndex - p_tra_info->rdIndex);
		// avoid the tail log size less than 4
		if(v_len < 4)
		{
			return v_len;
		}
		else
			return (v_len & MODEM_LOG_ALIGN_4);
	}
}

LOCAL uint32 SIO_GetModemLogTotalSize (void *p_ctrl_info)
{
	T_TRA_MODEM_LOG *p_tra_info = (T_TRA_MODEM_LOG *)p_ctrl_info;

	 if(p_tra_info->wrIndex >= p_tra_info->rdIndex)
            return  p_tra_info->wrIndex -  p_tra_info->rdIndex;
        else
            return p_tra_info->length- p_tra_info->rdIndex +  p_tra_info->wrIndex;
}


LOCAL void SIO_ModemCpLogThread (uint32 id, void *pdata)
{
    uint32	v_log_size,v_len,v_i,v_sended_size;
    T_TRA_MODEM_LOG * p_log_buff_info =NULL;
    T_TRA_MODEM_LOG * p_dsp_log_buff_info =NULL;
	uint32 actual_event = 0;
    int time = 0;


    for (; ;)
    {
		SCI_GetEvent(modem_cp_log_evt, 1, SCI_AND_CLEAR, &actual_event, SCI_WAIT_FOREVER);

        if( FALSE == drvUsbIsConnected() )
        {
            for(time=0;time<100;time++)
            {
                SCI_SLEEP(30);
                if( TRUE == drvUsbIsConnected() )
                {
                    break;
                }
            }

            if(FALSE == drvUsbIsConnected())
                {
                    SIO_ModemLogReset(0);
                    SIO_SetModemTracePort(cp_phy_port);
                    SIO_ModemTracePortNotify();
                }
        }
        else
        {
            if(NULL == p_modem_cp_log_buff_info)
            {
                p_modem_cp_log_buff_info = (T_TRA_MODEM_LOG *)GET_MemoryStartAddressAndLen(MEM_CP_DEBUG_NAME,&v_len);
            }
    	 	p_log_buff_info = p_modem_cp_log_buff_info;

    		if((NULL == p_log_buff_info) || (MODEM_LOG_BUFF_OK != p_log_buff_info->status))
    		{
    			SCI_TraceLow("modemLog:USB send,cp buff is uninitialized");

    		}
    	 	else
            {
    	        SIO_ModemTracePortNotify();
    			SIO_SendModemCpLog();
    	 	}
        }
    }
}


/*****************************************************************************/
// Description:
// Global resource dependence:
// Author:
// Note:
/*****************************************************************************/
LOCAL void SIO_ModemLogUsbCheckTimerExpired(void)
{
	SIO_MODEM_TRACE("modemLog:timer, CP wr0x%x,rd0x%x",p_modem_cp_log_buff_info->wrIndex,p_modem_cp_log_buff_info->rdIndex);

	//fix the converity38990. keep the original processing  and remove the length contraint
      //if((SIO_GetModemLogTotalSize((void*)p_modem_cp_log_buff_info) >= MODEM_CP_LOG_MIN_SIZE)  || \
	//  	(SIO_GetModemLogTotalSize((void*)p_modem_dsp_log_buff_info) >= MODEM_DSP_LOG_MIN_SIZE) )
	//{
		SIO_MDLogSendPutEvt();
      	//}


}


/*****************************************************************************/
// Description: usb send cp log api
// Global resource dependence:
// Author:
// Note:
/*****************************************************************************/
LOCAL void SIO_SendModemCpLog(void)
{
	uint32 v_log_size=0;
	uint32 v_sended_size=0;
	uint32 v_fifo_size=0;
	uint32 v_sending_size = 0;
	uint32 v_send_flag=0;
	T_TRA_MODEM_LOG * p_log_buff_info =p_modem_cp_log_buff_info;

	if(SIO_MODEM_LOG_USB_TX_DISABLE == v_sio_modem_log_tx_status)
	 {
	 	return;
	}


	if((NULL == p_log_buff_info) || (MODEM_LOG_BUFF_OK != p_log_buff_info->status))
	{
		SCI_TraceLow("modemLog:USB send,cp buff is uninitialized");
		return;
	}


	SCI_DisableIRQ();

	if(SCI_TRUE == v_sio_modem_log_tx_is_start)
	{
	   SCI_RestoreIRQ();
	   return;
	}

	v_sio_modem_log_tx_is_start = SCI_TRUE;
	SCI_RestoreIRQ();

	{
		p_log_buff_info = p_modem_cp_log_buff_info;

		SCI_DisableIRQ();

		if(MODEM_LOG_USB_WORK == v_sio_modem_log_usb_flag)
		{
			v_sio_modem_log_usb_fifo_full_cnt++;  // to disard the log and free the log buffer
			if(v_sio_modem_log_usb_fifo_full_cnt < MODEM_USB_DISCARD_THRESHOLD)
			{
				v_sio_modem_log_tx_is_start = SCI_FALSE;
				SCI_RestoreIRQ();
				return;
			}
			else
			{
				SCI_TraceLow("modemLog:USB send cp  log timerout and fail ");
			}
		}

		v_sio_modem_log_usb_fifo_full_cnt=0;
		v_sio_modem_log_usb_flag = MODEM_LOG_USB_WORK;
		SCI_RestoreIRQ();

		p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_sio_modem_cp_log_sending_sz) & (p_log_buff_info->length - 1);

		if(!v_sio_modem_cp_log_sending_total_sz)
			v_sio_modem_cp_log_sending_total_sz = SIO_GetModemLogSize((void*)p_log_buff_info);

		//avoid the v_sio_modem_cp_log_sending_total_sz /  MODEM_CP_LOG_MAX_SIZE  remainder is less than  MODEM_CP_LOG_MIN_SIZE.
		//Distinguish between the first size  and "v_sio_modem_cp_log_sending_total_sz /  MODEM_CP_LOG_MAX_SIZE  remainder" less than MODEM_CP_LOG_MIN_SIZE
		if(v_sio_modem_cp_log_sending_total_sz > (MODEM_CP_LOG_MAX_SIZE + MODEM_CP_LOG_MIN_SIZE))
			v_sio_modem_cp_log_sending_sz = MODEM_CP_LOG_MAX_SIZE;
		else
			v_sio_modem_cp_log_sending_sz = v_sio_modem_cp_log_sending_total_sz;


		v_sending_size = v_sio_modem_cp_log_sending_sz;
		if((v_sending_size > MODEM_CP_LOG_MIN_SIZE) || (p_log_buff_info->rdIndex > p_log_buff_info->wrIndex))
		{
			v_send_flag = 1;
			v_sio_modem_log_usb_cp_threshold_cnt=0;
		}
		else
		{
			v_send_flag=0;
			if(v_sending_size)
				v_sio_modem_log_usb_cp_threshold_cnt++;

			if(v_sio_modem_log_usb_cp_threshold_cnt > 3)
			{
				v_send_flag = 1;
				v_sio_modem_log_usb_cp_threshold_cnt = 0;
			}
			else
			{
				v_sio_modem_cp_log_sending_sz = 0;
				v_sio_modem_cp_log_sending_total_sz = 0;
			}

		}


		if(v_send_flag)
		{
			v_sended_size = ((sio_port[MODME_LOG_COM_PORT].sio_op.write) (SIO_PhyPortfromIndex (MODEM_LOG_USB_VIR_COM), \
					(uint8 *)( p_log_buff_info->rdIndex+p_log_buff_info->head),v_sending_size));
			SIO_MODEM_TRACE ("modemLog:USB send cp log,rd 0x%x,sending 0x%x,sended 0x%x,total 0x%x",p_log_buff_info->rdIndex,v_sending_size,v_sended_size,v_log_size);

			if(v_sended_size)
			{
				v_sio_modem_cp_log_sending_sz = v_sended_size;
				v_sio_modem_cp_log_sending_total_sz -= v_sended_size;
				SCI_DisableIRQ();
				v_sio_modem_log_tx_is_start = SCI_FALSE;
				SCI_RestoreIRQ();
			}
			else
			{
			       // USB abnormal,discard the log and update the read inedx with v_log_size
			       v_sending_size = v_sio_modem_cp_log_sending_total_sz;
			       p_log_buff_info->rdIndex =  (p_log_buff_info->rdIndex + v_sending_size) & (p_log_buff_info->length - 1);
		                v_sio_modem_cp_log_sending_sz = 0; // clear sending size.
		                v_sio_modem_cp_log_sending_total_sz = 0;
				v_sio_modem_cp_log_lost_cnt++;
				SCI_DisableIRQ();
				v_sio_modem_log_tx_is_start = SCI_FALSE;
				v_sio_modem_log_usb_flag = MODEM_LOG_USB_IDLE;
				SCI_RestoreIRQ();
				SCI_TraceLow("modemLog:USB send CP log fail,lost 0x%x,wr0x%x,rd0x%x,ofi0x%x",(v_sending_size),p_log_buff_info->wrIndex,p_log_buff_info->rdIndex,p_log_buff_info->overflowIndex);
			}
		}
		else
		{
			SCI_DisableIRQ();
		 	v_sio_modem_log_tx_is_start = SCI_FALSE;
			v_sio_modem_log_usb_flag = MODEM_LOG_USB_IDLE;
			SCI_RestoreIRQ();
		}
	  }
}


/*****************************************************************************/
// Description:callback to send the  the dsp log and cp log with the same usb port
// Global resource dependence:
// Author:
// Note:
/*****************************************************************************/
PUBLIC void SIO_ModemLogUsbSendCallBack(void)
{
	uint32 v_log_size=0,v_sended_size;
	T_TRA_MODEM_LOG * p_log_buff_info =NULL;


	if(NULL == p_modem_cp_log_buff_info)
	{
		SIO_MDLogSendPutEvt();
	    return;
	}


	SCI_DisableIRQ();
	v_sio_modem_log_usb_flag = MODEM_LOG_USB_FINISH;
	SCI_RestoreIRQ();

	SIO_SendModemCpLog();


	return;
}


/*****************************************************************************/
// Description: CP notify AP send log with mailbox irq
// Global resource dependence:
// Author:
// Note:
/*****************************************************************************/
PUBLIC void SIO_SendModemLogNotify(void)
{
	uint8 v_port;
	uint32	v_log_size,v_len,v_i,v_sended_size;

	v_port =  SIO_GetModemTracePort();

	if(NULL == p_modem_cp_log_buff_info)
	{
        SIO_MDLogSendPutEvt();
	    return;
	}

	SIO_SendModemCpLog();
}

//status: 0--disable usb tx 1--enable usb tx
PUBLIC void SIO_ModemLogReset(uint8 status)
{
	uint32 v_len;

	if(status)
	{
		SIO_MODEM_TRACE ("modemLog:enable reset");
		if (PNULL == s_sio_modem_log_usb_check_timer)
		{
			v_sio_modem_log_timer_value = MODEM_LOG_USB_TIMER;
			s_sio_modem_log_usb_check_timer = SCI_CreatePeriodTimer("MdLogUsbCheck",
		                                 SIO_ModemLogUsbCheckTimerExpired, 0, MODEM_LOG_USB_TIMER, SCI_AUTO_ACTIVATE);
		}
    }
    else
    {
		SIO_MODEM_TRACE ("modemLog:disable reset");
		if(s_sio_modem_log_usb_check_timer)
		{
			SCI_DeleteTimer(s_sio_modem_log_usb_check_timer);
			s_sio_modem_log_usb_check_timer = PNULL;
		}
	 }


	if(NULL == p_modem_cp_log_buff_info)
	{
		p_modem_cp_log_buff_info = (T_TRA_MODEM_LOG *)GET_MemoryStartAddressAndLen(MEM_CP_DEBUG_NAME,&v_len);
	}

	p_modem_cp_log_buff_info->rdIndex = ( p_modem_cp_log_buff_info->rdIndex + 4) & (p_modem_cp_log_buff_info->length - 1);

}

//get phy port with cp defined port
PUBLIC uint16 GetModemLogCpPhyPort()
{
    uint32 v_len;
    uint16 port;

    p_modem_cp_log_buff_info = (T_TRA_MODEM_LOG *)GET_MemoryStartAddressAndLen(MEM_CP_DEBUG_NAME,&v_len);

    if(p_modem_cp_log_buff_info->port == 0)
    {
        port = SIO_MODEM_UART;
    }
    else if(p_modem_cp_log_buff_info->port == 5)
    {
        port = SIO_MODEM_AP_USB;
    }
    else if(p_modem_cp_log_buff_info->port == 6)
    {
        port = SIO_MODEM_AP_DSK;
    }
    else if(p_modem_cp_log_buff_info->port == 2)
    {
        port = SIO_MODEM_CLOSE;
    }
    else
    {
        port = SIO_MODEM_UART;
    }

    return port;

}



/*****************************************************************************/
// Description: Dsp log task creat
// Global resource dependence: dsp_log_semaphore, dsp_log_thread_id, s_sio_dsp_frame_ctrl
// Author: fei.zhang
// Note:
/*****************************************************************************/

PUBLIC void SIO_CreateModemLogThread (void)
{

    if (SCI_NULL == modem_cp_log_evt)
    {
        modem_cp_log_evt = SCI_CreateEvent ("MODEM_CP_LOG_EVT");
    }

    SCI_PASSERT ( (modem_cp_log_evt != SCI_NULL), ("Create MODEM_CP_LOG_EVT failed!"));/*assert verified*/


    if (SCI_INVALID_BLOCK_ID == modem_cp_log_thread_id)
    {
        modem_cp_log_thread_id = SCI_CreateThread ("MODEM_CP_LOG_Task", "MODEM_CP_LOG_QUEUE",
                                              SIO_ModemCpLogThread/*void(* entry)(uint32,void *)*/,
                                              0, NULL, 2048, 1,
                                              TASK_MODEM_CP_LOG_PRI,  // same priority as sme
                                              SCI_PREEMPT,
                                              SCI_AUTO_START);
    }

    SCI_PASSERT ( (modem_cp_log_thread_id != SCI_INVALID_BLOCK_ID), ("Create MODEM CP LOG task failed!"));/*assert verified*/

    SCI_TRACE_LOW ("!!!Create SIO_ModemCpLogThread successfully!!!");

}

void SIO_Set_cp_stop_log(void)
{
    ipc_notify_cp_stop_log(1);
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif /* End of modem_log.c*/
