/******************************************************************************
 ** File Name:    TP_srv.c                                                     *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         6/25/2006                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 6/25/2006     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_customize_trc.h"
#include "tp_srv.h"
#include "threadx_os.h"
#include "os_apiext.h"
#include "dal_keypad.h"
#ifndef WIN32
    #include "power.h"
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
#define TPD_SRV_DEBUG

#ifdef TPD_SRV_DEBUG
    #define TPD_SRV_PRINT	SCI_TRACE_LOW
#else
    #define TPD_SRV_PRINT	
#endif

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/

/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
#ifndef WIN32
    extern void HandleGpioMessage(void);
    extern void TP_DInit(void);
#endif


//#define 	TP_MSG_MAX_NUM     		   (TP_MSG_MAX & 0x00FF)


/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

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
    //Do nothing ;
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
	char            *queue_name_ptr     = SCI_NULL;
	TX_THREAD     	* suspended_thread  = SCI_NULL;	
	TX_QUEUE      	* next_queue        = SCI_NULL;
       SCI_THREAD_T    * thread_block  = SCI_NULL;
       //SCI_TraceLow:"App_queue:\n"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_86_112_2_18_0_35_5_2093,(uint8*)"");
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

/*****************************************************************************/
//  Description:    Main thread of TPSVR task. It manages the pen signal
//                  from driver, and send it to the tasks which registered to 
//                  it before. It also processes PowerOn/PowerOff operation.
//  Author:         Robert.Lu
//	Note:           
/*****************************************************************************/
//LOCAL void thread_tpsvr_entry(uint32 argc, void *argv)
THREAD_ENTRY(P_TP)
{
//place it to init.c/CreateRefTasks()/before@if( g_nvitem_cache_enabled ) | line553
 //Messages send by this task is done in mmimain.c/TPCallBackFunc()/before@ |line220
	//TPDSVR_SIG_T       *tp_sig_ptr = SCI_NULL;
	
	#ifndef WIN32
    	TPD_TO_TPSVR_MSG_T *tp_from_tpd_sig_ptr = SCI_NULL;

        TP_DInit();
       // tp_from_tpd_sig_ptr = (TPD_TO_TPSVR_MSG_T *)SCI_ALLOC_APP(sizeof(TPD_TO_TPSVR_MSG_T));
    #endif

	for(;;)
    {
    	#ifndef WIN32
        	// Receive signal.
    		tp_from_tpd_sig_ptr = (TPD_TO_TPSVR_MSG_T  *)SCI_GetSignal(P_TPP);
    		if( (tp_from_tpd_sig_ptr->tpd_msg_valu == TP_ENTER_INTO_GPIO_HANDLER)
    			&& (tp_from_tpd_sig_ptr->SignalCode == TP_ENTER_INTO_GPIO_HANDLER) )
    		{
                HandleGpioMessage();
    		}

            SCI_FREE(tp_from_tpd_sig_ptr);

		#else

    		tp_sig_ptr = (TPDSVR_SIG_T *)SCI_GetSignal(P_TPP);
    		SCI_SendEventToClient(TP_SERVICE, tp_sig_ptr->SignalCode, (void *)tp_sig_ptr);  //20050625
            SCI_FREE(tp_sig_ptr);

        #endif
    }//end while(1)  
}

/*****************************************************************************/
//  Description:    Call this function to initialize touchpanel task. 
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           This function must be called before enter MMI task ;
/*****************************************************************************/
PUBLIC void TP_Init (void)
{  	
    SCI_CreateClientList(TP_SERVICE, (TP_MSG_MAX_NUM & 0x0ff), \
                                        (REG_CALLBACK)TOUCHPANEL_default );
}


//@David.Jia    2006.3.15   cr43392 end