
#ifndef _GUIEDIT_H_
#define _GUIEDIT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrltextedit_export.h"
#include "ctrldigitaledit_export.h"
#include "ctrlphonenumedit_export.h"
#include "ctrlpswedit_export.h"
#include "ctrllistedit_export.h"
#include "ctrldateedit_export.h"
#include "ctrltimeedit_export.h"
#include "ctrlipedit_export.h"

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

/*! @enum GUIEDIT_TIPS_POS_E
@brief TOUCH风格时间日期控件，文字提示的位置
*/
// 对应于CTRLBASEPICKER_TIPS_POS_E
typedef enum
{
    GUIEDIT_TIPS_POS_TOP,           /*!<顶部位置*/
    GUIEDIT_TIPS_POS_MIDDLE,        /*!<中间位置*/
    GUIEDIT_TIPS_POS_NONE,          /*!<没有提示*/
    GUIEDIT_TIPS_POS_MAX            /*!<保留位*/
} GUIEDIT_TIPS_POS_E;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#define GUIEDIT_SetRect                     CTRLBASE_SetRect
#define GUIEDIT_SetBorder                   CTRLBASE_SetBorder
#define GUIEDIT_SetBg                       CTRLBASE_SetBg

// base edit
#define GUIEDIT_GetType                     CTRLBASEEDIT_GetType
#define GUIEDIT_SetMargin                   CTRLBASEEDIT_SetMargin
#define GUIEDIT_SetAlign                    CTRLBASEEDIT_SetAlign
#define GUIEDIT_SetBgTransparent            CTRLBASEEDIT_SetBgTransparent
#define GUIEDIT_PermitBorder                CTRLBASEEDIT_PermitBorder

#define GUIEDIT_SetIm                       CTRLBASEEDIT_SetIm
#define GUIEDIT_SetImTp                     CTRLBASEEDIT_SetImTp
#define GUIEDIT_SetImHw                     CTRLBASEEDIT_SetImHw
#define GUIEDIT_SetImSymbol                 CTRLBASEEDIT_SetImSymbol
#define GUIEDIT_SetImTag                    CTRLBASEEDIT_SetImTag
#define GUIEDIT_ConfigImCustomKey           CTRLBASEEDIT_ConfigImCustomKey
#define GUIEDIT_ConfigDisableChar           CTRLBASEFLEX_ConfigDisableChar
#define GUIEDIT_ConfigAddEnableChar         CTRLBASEFLEX_ConfigAddEnableChar
#define GUIEDIT_CreateImEditActive          CTRLBASEEDIT_CreateImEditActive
#define GUIEDIT_IsTPPanelVisible            CTRLBASEEDIT_IsTPPanelVisible
#define GUIEDIT_IsImOpen                    CTRLBASEEDIT_IsImOpen
#define GUIEDIT_IsImNeedSoftkey				CTRLBASEEDIT_IsImNeedSoftkey
//#define GUIEDIT_SetImCapital                CTRLBASEEDIT_SetImCapital
//#define GUIEDIT_SetImInput                  CTRLBASEEDIT_SetImInput
#define GUIEDIT_GetIMHandle                 CTRLBASEEDIT_GetIMHandle
#define GUIEDIT_DelHighlightChars           CTRLBASEFLEX_DelHighlightChars
#define GUIEDIT_DisplayTitleInfo            CTRLBASEEDIT_DisplayTitleInfo

