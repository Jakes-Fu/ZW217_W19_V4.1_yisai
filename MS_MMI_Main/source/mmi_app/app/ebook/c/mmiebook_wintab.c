/****************************************************************************
** File Name:      mmiebook_wintab.c                              
** Author:         songbin.zeng                                                        
** Date:           2006/06/9 
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This file is used to read/write text file
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 06/6/2006         songbin.zeng          Create
** 
****************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "mmi_app_ebook_trc.h"
#include "window_parse.h"

#ifdef EBOOK_SUPPORT 
#include "tb_dal.h"
#include "mmk_app.h"
#include "guitext.h"
#include "guilcd.h"
#include "guilistbox.h"
#include "mmidisplay_data.h"
#include "mmi_menutable.h"
#include "mmipub.h"
#include "mmidisplay_data.h"
#include "mmi_nv.h"
#include "mmi_default.h"
#include "mmi_common.h"
#include "mmi_nv.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "guilabel.h"
#include "guifont.h"
#include "mmifmm_export.h"
#include "guibutton.h"
#include "mmibt_export.h"
#include "mmi_appmsg.h"
#include "guimenu.h"
#include "mmifmm_id.h"
#include "mmiudisk_export.h"
#include "mmi_mainmenu_export.h"
#include "guitab.h"
#include "mmicc_export.h"
#include "mmimp3_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmikl_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "guirichtext.h"
#include "guires.h"
#include "guimenu.h"
#include "guiedit.h"
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#include "Mmisrvaud_api.h"
#endif
//#include "mmiset_text.h"
//#include "mmiset_set_ring.h"
#include "mmisms_set.h"

//#include "mmiset_set_ring.h"
#include "guiform.h"
#include "guictrl_api.h"
#ifdef MMS_SUPPORT
#include "mmimms_export.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#include "mmiset_text.h"
#include "mmk_tp.h"

#include "mmiebook_nv.h"
#include "mmiebook_position.h"  
#include "mmiebook_menutable.h"
#include "mmiebook_image.h"
#include "mmiebook_id.h"
#include "mmiebook_text.h"
#include "mmiebook_file.h"
#include "mmiebook_task.h"
//#include "mmiebook.h"
#include "mmiebook_internal.h"
#ifdef MMI_TASK_MANAGER
#include "mmiebook_bookshelf.h"
#endif
#include "mmitheme_button.h"
#if defined MMI_VCARD_SUPPORT
#include "mmipb_export.h"
#include "mmipb_image.h"
#endif
#ifdef TOUCH_PANEL_SUPPORT
#ifndef WIN32
#include  "tp_srv.h"
#include "tp_gesture_drv.h"
#endif
#endif
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
/*#if defined MMI_PDA_SUPPORT
#define MMIEBOOK_COPY_SUPPORT
#elif !defined MMIEBOOK_PAGESLIDE_SUPPORT
#define MMIEBOOK_COPY_SUPPORT
#endif*/

#define     MMIEBOOK_INPUT_PERCENT_RANGE               100

//#ifndef WIN32//@zhaohui,cr111080
#define  UCS_CHAR_BLANK 0x0020
#define  UCS_CHAR_UNDEF 0x3000
//#else
//#define  UCS_CHAR_BLANK   0x2000
//#define  UCS_CHAR_UNDEF   0x0030
//#endif

#ifdef TTS_SUPPORT
#define MMIEBOOK_TTS_ITEM_MAX_NUM   2
#define EBOOK_AUDIO_SRV_NAME   "ebook_tts"
#endif 

#define  SCREEN_SWITCH_TIME_OUT  100

#define  MMIEBOOK_MAX_FILE_SIZE  (500*1024*1024)

//判断(x,y)点是否在rect区域内
#define IsInRect(x, y, rect) ((x>=rect.left&&x<=rect.right)&&(y>=rect.top&&y<=rect.bottom))

#define DAY_MODE_FONT_COLOR MMIEBOOK_COLOR_BLACK
#define NIGHT_MODE_FONT_COLOR MMIEBOOK_COLOR_WHITE
#define DAY_MODE_BG_ID MMIEBOOK_BG_2
#define NIGHT_MODE_BG_ID MMIEBOOK_BG_3

/**--------------------------------------------------------------------------*
**                              STRUCT DEFINITION                        *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         EXTERN VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
typedef enum
{
    MMIEBOOK_BUTTON_COLOR_1_FONT,
    MMIEBOOK_BUTTON_COLOR_2_FONT,
    MMIEBOOK_BUTTON_COLOR_3_FONT,
    MMIEBOOK_BUTTON_COLOR_4_FONT,
    MMIEBOOK_BUTTON_COLOR_5_FONT,
    MMIEBOOK_BUTTON_COLOR_6_FONT,
    MMIEBOOK_BUTTON_COLOR_1_BACK,
    MMIEBOOK_BUTTON_COLOR_2_BACK,
    MMIEBOOK_BUTTON_COLOR_3_BACK,
    MMIEBOOK_BUTTON_COLOR_4_BACK,
    MMIEBOOK_BUTTON_COLOR_5_BACK,
    MMIEBOOK_BUTTON_COLOR_6_BACK,
    MMIEBOOK_BUTTON_COLOR_MAX/*lint !e751*/
}MMIEBOOK_COLOR_BUTTON_E;

typedef enum
{
    MMIEBOOK_FOCUS_ON_FONT_COLOR,
    MMIEBOOK_FOCUS_ON_BACK_COLOR
}MMIEBOOK_SET_COLOR_FOCUS_E;
#ifdef MMIEBOOK_BOOKSHELF_SUPPORT
#ifndef MMI_TASK_MANAGER
extern const uint32 MMIEBOOK_LATEST_LIST_WIN_TAB[];/*lint !e752*/
#endif
#endif
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
LOCAL    BOOLEAN                s_ebook_file_list_update = FALSE;
LOCAL   BOOLEAN                        s_is_backlight_turn_off = FALSE;//@maggie add 
/* Bug fix:cr00198795. */

LOCAL MMIEBOOK_SET_BG_E s_ebook_bg_id = MMIEBOOK_BG_1;
LOCAL MMIEBOOK_SET_COLOR_E s_ebook_fontcolor_id = MMIEBOOK_COLOR_WHITE;
LOCAL MMIEBOOK_FONT_SIZE_E s_ebook_fontsize_id = MMIEBOOK_FONT_NORMAL;
LOCAL uint32 s_contrast = 0;
#if defined( MMI_GUI_STYLE_MINISCREEN)  
LOCAL BOOLEAN s_is_has_read = FALSE;
#endif
const GUI_COLOR_T s_ebook_color[MMIEBOOK_COLOR_MAX] ={
    0x0000,
    0x400E,
    0x94B2,
    0xFF34,
    0xAB7E,
    0xFFFF
};

const GUI_COLOR_T s_ebook_back_color[3] ={
    0x370,
    0xFFFF,
    0x80
};
#ifndef MMI_LOW_MEMORY_EBOOK
const uint32 s_ebook_back_img[3] ={
    IMAGE_EBOOK_COLOR_BG1,
    IMAGE_EBOOK_COLOR_BG2,
    IMAGE_EBOOK_COLOR_BG3
};
#endif
//电子书color设置button信息的数据结构
LOCAL const GUI_COLOR_T s_color_button_info[MMIEBOOK_BUTTON_COLOR_MAX] =
{
    // COLOR 1 FOR FONT
    0x0000,
    
    // COLOR 2 FOR FONT
    0x400E,

    // COLOR 3 FOR FONT
    0x94B2,

    // COLOR 4 FOR FONT
    0xFF34,

    // COLOR 5 FOR FONT
    0xAB7E,

    // COLOR 6 FOR FONT
    0xFFFF,

    // COLOR 1 FOR BACK
    0x0000,

    // COLOR 2 FOR BACK
    0x400E,

    // COLOR 3 FOR BACK
    0x94B2,

    // COLOR 4 FOR BACK
    0xFF34,

    // COLOR 5 FOR BACK
    0xAB7E,

    // COLOR 6 FOR BACK
    0xFFFF,
};
LOCAL   BOOLEAN s_is_full_screen_off = FALSE;

//LOCAL   BOOLEAN s_is_set_txt_box_rect = FALSE;

LOCAL   uint8   s_auto_read_timer_id = 0;

LOCAL   BOOLEAN s_is_need_close_timer = TRUE;


#ifdef TTS_SUPPORT
LOCAL   BOOLEAN    s_tts_auto_pagedown_flag = FALSE;
#endif

//LOCAL GUI_POINT_T s_down_point = {0};

#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
LOCAL MMI_HANDLE_T s_label_handle[MMIEBOOK_OSD_LABEL_NUM] = {PNULL,PNULL,PNULL,PNULL,PNULL};
#else
LOCAL MMI_HANDLE_T s_label_handle[MMIEBOOK_OSD_LABEL_NUM] = {PNULL,PNULL,PNULL,PNULL,PNULL,PNULL,PNULL};
#endif
#else//TOUCH_PANEL_SUPPORT
LOCAL MMI_HANDLE_T s_label_handle[MMIEBOOK_OSD_LABEL_NUM] = {PNULL,PNULL,PNULL,PNULL,PNULL};
#endif
#ifdef MMIEBOOK_OSDMENU_SUPPORT
LOCAL GUI_LCD_DEV_INFO s_osd_dev={ 0,UILAYER_NULL_HANDLE };
#else
LOCAL GUI_LCD_DEV_INFO s_osd_dev={ 0,0};
#endif
LOCAL BOOLEAN s_osd_is_active=FALSE;
//进入电子书显示浮动面板的时间
LOCAL   uint8   s_osd_display_timer_id = 0;
#define EBOOK_OSD_MENU_DISPLAY_TIME    2000     //OSD MENU SHOW TIME before closing

#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
LOCAL MMI_SLIDE_EBOOK_T  s_slide_ebook={0}; 
LOCAL GUI_LCD_DEV_INFO s_cur_page_dev={ 0,UILAYER_NULL_HANDLE };
LOCAL GUI_LCD_DEV_INFO s_next_page_dev={ 0,UILAYER_NULL_HANDLE };
LOCAL GUI_LCD_DEV_INFO s_out_page_dev={ 0,UILAYER_NULL_HANDLE };
LOCAL GUI_LCD_DEV_INFO s_main_dev = { GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN };
LOCAL GUI_LCD_DEV_INFO s_text_dev = { 0,UILAYER_NULL_HANDLE };
LOCAL SE_HANDLE s_effect_handle=NULL;
//LOCAL GUI_POINT_T s_down_point={0};

#define MMIEBOOK_SLIDE_REDRAW_TIME               10
#define MMIEBOOK_SLIDE_FLING_TIME               10
#define EBOOK_TP_MOVE_DIS      3
#define TOUCH_FLING_TIME                ((float)0.05)
#ifdef MAINLCD_SIZE_320X480
#define EBOOK_SLIDE_RECOVER_STEP    40
#else
#define EBOOK_SLIDE_RECOVER_STEP    20
#endif
#endif

LOCAL BOOLEAN is_disenable_txt_func=FALSE;

#ifdef TOUCH_PANEL_SUPPORT
LOCAL uint32 s_pre_tp_msg=MSG_TP_PRESS_DOWN;
LOCAL uint32 s_gesture_num_in=0;
LOCAL uint32 s_gesture_num_out=0;
#endif
#ifdef MMI_TASK_MANAGER	
/*****************************************************************************/
//  Description : the process message function of the ebook
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  EbookApplet_HandleEvent(    
            IAPPLET_T*          iapplet_ptr,
            MMI_MESSAGE_ID_E    msg_id, 
            DPARAM              param
            );

LOCAL MMIEBOOK_APPLET_T *s_ebook_applet_instance = PNULL; 
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmiebook_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_EBOOK_APPLET_ID), sizeof(MMIEBOOK_APPLET_T), EbookApplet_HandleEvent , 
        IMAGE_SECMENU_ICON_PASTIME_EBOOK, TXT_COMM_EBOOK},
};

//applet的信息, 被mmi_applet_table.c extern
LOCAL const CAF_STATIC_APPLET_INFO_T g_mmiebook_applet_info = 
{
    s_mmiebook_applet_array,/*lint !e605*/
    ARR_SIZE(s_mmiebook_applet_array)
};

#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/**************************************************************************************************/
// Description:import vcard call back function
// Date:
// Author:
// Note:
//*************************************************************************************************/
LOCAL void  Importvcard_Callback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleImportVcardWaitingWinMsg(
                                        MMI_WIN_ID_T win_id,            
                                        MMI_MESSAGE_ID_E msg_id,        
                                        DPARAM param
                                        );

/*****************************************************************************/
//  Description : 电子书显示窗口
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTxtShowWinMsg(
                                        MMI_WIN_ID_T win_id,            
                                        MMI_MESSAGE_ID_E msg_id,        
                                        DPARAM param
                                        );

/*****************************************************************************/
//  Description : 电子书显示选项
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTxtShowOptWinMsg (
                                            MMI_WIN_ID_T win_id,            
                                            MMI_MESSAGE_ID_E msg_id,        
                                            DPARAM param
                                            );

/*****************************************************************************/
//  Description : 书签列表
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBookmarkListWinMsg(
                                             MMI_WIN_ID_T win_id,           
                                             MMI_MESSAGE_ID_E msg_id,       
                                             DPARAM param
                                             );

/*****************************************************************************/
//  Description : 书签列表选项
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBookmarkListOptWinMsg(
                                                MMI_WIN_ID_T win_id,            
                                                MMI_MESSAGE_ID_E msg_id,        
                                                DPARAM param
                                                );
#ifdef MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E  HandleBookmarkListOptPopupWinMsg(
                                                    MMI_WIN_ID_T win_id,            
                                                    MMI_MESSAGE_ID_E msg_id,        
                                                    DPARAM param
                                                    );
#endif

//@maggie add begin for ebook    
/*****************************************************************************/
//  Description : open ebook txt show opt menu
//  Global resource dependence : 
//  Author:maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void  OpenEbookShowOptMenu(void);

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence : 
//  Author:maggie.ren
//  Note:
/*****************************************************************************/                           
LOCAL void InsertNodeByLabel(
                                     MMI_CTRL_ID_T      ctrl_id,        //控件id
                                     MMI_TEXT_ID_T      text_id,       //插入的节点的文本
                                     MMI_IMAGE_ID_T     image_id,       //插入的节点的文本
                                     uint32             node_id,        //节点编号
                                     uint32             parent_node_id, //父节点编号
                                     uint16             index           //位置
                                     );



//add by rongjin, fixed bug 21540, 070723        
LOCAL MMI_RESULT_E  HandleEnterTxtContentWinMsg(
                                             MMI_WIN_ID_T win_id,     
                                             MMI_MESSAGE_ID_E msg_id,     
                                             DPARAM param
                                             );

/*****************************************************************************/
//  Description : 大电子书警告窗口处理
//  Global resource dependence : 
//   Author: zhaohui
//  Note:   
/*****************************************************************************/
/*LOCAL MMI_RESULT_E  HandleBigEbookAlertWinMsg(
                                        MMI_WIN_ID_T win_id,            
                                        MMI_MESSAGE_ID_E msg_id,        
                                        DPARAM param
                                        );*/     

/*****************************************************************************/
//  Description : SetPreviewLabelColor
//  Global resource dependence : 
//  Author:   zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  SetPreviewLabelColor( MMI_CTRL_ID_T ctrl_id, MMIEBOOK_SET_COLOR_E color_id, 
                               MMIEBOOK_SET_BG_E backcolor_id, MMIEBOOK_FONT_SIZE_E size_id, BOOLEAN is_update);

/*****************************************************************************/
//  Description : HandleSetColorCtrlMsg
//  Global resource dependence : 
//  Author:   zhaohui
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSetColorCtrlMsg(
                               MMI_WIN_ID_T win_id,
                               MMI_CTRL_ID_T ctrl_id
                               );

                               
 /*****************************************************************************/
//  Description : 颜色设置存入NV
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetColorType(
     void
);

 /*****************************************************************************/
//     Description : restore lcd contrast
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void RestoreLcdContrast(void);

  /*****************************************************************************/
//     Description : set lcd contrast
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void SetLcdContrast(void);

 /*****************************************************************************/
//     Description : get the day night mode
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL MMIEBOOK_DAY_NIGHT_MODE_E GetDayNightMode(void);

  /*****************************************************************************/
  //     Description : set the day night mode
  // Global resource dependence : 
  //  Author: paul.huang
  // Note:
  /*****************************************************************************/
  LOCAL void SetDayNightMode(MMIEBOOK_DAY_NIGHT_MODE_E mode);

  /*****************************************************************************/
 //     Description : init the day night mode
 // Global resource dependence : 
 //  Author: paul.huang
 // Note:
 /*****************************************************************************/
 LOCAL void InitDayNightMode(void);

 /*****************************************************************************/
//     Description : check the day night mode
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void CheckDayNightMode(void);
 
/*****************************************************************************/
//  Description : write ebook book mark info
//  Global resource dependence : 
//  Author:  jian.ma
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  WriteEbookBookMarkInfo(BOOLEAN is_open_alert_win);

/*****************************************************************************/
//  Description : append bookmark to list ctrl
//  Global resource dependence : 
//  Author:  jian.ma
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void AppendBookmarkItemData(
                                 MMI_CTRL_ID_T  ctrl_id,
                                 uint16 index
                                 );

/*****************************************************************************/
//  Description : CloseEbookOpenWaitWin
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void CloseEbookOpenWaitWin( void );  //关闭打开电子书时的等待窗口

/*****************************************************************************/
//  Description : Enter txt or list
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void EnterTxtorList( BOOLEAN is_need_privacy_protection );

/*****************************************************************************/
//  Description : CloseAllEbookWin
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void CloseAllEbookWin( void );

/*****************************************************************************/
//  Description : 关闭电子书入口窗口
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void CloseEnterEbookWin( void );

/*****************************************************************************/
//  Description : Enter txt or list
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void OpenFmmWinFromShowWin( void );//若fmm窗口未打开，退出电子书阅读窗口时应打开该fmm窗口


/*****************************************************************************/
//  Description : EnterTxtContent
//  Global resource dependence : 
//   Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void EnterTxtContent(MMIEBOOK_FILE_BRIEF_T* ebook_file_brief,MMI_MESSAGE_ID_E msg_id);
#ifdef TTS_SUPPORT

/*****************************************************************************/
// 	Description : get the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIEBOOK_GetTTSInfo( void );

/*****************************************************************************/
// 	Description : get the info of TTS setting status
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
#ifdef EBOOK_TTS_SELF_VOLUME  
LOCAL uint8 MMIEBOOK_GetTTSVol( void );

/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void MMIEBOOK_SetTTSVol(
                              uint8 vol
                              );
#endif
/*****************************************************************************/
// 	Description : set the info of TTS setting status
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
LOCAL void MMIEBOOK_SetTTSInfo(
                               MMI_CTRL_ID_T	    ctrl_id
                               ); 
/*****************************************************************************/
// 	Description : add set tts list item
//	Global resource dependence : 
//  Author: barry.peng
//	Note:
/*****************************************************************************/
LOCAL void MMIEBOOK_AppendTTSListItem(
                                      MMI_CTRL_ID_T	    ctrl_id
                                      );

/*****************************************************************************/
// 	Description : to handle set TTS
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEbookSetTTSOpenCloseWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     );
/*****************************************************************************/
//Description : set tts vol
//Global resource dependence : 
//Author: 
//Note:
/*****************************************************************************/
#ifdef EBOOK_TTS_SELF_VOLUME
LOCAL MMI_RESULT_E  HandleEbookTTSVolSetWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               );
#endif
#endif

/*****************************************************************************/
//  Description : 编码方式设置界面
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCodingType(
                        MMI_WIN_ID_T       win_id, 
                        MMI_MESSAGE_ID_E   msg_id, 
                        DPARAM             param
);

/*****************************************************************************/
//  Description : 编码方式选项:1、GB(简体中文) 2、Big5(繁体中文)
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL void AppendCodingTyleListItem(
     MMI_CTRL_ID_T      ctrl_id
     );
     
/*****************************************************************************/
//  Description : 设置编码方式
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetCodingType(
     MMI_CTRL_ID_T      ctrl_id
);

/*****************************************************************************/
//  Description : 文本文件阅读窗口中设置横竖屏模式下的各控件的区域
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void SetTxtShowCtrlRect(void);

/*****************************************************************************/
//  Description : 电子书字体大小设置界面
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
 LOCAL MMI_RESULT_E  HandleFontSizeWinMsg(
                                         MMI_WIN_ID_T       win_id, 
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );                               
/*****************************************************************************/
//  Description : 字体大小选项:1、大 2、中 3、小
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
// LOCAL void AppendFontSizeListItem(
//      MMI_CTRL_ID_T      ctrl_id
//      );
     
/*****************************************************************************/
//  Description : 设置字体大小
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
// LOCAL void SetFontSize(
//                          MMI_CTRL_ID_T      ctrl_id
//                          );

/*****************************************************************************/
//  Description : 跳转页面
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePercentInputWinMsg (
                                           MMI_WIN_ID_T win_id,             
                                           MMI_MESSAGE_ID_E msg_id,         
                                           DPARAM param
                                           );


/*****************************************************************************/
 //     Description : 设置是否全屏显示
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
/*****************************************************************************/
LOCAL void SetIsFullScreenOff(BOOLEAN is_off);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void AppendAutoReadListItem(
     MMI_CTRL_ID_T      ctrl_id
     );

/*****************************************************************************/
//  Description : 自动浏览设置界面
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoRead(
                                        MMI_WIN_ID_T       win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );
                                        

/*****************************************************************************/
//  Description : 设置自动浏览类型
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetAutoReadType(
                         MMI_CTRL_ID_T      ctrl_id
                         );

/*****************************************************************************/
// 	Description : Start timer 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void EbookStartTimer(
                           uint8 *timer_id,
                           uint32 time_out,
                           BOOLEAN is_period
                           );

/*****************************************************************************/
// 	Description : close timer 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void EbookCloseTimer(
                           uint8 *timer_id
                           );

/*****************************************************************************/
//  Description : 获得自动浏览时间
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetAutoReadTime(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void SetTxtBoxRect(void);


/*****************************************************************************/
//  Description : 设置电子书字体大小
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetEbookFontSize(MMIEBOOK_FONT_SIZE_E font_size);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFileErrorAlertWinMsg(
                                        MMI_WIN_ID_T win_id,            
                                        MMI_MESSAGE_ID_E msg_id,        
                                        DPARAM param
                                        );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void  HandleLCDSwitchMsg(MMI_WIN_ID_T win_id);
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void  HandleShowTpUpMsg(MMI_WIN_ID_T win_id, DPARAM param);
#endif
#ifdef MMIEBOOK_OSDMENU_SUPPORT
/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateOSDMenu(MMI_WIN_ID_T win_id);

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DestroyOSDMenu(void);

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetOSDCtrlParam(void);

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetOSDCtrlDevInfo(GUI_LCD_DEV_INFO* s_osd_dev);

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateOSDLayer(MMI_WIN_ID_T win_id,GUI_LCD_DEV_INFO* s_osd_dev);

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateOSDCtrl(MMI_WIN_ID_T win_id);
#else
/****************************************************************************/    
//  Description :CreateTitleMenu
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateTitleMenu(MMI_WIN_ID_T win_id);

/****************************************************************************/    
//  Description :DestroyTitleMenu
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void DestroyTitleMenu(void);

/****************************************************************************/    
//  Description : SetTitleCtrlParam
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void SetTitleCtrlParam(void);

/****************************************************************************/    
//  Description :SetTitleCtrlDevInfo
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void SetTitleCtrlDevInfo(GUI_LCD_DEV_INFO* s_osd_dev);

/****************************************************************************/    
//  Description :CreateTitleCtrl
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateTitleCtrl(MMI_WIN_ID_T win_id);
#endif
/*****************************************************************************/
// 	Description : ButtonOwnerDrawFun
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr);

/*****************************************************************************/
 //     Description : HandleSetColorStyle
 // Global resource dependence : 
 //  Author: zhaohui
 // Note:
 /*****************************************************************************/
 LOCAL MMI_RESULT_E  HandleSetFontColor(
     MMI_WIN_ID_T       win_id, 
     MMI_MESSAGE_ID_E   msg_id, 
     DPARAM             param
     );

/*****************************************************************************/
 //     Description : HandleSetBGColor
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
 /*****************************************************************************/
 LOCAL MMI_RESULT_E  HandleSetBGColor(
     MMI_WIN_ID_T       win_id, 
     MMI_MESSAGE_ID_E   msg_id, 
     DPARAM             param
     );
     
#ifdef    MMIEBOOK_PAGESLIDE_SUPPORT
/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateAllTurnPageLayer(MMI_WIN_ID_T win_id);

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void ReleaseAllTurnPageLayer(void);

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateTextLayer(MMI_WIN_ID_T win_id,GUI_LCD_DEV_INFO* dev_ptr);

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateTurnPageLayer(MMI_WIN_ID_T win_id,GUI_LCD_DEV_INFO* dev_ptr);

/*****************************************************************************/
// Description : InitSlidecalendar
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void InitSlideEbook( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr,
                                MMI_HANDLE_T    handle);

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideRedrawTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr);
/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideRedrawTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr);

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideFlingTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr);

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideFlingTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr);

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideRecoverTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr);

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideRecoverTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr);
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleEbookSlideTpDown(
                                GUI_POINT_T       *tp_point_ptr,    // [in]
                                MMI_SLIDE_EBOOK_T    *slide_Ebook_ptr,    // [in]
                                MMI_HANDLE_T    handle
                                );


/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleEbookSlideTpMove(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_SLIDE_EBOOK_T    *slide_Ebook_ptr    // [in]
                                );

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleEbookSlideTpUp(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_SLIDE_EBOOK_T    *slide_Ebook_ptr    // [in]
                                );

/*****************************************************************************/
// Description : CreateEffect
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateEffect(SE_PAGE_TURN_MODE_E mode);

#endif
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void ExitSlideEbook( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr,BOOLEAN is_update,BOOLEAN is_update_text);
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void SetEffectBuffData(MMI_SLIDE_EBOOK_T *slide_Ebook_ptr);

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void DestroyEffect(SE_HANDLE handle);
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenderEffect(int16 hot_sot);

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void UpdateMainLayerBuffer(void);

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideEbookTimer(
                                       MMI_SLIDE_EBOOK_T *slide_Ebook_ptr, // [in]
                                       DPARAM           param           // [in]
                                       );

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint8 UpdateEbookLayer(
                               MMI_SLIDE_EBOOK_T *slide_Ebook_ptr, // [in]
                               int32 x_offset
                               );

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void UpdateEbookRecoverMoveDirection( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr);

#endif

/*****************************************************************************/
//     Description : SettingButtonCallBack
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SettingButtonCallBack(void);

/*****************************************************************************/
//     Description : HandleDisplaySettingWinMsg
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDisplaySettingWinMsg(
    MMI_WIN_ID_T       win_id, 
    MMI_MESSAGE_ID_E   msg_id, 
    DPARAM             param
    );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:paul.huang
//  Note:
/*****************************************************************************/
LOCAL void AppendDayNightModeListItem(
     MMI_CTRL_ID_T      ctrl_id
     );

/*****************************************************************************/
//  Description : 黑夜白天模式设置窗口
//  Global resource dependence : 
//  Author:paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDayNightModeWinMsg(
                                        MMI_WIN_ID_T       win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        );

#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void freeVirtuaLAudioHandle(MMISRV_HANDLE_T *handle);
#endif

/**--------------------------------------------------------------------------*
**                         WINTAB DEFINITION                                *
**--------------------------------------------------------------------------*/
//add by rongjin, fixed bug 21540, 070723        
WINDOW_TABLE(MMIEBOOK_ENTER_TXT_CONTENT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleEnterTxtContentWinMsg ),    
    WIN_ID(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_HIDE_STATUS,
#endif
    END_WIN
};
//add end

//电子书阅读窗口
WINDOW_TABLE( MMIEBOOK_TXT_SHOW_WIN_TAB ) = 
{
    WIN_FUNC((uint32)HandleTxtShowWinMsg),    
    WIN_ID( MMIEBOOK_TXT_SHOW_WIN_ID ),
#ifndef MMIEBOOK_OSDMENU_SUPPORT
    WIN_TITLE( TXT_NULL), 
#endif  
    WIN_HIDE_STATUS,
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif
    CREATE_TEXT_CTRL(MMIEBOOK_TXT_SHOW_CTRL_ID),
    END_WIN
};

//查看书签列表窗口
WINDOW_TABLE( MMIEBOOK_BOOKMARK_LIST_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)HandleBookmarkListWinMsg),    
        WIN_ID( MMIEBOOK_BOOKMARK_LIST_WIN_ID ),
        WIN_TITLE( TXT_EBOOK_SEE_BOOKMARK),
#ifndef MMI_PDA_SUPPORT
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIEBOOK_BOOKMARK_LIST_CTRL_ID),
        END_WIN
};

WINDOW_TABLE( MMIEBOOK_PERCENT_INPUT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePercentInputWinMsg),    
    WIN_ID( MMIEBOOK_PERCENT_INPUT_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_TITLE(TXT_NULL),
    //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    //WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_CANCEL),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIEBOOK_FORM_PERCENT_INPUT_CTRL_ID),
    CHILD_EDIT_DIGITAL_CTRL(TRUE,3,MMIEBOOK_PERCENT_INPUT_CTRL_ID,MMIEBOOK_FORM_PERCENT_INPUT_CTRL_ID),
    CHILD_SOFTKEY_CTRL(TXT_COMMON_OK, TXT_NULL, STXT_CANCEL,MMICOMMON_SOFTKEY_CTRL_ID,MMIEBOOK_FORM_PERCENT_INPUT_CTRL_ID),
#else
    WIN_BACKGROUND_ID((MMI_IMAGE_ID_T)IMAGE_COMMON_BG),
    WIN_TITLE(TXT_NULL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),  
    CREATE_EDIT_DIGITAL_CTRL(3, MMIEBOOK_PERCENT_INPUT_CTRL_ID),
#endif  
    END_WIN
};

WINDOW_TABLE( MMIEBOOK_SET_CODING_TYPE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetCodingType ),    
    WIN_ID( MMIEBOOK_SET_CODING_TYPE_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif    
    WIN_TITLE( TXT_CODING_TYPE ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIEBOOK_SET_CODINGTYPE_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else        
     WIN_SOFTKEY(TXT_NULL, TXT_NULL,STXT_CANCEL),
#endif    
    END_WIN
};

//ebook show option menu
WINDOW_TABLE(MMIEBOOK_TXT_SHOW_OPT_WIN_TAB) = 
{
    WIN_FUNC( (uint32)HandleTxtShowOptWinMsg),
	WIN_ID(MMIEBOOK_TXT_SHOW_OPT_WIN_ID),
#ifdef GUI_FULLSCREEN_POPMENU_SUPPORT
    WIN_TITLE( TXT_NULL),
#endif
#ifdef MMI_PDA_SUPPORT
    WIN_HIDE_STATUS,
#endif
    WIN_STYLE( WS_HAS_TRANSPARENT),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
#endif    
	END_WIN
};


//ebook show option menu

WINDOW_TABLE( MMIEBOOK_BOOKMARK_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleBookmarkListOptWinMsg),
    WIN_ID(MMIEBOOK_BOOKMARK_LIST_OPT_WIN_ID),
#ifdef MMI_PDA_SUPPORT
    WIN_HIDE_STATUS,
#endif
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_EBOOK_BOOKMARK_LIST_OPT,MMIEBOOK_BOOKMARK_LIST_OPT_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};


#ifdef MMI_PDA_SUPPORT
    WINDOW_TABLE( MMIEBOOK_BOOKMARK_LIST_OPT_POPUP_WIN_TAB ) = 
    {
        WIN_FUNC( (uint32)HandleBookmarkListOptPopupWinMsg),
        WIN_ID(MMIEBOOK_BOOKMARK_LIST_OPT_POPUP_WIN_ID),
        WIN_HIDE_STATUS,
        WIN_STYLE( WS_HAS_TRANSPARENT),
        //WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
        //WIN_SOFTKEY(TXT_NULL,STXT_RETURN,TXT_NULL),
        END_WIN
    };
#endif

#ifdef TTS_SUPPORT
WINDOW_TABLE( MMIEBOOK_SET_TTS_OPEN_CLOSE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEbookSetTTSOpenCloseWindow ),    
    WIN_ID( MMIEBOOK_SET_TTS_OPEN_CLOSE_WIN_ID ),
    WIN_TITLE( TXT_SET_TTS_AUTO_READ ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIEBOOK_SET_TTS_CTRL_ID),
#ifdef MMI_PDA_SUPPORT 
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#ifdef EBOOK_TTS_SELF_VOLUME  
WINDOW_TABLE(MMIEBOOK_SET_TTS_VOL_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleEbookTTSVolSetWindow),
    WIN_ID(MMIEBOOK_SET_TTS_VOL_WIN_ID),
    WIN_TITLE(TXT_SET_TTS_READ_VOL),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif 
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif
#endif

WINDOW_TABLE( MMIEBOOK_SET_AUTO_READ_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleSetAutoRead ),    
    WIN_ID( MMIEBOOK_SET_AUTO_READ_WIN_ID ),
    WIN_TITLE( TXT_EBOOK_AUTO_READ ),
    
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif    
    
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIEBOOK_SET_AUTO_READ_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(TXT_NULL, TXT_NULL,STXT_CANCEL),
#endif
    
    END_WIN
};
 
#ifndef MMIEBOOK_OSD_DISPLAY_SETTING
WINDOW_TABLE( MMIEBOOK_FONT_SIZE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleFontSizeWinMsg ),    
    WIN_ID( MMIEBOOK_FONT_SIZE_WIN_ID ),
    WIN_TITLE( TXT_COMMON_FONT_SIZE ),
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),   
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIEBOOK_SET_FONT_SIZE_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    
    END_WIN
};

