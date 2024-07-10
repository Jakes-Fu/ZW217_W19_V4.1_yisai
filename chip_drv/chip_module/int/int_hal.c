/******************************************************************************
 ** File Name:      int_hal.c                                                  *
 ** Author:         Xueliang.Wang                                             *
 ** DATE:           10/14/2002                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 10/14/2002     Xueliang.Wang    Create.
 ** 07/19/2010     Steve.Zhan        modifyed.                                *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "chip_drv_trc.h"
#include "os_api.h"
#include "chip_plf_export.h"
#include "task_monitor.h"
#include "task_analyzer.h"
#include "gpio_drvapi.h"
#include "int_phy.h"
//#include "isr_drvapi.h"
#include "fiq_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Data                                              *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
**                         Function Definitions                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function should be called at the startup of the
//                  application. It initializes TBIOS interrupt resources.
//                  This must be called before installing an IRQ handler.
//  Dependency:     irq_isr_array
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
void ISR_Init (void)
{
    Debug_Uart4_puts("Enter ISR_Init_1\n");
    INT_PHY_Init();
    Debug_Uart4_puts("Enter ISR_Init_2\n");
    Hisr_init();
    Debug_Uart4_puts("Enter ISR_Init_3\n");
#ifdef FPGA_VERIFICATION_DUAL_LZMA
    SoftInt_Init();
#endif
    Debug_Uart4_puts("Enter ISR_Init_4\n");
    INT_PHY_PMIC_INIT();
}

extern uint32 _tx_thread_system_state;

/*****************************************************************************/
//  Description:    This function is called when an IRQ happens.
//  Dependency:     IrqUnitRegValue, irq_isr_array
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
void ISR_Handler (void)
{
    int pri = 0;
    ISR_EXE_T needCallHisr = ISR_DONE;
#if defined(PLATFORM_UWS6121E)    
    uint32 int_id;
#endif
    INT_PHY_GetIrqIntStatus();
    {
        PUBLIC void TIMER_ReLoad (void);
        TIMER_ReLoad ();
    }

#ifdef CHIP_VER_6530
    if (0 == IrqUnitRegValue[0])
#else        
    if ((0 == IrqUnitRegValue[0]) && (0== IrqUnitRegValue[1]) && (0== IrqUnitRegValue[2]))
#endif
    {
        GPIO_ZeroIntHandler (0);
        return;
    }

#if !defined(PLATFORM_UWS6121E)    
    TM_SendIrqRequest (TM_ENTER_INT_REQ, IrqUnitRegValue[0]); // add test point record
    TM_SendTestPointRequest(IrqUnitRegValue[0], IrqUnitRegValue[1]);
    TM_SendTestPointRequest(0xAA, IrqUnitRegValue[2]);
#endif

    // interrupt reenter
    SCI_ASSERT(_tx_thread_system_state <= 1);

#if defined(PLATFORM_UWS6121E)    
    int_id = INT_PHY_GetLisrId();

    TM_SendIrqRequest (TM_ENTER_INT_REQ, int_id); // add test point record
    TM_SendTestPointRequest(IrqUnitRegValue[0], IrqUnitRegValue[1]);
    TM_SendTestPointRequest(0xAA, IrqUnitRegValue[2]);
    
    for (; pri < MAX_ISR_NUM; pri++)
    {
        if ( (int) irq_isr_array[pri].intLogicNum == (int) int_id)
        {
            break;
        }
    }

	if (pri != MAX_ISR_NUM)
	{
	    if (TB_NULL != irq_isr_array[pri].Handlefunc)
	    {
	        needCallHisr = (ISR_EXE_T) (*irq_isr_array[pri].Handlefunc) (irq_isr_array[pri].intLogicNum);
	    }
	    else if(TB_NULL != irq_isr_array[pri].isrDefaultFunc)
	    {
	        needCallHisr = (ISR_EXE_T) (*irq_isr_array[pri].isrDefaultFunc) (irq_isr_array[pri].intLogicNum);
	    }
	    else
	    {
	        needCallHisr = ISR_DONE;
	    }

	    if (needCallHisr == CALL_HISR)
	    {
	        Hisr_put (pri);
	    }
	}

    TM_SendTestPointRequest(int_id, pri);

    INT_PHY_CleanLisrInt(int_id);
#else
    do
    {
        if (IrqUnitRegValue[irq_isr_array[pri].IntRegValueIndex] & irq_isr_array[pri].IntRegBitMask)
        {
            if (TB_NULL != irq_isr_array[pri].Handlefunc)
            {
                needCallHisr = (ISR_EXE_T) (*irq_isr_array[pri].Handlefunc) (irq_isr_array[pri].intLogicNum);
            }
            else if(TB_NULL != irq_isr_array[pri].isrDefaultFunc)
            {
                needCallHisr = (ISR_EXE_T) (*irq_isr_array[pri].isrDefaultFunc) (irq_isr_array[pri].intLogicNum);
            }
            else
            {
                needCallHisr = ISR_DONE;
            }

            if (needCallHisr == CALL_HISR)
            {
                Hisr_put (pri);
            }

            break;
        }

        pri ++;

    }
    while (pri < MAX_ISR_NUM);
#endif
    TM_SendIrqRequest (TM_LEAVE_INT_REQ, 0);
}

