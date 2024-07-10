/*****************************************************************************
** File Name:      mmipicview_wintab.c                                       *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIPICVIEW_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pic_viewer_trc.h"
#ifdef WIN32 
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "window_parse.h"
#include "mmk_type.h"
#include "guilistbox.h"
#include "guiform.h"
#include "guilabel.h"
#include "guirichtext.h"
#include "guidropdownlist.h"
#include "mmipicview_export.h"
#include "mmipicview_internal.h"
#include "mmipicview_id.h"
#include "mmipicview_menutable.h"
#include "mmipicview_position.h"
#include "mmipicview_text.h"
#include "mmipicview_image.h"
#include "mmidisplay_data.h"
#include "mmifilearray_export.h"
#include "mmi_theme.h"
#include "mmitheme_special_func.h"
#include "mmimms_export.h"
#include "mmipub.h"
#include "mmiudisk_export.h"
#include "mmifmm_export.h"
#include "mmi_default.h"
#include "mmipic_edit_export.h"
#include "mmidc_export.h"
#include "mmidc_setting.h"
#include "mmidc_save.h"
#include "mmi_mainmenu_export.h"
#include "guires.h"
#include "mmk_timer.h"
#include "mmibt_export.h"
#include "guiedit.h"
#include "guistring.h"
#include "guictrl_api.h"
#include "mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_wallpaper_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"

#ifdef CMCC_UI_STYLE
#include "mmidl_export.h"
#endif

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif

#ifdef WRE_SUPPORT
#include "mmiwre.h"
#endif

#ifdef BAIDU_DRIVE_SUPPORT
#include "mmibaidu_export.h"
#endif

#if defined (WIFI_SUPPORT) && !defined(WIFI_SUPPORT_UNISOC_RX)
#include "mmiwifi_export.h"
#endif

#include "watch_commonwin_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIPICVIEW_FILE_SIZE_MAX_LEN    12  //****.**** K

//file suffix max len
#define MMIPICVIEW_DETAIL_STR_MAX_LEN       20

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef SNS_SUPPORT
static IMMISns  *pMe = PNULL;
#endif
LOCAL BOOLEAN   s_mmipicview_is_preview_del = FALSE;        //is preview delete
LOCAL BOOLEAN   s_mmipicview_del_file_is_cancelled = 0;     //delete file is canceled
LOCAL uint8     s_mmipicview_slide_play_timer_id = 0;       //slide play timer
LOCAL uint16    s_mmipicview_del_file_index = 0;            //delete the picture index

LOCAL const uint32  s_mmipicview_slide_time[MMIPICVIEW_TIME_TYPE_MAX] = {2000,3000,6000,10000,16000,20000}; //slide time
#ifdef PIC_EDITOR_SUPPORT
LOCAL uint16 s_editpic_index = 0;//add for cr NEWMS00108303
#endif
#ifdef CMCC_UI_STYLE
LOCAL MMIPICVIEW_PATH_TYPE_E s_mmipicview_path_type = MMIPICVIEW_PHOTO;
#endif

LOCAL uint16    s_mmipicview_current_download_pic_idx = 0 ;
LOCAL MMIPICVIEW_DEFAULT_PATH_T   download_path = {0};
LOCAL PIC_SOURCE_TYPE s_mmipicview_show_folder = PICVIEW_SOURCE_NONE;

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
//MMIPICVIEW_DEFAULT_PATH_T g_mmipicview_dc_path = {0};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifndef PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle list folder option win msg
//  Global resource dependence : 
//  Author: kevin.lou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicFolderlistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );
#endif
/*****************************************************************************/
//  Description : handle list preview option win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePiclistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );

// /*****************************************************************************/
// //  Description : handle list preview mark option win msg
// //  Global resource dependence : 
// //  Author: Jassmine
// //  Note: 
// /*****************************************************************************/
// LOCAL MMI_RESULT_E HandlePiclistMarkWinMsg(
//                                           MMI_WIN_ID_T        win_id,
//                                           MMI_MESSAGE_ID_E    msg_id,
//                                           DPARAM              param
//                                           );
                                          
/*****************************************************************************/
//  Description : set list opt menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayListOptMenu(
                              MMI_CTRL_ID_T     list_ctrl_id
                              );

// /*****************************************************************************/
// //  Description : set list MARK opt menu gray
// //  Global resource dependence : 
// //  Author: Jassmine
// //  Note: 
// /*****************************************************************************/
// LOCAL void SetGrayListMarkMenu(
//                               MMI_CTRL_ID_T     list_ctrl_id
//                               );
#endif

/*****************************************************************************/
//  Description : handle picture slide play win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicSlidePlayWinMsg(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            );

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSlidePlayAnim(
                               uint32           item_index, //from 0
                               MMI_CTRL_ID_T    ctrl_id
                               );

/*****************************************************************************/
//  Description : start slide play timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StartSlidePlayTimer(
                               MMI_WIN_ID_T   win_id
                               );

/*****************************************************************************/
//  Description : stop slide play timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StopSlidePlayTimer(void);

/*****************************************************************************/
//  Description : get the first mark picture index
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFirstMarkIndex(//is mark
                                uint32          *index_ptr,     //in[current index]/out[the first mark index]
                                uint32          toatal_num,     //in
                                MMI_CTRL_ID_T   list_ctrl_id    //in
                                );

/*****************************************************************************/
//  Description : switch the next picture
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NextSlidePic(//is change
                           uint32            cur_index,          //in:from
                           uint32            total_num,          //in
                           MMI_CTRL_ID_T     ctrl_id,            //in
                           MMI_CTRL_ID_T     list_ctrl_id        //in
                           );

/*****************************************************************************/
//  Description : switch the next marked picture
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NextMarkedPic(//is change
                            uint16          *mark_num_ptr,      //in/out:may PNULL
                            uint32          cur_index,          //in:from 0
                            uint32          toatal_num,         //in
                            MMI_CTRL_ID_T   ctrl_id,            //in
                            MMI_CTRL_ID_T   list_ctrl_id        //in
                            );

/*****************************************************************************/
//  Description : handle preview option win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreviewOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : set preview opt menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayPreviewOptMenu(
                                 MMI_CTRL_ID_T      list_ctrl_id
                                 );

/*****************************************************************************/
//  Description : send picture by mms
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SendPicByMms(
                        uint32  item_index  //from 0
                        );

/*****************************************************************************/
//  Description : send picture by blue tooth
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SendPicByBt(
                       uint32  item_index  //from 0
                       );

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : send picture to sns
//  Global resource dependence : 
//  Author: wei.ren
//  Note: 
/*****************************************************************************/
LOCAL void SendPicToSNS(
                       uint32  item_index ,
                       uint32  sns_index  //from 0
                       );
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle slide set win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicSlideSetWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                           );

/*****************************************************************************/
//  Description : set slide set param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetSlideSettings(void);

/*****************************************************************************/
//  Description : get slide effect index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideEffectIndex(void);

/*****************************************************************************/
//  Description : set slide effect index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetSlideEffectIndex(
                               uint16   cur_index
                               );
#endif
/*****************************************************************************/
//  Description : get slide time
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSlideTime(void); //return ms
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get slide time index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideTimeIndex(void);

/*****************************************************************************/
//  Description : set slide time index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetSlideTimeIndex(
                             uint16   cur_index
                             );

/*****************************************************************************/
//  Description : save slide set param
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SaveSlideSet(void);
#endif
/*****************************************************************************/
//  Description : set wallpaper
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetWallpaper(
                        uint32  item_index,  //from 0
                        MMI_CTRL_ID_T  ctrl_id
                        );
/*****************************************************************************/
//  Description : set power on/off anim
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetPowerAnim(
                        uint32  item_index,  //from 0
                        BOOLEAN is_power_on,
                        MMI_CTRL_ID_T  ctrl_id
                        );
/*****************************************************************************/
//  Description : set picture anim control param
//  Global resource dependence :
//  Author: langhua.yang
//  Note:
/*****************************************************************************/
BOOLEAN MMIPICVIEW_SetSelPicParam (
    const wchar          *full_path_wstr_ptr,//in
    uint16         full_path_wstr_len, //in:字节数
    MMI_CTRL_ID_T  ctrl_id
);
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : set PB contatct icon
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPhontbook(
                        uint32  item_index,  //from 0
                        MMI_CTRL_ID_T  ctrl_id
                        );
#endif
                 
#ifdef PICVIEW_VIEW_ZOOM

/*****************************************************************************/
//  Description : zoom picture
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void ZoomPic(
                   uint32   item_index  //from 0
                   );
#endif
#ifdef PIC_EDITOR_SUPPORT
/*****************************************************************************/
//  Description : edit picture
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void EditPic(
                   uint32  item_index  //from 0
                   );
#endif
/*****************************************************************************/
//  Description : handle picture list mark option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// LOCAL void HandlePicListMarked(
//                                MMI_CTRL_ID_T  list_ctrl_id
//                                );

/*****************************************************************************/
//  Description : handle picture list mark cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void HandlePicListUnmarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 );

/*****************************************************************************/
//  Description : handle picture list mark all option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// LOCAL void HandlePicListMarkedAll(
//                                   MMI_CTRL_ID_T   list_ctrl_id
//                                   );

/*****************************************************************************/
//  Description : handle picture list mark all cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
// LOCAL void HandlePicListUnmarkedAll(
//                                     MMI_CTRL_ID_T list_ctrl_id
//                                     );

/*****************************************************************************/
//  Description : handle picture preview delete msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePicPreviewDelQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleProcessWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       );

/*****************************************************************************/
//  Description : cancel delete    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StopDelete(void);

/*****************************************************************************/
//  Description : delete selected pictures
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void DeleteSelectedPic(
                             MMI_CTRL_ID_T      list_ctrl_id,   //in
                             MMI_WIN_ID_T       wait_win_id,    //in
                             MMI_WIN_ID_T       win_id          //in
                             );

/*****************************************************************************/
//  Description : handle delete selected pictures confirm message
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void HandleDeletePicCnf(
                              BOOLEAN           is_success,     //in:delete is success
                              MMI_CTRL_ID_T     list_ctrl_id,   //in:
                              MMI_WIN_ID_T      wait_win_id,    //in:
                              MMI_WIN_ID_T      win_id          //in:
                              );
#if 0
/*****************************************************************************/
//  Description : handle delete picture success alert window
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteSuccAlertWin(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            );
#endif
/*****************************************************************************/
//  Description : handle rename win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicRenameWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         );

/*****************************************************************************/
//  Description : set initial name
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetInitName(
                       MMI_CTRL_ID_T    ctrl_id,    //in
                       MMI_CTRL_ID_T    list_ctrl_id//in
                       );

/*****************************************************************************/
//  Description : rename picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenamePic(
                        MMI_CTRL_ID_T  ctrl_id,    //in
                        MMI_CTRL_ID_T  list_ctrl_id//in
                        );

/*****************************************************************************/
//  Description : update picture list array
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void UpdateListArray(
                           uint16   index,          //in
                           wchar    *new_name_ptr,  //in:full name
                           uint16   new_name_len    //in:full name
                           );

/*****************************************************************************/
//  Description : handle detail win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicDetailWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         );

/*****************************************************************************/
//  Description : set detail
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetDetail(
					 MMI_WIN_ID_T	win_id,
                     MMI_CTRL_ID_T  ctrl_id,
                     MMI_CTRL_ID_T  list_ctrl_id
                     );

/*****************************************************************************/
//  Description : add fix string in rich text
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AddFixStrItem(
                         uint16             *index_ptr, //in/out
                         MMI_TEXT_ID_T      text_id,
                         GUIRICHTEXT_ITEM_T *richtext_item_ptr,
                         MMI_CTRL_ID_T      ctrl_id
                         );

/*****************************************************************************/
//  Description : add string in rich text
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AddStrItem(
                      const uint8           *str_ptr,           //in
                      uint16                str_len,            //in
                      uint16                *index_ptr,         //in/out
                      GUIRICHTEXT_ITEM_T    *richtext_item_ptr, //in
                      MMI_CTRL_ID_T         ctrl_id
                      );

/*****************************************************************************/
//  Description : open set default path win
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void OpenSetDefaultPathWin(void);

/*****************************************************************************/
//  Description : open set default path callback
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void OpenSetDefaultPathCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      );

#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : handle list preview longok menu win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePiclistLongokOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );

/*****************************************************************************/
//  Description : set list longok opt menu gray
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayListLongokOptMenu(
                              MMI_CTRL_ID_T     list_ctrl_id
                              );
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle pda style setting win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicPdaSettingWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          );                                         
/*****************************************************************************/
//  Description : init pda style settings list ctrl
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void InitPicPdaSettingsListCtrl(void);
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void AppendPicPdaSettingsItem(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_TEXT_ID_T  text_2,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 );
#endif                                 
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : get text_id by view type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByViewType(MMIPICVIEW_VIEW_TYPE_E view_type);
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get text_id by effect type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByEffectType(MMIPICVIEW_EFFECT_TYPE_E effect_type);

/*****************************************************************************/
//  Description : get text_id by slide time type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByTimeType(MMIPICVIEW_TIME_TYPE_E time_type);
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : handle view set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicViewSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendViewListItem(void);
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle effect set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicEffectSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendEffectListItem(void);

/*****************************************************************************/
//  Description : handle effect set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicTimeSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       );

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendTimeListItem(void);
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Discription: append split item
//  Global resource dependence: none 
//  Author: xin.li
//  Note:
//
/*****************************************************************************/
LOCAL void AppendSplitListItem(	
                               MMI_CTRL_ID_T    ctrl_id,
                               MMI_TEXT_ID_T    text_id
                               );
#endif
/**--------------------------------------------------------------------------*
 **                         CONSTANT VARIABLES                               *
 **--------------------------------------------------------------------------*/

