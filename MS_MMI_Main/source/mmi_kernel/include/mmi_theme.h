/*****************************************************************************
** File Name:      mmi_theme.h                                               *
** Author:         Great.Tian                                                *
** Date:           06/21/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe the mmi theme               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2004        Great.Tian       Create								     *
**																		     *
*****************************************************************************/

/*!
 *  \addtogroup mmi_theme_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmi_theme.h
 *  \author	Great.Tian
 *  \date	June,2004
 *  \brief 	This file is used to describe the mmi theme
 */

#ifndef  _MMI_THEME_H_    
#define  _MMI_THEME_H_   

/*----------------------------------------------------------------------------*/
/*                         Include Files                                	  */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
//#include "guicommon.h"
//#include "guiblock.h"
//#include "guisoftkey.h"
//#include "guifont.h"
//#include "guititle.h"
//#include "guiedit.h"
//#include "guiwin.h"
#include "ui_layer.h"
#include "mmitheme_pos.h"
#include "mmitheme_update.h"
#include "mmi_default.h"
#include "mmitheme_title.h"
#include "mmitheme_softkey.h"
#include "mmitheme_richtext.h"
#include "mmitheme_list.h"
//#ifdef GUIF_SCROLLKEY
//#include "guiscrollkey.h"
//#endif
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
typedef enum
{
    MMITHEME_OFFSET_COLOR_POS_3 = 0,
    MMITHEME_OFFSET_COLOR_POS_2,
    MMITHEME_OFFSET_COLOR_POS_1,
    MMITHEME_OFFSET_COLOR_DEFAULT,
    MMITHEME_OFFSET_COLOR_NEG_1,
    MMITHEME_OFFSET_COLOR_NEG_2,
    MMITHEME_OFFSET_COLOR_NEG_3,

	MMITHEME_OFFSET_COLOR_MAX
}MMITHEME_OFFSET_COLOR_E;

typedef enum
{
    MMITHEME_ACCENT_COLOR_VIOLENT = 0,
    MMITHEME_ACCENT_COLOR_RED,
    MMITHEME_ACCENT_COLOR_YELLOW,
    MMITHEME_ACCENT_COLOR_GREEN,
    MMITHEME_ACCENT_COLOR_COPPER,
    MMITHEME_ACCENT_COLOR_BLUE,

	MMITHEME_ACCENT_COLOR_MAX
}MMITHEME_ACCENT_COLOR_E;

PUBLIC GUI_COLOR_T MMITHEME_GetAccentColorByOffset(MMITHEME_OFFSET_COLOR_E offset_id);

#ifdef WIN32
#define GUI_DUMP_CTRL_TREE
#endif

#define MAX_THEME_STYLE			    4   /*!< max number of theme */
#define WEEKDAY_NUM				    7   /*!< day in one week */
// for gui title

#define MMI_KBD_LONG_KEY_VALUE          1500
#define MMI_KBD_START_REPEAT_KEY_VALUE  300
#define MMI_KBD_REPEAT_KEY_VALUE        100
#define MMI_TP_LONG_PRESS_VALUE         600
#define MMI_TP_SHORT_PRESS_VALUE        150
#define MMI_TP_DBLCLK_PRESS_VALUE		360
#define MMI_KDB_DBLCLK_PRESS_VALUE		350

// tp fling recover timer
#define MMITHEME_RUN_RECOVER_TIME  10
/*----------------------------------------------------------------------------*/
/*                         TYPE AND STRUCT                                    */
/*----------------------------------------------------------------------------*/

