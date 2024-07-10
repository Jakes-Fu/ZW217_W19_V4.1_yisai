/******************************************************************************
 ** File Name:    ivsp_drv_sc6530.h                                           *
 ** Author:       Baggio.He                                                   *
 ** DATE:         10/17/2011                                                  *
 ** Copyright:    2011 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 10/17/2011    Baggio.He       Create.                                     *
 ******************************************************************************/
#ifndef _IVSP_SC6530_DRV_H_
#define _IVSP_SC6530_DRV_H_

/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
 #include "os_api.h"
 #include "sci_types.h"

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

#define IVSP_MODULE_BASE           0x20200000
#define IVSP_INT_STATUS_OFFSET     0x20
#define IVSP_INT_MASK_OFFSET       0x24
#define IVSP_INT_CLEAR_OFFSET      0x28

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/

typedef void (*ISP_VSP_ISR_FUNC)(void*);

typedef enum
{
    IVSP_IRQ_SENSOR_SOF = 0,
    IVSP_IRQ_SENSOR_EOF,
    IVSP_IRQ_CAP_SOF,
    IVSP_IRQ_CAP_EOF,
    IVSP_IRQ_TX_DONE,
    IVSP_IRQ_CAP_LINE_ERR,
    IVSP_IRQ_CAP_FIFO_OV,
    IVSP_IRQ_BSM_DONE,
    IVSP_IRQ_VLC_DONE,
    IVSP_IRQ_MBIO_DONE,
    IVSP_IRQ_CAP_FRAME_ERR,
    IVSP_IRQ_JPEG_BUF_OV,   
    IVSP_IRQ_VSP_TIMEOUT,
    IVSP_IRQ_VSP_VLD_ERR,   
    IVSP_IRQ_VSP_CMD_DONE,
    IVSP_IRQ_VSP_CMD_TIMEOUT,
    IVSP_IRQ_ISP_ERR_HANDLE,
    IVSP_IRQ_MAX
}IVSP_IRQ_ID_E;

typedef enum
{
    IVSP_MODE_ISP = 0,  
    IVSP_MODE_VSP,
    IVSP_MODE_IDLE,
    IVSP_MODE_MAX
}IVSP_MODE_E;

uint32 IVsp_SwitchModeEx(IVSP_MODE_E mode,BOOLEAN is_enter, uint32 wait_option);
    
PUBLIC void   IVsp_SwitchMode(IVSP_MODE_E mode,BOOLEAN is_enter);

PUBLIC uint32 IVsp_Open(IVSP_MODE_E mode, uint32 wait_option);

PUBLIC uint32 IVsp_Close(IVSP_MODE_E mode);


PUBLIC uint32 IVsp_RegisterIntFunc(IVSP_IRQ_ID_E int_num, ISP_VSP_ISR_FUNC isr_func);

PUBLIC uint32 IVsp_UnRegisterIntFunc(IVSP_IRQ_ID_E int_num);

PUBLIC void IVSP_TestPoint(uint32 param1, uint32 param2);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

/**---------------------------------------------------------------------------*/
#endif // for _IVSP_SC6530_DRV_H_
