/****************************************************************************
** File Name:      mmimms_previewwin.c                                         *
** Author:         minghu.mao                                                 *
** Date:           05/26/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2009        minghu.mao       create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
//#ifdef MMS_SUPPORT
#include "mmk_type.h"
#include "guibutton.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmisrvaud_api.h"
#include "mmisms_export.h"
#include "mmicc_export.h"
#include "mmidc_export.h"
#include "mmifmm_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
//#include "mmimultim_text.h"
#include "mmi_default.h"
#include "mmk_timer.h"
#include "mmisms_text.h"
#include "mmisms_id.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#include "dal_player.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "gui_ucs2b_converter.h"
#include "mmimms_id.h"
#include "mmimms_internal.h"
#include "mmimms_edit.h"
#include "mmimms_text.h"
#include "mmimms_menutable.h"
#include "mmimms_file.h"
#include "mms_image.h"
#include "mmimms_setting.h"
#include "mmimms_control_main.h"
#include "mmimms_net.h"
#include "mmimtv_export.h"
#include "mmk_app.h"
#include "sysMemFile.h"
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
//#include "mmiset_set_ring.h"
//#include "mmiset_text.h"
#endif
#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif
#ifdef MMI_PDA_SUPPORT
#include "guiform.h"
#include "mmisms_image.h"
#include "guictrl_api.h"
#endif
#ifdef SNS_SUPPORT
#include "mmisns_srv.h"
#endif
#ifdef  MMI_VCALENDAR_SUPPORT
#include "mmischedule_export.h"
#endif
#ifdef MMI_MMS_ADJUST_VOLUME_SUPPORT
#include "mmicom_panel.h"
#endif
#include "mmisms_internal.h"
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define MMIMMS_MULTIFILE_VOL_VALUE      MMISET_VOL_FOUR

#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
#define MMIMMS_LIMIT_VEDIO_SIZE   (100*1024)
#endif
#define MMIMMS_PREVIEW_TIME_LEN 3
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/
//play or stop
typedef enum
{
    MMIMMS_PREVIEW_PLAYING,
    MMIMMS_PREVIEW_STOP
//    MMIMMS_PREVIEW_LEAVE
}MMIMMS_PREVIEW_STATE;

//彩信阅览全局变量结构
typedef struct 
{
    BOOLEAN is_mms_noti_detail;                           //is notifycation detail or mt mms detail
    BOOLEAN is_play_video;
    BOOLEAN is_play_music;
    BOOLEAN is_play_video_losefocus;
    BOOLEAN is_parser_text;
    BOOLEAN is_extract_or_not;
    BOOLEAN is_web_play;
    BOOLEAN is_send_rpt;                                  //is send read report
    uint8  play_or_preview;                               //mms play or preview
    uint32 video_play_time;
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    GUI_FONT_T character_font;
#endif
    MMIMMS_EDIT_DOCUMENT_T * editdoc;        
}MMIMMS_PREVIEW_GLOBLE_VARIABLE_T;

#ifdef MMI_PDA_SUPPORT
//dial button info
typedef struct
{
    MMI_CTRL_ID_T   ctrl_id;
    MMI_IMAGE_ID_T          bg_img_id;
    MMI_IMAGE_ID_T          pressed_bg_img_id;
    MMI_IMAGE_ID_T          fg_img_id;
    MMI_IMAGE_ID_T          pressed_fg_img_id;        
} MMIMMS_BUTTON_T;
#endif
#ifdef SNS_SUPPORT
LOCAL IMMISns *pMe  = PNULL;
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
extern MMIMMS_SEND_RECV_GLOBLEE_T g_mms_send_recv_add_data;
MMIMMS_PREVIEW_GLOBLE_VARIABLE_T g_preview_globle = {0};
static MMIMMS_PREVIEW_STATE s_edit_mms_preview_state = MMIMMS_PREVIEW_PLAYING;  //static变量s_edit_mms_preview_state用来记录当前彩信状态(播放,停止)
static BOOLEAN s_bCflc_prtc = FALSE;
#ifdef MMI_PDA_SUPPORT


LOCAL MMIMMS_BUTTON_T s_mtbox_play_form_button_info[] = 
{
    {MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_MMS_PREVIEW_PDA,IMAGE_MMS_PREVIEW_PDA},   
    {MMIMMS_PREVIEW_BUTTON_SMS_REPLEY_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_MMS_REPLY_MMS_PDA,IMAGE_MMS_REPLY_MMS_PDA},
    {MMIMMS_PREVIEW_BUTTON_DELETE_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_DELETE_SMS_PDA,IMAGE_SMS_DELETE_SMS_PDA},
};

LOCAL MMIMMS_BUTTON_T s_sendbox_play_form_button_info[] = 
{
    {MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_MMS_PREVIEW_PDA,IMAGE_MMS_PREVIEW_PDA},   
    {MMIMMS_PREVIEW_BUTTON_FOWARD_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_FORWARD_SMS_PDA,IMAGE_SMS_FORWARD_SMS_PDA},
    {MMIMMS_PREVIEW_BUTTON_DELETE_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_DELETE_SMS_PDA,IMAGE_SMS_DELETE_SMS_PDA}
};

LOCAL MMIMMS_BUTTON_T s_outbox_play_form_button_info[] = 
{
    {MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_MMS_PREVIEW_PDA,IMAGE_MMS_PREVIEW_PDA},   
    {MMIMMS_PREVIEW_BUTTON_SEND_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_MMS_SEND_PDA,IMAGE_MMS_SEND_PDA},
    {MMIMMS_PREVIEW_BUTTON_DELETE_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_SMS_DELETE_SMS_PDA,IMAGE_SMS_DELETE_SMS_PDA}
};

LOCAL MMIMMS_BUTTON_T s_play_form_button_info[] = 
{
    {MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID,IMAGE_SMS_BUTTON_NORMAL,IMAGE_SMS_BUTTON_PRESSED,IMAGE_MMS_PREVIEW_PDA,IMAGE_MMS_PREVIEW_PDA},   
};

#define GUI_FORM_ONE_BUTTON_WIDTH (100)

#endif

 /*****************************************************************************/
// 	Description : 设置softkey的文本ID,并告知控件是否立即刷新
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
extern PUBLIC BOOLEAN CTRLSOFTKEY_SetTextId(
                                    MMI_HANDLE_T    win_handle,
                                    MMI_CTRL_ID_T   ctrl_id,
                                    MMI_TEXT_ID_T   leftsoft_id,   // the left softkey id
                                    MMI_TEXT_ID_T   middlesoft_id, // the middle softkey id
                                    MMI_TEXT_ID_T   rightsoft_id,  // the right softkey id
                                    BOOLEAN         is_need_update       // whether update
                                    );

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :mms read menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMMSOptMenuWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//  Description :mms detail read menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMMSDetailOptMenuWinMsg(
                                                      MMIMMS_WINDOW_ID_E win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param
                                                      );

/*****************************************************************************/
//  Description :mms save multifile menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSaveMultiFileWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                              );

/*****************************************************************************/
//  Description :mms save multifile view detail handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMultifileViewWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                              );

/*****************************************************************************/
//  Description :mms notification detail read menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMMSNotiWinMsg(
                                              MMIMMS_WINDOW_ID_E win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              );

/*****************************************************************************/
//  Description :send box mms read menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendboxMMSOptMenuWinMsg(
                                                  MMIMMS_WINDOW_ID_E win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                );

/*****************************************************************************/
//  Description :out box mms read menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutboxMMSOptMenuWinMsg(
                                                 MMIMMS_WINDOW_ID_E win_id, 
                                                 MMI_MESSAGE_ID_E msg_id, 
                                                 DPARAM param
                                                );

/*****************************************************************************/
//  Description :reading edit mms document play or preview
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint8  GetMMSPreviewOrPlay(void);

/*****************************************************************************/
//  Description :mms preview  handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewWinMsg(
                                           MMIMMS_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );

/*****************************************************************************/
//  Description :mms preview get number handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewNumberPopWinMsg(
                                                    MMIMMS_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Description :mms preview get email handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewEmailPopWinMsg(
                                                    MMIMMS_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Description :mms preview get url handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewUrlPopWinMsg(
                                                    MMIMMS_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Description :mms send rpt waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendReadRptWaitingWin(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                );

/*****************************************************************************/
//  Description :mms preview play the current slide
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  MMSPreviewPlayCurrentSlide(MMI_CTRL_ID_T richtext_ctrl_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description :music play callback
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsRingPlayCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
//  Description :video play callback
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
#ifdef VIDEO_PLAYER_SUPPORT
LOCAL void MmsVideoPlayCallBack(VP_CALLBACK_T *msg_ptr);
#endif
/*****************************************************************************/
//  Description :stop music 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsStopPreviewRing(void);

/*****************************************************************************/
//  Description :mms preview richtext add text item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsPreviewAddTextItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description :mms preview richtext add pic or video item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsPreviewAddPicAndMovieItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description :mms preview richtext add music item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsPreviewAddMusicItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description :mms inbox outbox sendbox detail entry
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailEntry(MMI_CTRL_ID_T ctrl_id, uint32 record_id);

/*****************************************************************************/
//  Description :mms notification detail  entry
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSNotiShowEntry(MMI_CTRL_ID_T ctrl_id, uint32 record_id);

/*****************************************************************************/
//  Description :add list item
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(                                    
                          wchar                     *string_ptr,
                          uint8                    string_len,
                          MMI_CTRL_ID_T            ctrl_id,
                          MMI_TEXT_ID_T           left_softkey_id
                          );

/*****************************************************************************/
//  Description :add list item in list box
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AddMultifileItemToList(P_MMS_CONTENT_T mms_content_ptr, MMI_CTRL_ID_T menu_ctrl_id);

/*****************************************************************************/
//  Description :mms  detail size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailSizeString(MMI_STRING_T *str_dst, uint32 size);

/*****************************************************************************/
//  Description :mms  detail subject
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailSubjectString(MMI_STRING_T *str_dst, wchar *subject_ptr, uint16 subject_len);

/*****************************************************************************/
//  Description :mms  detail expiry time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailExpiryString(MMI_STRING_T *str_dst, uint32 expiry_time);

/*****************************************************************************/
//  Description :mms  detail to
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailToString(MMI_STRING_T *str_dst, wchar *to_ptr);

/*****************************************************************************/
//  Description :mms  detail priority
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailPriorityString(MMI_STRING_T *str_dst, int32 priority);

/*****************************************************************************/
//  Description :free item data
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void FreeItemData(MMI_STRING_T *str_dst);

/*****************************************************************************/
//  Description :cat tow string
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_CatString(MMI_STRING_T *dst_ptr, MMI_STRING_T *src1_ptr, MMI_STRING_T *src2_ptr);

/*****************************************************************************/
//  Description :mms preview richtext add music item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsPreviewPlayMusicItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description :mms preview richtext play 3gp item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MmsPreviewPlayVideoItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);

/*****************************************************************************/
//  Description :open mms detail read option menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSDetailOptPopMenu(uint32 record_id);

/*****************************************************************************/
//  Description :insert dynamenu node by label
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,          //位置
                             BOOLEAN            is_grayed
                             );

/*****************************************************************************/
//  Description :set preview is play video 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsPlayVideo(BOOLEAN is_play);

/*****************************************************************************/
//  Description :set preview is play music 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsPlayMusic(BOOLEAN is_play);

/*****************************************************************************/
//  Description :get preview play video time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 GetMMSPreviewPlayVideoTime(void);

/*****************************************************************************/
//  Description :set preview play video time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSPreviewPlayVideoTime(uint32 time);

/*****************************************************************************/
//  Description :get preview is play video losefocus
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsPlayVideoLosefocus(void);

/*****************************************************************************/
//  Description :set preview is play video losefocus
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsPlayVideoLosefocus(BOOLEAN is_lose_foucs);

/*****************************************************************************/
//  Description :get preview is parser text
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsParserText(void);

/*****************************************************************************/
//  Description :set preview is parser text
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsParserText(BOOLEAN is_parser);

/*****************************************************************************/
//  Description :get preview is extract number/url
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsExtract(void);

/*****************************************************************************/
//  Description :set preview is extract number/url
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsExtract(BOOLEAN is_extract);

/*****************************************************************************/
//  Description :get preview is web to play
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsWebPlay(void);

/*****************************************************************************/
//  Description :set preview is web to play
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsWebPlay(BOOLEAN is_play);

/*****************************************************************************/
//  Description :view the current multifile
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 该函数显示的是彩信保存多媒体文件的时候查看单个问题的具体内容，图片
//        就显示该图片，声音和视频显示名称并且播放，text显示该text，其他附件就显示名称
/*****************************************************************************/
LOCAL BOOLEAN ViewMultifile(wchar *file_name_str);
/*****************************************************************************/
//  Description :play mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BoxPlayMMS(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr);
/*****************************************************************************/
//  Description :stop tts  
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 该函数的TTS宏控制在函数内容，如果没有定义TTS宏，则函数体为空，不能把TTS
//        宏放到函数，否则没定义宏时会编译不过!!!
/*****************************************************************************/
LOCAL void MmsStopPreviewTTSReading(void);
#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description :显示TTS 按键，如果条件适当的话, 如果不适合，则返回FALSE，交由其他处理
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MmsShowTTSKeyIfSuitable(MMIMMS_WINDOW_ID_E win_id, BOOLEAN is_refresh);
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MmsIsSuitableForTTS(void);
#endif

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description :get pre font
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL GUI_FONT_T MMIMMS_GetPreFont(void);

/*****************************************************************************/
//  Description :set pre font
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIMMS_SetPreFont(GUI_FONT_T font);
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get preview rich text rect
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_GetPreviewRichtextRect(GUI_RECT_T *rect_ptr);
/*****************************************************************************/
//  Description :mms noti win msg
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMMSNotiWinPenOkMsg(MMI_WIN_ID_T      win_id,         //in:
                              MMI_CTRL_ID_T     ctrl_id,
                              uint32 record_id);
/*****************************************************************************/
//  Description :set preview win msg
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMMSPreviewWinPenOkMsg(MMI_WIN_ID_T      win_id,         //in:
                              MMI_CTRL_ID_T     ctrl_id,
                              uint8 *timer_id,
                              uint8 *duration_id);
/*****************************************************************************/
//  Description :button form ctrl
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MmsDestroyMMSButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMIMMS_BUTTON_T *button_array,uint16 array_len);
/*****************************************************************************/
//  Description :construct button form ctrl
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MmsConstructMMSButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMIMMS_BUTTON_T *button_array,uint16 array_len);
/*****************************************************************************/
//  Description :Insert Node by label
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void InsertNodeByLabelEx(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                             MMI_IMAGE_ID_T          menu_icon_id,
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,          //位置
                             BOOLEAN            is_graged
                             );         

/*****************************************************************************/
//  Description :noti detail win button form
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSNotiDetailWinButtonFormInit(MMIMMS_WINDOW_ID_E win_id);   

/*****************************************************************************/
//  Description :left right key init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMSPreviewWinLFRHButtonInit(void);                                           
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :save file opt menu
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSaveFileOptMenuWinMsg(
                                           MMIMMS_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           );       
#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleMMSWhetherInstallWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             );
#endif                                             
#endif
/*****************************************************************************/
//  Description :show preview time
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowPreviewTime(MMIMMS_WINDOW_ID_E win_id,uint16 *duration_time);

#ifdef MMI_MMS_ADJUST_VOLUME_SUPPORT
/*****************************************************************************/
//  Description :mms preview  adjust volums handle
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr);

/*****************************************************************************/
//  Description :mms preview  adjust volums
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
LOCAL void OpenVolumePanel(void);
#endif
/**---------------------------------------------------------------------------*
**                         Constant Variables                                 *
**---------------------------------------------------------------------------*/
//preview main menu
WINDOW_TABLE(MMIMMS_PREVIEW_TAB) = 
{

    #ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
    #endif
    WIN_FUNC((uint32)HandleMMSPreviewWinMsg),    
    WIN_ID(MMIMMS_PREVIEW_WIN_ID),
    WIN_TITLE(TXT_MMS),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
   
#ifdef MMI_PDA_SUPPORT
    CREATE_BUTTON_CTRL(IMAGE_NULL,MMIMMS_BUTTON_LF_CTRL_ID),   
    CREATE_BUTTON_CTRL(IMAGE_NULL,MMIMMS_BUTTON_RH_CTRL_ID),
#else
    CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_LBTN_UN,MMIMMS_BUTTON_LF_CTRL_ID),    
    CREATE_BUTTON_CTRL(IMAGE_COMMON_COUNT_RBTN_UN,MMIMMS_BUTTON_RH_CTRL_ID),
#endif
    
    CREATE_RICHTEXT_CTRL(MMIMMS_PREVIEW_CTRL_ID),


        
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

WINDOW_TABLE(MMIMMS_PREVIEW_NUMBER_POP_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSPreviewNumberPopWinMsg),    
    WIN_ID(MMIMMS_PREVIEW_NUMBER_POP_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_MMS_NUMBER_POP_MENU, MMIMMS_PREVIEW_NUMBER_POP_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIMMS_PREVIEW_URL_POP_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSPreviewUrlPopWinMsg),    
    WIN_ID(MMIMMS_PREVIEW_URL_POP_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_MMS_URL_POP_MENU, MMIMMS_PREVIEW_URL_POP_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIMMS_PREVIEW_EMAIL_POP_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMMSPreviewEmailPopWinMsg),    
    WIN_ID(MMIMMS_PREVIEW_EMAIL_POP_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_MMS_EMAIL_POP_MENU, MMIMMS_PREVIEW_EMAIL_POP_CTRL_ID),
    END_WIN
};

//mms play opt menu
WINDOW_TABLE(MMIMMS_INBOX_MMSOPTMENU_WIN_TAB) = 
{
    //WIN_TITLE(TXT_MMS),
    WIN_FUNC((uint32)HandleInboxMMSOptMenuWinMsg),    
    WIN_ID(MMIMMS_INBOX_MMSOPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_MMS_INBOX_MMSOPTION, MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIMMS_SENTBOX_MMSMENU_WIN_TAB) = 
{
    //WIN_TITLE(TXT_MMS),
    WIN_FUNC((uint32)HandleSendboxMMSOptMenuWinMsg),    
    WIN_ID(MMIMMS_SENTBOX_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_MMS_SENTBOX_OPTION, MMIMMS_SENTBOX_OPT_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIMMS_OUTBOX_MENU_WIN_TAB) = 
{
    //WIN_TITLE(TXT_MMS),
    WIN_FUNC((uint32)HandleOutboxMMSOptMenuWinMsg),    
    WIN_ID(MMIMMS_OUTBOX_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_MMS_OUTBOX_OPTION, MMIMMS_OUTBOX_OPT_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIMMS_INBOX_NOTIDETAIL_WIN_TAB) = 
{
    #ifdef MMI_PDA_SUPPORT
    WIN_STATUSBAR,
    #endif
    WIN_TITLE(TXT_NULL),
    WIN_FUNC((uint32)HandleInboxMMSNotiWinMsg),    
    WIN_ID(MMIMMS_INBOX_NOTIDETAIL_WIN_ID),            
    CREATE_RICHTEXT_CTRL(MMIMMS_NOTIDETAIL_TEXTBOX_CTRL_ID),        
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    
    #ifdef MMI_PDA_SUPPORT
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_SBS,MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL,MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL,MMIMMS_INBOX_NOTIDETAIL_BUTTON_SMS_REPLY_CTRL_ID, MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID),
    CHILD_BUTTON_CTRL(FALSE, IMAGE_NULL,MMIMMS_INBOX_NOTIDETAIL_BUTTON_DELETE_CTRL_ID, MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID),
    #endif

    END_WIN
};

WINDOW_TABLE(MMIMMS_INBOX_MMSNOTIMENU_WIN_TAB) = 
{
    //WIN_TITLE(TXT_MMS),
    WIN_FUNC((uint32)HandleInboxMMSDetailOptMenuWinMsg),    
    WIN_ID(MMIMMS_INBOX_MMSNOTIOPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
//    CREATE_POPMENU_CTRL(MENU_MMS_INBOX_MMSNOTIOPTION, MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIMMS_SAVE_MULTIFILE_LIST_TAB) = 
{
    WIN_TITLE(TXT_COMM_SAVE_MEDIA_FILE),
    WIN_FUNC((uint32)HandleSaveMultiFileWinMsg),    
    WIN_ID(MMIMMS_SAVE_MULTIFILE_LIST_WIN_ID),
#ifdef MMI_GUI_STYLE_TYPICAL
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
#else
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
#endif    
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMMS_SAVE_MULTIFILE_LIST_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIMMS_MULTIFILE_VIEW_TAB) = 
{
    WIN_FUNC((uint32)HandleMultifileViewWinMsg),    
    WIN_ID(MMIMMS_MULTIFILE_VIEW_WIN_ID),
    WIN_TITLE(STXT_PREVIEW),
    WIN_SOFTKEY(STXT_SAVE, TXT_NULL, STXT_RETURN),
    
    CREATE_RICHTEXT_CTRL(MMIMMS_MULTIFILE_VIEW_CTRL_ID),
    WIN_MOVE_STYLE(MOVE_FORBIDDEN),
    END_WIN
};

#ifndef MMI_GUI_STYLE_TYPICAL
WINDOW_TABLE(MMIMMS_SAVE_FILE_OPT_TAB) = 
{
    //WIN_TITLE(TXT_MMS_MMS),
    WIN_FUNC((uint32)HandleSaveFileOptMenuWinMsg),    
    WIN_ID(MMIMMS_SAVE_FILE_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_MMS_SAVEFILE_OPTION, MMIMMS_SAVE_FILE_MENU_CTRL_ID),
    END_WIN
};
#endif

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
#ifdef MMI_MMS_ADJUST_VOLUME_SUPPORT
/*****************************************************************************/
//  Description :mms preview  adjust volums handle
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
LOCAL void SetVolumeCallback(MMICOM_PANEL_CALLBACK_T *para_ptr)
{
    if(PNULL != para_ptr)
    {
        MMIAPISET_SetMultimRealVolume(para_ptr->cur_value);
    }
}

/*****************************************************************************/
//  Description :mms preview  adjust volums
//  Global resource dependence : 
//  Author:CBK
//  Note: 
/*****************************************************************************/
LOCAL void OpenVolumePanel(
                void
                )
{
    MMICOM_PANEL_INFO_T panel_info = {0};

    panel_info.x = -1;
    panel_info.y = -1;
    panel_info.cur_value = MMIAPISET_GetMultimVolume();
    panel_info.last_value = MMIAPISET_GetLastMultimVolume();
    panel_info.min_value = MMISET_VOL_ZERO;   
    panel_info.max_value = MMISET_VOL_MAX;
    panel_info.set_callback = SetVolumeCallback;
#ifndef UI_MULTILAYER_SUPPORT
#if 0
	//panel_info.createwin_style = MMICOM_PLAYER_PANEL_CREATE_WIN;
#endif
#endif
    MMIAPICOM_OpenPanelChildWin(MMK_GetFocusWinId(),&panel_info);
}
#endif
#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :mms preview  handle fun
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewWinMsg(
                                           MMIMMS_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr                = PNULL;
    MMI_CTRL_ID_T richtext_ctrl_id                       = MMIMMS_PREVIEW_CTRL_ID;    
    static uint8 s_preview_timer_id                      = 0;                       //static变量记录定时器id   
    static uint8 s_preview_duration_id                   = 0;                       //播放时效timer
    static uint16 s_preview_duration_time_sec                   = 0;                       //播放时效计时
    uint32  duaration                                    = MMIMMS_SLIDE_DURATION_MIN * MMIMMS_SLIDE_TIME_DURING;      //当前帧播放间隔
    MMI_RESULT_E    ret                                  = MMI_RESULT_TRUE;         //返回值  
    uint16 cur_slide_index                               = 0;
    MMIMMS_ERROR_E   slide_err                           = MMIMMS_NO_ERROR;    
    uint16 slide_count                                   = 0;    
    MMISMS_ORDER_INFO_T *mms_list_info_ptr               = PNULL;
    uint32 vedio_time                                    = 0;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element            = {0};/*lint !e64*/
    wchar *focus_url_ptr                                 = PNULL;
    wchar *focus_num_ptr                                 = PNULL;
    uint8 *number_ptr                                    = PNULL;
    GUIRICHTEXT_FOCUS_TYPE_T focus_type                  = GUIRICHTEXT_FOCUS_NONE;
    uint16 num_len                                       = 0;
    CC_RESULT_E cc_result                                = CC_RESULT_SUCCESS;
    GUI_RECT_T  richtext_rect                            = {0};
    GUI_RECT_T      button_rect1                         = {0};
    GUI_RECT_T      button_rect2                         = {0};
    MMIMMS_WINDOW_ID_E quer_win_id                       = MMIMMS_INSERT_PIC_QUERY_WIN_ID;

    edit_doc_ptr = GetMMSReadEditDoc();    
    //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg msg_id=%x,cur_edoc=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_833_112_2_18_2_38_36_278,(uint8*)"dd",msg_id,edit_doc_ptr);
    switch(msg_id)
    {
    case MSG_LCD_SWITCH:
        MMIMMS_GetRichtextRect(&richtext_rect);
        MMIMMS_GetPagebarLbinRect(&button_rect1);
        MMIMMS_GetPagebarRbinRect(&button_rect2);
        GUIBUTTON_SetRect(MMIMMS_BUTTON_LF_CTRL_ID,&button_rect1);
        GUIBUTTON_SetRect(MMIMMS_BUTTON_RH_CTRL_ID,&button_rect2);
        GUIRICHTEXT_SetRect(richtext_ctrl_id,richtext_rect);
        break;
#ifdef VIDEO_PLAYER_SUPPORT
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
#ifndef MMI_MMS_ADJUST_VOLUME_SUPPORT
        if (!GetMMSPreviewIsPlayVideo())
        {
            ret = MMI_RESULT_FALSE;
        }
#else
        if (!GetMMSPreviewIsPlayVideo())
        {
    	     //for bug 82827
    	     OpenVolumePanel();
    	     ret = MMI_RESULT_FALSE;
        }
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
            if (GetMMSPreviewIsPlayVideo())
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_855_112_2_18_2_38_36_279,(uint8*)"");
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                MMIAPIVP_ExitPlay();                
                break;
            }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527           
#endif 

#ifdef MMI_PDA_SUPPORT
    case MSG_PROMPTWIN_OK:
        {
                mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
                if (PNULL != mms_list_info_ptr)
                {
                    //delete the mms 
                    MMIMMS_DeleteMMSAndSaveList(mms_list_info_ptr->record_id);
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                    
#endif
                    MMK_CloseWin(win_id);
                    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
                    {
                        MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                    }
                } 
        }
                break;
    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(quer_win_id);
        break; 
        
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef VIDEO_PLAYER_SUPPORT
            if (GetMMSPreviewIsPlayVideo())
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_891_112_2_18_2_38_36_280,(uint8*)"");
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                MMIAPIVP_ExitPlay();                
                break;
            }
#endif
#ifdef MMI_PDA_SUPPORT
            HandleMMSPreviewWinPenOkMsg(win_id,((MMI_NOTIFY_T*)param)->src_id,&s_preview_timer_id, &s_preview_duration_id);
#endif            
            if(MMIMMS_BUTTON_LF_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                msg_id = MSG_APP_LEFT;
                MMK_SendMsg(win_id,msg_id,param);
            }
            else if(MMIMMS_BUTTON_RH_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                msg_id = MSG_APP_RIGHT;
                MMK_SendMsg(win_id,msg_id,param);
            }  
            else if (MMIMMS_PREVIEW_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                msg_id = MSG_APP_WEB;
                MMK_SendMsg(win_id,msg_id,param);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_OPEN_WINDOW://窗口打开消息
    #ifdef MMI_PDA_SUPPORT

    if(MMS_PLAY == GetMMSPreviewOrPlay())
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
        if (PNULL != mms_list_info_ptr)
        {
            if(MMISMS_MO_SEND_SUCC == mms_list_info_ptr->mo_mt_type)
            {
                MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_sendbox_play_form_button_info,
                sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                );
                MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_sendbox_play_form_button_info,
                sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                );
                
            }
            else if(MMISMS_MO_SEND_FAIL == mms_list_info_ptr->mo_mt_type)
            {
                MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_outbox_play_form_button_info,
                sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                );
                MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_outbox_play_form_button_info,
                sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                );            
            }
            else
            {
                MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_mtbox_play_form_button_info,
                sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                );
                MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_mtbox_play_form_button_info,
                sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                );
            }
            if (mms_list_info_ptr->is_lock)
            {
                GUI_BG_T fg_info = {0};
                
                fg_info.bg_type = GUI_BG_IMG;
                fg_info.img_id = IMAGE_SMS_DELETE_GRAY_SMS_PDA;
                GUIBUTTON_SetGrayed(MMIMMS_PREVIEW_BUTTON_DELETE_CTRL_ID, TRUE, &fg_info, FALSE);
            }
        }
    }
    else
    {
        GUI_BG_T button_bg = {0};
        button_bg.bg_type = GUI_BG_IMG;
        MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
        s_play_form_button_info,
        sizeof(s_play_form_button_info)/sizeof(s_play_form_button_info[0])
        );
        MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
        s_play_form_button_info,
        sizeof(s_play_form_button_info)/sizeof(s_play_form_button_info[0])
        );       
        button_bg.img_id = IMAGE_MMS_PAUSE_PDA;
        GUIBUTTON_SetFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);    
        GUIBUTTON_SetPressedFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);    
    }

        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);    
#endif
        MMIMMS_SetFreq();
        MMIMMS_GetRichtextRect(&richtext_rect);
        MMIMMS_GetPagebarLbinRect(&button_rect1);
        MMIMMS_GetPagebarRbinRect(&button_rect2);
        GUIBUTTON_SetRect(MMIMMS_BUTTON_LF_CTRL_ID,&button_rect1);
        GUIBUTTON_SetRect(MMIMMS_BUTTON_RH_CTRL_ID,&button_rect2);
        #ifdef MMI_PDA_SUPPORT
        MMSPreviewWinLFRHButtonInit();
        #endif
        GUIRICHTEXT_SetRect(richtext_ctrl_id,richtext_rect);
        if(edit_doc_ptr)
        {
            //初始化static变量            
            s_preview_timer_id=0;
            //设置背光灯常亮
            MMIDEFAULT_SetBackLight(TRUE);            
            MMIMMS_WinInit(EDIT_PREVIEW_WIN_ENTRY);
            SetMMSPreviewIsWebPlay(FALSE);
            //定位到第一帧
            MMIMMS_HeadSlide(edit_doc_ptr);
            MMIMMS_CLEAR_CLIENT(win_id);

            if (MMS_PLAY == GetMMSPreviewOrPlay())
            {
                s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
                GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL); 
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
#ifdef TTS_SUPPORT
                if(!MmsShowTTSKeyIfSuitable(win_id, FALSE))
#endif
                {
#ifdef MMI_PDA_SUPPORT
                    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
                    GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_COMMON_PLAY, STXT_RETURN, FALSE); 
                }
            }
            else
            {
                s_edit_mms_preview_state = MMIMMS_PREVIEW_PLAYING;
                GUIRICHTEXT_DisableClipBoard(richtext_ctrl_id);
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#ifdef VIDEO_PLAYER_SUPPORT
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {     
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                    uint32 video_type     = MMS_TYPE_MAX;   
                    VP_PLAY_PARAM_T play_param  = {0};
                    MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);                      
                    if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif                        
                    {
                        MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);
                    }
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                    else
                    {
                        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
                        GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_PAUSE, STXT_RETURN, FALSE);     
                        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                    }
#endif                    
                }
                else
#endif                    
                {

                    GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
                    GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_PAUSE, STXT_RETURN, FALSE);     
                    MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                } 
            
            }         
        }
        else
        {
            MMK_CloseWin(win_id);
        }        
        //GUIRICHTEXT_Update(richtext_ctrl_id);
        s_preview_duration_time_sec = 0;
        MMK_SetAtvCtrl(win_id, richtext_ctrl_id);
        GUIWIN_SetTitleTextScroll(win_id, TITLE_NOSCROLL_BY_ELLIPSIS);
        break;
    case MSG_CLOSE_WINDOW:
        {
            //恢复允许关闭背光灯
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);            
            //关闭定时器
            if(0 != s_preview_timer_id)
            {
                MMK_StopTimer(s_preview_timer_id);
                s_preview_timer_id=0;
            }

            if(0 != s_preview_duration_id)
            {
                MMK_StopTimer(s_preview_duration_id);
                s_preview_duration_id = 0;                
            }
            s_preview_duration_time_sec = 0;
            
#ifdef VIDEO_PLAYER_SUPPORT
            if (GetMMSPreviewIsPlayVideo())
            {
                MMIAPIVP_ExitPlay();
            }
#endif
            // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
            MmsStopPreviewTTSReading();
            //关闭预览的铃声
            MmsStopPreviewRing();
            //退出预览模块,根据情况释放block,恢复背景音等
            GUIRICHTEXT_DeleteAllItems(richtext_ctrl_id);
            GUIRICHTEXT_ResetState(richtext_ctrl_id);
            MMIMMS_MMSPreviewExit();
            SetMMSPreviewIsWebPlay(FALSE);
            SetMMSPreviewIsPlayVideoLosefocus(FALSE);
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_NUMBER_POP_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_NUMBER_POP_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_URL_POP_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_URL_POP_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_INBOX_MMSOPT_MENU_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_INBOX_MMSOPT_MENU_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_SENTBOX_OPT_MENU_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_SENTBOX_OPT_MENU_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_OUTBOX_OPT_MENU_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_OUTBOX_OPT_MENU_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID) && !MMIMMS_GetPreviewMMSDetailFlag())
            {
                MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_SAVE_MULTIFILE_LIST_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_SAVE_MULTIFILE_LIST_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_MULTIFILE_VIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_MULTIFILE_VIEW_WIN_ID);
            }
            MMIMMS_WinExit(EDIT_PREVIEW_WIN_ENTRY); 
            MMK_CloseWin(quer_win_id);
            MMIMMS_RestoreFreq();
        }
        break;    
    case MSG_FULL_PAINT:        
        //if(!MMK_IsFocusWin(win_id) && edit_doc_ptr)//失去焦点下的刷品将半屏之外的地方还原
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video! MSG_FULL_PAINT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1131_112_2_18_2_38_36_281,(uint8*)"");
            break;
        }
        MMIMMS_CLEAR_CLIENT(win_id);
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state &&  MMS_PLAY == GetMMSPreviewOrPlay())
        { 
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
            switch(richtext_element.type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_NONE:
                {
#ifdef TTS_SUPPORT
                    if(!MmsShowTTSKeyIfSuitable(win_id, FALSE))
#endif 
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                }
                break;
            default:                        
                break;
            }               
        }// if(处于停止状态)
        if (MMS_PLAY == GetMMSPreviewOrPlay())
        {
            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
        }
        else
        {
            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
        }
        break;
        
    case MSG_MMS_PLAY_VIDEO:
        if (MmsPreviewPlayVideoItem(edit_doc_ptr))
        { 
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, FALSE);
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg play video"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1181_112_2_18_2_38_36_282,(uint8*)"");
        }
        else
        {
            s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
            GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            if (MMS_PLAY == GetMMSPreviewOrPlay())
            {
#ifdef MMI_PDA_SUPPORT
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
                GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_COMMON_PLAY, STXT_RETURN, FALSE);
            }
            else
            {
#ifdef MMI_PDA_SUPPORT
                GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
                GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_COMMON_PLAY, STXT_RETURN, FALSE);
            }
            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
        }

        break;
		
#ifdef VIDEO_PLAYER_SUPPORT
    case MSG_MMS_END_VIDEO:
        MMIAPIVP_ExitPlay();        
        GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_PAUSE, STXT_RETURN, TRUE);
        //MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL);
        //MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
        break;
#endif
    case MSG_MMS_PLAY_SLIDE:
        {
            if (GetMMSPreviewIsPlayVideo())
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video! MSG_MMS_PLAY_SLIDE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1219_112_2_18_2_38_37_283,(uint8*)"");
                break;
            }
            //MMIMMS_CLEAR_CLIENT(win_id);
            GUIRICHTEXT_DeleteAllItems(richtext_ctrl_id);
            GUIRICHTEXT_ResetState(richtext_ctrl_id);
            if (MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state && FALSE == GetMMSPreviewIsPlayVideo())
            {
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id = 0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }

                s_preview_duration_time_sec = 0;
                                
                vedio_time = GetMMSPreviewPlayVideoTime();                

                duaration = edit_doc_ptr->editbody_ptr->slide_cur->times;

                if (vedio_time < duaration)
                {
                    duaration = duaration - vedio_time;
                }
                if (duaration == 0)
                {
                    duaration = MMIMMS_SLIDE_PLAY_DEFAULT_TIME;
                }
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg vedio_time=%d,duaration=%d, music_time= %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1244_112_2_18_2_38_37_284,(uint8*)"ddd",vedio_time,duaration);
                s_preview_timer_id = MMK_CreateWinTimer(win_id, duaration + 100, FALSE);//Add 100ms to show End timer

                s_preview_duration_id = MMK_CreateWinTimer(win_id, 1000, TRUE);
                MMSShowPreviewTime(win_id, &s_preview_duration_time_sec);
                if (0 != vedio_time)
                {
                    SetMMSPreviewPlayVideoTime(0);
                } 
            }
            
            if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state && MMS_PREVIEW == GetMMSPreviewOrPlay())
            {
                GUI_BG_T button_bg = {0};
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id = 0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }
                button_bg.bg_type = GUI_BG_IMG;
                button_bg.img_id = IMAGE_MMS_PREVIEW_PDA;
                GUIBUTTON_SetFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);  
                GUIBUTTON_SetPressedFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);                           
                GUIBUTTON_Update(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID);
                GUIWIN_SetTitleSubText(win_id, FALSE, PNULL, 0, FALSE);
                GUIWIN_UpdateTitle(win_id);
            }
            
            
            if (MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state && MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
            {
                GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
            }
            else
            {
                GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
            }
            MMSPreviewPlayCurrentSlide(richtext_ctrl_id, edit_doc_ptr);
            if (MMS_PLAY == GetMMSPreviewOrPlay())
            {
                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
            }
            else
            {
                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
            }
            if (MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
            {
                MmsPreviewPlayMusicItem(edit_doc_ptr);
            }
        }       
        break;
    case MSG_LOSE_FOCUS:
#ifdef VIDEO_PLAYER_SUPPORT
        if (GetMMSPreviewIsPlayVideo())
        {
            MMIAPIVP_ExitPlay();
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, FALSE); 
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
            SetMMSPreviewIsPlayVideoLosefocus(TRUE);
        }
#endif
        if(MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
        {
            //get focus的时候要恢复播放
            s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
            GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            if (MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
            {
                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL); 
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
            }
        }
        //关闭定时器
        if(0 != s_preview_timer_id)
        {
            MMK_StopTimer(s_preview_timer_id);
            s_preview_timer_id = 0;
        }
        
        if(0 != s_preview_duration_id)
        {
            MMK_StopTimer(s_preview_duration_id);
            s_preview_duration_id = 0;
        }
        
        s_preview_duration_time_sec = 0;
        GUIWIN_SetTitleSubText(win_id, FALSE, PNULL, 0, FALSE);
        // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
        MmsStopPreviewTTSReading();
        MmsStopPreviewRing();
        break;
    case MSG_GET_FOCUS:
        //防止焦点切换后，会锁屏 
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video! MSG_GET_FOCUS"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1323_112_2_18_2_38_37_285,(uint8*)"");
            break;
        }

