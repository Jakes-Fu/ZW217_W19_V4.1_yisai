/*****************************************************************************
** File Name:      mmi_pubwin.c                                                  *
** Author:                                                                   *
** Date:           01/20/2007                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2007      Jassmine              Creat
******************************************************************************/

#define _MMI_PUBWIN_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_common_trc.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmipub_internal.h"
#include "mmi_id.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "guiblock.h"
#include "guilabel.h"
#include "mmi_theme.h"
#include "mmk_timer.h"
#include "guilcd.h"
#include "mmicc_export.h"
#include "guianim.h"
#include "guibutton.h"
#include "guistatusbar.h"
//#include "mmicountedtime_export.h"
#include "graphics_draw.h"

#include "mmi_mainmenu_export.h"
//#include "mmicc_image.h"
#include "cafapplet.h"
#include "mmi_resource.h"
#include "mmi_appmsg.h"
#include "guires.h"
#include "guitext.h"
#include "mmitheme_label.h"
#include "mmitheme_tips.h"
#include "mmitheme_text.h"
#include "mmi_autotest.h"
#include "mmidm_export.h"
#include "mmitheme_special.h"
#include "block_mem.h"
#include "guictrl_api.h"
#include "mmitheme_layer_special.h"

#include "mmimultim_image.h"
#include "mmk_tp.h"
#include "os_api.h"
#include "watch_commonwin_export.h"
#include "mmicom_trace.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define WATCH_STRING_FONT            SONG_FONT_24
#define WATCH_NOTE_LEN_1LINE         215
#define MMIPUB_WAIT_TEXT_LAEBL_NUM  MMIPUB_TEXT_MAX
#define MMIPUB_WAIT_STATUS_INDEX    0
#define MMIPUB_WAIT_PHONENUM_INDEX  2
#define MMIPUB__PROGRESS_SIZE_INFO_LEN 40
#define MMIPUB_PROGRESS_SPACE       20

#define MMIPUB_MULTILINE_MAX 4

#define MMIPUB_FRAME_NUM  8
#define MMIPUBWIN_TIPS_BG_SIDE_LEN          20                      //提示信息图片横向边距
#define MMIPUB_TIPS_DIS_HEIGHT              30

#define MMIPUB_TIPS_WIN                     MMIPUB_TIPS_WIN_ID

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
#if (defined MAINLCD_SIZE_240X320) || (defined MAINLCD_SIZE_240X400)
#define MMIPUB_HOL_GAP 3
#else
#define MMIPUB_HOL_GAP 10
#endif
#define MMIPUB_BUTTON_RECT_GAP 13
#endif
//added by hongjun.jia for bugzilla 2443/2444
#ifdef MAINLCD_SIZE_176X220 
#define MMI_PUBWIN_TEXT_BOTTOM_MARGIN 30
#endif
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
//提示信息Layer
LOCAL GUI_LCD_DEV_INFO            s_pub_tips_layer_info         = {0,UILAYER_NULL_HANDLE};
LOCAL MMI_HANDLE_T                s_pub_tips_layer_owner_handle = NULL;
LOCAL MMI_STRING_T                s_dis_text                    = {0};
LOCAL uint32                      s_time_out                    = 0xff;

LOCAL BOOLEAN                     s_tips_dis_running            = FALSE;
LOCAL MMIPUB_TIPS_DIS_STYLE_E     s_cur_tips_dis_style          = MMIPUB_TIPS_DIS_MAX;
LOCAL uint8                       s_tips_clear_timer_id         = NULL;

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
extern BOOLEAN MMK_IsHandleValid(MMI_HANDLE_T handle);//lint !e762

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//    Description : 显示提示信息字符串
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void DisplayTipsPtr(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : Tips窗口 消息处理
//  Global resource dependence : 
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTipsWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E  msg_id, 
    DPARAM param
);

/*****************************************************************************/
//    Description : 获取提示信息字符串 显示大区域
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTipsDisRect(MMIPUB_TIPS_DIS_STYLE_E dis_style);

/*****************************************************************************/
//    Description : 打开显示Tips窗口
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenTipsDisWin(MMI_WIN_ID_T *win_id_ptr);

/*****************************************************************************/
//    Description : 需要分发给前一个窗口处理的消息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MsgIsNeedSendToPreWin(MMI_MESSAGE_ID_E  msg_id);

/*****************************************************************************/
//  Description : update pubwin screen
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdatePubwinScreen(
                              MMI_WIN_ID_T       win_id
                              );

/*****************************************************************************/
//  Description : creat and open alert window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenAlertWin(
                        MMI_HANDLE_T                applet_handle,
                        uint32                      *time_period_ptr,   //time period,default is 3s
                        MMI_STRING_T                *text1_str_ptr,     //text 1
                        MMI_STRING_T                *text2_str_ptr,     //text 2
                        MMI_IMAGE_ID_T              image_id,           //alert image id
                        MMI_WIN_ID_T                *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                        MMI_WIN_PRIORITY_E          *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                        MMIPUB_SOFTKEY_STYLE_E      softkey_style,      //softkey style
                        MMIPUB_HANDLE_FUNC          handle_func,        //handle function
                        uint32                      user_data           //user data
                        );

/*****************************************************************************/
//  Description : creat and open progress window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenProgressWin(
                        MMI_HANDLE_T                applet_handle,
                        MMI_STRING_T                *text1_str_ptr,     //text 1
                        MMI_WIN_ID_T                *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                        MMI_WIN_PRIORITY_E          *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                        MMIPUB_SOFTKEY_STYLE_E      softkey_style,      //softkey style
                        MMIPUB_HANDLE_FUNC          handle_func         //handle function
                        );

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
LOCAL void OpenQueryWin(
                        MMI_HANDLE_T               applet_handle,
                        MMI_STRING_T               *text1_ptr,          //text 1,no default TXT_NULL
                        MMI_STRING_T               *text2_ptr,          //text 1,no default TXT_NULL
                        MMI_IMAGE_ID_T             image_id,           //alert image id
                        MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                        MMIPUB_HANDLE_FUNC         handle_func,         //handle function
                        uint32                     user_data
                        );

/*****************************************************************************/
//  Description : creat and open wait window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenWaitWin(
                       uint32                  type,
                       MMI_HANDLE_T            applet_handle,
                       uint8                   text_num,       //text number
                       MMI_STRING_T            *text1_ptr,     //text 1
                       MMI_STRING_T            *text2_ptr,     //text 2
                       MMI_STRING_T            *text3_ptr,     //text 3
                       MMI_WIN_ID_T            win_id,         //win id
                       MMI_IMAGE_ID_T          image_id,       //image id
                       MMI_ANIM_ID_T           anim_id,        //anim id
                       MMI_WIN_PRIORITY_E      win_priority,   //priority
                       MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                       MMIPUB_HANDLE_FUNC      handle_func,     //handle function
                       uint32                  user_data
                       );

/*****************************************************************************/
//  Description : display public window
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayProgressWin(
                              MMIPUB_INFO_T    *win_info_ptr,
                              uint32            total_size,
                              uint32            finished_size
                              );

/*****************************************************************************/
//  Description : reset pubwin, destroy all block and stop timer etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetPubwinInfo(
                           MMIPUB_INFO_T    *win_info_ptr
                           );

/*****************************************************************************/
//  Description : creat anim control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatAnimLabelCtrl(
                                 MMI_HANDLE_T   win_handle,
                                 MMI_CTRL_ID_T  ctrl_id,    //control id
                                 MMI_ANIM_ID_T  anim_id     //anim id
                                 );

/*****************************************************************************/
//  Description : get softkey text id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSoftkeyTextId(
                               MMI_TEXT_ID_T   *softkey_id,
                               MMIPUB_INFO_T   *win_info_ptr   //in
                               );

/*****************************************************************************/
//  Description : creat text box control
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T CreatTextBoxCtrl(
                                    MMI_HANDLE_T                   win_handle,     //win id
                                    MMI_CTRL_ID_T                  ctrl_id,    //control id
                                    MMI_STRING_T*                  string_ptr
                                    );

/*****************************************************************************/
//  Description : creat and open alert window
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
LOCAL void OpenAlertTextWin(
                            MMI_HANDLE_T                applet_handle,
                            uint32                      *time_period_ptr,   //time period,default is 2s
                            MMI_STRING_T                *text_str_ptr,     //text 1
                            MMI_WIN_ID_T                *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                            MMI_WIN_PRIORITY_E          *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                            MMIPUB_SOFTKEY_STYLE_E      softkey_style,      //softkey style
                            MMIPUB_HANDLE_FUNC          handle_func,        //handle function
                            uint32                      user_data           //user data
                            );

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
LOCAL void OpenQueryTextWin(
                            MMI_HANDLE_T               applet_handle,
                            MMI_STRING_T               *text_ptr,          //text,no default TXT_NULL
                            MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_QUERY_TEXT_WIN_ID
                            MMIPUB_HANDLE_FUNC         handle_func,         //handle function
                            uint32                     user_data
                            );
/*****************************************************************************/
//  Description : 类型转换
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL MMIPUB_SOFTKEY_STYLE_E ConvertStyleFromCafToGui(
                                                      CAF_MB_SOFTKEY_TYPE_E sk_style
                                                      );

/*****************************************************************************/
//  Description : set pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetPubWinText(
                         MMIPUB_INFO_T   *win_info_ptr,
                         MMI_STRING_T    **string_array,
                         uint16          string_num,
                         BOOLEAN         is_destroy
                         );

/*****************************************************************************/
//  Description : set pubwin text by index
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetPubWinTextByIndex(
                                MMIPUB_INFO_T   *win_info_ptr,
                                MMI_STRING_T    *text_str_ptr,
                                uint16          index
                                );

/*****************************************************************************/
//  Description : Set Pub window default title
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetPubWinDefaultTitle(
                                 MMIPUB_INFO_T *win_info_ptr
                                 );

/*****************************************************************************/
//  Description : set pubwin title text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetPubWinTitleText(
                              MMIPUB_INFO_T   *win_info_ptr,
                              MMI_STRING_T    *text_str_ptr
                              );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:是否横屏输入法
/*****************************************************************************/
LOCAL BOOLEAN IsImLandscape(
                            MMIPUB_INFO_T *win_info_ptr
                            );

/*****************************************************************************/
//  Description : create pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePubWinDisplayText(
                                      MMIPUB_INFO_T *win_info_ptr,
                                      MMI_STRING_T  *string_ptr
                                      );

/*****************************************************************************/
//  Description : destroy pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DestroyPubWinDisplayText(
                                    MMI_STRING_T  *string_ptr
                                    );

/*****************************************************************************/
//  Description : parse pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ParsePubWinDisplayText(
                                  MMIPUB_INFO_T *win_info_ptr
                                  );

/*****************************************************************************/
//  Description : display pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayPubWinText(
                             MMIPUB_INFO_T *win_info_ptr,
                             GUI_RECT_T rect,
                             GUI_LCD_DEV_INFO lcd_dev_info
                             );

/*****************************************************************************/
//  Description : display pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayProgressText(
                               MMIPUB_INFO_T *win_info_ptr,
                               MMI_STRING_T   string,
                               GUI_RECT_T     rect,
                               GUI_LCD_DEV_INFO lcd_dev_info
                               );

/*****************************************************************************/
//  Description : display pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayTitleText(
                            MMIPUB_INFO_T*   win_info_ptr,
                            GUI_RECT_T       title_rect,
                            GUI_LCD_DEV_INFO lcd_dev_info
                            );

/*****************************************************************************/
//  Description : set pubwin anim rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 此函数依赖于SetPubWinText的计算行数
/*****************************************************************************/
LOCAL void SetPubWinAnimRect(
                             MMIPUB_INFO_T *win_info_ptr
                             );

/*****************************************************************************/
//  Description : set pubwin textbox rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 此函数依赖于SetPubWinText的计算行数
/*****************************************************************************/
LOCAL void SetPubWinTextBoxRect(
                                MMIPUB_INFO_T *win_info_ptr
                                );

/*****************************************************************************/
//  Description : reset pubwin textbox string
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void ResetPubWinTextBoxString(
                                    MMIPUB_INFO_T *win_info_ptr
                                    );

/*****************************************************************************/
//  Description : draw pub win bg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayPubWinBg(
                           MMIPUB_INFO_T *win_info_ptr
                           );

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePubWinLayer(
                                MMIPUB_INFO_T    *win_info_ptr,  //in
                                GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                GUI_RECT_T       bg_rect,
                                BOOLEAN          is_mask_layer
                                );

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ReleasePubWinLayer(
                              GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                              );

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void AppendPubWinLayer(
                             GUI_LCD_DEV_INFO lcd_dev_info
                             );

/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence : 
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetPubWinLayer(
                                      MMIPUB_INFO_T    *win_info_ptr
                                      );

#ifndef MMIPUBWIN_PDASTYLE_SUPPORT	//pda大多数场景不显示softkey
/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence : 
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetPubWinFullBgLayer(
                                            MMIPUB_INFO_T    *win_info_ptr
                                            );
#endif

/*****************************************************************************/
//  Description : clear layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// LOCAL void ClearPubWinLayer(
//                             GUI_LCD_DEV_INFO lcd_dev_info
//                             );

/*****************************************************************************/
//  Description : display public anim
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayPubWinAnim(
                             MMIPUB_INFO_T    *win_info_ptr
                             );

/*****************************************************************************/
//  Description : display public anim
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetInlineCtrlDispLayer(
                                  MMIPUB_INFO_T    *win_info_ptr,
                                  GUI_LCD_DEV_INFO lcd_dev_info
                                  );

/*****************************************************************************/
//  Description : display public anim
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateInlineCtrl(
                            MMIPUB_INFO_T    *win_info_ptr
                            );

/*****************************************************************************/
//  Description : display public anim
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetInlineCtrlInvisible(
                                  MMIPUB_INFO_T    *win_info_ptr,
                                  BOOLEAN is_true
                                  );

/*****************************************************************************/
//  Description : set parent window softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetParentWinSoftkeyDispLayer(
                                        MMI_WIN_ID_T win_id
                                        );

/*****************************************************************************/
//  Description : is exist softkey
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsExistSoftkey(
                             MMIPUB_INFO_T *win_info_ptr
                             );

//fixed by hongjun.jia for bugzilla 2443/2444
#if defined MMIPUBWIN_PDASTYLE_SUPPORT || defined MAINLCD_SIZE_176X220
/*****************************************************************************/
//  Description : get pubwin top height
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetPubWinTopBGHeight(
                                  MMIPUB_INFO_T *win_info_ptr
                                  );

/*****************************************************************************/
//  Description : get pubwin bottom height
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetPubWinBottomBGHeight(
                                     MMIPUB_INFO_T *win_info_ptr,
                                     BOOLEAN        is_exist_skb
                                     );

/*****************************************************************************/
//  Description : display pubwin bottom bg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayPubWinBottomBG(
                                 MMIPUB_INFO_T    *win_info_ptr,
                                 GUI_RECT_T       *rect_ptr,
                                 GUI_LCD_DEV_INFO *dev_ptr,
                                 BOOLEAN           is_exist_skb
                                 );

/*****************************************************************************/
//  Description : get pubwin button rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void GetPubWinButtonRect(
                               MMIPUB_INFO_T *win_info_ptr,
                               GUI_RECT_T    rect_ptr[GUISOFTKEY_BUTTON_NUM],
                               uint32        num
                               );
#endif

/*****************************************************************************/
//  Description : get full bg rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetFullRect(
                             MMIPUB_INFO_T   *win_info_ptr
                             );

/*****************************************************************************/
//  Description : set window softkey
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetWinSoftkey(
                         MMI_WIN_ID_T win_id
                         );

/*****************************************************************************/
//  Description : query handle message
//  Global resource dependence : 
//  Author:peng.chen
//  Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_RESULT_E QueryHandleMsg(
                                         MMI_WIN_ID_T       win_id,     
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         void* param
                                         );

/*****************************************************************************/
//  Description : to handle the message of temp waiting window    
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTempWaitWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );

/*****************************************************************************/
//  Description : handle open start app waiter window
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartAppWaitWin(
                                         MMI_WIN_ID_T       win_id,     
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         );

/*****************************************************************************/
//  Description : handle query window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CAFHandleQueryWinMsg(
                                        MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        );

/*****************************************************************************/
//    Description : 清除Tips Timer消息处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void HandleClearTipsTimerEvent(
                                     uint8 timerid,
                                     uint32 param
                                     );

/*****************************************************************************/
//  Description : handle default window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:内部函数，处理默认的消息, 如close window, 防止应用未调用接口导致内存泄漏
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDefaultWinMsg(
                                       MMI_WIN_ID_T      win_id, 
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

/*****************************************************************************/
//  Description : set need update pre win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:短信等窗口需要提醒前一个窗口刷新num icon
/*****************************************************************************/
LOCAL void SetNeedPaintPreWin(
                              MMIPUB_INFO_T *win_info_ptr,
                              MMI_IMAGE_ID_T image_id
                              );

/*****************************************************************************/
//    Description : 创建提示信息Layer
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatPromptTextLayer(
                                   MMI_WIN_ID_T win_id
                                   );

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT

extern MMI_RESULT_E MMK_RunCtrlProc(
                                    MMI_HANDLE_T       ctrl_handle,
                                    MMI_MESSAGE_ID_E   msg_id,
                                    DPARAM 		       param
                                    );

extern MMI_RESULT_E MMK_RunWinProc(
                                   MMI_HANDLE_T         win_handle,
                                   MMI_MESSAGE_ID_E 	msg_id,
                                   DPARAM 				param
                                   );
#endif

/*****************************************************************************/
//  Description : creat and open alert window by text id
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWinByTextId(
                                        uint32                     *time_period_ptr,   //time period,default is 2s
                                        MMI_TEXT_ID_T              text1_id,           //text 1,no default TXT_NULL
                                        MMI_TEXT_ID_T              text2_id,           //text 2,no default TXT_NULL
                                        MMI_IMAGE_ID_T             image_id,           //alert image id
                                        MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                        MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                        MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                        MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                        )
{   
    TRACE_APP_OLD_PUPWIN("enter");

#ifdef DPHONE_SUPPORT 
    MMIPUB_OpenAlertWinByTextIdEx( MMK_GetFirstAppletHandle(), time_period_ptr, text1_id, text2_id,
        IMAGE_NULL, win_id_ptr,win_priority_ptr,softkey_style,handle_func, 0 );
#else
    MMIPUB_OpenAlertWinByTextIdEx( MMK_GetFirstAppletHandle(), time_period_ptr, text1_id, text2_id,
        image_id,win_id_ptr,win_priority_ptr,softkey_style,handle_func, 0 );
#endif
}

/*****************************************************************************/
//  Description : creat and open alert window by text id
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWinByTextIdEx(
                                          MMI_HANDLE_T               applet_handle,
                                          uint32                     *time_period_ptr,   //time period,default is 2s
                                          MMI_TEXT_ID_T              text1_id,           //text 1,no default TXT_NULL
                                          MMI_TEXT_ID_T              text2_id,           //text 2,no default TXT_NULL
                                          MMI_IMAGE_ID_T             image_id,           //alert image id
                                          MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                          MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                          MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                          MMIPUB_HANDLE_FUNC         handle_func,        //handle function
                                          uint32                     user_data           //user_data
                                          )
{
    MMI_STRING_T        text1_str = {0};
    MMI_STRING_T        text2_str = {0};
    TRACE_APP_OLD_PUPWIN("enter");

    //get text1 and text2 string
    if (TXT_NULL != text1_id)
    {
        MMI_GetLabelTextByLang(text1_id,&text1_str);
        
        MMI_TraceAutoTestOfMessage(text1_id);
    }

    if (TXT_NULL != text2_id)
    {
        MMI_GetLabelTextByLang(text2_id,&text2_str);
    }

    OpenAlertWin( applet_handle, time_period_ptr,&text1_str,&text2_str,
        image_id,win_id_ptr,win_priority_ptr,softkey_style,handle_func, user_data );
}

/*****************************************************************************/
//  Description : creat and open alert window by text pointer
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWinByTextPtr(
                                         uint32                    *time_period_ptr,   //time period,default is 3s
                                         MMI_STRING_T              *text1_ptr,         //text 1
                                         MMI_STRING_T              *text2_ptr,         //text 2
                                         MMI_IMAGE_ID_T            image_id,           //alert image id
                                         MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                         MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                         MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                         MMIPUB_HANDLE_FUNC        handle_func         //handle function
                                         )
{
    TRACE_APP_OLD_PUPWIN("enter");
    //creat and open alert window
    OpenAlertWin( MMK_GetFirstAppletHandle(), time_period_ptr,text1_ptr,text2_ptr,
        image_id,win_id_ptr,win_priority_ptr,softkey_style,handle_func, 0 );
}

/*****************************************************************************/
//  Description : creat and open alert window by text pointer
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertWinByTextPtrEx(
                                           MMI_HANDLE_T              applet_handle,
                                           uint32                    *time_period_ptr,   //time period,default is 3s
                                           MMI_STRING_T              *text1_ptr,         //text 1
                                           MMI_STRING_T              *text2_ptr,         //text 2
                                           MMI_IMAGE_ID_T            image_id,           //alert image id
                                           MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                           MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                           MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                           MMIPUB_HANDLE_FUNC        handle_func,        //handle function
                                           uint32                    user_data           //user_data
                                           )
{
    TRACE_APP_OLD_PUPWIN("enter");
    //creat and open alert window
    OpenAlertWin( applet_handle, time_period_ptr,text1_ptr,text2_ptr,
        image_id,win_id_ptr,win_priority_ptr,softkey_style,handle_func, user_data );
}

/*****************************************************************************/
//为了节省空间,新增下列4个函数，减少MMIPUB_OpenAlertWinByTextId的调用次数
/*****************************************************************************/

/*****************************************************************************/
//  Description : open success window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertSuccessWin(MMI_TEXT_ID_T text_id)
{
    TRACE_APP_OLD_PUPWIN("enter");
    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}
/*****************************************************************************/
//  Description : open warning window by text_id
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/

PUBLIC void MMIPUB_OpenAlertWarningWin(MMI_TEXT_ID_T text_id)
{
    TRACE_APP_OLD_PUPWIN("enter");
    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}
/*****************************************************************************/
//  Description :open fail window by text_id 
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/

PUBLIC void MMIPUB_OpenAlertFailWin(MMI_TEXT_ID_T text_id)
{
    TRACE_APP_OLD_PUPWIN("enter");
    MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
}

/*****************************************************************************/
//  Description :open waiting window by text_id 
//  Global resource dependence : 
//  Author: goku
//  Note:
/*****************************************************************************/

PUBLIC void MMIPUB_OpenAlertWaitingWin(MMI_TEXT_ID_T text_id)
{
    uint32 time_out = MMI_2SECONDS;
    TRACE_APP_OLD_PUPWIN("enter");

    //可调整切换系统的等待时间
    if ( TXT_SWITCHING_SYSTEM == text_id )
    {
        time_out = MMI_1SECONDS;
    }
    
    MMIPUB_OpenAlertWinByTextId( &time_out,text_id,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    
}

/*****************************************************************************/
//  Description : close alert public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseAlertWin(void)
{
    return MMK_CloseWin(MMIPUB_ALERT_WIN_ID);
}

/*****************************************************************************/
//  Description : close alert public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseAlertWinEx( 
                                      MMI_HANDLE_T  applet_handle
                                      )
{
    return MMK_CloseWin( MMK_GetWinHandle( applet_handle, MMIPUB_ALERT_WIN_ID ) );
}

/*****************************************************************************/
//  Description : handle alert window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_HandleAlertWinMsg(
                                             MMI_WIN_ID_T      win_id, 
                                             MMI_MESSAGE_ID_E  msg_id, 
                                             DPARAM            param
                                             )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    //SCI_TRACE_LOW:"MMIPUB_HandleAlertWinMsg, win_id = %d, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_2669_112_2_18_1_58_45_56,(uint8*)"dd", win_id, msg_id);
    
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
        //creat timer
        if ((0 == win_info_ptr->timer_id) &&
            (0 != win_info_ptr->time_period))
        {
            win_info_ptr->timer_id = MMK_CreateTimer(win_info_ptr->time_period,FALSE);
        }
        
        MMIPUBHandleWinMsg( win_info_ptr, msg_id, param );
        break;
        
    case MSG_LOSE_FOCUS:
        ResetPubwinInfo(win_info_ptr);
        if(0 != win_info_ptr->time_period)
        {
            MMK_CloseWin(win_id);
        }
        break;

    case MSG_TIMER:
        if(param != PNULL)
        {
            if (win_info_ptr->timer_id == *(uint8*)param)
            {
                SCI_TRACE_LOW("MMIPUB_HandleAlertWinMsg: close win");
                MMK_StopTimer(win_info_ptr->timer_id);
                win_info_ptr->timer_id = 0;
                
                MMK_CloseWin(win_id);
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    default:
        result = MMIPUBHandleWinMsg( win_info_ptr, msg_id, param );
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : set alert window text display
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetAlertWinTextByPtr(
                                        MMI_WIN_ID_T       win_id,
                                        MMI_STRING_T       *text1_ptr,         //text 1
                                        MMI_STRING_T       *text2_ptr,         //text 2
                                        BOOLEAN            is_fresh
                                        )
{ 
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMI_STRING_T    *string_array[MMIPUB_TEXT_MAX] = {0};

    if (PNULL == win_info_ptr)
    {
        return;
    }
    
    string_array[0] = text1_ptr;
    string_array[1] = text2_ptr;

    SetPubWinText( win_info_ptr, string_array, 2, FALSE );

    //SetPubWinAnimRect( win_info_ptr );

    if ( is_fresh )
    {
        //MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL );
        if(MMK_IsFocusWin(win_id))
        {
            UpdatePubwinScreen(win_id);
//            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            //临时修改花屏问题
            //MMK_UpdateScreen();
            //临时修改花屏问题
        }
    }
}

/*****************************************************************************/
//  Description : creat and open alert text window by text id
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertTextWinByTextId(
                                        uint32                     *time_period_ptr,   //time period,default is 2s
                                        MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                        MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                                        MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                        MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                        MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                        )
{   
    TRACE_APP_OLD_PUPWIN("enter");

    MMIPUB_OpenAlertTextWinByTextIdEx(
            MMK_GetFirstAppletHandle(),
            time_period_ptr, text_id, win_id_ptr,
            win_priority_ptr,softkey_style,handle_func, 0 );
}

/*****************************************************************************/
//  Description : creat and open alert text window by text id
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertTextWinByTextIdEx(
                                          MMI_HANDLE_T               applet_handle,
                                          uint32                     *time_period_ptr,   //time period,default is 2s
                                          MMI_TEXT_ID_T              text_id,           //text,no default TXT_NULL
                                          MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                                          MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                          MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                          MMIPUB_HANDLE_FUNC         handle_func,        //handle function
                                          uint32                     user_data           //user_data
                                          )
{
    MMI_STRING_T        text_str = {0};
    TRACE_APP_OLD_PUPWIN("enter");
    //get text1 and text2 string
    if (TXT_NULL != text_id)
    {
        MMI_GetLabelTextByLang(text_id,&text_str);
        MMI_TraceAutoTestOfMessage(text_id);
    }
    
    OpenAlertTextWin( applet_handle, time_period_ptr,&text_str,win_id_ptr,win_priority_ptr,softkey_style,handle_func, user_data );
}

/*****************************************************************************/
//  Description : creat and open alert window by text pointer
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlertTextWinByTextPtr(
                                         uint32                    *time_period_ptr,   //time period,default is 2s
                                         MMI_STRING_T              *text_ptr,         //text
                                         MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                                         MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                         MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                         MMIPUB_HANDLE_FUNC        handle_func         //handle function
                                         )
{
    TRACE_APP_OLD_PUPWIN("enter");
    //creat and open alert window
    OpenAlertTextWin(
            MMK_GetFirstAppletHandle(),
            time_period_ptr, text_ptr, win_id_ptr,
            win_priority_ptr,softkey_style,handle_func, 0 );
}

/*****************************************************************************/
//  Description : close alert public windows, free memory
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseAlertTextWin(void)
{
    return MMK_CloseWin(MMIPUB_ALERT_TEXT_WIN_ID);
}

/*****************************************************************************/
//  Description : creat and open query window by text id
//                当text1有值, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinByTextId(
                                        MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                        MMI_IMAGE_ID_T             image_id,           //alert image id
                                        MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                        MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                        )
{
    MMI_STRING_T        text_str = {0};
    TRACE_APP_OLD_PUPWIN("enter");
    MMI_GetLabelTextByLang(text_id,&text_str);
    MMIPUB_OpenQueryWinByTextPtr(&text_str, NULL, image_id, win_id_ptr, handle_func);
}

/*****************************************************************************/
//  Description : creat and open query window by text id
//                当text1有值, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinByTextIdEx(
                                          MMI_HANDLE_T               applet_handle,
                                          MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                          MMI_IMAGE_ID_T             image_id,           //alert image id
                                          MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                          MMIPUB_HANDLE_FUNC         handle_func,         //handle function
                                          uint32                     user_data           //user_data
                                          )
{
    MMI_STRING_T        text_str = {0};
    TRACE_APP_OLD_PUPWIN("enter");
    MMI_GetLabelTextByLang(text_id,&text_str);
    
    OpenQueryWin( applet_handle, &text_str, PNULL, image_id, win_id_ptr, handle_func, user_data );
}

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinByTextPtr(
                                         MMI_STRING_T               *text1_ptr,          //text 1,no default TXT_NULL
                                         MMI_STRING_T               *text2_ptr,          //text 1,no default TXT_NULL
                                         MMI_IMAGE_ID_T             image_id,           //alert image id
                                         MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                         MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                         )
{
    TRACE_APP_OLD_PUPWIN("enter");
    OpenQueryWin( MMK_GetFirstAppletHandle(), text1_ptr, text2_ptr, image_id, win_id_ptr, handle_func, 0 );
}

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinByTextPtrEx(
                                           MMI_HANDLE_T               applet_handle,
                                           MMI_STRING_T               *text1_ptr,          //text 1,no default TXT_NULL
                                           MMI_STRING_T               *text2_ptr,          //text 1,no default TXT_NULL
                                           MMI_IMAGE_ID_T             image_id,           //alert image id
                                           MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                           MMIPUB_HANDLE_FUNC         handle_func,         //handle function
                                           uint32                     user_data           //user_data
                                           )
{
    TRACE_APP_OLD_PUPWIN("enter");
    OpenQueryWin( applet_handle, text1_ptr, text2_ptr, image_id, win_id_ptr, handle_func, user_data );
}

