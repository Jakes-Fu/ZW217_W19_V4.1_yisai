/******************************************************************************
 ** File Name:      tp_multi_srv.c                                              *
 ** DATE:           2011.03.19                                                *
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **  2011.03.19                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                             Dependencies                                  *
 **---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "sci_types.h"
#include "tp_srv.h"
#include "tp_api.h"
#include "tasks_id.h"
#include "threadx_os.h"
#include "dal_lcd.h"
#include "tp_multi_drv.h"
#include "tp_multi_cfg.h"
#include "gpio_drv.h"
#include "os_apiext.h"
#include "dal_keypad.h"
#include "gpio_prod_api.h"
#include "diag.h"
/**---------------------------------------------------------------------------*
 **                            Compiler Flag                                  *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------*
 **                           Macros & Enums                                  *
 **---------------------------------------------------------------------------*/
//#define 	TP_MSG_MAX_NUM     		   (TP_MSG_MAX & 0x00FF)
#define TP_SYNC_EVENT              1
#define TP_TIMEOUT_INTERVAL        2000
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/
/*only valid for resistor tp now,
  for cap tp, we should add more info here*/
typedef struct
{
    TP_MSG_E    TP_type;
    uint16      TP_x;
    uint16      TP_y;
} TP_COMMON_T, *TP_COMMON_T_PTR;

/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/
LOCAL TP_INFO_T     s_tp_info_t = {0};
LOCAL TPDSVR_SIG_T  s_tp_sig;
LOCAL SCI_EVENT_GROUP_PTR  s_tp_event = PNULL;
/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:     TOUCHPANEL_default
//  Description:  initialize touach screen driver ,include GPIO/NV/TIMER
//  return:
//  Note:
/*****************************************************************************/
LOCAL void TOUCHPANEL_default( BLOCK_ID id, uint32 argc, void *argv )
{
}

/*****************************************************************************/
//  Function name:  TP_Hit
//  Description:
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL uint32 _TP_Hit (
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    TP_COMMON_T     *comm_ptr;

    comm_ptr = (TP_COMMON_T *) (src_ptr + sizeof (MSG_HEAD_T));
    // Build response to channel server
    *dest_ptr = SCI_ALLOC_APP (sizeof (MSG_HEAD_T));

    *dest_len_ptr = sizeof (MSG_HEAD_T);
    memcpy (*dest_ptr, src_ptr, sizeof (MSG_HEAD_T));
    ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;

    // decode COMM_TP_T to TPDSVR_SIG_T
    memset(&s_tp_sig,0, sizeof(s_tp_sig));
    s_tp_sig.SignalCode   = comm_ptr->TP_type;
    s_tp_sig.x_key        = comm_ptr->TP_x;
    s_tp_sig.y_key        = comm_ptr->TP_y;
    s_tp_sig.num_of_point = 1; //one point

    //SCI_TRACE_LOW:"SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_MULTI_SRV_97_112_2_18_0_35_5_2092,(uint8*)"ddd",s_tp_sig.SignalCode, s_tp_sig.x_key, s_tp_sig.y_key);
    SCI_SendEventToClient (TP_SERVICE, s_tp_sig.SignalCode, (void *) &s_tp_sig);

    return SCI_TRUE;
}

/******************************************************************************
 * THREAD_ENTRY
 ******************************************************************************/