/*****************************************************************************/
//  Description:    This function translate interrupt logic Number to related priority index.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
static __inline uint32 INT_GetPriIndex (uint32 logicNum, PRI_INDEX_GET_STATUS_T *p_isr_array)
{
    int i ;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if ( (int) p_isr_array[i].intLogicNum == (int) logicNum)
        {
            break;
        }
    }

    // CHIP_DRV_PASSERT ( (i < MAX_ISR_NUM), ("logicNum=%d\n",logicNum));

    return (uint32) i;
}

LOCAL uint32 _INT_RegHandler (uint32 logic_num, TB_ISR _isr_handler, PRI_INDEX_GET_STATUS_T *p_isr_array)
{
    TB_ISR isr_handler = _isr_handler;
    int pri_index;

    if (TB_NULL == isr_handler)
    {
        return TB_PTR_ERROR;
    }

    pri_index = INT_GetPriIndex (logic_num, p_isr_array);

    if (TB_NULL != (p_isr_array[pri_index].Handlefunc))
    {
        return TB_ISR_ASSIGNED;
    }
    else
    {
        p_isr_array[pri_index].Handlefunc = isr_handler;
        return TB_SUCCESS;
    }

}


LOCAL uint32 _INT_UnRegHandler (uint32 logic_num, PRI_INDEX_GET_STATUS_T *p_isr_array)
{
    int pri_index;

    pri_index = INT_GetPriIndex (logic_num, p_isr_array);

    if (TB_NULL != p_isr_array[pri_index].Handlefunc)
    {
        p_isr_array[pri_index].Handlefunc = TB_NULL;
        return TB_SUCCESS;
    }
    else
    {
        return TB_ISR_NULL ;
    }

}

LOCAL uint32 _INT_Callback (uint32 logic_num, TB_MSG *msg, PRI_INDEX_GET_STATUS_T *p_isr_array)
{
    int pri_index;

    if (TB_NULL == msg)
    {
        return TB_PTR_ERROR;
    }

    pri_index = INT_GetPriIndex (logic_num, p_isr_array);

    if (TB_NULL == p_isr_array[pri_index].callback)
    {
        return TB_NO_CALLBACK;
    }
    else
    {
        (*p_isr_array[pri_index].callback) ( (void *) msg);
        return TB_SUCCESS;
    }

}

LOCAL uint32 _INT_RegCallback (uint32 logic_num, TB_CALLBACK handler,PRI_INDEX_GET_STATUS_T *p_isr_array)
{
    int pri_index;

    if (TB_NULL == handler)
    {
        return TB_PTR_ERROR;
    }

    pri_index = INT_GetPriIndex (logic_num, p_isr_array);

    if (TB_NULL != p_isr_array[pri_index].callback)
    {
        return TB_CALLBACK_ASSIGNED;
    }
    else
    {
        p_isr_array[pri_index].callback = (CALLBACK_T) handler;
        return TB_SUCCESS;
    }

}

LOCAL uint32 _INT_UnRegCallback (uint32  logic_num,  PRI_INDEX_GET_STATUS_T *p_isr_array)
{
    int pri_index;

    pri_index = INT_GetPriIndex (logic_num, p_isr_array);

    if (TB_NULL ==  p_isr_array[pri_index].callback)
    {
        return TB_NO_CALLBACK;
    }
    else
    {
        p_isr_array[pri_index].callback = TB_NULL;
        return TB_SUCCESS;
    }

}

/*****************************************************************************/
//  Description:    This function is called to register a new ISR.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:           If success, the function returns TB_SUCCESS;
//                  if isr_handler is NULL, returns TB_PTR_ERROR;
//                  if logic_num is a invalid number, returns TB_INT_ERROR;
//                  if the isr_handler is installed before, returns
//                  TB_ISR_ASSIGNED.
/*****************************************************************************/
uint32 ISR_RegHandler (
    uint32 logic_num,      // Interrupt number
    TB_ISR isr_handler   // Interrupt service routine pointer.
)
{
    return _INT_RegHandler (logic_num, isr_handler, irq_isr_array);
}

