/****************************************************************************
** File Name:      mmiasp_wintable.c                                        *
** Author:                                                                  *
** Date:           2008.11.26                                               *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the ANSWER PHONE           *
*****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2008.11        byte.guo          Create
** 
****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_asp_trc.h"
#include "mmiasp_wintab.h"
#include "mmiasp_cortrol_layer.h"
#include "mmiasp_export.h"
#include "mmiasp_position.h"
#include "guilabel.h"
#include "mmiasp_nv.h"
#include "guirichtext.h"
#include "mmiidle_statusbar.h"
#include "mmimultim_text.h"
#include "mmicc_text.h"
#include "cafapplet_gen.h"
#include "cafmodule_gen.h"
#include "mmk_modinfo.h"
#include "cafctrllist.h"
#include "mmifmm_export.h"
#include "guidropdownlist.h"
#include "guiform.h"
#include "window_parse.h"
#include "mmiidle_export.h" 
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmisms_export.h"
#include "mmimms_export.h"
#include "mmi_default.h" 
#include "guibutton.h"
#include "mmicom_panel.h"
#include "mmitheme_pos.h"
#include "guires.h"
#include "guiownerdraw.h"
#include "guisetlist.h"
#include "mmisd_export.h"
#include "mmiudisk_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
//#include "mmipb_text.h"
//#include "mmiset_wintab.h" 
#include "guitext.h"

#ifdef MMIWIDGET_SUPPORT
#include "mmiwidget_export.h"
#include "mmiwidget.h"
#include"mmiwidget_id.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef SCREENSAVER_SUPPORT 
#include "mmiss_export.h"
#endif

#include "mmimultim_image.h"
#include "mmk_tp.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIASP_TP_DRAG_PERIOD               500

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL uint8     s_asp_tp_drag_timer = 0;

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : to handle the asp message list of delete query window    
//  Global resource dependence : 
//  Author:koki gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );
/*****************************************************************************/
//  Description : to handle the asp playing message of delete query window    
//  Global resource dependence : 
//  Author:koki gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlayMsgDelQueryWin(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            );
/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL void UpdateVolumeButton(BOOLEAN is_update);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL void UpdatePlayPauseButton(BOOLEAN is_update);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL void UpdateControlButton(BOOLEAN is_update);

/*****************************************************************************/
// Description : set vol
// Global resource dependence :
// Author: aoke.hu
// Note:
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr); 

/*****************************************************************************/
// Description : HandleMsgListWinMsg
// Global resource dependence : none
// Author: byte.guo
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgListWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        );

/*****************************************************************************/
// Description : Detail of play win
// Global resource dependence : none
// Author: byte.guo
// Note:
/*****************************************************************************/
LOCAL void UpdatePlayDetailInfo(void);

/*****************************************************************************/
//  Description : 显示进度条和时间
//  Global resource dependence : 
//  Author: apple.zhang
//  Note:
/*****************************************************************************/
LOCAL void DisplayASPWinProgressEx(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : 显示时间
//  Global resource dependence : 
//  Author: koki gao
//  Note:
/*****************************************************************************/
LOCAL void DisplayASPWinTimerEx(GUIOWNDRAW_INFO_T *owner_draw_ptr);

/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : none
//  Author: ryan.xu
//  Note: 
/*****************************************************************************/
LOCAL void SetFormParam(void);

/*****************************************************************************/
//  Description : handle progress bar tp key
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleProgressBarTpKey(int16 tp_x);

/*****************************************************************************/
//  Description : handle play window touch panel key
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlayWinTpKeyDown(
                                         MMI_WIN_ID_T     win_id,
                                         DPARAM     param
                                         );

/*****************************************************************************/
// Description : HandlePlayWinMsg
// Global resource dependence : none
// Author: byte.guo
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlayWinMsg(
                                     MMI_WIN_ID_T     win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM           param 
                                     );
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: 
// Note:    
/*****************************************************************************/
LOCAL void  AppendRecordList(
                             MMI_WIN_ID_T win_id, 
                             MMI_CTRL_ID_T list_id
                             );
/*****************************************************************************/
// Description : handle new msg 
// Global resource dependence : none
// Author: byte.guo
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIdleNewAspMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        );
/*****************************************************************************/
// Description : EnableMsgOpt
// Global resource dependence : none
// Author: byte.guo
// Note:      
/*****************************************************************************/
LOCAL void EnableMsgOpt(MMIASP_APPLET_T*  applet_ptr);
/*****************************************************************************/
// Description : opt handle msg for list win
// Global resource dependence : none
// Author: 
// Note:      
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgListOptMsg(
                                        MMI_WIN_ID_T      win_id,    
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        );
// Description : opt handle msg for help win
// Global resource dependence : none
// Author: 
// Note:      
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleHelpWinMsg(
                                        MMI_WIN_ID_T      win_id,    
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        );
/*****************************************************************************/
// Description : select ring for client
// Global resource dependence : none
// Author: 
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRingSelectWinMsg(
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM           param 
                                          );
/*****************************************************************************/
// Description : opt handle msg of play win
// Global resource dependence : none
// Author: byte.guo
// Note:      
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlayWinOptMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        );
/*****************************************************************************/
// FUNCTION:     Start answer machine
//  Description :   
//  Global resource dependence : 
// Author:       byte.guo
//  Note:   
/*****************************************************************************/
LOCAL MMI_RESULT_E AspAppletHandleEvent(    
                                        IAPPLET_T*          iapplet_ptr,
                                        CAF_MESSAGE_ID_E    msg_id, 
                                        void*              param
                                        );
/*****************************************************************************/
//  Description : get ans_mathine module info
//  Global resource dependence : 
// Author:byte.guo
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E AnsMathineModuleCreateInstance( IMODULE_T* pMod, CAF_GUID_T guid, void** ppObj );
/*****************************************************************************/
//  Description : get ans_mathine module info
//  Global resource dependence : 
// Author:byte.guo
//  Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E AnsMathineModuleEntry( const void* ph, IMODULE_T** ppMod );
/*****************************************************************************/
// Description : Init Button Operation
// Global resource dependence : 
// Author: aoke.hu
// Note:
/*****************************************************************************/
LOCAL void InitButtonOperation(MMIASP_APPLET_T* applet_ptr);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LeftCallback(void);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OKCallback(void);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RightCallback(void);

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VolumeCallback(void);

/*****************************************************************************/
// Description : setting msg
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAspSettingWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       );

/*****************************************************************************/
// Description : select fixed ring
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFixedAnswerRingSelectWinMsg(
                                                      MMI_WIN_ID_T        win_id, 
                                                      MMI_MESSAGE_ID_E    msg_id, 
                                                      DPARAM              param
                                                      );
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void AppendSettingAllDropDownListData( MMIFILE_DEVICE_E *device_arrary_ptr);

/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void InitSettingAllLabels(void);
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SaveAspSetting( MMIFILE_DEVICE_E device);
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL uint16 GetAnswerPhoneIndex(MN_DUAL_SYS_E dual_sys);//current sim
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetAnswerPhoneFlag(
                              MMIASP_NV_SETTING_T* set_nv_ptr,
                              MN_DUAL_SYS_E dual_sys,
                              uint16 index
                              );
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL uint16 GetRecordTimeIndex(void);
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetRecordTime(
                         MMIASP_NV_SETTING_T* set_nv_ptr,
                         uint16 index
                         );
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL uint16 GetWaitTimeIndex(void);
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetWaitTime(
                       MMIASP_NV_SETTING_T* set_nv_ptr,
                       uint16 index
                       );
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL uint16 GetSpeakerFlagIndex(void);
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetSpeakerFlag(
                          MMIASP_NV_SETTING_T* set_nv_ptr,
                          uint16 index
                          );
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetRecordSaveStorage(void);
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetRecordSavePath(
                             MMIASP_NV_SETTING_T* set_nv_ptr,
                             MMIFILE_DEVICE_E device
                             );
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetListMarkMenu(void);
/*****************************************************************************/
//  Description : handle  list mark option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleListMarkOperation(
                                   MMI_CTRL_ID_T   list_ctrl_id,
                                   BOOLEAN         mark_single
                                   );
/*****************************************************************************/
//  Description : handle  list unmark option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleListUnMarkOperation(
                                     MMI_CTRL_ID_T   list_ctrl_id,
                                     BOOLEAN         unmark_single
                                     );
/*****************************************************************************/
//  Description : handle  
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleAspMenuOperation(
                                  MMI_WIN_ID_T      win_id,    
                                  MMI_CTRL_ID_T     menu_ctrl_id
                                  );
/*****************************************************************************/
//  Description : 添加固定答录语list的item
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void AppendFixedAnswerRing(MMI_CTRL_ID_T    ctrl_id);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL  uint16 GetAnswerRingName(wchar* answer_name_ptr);//out

/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReleasingWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );


/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRecordingWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         );
/*****************************************************************************/
//  Description : 检查U盘的状态
//  Global resource dependence : 
//  Author:jun.hu
//  Note:  2010/2/1 
/*****************************************************************************/
LOCAL BOOLEAN IsUDiskRunning(void);
/*****************************************************************************/
//  Description : handle  list item delete option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleMSGListDeleteOperation(void);
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void AppendSwitchFlagDropDownListData( void );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
LOCAL void UpdateRecordingTime(BOOLEAN is_refresh);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetRecordingWinContent(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
LOCAL uint16 GetShowCallName(wchar*   show_name_ptr, //out
                             uint16   max_name_len   //in
                             );

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:jun.hu
// Note:
/*****************************************************************************/
LOCAL void UpdateBackwardForwardButtonFg(BOOLEAN is_fresh);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AppendOneListItem(
                                uint16           item_index,     //in:item索引
                                MMI_CTRL_ID_T    ctrl_id         //in:control id
                                );
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL void  SetButtonsBackGround(void);
/*****************************************************************************/
//  Description : handle msg delete option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleMSGDeleteOperation(void);

#if defined(BLUETOOTH_SUPPORT)  
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void  MMIASP_SendRecordFile(const wchar* record_path_ptr);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle Msg List LongOk ContextMenu
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgListLongOkOptMenuWinMsg(
                                     MMI_WIN_ID_T       win_id,     
                                     MMI_MESSAGE_ID_E    msg_id, 
                                     DPARAM              param
                                     );
#endif

/*****************************************************************************/
// 	Description : 获取指定LIST项的文本信息
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListItemTextInfo(
                                  MMI_CTRL_ID_T ctrl_id,    //[in]  控件ID
                                  uint16        index,      //[in]  索引
                                  MMI_STRING_T  *main_text,  //[out] 主标题
                                  MMI_STRING_T  *sub_text    //[out] 副标题
                                  );

/**-------------------------------------------------------------------------*
*                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/ 
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_answer_machine_applet_array[] = 
{ 
    { GUID_NAME_DEF(SPRD_ANSMACHINE_APPLET_ID), sizeof(MMIASP_APPLET_T), AspAppletHandleEvent, 0, 0 }//answer machine
};

//applet的信息
LOCAL const CAF_STATIC_APPLET_INFO_T s_answer_machine_applet_info = 
{
    //AnsMathineModuleEntry,
    s_answer_machine_applet_array, ARR_SIZE(s_answer_machine_applet_array)  /*lint !e605*/
};

/**--------------------------------------------------------------------------*
**                         WINDOW  DEFINITION                               *
**--------------------------------------------------------------------------*/

/* The play win of record answer*/
#ifndef MMI_PDA_SUPPORT
WINDOW_TABLE( MMIASP_MAIN_PLAY_WIN_TAB ) = 
{
    WIN_TITLE(TXT_MMIASP_PLAY_RECORD),
    WIN_FUNC( (uint32)HandlePlayWinMsg ), 
    WIN_ID( MMIASP_MAIN_PLAY_WIN_ID ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIASP_LABEL_NAME_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIASP_LABEL_TEL_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIASP_LABEL_TOTALTIME_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),
    
    CHILD_OWNDRAW_CTRL(FALSE,MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID,(uint32)DisplayASPWinProgressEx),
    CHILD_OWNDRAW_CTRL(FALSE,MMIASP_TIMER_OWNER_DRAW_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID,(uint32)DisplayASPWinTimerEx),
     
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIASP_PLAY_PANEL_FORM_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_PLAYER_BTN_VOL,MMIASP_BUTTON_VOL_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_BTN_PREV,MMIASP_BUTTON_PREV_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_BTN_PLAY,MMIASP_BUTTON_PLAY_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_BTN_NEXT,MMIASP_BUTTON_NEXT_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID),

    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};
#else
WINDOW_TABLE( MMIASP_MAIN_PLAY_WIN_TAB ) = 
{
    WIN_TITLE(TXT_MMIASP_PLAY_RECORD),
    WIN_FUNC( (uint32)HandlePlayWinMsg ), 
    WIN_ID( MMIASP_MAIN_PLAY_WIN_ID ),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIASP_PLAY_OWNDRAW_NULL_CTRL_ID, MMIASP_PLAY_FORM_CTRL_ID, PNULL),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIASP_LABEL_NAME_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIASP_LABEL_TEL_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,FALSE,MMIASP_LABEL_TOTALTIME_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE,MMIASP_PLAY_PROGRESS_BG_NULL_FORM_CTRL_ID,MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID,(uint32)PNULL),
    CHILD_OWNDRAW_CTRL(FALSE,MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID,MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID,(uint32)DisplayASPWinProgressEx),
    CHILD_OWNDRAW_CTRL(FALSE,MMIASP_TIMER_OWNER_DRAW_CTRL_ID,MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID,(uint32)DisplayASPWinTimerEx),
     
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_SBS,MMIASP_PLAY_PANEL_FORM_CTRL_ID,MMIASP_PLAY_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_PLAYER_BTN_VOL,MMIASP_BUTTON_VOL_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_BTN_PREV,MMIASP_BUTTON_PREV_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_BTN_PLAY,MMIASP_BUTTON_PLAY_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE,IMAGE_COMMON_BTN_NEXT,MMIASP_BUTTON_NEXT_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIASP_PLAY_OWNDRAW_BUTTON_CTRL_ID, MMIASP_PLAY_PANEL_FORM_CTRL_ID, PNULL),

    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};
#endif//MMI_PDA_SUPPORT
/* play win opt menu */
WINDOW_TABLE(MMIASP_MSG_PLAY_OPTION_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePlayWinOptMsg),
    WIN_ID(MMIASP_MSG_OPTION_PLAY_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_POPMENU_CTRL(MENU_ASP_PLAY_WIN_MSG_OPT, MMIASP_MSG_PLAY_WIN_OPT_MENU_CTRL_ID),
    END_WIN
};

/* The msg list show win */
WINDOW_TABLE(MMIASP_MSG_LIST_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMsgListWinMsg),
    WIN_ID(MMIASP_MSG_LIST_WIN_ID),
    
    WIN_TITLE( TXT_MMIASP_ANSWER_PHONE ),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIASP_MSG_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),   
    END_WIN
};

/* list win opt menu */
WINDOW_TABLE(MMIASP_MSG_LIST_OPTION_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMsgListOptMsg),
    WIN_ID(MMIASP_MSG_OPTION_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_POPMENU_CTRL(MENU_ASP_MSG_OPT,MMIASP_MSG_OPT_MENU_CTRL_ID),
    END_WIN
};

//the help win
WINDOW_TABLE(MMIASP_HELP_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleHelpWinMsg ),    
    WIN_ID(MMIASP_MSG_HELP_WIN_ID),
    WIN_TITLE(TXT_HELP),
    CREATE_TEXT_CTRL(MMIASP_HELP_TEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
}; 

/* setting win ----- start-------*/
WINDOW_TABLE(MMIASP_MSG_SETTING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAspSettingWinMsg),
    WIN_ID(MMIASP_MSG_SETTING_WIN_ID),
    WIN_TITLE(STXT_MAIN_SETTING ),
	
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_FORM_CTRL_ID),
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    // SIM1 开关
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_FLAG_SIM1_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_FLAG_SIM1_LABEL_CTRL_ID, MMIASP_SETTING_FLAG_SIM1_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_FLAG_SIM1_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_FLAG_SIM1_FORM_CTRL_ID),

    // SIM2 开关
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_FLAG_SIM2_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_FLAG_SIM2_LABEL_CTRL_ID, MMIASP_SETTING_FLAG_SIM2_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_FLAG_SIM2_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_FLAG_SIM2_FORM_CTRL_ID),


#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
    // SIM3 开关
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_FLAG_SIM3_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_FLAG_SIM3_LABEL_CTRL_ID, MMIASP_SETTING_FLAG_SIM3_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_FLAG_SIM3_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_FLAG_SIM3_FORM_CTRL_ID),

#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    // SIM4 开关
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_FLAG_SIM4_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_FLAG_SIM4_LABEL_CTRL_ID, MMIASP_SETTING_FLAG_SIM4_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_FLAG_SIM4_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_FLAG_SIM4_FORM_CTRL_ID),

#endif
#else
    // 开关
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_FLAG_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_FLAG_LABEL_CTRL_ID, MMIASP_SETTING_FLAG_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_FLAG_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_FLAG_FORM_CTRL_ID),

#endif
    // 答录语
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_AUDIO_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_AUDIO_LABEL_CTRL_ID, MMIASP_SETTING_AUDIO_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, TRUE, MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID, MMIASP_SETTING_AUDIO_FORM_CTRL_ID),

    // 留言长度
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_AUDIO_TIME_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_AUDIO_TIME_LABEL_CTRL_ID, MMIASP_SETTING_AUDIO_TIME_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_AUDIO_TIME_FORM_CTRL_ID),

    // n秒开始答录
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_WAIT_TIME_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_WAIT_TIME_LABEL_CTRL_ID, MMIASP_SETTING_WAIT_TIME_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_WAIT_TIME_FORM_CTRL_ID),

    // 扬声器
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_SPEAKER_FLAG_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_SPEAKER_FLAG_LABEL_CTRL_ID, MMIASP_SETTING_SPEAKER_FLAG_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_SPEAKER_FLAG_FORM_CTRL_ID),

    // 留言存储位置
    CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIASP_SETTING_SAVE_PATH_FORM_CTRL_ID,MMIASP_SETTING_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_SETTING_SAVE_PATH_LABEL_CTRL_ID, MMIASP_SETTING_SAVE_PATH_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE,MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID,MMIASP_SETTING_SAVE_PATH_FORM_CTRL_ID),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
        
};

WINDOW_TABLE(MMIASP_SETTING_SELECT_RING_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleRingSelectWinMsg),    
    WIN_ID( MMIASP_SETTING_RING_WIN_ID),
    WIN_TITLE( TXT_COMM_ANSWER),
#ifndef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_MENU_CTRL(MENU_SETTING_RING_SELECT_OPT, MMIASP_MSG_RING_SELECT_MENU_CTRL_ID),
#else
    CREATE_POPMENU_CTRL(MENU_SETTING_RING_SELECT_OPT,MMIASP_MSG_RING_SELECT_MENU_CTRL_ID),
#endif
    END_WIN
};

WINDOW_TABLE(MMIASP_SETTING_FIXED_ANSWERRING_WIN_TAB) = 
{
    WIN_TITLE( TXT_COMM_ANSWER),
    WIN_FUNC((uint32) HandleFixedAnswerRingSelectWinMsg),    
    WIN_ID(MMIASP_SETTING_FIXED_CONTENT_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIASP_SETTING_FIXED_ANSWERRING_LIST_CTRL_ID),
    END_WIN
};

// 答录机答录界面
WINDOW_TABLE( MMIASP_RECORDING_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleRecordingWinMsg),    
    WIN_ID(MMIASP_RECORDING_WIN_ID),
    WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),
    END_WIN
};

#ifndef MMI_PDA_SUPPORT
// 答录机答录界面
WINDOW_TABLE( MMIASP_RECORDING_WIN_TAB_V ) = 
{
    WIN_TITLE( TXT_ASP_RECORDING),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIASP_RECORDING_FORM_CTRL_ID),
    // 答录的图片
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIASP_RECORDING_IMAGE_ANIM_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 来电者的姓名
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_RECORDING_NAME_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 来电者的号码
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 答录的时间
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_RECORDING_TIMING_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),

    WIN_SOFTKEY(TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_COMMON_HAND_FREE),
    END_WIN
};

// 答录机答录界面
WINDOW_TABLE( MMIASP_RECORDING_WIN_TAB_H ) = 
{
    WIN_TITLE( TXT_ASP_RECORDING),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS, MMIASP_RECORDING_FORM_CTRL_ID),
    // 答录的图片
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIASP_RECORDING_IMAGE_ANIM_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    CHILD_FORM_CTRL(FALSE, GUIFORM_LAYOUT_ORDER, MMIASP_RECORDING_TEXT_FORM_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 来电者的姓名
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_RECORDING_NAME_LABEL_CTRL_ID, MMIASP_RECORDING_TEXT_FORM_CTRL_ID),
    // 来电者的号码
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID, MMIASP_RECORDING_TEXT_FORM_CTRL_ID),
    // 答录的时间
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIASP_RECORDING_TIMING_LABEL_CTRL_ID, MMIASP_RECORDING_TEXT_FORM_CTRL_ID),
 
    WIN_SOFTKEY(TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_COMMON_HAND_FREE),
    END_WIN
};
#else//pda style

// 答录机答录界面
WINDOW_TABLE( MMIASP_RECORDING_WIN_TAB_V ) = 
{
    WIN_TITLE( TXT_ASP_RECORDING),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIASP_RECORDING_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIASP_RECORDING_OWNDRAW_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID, PNULL),
    // 答录的图片
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIASP_RECORDING_IMAGE_ANIM_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 来电者的姓名
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIASP_RECORDING_NAME_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 来电者的号码
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 答录的时间
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIASP_RECORDING_TIMING_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),

    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_ANSWER_CALL, TXT_COMMON_HAND_FREE, TXT_CC_HANG_OFF),
    END_WIN
};

// 答录机答录界面
WINDOW_TABLE( MMIASP_RECORDING_WIN_TAB_H ) = 
{
    WIN_TITLE( TXT_ASP_RECORDING),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIASP_RECORDING_FORM_CTRL_ID),
    CHILD_OWNDRAW_CTRL(FALSE, MMIASP_RECORDING_OWNDRAW_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID, PNULL),
    // 答录的图片
    CHILD_ANIM_CTRL(FALSE, FALSE, MMIASP_RECORDING_IMAGE_ANIM_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 来电者的姓名
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIASP_RECORDING_NAME_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 来电者的号码
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),
    // 答录的时间
    CHILD_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, FALSE, MMIASP_RECORDING_TIMING_LABEL_CTRL_ID, MMIASP_RECORDING_FORM_CTRL_ID),

    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
    WIN_SOFTKEY(TXT_COMMON_ANSWER_CALL, TXT_COMMON_HAND_FREE, TXT_CC_HANG_OFF),
    END_WIN
};
#endif//MMI_PDA_SUPPORT

