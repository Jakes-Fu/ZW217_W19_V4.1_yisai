/*****************************************************************************
** File Name:      mmitheme_softkey.h                                        *
** Author:         Great.Tian                                                *
** Date:           03/9/2011                                                 *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2011        Xiaoqing.lu      Create								     *
**																		     *
*****************************************************************************/

#ifndef  _MMITHEME_SOFTKEY_H_    
#define  _MMITHEME_SOFTKEY_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "guicommon.h"
#include "guiblock.h"
#include "guisoftkey.h"
#include "guifont.h"
#include "guititle.h"
#include "guiedit.h"
#include "guiwin.h"
#include "ui_layer.h"
#include "mmitheme_pos.h"
#include "mmitheme_update.h"
#include "mmi_default.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

#ifdef MAINLCD_SIZE_320X480

//button位置
#define MMITHEME_SOFTKEY_1BUTTON_H_SIDE_MARGIN    75
#define MMITHEME_SOFTKEY_1BUTTON_V_SIDE_MARGIN    7//4
#define MMITHEME_SOFTKEY_1BUTTON_H_ITEM_SPACE     75
#define MMITHEME_SOFTKEY_1BUTTON_V_ITEM_SPACE     7

#define MMITHEME_SOFTKEY_2BUTTON_H_SIDE_MARGIN    7
#define MMITHEME_SOFTKEY_2BUTTON_V_SIDE_MARGIN    7//4
#define MMITHEME_SOFTKEY_2BUTTON_H_ITEM_SPACE     6
#define MMITHEME_SOFTKEY_2BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_SOFTKEY_3BUTTON_H_SIDE_MARGIN    3
#define MMITHEME_SOFTKEY_3BUTTON_V_SIDE_MARGIN    7//4
#define MMITHEME_SOFTKEY_3BUTTON_H_ITEM_SPACE     7
#define MMITHEME_SOFTKEY_3BUTTON_V_ITEM_SPACE     0 //7

// -----------------------------以下是横屏的位置信息-----------------------------//

#define MMITHEME_H_SOFTKEY_1BUTTON_H_SIDE_MARGIN    125
#define MMITHEME_H_SOFTKEY_1BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_1BUTTON_H_ITEM_SPACE     0//75
#define MMITHEME_H_SOFTKEY_1BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_H_SOFTKEY_2BUTTON_H_SIDE_MARGIN    27
#define MMITHEME_H_SOFTKEY_2BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_2BUTTON_H_ITEM_SPACE     26
#define MMITHEME_H_SOFTKEY_2BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_H_SOFTKEY_3BUTTON_H_SIDE_MARGIN    15
#define MMITHEME_H_SOFTKEY_3BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_3BUTTON_H_ITEM_SPACE     15
#define MMITHEME_H_SOFTKEY_3BUTTON_V_ITEM_SPACE     0 //7

#elif defined MAINLCD_SIZE_240X400
        
//button位置
#define MMITHEME_SOFTKEY_1BUTTON_H_SIDE_MARGIN    75
#define MMITHEME_SOFTKEY_1BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_SOFTKEY_1BUTTON_H_ITEM_SPACE     75
#define MMITHEME_SOFTKEY_1BUTTON_V_ITEM_SPACE     7

#define MMITHEME_SOFTKEY_2BUTTON_H_SIDE_MARGIN    7
#define MMITHEME_SOFTKEY_2BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_SOFTKEY_2BUTTON_H_ITEM_SPACE     6
#define MMITHEME_SOFTKEY_2BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_SOFTKEY_3BUTTON_H_SIDE_MARGIN    3
#define MMITHEME_SOFTKEY_3BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_SOFTKEY_3BUTTON_H_ITEM_SPACE     7
#define MMITHEME_SOFTKEY_3BUTTON_V_ITEM_SPACE     0 //7

// -----------------------------以下是横屏的位置信息-----------------------------//