/*! \brief message box style */
typedef struct 
{
    GUI_COLOR_T					back_color;				/*!< background color */
    MMI_IMAGE_ID_T				back_image_id;			/*!< background image id */
    GUI_COLOR_T					text_back_color;		/*!< background color of text area */
    GUI_COLOR_T					font_color;				/*!< font color */
    GUI_FONT_T					font;					/*!< font */
    uint8						move_line;  			/*!< line moved when stroll once */
    uint8						line_space;				/*!< space between rows */
    uint8       				char_space;				/*!< space between chars */
    uint8						title_height;			/*!< title height */
    GUI_COLOR_T					title_color;			/*!< title background color */
    uint8						img_vspace;				/*!< image y coordinate in message box rect */
    uint8						img_hspace;				/*!< image x coordinate in message box rect*/
    GUI_RECT_T					image_rect;				/*!< image rect */
    GUI_RECT_T					text_rect;				/*!< text display rect */
    GUI_RECT_T					sk_rect;				/*!< soft key rect */
    GUI_COLOR_T					sk_bg_color;			/*!< soft key background color */
    MMI_TEXT_ID_T				sk_text_id[3];			/*!< soft key text id */
    MMI_TEXT_ID_T				title_text_id;			/*!< title text id */
}MMI_MSGBOX_STYLE_T;

/*! \brief calendar style */
typedef struct 
{
    uint16						title_height;           /*!< title height */
    uint16						week_height;            /*!< week height */
    MMI_IMAGE_ID_T				back_image;             /*!< background image */
    GUI_FONT_T					day_font;               /*!< day font */
    GUI_COLOR_T					day_font_color;         /*!< day font color */
    GUI_COLOR_T					day_highlight_color;    /*!< day highlight color */
    GUI_FONT_T					week_font;              /*!< week font */
    GUI_COLOR_T					week_font_color;        /*!< week font color */
    GUI_COLOR_T					saturday_color;         /*!< saturday color */
    GUI_COLOR_T					sunday_color;           /*!< sunday color */
    GUI_FONT_T					title_font;             /*!< title font */
    GUI_COLOR_T					title_font_color;       /*!< title font color */
    GUI_COLOR_T					title_back_color;       /*!< title background color */
    GUI_COLOR_T					week_back_color;        /*!< week background color */
    GUI_COLOR_T					day_back_color;         /*!< day background color */
}MMI_CALENDAR_STYLE_T;

//@xiefen add 2006-06-08 start
/*! \brief graph style */
typedef struct
{
    GUI_COLOR_T              x_axis_color;               /*!< x_axis_color*/
    GUI_COLOR_T              y_axis_color;               /*!<y_axis_color*/
    GUI_COLOR_T              graph_color;               /*!<graph_color*/    
    GUI_COLOR_T              back_color;      /*!< back_color*/
}MMI_GRAPH_STYLE_T;
//@xiefen add 2006-06-08 end