#ifndef PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
//picture folder list option
WINDOW_TABLE( MMIPICVIEW_FOLDER_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePicFolderlistOptWinMsg),
    WIN_ID(MMIPICVIEW_PICFOLDER_LIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    CREATE_POPMENU_CTRL(MENU_PICVIEW_FOLDER_LIST_OPT,MMIPICVIEW_FOLDER_LIST_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
//picture viewer list option
WINDOW_TABLE( MMIPICVIEW_LIST_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePiclistOptWinMsg),
    WIN_ID(MMIPICVIEW_LIST_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    CREATE_POPMENU_CTRL(MENU_PICVIEW_LIST_OPT,MMIPICVIEW_LIST_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    CREATE_ANIM_CTRL (MMIPICVIEW_SEL_PICTURE_CTRL_ID, MMIPICVIEW_LIST_WIN_ID),
    END_WIN
};

//picture viewer list option
// WINDOW_TABLE( MMIPICVIEW_LIST_MARK_WIN_TAB ) = 
// {
//     WIN_FUNC( (uint32)HandlePiclistMarkWinMsg),
//     WIN_ID(MMIPICVIEW_LIST_MARK_WIN_ID),
//     WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
//     CREATE_POPMENU_CTRL(MENU_PICVIEW_MARK_OPT,MMIPICVIEW_LIST_MARK_MENU_CTRL_ID),
//     WIN_SOFTKEY(STXT_OK,TXT_NULL,STXT_RETURN),
//     END_WIN
// };
#endif
//picture preview option
WINDOW_TABLE( MMIPICVIEW_PREVIEW_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePreviewOptWinMsg),
    WIN_ID(MMIPICVIEW_PREVIEW_OPT_WIN_ID),
#ifdef MMI_GUI_STYLE_MINISCREEN
    WIN_TITLE(STXT_OPTION),
#endif
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    CREATE_POPMENU_CTRL(MENU_PICVIEW_PREVIEW_OPT,MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID),
    CREATE_ANIM_CTRL (MMIPICVIEW_SEL_PICTURE_CTRL1_ID, MMIPICVIEW_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//slide play
WINDOW_TABLE( MMIPICVIEW_SLIDE_PLAY_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicSlidePlayWinMsg), 
    WIN_ID( MMIPICVIEW_SLIDE_PLAY_WIN_ID ),
    WIN_STYLE( WS_DISABLE_COMMON_BG),
    WIN_HIDE_STATUS,
    CREATE_ANIM_CTRL(MMIPICVIEW_SLIDE_PLAY_ANIM_CTRL_ID,MMIPICVIEW_SLIDE_PLAY_WIN_ID),
    END_WIN
};
#ifndef MMI_PICVIEW_MINI_SUPPORT
//slide set
WINDOW_TABLE( MMIPICVIEW_SLIDE_SET_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicSlideSetWinMsg), 
    WIN_ID( MMIPICVIEW_SLIDE_SET_WIN_ID ),
    WIN_TITLE(TXT_PICVIEW_SLIDE_SET),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIPICVIEW_SET_FORM_CTRL_ID),
        //effect
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPICVIEW_EFFECT_FORM_CTRL_ID,MMIPICVIEW_SET_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPICVIEW_EFFECT_LABEL_CTRL_ID,MMIPICVIEW_EFFECT_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID,MMIPICVIEW_EFFECT_FORM_CTRL_ID),
        //time
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIPICVIEW_TIME_FORM_CTRL_ID,MMIPICVIEW_SET_FORM_CTRL_ID),
            CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT,FALSE,MMIPICVIEW_TIME_LABEL_CTRL_ID,MMIPICVIEW_TIME_FORM_CTRL_ID),
            CHILD_DROPDOWNLIST_CTRL(TRUE,MMIPICVIEW_TIME_DROPDOWN_CTRL_ID,MMIPICVIEW_TIME_FORM_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
//rename
WINDOW_TABLE( MMIPICVIEW_RENAME_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicRenameWinMsg), 
    WIN_ID( MMIPICVIEW_RENAME_WIN_ID ),
    WIN_TITLE(TXT_COMMON_RENAME),
    CREATE_EDIT_TEXT_CTRL(0,MMIPICVIEW_RENAME_EDITBOX_CTRL_ID),
#ifdef PIC_PREVIEW_U200_SUPPORT
    WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//detail
WINDOW_TABLE( MMIPICVIEW_DETAIL_WIN_TAB ) =
{
    WIN_FUNC( (uint32)HandlePicDetailWinMsg), 
    WIN_ID( MMIPICVIEW_DETAIL_WIN_ID ),
    WIN_TITLE(STXT_DETAIL),
    CREATE_RICHTEXT_CTRL(MMIPICVIEW_DETAIL_RICHTEXT_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL,TXT_NULL,STXT_RETURN),
    END_WIN
};

//PDA style
#ifdef PIC_PREVIEW_U200_SUPPORT

WINDOW_TABLE( MMIPICVIEW_LIST_LONGOK_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandlePiclistLongokOptWinMsg),
    WIN_ID(MMIPICVIEW_LIST_LONGOK_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT|WS_DISABLE_COMMON_BG),
    CREATE_POPMENU_CTRL(MENU_PICVIEW_LIST_LONGOK_OPT,MMIPICVIEW_LIST_LONGOK_OPT_MENU_CTRL_ID),
    CREATE_ANIM_CTRL (MMIPICVIEW_SEL_PICTURE_CTRL2_ID, MMIPICVIEW_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
// PDA style 设置窗口
WINDOW_TABLE( MMIPICVIEW_PDA_SETTING_WIN_TAB ) = 
{
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,
#endif
    WIN_TITLE(STXT_MAIN_SETTING),
    WIN_FUNC((uint32)HandlePicPdaSettingWinMsg),
    WIN_ID(MMIPICVIEW_PDA_SETTING_WIN_ID),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIPICVIEW_PDA_SETTINGS_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    END_WIN
};
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
LOCAL WINDOW_TABLE( MMIVP_SETTING_VIEW_WIN_TAB ) = 
{
    WIN_STATUSBAR,
    WIN_TITLE( TXT_PICVIEW_VIEW_SETTING ),
    WIN_FUNC( (uint32)HandlePicViewSetWin),    
    WIN_ID( MMIPICVIEW_SETTING_VIEW_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPICVIEW_VIEW_SET_LIST_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
    END_WIN
};
#endif //PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
LOCAL WINDOW_TABLE( MMIVP_SETTING_EFFECT_WIN_TAB ) = 
{
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,
#endif
    WIN_TITLE( TXT_EFFECT ),
    WIN_FUNC( (uint32)HandlePicEffectSetWin),    
    WIN_ID( MMIPICVIEW_SETTING_EFFECT_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPICVIEW_EFFECT_SET_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};

LOCAL WINDOW_TABLE( MMIVP_SETTING_TIME_WIN_TAB ) = 
{
#ifndef MMI_GUI_STYLE_MINISCREEN
    WIN_STATUSBAR,
#endif
    WIN_TITLE( TXT_PICVIEW_SLIDE_TIME ),
    WIN_FUNC( (uint32)HandlePicTimeSetWin),    
    WIN_ID( MMIPICVIEW_SETTING_TIME_WIN_ID ),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPICVIEW_TIME_SET_LIST_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
	WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_CANCEL),
#else
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
#endif
    END_WIN
};
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: tao.xue
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_OpenPicViewerWin(void)
{
#ifdef BAIDU_DRIVE_SUPPORT
	PIC_SOURCE_TYPE source_type =  PICVIEW_SOURCE_NONE;
	MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
	BOOLEAN    b_is_network_ok = FALSE;
	uint8  i = 0;
	
       MMISRV_BAIDUDRV_Need_Get_Token();
#if defined (MMI_WIFI_SUPPORT) 
	if(MMIWIFI_STATUS_CONNECTED == MMIAPIWIFI_GetStatus())
	{
		b_is_network_ok = TRUE;
	}
	else
#endif			
	{
		for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
		{
			if (MMIAPIPHONE_IsSimAvailable(i)  && MMIAPIPHONE_IsAttachGprsSuccess(i))
			{
				b_is_network_ok = TRUE;
				break;
			}
		}
	}

	if(b_is_network_ok && MMIAPIBAIDU_IsAccountExist())
	{		
		MMIAPIPICVIEW_SetShowFolderPath(PICVIEW_SOURCE_CLOUD_THUMBNAIL);
		MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);			
	}
	else
	{
		MMK_CreateWin((uint32 *)WATCH_GALLERY_MAIN_WIN_TAB,PNULL);
	}
#else
#ifndef MMI_PICVIEW_MINI_SUPPORT
    MMK_CreateWin((uint32 *)MMIPREVIEW_PICFOLDER_WIN_TAB,PNULL); 
#else
    //128X160无pic_folder窗口，直接打开MMIPICVIEW_LIST_WIN_TAB
    MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_WIN_TAB,PNULL);  
#endif
#endif
}
#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewer(void)
{
   MMIAPIPICVIEW_OpenPicViewerExt(MMIPICVIEW_PHOTO);
}

/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewerExt(MMIPICVIEW_PATH_TYPE_E type)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
    }
    else
    {       
        s_mmipicview_path_type = type;
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_MYDOC, MMIPICVIEW_OpenPicViewerWin);
    }
}

/*****************************************************************************/
//  Description : open pic viewer in photos directory
//  Global resource dependence : 
//  Author: kevinlou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenMyPhotos(void)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
    }
    else
    {
        MMIAPIPICVIEW_EnterMyPhotos();     
    }
}

/*****************************************************************************/
//  Description : get path type
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_PATH_TYPE_E MMIPICVIEW_GetPathType(void)
{
    return s_mmipicview_path_type;
}

#else
/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewer(void)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
    }
    else
    {//bug117686
        MMIAPISET_ValidatePrivacyAppEntry(MMISET_PROTECT_MAX, MMIPICVIEW_OpenPicViewerWin);//MMISET_PROTECT_MYDOC,
    }
}

/*****************************************************************************/
//  Description : open pic viewer in photos directory
//  Global resource dependence : 
//  Author: kevinlou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenMyPhotos(void)
{
    if (MMIAPIUDISK_UdiskIsRun())
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
    }
    else
    {
        MMIAPIPICVIEW_EnterMyPhotos();     
    }
}
#endif
/*****************************************************************************/
//  Description : close pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ClosePicViewer(void)
{
    MMK_CloseMiddleWin( MMIPICVIEW_WIN_ID_START, MMIPICVIEW_MAX_WIN_ID );
}

/*****************************************************************************/
//  Description : open set default path win
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void OpenSetDefaultPathWin(void)
{
#if 0 //temp fix 2093098
    wchar                       *path_ptr = PNULL;
    uint16                      path_len = 0;
    FILEARRAY                   ret_array    = MMIAPIFILEARRAY_Create();
    MMI_CTRL_ID_T               list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMIFMM_FILTER_T             filter       = {0};
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};

    //set list enable mark
    MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);

    SCI_MEMCPY(filter.filter_str, "*", sizeof("*"));
    
    if (MMIAPIDC_IsOpened())
    {
        //set path
        path_ptr = g_mmipicview_dc_path.path;
        path_len = g_mmipicview_dc_path.path_len;
    }
    else
    {
        //get default path
        MMIAPIPICVIEW_GetDefaultPath(&default_path);

        //set path
        path_ptr = default_path.path;
        path_len = default_path.path_len;
    }

    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(path_ptr,
                                            path_len,
                                            &filter,
                                            FUNC_FIND_FILE_AND_FOLDER,
                                            FALSE,
                                            PNULL,
                                            ret_array,
                                            OpenSetDefaultPathCallBack,
                                            TRUE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
#endif
    return;
}

/*****************************************************************************/
//  Description : handle set default path alert win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
// LOCAL MMI_RESULT_E HandleSetDefaultPathAlertWinMsg(
//                                                    MMI_WIN_ID_T      win_id, 
//                                                    MMI_MESSAGE_ID_E  msg_id, 
//                                                    DPARAM            param
//                                                    )
// {
//     MMI_RESULT_E result = MMI_RESULT_TRUE;
// 
//     switch (msg_id)
//     {
//     case MSG_CLOSE_WINDOW:
//         //post update list msg
// #ifdef PIC_PREVIEW_U200_SUPPORT
//         MMK_PostMsg( MMIPICVIEW_PICFOLDER_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL, 0 ); 
// #else
//         MMK_PostMsg( MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL, 0 );
// #endif
// 
//         result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
//         break;
//         
//     default:
//         result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
//         break;
//     }
// 
//     return (result);
// }

/*****************************************************************************/
//  Description : open set default path callback
//  Global resource dependence : none
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
LOCAL void OpenSetDefaultPathCallBack(
                                      BOOLEAN   is_success, 
                                      FILEARRAY file_array
                                      )
{
    FILEARRAY_DATA_T          file_array_data = {0};
    MMIPICVIEW_DEFAULT_PATH_T default_path = {0};

    if (is_success)
    {
        if (0 < MMIAPIFILEARRAY_GetArraySize(file_array) && MMIAPIFILEARRAY_Read(file_array,0,&file_array_data ))
        {
            //get path
            MMIAPICOM_Wstrncpy(default_path.path, file_array_data.filename,file_array_data.name_len );

            default_path.path_len = file_array_data.name_len;
#if 0 //temp fix 2093098
            //set path
            if (MMIAPIDC_IsOpened())
            {
                //copy path
                g_mmipicview_dc_path.path_len = default_path.path_len;
                MMI_MEMCPY(g_mmipicview_dc_path.path,sizeof(wchar)*MMIFMM_PATHNAME_LEN,
                    default_path.path,sizeof(wchar)*MMIFMM_PATHNAME_LEN,
                    sizeof(wchar)*default_path.path_len);
            }
            else
            {
                MMIAPIPICVIEW_SetDefaultPath(&default_path);
            }
#endif
		//xin.li_clean_popwin
            //MMIPUB_OpenAlertWinByTextId( PNULL, TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, HandleSetDefaultPathAlertWinMsg ); 
// #ifdef PIC_PREVIEW_U200_SUPPORT
		MMK_PostMsg( MMIPICVIEW_PICFOLDER_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL, 0 ); 
// #else
// 		MMK_PostMsg( MMIPICVIEW_LIST_WIN_ID, MSG_PICVIEWER_RELOAD_FILE, PNULL, 0 );
// #endif
	
	}
        else
        {
            //SCI_TRACE_LOW:"OpenSetDefaultPathCallBack array size == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1073_112_2_18_2_46_43_74,(uint8*)"");
        }
    }
    else
    {
         //SCI_TRACE_LOW:"OpenSetDefaultPathCallBack failed"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1078_112_2_18_2_46_43_75,(uint8*)"");
    }

    //释放列表
    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}

PUBLIC void MMIAPIPICVIEW_SetShowFolderPath(
    PIC_SOURCE_TYPE  type
)
{
    s_mmipicview_show_folder = type;
}

PUBLIC PIC_SOURCE_TYPE MMIAPIPICVIEW_GetShowFolderPath(void)
{
    return s_mmipicview_show_folder;
}


#ifndef PIC_PREVIEW_U200_SUPPORT
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle list folder option win msg
//  Global resource dependence : 
//  Author: kevin.lou
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicFolderlistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_FOLDER_LIST_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetGrayListOptMenu(list_ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
/*        
        case ID_PICVIEW_CHANGE_FOLDER:
            OpenSetDefaultPathWin();

            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            break;

        case ID_PICVIEW_SLIDE_PREVIEW:
			MMIPICVIEW_SliePlay(list_ctrl_id);
            break;

        case ID_PICVIEW_SLIDE_SET:
            MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_SET_WIN_TAB,PNULL);
            break;
*/            
        case ID_PICVIEW_SETTING:
            //open setting win
            MMK_CreateWin((uint32 *)MMIPICVIEW_PDA_SETTING_WIN_TAB,PNULL);
            break;
        default:
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePicFolderlistOptWinMsg:menu_id %d is error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1138_112_2_18_2_46_43_76,(uint8*)"d", menu_id);
            break;
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description : handle list preview option win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePiclistOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    uint32              cur_index = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_LIST_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T       sel_anim_ctrl_id = MMIPICVIEW_SEL_PICTURE_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetGrayListOptMenu(list_ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
        CTRLANIM_SetVisible(sel_anim_ctrl_id, FALSE, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
#ifndef MMI_PICVIEW_MINI_SUPPORT
        case ID_PICVIEW_CHANGE_FOLDER:
            OpenSetDefaultPathWin();

            if(MMIAPIDC_IsOpened())
            {
                MMIAPIDC_SetChangeFolderState(TRUE);
            }
            break;
#endif
        case ID_PICVIEW_MMS:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByMms(cur_index);
            break;

        case ID_PICVIEW_BT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByBt(cur_index);
            break;
#ifdef SNS_SUPPORT
        case ID_PICVIEW_SHARE_TO_SINA:
        case ID_PICVIEW_SHARE_TO_FACEBOOK:
        case ID_PICVIEW_SHARE_TO_TIWTTER:
            {
                int share_id = 0;
                
                if(ID_PICVIEW_SHARE_TO_SINA == menu_id)
                {
                    share_id = MMI_SNS_SINA;
                }
                else if(ID_PICVIEW_SHARE_TO_FACEBOOK == menu_id)
                {
                    share_id = MMI_SNS_FACEBOOK;
                }
                else
                {
                    share_id = MMI_SNS_TIWTTER;
                }
                
                cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
                
                SendPicToSNS(cur_index, share_id);
            }
            break;
#endif            
        case ID_PICVIEW_SLIDE_PREVIEW:
			MMIPICVIEW_SliePlay(list_ctrl_id);
            break;
#ifndef MMI_PICVIEW_MINI_SUPPORT
        case ID_PICVIEW_SLIDE_SET:
            MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_SET_WIN_TAB,PNULL);
            break;
            
        case ID_PICVIEW_WALLPAPER:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SetWallpaper(cur_index, sel_anim_ctrl_id);
            break;
        case ID_PICVIEW_POWER_ON_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, TRUE, sel_anim_ctrl_id);
            break;
            
        case ID_PICVIEW_POWER_OFF_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, FALSE, sel_anim_ctrl_id);
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_PICVIEW_PB:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            
            SetPhontbook(cur_index, sel_anim_ctrl_id);
            break;
#endif

#ifdef PIC_EDITOR_SUPPORT
        case ID_PICVIEW_EDIT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            EditPic(cur_index);
            break;
#endif
#endif
        case ID_PICVIEW_MARKED:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            HandlePicListMarked(list_ctrl_id);
            MMIPICVIEW_SetSelectedItem(list_ctrl_id,cur_index,TRUE);
            break;

        case ID_PICVIEW_UNMARKED:
            HandlePicListUnmarked(list_ctrl_id);
            break;

        case ID_PICVIEW_MARKED_ALL:
            HandlePicListMarkedAll(list_ctrl_id);
            break;

        case ID_PICVIEW_UNMARKED_ALL:
            HandlePicListUnmarkedAll(list_ctrl_id);
            break;

        case ID_PICVIEW_DELETE:
#ifdef DRM_SUPPORT
            if (MMIPICVIEW_IsDeleteDrm(list_ctrl_id))
            {
                if (1 < MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0))
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
                else
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
            }
            else
#endif
			{
				//prompt query window
				MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
			}
            break;

        case ID_PICVIEW_RENAME:
            MMK_CreateWin((uint32 *)MMIPICVIEW_RENAME_WIN_TAB,PNULL);
            break;

        case ID_PICVIEW_DETAIL:
            MMK_CreateWin((uint32 *)MMIPICVIEW_DETAIL_WIN_TAB,PNULL);
            break;

        case ID_PICVIEW_VIEW_LIST:
            if (MMIPICVIEW_VIEW_LIST != MMIPICVIEW_GetViewType())
            {
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID,MSG_PICVIEWER_MODIFY_VIEW,PNULL);
            }
            break;

        case ID_PICVIEW_VIEW_ICONLIST:
            if (MMIPICVIEW_VIEW_ICONLIST != MMIPICVIEW_GetViewType())
            {
                MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID,MSG_PICVIEWER_MODIFY_VIEW,PNULL);
            }
            break;
         