#ifdef MMI_PDA_SUPPORT
//答录信息列表 长按选项窗口
WINDOW_TABLE( MMIASP_MSGLIST_LONGOK_POPMENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleMsgListLongOkOptMenuWinMsg),
    WIN_ID(MMIASP_MSGLIST_LONGOK_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MSGLIST_LONGOK_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register www module applet info
//  Global resource dependence : none
// Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIASP_RegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_ASP, &s_answer_machine_applet_info );
}
/*****************************************************************************/
//  Description : 检查U盘的状态
//  Global resource dependence : 
//  Author:jun.hu
//  Note:  2010/2/1 
/*****************************************************************************/
LOCAL BOOLEAN IsUDiskRunning(void)
{
    BOOLEAN ret_value = TRUE;
    if(MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        ret_value = FALSE;
    }

    return ret_value;
}
/*****************************************************************************/
// Description : init button 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL void InitButtonOperation(MMIASP_APPLET_T* applet_ptr)
{
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP InitButtonOperation no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_869_112_2_18_2_2_45_136,(uint8*)"");
        return;
    }
    
    // 设置buttons 的前景.....
    UpdateControlButton(FALSE);
    
    GUIBUTTON_SetCallBackFunc(MMIASP_BUTTON_PREV_CTRL_ID,LeftCallback);
    GUIBUTTON_SetCallBackFunc(MMIASP_BUTTON_PLAY_CTRL_ID,OKCallback);
    GUIBUTTON_SetCallBackFunc(MMIASP_BUTTON_NEXT_CTRL_ID,RightCallback);
    GUIBUTTON_SetCallBackFunc(MMIASP_BUTTON_VOL_CTRL_ID,VolumeCallback);
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E LeftCallback(void)
{
    MMIASP_Prev();
    UpdatePlayDetailInfo();
    UpdateControlButton(TRUE);
    DisplayASPWinProgress(FALSE);
    DisplayASPWinProgressTime(FALSE);    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E OKCallback(void)
{
    MMIASP_MSG_INFO_T*  cur_node_ptr = PNULL;
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    if(PNULL == applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, applet_ptr->cur_gui_list_index);
    switch(MMIASP_GetPlayStatus())
    {
    case MMIASP_STATE_PLAY:
        MMIASP_Pause();
        MMIASP_StopProgressTimer();
        DisplayASPWinProgress(FALSE);
        DisplayASPWinProgressTime(FALSE);
        break;    
        
    case MMIASP_STATE_PAUSE:
        MMIASP_Resume();
        CreateProgressTimer();
        break;    
        
    case MMIASP_STATE_NONE:
    case MMIASP_STATE_STOP:// 停止状态下点击再放.....  
        if(PNULL != cur_node_ptr)
        {
            MMIASP_Play((const wchar*)cur_node_ptr->record_path);
            if(!cur_node_ptr->read_flag)
            {
                cur_node_ptr->read_flag = TRUE;
                MMIASP_WriteSysFile();
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIASP]:OKCallback, cur index = %d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_935_112_2_18_2_2_45_137,(uint8*)"d", applet_ptr->cur_gui_list_index);
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"[MMIASP]:PlayPauseKeyMsg, cur state = %d error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_940_112_2_18_2_2_45_138,(uint8*)"d",MMIASP_GetPlayStatus());
        break;
    } 
    UpdatePlayPauseButton(TRUE);

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RightCallback(void)
{
    MMIASP_Next();
    UpdatePlayDetailInfo();
    UpdateControlButton(TRUE);
    DisplayASPWinProgress(FALSE);
    DisplayASPWinProgressTime(FALSE);        
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E VolumeCallback(void)
{
    MMICOM_PANEL_INFO_T panel_info = {0};

    panel_info.x = -1;
    panel_info.y = -1;
    panel_info.cur_value = MMIAPISET_GetMultimVolume();
    panel_info.last_value = MMIAPISET_GetLastMultimVolume();
    panel_info.min_value = MMISET_VOL_ZERO;
    panel_info.max_value = MMISET_VOL_MAX;
    panel_info.set_callback = SetVolumeCallback;
    MMIAPICOM_OpenPanelChildWin(MMIASP_MAIN_PLAY_WIN_ID,&panel_info);
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL void UpdateVolumeButton(BOOLEAN is_update)
{
    GUI_BG_T button_fg = {0};
    uint8    volume = MMIAPISET_GetMultimVolume();

#ifdef MMI_PDA_SUPPORT
    GUI_BG_T press_button_fg = {0};
    press_button_fg.bg_type = GUI_BG_IMG;
#endif
    button_fg.bg_type = GUI_BG_IMG;
    if(0 == volume)
    {
        button_fg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE;
        #ifdef MMI_PDA_SUPPORT
        press_button_fg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_MUTE_FOCUS;
        #endif
    }
    else
    {
        button_fg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL;
        #ifdef MMI_PDA_SUPPORT
        press_button_fg.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_FOCUS;
        #endif
    }
#ifdef MMI_PDA_SUPPORT
    GUIBUTTON_SetPressedFg(MMIASP_BUTTON_VOL_CTRL_ID, &press_button_fg);
#else
    GUIBUTTON_SetPressedFg(MMIASP_BUTTON_VOL_CTRL_ID, &button_fg);
#endif
    GUIBUTTON_SetFg(MMIASP_BUTTON_VOL_CTRL_ID, &button_fg); 
    if(is_update)
    {
        GUIBUTTON_Update(MMIASP_BUTTON_VOL_CTRL_ID);
    }
}


/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL void UpdatePlayPauseButton(BOOLEAN is_update)
{
    GUI_BG_T button_fg = {0};

#ifdef MMI_PDA_SUPPORT
    GUI_BG_T press_button_fg = {0};
    press_button_fg.bg_type = GUI_BG_IMG;
#endif
    button_fg.bg_type = GUI_BG_IMG;
    if(MMIASP_STATE_PLAY == MMIASP_GetPlayStatus())
    {
        button_fg.img_id = IMAGE_COMMON_BTN_PAUSE;  
        #ifdef MMI_PDA_SUPPORT
        press_button_fg.img_id = IMAGE_COMMON_BTN_PAUSE_FOCUS;
        #endif
    }
    else
    {
        button_fg.img_id = IMAGE_COMMON_BTN_PLAY; 
        #ifdef MMI_PDA_SUPPORT
        press_button_fg.img_id = IMAGE_COMMON_BTN_PLAY_FOCUS;
        #endif
    }
#ifdef MMI_PDA_SUPPORT
    GUIBUTTON_SetPressedFg(MMIASP_BUTTON_PLAY_CTRL_ID, &press_button_fg);
#else
    GUIBUTTON_SetPressedFg(MMIASP_BUTTON_PLAY_CTRL_ID, &button_fg);
#endif
    GUIBUTTON_SetFg(MMIASP_BUTTON_PLAY_CTRL_ID, &button_fg); 
    
    if(is_update)
    {
        GUIBUTTON_Update(MMIASP_BUTTON_PLAY_CTRL_ID);
    }
}
/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:jun.hu
// Note:
/*****************************************************************************/
LOCAL void UpdateBackwardForwardButtonFg(BOOLEAN is_fresh)
{
    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    GUI_BG_T button_fg          = {0};
    uint16   msg_total_num      = 0;
#ifdef MMI_PDA_SUPPORT
    GUI_BG_T press_button_fg = {0};
    press_button_fg.bg_type = GUI_BG_IMG;
#endif
    
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP UpdateBackwardForwardButtonFg no aopplet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_1075_112_2_18_2_2_46_139,(uint8*)"");
        return ;
    }
    
    button_fg.bg_type = GUI_BG_IMG;
    //前一曲按钮
    if(0 < applet_ptr->cur_gui_list_index)
    {
        button_fg.img_id = IMAGE_COMMON_BTN_PREV;
        GUIBUTTON_SetFg(MMIASP_BUTTON_PREV_CTRL_ID, &button_fg);
#ifdef MMI_PDA_SUPPORT
        press_button_fg.img_id = IMAGE_COMMON_BTN_PREV_FOCUS;
        GUIBUTTON_SetPressedFg(MMIASP_BUTTON_PREV_CTRL_ID, &press_button_fg);
#else
        GUIBUTTON_SetPressedFg(MMIASP_BUTTON_PREV_CTRL_ID, &button_fg);
#endif
        GUIBUTTON_SetGrayedFg(MMIASP_BUTTON_PREV_CTRL_ID, FALSE, PNULL, is_fresh);
    }
    else
    {
        button_fg.img_id = IMAGE_COMMON_BTN_PREV_GREY;
        GUIBUTTON_SetGrayedFg(MMIASP_BUTTON_PREV_CTRL_ID, TRUE, &button_fg, is_fresh); 
    }
    //后一曲按钮
    msg_total_num = MMIASP_GetListMsgTotalNum(applet_ptr);
    if(msg_total_num > applet_ptr->cur_gui_list_index + 1)
    {
        button_fg.img_id = IMAGE_COMMON_BTN_NEXT;
        GUIBUTTON_SetFg(MMIASP_BUTTON_NEXT_CTRL_ID, &button_fg);
#ifdef MMI_PDA_SUPPORT
        press_button_fg.img_id = IMAGE_COMMON_BTN_NEXT_FOCUS;
        GUIBUTTON_SetPressedFg(MMIASP_BUTTON_NEXT_CTRL_ID, &press_button_fg);
#else
        GUIBUTTON_SetPressedFg(MMIASP_BUTTON_NEXT_CTRL_ID, &button_fg);
#endif
        GUIBUTTON_SetGrayedFg(MMIASP_BUTTON_NEXT_CTRL_ID, FALSE, PNULL, is_fresh);
    }
    else
    {
        button_fg.img_id = IMAGE_COMMON_BTN_NEXT_GREY;
        GUIBUTTON_SetGrayedFg(MMIASP_BUTTON_NEXT_CTRL_ID, TRUE, &button_fg, is_fresh); 
    }
#ifdef MMI_PDA_SUPPORT
    if(is_fresh)
    {
        GUIBUTTON_Update(MMIASP_BUTTON_PREV_CTRL_ID);
        GUIBUTTON_Update(MMIASP_BUTTON_NEXT_CTRL_ID);
    }
#endif
}
/*****************************************************************************/
// Description : 
// Global resource dependence :
// Author:aoke.hu
// Note:
/*****************************************************************************/
LOCAL void UpdateControlButton(BOOLEAN is_update)
{
    UpdateVolumeButton(is_update);
    UpdatePlayPauseButton(is_update);
    UpdateBackwardForwardButtonFg(is_update);
} 

/*****************************************************************************/
// Description : set vol
// Global resource dependence :
// Author: aoke.hu
// Note:
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
    MMISRV_HANDLE_T     audio_handle = MMIASP_GetPlayHandle();

    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    if(PNULL != applet_ptr && PNULL != para_ptr)
    {
        MMIAPISET_SetMultimVolume(para_ptr->cur_value);
        
        if(audio_handle > 0)
        {
            MMISRVAUD_SetVolume(audio_handle, para_ptr->cur_value);
        }
		if (MMISRVAUD_IsAudPlaying(MMIAPIMP3_GetBGPlayerHandle()))
		{
			MMISRVAUD_SetVolume(MMIAPIMP3_GetBGPlayerHandle(),para_ptr->cur_value);
		}
        UpdateVolumeButton(TRUE);
    }
}

/*****************************************************************************/
// Description : HandleMsgListWinMsg
// Global resource dependence : none
// Author: 
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgListWinMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        )
{
    MMI_RESULT_E        result       = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       list_ctrl_id = MMIASP_MSG_LIST_CTRL_ID;
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    MMIASP_MSG_INFO_T*  cur_node_ptr = PNULL;
    
    if(PNULL == applet_ptr)
    {
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP HandleMsgListWinMsg MSGid = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_1180_112_2_18_2_2_46_140,(uint8*)"d", msg_id);
    }

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_COLOR_T font_color=MMITHEME_GetCurThemeFontColor(MMI_THEME_LIST_ITEM0);
            GUILIST_SetTextFont (list_ctrl_id,MMI_DEFAULT_NORMAL_FONT,font_color);
            MMK_SetAtvCtrl(win_id, list_ctrl_id);
            applet_ptr->cur_gui_list_index = 0;
            AppendRecordList(win_id, list_ctrl_id);
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
                MMI_WINDOW_TABLE_CREATE_T create = {0};
                applet_ptr->cur_gui_list_index = GUILIST_GetCurItemIndex(list_ctrl_id ); 
                cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, applet_ptr->cur_gui_list_index);
                if(cur_node_ptr != PNULL )
                {
                        // 正在通话中时，不进入播放答录语 for cr194581.......
                        if(!MMIAPICC_IsInState(CC_IN_CALL_STATE))
                        {
                           
                            if(!cur_node_ptr->read_flag)
                            {
                                cur_node_ptr->read_flag = TRUE;
                                MMIASP_WriteSysFile();
                            }
                            create.win_table_ptr = MMIASP_MAIN_PLAY_WIN_TAB;
                            create.applet_handle = applet_ptr->caf_applet.app_handle;
                            MMK_CreateWinTable( &create );                
                          
                        }
                        else
                        {
                            MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
                        }
                }
        }
         break;
    case MSG_APP_OK:
    case MSG_CTL_OK:
	case MSG_APP_MENU:
        {
            MMI_WINDOW_TABLE_CREATE_T create = {0};
            
            applet_ptr->cur_gui_list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            create.win_table_ptr = MMIASP_MSG_LIST_OPTION_WIN_TAB;
            create.applet_handle = applet_ptr->caf_applet.app_handle;
            MMK_CreateWinTable( &create );    
        }
        break;
        
    case MSG_CTL_LIST_NEED_ITEM_DATA:
        {
            GUILIST_NEED_ITEM_CONTENT_T * need_item_content_ptr = (GUILIST_NEED_ITEM_CONTENT_T*)param;
            if( PNULL != need_item_content_ptr )
            {
                AppendOneListItem(need_item_content_ptr->item_index, list_ctrl_id);
            }
        }
        break;
          
    case MMIASP_MSG_UPDATELIST:
        {
            if(MMK_IsOpenWin(MMIASP_MSG_OPTION_WIN_ID))
            {
                MMK_CloseWin(MMIASP_MSG_OPTION_WIN_ID);
            }
            if(MMK_IsOpenWin(MMIASP_QUERY_WIN_ID))
            {
                MMK_CloseWin(MMIASP_QUERY_WIN_ID);
            }
            AppendRecordList(win_id, list_ctrl_id);
            HandleListUnMarkOperation(MMIASP_MSG_LIST_CTRL_ID, FALSE);
        }
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        MMIASP_CloseAspApplet();
        break;
    case MSG_CTL_LIST_MARK_ITEM:
        {
            GUILIST_MARK_ITEM_T* list_param_ptr = (GUILIST_MARK_ITEM_T *)param;
            
            if(PNULL != list_param_ptr)
            {
                if (list_param_ptr->had_maked)
                {
                    HandleListMarkOperation(MMIASP_MSG_LIST_CTRL_ID, TRUE);
                }
                else
                {
                    HandleListUnMarkOperation(MMIASP_MSG_LIST_CTRL_ID, TRUE);
                }
            }
        }
        break;

    case MSG_FULL_PAINT:
        if(0 == MMIASP_GetListMsgTotalNum(applet_ptr))
        {  
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        }
       break;
#ifdef MMI_PDA_SUPPORT
    case MSG_CTL_LIST_LONGOK:
        //长按List弹出context menu菜单
        if (0 != MMIASP_GetListMsgTotalNum(applet_ptr))
        {
            applet_ptr->cur_gui_list_index = GUILIST_GetCurItemIndex(list_ctrl_id ); 
            MMK_CreateWin((uint32*)MMIASP_MSGLIST_LONGOK_POPMENU_WIN_TAB,PNULL);
        }
        break;
#endif //MMI_PDA_SUPPORT

    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
// Description : Get time info
// Global resource dependence : none
// Author: byte.guo
// Note:
/*****************************************************************************/
PUBLIC void MMIASP_GetTimeInfo(
                              MMIASP_APPLET_T*    applet_ptr
                              )
{
    MMIASP_MSG_INFO_T*  msg_info_ptr = PNULL;
    MMISRVAUD_CONTENT_INFO_T  file_info = {0};
    MMISRV_HANDLE_T     audio_handle = MMIASP_GetPlayHandle();

    if(PNULL == applet_ptr)
    {
        return;
    }
    
    if(0 != audio_handle)
    {
        MMISRVAUD_GetContentInfo(audio_handle, &file_info);
        applet_ptr->total_time = file_info.total_time;
    }
    else 
    {
        msg_info_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr,applet_ptr->cur_gui_list_index);
        if(PNULL != msg_info_ptr)
        {
            MMISRVAUD_GetFileContentInfo(msg_info_ptr->record_path,MMIAPICOM_Wstrlen(msg_info_ptr->record_path),&file_info);
            applet_ptr->total_time = file_info.total_time;
        }
    }
}

/*****************************************************************************/
// Description : Detail of play win
// Global resource dependence : none
// Author: 
// Note:
/*****************************************************************************/
LOCAL void UpdatePlayDetailInfo(void)
{                            
    MMI_STRING_T        name_str = {0};
    MMI_STRING_T        unname_str = {0};
    MMI_STRING_T        phone_number_str = {0};
    MMI_STRING_T        hiden_number_info = {0};
    MMI_STRING_T        record_time_str = {0}; 
    MMI_STRING_T        cur_str_t = {0};
    MMI_STRING_T        user_name_str = {0};
    wchar               temp_str[MMIASP_MAX_STRING_LEN] = {0};
    wchar               phone_number_arr[MMIASP_MAX_STRING_LEN] = {0};
    wchar               time_arr[MMIASP_MAX_STRING_LEN] = {0};
    uint16              phone_number_len = 0;

    MMI_HANDLE_T        win_handle = 0;
    MMI_HANDLE_T        label_handle_1 =  MMIASP_LABEL_NAME_CTRL_ID ;
    MMI_HANDLE_T        label_handle_2 =  MMIASP_LABEL_TEL_CTRL_ID ;
    MMI_HANDLE_T        label_handle_3 =  MMIASP_LABEL_TOTALTIME_CTRL_ID ;

    uint8               uint8_str[MMIFILE_FILE_NAME_MAX_LEN+1] = {0};
    MMIASP_MSG_INFO_T*  msg_info_ptr = PNULL;
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );

    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP UpdatePlayDetailInfo no applet!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_1378_112_2_18_2_2_46_141,(uint8*)"");
        return;
    }
    win_handle = MMK_GetWinHandle( applet_ptr->caf_applet.app_handle, MMIASP_MAIN_PLAY_WIN_ID );
    msg_info_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr,applet_ptr->cur_gui_list_index);

    if(PNULL == msg_info_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP UpdatePlayDetailInfo cur index= %d!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_1386_112_2_18_2_2_46_142,(uint8*)"d", applet_ptr->cur_gui_list_index);
        return;
    }

    user_name_str.wstr_ptr = (wchar*)SCI_ALLOC_APP((CC_MAX_PBNAME_LEN+1) * sizeof(wchar));
    user_name_str.wstr_len = 0;
    if(PNULL == user_name_str.wstr_ptr)
    {
        return;
    }

    if(MMIASP_GetNameByNumber(msg_info_ptr->call_num, &user_name_str))
    {
        MMI_GetLabelTextByLang(TXT_MMIASP_LINKMAN, &name_str);
        SCI_MEMCPY(temp_str,  name_str.wstr_ptr, name_str.wstr_len*2);
        SCI_MEMCPY(temp_str + name_str.wstr_len, user_name_str.wstr_ptr, user_name_str.wstr_len*2);
        cur_str_t.wstr_len = name_str.wstr_len + user_name_str.wstr_len;
        cur_str_t.wstr_ptr = temp_str;
        GUILABEL_SetText(label_handle_1 , &cur_str_t, TRUE);
        
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MMIASP_LINKMAN, &name_str);
        MMI_GetLabelTextByLang(TXT_NO_NAME,  &unname_str);
        
        SCI_MEMCPY(temp_str,  name_str.wstr_ptr, name_str.wstr_len*2);
        SCI_MEMCPY(temp_str + name_str.wstr_len, unname_str.wstr_ptr, unname_str.wstr_len*2);
        
        cur_str_t.wstr_len = name_str.wstr_len + unname_str.wstr_len;
        cur_str_t.wstr_ptr = temp_str;
        GUILABEL_SetText(label_handle_1, &cur_str_t, TRUE);
    }

    
    /* display phone number */
    SCI_MEMSET(temp_str,0x00,sizeof(temp_str));
    MMI_GetLabelTextByLang(TXT_PHONE_NUMBER, &phone_number_str);
    MMIAPICOM_Wstrncpy(temp_str, phone_number_str.wstr_ptr, phone_number_str.wstr_len);
    MMIAPICOM_StrToWstr(msg_info_ptr->call_num ,phone_number_arr);
    phone_number_len = MMIAPICOM_Wstrlen(phone_number_arr);
    if(0==phone_number_len)
    {
        MMI_GetLabelTextByLang(TXT_UNKNOW_NUM,  &hiden_number_info);
        MMIAPICOM_Wstrcat(temp_str, hiden_number_info.wstr_ptr);
        cur_str_t.wstr_len = phone_number_str.wstr_len + hiden_number_info.wstr_len;
    }
    else
    {

        MMIAPICOM_Wstrcat(temp_str, phone_number_arr);
        cur_str_t.wstr_len = phone_number_str.wstr_len + phone_number_len;
    }   
    cur_str_t.wstr_ptr = temp_str;
    GUILABEL_SetText(label_handle_2, &cur_str_t, TRUE);

    // 时长
    SCI_MEMSET(uint8_str, 0x00, sizeof(uint8_str));
    SCI_MEMSET(time_arr, 0x00, sizeof(time_arr));
    sprintf((char*)uint8_str, "%02ld:%02ld",(uint32)(applet_ptr->total_time / 60), (applet_ptr->total_time % 60));
    //SCI_TRACE_LOW:"MMIASP total time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_1446_112_2_18_2_2_46_143,(uint8*)"d", applet_ptr->total_time);
    MMIAPICOM_StrToWstr(uint8_str, time_arr);

    
    /* display total time */
    SCI_MEMSET(temp_str,0x00,sizeof(temp_str));
    MMI_GetLabelTextByLang(TXT_TOTAL_TIME, &record_time_str);
    MMIAPICOM_Wstrncpy(temp_str, record_time_str.wstr_ptr, record_time_str.wstr_len);
    MMIAPICOM_Wstrcat(temp_str, time_arr);
    cur_str_t.wstr_len = record_time_str.wstr_len + MMIAPICOM_Wstrlen(time_arr);
    cur_str_t.wstr_ptr = temp_str;
    GUILABEL_SetText(label_handle_3, &cur_str_t, TRUE);
    
    
    /* free user name buffer */
    if (PNULL != user_name_str.wstr_ptr)
    {
        SCI_FREE(user_name_str.wstr_ptr);
        user_name_str.wstr_ptr = PNULL;
    }
}


/*****************************************************************************/
//  Description : 显示进度条?
//  Global resource dependence : 
//  Author: koki gao
//  Note:
/*****************************************************************************/
LOCAL void DisplayASPWinProgressEx(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    DisplayASPWinProgress(FALSE);  
}

/*****************************************************************************/
//  Description : 显示时间
//  Global resource dependence : 
//  Author: koki gao
//  Note:
/*****************************************************************************/
LOCAL void DisplayASPWinTimerEx(GUIOWNDRAW_INFO_T *owner_draw_ptr)
{
    DisplayASPWinProgressTime(FALSE);  
}

/*****************************************************************************/
//  Description : Display the progress according to dynamic playing info..
//  Global resource dependence : 
//  Author: koki.gao
//  Note: 
/*****************************************************************************/
PUBLIC void DisplayASPWinProgress(BOOLEAN is_end)
{
  
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_WIN_ID_T        win_id = MMIASP_MAIN_PLAY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID;
    MMISRVAUD_PLAY_INFO_T  playinfo = {0};
    MMISRV_HANDLE_T     audio_handle = MMIASP_GetPlayHandle();
/*进度条变量 */    
    GUI_POINT_T         dis_point = {0};
    uint16              slide_display_x_max = 0;
    uint16              cur_slide_point = 0;
    GUI_RECT_T          progress_lcd_rect = {0};
    GUI_RECT_T          progress_groove_rect = {0};
    uint16              progress_width = 0;
    uint16              progress_height = 0;
    uint16              slide_width = 0;
    uint16              slide_height = 0;
    int16                slide_y_offset = 0;
    
    uint16              t_width = 0;
    uint16              t_height = 0;
    uint16              t_yoffset = 0;
    
/*显示进度条*/
    GUIRES_GetImgWidthHeight(&t_width,&t_height,IMAGE_COMMON_PLAYER_PROCESS_GROOVE,win_id);
    GUIOWNDRAW_GetDisplayRect(ctrl_id, &progress_groove_rect);
    GUIRES_DisplayImg(PNULL,&progress_groove_rect,&progress_groove_rect,win_id,IMAGE_COMMON_BG,&lcd_dev_info);
    {
        GUI_RECT_T   rect_img        = {0};         
        
        rect_img.left = progress_groove_rect.left;
        rect_img.top = 2;
        rect_img.right = progress_groove_rect.right;
        rect_img.bottom = 2 + progress_groove_rect.bottom - progress_groove_rect.top;
        GUIRES_DisplayImg(PNULL,&progress_groove_rect,&rect_img,win_id,IMAGE_COMMON_PROCESSBAR_BG,&lcd_dev_info);
    }
    t_yoffset = (progress_groove_rect.bottom - progress_groove_rect.top + 1 - t_height)/2;
    progress_groove_rect.left = progress_groove_rect.left + MMIASP_PROGRESS_HORIZ_MARGIN;
    progress_groove_rect.right = progress_groove_rect.right - MMIASP_PROGRESS_HORIZ_MARGIN;
    progress_groove_rect.top += t_yoffset;
    progress_groove_rect.bottom = progress_groove_rect.top + t_height - 1;
    
    t_width=t_height=0;
    GUIRES_GetImgWidthHeight(&t_width,&t_height,IMAGE_COMMON_PLAYER_PROCESS,win_id);
    progress_lcd_rect.left  = progress_groove_rect.left;
    progress_lcd_rect.top   = progress_groove_rect.top;
    progress_lcd_rect.right = progress_groove_rect.right;
    progress_lcd_rect.bottom = progress_groove_rect.top + t_height - 1;
        
    GUIRES_GetImgWidthHeight(&slide_width,&slide_height,IMAGE_COMMON_PLAYER_PROCESS_SLIDE,win_id);
    progress_width = progress_lcd_rect.right - progress_lcd_rect.left + 1;
    progress_height = progress_lcd_rect.bottom - progress_lcd_rect.top + 1;

    slide_y_offset = (progress_height - slide_height)/2;

    if(audio_handle)
    {
        MMISRVAUD_GetPlayingInfo(audio_handle, &playinfo);      
    }

    GUIRES_DisplayImg(
            PNULL,
            &progress_groove_rect,
            PNULL,
            win_id,
            IMAGE_COMMON_PLAYER_PROCESS_GROOVE,
            &lcd_dev_info);
    
    if(is_end)
    {
            cur_slide_point = progress_width - slide_width + 1;
            slide_display_x_max = progress_groove_rect.left + progress_width - slide_width+ 1;
            progress_lcd_rect.right = progress_lcd_rect.left + (int16)(MMI_MIN(cur_slide_point+1, progress_width)); 

            if(progress_lcd_rect.right > progress_lcd_rect.left)
            {
                GUIRES_DisplayImg(PNULL,
                        &progress_lcd_rect,
                        PNULL,
                        win_id,
                        IMAGE_COMMON_PLAYER_PROCESS,
                        &lcd_dev_info);

            }

            IMG_EnableTransparentColor(TRUE);
            dis_point.x = MMI_MIN(progress_groove_rect.left + cur_slide_point,slide_display_x_max);
            dis_point.y = slide_y_offset + progress_groove_rect.top;
            GUIRES_DisplayImg(&dis_point,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
                    &lcd_dev_info);
            IMG_EnableTransparentColor(FALSE);
    }
    else
    {
        if ((0 != playinfo.total_data_length))
        {
                cur_slide_point = (uint16)((progress_width - slide_width + 1)*(uint64)playinfo.cur_data_offset /playinfo.total_data_length);
                slide_display_x_max = progress_groove_rect.left + progress_width - slide_width+ 1;
                progress_lcd_rect.right = progress_lcd_rect.left + (int16)(MMI_MIN(cur_slide_point+1, progress_width)); 

                if(progress_lcd_rect.right > progress_lcd_rect.left)
                {
                    GUIRES_DisplayImg(PNULL,
                            &progress_lcd_rect,
                            PNULL,
                            win_id,
                            IMAGE_COMMON_PLAYER_PROCESS,
                            &lcd_dev_info);

                }

                IMG_EnableTransparentColor(TRUE);
                dis_point.x = MMI_MIN(progress_groove_rect.left + cur_slide_point,slide_display_x_max);
                dis_point.y = slide_y_offset + progress_groove_rect.top;
                GUIRES_DisplayImg(&dis_point,
                        PNULL,
                        PNULL,
                        win_id,
                        IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
                        &lcd_dev_info);
                IMG_EnableTransparentColor(FALSE);
            
        }
        else
        {
            IMG_EnableTransparentColor(TRUE);
            dis_point.x = progress_groove_rect.left;
            dis_point.y = slide_y_offset + progress_groove_rect.top;
            GUIRES_DisplayImg(&dis_point,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_COMMON_PLAYER_PROCESS_SLIDE,
                    &lcd_dev_info);
            IMG_EnableTransparentColor(FALSE);
        }
     }
     
}