//all font color for a theme
// 这里的值必须要与s_theme_font_color_arry对应，还要在common_mdu_def.h里RES_ADD_COLOR
typedef enum  
{
    MMI_THEME_TITLE,
    MMI_THEME_MENU,
    MMI_THEME_POPMENU,
    MMI_THEME_MAINMENU_FONT,    
    MMI_THEME_PUBWIN_SOFTKEY_FONT,  
    MMI_THEME_TAB_FONT,      
    MMI_THEME_SOFTKEY_FONT,    
    MMI_THEME_SOFTKEY_PRESSED,  
    MMI_THEME_LABEL_FONT,    
    MMI_THEME_LIST_ITEM0,      
    MMI_THEME_LIST_ITEM1,       
    MMI_THEME_LIST_ITEM2,       
    MMI_THEME_LIST_ITEM_SMALL_FONT,
    MMI_THEME_CALENDOR_TITLE,
    MMI_THEME_IDLE_SOFTKEY,  
    MMI_THEME_IDLE_TIME,      
    MMI_THEME_IDLE_DATE,        
    MMI_THEME_IDLE_WEEK, 
    MMI_THEME_IDLE_IDLEWORD, 
    MMI_THEME_IDLE_MP3,
    MMI_THEME_IDLE_KUR,         
    MMI_THEME_IDLE_FM,         
    MMI_THEME_IDLE_COUNTTIME,
    MMI_THEME_IDLE_NETWORKNAME,
    MMI_THEME_IDLE_PROVIDERNAME,
    MMI_THEME_IDLE_CALENDAR,
    MMI_THEME_IDLE_STK,
    MMI_THEME_BACKGROUND_TIPS,
    MMI_THEME_EDIT_FONT,
    MMI_THEME_POPMENU_BG,
    MMI_THEME_POPMENU_BG_BORDER,
    MMI_THEME_CALENDAR_DAY,
    MMI_THEME_CALENDAR_WEEK,
    MMI_THEME_CALENDAR_YEAR,
    MMI_THEME_CALENDAR_MONTH,
    MMI_THEME_CALENDAR_TASK,
    MMI_THEME_TIPS_FONT,
    MMI_THEME_RICHTEXT_FONT,
    MMI_THEME_TEXT_FONT,
    MMI_THEME_RICHTEXT_BORDER,
    MMI_THEME_MMS_RICHTEXT_BORDER,
    MMI_THEME_SELECT,
    MMI_THEME_FORM_BG,
    MMI_THEME_CUBEMENU_TOP_SIDE,
    MMI_THEME_VKB_KEY_FONT_COLOR,     //虚拟键盘按键字符颜色
    MMI_THEME_VKB_KEY_FONT_COLOR_HIGHLIGHT,     //虚拟键盘按键高亮字符颜色
    MMI_THEME_SPLIT_LINE,
    MMI_THEME_MASK_LAYER,
    MMI_THEME_PICKER_CUR,
    MMI_THEME_SUB_FONT,
    MMI_THEME_EDIT_DEFAULT_FONT,
    MMI_THEME_PICKER_NORMAL,
    MMI_THEME_PICKER_TIPS,
    MMI_THEME_CONTEXTMENU_TITLE_FONT,
    MMI_THEME_PUBWIN_BG,
    
    MMI_THEME_FONT_COLOR_INDEX_MAX
}MMI_THEME_FONT_COLOR_INDEX_E;

typedef enum
{
    MMI_DIRECTION_LEFT,
    MMI_DIRECTION_RIGHT,
    MMI_DIRECTION_HMIDDLE
} MMI_DIRECTION_SET_E;

typedef enum
{
    MMITHEME_PB_ENCODE_NONE,
    MMITHEME_PB_ENCODE_80,
    MMITHEME_PB_ENCODE_81,
    MMITHEME_PB_ENCODE_82
} MMITHEME_PB_ENCODE_TYPE_E;

typedef enum
{
    MMITHEME_HIDE_NONE,
    MMITHEME_HIDE_FADE,         //淡入淡出
    MMITHEME_HIDE_CENTER_SILDE, //向中央滑动
    MMITHEME_HIDE_MAX
} MMITHEME_HIDE_ANIM_E;

typedef enum
{
	MMITHEME_TRIGGER_MODE_KEY,	//key
	MMITHEME_TRIGGER_MODE_TP,	//tp
	MMITHEME_TRIGGER_MODE_MAX
} MMITHEME_TRIGGER_MODE_E;

typedef struct
{
    MMITHEME_HIDE_ANIM_E type;
    uint16               frame;
} MMITHEME_HIDE_ANIM_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/*! \brief get the msgbox style, is called by the msgbox control
 *  \param[out]		msgbox_style_ptr		pointer to msgbox style
 *  \author  Great.Tian
 *  \return 
 *		- <i><c>TRUE</c></i>: 	get success
 * 		- <i><c>FALSE</c></i>:	get failed
 */
/******************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetMsgBoxStyle(MMI_MSGBOX_STYLE_T* msgbox_style_ptr);

/******************************************************************************/
/*! \brief get the transparence color
 *  \author  Great.Tian
 *  \return 
 *		transparence color 
 */
/******************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetTransparenceColor(void);

/*****************************************************************************/
// 	Description : get the return text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMITHEME_GetReturnTextID(
                                        void
                                        );