//         case ID_PICVIEW_SETTING:
//             //open setting win
//             MMK_CreateWin((uint32 *)MMIPICVIEW_PDA_SETTING_WIN_TAB,PNULL);
//             break;

        default:
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePiclistOptWinMsg:menu_id %d is error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1335_112_2_18_2_46_43_77,(uint8*)"d", menu_id);
            break;
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set list opt menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayListOptMenu(
                              MMI_CTRL_ID_T     list_ctrl_id
                              )
{
    MMI_CTRL_ID_T           ctrl_id = MMIPICVIEW_LIST_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T     group_id = MENU_PICVIEW_LIST_OPT;
    GUIMENU_POP_SEL_INFO_T  pop_item_info = {0};    
    MMIPICVIEW_VIEW_TYPE_E  view_type = MMIPICVIEW_VIEW_LIST;

    if (0 < MMIPICVIEW_GetAllPicNum())
    {
        uint16                  cur_index = 0;
        uint16                  mark_num = 0;
        uint32                  pic_num = MMIPICVIEW_GetAllPicNum();
        FILEARRAY_DATA_T        file_info = {0};
        MMI_MENU_GROUP_ID_T     mark_group_id = MENU_PICVIEW_MARK;
        
        //get item index
        cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
        
        //current item is mark
        if (MMIPICVIEW_IsSelectItem(list_ctrl_id,cur_index))
        {
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_MARKED,TRUE);
        }
        else//NEWMS00172875
        {
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_UNMARKED,TRUE);
        }
        
        //get mark num
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
        if (0 < mark_num)
        {            
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)	    
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_SEND,TRUE);
#endif
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_WALLPAPER,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_ON_SET,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_OFF_SET,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_PB,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_RENAME,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DETAIL,TRUE);
            if(mark_num == pic_num)//NEWMS00172875
            {
                GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_MARKED_ALL,TRUE);
            }
        }
        else
        {
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_UNMARKED_ALL,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,mark_group_id,ID_PICVIEW_UNMARKED,TRUE);
            //get picture info,only jpg support picture editor
            if ((MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)) && 
                (GUIANIM_TYPE_IMAGE_JPG == MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len)))
            {
                //do nothing
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
            }
        }
        #ifdef CMCC_UI_STYLE
        if(MMIPICVIEW_DOWNLOAD_PIC == s_mmipicview_path_type)
        {
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_CHANGE_FOLDER,TRUE);
        }
        #endif
    }
    else
    {
#if defined (MMS_SUPPORT) || defined (BLUETOOTH_SUPPORT) || defined(SNS_SUPPORT)	    
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_SEND,TRUE);
#endif		
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_SLIDE_PREVIEW,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_WALLPAPER,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_ON_SET,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_OFF_SET,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_PB,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_MARKED,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DELETE,TRUE); 
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_RENAME,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DETAIL,TRUE); 

        #ifdef CMCC_UI_STYLE  //xuefang.jiang add 20110226 to gray change folder function.
        if(MMIPICVIEW_DOWNLOAD_PIC == s_mmipicview_path_type)
        {
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_CHANGE_FOLDER,TRUE);
        }
        #endif
    }

    //get view type
    view_type = MMIPICVIEW_GetViewType();

    //SCI_TRACE_LOW:"MMIPICVIEW SetGrayListOptMenu view_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1449_112_2_18_2_46_44_78,(uint8*)"d", view_type);
    
    switch (view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        pop_item_info.menu_id = ID_PICVIEW_VIEW_LIST;
        break;

    case MMIPICVIEW_VIEW_ICONLIST:
    default:
        pop_item_info.menu_id = ID_PICVIEW_VIEW_ICONLIST;
        break;
    }

    //set pop-up menu status
    pop_item_info.is_static = TRUE;
    pop_item_info.ctrl_id   = ctrl_id;
    pop_item_info.group_id  = MENU_PICVIEW_VIEW;
    GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);  
}
#endif

/*****************************************************************************/
// 	Description : picture slide play
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SliePlay(
								MMI_CTRL_ID_T	ctrl_id
								)
{
#ifdef DRM_SUPPORT
    uint32              item_index = MMIPICVIEW_GetCurIndex(ctrl_id);
    uint32              pic_num = MMIPICVIEW_GetAllPicNum();

    //if mark,display the first mark picture
    if (GetFirstMarkIndex(&item_index,pic_num,ctrl_id))
    {
        //set list index
        MMIPICVIEW_SetCurIndex(ctrl_id,(uint16)item_index);
    }

//    if ((MMIPICVIEW_IsPreview(FALSE,item_index)))
    {
        MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_PLAY_WIN_TAB,PNULL);
    }
#else
    MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_PLAY_WIN_TAB,PNULL);
#endif
}

/*****************************************************************************/
//  Description : handle picture slide play win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicSlidePlayWinMsg(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            )
{
    LOCAL BOOLEAN       s_image_is_display = FALSE;
    uint16              mark_num = 0;
    uint32              item_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
    uint32              pic_num = MMIPICVIEW_GetAllPicNum();
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_SLIDE_PLAY_ANIM_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    GUI_BOTH_RECT_T     both_rect = MMITHEME_GetFullScreenBothRect();
    static BOOLEAN      s_refresh_preview_anim = TRUE;
	
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIAPICTRL_SetBothRect(ctrl_id,&both_rect);

        //set back light
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);

        //if mark,display the first mark picture
        if (GetFirstMarkIndex(&item_index,pic_num,list_ctrl_id))
        {
            //set list index
            MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)item_index);
        }

        //set anim param
        s_image_is_display = FALSE;
        SetSlidePlayAnim(item_index,ctrl_id);
        MMK_SetAtvCtrl(win_id,ctrl_id);
		s_refresh_preview_anim = TRUE;
        break;

    case MSG_CTL_ANIM_DISPLAY_IND:
#ifdef DRM_SUPPORT
		{
			FILEARRAY_DATA_T		file_info = {0};
			GUIANIM_DISPLAY_IND_T	*display_ind_ptr = (GUIANIM_DISPLAY_IND_T *)param;

			if(MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
			{
				if ((PNULL != display_ind_ptr) && 
					(GUIANIM_RESULT_SUCC == display_ind_ptr->result))
				{
					MMIAPIDRM_ConsumeRights(SFS_INVALID_HANDLE,file_info.filename);
				}
			}
			else
			{
                SCI_TRACE_LOW("MMIPICVIEWER HandlePicSlidePlayWinMsg MMIPICVIEW_GetPicFileInfo failed!");
			}
		}
#endif
        s_image_is_display = TRUE;
        if (MMK_IsFocusWin(win_id))
        {
            //play slide play timer
            StartSlidePlayTimer(win_id);
        }
        break;

    case MSG_TIMER:
        if (s_mmipicview_slide_play_timer_id == *(uint8*)param)
        {
            //stop slide play timer
            StopSlidePlayTimer();

            //set special effect
            MMITHEME_SetSpecialEffectDirty(MMITHEME_SPECIAL_PIC_SLIDE);

            //next mark picture
            if (NextMarkedPic(&mark_num,item_index,pic_num,ctrl_id,list_ctrl_id))
            {
                s_image_is_display = FALSE;
            }
            else
            {
                if ((0 == mark_num) && 
                    (NextSlidePic(item_index,pic_num,ctrl_id,list_ctrl_id)))
                {
                    s_image_is_display = FALSE;
                }
            }
        }
        else
        {
            result = MMI_RESULT_FALSE;
        }
        break;

    case MSG_GET_FOCUS:
        //set back light
        MMIDEFAULT_AllowTurnOffBackLight(FALSE);

        if (s_image_is_display)
        {
            //start timer
            StartSlidePlayTimer(win_id);
        }
        break;

    case MSG_LOSE_FOCUS:
        //stop slide play timer
        StopSlidePlayTimer();

        //reset special effect
        MMITHEME_SetSpecialEffectDirty(MMITHEME_SPECIAL_NONE);

        //set back light
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
#ifdef EFFECT_RIPPLE_SUPPORT         
        MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
#endif        
        break;

    //case MSG_APP_OK:
    //case MSG_APP_WEB:
    case MSG_APP_CANCEL:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_MENU:
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_RED:
        result = MMI_RESULT_FALSE;
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_WEB:
		SCI_TRACE_LOW("[baidudc] MSG_APP_WEB");
          MMK_CloseWin(win_id);
          if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){//bug751052, close pic preview win before open it
              MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
          }
          MMK_CreateWin((uint32 *)MMIPICVIEW_PREVIEW_WIN_TAB, PNULL);
	  break;

/*
    case MSG_APP_RED:
        s_refresh_preview_anim = FALSE;
        break;
*/        
   case MSG_CLOSE_WINDOW:
        //stop slide play timer
        StopSlidePlayTimer();
        
        //reset special effect
        MMITHEME_SetSpecialEffectDirty(MMITHEME_SPECIAL_NONE);

        //set back light
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);

#ifdef EFFECT_RIPPLE_SUPPORT  
        MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
#endif 
	    if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)  && s_refresh_preview_anim)
		{
	        MMIPICVIEW_RefreshAnimDisplay(item_index);
		}
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set anim parameter
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetSlidePlayAnim(
                               uint32           item_index, //from 0
                               MMI_CTRL_ID_T    ctrl_id
                               )
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    FILEARRAY_DATA_T        file_info = {0};
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     anim_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
//         if (!MMITHEME_IsSupportGsensor())
//         {
//             //set anim auto adapt display
//             GUIANIM_SetAutoAdaptDisplay(TRUE,ctrl_id);
//         }
        
        //set anim min period
        GUIANIM_SetPeriod(MMITHEME_ANIM_MIN_PERIOD,ctrl_id);

        //set anim param
        control_info.is_ctrl_id = TRUE;
        control_info.ctrl_id = ctrl_id;

        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.bg.bg_type  = GUI_BG_COLOR;
        display_info.bg.color    = MMI_BLACK_COLOR;//MMI_WINDOW_BACKGROUND_COLOR;

#ifdef DRM_SUPPORT
		if(!MMIPICVIEW_IsPreview(FALSE, item_index))
		{
			GUIANIM_DATA_INFO_T data_info={0};
			data_info.img_id  = IMAGE_DRM_PIC_LOCK_ICON;
			display_info.is_update = TRUE;
			anim_result = GUIANIM_SetParam(&control_info, &data_info, PNULL, &display_info);
		}
		else
#endif
		{
			anim_info.full_path_wstr_ptr = file_info.filename;
			anim_info.full_path_wstr_len = file_info.name_len;
			anim_result = GUIANIM_SetParam(&control_info, PNULL, &anim_info, &display_info);
		}

        if ((GUIANIM_RESULT_SUCC == anim_result) ||
            (GUIANIM_RESULT_WAIT == anim_result))
        {
            result = TRUE;
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : start slide play timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StartSlidePlayTimer(
                               MMI_WIN_ID_T   win_id
                               )
{
    uint32  time_out = 0;

    if (0 == s_mmipicview_slide_play_timer_id)
    {
        //get slide time
        time_out = GetSlideTime();
        s_mmipicview_slide_play_timer_id = MMK_CreateWinTimer(win_id,time_out,FALSE);
    }
    else
    {
        //SCI_TRACE_LOW:"StartSlidePlayTimer:don't creat slide play timer"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1719_112_2_18_2_46_44_79,(uint8*)"");
    }
}

/*****************************************************************************/
//  Description : stop slide play timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void StopSlidePlayTimer(void)
{
    if (0 < s_mmipicview_slide_play_timer_id)
    {
        MMK_StopTimer(s_mmipicview_slide_play_timer_id);
        s_mmipicview_slide_play_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : get the first mark picture index
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFirstMarkIndex(//is mark
                                uint32          *index_ptr,     //in[current index]/out[the first mark index]
                                uint32          toatal_num,     //in
                                MMI_CTRL_ID_T   list_ctrl_id    //in
                                )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     is_find = FALSE;
    uint16      i = 0;
    uint16      mark_num = 0;
    uint16      *mark_index_ptr = PNULL;

    if(PNULL == index_ptr)
    {
        return FALSE;
    }

    if (1 < toatal_num)
    {
        //alloc
        mark_index_ptr = SCI_ALLOC(toatal_num*sizeof(uint16));
        SCI_MEMSET(mark_index_ptr,0,(toatal_num*sizeof(uint16)));

        //get mark picture
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,mark_index_ptr,(uint16)toatal_num);
        if (0 < mark_num)
        {
            for (i=0; i<mark_num; i++)
            {
                //find the mark after current item
                if (mark_index_ptr[i] >= *index_ptr)
                {
                    *index_ptr = mark_index_ptr[i];
                    is_find = TRUE;
                    break;
                }
            }

            if (!is_find)
            {
                //no than from the first mark
                *index_ptr = mark_index_ptr[0];
            }

            result = TRUE;
        }

        //free mark index
        SCI_FREE(mark_index_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : switch the next picture
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NextSlidePic(//is change
                           uint32            cur_index,          //in:from
                           uint32            total_num,          //in
                           MMI_CTRL_ID_T     ctrl_id,            //in
                           MMI_CTRL_ID_T     list_ctrl_id        //in
                           )
{
    BOOLEAN     result = FALSE;
    BOOLEAN     set_result = FALSE;
	uint32      start_index = 0;

    if (0 == total_num)
    {
        result = FALSE;
        //SCI_TRACE_LOW:"MMIPICVIEW NextSlidePic total_num is 0 !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1812_112_2_18_2_46_44_80,(uint8*)"");
        return result;
    }

    if (1 < total_num)
    {
		//set start index
        start_index = cur_index;
		
        //set current index
        if (cur_index < (total_num - 1))
        {
            cur_index = cur_index + 1;
        }
        else
        {
            cur_index = 0;
        }
		
//#ifdef DRM_SUPPORT
//        while (!MMIPICVIEW_IsPreview(FALSE,cur_index))
//        {
//            //set current index
//            if (cur_index < (total_num - 1))
//            {
//                cur_index = cur_index + 1;
//            }
//            else
//            {
//                cur_index = 0;
//            }
//            
//            if (start_index == cur_index)
//            {
//                return (result);
//            }
//        }
//#endif

        //set list index
        MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)cur_index);
#ifdef EFFECT_RIPPLE_SUPPORT  
        if (MMIPICVIEW_EFFECT_RIPPLE == MMIAPIPICVIEW_GetSlideEffect())
        {
            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
        }

        if (MMIPICVIEW_EFFECT_ORDER == MMIAPIPICVIEW_GetSlideEffect())
        {
            MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
        }
#endif        
        //add pic
        set_result = SetSlidePlayAnim(cur_index,ctrl_id);
        if (!set_result)
        {
            GUIANIM_UpdateDisplay(ctrl_id);
        }
        
        result = TRUE;
    }


    return (result);
}

#ifdef BAIDU_DRIVE_SUPPORT
LOCAL MMI_RESULT_E HandleDownloadProcessWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       )
{
	MMI_RESULT_E    result  = MMI_RESULT_TRUE;
	MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID;
	FILEARRAY_DATA_T    file_info = {0};
	MMI_STRING_T                title_str = {0};
	uint16                             cur_index = 0;
	MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
	uint64 downloaded_file_size = 0 , total_file_size = 0 ;
	BOOLEAN b_isDone = FALSE;
	BAIDU_DRV_SIG_T * data =(BAIDU_DRV_SIG_T *)param;
	
    SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg msg_id=%x",msg_id);
     
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
        //GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, MIDDLE_BUTTON, FALSE);
        //GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_CANCEL, RIGHT_BUTTON, FALSE);
        //result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        MMISRV_BAIDUDRV_CreateTask(MMI_MODULE_PIC_VIEWER , BAIDU_DRV_TYPE_DOWNLOAD_PHOTO);
        break;
	case MSG_BAIDU_DRV_INIT_CNF : 
        SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_BAIDU_DRV_INIT_CNF s_mmipicview_current_download_pic_idx=%d",s_mmipicview_current_download_pic_idx);
	if (MMIPICVIEW_GetPicFileInfo(s_mmipicview_current_download_pic_idx,&file_info))
	{
            uint16       w_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1]={0};
            uint16      w_file_name_len=0;
			
            MMIAPIFMM_SplitLastName(file_info.filename, &file_info.name_len,w_file_name, &w_file_name_len);
			
            MMIPICVIEW_GetFixedFolderPhotoPath(
                    MMIAPIFMM_GetDevicePath(MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD)),
                    MMIAPIFMM_GetDevicePathLen(MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD)),
                    PICVIEW_SOURCE_CLOUD_DOWNLOADED,
                    &download_path);
			
            MMIAPICOM_Wstrcat(download_path.path, L"\\");
            MMIAPICOM_Wstrcat(download_path.path, w_file_name);
            download_path.path_len = MMIAPICOM_Wstrlen(download_path.path);
	     MMISRV_BAIDUDRV_Enable_Download();
            if(!MMISRV_BAIDUDRV_Download_Original_photo(download_path.path,download_path.path_len))
		{
                if(MMK_IsOpenWin(wait_win_id)){
					MMK_CloseWin(wait_win_id);
                    //MMIPUB_CloseProgressWin(&wait_win_id);
                }
                if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID)){
                    MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
                }
            }
	}

        break;
    case MSG_BAIDU_DRV_DOWNLOAD_PHOTO_NOTIFY :
    {
	uint8             progress_text[20] = {0};
	wchar           progress_text_wstr[40] = {0};
	MMI_STRING_T process_content_str = {0};
	uint64 progress = 0;

	downloaded_file_size = data->baidu_drv_msg_data.downloaded_file_size;
	total_file_size = data->baidu_drv_msg_data.total_file_size;	
	b_isDone = data->baidu_drv_msg_data.bDone;	

        
        SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_BAIDU_DRV_DOWNLOAD_PHOTO_NOTIFY download size=%d,total=%d",
            downloaded_file_size,total_file_size);

        if(MMK_IsOpenWin(wait_win_id)){
		//WatchCOM_ProgressWin_SetTotalSize(wait_win_id , 100);
            //MMIPUB_SetProgressTotalSize(&wait_win_id , total_file_size);

	     progress = (uint64)(downloaded_file_size * 100 / total_file_size) ; 

		SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_BAIDU_DRV_DOWNLOAD_PHOTO_NOTIFY progress =%d" , progress);
		
		if(progress <= 100)
            {
                //MMIPUB_UpdateProgressBar(&wait_win_id, downloaded_file_size);
                WatchCOM_ProgressWin_SetProgress(wait_win_id , (uint64)progress);
            }
#if 0
            SCI_MEMSET(progress_text,0,sizeof(progress_text) , (uint32)progress);
            if( total_file_size< 1024){
                sprintf((char*)progress_text, "%ldB", total_file_size);
            }else if(( total_file_size < 1024*1024)&& ( total_file_size >= 1024)){
                sprintf((char*)progress_text, "%ld.%01ldk",(total_file_size/1024),(( total_file_size%1024)*1000/1024));
            }else{
                sprintf((char*)progress_text, "%ld.%01ldM",( total_file_size/(1024*1024)),(( total_file_size%(1024*1024))*1000/1024));
            }
            MMIAPICOM_StrToWstr((const char *)progress_text, progress_text_wstr);
            process_content_str.wstr_ptr = progress_text_wstr;
            process_content_str.wstr_len = MMIAPICOM_Wstrlen(progress_text_wstr);
            MMIPUB_SetProgressBarTextByPtr(wait_win_id,&process_content_str,TRUE);
#endif			
        }
    }
    break;
    case MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF:
    {
	MMI_STRING_T txt_tip = {0};
	WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};

	downloaded_file_size = data->baidu_drv_msg_data.downloaded_file_size;
	total_file_size = data->baidu_drv_msg_data.total_file_size;	
	b_isDone = data->baidu_drv_msg_data.bDone;	

        SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF,done=%d",  b_isDone);

        if(MMK_IsOpenWin(wait_win_id)){
            if(downloaded_file_size == total_file_size){
				MMK_CloseWin(wait_win_id);
                //MMIPUB_CloseProgressWin(&wait_win_id);
				
                if(MMIAPIPICVIEW_GetShowFolderPath() == PICVIEW_SOURCE_NONE){
			
                    MMIAPIPICVIEW_AddFileToPicArray(MMIAPIPICVIEW_GetAllPicArray(),download_path.path);
                }

                MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
                if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
                    MMK_SendMsg(MMIPICVIEW_PREVIEW_WIN_ID, MSG_BAIDU_DRV_DOWNLOAD_PHOTO_CNF,  param);
                }else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
					MMI_GetLabelTextByLang(TXT_ERROR, &txt_tip);
					WatchCOM_NoteWin_1Line_Enter(MMIPICVIEW_LIST_WIN_ID, &txt_tip,0,softkey,PNULL);					                               
                }
                SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg cnf finish cur_index=%d,total=%d",cur_index,MMIPICVIEW_GetAllPicNum());
            }
        }
    }
    break;
    case MSG_BAIDU_DRV_ERROR_NOTIFY:
    {
	BAIDU_DRIVE_ERROR_E err_trype = data->baidu_drv_msg_data.error_type;
	MMI_STRING_T txt_tip = {0};
	WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};

	b_isDone = data->baidu_drv_msg_data.bDone;	

        SCI_TRACE_LOW("MMIPICVIEW HandleDownloadProcessWinMsg MSG_XXX_ERROR_NOTIFY,done=%d,err_trype=%d",b_isDone,err_trype);
		
       // if(err_trype==ONE_DRIVE_ERROR_NO_NETWORK ||err_trype==ONE_DRIVE_ERROR_HTTP_ERROR)
	{
            if(MMK_IsOpenWin(wait_win_id)){
                //MMIPUB_CloseProgressWin(&wait_win_id);
				MMK_CloseWin(wait_win_id);
            }
            MMK_CloseWin(MMIPICVIEW_PREVIEW_OPT_WIN_ID);
            if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID)){
				MMI_GetLabelTextByLang(TXT_ERROR, &txt_tip);
				WatchCOM_NoteWin_1Line_Enter(MMIPICVIEW_PREVIEW_WIN_ID, &txt_tip,0,softkey,PNULL);
            }else if(MMK_IsOpenWin(MMIPICVIEW_LIST_WIN_ID)){
				MMI_GetLabelTextByLang(TXT_ERROR, &txt_tip);
				WatchCOM_NoteWin_1Line_Enter(MMIPICVIEW_LIST_WIN_ID, &txt_tip,0,softkey,PNULL);                
            }
        }
    }
    break;
    case MSG_APP_CANCEL:
		{
			MMI_STRING_T txt_tip = {0};

			WATCH_SOFTKEY_TEXT_ID_T softkey = {TXT_NULL,TXT_NULL,TXT_NULL};
			MMISRV_BAIDUDRV_Cancel_Download();
			//MMIPUB_CloseProgressWin(&wait_win_id);
			MMK_CloseWin(wait_win_id);
			MMI_GetLabelTextByLang(STXT_CANCEL, &txt_tip);
			WatchCOM_NoteWin_1Line_Enter(MMIPICVIEW_PREVIEW_OPT_WIN_ID, &txt_tip,0,softkey,PNULL);        
		}
        break;
    case MSG_APP_WEB:
    case MSG_APP_OK:
        result = MMI_RESULT_FALSE;
        break;
    default:
        //result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
		result = MMI_RESULT_FALSE;            
        break;
    }
    
    return (result);
}