uint32 ISR_RegHandler_Ex (
    uint32 logic_num,      // Interrupt number
    TB_ISR isr_handler,   // Interrupt service routine pointer.

    hisr_func_t *pHisrFunc,
    CHIPDRV_HISR_PRI_E hisrPri,
    void              *pHisrData
)
{
    uint32 priIndex = 0;
    priIndex = INT_GetPriIndex (logic_num, irq_isr_array);
    Hisr_RegHandler (priIndex, pHisrFunc, hisrPri, pHisrData);
    return _INT_RegHandler (logic_num, isr_handler, irq_isr_array);
}

/*****************************************************************************/
//  Description:    This function translate interrupt logic Number to related
//                  priority index.
//  Dependency:
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
PUBLIC uint32 INT_HAL_GetPriIndex (uint32 logicNum, PRI_INDEX_GET_STATUS_T *p_isr_array)
{
    return INT_GetPriIndex (logicNum, p_isr_array);
}

/*****************************************************************************/
//  Description:    This function is called to UnRegister an ISR.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:           If success, the function returns TB_SUCCESS;
//                  if logic_num is a invalid number, returns TB_INT_ERROR;
//                  if the isr_handler is not installed before,
//                  returns TB_ISR_NULL.
/*****************************************************************************/
uint32 ISR_UnRegHandler (
    uint32 logic_num   // Interrupt number
)
{
    return _INT_UnRegHandler (logic_num, irq_isr_array);
}

/*****************************************************************************/
//  Description:    This function is called to do works defined by upper layer.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:           If OK, Returns TB_SUCCESS;
//                  if logic_num is an invalid number, returns TB_INT_ERROR;
//                  if msg is NULL, returns TB_PTR_ERROR ;
//                  if no callback function registered, returns TB_NO_CALLBACK
/*****************************************************************************/
uint32 ISR_Callback (
    uint32 logic_num, // Interrupt number
    TB_MSG *msg     // Message pointer
)
{
    return _INT_Callback (logic_num, msg, irq_isr_array);
}


/*****************************************************************************/
//  Description:    This function is called to register a callback function
//                  called from lower layer.
//  Dependency:     irq_isr_array
//  Author:         Richard.Yang
//  Note:           If OK, returns TB_SUCCESS;
//                  if logic_num is an invalid number, returns TB_INT_ERROR;
//                  if handler pointer is NULL, returns TB_PTR_ERROR;
//                  if callback function is already registered,
//                  returns TB_CALLBACK_ASSIGNED .
/*****************************************************************************/
uint32 ISR_RegCallback (
    uint32      logic_num,    // Interrupt number
    TB_CALLBACK handler     // Callback handler
)
{
    return _INT_RegCallback (logic_num, handler, irq_isr_array);

}


/*****************************************************************************/
//  Description:    This function is called to unregister a callback function
//                  called from lower layer.
//  Dependency:     irq_isr_array
//  Author:         Xueliang.Wang
//  Note:           Returns TB_SUCCESS if OK;
//                  returns TB_INT_ERROR if logic_num is an invalid number;
//                  returns TB_NO_CALLBACK if no callback function registered.
/*****************************************************************************/
uint32 ISR_UnRegCallback (
    uint32      logic_num     // Interrupt number
)
{
    return _INT_UnRegCallback (logic_num, irq_isr_array);
}


/*****************************************************************************/
//  Description:    using interrupt ligic number to clear the interrupt.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_IRQClear (uint32 logic_num)
{
}


/*****************************************************************************/
//  Description:    Enable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_IRQEnable (uint32 logic_num)
{
    INT_PHY_IRQEnable (logic_num);
}

/*****************************************************************************/
//  Description:    Disable certain interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_IRQDisable (uint32 logic_num)
{
    INT_PHY_IRQDisable (logic_num);
}

/*****************************************************************************/
//  Description:    Disable All interrupt source.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_IRQDisableAll()
{
    INT_PHY_IRQDisableAll();
}

/*****************************************************************************/
//  Description:    Get IRQ status
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
uint32 INT_HAL_IRQGetUint1Status()
{
    return INT_PHY_IRQGetUint1Status();
}

/*****************************************************************************/
//  Description:    Get IRQ IE State
//
//  Dependency:
//  Author:         Yong.Li
//  Note:           ;
/*****************************************************************************/
uint32 INT_HAL_IRQGetIEState()
{
    return INT_PHY_IRQGetIEState();
}