#ifdef MMI_PDA_SUPPORT
        if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state)
        {
            if(MMS_PLAY == GetMMSPreviewOrPlay())
            {
                mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
                if (PNULL != mms_list_info_ptr)
                {
                    if(MMISMS_MO_SEND_SUCC == mms_list_info_ptr->mo_mt_type)
                    {
                        MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                        s_sendbox_play_form_button_info,
                        sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                        );
                        MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                        s_sendbox_play_form_button_info,
                        sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                        );
                        
                    }
                    else if(MMISMS_MO_SEND_FAIL == mms_list_info_ptr->mo_mt_type)
                    {
                        MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                        s_outbox_play_form_button_info,
                        sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                        );
                        MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                        s_outbox_play_form_button_info,
                        sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                        );            
                    }
                    else
                    {
                        MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                        s_mtbox_play_form_button_info,
                        sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                        );
                        MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                        s_mtbox_play_form_button_info,
                        sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                        );
                    }
                    if (mms_list_info_ptr->is_lock)
                    {
                        GUI_BG_T fg_info = {0};
                        
                        fg_info.bg_type = GUI_BG_IMG;
                        fg_info.img_id = IMAGE_SMS_DELETE_GRAY_SMS_PDA;
                        GUIBUTTON_SetGrayed(MMIMMS_PREVIEW_BUTTON_DELETE_CTRL_ID, TRUE, &fg_info, FALSE);
                    }
                }
            }
            else
            {
                GUI_BG_T button_bg = {0};
                button_bg.bg_type = GUI_BG_IMG;      
                button_bg.img_id = IMAGE_MMS_PREVIEW_PDA;
                GUIBUTTON_SetFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);    
                GUIBUTTON_SetPressedFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);    
            }
        }        
#endif        
        MMIMMS_CLEAR_CLIENT(win_id);
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        if(edit_doc_ptr)
        {
            if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state)
            {                
                switch(richtext_element.type) 
                {
                case GUIRICHTEXT_FOCUS_PHONENUM:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_URL:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_EMAIL:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_NONE:
                    if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                    else
                    {
#ifdef TTS_SUPPORT
                        if(!MmsShowTTSKeyIfSuitable(win_id, FALSE))
#endif 
                        {
                            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }
                    }
                    break;
                default:                        
                    break;
                }               
            }// if(处于停止状态)
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            //如果字体不一样，在losefocus的状态发生了改变，重新画richtext
            if (MMIMMS_GetPreFont() != MMIAPISMS_GetWinFont())
            {
                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            }
#endif
            if (MMS_PLAY == GetMMSPreviewOrPlay())
            {
                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
            }
            else
            {
                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
            }
        }
        break;
    case MSG_TIMER:
        if(s_preview_timer_id == *((uint8 *) param) && edit_doc_ptr)
        {
            if (GetMMSPreviewIsPlayVideo())
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1384_112_2_18_2_38_37_286,(uint8*)"");
                break;
            }
            if(MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
            {
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id=0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }

                s_preview_duration_time_sec = 0;
                
                MmsStopPreviewRing();
                // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
                MmsStopPreviewTTSReading();
                if(MMK_IsFocusWin(win_id))
                {
                    cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                    slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);
                    
                    if(slide_count == cur_slide_index)
                    {         
                        if (1 < slide_count)
                        {
                            MMIMMS_HeadSlide(edit_doc_ptr);
                        }
                        if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                        {
#ifdef MMI_PDA_SUPPORT
                            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
                            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }
                        else
                        {
#ifdef MMI_PDA_SUPPORT
                            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
                            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }
                        //更新工具栏
                        if (MMS_PLAY == GetMMSPreviewOrPlay())
                        {
                            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
                        }
                        else
                        {
                            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
                        } 
                        s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
                        GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
                        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                        if(MMS_PLAY == GetMMSPreviewOrPlay())
                        {
                            mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
                            MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                                s_play_form_button_info,
                                sizeof(s_play_form_button_info)/sizeof(s_play_form_button_info[0])
                                ); 
                            if(mms_list_info_ptr != PNULL)
                            {
                                if((MMISMS_MO_SEND_SUCC == mms_list_info_ptr->mo_mt_type))
                                {
                                    MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                                        s_sendbox_play_form_button_info,
                                        sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                                        );                             
                                } 
                                else if(MMISMS_MO_SEND_FAIL == mms_list_info_ptr->mo_mt_type)
                                {
                                    MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                                        s_outbox_play_form_button_info,
                                        sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                                        );      
                                }
                                else
                                {
                                    MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                                        s_mtbox_play_form_button_info,
                                        sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                                        );
                                }
                            }
                        }

                        GUIWIN_SetTitleSubText(win_id, FALSE, PNULL, 0, FALSE);
                        GUIWIN_UpdateTitle(win_id);
                        
                        if (MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                            GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
                        }
                        else// if (1 < slide_count)
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        }
                    }// if(最后一帧)
                    else
                    {
                        //定位到下一帧
                        MMIMMS_NextSlide(edit_doc_ptr);

                        if(0 != s_preview_timer_id)
                        {
                            MMK_StopTimer(s_preview_timer_id);
                            s_preview_timer_id = 0;
                        }
                        
                        if(0 != s_preview_duration_id)
                        {
                            MMK_StopTimer(s_preview_duration_id);
                            s_preview_duration_id = 0;
                        }
        
                        s_preview_duration_time_sec = 0;
                        //MMSShowPreviewTime(win_id,&s_preview_duration_time_sec);
                    
#ifdef NANDBOOT_SUPPORT
                        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);                           
                        }
                        else
                        { 

                            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_STOP, STXT_RETURN, TRUE);
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        }
#else
#ifdef VIDEO_PLAYER_SUPPORT
                        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                        {
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                             uint32 video_type     = MMS_TYPE_MAX;   
                             VP_PLAY_PARAM_T play_param  = {0};
                             MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);
                        
                             if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif
                            {
                                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                                MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);   
                            }
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                            else
                            {
                                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_STOP, STXT_RETURN, TRUE);
                                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                            }
#endif
                        }
                        else
#endif                            
                        { 

                            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_STOP, STXT_RETURN, TRUE);
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        }
#endif
                       
                        
                    }
                }//focus in 
                else
                {
                    s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
                    GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
                    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                    if (MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
                    {
                        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL); 
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
                    }
                    if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                    {
#ifdef MMI_PDA_SUPPORT
                        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
                        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                    else
                    {
#ifdef MMI_PDA_SUPPORT
                        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                    
                }
            }//if 播放状态
            else
            {
                //SCI_TRACE_LOW:"timer:playing state!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1541_112_2_18_2_38_37_287,(uint8*)"");
            }
        }
        else if(s_preview_duration_id == *((uint8 *) param) && edit_doc_ptr)
        {  
            MMSShowPreviewTime(win_id,&s_preview_duration_time_sec);
        }
        break;
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video! MSG_CTL_RICHTEXT_SWITCHFOCUS"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1549_112_2_18_2_38_37_288,(uint8*)"");
            break;
        }
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        switch(focus_type) 
        {
        case GUIRICHTEXT_FOCUS_PHONENUM:
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
        	break;
        case GUIRICHTEXT_FOCUS_URL:
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
            break;
        case GUIRICHTEXT_FOCUS_EMAIL:
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
            break;
        case GUIRICHTEXT_FOCUS_NONE:
            if (MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state)
            {
                if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                {
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                }
                else
                {
#ifdef TTS_SUPPORT
                    if(!MmsShowTTSKeyIfSuitable(win_id, TRUE))
#endif 
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                }
            }
            break;
        default:                        
            break;
        }
        break;

        case MSG_KEYUP_GREEN: // same as MSG_KEYUP_CALL1
        case MSG_KEYUP_CALL2:
        case MSG_KEYUP_CALL3:
        case MSG_KEYUP_CALL4:
            if (edit_doc_ptr)
            {
#ifndef MMI_MULTI_GREEN_KEY_SINGLE
                CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX);
#endif
                
                GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
                focus_type = richtext_element.type;
                mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
                if (GUIRICHTEXT_FOCUS_PHONENUM == focus_type)
                {
                    if (PNULL == mms_list_info_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg 1 mms_list_info_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1605_112_2_18_2_38_37_289,(uint8*)"");
                        break;
                    }
                    focus_num_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
                    if (PNULL == focus_num_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1611_112_2_18_2_38_37_290,(uint8*)"");
                        break;
                    }
                    SCI_MEMSET(focus_num_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
                    number_ptr = SCI_ALLOCAZ(richtext_element.data.len + 1);
                    if (PNULL == number_ptr)
                    {
                        SCI_Free(focus_num_ptr);
                        focus_num_ptr = PNULL;
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg number_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1621_112_2_18_2_38_37_291,(uint8*)"");
                        break;
                    }
                    SCI_MEMSET(number_ptr, 0, (richtext_element.data.len + 1));
                    MMI_WSTRNTOSTR((uint8 *)number_ptr, richtext_element.data.len, focus_num_ptr, richtext_element.data.len, richtext_element.data.len);
                    num_len = SCI_STRLEN((char *)number_ptr);                   
                    
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
                    cc_result = MMIAPICC_MakeCall(
                        mms_list_info_ptr->dual_sys,
                        number_ptr,
                        num_len,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
#else
                    cc_result = MMIAPICC_MakeCall(
                        mms_list_info_ptr->dual_sys,
                        number_ptr,
                        num_len,
                        PNULL,
                        PNULL,
                        sim_type,
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
#endif
                    
                    SCI_Free(focus_num_ptr);
                    focus_num_ptr = PNULL;
                    SCI_Free(number_ptr);
                    number_ptr = PNULL;
                }
                else
                {
                    if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state &&  MMS_PLAY == GetMMSPreviewOrPlay())
                    {
                        if (PNULL == mms_list_info_ptr)
                        {
                            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg 2 mms_list_info_ptr == PNULL!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1663_112_2_18_2_38_37_292,(uint8*)"");
                            break;
                        }
                        
                        num_len = SCI_STRLEN((char *)mms_list_info_ptr->sender);
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
                        cc_result = MMIAPICC_MakeCall(
                            mms_list_info_ptr->dual_sys,
                            mms_list_info_ptr->sender,
                            num_len,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_NORMAL_CALL,
                            PNULL
                            );
#else
                        cc_result = MMIAPICC_MakeCall(
                            mms_list_info_ptr->dual_sys,
                            mms_list_info_ptr->sender,
                            num_len,
                            PNULL,
                            PNULL,
                            sim_type,
                            CC_CALL_NORMAL_CALL,
                            PNULL
                            );
#endif
                    }
                }
            }
            break;

    case MSG_SK_SKB_ICON_PRESSED:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:        
        if(edit_doc_ptr)
        {
            GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
#ifdef VIDEO_PLAYER_SUPPORT
            if (GetMMSPreviewIsPlayVideo())
            {
                MMIAPIVP_ExitPlay();
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE); 
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
            }
            else
#endif
            {
                focus_type = richtext_element.type;
                if (GetMMSPreviewIsWebPlay())
                {
                    focus_type = GUIRICHTEXT_FOCUS_NONE;
                    SetMMSPreviewIsWebPlay(FALSE);
                }
                MmsStopPreviewRing();
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id = 0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }

                s_preview_duration_time_sec = 0;
                
                if (GUIRICHTEXT_FOCUS_PHONENUM == focus_type)
                {
                    focus_num_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
                    if (PNULL == focus_num_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1729_112_2_18_2_38_38_293,(uint8*)"");
                        break;
                    }
                    //SCI_MEMSET(focus_num_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
                    number_ptr = SCI_ALLOC_APP(richtext_element.data.len + 1);
                    SCI_MEMSET(number_ptr, 0, (richtext_element.data.len + 1));
                    MMI_WSTRNTOSTR((uint8 *)number_ptr, richtext_element.data.len, focus_num_ptr, richtext_element.data.len, richtext_element.data.len);
                    MMK_CreateWin((uint32 *)MMIMMS_PREVIEW_NUMBER_POP_WIN_TAB, (ADD_DATA)number_ptr);
                    SCI_Free(focus_num_ptr);
                    focus_num_ptr = PNULL;
                }
                else if(GUIRICHTEXT_FOCUS_URL == focus_type)
                {
                    focus_url_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
                    if (PNULL == focus_url_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg focus_url_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1746_112_2_18_2_38_38_294,(uint8*)"");
                        break;
                    }
                    //SCI_MEMSET(focus_url_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(focus_url_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
                    MMK_CreateWin((uint32 *)MMIMMS_PREVIEW_URL_POP_WIN_TAB,(ADD_DATA)focus_url_ptr);
                }
                else if (GUIRICHTEXT_FOCUS_EMAIL == focus_type)
                {
                    focus_num_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
                    if (PNULL == focus_num_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1758_112_2_18_2_38_38_295,(uint8*)"");
                        break;
                    }
                    //SCI_MEMSET(focus_num_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
                    number_ptr = SCI_ALLOC_APPZ(richtext_element.data.len + 1);
                    //SCI_MEMSET(number_ptr, 0, (richtext_element.data.len + 1));
                    MMI_WSTRNTOSTR((uint8 *)number_ptr, richtext_element.data.len, focus_num_ptr, richtext_element.data.len, richtext_element.data.len);
                    MMK_CreateWin((uint32 *)MMIMMS_PREVIEW_EMAIL_POP_WIN_TAB, (ADD_DATA)number_ptr);
                    SCI_Free(focus_num_ptr);
                    focus_num_ptr = PNULL;
                }
                else if (GUIRICHTEXT_FOCUS_NONE == focus_type)
                {
                    if(MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
                    {                     
                        if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                        {
#ifdef MMI_PDA_SUPPORT
                            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
                            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }
                        else if (MMS_PLAY == GetMMSPreviewOrPlay())
                        {
#ifdef MMI_PDA_SUPPORT
                            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
                            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }

                        s_edit_mms_preview_state=MMIMMS_PREVIEW_STOP;
                        GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
                        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                        if ( MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
                        {
                            //MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL); 
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                            GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
                        }
                    }// if(播放状态)
                    else if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state)
                    {
#ifdef TTS_SUPPORT
                        if(!MmsIsSuitableForTTS())
                        {
                            if(!BoxPlayMMS(edit_doc_ptr))
                            {
                                break;
                            }
                        }
                        else
                        {
                            // 如果TTS正在运行
                            if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_MMS))
                            {
                                MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_MMS);
                                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_SET_TTS_PLAY, STXT_RETURN, TRUE);
                            }
                            else
                            {
                                // 获取MMS的文本内容...
                                MMI_STRING_T    mms_text_string = {0};
                                MMITTS_PLAY_PARAM_T play_param = {0};
                                if (MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_text_string))
                                {
                                    play_param.txt_addr = (uint8 *)mms_text_string.wstr_ptr;
                                    play_param.txt_len_in_byte = mms_text_string.wstr_len * 2;
                                    play_param.is_ucs2 = TRUE;
                                    play_param.play_times = 1;
                                    play_param.tts_vol = 4;
                                    play_param.ring_type = TTS_RING_TYPE_MMS;
                                    play_param.win_handle = MMIMMS_PREVIEW_WIN_ID;
									play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
									play_param.digit_mode = TTS_DIGIT_AUTO;
									play_param.punc_mode = TTS_PUNC_OFF;
                                    if(MMIAPITTS_PlayText(&play_param))
                                    {
                                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_STOP, STXT_RETURN, TRUE);
                                    }
                                }
                            }
                        }
#else
                        if(!BoxPlayMMS(edit_doc_ptr))
                        {
                            break;
                        }
#endif
                    }//else if(s_edit_mms_preview_state==MMIMMS_PREVIEW_STOP)
                    else
                    {
                        //SCI_TRACE_LOW:"s_edit_mms_preview_state=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1847_112_2_18_2_38_38_296,(uint8*)"d",s_edit_mms_preview_state);
                    }
                } 
                else
                {
                    //SCI_TRACE_LOW:"focus item type error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1852_112_2_18_2_38_38_297,(uint8*)"");
                }
            }            
        }            
        break;

#ifdef TTS_SUPPORT		
    case MSG_TTS_OVER:
        MmsShowTTSKeyIfSuitable(win_id, TRUE);
        break;
#endif

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        if(!edit_doc_ptr)
        {
            break;
        }        
        //左软键进选项菜单
#ifdef VIDEO_PLAYER_SUPPORT
        if (GetMMSPreviewIsPlayVideo())
        {
            MMIAPIVP_ExitPlay();
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
        }
#endif        
        if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state && MMS_PREVIEW != GetMMSPreviewOrPlay())
        {
            if(0 != s_preview_timer_id)
            {
                MMK_StopTimer(s_preview_timer_id);
                s_preview_timer_id=0;
            }
            
            if(0 != s_preview_duration_id)
            {
                MMK_StopTimer(s_preview_duration_id);
                s_preview_duration_id = 0;
            }

            s_preview_duration_time_sec = 0;
                
            MmsStopPreviewRing();
            // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
            MmsStopPreviewTTSReading();
            mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
            if (PNULL != mms_list_info_ptr)
            {
                if (   MMISMS_MT_TO_BE_READ == mms_list_info_ptr->mo_mt_type 
                    || MMISMS_MT_HAVE_READ == mms_list_info_ptr->mo_mt_type 
                    || MMISMS_MT_NOT_DOWNLOAD == mms_list_info_ptr->mo_mt_type
                    || MMISMS_MT_NOT_NOTIFY == mms_list_info_ptr->mo_mt_type)
                {
                    MMK_CreateWin((uint32 *)MMIMMS_INBOX_MMSOPTMENU_WIN_TAB,PNULL);
                }
                else if (MMISMS_MO_SEND_FAIL == mms_list_info_ptr->mo_mt_type)
                {
                    MMK_CreateWin((uint32 *)MMIMMS_OUTBOX_MENU_WIN_TAB,PNULL);
                }
                else if (MMISMS_MO_SEND_SUCC == mms_list_info_ptr->mo_mt_type)
                {
                    MMK_CreateWin((uint32 *)MMIMMS_SENTBOX_MMSMENU_WIN_TAB,PNULL);
                }
                else 
                {
                    //SCI_TRACE_LOW:"unkown mms type"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1910_112_2_18_2_38_38_298,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg : mms_list_info_ptr is null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1915_112_2_18_2_38_38_299,(uint8*)"");
            }
        }        
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef VIDEO_PLAYER_SUPPORT
        if (GetMMSPreviewIsPlayVideo())
        {
            MMIAPIVP_ExitPlay();
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
        }
        else
#endif    
        {
            MMK_CloseWin(win_id);
        }
        
        break;

    case MSG_APP_LEFT:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYLONG_STAR:
        if(!edit_doc_ptr)
        {
            break;
        }
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1945_112_2_18_2_38_38_300,(uint8*)"");
            break;
        }
        {   
            cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);

            //cr187107  ------ start ------
            if(cur_slide_index <= 1 && s_bCflc_prtc)
            {
                s_bCflc_prtc = FALSE;
#ifdef TTS_SUPPORT		
                cur_active_ctrl_id = MMISMS_GetCurActiveListCtrlId();
                total_item       = GUILIST_GetTotalItemNum(cur_active_ctrl_id);
                //+CR243394
                if (total_item>1)
                {
                    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
                    is_tts_played = FALSE;
                }
                //-CR243394
#endif        
                
                if (MSG_CTL_TITLE_MOVE_NEXT == msg_id)
                {
                    msg_id = MSG_APP_RIGHT;
                }
                else if (MSG_CTL_TITLE_MOVE_PREV == msg_id)
                {
                    msg_id = MSG_APP_LEFT;
                }
                
                // the MT SMS list box is opened                
                if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID) || MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
                {           
                    HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
                }
                break;
            }
            //cr187107  ------ end   ------			

            #ifdef MMI_PDA_SUPPORT
            GUIBUTTON_Update(MMIMMS_BUTTON_LF_CTRL_ID);            
            #endif
            if (cur_slide_index > 1)
            {
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id=0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }

                s_preview_duration_time_sec = 0;
                
                MmsStopPreviewRing();
                // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
                MmsStopPreviewTTSReading();
                if (MSG_KEYLONG_STAR == msg_id)
                {
                    MMIMMS_HeadSlide(edit_doc_ptr);
                }
                else
                {
                    slide_err = MMIMMS_PrevSlide(edit_doc_ptr);
                }
                
                if (MMIMMS_NO_ERROR == slide_err)
                {                    
#ifdef VIDEO_PLAYER_SUPPORT
                    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
                        && MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
                    {             
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                        uint32 video_type     = MMS_TYPE_MAX;   
                        VP_PLAY_PARAM_T play_param  = {0};
                        MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);

                        if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif                                
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);   
                        } 
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                        else
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        } 
#endif
                    }
                    else
#endif
                    {
                        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                    }                    
                }
            }
        }
        break;
    case MSG_APP_RIGHT: 
    case MSG_KEYDOWN_HASH:
        if(!edit_doc_ptr)
        {
            break;
        }
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_1999_112_2_18_2_38_38_301,(uint8*)"");
            break;
        }
        {  
            cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
            slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);

            //cr187107  ------ start ------
            if(cur_slide_index >= slide_count && s_bCflc_prtc)
            {
                s_bCflc_prtc = FALSE;
#ifdef TTS_SUPPORT		
                cur_active_ctrl_id = MMISMS_GetCurActiveListCtrlId();
                total_item       = GUILIST_GetTotalItemNum(cur_active_ctrl_id);
                //+CR243394
                if (total_item>1)
                {
                    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
                    is_tts_played = FALSE;
                }
                //-CR243394
#endif        
                
                if (MSG_CTL_TITLE_MOVE_NEXT == msg_id)
                {
                    msg_id = MSG_APP_RIGHT;
                }
                else if (MSG_CTL_TITLE_MOVE_PREV == msg_id)
                {
                    msg_id = MSG_APP_LEFT;
                }
                
                // the MT SMS list box is opened                
                if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID) || MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
                {           
                    HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
                }
                break;
            }
            //cr187107  ------ end   ------
            #ifdef MMI_PDA_SUPPORT
            GUIBUTTON_Update(MMIMMS_BUTTON_RH_CTRL_ID);            
            #endif
            if (cur_slide_index < slide_count)
            {
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id=0;
                }
                
                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }

                s_preview_duration_time_sec = 0;
                
                MmsStopPreviewRing();
                // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
                MmsStopPreviewTTSReading();
                slide_err = MMIMMS_NextSlide(edit_doc_ptr);
                if (MMIMMS_NO_ERROR == slide_err)
                {       
#ifdef VIDEO_PLAYER_SUPPORT                
                    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
                        && MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
                    {             
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                        uint32 video_type     = MMS_TYPE_MAX;   
                        VP_PLAY_PARAM_T play_param  = {0};
                        MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);

                        if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif                                
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);   
                        }  
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                        else
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        } 
#endif
                    }
                    else
#endif
                    {
                        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                    } 
                }
            }
        }               
        break;
    default:
        ret = MMI_RESULT_FALSE;
        break;
        
    }
    return ret;
}


#else
/*****************************************************************************/
//  Description :mms preview  handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewWinMsg(
                                           MMIMMS_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr                = PNULL;
    MMI_CTRL_ID_T richtext_ctrl_id                       = MMIMMS_PREVIEW_CTRL_ID;    
    static uint8 s_preview_timer_id                      = 0;                       //static变量记录定时器id    
    static uint8 s_preview_duration_id                   = 0;                       //播放时效timer
    static uint16 s_preview_duration_time_sec                   = 0;                       //播放时效计时
    uint32  duaration                                    = MMIMMS_SLIDE_DURATION_MIN * MMIMMS_SLIDE_TIME_DURING;      //当前帧播放间隔
    MMI_RESULT_E    ret                                  = MMI_RESULT_TRUE;         //返回值  
    uint16 cur_slide_index                               = 0;
    MMIMMS_ERROR_E   slide_err                           = MMIMMS_NO_ERROR;    
    uint16 slide_count                                   = 0;    
    MMISMS_ORDER_INFO_T *mms_list_info_ptr               = PNULL;
    uint32 vedio_time                                    = 0;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element            = {0};/*lint !e64*/
    wchar *focus_url_ptr                                 = PNULL;
    wchar *focus_num_ptr                                 = PNULL;
    uint8 *number_ptr                                    = PNULL;
    GUIRICHTEXT_FOCUS_TYPE_T focus_type                  = GUIRICHTEXT_FOCUS_NONE;
    uint16 num_len                                       = 0;
    CC_RESULT_E cc_result                                = CC_RESULT_SUCCESS;
    GUI_RECT_T  richtext_rect                            = {0};
    GUI_RECT_T      button_rect1                         = {0};
    GUI_RECT_T      button_rect2                         = {0};
    MMIMMS_WINDOW_ID_E quer_win_id                       = MMIMMS_INSERT_PIC_QUERY_WIN_ID;

    edit_doc_ptr = GetMMSReadEditDoc();    
    //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg msg_id=%x,cur_edoc=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2081_112_2_18_2_38_38_302,(uint8*)"dd",msg_id,edit_doc_ptr);
    switch(msg_id)
    {
    case MSG_LCD_SWITCH:
        MMIMMS_GetRichtextRect(&richtext_rect);
        MMIMMS_GetPagebarLbinRect(&button_rect1);
        MMIMMS_GetPagebarRbinRect(&button_rect2);
        GUIBUTTON_SetRect(MMIMMS_BUTTON_LF_CTRL_ID,&button_rect1);
        GUIBUTTON_SetRect(MMIMMS_BUTTON_RH_CTRL_ID,&button_rect2);
        GUIRICHTEXT_SetRect(richtext_ctrl_id,richtext_rect);
        break;
#ifdef VIDEO_PLAYER_SUPPORT
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
#ifndef MMI_MMS_ADJUST_VOLUME_SUPPORT
        if (!GetMMSPreviewIsPlayVideo())
        {
            ret = MMI_RESULT_FALSE;
        }
#else
	{
	     //for bug 72855
    	     OpenVolumePanel();
    	     ret = MMI_RESULT_FALSE;
	}
#endif
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
            if (GetMMSPreviewIsPlayVideo())
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2103_112_2_18_2_38_38_303,(uint8*)"");
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                MMIAPIVP_ExitPlay();                
                break;
            }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527    
#endif

#ifdef MMI_PDA_SUPPORT
    case MSG_PROMPTWIN_OK:
        {
                mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
                if (PNULL != mms_list_info_ptr)
                {
                    //delete the mms 
                    MMIMMS_DeleteMMSAndSaveList(mms_list_info_ptr->record_id);
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                    
#endif
                    MMK_CloseWin(win_id);
                    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
                    {
                        MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                    }
                } 
        }
                break;
    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(quer_win_id);
        break; 
        
#endif

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        {
#ifdef VIDEO_PLAYER_SUPPORT
            if (GetMMSPreviewIsPlayVideo())
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg MSG_CTL_PENOK playing video!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2140_112_2_18_2_38_38_304,(uint8*)"");
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE);
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                MMIAPIVP_ExitPlay();                
                break;
            }
#endif
#ifdef MMI_PDA_SUPPORT
            HandleMMSPreviewWinPenOkMsg(win_id,((MMI_NOTIFY_T*)param)->src_id,&s_preview_timer_id,&s_preview_duration_id);
#endif            
            if(MMIMMS_BUTTON_LF_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                msg_id = MSG_APP_LEFT;
                MMK_SendMsg(win_id,msg_id,param);
            }
            else if(MMIMMS_BUTTON_RH_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                msg_id = MSG_APP_RIGHT;
                MMK_SendMsg(win_id,msg_id,param);
            }  
            else if (MMIMMS_PREVIEW_CTRL_ID == ((MMI_NOTIFY_T*) param)->src_id)
            {
                msg_id = MSG_APP_WEB;
                MMK_SendMsg(win_id,msg_id,param);
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_OPEN_WINDOW://窗口打开消息
        s_bCflc_prtc = TRUE;
    #ifdef MMI_PDA_SUPPORT

    if(MMS_PLAY == GetMMSPreviewOrPlay())
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
        if (PNULL != mms_list_info_ptr)
        {
            if(MMISMS_MO_SEND_SUCC == mms_list_info_ptr->mo_mt_type)
            {
                MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_sendbox_play_form_button_info,
                sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                );
                MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_sendbox_play_form_button_info,
                sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                );
                
            }
            else if(MMISMS_MO_SEND_FAIL == mms_list_info_ptr->mo_mt_type)
            {
                MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_outbox_play_form_button_info,
                sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                );
                MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_outbox_play_form_button_info,
                sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                );                
            }
            else
            {
                MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_mtbox_play_form_button_info,
                sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                );
                MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                s_mtbox_play_form_button_info,
                sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                );
            }
        }
    }
    else
    {
        GUI_BG_T button_bg = {0};
        button_bg.bg_type = GUI_BG_IMG;
        MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
        s_play_form_button_info,
        sizeof(s_play_form_button_info)/sizeof(s_play_form_button_info[0])
        );
        MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
        s_play_form_button_info,
        sizeof(s_play_form_button_info)/sizeof(s_play_form_button_info[0])
        );       
        button_bg.img_id = IMAGE_MMS_PAUSE_PDA;
        GUIBUTTON_SetBg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);    
    }

        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        MMIMMS_SetFreq();
        MMIMMS_GetRichtextRect(&richtext_rect);
        MMIMMS_GetPagebarLbinRect(&button_rect1);
        MMIMMS_GetPagebarRbinRect(&button_rect2);
        GUIBUTTON_SetRect(MMIMMS_BUTTON_LF_CTRL_ID,&button_rect1);
        GUIBUTTON_SetRect(MMIMMS_BUTTON_RH_CTRL_ID,&button_rect2);
        GUIRICHTEXT_SetRect(richtext_ctrl_id,richtext_rect);
        if(edit_doc_ptr)
        {
            //初始化static变量            
            s_preview_timer_id=0;
            //设置背光灯常亮
            MMIDEFAULT_SetBackLight(TRUE);            
            MMIMMS_WinInit(EDIT_PREVIEW_WIN_ENTRY);
            SetMMSPreviewIsWebPlay(FALSE);
            //定位到第一帧
            MMIMMS_HeadSlide(edit_doc_ptr);
            MMIMMS_CLEAR_CLIENT(win_id);

            if (MMS_PLAY == GetMMSPreviewOrPlay())
            {
                s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
                GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
                MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL); 
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
#if 0 //def TTS_SUPPORT
                if(!MmsShowTTSKeyIfSuitable(win_id, FALSE))
#endif
                {
                    GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_COMMON_PLAY, STXT_RETURN, FALSE); 
                }
            }
            else
            {
                s_edit_mms_preview_state = MMIMMS_PREVIEW_PLAYING;
                GUIRICHTEXT_DisableClipBoard(richtext_ctrl_id);
                MMIDEFAULT_AllowTurnOffBackLight(FALSE);
#ifdef NANDBOOT_SUPPORT
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {                    
                    MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);
                }
                else
                {
                    GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,STXT_STOP, STXT_RETURN, FALSE);     
                    MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                } 
#else
#ifdef VIDEO_PLAYER_SUPPORT
                if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                {                    
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                    uint32 video_type     = MMS_TYPE_MAX;   
                    VP_PLAY_PARAM_T play_param  = {0};
                    MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);
                        
                    if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif
                    {
                        MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);
                    }
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                    else
                    {
                        GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_PAUSE, STXT_RETURN, FALSE);     
                        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                    }
#endif
                }
                else
#endif                    
                {
                    GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_PAUSE, STXT_RETURN, FALSE);     
                    MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                } 
#endif               
            }         
        }
        else
        {
            MMK_CloseWin(win_id);
        }        
        //GUIRICHTEXT_Update(richtext_ctrl_id);
        MMK_SetAtvCtrl(win_id, richtext_ctrl_id);
        GUIWIN_SetTitleTextScroll(win_id, TITLE_NOSCROLL_BY_ELLIPSIS);
        break;
    case MSG_CLOSE_WINDOW:
        {
            //恢复允许关闭背光灯
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);            
            //关闭定时器
            if(0 != s_preview_timer_id)
            {
                MMK_StopTimer(s_preview_timer_id);
                s_preview_timer_id=0;
            }

            if(0 != s_preview_duration_id)
            {
                MMK_StopTimer(s_preview_duration_id);
                s_preview_duration_id = 0;
            }

            s_preview_duration_time_sec = 0;
                
#ifdef VIDEO_PLAYER_SUPPORT
            if (GetMMSPreviewIsPlayVideo())
            {
                MMIAPIVP_ExitPlay();
            }
#endif
            // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
            MmsStopPreviewTTSReading();
            //关闭预览的铃声
            MmsStopPreviewRing();
            //退出预览模块,根据情况释放block,恢复背景音等
            MMIMMS_MMSPreviewExit();
            SetMMSPreviewIsWebPlay(FALSE);
            SetMMSPreviewIsPlayVideoLosefocus(FALSE);
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_NUMBER_POP_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_NUMBER_POP_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_URL_POP_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_URL_POP_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_INBOX_MMSOPT_MENU_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_INBOX_MMSOPT_MENU_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_SENTBOX_OPT_MENU_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_SENTBOX_OPT_MENU_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_OUTBOX_OPT_MENU_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_OUTBOX_OPT_MENU_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID) && !MMIMMS_GetPreviewMMSDetailFlag())
            {
                MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_SAVE_MULTIFILE_LIST_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_SAVE_MULTIFILE_LIST_WIN_ID);
            }
            if (MMK_IsOpenWin(MMIMMS_MULTIFILE_VIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_MULTIFILE_VIEW_WIN_ID);
            }
            MMIMMS_WinExit(EDIT_PREVIEW_WIN_ENTRY); 
            MMK_CloseWin(quer_win_id);
            MMIMMS_RestoreFreq();
        }
        break;    
    case MSG_FULL_PAINT:        
        //if(!MMK_IsFocusWin(win_id) && edit_doc_ptr)//失去焦点下的刷品将半屏之外的地方还原
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video! MSG_FULL_PAINT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2358_112_2_18_2_38_39_305,(uint8*)"");
            break;
        }
        MMIMMS_CLEAR_CLIENT(win_id);
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state &&  MMS_PLAY == GetMMSPreviewOrPlay())
        {                
            switch(richtext_element.type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
                break;
            case GUIRICHTEXT_FOCUS_NONE:
                {
#if 0 //def TTS_SUPPORT
                    if(!MmsShowTTSKeyIfSuitable(win_id, FALSE))
#endif 
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                }
                break;
            default:                        
                break;
            }               
        }// if(处于停止状态)
        if (MMS_PLAY == GetMMSPreviewOrPlay())
        {
            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
        }
        else
        {
            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
        }
        break;
        
    case MSG_MMS_PLAY_VIDEO:
        if (MmsPreviewPlayVideoItem(edit_doc_ptr))
        { 
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, FALSE);
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg play video"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2405_112_2_18_2_38_39_306,(uint8*)"");
        }
        else
        {
            //s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
            GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            if (MMS_PLAY == GetMMSPreviewOrPlay())
            {
                GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_PAUSE, STXT_RETURN, TRUE);
            }
            else
            {
                GUIWIN_SetSoftkeyTextId(win_id,STXT_OPTION,TXT_COMMON_PLAY, STXT_RETURN, TRUE);
            }
            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
        }

        break;

#ifdef VIDEO_PLAYER_SUPPORT
    case MSG_MMS_END_VIDEO:
        MMIAPIVP_ExitPlay();        
        GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,STXT_STOP, STXT_RETURN, TRUE);
        //MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL);
        //MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
        break;
#endif
    case MSG_MMS_PLAY_SLIDE:
        {
            if (GetMMSPreviewIsPlayVideo())
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video! MSG_MMS_PLAY_SLIDE"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2437_112_2_18_2_38_39_307,(uint8*)"");
                break;
            }
            //MMIMMS_CLEAR_CLIENT(win_id);
            GUIRICHTEXT_DeleteAllItems(richtext_ctrl_id);
            GUIRICHTEXT_ResetState(richtext_ctrl_id);
            if (MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state && FALSE == GetMMSPreviewIsPlayVideo())
            {
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id=0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }
                
                s_preview_duration_time_sec = 0;
                
                vedio_time = GetMMSPreviewPlayVideoTime();                

                duaration = edit_doc_ptr->editbody_ptr->slide_cur->times;

                if (vedio_time < duaration)
                {
                    duaration = duaration - (vedio_time/1000)*1000;
                    s_preview_duration_time_sec = (vedio_time/1000) + 1;  // continue time
                }
                if (duaration == 0)
                {
                    duaration = MMIMMS_SLIDE_PLAY_DEFAULT_TIME;
                }
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg vedio_time=%d,duaration=%d, music_time= %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2462_112_2_18_2_38_39_308,(uint8*)"ddd",vedio_time,duaration);
                s_preview_timer_id = MMK_CreateWinTimer(win_id,  duaration + 100, FALSE);//Add 100ms to show End timer
                s_preview_duration_id = MMK_CreateWinTimer(win_id, 1000, TRUE);
                MMSShowPreviewTime(win_id, &s_preview_duration_time_sec);
                
                if (0 != vedio_time)
                {
                    SetMMSPreviewPlayVideoTime(0);
                } 
            }
            if (MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state && MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
            {
                GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
            }
            else
            {
                GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
            }
            MMSPreviewPlayCurrentSlide(richtext_ctrl_id, edit_doc_ptr);
//            if (MMS_PLAY == GetMMSPreviewOrPlay())
//            {
//                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
//            }
//            else
//            {
//                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
//            }
            if (MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
            {
                MmsPreviewPlayMusicItem(edit_doc_ptr);
            }

            if (MMS_PLAY == GetMMSPreviewOrPlay())
            {
                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
            }
            else
            {
                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
            }
        }       
        break;
    case MSG_LOSE_FOCUS:
#ifdef VIDEO_PLAYER_SUPPORT
        if (GetMMSPreviewIsPlayVideo())
        {
            MMIAPIVP_ExitPlay();
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, FALSE); 
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
            SetMMSPreviewIsPlayVideoLosefocus(TRUE);
        }
#endif
        if(MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
        {
            //get focus的时候要恢复播放
            s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
            GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
            MMIDEFAULT_AllowTurnOffBackLight(TRUE);
            if (MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
            {
                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL); 
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
            }
        }
        //关闭定时器
        if(0 != s_preview_timer_id)
        {
            MMK_StopTimer(s_preview_timer_id);
            s_preview_timer_id = 0;
        }

        if(0 != s_preview_duration_id)
        {
            MMK_StopTimer(s_preview_duration_id);
            s_preview_duration_id = 0;
        }
        
        s_preview_duration_time_sec = 0;
        GUIWIN_SetTitleSubText(win_id, FALSE, PNULL, 0, FALSE);
        
        // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
        MmsStopPreviewTTSReading();
        MmsStopPreviewRing();
        break;
    case MSG_GET_FOCUS:
        //防止焦点切换后，会锁屏 
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video! MSG_GET_FOCUS"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2529_112_2_18_2_38_39_309,(uint8*)"");
            break;
        }
        MMIMMS_CLEAR_CLIENT(win_id);
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        if(edit_doc_ptr)
        {
            if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state)
            {                
                switch(richtext_element.type) 
                {
                case GUIRICHTEXT_FOCUS_PHONENUM:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_URL:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_EMAIL:
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
                    break;
                case GUIRICHTEXT_FOCUS_NONE:
                    if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                    else
                    {
#if 0 //def TTS_SUPPORT
                        if(!MmsShowTTSKeyIfSuitable(win_id, FALSE))
#endif 
                        {
                            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }
                    }
                    break;
                default:                        
                    break;
                }               
            }// if(处于停止状态)
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            //如果字体不一样，在losefocus的状态发生了改变，重新画richtext
            if (MMIMMS_GetPreFont() != MMIAPISMS_GetWinFont())
            {
                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            }