WINDOW_TABLE(MMIEBOOK_SET_FONT_COLOR_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleSetFontColor), 
    WIN_ID(MMIEBOOK_SET_FONT_COLOR_WIN_ID),    
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),   
    WIN_TITLE(TXT_COMMON_FONT_COLOR),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIEBOOK_FORM_FONT_COLOR_CTRL_ID),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL,MMIEBOOK_FORM_FONT_COLOR_CTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,NULL,MMIEBOOK_BUTTON_COLOR1_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL),
            CHILD_BUTTON_CTRL(FALSE,NULL,MMIEBOOK_BUTTON_COLOR2_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL),
            CHILD_BUTTON_CTRL(FALSE,NULL,MMIEBOOK_BUTTON_COLOR3_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2,MMIEBOOK_FORM_FONT_COLOR_CTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,NULL,MMIEBOOK_BUTTON_COLOR4_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2),
            CHILD_BUTTON_CTRL(FALSE,NULL,MMIEBOOK_BUTTON_COLOR5_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2),
            CHILD_BUTTON_CTRL(FALSE,NULL,MMIEBOOK_BUTTON_COLOR6_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2),
   
    END_WIN
}; 

WINDOW_TABLE(MMIEBOOK_SET_BG_COLOR_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleSetBGColor), 
    WIN_ID(MMIEBOOK_SET_BG_COLOR_WIN_ID),    
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),   
    WIN_TITLE(TXT_EBOOK_BACK_COLOR),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIEBOOK_FORM_BACK_COLOR_CTRL_ID),
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL,MMIEBOOK_FORM_BACK_COLOR_CTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_EBOOK_COLOR_BGICON1,MMIEBOOK_BUTTON_COLOR1_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_EBOOK_COLOR_BGICON2,MMIEBOOK_BUTTON_COLOR2_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_EBOOK_COLOR_BGICON3,MMIEBOOK_BUTTON_COLOR3_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL),
#ifndef MMI_LOW_MEMORY_EBOOK
        CHILD_FORM_CTRL(FALSE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2,MMIEBOOK_FORM_BACK_COLOR_CTRL_ID),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_EBOOK_COLOR_BGICON4,MMIEBOOK_BUTTON_COLOR4_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_EBOOK_COLOR_BGICON5,MMIEBOOK_BUTTON_COLOR5_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2),
            CHILD_BUTTON_CTRL(FALSE,IMAGE_EBOOK_COLOR_BGICON6,MMIEBOOK_BUTTON_COLOR6_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2),
#endif   
    END_WIN
};
#endif

#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
WINDOW_TABLE(MMIEBOOK_DISPLAY_SET_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32) HandleDisplaySettingWinMsg), 
    WIN_ID(MMIEBOOK_DISPLAY_SET_WIN_ID),    
    WIN_TITLE(TXT_DISPLAY_SET),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),        
#ifndef MMI_LOW_MEMORY_EBOOK         
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIEBOOK_FONT_COLOR_LABEL_CTRL_ID, MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIEBOOK_FORM_FONT_COLOR_CTRL_ID,MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL,MMIEBOOK_FORM_FONT_COLOR_CTRL_ID),
                CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_COLOR1_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL),
                CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_COLOR2_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL),
                CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_COLOR3_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL),
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2,MMIEBOOK_FORM_FONT_COLOR_CTRL_ID),
                CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_COLOR4_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2),
                CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_COLOR5_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2),
                CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_COLOR6_FONT_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2),
#endif 
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIEBOOK_BACK_COLOR_LABEL_CTRL_ID, MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIEBOOK_FORM_BACK_COLOR_CTRL_ID,MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL,MMIEBOOK_FORM_BACK_COLOR_CTRL_ID),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_EBOOK_COLOR_BGICON1,MMIEBOOK_BUTTON_COLOR1_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_EBOOK_COLOR_BGICON2,MMIEBOOK_BUTTON_COLOR2_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_EBOOK_COLOR_BGICON3,MMIEBOOK_BUTTON_COLOR3_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL),
#ifndef MMI_LOW_MEMORY_EBOOK
            CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2,MMIEBOOK_FORM_BACK_COLOR_CTRL_ID),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_EBOOK_COLOR_BGICON4,MMIEBOOK_BUTTON_COLOR4_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_EBOOK_COLOR_BGICON5,MMIEBOOK_BUTTON_COLOR5_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2),
                CHILD_BUTTON_CTRL(TRUE,IMAGE_EBOOK_COLOR_BGICON6,MMIEBOOK_BUTTON_COLOR6_BACK_CTRL,MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2),
#endif
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIEBOOK_FONT_SIZE_LABEL_CTRL_ID, MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL,MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),
            CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_FONT_SIZE1_CTRL,MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL),
            CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_FONT_SIZE2_CTRL,MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL),
#if !defined( MMI_GUI_STYLE_MINISCREEN)  
            CHILD_BUTTON_CTRL(TRUE,NULL,MMIEBOOK_BUTTON_FONT_SIZE3_CTRL,MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL),
#endif
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIEBOOK_PREVIEW_COLOR_LABEL_CTRL_ID, MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),
#if !defined( MMI_GUI_STYLE_MINISCREEN) 
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIEBOOK_PREVIEW_DISP_LABEL_CTRL_ID, MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),       
#else
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, TRUE, MMIEBOOK_PREVIEW_DISP_LABEL_CTRL_ID, MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID),       
#endif    
    END_WIN
}; 
#endif

WINDOW_TABLE( MMIEBOOK_DAY_NIGHT_MODE_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleDayNightModeWinMsg ),    
    WIN_ID( MMIEBOOK_DAY_NIGHT_MODE_WIN_ID ),
#ifdef MMI_PDA_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif    
    WIN_TITLE( TXT_EBOOK_DAY_NIGHT_MODE ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIEBOOK_DAY_NIGHT_MODE_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#else        
     WIN_SOFTKEY(TXT_NULL, TXT_NULL,STXT_CANCEL),
#endif    
    END_WIN
};
/*---------------------------------------------------------------------------*/
/*                          PUBLIC FUNCTION DEFINE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 电子书模块初始化
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_Init(void)
{
    MMIAPIEBOOK_CreateAllDevEbookDir();
}

//@maggie add begin for ebook
/*****************************************************************************/
//  FUNCTION:      MMIAPIEBOOK_InitModule
//  Description :   
//  Global resource dependence : 
//  Author:        maggie.ren
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_InitModule(void)
{
    MMIEBOOK_RegMenuGroup();
    MMIEBOOK_RegNv(); 
    MMIEBOOK_RegWinIdNameArr();
#ifdef MMI_TASK_MANAGER
    MMIEBOOK_RegAppletInfo();
#endif
}
//@maggie add end   for ebook

/*****************************************************************************/
//  Description : 娱乐-电子书入口函数
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_ManagerWinMsg (BOOLEAN is_need_privacy_protection)
{
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        return;
    }

    //获取存储设备的情况
    MMIEBOOK_GetDevInfo();
    if(0 == g_ebook_all_dev_file_brief.dev_num)//没有存储设备的情况，直接提示"电子书空"
    {
        MMIPUB_OpenAlertWarningWin(TXT_EBOOK_TXT_EMPTY);  
        return;
    }
    
    EnterTxtorList(is_need_privacy_protection);//娱乐-电子书入口    
}

/*****************************************************************************/
//  Description : 电子书从我的文档进入的入口函数
//  Global resource dependence : 
//  Author: rongjin
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_ManagerWinMsg_4Fmm (uint8 file_dev, const wchar* name_ptr, uint32 name_length, uint32 file_size, const wchar* full_name_ptr, uint32 full_name_length)
{
#ifdef MMI_TASK_MANAGER
    BOOLEAN is_open=FALSE;
#endif
    if(PNULL == name_ptr || 0 == name_length 
        || PNULL == full_name_ptr || 0 == full_name_length)
    {
        //SCI_TRACE_LOW:"MMIEBOOK_WINTAB MMIAPIEBOOK_ManagerWinMsg_4Fmm file_dev = %d, name_ptr = %x, name_length = %d, file_size= %d, full_name_ptr = %x, full_name_length =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_1343_112_2_18_2_15_27_69,(uint8*)"dddddd",file_dev,  name_ptr, name_length, file_size, full_name_ptr, full_name_length );

        return ;
    }
#ifdef MMI_TASK_MANAGER
    if(MMK_IsOpenWin(MMIEBOOK_LATEST_LIST_WIN_ID))
    {
        is_open=TRUE;
    }
    MMIAPIEBOOK_StopApplet(FALSE);
#endif
    if(FALSE == MMIEBOOK_InitFileBrief(file_dev,name_ptr,name_length,file_size,full_name_ptr,full_name_length))
    {
        return ;
    }

#ifdef MMI_TASK_MANAGER
    if(is_open)
    {
        //ADD_DATA param=0;/*lint !e64*/
       
        MMI_APPLET_START_T start ={0};
        
        //SCI_TRACE_LOW:"EnterTxtorList"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_1366_112_2_18_2_15_27_70,(uint8*)"");
        start.guid = SPRD_EBOOK_APPLET_ID;
        start.state = MMI_APPLET_STATE_BG_HIDE_WIN|MMI_APPLET_STATE_TASK_MANAGER;
        start.param_ptr = 0;
        start.size_of_param= sizeof(uint32);

        MMK_StartApplet( &start );
        MMK_CreateEbookWin((uint32*)MMIEBOOK_ENTER_TXT_CONTENT_WIN_TAB, PNULL);
    }
    else
    {
       MMK_CreateWin((uint32*)MMIEBOOK_ENTER_TXT_CONTENT_WIN_TAB, PNULL);//资源管理器中入口 
    }
#else
    MMK_CreateWin((uint32*)MMIEBOOK_ENTER_TXT_CONTENT_WIN_TAB, PNULL);//资源管理器中入口
#endif
}
//add end 

/*****************************************************************************/
//  Description : Create ebook reference window for MMI_APPLET_STATE_BG_HIDE_WIN
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
#ifdef MMI_TASK_MANAGER
PUBLIC MMI_HANDLE_T MMIEBOOK_OpenEbookWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  )
{
    MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};

    win_table_create.applet_handle = SPRD_EBOOK_APPLET_ID;
    win_table_create.win_table_ptr = win_table_ptr;
    win_table_create.add_data_ptr = add_data_ptr;
    
    return MMK_CreateWinTable(&win_table_create);
}
#endif
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DEFINE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 大电子书警告窗口处理
//  Global resource dependence : 
//   Author: zhaohui
//  Note:   
/*****************************************************************************/
/*LOCAL MMI_RESULT_E  HandleBigEbookAlertWinMsg(
                                              MMI_WIN_ID_T win_id,            
                                              MMI_MESSAGE_ID_E msg_id,        
                                              DPARAM param
                                              )
{
    static BOOLEAN s_is_open_big_book = FALSE;

    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    
    switch (msg_id)
    {
    case MSG_TIMER:
        if (MMIPUB_GetAlertTimerId(win_id) == *(uint8*)param)
        {
            s_is_open_big_book = TRUE;
            MMK_CreateEbookWin((uint32 *)MMIEBOOK_TXT_SHOW_WIN_TAB, PNULL);
        }
        else
        {
            MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        s_is_open_big_book = TRUE;
        MMK_CreateEbookWin((uint32 *)MMIEBOOK_TXT_SHOW_WIN_TAB, PNULL);
        break;
       
    case MSG_CLOSE_WINDOW:
        if (s_is_open_big_book)
        {
            s_is_open_big_book = FALSE;
        }
        else
        {
            MMK_CloseWin(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID); 
        }
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    case MSG_LOSE_FOCUS:
	 	MMK_StopTimer(MMIPUB_GetAlertTimerId(win_id));
		win_info_ptr->timer_id=0;
	 	break;
     case MSG_GET_FOCUS:
        if ((0 == win_info_ptr->timer_id) &&
            (0 != win_info_ptr->time_period))
        {
            win_info_ptr->timer_id = MMK_CreateTimer(win_info_ptr->time_period,FALSE);
        }
		break;           
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }    
    
    return (result);
}*/

/*****************************************************************************/
//  Description : enter txt content show window
//  Global resource dependence : g_textfile_all_info
//   Author: rongjin
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnterTxtContentWinMsg(
                                                    MMI_WIN_ID_T win_id,     
                                                    MMI_MESSAGE_ID_E msg_id,     
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMIEBOOK_FILE_BRIEF_T* s_ebook_file_brief = MMIEBOOK_GetFileBrief();
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:    
            //load电子书简略信息,分配资源等
            {                
                MMIAPIEBOOK_CreateAllDevEbookDir();
                //MMIEBOOK_SetTxtBrief(); put it into function MMIEBOOK_InitFileBrief
                //MMIEBOOK_GetAllDevTxtBrief();
                //MMIEBOOK_AllocEbookFileRes();
                //任务跟窗口的有共享变量，所以在此分配资源
                if(!MMIEBOOK_AllocEbookTaskRes())
                {
                    //out of memory                     
                    MMIPUB_OpenAlertWinByTextId(NULL,TXT_EMPTY_FILE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    MMK_CloseWin(win_id);
                }
                else
                {
                    EnterTxtContent(s_ebook_file_brief,MMIEBOOK_MSG_LOAD_TXT_BRIEF);
                }
            }
            break;
           
        case MMIEBOOK_MSG_CREATE_DETAIL_FILE_FAIL:                  //初始化详情文件失败       
        case MMIEBOOK_MSG_CREATE_DETAIL_FILE:                       //初始化详情文件任务完成    
			EnterTxtContent(s_ebook_file_brief,msg_id);
            break;
          
       /* case MSG_KEYUP_RED://@zhaohui,cr108136
        case MSG_KEYDOWN_RED:  
            //SCI_TRACE_LOW:"HandleEnterTxtContentWinMsg: MSG_KEYDOWN_RED "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_1514_112_2_18_2_15_27_71,(uint8*)"");
            break;*/  
      
        case MSG_CLOSE_WINDOW:
            //MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_TASK_EXIT,  0); //not useful now
            MMIEBOOK_CloseShowFile();
            MMIEBOOK_FreeEbookTaskRes();
            MMIEBOOK_FreeEbookFileRes();
            break;
      
        default:
            result = MMI_RESULT_FALSE;
            break;
        }
        
        return (result);
}

/*****************************************************************************/
//  Description : EnterTxtContent
//  Global resource dependence : 
//   Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void EnterTxtContent(MMIEBOOK_FILE_BRIEF_T* ebook_file_brief,MMI_MESSAGE_ID_E msg_id)
{
    //SCI_ASSERT(PNULL != ebook_file_brief);/*assert verified*/
    if(PNULL == ebook_file_brief)
    {
        return;
    }           

    //SCI_TRACE_LOW:"mmiebook_wintab.c EnterTxtContent msg_id is %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_1553_112_2_18_2_15_27_73,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MMIEBOOK_MSG_LOAD_TXT_BRIEF:
    case MMIEBOOK_MSG_CREATE_DETAIL_FILE://初始化详情文件任务完成
        if(0 == ebook_file_brief->file_size)
        {
            CloseEbookOpenWaitWin();
            // show "empty file" message box
            MMIPUB_OpenAlertWinByTextId(NULL,TXT_EMPTY_FILE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID);

        }
        else if(MMIEBOOK_MAX_FILE_SIZE <=ebook_file_brief->file_size)
        {
            CloseEbookOpenWaitWin();
            MMIPUB_OpenAlertWinByTextId(NULL,TXT_EBOOK_FILE_SIZE_ABOVE_5M,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID);
        }
        else
        {
#ifdef MMI_TASK_MANAGER
            if(MMIEBOOK_GetAppletInstance()!=PNULL)
#endif
            {
                MMK_CreateEbookWin((uint32 *)MMIEBOOK_TXT_SHOW_WIN_TAB, PNULL);
            }
#ifdef MMI_TASK_MANAGER
            else
            {
                MMK_CreateWin((uint32 *)MMIEBOOK_TXT_SHOW_WIN_TAB, PNULL);
            }
#endif
        }
        break;

    case MMIEBOOK_MSG_CREATE_DETAIL_FILE_FAIL://初始化详情文件失败
        CloseEbookOpenWaitWin();
            
        //提示U 盘空间不够
        MMIPUB_OpenAlertWarningWin(TXT_EBOOK_MEM_NO_SPACE);
#ifdef MMI_TASK_MANAGER
        if(MMIEBOOK_GetAppletInstance()!=PNULL)
#endif
        {
            MMK_CreateEbookWin((uint32 *)MMIEBOOK_TXT_SHOW_WIN_TAB, PNULL);
        }
#ifdef MMI_TASK_MANAGER
        else
        {
            MMK_CreateWin((uint32 *)MMIEBOOK_TXT_SHOW_WIN_TAB, PNULL);
        }
#endif
        break;

    default:
        //SCI_TRACE_LOW:"ebook EnterTxtContent msg_id is %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_1609_112_2_18_2_15_27_74,(uint8*)"d",msg_id);
        break;
    }
}

/*****************************************************************************/
//  Description : 文本文件阅读窗口中设置横竖屏模式下的各控件的区域
//  Global resource dependence : 
//  Author:zhaohui
//  Note: 
/*****************************************************************************/
LOCAL void SetTxtShowCtrlRect(void)
{
    GUI_BOTH_RECT_T     both_rect = {0};
    GUI_BOTH_RECT_T     both_client_rect = MMITHEME_GetWinClientBothRect(MMIEBOOK_TXT_SHOW_WIN_ID);
    GUI_BOTH_RECT_T     both_fullscreen_rect = MMITHEME_GetFullScreenBothRect();

    if(s_is_full_screen_off)
    {
#ifdef MMIEBOOK_OSDMENU_SUPPORT  
        both_rect.h_rect.left = both_rect.v_rect.left = 0;
        both_rect.h_rect.top = both_rect.v_rect.top = 0;
        both_rect.h_rect.bottom = (both_client_rect.h_rect.bottom - MMIEBOOK_COUNT_BAR_HEIGHT);
        both_rect.h_rect.right = both_client_rect.h_rect.right;
        both_rect.v_rect.bottom = (both_client_rect.v_rect.bottom - MMIEBOOK_COUNT_BAR_HEIGHT);
        both_rect.v_rect.right = both_client_rect.v_rect.right;
        GUIAPICTRL_SetBothRect(MMIEBOOK_TXT_SHOW_CTRL_ID,&both_rect);//set textbox rect 
#else
        GUIAPICTRL_SetBothRect(MMIEBOOK_TXT_SHOW_CTRL_ID,&both_client_rect);
#endif
    }
    else
    {
        both_rect.h_rect.left = both_client_rect.v_rect.left = 0;
        both_rect.h_rect.top = both_client_rect.v_rect.top = 0;
        both_rect.h_rect.bottom = both_fullscreen_rect.h_rect.bottom ;
        both_rect.h_rect.right = both_fullscreen_rect.h_rect.right;
        both_rect.v_rect.bottom = both_fullscreen_rect.v_rect.bottom;
        both_rect.v_rect.right = both_fullscreen_rect.v_rect.right;
        GUIAPICTRL_SetBothRect(MMIEBOOK_TXT_SHOW_CTRL_ID,&both_rect);//set textbox rect 
    }
}

/*****************************************************************************/
//  Description : 文本文件阅读窗口主函数
//  Global resource dependence : g_textfile_all_info,g_mmitext_file_dev
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTxtShowWinMsg(
                                        MMI_WIN_ID_T win_id,            
                                        MMI_MESSAGE_ID_E msg_id,        
                                        DPARAM param
                                        )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMIEBOOK_FILE_BRIEF_T* cur_file_brief=PNULL;
    GUI_BG_T txtbox_bg = {0};
#ifdef TOUCH_PANEL_SUPPORT
#ifndef WIN32
    static uint16 s_tp_ioctl=0;
#endif
#endif
#ifdef TTS_SUPPORT
    static uint32       s_cur_playing_page   = 0;
    static uint32      s_is_promt_no_tts_resource_flag = 0;
    static MMISRV_HANDLE_T  s_tts_virtual_handle=0;
#endif
    static BOOLEAN s_is_auto_dis_osd=FALSE;


    //SCI_TRACE_LOW:"MMIEBOOK_WINTAB HandleTxtShowWinMsg msg_id is %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_1665_112_2_18_2_15_28_75,(uint8*)"d",msg_id);
    switch( msg_id )
    {    		
    case MSG_FULL_PAINT:
        break;
        
    case MSG_GET_FOCUS:
        //恢复背光灯状态
        if (s_is_backlight_turn_off)
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        }
        else
        {
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }	
        //记录背光亮度
      //  s_contrast = MMIAPISET_GetCurrentContrast();   //bug161429        
        
        //设置背光亮度
        SetLcdContrast();        
#ifndef  MMIEBOOK_OSDMENU_SUPPORT
        if(s_is_full_screen_off)
        {
            CreateTitleMenu(win_id);
        }
        else
        {
             DestroyTitleMenu();
        }
#endif
        
#ifdef TTS_SUPPORT
        /* Bug fix: cr00202600 and cr00202627. If Font size changed or LCD rotate, 
        we should read the current page from head */
        if(s_tts_auto_pagedown_flag)
        {
            if(!MMIAPITTS_IsPlayRing(TTS_RING_TYPE_EBOOK))
            {
                MMK_SendMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_TTS_PLAY,PNULL);
            }
        }
        else
#endif
        {
            if(MMIEBOOK_AUTO_READ_OFF!=MMIAPIEBOOK_GetAutoReadType())
            {
                EbookStartTimer(&s_auto_read_timer_id, GetAutoReadTime(), FALSE);
            }
        }
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
        ReleaseAllTurnPageLayer();
        CreateAllTurnPageLayer(win_id);
#endif
#ifdef TOUCH_PANEL_SUPPORT 
#ifndef WIN32
        {
            uint16 size=25;
            TP_Ioctl(TP_IOCTL_SET_THRES_ZOOM, &size);
        }
#endif
#endif
        //CreateOSDMenu(win_id);
        break;
		
    case MSG_LOSE_FOCUS:
        //允许背光关闭
        MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
        //恢复背光亮度
        //RestoreLcdContrast();
#ifdef TTS_SUPPORT
        MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
        freeVirtuaLAudioHandle(&s_tts_virtual_handle);
#endif

        EbookCloseTimer(&s_auto_read_timer_id);
        if(!is_disenable_txt_func)
        {
            WriteEbookBookMarkInfo(FALSE);
        }
#ifdef MMIEBOOK_OSDMENU_SUPPORT
        DestroyOSDMenu();
#endif
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
        ExitSlideEbook(&s_slide_ebook, FALSE,TRUE);
        ReleaseAllTurnPageLayer();
#endif
#ifdef TOUCH_PANEL_SUPPORT 
#ifndef WIN32
        TP_Ioctl(TP_IOCTL_SET_THRES_ZOOM, &s_tp_ioctl);
#endif
#endif
        break;
        
    case MSG_OPEN_WINDOW:
#if defined( MMI_GUI_STYLE_MINISCREEN) 
        s_is_has_read = TRUE;//设置已经打开电子书，按红键并没有退出电子书
#endif
#ifndef MMIEBOOK_OSDMENU_SUPPORT
		SetIsFullScreenOff(FALSE);
#endif
        SCI_TRACE_LOW("ebook_time:HandleTxtShowWinMsg MSG_OPEN_WINDOW start=%d",SCI_GetTickCount());
        cur_file_brief= MMIEBOOK_GetFileBrief();
        if(!MMIEBOOK_isEbookTypeFile(cur_file_brief->full_name_wstr, cur_file_brief->full_name_len))
        {
           is_disenable_txt_func=TRUE;
        }
        else
        {
             is_disenable_txt_func=FALSE;

        }
#ifdef TTS_SUPPORT
        s_is_promt_no_tts_resource_flag = 0;
        if(!is_disenable_txt_func)
        {
            s_tts_auto_pagedown_flag = MMIEBOOK_GetTTSInfo();
        }
        else
        {
            s_tts_auto_pagedown_flag = FALSE;
        }
#endif
        //禁止背光灯关闭
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        s_is_backlight_turn_off = FALSE;
        //记录背光亮度
        s_contrast = MMIAPISET_GetCurrentContrast(); //用于记录进入ebook前的背光亮度，不允许被修改。
        /*{      
          MMI_STRING_T wait_text = {0};
          MMI_GetLabelTextByLang(TXT_EBOOK_OPENING_WAITING, &wait_text);
       
          MMIPUB_OpenWaitWin(1,  &wait_text, PNULL, PNULL,
                           MMIEBOOK_OPEN_WAIT_WIN_ID, IMAGE_NULL, 
                           ANIM_PUBWIN_WAIT,  WIN_ONE_LEVEL,  MMIPUB_SOFTKEY_NONE,  PNULL);
        }*/
        MMIAPIEBOOK_InitIsFullScreenOff();
        MMIEBOOK_SetIsNeedCloseTimer(TRUE);
		
        //设置字体和背景颜色    
        MMIAPIEBOOK_GetFontType();

        //设置白天黑色模式 
        InitDayNightMode();

        //设置背光亮度
        if(MMK_IsFocusWin(win_id))
        {
            SetLcdContrast();  
        }
        
        SetTxtShowCtrlRect();
#ifdef MMIEBOOK_OSDMENU_SUPPORT
        if(!s_is_full_screen_off)
        {
            MMIEBOOK_SetIsTxtShowWinCtrlvisible(FALSE);
        }
#endif
        //no progress
        GUITEXT_IsDisplayPrg(FALSE,MMIEBOOK_TXT_SHOW_CTRL_ID);
        GUITEXT_IsSlide(MMK_GetCtrlHandleByWin(win_id,MMIEBOOK_TXT_SHOW_CTRL_ID),FALSE);
#ifndef MMIEBOOK_COPY_SUPPORT
        GUITEXT_SetHandleTpMsg(FALSE,MMIEBOOK_TXT_SHOW_CTRL_ID);
        GUIAPICTRL_SetState(MMIEBOOK_TXT_SHOW_CTRL_ID, GUICTRL_STATE_DISABLE_ACTIVE,TRUE);
#endif
#ifndef MMI_LOW_MEMORY_EBOOK            
        if(s_ebook_bg_id<=MMIEBOOK_BG_3)
        {
            txtbox_bg.bg_type = GUI_BG_COLOR;
            txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
        }
        else
        {
            txtbox_bg.bg_type = GUI_BG_IMG;
            txtbox_bg.img_id=s_ebook_back_img[s_ebook_bg_id-MMIEBOOK_BG_4];/*lint !e656*/
        }
#else
        {
            txtbox_bg.bg_type = GUI_BG_COLOR;
            txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
        }
#endif
        GUITEXT_SetBg(MMIEBOOK_TXT_SHOW_CTRL_ID,&txtbox_bg);

        GUITEXT_SetFont(MMIEBOOK_TXT_SHOW_CTRL_ID,PNULL,&s_ebook_color[s_ebook_fontcolor_id]);/*lint !e605*/
        //解决有时候进去出现短暂乱码问题
        {
            wchar space[2]={0x20,0};
            GUITEXT_SetString(MMIEBOOK_TXT_SHOW_CTRL_ID, space,1,FALSE);
        }
        
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
        CreateAllTurnPageLayer(win_id);
#endif
        MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_SHOW_TXT_WIN_OPEN,  g_ebook_task_id,  0 );
        {
            /*GUI_RECT_T  screen_rect = MMITHEME_GetFullScreenRect();
            MMI_MULTI_KEY_TP_MSG_PARAM_T para = {0};
            para.cur_tp_point.x  = screen_rect.right /2 ;            
            para.cur_tp_point.y = screen_rect.bottom /2;      
            MMK_PostMsg( win_id,  MSG_TP_PRESS_DOWN,  &para,  sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T));
            MMK_PostMsg( win_id,  MSG_TP_PRESS_UP,  &para, sizeof(MMI_MULTI_KEY_TP_MSG_PARAM_T) );*/
            //CreateOSDMenu(win_id);
            s_is_auto_dis_osd=FALSE;
            //if(0 == s_osd_display_timer_id)
            //{
            //    s_osd_display_timer_id = MMK_CreateWinTimer( win_id, EBOOK_OSD_MENU_DISPLAY_TIME,FALSE);
            //}
        }
        SCI_TRACE_LOW("ebook_time:HandleTxtShowWinMsg MSG_OPEN_WINDOW end=%d",SCI_GetTickCount());
#ifdef TOUCH_PANEL_SUPPORT
        s_pre_tp_msg=MSG_TP_PRESS_DOWN;
        s_gesture_num_out=0;
        s_gesture_num_in=0;
#ifndef WIN32
        {
            uint16 size=25;
            TP_Ioctl(TP_IOCTL_GET_THRES_ZOOM, &s_tp_ioctl);
            TP_Ioctl(TP_IOCTL_SET_THRES_ZOOM, &size);
        }
#endif
#endif
        break;

    case MSG_LCD_SWITCH: 
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
        ExitSlideEbook(&s_slide_ebook, FALSE,FALSE);
        ReleaseAllTurnPageLayer();
        CreateAllTurnPageLayer(win_id);
#endif
		HandleLCDSwitchMsg(win_id);
#ifdef  MMIEBOOK_OSDMENU_SUPPORT
        if(s_osd_is_active)
        {
            CreateOSDMenu(win_id);
        }
#endif
        break;


#if 0    
    case MSG_APP_UPSIDE:
#ifdef TTS_SUPPORT
        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_EBOOK))
        {
            uint8 cur_vol = MMISMS_GetTTSVol();
            if(MMISET_VOL_MAX > cur_vol)
            {
                MMISMS_SetTTSVol(cur_vol + 1);
                MMIAPISET_InitAudioDevice(cur_vol + 1);
            }
        }
#endif
            break;
#endif

        //lint -fallthrough
    case MSG_APP_UP:        
    case MSG_APP_LEFT:
#ifdef TTS_SUPPORT
        if(MMIEBOOK_IsTurnLineOrPage(msg_id))
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
        }
#endif
        if (MSG_APP_UP == msg_id)
        {
            //向上翻行
            MMIEBOOK_TurnLineOrPage(MMIEBOOK_LINE_UP);
        }
        else
        {
            //向上翻页
            MMIEBOOK_TurnLineOrPage(MMIEBOOK_PAGE_UP);
        }
        if(MMIEBOOK_GetIsNeedCloseTimer())
        {
            EbookCloseTimer(&s_auto_read_timer_id);
        }
        else
        {
            MMIEBOOK_SetIsNeedCloseTimer(TRUE);
        }
#ifdef  MMIEBOOK_OSDMENU_SUPPORT
        if(s_osd_is_active)
        {
            SetOSDCtrlParam();
			if(s_osd_display_timer_id!=0)
			{
				MMK_StopTimer(s_osd_display_timer_id);
				//s_osd_display_timer_id = 0;
				s_osd_display_timer_id = MMK_CreateWinTimer( win_id, EBOOK_OSD_MENU_DISPLAY_TIME,FALSE);
			}
        }
#endif
        break;

#if 0        
    case MSG_APP_DOWNSIDE://向下翻页
#ifdef TTS_SUPPORT
        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_EBOOK))
        {
            uint8 cur_vol = MMISMS_GetTTSVol();
            if(MMISET_VOL_ZERO < cur_vol)
            {
                MMISMS_SetTTSVol(cur_vol - 1);
                MMIAPISET_InitAudioDevice(cur_vol - 1);
            }
        }       
#endif
	 break;
#endif

        //lint -fallthrough
    case MSG_APP_DOWN:
    case MSG_APP_RIGHT:
#ifdef TTS_SUPPORT
        if(MMIEBOOK_IsTurnLineOrPage(msg_id))
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
        }
#endif
        EbookCloseTimer(&s_auto_read_timer_id);
        if (MSG_APP_DOWN == msg_id)
        {
            //向下翻行
            MMIEBOOK_TurnLineOrPage(MMIEBOOK_LINE_DOWN);
        }
        else
        {
            //向下翻页
            MMIEBOOK_TurnLineOrPage(MMIEBOOK_PAGE_DOWN);
        }
#ifdef  MMIEBOOK_OSDMENU_SUPPORT
        if(s_osd_is_active)
        {
            SetOSDCtrlParam();
			if(s_osd_display_timer_id!=0)
			{
				MMK_StopTimer(s_osd_display_timer_id);
				//s_osd_display_timer_id = 0;
				s_osd_display_timer_id = MMK_CreateWinTimer( win_id, EBOOK_OSD_MENU_DISPLAY_TIME,FALSE);
			}
        }
#endif
        break;
        
    case MSG_TIMER:
        if (s_auto_read_timer_id ==  *( GUI_TIMER_ID_T*)param)
        {
            EbookCloseTimer(&s_auto_read_timer_id);			
#ifdef TTS_SUPPORT
            if((!s_tts_auto_pagedown_flag))
#endif
            {
                MMK_SendMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MSG_APP_RIGHT,PNULL);		
            }
        }
        else if(s_osd_display_timer_id == *( GUI_TIMER_ID_T*)param)
        {
#ifdef MMIEBOOK_OSDMENU_SUPPORT
            DestroyOSDMenu();
#endif
        }

#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
        else
        {
            HandleSlideEbookTimer(&s_slide_ebook,param);
        }
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
#ifndef MMIEBOOK_COPY_SUPPORT
        {
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            HandleEbookSlideTpDown(&point,&s_slide_ebook,win_id);
#endif
        }
#endif
        s_pre_tp_msg=MSG_TP_PRESS_DOWN;
         s_gesture_num_out=0;
        s_gesture_num_in=0;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:
        {
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
            GUI_POINT_T point = {0};
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
            HandleEbookSlideTpMove(&point,&s_slide_ebook);
#endif
        }
        s_pre_tp_msg=MSG_TP_PRESS_MOVE;
         s_gesture_num_out=0;
        s_gesture_num_in=0;
        break;