//*****************************************************************************
//  Description:    This function is called when an FIQ happens.
//  Dependency:     s_fiq_array
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC void ARM_FIQ_Handler (void)
{
    int pri = 0;

    INT_PHY_GetFiqIntStatus();

    TM_SendIrqRequest (TM_ENTER_FIQ, FiqUnitRegValue[0]);
    INT_PHYFiqDebug();

    do
    {
        if (FiqUnitRegValue[fiq_isr_array[pri].IntRegValueIndex] & fiq_isr_array[pri].IntRegBitMask)
        {
            if (TB_NULL != fiq_isr_array[pri].Handlefunc)
            {
                TA_EnterFiqRequest (pri);// task analyzer
                (*fiq_isr_array[pri].Handlefunc) (fiq_isr_array[pri].intLogicNum);
            }
            else
            {
                (*fiq_isr_array[pri].isrDefaultFunc) (fiq_isr_array[pri].intLogicNum);
            }

            break;
        }

        pri ++;

    }
    while (pri < MAX_ISR_NUM);

    TM_SendIrqRequest (TM_LEAVE_FIQ, 0);
}


/*****************************************************************************/
//  Description:    This function is called to register a new FIQ.
//  Dependency:     s_fiq_array
//  Author:         Steve.zhan
//  Note:           Now, fiq don't support hisr, if need, must change the struct
//                  FIQ_HANDLE type.
//                  If success, the function returns FIQ_SUCCESS;
//                  if fiq_handler is NULL, returns FIQ_PTR_ERROR;
//                  if fiq_num is a invalid number, returns FIQ_INT_ERROR;
//                  if the fiq_handler is installed before, returns
//                  FIQ_ISR_ASSIGNED.
/*****************************************************************************/
PUBLIC uint32 FIQ_RegHandler (
    uint32      fiq_num,        // Interrupt number
    FIQ_HANDLE  fiq_handler     // Interrupt service routine pointer.
)
{
    _INT_RegHandler (fiq_num, (TB_ISR) fiq_handler, fiq_isr_array);
    return FIQ_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is called to UnRegister an FIQ.
//  Dependency:     s_fiq_array
//  Author:         Andy.Chen
//  Note:           If success, the function returns FIQ_SUCCESS;
//                  if fiq_num is a invalid number, returns FIQ_INT_ERROR;
//                  if the fiq_handler is not installed before,
//                  returns FIQ_ISR_NULL.
/*****************************************************************************/
PUBLIC uint32 FIQ_UnRegHandler (
    uint32 fiq_num      // Interrupt number
)
{
    _INT_UnRegHandler (fiq_num, fiq_isr_array);
    return FIQ_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is called to do works defined by upper layer.
//  Dependency:     s_fiq_array
//  Author:         Andy.Chen
//  Note:           If OK, Returns FIQ_SUCCESS;
//                  if fiq_num is an invalid number, returns FIQ_INT_ERROR;
//                  if msg is NULL, returns FIQ_PTR_ERROR ;
//                  if no callback function registered, returns FIQ_NO_CALLBACK
/*****************************************************************************/
PUBLIC uint32 FIQ_Callback (
    uint32  fiq_num, // Fiq Interrupt number
    FIQ_MSG *msg     // Message pointer
)
{
    _INT_Callback (fiq_num, (TB_MSG*)msg, fiq_isr_array);
    return FIQ_SUCCESS;
}


/*****************************************************************************/
//  Description:    This function is called to register a callback function
//                  called from lower layer.
//  Dependency:     s_fiq_array
//  Author:         Richard.Yang
//  Note:           If OK, returns FIQ_SUCCESS;
//                  if fiq_num is an invalid number, returns FIQ_INT_ERROR;
//                  if handler pointer is NULL, returns FIQ_PTR_ERROR;
//                  if callback function is already registered,
//                  returns FIQ_CALLBACK_ASSIGNED .
/*****************************************************************************/
PUBLIC uint32 FIQ_RegCallback (
    uint32          fiq_num,    // Interrupt number
    FIQ_CALLBACK    handler     // Callback handler
)
{
    _INT_RegCallback (fiq_num, (TB_CALLBACK)handler,fiq_isr_array);
    return FIQ_SUCCESS;
}

/*****************************************************************************/
//  Description:    This function is called to unregister a callback function
//                  called from lower layer.
//  Dependency:     s_fiq_array
//  Author:         Xueliang.Wang
//  Note:           Returns FIQ_SUCCESS if OK;
//                  returns FIQ_INT_ERROR if int_num is an invalid number;
//                  returns FIQ_NO_CALLBACK if no callback function registered.
/*****************************************************************************/
PUBLIC uint32 FIQ_UnRegCallback (
    uint32  fiq_num     // Interrupt number
)
{
    _INT_UnRegCallback (fiq_num, fiq_isr_array);
    return FIQ_SUCCESS;
}


/*****************************************************************************/
//  Description:    Disable the interrupt source FOR fiq.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_FIQEnable (uint32 logicNum)
{
    INT_PHY_FIQEnable (logicNum);
}

/*****************************************************************************/
//  Description:    Disable the interrupt source FOR fiq.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_FIQDisable (uint32 logicNum)
{
    INT_PHY_FIQDisable (logicNum);
}

/*****************************************************************************/
//  Description:    Disable All interrupt source FOR fiq.
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_FIQDisableAll()
{
    INT_PHY_FIQDisableAll();
}

/*****************************************************************************/
//  Description:    Disable All interrupt
//
//  Dependency:
//  Author:         Steve.Zhan
//  Note:           ;
/*****************************************************************************/
void INT_HAL_DisableINT (void)
{
    INT_PHY_IRQDisableAll();
    INT_PHY_FIQDisableAll();

}

#if 0//is used by debug
PUBLIC void INT_DebugShowRegs (void)
{

    uint32  i = 0;

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {

        if (irq_isr_array[i].Handlefunc || irq_isr_array[i].callback)
        {
            if (! (irq_isr_array[i].Handlefunc))
                if (irq_isr_array[i].callback)
                {
                    CHIP_DRV_PASSERT ( (0), ("irq_isr_array[i].callback != null\n"));/*assert verified*/
                }

            //SCI_TraceLow:"IRQ HAS USED, intLogicNum = 0x%xn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,INT_HAL_583_112_2_17_23_4_57_1033,(uint8*)"d",irq_isr_array[i].intLogicNum);
        }

    }

    for (i = 0; i < MAX_ISR_NUM; ++i)
    {
        if (fiq_isr_array[i].Handlefunc || fiq_isr_array[i].callback)
        {
            if (! (fiq_isr_array[i].Handlefunc))
                if (fiq_isr_array[i].callback)
                {
                    CHIP_DRV_PASSERT ( (0), ("fiq_isr_array[i].callback != null\n"));/*assert verified*/
                }

            //SCI_TraceLow:"FIQ HAS USED, intLogicNum = 0x%xn"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,INT_HAL_599_112_2_17_23_4_57_1034,(uint8*)"d",fiq_isr_array[i].intLogicNum);
        }
    }

}

#endif

#ifdef FPGA_VERIFICATION_DUAL_LZMA
/*****************************************************************************/
//  Description:    SoftINT HISR task function
//                  .
//  Dependency:     irq_isr_array
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
LOCAL void _SoftIntHisrFunc(uint32 count, void *data)
{
extern BOOLEAN dp_handle_SoftIntHisrFunc(uint32 , void *);

    dp_handle_SoftIntHisrFunc(count, data);
}
/*****************************************************************************/
//  Description:    This function is used to set SoftINT enable and isr_handle
//                  application.
//  Dependency:     irq_isr_array
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
LOCAL ISR_EXE_T _SoftInt_Callback (void)
{
extern uint32 dp_handle_AbortVirAddrRelay(void);
    uint16 int_status;

    int_status = (REG32 (INT_IRQ_STS) & BIT_1);
    
    TM_SendTestPointRequest(0xA0000000, int_status);   
    
    if (int_status)
    {
        CHIP_REG_AND(INT_IRQ_DISABLE, BIT_1);
        CHIP_REG_AND(INT_IRQ_SOFT, ~BIT_1);
        dp_handle_AbortVirAddrRelay();
    }
    else
    {
        SCI_PASSERT(SCI_FALSE, ("%s, %d 0xA0000000 _softint_handler: Err INT Status", __MODULE__,__LINE__));    /*assert verified*/
    }

    return CALL_HISR;
}
/*****************************************************************************/
//  Description:    This function is used to set SoftINT enable and isr_handle
//                  application.
//  Dependency:     irq_isr_array
//  Author:         Steve.Zhan
//  Note:
/*****************************************************************************/
LOCAL void SoftInt_Init (void)
{
extern void * dp_handle_getVirAddr(void);

    INT_HAL_IRQEnable(TB_SOFT_INT);
    ISR_RegHandler_Ex (TB_SOFT_INT, (TB_ISR)_SoftInt_Callback, _SoftIntHisrFunc, CHIPDRV_HISR_PRIO_2, (dp_handle_getVirAddr()));
}
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

//End of file