LOCAL MMI_RESULT_E HandleDownloadWin(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    uint32              time_out = 0;
    uint32              deleted_num = 0;
    uint32              select_num = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    uint32                             pic_num = 0;
    uint16                             mark_num = 0;
    wchar           progress_text_wstr[] = L" ";
    MMI_STRING_T process_content_str = {0};
    uint32 sim_ok_num = 0;
    uint16 sim_ok = MN_DUAL_SYS_1;

    s_mmipicview_current_download_pic_idx = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    
    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:
            GUIWIN_SeSoftkeytButtonTextId(win_id, TXT_NULL, LEFT_BUTTON, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_OK, MIDDLE_BUTTON, FALSE);
            GUIWIN_SeSoftkeytButtonTextId(win_id, STXT_CANCEL, RIGHT_BUTTON, FALSE);
        break;
        case MSG_APP_WEB:
            sim_ok_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
            if(sim_ok_num == 0){
                MMIAPIPHONE_AlertSimNorOKStatus();
                MMK_CloseWin(win_id);
                break;
            }
            //open wait
            MMIPUB_OpenProgressWinByTextId(TXT_COMMON_DOWNLOADING_WAIT, &wait_win_id, PNULL, STXT_OK, HandleDownloadProcessWinMsg);
            process_content_str.wstr_ptr = progress_text_wstr;
            process_content_str.wstr_len = MMIAPICOM_Wstrlen(progress_text_wstr);
            MMIPUB_SetProgressBarTextByPtr(wait_win_id,&process_content_str,TRUE);
            MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
       MMK_CloseWin(win_id);
        break;
    case MSG_APP_OK:
        result = MMI_RESULT_FALSE;
    break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Creating dialog with progressbar
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateDownloadProgressDialogWin(void)
{
	wchar           progress_text_wstr[] = L" ";
	MMI_STRING_T process_content_str = {0};
	MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID;
	

	WatchCOM_ProgressWin_Enter_Ex(
                                                        MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID,
                                                        WATCH_COMMONWIN_TYPE_PROGRESS_H,
#ifndef SCREEN_SHAPE_CIRCULAR
								image_watch_notewin_2Line_tip_bg,
#else
								res_common_toast_bg_2,
#endif                                                                                            
                                                        PNULL,
                                                        100,
                                                        0,
                                                        HandleDownloadProcessWinMsg
                                                     );

//	WatchCOM_ProgressWin_SetTextById(MMIPICVIEW_WAIT_DOWNLOAD_WIN_ID , TXT_COMMON_DOWNLOADING_WAIT);
	
//	MMIPUB_OpenProgressWinByTextId(TXT_COMMON_DOWNLOADING_WAIT, &wait_win_id, PNULL, STXT_OK, HandleDownloadProcessWinMsg);
//	process_content_str.wstr_ptr = progress_text_wstr;
//	process_content_str.wstr_len = MMIAPICOM_Wstrlen(progress_text_wstr);
//	MMIPUB_SetProgressBarTextByPtr(wait_win_id,&process_content_str,TRUE);
}
#endif


/*****************************************************************************/
//  Description : switch the next marked picture
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN NextMarkedPic(//is change
                            uint16          *mark_num_ptr,      //in/out:may PNULL
                            uint32          cur_index,          //in:from 0
                            uint32          toatal_num,         //in
                            MMI_CTRL_ID_T   ctrl_id,            //in
                            MMI_CTRL_ID_T   list_ctrl_id        //in
                            )
{
    BOOLEAN     result = FALSE;
    uint16      i = 0;
    uint16      mark_num = 0;
    uint16      *mark_index_ptr = PNULL;
	uint32      start_index = 0;

    if (1 < toatal_num)
    {
        //alloc
        mark_index_ptr = SCI_ALLOC(toatal_num*sizeof(uint16));
        SCI_MEMSET(mark_index_ptr,0,(toatal_num*sizeof(uint16)));

        //set start index
        start_index = cur_index;
		
        //get mark picture
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,mark_index_ptr,(uint16)toatal_num);
        if (1 < mark_num)
        {
            for (i=0; i<mark_num; i++)
            {
                if (cur_index == mark_index_ptr[i])
                {
                    //set the next pic index
                    if (i < (mark_num-1))
                    {
                        cur_index = mark_index_ptr[i+1];
                    }
                    else
                    {
                        cur_index = mark_index_ptr[0];
                    }

//#ifdef DRM_SUPPORT
//                    if ((MMIPICVIEW_IsPreview(FALSE,cur_index)) &&
//                        (cur_index != start_index))
//#endif
					{
						//set list index
						MMIPICVIEW_SetCurIndex(list_ctrl_id,(uint16)cur_index);
#ifdef EFFECT_RIPPLE_SUPPORT  
                        if (MMIPICVIEW_EFFECT_RIPPLE == MMIAPIPICVIEW_GetSlideEffect())
                        {
                            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
                        }

                        if (MMIPICVIEW_EFFECT_ORDER == MMIAPIPICVIEW_GetSlideEffect())
                        {
                            MMIDEFAULT_FreeSavedMove3DBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
                            MMIDEFAULT_SaveOldMoveBuf(SCR_EFFECT_BUF_TYPE_SLIDE_RIPPLE);
                        }
#endif        

						//set the next pic
						if (!SetSlidePlayAnim(cur_index,ctrl_id))
						{
							GUIANIM_UpdateDisplay(ctrl_id);
						}
						break;
					}
                }
            }

            result = TRUE;
        }
        
        //free mark index
        SCI_FREE(mark_index_ptr);
    }

    if (PNULL != mark_num_ptr)
    {
        *mark_num_ptr = mark_num;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle preview option win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePreviewOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    uint32              cur_index = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T       sel_anim_ctrl_id = MMIPICVIEW_SEL_PICTURE_CTRL1_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T       menu_id = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;

    //SCI_TRACE_LOW:"MMIPICVIEW HandlePreviewOptWinMsg msg_id = 0x%x "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_1987_112_2_18_2_46_45_81,(uint8*)"d", msg_id);
    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetGrayPreviewOptMenu(list_ctrl_id);
#ifdef MMI_GUI_STYLE_MINISCREEN
        GUIMENU_SetState(ctrl_id,GUIMENU_STATE_CUSTOMER_TITLE,TRUE);
#endif
        MMK_SetAtvCtrl(win_id,ctrl_id);
        CTRLANIM_SetVisible(sel_anim_ctrl_id, FALSE, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_PICVIEW_MMS:
            //close preview win
            MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);

            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByMms(cur_index);
            break;

        case ID_PICVIEW_BT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByBt(cur_index);
            break;
#ifdef SNS_SUPPORT
        case ID_PICVIEW_SHARE_TO_SINA:
        case ID_PICVIEW_SHARE_TO_FACEBOOK:
        case ID_PICVIEW_SHARE_TO_TIWTTER:
            {
                int share_id = 0;
				if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
				{
		             MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
				}
				
                if(ID_PICVIEW_SHARE_TO_SINA == menu_id)
                {
                    share_id = MMI_SNS_SINA;
                }
                else if(ID_PICVIEW_SHARE_TO_FACEBOOK == menu_id)
                {
                    share_id = MMI_SNS_FACEBOOK;
                }
                else
                {
                    share_id = MMI_SNS_TIWTTER;
                }
                
                cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
                
                SendPicToSNS(cur_index, share_id);
            }
            break;
#endif            
        case ID_PICVIEW_SLIDE_PREVIEW:
            //close preview win
//             MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);

            MMIPICVIEW_SliePlay(list_ctrl_id);
            break;
#ifndef MMI_PICVIEW_MINI_SUPPORT
        case ID_PICVIEW_SLIDE_SET:
            MMK_CreateWin((uint32 *)MMIPICVIEW_SLIDE_SET_WIN_TAB,PNULL);
            break;
#endif
        case ID_PICVIEW_WALLPAPER:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            //MMK_CloseWin(win_id);

            SetWallpaper(cur_index, sel_anim_ctrl_id);
            break;
        case ID_PICVIEW_POWER_ON_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, TRUE, sel_anim_ctrl_id);
            break;
            
        case ID_PICVIEW_POWER_OFF_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, FALSE, sel_anim_ctrl_id);
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_PICVIEW_PB:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            
            SetPhontbook(cur_index, sel_anim_ctrl_id);
            break;
#endif
#ifdef PICVIEW_VIEW_ZOOM

        case ID_PICVIEW_ZOOM:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
	        ZoomPic(cur_index);
            break;
#endif
#ifdef PIC_EDITOR_SUPPORT
        case ID_PICVIEW_EDIT:
            //close preview win
            MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);

            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            EditPic(cur_index);
            break;
#endif
        case ID_PICVIEW_DELETE:
#ifdef DRM_SUPPORT
            if (MMIPICVIEW_IsDeleteDrm(list_ctrl_id))
            {
                if (1 < MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0))
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
                }
                else
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
                }
            }
            else
#endif
			{
				//prompt query window
				MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
			}
            break;

        case ID_PICVIEW_RENAME:
            MMK_CreateWin((uint32 *)MMIPICVIEW_RENAME_WIN_TAB,PNULL);
            break;

        case ID_PICVIEW_DETAIL:
            MMK_CreateWin((uint32 *)MMIPICVIEW_DETAIL_WIN_TAB,PNULL);
            break;
            
// #ifdef PIC_PREVIEW_U200_SUPPORT
//         case ID_PICVIEW_SETTING:
//             //open setting win
//             MMK_CreateWin((uint32 *)MMIPICVIEW_PDA_SETTING_WIN_TAB,PNULL);
//             break;
// #endif
            
        default:
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePreviewOptWinMsg:menu_id %d is error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_2125_112_2_18_2_46_45_82,(uint8*)"d",menu_id);
            break;
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef SNS_SUPPORT
    case MSG_SNS_OPERA_RESULT:
        {
            if(pMe)
            {
                IMMISNS_Release(pMe);
                pMe = PNULL;
            }
        }
        break;
#endif     
    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set preview opt menu gray
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayPreviewOptMenu(
                                 MMI_CTRL_ID_T      list_ctrl_id
                                 )
{
    uint16                  cur_index = 0;
    GUIANIM_TYPE_E          img_type = GUIANIM_TYPE_NONE;
    FILEARRAY_DATA_T        file_info = {0};
    MMI_MENU_GROUP_ID_T     group_id = MENU_PICVIEW_PREVIEW_OPT;
    
    //get item index
    cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    
    //get picture info,only jpg support picture editor
    if (MMIPICVIEW_GetPicFileInfo(cur_index,&file_info))
    {
        //get image type
        img_type = MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len);

#ifdef PICVIEW_VIEW_ZOOM        
        if (GUIANIM_TYPE_IMAGE_GIF == img_type)
        {
            GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_ZOOM,TRUE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_ZOOM,FALSE);
        }
#endif
        if (GUIANIM_TYPE_IMAGE_JPG == img_type)
        {
            GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_EDIT,FALSE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_EDIT,TRUE);
        }
    }
    else
    {
#ifdef PICVIEW_VIEW_ZOOM    
        GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_ZOOM,TRUE);
#endif        
        GUIMENU_SetItemGrayed(MMIPICVIEW_PREVIEW_OPT_MENU_CTRL_ID,group_id,ID_PICVIEW_EDIT,TRUE);
    }
}

/*****************************************************************************/
//  Description : send picture by mms
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SendPicByMms(
                        uint32  item_index  //from 0
                        )
{
    FILEARRAY_DATA_T    pic_file_info = {0};
    MMIFILE_FILE_INFO_T file_info = {0};

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        //get picture info
        if( !MMIPICVIEW_GetPicFileInfo(item_index,&pic_file_info) )
        {
            return;
        }
#ifdef DRM_SUPPORT
        //is preview
        if (!MMIPICVIEW_IsSend(TRUE,&pic_file_info))
        {
            return;
        }
#endif		
        MMIAPIFMM_GetFileInfoFormFullPath(pic_file_info.filename,pic_file_info.name_len,&file_info);
        MMIAPIMMS_EditMMSFromFile(&file_info,MMIFMM_FILE_TYPE_PICTURE);
    }
}