#endif      

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_GESTURE:
        {  
            MMIEBOOK_FONT_SIZE_E font_size = MMIEBOOK_FONT_NORMAL;
            s_pre_tp_msg=MSG_TP_GESTURE;
            SCI_TRACE_LOW("MMK_GET_TP_GESTURE param=%d",MMK_GET_TP_GESTURE(param));
            if (MMI_GESTURE_ZOOM_IN == MMK_GET_TP_GESTURE(param))
            {
                s_gesture_num_in++;
                if(s_gesture_num_in>=2)
                {
                    font_size = MMIAPIEBOOK_GetFontSize( );
                    s_gesture_num_out=0;
                    s_gesture_num_in=0;
                    SCI_TRACE_LOW("s_gesture_num_out-s_gesture_num_in=%d-%d",s_gesture_num_out,s_gesture_num_in);
                    if(font_size <= MMIEBOOK_FONT_BIG)
                    {
                        break;
                    }
                    font_size--;
                    SetEbookFontSize(font_size);/*lint !e64*/
                    MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_FONT_SIZE_CHANGE, g_ebook_task_id, PNULL);
                    }
                else
                {
                    //s_gesture_num_out=0;
                }

            }
            else if (MMI_GESTURE_ZOOM_OUT == MMK_GET_TP_GESTURE(param))
            {
                 s_gesture_num_out++;
                 if(s_gesture_num_out>=2)
                 {
                    font_size = MMIAPIEBOOK_GetFontSize( );
                    s_gesture_num_out=0;
                    s_gesture_num_in=0;
                    SCI_TRACE_LOW("s_gesture_num_in-s_gesture_num_out=%d-%d",s_gesture_num_in,s_gesture_num_out);
                     if(font_size >= MMIEBOOK_FONT_SMALL)
                    {
                        break;
                    }
                    font_size++;
                    SetEbookFontSize(font_size);/*lint !e64*/
                    MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_FONT_SIZE_CHANGE, g_ebook_task_id, PNULL);
                }
                 else
                 {
                     //s_gesture_num_in=0;
                 }
            } 
            
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        HandleShowTpUpMsg(win_id,param);
        result = MMI_RESULT_FALSE;
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
#if defined( MMI_GUI_STYLE_MINISCREEN)  
        s_is_has_read = FALSE;//设置已经退出电子书，下次进入电子书打开资源管理器
#endif
        MMK_CloseWin(win_id);
#ifndef MMI_PDA_SUPPORT
        //OpenFmmWinFromShowWin(); 
#endif
        break;
        
    case MSG_CLOSE_WINDOW:
        {
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
            ReleaseAllTurnPageLayer();
#endif
            //恢复背光亮度
            RestoreLcdContrast();
            //允许背光关闭
            MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
#ifdef TTS_SUPPORT
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
            freeVirtuaLAudioHandle(&s_tts_virtual_handle);
            s_tts_auto_pagedown_flag = FALSE;
            s_is_promt_no_tts_resource_flag = 0;
#endif
            EbookCloseTimer(&s_auto_read_timer_id);
            if(!is_disenable_txt_func)
            {
                WriteEbookBookMarkInfo(TRUE);
            }

            is_disenable_txt_func=FALSE;

            CloseEnterEbookWin();
            s_is_auto_dis_osd=FALSE;
#ifdef TOUCH_PANEL_SUPPORT
        s_pre_tp_msg=MSG_TP_PRESS_DOWN;
         s_gesture_num_out=0;
        s_gesture_num_in=0;
#ifndef WIN32
        {
            TP_Ioctl(TP_IOCTL_SET_THRES_ZOOM, &s_tp_ioctl);
            s_tp_ioctl=0;
        }
#endif
#endif
        }
        break;

    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
    case MSG_APP_HASH:  
    case MSG_APP_STAR:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
#ifdef  MMIEBOOK_OSDMENU_SUPPORT
        if(s_osd_is_active)
        {
            DestroyOSDMenu();
        }
        else
        {
            CreateOSDMenu(win_id);
        }
#else
        if(MMIEBOOK_GetIsFullScreenOff())
        {
            SetIsFullScreenOff(FALSE);

        }
        else
        {
            SetIsFullScreenOff(TRUE);
        }
        SetTxtBoxRect();
        MMIEBOOK_SendMsgtoTask( MMIEBOOK_MSG_FULL_SCREEN_SWITCH,  g_ebook_task_id,  0 );

#endif
        break;
 
#ifdef TTS_SUPPORT
    case MMIEBOOK_MSG_TTS_PLAY:
        {
            MMI_STRING_T    text_string ={0};  
            MMITTS_PLAY_PARAM_T play_param = {0};
            //SCI_TRACE_LOW:"MMIEBOOK_WINTAB HandleTxtShowWinMsg MMIAPITTS_PlayText()  MMIEBOOK_MSG_TTS_PLAY"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_2112_112_2_18_2_15_28_76,(uint8*)"");

            MMIEBOOK_GetShowTxt(&text_string);
            if(MMK_IsFocusWin(win_id))
            {
                if(MMIAPITTS_CheckResFile())
                {
                    play_param.txt_addr = (uint8 *)text_string.wstr_ptr;
                    play_param.txt_len_in_byte = text_string.wstr_len * 2;
                    play_param.is_ucs2 = TRUE;
                    play_param.play_times = 1;
#ifdef EBOOK_TTS_SELF_VOLUME
                     play_param.tts_vol=MMIEBOOK_GetTTSVol();
#else
                    play_param.tts_vol = MMIAPISET_GetMultimVolume();
                    play_param.volume_type=MMISRVAUD_VOLUME_TYPE_MED;
#endif
                    play_param.ring_type = TTS_RING_TYPE_EBOOK;
                    play_param.win_handle = MMIEBOOK_TXT_SHOW_WIN_ID;
                    play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
                    play_param.digit_mode = TTS_DIGIT_AUTO;
                    play_param.punc_mode = TTS_PUNC_OFF;
                    if(s_tts_virtual_handle==0)
                    {
                        s_tts_virtual_handle=MMISRVAUD_ReqVirtualHandle(EBOOK_AUDIO_SRV_NAME,MMISRVAUD_PRI_NORMAL);
                    }
                    if(MMIAPITTS_PlayText(&play_param))
                    {
                        s_tts_auto_pagedown_flag=TRUE;
                        s_cur_playing_page = 0;//g_ebook_show_file_detail_ptr->cur_page_num;测试修改
                        EbookCloseTimer(&s_auto_read_timer_id);
                    }
                    else
                    {
                        s_tts_auto_pagedown_flag=FALSE;
                    }
                }
                else
                {   
                    freeVirtuaLAudioHandle(&s_tts_virtual_handle);
                    //check if automatic read and tts
                    if(0 == s_is_promt_no_tts_resource_flag)
                    {
                        MMIPUB_OpenAlertWarningWin( TXT_TTS_NO_RESOURCE);                   
                        if(s_tts_auto_pagedown_flag)
                        {                    
                            s_is_promt_no_tts_resource_flag = 1;    //automatic tts
                        }
                    }
                    s_tts_auto_pagedown_flag=FALSE;
                }
                // do noting is play failed
            }           
        }
        break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {       
        MMI_CTRL_ID_T src_id = ((MMI_NOTIFY_T*)param)->src_id;
    
        if(!s_osd_is_active)
        {
            break;
        }
        switch(src_id)
        {
#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
        case MMIEBOOK_OSD_DISPLAY_SET_CTRL_ID:            
            MMK_CreateWin((uint32 *)MMIEBOOK_DISPLAY_SET_WIN_TAB, PNULL);
            break;
        case MMIEBOOK_OSD_MORE_CTRL_ID:
            MMK_SendMsg(win_id, MSG_APP_MENU, PNULL);
            break;
#else
        case MMIEBOOK_OSD_FONTCOLOR_CTRL_ID:
            MMK_CreatePubFormWin((uint32 *)MMIEBOOK_SET_FONT_COLOR_WIN_TAB, PNULL);
            break;
        case MMIEBOOK_OSD_FONTSIZE_CTRL_ID:
            MMK_CreatePubListWin((uint32 *)MMIEBOOK_FONT_SIZE_WIN_TAB, PNULL);
            break;
        case MMIEBOOK_OSD_BG_CTRL_ID:
            MMK_CreatePubFormWin((uint32 *)MMIEBOOK_SET_BG_COLOR_WIN_TAB, PNULL);
            break;
        case MMIEBOOK_OSD_MORE_CTRL_ID:
            MMK_SendMsg(win_id, MSG_APP_MENU, PNULL);
            break;
#endif
        default:
            break;
        }
#ifdef MMIEBOOK_OSDMENU_SUPPORT
        DestroyOSDMenu();   
#endif        
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        //if(is_disenable_txt_func && (!MMITHEME_IsIstyle()))
        //{
        //    break;
        //}
        EbookCloseTimer(&s_auto_read_timer_id);
        //电子书阅读选项
        //MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SHOW_WIN_OPT, g_ebook_task_id, 0);
        OpenEbookShowOptMenu();
        break;

#ifdef TTS_SUPPORT
    case MSG_TTS_OVER:  //ebook content to speech
        //SCI_TRACE_LOW:"MMIEBOOK_WINTAB HandleTxtShowWinMsg  MMITTS MSG_TTS_OVER MSG received, tts play state = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_2209_112_2_18_2_15_29_77,(uint8*)"d", MMIAPITTS_GetPlayStatus());
        // if(MMIAPITTS_GetPlayStatus())
        {
            if (MMIEBOOK_IsInLastPage())
            {
                MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
                freeVirtuaLAudioHandle(&s_tts_virtual_handle);
                s_tts_auto_pagedown_flag = FALSE;
            }
            else
            {
				if(MMK_IsFocusWin(win_id) && s_tts_auto_pagedown_flag)
				{
					MMK_PostMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MSG_APP_RIGHT,PNULL,0);
				}
            }
        }
        break;
#endif
        
    case MMIEBOOK_MSG_SHOW_TXT_RDY:
        {
            MMI_STRING_T    mmi_string ={0};
            SCI_TRACE_LOW("ebook_time:MMIEBOOK_MSG_SHOW_TXT_RDY  start=%d",SCI_GetTickCount());
            /*if (MMK_IsOpenWin(MMIEBOOK_OPEN_WAIT_WIN_ID))
            {
                CloseEbookOpenWaitWin();
            }*/
            //获得要显示的字符串
            MMIEBOOK_GetShowTxt(&mmi_string);
            
            //显示该页字符串内容
            GUITEXT_SetFont(MMIEBOOK_TXT_SHOW_CTRL_ID,MMIEbook_GetFont(),&s_ebook_color[s_ebook_fontcolor_id]);/*lint !e605*/
            GUITEXT_SetString(MMIEBOOK_TXT_SHOW_CTRL_ID, mmi_string.wstr_ptr,mmi_string.wstr_len,FALSE);
         
            if (s_is_backlight_turn_off)
            {//允许背光暗时,且为解析完的状态,则设置允许锁键
                MMIDEFAULT_AllowOpenKeylocWin(TRUE);
            }    

            //@songbin.zeng,MS00063924
            if (MMK_IsFocusWin(win_id))
            {
#ifdef  MMIEBOOK_OSDMENU_SUPPORT 
                if(!s_is_auto_dis_osd)
                {
                    CreateOSDMenu(win_id);//该函数会发送MSG_FULL_PAINT
                    s_is_auto_dis_osd=TRUE;
                }
                else
                {
                    MMK_PostMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
                }
#else
                if(s_is_full_screen_off)
                {
                    CreateTitleMenu(win_id);
                    MMK_PostMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
                }
                else
                {
                    DestroyTitleMenu();
                    MMK_PostMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MSG_FULL_PAINT, PNULL, 0);
                }
#endif                 
            }

#ifdef TTS_SUPPORT
            /* Bug fix: cr00202600 and cr00202627. If Font size changed or LCD rotate, 
            we should read the current page from head */
            if(s_tts_auto_pagedown_flag)
            {
                MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
				if(MMK_IsFocusWin(win_id))
				{
					MMK_SendMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_TTS_PLAY,PNULL);
				}
            }
            else
#endif
            {
                if(MMIEBOOK_AUTO_READ_OFF!=MMIAPIEBOOK_GetAutoReadType())
                {
                    EbookStartTimer(&s_auto_read_timer_id, GetAutoReadTime(), FALSE);
                }
            }
            SCI_TRACE_LOW("ebook_time:MMIEBOOK_MSG_SHOW_TXT_RDY  end=%d",SCI_GetTickCount());
        }
        break;
        
        
    case MMIEBOOK_MSG_OPEN_SHOW_FILE_FAIL:
        {
            //关闭提示等待框
            MMIPUB_CloseAlertWin();
#ifdef DRM_SUPPORT
            if (PNULL != param)
            {
                MMIEBOOK_OPEN_FILE_ERROR_E file_error = *((MMIEBOOK_OPEN_FILE_ERROR_E*)param);
                if (MMIEBOOK_OPEN_FILE_NO_RIGHTS == file_error)
                {
                    MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);
                }
            }
            else
#endif		
            MMIPUB_OpenAlertWarningWin(TXT_EBOOK_FILE_NOT_SUPPORT);

            CloseAllEbookWin();
            //OpenFmmWinFromShowWin(); 
        }
        break;
        
    case MMIEBOOK_MSG_ERROR_EXIT:
        CloseAllEbookWin();
        break;
             
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}
/*****************************************************************************/
//  Description : write ebook book mark info
//  Global resource dependence : 
//  Author:  jian.ma
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL BOOLEAN  WriteEbookBookMarkInfo(BOOLEAN is_open_alert_win)
{
    MMIEBOOK_SHOW_INFO_T* show_info =MMIEBOOK_GetShowInfo();
    MMIEBOOK_FILE_BRIEF_T* ebook_show_file_brief_ptr = MMIEBOOK_GetFileBrief();
    wchar bookmark_file_full_path [MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16          offset = 0;
    BOOLEAN         result = FALSE;
    if (PNULL!=show_info 
        &&PNULL!=show_info->cur_show_txt_ptr
        &&PNULL!= ebook_show_file_brief_ptr)
    {
        offset = MMIEBOOK_ConvertWstrOffsetToOtherCodeOffset(
            show_info->cur_show_txt_ptr->ucs2_str_ptr,
            (uint16)show_info->cur_show_txt_ptr->line_offset_tab_ptr[show_info->cur_line]
            );
        
        show_info->cur_read_offset = show_info->cur_show_txt_ptr->this_seg_offset + offset;
        MMIEBOOK_GetBookmarkFileName(ebook_show_file_brief_ptr->full_name_wstr,bookmark_file_full_path);
        if(MMIEBOOK_OpenBookmarkFile(bookmark_file_full_path,"r+"))                
        {
            result=MMIEBOOK_WriteBookmarkFile(MMIEBOOK_GetBookmarkFileHandle(),MMIEBOOK_BOOKMARK_FILE_CUR_READ_OFFSET,&(show_info->cur_read_offset));
            MMIEBOOK_CloseBookmarkFile();
        }
        if((!result)&&(is_open_alert_win))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL, TXT_EBOOK_FILE_INFO_NOT_SAVED, TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleFileErrorAlertWinMsg);
        }
    }
    return result;
}
/*****************************************************************************/
//  Description : append bookmark to list ctrl
//  Global resource dependence : 
//  Author:  jian.ma
//  RETRUN:   
//  Note:   
/*****************************************************************************/
LOCAL void AppendBookmarkItemData(
                                 MMI_CTRL_ID_T  ctrl_id,
                                 uint16 index
                                 )
{
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint16  *temp_bookmark_name_ptr = PNULL;
    uint16  temp_bookmark_name_len = 0;
    uint16 j = 0;
	MMIEBOOK_SHOW_INFO_T* show_info=MMIEBOOK_GetShowInfo();
    uint16 bookmark_pos=show_info->bookmark_info.total_num-1-index;
		
    item_t.item_style                        = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
    item_t.item_data_ptr                     = &item_data;
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;

    //在列表框里面列出所有书签名
    item_data.softkey_id[0] = STXT_OPTION;
#if defined( GUI_MID_SOFTKEY_USE_IMG)   //mini界面中间softkey使用图片替换文本
	item_data.softkey_id[1] =(MMI_IMAGE_ID_T)IMAGE_COMMON_SOFTKEY_OK_ICON;
#else
    item_data.softkey_id[1] = TXT_NULL;
#endif
    item_data.softkey_id[2] = STXT_RETURN;
    item_data.item_content[0].item_data.image_id = IMAGE_EBOOK_BOOKMARK_LIST;

    for (j=0;j<g_ebook_bookmark_name_tab[bookmark_pos].name_len;)
    {
        if ((UCS_CHAR_BLANK!= *((uint16*)g_ebook_bookmark_name_tab[bookmark_pos].name_wstr+j))
            &&(UCS_CHAR_UNDEF != *((uint16*)g_ebook_bookmark_name_tab[bookmark_pos].name_wstr+j)))
        {
            break;
        }
        j+=1;
    }

//  j*=2;

    if (j != g_ebook_bookmark_name_tab[bookmark_pos].name_len)
    {

        temp_bookmark_name_len = g_ebook_bookmark_name_tab[bookmark_pos].name_len - j;
        temp_bookmark_name_ptr = (wchar *)SCI_ALLOC_APP(temp_bookmark_name_len*sizeof(wchar));
        MMI_WSTRNCPY( temp_bookmark_name_ptr,temp_bookmark_name_len,
            g_ebook_bookmark_name_tab[bookmark_pos].name_wstr + j,temp_bookmark_name_len,
            temp_bookmark_name_len);
    }
    else
    {
        temp_bookmark_name_len = g_ebook_bookmark_name_tab[bookmark_pos].name_len;
        temp_bookmark_name_ptr = (wchar *)SCI_ALLOC_APP(temp_bookmark_name_len*sizeof(wchar));
        MMI_WSTRNCPY( temp_bookmark_name_ptr,temp_bookmark_name_len,
            g_ebook_bookmark_name_tab[bookmark_pos].name_wstr + j,temp_bookmark_name_len,
            temp_bookmark_name_len);
    }           

    item_data.item_content[1].item_data.text_buffer.wstr_len = temp_bookmark_name_len;

    item_data.item_content[1].item_data.text_buffer.wstr_ptr = temp_bookmark_name_ptr;

//     MMI_WSTRNCPY( item_data.item_content[1].item_data.text_buffer.wstr,
//         item_data.item_content[1].item_data.text_buffer.wstr_len,
//         temp_bookmark_name_ptr,
//         item_data.item_content[1].item_data.text_buffer.wstr_len,
//         item_data.item_content[1].item_data.text_buffer.wstr_len);

    GUILIST_SetItemData( ctrl_id, &item_data, index );

    if (PNULL != temp_bookmark_name_ptr)
    {
        SCI_FREE(temp_bookmark_name_ptr);
        temp_bookmark_name_ptr = PNULL;
        temp_bookmark_name_len = 0; 
    }    
}

//书签相关，暂时屏蔽
/*****************************************************************************/
//  Description : 书签列表
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBookmarkListWinMsg(
                                             MMI_WIN_ID_T win_id,            
                                             MMI_MESSAGE_ID_E msg_id,        
                                             DPARAM param
                                             )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    uint16                  cur_selection = 0;
    int16                   i = 0;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    extern uint16           g_ebook_new_bookmark_pos;
    MMIEBOOK_SHOW_INFO_T*   s_ebook_show_info = MMIEBOOK_GetShowInfo();
    
    UNUSED_PARAM(param);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetTitleIndexType( MMIEBOOK_BOOKMARK_LIST_CTRL_ID, GUILIST_TITLE_INDEX_DEFAULT );
        GUILIST_SetMaxItem( MMIEBOOK_BOOKMARK_LIST_CTRL_ID, s_ebook_show_info->bookmark_info.total_num, TRUE); //max item 50
    
        item_t.item_style                        = GUIITEM_STYLE_ONE_LINE_ICON_TEXT;
        item_t.item_data_ptr                     = &item_data;
        
        //for ( i = 0; i < (int32)s_ebook_show_info->bookmark_info.total_num; i++ )
        for ( i = (int32)s_ebook_show_info->bookmark_info.total_num-1; i>=0; i-- )
        {
            GUILIST_AppendItem(MMIEBOOK_BOOKMARK_LIST_CTRL_ID, &item_t);
        }       

        MMINV_READ(MMINV_EBOOK_NEW_BOOKMARK,&g_ebook_new_bookmark_pos,return_value);
        if (MN_RETURN_SUCCESS != return_value)
        {
            g_ebook_new_bookmark_pos = 0;
            MMINV_WRITE(MMINV_EBOOK_NEW_BOOKMARK, &g_ebook_new_bookmark_pos);
        }
        else
        {
            cur_selection = g_ebook_new_bookmark_pos; 
        }
        GUILIST_SetCurItemIndex(MMIEBOOK_BOOKMARK_LIST_CTRL_ID,cur_selection);

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

        MMK_SetAtvCtrl(win_id,MMIEBOOK_BOOKMARK_LIST_CTRL_ID);
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            uint16 index = 0;
            GUILIST_NEED_ITEM_DATA_T *need_item_data_ptr = (GUILIST_NEED_ITEM_DATA_T*)param;

            //SCI_ASSERT( PNULL != need_item_data_ptr );/*assert verified*/
            if(PNULL == need_item_data_ptr)
            {
                break;
            }
            index = need_item_data_ptr->item_index;
            AppendBookmarkItemData(MMIEBOOK_BOOKMARK_LIST_CTRL_ID,index);
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        //获得用户选择的书签
        cur_selection = GUILIST_GetCurItemIndex(MMIEBOOK_BOOKMARK_LIST_CTRL_ID); 
		cur_selection=s_ebook_show_info->bookmark_info.total_num-1-cur_selection;
        //SCI_PASSERT(cur_selection < s_ebook_show_info->bookmark_info.total_num/*g_ebook_show_file_detail_ptr->bookmark_total_num*/, /*assert verified*/   
        //("HandleBookmarkListWinMsg: cur_selection is %d,s_bookmark_info_ptr->total_num is %d",cur_selection,s_ebook_show_info->bookmark_info.total_num/*g_ebook_show_file_detail_ptr->bookmark_total_num*/));        
        if(cur_selection >= s_ebook_show_info->bookmark_info.total_num)
        {
            break;
        }
        s_ebook_show_info->cur_show_txt_ptr->this_seg_offset = s_ebook_show_info->bookmark_info.offset_tab[cur_selection];// bookmark_tab[cur_selection].page;        
        MMK_CloseWin( win_id );
        MMK_CloseWin(MMIEBOOK_TXT_SHOW_OPT_WIN_ID);
        /* Bug fix:cr00198795. */
        MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_TURNTO_OFFSET, g_ebook_task_id, 0);
        break;

#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:        
        cur_selection = GUILIST_GetCurItemIndex(MMIEBOOK_BOOKMARK_LIST_CTRL_ID); 
        MMK_CreateWin((uint32 *)MMIEBOOK_BOOKMARK_LIST_OPT_POPUP_WIN_TAB,(ADD_DATA)cur_selection);
        break;
#endif

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32 *)MMIEBOOK_BOOKMARK_LIST_OPT_WIN_TAB,PNULL);
        break;
    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:
        if ((s_ebook_file_list_update)&&((g_ebook_show_file_brief_ptr->file_dev >= MMI_DEVICE_SDCARD)))
        {
            if (MMK_IsOpenWin(MMIEBOOK_TXT_SHOW_OPT_WIN_ID))
                        MMK_CloseWin(MMIEBOOK_TXT_SHOW_OPT_WIN_ID);
            else
                    MMK_CloseWin(MMIEBOOK_TXT_SHOW_WIN_ID);
        }
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : 书签列表选项
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleBookmarkListOptWinMsg(
                                                MMI_WIN_ID_T win_id,            
                                                MMI_MESSAGE_ID_E msg_id,        
                                                DPARAM param
                                                )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T     group_id = 0; 
    MMI_MENU_ID_T           menu_id  = 0;
    uint16                  cur_selection = 0;
    MMIEBOOK_SHOW_INFO_T*   s_ebook_show_info = MMIEBOOK_GetShowInfo();        
    UNUSED_PARAM(param);
    
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        break;
        
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIEBOOK_BOOKMARK_LIST_OPT_CTRL_ID);
        break;
    
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(MMIEBOOK_BOOKMARK_LIST_OPT_CTRL_ID, &group_id, &menu_id);
        switch (menu_id)
        {
        case ID_EBOOK_READ:
            //获得用户选择的书签
            cur_selection = GUILIST_GetCurItemIndex(MMIEBOOK_BOOKMARK_LIST_CTRL_ID); 
            cur_selection=s_ebook_show_info->bookmark_info.total_num-1-cur_selection;
            //SCI_PASSERT(cur_selection < s_ebook_show_info->bookmark_info.total_num/*g_ebook_show_file_detail_ptr->bookmark_total_num*/,     /*assert verified*/   
            //("HandleBookmarkListOptWinMsg: cur_selection is %d,s_ebook_show_info->bookmark_info.total_num is %d",cur_selection,s_ebook_show_info->bookmark_info.total_num/*g_ebook_show_file_detail_ptr->bookmark_total_num*/));/*assert verified*/            
            if(cur_selection >= s_ebook_show_info->bookmark_info.total_num)
            {
                break;
            }
            
            s_ebook_show_info->cur_show_txt_ptr->this_seg_offset = s_ebook_show_info->bookmark_info.offset_tab[cur_selection];
            MMK_CloseWin( win_id );
            MMK_CloseWin(MMIEBOOK_BOOKMARK_LIST_WIN_ID);
            MMK_CloseWin(MMIEBOOK_TXT_SHOW_OPT_WIN_ID);
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_TURNTO_OFFSET, g_ebook_task_id, 0);
            break;
            
        case ID_EBOOK_DELETE:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        case ID_EBOOK_DELALL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELALL,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
            
        default:
            break;
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );      
            break;
            
        case MSG_CLOSE_WINDOW:
            //add by rongjin, fixed bug 22379,070803
            if ((s_ebook_file_list_update)&&((g_ebook_show_file_brief_ptr->file_dev >= MMI_DEVICE_SDCARD)))
            {
            MMK_CloseWin(MMIEBOOK_BOOKMARK_LIST_WIN_ID);
            }
            //add end
            break;
            
        case MSG_APP_LEFT:      
        case MSG_APP_RIGHT:
            break;
            
        case MSG_PROMPTWIN_OK:
            MMIPUB_CloseQuerytWin(PNULL);
            GUIMENU_GetId(MMIEBOOK_BOOKMARK_LIST_OPT_CTRL_ID, &group_id, &menu_id);
            switch (menu_id)
            {
            case ID_EBOOK_DELETE:
                cur_selection = GUILIST_GetCurItemIndex(MMIEBOOK_BOOKMARK_LIST_CTRL_ID); 
                //SCI_PASSERT(cur_selection < s_ebook_show_info->bookmark_info.total_num,  /*assert verified*/   
                //("HandleBookmarkListOptWinMsg: cur_selection is %d,s_ebook_show_info->bookmark_info.total_num is %d",cur_selection,s_ebook_show_info->bookmark_info.total_num/*g_ebook_show_file_detail_ptr->bookmark_total_num*/)); /*assert verified*/           
                if(cur_selection >= s_ebook_show_info->bookmark_info.total_num)
                {
                    break;
                }
                
                MMIEBOOK_DeleteBookmark(cur_selection);
                GUILIST_RemoveItem(MMIEBOOK_BOOKMARK_LIST_CTRL_ID, cur_selection);
                if (0 == s_ebook_show_info->bookmark_info.total_num)
                {
                    //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                    MMK_CloseWin( win_id );
                    MMK_CloseWin(MMIEBOOK_BOOKMARK_LIST_WIN_ID);
                }
                else
                {   
                    //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                    MMK_CloseWin( win_id );
                }
                break;
                
            case ID_EBOOK_DELALL:
                MMIEBOOK_DeleteAllBookmark();
                GUILIST_RemoveAllItems(MMIEBOOK_BOOKMARK_LIST_CTRL_ID);
                //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                MMK_CloseWin( win_id );
                MMK_CloseWin(MMIEBOOK_BOOKMARK_LIST_WIN_ID);
                break;
                
            default:
                break;
            }
            break;
            
        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            break;
                
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}

#ifdef MMI_PDA_SUPPORT
LOCAL MMI_RESULT_E  HandleBookmarkListOptPopupWinMsg(
                                                     MMI_WIN_ID_T win_id,            
                                                     MMI_MESSAGE_ID_E msg_id,        
                                                     DPARAM param
                                                     )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMIEBOOK_SHOW_INFO_T*   s_ebook_show_info = MMIEBOOK_GetShowInfo();   
    MMI_CTRL_ID_T           ctrl_id = MMIEBOOK_BOOKMARK_LIST_OPT_POPUP_CTRL_ID;
    
    uint32                  cur_list_selection = (uint32)MMK_GetWinAddDataPtr(win_id );
    uint32                  bookmark_pos = s_ebook_show_info->bookmark_info.total_num - 1 - cur_list_selection;
    
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        break;
        
    case MSG_OPEN_WINDOW:
        {
            uint32 i = 0;
            MMI_STRING_T kstring = {0};
            GUIMENU_BUTTON_INFO_T    button_info={0};
            int32 j= 0;
            for (j=0;j<g_ebook_bookmark_name_tab[bookmark_pos].name_len;)
            {
                if ((UCS_CHAR_BLANK!= *((uint16*)g_ebook_bookmark_name_tab[bookmark_pos].name_wstr+j))
                    &&(UCS_CHAR_UNDEF != *((uint16*)g_ebook_bookmark_name_tab[bookmark_pos].name_wstr+j)))
                {
                    break;
                }
                j+=1;
            }
            
            
            if (j != g_ebook_bookmark_name_tab[bookmark_pos].name_len)
            {
                
                kstring.wstr_len = g_ebook_bookmark_name_tab[bookmark_pos].name_len - j;
                kstring.wstr_ptr = (wchar *)SCI_ALLOC_APP((kstring.wstr_len + 1)*sizeof(wchar));
                SCI_MEMSET(kstring.wstr_ptr , 0, (kstring.wstr_len + 1)*sizeof(wchar));
                MMI_WSTRNCPY( kstring.wstr_ptr, kstring.wstr_len,
                    g_ebook_bookmark_name_tab[bookmark_pos].name_wstr + j,kstring.wstr_len,
                    kstring.wstr_len);
            }
            else
            {
                kstring.wstr_len = g_ebook_bookmark_name_tab[bookmark_pos].name_len;
                kstring.wstr_ptr = (wchar *)SCI_ALLOC_APP(kstring.wstr_len*sizeof(wchar));
                MMI_WSTRNCPY( kstring.wstr_ptr,kstring.wstr_len,
                    g_ebook_bookmark_name_tab[bookmark_pos].name_wstr + j,kstring.wstr_len,
                    kstring.wstr_len);
            }           
            
            GUIMENU_CreatDynamic(PNULL,win_id,ctrl_id,GUIMENU_STYLE_POPUP_AUTO);
            
            GUIMENU_SetMenuTitle(&kstring,ctrl_id);
            SCI_FREE(kstring.wstr_ptr);
            
            button_info.is_static=FALSE;
            button_info.node_id=0;
            button_info.button_style=GUIMENU_BUTTON_STYLE_CANCEL;
            GUIMENU_SetButtonStyle(ctrl_id,&button_info);
            
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_DELETE,//text_ptr 
                IMAGE_COMMON_OPTMENU_DELETE,
                1,//node_id
                0, //parent_node_id
                i++//index
                );
            MMK_SetAtvCtrl(win_id, MMIEBOOK_BOOKMARK_LIST_OPT_POPUP_CTRL_ID);
        }
        break;
        
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {   
            uint32 node_id = GUIMENU_GetCurNodeId(ctrl_id) ;
            switch (node_id)
            {                
            case 1:
                MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
                break;                
                
            default:
                break;
            }
        }
            break;
            
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_CLOSE_WINDOW:
        //add by rongjin, fixed bug 22379,070803
        if ((s_ebook_file_list_update)&&((g_ebook_show_file_brief_ptr->file_dev >= MMI_DEVICE_SDCARD)))
        {
            MMK_CloseWin(MMIEBOOK_BOOKMARK_LIST_WIN_ID);
        }
        break;
        
    case MSG_PROMPTWIN_OK:
        {
            uint32 node_id = GUIMENU_GetCurNodeId( ctrl_id) ;
            switch (node_id)
            {
            case 1:
                MMIEBOOK_DeleteBookmark(cur_list_selection);
                GUILIST_RemoveItem(MMIEBOOK_BOOKMARK_LIST_CTRL_ID, cur_list_selection);
                if (0 == s_ebook_show_info->bookmark_info.total_num)
                {
                    //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                    MMK_CloseWin( win_id );
                    MMK_CloseWin(MMIEBOOK_BOOKMARK_LIST_WIN_ID);
                }
                else
                {   
                    //MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
                    MMK_CloseWin( win_id );
                }
                break;                    
                
            default:
                break;
            }
            MMIPUB_CloseQuerytWin(PNULL);
        }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
        }
        
        return (result);
    }
#endif

/*****************************************************************************/
//  Description : 跳转百分比
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePercentInputWinMsg (
                                           MMI_WIN_ID_T win_id,             
                                           MMI_MESSAGE_ID_E msg_id,         
                                           DPARAM param
                                           )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_STRING_T    name_str = {0};
    uint16          *input_range_ucs_str = PNULL;
    uint16           *input_range_str = L"(0-100):";
    uint16 input_range_gb_len = 0;
    //wchar           input_range_ucs_str2[MMIEBOOK_INPUT_PERCENT_RANGE] = {0};
    uint8          input_digital_str[MMIEBOOK_INPUT_PERCENT_RANGE] = {0};
    //uint16          ucs2_len = 0;
    MMI_STRING_T   input_str = {0};
    int32           page_input = 0;