#define MMITHEME_H_SOFTKEY_1BUTTON_H_SIDE_MARGIN    125
#define MMITHEME_H_SOFTKEY_1BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_1BUTTON_H_ITEM_SPACE     0//75
#define MMITHEME_H_SOFTKEY_1BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_H_SOFTKEY_2BUTTON_H_SIDE_MARGIN    27
#define MMITHEME_H_SOFTKEY_2BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_2BUTTON_H_ITEM_SPACE     26
#define MMITHEME_H_SOFTKEY_2BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_H_SOFTKEY_3BUTTON_H_SIDE_MARGIN    15
#define MMITHEME_H_SOFTKEY_3BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_3BUTTON_H_ITEM_SPACE     15
#define MMITHEME_H_SOFTKEY_3BUTTON_V_ITEM_SPACE     0 //7

#elif defined MAINLCD_SIZE_240X320
        
//button位置
#define MMITHEME_SOFTKEY_1BUTTON_H_SIDE_MARGIN    DP2PX_VALUE(75)
#define MMITHEME_SOFTKEY_1BUTTON_V_SIDE_MARGIN    DP2PX_VALUE(4)
#define MMITHEME_SOFTKEY_1BUTTON_H_ITEM_SPACE     DP2PX_VALUE(75)
#define MMITHEME_SOFTKEY_1BUTTON_V_ITEM_SPACE     DP2PX_VALUE(7)

#define MMITHEME_SOFTKEY_2BUTTON_H_SIDE_MARGIN    DP2PX_VALUE(7)
#define MMITHEME_SOFTKEY_2BUTTON_V_SIDE_MARGIN    DP2PX_VALUE(4)
#define MMITHEME_SOFTKEY_2BUTTON_H_ITEM_SPACE     DP2PX_VALUE(6)
#define MMITHEME_SOFTKEY_2BUTTON_V_ITEM_SPACE     DP2PX_VALUE(0)//7

#define MMITHEME_SOFTKEY_3BUTTON_H_SIDE_MARGIN    DP2PX_VALUE(3)
#define MMITHEME_SOFTKEY_3BUTTON_V_SIDE_MARGIN    DP2PX_VALUE(4)
#define MMITHEME_SOFTKEY_3BUTTON_H_ITEM_SPACE     DP2PX_VALUE(7)
#define MMITHEME_SOFTKEY_3BUTTON_V_ITEM_SPACE     DP2PX_VALUE(0) //7

// -----------------------------以下是横屏的位置信息-----------------------------//

#define MMITHEME_H_SOFTKEY_1BUTTON_H_SIDE_MARGIN    DP2PX_VALUE(85)  
#define MMITHEME_H_SOFTKEY_1BUTTON_V_SIDE_MARGIN    DP2PX_VALUE(4)
#define MMITHEME_H_SOFTKEY_1BUTTON_H_ITEM_SPACE     DP2PX_VALUE(0)//75
#define MMITHEME_H_SOFTKEY_1BUTTON_V_ITEM_SPACE     DP2PX_VALUE(0)//7

#define MMITHEME_H_SOFTKEY_2BUTTON_H_SIDE_MARGIN    DP2PX_VALUE(27)
#define MMITHEME_H_SOFTKEY_2BUTTON_V_SIDE_MARGIN    DP2PX_VALUE(4)
#define MMITHEME_H_SOFTKEY_2BUTTON_H_ITEM_SPACE     DP2PX_VALUE(26)
#define MMITHEME_H_SOFTKEY_2BUTTON_V_ITEM_SPACE     DP2PX_VALUE(0)//7

#define MMITHEME_H_SOFTKEY_3BUTTON_H_SIDE_MARGIN    DP2PX_VALUE(15)
#define MMITHEME_H_SOFTKEY_3BUTTON_V_SIDE_MARGIN    DP2PX_VALUE(4)
#define MMITHEME_H_SOFTKEY_3BUTTON_H_ITEM_SPACE     DP2PX_VALUE(15)
#define MMITHEME_H_SOFTKEY_3BUTTON_V_ITEM_SPACE     DP2PX_VALUE(0) //7

#else
        