/*****************************************************************************/
//  Description :  播放界面时间信息显示
//  Global resource dependence : 
//  Author:koki gao
//  Note:
/*****************************************************************************/
PUBLIC void DisplayASPWinProgressTime(BOOLEAN is_end)
{
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_WIN_ID_T        win_id = MMIASP_MAIN_PLAY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIASP_TIMER_OWNER_DRAW_CTRL_ID;
    
    MMISRVAUD_PLAY_INFO_T playinfo = {0};
    MMISRV_HANDLE_T     audio_handle = MMIASP_GetPlayHandle();
    
 /*时间变量 */
    uint8           cur_time_str[32]    = {0}; 
    uint8           total_time_str[32]  = {0}; 
    wchar           cur_time_wstr[32]   = {0}; 
    wchar           total_time_wstr[32] = {0}; 
    MMI_STRING_T    cur_str_t   = {0};
    MMI_STRING_T    total_str_t = {0};
    GUI_RECT_T   cur_time_rect          = {0};
    GUI_RECT_T   total_time_rect        = {0}; 
    GUI_RECT_T   rect        = {0}; 
    GUISTR_STYLE_T      text_style      = {0};  
    GUISTR_STATE_T      state =         GUISTR_STATE_ALIGN|GUISTR_STATE_WORDBREAK|GUISTR_STATE_SINGLE_LINE; 
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );

    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP UpdatePlayDetailInfo no applet!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_1665_112_2_18_2_2_47_144,(uint8*)"");
        return;
    }
    /*显示时间*/
    if(audio_handle > 0)
    {
        MMISRVAUD_GetPlayingInfo(audio_handle, &playinfo);
    }
	
	playinfo.total_time = applet_ptr->total_time;
	

    GUIOWNDRAW_GetDisplayRect(ctrl_id, &rect);
    GUIRES_DisplayImg(PNULL,&rect,&rect,win_id,IMAGE_COMMON_BG,&lcd_dev_info);  
    {
        GUI_RECT_T   rect_img        = {0}; 
        rect_img.left = rect.left;
        rect_img.top = 2;
        rect_img.right = rect.right;
        rect_img.bottom = 2 + rect.bottom - rect.top;
        GUIRES_DisplayImg(PNULL,&rect,&rect_img,win_id,IMAGE_COMMON_PROCESSBAR_BG,&lcd_dev_info);
    }
    
    cur_time_rect.left   = rect.left  + MMIASP_PROGRESS_HORIZ_MARGIN;
    cur_time_rect.top    = rect.top; 
    cur_time_rect.right  = cur_time_rect.left + MMIASP_TIME_WIDTH;
    cur_time_rect.bottom = cur_time_rect.top + MMIASP_TIME_HEIGHT;
    total_time_rect.right  = rect.right  - MMIASP_PROGRESS_HORIZ_MARGIN;
    total_time_rect.top    = cur_time_rect.top;
    total_time_rect.left   = total_time_rect.right - MMIASP_TIME_WIDTH;
    total_time_rect.bottom =  cur_time_rect.bottom;

    if(is_end)
    {
        playinfo.cur_time = playinfo.total_time = applet_ptr->total_time;
    }
    
    if(playinfo.total_time/60 < 60)
    {
        cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld", playinfo.cur_time/60, playinfo.cur_time%60);
        total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld", playinfo.total_time/60, playinfo.total_time%60);
    }
    else
    {
        cur_str_t.wstr_len = sprintf((char*)cur_time_str, "%.2ld:%.2ld:%.2ld", 
            (playinfo.cur_time/60)/60, 
            (playinfo.cur_time/60)%60,
            (playinfo.cur_time%60));
        
        total_str_t.wstr_len = sprintf((char*)total_time_str, "%.2ld:%.2ld:%.2ld", 
            (playinfo.total_time/60)/60,
            (playinfo.total_time/60)%60, 
            playinfo.total_time%60);
    }

    cur_str_t.wstr_ptr = cur_time_wstr;
    total_str_t.wstr_ptr = total_time_wstr;
    MMI_STRNTOWSTR(cur_str_t.wstr_ptr, cur_str_t.wstr_len, (uint8*)cur_time_str, cur_str_t.wstr_len, cur_str_t.wstr_len);
    MMI_STRNTOWSTR(total_str_t.wstr_ptr, total_str_t.wstr_len, (uint8*)total_time_str, total_str_t.wstr_len, total_str_t.wstr_len);

    text_style.align = ALIGN_LEFT;
    text_style.font = MMIASP_TIME_FONT;
    text_style.font_color = MMI_GRAY_WHITE_COLOR;
      
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&cur_time_rect,       
        (const GUI_RECT_T      *)&cur_time_rect,     
        (const MMI_STRING_T    *)&cur_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        ); 

    text_style.align = ALIGN_RIGHT;
    GUISTR_DrawTextToLCDInRect( 
        (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
        (const GUI_RECT_T      *)&total_time_rect,       
        (const GUI_RECT_T      *)&total_time_rect,       
        (const MMI_STRING_T    *)&total_str_t,
        &text_style,
        state,
        GUISTR_TEXT_DIR_AUTO
        );

}

/*****************************************************************************/
//  Description : SetFormParam
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void SetFormParam(void)
{
    GUIFORM_CHILD_WIDTH_T    sbs = {0}; 
    uint16      ver_label_space = MMIASP_PLAY_TEXT_SPACE_HEIGHT;
    uint16      ver_space = MMIASP_PLAY_SPACE_HEIGHT;
    GUI_BG_T    gui_bg = {0};
#ifdef MMI_PDA_SUPPORT	
    uint16 height = 0;
    uint16 width = 0;
#endif
    GUIFORM_CHILD_HEIGHT_T child_height = {0};
    //set form not support slide
    GUIFORM_IsSlide(MMIASP_PLAY_FORM_CTRL_ID,FALSE);  
    gui_bg.bg_type = GUI_BG_IMG;
    gui_bg.img_id  = IMAGE_COMMON_BG;
    GUIFORM_SetBg(MMIASP_PLAY_FORM_CTRL_ID, &gui_bg);

#ifndef MMI_PDA_SUPPORT
    if(MMITHEME_IsMainScreenLandscape())
    {
        ver_label_space = MMIASP_PLAY_TEXT_SPACE_HEIGHT_H;
        ver_space = MMIASP_PLAY_SPACE_HEIGHT_H;
        sbs.add_data = MMIASP_PLAY_PROGRESS_WIDTH_H;
#ifdef MAINLCD_SIZE_176X220
        GUIFORM_SetChildDisplay(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_LABEL_TOTALTIME_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
#endif
    }
    else
    {
#ifdef MAINLCD_SIZE_176X220
        GUIFORM_SetChildDisplay(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_LABEL_TOTALTIME_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
#endif    
    }
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_LABEL_TEL_CTRL_ID,PNULL,&ver_label_space);
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_LABEL_TOTALTIME_CTRL_ID,PNULL,&ver_label_space);
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID,PNULL,&ver_space);
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_TIMER_OWNER_DRAW_CTRL_ID,PNULL,&ver_label_space);
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID,PNULL,&ver_label_space);

    child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
    GUIFORM_SetChildHeight(MMIASP_PLAY_FORM_CTRL_ID, MMIASP_PLAY_PANEL_FORM_CTRL_ID, &child_height);
    child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
    GUIFORM_SetChildHeight(MMIASP_PLAY_PANEL_FORM_CTRL_ID, MMIASP_BUTTON_VOL_CTRL_ID, &child_height);
#else//pda style

    GUIFORM_SetMargin(MMIASP_PLAY_FORM_CTRL_ID, 0);
    GUIFORM_SetMargin(MMIASP_PLAY_PANEL_FORM_CTRL_ID, 0);
    GUIFORM_SetMargin(MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID, 0); 
    gui_bg.bg_type = GUI_BG_IMG;
    gui_bg.img_id  = IMAGE_COMMON_PROCESSBAR_BG;
    GUIFORM_SetBg(MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID, &gui_bg);

    if(MMITHEME_IsMainScreenLandscape())
    {
        ver_label_space = MMIASP_PLAY_TEXT_SPACE_HEIGHT_H;
        ver_space = MMIASP_PLAY_SPACE_HEIGHT_H;
        sbs.add_data = MMIASP_PLAY_PROGRESS_WIDTH_H;
#ifdef MAINLCD_SIZE_176X220
        GUIFORM_SetChildDisplay(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_LABEL_TOTALTIME_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
#endif
#if defined MAINLCD_SIZE_240X400 && defined MMI_PDA_SUPPORT
#if defined MMI_ISTYLE_SUPPORT
        if(MMITHEME_IsIstyle())
        {
            GUIOWNDRAW_SetHeight(MMIASP_PLAY_OWNDRAW_NULL_CTRL_ID, MMIASP_PLAY_OWNDRAW_NULL_ISTYLE_HEIGHT_H);
        }
        else
#endif			
        {
            GUIOWNDRAW_SetHeight(MMIASP_PLAY_OWNDRAW_NULL_CTRL_ID, MMIASP_PLAY_OWNDRAW_NULL_HEIGHT_H);
        }
#else
        GUIOWNDRAW_SetHeight(MMIASP_PLAY_OWNDRAW_NULL_CTRL_ID, MMIASP_PLAY_OWNDRAW_NULL_HEIGHT_H);
#endif


#if defined MAINLCD_SIZE_240X320 && defined MMI_PDA_SUPPORT 
#if defined MMI_ISTYLE_SUPPORT
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUILABEL_SetFont(MMIASP_LABEL_NAME_CTRL_ID, SONG_FONT_20, MMI_BLACK_COLOR);
            GUILABEL_SetFont(MMIASP_LABEL_TEL_CTRL_ID, SONG_FONT_20, MMI_BLACK_COLOR);
            GUILABEL_SetFont(MMIASP_LABEL_TOTALTIME_CTRL_ID, SONG_FONT_20, MMI_BLACK_COLOR);
        }	
        else
#endif			
        {
            //need add later
        }
#endif
    }
    else
    {
#ifdef MAINLCD_SIZE_176X220
        GUIFORM_SetChildDisplay(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_LABEL_TOTALTIME_CTRL_ID,GUIFORM_CHILD_DISP_NORMAL);
#endif

        /*if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIOWNDRAW_SetHeight(MMIASP_PLAY_OWNDRAW_NULL_CTRL_ID, MMIASP_PLAY_OWNDRAW_NULL_ISTYLE_HEIGHT - MMI_STATUSBAR_HEIGHT);
        } 
        else*/
        {
            GUIOWNDRAW_SetHeight(MMIASP_PLAY_OWNDRAW_NULL_CTRL_ID, MMIASP_PLAY_OWNDRAW_NULL_HEIGHT);
        }         
    }
    GUIOWNDRAW_SetHeight(MMIASP_PLAY_PROGRESS_BG_NULL_FORM_CTRL_ID, MMIASP_PLAY_OWNDRAW_PROGRESS_BG_NULL_HEIGHT);
    child_height.type = GUIFORM_CHILD_HEIGHT_FIXED;
    GUIRES_GetImgWidthHeight(PNULL,&child_height.add_data,IMAGE_COMMON_PROCESSBAR_BG,MMIASP_MAIN_PLAY_WIN_ID); 
    GUIFORM_SetChildHeight(MMIASP_PLAY_FORM_CTRL_ID, MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID, &child_height);

    GUIRES_GetImgWidthHeight(PNULL,&child_height.add_data,IMAGE_COMMON_PLAYER_PROCESS_SLIDE,MMIASP_MAIN_PLAY_WIN_ID);    
    GUIFORM_SetChildHeight(MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID, MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID, &child_height);
    
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_LABEL_TEL_CTRL_ID,PNULL,&ver_label_space);
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_LABEL_TOTALTIME_CTRL_ID,PNULL,&ver_label_space);
    if(MMITHEME_IsMainScreenLandscape())
    {
        /*if(MMITHEME_IsIstyle())
        {
            width = MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT_H;
        }
        else*/
        {
            width = MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT_H;
        }
    }
    else
    {
        /*if(MMITHEME_IsIstyle())
        {
            width = MMIASP_PLAY_PROGRESS_BG_SPACE_ISTYLE_HEIGHT;
        }
        else*/
        {
            width = MMIASP_PLAY_PROGRESS_BG_SPACE_HEIGHT;
        }
    }
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_PLAY_PROGRESS_BG_FORM_CTRL_ID,PNULL,&width);
    width = MMIASP_PLAY_PROGRESS_OWNER_DRAW_SPACE_HEIGHT;
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID,PNULL,&width);
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_TIMER_OWNER_DRAW_CTRL_ID,PNULL,&ver_label_space);
    width = 0;
    GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID,PNULL,&width);
    //GUIFORM_SetChildSpace(MMIASP_PLAY_FORM_CTRL_ID,MMIASP_PLAY_PANEL_FORM_CTRL_ID,PNULL,&ver_label_space);

    {
        
//        GUI_BOTH_RECT_T both_rect = {0};
        //GUIFORM_CHILD_WIDTH_T button_width = {0};

        GUIFORM_SetAlign(MMIASP_PLAY_PANEL_FORM_CTRL_ID,GUIFORM_CHILD_ALIGN_HMIDDLE);
           
//        both_rect = MMITHEME_GetFullScreenBothRect();
/*        if(MMITHEME_IsMainScreenLandscape())
        {
            height = 1;
//            width = both_rect.h_rect.right;
        }
        else
        {
            GUIRES_GetImgWidthHeight(PNULL,&height,IMAGE_COMMON_BTN_BG,MMIASP_MAIN_PLAY_WIN_ID);     
//            width = both_rect.v_rect.right;
        }*/
//        GUIOWNDRAW_SetHeight(MMIASP_PLAY_OWNDRAW_BUTTON_CTRL_ID,height);
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT;
        GUIFORM_SetChildHeight(MMIASP_PLAY_FORM_CTRL_ID, MMIASP_PLAY_PANEL_FORM_CTRL_ID, &child_height);
        child_height.type = GUIFORM_CHILD_HEIGHT_LEFT_MIN;
        GUIFORM_SetChildHeight(MMIASP_PLAY_PANEL_FORM_CTRL_ID, MMIASP_BUTTON_VOL_CTRL_ID, &child_height);
        
//        sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
//        sbs.add_data = width + 1;

        sbs.type = GUIFORM_CHILD_WIDTH_PERCENT;
        sbs.add_data = 1;
        GUIFORM_SetChildWidth(MMIASP_PLAY_PANEL_FORM_CTRL_ID,MMIASP_PLAY_OWNDRAW_BUTTON_CTRL_ID,&sbs);
        sbs.add_data = 25;
        GUIFORM_SetChildWidth(MMIASP_PLAY_PANEL_FORM_CTRL_ID, MMIASP_BUTTON_VOL_CTRL_ID, &sbs);
        GUIFORM_SetChildWidth(MMIASP_PLAY_PANEL_FORM_CTRL_ID, MMIASP_BUTTON_PREV_CTRL_ID, &sbs);
        GUIFORM_SetChildWidth(MMIASP_PLAY_PANEL_FORM_CTRL_ID, MMIASP_BUTTON_PLAY_CTRL_ID, &sbs);
        sbs.add_data = 24;
        GUIFORM_SetChildWidth(MMIASP_PLAY_PANEL_FORM_CTRL_ID, MMIASP_BUTTON_NEXT_CTRL_ID, &sbs);        
    }