//    SCI_ASSERT(PNULL != g_ebook_show_file_detail_ptr);/*assert verified*/
    
    UNUSED_PARAM(param);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //显示输入页面范围标题
        MMI_GetLabelTextByLang(TXT_COMMON_INPUT_TIP,&name_str);
        input_range_gb_len = MMIAPICOM_Wstrlen(input_range_str);
        input_range_ucs_str =(uint16 *)SCI_ALLOCA(
            sizeof(wchar)*(input_range_gb_len+name_str.wstr_len+1));
        if (PNULL != input_range_ucs_str)
        {
            SCI_MEMSET(input_range_ucs_str,0,sizeof(wchar)*(input_range_gb_len+name_str.wstr_len+1));
            MMIAPICOM_Wstrncpy(input_range_ucs_str, name_str.wstr_ptr, name_str.wstr_len);
            MMIAPICOM_Wstrcat(input_range_ucs_str, input_range_str);
            name_str.wstr_ptr = input_range_ucs_str;
            name_str.wstr_len = MMIAPICOM_Wstrlen(name_str.wstr_ptr);
     
            GUIWIN_SetTitleText(win_id, name_str.wstr_ptr, name_str.wstr_len, FALSE);
            SCI_FREE(input_range_ucs_str);
        }
        
        GUIEDIT_SetSoftkey(MMIEBOOK_PERCENT_INPUT_CTRL_ID,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,MMIEBOOK_PERCENT_INPUT_CTRL_ID);
        
        break;
        
    case MSG_FULL_PAINT:
        //LCD_FillRect(MMITHEME_GetDefaultLcdDev(), rect, MMI_WINDOW_BACKGROUND_COLOR);
        break;
        
#ifdef MMI_PDA_SUPPORT     
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id= ((MMI_NOTIFY_T*) param)->src_id;
            switch(ctrl_id)
            {
            case MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID:
    			MMK_SendMsg(win_id, MSG_APP_OK,  PNULL);
    			break;
            case MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID:
    			MMK_SendMsg(win_id, MSG_APP_CANCEL,  PNULL);
    			break;
            default:
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    //case MSG_APP_MENU:
        //获取用户输入的跳转页面
        input_str.wstr_len = 0;
        SCI_MEMSET(input_digital_str, 0, MMIEBOOK_INPUT_PERCENT_RANGE);
        GUIEDIT_GetString(MMIEBOOK_PERCENT_INPUT_CTRL_ID, &input_str);
        if(input_str.wstr_len > 0)
        {
            MMIAPICOM_WstrToStr((const wchar*)input_str.wstr_ptr,input_digital_str);
            //用户输入了数字
            page_input = atol((char*)input_digital_str);

            //输入有效数字
            if(page_input <= 100
            && page_input >= 0
            )
            {
                MMIEBOOK_SetPercent((uint32)page_input);
            }
            else//如果输入0-100之外的跳转，弹出错误提示框，要求输入0-100
            {

                MMIPUB_OpenAlertWarningWin(TXT_EBOOK_JUMP_TO_PERCENT_WARNING);
                break;
            }
           

            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIEBOOK_TXT_SHOW_OPT_WIN_ID);
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_TURNTO_PERCENT, g_ebook_task_id, PNULL);//跳转百分比
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
//add by rongjin, fixed bug 22379,070803
    case MSG_CLOSE_WINDOW:
        if ((s_ebook_file_list_update)&&((g_ebook_show_file_brief_ptr->file_dev >= MMI_DEVICE_SDCARD)))
        {
            if (MMK_IsOpenWin(MMIEBOOK_TXT_SHOW_OPT_WIN_ID))
            {
                MMK_CloseWin(MMIEBOOK_TXT_SHOW_OPT_WIN_ID);
            }
            else
            {
                MMK_CloseWin(MMIEBOOK_TXT_SHOW_WIN_ID);
            }
        }
        break;
//add end

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : open ebook txt show opt menu
//  Global resource dependence : 
//  Author:maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void  OpenEbookShowOptMenu(void)
{
    MMI_WIN_ID_T                win_id = MMIEBOOK_TXT_SHOW_OPT_WIN_ID;
    MMI_CTRL_ID_T               ctrl_id = MMIEBOOK_TXT_SHOW_OPT_CTRL_ID;
    MMI_STRING_T                kstring = {0};
    uint32                      i =0;
    MMI_IMAGE_ID_T          image_id = 0;
    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    
    MMK_CreateWin((uint32 *)MMIEBOOK_TXT_SHOW_OPT_WIN_TAB,PNULL);
	
    //creat dynamic menu
    GUIMENU_CreatDynamic(PNULL,MMIEBOOK_TXT_SHOW_OPT_WIN_ID,ctrl_id,GUIMENU_STYLE_POPUP);

    MMI_GetLabelTextByLang(STXT_OPTION, &kstring);
    GUIMENU_SetMenuTitle(&kstring,ctrl_id);
    i = 0;
    if(!is_disenable_txt_func)
    {
#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_EBOOK_MENU_ICON_ADD_BOOKMARK;
#else
        image_id = 0;
#endif
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_ADD_BOOKMARK,//text_ptr 
            image_id,
            EBOOK_NODE_TXT_OPT_ADD_BOOKMARK,//node_id
            0, //parent_node_id
            i++//index
            );
        
#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_EBOOK_MENU_ICON_VIEW_BOOKMARK;
#else
        image_id = 0;
#endif
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_EBOOK_SEE_BOOKMARK,//text_ptr     
            image_id,
            EBOOK_NODE_TXT_OPT_SEE_BOOKMARK,//node_id
            0, //parent_node_id
            i++//index
            );    

#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_COMMON_OPTMENU_SET;
#else
        image_id = 0;
#endif
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_DISPLAY_SET,//text_ptr     
            image_id,
            EBOOK_NODE_TXT_OPT_DISPLAY_SETTING,//node_id
            0, //parent_node_id
            i++//index
            );   
#endif
        
        if (s_is_backlight_turn_off)
        {
#ifdef MMI_PDA_SUPPORT
            image_id = IMAGE_EBOOK_MENU_ICON_BACKLIGHT_ALYWAS_ON_ACITEVED;
#else
            image_id = 0;
#endif
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
//#ifdef MMI_PDA_SUPPORT
                TXT_EBOOK_PDA_BACKLIGHT_TURN_ON,//text_ptr
//#else
//                TXT_EBOOK_BACKLIGHT_TURN_ON,//text_ptr 
//#endif
                image_id,
                EBOOK_NODE_TXT_OPT_BACKLIGHT_TURN_ON,//node_id
                0, //parent_node_id
                i++//index
                );
        }
        else
        {
#ifdef MMI_PDA_SUPPORT
            image_id = IMAGE_EBOOK_MENU_ICON_BACKLIGHT_ALYWAS_ON_DEACITEVED;
#else
            image_id = 0;
#endif
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
//#ifdef MMI_PDA_SUPPORT
                TXT_EBOOK_PDA_BACKLIGHT_TURN_OFF,//text_ptr
//#else
//                TXT_EBOOK_BACKLIGHT_TURN_OFF,//text_ptr 
//#endif
                image_id,
                EBOOK_NODE_TXT_OPT_BACKLIGHT_TURN_OFF,//node_id
                0, //parent_node_id
                i++//index
                );
        }

#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_EBOOK_MENU_ICON_DAY_NIGHT_MODE;
#else
        image_id = 0;
#endif
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_EBOOK_DAY_NIGHT_MODE,//text_ptr     
            image_id,
            EBOOK_NODE_TXT_OPT_DAY_NIGHT_MODE,//node_id
            0, //parent_node_id
            i++//index
            );   
#if 0       
        if (s_is_full_screen_off)//全屏浏览
        {
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_EBOOK_FULL_SCREEN_ON,//text_ptr 
                0,
                EBOOK_NODE_TXT_OPT_FULL_SCREEN_ON,//node_id
                0, //parent_node_id
                i++//index
                );
        }
        else
        {
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_EBOOK_FULL_SCREEN_OFF,//text_ptr 
                0,
                EBOOK_NODE_TXT_OPT_FULL_SCREEN_OFF,//node_id
                0, //parent_node_id
                i++//index
                );
        }
        
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_EBOOK_JUMP_TO_BEGIN,//TXT_EBOOK_JUMP_FIRST_PAGE,//text_ptr 
            0,
            EBOOK_NODE_TXT_OPT_JUMP_TO_BEGIN,//node_id
            0, //parent_node_id
            i++//index
            );
        
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_EBOOK_JUMP_TO_END,//TXT_EBOOK_JUMP_LAST_PAGE,//text_ptr 
            0,
            EBOOK_NODE_TXT_OPT_JUMP_TO_END,//node_id
            0, //parent_node_id
            i++//index
            );
#endif
        
#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_EBOOK_MENU_ICON_JUMP_TO_PERCENT;
#else
        image_id = 0;
#endif

        InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_EBOOK_JUMP_TO_PERCENT,//text_ptr 
        image_id,
        EBOOK_NODE_TXT_OPT_JUMP_PERCENT,//node_id
        0, //parent_node_id
        i++//index
        );

#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_EBOOK_MENU_ICON_AUTO_BROWSE;
#else
        image_id = 0;
#endif
        
        //自动浏览
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_EBOOK_AUTO_READ,//text_ptr 
            image_id,
            EBOOK_NODE_TXT_OPT_AUTO_VIEW,//node_id
            0, //parent_node_id
            i++//index
            );

#ifdef TTS_SUPPORT
        if(s_tts_auto_pagedown_flag)
        {
#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_EBOOK_MENU_ICON_READ_OFF;
#else
        image_id = 0;
#endif
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_EBOOK_STOP_TTS,//text_ptr 
                image_id,
                EBOOK_NODE_TXT_OPT_READ_STOP,//node_id
                0, //parent_node_id
                i++//index
                );
        }
        else
        {
#ifdef MMI_PDA_SUPPORT
        image_id = IMAGE_EBOOK_MENU_ICON_READ_ON;
#else
        image_id = 0;
#endif
            InsertNodeByLabel(
                ctrl_id,//ctrl_id
                TXT_EBOOK_OPEN_TTS,//text_ptr 
                image_id,
                EBOOK_NODE_TXT_OPT_READ_START,//node_id
                0, //parent_node_id
                i++//index
                );
        }
#endif
    }
    else
    {
#ifdef MMI_PDA_SUPPORT
#ifdef MMI_VCARD_SUPPORT
        image_id = IMG_PB_MENU_IMPORT_ICON;
#else
        image_id = 0;
#endif
#else
        image_id = 0;
#endif
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_COMMON_IMPORT,//text_ptr 
            image_id,
            EBOOK_NODE_TXT_OPT_VCARD_IMPORT,//node_id
            0, //parent_node_id
            i++//index
            );
    }
#if defined MMI_PDA_SUPPORT && defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        InsertNodeByLabel(
        ctrl_id,//ctrl_id
        STXT_EXIT,//text_ptr 
        0,
        EBOOK_NODE_TXT_OPT_EXIT,//node_id
        0, //parent_node_id
        i++//index
        );
    }
#endif
}

PUBLIC MMI_HANDLE_T MMIEBOOK_OpenCodeTypeWin(ADD_DATA add_data_ptr)
{
#ifdef MMI_PDA_SUPPORT
        return MMK_CreatePubListWin((uint32 *)MMIEBOOK_SET_CODING_TYPE_WIN_TAB, add_data_ptr);
#else
        return MMK_CreateWin((uint32 *)MMIEBOOK_SET_CODING_TYPE_WIN_TAB, add_data_ptr);
#endif
}

PUBLIC MMI_HANDLE_T MMIEBOOK_OpenAutoReadSetWin(ADD_DATA add_data_ptr)
{
#ifdef MMI_PDA_SUPPORT
        return MMK_CreatePubListWin((uint32 *)MMIEBOOK_SET_AUTO_READ_WIN_TAB, add_data_ptr);
#else
        return MMK_CreateWin((uint32 *)MMIEBOOK_SET_AUTO_READ_WIN_TAB, add_data_ptr);
#endif
}

/*****************************************************************************/
//  Description : handle txt show opt menu msg
//  Global resource dependence : 
//  Author:maggie.ren
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleTxtShowOptWinMsg(
                                     MMI_WIN_ID_T win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM param
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
//     uint32       free_high_space = 0;
//     uint32       free_low_space = 0;
#if defined MMI_VCARD_SUPPORT
    wchar *file_name_ptr=PNULL;
    uint32 file_name_len=0;
    //MMIPB_CONTACT_T  contact_info = {0};
    MMIEBOOK_FILE_BRIEF_T* file_brief=PNULL;
#endif

    switch(msg_id)
    {
    case MSG_FULL_PAINT:       
        break;
        
    case MSG_OPEN_WINDOW:
        if (s_is_backlight_turn_off)
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        }
        else
        {   
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        MMK_SetAtvCtrl(win_id,  MMIEBOOK_TXT_SHOW_OPT_CTRL_ID);
        break;
        
    case MSG_LOSE_FOCUS:
        //允许背光关闭
        MMIDEFAULT_AllowTurnOffBackLight(TRUE); 
        break;
        
    case MSG_GET_FOCUS:
        //恢复背光灯状态
        if (s_is_backlight_turn_off)
        {
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        }
        else
        {
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
        }
        break;
        
        //add by rongjin, fixed bug 22379,070803
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        if((s_ebook_file_list_update)&&((g_ebook_show_file_brief_ptr->file_dev >= MMI_DEVICE_SDCARD)))
            MMK_CloseWin(MMIEBOOK_TXT_SHOW_WIN_ID);
        break;
        //add end
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        {
            uint16 node_id = 0;
            MMIEBOOK_SHOW_INFO_T* s_ebook_show_info = MMIEBOOK_GetShowInfo();
                        
            node_id = GUIMENU_GetCurNodeId(MMIEBOOK_TXT_SHOW_OPT_CTRL_ID);
            switch (node_id)
            {
                case EBOOK_NODE_TXT_OPT_ADD_BOOKMARK:
                    if (s_ebook_show_info->bookmark_info.total_num >= MMIEBOOK_BOOKMARK_MAX_NUM)
                    {
                        //显示提示书签满
                        MMIPUB_OpenAlertWarningWin(TXT_COMON_BOOKMARK_FULL);
                        MMK_CloseWin( win_id );
                        break;
                    }

                    if(!MMIEBOOK_AddBookmark())
                    {
                        if ( MMK_IsOpenWin( MMIPUB_ALERT_WIN_ID ) )
                        {
                            //关闭提示等待框
                            MMIPUB_CloseAlertWin();
                        }
                        MMIPUB_OpenAlertFailWin( TXT_EBOOK_INFO_SAVE_FAIL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
                    }
                    MMK_CloseWin( win_id );
                    break;

                case EBOOK_NODE_TXT_OPT_SEE_BOOKMARK:
                    if (s_ebook_show_info->bookmark_info.total_num == 0)
                    {
                        //显示提示书签空
                        MMIPUB_OpenAlertWarningWin(TXT_EBOOK_BOOKMARK_EMPTY);
                        MMK_CloseWin(MMIEBOOK_TXT_SHOW_OPT_WIN_ID);
                    }
                    else
                    {
                        MMK_CreateWin((uint32 *)MMIEBOOK_BOOKMARK_LIST_WIN_TAB, PNULL);
                    }
                    break;

#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
                case EBOOK_NODE_TXT_OPT_DISPLAY_SETTING:                
                    MMK_CreateWin((uint32 *)MMIEBOOK_DISPLAY_SET_WIN_TAB, PNULL);
                    break;
#endif

                case EBOOK_NODE_TXT_OPT_DAY_NIGHT_MODE:
#ifdef MMI_PDA_SUPPORT
                    MMK_CreatePubListWin((uint32 *)MMIEBOOK_DAY_NIGHT_MODE_WIN_TAB, PNULL);
#else
                    MMK_CreateWin((uint32 *)MMIEBOOK_DAY_NIGHT_MODE_WIN_TAB, PNULL);
#endif
                    break;

                case EBOOK_NODE_TXT_OPT_JUMP_TO_BEGIN:      //跳转至开始
                    MMK_CloseWin( win_id );
                    MMIEBOOK_SetPercent(0);
                    MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_TURNTO_PERCENT, g_ebook_task_id, PNULL);//跳转百分比
                    break;

                case EBOOK_NODE_TXT_OPT_JUMP_TO_END:       //跳转至结尾
                    MMK_CloseWin( win_id );
                    MMIEBOOK_SetPercent(100);
                    MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_TURNTO_PERCENT, g_ebook_task_id, PNULL);//跳转百分比
                    break;

                case EBOOK_NODE_TXT_OPT_JUMP_PERCENT:
                    MMK_CreateWin((uint32 *)MMIEBOOK_PERCENT_INPUT_WIN_TAB, PNULL);
                    break;

                case EBOOK_NODE_TXT_OPT_CODING_TYPE://编码方式
#ifdef MMI_PDA_SUPPORT
                    MMK_CreatePubListWin((uint32 *)MMIEBOOK_SET_CODING_TYPE_WIN_TAB, PNULL);
#else
                    MMK_CreateWin((uint32 *)MMIEBOOK_SET_CODING_TYPE_WIN_TAB, PNULL);
#endif
                    break;           

                //@maggie add begin for ebook
                case EBOOK_NODE_TXT_OPT_BACKLIGHT_TURN_OFF:
                    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                    s_is_backlight_turn_off = TRUE;
                    /*                if (MMIEBOOK_IsMakeSegRunning())//解析显示百分比时,不允许锁屏
                    {
                    MMIDEFAULT_AllowOpenKeylocWin(FALSE);
                    }*///已没有解析过程，允许锁屏
                    MMIPUB_OpenAlertSuccessWin(TXT_EBOOK_HAVE_BACKLIGHT_TURN_OFF);
                    MMK_CloseWin( win_id );
                    break;

                case EBOOK_NODE_TXT_OPT_BACKLIGHT_TURN_ON:
                    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
                    s_is_backlight_turn_off = FALSE;
                    MMIPUB_OpenAlertSuccessWin(TXT_EBOOK_HAVE_BACKLIGHT_TURN_ON);
                    MMK_CloseWin( win_id );
                    break;

                case EBOOK_NODE_TXT_OPT_FULL_SCREEN_OFF:
                    SetIsFullScreenOff(TRUE);
                    //s_is_set_txt_box_rect=TRUE;
                    if(!MMIEBOOK_GetIsFullScreenOff())
                    {
                        MMIEBOOK_SetIsTxtShowWinCtrlvisible(FALSE);
                    }
                    else
                    {
                        MMIEBOOK_SetIsTxtShowWinCtrlvisible(TRUE);
                    }
                    SetTxtBoxRect();
                    MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_FULL_SCREEN_SWITCH, g_ebook_task_id, PNULL);
                    break;
                    case EBOOK_NODE_TXT_OPT_FULL_SCREEN_ON:
                    SetIsFullScreenOff(FALSE);
                    //s_is_set_txt_box_rect=TRUE;
                    if(!MMIEBOOK_GetIsFullScreenOff())
                    {
                        MMIEBOOK_SetIsTxtShowWinCtrlvisible(FALSE);
                    }
                    else
                    {
                        MMIEBOOK_SetIsTxtShowWinCtrlvisible(TRUE);
                    }
                    SetTxtBoxRect();
                    MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_FULL_SCREEN_SWITCH, g_ebook_task_id, PNULL);
                    break;

                case EBOOK_NODE_TXT_OPT_AUTO_VIEW:
#ifdef MMI_PDA_SUPPORT
                    MMK_CreatePubListWin((uint32*)MMIEBOOK_SET_AUTO_READ_WIN_TAB, PNULL);
#else
                    MMK_CreateWin((uint32*)MMIEBOOK_SET_AUTO_READ_WIN_TAB, PNULL);  
#endif
                    MMK_CloseWin( win_id );
                    break;

#ifdef TTS_SUPPORT
                case EBOOK_NODE_TXT_OPT_READ_START:
                    s_tts_auto_pagedown_flag=TRUE;
                    break;
                
                case EBOOK_NODE_TXT_OPT_READ_STOP:
                    //MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
                    s_tts_auto_pagedown_flag=FALSE;
                    break;
#endif
                case EBOOK_NODE_TXT_OPT_EXIT:
                    MMK_SendMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MSG_APP_CANCEL, PNULL);
                    MMK_CloseWin( win_id );
                    break;
                case EBOOK_NODE_TXT_OPT_VCARD_IMPORT:
#if defined MMI_VCARD_SUPPORT
                    MMK_CloseWin( win_id );
                    file_name_ptr=(wchar*)SCI_ALLOC_APP(sizeof(wchar)*(MMIEBOOK_FULLNAME_MAX_LEN+1));
                    if(PNULL==file_name_ptr)
                    {
                        break;
                    }
                    SCI_MEMSET(file_name_ptr,0,sizeof(wchar)*(MMIEBOOK_FULLNAME_MAX_LEN+1));
                    file_brief= MMIEBOOK_GetFileBrief();
                    MMIAPICOM_Wstrncpy(file_name_ptr, file_brief->full_name_wstr, file_brief->full_name_len);
                    file_name_len=file_brief->full_name_len;
                    MMK_SendMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MSG_APP_CANCEL, PNULL);

                    if(MMIPB_ERROR_PENDING == MMIAPIPB_ImportVcardAsyn(MMIAPIPB_CreateHandle(), file_name_ptr, Importvcard_Callback))                   
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleImportVcardWaitingWinMsg);
                        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_SUCCESS,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                    }
                    SCI_FREE(file_name_ptr);
#endif
                    break;

                default:
                    //SCI_TRACE_LOW:"mmiebook_wintab.c HandleTxtShowOptWinMsg node_id is %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_3496_112_2_18_2_15_31_78,(uint8*)"d",node_id);
                    break;
            }
            MMK_CloseWin(win_id);
            break;
            //@maggie add end   for ebook
            
            }
            
            default:
                recode = MMI_RESULT_FALSE;
                break;
        }
        return recode;
}  

/**************************************************************************************************/
// Description:import vcard call back function
// Date:
// Author:
// Note:
//*************************************************************************************************/
LOCAL void  Importvcard_Callback(MMIPB_HANDLE_T handle, MMIPB_ERROR_E error)
{
    MMIAPIPB_CloseHandle(handle);
    MMIPUB_CloseAlertWin();
	//+Bug 134367 
    if(MMIPB_ERROR_NO_SPACE == error)
    {
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SPACE,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    //-Bug 134367 
    else if(MMIPB_ERROR_SUCCESS != error
      && MMIPB_ERROR_NO_ENTRY != error)
    {
        //import fail
       
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    MMK_CloseWin(MMIFMM_FILE_WAITING_WIN_ID);
}

/*****************************************************************************/
//  Description : insert dynamenu node by label
//  Global resource dependence : 
//  Author:maggie.ren
//  Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T      text_id,       //插入的节点的文本
                             MMI_IMAGE_ID_T     image_id,       //插入的节点的文本
                             uint32             node_id,        //节点编号
                             uint32             parent_node_id, //父节点编号
                             uint16             index           //位置
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;    
    node_item.select_icon_id = image_id;
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}  
//@maggie add end   for ebook

/*****************************************************************************/
//  Description : stop current operation when sd plug
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_StopOperaSDPlug(void)
{
    CloseAllEbookWin();
}

/*****************************************************************************/
//  Description : CloseAllEbookWin
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
/*****************************************************************************/
LOCAL void CloseAllEbookWin( void )
{
#ifdef MMI_TASK_MANAGER
    if(s_ebook_applet_instance != PNULL)
    {
        MMK_CloseMiddleWinByApplet(s_ebook_applet_instance->caf_applet.app_handle, MMIEBOOK_WIN_ID_MIN,MMIEBOOK_WIN_ID_MAX);
    }
    else
    {
        MMK_CloseMiddleWin(MMIEBOOK_WIN_ID_MIN,MMIEBOOK_WIN_ID_MAX);//MMK_CloseMiddleWinByApplet(SPRD_EBOOK_APPLET_ID, MMIEBOOK_WIN_ID_MIN,MMIEBOOK_WIN_ID_MAX);
    }
#else
    MMK_CloseMiddleWin(MMIEBOOK_WIN_ID_MIN,MMIEBOOK_WIN_ID_MAX);
#endif
}

//编码方式
/*****************************************************************************/
//  Description : 编码方式设置界面
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetCodingType(
                                        MMI_WIN_ID_T       win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIEBOOK_SET_CODINGTYPE_CTRL_ID,MMIEBOOK_CODING_TYPE_MAX, FALSE );//max item s2
        AppendCodingTyleListItem(MMIEBOOK_SET_CODINGTYPE_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMIEBOOK_SET_CODINGTYPE_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:       
    case MSG_APP_WEB:    
    case MSG_APP_OK:
    case MSG_CTL_OK:
        SetCodingType(MMIEBOOK_SET_CODINGTYPE_CTRL_ID);    
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id ); 
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            }
#endif     
            MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 编码方式选项:1、GB(简体中文) 2、Big5(繁体中文)
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL void AppendCodingTyleListItem(
     MMI_CTRL_ID_T      ctrl_id
     )
{
     uint16                      cur_selection = 0;
     MMIEBOOK_SET_CODING_TYPE_E     coding_type = MMIEBOOK_CODING_TYPE_GB;    
     
     MMIAPISET_AppendListItemByTextId( TXT_CODING_TYPE_GB, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
     MMIAPISET_AppendListItemByTextId( TXT_CODING_TYPE_BIG5, STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        
     //get current methed
     coding_type = MMIAPIEBOOK_GetCodingType();
     
     //the status is open
     cur_selection = coding_type;

     GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
     GUILIST_SetCurItemIndex(ctrl_id,cur_selection);         
}

/*****************************************************************************/
//  Description : 设置编码方式
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetCodingType(
                         MMI_CTRL_ID_T      ctrl_id
                         )
{
    uint16      cur_selection = 0;
    MMIEBOOK_SET_CODING_TYPE_E     coding_type = MMIEBOOK_CODING_TYPE_GB;  
    
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,MMIEBOOK_CODING_TYPE_MAX);
    //SCI_ASSERT((MMIEBOOK_CODING_TYPE_MAX > cur_selection));/*assert verified*/
    
    // set the global virable
    switch (cur_selection)
    {
    case MMIEBOOK_CODING_TYPE_GB:
        coding_type = MMIEBOOK_CODING_TYPE_GB;
        break;
        
    default:
        coding_type = MMIEBOOK_CODING_TYPE_BIG5;
        break;
    }
    
    //write the status of main menu animation into NV
    MMINV_WRITE(MMINV_EBOOK_CODING_TYPE,&coding_type);
}

/*****************************************************************************/
//     Description : 获得编码方式
// Global resource dependence : 
//  Author: zhaohui
// Note:
/*****************************************************************************/
PUBLIC MMIEBOOK_SET_CODING_TYPE_E MMIAPIEBOOK_GetCodingType( void )
{
    MMIEBOOK_SET_CODING_TYPE_E     coding_type = MMIEBOOK_CODING_TYPE_GB;    
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;

    //get menu animation info from NV
    MMINV_READ(MMINV_EBOOK_CODING_TYPE,&coding_type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
        MMIAPISET_GetLanguageType(&lang_type);
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
        if( MMISET_LANGUAGE_TRAD_CHINESE == lang_type )
        {
            coding_type = MMIEBOOK_CODING_TYPE_BIG5; 
        }
        else
#endif
        {
            coding_type = MMIEBOOK_CODING_TYPE_GB; 
        }
        MMINV_WRITE(MMINV_EBOOK_CODING_TYPE,&coding_type);
    }

    return (coding_type);
}

 /*****************************************************************************/
 //     Description : HandleSetColorStyle
 // Global resource dependence : 
 //  Author: zhaohui
 // Note:
 /*****************************************************************************/
 LOCAL MMI_RESULT_E  SetPreviewLabelColor( MMI_CTRL_ID_T ctrl_id, MMIEBOOK_SET_COLOR_E color_id, 
 								MMIEBOOK_SET_BG_E backcolor_id, MMIEBOOK_FONT_SIZE_E size_id, BOOLEAN is_update)
 {
    MMI_STRING_T    text = {0};
    GUI_BG_T txtbox_bg = {0};

    MMI_GetLabelTextByLang(TXT_FREEDOM_COMMUNICATION, &text);
#ifndef MMI_LOW_MEMORY_EBOOK            
    if(s_ebook_bg_id<=MMIEBOOK_BG_3)
    {
        txtbox_bg.bg_type = GUI_BG_COLOR;
        txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
    }
    else
    {
        txtbox_bg.bg_type = GUI_BG_IMG;
        txtbox_bg.img_id=s_ebook_back_img[s_ebook_bg_id-MMIEBOOK_BG_4];/*lint !e656*/
    }
#else
    {
        txtbox_bg.bg_type = GUI_BG_COLOR;
        txtbox_bg.color   = s_ebook_back_color[backcolor_id]; 
    }
#endif
    GUILABEL_SetBg(ctrl_id,&txtbox_bg);
    GUILABEL_SetFont(ctrl_id,MMIEBOOK_ConvertFontSize(size_id), s_ebook_color[color_id]);
    GUILABEL_SetText(ctrl_id, &text, is_update);     

    return MMI_RESULT_TRUE;
 }

 /*****************************************************************************/
 //  Description : HandleSetColorCtrlMsg
 //  Global resource dependence : 
 //  Author:   zhaohui
 //  Note:   
 /*****************************************************************************/
 LOCAL MMI_RESULT_E HandleSetColorCtrlMsg(
                                MMI_WIN_ID_T win_id,
                                MMI_CTRL_ID_T ctrl_id
                                )
 {
     MMI_RESULT_E  recode  = MMI_RESULT_TRUE;
    
     switch(ctrl_id) 
     {
     case MMIEBOOK_BUTTON_COLOR1_FONT_CTRL:
     case MMIEBOOK_BUTTON_COLOR2_FONT_CTRL:
     case MMIEBOOK_BUTTON_COLOR3_FONT_CTRL:
     case MMIEBOOK_BUTTON_COLOR4_FONT_CTRL:
     case MMIEBOOK_BUTTON_COLOR5_FONT_CTRL:
     case MMIEBOOK_BUTTON_COLOR6_FONT_CTRL:        
         GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + s_ebook_fontcolor_id,PNULL);
         s_ebook_fontcolor_id = ctrl_id - MMIEBOOK_BUTTON_COLOR1_FONT_CTRL;         
         GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + s_ebook_fontcolor_id,ButtonOwnerDrawFun);
         break;
         
     case MMIEBOOK_BUTTON_COLOR1_BACK_CTRL:
     case MMIEBOOK_BUTTON_COLOR2_BACK_CTRL:
     case MMIEBOOK_BUTTON_COLOR3_BACK_CTRL: 
     case MMIEBOOK_BUTTON_COLOR4_BACK_CTRL:
     case MMIEBOOK_BUTTON_COLOR5_BACK_CTRL:
     case MMIEBOOK_BUTTON_COLOR6_BACK_CTRL:
         GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + s_ebook_bg_id,PNULL);
         s_ebook_bg_id = ctrl_id - MMIEBOOK_BUTTON_COLOR1_BACK_CTRL;
         GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + s_ebook_bg_id,ButtonOwnerDrawFun);
         break;

     case MMIEBOOK_BUTTON_FONT_SIZE1_CTRL:
     case MMIEBOOK_BUTTON_FONT_SIZE2_CTRL:
     case MMIEBOOK_BUTTON_FONT_SIZE3_CTRL:        
        GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_FONT_SIZE1_CTRL + s_ebook_fontsize_id,PNULL);
        s_ebook_fontsize_id = ctrl_id - MMIEBOOK_BUTTON_FONT_SIZE1_CTRL;
        GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_FONT_SIZE1_CTRL + s_ebook_fontsize_id,ButtonOwnerDrawFun);
        break;
        
     default:
         //不出现该情况
         recode = MMI_RESULT_FALSE;
         break;
     } 
#ifdef MMI_LOW_MEMORY_EBOOK
     if(MMIEBOOK_BG_3 == s_ebook_bg_id)
     {
        s_ebook_fontcolor_id = MMIEBOOK_COLOR_WHITE;
     }
     else
     {
        s_ebook_fontcolor_id = MMIEBOOK_COLOR_BLACK;
     }
#endif
     SetPreviewLabelColor(MMIEBOOK_PREVIEW_DISP_LABEL_CTRL_ID, 
        s_ebook_fontcolor_id,
        s_ebook_bg_id, 
        s_ebook_fontsize_id,
        FALSE);
     
    // There is a probability that s_ebook_fontsize_id was changed,
    // so here we force to layout the ctrl of the window again.
    GUIFORM_ForceLayout(MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID);

    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
    return recode;
 }

 /*****************************************************************************/
//  Description : 颜色设置存入NV
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetColorType(
     void
)
{
     //SCI_ASSERT((MMIEBOOK_COLOR_MAX > s_ebook_fontcolor_id));/*assert verified*/
     //SCI_ASSERT((MMIEBOOK_COLOR_MAX > s_ebook_bg_id));/*assert verified*/
     if(s_ebook_fontcolor_id>=MMIEBOOK_COLOR_MAX)
     {
     	s_ebook_fontcolor_id=MMIEBOOK_COLOR_BLACK;
     }
     if(s_ebook_bg_id>=MMIEBOOK_BG_MAX)
     {
     	s_ebook_bg_id=MMIEBOOK_BG_1;
     }
     
     //write the status of main menu animation into NV
     MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_FONT,&s_ebook_fontcolor_id);
     MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_BACK,&s_ebook_bg_id);  
     SetEbookFontSize(s_ebook_fontsize_id);

     CheckDayNightMode();
}

  /*****************************************************************************/
 //     Description : get the info of browse page methed
 // Global resource dependence : 
 //  Author: zhaohui
 // Note:
 /*****************************************************************************/
 PUBLIC void MMIAPIEBOOK_GetFontType( void )
 {
 //    BOOLEAN                  is_reserve_lastline = FALSE;    
     MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
     
     //get menu animation info from NV
     MMINV_READ(MMINV_EBOOK_COLOR_FOR_FONT,&s_ebook_fontcolor_id,return_value);
     if (MN_RETURN_SUCCESS != return_value)
     {
        s_ebook_fontcolor_id = MMIEBOOK_COLOR_BLACK;
         MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_FONT,&s_ebook_fontcolor_id);
     }

     //get menu animation info from NV
     MMINV_READ(MMINV_EBOOK_COLOR_FOR_BACK,&s_ebook_bg_id,return_value);
     if (MN_RETURN_SUCCESS != return_value)
     {
#ifdef MMI_LOW_MEMORY_EBOOK
        s_ebook_bg_id=MMIEBOOK_BG_2;
#else
        s_ebook_bg_id=MMIEBOOK_BG_4;
#endif
         MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_BACK,&s_ebook_bg_id);
     }   

     s_ebook_fontsize_id = MMIAPIEBOOK_GetFontSize();

}

 /*****************************************************************************/