/*****************************************************************************/
//  Description : Interface for customer to open query win start app
//  Global resource dependence : 
//  Author: peng.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinStartApp( uint32 guid )
{
    MMI_STRING_T text_str = {0};
    TRACE_APP_OLD_PUPWIN("enter");

    MMI_GetLabelTextByLang( TXT_MOD_DYNA_ISVALID, &text_str );

    OpenQueryWin( MMK_GetFirstAppletHandle(), &text_str, PNULL, IMAGE_PUBWIN_QUERY, PNULL, QueryHandleMsg, guid );
}

/*****************************************************************************/
//  Description : Interface for customer to open query win no file exist
//  Global resource dependence : 
//  Author: peng.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryWinFilesNoExist(void)
{ 
    TRACE_APP_OLD_PUPWIN("enter");
    MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEINVALID);
}

/*****************************************************************************/
//  Description : close query public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseQuerytWin(
                              MMI_WIN_ID_T  *win_id_ptr
                              )
{
    BOOLEAN     result = FALSE;

    if (PNULL == win_id_ptr)
    {
        result = MMK_CloseWin(MMIPUB_QUERY_WIN_ID);
    }
    else
    {
        result = MMK_CloseWin(*win_id_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : close query public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseQuerytWinEx(
                                       MMI_HANDLE_T  applet_handle,
                                       MMI_WIN_ID_T  *win_id_ptr
                                       )
{
    BOOLEAN     result = FALSE;
    
    if (PNULL == win_id_ptr)
    {
        result = MMK_CloseWin( MMK_GetWinHandle( applet_handle, MMIPUB_QUERY_WIN_ID ) );
    }
    else
    {
        result = MMK_CloseWin( MMK_GetWinHandle( applet_handle, *win_id_ptr ) );
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : handle query window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_HandleQueryWinMsg(
                                             MMI_WIN_ID_T      win_id, 
                                             MMI_MESSAGE_ID_E  msg_id, 
                                             DPARAM            param
                                             )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"MMIPUB_HandleQueryWinMsg, win_id = %d, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_4117_112_2_18_1_58_47_61,(uint8*)"dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_LOSE_FOCUS:
        ResetPubwinInfo(win_info_ptr);
        break;
    case MSG_APP_MENU:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        MMK_SendMsg(MMK_GetPrevWinId(win_info_ptr->win_handle),MSG_PROMPTWIN_OK,(DPARAM)&(win_info_ptr->win_id));
        break;

    case MSG_APP_CANCEL:
        MMK_SendMsg(MMK_GetPrevWinId(win_info_ptr->win_handle),MSG_PROMPTWIN_CANCEL,(DPARAM)&(win_info_ptr->win_id));
        break;

    default:
        result = MMIPUBHandleWinMsg( win_info_ptr, msg_id, param );
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : creat and open query window by text id
//  Global resource dependence : 
//  Author: aoke
//  Note: 使用TEXT控件显示多行文本，不带图片
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryTextWinByTextId(
                                            MMI_TEXT_ID_T              text_id,           //text 1,no default TXT_NULL
                                            MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_QUERY_TEXT_WIN_ID
                                            MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                            )
{
    MMI_STRING_T        text_str = {0};
    TRACE_APP_OLD_PUPWIN("enter");
    MMI_GetLabelTextByLang(text_id,&text_str);
    MMIPUB_OpenQueryTextWinByTextPtr(&text_str,win_id_ptr, handle_func);
}

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenQueryTextWinByTextPtr(
                                             MMI_STRING_T               *text_ptr,          //text 1,no default TXT_NULL
                                             MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_QUERY_TEXT_WIN_ID
                                             MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                             )
{
    TRACE_APP_OLD_PUPWIN("enter");
    OpenQueryTextWin( MMK_GetFirstAppletHandle(), text_ptr, win_id_ptr, handle_func, 0 );
}

/*****************************************************************************/
//  Description : close query public windows, free memory
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseQuerytTextWin(
                                         MMI_WIN_ID_T  *win_id_ptr
                                         )
{
    BOOLEAN     result = FALSE;
    
    if (PNULL == win_id_ptr)
    {
        result = MMK_CloseWin(MMIPUB_QUERY_TEXT_WIN_ID);
    }
    else
    {
        result = MMK_CloseWin(*win_id_ptr);
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : creat and open wait window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenWaitWin(
                               uint8                   text_num,       //text number
                               MMI_STRING_T            *text1_ptr,     //text 1
                               MMI_STRING_T            *text2_ptr,     //text 2
                               MMI_STRING_T            *text3_ptr,     //text 3
                               MMI_WIN_ID_T            win_id,         //win id
                               MMI_IMAGE_ID_T          image_id,       //image id
                               MMI_ANIM_ID_T           anim_id,        //anim id
                               MMI_WIN_PRIORITY_E      win_priority,   //priority
                               MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                               MMIPUB_HANDLE_FUNC      handle_func     //handle function
                               )
{
    
    TRACE_APP_OLD_PUPWIN("enter");
#ifdef DPHONE_SUPPORT 
    OpenWaitWin( MMIPUBWIN_WAIT, MMK_GetFirstAppletHandle(), text_num, text1_ptr, text2_ptr, text3_ptr, win_id, image_id, NULL, win_priority, softkey_style, handle_func, 0 );
#else
    OpenWaitWin( MMIPUBWIN_WAIT, MMK_GetFirstAppletHandle(), text_num, text1_ptr, text2_ptr, text3_ptr, win_id, image_id, anim_id, win_priority, softkey_style, handle_func, 0 );
#endif 
}

/*****************************************************************************/
//  Description : creat and open alarm dialog window
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAlarmDialogWin(
                               uint8                   text_num,       //text number
                               MMI_STRING_T            *text1_ptr,     //text 1
                               MMI_STRING_T            *text2_ptr,     //text 2
                               MMI_STRING_T            *text3_ptr,     //text 3
                               MMI_WIN_ID_T            win_id,         //win id
                               MMI_IMAGE_ID_T          image_id,       //image id
                               MMI_ANIM_ID_T           anim_id,        //anim id
                               MMI_WIN_PRIORITY_E      win_priority,   //priority
                               MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                               MMIPUB_HANDLE_FUNC      handle_func     //handle function
                               )
{
    OpenWaitWin( MMIPUBWIN_ALARM_DIALOG, MMK_GetFirstAppletHandle(), text_num, text1_ptr, text2_ptr, text3_ptr, win_id, image_id, anim_id, win_priority, softkey_style, handle_func, 0 );
}

/*****************************************************************************/
//  Description : creat and open wait window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenWaitWinEx(
                                 MMI_HANDLE_T            applet_handle,
                                 uint8                   text_num,       //text number
                                 MMI_STRING_T            *text1_ptr,     //text 1
                                 MMI_STRING_T            *text2_ptr,     //text 2
                                 MMI_STRING_T            *text3_ptr,     //text 3
                                 MMI_WIN_ID_T            win_id,         //win id
                                 MMI_IMAGE_ID_T          image_id,       //image id
                                 MMI_ANIM_ID_T           anim_id,        //anim id
                                 MMI_WIN_PRIORITY_E      win_priority,   //priority
                                 MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                                 MMIPUB_HANDLE_FUNC      handle_func,    //handle function
                                 uint32                  user_data       //user_data
                                 )
{
    TRACE_APP_OLD_PUPWIN("enter");
    OpenWaitWin( MMIPUBWIN_WAIT, applet_handle, text_num, text1_ptr, text2_ptr, text3_ptr,
        win_id, image_id, anim_id, win_priority, softkey_style, handle_func, user_data );
}

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************/
//  Description : open waiting win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:android风格等待窗口
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_OpenWaitingWin(
                                     MMIPUB_WAITING_PARAM_T* param
                                     )
{
    MMI_STRING_T text_str = {0};
    MMI_WIN_ID_T win_id = 0;
    MMI_HANDLE_T applet_handle = 0;
    
    TRACE_APP_OLD_PUPWIN("enter");
    if ( PNULL == param )
    {
        return FALSE;
    }

    if ( 0 == param->win_id )
    {
        win_id = MMIPUB_WAITING_WIN_ID;
    }
    else
    {
        win_id = param->win_id;
    }

    if ( 0 == param->applet_handle )
    {
        applet_handle = MMK_GetFirstAppletHandle();
    }
    else
    {
        applet_handle = param->applet_handle;
    }
    
    if ( 0 == param->wait_id )
    {
        text_str = param->wait_string;   
    }
    else
    {
        MMI_GetLabelTextByLang( param->wait_id, &text_str );   
    }

    OpenWaitWin( MMIPUBWIN_WAITING, applet_handle, 1, &text_str, PNULL, PNULL, win_id, 0, 
        ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, param->user_func, 0 );

    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : open temp wait win, when lose_focus close
//  Global resource dependence : 
//  Author: bin.ji
//  Note: 可以用此函数弹出等待窗口，提前促使释放窗口内存
/*****************************************************************************/
PUBLIC void MMIPUB_OpenTempWaitWin(void)
{
    MMI_WINDOW_CREATE_T win_create = {0};
    TRACE_APP_OLD_PUPWIN("enter");
    BLKMEM_GarbageClean();
    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id = MMIPUB_TEMPWAITING_WIN_ID;
    win_create.func = HandleTempWaitWinMsg;
    win_create.win_priority = WIN_ONE_LEVEL;
    win_create.add_data_ex_ptr = PNULL;
    win_create.window_style = WS_NO_DEFAULT_STYLE;//WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN; 
    win_create.window_move_style = MOVE_FORBIDDEN;
    if( 0 == MMK_CreateWindow( &win_create )  )
    {
        //SCI_TRACE_LOW:"MMIPUB_OpenTempWaitWin() MMK_CreateWindow fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_6613_112_2_18_1_58_53_70,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : open temp wait win, and then close it.
//  Global resource dependence : 
//  Author: bin.ji
//  Note: Pop up a temp win and then close it immediatlly, normlly, it's used when some window need a
//          MSG_GET_FOCUS msg.
/*****************************************************************************/
PUBLIC void MMIPUB_OpenAndCloseTempWin(void)
{
    TRACE_APP_OLD_PUPWIN("enter");
    MMIPUB_OpenTempWaitWin();
    MMK_CloseWin(MMIPUB_TEMPWAITING_WIN_ID);
}

/*****************************************************************************/
//  Description : Interface for customer to open query win start app
//  Global resource dependence : 
//  Author: peng.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenStartAppWaitWin( uint32 guid )
{
    MMI_STRING_T text_str = {0};
    TRACE_APP_OLD_PUPWIN("enter");

    MMI_GetLabelTextByLang( TXT_COMMON_WAITING, &text_str );

    OpenWaitWin( MMIPUBWIN_WAIT, MMK_GetFirstAppletHandle(), 1, &text_str, PNULL, PNULL, MMIPUB_WAIT_WIN_ID,
        COMMON_IMAGE_NULL, ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, HandleStartAppWaitWin, guid );

    MMK_PostMsg( MMIPUB_WAIT_WIN_ID, MSG_PUB_START_APP_WAIT, NULL, 0 );
}

/*****************************************************************************/
//  Description : close wait public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseWaitWin(
                                   MMI_WIN_ID_T    win_id
                                   )
{
    return MMK_CloseWin(win_id);
}

/*****************************************************************************/
//  Description : close wait public windows, free memory
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_CloseWaitWinEx(
                                     MMI_HANDLE_T    applet_handle,
                                     MMI_WIN_ID_T    win_id
                                     )
{
    return MMK_CloseWin( MMK_GetWinHandle( applet_handle, win_id ) );
}

/*****************************************************************************/
//  Description : handle wait window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_HandleWaitWinMsg(
                                            MMI_WIN_ID_T      win_id, 
                                            MMI_MESSAGE_ID_E  msg_id,
                                            DPARAM            param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"MMIPUB_HandleWaitWinMsg, win_id = %d, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_4435_112_2_18_1_58_48_63,(uint8*)"dd", win_id, msg_id);

    switch (msg_id)
    {   
    case MSG_APP_OK:
        if ((0 != win_info_ptr->softkey_id[0]) &&
            (TXT_NULL != win_info_ptr->softkey_id[0]))
        {
            MMK_SendMsg(MMK_GetPrevWinId(win_id),MSG_PROMPTWIN_OK,PNULL);
        }
        break;
        
    case MSG_APP_CANCEL:
        if ((0 != win_info_ptr->softkey_id[2]) &&
            (TXT_NULL != win_info_ptr->softkey_id[2]))
        {
            MMK_SendMsg(MMK_GetPrevWinId(win_id),MSG_PROMPTWIN_CANCEL,PNULL);
        }
        break;

    default:
        result = MMIPUBHandleWinMsg( win_info_ptr, msg_id, param );
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set wait window text 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWaitWinText(
                                  BOOLEAN          is_fresh,   //is fresh
                                  uint8            text_num,   //text number
                                  MMI_STRING_T     *text1_ptr, //text 1
                                  MMI_STRING_T     *text2_ptr, //text 2
                                  MMI_STRING_T     *text3_ptr, //text 3
                                  MMI_WIN_ID_T     win_id
                                  )
{
    MMIPUB_INFO_T *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        MMI_STRING_T    *string_array[MMIPUB_TEXT_MAX] = {0};
        
        string_array[0] = text1_ptr;
        string_array[1] = text2_ptr;
        string_array[2] = text3_ptr;

        SetPubWinText( win_info_ptr, string_array, text_num, FALSE );

        //SetPubWinAnimRect( win_info_ptr );

        if(is_fresh && MMK_IsFocusWin(win_id))
        {
            UpdatePubwinScreen(win_id);
//            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            //临时修改花屏问题
            //MMK_UpdateScreen();
            //临时修改花屏问题
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIPUB_SetWaitWinText:win_id %d is not open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_4501_112_2_18_1_58_48_64,(uint8*)"d",win_id);
    }
}

/*****************************************************************************/
//  Description : set wait window text 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWaitWinTextEx(
                                  BOOLEAN          is_fresh,   //is fresh
                                  uint8            text_num_index,   //text number index
                                  MMI_STRING_T     *text_ptr, //text
                                  MMI_WIN_ID_T     win_id
                                  )
{
    MMIPUB_INFO_T *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        SetPubWinTextByIndex( win_info_ptr, text_ptr, text_num_index );

        ParsePubWinDisplayText( win_info_ptr );
        
        //SetPubWinAnimRect( win_info_ptr );
        //SetPubWinTextBoxRect( win_info_ptr );
        ResetPubWinTextBoxString( win_info_ptr );

        if(is_fresh && MMK_IsFocusWin(win_id))
        {
            UpdatePubwinScreen(win_id);
//            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            //临时修改花屏问题
            //MMK_UpdateScreen();
            //临时修改花屏问题
        }
    }
}

// add by @shengjie.liu for cr88570 -start
/*****************************************************************************/
//  Description : set wait window text 
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_UpdateWaitWinText(
                                     BOOLEAN          is_fresh,   //is fresh
                                     uint8            text_num,   //text number
                                     MMI_STRING_T     *text1_ptr, //text 1
                                     MMI_STRING_T     *text2_ptr, //text 2
                                     MMI_STRING_T     *text3_ptr, //text 3
                                     MMI_WIN_ID_T     win_id
                                     )
{
    MMIPUB_SetWaitWinText( is_fresh, text_num, text1_ptr, text2_ptr, text3_ptr, win_id );
}

/*****************************************************************************/
//  Description : creat and open progress window
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenProgressWinEx(
                        MMI_HANDLE_T                applet_handle,
                        MMI_STRING_T                *text1_str_ptr,     //text 1
                        MMI_WIN_ID_T                *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                        MMI_WIN_PRIORITY_E          *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                        MMIPUB_SOFTKEY_STYLE_E      softkey_style,      //softkey style
                        MMIPUB_HANDLE_FUNC          handle_func         //handle function
                        )
{
    TRACE_APP_OLD_PUPWIN("enter");
    OpenProgressWin(applet_handle, text1_str_ptr, win_id_ptr, win_priority_ptr, softkey_style, handle_func);
}

/*****************************************************************************/
//  Description : creat and open progress window by text id
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: J
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenProgressWinByTextId(
                                 MMI_TEXT_ID_T              text1_id,           //text 1,no default TXT_NULL
                                 MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                 MMI_WIN_PRIORITY_E         *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                 MMIPUB_SOFTKEY_STYLE_E     softkey_style,      //softkey style
                                 MMIPUB_HANDLE_FUNC         handle_func         //handle function
                                 )
{
    MMI_STRING_T        text1_str = {0};
    TRACE_APP_OLD_PUPWIN("enter");

    //get text1 and text2 string
    if (TXT_NULL != text1_id)
    {
        MMI_GetLabelTextByLang(text1_id,&text1_str);
        
        MMI_TraceAutoTestOfMessage(text1_id);
    }

    OpenProgressWin( MMK_GetFirstAppletHandle(), &text1_str, win_id_ptr,win_priority_ptr,softkey_style,handle_func);
}

/*****************************************************************************/
//  Description : creat and open progress window by text pointer
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_OpenProgressWinByTextPtr(
                                  MMI_STRING_T              *text1_ptr,         //text 1
                                  MMI_WIN_ID_T              *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                                  MMI_WIN_PRIORITY_E        *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                                  MMIPUB_SOFTKEY_STYLE_E    softkey_style,      //softkey style
                                  MMIPUB_HANDLE_FUNC        handle_func         //handle function
                                  )
{
    TRACE_APP_OLD_PUPWIN("enter");
    OpenProgressWin(MMK_GetFirstAppletHandle(), text1_ptr, win_id_ptr,win_priority_ptr,softkey_style,handle_func);
}

/*****************************************************************************/
//  Description : close progress windows
//                
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_CloseProgressWin(MMI_WIN_ID_T *win_id_ptr)
{
    MMI_WIN_ID_T win_id = 0;

    if(PNULL == win_id_ptr)
    {
        win_id = MMIPUB_PROGRESS_WIN_ID;
    }
    else
    {
        win_id = *win_id_ptr;
    }
    MMK_CloseWin(win_id);
}

/*****************************************************************************/
//  Description : handle progress window message
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_HandleProgressWinMsg(
                                                MMI_WIN_ID_T      win_id, 
                                                MMI_MESSAGE_ID_E  msg_id, 
                                                DPARAM            param
                                                )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    static uint32   s_total_size  = 0;
    static uint32   s_finished_size  = 0;

    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    // win_info_ptr->win_id = win_id;
    
    //SCI_TRACE_LOW:"MMIPUB_HandleProgressWinMsg, win_id = %d, msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_2450_112_2_18_1_58_44_55,(uint8*)"dd", win_id, msg_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MMI_RES_ORIENT_PORTRAIT
#ifndef MMI_LOW_MEMORY_RESOURCE
        if(MMITHEME_IsMainScreenLandscape())
        {
            GUIWIN_SetSoftkeyPressedStyle(win_id,
                MMITHEME_GetCurThemeFontColor(MMI_THEME_SOFTKEY_FONT), 
                GUI_SOFTKEY_BG_IMAGE_ONLY, 
                0, 
                IMAGE_SOFTKEY_BG_H_DOWN);
        }
#endif
#endif
        SetWinSoftkey(win_id);
        s_finished_size = 0;
        break;

    case MSG_FULL_PAINT:
        //display pubwin
        if ( !win_info_ptr->is_layout )
        {
            ParsePubWinDisplayText( win_info_ptr );
            
            MMIPUB_ReleaseLayer( win_info_ptr );

            MMIPUBLayout( win_info_ptr );
            MMIPUBSetButton( win_info_ptr, FALSE );
            SetPubWinTextBoxRect( win_info_ptr );
        }

        DisplayProgressWin(win_info_ptr, s_total_size, s_finished_size);
        break;

    case MSG_PUB_PROGRESS_SET_TOTAL_SIZE:
        //set total size
        s_total_size = (uint32)param;
        break;

    case MSG_PUB_PROGRESS_UPDATE_SIZE:
        //update current progress
        s_finished_size = (uint32)param;
//         if(MMK_IsFocusWin(win_id))
//         {
//             //MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
//             //临时修改花屏问题
//             MMK_UpdateScreen();
//             //临时修改花屏问题
//         }
        break;

    default:
        result = MMIPUBHandleWinMsg( win_info_ptr, msg_id, param );
        break;
    }

    return (result);    
}

/*****************************************************************************/
//  Description : set progress bar text display
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetProgressBarTextByPtr(
                                        MMI_WIN_ID_T       win_id,
                                        MMI_STRING_T       *text1_ptr,         //text 1
                                        BOOLEAN            is_fresh
                                        )
{ 
    MMIPUB_INFO_T *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMI_STRING_T *string_array[MMIPUB_TEXT_MAX] = {0};

    if (PNULL == win_info_ptr)
    {
        return;
    }
    
    string_array[0] = text1_ptr;

    SetPubWinText( win_info_ptr, string_array, 1, FALSE );

    //SetPubWinAnimRect( win_info_ptr );

    if ( is_fresh )
    {
        //MMK_SendMsg( win_id, MSG_FULL_PAINT, PNULL );
        if(MMK_IsFocusWin(win_id))
        {
            UpdatePubwinScreen(win_id);
//            MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            //临时修改花屏问题
            //MMK_UpdateScreen();
            //临时修改花屏问题
        }
    }
}

/*****************************************************************************/
//  Description : set progress win total size
//                
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetProgressTotalSize(MMI_WIN_ID_T *win_id_ptr, uint32 total_size)
{
    MMI_WIN_ID_T win_id = 0;

    if(PNULL == win_id_ptr)
    {
        win_id = MMIPUB_PROGRESS_WIN_ID;
    }
    else
    {
        win_id = *win_id_ptr;
    }

    MMK_SendMsg(win_id, MSG_PUB_PROGRESS_SET_TOTAL_SIZE, (DPARAM)total_size);
}

/*****************************************************************************/
//  Description : update progress
//                
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_UpdateProgressBar(
                                     MMI_WIN_ID_T   *win_id_ptr,
                                     uint32         finished_size
                                     )
{
    MMIPUB_UpdateProgressBarEx( win_id_ptr, finished_size, FALSE );
}

/*****************************************************************************/
//  Description : update progress
//                
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_UpdateProgressBarEx(MMI_WIN_ID_T *win_id_ptr, uint32 finished_size, BOOLEAN is_flash )
{
    MMI_WIN_ID_T win_id = 0;

    if(PNULL == win_id_ptr)
    {
        win_id = MMIPUB_PROGRESS_WIN_ID;
    }
    else
    {
        win_id = *win_id_ptr;
    }
    MMK_SendMsg(win_id, MSG_PUB_PROGRESS_UPDATE_SIZE, (DPARAM)finished_size);

    if ( is_flash )
    {
        UpdatePubwinScreen(win_id);
//        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
    }
}

/*****************************************************************************/
//  Description : open pubwin win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIPUB_OpenWin(
                                   MMIPUB_PARAM_T* param
                                   )
{
    BOOLEAN              result = FALSE;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    const MMIPUB_INIT_T* init_ptr = PNULL;
#endif
    MMI_WIN_ID_T         win_id = 0;
    MMI_HANDLE_T         applet_handle = 0;
    MMI_HANDLE_T         win_handle = 0;
    MMI_WIN_PRIORITY_E   win_priority = WIN_ONE_LEVEL;
    MMIPUB_INFO_T*       win_info_ptr = PNULL;
    MMI_WINDOW_CREATE_T  win_create = {0};
    
    TRACE_APP_OLD_PUPWIN("enter");
    if ( PNULL == param
        || PNULL == param->user_func )
    {
        //SCI_TRACE_LOW:"MMIPUB_OpenEditWin param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_6289_112_2_18_1_58_53_68,(uint8*)"");
        return 0;
    }

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    if ( SPRD_GUI_LIST_ID == param->guid )
    {
        init_ptr = &g_publistwin_init;
    }
    else if ( SPRD_GUI_EDITBOX_ID == param->guid || SPRD_GUI_FORM_ID == param->guid )
    {
        init_ptr = &g_pubformwin_init;
    }
    else
    {
        return 0;
    }
#endif

    if ( 0 != param->win_id )
    {
        win_id = param->win_id;
    }
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    else
    {
        win_id = init_ptr->default_win_id;
    }
#endif

    if ( 0 == param->applet_handle )
    {
        applet_handle = MMK_GetFirstAppletHandle();
    }
    else
    {
        applet_handle = param->applet_handle;
    }
    
    //judge window is already open
    if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
    {   
        //set window parameter
        win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_INFO_T));
        SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_INFO_T));

        //set softkey style
        win_info_ptr->softkey_style = MMIPUB_SOFTKEY_CUSTOMER;
        win_info_ptr->softkey_id[0] = param->left_sk_id;
        win_info_ptr->softkey_id[1] = param->middle_sk_id;
        win_info_ptr->softkey_id[2] = param->right_sk_id;
        win_info_ptr->win_id = win_id;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        win_info_ptr->type = init_ptr->type;
#endif
        //get pubwin theme
        MMITHEME_GetPubTheme( win_info_ptr->type, &win_info_ptr->pubwin_theme );
        
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
		if (MMIPUBWIN_LIST == win_info_ptr->type
			|| MMIPUBWIN_FORM == win_info_ptr->type)
		{        
			win_info_ptr->prompt_image_id = IMAGE_PUBWIN_DOWNARROW;
		}
#endif

        win_create.applet_handle = applet_handle;
        win_create.win_id = win_id;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        win_create.func = init_ptr->proc_func;
#endif
        win_info_ptr->user_func = param->user_func;
        win_create.win_priority = win_priority;
        win_create.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;
        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
        win_create.add_data_ptr = param->add_data_ptr;
        win_create.add_data_ex_ptr = win_info_ptr;
        
        if( 0 != ( win_handle = win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }
        
        if (result)
        {
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            if ( PNULL != init_ptr->init_func )
            {
                init_ptr->init_func( win_info_ptr, param );
            }
 #endif

            MMIPUBCreatSoftkeyCtrl( win_handle );
            MMIPUB_SetWinTitleTextId( win_handle, param->title_id, FALSE );
        }
        
        if (!result)
        {
            if (!MMK_IsOpenWin( win_handle ))
            {
                SCI_FREE(win_info_ptr);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIPUB_OpenWin:win_id %d is already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_6390_112_2_18_1_58_53_69,(uint8*)"d", win_id);
    }

    return win_handle;
}

/*****************************************************************************/
//  Description : Interface for customer to open query win start app
//  Global resource dependence : 
//  Author: peng.chen
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_MessageBox( 
                              const CAF_MESSAGEBOX_CREATE_T* msgbox_ptr
                              ) 
{
    MMI_WIN_ID_T       win_id        = 0;
    MMI_HANDLE_T       applet_handle = 0;
    MMI_IMAGE_ID_T     image_id      = IMAGE_NULL;
    MMI_STRING_T       text1_str     = {0};
    MMI_STRING_T       text2_str     = {0};
    MMI_HANDLE_TYPE_E  handle_type   = 0;

    if ( PNULL == msgbox_ptr )
    {
        return;
    }

    win_id = MMIAPICOM_GetInvalidWinId() - 1;

    handle_type = MMK_GetHandleType( msgbox_ptr->parent_handle );
    
    if( MMK_IsHandleValid( msgbox_ptr->parent_handle )
        && ( handle_type == MMI_HANDLE_APPLET || handle_type == MMI_HANDLE_WINDOW ) )
    {
        if( handle_type == MMI_HANDLE_WINDOW )
        {
            applet_handle = IAPPLET_GetAppletHandle( MMK_GetAppletPtrByWin( msgbox_ptr->parent_handle ) );
        }
        else
        {
            applet_handle = msgbox_ptr->parent_handle;
        }
        
        
        if ( PNULL == msgbox_ptr->text1_str )
        {
            MMIRES_GetText( msgbox_ptr->text1_id, applet_handle, &text1_str );
        }
        else
        {
            text1_str = *msgbox_ptr->text1_str;
        }
        
        if ( PNULL == msgbox_ptr->text2_str )
        {
            MMIRES_GetText( msgbox_ptr->text2_id, applet_handle, &text2_str );
        }
        else
        {
            text2_str = *msgbox_ptr->text2_str;
        }

        switch( msgbox_ptr->type )
        {           
        case CAF_MB_ALERT:
            if( 0 == msgbox_ptr->image_id )
            {
                image_id = IMAGE_PUBWIN_WARNING;
            }

            //note: this function must use handle!!!
            OpenAlertWin( applet_handle, msgbox_ptr->time_period_ptr, &text1_str, &text2_str, 
                image_id, &win_id, PNULL, ConvertStyleFromCafToGui(msgbox_ptr->sk_style), PNULL, 0 );
            
            break;
            
        case CAF_MB_QUERY:
            if( 0 == msgbox_ptr->image_id )
            {
                image_id = IMAGE_PUBWIN_QUERY;
            }

            //note: this function must use handle!!!
            OpenQueryWin( applet_handle, &text1_str, &text2_str, 
                image_id, &win_id, CAFHandleQueryWinMsg, msgbox_ptr->parent_handle ); //will send message to parent handle
            break;
            
        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : set pub win add data
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_SetWinAddData(
                                    MMI_HANDLE_T win_handle,
                                    uint32       user_data
                                    )
{
    BOOLEAN result = FALSE;
	MMIPUB_INFO_T  *win_info_ptr = PNULL;

    if (MMK_GetWinDisplayStyleState(win_handle, WS_HAS_PROMPT_WIN))
    {
        win_info_ptr = (MMIPUB_INFO_T*)MMK_GetWinAddDataEx( win_handle );
		if (PNULL != win_info_ptr)
		{
		    win_info_ptr->user_data = user_data;
			result = TRUE;
		}
    }

	return result;
}

/*****************************************************************************/
//  Description : get pub win add data
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMIPUB_INFO_T* MMIPUB_GetWinAddDataPtr(
                                              MMI_HANDLE_T win_handle
                                              )
{
    if ( MMK_GetWinDisplayStyleState( win_handle, WS_HAS_PROMPT_WIN ) )
    {
        return (MMIPUB_INFO_T*)MMK_GetWinAddDataEx( win_handle );
    }
    else
    {
        return PNULL;
    }
}

/*****************************************************************************/
//  Description : set pubwin window title text id
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinTitleTextId(
                                     MMI_WIN_ID_T     win_id,
                                     MMI_TEXT_ID_T    text_id,
                                     BOOLEAN          is_fresh
                                     )
{
    MMI_STRING_T text_str = {0};
    
    MMI_GetLabelTextByLang( text_id, &text_str );

    MMIPUB_SetWinTitleText( win_id, &text_str, is_fresh );
}

/*****************************************************************************/
//  Description : set pubwin window title text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinTitleText(
                                   MMI_WIN_ID_T     win_id,
                                   MMI_STRING_T     *text_ptr,
                                   BOOLEAN          is_fresh
                                   )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    
    if (PNULL != win_info_ptr)
    {
        SetPubWinTitleText( win_info_ptr, text_ptr );

        if(is_fresh && MMK_IsFocusWin(win_id))
        {
            UpdatePubwinScreen(win_id);
        }
    }
}

/*****************************************************************************/
//  Description : set pubwin window softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinSoftkey(
                                 MMI_WIN_ID_T          win_id,
                                 MMI_TEXT_ID_T         left_sk_id,
                                 MMI_TEXT_ID_T         right_sk_id,
                                 BOOLEAN               is_fresh
                                 )
{
    MMIPUB_SetWinSoftkeyEx( win_id, left_sk_id, TXT_NULL, right_sk_id, is_fresh );
}


/*****************************************************************************/
//  Description : set pubwin window softkey
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
// 如果ID为0，则不修改原内容
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinSoftkeyEx(
                                   MMI_WIN_ID_T          win_id,
                                   MMI_TEXT_ID_T         left_sk_id,
                                   MMI_TEXT_ID_T         middle_sk_id,
                                   MMI_TEXT_ID_T         right_sk_id,
                                   BOOLEAN               is_fresh
                                   )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        if (0 != left_sk_id)
        {
            win_info_ptr->softkey_id[0] = left_sk_id;
        }

        if (0 != middle_sk_id)
        {
            win_info_ptr->softkey_id[1] = middle_sk_id;
        }

        if (0 != right_sk_id)
        {
            win_info_ptr->softkey_id[2] = right_sk_id;
        }

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            if(is_fresh && MMK_IsFocusWin(win_id))
            {
                MMIPUBSetButton( win_info_ptr, TRUE );
            }
            else
            {
                MMIPUBSetButton( win_info_ptr, FALSE );
            }
#else
        GUIWIN_SetSoftkeyTextId(win_id,  left_sk_id, middle_sk_id, right_sk_id, is_fresh);
#endif
    }
    else
    {
        //SCI_TRACE_LOW:"MMIPUB_SetWinSoftkeyEx:win_id %d is not open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_3713_112_2_18_1_58_47_57,(uint8*)"d",win_id);
    }
}

