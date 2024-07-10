/*****************************************************************************
** File Name:      mmiebook_internal.h                                       *
** Author:                                                                   *
** Date:           11/02/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ebook                       *          
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2004       songbin.zeng       Create
** 02/2012       xiyuan.ma          Edit
******************************************************************************/
#ifndef _MMIEBOOK_INTERNAL_H_
#define _MMIEBOOK_INTERNAL_H_
#ifdef EBOOK_SUPPORT
#include "mmiebook_export.h"
#include "special_effect.h"

typedef enum
{
	MMIEBOOK_COLOR_BLACK,
 	MMIEBOOK_COLOR_PALM,
 	MMIEBOOK_COLOR_GRAY,
	MMIEBOOK_COLOR_BLUE,
	MMIEBOOK_COLOR_RED,
	MMIEBOOK_COLOR_WHITE,
	MMIEBOOK_COLOR_MAX
}MMIEBOOK_SET_COLOR_E;//枚举可供设置的六种颜色

typedef enum
{
	MMIEBOOK_BG_1,
 	MMIEBOOK_BG_2,
 	MMIEBOOK_BG_3,
	MMIEBOOK_BG_4,
	MMIEBOOK_BG_5,
	MMIEBOOK_BG_6,
	MMIEBOOK_BG_MAX
}MMIEBOOK_SET_BG_E;

typedef enum
{
	MMIEBOOK_DAY_NIGHT_MODE_CLOSE,
	MMIEBOOK_DAY_MODE,
	MMIEBOOK_NIGHT_MODE,
	MMIEBOOK_DAY_NIGHT_MODE_MAX
}MMIEBOOK_DAY_NIGHT_MODE_E;

//翻页方向
typedef enum
{
    MMIEBOOK_PAGE_UP = 0,  //向上翻页
    MMIEBOOK_LINE_UP ,      //向上翻行
	MMIEBOOK_PAGE_CUR ,  //不翻页
    MMIEBOOK_LINE_DOWN ,    //向下翻行
    MMIEBOOK_PAGE_DOWN   //向下翻页
} MMIEBOOK_PAGE_DIRECTION_E;

#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
typedef struct
{
    MMK_TP_SLIDE_E              slide_state;                                             
    GUI_POINT_T                 slide_start_point;                      
    GUI_POINT_T                 slide_pre_point;                        
    uint8                       redraw_timer_id;
    uint8                       fling_timer_id;
    uint8                       recover_timer_id;
    uint8                       update_result;
    uint8                       recover_move_direction;//0:向右；1:向左
    MMI_HANDLE_T                handle;
    SE_PAGE_TURN_MODE_E         turn_mode;
    int16                       hot_spot;
    float                       velocity;
    BOOLEAN                     is_init;
}MMI_SLIDE_EBOOK_T;
#endif

typedef struct 
{
    CAF_APPLET_T caf_applet;
    uint32  user_data;
}MMIEBOOK_APPLET_T;   //vt applet定义

typedef struct
{
	MMIFMM_DATA_INFO_T *list_data_ptr;
	uint32 index;
}MMIEBOOK_FMM_CALLBACK_INFO;
                                           
/*****************************************************************************/
//  Description : 调用fmm的接口打开资源管理器(过滤出电子书文件,可自定义软键栏)
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_OpenExplorerWinforEbook( 
                                             BOOLEAN is_has_read_ebook,
                                             BOOLEAN is_need_privacy_protection
                                             );
                


/*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsFileExist(
                                   const wchar *full_path_ptr,     //in
                                   uint16       full_path_len,    //in, 双字节为单位
                                   uint32       file_size
                                   );

/*****************************************************************************/
// 	Description : Register ebook menu group
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_RegMenuGroup(void);

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: maggie.ren
//	Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_RegNv(void);


/*****************************************************************************/
//  Description : MMIEBOOK_TurnLineOrPage
//  Global resource dependence :
//   Author: hui.zhao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_TurnLineOrPage(
                                    MMIEBOOK_PAGE_DIRECTION_E       direction //IN: 翻页方向
                                    );//向上/向下翻行或翻页

/*****************************************************************************/
 //     Description : 获取是否全屏显示
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetIsFullScreenOff(void);

/*****************************************************************************/
 //     Description : 
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SetIsTxtShowWinCtrlvisible(BOOLEAN is_visible);


/*****************************************************************************/
//     Description : 
// Global resource dependence : 
//  Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetIsNeedCloseTimer( void );

/*****************************************************************************/
//     Description : 
// Global resource dependence : 
//  Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SetIsNeedCloseTimer( BOOLEAN is_need_close );

/*****************************************************************************/
// 	Description : ebook set  TTS On or OFF
//	Global resource dependence : 
//  Author: koki.gao
//	Note:   
/*****************************************************************************/
#ifdef TTS_SUPPORT
PUBLIC MMI_HANDLE_T MMIEBOOK_OpenTTSOpenCloseWin(ADD_DATA add_data_ptr);
#ifdef EBOOK_TTS_SELF_VOLUME  
PUBLIC MMI_HANDLE_T MMIEBOOK_OpenTTSVolumeSetWin(ADD_DATA add_data_ptr);
#endif
#endif

/*****************************************************************************/
//  Description : Get ebook Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
#ifdef MMI_TASK_MANAGER
PUBLIC MMIEBOOK_APPLET_T *MMIEBOOK_GetAppletInstance(void);
#endif

/*****************************************************************************/
//  Description : register EBOOK module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_RegAppletInfo(void);

/*****************************************************************************/
//  Description : 在列表中寻找最近一次打开的电子书
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:TRUE->存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_FindLastOpenedBookinList( void ); 

PUBLIC BOOLEAN MMIEBOOK_GetLastOpenedBookPath(wchar *p_path_name, uint16 *p_path_name_len);

PUBLIC MMI_HANDLE_T MMIEBOOK_OpenTxtContentWin(ADD_DATA add_data_ptr);
PUBLIC MMI_HANDLE_T MMIEBOOK_OpenCodeTypeWin(ADD_DATA add_data_ptr);
PUBLIC MMI_HANDLE_T MMIEBOOK_OpenAutoReadSetWin(ADD_DATA add_data_ptr);


/*****************************************************************************/
//  Description : 资源管理中响应按键的callback函数
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_OpenPopmenuCallBack(MMIFMM_DATA_INFO_T *list_data_ptr,uint32 index);

#ifdef MMI_TASK_MANAGER
PUBLIC MMI_HANDLE_T MMIEBOOK_OpenEbookWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  );
#ifdef MMIEBOOK_BOOKSHELF_SUPPORT
#define MMK_CreateEbookWin( _WIN_TABLE_, _ADD_DATA_ )  MMIEBOOK_OpenEbookWin( _WIN_TABLE_, _ADD_DATA_ )
#else
#define MMK_CreateEbookWin   MMK_CreateWin
#endif

#else//MMI_TASK_MANAGER
#define MMK_CreateEbookWin   MMK_CreateWin
#endif

/*****************************************************************************/
//  Description : MMIEBOOK_ConvertFontSize
//  Global resource dependence :
//   Author: paul.huang
//  Note:   
/*****************************************************************************/
PUBLIC GUI_FONT_T MMIEBOOK_ConvertFontSize(MMIEBOOK_FONT_SIZE_E font_size);

#endif
#endif

