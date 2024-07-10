/****************************************************************************
** File Name:      mmicom_panel.h                                           *
** Author:                                                                  *
** Date:           2010/7/27                                                *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:                                                             *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 7/2010        aoke.hu            Create                                  *
**                                                                          *
****************************************************************************/
#ifndef MMICOM_PANEL_H
#define MMICOM_PANEL_H

/*-------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      */
/*-------------------------------------------------------------------------*/

#include "sci_types.h"
#include "guibutton.h"

/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             90
#elif MAINLCD_SIZE_240X320
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             134
#elif MAINLCD_SIZE_240X400
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             134
#elif MAINLCD_SIZE_320X480
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             180
#else
#define MMICOM_PLAYER_PANEL_PROGRESS_WIDTH             134
#endif

#ifndef UI_MULTILAYER_SUPPORT
#define MMICOM_PLAYER_PANEL_CREATE_WIN             0x01
#endif
/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/
typedef enum
{
    MMICOM_PANEL_VOLUME,
    MMICOM_PANEL_BRIGHTNESS,
    MMICOM_PANEL_TYPE_MAX
}MMICOM_PANEL_TYPE_E;

typedef enum
{
    MMICOM_PANEL_OPEN_NONE,
    MMICOM_PANEL_OPEN_INC,
    MMICOM_PANEL_OPEN_DEC,
    MMICOM_PANEL_OPEN_VALUE_SWITCH,     //打开时做静音非静音切换
    MMICOM_PANEL_OPEN_MAX
}MMICOM_PANEL_OPEN_TYPE_E;

typedef struct  
{
    uint32              cur_value;        //当前值。
    uint32              last_value;      //last_value在恢复静音时用，cur_value为0时有效
}MMICOM_PANEL_CALLBACK_T;

typedef void (*SETVALUE_CALLBACK)(MMICOM_PANEL_CALLBACK_T *para_ptr);

typedef struct  
{
    int16                       x;              //调节面板的x坐标, 预置负数则按默认算法
    int16                       y;              //调节面板的y坐标, 预置负数则按默认算法
    MMICOM_PANEL_TYPE_E         panel_type;    //调节面板类型
    MMICOM_PANEL_OPEN_TYPE_E    open_type;
    uint32                      cur_value;      //当前值,静音的话置为0
    uint32                      min_value;      //最小值,允许静音的话,置为0
    uint32                      max_value;      //最大值
    uint32                      last_value;      //last_value在恢复静音时用，cur_value为0时有效
    MMI_STRING_T                title_str;       //面板标题；  显示效果为："面板标题"音量调节/亮度调节
    uint32                      createwin_style; //当值为MMICOM_PLAYER_PANEL_CREATE_WIN时，表示创建了一个新窗口，默认创建子窗口，解决去图层引起的问题
    SETVALUE_CALLBACK           set_callback;   //调节函数
}MMICOM_PANEL_INFO_T;
 
/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 默认居中； 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_OpenPanelChildWin(MMI_WIN_ID_T win_id,
                                           MMICOM_PANEL_INFO_T *panel_info_ptr);

/*****************************************************************************/
//  Description :close panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_ClosePanelChildWin(void);

/*****************************************************************************/
//  Description : get panel is opened or not
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_IsPanelOpened(void);

/*****************************************************************************/
//  Description : get panel type
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC MMICOM_PANEL_TYPE_E MMIAPICOM_GetPanelType(void);

/*****************************************************************************/
//  Description : update panel
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_UpdatePanel(void);

/*****************************************************************************/
//  Description : display volume icon
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICOM_DisplayVolIcon( MMI_WIN_ID_T win_id,  
                                     uint8 volume, 
                                     GUIBUTTON_OWNER_DRAW_T    *owner_draw_ptr);

/*****************************************************************************/
//  Description : switch value
//  Global resource dependence : 
//  Author:aoke.hu
//  Note: value在0与非0间切换。    静音与非静音切换
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICOM_SwitchPanelValue(MMICOM_PANEL_TYPE_E  panel_type);

#endif  // MMICOM_PANEL_H
