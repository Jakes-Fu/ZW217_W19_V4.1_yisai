/******************************************************************************
 ** File Name:      tp_multi_drv.c                                            *
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
#include "gpio_drv.h"
#include "tasks_id.h"
#include "tp_multi_cfg.h"
#include "gpio_prod_cfg.h"
#include "gpio_prod_api.h"
#include "diag.h"
#include "dal_lcd.h"
#include "dal_keypad.h"
#include "tb_hal.h"
#include "deepsleep_drvapi.h"
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
#define TP_MOVE_INTRVAL	3
typedef struct {                    
    //STRUCT_HEAD           //@David.Jia 2006.3.17
    TOUCHPANEL_MSG_SERVICE_E TP_type;
    uint16 TP_x;
    uint16 TP_y;        
} COMM_TP_T, *COMM_TP_PTR;  /*lint -esym(751, COMM_TP_T)*/
/**---------------------------------------------------------------------------*
 **                           Data Structures                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                        Global/Local Variables                             *
 **---------------------------------------------------------------------------*/
LOCAL TPC_OPERATION_T *s_tpc_operations = PNULL;
TPDSVR_SIG_T tpc_total_data;
LOCAL uint16 tp_old_status 	= TP_UP_MSG ;
LOCAL BOOLEAN set_gesture_id  = SCI_FALSE;
LOCAL BOOLEAN  read_data_lock = SCI_FALSE;
LOCAL uint32 pm_level_valid =0;
LOCAL uint16 x_position=0, y_position=0;

/**---------------------------------------------------------------------------*
 **                      Global/Local Function Prototypes                     *
 **---------------------------------------------------------------------------*/
extern uint32 get_app_queue_available(void);
LOCAL void _TPM_IntHandler(uint32 int_id,uint32 int_state);
LOCAL void _TPM_Enable_Interrupt(void);
LOCAL BOOLEAN  _TPM_MountGPIOHandler  (void);
LOCAL BOOLEAN _read_lock(void);
LOCAL void _read_unlock(void);
LOCAL void TPC_Register_TouchPanel_Hit_CmdRoutine(void);
/**---------------------------------------------------------------------------*
 **                        Function Definitions                               *
 **---------------------------------------------------------------------------*/
 LOCAL BOOLEAN _read_lock(void)
{
    if(read_data_lock == SCI_FALSE)
    {
        read_data_lock = SCI_TRUE; 
        return SCI_TRUE;
    }
    else
        return SCI_FALSE; 
}

LOCAL void _read_unlock(void)
{
    read_data_lock = SCI_FALSE;
}
/******************************************************************************/
// FUNCTION:    TPC_Init
// Description: This function is used to init Capactive TouchPanel.
// Dependence: 
// Note:   If touchpanel isn't capacitive style, internal embedded touchpanel chip is used.
/******************************************************************************/
PUBLIC uint32 TPC_Init(void)
{
    uint8 rc = TP_MULTI_ERROR;
    if(PNULL == s_tpc_operations)//capactive touchpanel doesn't initial.
    {
        uint8 len = 0;
        int32 i;
        TPC_OPERATION_T** tpc_ops=PNULL;

        SCI_Sleep(1000);   //TP初使化之前需要延时1秒钟，避免电源的干扰
        tpc_ops=(TPC_OPERATION_T**)TPC_GetOpsTab();
        //TP_MULTI_DRV_PRINT:"TPC LEN 0x%x\r\n"
        len = TPC_GetOpsTabLen();
        if(len > 0)
        {
            len--;
        }
        //SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_MULTI_DRV_95_112_2_18_0_35_4_2088,(uint8*)"d", TPC_GetOpsTabLen());
        for(i=0;i<len; i++)
        {
        	if(PNULL != tpc_ops[i])
            {
                if(PNULL != (tpc_ops[i]->init))
                {
                    if(TP_MULTI_SUCCESS == (tpc_ops[i]->init()))
                    {
                        s_tpc_operations = tpc_ops[i];
                        break;
                    }
                }
            }
        }
        //no useable TPC
        if(PNULL == s_tpc_operations)
        {
         	rc = TP_MULTI_ERROR;
        }
        else
        {
            rc = TP_MULTI_SUCCESS;
        }
    }
    else
    {
        //init fm chip
        if(s_tpc_operations->init && (TP_MULTI_SUCCESS == s_tpc_operations->init()) )
	    {	
          	rc = TP_MULTI_SUCCESS;
	    }
	    else
	    {
	      	rc = TP_MULTI_SUCCESS;
	    }    
    }

    if(s_tpc_operations != PNULL)
    {
        _TPM_Enable_Interrupt();
    }

    TPC_Register_TouchPanel_Hit_CmdRoutine();
    
    return 0;
}