// base flex
#define GUIEDIT_GetString                   CTRLBASEFLEX_GetString
#define GUIEDIT_GetStringEx					CTRLBASEFLEX_GetStringEx
#define GUIEDIT_SetString                   CTRLBASEFLEX_SetString
#define GUIEDIT_InsertString                CTRLBASEFLEX_InsertString
#define GUIEDIT_ClearAllStr                 CTRLBASEFLEX_ClearAllStr
#define GUIEDIT_ClearAllStrEx               CTRLBASEFLEX_ClearAllStrEx
#define GUIEDIT_SetMaxLen                   CTRLBASEFLEX_SetMaxLen
#define GUIEDIT_GetCursorCurPos             CTRLBASEFLEX_GetCursorCurPos
#define GUIEDIT_SetCursorCurPos             CTRLBASEFLEX_SetCursorCurPos
#define GUIEDIT_SetCursorHideState          CTRLBASEFLEX_SetCursorHideState
#define GUIEDIT_SetStyle                    CTRLBASEFLEX_SetStyle
#define GUIEDIT_GetPosInfo                  CTRLBASEFLEX_GetPosInfo
#define GUIEDIT_GetHighlightInfo            CTRLBASEFLEX_GetHighlightInfo
#define GUIEDIT_GetHighlightData            CTRLBASEFLEX_GetHighlightData
#define GUIEDIT_SetDefaultString            CTRLBASEFLEX_SetDefaultString
#define GUIEDIT_SetDefaultStringById        CTRLBASEFLEX_SetDefaultStringById
#define GUIEDIT_SetClipboardEnabled         CTRLBASEFLEX_SetClipboardEnabled
#define GUIEDIT_SetDividingLine             CTRLBASEFLEX_SetDividingLine
#define GUIEDIT_SetSoftkey                  CTRLBASEFLEX_SetSoftkey
#define GUIEDIT_SetSoftkeyEx                CTRLBASEFLEX_SetSoftkeyEx
#define GUIEDIT_SetLongCancelDelAll         CTRLBASEFLEX_SetLongCancelDelAll
#define GUIEDIT_SetHandleRedKey             CTRLBASEFLEX_SetHandleRedKey
#ifdef TOUCH_PANEL_SUPPORT
#define GUIEDIT_SetHandleTpMsg              CTRLBASEFLEX_SetHandleTpMsg
#endif

// text edit
#define GUIEDIT_SetTextMaxLen               CTRLTEXTEDIT_SetTextMaxLen
#define GUIEDIT_SetTextMaxLenEx             CTRLTEXTEDIT_SetTextMaxLenEx
#define GUIEDIT_GetLeftNumInTextEditBox     CTRLTEXTEDIT_GetLeftNum
#define GUIEDIT_SetTextPage                 CTRLTEXTEDIT_SetTextPage
#define GUIEDIT_SetSmsEncode                CTRLTEXTEDIT_SetSmsEncode
#define GUIEDIT_SetPbEncode                 CTRLTEXTEDIT_SetPbEncode
#define GUIEDIT_GetPbEncode                 CTRLTEXTEDIT_GetPbEncode

// phonenum edit
#define GUIEDIT_SetFontEx                   CTRLPNEDIT_SetFontEx
#define GUIEDIT_SetDialEditAdaptive         CTRLPNEDIT_SetDialEditAdaptive
#define GUIEDIT_SetWinInterceptCtrlMsg      CTRLPNEDIT_SetWinInterceptCtrlMsg
#define GUIEDIT_GetPhoneNumString           CTRLPNEDIT_GetPhoneNumString
#define GUIEDIT_SetPhoneNumStyle            CTRLPNEDIT_SetPhoneNumStyle
#define GUIEDIT_SetPhoneNumDir              CTRLPNEDIT_SetPhoneNumDir
#define GUIEDIT_SetDtmfNum                  CTRLPNEDIT_SetDtmfNum

// list edit
#define GUIEDIT_GetListItemNum              CTRLLISTEDIT_GetListItemNum
#define GUIEDIT_GetListItemString           CTRLLISTEDIT_GetListItemString
#define GUIEDIT_AddListItem                 CTRLLISTEDIT_AddListItem
#define GUIEDIT_AddListItemArray            CTRLLISTEDIT_AddListItemArray
#define GUIEDIT_GetListItemArray            CTRLLISTEDIT_GetListItemArray
#define GUIEDIT_ReplaceCurListItem          CTRLLISTEDIT_ReplaceCurListItem
#define GUIEDIT_SetMaxListItemNum           CTRLLISTEDIT_SetMaxListItemNum
#define GUIEDIT_SetListStyle                CTRLLISTEDIT_SetListStyle

// password edit
#define GUIEDIT_SetPasswordStyle            CTRLPSWEDIT_SetPasswordStyle

// ip edit
#define GUIEDIT_GetIP                       CTRLIPEDIT_GetIP
#define GUIEDIT_SetIP                       CTRLIPEDIT_SetIP

// base picker
#define GUIEDIT_SetTipsPos                  CTRLBASEPICKER_SetTipsPos
#define GUIEDIT_EnbaleAnim                  CTRLBASEPICKER_EnableAnim
#define GUIEDIT_SetPickerHeight             CTRLBASEPICKER_SetHeight
#define GUIEDIT_SetPickerLineHeight         CTRLBASEPICKER_SetLineHeight
#define GUIEDIT_SetPickerFont               CTRLBASEPICKER_SetFont