//     Description : restore lcd contrast
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void RestoreLcdContrast(void)
{
    MMIAPISET_SetCurrentContrast(s_contrast);      
    MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, s_contrast);
    MMIAPISET_SetMainLCDContrast(); 
}

 /*****************************************************************************/
//     Description : set lcd contrast
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void SetLcdContrast(void)
{    
    uint32    real_contrast     =   0;
     MMIEBOOK_DAY_NIGHT_MODE_E mode = MMIEBOOK_DAY_NIGHT_MODE_MAX;    

    mode = GetDayNightMode();

    switch(mode)
    {
        case MMIEBOOK_DAY_MODE:
            real_contrast = 4;        
            MMIAPISET_SetCurrentContrast(real_contrast);      
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, real_contrast);
            MMIAPISET_SetMainLCDContrast();
            break;

        case MMIEBOOK_NIGHT_MODE:
            real_contrast = 7;        
            MMIAPISET_SetCurrentContrast(real_contrast);      
            MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, real_contrast);
            MMIAPISET_SetMainLCDContrast();
             break;

         case MMIEBOOK_DAY_NIGHT_MODE_CLOSE:
            RestoreLcdContrast();
            break;

         default:
            break;
     }         
}

 /*****************************************************************************/
//     Description : get the day night mode
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL MMIEBOOK_DAY_NIGHT_MODE_E GetDayNightMode(void)
{
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    MMIEBOOK_DAY_NIGHT_MODE_E mode = MMIEBOOK_DAY_NIGHT_MODE_CLOSE;    

     MMINV_READ(MMINV_EBOOK_DAY_NIGHT_MODE,&mode,return_value);
     if (MN_RETURN_SUCCESS != return_value)
     {
         mode = MMIEBOOK_DAY_NIGHT_MODE_CLOSE;
         MMINV_WRITE(MMINV_EBOOK_DAY_NIGHT_MODE,&mode);
     }

     return mode;
}

/*****************************************************************************/
//     Description : set the day night mode
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void SetDayNightMode(MMIEBOOK_DAY_NIGHT_MODE_E mode)
{
    MMINV_WRITE(MMINV_EBOOK_DAY_NIGHT_MODE,&mode);

    InitDayNightMode();
}

  /*****************************************************************************/
 //     Description : init the day night mode
 // Global resource dependence : 
 //  Author: paul.huang
 // Note:
 /*****************************************************************************/
 LOCAL void InitDayNightMode(void)
 {    
     GUI_BG_T txtbox_bg = {0};
     MMIEBOOK_DAY_NIGHT_MODE_E mode = MMIEBOOK_DAY_NIGHT_MODE_MAX;    

    mode = GetDayNightMode();

    switch(mode)
    {
        case MMIEBOOK_DAY_MODE:
            s_ebook_fontcolor_id = DAY_MODE_FONT_COLOR;        
            //MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_FONT,&s_ebook_fontcolor_id);
            s_ebook_bg_id = DAY_MODE_BG_ID;        
            //MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_BACK,&s_ebook_bg_id);            
            break;

        case MMIEBOOK_NIGHT_MODE:
             s_ebook_fontcolor_id = NIGHT_MODE_FONT_COLOR;        
             //MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_FONT,&s_ebook_fontcolor_id);
             s_ebook_bg_id = NIGHT_MODE_BG_ID;        
             //MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_BACK,&s_ebook_bg_id);             
             break;

         case MMIEBOOK_DAY_NIGHT_MODE_CLOSE:
            MMIAPIEBOOK_GetFontType();
            break;

         default:
            break;
     }     

     GUITEXT_SetFont(MMIEBOOK_TXT_SHOW_CTRL_ID,PNULL,&s_ebook_color[s_ebook_fontcolor_id]);/*lint !e605*/
#ifndef MMI_LOW_MEMORY_EBOOK           
     if(s_ebook_bg_id<=MMIEBOOK_BG_3)
     {
         txtbox_bg.bg_type = GUI_BG_COLOR;
         txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
     }
     else                    
     {
         txtbox_bg.bg_type = GUI_BG_IMG;
         txtbox_bg.img_id=s_ebook_back_img[s_ebook_bg_id-MMIEBOOK_BG_4];/*lint !e656*/ 
     }
#else
     {
         txtbox_bg.bg_type = GUI_BG_COLOR;
         txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
     }
#endif
     GUITEXT_SetBg(MMIEBOOK_TXT_SHOW_CTRL_ID,&txtbox_bg);
 }

 /*****************************************************************************/
//     Description : check the day night mode
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL void CheckDayNightMode(void)
{
    MMIEBOOK_DAY_NIGHT_MODE_E mode = MMIEBOOK_DAY_NIGHT_MODE_MAX;    

    mode = GetDayNightMode();

    if(MMIEBOOK_DAY_MODE == mode)
    {
        if(DAY_MODE_FONT_COLOR != s_ebook_fontcolor_id || DAY_MODE_BG_ID != s_ebook_bg_id)
        {
            mode = MMIEBOOK_DAY_NIGHT_MODE_CLOSE;
            SetDayNightMode(mode);
        }
    }
    else if(MMIEBOOK_NIGHT_MODE == mode)
    {
        if(NIGHT_MODE_FONT_COLOR != s_ebook_fontcolor_id || NIGHT_MODE_BG_ID != s_ebook_bg_id)
        {
            mode = MMIEBOOK_DAY_NIGHT_MODE_CLOSE;
            SetDayNightMode(mode);
        }
    }     
}

#ifdef TTS_SUPPORT
/*****************************************************************************/
//     Description : get the info of TTS setting status
// Global resource dependence : 
//  Author: barry.peng
// Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIEBOOK_GetTTSInfo( void )
{
    BOOLEAN                    is_tts_open = FALSE;    
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get TTS info from NV
    MMINV_READ(MMINV_EBOOK_TTS_FLAG,&is_tts_open,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_tts_open =FALSE;
        MMINV_WRITE(MMINV_EBOOK_TTS_FLAG,&is_tts_open);
    }
    
    return (is_tts_open);
}
/*****************************************************************************/
//     Description : set the info of TTS setting status
// Global resource dependence : 
//  Author: barry.peng
// Note:
/*****************************************************************************/
LOCAL void MMIEBOOK_SetTTSInfo(
                               MMI_CTRL_ID_T      ctrl_id
                               )
{
    uint16      cur_selection = 0;
    BOOLEAN     is_tts_open = FALSE;
    
    //get the selected item
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
    // set the global virable
    if ( 0 == cur_selection)
    {
        is_tts_open = TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIEBOOK_SetTTSInfo cur_selection = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4072_112_2_18_2_15_33_79,(uint8*)"d",cur_selection);
        is_tts_open = FALSE;
    }
    
    //write the status of TTS into NV
    MMINV_WRITE(MMINV_EBOOK_TTS_FLAG,&is_tts_open);
}

/*****************************************************************************/
//     Description : get the info of TTS setting status
// Global resource dependence : 
//  Author: xingdong.li
// Note:
/*****************************************************************************/
#ifdef EBOOK_TTS_SELF_VOLUME  
LOCAL uint8 MMIEBOOK_GetTTSVol( void )
{
    uint8  tts_vol = MMISET_VOL_SIX;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
    
    //get TTS info from NV
    MMINV_READ(MMINV_EBOOK_TTS_VOL,&tts_vol,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        tts_vol = MMISET_VOL_SIX;
        MMINV_WRITE(MMINV_EBOOK_TTS_VOL,&tts_vol);
    }
    
    return (tts_vol);
}

/*****************************************************************************/
//     Description : set the info of TTS setting status
// Global resource dependence : 
//  Author: xingdong.li
// Note:
/*****************************************************************************/
LOCAL void MMIEBOOK_SetTTSVol(
                              uint8 vol
                              )
{   
    if(vol <= MMISET_VOL_MAX)
    {
        MMINV_WRITE(MMINV_EBOOK_TTS_VOL,&vol);
    }
}
#endif
/*****************************************************************************/
//     Description : add set tts list item
// Global resource dependence : 
//  Author:
// Note:
/*****************************************************************************/
LOCAL void MMIEBOOK_AppendTTSListItem(
                                      MMI_CTRL_ID_T      ctrl_id
                                      )
{
    uint16                      cur_selection = 0;
    BOOLEAN                     is_tts_open = FALSE;
    
    MMIAPISET_AppendListItemByTextId( TXT_OPEN, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO ); 
    //get current tts setting info
    is_tts_open = MMIEBOOK_GetTTSInfo();
    //the status is open
    if (is_tts_open)
    {
        cur_selection = 0;
    }
    else
    {
        cur_selection = 1;
    }
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);    
}
#endif

/*****************************************************************************/
//  Description : MMIAPIEBOOK_IS_WaitingWinOpen 
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEBOOK_IS_WaitingWinOpen(void)
{
    return MMK_IsOpenWin(MMIEBOOK_OPEN_WAIT_WIN_ID);
}

/*****************************************************************************/
//  Description : return s_ebook_file_list_update
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SetFilelistupdate(BOOLEAN param)
{
    s_ebook_file_list_update = param;
}

/*****************************************************************************/
//  Description : CloseEbookOpenWaitWin
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void CloseEbookOpenWaitWin( void )  //关闭打开电子书时的等待窗口
{
    MMI_WIN_ID_T query_win_id = MMIEBOOK_OPEN_WAIT_WIN_ID;
    MMIPUB_CloseQuerytWin( &query_win_id );
}

//娱乐-电子书通过调用fmm接口打开资源管理器方式;最近阅读的10本电子书列表功能;begin
/*****************************************************************************/
//  Description : 调用fmm的接口打开资源管理器(过滤出电子书文件,可自定义软键栏)
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
PUBLIC void MMIEBOOK_OpenExplorerWinforEbook( BOOLEAN is_has_read_ebook,BOOLEAN is_need_privacy_protection ) 
{
    MMIFMM_OPENWIN_INFO_T openwin_info = {0};
    wchar path_name[MMIEBOOK_FULLNAME_MAX_LEN + 1] = {0};
    uint16 path_name_len = MMIEBOOK_FULLNAME_MAX_LEN;
    wchar file_name[MMIEBOOK_FULLNAME_MAX_LEN + 1] = {0};
    uint16 file_name_len = 0;

    if (is_has_read_ebook)/*之前打开过电子书,则打开该电子书所在的路径*/
    {
         MMIEBOOK_GetLastOpenedBookPath(path_name, &path_name_len);  
    }
    else//进入资源管理器电子书目录
    {
        MMIEBOOK_GetFileFullPathUCS2(
			g_ebook_all_dev_file_brief.dev_tab[0],//SD卡优先
			MMIMULTIM_DIR_EBOOK, 
			MMIAPICOM_Wstrlen(MMIMULTIM_DIR_EBOOK),	
			PNULL,
			0,
			path_name,
			&path_name_len
			);
    }

    openwin_info.path_ptr = path_name;
    openwin_info.path_len = path_name_len;
    openwin_info.filename = file_name;
    openwin_info.file_len = file_name_len;
    openwin_info.is_need_display = FALSE;
    openwin_info.find_suffix_type = MMIFMM_TXT_ALL;//MMIFMM_TXT_ALL;
#ifdef DRM_SUPPORT
    openwin_info.find_suffix_type |= MMIFMM_DRM_ALL;
#endif
	openwin_info.sk_text_id.leftsoft_id = STXT_OPTION;//选项
    openwin_info.sk_text_id.middlesoft_id = TXT_NULL;//TXT_COMM_OPEN;//打开
    openwin_info.sk_text_id.rightsoft_id = STXT_RETURN;//返回
    openwin_info.callback.callback_ok = MMIEBOOK_OpenPopmenuCallBack;
    openwin_info.callback.callback_web = PNULL;
    openwin_info.callback.callback_can = PNULL;
    openwin_info.privacy_protect = !is_need_privacy_protection;  
    MMIAPIFMM_OpenExplorerWin(&openwin_info);
}

/*****************************************************************************/
//  Description : 关闭电子书入口窗口
//  Global resource dependence : 
//  Author: zhaohui
//  Note:   
/*****************************************************************************/
LOCAL void CloseEnterEbookWin( void ) 
{
#ifdef MMI_TASK_MANAGER
    //	 MMK_CloseWin(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID);
    if(s_ebook_applet_instance != PNULL)
    {
        MMK_CloseWin(MMK_GetWinHandle(s_ebook_applet_instance->caf_applet.app_handle, MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID));
    }
    else
    {
        //MMK_CloseWin(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID);//MMK_CloseWin(MMK_GetWinHandle(SPRD_EBOOK_APPLET_ID, MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID));
        MMK_CloseWin(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID);
    }
#else
    MMK_CloseWin(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID);
#endif
}

/*****************************************************************************/
//  Description : Enter txt or list
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void EnterTxtorList( BOOLEAN is_need_privacy_protection )//娱乐-电子书的入口
{
#ifdef MMIEBOOK_BOOKSHELF_SUPPORT
    ADD_DATA param=(uint32)is_need_privacy_protection;/*lint !e64*/
    
#ifndef MMI_TASK_MANAGER
    MMK_CreateWin((uint32*)MMIEBOOK_LATEST_LIST_WIN_TAB, param); 
#else    
    MMI_APPLET_START_T start ={0};
    
    //SCI_TRACE_LOW:"EnterTxtorList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4257_112_2_18_2_15_33_80,(uint8*)"");
    start.guid = SPRD_EBOOK_APPLET_ID;
    start.state = MMI_APPLET_STATE_BG_HIDE_WIN|MMI_APPLET_STATE_TASK_MANAGER;
    start.param_ptr = param;
    start.size_of_param= sizeof(uint32);

    MMK_StartApplet( &start );
#endif/*MMI_TASK_MANAGER*/    

#else/*MMI_PDA_SUPPORT*/
    //SCI_TRACE_LOW:"MMIEBOOK_WINTAB EnterTxtorList enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4267_112_2_18_2_15_33_81,(uint8*)"");
#if defined( MMI_GUI_STYLE_MINISCREEN)  
    MMIEBOOK_OpenExplorerWinforEbook(s_is_has_read, is_need_privacy_protection);//从娱乐电子书入口,打开fmm前没有阅读过电子书
#else
    MMIEBOOK_OpenExplorerWinforEbook(FALSE, is_need_privacy_protection);//从娱乐电子书入口,打开fmm前没有阅读过电子书
#endif
#endif/*MMI_PDA_SUPPORT*/
}

PUBLIC MMI_HANDLE_T MMIEBOOK_OpenTxtContentWin(ADD_DATA add_data_ptr)
{
    return MMK_CreateEbookWin((uint32*)MMIEBOOK_ENTER_TXT_CONTENT_WIN_TAB, add_data_ptr);//资源管理器中入口
}
/*****************************************************************************/
//  Description : Enter txt or list
//  Global resource dependence : 
//  Author: zhaohui
//  Note:
//  Return:
/*****************************************************************************/
LOCAL void OpenFmmWinFromShowWin( void )//若fmm窗口未打开，退出电子书阅读窗口时应打开该fmm窗口
{
    if (!MMIAPIFMM_IsFmmMainWinOpen())//从娱乐-电子书进入阅读，册退出时需返回电子书列表   
    {
        //打开列表窗口，以便列出文件
        MMIEBOOK_OpenExplorerWinforEbook(TRUE,FALSE);//之前打开过电子书
    }   
}

#ifdef TTS_SUPPORT
PUBLIC MMI_HANDLE_T MMIEBOOK_OpenTTSOpenCloseWin(ADD_DATA add_data_ptr)
{
    return MMK_CreatePubListWin((uint32*)MMIEBOOK_SET_TTS_OPEN_CLOSE_WIN_TAB, add_data_ptr);  
}
/*****************************************************************************/
// 	Description : to handle set TTS
//	Global resource dependence : 
//  Author:
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEbookSetTTSOpenCloseWindow(
                                                     MMI_WIN_ID_T    win_id, 
                                                     MMI_MESSAGE_ID_E   msg_id, 
                                                     DPARAM             param
                                                     )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    //SCI_TRACE_LOW:"MMIEBOOK_WINTAB HandleEbookSetTTSOpenCloseWindow msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4306_112_2_18_2_15_33_82,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIEBOOK_SET_TTS_CTRL_ID,MMIEBOOK_TTS_ITEM_MAX_NUM, FALSE );
        MMIEBOOK_AppendTTSListItem(MMIEBOOK_SET_TTS_CTRL_ID);        
        MMK_SetAtvCtrl(win_id,MMIEBOOK_SET_TTS_CTRL_ID);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (!GUILIST_IsCurrItemInvalid(MMIEBOOK_SET_TTS_CTRL_ID))
        {
            MMIEBOOK_SetTTSInfo(MMIEBOOK_SET_TTS_CTRL_ID); 
            //success!
            //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
            
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
#ifdef EBOOK_TTS_SELF_VOLUME  
PUBLIC MMI_HANDLE_T MMIEBOOK_OpenTTSVolumeSetWin(ADD_DATA add_data_ptr)
{
    return MMK_CreateWin((uint32*)MMIEBOOK_SET_TTS_VOL_WIN_TAB, add_data_ptr);  
}

/*****************************************************************************/
//Description : set tts vol
//Global resource dependence : 
//Author: 
//Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEbookTTSVolSetWindow(
                                               MMI_WIN_ID_T    win_id, 
                                               MMI_MESSAGE_ID_E   msg_id, 
                                               DPARAM             param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    static	uint8   s_cur_vol = 0;
    GUI_RECT_T      image_rect = MMITHEME_GetClientRect();
	uint8           adj_vol = 0;
	GUI_POINT_T	    point = {0};
        
    //SCI_TRACE_LOW:"MMIEBOOK_WINTAB HandleEbookTTSVolSetWindow msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4362_112_2_18_2_15_33_83,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        s_cur_vol = MMIEBOOK_GetTTSVol();
        break;
        
    case MSG_FULL_PAINT:	
        GUIRES_DisplayImg(PNULL,
            &image_rect,
            &image_rect,
            win_id,
            IMAGE_COMMON_BG,
            MMITHEME_GetDefaultLcdDev());
        MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
        break;
        
    case MSG_APP_LEFT:
    case MSG_APP_DOWN:
    case MSG_KEYDOWN_DOWNSIDE:
        if (MMISET_VOL_ZERO < s_cur_vol)
        {
            s_cur_vol--;			   
        }
#ifdef MMI_PDA_SUPPORT
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#else
        MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#endif
        break;
        
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_KEYDOWN_UPSIDE:
        if (MMISET_VOL_MAX > s_cur_vol)
        {
            s_cur_vol++;
        }
#ifdef MMI_PDA_SUPPORT
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#else
        MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#endif
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    //case MSG_APP_MENU:
        MMIEBOOK_SetTTSVol(s_cur_vol);
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id );
        break;   
#ifdef MMI_PDA_SUPPORT  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;

            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527 
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);	
        adj_vol = MMIAPISET_TPAdjustVolume(point, s_cur_vol, MMISET_VOL_MAX, win_id);
        if (adj_vol != s_cur_vol)
        {
            s_cur_vol = adj_vol;
#ifdef MMI_PDA_SUPPORT
            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
#else
            MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#endif
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    default:
        recode = MMI_RESULT_FALSE;
        break;
    } //end switch
    
    return recode;
}
#endif
#endif


/*****************************************************************************/
// 	Description : ebook restore factory setting 
//	Global resource dependence : 
//  Author: jian.ma
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEBOOK_RestoreFactorySetting(void)
{  
     MMIEBOOK_SET_CODING_TYPE_E     coding_type = MMIEBOOK_CODING_TYPE_GB;   
     MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
     BOOLEAN flag = TRUE;
     MMIEBOOK_DAY_NIGHT_MODE_E mode = MMIEBOOK_DAY_NIGHT_MODE_CLOSE;
        
     MMIAPISET_GetLanguageType(&lang_type);
     
#if (defined IM_TRAD_CHINESE_SUPPORT) || (defined MMI_DISPLAY_TRAD_CHINESE_SUPPORT)
     if( MMISET_LANGUAGE_TRAD_CHINESE == lang_type )
     {
         coding_type = MMIEBOOK_CODING_TYPE_BIG5; 
     }
     else
#endif
     {
         coding_type = MMIEBOOK_CODING_TYPE_GB; 
     }
     MMINV_WRITE(MMINV_EBOOK_CODING_TYPE,&coding_type);

     s_ebook_fontcolor_id = MMIEBOOK_COLOR_BLACK;
    
     MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_FONT,&s_ebook_fontcolor_id);
     
#ifdef MMI_LOW_MEMORY_EBOOK
     s_ebook_bg_id=MMIEBOOK_BG_2;
#else
     s_ebook_bg_id=MMIEBOOK_BG_4;
#endif
     MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_BACK,&s_ebook_bg_id);

     s_ebook_fontsize_id = MMIEBOOK_FONT_NORMAL;
     SetEbookFontSize(s_ebook_fontsize_id);
     
     flag = FALSE;
     MMINV_WRITE(MMINV_EBOOK_IS_FULL_SCREEN_OFF,&flag);

     MMINV_WRITE(MMINV_EBOOK_DAY_NIGHT_MODE,&mode);

     return TRUE;
}
/*****************************************************************************/
//  Description : check whether file is exist
//  Global resource dependence : 
//  Author:liqing.peng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_IsFileExist(
                            const wchar *full_path_ptr,     //in
                            uint16       full_path_len   ,    //in, 双字节为单位
                            uint32      file_size
                            
                            )
{
    BOOLEAN         result = FALSE;
    SFS_HANDLE      sfs_handle = 0;
    SFS_FIND_DATA_T find_data = {0};

    //SCI_ASSERT(NULL != full_path_ptr);/*assert verified*/
    //SCI_ASSERT(0 < full_path_len);/*assert verified*/
    SCI_TRACE_LOW("ebook_time:MMIEBOOK_IsFileExist  start=%d",SCI_GetTickCount());
    if(PNULL == full_path_ptr ||0 == MMIAPICOM_Wstrlen(full_path_ptr) )
    {
        result = FALSE;
    }
    else
    {
        sfs_handle = MMIAPIFMM_FindFirstFile(full_path_ptr, full_path_len,&find_data);
        //SCI_TRACE_LOW:"MMIAPIFMM_IsFileExist: find first sfs_handle = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4532_112_2_18_2_15_33_84,(uint8*)"d", sfs_handle);

        if(SFS_INVALID_HANDLE == sfs_handle)			
        {
        	return FALSE;			
        }
#ifdef DRM_SUPPORT            
        if(MMIAPIDRM_IsDRMFile(SFS_INVALID_HANDLE, full_path_ptr))
        {
            MMIFILE_HANDLE file_handle = SFS_INVALID_HANDLE;
            file_handle = MMIAPIFMM_CreateFile(full_path_ptr,SFS_MODE_OPEN_EXISTING|SFS_MODE_READ, 0, 0);     
            find_data.length = MMIAPIFMM_GetFileSize(file_handle);
            //when drm rights is invalid ,file size return zero
            if(0 == find_data.length)
            {
                find_data.length = file_size;
            }
             MMIAPIFMM_CloseFile(file_handle);
        }
#endif
        
        if((!(SFS_ATTR_DIR&find_data.attr))&&(find_data.length==file_size))
        {              
            result = TRUE;
        }

        SFS_FindClose(sfs_handle);
    }
    SCI_TRACE_LOW("ebook_time:MMIEBOOK_IsFileExist  end=%d",SCI_GetTickCount());
    //SCI_TRACE_LOW:"MMIAPIFMM_IsFileExist: result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4562_112_2_18_2_15_34_85,(uint8*)"d", result);

    return result;
}
/*****************************************************************************/
// 	Description : ebook restore factory setting 
//	Global resource dependence : 
//  Author: jian.ma
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_WaitExit(void)
{
    CloseAllEbookWin();
}
/*****************************************************************************/
//  Description : privacy call back of ebook module 
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_ValidatePrivacyAppEntry(void)
{ 
    MMIAPIEBOOK_ManagerWinMsg(FALSE);
}
/*****************************************************************************/
//     Description : 获得字体大小
// Global resource dependence : 
//  Author: zhaohui
// Note:
/*****************************************************************************/
PUBLIC MMIEBOOK_FONT_SIZE_E MMIAPIEBOOK_GetFontSize( void )
{
    MMIEBOOK_FONT_SIZE_E        font_size = MMIEBOOK_FONT_NORMAL;    
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
 
    //get menu animation info from NV
    MMINV_READ(MMINV_EBOOK_FONT_SIZE,&font_size,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
         font_size = MMIEBOOK_FONT_NORMAL;

         MMINV_WRITE(MMINV_EBOOK_FONT_SIZE,&font_size);
    }

    return (font_size);
}
/*****************************************************************************/
//  Description : 设置电子书字体大小
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void SetEbookFontSize(MMIEBOOK_FONT_SIZE_E font_size)
{
    if (font_size>=MMIEBOOK_FONT_SIZE_MAX)
    {
        //SCI_TRACE_LOW:"MMIEbook: SetEbookFontSize font_size=%d,"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4610_112_2_18_2_15_34_86,(uint8*)"d",font_size);
        font_size = MMIEBOOK_FONT_NORMAL;
    }
    //write the status of main menu animation into NV
    MMINV_WRITE(MMINV_EBOOK_FONT_SIZE,&font_size);
}

/*****************************************************************************/
 //     Description : 读取是否全屏显示
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
/*****************************************************************************/
 PUBLIC void MMIAPIEBOOK_InitIsFullScreenOff( void )
 {    
     MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
     
     MMINV_READ(MMINV_EBOOK_IS_FULL_SCREEN_OFF,&s_is_full_screen_off,return_value);
     if (MN_RETURN_SUCCESS != return_value)
     {
         s_is_full_screen_off = FALSE;
         MMINV_WRITE(MMINV_EBOOK_IS_FULL_SCREEN_OFF,&s_is_full_screen_off);
     }
}
/*****************************************************************************/
 //     Description : 设置是否全屏显示
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
/*****************************************************************************/
LOCAL void SetIsFullScreenOff(BOOLEAN is_off)
{ 
	s_is_full_screen_off=is_off;
	MMINV_WRITE(MMINV_EBOOK_IS_FULL_SCREEN_OFF,&s_is_full_screen_off);
}
/*****************************************************************************/
 //     Description : 获取是否全屏显示
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetIsFullScreenOff(void)
{ 
    //SCI_TRACE_LOW:"MMIEBOOK_TASK MMIEBOOK_GetIsFullScreenOff not fullsecreen == %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_4647_112_2_18_2_15_34_87,(uint8*)"d", s_is_full_screen_off);
	return s_is_full_screen_off;
}

/*****************************************************************************/
 //     Description : 
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SetIsTxtShowWinCtrlvisible(BOOLEAN is_visible)
{ 
	MMI_CTRL_ID_T   ctrl_id = 0;
	ctrl_id = MMK_GetWinSoftkeyCtrlId(MMIEBOOK_TXT_SHOW_WIN_ID);
    
      GUIWIN_SetSoftkeyVisible(MMIEBOOK_TXT_SHOW_WIN_ID,is_visible);
}
//自动浏览
/*****************************************************************************/
//  Description : 自动浏览设置界面
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSetAutoRead(
                                        MMI_WIN_ID_T       win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIEBOOK_SET_AUTO_READ_CTRL_ID,MMIEBOOK_AUTO_READ_MAX, FALSE );
        AppendAutoReadListItem(MMIEBOOK_SET_AUTO_READ_CTRL_ID);
        MMK_SetAtvCtrl(win_id,MMIEBOOK_SET_AUTO_READ_CTRL_ID);
        break;
        
    case MSG_CTL_MIDSK:       
    case MSG_APP_WEB:    
    case MSG_APP_OK:
    case MSG_CTL_OK:
        SetAutoReadType(MMIEBOOK_SET_AUTO_READ_CTRL_ID);    
        //success!
        //MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        MMK_CloseWin( win_id ); 
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef MMI_PDA_SUPPORT
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            }
#endif     
            MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 字体大小
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFontSizeWinMsg(
                                        MMI_WIN_ID_T       win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id = MMIEBOOK_SET_FONT_SIZE_CTRL_ID;
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
    {
        uint32 font_size = MMIAPIEBOOK_GetFontSize();
        GUILIST_ITEM_T      item_t      =   {0};/*lint !e64*/
        GUILIST_ITEM_DATA_T item_data   =   {0};/*lint !e64*/
        
        GUILIST_SetMaxItem(ctrl_id, MMIEBOOK_FONT_SIZE_MAX, FALSE );
        
        item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
        item_t.item_data_ptr = &item_data;            
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        
        item_data.item_content[0].item_data.text_id = TXT_COMM_FONT_BIG; 
        GUILIST_AppendItem( ctrl_id, &item_t );    
        item_data.item_content[0].item_data.text_id = TXT_COMMON_NORMAL; 
        GUILIST_AppendItem( ctrl_id, &item_t );    
        item_data.item_content[0].item_data.text_id = TXT_FONT_SMALL; 
        GUILIST_AppendItem( ctrl_id, &item_t );              
      

        GUILIST_SetSelectedItem(ctrl_id, font_size, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id,font_size);  
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
    }
        break;
        
    case MSG_CTL_MIDSK:       
    case MSG_APP_WEB:    
    case MSG_APP_OK:
    case MSG_CTL_OK:
        //success!
        {
            uint16 cur_selection = 0;
            GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
            SetEbookFontSize(cur_selection);/*lint !e64*/
            MMK_CloseWin( win_id );
            MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_FONT_SIZE_CHANGE, g_ebook_task_id, PNULL);
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id_penok = ((MMI_NOTIFY_T*) param)->src_id;

            if( MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ctrl_id_penok)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            }
    
            MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void AppendAutoReadListItem(
     MMI_CTRL_ID_T      ctrl_id
     )
{
     uint16                      cur_selection = 0;
     MMIEBOOK_SET_AUTO_READ_E     type = MMIEBOOK_AUTO_READ_OFF;    
     
     MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
     MMIAPISET_AppendListItemByTextId( TXT_EBOOK_TURN_PAGE_5S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
     MMIAPISET_AppendListItemByTextId( TXT_EBOOK_TURN_PAGE_15S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
     MMIAPISET_AppendListItemByTextId( TXT_EBOOK_TURN_PAGE_30S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
     MMIAPISET_AppendListItemByTextId( TXT_EBOOK_TURN_PAGE_60S, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        
     type = MMIAPIEBOOK_GetAutoReadType();
     
     cur_selection = type;

     GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
     GUILIST_SetCurItemIndex(ctrl_id,cur_selection);         
}

/*****************************************************************************/
//     Description : 获得自动浏览方式
// Global resource dependence : 
//  Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC MMIEBOOK_SET_AUTO_READ_E MMIAPIEBOOK_GetAutoReadType( void )
{
    MMIEBOOK_SET_AUTO_READ_E     type = MMIEBOOK_AUTO_READ_OFF;    
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;

    //get menu animation info from NV
    MMINV_READ(MMINV_EBOOK_AUTO_READ_TYPE,&type,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        type = MMIEBOOK_AUTO_READ_OFF; 
        MMINV_WRITE(MMINV_EBOOK_AUTO_READ_TYPE,&type);
    }

    return (type);
}

/*****************************************************************************/
//  Description : 设置自动浏览类型
//  Global resource dependence : 
//  Author:zhaohui
//  Note:
/*****************************************************************************/
LOCAL void SetAutoReadType(
                         MMI_CTRL_ID_T      ctrl_id
                         )
{
    uint16      cur_selection = 0;
    MMIEBOOK_SET_AUTO_READ_E     type = MMIEBOOK_AUTO_READ_OFF;  
    
    GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
    
    switch (cur_selection)
    {
    case MMIEBOOK_AUTO_READ_5S:
        type = MMIEBOOK_AUTO_READ_5S;
        break;
	case MMIEBOOK_AUTO_READ_15S:
        type = MMIEBOOK_AUTO_READ_15S;
        break;
	case MMIEBOOK_AUTO_READ_30S:
        type = MMIEBOOK_AUTO_READ_30S;
        break;
	case MMIEBOOK_AUTO_READ_60S:
        type = MMIEBOOK_AUTO_READ_60S;
        break;
    default:
        type = MMIEBOOK_AUTO_READ_OFF;
        break;
    }
    
    MMINV_WRITE(MMINV_EBOOK_AUTO_READ_TYPE,&type);
}

/*****************************************************************************/
//  Description : 获得自动浏览时间
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint32 GetAutoReadTime(void)
{  
    uint32 time=0;

    MMIEBOOK_SET_AUTO_READ_E type=MMIAPIEBOOK_GetAutoReadType();

    switch (type)
    {
        case MMIEBOOK_AUTO_READ_5S:
            time = 5000;
            break;
        case MMIEBOOK_AUTO_READ_15S:
            time = 15000;
            break;
        case MMIEBOOK_AUTO_READ_30S:
            time = 30000;
            break;
        case MMIEBOOK_AUTO_READ_60S:
            time = 60000;
            break;
        default:
            time = 5000;
            break;
    }

    return time;
}

/*****************************************************************************/
// 	Description : Start timer 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void EbookStartTimer(
                           uint8 *timer_id,
                           uint32 time_out,
                           BOOLEAN is_period
                           )
{
    //SCI_ASSERT(timer_id != NULL);/*assert verified*/
    if(NULL ==timer_id)
    {
        return;
    }
    
    if (0 != *timer_id)
    {
        MMK_StopTimer(*timer_id);
        *timer_id = 0;
    }
    *timer_id = MMK_CreateTimer(time_out, is_period);
}

/*****************************************************************************/
// 	Description : close timer 
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void EbookCloseTimer(
                           uint8 *timer_id
                           )
{
    //SCI_ASSERT(timer_id != NULL);/*assert verified*/
    if(NULL == timer_id)
    {
        return;
    }
    
    if (0 != *timer_id)
    {
        MMK_StopTimer(*timer_id);
        *timer_id = 0;
    }
}

/*****************************************************************************/
//     Description : 
// Global resource dependence : 
//  Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEBOOK_GetIsNeedCloseTimer( void )
{
	return s_is_need_close_timer;
}

/*****************************************************************************/
//     Description : 
// Global resource dependence : 
//  Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_SetIsNeedCloseTimer( BOOLEAN is_need_close )
{
	 s_is_need_close_timer=is_need_close;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void SetTxtBoxRect(void)
{
    GUI_BOTH_RECT_T     both_rect = {0};
    GUI_BOTH_RECT_T     both_client_rect = MMITHEME_GetWinClientBothRect(MMIEBOOK_TXT_SHOW_WIN_ID);
    GUI_BOTH_RECT_T     full_both_client_rect = MMITHEME_GetFullScreenBothRect();
    // 	GUI_BG_T  bg={0};

    if(s_is_full_screen_off)
    {
#ifdef  MMIEBOOK_OSDMENU_SUPPORT
        both_rect.h_rect.left = both_rect.v_rect.left = 0;
        both_rect.h_rect.top = both_rect.v_rect.top = 0;
        both_rect.h_rect.bottom = (both_client_rect.h_rect.bottom - MMIEBOOK_COUNT_BAR_HEIGHT);
        both_rect.h_rect.right = both_client_rect.h_rect.right;
        both_rect.v_rect.bottom = (both_client_rect.v_rect.bottom - MMIEBOOK_COUNT_BAR_HEIGHT);
        both_rect.v_rect.right = both_client_rect.v_rect.right;
        GUIAPICTRL_SetBothRect(MMIEBOOK_TXT_SHOW_CTRL_ID,&both_rect);//set textbox rect 
#else        
        GUIAPICTRL_SetBothRect(MMIEBOOK_TXT_SHOW_CTRL_ID,&both_client_rect);
#endif
    }
    else
    {
        both_rect.h_rect.left = both_rect.v_rect.left = 0;
        both_rect.h_rect.top = both_rect.v_rect.top = 0;
        both_rect.h_rect.bottom = full_both_client_rect.h_rect.bottom ;
        both_rect.h_rect.right = full_both_client_rect.h_rect.right;
        both_rect.v_rect.bottom = full_both_client_rect.v_rect.bottom;
        both_rect.v_rect.right = full_both_client_rect.v_rect.right;
        GUIAPICTRL_SetBothRect(MMIEBOOK_TXT_SHOW_CTRL_ID,&both_rect);//set textbox rect 
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleImportVcardWaitingWinMsg(
                                        MMI_WIN_ID_T win_id,            
                                        MMI_MESSAGE_ID_E msg_id,        
                                        DPARAM param
                                        )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        MMIAPIPB_AbortPB();
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFileErrorAlertWinMsg(
                                        MMI_WIN_ID_T win_id,            
                                        MMI_MESSAGE_ID_E msg_id,        
                                        DPARAM param
                                        )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_CLOSE_WINDOW:
        CloseEnterEbookWin();
#ifndef MMI_PDA_SUPPORT
        OpenFmmWinFromShowWin();
#endif
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;

        default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return recode;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Note: 
/*****************************************************************************/
LOCAL void  HandleLCDSwitchMsg(MMI_WIN_ID_T win_id)
{
    MMI_STRING_T    mmi_string = {0};
#ifdef TTS_SUPPORT
    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
#endif

    EbookCloseTimer(&s_auto_read_timer_id);
    //GUITEXT_SetString(MMIEBOOK_TXT_SHOW_CTRL_ID, mmi_string.wstr_ptr,mmi_string.wstr_len,FALSE);
    MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_SCREEN_SWITCH ,g_ebook_task_id, PNULL);

    //获得要显示的字符串
    MMIEBOOK_GetShowTxt(&mmi_string);

    //显示该页字符串内容
    GUITEXT_SetString(MMIEBOOK_TXT_SHOW_CTRL_ID, mmi_string.wstr_ptr,mmi_string.wstr_len,FALSE);

    if(MMK_IsFocusWin(win_id))
    {
#ifdef TTS_SUPPORT
        if(s_tts_auto_pagedown_flag)
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
            MMK_SendMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_TTS_PLAY,PNULL);
        }
        else
#endif
        {
            if(MMIEBOOK_AUTO_READ_OFF!=MMIAPIEBOOK_GetAutoReadType())
            {
                EbookStartTimer(&s_auto_read_timer_id, GetAutoReadTime(), FALSE);
            }
        }
    }
}

/*****************************************************************************/
// 	Description : ebook set  TTS On or OFF
//	Global resource dependence : 
//  Author: koki.gao
//	Note:   
/*****************************************************************************/
#ifdef TTS_SUPPORT
PUBLIC void MMIAPIEBOOK_SetTTSOnOFF(BOOLEAN isrun)
{
     s_tts_auto_pagedown_flag=isrun;
     //write the status of TTS into NV
     MMINV_WRITE(MMINV_EBOOK_TTS_FLAG,&isrun);
}
#endif
#ifdef MMIEBOOK_OSDMENU_SUPPORT
/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateOSDMenu(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T rect={0};
    uint16 lcd_width = 0; 
    uint16 lcd_height = 0; 
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    rect.left=0;
    rect.right=lcd_width;
    rect.top=lcd_height-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN;
    rect.bottom=rect.top+MMIEBOOK_OSD_LABEL_HEIGHT_DOWN -1;
    
    DestroyOSDMenu();
    CreateOSDLayer(win_id,&s_osd_dev);
#ifdef TOUCH_PANEL_SUPPORT
    GUIRES_DisplayImg(PNULL, &rect, PNULL, win_id, IMAGE_EBOOK_OSD_DOWN, &s_osd_dev);
#endif
    CreateOSDCtrl(win_id);
    SetOSDCtrlDevInfo(&s_osd_dev);
    SetOSDCtrlParam();
    s_osd_is_active=TRUE;
    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
    if(0 == s_osd_display_timer_id)
    {
        s_osd_display_timer_id = MMK_CreateWinTimer( win_id, EBOOK_OSD_MENU_DISPLAY_TIME,FALSE);
    }
#if defined( MMI_GUI_STYLE_MINISCREEN)  
    MMK_SetAtvCtrl(win_id,MMIEBOOK_OSD_NAME_CTRL_ID);
#endif
}

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void DestroyOSDMenu(void)
{
    if(PNULL!=s_label_handle[0])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_TIME_CTRL_ID);
        s_label_handle[0]=PNULL;
    }
    if(PNULL!=s_label_handle[1])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_NAME_CTRL_ID);
        s_label_handle[1]=PNULL;
    }
    if(PNULL!=s_label_handle[2])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_PERCENT_CTRL_ID);
        s_label_handle[2]=PNULL;
    }