/*****************************************************************************/
// 	Description : get the return text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMITHEME_GetCancelTextID(
                                        void
                                        );

/*****************************************************************************/
// 	Description : get the Cancel text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyLeftCtrlId(
                                        void
                                        );

/*****************************************************************************/
// 	Description : get the Cancel text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyRightCtrlId(
                                        void
                                        );

/*****************************************************************************/
// 	Description : get the Cancel text id 
//	Global resource dependence : none
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetSoftkeyMidCtrlId(
                                        void
                                        );

/******************************************************************************/
/*! \brief get the invalid text id, is call by mmi kernel
 *  \author  Great.Tian
 *  \return 
 *		invalid text id 
 */
/******************************************************************************/
PUBLIC MMI_TEXT_ID_T MMITHEME_GetInvalidTextID(void);

/******************************************************************************/
/*! \brief check the whether image id is valid
 *  \param[in]		image_id	image id
 *  \author  Great.Tian
 *  \return 
 *		- <i><c>TRUE</c></i>: 	image id is valid
 * 		- <i><c>FALSE</c></i>:	image id is invalid
 */
/******************************************************************************/
PUBLIC BOOLEAN MMITHEME_CheckImageID(MMI_IMAGE_ID_T image_id);

/******************************************************************************/
/*! \brief update the rect, include the main screen and the sub screen
 *  \author  Great.Tian
 *  \return 
 *		- <i><c>TRUE</c></i>: 	update succeed
 * 		- <i><c>FALSE</c></i>:	update fail
 */
/******************************************************************************/
PUBLIC BOOLEAN MMITHEME_UpdateRectExt(void);

/******************************************************************************/
/*! \brief get the calendar style, is called by the calendar control
 *  \param[out]		calendar_style_ptr			calendar style pointer
 *  \author  bin.ji
 *  \return 
 *		- <i><c>TRUE</c></i>: 	get succeed
 * 		- <i><c>FALSE</c></i>:	get fail
 */
/******************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetCalendarStyle(MMI_CALENDAR_STYLE_T* calendar_style_ptr);

/*****************************************************************************/
/*! \brief display the background picture for the window
 *  \param[in]		image_id				the id of image
 *  \author  Tracy Zhang
 */
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayWinBackground(
										  MMI_IMAGE_ID_T	image_id,	//IN:
										  MMI_WIN_ID_T win_id
										  );

/*****************************************************************************/
/*! \brief get the invalid image id 
 *  \author  Tracy Zhang
 *  \return 
 *		invalid image id
 */
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMITHEME_GetInvalidImageID(void);

/*****************************************************************************/
/*! \brief get default lcd device
 *  \author  bruce.chi
 *  \return 
 *		the pointer of default lcd dev struct
 */
/*****************************************************************************/
PUBLIC const GUI_LCD_DEV_INFO *MMITHEME_GetDefaultLcdDev(void);

/*****************************************************************************/
/*! \brief get tv status
 *  \author  gang.tong
 *  \return 
 *		whether the tv is opened
 */
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsTVOpen(void);

/*****************************************************************************/
/*! \brief get tv out rect
 *  \param[in]		show_rect_ptr		the rect of tv show
 *  \author  gang.tong
 *	\return 
 *		whether success
 */
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetTVOutShowRect(GUI_RECT_T * show_rect_ptr);

/*****************************************************************************/
// 	Description : get menu status
//	Global resource dependence : none
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMenuItemGrayed(
                                         MMI_WIN_ID_T           win_id,
										 MMI_MENU_GROUP_ID_T    group_id,
										 MMI_MENU_ID_T          menu_id
                                         );

/*****************************************************************************/
// 	Description : free all menu grayed by window id
//	Global resource dependence : none
//  Author: jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_FreeMenuGrayed(
                                    MMI_HANDLE_T    win_handle
                                    );