#endif
            if (MMS_PLAY == GetMMSPreviewOrPlay())
            {
                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
            }
            else
            {
                MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
            }
        }
        break;
    case MSG_TIMER:
        if(s_preview_timer_id == *((uint8 *) param) && edit_doc_ptr)
        {
            if (GetMMSPreviewIsPlayVideo())
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2590_112_2_18_2_38_39_310,(uint8*)"");
                break;
            }
            if(MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
            {
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id=0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }
                
                s_preview_duration_time_sec = 0;
                
                MmsStopPreviewRing();
                // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
                MmsStopPreviewTTSReading();
                if(MMK_IsFocusWin(win_id))
                {
                    cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
                    slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);
                    
                    if(slide_count == cur_slide_index)
                    {         
                        if (1 < slide_count)
                        {
                            MMIMMS_HeadSlide(edit_doc_ptr);
                        }
                        if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                        {
                            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }
#if 1 //ndef TTS_SUPPORT
                        else
                        {
                            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }
#endif
                        //更新工具栏
                        if (MMS_PLAY == GetMMSPreviewOrPlay())
                        {
                            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,TRUE);
                        }
                        else
                        {
                            MMIMMS_DrawPageBarText(win_id, edit_doc_ptr,FALSE);
                        } 
                        s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
                        GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
                        MMIDEFAULT_AllowTurnOffBackLight(TRUE);

                        GUIWIN_SetTitleSubText(win_id, FALSE, PNULL, 0, FALSE);
                        GUIWIN_UpdateTitle(win_id);
                        
                        if (MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                            GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
                        }
                        else if (1 < slide_count)
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        }
                    }// if(最后一帧)
                    else
                    {
                        //定位到下一帧
                        MMIMMS_NextSlide(edit_doc_ptr);

                        if(0 != s_preview_timer_id)
                        {
                            MMK_StopTimer(s_preview_timer_id);
                            s_preview_timer_id = 0;
                        }                        

                        if(0 != s_preview_duration_id)
                        {
                            MMK_StopTimer(s_preview_duration_id);
                            s_preview_duration_id = 0;
                        }
                        
                        s_preview_duration_time_sec = 0;
                        //MMSShowPreviewTime(win_id,&s_preview_duration_time_sec);
#ifdef NANDBOOT_SUPPORT
                        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);                           
                        }
                        else
                        { 

                            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_STOP, STXT_RETURN, TRUE);
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        }
#else
#ifdef VIDEO_PLAYER_SUPPORT
                        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
                        {
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                            uint32 video_type     = MMS_TYPE_MAX;   
                            VP_PLAY_PARAM_T play_param  = {0};
                            MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);
                        
                            if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif
                            {
                                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                                MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);                           
                            }
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                            else
                            {
                                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_STOP, STXT_RETURN, TRUE);
                                MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                            }
#endif
                        }
                        else
#endif                            
                        { 

                            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_STOP, STXT_RETURN, TRUE);
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        }
#endif
                       
                        
                    }
                }//focus in 
                else
                {
                    s_edit_mms_preview_state = MMIMMS_PREVIEW_STOP;
                    GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
                    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                    if (MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
                    {
                        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL); 
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
                    }
                    if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                    else
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                    
                }
            }//if 播放状态
            else
            {
                //SCI_TRACE_LOW:"timer:playing state!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2705_112_2_18_2_38_39_311,(uint8*)"");
            }
        }
        else if(s_preview_duration_id == *((uint8 *) param) && edit_doc_ptr)
        {
            MMSShowPreviewTime(win_id,&s_preview_duration_time_sec);
        }

        break;
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video! MSG_CTL_RICHTEXT_SWITCHFOCUS"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2713_112_2_18_2_38_39_312,(uint8*)"");
            break;
        }
        GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
        focus_type = richtext_element.type;
        switch(focus_type) 
        {
        case GUIRICHTEXT_FOCUS_PHONENUM:
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, TRUE);
        	break;
        case GUIRICHTEXT_FOCUS_URL:
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, TRUE);
            break;
        case GUIRICHTEXT_FOCUS_EMAIL:
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, TRUE);
            break;
        case GUIRICHTEXT_FOCUS_NONE:
            if (MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state)
            {
                if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                {
                    GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                }
                else
                {
#ifdef TTS_SUPPORT
                    if(!MmsShowTTSKeyIfSuitable(win_id, TRUE))
#endif 
                    {
                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                    }
                }
            }
            break;
        default:                        
            break;
        }
        break;

        case MSG_KEYUP_GREEN: // same as MSG_KEYUP_CALL1
        case MSG_KEYUP_CALL2:
        case MSG_KEYUP_CALL3:
        case MSG_KEYUP_CALL4:
            if (edit_doc_ptr)
            {
#ifndef MMI_MULTI_GREEN_KEY_SINGLE
                CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType( msg_id, CC_CALL_SIM_MAX);
#endif
                
                GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
                focus_type = richtext_element.type;
                mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
                if (GUIRICHTEXT_FOCUS_PHONENUM == focus_type)
                {
                    if (PNULL == mms_list_info_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg 1 mms_list_info_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2769_112_2_18_2_38_40_313,(uint8*)"");
                        break;
                    }
                    focus_num_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
                    if (PNULL == focus_num_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2775_112_2_18_2_38_40_314,(uint8*)"");
                        break;
                    }
                    //SCI_MEMSET(focus_num_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
                    number_ptr = SCI_ALLOCAZ(richtext_element.data.len + 1);
                    if (PNULL == number_ptr)
                    {
                        SCI_Free(focus_num_ptr);
                        focus_num_ptr = PNULL;
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg number_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2785_112_2_18_2_38_40_315,(uint8*)"");
                        break;
                    }
                    SCI_MEMSET(number_ptr, 0, (richtext_element.data.len + 1));
                    MMI_WSTRNTOSTR((uint8 *)number_ptr, richtext_element.data.len, focus_num_ptr, richtext_element.data.len, richtext_element.data.len);
                    num_len = SCI_STRLEN((char *)number_ptr);                   
                    
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
                    cc_result = MMIAPICC_MakeCall(
                        mms_list_info_ptr->dual_sys,
                        number_ptr,
                        num_len,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
#else
                    cc_result = MMIAPICC_MakeCall(
                        mms_list_info_ptr->dual_sys,
                        number_ptr,
                        num_len,
                        PNULL,
                        PNULL,
                        sim_type,
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
#endif
                    
                    SCI_Free(focus_num_ptr);
                    focus_num_ptr = PNULL;
                    SCI_Free(number_ptr);
                    number_ptr = PNULL;
                }
                else
                {
                    if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state &&  MMS_PLAY == GetMMSPreviewOrPlay())
                    {
                        if (PNULL == mms_list_info_ptr)
                        {
                            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg 2 mms_list_info_ptr == PNULL!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2827_112_2_18_2_38_40_316,(uint8*)"");
                            break;
                        }
                        
                        num_len = SCI_STRLEN((char *)mms_list_info_ptr->sender);
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
                        cc_result = MMIAPICC_MakeCall(
                            mms_list_info_ptr->dual_sys,
                            mms_list_info_ptr->sender,
                            num_len,
                            PNULL,
                            PNULL,
                            CC_CALL_SIM_MAX,
                            CC_CALL_NORMAL_CALL,
                            PNULL
                            );
#else
                        cc_result = MMIAPICC_MakeCall(
                            mms_list_info_ptr->dual_sys,
                            mms_list_info_ptr->sender,
                            num_len,
                            PNULL,
                            PNULL,
                            sim_type,
                            CC_CALL_NORMAL_CALL,
                            PNULL
                            );
#endif
                    }
                }
            }
            break;

    case MSG_SK_SKB_ICON_PRESSED:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:        
        if(edit_doc_ptr)
        {
            GUIRICHTEXT_GetFocusElement(richtext_ctrl_id, &richtext_element);
#ifdef VIDEO_PLAYER_SUPPORT
            if (GetMMSPreviewIsPlayVideo())
            {
                MMIAPIVP_ExitPlay();
                GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE); 
                MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
            }
            else
#endif
            {
                focus_type = richtext_element.type;
                if (GetMMSPreviewIsWebPlay())
                {
                    focus_type = GUIRICHTEXT_FOCUS_NONE;
                    SetMMSPreviewIsWebPlay(FALSE);
                }
                MmsStopPreviewRing();
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id = 0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }
                
                s_preview_duration_time_sec = 0;
                GUIWIN_SetTitleSubText(win_id, FALSE, PNULL, 0, FALSE);
                GUIWIN_UpdateTitle(win_id);
                
                if (GUIRICHTEXT_FOCUS_PHONENUM == focus_type)
                {
                    focus_num_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
                    if (PNULL == focus_num_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2893_112_2_18_2_38_40_317,(uint8*)"");
                        break;
                    }
                    //SCI_MEMSET(focus_num_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
                    number_ptr = SCI_ALLOC_APPZ(richtext_element.data.len + 1);
                    //SCI_MEMSET(number_ptr, 0, (richtext_element.data.len + 1));
                    MMI_WSTRNTOSTR((uint8 *)number_ptr, richtext_element.data.len, focus_num_ptr, richtext_element.data.len, richtext_element.data.len);
                    MMK_CreateWin((uint32 *)MMIMMS_PREVIEW_NUMBER_POP_WIN_TAB, (ADD_DATA)number_ptr);
                    SCI_Free(focus_num_ptr);
                    focus_num_ptr = PNULL;
                }
                else if(GUIRICHTEXT_FOCUS_URL == focus_type)
                {
                    focus_url_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
                    if (PNULL == focus_url_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg focus_url_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2910_112_2_18_2_38_40_318,(uint8*)"");
                        break;
                    }
                    //SCI_MEMSET(focus_url_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(focus_url_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
                    MMK_CreateWin((uint32 *)MMIMMS_PREVIEW_URL_POP_WIN_TAB,(ADD_DATA)focus_url_ptr);
                }
                else if (GUIRICHTEXT_FOCUS_EMAIL == focus_type)
                {
                    focus_num_ptr = SCI_ALLOCAZ((richtext_element.data.len + 1)*sizeof(wchar));
                    if (PNULL == focus_num_ptr)
                    {
                        //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg focus_num_ptr == PNULL!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_2922_112_2_18_2_38_40_319,(uint8*)"");
                        break;
                    }
                    //SCI_MEMSET(focus_num_ptr, 0, ((richtext_element.data.len + 1)*sizeof(wchar)));
                    MMIAPICOM_Wstrncpy(focus_num_ptr,richtext_element.data.str_ptr,richtext_element.data.len);
                    number_ptr = SCI_ALLOC_APPZ(richtext_element.data.len + 1);
                    //SCI_MEMSET(number_ptr, 0, (richtext_element.data.len + 1));
                    MMI_WSTRNTOSTR((uint8 *)number_ptr, richtext_element.data.len, focus_num_ptr, richtext_element.data.len, richtext_element.data.len);
                    MMK_CreateWin((uint32 *)MMIMMS_PREVIEW_EMAIL_POP_WIN_TAB, (ADD_DATA)number_ptr);
                    SCI_Free(focus_num_ptr);
                    focus_num_ptr = PNULL;
                }
                else if (GUIRICHTEXT_FOCUS_NONE == focus_type)
                {
                    if(MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
                    {                     
                        if(MMS_PREVIEW == GetMMSPreviewOrPlay())//处于预览状态
                        {
                            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }
                        else if (MMS_PLAY == GetMMSPreviewOrPlay())
                        {
                            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_PLAY, STXT_RETURN, TRUE);
                        }

                        s_edit_mms_preview_state=MMIMMS_PREVIEW_STOP;
                        GUIRICHTEXT_EnableClipBoard(richtext_ctrl_id);
                        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                        if ( MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
                        {
                            //MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL, PNULL); 
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                            GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
                        }
                    }// if(播放状态)
                    else if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state)
                    {
#if 0 //def TTS_SUPPORT
                        if(!MmsIsSuitableForTTS())
                        {
                            if(!BoxPlayMMS(edit_doc_ptr))
                            {
                                break;
                            }
                        }
                        else
                        {
                            // 如果TTS正在运行
                            if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_MMS))
                            {
                                MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_MMS);
                                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_SET_TTS_PLAY, STXT_RETURN, TRUE);
                            }
                            else
                            {
                                // 获取MMS的文本内容...
                                MMI_STRING_T    mms_text_string = {0};
                                MMITTS_PLAY_PARAM_T play_param = {0};
                                if (MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_text_string))
                                {
                                    play_param.txt_addr = (uint8 *)mms_text_string.wstr_ptr;
                                    play_param.txt_len_in_byte = mms_text_string.wstr_len * 2;
                                    play_param.is_ucs2 = TRUE;
                                    play_param.play_times = 1;
                                    play_param.tts_vol = 4;
                                    play_param.ring_type = TTS_RING_TYPE_MMS;
                                    play_param.win_handle = MMIMMS_PREVIEW_WIN_ID;
									play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
									play_param.digit_mode = TTS_DIGIT_AUTO;
									play_param.punc_mode = TTS_PUNC_OFF;
                                    if(MMIAPITTS_PlayText(&play_param))
                                    {
                                        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_STOP, STXT_RETURN, TRUE);
                                    }
                                }
                            }
                        }
#else
                        {
						    // for bug 72807 
                            wchar music_name_wstr[MAX_NAME_LENGTH + 1] = {0};
                            uint16 music_name_wlen                 = 0;
                            uint8 *music_name_ptr                  = PNULL;
                            uint8 music_name_len                   = 0;
                            wchar suffix_name[MAX_NAME_LENGTH + 1] = {0};
                            uint16 suffix_len = MMIMMS_SUFFIX_LEN;
                            
                            music_name_ptr = (uint8 *)edit_doc_ptr->editbody_ptr->slide_cur->audiofile;
                            music_name_len = (uint8)strlen((char *)music_name_ptr);
                            music_name_wlen = GUI_UTF8ToWstr(music_name_wstr, music_name_len, music_name_ptr, music_name_len);	

                            if (MMIAPIFMM_SplitFileName(music_name_wstr, MMIAPICOM_Wstrlen(music_name_wstr),
														PNULL, PNULL,
														suffix_name, &suffix_len))
                            {								
                                if(MMIFMM_UNKNOW_TYPE == MMIAPIFMM_GetFileType(suffix_name, suffix_len))
                                {
                                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
                                    break;
                                }
                            }
                        }
                        if(!BoxPlayMMS(edit_doc_ptr))
                        {
                            break;
                        }
#endif
                    }//else if(s_edit_mms_preview_state==MMIMMS_PREVIEW_STOP)
                    else
                    {
                        //SCI_TRACE_LOW:"s_edit_mms_preview_state=%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3005_112_2_18_2_38_40_320,(uint8*)"d",s_edit_mms_preview_state);
                    }
                } 
                else
                {
                    //SCI_TRACE_LOW:"focus item type error!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3010_112_2_18_2_38_40_321,(uint8*)"");
                }
            }            
        }            
        break;

#ifdef TTS_SUPPORT		
    case MSG_TTS_OVER:
        MmsShowTTSKeyIfSuitable(win_id, TRUE);
        break;
#endif

    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_MENU:
        if(!edit_doc_ptr)
        {
            break;
        }        
        //左软键进选项菜单
#ifdef VIDEO_PLAYER_SUPPORT
        if (GetMMSPreviewIsPlayVideo())
        {
            MMIAPIVP_ExitPlay();
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
        }
#endif        
        if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state && MMS_PREVIEW != GetMMSPreviewOrPlay())
        {
            if(0 != s_preview_timer_id)
            {
                MMK_StopTimer(s_preview_timer_id);
                s_preview_timer_id=0;
            }
            
            if(0 != s_preview_duration_id)
            {
                MMK_StopTimer(s_preview_duration_id);
                s_preview_duration_id = 0;
            }
            
            s_preview_duration_time_sec = 0;
                
            MmsStopPreviewRing();
            // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
            MmsStopPreviewTTSReading();
            mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
            if (PNULL != mms_list_info_ptr)
            {
                if (   MMISMS_MT_TO_BE_READ == mms_list_info_ptr->mo_mt_type 
                    || MMISMS_MT_HAVE_READ == mms_list_info_ptr->mo_mt_type 
                    || MMISMS_MT_NOT_DOWNLOAD == mms_list_info_ptr->mo_mt_type
                    || MMISMS_MT_NOT_NOTIFY == mms_list_info_ptr->mo_mt_type)
                {
                    MMK_CreateWin((uint32 *)MMIMMS_INBOX_MMSOPTMENU_WIN_TAB,PNULL);
                }
                else if (MMISMS_MO_SEND_FAIL == mms_list_info_ptr->mo_mt_type)
                {
                    MMK_CreateWin((uint32 *)MMIMMS_OUTBOX_MENU_WIN_TAB,PNULL);
                }
                else if (MMISMS_MO_SEND_SUCC == mms_list_info_ptr->mo_mt_type)
                {
                    MMK_CreateWin((uint32 *)MMIMMS_SENTBOX_MMSMENU_WIN_TAB,PNULL);
                }
                else 
                {
                    //SCI_TRACE_LOW:"unkown mms type"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3068_112_2_18_2_38_40_322,(uint8*)"");
                }
            }
            else
            {
                //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg : mms_list_info_ptr is null"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3073_112_2_18_2_38_40_323,(uint8*)"");
            }
        }        
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
#ifdef VIDEO_PLAYER_SUPPORT
        if (GetMMSPreviewIsPlayVideo())
        {
            MMIAPIVP_ExitPlay();
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_PAUSE,STXT_RETURN, TRUE);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
        }
        else
#endif    
        {
            MMK_CloseWin(win_id);
        }
        
        break;

    case MSG_APP_LEFT:
    case MSG_KEYDOWN_STAR:
    case MSG_KEYLONG_STAR:
        if(!edit_doc_ptr)
        {
            break;
        }
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3103_112_2_18_2_38_40_324,(uint8*)"");
            break;
        }
        {   
            cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);

            //cr187107  ------ start ------
            if(cur_slide_index <= 1 && s_bCflc_prtc)
            {
                s_bCflc_prtc = FALSE;
#ifdef TTS_SUPPORT		
                cur_active_ctrl_id = MMISMS_GetCurActiveListCtrlId();
                total_item       = GUILIST_GetTotalItemNum(cur_active_ctrl_id);
                //+CR243394
                if (total_item>1)
                {
                    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
                    is_tts_played = FALSE;
                }
                //-CR243394
#endif        
                
                if (MSG_CTL_TITLE_MOVE_NEXT == msg_id)
                {
                    msg_id = MSG_APP_RIGHT;
                }
                else if (MSG_CTL_TITLE_MOVE_PREV == msg_id)
                {
                    msg_id = MSG_APP_LEFT;
                }
                
                // the MT SMS list box is opened                
                if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID) || MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
                {           
                    HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
                }
                break;
            }
            //cr187107  ------ end   ------		

            if (cur_slide_index > 1)
            {
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id=0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }
                
                s_preview_duration_time_sec = 0;
                
                MmsStopPreviewRing();
                // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
                MmsStopPreviewTTSReading();
                if (MSG_KEYLONG_STAR == msg_id)
                {
                    MMIMMS_HeadSlide(edit_doc_ptr);
                }
                else
                {
                    slide_err = MMIMMS_PrevSlide(edit_doc_ptr);
                }
                
                if (MMIMMS_NO_ERROR == slide_err)
                {                    
#ifdef VIDEO_PLAYER_SUPPORT
                    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
                        && MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
                    {         
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                        uint32 video_type     = MMS_TYPE_MAX;   
                        VP_PLAY_PARAM_T play_param  = {0};
                        MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);
                        
                        if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);  
                        }
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                        else
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        }
#endif
                    }
                    else
#endif                        
                    {
                        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                    }                    
                }
            }
        }
        break;
    case MSG_APP_RIGHT: 
    case MSG_KEYDOWN_HASH:
        if(!edit_doc_ptr)
        {
            break;
        }
        if (GetMMSPreviewIsPlayVideo())
        {
            //SCI_TRACE_LOW:"HandleMMSPreviewWinMsg playing video!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3154_112_2_18_2_38_40_325,(uint8*)"");
            break;
        }
        {  
            cur_slide_index = MMIMMS_GetSlideIndex(edit_doc_ptr);
            slide_count = MMIMMS_GetSlideCount(edit_doc_ptr);

            //cr187107  ------ start ------
            if(cur_slide_index >= slide_count && s_bCflc_prtc)
            {
                s_bCflc_prtc = FALSE;
#ifdef TTS_SUPPORT		
                cur_active_ctrl_id = MMISMS_GetCurActiveListCtrlId();
                total_item       = GUILIST_GetTotalItemNum(cur_active_ctrl_id);
                //+CR243394
                if (total_item>1)
                {
                    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_SMS);
                    is_tts_played = FALSE;
                }
                //-CR243394
#endif        
                
                if (MSG_CTL_TITLE_MOVE_NEXT == msg_id)
                {
                    msg_id = MSG_APP_RIGHT;
                }
                else if (MSG_CTL_TITLE_MOVE_PREV == msg_id)
                {
                    msg_id = MSG_APP_LEFT;
                }
                
                // the MT SMS list box is opened                
                if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID) || MMK_IsOpenWin(MMISMS_SECURITYBOX_CHILD_WIN_ID))
                {           
                    HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
                }
                break;
            }
            //cr187107  ------ end   ------

            if (cur_slide_index < slide_count)
            {
                if(0 != s_preview_timer_id)
                {
                    MMK_StopTimer(s_preview_timer_id);
                    s_preview_timer_id=0;
                }

                if(0 != s_preview_duration_id)
                {
                    MMK_StopTimer(s_preview_duration_id);
                    s_preview_duration_id = 0;
                }
                
                s_preview_duration_time_sec = 0;
                
                MmsStopPreviewRing();
                // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
                MmsStopPreviewTTSReading();
                slide_err = MMIMMS_NextSlide(edit_doc_ptr);
                if (MMIMMS_NO_ERROR == slide_err)
                {                    
#ifdef VIDEO_PLAYER_SUPPORT
                    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag 
                        && MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
                    {
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                        uint32 video_type     = MMS_TYPE_MAX;   
                        VP_PLAY_PARAM_T play_param  = {0};
                        MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);
                        
                        if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);  
                        }
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
                        else
                        {
                            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                        }
#endif
                    } 
                    else
#endif                        
                    {
                        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
                        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                    }
                }
            }
        }               
        break;
    default:
        ret = MMI_RESULT_FALSE;
        break;
        
    }
    return ret;
}
#endif

/*****************************************************************************/
//  Description :show preview time
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSShowPreviewTime(MMIMMS_WINDOW_ID_E win_id,uint16 *duration_time)
{
    char str[MMIMMS_PREVIEW_TIME_LEN + 1] = {0};
    wchar wstr[MMIMMS_PREVIEW_TIME_LEN + 1] = {0};
    if (GetMMSPreviewIsPlayVideo())
    {
        return;
    }    
    _snprintf(str, MMIMMS_PREVIEW_TIME_LEN + 1, "%ds",*duration_time);
    MMI_STRNTOWSTR(wstr, MMIMMS_PREVIEW_TIME_LEN, (uint8*)str, MMIMMS_PREVIEW_TIME_LEN, strlen(str));
    GUIWIN_SetTitleSubText(win_id,TRUE,wstr,MMIAPICOM_Wstrlen(wstr),FALSE);

    GUIWIN_UpdateTitle(win_id);

    *duration_time = *duration_time + 1;
}

/*****************************************************************************/
//  Description :mms preview get number handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewNumberPopWinMsg(
                                                    MMIMMS_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr  = PNULL;
    MMI_RESULT_E                 err_code  = MMI_RESULT_TRUE;
    uint32     group_id                    = 0;
    uint32   menu_id                       = 0;
    MMI_CTRL_ID_T menu_ctrl_id             = MMIMMS_PREVIEW_NUMBER_POP_CTRL_ID;
    uint8 *sender_ptr                      = ( uint8 * )MMK_GetWinAddDataPtr(win_id);
    uint16 num_len                         = 0;
    CC_RESULT_E cc_result                  = CC_RESULT_SUCCESS;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject            = {0};
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    
    edit_doc_ptr = GetMMSReadEditDoc();
    if (PNULL != edit_doc_ptr)
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
    }
    
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3229_112_2_18_2_38_41_326,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //弹出popmenu
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入文件管理器界面选择文件
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        if (PNULL == sender_ptr || PNULL == mms_list_info_ptr)
        {
            break;
        }
        switch (menu_id)
        {
        case ID_MMS_DIAL:
        case ID_MMS_IP_DIAL:            
        case ID_MMS_VIDEOPHONE_DAIL:
            num_len = SCI_STRLEN((char *)sender_ptr);
            if(ID_MMS_DIAL == menu_id)
            {                        
                cc_result = MMIAPICC_MakeCall(
                    mms_list_info_ptr->dual_sys,
                    sender_ptr,
                    num_len,
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_NORMAL_CALL,
                    PNULL
                    );
            }
#ifdef VT_SUPPORT
            else if (ID_MMS_VIDEOPHONE_DAIL == menu_id)
            {
                cc_result = MMIAPICC_MakeCall(
                    mms_list_info_ptr->dual_sys,
                    sender_ptr,
                    num_len,
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_VIDEO_CALL,
                    PNULL
                    );
            }
#endif
#ifdef MMI_IP_CALL_SUPPORT
            else
            {
                cc_result = MMIAPICC_MakeCall(
                    mms_list_info_ptr->dual_sys,
                    sender_ptr,
                    num_len,
                    PNULL,
                    PNULL,
                    CC_CALL_SIM_MAX,
                    CC_CALL_IP_CALL,
                    PNULL
                    );
            }
#endif
            break;
        case ID_MMS_ANSWER_SMS:
            MMIAPISMS_AnswerMessage(mms_list_info_ptr->dual_sys, PNULL, sender_ptr, 
                (uint8)(strlen((char *)sender_ptr) > MMIMMS_MAX_ADDRESS_LEN? MMIMMS_MAX_ADDRESS_LEN:strlen((char *)sender_ptr)));
            break;
        case ID_MMS_ANSWER_MMS:
            {
                num_len = strlen((char *)sender_ptr);
                //answer mms
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }   
                MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_READ_NUMBER_SEND);
                if(num_len > 0)
                {
                    num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APPZ(num_len * sizeof(wchar));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (uint8 *)sender_ptr, num_len, num_len);
                    string_to.wstr_len = num_len;
                    MMIAPIMMS_GetSubjectById(mms_list_info_ptr->record_id, &string_subject);
                    MMIAPIMMS_AnswerMMS(mms_list_info_ptr->dual_sys,&string_subject,&string_to);
                    SCI_FREE(string_to.wstr_ptr);
                    string_to.wstr_ptr = PNULL;
                    
                }                
            }
            break;
        case ID_MMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                // save to phonebook    
                MMIAPICOM_GenPartyNumber(sender_ptr,(uint16)SCI_STRLEN((char *)sender_ptr), &party_num);
                
                pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );
                MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
                //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_MMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                // save to phonebook    
                MMIAPICOM_GenPartyNumber(sender_ptr,(uint16)SCI_STRLEN((char *)sender_ptr), &party_num);
                
                pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );
                //MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
                MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
            }
            break;
#endif
        default :
            break;
        }        
        MMK_CloseWin(win_id);
        break; 

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :mms preview get url handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewUrlPopWinMsg(
                                                    MMIMMS_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
//  MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr = PNULL;
    MMI_RESULT_E                 err_code = MMI_RESULT_TRUE;
    uint32     group_id                   = 0;
    uint32   menu_id                      = 0;
    MMI_CTRL_ID_T menu_ctrl_id            = MMIMMS_PREVIEW_URL_POP_CTRL_ID;
    wchar  *url_winparam_ptr              = ( wchar *)MMK_GetWinAddDataPtr(win_id);

    //SCI_TRACE_LOW:"MMIMMS: HandleMMSEditMultiPopWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3395_112_2_18_2_38_41_327,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //弹出popmenu
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        if (MMIAPIDC_IsOpened())
        {
            GUIMENU_SetItemGrayed(MMIMMS_PREVIEW_URL_POP_CTRL_ID, MENU_MMS_URL_POP_MENU, ID_MMS_PREVIEW_URL_ENTER, TRUE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIMMS_PREVIEW_URL_POP_CTRL_ID, MENU_MMS_URL_POP_MENU, ID_MMS_PREVIEW_URL_ENTER, FALSE);
        }
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入文件管理器界面选择文件
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        if(PNULL == url_winparam_ptr)
        {
            break;
        }
        switch (menu_id)
        {
        case ID_MMS_PREVIEW_URL_ENTER:
            {
                uint8 *url_ptr = PNULL;
                uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);

                url_ptr = SCI_ALLOCAZ(str_len * sizeof(uint8));
                if(PNULL == url_ptr)
                {
                    //SCI_TRACE_LOW:"HandleMMSPreviewUrlPopWinMsg alloc failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3433_112_2_18_2_38_41_328,(uint8*)"");
                    break;
                }
                
                GUI_WstrToUTF8(url_ptr, str_len,url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));

#ifndef NANDBOOT_SUPPORT
                if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))//nor手机打开浏览器的时候关闭彩信阅读
                {
                    MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                }
#endif
                
#ifdef BROWSER_SUPPORT
                {
                    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
                    
                    entry_param.type = MMIBROWSER_ACCESS_URL;
                    entry_param.dual_sys = MN_DUAL_SYS_MAX;
                    entry_param.url_ptr = (char *)url_ptr;
                    entry_param.user_agent_ptr = PNULL;
#ifdef CMCC_UI_STYLE
                    entry_param.is_alert= TRUE;
#endif
                    MMIAPIBROWSER_Entry(&entry_param);
                }
#endif
                
                SCI_FREE(url_ptr);
                url_ptr = PNULL;
            }
            MMK_CloseWin(win_id);

            break;
        case ID_MMS_PREVIEW_URL_ADD_TO_BOOKMARK:
            {
                uint8 *url_ptr = PNULL;
                uint16 str_len = (uint16)(MMIAPICOM_Wstrlen(url_winparam_ptr) * 3 + 1);
                
                url_ptr = SCI_ALLOCAZ(str_len * sizeof(uint8));
                if(PNULL == url_ptr)
                {
                    //SCI_TRACE_LOW:"HandleMMSPreviewUrlPopWinMsg BOOKMARK alloc failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3470_112_2_18_2_38_41_329,(uint8*)"");
                    break;
                }
                
                //SCI_MEMSET(url_ptr, 0x00, (str_len * sizeof(uint8)));
                
                GUI_WstrToUTF8(url_ptr, str_len,url_winparam_ptr, MMIAPICOM_Wstrlen(url_winparam_ptr));
#ifndef NANDBOOT_SUPPORT
                if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))//nor手机打开浏览器的时候关闭彩信阅读
                {
                    MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                }
#endif                
#ifdef BROWSER_SUPPORT
                {
                    MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
                    
                    entry_param.type = MMIBROWSER_ADD_BOOKMARK;
                    entry_param.dual_sys = MN_DUAL_SYS_MAX;
                    entry_param.url_ptr = (char *)url_ptr;
                    entry_param.user_agent_ptr = PNULL;
                    MMIAPIBROWSER_Entry(&entry_param);
                }                
#endif
                SCI_FREE(url_ptr);
                url_ptr = PNULL;
            }
            MMK_CloseWin(win_id);
            break;
#ifndef PB_SUPPORT_LOW_MEMORY

        case ID_MMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                MMIPB_CUSTOM_TEXT_T url_text = {0};
                url_text.wstr_len = MMIAPICOM_Wstrlen(url_winparam_ptr);
                url_text.wstr_len = MIN( url_text.wstr_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                MMIAPICOM_Wstrncpy(url_text.wstr, url_winparam_ptr, url_text.wstr_len);
                MMIAPIPB_AddContactWin(&url_text, PB_GROUP_ALL, MMIPB_ADD_TYPE_URL);
            }
            MMK_CloseWin(win_id);
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_MMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                MMIPB_CUSTOM_TEXT_T url_text = {0};
                
                url_text.wstr_len = MMIAPICOM_Wstrlen(url_winparam_ptr);
                url_text.wstr_len = MIN(url_text.wstr_len, MMIPB_MAX_CUSTOM_TEXT_LEN);
                MMIAPICOM_Wstrncpy(url_text.wstr, url_winparam_ptr, url_text.wstr_len);
                //MMIAPIPB_OpenEntryListForEx(&url_text, MMIPB_ADD_TYPE_URL);
                MMIAPIPB_AddToExistContact(&url_text, MMIPB_ADD_TYPE_URL, PNULL);
            }
            MMK_CloseWin(win_id);
            break;
#endif
#endif
        default :
            MMK_CloseWin(win_id);
            break;
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        MMK_CloseWin(win_id);
        break;
   
    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :mms preview get email handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMMSPreviewEmailPopWinMsg(
                                                    MMIMMS_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr  = PNULL;
    MMI_RESULT_E                 err_code  = MMI_RESULT_TRUE;
    uint32     group_id                    = 0;
    uint32   menu_id                       = 0;
    MMI_CTRL_ID_T menu_ctrl_id             = MMIMMS_PREVIEW_EMAIL_POP_CTRL_ID;
    uint8 *sender_ptr                      = ( uint8 * )MMK_GetWinAddDataPtr(win_id);
    uint16 num_len                         = 0;
//  CC_RESULT_E cc_result                  = CC_RESULT_SUCCESS;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject            = {0};
    
    edit_doc_ptr = GetMMSReadEditDoc();
    if (PNULL != edit_doc_ptr)
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
    }    
    //SCI_TRACE_LOW:"MMIMMS: HandleMMSPreviewEmailPopWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3565_112_2_18_2_38_41_330,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //弹出popmenu
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入文件管理器界面选择文件
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        if (PNULL == sender_ptr || PNULL == mms_list_info_ptr)
        {
            break;
        }
        switch (menu_id)
        {        
        case ID_MMS_SEND_MMS:
            {
                num_len = strlen((char *)sender_ptr);
                //answer mms
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }   
                MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_READ_NUMBER_SEND);
                if(num_len > 0)
                {
                    num_len = MIN(num_len, MMIMMS_MAX_ADDRESS_LEN);
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APPZ(num_len * sizeof(wchar));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (uint8 *)sender_ptr, num_len, num_len);
                    string_to.wstr_len = num_len;
                    MMIAPIMMS_GetSubjectById(mms_list_info_ptr->record_id, &string_subject);
                    MMIAPIMMS_AnswerMMS(mms_list_info_ptr->dual_sys,&string_subject,&string_to);
                    SCI_FREE(string_to.wstr_ptr);
                    string_to.wstr_ptr = PNULL;                    
                }                
            }
            break;  
        case ID_MMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                MMIPB_MAIL_T email_text = {0};

                num_len = strlen((char *)sender_ptr);                
                email_text.wstr_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                MMI_STRNTOWSTR(email_text.wstr, MMIPB_MAX_MAIL_LEN, (uint8 *)sender_ptr, num_len, email_text.wstr_len);
                MMIAPIPB_AddContactWin(&email_text, PB_GROUP_ALL, MMIPB_ADD_TYPE_EMAIL);
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT            
        case ID_MMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                MMIPB_MAIL_T email_text = {0};
                
                num_len = strlen((char *)sender_ptr);                
                email_text.wstr_len = MIN(num_len, MMIPB_MAX_MAIL_LEN);
                MMI_STRNTOWSTR(email_text.wstr, MMIPB_MAX_MAIL_LEN, (uint8 *)sender_ptr, num_len, email_text.wstr_len);
              //  MMIAPIPB_OpenEntryListForEx(&email_text, MMIPB_ADD_TYPE_EMAIL);
                MMIAPIPB_AddToExistContact(&email_text, MMIPB_ADD_TYPE_EMAIL, PNULL);
            }
            break;
#endif
        default :
            break;
        }        
        MMK_CloseWin(win_id);
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        MMK_FreeWinAddData(win_id);
        break;

    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description :inbox mms read menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMMSOptMenuWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E  err_code                 = MMI_RESULT_TRUE;
    uint32     group_id                    = 0;
    uint32   menu_id                       = 0;
    BOOLEAN    is_locked                   = FALSE;
    MMISMS_FOLDER_TYPE_E folder_type       = MMISMS_FOLDER_NORMAL;
    MMI_CTRL_ID_T menu_ctrl_id             = MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr  = PNULL;
//  MN_DUAL_SYS_E dual_sys                 = MN_DUAL_SYS_1;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    char *sender_ptr                       = PNULL;
    uint32 record_id                       = 0;
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    uint8 num_len                          = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject            = {0};
    CC_RESULT_E cc_result                  = CC_RESULT_SUCCESS; 
    MMIMMS_WINDOW_ID_E quer_win_id         = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
#ifdef TTS_SUPPORT            
    MMI_STRING_T tts_read_menu = {0};
#endif    
    
    edit_doc_ptr = GetMMSReadEditDoc();
    if (PNULL != edit_doc_ptr)
    {
        record_id = edit_doc_ptr->record_id;
    } 
    mms_list_info_ptr = MMIMMS_GetListItemByID(record_id);    
    //SCI_TRACE_LOW:"MMIMMS: HandleInboxMMSOptMenuWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_3689_112_2_18_2_38_41_331,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MEM_CHECK
        Cms_LoadCheckMemModule();
#endif     
        
#ifdef VT_SUPPORT
        if (MN_GMMREG_RAT_3G == MMIAPIPHONE_GetTDOrGsm(MMI_DUAL_SYS_MAX))
        {
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSOPTION,ID_MMS_VIDEOPHONE_DAIL,FALSE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSOPTION,ID_MMS_VIDEOPHONE_DAIL,TRUE);
        }
#endif
        
        if (PNULL != mms_list_info_ptr)
        {
            is_locked = mms_list_info_ptr->is_lock;
            sender_ptr = (char *)mms_list_info_ptr->sender;
            folder_type = mms_list_info_ptr->folder_type;
        }
        
        if (!is_locked)
        {
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSNOTIOPTION,ID_MMS_INBOX_DEL,FALSE);
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSOPTION,ID_MMS_INBOX_DEL,FALSE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSNOTIOPTION,ID_MMS_INBOX_DEL,TRUE);
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSOPTION,ID_MMS_INBOX_DEL,TRUE);
        }

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        if (MMISMS_FOLDER_SECURITY == folder_type)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_SECURITY_ITEM,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_PREVIEW_ITEM,FALSE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_SECURITY_ITEM,FALSE);
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_PREVIEW_ITEM,TRUE);
        }
#endif
        //无收件人时灰化呼叫菜单和增加至地址簿菜单
        if (sender_ptr != PNULL && MMIAPICOM_IsValidNumberString(sender_ptr, SCI_STRLEN(sender_ptr)))
        {
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSNOTIOPTION,ID_MMS_CALL_OPT,FALSE);
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSOPTION,ID_MMS_CALL_OPT,FALSE);
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSNOTIOPTION,ID_MMS_ADD2PB,FALSE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSNOTIOPTION,ID_MMS_CALL_OPT,TRUE);
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSOPTION,ID_MMS_CALL_OPT,TRUE);
            GUIMENU_SetItemGrayed(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,MENU_MMS_INBOX_MMSNOTIOPTION,ID_MMS_ADD2PB,TRUE);
        }
#ifdef TTS_SUPPORT            
        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
        {
            MMI_GetLabelTextByLang(STXT_STOP, &tts_read_menu);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SET_TTS_PLAY, &tts_read_menu);
        }
        GUIMENU_SetStaticItem(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,group_id,ID_MMS_OUTBOX_PLAY,&tts_read_menu,0);