/*****************************************************************************/
//  Description : get pubwin window softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_GetWinSoftkey(
                                 MMI_WIN_ID_T          win_id,
                                 MMI_TEXT_ID_T         *left_sk_id_ptr,
                                 MMI_TEXT_ID_T         *middle_sk_id_ptr,
                                 MMI_TEXT_ID_T         *right_sk_id_ptr
                                 )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        if (PNULL != left_sk_id_ptr)
        {
            *left_sk_id_ptr = win_info_ptr->softkey_id[0];
        }

        if (PNULL != middle_sk_id_ptr)
        {
            *middle_sk_id_ptr = win_info_ptr->softkey_id[1];
        }

        if (PNULL != right_sk_id_ptr)
        {
            *right_sk_id_ptr = win_info_ptr->softkey_id[2];
        }
#else
        GUIWIN_GetSoftkeyTextId(win_id, left_sk_id_ptr, middle_sk_id_ptr, right_sk_id_ptr);
#endif
    }
    else
    {
        //SCI_TRACE_LOW:"MMIPUB_SetWinSoftkeyEx:win_id %d is not open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_3753_112_2_18_1_58_47_58,(uint8*)"d",win_id);
    }
}

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
/*****************************************************************************/
//  Description : set pubwin window softkey gray
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinSoftkeyGray(
                                     MMI_WIN_ID_T    win_id,
                                     BOOLEAN         is_left_gray,
                                     BOOLEAN         is_middle_gray,
                                     BOOLEAN         is_right_gray,
                                     BOOLEAN         is_fresh
                                     )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        if ( win_info_ptr->is_softkey_gray[0] != is_left_gray
            || win_info_ptr->is_softkey_gray[1] != is_middle_gray 
            || win_info_ptr->is_softkey_gray[2] != is_right_gray )
        {
            win_info_ptr->is_softkey_gray[0] = is_left_gray;
            win_info_ptr->is_softkey_gray[1] = is_middle_gray;
            win_info_ptr->is_softkey_gray[2] = is_right_gray;

            if(is_fresh && MMK_IsFocusWin(win_id))
            {
                MMIPUBSetButton( win_info_ptr, TRUE );
            }
            else
            {
                MMIPUBSetButton( win_info_ptr, FALSE );
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIPUB_SetWinSoftkeyGray:win_id %d is failed!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_3794_112_2_18_1_58_47_59,(uint8*)"d",win_id);
    }
}

/*****************************************************************************/
//  Description : get pubwin window softkey gray
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_GetWinSoftkeyGray(
                                     MMI_WIN_ID_T    win_id,
                                     BOOLEAN         *is_left_gray_ptr,
                                     BOOLEAN         *is_middle_gray_ptr,
                                     BOOLEAN         *is_right_gray_ptr
                                     )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        if (PNULL != is_left_gray_ptr)
        {
            *is_left_gray_ptr = win_info_ptr->is_softkey_gray[0];
        }

        if (PNULL != is_middle_gray_ptr)
        {
            *is_middle_gray_ptr = win_info_ptr->is_softkey_gray[1];
        }

        if (PNULL != is_right_gray_ptr)
        {
            *is_right_gray_ptr = win_info_ptr->is_softkey_gray[2];
        }
    }
}

/*****************************************************************************/
//  Description : set pubwin window softkey hanlde long
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetWinSoftkeyHandleLong(
                                           MMI_WIN_ID_T     win_id,
                                           uint16           button_index,
                                           BOOLEAN          is_handle_long
                                           )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr && button_index < GUISOFTKEY_BUTTON_NUM)
    {
        win_info_ptr->is_handle_long[button_index] = is_handle_long;
    }
}

#endif

/*****************************************************************************/
//  Description : get pubwin layer
//  Global resource dependence : 
//  Author: andrew
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUB_GetPubWinLayer(MMI_WIN_ID_T win_id, GUI_LCD_DEV_INFO* lcd_dev_info)
{
    MMIPUB_INFO_T *win_info_ptr = (MMIPUB_INFO_T *)MMK_GetWinAddDataEx(win_id);

    *lcd_dev_info = GetPubWinLayer(win_info_ptr);
}

/*****************************************************************************/
//  Description : Set Pub Win type as MMIPUBWIN_LIST
//  Global resource dependence : 
//  Author: sam.Hua
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUB_SetPubListWinType(MMIPUB_INFO_T* 	win_info_ptr)
{
	if (PNULL == win_info_ptr)
		return MMI_RESULT_FALSE;
	win_info_ptr->type = MMIPUBWIN_LIST;

	return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : get alert window timer id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIPUB_GetAlertTimerId(
                                    MMI_WIN_ID_T      win_id
                                    )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL == win_info_ptr)
    {
        return 0;
    }

    return (win_info_ptr->timer_id);
}

/*****************************************************************************/
//    Description : 显示提示信息字符串
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_DisplayTipsPtr(MMI_WIN_ID_T win_id, MMI_STRING_T* Text_Ptr)
{

    GUISTR_STATE_T   text_state   = 0;
    GUISTR_STYLE_T   text_style   = {0};
    GUISTR_INFO_T    text_info    = {0};
    GUI_RECT_T       text_rect    = {0};
    GUI_RECT_T       img_rect     = {0};
    GUI_POINT_T      start_point  = {0};
    uint16           img_width    = 0;
    uint16           img_height   = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMI_IMAGE_ID_T   bg_img_id    = 0;
    
    
    if (PNULL == Text_Ptr || !s_tips_dis_running)
    {
        return;
    }


    if (Text_Ptr->wstr_len > 0)
    {
        uint16 rect_width  = 0;
        uint16 rect_height = 0;
        
        if (CreatPromptTextLayer(win_id))
        {
            lcd_dev_info = s_pub_tips_layer_info;

            MMITHEME_GetTipsStrInfo(&text_rect , &text_style, &text_state, &bg_img_id);
            
            GUISTR_GetStringInfo(&text_style, Text_Ptr, text_state, &text_info);

            rect_width = text_rect.right - text_rect.left;
            rect_height = text_rect.bottom - text_rect.top;
            
            if (text_info.width > rect_width)
            {
                text_info.width = rect_width;
            }
            
            text_rect.bottom = text_rect.top + text_info.height;
            text_rect.left   = (rect_width - text_info.width) / 2;
            text_rect.right  = text_rect.left + text_info.width;
            
            (void)GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_img_id, win_id);
            
            start_point.x   = text_rect.left - MMIPUBWIN_TIPS_BG_SIDE_LEN;
            start_point.y    = text_rect.top - (img_height - text_info.height) / 2;
            img_rect.bottom = img_height;
            img_rect.right  = MMIPUBWIN_TIPS_BG_SIDE_LEN - 1;
            GUIRES_DisplayImg(&start_point, PNULL, &img_rect, win_id, bg_img_id, &lcd_dev_info);
            
            start_point.x = text_rect.left;
            img_rect.left   = img_rect.right;
            img_rect.right  = img_rect.left + text_info.width;
            GUIRES_DisplayImg(&start_point, PNULL, &img_rect, win_id, bg_img_id, &lcd_dev_info);
            start_point.x = text_rect.right + 1;
            img_rect.left   = img_width - MMIPUBWIN_TIPS_BG_SIDE_LEN + 1;
            img_rect.right  = img_width;
            GUIRES_DisplayImg(&start_point, PNULL, &img_rect, win_id, bg_img_id, &lcd_dev_info);
            
            GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, Text_Ptr,	&text_style, text_state, 0);
#ifdef UI_MULTILAYER_SUPPORT            
            {
                UILAYER_APPEND_BLT_T append_layer = {0};
                //add into layer queue
                append_layer.lcd_dev_info = lcd_dev_info;
                append_layer.layer_level = UILAYER_LEVEL_NORMAL;
                
                UILAYER_AppendBltLayer(&append_layer);
            }
#endif
            
        }
    }
}

/*****************************************************************************/
//    Description : 显示提示信息字符串
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_DisplayTipsID(MMI_WIN_ID_T win_id, MMI_TEXT_ID_T text_id)
{
    MMI_STRING_T Text = {0};
    MMI_GetLabelTextByLang(text_id, &Text);

    MMIPUB_DisplayTipsPtr(win_id, &Text);

}

/*****************************************************************************/
//    Description : 显示提示信息字符串 By Text ID
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_DisplayTipsIDEx(
    MMI_WIN_ID_T    *win_id_ptr,        //may NULL
    MMI_TEXT_ID_T   text_id, 
    MMIPUB_TIPS_DIS_STYLE_E dis_style,
    uint32          *time_out_ptr
    )
{
    MMI_STRING_T Text = {0};

    MMI_GetLabelTextByLang(text_id, &Text);

    MMIPUB_DisplayTipsPtrEx(win_id_ptr, &Text, dis_style, time_out_ptr);
}

/*****************************************************************************/
//    Description : 显示提示信息字符串 By Text ptr
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_DisplayTipsPtrEx(
    MMI_WIN_ID_T    *win_id_ptr,        //may NULL
    MMI_STRING_T    *Text_Ptr, 
    MMIPUB_TIPS_DIS_STYLE_E dis_style, 
    uint32          *time_out_ptr
    )
{
    uint16       len         = 0;

    if (PNULL == Text_Ptr)
    {
        return;
    }

    MMIPUB_ClearTips();

    if (OpenTipsDisWin(win_id_ptr))
    {
        if (PNULL != time_out_ptr)
        {
            s_time_out = *time_out_ptr;
        }
        else
        {
            s_time_out = MMI_1SECONDS; //tip默认1秒时间
        }

        len = (Text_Ptr->wstr_len + 1) * sizeof(wchar);
        s_dis_text.wstr_ptr = SCI_ALLOCA(len);
        if (PNULL == s_dis_text.wstr_ptr)
        {
            MMK_CloseWin(MMIPUB_TIPS_WIN);
            return;
        }
        SCI_MEMSET(s_dis_text.wstr_ptr, 0x00, len);
        MMIAPICOM_Wstrncpy(s_dis_text.wstr_ptr, Text_Ptr->wstr_ptr, Text_Ptr->wstr_len);

        s_dis_text.wstr_len = MMIAPICOM_Wstrlen(s_dis_text.wstr_ptr);
        s_cur_tips_dis_style = dis_style;
    }
}

/*****************************************************************************/
//    Description : 清除提示信息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_ClearTips(void)
{
    if (PNULL != s_dis_text.wstr_ptr)
    {
        SCI_FREE(s_dis_text.wstr_ptr);
        s_dis_text.wstr_ptr = PNULL;
    }

    s_tips_clear_timer_id         = NULL;
    s_cur_tips_dis_style          = MMIPUB_TIPS_DIS_MAX;
    s_time_out                    = 0xff;

    SCI_MEMSET(&s_dis_text, 0x00, sizeof(s_dis_text));
    MMIPUB_StopDisTipsTimer();

    if (MMK_IsOpenWin(MMIPUB_TIPS_WIN))
    {
        MMK_CloseWin(MMIPUB_TIPS_WIN);
    }
#ifdef TOUCH_PANEL_SUPPORT     
    //清除tp状态
    MMK_SetTPDownWin(0);
#endif
}

/*****************************************************************************/
//    Description : 设置是否显示的标志位
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_SetDisTipsFlag(BOOLEAN is_dis)
{
    s_tips_dis_running = is_dis;
}

/*****************************************************************************/
//    Description : 获取是否显示的标志位
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPUB_GetDisTipsFlag(void)
{
    return s_tips_dis_running;
}

/*****************************************************************************/
//    Description : 开始显示Tips
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_StartDisTips(MMI_WIN_ID_T win_id, uint32 *time_out_ptr)
{
    uint32 time_out = 0;

    MMIPUB_SetDisTipsFlag(TRUE);

    if (PNULL == time_out_ptr)
    {
        time_out = MMI_2SECONDS;
    }
    else
    {
        time_out = *time_out_ptr;
    }

    if (time_out > 0)
    {
        MMIPUB_StopDisTipsTimer();
        s_tips_clear_timer_id = MMK_CreateTimerCallback(time_out, HandleClearTipsTimerEvent, (uint32)win_id, FALSE);
    }
}

/*****************************************************************************/
//    Description : 停止Tips显示定时器
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIPUB_StopDisTipsTimer(void)
{
    if (NULL != s_tips_clear_timer_id)
    {
        MMK_StopTimer(s_tips_clear_timer_id);
        s_tips_clear_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:处理关闭窗口消息
/*****************************************************************************/
PUBLIC void MMIPUB_DestroyWinData(
                                  MMIPUB_INFO_T* win_info_ptr
                                  )
{
    MMI_STRING_T *string_array[MMIPUB_TEXT_MAX] = {0};
    
    SetPubWinText( win_info_ptr, string_array, MMIPUB_TEXT_MAX, TRUE );
    SetPubWinTitleText( win_info_ptr, PNULL );
    MMIPUB_ReleaseLayer(win_info_ptr);
    ResetPubwinInfo( win_info_ptr );
    MMK_FreeWinAddDataEx( win_info_ptr->win_handle );
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:处理关闭窗口消息
/*****************************************************************************/
PUBLIC void MMIPUB_HandleLcdSwitch(
                                   MMIPUB_INFO_T* win_info_ptr
                                   )
{
    win_info_ptr->is_layout = FALSE;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:release layer
/*****************************************************************************/
PUBLIC void MMIPUB_ReleaseLayer(
                                MMIPUB_INFO_T* win_info_ptr
                                )
{
    ReleasePubWinLayer( &win_info_ptr->pubwin_bg_dev );
    ReleasePubWinLayer( &win_info_ptr->pubwin_mask_dev );
}

/*****************************************************************************/
//  Description : creat and open alert window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenAlertWin(
                        MMI_HANDLE_T                applet_handle,
                        uint32                      *time_period_ptr,   //time period,default is 2s
                        MMI_STRING_T                *text1_str_ptr,     //text 1
                        MMI_STRING_T                *text2_str_ptr,     //text 2
                        MMI_IMAGE_ID_T              image_id,           //alert image id
                        MMI_WIN_ID_T                *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                        MMI_WIN_PRIORITY_E          *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                        MMIPUB_SOFTKEY_STYLE_E      softkey_style,      //softkey style
                        MMIPUB_HANDLE_FUNC          handle_func,        //handle function
                        uint32                      user_data           //user data
                        )
{
    BOOLEAN             result = FALSE;
    MMI_WIN_ID_T        win_id = MMIPUB_ALERT_WIN_ID;
    MMI_HANDLE_T        win_handle = 0;
    MMIPUB_INFO_T       *win_info_ptr = PNULL;
    MMI_WIN_PRIORITY_E  win_priority = WIN_ONE_LEVEL;
    PROCESSMSG_FUNC     handle_function = (PROCESSMSG_FUNC)MMIPUB_HandleAlertWinMsg;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_ANIM_ID_T       anim_id = 0;
    uint32              type = MMIPUBWIN_ALERT;
    
    //set window id
    if (PNULL != win_id_ptr)
    {
        win_id = *win_id_ptr;
    }
    if (MMIAPIDM_GetHandsetLock())//dm lock 状态下不能有其他提示
    {
        return;
    }
    if (MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id )))
    {
        MMK_CloseWin(MMK_GetWinHandle(applet_handle, win_id ));
    }
    //judge window is already open
    if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
    {
        //set window priority
        if (PNULL != win_priority_ptr)
        {
            win_priority = *win_priority_ptr;
        }

        //set call back function
        if (PNULL != handle_func)
        {
            handle_function = (PROCESSMSG_FUNC)handle_func;
        }

        //set window parameter
        win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_INFO_T));
        SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_INFO_T));

        //set user data
        win_info_ptr->user_data       = user_data;
        
        //set time period
        if (PNULL != time_period_ptr)
        {
            win_info_ptr->time_period = *time_period_ptr;
        }
		else 
		{
            win_info_ptr->time_period = 0;
		}

        //统一将IMAGE_PUBWIN_WAIT替换为动画
        if ( IMAGE_PUBWIN_WAIT == image_id )
        {
            image_id = 0;
            win_info_ptr->wait_anim_id = anim_id = ANIM_PUBWIN_WAIT;

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            type = MMIPUBWIN_WAITING;
#endif
        }

        //set prompt image id
#ifdef DPHONE_SUPPORT 
        win_info_ptr->prompt_image_id = IMAGE_NULL;
#else        
//        win_info_ptr->prompt_image_id = image_id;
#endif//syy modify 2010.11.30 -end

        //set softkey style
        win_info_ptr->softkey_style = softkey_style;
        win_info_ptr->win_id = win_id;
        win_info_ptr->type = type;

        //get pubwin theme
        MMITHEME_GetPubTheme( win_info_ptr->type, &win_info_ptr->pubwin_theme );

        win_create.applet_handle = applet_handle;
        win_create.win_id = win_id;
        win_create.func = HandleDefaultWinMsg;
        win_info_ptr->user_func = handle_function;
        win_create.win_priority = win_priority;
        win_create.add_data_ex_ptr = win_info_ptr;
        win_create.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;

        //禁止硬图标响应
        if( win_priority >= WIN_ONE_LEVEL )
        {
            win_create.window_style |= WS_DISABLE_HWICON;
        }

        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
        
        if( 0 != ( win_handle = win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }
        
		if (result)
		{
            MMI_STRING_T    *string_array[MMIPUB_TEXT_MAX] = {0};
            
            string_array[0] = text1_str_ptr;
            string_array[1] = text2_str_ptr;
            
            SetPubWinText( win_info_ptr, string_array, 2, FALSE );
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
     //       SetPubWinDefaultTitle( win_info_ptr );
#endif
            SetNeedPaintPreWin( win_info_ptr, image_id );

            MMIPUBCreatSoftkeyCtrl( win_handle );

            CreatAnimLabelCtrl( win_handle, MMIPUB_WAIT_ANIM_CTRL_ID, anim_id );
        }

        if (!result)
        {
            if (!MMK_IsOpenWin( win_handle ))
            {
                SCI_FREE(win_info_ptr);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenAlertWin:win_id %d is already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_2101_112_2_18_1_58_43_53,(uint8*)"d", win_id);
    }
}

/*****************************************************************************/
//  Description : creat and open alert window
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
LOCAL void OpenAlertTextWin(
                            MMI_HANDLE_T                applet_handle,
                            uint32                      *time_period_ptr,   //time period,default is 2s
                            MMI_STRING_T                *text_str_ptr,     //text 1
                            MMI_WIN_ID_T                *win_id_ptr,        //win id,default is MMIPUB_ALERT_TEXT_WIN_ID
                            MMI_WIN_PRIORITY_E          *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                            MMIPUB_SOFTKEY_STYLE_E      softkey_style,      //softkey style
                            MMIPUB_HANDLE_FUNC          handle_func,        //handle function
                            uint32                      user_data           //user data
                            )
{
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    MMI_WIN_ID_T win_id = MMIPUB_ALERT_TEXT_WIN_ID;
    TRACE_APP_OLD_PUPWIN("enter");
        //set window id
    if (PNULL != win_id_ptr)
    {
        win_id = *win_id_ptr;
    }

    OpenAlertWin( applet_handle, time_period_ptr, text_str_ptr, PNULL, 0, &win_id, win_priority_ptr, softkey_style, handle_func, user_data );
#else
    BOOLEAN             result = FALSE;
    MMI_WIN_ID_T        win_id = MMIPUB_ALERT_TEXT_WIN_ID;
    MMI_HANDLE_T        win_handle = 0;
    MMIPUB_INFO_T       *win_info_ptr = PNULL;
    MMI_WIN_PRIORITY_E  win_priority = WIN_ONE_LEVEL;
    PROCESSMSG_FUNC     handle_function = (PROCESSMSG_FUNC)MMIPUB_HandleAlertWinMsg;
    MMI_WINDOW_CREATE_T win_create = {0};
    
    //set window id
    if (PNULL != win_id_ptr)
    {
        win_id = *win_id_ptr;
    }

    //judge window is already open
    if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
    {
        //set window priority
        if (PNULL != win_priority_ptr)
        {
            win_priority = *win_priority_ptr;
        }

        //set call back function
        if (PNULL != handle_func)
        {
            handle_function = (PROCESSMSG_FUNC)handle_func;
        }

        //set window parameter
        win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_INFO_T));
        SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_INFO_T));

        //set user data
        win_info_ptr->user_data       = user_data;
        
        //set time period
        if (PNULL == time_period_ptr)
        {
            win_info_ptr->time_period = MMI_2SECONDS;
        }
        else
        {
            win_info_ptr->time_period = *time_period_ptr;
        }
        //set softkey style
        win_info_ptr->softkey_style = softkey_style;
        win_info_ptr->win_id = win_id;
        win_info_ptr->type = MMIPUBWIN_ALERT_TEXT;
        //get pubwin theme
        MMITHEME_GetPubTheme( win_info_ptr->type, &win_info_ptr->pubwin_theme );

        win_create.applet_handle = applet_handle;
        win_create.win_id = win_id;
        win_create.func = HandleDefaultWinMsg;
        win_info_ptr->user_func = handle_function;
        win_create.win_priority = win_priority;
        win_create.add_data_ex_ptr = win_info_ptr;
        win_create.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;

        //禁止硬图标响应
        if( win_priority >= WIN_ONE_LEVEL )
        {
            win_create.window_style |= WS_DISABLE_HWICON;
        }

        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
        
        if( 0 != ( win_handle = win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }

        if (result)
        {
            if ((NULL != text_str_ptr) && (0 < text_str_ptr->wstr_len))
            {
                //创建text控件
                CreatTextBoxCtrl( win_handle, MMIPUB_TEXT_CTRL_ID, text_str_ptr );
            }            
        }
        
        if (result)
        {
            MMIPUBCreatSoftkeyCtrl( win_handle );
        }

        if (!result)
        {
            if (!MMK_IsOpenWin( win_handle ))
            {
                SCI_FREE(win_info_ptr);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenAlertTextWin:win_id %d is already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_5081_112_2_18_1_58_50_65,(uint8*)"d", win_id);
    }
#endif
}

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//                当text1有值, text2值为空时, 如果text1文本很长, 自动换行显示,
//                最多显示3行
//  Global resource dependence : 
//  Author: liming.deng
//  Note:
/*****************************************************************************/
LOCAL void OpenQueryWin(
                        MMI_HANDLE_T               applet_handle,
                        MMI_STRING_T               *text1_ptr,          //text 1,no default TXT_NULL
                        MMI_STRING_T               *text2_ptr,          //text 1,no default TXT_NULL
                        MMI_IMAGE_ID_T             image_id,           //alert image id
                        MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                        MMIPUB_HANDLE_FUNC         handle_func,         //handle function
                        uint32                     user_data
                        )
{
    BOOLEAN             result = FALSE;
    MMI_WIN_ID_T        win_id = MMIPUB_QUERY_WIN_ID;
    MMIPUB_INFO_T       *win_info_ptr = PNULL;
    PROCESSMSG_FUNC     handle_function = (PROCESSMSG_FUNC)MMIPUB_HandleQueryWinMsg;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_HANDLE_T        win_handle = 0;
    
    //set window id
    if (PNULL != win_id_ptr)
    {
        win_id = *win_id_ptr;
    }

    //judge window is already open
    if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
    {
        //set call back function
        if (PNULL != handle_func)
        {
            handle_function = (PROCESSMSG_FUNC)handle_func;
        }

        //set window parameter
        win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_INFO_T));
        SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_INFO_T));

        //set user data
        win_info_ptr->user_data       = user_data;
        //set prompt image id