/*****************************************************************************/
//  Description : 计算字符串中UCS2,default ext,R8单，双字节编码字符的个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CountSpecialChar(
                                      BOOLEAN                   is_sms,         //in:
                                      wchar                     *txt_ptr,       //in:
                                      uint16                    len,            //in:
									  GUIEDIT_SMS_ENCODE_E		sms_encode,		//in:
                                      GUIEDIT_TEXT_SPCHAR_NUM_T *spchar_num_ptr //in/out:
                                      );

/*****************************************************************************/
//  Description : 计算字符串中Turkish,Spanish,Portuguese特有字符,共有字符个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CountLangChar(
                                   wchar                        *txt_ptr,           //in:
                                   uint16                       len,                //in:
                                   GUIEDIT_TEXT_LANGCHAR_NUM_T  *langchar_num_ptr   //in/out:
                                   );

/*****************************************************************************/
//  Description : 计算字符串中ucs2字符的个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_CountUcs2Char(
                                     BOOLEAN				is_sms,
                                     uint16					*txt_ptr,
                                     uint16					len,
									 GUIEDIT_SMS_ENCODE_E	sms_encode	//in:
                                     );

/*****************************************************************************/
//  Description : 计算字符串中R8双字节编码字符的个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_CountR82ByteChar(
                                        wchar   *txt_ptr, //in:
                                        uint16  len       //in:
                                        );

/*****************************************************************************/
//  Description : 计算字符串中R8双字节编码字符的个数
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_CountR81ByteChar(
                                        wchar   *txt_ptr, //in:
                                        uint16  len       //in:
                                        );

/*****************************************************************************/
//  Description : 获得字符串中最大和最小ucs2的值
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetMaxAndMinUcs2(
                                         wchar      *txt_ptr,       //in:
                                         uint16     len,            //in:
                                         wchar      *max_ucs2_ptr,  //in/out:
                                         wchar      *min_ucs2_ptr   //in/out:
                                         );

/*****************************************************************************/
//  Description : 判断是否为 UCS2 字符
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsUcs2Char(
                                   BOOLEAN				is_sms,		//in:
                                   uint16				ch,			//in:
								   GUIEDIT_SMS_ENCODE_E	sms_encode	//in:
                                   );

/*****************************************************************************/
//  Description : 判断是否为 default ext 字符
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsDefaultChar(
                                      uint16				ch,			//in:
									  GUIEDIT_SMS_ENCODE_E	sms_encode	//in:
                                      );

/*****************************************************************************/
//  Description : 判断是否为 R8 2byte 字符
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR82ByteChar(
                                      wchar     ch
                                      );

/*****************************************************************************/
//  Description : 判断是否为 R8 1byte 字符
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR81ByteChar(
                                      wchar     ch
                                      );

/*****************************************************************************/
// 	Description : Count default extension character
//	Global resource dependence : none
//  Author: Tracy Zhang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_CountDefaultExt(
									   wchar	            *ascii_ptr, //in:
									   uint16	            str_len,    //in:
                                       GUIEDIT_SMS_ENCODE_E	sms_encode	//in:
									   );

/*****************************************************************************/
//  Description : 判断是否为 土耳其 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR8Turkish(
                                    wchar   ch
                                    );

/*****************************************************************************/
//  Description : 判断是否为 西班牙 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR8Spanish(
                                    wchar   ch
                                    );

/*****************************************************************************/
//  Description : 判断是否为 葡萄牙 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR8Portuguese(
                                       wchar    ch
                                       );

/*****************************************************************************/
//  Description : 判断是否为土耳其和西班牙共用 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsSpanishAndTurkish(
                                            wchar   ch
                                            );

/*****************************************************************************/
//  Description : 判断是否为葡萄牙和西班牙共用 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsSpanishAndPortuguese(
                                               wchar    ch
                                               );