#endif 

        {
            GUIMENU_POP_SEL_INFO_T  pop_item_info  = {0};
            MMI_MENU_GROUP_ID_T extract_group_id   = MENU_MMS_EXTRACT;
            
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = menu_ctrl_id;
            if (MMIMMS_Setting_GetExtractContent())
            {
                pop_item_info.menu_id = ID_MMS_INBOX_EXTRACT_YES;
            }
            else
            {
                pop_item_info.menu_id = ID_MMS_INBOX_EXTRACT_NO;
            }
            
            pop_item_info.group_id = extract_group_id;
            
            GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
        }

        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;

    case MSG_CLOSE_WINDOW:
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id  )
        {
        case ID_MMS_IP_DIAL:
        case ID_MMS_VIDEOPHONE_DAIL:
        case ID_MMS_DIAL:
            {
                //must consider email case
                num_len = (uint8)SCI_STRLEN((char *)mms_list_info_ptr->sender);
                if(ID_MMS_DIAL == menu_id)
                {                        
                    cc_result = MMIAPICC_MakeCall(
                                                                    mms_list_info_ptr->dual_sys,
                                                                    mms_list_info_ptr->sender,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    CC_CALL_SIM_MAX,
                                                                    CC_CALL_NORMAL_CALL,
                                                                    PNULL
                                                                    );
                }
#ifdef VT_SUPPORT
                else if (ID_MMS_VIDEOPHONE_DAIL == menu_id)
                {
                    cc_result = MMIAPICC_MakeCall(
                                                                    mms_list_info_ptr->dual_sys,
                                                                    mms_list_info_ptr->sender,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    CC_CALL_SIM_MAX,
                                                                    CC_CALL_VIDEO_CALL,
                                                                    PNULL
                                                                    );
                }
#endif
#ifdef MMI_IP_CALL_SUPPORT
                else
                {
                    cc_result = MMIAPICC_MakeCall(
                                                                    mms_list_info_ptr->dual_sys,
                                                                    mms_list_info_ptr->sender,
                                                                    num_len,
                                                                    PNULL,
                                                                    PNULL,
                                                                    CC_CALL_SIM_MAX,
                                                                    CC_CALL_IP_CALL,
                                                                    PNULL
                                                                    );
                }
#endif
                MMK_CloseWin(win_id);
            }
            break;
            
        case ID_MMS_ANSWER_SMS:
            MMIAPISMS_AnswerMessage(mms_list_info_ptr->dual_sys, PNULL, mms_list_info_ptr->sender, (uint8)strlen((char *)mms_list_info_ptr->sender));
            MMK_CloseWin(win_id);
            break;
            
        case ID_MMS_ANSWER_MMS:
            {
                num_len = strlen((char *)mms_list_info_ptr->sender);
                //answer mms
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }   
                MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_READ_TO_ANSWER);
                if(num_len > 0)
                {
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APPZ(num_len * sizeof(wchar));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (uint8 *)mms_list_info_ptr->sender, num_len, num_len);
                    string_to.wstr_len = num_len;
                    MMIAPIMMS_GetSubjectById(mms_list_info_ptr->record_id, &string_subject);
                    MMIAPIMMS_AnswerMMS(mms_list_info_ptr->dual_sys,&string_subject,&string_to);
                    SCI_FREE(string_to.wstr_ptr);
                }
                MMK_CloseWin(win_id);
            }        
            break;
            
        case ID_MMS_INBOX_FORWARD:
            {
                //forward mms
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_READ_TO_FARWARD);
                MMIAPIMMS_FarwardMMS(mms_list_info_ptr->record_id);
                MMK_CloseWin(win_id);
            }
            break;        

        case ID_MMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                // save to phonebook    
                MMIAPICOM_GenPartyNumber(mms_list_info_ptr->sender,(uint16)SCI_STRLEN((char *)mms_list_info_ptr->sender), &party_num);

                pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                            pb_bcd.number,  
                            party_num.bcd_num, 
                            pb_bcd.number_len 
                            );
                MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
                //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
                MMK_CloseWin(win_id);
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT			
        case ID_MMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                // save to phonebook    
                MMIAPICOM_GenPartyNumber(mms_list_info_ptr->sender,(uint16)SCI_STRLEN((char *)mms_list_info_ptr->sender), &party_num);
                
                pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );
              //  MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
                MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
                MMK_CloseWin(win_id);
            }
            break;
#endif            
        case ID_MMS_SAVE_MEDIA_FILE:
            {
                //save multifile create saving win
                MMK_CreateWin((uint32*)MMIMMS_SAVE_MULTIFILE_LIST_TAB, PNULL);
                MMK_CloseWin(win_id);
            }
            break;
            
        case ID_MMS_INBOX_DEL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            break;            
            
            
        case ID_MMS_INBOX_DOWNLOAD_MMS:               
            //download mms
            if (PNULL != mms_list_info_ptr)
            {
                //MMIMMS_DetailAndDownloadMMS(mms_list_info_ptr->dual_sys, mms_list_info_ptr->record_id);
            }            
            break;
            
        case ID_MMS_OUTBOX_PLAY:
            MMK_CloseWin(win_id);
            SetMMSPreviewIsWebPlay(TRUE);
            //MMK_SendMsg(MMIMMS_PREVIEW_WIN_ID,MSG_APP_WEB,PNULL);
#ifdef TTS_SUPPORT
            if(!MmsIsSuitableForTTS())
            {
                if(!BoxPlayMMS(edit_doc_ptr))
                {
                    break;
                }
            }
            else
            {
                // 如果TTS正在运行
                if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_MMS))
                {
                    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_MMS);
                    GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_SET_TTS_PLAY, STXT_RETURN, TRUE);
                }
                else
                {
                    // 获取MMS的文本内容...
                    MMI_STRING_T    mms_text_string = {0};
                    MMITTS_PLAY_PARAM_T play_param = {0};
                    if (MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_text_string))
                    {
                        play_param.txt_addr = (uint8 *)mms_text_string.wstr_ptr;
                        play_param.txt_len_in_byte = mms_text_string.wstr_len * 2;
                        play_param.is_ucs2 = TRUE;
                        play_param.play_times = 1;
                        play_param.tts_vol = 4;
                        play_param.ring_type = TTS_RING_TYPE_MMS;
                        play_param.win_handle = MMIMMS_PREVIEW_WIN_ID;
						play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
						play_param.digit_mode = TTS_DIGIT_AUTO;
						play_param.punc_mode = TTS_PUNC_OFF;
                        if(MMIAPITTS_PlayText(&play_param))
                        {
                            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_STOP, STXT_RETURN, TRUE);
                        }
                    }
                }
            }           
#else            
            {
                // for bug 72807 
                wchar music_name_wstr[MAX_NAME_LENGTH + 1] = {0};
                uint16 music_name_wlen                 = 0;
                uint8 *music_name_ptr                  = PNULL;
                uint8 music_name_len                   = 0;
                wchar suffix_name[MAX_NAME_LENGTH + 1] = {0};
                uint16 suffix_len = MMIMMS_SUFFIX_LEN;
                
                if (PNULL != edit_doc_ptr)
                {
                    music_name_ptr = (uint8 *)(edit_doc_ptr->editbody_ptr->slide_cur->audiofile);
                    music_name_len = (uint8)strlen((char *)music_name_ptr);
                }
                music_name_wlen = GUI_UTF8ToWstr(music_name_wstr, music_name_len, music_name_ptr, music_name_len);	
                if (MMIAPIFMM_SplitFileName(music_name_wstr, MMIAPICOM_Wstrlen(music_name_wstr),
                    PNULL, PNULL,
                    suffix_name, &suffix_len))
                {
                    if(MMIFMM_UNKNOW_TYPE==MMIAPIFMM_GetFileType(suffix_name,suffix_len))
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_COMMON_NO_SUPPORT);
                        break;
                    }
                }
            }
            BoxPlayMMS(edit_doc_ptr);
#endif
            break;
        case ID_MMS_INBOX_EXTRACT_YES:               
            //extract 
            MMIMMS_Setting_SetExtractContent(TRUE);
            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_CloseWin(win_id);
            break;

        case ID_MMS_INBOX_EXTRACT_NO:               
            //extract
            MMIMMS_Setting_SetExtractContent(FALSE);
            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_CloseWin(win_id);
            break;

        case ID_MMS_INBOX_DETAIL:
            {
                MMIMMS_SetPreviewMMSDetailFlag(FALSE);
                MMK_CreateWin((uint32 *)MMIMMS_INBOX_NOTIDETAIL_WIN_TAB, (ADD_DATA)record_id);
                MMK_CloseWin(win_id);
            }            
            break;
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case ID_MMS_MENU_BOX_SECURITY_ITEM:
        case ID_MMS_MENU_BOX_PREVIEW_ITEM:
            MMIPUB_OpenQueryWinByTextId(TXT_MOVE,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
			MMK_CloseWin(win_id);
            break;
#endif

        #ifdef MMI_PDA_SUPPORT
        #ifdef TTS_SUPPORT
        #ifndef MMI_GUI_STYLE_TYPICAL
        case ID_MMS_TTS_PLAY:
            {
                
                if(!MmsIsSuitableForTTS())
                {
                    if(!BoxPlayMMS(edit_doc_ptr))
                    {
                        break;
                    }
                }
                else
                {
                    // 如果TTS正在运行
                    if(MMIAPITTS_GetPlayStatus())
                    {
                        MMIAPITTS_StopPlayText();
                    }
                    else
                    {
                        // 获取MMS的文本内容...
                        MMI_STRING_T    mms_text_string = {0};
                        MMITTS_PLAY_PARAM_T play_param = {0};
                        if (MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_text_string))
                        {
                            play_param.txt_addr = (uint8 *)mms_text_string.wstr_ptr;
                            play_param.txt_len_in_byte = mms_text_string.wstr_len * 2;
                            play_param.is_ucs2 = TRUE;
                            play_param.play_times = 1;
                            play_param.tts_vol = 4;
                            play_param.ring_type = TTS_RING_TYPE_MMS;
                            play_param.win_handle = MMIMMS_PREVIEW_WIN_ID;
							play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
							play_param.digit_mode = TTS_DIGIT_AUTO;
							play_param.punc_mode = TTS_PUNC_OFF;
                            if(MMIAPITTS_PlayText(&play_param))                        
                            {                            
                                //SCI_TRACE_LOW:"MMIMMS TTS play text ok"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_4051_112_2_18_2_38_42_332,(uint8*)"");
                            }
                        }
                    }
                }
                MMK_CloseWin(win_id);
            }    
            break;
        #endif   
        #endif
        #endif

        case ID_MMS_INBOX_PICKOUT_NUMBER:        
            {     
            }
            break;
            
        case ID_MMS_EDIT_EXTRACT_STADDR:
            { 
            }           
            break;
            
        default:
            //SCI_TRACE_MID:"warning_ menu_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_4073_112_2_18_2_38_42_333,(uint8*)"d",menu_id);
            break;
        }
        break;
        
    case MSG_PROMPTWIN_OK:
        {
            MMK_CloseWin(quer_win_id);
            GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
            switch ( menu_id  )
            {
            case ID_MMS_INBOX_DEL:
                if (PNULL != mms_list_info_ptr)
                {
                    //delete the mms 
                    MMIMMS_DeleteMMSAndSaveList(mms_list_info_ptr->record_id);
#ifdef MMI_GUI_STYLE_TYPICAL
                    MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                    
#endif
                    MMK_CloseWin(win_id);
                    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
                    {
                        MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                    }
                } 
                break;
                
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
            case ID_MMS_MENU_BOX_SECURITY_ITEM:
                MMIAPIMMS_MoveSecurityMMS(mms_list_info_ptr->record_id, TRUE);
                MMK_CloseWin(win_id);
                if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                }
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                
#endif
                break;

            case ID_MMS_MENU_BOX_PREVIEW_ITEM:
                MMIAPIMMS_MoveSecurityMMS(mms_list_info_ptr->record_id, FALSE);
                MMK_CloseWin(win_id);
                if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                }
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                
#endif
                break;
#endif

            default:
                //SCI_TRACE_MID:"warning_ menu_id=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_4121_112_2_18_2_38_42_334,(uint8*)"d",menu_id);
                break;
            }
               
        }
        break;  
        
    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(quer_win_id);
        break; 
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code;
}

/*****************************************************************************/
//  Description :mms detail read menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMMSDetailOptMenuWinMsg(
                                                      MMIMMS_WINDOW_ID_E win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param
                                                      )
{
    // MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr  = PNULL;
    MMI_RESULT_E  err_code                 = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id                  = MMIMMS_NOTIDETAIL_OPTMENU_CTRL_ID;
    uint32 record_id                       = 0;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    uint32 unsettle_num                    = 0;
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
    uint8 num_len                          = 0;
    MMI_STRING_T string_to                 = {0};
    MMI_STRING_T string_subject            = {0};
    uint32 node_id                         = 0;
    CC_RESULT_E cc_result                  = CC_RESULT_SUCCESS;
    MMIMMS_WINDOW_ID_E quer_win_id         = MMIMMS_INSERT_PIC_QUERY_WIN_ID;

    record_id = MMK_GetWinAddDataPtr(win_id);/*lint !e64*/
    if (MMIMMS_IsRecordIDExist(record_id))
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(record_id);
    }
    else
    {
        //assert
    }
    if(PNULL == mms_list_info_ptr)
    {
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW:"MMIMMS: HandleInboxMMSDetailOptMenuWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_4182_112_2_18_2_38_42_335,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //打开popmenu
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //进入选项
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch (node_id)
        {
        case MMIMMS_DETAIL_OPT_DOWNLOAD:
            //把该record id的彩信通知加入等待下载队列，同时改变该条消息的图标
            if (!MMIMMS_IsInQueue(record_id))//record_id没有在下载链表里面，加入下载链表
            {
                MMIMMS_PUSH_FILE_T *push_info_ptr      = PNULL;

                MMK_CloseWin(win_id);
                push_info_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
                if (PNULL != push_info_ptr)
                {
                    //SCI_MEMSET((uint8 *)push_info_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
                    MMIMMS_ReadPush(push_info_ptr, record_id);
                }
                else
                {
                    break;
                }
                if (MMIMMS_IsSimCardOK(mms_list_info_ptr->dual_sys))
                {
                    if (MMIMMS_IsExpire(mms_list_info_ptr->time,(uint32)push_info_ptr->expiry_time))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_EXPIRED, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                    }
                    else if (!MMIMMS_IsEnoughSpace(push_info_ptr->size + MMIMMS_RESERVE_LIST_SIZE))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_FULLSPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    }
                    else if(MMIMMS_DetailAndDownloadMMS(mms_list_info_ptr->dual_sys, record_id))
                    {
                        MMIAPISMS_UpdateMsgStatus(); 
                        if (MMIMMS_IsAutolistWithItem())
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_MMS_TEXT_PREPARE_DOWNLOAD, TXT_NULL, IMAGE_PUBWIN_WARNING);
                        }                                                
                        MMK_CloseWin(win_id);
                        if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
                        {
                            MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
                        }
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIMMS: HandleInboxMMSDetailOptMenuWinMsg active mms error!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_4240_112_2_18_2_38_43_336,(uint8*)"");
                    }
                }
                else
                {
                    MMIAPIPHONE_AlertSimNorOKStatusII(mms_list_info_ptr->dual_sys);
                } 
                SCI_FREE(push_info_ptr);
                push_info_ptr = PNULL;
            }
            break;
        case MMIMMS_DETAIL_OPT_UNDOWNLOAD:  
            //从下载等待队列中取消该条彩信，同时改变该条消息的状态图标
            if (MMIMMS_IsInQueue(record_id))
            {
                //[start]changed by minghu cr166756
                if (mms_list_info_ptr->mo_mt_type == MMISMS_MT_NOT_NOTIFY)
                {
                    mms_list_info_ptr->mo_mt_type = MMISMS_MT_NOT_DOWNLOAD;
                    MMIMMS_SaveMMSListFile();
                }
                //[end]
                MMK_CloseWin(win_id);
                MMIMMS_CancelSendRecvById(record_id);
                MMIAPISMS_UpdateMsgStatus();
                GUISOFTKEY_SetTextId(MMIMMS_INBOX_NOTIDETAIL_WIN_ID, MMICOMMON_SOFTKEY_CTRL_ID, STXT_OPTION, TXT_COMMON_DOWNLOAD_COMN, STXT_RETURN, TRUE);
            }            
            break;
        case MMIMMS_DETAIL_OPT_DEL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            break;
        case MMIMMS_DETAIL_OPT_ANSWER_SMS: 
            MMIAPISMS_AnswerMessage(mms_list_info_ptr->dual_sys, PNULL, mms_list_info_ptr->sender, (uint8)strlen((char *)mms_list_info_ptr->sender));
            MMK_CloseWin(win_id);
            break;
        case MMIMMS_DETAIL_OPT_ANSWER_MMS:  
            {
                num_len = strlen((char *)mms_list_info_ptr->sender);
                //answer mms  
                MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_LIST_ANSWER);
                if(num_len > 0)
                {
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APPZ(num_len * sizeof(wchar));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (uint8 *)mms_list_info_ptr->sender, num_len, num_len);
                    string_to.wstr_len = num_len;
                    MMIAPIMMS_GetSubjectById(mms_list_info_ptr->record_id, &string_subject);
                    MMIAPIMMS_AnswerMMS(mms_list_info_ptr->dual_sys,&string_subject,&string_to);
                    SCI_FREE(string_to.wstr_ptr);
                }
                MMK_CloseWin(win_id);
            }
            break;
        case MMIMMS_DETAIL_OPT_NOMAL_CALL:
        case MMIMMS_DETAIL_OPT_IP_CALL: 
        case MMIMMS_DETAIL_OPT_VIDEO_CALL:
            {
                num_len = (uint8)SCI_STRLEN((char *)mms_list_info_ptr->sender);
                if(MMIMMS_DETAIL_OPT_NOMAL_CALL == node_id)
                {                        
                    cc_result = MMIAPICC_MakeCall(
                        mms_list_info_ptr->dual_sys,
                        mms_list_info_ptr->sender,
                        num_len,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
                }
#ifdef MMI_IP_CALL_SUPPORT
                else if (MMIMMS_DETAIL_OPT_IP_CALL == node_id)
                {
                    cc_result = MMIAPICC_MakeCall(
                        mms_list_info_ptr->dual_sys,
                        mms_list_info_ptr->sender,
                        num_len,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        CC_CALL_IP_CALL,
                        PNULL
                        );
                }
#endif
#ifdef VT_SUPPORT
                else if(MMIMMS_DETAIL_OPT_VIDEO_CALL == node_id)
                {
                    cc_result = MMIAPICC_MakeCall(
                        mms_list_info_ptr->dual_sys,
                        mms_list_info_ptr->sender,
                        num_len,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        CC_CALL_VIDEO_CALL,
                        PNULL
                        );
                }
#endif
                MMK_CloseWin(win_id);
            }
            break;  
        case MMIMMS_DETAIL_OPT_ADD2PB_NEW_ITEM:
            MMIAPICOM_GenPartyNumber(mms_list_info_ptr->sender,
                (uint16)SCI_STRLEN((char *)mms_list_info_ptr->sender), &party_num);            
            pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY( 
                pb_bcd.number,  
                party_num.bcd_num, 
                pb_bcd.number_len 
                );
            MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
            //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
            MMK_CloseWin(win_id);
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT			
        case MMIMMS_DETAIL_OPT_ADD2PB_EXIST_ITEM:
            MMIAPICOM_GenPartyNumber(mms_list_info_ptr->sender,
                                    (uint16)SCI_STRLEN((char *)mms_list_info_ptr->sender), &party_num);            
            pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
            SCI_MEMCPY( 
                        pb_bcd.number,  
                        party_num.bcd_num, 
                        pb_bcd.number_len 
                        );
           // MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
            MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
            MMK_CloseWin(win_id);
            break;
#endif            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMIMMS_DETAIL_OPT_BOX_SECURITY_ITEM:
        case MMIMMS_DETAIL_OPT_BOX_PREVIEW_ITEM:
            MMIPUB_OpenQueryWinByTextId(TXT_MOVE,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            break;
#endif

        default :
            break;
        }        
        break;
    case MSG_PROMPTWIN_OK:
        //删除图片，音乐，当前页，视频
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        MMK_CloseWin(quer_win_id);
        switch (node_id)
        {
        case MMIMMS_DETAIL_OPT_DEL:
            MMIMMS_DeleteMMSAndSaveList(record_id);
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
            if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
            }
            unsettle_num = MMIMMS_GetUnsettleNum();
            if (0 == unsettle_num)
            {
                if (MMK_IsOpenWin(MMIMMS_NEWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_NEWMSG_WIN_ID);
                }
            }
            MMK_CloseWin(win_id);
            break;
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case MMIMMS_DETAIL_OPT_BOX_SECURITY_ITEM:
            MMIAPIMMS_MoveSecurityMMS(mms_list_info_ptr->record_id, TRUE);
            MMK_CloseWin(win_id);
            if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
            }  
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlrtWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
            break;

        case MMIMMS_DETAIL_OPT_BOX_PREVIEW_ITEM:
            MMIAPIMMS_MoveSecurityMMS(mms_list_info_ptr->record_id, FALSE);
            MMK_CloseWin(win_id);
            if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
            }
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
            break;
#endif

        default :
            break;
        }        
        break;
    case MSG_PROMPTWIN_CANCEL:
        //close query window
        MMK_CloseWin(quer_win_id);
        MMK_CloseWin(win_id);
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //返回编辑主界面
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        //返回编辑主界面        
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }
    
    
    return err_code;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :open mms detail read option menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSDetailOptPopMenu(uint32 record_id)
{
    MMI_CTRL_ID_T	ctrl_id                = MMIMMS_NOTIDETAIL_OPTMENU_CTRL_ID;
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#ifdef MMI_SMS_MOVE_SUPPORT
    uint16 menu_index                      = 0;
#endif
#endif
    uint16 child_index                     = 0;
    BOOLEAN  is_locked                     = FALSE;
    MMISMS_FOLDER_TYPE_E folder_type       = MMISMS_FOLDER_NORMAL;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    char *sender_ptr                       = PNULL;
    BOOLEAN is_grayed                      = FALSE;
    
    if (MMIMMS_IsRecordIDExist(record_id))
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(record_id);
        if (PNULL == mms_list_info_ptr)
        {
            return;
        }
        else
        {
            is_locked = mms_list_info_ptr->is_lock;
            folder_type = mms_list_info_ptr->folder_type;
            sender_ptr = (char *)mms_list_info_ptr->sender;
        }
    }
    else
    {
        return;
    }
    
    
    MMK_CreateWin((uint32 *)MMIMMS_INBOX_MMSNOTIMENU_WIN_TAB, (ADD_DATA)record_id);
    GUIMENU_CreatDynamic(PNULL, MMIMMS_INBOX_MMSNOTIOPT_MENU_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP);
    
    //if()//如果是下载等待中则取消下载如果没有下载则为下载
    
    if (is_locked)//锁定彩信不添加删除项
    {
        is_grayed = TRUE;   
    }
    else
    {
        is_grayed = FALSE;
    }
    is_grayed = FALSE;


    #ifdef MMIMMS_SMS_IN_1_SUPPORT
    InsertNodeByLabel(
    ctrl_id,//ctrl_id
    TXT_SMS_WRITE,//text_ptr 
    MMIMMS_DETAIL_OPT_ANSWER_SMS,//node_id
    0, //parent_node_id
    child_index++,//index
    is_grayed
    );
    #else    
    InsertNodeByLabelEx(
    ctrl_id,//ctrl_id
    TXT_MMS,//text_ptr 
    IMAGE_COMMON_OPTMENU_WRITE_MMS,
    MMIMMS_DETAIL_OPT_ANSWER_MMS,//node_id
    0, //parent_node_id
    child_index++,//index
    is_grayed
    );
    #endif

    
    child_index = 0;

    //if (!is_grayed)
    {
        InsertNodeByLabelEx(
            ctrl_id,//ctrl_id
            TXT_CALL_AUDIO,//text_ptr 
            IMAGE_MESSAGE_OPTMENU_VOICECALL,
            MMIMMS_DETAIL_OPT_NOMAL_CALL,//node_id
            0, //parent_node_id
            child_index++,//index
            is_grayed
            );
        
#ifdef VT_SUPPORT        
        InsertNodeByLabelEx(
            ctrl_id,//ctrl_id
            TXT_CALL_VIDEO,//text_ptr 
            IMAGE_COMMON_OPTMENU_VIDIO_CALL,
            MMIMMS_DETAIL_OPT_VIDEO_CALL,//node_id
            0, //parent_node_id
            child_index++,//index
            is_grayed
            );
#endif
#ifdef MMI_IP_CALL_SUPPORT
        InsertNodeByLabelEx(
            ctrl_id,//ctrl_id
            TXT_CALL_IPCALL,//text_ptr 
            IMAGE_COMMON_OPTMENU_IP_CALL,
            MMIMMS_DETAIL_OPT_IP_CALL,//node_id
            0, //parent_node_id
            child_index++,//index
            is_grayed
            );
#endif
    }    

    
    child_index = 0;

    InsertNodeByLabelEx(
        ctrl_id,//ctrl_id
        TXT_ADDTOPB_NEW,//text_ptr 
        IMAGE_MESSAGE_OPTMENU_INSERT_CONTACT,
        MMIMMS_DETAIL_OPT_ADD2PB_NEW_ITEM,//node_id
        0, //parent_node_id
        child_index++,//index
        is_grayed
        );
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    InsertNodeByLabelEx(
        ctrl_id,//ctrl_id
        TXT_ADDTOPB_EXIST,//text_ptr 
        IMAGE_COMMON_OPTMENU_SAVE_CONTENT,
        MMIMMS_DETAIL_OPT_ADD2PB_EXIST_ITEM,//node_id
        0, //parent_node_id
        child_index++,//index
        is_grayed
        );
#endif
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
#ifdef MMI_SMS_MOVE_SUPPORT
    is_grayed = FALSE;
    child_index = 0;
    InsertNodeByLabelEx(
        ctrl_id,//ctrl_id
        TXT_MOVE,//text_ptr 
        IMAGE_MESSAGE_OPTMENU_MOVE,
        MMIMMS_DETAIL_OPT_MOVE_ITEM,//node_id
        0, //parent_node_id
        menu_index++,//index
        is_grayed
        ); 
    if (MMISMS_FOLDER_SECURITY == folder_type)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS_SECURITY,//text_ptr 
            MMIMMS_DETAIL_OPT_BOX_SECURITY_ITEM,//node_id
            MMIMMS_DETAIL_OPT_MOVE_ITEM, //parent_node_id
            child_index++,//index
            TRUE
            );

        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS_PREVIEW,//text_ptr 
            MMIMMS_DETAIL_OPT_BOX_PREVIEW_ITEM,//node_id
            MMIMMS_DETAIL_OPT_MOVE_ITEM, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
    else
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS_SECURITY,//text_ptr 
            MMIMMS_DETAIL_OPT_BOX_SECURITY_ITEM,//node_id
            MMIMMS_DETAIL_OPT_MOVE_ITEM, //parent_node_id
            child_index++,//index
            FALSE
            );

        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS_PREVIEW,//text_ptr 
            MMIMMS_DETAIL_OPT_BOX_PREVIEW_ITEM,//node_id
            MMIMMS_DETAIL_OPT_MOVE_ITEM, //parent_node_id
            child_index++,//index
            TRUE
            );
    }

#ifdef MMI_PDA_SUPPORT
    {
        GUIMENU_BUTTON_INFO_T button_info = {0};
        button_info.is_static = FALSE;
        button_info.node_id= MMIMMS_DETAIL_OPT_MOVE_ITEM;
        button_info.button_style = GUIMENU_BUTTON_STYLE_CANCEL;
        GUIMENU_SetButtonStyle(ctrl_id, &button_info);
    }
#endif
#endif
#endif//MMI_SMS_SECURITYBOX_SUPPORT
}

#else
/*****************************************************************************/
//  Description :open mms detail read option menu
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void OpenMMSDetailOptPopMenu(uint32 record_id)
{
    MMI_CTRL_ID_T	ctrl_id                = MMIMMS_NOTIDETAIL_OPTMENU_CTRL_ID;
    uint16 menu_index                      = 0;
    uint16 child_index                     = 0;
    BOOLEAN  is_locked                     = FALSE;
    MMISMS_FOLDER_TYPE_E folder_type       = MMISMS_FOLDER_NORMAL;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    char *sender_ptr                       = PNULL;
    BOOLEAN is_grayed                      = FALSE;
    
    if (MMIMMS_IsRecordIDExist(record_id))
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(record_id);
        if (PNULL == mms_list_info_ptr)
        {
            return;
        }
        else
        {
            is_locked = mms_list_info_ptr->is_lock;
            folder_type = mms_list_info_ptr->folder_type;
            sender_ptr = (char *)mms_list_info_ptr->sender;
        }
    }
    else
    {
        return;
    }
    
    
    MMK_CreateWin((uint32 *)MMIMMS_INBOX_MMSNOTIMENU_WIN_TAB, (ADD_DATA)record_id);
    GUIMENU_CreatDynamic(PNULL, MMIMMS_INBOX_MMSNOTIOPT_MENU_WIN_ID,  ctrl_id, GUIMENU_STYLE_POPUP);
    
    //if()//如果是下载等待中则取消下载如果没有下载则为下载
    #ifdef MMI_GUI_STYLE_TYPICAL
    if (MMIMMS_IsInQueue(record_id))
    {
        InsertNodeByLabel(
            ctrl_id,                    //ctrl_id
            TXT_MMS_EXIT_DOWNLOAD,       //text_ptr 
            MMIMMS_DETAIL_OPT_UNDOWNLOAD, //node_id
            0,                          //parent_node_id
            menu_index++,               //index
            is_grayed
            );
    }  
    else
    {
        InsertNodeByLabel(
            ctrl_id,                    //ctrl_id
            TXT_COMMON_DOWNLOAD_COMN,       //text_ptr 
            MMIMMS_DETAIL_OPT_DOWNLOAD, //node_id
            0,                          //parent_node_id
            menu_index++,               //index
            is_grayed
            );
    }
    #endif
    
    if (is_locked)//锁定彩信不添加删除项
    {
        is_grayed = TRUE;   
    }
    else
    {
        is_grayed = FALSE;
    }
    #ifndef MMI_PDA_SUPPORT
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_DELETE,//text_ptr 
        MMIMMS_DETAIL_OPT_DEL,//node_id
        0, //parent_node_id
        menu_index++,//index
        is_grayed
        );
    #endif
    is_grayed = FALSE;
    
    #ifdef MMIMMS_SMS_IN_1_SUPPORT
    InsertNodeByLabel(
    ctrl_id,//ctrl_id
    TXT_SMS_WRITE,//text_ptr 
    MMIMMS_DETAIL_OPT_ANSWER_SMS,//node_id
    MMIMMS_DETAIL_OPT_ANSWER, //parent_node_id
    child_index++,//index
    is_grayed
    );
    #else
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_REPLY,//text_ptr 
        MMIMMS_DETAIL_OPT_ANSWER,//node_id
        0, //parent_node_id
        menu_index++,//index
        is_grayed
        );
    //if (!is_grayed)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS,//text_ptr 
            MMIMMS_DETAIL_OPT_ANSWER_SMS,//node_id
            MMIMMS_DETAIL_OPT_ANSWER, //parent_node_id
            child_index++,//index
            is_grayed
            );
        
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_MMS,//text_ptr 
            MMIMMS_DETAIL_OPT_ANSWER_MMS,//node_id
            MMIMMS_DETAIL_OPT_ANSWER, //parent_node_id
            child_index++,//index
            is_grayed
            );
    }    
    #endif
    
    child_index = 0;
    
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_CALL,//text_ptr 
        MMIMMS_DETAIL_OPT_CALL,//node_id
        0, //parent_node_id
        menu_index++,//index
        is_grayed
        );
    
    //if (!is_grayed)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_CALL_AUDIO,//text_ptr 
            MMIMMS_DETAIL_OPT_NOMAL_CALL,//node_id
            MMIMMS_DETAIL_OPT_CALL, //parent_node_id
            child_index++,//index
            is_grayed
            );
        
#ifdef VT_SUPPORT        
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_CALL_VIDEO,//text_ptr 
            MMIMMS_DETAIL_OPT_VIDEO_CALL,//node_id
            MMIMMS_DETAIL_OPT_CALL, //parent_node_id
            child_index++,//index
            is_grayed
            );
#endif
#ifdef MMI_IP_CALL_SUPPORT
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_CALL_IPCALL,//text_ptr 
            MMIMMS_DETAIL_OPT_IP_CALL,//node_id
            MMIMMS_DETAIL_OPT_CALL, //parent_node_id
            child_index++,//index
            is_grayed
            );
#endif
    }    
    
    child_index = 0;

    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_ADDCONTACT,//text_ptr 
        MMIMMS_DETAIL_OPT_ADD2PB,//node_id
        0, //parent_node_id
        menu_index++,//index
        is_grayed
        );  
	
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_ADDTOPB_NEW,//text_ptr 
        MMIMMS_DETAIL_OPT_ADD2PB_NEW_ITEM,//node_id
        MMIMMS_DETAIL_OPT_ADD2PB, //parent_node_id
        child_index++,//index
        is_grayed
        );
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_ADDTOPB_EXIST,//text_ptr 
        MMIMMS_DETAIL_OPT_ADD2PB_EXIST_ITEM,//node_id
        MMIMMS_DETAIL_OPT_ADD2PB, //parent_node_id
        child_index++,//index
        is_grayed
        );

#endif
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
    is_grayed = FALSE;
    child_index = 0;
    InsertNodeByLabel(
        ctrl_id,//ctrl_id
        TXT_MOVE,//text_ptr 
        MMIMMS_DETAIL_OPT_MOVE_ITEM,//node_id
        0, //parent_node_id
        menu_index++,//index
        is_grayed
        ); 
    
    if (MMISMS_FOLDER_SECURITY == folder_type)
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS_SECURITY,//text_ptr 
            MMIMMS_DETAIL_OPT_BOX_SECURITY_ITEM,//node_id
            MMIMMS_DETAIL_OPT_MOVE_ITEM, //parent_node_id
            child_index++,//index
            TRUE
            );

        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS_PREVIEW,//text_ptr 
            MMIMMS_DETAIL_OPT_BOX_PREVIEW_ITEM,//node_id
            MMIMMS_DETAIL_OPT_MOVE_ITEM, //parent_node_id
            child_index++,//index
            FALSE
            );
    }
    else
    {
        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS_SECURITY,//text_ptr 
            MMIMMS_DETAIL_OPT_BOX_SECURITY_ITEM,//node_id
            MMIMMS_DETAIL_OPT_MOVE_ITEM, //parent_node_id
            child_index++,//index
            FALSE
            );

        InsertNodeByLabel(
            ctrl_id,//ctrl_id
            TXT_SMS_PREVIEW,//text_ptr 
            MMIMMS_DETAIL_OPT_BOX_PREVIEW_ITEM,//node_id
            MMIMMS_DETAIL_OPT_MOVE_ITEM, //parent_node_id
            child_index++,//index
            TRUE
            );
    }
 #endif
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :Insert Node by label Ex
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void InsertNodeByLabelEx(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                             MMI_IMAGE_ID_T          menu_icon_id,
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,          //位置
                             BOOLEAN            is_graged
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = is_graged;
    node_item.select_icon_id = menu_icon_id;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}
#endif

/*****************************************************************************/
//  Description :insert dynamenu node by label
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,       //插入的节点的文本
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
                             uint16          	index,           //位置
                             BOOLEAN            is_grayed
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = is_grayed;
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

/*****************************************************************************/
//  Description :send box mms read menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendboxMMSOptMenuWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E err_code                  = MMI_RESULT_TRUE;
    uint32  group_id                       = 0;
    uint32  menu_id                        = 0;
    BOOLEAN is_locked                      = FALSE;
    MMISMS_FOLDER_TYPE_E folder_type       = MMISMS_FOLDER_NORMAL;
    MMI_CTRL_ID_T menu_ctrl_id             = MMIMMS_SENTBOX_OPT_MENU_CTRL_ID;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr  = PNULL;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    MMIMMS_WINDOW_ID_E quer_win_id         = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMIPB_BCD_NUMBER_T    pb_bcd           = {0};
    MMI_PARTY_NUMBER_T    party_num        = {MN_NUM_TYPE_UNKNOW, 0, 0};
#ifdef TTS_SUPPORT            
    MMI_STRING_T tts_read_menu = {0};
#endif    
    
    edit_doc_ptr = GetMMSReadEditDoc();
    if (PNULL != edit_doc_ptr)
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id);
    }    
    if(PNULL == mms_list_info_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW:"MMIMMS: HandleSendboxMMSOptMenuWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_4988_112_2_18_2_38_44_337,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));

        {
            is_locked = mms_list_info_ptr->is_lock;
            folder_type = mms_list_info_ptr->folder_type;
        }        
        if (!is_locked)
        {
            GUIMENU_SetItemGrayed(MMIMMS_SENTBOX_OPT_MENU_CTRL_ID,group_id,ID_MMS_SENTBOX_DEL,FALSE);
        }
        else
        { 
            GUIMENU_SetItemGrayed(MMIMMS_SENTBOX_OPT_MENU_CTRL_ID,group_id,ID_MMS_SENTBOX_DEL,TRUE);
        }

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        if (MMISMS_FOLDER_SECURITY == folder_type)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_SECURITY_ITEM,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_PREVIEW_ITEM,FALSE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_SECURITY_ITEM,FALSE);
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_PREVIEW_ITEM,TRUE);
        }
#endif
#ifdef TTS_SUPPORT            
        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
        {
            MMI_GetLabelTextByLang(STXT_STOP, &tts_read_menu);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SET_TTS_PLAY, &tts_read_menu);
        }
        GUIMENU_SetStaticItem(MMIMMS_INBOX_MMSOPT_MENU_CTRL_ID,group_id,ID_MMS_OUTBOX_PLAY,&tts_read_menu,0);
#endif 

        {
            GUIMENU_POP_SEL_INFO_T  pop_item_info  = {0};
            MMI_MENU_GROUP_ID_T extract_group_id   = MENU_MMS_EXTRACT;
            
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = menu_ctrl_id;
            if (MMIMMS_Setting_GetExtractContent())
            {
                pop_item_info.menu_id = ID_MMS_INBOX_EXTRACT_YES;
            }
            else
            {
                pop_item_info.menu_id = ID_MMS_INBOX_EXTRACT_NO;
            }
            
            pop_item_info.group_id = extract_group_id;
            
            GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
        }

        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));        
        switch ( menu_id )
        {
        case ID_MMS_SENTBOX_FORWARD:
            {
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_SENDBOX_READ_TO_FARWARD);
                MMIAPIMMS_FarwardMMS(mms_list_info_ptr->record_id);  
                MMK_CloseWin(win_id);
            }
            break; 
        case ID_MMS_SENTBOX_DEL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            break;

        case ID_MMS_INBOX_EXTRACT_YES:               
            //extract 
            MMIMMS_Setting_SetExtractContent(TRUE);
            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_CloseWin(win_id);
            break;

        case ID_MMS_INBOX_EXTRACT_NO:               
            //extract
            MMIMMS_Setting_SetExtractContent(FALSE);
            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_CloseWin(win_id);
            break;

        case ID_MMS_SENTBOX_DETAIL:
            {
                uint32 record_id = 0;

                record_id = mms_list_info_ptr->record_id;
                MMIMMS_SetPreviewMMSDetailFlag(FALSE);                
                MMK_CreateWin((uint32 *)MMIMMS_INBOX_NOTIDETAIL_WIN_TAB, (ADD_DATA)record_id);
                MMK_CloseWin(win_id);
            }            
            break;
        case ID_MMS_SENTBOX_PLAY:
            MMK_CloseWin(win_id);
            SetMMSPreviewIsWebPlay(TRUE);
            //MMK_SendMsg(MMIMMS_PREVIEW_WIN_ID,MSG_APP_WEB,PNULL);