#ifdef DPHONE_SUPPORT 
        win_info_ptr->prompt_image_id = IMAGE_NULL;
#else        
        win_info_ptr->prompt_image_id = image_id;
#endif//syy modify 2010.11.30 -end        

        //set softkey style
        win_info_ptr->softkey_style = MMIPUB_SOFTKEY_OKCANCEL;
        win_info_ptr->win_id = win_id;
        win_info_ptr->type = MMIPUBWIN_QUERY;
        //get pubwin theme
        MMITHEME_GetPubTheme( win_info_ptr->type, &win_info_ptr->pubwin_theme );
        
        win_create.applet_handle = applet_handle;
        win_create.win_id = win_id;
        win_create.func = HandleDefaultWinMsg;
        win_info_ptr->user_func = handle_function;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ex_ptr = win_info_ptr;
        win_create.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;
        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
        
        if( 0 != ( win_handle = win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }

		if (result)
		{
            MMI_STRING_T    *string_array[MMIPUB_TEXT_MAX] = {0};
            
            string_array[0] = text1_ptr;
            string_array[1] = text2_ptr;
            
            SetPubWinText( win_info_ptr, string_array, 2, FALSE );
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            //SetPubWinDefaultTitle( win_info_ptr );
#endif
            MMIPUBCreatSoftkeyCtrl( win_handle );
        }
        
        if (!result)
        {
            if (!MMK_IsOpenWin( win_handle ))
            {
                SCI_FREE(win_info_ptr);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenQueryWin:win_id %d is already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_4062_112_2_18_1_58_47_60,(uint8*)"d", win_id);
    }
}

/*****************************************************************************/
//  Description : creat and open query window by text ptr
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
LOCAL void OpenQueryTextWin(
                            MMI_HANDLE_T               applet_handle,
                            MMI_STRING_T               *text_ptr,          //text,no default TXT_NULL
                            MMI_WIN_ID_T               *win_id_ptr,        //win id,default is MMIPUB_QUERY_TEXT_WIN_ID
                            MMIPUB_HANDLE_FUNC         handle_func,         //handle function
                            uint32                     user_data
                            )
{
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    MMI_WIN_ID_T win_id = MMIPUB_QUERY_TEXT_WIN_ID;
    if (PNULL != win_id_ptr)
    {
        win_id = *win_id_ptr;
    }
    
    TRACE_APP_OLD_PUPWIN("enter");
    OpenQueryWin( applet_handle, text_ptr, PNULL, 0, &win_id, handle_func, user_data );
#else
    BOOLEAN             result = FALSE;
    MMI_WIN_ID_T        win_id = MMIPUB_QUERY_TEXT_WIN_ID;
    MMIPUB_INFO_T       *win_info_ptr = PNULL;
    PROCESSMSG_FUNC     handle_function = (PROCESSMSG_FUNC)MMIPUB_HandleQueryWinMsg;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_HANDLE_T        win_handle = 0;
    
    TRACE_APP_OLD_PUPWIN("enter");
    //set window id
    if (PNULL != win_id_ptr)
    {
        win_id = *win_id_ptr;
    }

    //judge window is already open
    if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
    {
        //set call back function
        if (PNULL != handle_func)
        {
            handle_function = (PROCESSMSG_FUNC)handle_func;
        }

        //set window parameter
        win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_INFO_T));
        SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_INFO_T));

        //set user data
        win_info_ptr->user_data       = user_data;

        //set softkey style
        win_info_ptr->softkey_style = MMIPUB_SOFTKEY_OKCANCEL;
        win_info_ptr->win_id = win_id;
        win_info_ptr->type = MMIPUBWIN_QUERY_TEXT;
        //get pubwin theme
        MMITHEME_GetPubTheme( win_info_ptr->type, &win_info_ptr->pubwin_theme );
        
        win_create.applet_handle = applet_handle;
        win_create.win_id = win_id;
        win_create.func = HandleDefaultWinMsg;
        win_info_ptr->user_func = handle_function;
        win_create.win_priority = WIN_ONE_LEVEL;
        win_create.add_data_ex_ptr = win_info_ptr;
        win_create.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;
        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
        
        if( 0 != ( win_handle = win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }

        if ((result) && (PNULL != text_ptr) && (0 < text_ptr->wstr_len))
        {
            //创建text控件
            CreatTextBoxCtrl( win_handle, MMIPUB_TEXT_CTRL_ID, text_ptr );
        }
        
        if (result)
        {
            MMIPUBCreatSoftkeyCtrl(win_handle);
        }

        if (!result)
        {
            if (!MMK_IsOpenWin( win_handle ))
            {
                SCI_FREE(win_info_ptr);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenQueryTextWin:win_id %d is already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_5285_112_2_18_1_58_50_66,(uint8*)"d", win_id);
    }
#endif
}