/*****************************************************************************/
//  Description : send picture by blue tooth
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SendPicByBt(
                       uint32  item_index  //from 0
                       )
{
#ifdef BLUETOOTH_SUPPORT    
    FILEARRAY_DATA_T    pic_info = {0};
    MMIFMM_BT_SENDFILE_INFO_T send_file_info = {0};
	
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
    }
    else
    {
        //get picture info
       if (MMIPICVIEW_GetPicFileInfo(item_index,&pic_info))
       {
#ifdef DRM_SUPPORT
            //is preview
            if (!MMIPICVIEW_IsSend(FALSE,&pic_info))
            {
                return;
            }
#endif
            MMIAPICOM_Wstrncpy(send_file_info.filepath_name, pic_info.filename,pic_info.name_len);
            send_file_info.filepath_len = pic_info.name_len;
            send_file_info.file_size = pic_info.size;
            send_file_info.is_temp_file = FALSE;
            //send by bt
            MMIAPIBT_SendMultiFile(&send_file_info,1);	     		
        }
    }
#endif    
}
#ifdef SNS_SUPPORT

/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandlePicViewWhetherInstallWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    int            err             = 0;
    int            widget_id       = 0;
    MMI_RESULT_E   recode          = MMI_RESULT_TRUE;
    MMIPUB_INFO_T *pubwin_info_ptr = PNULL;
    
    pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);

    if(PNULL == pubwin_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    widget_id = pubwin_info_ptr->user_data;
    
    switch(msg_id)
    {
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            IMMISNS_CreateObject(&pMe);

            if(pMe)
            {
                IMMISNS_AppInstall(pMe, widget_id);
                
                IMMISNS_Release(pMe);
                pMe         = PNULL;
            }
            break;
        }
        case MSG_APP_CANCEL:
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }
    
    return recode;
}


/*****************************************************************************/
//  Description : send pic to sns
//  Global resource dependence : 
//  Author: weiren
//  Note: 
/*****************************************************************************/
LOCAL void SendPicToSNS(
                       uint32  item_index ,
                       uint32  sns_index  //from 0
                       )
{
    FILEARRAY_DATA_T  pic_info = {0};
    MMI_STRING_T     pic_file_info = {0};
    
    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_UDISK_USING,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    else
    {
        //get picture info
        if (MMIPICVIEW_GetPicFileInfo(item_index,&pic_info))
        {
#ifdef DRM_SUPPORT
            //is preview
            if (!MMIPICVIEW_IsSend(FALSE,&pic_info))
            {
                return;
            }
#endif
            //send to sns

            IMMISNS_CreateObject(&pMe);

            if(pMe)
            {
                uint32        time_num    = 500;
                
                pic_file_info.wstr_ptr = (wchar*)SCI_ALLOC_APP(sizeof(wchar) *(pic_info.name_len +1));

                if(PNULL == pic_file_info.wstr_ptr)
                {
                    return ;
                }

                SCI_MEMSET(pic_file_info.wstr_ptr, 0, sizeof(wchar) *(pic_info.name_len+1));

                MMIAPICOM_Wstrncpy((wchar*)pic_file_info.wstr_ptr, pic_info.filename, pic_info.name_len); 	
                pic_file_info.wstr_len = pic_info.name_len;

                IMMISNS_ShareToSns(pMe, sns_index, SNS_MODULE_IMAGE_BROWSER, (uint32)&pic_file_info);

                SCI_FREE(pic_file_info.wstr_ptr);
                pic_file_info.wstr_ptr = PNULL;
                MMIPUB_OpenAlertWinByTextId(&time_num, TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

            }
        }
    }
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle slide set win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicSlideSetWinMsg(
                                           MMI_WIN_ID_T        win_id,
                                           MMI_MESSAGE_ID_E    msg_id,
                                           DPARAM              param
                                           )
{
    MMI_RESULT_E            result = MMI_RESULT_TRUE;

    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        //set slide set
        SetSlideSettings();
        MMK_SetAtvCtrl(win_id,MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID);
        break;

    case MSG_CTL_OK:
        SaveSlideSet();
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set slide set param
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetSlideSettings(void)
{
    uint16                  cur_index = 0;
    uint32                  i = 0;
    MMI_TEXT_ID_T           effect_text[MMIPICVIEW_EFFECT_TYPE_MAX] = {TXT_PICVIEW_EFFECT_ORDER,
                                             TXT_PICVIEW_EFFECT_ENLARGE,
                                             TXT_PICVIEW_EFFECT_GRID,
                                             TXT_PICVIEW_EFFECT_LEFTWARD,
                                             TXT_PICVIEW_EFFECT_RIGHTWARD,
                                             TXT_PICVIEW_EFFECT_UPWARD,
                                             TXT_PICVIEW_EFFECT_DOWNWARD,
#ifdef EFFECT_RIPPLE_SUPPORT                                               
                                             TXT_PICVIEW_EFFECT_RIPPLE,                                             
#endif                                             
                                             TXT_NONE};
    MMI_TEXT_ID_T           time_text[MMIPICVIEW_SLIDE_TIME_NUM] = {TXT_COMM_2_SECONDS,
                                           TXT_COMM_3_SECONDS,
                                           TXT_COMM_6_SECONDS,
                                           TXT_COMM_10_SECONDS,
                                           TXT_PICVIEW_SLIDE_TIME_16S,
                                           TXT_COMM_20_SECONDS};
    MMI_CTRL_ID_T           effect_form_id = MMIPICVIEW_EFFECT_FORM_CTRL_ID;
    MMI_CTRL_ID_T           effect_ctrl_id = MMIPICVIEW_EFFECT_LABEL_CTRL_ID;
    MMI_CTRL_ID_T           effect_dropdown_id = MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID;
    MMI_CTRL_ID_T           time_form_id = MMIPICVIEW_TIME_FORM_CTRL_ID;
    MMI_CTRL_ID_T           time_ctrl_id = MMIPICVIEW_TIME_LABEL_CTRL_ID;
    MMI_CTRL_ID_T           time_dropdown_id = MMIPICVIEW_TIME_DROPDOWN_CTRL_ID;
    MMI_STRING_T            effect_item[MMIPICVIEW_EFFECT_TYPE_MAX] ={0};
    MMI_STRING_T            time_item[MMIPICVIEW_SLIDE_TIME_NUM] ={0};
    
#ifdef PIC_PREVIEW_U200_SUPPORT
    GUIFORM_SetType(MMIPICVIEW_SET_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif

    //set effect and time form style
    GUIFORM_SetStyle(effect_form_id,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(time_form_id,GUIFORM_STYLE_UNIT);

    //set effect name
    GUILABEL_SetTextById(effect_ctrl_id,TXT_EFFECT,FALSE);

    //set time name
    GUILABEL_SetTextById(time_ctrl_id,TXT_PICVIEW_SLIDE_TIME,FALSE);

    //get effect item
    for (i=0; i<MMIPICVIEW_EFFECT_TYPE_MAX; i++)
    {
        MMI_GetLabelTextByLang(effect_text[i],&effect_item[i]);
    }

    //set effect drop down list
    GUIDROPDOWNLIST_AppendItemArray(effect_dropdown_id,effect_item,MMIPICVIEW_EFFECT_TYPE_MAX);
    
    //set display ellipsis
    GUIDROPDOWNLIST_SetEllipsis(effect_dropdown_id,TRUE);
    
    //get current slide effect index
    cur_index = GetSlideEffectIndex();

    //set select effect item
    GUIDROPDOWNLIST_SetCurItemIndex(effect_dropdown_id,cur_index);

    //get time item
    for (i=0; i<MMIPICVIEW_SLIDE_TIME_NUM; i++)
    {
        MMI_GetLabelTextByLang(time_text[i],&time_item[i]);
    }

    //set time drop down list
    GUIDROPDOWNLIST_AppendItemArray(time_dropdown_id,time_item,MMIPICVIEW_SLIDE_TIME_NUM);

    //get current slide effect index
    cur_index = GetSlideTimeIndex();

    //set select time item
    GUIDROPDOWNLIST_SetCurItemIndex(time_dropdown_id,cur_index);

    //set display ellipsis
    GUIDROPDOWNLIST_SetEllipsis(time_dropdown_id,TRUE);
}

/*****************************************************************************/
//  Description : get slide effect index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideEffectIndex(void)
{
    uint16                      cur_index = 0;
    MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIPICVIEW_EFFECT_NONE;

    //get slide effect
    slide_effect = MMIAPIPICVIEW_GetSlideEffect();

    cur_index = (uint16)slide_effect;

    return (cur_index);
}

/*****************************************************************************/
//  Description : set slide effect index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetSlideEffectIndex(
                               uint16   cur_index
                               )
{
    MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIPICVIEW_EFFECT_NONE;

    if (MMIPICVIEW_EFFECT_TYPE_MAX <= cur_index)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW SetSlideEffectIndex cur_index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_2536_112_2_18_2_46_46_83,(uint8*)"");
        return ;
    }

    slide_effect = (MMIPICVIEW_EFFECT_TYPE_E)cur_index;

    //set slide effect
    MMIPICVIEW_SetSlideEffect(slide_effect);
}
#endif
/*****************************************************************************/
//  Description : get slide time
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint32 GetSlideTime(void) //return ms
{
    uint32                  time_out = 0;
    MMIPICVIEW_TIME_TYPE_E  slide_time = MMIPICVIEW_GetSlideTime();

    time_out = s_mmipicview_slide_time[slide_time];

    return (time_out);
}
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get slide time index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL uint16 GetSlideTimeIndex(void)
{
    uint16                  cur_index = 0;
    MMIPICVIEW_TIME_TYPE_E  slide_time = MMIPICVIEW_TIME_2S;

    //get slide time
    slide_time = MMIPICVIEW_GetSlideTime();

    cur_index = (uint16)slide_time;

    return (cur_index);
}

/*****************************************************************************/
//  Description : set slide time index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetSlideTimeIndex(
                             uint16   cur_index
                             )
{
    MMIPICVIEW_TIME_TYPE_E  slide_time = MMIPICVIEW_TIME_2S;

    if (MMIPICVIEW_TIME_TYPE_MAX <= cur_index)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW SetSlideTimeIndex cur_index is overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_2589_112_2_18_2_46_46_84,(uint8*)"");
        return ;
    }

    slide_time = (MMIPICVIEW_TIME_TYPE_E)cur_index;

    //write slide time
    MMIPICVIEW_SetSlideTime(slide_time);
}

/*****************************************************************************/
//  Description : save slide set param
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SaveSlideSet(void)
{
    uint16          cur_index = 0;
    MMI_CTRL_ID_T   effect_dropdown_id = MMIPICVIEW_EFFECT_DROPDOWN_CTRL_ID;
    MMI_CTRL_ID_T   time_dropdown_id = MMIPICVIEW_TIME_DROPDOWN_CTRL_ID;

    //save slide effect
    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(effect_dropdown_id);
    SetSlideEffectIndex(cur_index);

    //save slide time
    cur_index = GUIDROPDOWNLIST_GetCurItemIndex(time_dropdown_id);
    SetSlideTimeIndex(cur_index);
}
#endif
/*****************************************************************************/
//  Description : set wallpaper
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetWallpaper(
                        uint32  item_index, //from 0
                        MMI_CTRL_ID_T  ctrl_id
                        )
{
    FILEARRAY_DATA_T    file_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {       
        //set anim param
        if(!MMIPICVIEW_SetSelPicParam (file_info.filename, file_info.name_len, ctrl_id))
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            return;
        }
        if(file_info.size == 0)//NEWMS00181048 
        {
            MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            return;
        }
#ifdef DRM_SUPPORT
        //is set wallpaper
        if (!MMIPICVIEW_IsSetWallpaper(item_index))
        {
			MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_WALLPAPER);
            return;
        }
#endif
        //if (!MMIAPIFMM_SetPictureToWallpaper(FALSE,file_info.filename,file_info.name_len))
#if defined MMI_WALLPAPER_SUPPORT
        if (!MMIAPIFMM_SetPictureToWallpaperEx(FALSE,file_info.filename,file_info.name_len))
        {
            //prompt fail
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
#endif
//#ifdef MMI_PICVIEW_MINI_SUPPORT
        /* add by langhua.yang for NEWMS00205097*/
        else
        {
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        }
//#endif
    }
}

/*****************************************************************************/
//  Description : set power on/off anim
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void SetPowerAnim(
                        uint32  item_index,  //from 0
                        BOOLEAN is_power_on,
                        MMI_CTRL_ID_T  ctrl_id
                        )
{
    FILEARRAY_DATA_T    file_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {   
        //set anim param
        if(!MMIPICVIEW_SetSelPicParam (file_info.filename, file_info.name_len, ctrl_id))
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            return;
        }
        if(file_info.size == 0)//NEWMS00181048 
        {
            MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            return;
        }
        MMIAPIFMM_SetAsPowerOnOffAnim (is_power_on, file_info.filename, file_info.name_len);
    }
}
/*****************************************************************************/
//  Description : set picture anim control param
//  Global resource dependence :
//  Author: langhua.yang
//  Note:
/*****************************************************************************/
BOOLEAN MMIPICVIEW_SetSelPicParam (
    const wchar          *full_path_wstr_ptr,//in
    uint16         full_path_wstr_len, //in:字节数
    MMI_CTRL_ID_T  ctrl_id
)
{
    BOOLEAN                 result = FALSE;
    GUIANIM_RESULT_E        anim_result = GUIANIM_RESULT_SUCC;
    GUIANIM_CTRL_INFO_T     control_info = {0};
    GUIANIM_FILE_INFO_T     file_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    if (PNULL == full_path_wstr_ptr)
    {
        return FALSE;
    }

    if (0 == full_path_wstr_len)
    {
        return FALSE;
    }
    control_info.is_ctrl_id = TRUE;
    control_info.ctrl_id = ctrl_id;
    control_info.ctrl_ptr = PNULL;

    file_info.full_path_wstr_ptr = full_path_wstr_ptr;
    file_info.full_path_wstr_len = full_path_wstr_len;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_play_once = FALSE;
    display_info.bg.bg_type = GUI_BG_COLOR;
    display_info.bg.color   = MMI_WINDOW_BACKGROUND_COLOR;

    anim_result = GUIANIM_SetParam (&control_info, PNULL, &file_info, &display_info);

    if ( (GUIANIM_RESULT_SUCC == anim_result) ||
            (GUIANIM_RESULT_WAIT == anim_result))
    {
        result = TRUE;
    }
    return (result);
}

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
/*****************************************************************************/
//  Description : set PB contact icon
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void SetPhontbook(
                        uint32  item_index,  //from 0
                        MMI_CTRL_ID_T  ctrl_id
                        )
{
    FILEARRAY_DATA_T        file_info = {0};
    MMIPB_CUSTOM_PHOTO_T    data_info = {0};

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {   
        //set anim param
        if(!MMIPICVIEW_SetSelPicParam (file_info.filename, file_info.name_len, ctrl_id))
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            return;
        }
        if(file_info.size == 0)//NEWMS00181048 
        {
            MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            return;
        }
#ifdef DRM_SUPPORT
        //is set PB,限制相同与Wallpaper
        if (!MMIPICVIEW_IsSetWallpaper(item_index))
        {
			MMIPUB_OpenAlertWarningWin(TXT_DRM_NOT_SET_PB);
            return;
        }
#endif
//#ifdef MMI_PIC_CROP_SUPPORT
        if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
		{
             MMK_CloseWin(MMIPICVIEW_PREVIEW_WIN_ID);
		}
//#endif
        SCI_MEMSET(&data_info, 0, sizeof(MMIPB_CUSTOM_PHOTO_T));
        MMI_WSTRNCPY(data_info.file_name, MMIFILE_FULL_PATH_MAX_LEN, file_info.filename, MMIFILE_FULL_PATH_MAX_LEN, file_info.name_len);
        data_info.file_name_len = file_info.name_len;
        data_info.file_size     = file_info.size;
        data_info.is_valid      = TRUE;
        data_info.photo_type    = file_info.type;

        MMIAPIPB_AddToExistContact(&data_info,  MMIPB_ADD_TYPE_PIC, PNULL);
    }
}
#endif
#ifdef PICVIEW_VIEW_ZOOM
/*****************************************************************************/
//  Description : zoom picture
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void ZoomPic(
                   uint32   item_index  //from 0
                   )
{
    FILEARRAY_DATA_T    file_info = {0};
#if defined(BAIDU_DRIVE_SUPPORT)
	wchar  file_path[MMIFMM_PATHNAME_LEN + 1] = {0};
	MMIFILE_DEVICE_E file_dev = MMI_DEVICE_SDCARD;
	wchar* default_path = PNULL;
	wchar  wfilename[64] = {0};
	uint16  wfilenamelen = 0;
#endif

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
#if defined(BAIDU_DRIVE_SUPPORT)
	MMIAPIFMM_SplitLastName(file_info.filename, &file_info.name_len, wfilename, &wfilenamelen);
	file_dev = MMI_DEVICE_SDCARD;
	file_dev = MMIAPIFMM_GetValidateDevice(file_dev);
	if (file_dev < MMI_DEVICE_NUM)
	{
		default_path = MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD ;
	}
	MMIAPIFMM_CombineFullPathEx(file_path, MMIFILE_FULL_PATH_MAX_LEN, file_dev, default_path, wfilename);
        MMIPICVIEW_Zoom(file_path, MMIAPICOM_Wstrlen(file_path));
#else
        MMIPICVIEW_Zoom(file_info.filename,file_info.name_len);
#endif
    }
}
#endif