#ifdef TTS_SUPPORT
            if(!MmsIsSuitableForTTS())
            {
                if(!BoxPlayMMS(edit_doc_ptr))
                {
                    break;
                }
            }
            else
            {
                // 如果TTS正在运行
                if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_MMS))
                {
                    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_MMS);
                    //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_SET_TTS_PLAY, STXT_RETURN, TRUE);
                }
                else
                {
                    // 获取MMS的文本内容...
                    MMI_STRING_T    mms_text_string = {0};
                    MMITTS_PLAY_PARAM_T play_param = {0};
                    if (MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_text_string))
                    {
                        play_param.txt_addr = (uint8 *)mms_text_string.wstr_ptr;
                        play_param.txt_len_in_byte = mms_text_string.wstr_len * 2;
                        play_param.is_ucs2 = TRUE;
                        play_param.play_times = 1;
                        play_param.tts_vol = 4;
                        play_param.ring_type = TTS_RING_TYPE_MMS;
                        play_param.win_handle = MMIMMS_PREVIEW_WIN_ID;
						play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
						play_param.digit_mode = TTS_DIGIT_AUTO;
						play_param.punc_mode = TTS_PUNC_OFF;
                        if(MMIAPITTS_PlayText(&play_param))
                        {
                            //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_STOP, STXT_RETURN, TRUE);
                        }
                    }
                }
            }           
#else            
            BoxPlayMMS(edit_doc_ptr);
#endif
            break;  
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case ID_MMS_MENU_BOX_SECURITY_ITEM:
        case ID_MMS_MENU_BOX_PREVIEW_ITEM:
            MMIPUB_OpenQueryWinByTextId(TXT_MOVE,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            break;
#endif

        case ID_MMS_MENU_EXTRACT_ADDTOPB_NEW_ITEM_ID:
            {
                // save to phonebook    
                MMIAPICOM_GenPartyNumber(mms_list_info_ptr->sender,(uint16)SCI_STRLEN((char *)mms_list_info_ptr->sender), &party_num);

                pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                            pb_bcd.number,  
                            party_num.bcd_num, 
                            pb_bcd.number_len 
                            );
                MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
                //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
                MMK_CloseWin(win_id);
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT			
        case ID_MMS_MENU_EXTRACT_ADDTOPB_EXIST_ITEM_ID:
            {
                // save to phonebook    
                MMIAPICOM_GenPartyNumber(mms_list_info_ptr->sender,(uint16)SCI_STRLEN((char *)mms_list_info_ptr->sender), &party_num);
                
                pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );
                //MMIAPIPB_OpenEntryListForCallLog(&pb_bcd);
                MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL);
                MMK_CloseWin(win_id);
            }
            break;
#endif            
        default:
            break;
        }
        break;        
    case MSG_PROMPTWIN_OK:
        GUIMENU_GetId(menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));            
        switch(menu_id )
        {
        case ID_MMS_SENTBOX_DEL:
            {
                MMK_CloseWin(quer_win_id);
                MMIMMS_DeleteMMSAndSaveList(mms_list_info_ptr->record_id);
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);                
#endif
                MMK_CloseWin(win_id);
                if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                }
            }
            break;
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case ID_MMS_MENU_BOX_SECURITY_ITEM:
            MMIAPIMMS_MoveSecurityMMS(mms_list_info_ptr->record_id, TRUE);
            MMK_CloseWin(win_id);
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
            }
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
            break;

        case ID_MMS_MENU_BOX_PREVIEW_ITEM:
            MMIAPIMMS_MoveSecurityMMS(mms_list_info_ptr->record_id, FALSE);
            MMK_CloseWin(win_id);
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
            }
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
            break;
#endif

        default:
            break;
        }            
        break;
    case MSG_PROMPTWIN_CANCEL:
        //close query window
        MMK_CloseWin(quer_win_id);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        // close the option window
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
         break;
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    return err_code;   
}

/*****************************************************************************/
//  Description :out box mms read menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOutboxMMSOptMenuWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E err_code                  = MMI_RESULT_TRUE;
    uint32  group_id                       = 0;
    uint32  menu_id                        = 0;
    BOOLEAN is_locked                      = FALSE;    
    MMISMS_FOLDER_TYPE_E folder_type       = MMISMS_FOLDER_NORMAL;
    MMI_CTRL_ID_T menu_ctrl_id             = MMIMMS_OUTBOX_OPT_MENU_CTRL_ID;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr  = PNULL;
    MMIMMS_WINDOW_ID_E quer_win_id         = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
#ifdef TTS_SUPPORT            
    MMI_STRING_T tts_read_menu = {0};
#endif    

    edit_doc_ptr = GetMMSReadEditDoc();
    if (edit_doc_ptr != PNULL)
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id); 
    }
    if(PNULL == mms_list_info_ptr)
    {
        MMK_CloseWin(win_id);
        return MMI_RESULT_FALSE;
    }
    //SCI_TRACE_LOW:"MMIMMS: HandleOutboxMMSOptMenuWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_5282_112_2_18_2_38_45_338,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));

        {
            is_locked = mms_list_info_ptr->is_lock;
            folder_type = mms_list_info_ptr->folder_type;
        }        
        if (!is_locked)
        {
            GUIMENU_SetItemGrayed(MMIMMS_OUTBOX_OPT_MENU_CTRL_ID,group_id,ID_MMS_OUTBOX_DEL,FALSE);
        }
        else
        { 
            GUIMENU_SetItemGrayed(MMIMMS_OUTBOX_OPT_MENU_CTRL_ID,group_id,ID_MMS_OUTBOX_DEL,TRUE);
        }

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        if (MMISMS_FOLDER_SECURITY == folder_type)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_SECURITY_ITEM,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_PREVIEW_ITEM,FALSE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_SECURITY_ITEM,FALSE);
            MMIAPICOM_EnableGrayed(win_id,MENU_MMS_SECURITY_SELECT,ID_MMS_MENU_BOX_PREVIEW_ITEM,TRUE);
        }
#endif
#ifdef TTS_SUPPORT            
        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_SMS))
        {
            MMI_GetLabelTextByLang(STXT_STOP, &tts_read_menu);
        }
        else
        {
            MMI_GetLabelTextByLang(TXT_SET_TTS_PLAY, &tts_read_menu);
        }
        GUIMENU_SetStaticItem(MMIMMS_OUTBOX_OPT_MENU_CTRL_ID,group_id,ID_MMS_OUTBOX_PLAY,&tts_read_menu,0);
#endif 
        {
            GUIMENU_POP_SEL_INFO_T  pop_item_info  = {0};
            MMI_MENU_GROUP_ID_T extract_group_id   = MENU_MMS_EXTRACT;
            
            pop_item_info.is_static = TRUE;
            pop_item_info.ctrl_id = menu_ctrl_id;
            if (MMIMMS_Setting_GetExtractContent())
            {
                pop_item_info.menu_id = ID_MMS_INBOX_EXTRACT_YES;
            }
            else
            {
                pop_item_info.menu_id = ID_MMS_INBOX_EXTRACT_NO;
            }
            
            pop_item_info.group_id = extract_group_id;
            
            GUIMENU_SetPopItemStatus(TRUE,&pop_item_info);
        }

        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id)
        {
        case ID_MMS_OUTBOX_SEND:
            {
                {
                    MMIMMS_OutboxSendMMS(mms_list_info_ptr->record_id, edit_doc_ptr);
                }                
                MMK_CloseWin(win_id);
            }
            break;
        case ID_MMS_OUTBOX_PLAY:
            MMK_CloseWin(win_id);
            SetMMSPreviewIsWebPlay(TRUE);
#ifdef TTS_SUPPORT
            if(!MmsIsSuitableForTTS())
            {
                if(!BoxPlayMMS(edit_doc_ptr))
                {
                    break;
                }
            }
            else
            {
                // 如果TTS正在运行
                if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_MMS))
                {
                    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_MMS);
                    //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_SET_TTS_PLAY, STXT_RETURN, TRUE);
                }
                else
                {
                    // 获取MMS的文本内容...
                    MMI_STRING_T    mms_text_string = {0};
                    MMITTS_PLAY_PARAM_T play_param = {0};
                    if (MMIMMS_GetCurrentSlideText(edit_doc_ptr, &mms_text_string))
                    {
                        play_param.txt_addr = (uint8 *)mms_text_string.wstr_ptr;
                        play_param.txt_len_in_byte = mms_text_string.wstr_len * 2;
                        play_param.is_ucs2 = TRUE;
                        play_param.play_times = 1;
                        play_param.tts_vol = 4;
                        play_param.ring_type = TTS_RING_TYPE_MMS;
                        play_param.win_handle = MMIMMS_PREVIEW_WIN_ID;
						play_param.tts_route = MMISRVAUD_ROUTE_AUTO;
						play_param.digit_mode = TTS_DIGIT_AUTO;
						play_param.punc_mode = TTS_PUNC_OFF;
                        if(MMIAPITTS_PlayText(&play_param))
                        {
                            //GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_STOP, STXT_RETURN, TRUE);
                        }
                    }
                }
            }           
#else            
            BoxPlayMMS(edit_doc_ptr);
#endif
            break;            
        case ID_MMS_OUTBOX_EDIT:
            {
                MMIMMS_EDIT_DOCUMENT_T * editdoc_ptr = PNULL;
                // MMIMMS_ERROR_E edit_err              = MMIMMS_NO_ERROR;
                
                MMK_CloseWin(win_id);
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                editdoc_ptr = GetMMSReadEditDoc();
                if (PNULL == editdoc_ptr)
                {
                    //提示彩信初始化失败
                }
                else
                {
                    MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_OUTBOX_READ_TO_EDIT);
                    MMIMMS_AttachmentTextEncodeType(editdoc_ptr);
                    MMIMMS_MMSEditInit(editdoc_ptr, MMIMMS_RICHTEXT_TEXT);
                    MMIMMS_CreateEditWin();
                } 
                
            }
            break;
        case ID_MMS_OUTBOX_DEL:
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            break;

        case ID_MMS_INBOX_EXTRACT_YES:               
            //extract 
            MMIMMS_Setting_SetExtractContent(TRUE);
            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_CloseWin(win_id);
            break;

        case ID_MMS_INBOX_EXTRACT_NO:               
            //extract
            MMIMMS_Setting_SetExtractContent(FALSE);
            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_CloseWin(win_id);
            break;
            
        case ID_MMS_OUTBOX_DETAIL:
            {
                uint32 record_id = 0;

                record_id = mms_list_info_ptr->record_id;      
                MMIMMS_SetPreviewMMSDetailFlag(FALSE);
                MMK_CreateWin((uint32 *)MMIMMS_INBOX_NOTIDETAIL_WIN_TAB, (ADD_DATA)record_id);
                MMK_CloseWin(win_id);
            }            
            break;

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case ID_MMS_MENU_BOX_SECURITY_ITEM:
        case ID_MMS_MENU_BOX_PREVIEW_ITEM:
            MMIPUB_OpenQueryWinByTextId(TXT_MOVE,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            break;
#endif

        default:
            //SCI_TRACE_LOW:"menu_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_5467_112_2_18_2_38_45_339,(uint8*)"d",menu_id);
            break;
        }
        break;
    case MSG_PROMPTWIN_OK:
        GUIMENU_GetId(menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));        
        switch(menu_id)
        {
        case ID_MMS_OUTBOX_DEL:
            {
                MMK_CloseWin(quer_win_id);
                MMIMMS_DeleteMMSAndSaveList(mms_list_info_ptr->record_id);
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
                MMK_CloseWin(win_id);
                if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
                }
            }
            break;

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case ID_MMS_MENU_BOX_SECURITY_ITEM:
            MMIAPIMMS_MoveSecurityMMS(mms_list_info_ptr->record_id, TRUE);
            MMK_CloseWin(win_id);
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
            }
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
            break;

        case ID_MMS_MENU_BOX_PREVIEW_ITEM:
            MMIAPIMMS_MoveSecurityMMS(mms_list_info_ptr->record_id, FALSE);
            MMK_CloseWin(win_id);
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
            }
#ifdef MMI_GUI_STYLE_TYPICAL
            MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);
#endif
            break;
#endif

        default:
            break;
        }        
        break;        
    case MSG_PROMPTWIN_CANCEL:
        //close query window
        MMK_CloseWin(quer_win_id);
        break;            
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        // close the option window
        MMK_CloseWin(win_id);
        break;            
    case MSG_CLOSE_WINDOW:
        break;            
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    return err_code;
}

/*****************************************************************************/
//  Description :mms save multifile menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSaveMultiFileWinMsg(
                                              MMIMMS_WINDOW_ID_E win_id, 
                                              MMI_MESSAGE_ID_E msg_id, 
                                              DPARAM param
                                              )
{
    MMI_RESULT_E   err_code                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_ctrl_id                       = MMIMMS_SAVE_MULTIFILE_LIST_CTRL_ID;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr            = PNULL;
    P_MMS_SLIDE_T p_mms_slide_temp                   = PNULL;
    MMS_ACCESSORY_PARAM_T *accessory_ptr             = PNULL;
    P_MMS_CONTENT_T p_mms_content_temp               = PNULL;
    static uint16    select_index                    = 0;
    uint8 *multifile_name_buf_ptr                    = PNULL;
    uint16 multifile_name_len                        = 0;
    uint16 multifile_name_wlen                       = 0;
    wchar multifile_name_wstr[MMS_FILE_NAME_LEN + 1] = {0};
    wchar *viewfile_name_wstr                        = PNULL;
    // uint8 *multifile_data_ptr                        = PNULL;
    // uint32 multifile_data_len                        = 0;
    //const GUILIST_ITEM_T *list_item_ptr                    = PNULL;
    // uint32   time_out                                = 0;
    // MMI_WIN_ID_T  alert_win_id                       = MMIMULTIM_DOWANLOAD_WAIT_WIN_ID;
    uint32    usc_len                                = 0;
    uint16  item_num                                 = 0;
    uint16 suffix_wstr[MMIMMS_SUFFIX_LEN + 1]        = {0};
    uint16 suffix_len                                = MMIMMS_SUFFIX_LEN;
    wchar  suffix_text[]                             = {'t','x','t',0};
    MMIFMM_FILE_TYPE_E file_type                     = MMIFMM_FILE_TYPE_NORMAL;
#if (!defined(MMI_GUI_STYLE_TYPICAL)) || defined(MMI_MMS_MINI_SUPPORT)
    uint8 multi_temp[MMS_FILE_NAME_LEN*3 + 1]        = {0};
    MMIMMS_ERROR_E mms_op_res                        = MMIMMS_NO_ERROR;
    BOOLEAN is_content_text                          = FALSE;
    MMI_STRING_T save_file_to_str   = {0};
    MMI_WIN_ID_T  alert_win_id = MMIMMS_ALERT_WIN_ID;
#endif
    wchar *save_file_name_ptr  = PNULL;
    uint16 file_index = 0;
    uint32 item_data = 0;

    //SCI_TRACE_LOW:"MMIMMS: HandleSaveMultiFileWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_5578_112_2_18_2_38_45_340,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        MMIMMS_CLEAR_CLIENT(win_id);
        item_num = GUILIST_GetTotalItemNum(menu_ctrl_id);
        if (0 == item_num)
        {
            GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL,TXT_NULL,STXT_RETURN,FALSE);
        }
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
    case MSG_OPEN_WINDOW:
        //提取媒体文件，以列表的格式显示
        {
            GUILIST_SetMaxItem(menu_ctrl_id, MMIMMS_MULTIFILE_LIST_MAX_ITEM, FALSE );            
            select_index = 0;
            edit_doc_ptr = GetMMSReadEditDoc();
            if (PNULL != edit_doc_ptr)
            {
                p_mms_slide_temp = edit_doc_ptr->editbody_ptr->slide_root;
                while (p_mms_slide_temp)
                {
                    if (1 == p_mms_slide_temp->text_flag)
                    {
                        p_mms_content_temp = p_mms_slide_temp->content_root;
                        while (p_mms_content_temp)//get text name
                        {
                            if (MMS_TYPE_TEXT == p_mms_content_temp->type)
                            {
                                AddMultifileItemToList(p_mms_content_temp, menu_ctrl_id);
                                break;
                            }
                            p_mms_content_temp = p_mms_content_temp->next;
                        }
                    }
                    
                    if (1 == p_mms_slide_temp->video_flag)
                    {
                        p_mms_content_temp = p_mms_slide_temp->content_root;
                        while (p_mms_content_temp)//get video name
                        {
                            if (MMS_TYPE_MP4 ==  p_mms_content_temp->type || 
                                MMS_TYPE_3GP == p_mms_content_temp->type  ||
                                MMS_TYPE_AVI == p_mms_content_temp->type)
                            {
                                AddMultifileItemToList(p_mms_content_temp, menu_ctrl_id);
                                break;
                            }
                            p_mms_content_temp = p_mms_content_temp->next;
                        }
                    }
                    else if (1 == p_mms_slide_temp->image_flag && 0 != p_mms_slide_temp->audiosize)
                    {
                        p_mms_content_temp = p_mms_slide_temp->content_root;
                        while (p_mms_content_temp)//get image name
                        {
                            if (MMS_TYPE_TEXT != p_mms_content_temp->type)
                            {
                                AddMultifileItemToList(p_mms_content_temp, menu_ctrl_id);
                                break;
                            }                            
                            p_mms_content_temp = p_mms_content_temp->next;
                        }
                        //get music name
                        multifile_name_buf_ptr = (uint8 *)p_mms_slide_temp->audiofile; 
                        multifile_name_len = (uint16)strlen((char *)multifile_name_buf_ptr);
                        //MMI_STRNTOWSTR(multifile_name_wstr, MMS_FILE_NAME_LEN, (char *)multifile_name_buf_ptr, multifile_name_len, multifile_name_len);
                        multifile_name_wlen = GUI_UTF8ToWstr(multifile_name_wstr, multifile_name_len, multifile_name_buf_ptr, multifile_name_len);
                        AppendListItem(multifile_name_wstr,
                            multifile_name_wlen,
                            menu_ctrl_id,
                            STXT_SAVE);
                    }
                    else if (0 != p_mms_slide_temp->audiosize && 1 != p_mms_slide_temp->image_flag)
                    {
                        multifile_name_buf_ptr = (uint8 *)p_mms_slide_temp->audiofile; 
                        multifile_name_len = (uint16)strlen((char *)multifile_name_buf_ptr);
                        //MMI_STRNTOWSTR(multifile_name_wstr, MMS_FILE_NAME_LEN, (char *)multifile_name_buf_ptr, multifile_name_len, multifile_name_len);
                        multifile_name_wlen = GUI_UTF8ToWstr(multifile_name_wstr, multifile_name_len, multifile_name_buf_ptr, multifile_name_len);
                        AppendListItem(multifile_name_wstr,
                            multifile_name_wlen,
                            menu_ctrl_id,
                            STXT_SAVE);
                    }
                    else if (1 == p_mms_slide_temp->image_flag && 0 == p_mms_slide_temp->audiosize)
                    {
                        p_mms_content_temp = p_mms_slide_temp->content_root;
                        while (p_mms_content_temp)//get image name
                        {
                            if (MMS_TYPE_TEXT != p_mms_content_temp->type)
                            {
                                AddMultifileItemToList(p_mms_content_temp, menu_ctrl_id);
                                break;
                            }
                            p_mms_content_temp = p_mms_content_temp->next;
                        }
                    }
                    p_mms_slide_temp = p_mms_slide_temp->next;
                }
                accessory_ptr = edit_doc_ptr->editbody_ptr->accessory_ptr;                
                while (PNULL != accessory_ptr)
                {
                    multifile_name_buf_ptr = (uint8 *)accessory_ptr->filename;
                    multifile_name_len = (uint16)strlen((char *)multifile_name_buf_ptr);
                    //MMI_STRNTOWSTR(multifile_name_wstr, MMS_FILE_NAME_LEN, (char *)multifile_name_buf_ptr, multifile_name_len, multifile_name_len);
                    multifile_name_wlen = GUI_UTF8ToWstr(multifile_name_wstr, multifile_name_len, multifile_name_buf_ptr, multifile_name_len);
                    AppendListItem(multifile_name_wstr,
                        multifile_name_wlen,
                        menu_ctrl_id,
                            STXT_SAVE);
                    accessory_ptr = accessory_ptr->next;
                }
            }
            //GUILIST_SetCurItemIndex(menu_ctrl_id, 0);
        }       
        break;
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        {
            item_num = GUILIST_GetTotalItemNum(menu_ctrl_id);
            if (0 == item_num)
            {
                break;
            }
            select_index = GUILIST_GetCurItemIndex(menu_ctrl_id);
         //   list_item_ptr = GUILIST_GetItemPtrByIndex(menu_ctrl_id, select_index);

            if (!GUILIST_GetItemData(menu_ctrl_id, select_index, &item_data))
            {
                break;
            }
            save_file_name_ptr = (wchar *)item_data;
            usc_len = MMIAPICOM_Wstrlen(save_file_name_ptr);
           
            viewfile_name_wstr = (wchar *)SCI_ALLOC_APPZ((usc_len + 1) * sizeof(wchar));
            //SCI_MEMSET((char *)viewfile_name_wstr, 0, ((usc_len + 1) * sizeof(wchar)));
            MMIAPICOM_Wstrncpy(viewfile_name_wstr, save_file_name_ptr, usc_len);            
            MMIAPIFMM_SplitFileName(viewfile_name_wstr, usc_len, PNULL, PNULL, suffix_wstr, &suffix_len);
            file_type = MMIAPIFMM_ConvertFileType(suffix_wstr, suffix_len);
            
            if (   MMIFMM_FILE_TYPE_PICTURE == file_type
                || MMIFMM_FILE_TYPE_MUSIC == file_type
                || MMIFMM_FILE_TYPE_MOVIE == file_type
#ifdef EBOOK_SUPPORT
                || (MMIFMM_FILE_TYPE_EBOOK == file_type && !MMIAPICOM_CompareTwoWstrExt(suffix_wstr, suffix_len, suffix_text, MMIAPICOM_Wstrlen(suffix_text), FALSE))
#endif
#ifdef MMI_VCARD_SUPPORT
                || MMIFMM_FILE_TYPE_VCARD == file_type
#endif
                )
            {
                MMK_CreateWin((uint32*)MMIMMS_MULTIFILE_VIEW_TAB, (ADD_DATA)viewfile_name_wstr);
            }            
#ifdef MMI_VCALENDAR_SUPPORT
            else if (MMIFMM_FILE_TYPE_VCALENDAR == file_type)
            {
                uint8 multi_name[MMS_FILE_NAME_LEN*3 + 1] = {0};
                uint8 *data_buf_ptr = PNULL;
                uint32 data_buf_len = 0;

                GUI_WstrToUTF8(multi_name, MMS_FILE_NAME_LEN*3, viewfile_name_wstr, MMIAPICOM_Wstrlen(viewfile_name_wstr));
                MMIMMS_GetContentByName(multi_name, &data_buf_ptr, &data_buf_len);

                MMIAPISCH_ReadVCalendarData(data_buf_ptr, data_buf_len);

                SCI_FREE(viewfile_name_wstr);
                viewfile_name_wstr = PNULL;
            }
#endif
        }
        break;
        
#ifdef MMI_PDA_SUPPORT
#ifndef MMI_GUI_STYLE_TYPICAL
    case MSG_CTL_LIST_LONGOK:    
        MMK_CreateWin((uint32*)MMIMMS_SAVE_FILE_OPT_TAB,PNULL);
        break;
#endif
#endif
    
    case MSG_CTL_OK:
    case MSG_APP_OK:
#if (!defined(MMI_GUI_STYLE_TYPICAL)) || defined(MMI_MMS_MINI_SUPPORT)
        //保存当前焦点的媒体文件进入相应的默认目录下面，首先要找到该文件，通过名称来找文件
        {  
            wchar file_save_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};
            MMI_STRING_T alert_string   = {0};  
            item_num = GUILIST_GetTotalItemNum(menu_ctrl_id);
            if (0 == item_num)
            {
                break;
            }
            select_index = GUILIST_GetCurItemIndex(menu_ctrl_id);
          //  list_item_ptr = GUILIST_GetItemPtrByIndex(menu_ctrl_id, select_index);
            
            if (!GUILIST_GetItemData(menu_ctrl_id, select_index, &item_data))
            {
                break;
            }
            save_file_name_ptr = (wchar *)item_data;
            usc_len = MMIAPICOM_Wstrlen(save_file_name_ptr);

            //MMI_WSTRNTOSTR((char *)multi_temp, MMS_FILE_NAME_LEN, multifile_name_wstr, MMS_FILE_NAME_LEN, MMIAPICOM_Wstrlen(multifile_name_wstr));
            GUI_WstrToUTF8(multi_temp, MMS_FILE_NAME_LEN*3, save_file_name_ptr,  MMIAPICOM_Wstrlen(save_file_name_ptr));
            {//判断是不是text
                edit_doc_ptr = GetMMSReadEditDoc();
                is_content_text = FALSE;
                if (PNULL != edit_doc_ptr)
                {
                    p_mms_slide_temp = edit_doc_ptr->editbody_ptr->slide_root;
                    while (p_mms_slide_temp)
                    {
                        p_mms_content_temp = p_mms_slide_temp->content_root;
                        while (p_mms_content_temp)
                        {
                            if (MMS_TYPE_TEXT == p_mms_content_temp->type && (0 == strcmp((char *)multi_temp, (char *)p_mms_content_temp->filename)))
                            {
                                is_content_text = TRUE;
                                break;
                            }
                            p_mms_content_temp = p_mms_content_temp->next;
                        }
                        if (is_content_text)
                        {
                            break;
                        }
                        else
                        {
                            p_mms_slide_temp = p_mms_slide_temp->next;
                        }                        
                    }

                }
            }

            if (!is_content_text)
            {
                MMIAPIFMM_SplitFileName(save_file_name_ptr, usc_len, PNULL, PNULL, suffix_wstr, &suffix_len);
                file_type = MMIAPIFMM_ConvertFileType(suffix_wstr, suffix_len);
                if (MMIFMM_FILE_TYPE_EBOOK == file_type && !MMIAPICOM_Wstrcmp(suffix_wstr, suffix_text))
                {
                    is_content_text = TRUE;
                }
            }
            
            mms_op_res = MMIMMS_SaveMMSContent(multi_temp, is_content_text, file_save_path
#ifdef DRM_SUPPORT
                ,select_index
#endif
                );
#ifdef DRM_SUPPORT
            if(MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(multi_temp) || MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(multi_temp))
            {
                break;
            }
#endif
            if (MMIMMS_NO_ERROR != mms_op_res)
            {
				//没有保存成功需要删除文件
				MMIAPIFMM_DeleteFile(file_save_path, PNULL);//CR190185
                if (MMIMMS_ERROR_NO_SPACE == mms_op_res)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else if (MMIMMS_ERROR_FILENAME == mms_op_res)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_FILE_NAME_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else
                {
                    MMIMMS_EditMMSAlertWin(mms_op_res);
                }                
                MMK_CloseWin(win_id);
                break;
            }
            if (MMK_IsOpenWin(alert_win_id))
            {
                MMK_CloseWin(alert_win_id);
            }
            MMI_GetLabelTextByLang(TXT_COMM_FILE_SAVE_TO, &save_file_to_str);
            alert_string.wstr_ptr = file_save_path;
            alert_string.wstr_len = MMIAPICOM_Wstrlen(file_save_path);
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&save_file_to_str,&alert_string,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
#endif
        break; 
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //退出媒体保存界面，回到阅读opt界面
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        {
            item_num = GUILIST_GetTotalItemNum(menu_ctrl_id);
            if (0 == item_num)
            {
                break;
            }
            for (file_index = 0; file_index < item_num; file_index++)
            {
                if (GUILIST_GetItemData(menu_ctrl_id, file_index, &item_data))
                {
                    save_file_name_ptr = (wchar *)item_data;
                    SCI_FREE(save_file_name_ptr);
                    save_file_name_ptr = PNULL;
                }
            }
        }
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :mms save multifile view detail handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMultifileViewWinMsg(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                              )
{
    MMI_RESULT_E   err_code                          = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T menu_ctrl_id                       = MMIMMS_MULTIFILE_VIEW_CTRL_ID;
    static MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr     = PNULL;
    static P_MMS_SLIDE_T p_mms_slide_temp            = PNULL;
    P_MMS_CONTENT_T p_mms_content_temp               = PNULL;
    MMIMMS_ERROR_E mms_op_res                        = MMIMMS_NO_ERROR;
    uint8 multi_temp[MMS_FILE_NAME_LEN* 3 + 1]       = {0};
    BOOLEAN is_content_text                          = FALSE;
    wchar *viewfile_name_wstr                        = (wchar *)(MMK_GetWinAddDataPtr(win_id));
    uint16 suffix_wstr[MMIMMS_SUFFIX_LEN + 1]        = {0};
    uint16 suffix_len                                = MMIMMS_SUFFIX_LEN;
    wchar  suffix_text[]                             = {'t','x','t',0};
    MMIFMM_FILE_TYPE_E file_type                     = MMIFMM_FILE_TYPE_NORMAL;    
    wchar save_alert_str[MMIFILE_FULL_PATH_MAX_LEN]  = {0}; 
    MMI_STRING_T alert_string   = {0};
    MMI_STRING_T save_file_to_str   = {0};
    MMI_WIN_ID_T  alert_win_id = MMIMMS_ALERT_WIN_ID;

    //SCI_TRACE_LOW:"MMIMMS: HandleMultifileViewWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_5902_112_2_18_2_38_46_341,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        if (!GetMMSPreviewIsPlayVideo())
        {
            MMIMMS_CLEAR_CLIENT(win_id);
        }        
        break;
    case MSG_OPEN_WINDOW:
        {   
            if (PNULL == viewfile_name_wstr)
            {
                break;
            }
            MMIAPIFMM_SplitFileName(viewfile_name_wstr, MMIAPICOM_Wstrlen(viewfile_name_wstr), PNULL, PNULL, suffix_wstr, &suffix_len);
            file_type = MMIAPIFMM_ConvertFileType(suffix_wstr, suffix_len);
            if (MMIFMM_FILE_TYPE_MOVIE == file_type)
            {
                MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);
            }
            else
            {
                ViewMultifile(viewfile_name_wstr);
            }
            
            MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        }       
        break;

    case MSG_MMS_PLAY_VIDEO:
        if (!ViewMultifile(viewfile_name_wstr))
        {
            MMK_CloseWin(win_id);           
        }        
        break;
    case MSG_LOSE_FOCUS:
        MmsStopPreviewRing();
#ifdef VIDEO_PLAYER_SUPPORT
        if (GetMMSPreviewIsPlayVideo())
        {
            MMIAPIVP_ExitPlay();
        }
#endif
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
        //保存当前媒体文件关闭浏览窗口
        {
            wchar file_save_path[MMIFILE_FULL_PATH_MAX_LEN+1] = {0};

            if (GetMMSPreviewIsPlayVideo())
            {
#ifdef VIDEO_PLAYER_SUPPORT
                MMIAPIVP_ExitPlay();
#endif
                SetMMSPreviewIsPlayVideo(FALSE);
                break;
            }
            if (PNULL == viewfile_name_wstr)
            {
                break;
            }

            //MMI_WSTRNTOSTR((char *)multi_temp, MMS_FILE_NAME_LEN, viewfile_name_wstr, MMS_FILE_NAME_LEN, MMIAPICOM_Wstrlen(viewfile_name_wstr));
            GUI_WstrToUTF8(multi_temp, MMS_FILE_NAME_LEN*3, viewfile_name_wstr,  MMIAPICOM_Wstrlen(viewfile_name_wstr));
            {//判断是不是text
                edit_doc_ptr = GetMMSReadEditDoc();
                is_content_text = FALSE;
                if (PNULL != edit_doc_ptr)
                {
                    p_mms_slide_temp = edit_doc_ptr->editbody_ptr->slide_root;
                    while (p_mms_slide_temp)
                    {
                        p_mms_content_temp = p_mms_slide_temp->content_root;
                        while (p_mms_content_temp)
                        {
                            if (MMS_TYPE_TEXT == p_mms_content_temp->type && (0 == strcmp((char *)multi_temp, (char *)p_mms_content_temp->filename)))
                            {
                                is_content_text = TRUE;
                                break;
                            }
                            p_mms_content_temp = p_mms_content_temp->next;
                        }
                        if (is_content_text)
                        {
                            break;
                        }
                        else
                        {
                            p_mms_slide_temp = p_mms_slide_temp->next;
                        }                        
                    }

                }
            }

            if (!is_content_text)
            {
                MMIAPIFMM_SplitFileName(viewfile_name_wstr, MMIAPICOM_Wstrlen(viewfile_name_wstr), PNULL, PNULL, suffix_wstr, &suffix_len);
                file_type = MMIAPIFMM_ConvertFileType(suffix_wstr, suffix_len);            
                if (MMIFMM_FILE_TYPE_EBOOK == file_type && !MMIAPICOM_CompareTwoWstrExt(suffix_wstr, suffix_len, suffix_text, MMIAPICOM_Wstrlen(suffix_text), FALSE))
                {
                    is_content_text = TRUE;                    
                }
            }
            
            mms_op_res = MMIMMS_SaveMMSContent(multi_temp, is_content_text, file_save_path
#ifdef DRM_SUPPORT
                ,0
#endif
                );
            if (MMIMMS_NO_ERROR != mms_op_res)
            {
                if (MMIMMS_ERROR_NO_SPACE == mms_op_res)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_NO_SPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else if (MMIMMS_ERROR_FILENAME == mms_op_res)
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_FILE_NAME_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                }
                else
                {
                    MMIMMS_EditMMSAlertWin(mms_op_res);
                }                
                MMK_CloseWin(win_id);
                break;
            }  
            if (MMK_IsOpenWin(alert_win_id))
            {
                MMK_CloseWin(alert_win_id);
            }
            MMI_GetLabelTextByLang(TXT_COMM_FILE_SAVE_TO, &save_file_to_str);
            MMIAPICOM_Wstrncpy(save_alert_str, save_file_to_str.wstr_ptr, save_file_to_str.wstr_len);
            MMIAPICOM_Wstrncpy(save_alert_str + save_file_to_str.wstr_len, file_save_path, MMIAPICOM_Wstrlen(file_save_path));
            alert_string.wstr_ptr = save_alert_str;
            alert_string.wstr_len = MMIAPICOM_Wstrlen(save_alert_str);
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&alert_string,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_win_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(win_id);
         }
        break; 
#ifdef VIDEO_PLAYER_SUPPORT
    case MSG_MMS_END_VIDEO:
        MMIAPIVP_ExitPlay();
        break;
#endif
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //退出媒体保存界面，回到阅读opt界面
        if (GetMMSPreviewIsPlayVideo())
        {
#ifdef VIDEO_PLAYER_SUPPORT
            MMIAPIVP_ExitPlay();
#endif
            SetMMSPreviewIsPlayVideo(FALSE);
            break;
        }
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if (PNULL != viewfile_name_wstr)
        {
            SCI_FREE(viewfile_name_wstr);
            viewfile_name_wstr = PNULL;
        }

        MmsStopPreviewRing();

        if (GetMMSPreviewIsPlayVideo())
        {
#ifdef VIDEO_PLAYER_SUPPORT
            MMIAPIVP_ExitPlay();
#endif
            SetMMSPreviewIsPlayVideo(FALSE);
        }
        break;
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description :view the current multifile
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 该函数显示的是彩信保存多媒体文件的时候查看单个问题的具体内容，图片
//        就显示该图片，声音和视频显示名称并且播放，text显示该text，其他附件就显示名称
/*****************************************************************************/
LOCAL BOOLEAN ViewMultifile(wchar *file_name_str)
{
    uint8 multi_name[MMS_FILE_NAME_LEN*3 + 1] = {0};
    // MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr     = PNULL;
    MMIFMM_FILE_TYPE_E file_type              = MMIFMM_FILE_TYPE_NORMAL;
    uint16 suffix_wstr[MMIMMS_SUFFIX_LEN + 1] = {0};
    uint16 suffix_len                         = MMIMMS_SUFFIX_LEN;
    uint32 temp_file_tpye                     = 0;
    GUIRICHTEXT_ITEM_T item                   = {0};/*lint !e64*/
    uint16 index                              = 0;
    uint8 *data_buf_ptr                       = PNULL;
    uint32 data_buf_len                       = 0;
    MMISRVAUD_RING_FMT_E  fmt               = MMISRVAUD_RING_FMT_MAX;
    uint8 voice_volume                        = MMIMMS_MULTIFILE_VOL_VALUE;
#ifdef VIDEO_PLAYER_SUPPORT
    VP_PLAY_PARAM_T                 play_param  = {0};
    GUI_RECT_T                      full_rect = {0};
#endif
    uint16 pic_width                          = 0;
    uint16 pic_height                         = 0;
    GUICTRL_STATE_T dispay_state              = 0;
    MMI_HANDLE_T view_ctrl_handle             = NULL;

    MMIMMS_GetPicDefaultWidthAndHeight(&pic_width, &pic_height);
    //获得文件类型
    MMIAPIFMM_SplitFileName(file_name_str, MMIAPICOM_Wstrlen(file_name_str), PNULL, PNULL, suffix_wstr, &suffix_len);
    file_type = MMIAPIFMM_ConvertFileType(suffix_wstr, suffix_len);

    //MMI_WSTRNTOSTR((char *)multi_name, MMS_FILE_NAME_LEN, file_name_str, MMS_FILE_NAME_LEN, MMIAPICOM_Wstrlen(file_name_str));
    GUI_WstrToUTF8(multi_name, MMS_FILE_NAME_LEN*3, file_name_str, MMIAPICOM_Wstrlen(file_name_str));

    if (MMIFMM_FILE_TYPE_PICTURE == file_type)
    {
        MMIMMS_GetContentByName(multi_name, &data_buf_ptr, &data_buf_len);
        if (PNULL != data_buf_ptr && 0 != data_buf_len)
        {   
            item.img_type = GUIRICHTEXT_IMAGE_BUF;
            item.img_data.buf.buf_ptr = data_buf_ptr;
            item.img_data.buf.size = data_buf_len;
            item.tag_type = GUIRICHTEXT_TAG_NONE;
            GUIRICHTEXT_AddItem(MMIMMS_MULTIFILE_VIEW_CTRL_ID, &item, &index);
        }
    }
    else if (MMIFMM_FILE_TYPE_MUSIC == file_type)
    {
        item.img_type = GUIRICHTEXT_IMAGE_RES;
        item.img_data.res.id = IMAGE_SECMENU_ICON_MULTIM_MP3;//显示资源图片：音频.bmp            
        item.tag_type = GUIRICHTEXT_TAG_NONE;
        item.align_type = GUIRICHTEXT_ALIGN_DEFAULT;            
        item.text_type = GUIRICHTEXT_TEXT_BUF;          
        item.text_data.buf.str_ptr = file_name_str;
        item.text_data.buf.len = MMIAPICOM_Wstrlen(file_name_str);
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
        item.text_set_font = TRUE;
        item.setting_font = MMIAPISMS_GetWinFont();
#endif
        GUIRICHTEXT_AddItem(MMIMMS_MULTIFILE_VIEW_CTRL_ID, &item, &index);
        
        temp_file_tpye = MMIAPIFMM_GetFileType(suffix_wstr, suffix_len);
        switch(temp_file_tpye) 
        {
        case MMIFMM_MUSIC_MP3:
            fmt = MMISRVAUD_RING_FMT_MP3;
            break;
        case MMIFMM_MUSIC_WMA:
            fmt = MMISRVAUD_RING_FMT_WMA;
            break;
        //case MMIFMM_MUSIC_MIDI:==MMIFMM_MUSIC_MID
        case MMIFMM_MUSIC_MID:
            fmt = MMISRVAUD_RING_FMT_MIDI;
            break;
        case MMIFMM_MUSIC_AMR:
            fmt = MMISRVAUD_RING_FMT_AMR;
            break;
        case MMIFMM_MUSIC_ACC:
            fmt = MMISRVAUD_RING_FMT_AAC;
            break;
        case MMIFMM_MUSIC_M4A:
            fmt = MMISRVAUD_RING_FMT_M4A;
            break;
        case MMIFMM_MUSIC_WAV:
            fmt = MMISRVAUD_RING_FMT_WAVE;
            break;
//        case MMIFMM_MOVIE_KUR:
//            //fmt = MMISRVAUD_RING_FMT_MP3;
//            break;
        default:
            break;
        }
        MMIMMS_GetContentByName(multi_name, &data_buf_ptr, &data_buf_len);
        if (PNULL != data_buf_ptr && 0 != data_buf_len)
        {
            if(MMISRVAUD_RING_FMT_MAX != fmt)
            {
                if (MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_OTHER))
                {   
                    MMIAPISET_PlayRingByPtr(MMISET_RING_TYPE_OTHER,
                        MMIMMS_MULTIFILE_VOL_VALUE,
                        data_buf_ptr,
                        data_buf_len,
                        1,
                        fmt,
                        MmsRingPlayCallBack
                        );
                }
                else
                {
                    //SCI_TRACE_LOW:"not permit play ring!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_6199_112_2_18_2_38_46_342,(uint8*)"");
                    MmsStopPreviewRing();
                }
            }
        }
    }