/*****************************************************************************/
//  Description : creat and open wait window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void OpenWaitWin(
                       uint32                  type,
                       MMI_HANDLE_T            applet_handle,
                       uint8                   text_num,       //text number
                       MMI_STRING_T            *text1_ptr,     //text 1
                       MMI_STRING_T            *text2_ptr,     //text 2
                       MMI_STRING_T            *text3_ptr,     //text 3
                       MMI_WIN_ID_T            win_id,         //win id
                       MMI_IMAGE_ID_T          image_id,       //image id
                       MMI_ANIM_ID_T           anim_id,        //anim id
                       MMI_WIN_PRIORITY_E      win_priority,   //priority
                       MMIPUB_SOFTKEY_STYLE_E  softkey_style,  //softkey style
                       MMIPUB_HANDLE_FUNC      handle_func,     //handle function
                       uint32                  user_data
                       )
{
    BOOLEAN             result = FALSE;
    MMIPUB_INFO_T       *win_info_ptr = PNULL;
    PROCESSMSG_FUNC     handle_function = (PROCESSMSG_FUNC)MMIPUB_HandleWaitWinMsg;
    MMI_WINDOW_CREATE_T win_create = {0};
    MMI_HANDLE_T win_handle = 0;
    
    if (3 < text_num)
    {
        return;
    }

    if (!( MMIPUBWIN_WAIT == type || MMIPUBWIN_WAITING == type ))
    {
        return;
    }

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    if ( IMAGE_PUBWIN_WAIT == image_id )
    {
        image_id = 0;
        anim_id = ANIM_PUBWIN_WAIT;
    }

    if ( ANIM_PUBWIN_WAIT == anim_id )
    {
        type = MMIPUBWIN_WAITING;
    }
#endif
    
    if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
    {
        //set call back function
        if (PNULL != handle_func)
        {
            handle_function = (PROCESSMSG_FUNC)handle_func;
        }
        
        //set window parameter
        win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_INFO_T));
        SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_INFO_T));
        
        win_create.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;
        
        //set user data
        win_info_ptr->user_data     = user_data;
        
        //set wait window text number
        //win_info_ptr->wait_text_num = text_num;
        
        //set image id
        win_info_ptr->prompt_image_id = image_id;
        
        //set wait anim id
        win_info_ptr->wait_anim_id = anim_id;
        
        //set softkey style
        win_info_ptr->softkey_style = softkey_style;
        win_info_ptr->win_id = win_id;
        win_info_ptr->type = type;

        // set waitwin is update delay
        win_info_ptr->is_waitwin_update_delay = TRUE;
        //get pubwin theme
        MMITHEME_GetPubTheme( win_info_ptr->type, &win_info_ptr->pubwin_theme );
        
        win_create.applet_handle = applet_handle;
        win_create.win_id = win_id;
        win_create.func = HandleDefaultWinMsg;
        win_info_ptr->user_func = handle_function;
        win_create.win_priority = win_priority;
        win_create.add_data_ex_ptr = win_info_ptr;
        
        //禁止硬图标响应
        if( win_priority >= WIN_ONE_LEVEL )
        {
            win_create.window_style |= WS_DISABLE_HWICON;
        }
        
        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
        
        if( 0 != ( win_handle = win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }
        
        if (result)
        {
            MMI_STRING_T    *string_array[MMIPUB_TEXT_MAX] = {0};
            
            string_array[0] = text1_ptr;
            string_array[1] = text2_ptr;
            string_array[2] = text3_ptr;
            
            SetPubWinText( win_info_ptr, string_array, text_num, FALSE );

            MMIPUBCreatSoftkeyCtrl( win_handle );

            CreatAnimLabelCtrl( win_handle, MMIPUB_WAIT_ANIM_CTRL_ID, anim_id );
        }
        
        if (!result)
        {
            if (!MMK_IsOpenWin( win_handle ))
            {
                SCI_FREE(win_info_ptr);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"OpenWaitWin:win_id %d is already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_4313_112_2_18_1_58_48_62,(uint8*)"d", win_id);
    }
}

//note, ishell interface must use handle!!!
/*****************************************************************************/
//  Description : creat and open progress window
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void OpenProgressWin(
                        MMI_HANDLE_T                applet_handle,
                        MMI_STRING_T                *text1_str_ptr,     //text 1
                        MMI_WIN_ID_T                *win_id_ptr,        //win id,default is MMIPUB_ALERT_WIN_ID
                        MMI_WIN_PRIORITY_E          *win_priority_ptr,  //win priority,default is WIN_ONE_LEVEL
                        MMIPUB_SOFTKEY_STYLE_E      softkey_style,      //softkey style
                        MMIPUB_HANDLE_FUNC          handle_func         //handle function
                        )
{
    BOOLEAN             result = FALSE;
    MMI_WIN_ID_T        win_id = MMIPUB_PROGRESS_WIN_ID;
    MMI_HANDLE_T        win_handle = 0;
    MMIPUB_INFO_T       *win_info_ptr = PNULL;
    MMI_WIN_PRIORITY_E  win_priority = WIN_ONE_LEVEL;
    PROCESSMSG_FUNC     handle_function = (PROCESSMSG_FUNC)MMIPUB_HandleProgressWinMsg;
    MMI_WINDOW_CREATE_T win_create = {0};
    
    TRACE_APP_OLD_PUPWIN("enter");
    //set window id
    if (PNULL != win_id_ptr)
    {
        win_id = *win_id_ptr;
    }

    //judge window is already open
    if ( !MMK_IsOpenWin( MMK_GetWinHandle( applet_handle, win_id ) ) )
    {
        //set window priority
        if (PNULL != win_priority_ptr)
        {
            win_priority = *win_priority_ptr;
        }

        //set call back function
        if (PNULL != handle_func)
        {
            handle_function = (PROCESSMSG_FUNC)handle_func;
        }

        //set window parameter
        win_info_ptr = SCI_ALLOC_APP(sizeof(MMIPUB_INFO_T));
        SCI_MEMSET(win_info_ptr,0,sizeof(MMIPUB_INFO_T));

        //set softkey style
        win_info_ptr->softkey_style = softkey_style;
        win_info_ptr->win_id = win_id;
        win_info_ptr->type = MMIPUBWIN_PROGRESS;
        //////////////////////////////////////////////////////////////////////////
        // fixed by feng.xiao for NEWMS00180461
//         win_info_ptr->prompt_image_id = IMAGE_COMMON_PROCESS_GROOVE_1;
#ifndef MMI_RES_LOW_COST 
        win_info_ptr->prg_img_info.m_bg_img_id = IMAGE_CONTROL_PROGRESS_BG;
        win_info_ptr->prg_img_info.m_ft_img_id = IMAGE_CONTROL_PROGRESS;
#endif
        //get pubwin theme
        MMITHEME_GetPubTheme( win_info_ptr->type, &win_info_ptr->pubwin_theme );

        win_create.applet_handle = applet_handle;
        win_create.win_id = win_id;
        win_create.func = HandleDefaultWinMsg;
        win_info_ptr->user_func = handle_function;
        win_create.win_priority = win_priority;
        win_create.add_data_ex_ptr = win_info_ptr;
        win_create.window_style = WS_HAS_TRANSPARENT|WS_HAS_PROMPT_WIN;

        //禁止硬图标响应
        if( win_priority >= WIN_ONE_LEVEL )
        {
            win_create.window_style |= WS_DISABLE_HWICON;
        }

        win_create.window_move_style = MOVE_NOT_FULL_SCREEN_WINDOW;
        
        if( 0 != ( win_handle = win_info_ptr->win_handle = MMK_CreateWindow( &win_create ) ) )
        {
            result = TRUE;
        }

		if (result)
		{
            MMI_STRING_T    *string_array[MMIPUB_TEXT_MAX] = {0};
            
            string_array[0] = text1_str_ptr;
            
            SetPubWinText( win_info_ptr, string_array, 1, FALSE );
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
            SetPubWinDefaultTitle( win_info_ptr );
#endif
			MMIPUBCreatSoftkeyCtrl( win_handle );
		}
        
        if (!result)
        {
            if (!MMK_IsOpenWin( win_handle ))
            {
                SCI_FREE(win_info_ptr);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIPUB_OpenAlertWinByTextId:win_id %d is already open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_2424_112_2_18_1_58_44_54,(uint8*)"d", win_id);
    }
}

/*****************************************************************************/
//    Description : 打开显示Tips窗口
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN OpenTipsDisWin(MMI_WIN_ID_T *win_id_ptr)
{
    MMI_HANDLE_T        win_handle  = 0;
    MMI_WINDOW_CREATE_T win_create  = {0};
    
    TRACE_APP_OLD_PUPWIN("enter");
    win_create.applet_handle = MMK_GetFirstAppletHandle();
    win_create.win_id        = MMIPUB_TIPS_WIN;
    win_create.func          = (PROCESSMSG_FUNC)HandleTipsWinMsg;;
    win_create.win_priority  = WIN_ONE_LEVEL;
    win_create.window_move_style  = MOVE_FORBIDDEN;
    win_create.window_style  = WS_HAS_TRANSPARENT | WS_TOOL_WIN;
    if(PNULL != win_id_ptr)
    {
        win_create.add_data_ptr  = (ADD_DATA)(*win_id_ptr);
    }
    
    if(0 != (win_handle = MMK_CreateWindow(&win_create)))
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : get pubwin bg rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: for MMIPUBWIN_ALERT MMIPUBWIN_QUERY MMIPUBWIN_WAIT
/*****************************************************************************/
PUBLIC void MMIPUBLayout(
                         MMIPUB_INFO_T *win_info_ptr
                         )
{
    GUI_RECT_T bg_rect      = {0};
    GUI_RECT_T alert_rect   = {0};
    GUI_RECT_T title_rect   = {0};
    GUI_RECT_T client_rect  = {0};
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
    GUI_RECT_T inter_bg_rect = {0}; 
#endif
    uint16     top_height   = 0;
    int16      v_margin     = 0;
    uint16     image_width  = 0;    // pubwin的宽度
    uint16     image_height = 0;    // pubwin的高度
    uint16     alert_width  = 0;
    uint16     alert_height = 0;
    uint16     total_height = 0;
    uint16     line_height = 0;
    uint16     im_height = 0;
    uint16     frame_num = 0;
    uint16     line_num = 0;
    uint16     max_line = 0;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    uint16     bottom_height= 0;
    uint16     list_normal_line_height = 0; // list正常状态下的行高
    uint16     list_client_height = 0;      // list客户区高度
    MMI_HANDLE_T im_handle = 0;
#endif

    if ( PNULL == win_info_ptr || win_info_ptr->is_layout )
    {
        return;
    }
	bg_rect = GetFullRect(win_info_ptr);

    // 获取alert图片的宽高
    if ( 0 == win_info_ptr->wait_anim_id )
    {
        //alert image
        GUIRES_GetImgWidthHeight(
                &alert_width, &alert_height,
                win_info_ptr->prompt_image_id,
                win_info_ptr->win_handle );
    }
    else
    {
        //alert anim
		GUIRES_GetAnimWidthHeight(
		        &alert_width, &alert_height, &frame_num,
		        win_info_ptr->wait_anim_id,
		        win_info_ptr->win_handle );
    }

    // 1. 获取整个PUBWIN客户区的位置
    if (MMIPUBWIN_WAITING == win_info_ptr->type)
    {
        uint16      waiting_bg_width = 0;
        uint16      waiting_bg_height = 0;

        GUIRES_GetImgWidthHeight(
                &waiting_bg_width, &waiting_bg_height,
                win_info_ptr->pubwin_theme.bg_id,
                win_info_ptr->win_handle );

        // waiting窗口没有title, button，所以只算里面文字的高度
        line_num = win_info_ptr->line_num;

        line_height = GUI_ConvertGUIFontToSize(win_info_ptr->pubwin_theme.small_font)
                + win_info_ptr->pubwin_theme.line_space;
        
        max_line = (
                        bg_rect.bottom - bg_rect.top + 1
                        - win_info_ptr->pubwin_theme.layout.client_v_margin * 2
                    ) / line_height;
        
        //不能超过屏幕的高度
        if ( line_num > max_line )
        {
            line_num = max_line;
        }
   
        total_height = line_num * line_height;

        //高度需要>=动画的高度
        image_height = MAX( total_height, alert_height );
        image_height = MAX( image_height, waiting_bg_height );
        image_height += win_info_ptr->pubwin_theme.layout.client_v_margin * 2;
        image_width = bg_rect.right - bg_rect.left + 1;
    }
    else
    {
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        // 1) PUBWIN顶上title的宽高
        GUIRES_GetImgWidthHeight(
                &image_width, &image_height,
                win_info_ptr->pubwin_theme.layout.top_bg_id,
                win_info_ptr->win_handle);

        top_height = image_height;
        total_height += image_height;

        // 2) PUBWIN底下button的宽高
        image_height = GetPubWinBottomBGHeight( win_info_ptr, IsExistSoftkey( win_info_ptr ) );
        bottom_height = image_height;
        total_height += image_height;

        // 3) 获取中间部分的宽高
        if ( MMIPUBWIN_FORM == win_info_ptr->type )
        {
            //特殊处理,设置输入法的lcd dev
            GUI_RECT_T   im_rect   = {0};
            
            im_handle = MMK_GetCtrlHandleByGuid( win_info_ptr->win_handle, SPRD_GUI_IM_ID );

            if ( GUIIM_GetImRect( im_handle, &im_rect ) )
            {
                im_height = im_rect.bottom - im_rect.top + 1;

#if defined(GUIF_EDIT_PDASTYLE) && defined(MMI_IM_PDA_SUPPORT)
                if ( MMK_IsWindowLandscape( win_info_ptr->win_handle ) )
                {
                    bottom_height = 0;
                }
#endif
            }

            max_line = (bg_rect.bottom - bg_rect.top + 1
                    - top_height - bottom_height - im_height
                    - win_info_ptr->pubwin_theme.layout.client_v_margin * 2);

            line_num = win_info_ptr->line_num;

            //不能超过屏幕的高度
            line_num = MIN( line_num, max_line );

            total_height += line_num + win_info_ptr->pubwin_theme.layout.client_v_margin * 2;
            
            image_width = bg_rect.right - bg_rect.left + 1;
            image_height = total_height;

            line_height = 1;
            //line_height = 2;
        }
        else
        {
			//prgwin需要为 0/0 和图片预留两行
			if (MMIPUBWIN_PROGRESS == win_info_ptr->type)
			{
                line_num = win_info_ptr->line_num + 2;
			}
            else
			{
 			    line_num = win_info_ptr->line_num;
			}

            if ( MMIPUBWIN_LIST == win_info_ptr->type  )
            {
                // list高亮与正常状态的行高有可能不一样，这里分开处理
                const THEMELIST_ITEM_STYLE_T* layout_ptr = THEMELIST_GetStyle(GUIITEM_LAYOUT_ONE_LINE_TEXT);

                // 以高亮时的行高来计算总数
                line_height = layout_ptr->height_focus;

                list_normal_line_height = layout_ptr->height;
            }
            else
            {
                line_height = GUI_ConvertGUIFontToSize(win_info_ptr->pubwin_theme.small_font)
                        + win_info_ptr->pubwin_theme.line_space;
            }
  
            max_line = (
                            bg_rect.bottom - bg_rect.top + 1 
                            - top_height - bottom_height
                            - win_info_ptr->pubwin_theme.layout.client_v_margin * 2
                        ) / line_height;
            
            //不能超过屏幕的高度
            if ( line_num > max_line )
            {
                line_num = max_line;
            }

            if (MMIPUBWIN_ALERT == win_info_ptr->type
                || MMIPUBWIN_QUERY == win_info_ptr->type
                || MMIPUBWIN_ALERT_TEXT == win_info_ptr->type
                || MMIPUBWIN_QUERY_TEXT == win_info_ptr->type)
            {
                GUIRES_GetImgWidthHeight(
                        &image_width, &image_height,
                        win_info_ptr->pubwin_theme.bg_id,
                        win_info_ptr->win_handle);

                if (line_num * line_height + win_info_ptr->pubwin_theme.layout.client_v_margin * 2 < image_height)
                {
                    total_height += image_height;
                }
                else
                {
                    total_height += line_num * line_height + win_info_ptr->pubwin_theme.layout.client_v_margin * 2;
                }
            }
            else if (MMIPUBWIN_LIST == win_info_ptr->type)
            {
                total_height += win_info_ptr->pubwin_theme.layout.client_v_margin * 2;

                if (line_num > 1)
                {
                    list_client_height = ((line_num - 1) * list_normal_line_height + line_height);
                }
                else
                {
                    list_client_height = line_num * line_height + win_info_ptr->pubwin_theme.line_space * 4;
                }

                total_height += list_client_height;
            }
            else
            {
                total_height += line_num * line_height + win_info_ptr->pubwin_theme.layout.client_v_margin * 2;
            }
            
            image_width = bg_rect.right - bg_rect.left + 1;
            image_height = total_height;
        }
#else   // not MMIPUBWIN_PDASTYLE_SUPPORT
        //prgwin需要为 0/0 预留一行
        if (MMIPUBWIN_PROGRESS == win_info_ptr->type)
        {
            line_num = win_info_ptr->line_num + 1;
        }
        else
        {
            line_num = win_info_ptr->line_num;
        }

        if ( line_num >= MMIPUB_MULTILINE_MAX )
        {
            line_height = GUI_ConvertGUIFontToSize(win_info_ptr->pubwin_theme.small_font)
                    + win_info_ptr->pubwin_theme.line_space;

            line_num = MMIPUB_MULTILINE_MAX;
        }
        else
        {
            line_height = GUI_ConvertGUIFontToSize(win_info_ptr->pubwin_theme.big_font)
                    + win_info_ptr->pubwin_theme.line_space;
        }

        GUIRES_GetImgWidthHeight(
                &image_width, &image_height,
                win_info_ptr->pubwin_theme.bg_id,
                win_info_ptr->win_handle );

        total_height = line_num * line_height + alert_height + win_info_ptr->pubwin_theme.layout.title_v_top_margin;

        if ( image_height < total_height )
        {
            image_height = total_height;

            if ( line_num < MMIPUB_MULTILINE_MAX )
            {
                image_height += line_height;
            }
        }
#endif
    }

    if ( 0 != image_width && 0 != image_height )
    {
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        //横屏下宽度还需要小一点
        if (MMIPUBWIN_WAITING != win_info_ptr->type
            && MMK_IsWindowLandscape( win_info_ptr->win_handle )
            && 0 == im_handle
            && image_width > win_info_ptr->pubwin_theme.layout.win_h_add_margin * 2 )
        {
            image_width -= win_info_ptr->pubwin_theme.layout.win_h_add_margin * 2;
        }
#endif

        bg_rect.bottom -= im_height;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        bg_rect = GUI_GetCenterRect( bg_rect, image_width, image_height );
#else
		inter_bg_rect = MMITHEME_GetClientRect();
		bg_rect.bottom = bg_rect.bottom -(bg_rect.bottom - inter_bg_rect.bottom);
		bg_rect = GUI_GetCenterRect( bg_rect, image_width, image_height );
#endif
    }

    win_info_ptr->bg_rect = bg_rect;

    // 2. 获取PUBWIN告示图标的位置
    alert_rect = bg_rect;

    if ( MMIPUBWIN_WAITING == win_info_ptr->type )
    {
        alert_rect.left += win_info_ptr->pubwin_theme.layout.client_h_margin;
        alert_rect.right = alert_rect.left + alert_width - 1;

        alert_rect = GUI_GetCenterRect( alert_rect, alert_width, alert_height );
    }
    else
    {
        v_margin = win_info_ptr->pubwin_theme.layout.title_v_top_margin;

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        if ( MMIPUBWIN_PROGRESS == win_info_ptr->type )
        {
            alert_rect.top = bg_rect.top + top_height + win_info_ptr->pubwin_theme.layout.client_v_margin;
            alert_rect.bottom = alert_rect.top + line_height - 1;
            //////////////////////////////////////////////////////////////////////////
            // fixed by feng.xiao for NEWMS00180461
            alert_rect.left = bg_rect.left + MMIPUB_PROGRESS_SPACE;
            alert_rect.right = bg_rect.right - MMIPUB_PROGRESS_SPACE;
            GUIRES_GetImgWidthHeight(&alert_width, &alert_height, 
                                     win_info_ptr->prg_img_info.m_bg_img_id, win_info_ptr->win_handle);
            alert_width = bg_rect.right - bg_rect.left - MMIPUB_PROGRESS_SPACE*2;
        }
        else
        {
            alert_rect.left += win_info_ptr->pubwin_theme.layout.title_start_margin;
            alert_rect.right = alert_rect.left + alert_width - 1;

            alert_rect.top = bg_rect.top + v_margin;
            alert_rect.bottom = bg_rect.top + top_height - win_info_ptr->pubwin_theme.layout.title_v_bottom_margin - 1;
        }
#else
        if ( MMIPUBWIN_PROGRESS == win_info_ptr->type )
        {
			GUIRES_GetImgWidthHeight(&alert_width, &alert_height, 
                win_info_ptr->prg_img_info.m_bg_img_id, win_info_ptr->win_handle);
            alert_rect.bottom = bg_rect.bottom - v_margin;
            alert_rect.top    = alert_rect.bottom - alert_height + 1;
            //////////////////////////////////////////////////////////////////////////
            // fixed by feng.xiao for NEWMS00180461
            alert_rect.left = bg_rect.left + MMIPUB_PROGRESS_SPACE;
            alert_rect.right = bg_rect.right - MMIPUB_PROGRESS_SPACE;

            alert_width = bg_rect.right - bg_rect.left - MMIPUB_PROGRESS_SPACE*2;
        }
        else
        {
            alert_rect.top    = bg_rect.top + v_margin;
            alert_rect.bottom = alert_rect.top + alert_height - 1;
        }
#endif

        alert_rect = GUI_GetCenterRect( alert_rect, alert_width, alert_height );
    }

    win_info_ptr->alert_rect = alert_rect;

    // 3. 获取PUBWIN title文字的位置
    if ( MMIPUBWIN_WAITING != win_info_ptr->type )
    {
        title_rect = bg_rect;

        title_rect.left += win_info_ptr->pubwin_theme.layout.title_start_margin;

        // 进度条PUBWIN没有提示图标，所以文本就从IMAGE的位置开始
        // 非进度条的有图标，从图标后显示
        if ( MMIPUBWIN_PROGRESS != win_info_ptr->type )
        {
            title_rect.left += alert_width + win_info_ptr->pubwin_theme.layout.title_image_text_space;
        }

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
#if defined(GUIF_EDIT_PDASTYLE) && defined(MMI_IM_PDA_SUPPORT)
        if (MMK_IsWindowLandscape(win_info_ptr->win_handle)
            && 0 != im_handle)
        {
            if ( PNULL != win_info_ptr->title_string.wstr_ptr )
            {
                title_rect.right -= image_width / 4;
                title_rect.top = MMI_STATUSBAR_HEIGHT;
                title_rect.bottom = MMI_STATUSBAR_HEIGHT + win_info_ptr->pubwin_theme.big_font;
            }
            else
            {
                title_rect.top = title_rect.bottom = MMI_STATUSBAR_HEIGHT;
            }
        }
        else
#endif
#endif
        {
            title_rect.right -= win_info_ptr->pubwin_theme.layout.title_start_margin;  // 右边的间距，与image的间距相同

            title_rect.top = bg_rect.top + v_margin;
            title_rect.bottom = bg_rect.top + top_height - 1;
        }
    }

    win_info_ptr->title_rect = title_rect;

    // 4. 获取PUBWIN中间客户区的位置
    client_rect = bg_rect;
    
    if ( MMIPUBWIN_WAITING == win_info_ptr->type )
    {
        client_rect.left   += win_info_ptr->pubwin_theme.layout.client_h_margin
                + alert_width + win_info_ptr->pubwin_theme.layout.title_image_text_space;

        client_rect.right  -= win_info_ptr->pubwin_theme.layout.client_h_margin;
        client_rect.top    += win_info_ptr->pubwin_theme.layout.client_v_margin;
        client_rect.bottom -= win_info_ptr->pubwin_theme.layout.client_v_margin;
    }
    else
    {
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
#if defined(GUIF_EDIT_PDASTYLE) && defined(MMI_IM_PDA_SUPPORT)
        if ( MMK_IsWindowLandscape( win_info_ptr->win_handle )
            && 0 != im_handle )
        {
            client_rect.right -= image_width / 4;
            client_rect.top = title_rect.bottom + 1;
        }
        else
#endif
        {
            if ( MMIPUBWIN_PROGRESS == win_info_ptr->type )
            {
                //prgwin需要为 0/0 和图片预留两行
                if ( line_num > 2 )
                {
                    client_rect.top += top_height + win_info_ptr->pubwin_theme.layout.client_v_margin + line_height;
                    client_rect.bottom = client_rect.top + ( line_num - 2 ) * line_height - 1;
                }
                else
                {
                    //no text rect
                }
            }
            else if (MMIPUBWIN_LIST == win_info_ptr->type)
            {
                client_rect.top += top_height + win_info_ptr->pubwin_theme.layout.client_v_margin;
                client_rect.bottom = client_rect.top + list_client_height - 1;
            }
			else
            {
                client_rect.top += top_height + win_info_ptr->pubwin_theme.layout.client_v_margin;
                client_rect.bottom = client_rect.top + line_num * line_height - 1;
			}

			client_rect.left += win_info_ptr->pubwin_theme.layout.client_h_margin;
			client_rect.right -= win_info_ptr->pubwin_theme.layout.client_h_margin;
        }
#else
        client_rect.top += win_info_ptr->pubwin_theme.layout.client_v_margin;
        client_rect.left += win_info_ptr->pubwin_theme.layout.client_h_margin;
        client_rect.right -= win_info_ptr->pubwin_theme.layout.client_h_margin;
        client_rect.bottom -= win_info_ptr->pubwin_theme.layout.client_v_margin;
#endif
    }

    win_info_ptr->client_rect = client_rect;

    win_info_ptr->is_layout = TRUE;
}

/*****************************************************************************/
//  Description : handle default window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:内部函数，处理默认的消息, 如close window, 防止应用未调用接口导致内存泄漏
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDefaultWinMsg(
                                       MMI_WIN_ID_T      win_id, 
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
    MMI_HANDLE_T    anim_handle = 0;

    if ( PNULL == win_info_ptr || PNULL == win_info_ptr->user_func )
    {
        return MMI_RESULT_FALSE;
    }

    //转义消息
    if ( MMI_RESULT_TRUE == ( result = MMIPUBHandleButtonMsg( win_info_ptr, msg_id, param ) ) )
    {
        return result;
    }

    // 为防止在MMIPUBHandleButtonMsg里销毁了window，win_info_ptr变成野指针
    if ( !MMK_IsOpenWin( win_id ) )
    {
        return result;
    }

    result = win_info_ptr->user_func( win_id, msg_id, param );
    //用户开自定义的timer里关窗口就会跑到这个case
    //如果不return下面处理waitwin timer的case里就会写野指针!
    if ( !MMK_IsOpenWin( win_id ) )
    {
        return result;
    }

    //wait win延迟刷新处理
    if ( (MMIPUBWIN_WAIT == win_info_ptr->type) && (win_info_ptr->is_waitwin_update_delay)) 
    {
        switch( msg_id )
        {
        case MSG_OPEN_WINDOW:
            //开启延迟刷新定时器
            if ( 0 != ( win_info_ptr->update_delay_timer_id = MMK_CreateWinTimer( win_info_ptr->win_handle,
                win_info_ptr->pubwin_theme.update_delay_ticks, FALSE ) ) )
            {
                MMITHEME_SetUpdateDelayCount(0xFF);
            }
            break;
            
        case MSG_TIMER:
            if (*(uint8*)param == win_info_ptr->update_delay_timer_id)
            {
                MMK_StopTimer( win_info_ptr->update_delay_timer_id );
                win_info_ptr->update_delay_timer_id = 0;
                MMITHEME_SetUpdateDelayCount(0);
                UpdatePubwinScreen(win_id);
//                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
            }
            break;

        case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
            //取消延迟刷新
            if ( 0 != win_info_ptr->update_delay_timer_id )
            {
                MMK_StopTimer( win_info_ptr->update_delay_timer_id );
                win_info_ptr->update_delay_timer_id = 0;
                MMITHEME_SetUpdateDelayCount(0);
            }
            break;
            
        default:
            break;
        }
    }
    //progress win起timer刷新, 而不是每次有改变就刷新
    else if ( MMIPUBWIN_PROGRESS == win_info_ptr->type )
    {
        switch( msg_id )
        {
        case MSG_FULL_PAINT:
            MMK_StopTimer(win_info_ptr->timer_id);
            win_info_ptr->timer_id = 0;

            //开启延迟刷新定时器
            win_info_ptr->timer_id = MMK_CreateWinTimer( win_info_ptr->win_handle, MMI_500MSECONDS, FALSE );
            break;
            
        case MSG_TIMER:
            if ( MMK_IsFocusWin(win_id)
                && win_info_ptr->timer_id == *(uint8*)param )
            {
                UpdatePubwinScreen(win_id);
//                MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                //临时修改花屏问题
                //MMK_UpdateScreen();
                //临时修改花屏问题
                MMK_StopTimer(win_info_ptr->timer_id);
                win_info_ptr->timer_id = 0;

                win_info_ptr->timer_id = MMK_CreateWinTimer( win_info_ptr->win_handle, MMI_500MSECONDS, FALSE );
            }
            break;

        case MSG_GET_FOCUS:
            MMK_StopTimer(win_info_ptr->timer_id);
            win_info_ptr->timer_id = 0;

            win_info_ptr->timer_id = MMK_CreateWinTimer( win_info_ptr->win_handle, MMI_500MSECONDS, FALSE );
            break;

        case MSG_LOSE_FOCUS:
        case MSG_CLOSE_WINDOW:
            MMK_StopTimer( win_info_ptr->timer_id );
            win_info_ptr->timer_id = 0;
            break;
            
        default:
            break;
        }
    }

    //防止用户未调用接口导致内存泄漏
    //必须要呼叫的函数都写在此处, 防止用户遗忘调用
    if ( MSG_CLOSE_WINDOW == msg_id )
    {
        MMIPUB_DestroyWinData( win_info_ptr );
    }
    else if( MSG_OPEN_WINDOW == msg_id )
    {
        switch( win_info_ptr->type )
        {
        case MMIPUBWIN_ALERT:
        case MMIPUBWIN_QUERY:
        case MMIPUBWIN_WAIT:
            anim_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_WAIT_ANIM_CTRL_ID );
            MMK_SetAtvCtrl(win_id, anim_handle);
            break;
            
        default:
            break;
        }
    }
    else if ( MSG_LCD_SWITCH == msg_id )
    {
        MMIPUB_HandleLcdSwitch( win_info_ptr );
    }
    else if ( MSG_NOTIFY_UPDATE == msg_id )
    {
        MMIPUBClearClientRectBg( win_info_ptr, param );
    }

    return result;
}

/*****************************************************************************/
//  Description : convert button message to softkey
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:PDA工程,将button消息转为softkey消息,兼容
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUBHandleButtonMsg(
                                          MMIPUB_INFO_T*    win_info_ptr,
                                          MMI_MESSAGE_ID_E  msg_id, 
                                          DPARAM            param
                                          )
{
    MMI_RESULT_E     result = MMI_RESULT_FALSE;

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    MMI_HANDLE_T     win_handle = win_info_ptr->win_handle; // 先保存下这个handle，因为有可能后期会被close掉，win_info_ptr指向非法地址
    MMI_MESSAGE_ID_E new_msg_id = msg_id;

    if ( PNULL != param )
    {
        MMI_NOTIFY_T* notify_ptr = NULL;

        if ( MSG_NOTIFY_PENOK == msg_id || MSG_NOTIFY_MIDSK == msg_id )
        {
            notify_ptr = (MMI_NOTIFY_T*)param;
            
            if ( MMIPUB_LEFT_BUTTON_CTRL_ID == notify_ptr->src_id )
            {
                new_msg_id = MSG_APP_OK;
            }
             if ( MMIPUB_MIDDLE_BUTTON_CTRL_ID == notify_ptr->src_id )
            {
                new_msg_id = MSG_APP_WEB;
            }
            else if ( MMIPUB_RIGHT_BUTTON_CTRL_ID == notify_ptr->src_id )
            {
                new_msg_id = MSG_APP_CANCEL;
            }
        }
        else if ( MSG_NOTIFY_PENLONGOK == msg_id )
        {
            notify_ptr = (MMI_NOTIFY_T*)param;

            if ( MMIPUB_LEFT_BUTTON_CTRL_ID == notify_ptr->src_id )
            {
                new_msg_id = 0; //do nothing
            }
            if ( MMIPUB_MIDDLE_BUTTON_CTRL_ID == notify_ptr->src_id )
            {
                new_msg_id = 0; //do nothing
            }
            else if ( MMIPUB_RIGHT_BUTTON_CTRL_ID == notify_ptr->src_id )
            {
                new_msg_id = MSG_KEYLONG_CANCEL;
            }
        }
    }

    if ( new_msg_id != msg_id
        && 0 != new_msg_id )
    {
        //get active control handle
        MMI_HANDLE_T ctrl_handle = MMK_GetActiveCtrl(win_handle);

        if ( 0 != ctrl_handle )
        {
            result = MMK_RunCtrlProc(ctrl_handle,new_msg_id,PNULL);
            if (!result)
            {
                //get parent handle
                ctrl_handle = MMK_GetParentCtrlHandle(ctrl_handle);
                while (0 != ctrl_handle)
                {
                    result = MMK_RunCtrlProc(ctrl_handle,new_msg_id,PNULL);
                    if (result)
                    {
                        break;
                    }
                    ctrl_handle = MMK_GetParentCtrlHandle(ctrl_handle);
                }
            }
        }
        
        if ( MMI_RESULT_FALSE == result )
        {
            result = MMK_RunWinProc( win_handle, new_msg_id, PNULL );
        }
    }

    //若窗口已不存在, 立即返回
    if ( !MMK_IsOpenWin( win_handle ) )
    {
        result = MMI_RESULT_TRUE;
    }
#endif

    return result;
}

/*****************************************************************************/
//  Description : handle window message
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIPUBHandleWinMsg(
                                       MMIPUB_INFO_T*    win_info_ptr,
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    if (PNULL == win_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW: 
        if ( MMIPUBWIN_ALERT == win_info_ptr->type
            || MMIPUBWIN_WAIT == win_info_ptr->type
            || MMIPUBWIN_WAITING == win_info_ptr->type )
        {
            MMIDEFAULT_TurnOnBackLight();
        }
        SetWinSoftkey( win_info_ptr->win_handle );
        break;

    case MSG_FULL_PAINT:
        //display pubwin
        if ( !win_info_ptr->is_layout )
        {
            ParsePubWinDisplayText( win_info_ptr );
            
            MMIPUB_ReleaseLayer( win_info_ptr );
            
            MMIPUBLayout( win_info_ptr );
			MMIPUBSetButton( win_info_ptr, FALSE ); 
            SetPubWinAnimRect( win_info_ptr );
            SetPubWinTextBoxRect( win_info_ptr );
        }

        MMIPUBDisplayWin( win_info_ptr );
        break;

    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMIPUBHandlePubwinSoftkey( ((MSG_APP_CANCEL == msg_id) || (MSG_APP_WEB == msg_id)) ? TRUE : FALSE, win_info_ptr->win_handle, win_info_ptr->softkey_style );//bug1259177
        break;
	case MSG_KEYUP_RED:			//bug   待机界面按红键会去主菜单
		break;
    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Handle public windows softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBHandlePubwinSoftkey(
                                      BOOLEAN                  is_right_softkey,
                                      MMI_WIN_ID_T             win_id,
                                      MMIPUB_SOFTKEY_STYLE_E   softkey_style
                                      )
{
    if ((MMIPUB_SOFTKEY_OKEXIT == softkey_style) || 
        (MMIPUB_SOFTKEY_OKCANCEL == softkey_style) ||
        ((MMIPUB_SOFTKEY_ONE == softkey_style) && (is_right_softkey)))
    { 
        MMK_CloseWin(win_id);
    }
}

/*****************************************************************************/
//  Description : query handle message
//  Global resource dependence : 
//  Author:peng.chen
//  Note:
/*****************************************************************************/
LOCAL CAF_HANDLE_RESULT_E QueryHandleMsg(
                                         MMI_WIN_ID_T       win_id,     
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         void* param
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch(msg_id)
    {
    case MSG_APP_OK:
        {
            MMI_APPLET_START_T start ={0};

            MMIPUB_INFO_T *win_info_ptr = MMIPUB_GetWinAddDataPtr( win_id );
            if (PNULL != win_info_ptr)
            {
                start.guid = win_info_ptr->user_data;
                //动态applet加上标记
                start.state |= MMI_APPLET_STATE_DYNAMIC;
            
                MMIPUB_CloseQuerytWin(PNULL);

                MMK_StartApplet( &start ); 
            }                     
        }
        break;

    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;
        
    default:
        result = MMIPUB_HandleQueryWinMsg( win_id,msg_id,param );
        break;
    }
    return (result);
}

/*****************************************************************************/
//  Description : handle open start app waiter window
//  Global resource dependence : 
//  Author: liqing.peng
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleStartAppWaitWin(
                                         MMI_WIN_ID_T       win_id,     
                                         MMI_MESSAGE_ID_E   msg_id, 
                                         DPARAM             param
                                         )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
        case MSG_PUB_START_APP_WAIT:
        {
            MMI_APPLET_START_T start ={0};

            MMIPUB_INFO_T *win_info_ptr = MMIPUB_GetWinAddDataPtr( win_id );
            
            if (PNULL != win_info_ptr)
            {
                start.guid = win_info_ptr->user_data;
                //动态applet加上标记
                start.state |= MMI_APPLET_STATE_DYNAMIC;

                MMK_CloseWin(win_id);
                MMK_StartApplet(&start);
            }
            break;
        }
            
        default:
            result = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;      
    }
    return result;
}

/*****************************************************************************/
//  Description : handle query window message
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E CAFHandleQueryWinMsg(
                                        MMI_WIN_ID_T      win_id, 
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    if( msg_id == MSG_APP_WEB || msg_id == MSG_APP_OK || msg_id == MSG_APP_CANCEL )
    {
        MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

        if (PNULL == win_info_ptr)
        {
            return MMI_RESULT_FALSE;
        }

        //win_info_ptr->win_id = win_id;
        
        if( msg_id == MSG_APP_CANCEL )
        {
            MMK_SendMsg( win_info_ptr->user_data, MSG_PROMPTWIN_CANCEL, (DPARAM)&(win_info_ptr->win_id) );
        }
        else
        {
            MMK_SendMsg( win_info_ptr->user_data, MSG_PROMPTWIN_OK, (DPARAM)&(win_info_ptr->win_id) );
        }

        MMK_CloseWin( win_id );
    }
    else
    {
        result = MMIPUB_HandleQueryWinMsg( win_id, msg_id, param );
    }

    return result;
}

/*****************************************************************************/
//  Description : to handle the message of temp waiting window    
//  Global resource dependence : 
//  Author:bin.ji
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTempWaitWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
   
    switch (msg_id)
    {
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.    
    case MSG_APP_FLIP:  // 不响应翻盖
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;
    default:
//        result = MMIPUB_HandleWaitWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : Tips窗口 消息处理
//  Global resource dependence : 
//  Author: xiaoming.ren
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTipsWinMsg(
    MMI_WIN_ID_T win_id,
    MMI_MESSAGE_ID_E  msg_id, 
    DPARAM param
)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T    from_win_id = (MMI_WIN_ID_T)MMK_GetWinAddDataPtr(win_id);
    IGUICTRL_T     *ctrl_handle = PNULL;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            ctrl_handle = MMK_GetCtrlPtrByWin(win_id, MMITHEME_GetStatusBarCtrlId());
            if(0 != ctrl_handle)
            {
                IGUICTRL_SetState(ctrl_handle, GUICTRL_STATE_DISABLE_ACTIVE | GUICTRL_STATE_INVISIBLE | GUICTRL_STATE_DISABLE_TP, TRUE);
            }
            MMIPUB_StartDisTips(win_id, &s_time_out);
#if defined(MMIPUBWIN_PDASTYLE_SUPPORT)
            GUIWIN_SetStbState(win_id, GUISTATUSBAR_STATE_INVALID, TRUE); 
#endif
        }
        break;
    case MSG_FULL_PAINT:
        {
            DisplayTipsPtr(win_id);
        }
        break;
    case MSG_LOSE_FOCUS:
        {
            MMIPUB_ClearTips();
        }
        break;
    case MSG_CLOSE_WINDOW:
        {
            if (0 != from_win_id && MMK_IsOpenWin(from_win_id))
            {
                //from_win_id 为0时消息不下发
                MMK_SendMsg(from_win_id, MSG_TIPSWIN_CLOSED, PNULL);
            }
        }
        break;
    case MSG_GET_FOCUS:
    case MSG_PRE_FULL_PAINT:
    case MSG_LCD_SWITCH:
        break;
    default:
        {
            if (MsgIsNeedSendToPreWin(msg_id))
            {
                if (MMK_SendMsg(MMK_GetPrevWinId(win_id), msg_id, param))
                {
                    MMIPUB_ClearTips();
                }
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }    
        break;
    }

    return result;
}

#if defined (MAINLCD_SIZE_128X64)
/*****************************************************************************/
//  Description : display public window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBDisplayWin(
                             MMIPUB_INFO_T    *win_info_ptr
                             )
{
    GUI_RECT_T      bg_rect = {0};
    GUI_RECT_T      alert_rect = {0};

    // low memory project
    GUI_RECT_T				rect ={0,0,MMI_MAINSCREEN_RIGHT_MAX_PIXEL,MMI_CLIENT_RECT_BOTTOM};
    const GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

    GUI_FillRect(&lcd_dev_info, rect, MMI_WHITE_COLOR);

    //draw text
    bg_rect = win_info_ptr->bg_rect;
    alert_rect = win_info_ptr->alert_rect;

     //add for cr224580  begin           
    if ( MMITHEME_CheckImageID( win_info_ptr->prompt_image_id ) )
    {
        GUIRES_DisplayImg(PNULL,
            &alert_rect,
            PNULL,
            win_info_ptr->win_handle,
            win_info_ptr->prompt_image_id,
            &lcd_dev_info);
    }
     //add for cr224580  end           
    
    if ( !GUI_IsInvalidRect( alert_rect ) )
    {
        bg_rect.top = alert_rect.bottom;
    }

    bg_rect.top += win_info_ptr->pubwin_theme.layout.client_v_margin;

    DisplayPubWinText( win_info_ptr, bg_rect, lcd_dev_info ); 
}
#else
/*****************************************************************************/
//  Description : display public window
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBDisplayWin(
                             MMIPUB_INFO_T    *win_info_ptr
                             )
{
#ifdef MMI_MENUMASKLAYER_SUPPORT
    BOOLEAN         create_1_ok = FALSE;
#endif
    BOOLEAN         create_2_ok = FALSE;
    GUI_RECT_T      full_rect = {0};
    GUI_RECT_T      bg_rect = {0};
    GUI_RECT_T      alert_rect = {0};
    GUI_RECT_T      title_rect = {0};
    GUI_RECT_T      client_rect = {0};
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    GUI_RECT_T      image_rect = {0};
#endif
    GUI_LCD_DEV_INFO lcd_dev_info = {0};

    if (PNULL == win_info_ptr)
    {
        return;
    }
    if(MMICC_IsExistActiveCall()||MMICC_IsExistIncommingCall()||MMICC_IsExistOutgoingCall())    //add 打电话的时候不弹出微聊提醒//zhou
    {
		MMK_CloseWin(win_info_ptr->win_id);
		return;		 
	}

    if ( 0 != win_info_ptr->update_delay_timer_id || !MMK_IsFocusWin(win_info_ptr->win_handle) )
    {
        if ( MMIPUBWIN_FORM != win_info_ptr->type )
        {
            SetInlineCtrlInvisible(win_info_ptr, TRUE); // 设置不可见
            return;
        }
    }

    //if(win_info_ptr->title_string.wstr_len == 0)
    {
        win_info_ptr->client_rect.top = win_info_ptr->title_rect.top;
    }
    
    //yangyu end

    bg_rect = win_info_ptr->bg_rect;
    alert_rect = win_info_ptr->alert_rect;
    title_rect = win_info_ptr->title_rect;
    client_rect = win_info_ptr->client_rect;

    full_rect = GetFullRect(win_info_ptr);

    if ( MMK_IsFocusWin(win_info_ptr->win_handle) )
    {
#ifdef MMI_MENUMASKLAYER_SUPPORT
        //  创建层
        create_1_ok = CreatePubWinLayer(win_info_ptr, &win_info_ptr->pubwin_mask_dev, full_rect, TRUE);
        
        create_2_ok = CreatePubWinLayer(win_info_ptr, &win_info_ptr->pubwin_bg_dev, bg_rect, FALSE);
        
        if (!create_1_ok || !create_2_ok) // 图层申请失败，都画在主层上
        {
            ReleasePubWinLayer( &win_info_ptr->pubwin_bg_dev );
            ReleasePubWinLayer( &win_info_ptr->pubwin_mask_dev );
        }
#else
#if 0 //remove because all displays are drawn on the main layer 
        create_2_ok = CreatePubWinLayer(win_info_ptr, &win_info_ptr->pubwin_bg_dev, bg_rect, FALSE);
        
        if (!create_2_ok) // 图层申请失败，都画在主层上
        {
            ReleasePubWinLayer( &win_info_ptr->pubwin_bg_dev );
        }
#endif
#endif
        lcd_dev_info = GetPubWinLayer(win_info_ptr);
    }

    // 设置控件可见
    SetInlineCtrlInvisible(win_info_ptr, FALSE); // 设置可见
    
    // 获得主背景层，并clear layer
//     ClearPubWinLayer(lcd_dev_info);

    // 设置控件显示的层
    SetInlineCtrlDispLayer(win_info_ptr, lcd_dev_info);

    // 设置softkey显示的层
    SetParentWinSoftkeyDispLayer(win_info_ptr->win_handle);

    // 画背景蒙层，如果支持多层，则会append full layer
    DisplayPubWinBg( win_info_ptr);

    // 具体画图和文字的操作
    IMG_EnableTransparentColor(TRUE);

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    if ( MMIPUBWIN_WAITING != win_info_ptr->type )
    {
        //分别绘制top\text\bottom bg 
        image_rect = bg_rect;
        
        if ( IsImLandscape( win_info_ptr ) )
        {
            image_rect.top = title_rect.top;
            image_rect.left = client_rect.right + 1;
            LCD_FillRect( &lcd_dev_info, image_rect, MMIPUB_IM_BUTTON_BG_COLOR );

            image_rect.left = bg_rect.left;
            image_rect.right = client_rect.right;
            LCD_FillRect( &lcd_dev_info, image_rect, MMIPUB_IM_BG_COLOR );
            
            //draw title
            DisplayTitleText( win_info_ptr, title_rect, lcd_dev_info );
        }
        else
        {
            BOOLEAN is_exist_skb = IsExistSoftkey( win_info_ptr );

	     #if 1   //xiongkai    带按钮弹出框左右空出的像素   来微聊提醒框   IMEI查看框   定时开关机来闹
               image_rect.left=image_rect.left+3;
               image_rect.right=image_rect.right-3;		 
	     #else	 
               image_rect.left=image_rect.left+10;
               image_rect.right=image_rect.right-10;
	     #endif
			SCI_TRACE_LOW("[mmmmmm] line[%d] file[%s] func[%s] win_info_ptr->type=%d win_info_ptr->softkey_style=%d", __LINE__, __FILE__, __FUNCTION__,__FUNCTION__,win_info_ptr->type,win_info_ptr->softkey_style);

            // 顶上的title图
            image_rect.bottom = image_rect.top + GetPubWinTopBGHeight( win_info_ptr ) - 1;
			//LCD_FillRoundedRect(&lcd_dev_info, image_rect, image_rect , MMI_BLACK_COLOR);
            //DisplayTitleText( win_info_ptr, title_rect, lcd_dev_info );
			
			//LCD_FillRoundedRect(&lcd_dev_info, image_rect, image_rect , MMI_WHITE_COLOR);
								
            // 客户区背景
            //image_rect.top = image_rect.bottom + 1;
            image_rect.top = image_rect.bottom-3;    
            image_rect.bottom = bg_rect.bottom - GetPubWinBottomBGHeight( win_info_ptr, is_exist_skb );
            //yangyu add
            if(win_info_ptr->title_string.wstr_len == 0)
            {
                image_rect.top = win_info_ptr->title_rect.top;
            }

            //yangyu end
			LCD_FillRoundedRect(&lcd_dev_info, image_rect, image_rect , MMI_WHITE_COLOR);  //xiongkai  弹出框正文背景色
            //GUI_FillRect(&lcd_dev_info, image_rect, MMI_WHITE_COLOR); //xiongkai  弹出框正文背景色
			
            
            /*GUIRES_DisplayImg(PNULL,
                &image_rect,
                PNULL,
                win_info_ptr->win_handle,
                win_info_ptr->pubwin_theme.bg_id,
                &lcd_dev_info);*/
            
            // 底下的button背景
            //image_rect.top = image_rect.bottom + 1;
			image_rect.top = image_rect.bottom-5;
            image_rect.bottom = bg_rect.bottom;
            
            //DisplayPubWinBottomBG( win_info_ptr, &image_rect, &lcd_dev_info, is_exist_skb );
         GUIRES_DisplayImg(PNULL,
         	&image_rect,
         	PNULL,
         	win_info_ptr->win_handle,
         	IMG_POPUP_BUTTON, // win_info_ptr->pubwin_theme.bg_id,
         	&lcd_dev_info);
        }
    }
    else
#endif
    {
        GUIRES_DisplayImg(PNULL,
            &bg_rect,
            PNULL,
            win_info_ptr->win_handle,
            win_info_ptr->pubwin_theme.bg_id,
            &lcd_dev_info);
    }
    
    //没有动画才需要绘制
    if ( 0 == win_info_ptr->wait_anim_id )
    {
        if ( MMITHEME_CheckImageID( win_info_ptr->prompt_image_id ) )
        {
            GUIRES_DisplayImg(PNULL,
                &alert_rect,
                PNULL,
                win_info_ptr->win_handle,
                win_info_ptr->prompt_image_id,
                &lcd_dev_info);
        }
    }
     
    if ( !GUI_IsInvalidRect( alert_rect ) )
    {
        bg_rect.top = alert_rect.bottom;
    }
    
    bg_rect.top += win_info_ptr->pubwin_theme.layout.client_v_margin;
//fixed by hongjun.jia for bugzilla 2443/2444
#ifdef MAINLCD_SIZE_176X220 
	bg_rect.bottom -= MMI_PUBWIN_TEXT_BOTTOM_MARGIN;
#endif
    
    // 客户区文字
    DisplayPubWinText( win_info_ptr, bg_rect, lcd_dev_info );

    IMG_EnableTransparentColor(FALSE);

    // 显示动画
    DisplayPubWinAnim(win_info_ptr);

    win_info_ptr->is_3danim_invalid = TRUE;
}
#endif