#endif
    SetButtonsBackGround();
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL void  SetButtonsBackGround(void)
{
    GUI_BG_T    bg_info = {0}; 
#ifdef MMI_PDA_SUPPORT
    GUI_BG_T    fg_info = {0}; 
#endif

    // 设置四个button的背景图片.....
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_COMMON_BTN_BG;
    GUIFORM_SetBg(MMIASP_PLAY_PANEL_FORM_CTRL_ID, &bg_info);

#ifndef MMI_PDA_SUPPORT
    //音量按钮 没有光影效果，设置按下的背景图片以及正常没有背景图片
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(MMIASP_BUTTON_VOL_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(MMIASP_BUTTON_VOL_CTRL_ID,&bg_info);
    GUIBUTTON_SetRunSheen(MMIASP_BUTTON_VOL_CTRL_ID, FALSE);  
    
    //前一曲按钮 没有光影效果，设置按下的背景图片以及正常没有背景图片，处理长按消息 
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(MMIASP_BUTTON_PREV_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(MMIASP_BUTTON_PREV_CTRL_ID,&bg_info);
    GUIBUTTON_SetRunSheen(MMIASP_BUTTON_PREV_CTRL_ID, FALSE);  
    GUIBUTTON_SetHandleLong(MMIASP_BUTTON_PREV_CTRL_ID,TRUE);
    
    //后一曲按钮 没有光影效果，设置按下的背景图片以及正常没有背景图片，处理长按消息
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(MMIASP_BUTTON_NEXT_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(MMIASP_BUTTON_NEXT_CTRL_ID,&bg_info);
    GUIBUTTON_SetRunSheen(MMIASP_BUTTON_NEXT_CTRL_ID, FALSE);  
    GUIBUTTON_SetHandleLong(MMIASP_BUTTON_NEXT_CTRL_ID,TRUE);
    
    //播放暂停按钮 没有光影效果，设置按下的背景图片以及正常没有背景图片
    bg_info.bg_type = GUI_BG_IMG;
    bg_info.img_id = IMAGE_COMMON_BTN_PRESS;
    GUIBUTTON_SetPressedBg(MMIASP_BUTTON_PLAY_CTRL_ID,&bg_info);  
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetBg(MMIASP_BUTTON_PLAY_CTRL_ID,&bg_info);
    GUIBUTTON_SetRunSheen(MMIASP_BUTTON_PLAY_CTRL_ID, FALSE);
#else//pda style
    //音量按钮 没有光影效果，设置按下的背景图片以及正常没有背景图片
    bg_info.bg_type = GUI_BG_NONE;
    GUIBUTTON_SetPressedBg(MMIASP_BUTTON_VOL_CTRL_ID,&bg_info);  
    GUIBUTTON_SetBg(MMIASP_BUTTON_VOL_CTRL_ID,&bg_info);
    GUIBUTTON_SetPressedBg(MMIASP_BUTTON_PREV_CTRL_ID,&bg_info);  
    GUIBUTTON_SetBg(MMIASP_BUTTON_PREV_CTRL_ID,&bg_info);
    GUIBUTTON_SetPressedBg(MMIASP_BUTTON_NEXT_CTRL_ID,&bg_info);  
    GUIBUTTON_SetBg(MMIASP_BUTTON_NEXT_CTRL_ID,&bg_info);
    GUIBUTTON_SetPressedBg(MMIASP_BUTTON_PLAY_CTRL_ID,&bg_info);  
    GUIBUTTON_SetBg(MMIASP_BUTTON_PLAY_CTRL_ID,&bg_info);
    
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = IMAGE_COMMON_PLAYER_BTN_VOL_FOCUS;
    GUIBUTTON_SetPressedFg(MMIASP_BUTTON_VOL_CTRL_ID,&fg_info);  
    fg_info.img_id = IMAGE_COMMON_PLAYER_BTN_VOL;
    GUIBUTTON_SetFg(MMIASP_BUTTON_VOL_CTRL_ID,&fg_info);
    GUIBUTTON_SetRunSheen(MMIASP_BUTTON_VOL_CTRL_ID, FALSE);  
    
    //前一曲按钮 没有光影效果，设置按下的背景图片以及正常没有背景图片，处理长按消息 
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = IMAGE_COMMON_BTN_PREV_FOCUS;
    GUIBUTTON_SetPressedFg(MMIASP_BUTTON_PREV_CTRL_ID,&fg_info);  
    fg_info.img_id = IMAGE_COMMON_BTN_PREV;
    GUIBUTTON_SetFg(MMIASP_BUTTON_PREV_CTRL_ID,&fg_info);
    GUIBUTTON_SetRunSheen(MMIASP_BUTTON_PREV_CTRL_ID, FALSE);  
    GUIBUTTON_SetHandleLong(MMIASP_BUTTON_PREV_CTRL_ID,TRUE);
    
    //后一曲按钮 没有光影效果，设置按下的背景图片以及正常没有背景图片，处理长按消息
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = IMAGE_COMMON_BTN_NEXT_FOCUS;
    GUIBUTTON_SetPressedFg(MMIASP_BUTTON_NEXT_CTRL_ID,&fg_info);  
    fg_info.img_id = IMAGE_COMMON_BTN_NEXT;
    GUIBUTTON_SetFg(MMIASP_BUTTON_NEXT_CTRL_ID,&fg_info);
    GUIBUTTON_SetRunSheen(MMIASP_BUTTON_NEXT_CTRL_ID, FALSE);  
    GUIBUTTON_SetHandleLong(MMIASP_BUTTON_NEXT_CTRL_ID,TRUE);
    
    //播放暂停按钮 没有光影效果，设置按下的背景图片以及正常没有背景图片
    fg_info.bg_type = GUI_BG_IMG;
    fg_info.img_id = IMAGE_COMMON_BTN_PLAY_FOCUS;
    GUIBUTTON_SetPressedFg(MMIASP_BUTTON_PLAY_CTRL_ID,&fg_info);  
    fg_info.img_id = IMAGE_COMMON_BTN_PLAY;
    GUIBUTTON_SetFg(MMIASP_BUTTON_PLAY_CTRL_ID,&fg_info);
    GUIBUTTON_SetRunSheen(MMIASP_BUTTON_PLAY_CTRL_ID, FALSE);
#endif//MMI_PDA_SUPPORT
}

/*****************************************************************************/
//  Description : handle progress bar tp key
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleProgressBarTpKey(int16 tp_x)
{        
    GUI_RECT_T      progress_bar_rect   = {0};
    MMISRV_HANDLE_T handle = MMIASP_GetPlayHandle();
    MMISRVAUD_PLAY_INFO_T play_info = {0};
    uint16 percent = 0;
    uint32 seek_offset = 0;    
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    
    //SCI_TRACE_LOW:"[MMIASP]: HandleProgressBarTpKey, tp_x= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2039_112_2_18_2_2_47_145,(uint8*)"d",tp_x);
    
    GUIOWNDRAW_GetDisplayRect(MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID, &progress_bar_rect);

    if(handle > 0)
    {
        if(MMISRVAUD_GetPlayingInfo(handle, &play_info))
        {                
            percent = (uint16)(100*(tp_x - progress_bar_rect.left)/(progress_bar_rect.right - progress_bar_rect.left + 1));
            seek_offset = (uint32)((play_info.total_data_length * percent)/100);
            result = MMISRVAUD_Seek(handle, seek_offset);                
            //SCI_TRACE_LOW:"[MMIASP]: HandleProgressBarTpKey, percent = %d,seek_offset = %d, result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2050_112_2_18_2_2_47_146,(uint8*)"ddd", percent, seek_offset, result);
            DisplayASPWinProgress(FALSE);
            DisplayASPWinProgressTime(FALSE);
            CreateProgressTimer();
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }           

    return result;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StartTpDragTimer(MMI_WIN_ID_T     win_id)
{
    //SCI_TRACE_LOW:"[MMIASP]: StartTpDragTimer"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2074_112_2_18_2_2_48_147,(uint8*)"");
    
    if (0 == s_asp_tp_drag_timer)
    {
        s_asp_tp_drag_timer = MMK_CreateWinTimer(win_id,MMIASP_TP_DRAG_PERIOD,FALSE);
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: StartTpDragTimer: the timer has started!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2082_112_2_18_2_2_48_148,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
LOCAL void StopTpDragTimer(void)
{
    if (0 < s_asp_tp_drag_timer)
    {
        MMK_StopTimer(s_asp_tp_drag_timer);
        s_asp_tp_drag_timer = 0;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIASP]: StopTpDragTimer: the timer has stop!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2099_112_2_18_2_2_48_149,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : handle play window touch panel key
//  Global resource dependence : 
//  Author: paul.huang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlayWinTpKeyDown(
                                         MMI_WIN_ID_T     win_id,
                                         DPARAM     param
                                         )
{        
    GUI_RECT_T      progress_bar_rect   = {0};
    GUI_POINT_T     tp_point = {0};
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    
    //SCI_TRACE_LOW:"[MMIASP]: HandlePlayWinTpKeyDown"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2116_112_2_18_2_2_48_150,(uint8*)"");

    //get tp point
    tp_point.x = MMK_GET_TP_X(param);
    tp_point.y = MMK_GET_TP_Y(param);
    
    GUIOWNDRAW_GetDisplayRect(MMIASP_PROGRESS_OWNER_DRAW_CTRL_ID, &progress_bar_rect);

    if (GUI_PointIsInRect(tp_point,progress_bar_rect) )
    {
        result = HandleProgressBarTpKey(tp_point.x);

        if(MMI_RESULT_TRUE == result)
        {
            StartTpDragTimer(win_id);
        }
    }
    
    return (result);
}

/*****************************************************************************/
// Description : HandlePlayWinMsg
// Global resource dependence : none
// Author: byte.guo
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlayWinMsg(
                                     MMI_WIN_ID_T     win_id, 
                                     MMI_MESSAGE_ID_E msg_id, 
                                     DPARAM           param 
                                     )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    MMISRVAUD_REPORT_RESULT_E *result_ptr = PNULL;
    MMISRV_HANDLE_T     audio_handle = MMIASP_GetPlayHandle();  
    MMIASP_APPLET_T*        applet_ptr  =(MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    MMIASP_MSG_INFO_T*  cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, applet_ptr->cur_gui_list_index);
    if(PNULL == applet_ptr)
    {
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP HandlePlayWinMsg MSG_ID = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2159_112_2_18_2_2_48_151,(uint8*)"d", msg_id);
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetFormParam(); 
        if(PNULL != cur_node_ptr)
        {
            MMIASP_Play((const wchar*)cur_node_ptr->record_path);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIASP play win null code , index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2172_112_2_18_2_2_48_152,(uint8*)"d", applet_ptr->cur_gui_list_index);
        }

        InitButtonOperation(applet_ptr);
        MMIASP_GetTimeInfo(applet_ptr);
        applet_ptr->process_time = 0;
        UpdatePlayDetailInfo();
        //MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;        
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        OKCallback();
        break;

    case MSG_LCD_SWITCH:
        StopTpDragTimer();
        SetFormParam();
        UpdateControlButton(FALSE);
        break;
        
    case MSG_LOSE_FOCUS:
        StopTpDragTimer();
//        /* lost focus that will stop play record */
//        if (MMIASP_STATE_PLAY == MMIASP_GetPlayStatus() ||
//            MMIASP_STATE_PAUSE == MMIASP_GetPlayStatus())
//        {
//            MMIASP_Stop();
//            UpdatePlayPauseButton(FALSE);
//        }
//        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        break;

    case MSG_GET_FOCUS: 
        if (0 == MMIASP_GetListMsgTotalNum(applet_ptr))
        {
            MMK_CloseWin(win_id);
        }
        else
        {
            MMIASP_GetTimeInfo(applet_ptr);
            UpdatePlayDetailInfo();
            UpdateControlButton(FALSE);
        }
        break;
        
    case MSG_APP_UPSIDE:
    case MSG_APP_DOWNSIDE: 
    case MSG_APP_UP:
    case MSG_APP_DOWN:
        VolumeCallback();
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
	case MSG_APP_MENU:	
        {
            MMI_WINDOW_TABLE_CREATE_T create = {0};
            MMIAPICOM_ClosePanelChildWin();
            create.win_table_ptr = MMIASP_MSG_PLAY_OPTION_WIN_TAB;
            create.applet_handle = applet_ptr->caf_applet.app_handle;
            MMK_CreateWinTable( &create );
			//GUIMENU_CreatDynamic(PNULL, MMIASP_MSG_PLAY_OPTION_WIN_TAB, MMIASP_MSG_PLAY_WIN_OPT_MENU_CTRL_ID, GUIMENU_STYLE_POPUP); 
        }
        break;
        
    case MSG_APP_LEFT:
        LeftCallback();
        break;
        
    case MSG_APP_RIGHT:
        RightCallback();
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_ASP_PLAY_CALL_BACK:
        //SCI_TRACE_LOW:"[MMIASP]: HandlePlayWinMsg MSG_RECORD_PLAY_CALL_BACK "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2255_112_2_18_2_2_48_153,(uint8*)"");
        result_ptr = (MMISRVAUD_REPORT_RESULT_E  *)param;

        if(PNULL != result_ptr)
        {
            //MMIDEFAULT_TurnOnBackLight();
            
            //SCI_TRACE_LOW:"[MMIASP]: HandlePlayWinMsg *result_ptr = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2262_112_2_18_2_2_48_154,(uint8*)"d", *result_ptr);

            if (MMISRVAUD_REPORT_RESULT_SUCESS != (*result_ptr))
            {
                if((PNULL != cur_node_ptr)
                    && (PNULL != cur_node_ptr->record_path)
                    && !MMIAPIFMM_IsFileExist((const wchar*)cur_node_ptr->record_path,MMIAPICOM_Wstrlen(cur_node_ptr->record_path)))
                {
                    MMIPUB_OpenAlertFailWin(TXT_COM_FILE_NO_EXIST);  
                }
                else
                {
                    MMIPUB_OpenAlertFailWin(TXT_COMMON_PLAY_ERROR);  
                }
            }
            MMIASP_Stop();
            if(MMK_IsFocusWin(MMIASP_MAIN_PLAY_WIN_ID))
            {
                UpdatePlayPauseButton(TRUE);
                DisplayASPWinProgress(FALSE);
                DisplayASPWinProgressTime(FALSE);
            }
            //MMIDEFAULT_AllowOpenKeylocWin(TRUE);
            // 如果是自动播放..............
            if(MMIASP_AUTO_PLAY_MESSEAGES == applet_ptr->start_type)
            {
                RightCallback();
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIASP MSG_ASP_PLAY_CALL_BACK return msg"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2293_112_2_18_2_2_48_155,(uint8*)"");
        }
        break;
      
    case MSG_CLOSE_WINDOW:
        // 如果没有了未读的留言，并打开了提示新留言的窗口，则关掉它....
        // 如果还有 未读留言的数目可能发生了变化，需要先关闭，再打开之....
        if(MMK_IsOpenWin(MMIASP_NEWMSG_WIN_ID))
        {
            MMK_CloseWin(MMIASP_NEWMSG_WIN_ID);
            /*在MMIASP_ShowNewAspMsgWin函数中有对MMIASP_MSG_LIST_WIN_ID 发MMIASP_MSG_UPDATELIST消息，
            使答录机列表窗口得到更新, 假如有谁去掉MMIASP_ShowNewAspMsgWin, 请注意该项事宜*/
            MMIASP_ShowNewAspMsgWin();
        }
        else
        {
            MMK_SendMsg(MMIASP_MSG_LIST_WIN_ID, MMIASP_MSG_UPDATELIST, PNULL);
        }
        MMIASP_Stop();
        StopTpDragTimer();
        
        if(audio_handle)
        {
            MMISRVMGR_Free(audio_handle);
            MMIASP_SetPlayHandle(0);
        }

        if(MMIASP_AUTO_PLAY_MESSEAGES == applet_ptr->start_type)
        {
            MMK_CloseApplet(SPRD_ANSMACHINE_APPLET_ID);
        }
        //MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        break;
   case MSG_TIMER:
        {
            if(applet_ptr->progress_time_id == *(uint8*)param)
            {
                if(MMK_IsFocusWin(win_id))
                {
                  DisplayASPWinProgress(FALSE);
                  DisplayASPWinProgressTime(FALSE);
                }
                CreateProgressTimer();
            }
            else if(s_asp_tp_drag_timer == *(uint8*)param)
            {
                MMI_TP_STATUS_E tp_status = MMI_TP_NONE;
                GUI_POINT_T      tp_point              = {0};

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
                MMK_GetLogicTPMsg(&tp_status, &tp_point);   
#endif
                HandleProgressBarTpKey(tp_point.x);                
                StopTpDragTimer();                
                StartTpDragTimer(win_id);
            }
        }
        break; 

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:
        HandlePlayWinTpKeyDown(win_id, param);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        StopTpDragTimer();
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}
/*****************************************************************************/
// Description : select fixed ring
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL void  DefaultSetFixedRingID(uint16 id)
{
    MMI_CTRL_ID_T       label_ctrl_id = MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID;
    MMIASP_APPLET_T*    applet_ptr    = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    MMI_STRING_T        text           = {0};
    MMI_TEXT_ID_T       string_id      = TXT_NULL;

    // 先放到这里边一下，待用户确定后再存到nv中
    if(PNULL != applet_ptr)
    {
        applet_ptr->nv_info.audio_type = MMIASP_SETTING_FIX_AUDIO;
        applet_ptr->nv_info.fixed_ring_id = id;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP please check applet!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2384_112_2_18_2_2_48_156,(uint8*)"");
    }
    
    // 给设置的label控件更新内容
    string_id = MMIASP_GetFixedAnswerRingTextId(id);
    MMI_GetLabelTextByLang(string_id, &text);
    
    GUILABEL_SetText(label_ctrl_id, &text, FALSE);
}

/*****************************************************************************/
// Description : select fixed ring
// Global resource dependence : none
// Author: yaye.jiang
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleFixedAnswerRingSelectWinMsg(
                                                      MMI_WIN_ID_T        win_id, 
                                                      MMI_MESSAGE_ID_E    msg_id, 
                                                      DPARAM              param
                                                      )
{
    MMI_RESULT_E        recode       = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       list_ctrl_id = MMIASP_SETTING_FIXED_ANSWERRING_LIST_CTRL_ID;
    PRINGSELECT_CALLBACKFUNC callback_fun_ptr = (PRINGSELECT_CALLBACKFUNC)MMK_GetWinAddDataPtr(win_id);/*lint !e611*/
    uint16              list_index   = 0;
    
    switch(msg_id)
    {
        
        case MSG_OPEN_WINDOW:
            { 
                AppendFixedAnswerRing(list_ctrl_id);
                MMK_SetAtvCtrl(win_id, list_ctrl_id);
                
            }
            break;
        
        case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#ifndef MMI_PDA_SUPPORT        
		case MSG_CTL_OK:
#endif
        case MSG_APP_OK:
        case MSG_APP_WEB:
#ifdef MMI_PDA_SUPPORT	
			list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            MMIAPISET_PreviewRing(MN_DUAL_SYS_1, list_index+1,3,1,MMISET_RING_TYPE_ASP,PNULL);
#else
            {
                if(MMK_IsSingleClick(msg_id, param))
                {
                    break;
                }
                list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
                if(PNULL != callback_fun_ptr)
                {
                    callback_fun_ptr(TRUE, list_index, PNULL);
                }
                else
                {
                    DefaultSetFixedRingID(list_index);
                }
                MMK_CloseWin(MMIASP_SETTING_RING_WIN_ID);
                MMK_CloseWin(win_id);
            }
#endif	
            break;

#ifdef MMI_PDA_SUPPORT
		case MSG_CTL_OK:
			{
                list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
                if(PNULL != callback_fun_ptr)
                {
                    callback_fun_ptr(TRUE, list_index, PNULL);
                }
                else
                {
                    DefaultSetFixedRingID(list_index);
                }
                MMK_CloseWin(MMIASP_SETTING_RING_WIN_ID);
                MMK_CloseWin(win_id);
            }
		break;
#endif
        case MSG_CTL_LIST_MOVETOP:
        case MSG_CTL_LIST_MOVEBOTTOM:
        case MSG_CTL_LIST_MOVEUP:
        case MSG_CTL_LIST_MOVEDOWN:
            list_index = GUILIST_GetCurItemIndex(list_ctrl_id);
            MMIAPISET_PreviewRing(MN_DUAL_SYS_1, list_index+1,3,1,MMISET_RING_TYPE_ASP,PNULL);
            break;
        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
            break;

        case MSG_CLOSE_WINDOW:
            MMIAPISET_StopAppRing();
            break;
			
        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    
    return recode;
    
}


/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL uint16 GetAnswerPhoneIndex(MN_DUAL_SYS_E dual_sys)//current sim
{
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    uint16               cur_index  = 0;

    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        if(cur_nv.asp_flag[dual_sys])
        {
            cur_index = 0;
        }
        else
        {
            cur_index = 1;
        }
    }
  
    return cur_index;
    
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetAnswerPhoneFlag(
                              MMIASP_NV_SETTING_T* set_nv_ptr,
                              MN_DUAL_SYS_E dual_sys,
                              uint16 index
                              )
{
    if(PNULL == set_nv_ptr || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return;
    }

    if(0 == index)
    {
        set_nv_ptr->asp_flag[dual_sys]= TRUE;
    }
    else
    {
        set_nv_ptr->asp_flag[dual_sys] = FALSE;
    }
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL uint16 GetRecordTimeIndex(void)
{
    MMIASP_RECORD_TIME_E recordTime = MMIASP_RECORD_TIME_3M;
    uint16               cur_index  = 0;
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    recordTime = cur_nv.record_time;
    
    cur_index = (uint16)recordTime;
    return cur_index;
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetRecordTime(
                         MMIASP_NV_SETTING_T* set_nv_ptr,
                         uint16 index
                         )
{
    MMIASP_RECORD_TIME_E recordTime = MMIASP_RECORD_TIME_3M;
    
    if(PNULL == set_nv_ptr)
    {
        return;
    }
    
    recordTime = (MMIASP_RECORD_TIME_E)index;
    
    set_nv_ptr->record_time = recordTime;
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL uint16 GetWaitTimeIndex(void)
{
    MMIASP_WAIT_TIME_E waitTime = MMIASP_WAIT_TIME_10S;
    uint16               cur_index  = 0;
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    waitTime = cur_nv.wait_time;
    
    cur_index = (uint16)waitTime;
    return cur_index;
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetWaitTime(
                       MMIASP_NV_SETTING_T* set_nv_ptr,
                       uint16 index
                       )
{
    MMIASP_WAIT_TIME_E waitTime = MMIASP_WAIT_TIME_10S;

    if(PNULL == set_nv_ptr)
    {
        return;
    }
    
    waitTime = (MMIASP_WAIT_TIME_E)index;
    
    set_nv_ptr->wait_time = waitTime;
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL uint16 GetSpeakerFlagIndex(void)
{
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    uint16               cur_index  = 0;
    if(cur_nv.speaker_flag)
    {
        cur_index = 0;
    }
    else
    {
        cur_index = 1;
    }
    
    return cur_index;
    
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetSpeakerFlag(
                          MMIASP_NV_SETTING_T* set_nv_ptr,
                          uint16 index
                          )
{
    if(PNULL == set_nv_ptr)
    {
        return;
    }
    if(0 == index)
    {
        set_nv_ptr->speaker_flag = TRUE;
    }
    else
    {
        set_nv_ptr->speaker_flag = FALSE;
    }
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E GetRecordSaveStorage(void)
{
    MMIFILE_DEVICE_E     device  = MMI_DEVICE_SDCARD;
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    
    if(MMIAPIFMM_GetDeviceTypeStatus(cur_nv.record_save_dev))
    {
        device = cur_nv.record_save_dev; 
    }
    else
    {
        device =  MMIAPIFMM_GetValidateDevice(device);
    }
    
    return device;
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetRecordSavePath(
                             MMIASP_NV_SETTING_T* set_nv_ptr,
                             MMIFILE_DEVICE_E device
                             )
{
    if(PNULL != set_nv_ptr)
    {
        set_nv_ptr->record_save_dev = device;
    }
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void InitSettingAllLabels(void)
{
    
    MMI_STRING_T        text            = {0};
    wchar               name[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    MMIFILE_DEVICE_E    valid_dev = MMI_DEVICE_NUM;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 i = MN_DUAL_SYS_1;
    
    MMI_CTRL_ID_T      ctrl_id[] = 
    {        
        MMIASP_SETTING_FLAG_SIM1_LABEL_CTRL_ID,
        MMIASP_SETTING_FLAG_SIM2_LABEL_CTRL_ID,
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)    
        MMIASP_SETTING_FLAG_SIM3_LABEL_CTRL_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)   
        MMIASP_SETTING_FLAG_SIM4_LABEL_CTRL_ID,
#endif
    };
    MMI_CTRL_ID_T      form_id[] = 
    {
        MMIASP_SETTING_FLAG_SIM1_FORM_CTRL_ID,
        MMIASP_SETTING_FLAG_SIM2_FORM_CTRL_ID,   

#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)    
        MMIASP_SETTING_FLAG_SIM3_FORM_CTRL_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)   
        MMIASP_SETTING_FLAG_SIM4_FORM_CTRL_ID,
#endif
    };

     MMI_CTRL_ID_T      lable_id[] = 
    {
        TXT_ANSWER_PHONE_SIM1,
        TXT_ANSWER_PHONE_SIM2,   

#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)    
        TXT_ANSWER_PHONE_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)   
        TXT_ANSWER_PHONE_SIM4,
#endif
    };

    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {        
        GUIFORM_SetStyle(form_id[i],GUIFORM_STYLE_UNIT);
        MMI_GetLabelTextByLang(lable_id[i], &text);
        GUILABEL_SetText(ctrl_id[i], &text, FALSE);        
    }      
#else
    // 开关
    GUIFORM_SetStyle(MMIASP_SETTING_FLAG_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_MMIASP_ANSWER_PHONE, &text);
    GUILABEL_SetText(MMIASP_SETTING_FLAG_LABEL_CTRL_ID, &text, FALSE);
#endif

    //答录语
    GUIFORM_SetStyle(MMIASP_SETTING_AUDIO_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_COMM_ANSWER, &text);
    GUILABEL_SetText(MMIASP_SETTING_AUDIO_LABEL_CTRL_ID, &text, FALSE);
    
    text.wstr_ptr = name;
    text.wstr_len = GetAnswerRingName(name);
    GUILABEL_SetText(MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID, &text, FALSE);

    //答录机留言长度
    GUIFORM_SetStyle(MMIASP_SETTING_AUDIO_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_MMIASP_MSG_TIME, &text);
    GUILABEL_SetText(MMIASP_SETTING_AUDIO_TIME_LABEL_CTRL_ID, &text, FALSE);
    
    //n秒开始答录
    GUIFORM_SetStyle(MMIASP_SETTING_WAIT_TIME_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_MMIASP_WAIT_TIME, &text);
    GUILABEL_SetText(MMIASP_SETTING_WAIT_TIME_LABEL_CTRL_ID, &text, FALSE);
    //扬声器
    GUIFORM_SetStyle(MMIASP_SETTING_SPEAKER_FLAG_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_MMIASP_SPEAKER, &text);
    GUILABEL_SetText(MMIASP_SETTING_SPEAKER_FLAG_LABEL_CTRL_ID, &text, FALSE);
    //留言存储位置
    valid_dev = MMIAPIFMM_GetFirstValidDevice();
    if(valid_dev >= MMI_DEVICE_NUM)
    {
        GUIFORM_SetChildDisplay(MMIASP_SETTING_FORM_CTRL_ID,MMIASP_SETTING_SAVE_PATH_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
    }
    
//#ifndef NANDBOOT_SUPPORT
//    GUIFORM_SetChildDisplay(MMIASP_SETTING_FORM_CTRL_ID,MMIASP_SETTING_SAVE_PATH_FORM_CTRL_ID,GUIFORM_CHILD_DISP_HIDE);
//#endif
    GUIFORM_SetStyle(MMIASP_SETTING_SAVE_PATH_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    MMI_GetLabelTextByLang(TXT_PRIOR_STORE_MEDIUM, &text);
    GUILABEL_SetText(MMIASP_SETTING_SAVE_PATH_LABEL_CTRL_ID, &text, FALSE);
    
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void AppendSwitchFlagDropDownListData( void )
{
    //MMI_STRING_T flag_dropdownlist[ASP_SWITCH_FLAG_TYPE_NUM] = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    MMI_CTRL_ID_T      flag_dropdown_id[]                    = 
    {
        MMIASP_SETTING_FLAG_SIM1_DROPDOWNLIST_CTRL_ID,
        MMIASP_SETTING_FLAG_SIM2_DROPDOWNLIST_CTRL_ID,
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)    
        MMIASP_SETTING_FLAG_SIM3_DROPDOWNLIST_CTRL_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)   
        MMIASP_SETTING_FLAG_SIM4_DROPDOWNLIST_CTRL_ID,
#endif
    };
#else
    MMI_CTRL_ID_T      flag_dropdown_id[]                    = 
    {
        MMIASP_SETTING_FLAG_DROPDOWNLIST_CTRL_ID
    };
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE    //NEWMS00162552
	MMI_CTRL_ID_T      lable_id[] = 
    {
        TXT_ANSWER_PHONE_SIM1,
        TXT_ANSWER_PHONE_SIM2,   

#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)    
        TXT_ANSWER_PHONE_SIM3,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)   
        TXT_ANSWER_PHONE_SIM4,
#endif
    };
#else	//NEWMS00162552
	MMI_CTRL_ID_T      lable_id[] = 
	{
	TXT_MMIASP_ANSWER_PHONE
	};
#endif
	MMI_TEXT_ID_T      flag_text[ASP_SWITCH_FLAG_TYPE_NUM] = {TXT_OPEN, TXT_CLOSE};
    uint32             i                                   = 0;
    uint32             j                                   = 0;
    uint16             cur_index                           = 0;

	for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {       
        for(j= 0; j< ASP_SWITCH_FLAG_TYPE_NUM; j++)
        {
            GUISETLIST_AddItemById(flag_dropdown_id[i], flag_text[j]);
			GUISETLIST_SetTitleTextId(flag_dropdown_id[i], lable_id[i]);
        }
        cur_index = GetAnswerPhoneIndex(i);
        GUISETLIST_SetCurIndex(flag_dropdown_id[i], cur_index);
    } 	
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void AppendSettingAllDropDownListData( MMIFILE_DEVICE_E *device_arrary_ptr)
{
    //MMI_STRING_T audio_time_dropdownlist[ASP_AUDIO_TIME_TYPE_NUM]     = {0};
    //MMI_STRING_T wait_time_dropdownlist[ASP_WAIT_TIME_TYPE_NUM]       = {0};
    //MMI_STRING_T speaker_flag_dropdownlist[ASP_SPEAKER_FLAG_TYPE_NUM] = {0};
    MMI_STRING_T save_path_dropdownlist[ASP_SAVE_PATH_TYPE_NUM]       = {0};
    
   
    MMI_TEXT_ID_T  audio_time_text[ASP_AUDIO_TIME_TYPE_NUM]    = {TXT_COMM_1_MINUTE, TXT_MMIASP_3_MINUTES, TXT_COMM_5_MINUTES, TXT_COMM_10MINUTES};
    MMI_TEXT_ID_T  wait_time_text[ASP_WAIT_TIME_TYPE_NUM]      =  {TXT_COMM_1_SECOND, TXT_COMM_5_SECONDS, TXT_COMM_10_SECONDS, TXT_MMIASP_15_SECONDS, TXT_COMM_20_SECONDS};
    MMI_TEXT_ID_T  speaker_flag_text[ASP_SPEAKER_FLAG_TYPE_NUM]= {TXT_OPEN, TXT_CLOSE};
    //MMI_TEXT_ID_T  save_path_text[ASP_SAVE_PATH_TYPE_NUM]      = {TXT_MMIASP_DEV_U, TXT_COMMON_SD_CARD};

    //MMI_CTRL_ID_T      audio_time_dropdown_id                                  = MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID;
    //MMI_CTRL_ID_T      wait_time_dropdown_id                                   = MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID;
    //MMI_CTRL_ID_T      speaker_flag_dropdown_id                                = MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID;
    //MMI_CTRL_ID_T      save_path_dropdown_id                                   = MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID;
    
    uint32             i                                                       = 0;
    uint16             cur_index                                               = 0;
    uint16             save_device_num                                         = 0;
    //MMI_STRING_T       string = {0};
    MMIFILE_DEVICE_E   device = GetRecordSaveStorage();
    
    // 答录机开关
    AppendSwitchFlagDropDownListData();

	// 答录机留言长度
	for(i= 0; i < ASP_AUDIO_TIME_TYPE_NUM; i++)
    {
		GUISETLIST_AddItemById(MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID,audio_time_text[i]);
	}
	cur_index = GetRecordTimeIndex();
	GUISETLIST_SetCurIndex(MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID, cur_index);
	GUISETLIST_SetTitleTextId(MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID, TXT_MMIASP_MSG_TIME);

    // n秒开始答录
    for(i= 0; i < ASP_WAIT_TIME_TYPE_NUM; i++)
    {
        GUISETLIST_AddItemById(MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID, wait_time_text[i]);
    }
    cur_index = GetWaitTimeIndex();
    GUISETLIST_SetCurIndex(MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID, cur_index);
    GUISETLIST_SetTitleTextId(MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID, TXT_MMIASP_WAIT_TIME);
	
    // 扬声器
    for(i= 0; i < ASP_SPEAKER_FLAG_TYPE_NUM; i++)
    {
        GUISETLIST_AddItemById(MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID, speaker_flag_text[i]);
    }
    cur_index = GetSpeakerFlagIndex();
    GUISETLIST_SetCurIndex(MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID, cur_index);
	GUISETLIST_SetTitleTextId(MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID, TXT_MMIASP_SPEAKER);

    // 留言存储位置
    for(i= MMI_DEVICE_UDISK; i < MMI_DEVICE_NUM; i++)
    {
        if(MMIAPIFMM_GetDeviceTypeStatus((MMIFILE_DEVICE_E)i))
        {
            //SCI_MEMSET(&string, 0, sizeof(MMI_STRING_T));
			GUISETLIST_AddItemById(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID, MMIAPIFMM_GetDeviceName((MMIFILE_DEVICE_E)i));
            MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName((MMIFILE_DEVICE_E)i), &save_path_dropdownlist[save_device_num]);
            save_path_dropdownlist[save_device_num].wstr_len = MIN(save_path_dropdownlist[save_device_num].wstr_len, DROPDOWNLIST_STRING_MAX_NUM);
            if(device_arrary_ptr != PNULL)
            {
                *device_arrary_ptr++ = i;
            }
            if(i == device)
            {
                cur_index = save_device_num;
            }
            save_device_num++;
        }
    }
	GUISETLIST_SetCurIndex(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID, cur_index);
	GUISETLIST_SetTitleTextId(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID, TXT_PRIOR_STORE_MEDIUM);
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SaveAspSetting( MMIFILE_DEVICE_E device )
{
    uint16             cur_index = 0;
#ifndef MMI_MULTI_SIM_SYS_SINGLE
     MMI_CTRL_ID_T      flag_dropdown_id[] = 
    {
        MMIASP_SETTING_FLAG_SIM1_DROPDOWNLIST_CTRL_ID,
        MMIASP_SETTING_FLAG_SIM2_DROPDOWNLIST_CTRL_ID,
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)    
        MMIASP_SETTING_FLAG_SIM3_DROPDOWNLIST_CTRL_ID,
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)   
        MMIASP_SETTING_FLAG_SIM4_DROPDOWNLIST_CTRL_ID,
#endif
    } ;
#else
     MMI_CTRL_ID_T      flag_dropdown_id[] = 
    {
        MMIASP_SETTING_FLAG_DROPDOWNLIST_CTRL_ID,   
    } ;  
#endif
    //MMI_CTRL_ID_T      aduio_dropdown_id                        = MMIASP_SETTING_AUDIO_DROPDOWNLIST_CTRL_ID;
    //MMI_CTRL_ID_T      audio_time_dropdown_id                     = MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID;
    //MMI_CTRL_ID_T      msg_time_dropdown_id                       = MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID;
    //MMI_CTRL_ID_T      speaker_flag_dropdown_id                   = MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID;
    //MMI_CTRL_ID_T      save_path_dropdown_id                      = MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID;
    MMIASP_NV_SETTING_T set_nv                                    = MMIASP_GetSettingNvValue();
    MMIASP_APPLET_T*    applet_ptr                                = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    uint32 i = MN_DUAL_SYS_1;
    for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
    {
         // 开关
        cur_index = GUISETLIST_GetCurIndex(flag_dropdown_id[i]);
        SetAnswerPhoneFlag(&set_nv, i, cur_index);        
    }   
    // 答录语
    if(PNULL != applet_ptr)
    {
        set_nv.audio_type = applet_ptr->nv_info.audio_type;
        if(MMIASP_SETTING_MORE_AUDIO == set_nv.audio_type) 
        {
            set_nv.fixed_ring_id = 0;
            MMIAPICOM_Wstrncpy(set_nv.record_path_for_client, applet_ptr->nv_info.record_path_for_client, MMIFILE_FILE_NAME_MAX_LEN);
        }
        else
        {
            set_nv.fixed_ring_id = applet_ptr->nv_info.fixed_ring_id;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP please check applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_2984_112_2_18_2_2_49_157,(uint8*)"");
    }

	// 答录机留言长度
	cur_index = GUISETLIST_GetCurIndex(MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID);
    SetRecordTime(&set_nv, cur_index);
    
    // n秒开始答录
    cur_index = GUISETLIST_GetCurIndex(MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID);
    SetWaitTime(&set_nv, cur_index);

    
    // 扬声器
	cur_index = GUISETLIST_GetCurIndex(MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID);
    SetSpeakerFlag(&set_nv, cur_index);
    
    // 留言存储位置
    //cur_index = GUIDROPDOWNLIST_GetCurItemIndex(save_path_dropdown_id);
    SetRecordSavePath(&set_nv, device);
    
    MMIASP_SetSetting2NV(set_nv);
    
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: 
// Note:    
/*****************************************************************************/
LOCAL void  AppendRecordList(
                             MMI_WIN_ID_T win_id, 
                             MMI_CTRL_ID_T list_id
                             )
{
    uint16              list_num     = 0;
    uint16              item_index   = 0;
    MMIASP_APPLET_T*    applet_ptr   = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    GUILIST_ITEM_T      item_t       = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T item_data    = {0}; /*lint !e64*/

    item_t.item_style    =  GUIITEM_STYLE_TWO_LINE_ICON_TEXT_AND_TEXT;      
    item_t.item_data_ptr =  &item_data; 
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP AppendRecordList no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_3026_112_2_18_2_2_49_158,(uint8*)"");
        return;
    }
    
    if(!MMIASP_ReadSysFileWithApplet(applet_ptr))
    {
        //SCI_TRACE_LOW:"AppendRecordList ASP read sys file fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_3032_112_2_18_2_2_49_159,(uint8*)"");
    }
    
    list_num = MMIASP_GetListMsgTotalNum(applet_ptr);
    
    GUILIST_SetMaxItem(list_id, list_num, TRUE);
    if(0 < list_num)
    {
        MMIASP_SortRecordList();
        GUILIST_SetTitleIndexType(list_id, GUILIST_TITLE_INDEX_DEFAULT );
        for(item_index = 0; item_index < list_num; item_index++)
        {
            GUILIST_AppendItem(list_id, &item_t);
        }
    }
    else
    {
        GUILIST_SetTitleIndexType(list_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
    }

    GUILIST_SetCurItemIndex(list_id, applet_ptr->cur_gui_list_index);
    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE );
    

}
/*****************************************************************************/
// Description : handle new msg 
// Global resource dependence : none
// Author: byte.guo
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIdleNewAspMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        )
{
    MMI_RESULT_E    recode  = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (MMK_IsOpenWin(MMIMAIN_SHOWTIME_WIN_ID))
        {
            MMK_CloseWin(MMIMAIN_SHOWTIME_WIN_ID);
        }
        break;
        
    case MSG_FULL_PAINT:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
#endif
        {
            MMIPUB_SetWinSoftkey(win_id, STXT_SELECT, STXT_EXIT, FALSE);
        }
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
        {
            uint16 node_index = 0;
            uint16 new_msg_count = MMIAPIASP_GetNewRecordNum();
            if (0 == new_msg_count)
            {
                return MMI_RESULT_FALSE;
            }
            if (new_msg_count>1 )
            {
                recode = MMIAPIASP_EnterAsp();
            }
            else
            {
                if (MMIAPIASP_GetLastestUnreadRecord(PNULL, &node_index))
                {
                    recode = MMIAPIASP_OpenPlayWinFromExternal(node_index);
                }
            }
            MMK_CloseWin(win_id);
        }
        
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
#ifdef MMI_KEY_LOCK_SUPPORT
        if (MMIAPIKL_IsPhoneLocked())
        {
            MMK_CloseWin(win_id);
        }
        else
#endif
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_CLOSE_WINDOW:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_LOSE_FOCUS:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        //MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    
    return (recode);
}
/*****************************************************************************/
// Description : show nes msg win
// Global resource dependence : none
// Author: byte.guo
// Note:    
/*****************************************************************************/
PUBLIC void MMIASP_ShowNewAspMsgWin(void)
{
    uint16       msg_count  = 0;
    MMI_STRING_T prompt_str = {0};
    MMI_STRING_T tmp_prompt = {0};
    uint8        disp_str_len   = 0;
    uint8        new_msg_num[6] = {0};
    wchar        wchar_new_msg_num[6] = {0};
    uint32       time_out = 0;
    uint32       wchar_new_msg_num_len = 0;
    
    MMI_WIN_PRIORITY_E  win_priority = WIN_LOWEST_LEVEL;
    MMI_WIN_ID_T        alert_win_id = MMIASP_NEWMSG_WIN_ID;
    
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    
    if(PNULL != applet_ptr)
    {
        msg_count = applet_ptr->new_msg_num;
    }

    //SCI_TRACE_LOW:"MMIASP MMIASP_ShowNewAspMsgWin msg_count = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_3174_112_2_18_2_2_50_160,(uint8*)"d", msg_count);

    // 如果打开了答录列表界面，则更新一下列表....
    if(MMK_IsOpenWin(MMIASP_MSG_LIST_WIN_ID))
    {
         MMK_SendMsg(MMIASP_MSG_LIST_WIN_ID, MMIASP_MSG_UPDATELIST, PNULL);
    }

    if (0 == msg_count)
    {
        return;
    }

#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_ASP);
#endif       
    /* comb string  */
    sprintf((char *) new_msg_num, "%d", msg_count);
    MMIAPICOM_StrToWstr(new_msg_num, wchar_new_msg_num);
    wchar_new_msg_num_len = MMIAPICOM_Wstrlen((const wchar*)wchar_new_msg_num);
    if(1 < msg_count)
    {
        MMI_GetLabelTextByLang(TXT_MMIASP_NEW_MULT_MSGS, &tmp_prompt);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MMIASP_NEW_MSG, &tmp_prompt);
    }
    // the total string length
    disp_str_len =  wchar_new_msg_num_len + tmp_prompt.wstr_len + 1;
    
    // allocate memory
    prompt_str.wstr_ptr = (wchar*)SCI_ALLOC_APP(disp_str_len * sizeof(wchar));
    if(PNULL == prompt_str.wstr_ptr)
    {
        return;
    }
    
    // copy the num
    MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, wchar_new_msg_num,  wchar_new_msg_num_len);
    
    prompt_str.wstr_len = wchar_new_msg_num_len;
    
    // copy the prompt string
    MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr + prompt_str.wstr_len, tmp_prompt.wstr_ptr, tmp_prompt.wstr_len);
    
    prompt_str.wstr_len += tmp_prompt.wstr_len;

    if (MMK_IsFocusWin(MMIASP_NEWMSG_WIN_ID))
    {
        // set to window
        MMIPUB_SetAlertWinTextByPtr(MMIASP_NEWMSG_WIN_ID,&prompt_str,PNULL,TRUE);
        
        // play the ring
        // MMK_PostMsg(MMIASP_NEWMSG_WIN_ID, MSG_SMS_PLAY_RING, PNULL,0);
    }
    else
    {
        MMK_CloseWin(MMIASP_NEWMSG_WIN_ID);
#ifdef SCREENSAVER_SUPPORT        
        if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
        {
            MMIAPIIDLESS_CloseScreenSaver();
        }
#endif        
#ifdef MMIWIDGET_SUPPORT
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
        {
            MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_AUTOANSWER_LOG_ID, FALSE, TRUE);
        }
        else
#endif
            MMIPUB_OpenAlertWinByTextPtr(&time_out,&prompt_str,PNULL,IMAGE_PUBWIN_NEWMSG,&alert_win_id,&win_priority,
               MMIPUB_SOFTKEY_CUSTOMER,HandleIdleNewAspMsg);
    }

    
    if (PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr = PNULL;
    }
    
    return;
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:      
/*****************************************************************************/
LOCAL void SetListMarkMenu(void)
{
    MMI_WIN_ID_T  win_id       = MMIASP_MSG_OPTION_WIN_ID;
    MMI_CTRL_ID_T list_ctrl_id = MMIASP_MSG_LIST_CTRL_ID;
    const GUILIST_ITEM_T *item_ptr   = PNULL;
    
    uint16 totalnum  = 0;
    uint16 selectnum = 0;
    uint16 curindex = 0;
    
    totalnum  = GUILIST_GetTotalItemNum(list_ctrl_id);
    curindex  = GUILIST_GetCurItemIndex(list_ctrl_id);  
    
    // 得到list上选中的条目数
    selectnum = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
    
    item_ptr = GUILIST_GetItemPtrByIndex(list_ctrl_id, curindex);
    
    if(PNULL != item_ptr)
    {
        // 如果当前的条目被选中
        if(GUILIST_GetItemState(item_ptr, GUIITEM_STATE_SELECTED))
        {
            // 灰化标记
            MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MARK_OPT, MMIASP_MENU_MARK_SINGLE_ITEM_ID, TRUE);
            if(selectnum >= totalnum)
            {   // 灰化标记全部
                MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MARK_OPT, MMIASP_MENU_MARK_ALL_ITEM_ID, TRUE);
            }
        }
        else// 如果当前的条目没有被选中
        {
            // 灰化取消标记
            MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MARK_OPT, MMIASP_MENU_UNMARK_SINGLE_ITEM_ID, TRUE);
            // 如果一条也没有被选中，灰化取消全部标记
            if(0 == selectnum)
            {
                MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MARK_OPT, MMIASP_MENU_UNMARK_ALL_ITEM_ID, TRUE);
            }
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP SetListMarkMenu has error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_3306_112_2_18_2_2_50_161,(uint8*)"");
    }
}
/*****************************************************************************/
//  Description : handle  list mark option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleListMarkOperation(
                                   MMI_CTRL_ID_T   list_ctrl_id,
                                   BOOLEAN         mark_single
                                   )
{
    uint16      mark_num  = 0;
    uint16      cur_index = 0;
    
    uint16      item_num  = GUILIST_GetTotalItemNum(list_ctrl_id);
    
    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
    if(0 == mark_num)
    {
        //set list enable mark
        GUILIST_SetTextListMarkable(list_ctrl_id, TRUE);
        //set mark max number
        GUILIST_SetMaxSelectedItem(list_ctrl_id, item_num);
    }
    //get item index
    cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    //mark item
    if(mark_single)
    {
        GUILIST_SetSelectedItem(list_ctrl_id, cur_index, TRUE);
    }
    else
    {
        GUILIST_SetAllSelected(list_ctrl_id, TRUE);
    }
}
/*****************************************************************************/
//  Description : handle  list unmark option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleListUnMarkOperation(
                                     MMI_CTRL_ID_T   list_ctrl_id,
                                     BOOLEAN         unmark_single
                                     )
{
    uint16  mark_num  = 0;
    uint16  cur_index = 0;
    
    if(unmark_single)
    {    //get item index
        cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);
        //cancel mark item
        GUILIST_SetSelectedItem(list_ctrl_id, cur_index, FALSE);
        //get mark num
        mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, PNULL, 0);
        if (0 == mark_num)
        {
            //set list enable mark
            GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
        }
    }
    else
    {
        GUILIST_SetAllSelected(list_ctrl_id, FALSE);
        GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
    }
    
}

