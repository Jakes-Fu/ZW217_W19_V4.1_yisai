/*****************************************************************************
** File Name:      mmitheme_edit.h                                           *
** Author:                                                                   *
** Date:           04/26/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2006      Jassmine              Creat
******************************************************************************/

#ifndef _MMITHEME_EDIT_H_
#define _MMITHEME_EDIT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guifont.h"
#include "guiim.h"
#include "guisoftkey.h"
#include "mmi_font.h"
#include "mmitheme_clipbrd.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
//优先通过函数设置softkey[in:字符串指针in:字符串长度,out:softkey值]
typedef BOOLEAN (*MMITHEME_EDIT_SET_BUTTON_PFUNC)(wchar *str_ptr,uint16 str_len,MMI_TEXT_ID_T *text_id_ptr);
#ifdef CMCC_UI_STYLE
typedef void (*MMITHEME_EDIT_EXCEED_MAXLEN_PFUNC)(void);
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

//edit button
typedef struct
{
    BOOLEAN                         is_modify;  //是否用户更该过
    uint16                          str_len;    //0代表一直不变,len之前显示text 1,len及之后显示text 2
    MMI_TEXT_ID_T                   text_1;     //NULL代表不显示
    MMI_TEXT_ID_T                   text_2;     //NULL代表不显示
    MMI_IMAGE_ID_T                  image_id;   //
    uint16                          button_type; //0代表EDIT_BUTTON_TEXT_ID，显示字符串；1代表EDIT_BUTTON_IMAGE_ID，显示图片
    MMITHEME_EDIT_SET_BUTTON_PFUNC  setsoftkey; //优先通过函数设置softkey[in:字符串指针in:字符串长度,out:softkey值]
} MMITHEME_EDIT_BUTTON_T;

//edit softkey
typedef struct
{
    MMITHEME_EDIT_BUTTON_T      button[GUISOFTKEY_BUTTON_NUM];
} MMITHEME_EDIT_SOFTKEY_T;

//the edit common theme
typedef struct
{
    BOOLEAN                     is_cancel_delete;   //cancel is delete char
    BOOLEAN                     is_cir_handle_ud;   //is circular handle up/down
    BOOLEAN                     is_cir_handle_lf;   //is circular handle left/right
    BOOLEAN                     is_disp_highlight;  // whether display highlight when im underline

    uint16                      char_space;         //space between chars,must >=1,include cursor
    uint16                      line_space;         //space between rows,include dividing line
    uint16                      margin_lr_space;    //left and right margin,must >=1,may display cursor
    uint16                      margin_tb_space;    //top and bottom margin

    GUI_BORDER_T                border;             //border
    GUI_BG_T                    bg;                 //background

    uint16                      dividing_line_width;//dividing line width,must <= line space
    GUI_COLOR_T                 dividing_line_color;//dividing line color

    GUI_FONT_T                  info_font;          //font
    GUI_FONT_ALL_T              font;               //font
    GUI_COLOR_T                 highlight_str_color;//highlight string color    
    GUI_COLOR_T                 highlight_bg_color; //highlight bg color

    MMITHEME_EDIT_SOFTKEY_T     softkey;            //edit softkey

    GUI_LCD_DEV_INFO            lcd_dev;            //lcd dev
} MMITHEME_EDIT_T;

//theme for base flex
typedef struct
{
    BOOLEAN                     is_handle_tp;       //is handle tp msg
    BOOLEAN                     is_support_slide;   //is support slide
    uint16                      slide_period;       //slide period
    uint16                      reset_slide_start;  //reset slide the start frame
    uint16                      reset_slide_frame;  //reset slide frame number

    BOOLEAN                     is_handle_red_key;  //is handle red,default TRUE
    BOOLEAN                     is_delete_all;      //long cancel is delete all chars
    GUI_COLOR_T                 default_str_color;  //default string color
    uint32                      cursor_period;      //cursor period
    uint32                      tp_scroll_period;   //tp scroll period
    MMITHEME_DRAGMODE_E         drag_mode;          // 滑块操作模式
    uint32                      slider_display_period;  // 滑块显示时间
#ifdef CMCC_UI_STYLE
    MMITHEME_EDIT_EXCEED_MAXLEN_PFUNC  exceedMaxLenFun;
#endif
} MMITHEME_BASEFLEX_T;

//the edit im
typedef struct
{
    BOOLEAN                     is_forbid_symbol;   //is forbid symbol
    GUIIM_TYPE_T                init_im;            //init im
    GUIIM_TYPE_T                allow_im;           //allow im set
    GUIIM_INPUT_TYPE_T          init_input_type;    //init input type,key,tp,handwriting
    GUIIM_INPUT_TYPE_T          allow_input_type;   //allow input type,key,tp,handwriting
    GUIIM_CAPITAL_MODE_T        capital_mode;       //capital mode
    GUIIM_TAG_T                 tag;                //附加属性
} MMITHEME_EDIT_IM_T;

