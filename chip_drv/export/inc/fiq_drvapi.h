/******************************************************************************
 ** File Name:      fiq_drvapi.h                                              *
 ** Author:         Yuhua.Shi                                              *
 ** DATE:           06/01/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the related driver of fiq include dsp fiq     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
  ** 06/01/2009     Yuhua.Shi        Create                           *
******************************************************************************/
#ifndef _FIQ_DRVAPI_H_
#define _FIQ_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

#ifndef WIN32
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// Return value.
#define FIQ_NULL                0
#define FIQ_SUCCESS             1
#define FIQ_ISR_NULL            2
#define FIQ_NO_CALLBACK         3
#define FIQ_CALLBACK_ASSIGNED   4
#define FIQ_ISR_ASSIGNED        5
#define FIQ_PTR_ERROR           6
#define FIQ_INT_ERROR           7
#define FIQ_PARAM_ERROR         8

//timer1 enable parameter
#define TIMER_INT_ENABLE        1
#define TIMER_INT_DISABLE       0
#define TIMER_COUNT_FULL        0x7FFFFF

//define the time value for using polling way
#define HOLD_ARM_TIME_BY_POLLING 0xFFFFFFFF

typedef enum
{
    REF_CMD_SLEEP_START,
    REF_CMD_WAKEDUP,
    REF_CMD_GET_ABS_TIME,
    REF_CMD_ASSERT,
    REF_CMD_SLEEP_END,
    REF_CMD_IRQ_DSP_INT,
    REF_CMD_IRQ_INT,
    REF_CMD_IRQ_RTC_INT,
    REF_CMD_IRQ_DSP_FRAME
} Ref_CMD_E;

typedef struct
{
    uint32   message;
    uint32   wparam;
    void     *lparam;
} FIQ_MSG;


// ISR function pointer prototype.
typedef void (* FIQ_HANDLE) (uint32);

// CallBack function pointer prototype.
typedef void (* FIQ_CALLBACK) (FIQ_MSG *);

typedef struct
{
    uint16 Cmd_ID;           // 命令类型
    uint16 SeqNr;            // 保留
    uint16 cmd_done;         // DSP是否完成上次操作
    int16  TimeValid;        // REF是否成功的将补偿的时间信息写入
    uint16 Time_FN;          // Frame number 4.615ms
    uint16 Time_QBC;         // Number of QBC, the minimal accuracy is 100 QBC
    uint16 reserved1;
    uint16 reserved2;
} REF_CMD_T;

/* The end of memory access type-defined for MCU */
// The following defines memory for DSP access
// REF Response struct defined
typedef struct
{
    uint16 Cmd_ID;                  // 命令类型
    uint16 SeqNr;                   // 保留
    uint16 Resp_done;               // REF是否完成上次操作
    uint16 abs_time_milli_sec_low; // 当前的绝对时间信息，最小单位是毫秒, 无效值为0xffffffff
    uint16 abs_time_milli_sec_high;
    uint16 wakeup_abs_time_valid;  // 1 abs time valid; 0 invalid. Ref set it to 0 after 1
    uint16 reserved2;
} DSP_RESP_T;

typedef struct
{
    REF_CMD_T              RefCmd_block;
    DSP_RESP_T             DSPRes_block;
} DSP_MCU_CMD_T;


extern volatile DSP_MCU_CMD_T *cmd_ptr;

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// Description :    This function is used to init the module of dsp hold arm
// Global resource dependence :
// Author : 
// Note :
/*****************************************************************************/
PUBLIC void DSP_HoldArmInit(void);

/*****************************************************************************/
// Description :    This function is used to check whether arm hold is enable
// Global resource dependence :
// Author :  Yuhua.Shi
// Note   :  SCI_TRUE:enable, SCI_FALSE:disable
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsEnalbeArmHold (void);

/*****************************************************************************/
// Description :    This function is used to enable the arm hold function.
// Global resource dependence : s_arm_hold_cnt
// Author :
// Note : is_enable :   TURE: hold the arm; FALSE: not hold the arm
/*****************************************************************************/
PUBLIC void SCI_EnableArmHold (
    BOOLEAN is_enable       //TURE: hold the arm; FALSE: not hold the arm
);

/*****************************************************************************/
// Description :    This function is used to hold arm not to operate emc.
// Global resource dependence : s_arm_hold_cnt
// Author :
// Note : hold_time: if equal to 0xFFFFFFFF, it will polling the exit flag which set by dsp, else it
//will only hold arm the time defined by hold_time and exit directly.
/*****************************************************************************/
PUBLIC void DSP_HoldArmByFIQ (uint32 hold_time);

/*****************************************************************************/
// Description :    This function is used to enable the function to speed down dsp queue
// Global resource dependence : s_dsp_queue_sd_cnt
// Author :
// Note :
/*****************************************************************************/
PUBLIC void SCI_EnableDspQueueSpeedDown (BOOLEAN is_enable);

/*****************************************************************************/
// Description :    This function is used to check whether need to speed down dsp queue
// Global resource dependence : s_arm_hold_cnt
// Author :
// Note :
/*****************************************************************************/
PUBLIC BOOLEAN SCI_IsNeedDspQueueSpeedDown (void);

//*****************************************************************************
//  Description:    This function is called to open share memory clock.
//  Dependency:
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC void SCI_ShareMemClkOpen (void);
//*****************************************************************************
//  Description:    This function is called to close share memory clock.
//  Dependency:
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC void SCI_ShareMemClkClose (void);
//*****************************************************************************
//  Description:    This function is called to get share memory clock status.
//  Dependency:
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC uint32 SCI_ShareMemClkStat (void);

/*****************************************************************************/
// Description :    Timer1 fiq handler
// Global resource dependence :
// Author : Yuhua.Shi
// Note :
/*****************************************************************************/
PUBLIC void TIMER1_FiqHandle (
    uint32 int_num          // Interrupt num.
);

//*****************************************************************************
//  Description:    This function should be called at the startup of the
//                  application. It initializes FIQ interrupt resources.
//                  This must be called before installing an IRQ handler.
//  Dependency:     s_fiq_array
//  Author:         Andy.Chen
//    Note:
//*****************************************************************************
PUBLIC void FIQ_Init (void);

//*****************************************************************************
//  Description:    This function is called when an FIQ happens.
//  Dependency:     s_fiq_array
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC void ARM_FIQ_Handler (void);

/*****************************************************************************/
//  Description:    This function is called to register a new FIQ.
//  Dependency:     s_fiq_array
//  Author:         Andy.Chen
//  Note:           If success, the function returns FIQ_SUCCESS;
//                  if fiq_handler is NULL, returns FIQ_PTR_ERROR;
//                  if fiq_num is a invalid number, returns FIQ_INT_ERROR;
//                  if the fiq_handler is installed before, returns
//                  FIQ_ISR_ASSIGNED.
/*****************************************************************************/
PUBLIC uint32 FIQ_RegHandler (
    uint32      fiq_num,        // Interrupt number
    FIQ_HANDLE  fiq_handler     // Interrupt service routine pointer.
);

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
);

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
);

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
);

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
);


//*****************************************************************************
//  Description:    This function is called to clear DSP FIQ.
//  Dependency:
//  Author:         Andy.Chen
//  Note:
//*****************************************************************************
PUBLIC void SCI_ClrDspFiqInt (void);


#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif //

#endif // _FIQ_DRVAPI_H_