/*****************************************************************************/
//  Description : to handle the asp message list of delete query window    
//  Global resource dependence : 
//  Author:koki gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleQueryWinMsg(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T  query_win_id = MMIASP_QUERY_WIN_ID;
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK: 
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        MMIPUB_CloseQuerytWin(&query_win_id);
        HandleMSGListDeleteOperation();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : to handle the asp playing message of delete query window    
//  Global resource dependence : 
//  Author:koki gao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePlayMsgDelQueryWin(
                                            MMI_WIN_ID_T        win_id,    //IN:
                                            MMI_MESSAGE_ID_E    msg_id,    //IN:
                                            DPARAM                param    //IN:
                                            )
{
    MMI_RESULT_E  recode = MMI_RESULT_TRUE;
    MMI_WIN_ID_T  query_win_id = MMIASP_QUERY_WIN_ID;
    switch (msg_id)
    {
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        MMIPUB_CloseQuerytWin(&query_win_id);
        HandleMSGDeleteOperation();
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        recode = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : handle  
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleAspMenuOperation(
                                  MMI_WIN_ID_T      win_id,    
                                  MMI_CTRL_ID_T     menu_ctrl_id
                                  )
{
    MMI_MENU_ID_T       menu_id  = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMIFILE_FILE_INFO_T file_info = {0};
    MMIASP_MSG_INFO_T*  cur_node = PNULL;
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    uint8               num_len = 0;
    MMI_CTRL_ID_T       list_ctrl_id = MMIASP_MSG_LIST_CTRL_ID;
    uint16              full_path_len = 0;
    MN_DUAL_SYS_E sim_type = MN_DUAL_SYS_MAX;
    MMI_STRING_T string_to = {0};
    MMI_WIN_ID_T        query_win_id = MMIASP_QUERY_WIN_ID;
    
    if(PNULL == applet_ptr)
    {
        return;
    }
    cur_node = MMIASP_GetCurNodeByGuiIndex(applet_ptr, applet_ptr->cur_gui_list_index);
    //SCI_TraceLow:"MMIASP HandleAspMenuOperation index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_3471_112_2_18_2_2_50_162,(uint8*)"d", applet_ptr->cur_gui_list_index);

    if (PNULL != cur_node)
    {    
        //MMIAPICOM_WstrToStr(cur_node->call_num,number);
        num_len = (uint8)strlen((char*)cur_node->call_num); 
    }
    else
    {
        //SCI_TraceLow:"HandleAspMenuOperation null Node"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_3480_112_2_18_2_2_50_163,(uint8*)"");
    }

    //SCI_TraceLow:"MMIASP num_len  = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_3483_112_2_18_2_2_50_164,(uint8*)"d", num_len);
    
    GUIMENU_GetId(menu_ctrl_id, &group_id, &menu_id);
    
    switch(menu_id)
    {
        case MMIASP_MENU_CALL_AUDIO_ITEM_ID:
            if(PNULL != cur_node)
            {
                MMIAPICC_MakeCall(MMIAPISET_GetActiveSim(), cur_node->call_num, num_len, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
            }
            MMK_CloseWin(win_id);
            break; 
#ifdef VT_SUPPORT          
        case MMIASP_MENU_CALL_VIDEO_ITEM_ID:
            if(PNULL != cur_node)
            {
                MMIAPICC_MakeCall(MMIAPISET_GetActiveSim(), cur_node->call_num, num_len, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_VIDEO_CALL, PNULL);
            }
            MMK_CloseWin(win_id);
            break;
#endif
#ifdef MMI_IP_CALL_SUPPORT
        case MMIASP_MENU_CALL_IP_ITEM_ID:
            if(PNULL != cur_node)
            {
                MMIAPICC_MakeCall(MMIAPISET_GetActiveSim(), cur_node->call_num, num_len, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_IP_CALL, PNULL);
            }
            MMK_CloseWin(win_id);
            break;
#endif

            /* Write and send new message by the phone number */
        case MMIASP_MENU_MSG_SMS_ITEM_ID:
            {
                if ( (num_len > 0)&&(PNULL != cur_node))
                {
                     //调用sms接口函数
                    MMIAPISMS_WriteNewMessage(MMIAPISET_GetActiveSim(), PNULL,cur_node->call_num, num_len);
                }
                else
                {
                    //提示窗口没有号码
#ifdef MMI_PDA_SUPPORT
                    MMIPUB_DisplayTipsIDEx(PNULL, TXT_CL_EMPTY_NUM, MMIPUB_TIPS_DIS_BOTTOM, PNULL);
#else
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
#endif
                }
                MMK_CloseWin(win_id);            
            }
            break;
        
            // 回复彩信...
        case MMIASP_MENU_MSG_MMS_ITEM_ID:
            {
                if ( (num_len > 0)&&(PNULL != cur_node))
                {
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));

                    SCI_MEMSET(string_to.wstr_ptr, 0x00, (num_len * sizeof(wchar)));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, cur_node->call_num, num_len, num_len);
                    string_to.wstr_len = num_len;
                    MMIAPIMMS_AnswerMMS(sim_type, PNULL, &string_to);
                    SCI_FREE(string_to.wstr_ptr);
                    string_to.wstr_ptr = PNULL;
                }
                else
                {
#ifdef MMI_PDA_SUPPORT
                    MMIPUB_DisplayTipsIDEx(PNULL, TXT_CL_EMPTY_NUM, MMIPUB_TIPS_DIS_BOTTOM, PNULL);           
#else
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);  
#endif
                }
                MMK_CloseWin(win_id);
            }
            break;
        
            /* Delete the item which you have been select */
        case MMIASP_MENU_DELETE_ITEM_ID:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY,  &query_win_id, HandleQueryWinMsg);
            MMK_CloseWin(win_id);
            break;
        
        case MMIASP_MENU_MARK_SINGLE_ITEM_ID:
            HandleListMarkOperation(list_ctrl_id, TRUE);
            MMK_CloseWin(win_id);
            break;
        
            /* Undo above handle*/
        case MMIASP_MENU_UNMARK_SINGLE_ITEM_ID:
            HandleListUnMarkOperation(list_ctrl_id, TRUE);
            MMK_CloseWin(win_id);
            break;
        
            /* Select all item and mark them */
        case MMIASP_MENU_MARK_ALL_ITEM_ID:
            HandleListMarkOperation(list_ctrl_id, FALSE);
            MMK_CloseWin(win_id);
            break;
        
            /* Undo above handle*/
        case MMIASP_MENU_UNMARK_ALL_ITEM_ID:
            HandleListUnMarkOperation(list_ctrl_id, FALSE);
            MMK_CloseWin(win_id);
            break;
        
        
            /* Send mms message */
        case MMIASP_MENU_SEND_MMS_ITEM_ID:
            {
                if(PNULL != cur_node)
                {
                    full_path_len = MMIAPICOM_Wstrlen(cur_node->record_path);
                    if(MMIAPIFMM_IsFileExist(cur_node->record_path, full_path_len))
                    {
                        MMIAPIFMM_GetFileInfoFormFullPath(cur_node->record_path,full_path_len,&file_info);
                        MMIAPIMMS_EditMMSFromContent(PNULL, PNULL, &file_info, MMIFMM_FILE_TYPE_MUSIC);
                    }
                    else
                    {
                        MMIAPIMMS_EditMMSFromContent(PNULL, PNULL, PNULL, MMIFMM_FILE_TYPE_NORMAL);
                    }
                }
                MMK_CloseWin(win_id);
            }
            break;

#if defined(BLUETOOTH_SUPPORT)  
            /* Send  message by bluetooth */
        case MMIASP_MENU_SEND_BLUETOOTH_ITEM_ID:
            {
                
                if(PNULL != cur_node)
                {
                	MMIASP_SendRecordFile((const wchar*)cur_node->record_path);
                }
                MMK_CloseWin(win_id);
                break;				
            }
#endif
        
            /* Setting */
            case MMIASP_MENU_SETTING_ITEM_ID:
                {
                    if(!IsUDiskRunning())
                    {
                        MMI_WINDOW_TABLE_CREATE_T create = {0};
                        create.win_table_ptr = MMIASP_MSG_SETTING_WIN_TAB;
                        create.applet_handle = applet_ptr->caf_applet.app_handle;
                        
                        /* creat setting win */
                        MMK_CreateWinTable( &create );
                        MMK_CloseWin(win_id);
                    }
                }
                break;
            case MMIASP_MENU_HELP_ITEM_ID:
                {
                        MMI_WINDOW_TABLE_CREATE_T win_table_create = {0};
                        win_table_create.win_table_ptr = MMIASP_HELP_WIN_TAB;
                        win_table_create.applet_handle = applet_ptr->caf_applet.app_handle;
                        MMK_CreateWinTable( &win_table_create );
                        MMK_CloseWin(win_id);
                }
                break;
        default: 
            break;
       }
}
/*****************************************************************************/
// Description : EnableMsgOpt
// Global resource dependence : none
// Author: 
// Note:      
/*****************************************************************************/
LOCAL void EnableMsgOpt(MMIASP_APPLET_T*  applet_ptr)
{
    MMI_WIN_ID_T win_id = MMIASP_MSG_OPTION_WIN_ID;
    uint16 total_num = MMIASP_GetListMsgTotalNum(applet_ptr);
    uint16 mark_num = GUILIST_GetSelectedItemIndex(MMIASP_MSG_LIST_CTRL_ID, PNULL, 0);
    if(mark_num>=1  || 0 == total_num)
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_SEND_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_CALL_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_MSG_ITEM_ID, TRUE);
    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_SEND_ITEM_ID,FALSE);
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_CALL_ITEM_ID,FALSE);
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_MSG_ITEM_ID, FALSE);
    }
    
    if (0 == total_num )
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_DELETE_ITEM_ID,TRUE);
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_MARK_ITEM_ID,  TRUE);
    }
    else
    {
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_DELETE_ITEM_ID,FALSE);
        MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSG_OPT,MMIASP_MENU_MARK_ITEM_ID,  FALSE);
    }
    
}
/*****************************************************************************/
// Description : opt handle msg for list win
// Global resource dependence : none
// Author: 
// Note:      
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgListOptMsg(
                                        MMI_WIN_ID_T      win_id,    
                                        MMI_MESSAGE_ID_E  msg_id, 
                                        DPARAM            param
                                        )
{
    MMI_RESULT_E        result       = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       menu_ctrl_id = MMIASP_MSG_OPT_MENU_CTRL_ID;
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            EnableMsgOpt(applet_ptr);
            SetListMarkMenu();
            MMK_SetAtvCtrl(win_id, menu_ctrl_id);

            break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
        case MSG_CTL_MIDSK:
            HandleAspMenuOperation(win_id, menu_ctrl_id);
            break;
        
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
        default:
            result = MMI_RESULT_FALSE;
            break;
        
    }
        
        return result;
}