#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
    if(PNULL!=s_label_handle[3])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_DISPLAY_SET_CTRL_ID);
        s_label_handle[3]=PNULL;
    }
    if(PNULL!=s_label_handle[4])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_MORE_CTRL_ID);
        s_label_handle[4]=PNULL;
    }
#else
    if(PNULL!=s_label_handle[3])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_FONTCOLOR_CTRL_ID);
        s_label_handle[3]=PNULL;
    }
    if(PNULL!=s_label_handle[4])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_FONTSIZE_CTRL_ID);
        s_label_handle[4]=PNULL;
    }
    if(PNULL!=s_label_handle[5])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_BG_CTRL_ID);
        s_label_handle[5]=PNULL;
    }
    if(PNULL!=s_label_handle[6])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_MORE_CTRL_ID);
        s_label_handle[6]=PNULL;
    }
#endif
#else//TOUCH_PANEL_SUPPORT
if(PNULL!=s_label_handle[3])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_LSK_CTRL_ID);
        s_label_handle[3]=PNULL;
    }
    if(PNULL!=s_label_handle[4])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_RSK_CTRL_ID);
        s_label_handle[4]=PNULL;
    }
#endif
    if(s_osd_dev.block_id!=UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_osd_dev);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_osd_dev);
//        s_osd_dev.block_id=UILAYER_NULL_HANDLE;
//        s_osd_dev.lcd_id=0;
    }

    s_osd_is_active=FALSE;
    if(s_osd_display_timer_id!=0)
    {
        MMK_StopTimer(s_osd_display_timer_id);
        s_osd_display_timer_id = 0;
    }
}

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetOSDCtrlParam(void)
{
    GUI_BG_T bg={0};
    int8     time[MMISET_TIME_STR_12HOURS_LEN+1] = {0};
    MMI_STRING_T time_str= {0};
    SCI_TIME_T      sys_time = {0};
    MMIEBOOK_FILE_BRIEF_T*file_brief=PNULL;
    wchar			file_name[MMIEBOOK_TXT_NAME_MAX_LEN+1] = {0};
    uint16			file_name_len = 0;
    MMI_STRING_T str={0};
    
    bg.bg_type=GUI_BG_IMG;  
    bg.img_id=IMAGE_EBOOK_OSD_UP;

    GUILABEL_SetBg(MMIEBOOK_OSD_TIME_CTRL_ID, & bg);
    GUILABEL_SetBg(MMIEBOOK_OSD_NAME_CTRL_ID, & bg);
    GUILABEL_SetBg(MMIEBOOK_OSD_PERCENT_CTRL_ID, & bg);

#ifdef TOUCH_PANEL_SUPPORT
    bg.bg_type=GUI_BG_IMG;
#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
    bg.img_id=IMAGE_EBOOK_FONT_COLOR;
    GUILABEL_SetBg(MMIEBOOK_OSD_DISPLAY_SET_CTRL_ID, & bg);
#else
    bg.img_id=IMAGE_EBOOK_FONT_COLOR;
    GUILABEL_SetBg(MMIEBOOK_OSD_FONTCOLOR_CTRL_ID, & bg);
    bg.img_id=IMAGE_EBOOK_FONT_SIZE;
    GUILABEL_SetBg(MMIEBOOK_OSD_FONTSIZE_CTRL_ID, & bg);
    bg.img_id=IMAGE_EBOOK_BG;
    GUILABEL_SetBg(MMIEBOOK_OSD_BG_CTRL_ID, & bg);
#endif    
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        bg.img_id=IMAGE_EBOOK_MORE;
        GUILABEL_SetBg(MMIEBOOK_OSD_MORE_CTRL_ID, & bg);
    }
#endif	
#else//TOUCH_PANEL_SUPPORT
    bg.bg_type=GUI_BG_IMG;
    bg.img_id=IMAGE_EBOOK_OSD_DOWN;
    GUILABEL_SetBg(MMIEBOOK_OSD_LSK_CTRL_ID, & bg);
    GUILABEL_SetBg(MMIEBOOK_OSD_RSK_CTRL_ID, & bg);

    MMI_GetLabelTextByLang(STXT_OPTION, &str);    
    GUILABEL_SetText(MMIEBOOK_OSD_LSK_CTRL_ID, &str, FALSE);
    GUILABEL_SetFont(MMIEBOOK_OSD_LSK_CTRL_ID,MMIEBOOK_OSD_FONT,MMIEBOOK_OSD_FONT_COLOR);
    MMI_GetLabelTextByLang(STXT_RETURN, &str);    
    GUILABEL_SetText(MMIEBOOK_OSD_RSK_CTRL_ID, &str, FALSE);
    GUILABEL_SetFont(MMIEBOOK_OSD_RSK_CTRL_ID,MMIEBOOK_OSD_FONT,MMIEBOOK_OSD_FONT_COLOR);
#endif

    TM_GetSysTime(&sys_time);
    MMIAPISET_FormatTimeStrByTime(sys_time.hour, sys_time.min,time,MMISET_TIME_STR_12HOURS_LEN+1);/*lint !e64*/
    time_str.wstr_len = SCI_STRLEN((char*)time);
    time_str.wstr_ptr = SCI_ALLOC_APPZ((time_str.wstr_len +1)*sizeof(wchar));
    MMI_STRNTOWSTR(time_str.wstr_ptr,time_str.wstr_len,time,strlen((char*)time),strlen((char*)time));/*lint !e64*/
    GUILABEL_SetText(MMIEBOOK_OSD_TIME_CTRL_ID, &time_str, FALSE);
    if (PNULL != time_str.wstr_ptr)
    {
        SCI_FREE(time_str.wstr_ptr);
        time_str.wstr_ptr = PNULL;
    }

    GUILABEL_SetFont(MMIEBOOK_OSD_TIME_CTRL_ID,MMIEBOOK_OSD_FONT,MMIEBOOK_OSD_FONT_COLOR);
    file_brief= MMIEBOOK_GetFileBrief();
    MMIAPIFMM_SplitFullPath(file_brief->full_name_wstr, file_brief->full_name_len,
                        		NULL, NULL, NULL, NULL,
                        		file_name, &file_name_len);
    str.wstr_ptr=file_name;
    str.wstr_len=file_name_len;
    GUILABEL_SetText(MMIEBOOK_OSD_NAME_CTRL_ID, &str, FALSE);
    GUILABEL_SetFont(MMIEBOOK_OSD_NAME_CTRL_ID,MMIEBOOK_OSD_FONT,MMIEBOOK_OSD_FONT_COLOR);

    MMIEBOOK_ShowCurPercent(MMIEBOOK_OSD_PERCENT_CTRL_ID);
    GUILABEL_SetFont(MMIEBOOK_OSD_PERCENT_CTRL_ID,MMIEBOOK_OSD_FONT,MMIEBOOK_OSD_FONT_COLOR);

}

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void SetOSDCtrlDevInfo(GUI_LCD_DEV_INFO* osd_dev)
{
    uint32 i=0;
    GUI_LCD_DEV_INFO* dev=osd_dev;
    
    for(i=0;i<MMIEBOOK_OSD_LABEL_NUM;i++)
    {
       GUICTRL_SetLcdDevInfo(s_label_handle[i], dev); /*lint !e64*/
    }
}

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateOSDLayer(MMI_WIN_ID_T win_id,GUI_LCD_DEV_INFO* osd_dev)
{
    if (PNULL == osd_dev)
    {
        return;
    }
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
        uint16 lcd_width = 0; 
        uint16 lcd_height = 0; 
        UILAYER_APPEND_BLT_T append_layer = {0};

        if(osd_dev->block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(osd_dev);
//            UILAYER_ReleaseLayer(osd_dev);
//            osd_dev->block_id=UILAYER_NULL_HANDLE;
//            osd_dev->lcd_id=0;
        }

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
        
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = lcd_width;
        create_info.height = lcd_height;
        create_info.is_bg_layer = FALSE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(&create_info, osd_dev);

        //UILAYER_SetLayerColorKey(s_osd_dev, TRUE, UILAYER_TRANSPARENT_COLOR);
        UILAYER_Clear(osd_dev);
        
		//UILAYER_SetLayerAlpha(&s_osd_dev, 32);	
       
        append_layer.lcd_dev_info = *osd_dev;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
      
    }
}

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateOSDCtrl(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T    create = {0};
    GUILABEL_INIT_DATA_T     init_data = {0};
    uint16 lcd_width = 0; 
    uint16 lcd_height = 0; 
    uint16 img_width = 0; 
    uint16 img_height = 0; 
    uint16 osd_icon_num = 3;    
    uint16 osd_width = 0;
    uint16 osd_icon_offset = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    GUIRES_GetImgWidthHeight(&img_width, &img_height, IMAGE_EBOOK_FONT_COLOR, win_id);

#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        osd_icon_num = 2;
    }
    else
#endif		
    {
        osd_icon_num = 1;
    }
#else
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {
        osd_icon_num = 4;
    }
    else
#endif		
    {
        osd_icon_num = 3;
    }
#endif
#else//TOUCH_PANEL_SUPPORT
    osd_icon_num = 2;
#endif

    osd_width = lcd_width/osd_icon_num;
    osd_icon_offset = (osd_width > img_width)?(osd_width - img_width)/2:0;
    
    /* Create label */
    create.ctrl_id = MMIEBOOK_OSD_TIME_CTRL_ID;
    create.guid    = SPRD_GUI_LABEL_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;

    init_data.align= GUILABEL_ALIGN_MIDDLE;
    init_data.both_rect.v_rect.left= 0;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+OSD_TIME_LABEL_LEN;//lcd_width/MMIEBOOK_OSD_RADIX_V;
    init_data.both_rect.v_rect.top= 0;
    init_data.both_rect.v_rect.bottom= init_data.both_rect.v_rect.top+MMIEBOOK_OSD_LABEL_HEIGHT_UP;
    init_data.both_rect.h_rect.left= 0;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+OSD_TIME_LABEL_LEN;
    init_data.both_rect.h_rect.top= 0;
    init_data.both_rect.h_rect.bottom= init_data.both_rect.h_rect.top+MMIEBOOK_OSD_LABEL_HEIGHT_UP;
    s_label_handle[0]=MMK_CreateControl( &create );/*lint !e64*/

    create.ctrl_id = MMIEBOOK_OSD_NAME_CTRL_ID;
    init_data.align= GUILABEL_ALIGN_MIDDLE;
    init_data.both_rect.v_rect.left= init_data.both_rect.v_rect.right+1;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+lcd_width - OSD_PERCENT_LABEL_LEN -OSD_TIME_LABEL_LEN;
    init_data.both_rect.h_rect.left= init_data.both_rect.h_rect.right;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+lcd_width - OSD_PERCENT_LABEL_LEN -OSD_TIME_LABEL_LEN;
    s_label_handle[1]=MMK_CreateControl( &create );/*lint !e64*/

    create.ctrl_id = MMIEBOOK_OSD_PERCENT_CTRL_ID;

    init_data.align= GUILABEL_ALIGN_MIDDLE;
    init_data.both_rect.v_rect.left= init_data.both_rect.v_rect.right+1;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+OSD_PERCENT_LABEL_LEN;
    init_data.both_rect.h_rect.left= init_data.both_rect.h_rect.right;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+OSD_PERCENT_LABEL_LEN;

    s_label_handle[2]=MMK_CreateControl( &create );/*lint !e64*/

#ifdef TOUCH_PANEL_SUPPORT
#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
    create.ctrl_id = MMIEBOOK_OSD_DISPLAY_SET_CTRL_ID;
    init_data.align= GUILABEL_ALIGN_MIDDLE;
    init_data.both_rect.v_rect.left= osd_icon_offset;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+img_width-1;
    init_data.both_rect.v_rect.top= lcd_height-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN+(MMIEBOOK_OSD_LABEL_HEIGHT_DOWN-img_height)/2;
    init_data.both_rect.v_rect.bottom=init_data.both_rect.v_rect.top+img_height;
    init_data.both_rect.h_rect.left= osd_icon_offset;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+img_width-1;
    init_data.both_rect.h_rect.top= lcd_height-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN+(MMIEBOOK_OSD_LABEL_HEIGHT_DOWN-img_height)/2;
    init_data.both_rect.h_rect.bottom= init_data.both_rect.h_rect.top+img_height;
    s_label_handle[3]=MMK_CreateControl( &create );/*lint !e64*/
#if defined(MMI_ISTYLE_SUPPORT)
    if (MMITHEME_IsIstyle())
    {    
        create.ctrl_id = MMIEBOOK_OSD_MORE_CTRL_ID;
        init_data.both_rect.v_rect.left= osd_icon_offset + osd_width;
        init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+img_width-1;
        init_data.both_rect.h_rect.left= osd_icon_offset + osd_width;
        init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+img_width-1;
        s_label_handle[4]=MMK_CreateControl( &create );/*lint !e64*/
    }
#endif	
#else
    create.ctrl_id = MMIEBOOK_OSD_FONTCOLOR_CTRL_ID;
    init_data.align= GUILABEL_ALIGN_MIDDLE;
    init_data.both_rect.v_rect.left= osd_icon_offset;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+img_width-1;
    init_data.both_rect.v_rect.top= lcd_height-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN+(MMIEBOOK_OSD_LABEL_HEIGHT_DOWN-img_height)/2;
    init_data.both_rect.v_rect.bottom=init_data.both_rect.v_rect.top+img_height;
    init_data.both_rect.h_rect.left= osd_icon_offset;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+img_width-1;
    init_data.both_rect.h_rect.top= lcd_height-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN+(MMIEBOOK_OSD_LABEL_HEIGHT_DOWN-img_height)/2;
    init_data.both_rect.h_rect.bottom= init_data.both_rect.h_rect.top+img_height;
    s_label_handle[3]=MMK_CreateControl( &create );/*lint !e64*/
    
    create.ctrl_id = MMIEBOOK_OSD_FONTSIZE_CTRL_ID;
    init_data.both_rect.v_rect.left= osd_icon_offset + osd_width;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+img_width-1;
    init_data.both_rect.h_rect.left= osd_icon_offset + osd_width;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+img_width-1;
    s_label_handle[4]=MMK_CreateControl( &create );/*lint !e64*/
    
    create.ctrl_id = MMIEBOOK_OSD_BG_CTRL_ID;
    init_data.both_rect.v_rect.left= osd_icon_offset + osd_width* 2;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+img_width-1;
    init_data.both_rect.h_rect.left= osd_icon_offset + osd_width* 2;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+img_width-1;
    s_label_handle[5]=MMK_CreateControl( &create );/*lint !e64*/
#if defined(MMI_ISTYLE_SUPPORT)    
    if (MMITHEME_IsIstyle())
    {
        create.ctrl_id = MMIEBOOK_OSD_MORE_CTRL_ID;
        init_data.both_rect.v_rect.left= osd_icon_offset + osd_width* 3;
        init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+img_width-1;
        init_data.both_rect.h_rect.left= osd_icon_offset + osd_width* 3;
        init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+img_width-1;
        s_label_handle[6]=MMK_CreateControl( &create );/*lint !e64*/
    }
#endif	
#endif
#else//TOUCH_PANEL_SUPPORT 
    create.ctrl_id = MMIEBOOK_OSD_LSK_CTRL_ID;
    init_data.align= GUILABEL_ALIGN_LEFT;
    init_data.both_rect.v_rect.left= 0;
    init_data.both_rect.v_rect.right= osd_width-1;
    init_data.both_rect.v_rect.top= lcd_height-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN;
    init_data.both_rect.v_rect.bottom=init_data.both_rect.v_rect.top+MMIEBOOK_OSD_LABEL_HEIGHT_DOWN-1;
    init_data.both_rect.h_rect.left= 0;
    init_data.both_rect.h_rect.right= osd_width-1;
    init_data.both_rect.h_rect.top= lcd_height-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN;
    init_data.both_rect.h_rect.bottom= init_data.both_rect.h_rect.top+MMIEBOOK_OSD_LABEL_HEIGHT_DOWN -1;
    s_label_handle[3]=MMK_CreateControl( &create );/*lint !e64*/

    create.ctrl_id = MMIEBOOK_OSD_RSK_CTRL_ID;    
    init_data.align= GUILABEL_ALIGN_RIGHT;
    init_data.both_rect.v_rect.left= osd_width;
    init_data.both_rect.v_rect.right= osd_width* 2-1;
    init_data.both_rect.h_rect.left= osd_width;
    init_data.both_rect.h_rect.right= osd_width* 2-1;
    s_label_handle[4]=MMK_CreateControl( &create );/*lint !e64*/
#endif
}
#else
/****************************************************************************/    
//  Description :CreateTitleMenu
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateTitleMenu(MMI_WIN_ID_T win_id)
{
    GUI_RECT_T rect={0};
    uint16 lcd_width = 0; 
    uint16 lcd_height = 0; 
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    rect.left=0;
    rect.right=lcd_width;
    rect.top=lcd_height-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN;
    rect.bottom=rect.top+MMIEBOOK_OSD_LABEL_HEIGHT_DOWN -1;
    
    DestroyTitleMenu();
    CreateTitleCtrl(win_id);
    SetTitleCtrlDevInfo(&s_osd_dev);
    SetTitleCtrlParam();
    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL, 0);
#if defined( MMI_GUI_STYLE_MINISCREEN)
    MMK_SetAtvCtrl(win_id,MMIEBOOK_OSD_NAME_CTRL_ID);
#endif
}

/****************************************************************************/    
//  Description :DestroyTitleMenu
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void DestroyTitleMenu(void)
{
    if(PNULL!=s_label_handle[0])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_TIME_CTRL_ID);
        s_label_handle[0]=PNULL;
    }
    if(PNULL!=s_label_handle[1])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_NAME_CTRL_ID);
        s_label_handle[1]=PNULL;
    }
    if(PNULL!=s_label_handle[2])
    {
        MMK_DestroyControl(MMIEBOOK_OSD_PERCENT_CTRL_ID);
        s_label_handle[2]=PNULL;
    }
}

/****************************************************************************/    
//  Description :SetTitleCtrlParam
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void SetTitleCtrlParam(void)
{
    GUI_BG_T bg={0};
    int8     time[MMISET_TIME_STR_12HOURS_LEN+1] = {0};
    MMI_STRING_T time_str= {0};
    SCI_TIME_T      sys_time = {0};
    MMIEBOOK_FILE_BRIEF_T*file_brief=PNULL;
    wchar			file_name[MMIEBOOK_TXT_NAME_MAX_LEN+1] = {0};
    uint16			file_name_len = 0;
    MMI_STRING_T str={0};
    
    bg.bg_type=GUI_BG_IMG;
    bg.img_id = IMAGE_TITLE_BAR;
    GUILABEL_SetBg(MMIEBOOK_OSD_TIME_CTRL_ID, & bg);
    GUILABEL_SetBg(MMIEBOOK_OSD_NAME_CTRL_ID, & bg);
    GUILABEL_SetBg(MMIEBOOK_OSD_PERCENT_CTRL_ID, & bg);

    TM_GetSysTime(&sys_time);
    MMIAPISET_FormatTimeStrByTime(sys_time.hour, sys_time.min,time,MMISET_TIME_STR_12HOURS_LEN+1);/*lint !e64*/
    time_str.wstr_len = SCI_STRLEN((char*)time);
    time_str.wstr_ptr = SCI_ALLOC_APPZ((time_str.wstr_len +1)*sizeof(wchar));
    MMI_STRNTOWSTR(time_str.wstr_ptr,time_str.wstr_len,time,strlen((char*)time),strlen((char*)time));/*lint !e64*/
    GUILABEL_SetText(MMIEBOOK_OSD_TIME_CTRL_ID, &time_str, FALSE);
    if (PNULL != time_str.wstr_ptr)
    {
        SCI_FREE(time_str.wstr_ptr);
        time_str.wstr_ptr = PNULL;
    }

    GUILABEL_SetFont(MMIEBOOK_OSD_TIME_CTRL_ID,MMIEBOOK_OSD_FONT,MMIEBOOK_OSD_FONT_COLOR);
    file_brief= MMIEBOOK_GetFileBrief();
    MMIAPIFMM_SplitFullPath(file_brief->full_name_wstr, file_brief->full_name_len,
                        		NULL, NULL, NULL, NULL,
                        		file_name, &file_name_len);
    str.wstr_ptr=file_name;
    str.wstr_len=file_name_len;
    GUILABEL_SetText(MMIEBOOK_OSD_NAME_CTRL_ID, &str, FALSE);
    GUILABEL_SetFont(MMIEBOOK_OSD_NAME_CTRL_ID,MMIEBOOK_OSD_FONT,MMIEBOOK_OSD_FONT_COLOR);

    MMIEBOOK_ShowCurPercent(MMIEBOOK_OSD_PERCENT_CTRL_ID);
    GUILABEL_SetFont(MMIEBOOK_OSD_PERCENT_CTRL_ID,MMIEBOOK_OSD_FONT,MMIEBOOK_OSD_FONT_COLOR);

}

/****************************************************************************/    
//  Description :SetTitleCtrlDevInfo
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void SetTitleCtrlDevInfo(GUI_LCD_DEV_INFO* osd_dev)
{
    uint32 i=0;
    GUI_LCD_DEV_INFO* dev=osd_dev;
    
    for(i=0;i<MMIEBOOK_OSD_LABEL_NUM;i++)
    {
       GUICTRL_SetLcdDevInfo((IGUICTRL_T*)s_label_handle[i], dev); //pclint
    }
}

/****************************************************************************/    
//  Description :CreateTitleCtrl
//  Global resource dependence : none
//  Author: yongsheng.wang
//  Note:
/*****************************************************************************/
LOCAL void CreateTitleCtrl(MMI_WIN_ID_T win_id)
{
    MMI_CONTROL_CREATE_T    create = {0};
    GUILABEL_INIT_DATA_T     init_data = {0};
    uint16 lcd_width = 0; 
    uint16 lcd_height = 0; 

    uint16 osd_icon_num = 3;    
    uint16 osd_width = 0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);


    osd_width = lcd_width/osd_icon_num;
  
    /* Create label */
    create.ctrl_id = MMIEBOOK_OSD_TIME_CTRL_ID;
    create.guid    = SPRD_GUI_LABEL_ID;
    create.parent_win_handle = win_id;
    create.init_data_ptr = &init_data;
    init_data.align= GUILABEL_ALIGN_MIDDLE;

    init_data.both_rect.v_rect.left= 0;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+OSD_TIME_LABEL_LEN;//lcd_width/MMIEBOOK_OSD_RADIX_V;
#ifdef MMI_MINI_QVGA_UI_SUPPORT
	init_data.both_rect.v_rect.top= -2;
#else
	init_data.both_rect.v_rect.top= 0;
#endif 
    init_data.both_rect.v_rect.bottom= init_data.both_rect.v_rect.top+MMIEBOOK_OSD_LABEL_HEIGHT_UP;
    init_data.both_rect.h_rect.left= 0;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+OSD_TIME_LABEL_LEN;
#ifdef MMI_MINI_QVGA_UI_SUPPORT    
	init_data.both_rect.h_rect.top= -2;
#else
	init_data.both_rect.h_rect.top= 0;
#endif
    init_data.both_rect.h_rect.bottom= init_data.both_rect.h_rect.top+MMIEBOOK_OSD_LABEL_HEIGHT_UP;
    s_label_handle[0]=MMK_CreateControl( &create );/*lint !e64*/

    create.ctrl_id = MMIEBOOK_OSD_NAME_CTRL_ID;
    init_data.align= GUILABEL_ALIGN_LEFT;/*GUILABEL_ALIGN_MIDDLE*/
    init_data.both_rect.v_rect.left= init_data.both_rect.v_rect.right+1;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+lcd_width - OSD_PERCENT_LABEL_LEN -OSD_TIME_LABEL_LEN;
    init_data.both_rect.h_rect.left= init_data.both_rect.h_rect.right;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+lcd_width - OSD_PERCENT_LABEL_LEN -OSD_TIME_LABEL_LEN;
    s_label_handle[1]=MMK_CreateControl( &create );/*lint !e64*/

    create.ctrl_id = MMIEBOOK_OSD_PERCENT_CTRL_ID;

    init_data.align= GUILABEL_ALIGN_MIDDLE;
    init_data.both_rect.v_rect.left= init_data.both_rect.v_rect.right+1;
    init_data.both_rect.v_rect.right= init_data.both_rect.v_rect.left+OSD_PERCENT_LABEL_LEN;
    init_data.both_rect.h_rect.left= init_data.both_rect.h_rect.right;
    init_data.both_rect.h_rect.right= init_data.both_rect.h_rect.left+OSD_PERCENT_LABEL_LEN;


    s_label_handle[2]=MMK_CreateControl( &create );/*lint !e64*/

}
#endif
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void  HandleShowTpUpMsg(MMI_WIN_ID_T win_id, DPARAM param)
{
    GUI_POINT_T point = {0};
    GUI_RECT_T label_rect[4]={0};
    MMIEBOOK_SHOW_RECT_INFO_T* show_rect_info=PNULL;
    GUI_RECT_T rect_up={0};
    GUI_RECT_T rect_down={0};
    GUI_RECT_T rect_osd={0};
    uint16 lcd_width = 0; 
    uint16 lcd_height = 0; 
    
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);

#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
#ifndef MMIEBOOK_COPY_SUPPORT
    if(!s_slide_ebook.is_init)
    {
        return;
    }
#endif
#ifdef MMIEBOOK_COPY_SUPPORT
    if(s_slide_ebook.is_init)
#endif
    {
        StopSlideRedrawTimer(&s_slide_ebook);
        if (s_slide_ebook.slide_state!=MMK_TP_SLIDE_NONE
            ||  MMK_GET_TP_SLIDE(param))
        { 
            SCI_TRACE_LOW("ebook:tp_up_slide");
            HandleEbookSlideTpUp(&point,&s_slide_ebook);
            //s_down_point.x=-1;
            //s_down_point.y=-1;
            return;
            //if(abs(s_down_point.x-point.x)>3 && abs(s_down_point.y-point.y)>3)
            //{
            //    return;
            //}       
       }
   }
#endif     
#ifdef TOUCH_PANEL_SUPPORT 
    if(s_pre_tp_msg==MSG_TP_GESTURE)
    {
#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
        SCI_TRACE_LOW("ebook:s_pre_tp_msg==MSG_TP_GESTURE");
        s_gesture_num_out=0;
        s_gesture_num_in=0;
#endif
        return;
    }
