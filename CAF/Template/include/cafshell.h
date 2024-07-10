/****************************************************************************
** File Name:      cafshell.h                                              *
** Author:         James.Zhang                                             *
** Date:           04/15/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAF_SHELL_H_    
#define  _CAF_SHELL_H_  

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "caf.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

#define CAF_WINDOW_TABLE(_WINTAB_)                  const uint32 _WINTAB_[]

//#define CAF_CLEAR_LCD                               CAF_PARSEWIN_CLEAR_LCD
   
#define CAF_WIN_ID(_ID_)                            CAF_PARSEWIN_WINID, _ID_

#define CAF_WIN_TITLE_ID(_TITEL_ID_)                CAF_PARSEWIN_WINTITLEID, _TITEL_ID_

#define CAF_WIN_STYLE(_WIN_STYLE_)                  CAF_PARSEWIN_WINDOW_STYLE , _WIN_STYLE_

#define CAF_WIN_BACKGROUND_ID(_BACKGROUND_ID_)      CAF_PARSEWIN_WINBACKGROUNDID, _BACKGROUND_ID_


#define CAF_WIN_SOFTKEY(_LEFT_SOFT_ID_, _MID_SOFT_ID_, _RIGHT_SOFT_ID_) \
        CAF_PARSEWIN_CREATE_SOFTKEY, _LEFT_SOFT_ID_, _MID_SOFT_ID_, _RIGHT_SOFT_ID_


// create the listbox control
#define CAF_CREATE_LISTBOX_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _LIST_TYPE_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_LISTBOX, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _LIST_TYPE_, _CTRL_ID_

// create the text editbox control
#define CAF_CREATE_TEXTEDITBOX_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _MAX_STR_LEN_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_TEXTEDITBOX, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _MAX_STR_LEN_, _CTRL_ID_

       // create the phonenumber editbox control
#define CAF_CREATE_PHONENUMEDITBOX_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _MAX_STR_LEN_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_PHONENUMEDITBOX, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _MAX_STR_LEN_, _CTRL_ID_

       // create the digital editbox control
#define CAF_CREATE_DIGITALEDITBOX_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _MAX_STR_LEN_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_DIGITALEDITBOX, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _MAX_STR_LEN_, _CTRL_ID_

       // create the password editbox control
#define CAF_CREATE_PASSWORDEDITBOX_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _MAX_STR_LEN_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_PASSWORDEDITBOX, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _MAX_STR_LEN_, _CTRL_ID_

       // create the label control
#define CAF_CREATE_LABEL_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _ALIGNMENT_, _TEXT_ID_, _IMAGE_ID_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_LABEL, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _ALIGNMENT_, _TEXT_ID_, _IMAGE_ID_, _CTRL_ID_

       // create the textbox control
#define CAF_CREATE_TEXTBOX_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _TEXT_ID_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_TEXTBOX, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _TEXT_ID_, _CTRL_ID_

    // create the animation
#define CAF_CREATE_ANIM_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _CTRL_ID_,_WIN_ID_) \
        CAF_PARSEWIN_CREATE_ANIM, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _CTRL_ID_,_WIN_ID_

    // create the button control
#define CAF_CREATE_BUTTON_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _ICON_ALIGN_,_BG_RELEASED_IMAGE_ID, _BG_PRESSED_IMAGE_ID, _GRAYED_FG_IMAGE_ID_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_BUTTON, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _ICON_ALIGN_, _BG_RELEASED_IMAGE_ID, _BG_PRESSED_IMAGE_ID, _GRAYED_FG_IMAGE_ID_,  _CTRL_ID_

    //create the dropdown list box
#define CAF_CREATE_DROPDOWNLIST_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, PAGE_ITEM_NUM,_CTRL_ID_) \
        CAF_PARSEWIN_CREATE_DROPDOWNLIST, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_,  PAGE_ITEM_NUM,_CTRL_ID_
        
    // create the iconlist control
#define CAF_CREATE_ICONLIST_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_,   _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_ICONLIST, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_,   _CTRL_ID_


// create the menu control        
#define CAF_CREATE_MENU_CTRL(_LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _GROUP_ID_, _CTRL_ID_) \
        CAF_PARSEWIN_CREATE_MENU, _LEFT_, _TOP_, _RIGHT_, _BOTTOM_, _GROUP_ID_, _CTRL_ID_


// create the pop menu control        
#define CAF_CREATE_POPMENU_CTRL(_CTRL_ID_) \
        CAF_PARSEWIN_CREATE_POPMENU, _CTRL_ID_

#define CAF_END_WIN                                 CAF_PARSEWIN_LAST



//caf user window\ctrl id start
#define CAF_USER_ID_START 0xff000000


/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

typedef enum
{
    CAF_PARSEWIN_FIRST = 0xE000,

    //CAF_PARSEWIN_CLEAR_LCD = CAF_PARSEWIN_FIRST,
    CAF_PARSEWIN_WINID = 0xE001,
    CAF_PARSEWIN_WINDOW_STYLE = 0xE002,
    CAF_PARSEWIN_WINTITLEID = 0xE003,
	CAF_PARSEWIN_WINBACKGROUNDID = 0xE004,

	CAF_PARSEWIN_CREATE_SOFTKEY = 0xE005,

    CAF_PARSEWIN_CREATE_LISTBOX = 0xE006,
    
    CAF_PARSEWIN_CREATE_TEXTEDITBOX = 0xE007, 
    CAF_PARSEWIN_CREATE_PHONENUMEDITBOX = 0xE008, 
    CAF_PARSEWIN_CREATE_DIGITALEDITBOX = 0xE009, 
    CAF_PARSEWIN_CREATE_PASSWORDEDITBOX = 0xE00A, 
    CAF_PARSEWIN_CREATE_DATEEDITBOX = 0xE00B, 
    CAF_PARSEWIN_CREATE_TIMEEDITBOX = 0xE00C, 

    CAF_PARSEWIN_CREATE_LABEL = 0xE00D,
    CAF_PARSEWIN_CREATE_TEXTBOX = 0xE00E,
    CAF_PARSEWIN_CREATE_ANIM = 0xE00F,
    CAF_PARSEWIN_CREATE_BUTTON = 0xE010,
    CAF_PARSEWIN_CREATE_DROPDOWNLIST = 0xE011,
    CAF_PARSEWIN_CREATE_ICONLIST = 0xE012,
    CAF_PARSEWIN_CREATE_MENU = 0xE013,
    CAF_PARSEWIN_CREATE_POPMENU = 0xE014,
    
 	CAF_PARSEWIN_MAX,
    CAF_PARSEWIN_LAST = 0xEFFF
} CAF_PARSEWIN_CODE_E;

typedef uint32                  CAF_HANDLE_RESULT_E;
#define CAF_HANDLE_RESULT_FALSE 0
#define CAF_HANDLE_RESULT_TRUE  1

// typedef enum
// {
// 	CAF_HANDLE_RESULT_FALSE,     /*!< when window, control or app do not handle this message, will return  CAF_HANDLE_RESULT_FALSE */ 
// 	CAF_HANDLE_RESULT_TRUE,      /*!< when window, control or app have handled this message, will return CAF_HANDLE_RESULT_TRUE */
// 
// }CAF_HANDLE_RESULT_E;