/*****************************************************************************/
// Description : opt handle msg for help win
// Global resource dependence : none
// Author: 
// Note:      
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleHelpWinMsg(
                                            MMI_WIN_ID_T        win_id, 
                                            MMI_MESSAGE_ID_E    msg_id, 
                                            DPARAM              param
                                            )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    MMI_STRING_T txtHelp = {0};
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
      MMI_GetLabelTextByLang(TXT_MMIASP_ANSWER_AUDIOHELP, &txtHelp);
      GUITEXT_SetString(MMIASP_HELP_TEXT_CTRL_ID, txtHelp.wstr_ptr, txtHelp.wstr_len, FALSE);
      MMK_SetAtvCtrl(win_id, MMIASP_HELP_TEXT_CTRL_ID);
      break;

    case MSG_NOTIFY_CANCEL:
      MMK_CloseWin(win_id);
      break;

    default:
      recode = MMI_RESULT_FALSE;
      break;
    }

    return recode;
}


#ifdef MMI_RECORD_SUPPORT
/*****************************************************************************/
// Description : ASP 插入录音的回调函数 
// Global resource dependence : none
// Author: 
// Note:    
/*****************************************************************************/
LOCAL void  RecordWorkASPCallBack( MMIRECORD_WORK_ERROR_E error_ret, MMIRECORD_WORK_DATA_INFO_T  *data_info_ptr)
{
    MMI_CTRL_ID_T       label_ctrl_id = MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID;
    MMI_STRING_T        ring_name = {0};
    MMIASP_APPLET_T*    applet_ptr  =(MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    MMIASP_NV_SETTING_T set_nv  = MMIASP_GetSettingNvValue();
      if(error_ret == MMIRECORD_WORK_SUCCESS)
      {
        if(PNULL != data_info_ptr->file_name)
        {       
            ring_name.wstr_ptr = (wchar*)data_info_ptr->file_name;
            ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
            if(PNULL != applet_ptr)
            {
                SCI_MEMSET(applet_ptr->nv_info.record_path_for_client, 0, sizeof(applet_ptr->nv_info.record_path_for_client));
                MMIAPICOM_Wstrncpy(applet_ptr->nv_info.record_path_for_client, ring_name.wstr_ptr, ring_name.wstr_len);     
                applet_ptr->nv_info.audio_type = MMIASP_SETTING_MORE_AUDIO;

                set_nv.audio_type = applet_ptr->nv_info.audio_type;
                set_nv.fixed_ring_id = 0;
                MMIAPICOM_Wstrncpy(set_nv.record_path_for_client, applet_ptr->nv_info.record_path_for_client, MMIFILE_FILE_NAME_MAX_LEN);
            }
            GUILABEL_SetText(label_ctrl_id, &ring_name, FALSE);
            MMIASP_SetSetting2NV(set_nv);
        }        
      } 
      MMIAPIASP_SetRecordRingState(FALSE);
      MMK_CloseWin(MMIASP_SETTING_RING_WIN_ID);

}
#endif

/*****************************************************************************/
// Description : setting msg
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAspSettingWinMsg(
                                       MMI_WIN_ID_T      win_id,    
                                       MMI_MESSAGE_ID_E  msg_id, 
                                       DPARAM            param
                                       )
{
    MMI_RESULT_E   result                                 = MMI_RESULT_TRUE;
    static  MMIFILE_DEVICE_E s_device_arr[MMI_DEVICE_NUM] = {0};
    uint16         cur_index                              = 0;
	uint32         i                                      = 0;
	MMI_CTRL_ID_T  ctrl_id                                = NULL;
	 //MMIASP_NV_SETTING_T set_nv                           = MMIASP_GetSettingNvValue();
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            SCI_MEMSET(s_device_arr, 0x00, sizeof(s_device_arr));
			GUIFORM_SetType(MMIASP_SETTING_FORM_CTRL_ID, GUIFORM_TYPE_TP);
			GUISETLIST_SetFontAlign(MMIASP_SETTING_AUDIO_TIME_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
			GUISETLIST_SetFontAlign(MMIASP_SETTING_WAIT_TIME_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
			GUISETLIST_SetFontAlign(MMIASP_SETTING_SPEAKER_FLAG_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
			{
			#ifndef MMI_MULTI_SIM_SYS_SINGLE
				MMI_CTRL_ID_T      flag_dropdown_id[] = 
				{
					MMIASP_SETTING_FLAG_SIM1_DROPDOWNLIST_CTRL_ID,
						MMIASP_SETTING_FLAG_SIM2_DROPDOWNLIST_CTRL_ID,
				#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)    
						MMIASP_SETTING_FLAG_SIM3_DROPDOWNLIST_CTRL_ID,
				#endif
				#if defined(MMI_MULTI_SIM_SYS_QUAD)   
						MMIASP_SETTING_FLAG_SIM4_DROPDOWNLIST_CTRL_ID,
				#endif
				} ;
			#else
				MMI_CTRL_ID_T      flag_dropdown_id[] = 
				{
					MMIASP_SETTING_FLAG_DROPDOWNLIST_CTRL_ID,   
				} ;  
			#endif
				for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
				{       
					GUISETLIST_SetFontAlign(flag_dropdown_id[i], ALIGN_LVMIDDLE);
				} 	
			}
			GUISETLIST_SetFontAlign(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID,ALIGN_LVMIDDLE);
            InitSettingAllLabels();
            AppendSettingAllDropDownListData(s_device_arr);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
            MMK_SetAtvCtrl(win_id, MMIASP_SETTING_FLAG_SIM1_DROPDOWNLIST_CTRL_ID);
#else
            MMK_SetAtvCtrl(win_id, MMIASP_SETTING_FLAG_DROPDOWNLIST_CTRL_ID);
#endif
            break;

#ifndef MMI_PDA_SUPPORT 
        case MSG_CTL_OK:
        case MSG_APP_WEB:
			cur_index =	GUISETLIST_GetCurIndex(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID);
            if(cur_index < MMI_DEVICE_NUM)
            {
                SaveAspSetting(s_device_arr[cur_index]);
            }
            else
            {
                //否则默认存储位置为T卡
                SaveAspSetting(MMI_DEVICE_SDCARD);
            }
            
            //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            // 更新IDLE上ASP的头标状态....
            MMIAPIIDLE_UpdateAspState();
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
            if(MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                MMIASP_OpenRingSelectWin(PNULL);
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
            break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

        case MSG_CTL_MIDSK:
        case MSG_APP_OK:
            {
                ctrl_id = MMK_GetActiveCtrlId(win_id);
                if(MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID == ctrl_id)
                {
                    MMIASP_OpenRingSelectWin(PNULL);
                }
                else
                {
                    result = MMI_RESULT_FALSE;
                }
            }
            break;

        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
			cur_index =	GUISETLIST_GetCurIndex(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID);
            if(cur_index < MMI_DEVICE_NUM)
            {
                SaveAspSetting(s_device_arr[cur_index]);
            }
            else
            {
                //否则默认存储位置为话机
                SaveAspSetting(MMI_DEVICE_UDISK);
            }
            
            //MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
            // 更新IDLE上ASP的头标状态....
            MMIAPIIDLE_UpdateAspState();
            MMK_CloseWin(win_id);
            break;
#if 1
        case MSG_GET_FOCUS:
			{
				//MMIFILE_DEVICE_E      i= MMI_DEVICE_UDISK;
				MMIFILE_DEVICE_E      user_device= MMI_DEVICE_UDISK;
				MMI_CTRL_ID_T      save_path_dropdown_id = MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID;
				//uint16      cur_index = 0;
				uint16      save_device_num = 0;
				//MMI_TEXT_ID_T       save_path_text[ASP_SAVE_PATH_TYPE_NUM]  = {0};
				MMI_STRING_T      save_path_dropdownlist[ASP_SAVE_PATH_TYPE_NUM] = {0};
				//在getfocus时先保存一下nv中路径的数据，
				//#if 0
				{
				//	set_nv = MMIASP_GetSettingNvValue();
					cur_index =	GUISETLIST_GetCurIndex(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID);
					if(cur_index < MMI_DEVICE_NUM)
					{
						SaveAspSetting(s_device_arr[cur_index]);
					}
					else
					{
						//否则默认存储位置为话机
						SaveAspSetting(MMI_DEVICE_UDISK);
					}
			//		MMIASP_SetSetting2NV(set_nv);
				}
			//	#endif

				
				
				GUISETLIST_DeleteAllItem(save_path_dropdown_id);
				cur_index =	GUISETLIST_GetCurIndex(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID);
				user_device = s_device_arr[cur_index];
				SCI_MEMSET(s_device_arr, 0x00, sizeof(s_device_arr));
				//user_device = GetRecordSaveStorage();
				for( i= MMI_DEVICE_UDISK ; i < ASP_SAVE_PATH_TYPE_NUM; i++)
				{
					if(MMIAPIFMM_GetDeviceTypeStatus(i))
					{
						s_device_arr[save_device_num] = i;
						GUISETLIST_AddItemById(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID, MMIAPIFMM_GetDeviceName((MMIFILE_DEVICE_E)i));
						MMI_GetLabelTextByLang(MMIAPIFMM_GetDeviceName(i), &save_path_dropdownlist[save_device_num]);
						if(user_device == i)
						{
							cur_index = save_device_num;
						}
						save_device_num++;
					}
				}
				GUISETLIST_SetCurIndex(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID, cur_index);
				MMIAPIIDLE_UpdateAspState();
			}
             break;
#endif		
        case MMI_MSG_RECORDTORING:
			{
				MMI_CTRL_ID_T       label_ctrl_id = MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID;
				MMI_STRING_T        ring_name = {0};
				MMIASP_APPLET_T*    applet_ptr  =(MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
				if(PNULL != param)
				{       
                    ring_name.wstr_ptr = (wchar*)param;
                    ring_name.wstr_len = MMIAPICOM_Wstrlen(ring_name.wstr_ptr);
                    if(PNULL != applet_ptr)
                    {
                        SCI_MEMSET(applet_ptr->nv_info.record_path_for_client, 0, sizeof(applet_ptr->nv_info.record_path_for_client));
                        MMIAPICOM_Wstrncpy(applet_ptr->nv_info.record_path_for_client, ring_name.wstr_ptr, ring_name.wstr_len);     
                        applet_ptr->nv_info.audio_type = MMIASP_SETTING_MORE_AUDIO;
                    }
                    GUILABEL_SetText(label_ctrl_id, &ring_name, FALSE);
					cur_index =	GUISETLIST_GetCurIndex(MMIASP_SETTING_SAVE_PATH_DROPDOWNLIST_CTRL_ID);
                    if(cur_index < MMI_DEVICE_NUM)
                    {
                        SaveAspSetting(s_device_arr[cur_index]);
                    }
                    //SaveAspSetting();
				}        
			}
        break;

        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return result;
}
/*****************************************************************************/
//  Description : 添加固定答录语list的item
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void AppendFixedAnswerRing(MMI_CTRL_ID_T    ctrl_id)
{
    MMIASP_NV_SETTING_T set_nv      = MMIASP_GetSettingNvValue();
    uint16              cur_index   = 0;
    uint16              i           = 0;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    GUILIST_SetMaxItem(ctrl_id, MMIASP_FIXED_ANSWERRING_NUM,  FALSE);
    for (i = 0; i < MMIASP_FIXED_ANSWERRING_NUM; i++)
    {   
        text_id = MMIASP_GetFixedAnswerRingTextId(i);
        MMIAPISET_AppendListItemByTextId(text_id, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO );
    }

    if(MMIASP_FIXED_ANSWERRING_NUM > set_nv.fixed_ring_id)
    {
        cur_index = set_nv.fixed_ring_id;
    }
    else
    {
        cur_index = 1;
    }

    GUILIST_SetCurItemIndex(ctrl_id, cur_index);
    GUILIST_SetSelectedItem(ctrl_id, cur_index, TRUE);
    //play ring
    MMIAPISET_PreviewRing(MN_DUAL_SYS_1, cur_index+1,3,1,MMISET_RING_TYPE_ASP,PNULL);
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIASP_GetFixedAnswerRingTextId(uint8 ring_id)
{
    MMI_TEXT_ID_T text_id = TXT_NULL;
    
    switch(ring_id)
    {
        case 0:
            text_id =  TXT_ASP_ANSWER_RING_1;
            break;
        case 1:
            text_id =  TXT_ASP_ANSWER_RING_2;
            break;
        case 2:
            text_id =  TXT_ASP_ANSWER_RING_3;
            break;
        case 3:
            text_id =  TXT_ASP_ANSWER_RING_4;
            break;
        case 4:
            text_id =  TXT_ASP_ANSWER_RING_5;
            break;
        case 5:
            text_id =  TXT_ASP_ANSWER_RING_6;
            break;
        case 6:
            text_id =  TXT_ASP_ANSWER_RING_7;
            break;
        case 7:
            text_id =  TXT_ASP_ANSWER_RING_8;
            break;      
        default:
            text_id = TXT_NULL;
            break;
    }

    return text_id;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL  uint16 GetAnswerRingName(wchar* answer_name_ptr)//out
{
    MMIASP_NV_SETTING_T set_nv = MMIASP_GetSettingNvValue();
    MMI_STRING_T  answerString = {0};
    MMI_TEXT_ID_T string_id    = TXT_NULL;
    
    if(PNULL == answer_name_ptr)
    {
        return 0;
    }
    
    // 如果是固定答录语
    if(MMIASP_SETTING_FIX_AUDIO == set_nv.audio_type)
    {
        string_id = MMIASP_GetFixedAnswerRingTextId(set_nv.fixed_ring_id);
        MMI_GetLabelTextByLang(string_id, &answerString);
    }
    else
    {
        if(MMIAPIFMM_IsFileExist((const wchar*)set_nv.record_path_for_client,MMIAPICOM_Wstrlen(set_nv.record_path_for_client)) == FALSE)
        {
            string_id = MMIASP_GetFixedAnswerRingTextId(set_nv.fixed_ring_id);
            MMI_GetLabelTextByLang(string_id, &answerString);
        }
        else
        {
            answerString.wstr_ptr = set_nv.record_path_for_client;
            answerString.wstr_len = MMIAPICOM_Wstrlen(set_nv.record_path_for_client);
        }
    }
    
    MMIAPICOM_Wstrncpy(answer_name_ptr, answerString.wstr_ptr, answerString.wstr_len);
    
    return answerString.wstr_len;
    
}
/*****************************************************************************/
// Description : select ring for client
// Global resource dependence : none
// Author: 
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRingSelectWinMsg(
                                          MMI_WIN_ID_T     win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM           param 
                                          )
{
    MMIASP_APPLET_T*        applet_ptr  =(MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    MMI_HANDLE_T            menu_handle = MMK_GetCtrlHandleByWin( win_id, MMIASP_MSG_RING_SELECT_MENU_CTRL_ID );
    MMIFMM_SELECT_INFO_T    select_info = {MMIFMM_MUSIC_WAV,0, 0, 0, PNULL, 0, 0};
    MULTIM_SELECT_RETURN_T* win_param   = PNULL;
    MMI_RESULT_E            result      = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T     group_id    = 0;
    MMI_MENU_ID_T           menu_id     = 0;
    PRINGSELECT_CALLBACKFUNC callback_fun_ptr = (PRINGSELECT_CALLBACKFUNC)MMK_GetWinAddDataPtr(win_id);/*lint !e611*/
	//MMI_HANDLE_T            applet_handle = 0;
#ifdef MMI_PDA_SUPPORT
	GUIMENU_BUTTON_INFO_T   menu_button_info = {0};
#endif
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetActiveCtrl( menu_handle, FALSE );
			
#ifdef MMI_PDA_SUPPORT
			menu_button_info.is_static = TRUE;
			menu_button_info.group_id = MENU_SETTING_RING_SELECT_OPT;
			menu_button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
			GUIMENU_SetButtonStyle(MMIASP_MSG_RING_SELECT_MENU_CTRL_ID, &menu_button_info);
#endif
            // 通话中灰掉录制答录语这一项.............
            if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {
                MMIAPICOM_EnableGrayed(win_id, MENU_SETTING_RING_SELECT_OPT,MMIASP_CUSTOM_RING_RECORDONE, TRUE);
            }
            else
            {
                MMIAPICOM_EnableGrayed(win_id, MENU_SETTING_RING_SELECT_OPT,MMIASP_CUSTOM_RING_RECORDONE, FALSE);
            }
            break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:       
            GUIMENU_GetId( menu_handle, &group_id, &menu_id);
            switch(menu_id)
            {
                case MMIASP_CUSTOM_RING_FIXED:
                    if(!IsUDiskRunning())
                    {
#ifndef MMI_PDA_SUPPORT
                        MMK_CreateWin((uint32*)MMIASP_SETTING_FIXED_ANSWERRING_WIN_TAB, (ADD_DATA)callback_fun_ptr);/*lint !e611*/
#else
                        MMK_CreatePubListWin((uint32*)MMIASP_SETTING_FIXED_ANSWERRING_WIN_TAB, (ADD_DATA)callback_fun_ptr);/*lint !e611*/
#endif
                    }
                    break;
            
                case MMIASP_CUSTOM_RING_DOWNLOAD:
                    select_info.win_id = win_id;
                    select_info.ring_vol = MMIAPISET_GetMultimVolume();
#ifdef DRM_SUPPORT
                    select_info.select_file[MMIFMM_SELECT_MAIN_TYPE] |= MMIFMM_DRM_ALL; 
#endif
                    MMIAPIFMM_OpenSelectMusicWin(&select_info);
#ifdef DRM_SUPPORT
                    {
                        MMIFMM_DRM_LIMIT_TYPE_T limit_value = {0};
                        limit_value.cons_mode_limit = MMIFMM_DRM_CONS_LIMIT_COUNT;
                        limit_value.is_rights_valid = 1;
                        MMIAPIFMM_SetSelDrmLimitValue(limit_value);
                    }
#endif
                    break;

                case MMIASP_CUSTOM_RING_RECORDONE:
                    {
#ifdef MMI_RECORD_SUPPORT						
                        MMIRECORD_WORK_PARAM_T parameter={0};
                        parameter.max_file_size=NULL;
                        parameter.max_file_time=NULL;//10*1000;
                        parameter.record_support_type=MMIRECORD_WORK_FORMAT_WAV;
                        parameter.work_callback=RecordWorkASPCallBack;                  
                        if(MMIAPIRECORD_OpenMainWin(&parameter) == MMIRECORD_WORK_SUCCESS)
                        {
                            MMIAPIASP_SetRecordRingState(TRUE);
                        }
#endif                        
                    }

                    break;

                default:
                    break;
            }
            break;
        
        
        case MSG_MULTIM_SELECTED_RETURN : 
            {
                MMI_CTRL_ID_T       label_ctrl_id = MMIASP_SETTING_AUDIO_SELECT_LABEL_CTRL_ID;
                MMI_STRING_T        ring_name = {0};
                
                if(PNULL != param)
                {
                    win_param = (MULTIM_SELECT_RETURN_T *)param; 
                    if(PNULL == callback_fun_ptr)
                    {
                        if(PNULL != applet_ptr)
                        {
                            SCI_MEMSET(applet_ptr->nv_info.record_path_for_client, 0, sizeof(applet_ptr->nv_info.record_path_for_client));
                            MMIAPICOM_Wstrncpy(applet_ptr->nv_info.record_path_for_client, win_param->file_name_ptr, win_param->file_name_len);     
                            applet_ptr->nv_info.audio_type = MMIASP_SETTING_MORE_AUDIO;
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"no applet, please check!!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4243_112_2_18_2_2_52_165,(uint8*)"");
                        }
                        // 给label赋值
                        ring_name.wstr_ptr = win_param->file_name_ptr;
                        ring_name.wstr_len = win_param->file_name_len;
                        GUILABEL_SetText(label_ctrl_id, &ring_name, FALSE);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIASP HandleRingSelectWinMsg has callback function now"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4252_112_2_18_2_2_52_166,(uint8*)"");
                        callback_fun_ptr(FALSE, 0, win_param->file_name_ptr);
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIASP HandleRingSelectWinMsg MULTIM_SELECTED_RETURN null para"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4258_112_2_18_2_2_52_167,(uint8*)"");
                }

                MMK_CloseWin(win_id);
                break;
            }
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMIAPIASP_SetRecordRingState(FALSE);
            MMK_CloseWin(win_id);
            break;
        
        case MSG_CLOSE_WINDOW:
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
    }
    
    return (result);
}
/*****************************************************************************/
// Description : opt handle msg of play win
// Global resource dependence : none
// Author: byte.guo
// Note:      
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePlayWinOptMsg(
                                        MMI_WIN_ID_T     win_id, 
                                        MMI_MESSAGE_ID_E msg_id, 
                                        DPARAM           param 
                                        )
{
    MMI_RESULT_E        result   = MMI_RESULT_TRUE;
    MMI_MENU_ID_T       menu_id  = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    uint8               num_len = 0;
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    MMIASP_MSG_INFO_T*  cur_node = MMIASP_GetCurNodeByGuiIndex(applet_ptr,applet_ptr->cur_gui_list_index);
    MMI_HANDLE_T        menu_handle = MMK_GetCtrlHandleByWin( win_id, MMIASP_MSG_PLAY_WIN_OPT_MENU_CTRL_ID );
    MN_DUAL_SYS_E       sim_type = MN_DUAL_SYS_MAX;
    MMI_STRING_T        string_to = {0};
    MMI_WIN_ID_T        query_win_id = MMIASP_QUERY_WIN_ID;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        EnableMsgOpt(applet_ptr);
        MMK_SetActiveCtrl( menu_handle, FALSE);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        if(PNULL == cur_node)
        {
            break;
        }
        
        GUIMENU_GetId( menu_handle, &group_id, &menu_id);
        num_len = (uint8)strlen( (char*)cur_node->call_num); 
        
        switch(menu_id)
        {
            /* Make call by the phone number */
        case MMIASP_MENU_CALL_AUDIO_ITEM_ID:  
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                 cur_node->call_num,
                num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_NORMAL_CALL,
                PNULL);
            MMK_CloseWin(win_id);
            break;                    
            
            
#ifdef MMI_IP_CALL_SUPPORT
        case MMIASP_MENU_CALL_IP_ITEM_ID:
            MMIAPICC_MakeCall(MN_DUAL_SYS_MAX,
                cur_node->call_num,
                num_len,
                PNULL,
                PNULL,
                CC_CALL_SIM_MAX,
                CC_CALL_IP_CALL,
                PNULL);
            MMK_CloseWin(win_id);
            break;
#endif
            
            /* Write and send new message by the phone number */
        case MMIASP_MENU_MSG_SMS_ITEM_ID:
            {
                if (num_len > 0)
                {
                     //调用sms接口函数
                    MMIAPISMS_WriteNewMessage(MMIAPISET_GetActiveSim(), PNULL,cur_node->call_num, num_len);
                }
                else
                {
                    //提示窗口没有号码
#ifdef MMI_PDA_SUPPORT
                    MMIPUB_DisplayTipsIDEx(PNULL, TXT_CL_EMPTY_NUM, MMIPUB_TIPS_DIS_MIDDLE, PNULL);
#else
					MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
#endif
                }
                MMK_CloseWin(win_id);            
            }
            break;
            // 回复彩信....
        case MMIASP_MENU_MSG_MMS_ITEM_ID:
            {
                if (num_len > 0)
                {
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));

                    SCI_MEMSET(string_to.wstr_ptr, 0x00, (num_len * sizeof(wchar)));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, cur_node->call_num, num_len, num_len);
                    string_to.wstr_len = num_len;
                    MMIAPIMMS_AnswerMMS(sim_type, PNULL, &string_to);
                    SCI_FREE(string_to.wstr_ptr);
                    string_to.wstr_ptr = PNULL;
                }
                else
                {
#ifdef MMI_PDA_SUPPORT
                    MMIPUB_DisplayTipsIDEx(PNULL, TXT_CL_EMPTY_NUM, MMIPUB_TIPS_DIS_MIDDLE, PNULL);
#else
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);  
#endif   
                }
                MMK_CloseWin(win_id);
            }
            break; 
        case MMIASP_MENU_DELETE_ITEM_ID:
             MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY, &query_win_id, HandlePlayMsgDelQueryWin);
             MMK_CloseWin(win_id);
            break;
            
        default: 
            break;
        }
        break;
        
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
            break;
            
        default:
            result = MMI_RESULT_FALSE;
            break;
            
    }
    
    return result;
}


/*****************************************************************************/
// FUNCTION:     Start answer machine applet
// Description :   
// Global resource dependence : 
// Author:       byte.guo
// Note:   
/*****************************************************************************/
LOCAL CAF_HANDLE_RESULT_E AspAppletHandleEvent(    
                                               IAPPLET_T*          iapplet_ptr,
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               void*              param
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)iapplet_ptr;

    if(PNULL == iapplet_ptr)
    {
        return result;
    }
    
   switch ( msg_id )
   {
   case MSG_APPLET_START:
       {
           MMI_WINDOW_TABLE_CREATE_T win_table = {0};
           applet_ptr->start_type = *(MMIASP_START_APPLET_E*)param;
           win_table.applet_handle = applet_ptr->caf_applet.app_handle;
           /* start app and open win */
           if (MMIASP_START_DEFAULT ==  applet_ptr->start_type)
           {
                if(MMK_IsOpenWin(MMIASP_QUERY_WIN_ID))
                {
                    MMK_CloseWin(MMIASP_QUERY_WIN_ID);
                }
               win_table.win_table_ptr = MMIASP_MSG_LIST_WIN_TAB;
               MMK_CreateWinTable( &win_table );
           }
           else if(MMIASP_AUTO_PLAY_MESSEAGES ==  applet_ptr->start_type)
           {
               MMIASP_AutoPlayRecordMessages();
           }
           else
           {
               //SCI_TRACE_LOW:"[MMIASP] AspAppletHandleEvent start app only..."
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4466_112_2_18_2_2_52_168,(uint8*)"");
           }
       }
       break;
       
   case MSG_APPLET_STOP:
       //SCI_TRACE_LOW:"[MMIASP] AspAppletHandleEvent MSG_APPLET_STOP"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4472_112_2_18_2_2_52_169,(uint8*)"");

       MMIAPIASP_StopCallAnswer();
       /* free list and close cur applet */
       MMIASP_MsgListFree(applet_ptr);   
       break;
       
   case MSG_APPLET_SUSPEND:
       break;
       
   case MSG_APPLET_RESUME:
       break;
   default:
       break;
   }
   
   return result;
}