//button位置
#define MMITHEME_SOFTKEY_1BUTTON_H_SIDE_MARGIN    75
#define MMITHEME_SOFTKEY_1BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_SOFTKEY_1BUTTON_H_ITEM_SPACE     75
#define MMITHEME_SOFTKEY_1BUTTON_V_ITEM_SPACE     7

#define MMITHEME_SOFTKEY_2BUTTON_H_SIDE_MARGIN    7
#define MMITHEME_SOFTKEY_2BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_SOFTKEY_2BUTTON_H_ITEM_SPACE     6
#define MMITHEME_SOFTKEY_2BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_SOFTKEY_3BUTTON_H_SIDE_MARGIN    3
#define MMITHEME_SOFTKEY_3BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_SOFTKEY_3BUTTON_H_ITEM_SPACE     7
#define MMITHEME_SOFTKEY_3BUTTON_V_ITEM_SPACE     0 //7

// -----------------------------以下是横屏的位置信息-----------------------------//

#define MMITHEME_H_SOFTKEY_1BUTTON_H_SIDE_MARGIN    125
#define MMITHEME_H_SOFTKEY_1BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_1BUTTON_H_ITEM_SPACE     0//75
#define MMITHEME_H_SOFTKEY_1BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_H_SOFTKEY_2BUTTON_H_SIDE_MARGIN    27
#define MMITHEME_H_SOFTKEY_2BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_2BUTTON_H_ITEM_SPACE     26
#define MMITHEME_H_SOFTKEY_2BUTTON_V_ITEM_SPACE     0//7

#define MMITHEME_H_SOFTKEY_3BUTTON_H_SIDE_MARGIN    15
#define MMITHEME_H_SOFTKEY_3BUTTON_V_SIDE_MARGIN    4
#define MMITHEME_H_SOFTKEY_3BUTTON_H_ITEM_SPACE     15
#define MMITHEME_H_SOFTKEY_3BUTTON_V_ITEM_SPACE     0 //7

#endif

#define MMITHEME_SOFTKEY_BACKGROUND_COLOR   MMI_DARK_GRAY_COLOR
/*----------------------------------------------------------------------------*/
/*                         type and structure                                 */
/*----------------------------------------------------------------------------*/
//文本对齐方式
typedef enum
{
    TEXT_ALIGN_LEFT,                     /*!< 左对齐 */
    TEXT_ALIGN_RIGHT,                    /*!< 右对齐 */
    TEXT_ALIGN_HMIDDLE,                  /*!< 水平居中 */
    TEXT_ALIGN_TOP,                      /*!< 上对齐 */
    TEXT_ALIGN_BOTTOM,                   /*!< 下对齐 */
    TEXT_ALIGN_VMIDDLE,                  /*!< 垂直居中 */
    TEXT_ALIGN_LVMIDDLE,                 /*!< 水平方向左对齐，垂直方向居中 */
    TEXT_ALIGN_RVMIDDLE,                 /*!< 水平方向右对齐，垂直方向居中 */
    TEXT_ALIGN_HVMIDDLE                  /*!< 水平居中，垂直居中 */
}TEXT_ALIGN_E;

// softkey theme structure
typedef struct 
{
    uint32						bg_type;					/*!< background type */
    MMI_IMAGE_ID_T				bg_image;					/*!< background image */
    GUI_COLOR_T					bg_color;					/*!< background color */
    uint8                       bg_alpha;

    GUI_COLOR_T					font_color;					/*!< font color */
    GUI_COLOR_T					pressed_font_color; 		/*!< font color */
    GUI_FONT_T					font;						/*!< font */    // 这个参数暂时没用上

    uint16                      first_icon_offset;          // 第一个图标的偏移量
    uint16                      second_icon_offset;         // 第二个图标的偏移量
    uint16                      third_icon_offset;          // 第三个图标的偏移量
    MMK_WIN_BUTTON_T            skb_button[GUISOFTKEY_BUTTON_NUM];
    TEXT_ALIGN_E                align[GUISOFTKEY_BUTTON_NUM];
}MMI_SOFTKEY_STYLE_T;