#ifdef PIC_EDITOR_SUPPORT
/*****************************************************************************/
//  Description : edit picture
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void EditPic(
                   uint32  item_index  //from 0
                   )
{
    FILEARRAY_DATA_T    file_info = {0};
    s_editpic_index = item_index ;//add for NEWMS00108303
    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
        if(file_info.size == 0)//NEWMS00181048 
        {
            MMIPUB_OpenAlertWarningWin(TXT_EMPTY_FILE);
            return;
        }
        if (GUIANIM_TYPE_IMAGE_JPG == MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len))
        {
            MMIAPIPIC_OpenPicEditorMainWin(file_info.filename,file_info.name_len);
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
        }
    }
}
#endif
/*****************************************************************************/
//  Description : handle picture list mark option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               )
{
    uint16      mark_num = 0;
    //uint16      cur_index = 0;
    uint32      pic_num = MMIPICVIEW_GetAllPicNum();

    //get mark num
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        MMIPICVIEW_SetListMarkable(list_ctrl_id,TRUE);

        //set mark max number
        MMIPICVIEW_SetMaxSelected(list_ctrl_id,(uint16)pic_num);
    }

    //get item index
    //cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

    //mark item
    //MMIPICVIEW_SetSelectedItem(list_ctrl_id,cur_index,TRUE);
}

/*****************************************************************************/
//  Description : handle picture list mark cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void HandlePicListUnmarked(
                                 MMI_CTRL_ID_T    list_ctrl_id
                                 )
{
    uint16      cur_index = 0;
    uint16      mark_num = 0;

    //get item index
    cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

    //cancel mark item
    MMIPICVIEW_SetSelectedItem(list_ctrl_id,cur_index,FALSE);

    //get mark num
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);
    }
}

/*****************************************************************************/
//  Description : handle picture list mark all option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListMarkedAll(
                                  MMI_CTRL_ID_T   list_ctrl_id
                                  )
{
    uint16      mark_num = 0;
    uint32      pic_num = MMIPICVIEW_GetAllPicNum();

    //get mark num
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //set list enable mark
        MMIPICVIEW_SetListMarkable(list_ctrl_id,TRUE);

        //set mark max number
        MMIPICVIEW_SetMaxSelected(list_ctrl_id,(uint16)pic_num);
    }

    //mark all item
    MMIPICVIEW_SetAllSelected(list_ctrl_id,TRUE);
}

/*****************************************************************************/
//  Description : handle picture list mark all cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListUnmarkedAll(
                                    MMI_CTRL_ID_T list_ctrl_id
                                    )
{
    MMIPICVIEW_SetAllSelected(list_ctrl_id,FALSE);

    //set list enable mark
//    MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);
}
#ifdef PIC_EDITOR_SUPPORT    //norflashcut_pic     
//add for cr NEWMS00108303
/*****************************************************************************/
//  Description : get editpic index
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPICVIEW_GetAndResetEditPicIndex(void)
{
    uint16 cur_index = s_editpic_index;
    s_editpic_index = 0;
    return cur_index;
}
//add for cr NEWMS00108303
#endif


/*****************************************************************************/
//  Description : handle picture list delete msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPICVIEW_HandleDelQueryWin(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 )
{
    static uint32       s_total_num = 0;
    uint32              time_out = 0;
    uint32              deleted_num = 0;
    uint32              select_num = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_DEL_WAIT_WIN_ID;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;

    switch (msg_id)
    {
    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(&query_win_id);
        s_mmipicview_del_file_is_cancelled = FALSE;
SCI_TRACE_LOW("[pic_view]MMIAPIPICVIEW_HandleDelQueryWin start_time = %d", SCI_GetTickCount());

/*        if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
        {
            MMIAPIPICVIEW_DeleteOrderInTime(MMIPICVIEW_LIST_CTRL_ID);
        }*/

        //open wait
        s_total_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
        if (1 < s_total_num)
        {
            MMIPUB_OpenProgressWinByTextId(TXT_PUB_DELETING, &wait_win_id, PNULL, MMIPUB_SOFTKEY_CUSTOMER, HandleProcessWinMsg);
            MMIPUB_SetProgressTotalSize(&wait_win_id, s_total_num);
        }
        else
        {
//            MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
        }

        //delete pictures
        s_mmipicview_is_preview_del = FALSE;
        DeleteSelectedPic(list_ctrl_id,wait_win_id,win_id);

        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;
        
    case MSG_PICVIEWER_DELETE_CNF:
        HandleDeletePicCnf(*((BOOLEAN*)param),list_ctrl_id,wait_win_id,win_id);
        if (!s_mmipicview_is_preview_del)
        {
            select_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);

            deleted_num = ((s_total_num > select_num) ? (s_total_num - select_num) : 0);
            MMIPUB_UpdateProgressBar(&wait_win_id, deleted_num);
        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : handle picture preview delete msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePicPreviewDelQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               )
{
    uint32              time_out = 0;
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T        wait_win_id = MMIPICVIEW_DEL_WAIT_WIN_ID;
    MMI_WIN_ID_T        query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_WIN_ID_T        preview_win_id = MMIPICVIEW_PREVIEW_WIN_ID;
    MMI_WIN_ID_T        list_win_id = MMIPICVIEW_LIST_WIN_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;

    switch (msg_id)
    {
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:         
        MMIPUB_CloseQuerytWin(&query_win_id);
        s_mmipicview_del_file_is_cancelled = FALSE;

 /*       if(MMIPICVIEW_VIEW_ICONLIST == MMIPICVIEW_GetViewType())
        {
            MMIAPIPICVIEW_DeleteOrderInTime(MMIPICVIEW_LIST_CTRL_ID);
        } */
      
        //open wait
//        MMIPUB_OpenAlertWinByTextId(&time_out,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&wait_win_id,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);

        //close preview win
        MMK_CloseWin(preview_win_id);

        //delete pictures
        s_mmipicview_is_preview_del = TRUE;
        DeleteSelectedPic(list_ctrl_id,wait_win_id,list_win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:    
        MMIPUB_CloseQuerytWin(&query_win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : to handle the message of progress window    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleProcessWinMsg(
                                       MMI_WIN_ID_T     win_id, 
                                       MMI_MESSAGE_ID_E msg_id, 
                                       DPARAM           param
                                       )
{
    MMI_RESULT_E    result  = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE); 
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;

    case MSG_APP_CANCEL:
        StopDelete();
        break;

    default:
        result = MMIPUB_HandleProgressWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : cancel delete    
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void StopDelete(void)
{
    //SCI_TRACE_LOW:"StopDelete"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_2993_112_2_18_2_46_47_85,(uint8*)"");
    s_mmipicview_del_file_is_cancelled = TRUE;
}

/*****************************************************************************/
//  Description : delete selected pictures
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void DeleteSelectedPic(
                             MMI_CTRL_ID_T      list_ctrl_id,   //in
                             MMI_WIN_ID_T       wait_win_id,    //in
                             MMI_WIN_ID_T       win_id          //in
                             )
{
    uint16              mark_num = 0;
    uint16              item_index = 0;
    FILEARRAY_DATA_T    file_info = {0};
    BOOLEAN             result    = FALSE;
#if defined(BAIDU_DRIVE_SUPPORT)
    wchar       Wfilename[128]={0};
    uint16       wfilenamelen = 0;
    uint16       templen = 0;
    uint8        *filename =NULL;
#endif
SCI_TRACE_LOW("[pic_view]DeleteSelectedPic start_time = %d", SCI_GetTickCount());
    //get mark num
    mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
    if (0 == mark_num)
    {
        //ger current index
        item_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    }
    else
    {
        //delete all mark items
        MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,&item_index,1);
    }

    s_mmipicview_del_file_index = item_index;

    //get file info
    if (MMIPICVIEW_GetPicFileInfo(item_index,&file_info))
    {
        if( MMIAPIFMM_DeleteFileAsyn(file_info.filename,file_info.name_len,win_id,MSG_PICVIEWER_DELETE_CNF) )
        {
            result = TRUE;
#if defined(BAIDU_DRIVE_SUPPORT)
            if(MMISRV_BAIDUDRV_Get_Connect_State()
                                                    &&MMIAPIBAIDU_IsAccountExist())
            {
                filename = SCI_ALLOC_APPZ(64);
                templen = file_info.name_len;
                MMIAPIFMM_SplitLastName(file_info.filename, &templen, Wfilename, &wfilenamelen);
                MMIAPICOM_WstrToStr(Wfilename, filename);
                SCI_TRACE_LOW("[pic_view]filename:%s", filename);
                MMISRV_BAIDUDRV_Delete_Photo_note(filename, strlen(filename));
            }
#endif
        }
    }
SCI_TRACE_LOW("[pic_view]DeleteSelectedPic end_time = %d", SCI_GetTickCount());
    //delete the current item
    if ( !result )
    {
        //prompt fail
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
        
        //close wait win
        MMK_CloseWin(wait_win_id);
    }
}

/*****************************************************************************/
//  Description : handle delete selected pictures confirm message
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL void HandleDeletePicCnf(
                              BOOLEAN           is_success,     //in:delete is success
                              MMI_CTRL_ID_T     list_ctrl_id,   //in:
                              MMI_WIN_ID_T      wait_win_id,    //in:
                              MMI_WIN_ID_T      win_id          //in:
                              )
{
    uint16          pic_index = s_mmipicview_del_file_index;
    uint16          mark_num = 0;
    uint16          cur_index = 0;
#ifndef MMI_PICVIEW_MINI_SUPPORT
    uint16  		pre_folder_num  = 0;
#endif

    if (is_success)
    {
        //update list
        MMIPICVIEW_RemoveItem(list_ctrl_id,pic_index);
#ifndef MMI_PICVIEW_MINI_SUPPORT
        pre_folder_num = MMIAPIFILEARRAY_GetTotalFolderCount(MMIPICVIEW_GetPicLibArray());
#endif
        //update picture array
SCI_TRACE_LOW("[pic_view]MMIPICVIEW_DeletePicFileInfo start_time = %d", SCI_GetTickCount());
        MMIPICVIEW_DeletePicFileInfo(pic_index);
SCI_TRACE_LOW("[pic_view]MMIPICVIEW_DeletePicFileInfo end_time = %d", SCI_GetTickCount());
        if(s_mmipicview_del_file_is_cancelled)
        {
            //stop delete
            HandlePicListUnmarkedAll(list_ctrl_id);

            cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
            MMIPICVIEW_PostUpdateMsg(cur_index);
#ifdef PIC_PREVIEW_U200_SUPPORT
            MMIPICVIEW_DestroyMarkMenuButtons(win_id);
            MMIPICVIEW_SetButtonsVisible(TRUE);
#endif
            MMK_CloseWin(wait_win_id);
        }
        else
        {
            //get mark num
            mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
            if (0 == mark_num)
            {
SCI_TRACE_LOW("[pic_view]MMIAPIPICVIEW_HandleDelQueryWin end_time = %d", SCI_GetTickCount());
                //set list not enable mark
                MMIPICVIEW_SetListMarkable(list_ctrl_id,FALSE);
                if(
#ifndef MMI_PICVIEW_MINI_SUPPORT
                    (pre_folder_num != MMIAPIFILEARRAY_GetTotalFolderCount(MMIPICVIEW_GetPicLibArray())) 
					|| 
#endif
					(MMIPICVIEW_GetifListFromDc() && 0 == MMIPICVIEW_GetAllPicNum())
					)
                {

        	        MMIPICVIEW_PostUpdateMsg(cur_index);
                    MMIAPIPICVIEW_ReloadFile();
                    //close wait win
                    MMK_CloseWin(wait_win_id);

                    MMK_DestroyControl(MMIPICVIEW_LIST_CTRL_ID);
#ifdef PIC_PREVIEW_U200_SUPPORT
                    MMIPICVIEW_DestroyMarkMenuButtons(win_id);
                    MMIPICVIEW_SetButtonsVisible(TRUE);
                    GUIBUTTON_SetGrayed(MMIPICVIEW_LIST_SLIDE_PREVIEW, TRUE, PNULL, TRUE);
                    GUIBUTTON_SetGrayed(MMIPICVIEW_LIST_MARKED, TRUE, PNULL, TRUE);
#else
					GUIWIN_SetSoftkeyBtnGray(win_id, LEFT_BUTTON, TRUE);
			        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#endif
                    MMIPICVIEW_SetListEmpty(TRUE);
                }
                else
                {
                    //prompt success
        		  //xin.li_clean_popwin	
        		  //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DELETED,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandleDeleteSuccAlertWin);
        		  //get item index
        	        cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
        	        
        	        MMIPICVIEW_PostUpdateMsg(cur_index);
                    //MMIAPIPICVIEW_ReloadFile();
                    
                    //close wait win
                    MMK_CloseWin(wait_win_id);
#ifdef PIC_PREVIEW_U200_SUPPORT
                    MMIPICVIEW_DestroyMarkMenuButtons(win_id);
                    MMIPICVIEW_SetButtonsVisible(TRUE);
#endif
                }
                
            }
            else
            {
                //delete the next picture
                DeleteSelectedPic(list_ctrl_id,wait_win_id,win_id);
            }
        }
    }
    else
    {
        //prompt fail
        MMIPUB_OpenAlertFailWin(TXT_ERROR);

        //close wait win
        MMK_CloseWin(wait_win_id);
        cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
        MMIPICVIEW_PostUpdateMsg(cur_index);
#ifdef PIC_PREVIEW_U200_SUPPORT
		MMIPICVIEW_DestroyMarkMenuButtons(win_id);
        MMIPICVIEW_SetButtonsVisible(TRUE);
#endif
    }
}
#if 0
/*****************************************************************************/
//  Description : handle delete picture success alert window
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDeleteSuccAlertWin(
                                            MMI_WIN_ID_T        win_id,
                                            MMI_MESSAGE_ID_E    msg_id,
                                            DPARAM              param
                                            )
{
    uint16          cur_index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);

        //get item index
        cur_index = MMIPICVIEW_GetCurIndex(MMIPICVIEW_LIST_CTRL_ID);
        
        MMIPICVIEW_PostUpdateMsg(cur_index);
        break;
    
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }

    return (result);
}
#endif
/*****************************************************************************/
//  Description : handle rename win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicRenameWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_RENAME_EDITBOX_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;

    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        //init name
        SetInitName(ctrl_id,list_ctrl_id);
        
        //set left softkey
        GUIEDIT_SetSoftkey(ctrl_id,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#ifdef PIC_PREVIEW_U200_SUPPORT
        if(PNULL != param)
        {
            if ((MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //left button
                MMK_PostMsg(win_id, MSG_CTL_OK, PNULL, 0);
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_MIDDLE_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //middle button
            }
            else if ((MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ((MMI_NOTIFY_T*)param)->src_id))
            {
                //right button
                MMK_PostMsg(win_id, MSG_CTL_CANCEL, PNULL, 0);
            }
        }
#endif //PIC_PREVIEW_U200_SUPPORT
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (RenamePic(ctrl_id,list_ctrl_id))
        {
    	    if(MMK_IsOpenWin(MMIPICVIEW_PREVIEW_WIN_ID))
            {
                MMIPICVIEW_RefreshAnimDisplay(MMIPICVIEW_GetCurIndex(list_ctrl_id));
            }

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set initial name
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetInitName(
                       MMI_CTRL_ID_T    ctrl_id,    //in
                       MMI_CTRL_ID_T    list_ctrl_id//in
                       )
{
    uint16              cur_index = 0;
    uint16              path_len = 0;
    uint16              name_len = 0;
    uint16              char_max_num = 0;
    FILEARRAY_DATA_T    file_info = {0};

    //get item index
    cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

    //get picture info
    if (MMIPICVIEW_GetPicFileInfo(cur_index,&file_info))
    {
        //split file name
        MMIAPIFMM_SplitFullPathExt(file_info.filename,file_info.name_len,
            PNULL,&path_len,
            PNULL,&name_len,
            PNULL,PNULL);

        char_max_num = (uint16)(MMIFILE_FILE_NAME_MAX_LEN - file_info.name_len + name_len);

        //set edit box max len
        GUIEDIT_SetTextMaxLen(ctrl_id,char_max_num,char_max_num);

        //set text string
        GUIEDIT_SetString(ctrl_id,file_info.filename + path_len,name_len);
    }
}

/*****************************************************************************/
//  Description : rename picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN RenamePic(
                        MMI_CTRL_ID_T  ctrl_id,    //in
                        MMI_CTRL_ID_T  list_ctrl_id//in
                        )
{
    BOOLEAN             result = FALSE;
    uint16              cur_index = 0;
    uint16              path_len = MMIFILE_FILE_NAME_MAX_LEN;
    uint16              suffix_len = MMIPICVIEW_SUFFIX_MAX_LEN;
    uint16              new_name_len = 0;
    wchar               *new_name_ptr = PNULL;
    wchar               suffix[MMIPICVIEW_SUFFIX_MAX_LEN+1] = {0};
    SFS_ERROR_E         rename_result = SFS_ERROR_NONE;
    MMI_STRING_T        name_str = {0};
    FILEARRAY_DATA_T    file_info = {0};

    //get edit box text info
    GUIEDIT_GetString(ctrl_id,&name_str);

    if (0 < name_str.wstr_len)
    {
        //judge file name is valid
        if(!MMIAPICOM_IsIncludeInvalidChar(name_str.wstr_ptr,name_str.wstr_len))
        {
            //new name alloc memory
            new_name_ptr = SCI_ALLOC_APP((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar));
            SCI_MEMSET(new_name_ptr,0, ((MMIFILE_FILE_NAME_MAX_LEN+1)*sizeof(wchar)));

            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            //get old full name
            if (MMIPICVIEW_GetPicFileInfo(cur_index,&file_info))
            {
                //get new full name, split old full name,get the path and suffix
                MMIAPIFMM_SplitFullPathExt(file_info.filename,file_info.name_len,
                    new_name_ptr,&path_len,
                    PNULL,PNULL,
                    suffix,&suffix_len);
                new_name_len = path_len;

				if (!MMIAPICOM_GetValidFileName(FALSE,
						name_str.wstr_ptr,name_str.wstr_len,
						(new_name_ptr+new_name_len),(MMIFILE_FILE_NAME_MAX_LEN+1-new_name_len)))
				{
					//prompt valid name
					MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEINVALID);
				}
				else
				{
					//get new name string len
					new_name_len = (uint16)MMIAPICOM_Wstrlen(new_name_ptr);

					//copy suffix
					MMI_WSTRNCPY(new_name_ptr+new_name_len,(MMIFILE_FILE_NAME_MAX_LEN-new_name_len),
						suffix,suffix_len,
						suffix_len);
					new_name_len = (uint16)(new_name_len + suffix_len);

					//new name and old name is same
					if (0 == MMIAPICOM_CompareTwoWstr(new_name_ptr,new_name_len,file_info.filename,file_info.name_len))
					{
						//prompt complete
						MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
						result = TRUE;
					}
					else
					{
						//rename file
                        rename_result = SFS_RenameFile(file_info.filename,new_name_ptr,PNULL);
                        switch (rename_result)
                        {
                        case SFS_ERROR_NONE:
							//is wallpaper
#if defined MMI_WALLPAPER_SUPPORT
							if(MMIAPIFMM_IsWallPaperFile(file_info.filename,file_info.name_len))
							{
								MMIAPIFMM_ChangeWallPaperFileName(new_name_ptr,new_name_len);
                                
                                //update wallpaper
                                MMIAPIIDLE_SetWallpaper(TRUE);
							}
#endif
							//delete picture miniature
							GUIANIM_DeleteMiniatureFile(file_info.filename,file_info.name_len);

							//update pic array
							UpdateListArray(cur_index,new_name_ptr,new_name_len);

                            //modify list item
                            MMIPICVIEW_ModifyItemText(list_ctrl_id,cur_index);
            
							//prompt complete
							MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
							result = TRUE;
                            break;

                        case SFS_ERROR_HAS_EXIST:
                            //prompt name repeat
							MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEREPEAT);
                            break;

                        default:
							//prompt error
							MMIPUB_OpenAlertFailWin(TXT_ERROR);
                            break;
                        }
					}

                    //modify name
                    MMIPICVIEW_ModifyName(cur_index);
				}
            }

            //free new name
            SCI_FREE(new_name_ptr);
        }
        else
        {
            //prompt valid name
            MMIPUB_OpenAlertWarningWin(TXT_DC_NAMEINVALID);
        }
    }

    return (result);
}

/*****************************************************************************/
//  Description : update picture list array
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void UpdateListArray(
                           uint16   index,          //in
                           wchar    *new_name_ptr,  //in:full name
                           uint16   new_name_len    //in:full name
                           )
{
    FILEARRAY_DATA_T    file_info = {0};

    //get old file info
    if (MMIPICVIEW_GetPicFileInfo(index,&file_info))
    {
        //set new name
        SCI_MEMSET(file_info.filename,0,sizeof(file_info.filename));
        file_info.name_len = new_name_len;
        MMI_WSTRNCPY(file_info.filename,FILEARRAY_FILENAME_MAX_LEN,
            new_name_ptr,new_name_len,
            new_name_len);

        MMIPICVIEW_ModifyPicFileInfo(index,&file_info);
    }
}

/*****************************************************************************/
//  Description : handle detail win msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicDetailWinMsg(
                                         MMI_WIN_ID_T        win_id,
                                         MMI_MESSAGE_ID_E    msg_id,
                                         DPARAM              param
                                         )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_DETAIL_RICHTEXT_CTRL_ID;
    MMI_CTRL_ID_T       list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;

    switch( msg_id)
    {
    case MSG_OPEN_WINDOW:
        //init detail
        SetDetail(win_id,ctrl_id,list_ctrl_id);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef DRM_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
		{
			uint16				cur_index = 0;
			FILEARRAY_DATA_T    file_info = {0};
			
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

			//cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);

			//get file info
			if ((MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)) &&
				(MMIPICVIEW_IsDrmFile(file_info.filename,file_info.name_len)) &&
				(DRM_LEVEL_SD == MMIAPIDRM_GetDRMFileType(SFS_INVALID_HANDLE,file_info.filename)))
			{
				MMIAPIFMM_DrmOpenDownloadMenuWin(file_info.filename);
			}
		}
        break;
#ifdef PIC_PREVIEW_U200_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
    {
        uint16				cur_index = 0;
        FILEARRAY_DATA_T    file_info = {0};

        cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
       // cur_index = GUILIST_GetCurItemIndex(list_ctrl_id);

        //get file info
        if ((MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)) &&
        	(MMIPICVIEW_IsDrmFile(file_info.filename,file_info.name_len)) &&
        	(DRM_LEVEL_SD == MMIAPIDRM_GetDRMFileType(SFS_INVALID_HANDLE,file_info.filename)))
        {
                MMIAPIDRM_DownloadRightsFile(SFS_INVALID_HANDLE, file_info.filename);
                MMK_CloseWin(win_id);
        }        
    }
    break;