#ifdef VIDEO_PLAYER_SUPPORT
    else if (MMIFMM_FILE_TYPE_MOVIE == file_type)
    {
        item.text_type = GUIRICHTEXT_TEXT_BUF;
        item.text_data.buf.str_ptr = file_name_str;
        item.text_data.buf.len = MMIAPICOM_Wstrlen(file_name_str);
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
        item.text_set_font = TRUE;
        item.setting_font = MMIAPISMS_GetWinFont();
#endif
        GUIRICHTEXT_AddItem(MMIMMS_MULTIFILE_VIEW_CTRL_ID, &item, &index);

        MMIMMS_GetContentByName(multi_name, &data_buf_ptr, &data_buf_len);
        if (PNULL != data_buf_ptr && 0 != data_buf_len)
        {
            if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {
                MMSMMI_OpenMMSAlertWin(TXT_MMS_IN_CALL, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                return FALSE;
            }
            else if (MMIAPIMTV_IsMTVRunning())
            {
                MMSMMI_OpenMMSAlertWin(TXT_EXIT_MOBILETV, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                return FALSE;
            }
#ifdef BROWSER_SUPPORT
            else if (MMIAPIBROWSER_IsRunning())
            {
                MMSMMI_OpenMMSAlertWin(TXT_BROWSER_VEDIO_NOT_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);                
                return FALSE;
            }
#endif
#ifdef VIDEO_PLAYER_SUPPORT 
			else if (MMIAPIVP_IsOpenPlayerWin())
	        {
                MMSMMI_OpenMMSAlertWin(TXT_VP_VEDIO_NOT_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);	            
	            return FALSE;
	        }       
#endif
            else
            {
                if (!MMIMMS_Setting_GetPlaysound())
                {
                    voice_volume = MMISET_VOL_ZERO;
                }
                SetMMSPreviewIsPlayVideo(TRUE);
                
                //不显示title
                MMIMMS_SetWinStatusBarVisible(MMIMMS_MULTIFILE_VIEW_WIN_ID,FALSE);                
                
                //关闭3d效果
                //MMI_Enable3DMMI(FALSE); 
#ifdef UI_P3D_SUPPORT
                MMI_Disable3DEffect(MMI_3D_EFFECT_MMS_PREVIEW);
#endif            
                MMIAPISET_StopAllRing(FALSE);
                
                play_param.play_type = VP_PLAY_FROM_BUFFER;  
	            play_param.display_param.audioSupport = 1;	    
	            play_param.display_param.videoSupport = 1;	            
	            play_param.is_scal_up = TRUE;
	            play_param.display_param.disp_rect.x = 0;
	            play_param.display_param.disp_rect.y = 0;                
	            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_3GP;
	            play_param.video_buf_info.video_buf_len = data_buf_len;
	            play_param.video_buf_info.video_buf_ptr = data_buf_ptr;
                
                play_param.display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID); 
                full_rect = MMITHEME_GetFullScreenRect(); 
                if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
                {
                    play_param.display_param.RotateAngle = LCD_ANGLE_90;
                    play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                    play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
                }
                else if(LCD_ANGLE_180 == play_param.display_param.RotateAngle)
                {
                    play_param.display_param.RotateAngle = LCD_ANGLE_270;
                    play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                    play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
                }
                else
                {
                    play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
                    play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
                }

                           
                

                play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
                play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
                play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
                play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;

                play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;
                
                play_param.vol = voice_volume;

	                       
	            if(MMIAPIVP_Play(&play_param,MmsVideoPlayCallBack,0))
                {
                    dispay_state = GUICTRL_STATE_DISABLE_TP;
                    view_ctrl_handle = MMK_GetCtrlHandleByWin(MMIMMS_MULTIFILE_VIEW_WIN_ID, MMIMMS_MULTIFILE_VIEW_CTRL_ID);
                    IGUICTRL_SetState(MMK_GetCtrlPtr(view_ctrl_handle), dispay_state, TRUE);
                    GUIWIN_SetSoftkeyTPEnable(MMIMMS_MULTIFILE_VIEW_WIN_ID, FALSE);
                    MMK_SetWinSupportAngle(MMIMMS_MULTIFILE_VIEW_WIN_ID, WIN_SUPPORT_ANGLE_CUR);
                }              
                return TRUE;
            }
        }
    }
#endif
#ifdef EBOOK_SUPPORT
    else if (MMIFMM_FILE_TYPE_EBOOK == file_type)
    {
        MMIMMS_GetContentByName(multi_name, &data_buf_ptr, &data_buf_len);
        if (PNULL != data_buf_ptr && 0 != data_buf_len)
        {
            item.text_type = GUIRICHTEXT_TEXT_BUF;
            item.text_data.buf.str_ptr = (wchar*)(data_buf_ptr+sizeof(uint32));
            //item.text_data.buf.len = (*(uint16 *)data_buf_ptr)/2;
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            item.text_set_font = TRUE;
            item.setting_font = MMIAPISMS_GetWinFont();
#endif
            item.text_data.buf.len = MMIAPICOM_Wstrlen(item.text_data.buf.str_ptr);
            item.text_data.buf.len = MIN(item.text_data.buf.len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);            

            //SCI_TRACE_LOW:"MMIMMS: item.text_data.buf.len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_6331_112_2_18_2_38_47_343,(uint8*)"d", item.text_data.buf.len);
            //SCI_TRACE_LOW:"MMIMMS: ViewMultifile data %x %x %x %x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_6332_112_2_18_2_38_47_344,(uint8*)"dddd", *(uint8 *)item.text_data.buf.str_ptr, *((uint8 *)item.text_data.buf.str_ptr + 1), *((uint8 *)item.text_data.buf.str_ptr + 2), *((uint8 *)item.text_data.buf.str_ptr + 3));

            if (0 < item.text_data.buf.len)
            {
                GUIRICHTEXT_AddItem(MMIMMS_MULTIFILE_VIEW_CTRL_ID, &item, &index);
            }
            else
            {
                MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);                
            }
        }        
    }
#endif
#ifdef  MMI_VCARD_SUPPORT
    else if (MMIFMM_FILE_TYPE_VCARD == file_type)
    {
        MMIMMS_GetContentByName(multi_name, &data_buf_ptr, &data_buf_len);
        if (PNULL != data_buf_ptr && 0 != data_buf_len)
        {
            wchar *vcard_content_ptr = PNULL;

            vcard_content_ptr = SCI_ALLOCAZ((data_buf_len + 1) * sizeof(wchar));

            if (PNULL == vcard_content_ptr)
            {
                return FALSE;
            }

            MMI_STRNTOWSTR(vcard_content_ptr, data_buf_len, data_buf_ptr, data_buf_len, data_buf_len);
        
            item.text_type = GUIRICHTEXT_TEXT_BUF;
            item.text_data.buf.str_ptr = vcard_content_ptr;
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            item.text_set_font = TRUE;
            item.setting_font = MMIAPISMS_GetWinFont();
#endif
            item.text_data.buf.len = MMIAPICOM_Wstrlen(item.text_data.buf.str_ptr);
            item.text_data.buf.len = MIN(item.text_data.buf.len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);            

            if (0 < item.text_data.buf.len)
            {
                GUIRICHTEXT_AddItem(MMIMMS_MULTIFILE_VIEW_CTRL_ID, &item, &index);
            }
            else
            {
                MMSMMI_OpenMMSAlertWin(TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_WARNING);
            }

            SCI_FREE(vcard_content_ptr);
        }        
    }
#endif
    else
    {
        item.text_type = GUIRICHTEXT_TEXT_BUF;
        item.text_data.buf.str_ptr = file_name_str;
        item.text_data.buf.len = MMIAPICOM_Wstrlen(file_name_str);
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
        item.text_set_font = TRUE;
        item.setting_font = MMIAPISMS_GetWinFont();
#endif
        GUIRICHTEXT_AddItem(MMIMMS_MULTIFILE_VIEW_CTRL_ID, &item, &index);
    }

    return TRUE;
}

/*****************************************************************************/
//  Description :add list item in list box
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AddMultifileItemToList(P_MMS_CONTENT_T mms_content_ptr, MMI_CTRL_ID_T menu_ctrl_id)
{
    
    uint8 *multifile_name_buf_ptr                    = PNULL;
    uint16 multifile_name_len                        = 0;
    uint16 multifile_name_wlen                       = 0;
    wchar multifile_name_wstr[MMS_FILE_NAME_LEN + 1] = {0};
    
    multifile_name_buf_ptr = (uint8 *)mms_content_ptr->filename;
    multifile_name_len = (uint16)strlen((char *)multifile_name_buf_ptr);
    //MMI_STRNTOWSTR(multifile_name_wstr, MMS_FILE_NAME_LEN, (char *)multifile_name_buf_ptr, multifile_name_len, multifile_name_len);
    multifile_name_wlen = GUI_UTF8ToWstr(multifile_name_wstr, multifile_name_len,multifile_name_buf_ptr, multifile_name_len);
    AppendListItem(multifile_name_wstr,
        multifile_name_wlen,
        menu_ctrl_id,
        STXT_SAVE);
}

/*****************************************************************************/
//  Description :add list item in list box
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void AppendListItem(                                    
                          wchar                     *string_ptr,
                          uint8                    string_len,
                          MMI_CTRL_ID_T            ctrl_id,
                          MMI_TEXT_ID_T           left_softkey_id
                          )
{
    GUILIST_ITEM_T      item_t                = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T item_data             = {0};/*lint !e64*/
    uint16 suffix_wstr[MMIMMS_SUFFIX_LEN + 1] = {0};
    uint16 suffix_len                         = MMIMMS_SUFFIX_LEN;
#ifdef EBOOK_SUPPORT
    wchar  suffix_text[]                      = {'t','x','t',0};
#endif
    MMIFMM_FILE_TYPE_E file_type              = MMIFMM_FILE_TYPE_NORMAL;
    wchar *mult_file_name_ptr                 = PNULL;
    
    if(!string_ptr)
    {
        //SCI_TRACE_MID:"warning_ AppendListItem"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_6403_112_2_18_2_38_47_345,(uint8*)"");
        return;
    }

    MMIAPIFMM_SplitFileName(string_ptr, string_len, PNULL, PNULL, suffix_wstr, &suffix_len);
    file_type = MMIAPIFMM_ConvertFileType(suffix_wstr, suffix_len);

    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
    item_t.item_data_ptr = &item_data;
    mult_file_name_ptr = (wchar *)SCI_ALLOCAZ((string_len + 1)*sizeof(wchar));
    if (PNULL != mult_file_name_ptr)
    {
        SCI_MEMSET((char *)mult_file_name_ptr, 0, (string_len + 1)*sizeof(wchar));
        MMIAPICOM_Wstrncpy(mult_file_name_ptr, string_ptr, string_len);
    }
    item_t.user_data = (uint32)mult_file_name_ptr;
    
    item_data.softkey_id[0] = left_softkey_id;
    if (   MMIFMM_FILE_TYPE_PICTURE == file_type
        || MMIFMM_FILE_TYPE_MUSIC == file_type
        || MMIFMM_FILE_TYPE_MOVIE == file_type
#ifdef EBOOK_SUPPORT
        || (MMIFMM_FILE_TYPE_EBOOK == file_type && !MMIAPICOM_CompareTwoWstrExt(suffix_wstr, suffix_len, suffix_text, MMIAPICOM_Wstrlen(suffix_text), FALSE))
#endif
#ifdef MMI_VCARD_SUPPORT
        || MMIFMM_FILE_TYPE_VCARD == file_type
#endif
#ifdef MMI_VCALENDAR_SUPPORT
        || MMIFMM_FILE_TYPE_VCALENDAR == file_type
#endif
        )
    {
        item_data.softkey_id[1] = STXT_PREVIEW;
    }
    else
    {
        item_data.softkey_id[1] = TXT_NULL;
    }
    item_data.softkey_id[2] = STXT_RETURN;
    
    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[0].item_data.text_buffer.wstr_len = string_len;
    item_data.item_content[0].item_data.text_buffer.wstr_ptr = string_ptr;
    GUILIST_AppendItem(ctrl_id, &item_t);      
}

/*****************************************************************************/
//  Description :mms inbox outbox sendbox detail entry
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailEntry(MMI_CTRL_ID_T ctrl_id, uint32 record_id)
{
    GUIRICHTEXT_ITEM_T  item_data       = {0};   /*lint !e64*/
    uint16  index                       = 0;
    MMI_STRING_T  item_data_str         = {0};
    MMISMS_ORDER_INFO_T *noti_order_ptr = PNULL;
    MMIMMS_EDIT_DOCUMENT_T *      e_doc = PNULL;
    // uint8 subject[MMIMMS_MAX_SUBJECT_LEN + 1] = {0};          
    
    e_doc = GetMMSReadEditDoc();
    if (PNULL == e_doc)
    {
        return;
    }    
    noti_order_ptr =  MMIMMS_GetListItemByID(e_doc->record_id);

    if (PNULL == noti_order_ptr)
    {
        return;
    }
    
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    
    //time
    if (MMISMS_MO_SEND_SUCC == noti_order_ptr->mo_mt_type || MMISMS_MO_SEND_FAIL == noti_order_ptr->mo_mt_type)
    {
        MMSDetailTimeString(&item_data_str, noti_order_ptr->time, TRUE);
    }
    else
    {
        MMSDetailTimeString(&item_data_str, noti_order_ptr->time, FALSE);
    }    
    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;   
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index); 
    FreeItemData(&item_data_str);
    
    //size
    MMSDetailSizeString(&item_data_str, e_doc->editbody_ptr->totalsize);
    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;     
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    FreeItemData(&item_data_str);
    
    //subject
    if (e_doc->edit_subject.wstr_len != 0)
    {
        MMSDetailSubjectString(&item_data_str, e_doc->edit_subject.wstr_ptr,e_doc->edit_subject.wstr_len);
        item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
        item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr; 
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        FreeItemData(&item_data_str);
    }   

    //inbox from(sender)
    if (MMISMS_MT_TO_BE_READ == noti_order_ptr->mo_mt_type || MMISMS_MT_HAVE_READ == noti_order_ptr->mo_mt_type)
    {
        MMIAPIMMS_DetailSenderString(&item_data_str, noti_order_ptr->sender, TXT_MMS_DETAIL_FROM);
        item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
        item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;  
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        FreeItemData(&item_data_str);
    }

    //mms to
    MMSDetailToString(&item_data_str, e_doc->edit_to.wstr_ptr);
    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;   
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    FreeItemData(&item_data_str);

    //mms priority
    MMSDetailPriorityString(&item_data_str, e_doc->pritority);
    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;     
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    FreeItemData(&item_data_str);
}

/*****************************************************************************/
//  Description :mms notification detail entry
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSNotiShowEntry(MMI_CTRL_ID_T ctrl_id, uint32 record_id)
{
    GUIRICHTEXT_ITEM_T  item_data       = {0};   /*lint !e64*/
    uint16  index                       = 0;
    MMI_STRING_T  item_data_str         = {0};    
    MMIMMS_PUSH_FILE_T  *push_file_ptr  = PNULL;
    MMISMS_ORDER_INFO_T *noti_order_ptr = PNULL;

    if(!MMIMMS_IsRecordIDExist(record_id))
    {
        return;
    }
    push_file_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
    if (PNULL == push_file_ptr)
    {
        return;
    }
    //SCI_MEMSET((uint8 *)push_file_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
    MMIMMS_ReadPush(push_file_ptr, record_id);
    noti_order_ptr =  MMIMMS_GetListItemByID(record_id);

    if (PNULL == noti_order_ptr)
    {
        return;
    }
    
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_NORMAL;

    //time
    MMSDetailTimeString(&item_data_str, noti_order_ptr->time, FALSE);
    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;    
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index); 
    FreeItemData(&item_data_str);
    
    //size
    MMSDetailSizeString(&item_data_str, push_file_ptr->size);
    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;    
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    FreeItemData(&item_data_str);

    //subject
    if (0 != push_file_ptr->subject_len)
    {
        MMSDetailSubjectString(&item_data_str,push_file_ptr->subject,push_file_ptr->subject_len);
        item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
        item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;    
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        FreeItemData(&item_data_str);
    }    

    MMSDetailSenderString(&item_data_str, noti_order_ptr->sender, TXT_MMS_DETAIL_FROM);
    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;   
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    FreeItemData(&item_data_str);

    MMSDetailSenderString(&item_data_str, push_file_ptr->sender, TXT_MMS_PUSH_SMSC);

    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;    
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    FreeItemData(&item_data_str);

    //expiry time
    MMSDetailExpiryString(&item_data_str, push_file_ptr->expiry_time);
    item_data.text_data.buf.len = MIN(item_data_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
    item_data.text_data.buf.str_ptr = item_data_str.wstr_ptr;     
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    FreeItemData(&item_data_str);

    SCI_FREE(push_file_ptr);
    push_file_ptr = PNULL;
}

/*****************************************************************************/
//  Description :free item data
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void FreeItemData(MMI_STRING_T *str_dst)
{
    if (PNULL == str_dst)
    {
        return;
    }
    if (PNULL != str_dst->wstr_ptr)
    {
        MMIMMS_FREE(str_dst->wstr_ptr);
        str_dst->wstr_ptr = PNULL;
        str_dst->wstr_len = 0;
    }
}

/*****************************************************************************/
//  Description :cat tow string
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_CatString(MMI_STRING_T *dst_ptr, MMI_STRING_T *src1_ptr, MMI_STRING_T *src2_ptr)
{
    uint32    str1_len      = 0;
    wchar    *str1_text_ptr = PNULL;    
    uint32    str2_len      = 0;
    wchar    *str2_text_ptr = PNULL;
    
    if(PNULL==dst_ptr || PNULL==src1_ptr || PNULL==src2_ptr)
    {
        //SCI_TRACE_MID:"warning catstring dst_ptr=%d,src1_ptr=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_6639_112_2_18_2_38_47_346,(uint8*)"dd",dst_ptr,src1_ptr);
        return FALSE;
    }
    if (0 == src1_ptr->wstr_len && 0 == src2_ptr->wstr_len)
    {
        //SCI_TRACE_MID:"warning catstring len1=%d,len2=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_6644_112_2_18_2_38_47_347,(uint8*)"dd",src1_ptr->wstr_len,src2_ptr->wstr_len);
        return FALSE;
    }    
    str1_text_ptr    = src1_ptr->wstr_ptr;
    str1_len        = src1_ptr->wstr_len;    
    str2_text_ptr    = src2_ptr->wstr_ptr;
    str2_len        = src2_ptr->wstr_len;
    //merge
    dst_ptr->wstr_len = str1_len + str2_len;    
    dst_ptr->wstr_ptr = MMIMMS_ALLOC( (dst_ptr->wstr_len + 1) * sizeof(wchar) );
    if (PNULL == dst_ptr->wstr_ptr)
    {
        //SCI_TRACE_LOW:"MMIMMS_CatString: malloc for dst_ptr->str_ptr fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_6656_112_2_18_2_38_47_348,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(dst_ptr->wstr_ptr, 0, ((dst_ptr->wstr_len + 1) * sizeof(wchar)));    
    if (PNULL != str1_text_ptr)
    {
        MMI_WSTRNCPY(dst_ptr->wstr_ptr, dst_ptr->wstr_len, str1_text_ptr, str1_len, str1_len);
    }    
    if (PNULL != str2_text_ptr)
    {
        MMI_WSTRNCPY(dst_ptr->wstr_ptr + str1_len, dst_ptr->wstr_len, str2_text_ptr, str2_len, str2_len);
    }
    return TRUE;
}

/*****************************************************************************/
//  Description :mms  detail time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMSDetailTimeString(MMI_STRING_T *str_dst, uint32 time, BOOLEAN is_send_time)
{
    uint32 time_temp                         = 0;
    MMI_TM_T time_st                         = {0};
    uint8      date[MMISET_DATE_STR_LEN + 1] = {0};
    uint8 time_str[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    uint8 time_show[MMIMMS_TIME_STR_LEN + 1] = {0};
    wchar time_wch[MMIMMS_TIME_STR_LEN + 1]  = {0};
    MMI_STRING_T time_string                 = {0};
    uint16 time_str_len                      = 0;
    MMI_STRING_T line_str                    = {0};
    wchar line_wch[]                         = {'\n', 0};

    if (PNULL == str_dst)
    {
        return;
    }
    line_str.wstr_ptr = line_wch;
    line_str.wstr_len = MMIAPICOM_Wstrlen(line_wch);
    time_temp = time;
    time_st = MMIAPICOM_Second2Tm(time_temp);
    MMIAPISET_FormatDateStrByDateStyle(time_st.tm_year, time_st.tm_mon, time_st.tm_mday, '-', date, MMISET_DATE_STR_LEN);
    MMIAPISET_FormatTimeStrByTime(time_st.tm_hour, time_st.tm_min, time_str, MMISET_TIME_STR_12HOURS_LEN + 1);
    sprintf((char *)time_show, "%s %s", (char *)date,(char *)time_str);
    MMI_STRNTOWSTR(time_wch, MMIMMS_TIME_STR_LEN, (uint8 *)time_show, MMIMMS_TIME_STR_LEN, strlen((char *)time_show));
    time_str_len = MMIAPICOM_Wstrlen(time_wch);
    if (is_send_time)
    {
        MMI_GetLabelTextByLang(TXT_MMS_DETAIL_SEND_TIME, &time_string);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MMS_DETAIL_RCV_TIME, &time_string);
    }
    
    MMIMMS_CatString(&time_string, &time_string, &line_str);
    str_dst->wstr_len = time_string.wstr_len + time_str_len;
    str_dst->wstr_ptr = (wchar *)MMIMMS_ALLOC((str_dst->wstr_len + 1) * sizeof(wchar));    
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, time_string.wstr_ptr, time_string.wstr_len);
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr + time_string.wstr_len, time_wch, time_str_len);
    if (PNULL != time_string.wstr_ptr)
    {
        MMIMMS_FREE(time_string.wstr_ptr);
        time_string.wstr_ptr = PNULL;
        time_string.wstr_len = 0;
    }
    return;
}

/*****************************************************************************/
//  Description :mms  detail size
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailSizeString(MMI_STRING_T *str_dst, uint32 size)
{
    MMI_STRING_T size_string = {0};
    uint8 size_str[16]       = {0}; 
    wchar size_wch[16]       = {0};
    MMI_STRING_T line_str    = {0};
    wchar line_wch[]         = {'\n', 0};

    if (PNULL == str_dst)
    {
        return;
    }
    line_str.wstr_ptr = line_wch;
    line_str.wstr_len = MMIAPICOM_Wstrlen(line_wch);
    MMI_GetLabelTextByLang(TXT_DETAIL_SIZE, &size_string);
    MMIMMS_CatString(&size_string, &size_string, &line_str);
    sprintf((void*)size_str,"%.1fK\r",(float)size/ONE_K_LEN);
    if (((float)size/ONE_K_LEN) < 0.100000 && ((float)size/ONE_K_LEN) > 0.000000)
    {
        sprintf((void*)size_str,"%.1fk\r",0.1);
    }
    MMI_STRNTOWSTR(size_wch, 16, (uint8 *)size_str, 16, strlen((char *)size_str));

    str_dst->wstr_len = size_string.wstr_len + MMIAPICOM_Wstrlen(size_wch);
    str_dst->wstr_ptr = (wchar *)MMIMMS_ALLOC((str_dst->wstr_len + 1) * sizeof(wchar));    
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, size_string.wstr_ptr, size_string.wstr_len);
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr + size_string.wstr_len, size_wch, MMIAPICOM_Wstrlen(size_wch));
    if (PNULL != size_string.wstr_ptr)
    {
        MMIMMS_FREE(size_string.wstr_ptr);
        size_string.wstr_ptr = PNULL;
        size_string.wstr_len = 0;
    }
}

/*****************************************************************************/
//  Description :mms  detail subject
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailSubjectString(MMI_STRING_T *str_dst, wchar *subject_ptr, uint16 subject_len)
{
    MMI_STRING_T temp_string                  = {0};   
    MMI_STRING_T line_str                     = {0};
    wchar line_wch[]                          = {'\n', 0};
    
    if (PNULL == str_dst)
    {
        return;
    }
    line_str.wstr_ptr = line_wch;
    line_str.wstr_len = MMIAPICOM_Wstrlen(line_wch);
    MMI_GetLabelTextByLang(TXT_INPUT_SUBJECT, &temp_string);
    MMIMMS_CatString(&temp_string, &temp_string, &line_str);
    str_dst->wstr_len = temp_string.wstr_len + subject_len;
    str_dst->wstr_ptr = (wchar *)MMIMMS_ALLOC((str_dst->wstr_len + 1) * sizeof(wchar));    
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, temp_string.wstr_ptr, temp_string.wstr_len);
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr + temp_string.wstr_len, subject_ptr, subject_len);
    if (PNULL != temp_string.wstr_ptr)
    {
        MMIMMS_FREE(temp_string.wstr_ptr);
        temp_string.wstr_ptr = PNULL;
        temp_string.wstr_len = 0;
    }
}

/*****************************************************************************/
//  Description :mms  detail expiry time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailExpiryString(MMI_STRING_T *str_dst, uint32 expiry_time)
{
    MMI_STRING_T temp_string = {0};   
    MMI_STRING_T line_str    = {0};
    MMI_STRING_T colon_str   = {0};
    wchar line_wch[]         = {'\n', 0};
    wchar colon_wch[]        = {':',0};
    wchar expiry_wch[16]     = {0};
    uint8 expiry_ch[16]      = {0};
    MMI_STRING_T hour_str    = {0};
    uint32 temp_time         = 0;
    
    if (PNULL == str_dst)
    {
        return;
    }
    line_str.wstr_ptr = line_wch;
    line_str.wstr_len = MMIAPICOM_Wstrlen(line_wch);
    colon_str.wstr_ptr = colon_wch;
    colon_str.wstr_len = MMIAPICOM_Wstrlen(colon_wch);
    MMI_GetLabelTextByLang(TXT_MMS_DURATION, &temp_string);
    MMIMMS_CatString(&temp_string, &temp_string, &colon_str);
    MMIMMS_CatString(&temp_string, &temp_string, &line_str);
    
    temp_time = expiry_time;
    expiry_time = expiry_time/3600;
    if (0 < (temp_time - expiry_time * 3600))
    {
        expiry_time = expiry_time + 1;
    }
    sprintf((char *)expiry_ch,"%ld",expiry_time);
    MMI_STRNTOWSTR(expiry_wch, 16, (uint8 *)expiry_ch, 16, strlen((char *)expiry_ch));
    if(expiry_time > 1)
    {
        MMI_GetLabelTextByLang(TXT_MMS_HOURS,&hour_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MMS_HOUR,&hour_str);
    }
    MMIAPICOM_Wstrncpy(expiry_wch + strlen((char *)expiry_ch), hour_str.wstr_ptr, hour_str.wstr_len);

    str_dst->wstr_len = temp_string.wstr_len + MMIAPICOM_Wstrlen(expiry_wch);
    str_dst->wstr_ptr = (wchar *)MMIMMS_ALLOC((str_dst->wstr_len + 1) * sizeof(wchar));    
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, temp_string.wstr_ptr, temp_string.wstr_len);
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr + temp_string.wstr_len, expiry_wch, MMIAPICOM_Wstrlen(expiry_wch));
    if (PNULL != temp_string.wstr_ptr)
    {
        MMIMMS_FREE(temp_string.wstr_ptr);
        temp_string.wstr_ptr = PNULL;
        temp_string.wstr_len = 0;
    }
}

/*****************************************************************************/
//  Description :mms  detail sender
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMSDetailSenderString(MMI_STRING_T *str_dst, uint8 *sender_ptr,  MMI_TEXT_ID_T text_id)
{
    MMI_STRING_T temp_string                  = {0};   
    MMI_STRING_T line_str                     = {0};
    wchar line_wch[]                          = {'\n', 0};
    wchar sender_wch[MMIMMS_SENDER_LEN]       = {0};
    uint8 sender_ch[MMIMMS_SENDER_LEN]        = {0};
    
    if (PNULL == str_dst)
    {
        return;
    }
    line_str.wstr_ptr = line_wch;
    line_str.wstr_len = MMIAPICOM_Wstrlen(line_wch);
    MMI_GetLabelTextByLang(text_id, &temp_string);
    MMIMMS_CatString(&temp_string, &temp_string, &line_str);

    if(PNULL != sender_ptr)
    {
        sprintf((char *)sender_ch, 
            "%s\r",sender_ptr);
    }
    else
    {
        sprintf((char *)sender_ch, 
            "%s\r"," ");
    }
    MMI_STRNTOWSTR(sender_wch, MMIMMS_SENDER_LEN, (uint8 *)sender_ch, MMIMMS_SENDER_LEN, strlen((char *)sender_ch));
    
    str_dst->wstr_len = temp_string.wstr_len + MMIAPICOM_Wstrlen(sender_wch);
    str_dst->wstr_ptr = (wchar *)MMIMMS_ALLOC((str_dst->wstr_len + 1) * sizeof(wchar));    
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, temp_string.wstr_ptr, temp_string.wstr_len);
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr + temp_string.wstr_len, sender_wch, MMIAPICOM_Wstrlen(sender_wch));
    if (PNULL != temp_string.wstr_ptr)
    {
        MMIMMS_FREE(temp_string.wstr_ptr);
        temp_string.wstr_ptr = PNULL;
        temp_string.wstr_len = 0;
    }
}

/*****************************************************************************/
//  Description :mms  detail to
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailToString(MMI_STRING_T *str_dst, wchar *to_ptr)
{
    MMI_STRING_T temp_string                  = {0};   
    MMI_STRING_T line_str                     = {0};
    wchar line_wch[]                          = {'\n', 0};
    
    if (PNULL == str_dst)
    {
        return;
    }
    line_str.wstr_ptr = line_wch;
    line_str.wstr_len = MMIAPICOM_Wstrlen(line_wch);
    MMI_GetLabelTextByLang(TXT_COMMON_TO, &temp_string);
    MMIMMS_CatString(&temp_string, &temp_string, &line_str);
    
    if (PNULL != to_ptr)
    {
        str_dst->wstr_len = temp_string.wstr_len + MMIAPICOM_Wstrlen(to_ptr);
        str_dst->wstr_ptr = (wchar *)MMIMMS_ALLOC((str_dst->wstr_len + 1) * sizeof(wchar));    
        MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, temp_string.wstr_ptr, temp_string.wstr_len);
        MMIAPICOM_Wstrncpy(str_dst->wstr_ptr + temp_string.wstr_len, to_ptr, MMIAPICOM_Wstrlen(to_ptr));
    }
    else 
    {
        str_dst->wstr_len = temp_string.wstr_len;
        str_dst->wstr_ptr = (wchar *)MMIMMS_ALLOC((str_dst->wstr_len + 1) * sizeof(wchar));    
        MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, temp_string.wstr_ptr, temp_string.wstr_len);
    }    

    if (PNULL != temp_string.wstr_ptr)
    {
        MMIMMS_FREE(temp_string.wstr_ptr);
        temp_string.wstr_ptr = PNULL;
        temp_string.wstr_len = 0;
    }
    return;
}

/*****************************************************************************/
//  Description :mms  detail priority
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMSDetailPriorityString(MMI_STRING_T *str_dst, int32 priority)
{
    MMI_STRING_T temp_string  = {0};   
    MMI_STRING_T line_str     = {0};
    wchar line_wch[]          = {'\n', 0};
    MMI_STRING_T priority_str = {0};
    
    if (PNULL == str_dst)
    {
        return;
    }
    line_str.wstr_ptr = line_wch;
    line_str.wstr_len = MMIAPICOM_Wstrlen(line_wch);
    MMI_GetLabelTextByLang(TXT_DETAIL_PRIORITY, &temp_string);
    MMIMMS_CatString(&temp_string, &temp_string, &line_str);
    
    switch(priority) 
    {
    case MMS_PRIORITY_VALUE_HIGH:
        MMI_GetLabelTextByLang(TXT_COMMON_HIGH, &priority_str);
        break;
    case MMS_PRIORITY_VALUE_NORMAL:
        MMI_GetLabelTextByLang(TXT_COMMON_MID, &priority_str);
        break;
    case MMS_PRIORITY_VALUE_LOW:
        MMI_GetLabelTextByLang(TXT_COMM_LOW, &priority_str);
        break;
    default:
        MMI_GetLabelTextByLang(TXT_COMMON_MID, &priority_str);
        break;
    }
    
    str_dst->wstr_len = temp_string.wstr_len + priority_str.wstr_len;
    str_dst->wstr_ptr = (wchar *)MMIMMS_ALLOC((str_dst->wstr_len + 1) * sizeof(wchar));    
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, temp_string.wstr_ptr, temp_string.wstr_len);
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr + temp_string.wstr_len, priority_str.wstr_ptr, priority_str.wstr_len);     
    
    if (PNULL != temp_string.wstr_ptr)
    {
        MMIMMS_FREE(temp_string.wstr_ptr);
        temp_string.wstr_ptr = PNULL;
        temp_string.wstr_len = 0;
    }
    return;
}

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description :save file opt menu
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSaveFileOptMenuWinMsg(
                                           MMIMMS_WINDOW_ID_E win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param
                                           )
{
    MMI_RESULT_E err_code                  = MMI_RESULT_TRUE;
    uint32  group_id                       = 0;
    uint32  menu_id                        = 0;
    MMI_CTRL_ID_T menu_ctrl_id             = MMIMMS_SAVE_FILE_MENU_CTRL_ID;
    
    //SCI_TRACE_LOW:"MMIMMS: HandleSaveFileOptMenuWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_7004_112_2_18_2_38_48_349,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, menu_ctrl_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId( menu_ctrl_id, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id)
        {
        case ID_MMS_SAVE_MMS_FILE:       
            MMK_SendMsg(MMIMMS_SAVE_MULTIFILE_LIST_WIN_ID,MSG_CTL_OK,PNULL);
            MMK_CloseWin(win_id);
            break;
#ifdef SNS_SUPPORT
        case ID_MMS_SHARE_TO_SINA:
        case ID_MMS_SHARE_TO_FACEBOOK:
        case ID_MMS_SHARE_TO_TWITTER:
        {
                MMI_CTRL_ID_T list_ctrl_id = MMIMMS_SAVE_MULTIFILE_LIST_CTRL_ID;
                uint16 select_index = 0;
                uint32 item_data = 0;
                wchar *save_file_name_ptr  = PNULL;                   
                int share_id = 0;
                uint32    usc_len                                = 0;
                T_MMS_SHARE_TO_SNS mmsdata ={0};
                uint8 multi_temp[MMS_FILE_NAME_LEN*3 + 1]        = {0};
                uint16 suffix_wstr[MMIMMS_SUFFIX_LEN + 1] = {0};
                 uint16   suffix_len = MMIMMS_SUFFIX_LEN;
                 MMIFMM_FILE_TYPE_E file_type = 0;
                
                uint8 *data_buf_ptr = PNULL;
                uint32 data_buf_len = 0;
                uint8 *sns_data_buf_ptr = PNULL;
                wchar  suffix_text[]                             = {'t','x','t',0};

                select_index = GUILIST_GetCurItemIndex(list_ctrl_id);
    
                if (!GUILIST_GetItemData(list_ctrl_id, select_index, &item_data))
                {
                    break;
                }
                save_file_name_ptr = (wchar *)item_data;
                usc_len = MMIAPICOM_Wstrlen(save_file_name_ptr);

                MMIAPIFMM_SplitFileName(save_file_name_ptr, usc_len, PNULL, PNULL, suffix_wstr, &suffix_len);
                file_type = MMIAPIFMM_ConvertFileType(suffix_wstr, suffix_len);              
                GUI_WstrToUTF8(multi_temp, MMS_FILE_NAME_LEN*3, save_file_name_ptr,  MMIAPICOM_Wstrlen(save_file_name_ptr));
     
#ifdef DRM_SUPPORT
                if(MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(multi_temp) || MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(multi_temp))
                {
                    break;
                }
#endif
                MMIMMS_GetContentByName(multi_temp, &data_buf_ptr, &data_buf_len);

                if (MMIFMM_FILE_TYPE_EBOOK == file_type || 
                    !MMIAPICOM_CompareTwoWstrExt(suffix_wstr, suffix_len, suffix_text, MMIAPICOM_Wstrlen(suffix_text), FALSE)
                    )
                {//text data 注意text是wchar的数据 ，长度最好计算不要使用data_buf_len
                    sns_data_buf_ptr = data_buf_ptr + 4;
                    mmsdata.buf_type = MMI_SNS_DATA_TEXT;
                }
                else
                {//其他的图片和音频都是可以data_buf_len
                    sns_data_buf_ptr = data_buf_ptr;
                    mmsdata.buf_type = MMI_SNS_DATA_PIC;
                }
                
                              
                if(ID_MMS_SHARE_TO_SINA == menu_id)
                {
                    share_id = MMI_SNS_SINA;
                }
                else if(ID_MMS_SHARE_TO_FACEBOOK == menu_id)
                {
                    share_id = MMI_SNS_FACEBOOK;
                }
                else
                {
                    share_id = MMI_SNS_TIWTTER;
                }
                
                IMMISNS_CreateObject(&pMe);
                
                if(pMe)
                {
                    if(IMMISNS_IsFixAppInstalled(pMe, share_id) < 0)
                    {
                        IMMISNS_Release(pMe);
                        pMe = PNULL;
                        
                        MMIPUB_OpenAlertWinByTextIdEx(MMK_GetFirstAppletHandle(),PNULL,TXT_WHETHER_INSTALL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleMMSWhetherInstallWinMsg, share_id);
                    }
                    else
                    {    
                        uint32        time_num    = 500;
                        
                        mmsdata.mms_buff     = sns_data_buf_ptr;
                        mmsdata.mms_buff_len = data_buf_len;
                        IMMISNS_ShareToSns(pMe, share_id, SNS_MODULE_MMS , (unsigned long int)&mmsdata);
                        MMIPUB_OpenAlertWinByTextId(&time_num, TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

                    }
                }
                break;
          }    
#endif

        default:
            //SCI_TRACE_LOW:"menu_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_7119_112_2_18_2_38_48_350,(uint8*)"d",menu_id);
            break;
        }
        break;
 
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;            
    case MSG_CLOSE_WINDOW:
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
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    return err_code;
}

#ifdef SNS_SUPPORT
/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleMMSWhetherInstallWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
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
#endif
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get preview rich text rect
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMMS_GetPreviewRichtextRect(GUI_RECT_T *rect_ptr)
{
    uint16 mainscreen_height          = 0;
    uint16 mainscreen_width           = 0;


    if (PNULL == rect_ptr)
    {
        return FALSE;
    }
    if (!GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID, &mainscreen_width, &mainscreen_height))
    {
        return FALSE;
    }

    rect_ptr->left = 0;
    rect_ptr->top = MMI_TITLE_HEIGHT +MMI_STATUSBAR_HEIGHT - 1;
    rect_ptr->right = mainscreen_width;
    rect_ptr->bottom = mainscreen_height - MMIMMS_BUTTON_FORM_HEIGHT  -1;  // 40 BUTTON FORM HEIGHT
    return TRUE;
}