THREAD_ENTRY (P_TP)/*lint -esym(765,thread_entry_P_TP)*/
{

#ifndef WIN32
	TPD_TO_TPSVR_MSG_T *tp_from_tpd_sig_ptr = SCI_NULL;
    SCI_TRACE_LOW("TPC INIT\r\n");
    SCI_CreateClientList(TP_SERVICE, (TP_MSG_MAX_NUM & 0x0ff), \
                                        (REG_CALLBACK)TOUCHPANEL_default );
    if(PNULL == s_tp_event)
    {
        s_tp_event = SCI_CreateEvent("TP_Event");
        if(PNULL == s_tp_event)
        {
            SCI_TRACE_LOW("SCI_CreateEvent TP_Event error\n");
        }
    }

    TPC_Init();

    DIAG_RegisterCmdRoutine (DIAG_TP_F,  _TP_Hit);

#endif

    while(1)/*lint !e716*/
    {
#ifndef WIN32
        // Receive signal.
        tp_from_tpd_sig_ptr = (TPD_TO_TPSVR_MSG_T  *)SCI_GetSignal(P_TPP);//SendSignal is in interrupt handler.
           SCI_TRACE_LOW("GetSignal(P_TPP)");
        if( (tp_from_tpd_sig_ptr->tpd_msg_valu == TP_ENTER_INTO_GPIO_HANDLER)
            && (tp_from_tpd_sig_ptr->SignalCode == TP_ENTER_INTO_GPIO_HANDLER) )
        {
            HandleTPMessage();
        }
        else if((tp_from_tpd_sig_ptr->tpd_msg_valu == TP_ENTER_ENABLE)
            && (tp_from_tpd_sig_ptr->SignalCode == TP_ENTER_ENABLE))
        {
            TPC_Resume();
            SCI_SetEvent(s_tp_event, TP_SYNC_EVENT, SCI_OR);
            GPIO_TPIRQCtrl(SCI_TRUE);
        }
        else if((tp_from_tpd_sig_ptr->tpd_msg_valu == TP_ENTER_DISABLE)
            && (tp_from_tpd_sig_ptr->SignalCode == TP_ENTER_DISABLE))
        {
            GPIO_TPIRQCtrl(SCI_FALSE);
            TPC_Suspend();
            SCI_SetEvent(s_tp_event, TP_SYNC_EVENT, SCI_OR);
        }
        else if((tp_from_tpd_sig_ptr->tpd_msg_valu == TP_ENTER_CMD)
            && (tp_from_tpd_sig_ptr->SignalCode == TP_ENTER_CMD))
        {
            GPIO_TPIRQCtrl(SCI_FALSE);
            TPC_Ioctl(tp_from_tpd_sig_ptr->cmd, tp_from_tpd_sig_ptr->arg);
            GPIO_TPIRQCtrl(SCI_TRUE);
        }
        SCI_FREE(tp_from_tpd_sig_ptr);
    }
#else
        tpc_total_data = (TPDSVR_SIG_T *)SCI_GetSignal(P_TPP);
        SCI_SendEventToClient(TP_SERVICE, tpc_total_data->SignalCode, (void *)tpc_total_data);  //20050625
#endif
}

PUBLIC void TP_Wait_SYNC_Event()
{
    uint32 event_flag = 0;
    //SCI_TRACE_LOW("TP_Wait_SYNC_Event");
    SCI_GetEvent(s_tp_event, TP_SYNC_EVENT, SCI_OR_CLEAR, &event_flag, TP_TIMEOUT_INTERVAL);
    //SCI_TRACE_LOW("TP_Wait_SYNC_Event end");
}

LOCAL TPD_TO_TPSVR_MSG_T* TP_Alloc_SignalBuffer()
{
        TPD_TO_TPSVR_MSG_T *tp_to_tpsvr_ptr    =  SCI_NULL;
        tp_to_tpsvr_ptr = (TPD_TO_TPSVR_MSG_T *)SCI_ALLOCA(sizeof(TPD_TO_TPSVR_MSG_T));


       SCI_ASSERT(SCI_NULL != tp_to_tpsvr_ptr);   /*assert verified*/

        tp_to_tpsvr_ptr->SignalSize = sizeof(TPD_TO_TPSVR_MSG_T);
        tp_to_tpsvr_ptr->Sender = P_TPP;

        return (TPD_TO_TPSVR_MSG_T*)tp_to_tpsvr_ptr;

}

PUBLIC void TP_Enable()
{
        TPD_TO_TPSVR_MSG_T *tp_to_tpsvr_sig_ptr    =  SCI_NULL;

        tp_to_tpsvr_sig_ptr = TP_Alloc_SignalBuffer();

        tp_to_tpsvr_sig_ptr->SignalCode = TP_ENTER_ENABLE;
        tp_to_tpsvr_sig_ptr->tpd_msg_valu = TP_ENTER_ENABLE;

        SCI_SendSignal((xSignalHeader)tp_to_tpsvr_sig_ptr, P_TPP);


}