/*****************************************************************************/
// Description : get ans_mathine module info
// Global resource dependence : 
// Author:byte.guo
// Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E AnsMathineModuleCreateInstance( 
                                                  IMODULE_T* pMod, 
                                                  CAF_GUID_T guid, 
                                                  void** ppObj 
                                                  )
{
   int32 result = FALSE;
   
   *ppObj = PNULL;
   
   if( guid == SPRD_ANSMACHINE_APPLET_ID )
   {
       result = CAFAPPLET_New( sizeof(MMIASP_APPLET_T), guid, pMod, (IAPPLET_T**)ppObj, AspAppletHandleEvent, PNULL, PNULL );
   }
   
   return result;
}

/*****************************************************************************/
// Description : get ans_mathine module info
// Global resource dependence : 
// Author:byte.guo
// Note:
/*****************************************************************************/
LOCAL CAF_RESULT_E AnsMathineModuleEntry(
                                         const void* ph, 
                                         IMODULE_T** ppMod 
                                         )
{
   return CAFMODULE_New( sizeof(CAF_MODULE_T), ph, ppMod, AnsMathineModuleCreateInstance, PNULL );
}
/*****************************************************************************/
// Description : get ans_mathine module info
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
PUBLIC void MMIASP_EnterMainFrame(void)
{
   if(!IsUDiskRunning())
   {
       MMK_CreateWin((uint32*)MMIASP_MSG_LIST_WIN_TAB, PNULL);
   }
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
LOCAL uint16 GetShowCallName(
                             wchar*   show_name_ptr, //out
                             uint16   max_name_len   //in
                             )
{
    uint16              show_name_len = 0;
    MMI_STRING_T        name_str      = {0};
    MMI_STRING_T        unname_str    = {0};
    MMI_STRING_T        user_name_str = {0};
    uint8               cur_call_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8               tel_num_len = CC_MAX_TELE_NUM_LEN + 2;
    wchar               name_arr[MMIASP_MAX_NAME_LEN+1] = {0};

    MMIAPICC_GetCallNumber(cur_call_num, tel_num_len);
    user_name_str.wstr_ptr = name_arr;
    
    if(MMIASP_GetNameByNumber(cur_call_num, &user_name_str))
    {
        MMI_GetLabelTextByLang(TXT_MMIASP_LINKMAN, &name_str);
        SCI_MEMCPY(show_name_ptr,  name_str.wstr_ptr, name_str.wstr_len*2);
        SCI_MEMCPY(show_name_ptr + name_str.wstr_len, user_name_str.wstr_ptr, user_name_str.wstr_len*2);
        show_name_len = name_str.wstr_len + user_name_str.wstr_len;    
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MMIASP_LINKMAN, &name_str);
        MMI_GetLabelTextByLang(TXT_NO_NAME,  &unname_str);
        
        SCI_MEMCPY(show_name_ptr,  name_str.wstr_ptr, name_str.wstr_len*2);
        SCI_MEMCPY(show_name_ptr + name_str.wstr_len, unname_str.wstr_ptr, unname_str.wstr_len*2);
        
        show_name_len = name_str.wstr_len + unname_str.wstr_len;
    }
    
    return show_name_len;
}
#if defined(MMI_PDA_SUPPORT) 
/*****************************************************************************/
//  Description : display win background
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
LOCAL void DisplayAspWinBg( MMI_WIN_ID_T win_id )
{
    GUI_POINT_T dis_point = {0}; 
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID,GUI_BLOCK_MAIN}; 
 
    //display bg
    GUIRES_DisplayImg(&dis_point,
        PNULL,
        PNULL,
        win_id,
        IMAGE_COMMON_BG,
        &lcd_dev_info); 
}
#endif

/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleReleasingWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E        recode       = MMI_RESULT_TRUE;

    switch(msg_id)
    {


        case MSG_ASP_CLOSE_RELEASING_WIN:
            MMK_CloseWin(win_id);
            break;

        default:
            recode = MMIPUB_HandleAlertWinMsg( win_id,  msg_id,  param);
            break;
    }
    return recode;
}
/*****************************************************************************/
// Description : 
// Global resource dependence : none
// Author: jun.hu
// Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleRecordingWinMsg(
                                         MMI_WIN_ID_T        win_id, 
                                         MMI_MESSAGE_ID_E    msg_id, 
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E        recode       = MMI_RESULT_TRUE;

    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    MMIASP_NV_SETTING_T  cur_nv = MMIASP_GetSettingNvValue();
    MMISRVAUD_RECORD_INFO_T record_info = {0};
    MMISRV_HANDLE_T handle = {0};   
    uint32	record_time = 0;
    
    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            { 
				MMK_SetAtvCtrlEx(win_id, MMIASP_RECORDING_FORM_CTRL_ID, FALSE);
				//GUICTRL_SetState(MMK_GetCtrlPtr(MMIASP_RECORDING_FORM_CTRL_ID),GUICTRL_STATE_DISABLE_ACTIVE,TRUE);
                MMIASP_SetCallState(MMIASP_MACHINE_CALL_PLAYAUDIO);
                if(PNULL != applet_ptr)
                {
                    if(MMI_NONE_TIMER_ID != applet_ptr->count_time_id)
                    {
                        MMK_StopTimer(applet_ptr->count_time_id);
                        applet_ptr->count_time_id = MMI_NONE_TIMER_ID;
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIASP HandleRecordingWinMsg OPEN but applet is null"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4625_112_2_18_2_2_53_170,(uint8*)"");
                }
                if(cur_nv.speaker_flag)
                {
                    MMIAPICC_EnableHandfreeMode(TRUE);                                       
#if defined(MMI_PDA_SUPPORT) 
                    GUIWIN_SeSoftkeytButtonTextId( win_id,  TXT_HAND_ON, 1, TRUE);
#endif
                }
                else
                {
                    MMIAPICC_EnableHandfreeMode(FALSE);
                }
                // 开始播放答录语....
                MMIASP_StartPlayAnswerMusic();
            }
            break;
            
        case MSG_LCD_SWITCH:
            MMK_CreateCtrlByWinTabEx(win_id,  (MMITHEME_IsMainScreenLandscape() ? MMIASP_RECORDING_WIN_TAB_H : MMIASP_RECORDING_WIN_TAB_V));   /*lint !e605*/   
            SetRecordingWinContent();
            break;
            
        case MSG_KEYDOWN_HEADSET_BUTTON: // 耳机按键.....             
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
            {
                MMIASP_SetCallState(MMIASP_MACHINE_CALL_SPEAKING);
                MMIAPIASP_StopCallAnswer();
                MMIAPICC_GotoRealCallState(applet_ptr->record_time_count);
                MMK_CloseWin(win_id);
            }
            break;
            
        case MSG_TIMER:
            {
                if(applet_ptr->count_time_id == *(uint8*)param)
                {
#ifdef WIN32
                    applet_ptr->record_time_count++;
                    record_time = MMI_1SECONDS*applet_ptr->record_time_count;
#else
                    handle = MMIASP_GetRecordHandle();
                    if(handle > 0)
                    {
                        MMISRVAUD_GetRecordInfo(handle, &record_info);
                        applet_ptr->record_time_count = MMIAPICOM_GetSecondFromMillisecond(record_info.total_time);
                        record_time = record_info.total_time;
                    }
#endif
                    UpdateRecordingTime(TRUE);

                    if(record_time >= (MMIASP_GetRecordTime() - MMI_500MSECONDS))//count_time_id每500ms响应一次，因此有500ms的误差
                    {
                        MMIAPIASP_SetAspRecordTimeOutState(TRUE);
                        MMIASP_RecordTimeout();
                    }
                }
                else if(applet_ptr->record_timer_id == *(uint8*)param)
                {
                    MMIAPIASP_SetAspRecordTimeOutState(TRUE);
                    MMIASP_RecordTimeout();
                }
                else if(applet_ptr->answer_ring_time_id== *(uint8*)param)
                {  
                    if(MMI_NONE_TIMER_ID != applet_ptr->answer_ring_time_id)
                    {
                        MMK_StopTimer(applet_ptr->answer_ring_time_id);
                        applet_ptr->answer_ring_time_id = MMI_NONE_TIMER_ID;
                    }
                    MMIAPIASP_HandlePlayAudioEnd();
                }
            }
            break;
#if defined(MMI_PDA_SUPPORT) 
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
   case MSG_CTL_PENOK: 
        if ((PNULL != param) && ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                || (MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)
                || (MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id)))
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left softkey处理
                //SCI_TRACE_LOW:"HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4693_112_2_18_2_2_53_171,(uint8*)"");
                MMIASP_SetCallState(MMIASP_MACHINE_CALL_SPEAKING);
                MMIAPIASP_StopCallAnswer();
                MMIAPICC_GotoRealCallState(applet_ptr->record_time_count);
                MMK_CloseWin(win_id);
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle softkey处理
                //SCI_TRACE_LOW:"HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4702_112_2_18_2_2_53_172,(uint8*)"");
                if (MMIAPICC_IsHandFree())
                {
                    MMIAPICC_EnableHandfreeMode(FALSE);
                    // 同时更新softkey
                    //GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_COMMON_HAND_FREE, TRUE);
                    GUIWIN_SeSoftkeytButtonTextId( win_id,  TXT_COMMON_HAND_FREE, 1, TRUE);
                }
                else
                {
                    MMIAPICC_EnableHandfreeMode(TRUE);
                    // 同时更新softkey
                    //GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_HAND_ON, TRUE);
                    GUIWIN_SeSoftkeytButtonTextId( win_id,  TXT_HAND_ON, 1, TRUE);
                }
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right softkey处理
                //SCI_TRACE_LOW:"HandleRecordingWinMsg() MSG_CTL_PENOK MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_4721_112_2_18_2_2_53_173,(uint8*)"");
                MMIAPICC_ReleaseCallByRedkey();   
                //增加释放中界面，避免直接回到通话前的窗口
                {

                    uint32 time_period = MMI_10SECONDS * 10;
                    MMI_WIN_ID_T alert_win_id = MMIASP_CALL_RELEASING_WIN_ID;
                    MMI_WIN_PRIORITY_E win_priority = WIN_ONE_LEVEL;
                    MMIPUB_SOFTKEY_STYLE_E softkey_style = MMIPUB_SOFTKEY_NONE;

                    MMIPUB_OpenAlertTextWinByTextId(&time_period, TXT_CC_RELEASING, &alert_win_id, &win_priority, softkey_style, HandleReleasingWinMsg);
                }
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

        case MSG_FULL_PAINT:
#if defined(MMI_PDA_SUPPORT) 
        DisplayAspWinBg(win_id);
        if (MMIAPICC_IsHandFree())
        {
            GUIWIN_SeSoftkeytButtonTextId( win_id,  TXT_HAND_ON, 1, FALSE);
        }
        else
        {
            GUIWIN_SeSoftkeytButtonTextId( win_id,  TXT_COMMON_HAND_FREE, 1, FALSE);
        }
#else
        if (MMIAPICC_IsHandFree())
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_HAND_ON, FALSE);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_COMMON_HAND_FREE, FALSE);
        }
#endif
         break;
         
        case MSG_APP_CANCEL:
        case MSG_CTL_CANCEL:
#if defined(MMI_PDA_SUPPORT) 
            //MMIAPICC_ReleaseCallByRedkey();
#else
            if (MMIAPICC_IsHandFree())
            {
                MMIAPICC_EnableHandfreeMode(FALSE);
                // 同时更新softkey
                GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_COMMON_HAND_FREE, TRUE);
            }
            else
            {
                MMIAPICC_EnableHandfreeMode(TRUE);
                // 同时更新softkey
                GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_HAND_ON, TRUE);
            }
#endif
            break;
            
        case MSG_CLOSE_WINDOW:
            if(PNULL != applet_ptr)
            {
                if(MMI_NONE_TIMER_ID != applet_ptr->count_time_id)
                {
                    MMK_StopTimer(applet_ptr->count_time_id);
                    applet_ptr->count_time_id = MMI_NONE_TIMER_ID;
                }
                applet_ptr->record_time_count = 0;
                MMIASP_ShowNewAspMsgWin();
            }
            break;
    
        case MSG_KEYUP_SDCARD_DETECT:
            {
                if(((MMI_DEVICE_SDCARD <= cur_nv.record_save_dev))
                    &&(MMIASP_GetCallState()==MMIASP_MACHINE_CALL_RECORDING))
                {   
                    MMIASP_MSG_INFO_T *next_ptr =PNULL ;
                    next_ptr=applet_ptr->list_head_ptr;
                    MMIASP_SetListHead( &applet_ptr->list_head_ptr, next_ptr->next_ptr );
                    SCI_FREE(next_ptr);
                    next_ptr = PNULL;
                    
                    MMIASP_WriteSysFile();
                    MMIASP_RecordTimeout();//停止答录，挂断电话。
                }
            }
            break;
    
        case MSG_KEYDOWN_HEADSET_DETECT:
        case MSG_KEYUP_HEADSET_DETECT:
            {
                if(MMIAPICC_IsHandFree())
                {
#if defined(MMI_PDA_SUPPORT) 
                    GUIWIN_SeSoftkeytButtonTextId( win_id,  TXT_HAND_ON, 1, TRUE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_HAND_ON, TRUE);
#endif
                }
                else
                {
                
#if defined(MMI_PDA_SUPPORT) 
                    GUIWIN_SeSoftkeytButtonTextId( win_id,  TXT_COMMON_HAND_FREE, 1, TRUE);
#else
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_ANSWER_CALL, TXT_NULL, TXT_COMMON_HAND_FREE, TRUE);
#endif
                }
             }
            break;

        case MSG_ASP_CLOSE_RECORD_WIN:
            MMK_CloseWin(win_id);
            break;

#ifdef MMI_PDA_SUPPORT
        //答录界面最小化有问题，先屏蔽掉HOME键
        case MSG_KEYUP_RED:
            break;
#endif            
            
        default:
            recode = MMI_RESULT_FALSE;
            break;    
    }
    
    return recode;
    
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
LOCAL void UpdateRecordingTime(BOOLEAN is_refresh)
{
    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    wchar           all_time[MMIASP_MAX_STRING_LEN + 1] = {0};
    uint8           time_str[CC_DISPLAY_TIME_LEN +1] = {0};
    wchar           time[CC_DISPLAY_TIME_LEN + 1]    = {0};
    uint32          cur_time_count =   0;
    uint16          time_len       =   0;
    MMI_STRING_T    time_txt = {0};
    MMI_STRING_T    time_pre = {0};
    
    if(PNULL != applet_ptr)
    {
        cur_time_count = applet_ptr->record_time_count;
        
        if(MMI_NONE_TIMER_ID != applet_ptr->count_time_id)
        {
            sprintf((char*)time_str, 
                "%02d:%02d:%02d", 
                (int)(cur_time_count / 3600),
                (int)((cur_time_count % 3600) / 60),
                (int)(cur_time_count % 60)
                );
            
            time_txt.wstr_len = strlen((char*)time_str);
            MMI_STRNTOWSTR(time, time_txt.wstr_len, time_str, time_txt.wstr_len, time_txt.wstr_len);
            time_len = MMIAPICOM_Wstrlen(time);

            MMI_GetLabelTextByLang(TXT_TOTAL_TIME, &time_pre);
            MMIAPICOM_Wstrncpy(all_time, time_pre.wstr_ptr, time_pre.wstr_len);
            MMIAPICOM_Wstrcat(all_time, time);

            time_txt.wstr_len = time_pre.wstr_len + time_len;
            time_txt.wstr_ptr = all_time;
 
            GUILABEL_SetText(MMIASP_RECORDING_TIMING_LABEL_CTRL_ID, &time_txt, is_refresh); 
        }
    }
    
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
LOCAL void SetRecordingWinContent(void)
{
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    MMI_STRING_T            name_info = {0};
    MMI_STRING_T            phone_number_str = {0};
    MMI_STRING_T            hiden_number_info = {0};
    MMI_STRING_T            phone_number_info = {0};
    uint16                  name_len = 0;
#if defined MAINLCD_SIZE_240X320 || defined MAINLCD_SIZE_240X400
    GUI_FONT_T              lable_font = SONG_FONT_20;
#endif

    wchar               name[MMIASP_MAX_NAME_LEN+1] = {0};
    uint8               cur_call_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8               tel_num_len = CC_MAX_TELE_NUM_LEN + 2;
    wchar               phone_number_arr[MMIASP_MAX_STRING_LEN + 1] = {0};
    wchar               temp_str[MMIASP_MAX_STRING_LEN + 1] = {0};

    GUIFORM_CHILD_WIDTH_T sbs = {0};
#ifdef MMI_PDA_SUPPORT
    uint16 space = 0;
#endif
    //set form not support slide
    GUIFORM_IsSlide(MMIASP_RECORDING_FORM_CTRL_ID,FALSE);

    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = MMIASP_RECORDING_IMAGE_ANIM_CTRL_ID;
    control_info.ctrl_ptr = PNULL;
#ifdef MMI_RECORD_SUPPORT
    data_info.img_id = IMAGE_COMMON_RECORD_BG;
#endif
    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    GUIANIM_SetParam(&control_info,&data_info,&file_info,&display_info);

    if(MMITHEME_IsMainScreenLandscape())
    {
        //sbs.type = GUIFORM_CHILD_WIDTH_FIXED;
		sbs.type = GUIFORM_CHILD_WIDTH_AUTO;
#ifdef MMI_RECORD_SUPPORT
        GUIRES_GetImgWidthHeight(&sbs.add_data,PNULL,IMAGE_COMMON_RECORD_BG,MMIASP_RECORDING_WIN_ID);
#endif
        GUIFORM_SetChildWidth(MMIASP_RECORDING_FORM_CTRL_ID,MMIASP_RECORDING_IMAGE_ANIM_CTRL_ID,&sbs);
#ifdef MMI_PDA_SUPPORT
        GUIOWNDRAW_SetHeight(MMIASP_RECORDING_OWNDRAW_CTRL_ID, MMIASP_RECORDING_OWNDRAW_HEIGHT_H);
        space = MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT_H;
        GUIFORM_SetChildSpace(MMIASP_RECORDING_FORM_CTRL_ID, MMIASP_RECORDING_NAME_LABEL_CTRL_ID, PNULL, &space);
        space = MMIASP_PLAY_TEXT_SPACE_HEIGHT_H;
        GUIFORM_SetChildSpace(MMIASP_RECORDING_FORM_CTRL_ID, MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID, PNULL, &space);
        GUIFORM_SetChildSpace(MMIASP_RECORDING_FORM_CTRL_ID, MMIASP_RECORDING_TIMING_LABEL_CTRL_ID, PNULL, &space);
#endif
#if defined MAINLCD_SIZE_240X320 || defined MAINLCD_SIZE_240X400
#if defined MMI_ISTYLE_SUPPORT
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
             lable_font = SONG_FONT_14;
        }
        else
#endif			
        {
            lable_font = MMIASP_LANDSCAPE_TIME_FONT;
        }
        GUILABEL_SetFont(MMIASP_RECORDING_NAME_LABEL_CTRL_ID, lable_font, MMI_BLACK_COLOR);
        GUILABEL_SetFont(MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID, lable_font, MMI_BLACK_COLOR);
        GUILABEL_SetFont(MMIASP_RECORDING_TIMING_LABEL_CTRL_ID, lable_font, MMI_BLACK_COLOR);
#endif
    }
    else
    {
        sbs.type = GUIFORM_CHILD_WIDTH_AUTO;
        sbs.add_data = 100;
        GUIFORM_SetChildWidth(MMIASP_RECORDING_FORM_CTRL_ID,MMIASP_RECORDING_IMAGE_ANIM_CTRL_ID,&sbs);
#ifdef MMI_PDA_SUPPORT
#if defined MMI_ISTYLE_SUPPORT
        if (MMISET_IDLE_ISTYLE == MMIAPISET_GetIdleStyle())
        {
            GUIOWNDRAW_SetHeight(MMIASP_RECORDING_OWNDRAW_CTRL_ID, MMIASP_RECORDING_OWNDRAW_HEIGHT - MMI_STATUSBAR_HEIGHT);
        }
        else
#endif			
        {
            GUIOWNDRAW_SetHeight(MMIASP_RECORDING_OWNDRAW_CTRL_ID, MMIASP_RECORDING_OWNDRAW_HEIGHT);
        }
        space = MMIASP_RECORDING_NAME_LABLE_SPACE_HEIGHT;
        GUIFORM_SetChildSpace(MMIASP_RECORDING_FORM_CTRL_ID, MMIASP_RECORDING_NAME_LABEL_CTRL_ID, PNULL, &space);

        space = MMIASP_PLAY_TEXT_SPACE_HEIGHT;
        GUIFORM_SetChildSpace(MMIASP_RECORDING_FORM_CTRL_ID, MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID, PNULL, &space);
        GUIFORM_SetChildSpace(MMIASP_RECORDING_FORM_CTRL_ID, MMIASP_RECORDING_TIMING_LABEL_CTRL_ID, PNULL, &space);
#endif
    }
    
    
    // 
    name_len = GetShowCallName(name, MMIASP_MAX_STRING_LEN);
    name_info.wstr_len = name_len;
    name_info.wstr_ptr = name;
    GUILABEL_SetText(MMIASP_RECORDING_NAME_LABEL_CTRL_ID, &name_info, FALSE);

     // 号码:
    MMI_GetLabelTextByLang(TXT_PHONE_NUMBER, &phone_number_str);
    tel_num_len=MMIAPICC_GetCallNumber(cur_call_num, tel_num_len);
    MMIAPICOM_StrToWstr(cur_call_num, phone_number_arr);
    
    SCI_MEMCPY(temp_str,  phone_number_str.wstr_ptr, phone_number_str.wstr_len*2);

    if(tel_num_len>0)
    {
        MMIAPICOM_Wstrcpy(temp_str + phone_number_str.wstr_len, phone_number_arr);
        phone_number_info.wstr_len = phone_number_str.wstr_len + MMIAPICOM_Wstrlen(phone_number_arr);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_UNKNOW_NUM,  &hiden_number_info);
        SCI_MEMCPY(temp_str + phone_number_str.wstr_len, hiden_number_info.wstr_ptr, hiden_number_info.wstr_len*2);
        phone_number_info.wstr_len = phone_number_str.wstr_len + hiden_number_info.wstr_len;
    }

    phone_number_info.wstr_ptr = temp_str;
    GUILABEL_SetText(MMIASP_RECORDING_NUMBER_LABEL_CTRL_ID, &phone_number_info, FALSE);
    // 时长
    UpdateRecordingTime(FALSE);
      
   
    
}
/*****************************************************************************/
//  Description : open 答录的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_OpenRecordingWin(void)
{
	/*lint -e529*/
     BOOLEAN   result  = FALSE;
    //打开播放器 
    MMK_CreateWin((uint32 *)MMIASP_RECORDING_WIN_TAB, PNULL);  
    //MMK_CreateWin((uint32 *)MMIASP_RECORDING_WIN_TAB_H, PNULL); 
    result=MMK_CreateCtrlByWinTabEx( MMIASP_RECORDING_WIN_ID,  (MMITHEME_IsMainScreenLandscape() ? MMIASP_RECORDING_WIN_TAB_H : MMIASP_RECORDING_WIN_TAB_V) );/*lint !e605*/

    if(!result)
    {
       MMIAPICC_ReleaseAspCallReq(MN_CAUSE_NORMAL_CLEARING);
    }
    SetRecordingWinContent();
	/*lint -e529*/ 
}
/*****************************************************************************/
//  Description : close 答录的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_CloseRecordingWin(void)
{
    MMK_PostMsg(MMIASP_RECORDING_WIN_ID, MSG_ASP_CLOSE_RECORD_WIN, PNULL, 0);
}

