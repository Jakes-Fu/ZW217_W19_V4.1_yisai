/******************************************************************************
 ** File Name:    lcdc_app.h                                                  *
 ** Author:       Tim.zhu                                                     *
 ** DATE:         06/01/2009                                                  *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/01/2009    Tim.zhu   Create.                                           *
 ******************************************************************************/
#ifndef _LCDC_APP_H_
#define _LCDC_APP_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"


#include "os_api.h"
#include "tb_hal.h"

#include "lcdc_types.h"
#include "tf_lcdc_drv.h"
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
// Boolean value
#define SCI_TRUE                    TRUE        // Boolean true value
#define SCI_FALSE                   FALSE       // Boolean false value

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/

/*************************************************************************************/
//Description: config the block parameters
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppConfigBlock (LCD_LAYER_ID_E layer_id, void *blk_param_ptr);
/*************************************************************************************/
//Description: config capture parameters
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppConfigCapture (LCDC_CAP_PARAM_T *param_ptr);
/*************************************************************************************/
//Description: set the color key of the destify layer
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppSetColorKey (LCD_LAYER_ID_E layer_id, uint32 colorkey);
/*************************************************************************************/
//Description: set lcdc background color
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppSetBGColor (uint32 bg_color);
/*************************************************************************************/
//Description: enable lcdc dither
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppEnableDither (void);
/*************************************************************************************/
//Description: disable lcdc dither
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppDisableDither (void);
/*************************************************************************************/
//Description: set fmark mode and pol
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppSetFmark (uint32 lcd_id,LCD_FMARK_MODE_E mode, LCD_FMARK_POL_E pol);
/*************************************************************************************/
//Description: set logic size of LCD
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppSetLogicSize (LCDC_SIZE_T *size);
/*************************************************************************************/
//Description: set display rect
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppSetDisplayRect (LCDC_RECT_T *rect);
/*************************************************************************************/
//Description: set parameters for yuv2rgb
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppSetYUV2RGBParam (LCD_YUV2RGB_PARAM_T *param_ptr);
/*************************************************************************************/
//Description:   invalidate a rectangle of the lcdc logic plane to the lcd device, 
//                 must be called after all the parameters having been set
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppInvalidateRect (
    uint32 lcd_id,
    uint16 left,     //left of the rectangle
    uint16 top,     //top of the rectangle
    uint16 right,     //right of the rectangle
    uint16 bottom    //bottom of the rectangle.
);
#ifdef PLATFORM_SC8800G
/*************************************************************************************/
//Description: set cs pin
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC void LCDC_AppSetCSPin (uint32 lcd_id,uint32 cs_pin);
#endif
/*************************************************************************************/
//Description:   initialize lcdc
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppOpen (void);
/*************************************************************************************/
//Description:   close lcdc
//Global resource dependence:
//Author: Shan.he
/*************************************************************************************/
PUBLIC uint32 LCDC_AppClose (void);
/*************************************************************************************/
//Description: register lcdc module irq handle func
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_AppRegIntFunc (LCD_INT_TYPE_E irq_num, LCD_IRQ_FUNC func);
/*************************************************************************************/
//Description: register lcdc module irq handle func
//Global resource dependence:
//Author: Tim.zhu
/*************************************************************************************/
PUBLIC uint32 LCDC_AppUnRegIntFunc (LCD_INT_TYPE_E irq_num);
/*************************************************************************************/
//Description: close lcdc irq
//Global resource dependence:
//Author: Jianping.Wang
/*************************************************************************************/
PUBLIC void LCDC_AppLcdcDone (void);
/*************************************************************************************/
//Description: set if check state of lcdc done
//Global resource dependence:
//Author: Jianping.Wang
/*************************************************************************************/
PUBLIC void LCDC_AppSetCheckState (BOOLEAN is_check);
/*************************************************************************************/
//Description:
//Global resource dependence:
//Author: Jianping.Wang
/*************************************************************************************/
PUBLIC BOOLEAN LCDC_AppGetCheckState (void);
/*************************************************************************************/
//Description:   close all layer
//Global resource dependence:
//Author: Jianping.Wang
/*************************************************************************************/
PUBLIC uint32 LCDC_AppDisableAllLayer (void);
/*************************************************************************************/
//Description: open lcdc irq
//Global resource dependence:
//Author: Jianping.Wang
/*************************************************************************************/
PUBLIC uint32 LCDC_AppOpenInt (LCD_INT_TYPE_E int_type,LCD_IRQ_FUNC irq_func);
/*************************************************************************************/
//Description: close lcdc irq
//Global resource dependence:
//Author: Jianping.Wang
/*************************************************************************************/
PUBLIC uint32 LCDC_AppCloseInt (LCD_INT_TYPE_E irq_num);
/*************************************************************************************/
//Description:   register callback
//Global resource dependence:
//Author: Jianping.Wang
/*************************************************************************************/
PUBLIC uint32 LCDC_AppRegisterCallBack (LCD_INT_TYPE_E type, 
                                                 LCD_IRQ_FUNC callback
                                                );

/********************************************************************************************/
//Description: set the interval between 2 AHB master requests for each client.
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_AppSetAhbReqestGap (uint8 cycles);
/**-----------------------------------------------------------------------------------*
**                                Compiler Flag                                      **
**------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