/*****************************************************************************/
//  Description : 判断是否为R8 共用 字符
//  Global resource dependence : none
//  Author: fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsR8All(
                                wchar   ch
                                );

/*****************************************************************************/
//  Description : get pb encode type
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMITHEME_PB_ENCODE_TYPE_E MMITHEME_GetEncodeTypeForPb(
                                                             wchar  max_ucs2,   //in:
                                                             wchar  min_ucs2    //in:
                                                             );

/*****************************************************************************/
//  Description : 根据PB压缩算法，获得可输入的最大长度
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetMaxLenForPb(
                                      uint16                    ascii_max_len,  //in:
                                      uint16                    ucs2_max_len,   //in:
                                      MMITHEME_PB_ENCODE_TYPE_E encode_type     //in:
                                      );

/*****************************************************************************/
// 	Description : to get the text by label and language
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetLabelTextByLang(
                                        MMI_TEXT_ID_T    label,     //[in] the label 
                                        MMI_STRING_T     *str_ptr   //[out] the data of text
                                        );
/*****************************************************************************/
//  Description : get text ptr by text id
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC const wchar* MMITHEME_GetResText(
                                        MMI_TEXT_ID_T   text_id,
                                        CAF_HANDLE_T    handle_id,
                                        MMI_STRING_T    *str_ptr
                                        );

/*****************************************************************************/
//  Description : switch static theme
//  Global resource dependence : none
//  Author:  Jassmine
//  Note: 横竖屏切换时调用
/*****************************************************************************/
PUBLIC void MMITHEME_SwitchStaticThemeRes(void);

/*****************************************************************************/
//  Description : 判断lcd是否点亮        
//  Global resource dependence : 
//  Author: xiaoqing.lu 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsBacklightOn(void);

/*****************************************************************************/
// 	Description : Set Display direction
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_SetDisDirection(MMI_DIRECTION_SET_E direction);

/*****************************************************************************/
// 	Description : Get Display direction
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC MMI_DIRECTION_SET_E MMITHEME_GetDisDirection(void);

/*****************************************************************************/
// 	Description : Set is scroll as pixel
//	Global resource dependence : 
//  Author:yongwei.he
//	Note:
//  TRUE: scroll as pixel 
//  FALSE: scroll as character 
/*****************************************************************************/
PUBLIC void MMITHEME_SetIsScrollAsPixel(BOOLEAN is_scroll_pixel);

/*****************************************************************************/
// 	Description : Get is scroll as pixel
//	Global resource dependence : 
//  Author:yongwei.he
//	Note:
//  TRUE: scroll as pixel 
//  FALSE: scroll as character 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_GetIsScrollAsPixel(void);

/*****************************************************************************/
// 	Description : get default lcd id and block id
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDefaultLcdBlock(
										uint32		*lcd_id, 
										uint32		*block_id
										);

/*****************************************************************************/
// 	Description : check lcd id and block id is valid
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_CheckLcdBlockId(
										uint32		lcd_id, 
										uint32		block_id
										);


/*****************************************************************************/
// 	Description : 获取应图标所在区域
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T MMITHEME_GetHardIconRect(void);

/*****************************************************************************/
// 	Description : display bg in rect,title,softkey etc.
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 图片显示区域必须和lcd显示区域相同
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayBg(
                               GUI_RECT_T               *rect_ptr,      //in
                               MMI_WIN_ID_T             win_id,         //in
                               const GUI_LCD_DEV_INFO   *dev_info_ptr   //in:
                               );

/*****************************************************************************/
// 	Description : display common bg in rect
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 图片显示区域必须和lcd显示区域相同
/*****************************************************************************/
PUBLIC void MMITHEME_DisplayCommonBg(
                                     GUI_RECT_T               *rect_ptr,      //in
                                     MMI_WIN_ID_T             win_id,         //in
                                     const GUI_LCD_DEV_INFO   *dev_info_ptr   //in:
                                     );