typedef uint32 CAF_WINDOW_DISPLAY_STYLE;
//#define CAF_WS_HAS_SOFTKEY                 0x01
#define CAF_WS_DISABLE_RETURN_WIN          0x01     //类似锁屏窗口, 按红键不会返回到idle
#define CAF_WS_HAS_PROMPT_WIN			   0x02     //pubwin窗口
#define CAF_WS_HAS_TRANSPARENT             0x04     //窗口透明显示
#define CAF_WS_WIN_PRIORITY_KEY            0x08     //窗口优先处理key消息
#define CAF_WS_HAS_ANIM_BG                 0x10     //idle窗口以anim控件为背景, 先画anim后画窗口
#define CAF_WS_DISABLE_HWICON              0x20     //pubwin等窗口不响应硬图标
#define CAF_WS_MASK                        0xFFFFFFFF //用于设置或去除窗口所有状态

//typedef uint32 CAF_MESSAGE_ID_E;

//CAF_TBD use wparam later
typedef CAF_HANDLE_RESULT_E (* WINDOW_HANDLE_EVENT)( CAF_HANDLE_T win_handle, CAF_MESSAGE_ID_E msg_id, void* param );

typedef struct  
{
    CAF_HANDLE_T         applet_handle;
    CAF_HANDLE_T         parent_win_handle;
    const uint32*        win_table_ptr;
    WINDOW_HANDLE_EVENT  func;
    void*                add_data_ptr;
    uint32               reserved;

}CAF_WINDOW_TABLE_CREATE_T;