/*****************************************************************************/
//  Description : display public window
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void DisplayProgressWin(
                              MMIPUB_INFO_T    *win_info_ptr,
                              uint32            total_size,
                              uint32            finished_size
                              )
{
#ifdef MMI_MENUMASKLAYER_SUPPORT
    BOOLEAN         create_1_ok = FALSE;
#endif
    BOOLEAN         create_2_ok = FALSE;
    MMI_STRING_T    disp_str = {0};
    wchar           wdisp_buf[MMIPUB__PROGRESS_SIZE_INFO_LEN +1] = {0};
    uint8           u8disp_buf[MMIPUB__PROGRESS_SIZE_INFO_LEN +1] = {0};
    GUI_RECT_T      disp_rect  = {0};
    GUI_RECT_T      bg_rect    = {0};
    GUI_RECT_T      alert_rect = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    GUI_RECT_T      title_rect = {0};
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
    uint16          line_height = 0;
    uint16          line_num    = 0;
#else
    GUI_RECT_T      image_rect = {0};
    BOOLEAN         is_exist_skb = FALSE;
#endif
    
    if (PNULL == win_info_ptr 
        || !MMK_IsFocusWin(win_info_ptr->win_handle) )
    {
        return ;
    }

    // 计算区域
    //MMIPUBLayout( win_info_ptr, &bg_rect, &alert_rect, &title_rect, PNULL );
    bg_rect = win_info_ptr->bg_rect;
    alert_rect = win_info_ptr->alert_rect;
    title_rect = win_info_ptr->title_rect;

#ifdef MMI_MENUMASKLAYER_SUPPORT
    //  创建层
    create_1_ok = CreatePubWinLayer(win_info_ptr, &win_info_ptr->pubwin_mask_dev, GetFullRect(win_info_ptr), TRUE);
    
    create_2_ok = CreatePubWinLayer(win_info_ptr, &win_info_ptr->pubwin_bg_dev, bg_rect, FALSE);
    
    if (!create_1_ok || !create_2_ok) // 图层申请失败，都画在主层上
    {
        ReleasePubWinLayer( &win_info_ptr->pubwin_bg_dev );
        ReleasePubWinLayer( &win_info_ptr->pubwin_mask_dev );
    }
#else
    create_2_ok = CreatePubWinLayer(win_info_ptr, &win_info_ptr->pubwin_bg_dev, bg_rect, FALSE);
    
    if (!create_2_ok) // 图层申请失败，都画在主层上
    {
        ReleasePubWinLayer( &win_info_ptr->pubwin_bg_dev );
    }
#endif
    lcd_dev_info = GetPubWinLayer(win_info_ptr);

    // 显示背景，并append layer
    DisplayPubWinBg( win_info_ptr );

    // 获得主背景，并clear layer

//     ClearPubWinLayer(lcd_dev_info);

    // 设置控件显示的层
    SetInlineCtrlDispLayer(win_info_ptr, lcd_dev_info);

    // 设置显示的层
    SetParentWinSoftkeyDispLayer(win_info_ptr->win_handle);

    // 下面具体画图片和文字
    IMG_EnableTransparentColor(TRUE);

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    //分别绘制top\text\bottom bg 
    image_rect = bg_rect;

    is_exist_skb = IsExistSoftkey( win_info_ptr );
    
    //top bg
    image_rect.bottom = image_rect.top + GetPubWinTopBGHeight( win_info_ptr ) - 1;
    GUIRES_DisplayImg(PNULL,
        &image_rect,
        PNULL,
        win_info_ptr->win_handle,
        win_info_ptr->pubwin_theme.layout.top_bg_id,
        &lcd_dev_info);

    //draw title
    DisplayTitleText( win_info_ptr, title_rect, lcd_dev_info );
    
    //text bg
    image_rect.top = image_rect.bottom + 1;
    image_rect.bottom = bg_rect.bottom - GetPubWinBottomBGHeight( win_info_ptr, is_exist_skb );
    GUIRES_DisplayImg(PNULL,
        &image_rect,
        PNULL,
        win_info_ptr->win_handle,
        win_info_ptr->pubwin_theme.bg_id,
        &lcd_dev_info);
    
    //bottom bg
    image_rect.top = image_rect.bottom + 1;
    image_rect.bottom = bg_rect.bottom;

    DisplayPubWinBottomBG( win_info_ptr, &image_rect, &lcd_dev_info, is_exist_skb );

#else
    GUIRES_DisplayImg(PNULL,
        &bg_rect,
        PNULL,
        win_info_ptr->win_handle,
        win_info_ptr->pubwin_theme.bg_id,
        &lcd_dev_info);
#endif

    //display "XXXXX/xxxxx" size information
    sprintf(u8disp_buf, "%ld/%ld",finished_size, total_size);/*lint !e64*/
    disp_str.wstr_len = (uint16)strlen((char *)u8disp_buf);
    disp_str.wstr_len = (uint16)MIN(disp_str.wstr_len, (uint16)MMIPUB__PROGRESS_SIZE_INFO_LEN);/*lint !e64*/
    MMI_STRNTOWSTR(wdisp_buf, MMIPUB__PROGRESS_SIZE_INFO_LEN, u8disp_buf, MMIPUB__PROGRESS_SIZE_INFO_LEN, disp_str.wstr_len);/*lint !e64*/
    disp_str.wstr_ptr = wdisp_buf;

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    disp_rect = image_rect;
    disp_rect.bottom = image_rect.top - win_info_ptr->pubwin_theme.layout.client_v_margin;
    disp_rect.top = disp_rect.bottom
            - GUI_ConvertGUIFontToSize( win_info_ptr->pubwin_theme.small_font )
            - win_info_ptr->pubwin_theme.line_space
            + 1;
#else
    line_num = ( win_info_ptr->line_num >= MMIPUB_MULTILINE_MAX - 1 ) ? MMIPUB_MULTILINE_MAX : win_info_ptr->line_num + 1;
#ifdef MAINLCD_SIZE_176X220 
	bg_rect.bottom -= MMI_PUBWIN_TEXT_BOTTOM_MARGIN;
#endif
    line_height = ( bg_rect.bottom - bg_rect.top - ( alert_rect.bottom - alert_rect.top )
        - win_info_ptr->pubwin_theme.layout.title_v_top_margin - win_info_ptr->pubwin_theme.layout.client_v_margin) /line_num;

    //draw text
    disp_rect = bg_rect;
    disp_rect.top += win_info_ptr->pubwin_theme.layout.client_v_margin;
    disp_rect.bottom = disp_rect.top + (line_num - 1) * line_height;
    DisplayPubWinText( win_info_ptr, disp_rect, lcd_dev_info );

    disp_rect.top = disp_rect.bottom;
    disp_rect.bottom = disp_rect.top + line_height;
#endif

    DisplayProgressText( win_info_ptr, disp_str, disp_rect, lcd_dev_info );

    //////////////////////////////////////////////////////////////////////////
    // fixed by feng.xiao for NEWMS00180461
    // -fix: progress bg using prg_img_info instead of prompt_image_id
    // -fix: progress ft using display img instead of rect filling
    // -add: force update screen, if the app close win before the last prg disp
//     if ( MMITHEME_CheckImageID( win_info_ptr->prompt_image_id ) 
    if (MMITHEME_CheckImageID(win_info_ptr->prg_img_info.m_bg_img_id) 
        && MMITHEME_CheckImageID(win_info_ptr->prg_img_info.m_ft_img_id))
    {
//         GUIRES_DisplayImg(PNULL,
//             &alert_rect,
//             PNULL,
//             win_info_ptr->win_handle,
//             win_info_ptr->prompt_image_id,
//             &lcd_dev_info);
        GUIRES_DisplayImg(PNULL, &alert_rect, PNULL, win_info_ptr->win_handle, 
                          win_info_ptr->prg_img_info.m_bg_img_id, &lcd_dev_info);

        if ( total_size > 0
            && finished_size > 0 )
        {
            int16 right = 0;
            
            disp_rect = alert_rect;
            
#if 0
#ifdef MAINLCD_SIZE_320X480
            disp_rect.top += 1;
            disp_rect.bottom -= 1;
#else
            disp_rect.top += 3;
            disp_rect.bottom -= 3;
#endif
#endif
			//bug98014: 当rect出现0值时，right减1后出现负值，导致绘制图片区域的left大于right，不应进行减一运算，注掉下面两行
			/*
            disp_rect.left += 1;
            disp_rect.right -= 1;
            */
            right = disp_rect.left + (int16)((uint16)(alert_rect.right - alert_rect.left - 1) * finished_size / total_size) - 1;
            
            disp_rect.right = MIN( disp_rect.right, right );
            
//             LCD_FillRect(&lcd_dev_info, disp_rect, MMI_DARK_BLUE_COLOR);
            GUIRES_DisplayImg(PNULL, &disp_rect, PNULL, win_info_ptr->win_handle, 
                              win_info_ptr->prg_img_info.m_ft_img_id, &lcd_dev_info);
            //force update screen, to avoid the app close progress win before the final prg disp
            if (disp_rect.right == alert_rect.right - 1)
            {
                MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );
                GUITEXT_SetString( ctrl_handle, win_info_ptr->string[0].wstr_ptr, win_info_ptr->string[0].wstr_len, TRUE );
                MMIPUBSetButton( win_info_ptr, TRUE );
                MMITHEME_UpdateRect();
            }
        }
    }

    IMG_EnableTransparentColor(FALSE);

    // 显示动画
    DisplayPubWinAnim(win_info_ptr);

    win_info_ptr->is_3danim_invalid = TRUE;
}

/*****************************************************************************/
//  Description : draw pub win bg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayPubWinBg(
                           MMIPUB_INFO_T *win_info_ptr
                           )
{
    GUI_LCD_DEV_INFO lcd_dev_info = win_info_ptr->pubwin_theme.bg_lcd_dev;

    //非focus不需要绘制, 否则多窗口叠加颜色会越来越深
    if ( MMK_IsFocusWin( win_info_ptr->win_handle ) )
    {
#ifdef UI_MULTILAYER_SUPPORT
        if (UILAYER_IsMultiLayerEnable() 
            && UILAYER_IsLayerActive(&win_info_ptr->pubwin_mask_dev)
            && 0 != win_info_ptr->pubwin_mask_dev.block_id)
        {
            lcd_dev_info = win_info_ptr->pubwin_mask_dev;

            AppendPubWinLayer(lcd_dev_info);
            //ClearPubWinLayer(lcd_dev_info);
        }
#endif
    }

	if (MMIPUBWIN_WAITING != win_info_ptr->type
        && (
#ifdef MMI_MENUMASKLAYER_SUPPORT
            win_info_ptr->pubwin_theme.is_support_anim ||
#endif
            MMK_IsUpdateScreen()
        ))
	{
		LCD_FillArgbRect( &lcd_dev_info, 
			GetFullRect(win_info_ptr), 
			win_info_ptr->pubwin_theme.bg_color,
			win_info_ptr->pubwin_theme.bg_alpha );
	}
}

/*****************************************************************************/
//  Description : display pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayTitleText(
                            MMIPUB_INFO_T*   win_info_ptr,
                            GUI_RECT_T       title_rect,
                            GUI_LCD_DEV_INFO lcd_dev_info
                            )
{
    GUISTR_STYLE_T title_style = {0};
    
    title_style.align      = ALIGN_LVMIDDLE;
    title_style.font       = win_info_ptr->pubwin_theme.normal_font;
    title_style.font_color = win_info_ptr->pubwin_theme.font_color;
    title_style.line_space = win_info_ptr->pubwin_theme.line_space;
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &title_rect,
        &title_rect,
        &win_info_ptr->title_string,	   
        &title_style,
        GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
//  Description : display pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayPubWinText(
                             MMIPUB_INFO_T *win_info_ptr,
                             GUI_RECT_T rect,
                             GUI_LCD_DEV_INFO lcd_dev_info
                             )
{
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_WORDBREAK | GUISTR_STATE_ELLIPSIS;
    uint16 line_num = ( MMIPUBWIN_PROGRESS == win_info_ptr->type ) ? MMIPUB_MULTILINE_MAX - 1 : MMIPUB_MULTILINE_MAX;

    text_style.align = ALIGN_HVMIDDLE;    
    text_style.font_color = win_info_ptr->pubwin_theme.font_color;
    text_style.line_space = win_info_ptr->pubwin_theme.line_space;

    rect.left += win_info_ptr->pubwin_theme.layout.client_h_margin;
    rect.right -= win_info_ptr->pubwin_theme.layout.client_h_margin;

    if ( win_info_ptr->line_num > line_num )
    {
        uint32 i = 0;
        uint32 last_index = 0;
        GUI_RECT_T line_rect = rect;
        uint16 line_height = ( rect.bottom - rect.top ) / line_num;

        text_style.font  = win_info_ptr->pubwin_theme.normal_font;

        for ( i = 0; i < MMIPUB_TEXT_MAX; i++ )
        {
            if ( PNULL != win_info_ptr->string[i].wstr_ptr
                && 0 != win_info_ptr->string[i].wstr_len )
            {
                last_index = i;
            }
        }

        line_rect.bottom = line_rect.top + line_height;

        for ( i = 0; i < MMIPUB_TEXT_MAX; i++ )
        {
            if ( PNULL != win_info_ptr->string[i].wstr_ptr
                && 0 != win_info_ptr->string[i].wstr_len )
            {
                if ( last_index == i )
                {
                    line_rect.bottom = rect.bottom;
					if (0 != last_index)
					{
						text_state |= GUISTR_STATE_SINGLE_LINE;
					}
                    GUISTR_DrawTextToLCDInRect(
                        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                        &line_rect,
                        &line_rect,
                        &win_info_ptr->string[i],	   
                        &text_style,
                        text_state,
                        GUISTR_TEXT_DIR_AUTO
                        );
                }
                else
                {
                    GUISTR_DrawTextToLCDInRect(
                        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                        &line_rect,
                        &line_rect,
                        &win_info_ptr->string[i],	   
                        &text_style,
                        text_state | GUISTR_STATE_SINGLE_LINE,
                        GUISTR_TEXT_DIR_AUTO
                        );
                }

                line_rect.top = line_rect.bottom;
                line_rect.bottom = line_rect.top + line_height;
            }
        }
    }
    else
    {
        MMI_STRING_T string = {0};

        if ( CreatePubWinDisplayText( win_info_ptr, &string ) )
        {
            text_style.font  = ( win_info_ptr->line_num >= line_num )
                ? win_info_ptr->pubwin_theme.normal_font : win_info_ptr->pubwin_theme.big_font;
            
            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                &rect,
                &rect,
                &string,	   
                &text_style,
                text_state,
                GUISTR_TEXT_DIR_AUTO
                );
            
            DestroyPubWinDisplayText( &string );
        }
    }
#endif
}

/*****************************************************************************/
//  Description : display pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayProgressText(
                               MMIPUB_INFO_T *win_info_ptr,
                               MMI_STRING_T   string,
                               GUI_RECT_T     rect,
                               GUI_LCD_DEV_INFO lcd_dev_info
                               )
{
    GUISTR_STYLE_T text_style = {0};
    GUISTR_STATE_T text_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ELLIPSIS;

    rect.left += win_info_ptr->pubwin_theme.layout.client_h_margin;
    rect.right -= win_info_ptr->pubwin_theme.layout.client_h_margin;
    
    text_style.align = ALIGN_HVMIDDLE;

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    text_style.font = win_info_ptr->pubwin_theme.normal_font;
#else
    text_style.font  = ( win_info_ptr->line_num >= MMIPUB_MULTILINE_MAX - 1 )//prgwin需要为 0/0 预留一行
        ? win_info_ptr->pubwin_theme.normal_font : win_info_ptr->pubwin_theme.big_font;
#endif

    text_style.font_color = win_info_ptr->pubwin_theme.font_color;
    text_style.line_space = win_info_ptr->pubwin_theme.line_space;
    
    GUISTR_DrawTextToLCDInRect(
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        &rect,
        &rect,
        &string,	   
        &text_style,
        text_state,
        GUISTR_TEXT_DIR_AUTO
        );
}

/*****************************************************************************/
//  Description : display public anim
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void DisplayPubWinAnim(
                             MMIPUB_INFO_T    *win_info_ptr
                             )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
	//SCALE_IMAGE_CAP_T scale_cap = {0};

    lcd_dev_info = GetPubWinLayer(win_info_ptr);

    // AppendPubWinLayer(win_info_ptr->pubwin_bg_dev);

    //软件做特效，不需要判断硬件状态
// 	GRAPH_GetScaleCapability(&scale_cap);
// 
// 	if (!scale_cap.is_idle)
// 	{
// 		// append pubwin的层
// 		AppendPubWinLayer(win_info_ptr->pubwin_bg_dev);
// 		return;
// 	}
#ifdef UI_MULTILAYER_SUPPORT
    if (win_info_ptr->pubwin_theme.is_support_anim
        && !win_info_ptr->is_3danim_invalid 
        && 0 != lcd_dev_info.block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info.block_id
        && UILAYER_IsLayerActive(&lcd_dev_info)
        && MMI_IsPermit3D((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MMI_3D_DESKTOP)
#ifdef UI_P3D_SUPPORT     
        && MMITHEME_Is3DOpen()
#endif        
        )
    {
#ifdef EFFECT_ZOOM_SUPPORT
        if ( !MMK_GetCtrlHandleByGuid( win_info_ptr->win_handle, SPRD_GUI_IM_ID ) )
        {
            MMITHEME_ZOOM_DATA_T    zoom_data = {0};
            // 刷新原始层上的控件
            UpdateInlineCtrl(win_info_ptr);

            // 做动画
            zoom_data.content_lcd_dev = win_info_ptr->pubwin_bg_dev;
            zoom_data.mask_lcd_dev = win_info_ptr->pubwin_mask_dev;
            zoom_data.mask_alpha = win_info_ptr->pubwin_theme.bg_alpha;
            zoom_data.mask_color = win_info_ptr->pubwin_theme.bg_color;
            //zoom_data.rect_ptr = &win_info_ptr->bg_rect;
            MMITHEME_PlayZoomAnim(&zoom_data, FALSE);

            AppendPubWinLayer(zoom_data.mask_lcd_dev);
        }
#endif
    }
#endif
    // append pubwin的层
    AppendPubWinLayer(win_info_ptr->pubwin_bg_dev);
}

/*****************************************************************************/
//  Description : display pubwin bottom bg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void DisplayPubWinBottomBG(
                                 MMIPUB_INFO_T    *win_info_ptr,
                                 GUI_RECT_T       *rect_ptr,
                                 GUI_LCD_DEV_INFO *dev_ptr,
                                 BOOLEAN           is_exist_skb
                                 )
{
    MMI_IMAGE_ID_T image_id = 0;

    if (is_exist_skb)
    {
        image_id = win_info_ptr->pubwin_theme.layout.bottom_bg_id;
    }
    else
    {
        //image_id = win_info_ptr->pubwin_theme.layout.bottom_bg_without_button_id;
		image_id = win_info_ptr->pubwin_theme.layout.bottom_bg_id;
    }

    //GUIRES_DisplayImg(
       // PNULL, rect_ptr, PNULL,
       // win_info_ptr->win_handle, image_id, dev_ptr );
}

/*****************************************************************************/
//  Description : creat text box control
//  Global resource dependence : 
//  Author: aoke
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T CreatTextBoxCtrl(
                                    MMI_HANDLE_T                   win_handle,     //win id
                                    MMI_CTRL_ID_T                  ctrl_id,    //control id
                                    MMI_STRING_T*                  string_ptr
                                    )
{
    MMI_HANDLE_T   ctrl_handle  = 0;
    MMIPUB_INFO_T* win_info_ptr = MMIPUB_GetWinAddDataPtr( win_handle );
    
    if ( PNULL != win_info_ptr )
    {
        MMI_CONTROL_CREATE_T create = {0};
        GUITEXT_INIT_DATA_T  init_data = {0};
        GUI_BG_T             bg = {0};
        
        //init_data.both_rect.v_rect = init_data.both_rect.h_rect = win_info_ptr->client_rect;
        create.ctrl_id = ctrl_id;
        create.guid    = SPRD_GUI_TEXTBOX_ID;
        create.parent_win_handle = win_handle;  
        create.init_data_ptr = &init_data;
        MMK_CreateControl( &create );
        
        ctrl_handle = MMK_GetCtrlHandleByWin( win_handle, ctrl_id );
        
        GUITEXT_SetBg( ctrl_handle, &bg );
        
        GUITEXT_IsDisplayPrg( FALSE, ctrl_handle );
        
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        GUITEXT_SetFont( ctrl_handle, &win_info_ptr->pubwin_theme.small_font, &win_info_ptr->pubwin_theme.font_color );
        GUITEXT_SetAlign( ctrl_handle, ALIGN_HVMIDDLE );
        GUITEXT_SetMarginEx( 
                ctrl_handle,
                win_info_ptr->pubwin_theme.layout.button_margin - win_info_ptr->pubwin_theme.layout.client_h_margin,
                win_info_ptr->pubwin_theme.layout.button_margin - win_info_ptr->pubwin_theme.layout.client_h_margin,
                win_info_ptr->pubwin_theme.layout.client_v_margin,
                win_info_ptr->pubwin_theme.layout.client_v_margin);

        GUITEXT_SetLineSpace( ctrl_handle, win_info_ptr->pubwin_theme.line_space);
#else
        GUITEXT_SetAlign( ctrl_handle, ALIGN_HVMIDDLE );
#endif
        GUITEXT_SetString( ctrl_handle, string_ptr->wstr_ptr, string_ptr->wstr_len, FALSE );

        GUIAPICTRL_SetState( ctrl_handle, GUICTRL_STATE_DISABLE_TP | GUICTRL_STATE_DISABLE_ACTIVE, TRUE );
    }

    return ctrl_handle;
}

/*****************************************************************************/
//  Description : set button
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBSetButton(
                            MMIPUB_INFO_T *win_info_ptr,
                            BOOLEAN is_fresh
                            )
{
//fixed by hongjun.jia for bugzilla 2443/2444
#if defined MMIPUBWIN_PDASTYLE_SUPPORT || defined MAINLCD_SIZE_176X220
    if ( PNULL != win_info_ptr 
        && MMIPUBWIN_WAITING != win_info_ptr->type )
    {
        MMI_TEXT_ID_T  softkey_id[GUISOFTKEY_BUTTON_NUM] = {0};
        BOOLEAN        is_softkey_gray[GUISOFTKEY_BUTTON_NUM] = {0};
        GUI_RECT_T     rect_arry[GUISOFTKEY_BUTTON_NUM] = {0};
        MMI_CTRL_ID_T  id_array[GUISOFTKEY_BUTTON_NUM] = {0};
        MMI_CTRL_ID_T  handle_array[GUISOFTKEY_BUTTON_NUM] = {0};
        uint32         num = 0;
        int32          i = 0;
        
        id_array[0] = MMIPUB_LEFT_BUTTON_CTRL_ID;
        id_array[1] = MMIPUB_MIDDLE_BUTTON_CTRL_ID;
        id_array[2] = MMIPUB_RIGHT_BUTTON_CTRL_ID;

        switch (win_info_ptr->softkey_style)
        {
        case MMIPUB_SOFTKEY_ONE:
        case MMIPUB_SOFTKEY_DIALOG_DISMISS:
            softkey_id[2] = win_info_ptr->softkey_id[2];
            is_softkey_gray[2] = win_info_ptr->is_softkey_gray[2];
            break;
            
        case MMIPUB_SOFTKEY_OKEXIT:
        case MMIPUB_SOFTKEY_OKCANCEL:
        case MMIPUB_SOFTKEY_CUSTOMER:
        case MMIPUB_SOFTKEY_DIALOG_ALARM:
            softkey_id[0] = win_info_ptr->softkey_id[0];
            softkey_id[1] = win_info_ptr->softkey_id[1];
            softkey_id[2] = win_info_ptr->softkey_id[2];

            is_softkey_gray[0] = win_info_ptr->is_softkey_gray[0];
            is_softkey_gray[1] = win_info_ptr->is_softkey_gray[1];
            is_softkey_gray[2] = win_info_ptr->is_softkey_gray[2];
            break;
            
        default:
            break;
        }


        for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
        {
            handle_array[i] = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, id_array[i] );

            if ( 0 != softkey_id[i]
                && TXT_NULL != softkey_id[i] )
            {
                if ( 0 == handle_array[i] )
                {
                    GUIBUTTON_INIT_DATA_T init_data = {0};
                    GUI_FONT_ALL_T  font={0};
                    //init
                    init_data.bg.bg_type = GUI_BG_IMG;
					init_data.bg.color	 = MMI_DARK_GREEN_COLOR; //  MMI_DARK_GREEN_COLOR; //win_info_ptr->pubwin_theme.dialog_bg_color;
                    init_data.bg.img_id  = PNULL; // win_info_ptr->pubwin_theme.layout.button_img_id;
                    //init_data.both_rect.v_rect = init_data.both_rect.h_rect = rect_arry[i];
                    //create
                    handle_array[i] = GUIBUTTON_CreateDynamic( win_info_ptr->win_handle, id_array[i], &init_data );

					font = win_info_ptr->pubwin_theme.layout.button_font;
                    
                    GUIBUTTON_SetFont( handle_array[i], &font );
                    
                    GUIBUTTON_SetRunSheen( handle_array[i], FALSE );
                    
                    init_data.bg.img_id = win_info_ptr->pubwin_theme.layout.button_img_hl_id;
                    GUIBUTTON_SetPressedBg( handle_array[i], &init_data.bg );

#ifdef MMI_PDA_SUPPORT
                    // 与需求讨论后，决定在非PDA模式下不走焦点
					GUIAPICTRL_SetState(handle_array[i], GUICTRL_STATE_KEYMODE_ACTIVE, TRUE);
#endif

                    // 这些情况下，让BUTTON获取焦点
                    if (MMIPUBWIN_ALERT == win_info_ptr->type
                        || MMIPUBWIN_QUERY == win_info_ptr->type
                        || MMIPUBWIN_WAIT == win_info_ptr->type)
                    {
                        if (0 == num)
                        {
                            MMK_SetAtvCtrl(win_info_ptr->win_handle, handle_array[i]);
                        }
                    }
                }

                num++;

                GUIBUTTON_SetTextId( handle_array[i], softkey_id[i] );
            }
            else
            {
                MMK_DestroyControl( handle_array[i] );
                handle_array[i] = 0;
            }
        }

        if ( IsExistSoftkey( win_info_ptr ) )
        {
            GUI_BG_T gray_bg = {0};
            
            gray_bg.bg_type = GUI_BG_IMG; // GUI_BG_COLOR; 
            gray_bg.color   = MMI_DARK_GREEN_COLOR; // MMI_DARK_GREEN_COLOR; // win_info_ptr->pubwin_theme.dialog_bg_color;
            gray_bg.img_id  = PNULL; //win_info_ptr->pubwin_theme.layout.button_img_id;		
            
            GetPubWinButtonRect( win_info_ptr, rect_arry, num );
            
            for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
            {
                GUIBUTTON_SetRect( handle_array[i], &rect_arry[i] );

                GUIBUTTON_SetHandleLong(handle_array[i], win_info_ptr->is_handle_long[i]);

                GUIBUTTON_SetGrayed( handle_array[i], is_softkey_gray[i], &gray_bg, is_fresh );
            }
        }
    }