//对应每种style的按钮数量，按钮布局，按钮字体
typedef struct 
{
//    uint32 button_num;
//    GUI_FONT_ALL_T font;
//    MMI_CTRL_ID_T button_ctrl_id[GUISOFTKEY_BUTTON_NUM];
//    BOOLEAN button_active[GUISOFTKEY_BUTTON_NUM];
//    GUI_BOTH_RECT_T button_both_rect[GUISOFTKEY_BUTTON_NUM];
//    MMI_IMAGE_ID_T img_id[GUISOFTKEY_BUTTON_NUM];
//    MMI_IMAGE_ID_T pressed_img_id[GUISOFTKEY_BUTTON_NUM];
//    MMI_IMAGE_ID_T grayed_img_id[GUISOFTKEY_BUTTON_NUM];
    GUI_FONT_ALL_T font;
    MMI_IMAGE_ID_T img_id;
    MMI_IMAGE_ID_T pressed_img_id;
    MMI_IMAGE_ID_T grayed_img_id;    
    MMI_IMAGE_ID_T bg_image;    //背景图
}MMI_BUTTONSOFTKEY_STYLE_T;


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get softkey style
//	Global resource dependence : 
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetSoftkeyStyle(MMI_SOFTKEY_STYLE_T* softkey_style_ptr);

/*****************************************************************************/
// 	Description : get the default softkey image info
//	Global resource dependence : 
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDefaultSoftkeyImg(
                                          GUISOFTKEY_IMG_T    *img_ptr    //in/out
                                          );

/*****************************************************************************/
// 	Description : get the mid softkey is null
//	Global resource dependence : 
//  Author: nanji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMidkeyNull(
                                          MMI_TEXT_ID_T text_id
                                          );
/*****************************************************************************/
// 	Description : get the height of the softkey area, is called by the MMI Kernel
//	Global resource dependence : 
//  Author:Robert.Lu
//	Note:
/*****************************************************************************/
PUBLIC uint32  MMITHEME_GetSoftkeyHeight(void);

/*****************************************************************************/
//  Description : get win softkey height
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetWinSoftkeyHeight(
                                           MMI_HANDLE_T win_handle
                                           );
                                           
/*****************************************************************************/
// 	Description : copy rect
//	Global resource dependence : 
//  Author:lin.lin
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetSoftKeyTextRect(
                                              GUI_ALIGN_E    align_num,
                                              GUI_RECT_T     rect
                                              );

/*****************************************************************************/
// 	Description : get softkey text font
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC GUI_FONT_T MMITHEME_GetSoftKeyTextFont(void);


/*****************************************************************************/
// 	Description : get the softkey bar rect
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyCtrlId(void);

/*****************************************************************************/
// 	Description : get the softkey button ctrl id
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetSoftkeyButtonCtrlId(
                                            MMI_CTRL_ID_T *button_ctrl_id_arr     //out
                                            );

/*****************************************************************************/
// 	Description : get the button softkey style, is called by the softkey control
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetButtonSoftkeyStyle(
                                              MMI_BUTTONSOFTKEY_STYLE_T* softkey_style_ptr
                                              );

/*****************************************************************************/
// Description : calc softkey button rect
// Global resource dependence : 
// Author: Jibin
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_CalcSoftkeyButtonRect(
                             const GUI_RECT_T *rect_ptr,
                             GUISOFTKEY_BUTTONSTYLE_E style,
                             GUI_RECT_T *button_rect_ptr   //out
                             );
/*****************************************************************************/
// 	Description : get mid softkey image whose style is GUISOFTKEY_STYLE_BAR, 
//	Global resource dependence : 
//  Author: hongjun.jia
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetBarMidSoftkeyImage(void);


/*****************************************************************************/
// 	Description : get common softkey background color
//	Global resource dependence : 
//  Author: chunjuan.liang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetSoftkeyBg(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif // _MMITHEME_SOFTKEY_H_