// picker
#define GUIEDIT_SetPickerContent            CTRLPICKER_SetContent
#define GUIEDIT_SetPickerLoop               CTRLPICKER_SetLoop
#define GUIEDIT_SetPickerSelectedItem       CTRLPICKER_SetSelectedItem
#define GUIEDIT_GetPickerSelectedItem       CTRLPICKER_GetSelectedItem


/*****************************************************************************/
//  Description : get date
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_GetDate(
                            MMI_CTRL_ID_T   ctrl_id,    //in
                            uint16          *year_ptr,  //in/out:may PNULL
                            uint8           *month_ptr, //in/out:may PNULL
                            uint8           *day_ptr    //in/out:may PNULL
                            );

/*****************************************************************************/
//  Description : set date
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetDate(
                            MMI_CTRL_ID_T   ctrl_id,
                            uint16          year,
                            uint8           month,
                            uint8           day
                            );

/*****************************************************************************/
//  Description : set date style and separator
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetDateStyle(
                                 MMI_CTRL_ID_T          ctrl_id,
                                 GUIEDIT_SEPARATOR_E    *separator_ptr, //may PNULL
                                 GUIEDIT_DATE_STYLE_E   *date_style_ptr,//may PNULL
                                 BOOLEAN                is_update
                                 );

/*****************************************************************************/
//  Description : get date range
//  Global resource dependence : 
//  Author: Jassmine
//  Note:  
/*****************************************************************************/
PUBLIC GUIEDIT_DATE_YEAR_RANGE_E GUIEDIT_GetDateRange(
                                                      MMI_CTRL_ID_T     ctrl_id
                                                      );

/*****************************************************************************/
//  Description : set date range
//  Global resource dependence : 
//  Author: Jassmine
//  Note: default is GUIEDIT_DATE_YEAR_RANGE_SET 
/*****************************************************************************/
PUBLIC void GUIEDIT_SetDateRange(
                                 MMI_CTRL_ID_T              ctrl_id,
                                 GUIEDIT_DATE_YEAR_RANGE_E  year_range_type
                                 );

/*****************************************************************************/
//  Description : get time
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_GetTime(
                            MMI_CTRL_ID_T   ctrl_id,    //in
                            uint8           *hour_ptr,  //in/out:may PNULL
                            uint8           *minute_ptr,//in/out:may PNULL
                            uint8           *second_ptr //in/out:may PNULL
                            );

/*****************************************************************************/
//  Description : set time
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetTime(
                            MMI_CTRL_ID_T   ctrl_id,
                            uint8           hour,
                            uint8           minute,
                            uint8           second
                            );

/*****************************************************************************/
//  Description : set time style and display style
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIEDIT_SetTimeStyle(
                                 MMI_CTRL_ID_T                  ctrl_id,
                                 GUIEDIT_SEPARATOR_E            *separator_ptr,     //may PNULL
                                 GUIEDIT_TIME_STYLE_E           *time_style_ptr,    //may PNULL
                                 GUIEDIT_TIME_DISPLAY_STYLE_E   *display_style_ptr, //may PNULL
                                 BOOLEAN                        is_update
                                 );

/***************************************************************************//*!
@brief 设置字体
@author Jassmine
@param ctrl_id [in] 控件id
@param font    [in] 字体
@note 
- 此函数会引起输入内容的重新解析，建议在输入之前设置。
- GUIEDIT_SetFontColor只设置字体颜色。
*******************************************************************************/
PUBLIC void GUIEDIT_SetFont(
                            MMI_CTRL_ID_T   ctrl_id,//in
                            GUI_FONT_T      font    //in
                            );

/***************************************************************************//*!
@brief 设置字体颜色
@author Jassmine
@param ctrl_id    [in] 控件id
@param font_color [in] 颜色
@note 
*******************************************************************************/
PUBLIC void GUIEDIT_SetFontColor(
                                 MMI_CTRL_ID_T  ctrl_id,    //in
                                 GUI_COLOR_T    font_color  //in
                                 );

/***************************************************************************//*!
@brief 设置编辑框是否显示输入法图标或剩余字符信息
@author Jassmine
@param ctrl_id              [in] 控件id
@param is_disp_im_icon_ptr  [in] 是否显示输入法图标
@param is_disp_num_info_ptr [in] 是否显示剩余字符
@note 固定格式编辑框如时间日期等默认显示,其他默认不显示
*******************************************************************************/
PUBLIC void GUIEDIT_SetDispImIcon(
                                  MMI_CTRL_ID_T     ctrl_id,                //in:
                                  BOOLEAN           *is_disp_im_icon_ptr,   //in:
                                  BOOLEAN           *is_disp_num_info_ptr   //in:
                                  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