#endif
}

/*****************************************************************************/
//  Description : creat softkey control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBCreatSoftkeyCtrl(
                                   MMI_HANDLE_T win_handle     //win id
                                   )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_handle);
    if (PNULL != win_info_ptr)
    {        
#if !defined (MMIPUBWIN_PDASTYLE_SUPPORT) && !defined (MAINLCD_SIZE_176X220)
        GUISOFTKEY_INIT_DATA_T	softkey_data = {0};
        MMI_CONTROL_CREATE_T softkey_create = {0};
        MMI_CTRL_ID_T	softkey_ctrl_id = 0;
        
        softkey_data.leftsoft_id	= TXT_COMMON_OK;
        softkey_data.midsoft_id		= TXT_NULL;
        softkey_data.rightsoft_id	= STXT_RETURN;
        
        softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
        softkey_create.ctrl_id = softkey_ctrl_id;
        softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
        softkey_create.parent_win_handle = win_handle;
        softkey_create.init_data_ptr = &softkey_data;
        
        MMK_CreateControl( &softkey_create );
        
        MMK_SetWinSoftkeyCtrlId(win_handle, softkey_ctrl_id);
        
#else
        if (MMIPUBWIN_WAITING == win_info_ptr->type)
        {
            GUISOFTKEY_INIT_DATA_T	softkey_data = {0};
            MMI_CONTROL_CREATE_T softkey_create = {0};
            MMI_CTRL_ID_T	softkey_ctrl_id = 0;
            
            softkey_data.leftsoft_id	= TXT_COMMON_OK;
            softkey_data.midsoft_id		= TXT_NULL;
            softkey_data.rightsoft_id	= STXT_RETURN;
            
            softkey_ctrl_id = MMITHEME_GetSoftkeyCtrlId();
            softkey_create.ctrl_id = softkey_ctrl_id;
            softkey_create.guid    = SPRD_GUI_SOFTKEY_ID;
            softkey_create.parent_win_handle = win_handle;
            softkey_create.init_data_ptr = &softkey_data;
            
            MMK_CreateControl( &softkey_create );
            
            MMK_SetWinSoftkeyCtrlId(win_handle, softkey_ctrl_id);
        }
        
#endif
    }
    SetWinSoftkey(win_handle);
    
    return;
}

/*****************************************************************************/
//  Description : creat anim control
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatAnimLabelCtrl(
                                 MMI_HANDLE_T   win_handle,
                                 MMI_CTRL_ID_T  ctrl_id,    //control id
                                 MMI_ANIM_ID_T  anim_id     //anim id
                                 )
{
    MMIPUB_INFO_T*          win_info_ptr = PNULL;
    void*                   anim_ctrl_ptr = PNULL;
    MMI_CONTROL_CREATE_T    create = {0};
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    GUIANIM_INIT_DATA_T     init_data = {0};
    
    if ( 0 == anim_id  )
    {
        return FALSE;
    }

    if ( PNULL == ( win_info_ptr = MMIPUB_GetWinAddDataPtr( win_handle ) ) )
    {
        return FALSE;
    }

    create.ctrl_id = ctrl_id;
    create.guid    = SPRD_GUI_ANIM_ID;
    create.parent_win_handle = win_handle;
    create.init_data_ptr = &init_data;
    
    anim_ctrl_ptr = MMK_CreateControl( &create );

    control_info.is_ctrl_id = FALSE;
    //control_info.ctrl_id = ctrl_id;
    control_info.ctrl_ptr = anim_ctrl_ptr;
    
    data_info.anim_id = anim_id;
    
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    if (!win_info_ptr->pubwin_theme.is_support_anim)
    {
        display_info.is_bg_buf = TRUE;
    }

    //set anim parameter
    GUIANIM_SetParam(&control_info,&data_info,PNULL,&display_info);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : set pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetPubWinText(
                         MMIPUB_INFO_T   *win_info_ptr,
                         MMI_STRING_T    **string_array,
                         uint16          string_num,
                         BOOLEAN         is_destroy
                         )
{
    uint32 i = 0;

    if ( PNULL != string_array && string_num > 0 )
    {
        for ( i = 0; i < string_num; i++ )
        {
            SetPubWinTextByIndex( win_info_ptr, string_array[i], i );
        }

        for ( i = string_num; i < MMIPUB_TEXT_MAX; i++ )
        {
            SetPubWinTextByIndex( win_info_ptr, PNULL, i );
        }
        
        ParsePubWinDisplayText( win_info_ptr );

#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
        if ( !is_destroy )
        {
            MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );

            if ( 0 == ctrl_handle )
            {
                MMI_STRING_T string = {0};

                CreatePubWinDisplayText( win_info_ptr, &string );

                CreatTextBoxCtrl( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID, &string );

                DestroyPubWinDisplayText( &string );
            }
            else
            {
                //SetPubWinTextBoxRect( win_info_ptr );
                ResetPubWinTextBoxString( win_info_ptr );
            }
        }
#endif
    }
}

/*****************************************************************************/
//  Description : set pubwin text by index
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetPubWinTextByIndex(
                                MMIPUB_INFO_T   *win_info_ptr,
                                MMI_STRING_T    *text_str_ptr,
                                uint16          index
                                )
{
    if ( index < MMIPUB_TEXT_MAX )
    {
        if ( PNULL != win_info_ptr->string[index].wstr_ptr )
        {
            SCI_FREE( win_info_ptr->string[index].wstr_ptr );
            win_info_ptr->string[index].wstr_ptr = PNULL;
            win_info_ptr->string[index].wstr_len = 0;
        }
        
        if ( PNULL != text_str_ptr 
            && PNULL != text_str_ptr->wstr_ptr
            && 0 != text_str_ptr->wstr_len )
        {
            win_info_ptr->string[index].wstr_ptr = SCI_ALLOC_APP( ( text_str_ptr->wstr_len + 1 ) * sizeof(wchar) );
            
            SCI_MEMSET( win_info_ptr->string[index].wstr_ptr, 0, (( text_str_ptr->wstr_len + 1 ) * sizeof(wchar) ));
            
            MMIAPICOM_Wstrncpy( win_info_ptr->string[index].wstr_ptr, text_str_ptr->wstr_ptr, text_str_ptr->wstr_len );
            
            win_info_ptr->string[index].wstr_len = text_str_ptr->wstr_len;
        }
        
        win_info_ptr->is_layout = FALSE;
    }
}

/*****************************************************************************/
//  Description : reset pubwin textbox string
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void ResetPubWinTextBoxString(
                                    MMIPUB_INFO_T *win_info_ptr
                                    )
{
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );

    if ( 0 != ctrl_handle )
    {
        MMI_STRING_T string = {0};
        
        CreatePubWinDisplayText( win_info_ptr, &string );
        
        GUITEXT_SetString( ctrl_handle, string.wstr_ptr, string.wstr_len, FALSE );
        
        DestroyPubWinDisplayText( &string );
    }
}

/*****************************************************************************/
//  Description : parse pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void ParsePubWinDisplayText(
                                  MMIPUB_INFO_T *win_info_ptr
                                  )
{
    MMI_STRING_T string = {0};
    uint16       width = 0;
    uint16       text_margin_space = 0;
    uint16       char_space = 0;
    GUI_FONT_T   text_font = 0;
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    GUI_RECT_T   full_rect = {0};
    uint16     alert_width  = 0;
    uint16     alert_height = 0;
    uint16     frame_num = 0;

    // 与button对齐
    text_margin_space = win_info_ptr->pubwin_theme.layout.button_margin;
    text_font = win_info_ptr->pubwin_theme.small_font;

    full_rect = GetFullRect( win_info_ptr );
    width = full_rect.right - full_rect.left + 1;

    if ( MMIPUBWIN_WAITING == win_info_ptr->type )
    {
        if ( 0 == win_info_ptr->wait_anim_id )
        {
            //alert image
            GUIRES_GetImgWidthHeight(
                    &alert_width, &alert_height,
                    win_info_ptr->prompt_image_id,
                    win_info_ptr->win_handle );
        }
        else
        {
            //alert anim
			GUIRES_GetAnimWidthHeight(
			    &alert_width, &alert_height, &frame_num,
			    win_info_ptr->wait_anim_id, win_info_ptr->win_handle );
		}

        width -= alert_width + win_info_ptr->pubwin_theme.layout.title_image_text_space;
        text_margin_space = win_info_ptr->pubwin_theme.layout.client_h_margin;
    }
    else
    {
        //横屏下宽度还需要小一点
        if ( MMK_IsWindowLandscape( win_info_ptr->win_handle )
#if defined(GUIF_EDIT_PDASTYLE) && defined(MMI_IM_PDA_SUPPORT)
            && 0 == MMK_GetCtrlHandleByGuid( win_info_ptr->win_handle, SPRD_GUI_IM_ID )
#endif  //#if defined(GUIF_EDIT_PDASTYLE) && defined(MMI_IM_PDA_SUPPORT)
            && width > (text_margin_space + win_info_ptr->pubwin_theme.layout.win_h_add_margin) * 2 )
        {
            text_margin_space += win_info_ptr->pubwin_theme.layout.win_h_add_margin;
        }
    }
#else
    text_margin_space = win_info_ptr->pubwin_theme.layout.client_h_margin;
    text_font = win_info_ptr->pubwin_theme.big_font;
    GUIRES_GetImgWidthHeight( &width, PNULL, win_info_ptr->pubwin_theme.bg_id, win_info_ptr->win_handle );
#endif

    if ( CreatePubWinDisplayText( win_info_ptr, &string ) )
    {
        MMITHEME_TEXT_T text_theme = {0};
        MMITHEME_GetTextTheme(&text_theme);
        char_space = text_theme.char_space;

        win_info_ptr->line_num = GUI_CalculateStringLinesByPixelNum(
            width - text_margin_space * 2, 
            string.wstr_ptr, string.wstr_len,
            text_font, char_space, TRUE ) + 1;
        
        if ( 0 == win_info_ptr->line_num )
        {
            win_info_ptr->line_num = 1;
        }
        
        DestroyPubWinDisplayText( &string );
    }
}

/*****************************************************************************/
//  Description : create pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePubWinDisplayText(
                                      MMIPUB_INFO_T *win_info_ptr,
                                      MMI_STRING_T  *string_ptr
                                      )
{
    BOOLEAN result = FALSE;

    if ( PNULL != string_ptr )
    {
        uint32 i = 0;
        uint32 start = 0;
        uint32 last_index = 0;
        
        string_ptr->wstr_len = 0;
        
        for ( i = 0; i < MMIPUB_TEXT_MAX; i++ )
        {
            if ( PNULL != win_info_ptr->string[i].wstr_ptr
                && 0 != win_info_ptr->string[i].wstr_len )
            {
                string_ptr->wstr_len += win_info_ptr->string[i].wstr_len;
                last_index = i;
            }
        }
        
        if ( string_ptr->wstr_len > 0 )
        {
            string_ptr->wstr_ptr = SCI_ALLOC_APP( (string_ptr->wstr_len + MMIPUB_TEXT_MAX) * sizeof(wchar) );
            SCI_MEMSET( string_ptr->wstr_ptr, 0, (string_ptr->wstr_len + MMIPUB_TEXT_MAX) * sizeof(wchar) );
            
            for ( i = 0; i < MMIPUB_TEXT_MAX; i++ )
            {
                if ( PNULL != win_info_ptr->string[i].wstr_ptr
                    && 0 != win_info_ptr->string[i].wstr_len )
                {
                    MMIAPICOM_Wstrncpy(
                            string_ptr->wstr_ptr + start,
                            win_info_ptr->string[i].wstr_ptr,
                            win_info_ptr->string[i].wstr_len);
                    
                    start += win_info_ptr->string[i].wstr_len;
                    
                    if ( i != last_index )
                    {
                        string_ptr->wstr_ptr[start] = 0x0a;
                        string_ptr->wstr_len++;
                        start++;
                    }
                }
            }

            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : destroy pubwin text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void DestroyPubWinDisplayText(
                                    MMI_STRING_T  *string_ptr
                                    )
{
    if ( PNULL != string_ptr )
    {
        SCI_FREE( string_ptr->wstr_ptr );
        string_ptr->wstr_ptr = PNULL;
        string_ptr->wstr_len = 0;
    }
}

/*****************************************************************************/
//  Description : set pubwin title text
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetPubWinTitleText(
                              MMIPUB_INFO_T   *win_info_ptr,
                              MMI_STRING_T    *text_str_ptr
                              )
{
    if ( PNULL != win_info_ptr->title_string.wstr_ptr )
    {
        SCI_FREE( win_info_ptr->title_string.wstr_ptr );
        win_info_ptr->title_string.wstr_ptr = PNULL;
        win_info_ptr->title_string.wstr_len = 0;
    }
    
    if ( PNULL != text_str_ptr 
        && PNULL != text_str_ptr->wstr_ptr
        && 0 != text_str_ptr->wstr_len )
    {
        win_info_ptr->title_string.wstr_ptr = SCI_ALLOC_APP( ( text_str_ptr->wstr_len + 1 ) * sizeof(wchar) );
        
        SCI_MEMSET( win_info_ptr->title_string.wstr_ptr, 0, (( text_str_ptr->wstr_len + 1 ) * sizeof(wchar) ));
        
        MMIAPICOM_Wstrncpy( win_info_ptr->title_string.wstr_ptr, text_str_ptr->wstr_ptr, text_str_ptr->wstr_len );
        
        win_info_ptr->title_string.wstr_len = text_str_ptr->wstr_len;
    }

    win_info_ptr->is_layout = FALSE;
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:是否横屏输入法
/*****************************************************************************/
LOCAL BOOLEAN IsImLandscape(
                            MMIPUB_INFO_T *win_info_ptr
                            )
{
#if defined(GUIF_EDIT_PDASTYLE) && defined(MMI_IM_PDA_SUPPORT)
    return (BOOLEAN)( MMK_IsWindowLandscape( win_info_ptr->win_handle ) 
        && MMK_GetCtrlHandleByGuid( win_info_ptr->win_handle, SPRD_GUI_IM_ID ) );
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Description : set pubwin anim rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 此函数依赖于SetPubWinText的计算行数
/*****************************************************************************/
LOCAL void SetPubWinAnimRect(
                             MMIPUB_INFO_T *win_info_ptr
                             )
{
    
    MMI_HANDLE_T anim_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_WAIT_ANIM_CTRL_ID );
    
    GUIANIM_SetCtrlRect( anim_handle, &win_info_ptr->alert_rect );
}

/*****************************************************************************/
//  Description : set pubwin textbox rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 此函数依赖于SetPubWinText的计算行数
/*****************************************************************************/
LOCAL void SetPubWinTextBoxRect(
                                MMIPUB_INFO_T *win_info_ptr
                                )
{
    MMI_HANDLE_T ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );
    
    GUI_RECT_T text_rect = win_info_ptr->client_rect;

    text_rect.top    -= win_info_ptr->pubwin_theme.layout.client_v_margin;
    text_rect.bottom += win_info_ptr->pubwin_theme.layout.client_v_margin;

    GUITEXT_SetRect( ctrl_handle, &text_rect );
}

/*****************************************************************************/
//  Description : get full bg rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetFullRect(
                             MMIPUB_INFO_T   *win_info_ptr
                             )
{
    GUI_BOTH_RECT_T full_rect = MMITHEME_GetFullScreenBothRect();

    return MMK_IsWindowLandscape( win_info_ptr->win_handle ) ? full_rect.h_rect : full_rect.v_rect;
}

/*****************************************************************************/
//  Description : set need update pre win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:短信等窗口需要提醒前一个窗口刷新num icon
/*****************************************************************************/
LOCAL void SetNeedPaintPreWin(
                              MMIPUB_INFO_T *win_info_ptr,
                              MMI_IMAGE_ID_T image_id
                              )
{
    if ( PNULL != win_info_ptr )
    {
        if ( IMAGE_PUBWIN_NEWCALL == image_id
            || IMAGE_PUBWIN_NEWMSG == image_id )
        {
            MMI_WIN_ID_T pre_win_id = MMK_GetPrevWinId( win_info_ptr->win_handle );
            
            if ( MAIN_IDLE_WIN_ID == pre_win_id
                || MAIN_MAINMENU_WIN_ID == pre_win_id )
            {
                MMK_SetWinDisplayStyleState( win_info_ptr->win_handle, WS_NEED_PAINT_PRE_WIN, TRUE );
            }
        }
    }
}

/*****************************************************************************/
//  Description : Set Pub window default title
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void SetPubWinDefaultTitle(
                                 MMIPUB_INFO_T *win_info_ptr
                                 )
{
    if ( PNULL != win_info_ptr)
    {
        MMI_TEXT_ID_T text_id = TXT_WARNING;
        MMI_STRING_T  text_str = {0};
        
        switch(win_info_ptr->prompt_image_id )
        {
        case IMAGE_PUBWIN_SUCCESS:
            text_id = TXT_SUCCESS;
            break;
        case IMAGE_PUBWIN_NEWCALL: // liuzhe
            text_id = STXT_NOTIFICATION;
            break;
        case IMAGE_PUBWIN_WAIT:
        case IMAGE_COMMON_PROCESS_GROOVE_1:
            text_id = TXT_COMMON_WAITING;
            break;
            
        default:
            text_id = TXT_WARNING;
            break;
        }

        MMI_GetLabelTextByLang( text_id, &text_str );
        SetPubWinTitleText( win_info_ptr, &text_str );
    }
}

/*****************************************************************************/
//  Description : get softkey text id
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetSoftkeyTextId(
                            MMI_TEXT_ID_T   *softkey_id,
                            MMIPUB_INFO_T   *win_info_ptr   //in
                            )
{
    BOOLEAN         result = TRUE;
    BOOLEAN         is_null_softkey = TRUE;
    uint16          i = 0;

    if (PNULL == win_info_ptr || PNULL == softkey_id)
    {
        return FALSE;
    }

    //init softkey id
    for (i=0; i<GUISOFTKEY_BUTTON_NUM; i++)
    {
        softkey_id[i] = TXT_NULL;
    }

    switch (win_info_ptr->softkey_style)
    {
    case MMIPUB_SOFTKEY_NONE:
		softkey_id[0] = PNULL;
        softkey_id[1] = PNULL;
        softkey_id[2] = PNULL;
	    win_info_ptr->softkey_id[0] = PNULL;
		win_info_ptr->softkey_id[1] = PNULL;  
        win_info_ptr->softkey_id[2] = PNULL;
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,PNULL,0,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,PNULL,1,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,PNULL,2,TRUE);
        break;

    case MMIPUB_SOFTKEY_ONE:
        softkey_id[2] = STXT_EXIT;
        is_null_softkey = FALSE;
        break;

    case MMIPUB_SOFTKEY_OKEXIT:
        softkey_id[0] = TXT_COMMON_OK;
        softkey_id[2] = STXT_EXIT;
        is_null_softkey = FALSE;
        break;

    case MMIPUB_SOFTKEY_OKCANCEL:
        softkey_id[0] = TXT_COMMON_OK;
        softkey_id[2] = STXT_CANCEL;
        win_info_ptr->softkey_id[0] = TXT_COMMON_OK;
	    win_info_ptr->softkey_id[1] = PNULL;  
        win_info_ptr->softkey_id[2] = STXT_CANCEL;
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,TXT_COMMON_OK,0,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,PNULL,1,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,STXT_CANCEL,2,TRUE);
        is_null_softkey = FALSE;
        break;

    case MMIPUB_SOFTKEY_CUSTOMER:
        softkey_id[0] = win_info_ptr->softkey_id[0];
        softkey_id[1] = win_info_ptr->softkey_id[1];
        softkey_id[2] = win_info_ptr->softkey_id[2];
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,softkey_id[0],0,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,softkey_id[1],1,TRUE);
        GUIWIN_SeSoftkeytButtonTextId(win_info_ptr->win_handle,softkey_id[2],2,TRUE);
        is_null_softkey = FALSE;
        break;

    case MMIPUB_SOFTKEY_DIALOG_DISMISS:
        softkey_id[1] = text_softkey_ok_mk;
        is_null_softkey = FALSE;
        break;

    case MMIPUB_SOFTKEY_DIALOG_ALARM:
        softkey_id[0] = text_softkey_open_lsk;
        softkey_id[1] = text_softkey_sleep_mk;
        softkey_id[2] = text_softkey_stop_rsk;
        is_null_softkey = FALSE;
        break;

    default:
        result = FALSE;
        break;
    }


    return (result);
}


/*****************************************************************************/
//  Description : set parent window softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetParentWinSoftkeyDispLayer(
                                        MMI_HANDLE_T win_handle
                                        )
{
#ifndef MMIPUBWIN_PDASTYLE_SUPPORT	//pda大多数场景不显示softkey
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_handle);

    if ( PNULL != win_info_ptr )
    {
        MMI_HANDLE_T    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMK_GetWinSoftkeyCtrlId(win_handle) );
        IGUICTRL_T      *ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
        GUI_LCD_DEV_INFO lcd_dev_info = GetPubWinFullBgLayer(win_info_ptr);
        
        if ( PNULL != ctrl_ptr )
        {
            GUICTRL_SetLcdDevInfo(ctrl_ptr, &lcd_dev_info);
        }
    }
#endif
}

/*****************************************************************************/
//  Description : is exist softkey
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsExistSoftkey(
                             MMIPUB_INFO_T *win_info_ptr
                             )
{
    if ( PNULL != win_info_ptr )
    {
        MMI_TEXT_ID_T softkey_id[GUISOFTKEY_BUTTON_NUM] = {0};
        uint32 i = 0;

        GetSoftkeyTextId( softkey_id, win_info_ptr);

        for ( i = 0; i < GUISOFTKEY_BUTTON_NUM; i++ )
        {
            if ( 0 != softkey_id[i]
                && TXT_NULL != softkey_id[i] )
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : set parent window softkey
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void SetWinSoftkey(
                         MMI_WIN_ID_T win_id
                         )
{

    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if ( PNULL != win_info_ptr )
    {
        MMI_TEXT_ID_T   softkey_id[GUISOFTKEY_BUTTON_NUM] = {0};

        GetSoftkeyTextId( softkey_id, (MMIPUB_INFO_T*)win_info_ptr);
//fixed by hongjun.jia for bugzilla 2443/2444
#if defined MMIPUBWIN_PDASTYLE_SUPPORT || defined MAINLCD_SIZE_176X220
        if (MMIPUBWIN_WAITING == win_info_ptr->type)
        {        
            GUIWIN_SetSoftkeyTextId( win_id,  softkey_id[LEFT_BUTTON], softkey_id[MIDDLE_BUTTON], softkey_id[RIGHT_BUTTON], FALSE );
        }
        else
        {
            win_info_ptr->softkey_id[LEFT_BUTTON] = softkey_id[LEFT_BUTTON];
            win_info_ptr->softkey_id[MIDDLE_BUTTON] = softkey_id[MIDDLE_BUTTON];
            win_info_ptr->softkey_id[RIGHT_BUTTON] = softkey_id[RIGHT_BUTTON];
        }

#else
        GUIWIN_SetSoftkeyTextId( win_id,  softkey_id[LEFT_BUTTON], softkey_id[MIDDLE_BUTTON], softkey_id[RIGHT_BUTTON], FALSE );
#endif
    }
}

/*****************************************************************************/
//  Description : create move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatePubWinLayer(
                                MMIPUB_INFO_T    *win_info_ptr,  //in
                                GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                GUI_RECT_T       bg_rect,
                                BOOLEAN          is_mask_layer
                                )
{   
    BOOLEAN result = FALSE;
#ifdef UI_MULTILAYER_SUPPORT
    UILAYER_CREATE_T    create_info = {0};

    if ( win_info_ptr->pubwin_theme.is_support_anim 
        && !GUI_IsInvalidRect(bg_rect)
        && PNULL != lcd_dev_ptr )
    {
        if ( 0 == lcd_dev_ptr->block_id
            || UILAYER_NULL_HANDLE == lcd_dev_ptr->block_id )
        {
            int32 offset = ( is_mask_layer ) ? 0 : MMITHEME_ZOOM_MEM_ALIGN;

            // 创建层
            create_info.lcd_id = GUI_MAIN_LCD_ID;
            create_info.owner_handle = win_info_ptr->win_handle;
//             create_info.offset_x = bg_rect.left;
//             create_info.offset_y = bg_rect.top;
            create_info.width = (bg_rect.right + 1 - bg_rect.left);
            create_info.height = (bg_rect.bottom + 1 - bg_rect.top + offset * 2);
            create_info.is_bg_layer = FALSE;
            create_info.is_static_layer = FALSE;
            create_info.format = is_mask_layer ? UILAYER_MEM_IMMUTABLE : UILAYER_MEM_FPS_ACCELERATE;
            
            if (UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, lcd_dev_ptr))
            {
                if (UILAYER_IsLayerActive(lcd_dev_ptr)) // 判断多层是否有效
                {
                    // 设置color key
                    UILAYER_SetLayerColorKey(lcd_dev_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);
                    // 使用color清除层
                    UILAYER_Clear(lcd_dev_ptr);
                    
                    UILAYER_SetLayerPosition(lcd_dev_ptr, bg_rect.left, bg_rect.top - offset);
                    
                    result = TRUE;
                }
            }
        }
        else
        {
            if (UILAYER_IsLayerActive(lcd_dev_ptr))
            {
                UILAYER_Clear(lcd_dev_ptr);
                result = TRUE;
            }
        }
    }
    
    if (!result) // 申请多层失败，不支持动画
    {
        win_info_ptr->pubwin_theme.is_support_anim = FALSE;
    }
#endif
    return result;
}

/*****************************************************************************/
//  Description : release move layer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void ReleasePubWinLayer(
                              GUI_LCD_DEV_INFO *lcd_dev_info_ptr
                              )
{
#ifdef UI_MULTILAYER_SUPPORT
    // 释放层
    if (UILAYER_IsMultiLayerEnable()
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        UILAYER_RELEASELAYER(lcd_dev_info_ptr);   /*lint !e506 !e774*/
//        UILAYER_ReleaseLayer(lcd_dev_info_ptr);
//        lcd_dev_info_ptr->lcd_id = GUI_MAIN_LCD_ID;
//        lcd_dev_info_ptr->block_id = UILAYER_NULL_HANDLE;
    }
#endif
    return;
}

/*****************************************************************************/
//  Description : 合并层
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void AppendPubWinLayer(
                             GUI_LCD_DEV_INFO lcd_dev_info
                             )
{
    UILAYER_APPEND_BLT_T append_layer = {0};
#ifdef UI_MULTILAYER_SUPPORT    
    if (UILAYER_IsMultiLayerEnable() 
        && UILAYER_IsLayerActive(&lcd_dev_info)
        && 0 != lcd_dev_info.block_id)
    {
        append_layer.lcd_dev_info = lcd_dev_info;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;
        UILAYER_AppendBltLayer(&append_layer);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence : 
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetPubWinLayer(
                                      MMIPUB_INFO_T    *win_info_ptr
                                      )
{
    GUI_LCD_DEV_INFO lcd_dev_info = win_info_ptr->pubwin_theme.bg_lcd_dev;
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable() 
        && UILAYER_IsLayerActive(&win_info_ptr->pubwin_bg_dev))
    {
        lcd_dev_info = win_info_ptr->pubwin_bg_dev;
    }
#endif
    return lcd_dev_info;
}

#ifndef MMIPUBWIN_PDASTYLE_SUPPORT	//pda大多数场景不显示softkey
/*****************************************************************************/
//  Description : 获取当前层信息
//  Global resource dependence : 
//  Author: xiaoqing
//  Note:
/*****************************************************************************/
LOCAL GUI_LCD_DEV_INFO GetPubWinFullBgLayer(
                                            MMIPUB_INFO_T    *win_info_ptr
                                            )
{
    GUI_LCD_DEV_INFO lcd_dev_info = win_info_ptr->pubwin_theme.bg_lcd_dev;
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_IsMultiLayerEnable() 
        && UILAYER_IsLayerActive(&win_info_ptr->pubwin_mask_dev))
    {
        lcd_dev_info = win_info_ptr->pubwin_mask_dev;
    }
#endif
    return lcd_dev_info;
}
#endif

