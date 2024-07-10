#include "ms_ref_base_trc.h"
#include "tp_srv_entry.h"
#include "threadx_os.h"
#ifndef WIN32
#include "power.h"
//#include "mn_type.h"
//#include "mmi_nv.h"
extern void IsGpioMessage(void);

extern void TP_DInit(void);
#endif

#define TPD_SRV_DEBUG
#ifdef TPD_SRV_DEBUG
    #define TPD_SRV_PRINT	SCI_TRACE_LOW
#else
    #define TPD_SRV_PRINT	
#endif

/**---------------------------------------------------------------------------*/
//                            Global Function Definitions
/**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  FUNCTION:     TOUCHPANEL_default
//  Description:    initialize touach panel driver ,include GPIO/NV/TIMER
//  return:     
//  Author:         
//  date:  2005.05.11
//	Note:        
/*****************************************************************************/
LOCAL void TOUCHPANEL_default( BLOCK_ID id, uint32 argc, void *argv )
{
}


/*****************************************************************************/
//  FUNCTION:     TouchPanel_Init
//  Description:    initialize touach panel driver ,include GPIO/NV/TIMER
//  return:     
//  Author:         
//  date:  2005.05.11
//	Note:        
/*****************************************************************************/
PUBLIC void TouchPanel_Init(void)
{
    SCI_CreateClientList(TP_SERVICE, (TP_MSG_MAX_NUM & 0x0ff), 
        (REG_CALLBACK)TOUCHPANEL_default );
    
    #ifndef WIN32
    TP_DInit();
    #endif
}
/*****************************************************************************/
//  Description:    This function is used to Calculate touch panel screen posite from ADC convertor valu
//  Author:           manliang.yang
//  Date:              2005.05.11 
//	Note:             to be sure that the Calculate data is effective,so need't judge denominator is zreo or not
/*****************************************************************************/
LOCAL BOOLEAN TPIsMsReady(void)
{
    BOOLEAN status = SCI_TRUE;
#ifndef WIN32    
    POWER_MS_STATUS_E   ms_state;

    // Get the MS current state.
    ms_state = POWER_GetMsStatus();
    
    if ((POWER_MS_STATUS_POWER_ON == ms_state) || (POWER_MS_STATUS_POWER_OFF == ms_state) )
    {
        status = SCI_TRUE;
    }
    else
    {
        status = SCI_FALSE;
    }
#endif    
    return status;
}

/*****************************************************************************/
//  Description:    get the app queue's avilable queue,
//  parameter:  none(may be the task id, it can be update in future)
//  return value: the avilable queue's num
//  Author:         yingchun.li
//	Note:           
/*****************************************************************************/

PUBLIC uint32 get_app_queue_avilable(void)
{
       uint32          status              = 0;
	uint32          count               = 0;
	uint32          enqueued            = 0;
	uint32          store               = 0;
	//char            *thread_name_ptr    = SCI_NULL;
	char            *queue_name_ptr     = SCI_NULL;
	TX_THREAD     	* suspended_thread  = SCI_NULL;	
	TX_QUEUE      	* next_queue        = SCI_NULL;
       SCI_THREAD_T    * thread_block  = SCI_NULL;
       //SCI_TraceLow:"App_queue:\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_ENTRY_89_112_2_18_1_5_51_176,(uint8*)"");
    thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(P_APP);
    status = tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
		                       &queue_name_ptr,
							   (uint32 *)&enqueued,
							   (uint32 *)&store,
							   (TX_THREAD **)&suspended_thread,
							   (uint32 *)&count,
							   (TX_QUEUE **)&next_queue);
//	SAVE_RETURN_VAL_AND_ASSERT_ERROR(status);
      //SCI_TraceLow:"queue info:enqueued:%d, store: %d, count: %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_ENTRY_100_112_2_18_1_5_51_177,(uint8*)"ddd", enqueued, store, count);
      return store;
}

/*****************************************************************************/
//  Description:    Main thread of TPSVR task. It manages the pen signal
//                  from driver, and send it to the tasks which registered to 
//                  it before. It also processes PowerOn/PowerOff operation.
//  Author:         Robert.Lu
//	Note:           
/*****************************************************************************/
THREAD_ENTRY(P_TPP)
{//place it to init.c/CreateRefTasks()/before@if( g_nvitem_cache_enabled ) | line553
 //Messages send by this task is done in mmimain.c/TPCallBackFunc()/before@ |line220
	TPDSVR_SIG_T       *tp_sig_ptr = SCI_NULL;
	#ifndef WIN32
	TPD_TO_TPSVR_MSG_T *tp_from_tpd_sig_ptr = SCI_NULL;
	#endif

    //Init TouchPanel
    TouchPanel_Init();
    
    //tp_sig_ptr = (TPDSVR_SIG_T *)SCI_ALLOC_APP(sizeof(TPDSVR_SIG_T));
    #ifndef WIN32
   // tp_from_tpd_sig_ptr = (TPD_TO_TPSVR_MSG_T *)SCI_ALLOC_APP(sizeof(TPD_TO_TPSVR_MSG_T));
    #endif

    while (1)
    {
    	#ifndef WIN32
    	// Receive signal.
		tp_from_tpd_sig_ptr = (TPD_TO_TPSVR_MSG_T  *)SCI_GetSignal(P_TPP);
		if( (tp_from_tpd_sig_ptr->tpd_msg_valu == TP_ENTER_INTO_GPIO_HANDLER)
			&& (tp_from_tpd_sig_ptr->SignalCode == TP_ENTER_INTO_GPIO_HANDLER) )
		{
		    //if (SCI_FALSE != TPIsMsReady())
                         //get_queue_info();
			    IsGpioMessage();
		}
       //       SCI_FREE(tp_sig_ptr);
	        SCI_FREE(tp_from_tpd_sig_ptr);
 
		#else
		tp_sig_ptr = (TPDSVR_SIG_T *)SCI_GetSignal(P_TPP);
		SCI_SendEventToClient(TP_SERVICE, tp_sig_ptr->SignalCode, (void *)tp_sig_ptr);  //20050625
              SCI_FREE(tp_sig_ptr);
        #endif
    }//end while(1)  
	
}