/*****************************************************************************/
//  FUNCTION:     TPC_Read
//  Description:  Data from capacitive touchpanel chip.
//	Note:
/*****************************************************************************/
PUBLIC uint32 TPC_Read(TPDSVR_SIG_T *data)
{
    if(!(_read_lock()))
       return SCI_ERROR;
    if(s_tpc_operations != PNULL)
    {
        s_tpc_operations->read(data);
        _read_unlock();
        return SCI_SUCCESS;
    }
    else
        _read_unlock();
    return SCI_ERROR;
}

/*****************************************************************************/
//  FUNCTION:     TPC_Ioctl
//  Description:  Data from capacitive touchpanel chip.
//	Note:
/*****************************************************************************/
PUBLIC uint32 TPC_Ioctl(uint32 cmd, void *arg)/*lint !e18 !e532*/
{
    uint32 ret = SCI_SUCCESS;
    if(s_tpc_operations != PNULL)
    {
        ret = s_tpc_operations->ioctl(cmd, arg);
        return ret;
    }
    else
        return SCI_ERROR;
}

/*****************************************************************************/
//  FUNCTION:     TPC_Suspend
//  Description:  suspend capacitive touchpanel chip.
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN TPC_Suspend(void)
{
    if(s_tpc_operations != PNULL)
    {
        s_tpc_operations->suspend();
        return SCI_TRUE;
    }
    return SCI_FALSE;
}

/*****************************************************************************/
//  FUNCTION:     TPC_Resume
//  Description:  Resume from capacitive touchpanel chip.
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN TPC_Resume(void)
{
    if(s_tpc_operations != PNULL)
    {
        s_tpc_operations->resume();
        return SCI_TRUE;
    }
    return SCI_FALSE;
}

/*****************************************************************************/
//  FUNCTION:     _TPM_EnSet_Gesture
//  Description:  Now only set gesture one time between down MSG and up MSG.
//	Note:
/*****************************************************************************/
LOCAL void _TPM_EnSet_Gesture(void)
{
    set_gesture_id = SCI_TRUE;
}
/*****************************************************************************/
//  FUNCTION:     _TPM_DisSet_Gesture
//  Description:  Now only set gesture one time between down MSG and up MSG.
//	Note:
/*****************************************************************************/
LOCAL void _TPM_DisSet_Gesture(void)
{
    set_gesture_id = SCI_FALSE;
}
/*****************************************************************************/
//  FUNCTION:     _TPM_Get_GestureStatus
//  Description:  Get Enable or Disable gesture status.
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN _TPM_Get_GestureStatus(void)
{
    return set_gesture_id;
}

/*****************************************************************************/
//  FUNCTION:     _TPM_Get_LastStatus
//  Description:  Get tp last MSG status.
//	Note:
/*****************************************************************************/
LOCAL uint16 _TPM_Get_LastStatus(void)
{
    return tp_old_status;
}

/*****************************************************************************/
//  FUNCTION:     _TPM_Set_LastStatus
//  Description:  Get tp last MSG status.
//	Note:
/*****************************************************************************/
LOCAL void _TPM_Set_LastStatus(uint16 msg)
{
    tp_old_status = msg;
}

PUBLIC uint32 TP_Read(TPDSVR_SIG_T *data)
{
	if(data != PNULL)
    {
		SCI_MEMCPY(data, &tpc_total_data, sizeof(TPDSVR_SIG_T));
	}
	else
	{
		//SCI_TRACE_LOW:"TPC: <Error>data is null\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_MULTI_DRV_241_112_2_18_0_35_4_2089,(uint8*)"");
		return SCI_ERROR;
	}	
    return SCI_SUCCESS;
}
/*****************************************************************************/
//  FUNCTION:     IsMove
//  Description:  Get tp move interval.
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsMove()
{
	if((abs(tpc_total_data.x_key - x_position)) >TP_MOVE_INTRVAL ||(abs(tpc_total_data.y_key - y_position))>TP_MOVE_INTRVAL)
	{
		x_position = tpc_total_data.x_key;
		y_position = tpc_total_data.y_key;
		return SCI_TRUE;
	}
	else
		return SCI_FALSE;
}
/*****************************************************************************/
//  FUNCTION:     _TPM_Enable_Interrupt
//  Description:  capacitive touchpanel set to interrupt  after tp chip initial.
//	Note:
/*****************************************************************************/
LOCAL void _TPM_Enable_Interrupt(void)
{
    if(_TPM_MountGPIOHandler())
    {
        GPIO_TPIRQCtrl(SCI_FALSE);
        pm_level_valid = GPIO_TPGetInterruptStatus();
        GPIO_ClearIntStatus((uint32)GPIO_TPInterruptPin());
        GPIO_TPInterruptSense(pm_level_valid);
        GPIO_TPIRQCtrl(SCI_TRUE);
    }
    
}

