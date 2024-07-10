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

//buttonλ��
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

// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

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
        
//buttonλ��
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

// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

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
        
//buttonλ��
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

// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

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
        
//buttonλ��
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

// -----------------------------�����Ǻ�����λ����Ϣ-----------------------------//

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
//�ı����뷽ʽ
typedef enum
{
    TEXT_ALIGN_LEFT,                     /*!< ����� */
    TEXT_ALIGN_RIGHT,                    /*!< �Ҷ��� */
    TEXT_ALIGN_HMIDDLE,                  /*!< ˮƽ���� */
    TEXT_ALIGN_TOP,                      /*!< �϶��� */
    TEXT_ALIGN_BOTTOM,                   /*!< �¶��� */
    TEXT_ALIGN_VMIDDLE,                  /*!< ��ֱ���� */
    TEXT_ALIGN_LVMIDDLE,                 /*!< ˮƽ��������룬��ֱ������� */
    TEXT_ALIGN_RVMIDDLE,                 /*!< ˮƽ�����Ҷ��룬��ֱ������� */
    TEXT_ALIGN_HVMIDDLE                  /*!< ˮƽ���У���ֱ���� */
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
    GUI_FONT_T					font;						/*!< font */    // ���������ʱû����

    uint16                      first_icon_offset;          // ��һ��ͼ���ƫ����
    uint16                      second_icon_offset;         // �ڶ���ͼ���ƫ����
    uint16                      third_icon_offset;          // ������ͼ���ƫ����
    MMK_WIN_BUTTON_T            skb_button[GUISOFTKEY_BUTTON_NUM];
    TEXT_ALIGN_E                align[GUISOFTKEY_BUTTON_NUM];
}MMI_SOFTKEY_STYLE_T;

//��Ӧÿ��style�İ�ť��������ť���֣���ť����
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
    MMI_IMAGE_ID_T bg_image;    //����ͼ
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
