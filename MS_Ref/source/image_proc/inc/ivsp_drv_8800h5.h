/******************************************************************************
 ** File Name:    sc6600l_dcam_common.h                                       *
 ** Author:       Jianping.Wang                                               *
 ** DATE:         12/06/2008                                                  *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 12/06/2008    Jianping.Wang   Create.                                     *
 ******************************************************************************/
#ifndef _IVSP_DRV_H_
#define _IVSP_DRV_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
 #include "os_api.h"
//#include "sc8800h_reg_int.h"

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
//reserved the old style
typedef void (*ISP_VSP_IRQ_FUNC)(void);
//interrupt handle function declare
typedef void (*ISP_VSP_LISR_FUNC)(void);
//high lever interrupt handle function
typedef void (*ISP_VSP_HISR_FUNC)(void);

typedef enum
{
    SENSOR_SOF_ID = 0,
    SENSOR_EOF_ID,
    CAP_SOF_ID,
    CAP_EOF_ID,
    ISP_TX_DONE_ID,
    SC_DONE_ID,
    ISP_BUFFER_OVF_ID,
    VSP_BSM_DONE_ID,
    VSP_VLC_DONE_ID,
	VSP_DBK_DONE_ID,	
    CAP_FIFO_OVF_ID,
    JPEG_BUF_OVF_ID,	
    VSP_TIMEOUT_ID,
    VSP_VLD_ERR_ID,	
    VSP_MEA_DONE_ID,
    ISP_INT_ID_MAX
}IVSP_INT_ID_E;

typedef enum
{
    IVSP_CLK_48M = 0,
    IVSP_CLK_24M,
    IVSP_CLK_26M,
    IVSP_CLK_MAX
}IVSP_CLK_E;

typedef enum
{
	IVSP_MODE_ISP,	
	IVSP_MODE_VSP,
	IVSP_MODE_IDLE,
	
	IVSP_MODE_MAX
		
}IVSP_MODE_E;

typedef enum
{
	IVSP_CALLBACK_PRE_CLEAR = 0,
	IVSP_CALLBACK_POST_CLEAR,
	IVSP_CALLBACK_MAX
}IVSP_IRQ_CALLBACK_ID_E;

typedef uint32 (*IVSP_IRQ_CALLBACK)(IVSP_INT_ID_E int_id, uint32 irq_status, uint32 param);

/*****************************************************************************/
//  Description:Swithc isp/vsp mode
//	Global resource dependence:
//  Author: Jianping.wang
//	Note:
//		input:
//			sm    -  semaphore
//		output:
//			none
//		return:
//			none
/*****************************************************************************/
PUBLIC void IVsp_SwitchMode(
		IVSP_MODE_E mode,
		BOOLEAN     is_enter
	);

/****************************************************************************************/
// Description: register dcam module irq handle func
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int32 IVsp_RegisterIntFunc(IVSP_INT_ID_E int_num, ISP_VSP_LISR_FUNC func);

/****************************************************************************************/
// Description: unregister dcam module irq handle func
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC int32 IVsp_UnRegisterIntFunc(IVSP_INT_ID_E int_num);

/****************************************************************************************/
// Description: unregister all dcam module irq handle func
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void IVsp_UnRegisterIntFuncAll(void);

/****************************************************************************************/
// Description: soft reset the dcam modules
// Global resource dependence: 
// Author: Tim.zhu
// Note: When disable cap need to reset dcam modules
/*****************************************************************************************/
PUBLIC void IVsp_SoftReset(void);

/****************************************************************************************/
// Description: IVsp_Stop
// Global resource dependence: 
// Author: Tim.zhu
// Note: When disable cap need to reset dcam modules
/*****************************************************************************************/
PUBLIC void IVsp_Stop(void);

/****************************************************************************************/
// Description: close interrupt
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void IVsp_OpenInt(IVSP_INT_ID_E int_id);

/****************************************************************************************/
// Description: close interrupt
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void IVsp_CloseInt(IVSP_INT_ID_E int_id);

/****************************************************************************************/
// Description: close interrupt
// Global resource dependence: 
// Author: Tim.zhu
// Note:
/*****************************************************************************************/
PUBLIC void IVsp_ClearInt(IVSP_INT_ID_E int_id);

PUBLIC void IVSP_TestPoint(uint32 param1, uint32 param2);

/****************************************************************************************/
// Description: register irq callback
// Global resource dependence: 
// Author: Shan.He
// Note:
/*****************************************************************************************/
PUBLIC void IVsp_RegisterIrqCallback(IVSP_IRQ_CALLBACK_ID_E callback_id,
										IVSP_IRQ_CALLBACK irq_callback);

/****************************************************************************************/
// Description: clear all registered irq callbacks
// Global resource dependence: 
// Author: Shan.He
// Note:
/*****************************************************************************************/
PUBLIC void IVsp_ClearAllIrqCallback(void);


/****************************************************************************************/
// Description: register dcam module irq handle func
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int32 IVsp_RegisterIntFuncEx(IVSP_INT_ID_E int_num, ISP_VSP_LISR_FUNC lisr_func, 
									 ISP_VSP_HISR_FUNC hisr_func);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 