/*****************************************************************************/
//  Description : display public anim
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetInlineCtrlDispLayer(
                                  MMIPUB_INFO_T    *win_info_ptr,
                                  GUI_LCD_DEV_INFO lcd_dev_info
                                  )
{
    MMI_HANDLE_T ctrl_handle = 0;
    //IGUICTRL_T* ctrl_ptr = PNULL;
    
    // 设置动画控件的层
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_WAIT_ANIM_CTRL_ID );
    
    MMK_SetCtrlLcdDevInfo(ctrl_handle, &lcd_dev_info);
    
    // 设置text控件层
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );
    
    MMK_SetCtrlLcdDevInfo(ctrl_handle, &lcd_dev_info);

#if defined MMIPUBWIN_PDASTYLE_SUPPORT || defined MAINLCD_SIZE_176X220
    // 设置button控件的层
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_LEFT_BUTTON_CTRL_ID );
    
    MMK_SetCtrlLcdDevInfo(ctrl_handle, &lcd_dev_info);
    
    // 设置button控件的层
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_MIDDLE_BUTTON_CTRL_ID );
    
    MMK_SetCtrlLcdDevInfo(ctrl_handle, &lcd_dev_info);
    
    
    // 设置button控件的层
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_RIGHT_BUTTON_CTRL_ID );
    
    MMK_SetCtrlLcdDevInfo(ctrl_handle, &lcd_dev_info);
    
    // 设置控件的层
    MMK_SetCtrlLcdDevInfo(win_info_ptr->ctrl_handle, &lcd_dev_info);

    if ( MMIPUBWIN_FORM == win_info_ptr->type )
    {
        //特殊处理,设置输入法的lcd dev
        MMI_HANDLE_T im_handle = MMK_GetCtrlHandleByGuid( win_info_ptr->win_handle, SPRD_GUI_IM_ID );

        MMK_SetCtrlLcdDevInfo( im_handle, &win_info_ptr->pubwin_mask_dev );
    }
#endif
}

/*****************************************************************************/
//  Description : display public anim
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void UpdateInlineCtrl(
                            MMIPUB_INFO_T    *win_info_ptr
                            )
{
    
    MMI_HANDLE_T ctrl_handle = 0;
    IGUICTRL_T* ctrl_ptr = PNULL;
    
    // 刷新动画控件
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_WAIT_ANIM_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_HandleEvent( ctrl_ptr, MSG_CTL_PAINT, PNULL );
    }
    
    // 刷新text控件
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_HandleEvent( ctrl_ptr, MSG_CTL_PAINT, PNULL );
    }

#if defined MMIPUBWIN_PDASTYLE_SUPPORT || defined MAINLCD_SIZE_176X220
    // 刷新button控件
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_LEFT_BUTTON_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_HandleEvent( ctrl_ptr, MSG_CTL_PAINT, PNULL );
    }
    
    // 刷新button控件
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_MIDDLE_BUTTON_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_HandleEvent( ctrl_ptr, MSG_CTL_PAINT, PNULL );
    }

    // 刷新button控件
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_RIGHT_BUTTON_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_HandleEvent( ctrl_ptr, MSG_CTL_PAINT, PNULL );
    }

    // 刷新控件
    ctrl_handle = win_info_ptr->ctrl_handle;
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );

    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_HandleEvent( ctrl_ptr, MSG_CTL_PAINT, PNULL );
        
        if ( MMIPUBWIN_FORM == win_info_ptr->type )
        {
            MMK_DrawAllChildCtrl( ctrl_handle );
        }
    }
#endif
}

/*****************************************************************************/
//  Description : display public anim
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void SetInlineCtrlInvisible(
                                  MMIPUB_INFO_T    *win_info_ptr,
                                  BOOLEAN is_true
                                  )
{
    
    MMI_HANDLE_T ctrl_handle = 0;
    IGUICTRL_T* ctrl_ptr = PNULL;

    // 设置动画控件可见
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_WAIT_ANIM_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_SetState((IGUICTRL_T *)ctrl_ptr,GUICTRL_STATE_INVISIBLE, is_true);
    }
    
    // 设置text控件可见
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_TEXT_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_SetState((IGUICTRL_T *)ctrl_ptr,GUICTRL_STATE_INVISIBLE, is_true);
    }

#if defined MMIPUBWIN_PDASTYLE_SUPPORT || defined MAINLCD_SIZE_176X220
    // 设置button控件可见
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_LEFT_BUTTON_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_SetState((IGUICTRL_T *)ctrl_ptr,GUICTRL_STATE_INVISIBLE, is_true);
    }
    
    // 设置button控件可见
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_MIDDLE_BUTTON_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_SetState((IGUICTRL_T *)ctrl_ptr,GUICTRL_STATE_INVISIBLE, is_true);
    }

    // 设置button控件可见
    ctrl_handle = MMK_GetCtrlHandleByWin( win_info_ptr->win_handle, MMIPUB_RIGHT_BUTTON_CTRL_ID );
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        IGUICTRL_SetState((IGUICTRL_T *)ctrl_ptr,GUICTRL_STATE_INVISIBLE, is_true);
    }

    // 设置控件可见
    ctrl_handle = win_info_ptr->ctrl_handle;
    ctrl_ptr = MMK_GetCtrlPtr( ctrl_handle );
    
    if ( PNULL != ctrl_ptr )
    {
        if ( MMIPUBWIN_FORM == win_info_ptr->type )
        {
            GUIFORM_SetVisible( ctrl_handle, (BOOLEAN)(!is_true), FALSE );
        }
        else
        {
            IGUICTRL_SetState((IGUICTRL_T *)ctrl_ptr,GUICTRL_STATE_INVISIBLE, is_true);
        }
    }
#else
    GUIWIN_SetSoftkeyVisible(win_info_ptr->win_handle, (BOOLEAN)(!is_true));
#endif

    return;
}

/*****************************************************************************/
//  Description : clear layer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
// LOCAL void ClearPubWinLayer(
//                             GUI_LCD_DEV_INFO lcd_dev_info
//                             )
// {
//     if (UILAYER_IsMultiLayerEnable() 
//         && UILAYER_IsLayerActive(&lcd_dev_info)
//         && 0 != lcd_dev_info.block_id)
//     {
//         UILAYER_Clear(&lcd_dev_info);
//     }
// }

/*****************************************************************************/
//  Description : reset pubwin, destroy all block and stop timer etc.
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ResetPubwinInfo(
                           MMIPUB_INFO_T    *win_info_ptr
                           )
{
    //stop pubwin timer
    if (PNULL != win_info_ptr && 0 < win_info_ptr->timer_id)
    {
        MMK_StopTimer(win_info_ptr->timer_id);
        win_info_ptr->timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : update pubwin screen
//  Global resource dependence : 
//  Author: bin.ji
//  Note:
/*****************************************************************************/
LOCAL void UpdatePubwinScreen(
                              MMI_WIN_ID_T       win_id
                              )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if ((PNULL != win_info_ptr) && (win_info_ptr->pubwin_theme.is_support_anim))
    {
        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);		
    }
    else
    {
        MMK_UpdateScreen();
    }
}

/*****************************************************************************/
//    Description : 创建提示信息Layer
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN CreatPromptTextLayer(
                                   MMI_WIN_ID_T win_id
                                   )
{
    UILAYER_CREATE_T     create_info = {0};
    GUI_RECT_T           layer_rect  = {0};
    BOOLEAN              result      = FALSE;
#ifdef UI_MULTILAYER_SUPPORT
    if (UILAYER_HANDLE_MULTI != UILAYER_GetHandleType(&s_pub_tips_layer_info))
    {
        MMITHEME_GetTipsStrInfo(&layer_rect , PNULL, PNULL, PNULL);
        
        //creat text layer
        create_info.lcd_id          = GUI_MAIN_LCD_ID;
        create_info.owner_handle    = win_id;
        create_info.offset_x        = layer_rect.left;
        create_info.offset_y        = layer_rect.top - 10;
        create_info.width           = layer_rect.right;
        create_info.height          = layer_rect.bottom;
        create_info.is_bg_layer     = FALSE;
        create_info.is_static_layer = FALSE;

        if (UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_info, &s_pub_tips_layer_info))
        {
            s_pub_tips_layer_owner_handle = win_id;
            result = TRUE;
        }
    }
    else
    {
        if (s_pub_tips_layer_owner_handle == win_id)
        {
            MMIPUB_ClearTips();
            result = TRUE;
        }
        else
        {
            if (UILAYER_IsMultiLayerEnable())
            {
                UILAYER_RELEASELAYER(&s_pub_tips_layer_info);   /*lint !e506 !e774*/
//                UILAYER_ReleaseLayer(&s_pub_tips_layer_info);
//                s_pub_tips_layer_info.lcd_id   = GUI_MAIN_LCD_ID;
//                s_pub_tips_layer_info.block_id = UILAYER_NULL_HANDLE;
                s_pub_tips_layer_owner_handle  = NULL;

                CreatPromptTextLayer(win_id);
                result = TRUE;
            }

        }
    }
#endif
  return result;
}

/*****************************************************************************/
//  Description : 类型转换
//  Global resource dependence : 
//  Author: peng.chen
//  Note:
/*****************************************************************************/
LOCAL MMIPUB_SOFTKEY_STYLE_E ConvertStyleFromCafToGui(
                                                      CAF_MB_SOFTKEY_TYPE_E sk_style
                                                      )
{
    MMIPUB_SOFTKEY_STYLE_E gui_style = MMIPUB_SOFTKEY_NONE;

    switch(sk_style)
    {
    case CAF_MB_SOFTKEY_NONE:
        gui_style = MMIPUB_SOFTKEY_NONE;
        break;

    case CAF_MB_SOFTKEY_EXIT:
        gui_style = MMIPUB_SOFTKEY_ONE;
        break;

    case CAF_MB_SOFTKEY_OKEXIT:
        gui_style = MMIPUB_SOFTKEY_OKEXIT;
        break;

    case CAF_MB_SOFTKEY_OKCANCEL:
        gui_style = MMIPUB_SOFTKEY_OKCANCEL;
        break;
        
    default:
        gui_style = MMIPUB_SOFTKEY_NONE;
        break;
        
    }

    return gui_style;
}

/*****************************************************************************/
//    Description : 清除Tips Timer消息处理
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void HandleClearTipsTimerEvent(
                                     uint8 timerid,
                                     uint32 param
                                     )
{
    if (timerid == s_tips_clear_timer_id)
    {
        //MMI_WIN_ID_T win_id = (MMI_WIN_ID_T)param;

        MMIPUB_ClearTips();
    }
}

/*****************************************************************************/
//    Description : 显示提示信息字符串
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL void DisplayTipsPtr(MMI_WIN_ID_T win_id)
{

    GUISTR_STATE_T   text_state   = 0;
    GUISTR_STYLE_T   text_style   = {0};
    GUISTR_INFO_T    text_info    = {0};
    GUI_RECT_T       text_rect    = {0};
    GUI_RECT_T       img_rect     = {0};
    GUI_POINT_T      start_point  = {0};
    uint16           img_width    = 0;
    uint16           img_height   = 0;
    GUI_LCD_DEV_INFO lcd_dev_info = {0};
    MMI_IMAGE_ID_T   bg_img_id    = 0;
    
    if (PNULL == s_dis_text.wstr_ptr)
    {
        return;
    }

    if (s_dis_text.wstr_len > 0)
    {
        uint16 rect_width  = 0;
        uint16 rect_height = 0;

        MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
        
        MMITHEME_GetTipsStrInfo(PNULL, &text_style, &text_state, &bg_img_id);
        
        GUISTR_GetStringInfo(&text_style, &s_dis_text, text_state, &text_info);
        
        text_rect = GetTipsDisRect(s_cur_tips_dis_style);
        
        rect_width = text_rect.right - text_rect.left;
        rect_height = text_rect.bottom - text_rect.top;
        
        if (text_info.width > rect_width)
        {
            text_info.width = rect_width;
        }
        
        text_rect.bottom = text_rect.top + text_info.height;
        text_rect.left   = (rect_width - text_info.width) / 2;
        text_rect.right  = text_rect.left + text_info.width;
        
        (void)GUIRES_GetImgWidthHeight(&img_width, &img_height, bg_img_id, win_id);
        
        start_point.x   = text_rect.left - MMIPUBWIN_TIPS_BG_SIDE_LEN;
        start_point.y    = text_rect.top - (img_height - text_info.height) / 2;
        img_rect.bottom = img_height;
        img_rect.right  = MMIPUBWIN_TIPS_BG_SIDE_LEN - 1;
        GUIRES_DisplayImg(&start_point, PNULL, &img_rect, win_id, bg_img_id, &lcd_dev_info);
        
        start_point.x = text_rect.left;
        img_rect.left   = img_rect.right;
        img_rect.right  = img_rect.left + text_info.width;
        GUIRES_DisplayImg(&start_point, PNULL, &img_rect, win_id, bg_img_id, &lcd_dev_info);
        start_point.x = text_rect.right + 1;
        img_rect.left   = img_width - MMIPUBWIN_TIPS_BG_SIDE_LEN + 1;
        img_rect.right  = img_width;
        GUIRES_DisplayImg(&start_point, PNULL, &img_rect, win_id, bg_img_id, &lcd_dev_info);
        
        GUISTR_DrawTextToLCDInRect(&lcd_dev_info, &text_rect, &text_rect, &s_dis_text,	&text_style, text_state, 0);
    }
}

/*****************************************************************************/
//    Description : 获取提示信息字符串 显示大区域
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL GUI_RECT_T GetTipsDisRect(MMIPUB_TIPS_DIS_STYLE_E dis_style)
{
    uint16           lcd_height         = 0;
    GUI_RECT_T       dis_rect           = {0};

    if (dis_style >= MMIPUB_TIPS_DIS_MAX)
    {
        //SCI_TRACE_LOW:"GetTipsDisRect, cur dis_style error, dis_style = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_PUBWIN_5659_112_2_18_1_58_51_67,(uint8*)"d", dis_style);
        dis_style = MMIPUB_TIPS_DIS_BOTTOM;
    }

    dis_rect = MMITHEME_GetFullScreenRect();

    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, PNULL, &lcd_height);

    switch(dis_style)
    {
    case MMIPUB_TIPS_DIS_TOP:
        {
            dis_rect.top = lcd_height * 1 / 4;
        }
        break;    
    case MMIPUB_TIPS_DIS_MIDDLE:
        {
            dis_rect.top = lcd_height * 2 / 4;
        }
    	break;
    case MMIPUB_TIPS_DIS_BOTTOM:
        {
            dis_rect.top = lcd_height * 3 / 4;
        }
        break;
    default:
        {
            dis_rect.top = lcd_height * 3 / 4;
        }
        break;
    }

    dis_rect.bottom = dis_rect.top + MMIPUB_TIPS_DIS_HEIGHT;
    
    return dis_rect;
}

/*****************************************************************************/
//    Description : 需要分发给前一个窗口处理的消息
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
LOCAL BOOLEAN MsgIsNeedSendToPreWin(MMI_MESSAGE_ID_E  msg_id)
{
    BOOLEAN result = FALSE;

    switch(msg_id)
    {
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
    case MSG_TP_PRESS_UP:
    case MSG_TP_PRESS_MOVE:
    case MSG_TP_PRESS_SHORT:
    case MSG_TP_PRESS_DOUBLE:
    case MSG_TP_PRESS_LONG:
    case MSG_TP_PRESS_DRAG:
    case MSG_TP_PRESS_DBLCLK_UP:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
    case MSG_APP_1:
    case MSG_APP_2:
    case MSG_APP_3:
    case MSG_APP_4:
    case MSG_APP_5:
    case MSG_APP_6:
    case MSG_APP_7:
    case MSG_APP_8:
    case MSG_APP_9:
    case MSG_APP_0:
    case MSG_APP_UP:
    case MSG_APP_DOWN:
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_APP_GREEN:
    case MSG_APP_CAMERA:
    case MSG_APP_HASH:
    case MSG_APP_STAR:
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
    //case MSG_APP_RED://这个case会导致assert，发送消息到前一个窗口，前一窗口是idle，是lose focus状态，层还没有创建...@juan.wu
    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE:
    case MSG_APP_WEB:
    case MSG_APP_FLIP:
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    case MSG_APP_POWER:
    case MSG_APP_MENU:
#endif        
        result = TRUE;
        break;
    default:
        result = FALSE;
        break;
    }

    return result;     
}


//fixed by hongjun.jia for bugzilla 2443/2444
#if defined MMIPUBWIN_PDASTYLE_SUPPORT || defined MAINLCD_SIZE_176X220
/*****************************************************************************/
//  Description : get pubwin top height
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetPubWinTopBGHeight(
                                  MMIPUB_INFO_T *win_info_ptr
                                  )
{
    uint16 height = 0;

    GUIRES_GetImgWidthHeight(
            PNULL, &height,
            win_info_ptr->pubwin_theme.layout.top_bg_id,
            win_info_ptr->win_handle );

    return height;
}

/*****************************************************************************/
//  Description : get pubwin bottom height
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetPubWinBottomBGHeight(
                                     MMIPUB_INFO_T *win_info_ptr,
                                     BOOLEAN        is_exist_skb
                                     )
{
    uint16 height = 0;
    MMI_IMAGE_ID_T image_id = 0;

    if (is_exist_skb)
    {
        image_id = win_info_ptr->pubwin_theme.layout.bottom_bg_id;
    }
    else
    {
        image_id = win_info_ptr->pubwin_theme.layout.bottom_bg_without_button_id;
    }

    GUIRES_GetImgWidthHeight(PNULL, &height, image_id, win_info_ptr->win_handle);

    return height;
}

/*****************************************************************************/
//  Description : get pubwin button rect
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
LOCAL void GetPubWinButtonRect(
                               MMIPUB_INFO_T *win_info_ptr,
                               GUI_RECT_T    rect_ptr[GUISOFTKEY_BUTTON_NUM],
                               uint32        num
                               )
{
    GUI_RECT_T bg_rect       = {0};
    uint16     button_width  = 0;
    uint16     button_height = 0;
    uint16     bottom_height = 0;
    uint16     h_margin      = 0;
    uint16     v_margin      = 0;
    uint16     h_num = 0;
    uint16     v_num = 0;
    uint16     h_space = 0;
    uint32     i = 0;

    if ( !win_info_ptr->is_layout )
    {
        return;
    }

    bg_rect = win_info_ptr->bg_rect;
    GUIRES_GetImgWidthHeight(
            PNULL, &button_height,
            win_info_ptr->pubwin_theme.layout.button_img_id,
            win_info_ptr->win_handle );
    
    if ( IsImLandscape( win_info_ptr ) )
    {
        bg_rect.top = MMI_STATUSBAR_HEIGHT;
        button_width = (bg_rect.right - bg_rect.left)/4;
        bg_rect.left = bg_rect.right - button_width + 1;

        // TODO: error
        h_margin = v_margin = win_info_ptr->pubwin_theme.layout.client_v_margin;
        h_num = 1;
        v_num = num;
    }
    else
    {
        GUIRES_GetImgWidthHeight(
                PNULL, &bottom_height,
                win_info_ptr->pubwin_theme.layout.bottom_bg_id,
                win_info_ptr->win_handle );

        bottom_height -= win_info_ptr->pubwin_theme.layout.button_bottom_margin;

		bg_rect.left = bg_rect.left + win_info_ptr->pubwin_theme.layout.button_margin;
		bg_rect.right = bg_rect.right - win_info_ptr->pubwin_theme.layout.button_margin;
//fixed by hongjun.jia for bugzilla 2443/2444
#ifdef MAINLCD_SIZE_176X220
		bg_rect.bottom = bg_rect.bottom - win_info_ptr->pubwin_theme.layout.button_bottom_margin;
#else
		bg_rect.bottom = bg_rect.bottom - win_info_ptr->pubwin_theme.layout.button_bottom_margin - (bottom_height - button_height) / 2;
#endif
        bg_rect.top = bg_rect.bottom - button_height + 1;

        h_space = win_info_ptr->pubwin_theme.layout.button_space;

        h_num = num;
        v_num = 1;
    }

    if ( num > GUISOFTKEY_BUTTON_NUM )
    {
        num = GUISOFTKEY_BUTTON_NUM;
    }
    
    GUI_CreateMatrixRect(&bg_rect,
        h_margin, 
        v_margin, 
        h_space, 
        0, 
        v_num, 
        h_num,
        rect_ptr
        );
    
    for ( i = 0; i < num; i++ )
    {
        rect_ptr[i] = GUI_GetCenterRect( rect_ptr[i], rect_ptr[i].right - rect_ptr[i].left + 1, button_height );
    }
    
    if ( 1 == num )
    {
        rect_ptr[2] = rect_ptr[0];
    }
    else if ( 2 == num )
    {
        rect_ptr[2] = rect_ptr[1];
    }
}

#endif

/*****************************************************************************/
//  Description : clear client rect bg
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPUBClearClientRectBg(
                                    MMIPUB_INFO_T* win_info_ptr,
                                    GUI_RECT_T*    rect_ptr
                                    )
{
#ifdef MMIPUBWIN_PDASTYLE_SUPPORT
    GUI_RECT_T      cross_rect = {0};

    if ( PNULL != win_info_ptr 
        && PNULL != rect_ptr )
    {
        GUI_LCD_DEV_INFO lcd_dev_info = GetPubWinLayer( win_info_ptr );
        
        if ( IsImLandscape( win_info_ptr ) )
        {
            //get crossed rect
            if (GUI_IntersectRect(&cross_rect,win_info_ptr->client_rect,*rect_ptr))
            {
                MMITHEME_ClearRect( &lcd_dev_info, &cross_rect );
                
                 //clear text bg
                GUI_FillRect( &lcd_dev_info, cross_rect, MMIPUB_IM_BG_COLOR );
            }
            else if ( rect_ptr->left > win_info_ptr->client_rect.right )
            {
                //button
                if ( IsExistSoftkey( win_info_ptr )
                    && GUI_IntersectRect(&cross_rect,win_info_ptr->bg_rect,*rect_ptr) )
                {
                    MMITHEME_ClearRect( &lcd_dev_info, &cross_rect );

                    GUI_FillRect( &lcd_dev_info, cross_rect, MMIPUB_IM_BUTTON_BG_COLOR );
                }
            }
            else if (GUI_IntersectRect(&cross_rect,win_info_ptr->alert_rect,*rect_ptr))
            {
                if (0 != lcd_dev_info.block_id
                    && UILAYER_NULL_HANDLE != lcd_dev_info.block_id)
                {
                    GUI_RECT_T  update_img_rect = {0};

                    update_img_rect = win_info_ptr->bg_rect;
                    update_img_rect.left = rect_ptr->left - update_img_rect.left;
                    update_img_rect.right = rect_ptr->right - update_img_rect.left;
                    update_img_rect.top = rect_ptr->top - update_img_rect.top;
                    update_img_rect.bottom = rect_ptr->bottom - update_img_rect.top;
#ifdef UI_MULTILAYER_SUPPORT
                    UILAYER_ClearRect(&lcd_dev_info, *rect_ptr);
#endif
                    GUIRES_DisplayImg(PNULL,
                        rect_ptr,
                        &update_img_rect,
                        win_info_ptr->win_handle,
                        win_info_ptr->pubwin_theme.bg_id,
                        &lcd_dev_info);
                }
            }
        }
        else
        {
            //get crossed rect
            if (GUI_IntersectRect(&cross_rect,win_info_ptr->client_rect,*rect_ptr))
            {
                MMITHEME_ClearRect( &lcd_dev_info, &cross_rect );
                
                //clear text bg
                GUIRES_DisplayImg(PNULL,
                    &cross_rect,
                    &cross_rect,
                    win_info_ptr->win_handle,
                    win_info_ptr->pubwin_theme.bg_id,
                    &lcd_dev_info);
            }
            else if ( rect_ptr->top > win_info_ptr->client_rect.bottom )
            {
                //button
                if ( IsExistSoftkey( win_info_ptr )
                    && GUI_IntersectRect(&cross_rect,win_info_ptr->bg_rect,*rect_ptr) )
                {
                    GUI_RECT_T image_rect = cross_rect;
                    MMITHEME_ClearRect( &lcd_dev_info, &cross_rect );
                    
                    image_rect.top = 0;
                    
                    /*GUIRES_DisplayImg( 
                        PNULL,
                        &cross_rect,
                        &image_rect,
                        win_info_ptr->win_handle,
                        PNULL,
                        &lcd_dev_info );*/
                      //GUI_FillRect(&lcd_dev_info, image_rect, MMI_WHITE_COLOR);
                      GUI_FillRect( &lcd_dev_info, cross_rect, MMI_WHITE_COLOR );
                }
            }
            else if (GUI_IntersectRect(&cross_rect,win_info_ptr->alert_rect,*rect_ptr))
            {
                if (0 != lcd_dev_info.block_id
                    && UILAYER_NULL_HANDLE != lcd_dev_info.block_id)
                {
                    GUI_RECT_T  update_img_rect = {0};

                    update_img_rect = win_info_ptr->bg_rect;
                    update_img_rect.left = rect_ptr->left - update_img_rect.left;
                    update_img_rect.right = rect_ptr->right - update_img_rect.left;
                    update_img_rect.top = rect_ptr->top - update_img_rect.top;
                    update_img_rect.bottom = rect_ptr->bottom - update_img_rect.top;
#ifdef UI_MULTILAYER_SUPPORT
                    UILAYER_ClearRect(&lcd_dev_info, *rect_ptr);
#endif
                    GUIRES_DisplayImg(PNULL,
                        rect_ptr,
                        &update_img_rect,
                        win_info_ptr->win_handle,
                        win_info_ptr->pubwin_theme.bg_id,
                        &lcd_dev_info);
                }
            }
        }
    }
#endif    
}

/*****************************************************************************/
//  Description : set wait pubwin window is update delay
//  Global resource dependence : 
//  Author: hongjun.jia
//  Note: TRUE 代表支持延迟刷新；FALSE 代表不支持延迟刷新
/*****************************************************************************/
PUBLIC void MMIPUB_SetWaitWinIsupdatedelay(
                                 MMI_WIN_ID_T          win_id,
                                 BOOLEAN               is_updatedelay
                                 )
{
    MMIPUB_INFO_T   *win_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        win_info_ptr->is_waitwin_update_delay = is_updatedelay;
    }
}

/*****************************************************************************/
//  Description : com alert note win
//  Parameter: [In] win Id
//             [In] text_id
//  Author: qi.liu1
//  Note:
/*****************************************************************************/
PUBLIC void WatchPUB_Alert_Note_Win(MMI_WIN_ID_T    win_id, MMI_TEXT_ID_T text_id)
{
    MMI_STRING_T    noteStr = {0};
    WATCH_SOFTKEY_TEXT_ID_T softKey = {0};
    GUISTR_STYLE_T  noteStyle = {0};
    GUISTR_STATE_T  state = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE;
    noteStyle.align = ALIGN_LEFT;
    noteStyle.font = WATCH_STRING_FONT;
    MMI_GetLabelTextByLang(text_id, &noteStr);
    if(GUISTR_GetStringWidth(&noteStyle, &noteStr, state) < WATCH_NOTE_LEN_1LINE)
    {
        WatchCOM_NoteWin_1Line_Enter(win_id, &noteStr, NULL, softKey, NULL);
    }
    else
    {
        WatchCOM_NoteWin_2Line_Enter(win_id, &noteStr, NULL, softKey, NULL);
    }
}


/*Edit by script, ignore 7 case. Thu Apr 26 19:01:12 2012*/ //IGNORE9527
