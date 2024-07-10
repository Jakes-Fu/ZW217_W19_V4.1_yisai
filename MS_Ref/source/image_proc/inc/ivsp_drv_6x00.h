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
#define DCAM_IRQ_NUM        14
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
    #if (defined(PLATFORM_SC6600L))    
    SC_DONE_ID,
    #elif (defined(PLATFORM_SC6800H))    
    CAP_LINE_ERR_ID,
    #endif    
    ISP_BUFFER_OVF_ID,
    VSP_BSM_DONE_ID,
    VSP_VLC_DONE_ID,
    VSP_MBIO_DOEN_ID,
    #if (defined(PLATFORM_SC6600L))    
    CAP_FIFO_OVF_ID,
    #elif (defined(PLATFORM_SC6800H))    
    CAP_FRAME_ERR_ID,
    #endif      
    JPEG_BUF_OVF_ID,	
    VSP_TIMEOUT_ID,
    VSP_VLD_ERR_ID,	
    ISP_INT_ID_MAX
}IVSP_INT_ID_E;

typedef enum
{
    #if (defined(PLATFORM_SC6800H))    
    IVSP_CLK_48M = 0,     
    IVSP_CLK_64M,    
    IVSP_CLK_96M,  
    IVSP_CLK_26M,    
    IVSP_CLK_72M,
    IVSP_CLK_12M,
 #if 0
    #elif (defined(PLATFORM_SC6600L))
    IVSP_CLK_48M = 0,    
    IVSP_CLK_24M,
    IVSP_CLK_26M,
    #endif    
#endif

    IVSP_CLK_MAX
}IVSP_CLK_E;

typedef enum
{
	IVSP_MODE_ISP,	
	IVSP_MODE_VSP,
	IVSP_MODE_IDLE,
	
	IVSP_MODE_MAX
		
}IVSP_MODE_E;



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

/*****************************************************************************/
//  Description:dcam module clk enable and dcam module enable
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		return:
//			none
/*****************************************************************************/
PUBLIC void IVsp_SetClk(IVSP_CLK_E clk);

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

/*****************************************************************************/
//  Description:dcam module reset 
//	Global resource dependence:
//  Author: Tim.zhu
//	Note:
//		return:
//			none
/*****************************************************************************/
PUBLIC void _IVsp_ErrHandler(void);

/****************************************************************************************/
// Description: register dcam module irq handle func
// Global resource dependence: 
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC int32 IVsp_RegisterIntFuncEx(IVSP_INT_ID_E int_num, ISP_VSP_LISR_FUNC lisr_func, 
									 ISP_VSP_HISR_FUNC hisr_func);


PUBLIC void IVSP_TestPoint(uint32 param1, uint32 param2);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 