/*****************************************************************************/
//  Description : button form控件
//  Global resource dependence : 
//  Author: rong.gu
//  Note:
/*****************************************************************************/
LOCAL void MmsDestroyMMSButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMIMMS_BUTTON_T *button_array,uint16 array_len)
{
    uint16 index = 0;
    if(0 != MMK_GetCtrlHandleByWin(win_id, ctrl_id))
    {
        MMK_DestroyControl(ctrl_id);
        for(index = 0; index < array_len; index++)
        {
            MMK_DestroyDynaCtrl(button_array[index].ctrl_id);
        }
    }
}
/*****************************************************************************/
//  Description :construct button form ctrl
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MmsConstructMMSButtonFormCtrl(MMI_WIN_ID_T win_id,MMI_CTRL_ID_T ctrl_id,MMIMMS_BUTTON_T *button_array,uint16 array_len)
{
    GUIBUTTON_INIT_DATA_T       button_init = {0};
    GUI_BG_T bg = {0};
    MMI_CONTROL_CREATE_T        form_ctrl = {0};
    GUIFORM_INIT_DATA_T         form_init = {0};//parent form
    GUIFORM_DYNA_CHILD_T    form_child_ctrl= {0};//child form
    uint16 index = 0;
    GUI_BOTH_RECT_T client_rect = {0};

    client_rect = MMITHEME_GetWinClientBothRect(win_id);

    //create MMIIDLE_DIAL_BUTTON_FORM_CTRL_ID control
    form_init.both_rect.v_rect.left =  client_rect.v_rect.left;
    form_init.both_rect.v_rect.top = client_rect.v_rect.bottom - MMIMMS_BUTTON_FORM_HEIGHT + 1;
    form_init.both_rect.v_rect.right = client_rect.v_rect.right;
    form_init.both_rect.v_rect.bottom = client_rect.v_rect.bottom;
    
    form_init.both_rect.h_rect.left = client_rect.h_rect.left;
    form_init.both_rect.h_rect.top = client_rect.h_rect.bottom - MMIMMS_BUTTON_FORM_HEIGHT + 1;
    form_init.both_rect.h_rect.right = client_rect.h_rect.right;
    form_init.both_rect.h_rect.bottom = client_rect.h_rect.bottom;
    form_init.layout_type = GUIFORM_LAYOUT_SBS;    

    form_ctrl.ctrl_id           = ctrl_id;
    form_ctrl.guid              = SPRD_GUI_FORM_ID;
    form_ctrl.init_data_ptr     = &form_init;
    form_ctrl.parent_win_handle = win_id;
    MMK_CreateControl(&form_ctrl);
        
    bg.bg_type = GUI_BG_IMG;
    bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;                      
    GUIFORM_SetBg(ctrl_id, &bg);
    GUIFORM_SetMargin(ctrl_id, 6);
    //not slide
    GUIFORM_IsSlide(ctrl_id,FALSE);

    for(index = 0; index < array_len; index++)
    {    
        form_child_ctrl.child_handle = button_array[index].ctrl_id;
        form_child_ctrl.guid = SPRD_GUI_BUTTON_ID;
        
        button_init.bg.bg_type = GUI_BG_IMG;
        button_init.bg.img_id  = button_array[index].bg_img_id;
        form_child_ctrl.init_data_ptr = &button_init;
        
        form_child_ctrl.is_bg = FALSE;
        form_child_ctrl.is_get_active = FALSE;

        GUIFORM_CreatDynaChildCtrl(win_id, ctrl_id,  &form_child_ctrl);
        
        bg.img_id = button_array[index].pressed_bg_img_id;
        GUIBUTTON_SetPressedBg(button_array[index].ctrl_id, &bg);

        bg.img_id = button_array[index].fg_img_id;
        GUIBUTTON_SetFg(button_array[index].ctrl_id, &bg);

        bg.img_id = button_array[index].pressed_fg_img_id;
        GUIBUTTON_SetPressedFg(button_array[index].ctrl_id, &bg);        
    }
    if(1 == array_len)
    {
        GUIFORM_CHILD_WIDTH_T width = {0};
        width.type = GUIFORM_CHILD_WIDTH_FIXED;
        width.add_data = GUI_FORM_ONE_BUTTON_WIDTH;
        GUIFORM_SetChildWidth(ctrl_id, button_array[0].ctrl_id,&width);
        GUIFORM_SetAlign(ctrl_id, GUIFORM_CHILD_ALIGN_HMIDDLE);
    }
}
/*****************************************************************************/
//  Description :set preview win msg
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMMSPreviewWinPenOkMsg(MMI_WIN_ID_T      win_id,         //in:
                              MMI_CTRL_ID_T     ctrl_id,
                              uint8 *timer_ptr,
                              uint8 *duration_ptr)
{
    MMIMMS_WINDOW_ID_E quer_win_id         = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    MMIMMS_EDIT_DOCUMENT_T *edit_doc_ptr = PNULL;
    GUI_BG_T button_bg = {0};
    edit_doc_ptr = GetMMSReadEditDoc();
    if (edit_doc_ptr != PNULL)
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id); 
    }
    
    switch (ctrl_id)
    {
        case MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID:
            {
                if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state)
                {
                    if(mms_list_info_ptr != PNULL && MMS_PLAY == GetMMSPreviewOrPlay())//play status
                    {
                        if(MMISMS_MO_SEND_SUCC == mms_list_info_ptr->mo_mt_type)
                        {
                              MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                              s_sendbox_play_form_button_info,
                              sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                              );                             
                        } 
                        else if(MMISMS_MO_SEND_FAIL == mms_list_info_ptr->mo_mt_type)
                        {
                              MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                              s_outbox_play_form_button_info,
                              sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                              );  
                        }
                        else
                        {
                              MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                              s_mtbox_play_form_button_info,
                              sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                              );
                        }
                        MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                        s_play_form_button_info,
                        sizeof(s_play_form_button_info)/sizeof(s_play_form_button_info[0])
                        );
                    }
                    
                    button_bg.bg_type = GUI_BG_IMG;
                    button_bg.img_id = IMAGE_MMS_PAUSE_PDA;
                    GUIBUTTON_SetFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);   
                    GUIBUTTON_SetPressedFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);   
                    SetMMSPreviewIsWebPlay(TRUE);                
                    if(!BoxPlayMMS(edit_doc_ptr))
                    {
                        break;
                    }      
                }
                else if(MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state)
                {            
                     if(MMS_PREVIEW == GetMMSPreviewOrPlay())
                     {
                        button_bg.bg_type = GUI_BG_IMG;
                        button_bg.img_id = IMAGE_MMS_PREVIEW_PDA;
                        GUIBUTTON_SetFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);  
                        GUIBUTTON_SetPressedFg(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID, &button_bg);                           
                        //GUIBUTTON_Update(MMIMMS_PREVIEW_BUTTON_PLAY_CTRL_ID);   
                     }
                     else
                     {
                        MmsDestroyMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                        s_play_form_button_info,
                        sizeof(s_play_form_button_info)/sizeof(s_play_form_button_info[0])
                        ); 
                        if(mms_list_info_ptr != PNULL)
                        {
                            if((MMISMS_MO_SEND_SUCC == mms_list_info_ptr->mo_mt_type))
                            {
                              MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                              s_sendbox_play_form_button_info,
                              sizeof(s_sendbox_play_form_button_info)/sizeof(s_sendbox_play_form_button_info[0])
                              );                             
                            } 
                            else if(MMISMS_MO_SEND_FAIL == mms_list_info_ptr->mo_mt_type)
                            {
                              MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                              s_outbox_play_form_button_info,
                              sizeof(s_outbox_play_form_button_info)/sizeof(s_outbox_play_form_button_info[0])
                              );      
                            }
                            else
                            {
                              MmsConstructMMSButtonFormCtrl(win_id,MMIMMS_PREVIEW_BUTTON_FORM_CTRL_ID,
                              s_mtbox_play_form_button_info,
                              sizeof(s_mtbox_play_form_button_info)/sizeof(s_mtbox_play_form_button_info[0])
                              );
                            }
                        }
                        
                     }
                    
                    MmsStopPreviewRing();
                    MmsStopPreviewTTSReading();
                    if(0 != *timer_ptr)
                    {
                        MMK_StopTimer(*timer_ptr);
                        *timer_ptr = 0;
                    }

                    if(0 != *duration_ptr)
                    {
                        MMK_StopTimer(*duration_ptr);
                        *duration_ptr = 0;
                    }
                    
                    GUIWIN_SetTitleSubText(win_id, FALSE, PNULL, 0, FALSE);
                    GUIWIN_UpdateTitle(win_id);
                    
                    if (GetMMSPreviewIsWebPlay())
                    {
                        SetMMSPreviewIsWebPlay(FALSE);
                    }

                    if ( MMS_PLAY == GetMMSPreviewOrPlay() && MMIMMS_Setting_GetExtractContent())
                    {
                        GUIRICHTEXT_SetFocusMode(MMIMMS_PREVIEW_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
                    }
                    
                    s_edit_mms_preview_state=MMIMMS_PREVIEW_STOP;
    
                    MMIDEFAULT_AllowTurnOffBackLight(TRUE);
                    MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
                }
            }
            break;
        case MMIMMS_PREVIEW_BUTTON_FOWARD_CTRL_ID:
            {
                if(PNULL == mms_list_info_ptr )
                {
                    MMK_CloseWin(win_id);
                    return FALSE;
                }
            
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }
                MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_SENDBOX_READ_TO_FARWARD);
                MMIAPIMMS_FarwardMMS(mms_list_info_ptr->record_id);  
                MMK_CloseWin(win_id);
 
                break;
            }
        case MMIMMS_PREVIEW_BUTTON_SEND_CTRL_ID:
            if(PNULL == mms_list_info_ptr )
            {
                MMK_CloseWin(win_id);
                return FALSE;
            }
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            MMIMMS_SetIsMMSMode(TRUE);
#endif
            MMIMMS_OutboxSendMMS(mms_list_info_ptr->record_id, edit_doc_ptr);              
            MMK_CloseWin(win_id);
            break;
        case MMIMMS_PREVIEW_BUTTON_SMS_REPLEY_CTRL_ID:
#ifdef MMIMMS_SMS_IN_1_SUPPORT
            if(PNULL == mms_list_info_ptr )
            {
                MMK_CloseWin(win_id);
                return FALSE;
            }            
            MMIAPISMS_AnswerMessage(mms_list_info_ptr->dual_sys, PNULL, mms_list_info_ptr->sender, (uint8)strlen((char *)mms_list_info_ptr->sender));
#else
            {
                uint8 num_len                          = 0;
                MMI_STRING_T string_to                 = {0};
                MMI_STRING_T string_subject            = {0};
                if(PNULL == mms_list_info_ptr )
                {
                    MMK_CloseWin(win_id);
                    return FALSE;
                }
                num_len = strlen((char *)mms_list_info_ptr->sender);
                //answer mms
                if (MMK_IsOpenWin(MMIMMS_EDIT_MENU_WIN_ID))
                {
                    MMSMMI_OpenMMSAlertWin(TXT_MMS_EDIT_WIN_IN_USED, TXT_NULL, IMAGE_PUBWIN_WARNING);                    
                    break;
                }   
                MMIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_INBOX_READ_TO_ANSWER);
                if(num_len > 0)
                {
                    string_to.wstr_ptr= (wchar*)SCI_ALLOC_APPZ(num_len * sizeof(wchar));
                    MMI_STRNTOWSTR(string_to.wstr_ptr, num_len, (uint8 *)mms_list_info_ptr->sender, num_len, num_len);
                    string_to.wstr_len = num_len;
                    MMIAPIMMS_GetSubjectById(mms_list_info_ptr->record_id, &string_subject);
                    MMIAPIMMS_AnswerMMS(mms_list_info_ptr->dual_sys,&string_subject,&string_to);
                    SCI_FREE(string_to.wstr_ptr);
                }
                MMK_CloseWin(win_id);
            }
#endif
            break;
            
        case MMIMMS_PREVIEW_BUTTON_DELETE_CTRL_ID:                      
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
            break;    
            
        default:
            return FALSE;
    }
    return TRUE;

}

/*****************************************************************************/
//  Description :mms noti win msg
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMMSNotiWinPenOkMsg(MMI_WIN_ID_T      win_id,         //in:
                              MMI_CTRL_ID_T     ctrl_id,
                              uint32 record_id)
{ 
    MMIMMS_WINDOW_ID_E quer_win_id         = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    if (MMIMMS_IsRecordIDExist(record_id))
    {
        mms_list_info_ptr = MMIMMS_GetListItemByID(record_id);
    }

    if(PNULL == mms_list_info_ptr)
    {
        MMK_CloseWin(win_id);
        return FALSE;
    }
    
    switch (ctrl_id)
    {
        case MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID:
        //把该record id的彩信通知加入等待下载队列，同时改变该条消息的图标
            if (!MMIMMS_IsInQueue(record_id))//record_id没有在下载链表里面，加入下载链表
            {
                MMIMMS_PUSH_FILE_T *push_info_ptr      = PNULL;

                push_info_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
                if (PNULL != push_info_ptr)
                {
                    //SCI_MEMSET((uint8 *)push_info_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
                    MMIMMS_ReadPush(push_info_ptr, record_id);
                }
                else
                {
                    break;
                }
                if (MMIMMS_IsSimCardOK(mms_list_info_ptr->dual_sys))
                {
                    if (MMIMMS_IsExpire(mms_list_info_ptr->time,(uint32)push_info_ptr->expiry_time))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_EXPIRED, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                    }
                    else if (!MMIMMS_IsEnoughSpace(push_info_ptr->size + MMIMMS_RESERVE_LIST_SIZE))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_FULLSPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    }
                    else if(MMIMMS_DetailAndDownloadMMS(mms_list_info_ptr->dual_sys, record_id))
                    {
                        MMIAPISMS_UpdateMsgStatus(); 
                        if (MMIMMS_IsAutolistWithItem())
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_MMS_TEXT_PREPARE_DOWNLOAD, TXT_NULL, IMAGE_PUBWIN_WARNING);
                        }                                                
                        MMK_CloseWin(win_id);
                        if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
                        {
                            MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
                        }
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIMMS: HandleInboxMMSDetailOptMenuWinMsg active mms error!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_7592_112_2_18_2_38_49_351,(uint8*)"");
                    }
                }
                else
                {
                    MMIAPIPHONE_AlertSimNorOKStatusII(mms_list_info_ptr->dual_sys);
                } 
                SCI_FREE(push_info_ptr);
                push_info_ptr = PNULL;
            }
            else
            {
              //[start]changed by minghu cr166756
                if (mms_list_info_ptr->mo_mt_type == MMISMS_MT_NOT_NOTIFY)
                {
                    mms_list_info_ptr->mo_mt_type = MMISMS_MT_NOT_DOWNLOAD;
                    MMIMMS_SaveMMSListFile();
                }
                //[end]
                MMK_CloseWin(win_id);
                MMIMMS_CancelSendRecvById(record_id);
                MMIAPISMS_UpdateMsgStatus();
           }
        break;
        
        case MMIMMS_INBOX_NOTIDETAIL_BUTTON_SMS_REPLY_CTRL_ID:
        MMIAPISMS_AnswerMessage(mms_list_info_ptr->dual_sys, PNULL, mms_list_info_ptr->sender, (uint8)strlen((char *)mms_list_info_ptr->sender));
       
        break;
        case MMIMMS_INBOX_NOTIDETAIL_BUTTON_DELETE_CTRL_ID:                      
        MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
        break;
            
        default:
            return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description :noti detail win button form
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
LOCAL void MMSNotiDetailWinButtonFormInit(MMIMMS_WINDOW_ID_E win_id)
{
        GUI_BG_T         bg = {0};
        GUI_BOTH_RECT_T both_rect = {0};
        GUI_BOTH_RECT_T client_rect = {0};

        client_rect = MMITHEME_GetWinClientBothRect(win_id);       

        both_rect.v_rect.left = client_rect.v_rect.left ;
        both_rect.v_rect.right = client_rect.v_rect.right;
        both_rect.v_rect.top = client_rect.v_rect.bottom - MMIMMS_BUTTON_FORM_HEIGHT;
        both_rect.v_rect.bottom = client_rect.v_rect.bottom ;

        both_rect.h_rect.left = client_rect.h_rect.left;
        both_rect.h_rect.right = client_rect.h_rect.right;
        both_rect.h_rect.top = client_rect.h_rect.bottom - MMIMMS_BUTTON_FORM_HEIGHT ;
        both_rect.h_rect.bottom = client_rect.h_rect.bottom;
        
        GUIAPICTRL_SetBothRect(MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID, &both_rect);
        GUIFORM_IsSlide(MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID, FALSE);

        
        bg.bg_type = GUI_BG_IMG;
        bg.img_id = IMAGE_SMS_BUTTON_FORM_BG;                      
        GUIFORM_SetBg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID, &bg);
        GUIFORM_SetMargin(MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID, 6);

        GUIBUTTON_SetRunSheen(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, FALSE);
        GUIBUTTON_SetRunSheen(MMIMMS_INBOX_NOTIDETAIL_BUTTON_SMS_REPLY_CTRL_ID, FALSE);
        GUIBUTTON_SetRunSheen(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DELETE_CTRL_ID, FALSE);   
        
        bg.img_id = IMAGE_SMS_BUTTON_NORMAL;
        GUIBUTTON_SetBg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, &bg);
        GUIBUTTON_SetBg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_SMS_REPLY_CTRL_ID, &bg);
        GUIBUTTON_SetBg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DELETE_CTRL_ID, &bg);            
    
        bg.img_id = IMAGE_SMS_BUTTON_PRESSED;               
        GUIBUTTON_SetPressedBg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, &bg);
        GUIBUTTON_SetPressedBg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_SMS_REPLY_CTRL_ID, &bg);
        GUIBUTTON_SetPressedBg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DELETE_CTRL_ID, &bg);
    
        bg.img_id =  IMAGE_MMS_DOWNLOAD_PDA;          
        GUIBUTTON_SetFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, &bg);       
        GUIBUTTON_SetPressedFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, &bg);
    
        bg.img_id =  IMAGE_SMS_REPLY_SMS_PDA;          
        GUIBUTTON_SetFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_SMS_REPLY_CTRL_ID, &bg);       
        GUIBUTTON_SetPressedFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_SMS_REPLY_CTRL_ID, &bg);
    
        bg.img_id =  IMAGE_SMS_DELETE_SMS_PDA;          
        GUIBUTTON_SetFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DELETE_CTRL_ID, &bg);       
        GUIBUTTON_SetPressedFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DELETE_CTRL_ID, &bg);        

}

/*****************************************************************************/
//  Description :left right key init
//  Global resource dependence : 
//  Author:rong.gu
//  Note: 
/*****************************************************************************/
PUBLIC void MMSPreviewWinLFRHButtonInit(void)
{
     GUI_BG_T         bg = {0}; 
     bg.bg_type = GUI_BG_IMG;            
     
     GUIBUTTON_SetRunSheen(MMIMMS_BUTTON_LF_CTRL_ID,FALSE);
     GUIBUTTON_SetRunSheen(MMIMMS_BUTTON_RH_CTRL_ID,FALSE);
     bg.img_id = IMAGE_MMS_COUNT_LBTN_UN;
     GUIBUTTON_SetBg(MMIMMS_BUTTON_LF_CTRL_ID,&bg);
     GUIBUTTON_SetFg(MMIMMS_BUTTON_LF_CTRL_ID,&bg);
     bg.img_id = IMAGE_MMS_COUNT_LBTN_PR;
     GUIBUTTON_SetPressedFg(MMIMMS_BUTTON_LF_CTRL_ID,&bg);  
     bg.img_id = IMAGE_MMS_COUNT_RBTN_UN;
     GUIBUTTON_SetBg(MMIMMS_BUTTON_RH_CTRL_ID,&bg);
     GUIBUTTON_SetFg(MMIMMS_BUTTON_RH_CTRL_ID,&bg);
     bg.img_id = IMAGE_MMS_COUNT_RBTN_PR;
     GUIBUTTON_SetPressedFg(MMIMMS_BUTTON_RH_CTRL_ID,&bg);  
}
#endif

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :mms notification detail read menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMMSNotiWinMsg(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E   err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id                 = MMIMMS_NOTIDETAIL_TEXTBOX_CTRL_ID;    
    ADD_DATA  record_id_ptr                = MMK_GetWinAddDataPtr(win_id);
    uint32 record_id                       = 0;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    MMIMMS_PUSH_FILE_T *push_info_ptr      = PNULL;
    uint32  unsettle_num                   = 0;
    MMIMMS_WINDOW_ID_E quer_win_id         = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
#ifdef MMI_PDA_SUPPORT
    GUI_RECT_T richtext_rect = {0};
#endif
    
    record_id = (uint32)record_id_ptr;
    //SCI_TRACE_LOW:"MMIMMS: HandleInboxMMSNotiWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_7738_112_2_18_2_38_50_352,(uint8*)"d", msg_id);
    switch(msg_id)
    {    
    #ifdef MMI_PDA_SUPPORT
    case MSG_LCD_SWITCH:
        MMIMMS_GetPreviewRichtextRect(&richtext_rect);
        GUIRICHTEXT_SetRect(ctrl_id,richtext_rect);
        break;
    #endif
    
    case MSG_OPEN_WINDOW:  
#ifdef MMI_PDA_SUPPORT
    {
        GUI_BOTH_RECT_T  client_rect = {0};
        client_rect = MMITHEME_GetWinClientBothRect(win_id);
        MMIMMS_GetPreviewRichtextRect(&richtext_rect);
        
        
        //GUIRES_GetImgWidthHeight(&img_width, &img_height, s_button_ctrl_id[0].button_img_id, win_id);        
        GUIRICHTEXT_SetRect(ctrl_id,richtext_rect);
        if (MMIMMS_GetPreviewMMSDetailFlag())
        {                    

        MMSNotiDetailWinButtonFormInit(win_id);
        }
        else
        {
            richtext_rect.bottom += MMIMMS_BUTTON_FORM_HEIGHT;
            GUIFORM_SetVisible(MMIMMS_INBOX_NOTIDETAIL_BUTTON_FORM_CTRL_ID, FALSE,FALSE);  
        }
        
    }

        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

        if (MMIMMS_GetPreviewMMSDetailFlag())
        {
            MMSNotiShowEntry(ctrl_id, record_id);
        }
        else 
        {
            MMSDetailEntry(ctrl_id, record_id); 
        }               
        //set sofkey           
        if(MMIMMS_GetPreviewMMSDetailFlag())
        {
            if (!MMIMMS_IsInQueue(record_id))//record_id没有在下载链表里面
            {
                #ifdef MMI_PDA_SUPPORT 
                GUI_BG_T button = {0};                
                button.bg_type = GUI_BG_IMG;
                button.img_id =  IMAGE_MMS_DOWNLOAD_PDA;          
                GUIBUTTON_SetFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, &button);       
                GUIBUTTON_SetPressedFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, &button);
                //set softkey
                #else
                 GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, STXT_OPTION, TXT_COMMON_DOWNLOAD_COMN, STXT_RETURN, FALSE);
                #endif
            }
            else
            {                
                #ifdef MMI_PDA_SUPPORT
                GUI_BG_T button = {0};                
                button.bg_type = GUI_BG_IMG;
                button.img_id  = IMAGE_MMS_DOWNLOAD_CANCEL_PDA;
                GUIBUTTON_SetFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, &button);       
                GUIBUTTON_SetPressedFg(MMIMMS_INBOX_NOTIDETAIL_BUTTON_DOWNLOAD_CTRL_ID, &button);
                #else
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
                #endif
            } 
            GUIWIN_SetTitleTextId(win_id,TXT_MMS_DETAIL, FALSE);
        }
        else
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
            GUIWIN_SetTitleTextId(win_id, STXT_DETAIL, FALSE);
        }
        //set active control
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;        
    case MSG_FULL_PAINT:
        MMIMMS_CLEAR_CLIENT(win_id);        
        break;        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
           MMK_CloseWin(win_id);
           break;           
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if(MMIMMS_GetPreviewMMSDetailFlag())
        {
            //noti menu
            OpenMMSDetailOptPopMenu(record_id);
        }
        break; 
    case MSG_PROMPTWIN_OK:
        {
                MMIMMS_DeleteMMSAndSaveList(record_id);
#ifdef MMI_GUI_STYLE_TYPICAL
                MMSMMI_OpenMMSAlertWin(TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
                if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
                }
                unsettle_num = MMIMMS_GetUnsettleNum();
                if (0 == unsettle_num)
                {
                    if (MMK_IsOpenWin(MMIMMS_NEWMSG_WIN_ID))
                    {
                        MMK_CloseWin(MMIMMS_NEWMSG_WIN_ID);
                    }
                }
                MMK_CloseWin(win_id);
        }
        break;

        
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        HandleMMSNotiWinPenOkMsg(win_id,((MMI_NOTIFY_T*)param)->src_id,record_id);
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    
    case MSG_PROMPTWIN_CANCEL:
        //close query window
        MMK_CloseWin(quer_win_id);        
        break;
        //case MSG_CTL_PENOK: //触笔
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef WIN32
    case MSG_APP_GREEN:
    case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:    
#endif
        if(MMIMMS_GetPreviewMMSDetailFlag())
        {  
            mms_list_info_ptr = MMIMMS_GetListItemByID(record_id);

            if (PNULL == mms_list_info_ptr)
            {
                break;
            }
            
            push_info_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
            if (PNULL == push_info_ptr)
            {
                break;
            }
            //SCI_MEMSET((uint8 *)push_info_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
            MMIMMS_ReadPush(push_info_ptr, record_id);
            if (!MMIMMS_IsInQueue(record_id))//record_id没有在下载链表里面，加入下载链表
            {
                if (MMIMMS_IsSimCardOK(mms_list_info_ptr->dual_sys))
                {
                    if (MMIMMS_IsExpire(mms_list_info_ptr->time,(uint32)push_info_ptr->expiry_time))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_EXPIRED, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                    }
                    else if (!MMIMMS_IsEnoughSpace(push_info_ptr->size + MMIMMS_RESERVE_LIST_SIZE))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_FULLSPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    }
                    else if(MMIMMS_DetailAndDownloadMMS(mms_list_info_ptr->dual_sys, record_id))
                    {
                        //更新短消息的状态图标                
                        MMIAPISMS_UpdateMsgStatus(); 
                        if (MMIMMS_IsAutolistWithItem())
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_MMS_TEXT_PREPARE_DOWNLOAD, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                        }                        
                        if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
                        {
                            MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
                        }
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIMMS: HandleInboxMMSNotiWinMsg active mms error!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_7919_112_2_18_2_38_50_353,(uint8*)"");
                    }
                }
                else
                {
                    MMIAPIPHONE_AlertSimNorOKStatusII(mms_list_info_ptr->dual_sys);
                }            
            } 
            SCI_FREE(push_info_ptr);
            push_info_ptr = PNULL;
        }
        break;
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(quer_win_id);
        if (MMK_IsOpenWin(MMIMMS_INBOX_MMSNOTIOPT_MENU_WIN_ID))
        {
            MMK_CloseWin(MMIMMS_INBOX_MMSNOTIOPT_MENU_WIN_ID);
        }
        break;
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code; 
}
#else
/*****************************************************************************/
//  Description :mms notification detail read menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleInboxMMSNotiWinMsg(
                                             MMIMMS_WINDOW_ID_E win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param
                                             )
{
    MMI_RESULT_E   err_code                = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T  ctrl_id                 = MMIMMS_NOTIDETAIL_TEXTBOX_CTRL_ID;    
    ADD_DATA  record_id_ptr                = MMK_GetWinAddDataPtr(win_id);
    uint32 record_id                       = 0;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    MMIMMS_PUSH_FILE_T *push_info_ptr      = PNULL;
    uint32  unsettle_num                   = 0;
    MMIMMS_WINDOW_ID_E quer_win_id         = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    
    record_id = (uint32)record_id_ptr;
    //SCI_TRACE_LOW:"MMIMMS: HandleInboxMMSNotiWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_7965_112_2_18_2_38_50_354,(uint8*)"d", msg_id);
    switch(msg_id)
    {    
    case MSG_OPEN_WINDOW:  

        if (MMIMMS_GetPreviewMMSDetailFlag())
        {
            MMSNotiShowEntry(ctrl_id, record_id);
        }
        else 
        {
            MMSDetailEntry(ctrl_id, record_id); 
        }               
        //set sofkey           
        if(MMIMMS_GetPreviewMMSDetailFlag())
        {
            if (!MMIMMS_IsInQueue(record_id))//record_id没有在下载链表里面
            {
                 GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, STXT_OPTION, TXT_COMMON_DOWNLOAD_COMN, STXT_RETURN, FALSE);

            }
            else
            {                
                GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
            } 
            GUIWIN_SetTitleTextId(win_id,TXT_MMS_DETAIL, FALSE);
        }
        else
        {
            GUISOFTKEY_SetTextId(win_id, MMICOMMON_SOFTKEY_CTRL_ID, TXT_NULL, TXT_NULL, STXT_RETURN, FALSE);
            GUIWIN_SetTitleTextId(win_id, STXT_DETAIL, FALSE);
        }

        //set active control
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;        
    case MSG_FULL_PAINT:
        MMIMMS_CLEAR_CLIENT(win_id);        
        break;        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
           MMK_CloseWin(win_id);
           break;           
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        if(MMIMMS_GetPreviewMMSDetailFlag())
        {
            //noti menu
            OpenMMSDetailOptPopMenu(record_id);
        }
        break; 
    case MSG_PROMPTWIN_OK:
        {
            //delete the mms
            MMK_CloseWin(quer_win_id);
            MMIMMS_DeleteMMSAndSaveList(record_id);
            MMK_CloseWin(win_id);
            if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
            {
                MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
            }
            unsettle_num = MMIMMS_GetUnsettleNum();
            if (0 == unsettle_num)
            {
                if (MMK_IsOpenWin(MMIMMS_NEWMSG_WIN_ID))
                {
                    MMK_CloseWin(MMIMMS_NEWMSG_WIN_ID);
                }
            }
        }
        break;       
    
    case MSG_PROMPTWIN_CANCEL:
        //close query window
        MMK_CloseWin(quer_win_id);        
        break;
        //case MSG_CTL_PENOK: //触笔
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
#ifdef WIN32
    case MSG_APP_GREEN:
	case MSG_APP_CALL2:
    case MSG_APP_CALL3:
    case MSG_APP_CALL4:
#endif
        if(MMIMMS_GetPreviewMMSDetailFlag())
        {  
            mms_list_info_ptr = MMIMMS_GetListItemByID(record_id);

            if (PNULL == mms_list_info_ptr)
            {
                break;
            }
            
            push_info_ptr = (MMIMMS_PUSH_FILE_T*)SCI_ALLOCAZ(sizeof(MMIMMS_PUSH_FILE_T));
            if (PNULL == push_info_ptr)
            {
                break;
            }
            //SCI_MEMSET((uint8 *)push_info_ptr, 0 , sizeof(MMIMMS_PUSH_FILE_T));
            MMIMMS_ReadPush(push_info_ptr, record_id);
            if (!MMIMMS_IsInQueue(record_id))//record_id没有在下载链表里面，加入下载链表
            {
                if (MMIMMS_IsSimCardOK(mms_list_info_ptr->dual_sys))
                {
                    if (MMIMMS_IsExpire(mms_list_info_ptr->time,(uint32)push_info_ptr->expiry_time))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_EXPIRED, TXT_NULL, IMAGE_PUBWIN_WARNING);                        
                    }
                    else if (!MMIMMS_IsEnoughSpace(push_info_ptr->size + MMIMMS_RESERVE_LIST_SIZE))
                    {
                        MMSMMI_OpenMMSAlertWin(TXT_MMS_FULLSPACE, TXT_NULL, IMAGE_PUBWIN_WARNING);
                    }
                    else if(MMIMMS_DetailAndDownloadMMS(mms_list_info_ptr->dual_sys, record_id))
                    {
                        //更新短消息的状态图标                
                        MMIAPISMS_UpdateMsgStatus(); 
                        if (MMIMMS_IsAutolistWithItem())
                        {
                            MMSMMI_OpenMMSAlertWin(TXT_MMS_TEXT_PREPARE_DOWNLOAD, TXT_NULL, IMAGE_PUBWIN_WARNING);                            
                        }                        
                        if (MMK_IsOpenWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID))
                        {
                            MMK_CloseWin(MMIMMS_INBOX_NOTIDETAIL_WIN_ID);
                        }
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIMMS: HandleInboxMMSNotiWinMsg active mms error!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8094_112_2_18_2_38_50_355,(uint8*)"");
                    }
                }
                else
                {
                    MMIAPIPHONE_AlertSimNorOKStatusII(mms_list_info_ptr->dual_sys);
                }            
            } 
            SCI_FREE(push_info_ptr);
            push_info_ptr = PNULL;
        }
        break;
    case MSG_CLOSE_WINDOW:
        MMK_CloseWin(quer_win_id);
        if (MMK_IsOpenWin(MMIMMS_INBOX_MMSNOTIOPT_MENU_WIN_ID))
        {
            MMK_CloseWin(MMIMMS_INBOX_MMSNOTIOPT_MENU_WIN_ID);
        }
        break;
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code; 
}
#endif

/*****************************************************************************/
//  Description :mms send rpt waiting win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendReadRptWaitingWin(
                                                MMIMMS_WINDOW_ID_E win_id, 
                                                MMI_MESSAGE_ID_E msg_id, 
                                                DPARAM param
                                                )
{
    MMI_RESULT_E err_code                    = MMI_RESULT_TRUE;
    MMIMMS_EDIT_DOCUMENT_T *    editdoc_ptr  = PNULL;
    MMIMMS_MSG_SEND_END_T *send_end_info_ptr = PNULL;
    MMIMMS_ERROR_E op_error                  = MMIMMS_ERROR_MAX;    
    MMIMMS_ERROR_E   send_rpt_result         = MMIMMS_NO_ERROR;
    MMIMMS_WINDOW_ID_E quer_win_id           = MMIMMS_INSERT_PIC_QUERY_WIN_ID;
    LOCAL BOOLEAN is_normal_close               = FALSE;
    
    editdoc_ptr = GetMMSReadEditDoc();
    //SCI_TRACE_LOW:"MMIMMS: HandleSendReadRptWaitingWin msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8140_112_2_18_2_38_50_356,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (MMIMMS_GetIsSendRPT())
        {
            MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_SEND_READRPT,IMAGE_PUBWIN_QUERY,&quer_win_id,PNULL);
        }
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    case MSG_FULL_PAINT:
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);;
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
    case MSG_PROMPTWIN_OK:
        //发送阅读报告
        {
            MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
            
            MMK_CloseWin(quer_win_id);
            mms_list_info_ptr = MMIMMS_GetListItemByID(editdoc_ptr->record_id);
            if (PNULL != mms_list_info_ptr)
            {
                if (MMIMMS_IsSimCardOK(mms_list_info_ptr->dual_sys))
                {
                    send_rpt_result = MMIMMS_SendReadReport(mms_list_info_ptr->dual_sys, editdoc_ptr);
                    if (MMIMMS_NO_ERROR != send_rpt_result)
                    {
                        MMIMMS_EditMMSAlertWin(send_rpt_result);
                        is_normal_close = TRUE;
                        MMK_CloseWin(win_id);                        
                        break;
                    }
                } 
                else
                {
                    MMIAPIPHONE_AlertSimNorOKStatusII(mms_list_info_ptr->dual_sys);
                    is_normal_close = TRUE;
                    MMK_CloseWin(win_id);                    
                }
            }            
            else
            {
                //trace: list info error
            }
        }
        break;
    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(quer_win_id);
        is_normal_close = TRUE;
        MMK_CloseWin(win_id);        
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        is_normal_close = TRUE;
        MMK_CloseWin(win_id);        
        break;
    case MSG_CLOSE_WINDOW:
        {
            if (is_normal_close)
            {
                MMIMMS_CreateMMSPreviewWin(editdoc_ptr, MMS_PLAY);
            }
            is_normal_close = FALSE;
        }
        break;
    case MSG_MMS_SEND_END:
        //发送完成，可以把彩信保存进入发件箱,根据发送结果提示用户        
        send_end_info_ptr = g_mms_send_recv_add_data.send_ptr;
        if (PNULL != send_end_info_ptr)
        {
            op_error = send_end_info_ptr->send_error;;
        }        
        switch(op_error) 
        {
        case MMIMMS_NO_ERROR:
#ifndef MMI_MMS_MINI_SUPPORT         
            //提示发送成功
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_SUCCEED, TXT_NULL, IMAGE_PUBWIN_SUCCESS);            
#endif
            break;
        case MMIMMS_ERROR_FAIL:
        case MMIMMS_ERROR_CANCEL:
        case MMIMMS_ERROR_NETLINK_FAILED:
        case MMIMMS_ERROR_SERVICE_DENIED:
        case MMIMMS_ERROR_FORMAT_CORRUPT:
        case MMIMMS_ERROR_ADDRESS_UNRESOLVED:
        case MMIMMS_ERROR_MESSAGE_NOT_FOUND:
        case MMIMMS_ERROR_SERVER_BUSY:
        case MMIMMS_ERROR_NOT_ACCEPTED:
        case MMIMMS_ERROR_UNSUPORTED_MESSAGE:
            //提示发送失败
            MMSMMI_OpenMMSAlertWin(TXT_COMM_SEND_FAILED, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            break;
        case MMIMMS_ERROR_NETLINK_FDN_ONLY:
            //提示FDN only
            MMSMMI_OpenMMSAlertWin(TXT_CC_FDN_ONLY, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            break;
        default:
            break;
        }
        is_normal_close = TRUE;
        MMK_CloseWin(win_id);        
        break;
    default :
        err_code = MMIPUB_HandleWaitWinMsg( win_id, msg_id, param);
        break;
    }    
    
    return err_code;   
}