/*****************************************************************************/
// 	Description : 返回背景层句柄
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC UILAYER_HANDLE_T MMITHEME_GetBgLayer(void);

/*****************************************************************************/
// 	Description : （如果需要的话）创建并重画背景层
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMITHEME_RedrawBgLayer(void);

/*****************************************************************************/
// 	Description : 设置背景层数据发生变化
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMITHEME_SetBgLayerChanged(void);

/*****************************************************************************/
// 	Description : clear rect
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_ClearRect(
                               const GUI_LCD_DEV_INFO   *dev_info_ptr,
                               GUI_RECT_T               *rect_ptr
                               );

/*****************************************************************************/
// 	Description : get bg image id
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMITHEME_GetBgImgId(void);


/*****************************************************************************/
// 	Description : get font color of cur theme
//	Global resource dependence : 
//  Author: apple.zhang
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetCurThemeFontColor(
                                                 MMI_THEME_FONT_COLOR_INDEX_E font_color_index
                                                 );

/*****************************************************************************/
// 	Description : 获取按像素滚动的像素数
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMITHEME_ScrollPixelNum(void);

/*****************************************************************************/
// 	Description : trigger the mmi task
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMITHEME_TriggerMMITask(void);

/*****************************************************************************/
//  Description : convert (system)signal_keycode to (mmk)keycode
//  Global resource dependence : 
//  Author: jibin
//  Note:
/*****************************************************************************/    
uint8 MMIDEFAULT_ConvertKeyCode( 
                                uint32  signal_keycode 
                                );
#ifdef MMI_WINIDNAME_SUPPORT
/*****************************************************************************/
//  Description : register the menu group for every module  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetWinIdNameArr(
                                     MMI_WIN_ID_T win_id, 
                                     uint8 **name_arr_ptr
                                     );
#endif

/******************************************************************************/
/*! \brief get the invalid ctrl id
 *  \author jibin 
 *  \return invalid ctrl id
 */
/******************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_GetInvalidCtrlId(void);

/******************************************************************************/
/*! \brief get the invalid window id
 *  \author jibin 
 *  \return invalid window id
 */
/******************************************************************************/
PUBLIC MMI_WIN_ID_T MMITHEME_GetInvalidWinId(void);

/*****************************************************************************/
//  Description : g-sensor is support
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsSupportGsensor(void);

/*****************************************************************************/
//  Description : get owner draw control default height
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMITHEME_GetOwnDrawDefaultHeight(void);

/*****************************************************************************/
//  Description : focus win is tp coordinate win
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsFocusTpCoordinateWin(void);

/*****************************************************************************/
// 	Description : 返回common背景
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BG_T MMITHEME_GetCommonBg(void);

//@for light function modify
#if (defined MAINLCD_SIZE_240X320) || (defined DP_SCREEN_SIZE_240X240)
/*****************************************************************************/
// 	Description : 返回common light背景
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BG_T MMITHEME_GetCommonLightBg(void);
/*****************************************************************************/
// 	Description : 返回Second Menu背景
//	Global resource dependence : 
//  Author: chunjuan.liang
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BG_T MMITHEME_GetSecMenuBg(void);
#endif

/*****************************************************************************/
// 	Description : 返回Theme背景
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note: 
/*****************************************************************************/
PUBLIC GUI_BG_T MMITHEME_GetThemeBg(void);



/*****************************************************************************/
//  Description : hide anim type
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_SetHideAnimType(
                                        MMITHEME_HIDE_ANIM_E type
                                        );

/*****************************************************************************/
//  Description : get hide anim
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetHideAnim( 
                                 MMITHEME_HIDE_ANIM_T* anim_ptr
                                 );

#ifndef WIN32
/*****************************************************************************/
//  Description : get freg handler
//  Global resource dependence :                          
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMITHEME_GetFreqHandler(void);
#endif

