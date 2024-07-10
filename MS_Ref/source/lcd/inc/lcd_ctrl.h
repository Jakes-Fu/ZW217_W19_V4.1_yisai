/****************************************************************************************
 ** File Name:    lcd_ctrl.h                                                            *
 ** Author:       Jianping.Wang                                                         *
 ** DATE:         06/01/2009                                                            *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.                     *
 ** Description:                                                                        *
 ****************************************************************************************/
/****************************************************************************************
 **                   Edit    History                                                   *
 **-------------------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                           *
 ** 06/01/2009    Tim.zhu   Create.                                                     *
 ****************************************************************************************/
#ifndef _LCD_CTRL_H_
#define _LCD_CTRL_H_
/*--------------------------------------------------------------------------------------*
 **                          Dependencies                                               *
 **-------------------------------------------------------------------------------------*/
#include "os_api.h"
#include "sci_types.h"
#include "dal_lcd.h"
#include "lcd_cfg.h"
#include "rotation_app.h"

#include "display.h"
#include "display_cfg.h"
#include "ref_lcd.h"
/**-------------------------------------------------------------------------------------*
 **                          Compiler Flag                                              *
 **-------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
/**-------------------------------------------------------------------------------------*
**                               Micro Define                                          **
**--------------------------------------------------------------------------------------*/

#define OUTPUT_LOG                  SCI_FALSE

#if OUTPUT_LOG
    #define DISPLAY_TRACE           SCI_TRACE_LOW
#else
    #define DISPLAY_TRACE
#endif

#define DEBUG_MODE                  1
#define SUPPORT_LCD_NUM             2
#ifndef WIN32
#define SUPPORT_MENU_NUM            2
#else
#define SUPPORT_MENU_NUM            2
#endif
#define LCD_CTRL_EVENT              1
#define DISPLAY_UPDATE_LAYER_NUM    4 

#define LAYER_SUPPORT_NUM           8
/**-------------------------------------------------------------------------------------*
 **                         data type define                                           **
 **------------------------------------------------------------------------------------ */
typedef enum
{
    LCD_PLACED_VERTICAL,
    LCD_PLACED_HORIZONTAL,
    LCD_PLACED_MAX
}LCD_PLACED_FASHION_E;

typedef struct _lcd_display_ctrl_info_tag
{
    BOOLEAN is_used;
    BOOLEAN is_use_ctrl_event;
    BOOLEAN is_init;
    uint8   padding0;//reserved    
    SCI_SEMAPHORE_PTR lcd_ctrl_sm_ptr[SUPPORT_LCD_NUM];
    uint32 menu_buf_sum[SUPPORT_LCD_NUM];
    uint32 menu_buf_sum_dc[SUPPORT_LCD_NUM];
    LCD_LIMIT_TYPE_E limit_type[SUPPORT_LCD_NUM];
    uint16 logic_width[SUPPORT_LCD_NUM];
    uint16 logic_height[SUPPORT_LCD_NUM];
    uint16 physical_width[SUPPORT_LCD_NUM];
    uint16 physical_height[SUPPORT_LCD_NUM];
    uint16 layer_num[SUPPORT_LCD_NUM];
    uint16 full_copy_flag[SUPPORT_LCD_NUM];
    LCD_PLACED_FASHION_E lcd_direction[SUPPORT_LCD_NUM];
    LCD_BUF_INFO_T menu_buffer_info[SUPPORT_LCD_NUM][SUPPORT_MENU_NUM];
  //  LCD_BUF_INFO_T menu_buffer_dc[SUPPORT_LCD_NUM][SUPPORT_MENU_NUM];
    DISPLAY_OPERATION_MODE_E op_mode[LCD_SUPPORT_MAX];
    SCI_EVENT_GROUP_PTR  lcd_ctrl_event;
    uint32 is_copy_osd[LCD_SUPPORT_MAX];
    DISPLAY_OPERATION_MODE_E previous_mode[LCD_SUPPORT_MAX];
}LCD_DISPLAY_CTRL_INFO_T;
/**-------------------------------------------------------------------------------------*
**                         Functions                                                   **
**--------------------------------------------------------------------------------------*/

/****************************************************************************************/
// Description: coordinate convert from phsical plane to logic plane
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC ERR_DISPLAY_E _convert_coordinate (LCD_ID_E  lcd_id, 
                                     DISPLAY_CONVERT_INPUT_T *out_ptr,
                                     DISPLAY_CONVERT_OUTPUT_T *input_ptr
                                     );

/****************************************************************************************/
// Description: invalidate on simulator
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC void _invalidate_on_simulator( LCD_ID_E  lcd_id, 
                                            DISPLAY_LAYER_INFO_T *layer_arr[],
                                            uint32 array_size,
                                            DISPLAY_INVALIDATE_POS_T *param_ptr);

/****************************************************************************************/
// Description: invalidate on nand platform
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC void _invalidate_on_lcd( LCD_ID_E    lcd_id, 
                                     DISPLAY_LAYER_INFO_T *layer_arr[],
                                     uint32 layer_num,
                                     DISPLAY_INVALIDATE_POS_T *param_ptr);

/****************************************************************************************/
// Description: set event
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC void _set_ctrl_event(uint32 set_flags);

/****************************************************************************************/
// Description: check parameter
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC ERR_DISPLAY_E _check_convert_param(LCD_ID_E lcd_id,
                                             DISPLAY_CONVERT_INPUT_T *input_ptr);

/****************************************************************************************/
// Description: get byte number for data type
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC uint32 _get_byte_num(DISPLAY_DATA_TYPE_E data_format);

/****************************************************************************************/
// Description: convert configuration
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC void _convert_cfg(DISPLAY_LAYER_INFO_T *blk_ptr, BLOCKCFG_T *cfg_ptr);

/****************************************************************************************/
// Description: data format convert when captrue mode
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC LCD_DATA_FORMAT_E _cap_data_format_convert(DISPLAY_DATA_TYPE_E type);

/****************************************************************************************/
// Description: 
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC void _initial_lcd_memory(LCD_ID_E lcd_id);

/****************************************************************************************/
// Description: get information of display control
// Author: Jianping.Wang
/****************************************************************************************/
PUBLIC LCD_DISPLAY_CTRL_INFO_T *_get_ctrl_info(void);
/****************************************************************************************/
//  Description: 
//  Global resource dependence:
//  Author: Jianping.wang
//  Note:
/****************************************************************************************/
PUBLIC void _display_init(void);

/**--------------------------------------------------------------------------------------*
**                         Compiler Flag                                                **
**---------------------------------------------------------------------------------------*/


#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