/*****************************************************************************/
//  Description :get reading edit mms document 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIMMS_EDIT_DOCUMENT_T *  GetMMSReadEditDoc(void)
{
    return g_preview_globle.editdoc;
}

/*****************************************************************************/
//  Description :reading edit mms document play or preview
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint8  GetMMSPreviewOrPlay(void)
{
    return g_preview_globle.play_or_preview;
}

/*****************************************************************************/
//  Description :get preview is play video 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsPlayVideo(void)
{
    return g_preview_globle.is_play_video;
}

/*****************************************************************************/
//  Description :set preview is play video 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsPlayVideo(BOOLEAN is_play)
{
    g_preview_globle.is_play_video = is_play;
}

/*****************************************************************************/
//  Description :get preview is play music 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsPlayMusic(void)
{
    return g_preview_globle.is_play_music;
}

/*****************************************************************************/
//  Description :set preview is play music 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsPlayMusic(BOOLEAN is_play)
{
    g_preview_globle.is_play_music = is_play;
}

/*****************************************************************************/
//  Description :get preview play video time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 GetMMSPreviewPlayVideoTime(void)
{
    return g_preview_globle.video_play_time;
}

/*****************************************************************************/
//  Description :get preview is play video losefocus
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsPlayVideoLosefocus(void)
{
    return g_preview_globle.is_play_video_losefocus;
}

/*****************************************************************************/
//  Description :set preview is play video losefocus
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsPlayVideoLosefocus(BOOLEAN is_lose_foucs)
{
    g_preview_globle.is_play_video_losefocus = is_lose_foucs;
}

/*****************************************************************************/
//  Description :get preview is parser text
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsParserText(void)
{
    return g_preview_globle.is_parser_text;
}

/*****************************************************************************/
//  Description :set preview is parser text
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsParserText(BOOLEAN is_parser)
{
    g_preview_globle.is_parser_text = is_parser;
}

/*****************************************************************************/
//  Description :get preview is extract number/url
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsExtract(void)
{
    return g_preview_globle.is_extract_or_not;
}

/*****************************************************************************/
//  Description :set preview is extract number/url
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsExtract(BOOLEAN is_extract)
{
    g_preview_globle.is_extract_or_not = is_extract;
}

/*****************************************************************************/
//  Description :get preview is web to play
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN GetMMSPreviewIsWebPlay(void)
{
    return g_preview_globle.is_web_play;
}

/*****************************************************************************/
//  Description :set preview is web to play
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  SetMMSPreviewIsWebPlay(BOOLEAN is_play)
{
    g_preview_globle.is_web_play = is_play;
}

/*****************************************************************************/
//  Description :set preview play video time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void SetMMSPreviewPlayVideoTime(uint32 time)
{
    g_preview_globle.video_play_time = time;
}

/*****************************************************************************/
//  Description :reading edit mms document is send rpt get
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMMS_GetIsSendRPT(void)
{
    return g_preview_globle.is_send_rpt;
}

/*****************************************************************************/
//  Description :reading edit mms document is send rpt set
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_SetIsSendRPT(BOOLEAN is_send_rpt)
{
    g_preview_globle.is_send_rpt = is_send_rpt;
}

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
/*****************************************************************************/
//  Description :get pre font
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL GUI_FONT_T MMIMMS_GetPreFont(void)
{
    return g_preview_globle.character_font;
}

/*****************************************************************************/
//  Description :set pre font
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIMMS_SetPreFont(GUI_FONT_T font)
{
    g_preview_globle.character_font = font;
}
#endif

/*****************************************************************************/
//  Description :reading edit mms preview document init
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_MMSPreviewInit(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, uint8 play_or_preview)
{
    g_preview_globle.editdoc = edit_doc_ptr;
    //g_preview_globle.is_mms_noti_detail = FALSE;
    g_preview_globle.play_or_preview = play_or_preview; 
    SetMMSPreviewIsPlayMusic(FALSE);
    SetMMSPreviewIsPlayVideoLosefocus(FALSE);
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    MMIMMS_SetPreFont(MMI_DEFAULT_NORMAL_FONT);
#endif
}

/*****************************************************************************/
//  Description : reading edit mms preview document exit
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_MMSPreviewExit(void)
{
    //g_preview_globle.is_mms_noti_detail = FALSE;    
    g_preview_globle.is_send_rpt = FALSE;
    if(MMS_PLAY == g_preview_globle.play_or_preview)
    {
        if (PNULL != g_preview_globle.editdoc)
        {
            MMIMMS_DestroyEditDocument(g_preview_globle.editdoc);
            MMIMMS_MMSPreviewInit(PNULL, 0);
        }
    }
    g_preview_globle.play_or_preview = MMS_NONE;
}

/*****************************************************************************/
//  Description :create mms preview win 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateMMSPreviewWin(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr, MMS_DISPLAY_CONTENT play_or_pre)
{
    if (PNULL == edit_doc_ptr)
    {
        return;
    }
    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
    {
        if (MMS_PLAY== play_or_pre)
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_NOW_PREVIEWING, TXT_NULL, IMAGE_PUBWIN_WARNING);            
        }
        else if (MMS_PREVIEW== play_or_pre)
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_NOW_PLAYING, TXT_NULL, IMAGE_PUBWIN_WARNING);            
        }        
        return;
    }
    
    MMIMMS_MMSPreviewInit(edit_doc_ptr, play_or_pre);
    MMK_CreateWin((uint32*)MMIMMS_PREVIEW_TAB, PNULL);    
}

/*****************************************************************************/
//  Description :create mms detail read win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateMMSDetailShowWin(uint32 record_id)
{
    MMIMMS_SetPreviewMMSDetailFlag(TRUE);
    if (record_id == MMIMMS_GetNowReadMMSId())
    {
        MMK_CreateWin((uint32 *)MMIMMS_INBOX_NOTIDETAIL_WIN_TAB, (ADD_DATA)record_id);
    }    
}

/*****************************************************************************/
//  Description :mms preview play the current slide
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void  MMSPreviewPlayCurrentSlide(MMI_CTRL_ID_T richtext_ctrl_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    MMS_TEXT_IMAGE_ORDER_E text_ordor     = MMS_ORDER_TEXT_FIRST;

    if (PNULL == edit_doc_ptr)
    {
        return;
    }

    text_ordor = edit_doc_ptr->editbody_ptr->slide_cur->textimage_order;
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    MMIMMS_SetPreFont(MMIAPISMS_GetWinFont());
#endif
    if (MMS_ORDER_IMAGE_FIRST == text_ordor)
    {
        MmsPreviewAddPicAndMovieItem(edit_doc_ptr);
        MmsPreviewAddTextItem(edit_doc_ptr);         
    }
    else
    {
        MmsPreviewAddTextItem(edit_doc_ptr);
        MmsPreviewAddPicAndMovieItem(edit_doc_ptr);
    }    
    MmsPreviewAddMusicItem(edit_doc_ptr);

    GUIRICHTEXT_Update(MMIMMS_PREVIEW_CTRL_ID);
    return;
}

/*****************************************************************************/
//  Description :create mms send rpt waiting win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIMMS_CreateMMSRPTSendWaitingWin(MMIMMS_EDIT_DOCUMENT_T *    editdoc_ptr)
{
    MMI_STRING_T rpt_str = {0};
    
    MMIMMS_MMSPreviewInit(editdoc_ptr, MMS_PLAY);
    SCI_MEMSET(&rpt_str, 0, sizeof(rpt_str));
    MMI_GetLabelTextByLang(TXT_MMS_SENDING_READRPT, &rpt_str);
    MMIPUB_OpenWaitWin(1,&rpt_str,PNULL,PNULL,MMIMMS_SEND_MMSREADRPT_WAITING_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,(MMIPUB_HANDLE_FUNC)HandleSendReadRptWaitingWin);
}

/*****************************************************************************/
//  Description :mms preview richtext add text item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsPreviewAddTextItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    GUIRICHTEXT_ITEM_T item  = {0};/*lint !e64*/
    uint16 index             = 0;
    MMI_STRING_T text_str    = {0};
    
    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr)
    {
        if (edit_doc_ptr->editbody_ptr->slide_cur->text_flag == 1)
        {
            MMIMMS_GetCurrentSlideText(edit_doc_ptr, &text_str);
        }
    }
    if (PNULL == text_str.wstr_ptr)
    {
        return;
    }
    
    item.text_type = GUIRICHTEXT_TEXT_BUF;
    item.text_data.buf.str_ptr = text_str.wstr_ptr;
    item.text_data.buf.len = MMIAPICOM_Wstrlen(text_str.wstr_ptr);
    item.text_data.buf.len = MIN(item.text_data.buf.len, text_str.wstr_len);
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    item.text_set_font = TRUE;
    item.setting_font = MMIAPISMS_GetWinFont();    
#endif

    if (MMIMMS_Setting_GetExtractContent())
    {
        item.tag_type = GUIRICHTEXT_TAG_PARSE_PHONENUM | GUIRICHTEXT_TAG_PARSE_EMAIL;/*lint !e655*/

#ifdef BROWSER_SUPPORT
        item.tag_type = item.tag_type | GUIRICHTEXT_TAG_PARSE_URL;/*lint !e655*/
#endif
    } 
    GUIRICHTEXT_AddItem(MMIMMS_PREVIEW_CTRL_ID, &item, &index);    
}

/*****************************************************************************/
//  Description :mms preview richtext add pic or video item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsPreviewAddPicAndMovieItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    GUIRICHTEXT_ITEM_T item                = {0};/*lint !e64*/
    uint16 index                           = 0;
    uint8 *image_buf_ptr                   = PNULL;
    uint32 image_buf_len                   = 0;
    uint32 image_type                      = 0; 
    uint16 pic_width                      = 0;
    uint16 pic_height                     = 0;

    MMIMMS_GetPicDefaultWidthAndHeight(&pic_width, &pic_height);
    
    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr)
    {
        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->image_flag)//图片存在
        {
#ifdef DRM_SUPPORT
            uint8 file_name_str[MMIMMS_FILE_NAME_LEN + 1] = {0};            
            
            MMIMMS_GetCurrentSlideImageInfo(file_name_str, MMIMMS_FILE_NAME_LEN, edit_doc_ptr);
            if ((MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str))||(MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str)))
            {
                //SCI_TRACE_LOW:"MmsPreviewAddPicAndMovieItem drm file "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8624_112_2_18_2_38_52_357,(uint8*)"");
                item.img_type = GUIRICHTEXT_IMAGE_RES;
                item.img_data.res.id = IMAGE_DRM_PIC_LOCK_ICON;
                
                item.img_set_size = TRUE;
                item.img_setting_width  = pic_width;
                item.img_setting_height = pic_height;
                item.img_frame_type = GUIRICHTEXT_FRAME_NONE;
				GUIRICHTEXT_AddItem(MMIMMS_PREVIEW_CTRL_ID, &item, &index);
				if ( MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state )
				{
					MMIPUB_OpenAlertWarningWin(TXT_MMS_DRM_SAVE);
				}	
            }
#if 0
            else if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str))
            {
                MMIMMS_GetCurrentSlideImage(&image_type, &image_buf_len, &image_buf_ptr, edit_doc_ptr);
				if (PNULL != image_buf_ptr)
				{
					item.img_type = GUIRICHTEXT_IMAGE_BUF;
					item.img_data.buf.buf_ptr = image_buf_ptr;
					item.img_data.buf.size = image_buf_len; 
					item.tag_type = GUIRICHTEXT_TAG_NONE;
					GUIRICHTEXT_AddItem(MMIMMS_PREVIEW_CTRL_ID, &item, &index);
				}
            }
#endif
            else
#endif
            {
                MMIMMS_GetCurrentSlideImage(&image_type, &image_buf_len, &image_buf_ptr, edit_doc_ptr);
                if (PNULL != image_buf_ptr)
                {
                    item.img_type = GUIRICHTEXT_IMAGE_BUF;
                    item.img_data.buf.buf_ptr = image_buf_ptr;
                    item.img_data.buf.size = image_buf_len; 
                    item.tag_type = GUIRICHTEXT_TAG_NONE;
                    GUIRICHTEXT_AddItem(MMIMMS_PREVIEW_CTRL_ID, &item, &index);
                }
            }  
            SCI_TRACE_LOW("MmsPreviewAddPicAndMovieItem [CBK] has image!");
        }
        if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
        {
#ifdef DRM_SUPPORT
            uint8 file_name_str[MMIMMS_FILE_NAME_LEN + 1] = {0};
            
            MMIMMS_GetCurrentSlideVideoInfo(file_name_str, MMIMMS_FILE_NAME_LEN, edit_doc_ptr);            
            if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(file_name_str))
            {
                item.img_type = GUIRICHTEXT_IMAGE_RES;
                item.img_data.res.id = IMAGE_DRM_PIC_LOCK_ICON;
				if ( MMIMMS_PREVIEW_PLAYING == s_edit_mms_preview_state )
				{
					MMIPUB_OpenAlertWarningWin(TXT_MMS_DRM_SAVE);
				}
            }
            else
#endif
            {
                item.img_type = GUIRICHTEXT_IMAGE_RES;
                item.img_data.res.id = IMAGE_MMS_EDIT_VIDEO;
            }
            item.img_frame_type = GUIRICHTEXT_FRAME_NONE;            
            item.tag_type = GUIRICHTEXT_TAG_NONE;
            GUIRICHTEXT_AddItem(MMIMMS_PREVIEW_CTRL_ID, &item, &index);
            SCI_TRACE_LOW("MmsPreviewAddPicAndMovieItem [CBK] has video!");
        }
    }
    return;
}
/*****************************************************************************/
//  Description :mms preview richtext play music item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsPreviewPlayMusicItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    MMISRVAUD_RING_FMT_E         fmt = MMISRVAUD_RING_FMT_MAX;
    uint32  music_buf_len                  = 0;
    uint8 *music_buf_ptr                   = PNULL;
    //播放音乐for cr247444
    if (!MMIMMS_Setting_GetPlaysound() || MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {
        return;
    }
#ifdef DRM_SUPPORT
    {
        uint8 file_name[MMIMMS_FILE_NAME_LEN + 1] = {0};

        MMIMMS_GetCurrentSlideAudioInfo(file_name, MMIMMS_FILE_NAME_LEN, edit_doc_ptr);
        if (MMIFMM_DCF_FILE ==  MMIMMS_GetDrmFileTypeFromName(file_name))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);                   
            return;
        }        
    }
#endif
    if (MMIMMS_GetCurrentSlideAudio(&fmt, &music_buf_len, &music_buf_ptr, edit_doc_ptr))
    {
        //SCI_TRACE_LOW:"MmsPreviewAddMusicItem fmt=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8712_112_2_18_2_38_52_358,(uint8*)"d",fmt);
        if(MMISRVAUD_RING_FMT_MAX != fmt)
        {
            if (MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_OTHER))
            {   
                SetMMSPreviewIsPlayMusic(TRUE);

                MMIAPISET_PlayRingByPtr(MMISET_RING_TYPE_OTHER,
#ifdef MMI_MMS_ADJUST_VOLUME_SUPPORT                
                    MMIAPISET_GetMultimVolume(),
#else                    
                    MMIMMS_MULTIFILE_VOL_VALUE,
#endif                    
                    music_buf_ptr,
                    music_buf_len,
                    1,
                    fmt,
                    MmsRingPlayCallBack
                    );
            }
            else
            {
                //SCI_TRACE_LOW:"not permit play ring!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8730_112_2_18_2_38_52_359,(uint8*)"");
                MmsStopPreviewRing();
                // 关闭TTS播报，在函数内容有TTS_SUPPORT宏控制，如果没定义该函数，函数体为空...
                MmsStopPreviewTTSReading();
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MmsPreviewAddMusicItem audio type error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8738_112_2_18_2_38_52_360,(uint8*)"");
        }
    }
    else 
    {
        //SCI_TRACE_LOW:"MmsPreviewAddMusicItem data error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8743_112_2_18_2_38_52_361,(uint8*)"");
        return;
    }
}

/*****************************************************************************/
//  Description :mms preview richtext play 3gp item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MmsPreviewPlayVideoItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    
#ifdef VIDEO_PLAYER_SUPPORT    
    uint32 video_type     = MMS_TYPE_MAX;   
    DPLAYER_MEDIA_INFO_T media_info = {0};
    uint8 voice_volume    = MMIAPISET_GetCallRingVolume();//cr191444 MutMultimVolume can not be set and user set in idle or setting menu to make also VP's;
    VP_PLAY_PARAM_T play_param  = {0};
    GUI_RECT_T full_rect = {0};
    GUICTRL_STATE_T dispay_state     = 0;
    MMI_HANDLE_T play_ctrl_handle    = NULL;

    if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE))
    {//forcr247444
        return FALSE;
    }
#ifdef DRM_SUPPORT
    {
        uint8 file_name[MMIMMS_FILE_NAME_LEN + 1] = {0};

        MMIMMS_GetCurrentSlideVideoInfo(file_name, MMIMMS_FILE_NAME_LEN, edit_doc_ptr);
        if (MMIFMM_DCF_FILE ==  MMIMMS_GetDrmFileTypeFromName(file_name))
        {
            MMIPUB_OpenAlertWarningWin(TXT_DRM_COPYRIGHTS_INVLIAD_NOT_OPERATE);                   
            return FALSE;
        }        
    }
#endif
    SCI_MEMSET(&play_param,0,sizeof(VP_PLAY_PARAM_T));
    if (MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr))
    {
        //SCI_TRACE_LOW:"MmsPreviewPlayVideoItem video_type=%d, play_param.video_buf_info.video_buf_ptr = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8783_112_2_18_2_38_52_362,(uint8*)"dd",video_type,play_param.video_buf_info.video_buf_ptr);
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
        if(play_param.video_buf_info.video_buf_len > MMIMMS_LIMIT_VEDIO_SIZE)
        {
            return FALSE;
        }
#endif
        switch(video_type) 
        {
        case MMS_TYPE_3GP:
            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_3GP;
            break;
#ifdef AVI_DEC_SUPPORT
        case MMS_TYPE_AVI:
            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_AVI;
            break;
#endif
        case MMS_TYPE_MP4:
            play_param.video_buf_info.type = MMICOM_VIDEO_TYPE_MP4;
            break;
        default:
            MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            //SCI_TRACE_LOW:"MmsPreviewPlayVideoItem video type error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8799_112_2_18_2_38_52_363,(uint8*)"");
            return FALSE;
        }

           
  
        if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            MMSMMI_OpenMMSAlertWin(TXT_MMS_IN_CALL, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
        else if (MMIAPIMTV_IsMTVRunning())
        {
            MMSMMI_OpenMMSAlertWin(TXT_EXIT_MOBILETV, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
#ifdef BROWSER_SUPPORT
        else if (MMIAPIBROWSER_IsRunning())
        {
            MMSMMI_OpenMMSAlertWin(TXT_BROWSER_VEDIO_NOT_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
#endif
#ifdef VIDEO_PLAYER_SUPPORT 
        else if (MMIAPIVP_IsOpenPlayerWin())
        {
            MMSMMI_OpenMMSAlertWin(TXT_VP_VEDIO_NOT_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            return FALSE;
        }
#endif
        else
        {
            SetMMSPreviewIsPlayVideo(TRUE);
            MMIAPIVP_GetMediaInfoFromBuf(play_param.video_buf_info.video_buf_ptr,play_param.video_buf_info.video_buf_len,play_param.video_buf_info.type,&media_info);
            SetMMSPreviewPlayVideoTime(media_info.media_length);

            //不显示title             
            MMIMMS_SetWinStatusBarVisible(MMIMMS_PREVIEW_WIN_ID,FALSE);  
                
            //关闭3d效果
            //MMI_Enable3DMMI(FALSE);  
#ifdef UI_P3D_SUPPORT
            MMI_Disable3DEffect(MMI_3D_EFFECT_MMS_PREVIEW);
#endif            
            MMIAPISET_StopAllRing(FALSE);
            if (!MMIMMS_Setting_GetPlaysound())
            {
                voice_volume = MMISET_VOL_ZERO;
            }
            play_param.play_type = VP_PLAY_FROM_BUFFER;   
            
            play_param.display_param.audioSupport = 1;
            play_param.display_param.videoSupport = 1;
            play_param.display_param.RotateAngle = 1;          
            play_param.is_scal_up = TRUE;
            play_param.display_param.disp_rect.x = 0;
            play_param.display_param.disp_rect.y = 0;

            play_param.display_param.RotateAngle = GUILCD_GetLogicAngle(GUI_MAIN_LCD_ID); 
            full_rect = MMITHEME_GetFullScreenRect();   
            if (LCD_ANGLE_0 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_90;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else if(LCD_ANGLE_180 == play_param.display_param.RotateAngle)
            {
                play_param.display_param.RotateAngle = LCD_ANGLE_270;
                play_param.display_param.disp_rect.dy = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dx = full_rect.bottom - full_rect.top + 1;
            }
            else         
            {
                play_param.display_param.disp_rect.dx = full_rect.right - full_rect.left + 1;           
                play_param.display_param.disp_rect.dy = full_rect.bottom - full_rect.top + 1;
            }

            play_param.display_param.target_rect.x = play_param.display_param.disp_rect.x;
            play_param.display_param.target_rect.y = play_param.display_param.disp_rect.y;
            play_param.display_param.target_rect.dx = play_param.display_param.disp_rect.dx;
            play_param.display_param.target_rect.dy = play_param.display_param.disp_rect.dy;
            play_param.display_param.disp_mode = DPLAYER_DISP_CUSTOMIZE;

            play_param.vol = voice_volume;

            
            if (MMIAPIVP_Play(&play_param,MmsVideoPlayCallBack,0))
            {
                dispay_state = GUICTRL_STATE_DISABLE_TP;
                play_ctrl_handle = MMK_GetCtrlHandleByWin(MMIMMS_PREVIEW_WIN_ID, MMIMMS_PREVIEW_CTRL_ID);
                IGUICTRL_SetState(MMK_GetCtrlPtr(play_ctrl_handle), dispay_state, TRUE);
                play_ctrl_handle = MMK_GetCtrlHandleByWin(MMIMMS_PREVIEW_WIN_ID, MMIMMS_BUTTON_LF_CTRL_ID);
                IGUICTRL_SetState(MMK_GetCtrlPtr(play_ctrl_handle), dispay_state, TRUE);
                play_ctrl_handle = MMK_GetCtrlHandleByWin(MMIMMS_PREVIEW_WIN_ID, MMIMMS_BUTTON_RH_CTRL_ID);
                IGUICTRL_SetState(MMK_GetCtrlPtr(play_ctrl_handle), dispay_state, TRUE);
                GUIWIN_SetSoftkeyTPEnable(MMIMMS_PREVIEW_WIN_ID, FALSE);
                MMK_SetWinSupportAngle(MMIMMS_PREVIEW_WIN_ID, WIN_SUPPORT_ANGLE_CUR);
            }
            return TRUE;
        }                
           
    
    }
    else 
#endif
    {
        //SCI_TRACE_LOW:"MmsPreviewPlayVideoItem data error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8902_112_2_18_2_38_52_364,(uint8*)"");
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :mms preview richtext add music item 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsPreviewAddMusicItem(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    GUIRICHTEXT_ITEM_T item                = {0};/*lint !e64*/
    uint16 index                           = 0;
    wchar music_name_wstr[MAX_NAME_LENGTH] = {0};
    uint16 music_name_wlen                 = 0;
    uint8 *music_name_ptr                  = PNULL;
    uint8 music_name_len                   = 0;
    // uint32  music_buf_len                  = 0;
    // uint8 *music_buf_ptr                   = PNULL;
    
    if (PNULL != edit_doc_ptr && PNULL != edit_doc_ptr->editbody_ptr)
    {
        if (0  != edit_doc_ptr->editbody_ptr->slide_cur->audiosize)//音乐存在
        {
            static P_MMS_SLIDE_T p_mms_slide_temp = NULL;            
       
            item.tag_type = GUIRICHTEXT_TAG_NONE;
            item.align_type = GUIRICHTEXT_ALIGN_DEFAULT;            
            item.text_type = GUIRICHTEXT_TEXT_BUF;
            p_mms_slide_temp = edit_doc_ptr->editbody_ptr->slide_cur;//edit_doc_ptr->editbody_ptr->slide_root;
            // while(p_mms_slide_temp)
            {
                if (0 != p_mms_slide_temp->audio_type)
                {
                    music_name_ptr = (uint8 *)p_mms_slide_temp->audiofile;
                    music_name_len = (uint8)strlen((char *)music_name_ptr);
                    music_name_wlen = GUI_UTF8ToWstr(music_name_wstr, music_name_len, music_name_ptr, music_name_len);
                    //music_name_wlen = MMIMMS_GetFileNameFromVfname(edit_doc_ptr,music_name_wstr,MAX_NAME_LENGTH,music_name_ptr,music_name_len);
                }
                // p_mms_slide_temp = p_mms_slide_temp->next;
            }  
#ifdef DRM_SUPPORT  
			if (MMIFMM_DM_FILE == MMIMMS_GetDrmFileTypeFromName(music_name_ptr) || MMIFMM_DCF_FILE == MMIMMS_GetDrmFileTypeFromName(music_name_ptr))
			{
				item.img_type = GUIRICHTEXT_IMAGE_RES;
				item.img_data.res.id = IMAGE_DRM_AUDIO_LOCK_ICON;
			}
			else              
#endif
			{
				item.img_type = GUIRICHTEXT_IMAGE_RES;
				item.img_data.res.id = IMAGE_SECMENU_ICON_MULTIM_MP3;//显示资源图片：音频.bmp
			}
            item.text_data.buf.str_ptr = music_name_wstr;
            item.text_data.buf.len = music_name_wlen; 
#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
            item.text_set_font = TRUE;
            item.setting_font = MMIAPISMS_GetWinFont();
#endif
            
            GUIRICHTEXT_AddItem(MMIMMS_PREVIEW_CTRL_ID, &item, &index);            
        }    
    }
}

/*****************************************************************************/
//  Description :music play callback
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsRingPlayCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    //SCI_TRACE_LOW:"MmsRingPlayCallBack result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_8973_112_2_18_2_38_52_365,(uint8*)"d",result);
    if(MMISRVAUD_REPORT_RESULT_STOP != result)
    {
        SetMMSPreviewIsPlayMusic(FALSE);
        if(MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID) || MMK_IsOpenWin(MMIMMS_MULTIFILE_VIEW_WIN_ID))
        {
            if(MMISRVAUD_REPORT_RESULT_SUCESS != result)
            {
                MMSMMI_OpenMMSAlertWin(TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING);            
            }
        }
    }
}

/*****************************************************************************/
//  Description :video play callback
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
#ifdef VIDEO_PLAYER_SUPPORT
LOCAL void MmsVideoPlayCallBack(VP_CALLBACK_T *msg_ptr)
{
    GUICTRL_STATE_T dispay_state     = 0;
    MMI_HANDLE_T view_ctrl_handle    = NULL;
    MMI_HANDLE_T play_ctrl_handle    = NULL;
        
    if (PNULL == msg_ptr)
    {
        return;
    }    
    
    //SCI_TRACE_LOW:"[VP] MmsVideoPlayCallBack msg_ptr->msg_id = %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_9003_112_2_18_2_38_52_366,(uint8*)"d",msg_ptr->msg_id);
    switch(msg_ptr->msg_id) 
    {       
    case VP_STOP_IND: 
        if (MMK_IsOpenWin(MMIMMS_MULTIFILE_VIEW_WIN_ID))
        {
            dispay_state = GUICTRL_STATE_DISABLE_TP;
            view_ctrl_handle = MMK_GetCtrlHandleByWin(MMIMMS_MULTIFILE_VIEW_WIN_ID, MMIMMS_MULTIFILE_VIEW_CTRL_ID);
            IGUICTRL_SetState(MMK_GetCtrlPtr(view_ctrl_handle), dispay_state, FALSE);
            GUIWIN_SetSoftkeyTPEnable(MMIMMS_MULTIFILE_VIEW_WIN_ID, TRUE);
            
            MMK_PostMsg(MMIMMS_MULTIFILE_VIEW_WIN_ID,MSG_MMS_END_VIDEO,PNULL,PNULL);
            MMK_PostMsg(MMIMMS_MULTIFILE_VIEW_WIN_ID,MSG_FULL_PAINT,PNULL,PNULL);
            MMK_SetWinSupportAngle(MMIMMS_MULTIFILE_VIEW_WIN_ID, WIN_SUPPORT_ANGLE_ALL);
            MMIMMS_SetWinStatusBarVisible(MMIMMS_MULTIFILE_VIEW_WIN_ID,TRUE);                          
        }
        else
        {
            dispay_state = GUICTRL_STATE_DISABLE_TP;
            play_ctrl_handle = MMK_GetCtrlHandleByWin(MMIMMS_PREVIEW_WIN_ID, MMIMMS_PREVIEW_CTRL_ID);
            IGUICTRL_SetState(MMK_GetCtrlPtr(play_ctrl_handle), dispay_state, FALSE);
            play_ctrl_handle = MMK_GetCtrlHandleByWin(MMIMMS_PREVIEW_WIN_ID, MMIMMS_BUTTON_LF_CTRL_ID);
            IGUICTRL_SetState(MMK_GetCtrlPtr(play_ctrl_handle), dispay_state, FALSE);
            play_ctrl_handle = MMK_GetCtrlHandleByWin(MMIMMS_PREVIEW_WIN_ID, MMIMMS_BUTTON_RH_CTRL_ID);
            IGUICTRL_SetState(MMK_GetCtrlPtr(play_ctrl_handle), dispay_state, FALSE);
            GUIWIN_SetSoftkeyTPEnable(MMIMMS_PREVIEW_WIN_ID, TRUE);

            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID,MSG_MMS_END_VIDEO,PNULL,PNULL);
            MMK_SetWinSupportAngle(MMIMMS_PREVIEW_WIN_ID, WIN_SUPPORT_ANGLE_ALL);            
            MMIMMS_SetWinStatusBarVisible(MMIMMS_PREVIEW_WIN_ID,TRUE);              
        }
        
        SetMMSPreviewIsPlayVideo(FALSE);
        //关闭3d效果
        //MMI_Enable3DMMI(TRUE);
#ifdef UI_P3D_SUPPORT
        MMI_Enable3DEffect(MMI_3D_EFFECT_MMS_PREVIEW);
#endif        
        MMIAPISET_StopAllRing(TRUE);
        if (
            //!GetMMSPreviewIsPlayVideoLosefocus() && 
            !MMK_IsOpenWin(MMIMMS_MULTIFILE_VIEW_WIN_ID)
            )
        {
            //SCI_TRACE_LOW:"MmsVideoPlayCallBack "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_PREVIEWWIN_9047_112_2_18_2_38_52_367,(uint8*)"");
            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_PostMsg(MMIMMS_PREVIEW_WIN_ID, MSG_FULL_PAINT, PNULL,PNULL);
            SetMMSPreviewIsPlayVideoLosefocus(FALSE);
        }
        else
        {
            SetMMSPreviewIsPlayVideoLosefocus(FALSE);
        }         
        break;


    default:
        break;
    }
}
#endif
/*****************************************************************************/
//  Description :stop music 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MmsStopPreviewRing(void)
{
    MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
}
/*****************************************************************************/
//  Description :out box send old mms  
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_OutboxSendMMS(uint32 record_id, MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    MMIMMS_ERROR_E   send_result           = MMIMMS_NO_ERROR;
    MMISMS_ORDER_INFO_T *mms_list_info_ptr = PNULL;
    uint16 sim_sys                        = MN_DUAL_SYS_1;
    uint32 sim_num                        = 0;
    MN_DUAL_SYS_E send_dual_sim            = MN_DUAL_SYS_1;

    if (PNULL == edit_doc_ptr)
    {
        return;
    }        
    mms_list_info_ptr = MMIMMS_GetListItemByID(edit_doc_ptr->record_id); 

    if (PNULL == mms_list_info_ptr)
    {
        return;
    }
    
    MMIMMS_SetMMSSendStatus(SEND_OUTBOX_MMS);

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
        return;
    }
    else if (1 == sim_num)
    {
        send_dual_sim = (MN_DUAL_SYS_E)sim_sys;

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        if (!MMIAPICC_IsSimServiceAvail(send_dual_sim))
        {
            MMIAPICC_OpenSIMCallingPrompt();
            return;
        }
#endif

        MMSMMI_CreateSendingWaitingWin(send_dual_sim, record_id);
    }
    else
    {
        MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_sel = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_MMS);
        if(major_sim_sel == MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
        {
            MMIMMS_CreateSimCardSelectWin(record_id);
            return;
        }
        else if(major_sim_sel < MMISET_MAJOR_SIM_SEL_MANUAL_SEL)
        {
            send_dual_sim = (MN_DUAL_SYS_E)(MN_DUAL_SYS_1+(major_sim_sel-MMISET_MAJOR_SIM_SEL_SIM1));/*lint !e656*/

#ifndef MMI_MULTI_SIM_SYS_SINGLE
            if (!MMIAPICC_IsSimServiceAvail(send_dual_sim))
            {
                MMIAPICC_OpenSIMCallingPrompt();
                return;
            }
#endif
            
            MMSMMI_CreateSendingWaitingWin(send_dual_sim, record_id);
        }
        else
        {
            //do nothing now!!!
        }
    } 

    send_result= MMIMMS_SendOldMMS(send_dual_sim, record_id);
#ifdef MMI_SMS_CHAT_SUPPORT    
    MMIAPISMS_UpdateResendMMSLabel(record_id);
#endif
    if (MMIMMS_NO_ERROR != send_result)
    {
        MMIMMS_EditMMSAlertWin(send_result);
    }
    if (MMK_IsOpenWin(MMIMMS_PREVIEW_WIN_ID))
    {
        MMK_CloseWin(MMIMMS_PREVIEW_WIN_ID);
    }
}


/*****************************************************************************/
//  Description :set if it is notifycation detail 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMMS_SetPreviewMMSDetailFlag(BOOLEAN is_mms_noti_detail)
{
    g_preview_globle.is_mms_noti_detail = is_mms_noti_detail;
}

/*****************************************************************************/
//  Description :get if it is notifycation detail 
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetPreviewMMSDetailFlag(void)
{
    return g_preview_globle.is_mms_noti_detail;
}
/*****************************************************************************/
//  Description :play mms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN BoxPlayMMS(MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr)
{
    MMI_CTRL_ID_T richtext_ctrl_id           = MMIMMS_PREVIEW_CTRL_ID;
    MMIMMS_WINDOW_ID_E win_id = MMIMMS_PREVIEW_WIN_ID;
    
#ifdef NANDBOOT_SUPPORT
    if(PNULL == edit_doc_ptr)
    {
        return FALSE;
    }
#ifdef VIDEO_PLAYER_SUPPORT    
    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
        MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);  
    }
    else
#endif  
/*
#ifdef DRM_SUPPORT
	if (MMIFMM_FILE_TYPE_DRM == MMIAPIFMM_GetFileTypeByFileName(
		edit_doc_ptr->editbody_ptr->slide_root->content_root->filename,
		SCI_STRLEN(edit_doc_ptr->editbody_ptr->slide_root->content_root->filename)))
	{
		MMIPUB_OpenAlertWarningWin(TXT_NULL);
	} 
	else
#endif*/
    {
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, STXT_STOP, STXT_RETURN, TRUE);
        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
    }
#else
    if(PNULL == edit_doc_ptr)
    {
        return FALSE;
    }
#ifdef VIDEO_PLAYER_SUPPORT    
    if (1 == edit_doc_ptr->editbody_ptr->slide_cur->video_flag)
    {
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
        uint32 video_type     = MMS_TYPE_MAX;   
        VP_PLAY_PARAM_T play_param  = {0};
        MMIMMS_GetCurrentSlideVideo(&video_type, &(play_param.video_buf_info.video_buf_len), &(play_param.video_buf_info.video_buf_ptr), edit_doc_ptr);
        
        if(play_param.video_buf_info.video_buf_len <= MMIMMS_LIMIT_VEDIO_SIZE)
#endif
        {
            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_PostMsg(win_id,MSG_MMS_PLAY_VIDEO,PNULL,PNULL);   
        }
#ifdef MMIMMS_VIDEO_PREVIEW_SIZE_LIMIT
        else
        {
#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
            GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_PAUSE, STXT_RETURN, FALSE);
            MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
            MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
        }
#endif
    }
    else
#endif        
    {
#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
#endif
        GUIWIN_SetSoftkeyTextId(win_id,TXT_NULL,TXT_PAUSE, STXT_RETURN, FALSE);
        MMK_PostMsg(win_id, MSG_MMS_PLAY_SLIDE, PNULL,PNULL);
        MMK_PostMsg(win_id, MSG_FULL_PAINT, PNULL,PNULL);
	}
#endif                        
    s_edit_mms_preview_state = MMIMMS_PREVIEW_PLAYING;
    GUIRICHTEXT_DisableClipBoard(MMIMMS_PREVIEW_CTRL_ID);
    MMIDEFAULT_AllowTurnOffBackLight(FALSE);
    if (MMS_PLAY == GetMMSPreviewOrPlay())
    {
        GUIRICHTEXT_SetFocusMode(richtext_ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
    }
    
    return TRUE;
}
/*****************************************************************************/
//  Description :stop tts 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL void MmsStopPreviewTTSReading(void)
{
#ifdef TTS_SUPPORT
    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_MMS);
#endif

}

#ifdef TTS_SUPPORT
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MmsIsSuitableForTTS(void)
{
    BOOLEAN                is_suitable             = FALSE;
    MMIMMS_EDIT_DOCUMENT_T * edit_doc_ptr          = PNULL;

    if(MMIMMS_PREVIEW_STOP == s_edit_mms_preview_state &&  MMS_PLAY == GetMMSPreviewOrPlay())
    {
        edit_doc_ptr = GetMMSReadEditDoc();
        if(PNULL != edit_doc_ptr)
        {
            // 如果包含文本
            if (1 == edit_doc_ptr->editbody_ptr->slide_cur->text_flag)
            {
                is_suitable = TRUE;
            }
        }
        
    }

    return is_suitable;
}
/*****************************************************************************/
//  Description :显示TTS 按键，如果条件适当的话, 如果不适合，则返回FALSE，交由其他处理
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MmsShowTTSKeyIfSuitable(MMIMMS_WINDOW_ID_E win_id, BOOLEAN is_refresh)
{
    BOOLEAN has_showTTS = FALSE;
    if(MmsIsSuitableForTTS())
    {
        // 如果TTS正在播放中
        if(MMIAPITTS_IsPlayRing(TTS_RING_TYPE_MMS))
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, STXT_STOP, STXT_RETURN, is_refresh);
        }
        else
        {
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_SET_TTS_PLAY, STXT_RETURN, is_refresh);
        }
        
        has_showTTS = TRUE;
    }
    
    return has_showTTS;
}
#endif

/*****************************************************************************/
//  Description : is status bar visible
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetWinStatusBarVisible(MMI_WIN_ID_T win_id, BOOLEAN is_visible)
{
    IGUICTRL_T* ctrl_ptr = PNULL;  
    
    ctrl_ptr = MMK_GetCtrlPtrByWin(MMK_GetWinHandle(MMK_GetFirstAppletHandle(), win_id), MMITHEME_GetStatusBarCtrlId());
    if(PNULL != ctrl_ptr )
    {
        IGUICTRL_SetState(ctrl_ptr , GUICTRL_STATE_INVISIBLE , (BOOLEAN)!is_visible);   //取非操作
    }
}

/*Edit by script, ignore 2 case. Thu Apr 26 19:00:59 2012*/ //IGNORE9527


/*Edit by script, ignore 13 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