typedef struct  
{
    CAF_GUID_T           guid;
    uint32               status;
    void*                param_ptr;
    uint32               size_of_param;
    CAF_HANDLE_T         parent_applet_handle;
    uint32               reserved;
        
}CAF_APPLET_START_T;

typedef struct 
{
    uint8   mday;       // day of the month - [1,31] 
    uint8   mon;        // months  - [1,12] 
    uint16  year;       // years [2000,2049] 
}CAF_DATE_T;

typedef struct 
{
    uint8   sec;        // secondsafter the minute - [0,59] 
    uint8   min;        // minutesafter the hour - [0,59] 
    uint8   hour;       // hours since midnight - [0,23] 
}CAF_TIME_T;

typedef struct
{
   uint16            screen_width;
   uint16            screen_height;

   uint32            reserved[32]; //reserved

}CAF_DEVICE_INFO;

typedef enum
{
    CAF_MB_ALERT,
    CAF_MB_QUERY,

} CAF_MB_ICON_TYPE_E;

typedef enum
{
    CAF_MB_SOFTKEY_NONE,        //none
    CAF_MB_SOFTKEY_EXIT,        //one:exit
    CAF_MB_SOFTKEY_OKEXIT,      //two:ok exit
    CAF_MB_SOFTKEY_OKCANCEL,    //two:ok cancel

} CAF_MB_SOFTKEY_TYPE_E;

typedef struct
{
    CAF_HANDLE_T          parent_handle;
    CAF_MB_ICON_TYPE_E    type;
    CAF_MB_SOFTKEY_TYPE_E sk_style;
    CAF_STRING_T          *text1_str;
    uint32                text1_id;
    CAF_STRING_T          *text2_str;
    uint32                text2_id;
    uint32                image_id;
    uint32                *time_period_ptr;
    uint32                reserved;

}CAF_MESSAGEBOX_CREATE_T;

//创建实例参数
typedef struct
{
    CAF_RECT_T      rect; //控件矩形
    CAF_HANDLE_T    parent_handle; //控件父窗口句柄    
    uint32          ctrl_id; //控件ID
    void *          init_data_ptr; //控件自定义数据
}CAF_CONTROL_PARAM_T;

//for include

//add by songliang.liu 
typedef void    *CAF_SEMAPHORE_PTR;
typedef struct IAPPLET_T  IAPPLET_T;
typedef struct IGUICTRL_T IGUICTRL_T;
typedef struct IDC_T      IDC_T;
#define IMedia void
#define IImage void