#endif
    if(s_osd_is_active)
    {
#ifdef MMIEBOOK_OSD_DISPLAY_SETTING
        GUILABEL_GetRect(MMIEBOOK_OSD_DISPLAY_SET_CTRL_ID,&label_rect[0]);
        GUILABEL_GetRect(MMIEBOOK_OSD_MORE_CTRL_ID,&label_rect[1]);
        if(GUI_PointIsInRect(point, label_rect[0]))
        {                
            MMK_CreateWin((uint32 *)MMIEBOOK_DISPLAY_SET_WIN_TAB, PNULL);
        }
        else if(GUI_PointIsInRect(point, label_rect[1]))
        {
            MMK_SendMsg(win_id, MSG_APP_MENU, PNULL);
        }
#else
        GUILABEL_GetRect(MMIEBOOK_OSD_FONTCOLOR_CTRL_ID,&label_rect[0]);
        GUILABEL_GetRect(MMIEBOOK_OSD_FONTSIZE_CTRL_ID,&label_rect[1]);
        GUILABEL_GetRect(MMIEBOOK_OSD_BG_CTRL_ID,&label_rect[2]);
        GUILABEL_GetRect(MMIEBOOK_OSD_MORE_CTRL_ID,&label_rect[3]);
        if(GUI_PointIsInRect(point, label_rect[0]))
        {
            MMK_CreatePubFormWin((uint32 *)MMIEBOOK_SET_FONT_COLOR_WIN_TAB, PNULL);
        }
        else if(GUI_PointIsInRect(point, label_rect[1]))
        {
            MMK_CreatePubListWin((uint32 *)MMIEBOOK_FONT_SIZE_WIN_TAB, PNULL);
        }
        else if(GUI_PointIsInRect(point, label_rect[2]))
        {
            MMK_CreatePubFormWin((uint32 *)MMIEBOOK_SET_BG_COLOR_WIN_TAB, PNULL);
        }
        else if(GUI_PointIsInRect(point, label_rect[3]))
        {
            MMK_SendMsg(win_id, MSG_APP_MENU, PNULL);
        }
#endif
#ifdef MMIEBOOK_OSDMENU_SUPPORT
        DestroyOSDMenu();
#endif
        return ;
    }     
    show_rect_info= MMIEBOOK_GetEbookShowRectInfo();

    rect_up.left=show_rect_info->txt_show_left;
    rect_up.right=show_rect_info->txt_show_right;
    rect_up.top=show_rect_info->txt_show_top;     
    if(s_osd_is_active)
    {
        rect_up.top=show_rect_info->txt_show_top+MMIEBOOK_OSD_LABEL_HEIGHT_UP;
    }
    else
    {
        rect_up.top=show_rect_info->txt_show_top;
    }
    rect_up.bottom=show_rect_info->txt_show_top+(show_rect_info->txt_show_bottom-show_rect_info->txt_show_top)/3;

    rect_down.left=show_rect_info->txt_show_left;
    rect_down.right=show_rect_info->txt_show_right;
    rect_down.top=show_rect_info->txt_show_top+(show_rect_info->txt_show_bottom-show_rect_info->txt_show_top)*2/3;
    rect_down.bottom=show_rect_info->txt_show_bottom;    
    if(s_osd_is_active)
    {
        rect_down.bottom=show_rect_info->txt_show_bottom-MMIEBOOK_OSD_LABEL_HEIGHT_DOWN;
    }
    else
    {
        rect_down.bottom=show_rect_info->txt_show_bottom;
    }
    rect_osd.left=show_rect_info->txt_show_left;
    rect_osd.right=show_rect_info->txt_show_right;
    rect_osd.top=show_rect_info->txt_show_top+(show_rect_info->txt_show_bottom-show_rect_info->txt_show_top)/3;
    rect_osd.bottom=show_rect_info->txt_show_bottom;
        
    if(IsInRect(point.x, point.y, rect_down))
    {   
#ifdef MMIEBOOK_OSDMENU_SUPPORT
        if(s_osd_is_active)
        {
            DestroyOSDMenu();
        }
#endif
#ifdef TTS_SUPPORT
        if(MMIEBOOK_IsTurnLineOrPage(MSG_APP_RIGHT))
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
        }
#endif 
        MMIEBOOK_TurnLineOrPage(MMIEBOOK_PAGE_DOWN);
        EbookCloseTimer(&s_auto_read_timer_id);
    }
    else if(IsInRect(point.x, point.y, rect_up))
    {
#ifdef MMIEBOOK_OSDMENU_SUPPORT
        if(s_osd_is_active)
        {
            DestroyOSDMenu();
        }  
#endif
#ifdef TTS_SUPPORT
        if(MMIEBOOK_IsTurnLineOrPage(MSG_APP_LEFT))
        {
            if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_EBOOK))
            {
                MMIAPITTS_StopPlayText();
            }
        }
#endif 

        MMIEBOOK_TurnLineOrPage(MMIEBOOK_PAGE_UP);
        if(MMIEBOOK_GetIsNeedCloseTimer())
        {
            EbookCloseTimer(&s_auto_read_timer_id);
        }
        else
        {
            MMIEBOOK_SetIsNeedCloseTimer(TRUE);
        }
    } 
#ifdef MMIEBOOK_OSDMENU_SUPPORT
    else if(IsInRect(point.x, point.y, rect_osd))
    {
        if(s_osd_is_active)
        {
            DestroyOSDMenu();
        }
        else
        {
            CreateOSDMenu(win_id);
        }
    }
    else
    {
        if(s_osd_is_active)
        {
            DestroyOSDMenu();
        }
    }  
#endif
}
#endif
//#else
#if 0
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void  HandleShowTpUpMsg( MMI_WIN_ID_T win_id,DPARAM param)
{
    if (!MMK_GET_TP_SLIDE(param))
    {
        GUI_BOTH_RECT_T both_rect_lbtn = {0};
        GUI_BOTH_RECT_T both_rect_rbtn = {0};
        GUI_POINT_T point = {0};
        BOOLEAN is_page_change = FALSE;
    	MMIEBOOK_SHOW_RECT_INFO_T* show_rect_info=PNULL;
    	GUI_RECT_T rect_up={0};
    	GUI_RECT_T rect_down={0};


        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);

    	if(s_is_full_screen_off)
    	{
            GUIAPICTRL_GetBothRect(MMIEBOOK_TXT_SHOW_BUTTON_LF_CTRL_ID, &both_rect_lbtn);
            GUIAPICTRL_GetBothRect(MMIEBOOK_TXT_SHOW_BUTTON_RH_CTRL_ID, &both_rect_rbtn);
            
            if (MMITHEME_IsMainScreenLandscape())
            {
                if(!(IsInRect(point.x, point.y, both_rect_lbtn.h_rect)  || IsInRect(point.x, point.y, both_rect_rbtn.h_rect)))
                {
                    is_page_change = TRUE;
                }
            }
            else
            {
                if(!(IsInRect(point.x, point.y, both_rect_lbtn.v_rect)  || IsInRect(point.x, point.y, both_rect_rbtn.v_rect)))
                {
                    is_page_change = TRUE;
                }
            }
    	}
    	else
    	{
    		is_page_change = TRUE;
    	}
        
        if ( is_page_change )
        { 
    		show_rect_info= MMIEBOOK_GetEbookShowRectInfo();

    		rect_up.left=show_rect_info->txt_show_left;
    		rect_up.right=show_rect_info->txt_show_right;
    		rect_up.top=show_rect_info->txt_show_top;
    		rect_up.bottom=(show_rect_info->txt_show_bottom+show_rect_info->txt_show_top)/2;

    		rect_down.left=show_rect_info->txt_show_left;
    		rect_down.right=show_rect_info->txt_show_right;
    		rect_down.top=(show_rect_info->txt_show_bottom+show_rect_info->txt_show_top)/2;
    		rect_down.bottom=show_rect_info->txt_show_bottom;
    		if(IsInRect(point.x, point.y, rect_down))
    		{
#ifdef TTS_SUPPORT
    			if(MMIEBOOK_IsTurnLineOrPage(MSG_APP_RIGHT))
    			{

                        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_EBOOK))
                        {
                            MMIAPITTS_StopPlayText();
                        }

    			}
#endif 
           		MMIEBOOK_TurnLineOrPage(MMIEBOOK_PAGE_DOWN);
    			EbookCloseTimer(&s_auto_read_timer_id);
    		}
    		else if(IsInRect(point.x, point.y, rect_up))
    		{
#ifdef TTS_SUPPORT
    			if(MMIEBOOK_IsTurnLineOrPage(MSG_APP_LEFT))
    			{
                        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_EBOOK))
                        {
                            MMIAPITTS_StopPlayText();
                        }
    			}
#endif 
    			
    			MMIEBOOK_TurnLineOrPage(MMIEBOOK_PAGE_UP);
    			if(MMIEBOOK_GetIsNeedCloseTimer())
    			{
    				EbookCloseTimer(&s_auto_read_timer_id);
    			}
    			else
    			{
    				MMIEBOOK_SetIsNeedCloseTimer(TRUE);
    			}
    		}
        }
    } 
}
#endif
/*****************************************************************************/
// 	Description : ButtonOwnerDrawFun
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:   
/*****************************************************************************/
LOCAL void ButtonOwnerDrawFun(GUIBUTTON_OWNER_DRAW_T *owner_draw_ptr)
{
    uint32 ctrl_id=0;
    GUI_RECT_T rect={0};//max_rect与owner_draw_ptr->display_rect相交区域，也即图片最后显示区域
    GUI_RECT_T max_rect={0};//图片可以显示的最大区域
    GUI_RECT_T img_rect={0};//图片裁剪区
    MMI_HANDLE_T win_handle = 0;
    MMI_WIN_ID_T    win_id=0;
    uint16 image_w=0;
    uint16 iamge_h=0;
    GUI_RECT_T ctrl_rect={0};

    ctrl_id=MMK_GetCtrlId(owner_draw_ptr->ctrl_handle);
    win_handle=MMK_GetWinHandleByCtrl(owner_draw_ptr->ctrl_handle);
    win_id=MMK_GetWinId(win_handle);
    GUIRES_GetImgWidthHeight(&image_w, &iamge_h, IMAGE_EBOOK_COLOR_SELECT, win_id);
    GUICTRL_GetRect(MMK_GetCtrlPtr(owner_draw_ptr->ctrl_handle),&ctrl_rect);

    max_rect.left=owner_draw_ptr->display_x+ctrl_rect.right-ctrl_rect.left-image_w;
    max_rect.top=owner_draw_ptr->display_y+ctrl_rect.bottom-ctrl_rect.top-iamge_h;
    max_rect.right=max_rect.left+image_w;
    max_rect.bottom=max_rect.top+iamge_h;

    if(!GUI_IntersectRect(&rect,max_rect,owner_draw_ptr->display_rect))
    {//没有相交区域，返回
        return ;
    }

    img_rect.left=rect.left-max_rect.left;
    img_rect.top=rect.top-max_rect.top;
    img_rect.right=img_rect.left+(rect.right-rect.left);
    img_rect.bottom=img_rect.top+(rect.bottom-rect.top);

    if(GUI_IsInvalidRect(img_rect)||GUI_IsInvalidRect(rect))//无效矩形不要画了
    {
        return ;
    }
	
    GUIRES_DisplayImg(PNULL, &rect, &img_rect, win_handle, IMAGE_EBOOK_COLOR_SELECT, &owner_draw_ptr->lcd_dev);
}

/*****************************************************************************/
 //     Description : HandleSetFontColor
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
 /*****************************************************************************/
 LOCAL MMI_RESULT_E  HandleSetFontColor(
     MMI_WIN_ID_T       win_id, 
     MMI_MESSAGE_ID_E   msg_id, 
     DPARAM             param
     )
{
    MMI_RESULT_E        recode  =MMI_RESULT_TRUE;
    uint32              button_num    = 0;
    GUI_BG_T            button = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //MMI_CTRL_ID_T ctrl_id = MMIEBOOK_BUTTON_COLOR1_FONT_CTRL;
            GUIFORM_CHILD_WIDTH_T  form_width={0};
            GUIFORM_CHILD_HEIGHT_T form_height={0};
            uint16 hor_space=MMIEBOOK_COLOR_HOR;
            uint16 ver_space=MMIEBOOK_COLOR_VER;
            
            GUIFORM_PermitChildBg(MMIEBOOK_FORM_FONT_COLOR_CTRL_ID,FALSE);
    		GUIFORM_PermitChildFont(MMIEBOOK_FORM_FONT_COLOR_CTRL_ID,FALSE);
            MMIAPIEBOOK_GetFontType();
            
            for(button_num = MMIEBOOK_BUTTON_COLOR_1_FONT;button_num <=MMIEBOOK_BUTTON_COLOR_6_FONT;button_num++)
            {
                button.bg_type = GUI_BG_COLOR;
                button.shape   = GUI_SHAPE_ROUNDED_RECT;
                button.color   = s_color_button_info[button_num];
                GUIBUTTON_SetBg(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num,&button);
                GUIBUTTON_SetRunSheen(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num,FALSE);
                GUIBUTTON_SetPressedBg(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num,&button);

                form_width.type=GUIFORM_CHILD_WIDTH_FIXED;
                form_width.add_data=MMIEBOOK_COLOR_WIDTH;
                form_height.type=GUIFORM_CHILD_HEIGHT_FIXED;
                form_height.add_data=MMIEBOOK_COLOR_HEIGHT;
                if(button_num<=MMIEBOOK_BUTTON_COLOR_3_FONT)
                {
                    GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL, MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, &form_width);
                    GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL, MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, &form_height);
                }
                else
                {
                    GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2, MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, &form_width);
                    GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2, MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, &form_height);
                }
                
            }
            GUIFORM_SetSpace(MMIEBOOK_FORM_FONT_COLOR_CTRL_ID,&hor_space,&ver_space);
            GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL,GUIFORM_CHILD_ALIGN_HMIDDLE);
            GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2,GUIFORM_CHILD_ALIGN_HMIDDLE);
            GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + s_ebook_fontcolor_id,ButtonOwnerDrawFun);
        }
        break;
        
    case MSG_FULL_PAINT:	                
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        break;  	
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
            else if(ctrl_id>=MMIEBOOK_BUTTON_COLOR1_FONT_CTRL && ctrl_id<=MMIEBOOK_BUTTON_COLOR6_FONT_CTRL)
            {
                s_ebook_fontcolor_id=ctrl_id-MMIEBOOK_BUTTON_COLOR1_FONT_CTRL;
                MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_FONT,&s_ebook_fontcolor_id);
                CheckDayNightMode();
                GUITEXT_SetFont(MMIEBOOK_TXT_SHOW_CTRL_ID,PNULL,&s_ebook_color[s_ebook_fontcolor_id]);/*lint !e605*/
                MMK_CloseWin( win_id );
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
 //     Description : HandleSetBGColor
 // Global resource dependence : 
 //  Author: renyi.hu
 // Note:
 /*****************************************************************************/
 LOCAL MMI_RESULT_E  HandleSetBGColor(
     MMI_WIN_ID_T       win_id, 
     MMI_MESSAGE_ID_E   msg_id, 
     DPARAM             param
     )
{
    MMI_RESULT_E        recode  =MMI_RESULT_TRUE;
    uint32              button_num    = 0;
    GUI_BG_T            button = {0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            //MMI_CTRL_ID_T ctrl_id = MMIEBOOK_BUTTON_COLOR1_BACK_CTRL;
            GUIFORM_CHILD_WIDTH_T  form_width={0};
            GUIFORM_CHILD_HEIGHT_T form_height={0};
            uint16 hor_space=MMIEBOOK_COLOR_HOR;
            uint16 ver_space=MMIEBOOK_COLOR_VER;
            uint32 bg_img[]={
                                IMAGE_EBOOK_COLOR_BGICON1,
                                IMAGE_EBOOK_COLOR_BGICON2,
                                IMAGE_EBOOK_COLOR_BGICON3,
#ifndef MMI_LOW_MEMORY_EBOOK
                                IMAGE_EBOOK_COLOR_BGICON4,
                                IMAGE_EBOOK_COLOR_BGICON5,
                                IMAGE_EBOOK_COLOR_BGICON6,
#endif
                            };
            
            GUIFORM_PermitChildBg(MMIEBOOK_FORM_BACK_COLOR_CTRL_ID,FALSE);
    		GUIFORM_PermitChildFont(MMIEBOOK_FORM_BACK_COLOR_CTRL_ID,FALSE);
            MMIAPIEBOOK_GetFontType();
#ifndef MMI_LOW_MEMORY_EBOOK            
            for(button_num = 0;button_num <=MMIEBOOK_BUTTON_COLOR_6_BACK-MMIEBOOK_BUTTON_COLOR_1_BACK;button_num++)/*lint !e656*/
#else
            for(button_num = 0;button_num <=MMIEBOOK_BUTTON_COLOR_3_BACK-MMIEBOOK_BUTTON_COLOR_1_BACK;button_num++)/*lint !e656*/
#endif
            {
                button.bg_type = GUI_BG_IMG;
                //button.shape   = GUI_SHAPE_ROUNDED_RECT;
                button.img_id= bg_img[button_num];
                //GUIBUTTON_SetBg(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num,&button);
                GUIBUTTON_SetRunSheen(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num,FALSE);
                GUIBUTTON_SetPressedBg(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num,&button);

                form_width.type=GUIFORM_CHILD_WIDTH_FIXED;
                form_width.add_data=MMIEBOOK_COLOR_WIDTH;
                form_height.type=GUIFORM_CHILD_HEIGHT_FIXED;
                form_height.add_data=MMIEBOOK_COLOR_HEIGHT;
                if(button_num<=MMIEBOOK_BUTTON_COLOR_3_BACK-MMIEBOOK_BUTTON_COLOR_1_BACK)/*lint !e656*/
                {
                    GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL, MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, &form_width);
                    GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL, MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, &form_height);
                }
                else
                {
                    GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2, MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, &form_width);
                    GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2, MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, &form_height);
                }
            }
            GUIFORM_SetSpace(MMIEBOOK_FORM_BACK_COLOR_CTRL_ID,&hor_space,&ver_space);
            GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL,GUIFORM_CHILD_ALIGN_HMIDDLE);
            GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2,GUIFORM_CHILD_ALIGN_HMIDDLE);
            GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + s_ebook_bg_id,ButtonOwnerDrawFun);
        }
        break;
        
    case MSG_FULL_PAINT:	                
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        break;  	
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
            
            if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
            }
            else if(ctrl_id>=MMIEBOOK_BUTTON_COLOR1_BACK_CTRL && ctrl_id<=MMIEBOOK_BUTTON_COLOR6_BACK_CTRL)
            {
                GUI_BG_T txtbox_bg = {0};
 #ifndef MMI_LOW_MEMORY_EBOOK           
                s_ebook_bg_id=ctrl_id-MMIEBOOK_BUTTON_COLOR1_BACK_CTRL;
                MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_BACK,&s_ebook_bg_id);
                if(s_ebook_bg_id<=MMIEBOOK_BG_3)
                {
                    txtbox_bg.bg_type = GUI_BG_COLOR;
                    txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
                }
                else                    
                {
                    txtbox_bg.bg_type = GUI_BG_IMG;
                    txtbox_bg.img_id=s_ebook_back_img[s_ebook_bg_id-MMIEBOOK_BG_4];/*lint !e656*/
                }
#else
                s_ebook_bg_id=ctrl_id-MMIEBOOK_BUTTON_COLOR4_BACK_CTRL;
                MMINV_WRITE(MMINV_EBOOK_COLOR_FOR_BACK,&s_ebook_bg_id);
                {
                    txtbox_bg.bg_type = GUI_BG_COLOR;
                    txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
                }
#endif
                CheckDayNightMode();
                GUITEXT_SetBg(MMIEBOOK_TXT_SHOW_CTRL_ID,&txtbox_bg);
                MMK_CloseWin( win_id );
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
            
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef MMIEBOOK_PAGESLIDE_SUPPORT
/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateAllTurnPageLayer(MMI_WIN_ID_T win_id)
{
    CreateTurnPageLayer(win_id,&s_cur_page_dev);
    CreateTurnPageLayer(win_id,&s_next_page_dev);
    CreateTurnPageLayer(win_id,&s_out_page_dev);
    CreateTextLayer(win_id,&s_text_dev);
}

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void ReleaseAllTurnPageLayer(void)
{
    if(s_cur_page_dev.block_id!=UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_cur_page_dev);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_cur_page_dev);
//        s_cur_page_dev.block_id=UILAYER_NULL_HANDLE;
//        s_cur_page_dev.lcd_id=0;
    }
    if(s_next_page_dev.block_id!=UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_next_page_dev);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_next_page_dev);
//        s_next_page_dev.block_id=UILAYER_NULL_HANDLE;
//        s_next_page_dev.lcd_id=0;
    }
    if(s_out_page_dev.block_id!=UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_out_page_dev);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_out_page_dev);
//        s_out_page_dev.block_id=UILAYER_NULL_HANDLE;
//        s_out_page_dev.lcd_id=0;
    }
    if(s_text_dev.block_id!=UILAYER_NULL_HANDLE)
    {
        UILAYER_RELEASELAYER(&s_text_dev);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(&s_text_dev);
//        s_text_dev.block_id=UILAYER_NULL_HANDLE;
//        s_text_dev.lcd_id=0;
    }
}
/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateTextLayer(MMI_WIN_ID_T win_id,GUI_LCD_DEV_INFO* dev_ptr)
{   
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
        //UILAYER_APPEND_BLT_T append_layer = {0};

        if(dev_ptr->block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(dev_ptr);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(dev_ptr);
//            dev_ptr->block_id=UILAYER_NULL_HANDLE;
//            dev_ptr->lcd_id=0;
        }
        
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = 1;
        create_info.height = 1;
        create_info.is_bg_layer = TRUE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(&create_info, dev_ptr);
    }
}

/****************************************************************************/    
//  Description :CreateOSDCtrl
//  Global resource dependence : none
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void CreateTurnPageLayer(MMI_WIN_ID_T win_id,GUI_LCD_DEV_INFO* dev_ptr)
{  
    if (UILAYER_IsMultiLayerEnable())
    {
        UILAYER_CREATE_T create_info = {0};
        uint16 lcd_width = 0; 
        uint16 lcd_height = 0; 
        //UILAYER_APPEND_BLT_T append_layer = {0};

        if(dev_ptr->block_id!=UILAYER_NULL_HANDLE)
        {
            UILAYER_RELEASELAYER(dev_ptr);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(dev_ptr);
//            dev_ptr->block_id=UILAYER_NULL_HANDLE;
//            dev_ptr->lcd_id=0;
        }

        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
        
        create_info.lcd_id = GUI_MAIN_LCD_ID;
        create_info.owner_handle = win_id;
        create_info.offset_x = 0;
        create_info.offset_y = 0;
        create_info.width = lcd_width;
        create_info.height = lcd_height;
        create_info.is_bg_layer = TRUE;
        create_info.is_static_layer = FALSE; 
        UILAYER_CreateLayer(&create_info, dev_ptr);
    }
}
/*****************************************************************************/
// Description : InitSlidecalendar
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void InitSlideEbook( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr,
                                MMI_HANDLE_T    handle)
{
    SCI_MEMSET(slide_Ebook_ptr,0,sizeof(MMI_SLIDE_EBOOK_T));
    slide_Ebook_ptr->handle=handle;
    slide_Ebook_ptr->is_init=TRUE;
    UILAYER_Clear(&s_cur_page_dev);
    UILAYER_Clear(&s_next_page_dev);
    UILAYER_Clear(&s_out_page_dev);
}

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideRedrawTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr)
{
    if (PNULL != slide_Ebook_ptr)
    {
        if(0 != slide_Ebook_ptr->redraw_timer_id)
        {
            StopSlideRedrawTimer(slide_Ebook_ptr);
        }
        slide_Ebook_ptr->redraw_timer_id = MMK_CreateWinTimer(
            slide_Ebook_ptr->handle, 
            MMIEBOOK_SLIDE_REDRAW_TIME,
            FALSE );
    }
}
/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideRedrawTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr)
{
    if (PNULL != slide_Ebook_ptr && 0 != slide_Ebook_ptr->redraw_timer_id)
    {
        MMK_StopTimer(slide_Ebook_ptr->redraw_timer_id);
        slide_Ebook_ptr->redraw_timer_id = 0;
    }
}

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideFlingTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr)
{
    if (PNULL != slide_Ebook_ptr)
    {
        if(0 != slide_Ebook_ptr->fling_timer_id)
        {
            StopSlideFlingTimer(slide_Ebook_ptr);
        }
        slide_Ebook_ptr->fling_timer_id = MMK_CreateWinTimer(
            slide_Ebook_ptr->handle, 
            MMIEBOOK_SLIDE_FLING_TIME,
            FALSE );
    }
}

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideFlingTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr)
{
    if (PNULL != slide_Ebook_ptr && 0 != slide_Ebook_ptr->fling_timer_id)
    {
        MMK_StopTimer(slide_Ebook_ptr->fling_timer_id);
        slide_Ebook_ptr->fling_timer_id = 0;
    }
}

/*****************************************************************************/
// Description : StartSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StartSlideRecoverTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr)
{
    if (PNULL != slide_Ebook_ptr)
    {
        if(0 != slide_Ebook_ptr->recover_timer_id)
        {
            StopSlideRecoverTimer(slide_Ebook_ptr);
        }
        slide_Ebook_ptr->recover_timer_id = MMK_CreateWinTimer(
            slide_Ebook_ptr->handle, 
            MMIEBOOK_SLIDE_FLING_TIME,
            FALSE );
    }
}