//the edit text theme
typedef struct
{
    GUI_ALIGN_E                 align;  //text align
    MMITHEME_EDIT_IM_T          im;     //im
} MMITHEME_EDIT_TEXT_T;

//the edit password theme
typedef struct
{
    uint32                      period; //visible time
    GUI_ALIGN_E                 align;  //text align
    MMITHEME_EDIT_IM_T          im;     //im
} MMITHEME_EDIT_PSW_T;

//the edit date theme
typedef struct
{
    uint32                      list_style;         //GUIEDIT_LIST_STYLE_E
    uint32                      separator_style;    //GUIEDIT_SEPARATOR_E
    GUI_ALIGN_E                 align;              //list align
    MMITHEME_EDIT_IM_T          phonenum_im;        //im
    MMITHEME_EDIT_IM_T          email_im;           //im
    MMITHEME_EDIT_IM_T          all_im;             //im
} MMITHEME_EDIT_LIST_T;

//the edit date theme
typedef struct
{
    uint32                      date_style;     //GUIEDIT_DATE_STYLE_E
    uint32                      separator_style;//GUIEDIT_SEPARATOR_E
    GUI_ALIGN_E                 align;          //date align
    MMITHEME_EDIT_IM_T          im;             //im
} MMITHEME_EDIT_DATE_T;

//the edit time theme
typedef struct
{
    uint32                      time_style;     //GUIEDIT_TIME_STYLE_E
    uint32                      separator_style;//GUIEDIT_SEPARATOR_E
    uint32                      display_style;  //GUIEDIT_TIME_DISPLAY_STYLE_E
    GUI_ALIGN_E                 align;          //time align
    MMITHEME_EDIT_IM_T          im;             //im
} MMITHEME_EDIT_TIME_T;

//the edit IP theme
typedef struct
{
    GUI_ALIGN_E                 align;  //IP align
    MMITHEME_EDIT_IM_T          im;     //im
} MMITHEME_EDIT_IP_T;

//the edit common theme
typedef struct
{
    GUI_BORDER_T                border;     //border
    GUI_BG_T                    bg;         //background
    GUI_FONT_ALL_T              font;       //font
} MMITHEME_EDIT_PSW_DISPLAY_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get edit common theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetEditTheme(
                           MMITHEME_EDIT_T  *theme_ptr  //in/out:
                           );

/*****************************************************************************/
//  Description : get baseflex edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetBaseFlexTheme(
                           MMITHEME_BASEFLEX_T   *flex_theme_ptr  //in/out:
                           );

/*****************************************************************************/
//  Description : get text edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTextEditTheme(
    MMITHEME_EDIT_TEXT_T    *text_theme_ptr
    );

/*****************************************************************************/
//  Description : get phonenum edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPhoneNumEditTheme(
    MMITHEME_EDIT_TEXT_T    *phonenum_theme_ptr
    );

/*****************************************************************************/
//  Description : get digital edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDigitalEditTheme(
    MMITHEME_EDIT_TEXT_T    *digital_theme_ptr
    );

/*****************************************************************************/
//  Description : get password edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPswEditTheme(
    MMITHEME_EDIT_PSW_T     *password_theme_ptr
    );

/*****************************************************************************/
//  Description : get list edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetListEditTheme(
    MMITHEME_EDIT_LIST_T    *list_theme_ptr
    );

/*****************************************************************************/
//  Description : get date edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDateEditTheme(
    MMITHEME_EDIT_DATE_T    *date_theme_ptr
    );

/*****************************************************************************/
//  Description : get time edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTimeEditTheme(
    MMITHEME_EDIT_TIME_T    *time_theme_ptr
    );

/*****************************************************************************/
//  Description : get IP edit theme
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetIPEditTheme(
    MMITHEME_EDIT_IP_T      *ip_theme_ptr
    );

/*****************************************************************************/
//  Description : get password display theme,include border,bg etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPwDisplayTheme(
                                       MMITHEME_EDIT_PSW_DISPLAY_T  *theme_ptr  //in/out:
                                       );

/*****************************************************************************/
//  Description : get edit date min and max year
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
void MMITHEME_GetEditDateRange(
                               uint16    *min_year_ptr,  //in/out:
                               uint16    *max_year_ptr,  //in/out:
                               uint32    year_range_type //in:GUIEDIT_DATE_YEAR_RANGE_E
                               );

#if defined GUIF_EDIT_PDASTYLE && defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : open landscape edit win
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMITHEME_OpenLandscapeEdit(//edit ctrl id
                                                uint32          edit_type,      //GUIEDIT_TYPE_E
                                                MMI_CTRL_ID_T   parent_ctrl_id
                                                );

/*****************************************************************************/
//  Description : is landscape edit win
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITHEME_IsLandscapeEdit(
                                        MMI_HANDLE_T    win_handle
                                        );
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