/*****************************************************************************/
//  FUNCTION:       _TPM_MountGPIOHandler
//  Description:    mount TP_GPIOHandler to GPIO interrupt vector table
//  Return:         TRUE
//              	FALSE
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  _TPM_MountGPIOHandler  (void)
{
	//TP_MULTI_DRV_PRINT:"TP_DRIVER: enter into TP_Multi_MountGPIOHandler\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_MULTI_DRV_281_112_2_18_0_35_4_2090,(uint8*)"");

	if(GPIO_PROD_RegGpio( //GPIO_AddCallbackToIntTable
		GPIO_PROD_TP_INT_ID,
		SCI_FALSE,
		SCI_FALSE,
		SCI_FALSE,
		0,
		(GPIO_PROD_CALLBACK)_TPM_IntHandler))
    {
            //TP_MULTI_DRV_PRINT("TP_MULTI_DRIVER: TP_Multi_MountGPIOHandler SUCCESS\n");
            //GPIO_TPIRQCtrl(SCI_TRUE);
            return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/*****************************************************************************/
//  FUNCTION:       _TPM_IntHandler
//  Description:    interrupt handler for sending signal as capacitive touchpanel.
//  Return:         
//	Note:           Signal function getting is THREAD_ENTRY (P_TP) in tp_multi_srv.c file
/*****************************************************************************/
LOCAL void _TPM_IntHandler(uint32 int_id,uint32 int_state)
{
    TPD_TO_TPSVR_MSG_T *tp_to_tpsvr_sig_ptr    =  SCI_NULL;
    //TP_MULTI_DRV_PRINT("TP:: _TPM_IntHandler, int_state 0x%x!\r\n", int_state);

    if(PNULL == s_tpc_operations)
    {
        return;
    }
    if(int_state == pm_level_valid)
    {
    	 GPIO_ClearIntStatus((uint32)GPIO_TPInterruptPin());
        GPIO_TPInterruptSense((uint32)!pm_level_valid);
               
        tp_to_tpsvr_sig_ptr = (TPD_TO_TPSVR_MSG_T *)SCI_ALLOCA(sizeof(TPD_TO_TPSVR_MSG_T));
      
        SCI_ASSERT(SCI_NULL != tp_to_tpsvr_sig_ptr);   /*assert verified*/
	
	
        tp_to_tpsvr_sig_ptr->SignalSize = sizeof(TPD_TO_TPSVR_MSG_T);
        tp_to_tpsvr_sig_ptr->Sender = P_TPP;
          	
        tp_to_tpsvr_sig_ptr->SignalCode = TP_ENTER_INTO_GPIO_HANDLER;
        tp_to_tpsvr_sig_ptr->tpd_msg_valu = TP_ENTER_INTO_GPIO_HANDLER;
        
        SCI_SendSignal((xSignalHeader)tp_to_tpsvr_sig_ptr, P_TPP);
    }
    else
    {
        GPIO_ClearIntStatus((uint32)GPIO_TPInterruptPin());
        GPIO_TPInterruptSense((uint32)pm_level_valid);
    }
  	
}



/*****************************************************************************/
//  FUNCTION:       HandleTPMessage
//  Description:    This function to handle TP gpio message
//                  
//	Note:           This function will be called in P_TP task in tp_multi_srv.c file
/*****************************************************************************/
void HandleTPMessage(void)
{
   	uint32 app_queue_avilable = 0;
   
 	//TP_MULTI_DRV_PRINT("TPC_MULTI: IsGpioMessage\n\r");
 	GPIO_TPIRQCtrl(SCI_FALSE);
	
	if(_TPM_Get_LastStatus() == TP_UP_MSG)
	{
		//Disable APB sleep
		SCI_TPC_EnableDeepSleep (DISABLE_APB_SLEEP);
	}
	TPC_Read(&tpc_total_data); 
	
	if(tpc_total_data.SignalCode == TP_DOWN_MSG )
	{
		_TPM_EnSet_Gesture();
	}
    
#if 1
    //Temporary: when arm be off, DOWN MSG will be lose.FT5206
    if((_TPM_Get_LastStatus() == TP_UP_MSG) && (tpc_total_data.SignalCode == TP_MOVE_MSG))
    {
        tpc_total_data.SignalCode = TP_DOWN_MSG;
        _TPM_EnSet_Gesture();
    }
    //Temporary
#endif


        if((tpc_total_data.gesture_type > TG_NO_DETECT) && (tpc_total_data.gesture_type < TG_UNKNOWN_STATE))
        {
	        if((_TPM_Get_LastStatus() != TP_GESTURE_MSG) /*&& (_TPM_Get_GestureStatus())*/)
	        {
		    	tpc_total_data.SignalCode= TP_GESTURE_MSG;
		    	_TPM_DisSet_Gesture();
	        }
            else
            {
            	tpc_total_data.SignalCode= TP_MOVE_MSG;//DON'T 
            	_TPM_Set_LastStatus(tpc_total_data.SignalCode);
            }
        }
	
	//Send event to Client
	app_queue_avilable = get_app_queue_available();
	if (app_queue_avilable > 12)
	{
	    if(tpc_total_data.SignalCode == TP_MOVE_MSG)
		{
	       if((IsMove()) && (_TPM_Get_GestureStatus()))
           {
                _TPM_Set_LastStatus(tpc_total_data.SignalCode);
    			SCI_SendEventToClient(TP_SERVICE, tpc_total_data.SignalCode, (void *)&tpc_total_data);	
           }
	    }
		else if(((_TPM_Get_LastStatus() != TP_UP_MSG) && (tpc_total_data.SignalCode == TP_UP_MSG)) || (tpc_total_data.SignalCode != TP_UP_MSG))//down,up msg
		{
		    _TPM_Set_LastStatus(tpc_total_data.SignalCode);
			SCI_SendEventToClient(TP_SERVICE, tpc_total_data.SignalCode, (void *)&tpc_total_data);	
		}
	}
	// make sure the up signal must be sent out, meanwhile, repeated up signal should be discard;
	else if((_TPM_Get_LastStatus() != TP_UP_MSG) && (tpc_total_data.SignalCode == TP_UP_MSG))
	{
		// TP_MULTI_DRV_PRINT("TPC_MULTI: >10,PEN_UP SignalCode: 0x%x\r\n",tpc_total_data.SignalCode);
		_TPM_Set_LastStatus(tpc_total_data.SignalCode);
		SCI_SendEventToClient(TP_SERVICE, tpc_total_data.SignalCode, (void *)&tpc_total_data);
	}    
	while(get_app_queue_available() < 12){
		SCI_Sleep(50);    
	}

	if(tpc_total_data.SignalCode == TP_UP_MSG)
	{
		//Enable APB sleep
		SCI_TPC_EnableDeepSleep (ENABLE_APB_SLEEP);
	}
	GPIO_TPIRQCtrl(SCI_TRUE);
}


LOCAL uint32 TPC_TouchPanelHit(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8. 
{
 
    COMM_TP_PTR     comm_ptr;
   // TPDSVR_SIG_T    *tp_sig_ptr = &s_tp_sig;
    
    comm_ptr = (COMM_TP_PTR)(src_ptr + sizeof(MSG_HEAD_T));

    // Build response to channel server
    *dest_ptr = SCI_ALLOC_APP(sizeof(MSG_HEAD_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T);
    memcpy(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
    
    //decode COMM_TP_T to TPDSVR_SIG_T
    tpc_total_data.SignalCode = comm_ptr->TP_type;
    tpc_total_data.x_key = comm_ptr->TP_x;
    tpc_total_data.y_key = comm_ptr->TP_y;
    
    //SCI_ASSERT((TP_UP==tpc_total_data.SignalCode) || (TP_DOWN==tpc_total_data.SignalCode) || (TP_MOVE==tpc_total_data.SignalCode));
    
    //send to TP_SERVICE's client
    //SCI_TRACE_LOW:" TPC:SIMULATOR, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_MULTI_DRV_451_112_2_18_0_35_5_2091,(uint8*)"ddd", tpc_total_data.SignalCode, tpc_total_data.x_key, tpc_total_data.y_key);
    SCI_SendEventToClient(TP_SERVICE, tpc_total_data.SignalCode, (void *)&tpc_total_data);

    return SCI_TRUE;

}  

LOCAL void TPC_Register_TouchPanel_Hit_CmdRoutine( void )
{    
    DIAG_RegisterCmdRoutine( DIAG_TP_F,  TPC_TouchPanelHit );
}



#ifdef __cplusplus
}
#endif

/******************************************************************************/