#endif
#endif

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set detail
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void SetDetail(
					 MMI_WIN_ID_T	win_id,
                     MMI_CTRL_ID_T  ctrl_id,
                     MMI_CTRL_ID_T  list_ctrl_id
                     )
{
    uint8               detail_str[MMIPICVIEW_DETAIL_STR_MAX_LEN] = {0};
    uint16              cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
    uint16              richtext_index = 0;
    uint16              path_len = 0;
    uint16              pic_width = 0;
    uint16              pic_height = 0;
    FILEARRAY_DATA_T    file_info = {0};
    GUIRICHTEXT_ITEM_T  richtext_item = {0};
    MMIFILE_DEVICE_E dev = MMI_DEVICE_NUM;
    
    //get file info
    if( !MMIPICVIEW_GetPicFileInfo(cur_index,&file_info) )
    {
        return;
    }

    //add file name label
    AddFixStrItem(&richtext_index,TXT_INPUT_FILE_NAME,&richtext_item,ctrl_id);

    //split file name
    MMIAPIFMM_SplitFullPathExt(file_info.filename,file_info.name_len,
        PNULL,&path_len,
        PNULL,PNULL,
        PNULL,PNULL);

    //add file name
    richtext_item.text_type = GUIRICHTEXT_TEXT_BUF;
    richtext_item.text_data.buf.len = (uint16)MIN(GUIRICHTEXT_TEXT_MAX_LEN,(file_info.name_len-path_len));
    richtext_item.text_data.buf.str_ptr = (file_info.filename+path_len);
    GUIRICHTEXT_AddItem(ctrl_id,&richtext_item,&richtext_index);

    //add date label
    AddFixStrItem(&richtext_index,TXT_COMMON_DATE,&richtext_item,ctrl_id);

    //get date
    SCI_MEMSET(detail_str,0,sizeof(detail_str));
    MMIAPISET_FormatDateStrByDateStyle(file_info.modify_date.year,
        file_info.modify_date.mon,file_info.modify_date.mday,
        '.', detail_str,MMIPICVIEW_DETAIL_STR_MAX_LEN);

    //add date
    AddStrItem(detail_str,(uint16)strlen((char *)detail_str),&richtext_index,&richtext_item,ctrl_id); /*lint !e64*/

    //add time label
    AddFixStrItem(&richtext_index,TXT_COMMON_TIME,&richtext_item,ctrl_id);

    //get time
    SCI_MEMSET(detail_str,0,sizeof(detail_str));
    MMIAPISET_FormatTimeStrByTime(file_info.modify_time.hour,file_info.modify_time.min,detail_str,MMIPICVIEW_DETAIL_STR_MAX_LEN);

    //add time
    AddStrItem(detail_str,(uint16)strlen((char *)detail_str),&richtext_index,&richtext_item,ctrl_id);/*lint !e64*/

    //add size label
    AddFixStrItem(&richtext_index,TXT_DETAIL_SIZE,&richtext_item,ctrl_id);

    //get size
    SCI_MEMSET(detail_str,0,sizeof(detail_str));
    sprintf((char*)detail_str, "%ld.%03ldK",(file_info.size/1024),((file_info.size%1024)*1000/1024));

    //add size
    AddStrItem(detail_str,(uint16)strlen((char *)detail_str),&richtext_index,&richtext_item,ctrl_id);/*lint !e64*/

    //add location label
    AddFixStrItem(&richtext_index,TXT_LOCATION ,&richtext_item,ctrl_id);


    dev = MMIAPIFMM_GetDeviceTypeByPath(file_info.filename, MMIFILE_DEVICE_LEN);
    if(dev >= MMI_DEVICE_NUM)
    {
        //SCI_TRACE_LOW:"[MMIPIC]:SetDetail dev=%d,ERROR!!!!!!!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3585_112_2_18_2_46_48_86,(uint8*)"d",dev);
        dev = MMI_DEVICE_UDISK;
    }
    AddFixStrItem(&richtext_index, MMIAPIFMM_GetDeviceName(dev),&richtext_item,ctrl_id);
    
    //add resolution label
    AddFixStrItem(&richtext_index,TXT_COMM_RESOLUTION,&richtext_item,ctrl_id);
    
    //get picture width and height
    if (0 < file_info.size)
    {
        if (MMIAPIFMM_GetPictureWidthHeight(file_info.filename,file_info.name_len,file_info.size,&pic_width,&pic_height))
        {
            SCI_MEMSET(detail_str,0,sizeof(detail_str));
            sprintf((char*)detail_str, "%dX%d",pic_width,pic_height);

            //add resolution
            AddStrItem(detail_str,(uint16)strlen((char *)detail_str),&richtext_index,&richtext_item,ctrl_id);/*lint !e64*/
        }
        else
        {
            AddFixStrItem(&richtext_index,TXT_COMMON_NO_SUPPORT,&richtext_item,ctrl_id);
        }
    }
    else
    {
        AddFixStrItem(&richtext_index,TXT_EMPTY_FILE,&richtext_item,ctrl_id);
    }
	
#ifdef DRM_SUPPORT
    //add drm file detail
    MMIPICVIEW_AddDrmDetail(file_info.filename,file_info.name_len,win_id,ctrl_id);
#endif
}

/*****************************************************************************/
//  Description : add fix string in rich text
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AddFixStrItem(
                         uint16             *index_ptr, //in/out
                         MMI_TEXT_ID_T      text_id,
                         GUIRICHTEXT_ITEM_T *richtext_item_ptr,
                         MMI_CTRL_ID_T      ctrl_id
                         )
{
    MMI_STRING_T    item_str = {0};
    
    if (PNULL == index_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddFixStrItem index_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3635_112_2_18_2_46_48_87,(uint8*)"");
        return ;
    }
    
    if (PNULL == richtext_item_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddFixStrItem richtext_item_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3641_112_2_18_2_46_48_88,(uint8*)"");
        return ;
    }

    //get resolution label
    MMI_GetLabelTextByLang(text_id,&item_str);

    //add resolution label
    richtext_item_ptr->text_type = GUIRICHTEXT_TEXT_BUF;
    richtext_item_ptr->text_data.buf.len = (uint16)MIN(GUIRICHTEXT_TEXT_MAX_LEN,item_str.wstr_len);
    richtext_item_ptr->text_data.buf.str_ptr = item_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id,richtext_item_ptr,index_ptr);
}

/*****************************************************************************/
//  Description : add string in rich text
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
LOCAL void AddStrItem(
                      const uint8           *str_ptr,           //in
                      uint16                str_len,            //in
                      uint16                *index_ptr,         //in/out
                      GUIRICHTEXT_ITEM_T    *richtext_item_ptr, //in
                      MMI_CTRL_ID_T         ctrl_id
                      )
{
    wchar item_text[GUIRICHTEXT_TEXT_MAX_LEN + 1] = {0};

    if (PNULL == str_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddStrItem str_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3671_112_2_18_2_46_48_89,(uint8*)"");
        return ;
    }
    
    if (PNULL == index_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddStrItem index_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3677_112_2_18_2_46_48_90,(uint8*)"");
        return ;
    }
    
    if (PNULL == richtext_item_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW AddStrItem richtext_item_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3683_112_2_18_2_46_48_91,(uint8*)"");
        return ;
    }

    richtext_item_ptr->text_type = GUIRICHTEXT_TEXT_BUF;
    richtext_item_ptr->text_data.buf.len = (uint16)MIN(GUIRICHTEXT_TEXT_MAX_LEN,str_len);
    MMI_STRNTOWSTR(item_text, GUIRICHTEXT_TEXT_MAX_LEN, str_ptr, str_len, richtext_item_ptr->text_data.buf.len); /*lint !e64*/
    richtext_item_ptr->text_data.buf.str_ptr = item_text;
    GUIRICHTEXT_AddItem(ctrl_id,richtext_item_ptr,index_ptr);
}

/*****************************************************************************/
//  Description : picture viewer is delete file
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsDeleteFiles(void)
{
    BOOLEAN         result = FALSE;
    MMI_WIN_ID_T    wait_win_id = MMIPICVIEW_DEL_WAIT_WIN_ID;

    if(MMK_IsOpenWin(wait_win_id))
    {
        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : stop delete file
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_StopDeleteFiles(void)
{
    StopDelete();
    MMK_CloseWin(MMIPICVIEW_DEL_WAIT_WIN_ID);
}


#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : handle list preview longok menu win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePiclistLongokOptWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    uint32                  cur_index = 0;
    MMI_RESULT_E            result = MMI_RESULT_TRUE;
    MMI_WIN_ID_T            query_win_id = MMIPICVIEW_QUERY_WIN_ID;
    MMI_CTRL_ID_T           ctrl_id = MMIPICVIEW_LIST_LONGOK_OPT_MENU_CTRL_ID;
    MMI_CTRL_ID_T           sel_anim_ctrl_id = MMIPICVIEW_SEL_PICTURE_CTRL2_ID;
    MMI_CTRL_ID_T           list_ctrl_id = MMIPICVIEW_LIST_CTRL_ID;
    MMI_MENU_ID_T           menu_id = 0;
    MMI_MENU_GROUP_ID_T     group_id = 0;
    GUIMENU_TITLE_INFO_T    title_info = {0};
    GUIMENU_BUTTON_INFO_T   button_info = {0};
    MMI_STRING_T            main_title = {0};
    FILEARRAY_DATA_T		file_info = {0};
    wchar                   file_name[MMIFILE_FILE_NAME_MAX_LEN +1] = {0};
    uint16                  file_name_len = 0;
    uint32                  list_item_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        SetGrayListLongokOptMenu(list_ctrl_id);
        
        //set title
        title_info.is_static    = TRUE;
        title_info.group_id     = MENU_PICVIEW_LIST_LONGOK_OPT;
        // get file name
        if(MMIPICVIEW_GetPicFileInfo(list_item_index, &file_info))
        {
	        MMIAPIFMM_SplitFullPath(file_info.filename,
	            file_info.name_len,
	            PNULL,
	            PNULL,
	            PNULL,
	            PNULL,
	            file_name,
	            &file_name_len);
        }
        
        main_title.wstr_ptr = file_name;
        main_title.wstr_len = file_name_len;
        title_info.title_ptr = &main_title;
        GUIMENU_SetMenuTitleEx(ctrl_id, &title_info);
        
        //set context menu button style
        button_info.is_static   = TRUE;
        button_info.group_id    = MENU_PICVIEW_LIST_LONGOK_OPT;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        CTRLANIM_SetVisible(sel_anim_ctrl_id, FALSE, FALSE);
        break;

    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
        switch (menu_id)
        {
        case ID_PICVIEW_MMS:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByMms(cur_index);
            break;

        case ID_PICVIEW_BT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SendPicByBt(cur_index);
            break;
#ifdef SNS_SUPPORT
        case ID_PICVIEW_SHARE_TO_SINA:
        case ID_PICVIEW_SHARE_TO_FACEBOOK:
        case ID_PICVIEW_SHARE_TO_TIWTTER:
            {
                int share_id = 0;
                
                if(ID_PICVIEW_SHARE_TO_SINA == menu_id)
                {
                    share_id = MMI_SNS_SINA;
                }
                else if(ID_PICVIEW_SHARE_TO_FACEBOOK == menu_id)
                {
                    share_id = MMI_SNS_FACEBOOK;
                }
                else
                {
                    share_id = MMI_SNS_TIWTTER;
                }
                
                cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
                
                SendPicToSNS(cur_index, share_id);
            }
            break;
#endif            
        case ID_PICVIEW_SLIDE_PREVIEW:
			MMIPICVIEW_SliePlay(list_ctrl_id);
            break;

        case ID_PICVIEW_WALLPAPER:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            SetWallpaper(cur_index, sel_anim_ctrl_id);
            break;
            
        case ID_PICVIEW_POWER_ON_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, TRUE, sel_anim_ctrl_id);
            break;
            
        case ID_PICVIEW_POWER_OFF_SET:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            SetPowerAnim(cur_index, FALSE, sel_anim_ctrl_id);
            break;

#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_PICVIEW_PB:
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
            
            SetPhontbook(cur_index, sel_anim_ctrl_id);
            break;
#endif            
#ifdef PIC_EDITOR_SUPPORT
        case ID_PICVIEW_EDIT:
            //get item index
            cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);

            EditPic(cur_index);
            break;

#endif

        case ID_PICVIEW_DELETE:
#ifdef DRM_SUPPORT
            if (MMIPICVIEW_IsDeleteDrm(list_ctrl_id))
            {
                if (1 < MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0))
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE_ALL,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
                else
                {
                    //prompt query window
                    MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
                }
            }
            else