/*****************************************************************************/
// Description : StopSlideRedrawTimer
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void StopSlideRecoverTimer( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr)
{
    if (PNULL != slide_Ebook_ptr && 0 != slide_Ebook_ptr->recover_timer_id)
    {
        MMK_StopTimer(slide_Ebook_ptr->recover_timer_id);
        slide_Ebook_ptr->recover_timer_id = 0;
    }
}
/*****************************************************************************/
//  Description : to check whether need to redraw
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
__inline LOCAL BOOLEAN IsTpMoved(
                                 MMI_SLIDE_EBOOK_T    *slide_Ebook_ptr,  // [in]
                                 GUI_POINT_T    *point_ptr
                                 )
{
    if (abs(point_ptr->x - slide_Ebook_ptr->slide_start_point.x) > EBOOK_TP_MOVE_DIS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleEbookSlideTpDown(
                                GUI_POINT_T       *tp_point_ptr,    // [in]
                                MMI_SLIDE_EBOOK_T    *slide_Ebook_ptr,    // [in]
                                MMI_HANDLE_T    handle
                                )
{
    //SCI_TRACE_LOW:"HandleEbookSlideTpDown:slide_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_6303_112_2_18_2_15_37_88,(uint8*)"d",slide_Ebook_ptr->slide_state);
    MMK_DelayTpMove(FALSE);
    if(slide_Ebook_ptr->slide_state!=MMK_TP_SLIDE_NONE)
    {
        ExitSlideEbook(slide_Ebook_ptr,TRUE,TRUE);
    }
    InitSlideEbook(slide_Ebook_ptr,handle);
    // 增加速度点
    MMK_AddVelocityItem( tp_point_ptr->x, tp_point_ptr->y );

    // 记录起始点
    slide_Ebook_ptr->slide_start_point.x = tp_point_ptr->x;
    slide_Ebook_ptr->slide_start_point.y = tp_point_ptr->y;

    // 记录前一个点
    slide_Ebook_ptr->slide_pre_point.x = tp_point_ptr->x;
    slide_Ebook_ptr->slide_pre_point.y = tp_point_ptr->y;

    slide_Ebook_ptr->slide_state = MMK_TP_SLIDE_NONE;

    StartSlideRedrawTimer(slide_Ebook_ptr);
}

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleEbookSlideTpMove(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_SLIDE_EBOOK_T    *slide_Ebook_ptr    // [in]
                                )
{
    
    if(!slide_Ebook_ptr->is_init)
    {
#ifdef MMIEBOOK_COPY_SUPPORT
        HandleEbookSlideTpDown(point_ptr,slide_Ebook_ptr,MMIEBOOK_TXT_SHOW_WIN_ID);
#endif
        return;
    }
    
    // 增加速度点
    MMK_AddVelocityItem( point_ptr->x, point_ptr->y );
    //SCI_TRACE_LOW:"HandleEbookSlideTpMove:slide_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_6341_112_2_18_2_15_37_89,(uint8*)"d",slide_Ebook_ptr->slide_state);
    if (MMK_TP_SLIDE_NONE == slide_Ebook_ptr->slide_state
        && IsTpMoved(slide_Ebook_ptr, point_ptr))
    {
        uint16 w=0;
        uint16 h=0;
        GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &w, &h);
        
        if((point_ptr->x < slide_Ebook_ptr->slide_pre_point.x)
            &&(slide_Ebook_ptr->slide_pre_point.x>=(w/2)) && (!MMIEBOOK_IsInLastPage()))
        {
            EbookCloseTimer(&s_auto_read_timer_id);
#ifdef TTS_SUPPORT
            MMIAPITTS_StopPlayText();
#endif
            if(point_ptr->y<(h/2))
            {
                slide_Ebook_ptr->turn_mode=SE_PAGE_TURN_RIGHT_TOP;
            }
            else
            {
                slide_Ebook_ptr->turn_mode=SE_PAGE_TURN_RIGHT_BOTTOM;
            }
            slide_Ebook_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
            slide_Ebook_ptr->slide_pre_point.x = point_ptr->x;
            slide_Ebook_ptr->slide_pre_point.y = point_ptr->y;
            CreateEffect(slide_Ebook_ptr->turn_mode);
            SetEffectBuffData(slide_Ebook_ptr);    
#ifdef MMIEBOOK_OSDMENU_SUPPORT
            if(s_osd_is_active)
            {
                DestroyOSDMenu();
            }
#endif
        }
        else if((point_ptr->x > slide_Ebook_ptr->slide_pre_point.x)
            &&(slide_Ebook_ptr->slide_pre_point.x<(w/2))&& (!MMIEBOOK_IsFirstPage()))
        {
            EbookCloseTimer(&s_auto_read_timer_id);
#ifdef TTS_SUPPORT
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
#endif
            if(point_ptr->y<(h/2))
            {
                slide_Ebook_ptr->turn_mode=SE_PAGE_TURN_LEFT_TOP;
            }
            else
            {
                slide_Ebook_ptr->turn_mode=SE_PAGE_TURN_LEFT_BOTTOM;
            }
            slide_Ebook_ptr->slide_state = MMK_TP_SLIDE_TP_SCROLL;
            slide_Ebook_ptr->slide_pre_point.x = point_ptr->x;
            slide_Ebook_ptr->slide_pre_point.y = point_ptr->y;
            CreateEffect(slide_Ebook_ptr->turn_mode);
            SetEffectBuffData(slide_Ebook_ptr);
#ifdef MMIEBOOK_OSDMENU_SUPPORT
            if(s_osd_is_active)
            {
                DestroyOSDMenu();
            }
#endif
        }
        else
        {
            slide_Ebook_ptr->slide_pre_point.x = point_ptr->x;
            slide_Ebook_ptr->slide_pre_point.y = point_ptr->y;
            slide_Ebook_ptr->slide_start_point.x = point_ptr->x;
            slide_Ebook_ptr->slide_start_point.y = point_ptr->y;
            MMK_ResetVelocityItem();
        }
    }

    return;
}

/*****************************************************************************/
//  Description : HandleSlideTpDown
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleEbookSlideTpUp(
                                GUI_POINT_T       *point_ptr,    // [in]
                                MMI_SLIDE_EBOOK_T    *slide_Ebook_ptr    // [in]
                                )
{
    //SCI_TRACE_LOW:"HandleEbookSlideTpUp:slide_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_6421_112_2_18_2_15_37_90,(uint8*)"d",slide_Ebook_ptr->slide_state);
    if(!slide_Ebook_ptr->is_init)
    {
        return;
    }
    
    if(MMK_TP_SLIDE_TP_SCROLL==slide_Ebook_ptr->slide_state)
    {
        // 增加速度点
        StopSlideRedrawTimer(slide_Ebook_ptr);
        MMK_AddVelocityItem( point_ptr->x, point_ptr->y );
        MMK_ComputeCurrentVelocity(&slide_Ebook_ptr->velocity, PNULL, FLING_MAX_SUM_VELOCITY, 0);/*lint !e747*/
        slide_Ebook_ptr->slide_pre_point.x = point_ptr->x;
        slide_Ebook_ptr->slide_pre_point.y = point_ptr->y;
        if (FLING_MIN_VELOCITY <= abs((int32)slide_Ebook_ptr->velocity))  // 达到一定速度，需要滑动
        {
            if(slide_Ebook_ptr->velocity>FLING_MIN_VELOCITY)
            {
                slide_Ebook_ptr->velocity=FLING_MAX_SUM_VELOCITY;
            }
            else
            {
                slide_Ebook_ptr->velocity=-FLING_MAX_SUM_VELOCITY;
            }
            // 进入滑动状态，起动滑动定时器
            slide_Ebook_ptr->slide_state = MMK_TP_SLIDE_FLING;
            StartSlideFlingTimer(slide_Ebook_ptr);
        }
        else
        {
            slide_Ebook_ptr->slide_state = MMK_TP_SLIDE_RECOVER;
            UpdateEbookRecoverMoveDirection(slide_Ebook_ptr);
            StartSlideRecoverTimer(slide_Ebook_ptr);

        }
    }
    else
    {
        ExitSlideEbook(slide_Ebook_ptr,TRUE,TRUE);
    }
    return;
}
#endif
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void ExitSlideEbook( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr,BOOLEAN is_update,BOOLEAN is_update_text)
{
    MMI_STRING_T text_string={0};
#ifdef MMIEBOOK_COPY_SUPPORT
    if(!slide_Ebook_ptr->is_init)
    {
        return ;
    }
#endif
    StopSlideFlingTimer(slide_Ebook_ptr);
    StopSlideRecoverTimer(slide_Ebook_ptr);
    StopSlideRedrawTimer(slide_Ebook_ptr);
    DestroyEffect(s_effect_handle);
    
    GUICTRL_SetLcdDevInfo(MMK_GetCtrlPtr(MMIEBOOK_TXT_SHOW_CTRL_ID), &s_main_dev);
    if(slide_Ebook_ptr->update_result==1)//当翻页动作没完成回到原来页时，textbox的内容需要恢复
    {
        if(slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_TOP || slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_BOTTOM)
        {
            MMIEBOOK_GetNextPageForEffect(SE_PAGE_TURN_RIGHT_TOP);
        }
        else
        {
            MMIEBOOK_GetNextPageForEffect(SE_PAGE_TURN_LEFT_TOP);
        }
        
    }
    if(is_update_text)
    {
        MMIEBOOK_GetShowTxt(&text_string);
        GUITEXT_SetString(MMIEBOOK_TXT_SHOW_CTRL_ID, text_string.wstr_ptr,text_string.wstr_len,is_update);
    }

    if(slide_Ebook_ptr->slide_state!=MMK_TP_SLIDE_NONE)
    {
#ifdef TTS_SUPPORT
        if(s_tts_auto_pagedown_flag)
        {
            MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_EBOOK);
            MMK_PostMsg(MMIEBOOK_TXT_SHOW_WIN_ID, MMIEBOOK_MSG_TTS_PLAY,PNULL,0);
        }
        else
#endif
        {
            if(MMIEBOOK_AUTO_READ_OFF!=MMIAPIEBOOK_GetAutoReadType())
            {
                EbookStartTimer(&s_auto_read_timer_id, GetAutoReadTime(), FALSE);
            }
        }
    }

    SCI_MEMSET(slide_Ebook_ptr,0,sizeof(MMI_SLIDE_EBOOK_T));
    MMK_ResetVelocityItem();
}
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void SetEffectBuffData(MMI_SLIDE_EBOOK_T *slide_Ebook_ptr)
{
    MMI_CTRL_ID_T ctrl_id=MMIEBOOK_TXT_SHOW_CTRL_ID;
    MMI_STRING_T  mmi_string={0};
    UILAYER_INFO_T cur_layer_info={0};
    GUI_RECT_T rect={0};
    GUI_LCD_DEV_INFO dev1=s_cur_page_dev;
    GUI_LCD_DEV_INFO dev2=s_next_page_dev;
    if(slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_TOP || slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_BOTTOM)
    {
        dev2=s_cur_page_dev;
        dev1=s_next_page_dev;
    }   
    UILAYER_GetLayerInfo(&dev1,&cur_layer_info);
    rect.right=cur_layer_info.layer_width;
    rect.bottom=cur_layer_info.layer_height;
    UILAYER_BltLayerToLayer(&dev1,&s_main_dev,&rect,FALSE);    
    MMIEBOOK_GetNextPageForEffect(slide_Ebook_ptr->turn_mode);
    GUICTRL_SetLcdDevInfo(MMK_GetCtrlPtr(ctrl_id), &dev2);
    MMIEBOOK_GetShowTxt(&mmi_string);
    GUITEXT_SetString(ctrl_id, mmi_string.wstr_ptr,mmi_string.wstr_len,TRUE);

    GUICTRL_SetLcdDevInfo(MMK_GetCtrlPtr(ctrl_id), &s_text_dev);
    
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateEffect(SE_PAGE_TURN_MODE_E mode)
{
    uint16 w=0;
    uint16 h=0;
    UILAYER_INFO_T cur_layer_info={0};
    UILAYER_INFO_T next_layer_info={0};
    SE_PAGE_TURN_INIT_PARAM_T init={0};
        
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &w, &h);
    UILAYER_GetLayerInfo(&s_cur_page_dev,&cur_layer_info);
    UILAYER_GetLayerInfo(&s_next_page_dev,&next_layer_info);
  
    init.page_width  	= w;
    init.page_height 	= h;
    init.format 		= IMGREF_FORMAT_RGB565;

    init.cur_page_ptr   = UILAYER_GetLayerBufferPtr(&s_cur_page_dev);      
    init.next_page_ptr  = UILAYER_GetLayerBufferPtr(&s_next_page_dev); 

    if ((PNULL == init.cur_page_ptr) || (PNULL == init.next_page_ptr))
    {
        return FALSE;
    }
//    init.cur_page_ptr   = cur_layer_info.layer_buf_ptr;      
//    init.next_page_ptr  = next_layer_info.layer_buf_ptr;     
    init.backface_color = 0xffff;
    /*if(mode==SE_PAGE_TURN_LEFT_TOP || mode==SE_PAGE_TURN_RIGHT_TOP)
    {
        init.mode= SE_PAGE_TURN_RIGHT_TOP;
    }
    else if(mode==SE_PAGE_TURN_LEFT_BOTTOM|| mode==SE_PAGE_TURN_RIGHT_BOTTOM)
    {
        init.mode= SE_PAGE_TURN_RIGHT_BOTTOM;
    }*/

    if(mode==SE_PAGE_TURN_LEFT_BOTTOM || mode==SE_PAGE_TURN_RIGHT_TOP)
    {
        init.mode= SE_PAGE_TURN_RIGHT_TOP;
    }
    else if(mode==SE_PAGE_TURN_LEFT_TOP|| mode==SE_PAGE_TURN_RIGHT_BOTTOM)
    {
        init.mode= SE_PAGE_TURN_RIGHT_BOTTOM;
    }

    DestroyEffect(s_effect_handle);
    s_effect_handle = SE_Create(SE_ID_PAGE_TURN,&init);
    if(NULL == s_effect_handle)                      
    {
        return FALSE;
    }

    return TRUE;
}
#endif

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void DestroyEffect(SE_HANDLE handle)
{
    if(NULL != handle)
    {
       SE_Destory(handle);
       handle=NULL;
    }
}
/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenderEffect(int16 hot_sot)
{ 
    SE_PAGE_TURN_RENDER_IN_PARAM_T 		input	= {0};	
    SE_PAGE_TURN_RENDER_OUT_PARAM_T 	output	= {0};
    SE_RESULT_E 						ret	= SE_RESULT_SUCCESS;

    UILAYER_INFO_T out_layer_info={0};

    uint32 time=0;

    UILAYER_GetLayerInfo(&s_out_page_dev,&out_layer_info);
    UILAYER_Clear(&s_out_page_dev);

    input.frame_index = -1;	
    input.hot_spot = hot_sot;
 
    input.target_buf_ptr = UILAYER_GetLayerBufferPtr(&s_out_page_dev); 

    if (PNULL == input.target_buf_ptr)
    {
        return FALSE;
    }
//    input.target_buf_ptr = out_layer_info.layer_buf_ptr; 

    input.target_buf_size = out_layer_info.layer_width*out_layer_info.layer_height*2;

    time=SCI_GetTickCount();
    ret = SE_Render(s_effect_handle, &input, &output);
    //SCI_TRACE_LOW:"RenderEffect:time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_6633_112_2_18_2_15_38_91,(uint8*)"d",SCI_GetTickCount()-time);
    if (SE_RESULT_SUCCESS != ret)
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
// Description : set common character
// Global resource dependence : 
// Author:renyi.hu
// Note: 
/*****************************************************************************/
LOCAL void UpdateMainLayerBuffer(void)
{

    UILAYER_INFO_T out_layer_info={0};
    GUI_RECT_T rect={0};
    UILAYER_GetLayerInfo(&s_out_page_dev,&out_layer_info);
    rect.right=out_layer_info.layer_width;
    rect.bottom=out_layer_info.layer_height;
    UILAYER_BltLayerToLayer(&s_main_dev,&s_out_page_dev,&rect,FALSE);   

}

/*****************************************************************************/
//  Description : HandleSlideEbookTimer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSlideEbookTimer(
                                       MMI_SLIDE_EBOOK_T *slide_Ebook_ptr, // [in]
                                       DPARAM           param           // [in]
                                       )
{
    MMI_RESULT_E        result              = MMI_RESULT_TRUE;
    GUI_POINT_T         point               = {0};
    MMI_TP_STATUS_E     state               = MMI_TP_NONE;
    int16 x_offset=0;
    //int16 y_offset=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    int32   old_pos = 0;        
    int32   cur_pos = 0;        

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);

    if (slide_Ebook_ptr->redraw_timer_id == *(uint8*)param)  // tp move timer
    {
        MMK_GetLogicTPMsg(&state, &point);
        
        // restart tp move timer
        StopSlideRedrawTimer(slide_Ebook_ptr);
        StartSlideRedrawTimer(slide_Ebook_ptr);
        
	    //UILAYER_SetDirectDraw( TRUE );
        if(slide_Ebook_ptr->slide_pre_point.x == point.x)
        {
            return MMI_RESULT_FALSE;
        }
 

        if(MMK_TP_SLIDE_TP_SCROLL==slide_Ebook_ptr->slide_state)
        {
            x_offset=point.x-slide_Ebook_ptr->slide_start_point.x;
            if(slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_RIGHT_TOP || slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_RIGHT_BOTTOM)
            {
                if(x_offset<0)
                {
                    slide_Ebook_ptr->hot_spot=abs(x_offset);
                    RenderEffect(slide_Ebook_ptr->hot_spot);
                    UpdateMainLayerBuffer();
                }
                else
                {
                    if(slide_Ebook_ptr->hot_spot>0)
                    {
                        slide_Ebook_ptr->hot_spot=0;
                        RenderEffect(slide_Ebook_ptr->hot_spot);
                        UpdateMainLayerBuffer();
                    }
                }
            }
            else if(slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_TOP || slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_BOTTOM)
            {
                if(x_offset>0)
                {
                    slide_Ebook_ptr->hot_spot=lcd_width-x_offset;
                    RenderEffect(slide_Ebook_ptr->hot_spot);
                    UpdateMainLayerBuffer();
                }
                else
                {
                    if(slide_Ebook_ptr->hot_spot<0)
                    {
                        slide_Ebook_ptr->hot_spot=lcd_width;
                        RenderEffect(slide_Ebook_ptr->hot_spot);
                        UpdateMainLayerBuffer();
                    }
                }
            }
        
            slide_Ebook_ptr->slide_pre_point.x = point.x;
            slide_Ebook_ptr->slide_pre_point.y = point.y;
        }
    }
    else if(slide_Ebook_ptr->fling_timer_id == *(uint8*)param)
    {
        if (MMK_TP_SLIDE_FLING == slide_Ebook_ptr->slide_state)
        {
            StopSlideFlingTimer(slide_Ebook_ptr);
            old_pos = slide_Ebook_ptr->slide_pre_point.x;

            // 重算加速度
            cur_pos = MMK_GetFlingOffset(
                    old_pos, slide_Ebook_ptr->velocity,
                    TOUCH_FLING_TIME, &slide_Ebook_ptr->velocity);
            
            slide_Ebook_ptr->slide_pre_point.x=cur_pos;
            
            if (FLING_RUN_MIN_VELOCITY <= abs((int32)slide_Ebook_ptr->velocity)) // 还可以加速运动
            {
                slide_Ebook_ptr->update_result=UpdateEbookLayer(slide_Ebook_ptr, cur_pos-old_pos);
                if(0==slide_Ebook_ptr->update_result)
                {
                    StartSlideFlingTimer(slide_Ebook_ptr);
                }
                else
                {
                    ExitSlideEbook(slide_Ebook_ptr,TRUE,TRUE);
                }
            }
            else    // 小于最小加速度，进入恢复状态
            {
                slide_Ebook_ptr->slide_state = MMK_TP_SLIDE_RECOVER;
                UpdateEbookRecoverMoveDirection(slide_Ebook_ptr);
                StartSlideRecoverTimer(slide_Ebook_ptr);
            }
        }
    }
    else if(slide_Ebook_ptr->recover_timer_id == *(uint8*)param)
    {
        if (MMK_TP_SLIDE_RECOVER== slide_Ebook_ptr->slide_state)
        {
            //uint8 update_result=0;
            StopSlideRecoverTimer(slide_Ebook_ptr);
            old_pos = slide_Ebook_ptr->slide_pre_point.x;
            if(slide_Ebook_ptr->recover_move_direction==1)
            {
                cur_pos =old_pos-EBOOK_SLIDE_RECOVER_STEP ;
            }
            else
            {
                cur_pos =old_pos+EBOOK_SLIDE_RECOVER_STEP ;
            }

            slide_Ebook_ptr->slide_pre_point.x=cur_pos;

            
            slide_Ebook_ptr->update_result=UpdateEbookLayer(slide_Ebook_ptr, cur_pos-old_pos);
            if(0==slide_Ebook_ptr->update_result)
            {
                StartSlideRecoverTimer(slide_Ebook_ptr);
            }
            else
            {
                ExitSlideEbook(slide_Ebook_ptr,TRUE,TRUE);
            }

        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL uint8 UpdateEbookLayer(
                               MMI_SLIDE_EBOOK_T *slide_Ebook_ptr, // [in]
                               int32 x_offset
                               )
{

    uint8 result=0;
    uint16 lcd_width=0;
    uint16 lcd_height=0;
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    
    if(x_offset>0)
    {
        slide_Ebook_ptr->hot_spot-=x_offset;
        if(slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_TOP || slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_BOTTOM)
        {
            
            if(slide_Ebook_ptr->hot_spot<=0)
            {
              slide_Ebook_ptr->hot_spot=0;
              result=2;
            }
        }
        else
        {
            if(slide_Ebook_ptr->hot_spot<=0)
            {
              slide_Ebook_ptr->hot_spot=0;
              result=1;
            } 
        }
        RenderEffect(slide_Ebook_ptr->hot_spot);
        UpdateMainLayerBuffer();
    }
    else if(x_offset<0)
    {
        x_offset= -x_offset;
        slide_Ebook_ptr->hot_spot+=x_offset;
        if(slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_TOP || slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_BOTTOM)
        {
            
            if(slide_Ebook_ptr->hot_spot>=lcd_width)
            {
              slide_Ebook_ptr->hot_spot=lcd_width;
              result=1;
            }
        }
        else
        {
            if(slide_Ebook_ptr->hot_spot>=lcd_width)
            {
              slide_Ebook_ptr->hot_spot=lcd_width;
              result=2;
            } 
        }
        RenderEffect(slide_Ebook_ptr->hot_spot);
        UpdateMainLayerBuffer();
    }
    else
    {
        //SCI_TRACE_LOW:"UpdateLayer:error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_6873_112_2_18_2_15_38_92,(uint8*)"");
    }
    
    return result;
}
/*****************************************************************************/
//  Description : handle timer
//  Global resource dependence :
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void UpdateEbookRecoverMoveDirection( MMI_SLIDE_EBOOK_T *slide_Ebook_ptr)
{
    uint16 lcd_width=0;
    uint16 lcd_height=0;

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &lcd_width, &lcd_height);
    if(slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_TOP || slide_Ebook_ptr->turn_mode==SE_PAGE_TURN_LEFT_BOTTOM)
    {
        
        if(slide_Ebook_ptr->hot_spot>(lcd_width*3)/4)
        {
            slide_Ebook_ptr->recover_move_direction=1;
        }
        else
        {
            slide_Ebook_ptr->recover_move_direction=0;
        }
    }
    else
    {
        if(slide_Ebook_ptr->hot_spot<lcd_width/2)
        {
            slide_Ebook_ptr->recover_move_direction=0;
        }
        else
        {
            slide_Ebook_ptr->recover_move_direction=1;
        }
    }

}
#endif



#ifdef MMI_TASK_MANAGER
/*****************************************************************************/
//  Description : register EBOOK module applet info
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIEBOOK_RegAppletInfo(void)
{
    MMI_RegAppletInfo(MMI_MODULE_EBOOK, &g_mmiebook_applet_info);
}

/*****************************************************************************/
//  Description : the process message function of the ebook
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  EbookApplet_HandleEvent(    
    IAPPLET_T*          iapplet_ptr,
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIEBOOK_APPLET_T* applet_ptr = (MMIEBOOK_APPLET_T*)iapplet_ptr;

    //SCI_TRACE_LOW:"mmi EbookApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIEBOOK_WINTAB_6940_112_2_18_2_15_38_93,(uint8*)"d",msg_id);
    switch (msg_id)
    {
    case MSG_APPLET_START:
        s_ebook_applet_instance = applet_ptr;
        //MMK_CreateEbookWin((uint32*)MMIEBOOK_LATEST_LIST_WIN_TAB,param);
        MMIEBOOK_OpenBookListWin(param);
        break;
        
    case MSG_APPLET_STOP: 
        s_ebook_applet_instance = PNULL;
        break;

    case MSG_APPLET_SUSPEND:
        break;
    case MSG_APPLET_RESUME:
        break;

    case MSG_APPLET_SWITCH:
        break;        
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Get ebook Applet Instance
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC MMIEBOOK_APPLET_T *MMIEBOOK_GetAppletInstance(void)
{
    return s_ebook_applet_instance;
}

/*****************************************************************************/
//  Description : Stop ebook applet
//  Global resource dependence : 
//  Author:fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIEBOOK_StopApplet(BOOLEAN is_minimize)
{
#if 0
    MMIEBOOK_APPLET_T    *applet_ptr = MMIEBOOK_GetAppletInstance();

    if (PNULL != applet_ptr)
    {
        MMI_HANDLE_T applet_handle = MMIGMBOX_HANDLE(applet_ptr);

        if (is_minimize)
        {
            //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
            MMK_HideWinByApplet(applet_handle);
        }
        else
        {
            MMK_CloseApplet(applet_handle);
        }
    }
#endif
    if (is_minimize)
    {
        //MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG_HIDE_WIN, TRUE);
        MMK_HideWinByApplet(SPRD_EBOOK_APPLET_ID);
    }
    else
    {
        MMK_CloseApplet(SPRD_EBOOK_APPLET_ID);
    }
}

#endif

/*****************************************************************************/
//     Description : SettingButtonCallBack
// Global resource dependence : 
//  Author: paul.huang
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E SettingButtonCallBack(void)
{
    MMI_WIN_ID_T win_id= MMIEBOOK_DISPLAY_SET_WIN_ID;
    MMI_CTRL_ID_T ctrl_id = MMK_GetActiveCtrlId(win_id);

    HandleSetColorCtrlMsg(win_id, ctrl_id);

    return MMI_RESULT_TRUE;
}

 /*****************************************************************************/
 //     Description : HandleDisplaySettingWinMsg
 // Global resource dependence : 
 //  Author: paul.huang
 // Note:
 /*****************************************************************************/
 LOCAL MMI_RESULT_E  HandleDisplaySettingWinMsg(
     MMI_WIN_ID_T       win_id, 
     MMI_MESSAGE_ID_E   msg_id, 
     DPARAM             param
     )
{
    MMI_RESULT_E        recode  =MMI_RESULT_TRUE;
    uint32              button_num    = 0;
    MMI_STRING_T        text            = {0};
    GUI_BG_T            button = {0};    
    GUI_BG_T txtbox_bg = {0};
    GUIFORM_CHILD_WIDTH_T  form_width={0};
    GUIFORM_CHILD_HEIGHT_T form_height={0};
    MMI_CTRL_ID_T ctrl_id = 0;
    uint16 hor_space=MMIEBOOK_COLOR_HOR;
    uint16 ver_space=MMIEBOOK_COLOR_VER;
    uint32 bg_img[]={
                        IMAGE_EBOOK_COLOR_BGICON1,
                        IMAGE_EBOOK_COLOR_BGICON2,
                        IMAGE_EBOOK_COLOR_BGICON3,
#ifndef MMI_LOW_MEMORY_EBOOK
                        IMAGE_EBOOK_COLOR_BGICON4,
                        IMAGE_EBOOK_COLOR_BGICON5,
                        IMAGE_EBOOK_COLOR_BGICON6,
#endif
                    };
    GUI_BG_T font_size_bg={0};
    //MMITHEME_BUTTON_T  button_theme={0};
    wchar font_sample[]={65,97,0};//Aa
    GUI_FONT_ALL_T font_all={0};
    GUI_BORDER_T     border={0};
    MMITHEME_FORM_T  form_theme={0};

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //form child不显示背景
        GUIFORM_PermitChildBg(MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID,FALSE);
        GUIFORM_PermitChildFont(MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID,FALSE);     

        GUIFORM_SetCircularHandleLeftRight(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL, TRUE);        
        GUIFORM_SetCircularHandleLeftRight(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2, TRUE);
        GUIFORM_SetCircularHandleLeftRight(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL, TRUE);        
        GUIFORM_SetCircularHandleLeftRight(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2, TRUE);
        GUIFORM_SetCircularHandleLeftRight(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL, TRUE);

        MMITHEME_GetFormTheme(&form_theme);
        border=form_theme.active_child.border;
        border.width=2;
        GUIFORM_SetActiveChildTheme(MMIEBOOK_FORM_DISPLAY_SET_CTRL_ID,PNULL,&border,PNULL);
        
        //字体颜色
        MMI_GetLabelTextByLang(TXT_COMMON_FONT_COLOR, &text);
        GUILABEL_SetText(MMIEBOOK_FONT_COLOR_LABEL_CTRL_ID, &text, FALSE);
        
        MMIAPIEBOOK_GetFontType();
            
        for(button_num = MMIEBOOK_BUTTON_COLOR_1_FONT;button_num <=MMIEBOOK_BUTTON_COLOR_6_FONT;button_num++)
        {
            button.bg_type = GUI_BG_COLOR;
            button.shape   = GUI_SHAPE_ROUNDED_RECT;
            button.color   = s_color_button_info[button_num];
            GUIBUTTON_SetBg(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num,&button);
            GUIBUTTON_SetRunSheen(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num,FALSE);
            GUIBUTTON_SetPressedBg(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num,&button);

            form_width.type=GUIFORM_CHILD_WIDTH_FIXED;
            form_width.add_data=MMIEBOOK_COLOR_WIDTH;
            form_height.type=GUIFORM_CHILD_HEIGHT_FIXED;
            form_height.add_data=MMIEBOOK_COLOR_HEIGHT;
            if(button_num<=MMIEBOOK_BUTTON_COLOR_3_FONT)
            {
                GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL, MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, &form_width);
                GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL, MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, &form_height);
            }
            else
            {
                GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2, MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, &form_width);
                GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2, MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, &form_height);
            }
            GUIBUTTON_SetCallBackFunc(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + button_num, SettingButtonCallBack);
        }        
        GUIFORM_SetSpace(MMIEBOOK_FORM_FONT_COLOR_CTRL_ID,&hor_space,&ver_space);
        GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL,GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_COLOR_FONT_CTRL_2,GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_COLOR1_FONT_CTRL + s_ebook_fontcolor_id,ButtonOwnerDrawFun);

        //背景颜色
        MMI_GetLabelTextByLang(TXT_EBOOK_BACK_COLOR, &text);
        GUILABEL_SetText(MMIEBOOK_BACK_COLOR_LABEL_CTRL_ID, &text, FALSE); // text

#ifndef MMI_LOW_MEMORY_EBOOK            
        for(button_num = 0;button_num <=MMIEBOOK_BUTTON_COLOR_6_BACK-MMIEBOOK_BUTTON_COLOR_1_BACK;button_num++)/*lint !e656*/
#else
        for(button_num = 0;button_num <=MMIEBOOK_BUTTON_COLOR_3_BACK-MMIEBOOK_BUTTON_COLOR_1_BACK;button_num++)/*lint !e656*/
#endif
        {
            button.bg_type = GUI_BG_IMG;
            //button.shape   = GUI_SHAPE_ROUNDED_RECT;
            button.img_id= bg_img[button_num];
            //GUIBUTTON_SetBg(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num,&button);
            GUIBUTTON_SetRunSheen(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num,FALSE);
            GUIBUTTON_SetPressedBg(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num,&button);

            form_width.type=GUIFORM_CHILD_WIDTH_FIXED;
            form_width.add_data=MMIEBOOK_COLOR_WIDTH;
            form_height.type=GUIFORM_CHILD_HEIGHT_FIXED;
            form_height.add_data=MMIEBOOK_COLOR_HEIGHT;
            if(button_num<=MMIEBOOK_BUTTON_COLOR_3_BACK-MMIEBOOK_BUTTON_COLOR_1_BACK)/*lint !e656*/
            {
                GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL, MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, &form_width);
                GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL, MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, &form_height);
            }
            else
            {
                GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2, MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, &form_width);
                GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2, MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, &form_height);
            }       
            GUIBUTTON_SetCallBackFunc(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + button_num, SettingButtonCallBack);
        }
        GUIFORM_SetSpace(MMIEBOOK_FORM_BACK_COLOR_CTRL_ID,&hor_space,&ver_space);
        GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL,GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_COLOR_BACK_CTRL_2,GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_COLOR1_BACK_CTRL + s_ebook_bg_id,ButtonOwnerDrawFun);
        
        //字体大小
        MMI_GetLabelTextByLang(TXT_COMMON_FONT_SIZE, &text);
        GUILABEL_SetText(MMIEBOOK_FONT_SIZE_LABEL_CTRL_ID, &text, FALSE); // text

        //GUIBUTTON_SetTextId(MMIEBOOK_BUTTON_FONT_SIZE1_CTRL, TXT_COMM_FONT_BIG);
        //GUIBUTTON_SetTextId(MMIEBOOK_BUTTON_FONT_SIZE2_CTRL, TXT_COMMON_NORMAL);
        //GUIBUTTON_SetTextId(MMIEBOOK_BUTTON_FONT_SIZE3_CTRL, TXT_FONT_SMALL);
        GUIBUTTON_SetText(MMIEBOOK_BUTTON_FONT_SIZE1_CTRL, font_sample,2);
        GUIBUTTON_SetText(MMIEBOOK_BUTTON_FONT_SIZE2_CTRL, font_sample,2);
        GUIBUTTON_SetText(MMIEBOOK_BUTTON_FONT_SIZE3_CTRL, font_sample,2);
        form_width.type=GUIFORM_CHILD_WIDTH_FIXED;
        form_width.add_data=MMIEBOOK_COLOR_WIDTH;
        form_height.type=GUIFORM_CHILD_HEIGHT_FIXED;
        form_height.add_data=MMIEBOOK_COLOR_HEIGHT;
        GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL, MMIEBOOK_BUTTON_FONT_SIZE1_CTRL, &form_width);
        GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL, MMIEBOOK_BUTTON_FONT_SIZE1_CTRL, &form_height);
        GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL, MMIEBOOK_BUTTON_FONT_SIZE2_CTRL, &form_width);
        GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL, MMIEBOOK_BUTTON_FONT_SIZE2_CTRL, &form_height);
        GUIFORM_SetChildWidth(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL, MMIEBOOK_BUTTON_FONT_SIZE3_CTRL, &form_width);
        GUIFORM_SetChildHeight(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL, MMIEBOOK_BUTTON_FONT_SIZE3_CTRL, &form_height);
        GUIBUTTON_SetCallBackFunc(MMIEBOOK_BUTTON_FONT_SIZE1_CTRL, SettingButtonCallBack);        
        GUIBUTTON_SetCallBackFunc(MMIEBOOK_BUTTON_FONT_SIZE2_CTRL, SettingButtonCallBack);        
        GUIBUTTON_SetCallBackFunc(MMIEBOOK_BUTTON_FONT_SIZE3_CTRL, SettingButtonCallBack);
        font_size_bg.bg_type=GUI_BG_COLOR;
        font_size_bg.color=MMI_BLACK_COLOR;
        GUIBUTTON_SetBg(MMIEBOOK_BUTTON_FONT_SIZE1_CTRL,&font_size_bg);
        GUIBUTTON_SetBg(MMIEBOOK_BUTTON_FONT_SIZE2_CTRL,&font_size_bg);
        GUIBUTTON_SetBg(MMIEBOOK_BUTTON_FONT_SIZE3_CTRL,&font_size_bg);
        //MMITHEME_GetButtonTheme(&button_theme);
        //font_all=button_theme.font;
        font_all.color=MMI_WHITE_COLOR;
        font_all.font=MMIEBOOK_ConvertFontSize(MMIEBOOK_FONT_BIG);
        GUIBUTTON_SetFont(MMIEBOOK_BUTTON_FONT_SIZE1_CTRL, &font_all);
        font_all.font=MMIEBOOK_ConvertFontSize(MMIEBOOK_FONT_NORMAL);
        GUIBUTTON_SetFont(MMIEBOOK_BUTTON_FONT_SIZE2_CTRL, &font_all);
        font_all.font=MMIEBOOK_ConvertFontSize(MMIEBOOK_FONT_SMALL);
        GUIBUTTON_SetFont(MMIEBOOK_BUTTON_FONT_SIZE3_CTRL, &font_all);
        
        GUIFORM_SetSpace(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL,&hor_space,&ver_space);
        GUIFORM_SetAlign(MMIEBOOK_FORM_BUTTON_FONT_SIZE_CTRL,GUIFORM_CHILD_ALIGN_HMIDDLE);
        GUIBUTTON_SetOwnerDraw(MMIEBOOK_BUTTON_FONT_SIZE1_CTRL + s_ebook_fontsize_id,ButtonOwnerDrawFun);
        
        //预览
        MMI_GetLabelTextByLang(STXT_PREVIEW, &text);
        GUILABEL_SetText(MMIEBOOK_PREVIEW_COLOR_LABEL_CTRL_ID, &text, FALSE); // text
        //预览效果
        MMI_GetLabelTextByLang(TXT_FREEDOM_COMMUNICATION, &text);
        GUILABEL_SetText(MMIEBOOK_PREVIEW_DISP_LABEL_CTRL_ID, &text, FALSE);    

        SetPreviewLabelColor(MMIEBOOK_PREVIEW_DISP_LABEL_CTRL_ID, 
            s_ebook_fontcolor_id,
            s_ebook_bg_id, 
            s_ebook_fontsize_id,
            FALSE );       
#ifndef MMI_LOW_MEMORY_EBOOK        
        ctrl_id = MMIEBOOK_BUTTON_COLOR1_FONT_CTRL;
#else    
        ctrl_id = MMIEBOOK_BUTTON_COLOR1_BACK_CTRL;
#endif
		
#ifdef GUI_MID_SOFTKEY_USE_IMG
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_COMMON_SOFTKEY_OK_ICON, MIDDLE_BUTTON, FALSE);
#elif defined MMI_MINI_QVGA_UI_SUPPORT
		GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_SELECT, MIDDLE_BUTTON, FALSE);  
#endif
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_FULL_PAINT:
        break;

    case MSG_NOTIFY_FORM_SWITCH_ACTIVE:
        break;  	
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
#ifdef MMI_PDA_SUPPORT
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            }
#endif     
        }
        break;
        
    case MSG_CTL_OK:     
    case MSG_APP_OK:
        //保存NV    
        SetColorType();

        GUITEXT_SetFont(MMIEBOOK_TXT_SHOW_CTRL_ID,PNULL,&s_ebook_color[s_ebook_fontcolor_id]);/*lint !e605*/
#ifndef MMI_LOW_MEMORY_EBOOK           
        if(s_ebook_bg_id<=MMIEBOOK_BG_3)
        {
            txtbox_bg.bg_type = GUI_BG_COLOR;
            txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
        }
        else                    
        {
            txtbox_bg.bg_type = GUI_BG_IMG;
            txtbox_bg.img_id=s_ebook_back_img[s_ebook_bg_id-MMIEBOOK_BG_4]; /*lint !e656*/
        }
#else
        {
            txtbox_bg.bg_type = GUI_BG_COLOR;
            txtbox_bg.color   = s_ebook_back_color[s_ebook_bg_id]; 
        }
#endif
        GUITEXT_SetBg(MMIEBOOK_TXT_SHOW_CTRL_ID,&txtbox_bg);

        MMIEBOOK_SendMsgtoTask(MMIEBOOK_MSG_FONT_SIZE_CHANGE, g_ebook_task_id, PNULL);
        //success!
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMPLETE,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        MMK_CloseWin( win_id );         
        break;
            
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:paul.huang
//  Note:
/*****************************************************************************/
LOCAL void AppendDayNightModeListItem(
     MMI_CTRL_ID_T      ctrl_id
     )
{
     uint16                      cur_selection = 0;
     MMIEBOOK_DAY_NIGHT_MODE_E     mode = MMIEBOOK_DAY_NIGHT_MODE_MAX;    

     MMIAPISET_AppendListItemByTextId( TXT_CLOSE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
     MMIAPISET_AppendListItemByTextId( TXT_EBOOK_DAY_MODE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
     MMIAPISET_AppendListItemByTextId( TXT_EBOOK_NIGHT_MODE, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
        
     mode = GetDayNightMode();
     
     cur_selection = mode;

     GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
     GUILIST_SetCurItemIndex(ctrl_id,cur_selection);         
}

/*****************************************************************************/
//  Description : 黑夜白天模式设置窗口
//  Global resource dependence : 
//  Author:paul.huang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDayNightModeWinMsg(
                                        MMI_WIN_ID_T       win_id, 
                                        MMI_MESSAGE_ID_E   msg_id, 
                                        DPARAM             param
                                        )
{
    MMI_RESULT_E   recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id  = MMIEBOOK_DAY_NIGHT_MODE_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(ctrl_id,MMIEBOOK_DAY_NIGHT_MODE_MAX, FALSE );
        AppendDayNightModeListItem(ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:       
    case MSG_APP_WEB:    
    case MSG_APP_OK:
    case MSG_CTL_OK:   
        {            
            MMIEBOOK_DAY_NIGHT_MODE_E     mode = MMIEBOOK_DAY_NIGHT_MODE_MAX;   
            uint16      cur_selection = GUILIST_GetCurItemIndex(ctrl_id);
            
            switch (cur_selection)
            {
                case MMIEBOOK_DAY_NIGHT_MODE_CLOSE:
                    mode = MMIEBOOK_DAY_NIGHT_MODE_CLOSE;
                    break;
            
                case MMIEBOOK_DAY_MODE:
                    mode = MMIEBOOK_DAY_MODE;
                    break;        
            
                case MMIEBOOK_NIGHT_MODE:
                    mode = MMIEBOOK_NIGHT_MODE;
                    break;
                    
                default:
                    mode = MMIEBOOK_DAY_MODE;
                    break;
            }
            
            SetDayNightMode(mode);

            MMK_CloseWin( win_id ); 
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
            ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
#ifdef MMI_PDA_SUPPORT
            if( MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
                break;
            }
            else if( MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
            {
                MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
                break;
            }
#endif     
            MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
LOCAL void freeVirtuaLAudioHandle(MMISRV_HANDLE_T *handle)
{
    MMISRVAUD_FreeVirtualHandle(EBOOK_AUDIO_SRV_NAME);
    *handle=0;
}
#endif

/*****************************************************************************/
//  Description : MMIAPIEBOOK_IsRunning
//  Global resource dependence : 
//  Author:renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIEBOOK_IsRunning(void)
{
    BOOLEAN result = FALSE;

    
    if (MMK_IsOpenWin(MMIEBOOK_LATEST_LIST_WIN_ID)
       ||MMK_IsOpenWin(MMIEBOOK_ENTER_TXT_CONTENT_WIN_ID))
    {
        result = TRUE; 
    }

    return result;
}
#endif


/*Edit by script, ignore 5 case. Thu Apr 26 19:00:53 2012*/ //IGNORE9527


/*Edit by script, ignore 15 case. Fri Apr 27 09:38:49 2012*/ //IGNORE9527