typedef struct
{
    CAF_RESULT_E	(*CreateInstance) ( CAF_GUID_T guid, void ** object_pptr );
    CAF_HANDLE_T	(*CreateWinTable) ( const CAF_WINDOW_TABLE_CREATE_T* win_table_ptr );
    BOOLEAN			(*CloseWindow)    ( CAF_HANDLE_T win_handle );
    BOOLEAN			(*SetWinSoftkeyId)( CAF_HANDLE_T win_handle, uint32 leftsoft_id, uint32 middlesoft_id, uint32 rightsoft_id, BOOLEAN is_need_update );
    IDC_T*			(*GetDC)          ( CAF_HANDLE_T handle );
    IAPPLET_T*		(*GetAppletByWin) ( CAF_HANDLE_T win_handle );
    IGUICTRL_T*		(*GetCtrlByWin)   ( CAF_HANDLE_T win_handle, uint32 ctrl_id );
    BOOLEAN			(*SetActiveCtrl)  ( CAF_HANDLE_T ctrl_handle );
    CAF_RESULT_E	(*GetSystemDate)  ( CAF_DATE_T* date_ptr );
    CAF_RESULT_E	(*GetSystemTime)  ( CAF_TIME_T* time_ptr );
    BOOLEAN			(*GetTpPoint)     ( void* param, CAF_POINT_T* point_ptr );
    BOOLEAN			(*PointIsInRect)  ( const CAF_POINT_T* point_ptr, const CAF_RECT_T* rect_ptr );
    CAF_RESULT_E	(*LoadImage)	  ( const wchar *  imagefile_ptr, IImage ** image_pptr);
    CAF_RESULT_E	(*LoadRing)		  (const wchar * ringfile_ptr, IMedia** media_pptr );
    CAF_RESULT_E	(*LoadAnim)       (const wchar * animfile_ptr, IImage ** image_pptr);
    CAF_RESULT_E	(*LoadResImage)   (uint32 image_id, CAF_HANDLE_T applet_handle, IImage ** image_pptr );
    CAF_RESULT_E	(*LoadResText)    (uint32 text_id, CAF_HANDLE_T applet_handle, CAF_STRING_T * str_ptr );
    CAF_RESULT_E	(*LoadResAnim)    (uint32 anim_id, CAF_HANDLE_T applet_handle, IImage ** image_pptr );
    CAF_RESULT_E	(*LoadResRing)    (uint32 ring_id, uint32 ring_type,CAF_HANDLE_T applet_handle, IMedia** media_pptr );
    CAF_RESULT_E	(*LoadResData)    (uint32 res_id, CAF_RESOURCE_TYPE_E type ,  CAF_HANDLE_T applet_handle, void ** buffer_pptr);
    CAF_RESULT_E	(*StartApplet)    ( const CAF_APPLET_START_T* start_ptr );   
    CAF_RESULT_E	(*CloseApplet)    ( CAF_HANDLE_T applet_handle );
    BOOLEAN			(*PostMessage)    ( CAF_HANDLE_T handle, CAF_MESSAGE_ID_E msg_id, void* param_ptr, uint32 size_of_param );
    BOOLEAN			(*SendMessage)    ( CAF_HANDLE_T handle, CAF_MESSAGE_ID_E msg_id, void* param_ptr );
    uint8			(*CreateTimer)    ( CAF_HANDLE_T handle, uint32 time_out, BOOLEAN is_period );
    BOOLEAN			(*StartTimer)     ( CAF_HANDLE_T handle, uint8 timer_id, uint32 time_out, BOOLEAN is_period );
    BOOLEAN			(*StopTimer)      ( uint8 timer_id );
    BOOLEAN			(*PauseTimer)     ( uint8 timer_id );
    BOOLEAN			(*ResumeTimer)    ( uint8 timer_id );
    BOOLEAN			(*IsTimerActive)  ( uint8 timer_id );
    void			(*GetScreenInfo)  ( CAF_SCREENINFO_T* info_ptr ) ;
    CAF_RESULT_E	(*GetMemoryInfo)  ( CAF_MEMORYINFO_T* info_ptr ,CAF_MEMORY_TYPE_E memory_type);
    void	        (*MessageBox)     ( const CAF_MESSAGEBOX_CREATE_T* msgbox_ptr );
    void		    (*AllowTurnOffBackLight) ( BOOLEAN allow_turn_off );
    CAF_SEMAPHORE_PTR (*CreateSemaphore)( const char *name_ptr,	uint32 initial_count );
    uint32			(*DeleteSemaphore)( CAF_SEMAPHORE_PTR sem_ptr );
    uint32			(*GetSemaphore)   (CAF_SEMAPHORE_PTR sem_ptr, uint32 wait_option );
    uint32			(*PutSemaphore)   (CAF_SEMAPHORE_PTR sem_ptr );
    
    //added by andrew.zhang at 2009/05/26
    BOOLEAN			(*SetWinTitle)    ( CAF_HANDLE_T handle, CAF_STRING_T * str_ptr, BOOLEAN is_need_update );
    BOOLEAN			(*SetWinTitleByID)( CAF_HANDLE_T handle, uint32 text_id, BOOLEAN is_need_update );
    
    BOOLEAN         (*GetAppletInstallDirectory)( CAF_HANDLE_T handle, wchar *path_ptr, uint16 *wstr_len_ptr );
    BOOLEAN         (*IsFocusWindow)( CAF_HANDLE_T win_handle );
    
    BOOLEAN         (*GetWinRect)( CAF_HANDLE_T handle, CAF_RECT_T*    rect_ptr );
    
    CAF_RESULT_E    (*CreateInstanceEx)( CAF_GUID_T guid, void ** object_pptr, void * param_ptr );
    CAF_RESULT_E    (*CloseInstance)( CAF_GUID_T guid, void * object_ptr );	
    
    uint32          (*SendNotify)( CAF_HANDLE_T ctrl_handle, CAF_MESSAGE_ID_E msg_id );		
    uint32          (*PostNotify)( CAF_HANDLE_T ctrl_handle, CAF_MESSAGE_ID_E msg_id );		
}CAF_SHELL_T;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