#endif
			{
				//prompt query window
				MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,PNULL);
			}
            break;

        case ID_PICVIEW_RENAME:
            MMK_CreateWin((uint32 *)MMIPICVIEW_RENAME_WIN_TAB,PNULL);
            break;

        case ID_PICVIEW_DETAIL:
            MMK_CreateWin((uint32 *)MMIPICVIEW_DETAIL_WIN_TAB,PNULL);
            break;
            
        default:
            //SCI_TRACE_LOW:"MMIPICVIEW HandlePiclistLongokOptWinMsg:menu_id %d is error!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_WINTAB_3882_112_2_18_2_46_49_92,(uint8*)"d", menu_id);
            break;
        }

        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        //GUI_INVALID_PARAM(param);
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : set list longok opt menu gray
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void SetGrayListLongokOptMenu(
                              MMI_CTRL_ID_T     list_ctrl_id
                              )
{
    MMI_CTRL_ID_T           ctrl_id = MMIPICVIEW_LIST_LONGOK_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T     group_id = MENU_PICVIEW_LIST_LONGOK_OPT;
    
    if(0 < MMIPICVIEW_GetAllPicNum())
    {
        uint16                  cur_index = 0;
        uint16                  mark_num = 0;
        FILEARRAY_DATA_T        file_info = {0};
        
        //get item index
        cur_index = MMIPICVIEW_GetCurIndex(list_ctrl_id);
        
        
        //get mark num
        mark_num = MMIPICVIEW_GetSelectItemIndex(list_ctrl_id,PNULL,0);
        if (0 < mark_num)
        {
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_MMS,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_BT,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_WALLPAPER,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_ON_SET,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_OFF_SET,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_PB,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_RENAME,TRUE);
            GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DETAIL,TRUE);
        }
        else
        {
            //get picture info,only jpg support picture editor
            if ((MMIPICVIEW_GetPicFileInfo(cur_index,&file_info)) && 
                (GUIANIM_TYPE_IMAGE_JPG == MMIAPICOM_GetImgaeType(file_info.filename,file_info.name_len)))
            {
                //do nothing
            }
            else
            {
                GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
            }
        }
    }
    else
    {
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_MMS,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_BT,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_SLIDE_PREVIEW,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_WALLPAPER,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_ON_SET,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_POWER_OFF_SET,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_PB,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_EDIT,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DELETE,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_RENAME,TRUE);
        GUIMENU_SetItemGrayed(ctrl_id,group_id,ID_PICVIEW_DETAIL,TRUE);
    }
}

/*****************************************************************************/
//  Description : open pic list longok menu win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_OpenPicListLongokOptWin(void)
{
    MMK_CreateWin((uint32 *)MMIPICVIEW_LIST_LONGOK_OPT_WIN_TAB,PNULL);
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle pda style setting win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicPdaSettingWinMsg(
                                          MMI_WIN_ID_T        win_id,
                                          MMI_MESSAGE_ID_E    msg_id,
                                          DPARAM              param
                                          )
{
    MMI_CTRL_ID_T       ctrl_id = MMIPICVIEW_PDA_SETTINGS_LIST_CTRL_ID;
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    uint16              cur_item = 0;
    uint32              user_data = 0;
    static int32        list_offset_y = 0;
        
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        list_offset_y = 0;
        InitPicPdaSettingsListCtrl();
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_LOSE_FOCUS:
        GUILIST_GetTopItemOffset(ctrl_id, &list_offset_y);
        break;
        
    case MSG_GET_FOCUS:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_RemoveAllItems(ctrl_id);
        InitPicPdaSettingsListCtrl();
        GUILIST_SetCurItemIndex(ctrl_id, cur_item);
        //set list offset
        GUILIST_SetTopItemOffset(ctrl_id, list_offset_y);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_WEB:
        cur_item = GUILIST_GetCurItemIndex(ctrl_id);
        GUILIST_GetItemData(ctrl_id, cur_item, &user_data);
        
        switch((MMIPICVIEW_PDA_SETTING_TYPE_E)user_data)
        {
         case MMIPICVIEW_PDA_SETTING_CHANGE_FOLDER:
            {
                OpenSetDefaultPathWin(); 
            }
            MMK_CloseWin(win_id);
            break;            
            
        case MMIPICVIEW_PDA_SETTING_EFFECT:
            MMK_CreatePubListWin((uint32 *)MMIVP_SETTING_EFFECT_WIN_TAB, PNULL);
            break;

        case MMIPICVIEW_PDA_SETTING_TIME:
            MMK_CreatePubListWin((uint32 *)MMIVP_SETTING_TIME_WIN_TAB, PNULL);
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
    
    return (result);
}
/*****************************************************************************/
//  Description : init pda style settings list ctrl
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void InitPicPdaSettingsListCtrl(void)
{
    MMI_CTRL_ID_T       ctrl_id            = MMIPICVIEW_PDA_SETTINGS_LIST_CTRL_ID;
    
    MMIPICVIEW_DEFAULT_PATH_T    default_path = {0};
    GUILIST_ITEM_T                          item_t = {0};
    GUILIST_ITEM_DATA_T                item_data = {0};


    //set max item
    GUILIST_SetMaxItem(ctrl_id, MMIPICVIEW_PDA_SETTING_MAX, FALSE);
       
    AppendSplitListItem(ctrl_id, TXT_PICVIEW_SLIDE_SET);
    //Effect
    AppendPicPdaSettingsItem(
        ctrl_id,
        TXT_EFFECT,
        GetTextIdByEffectType(MMIAPIPICVIEW_GetSlideEffect()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIPICVIEW_PDA_SETTING_EFFECT);
    
    //Time
    AppendPicPdaSettingsItem(
        ctrl_id,
        TXT_PICVIEW_SLIDE_TIME,
        GetTextIdByTimeType(MMIPICVIEW_GetSlideTime()),
        IMAGE_COMMON_RIGHT_ARROW,
        (uint32)MMIPICVIEW_PDA_SETTING_TIME);

#if defined(CMCC_UI_STYLE)
    if (MMIPICVIEW_DOWNLOAD_PIC != MMIPICVIEW_GetPathType())
    {
#endif
        AppendSplitListItem(ctrl_id, TXT_PICVIEW_CHANGE_FOLDER);
        
        //get default path
        MMIAPIPICVIEW_GetDefaultPath(&default_path);
        
        //chang folder
        item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
        item_t.item_data_ptr = &item_data;
        item_t.user_data     = (uint32)MMIPICVIEW_PDA_SETTING_CHANGE_FOLDER;
        
        //text 1
        item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = TXT_PICVIEW_CHANGE_FOLDER;
        
        //anim
        item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
        item_data.item_content[1].item_data.image_id = IMAGE_PICVIEW_CHANGE_FOLDER;
        
        //text 2
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_BUFFER;
        item_data.item_content[2].item_data.text_buffer.wstr_ptr = default_path.path;   
        item_data.item_content[2].item_data.text_buffer.wstr_len = default_path.path_len;
        
        GUILIST_AppendItem(ctrl_id, &item_t);
#if defined(CMCC_UI_STYLE)
    }
#endif

}
/*****************************************************************************/
//  Description : append settings item
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL void AppendPicPdaSettingsItem(
                                 MMI_CTRL_ID_T  ctrl_id,    //控件ID
                                 MMI_TEXT_ID_T  text_1,     //主文本
                                 MMI_TEXT_ID_T  text_2,     //副文本
                                 MMI_IMAGE_ID_T image_id,   //图标ID
                                 uint32         user_data   //用户数据
                                 )
{
    GUILIST_ITEM_T      item_t = {0};
    GUILIST_ITEM_DATA_T item_data = {0};
    
    item_t.item_style    = GUIITEM_STYLE_TWO_LINE_TEXT_AND_TEXT_ANIM;
    item_t.item_data_ptr = &item_data;
    item_t.user_data     = user_data;
    
    //text 1
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_1;
    //text 2
    if(TXT_NULL != text_2)
    {
        item_data.item_content[2].item_data_type    = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[2].item_data.text_id = text_2;
    }
    
    //anim
    item_data.item_content[1].item_data_type    = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[1].item_data.text_id = image_id;
    
    GUILIST_AppendItem(ctrl_id, &item_t);
}
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : get text_id by view type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByViewType(MMIPICVIEW_VIEW_TYPE_E view_type)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    switch(view_type)
    {
    case MMIPICVIEW_VIEW_LIST:
        text_id = TXT_COMMON_LIST;
        break;
      
    case MMIPICVIEW_VIEW_ICONLIST:
        text_id = TXT_PICVIEW_VIEW_ICONLIST;
        break;
        
    default:
        break;
    }
    
    return text_id;
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get text_id by effect type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByEffectType(MMIPICVIEW_EFFECT_TYPE_E effect_type)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    switch(effect_type)
    {
    case MMIPICVIEW_EFFECT_ORDER:
        text_id = TXT_PICVIEW_EFFECT_ORDER;
        break;
        
    case MMIPICVIEW_EFFECT_ENLARGE:
        text_id = TXT_PICVIEW_EFFECT_ENLARGE;
        break;
        
    case MMIPICVIEW_EFFECT_GRID:
        text_id = TXT_PICVIEW_EFFECT_GRID;
        break;
        
    case MMIPICVIEW_EFFECT_LEFTWARD:
        text_id = TXT_PICVIEW_EFFECT_LEFTWARD;
        break;
        
    case MMIPICVIEW_EFFECT_RIGHTWARD:
        text_id = TXT_PICVIEW_EFFECT_RIGHTWARD;
        break;
        
    case MMIPICVIEW_EFFECT_UPWARD:
        text_id = TXT_PICVIEW_EFFECT_UPWARD;
        break;
        
    case MMIPICVIEW_EFFECT_DOWNWARD:
        text_id = TXT_PICVIEW_EFFECT_DOWNWARD;
        break;
        
#ifdef EFFECT_RIPPLE_SUPPORT
    case MMIPICVIEW_EFFECT_RIPPLE:
        text_id = TXT_PICVIEW_EFFECT_RIPPLE;
        break;
#endif
        
    case MMIPICVIEW_EFFECT_NONE:
        text_id = TXT_NONE;
        break;
        
    default:
        break;
    }
    
    return text_id;
}

/*****************************************************************************/
//  Description : get text_id by slide time type
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_TEXT_ID_T GetTextIdByTimeType(MMIPICVIEW_TIME_TYPE_E time_type)
{
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    switch(time_type)
    {
    case MMIPICVIEW_TIME_2S:
        text_id = TXT_COMM_2_SECONDS;
        break;
        
    case MMIPICVIEW_TIME_3S:
        text_id = TXT_COMM_3_SECONDS;
        break;
        
    case MMIPICVIEW_TIME_6S:
        text_id = TXT_COMM_6_SECONDS;
        break;
        
    case MMIPICVIEW_TIME_10S:
        text_id = TXT_COMM_10_SECONDS;
        break;
        
    case MMIPICVIEW_TIME_16S:
        text_id = TXT_PICVIEW_SLIDE_TIME_16S;
        break;
        
    case MMIPICVIEW_TIME_20S:
        text_id = TXT_COMM_20_SECONDS;
        break;
        
    default:
        break;
    }
    
    return text_id;
}
#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : handle view set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicViewSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    uint16          index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_VIEW_SET_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendViewListItem();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &index, 1);
        
        if((MMIPICVIEW_VIEW_TYPE_E)index != MMIPICVIEW_GetViewType())
        {
            MMK_SendMsg(MMIPICVIEW_LIST_WIN_ID,MSG_PICVIEWER_MODIFY_VIEW,PNULL);
        }
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendViewListItem(void)
{
    uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_VIEW_SET_LIST_CTRL_ID;
    
    GUILIST_SetMaxItem(ctrl_id, MMIPICVIEW_VIEW_MAX, FALSE);
    
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_LIST,     STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_VIEW_ICONLIST, TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //get current set
    switch(MMIPICVIEW_GetViewType())
    {
    case MMIPICVIEW_VIEW_LIST:
        cur_selection = MMIPICVIEW_VIEW_LIST;
        break;
       
    case MMIPICVIEW_VIEW_ICONLIST:
        cur_selection = MMIPICVIEW_VIEW_ICONLIST;
        break;
        
    default:
        break;
    }
    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : handle effect set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicEffectSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    uint16          index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_EFFECT_SET_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendEffectListItem();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &index, 1);
        
        SetSlideEffectIndex(index);
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendEffectListItem(void)
{
    uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_EFFECT_SET_LIST_CTRL_ID;
    
    GUILIST_SetMaxItem(ctrl_id, MMIPICVIEW_EFFECT_TYPE_MAX, FALSE);
    
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_ORDER,      TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_ENLARGE,    TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_GRID,       TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_LEFTWARD,   TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_RIGHTWARD,  TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_UPWARD,     TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_DOWNWARD,   TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#ifdef EFFECT_RIPPLE_SUPPORT    
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_EFFECT_RIPPLE,     TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
#endif
    MMIAPISET_AppendListItemByTextId(TXT_NONE,       TXT_COMMON_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //get current set
    cur_selection = GetSlideEffectIndex();
    
    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}

/*****************************************************************************/
//  Description : handle effect set win msg
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePicTimeSetWin(
                                       MMI_WIN_ID_T        win_id,
                                       MMI_MESSAGE_ID_E    msg_id,
                                       DPARAM              param
                                       )
{
    uint16          index = 0;
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_TIME_SET_LIST_CTRL_ID;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        AppendTimeListItem();
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
        GUILIST_GetSelectedItemIndex(ctrl_id, &index, 1);
        
        SetSlideTimeIndex(index);
        
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

/*****************************************************************************/
//  Description : append view list item
//  Global resource dependence : 
//  Author: xin.li
//  Note:
/*****************************************************************************/
LOCAL void AppendTimeListItem(void)
{
    uint16          cur_selection = 0;
    MMI_CTRL_ID_T   ctrl_id = MMIPICVIEW_TIME_SET_LIST_CTRL_ID;
    
    GUILIST_SetMaxItem(ctrl_id, MMIPICVIEW_TIME_TYPE_MAX, FALSE);
    
    MMIAPISET_AppendListItemByTextId(TXT_COMM_2_SECONDS,      STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_3_SECONDS,      STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_6_SECONDS,      STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_10_SECONDS,     STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_PICVIEW_SLIDE_TIME_16S,     STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_COMM_20_SECONDS,     STXT_OK, ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    //get current set
    cur_selection = GetSlideTimeIndex();
    
    //set select item
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    //set current item
    GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Discription: append split item
//  Global resource dependence: none 
//  Author: xin.li
//  Note:
//
/*****************************************************************************/
LOCAL void AppendSplitListItem(
                               MMI_CTRL_ID_T    ctrl_id,
                               MMI_TEXT_ID_T    text_id
                               )
{
    GUILIST_ITEM_T          item_t    = {0}; /*lint !e64*/
    GUILIST_ITEM_DATA_T     item_data = {0}; /*lint !e64*/
    
    item_t.item_style = GUIITEM_STYLE_ONE_LINE_TEXT_SPLIT;
    
    item_t.item_data_ptr = &item_data;
    item_t.item_state |= GUIITEM_STATE_SPLIT;
    item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
    item_data.item_content[0].item_data.text_id = text_id;
    
    GUILIST_AppendItem( ctrl_id, &item_t );
}

#endif
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Discription: delete cur preview file
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_DeletePreviewFile(MMI_CTRL_ID_T list_ctrl_id)
{
    MMI_WIN_ID_T        query_win_id    = MMIPICVIEW_QUERY_WIN_ID;

#ifdef DRM_SUPPORT
    if (MMIPICVIEW_IsDeleteDrm(list_ctrl_id))
    {
        //prompt query window
        MMIPUB_OpenQueryWinByTextId(TXT_DRM_RIGHTS_VALID_WHETHER_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
    }
    else
#endif
    {
        //prompt query window
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE,IMAGE_PUBWIN_QUERY,&query_win_id,HandlePicPreviewDelQueryWin);
    }
}

/*****************************************************************************/
//  Discription: open set win
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenSetWin(void)
{
    MMK_CreateWin((uint32 *)MMIPICVIEW_PDA_SETTING_WIN_TAB,PNULL);
}
#endif
#endif //#ifdef PIC_VIEWER_SUPPORT


/*Edit by script, ignore 1 case. Thu Apr 26 19:01:03 2012*/ //IGNORE9527


/*Edit by script, ignore 10 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
