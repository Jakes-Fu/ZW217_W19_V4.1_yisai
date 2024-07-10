#ifndef _MMIIM_SOGOU_H_
#define _MMIIM_SOGOU_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmiim_base.h"
#include "WD_Def.h"
#include "wdp_SG_Interface_net.h"
#if defined(MMIIM_USE_KB_SOGOU)

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIIM_SOGOU_KEY_TYPE_MASK (0xff00)
#define MMIIM_SOGOU_KEY_MASK (0x00ff)
#define MMIIM_SOGOU_VKEY_PRESSSED (0x00000000)
#define MMIIM_SOGOU_VKEY_RELEASED (0x00020000)

#define SOGOU_VK_NONE          0x0
#define SOGOU_VK_LONGCLICK     0x00010000      /* 表示长按键 */

#define SOGOU_EVENT_KEYBOARD       0x0001
#define SOGOU_EVENT_TIMER          0x0002
#define SOGOU_EVENT_CHAR			0x0003
#define SOGOU_EVENT_CHAR_APPEND    0x0003
#define SOGOU_EVENT_CHAR_REPLACE   0x0004

#define MMI_DEFAULT_LASTDISPLAY_NUM  10

#define SOGOU_HW_TRACE_CNT_LIMIT (512)
#define SOGOU_HW_MAXCANDNUM (16)

#if (defined MMI_IM_PDA_SUPPORT && defined IM_SIMP_CHINESE_SUPPORT)	//在中文情况下，才有皮肤功能
#define SOGOU_USE_PDA_SKIN
#endif

#ifdef MAINLCD_SIZE_240X320
#define SG_SKIN_LEFT_WIDTH        144
#define SG_SKIN_SCREEN_P_WIDTH    240
#define SG_SKIN_SCREEN_L_WIDTH    320
#define SG_SKIN_REPEAT_AREA_WIDTH 5
#define SG_SKIN_HEIGHT            90
#elif defined (MAINLCD_SIZE_128X160)
#define SG_SKIN_LEFT_WIDTH        106
#define SG_SKIN_SCREEN_P_WIDTH    176
#define SG_SKIN_SCREEN_L_WIDTH    220
#define SG_SKIN_REPEAT_AREA_WIDTH 5
#define SG_SKIN_HEIGHT            66
#elif defined(MAINLCD_SIZE_176X220)
#define SG_SKIN_LEFT_WIDTH        106
#define SG_SKIN_SCREEN_P_WIDTH    176
#define SG_SKIN_SCREEN_L_WIDTH    220
#define SG_SKIN_REPEAT_AREA_WIDTH 5
#define SG_SKIN_HEIGHT            66
#elif defined (MAINLCD_SIZE_240X400)
#define SG_SKIN_LEFT_WIDTH        144
#define SG_SKIN_SCREEN_P_WIDTH    240
#define SG_SKIN_SCREEN_L_WIDTH    400
#define SG_SKIN_REPEAT_AREA_WIDTH 5
#define SG_SKIN_HEIGHT            90
#elif defined (MAINLCD_SIZE_320X480)
#define SG_SKIN_LEFT_WIDTH        192
#define SG_SKIN_SCREEN_P_WIDTH    320
#define SG_SKIN_SCREEN_L_WIDTH    480
#define SG_SKIN_REPEAT_AREA_WIDTH 5
#define SG_SKIN_HEIGHT            120
#endif


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//ui data
typedef struct
{
    GUI_LCD_DEV_INFO *lcd_info_ptr;
    GUIIM_INIT_KERNEL_T  init_param;
    GUI_RECT_T           rect;
    MMI_WIN_ID_T         win_id;
    MMIIM_PRE_EDIT_WIN_T edit_win;
    MMIIM_CAND_WIN_T     cand_win;
    MMIIM_CAND_WIN_T     letter_win;
    WD_imeResult         imeResult;
    uint16               preeditwin_height;
    uint16               letterwin_height;
    uint16               candwin_height;
    GUIIM_METHOD_T       method;
    WD_UINT8             lastdisp;
    WD_UINT8 		     lastletterdisp;
    BOOLEAN              isHandleLongDel;
} MMIIM_SOGOU_DATA_T;


/*---------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                               */
/*---------------------------------------------------------------------------*/

/******************************************************************************/
// Description: 转化输入法类型
// Global resource dependence: NONE
// Author: Ming.Song
// Note:
/******************************************************************************/
PUBLIC SG_UINT MMIIM_Sogou_ConvertMethod (GUIIM_METHOD_T method);

/*****************************************************************************/
//  Description : ime init
//  Global resource dependence : none
//  Author:
//  Note:imeType 窗口的ID号
/*****************************************************************************/
PUBLIC uint8 WD_ImeInit(uint8 allowed_input, GUIIM_LANGUAGE_T language_type);

#ifdef SOGOU_USE_PDA_SKIN
/******************************************************************************/
// Description: 获取当前设置的皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGGetImeSkin(void);

/******************************************************************************/
// Description: 释放当前皮肤占用的内存空间
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGFreeImeSkinBuffer(void);

/******************************************************************************/
// Description: 是否使用的是默认皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGIsUseDefaultSkin(void);

#endif

/******************************************************************************/
// Description: 是否需要绘制皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGIsDrawSkin(void);

#ifdef SOGOU_USE_PDA_SKIN
/******************************************************************************/
// Description: 在指定区域内绘制皮肤
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_DrawSkinInRect(MMI_WIN_ID_T win_id, GUI_RECT_T *rect_ptr, 
                                       GUILCD_DRAW_DC_T *dc_data_ptr);
#endif
/******************************************************************************/
// Description: 籑MIAPIIM_SGResetDefault
// Global resource dependence: NONE
// Author: Guopeng.Zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN MMIAPIIM_SGResetDefault(void);
PUBLIC BOOLEAN MMIAPIIM_SGResetNv (void);
PUBLIC void MMIAPIIM_SGInit (void);

#ifdef __cplusplus 
extern "C"{ 
#endif 

#ifdef __cplusplus 
} 
#endif 

#endif //MMIIM_USE_KB_SOGOU

#endif //_MMIIM_SOGOU_H_

