/*****************************************************************************
** File Name:      ctrlbaseedit_export.h                                        *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLBASEEDIT_EXPORT_H_
#define _CTRLBASEEDIT_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "guiim.h"
#include "ctrlbase_export.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*! @enum GUIEDIT_TYPE_E
@brief 编辑框类型
*/
typedef enum
{
    GUIEDIT_TYPE_NULL,      /*!<保留位*/
    GUIEDIT_TYPE_TEXT,      /*!<文本编辑框*/
    GUIEDIT_TYPE_PHONENUM,  /*!<电话号码编辑框,[0-9,*,#]*/
    GUIEDIT_TYPE_DIGITAL,   /*!<数字编辑框,[0-9]*/
    GUIEDIT_TYPE_PASSWORD,  /*!<密码编辑框,[0-9,*,#]*/
    GUIEDIT_TYPE_LIST,      /*!<列表编辑框list[电话号码列表,E-mail列表]*/
    GUIEDIT_TYPE_DATE,      /*!<日期编辑框*/
    GUIEDIT_TYPE_TIME,      /*!<时间编辑框*/
    GUIEDIT_TYPE_IP,        /*!<IP编辑框*/
    GUIEDIT_TYPE_TOUCH_DATE,/*!<全触摸屏日期*/
    GUIEDIT_TYPE_TOUCH_TIME,/*!<全触摸屏时间*/
    GUIEDIT_TYPE_PICKER,    /*!<滚轴选取*/
    GUIEDIT_TYPE_MAX        /*!<保留位*/
} GUIEDIT_TYPE_E;

/*! @enum GUIEDIT_STYLE_E
@brief 编辑框风格
*/
typedef enum
{
    GUIEDIT_STYLE_NULL,     /*!<保留位*/
    GUIEDIT_STYLE_SINGLE,   /*!<单行,没有滚动条*/
    GUIEDIT_STYLE_MULTI,    /*!<多行,有滚动条*/
    GUIEDIT_STYLE_ADAPTIVE, /*!<多行,没有滚动条,内嵌在组合控件中*/
    GUIEDIT_STYLE_MAX       /*!<保留位*/
} GUIEDIT_STYLE_E;

/*! @enum GUIEDIT_DISPLAY_DIR_E
@brief 编辑框显示方向
*/
typedef enum
{
    GUIEDIT_DISPLAY_DIR_LT, /*!<从左上开始显示*/
    GUIEDIT_DISPLAY_DIR_RB, /*!<从右下开始显示*/
    GUIEDIT_DISPLAY_DIR_MAX /*!<保留位*/
} GUIEDIT_DISPLAY_DIR_E;

/*! @enum GUIEDIT_SEPARATOR_E
@brief 编辑框分隔符类型
*/
typedef enum
{
    GUIEDIT_SEPARATOR_NULL,         /*!<保留位*/
    GUIEDIT_SEPARATOR_DOT,          /*!<'.'*/
    GUIEDIT_SEPARATOR_LINE,         /*!<'-'*/
    GUIEDIT_SEPARATOR_COMMA,        /*!<','*/
    GUIEDIT_SEPARATOR_BLANK,        /*!<' '*/
    GUIEDIT_SEPARATOR_COLON,        /*!<':'*/
    GUIEDIT_SEPARATOR_SOLIDUS,      /*!<'/'*/
    GUIEDIT_SEPARATOR_SEMICOLON,    /*!<';'*/
    GUIEDIT_SEPARATOR_MAX           /*!<保留位*/
} GUIEDIT_SEPARATOR_E;

/*! @struct GUIEDIT_INIT_LIST_T
@brief 列表编辑框初始化数据
*/
typedef struct
{
    uint16      item_num;       /*!<item个数*/
    uint16      item_max_len;   /*!<每个item的最大字符长度*/
} GUIEDIT_INIT_LIST_T;

/*! @union GUIEDIT_INIT_ADD_U
@brief 编辑框初始化附加数据
*/
typedef union
{
    GUIEDIT_INIT_LIST_T     list;   /*!<列表初始化数据*/
} GUIEDIT_INIT_ADD_U;

/*! @struct GUIEDIT_INIT_DATA_T
@brief 编辑框初始化数据
*/
typedef struct
{
    BOOLEAN                 is_rect_adaptive;   /*!<区域是否根据字符串自动调整*/
    uint16                  str_max_len;        /*!<允许的最大字符长度*/
    GUI_BOTH_RECT_T         both_rect;          /*!<横竖区域*/
    GUIEDIT_TYPE_E          type;               /*!<编辑框类型*/
    GUIEDIT_INIT_ADD_U      add_data;           /*!<初始化附加数据*/
} GUIEDIT_INIT_DATA_T;