/*****************************************************************************/
//  Description : close 正在释放的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_CloseReleasingWin(void)
{
    MMK_PostMsg(MMIASP_CALL_RELEASING_WIN_ID, MSG_ASP_CLOSE_RELEASING_WIN, PNULL, 0);
}

/*****************************************************************************/
//  Description : open 答录的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_OpenRingSelectWin(PRINGSELECT_CALLBACKFUNC callback_fun_ptr)
{
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_CALL_USING);
    }
    else if(!IsUDiskRunning()) //add by kunliu 2011.06.08
    {
        if(PNULL != callback_fun_ptr)
        {
            MMK_CreateWin((uint32*)MMIASP_SETTING_SELECT_RING_WIN_TAB, (ADD_DATA)callback_fun_ptr); /*lint !e611*/
        }
        else
        {
            MMK_CreateWin((uint32*)MMIASP_SETTING_SELECT_RING_WIN_TAB, PNULL);
        }
    }
}
/*****************************************************************************/
//  Description : handle  list item delete option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleMSGListDeleteOperation(void)
{
    MMIASP_APPLET_T* applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    uint16         selected_idx[MMIASP_MAX_MSG_NUM + 1] = {0};
    MMI_CTRL_ID_T  list_ctrl_id                         = MMIASP_MSG_LIST_CTRL_ID;
    MMIASP_MSG_INFO_T*  select_node_ptr                 = PNULL;
    BOOLEAN   is_mult_delete                            = FALSE;
    uint16    item_index                                = 0;
    uint16    mark_num                                  = 0; 
    uint16    i                                         = 0;
    
    //get mark num
    mark_num = GUILIST_GetSelectedItemIndex(list_ctrl_id, selected_idx, MMIASP_MAX_MSG_NUM);
    if(PNULL != applet_ptr)
    {
	    if (0 == mark_num)
	    {
	        //ger current index
	        item_index = GUILIST_GetCurItemIndex(list_ctrl_id);
	        select_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, item_index);
	        if(PNULL != select_node_ptr)
	        {
	            select_node_ptr->mark_flag = TRUE;
	        }
	        // 先从list 上移掉
	        GUILIST_RemoveItem(list_ctrl_id, item_index);
	    }
	    else
	    {    
	        for(i = 0; i < mark_num; i++)
	        {
	            select_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, selected_idx[i]);
	            if(PNULL != select_node_ptr)
	            {
	                select_node_ptr->mark_flag = TRUE;
	            }
	        }

	        is_mult_delete = TRUE;
	        
	    }
	 
	    // 从链表上移掉.....
	    MMIASP_DelMarkNodeFromList(applet_ptr);
	    MMIASP_WriteSysFile();
	    GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
	    if(is_mult_delete)
	    {
	        applet_ptr->cur_gui_list_index = 0;
	        MMK_SendMsg(MMIASP_MSG_LIST_WIN_ID, MMIASP_MSG_UPDATELIST, PNULL);
	    }
	    if(MMK_IsOpenWin(MMIASP_NEWMSG_WIN_ID))
	    {
	        MMK_CloseWin(MMIASP_NEWMSG_WIN_ID);
	        MMIASP_ShowNewAspMsgWin();
	    }
    }
    // 提示已删除//必须要已删除提示，否则当多项删除时可能会有cr204119
    //MMIPUB_OpenAlertSuccessWin( TXT_DELETED);
}
/*****************************************************************************/
//  Description : handle msg delete option
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL void HandleMSGDeleteOperation(void)
{
    MMIASP_APPLET_T* applet_ptr           = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    MMI_CTRL_ID_T  list_ctrl_id           = MMIASP_MSG_LIST_CTRL_ID;
    MMIASP_MSG_INFO_T*  select_node_ptr   = PNULL;
    uint16    item_index                  = 0;
    
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP HandleMSGDeleteOperation no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5128_112_2_18_2_2_54_174,(uint8*)"");
        return;
    }
    //ger current index
    item_index = applet_ptr->cur_gui_list_index;
    select_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, item_index);
    if(PNULL != select_node_ptr)
    {
        select_node_ptr->mark_flag = TRUE;
    }
    //先停止播放
    MMIASP_Stop();
    // 先从list 上移掉
    GUILIST_RemoveItem(list_ctrl_id, item_index);
    // 从链表上移掉.....
    MMIASP_DelMarkNodeFromList(applet_ptr);
    MMIASP_WriteSysFile();
    GUILIST_SetTextListMarkable(list_ctrl_id, FALSE);
    if(applet_ptr->cur_gui_list_index>0)
    {
        applet_ptr->cur_gui_list_index--;
    }
    // 提示已删除
    //MMIPUB_OpenAlertSuccessWin( TXT_DELETED);

    //关闭播放窗口
    MMK_CloseWin(MMIASP_MAIN_PLAY_WIN_ID);
}
/*****************************************************************************/
//  Description : open 播放界面的界面
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   此函数专供从外部直接进入到播放界面(不是从留言列表界面进入的）
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_OpenPlayWinFromExternal(uint16 node_index)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    MMI_WINDOW_TABLE_CREATE_T create = {0};
    MMIASP_MSG_INFO_T*  cur_node_ptr = PNULL;
    uint16              list_num     = 0;

    //SCI_TRACE_LOW:"MMIASP MMIASP_OpenPlayWinFromExternal node_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5162_112_2_18_2_2_54_175,(uint8*)"d", node_index);
    
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP MMIASP_OpenPlayWinFromExternal find no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5166_112_2_18_2_2_54_176,(uint8*)"");
        return FALSE;
    }
    MMIASP_ReadSysFileWithApplet(applet_ptr);
    list_num = MMIASP_GetListMsgTotalNum(applet_ptr);
    if(node_index > list_num)
    {
        //SCI_TRACE_LOW:"MMIASP error index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5173_112_2_18_2_2_54_177,(uint8*)"");
        return FALSE;
    }
    
    applet_ptr->cur_gui_list_index = node_index;
    cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, node_index);
    if(PNULL != cur_node_ptr)
    {
        create.win_table_ptr = MMIASP_MAIN_PLAY_WIN_TAB;
        create.applet_handle = applet_ptr->caf_applet.app_handle;
        MMK_CreateWinTable( &create );  
        if(!cur_node_ptr->read_flag)
        {
            cur_node_ptr->read_flag = TRUE;
            MMIASP_WriteSysFile();
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIASP MMIASP_OpenPlayWinFromExternal no node"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5192_112_2_18_2_2_54_178,(uint8*)"");
        return FALSE;
    }

    return TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: junhu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN AppendOneListItem(
                                uint16           item_index,     //in:item索引
                                MMI_CTRL_ID_T    ctrl_id         //in:control id
                                )
{
    MMIASP_APPLET_T*        applet_ptr   = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    MMIASP_MSG_INFO_T*      cur_node_ptr = PNULL;
    GUILIST_ITEM_DATA_T     item_data              = {0};     /*lint !e64*/
    // 姓名
    wchar     item_name[MMIASP_MAX_NAME_LEN +1]    = {0};
    MMI_STRING_T  name_str                         = {0};
    // 日期和时间
    wchar  date_time[GUILIST_STRING_MAX_NUM +1]    = {0};
    wchar  time_wstr[GUILIST_STRING_MAX_NUM +1]    = {0};
    uint8  time_str[GUILIST_STRING_MAX_NUM+1]      = {0};
    uint8  date_str[GUILIST_STRING_MAX_NUM+1]      = {0};
    SCI_DATE_T    item_date                        = {0};
    SCI_TIME_T    item_time                        = {0};
    uint16        date_time_len                    =  0;
    
    BOOLEAN       result                           = FALSE;
    
    //SCI_TRACE_LOW:"MMIASP AppendOneListItem item_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5224_112_2_18_2_2_54_179,(uint8*)"d", item_index);
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP AppendOneListItem no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5227_112_2_18_2_2_54_180,(uint8*)"");
        return FALSE;
    }
    
    cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, item_index);
    if(PNULL == cur_node_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP AppendOneListItem no node item_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5234_112_2_18_2_2_54_181,(uint8*)"d", item_index);
        return FALSE;
    }

    item_data.softkey_id[0] = STXT_OPTION;
    item_data.softkey_id[1] = TXT_NULL;
    item_data.softkey_id[2] = STXT_RETURN;

    // 图片
    item_data.item_content[0].item_data_type = GUIITEM_DATA_IMAGE_ID;
    if(cur_node_ptr->read_flag)
    {
        item_data.item_content[0].item_data.image_id = IMAGE_ASP_ICON_CALL_RECORD_READ;
    }
    else
    {
        item_data.item_content[0].item_data.image_id = IMAGE_SECMENU_ICON_CALL_RECORD_UNREAD;
    }
    
    // 第一行文字
    name_str.wstr_ptr = item_name;
    name_str.wstr_len = 0;
    if(MMIASP_GetNameByNumber(cur_node_ptr->call_num, &name_str))
    { 
        //SCI_TRACE_LOW:"MMIASP this number has find a name "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5258_112_2_18_2_2_54_182,(uint8*)"");
    }
    else
    {
        MMIAPICOM_StrToWstr(cur_node_ptr->call_num, name_str.wstr_ptr);
    }
    name_str.wstr_len = MMIAPICOM_Wstrlen(name_str.wstr_ptr);
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = name_str.wstr_ptr;
    item_data.item_content[1].item_data.text_buffer.wstr_len = MIN( name_str.wstr_len, GUILIST_STRING_MAX_NUM );
    
    // 第二文字: 日期+时间
    item_date = cur_node_ptr->date_info.sys_date;
    item_time = cur_node_ptr->date_info.sys_time;
    // 日期
    MMIAPISET_FormatDateStrByDateStyle(item_date.year, item_date.mon, item_date.mday, 
        '/', date_str, GUILIST_STRING_MAX_NUM+1);
    MMIAPICOM_StrToWstr(date_str, date_time);
    // 时间
    MMIAPISET_FormatTimeStrByTime(item_time.hour,item_time.min,time_str,GUILIST_STRING_MAX_NUM);
    MMIAPICOM_StrToWstr(time_str, time_wstr);
    MMIAPICOM_Wstrcat(date_time, L"  ");
    MMIAPICOM_Wstrcat(date_time, time_wstr);
    date_time_len = MMIAPICOM_Wstrlen(date_time);
    
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = date_time;
    item_data.item_content[2].item_data.text_buffer.wstr_len = MIN( date_time_len, GUILIST_STRING_MAX_NUM );
    
    result = GUILIST_SetItemData(ctrl_id, &item_data, item_index);
    
    return result;
}
/*****************************************************************************/
//  Description : 自动播放答录留言.....
//  Global resource dependence : none
//  Author: jun.hu
//  Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIASP_AutoPlayRecordMessages(void)
{
    MMIASP_APPLET_T*    applet_ptr = (MMIASP_APPLET_T*)MMK_GetAppletPtr(SPRD_ANSMACHINE_APPLET_ID);
    MMI_WINDOW_TABLE_CREATE_T create = {0};
    MMIASP_MSG_INFO_T*  cur_node_ptr = PNULL;
    uint32              list_num     = 0;
    
    if(PNULL == applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIASP_AutoPlayRecordMessages find no applet"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5305_112_2_18_2_2_54_183,(uint8*)"");
        //MMIPUB_OpenAlertWarningWin(TXT_EMPTY_LIST);
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_COMMON_EMPTY_LIST);
        return FALSE;
    }
    
    list_num = MMIASP_GetRecordNum();
    if(0 == list_num)
    {
        //SCI_TRACE_LOW:"MMIASP error index"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5314_112_2_18_2_2_54_184,(uint8*)"");
        //MMIPUB_OpenAlertWarningWin(TXT_EMPTY_LIST);
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_COMMON_EMPTY_LIST);
        return FALSE;
    }
    
    MMIASP_ReadSysFileWithApplet(applet_ptr);
    
    applet_ptr->cur_gui_list_index = 0;
    cur_node_ptr = MMIASP_GetCurNodeByGuiIndex(applet_ptr, applet_ptr->cur_gui_list_index);
    if(PNULL != cur_node_ptr)
    {
        create.win_table_ptr = MMIASP_MAIN_PLAY_WIN_TAB;
        create.applet_handle = applet_ptr->caf_applet.app_handle;
        MMK_CreateWinTable( &create );      
        if(!cur_node_ptr->read_flag)
        {
            cur_node_ptr->read_flag = TRUE;
            MMIASP_WriteSysFile();
        }
    }
    return TRUE;
}
/*****************************************************************************/
//  Description :刷新录音界面记时时间
//  Global resource dependence : none
//  Author: koki gao
//  Note:   
/*****************************************************************************/
PUBLIC void MMIASP_UpdateRecordingTime(BOOLEAN is_refresh)
{
     UpdateRecordingTime( is_refresh);
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : handle Msg List LongOk ContextMenu
//  Global resource dependence : none
//  Author: jun.hu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMsgListLongOkOptMenuWinMsg(
													MMI_WIN_ID_T       win_id,     
													MMI_MESSAGE_ID_E    msg_id, 
													DPARAM              param
													)
{
	MMI_RESULT_E            result            = MMI_RESULT_TRUE;
	MMIASP_APPLET_T         * applet_ptr      = (MMIASP_APPLET_T*)MMK_GetAppletPtr( SPRD_ANSMACHINE_APPLET_ID );
    uint16                  total_num         = MMIASP_GetListMsgTotalNum(applet_ptr);
    uint16                  mark_num          = GUILIST_GetSelectedItemIndex(MMIASP_MSG_LIST_CTRL_ID, PNULL, 0);
	MMI_MENU_GROUP_ID_T     group_id          = MENU_ASP_MSGLOCALLIST_LONGOK_OPT;
	MMI_CTRL_ID_T           ctrl_id           = MMIASP_MSGLIST_LONGOK_OPT_MENU_CTRL_ID;
	MMI_CTRL_ID_T           list_ctrl_id      = MMIASP_MSG_LIST_CTRL_ID;
	GUIMENU_TITLE_INFO_T    title_info        = {0};
    GUIMENU_BUTTON_INFO_T   button_info       = {0};
	MMI_STRING_T            main_title        = {0};
    MMI_STRING_T            sub_title         = {0};
	MMI_MENU_ID_T           menu_id           = {0};
	MMIASP_MSG_INFO_T       * cur_node        = PNULL;
	uint8                   num_len           = NULL;
	MMI_STRING_T            string_to         = {0};
	MN_DUAL_SYS_E           sim_type          = MN_DUAL_SYS_MAX;
    MMI_WIN_ID_T            query_win_id      = MMIASP_QUERY_WIN_ID;
	uint16                  full_path_len     = 0;
	MMIFILE_FILE_INFO_T     file_info         = {0};
	cur_node = MMIASP_GetCurNodeByGuiIndex(applet_ptr, applet_ptr->cur_gui_list_index);
    //SCI_TraceLow:"MMIASP HandleAspMenuOperation index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5377_112_2_18_2_2_54_185,(uint8*)"d", applet_ptr->cur_gui_list_index);
	
    if (PNULL != cur_node)
    {    
        //MMIAPICOM_WstrToStr(cur_node->call_num,number);
        num_len = (uint8)strlen((char*)cur_node->call_num); 
    }
    else
    {
        //SCI_TraceLow:"HandleAspMenuOperation null Node"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5386_112_2_18_2_2_54_186,(uint8*)"");
    }
	
    //SCI_TraceLow:"MMIASP num_len  = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5389_112_2_18_2_2_54_187,(uint8*)"d", num_len);
    switch (msg_id)
    {
    case MSG_FULL_PAINT:
		{
			if(mark_num >=1  || 0 == total_num)
			{
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_CALL_AUDIO_ITEM_ID,TRUE);
#ifdef VT_SUPPORT
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_CALL_VIDEO_ITEM_ID,TRUE);
#endif
#ifdef MMI_IP_CALL_SUPPORT
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_CALL_IP_ITEM_ID, TRUE);
#endif
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_MSG_SMS_ITEM_ID, TRUE);
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_MSG_MMS_ITEM_ID, TRUE);
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_SEND_MMS_ITEM_ID, TRUE);
#if defined(BLUETOOTH_SUPPORT)  
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_SEND_BLUETOOTH_ITEM_ID, TRUE);
#endif
			}
			else
			{
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_CALL_AUDIO_ITEM_ID,FALSE);
#ifdef VT_SUPPORT
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_CALL_VIDEO_ITEM_ID,FALSE);
#endif
#ifdef MMI_IP_CALL_SUPPORT
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_CALL_IP_ITEM_ID, FALSE);
#endif
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_MSG_SMS_ITEM_ID, FALSE);
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_MSG_MMS_ITEM_ID, FALSE);
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_SEND_MMS_ITEM_ID, FALSE);
#if defined(BLUETOOTH_SUPPORT)  
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_SEND_BLUETOOTH_ITEM_ID, FALSE);
#endif
			}
			
			if (0 == total_num )
			{
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_DELETE_ITEM_ID,TRUE);
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_MARK_ITEM_ID,  TRUE);
			}
			else
			{
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_DELETE_ITEM_ID,FALSE);
				MMIAPICOM_EnableGrayed(win_id, MENU_ASP_MSGLOCALLIST_LONGOK_OPT,MMIASP_MENU_MARK_ITEM_ID,  FALSE);
			}
		}
		
	       //set title
        GetListItemTextInfo(list_ctrl_id, GUILIST_GetCurItemIndex(list_ctrl_id), &main_title, &sub_title);
        title_info.is_static    = TRUE;
        title_info.group_id     = group_id;
        title_info.title_ptr    = &main_title;
        title_info.sub_title_ptr = &sub_title; 
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
        
        //set context menu button style
        button_info.is_static   = TRUE;
        button_info.group_id    = group_id;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
		break;
		
	case MSG_CTL_MIDSK:  
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        
        //SCI_TRACE_LOW:"[ASP] HandleLocalListLongOkOptMenuWinMsg menu_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIASP_WINTAB_5458_112_2_18_2_2_54_188,(uint8*)"d",menu_id);
        switch (menu_id)
        {
		case MMIASP_MENU_CALL_AUDIO_ITEM_ID:
			if(PNULL != cur_node)
            {
                MMIAPICC_MakeCall(MMIAPISET_GetActiveSim(), cur_node->call_num, num_len, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
            }
            MMK_CloseWin(win_id);	
			break;
			
#ifdef VT_SUPPORT          
        case MMIASP_MENU_CALL_VIDEO_ITEM_ID:
            if(PNULL != cur_node)
            {
                MMIAPICC_MakeCall(MMIAPISET_GetActiveSim(), cur_node->call_num, num_len, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_VIDEO_CALL, PNULL);
            }
            MMK_CloseWin(win_id);
            break;
#endif

#ifdef MMI_IP_CALL_SUPPORT
        case MMIASP_MENU_CALL_IP_ITEM_ID:
            if(PNULL != cur_node)
            {
                MMIAPICC_MakeCall(MMIAPISET_GetActiveSim(), cur_node->call_num, num_len, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_IP_CALL, PNULL);
            }
            MMK_CloseWin(win_id);
            break;
#endif

			/* Write and send new message by the phone number */
        case MMIASP_MENU_MSG_SMS_ITEM_ID:
            {
                if ( (num_len > 0)&&(PNULL != cur_node))
                {
					//调用sms接口函数
                    MMIAPISMS_WriteNewMessage(MMIAPISET_GetActiveSim(), PNULL,cur_node->call_num, num_len);
                }
                else
                {
                    //提示窗口没有号码
					MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
                }
                MMK_CloseWin(win_id);            
            }
            break;
			
			// 回复彩信...
        case MMIASP_MENU_MSG_MMS_ITEM_ID:
            {
                if ( (num_len > 0)&&(PNULL != cur_node))
                {
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APP(num_len * sizeof(wchar));
					
                    SCI_MEMSET(string_to.wstr_ptr, 0x00, (num_len * sizeof(wchar)));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, cur_node->call_num, num_len, num_len);
                    string_to.wstr_len = num_len;
                    MMIAPIMMS_AnswerMMS(sim_type, PNULL, &string_to);
                    SCI_FREE(string_to.wstr_ptr);
                    string_to.wstr_ptr = PNULL;
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);  
                }
                MMK_CloseWin(win_id);
            }
            break;
			
			/* Delete the item which you have been select */
        case MMIASP_MENU_DELETE_ITEM_ID:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY,  &query_win_id, HandleQueryWinMsg);
            MMK_CloseWin(win_id);
            break;
			
            /* Send mms message */
        case MMIASP_MENU_SEND_MMS_ITEM_ID:
            {
                if(PNULL != cur_node)
                {
                    full_path_len = MMIAPICOM_Wstrlen(cur_node->record_path);
                    if(MMIAPIFMM_IsFileExist(cur_node->record_path, full_path_len))
                    {
                        MMIAPIFMM_GetFileInfoFormFullPath(cur_node->record_path,full_path_len,&file_info);
                        MMIAPIMMS_EditMMSFromContent(PNULL, PNULL, &file_info, MMIFMM_FILE_TYPE_MUSIC);
                    }
                    else
                    {
                        MMIAPIMMS_EditMMSFromContent(PNULL, PNULL, PNULL, MMIFMM_FILE_TYPE_NORMAL);
                    }
                }
                MMK_CloseWin(win_id);
            }
            break;

#if defined(BLUETOOTH_SUPPORT)  			
            /* Send  message by bluetooth */
        case MMIASP_MENU_SEND_BLUETOOTH_ITEM_ID:
            {
                if(PNULL != cur_node)
                {
                    MMIASP_SendRecordFile((const wchar*)cur_node->record_path);
                }
                MMK_CloseWin(win_id);
                break;
            }
#endif
		default:
			result = MMI_RESULT_FALSE;
			break;
		}
		break;

	case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
		
    default:
		result = MMI_RESULT_FALSE;
        break;
	}
    return result;
}
#endif

/*****************************************************************************/
// 	Description : 获取指定LIST项的文本信息
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetListItemTextInfo(
                                  MMI_CTRL_ID_T ctrl_id,    //[in]  控件ID
                                  uint16        index,      //[in]  索引
                                  MMI_STRING_T  *main_text,  //[out] 主标题
                                  MMI_STRING_T  *sub_text    //[out] 副标题
                                  )
{
    BOOLEAN                   return_code  = FALSE;
    const GUILIST_ITEM_T      *item_t_ptr  = PNULL;
    const GUIITEM_STYLE_T           *style_t_ptr = PNULL;
    int16                     main_index   = 0;
    int16                     sub_index    = 0;
    
    item_t_ptr = GUILIST_GetItemPtrByIndex(ctrl_id, index);
    if(PNULL != item_t_ptr)
    {
        //通过style获取文本数据的索引位置
        style_t_ptr = MMITHEME_GetItemStyle(item_t_ptr->item_style);
        if(PNULL != style_t_ptr)
        {
            //主文本信息
            main_index = style_t_ptr->main_index;
            if(main_index >= 0)
            {
                main_text->wstr_ptr = item_t_ptr->item_data_ptr->item_content[main_index].item_data.text_buffer.wstr_ptr;
                main_text->wstr_len = item_t_ptr->item_data_ptr->item_content[main_index].item_data.text_buffer.wstr_len;
                
                return_code = TRUE;
            }
            
            //副文本信息
            sub_index = style_t_ptr->auxiliary_index;
            if(sub_index >= 0)
            {
                sub_text->wstr_ptr = item_t_ptr->item_data_ptr->item_content[sub_index].item_data.text_buffer.wstr_ptr;
                sub_text->wstr_len = item_t_ptr->item_data_ptr->item_content[sub_index].item_data.text_buffer.wstr_len;
            }
        }
    }
    
    return return_code;
}

#if defined(BLUETOOTH_SUPPORT)  
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void  MMIASP_SendRecordFile(const wchar *record_path_ptr)
{
	uint32        file_size  = 0;
	uint32        record_path_len = 0;
	MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};
	
	if(PNULL == record_path_ptr)
	{	
		return;
	}
    record_path_len = MMIAPICOM_Wstrlen(record_path_ptr);
      
    MMIAPIFMM_GetFileInfo(record_path_ptr, record_path_len,&file_size,PNULL,PNULL );
	MMIAPICOM_Wstrncpy((wchar *)(send_file_info.filepath_name),record_path_ptr,record_path_len);
	send_file_info.filepath_len = record_path_len;
	send_file_info.file_size = file_size;
	send_file_info.is_temp_file = FALSE;
    MMIAPIBT_SendMultiFile(&send_file_info,1);
}
#endif


/*Edit by script, ignore 2 case. Thu Apr 26 19:00:48 2012*/ //IGNORE9527


/*Edit by script, ignore 8 case. Fri Apr 27 09:38:48 2012*/ //IGNORE9527