/*****************************************************************************/
//  Description : 获得星期的字符串ID
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetWeekdayId( 
                                  MMI_TEXT_ID_T *weekday_id_ptr,
                                  uint16 day_num
                                  );

/*****************************************************************************/
// 	Description : get gray color
//	Global resource dependence : none
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC GUI_COLOR_T MMITHEME_GetGrayColor(
                                         void
                                         );

/*****************************************************************************/
//  Description : tts interface to theme file
//  Global resource dependence : 
//  Author: violent.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMITheme_StopControlText(void);

/*****************************************************************************/
//  Description : tts interface to theme file
//  Global resource dependence : 
//  Author: violent.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMITheme_PlayControlText(
                                     MMI_STRING_T* str_ptr, 
                                     uint32 num
                                     );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: violent.wei
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITheme_IsControlTTSOpen(void);

/*****************************************************************************/
//  Description : 根据速度获得加速度
//  Global resource dependence :                          
//  Author: xiaoqing.lu
//  Note: 用户可以修改这个函数，目前的实现是根据速度获取，如果用户觉得不需要这个速度，可以忽略
/*****************************************************************************/
PUBLIC int32 MMITHEME_GetFlingFriction(
                                       float velocity // 速度
                                       );
PUBLIC float MMITHEME_GetFlingFrictionExt(void);
/*****************************************************************************/
//  Description : get current trigger mode
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMITHEME_TRIGGER_MODE_E MMITHEME_GetTiggerMode(void);

/*****************************************************************************/
//  Description : get current trigger mode
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMITHEME_TRIGGER_MODE_E MMITHEME_GetPreTiggerMode(void);

/*****************************************************************************/
//  Description : get current trigger mode
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_SetTiggerMode(
                                   MMITHEME_TRIGGER_MODE_E  mode
                                   );

/*****************************************************************************/
//  Description : get current drag mode
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC MMITHEME_DRAGMODE_E MMITHEME_GetDragMode(void);

/*****************************************************************************/
//  Description : set current drag mode
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_SetDragMode(
                                 MMITHEME_DRAGMODE_E drag_mode
                                 );

/*****************************************************************************/
// 	Description : get default window style
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC WINDOW_DISPLAY_STYLE MMITHEME_GetDefaultWinStyle(void);


/*****************************************************************************/
//  Description :Is IStyle
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsIstyle(void);

/*****************************************************************************/
//  Description : is mainlayer type changable
//  Global resource dependence :                     
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsMainLayerTypeChangable(void);

/*****************************************************************************/
// 	Description : get mainlayer default type
//	Global resource dependence : 
//  Author: bin.ji
//	Note:
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E MMITHEME_GetMainLayerDefaultType(void);

/*****************************************************************************/
//  Description : restart screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMITHEME_RestartIdleSsTimer(void);
//屏保定时器改为系统级
#define MMITHEME_RestartIdleSsTimer()
/*****************************************************************************/
//  Description : stop screen saver timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
//PUBLIC void MMITHEME_StopIdleSsTimer(void);
//屏保定时器改为系统级
#define MMITHEME_StopIdleSsTimer()
/*****************************************************************************/
//  Description : 震动
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_Vibrate(uint32 duaration);

/*****************************************************************************/
//  Description : 震动
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_StopVibrate(void);

#ifdef GUI_DUMP_CTRL_TREE

/*****************************************************************************/
//  Description : get control guid name
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC char* MMITHEME_DumpCurrentWindow(void);
#endif

/*****************************************************************************/
// 	Description : get ctrl name
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC const char* MMITHEME_GetCtrlName(
                                        CAF_GUID_T  guid
                                        );

/*****************************************************************************/
//  Description : dump info
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_PrintInfo(void *exp, void *file, uint32 line);

/*****************************************************************************/
// 	Description : check ctrl creating param
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void MMITHEME_CheckCtrlParam(
    MMI_CONTROL_CREATE_T    *create_ptr
    );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif /* END _MMK_APP_H_  */
/*! @} */