/*! @struct GUIEDIT_HIGHLIGHT_T
@brief 编辑框高亮信息
*/
typedef struct
{
    BOOLEAN     is_underline;   /*!<TRUE:im提交不确定字符,FALSE:复制/粘贴等确定字符*/
    uint16      start_pos;      /*!<高亮起始位置*/
    uint16      end_pos;        /*!<高亮结束位置*/
} GUIEDIT_HIGHLIGHT_T;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get edit type
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC GUIEDIT_TYPE_E CTRLBASEEDIT_GetType (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : set edit is display im icon and number info
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:text and list edit display,others not display
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetDispImIcon (
    MMI_CTRL_ID_T     ctrl_id,                //in:
    BOOLEAN           is_disp_im_icon        //in:
);

/*****************************************************************************/
// 	Description : set edit is permit display border
//	Global resource dependence :
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_PermitBorder (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_permit
);

/*****************************************************************************/
//  Description : set edit bg is transparent display image
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_SetBgTransparent (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_transparent
);

/*****************************************************************************/
//  Description : set edit margin
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetMargin (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    uint16            margin_lr,  //in:left and right margin
    uint16            margin_tb   //in:top and bottom margin
);

/*****************************************************************************/
//  Description : set edit font
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetFont (
    MMI_CTRL_ID_T       ctrl_id,
    GUI_FONT_T          *font_size_ptr,
    GUI_COLOR_T         *font_color_ptr
);

/*****************************************************************************/
//  Description : set edit align
//  Global resource dependence :
//  Author: Jassmine
//  Note: only for single line
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetAlign (
    MMI_CTRL_ID_T  ctrl_id,//in
    GUI_ALIGN_E    align   //in
);

/*****************************************************************************/
//  Description : set edit rect is adaptive
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetRectAdaptive(
                                         MMI_CTRL_ID_T  ctrl_id,//in
                                         BOOLEAN        is_true //in:
                                         );

/*****************************************************************************/
//  Description : set edit im
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:允许所有的输入法,初始输入法是智能输入
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetIm (
    MMI_CTRL_ID_T     ctrl_id,    //in
    GUIIM_TYPE_T      allow_im,   //in:允许的输入集合
    GUIIM_TYPE_T      init_im     //in:初始输入法
);

///*****************************************************************************/
////  Description : set edit im capital mode
////  Global resource dependence :
////  Author: Jassmine
////  Note:
///*****************************************************************************/
//PUBLIC void CTRLBASEEDIT_SetImCapital (
//    MMI_CTRL_ID_T			ctrl_id,	//in
//    GUIIM_CAPITAL_MODE_T	capital_mode//in:大小写模式
//);

///*****************************************************************************/
////  Description : set edit im input type
////  Global resource dependence :
////  Author: Jassmine
////  Note:
///*****************************************************************************/
//PUBLIC void CTRLBASEEDIT_SetImInput (
//    MMI_CTRL_ID_T		ctrl_id,		//in
//    GUIIM_INPUT_TYPE_T	init_input_type,//in:初始输入法类型
//    GUIIM_INPUT_TYPE_T	allow_input_type//in:允许的输入类型集合
//);

/*****************************************************************************/
//  Description : set edit im is enable tp
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:是否允许TP IM
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImTp (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_enable
);

/*****************************************************************************/
//  Description : set edit im is enable hand writing
//  Global resource dependence :
//  Author: Jassmine
//  Note: Default:是否允许IM hand writing
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImHw (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_enable
);

/*****************************************************************************/
//  Description : set edit im is forbid symbo;
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImSymbol (
    MMI_CTRL_ID_T   ctrl_id,
    BOOLEAN         is_forbid
);

/*****************************************************************************/
//  Description : set edit im tag
//  Global resource dependence :
//  Author: Jassmine
//  Note: 比如是email输入的时候，一些输入法会定制一些键盘
//      建议在激活edit控件之前设置，因为在输入法创建之后设置，会重建输入法
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_SetImTag (
    MMI_CTRL_ID_T  ctrl_id,
    GUIIM_TAG_T    tag
);

/*****************************************************************************/
//  Description : config edit im custom key
//  Global resource dependence :
//  Author: Jassmine
//  Note: must < 9 ,only config digital or multi-tap im mode
/*****************************************************************************/
PUBLIC void CTRLBASEEDIT_ConfigImCustomKey (
    MMI_CTRL_ID_T         ctrl_id,    //in:
    GUIIM_TYPE_T          im_type,    //in:only digital or multi-tap
    GUIIM_CUSTOM_KEY_T    custom_key, //in:
    wchar                 *value_ptr, //in:
    uint32                value_count //in:
);

/*****************************************************************************/
//  Description: check whether im is open
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsImOpen (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description: judge whether application can set softkey or not
//  Author: arvin.zhang
//  Note:app cann't modify softkey if im is working on several state
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsImNeedSoftkey (
	MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : when edit get active, is create im
//  Global resource dependence :
//  Author: Jassmine
//  Note: default TRUE
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_CreateImEditActive (
    MMI_CTRL_ID_T     ctrl_id,    //in:
    BOOLEAN           is_create   //in:
);

/*****************************************************************************/
//  Description: judge whether the TP panel is visible
//  Author: wancan.you
//  Param
//       ctrl_id :control id
//  Return:
//       TRUE:visible;FALSE:invisible
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEEDIT_IsTPPanelVisible (
    MMI_CTRL_ID_T ctrl_id
);

/*****************************************************************************/
//  Description : get im handle
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLBASEEDIT_GetIMHandle(
                                        MMI_CTRL_ID_T     ctrl_id    //in:
                                        );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