/*****************************************************************************/
//  Function name:	TP_Disable
//  Description:
//  Global resource dependence:
//  Note:
/*****************************************************************************/
PUBLIC void TP_Disable()
{
    TPD_TO_TPSVR_MSG_T *tp_to_tpsvr_sig_ptr    =  SCI_NULL;

    tp_to_tpsvr_sig_ptr = TP_Alloc_SignalBuffer();

    tp_to_tpsvr_sig_ptr->SignalCode = TP_ENTER_DISABLE;
    tp_to_tpsvr_sig_ptr->tpd_msg_valu = TP_ENTER_DISABLE;

    SCI_SendSignal((xSignalHeader)tp_to_tpsvr_sig_ptr, P_TPP);
}

/*****************************************************************************/
//  Function name:	TP_Ioctl
//  Description:
//  Global resource dependence:
//  Note:
/*****************************************************************************/
PUBLIC void TP_Ioctl(uint32 cmd, void *arg)
{
    TPD_TO_TPSVR_MSG_T *tp_to_tpsvr_sig_ptr    =  SCI_NULL;

    tp_to_tpsvr_sig_ptr = TP_Alloc_SignalBuffer();

    tp_to_tpsvr_sig_ptr->SignalCode = TP_ENTER_CMD;
    tp_to_tpsvr_sig_ptr->tpd_msg_valu = TP_ENTER_CMD;
    tp_to_tpsvr_sig_ptr->cmd = cmd;
    tp_to_tpsvr_sig_ptr->arg = arg;

    SCI_SendSignal((xSignalHeader)tp_to_tpsvr_sig_ptr, P_TPP);
}

PUBLIC uint32 get_app_queue_available(void)
{
    uint32          status              = 0;
    uint32          count               = 0;
    uint32          enqueued            = 0;
    uint32          store               = 0;
    char            *queue_name_ptr     = SCI_NULL;
    TX_THREAD       * suspended_thread  = SCI_NULL;
    TX_QUEUE        * next_queue        = SCI_NULL;
    SCI_THREAD_T    * thread_block  = SCI_NULL;

    thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(P_APP);
    status = tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
                                &queue_name_ptr,
                                (uint32 *)&enqueued,
                                (uint32 *)&store,
                                (TX_THREAD **)&suspended_thread,
                                (uint32 *)&count,
                                (TX_QUEUE **)&next_queue);
      return store;
}

/******************************************************************************
 * TP_GetInfo
 * This function isn't used in multi-TP, because Cap-touchscreen doesn't
    calibrate and doesn't get size of LCD.
 ******************************************************************************/
PUBLIC TP_INFO_T *TP_GetInfo (void)
{
    LCD_INFO_T lcd_info;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    SCI_ASSERT(lcd_info.lcd_width&&lcd_info.lcd_height);   /*assert verified*/

    s_tp_info_t.tp_width      = lcd_info.lcd_width;        //TP_WIDTH;
    s_tp_info_t.tp_height     = lcd_info.lcd_height+0XA0;  //TP_HEIGHT;
    s_tp_info_t.tp_lcd_height = lcd_info.lcd_height;       //TP_HEIGHT_LCD;

    s_tp_info_t.adc_min_x     = 0XA0;
    s_tp_info_t.adc_min_y     = 0XA0;
    s_tp_info_t.adc_max_x     = 0XA0;
    s_tp_info_t.adc_max_y     = 0XA0;

    s_tp_info_t.exchange_x    = 0XA0;

    s_tp_info_t.exchange_y    = 0XA0;

    return &s_tp_info_t;
}

/******************************************************************************
 * TP_SetRawDataMode
 ******************************************************************************/
PUBLIC void TP_SetRawDataMode (BOOLEAN is_raw_mode)
{

}

/******************************************************************************
 * TP_UpdateCalibration
 ******************************************************************************/
PUBLIC BOOLEAN TP_UpdateCalibration (TP_CALIBRATION_T *param)
{
    return SCI_SUCCESS;
}

#ifdef __cplusplus
}
#endif

/******************************************************************************/


