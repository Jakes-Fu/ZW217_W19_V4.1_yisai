/****************************************************************************
** File Name:      mmicc_app.c                                             *
** Author:                                                                 *
** Date:           01/12/2003                                              *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003        figo.feng         Create
** 12/2003        louis.wei         add CW, CH,MPTY, auto redial and Fire wall
****************************************************************************/
#define _CC_APP_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_cc_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmk_timer.h"
#include "mmiphone_export.h"
#include "mmi_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "window_parse.h"
#include "guitext.h"
#include "guilcd.h"
#include "mmi_menutable.h"
#include "guilistbox.h"
#include "mmi_image.h"
#include "mmipb_export.h"
#include "mmicl_export.h"

#include "mmidisplay_data.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmiidle_export.h"
#include "mmi_default.h"
#include "mmi_textfun.h"
#include "mmi_appmsg.h"
#include "mmitv_out.h"
#include "mmiparse_export.h"
#ifdef WIN32
#else
#include "gpio_prod_api.h"
#include "audio_config.h"
#endif
#include "mmisms_export.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmiudisk_export.h"
#include "mmisd_export.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
#include "mmibt_export.h" // baokun add
#include "mmiset_id.h"
#include "mmifdn_export.h"
#include "mmicc_internal.h"
#include "mmicc_id.h"
#include "mmicc_nv.h"
#include "mmicc_text.h"
#include "mmicc_image.h"
#include "mmicc_anim.h"
#include "guimenu.h"
#include "guistatusbar.h"
#include "mmipub.h"
#include "mmitheme_softkey.h"
#include "mmiset_image.h" 
#include "mmivt_export.h"
#include "mmiussd_export.h"
//#include "mmiset_set_ring.h"
//#include "mmiset_call.h"
#include "mmiset_call_export.h"
//#include "mmist_export.h"
#include "mmi_nv.h"

#include "guilabel.h"
#include "mmicountedtime_export.h"//@zhaohui,counted time
#include "mmiudisk_export.h"
#include "mmi_mainmenu_export.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmifmm_export.h"
#include "mmidc_export.h"
#include "mmistk_export.h"
#include "mmicc_audio.h"
#include "mmipicview_export.h"
#include "guiedit.h"
#include "mmimp3_export.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef TTS_SUPPORT
#include "mmitts_export.h"
#include "Mmisrvaud_api.h"
//#include "mmiset_display.h"
#endif
#include "mmidm_export.h"
#include "mmimtv_export.h"

#ifdef CSC_XML_SUPPORT
#include "guiurl.h"
#endif

#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif

#ifdef ASP_SUPPORT
#include "mmiasp_export.h"
#endif

#ifdef MMIWIDGET_SUPPORT
#include "mmiwidget_export.h"
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#ifdef  VT_SUPPORT
#include "mmivt_kernel.h"
#endif

#ifdef FM_SUPPORT 
#include "mmifm_export.h"
#endif

#ifdef DRM_SUPPORT
#include "mmidrm_export.h"
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif

#if !defined(MMI_GUI_STYLE_TYPICAL)
#include "mmi_resource.h"
#endif
#ifdef MET_MEX_SUPPORT
#include "met_mex_export.h"
#ifdef MET_MEX_AQ_SUPPORT
#include "met_mex_aq.h"
#endif
#endif

#include "mmiidle_statusbar.h"
#include "mmimultim_image.h"
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#include "mmisrvrecord_export.h"
#endif
#ifdef MMS_SUPPORT
#include "Mmimms_export.h"
#endif

#ifdef BT_DIALER_SUPPORT
#include "mmicc_internal.h"
// #include "os_alloc.h"
#endif
#include "sig_code.h"
#include "mn_type.h"
#include "mn_api.h"
#include "watch_gallery_export.h"
#include "ual_bt.h"
#ifdef UAL_BT_HFU_SUPPORT
#include "ual_bt_hfu.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


#define MMI_10MIN   600000

#define MMI_DTMF_TONE_PLAY_TIME 300


#define CC_REDIAL_PROGRESS_TOTAL    100
#define CC_MPTY_MENU                2
#define CC_ANSWER3_MENU				9
#define CC_NO_NUM					0

#define ANIMATION_FRAME_NUM		4
#define	MMICC_3SECOND			3000

#define CC_AUTO_DIAL_WIN_BG_COLOR 0xFF56	//RGB: 255, 234, 181


#define DTMF_MN_SYNC_IND_BIT            0x01
#define DTMF_MN_PROGRESS_IND_BIT        0x02


#define MMICC_DISCONNECT_CAUSE_FOR_REDIAL 0x0a
#define MMICC_ERROR_IND_MAX_REDIAL_TIME		1

#define MMI_DETACHCNF_WAIT_TIME   30000

//MT的时候，当sync_ind中tch资源分配了并且上层已经发起接通电话，则应该打开codec cr133748
#define CC_TCH_RESOURCE_ASSIGNED    0x01    //tch资源已经分配标志位
#define CC_MT_CALL_CONNECTING   0x02    //接通电话的动作已经发起

#define CC_CODE_PLUS  0x2b  // ascii code +

#ifdef BT_DIALER_SUPPORT
#define MMI_SEND_SCO_REQUEST_TIMEOUT 500
#endif

#define CC_WATCH_PLAY_CALL_END_HINT_TIME    3   //播放通话结束提醒音的次数

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
//呼叫方式，分mmi发起和非mmi发起，收到start_ind消息的时候，如果不是mmi发起，则认为是非mmi方式发起
typedef enum
{
    CC_DIAL_METHOD_NULL,    //初始默认值
    CC_DIAL_METHOD_MMI,     //mmi发起电话呼叫
    CC_DIAL_METHOD_OTHER,   //非mmi方式发起电话呼叫
    CC_DIAL_METHOD_MAX  
}CC_DIAL_METHOD_E;

//MO calling状态时，打开下行声音的时机 相关标志位
#define MO_CALL_STATE_INIT              0x00
#define MO_CALL_STATE_SYNC              0x01
#define MO_CALL_STATE_SETUP_OR_PROGRESS 0x02
#define MO_CALL_STATE_DONE              (MO_CALL_STATE_SYNC|MO_CALL_STATE_SETUP_OR_PROGRESS)

#ifdef CSC_XML_SUPPORT
typedef struct MMICC_CSC_SOS_NUM_INFO_Tag
{
    uint8 *sos_num_ptr;

    struct MMICC_CSC_SOS_NUM_INFO_Tag *next; 
}MMICC_CSC_SOS_NUM_INFO_T;

typedef struct MMICC_CSC_SOS_NUM_Tag
{
    uint8 sos_num;

    MMICC_CSC_SOS_NUM_INFO_T *sos_num_info_ptr;
}MMICC_CSC_SOS_NUM_T;
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL CC_CALL_CONTEXT_T s_call_context;//当前CC模块的总体情况

LOCAL APP_MN_SETUP_IND_T s_setupind_param;   //存储setup_ind消息相关信息

#ifdef ASP_SUPPORT
LOCAL uint8 s_asp_call_id = 0;  //正在答录的电话id
LOCAL BOOLEAN s_accept_call  = FALSE;   // 是否接听了电话.....
#endif

/*
1) 112, 911任何情况下都是紧急呼叫号码
2) 无卡时，000, 08, 110, 999, 118, and 119 都是紧急呼叫号码
*/
//emergency call table
static const char* const s_emergency_call[] = 
{
    "112", "911"
};

static const char* const s_emergency_call_no_sim[] =
{
    "000", "08", "110", "999", "118", "119"
};

//MS00186848 用于自动测试中,避免呼出一些报警求救电话等
static const char* const s_eng_call_barring_num[] =
{
    "110", "119", "120", "122", "12315", "12395", "95119"
};

LOCAL BOOLEAN       s_call_ready[MMI_DUAL_SYS_MAX] = {FALSE};    //sim卡是否初始化完成并可以进行电话操作

LOCAL BOOLEAN s_is_need_call_after_hold = FALSE; //cr132811 收到hold cnf消息后是否需要发起call

//cr133748 用来判断mt的时候，是否需要打开voice codec;
LOCAL uint8 s_mt_need_open_codec_flag = 0;  //mt的时候此flag必须等于CC_TCH_RESOURCE_ASSIGNED|CC_MT_CALL_CONNECTING,打开voice codec.打开或者收到disconnected_ind和err_ind之后，重置该值
LOCAL uint8 s_is_downlnk_mute_mocalling_flag = 0; //mo call时关闭/打开下行标志位
LOCAL BOOLEAN s_is_downlnk_mute_full_circle = TRUE;

LOCAL uint8 s_network_ring_timer =0; //tch没有分配，对方已经振铃，播放本地回铃音的计时器

LOCAL CC_DIAL_METHOD_E s_dial_method = CC_DIAL_METHOD_NULL; //当前电话由谁发起

LOCAL uint8 s_current_dtmf_call_id = CC_MAX_CALL_NUM; //start dtmf call id
LOCAL BOOLEAN               s_is_new_missed_call ;  //某次通话之后，是否有新的未接来电

LOCAL uint16 s_disc_info_cause = 0; //for record cause param of APP_MN_DISC_INFO_IND

LOCAL BOOLEAN s_is_call_modified_store = FALSE;
LOCAL MN_CALLED_NUMBER_T s_call_modified_address = {0};

#ifdef CSC_XML_SUPPORT
LOCAL MMICC_CSC_SOS_NUM_T s_cc_csc_sos = {0};
LOCAL MMICC_CSC_SOS_NUM_T s_cc_csc_no_sim_sos = {0};
#endif

#ifdef BT_DIALER_SUPPORT
LOCAL uint8   		s_bt_current_call_index = CC_INVALID_INDEX;
LOCAL BOOLEAN s_bt_call_flag = FALSE;
LOCAL BOOLEAN s_bt_setupind_flag = FALSE;
LOCAL BOOLEAN s_bt_connect_wait_exist =FALSE;
LOCAL BOOLEAN s_bt_audconnind_flag =FALSE;
LOCAL uint8   s_current_index      = CC_INVALID_INDEX;
BOOLEAN g_call_disc_by_bt    = FALSE;
LOCAL uint8 s_sco_request_timer = 0;
LOCAL BOOLEAN s_bt_sco_link_is_exist = FALSE;
LOCAL CC_MAKE_CALL_INFO s_bt_make_call_info = {0};
LOCAL BT_HFU_CIEV_T s_bt_ciev_info = {0};
LOCAL uint8 s_bt_clcc_record_count =0;
#endif

#ifdef MMI_INDIAN_SOS_SUPPORT
typedef struct MMICC_SOS_CALL_INFO_Tag{
	BOOLEAN  sos_available;
	uint8  sos_call_id;
	uint8 sos_num_index;
	MN_DUAL_SYS_E sos_dual_sys;
	uint8 telnum[MMISET_SOS_CONTACTS_MAX_NUM][CC_MAX_TELE_NUM_LEN];
	uint8 num_count;
	MMISET_SOS_CALLBACK_PFUNC callback;
}MMICC_SOS_CALL_INFO_T;

LOCAL MMICC_SOS_CALL_INFO_T s_sos_call_info = {0};
#endif

LOCAL APP_MN_EMG_NUM_LIST_IND_T *s_cc_network_emg_num_list_ptr[MMI_DUAL_SYS_MAX];

LOCAL MMICC_MO_CALL_ERROR_TYPE_E  s_cc_mo_error_type = CC_MO_ERROR_NONE;

#ifdef MMI_RECORD_SUPPORT
LOCAL RECORD_SRV_HANDLE  s_cc_watch_record_handle = PNULL;
#endif
LOCAL uint8 s_cc_call_end_hint_timer = 0;

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
#ifdef DPHONE_SUPPORT
LOCAL void ResetCallHandfreeState(void);
PUBLIC void CC_SetDevModeAndVolume(AUDIO_DEVICE_MODE_TYPE_E mode);
#endif

/*****************************************************************************/
// 	Description : handle MT Call disconnect
//	Global resource dependence : 
//  Author:figo.feng
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMTCallDisc(DPARAM param);
//@$disscus 这个函数68上没有，是从MMIAPICC_ProcessPhoneNumExt重整出来的，是否需要加宏？见issue1
/*****************************************************************************/
// 	Description : process phone number
//	Global resource dependence : 
//  Author:bin.ji   
//	Note:2004.9.25
/*****************************************************************************/ 
LOCAL CC_RESULT_E ProcessPhoneNumExt(
    MN_DUAL_SYS_E dual_sys,
    uint8 *tele_num,
    uint8 tele_len,
    MMI_STRING_T* disp_ptr,
    MMI_STRING_T* name_ptr,
    BOOLEAN	clir_present,  //true, not case the value of clir_type
    uint8		clir_type,
    uint32 call_type, //call type,
    MN_SUBADDR_T* sub_addr
    );

/*****************************************************************************/
// 	Description : make a call from pb or not from pb
//	Global resource dependence : s_call_context
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E MakeCallReqExt(
    MN_DUAL_SYS_E dual_sys,
    uint8 *src_tele_num,
    uint8 src_tele_len,
    MMI_STRING_T* disp_ptr,
    MMI_STRING_T* name_ptr,
    BOOLEAN	clir_present,  //true, not case the value of clir_type
    uint8		clir_type,
    uint32 mmi_call_type, //call type,
    MN_SUBADDR_T* sub_addr
    );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetWantMoCallInfo(MN_DUAL_SYS_E dual_sys,
                                                                uint8 *tele_num,
                                                                uint16 tele_len,
                                                                MMI_STRING_T* disp_ptr,
                                                                MMI_STRING_T* name_ptr,
                                                                MN_SUBADDR_T *sub_addr,
                                                                CC_WANT_MO_CALL_T *want_mo_call_ptr);

/*****************************************************************************/
//  Description : get current call number
//  Global resource dependence : none
//  Author: baokun
//  Note: 
/*****************************************************************************/
LOCAL void GetCurrentCallNumber(MMIPB_BCD_NUMBER_T *bcd_number_ptr, BOOLEAN is_remove_ip_head, BOOLEAN is_want_mo_call);

/*****************************************************************************/
// 	Description : 是否存在正在建立mpty，分离mpty，断开等call
//	Global resource dependence : s_call_context
//  Author:bin.ji
//	Note:2004.11.26
/*****************************************************************************/
LOCAL BOOLEAN CC_IsExistOperatedCall(void);

#if 0
/*****************************************************************************/
// 	Description : 同步工具run的时候来电不接
//	Global resource dependence : s_call_context
//  Author:bin.ji
//	Note:2004.11.26
/*****************************************************************************/
LOCAL BOOLEAN CC_MTCallAtcRun( 
    uint8 call_id
    );
#endif

/*****************************************************************************/
// 	Description : 保存SetupIndParam
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void SaveSetupIndParam(
    DPARAM param
    );

/*****************************************************************************/
// 	Description : 载入SetupIndParam
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL DPARAM LoadSetupIndParam(void);

/*****************************************************************************/
// 	Description : 清除SetupIndParam
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void ClearSetupIndParam(void);

/*****************************************************************************/
// 	Description : 判断是否存在SetupIndParam
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
//LOCAL BOOLEAN IsExistSetupIndParam(void);

/*****************************************************************************/
// 	Description : handle MTFW Disconnected msg
//	Global resource dependence : 
//  Author:jibin
//	Note:2004.10.12
/*****************************************************************************/
LOCAL BOOLEAN HandleMTFWDisconnected(
    DPARAM param
    );

/*****************************************************************************/
// 	Description : process when received signal APP_MN_CALL_START_IND
//	Global resource dependence : s_call_context
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallStartInd(DPARAM param);

/*****************************************************************************/
// 	Description : process when received signal APP_MN_SYNC_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_SyncInd(DPARAM param);

/*****************************************************************************/
// 	Description : process when received signal APP_MN_ALERTING_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallAlertingInd(DPARAM param);

/*****************************************************************************/
//  Discription: receive amr type ind,if amr-wb,display hd icon.
//  Global resource dependence: None
//  Author: xiaoju.cheng
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL void CC_AMRTypeInd(DPARAM param);


/*****************************************************************************/
// 	Description : process when received signal APP_MN_PROGRESS_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallProgressInd(DPARAM param);

#ifdef BT_DIALER_SUPPORT

/*****************************************************************************/
// 	Description : the process message function of the BT CC application
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL MMI_RESULT_E MMICC_HandleBtPsMsg( PWND app_ptr, uint16 msg_id, DPARAM param);
/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: chunjie.liu
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_BTSendDtmfByChar(uint8 dtmf_ch);
/*****************************************************************************/
// 	Description : handle MT Call disconnect
//	Global resource dependence : 
//  Author:chunjie liu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSimMTCallDisc(DPARAM param);
/*****************************************************************************/
// 	Description : process when received signal ciev call == 0
//	Global resource dependence : 
//  Author:chunjie liu
//	Note:2013.06.08
/*****************************************************************************/
LOCAL void CC_BtSaveCLInfo(MMICL_CALL_INFO_T*  cl_info_ptr,MMICL_CALL_TYPE_E * calllog_type_ptr,uint8 call_index);
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CIEV_IND callsetup=2
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.06.09
/*****************************************************************************/
LOCAL void CC_BtCallStartInd(void);
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CIND_IND
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallCindInd(BT_MSG_T *msg_ptr);
/*****************************************************************************/
// 	Description : process when received signal APP_MN_CALL_START_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallCievInd(BT_MSG_T *msg_ptr);
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CLIP_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallClipInd(BT_MSG_T *msg_ptr);
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CCWA_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallCcwaInd(BT_MSG_T *msg_ptr);
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_SPK_VOL_IND
//	Global resource dependence : s_call_context
//  Author:kay.chen
//	Note:2014.9.3
/*****************************************************************************/
LOCAL void CC_BtCallSpkVolInd(BT_MSG_T *msg_ptr);
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CLCC_RECORD_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallClccRecordInd(BT_MSG_T *msg_ptr);
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CLCC_RESULT_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallClccResultInd(BT_MSG_T *msg_ptr);
/*****************************************************************************/
// 	Description : process when received signal setup=0
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:2012.06.06
/*****************************************************************************/
LOCAL void CC_SetupComplete(void);
/*****************************************************************************/
// 	Description : process when received signal ciev call == 0
//	Global resource dependence : 
//  Author:chunjie liu
//	Note:2013.06.08
/*****************************************************************************/
LOCAL void CC_BtDisconnectedCall(void);
/*****************************************************************************/
// 	Description : HFU call hold result
//	Global resource dependence : 
//  Author:wenny cheng
//	Note:2013.06.08
/*****************************************************************************/
LOCAL void CC_BtHfuHoldCfnInd(BT_MSG_T  *msg_ptr);

#if defined(MMI_MULTI_SIM_SYS_NONE) || defined(BT_DIALER_SUPPORT)//bug924697
/*****************************************************************************/
// 	Description : process when received signal ciev call == 0
//	Global resource dependence : 
//  Author:chunjie liu
//	Note:2013.06.08
/*****************************************************************************/
LOCAL void CC_BtDisconnectCallByIndex(uint8 call_index);
#endif

/*****************************************************************************/
//  Description : IsValidTelenum
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  IsValidTelenum(
                      char *tele_num,   // String的头指针
                      uint8 tele_len  // String的长度
                            );
/*****************************************************************************/
//  Description : tel number string valid check
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: valid tel number
//          FALSE:invalid tel number
/*****************************************************************************/
LOCAL BOOLEAN check_tel_string(char *pStr);

/*****************************************************************************/
//  Description : handle send sco request
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMICC_HandleScoTimer(uint8 timer_id, uint32 param);
#endif

/*****************************************************************************/
// 	Description : process when received signal EV_SETUP_COMPLETE_CNF
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_SetupCompleteCnf(DPARAM param);

/*****************************************************************************/
// 	Description : process when user hang up the call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseCallReq(MN_DUAL_SYS_E dual_sys, uint8 call_index,MN_CALL_DISCONNECT_CALL_CAUSE_E cause);

/*****************************************************************************/
// 	Description : process when received signal APP_MN_CALL_DISCONNECTED_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_DisconnectedCall(uint16 msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : disconnected call by index
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void CC_DisconnectedCallByIndex(
    uint8 call_index,
    MN_CALL_DISCONNECT_CALL_CAUSE_E cause
    );

/*****************************************************************************/
//  Description : 通话结束嘟嘟音，根据传入的param，自减1做嘟嘟音计数
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_PlayCallEndHint(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : VOLTE电话,有时网络携带的是sip类型字符串号码
//                sip:15982864307@sc.ims.mnc001.mcc460.3gppnetwork.org;user=phone
//  Author:Yonghua.zhang
//  Note:
/*****************************************************************************/
LOCAL void CC_ParseMTNumberFromSipUri(DPARAM param);

/*****************************************************************************/
// 	Description : process when received signal APP_MN_SETUP_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallSetupInd(DPARAM param);

/*****************************************************************************/
// 	Description : process when user want to receive a MT call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ConnectCallReq(void);

/*****************************************************************************/
// 	Description : process when received signal EV_SETUP_COMPLETE_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_SetupCompleteInd(DPARAM param);

/*****************************************************************************/
// 	Description : process the extension number when make a call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_ProcessExtensionNum(uint8 *tele_num, uint8 *tele_len);

/*****************************************************************************/
// 	Description : clear the stored DTMF after sending them
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_ClearStoredDtmf(void);

/*****************************************************************************/
// 	Description : clear the stored DTMF after sending them
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E CC_SendSingleDtmf( uint32 msg_id);

/*****************************************************************************/
// 	Description : get the index of call according to the call id
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
uint8 CC_GetIndexOfCallId(MN_DUAL_SYS_E dual_sys, uint8 call_id);

/*****************************************************************************/
// 	Description : get the index of call according to the call id
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_DeleteCallInfo(uint8 call_index);

#ifdef MMI_DUALMODE_ENABLE
/*****************************************************************************/
// 	Description : if GSM network support
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
//LOCAL BOOLEAN CC_IsGsmSupport(void);
LOCAL BOOLEAN CC_IsNetSupport(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
// 	Description : if GSM network support
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_IsEmergencyOnly(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
/*****************************************************************************/
// 	Description : local setting of emergency call
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_IsEmergencyInLocalEmergencyList(uint8 *tele_num, uint8 tele_len, BOOLEAN *is_local_exisit, MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
// 	Description : if the SIM is ok
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
//LOCAL BOOLEAN CC_IsSimOk(void);
LOCAL BOOLEAN CC_IsSimOk(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : open speaker and microphone
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_OpenAudioTunnel(BOOLEAN is_enable);

/*****************************************************************************/
// 	Description : process to release the active call and connect the waitting call 
//	Global resource dependence : 
//  Author:Louis wei
//	Note:2003.12.19
/*****************************************************************************/
LOCAL void CC_HandleHoldCnf( 
    DPARAM param 
    );

/*****************************************************************************/
// 	Description : process hold the call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//  Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_HoldCall(
                              void
    );

/*****************************************************************************/
// 	Description : process to signal of retrieve confirm
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleRetrieveCnf( DPARAM param );

/*****************************************************************************/
// 	Description : process the s_call_context in the MPTY
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleDisconnectedInMPTY(
    uint8  call_index ,//the index of call id which was released
    MN_CALL_DISCONNECT_CALL_CAUSE_E cause 
    );

/*****************************************************************************/
// 	Description : process to handle the confirm for build MPTY call 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleBuildMPTYCnf(
    DPARAM param
    );

/*****************************************************************************/
// 	Description : process to handle the confirm for split call 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleSplitCallCnf(
    DPARAM param
    );
#ifdef MMI_CRSS_SUPPORT
/*****************************************************************************/
// 	Description : get active call index
//	Global resource dependence : s_call_context
//  Author:
//	Note: get a active call or held call index
/*****************************************************************************/
LOCAL uint8 CC_GetActiveCallIndex(void);
#endif

/*****************************************************************************/
// 	Description : ECT Transfer Indication: active, Rdn or alering
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleECTInd(DPARAM param);

/*****************************************************************************/
// 	Description : ECT Transfer confirm
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleECTCnf(DPARAM param);
/*****************************************************************************/
// 	Description : ECT Transfer Indication: active, Rdn or alering
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void ReplaceCallNumberByCallId(APP_MN_ECT_INVOKE_IND_T *call_info_ptr, //IN:
                                     MMI_STRING_T *name_ptr, //IN:
                                     BOOLEAN is_exist_in_pb//IN:
                                     );

/*****************************************************************************/
// 	Description : to find a call in call module
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL uint8 FindCallInModule(
    void
    );

/*****************************************************************************/
// 	Description : Release all hold call 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseHoldCall(void);

/*****************************************************************************/
// 	Description : Release all active call 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseActiveCall(void);

#if 0
/*****************************************************************************/
// 	Description : Release all hold call expect the waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseAllCall(void);
#endif

/*****************************************************************************/
// 	Description : process to build MPTY call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_BuildMPTYCall(
    void
    );

/*****************************************************************************/
// 	Description : Hold the active call for preparing  for waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_HoldAndAnswerCW(void);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : process to disconnect the call which is  satisfied
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_MTCallFireWall ( 
    MN_DUAL_SYS_E		dual_sys,
    MN_CALLING_NUMBER_T *call_num_ptr, 
    uint8               call_id,
    MN_CALL_TYPE_E      call_type,
    uint8 line    
    );
#endif

/*****************************************************************************/
// 	Description : whether the call need to dial again
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_AutoMoCall ( 
    uint8                call_index,
    MN_CALL_DISCONNECT_CALL_CAUSE_E cause 
    );

/*****************************************************************************/
// 	Description : search the call which state is pointed by param
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL uint8   FindStateCall(
    CC_CALL_STATE_E  state 
    );

// 	Description : make call by adding ip number
//	Global resource dependence : 
//  Author:lin.lin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsHavingCountryCode(const uint8* number_ptr, uint8 *country_code_length);

/*****************************************************************************/
// 	Description : Get name from phonebook list
//	Global resource dependence : s_call_context
//  Author:lin.lin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNameFromPb(
    CC_WANT_MO_CALL_T *want_mo_call_ptr,//[IN]
    uint8*			tele_num,		//[IN]
    uint8			tele_len,		//[IN]
    MMI_STRING_T	*name_ptr,		//[OUT]
    uint16			*entry_id_ptr,	//[OUT]
    uint8			*group_id_ptr	//[OUT]
    );

/*****************************************************************************/
// 	Description : Get called info from the called number
//	Global resource dependence : 
//  Author:lin.lin
//	Note: add ip number parse
/*****************************************************************************/
LOCAL void GetCallInfoNumber(
    MMICL_CALL_INFO_T		*cl_call_info_ptr,
    MN_CALLED_NUMBER_T      *called_number_ptr
    );

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_CALL_FORWARD_IND
//	Global resource dependence : none
//  Author:Maoxy
//	Note:
/*****************************************************************************/
LOCAL void HandleCallForwardInd( 
    DPARAM param 
    );

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_FORWARD_STATUS_IND
//	Global resource dependence : none
//  Author:Maoxy
//	Note:
/*****************************************************************************/
LOCAL void HandleForwardStatusInd( 
    DPARAM param 
    );

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_ACMMAX_IND
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleAcmMaxInd( 
    DPARAM param 
    );

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_CALL_HELD_IND
//	Global resource dependence : none
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void HandleRemoteHoldCallInd( 
    DPARAM param 
    );

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_CALL_RETRIEVED_IND
//	Global resource dependence : none
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void HandleRemoteRetrievedCallInd( 
    DPARAM param 
    );

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_BUILD_MPTY_IND
//	Global resource dependence : none
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void HandleRemoteBuildMPTYInd( 
    DPARAM param 
    );
    
#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : get dialed ip num 
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_GetIPCallNum(MN_DUAL_SYS_E dual_sys, 
    uint8* tele_num, //[in]
    uint32 tele_len, //[in]
    uint8* phone_number, //[out]
    uint32  phone_num_len //[in]
    );
#endif

/*****************************************************************************/
//  Description : if the disc cause is "unobtainable destination-permanent/long term"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是号码错误,参见GSM 02.07
/*****************************************************************************/
LOCAL BOOLEAN CC_IsValideDest(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause);

/*****************************************************************************/
//  Description : if the disc cause is "busy destination" or "tempory"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是否时用户忙或者暂时无法接通,参见GSM 02.07
/*****************************************************************************/
LOCAL BOOLEAN CC_IsUserBusy(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause);

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E SendDtmfByChar(MN_DUAL_SYS_E dual_sys, uint8 dtmf_ch, BOOLEAN is_stop_immediately);

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E StopDtmfByChar(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Get Mo Call Sim Id Ex
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetMoCallSimIdEx(
                                                           MN_DUAL_SYS_E dual_sys,//current sim
                                                           uint8 *tele_num,//phone number
                                                           uint8 tele_len,//phone number wstr_len
                                                           CC_CALL_SIM_TYPE_E call_sim_type//call  sim type
                                                           );

/*****************************************************************************/
// 	Description : get mo call sim card id
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetMoCallSimId(
    MN_DUAL_SYS_E dual_sys,//current sim
    CC_CALL_SIM_TYPE_E call_sim_type//call  sim type
    );

/*****************************************************************************/
//  Description : 截掉了非首位的+号以及后面的字符
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
/*LOCAL uint8 MMICC_InterceptPhoneNum(//截取后的长度
                               uint8    *str_ptr,
                               uint8    str_len
                               );*/

/*****************************************************************************/
// 	Description : if tch not assigned and alerting is indicated, mo start playing
//                local ring to simulate the net ring
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void StartLocalNetRing(void);

/*****************************************************************************/
// 	Description : if tch not assigned and alerting is indicated, mo play
//                local ring to simulate the net ring
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void PlayLocalNetRing(uint8  timer_id, uint32 param);

/*****************************************************************************/
// 	Description : tch assigned or call disconnected, mo stop playing
//                local ring to simulate the net ring
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void StopLocalNetRing(void);

/*****************************************************************************/
// 	Description : 接收ps消息的时候，对于at cmd发起的电话，mmi不做任何处理，界面需要返回idle
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 返回true,代表是at cmd发起的call，则消息不做处理，反之，是mmi发起call.
/*****************************************************************************/
LOCAL BOOLEAN FilterMsgFromATCmdCall( 
                                      MMI_MESSAGE_ID_E    msg_id ,
                                      DPARAM  param
                                      );

/*****************************************************************************/
// 	Description : 重置cc的各项参数，当判定为at cmd发起的call时候进行调用
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void resetCallPara( void );


/*****************************************************************************/
// 	Description : 没有VT功能的时候，收到VT来电，需要挂断电话，并回CAUSE:88
//	Global resource dependence : 
//  Author: SAMBOO
//	Note: return True means vt incoming call is rejected
/*****************************************************************************/
LOCAL BOOLEAN HandleIncomingMsgWhenVTNotSupport( uint16 msg_id, DPARAM param );

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 当收到APP_MN_SYNC_IND,没有收到APP_MN_SETUP_COMPLETE_CNF和APP_MN_PROGRESS_IND时
//  设置为下行静音,收到APP_MN_SETUP_COMPLETE_CNF和APP_MN_PROGRESS_IND后,关闭静音
/*****************************************************************************/
LOCAL BOOLEAN  HandleMoMuteAndLocalRingState(uint16 msg_id);

#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : play incoming call name, play name and 来电
//	Global resource dependence : 
//  Author:barry peng
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IncomingCallName_To_Speech(void);
#endif

/*****************************************************************************/
// 	Description : 是否要正常对待当前的这通来电...
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAcceptCurCallAsNormal(void);


/*****************************************************************************/
// 	Description : 是否包含限制呼叫的号码
//	Global resource dependence : s_eng_call_barring_num
//  Author:fen.xie
//	Note:MS00186848
/*****************************************************************************/
LOCAL BOOLEAN IsIncludeBarringNumbers(	
                            const uint8* tel_num_str,		// input phone number string in ASCII
                            const uint8  tel_num_length  // string length
                            );


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_IsCallIDValid(uint8 call_id);              

#ifdef MMI_CRSS_SUPPORT
/*****************************************************************************/
// 	Description : is a ECT string(23091)
//	Global resource dependence : s_call_context
//    Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
LOCAL BOOLEAN IsECTString(uint8 *tele_num, uint8 tele_len);

/*****************************************************************************/
// 	Description : is a CRSS string
//	Global resource dependence : s_call_context
//  Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
LOCAL BOOLEAN IsStringOfCRSS(uint8 *tele_num, uint8 tele_len);

/*****************************************************************************/
// 	Description : the situation is appropriate for CRSS
//	Global resource dependence : s_call_context
//  Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
LOCAL BOOLEAN IsNeedCRSS(void);
#endif
/*****************************************************************************/
//  Description :judge whether the waiting call is being
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_IsWaitCallBeing(void);

/*****************************************************************************/
// 	Description : Release waiting call 
//	Global resource dependence : s_call_context
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseWaitCall(void);

#ifdef MMI_CRSS_SUPPORT
/*****************************************************************************/
// 	Description : Release the active call for preparing for waiting call or hold call
//	Global resource dependence : s_call_context
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_CRSSReleaseAndAnswer(void);

/*****************************************************************************/
// 	Description : Release the active call for preparing for waiting call or hold call
//	Global resource dependence : s_call_context
//    Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_CRSSHoldAndAnswer(void);
#endif
#if 0
/*****************************************************************************/
// 	Description : Release active call by index
//	Global resource dependence : s_call_context
//  Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
PUBLIC CC_RESULT_E CC_ReleaseActiveCallByIndex(uint8 call_index);
#endif

#ifdef MMI_CRSS_SUPPORT
/*****************************************************************************/
// 	Description : parse CRSS string in call
//	Global resource dependence : s_call_context
//  Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
LOCAL CC_RESULT_E MMICC_ParseStringOfCRSS(uint8 *tele_num, uint8 tele_len);
#endif

/*****************************************************************************/
// 	Description : clear s_call_context.want_mo_call
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void ClearWantCallInfo(void);

/*****************************************************************************/
// 	Description : EnableSpeakerMode 
//  Global resource dependence : 
//  Author:bin.ji
//	Note: TRUE success, FALSE fail
/*****************************************************************************/
LOCAL BOOLEAN EnableSpeakerMode(
    BOOLEAN is_turnon
    );

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
// 	Description : Parse CSC SOS Number
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseCSCSosNum(uint8 *sos_number_ptr, MMICC_CSC_SOS_NUM_T *cc_csc_sos_ptr);

/*****************************************************************************/
// 	Description : Is CSC SOS Number
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCSCSosNum(uint8 *tele_num_ptr, uint16 tele_len, MMICC_CSC_SOS_NUM_T *cc_csc_sos_ptr);
#endif

/*****************************************************************************/
// 	Description : Handle Net work Emg Ind(APP_MN_EMG_NUM_LIST_IND)
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleEmgNumListInd(DPARAM param);

#ifdef MMI_INDIAN_SOS_SUPPORT
LOCAL BOOLEAN CC_GetAvailableSimIndex(MN_DUAL_SYS_E *dual_sys){
	BOOLEAN ret = FALSE;
	*dual_sys = CC_GetSosCallingSimIndex();
	
	if( *dual_sys == MN_DUAL_SYS_1 && SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_2) ){
		*dual_sys = MN_DUAL_SYS_2;
	}
#if defined( MULTI_SIM_SYS_TRI )
	else if(*dual_sys == MN_DUAL_SYS_2 && SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_3) ){
		*dual_sys = MN_DUAL_SYS_3;
	}
#elif defined(MULTI_SIM_SYS_QUAD)
	else if(*dual_sys == MN_DUAL_SYS_2 && SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_3) ){
		*dual_sys = MN_DUAL_SYS_3;
	}else if( *dual_sys == MN_DUAL_SYS_3 && SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_4) ){
		*dual_sys = MN_DUAL_SYS_4;
	}
#endif
	if( *dual_sys != CC_GetSosCallingSimIndex() ){
		ret = TRUE;
	}

	return ret;
}

LOCAL BOOLEAN CC_IsSosCalling(){
	SCI_TRACE_LOW("CC_IsSosCalling enter, available=%d", s_sos_call_info.sos_available);
    return s_sos_call_info.sos_available;
}

LOCAL void CC_SetSosAvailable(BOOLEAN flag){
	SCI_TRACE_LOW("CC_SetSosAvailable enter, available=%d", flag);
    if(s_sos_call_info.sos_available == flag){
        return;
    }

    s_sos_call_info.sos_available = flag;
}

LOCAL void CC_SetSosCallId(uint8 call_id){
	SCI_TRACE_LOW("CC_SetSosCallId enter, call id=%d", call_id);
    s_sos_call_info.sos_call_id = call_id;
}

LOCAL uint8 CC_GetSosCallId(){
	SCI_TRACE_LOW("CC_GetSosCallId enter, call id=%d", s_sos_call_info.sos_call_id);
    return s_sos_call_info.sos_call_id;
}

LOCAL void CC_SetSosCallNumIndex(uint8 index){
	SCI_TRACE_LOW("CC_SetSosCallNumIndex enter, index=%d", index);
    s_sos_call_info.sos_num_index = index;
}

LOCAL uint8 CC_GetSosCallNumIndex(){
	SCI_TRACE_LOW("CC_GetSosCallNumIndex enter,num_ind=%d", s_sos_call_info.sos_num_index);
    return s_sos_call_info.sos_num_index;
}

LOCAL MN_DUAL_SYS_E CC_GetSosCallingSimIndex(){
	SCI_TRACE_LOW("CC_GetSosCallingSimIndex enter,sim_index=%d", s_sos_call_info.sos_dual_sys);
	return s_sos_call_info.sos_dual_sys;
}


LOCAL void CC_SetSosCallingSimIndex(MN_DUAL_SYS_E dual_sys){
		SCI_TRACE_LOW("CC_SetSosCallingSimIndex enter,dual_sys=%d", dual_sys);
		s_sos_call_info.sos_dual_sys = dual_sys;
}

LOCAL void CC_TerminateSosCall(BOOLEAN flag){    //flag : flag to distinguish whether flow stopped normally.
	SCI_TRACE_LOW("CC_TerminateSosCall enter,flag=%d", flag);
	s_sos_call_info.num_count = 0;
	CC_SetSosCallNumIndex(0);
	CC_SetSosCallingSimIndex(MN_DUAL_SYS_1);
	SCI_MEMSET(s_sos_call_info.telnum, 0x00, CC_MAX_TELE_NUM_LEN*MMISET_SOS_CONTACTS_MAX_NUM);
	CC_SetSosAvailable(FALSE);
	if(s_sos_call_info.callback != PNULL){
		s_sos_call_info.callback(flag);
	}
}


LOCAL CC_RESULT_E CC_ProcessSosCall(MN_DUAL_SYS_E dual_sys){
    CC_MAKE_CALL_INFO sos_call_info = {0};
	uint8 num_ind = CC_GetSosCallNumIndex();

	SCI_TRACE_LOW("CC_ProcessSosCall enter, dual_sys=%d", dual_sys);
    
    sos_call_info.dual_sys = dual_sys;
    MMI_MEMCPY(sos_call_info.tele_num, 
        CC_MAX_TELE_NUM_LEN,
        s_sos_call_info.telnum[num_ind],
        strlen(s_sos_call_info.telnum[num_ind]),
        MIN(CC_MAX_TELE_NUM_LEN,  strlen(s_sos_call_info.telnum[num_ind])));
    sos_call_info.tele_len = strlen(s_sos_call_info.telnum[num_ind]);
	sos_call_info.call_sim_type = CC_CALL_SIM_MAX;   //operating sim

	SCI_TRACE_LOW("CC_ProcessSosCall, sos_num=%s", sos_call_info.tele_num);	
    if(MMICC_ProcessMakeCall(dual_sys, &sos_call_info)){       //0 : calling failed
		CC_TerminateSosCall(FALSE);
        return CC_RESULT_ERROR;
    }
	
    return CC_RESULT_SUCCESS;
}

PUBLIC CC_RESULT_E MMICC_MakeSosCall(void* param, MMISET_SOS_CALLBACK_PFUNC callback){
	MMISET_SOS_CONTACT_ITEM_T *sos_num_list_tag = (MMISET_SOS_CONTACT_ITEM_T*)param;
	MN_DUAL_SYS_E call_dual_sys = MN_DUAL_SYS_MAX;
	uint32 sim_num = MMIAPIPHONE_GetSimExistedNum(PNULL, 0);
	uint8 i;

	SCI_TRACE_LOW("MMICC_MakeSosCall enter");
	
	
	for(i = 0; i < MMISET_SOS_CONTACTS_MAX_NUM; i++){
		if(sos_num_list_tag[i].num_len > 0){
			if(sim_num <= 0){
				char sos_num[CC_MAX_TELE_NUM_LEN] = {0};
				MMIAPICOM_WstrToStr(sos_num_list_tag[i].num, sos_num);
				if(!MMIAPICC_IsEmergencyNum(sos_num, sos_num_list_tag[i].num_len,
					MN_DUAL_SYS_1, FALSE)){
					continue;
				}
			}
			MMIAPICOM_WstrToStr(sos_num_list_tag[i].num, s_sos_call_info.telnum[s_sos_call_info.num_count]);
			s_sos_call_info.num_count++;
			SCI_TRACE_LOW("MMICC_MakeSosCall, sos_num=%s, num_len=%d", s_sos_call_info.telnum[i], sos_num_list_tag[i].num_len);
		}
		//
	}

	if( sim_num <= 0 && s_sos_call_info.num_count == 0){
		MMIPUB_OpenAlertWarningWin(TXT_CC_EMERGENCYONLY);
		CC_TerminateSosCall(FALSE);
		
		return CC_RESULT_ERROR;
	}

	s_sos_call_info.callback = callback;
	CC_SetSosAvailable(TRUE);
	 
	if(sim_num >= 2){		//unless exists only one sim card, apply MN_DUAL_SYS_1 by default.
	 	call_dual_sys = MN_DUAL_SYS_1;
	}else{
	 	//automatically to select a availd sim card with single sim case.
		call_dual_sys = GetMoCallSimIdEx(MN_DUAL_SYS_MAX, s_sos_call_info.telnum[0], strlen(s_sos_call_info.telnum[0]), CC_CALL_SIM_MAX);
	}   

	return CC_ProcessSosCall(call_dual_sys);
}
#endif

/*****************************************************************************/
// 	Description : process when received signal MN_SETUP_COMPLETE_CNF
//	Global resource dependence : 
//    Author:yonghua.zhang
//	Note:2020.5.1
/*****************************************************************************/
LOCAL BOOLEAN CC_IsInvalidSetupCompleteCnf(DPARAM param);
#ifdef MMI_VOLTE_SUPPORT
/*****************************************************************************/
// 	Description : process when received signal APP_MN_CALL_HO_PROGRESS_IND in VOLTE
//	Global resource dependence : 
//   Author:yonghua.zhang
/*****************************************************************************/
LOCAL void CC_SetCallPathStatus(MN_DUAL_SYS_E dual_sys, BOOLEAN isPScall);
/*****************************************************************************/
// 	Description : process when received signal APP_MN_SETUP_IND in VOLTE
//	Global resource dependence : 
//   Author:yonghua.zhang
/*****************************************************************************/
LOCAL void CC_CallSetupIndForVoLTE(DPARAM param);
/*****************************************************************************/
//  Description : 打开/关闭volte codec
//  Global resource dependence : 
//  Author: Yonghua.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_EnableVoLTECodec ( BOOLEAN is_enable );
/*****************************************************************************/
//  Description : 处理回落的来电，设置接听回落的来电
//  Global resource dependence : 
//  Author: Yonghua.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_MtCSFBInd(DPARAM param)
{
    APP_MN_MT_CSFB_IND_T *pSig = (APP_MN_MT_CSFB_IND_T*)param;

    if(PNULL== pSig)
    {
        SCI_TRACE_LOW("volte ccapp.c: CC_MtCSFBInd() pSig == PNULL");
        return;
    }

    SCI_TRACE_LOW("volte ccapp.c:enter function CC_MtCSFBInd(), dual_sys = %d, call_id = %d ",pSig->dual_sys, pSig->call_id);
#ifndef WIN32	
    MNCALL_MtCSFBRspEx(pSig->dual_sys, pSig->call_id, TRUE);
#endif
}
/*****************************************************************************/
//  Description : PS call 网络铃音消息，收到该消息后，enable volte codec
//  Global resource dependence : 
//  Author: Yonghua.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_CallEarlyMediaInd(DPARAM param)
{
    APP_MN_EARLY_MEDIA_IND_T *pSig = (APP_MN_EARLY_MEDIA_IND_T*)param;

    if(PNULL== pSig)
    {
        SCI_TRACE_LOW("volte ccapp.c: CC_CallEarlyMediaInd() pSig == PNULL");
        return;
    }
    SCI_TRACE_LOW("volte ccapp.c: CC_CallEarlyMediaInd(), dual_sys = %d, call_id=%d,  is_announce_ply = %d,  s_call_context.pscall_info.is_ps_call =%d",pSig->dual_sys, pSig->call_id, pSig->is_announce_ply, s_call_context.pscall_info.is_ps_call);

    if(pSig->is_announce_ply && s_call_context.pscall_info.is_ps_call)
    {
        StopLocalNetRing();
        SCI_TRACE_LOW("volte ccapp.c: CC_CallEarlyMediaInd  is_volte_codec_on: %d ",s_call_context.pscall_info.is_volte_codec_on);
        if(!s_call_context.pscall_info.is_volte_codec_on)
        {
            CC_EnableVoLTECodec(TRUE);
        }
    }
}
/*****************************************************************************/
//  Description : SRVCC 处理呼叫过程中4G/2G切换
//  Global resource dependence : 
//  Author: Yonghua.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_CallHOInd(DPARAM param)
{
    APP_MN_CALL_HO_PROGRESS_IND_T *pSig = (APP_MN_CALL_HO_PROGRESS_IND_T*)param;

    if(PNULL== pSig)
    {
        SCI_TRACE_LOW("volte ccapp.c: CC_CallHOInd() pSig == PNULL");
        return;
    }

    SCI_TRACE_LOW("volte ccapp.c:enter function CC_CallHOInd(), dual_sys = %d, ho_progress_type = %d ,callnum =%d ",pSig->dual_sys, pSig->ho_progress_type, pSig->callnum);
    if( s_call_context.dual_sys == pSig->dual_sys)
    {
        // 4G->2G
        if(pSig->ho_progress_type == MN_CALL_PSTOCS_SRVCC_PROGRESS_SUCCESS 
			||pSig->ho_progress_type == MN_CALL_PSTOCS_VSRVCC_PROGRESS_SUCCESS)// AT 中未处理该消息
        {
             CC_EnableVoLTECodec(FALSE);
             // TODO: 确认是否需要reset voice codec
             CC_SetCallPathStatus(pSig->dual_sys, FALSE);
        }
        else if(pSig->ho_progress_type == MN_CALL_CSTOPS_SRVCC_PROGRESS_SUCCESS)// 2G->4G
        {
             CC_EnableVoLTECodec(TRUE);
             CC_SetCallPathStatus(pSig->dual_sys, TRUE);
        }
    }
    //update status bar HD icon
}
/*****************************************************************************/
// 	Description : process when received signal APP_MN_SETUP_IND mt/4G
//	Global resource dependence : 
//  Author:figo.feng
//	Note:modify by louis.wei 
/*****************************************************************************/
LOCAL void CC_CallSetupIndForVoLTE(DPARAM param)
{
    APP_MN_SETUP_IND_T        *pSig = (APP_MN_SETUP_IND_T*)param;
    if(PNULL== pSig)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_CallSetupInd() pSig == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3217_112_2_18_2_10_13_82,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_CallSetupInd(), s_call_context.dual_sys = %d, s_call_context.call_number = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3214_112_2_18_2_10_13_81,(uint8*)"dd",pSig->dual_sys, s_call_context.call_number);
    SCI_TRACE_LOW("CC_CallSetupIndForVoLTE  call_form =%d",pSig->call_form);
    if(pSig->call_form == MN_CALL_FORM_IP_NORMAL_CALL || pSig->call_form == MN_CALL_FORM_IP_CONF_CALL)
    {
        CC_CallSetupInd(param);
        s_call_context.pscall_info.is_ps_call = TRUE;
        ClearSetupIndParam();
        CC_SetCallPathStatus(s_setupind_param.dual_sys, TRUE);	
    }
}

/*****************************************************************************/
//  Description : 清空4G audio 相关的参数信息
//  Global resource dependence : 
//  Author: Yonghua.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_ClearPscallInfo(void)
{
    SCI_TRACE_LOW("volte ccapp.c: CC_ClearPscallInfo");
    SCI_MEMSET(&s_call_context.pscall_info, 0x00, sizeof(CC_PSCALL_INFO_T));
}

/*****************************************************************************/
//  Description : 打开/关闭volte codec
//  Global resource dependence : 
//  Author: Yonghua.zhang
//  Note: 
/*****************************************************************************/
LOCAL void CC_EnableVoLTECodec ( BOOLEAN is_enable )
{
    SCI_TRACE_LOW("volte ccapp.c: MMICC_EnableVoLTECodec  is_enable: %d ",is_enable);
    if(is_enable)
    {
        if(s_call_context.pscall_info.is_ps_call && (!s_call_context.pscall_info.is_volte_codec_on))
        {
            SCI_TRACE_LOW("volte ccapp: MMICC_EnableVoLTECodec true");
            s_call_context.pscall_info.is_volte_codec_on = TRUE;
            MNPHONE_StartVolteCodec(s_call_context.dual_sys);
        }
    }
    else
    {
        if(s_call_context.pscall_info.is_ps_call && s_call_context.pscall_info.is_volte_codec_on)
        {
            SCI_TRACE_LOW("volte ccapp: MMICC_EnableVoLTECodec false");
            MNPHONE_StopVolteCodec(s_call_context.dual_sys);
            s_call_context.pscall_info.is_volte_codec_on = FALSE;
        }
    }
}
/*****************************************************************************/
// 	Description : process when received signal APP_MN_CALL_HO_PROGRESS_IND in VOLTE
//	Global resource dependence : 
//   Author:yonghua.zhang
/*****************************************************************************/
LOCAL void CC_SetCallPathStatus(MN_DUAL_SYS_E dual_sys, BOOLEAN isPScall)
{
    SCI_TRACE_LOW("volte ccapp.c: CC_SetCallPathStatus %d, dual_sys =%d, current calling sim =%d", isPScall, dual_sys, s_call_context.dual_sys);
    if(dual_sys == s_call_context.dual_sys)
    {
        s_call_context.pscall_info.is_ps_call  = isPScall;
    }
    //update call UI
    MMICC_UpdateCallStatusDisplay(MMICC_VOLTE_CALL_IND);
}
/*****************************************************************************/
// 	Description : check is current call path is 4G
//	Global resource dependence : 
//   Author:yonghua.zhang
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsVolteNetworkStatus(void)
{
    SCI_TraceLow("volte ccapp.c CC_IsVolteNetworkStatus call_path_status[%d] =%d",s_call_context.dual_sys,s_call_context.pscall_info.is_ps_call );
    return s_call_context.pscall_info.is_ps_call;
}

#endif
/*****************************************************************************/
// 	Description : reset disconnect cause in 4G, change MN_CAUSE_NORMAL_CLEARING to MN_CAUSE_IMS_REASON_RELEASE_X
//	Global resource dependence : 
//   Author:yonghua.zhang
/*****************************************************************************/
LOCAL MN_CALL_DISCONNECT_CALL_CAUSE_E CC_ResetDisconnectCause(MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    MN_CALL_DISCONNECT_CALL_CAUSE_E disconnect_cause = MN_CAUSE_NORMAL_CLEARING;

    SCI_TraceLow("volte ccapp.c CC_ResetDisconnectCause input cause = %d",cause);
    disconnect_cause = cause;	
#ifdef MMI_VOLTE_SUPPORT	
    if(TRUE == MMICC_IsVolteNetworkStatus() && (cause == MN_CAUSE_NORMAL_CLEARING))
    {
            disconnect_cause =  MN_CAUSE_IMS_REASON_RELEASE_X;
    }
#endif
    SCI_TraceLow("volte ccapp.c CC_ResetDisconnectCause return cause = %d",disconnect_cause);
    return disconnect_cause;
}

//该宏定义用于实现W317项目需求:存在通话时，对于新的MT做挂断处理
#define WATCH_REJECT_MT_INCALL 1
#ifdef WATCH_REJECT_MT_INCALL
LOCAL APP_MN_SETUP_IND_T  s_setupind_reject_param = {0};   //存储需要挂断的setup_ind消息
LOCAL BOOLEAN  s_is_exist_reject_mt = FALSE;

LOCAL MMI_RESULT_E MMICC_HandlePsMsg_RejectMt( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    SCI_TRACE_LOW("ccapp.c:enter function CC_HandlePsMsg_RejectMt(), msg_id = %d", msg_id);
    switch(msg_id)
    {
#ifdef MMI_VOLTE_SUPPORT
        case APP_MN_MT_CSFB_IND:
        {
            SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_MT_CSFB_IND. s_call_context.call_number = %d.", s_call_context.call_number);
            if(!s_is_exist_reject_mt
                && CC_NO_NUM != s_call_context.call_number) //判断之前有没有电话
            {
                //当前已经存在一路电话,又接收到VOLTE MT. Do Nothing, 等到APP_MN_SETUP_IND上报的时候再去拒绝此路MT. yuming.yang
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_MT_CSFB_IND_T, break");
                break;
            }
            else
            {
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_MT_CSFB_IND_T, pass it");
                result = MMI_RESULT_FALSE;
            }
            break;
        }
#endif
        case APP_MN_SETUP_IND:
        {
            APP_MN_SETUP_IND_T  *pSig = (APP_MN_SETUP_IND_T*)param;
            SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_SETUP_IND. s_call_context.call_number = %d.", s_call_context.call_number);
            if(!s_is_exist_reject_mt
                && CC_NO_NUM != s_call_context.call_number) //判断之前有没有电话
            {
                s_is_exist_reject_mt = TRUE;
                s_setupind_reject_param = *(APP_MN_SETUP_IND_T*)param;

                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_SETUP_IND reject one mt. call id = %d", s_setupind_reject_param.call_id);
                //挂断该通电话
                if(MMICC_IsVolteNetworkStatus()) //bug 1331084
                {
                    MNCALL_DisconnectCallEx(pSig->dual_sys, pSig->call_id, MN_CAUSE_IMS_REASON_RELEASE_X, PNULL);
                }
                else
                {
                    MNCALL_DisconnectCallEx(pSig->dual_sys, pSig->call_id, MN_CAUSE_USER_BUSY, PNULL);
                }
            }
            else
            {
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_SETUP_IND, pass it");
                result = MMI_RESULT_FALSE;
            }
            break;
        }
        case APP_MN_ALERTING_IND:
        {
            APP_MN_ALERTING_IND_T *pSig = (APP_MN_ALERTING_IND_T*)param;
            //SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_ALERTING_IND");
            SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_ALERTING_IND, call id = %d", pSig->call_id);

            if(s_is_exist_reject_mt && (pSig->call_id == s_setupind_reject_param.call_id))
            {
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_ALERTING_IND, reject it");
                //do not handle this msg
                break;
            }
            else
            {
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_ALERTING_IND, pass it");
                result = MMI_RESULT_FALSE;
            }
            break;
        }
        case APP_MN_CALL_ERR_IND:
        {
            APP_MN_CALL_ERR_IND_T *pSig = (APP_MN_CALL_ERR_IND_T*)param;
            //SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_CALL_ERR_IND");
            SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_CALL_ERR_IND, call id = %d", pSig->call_id);
            if(s_is_exist_reject_mt && (pSig->call_id == s_setupind_reject_param.call_id))
            {
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_CALL_ERR_IND, reject it");
                //do not handle this msg
                break;
            }
            else
            {
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_CALL_ERR_IND, pass it");
                result = MMI_RESULT_FALSE;
            }
            break;
        }
        case APP_MN_CALL_DISCONNECTED_IND:
        {
            APP_MN_CALL_DISCONNECTED_IND_T *pSig = (APP_MN_CALL_DISCONNECTED_IND_T*)param;
            //SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_CALL_DISCONNECTED_IND");
            SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt,receive APP_MN_CALL_DISCONNECTED_IND, call id = %d", pSig->call_id);
            if(s_is_exist_reject_mt && (pSig->call_id == s_setupind_reject_param.call_id))
            {
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_CALL_DISCONNECTED_IND, reject it");
                SCI_MEMSET(&s_setupind_reject_param, 0x00, sizeof(s_setupind_reject_param));
                s_setupind_reject_param.call_id = CC_INVALID_ID;
                s_is_exist_reject_mt = FALSE;
                break;
            }
            else
            {
                SCI_TRACE_LOW("MMICC_HandlePsMsg_RejectMt ,receive APP_MN_CALL_DISCONNECTED_IND, pass it");
                result = MMI_RESULT_FALSE;
            }
            break;
        }
        default:
        {
            result = MMI_RESULT_FALSE;
            break;
        }
    }
    return result;
}
#endif

// 	Description : the process message function of the CC application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICC_HandlePsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    MN_CALL_DISCONNECT_CALL_CAUSE_E dis_cause = MN_CAUSE_UNASSIGNED_NO;
    APP_MN_CALL_READY_IND_T call_ready_param_ptr = {0};
#ifdef BT_DIALER_SUPPORT   
    if(msg_id >= APP_BT_HFU_DIAL_NUMBER_RESULT_IND && msg_id <= APP_BT_HFU_CCWA_IND)
    {
        return MMICC_HandleBtPsMsg(app_ptr,msg_id,param);
    }
#endif 
#ifdef VT_SUPPORT
    if ((MMIAPIVT_IsVTIncomingMsg(msg_id, param)||MMIAPIVT_IsVtCalling()))
    {
        return MMI_RESULT_FALSE;
    }
#else//不支持VT的时候，来VT电话直接挂断
    if (HandleIncomingMsgWhenVTNotSupport(msg_id, param))
    {
        return MMI_RESULT_TRUE;
    }
#endif
    
    //check the param
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandlePsMsg(), msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_966_112_2_18_2_10_8_0,(uint8*)"d", msg_id);
    
    if(PNULL== app_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_HandlePsMsg() app_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_970_112_2_18_2_10_8_1,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    if (FilterMsgFromATCmdCall(msg_id, param))
    {
        return result;
    }

#ifdef WATCH_REJECT_MT_INCALL
    do
    {
        MMI_RESULT_E mRejectMtRes = MMICC_HandlePsMsg_RejectMt(app_ptr, msg_id, param);

        if (mRejectMtRes)
        {
            return mRejectMtRes;
        }
    }while(0);
#endif

    switch( msg_id )
    {
#ifdef MMI_VOLTE_SUPPORT
    case APP_MN_MT_CSFB_IND:
        SCI_TRACE_LOW("volte MMICC_HandlePsMsg,receive APP_MN_MT_CSFB_IND");
        CC_MtCSFBInd(param);
        break;
    case APP_MN_CALL_EARLY_MEDIA_IND:
        SCI_TRACE_LOW("volte MMICC_HandlePsMsg,receive APP_MN_CALL_EARLY_MEDIA_IND");
        CC_CallEarlyMediaInd(param);
        break;
    case APP_MN_CALL_HO_PROGRESS_IND://处理回落
        SCI_TRACE_LOW("volte MMICC_HandlePsMsg,receive APP_MN_CALL_HO_PROGRESS_IND");
        CC_CallHOInd(param); 
        break;
#endif
    case APP_MN_AMR_TYPE_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_AMR_TYPE_IND");
        CC_AMRTypeInd(param);
        break;
    case APP_MN_CALL_START_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_START_IND");
        //收到分配CALL ID事件的处理
        CC_CallStartInd(param);
        //clear s_allow_dtmf_flag
        s_call_context.dtmf_state.allow_send_flag = 0;
        break;
        
    case APP_MN_ALERTING_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_ALERTING_IND");
        //收到振铃提示事件的处理
        CC_CallAlertingInd(param);
        break;
        
    case APP_MN_PROGRESS_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_PROGRESS_IND");

        CC_CallProgressInd(param);
        break;
        
    case APP_MN_SETUP_COMPLETE_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_SETUP_COMPLETE_IND");

        CC_SetupCompleteInd(param);
//============== MET MEX start ===================
#ifdef MET_MEX_SUPPORT
        MMIMEX_PhsOutCallConnectedCB((void*)((uint32)param));
#endif    
//============== MET MEX end ===================    
        break;
        
    case APP_MN_SYNC_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_SYNC_IND");
        //收到信道分配事件的处理
        CC_SyncInd(param);
//============== MET MEX start ===================
#ifdef MET_MEX_SUPPORT 
        MMIMEX_PhsOutCallEventCB((void*)((uint32)param));
#endif    
//============== MET MEX end ===================     
        break;
        
    case APP_MN_SETUP_COMPLETE_CNF:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_SETUP_COMPLETE_CNF");

        //收到建立连接完成事件（MO）的处理
       //Bug 1309261 - [Queen2]4G网络下MO建立通话后，再有来电会收到重复的setup_complete_cnf
        if(CC_IsInvalidSetupCompleteCnf(param))
            break;
        CC_SetupCompleteCnf(param);
        MMIAPISTK_SetupCall(((APP_MN_SETUP_COMPLETE_CNF_T*)param)->dual_sys, TRUE, TRUE, 0);
#ifdef MMI_INDIAN_SOS_SUPPORT
	if(CC_IsSosCalling() && (((APP_MN_SETUP_COMPLETE_CNF_T*)param)->call_id == CC_GetSosCallId())){
		CC_TerminateSosCall(TRUE);
	}
#endif
        break;
        
    case APP_MN_CALL_DISCONNECTED_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_DISCONNECTED_IND");
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandlePsMsg(), APP_MN_CALL_DISCONNECTED_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1022_112_2_18_2_10_8_2,(uint8*)"");
        if (0 == s_disc_info_cause && MMIAPISET_GetIsAutoRedail())
        {
            //write call disconnect cause to nv
            MMICC_SetRepeatInfoCause(((APP_MN_CALL_DISCONNECTED_IND_T*)param)->disconnected_cause); 
        }

        if (PNULL != param)
        {
            if ((((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id == ((APP_MN_SETUP_IND_T*)LoadSetupIndParam())->call_id)
                &&(((APP_MN_CALL_DISCONNECTED_IND_T*)param)->dual_sys == ((APP_MN_SETUP_IND_T*)LoadSetupIndParam())->dual_sys)
                )
            {
                ClearSetupIndParam();
            }
            else if (!HandleMTFWDisconnected(param))        //收到CALL中断事件的处理
            {
                //不是MTFW disconnected
                CC_DisconnectedCall(msg_id,param); 
            }

        }
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HD_CALL, FALSE);
        GUIWIN_UpdateStbIcon();  
#ifdef MMI_INDIAN_SOS_SUPPORT
		if(CC_IsSosCalling() && ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id == CC_GetSosCallId()){
			MN_DUAL_SYS_E sos_dual_sys = CC_GetSosCallingSimIndex();
			uint8 num_ind = CC_GetSosCallNumIndex();
			
			SCI_TRACE_LOW("SOS Calling, call id=%d", ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id);
			if(CC_GetAvailableSimIndex(&sos_dual_sys)){
				CC_SetSosCallingSimIndex(sos_dual_sys);
			}else{
				
				if(num_ind == s_sos_call_info.num_count - 1){
					CC_TerminateSosCall(FALSE);   //only loop 1 times.
					break;
			
				}else{
					if( MMIAPIPHONE_GetSimExistedNum(PNULL, 0) >= 2 ){
						sos_dual_sys = MN_DUAL_SYS_1;	
					}else{
						sos_dual_sys = GetMoCallSimIdEx(MN_DUAL_SYS_MAX, 
						s_sos_call_info.telnum[num_ind], strlen(s_sos_call_info.telnum[num_ind]), CC_CALL_SIM_MAX);
					}
					
					CC_SetSosCallingSimIndex(sos_dual_sys);
					num_ind++;
				}
				CC_SetSosCallNumIndex(num_ind);
			}
			CC_ProcessSosCall(sos_dual_sys);
		}
#endif
        break;

        //add for auto redial
    case APP_MN_DISC_INFO_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_DISC_INFO_IND");

        s_disc_info_cause = ((APP_MN_DISC_INFO_IND_T*)param)->cause;
        if (MMIAPISET_GetIsAutoRedail())
        {
            //write call disconnect cause to nv
            MMICC_SetRepeatInfoCause(((APP_MN_DISC_INFO_IND_T*)param)->cause);
        }
        //disable codec to avoid unnessary voice from network (sometimes network close chanel early)
        if (s_call_context.call_number <= 1)
        {
            MMICC_EnableMute(FALSE);
            MMICC_EnableVoiceCodec(FALSE);
        }
        break;
        
    case APP_MN_CALL_ERR_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_ERR_IND");

        //收到网络错误事件的处理
        //记录挂断原因
        MMICC_SetRepeatInfoCause(((APP_MN_CALL_ERR_IND_T*)param)->error_cause);

        if ((((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id == ((APP_MN_SETUP_IND_T*)LoadSetupIndParam())->call_id)
            &&(((APP_MN_CALL_DISCONNECTED_IND_T*)param)->dual_sys == ((APP_MN_SETUP_IND_T*)LoadSetupIndParam())->dual_sys)
            )
        {
            ClearSetupIndParam();
        }
        else
        {
            dis_cause = ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->disconnected_cause;
            //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandlePsMsg(), APP_MN_CALL_ERR_IND, dis_cause = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1075_112_2_18_2_10_8_3,(uint8*)"d", dis_cause);
            CC_DisconnectedCall(msg_id, param);
	#ifdef MMI_INDIAN_SOS_SUPPORT
			if(CC_IsSosCalling() && ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id == CC_GetSosCallId()){
				MN_DUAL_SYS_E sos_dual_sys = CC_GetSosCallingSimIndex();
				uint8 num_ind = CC_GetSosCallNumIndex();
			
				SCI_TRACE_LOW("SOS Calling, call id=%d", ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id);
				if(CC_GetAvailableSimIndex(&sos_dual_sys)){
					CC_SetSosCallingSimIndex(sos_dual_sys);
				}else{
				
					if(num_ind == s_sos_call_info.num_count - 1){
						CC_TerminateSosCall(FALSE);   //only loop 1 times.
						break;
			
					}else{
						if( MMIAPIPHONE_GetSimExistedNum(PNULL, 0) >= 2 ){
							sos_dual_sys = MN_DUAL_SYS_1;	
						}else{
							sos_dual_sys = GetMoCallSimIdEx(MN_DUAL_SYS_MAX, 
							s_sos_call_info.telnum[num_ind], strlen(s_sos_call_info.telnum[num_ind]), CC_CALL_SIM_MAX);
						}
						
						CC_SetSosCallingSimIndex(sos_dual_sys);
						num_ind++;
					}
					CC_SetSosCallNumIndex(num_ind);
				}
				CC_ProcessSosCall(sos_dual_sys);
			}
	#endif
        }
        break;        
        
    case APP_MN_SETUP_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_SETUP_IND");

        //SCI_TRACE_LOW:"ccapp: CC_HandlePsMsg->APP_MN_SETUP_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1081_112_2_18_2_10_8_4,(uint8*)"");
#ifdef AVOID_MTC_DC_DISTURB_SUPPORT        
        MMIPUB_OpenAlertWarningWin(TXT_CC_SYSTEM_RUNNING); //专门处理来电dc绿条纹问题
#endif

#ifdef MMI_VOLTE_SUPPORT
        CC_ParseMTNumberFromSipUri(param);
#endif

        if (!HandleMTCallDisc(param))
        {
#ifdef BT_DIALER_SUPPORT
            if(MMICC_GetBtCallFlag())
            {
		        // Don't disconnect 3rd MT call here
                SCI_TRACE_LOW("Don't disconnect 3rd MT call here");
				// HandleSimMTCallDisc(param);
            }
            else
#endif     
            //如果另外一张卡刚刚发出mo，需要挂断mo，保证mt的处理
            if (CC_IsCallIDValid(s_call_context.current_call) 
                && (CC_CALLING_STATE == MMICC_GetCurrentCallStatus()) )
            {
                //SCI_TRACE_LOW:"ccapp disc sim %d mo call"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1088_112_2_18_2_10_8_5,(uint8*)"d", s_call_context.call_info[s_call_context.current_call].dual_sys);
                CC_ReleaseCallReq(s_call_context.call_info[s_call_context.current_call].dual_sys, 
                    s_call_context.current_call,
                    MN_CAUSE_NORMAL_CLEARING);
            }               
            //保存参数，收到APP_MN_ALERTING_IND消息时处理
            SaveSetupIndParam(param);
            
            //MT call come in, so exit TV-out
            MMIAPITVOUT_Close();
#ifdef MMI_VOLTE_SUPPORT
            CC_CallSetupIndForVoLTE(param);
#endif
        }
        
        break;
        
    case APP_MN_CALL_HELD_CNF:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_HELD_CNF");

        //receive the cofirm about hold call
        CC_HandleHoldCnf(param);
        break;
        
    case APP_MN_CALL_RETRIEVED_CNF:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_RETRIEVED_CNF");

        CC_HandleRetrieveCnf(param);
        break;
        
    case APP_MN_BUILD_MPTY_CNF:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_BUILD_MPTY_CNF");
        CC_HandleBuildMPTYCnf(param);
        break;
        
    case APP_MN_SPLIT_MPTY_CNF:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_SPLIT_MPTY_CNF");
        CC_HandleSplitCallCnf(param);
        break;
        
    case APP_MN_CALL_READY_IND://该消息只是表示跟PB是否OK有关，跟网络操作无关
        // CC_FDNInit();
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_READY_IND");
        call_ready_param_ptr = *(APP_MN_CALL_READY_IND_T *)param;
        
        if(call_ready_param_ptr.dual_sys < MMI_DUAL_SYS_MAX)
        {
            s_call_ready[call_ready_param_ptr.dual_sys] = TRUE;
            MMIAPISET_GetCallLineInfo(call_ready_param_ptr.dual_sys);
            //SCI_TRACE_LOW:"ccapp.c CC_HandlePsMsg, dual_sys = %d, s_call_ready = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1127_112_2_18_2_10_9_6,(uint8*)"dd", call_ready_param_ptr.dual_sys, s_call_ready[call_ready_param_ptr.dual_sys]);
        }
        else
        {
            //SCI_TRACE_LOW:"ccapp error call_ready_param_ptr.dual_sys = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1131_112_2_18_2_10_9_7,(uint8*)"d", call_ready_param_ptr.dual_sys);
        }
        result = MMI_RESULT_FALSE;
        break;
        
    case APP_MN_CALL_FORWARD_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_FORWARD_IND");
        //收到呼叫转移提示事件的处理
        HandleCallForwardInd(param);
        break;
        
    case APP_MN_FORWARD_STATUS_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_FORWARD_STATUS_IND");
        //收到转移状态提示事件的处理
        HandleForwardStatusInd(param);            
        break;
        
    case APP_MN_ACMMAX_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_ACMMAX_IND");
        //收到通话费用超出上限事件的处理
        HandleAcmMaxInd(param);
        break;

    case APP_MN_CALL_HELD_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_HELD_IND");
        //收到远端保持通话提示消息的处理
        HandleRemoteHoldCallInd(param);
        break;

    case APP_MN_CALL_RETRIEVED_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_CALL_RETRIEVED_IND");
        //收到远端恢复通话提示消息的处理
        HandleRemoteRetrievedCallInd(param);
        break;
        
    case APP_MN_BUILD_MPTY_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_BUILD_MPTY_IND");
        //收到远端建立多方通话提示消息的处理
        HandleRemoteBuildMPTYInd(param);
        break; 

    case APP_MN_ECT_INVOKE_CNF:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_ECT_INVOKE_CNF");		
        //A ECT ack
        HandleECTCnf(param);
        break;

    case APP_MN_ECT_INVOKE_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_ECT_INVOKE_IND");		
        //B, C notification (ECT):active, Rdn
        HandleECTInd(param);
        break;

    case APP_MN_EMG_NUM_LIST_IND:
        SCI_TRACE_LOW("MMICC_HandlePsMsg,receive APP_MN_EMG_NUM_LIST_IND");		
        HandleEmgNumListInd(param);
        break;

//lzk mark
#ifdef BT_DIALER_SUPPORT
    case APP_MN_CALL_WAIT_IND:
        {
            MMIPUB_OpenAlertWarningWin(TXT_CALL_WAITING);
        }
        break;
#endif

    default:
        result = MMI_RESULT_FALSE;
        //SCI_TRACE_LOW:"ccapp.c: CC_HandlePsMsg() CC isn't process, msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1178_112_2_18_2_10_9_8,(uint8*)"d", msg_id);
        break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 保存SetupIndParam
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void SaveSetupIndParam(
                             DPARAM param
                             )
{    
    s_setupind_param = *(APP_MN_SETUP_IND_T*)param;
}

/*****************************************************************************/
// 	Description : 载入SetupIndParam
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL DPARAM LoadSetupIndParam(void)
{
    return (DPARAM)(&s_setupind_param);
}

/*****************************************************************************/
// 	Description : 清除SetupIndParam
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
LOCAL void ClearSetupIndParam(void)
{
    SCI_MEMSET(&s_setupind_param, 0x00, sizeof(s_setupind_param));
    s_setupind_param.call_id = CC_INVALID_ID;
}

/*****************************************************************************/
// 	Description : handle MTFW Disconnected msg
//	Global resource dependence : 
//  Author:jibin
//	Note:2004.10.12
/*****************************************************************************/
LOCAL BOOLEAN HandleMTFWDisconnected(
                                     DPARAM param
                                     )
{
    uint8                   call_id = CC_INVALID_ID;
    uint8                   call_index = CC_INVALID_INDEX;
    MN_CALL_DISCONNECT_CALL_CAUSE_E    cause = MN_CAUSE_UNASSIGNED_NO;
    
    call_id = ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id;
    cause = ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->disconnected_cause;
    //SCI_TRACE_LOW:"ccapp.c:enter function HandleMTFWDisconnected() disconnected cause = %d, call_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1229_112_2_18_2_10_9_9,(uint8*)"dd", cause, call_id);
    call_index = CC_GetIndexOfCallId(((APP_MN_CALL_DISCONNECTED_IND_T*)param)->dual_sys, call_id);
    
    if (s_call_context.call_info[call_index].is_mtfw_reject)
    {
        CC_DeleteCallInfo(call_index);
        s_call_context.exist_call.exist_mtfw = FALSE;
        return TRUE;
    }
    return FALSE;
}

#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify CC
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleSIMPlugNotifyCCFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMICC]: HandleSIMPlugNotifySTKFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1247_112_2_18_2_10_9_10,(uint8*)"dd", dual_sys, notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
        if (MMIAPICC_IsInState(CC_IN_CALL_STATE) && (dual_sys == s_call_context.dual_sys))
        {
            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);
        }
        MMK_CloseWin(MMICL_CALL_COSTS_WIN_ID);
#ifdef MMI_LDN_SUPPORT
        MMIAPICL_SetIsCallInfoReady(dual_sys, FALSE);
#endif        
        break;

    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
        if (MMIAPICC_IsInState(CC_IN_CALL_STATE) && (dual_sys == s_call_context.dual_sys))
        {
            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_PROMPT);
        }

        if (dual_sys == MMIAPIUSSD_GetDualSys())
        {
            MMIAPIUSSD_CloseSendWin();
            MMIAPIUSSD_CloseInputWin();
            MMIAPIUSSD_CloseShowDataWin();
        }

#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMK_CloseWin(MMICC_COM_SELECT_SIM_WIN_ID);
        MMK_CloseWin(MMICL_COM_SELECT_SIM_WIN_ID);
        MMK_CloseWin(MAIN_SIM_SELECT_WIN_ID);
#endif
        MMK_CloseWin(MMICL_CALL_COSTS_WIN_ID);
        MMK_CloseWin(MMICL_CALL_TIMER_WIN_ID);
#ifdef MMI_GPRS_SUPPORT
        MMK_CloseWin(MMICL_GPRS_FLOWRATE_WIN_ID);
#endif

#ifdef MMI_LDN_SUPPORT
        MMIAPICL_SetIsCallInfoReady(dual_sys, TRUE);
        MMIAPICL_ResetCallInfoRecord(dual_sys);
#endif
        break;

    default:
        {
            // do nothing now!!
        }
        break;
    }
}
#endif

/*****************************************************************************/
// 	Description : init the cc global
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMICC_Init_Global(void)
{
    uint8 i = 0;
    
    //initial global variable: s_call_context
    SCI_MEMSET(&s_call_context, 0, sizeof(s_call_context));
    
    //initial the special member of s_call_context
    s_call_context.current_call = CC_INVALID_INDEX;
    for(i = 0; i < CC_MAX_CALL_NUM; i++)
    {
        s_call_context.call_info[i].call_id = CC_INVALID_ID;
    }
    //MMICC_ClearAutoRedialInfo();
    
    ClearSetupIndParam();
    
}

/*****************************************************************************/
// 	Description : process phone number
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.9.25
/*****************************************************************************/
LOCAL CC_RESULT_E ProcessPhoneNumExt(
                                     MN_DUAL_SYS_E dual_sys,
                                     uint8 *tele_num,
                                     uint8 tele_len,
                                     MMI_STRING_T* disp_ptr,
                                     MMI_STRING_T* name_ptr,
                                     BOOLEAN	clir_present,  //true, not case the value of clir_type
                                     uint8		clir_type,
                                     uint32 call_type, //call type,
                                     MN_SUBADDR_T* sub_addr
                                     )
{
    uint32 i = 0;
#if 0    
    BOOLEAN is_call_video_call = FALSE;
#endif
    //check the param
    if(PNULL== tele_num)
    {
        //SCI_TRACE_LOW:"ccapp.c: ProcessPhoneNumExt() tele_num == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1345_112_2_18_2_10_9_11,(uint8*)"");
        return CC_RESULT_ERROR;
    }
#if 0    
    if (call_type & CC_CALL_VIDEO_CALL) //video Call
    {
       is_call_video_call = TRUE;
    }
#endif

    //如果拨打方式为非默认值，则认为处于异常状态
    if (CC_DIAL_METHOD_NULL != s_dial_method)
    {
        //SCI_TRACE_LOW:"ccapp: dial_method error %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1356_112_2_18_2_10_9_12,(uint8*)"d", s_dial_method);
        //MMIPUB_OpenAlertWarningWin(TXT_CC_OPER_FAIL);
        s_cc_mo_error_type = CC_MO_OPER_FAIL;
        return CC_RESULT_ERROR;
    }
    
    //only permit one dtmf timer
    MMIICC_StopDtmfTimerId();
    
    if(tele_len > CC_MAX_TELE_NUM_LEN)
    {
        tele_len = CC_MAX_TELE_NUM_LEN;
        tele_num[tele_len] = 0;
    }
    
    for(i = 1; i < tele_len; i++)
    {
        if (CC_CODE_PLUS == tele_num[i])
        {
            tele_num[i] = 0;
            tele_len = i;
            break;
        }
    }
    
    if(MMIAPICC_IsVideoCall())
    { 
        //SCI_TRACE_LOW:"ccapp.c: ProcessPhoneNumExt() req is reject by vt"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1382_112_2_18_2_10_9_13,(uint8*)"");
        //MMIPUB_OpenAlertWarningWin(TXT_CC_OPER_FAIL);
        s_cc_mo_error_type = CC_MO_OPER_FAIL;
        return CC_RESULT_ERROR;
    }
    
    if(0 == tele_len)
    {
        //SCI_TRACE_LOW:"ccapp.c:ProcessPhoneNumExt() tele_len = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1389_112_2_18_2_10_9_14,(uint8*)"");
        return CC_RESULT_ERROR;
    }
    SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call));    
    s_call_context.want_mo_call.is_want_mo_call = TRUE;
    
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    s_call_context.dual_sys = dual_sys;
#endif
    
    //SCI_TRACE_LOW:"mmicc: ProcessPhoneNumExt, dual_sys = %d, tele_num = %s, tele_len =%d, call_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1399_112_2_18_2_10_9_15,(uint8*)"dsdd", dual_sys, tele_num, tele_len, call_type);
    
    //ready to start call
    if(CC_RESULT_SUCCESS != MakeCallReqExt(dual_sys, tele_num, tele_len, disp_ptr, name_ptr, clir_present, clir_type, call_type, sub_addr))
    {
        //SCI_TRACE_LOW:"ccapp.c:ProcessPhoneNumExt() call MMIAPICC_MakeCallReq error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1404_112_2_18_2_10_9_16,(uint8*)"");
        //clear extension number
        CC_ClearStoredDtmf();
        //clear s_call_context.want_mo_call
        SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call));
        s_call_context.want_mo_call.is_want_mo_call = FALSE;
        return CC_RESULT_ERROR;
    }
    else
    {
        //call succ, so exit TV-out
        MMIAPITVOUT_Close();
    }
#if !defined(MMI_GUI_STYLE_TYPICAL) &&defined(CALL_TIMES_SUPPORT)
    /*从idle进call logs必须在打电话时删除显示*/
    if(MMK_IsOpenWin(MMIIDLE_DIAL_WIN_ID))
    {
        MMK_CloseWin(MMICL_CALL_TIMES_WIN_ID); 
    }
#endif
#if defined(MMI_PDA_SUPPORT) 
    MMIAPIIDLE_ResetDialNum();
#else
    MMIAPIIDLE_CloseDialWin();
#endif
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
// 	Description : make a call from pb or not from pb
//	Global resource dependence : s_call_context
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E MakeCallReqExt(
                                 MN_DUAL_SYS_E dual_sys, 
                                 uint8 *src_tele_num,
                                 uint8 src_tele_len,
                                 MMI_STRING_T* disp_ptr,
                                 MMI_STRING_T* name_ptr,
                                 BOOLEAN	clir_present,  //true, not case the value of clir_type
                                 uint8		clir_type,
                                 uint32 mmi_call_type, //call type,
                                 MN_SUBADDR_T* sub_addr
                                 )
{
    
    MN_CALL_TYPE_E call_type = MN_CALL_TYPE_NORMAL;
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    uint8 copy_len = 0;
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 1] = {0};
    uint8 tele_len = 0;
    MN_CALL_CLIR_E mmi_clir_type = MMIAPISET_GetClirType(dual_sys);
    BOOLEAN is_call_video_call = FALSE;
    MN_CALLED_NUMBER_T mo_address = {0};

    //check the param
    if(PNULL== src_tele_num)
    {
        //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_MakeCallReq() tele_num == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1465_112_2_18_2_10_9_17,(uint8*)"");
        return CC_RESULT_ERROR;
    }

    SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));

    if (mmi_call_type & CC_CALL_VIDEO_CALL) //video Call
    {
       is_call_video_call = TRUE;
    }

#ifdef MMI_MULTI_SIM_SYS_SINGLE
    dual_sys = MN_DUAL_SYS_1;
#else    
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"ccapp error dualsys = %d, is_call_video_call = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1481_112_2_18_2_10_9_18,(uint8*)"dd", dual_sys, is_call_video_call);
        return CC_RESULT_ERROR;
    }
#endif
    //copy tele_num and tele_len
    MMI_MEMCPY(
        tele_num,
        (CC_MAX_TELE_NUM_LEN + 1),
        src_tele_num,
        (CC_MAX_TELE_NUM_LEN + 1),
        src_tele_len
        );
    tele_len = src_tele_len;
    
    //if total call number is beyond 7
    if(s_call_context.call_number >= CC_MAX_CALL_NUM )
    {
        //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() call_number= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1498_112_2_18_2_10_9_19,(uint8*)"d", s_call_context.call_number);
        return CC_RESULT_ERROR; 
    }
    if(tele_len > CC_MAX_TELE_NUM_LEN)
    {
        //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() tele_len= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1503_112_2_18_2_10_9_20,(uint8*)"d", tele_len);
        tele_len = CC_MAX_TELE_NUM_LEN;
        tele_num[tele_len] = 0;
    }	
    if(0 == tele_len)
    {
        //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() tele_len = 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1509_112_2_18_2_10_9_21,(uint8*)"");
        return CC_RESULT_ERROR;
    }

    //get mo address
    //convert str to BCD
    if(!MMIAPICOM_GenPartyNumber(tele_num, tele_len, &party_num))
    {
        //SCI_TRACE_LOW:"ccapp.c:MakeCallReqExt() CC_GenPartyNumber return failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1517_112_2_18_2_10_9_22,(uint8*)"");
        return CC_RESULT_ERROR;
    }

    mo_address.number_type = party_num.number_type ;
    mo_address.number_plan = MN_NUM_PLAN_ISDN_TELE;
    
    SCI_MEMSET((void*)mo_address.party_num, 0xFF, sizeof(mo_address.party_num));
    
    copy_len = ( party_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num.num_len;
    MMI_MEMCPY(
        (void*)mo_address.party_num,
        sizeof(mo_address.party_num),
        (void*)party_num.bcd_num,
        sizeof(party_num.bcd_num),
        copy_len
        );
    mo_address.num_len = copy_len ;
    
    //process the extension number
    if(!CC_ProcessExtensionNum(tele_num, &tele_len))
    {
        //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() CC_ProcessExtensionNum error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1539_112_2_18_2_10_9_23,(uint8*)"");
        //提示无效号码
        //MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
        s_cc_mo_error_type = CC_MO_INVALID_NUM;
        return CC_RESULT_ERROR; 
    }
    
    //if there is emergency call
    if (MMIAPICC_IsEmergencyNum(tele_num, tele_len, dual_sys, FALSE))
    {
        //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() call_type = MN_CALL_TYPE_EMERGENCY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1548_112_2_18_2_10_9_24,(uint8*)"");
        call_type = MN_CALL_TYPE_EMERGENCY;
    }
    else
    {
        //s_call_context.want_mo_call.call_type = is_call_video_call;	//@samboo 非紧急呼叫号码才可以拨打vt
        call_type = (is_call_video_call) ? MN_CALL_TYPE_DATA : MN_CALL_TYPE_NORMAL;
    }
    //if setup_ind msg 已经收到，拨号失败
    if (CC_INVALID_ID != s_setupind_param.call_id)
    {
        //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() setup_ind msg is being"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1559_112_2_18_2_10_9_25,(uint8*)"");
        //MMIPUB_OpenAlertWarningWin(TXT_DIAL_FAILED);
        s_cc_mo_error_type = CC_MO_DIAL_FAILED;
        return CC_RESULT_ERROR;         
    }        
    
    s_call_context.exist_call.exist_outgoing_call = TRUE;
    s_call_context.want_mo_call.call_type = mmi_call_type;
    s_call_context.want_mo_call.clir_present = clir_present;
    s_call_context.want_mo_call.clir_type = clir_type;
#ifdef MMI_CSP_SUPPORT
    s_call_context.want_mo_call.line = MMIAPISET_GetCallLineInfo(s_call_context.dual_sys);
#endif
    
    if (!GetWantMoCallInfo(dual_sys,
                                tele_num,
                                tele_len,
                                disp_ptr,
                                name_ptr,
                                sub_addr,
                                &s_call_context.want_mo_call
                                ))
    {
        //SCI_TRACE_LOW:"mmiccapp: s_call_context.want_mo_call.address.num_len = %d, tele_num = %s, tele_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1590_112_2_18_2_10_10_27,(uint8*)"dsd", s_call_context.want_mo_call.address.num_len, tele_num, tele_len);

        return CC_RESULT_ERROR;
    }

    if (MN_CALL_TYPE_EMERGENCY == call_type)
    {
        if (CC_NO_NUM != MMICC_GetCallNum())
        {
            SCI_TRACE_LOW("ccapp.c: emergency call and exist other call");

            s_call_context.operate_waiting_call = RELEASE_ALL_CALL_AND_MAKE_CALL;

            MMIAPICC_ReleaseAllCallExt(CC_RELEASE_ALL_NORMAL);

            return CC_RESULT_SUCCESS;
        }
    }

#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
    MMIMEX_AQIPPhNum(party_num, &s_call_context,copy_len,tele_num, &tele_len);
#endif

    if(clir_present)
    {
        mmi_clir_type = (MN_CALL_CLIR_E)clir_type;
    }
    
    //SCI_TRACE_LOW:"ccapp.c:MakeCallReqExt() mmi_clir_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1727_112_2_18_2_10_10_28,(uint8*)"d", mmi_clir_type);

    if(s_dial_method != CC_DIAL_METHOD_NULL)
    {
        //SCI_TRACE_MID:"ERR: ccapp dial method %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1731_112_2_18_2_10_10_29,(uint8*)"d", s_dial_method);
    }
    
    s_dial_method = CC_DIAL_METHOD_MMI;//mn task优先级高于mmi task,为避免mn消息先发上来，造成系统异常，所以先设置，如果呼叫失败再进行复位

#ifndef WIN32
    if(MN_CALL_TYPE_EMERGENCY == call_type)
    {
         /* fix compile error, modified by zhigang.peng 2019-07-15 */
        if(MN_RETURN_SUCCESS != MNCALL_StartCallWithEscvEx(
                                                        dual_sys,
                                                        call_type,           // The call type:i.e"normal call,emergency call'
                                                        &(s_call_context.want_mo_call.address),
                                                        sub_addr, 
                                                        mmi_clir_type, 
                                                        PNULL,
                                                        FALSE,
                                                        0,
                                                        NULL  // new add
                                                        )
        )
        {
            s_dial_method = CC_DIAL_METHOD_NULL;
            s_call_context.exist_call.exist_outgoing_call = FALSE;
            if (ERR_MNCALL_FAIL_BY_FDN == MNCALL_GetErrCodeEx(dual_sys))
            {
                //MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
                //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() MNCALL_StartCall Fail by FDN"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1754_112_2_18_2_10_10_30,(uint8*)"");
                //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_FDN_ONLY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);
                s_cc_mo_error_type = CC_MO_FDN_ONLY;
            }
            else
            {
                //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() MNCALL_StartCall return failure"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1759_112_2_18_2_10_10_31,(uint8*)"");
                //MMIPUB_OpenAlertWarningWin(TXT_DIAL_FAILED);
                s_cc_mo_error_type = CC_MO_DIAL_FAILED;
            }
            return CC_RESULT_ERROR;
        }
    }
    else
    {
#endif
#ifdef MMI_DUALMODE_ENABLE
        /* fix compile error, modified by zhigang.peng 2019-07-15 */
        if (MN_RETURN_SUCCESS != MNCALL_StartCallEx(dual_sys, call_type,
            &(s_call_context.want_mo_call.address),
            sub_addr,
            mmi_clir_type,
            PNULL,PNULL ) )
#else
        if (MN_RETURN_SUCCESS != MNCALL_StartCallWithFdnCheckEx(dual_sys, call_type,
            &mo_address,
            sub_addr,
            mmi_clir_type,
            PNULL ) )
#endif
        {
            s_dial_method = CC_DIAL_METHOD_NULL;
            s_call_context.exist_call.exist_outgoing_call = FALSE;
            if (ERR_MNCALL_FAIL_BY_FDN == MNCALL_GetErrCodeEx(dual_sys))
            {
                //MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
                //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() MNCALL_StartCall Fail by FDN"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1754_112_2_18_2_10_10_30,(uint8*)"");
                //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_FDN_ONLY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);
                s_cc_mo_error_type = CC_MO_FDN_ONLY;
            }
            else
            {
                //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() MNCALL_StartCall return failure"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1759_112_2_18_2_10_10_31,(uint8*)"");
                 //MMIPUB_OpenAlertWarningWin(TXT_DIAL_FAILED);
                s_cc_mo_error_type = CC_MO_DIAL_FAILED;
             }
            return CC_RESULT_ERROR; 
        }
    
#ifndef WIN32
    }
#endif
    
    //MMIAUDIO_HandleNewMsgKey();
    
#ifdef FM_SUPPORT
    MMIAPIFM_StopRecord();
    MMIAPIFM_StopAutoSearch();
#endif
    //suspend Mp3 or FM background play
    //MMIAUDIO_PauseBgPlay(MMIBGPLAY_MODULE_CC);
	
    MMIAPIFMM_ExitFileOption();
    
    MMIAPIDC_Interrupt();
//	MMIAPICC_StopCCRecord();
#ifdef MMI_RECORD_SUPPORT
    MMIAPIRECORD_StopRecordOrPlay(); //打出电话的时候停止录音
#endif    
    MMIAPIPICVIEW_StopDeleteFiles();
    //not necessary here
#if defined(VIDEO_PLAYER_SUPPORT) && !defined(MMI_VIDEOPLAYER_MINI_FUNCTION)
    MMIAPIVP_NotifyVPStop();
#endif

#ifdef MOBILE_VIDEO_SUPPORT
    if(MMIAPIMV_IsLiving())
    {
        MMIAPIMV_Exit();
    }
#endif           
    //MMIAUDIO_EnableKeyRing(MMIAUDIO_KEY_ALL, MMIBGPLAY_MODULE_CC, FALSE);
    
    //@liqing.peng, cr72005
//    MMICC_EnableMute(FALSE);//cr228324
    //MMICC_EnableRemoteMute(FALSE); //bug 184484

    s_is_downlnk_mute_mocalling_flag = MO_CALL_STATE_INIT;
    s_is_downlnk_mute_full_circle = TRUE;
//============== MET MEX start ===================
#ifdef MET_MEX_SUPPORT
    if(MMIMEX_IsOutCalling())
    {
        //mex application registered to hook this message
        return CC_RESULT_SUCCESS;
    }   
#endif    
//============== MET MEX end ===================    
    //修改CALL LOG resolve in the future
    MMICC_UpdateCallStatusDisplay(MMICC_MO_START_IND);
    MMICC_EnableRemoteMute(FALSE); //bug 184484
    return CC_RESULT_SUCCESS; 
}

/*****************************************************************************/
// 	Description : If ip number is included in telephone number, remove it
//	Global resource dependence : s_call_context
//  Author:lin.lin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_RemoveIpNumber(
                                              uint8*  tele_num_ptr, 
                                              uint8   tele_len, 
                                              uint8*  origin_number_ptr,
                                              uint8   origin_number_len
                                              )
{
#ifdef MMI_IP_CALL_SUPPORT
    uint8 ip_number[MMISET_IPNUMBER_MAX_LEN+1] = {0};
    uint8 ip_number_len = 0;

    if (PNULL == tele_num_ptr || 0 == tele_len)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIAPICC_RemoveIpNumber:%s,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1833_112_2_18_2_10_10_32,(uint8*)"sd", tele_num_ptr, tele_len);
    
    MMIAPISET_GetIPNumber(s_call_context.dual_sys, tele_num_ptr, ip_number, &ip_number_len);
    
    tele_num_ptr[tele_len] = 0;
    if ( (0 != ip_number_len ) && (tele_len > ip_number_len) && (0 == strncmp((char*)tele_num_ptr, (char*)ip_number, ip_number_len)))
    {
        if(origin_number_len >= tele_len - ip_number_len + 1) 
        {
            strcpy((char*)origin_number_ptr, (char*)tele_num_ptr + ip_number_len);
            return TRUE;
        }
        else
        {
            //SCI_TRACE_LOW:"ccapp error origin_number_len = %d, tele_len = %d, ip_number_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1847_112_2_18_2_10_10_33,(uint8*)"ddd", origin_number_len, tele_len, ip_number_len);
            return FALSE;
        }
    }
    return FALSE;    
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : Get name from phonebook list
//	Global resource dependence : s_call_context
//  Author:lin.lin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNameFromPb(
                            CC_WANT_MO_CALL_T *want_mo_call_ptr,//[IN]
                            uint8*			tele_num,		//[IN]
                            uint8			tele_len,		//[IN]
                            MMI_STRING_T	*name_ptr,		//[OUT]
                            uint16			*entry_id_ptr,	//[OUT]
                            uint8			*group_id_ptr	//[OUT]
                            )
{
    uint8					origin_number_arr[CC_MAX_TELE_NUM_LEN] = {0};/*lint !e64*/
    int16                   origin_num_len                         = 0;
    BOOLEAN					b_search_result                        = FALSE;
    MMIPB_BCD_NUMBER_T		pb_num                                 = {0};/*lint !e64*/
    MMI_PARTY_NUMBER_T		party_num                              = {0};/*lint !e64*/
    
    SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));
    if(PNULL== name_ptr || entry_id_ptr == PNULL ||group_id_ptr == PNULL || PNULL == want_mo_call_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c: GetNameFromPb() name_ptr  ||entry_id_ptr || group_id_ptr is null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1875_112_2_18_2_10_10_34,(uint8*)"");
        return FALSE; 
    }

    pb_num.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(want_mo_call_ptr->address.number_type, MN_NUM_PLAN_UNKNOW);
    pb_num.number_len = MIN( want_mo_call_ptr->address.num_len , MMIPB_BCD_NUMBER_MAX_LEN );					
    MMI_MEMCPY(
        pb_num.number,
        sizeof(pb_num.number),
        want_mo_call_ptr->address.party_num,
        sizeof(want_mo_call_ptr->address.party_num),
        pb_num.number_len
        );

    b_search_result = MMIAPIPB_GetNameByNumber(&pb_num, name_ptr, CC_MAX_PBNAME_LEN, FALSE);/*lint !e506 */
    
    if (
#ifdef BT_DIALER_SUPPORT
        !MMICC_GetBtCallFlag() &&
#endif
        !b_search_result && MMIAPICC_RemoveIpNumber(tele_num, tele_len, origin_number_arr,CC_MAX_TELE_NUM_LEN))    
    {
        SCI_MEMSET(&pb_num, 0x00, sizeof(pb_num));
    
        origin_num_len = strlen((char*)origin_number_arr);
        
        if(MMIAPICOM_GenPartyNumber(origin_number_arr, origin_num_len, &party_num))
        {
            pb_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            pb_num.number_len = MIN(MN_MAX_ADDR_BCD_LEN, party_num.num_len);
            MMI_MEMCPY(
                pb_num.number,
                sizeof(pb_num.number),
                (void*)party_num.bcd_num,
                sizeof(party_num.bcd_num),
                party_num.num_len
                );        

            b_search_result = MMIAPIPB_GetNameByNumber(&pb_num, name_ptr, CC_MAX_PBNAME_LEN, FALSE);/*lint !e506 */
        }
        else
        {
            return FALSE;
        }
    }
    
    SCI_TRACE_BUF("ccapp.c, GetNameFromPb pb_num=%x", pb_num.number, sizeof(pb_num.number));
    //SCI_TRACE_LOW:"ccapp.c, GetNameFromPb search_result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1915_112_2_18_2_10_10_35,(uint8*)"d", b_search_result);
    return b_search_result;
}


/*****************************************************************************/
//  Description : get current exited call count
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8	MMIAPICC_GetExitedCallCount(void)
{
    uint8 call_count = 0;
#ifdef VT_SUPPORT
    if (MMIAPIVT_IsVtCalling())
    {
        call_count = 1;
    }
    else
#endif
    {
        //SCI_TRACE_LOW:"MMIAPICC_GetExitedCallCount: The active call number is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1935_112_2_18_2_10_10_36,(uint8*)"d",s_call_context.exist_call.exist_active_call_num);
        call_count = s_call_context.exist_call.exist_active_call_num + s_call_context.exist_call.exist_hold_call_num;
    }
    return	(call_count);	
}

/*****************************************************************************/
//  Discription: the blue tooth headset  accept the incoming call
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPPICC_BtAcceptCall(void)
{
#ifdef BLUETOOTH_SUPPORT
    CC_CALL_STATE_E			call_status = CC_INIT_STATE;
    
    call_status = MMICC_GetCurrentCallStatus();
    
    //只有在Incoming Call状态才处理这个消息，别的通话状态均不处理
    if (CC_INCOMING_CALL_STATE == call_status)
    {	
        MMK_SendMsg(WATCHCC_MTCALLING_WIN_ID,MSG_BT_ACCEPT_INCOMING_CALL,PNULL);
    }
    else if (CC_WAITING_STATE == call_status)
    {
        CC_HoldAndAnswerCW();
        //SCI_TRACE_LOW:"MMIAPPICC_BtAcceptCall: hold and answer call!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1961_112_2_18_2_10_10_37,(uint8*)"");
    }
#ifdef VT_SUPPORT
    else if (MMIAPIVT_IsVtMtIncoming())
    {
        MMIAPIVT_ConnectCall();
    }
#endif
    //SCI_TRACE_LOW:"MMIAPPICC_BtAcceptCall: the call status is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1969_112_2_18_2_10_10_38,(uint8*)"d", call_status);
#endif
}

/*****************************************************************************/
//  Discription: the blue tooth headset cancel the call including(incoming and outgoing call)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPICC_BtCancelCall(void)
{
#ifdef BLUETOOTH_SUPPORT
    CC_CALL_STATE_E			call_status = CC_INIT_STATE;
    
    call_status = MMICC_GetCurrentCallStatus();
    //SCI_TRACE_LOW:"MMIAPICC_BtCancelCall: the call status is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_1984_112_2_18_2_10_10_39,(uint8*)"d",call_status);
    
    //Incoming call时，拒接来电
    if (CC_INCOMING_CALL_STATE == call_status)
    {
        MMK_SendMsg(WATCHCC_MTCALLING_WIN_ID,MSG_BT_REJECT_INCOMING_CALL,PNULL);
    }
    else if( CC_CALLING_STATE == call_status)
    {
        MMK_SendMsg(WATCHCC_MOCALLING_WIN_ID,MSG_BT_CANCEL_OUTGOING_CALL,PNULL);
#if !defined(MMI_GUI_STYLE_TYPICAL)
        MMK_SendMsg(WATCHCC_CONNECTED_WIN_ID,MSG_BT_CANCEL_OUTGOING_CALL,PNULL);
#endif
    }
    else if ( CC_CALL_STATE == call_status || CC_HOLD_STATE == call_status || CC_WAITING_STATE == call_status)//正在通话中，将电话挂断
    {
        if (MMK_IsOpenWin(WATCHCC_CONNECTED_WIN_ID))
        {
            MMK_SendMsg(WATCHCC_CONNECTED_WIN_ID,MSG_BT_CANCEL_CALL,PNULL);
        }
        else if (CC_IsCallIDValid(s_call_context.current_call))
        {
            MMIAPICC_ReleaseCallByRedkey();
        }
    }
#ifdef VT_SUPPORT
    else if (MMIAPIVT_IsVtMtIncoming() || MMIAPIVT_IsVtConnecting())
    {
        MMIAPIVT_ReleaseVTCallByRedkey();
    }
#endif
#endif
}

/*****************************************************************************/
//  Discription: receive amr type ind,if amr-wb,display hd icon.
//  Global resource dependence: None
//  Author: xiaoju.cheng
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL void CC_AMRTypeInd(DPARAM param)
{
    APP_MN_AMR_TYPE_IND_T	*sig_ptr = (APP_MN_AMR_TYPE_IND_T*)param;

    SCI_TRACE_LOW("CC_AMRTypeInd,amr_type is %d",sig_ptr->amr_type);
    /* fix compile error, modified by zhigang.peng 2019-07-15 */	
    if(sig_ptr->amr_type == 1)   // MN_AMR_TYPE_WB
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HD_CALL, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HD_CALL, FALSE);
    }
    GUIWIN_UpdateStbIcon();  
}

/*****************************************************************************/
// 	Description : process when received signal APP_MN_CALL_START_IND
//	Global resource dependence : s_call_context
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallStartInd(DPARAM param)
{
    APP_MN_CALL_START_IND_T	*sig_ptr = (APP_MN_CALL_START_IND_T*)param;
    uint8				copy_len = 0;
    
    if(PNULL== sig_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_CallStartInd() pSig == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2031_112_2_18_2_10_10_41,(uint8*)"");
        return;
    }

    //check the param
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_CallStartInd(), call_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2028_112_2_18_2_10_10_40,(uint8*)"d", sig_ptr->call_id);
    
    if( sig_ptr->call_id >= CC_MAX_CALL_NUM )
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_CallStartInd() , call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2037_112_2_18_2_10_10_42,(uint8*)"d", sig_ptr->call_id);
        return;
    }
	
#ifdef MMI_INDIAN_SOS_SUPPORT
	if(CC_IsSosCalling()){
		CC_SetSosCallId(sig_ptr->call_id);
	}
#endif
	
    //modify global: s_call_context	
    s_call_context.call_number++;
    s_call_context.current_call = s_call_context.call_number - 0x01;
    //get name from control
    //SCI_MEMCPY(s_call_context.call_info[s_call_context.current_call].name, name, strlen(name));
    s_call_context.call_info[s_call_context.current_call].call_id = sig_ptr->call_id;
    s_call_context.call_info[s_call_context.current_call].call_state = CC_CALLING_STATE;
    s_call_context.call_info[s_call_context.current_call].dual_sys = sig_ptr->dual_sys;
    s_call_context.dual_sys = sig_ptr->dual_sys;
    
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
    copy_len = s_call_context.want_mo_call.name_len;
#else    
    copy_len = ( s_call_context.want_mo_call.name_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : s_call_context.want_mo_call.name_len;/*lint !e506 */
#endif
    MMI_WSTRNCPY(
        s_call_context.call_info[s_call_context.current_call].name,
        sizeof(s_call_context.call_info[s_call_context.current_call].name),
        s_call_context.want_mo_call.name,
        sizeof(s_call_context.want_mo_call.name),
        copy_len
        );
    s_call_context.call_info[s_call_context.current_call].name_len = copy_len;
    s_call_context.call_info[s_call_context.current_call].address = s_call_context.want_mo_call.address;
    s_call_context.call_info[s_call_context.current_call].is_save_name = s_call_context.want_mo_call.is_save_name;
    s_call_context.call_info[s_call_context.current_call].is_name_exist = s_call_context.want_mo_call.is_name_exist;
    if (s_call_context.want_mo_call.call_type & CC_CALL_STK_CALL) //stk Call
    {
       s_call_context.call_info[s_call_context.current_call].is_stk_call = TRUE;
    }
    else
    {
       s_call_context.call_info[s_call_context.current_call].is_stk_call = FALSE;
    }
    
    s_call_context.call_info[s_call_context.current_call].call_type = CC_MO_CALL_TYPE;
    s_call_context.call_info[s_call_context.current_call].call_time.start_time = MMIAPICOM_GetCurTime( );
    s_call_context.call_info[s_call_context.current_call].call_time.stop_time
        = s_call_context.call_info[s_call_context.current_call].call_time.start_time;
    s_call_context.call_info[s_call_context.current_call].is_mute = FALSE;
#ifdef MMI_CSP_SUPPORT
    s_call_context.call_info[s_call_context.current_call].line = s_call_context.want_mo_call.line;
#endif

    //dtmf
    if (s_call_context.dtmf_state.is_dtmf)
    {
        s_current_dtmf_call_id = sig_ptr->call_id;
    }

    //Save modified number
    if (s_is_call_modified_store)
    {
        s_is_call_modified_store = FALSE;

        s_call_context.call_info[s_call_context.current_call].is_call_modified_store = TRUE;

        SCI_MEMSET(&s_call_context.call_info[s_call_context.current_call].call_modified_address, 0x00, sizeof(s_call_context.call_info[s_call_context.current_call].call_modified_address));

        SCI_MEMCPY(&s_call_context.call_info[s_call_context.current_call].call_modified_address,
                            &s_call_modified_address,
                            sizeof(s_call_context.call_info[s_call_context.current_call].call_modified_address));
    }

    //INIT AUDIO to send bt connect req
    MMICC_ConstructVoiceCodec(FALSE);
#ifdef BLUETOOTH_SUPPORT
    //#ifdef MMI_BLUETOOTH
    MMIAPIBT_OutgoingCall();
    //#endif
#endif
#ifdef DPHONE_SUPPORT
	if (!MMIDEFAULT_GetHookStates())
	{
	    //SCI_TRACE_LOW:"---WD:CC_CallStartInd deal with hook"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2097_112_2_18_2_10_11_43,(uint8*)"");
	    MMIDEFAULT_SetHandFreeStatesAndLed(TRUE);
	}
#endif

    MMICC_EnableMute(MMICC_IsCurrentCallMute());
#ifdef MMI_VOLTE_SUPPORT
    if(sig_ptr->is_ip_call)
    {
        SCI_TRACE_LOW("ccapp.c: CC_CallStartInd() PS call Enable codec");
        s_call_context.pscall_info.is_ps_call = TRUE;
        MMICC_EnableVoiceCodec(TRUE);//for mo, enable codec is done when receive sync ind in 2G
        CC_SetCallPathStatus( sig_ptr->dual_sys, TRUE);
    }
#endif
}

/*****************************************************************************/
// 	Description : process when received signal APP_MN_SYNC_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_SyncInd(DPARAM param)
{
    APP_MN_SYNC_IND_T *pSig = (APP_MN_SYNC_IND_T*)param;
    uint8 index = 0;
    
    //check the param
    
    if(PNULL== pSig)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_SyncInd() pSig == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2119_112_2_18_2_10_11_45,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"ccapp.c:enter function CC_SyncInd(), sync_cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2116_112_2_18_2_10_11_44,(uint8*)"d", pSig->sync_cause);
    
    if(MN_CALL_SYNC_RESOURCE_ASSIGNMENT == pSig->sync_cause)
    {
        index = s_call_context.current_call;
        if (CC_MAX_CALL_NUM <= index)
        {
            //SCI_TRACE_LOW:"ccapp SyncInd current call index:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2128_112_2_18_2_10_11_46,(uint8*)"d", index);
            return;
        }
        if (s_call_context.call_info[index].is_mtfw_reject)
        {
            //SCI_TRACE_LOW:"ccapp SyncInd param mtfw index:%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2133_112_2_18_2_10_11_47,(uint8*)"d", index);
        }
        else
        {
            //c133748
            s_mt_need_open_codec_flag = s_mt_need_open_codec_flag|CC_TCH_RESOURCE_ASSIGNED;
            //open speaker and microphone
            CC_OpenAudioTunnel(TRUE);
    
            if (s_call_context.call_info[index].call_type == CC_MO_CALL_TYPE) 
            {
#ifdef BLUETOOTH_SUPPORT
                MMIAPIBT_RemoteAlerting();
#endif
            }
            // 放到外边来，因为里面有一个相同的判断条件，不满足时会有处理
            HandleMoMuteAndLocalRingState(APP_MN_SYNC_IND);
        }
    }
    else if (MN_CALL_SYNC_CIPHERING == pSig->sync_cause)
    {
        if (MMIAPIPHONE_IsOfmActived(pSig->dual_sys))
        {
            s_call_context.call_info[index].is_unciphering_call = TRUE;
        }
    }
    
    s_call_context.dtmf_state.allow_send_flag = (s_call_context.dtmf_state.allow_send_flag | DTMF_MN_SYNC_IND_BIT);
#if !defined(MMI_GUI_STYLE_TYPICAL)
    if (MMICC_IsAllowSendDtmf())
    {
        MMK_PostMsg(MMICC_STATUS_WIN_ID, MSG_CC_ALLOW_SEND_DEMF_MSG, PNULL, 0);
        MMK_PostMsg(MMICC_ANIMATION_WIN_ID, MSG_CC_ALLOW_SEND_DEMF_MSG, PNULL, 0);
    }
#endif
    
}


/*****************************************************************************/
// 	Description : process when received signal APP_MN_ALERTING_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallAlertingInd(DPARAM param)
{
    APP_MN_ALERTING_IND_T *pSig = (APP_MN_ALERTING_IND_T*)param;
    BOOLEAN result = FALSE;
    
    //check the param
    
    if(PNULL== pSig)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_CallAlertingInd() pSig == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2174_112_2_18_2_10_11_49,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"ccapp.c:enter function CC_CallAlertingInd(), call_id = %d, alerting_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2171_112_2_18_2_10_11_48,(uint8*)"dd", pSig->call_id, pSig->alerting_type);
        
    if(  pSig->call_id >= CC_MAX_CALL_NUM )
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_CallAlertingInd() , call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2179_112_2_18_2_10_11_50,(uint8*)"d", pSig->call_id);
        return;
    }
#ifdef MET_MEX_SUPPORT
    if(1 == MMIMex_Phs_HookResult(MEX_MSG_ID_CC_CALL_RING_IND, &s_setupind_param, 0, NULL, MEX_HOOK_MSG_FROM_MMITASK))
    {
        mex_log(1, "hook ccalerting");
        //mex application registered to hook this message
        return;
    }
#endif
    //get amr type,if result is wb,display hd icon.
    #ifndef WIN32
    result = em_get_amr_wb_from_network();
	#endif
    SCI_TRACE_LOW("CC_CallAlertingInd,result is %d",result);

    if(result == TRUE)
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HD_CALL, TRUE);
    }
    else
    {
        GUIWIN_SetStbItemVisible(MMI_WIN_ICON_HD_CALL, FALSE);
    }
    GUIWIN_UpdateStbIcon();  

    switch( pSig->alerting_type )
    {
    case MN_CALL_IN_BAND_AVAILABLE:        
        break;
        
        //MO,收到对端发送来的ALERT消息
    case MN_CALL_REMOTE_USER_ALERTING:
#ifdef MMI_VOLTE_SUPPORT
        if(pSig->is_ip_call)
        {
            StopLocalNetRing();
            SCI_TRACE_LOW("volte ccapp.c: CC_CallAlertingInd() Remote ring: %d ",s_call_context.pscall_info.is_volte_codec_on);
            if(!s_call_context.pscall_info.is_volte_codec_on)
            {
                //in fact, volte codec is already enable when receive APP_MN_CALL_EARLY_MEDIA_IND
                CC_EnableVoLTECodec(TRUE);
            }
        }
        else
#endif
        {
            HandleMoMuteAndLocalRingState(APP_MN_ALERTING_IND);
        }
        break;
            
            //MT,indicate alerting
    case MN_CALL_LOCAL_USER_ALERTING: 
        //modify for MT Firewall
#ifdef MMI_VOLTE_SUPPORT
        if(TRUE == pSig->is_ip_call)
        {
            SCI_TRACE_LOW("volte ccapp.c: CC_CallAlertingInd() local ring: %d ",pSig->alerting_type);
            HandleMoMuteAndLocalRingState(APP_MN_ALERTING_IND);
        }
        else
#endif
        {
             if(pSig->call_id == ((APP_MN_SETUP_IND_T*)LoadSetupIndParam())->call_id)
             {
                 //MNCALL_IndicateAlert( pSig->call_id, PNULL );
                 MNCALL_IndicateAlertEx(pSig->dual_sys, pSig->call_id, PNULL );
                 CC_CallSetupInd(LoadSetupIndParam());
                 ClearSetupIndParam();
             }
        }
        break;
        
    default:
        //SCI_TRACE_LOW:"ccapp.c:CC_CallAlertingInd() alerting_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2217_112_2_18_2_10_11_51,(uint8*)"d", pSig->alerting_type);
        break;
    }	
}

/*****************************************************************************/
// 	Description : process when received signal APP_MN_PROGRESS_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_CallProgressInd(DPARAM param)
{
    APP_MN_PROGRESS_IND_T *pSig = (APP_MN_PROGRESS_IND_T*)param;
    
    //check the param
    
    if(PNULL== pSig)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_CallProgressInd() pSig == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2235_112_2_18_2_10_11_53,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"ccapp.c:enter function CC_CallProgressInd(), descrip = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2232_112_2_18_2_10_11_52,(uint8*)"d",pSig->progress_ind.descrip);
    
    if (((pSig->progress_ind.descrip >= 1) && (pSig->progress_ind.descrip <= 3))
        ||((pSig->progress_ind.descrip >= 6) && (pSig->progress_ind.descrip <= 20)))
    {
        s_call_context.dtmf_state.allow_send_flag = (s_call_context.dtmf_state.allow_send_flag | DTMF_MN_PROGRESS_IND_BIT);
        HandleMoMuteAndLocalRingState(APP_MN_PROGRESS_IND);

#if !defined(MMI_GUI_STYLE_TYPICAL)
        if (MMICC_IsAllowSendDtmf())
        {
            MMK_PostMsg(MMICC_STATUS_WIN_ID, MSG_CC_ALLOW_SEND_DEMF_MSG, PNULL, 0);
            MMK_PostMsg(MMICC_ANIMATION_WIN_ID, MSG_CC_ALLOW_SEND_DEMF_MSG, PNULL, 0);
        }
#endif
    }

}



/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetWantMoCallInfo(MN_DUAL_SYS_E dual_sys,
                                                                uint8 *tele_num,
                                                                uint16 tele_len,
                                                                MMI_STRING_T* disp_ptr,
                                                                MMI_STRING_T* name_ptr,
                                                                MN_SUBADDR_T *sub_addr,
                                                                CC_WANT_MO_CALL_T *want_mo_call_ptr)
{
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    BOOLEAN b_search_result = FALSE;
    MMI_STRING_T name = {0};
    uint8 copy_len = 0;
    uint8 pb_group_id = 0;
    uint16 pb_entry_id = 0;

    if (PNULL == tele_num
        || 0 == tele_len
        || PNULL == want_mo_call_ptr
        || dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));

    //convert str to BCD
    if (!MMIAPICOM_GenPartyNumber(tele_num, tele_len, &party_num))
    {
        SCI_TRACE_LOW("ccapp.c:GetWantMoCallInfo() CC_GenPartyNumber return failure");

        return FALSE;
    }

    want_mo_call_ptr->address.number_type = party_num.number_type ;
    want_mo_call_ptr->address.number_plan = MN_NUM_PLAN_ISDN_TELE;
    
    SCI_MEMSET((void*)want_mo_call_ptr->address.party_num, 0xFF, sizeof(want_mo_call_ptr->address.party_num));
    
    copy_len = ( party_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num.num_len;
    MMI_MEMCPY(
        (void*)want_mo_call_ptr->address.party_num,
        sizeof(want_mo_call_ptr->address.party_num),
        (void*)party_num.bcd_num,
        sizeof(party_num.bcd_num),
        copy_len
        );

    want_mo_call_ptr->address.num_len = copy_len ;
#ifdef BT_DIALER_SUPPORT
    if(!MMICC_GetBtCallFlag())
#endif
{
    want_mo_call_ptr->dual_sys = dual_sys;
}
    //sub addr
    SCI_MEMSET(&want_mo_call_ptr->sub_addr, 0x00, sizeof(want_mo_call_ptr->sub_addr));

    if (PNULL != sub_addr)
    {
        want_mo_call_ptr->is_sub_addr_present = TRUE;
        SCI_MEMCPY(&want_mo_call_ptr->sub_addr, sub_addr, sizeof(want_mo_call_ptr->sub_addr));
    }
    else
    {
        want_mo_call_ptr->is_sub_addr_present = FALSE;
    }
    
    SCI_TRACE_LOW("mmiccapp: want_mo_call_ptr->address.num_len = %d, tele_num = %s, tele_len = %d", want_mo_call_ptr->address.num_len, tele_num, tele_len);
    
    //调用pb接口获得相应的电话信息
    SCI_MEMSET( &name, 0, sizeof(name) );
    SCI_MEMSET( want_mo_call_ptr->name, 0, sizeof(want_mo_call_ptr->name) );
    name.wstr_ptr = want_mo_call_ptr->name;

    SCI_MEMSET(want_mo_call_ptr->specified_str, 0x00, sizeof(want_mo_call_ptr->specified_str));

    //如果是紧急呼叫号码
    //bug 186648 modify is_only_for_display(the last parameter of MMIAPICC_IsEmergencyNum) true to false,is_only_for_display is setted true is to some requirement,
    //but in different countries in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num(no sim emergency) ,
    //it is not normal display emergency call in window
    if (
#ifdef BT_DIALER_SUPPORT
        !MMICC_GetBtCallFlag() &&
#endif
	    MMIAPICC_IsEmergencyNum(tele_num, tele_len, dual_sys, FALSE))
    {
        MMI_GetLabelTextByLang(TXT_KL_PSW_EMERGENCY_CALL, &name);

        MMI_WSTRNCPY(
            want_mo_call_ptr->name,
            name.wstr_len,
            name.wstr_ptr,
            name.wstr_len,
            name.wstr_len
            );
        want_mo_call_ptr->name_len = name.wstr_len;
    }
    else
    {
        if (PNULL != disp_ptr && 0 != disp_ptr->wstr_len && PNULL != disp_ptr->wstr_ptr)  //not from pb
        {
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
            copy_len = disp_ptr->wstr_len;
#else
            copy_len = ( disp_ptr->wstr_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : disp_ptr->wstr_len;
#endif

            MMI_WSTRNCPY(
                (void*)want_mo_call_ptr->specified_str,
                sizeof(want_mo_call_ptr->specified_str) - 1,
                (void*)disp_ptr->wstr_ptr,
                disp_ptr->wstr_len,
                copy_len
                );

            want_mo_call_ptr->specified_str_len = copy_len;

            want_mo_call_ptr->is_exist_specified_str = TRUE;
        }
        else
        {
            want_mo_call_ptr->specified_str_len = 0;
            want_mo_call_ptr->is_exist_specified_str = FALSE;
        }
        
        /*打开宏CL_SAVE_NAME_SUPPORT，则fdn、sdn、adn等列表记录有姓名，使用该姓名拨号并保存至CL，即使姓名为空*/
        /*拨号盘、sms、mms、email、www等不存储姓名的，使用pb中的姓名*/
        if (PNULL != name_ptr)
        {
            want_mo_call_ptr->is_save_name = TRUE;//这是从ADN/FDN/SDN发起的通话. this is a call from ADN/FDN/SDN list
            if (0 != name_ptr->wstr_len && PNULL != name_ptr->wstr_ptr)
            {
                want_mo_call_ptr->is_name_exist = TRUE;
                
                want_mo_call_ptr->name_len = name_ptr->wstr_len;   
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
                copy_len = want_mo_call_ptr->name_len;
#else    
                copy_len = ( want_mo_call_ptr->name_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : want_mo_call_ptr->name_len;/*lint !e506 */
#endif
                MMI_WSTRNCPY(
                    want_mo_call_ptr->name,
                    sizeof(want_mo_call_ptr->name),
                    name_ptr->wstr_ptr,
                    name_ptr->wstr_len,
                    copy_len
                    );
            }
            else
            {
                want_mo_call_ptr->is_name_exist = FALSE;

                want_mo_call_ptr->name_len = tele_len;
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
                copy_len = tele_len;
#else
                copy_len = ( tele_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : tele_len;
#endif
                MMI_STRNTOWSTR(
                    want_mo_call_ptr->name,
                    want_mo_call_ptr->name_len,
                    tele_num,
                    tele_len,
                    copy_len
                    );
                want_mo_call_ptr->name_len = copy_len;
            }
        }
        else
        {
            want_mo_call_ptr->is_save_name = FALSE;
            //查找pb
            b_search_result = GetNameFromPb(want_mo_call_ptr, tele_num, tele_len, &name, &pb_entry_id, &pb_group_id);

#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
            MMIMEX_AQIPPhNum(party_num,&s_call_context,copy_len,tele_num,&tele_len);
#endif
            if (b_search_result && (0 != name.wstr_len))//电话本中存在且名字不为空
            {
                want_mo_call_ptr->is_name_exist = TRUE;
                
        #ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
                copy_len = name.wstr_len;
        #else
                copy_len = ( name.wstr_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : name.wstr_len;
        #endif

                want_mo_call_ptr->name_len = copy_len;
            }
            else
            {
                want_mo_call_ptr->is_name_exist = FALSE;
                
                want_mo_call_ptr->name_len = tele_len;//strlen((char*)tele_num);
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
                copy_len = tele_len;
#else
                copy_len = ( tele_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : tele_len;
#endif
                MMI_STRNTOWSTR(
                    want_mo_call_ptr->name,
                    want_mo_call_ptr->name_len,
                    tele_num,
                    tele_len,
                    copy_len
                    );
                want_mo_call_ptr->name_len = copy_len;
            }
        }
    }

    return TRUE;
}
/*****************************************************************************/
// 	Description : process when received signal MN_SETUP_COMPLETE_CNF
//	Global resource dependence : 
//    Author:yonghua.zhang
//	Note:2020.5.1
/*****************************************************************************/
LOCAL BOOLEAN CC_IsInvalidSetupCompleteCnf(DPARAM param)
{
    APP_MN_SETUP_COMPLETE_CNF_T *pSig = (APP_MN_SETUP_COMPLETE_CNF_T *)param;
    uint8 call_index = CC_INVALID_ID;

    if(PNULL != pSig)
    {
        if(pSig->call_id < CC_MAX_CALL_NUM)
        {
            call_index = CC_GetIndexOfCallId(pSig->dual_sys, pSig->call_id);
            if(CC_IsCallIDValid(call_index))
            {
                if(CC_CALL_STATE == MMICC_GetCallStatusByIndex( call_index)
                    || CC_HOLD_STATE == MMICC_GetCallStatusByIndex( call_index))
                {
                    //通话已经建立，收到setup complete cnf，不做处理
                    SCI_TRACE_LOW("CC_IsInvalidSetupCompleteCnf invalid call_index=%d ",call_index);
                    return TRUE;
                }
                else
                {
                    SCI_TRACE_LOW("CC_IsInvalidSetupCompleteCnf  valid call_index=%d ",call_index);
                    return FALSE;
                }
             }
        }
    }
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2269_112_2_18_2_10_11_54,(uint8*)"");
    return TRUE;
}

/*****************************************************************************/
// 	Description : process when received signal EV_SETUP_COMPLETE_CNF
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_SetupCompleteCnf(DPARAM param)
{
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS		address = {0};
#endif
    
    APP_MN_SETUP_COMPLETE_CNF_T *pSig = (APP_MN_SETUP_COMPLETE_CNF_T *)param;
    
    //check the param
    
    if(PNULL == pSig ||pSig->call_id >= CC_MAX_CALL_NUM)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_SetupCompleteCnf(), pSig = NULL ||pSig->call_id >= 7"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2269_112_2_18_2_10_11_54,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_SetupCompleteCnf(), call_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2272_112_2_18_2_10_11_55,(uint8*)"d", pSig->call_id);
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    s_call_context.dual_sys = pSig->dual_sys;
#endif
    
    //get the call index according to its call id
    s_call_context.current_call = CC_GetIndexOfCallId(pSig->dual_sys, pSig->call_id);

    if(!CC_IsCallIDValid(s_call_context.current_call))
    {
        //SCI_TRACE_LOW:"ccapp.c:CC_SetupCompleteCnf() current_call invalid %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2282_112_2_18_2_10_11_56,(uint8*)"d", s_call_context.current_call);
        return;
    }
    
    if (pSig->connected_num_present && pSig->connected_num.num_len && MN_PRESENT_RESTRICT != pSig->connected_num.present_ind)
    {
        if (pSig->connected_num.num_len != s_call_context.call_info[s_call_context.current_call].address.num_len
            || pSig->connected_num.number_type != s_call_context.call_info[s_call_context.current_call].address.number_type
            || pSig->connected_num.number_plan != s_call_context.call_info[s_call_context.current_call].address.number_plan
            || 0 != memcmp(pSig->connected_num.party_num, (uint8 *)s_call_context.call_info[s_call_context.current_call].address.party_num, pSig->connected_num.num_len))
        {
            CC_WANT_MO_CALL_T want_mo_call = {0};
            uint8 tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
            uint16 tel_num_len = 0;
            uint16 copy_len = 0;

            //保存原始号码
            s_call_context.call_info[s_call_context.current_call].is_call_modified_store = TRUE;

            SCI_MEMSET(&s_call_context.call_info[s_call_context.current_call].call_modified_address, 0x00, sizeof(s_call_context.call_info[s_call_context.current_call].call_modified_address));

            SCI_MEMCPY(&s_call_context.call_info[s_call_context.current_call].call_modified_address,
                                &s_call_context.call_info[s_call_context.current_call].address,
                                sizeof(s_call_context.call_info[s_call_context.current_call].call_modified_address));

            tel_num_len = MMIAPICOM_GenDispNumber(pSig->connected_num.number_type, 
                                                                                (uint8)pSig->connected_num.num_len, 
                                                                                (uint8 *)pSig->connected_num.party_num,
                                                                                tel_num,
                                                                                sizeof(tel_num)
                                                                                );

            GetWantMoCallInfo(s_call_context.dual_sys,
                                        tel_num,
                                        tel_num_len,
                                        PNULL,
                                        PNULL,
                                        PNULL,
                                        &want_mo_call
                                        );

            copy_len = (want_mo_call.name_len > CC_MAX_PBNAME_LEN) ? CC_MAX_PBNAME_LEN : want_mo_call.name_len;/*lint !e506 */

            s_call_context.call_info[s_call_context.current_call].is_name_exist = want_mo_call.is_name_exist;

            MMI_WSTRNCPY(
                s_call_context.call_info[s_call_context.current_call].name,
                sizeof(s_call_context.call_info[s_call_context.current_call].name),
                want_mo_call.name,
                sizeof(want_mo_call.name),
                copy_len
                );

            s_call_context.call_info[s_call_context.current_call].name_len = copy_len;
            s_call_context.call_info[s_call_context.current_call].address = want_mo_call.address;
        }
    }

    //vibrate for call connect prompt
    MMICC_VibrateForConnectPrompt();

    HandleMoMuteAndLocalRingState(APP_MN_SETUP_COMPLETE_CNF);	
    
    //update the global variable:s_call_context
    s_call_context.exist_call.exist_outgoing_call = FALSE;
    SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call));
    s_call_context.want_mo_call.is_want_mo_call = FALSE;
    
    s_call_context.exist_call.exist_active_call_num++;
    s_call_context.call_info[s_call_context.current_call].call_state = CC_CALL_STATE;
    s_call_context.call_info[s_call_context.current_call ].operated_by_user = CC_NO_OPER;
    s_call_context.call_info[s_call_context.current_call].call_type = CC_MO_CALL_TYPE;
    s_call_context.call_info[s_call_context.current_call].call_time.start_time = MMIAPICOM_GetCurTime( );
    s_call_context.call_info[s_call_context.current_call].dual_sys = pSig->dual_sys;

#ifdef BLUETOOTH_SUPPORT
    //判断是否有激活的蓝牙耳机	
    MMIAPIBT_CallConnect(&address);
#ifdef BT_BQB_SUPPORT
    MMIAPIBT_SetCallHeldStatus();
#endif
#endif
    
    //open speaker and microphone
    //CC_OpenAudioTunnel(TRUE);
   
#ifdef DPHONE_SUPPORT
 /*Begin of yanqi on 2010-Mar-26 for 来电时，免提没有回铃音*/
	if (!MMIDEFAULT_GetHookStates())
    {
       //SCI_TRACE_LOW:"---WD:CC_SetupCompleteCnf deal with hook"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2313_112_2_18_2_10_11_57,(uint8*)"");
       MMIDEFAULT_SetHandFreeStatesAndLed(TRUE);
    }
 /*End of yanqi on 2010-Mar-26*/
#endif
//    MMICC_EnableMute(FALSE);//cr228324
    EnableSpeakerMode(TRUE);

    //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_RESUME, MMICC_AUDIO_DEV_MAX);
	
    //reset call time if only one call
    if (1 == s_call_context.call_number) 
    {
        s_call_context.call_time_count = 0;
    }
    s_call_context.call_info[s_call_context.current_call].start_call_time_count = s_call_context.call_time_count;

    MMICC_UpdateCallStatusDisplay(MMICC_MO_CONNECTED_IND);
    MMIICC_StopDtmfTimerId();
    
    if (s_call_context.dtmf_state.is_dtmf)
    {
        //start timer
        MMIICC_StartDtmfTimerId();
        s_current_dtmf_call_id = pSig->call_id;
    }
    
#ifdef DPHONE_SUPPORT
    ResetCallHandfreeState();  //Modified by yanqi.2010-Apr-13 for 未接通前回铃音等状态不对
#endif
#ifdef MMI_VOLTE_SUPPORT
    SCI_TRACE_LOW("volte ccapp.c: CC_SetupCompleteCnf  is_ip_call: %d ,s_call_context.pscall_info.is_volte_codec_on=%d",pSig->is_ip_call,s_call_context.pscall_info.is_volte_codec_on);
    if(pSig->is_ip_call)
    {
        s_call_context.pscall_info.is_ps_call = TRUE;
        if(!s_call_context.pscall_info.is_volte_codec_on)
        {
            CC_EnableVoLTECodec(TRUE);
        }
    }
#endif

}

/*****************************************************************************/
// 	Description : process when user hang up the call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseCallReq(MN_DUAL_SYS_E dual_sys, uint8 call_index,MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    
    //check the param
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_ReleaseCallReq(), dual_sys = %d, call_id = %d, cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2355_112_2_18_2_10_11_58,(uint8*)"ddd",dual_sys, call_index, cause);
    //SCI_TRACE_LOW("ccapp.c  CC_ReleaseCallReq disconnected cause = %d", cause);

#ifdef MMI_INDIAN_SOS_SUPPORT
	if(CC_IsSosCalling() && (s_call_context.call_info[call_index].call_id == CC_GetSosCallId())){
		CC_TerminateSosCall(TRUE);
	}
#endif
    
    if( call_index >= CC_MAX_CALL_NUM || s_call_context.call_info[call_index].call_id >= CC_MAX_CALL_NUM)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_ReleaseCallReq , call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2360_112_2_18_2_10_11_59,(uint8*)"d", call_index);
        return CC_RESULT_ERROR;
    }
    
    //get the call index according to its call id
    s_call_context.current_call = call_index;
    if(!CC_IsCallIDValid(s_call_context.current_call))
    {
        //SCI_TRACE_LOW:"ccapp.c:CC_ReleaseCallReq() current_call invalid %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2368_112_2_18_2_10_11_60,(uint8*)"d",s_call_context.current_call);
        return CC_RESULT_ERROR;
    }
    
    //call function of MN to disconnect call
    if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(s_call_context.dual_sys, s_call_context.call_info[call_index].call_id, CC_ResetDisconnectCause(cause), PNULL))
    {
        //SCI_TRACE_LOW:"ccapp.c:CC_ReleaseCallReq() MNCALL_DiscCall return failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2375_112_2_18_2_10_11_61,(uint8*)"");
        return CC_RESULT_ERROR;
    }
    
    s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_RELEASE_OPER;
    //update the prompt in the cc window
    MMICC_UpdateCurrentCallProcessing();    //call processing
    return CC_RESULT_SUCCESS; 
}


/*****************************************************************************/
// 	Description : process when user hang up the call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseCallReq(MN_DUAL_SYS_E dual_sys, uint8 call_index,MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    return CC_ReleaseCallReq(dual_sys, call_index, cause);
}

/*****************************************************************************/
// 	Description : Get called info from the called number
//	Global resource dependence : 
//  Author:lin.lin
//	Note: add ip number parse
/*****************************************************************************/
LOCAL void GetCallInfoNumber(
                             MMICL_CALL_INFO_T		*cl_call_info_ptr,
                             MN_CALLED_NUMBER_T      *called_number_ptr
                             )
{
    uint8 tele_number[CC_MAX_TELE_NUM_LEN + 2] = {0};
    MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
    uint8 max_tele_len  = CC_MAX_TELE_NUM_LEN + 2;
    
    SCI_MEMSET(&party_num, 0, sizeof(MMI_PARTY_NUMBER_T));
    MMIAPICOM_GenDispNumber(called_number_ptr->number_type, 
        (uint8)called_number_ptr->num_len, 
        called_number_ptr->party_num, 
        tele_number, 
        max_tele_len);
    
    MMI_MEMCPY(
        &cl_call_info_ptr->call_num.number,
        sizeof(cl_call_info_ptr->call_num.number),
        called_number_ptr,
        sizeof(MN_CALLED_NUMBER_T),
        sizeof(MN_CALLED_NUMBER_T)
        );
    
    cl_call_info_ptr->dual_sys = s_call_context.dual_sys;    
}

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_CALL_FORWARD_IND
//	Global resource dependence : none
//  Author:Maoxy
//	Note:
/*****************************************************************************/
LOCAL void HandleCallForwardInd( 
                                DPARAM param 
                                )
{
    APP_MN_CALL_FORWARD_IND_T *pSig = (APP_MN_CALL_FORWARD_IND_T*)param;
    MMI_TEXT_ID_T  forwared_type_textid = TXT_NULL;
    uint8 call_index = 0;
    //check the param
    
    if(PNULL == pSig ||pSig->call_id >= CC_MAX_CALL_NUM)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function HandleCallForwardInd(), pSig = NULL ||pSig->call_id >= 7"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2441_112_2_18_2_10_11_62,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp.c:enter function HandleCallForwardInd(), call_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2444_112_2_18_2_10_11_63,(uint8*)"d", pSig->call_id);
    call_index = CC_GetIndexOfCallId(pSig->dual_sys, pSig->call_id);

    if(s_call_context.call_info[call_index].is_mtfw_reject)
    {
        //SCI_TRACE_LOW:"ccapp.c:HandleCallForwardInd() mtfw"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2449_112_2_18_2_10_11_64,(uint8*)"");
        return;
    }

    switch (pSig->forward_type)
    {
    case MN_SS_OUTGOING_FORWARED_TO_C:
        forwared_type_textid= TXT_OUTGOING_FORWARED_TO_C;
        break;
        
    case MN_SS_INCOMING_FORWARED_TO_C:
        forwared_type_textid= TXT_INCOMING_FORWARED_TO_C;
        break;
        
    case MN_SS_INCOMING_IS_FORWARED:
        forwared_type_textid= TXT_INCOMING_IS_FORWARED;
        break;
        
    default:
        break;
    }

    if (TXT_NULL != forwared_type_textid)    
    {
        MMICC_OpenAlertWarningWin(forwared_type_textid);  
    }
}

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_FORWARD_STATUS_IND
//	Global resource dependence : none
//  Author:Maoxy
//	Note:
/*****************************************************************************/
LOCAL void HandleForwardStatusInd( 
                                  DPARAM param 
                                  )
{
    APP_MN_FORWARD_STATUS_IND_T *pSig = (APP_MN_FORWARD_STATUS_IND_T *)param;
    MMI_TEXT_ID_T  status_ind_textid = TXT_NULL;
    uint8 call_index = 0;
    
    if(PNULL == pSig ||pSig->dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function HandleForwardStatusInd(), pSig = NULL ||pSig->call_id >= MMI_DUAL_SYS_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2491_112_2_18_2_10_11_65,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp: HandleForwardStatusInd oper: %d stat: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2494_112_2_18_2_10_11_66,(uint8*)"dd",pSig->forward_operation, pSig->forward_status);
    call_index = CC_GetIndexOfCallId(pSig->dual_sys, pSig->call_id);

    if(s_call_context.call_info[call_index].is_mtfw_reject)
    {
        //SCI_TRACE_LOW:"ccapp.c:HandleCallForwardInd() mtfw"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2499_112_2_18_2_10_11_67,(uint8*)"");
        return;
    }    

    if (MN_SS_ACTIVE_OPERATIVE == pSig->forward_status)
    {
        switch(pSig->forward_operation) 
        {
        case MN_SS_CFU:
            status_ind_textid = TXT_CC_CFU_STARTUP;
            break;
        case MN_SS_ALL_COND_FORWARD:
        case MN_SS_CFB:
        case MN_SS_CFNRY:
        case MN_SS_CFNRC:
            status_ind_textid = TXT_CC_CFC_STARTUP;
            break;
            
        default:
            
            break;
        }
    }

    if (TXT_NULL != status_ind_textid)
    {
        MMICC_OpenAlertWarningWin(status_ind_textid);  
    }
}

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_ACMMAX_IND
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleAcmMaxInd(DPARAM param)
{
    uint32 timer_period = MMI_10MIN;
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

    MMIDEFAULT_TurnOnBackLight();

    if ( MMK_IsOpenWin( MMIPUB_ALERT_WIN_ID ) )
    {
        MMIPUB_CloseAlertWin();
    }

    MMIPUB_OpenAlertWinByTextId(&timer_period,TXT_CC_COST_EXCEED_LIMIT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);
}

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_CALL_HELD_IND
//	Global resource dependence : none
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void HandleRemoteHoldCallInd(DPARAM param)
{
    MMIDEFAULT_TurnOnBackLight();

    if ( MMK_IsOpenWin( MMIPUB_ALERT_WIN_ID ) )
    {
        MMIPUB_CloseAlertWin();
    }
    //MMICC_OpenAlertWarningWin(TXT_CC_REMOTE_HOLD_IND);
}

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_CALL_RETRIEVED_IND
//	Global resource dependence : none
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void HandleRemoteRetrievedCallInd(DPARAM param)
{
    MMIDEFAULT_TurnOnBackLight();

    if ( MMK_IsOpenWin( MMIPUB_ALERT_WIN_ID ) )
    {
        MMIPUB_CloseAlertWin();
    }
    //MMICC_OpenAlertWarningWin(TXT_CC_REMOTE_RETRIEVED_IND);
}

/*****************************************************************************/
// 	Description : to handle the singal APP_MN_BUILD_MPTY_IND
//	Global resource dependence : none
//	Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL void HandleRemoteBuildMPTYInd( DPARAM param)
{
    MMIDEFAULT_TurnOnBackLight();

    if ( MMK_IsOpenWin( MMIPUB_ALERT_WIN_ID ) )
    {
        MMIPUB_CloseAlertWin();
    }
    MMICC_OpenAlertWarningWin(TXT_CC_REMOTE_BUILD_MPTY);
}

/*****************************************************************************/
// 	Description : process when received signal APP_MN_CALL_DISCONNECTED_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01 modify by jibin 2004.10.16
/*****************************************************************************/
LOCAL void CC_DisconnectedCall(uint16 msg_id, DPARAM param)
{
    uint8					call_id = CC_INVALID_ID;
    uint8					call_index = CC_INVALID_INDEX;
    MN_CALL_DISCONNECT_CALL_CAUSE_E    cause = MN_CAUSE_UNASSIGNED_NO;
    CC_OPERATED_BY_USER_E operated_by_user = CC_NO_OPER;
    MN_DUAL_SYS_E           dual_sys = MN_DUAL_SYS_MAX;
    
    //check the param
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_DisconnectedCall()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2605_112_2_18_2_10_12_68,(uint8*)"");
    if(PNULL== param)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_DisconnectedCall() param == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2608_112_2_18_2_10_12_69,(uint8*)"");
        return;
    }
    
    if(APP_MN_CALL_DISCONNECTED_IND == msg_id)
    {
        dual_sys = ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->dual_sys;
        call_id = ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->call_id;
        cause = ((APP_MN_CALL_DISCONNECTED_IND_T*)param)->disconnected_cause;
//============== MET MEX start ===================        
#ifdef MET_MEX_SUPPORT        
        MMIMEX_PhsOutCallDroped((void*)NULL);
#endif
//============== MET MEX end ===================    
        //SCI_TRACE_LOW:"ccapp.c  disconnected call_id = %d, cause = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2622_112_2_18_2_10_12_70,(uint8*)"dd", call_id, cause);
    }
    
    if(APP_MN_CALL_ERR_IND == msg_id)
    {
        dual_sys = ((APP_MN_CALL_ERR_IND_T*)param)->dual_sys;
        call_id = ((APP_MN_CALL_ERR_IND_T*)param)->call_id;
        cause = ((APP_MN_CALL_ERR_IND_T*)param)->error_cause;
//============== MET MEX start ===================
#ifdef MET_MEX_SUPPORT
		MMIMEX_PhsOutCallEventCB((void *)NULL);
#endif    
//============== MET MEX end ===================    
        //SCI_TRACE_LOW:"APP_MN_CALL_ERR_IND:call_id = %d, cause = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2630_112_2_18_2_10_12_71,(uint8*)"dd", call_id, cause);
    }
    
    //cr133748
    s_mt_need_open_codec_flag = 0; //重置
    //StopLocalNetRing();  //断开电话的时候，关闭本地回铃音

    HandleMoMuteAndLocalRingState(msg_id);
    
    if(  call_id >= CC_MAX_CALL_NUM )
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_DisconnectedCall() , call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2641_112_2_18_2_10_12_72,(uint8*)"d", call_id);

        if ((CC_INVALID_ID == call_id)
            && (MN_CAUSE_SIM_STK_NOT_ALLOWED == cause)
            && MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
            )//call control fail
        {
            MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

            MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
#if !defined(MMI_GUI_STYLE_TYPICAL)
            MMK_CloseWin(MMICC_STATUS_WIN_ID);
#endif
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_CALL_CONTROL_NOT_ALLOW,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,PNULL);

            MMIAPISTK_SetupCall(dual_sys, FALSE, FALSE, cause);
        }

        return;
    }
    
    //get the call index according to its call id
    call_index = CC_GetIndexOfCallId(dual_sys, call_id);

#if 0 //222299,225417
#ifdef BT_DIALER_SUPPORT
    if(MMICC_GetBtCallFlag())
    {
        s_call_context.current_call = call_index;
    }
#endif
#endif

    operated_by_user = s_call_context.call_info[call_index].operated_by_user;
    
    if(  (s_call_context.exist_call.exist_incoming_call) &&  (s_call_context.incoming_call_index  == call_index ))
    {
        MMIAPICC_StopRingOrVibrate();
    }
    
    if(!CC_IsCallIDValid(s_call_context.current_call))
    {
        //SCI_TRACE_LOW:"ccapp.c ERROR:CC_DisconnectedCall() current_call invalid %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2672_112_2_18_2_10_12_73,(uint8*)"d", s_call_context.current_call);
        MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
        return;
    }
    
    MMIDEFAULT_TurnOnBackLight();

    CC_DisconnectedCallByIndex(call_index, cause);
    
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetCallHeldStatus();
#endif

//lzk mark
#ifdef BT_DIALER_SUPPORT
/*    MMIAPISMS_OpenDelWindow();*/ //btdialer ok
#else
    //MMIAPISMS_OpenDelWindow();
#endif
    if (CC_RELEASE_OPER == operated_by_user
        && APP_MN_CALL_DISCONNECTED_IND == msg_id)
    {
        MMIAPISTK_SetupCall(dual_sys, TRUE, FALSE, cause);
    }
    else
    {
        MMIAPISTK_SetupCall(dual_sys, FALSE, TRUE, cause);
    }

#ifdef DPHONE_SUPPORT
    if (MMIDEFAULT_GetHandFreeStates() && !MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        MMIDEFAULT_SetHandFreeStatesAndLed(FALSE);
        MMIDEFAULT_SetToneStates(FALSE); 
    }
#endif
}

/*****************************************************************************/
// 	Description : disconnected call by index
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void CC_DisconnectedCallByIndex(
                                      uint8 call_index,
                                      MN_CALL_DISCONNECT_CALL_CAUSE_E cause
                                      )
{
    MMICL_CALL_INFO_T		cl_info = {0};/*lint !e64*/
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    BOOLEAN is_need_answer = FALSE;
    BOOLEAN is_need_answer_sms = FALSE;
#endif
    BOOLEAN is_stk_call = FALSE;
    uint8			        tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8			        tel_num_len = 0;
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS		address={0};
#endif    
    MMICL_CALL_TYPE_E calllog_type = MMICL_CALL_DIALED;

    if (!CC_IsCallIDValid(call_index))
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_DisconnectedCallByIndex() invalid index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2731_112_2_18_2_10_12_74,(uint8*)"d", call_index);
        return;
    }

    switch(s_call_context.call_info[call_index].call_state)
    {
    case CC_INIT_STATE:
        break;
        
    case CC_CALLING_STATE:
        s_call_context.exist_call.exist_outgoing_call = FALSE;
        break;
        
    case CC_CALL_STATE:
        s_call_context.exist_call.exist_active_call_num--;
        break;
        
    case CC_INCOMING_CALL_STATE:
    case CC_WAITING_STATE:
        s_call_context.exist_call.exist_incoming_call = FALSE;
        s_call_context.incoming_call_index = CC_INVALID_INDEX;
        
        //if mt call is released, the operate for waiting call is invalid
        if (OTHER_OPERATE != s_call_context.operate_waiting_call)
        {
            s_call_context.operate_waiting_call = OTHER_OPERATE;
        }
        
        //need answer sms after rejecting call
        //if (s_call_context.call_number <= 1)
        {
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
            is_need_answer = TRUE;
#endif
            tel_num_len = MMIAPICOM_GenNetDispNumber(
                s_call_context.call_info[call_index].address.number_type, 
                (uint8)s_call_context.call_info[call_index].address.num_len, 
                s_call_context.call_info[call_index].address.party_num,
                tel_num,
                sizeof(tel_num)
                );
        }
        break;
        
    case CC_HOLD_STATE:
        s_call_context.exist_call.exist_hold_call_num--;
        break;
        
    default:
        //SCI_TRACE_LOW:"ccapp.c:CC_DisconnectedCallByIndex() call_state = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2780_112_2_18_2_10_12_75,(uint8*)"d", s_call_context.call_info[s_call_context.current_call].call_state);
        break;
        
    }
#ifdef BLUETOOTH_SUPPORT    
    //#ifdef MMI_BLUETOOTH
    //判断是否有激活的蓝牙耳机	
    if (MMIAPIBT_GetActivatedHS(&address)
        )
    {
        uint32 on_bt_call_num = s_call_context.exist_call.exist_active_call_num +
                               s_call_context.exist_call.exist_hold_call_num +
                               s_call_context.exist_call.exist_outgoing_call;
    
        switch(s_call_context.call_info[call_index].call_state)
        {
        case CC_INIT_STATE:
            break;
            
        case CC_CALLING_STATE:
            //SCI_TRACE_LOW:"CC_DisconnectedCallByIndex :Call the function: BT_StopAlert"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2800_112_2_18_2_10_12_76,(uint8*)"");
            //cr143855 临时：如果存在呼出电话，通话中的电话挂断的时候不会断开蓝牙
            MMIAPIBT_StopAlert();
            break;
            
        case CC_CALL_STATE:
        case CC_HOLD_STATE:

            break;
            
        case CC_INCOMING_CALL_STATE:
        case CC_WAITING_STATE:
            //SCI_TRACE_LOW:"CC_DisconnectedCallByIndex :Call the function: BT_StopRing"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2812_112_2_18_2_10_12_77,(uint8*)"");
            MMIAPIBT_StopRing();
            break;     

            
        default:
            //SCI_TRACE_LOW:"ccapp.c:CC_DisconnectedCallByIndex() call_state = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2818_112_2_18_2_10_12_78,(uint8*)"d", s_call_context.call_info[s_call_context.current_call].call_state);
            break;
            
        }

        if (0 == on_bt_call_num)
        {
            //SCI_TRACE_LOW:"CC_DisconnectedCallByIndex :Call the function: BT_CallDisconnected"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_2825_112_2_18_2_10_12_79,(uint8*)"");
            MMIAPIBT_CallDisconnect();
        }
    }
//     MMIAPIBT_RecoveryBtStatus(MMIBT_MODULE_CC);
    //#endif
#endif//BLUETOOTH_SUPPORT
    //记录call log
    //处理窗口前应先对通话结束的时间赋值,否则通话时间计算有误
    if (CC_INCOMING_CALL_STATE == s_call_context.call_info[call_index].call_state
        ||CC_WAITING_STATE == s_call_context.call_info[call_index].call_state
        ||CC_CALLING_STATE == s_call_context.call_info[call_index].call_state
        )
    {
        //未接通
        s_call_context.call_info[call_index].call_time.stop_time
            = s_call_context.call_info[call_index].call_time.start_time;
    }
    else
    {
        s_call_context.call_info[ call_index ].call_time.stop_time = MMIAPICOM_GetCurTime(); 
    }

    //处理窗口 begin    
    MMICC_UpdateCallStatusDisplay(MMICC_DISCONNECTED_IND);

    //非本方挂断,需要发挂断音提示用户
    if (CC_RELEASE_OPER != MMICC_GetCallOperatedByIndex(MMICC_GetCurrentCallIndex())
        && CC_INCOMING_CALL_STATE != MMICC_GetCallStatusByIndex(MMICC_GetCurrentCallIndex()))
    {
        //启动定时器，开始通话挂断音提示，CC_PLAY_CALL_END_HINT_TIME指定提示音次数
        s_cc_call_end_hint_timer = MMK_CreateTimerCallback(MMI_100MSECONDS, CC_PlayCallEndHint, CC_WATCH_PLAY_CALL_END_HINT_TIME, FALSE);
    }
    else
    {
        //destory audio handle
        if(PNULL != MMICC_GetAppletInstance())
        {
            //stop cc applet
            MMICC_DestroyAudioHandle();
            MMK_PostMsg(MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
        }
    }

    if (!s_call_context.call_info[call_index].is_release_all)    //不是挂断所有电话
    {
        if (RELEASE_ALL_ACTIVE_AND_ACCEPT != s_call_context.operate_waiting_call
            &&((RELEASE_ALL_ACTIVE_AND_ACCEPT_WAIT != s_call_context.operate_CRSS_code)
                &&(RELEASE_ALL_ACTIVE_AND_RETRIEVE_HOLD != s_call_context.operate_CRSS_code)))
        {
            MN_CALL_DISCONNECT_CALL_CAUSE_E set_cause = 0;
            //uint32 call_time = MMIAPICL_GetCallDurationTime(s_call_context.call_info[call_index].call_time.start_time,
            //                s_call_context.call_info[call_index].call_time.stop_time);//s_call_context.call_info[call_index].call_time.stop_time - s_call_context.call_info[call_index].call_time.start_time;

            //bt dialer ok
            if(0 != s_disc_info_cause)
            {
                set_cause = (MN_CALL_DISCONNECT_CALL_CAUSE_E)((0 != s_disc_info_cause)?s_disc_info_cause:cause);
            }
            //display disconnect window
            //MMICC_OpenDisconnectWin(call_index, set_cause, call_time);
        }
        else
        {
            SCI_TRACE_LOW("ccapp not open disconnect win: %d, %d", s_call_context.operate_waiting_call, s_call_context.operate_CRSS_code);
        }
        
        s_disc_info_cause = 0;
        
        if ( 0 == s_call_context.exist_call.exist_active_call_num + s_call_context.exist_call.exist_hold_call_num
            &&((RELEASE_ALL_ACTIVE_AND_ACCEPT_WAIT != s_call_context.operate_CRSS_code)
                &&(RELEASE_ALL_ACTIVE_AND_RETRIEVE_HOLD != s_call_context.operate_CRSS_code)))
        {

            //停止录音
            if (!MMIAPIENG_GetIQModeStatus())
            {
                MMIAPICC_StopCCRecord(); //挂断的是后停止录音
            }

            MMK_CloseWin( MMICC_CALLLIST_WIN_ID );
            MMK_CloseWin( MMICC_MENU_WIN_ID );
            MMK_CloseWin( MMICC_HOLDMENU_WIN_ID );
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
            {
                MMK_CloseWin( MMICC_STATUS_WIN_ID );
            }
            MMIAPIIDLE_CloseDialMenuWin();
        }
    }    
    else    //挂断所有电话的一通
    {
        if (CC_RELEASE_ALL_NOPROMPT != s_call_context.call_info[call_index].release_all_type)        
        {
            MN_CALL_DISCONNECT_CALL_CAUSE_E set_cause = 0;
            //uint32 call_time = MMIAPICL_GetCallDurationTime(s_call_context.call_info[call_index].call_time.start_time,
            //    s_call_context.call_info[call_index].call_time.stop_time);//s_call_context.call_info[call_index].call_time.stop_time - s_call_context.call_info[call_index].call_time.start_time;

            if(0 != s_disc_info_cause)
            {
                set_cause = (MN_CALL_DISCONNECT_CALL_CAUSE_E)((0 != s_disc_info_cause)?s_disc_info_cause:cause);
            }
            //display disconnect window
            //MMICC_OpenDisconnectWin(call_index, set_cause, call_time);
        }            

            s_disc_info_cause = 0;
        
        //挂断所有电话的最后一通
        if (0 == (s_call_context.exist_call.exist_active_call_num + 
            s_call_context.exist_call.exist_hold_call_num))
        {

            //停止录音
            if (!MMIAPIENG_GetIQModeStatus())
            {
                MMIAPICC_StopCCRecord();
            }
            
            
            if (CC_RELEASE_ALL_PROMPT == s_call_context.call_info[call_index].release_all_type)
            {      
                if (MMK_IsOpenWin( MMICC_DISCONNECT_WIN_ID ))
                {
                    //MMK_CloseMiddleWin(MAIN_IDLE_WIN_ID, MMICC_DISCONNECT_WIN_ID);
                }
                else
                {
                    MMK_ReturnIdleWin();
                }
            }
            //关闭所有call窗口
            EnableSpeakerMode(TRUE);
            
#ifdef BT_DIALER_SUPPORT
            if(!MMICC_GetBtCallFlag())
#endif
{
            MMK_CloseWin(MMICC_STATUS_WIN_ID);
}
            MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
            MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
            MMK_CloseWin(MMICC_HOLDMENU_WIN_ID);
            MMIAPIIDLE_CloseDialMenuWin();
            
        }
    }
    if((CC_INCOMING_CALL_STATE == s_call_context.call_info[call_index].call_state)
        ||(CC_WAITING_STATE == s_call_context.call_info[call_index].call_state))
    {
        MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
    }


    if (0 == (s_call_context.exist_call.exist_active_call_num + 
            s_call_context.exist_call.exist_hold_call_num))
    {
        MMICC_SetDtmfSwitch(TRUE);
    }

	//close dtmf sending win
    if (s_call_context.call_info[call_index].call_id == s_current_dtmf_call_id)
    {
        CC_ClearStoredDtmf();
        MMK_CloseWin(WATCHCC_SEND_DTMF_NOTE_WIN_ID);// fix bug1379767 
        MMK_CloseWin(WATCHCC_SEND_DTMF_QUERY_WIN_ID);
    }
        
    SCI_MEMSET(&cl_info, 0, sizeof(cl_info));
    
    //s_call_context.call_info[ call_index ].call_time.stop_time = MMIAPICOM_GetCurTime();
    
    MMI_MEMCPY(
        &cl_info.call_time,
        sizeof(cl_info.call_time),
        &s_call_context.call_info[call_index].call_time,
        sizeof(s_call_context.call_info[call_index].call_time),
        sizeof(CC_CALL_TIME_T)
        );
    
    if( s_call_context.call_info[call_index].name_len > 0 )
    {
        cl_info.call_num.is_num_present = TRUE;
    }
    else
    {
        cl_info.call_num.is_num_present = FALSE;
    }
    
#if defined(CL_SAVE_NAME_SUPPORT) 
    cl_info.name.is_use_nv_name = s_call_context.call_info[call_index].is_save_name;
    
    if (s_call_context.call_info[call_index].is_save_name && s_call_context.call_info[call_index].is_name_exist)
    {
        cl_info.name.wstr_len = MIN(s_call_context.call_info[call_index].name_len, MMICL_NAME_MAX_LEN);
        MMI_WSTRNCPY(
            cl_info.name.wstr,
            MMICL_NAME_MAX_LEN,
            s_call_context.call_info[call_index].name,
            CC_MAX_PBNAME_LEN,
            cl_info.name.wstr_len
            );  
    }
#endif

    s_is_call_modified_store = FALSE;
    
    //for ip call
    if (s_call_context.call_info[call_index].is_call_modified_store)
    {
        s_call_context.call_info[call_index].is_call_modified_store = FALSE;

        GetCallInfoNumber(&cl_info, &s_call_context.call_info[call_index].call_modified_address);

        //是否是紧急呼叫
        if (CC_MO_CALL_TYPE == s_call_context.call_info[call_index].call_type)
        {
            tel_num_len = MMIAPICOM_GenDispNumber(
                s_call_context.call_info[call_index].call_modified_address.number_type, 
                (uint8)s_call_context.call_info[call_index].call_modified_address.num_len, 
                s_call_context.call_info[call_index].call_modified_address.party_num,
                tel_num,
                sizeof(tel_num)
                );
            cl_info.is_emergency_call = MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, s_call_context.dual_sys, FALSE);
        }
    }
    else
    {
        GetCallInfoNumber(&cl_info, &s_call_context.call_info[call_index].address);

        //是否是紧急呼叫
        if (CC_MO_CALL_TYPE == s_call_context.call_info[call_index].call_type)
        {
            tel_num_len = MMIAPICOM_GenDispNumber(
                s_call_context.call_info[call_index].address.number_type, 
                (uint8)s_call_context.call_info[call_index].address.num_len, 
                s_call_context.call_info[call_index].address.party_num,
                tel_num,
                sizeof(tel_num)
                );
            //bug 186648 modify is_only_for_display(the last parameter of MMIAPICC_IsEmergencyNum) true to false,is_only_for_display is setted true is to some requirement,
            //but in different countries in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num(no sim emergency) ,
            //it is not normal display emergency call in window            
            cl_info.is_emergency_call = MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, s_call_context.dual_sys, FALSE);
        }
    }
    
#ifdef MMI_CSP_SUPPORT
    if(!cl_info.is_emergency_call)
    {
        cl_info.line = s_call_context.call_info[call_index].line;
    }
#endif
    
    switch(  s_call_context.call_info[ call_index ].call_type )
    {
    case CC_MO_CALL_TYPE:
        // 已拨电话//@samboo 建议将电话纪录单独提取出来，在disconnect_ind收到后处理，而不是分布在各个地方进行处理
        //call the interface of calllog  
        calllog_type = MMICL_CALL_DIALED;
        is_stk_call = s_call_context.call_info[call_index].is_stk_call;
        break;
        
    case CC_MT_CALL_TYPE:
        //SCI_TRACE_LOW:"mmiccapp:CC_DisconnectedCallByIndex,operated_by_user = %d, call_state = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3045_112_2_18_2_10_13_80,(uint8*)"dd", s_call_context.call_info[ call_index ].operated_by_user, s_call_context.call_info[call_index].call_state);
        if (((CC_NO_OPER == s_call_context.call_info[ call_index ].operated_by_user) &&
            ((CC_INCOMING_CALL_STATE == s_call_context.call_info[call_index].call_state) ||
            (CC_WAITING_STATE == s_call_context.call_info[call_index].call_state)))
            )
        {
            //未接来电
            calllog_type = MMICL_CALL_MISSED;

            s_call_context.missed_call_num++;
            MAIN_SetMissedCallState(TRUE);
#ifdef PDA_UI_DROPDOWN_WIN
            MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_MISS_CALL);
#endif
            
            MMIAPICC_SaveMissedCallNumToNv();
            s_is_new_missed_call = TRUE;
        }
        else
        {
            //已接电话
            calllog_type = MMICL_CALL_RECEIVED;
#ifdef ASP_SUPPORT
            if(call_index == s_asp_call_id)
            {
                if(MMIAPIASP_GetAspRecordTimeOutState())   //答录时间到自动挂断
                {
                    MMIAPIASP_SetAspRecordTimeOutState(FALSE);
                    //未接来电
                    calllog_type = MMICL_CALL_MISSED;
                    
                    s_call_context.missed_call_num++;
                    MAIN_SetMissedCallState(TRUE);
                    MMIAPICC_SaveMissedCallNumToNv();
                    s_is_new_missed_call = TRUE;
                }
                else if((s_call_context.call_info[call_index].operated_by_user == CC_RELEASE_OPER))
                {
                    //用户挂断
                }
                else if(!MMIAPIASP_lsCallSpeakedByOwner())
                {
                    //未接来电
                    calllog_type = MMICL_CALL_MISSED;
                    
                    s_call_context.missed_call_num++;
                    MAIN_SetMissedCallState(TRUE);
                    MMIAPICC_SaveMissedCallNumToNv();
                    s_is_new_missed_call = TRUE;
                }
#ifdef PDA_UI_DROPDOWN_WIN
				if( s_is_new_missed_call )
				{
					MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_MISS_CALL);
				}
#endif
            }
#endif

        }
        break;
        
    default:
        break;
        
    }    

#ifdef ASP_SUPPORT
    if(call_index == s_asp_call_id)
    {
        MMIAPIASP_RealseCall();
    }
#endif

    //设置状态
#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    {
        MMISET_ANYKEY_REJECT_SET_T  reject_mode = MMIAPISET_GetAnykeyRejectMode();

        if (reject_mode.is_sms_reply_open)
        {                
            if (is_need_answer && (CC_RELEASE_OPER == s_call_context.call_info[call_index].operated_by_user))
            {
                is_need_answer_sms = TRUE;
            }
            else
            {
                is_need_answer_sms = FALSE;
            }
        }
    }
#endif


    CC_HandleDisconnectedInMPTY( call_index, cause );    

    //MMICC_HandleDownlinkChanel();//因为307项目只支持单通电话，这个接口没有意义，还会引起bug1668325。
    
    if (!is_stk_call)//stk call need not save to call log
    {
        MMIAPICL_RecordCallInfo(s_call_context.dual_sys, calllog_type, cl_info );
#ifdef ADULT_WATCH_SUPPORT
        {
            MMIEVENT_INFO_T event_info = {0};
            uint32 cl_type = MMIEVENT_CL_MISSED_CALL;

            event_info.event_type = CL_UPDATE_EVENT;

#ifdef DROPDOWN_NOTIFY_SHORTCUT
            if(MMICL_CALL_MISSED == calllog_type)
            {
                cl_type = MMIEVENT_CL_MISSED_CALL;
                event_info.param_ptr = &cl_type;
            }
#endif
            MMIFW_TriggerEvent(&event_info);
        }
#endif
    }
    /*cr128956 renwei add*/
    #ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT   
    if(MMICL_CALL_MISSED == calllog_type)
    {
        MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL);
    }
    #endif
    /*cr128956 renwei add*/
    SCI_MEMSET(tel_num, 0x00, sizeof(tel_num));
    if (MMICL_CALL_DIALED == calllog_type)
    {
        tel_num_len = MMIAPICOM_GenDispNumber(
                                                cl_info.call_num.number.number_type , 
                                                (uint8)cl_info.call_num.number.num_len, 
                                                cl_info.call_num.number.party_num,
                                                tel_num,
                                                sizeof(tel_num)
                                                );
    }
    else
    {
        tel_num_len = MMIAPICOM_GenNetDispNumber(
                                                cl_info.call_num.number.number_type , 
                                                (uint8)cl_info.call_num.number.num_len, 
                                                cl_info.call_num.number.party_num,
                                                tel_num,
                                                sizeof(tel_num)
                                                );
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    MMIAPIPB_SetNumberUsed(tel_num, tel_num_len);
#endif
    /* Bug fix: cr00198061. when a call disconnected, allow key auto lock */
    MMIDEFAULT_AllowTurnOffBackLight(TRUE);

    //if don't play end hit, excute call end operation
    if(!MMICC_GetIsPlayCallEndHintFlag() && !MMK_IsOpenWin(MMICC_DISCONNECT_WIN_ID))
    {
        //MMICC_HandleOperAfterCallEnd();
    }

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
    if (is_need_answer_sms)
    {
        if (!MMIAPIDM_GetHandsetLock())
        {
            //sms reject 拒接窗口存在，则关闭
            if (MMK_IsOpenWin(MMICC_ANSWER_SMS_AFTER_REJECTING_WIN_ID))
            {
                MMK_CloseWin(MMICC_ANSWER_SMS_AFTER_REJECTING_WIN_ID);
            }

            MMIAPICC_SendSMSAfterRejectMTCall(tel_num, tel_num_len);
        }
    }
#endif
    if (0 != MMICC_GetCallNum())
    {
        MAIN_SetCallingState(TRUE);
    }
}

/*****************************************************************************/
//  Description : 通话结束嘟嘟音，根据传入的param，自减1做嘟嘟音计数
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void CC_PlayCallEndHint(uint8 timer_id, uint32 param)
{
    uint32  hintVolume = MMISET_VOL_FOUR;

    if (0 != s_cc_call_end_hint_timer)
    {
        MMK_StopTimer(s_cc_call_end_hint_timer);
        s_cc_call_end_hint_timer = 0;

        if (param > 0)
        {
            //可能这时tone音正在播放，需要停止
            MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
            MMISRVAUD_SetVolumeNoHandle(hintVolume);
            MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, MMI_NT_RING_DIS_ID, MMI_200MSECONDS, MMISRVAUD_TONE_FREQ_DEFAULT);
            param --;
            s_cc_call_end_hint_timer = MMK_CreateTimerCallback(MMI_200MSECONDS, CC_PlayCallEndHint, param, FALSE);
        }
        else if (0 == param)//挂断音播放结束之后，需要把音量值设回原来的值
        {
            MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
            MMISRVAUD_SetVolumeNoHandle( MMIAPICC_GetCallVolume());

            //destory audio handle
            if(PNULL != MMICC_GetAppletInstance())
            {
                //stop cc applet
                MMICC_DestroyAudioHandle();
                MMK_PostMsg(MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
            }
        }
    }
}

typedef struct
{
    uint8    number[CC_MAX_TELE_NUM_LEN + 2];
}URI_NUMBER_T;

LOCAL BOOLEAN is_valid_number(
                            uint8    *tel_str    // in:  telephone number string
                            )
{
    int32   i = 0;
    int32   len =  strlen(tel_str);
    for(i=0; i < len; i++)
    {
        switch (*tel_str)
        {
            case '+':
            {
                break;
            }

            default:
            {
                if (*tel_str >= '0' && *tel_str <= '9')
                {
                    break;
                }
                else
                {
                    return FALSE;
                }
            }
        }

        tel_str++;
    }
    return TRUE;
}

LOCAL BOOLEAN CC_GetUriPhoneNumber(
            const char    *uri_ptr,       //[IN]
            URI_NUMBER_T  *uri_number_ptr //[OUT]
            )
{
    const char    *uri_number_start_ptr = PNULL;
    const char    *uri_number_end_ptr = PNULL;
    const char    *uri_host_start_ptr = PNULL;
    const char    *uri_number_semicolon_ptr = PNULL;

    if (uri_ptr == PNULL || uri_number_ptr == PNULL)
    {
        SCI_TRACE_LOW("CC_GetUriPhoneNumber para error");
        return FALSE;
    }

    uri_number_start_ptr = strstr(uri_ptr, "sip:");
    if (uri_number_start_ptr != PNULL)
    {
        uri_number_start_ptr += strlen("sip:");
    }
    else
    {
        SCI_TRACE_LOW("CC_GetUriPhoneNumber wrong number head! uri_number_start_ptr = %s", uri_number_start_ptr);
        return FALSE;
    }

    //get host string start pointer
    uri_host_start_ptr = strstr(uri_ptr, "@");
    uri_number_semicolon_ptr = strstr(uri_ptr, ";");
    if (NULL != uri_number_semicolon_ptr)
    {
        if (NULL == uri_host_start_ptr)
        {
            uri_number_end_ptr = uri_number_semicolon_ptr;
        }
        else if (0 > (uri_number_semicolon_ptr - uri_host_start_ptr))
        {
            uri_number_end_ptr = uri_number_semicolon_ptr;
        }
        else
        {
            uri_number_end_ptr = uri_host_start_ptr;
        }
    }
    else if (NULL != uri_host_start_ptr)
    {
        uri_number_end_ptr = uri_host_start_ptr;
    }
    else
    {
        SCI_TRACE_LOW("CC_GetUriPhoneNumber wrong number semicolon!  uri_number_start_ptr = %s", uri_number_semicolon_ptr);
        return FALSE;
    }

    if (uri_number_end_ptr != uri_number_start_ptr)
    {
        strncpy(
            uri_number_ptr->number,
            uri_number_start_ptr,
            MIN(sizeof(uri_number_ptr->number) - 1, uri_number_end_ptr - uri_number_start_ptr)
            );

       if (is_valid_number( uri_number_ptr->number))
       {
            return TRUE;
       }
       else
       {
            SCI_TRACE_LOW("CC_GetUriPhoneNumber Invalid Number");
       }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : VOLTE电话,有时网络携带的是sip类型字符串号码
//                sip:15982864307@sc.ims.mnc001.mcc460.3gppnetwork.org;user=phone
//  Author:Yonghua.zhang
//  Note:
/*****************************************************************************/
LOCAL void CC_ParseMTNumberFromSipUri(DPARAM param)
{
    APP_MN_SETUP_IND_T *pSig = (APP_MN_SETUP_IND_T*)param;
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2]= {0};
    URI_NUMBER_T uri_number = {0};
    uint8 tel_num_len = 0;
    uint8 trans_len = 0;

    SCI_TRACE_LOW("CC_ParseMTNumberFromSipUri pSig->calling_num_present=%d, uri_len =%d",pSig->calling_num_present, strlen((char*)pSig->uri_addr));

    //号码信息保存在uri_addr字段中
    if (!pSig->calling_num_present)
    {
        //check if it is  uri number or not
        if (strlen((char*)pSig->uri_addr) > 0)
        {
            SCI_TRACE_LOW("CC_ParseMTNumberFromSipUri   pSig->calling_num.calling_uri = %s",pSig->uri_addr);

            memset(&uri_number,0,sizeof(URI_NUMBER_T));
            if (CC_GetUriPhoneNumber(pSig->uri_addr , &uri_number))
            {
                if (uri_number.number[0] == '+')
                {
                    pSig->calling_num.number_type=MN_NUM_TYPE_INTERNATIONAL;
                    strcpy(tele_num,&uri_number.number[1]);
                    tel_num_len = strlen((char*)tele_num);
                    SCI_TRACE_LOW("CC_ParseMTNumberFromSipUri+ len=%d num=%s",tel_num_len,tele_num);
                }
                else if ((uri_number.number[0] == '0') && (uri_number.number[1] == '0'))
                {
                    pSig->calling_num.number_type=MN_NUM_TYPE_INTERNATIONAL;
                    strcpy(tele_num,&uri_number.number[2]);
                    tel_num_len = strlen((char*)tele_num);
                    SCI_TRACE_LOW("CC_ParseMTNumberFromSipUri len=%d num=%s",tel_num_len,tele_num);
                }
                else
                {
                    strcpy(tele_num,uri_number.number);
                    tel_num_len = strlen((char*)tele_num);
                    SCI_TRACE_LOW("CC_ParseMTNumberFromSipUri len=%d num=%s",tel_num_len,tele_num);
                }
                memset(pSig->calling_num.party_num,0xff,MN_MAX_ADDR_BCD_LEN);
                MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, tele_num, (uint8*)pSig->calling_num.party_num);
                pSig->calling_num.num_len = (strlen((char *)tele_num) + 1)/2;
                pSig->calling_num_present = TRUE;
            }
            else
            {
                SCI_TRACE_LOW("CC_ParseMTNumberFromSipUri Failed");
            }
        }
    }
    else
    {
        if (MN_NUM_TYPE_INTERNATIONAL == pSig->calling_num.number_type)
        {
            /* There is already a "+" in the number. So change the number_type to avoid adding other "+". */
            trans_len = (uint8)(pSig->calling_num.num_len << 1);
            MMIAPICOM_BcdToStr(PACKED_LSB_FIRST ,(uint8*)pSig->calling_num.party_num, trans_len, (char *)tele_num);

            if (('+' == tele_num[0]))
            {
                memset(pSig->calling_num.party_num,0xff,MN_MAX_ADDR_BCD_LEN);
                MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, &tele_num[1], (uint8*)pSig->calling_num.party_num);
                pSig->calling_num.num_len = (strlen((char *)&tele_num[1]) + 1)/2;
            }
            else if (('0' == tele_num[0]) && ('0' == tele_num[1]))
            {
                memset(pSig->calling_num.party_num,0xff,MN_MAX_ADDR_BCD_LEN);
                MMIAPICOM_StrToBcd(PACKED_LSB_FIRST, &tele_num[2], (uint8*)pSig->calling_num.party_num);
                pSig->calling_num.num_len = (strlen((char *)&tele_num[2]) + 1)/2;
            }

            SCI_TRACE_LOW("CC_ParseMTNumberFromSipUri tele_num = %s, len = %d, trans_len = %d",tele_num, pSig->calling_num.num_len, trans_len);
        }
    }
    return;
}

/*****************************************************************************/
// 	Description : process when received signal APP_MN_SETUP_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:modify by louis.wei 
/*****************************************************************************/
LOCAL void CC_CallSetupInd(DPARAM param)
{
    APP_MN_SETUP_IND_T		*pSig = (APP_MN_SETUP_IND_T*)param;
    uint8				tele_num[CC_MAX_TELE_NUM_LEN + 2]= {0};
    MMIPB_BCD_NUMBER_T	pb_num = {0};
    MMI_STRING_T		name = {0};
    uint8				copy_len = 0;
    uint8				mt_call = CC_INVALID_INDEX;
    uint8	tel_num_len = 0;
    
    if(PNULL== pSig)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_CallSetupInd() pSig == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3217_112_2_18_2_10_13_82,(uint8*)"");
        return;
    }

    //check the param
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_CallSetupInd(), s_call_context.dual_sys = %d, s_call_context.call_number = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3214_112_2_18_2_10_13_81,(uint8*)"dd",pSig->dual_sys, s_call_context.call_number);
    
    //if total call number is beyond 7
    if(s_call_context.call_number >= CC_MAX_CALL_NUM)
    {
        //SCI_TRACE_LOW:"ccapp.c:CC_CallSetupInd() call_number= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3224_112_2_18_2_10_13_83,(uint8*)"d", s_call_context.call_number);
        return; 
    }
    if(  pSig->call_id >= CC_MAX_CALL_NUM ) 
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_CallSetupInd() , call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3229_112_2_18_2_10_13_84,(uint8*)"d", pSig->call_id);
        return;
    }
  		
    //for mt call fire wall
    if( pSig->calling_num_present )
    {        
        tel_num_len =  MMIAPICOM_GenNetDispNumber( pSig->calling_num.number_type,
            (uint8)pSig->calling_num.num_len,
            (uint8*)pSig->calling_num.party_num,
            tele_num ,
            (CC_MAX_TELE_NUM_LEN + 2));
        }
    //update s_call_context
    {
        s_call_context.dual_sys = pSig->dual_sys;
        s_call_context.call_number++;
        s_call_context.incoming_call_index = s_call_context.call_number - 1;
        mt_call = s_call_context.incoming_call_index;
        s_call_context.exist_call.exist_incoming_call = TRUE;
        s_call_context.call_info[mt_call].call_id = pSig->call_id;
        s_call_context.call_info[mt_call].dual_sys = pSig->dual_sys;
#ifdef MMI_CSP_SUPPORT
        s_call_context.call_info[mt_call].line = (MN_BEARER_CAPABILITY_LINE_2 == pSig->bc_type)?1:0;
#endif
            //s_call_context.call_info[mt_call].call_mode = CC_CALL_NORMAL;
        if( CC_NO_NUM == s_call_context.call_number -1 )//判断之前有没有电话
        {
            s_call_context.call_info[mt_call].call_state = CC_INCOMING_CALL_STATE;
        }
        else
        {
            s_call_context.call_info[mt_call].call_state = CC_WAITING_STATE;
        }    
        
        s_call_context.call_info[mt_call].address.number_type
            =  pSig->calling_num.number_type;
        s_call_context.call_info[mt_call].address.number_plan
            =  pSig->calling_num.number_plan;
        
        s_call_context.call_info[mt_call].call_type = CC_MT_CALL_TYPE;
        s_call_context.call_info[mt_call].call_time.start_time = MMIAPICOM_GetCurTime( );
        
        s_call_context.current_call = FindStateCall(CC_INCOMING_CALL_STATE);
        if (s_call_context.current_call >= s_call_context.call_number)
        {
            s_call_context.current_call = FindStateCall(CC_WAITING_STATE);
        }
        
        //MO的时候MT进来，MT优先，断掉MO
        s_call_context.want_mo_call.is_want_mo_call = FALSE;
    }
    
    // copy the number to address 
    if( pSig->calling_num_present )
    {
        BOOLEAN     result = FALSE;
        s_call_context.call_info[mt_call].address.num_len
            =  pSig->calling_num.num_len;
        MMI_MEMCPY(
            s_call_context.call_info[mt_call].address.party_num,
            sizeof(s_call_context.call_info[mt_call].address.party_num),
            pSig->calling_num.party_num,
            sizeof(pSig->calling_num.party_num),
            sizeof(s_call_context.call_info[mt_call].address.party_num)
            );
        
        //根据号码搜索电话簿
        {
            SCI_MEMSET((void*)s_call_context.call_info[mt_call].name, 0, sizeof(s_call_context.call_info[mt_call].name) );
            SCI_MEMSET( &name, 0, sizeof(name) );
            name.wstr_ptr = s_call_context.call_info[mt_call].name;    
            
            //获得号码
            pb_num.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(pSig->calling_num.number_type, pSig->calling_num.number_plan );
            
            copy_len = ( pSig->calling_num.num_len > MMIPB_BCD_NUMBER_MAX_LEN ) ? MMIPB_BCD_NUMBER_MAX_LEN : pSig->calling_num.num_len;
            pb_num.number_len = copy_len;
            MMI_MEMCPY(
                pb_num.number,
                sizeof(pb_num.number),
                pSig->calling_num.party_num,
                sizeof(pSig->calling_num.party_num),
                copy_len
                );
            
            //在电话本中查找
            result = MMIAPIPB_GetNameByNumber(&pb_num, &name, CC_MAX_PBNAME_LEN, FALSE) ;/*lint !e506 */
            SCI_TRACE_BUF("ccapp.c, CC_CallSetupInd pb_num=%x", pb_num.number, sizeof(pb_num.number));
            //SCI_TRACE_LOW:"ccapp.c, CC_CallSetupInd search_result=%d, name.wstr_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3315_112_2_18_2_10_13_85,(uint8*)"dd", result, name.wstr_len);
        }

        s_call_context.call_info[mt_call].is_mute = FALSE;
        s_call_context.want_mo_call.is_save_name = FALSE;
        
        //名字
        if (result && (0 != name.wstr_len))//电话本中存在且名字不为空
        {
            s_call_context.call_info[mt_call].is_name_exist = TRUE;
            
            s_call_context.call_info[mt_call].name_len = name.wstr_len ;
        }
        else
        {
            uint8	str_len = strlen((char*)tele_num);

            s_call_context.call_info[mt_call].is_name_exist = FALSE;
            
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
            copy_len = str_len;
#else            
            copy_len = ( str_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : str_len;/*lint !e506 */
#endif
            s_call_context.call_info[mt_call].name_len = copy_len;
            //SCI_TRACE_LOW:"%d,%d,%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3340_112_2_18_2_10_13_86,(uint8*)"ddd",s_call_context.call_info[mt_call].name_len,sizeof(tele_num),copy_len);
            MMI_STRNTOWSTR(
                (void*)s_call_context.call_info[mt_call].name,
                s_call_context.call_info[mt_call].name_len,
                (void*)tele_num,
                sizeof(tele_num),
                copy_len
                );
        }
        }
    else
    {
        s_call_context.call_info[mt_call].name_len = 0;
    }	
    MMIAPIFMM_ExitFileOption();
    
    MMIAPIDC_Interrupt();

    //MMIAPIRECORD_Stop();
#ifdef MMS_SUPPORT
    MMIAPIMMS_Interupt();
#endif
    
    MMIAPIPICVIEW_StopDeleteFiles();
    //not necessary here
#ifdef VIDEO_PLAYER_SUPPORT
    WatchGallery_Vp_PauseForCC();//1341508,1346393
#endif

#ifdef MOBILE_VIDEO_SUPPORT
    if(MMIAPIMV_IsLiving())
    {
        MMIAPIMV_Exit();
    }
#endif    
    
#ifdef VT_SUPPORT
    if (MMIAPISET_GetVTOneTimeRingOption()
        && MMIAPIVT_IsFallBackCall(s_call_context.call_info[s_call_context.current_call].address))
    {
        //回落电话自动接听
#ifndef MMI_GUI_STYLE_TYPICAL    
        MMICC_UpdateCallStatusDisplay(MMICC_MT_START_IND);//bug337284 //先启动一下CC的Applet，否则直接接听会异常
#endif
        CC_ConnectCallReq();
    }
    else
#endif
    {
        MMICC_UpdateCallStatusDisplay(MMICC_MT_START_IND);
    }

    //check close stk display text win
    MMIAPISTK_CheckCloseDisplayTextIRWin();

    MMICC_EnableMute(MMICC_IsCurrentCallMute());
}

/*****************************************************************************/
// 	Description : process when user want to receive a MT call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ConnectCallReq(void)
{
#if defined(MMI_MULTI_SIM_SYS_NONE) && defined(BT_DIALER_SUPPORT)
    BT_STATUS bt_status = BT_ERROR;
#endif    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_ConnectCallReq()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3399_112_2_18_2_10_13_87,(uint8*)"");
    
    //call function of MN to accept call
#ifndef MMI_MULTI_SIM_SYS_NONE    
    if(MN_RETURN_SUCCESS != MNCALL_ConnectCallEx(
        s_call_context.dual_sys,
        s_call_context.call_info[s_call_context.current_call].call_id, 
        PNULL
        )
        )
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_ConnectCallReq(), call MNCALL_ConnectCall failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3410_112_2_18_2_10_13_88,(uint8*)"");
        return CC_RESULT_ERROR;
    }
#else
#ifdef BT_DIALER_SUPPORT 	
    if(MMICC_GetBtCallFlag())
    {            
        bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_HOLD_ACTIVE_CALLS);
        if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c: CC_ConnectCallReq  MMIAPIBT_HfuCallHold  fail");
        }
    }
#endif
#endif
    
    s_call_context.exist_call.exist_incoming_call = FALSE;
    //stop ring
    MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
    MMIAPISET_StopRing(MMISET_RING_TYPE_CALL_IN_CALL);

#ifdef MMI_ETWS_SUPPORT
    if( MMK_IsOpenWin(MMISMS_ETWS_NEWMSG_WIN_ID) )
    {
        MMIAPISMSCB_SetIsNeedRedisplay(TRUE);
    }

    if( TRUE == MMIAPISMSCB_IsETWSAlert() )
    {
        MMIAPISET_StopSOSVibrate();
    }
#endif
    //修改CALL LOG
    //更新显示
    s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_CONNECT_OPER;
    MMICC_UpdateCurrentCallProcessing();    //call processing
    
    //cr133748
    s_mt_need_open_codec_flag = s_mt_need_open_codec_flag|CC_MT_CALL_CONNECTING;
    CC_OpenAudioTunnel(TRUE);
    
    return CC_RESULT_SUCCESS; 
}

/*****************************************************************************/
// 	Description : process when received signal EV_SETUP_COMPLETE_IND
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_SetupCompleteInd(DPARAM param)
{
    APP_MN_SETUP_COMPLETE_IND_T	*sig_ptr = (APP_MN_SETUP_COMPLETE_IND_T*)param;
    MMICL_CALL_INFO_T       call_info = {0};/*lint !e64*/
#ifdef BLUETOOTH_SUPPORT
    BT_ADDRESS		address = {0};
#endif    
    
    if(PNULL== sig_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_SetupCompleteInd() sig_ptr == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3445_112_2_18_2_10_13_90,(uint8*)"");
        return;
    }
    //check the param 
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_SetupCompleteInd(), call_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3442_112_2_18_2_10_13_89,(uint8*)"d", sig_ptr->call_id);
    
    if(  sig_ptr->call_id >= CC_MAX_CALL_NUM ) 
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_SetupCompleteInd() , call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3450_112_2_18_2_10_13_91,(uint8*)"d", sig_ptr->call_id);
        return;
    }
    
    //update s_call_context
    s_call_context.current_call = CC_GetIndexOfCallId(sig_ptr->dual_sys, sig_ptr->call_id);
    s_call_context.exist_call.exist_incoming_call = FALSE;
    s_call_context.exist_call.exist_active_call_num++;
    s_call_context.call_info[s_call_context.current_call].call_state = CC_CALL_STATE;
    s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_NO_OPER;
    s_call_context.call_info[s_call_context.current_call].call_type = CC_MT_CALL_TYPE;
    s_call_context.call_info[s_call_context.current_call].call_time.start_time = MMIAPICOM_GetCurTime( );
    s_call_context.call_info[s_call_context.current_call].call_time.stop_time
        = s_call_context.call_info[s_call_context.current_call].call_time.start_time;

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetCallHeldStatus();
#endif

    
    //reset call time if only one call
    if (1 == s_call_context.call_number) 
    {
        s_call_context.call_time_count = 0;
    }    
    s_call_context.call_info[s_call_context.current_call].start_call_time_count = s_call_context.call_time_count;
    
    // 处理当前的来电为正常来电，而非做答录处理时，才建立起通话的画面
    if(IsAcceptCurCallAsNormal())
    {
#ifdef BLUETOOTH_SUPPORT
        //MMIAPIBT_CallConnect(&address); //Bug 1621882
#endif
        //open speaker and microphone
        CC_OpenAudioTunnel(TRUE);
        //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_RESUME, MMICC_AUDIO_DEV_MAX);
        MMICC_UpdateCallStatusDisplay(MMICC_MT_CONNECTED_IND);
        }
#ifdef ASP_SUPPORT
    else
    {
#ifdef BLUETOOTH_SUPPORT
        MMIAPIBT_StopRing();
#endif
        CC_OpenAudioTunnel(TRUE);
        //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_RESUME, MMICC_AUDIO_DEV_MAX);
        MMIAPIASP_StartAnswerCall();
#if !defined(MMI_GUI_STYLE_TYPICAL)//
        MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
#endif
        s_asp_call_id = s_call_context.current_call;
#ifdef MV_SUPPORT
        MMICC_SetIsNeedOpenMv(FALSE); //答录机开启需要关闭魔音
#endif
    }
#endif
    //修改CALL LOG
    // 已接电话
    SCI_MEMSET(&call_info, 0, sizeof(call_info));
    MMI_MEMCPY(
        &call_info.call_time,
        sizeof(call_info.call_time),
        &s_call_context.call_info[s_call_context.current_call].call_time,
        sizeof(s_call_context.call_info[s_call_context.current_call].call_time),
        sizeof(CC_CALL_TIME_T)
        );
    if( s_call_context.call_info[s_call_context.current_call].name_len > 0 )
    {
        call_info.call_num.is_num_present = TRUE;
    }
    else
    {
        call_info.call_num.is_num_present = FALSE;
    }	
    MMI_MEMCPY(
        &call_info.call_num.number,
        sizeof(call_info.call_num.number),
        &s_call_context.call_info[s_call_context.current_call].address,
        sizeof(s_call_context.call_info[s_call_context.current_call].address),
        sizeof(MN_CALLED_NUMBER_T)
        );
    MMICC_EnableMute(MMICC_IsCurrentCallMute());
    EnableSpeakerMode(TRUE); //modify for cr 135730
    //SCI_TRACE_LOW:"CC_SetupCompleteInd(), call_info.call_type= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3526_112_2_18_2_10_13_92,(uint8*)"d", call_info.call_type);
    
    //if connected call num is more than one , asp need to be stopped
#ifdef ASP_SUPPORT
    if(1 < (s_call_context.exist_call.exist_active_call_num + s_call_context.exist_call.exist_hold_call_num))
    {
        MMIAPIASP_StopWaitTimer();
        if(MMIAPIASP_IsMachineWorking())
        {
            MMIAPIASP_StopCallAnswer();
        }
    }
#endif    

#ifdef DPHONE_SUPPORT
    ResetCallHandfreeState();  //Modified by yanqi.2010-Apr-13 for 未接通前回铃音等状态不对
#endif
}

/*****************************************************************************/
// 	Description : process the extension number when make a call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:modify by louis.wei 
/*****************************************************************************/
LOCAL BOOLEAN CC_ProcessExtensionNum(uint8 *tele_num, uint8 *tele_len)
{
    int32	i = 0;
    uint8	copy_len = 0;
    CC_RESULT_E is_dtmf = CC_RESULT_SUCCESS;
    BOOLEAN is_valid_dtmf = FALSE;
    
    //check the param
    if(PNULL== tele_num)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_ProcessExtensionNum() tele_num == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3559_112_2_18_2_10_14_93,(uint8*)"");
        return FALSE;
    }
    if(*tele_len > CC_MAX_TELE_NUM_LEN)
    {
        *tele_len = CC_MAX_TELE_NUM_LEN;
        tele_num[*tele_len] = 0;
    }
    
    
    //check if the extension number exist
    s_call_context.dtmf_state.is_dtmf = FALSE;

    //@fen.xie MS00180417:判断"+P/W..."这种无效的号码
    if (*tele_len > 1)
    {
        if (('+' == tele_num[0]) 
            && (('P' == tele_num[1]) || ('W' == tele_num[1]) || ('p' == tele_num[1]) || ('w' == tele_num[1])))
        {
           return FALSE;
        }
    }
    
    for(i = 1; i < *tele_len; i++)
    {
        //the extension number exist
        is_valid_dtmf = ('P' == tele_num[i] || 'W' == tele_num[i] || 'p' == tele_num[i] || 'w' == tele_num[i]);
        if(is_valid_dtmf)
        {
            //update s_call_context and store extension number			
            s_call_context.dtmf_state.dtmf_len = *tele_len - i;
            if(s_call_context.dtmf_state.dtmf_len > 1)
            {
                s_call_context.dtmf_state.is_dtmf = TRUE;
                copy_len = ( s_call_context.dtmf_state.dtmf_len > CC_MAX_TELE_NUM_LEN ) ? CC_MAX_TELE_NUM_LEN : s_call_context.dtmf_state.dtmf_len;
                MMI_MEMCPY(
                    (char*)s_call_context.dtmf_state.dtmf_str,
                    sizeof(s_call_context.dtmf_state.dtmf_str) - 1,
                    (tele_num + i),
                    ((*tele_len) - i),
                    copy_len
                    );
                s_call_context.dtmf_state.dtmf_len = copy_len;
            
                is_dtmf = MMICC_IsDTMF(s_call_context.dtmf_state.dtmf_str, s_call_context.dtmf_state.dtmf_len);
            }
            else
            {
                s_call_context.dtmf_state.is_dtmf = FALSE;
            }
            tele_num[i] = 0;//exception
            *tele_len = i;            

            return TRUE;
        }
    }

    return TRUE;
    
}



/*****************************************************************************/
// 	Description : clear the stored DTMF after sending them
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL void CC_ClearStoredDtmf(void)
{    
    //update s_call_context
    s_call_context.dtmf_state.is_dtmf = FALSE;
    s_call_context.dtmf_state.dtmf_len = 0;
    SCI_MEMSET((char*)s_call_context.dtmf_state.dtmf_str,0, sizeof(s_call_context.dtmf_state.dtmf_str));
    MMIICC_StopDtmfTimerId();
    s_current_dtmf_call_id = CC_MAX_CALL_NUM;
}

/*****************************************************************************/
// 	Description : clear the stored DTMF after sending them
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMICC_ClearStoredDtmf(void)
{
            CC_ClearStoredDtmf();
}
/*****************************************************************************/
// 	Description : clear the stored DTMF after sending them
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SendSingleDtmf(uint32 msg_id)
{
    return CC_SendSingleDtmf(msg_id);
}

/*****************************************************************************/
// 	Description : clear the stored DTMF after sending them
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL CC_RESULT_E CC_SendSingleDtmf( uint32 msg_id)
{
    uint8 dtmf_ch = 0;
    uint8 is_phone = 1;
    BOOLEAN ret = FALSE;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif
    if( 1 == is_phone )
    {
    if (MMICC_IsAllowSendDtmf())
    {
        //CC界面，全键盘工程，默认需对共用的数字按键进行消息转换。
        if ((MSG_KEYDOWN_Q <= msg_id && msg_id <= MSG_KEYDOWN_EXCLAMATION)
            || (MSG_KEYUP_Q <= msg_id && msg_id <= MSG_KEYUP_EXCLAMATION))
        {
            msg_id = MMIAPIIDLE_GetNumMsgId(msg_id);
        }

        if ((MSG_KEYUP_1 <= msg_id && msg_id <= MSG_KEYUP_9)
            || MSG_KEYUP_0 == msg_id 
            || MSG_KEYUP_STAR == msg_id
            || MSG_KEYUP_HASH == msg_id
            )
        {
            StopDtmfByChar(MN_DUAL_SYS_MAX);
            return CC_RESULT_ERROR;
        }
        //get the ascii from msg id
        dtmf_ch = MMIAPICC_MsgidToAscii(msg_id);
        //if the pressed key is valid
#ifdef BT_DIALER_SUPPORT
        if(MMICC_GetBtCallFlag())
        {
            return CC_BTSendDtmfByChar(dtmf_ch);
        }
        else
#endif    
        return SendDtmfByChar(MN_DUAL_SYS_MAX, dtmf_ch, FALSE);
    }
    else
    {
        return CC_RESULT_ERROR;
    }
    }
    else
    {
        //get the ascii from msg id
        dtmf_ch = MMIAPICC_MsgidToAscii(msg_id);
#ifdef UAL_BT_HFU_SUPPORT
        ret = ual_bt_send_dtmf( dtmf_ch );
#endif
#ifndef WIN32
        SCI_TRACE_LOW("%s: current product ret = %d msg_id = 0x%x, dtmf_ch = %d", __FUNCTION__,ret, msg_id, dtmf_ch);
#endif
    }
}

/*****************************************************************************/
// 	Description : get the ascii from msg id
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_MsgidToAscii(
                                   MMI_MESSAGE_ID_E msg_id
                                   )
{
    uint8 result = CC_INVALID_DTMF_VALUE;
    
    switch(msg_id)
    {
        //press key 1-9
    case MSG_KEYDOWN_1:
    case MSG_KEYDOWN_2:
    case MSG_KEYDOWN_3:
    case MSG_KEYDOWN_4:
    case MSG_KEYDOWN_5:
    case MSG_KEYDOWN_6:
    case MSG_KEYDOWN_7:
    case MSG_KEYDOWN_8:
    case MSG_KEYDOWN_9:
        result = msg_id - (MSG_KEYDOWN_1) + '1';
        break;
        
        //release key 1-9
//    case MSG_KEYUP_1:
//    case MSG_KEYUP_2:
//    case MSG_KEYUP_3:
//    case MSG_KEYUP_4:
//    case MSG_KEYUP_5:
//    case MSG_KEYUP_6:
//    case MSG_KEYUP_7:
//    case MSG_KEYUP_8:
//    case MSG_KEYUP_9:
//        result = msg_id - (MSG_KEYUP_1) + '1';
//        break;
        
        //press key *
    case MSG_KEYDOWN_STAR:
//    case MSG_KEYUP_STAR:
        result = '*';
        break;
        
        //press key 0
    case MSG_KEYDOWN_0:
//    case MSG_KEYUP_0:
        result = '0';
        break;
        
        //press key #
    case MSG_KEYDOWN_HASH:
//    case MSG_KEYUP_HASH:
        result = '#';
        break;
        
    default:
        result = CC_INVALID_DTMF_VALUE;
        break;
    }
    
    return result;
}

#ifdef VT_SUPPORT
/*****************************************************************************/
// 	Description : give a hint to user that there is a missed call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMIAPICC_HandleMissedCall(void)
{
    s_call_context.missed_call_num++;
    MAIN_SetMissedCallState(TRUE);
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_MISS_CALL);
#endif
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT            
	MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL);
#endif
    MMIAPICC_SaveMissedCallNumToNv();
}
#endif


/*****************************************************************************/
// 	Description : get the index of call according to the call id
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
uint8 CC_GetIndexOfCallId(MN_DUAL_SYS_E dual_sys, uint8 call_id)
{
    uint8 i = CC_INVALID_INDEX;
    //check the param
    if( call_id >= CC_MAX_CALL_NUM ) 
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_GetIndexOfCallId , call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3775_112_2_18_2_10_14_94,(uint8*)"d", call_id);
        return CC_INVALID_INDEX;
    }
    
    //循环查找匹配的call id
    for(i = 0; i < s_call_context.call_number; i++)
    {
        if( dual_sys == s_call_context.call_info[i].dual_sys
           && call_id == s_call_context.call_info[i].call_id)
        {
            return i;
        }
    }
    return CC_INVALID_INDEX;
}

/*****************************************************************************/
// 	Description : get the index of call according to the call id
//	Global resource dependence : 
//  Author:figo.feng
//	Note:modify by louis.wei
/*****************************************************************************/
LOCAL void CC_DeleteCallInfo(uint8 call_index)
{
    uint8 i = 0;
    //check the param
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_DeleteCallInfo(), call_index = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3799_112_2_18_2_10_14_95,(uint8*)"d", call_index);
    if( ( call_index >= s_call_context.call_number ) )
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_DeleteCallInfo , call_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3802_112_2_18_2_10_14_96,(uint8*)"d", call_index);
        return;
    }

#ifdef BT_DIALER_SUPPORT
    SCI_TRACE_LOW("ccapp.c:enter function CC_DeleteCallInfo() call_index:%d, s_call_context.current_call:%d, s_call_context.exist_call.exist_incoming_call:%d,s_call_context.incoming_call_index:%d,s_call_context.call_number:%d,btflage:%d", 
    call_index,s_call_context.current_call, s_call_context.exist_call.exist_incoming_call,s_call_context.incoming_call_index,s_call_context.call_number,MMICC_GetBtCallFlag());
#endif

    //delete the call info and update s_call_context
    for(i = ( call_index + 1 ); i < s_call_context.call_number; i++)
    {
        s_call_context.call_info[i - 1] = s_call_context.call_info[i];
    }
    
    if( s_call_context.current_call == call_index )
    {
        s_call_context.current_call = CC_INVALID_INDEX;
    }
    else if( s_call_context.exist_call.exist_incoming_call && ( s_call_context.incoming_call_index  == call_index ) )
    {
      		s_call_context.exist_call.exist_incoming_call = FALSE;
            s_call_context.incoming_call_index = CC_INVALID_INDEX;
    }
    else
    {
#ifdef BT_DIALER_SUPPORT
        if( MMICC_GetBtCallFlag() && ( s_bt_current_call_index > call_index ) &&  (s_bt_current_call_index < s_call_context.call_number))
        {
            s_bt_current_call_index--;
        }
#endif     
        if(( s_call_context.current_call > call_index ) && (s_call_context.current_call < s_call_context.call_number))
        {
            s_call_context.current_call--;
        }
    }
    
    if( ( s_call_context.exist_call.exist_incoming_call ) &&
        ( s_call_context.incoming_call_index > call_index ) )
    {
        s_call_context.incoming_call_index --;        
    }
    
    SCI_MEMSET(&s_call_context.call_info[s_call_context.call_number - 1], 0, sizeof(s_call_context.call_info[s_call_context.call_number - 1]));
    s_call_context.call_info[s_call_context.call_number - 1].call_id = CC_INVALID_ID;
    s_call_context.call_number--;
    //s_isvideocall = FALSE;//@zhaohui add,for cr88897
}

#ifdef MMI_DUALMODE_ENABLE
/*****************************************************************************/
// 	Description : if GSM network support
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_IsNetSupport(MN_DUAL_SYS_E dual_sys)
{
    MN_PHONE_PLMN_STATUS_E plmn_status = PLMN_NO_SERVICE;
    
    plmn_status = MMIAPIPHONE_GetPlmnStatus(dual_sys);
    SCI_TRACE_LOW("ProcessMakeCall IsNetSupport plmn[%d]= %d",dual_sys, plmn_status);
    if( ( PLMN_NO_SERVICE == plmn_status)
       // || ( PLMN_NORMAL_GPRS_ONLY == plmn_status)
        || ( PLMN_REGISTER_GPRS_ONLY == plmn_status)
        || ( PLMN_REGISTER_SERVICE == plmn_status))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
    
}
#endif

/*****************************************************************************/
// 	Description : if GSM network support
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
LOCAL BOOLEAN CC_IsEmergencyOnly(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN is_sim_exist = FALSE;
    
    is_sim_exist = MMIAPIPHONE_GetSimExistedStatus(dual_sys);
    
    return (!is_sim_exist || MMIAPIDM_GetHandsetLock() || SIM_NOT_INITED == MMIAPIPHONE_GetSimStatus(dual_sys));
}

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
/*****************************************************************************/
// 	Description : local setting of emergency call
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_IsEmergencyInLocalEmergencyList(uint8 *tele_num, uint8 tele_len, BOOLEAN *is_local_exisit, MN_DUAL_SYS_E dual_sys)
{
    uint32 i = 0;
    BOOLEAN is_emergency_call = FALSE;
    const MMIPLMN_EMERGENCY_CALL_T *local_emergency_list_ptr = PNULL;

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    local_emergency_list_ptr = MMIAPIPLMN_GetLocalEmergency(dual_sys);

    if (PNULL != is_local_exisit)
    {
        if (PNULL != local_emergency_list_ptr)
        {
            *is_local_exisit = TRUE;
        }
        else
        {
            *is_local_exisit = FALSE;
        }
    }

    if (PNULL == tele_num || 0 == tele_len)
    {
        return is_emergency_call;
    }

    if (PNULL != local_emergency_list_ptr)
    {
        for (i = 0; ; i++)
        {
            if (PNULL != local_emergency_list_ptr[i].emergency_call_ptr)
            {
                uint8 temp_tele_len = strlen(local_emergency_list_ptr[i].emergency_call_ptr);

                if ((temp_tele_len == tele_len)
                    && (0 == strncmp((char*)tele_num, local_emergency_list_ptr[i].emergency_call_ptr, temp_tele_len)))
                {
                    SCI_TRACE_LOW("ccapp.c: CC_IsEmergencyInLocalEmergencyList() is emergency call");

                    is_emergency_call = TRUE;

                    break;
                }
            }
            else
            {
                break;
            }
        }
    }

    return is_emergency_call;
}
#endif

/*****************************************************************************/
// 	Description : 
// 1) 112和911任何情况下都是紧急呼叫
// 2) sim/usim卡上的ecc文件中的紧急呼叫号码呼出都是紧急呼叫
// 3) 无卡时，000, 08, 110, 999, 118 and 119 呼出都是紧急呼叫
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:is_only_for_display means in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num
// but display ENMERGENCY CALL in window 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsEmergencyNum(
                                 uint8 *tele_num, 
                                 uint8 tele_len,
                                 MN_DUAL_SYS_E dual_sys,
                                 BOOLEAN is_only_for_display
                                 )
{
    uint32 i = 0;
    MNSIM_ECC_T sim_emc = {0};//记录sim卡存储的emc号码
    uint8 temp_tele_num[CC_MAX_TELE_NUM_LEN + 1] = {0};
    uint8 temp_tele_len = 0;
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    BOOLEAN is_local_exisit = FALSE;
    BOOLEAN is_emergency_call_in_local = FALSE;
#endif
    
    //check the param
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"ccapp MMIAPICC_IsEmergencyNum error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3901_112_2_18_2_10_14_97,(uint8*)"d", dual_sys);
        return FALSE;
    }
        
    if(PNULL == tele_num)
    {
        //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_IsEmergencyNum tele_num == PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3907_112_2_18_2_10_14_98,(uint8*)"");
        return FALSE;
    }
    
    if(0 == tele_len)
    {
        //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_IsEmergencyNum tele_len == 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3913_112_2_18_2_10_14_99,(uint8*)"");
        return FALSE;
    }

    if ('+' == tele_num[0])
    {
        temp_tele_len = MIN(tele_len - 1, CC_MAX_TELE_NUM_LEN);

        if(0 == temp_tele_len)
        {
            //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_IsEmergencyNum temp_tele_len == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3923_112_2_18_2_10_14_100,(uint8*)"");
            return FALSE;
        }

        SCI_MEMCPY(temp_tele_num, tele_num + 1, temp_tele_len);
    }
    else
    {
        temp_tele_len = MIN(tele_len, CC_MAX_TELE_NUM_LEN);
        SCI_MEMCPY(temp_tele_num, tele_num, temp_tele_len);
    }
    
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
   is_emergency_call_in_local = CC_IsEmergencyInLocalEmergencyList(tele_num, tele_len, &is_local_exisit, dual_sys);

    if (is_emergency_call_in_local)
    {
        return TRUE;
    }
#endif

#ifdef CSC_XML_SUPPORT
    if (s_cc_csc_sos.sos_num > 0)
    {
        if (IsCSCSosNum(temp_tele_num, temp_tele_len, &s_cc_csc_sos))
        {
            return TRUE;
        }
    }
    else
#endif
    {
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
        if (!is_local_exisit)
#endif
        {
            for(i = 0; i < ARR_SIZE(s_emergency_call); i++)
            {
                if( (temp_tele_len == strlen(s_emergency_call[i]))
                    && (0 == strncmp((char*)temp_tele_num, s_emergency_call[i], temp_tele_len)) )
                {
                    //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_IsEmergencyNum is emergency call"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3951_112_2_18_2_10_14_101,(uint8*)"");
                    return TRUE;
                }
            }
        }
    }
    
    sim_emc = MNSIM_GetEccEx(dual_sys);
    
    //SCI_TRACE_LOW:"MMIAPICC_IsEmergencyCall: emc num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3959_112_2_18_2_10_14_102,(uint8*)"d", sim_emc.validate_num);
    
    if (0 < sim_emc.validate_num)
    {    
        for (i = 0; i < sim_emc.validate_num; i++)
        {
            uint8 ecc_code[MN_MAX_SIM_ECC_LEN*2 + 2] = {0};
            uint8 tel_num_len = 0;
            
            SCI_MEMSET(ecc_code, 0x00, sizeof(ecc_code));
            //SCI_MEMCPY(ecc_code, sim_emc.call_codes[i].ecc_code, MN_MAX_SIM_ECC_LEN);
            tel_num_len = MMIAPICOM_GenNetDispNumber(
                MN_NUM_TYPE_NATIONAL, 
                (uint8)(MN_MAX_SIM_ECC_LEN),
                (uint8*)sim_emc.call_codes[i].ecc_code,
                ecc_code,
                (uint8)(MN_MAX_SIM_ECC_LEN*2+2)
                );
            
            //SCI_TRACE_LOW:"MMIAPICC_IsEmergencyNum: ecc_code=%s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3978_112_2_18_2_10_14_103,(uint8*)"s", ecc_code);
            
            if((tele_len == tel_num_len)
                && (0 == strncmp((char*)tele_num, (char *)ecc_code, tele_len)))
            {
                //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_IsEmergencyNum() is sim emc"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3983_112_2_18_2_10_14_104,(uint8*)"");
                return TRUE;
            }
        }
    }

    if (PNULL != s_cc_network_emg_num_list_ptr[dual_sys]
        && s_cc_network_emg_num_list_ptr[dual_sys]->list_num > 0)
    {
        for (i = 0; i < s_cc_network_emg_num_list_ptr[dual_sys]->list_num; i++)
        {
            uint8 ecc_code[MN_MAX_ADDR_BCD_LEN*2 + 2] = {0};
            uint8 tel_num_len = 0;
            
            SCI_MEMSET(ecc_code, 0x00, sizeof(ecc_code));

            tel_num_len = MMIAPICOM_GenNetDispNumber(
                MN_NUM_TYPE_NATIONAL, 
                s_cc_network_emg_num_list_ptr[dual_sys]->emg_num_list[i].num_len,
                (uint8*)s_cc_network_emg_num_list_ptr[dual_sys]->emg_num_list[i].party_num,
                ecc_code,
                (uint8)(MN_MAX_ADDR_BCD_LEN*2+2)
                );
            
            SCI_TRACE_LOW("MMIAPICC_IsEmergencyNum: list_num=%s", ecc_code);
            
            if((tele_len == tel_num_len)
                && (0 == strncmp((char*)tele_num, (char *)ecc_code, tele_len)))
            {
                return TRUE;
            }
        }
    }
    
    if (is_only_for_display || (!is_only_for_display && CC_IsEmergencyOnly(dual_sys)))/*lint !e774 */
    {
#ifdef CSC_XML_SUPPORT
        if (s_cc_csc_no_sim_sos.sos_num > 0)
        {
            if (IsCSCSosNum(temp_tele_num, temp_tele_len, &s_cc_csc_no_sim_sos))
            {
                return TRUE;
            }
        }
        else
#endif
        {
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
            if (!is_local_exisit)
#endif
            {
                for(i = 0; i < ARR_SIZE(s_emergency_call_no_sim); i++)
                {
                    if( (temp_tele_len == strlen(s_emergency_call_no_sim[i]))
                        && (0 == strncmp((char*)temp_tele_num, s_emergency_call_no_sim[i], temp_tele_len)) )
                    {
                        //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_IsEmergencyNum is s_emergency_call_no_sim call %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4007_112_2_18_2_10_14_105,(uint8*)"d", is_only_for_display);
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
    
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsEmergencyCall(void)
{
    return (MN_CALL_TYPE_EMERGENCY == s_call_context.want_mo_call.call_type);
}


/*****************************************************************************/
// 	Description : if the SIM is ok
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
//LOCAL BOOLEAN CC_IsSimOk()
LOCAL BOOLEAN CC_IsSimOk(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"ccapp CC_IsSimOk error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4036_112_2_18_2_10_14_106,(uint8*)"d", dual_sys);
        return FALSE;
    }
    
    return  MMIAPIPHONE_IsSimOk(dual_sys);
}
/*****************************************************************************/
// 	Description : open speaker and microphone
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_OpenAudioTunnel(
                              BOOLEAN is_enable
                              )
{
    if (
#ifdef BT_DIALER_SUPPORT
        !MMICC_GetBtCallFlag() &&
#endif
	    is_enable
        && (CC_INCOMING_CALL_STATE == s_call_context.call_info[ s_call_context.current_call].call_state ) 
        && ((CC_TCH_RESOURCE_ASSIGNED|CC_MT_CALL_CONNECTING) != s_mt_need_open_codec_flag))   //cr133748 mt的时候tch没有分配或者用户没有接听电话，都不能打开voice codec
    {
        //SCI_TRACE_LOW:"ccapp: CC_OpenAudioTunnel codec_flag %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4054_112_2_18_2_10_15_107,(uint8*)"d", s_mt_need_open_codec_flag);
        return;
    }
    
    if (is_enable)
    {
        //StopLocalNetRing();     //打开voice code之前需要关闭本地回铃音
        //MMIAPICC_SwitchAudioDevice(MMICC_AUDIO_OPER_INIT, MMICC_AUDIO_DEV_MAX);
        //init操作有时候是由其他操作发起的，并不一定满足打开voice codec的条件，需要放在外面处理
        MMICC_ConstructVoiceCodec(FALSE);
        MMICC_EnableVoiceCodec(TRUE);
#ifdef MMI_VOLTE_SUPPORT
        CC_EnableVoLTECodec(TRUE);
#endif
        s_mt_need_open_codec_flag = 0;  //打开voice codec之后重置

#ifdef MV_SUPPORT
        //@fen.xie MS00177206:MO/MT时选择取消键则不开启魔音效果
        MMIAPICC_EnableMV();
#endif
        
    }
    else
    {
#ifdef MMI_VOLTE_SUPPORT
        CC_EnableVoLTECodec(FALSE);
#endif
        MMICC_EnableVoiceCodec(FALSE);
        MMICC_DestroyVoiceCodec();
    }
    
}

/*****************************************************************************/
// 	Description : Release all active call 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseActiveCall(void)
{
    uint16  i = 0;
    uint16  index = CC_INVALID_INDEX;
    CC_RESULT_E  result = CC_RESULT_SUCCESS;
#if defined(MMI_MULTI_SIM_SYS_NONE) && defined(BT_DIALER_SUPPORT)
    BT_STATUS bt_status = BT_ERROR;
#endif 

#ifndef MMI_MULTI_SIM_SYS_NONE    
    for( i = 0; i < s_call_context.call_number ; i++ )
    {
        if( CC_CALL_STATE == s_call_context.call_info[ i ].call_state )
        {
            //disconnect the call which state is active
#ifdef BT_DIALER_SUPPORT
            if(MMICC_GetBtCallFlag() && s_bt_current_call_index == i)
            {
                SCI_TRACE_LOW("SPCSS00088114, MMIAPIBT_HfuHangup");
                MMIAPIBT_HfuHangup();
            }
            else
#endif
            {
                
                //whether the call ID is vaild
                if(  s_call_context.call_info[ i ].call_id >= CC_MAX_CALL_NUM )
                {
                    //SCI_TRACE_LOW:"CC_ReleaseActiveCall , call_id = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4101_112_2_18_2_10_15_109,(uint8*)"d", s_call_context.call_info[ i ].call_id);
                    return CC_RESULT_ERROR;
                }
                if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(s_call_context.dual_sys, s_call_context.call_info[ i ].call_id, CC_ResetDisconnectCause(MN_CAUSE_NORMAL_CLEARING), PNULL))
                {
                    result = CC_RESULT_ERROR;
                }
            }
            
            if( !CC_IsCallIDValid(index) )
            {
                index = i;
            }
            
            s_call_context.call_info[ i ].operated_by_user = CC_RELEASE_OPER;
            
            //whether the call is in mpty
            if( ! s_call_context.call_info[i].is_in_mpty )
                break;      
        }
    }
#else
    index = FindStateCall(CC_CALL_STATE);
    if(index < s_call_context.call_number)
    {
        s_bt_current_call_index = index;
        s_call_context.call_info[ s_bt_current_call_index].operated_by_user = CC_RELEASE_OPER;
        bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_RELEASE_ACTIVE_CALLS);
        if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c: CC_HoldAndAnswerCW  MMIAPIBT_HfuCallHold  fail");
        }				
    }
#endif
    
    if( CC_IsCallIDValid(index) )
    {
        //更新提示信息
        s_call_context.current_call = index;
        MMICC_UpdateCurrentCallProcessing();    //call processing
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : Release all hold call 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseHoldCall(void)
{
    uint16  i = 0;
    uint16  index = CC_INVALID_INDEX;
    CC_RESULT_E result = CC_RESULT_SUCCESS;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_ReleaseHoldCall()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4146_112_2_18_2_10_15_110,(uint8*)"");
    
    //search the call which state is in hold
    for( i = 0; i < s_call_context.call_number ; i++ )
    {
        if( CC_HOLD_STATE == s_call_context.call_info[ i ].call_state )
        {
            //whether the call ID is vaild
            if(  ( s_call_context.call_info[ i ].call_id >= CC_MAX_CALL_NUM ) )
            {
                //SCI_TRACE_LOW:"CC_ReleaseHoldCall , call_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4156_112_2_18_2_10_15_111,(uint8*)"d", s_call_context.call_info[ i ].call_id);
                return CC_RESULT_ERROR;
            }
            
            //disconnect the call which state is in hold state
            

            if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(s_call_context.dual_sys, s_call_context.call_info[ i ].call_id, CC_ResetDisconnectCause(MN_CAUSE_NORMAL_CLEARING), PNULL))
            {
                //SCI_TRACE_LOW:"CC_ReleaseHoldCall MNCALL_DiscCall return failure"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4165_112_2_18_2_10_15_112,(uint8*)"");
                result = CC_RESULT_ERROR;
            }
            
            if( !CC_IsCallIDValid(index) )
            {
                index = i;
            }
            
            s_call_context.call_info[ i ].operated_by_user = CC_RELEASE_OPER;
            
            //whether the call is in mpty
            if( ! s_call_context.call_info[i].is_in_mpty )
                break;
        }
    }
    
    if( CC_IsCallIDValid(index) )
    {
        //更新提示信息
        s_call_context.current_call = index;
        MMICC_UpdateCurrentCallProcessing();    //call processing
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : s_call_context
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseActiveCall(void)
{
    return CC_ReleaseActiveCall();
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseHoldCall(void)
{
    return CC_ReleaseHoldCall();
}


/*****************************************************************************/
// 	Description : Release all call ext
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:2004.10.14
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_ReleaseAllCallExt(
                                              CC_RELEASE_ALL_TYPE_E release_all_type
                                              )
{
    uint16  i = 0;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function MMIAPICC_ReleaseAllCallExt()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4221_112_2_18_2_10_15_113,(uint8*)"");

    //如果此时不是mmi发起的呼叫，则挂断键无效
    if ((CC_DIAL_METHOD_OTHER == s_dial_method) && (s_call_context.call_number >0))
    {
        //SCI_TRACE_LOW:"ccapp: ReleaseAllCall fail for at cmd"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4226_112_2_18_2_10_15_114,(uint8*)"");
        return CC_RESULT_ERROR;
    }
    
    for( i = 0; i < s_call_context.call_number; i++ )
    {
        if(s_call_context.call_info[i].call_id >= CC_MAX_CALL_NUM)
        {
            //SCI_TRACE_LOW:"ccapp ReleaseAllCallExt error call_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4234_112_2_18_2_10_15_115,(uint8*)"d", s_call_context.call_info[i].call_id);
            return CC_RESULT_ERROR;
        }
        //原来已经在release状态的就不用release了，但是要改一下release_all_type
        if (CC_RELEASE_OPER != s_call_context.call_info[i].operated_by_user)
        {
            //release
            
	        if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(s_call_context.dual_sys, s_call_context.call_info[ i ].call_id, CC_ResetDisconnectCause(MN_CAUSE_NORMAL_CLEARING), PNULL))

	        {
	            return CC_RESULT_ERROR;
	        }            
	        
	        //set flag
	        s_call_context.call_info[i].operated_by_user = CC_RELEASE_OPER;    
        }
        s_call_context.call_info[i].release_all_type = release_all_type;     
        s_call_context.call_info[i].is_release_all = TRUE;
    }
    
    //提示正在释放
    if (CC_RELEASE_ALL_NOPROMPT != release_all_type)
    {
        MMICC_UpdateCurrentCallProcessing();    //call processing
//        MMK_CloseWin( MMICC_ANIMATION_WIN_ID  );
    }
    return CC_RESULT_SUCCESS;    
}

#if 0
/*****************************************************************************/
// 	Description : Release all call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseAllCall()
{
    uint16  i = 0;
    uint16  index = CC_INVALID_INDEX;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_ReleaseAllCall()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4273_112_2_18_2_10_15_116,(uint8*)"");
    
    for( i = 0; i < s_call_context.call_number ; i++ )
    {        
        if( CC_WAITING_STATE != s_call_context.call_info[i].call_state )
        {
            //whether the call ID is vaild
            if( ( s_call_context.call_info[ i ].call_id >= CC_MAX_CALL_NUM ))
            {
                //SCI_TRACE_LOW:"CC_ReleaseHoldCall , call_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4282_112_2_18_2_10_15_117,(uint8*)"d", s_call_context.call_info[ i ].call_id);
                return CC_RESULT_ERROR;
            }
            
            if( !CC_IsCallIDValid(index) )
            {
                index = i;
            }
            s_call_context.call_info[ i ].operated_by_user = CC_RELEASE_OPER;
        }
    }
    
    if( CC_IsCallIDValid(index) )
    {
        //更新提示信息
        
        
        //        if( MN_RETURN_SUCCESS != MNCALL_ReleaseActiveCalls() )
        if( MN_RETURN_SUCCESS != MNCALL_ReleaseActiveCallsEx(s_call_context.dual_sys) )
        {
            //SCI_TRACE_LOW:"CC_ReleaseActiveCall MNCALL_ReleaseActiveCalls return failure"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4302_112_2_18_2_10_15_118,(uint8*)"");
            return CC_RESULT_ERROR;
        }
        
        s_call_context.current_call = index;
        //提示正在释放
        MMICC_UpdateCurrentCallProcessing();    //call processing
    }
    
    return CC_RESULT_SUCCESS;
}
#endif

/*****************************************************************************/
// 	Description : Hold the active call for preparing  for waiting call
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_HoldAndAnswerCW(void)
{
    return CC_HoldAndAnswerCW();
}

/*****************************************************************************/
// 	Description : Hold the active call for preparing  for waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_HoldAndAnswerCW(void)
{
    uint8  index = 0;
#if defined(MMI_MULTI_SIM_SYS_NONE) && defined(BT_DIALER_SUPPORT)
    BT_STATUS bt_status = BT_ERROR;
#endif

    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HoldAndAnswerCW()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4331_112_2_18_2_10_15_119,(uint8*)"");
    
    if( CC_NO_NUM == s_call_context.call_number )
    {
        return CC_RESULT_ERROR;
    }
    
    //whether the cc module has only active call
    if( ( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num ) &&
        ( CC_NO_NUM == s_call_context.exist_call.exist_hold_call_num ) )    //只有active
    {
        index = FindStateCall( CC_CALL_STATE );
        
        if(index >= s_call_context.call_number)
        {
            //SCI_TRACE_LOW:"ccapp.c: CC_HoldAndAnswerCW() index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4346_112_2_18_2_10_15_120,(uint8*)"d",index);
            return CC_RESULT_ERROR;
        }
        
        //hold the active call
        
#ifndef MMI_MULTI_SIM_SYS_NONE        
        //        if( MN_RETURN_SUCCESS != MNCALL_HoldCall( s_call_context.call_info[ index ].call_id ) )
        if( MN_RETURN_SUCCESS != MNCALL_HoldCallEx(s_call_context.dual_sys, s_call_context.call_info[ index ].call_id ) )
        {
            //SCI_TRACE_LOW:"CC_HoldAndAnswerCW: the result of MNCALL_HoldCall is error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4355_112_2_18_2_10_15_121,(uint8*)"");
            return CC_RESULT_ERROR;
        }
#else
#ifdef BT_DIALER_SUPPORT
        if(MMICC_GetBtCallFlag())
        {    
            s_bt_current_call_index = index;
            bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_HOLD_ACTIVE_CALLS);
            if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
            {
                SCI_TRACE_LOW("ccapp.c: CC_HoldAndAnswerCW  MMIAPIBT_HfuCallHold  fail");
            }
        }
#endif
#endif

        s_call_context.operate_waiting_call = HOLD_ALL_ACTIVE_AND_ACCEPT;
        s_call_context.current_call = index ;
        s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_HOLD_OPER;
        
        //Update the prompt information in the cc window
        MMICC_UpdateCurrentCallProcessing();    //call processing
        MMK_CloseWin(MMICC_MENU_WIN_ID ) ;
    }
    else if( ( CC_NO_NUM == s_call_context.exist_call.exist_active_call_num ) &&
        ( CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num ) )    //只有hold
    {
        // the call module just has hold call
        //accept the incoming call
        s_call_context.current_call = s_call_context.incoming_call_index ;
        if (!MMIAPISTK_SetupCall(s_call_context.dual_sys, TRUE, FALSE, 0))//handle stk call
        {
            CC_ConnectCallReq();
        }
        MMK_CloseWin(MMICC_MENU_WIN_ID ) ;
    }
    else    //既有active又有hold
    {   
        //the cc module have both active call and hold call
        //so need to build MPTY before answer the call
#if !defined(MMI_GUI_STYLE_TYPICAL)
        MMIPUB_OpenAlertWarningWin(TXT_CC_MERGE_CALLS_FIRST);
        MMIPUB_SetWinTitleTextId( MMIPUB_ALERT_WIN_ID, TXT_CC_COMMUN, FALSE );
#else
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_EST_MPTY_FIRST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKEXIT,PNULL);
#endif
    }
#ifdef ASP_SUPPORT
    s_accept_call = TRUE;
    if((index == s_asp_call_id) && (MMIAPIASP_IsMachineWorking()))
    {
        MMIAPIASP_StopAspRecording();
    }
#endif
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
// 	Description : Hold the active call for preparing  for waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_HoldAndAnswerCW(void)
{
    return CC_HoldAndAnswerCW();
}


/*****************************************************************************/
// 	Description : Release the active call for preparing for waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseAndAnswerCW(void)
{
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_ReleaseAndAnswerCW()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4417_112_2_18_2_10_15_122,(uint8*)"");
    
    //whether the call module has the active call
    if( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num )
    {
        s_call_context.operate_waiting_call = RELEASE_ALL_ACTIVE_AND_ACCEPT;
        if( CC_RESULT_ERROR == CC_ReleaseActiveCall() )
        {
            return CC_RESULT_ERROR;
        }
    }
    else if (CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num)
    {
        s_call_context.operate_waiting_call = RELEASE_ALL_ACTIVE_AND_ACCEPT;
        if( CC_RESULT_ERROR == CC_ReleaseHoldCall() )
        {
            return CC_RESULT_ERROR;
        }        
    }
    else
    {

        //answer the waiting call
        s_call_context.current_call = s_call_context.incoming_call_index ;
        CC_ConnectCallReq();
    }
#ifdef ASP_SUPPORT   
    s_accept_call = TRUE;
#endif
    MMK_CloseWin(MMICC_MENU_WIN_ID) ;
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
// 	Description : Release the active call for preparing for waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseAndAnswerCW(void)
{
    return CC_ReleaseAndAnswerCW();
}

/*****************************************************************************/
// 	Description : process to signal of retrieve confirm
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleRetrieveCnf( DPARAM param )
{
    APP_MN_CALL_RETRIEVED_CNF_T *sig_ptr      = (APP_MN_CALL_RETRIEVED_CNF_T*)param;
    uint8                  call_index   = CC_INVALID_INDEX;
    uint8                   i = 0;
    
    if(PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_HandleRetrieveCnf( param == NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4471_112_2_18_2_10_15_123,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleRetrieveCnf(), call_id = %d, req_is_accepted = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4474_112_2_18_2_10_15_124,(uint8*)"dd", sig_ptr->call_id, sig_ptr->req_is_accepted);
    
    //get the index of call id
    call_index = CC_GetIndexOfCallId(sig_ptr->dual_sys, sig_ptr->call_id );
    
    if( sig_ptr->req_is_accepted )
    {
        //retrieve operation is scuccess
        s_call_context.current_call = call_index;
        //prompt the operation is success
        
        MMICC_UpdateCurrentCallProcessing();    //call processing
        //the operation is shuttle between the active and the hold call
        if( ( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num ) &&
            ( CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num ) )
        {
            //modify the information in the s_call_context
            for( i = 0 ; i < s_call_context.call_number ; i++ )
            {
                if( CC_WAITING_STATE != s_call_context.call_info[i].call_state )
                {
                    if( CC_HOLD_STATE == s_call_context.call_info[i].call_state )
                    {
                        s_call_context.call_info[i].call_state = CC_CALL_STATE;
                        s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
                        s_call_context.exist_call.exist_active_call_num ++;
                        s_call_context.exist_call.exist_hold_call_num --;
                    }
                    else
                    {
                        s_call_context.call_info[i].call_state = CC_HOLD_STATE;
                        s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
                        s_call_context.exist_call.exist_hold_call_num ++;
                        s_call_context.exist_call.exist_active_call_num --;
                    }
                }
            }
        }
        else
        {
            //the operation is to retrieve the hold call
            if(call_index >= CC_MAX_CALL_NUM)
            {
                //SCI_TRACE_LOW:"ccapp HandleRetrieveCnf error call_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4517_112_2_18_2_10_15_125,(uint8*)"d", call_index);
                return;
            }
            
            if(call_index < CC_MAX_CALL_NUM && s_call_context.call_info[call_index].is_in_mpty )
            {
                for( i = 0; i < s_call_context.call_number ; i++ )
                {
                    if( s_call_context.call_info[ i ].is_in_mpty )
                    {
                        s_call_context.call_info[ i ].call_state = CC_CALL_STATE;
                        s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
                        s_call_context.exist_call.exist_active_call_num ++;
                        s_call_context.exist_call.exist_hold_call_num --;
                    }
                }
                
            }
            else if(call_index < CC_MAX_CALL_NUM)
            {
                s_call_context.call_info[ call_index ].call_state = CC_CALL_STATE;
                s_call_context.call_info[call_index].operated_by_user = CC_NO_OPER;
                s_call_context.exist_call.exist_active_call_num ++;
                s_call_context.exist_call.exist_hold_call_num --;
            }
        }
        MMICC_EnableMute(MMICC_IsCurrentCallMute());
        
        s_call_context.current_call = FindCallInModule();
        MMICC_UpdateCallStatusDisplay(MMICC_RETRIEVE_SUC_IND);

        //the operation of hold active call is successful
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
        MMIAPIBT_SetCallHeldStatus();
#endif
    }
    else
    {
        if(call_index >= CC_MAX_CALL_NUM)
        {
            //SCI_TRACE_LOW:"ccapp HandleRetrieveCnf call index %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4552_112_2_18_2_10_16_126,(uint8*)"d", call_index);
            return ;
        }        
                
        if (call_index < CC_MAX_CALL_NUM)
        {
            s_call_context.call_info[call_index].operated_by_user = CC_NO_OPER;
        }
        s_call_context.current_call = call_index;
        MMICC_UpdateCallStatusDisplay(MMICC_RETRIEVE_FAIL_IND);
    }
    
}


/*****************************************************************************/
// 	Description : process to the signal of the hold confirm
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleHoldCnf( 
                            DPARAM param 
                            )
{
    APP_MN_CALL_HELD_CNF_T* sig_ptr = (APP_MN_CALL_HELD_CNF_T*)param;
    uint8             call_index = CC_INVALID_INDEX;
    uint8             i = 0;
    //    uint8             index;
    uint8             tele_num_arr[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8             tele_len = 0;
    
    if(PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_HandleHoldCnf( param == NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4584_112_2_18_2_10_16_127,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleHoldCnf(), call_id = %d, req_is_accepted = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4587_112_2_18_2_10_16_128,(uint8*)"dd", sig_ptr->call_id, sig_ptr->req_is_accepted);
    
    call_index = CC_GetIndexOfCallId(sig_ptr->dual_sys, sig_ptr->call_id );

    if( sig_ptr->req_is_accepted )
    {
        if( CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num )	//既有active又有hold
        {
            //do nothing
        }
        else	//只有active
        {
            //save the information of s_call_context
            
            if(call_index >= CC_MAX_CALL_NUM)
            {
                //SCI_TRACE_LOW:"ccapp HandleHoldCnf1 call index: %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4608_112_2_18_2_10_16_129,(uint8*)"d", call_index);
                return ;
            }
            
            s_call_context.current_call = call_index;
            
            if( s_call_context.call_info[ call_index ].is_in_mpty )
            {
                //modify the state of MPTY call
                for( i = 0; i < s_call_context.call_number ; i++ )
                {
                    if( ( s_call_context.call_info[ i ].is_in_mpty ) &&
                        ( CC_CALL_STATE == s_call_context.call_info[ i ].call_state ) )
                    {
                        //update the information of s_call_context
                        s_call_context.call_info[ i ].call_state = CC_HOLD_STATE;
                        s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
                        s_call_context.exist_call.exist_hold_call_num ++;
                        s_call_context.exist_call.exist_active_call_num --;
                    }
                }
            }
            else
            {
                s_call_context.call_info[ call_index ].call_state = CC_HOLD_STATE;
                s_call_context.call_info[call_index].operated_by_user = CC_NO_OPER;
                s_call_context.exist_call.exist_hold_call_num ++;
                s_call_context.exist_call.exist_active_call_num --;
            }
          
            //the operation of hold active call is successful
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
            MMIAPIBT_SetCallHeldStatus();
#endif

            MMICC_EnableMute(MMICC_IsCurrentCallMute());
            //whether there has incoming call
            if( MMICC_IsExistIncommingCall() )
            {
                s_call_context.current_call = s_call_context.incoming_call_index ;
                s_call_context.incoming_call_index = CC_INVALID_INDEX;
                
                if( HOLD_ALL_ACTIVE_AND_ACCEPT == s_call_context.operate_waiting_call )
                {
                    if (!MMIAPISTK_SetupCall(sig_ptr->dual_sys, TRUE, FALSE, 0))//handle stk call
                    {
                        CC_ConnectCallReq();
                    }
                }
                else
                {
                    //the prompt of incoming call 来电提示
                    MMICC_UpdateCallStatusDisplay(MMICC_HOLD_SUC_IND);
                }
            }
            else if( ( s_is_need_call_after_hold ) &&
                ( CC_NO_NUM == s_call_context.exist_call.exist_active_call_num ) )
            {
                uint8 max_tele_len = CC_MAX_TELE_NUM_LEN + 2;
                CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
                
                SCI_MEMSET( (void*)tele_num_arr, 0, sizeof(tele_num_arr));
                tele_len = MMIAPICOM_GenDispNumber(
                    s_call_context.want_mo_call.address.number_type,
                    (uint8)s_call_context.want_mo_call.address.num_len,
                    s_call_context.want_mo_call.address.party_num,
                    tele_num_arr,
                    max_tele_len
                    );
                                
                if (s_call_context.want_mo_call.is_save_name)//this call is from a record
                {
                    MMI_STRING_T name_str = {0};
                    wchar               str_buf[CC_MAX_PBNAME_LEN + 1] = {0};
                
                    name_str.wstr_ptr = str_buf;
                    if (s_call_context.want_mo_call.is_name_exist)
                    {
                        name_str.wstr_len = s_call_context.want_mo_call.name_len;
                        MMI_WSTRNCPY(name_str.wstr_ptr, 
                                                    CC_MAX_PBNAME_LEN, 
                                                    s_call_context.want_mo_call.name, 
                                                    sizeof(s_call_context.want_mo_call.name), 
                                                    name_str.wstr_len);
                    }
                
                    cc_result = MMIAPICC_MakeCall(
                        sig_ptr->dual_sys,
                        tele_num_arr,
                        tele_len,
                        PNULL,
                        &name_str,
                        MMIAPICC_GetCallSimType(sig_ptr->dual_sys),
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
                }
                else
                {
                    cc_result = MMIAPICC_MakeCall(
                        sig_ptr->dual_sys,
                        tele_num_arr,
                        tele_len,
                        PNULL,
                        PNULL,
                        MMIAPICC_GetCallSimType(sig_ptr->dual_sys),
                        CC_CALL_NORMAL_CALL,
                        PNULL
                        );
                }
                
                
                if (CC_RESULT_SUCCESS != cc_result)
                {
                    s_call_context.current_call = call_index;

                    MMICC_UpdateCallStatusDisplay(MMICC_CALL_AFTER_HOLD_FAIL_IND);
                }
            }
            else
            {
                //there just have/has hold call 
                s_call_context.current_call = call_index;
                MMICC_UpdateCallStatusDisplay(MMICC_HOLD_SUC_IND);

                MMIAPISTK_SetupCall(sig_ptr->dual_sys, TRUE, FALSE, 0);
            }
            
        }
        
    }
    else
    {
        ClearWantCallInfo();
        
        if(call_index >= CC_MAX_CALL_NUM)
        {
            //SCI_TRACE_LOW:"ccapp HandleHoldCnf call index: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4740_112_2_18_2_10_16_130,(uint8*)"d", call_index);
            return ;
        }
                
        //update the information  and prompt in cc window
        if(call_index < CC_MAX_CALL_NUM)
        {
            s_call_context.call_info[ call_index ].operated_by_user = CC_NO_OPER;
        }
        s_call_context.current_call = FindCallInModule();
        MMICC_UpdateCallStatusDisplay(MMICC_HOLD_FAIL_IND);        
        MMIAPISTK_SetupCall(sig_ptr->dual_sys, FALSE, FALSE, MN_CAUSE_FACILITY_REJECTED);
    }
    s_is_need_call_after_hold = FALSE;
    
}

/*****************************************************************************/
// 	Description : process the s_call_context in the MPTY
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleDisconnectedInMPTY(
                                       uint8  call_index , //the index of call id which was released
                                       MN_CALL_DISCONNECT_CALL_CAUSE_E cause 
                                       )
{
    uint16      i = 0;
    uint8       index = 0;
    BOOLEAN     is_in_mpty      = FALSE;
    BOOLEAN     need_auto_dial = FALSE;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleDisconnectedInMPTY(), call_index = %d, cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4771_112_2_18_2_10_16_131,(uint8*)"dd", call_index, cause);
    is_in_mpty = s_call_context.call_info[ call_index ].is_in_mpty;
    
#ifdef BT_DIALER_SUPPORT
    if( !MMICC_GetBtCallFlag())
#endif
{  
    need_auto_dial = CC_AutoMoCall( call_index, cause );
}    
    //删除相关CALL INFO
    CC_DeleteCallInfo( call_index );

    MMICC_EnableMute(MMICC_IsCurrentCallMute());
    
    //cr132401 没有处于通话中，保持中和呼出中的电话，则关闭voice codec
    //SCI_TRACE_LOW:"ccapp: mpty active: %d, hold: %d, outgoing: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4780_112_2_18_2_10_16_132,(uint8*)"ddd", s_call_context.exist_call.exist_active_call_num, s_call_context.exist_call.exist_hold_call_num,s_call_context.exist_call.exist_outgoing_call);
    if (0 == s_call_context.exist_call.exist_active_call_num
        &&0 == s_call_context.exist_call.exist_hold_call_num 
        &&0 == s_call_context.exist_call.exist_outgoing_call
#ifdef BT_DIALER_SUPPORT
        && !MMICC_GetBtCallFlag()
#endif    
        )
    {
        if ((!MMICC_IsExistIncommingCall())
            && (0 == MMICC_GetCallNum()) //用户已经接听来电，正在连接中，此时不应该关闭audio,但是要关闭codec
            )
        {
             SCI_TRACE_LOW("volte ccapp.c close audio tunel and clear ps call audio info");
             CC_OpenAudioTunnel(FALSE);
#ifdef MMI_VOLTE_SUPPORT
             CC_ClearPscallInfo();
#endif
         }
        else
        {
            //有尚未接听的来电，只关闭voice codec，不清空参数
            SCI_TRACE_LOW("volte ccapp.c close audio tunel, do not clear ps call audio info");
            CC_OpenAudioTunnel(FALSE);
        }
        //close speaker and microphone
    }

    //SCI_TRACE_LOW:"ccapp DisconnectedInMPTY call_number: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4800_112_2_18_2_10_16_133,(uint8*)"d", s_call_context.call_number);
    if( CC_NO_NUM == s_call_context.call_number )
    {        
        s_call_context.call_time_count = 0;
        
        s_call_context.current_call = CC_INVALID_INDEX;

        s_call_context.dtmf_state.allow_send_flag = 0; 

        if( RELEASE_ALL_CALL_AND_MAKE_CALL == s_call_context.operate_waiting_call )
        {
            MN_SUBADDR_T sub_addr_info = {0};
            MN_SUBADDR_T *sub_addr = PNULL;
            uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
            uint32 tele_len = 0;

            s_call_context.operate_waiting_call = OTHER_OPERATE;

            tele_len = MMIAPICOM_GenDispNumber(s_call_context.want_mo_call.address.number_type, 
                (uint8)s_call_context.want_mo_call.address.num_len, 
                s_call_context.want_mo_call.address.party_num, 
                tele_num, 
                CC_MAX_TELE_NUM_LEN + 2);

            if (s_call_context.want_mo_call.is_sub_addr_present)
            {
                sub_addr_info = s_call_context.want_mo_call.sub_addr;
                sub_addr = &sub_addr_info;
            }

            MakeCallReqExt(s_call_context.want_mo_call.dual_sys,
                                        (uint8 *)tele_num,
                                        tele_len,
                                        PNULL,
                                        PNULL,
                                        s_call_context.want_mo_call.clir_present,
                                        s_call_context.want_mo_call.clir_type,
                                        s_call_context.want_mo_call.call_type,
                                        sub_addr);
        }
    }
    else  if( RELEASE_ALL_ACTIVE_AND_ACCEPT == s_call_context.operate_waiting_call )
    {
        
        if( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num )
        {
            //need wait the other active call to release
            index = FindStateCall( CC_CALL_STATE );
            if( index >= s_call_context.call_number )
            {
                //SCI_TRACE_LOW:"the index of active call is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4817_112_2_18_2_10_16_134,(uint8*)"d", index);
                return;
            }
            s_call_context.current_call = index;
            
        }
        else
        {
            s_call_context.operate_waiting_call = OTHER_OPERATE;
            if( s_call_context.exist_call.exist_incoming_call )
            {
                //accept the waiting call
                s_call_context.current_call = s_call_context.incoming_call_index ;
                s_call_context.incoming_call_index = CC_INVALID_INDEX;
                CC_ConnectCallReq();
                
            }
            else
            {
                //check the status of call module
                //need to find a call in call module
                s_call_context.current_call = FindCallInModule();
            }
        }
    }
    else  if( RELEASE_ALL_ACTIVE_AND_ACCEPT_WAIT == s_call_context.operate_CRSS_code )
    {
        if( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num )
        {
            //need wait the other active call to release
            index = FindStateCall( CC_CALL_STATE );
            if( index >= s_call_context.call_number )
            {
                //SCI_TRACE_LOW:"the index of active call is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4850_112_2_18_2_10_16_135,(uint8*)"d", index);
                return;
            }
            s_call_context.current_call = index;
        }
        else
        {
            s_call_context.operate_CRSS_code = CRSS_OTHER_OPERATE;
            if( s_call_context.exist_call.exist_incoming_call )
            {
                //accept the waiting call
                s_call_context.current_call = s_call_context.incoming_call_index ;
                s_call_context.incoming_call_index = CC_INVALID_INDEX;
                CC_ConnectCallReq();
            }
            else
            {
                //check the status of call module
                //need to find a call in call module
                s_call_context.current_call = FindCallInModule();
            }
        }
    }
    else  if( RELEASE_ALL_ACTIVE_AND_RETRIEVE_HOLD== s_call_context.operate_CRSS_code )
    {
        if( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num )
        {
            //need wait the other active call to release
            index = FindStateCall( CC_CALL_STATE );
            if( index >= s_call_context.call_number )
            {
                //SCI_TRACE_LOW:"the index of active call is %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4881_112_2_18_2_10_16_136,(uint8*)"d", index);
                return;
            }
            s_call_context.current_call = index;
        }
        else
        {
            s_call_context.operate_CRSS_code = CRSS_OTHER_OPERATE;
            if( s_call_context.exist_call.exist_hold_call_num )
            {
                //retrieve the hold call
                CC_HoldCall();
            }
            else
            {
                //check the status of call module
                //need to find a call in call module
                s_call_context.current_call = FindCallInModule();
            }
        }
    }
    else
    {
        if( !CC_IsCallIDValid(s_call_context.current_call) )
        {
            
            if( is_in_mpty )
            {
                //find another call in MPTY
                for( i = 0; i < s_call_context.call_number; i++ )
                {
                    if( s_call_context.call_info[ i ].is_in_mpty )
                    {
                        break;
                    }
                }
                
                if( i < s_call_context.call_number )
                {
                    //modify the current call in the call module
                    if( CC_NO_OPER != s_call_context.call_info[i].operated_by_user )
                    {
                        s_call_context.current_call = i;
                    }
                    else
                    {
                        s_call_context.current_call = FindCallInModule();
                    }                    
                }
                else
                {
                    //this call is the last call in MPTY 
                    //so need to find a call in call module
                    s_call_context.current_call = FindCallInModule();
                }
            }
            else
            {
                //need to find a call in call module
                //to find the call which is releasing 
                for( i = 0; i < s_call_context.call_number ; i++ )
                {
                    if( CC_RELEASE_OPER == s_call_context.call_info[i].operated_by_user )
                    {
                        s_call_context.current_call = i;
                        break;
                    }
                }
                
                if( i == s_call_context.call_number )
                {
                    s_call_context.current_call = FindCallInModule();
                }
            }
        }
        //cr111189
        if (s_call_context.exist_call.exist_incoming_call && s_call_context.call_number == 1)
        {
            s_call_context.call_info[s_call_context.current_call].call_state = CC_INCOMING_CALL_STATE;
#ifdef BLUETOOTH_SUPPORT
            MMIAPIBT_SetBtCallStatus(0);    //通知bt目前没有激活电话。
#endif
        }
        if (is_in_mpty && !MMICC_IsExistMPTY())
        {
            //find another call in MPTY
            for( i = 0; i < s_call_context.call_number; i++ )
            {
                //modify the last call in MPTY as not a MPTY call
                if( s_call_context.call_info[ i ].is_in_mpty )
                {
                    s_call_context.call_info[ i ].is_in_mpty = FALSE;
                    break;
                }
            }
        }
        //如果都是hold call，则切换到激活电话
//        if ((s_call_context.exist_call.exist_hold_call_num > CC_NO_NUM)
//            && (s_call_context.call_number == s_call_context.exist_call.exist_hold_call_num )
//            )
//        {
//            CC_HoldCall();
//        }
    }
    //SCI_TRACE_LOW:"ccapp DisconnectedInMPTY current call:%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_4984_112_2_18_2_10_16_137,(uint8*)"d", s_call_context.current_call);
}

/*****************************************************************************/
// 	Description : to find a call in call module
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL uint8 FindCallInModule(
                             void
                             )
{
    uint8   call_index = CC_INVALID_INDEX;
    uint8   index = 0;
    
    if( CC_NO_NUM != s_call_context.call_number )
    {
        if( s_call_context.exist_call.exist_incoming_call )
        {
            call_index = s_call_context.incoming_call_index ;
        }
        else if( s_call_context.exist_call.exist_active_call_num )
        {
            //find a active call
            index = FindStateCall( CC_CALL_STATE );
            //SCI_TRACE_LOW:"mmiccapp: FindCallInModule , index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5008_112_2_18_2_10_16_138,(uint8*)"d", index);
            if (index >= s_call_context.call_number)
            {
                index = 0;
            }
            call_index = index;
        }
        else if( s_call_context.exist_call.exist_hold_call_num )
        {
            //find a hold call           
            index = FindStateCall( CC_HOLD_STATE );
            //SCI_TRACE_LOW:"mmiccapp: FindCallInModule , index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5019_112_2_18_2_10_16_139,(uint8*)"d", index);
            if (index >= s_call_context.call_number)
            {
                index = 0;
            }
            call_index = index;            
        }
        else
        {
            index = FindStateCall( CC_CALLING_STATE );
            
            //SCI_TRACE_LOW:"mmiccapp: FindCallInModule , index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5030_112_2_18_2_10_16_140,(uint8*)"d", index);
            if (index >= s_call_context.call_number)
            {
                index = 0;
            }
            call_index = index;
            
        }
    }
    
    return call_index;
}


/*****************************************************************************/
// 	Description : process hold the call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_HoldCall(
                              void
                              )
{
    uint8   active_index = CC_INVALID_INDEX;
    uint8   hold_index = CC_INVALID_INDEX;
#ifdef BT_DIALER_SUPPORT 	
    BT_STATUS bt_status = BT_ERROR;
#endif

    if(CC_IsExistOperatedCall())
    {
        SCI_TRACE_LOW("ccapp hold call fail");
		return CC_RESULT_ERROR;
    }
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HoldCall()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5055_112_2_18_2_10_17_141,(uint8*)"");
    
    //whether the call module has not call
    if( CC_NO_NUM != s_call_context.call_number )
    {
        if( ( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num ) &&
            ( CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num ) )
        {
            //shuttle the hold call and active call
            active_index = FindStateCall( CC_CALL_STATE );
            if (active_index >= s_call_context.call_number)
            {
                //SCI_TRACE_LOW:"ccapp HoldCall: invalid active idx: %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5067_112_2_18_2_10_17_142,(uint8*)"d", active_index);
                return CC_RESULT_ERROR;
            }
            s_call_context.call_info[active_index].operated_by_user = CC_SHUTTLE_OPER;
            s_call_context.current_call = active_index;
            //SCI_TRACE_LOW:"CC_HoldCall: the active call is %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5072_112_2_18_2_10_17_143,(uint8*)"s", s_call_context.call_info[active_index].name);
            
            //get the hold call
            hold_index =FindStateCall( CC_HOLD_STATE );
            if (hold_index >= s_call_context.call_number)
            {
                //SCI_TRACE_LOW:"ccapp HoldCall: invalid hold idx: %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5078_112_2_18_2_10_17_144,(uint8*)"d", hold_index);
                return CC_RESULT_ERROR;
            }
            s_call_context.call_info[hold_index].operated_by_user = CC_SHUTTLE_OPER;
            //SCI_TRACE_LOW:"CC_HoldCall: the hold call is %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5082_112_2_18_2_10_17_145,(uint8*)"s", s_call_context.call_info[hold_index].name);
            
#ifdef BT_DIALER_SUPPORT   		
        if(MMICC_GetBtCallFlag())
        {
            
            bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_HOLD_ACTIVE_CALLS);
            if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
            {
                SCI_TRACE_LOW("ccapp.c: CC_HoldCall  MMIAPIBT_HfuCallHold  fail");
            }
        }
        else
        {
            //the operation is to shuttle the active call and the hold call
            if( MN_RETURN_SUCCESS != MNCALL_SwapCallsEx(
                s_call_context.dual_sys,       
                s_call_context.call_info[active_index].call_id, 
                s_call_context.call_info[hold_index].call_id) )    
            {
                SCI_TRACE_LOW("CC_HoldCall: MNCALL_SwapCalls is fault");
                return CC_RESULT_ERROR;
            }
        }
#else
            //the operation is to shuttle the active call and the hold call
            if( MN_RETURN_SUCCESS != MNCALL_SwapCallsEx(
                s_call_context.dual_sys,       
                s_call_context.call_info[active_index].call_id, 
                s_call_context.call_info[hold_index].call_id) )    
            {
                SCI_TRACE_LOW("CC_HoldCall: MNCALL_SwapCalls is fault");
                return CC_RESULT_ERROR;
            }
#endif
        }
        else if( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num )
        {
            //just a active state call 
            active_index = FindStateCall( CC_CALL_STATE );
            if (active_index >= s_call_context.call_number)
            {
                //SCI_TRACE_LOW:"ccapp HoldCall: invalid hold idx: %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5100_112_2_18_2_10_17_146,(uint8*)"d", active_index);
                return CC_RESULT_ERROR;
            }
            s_call_context.call_info[active_index].operated_by_user = CC_HOLD_OPER;
            s_call_context.current_call = active_index;
            //hold the active call
#ifdef BT_DIALER_SUPPORT   		
        if(MMICC_GetBtCallFlag())
        {
            SCI_TRACE_LOW("ccapp.c: ENTER CC_HoldCall  MMIAPIBT_HfuCallHold(BT_HOLD_HOLD_ACTIVE_CALLS)!!!!!");
            bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_HOLD_ACTIVE_CALLS);
            if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
            {
                SCI_TRACE_LOW("ccapp.c: CC_HoldCall  MMIAPIBT_HfuCallHold  fail");
            }
        }
        else
        {
            if( MN_RETURN_SUCCESS != MNCALL_HoldCallEx(s_call_context.dual_sys, s_call_context.call_info[active_index].call_id ) )
            {
                SCI_TRACE_LOW("CC_HoldCall: MNCALL_HoldCall is fault");
                return CC_RESULT_ERROR;
            }
         }
#else
            if( MN_RETURN_SUCCESS != MNCALL_HoldCallEx(s_call_context.dual_sys, s_call_context.call_info[active_index].call_id ) )
            {
                SCI_TRACE_LOW("CC_HoldCall: MNCALL_HoldCall is fault");
                return CC_RESULT_ERROR;
            }
#endif 
        }
        else
        {
            //just the hold state call
            hold_index = FindStateCall( CC_HOLD_STATE );
            if (hold_index >= s_call_context.call_number)
            {
                //SCI_TRACE_LOW:"ccapp HoldCall: invalid hold idx: %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5118_112_2_18_2_10_17_147,(uint8*)"d", hold_index);
                return CC_RESULT_ERROR;
            }
            s_call_context.call_info[hold_index].operated_by_user = CC_RETRIEVE_OPER;
            s_call_context.current_call = hold_index;
            //retrieve the hold call
#ifdef BT_DIALER_SUPPORT   		
        if(MMICC_GetBtCallFlag())
        {
            SCI_TRACE_LOW("ccapp.c: ENTER CC_HoldCall  MMIAPIBT_HfuCallHold(BT_HOLD_HOLD_ACTIVE_CALLS)!!!!!");
            s_bt_current_call_index = hold_index;
            bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_HOLD_ACTIVE_CALLS);
            if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
            {
                SCI_TRACE_LOW("ccapp.c: CC_HoldCall  MMIAPIBT_HfuCallHold  fail");
            }
        }
        else
        {
            if( MN_RETURN_SUCCESS != MNCALL_RetrieveCallEx(s_call_context.dual_sys, s_call_context.call_info[hold_index].call_id ) )
            {
                SCI_TRACE_LOW("CC_HoldCall: MNCALL_RetrieveCall is fault");
                return CC_RESULT_ERROR;
            }
         }
#else
            if( MN_RETURN_SUCCESS != MNCALL_RetrieveCallEx(s_call_context.dual_sys, s_call_context.call_info[hold_index].call_id ) )
            {
                SCI_TRACE_LOW("CC_HoldCall: MNCALL_RetrieveCall is fault");
                return CC_RESULT_ERROR;
            }
#endif
        }
        //update the cc window
        MMICC_UpdateCurrentCallProcessing();    //call processing
    }
    else
    {
        //use the msgbox to prompt the operation is fail
        MMIPUB_OpenAlertWarningWin(TXT_CC_OPER_FAIL);
#if !defined(MMI_GUI_STYLE_TYPICAL)
        MMIPUB_SetWinTitleTextId( MMIPUB_ALERT_WIN_ID, TXT_CC_HOLD_PDA, FALSE );
#endif
        return CC_RESULT_ERROR;
    }
    
    return CC_RESULT_SUCCESS;
}
/*****************************************************************************/
// 	Description : process hold the call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_HoldCall(
                              void
                              )
{
    return CC_HoldCall();
}

/*****************************************************************************/
// 	Description : process to build MPTY call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_BuildMPTYCall(
                                   void
                                   )
{
    uint8   active_index = CC_INVALID_INDEX;
    uint8   hold_index   = CC_INVALID_INDEX;
#ifdef BT_DIALER_SUPPORT
    BT_STATUS bt_status = BT_ERROR;
#endif

    if(CC_IsExistOperatedCall())
    {
        SCI_TRACE_LOW("ccapp build mpty call fail");
		return CC_RESULT_ERROR;
    }
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_BuildMPTYCall()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5167_112_2_18_2_10_17_148,(uint8*)"");
    
    //whether the condition is satisfied
    if( ( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num ) &&
        ( CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num ) )
    {
        //get the active call
        active_index = FindStateCall( CC_CALL_STATE );
        s_call_context.current_call = active_index;
        if(active_index >= s_call_context.call_number)
        {
            //SCI_TRACE_LOW:"ccapp error active_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5178_112_2_18_2_10_17_149,(uint8*)"d", active_index);
            return CC_RESULT_ERROR;
        }
        s_call_context.call_info[active_index].operated_by_user = CC_BUILDMPTY_OPER;
        
        //get the hold call
        hold_index = FindStateCall( CC_HOLD_STATE );
        if(hold_index >= s_call_context.call_number)
        {
            //SCI_TRACE_LOW:"ccapp error hold_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5187_112_2_18_2_10_17_150,(uint8*)"d", hold_index);
            return CC_RESULT_ERROR;
        }
        s_call_context.call_info[hold_index].operated_by_user = CC_BUILDMPTY_OPER;
        
        //call the function to build MPTY
		//bug 478014
#if !defined( MMI_MULTI_SIM_SYS_NONE )
        MNCALL_BuildMPTYEx(
            s_call_context.dual_sys,
            s_call_context.call_info[ hold_index ].call_id,
            s_call_context.call_info[ active_index].call_id );
        MMICC_UpdateCurrentCallProcessing();    //call processing
#else
        bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_ADD_HELD_CALL);
        if(BT_SUCCESS !=  bt_status && BT_PENDING != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c:CC_BtCallCcwaInd() MMIAPIBT_HfuCallHold fail");
        }
        MMICC_UpdateCurrentCallProcessing();    //call processing
#endif
    }
    else
    {
        //use the MSGBOX to prompt the operation is fail
        MMIPUB_OpenAlertWarningWin(TXT_CC_OPER_FAIL);
        
        return CC_RESULT_ERROR;
    }
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
// 	Description : process to build MPTY call for accepting the incoming call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_BuildMPTYCall(
                                          void
                                          )
{
    return CC_BuildMPTYCall();
}


/*****************************************************************************/
// 	Description : process to build MPTY call for accepting the incoming call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_BuildMPTYAndAnswerCW(
                                          void
                                          )
{
    CC_RESULT_E     result = CC_RESULT_ERROR;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_BuildMPTYAndAnswerCW()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5231_112_2_18_2_10_17_151,(uint8*)"");
    
    //build mpty call in the first
    MMK_CloseWin( MMICC_ANIMATION_WIN_ID  );
    if( CC_RESULT_SUCCESS == CC_BuildMPTYCall( ) )
    {
        s_call_context.operate_waiting_call = BUILD_MPTY_AND_ACCEPT;
        result = CC_RESULT_SUCCESS;
    }
    else
    {
        result = CC_RESULT_ERROR;
    }
    
    return result;
}


/*****************************************************************************/
// 	Description : process to change the operate_state to CC_NO_OPER
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void TranslateOperState( 
                              CC_OPERATED_BY_USER_E   operate_state
                              )
{
    uint8	i = 0;
    
    //SCI_TRACE_LOW("ccapp.c:enter function TranslateOperState()");
    
    for( i = 0; i < s_call_context.call_number; i++ )
    {
        if(  operate_state == s_call_context.call_info[i].operated_by_user  )
        {
            s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
        }
    }
}
/*****************************************************************************/
// 	Description : process to handle the confirm for build MPTY call 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleBuildMPTYCnf(
                                 DPARAM param
                                 )
{
    APP_MN_BUILD_MPTY_CNF_T *sig_ptr = (APP_MN_BUILD_MPTY_CNF_T*)param;
    uint8               i = 0;
    uint8               call_index = CC_INVALID_INDEX;
    
    if(PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_HandleBuildMPTYCnf( param == NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5283_112_2_18_2_10_17_152,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleBuildMPTYCnf(), call_id = %d, req_is_accepted = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5286_112_2_18_2_10_17_153,(uint8*)"dd", sig_ptr->call_id, sig_ptr->req_is_accepted);
    
    //whether the call id is vaild
    if( ( sig_ptr->call_id >= CC_MAX_CALL_NUM ) )
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_HandleBuildMPTYCnf() , call_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5291_112_2_18_2_10_17_154,(uint8*)"d", sig_ptr->call_id);
        return;
    }
    
    call_index = CC_GetIndexOfCallId(sig_ptr->dual_sys, sig_ptr->call_id );

    if(!CC_IsCallIDValid(call_index) )
    {
        //SCI_TRACE_LOW:"CC_HandleBuildMPTYCnf() invald call index %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5299_112_2_18_2_10_17_155,(uint8*)"d", call_index);
        return;
    }
    
    s_call_context.current_call = call_index;
    s_call_context.call_info[call_index].operated_by_user = CC_NO_OPER;/*lint !e661 !e662*/ 
    
    TranslateOperState(  CC_BUILDMPTY_OPER );
    
    if( sig_ptr->req_is_accepted )
    {
        for( i = 0; i < s_call_context.call_number ; i++ )
        {
            if( CC_WAITING_STATE != s_call_context.call_info[ i ].call_state )
            {
                s_call_context.call_info[ i ].is_in_mpty = TRUE;
                
                //modify the call state in the s_call_context
                if( CC_HOLD_STATE == s_call_context.call_info[i].call_state)
                {
                    s_call_context.call_info[i].call_state = CC_CALL_STATE;
                    s_call_context.exist_call.exist_active_call_num++;
                    s_call_context.exist_call.exist_hold_call_num--;
                }
            }
        }
        
        if( ( BUILD_MPTY_AND_ACCEPT == s_call_context.operate_waiting_call ) &&
            ( s_call_context.exist_call.exist_incoming_call ) )
        {
            
            //hold the MPTY for accepting incoming call
            //            if( MN_RETURN_SUCCESS != MNCALL_HoldCall( sig_ptr->call_id ) )
            if( MN_RETURN_SUCCESS != MNCALL_HoldCallEx(s_call_context.dual_sys, sig_ptr->call_id ) )
            {
                //SCI_TRACE_LOW:"CC_HandleBuildMPTYCnf: MNCALL_HoldCall is fault"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5334_112_2_18_2_10_17_156,(uint8*)"");
                return;
            }
            
            s_call_context.operate_waiting_call = HOLD_ALL_ACTIVE_AND_ACCEPT;
            s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_HOLD_OPER;
        }
        else
        {
            //the MPTY is in active state
            s_call_context.call_info[ s_call_context.current_call  ].operated_by_user = CC_NO_OPER;
        }
        MMICC_EnableMute(MMICC_IsCurrentCallMute());
        //update the prompt in the cc window
        MMICC_UpdateCallStatusDisplay(MMICC_BUILD_MPTY_SUC_IND);
    }
    else
    {
        s_call_context.current_call = FindCallInModule();
        MMICC_UpdateCallStatusDisplay(MMICC_BUILD_MPTY_FAIL_IND);

    }
//合并成为多方通话后需要通知蓝牙更新状态
#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetCallHeldStatus();
#endif
}

/*****************************************************************************/
// 	Description : check is exist outgoing call or not
//	Global resource dependence : s_call_context
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistOutgoingCall(void)
{
    BOOLEAN ret = FALSE;
    uint8 is_phone = 1;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif

    if(1 == is_phone)
    {
        ret = s_call_context.exist_call.exist_outgoing_call;
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT    
        ret = ual_bt_is_exist_outgoing_call();
#endif
    }
#ifndef WIN32
    SCI_TRACE_LOW("%s: ret = %d ", __FUNCTION__,ret);
#endif

    return ret;
}

/*****************************************************************************/
// 	Description : check is exist incomming call or not
//	Global resource dependence : s_call_context
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistIncommingCall(void)
{
    BOOLEAN ret = FALSE;
    uint8 is_phone = 1;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif

    if(1 == is_phone)
    {
        ret = s_call_context.exist_call.exist_incoming_call;
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT
        ret = ual_bt_is_exist_incoming_call();
#endif
    }
#ifndef WIN32
    SCI_TRACE_LOW("%s: ret = %d ", __FUNCTION__,ret);
#endif
    return ret;
}


/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveMPTY(void)
{
    //SCI_TRACE_LOW("ccapp.c:enter function IsExistMPTY()");
    
    if (1 < s_call_context.exist_call.exist_active_call_num)
    {
        return TRUE; 
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistHoldMPTY(void)
{
    //SCI_TRACE_LOW("ccapp.c:enter function IsExistMPTY()");
    
    if (1 < s_call_context.exist_call.exist_hold_call_num)
    {
        return TRUE; 
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//Description : check is exist active call or not
//Global resource dependence : s_call_context
//Author:jibin
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveCall(void)
{
    uint8 is_phone = 1;
    BOOLEAN ret = FALSE;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif

    if(1 == is_phone)
    {
        if (0 < s_call_context.exist_call.exist_active_call_num)
        {
            ret = TRUE;
        }
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT
        if(TRUE == ual_bt_is_exist_active_call())
        {
            ret = TRUE;
        }
#endif
    }
#ifndef WIN32
    SCI_TRACE_LOW("%s: ret = %d ", __FUNCTION__,ret);
#endif
    return ret;
}

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistHoldCall(void)
{
    uint8 is_phone = 1;
    BOOLEAN ret = FALSE;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif

    if(1 == is_phone)
    {
        if (0 < s_call_context.exist_call.exist_hold_call_num)
        {
            ret = TRUE;
        }
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT
        ret = ual_bt_is_exist_hold_call();
#endif
    }
#ifndef WIN32
    SCI_TRACE_LOW("%s: ret = %d ", __FUNCTION__,ret);
#endif
    return ret;
}

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistActiveAndHoldCalls(void)
{
    if (0 < s_call_context.exist_call.exist_active_call_num
        && 0 < s_call_context.exist_call.exist_hold_call_num)
    {
        return TRUE; 
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : check is exist MPTY or not
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistMPTY(void)
{
    uint8 is_phone = 1;
    BOOLEAN ret = FALSE;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif

    if(1 == is_phone)
    {
        if (MMICC_IsExistActiveMPTY()
        || MMICC_IsExistHoldMPTY())
        {
            ret = TRUE; 
        }
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT
        ret = ual_bt_is_exist_mpty_call();
#endif
    }
#ifndef WIN32
    SCI_TRACE_LOW("%s: ret = %d ", __FUNCTION__,ret);
#endif
    return ret;
}
/*****************************************************************************/
// 	Description : process to spilt call from MPTY 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note: must is exist mpty! must only 1 state!
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SplitSpecCall(
                                   uint8 call_index
                                   )
{
    uint8       i = 0;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_SplitSpecCall(), call_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5492_112_2_18_2_10_17_157,(uint8*)"d", call_index);
    
    //check the call id is vaild
    if (call_index >= CC_MAX_CALL_NUM || s_call_context.call_info[call_index].call_id >= CC_MAX_CALL_NUM || !MMICC_IsExistMPTY())
    {
        //SCI_TRACE_LOW:"ccapp CC_SplitSpecCall error call_id:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5497_112_2_18_2_10_17_158,(uint8*)"d", call_index);
        return CC_RESULT_ERROR;
    }
    //must only 1 state
    if (CC_NO_NUM != s_call_context.exist_call.exist_active_call_num
        && CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num)
    {
        //SCI_TRACE_LOW:"ccapp CC_SplitSpecCall error active:%d, hold:%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5504_112_2_18_2_10_17_159,(uint8*)"dd", s_call_context.exist_call.exist_active_call_num, s_call_context.exist_call.exist_hold_call_num);
        return CC_RESULT_ERROR;
    }

        //split the prointed call
    if( MN_RETURN_SUCCESS != MNCALL_SplitMPTYEx(s_call_context.dual_sys, s_call_context.call_info[call_index].call_id ) )
    {
        //SCI_TRACE_LOW:"CC_SplitSpecCall: MNCALL_SplitMPTY return failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5511_112_2_18_2_10_17_160,(uint8*)"");
        return CC_RESULT_ERROR;
    }
    s_call_context.call_info[i].operated_by_user = CC_SPLIT_OPER;
    s_call_context.current_call = i;
    //update the call state in the cc window
    MMICC_UpdateCurrentCallProcessing();    //call processing
        
    return CC_RESULT_SUCCESS;

}

/*****************************************************************************/
// 	Description : process to handle the confirm for split call 
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL void CC_HandleSplitCallCnf(
                                 DPARAM param
                                 )
{
    APP_MN_SPLIT_MPTY_CNF_T *sig_ptr = (APP_MN_SPLIT_MPTY_CNF_T*)param;
    uint8           call_index = 0;
    uint8           i = 0;
    uint8           sum_call = 0;
    
    if(PNULL == sig_ptr)
    {
        //SCI_TRACE_LOW:"CC_HandleSpecCallCnf( param == NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5538_112_2_18_2_10_18_161,(uint8*)"");
        return;
    }
    
    call_index = CC_GetIndexOfCallId(sig_ptr->dual_sys, sig_ptr->call_id );
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_HandleSplitCallCnf(), call_id = %d, req_is_accepted = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5544_112_2_18_2_10_18_162,(uint8*)"dd", sig_ptr->call_id, sig_ptr->req_is_accepted);
    if (sig_ptr->call_id >= CC_MAX_CALL_NUM || call_index >= s_call_context.call_number)
    {
        //SCI_TRACE_LOW:"ccapp error sig_ptr->call_id = %d, call_index = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5547_112_2_18_2_10_18_163,(uint8*)"dd", sig_ptr->call_id, call_index);
        return;
    }

    if( sig_ptr->req_is_accepted )
    {
        //modify the call state in the MPTY
        for( i = 0; i < s_call_context.call_number; i++ )
        {
            if( s_call_context.call_info[i].is_in_mpty )
            {
                if( call_index == i )
                {
                    s_call_context.call_info[i].call_state = CC_CALL_STATE;
                    s_call_context.call_info[i].is_in_mpty = FALSE;
                    s_call_context.current_call = call_index;
                }
                else
                {
                    s_call_context.call_info[i].call_state = CC_HOLD_STATE;
                    if (2 == s_call_context.call_number)
                    {
                        //modify the last call in MPTY as not a MPTY call
                        s_call_context.call_info[i].is_in_mpty = FALSE;
                    }
                }
                s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
            }
        }
        //modify the information in the s_call_context
        sum_call = s_call_context.exist_call.exist_active_call_num + s_call_context.exist_call.exist_hold_call_num ;
        s_call_context.exist_call.exist_active_call_num = 1;
        s_call_context.exist_call.exist_hold_call_num  = sum_call - 1;
        MMICC_EnableMute(MMICC_IsCurrentCallMute());
        MMICC_UpdateCallStatusDisplay(MMICC_SPLIT_MPTY_SUC_IND);
        }
    else
    {
        if(call_index >= CC_MAX_CALL_NUM)
        {
            //SCI_TRACE_LOW:"ccapp HandleSplitCallCnf call_index: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5587_112_2_18_2_10_18_164,(uint8*)"d", call_index);
            return ;
        }   
        
        //if(call_index < CC_MAX_CALL_NUM)
        //{
        //    s_call_context.call_info[ call_index ].operated_by_user = CC_NO_OPER;
        //}
        //modify the call state in the MPTY
        for( i = 0; i < s_call_context.call_number; i++ )
        {
            if( s_call_context.call_info[i].is_in_mpty )
            {
                s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
            }
        }

        s_call_context.current_call = FindCallInModule();
        MMICC_UpdateCallStatusDisplay(MMICC_SPLIT_MPTY_FAIL_IND);
    }
}

/*****************************************************************************/
// 	Description : 是否存在正在建立mpty，分离mpty，断开等call
//	Global resource dependence : s_call_context
//  Author:bin.ji
//	Note:2004.11.26
/*****************************************************************************/
LOCAL BOOLEAN CC_IsExistOperatedCall(void)
{
    //check if exist operated call
    if (CC_NO_NUM != s_call_context.call_number
        && CC_IsCallIDValid(s_call_context.current_call)
        && CC_NO_OPER != s_call_context.call_info[s_call_context.current_call].operated_by_user)
    {
        return TRUE;
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : 是否存在正在建立mpty，分离mpty，断开等call
//	Global resource dependence : s_call_context
//  Author:bin.ji
//	Note:2004.11.26
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistOperatedCall(void)
{
    return CC_IsExistOperatedCall();
}

#if 0
/*****************************************************************************/
// 	Description : 同步工具run的时候来电不接
//	Global resource dependence : s_call_context
//  Author:bin.ji
//	Note:2004.11.26
/*****************************************************************************/
LOCAL BOOLEAN CC_MTCallAtcRun( 
                              uint8 call_id
                              )
{
    if (MMIAPIUdisk_VUARTIsRun()|| MMIAPIBT_DUNIsRun())//DUN Using
    {
        return TRUE;
    }
    return FALSE;
}
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
// 	Description : process to disconnect the call which is  satisfied
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_MTCallFireWall ( 
                                 MN_DUAL_SYS_E		dual_sys,
                                 MN_CALLING_NUMBER_T *call_num_ptr, 
                                 uint8               call_id,
                                 MN_CALL_TYPE_E call_type,
                                 uint8 line
                                 )
{
    uint8				copy_len = 0;
    MMIPB_BCD_NUMBER_T	call_num = {0};
    BOOLEAN				result = FALSE;
    MMICL_CALL_INFO_T	call_info = {0};/*lint !e64*/
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_MTCallFireWall()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5667_112_2_18_2_10_18_165,(uint8*)"");
    
    if(PNULL == call_num_ptr ||call_id >= CC_MAX_CALL_NUM)
    {
        //SCI_TRACE_LOW:"ccapp.c: CC_MTCallFireWall() call_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5671_112_2_18_2_10_18_166,(uint8*)"d",call_id);
        return FALSE;
    }
    
    SCI_MEMSET(&call_num, 0, sizeof(call_num));
    call_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(call_num_ptr->number_type, call_num_ptr->number_plan );
    
    copy_len = ( call_num_ptr->num_len > MMIPB_BCD_NUMBER_MAX_LEN) ? MMIPB_BCD_NUMBER_MAX_LEN: call_num_ptr->num_len;
    MMI_MEMCPY(
        call_num.number,
        sizeof(call_num.number),
        (uint8*)call_num_ptr->party_num,
        sizeof(call_num_ptr->party_num),
        copy_len
        );
    call_num.number_len = copy_len;
    
    //对号码进行解析      
    //result = MMIAPIPB_IsMTFWNumber(&call_num);
    result = MMIAPISET_IsCCBlacklist(&call_num,call_type);
    
    if( result )    //匹配
    {
        SCI_MEMSET(&call_info, 0, sizeof(call_info));
        //copy call_info.call_time
        call_info.call_time.start_time = call_info.call_time.stop_time
            = MMIAPICOM_GetCurTime();
        //copy call_info.call_num.is_num_present
        if(call_num_ptr->num_len > 0)
        {
            call_info.call_num.is_num_present = TRUE;
        }
        else
        {
            call_info.call_num.is_num_present = FALSE;
        }	  
        // call type
        if (MN_CALL_TYPE_DATA == call_type)
        {
            call_info.call_type = CC_CALL_VIDEO;
        }
        else
        {
            call_info.call_type = CC_CALL_NORMAL;
        }	  
        //copy call_info.call_num.number
        call_info.call_num.number.num_len       = call_num_ptr->num_len;
        call_info.call_num.number.number_plan   = call_num_ptr->number_plan;
        call_info.call_num.number.number_type   = call_num_ptr->number_type;
        MMI_MEMCPY(
            call_info.call_num.number.party_num,
            MN_MAX_ADDR_BCD_LEN,
            call_num_ptr->party_num,
            MN_MAX_ADDR_BCD_LEN,
            call_num_ptr->num_len
            );          
#ifdef BT_DIALER_SUPPORT
        if(!MMICC_GetBtCallFlag())
#endif
 	    {
#ifdef MMI_CSP_SUPPORT
            call_info.line = line;
#endif
 	    }  
        MMIAPICL_RecordCallInfo(dual_sys, MMICL_CALL_REFUSED, call_info);
        return TRUE;                    
    }
    else
    {
        return FALSE;
    }
}
#endif

/*****************************************************************************/
// 	Description : whether the call need to dial again
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_AutoMoCall ( 
                             uint8                call_index,
                             MN_CALL_DISCONNECT_CALL_CAUSE_E cause 
                             )
{
    BOOLEAN		result = FALSE;
    uint16 copy_len = 0;
    
    //SCI_TRACE_LOW:"CC_AutoMoCall: call_index = %d, cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5750_112_2_18_2_10_18_167,(uint8*)"dd",call_index,  cause);
    //SCI_TRACE_LOW:"CC_AutoMoCall: call_state = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5751_112_2_18_2_10_18_168,(uint8*)"d", s_call_context.call_info[call_index].call_state);
    
    if( (MMIAPISET_GetIsAutoRedail() || MMIAPISTK_GetSetupCallDuration())&&
        ( CC_CALLING_STATE == s_call_context.call_info[call_index].call_state ) )
    {
        if (CC_NO_OPER == s_call_context.call_info[call_index].operated_by_user) //对方或网络挂断
        {
            //恢复want_mo_call
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
            copy_len = s_call_context.call_info[call_index].name_len;
#else
            copy_len = ( s_call_context.call_info[call_index].name_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : s_call_context.call_info[call_index].name_len;/*lint !e506 */
#endif
            MMI_MEMCPY(
                s_call_context.want_mo_call.name,
                sizeof(s_call_context.want_mo_call.name),
                s_call_context.call_info[call_index].name,
                sizeof(s_call_context.call_info[call_index].name),
                copy_len
                );
            s_call_context.want_mo_call.name_len = copy_len;
            s_call_context.want_mo_call.address = s_call_context.call_info[call_index].address;
            s_call_context.want_mo_call.is_want_mo_call = TRUE;
            
            MMICC_SetAutoDialTimesMax(10);
            result =  TRUE;                    
        }
        else
        {
            MMICC_SetAutoDialTimesMax(0);
            result = FALSE;
        }        
    }
    else
    {
        MMICC_SetAutoDialTimesMax(0);
        result = FALSE;
    }
    //SCI_TRACE_LOW:"CC_AutoMoCall: Need auto dial = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5789_112_2_18_2_10_18_169,(uint8*)"d",result);
    return result;
}


/*****************************************************************************/
// 	Description : search the call which state is pointed by param
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
LOCAL uint8   FindStateCall(
                            CC_CALL_STATE_E  state 
                            )
{
    uint8   i = 0;
    
    //SCI_TRACE_LOW("ccapp.c:enter function FindStateCall()");
    
    for( i = 0; i < s_call_context.call_number ; i++ )
    {
        if( state == s_call_context.call_info[ i ].call_state )
        {
            break;
        }
    }
    
    return i;
}

/*****************************************************************************/
// 	Description : search the call which state is pointed by param
//	Global resource dependence : 
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC uint8   MMICC_FindStateCall(
                            CC_CALL_STATE_E  state 
                            )
{
    return FindStateCall(state);
}   

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E SendDtmfByChar(MN_DUAL_SYS_E dual_sys, uint8 dtmf_ch, BOOLEAN is_stop_immediately)
{
    CC_RESULT_E result = CC_RESULT_ERROR;
    MN_DUAL_SYS_E dtmf_dual_sys = MN_DUAL_SYS_MAX;

    //check the param
    if (!CC_IsCallIDValid(s_call_context.current_call))
    {
        //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_SendDtmfByChar() current_call invalid %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5841_112_2_18_2_10_18_170,(uint8*)"d", s_call_context.current_call);
        return CC_RESULT_ERROR;
    }
    
    if (dual_sys == MN_DUAL_SYS_MAX)
    {
        dtmf_dual_sys = s_call_context.dual_sys;
    }
    else
    {
        dtmf_dual_sys = dual_sys;
    }

    if(!((dtmf_ch >= '0' && dtmf_ch <= '9') || 
         (dtmf_ch >= 'A' && dtmf_ch <= 'D') ||
          dtmf_ch == '#' ||
          dtmf_ch =='*')
        )
    {
        dtmf_ch = CC_INVALID_DTMF_VALUE;
    }
    
    //if the pressed key is valid
    if(CC_INVALID_DTMF_VALUE != dtmf_ch)
    {
#if !defined(MMI_DIALPANEL_DTMF_SUPPORT)
        //播放tone音
        if(CC_INCOMING_CALL_STATE != s_call_context.call_info[s_call_context.current_call].call_state)
        {
            MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, MMI_NT_RING_CON_ID, MMI_DTMF_TONE_PLAY_TIME,MMISRVAUD_TONE_FREQ_DEFAULT);
        }
#endif

        if (s_call_context.call_info[s_call_context.current_call].is_dtmf_sent)
        {
            StopDtmfByChar(dtmf_dual_sys);
        }

        //start send dtmf
         /* fix compile error, modified by zhigang.peng 2019-07-15 */
        if(MN_RETURN_SUCCESS != MNCALL_StartDTMFEx(dtmf_dual_sys, dtmf_ch, s_call_context.call_info[s_call_context.current_call].call_id,FALSE,0,0))
        {
            //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_SendDtmfByChar() start dtmf failure"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5877_112_2_18_2_10_18_171,(uint8*)"");
            return CC_RESULT_ERROR;
        }

        s_call_context.call_info[s_call_context.current_call].is_dtmf_sent = TRUE;

        if (is_stop_immediately)
        {
            StopDtmfByChar(dtmf_dual_sys);
        }
        else
        {
            MMIICC_StartDtmfSendingTimer();
        }

        return CC_RESULT_SUCCESS;
    }

    return result;
}

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E StopDtmfByChar(MN_DUAL_SYS_E dual_sys)
{
    MN_DUAL_SYS_E dtmf_dual_sys = MN_DUAL_SYS_MAX;

    //check the param
    if (!CC_IsCallIDValid(s_call_context.current_call))
    {
        SCI_TRACE_LOW("ccapp.c:StopDtmfByChar() current_call invalid %d", s_call_context.current_call);

        return CC_RESULT_ERROR;
    }

    MMIICC_StopDtmfSendingTimer();

    if (dual_sys == MN_DUAL_SYS_MAX)
    {
        dtmf_dual_sys = s_call_context.dual_sys;
    }
    else
    {
        dtmf_dual_sys = dual_sys;
    }

    if (s_call_context.call_info[s_call_context.current_call].is_dtmf_sent)
    {
        s_call_context.call_info[s_call_context.current_call].is_dtmf_sent = FALSE;
    
        //stop send dtmf
        if (MN_RETURN_SUCCESS != MNCALL_StopDTMFEx(dtmf_dual_sys, s_call_context.call_info[s_call_context.current_call].call_id))
        {    
            //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_SendDtmfByChar() stop dtmf failure"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5884_112_2_18_2_10_18_172,(uint8*)"");

            return CC_RESULT_ERROR;
        }
    }

    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_SendDtmfByChar(MN_DUAL_SYS_E dual_sys, uint8 dtmf_ch)
{
    if (MMICC_IsAllowSendDtmf())
    {
        return SendDtmfByChar(dual_sys, dtmf_ch, FALSE);
    }
    else
    {
        return CC_RESULT_ERROR;
    }
}

/*****************************************************************************/
// 	Description : stop dtmf
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_StopDtmf(MN_DUAL_SYS_E dual_sys)
{
    if (MMICC_IsAllowSendDtmf())
    {
        return StopDtmfByChar(dual_sys);
    }
    else
    {
        return CC_RESULT_ERROR;
    }
}

/***************************Handle window and control****************************/


/*****************************************************************************/
//  Description : check is in call state, include incoming and outgoing,connected
//  Global resource dependence : none
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsInState(CC_CALL_STATE_FOR_USING_E call_state)
{
    BOOLEAN result = FALSE;
    
    if (s_call_context.exist_call.exist_mtfw && (1 >= s_call_context.call_number) )
    {
        //SCI_TRACE_LOW:"mmiapp.c: MMIAPICC_IsInState call is rejected by black list"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_5921_112_2_18_2_10_18_173,(uint8*)"");
        return FALSE;
    }

    switch(call_state)
    {
    case CC_IN_CALL_STATE:
        if (
            //cc applet 采用异步释放导致一些模块恢复播放有问题
            MMICC_IsAudioHandleEnable()//PNULL != MMICC_GetAppletInstance() 
#ifdef VT_SUPPORT
            ||(MMIAPIVT_IsVtCalling())
#endif
            )
        {
            return TRUE;
        }
        break;
        
    case CC_MO_CONNECTING_STATE:
        if ((FALSE != MMICC_IsExistOutgoingCall())
#ifdef VT_SUPPORT
            ||MMIAPIVT_IsVtMoCalling()
#endif
            )
        {
            return TRUE;
        }
        break;
    case CC_MT_CONNECTING_STATE:
        if ((FALSE != MMICC_IsExistIncommingCall())
#ifdef VT_SUPPORT
            ||MMIAPIVT_IsVtMtIncoming()
#endif
            )
        {
            return TRUE;
        }
        break;
    case CC_CALL_CONNECTED_STATE:
        if ((FALSE != MMICC_IsExistActiveCall()||
            FALSE != MMICC_IsExistHoldCall())
#ifdef VT_SUPPORT
            ||MMIAPIVT_IsVtConnecting()
#endif
            )
        {
            return TRUE;
        }
        break;
    case CC_CALL_DISCONNECTED_STATE:
        if(MMK_IsOpenWin(MMICC_DISCONNECT_WIN_ID) || MMK_IsOpenWin(WATCHCC_CALL_ENDED_WIN_ID))
        {
            return TRUE;
        }
        break;
        
    default:
        break;
    }
    SCI_TRACE_LOW("ccapp MMIAPICC_IsInState, call_state = %d result = %d", call_state, result);
    return result;
}


//@zhaohui add
/*****************************************************************************/
// 	Description : to get the name of call
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCallName( 
                                 //					   uint8		call_id,//暂时屏蔽，不考虑vt多方通话
                                 MMI_STRING_T	*name_str_ptr//OUT
                                 )
{
    if( s_call_context.call_info[s_call_context.current_call].name_len > 0)
    {
        name_str_ptr->wstr_len =  s_call_context.call_info[s_call_context.current_call].name_len;
        name_str_ptr->wstr_ptr = s_call_context.call_info[s_call_context.current_call].name;
    }
    else
    {
        MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  name_str_ptr);
    }
}
/*****************************************************************************/
// 	Description : make call by adding ip number
//	Global resource dependence : 
//  Author:lin.lin
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN CheckIsHavingCountryCode(const uint8* number_ptr, uint8 *country_code_length)
{
    BOOLEAN is_having = FALSE;
    
    *country_code_length = 0;
    
    // check if it begins with country code
    if(strncmp((const char*)number_ptr, "+86", 3) == 0)
    {
        is_having = TRUE;
        *country_code_length = 3;
    }
    
    return is_having;
}

/*****************************************************************************/
//  Description : get current call number
//  Global resource dependence : none
//  Author: baokun
//  Note: 
/*****************************************************************************/
LOCAL void GetCurrentCallNumber(MMIPB_BCD_NUMBER_T *bcd_number_ptr, BOOLEAN is_remove_ip_head, BOOLEAN is_want_mo_call)
{
    int                 origin_num_len                         = 0;
    MN_CALLED_NUMBER_T *address_ptr = PNULL; 
    MMI_PARTY_NUMBER_T		party_num                              = {0};/*lint !e64*/
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8 tele_number_no_ip[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint32 tele_len = 0;
    
    if(PNULL == bcd_number_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function GetCurrentCallNumber(), bcd_number_ptr = NULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6041_112_2_18_2_10_19_175,(uint8*)"");
        return;
    }
    if(is_want_mo_call && s_call_context.want_mo_call.is_want_mo_call)
    {
        //MO call, get MO Phone number 
        address_ptr = &(s_call_context.want_mo_call.address);
    }
    else
    {
        address_ptr = &(s_call_context.call_info[s_call_context.current_call].address); 
    }
    
    if (is_remove_ip_head)
    {
        if (s_call_context.want_mo_call.is_want_mo_call
            || CC_MO_CALL_TYPE == s_call_context.call_info[s_call_context.current_call].call_type)
        {
            tele_len = MMIAPICOM_GenDispNumber(address_ptr->number_type, 
                (uint8)address_ptr->num_len, 
                address_ptr->party_num, 
                tele_num, 
                CC_MAX_TELE_NUM_LEN + 2);
        }
        else
        {
            tele_len = MMIAPICOM_GenNetDispNumber(address_ptr->number_type, 
                (uint8)address_ptr->num_len, 
                address_ptr->party_num, 
                tele_num, 
                CC_MAX_TELE_NUM_LEN + 2);
        }
        
        if (MMIAPICC_RemoveIpNumber(tele_num, tele_len, tele_number_no_ip,CC_MAX_TELE_NUM_LEN))
        {
            SCI_MEMSET(bcd_number_ptr, 0x00, sizeof(MMIPB_BCD_NUMBER_T));

            origin_num_len = strlen((char*)tele_number_no_ip);
            
            if(MMIAPICOM_GenPartyNumber(tele_number_no_ip, origin_num_len, &party_num))
            {
                bcd_number_ptr->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                bcd_number_ptr->number_len = MIN(MN_MAX_ADDR_BCD_LEN, party_num.num_len);
                MMI_MEMCPY(
                    bcd_number_ptr->number,
                    sizeof(bcd_number_ptr->number),
                    (void*)party_num.bcd_num,
                    sizeof(party_num.bcd_num),
                    party_num.num_len
                    );
                return ;
            }
        }
    }

    bcd_number_ptr->npi_ton = MMIAPICOM_ChangeTypePlanToUint8(address_ptr->number_type, MN_NUM_PLAN_UNKNOW);
    bcd_number_ptr->number_len = MIN(MN_MAX_ADDR_BCD_LEN, address_ptr->num_len);
    MMI_MEMCPY(
        bcd_number_ptr->number,
        sizeof(bcd_number_ptr->number),
        (void*)address_ptr->party_num,
        sizeof(address_ptr->party_num),
        address_ptr->num_len
        );	
}
/*****************************************************************************/
//  Description : get current call number
//  Global resource dependence : none
//  Author: baokun
//  Note: 
/*****************************************************************************/
PUBLIC void MMICC_GetCurrentCallNumber(MMIPB_BCD_NUMBER_T *bcd_number_ptr, BOOLEAN is_remove_ip_head, BOOLEAN is_want_mo_call)
{
    GetCurrentCallNumber(bcd_number_ptr, is_remove_ip_head, is_want_mo_call);
}

/*****************************************************************************/
//  Description : clear missed call number
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ClearMissedCallNum(void)
{
    s_call_context.missed_call_num = 0;
    MAIN_SetMissedCallState(FALSE);
    MMIAPICC_SaveMissedCallNumToNv();
    if(MMK_IsOpenWin(MMICC_MISSCALL_WIN_ID))
    {
        MMK_CloseWin(MMICC_MISSCALL_WIN_ID);
    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : handle MT Call disconnect
//	Global resource dependence : 
//  Author:figo.feng
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleMTCallDisc(DPARAM param)
{
    uint8 line = 0;    
    APP_MN_SETUP_IND_T *pSig = (APP_MN_SETUP_IND_T*)param;
    
    line = (MN_BEARER_CAPABILITY_LINE_2 == pSig->bc_type)?1:0;
    if( pSig->calling_num_present)
    {
#ifdef DPHONE_SUPPORT
    /*Begin of yanqi on 2010-Mar-31 for 摘机不能来电*/
        if( CC_IsExistOperatedCall()
#if defined(MMI_BLACKLIST_SUPPORT)
            || CC_MTCallFireWall(pSig->dual_sys, &pSig->calling_num, pSig->call_id, pSig->call_type, line) 
#endif
#if defined(MET_MEX_SUPPORT) && defined(MET_MEX_AQ_SUPPORT)
            || MMIMEX_AQPhoneWall(pSig->dual_sys, &pSig->calling_num)
#endif
            || ((MMIDEFAULT_GetHookStates()||MMIDEFAULT_GetHandFreeStates())&&(!MMIAPICC_IsInState(CC_IN_CALL_STATE))))
    /*End of yanqi on 2010-Mar-31*/
#else
        if( CC_IsExistOperatedCall()
#if defined(MMI_BLACKLIST_SUPPORT)
            || CC_MTCallFireWall(pSig->dual_sys, &pSig->calling_num, pSig->call_id, pSig->call_type, line)
#endif
            )
#endif
	{
            //SCI_TRACE_LOW:"CC_HandleMTCallDisc: the call is disconnected"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6155_112_2_18_2_10_19_176,(uint8*)"");
          
            MNCALL_DisconnectCallEx(pSig->dual_sys, pSig->call_id, MN_CAUSE_USER_BUSY, NULL);

            if (s_call_context.exist_call.exist_outgoing_call)
            {
                return TRUE;
            }
            s_call_context.call_number++;
            SCI_MEMSET(
                &s_call_context.call_info[s_call_context.call_number - 1],
                0x00,
                sizeof(s_call_context.call_info[s_call_context.call_number - 1]));
            s_call_context.call_info[s_call_context.call_number - 1].call_type = CC_MT_CALL_TYPE;
            s_call_context.call_info[s_call_context.call_number - 1].call_state = CC_INIT_STATE;
            s_call_context.call_info[s_call_context.call_number - 1].is_in_mpty = FALSE;
            s_call_context.call_info[s_call_context.call_number - 1].is_mtfw_reject = TRUE;
            s_call_context.call_info[s_call_context.call_number - 1].call_id = pSig->call_id;
            s_call_context.call_info[s_call_context.call_number - 1].dual_sys= pSig->dual_sys;

            s_call_context.exist_call.exist_mtfw = TRUE;
            
            return TRUE;
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
// 	Description : handle MT Call disconnect
//	Global resource dependence : 
//  Author:figo.feng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_HandleMTCallDisc(DPARAM param)
{
    return HandleMTCallDisc(param);
}


/*****************************************************************************/
//  Description :ShowCallDiscCause
//  Global resource dependence : none
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC CC_MN_CALL_DISC_CAUSE_E MMICC_ShowCallDiscCause(
                                                MN_CALL_DISCONNECT_CALL_CAUSE_E cause
                                                )
{
    CC_MN_CALL_DISC_CAUSE_E cause_type = CC_DISC_CAUSE_NORMAL;
    
    //normal cause
    if (cause <= MN_CAUSE_NORMAL_UNSPECIFIED)
    {
        switch(cause)
        {
            //cause by normal disconnect
        case MN_CAUSE_NORMAL_CLEARING:
        case MN_CAUSE_NORMAL_UNSPECIFIED:
            cause_type = CC_DISC_CAUSE_NORMAL;
            break;
            
            //cause by user busy
        case MN_CAUSE_USER_BUSY:
            cause_type = CC_DISC_CAUSE_USER_BUSY;
            break;
            
            //cause by user not responding
        case MN_CAUSE_NO_USER_RESPONDING:
        case MN_CAUSE_ALERTING_NO_ANSWER:
            cause_type = CC_DISC_CAUSE_USER_NOT_RESPOND;
            break;
            
            //cause by error number
        case MN_CAUSE_UNASSIGNED_NO:
        case MN_CAUSE_NUMBER_CHANGED:
        case MN_CAUSE_INVALID_NO_FORMAT:
            cause_type = CC_DISC_CAUSE_ERROR_NUMBER;
            break;
            
            //cause by cannot connect to user
        case MN_CAUSE_NO_ROUTE_TO_DEST:
        case MN_CAUSE_CHAN_UNACCEPTABLE:
        case MN_CAUSE_OPER_DETERM_BARRING:
        case MN_CAUSE_NONSEL_USER_CLRING:
        case MN_CAUSE_DEST_OUT_OF_ORDER:
        case MN_CAUSE_FACILITY_REJECTED:
        case MN_CAUSE_RSP_TO_STATUS_ENQ:
        case MN_CAUSE_CALL_REJECTED:
            cause_type = CC_DISC_CAUSE_CALL_REJECTED;
            break;
            
        default:
            break;
        }
    }
    // Resource Unavailable
    else if ((MN_CAUSE_NORMAL_UNSPECIFIED < cause) && (cause <= MN_CAUSE_RESOURCES_UNAV))
    {
        cause_type = CC_DISC_CAUSE_RES_UNAV;
    }
    else if (MN_CAUSE_REQ_FAC_NOT_SUBSCR == cause && MN_CALL_CLIR_INVOCATION == MMIAPISET_GetClirType(s_call_context.dual_sys))
    {
        cause_type = CC_DISC_CAUSE_HIDE_ID;
    }
    // Service or Option Not Available
    else if ((MN_CAUSE_RESOURCES_UNAV < cause) && (cause <= MN_CAUSE_SERV_OPT_UNAV))
    {
        cause_type = CC_DISC_CAUSE_SERVICE_UNAV;
    }
    // Call beings exceed limit
    else if (MN_CAUSE_ACM_EQ_OR_GT_ACMMAX == cause)
    {
        cause_type = CC_DISC_CAUSE_BEING_EXCEED_LIMIT;
    }
    // Service or Option Not Implement
    else if ((MN_CAUSE_SERV_OPT_UNAV < cause) && (cause <= MN_CAUSE_SVC_OPT_NOT_IMPL))
    {
        cause_type = CC_DISC_CAUSE_SERVICE_NOT_IMP;
    }
    // Invalid Message
    else if ((MN_CAUSE_SVC_OPT_NOT_IMPL < cause) && (cause <= MN_CAUSE_INVALID_MSG_SEMANTIC))
    {
        cause_type = CC_DISC_CAUSE_INVALID_MES;
    }
    // Protocol Error
    else if ((MN_CAUSE_INVALID_MSG_SEMANTIC < cause) && (cause <= MN_CAUSE_PROTOCOL_ERROR))
    {
        cause_type = CC_DISC_CAUSE_PROTOCOL_ERROR;
    }
    // Network Error
    else if ((MN_CAUSE_PROTOCOL_ERROR < cause) && (cause <= MN_CAUSE_INTERWORKING))
    {
        cause_type = CC_DISC_CAUSE_NETWORK_ERROR;
    }
    else
    {
        cause_type = CC_DISC_CAUSE_NORMAL;
    }
    
    //SCI_TRACE_LOW:"ccapp ShowCallDiscCause:cause type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6293_112_2_18_2_10_19_177,(uint8*)"d", cause_type);
    
    return cause_type;
}

/*****************************************************************************/
// 	Description : 获取当前呼叫的类型
//	Global resource dependence : 
//  Author:xiongxiaoyan 
//	Note:20060404
/*****************************************************************************/
PUBLIC CC_CALL_CONTEXT_T* MMICC_GetCallText(void)
{
    return(&s_call_context);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsVideoCall(void)
{
#ifdef VT_SUPPORT
    return MMIAPIVT_IsVtCalling();
#else
    return FALSE;
#endif
}

/*****************************************************************************/
// 	Description : 获取当前通话时间信息
//	Global resource dependence : 
//  Author:xiongxiaoyan
//	Note:获取当前通话时间信息
/*****************************************************************************/
PUBLIC uint32 MMICC_GetCurrentCallTime(void)
{
    return s_call_context.call_time_count -s_call_context.call_info[s_call_context.current_call].start_call_time_count;
}
/*****************************************************************************/
// 	Description : 获取当前通话时间信息
//	Global resource dependence : 
//  Author:xiongxiaoyan
//	Note:获取当前通话时间信息
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCallTimeStr(MMI_STRING_T *prompt_str)
{	 
    uint8 time_str[CC_DISPLAY_TIME_LEN]={0};
    //通话中时间的计算:通过总时间减去该通电话
    // 的起始时间来获得
    uint32 cur_time_count = s_call_context.call_time_count -
        s_call_context.call_info[s_call_context.current_call].start_call_time_count;
    
    sprintf((char*)time_str,
        "%02d:%02d:%02d",
        (int)(cur_time_count / CC_HOUR_TO_SECOND),
        (int)((cur_time_count % CC_HOUR_TO_SECOND) / CC_MINUTE_TO_SECOND),
        (int)(cur_time_count % CC_MINUTE_TO_SECOND)
        );
    
    prompt_str->wstr_len = strlen((char*)time_str);
    //SCI_MEMCPY(prompt_str->wstr_ptr, time_str, strlen((char*)time_str));
    MMI_STRNTOWSTR(prompt_str->wstr_ptr, prompt_str->wstr_len, (uint8 *)time_str, CC_DISPLAY_TIME_LEN, prompt_str->wstr_len);
    return;
}


/*****************************************************************************/
// 	Description : close MMICC_DISCONNECT_WIN_ID
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:2007.10.19
/*****************************************************************************/
PUBLIC void MMIAPICC_CloseDisconnectWin(void)
{
    MMK_CloseWin(MMICC_DISCONNECT_WIN_ID);
}


/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_ConnectCallReq(void)
{
    CC_ConnectCallReq();
}

/*****************************************************************************/
// 	Description : 挂断当前答录电话
//	Global resource dependence : 
//  Author:koki gao
//	Note: 
/*****************************************************************************/
#ifdef ASP_SUPPORT
PUBLIC CC_RESULT_E MMIAPICC_ReleaseAspCallReq(MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    if((s_call_context.call_number == 0) || (s_call_context.call_number > CC_MAX_CALL_NUM))
    {
        return CC_RESULT_ERROR;
    }

    if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(s_call_context.dual_sys, s_asp_call_id, CC_ResetDisconnectCause(cause), PNULL))
    {
        //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_ReleaseAspCallReq() MNCALL_DiscCall return failure"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6387_112_2_18_2_10_19_178,(uint8*)"");
        return CC_RESULT_ERROR;
    }

    s_call_context.call_info[s_asp_call_id].operated_by_user = CC_RELEASE_OPER;
    return CC_RESULT_SUCCESS;
}
#endif

/*****************************************************************************/
// 	Description : 挂断当前电话
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseCurrentCallReq(MN_CALL_DISCONNECT_CALL_CAUSE_E cause)
{
    return CC_ReleaseCallReq(s_call_context.call_info[s_call_context.current_call].dual_sys, s_call_context.current_call, cause );
}
/*****************************************************************************/
// 	Description : making redial call , video call or normal call
//	Global resource dependence : s_call_text
//  Author:samboo.shen
//	Note:2007.7.27
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMICC_MakeRedialCall( MN_DUAL_SYS_E dual_sys, uint8 *tele_num, uint8 tele_len)
{
    MMI_STRING_T name_str = {0};
    MMI_STRING_T *name_ptr = PNULL;
    wchar               str_buf[CC_MAX_PBNAME_LEN + 1] = {0};
                
    if (0 != s_call_context.want_mo_call.name_len)//this call is from a record
    {
        name_str.wstr_ptr = str_buf;
        if (s_call_context.want_mo_call.is_name_exist)
        {
            name_str.wstr_len = s_call_context.want_mo_call.name_len;
            MMI_WSTRNCPY(name_str.wstr_ptr, 
                                        CC_MAX_PBNAME_LEN, 
                                        s_call_context.want_mo_call.name, 
                                        sizeof(s_call_context.want_mo_call.name), 
                                        name_str.wstr_len);
        }
        name_ptr = &name_str;
    }
    else
    {
        name_ptr = PNULL;
    }

    //SCI_TRACE_LOW:"ccapp: auto dial normal call..."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6433_112_2_18_2_10_19_179,(uint8*)"");
    if( CC_RESULT_SUCCESS != MMIAPICC_MakeCall(
        dual_sys,
        tele_num,
        tele_len,
        PNULL,
        name_ptr,
        MMIAPICC_GetCallSimType(dual_sys),
        CC_CALL_NORMAL_CALL,
        PNULL
        ))
    {					
        return MMI_RESULT_FALSE;   
    }
    return MMI_RESULT_TRUE;
} 


/*****************************************************************************/
//  Description : if the disc cause is "unobtainable destination-permanent/long term"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是号码错误,参见GSM 02.07
/*****************************************************************************/
LOCAL BOOLEAN CC_IsValideDest(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause)
{
    BOOLEAN result = FALSE;
    
    //SCI_TRACE_LOW:"mmicc_app: CC_IsValideDest, disc_cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6459_112_2_18_2_10_19_180,(uint8*)"d", disc_cause);
    
    //以下几种情况自动重拨只拨出一次
    result = disc_cause == MN_CAUSE_UNASSIGNED_NO || 
        disc_cause == MN_CAUSE_NO_ROUTE_TO_DEST || 
        disc_cause == MN_CAUSE_NUMBER_CHANGED || 
        disc_cause == MN_CAUSE_INVALID_NO_FORMAT || 
        disc_cause == MN_CAUSE_NET_OUT_OF_ORDER ||
        disc_cause == MN_CAUSE_NORMAL_UNSPECIFIED; //协议上没有31, CTA 测试 空号有时返回31，只能重拨一次，统一的放到这里;
    
    return result;
}

/*****************************************************************************/
//  Description : if the disc cause is "unobtainable destination-permanent/long term"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是号码错误,参见GSM 02.07
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsValideDest(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause)
{
    return CC_IsValideDest(disc_cause);
}
/*****************************************************************************/
//  Description : if the disc cause is "busy destination" or "tempory"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是否时用户忙或者暂时无法接通,参见GSM 02.07
/*****************************************************************************/
LOCAL BOOLEAN CC_IsUserBusy(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause)
{
    BOOLEAN result = FALSE;
    //SCI_TRACE_LOW:"mmicc_app: CC_IsUserBusy, disc_cause = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6487_112_2_18_2_10_20_181,(uint8*)"d", disc_cause);
    
    if (disc_cause > MN_CAUSE_NORMAL_UNSPECIFIED && disc_cause < MN_CAUSE_RESOURCES_UNAV && disc_cause != MN_CAUSE_NET_OUT_OF_ORDER)
    {
        disc_cause = MN_CAUSE_RESOURCES_UNAV;
    }
    result = disc_cause == MN_CAUSE_USER_BUSY || 
        disc_cause == MN_CAUSE_NO_USER_RESPONDING || 
        disc_cause == MN_CAUSE_ALERTING_NO_ANSWER || 
        disc_cause == MN_CAUSE_DEST_OUT_OF_ORDER ||
        //现在MN_CAUSE_NO_CIRC_CHAN_AV，MN_CAUSE_TEMP_FAILURE，MN_CAUSE_SWITCH_CONGESTION统一的转化为MN_CAUSE_RESOURCES_UNAV，重拨10次
        disc_cause == MN_CAUSE_NO_CIRC_CHAN_AV ||
        disc_cause == MN_CAUSE_TEMP_FAILURE ||
        disc_cause == MN_CAUSE_SWITCH_CONGESTION ||
        disc_cause == MN_CAUSE_REQ_CIRC_CHAN_UNAV ||
        disc_cause == MN_CAUSE_RESOURCES_UNAV ;
    /*disc_cause == MN_CAUSE_NORMAL_UNSPECIFIED; //临时加上验证问题31，协议上没有31*/
    
    return result;   
}
/*****************************************************************************/
//  Description : if the disc cause is "busy destination" or "tempory"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是否时用户忙或者暂时无法接通,参见GSM 02.07
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsUserBusy(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause)
{
    return CC_IsUserBusy(disc_cause);
}



/*****************************************************************************/
// 	Description : get sim status when get call ready status msg
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_GetSimCallReady(MN_DUAL_SYS_E dual_sys)
{
    if(MMI_DUAL_SYS_MAX <= dual_sys)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function MMIAPICC_GetSimCallReady(), dual_sys >= MMI_DUAL_SYS_MAX "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6526_112_2_18_2_10_20_182,(uint8*)"");
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"ccapp: MMIAPICC_GetSimCallReady, s_call_ready[dual_sys] = %d, dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6530_112_2_18_2_10_20_183,(uint8*)"dd", s_call_ready[dual_sys], dual_sys);

    return s_call_ready[dual_sys];
}

/*****************************************************************************/
// 	Description : set sim status when switch card
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SetSimCallReady(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ready)
{
    //SCI_TRACE_LOW:"ccapp: MMIAPICC_SetSimCallReady, is_ready = %d, dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6544_112_2_18_2_10_20_184,(uint8*)"dd", is_ready, dual_sys);
    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        s_call_ready[dual_sys] = is_ready;
    }
    else
    {
        //SCI_TRACE_LOW:"ccapp: error dual_sys = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6551_112_2_18_2_10_20_185,(uint8*)"d", dual_sys);
    }
}

#ifdef MMI_IP_CALL_SUPPORT
/*****************************************************************************/
//  Description : get dialed ip num 
//  Global resource dependence : 
//  Author: bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_GetIPCallNum(MN_DUAL_SYS_E dual_sys, 
    uint8* tele_num, //[in]
    uint32 tele_len, //[in]
    uint8* phone_number, //[out]
    uint32  phone_num_len //[in]
    )
{
    uint8 ip_number[MMISET_IPNUMBER_MAX_LEN + 1] = {0};
    BOOLEAN is_having_country_code = FALSE;
    uint8 country_code_length = 0;
    //@fen.xie MS00179752
    uint8 ip_num_len = 0;
    uint32  actual_phone_num_len = 0;

    if(PNULL == tele_num)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function CC_GetIPCallNum(), tele_num is NULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6579_112_2_18_2_10_20_186,(uint8*)"");
        return FALSE;
    }
    
    MMIAPISET_GetActiveIPNumber(dual_sys, ip_number, (MMISET_IPNUMBER_MAX_LEN + 1));
    ip_num_len = (uint8)strlen((char*)ip_number);
    
    if (CheckIsHavingCountryCode((const uint8*)tele_num, &country_code_length))
    {
        is_having_country_code = TRUE;
    }
    
    //sprintf((char*)phone_number, "%s%s",(char*)ip_number, (char*)number_ptr);
    
    //@fen.xie MS00179752:当tele num +ip 超过CC_MAX_TELE_NUM_LEN 时,丢弃多余的phone number 再进行ip呼叫
    if(*tele_num != '+' || is_having_country_code)
    {
        actual_phone_num_len = MIN(phone_num_len, CC_MAX_TELE_NUM_LEN);//保护
        
        if(0 == strncmp((char *)tele_num, (char *)ip_number, ip_num_len))//tele_num中已经包含IP号码则直接拷贝 tele_num
        {
            actual_phone_num_len = MIN(actual_phone_num_len, tele_len);//保护
           
            MMI_MEMCPY(
                phone_number,
                phone_num_len,
                tele_num,
                tele_len,
                actual_phone_num_len);
        }
        else
        {
            //copy ip number
            MMI_MEMCPY(
                phone_number,
                phone_num_len,
                ip_number,
                sizeof(ip_number),
                MIN(sizeof(ip_number), phone_num_len));

            //copy actual tele number    
            actual_phone_num_len -= ip_num_len;//除去ip number之后
            actual_phone_num_len = MIN(actual_phone_num_len, (tele_len - country_code_length));//保护,丢弃多余numbers
            
            MMI_MEMCPY(
                phone_number + ip_num_len,
                phone_num_len - ip_num_len,
                tele_num + country_code_length,
                tele_len - country_code_length,
                actual_phone_num_len);
        }
        return TRUE;
    }
    
    return FALSE;
}
#endif

/*****************************************************************************/
// 	Description : emergency call win msg
//	Global resource dependence : 
//  Author:bownzhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsInputEmergencyCall(MMI_STRING_T *str_info_ptr, uint16 *emergency_call_len_ptr, MN_DUAL_SYS_E dual_sys)
{
    uint32 i = 0;
    uint32 j = 0;
    uint16 emergency_call_len = 0;
    wchar temp_emergency_call[CC_EMERGENCY_CALL_NUM_MAX_LEN + 1] = {0};
    uint16 temp_emergency_call_len = 0;
    BOOLEAN is_input_emergency_call = FALSE;
    MNSIM_ECC_T sim_emc = {0};//记录sim卡存储的emc号码
    MN_DUAL_SYS_E call_dual_sys = MN_DUAL_SYS_1;
    uint8 *emergency_call_ptr = PNULL;
    BOOLEAN is_check_sim_ok = TRUE;//是否检查SIM OK
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    BOOLEAN is_local_exisit = FALSE;
    //BOOLEAN is_emergency_call_in_local = FALSE;
    const MMIPLMN_EMERGENCY_CALL_T *local_emergency_list_ptr = PNULL;
#endif
    
    if (PNULL == str_info_ptr || PNULL == emergency_call_len_ptr)
    {
        return FALSE;
    }
    
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    for (j = 0; j < MMI_DUAL_SYS_MAX; j++)
    {
        local_emergency_list_ptr = MMIAPIPLMN_GetLocalEmergency(i);//@@@

        if (PNULL != local_emergency_list_ptr && !is_local_exisit)
        {
            is_local_exisit = TRUE;
        }
        else
        {
            is_local_exisit = FALSE;
        }

        //local emergency call check
        if (PNULL != local_emergency_list_ptr)
        {
            for (i = 0; ; i++)
            {
                if (PNULL != local_emergency_list_ptr[i].emergency_call_ptr)
                {
                    SCI_MEMSET(temp_emergency_call, 0x00, sizeof(temp_emergency_call));
                    temp_emergency_call_len = strlen(local_emergency_list_ptr[i].emergency_call_ptr);
                    temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
                    MMI_STRNTOWSTR(temp_emergency_call,
                        CC_CALL_NUM_MAX_LEN_EX,
                        (uint8 *)local_emergency_list_ptr[i].emergency_call_ptr,
                        strlen(local_emergency_list_ptr[i].emergency_call_ptr),
                        temp_emergency_call_len);
                    
                    if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, temp_emergency_call, temp_emergency_call_len))
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else if (strlen(local_emergency_list_ptr[i].emergency_call_ptr) < str_info_ptr->wstr_len)
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else
                    {
                        *emergency_call_len_ptr = strlen(local_emergency_list_ptr[i].emergency_call_ptr);
                        is_input_emergency_call = TRUE;
                        
                        return is_input_emergency_call;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
#endif
    
#ifdef CSC_XML_SUPPORT
    if (s_cc_csc_sos.sos_num > 0)
    {
        MMICC_CSC_SOS_NUM_INFO_T *cur_csc_sos_ptr = PNULL;

        cur_csc_sos_ptr = s_cc_csc_sos.sos_num_info_ptr;

        while (PNULL != cur_csc_sos_ptr)
        {
            if (PNULL != cur_csc_sos_ptr->sos_num_ptr)
            {
                SCI_MEMSET(temp_emergency_call, 0x00, sizeof(temp_emergency_call));
                temp_emergency_call_len = strlen((char *)cur_csc_sos_ptr->sos_num_ptr);
                temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
                temp_emergency_call_len = MIN(temp_emergency_call_len, CC_EMERGENCY_CALL_NUM_MAX_LEN);

                MMI_STRNTOWSTR(temp_emergency_call,
                    CC_EMERGENCY_CALL_NUM_MAX_LEN,
                    cur_csc_sos_ptr->sos_num_ptr,
                    strlen((char *)cur_csc_sos_ptr->sos_num_ptr),
                    temp_emergency_call_len);

                if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, temp_emergency_call, temp_emergency_call_len))
                {
                    is_input_emergency_call = FALSE;
                }
                else if (strlen((char *)cur_csc_sos_ptr->sos_num_ptr) < str_info_ptr->wstr_len)
                {
                    is_input_emergency_call = FALSE;
                }
                else
                {
                    *emergency_call_len_ptr = strlen((char *)cur_csc_sos_ptr->sos_num_ptr);
                    is_input_emergency_call = TRUE;

                    return is_input_emergency_call;
                }
            }

            cur_csc_sos_ptr = cur_csc_sos_ptr->next;
        }
    }
    else
#endif
    {
        emergency_call_len = ARR_SIZE(s_emergency_call);

#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
        if (!is_local_exisit)
#endif
        {
            for (i = 0; i < emergency_call_len; i++)
            {
                SCI_MEMSET(temp_emergency_call, 0x00, sizeof(temp_emergency_call));
                temp_emergency_call_len = strlen(s_emergency_call[i]);
                temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
                MMI_STRNTOWSTR(temp_emergency_call,
                    CC_EMERGENCY_CALL_NUM_MAX_LEN,
                    (uint8 *)s_emergency_call[i],
                    strlen(s_emergency_call[i]),
                    temp_emergency_call_len);

                if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, temp_emergency_call, temp_emergency_call_len))
                {
                    is_input_emergency_call = FALSE;
                }
                else if (strlen(s_emergency_call[i]) < str_info_ptr->wstr_len)
                {
                    is_input_emergency_call = FALSE;
                }
                else
                {
                    *emergency_call_len_ptr = strlen(s_emergency_call[i]);
                    is_input_emergency_call = TRUE;

                    return is_input_emergency_call;
                }
            }
        }
    }

    for (j = 0; j < MMI_DUAL_SYS_MAX; j++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(j))
        {
            sim_emc = MNSIM_GetEccEx(j);
                
            if (0 < sim_emc.validate_num)
            {
                for (i = 0; i < sim_emc.validate_num; i++)
                {
                    uint8 ecc_code[MN_MAX_SIM_ECC_LEN*2 + 2] = {0};
                    wchar ecc_code_wstr[MN_MAX_SIM_ECC_LEN*2 + 2] = {0};
                    uint8 tel_num_len = 0;
                    
                    SCI_MEMSET(ecc_code, 0x00, sizeof(ecc_code));
                    tel_num_len = MMIAPICOM_GenNetDispNumber(
                        MN_NUM_TYPE_NATIONAL, 
                        (uint8)(MN_MAX_SIM_ECC_LEN),
                        (uint8*)sim_emc.call_codes[i].ecc_code,
                        ecc_code,
                        (uint8)(MN_MAX_SIM_ECC_LEN*2+2)
                        );

                    temp_emergency_call_len = MIN(tel_num_len, MN_MAX_SIM_ECC_LEN*2);
                    temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
                    SCI_MEMSET(ecc_code_wstr, 0x00, sizeof(ecc_code_wstr));
                    
                    MMI_STRNTOWSTR(ecc_code_wstr,
                        MN_MAX_SIM_ECC_LEN*2,
                        ecc_code,
                        tel_num_len,
                        temp_emergency_call_len);
                    
                    if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, ecc_code_wstr, temp_emergency_call_len))
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else if (temp_emergency_call_len < str_info_ptr->wstr_len)
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else
                    {
                        *emergency_call_len_ptr = MIN(tel_num_len, MN_MAX_SIM_ECC_LEN*2);
                        is_input_emergency_call = TRUE;
                        
                        return is_input_emergency_call;
                    }
                }
            }
    
            if (dual_sys < MMI_DUAL_SYS_MAX && PNULL != s_cc_network_emg_num_list_ptr[dual_sys]
                && s_cc_network_emg_num_list_ptr[dual_sys]->list_num > 0)
            {
                for (i = 0; i < s_cc_network_emg_num_list_ptr[dual_sys]->list_num; i++)
                {
                    uint8 ecc_code[MN_MAX_ADDR_BCD_LEN*2 + 2] = {0};
                    wchar ecc_code_wstr[MN_MAX_ADDR_BCD_LEN*2 + 2] = {0};
                    uint8 tel_num_len = 0;
                    
                    SCI_MEMSET(ecc_code, 0x00, sizeof(ecc_code));
                    tel_num_len = MMIAPICOM_GenNetDispNumber(
                        MN_NUM_TYPE_NATIONAL, 
                        s_cc_network_emg_num_list_ptr[dual_sys]->emg_num_list[i].num_len,
                        (uint8*)s_cc_network_emg_num_list_ptr[dual_sys]->emg_num_list[i].party_num,
                        ecc_code,
                        (uint8)(MN_MAX_ADDR_BCD_LEN*2+2)
                        );

                    temp_emergency_call_len = MIN(tel_num_len, MN_MAX_ADDR_BCD_LEN*2);
                    temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
                    SCI_MEMSET(ecc_code_wstr, 0x00, sizeof(ecc_code_wstr));
                    
                    MMI_STRNTOWSTR(ecc_code_wstr,
                        MN_MAX_ADDR_BCD_LEN*2,
                        ecc_code,
                        tel_num_len,
                        temp_emergency_call_len);
                    
                    if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, ecc_code_wstr, temp_emergency_call_len))
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else if (temp_emergency_call_len < str_info_ptr->wstr_len)
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else
                    {
                        *emergency_call_len_ptr = MIN(tel_num_len, MN_MAX_ADDR_BCD_LEN*2);
                        is_input_emergency_call = TRUE;
                        
                        return is_input_emergency_call;
                    }
                }
            }
        }
    }

    //获取紧急拨号SIM卡
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    if (!is_local_exisit)
#endif
    {    
        if (dual_sys < MMI_DUAL_SYS_MAX)
        {
            emergency_call_ptr = (uint8 *)SCI_ALLOCA(str_info_ptr->wstr_len + 1);

            if (PNULL == emergency_call_ptr)
            {
                return FALSE;
            }

            SCI_MEMSET(emergency_call_ptr, 0x00, (str_info_ptr->wstr_len + 1));

            MMI_WSTRNTOSTR(emergency_call_ptr,
                                        str_info_ptr->wstr_len,
                                        str_info_ptr->wstr_ptr,
                                        str_info_ptr->wstr_len,
                                        str_info_ptr->wstr_len);

            call_dual_sys = GetMoCallSimIdEx(dual_sys,
                                                            emergency_call_ptr,
                                                            str_info_ptr->wstr_len,
                                                            CC_CALL_SIM_MAX);

            SCI_FREE(emergency_call_ptr); 

            is_check_sim_ok = MMIAPIPHONE_IsSimOk(call_dual_sys);
        }
        else
        {
            is_check_sim_ok = TRUE;
        }

        if (is_check_sim_ok
    		|| SIM_NOT_INITED == MMIAPIPHONE_GetSimStatus(dual_sys))
        {
#ifdef CSC_XML_SUPPORT
            if (s_cc_csc_no_sim_sos.sos_num > 0)
            {
                MMICC_CSC_SOS_NUM_INFO_T *cur_csc_sos_ptr = PNULL;

                cur_csc_sos_ptr = s_cc_csc_no_sim_sos.sos_num_info_ptr;

                while (PNULL != cur_csc_sos_ptr)
                {
                    if (PNULL != cur_csc_sos_ptr->sos_num_ptr)
                    {
                        SCI_MEMSET(temp_emergency_call, 0x00, sizeof(temp_emergency_call));
                        temp_emergency_call_len = strlen((char *)cur_csc_sos_ptr->sos_num_ptr);
                        temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
                        temp_emergency_call_len = MIN(temp_emergency_call_len, CC_EMERGENCY_CALL_NUM_MAX_LEN);

                        MMI_STRNTOWSTR(temp_emergency_call,
                            CC_EMERGENCY_CALL_NUM_MAX_LEN,
                            cur_csc_sos_ptr->sos_num_ptr,
                            strlen((char *)cur_csc_sos_ptr->sos_num_ptr),
                            temp_emergency_call_len);
                        
                        if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, temp_emergency_call, temp_emergency_call_len))
                        {
                            is_input_emergency_call = FALSE;
                        }
                        else if (strlen((char *)cur_csc_sos_ptr->sos_num_ptr) < str_info_ptr->wstr_len)
                        {
                            is_input_emergency_call = FALSE;
                        }
                        else
                        {
                            *emergency_call_len_ptr = strlen((char *)cur_csc_sos_ptr->sos_num_ptr);
                            is_input_emergency_call = TRUE;
                            
                            return is_input_emergency_call;
                        }
                    }

                    cur_csc_sos_ptr = cur_csc_sos_ptr->next;
                }
            }
            else
#endif
            {
                emergency_call_len = ARR_SIZE(s_emergency_call_no_sim);
                
                for (i = 0; i < emergency_call_len; i++)
                {
                    SCI_MEMSET(temp_emergency_call, 0x00, sizeof(temp_emergency_call));
                    temp_emergency_call_len = strlen(s_emergency_call_no_sim[i]);
                    temp_emergency_call_len = MIN(temp_emergency_call_len, str_info_ptr->wstr_len);
                    MMI_STRNTOWSTR(temp_emergency_call,
                        CC_EMERGENCY_CALL_NUM_MAX_LEN,
                        (uint8 *)s_emergency_call_no_sim[i],
                        strlen(s_emergency_call_no_sim[i]),
                        temp_emergency_call_len);
                    
                    if (0 != MMIAPICOM_CompareTwoWstr(str_info_ptr->wstr_ptr, str_info_ptr->wstr_len, temp_emergency_call, temp_emergency_call_len))
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else if (strlen(s_emergency_call_no_sim[i]) < str_info_ptr->wstr_len)
                    {
                        is_input_emergency_call = FALSE;
                    }
                    else
                    {
                        *emergency_call_len_ptr = strlen(s_emergency_call_no_sim[i]);
                        is_input_emergency_call = TRUE;
                        
                        return is_input_emergency_call;
                    }
                }
            }
        }
    }    
    
    return is_input_emergency_call;
}

/*****************************************************************************/
// 	Description : is dtmf
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_IsDTMF(uint8* dtmf_str, uint8 dtmf_len)
{
    uint32 i = 0;
    
    for (i = 0; i < dtmf_len; i++)
    {
        //DTMF 0-9,A,B,C,D,*,#
        if ((dtmf_str[i] >= '0' && dtmf_str[i] <= '9') || 
            (dtmf_str[i] >= 'A' && dtmf_str[i] <= 'D') ||
            dtmf_str[i] == '#' ||
            dtmf_str[i] =='*')
        {
            break;
        }
    }
    
    if (i >= s_call_context.dtmf_state.dtmf_len)
    {
        s_call_context.dtmf_state.is_dtmf= FALSE;
        
        MMK_CloseWin(WATCHCC_SEND_DTMF_NOTE_WIN_ID);
        MMK_CloseWin(WATCHCC_SEND_DTMF_QUERY_WIN_ID);
        CC_ClearStoredDtmf();
        
        return CC_RESULT_ERROR;
    }
    
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
// 	Description : send valid dtmf
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_SendDTMFStr(uint8* dtmf_str, uint8 dtmf_len)
{
    uint32 i = 0;
    
    //SCI_TRACE_LOW:"mmiccapp: CC_SendDTMFStr, dtmf_str = %s, dtmf_len = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_6948_112_2_18_2_10_21_187,(uint8*)"sd", dtmf_str, dtmf_len);
    
    for(i = 0; i < dtmf_len; i++)
    {
        SendDtmfByChar(MN_DUAL_SYS_MAX, dtmf_str[i], TRUE);
    }

    return CC_RESULT_SUCCESS;

}

/*****************************************************************************/
// 	Description : get mo call sim card id
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetMoCallSimId(
                                   MN_DUAL_SYS_E dual_sys,//current sim
                                   CC_CALL_SIM_TYPE_E call_sim_type//call  sim type
                                   )
{
//if call_sim_type == CC_MAJOR_SIM_CALL or CC_SECONDARY_SIM_CALL or CC_CALL_SIM_MAX, 
//then return MN_DUAL_SYS_MAX. (call_dual_sys== MN_DUAL_SYS_MAX).
//so user should sellect which sim to call.
#if !defined( MMI_MULTI_SIM_SYS_SINGLE )&& !defined( MMI_MULTI_SIM_SYS_NONE )
//     MMISET_DUALSYS_TYPE_E dual_setting = MMISET_DAULSYS_ONLY_SIM1_ON;
    MN_DUAL_SYS_E call_dual_sys = MN_DUAL_SYS_MAX;
    MMISET_MAJOR_SIM_SEL_TYPE_E major_sim_sel_type = MMISET_MAJOR_SIM_SEL_MANUAL_SEL;
    BOOLEAN is_origin_reply = FALSE;
    uint16 sim_ok = MN_DUAL_SYS_1;
    uint16 sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok,1);
    if (1 == sim_num)
    {
        if(call_sim_type >= CC_SIM1_CALL && call_sim_type < CC_SIM1_CALL + MMI_DUAL_SYS_MAX)
        {
             call_dual_sys = MN_DUAL_SYS_1 + call_sim_type - (uint32)CC_SIM1_CALL;
        }
        else
        {
            call_dual_sys = (uint32)sim_ok;
        }
    }
    else if(sim_num > 1)
    {
         is_origin_reply = MMIAPISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_CALL);
         if(call_sim_type >= CC_SIM1_CALL && call_sim_type < CC_SIM1_CALL + MMI_DUAL_SYS_MAX)
         {
             call_dual_sys = (MN_DUAL_SYS_E)(MN_DUAL_SYS_1 + ((uint16)call_sim_type -CC_SIM1_CALL) );
         }
         else if(is_origin_reply && (dual_sys < MMI_DUAL_SYS_MAX) && MMIAPIPHONE_IsSimOk(dual_sys))
         {
             call_dual_sys = dual_sys;
         }
         else
         {
             major_sim_sel_type = MMIAPISET_GetMajorSim(MMISET_MAJOR_SIM_SEL_FUNC_CALL); //if not need major sim. and ok sim > 1,this call return MMISET_MAJOR_SIM_SEL_MANUAL_SEL
             if(major_sim_sel_type >= MMISET_MAJOR_SIM_SEL_SIM1 && major_sim_sel_type < MMISET_MAJOR_SIM_SEL_SIM1 + MMI_DUAL_SYS_MAX)
             {
                 if(CC_MAJOR_SIM_CALL == call_sim_type)
                 {
                     call_dual_sys = (MN_DUAL_SYS_E)(MN_DUAL_SYS_1 + ((uint16)major_sim_sel_type -MMISET_MAJOR_SIM_SEL_SIM1) );
                 }
             }
         }
    }
    else
    {
        uint32 i = 0;

        for (i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {
            if (MMIAPIPHONE_GetIsPsReady(i))
            {
                call_dual_sys = (MN_DUAL_SYS_E)i;

                break;
            }
        }
        // 如果是飞行模式或者零待机的情况下，不能操作选卡界面。
        if(MN_DUAL_SYS_MAX == call_dual_sys)
        {
            call_dual_sys = MN_DUAL_SYS_1;
        }        
    }

    return call_dual_sys;
#else /* MMI_MULTI_SIM_SYS_SINGLE */
    return MN_DUAL_SYS_1;
#endif /* MMI_MULTI_SIM_SYS_SINGLE */
}

/*****************************************************************************/
//  Description : 截掉了非首位的+号以及后面的字符
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
/*LOCAL uint8 MMICC_InterceptPhoneNum(//截取后的长度
                               uint8    *str_ptr,
                               uint8    str_len
                               )
{
    uint8      intercept_len = 0;
    uint8      i = 0;

    //截掉了非首位的+号以及后面的字符
    intercept_len = str_len;
    for (i=1; i<str_len; i++)
    {
        if ('+' == str_ptr[i])
        {
            intercept_len = i;
            break;
        }
    }

    return (intercept_len);
}*/

#ifdef MMI_CRSS_SUPPORT
/*****************************************************************************/
// 	Description : is a ECT string(23091)
//	Global resource dependence : s_call_context
//    Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
LOCAL BOOLEAN IsECTString(uint8 *tele_num, uint8 tele_len)
{

    if(tele_num != PNULL && tele_len == 1 && tele_num[0] == '4')
    {
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : is a CRSS string(22030-340:6.5.5)
//	Global resource dependence : s_call_context
//    Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
LOCAL BOOLEAN IsStringOfCRSS(uint8 *tele_num, uint8 tele_len)
{
    uint8 cmd = 0;
    uint8 call_index = 0;
    uint8 cmd_length = tele_len;

    cmd = tele_num[0];
    if (cmd_length > 1)
    {
        call_index = tele_num[1];
    }

    if (( cmd_length == 1 && cmd <= '5' )
        || ( cmd_length == 2 && ((cmd == '1' || cmd == '2') && (call_index >= '1' && (call_index - '0') <= CC_MAX_CALL_NUM)) )
        || ( cmd_length > 2 && cmd == '4' && call_index == '*'))
    {
        return TRUE;
    }
    else 
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : the situation is appropriate for CRSS
//	Global resource dependence : s_call_context
//  Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
LOCAL BOOLEAN IsNeedCRSS(void)
{
    if (( CC_HOLD_STATE == s_call_context.call_info[ s_call_context.current_call].call_state )
        ||( CC_CALL_STATE == s_call_context.call_info[ s_call_context.current_call].call_state )
        ||( CC_WAITING_STATE == s_call_context.call_info[ s_call_context.current_call].call_state ))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif
/*****************************************************************************/
//  Description :judge whether the waiting call is being
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CC_IsWaitCallBeing(void)
{
    BOOLEAN is_wait_call_being = FALSE;
    uint8 call_index = 0;
    
    for( call_index = 0; call_index < s_call_context.call_number ; call_index++ )
    {
        if( CC_WAITING_STATE == s_call_context.call_info[ call_index ].call_state )
        {
            is_wait_call_being = TRUE;
            break;
        }
    }
    //SCI_TRACE_LOW:"ccapp:CC_IsWaitCallBeing %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7146_112_2_18_2_10_21_188,(uint8*)"d", is_wait_call_being);
    return is_wait_call_being;
}
/*****************************************************************************/
// 	Description : Release waiting call 
//	Global resource dependence : s_call_context
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_ReleaseWaitCall(void)
{
    uint16  i = 0;
    uint16  index = CC_INVALID_INDEX;
    CC_RESULT_E result = CC_RESULT_SUCCESS;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_ReleaseWaitCall()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7159_112_2_18_2_10_21_189,(uint8*)"");
    
    //search the call which state is in waiting
    for( i = 0; i < s_call_context.call_number ; i++ )
    {
        if( CC_WAITING_STATE == s_call_context.call_info[ i ].call_state )
        {
            //whether the call ID is vaild
            if(  ( s_call_context.call_info[ i ].call_id >= CC_MAX_CALL_NUM ) )
            {
                //SCI_TRACE_LOW:"CC_ReleaseWaitCall , call_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7169_112_2_18_2_10_21_190,(uint8*)"d", s_call_context.call_info[ i ].call_id);
                return CC_RESULT_ERROR;
            }
            
            //disconnect the call which state is in wait state
            if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(s_call_context.dual_sys, s_call_context.call_info[ i ].call_id, MN_CAUSE_USER_BUSY, PNULL))
            {
                //SCI_TRACE_LOW:"CC_ReleaseWaitCall MNCALL_DiscCall return failure"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7176_112_2_18_2_10_21_191,(uint8*)"");
                result = CC_RESULT_ERROR;
            }
            
            if( !CC_IsCallIDValid(index) )
            {
                index = i;
            }
            
            s_call_context.call_info[ i ].operated_by_user = CC_RELEASE_OPER;
            
            //whether the call is in mpty
            if( ! s_call_context.call_info[i].is_in_mpty )
                break;
        }
    }
    
    if( CC_IsCallIDValid(index) )
    {
        //更新提示信息
        s_call_context.current_call = index;
        MMICC_UpdateCurrentCallProcessing();    //call processing
    }
    
    return result;
}

#ifdef MMI_CRSS_SUPPORT
/*****************************************************************************/
// 	Description : Release the active call for preparing for waiting call or hold call
//	Global resource dependence : s_call_context
//  Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_CRSSReleaseAndAnswer(void)
{
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_CRSSReleaseAndAnswer()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7209_112_2_18_2_10_21_192,(uint8*)"");
    
    //whether the call module has the active call
    if( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num )
    {
        if (CC_IsWaitCallBeing())
        {
            s_call_context.operate_CRSS_code = RELEASE_ALL_ACTIVE_AND_ACCEPT_WAIT;
        }
        else if (CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num)
        {
            s_call_context.operate_CRSS_code = RELEASE_ALL_ACTIVE_AND_RETRIEVE_HOLD;
        }
        else
        {
            s_call_context.operate_CRSS_code = CRSS_OTHER_OPERATE;
        }
        
        if ( CC_RESULT_ERROR == CC_ReleaseActiveCall() )
        {
            return CC_RESULT_ERROR;
        }
    }
    else
    {
        if (CC_IsWaitCallBeing())
        {
            //answer the waiting call
            s_call_context.current_call = s_call_context.incoming_call_index ;
            CC_ConnectCallReq();
        }
        else if (CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num)
        {
            CC_HoldCall();
        }
        else
        {
            return CC_RESULT_ERROR;
        }
    }
    
#ifdef ASP_SUPPORT   
    s_accept_call = TRUE;
#endif
    return CC_RESULT_SUCCESS;
}

/*****************************************************************************/
// 	Description : Release the active call for preparing for waiting call or hold call
//	Global resource dependence : s_call_context
//    Author:tao.xue
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_CRSSHoldAndAnswer(void)
{
    uint8  index = 0;
    
    //SCI_TRACE_LOW:"ccapp.c:enter function CC_CRSSHoldAndAnswer()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7264_112_2_18_2_10_21_193,(uint8*)"");

    if ((CC_NO_NUM != s_call_context.exist_call.exist_active_call_num)
        &&(CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num)
        &&(CC_IsWaitCallBeing()))    
    {   
        //the cc module have both active call, hold call and waiting call
        //so need to build MPTY before answer the call
#if !defined(MMI_GUI_STYLE_TYPICAL)
        MMIPUB_OpenAlertWarningWin(TXT_CC_MERGE_CALLS_FIRST);
        MMIPUB_SetWinTitleTextId( MMIPUB_ALERT_WIN_ID, TXT_CC_COMMUN, FALSE );
#else
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_EST_MPTY_FIRST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKEXIT,PNULL);
#endif
        return CC_RESULT_ERROR;
    }
    
    //whether the call module has the active call
    if( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num )
    {
        if (CC_IsWaitCallBeing())
        {
            index = FindStateCall( CC_CALL_STATE );
            
            if (index >= s_call_context.call_number)
            {
                //SCI_TRACE_LOW:"ccapp CC_CRSSHoldAndAnswer error index %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7290_112_2_18_2_10_21_194,(uint8*)"d", index);
                return CC_RESULT_ERROR;
            }
            if( MN_RETURN_SUCCESS != MNCALL_HoldCallEx(s_call_context.dual_sys, s_call_context.call_info[ index ].call_id ) )
            {
                //SCI_TRACE_LOW:"CC_CRSSHoldAndAnswer: the result of MNCALL_HoldCall is error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7295_112_2_18_2_10_21_195,(uint8*)"");
                return CC_RESULT_ERROR;
            }
            
            s_call_context.operate_waiting_call = HOLD_ALL_ACTIVE_AND_ACCEPT;
            s_call_context.current_call = index ;
            s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_HOLD_OPER;
            
            //Update the prompt information in the cc window
            MMICC_UpdateCurrentCallProcessing();
        }
        else if (CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num)
        {
            return CC_HoldCall();
        }
        else
        {
            return CC_HoldCall();
        }
    }
    else
    {
        if (CC_IsWaitCallBeing())
        {
            // the call module just has hold call
            //accept the incoming call
            s_call_context.current_call = s_call_context.incoming_call_index ;
            if (!MMIAPISTK_SetupCall(s_call_context.dual_sys, TRUE, FALSE, 0))//handle stk call
            {
                CC_ConnectCallReq();
            }
        }
        else if (CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num)
        {
            return CC_HoldCall();
        }
        else
        {
            return CC_RESULT_ERROR;
        }
    }
#ifdef ASP_SUPPORT   
    s_accept_call = TRUE;
#endif        
    return CC_RESULT_SUCCESS;
}

#if 0
/*****************************************************************************/
// 	Description : Release active call by index
//	Global resource dependence : s_call_context
//  Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
PUBLIC CC_RESULT_E CC_ReleaseActiveCallByIndex(uint8 call_index)
{    
    //SCI_TRACE_LOW:"ccapp:CC_ReleaseActiveCallByIndex()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7349_112_2_18_2_10_21_196,(uint8*)"");
    if (CC_CALL_STATE == s_call_context.call_info[call_index].call_state)
    {
        return CC_ReleaseCallReq(s_call_context.call_info[s_call_context.current_call].dual_sys, s_call_context.current_call, MN_CAUSE_NORMAL_CLEARING);
    }
    else
    {
        return CC_RESULT_ERROR;
    }
}
#endif

/*****************************************************************************/
// 	Description : parse CRSS string in call
//	Global resource dependence : s_call_context
//  Author:taoxue
//	Note:2011.01.10
/*****************************************************************************/
LOCAL CC_RESULT_E MMICC_ParseStringOfCRSS(uint8 *tele_num, uint8 tele_len)
{
    uint8 cmd1 = 0;
    uint8 call_index = 0;
    uint8 cmd_length = tele_len;
    
    //SCI_TRACE_LOW:"ccapp:MMICC_ParseStringOfCRSS()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7371_112_2_18_2_10_21_197,(uint8*)"");
    
    cmd1 = tele_num[0] - '0';
    if (cmd_length > 1)
    {
        call_index = tele_num[1] - '0';
    }
    
    switch ( cmd1 )
    {
        case CC_CRSS_RELEASE_DEACTIVE:
            //release waiting call in high priority, or release hold call in normal priority;
            if (CC_IsWaitCallBeing())
            {
                return CC_ReleaseWaitCall();
            }
            else if (MMICC_IsExistHoldCall())
            {
                return CC_ReleaseHoldCall();
            }
            else
            {
                return CC_RESULT_ERROR; 
            }
            break;/*lint !e527*/
        
        case CC_CRSS_RELEASE_ACTIVE_AND_ANSWER:
            //release active call and answer waiting call in high priority, or Retrieve hold call in normal priority;
            if (call_index == 0)
            {
                return CC_CRSSReleaseAndAnswer();
            }
            else if (call_index <= CC_MAX_CALL_NUM)
            {
                if(CC_CALL_STATE != s_call_context.call_info[call_index - 1].call_state)
                {
                    return CC_RESULT_ERROR;
                }
                else
                {
                    return CC_ReleaseCallReq(s_call_context.call_info[s_call_context.current_call].dual_sys, call_index - 1,MN_CAUSE_NORMAL_CLEARING );
                }
            }
            else
            {
                return CC_RESULT_ERROR; 
            }
            break;/*lint !e527*/
        
        case CC_CRSS_HOLD_ACTIVE_AND_ANSWER:
            //hold active call and answer waiting call in high priority, or Retrieve hold call in normal priority;
            if (call_index == 0)
            {
                return CC_CRSSHoldAndAnswer();
            }
            else if (CC_NO_NUM == s_call_context.exist_call.exist_active_call_num || CC_NO_NUM == s_call_context.exist_call.exist_hold_call_num)
            {
                return MMICC_SplitSpecCall(call_index - 1);
            }
            else
            {
                return CC_RESULT_ERROR; 
            }
            break;/*lint !e527*/

        case CC_CRSS_ADD_HOLD_CALL_TO_MPTY:
            if(MMICC_IsExistActiveAndHoldCalls())
            {
                return CC_BuildMPTYCall();
            }
            else
            {
                return CC_RESULT_ERROR; 
            }
            break;/*lint !e527*/

        case CC_CRSS_ECT:
            //ECT 切换
            if (MMICC_ECTIsPermit())
            {
                return MMICC_ECTTransfer(); 
            }
            else
            {
                return CC_RESULT_ERROR;
            }
            break;/*lint !e527*/

        case CC_CRSS_RESERVED2:
            return CC_RESULT_ERROR; 
            break;/*lint !e527*/

        default:
            return CC_RESULT_ERROR; 
            break;/*lint !e527*/
    }
}
#endif

/*****************************************************************************/
// 	Description : Get Mo Call Sim Id Ex
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
LOCAL MN_DUAL_SYS_E GetMoCallSimIdEx(
                                                           MN_DUAL_SYS_E dual_sys,//current sim
                                                           uint8 *tele_num,//phone number
                                                           uint8 tele_len,//phone number wstr_len
                                                           CC_CALL_SIM_TYPE_E call_sim_type//call  sim type
                                                           )
{
    MN_DUAL_SYS_E call_dual_sys = MN_DUAL_SYS_MAX;
    uint16 sim_sys[MMI_DUAL_SYS_MAX] = {0};
    uint32 sim_num = 0;
    BOOLEAN is_emergency_num = FALSE;
    uint32 i = 0;
    MN_DUAL_SYS_E emergency_sim = MN_DUAL_SYS_MAX;

    sim_num = MMIAPIPHONE_GetPSReadyNum(sim_sys, MMI_DUAL_SYS_MAX);    

    call_dual_sys = GetMoCallSimId(dual_sys, call_sim_type);

    for( i = 0;  i < MMI_DUAL_SYS_MAX; i++)
    {
        //遍历每个SIM 卡查找，确认所拨号码是否是紧急号码
        if( MMIAPIPHONE_GetSimExistedStatus((MN_DUAL_SYS_E) i)
            //bug 186648 modify is_only_for_display(the last parameter of MMIAPICC_IsEmergencyNum) true to false,is_only_for_display is setted true is to some requirement,
            //but in different countries in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num(no sim emergency) ,
            //it is not normal display emergency call in window
            && MMIAPICC_IsEmergencyNum(tele_num, tele_len, (MN_DUAL_SYS_E)i, FALSE))
        {
            emergency_sim = (MN_DUAL_SYS_E)i;
            is_emergency_num = TRUE;
            break;
        }
    }

    if ( (sim_num > 0) && is_emergency_num && !MMIAPISET_GetFlyMode())
    {
        if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
        {
            call_dual_sys = s_call_context.dual_sys;
        }
        else
        {
            uint16 sim_ok = 0;
            //先确定是否有有效的sim卡，如果有就选定一张有效的sim卡呼出
            if(0 < MMIAPIPHONE_GetSimOkNum(&sim_ok, 1))
            {
                //如果对应紧急号码的SIM卡是OK的，用相应的SIM卡呼出，否则再选择一张OK的sim卡呼出，确保可以正常呼叫！
                if((emergency_sim < MMI_DUAL_SYS_MAX) && MMIAPIPHONE_IsSimOk(emergency_sim))
                {
                    call_dual_sys = emergency_sim;
                }
                else
                {
                    call_dual_sys = (MN_DUAL_SYS_E)sim_ok;
                }
            }
            else if ((call_dual_sys >= MN_DUAL_SYS_MAX) || !MMIAPIPHONE_GetIsPsReady(call_dual_sys))
            {
                call_dual_sys = (MN_DUAL_SYS_E)sim_sys[0];
            }
        }
    }    

    return call_dual_sys;
}

/*****************************************************************************/
//  Description : make call api for adult watch
//  Parameter: [In] call info
//             [Out] None
//             [Return] make call success or fail
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E WatchCC_MakeCall(MN_DUAL_SYS_E dual_sys,
                                               uint8 *tele_num,
                                               uint8  tele_len
                                               )
{
    uint8  is_phone = 1;

    is_phone = ual_bt_get_product_role();
    SCI_TRACE_LOW("ccapp.c WatchCC_MakeCall, current product mode = %d", is_phone);

    if(1 == is_phone)
    {
        return MMIAPICC_MakeCall(dual_sys, tele_num, tele_len, PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
    }
    else
    {
        return MMIAPICC_MakeBtCall(tele_num, tele_len);
    }
}


/*****************************************************************************/
//  Description : get call error type
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMICC_MO_CALL_ERROR_TYPE_E MMICC_GetMoCallErrorType(void)
{
    return s_cc_mo_error_type;
}

/*****************************************************************************/
// 	Description : Make Call Interface
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_MakeCall(
   MN_DUAL_SYS_E dual_sys,//current sim
   uint8 *tele_num,//phone number
   uint8 tele_len,//phone number wstr_len
   MMI_STRING_T* disp_ptr,//disp ptr
   MMI_STRING_T* name_ptr,//name ptr
   CC_CALL_SIM_TYPE_E call_sim_type,//call  sim type
   uint32 call_type, //call type
   MN_SUBADDR_T* sub_addr //STK中拨打分机号
   )
{
    #if !defined(MMI_MULTI_SIM_SYS_NONE) || !defined(BT_DIALER_SUPPORT)
    MN_DUAL_SYS_E call_dual_sys = MN_DUAL_SYS_MAX;
    CC_MAKE_CALL_INFO make_call_info = {0};/*lint !e64*/
#endif
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
#if defined(MMI_MULTI_SIM_SYS_NONE) && defined(BT_DIALER_SUPPORT)
    BT_STATUS bt_status = BT_ERROR;
    uint8 index=0;
#endif

#if defined(BT_DIALER_SUPPORT) && !defined(MMI_MULTI_SIM_SYS_NONE)
    if(MMICC_GetBtCallFlag())
    {
        SCI_TRACE_LOW("ccapp.c: MMIAPICC_MakeCall bt call is using, make call faild!!!");
        //MMIPUB_OpenAlertWarningWin(TXT_DIAL_FAILED);
        s_cc_mo_error_type = CC_MO_DIAL_FAILED;
        return CC_RESULT_ERROR;
    }
#endif

    if(PNULL == tele_num)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function MMIAPICC_MakeCall(), tele_num is NULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7556_112_2_18_2_10_22_198,(uint8*)"");
        return CC_RESULT_ERROR;
    }

#if defined(MMI_MULTI_SIM_SYS_NONE) && defined(BT_DIALER_SUPPORT)
    if(1 < s_call_context.call_number)
    {
        SCI_TRACE_LOW("ccapp.c: MMIAPICC_MakeCall bt call is using, make call faild!!!");
        //MMIPUB_OpenAlertWarningWin(TXT_DIAL_FAILED);
        s_cc_mo_error_type = CC_MO_DIAL_FAILED;
        return CC_RESULT_ERROR;
    }
    
    if(MMICC_GetBtCallFlag() && (0 <s_call_context.exist_call.exist_active_call_num))
    {
        //hold call first
        SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call));    
        s_call_context.want_mo_call.is_want_mo_call = TRUE;
        s_call_context.exist_call.exist_outgoing_call = TRUE;
        SCI_MEMSET(&s_bt_make_call_info, 0x00, sizeof(CC_MAKE_CALL_INFO));
        s_bt_make_call_info.tele_len = MIN(tele_len, CC_MAX_TELE_NUM_LEN);
        SCI_MEMCPY(s_bt_make_call_info.tele_num, tele_num, s_bt_make_call_info.tele_len);
        
        index = FindStateCall( CC_CALL_STATE );
        
        if(index >= s_call_context.call_number)
        {
            SCI_TRACE_LOW("MMIAPICC_MakeCall BT find FindStateCall( CC_CALL_STATE ) error !!!");
            return CC_RESULT_ERROR;
        }
        
        s_bt_current_call_index = index;
        s_call_context.operate_waiting_call = HOLD_ALL_ACTIVE_AND_ACCEPT;
        s_call_context.current_call = index ;
        s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_HOLD_OPER;
        
        bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_HOLD_ACTIVE_CALLS);
        if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c: MMIAPICC_MakeCall  MMIAPIBT_HfuCallHold  fail");
        }      
    }
    else
    {
        cc_result = MMIAPICC_MakeBtCall(tele_num, tele_len);
    }   
#else

    //先解析是否跟SIM卡操作有关
    if (!(CC_CALL_STK_CALL & call_type))
    {
        if(MMIAPIPARSE_ParseString(tele_num, tele_len))
        {
            //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_MakeCall Parsed in ParseString"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7565_112_2_18_2_10_22_199,(uint8*)"");
            return cc_result;
        }
    }

    //CRSS
#ifdef MMI_CRSS_SUPPORT
    if ( (IsNeedCRSS() && IsStringOfCRSS(tele_num, tele_len))
        ||(IsECTString(tele_num, tele_len) && MMICC_ECTIsPermit()))
    { 
        //SCI_TRACE_LOW:"ccapp.c: MakeCall req is process by CRSS"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7574_112_2_18_2_10_22_200,(uint8*)"");
        if ( CC_RESULT_ERROR == MMICC_ParseStringOfCRSS(tele_num, tele_len))
        {
            //MMIPUB_OpenAlertWarningWin(TXT_COMMON_CMD_NO_SUPPORT);
            s_cc_mo_error_type = CC_MO_CMD_NO_SUPPORT;
            MMIAPIIDLE_CloseDialWin();
            return CC_RESULT_ERROR;
        }
        else
        {
            MMIAPIIDLE_CloseDialWin();
            return CC_RESULT_SUCCESS;
        }

    }
#endif

    //mo mt的时候不能发起呼叫
    if (MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
        ||MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
    { 
        //SCI_TRACE_LOW:"ccapp.c: MakeCall req is reject when mo or mt"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7593_112_2_18_2_10_22_201,(uint8*)"");
        return CC_RESULT_ERROR;
    }
    
    if (MMIAPICC_IsVideoCall())
    { 
        //SCI_TRACE_LOW:"ccapp.c: MakeCall req is reject when vt"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7599_112_2_18_2_10_22_202,(uint8*)"");
        //MMIPUB_OpenAlertWarningWin(TXT_CC_FORBID_MAKECALL_WHEN_VT);
        s_cc_mo_error_type = CC_MO_FORBID_IN_VT;
        return CC_RESULT_ERROR;
    }
    
    //SCI_TRACE_LOW:"MMIAPICC_MakeCall dual_sys=%d,call_sim_type=%d,call_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7604_112_2_18_2_10_22_203,(uint8*)"ddd", dual_sys, call_sim_type, call_type);
    
    //store make call info
    SCI_MEMSET(&make_call_info, 0x00, sizeof(CC_MAKE_CALL_INFO));
    make_call_info.dual_sys = dual_sys;
    make_call_info.tele_len = MIN(tele_len, CC_MAX_TELE_NUM_LEN);
    SCI_MEMCPY(make_call_info.tele_num, tele_num, make_call_info.tele_len);
    make_call_info.disp_ptr= disp_ptr;
    make_call_info.name_ptr = name_ptr;
    make_call_info.call_sim_type = call_sim_type;
    make_call_info.call_type = call_type;
    make_call_info.sub_addr= sub_addr;
    
    if((CC_CALL_STK_CALL & call_type) && (MMI_DUAL_SYS_MAX > dual_sys))
    {
        call_dual_sys = dual_sys;
    }
#ifdef VT_SUPPORT//Bug 336791
    else if (call_type & CC_CALL_VIDEO_CALL) //video Call
    {
        call_dual_sys = MN_DUAL_SYS_1;
    }
#endif
    else
    {
        call_dual_sys = GetMoCallSimIdEx(dual_sys, tele_num, tele_len, call_sim_type);
    }

    //SCI_TRACE_LOW:"MMIAPICC_MakeCall call_dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7626_112_2_18_2_10_22_204,(uint8*)"d", call_dual_sys);
    // make ecc in fly mode
    if (MMIAPICC_IsEmergencyNum(make_call_info.tele_num, make_call_info.tele_len, call_dual_sys, FALSE) && MMICC_IsNeedPowerOnPs(tele_num, tele_len))
    {
         MMICC_EnableNetworkForECC();
    }
    else if(MMIAPISET_GetFlyMode())
    {
         s_cc_mo_error_type = CC_MO_IN_FLY_MODE;
         return CC_RESULT_ERROR;
    }
    else
    {
        cc_result = MMICC_ProcessMakeCall(call_dual_sys, &make_call_info);
    }
#endif
    return cc_result;
}

/*****************************************************************************/
// 	Description : Process Make Call
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ProcessMakeCall(
   MN_DUAL_SYS_E dual_sys,
   CC_MAKE_CALL_INFO *make_call_info_ptr
   )
{
   BOOLEAN is_num_valid = FALSE;
#ifdef MMI_IP_CALL_SUPPORT
   BOOLEAN is_ip_call = FALSE;
#endif
   BOOLEAN is_call_video_call = FALSE;
   BOOLEAN is_stk_call = FALSE;
   CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
   MMI_PARTY_NUMBER_T party_num = {0};/*lint !e64*/
   uint8 tele_number[CC_MAX_TELE_NUM_LEN + 1] = {0};
   uint8 tele_len = 0;
   uint8 copy_len = 0;
   MMI_PARSE_TYPE_E parse_result = MMI_PARSE_NORMAL_CALL_TYPE_E;
   MMI_STRING_T *new_call_name_ptr = {0};
#ifdef MV_SUPPORT   
   BOOLEAN is_need_open_mv = FALSE;
#endif
    
    if(PNULL == make_call_info_ptr)
    {
        //SCI_TRACE_LOW:"ccapp.c:enter function MMICC_ProcessMakeCall(), make_call_info_ptr is NULL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7659_112_2_18_2_10_22_205,(uint8*)"");
        return CC_RESULT_ERROR;
    }
   
    if (0 == make_call_info_ptr->tele_len)
    {
       //SCI_TRACE_MID:"ProcessMakeCall Telephone number is null!"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7665_112_2_18_2_10_22_206,(uint8*)"");

       //提示空号!
       //MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
       s_cc_mo_error_type = CC_MO_EMPTY_NUM;
       return CC_RESULT_ERROR;
    }

    if (MMIAPISET_GetFlyMode())
    {
        //MMIAPIPHONE_AlertSimNorOKStatus();
        s_cc_mo_error_type = CC_MO_IN_FLY_MODE;
        return CC_RESULT_ERROR;
    }
    
   //SCI_TRACE_LOW:"ProcessMakeCall dual_sys=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7679_112_2_18_2_10_22_207,(uint8*)"d", dual_sys);

    //MS00186848 用于自动测试中,避免呼出一些报警电话等
    if (MMIAPIENG_GetCallBarring() && IsIncludeBarringNumbers(make_call_info_ptr->tele_num, make_call_info_ptr->tele_len))
    {
        //SCI_TRACE_LOW:"***ProcessMakeCall MO call is barring: tele_num = %s***"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7684_112_2_18_2_10_22_208,(uint8*)"s", make_call_info_ptr->tele_num);
        //MMIPUB_OpenAlertWarningWin(TXT_CC_CANNOT_CALL);
        s_cc_mo_error_type = CC_MO_IS_BARRING_NUM;
        return CC_RESULT_ERROR;
    }
   
   //parse call type
#ifdef MMI_IP_CALL_SUPPORT
    if (make_call_info_ptr->call_type & CC_CALL_IP_CALL) //IP Call
    {
       is_ip_call = TRUE;
    }
#endif
#ifdef VT_SUPPORT
    if (make_call_info_ptr->call_type & CC_CALL_VIDEO_CALL) //video Call
    {
       is_call_video_call = TRUE;
    }
#endif
    if (make_call_info_ptr->call_type & CC_CALL_STK_CALL) //stk Call
    {
       is_stk_call = TRUE;
    }
#ifdef MV_SUPPORT
    is_need_open_mv = IsCanUseMagicVoice(TRUE) 
                   && !MMK_IsOpenWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID) 
                   && (0 == s_call_context.call_number) 
                   && !MMIAPICC_IsEmergencyNum(make_call_info_ptr->tele_num, make_call_info_ptr->tele_len, dual_sys, FALSE)
                   && !MMK_IsOpenWin(MMICC_AUTO_DIAL_WIN_ID);
#endif
    if ((MN_DUAL_SYS_MAX > dual_sys) 
#ifdef MV_SUPPORT
    && !is_need_open_mv
#endif
    )
    {
#ifdef MMI_DUALMODE_ENABLE
        if (!CC_IsNetSupport(dual_sys))
        {
            //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() network not support"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7719_112_2_18_2_10_22_209,(uint8*)"");
            //提示网络不可用
            //MMIPUB_OpenAlertWarningWin(TXT_NET_UNAVAIL_FOR_CALL);
            s_cc_mo_error_type = CC_MO_NET_UNAVAIL;
            return CC_RESULT_ERROR; 
        }
#endif
       //cr129131
       is_num_valid = !('P' == make_call_info_ptr->tele_num[0] 
           || 'W' == make_call_info_ptr->tele_num[0] 
           || 'p' == make_call_info_ptr->tele_num[0] 
           || 'w' == make_call_info_ptr->tele_num[0]
           || 'N' == make_call_info_ptr->tele_num[0]
           || 'n' == make_call_info_ptr->tele_num[0]
           || !MMIAPICOM_IsValidNumberString((char *)make_call_info_ptr->tele_num, make_call_info_ptr->tele_len)
           );
       if (!is_num_valid)
       {
           //SCI_TRACE_LOW:"ccapp.c: CC_ProcessExtensionNum() 'P' == tele_num[0]"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7736_112_2_18_2_10_22_210,(uint8*)"");
           //MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
           s_cc_mo_error_type = CC_MO_INVALID_NUM;
           return CC_RESULT_ERROR;
       }
#ifdef MMI_IP_CALL_SUPPORT
       if (is_ip_call) //IP Call
       {
           uint8 ip_number[MMISET_IPNUMBER_MAX_LEN + 1] = {0};
           BOOLEAN is_ip_active = FALSE;
           is_ip_active = MMIAPISET_GetActiveIPNumber(dual_sys, ip_number, (MMISET_IPNUMBER_MAX_LEN + 1));
           
           if (is_ip_active)
           {
               SCI_MEMSET(tele_number, 0x00, sizeof(tele_number));
               
               is_num_valid = CC_GetIPCallNum(dual_sys, make_call_info_ptr->tele_num, make_call_info_ptr->tele_len, tele_number, sizeof(tele_number));
               if (!is_num_valid)
               {
                   //MMIPUB_OpenAlertWarningWin(TXT_CC_INVALID_NUM);
                   s_cc_mo_error_type = CC_MO_INVALID_NUM;
                   return CC_RESULT_ERROR;
               }
           }
           else
           {
               //MMIPUB_OpenAlertWarningWin(TXT_CC_NO_IP_NUM);//MS00189367
               s_cc_mo_error_type = CC_MO_NO_IP_NUM;
               return CC_RESULT_ERROR;
           }
       }
       else
#endif
       {
           MMI_MEMCPY(
               tele_number,
               sizeof(tele_number),
               make_call_info_ptr->tele_num,
               make_call_info_ptr->tele_len,
               MIN(sizeof(tele_number), make_call_info_ptr->tele_len)
               );
       }
       
       tele_len = strlen((char*)tele_number);
       
       if (0 == tele_len)
       {
           //提示空号!
           //MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
           s_cc_mo_error_type = CC_MO_EMPTY_NUM;
           return CC_RESULT_ERROR;
       }
       
       //translate the telenumber to party number
       if(tele_len > CC_MAX_TELE_NUM_LEN)
       {
           tele_len = CC_MAX_TELE_NUM_LEN;
       }
	      
       if (!is_stk_call)
       {
            parse_result = MMIAPIPARSE_ParseTeleNum(dual_sys, tele_number, tele_len, is_call_video_call);
       }

       //SCI_TRACE_LOW:"ProcessMakeCall parse_result=%d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7796_112_2_18_2_10_22_211,(uint8*)"d", parse_result);
       
       if (MMI_PARSE_CALL_CLIR_SUPPRESSION_E == parse_result
           || MMI_PARSE_CALL_CLIR_INVOCATION_E == parse_result
           || MMI_PARSE_NORMAL_CALL_TYPE_E == parse_result)
       {
            //if sim is ok
            if (dual_sys < MMI_DUAL_SYS_MAX
                &&!MMIAPICC_IsEmergencyNum(make_call_info_ptr->tele_num, make_call_info_ptr->tele_len, dual_sys, FALSE)
                &&!CC_IsSimOk(dual_sys))
            {
                //SIM card isn't ok
                //SCI_TRACE_LOW:"ccapp.c:MMIAPICC_MakeCallReq() SIM isn't ok"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7808_112_2_18_2_10_22_212,(uint8*)"");
#if 0
                if (0 == MMIAPISET_GetMultiSysSettingNum(PNULL, 0))//无待机卡
                {
                    MMIAPIPHONE_AlertSimNorOKStatus();
                }
                else if (!MMIAPIPHONE_IsSimAvailable(dual_sys))//该卡槽是否有卡并且待机
                {
                    MMIAPIPHONE_AlertSimNorOKStatusII(dual_sys);
                }
                else
                {
                    MMIPUB_OpenAlertWarningWin(TXT_CC_EMERGENCYONLY);
                }
#endif
                s_cc_mo_error_type = CC_MO_NO_SIM;
                return CC_RESULT_ERROR;
            }

           if ((s_call_context.call_number > CC_NO_NUM) && (s_call_context.dual_sys != dual_sys))//cr128072
           {
               //准备呼出的sim卡和正在通话中的sim卡不相同，则不能呼出
               //SCI_TRACE_LOW:"ccapp: sim corrupt!"
               SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7828_112_2_18_2_10_22_213,(uint8*)"");
               //MMIPUB_OpenAlertWarningWin(TXT_SIM_CORRUPT);
               s_cc_mo_error_type = CC_MO_SIM_CORRUPT;
               return CC_RESULT_ERROR;
           }
           if (CC_NO_NUM != s_call_context.exist_call.exist_active_call_num)//已有通话
           {
               if (CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num) //已有保持通话
               {
                   //提示用户需先建立MPTY
#if !defined(MMI_GUI_STYLE_TYPICAL)
                    //MMIPUB_OpenAlertWarningWin(TXT_CC_MERGE_CALLS_FIRST);
                   s_cc_mo_error_type = CC_MO_MERGR_CALL_FIRST;
                   MMIPUB_SetWinTitleTextId( MMIPUB_ALERT_WIN_ID, TXT_CC_COMMUN, FALSE );
#else
                   //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_EST_MPTY_FIRST,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKEXIT,PNULL);
                   s_cc_mo_error_type = CC_MO_MERGR_CALL_FIRST;
#endif
                   return CC_RESULT_ERROR;
               }
               else
               {
                    SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, (sizeof(s_call_context.want_mo_call)));
                    
                    //set is stk call
                    s_call_context.want_mo_call.call_type = make_call_info_ptr->call_type;

                   //set the information for s_call_context
                   s_call_context.want_mo_call.is_want_mo_call = TRUE;
                   
                   MMIAPICOM_GenPartyNumber(tele_number, tele_len, &party_num );
                   
                   s_call_context.want_mo_call.address.number_type = party_num.number_type ;
                   
                   s_call_context.want_mo_call.address.number_plan = MN_NUM_PLAN_ISDN_TELE;
                   
                   SCI_MEMSET((void*)s_call_context.want_mo_call.address.party_num, 0xFF, sizeof(s_call_context.want_mo_call.address.party_num));
                   
                   copy_len = ( party_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num.num_len;
                   s_call_context.want_mo_call.address.num_len = copy_len ;
                   MMI_MEMCPY(
                       (void*)s_call_context.want_mo_call.address.party_num,
                       sizeof(s_call_context.want_mo_call.address.party_num),
                       (void*)party_num.bcd_num,
                       sizeof(party_num.bcd_num),
                       copy_len
                       );
                    
                    if (PNULL != make_call_info_ptr->name_ptr)
                    {
                        new_call_name_ptr = make_call_info_ptr->name_ptr;
                        if (0 != new_call_name_ptr->wstr_len && PNULL != new_call_name_ptr->wstr_ptr)
                        {
                            s_call_context.want_mo_call.is_name_exist = TRUE;//传入的姓名不为空,这是从ADN/FDN/SDN发起的通话. this is a call from ADN/FDN/SDN list
                            
                            s_call_context.want_mo_call.name_len = new_call_name_ptr->wstr_len;   
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
                            copy_len = s_call_context.want_mo_call.name_len;
#else    
                            copy_len = ( s_call_context.want_mo_call.name_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : s_call_context.want_mo_call.name_len;/*lint !e506 */
#endif
                            MMI_WSTRNCPY(
                                s_call_context.want_mo_call.name,
                                sizeof(s_call_context.want_mo_call.name),
                                new_call_name_ptr->wstr_ptr,
                                new_call_name_ptr->wstr_len,
                                copy_len
                                );
                        }
                        else
                        {
                            s_call_context.want_mo_call.is_name_exist = FALSE;

                            s_call_context.want_mo_call.name_len = tele_len;
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
                            copy_len = tele_len;
#else
                            copy_len = ( tele_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : tele_len;
#endif
                            MMI_STRNTOWSTR(
                                s_call_context.want_mo_call.name,
                                s_call_context.want_mo_call.name_len,
                                tele_number,
                                tele_len,
                                copy_len
                                );
                            s_call_context.want_mo_call.name_len = copy_len;
                        }
                    }
                        
                   //get the active call
                   if ( CC_CALL_STATE != s_call_context.call_info[ s_call_context.current_call ].call_state)
                   {
                       s_call_context.current_call = FindStateCall( CC_CALL_STATE );
                       if (s_call_context.current_call >= s_call_context.call_number)
                       {
                           //SCI_TRACE_LOW:"s_call_context.call_number = %d"
                           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7921_112_2_18_2_10_23_214,(uint8*)"d", s_call_context.call_number);
                           return CC_RESULT_ERROR;
                       }
                   }
                   
                   //hold the active for preparing to send mo call
                   if ( MN_RETURN_SUCCESS != MNCALL_HoldCallEx(s_call_context.dual_sys, s_call_context.call_info[ s_call_context.current_call ].call_id ) )
                   {
                       ClearWantCallInfo();
                       //SCI_TRACE_LOW:"ProcessMakeCall: the result of MNCALL_HoldCall is error"
                       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7930_112_2_18_2_10_23_215,(uint8*)"");
                       
                       return CC_RESULT_ERROR;
                   }
                   else
                   {
                       s_is_need_call_after_hold = TRUE;
                   }
                   
                   //更新显示提示信息
                   s_call_context.call_info[s_call_context.current_call].operated_by_user = CC_HOLD_OPER;
                   MMICC_UpdateCurrentCallProcessing();    //call processing
               }
           }
           else
           {
               if ((MMI_PARSE_CALL_CLIR_SUPPRESSION_E == parse_result) && (4 < tele_len))
               {
                   cc_result = ProcessPhoneNumExt(dual_sys, tele_number + 4, tele_len - 4, make_call_info_ptr->disp_ptr, make_call_info_ptr->name_ptr, TRUE, MN_CALL_CLIR_SUPPRESSION, make_call_info_ptr->call_type, make_call_info_ptr->sub_addr);
               }
               else if ((MMI_PARSE_CALL_CLIR_INVOCATION_E == parse_result) && (4 < tele_len))
               {
                   cc_result = ProcessPhoneNumExt(dual_sys, tele_number + 4, tele_len - 4, make_call_info_ptr->disp_ptr, make_call_info_ptr->name_ptr, TRUE, MN_CALL_CLIR_INVOCATION, make_call_info_ptr->call_type, make_call_info_ptr->sub_addr);
               }
               else// if (MMI_PARSE_NORMAL_CALL_TYPE_E == parse_result)
               {
                   cc_result = ProcessPhoneNumExt(dual_sys, tele_number, tele_len, make_call_info_ptr->disp_ptr, make_call_info_ptr->name_ptr, FALSE, 0, make_call_info_ptr->call_type, make_call_info_ptr->sub_addr);
               }
           }
       }
       else
       {
           //SCI_TRACE_LOW:"ProcessMakeCall: has handled in parse function"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7962_112_2_18_2_10_23_216,(uint8*)"");

#ifdef MMI_NOT_SUPPORT_USSD_SS_INCC
            if((MMI_PARSE_SS_TYPE_E == parse_result) || (MMI_PARSE_USSD_TYPE_E == parse_result))
            {
                SCI_TRACE_LOW("MMICC_ProcessMakeCall:parse_result:%d\n",parse_result);
                s_cc_mo_error_type = CC_MO_CMD_NO_SUPPORT;
                cc_result = CC_RESULT_ERROR;
            }
#endif
           return cc_result;
       }
    }
    else//第一通去电时,才可以主动打开魔音选择窗口
    {
        CC_MAKE_CALL_INFO * call_info_ptr = PNULL;

        if (MMIAPISET_GetFlyMode())
        {
            //MMIAPIPHONE_AlertSimNorOKStatus();
            s_cc_mo_error_type = CC_MO_IN_FLY_MODE;
            return CC_RESULT_ERROR;
        }
        
        call_info_ptr = (CC_MAKE_CALL_INFO *)SCI_ALLOCA(sizeof(CC_MAKE_CALL_INFO));
        if (PNULL != call_info_ptr)
        {
            SCI_MEMSET(call_info_ptr, 0x00, sizeof(CC_MAKE_CALL_INFO));
            SCI_MEMCPY(call_info_ptr, make_call_info_ptr, sizeof(CC_MAKE_CALL_INFO));
            
            if (PNULL != make_call_info_ptr->disp_ptr)
            {
                call_info_ptr->disp_ptr = (MMI_STRING_T *)SCI_ALLOCA(sizeof(MMI_STRING_T));
                SCI_MEMSET(call_info_ptr->disp_ptr, 0x00, sizeof(MMI_STRING_T));
                SCI_MEMCPY(call_info_ptr->disp_ptr, make_call_info_ptr->disp_ptr, sizeof(MMI_STRING_T));
                
                if (PNULL != make_call_info_ptr->disp_ptr->wstr_ptr)
                {
                    call_info_ptr->disp_ptr->wstr_ptr = SCI_ALLOCA((make_call_info_ptr->disp_ptr->wstr_len + 1)*sizeof(wchar));
                    SCI_MEMSET(call_info_ptr->disp_ptr->wstr_ptr, 0x00, (make_call_info_ptr->disp_ptr->wstr_len*sizeof(wchar)));
                    MMI_WSTRNCPY(call_info_ptr->disp_ptr->wstr_ptr, 
                        make_call_info_ptr->disp_ptr->wstr_len,
                        make_call_info_ptr->disp_ptr->wstr_ptr,
                        make_call_info_ptr->disp_ptr->wstr_len,
                        make_call_info_ptr->disp_ptr->wstr_len);
                }
            }
            
            if (PNULL != make_call_info_ptr->name_ptr)
            {
                call_info_ptr->name_ptr = (MMI_STRING_T *)SCI_ALLOCA(sizeof(MMI_STRING_T));
                SCI_MEMSET(call_info_ptr->name_ptr, 0x00, sizeof(MMI_STRING_T));
                SCI_MEMCPY(call_info_ptr->name_ptr, make_call_info_ptr->name_ptr, sizeof(MMI_STRING_T));
                
                if (PNULL != make_call_info_ptr->name_ptr->wstr_ptr)
                {
                    call_info_ptr->name_ptr->wstr_ptr = SCI_ALLOCA((make_call_info_ptr->name_ptr->wstr_len + 1)*sizeof(wchar));
                    SCI_MEMSET(call_info_ptr->name_ptr->wstr_ptr, 0x00, (make_call_info_ptr->name_ptr->wstr_len*sizeof(wchar)));
                    MMI_WSTRNCPY(call_info_ptr->name_ptr->wstr_ptr, 
                        make_call_info_ptr->name_ptr->wstr_len,
                        make_call_info_ptr->name_ptr->wstr_ptr,
                        make_call_info_ptr->name_ptr->wstr_len,
                        make_call_info_ptr->name_ptr->wstr_len);
                }
            }
            
#ifdef MV_SUPPORT
            if (MN_DUAL_SYS_MAX == dual_sys)//先进行双卡的选择
#endif
            {
                MMICC_OpenSelectSimWin((ADD_DATA) call_info_ptr);
            }
#ifdef MV_SUPPORT
            else
            {
                if (!is_stk_call)
                {
                     parse_result = MMIAPIPARSE_ParseTeleNum(dual_sys, make_call_info_ptr->tele_num, make_call_info_ptr->tele_len, is_call_video_call);
                }
                       
                if ((MMI_PARSE_CALL_CLIR_SUPPRESSION_E == parse_result)
                    || (MMI_PARSE_CALL_CLIR_INVOCATION_E == parse_result)
                    || (MMI_PARSE_NORMAL_CALL_TYPE_E == parse_result))
                {
                    call_info_ptr->dual_sys = dual_sys;
                    MMICC_OpenMVBeforeCallConnected((ADD_DATA)call_info_ptr);//OpenMagicVoiceSelectWindow((ADD_DATA) call_info_ptr);
                }
                else
                {
                    if (PNULL != call_info_ptr->disp_ptr)
                    {
                        if (PNULL != call_info_ptr->disp_ptr->wstr_ptr)
                        {
                            SCI_FREE(call_info_ptr->disp_ptr->wstr_ptr);
                            call_info_ptr->disp_ptr->wstr_ptr = PNULL;
                        }
                        
                        SCI_FREE(call_info_ptr->disp_ptr);
                        call_info_ptr->disp_ptr = PNULL;
                    }
                    
                    if (PNULL != call_info_ptr->name_ptr)
                    {
                        if (PNULL != call_info_ptr->name_ptr->wstr_ptr)
                        {
                            SCI_FREE(call_info_ptr->name_ptr->wstr_ptr);
                            call_info_ptr->name_ptr->wstr_ptr = PNULL;
                        }
                        
                        SCI_FREE(call_info_ptr->name_ptr);
                        call_info_ptr->name_ptr = PNULL;
                    }

                    SCI_FREE(call_info_ptr);
                    call_info_ptr = PNULL;

#ifdef MMI_NOT_SUPPORT_USSD_SS_INCC
                    if((MMI_PARSE_SS_TYPE_E == parse_result) || (MMI_PARSE_USSD_TYPE_E == parse_result))
                    {
                        SCI_TRACE_LOW("MMICC_ProcessMakeCall:mv_support parse_result:%d\n",parse_result);
                        s_cc_mo_error_type = CC_MO_CMD_NO_SUPPORT;
                        return CC_RESULT_ERROR;
                    }
#endif
                }
            }
#endif
        }
        else
        {
            //SCI_TRACE_MID:"ProcessMakeCall: Alloc memory Fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8075_112_2_18_2_10_23_217,(uint8*)"");
            
            return CC_RESULT_ERROR;
        }
    }
    
    return cc_result;
}

#ifndef MMI_MULTI_GREEN_KEY_SINGLE
/*****************************************************************************/
// 	Description : get call sim type according to call key
//	Global resource dependence : 
//  Author: Michael.shi
//	Note: especially for mulstim.
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_CheckDualSysType(MMI_MESSAGE_ID_E msg_id, MN_DUAL_SYS_E dual_sys)
{
    MN_DUAL_SYS_E new_dual_sys;

    new_dual_sys = dual_sys;
    switch (msg_id)
    {
        case MSG_KEYUP_CALL1: // MSG_KEYUP_GREEN
        case MSG_KEYDOWN_CALL1: //MSG_KEYDOWN_GREEN
            new_dual_sys = MN_DUAL_SYS_1;
            break;
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL2:            
        case MSG_KEYDOWN_CALL2:    
            new_dual_sys = MN_DUAL_SYS_2;
            break;
#endif
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL3:
        case MSG_KEYDOWN_CALL3:    
            new_dual_sys = MN_DUAL_SYS_3;
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL4:
        case MSG_KEYDOWN_CALL4:    
            new_dual_sys = MN_DUAL_SYS_4;
            break;
#endif

        default:
            break;
    }

    return new_dual_sys;
}

/*****************************************************************************/
// 	Description : get call sim type according to call key
//	Global resource dependence : 
//  Author: Michael.shi
//	Note: especially for mulstim.
/*****************************************************************************/
PUBLIC CC_CALL_SIM_TYPE_E MMIAPICC_CheckCallSimType(MMI_MESSAGE_ID_E msg_id, CC_CALL_SIM_TYPE_E sim_type)
{
    CC_CALL_SIM_TYPE_E new_call_sim_type;

    new_call_sim_type = sim_type;
    switch (msg_id)
    {
        case MSG_KEYUP_CALL1:            //MSG_KEYUP_GREEN
        case MSG_KEYDOWN_CALL1:    //MSG_KEYDOWN_GREEN
            new_call_sim_type = CC_SIM1_CALL;
            break;

        case MSG_KEYUP_CALL2:
        case MSG_KEYDOWN_CALL2:    
            new_call_sim_type = CC_SIM2_CALL;
            break;

#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL3:
        case MSG_KEYDOWN_CALL3:    
            new_call_sim_type = CC_SIM3_CALL;
            break;
#endif

#if defined(MMI_MULTI_SIM_SYS_QUAD)
        case MSG_KEYUP_CALL4:
        case MSG_KEYDOWN_CALL4:    
            new_call_sim_type = CC_SIM4_CALL;
            break;
#endif

        default:
            break;
    }

    return new_call_sim_type;
}
#endif /* MMI_MULTI_GREEN_KEY_SINGLE */

/*****************************************************************************/
// 	Description : get call sim type
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC CC_CALL_SIM_TYPE_E MMIAPICC_GetCallSimType(MN_DUAL_SYS_E dual_sys)
{
    CC_CALL_SIM_TYPE_E call_sim_type = CC_CALL_SIM_MAX;

    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        call_sim_type = (CC_CALL_SIM_TYPE_E)(CC_SIM1_CALL + ((CC_CALL_SIM_TYPE_E)dual_sys - MN_DUAL_SYS_1));
    }
    else if(dual_sys == MMI_DUAL_SYS_MAX)
    {
        call_sim_type = CC_CALL_SIM_MAX;
    }
    else
    {
        // do nothing now!!!
    }
        
    //SCI_TRACE_LOW:"MMIAPICC_GetCallSimType dual_sys=%d,call_sim_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8191_112_2_18_2_10_23_218,(uint8*)"dd", dual_sys, call_sim_type);
    
    return call_sim_type;
}



/*****************************************************************************/
// 	Description : if tch not assigned and alerting is indicated, mo start playing
//                local ring to simulate the net ring
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void StartLocalNetRing(void)
{
    //SCI_TRACE_LOW:"ccapp: StartLocalNetRing"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8205_112_2_18_2_10_23_219,(uint8*)"");
    MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, MMI_NT_RING_CON_ID, MMI_1SECONDS,MMISRVAUD_TONE_FREQ_DEFAULT);
    if (0 != s_network_ring_timer)
    {
        MMK_StopTimer(s_network_ring_timer);
        s_network_ring_timer = 0;
    }

    s_network_ring_timer = MMK_CreateTimerCallback(MMICC_3SECOND, PlayLocalNetRing, PNULL, FALSE);
}

/*****************************************************************************/
// 	Description : if tch not assigned and alerting is indicated, mo play
//                local ring to simulate the net ring
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void PlayLocalNetRing(uint8  timer_id, uint32 param)
{
    if (0 != s_network_ring_timer)
    {
        MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, MMI_NT_RING_CON_ID, MMI_1SECONDS,MMISRVAUD_TONE_FREQ_DEFAULT);

        MMK_StartTimerCallback(s_network_ring_timer, MMICC_3SECOND, PlayLocalNetRing, PNULL, FALSE);
    }
}

/*****************************************************************************/
// 	Description : tch assigned or call disconnected, mo stop playing
//                local ring to simulate the net ring
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void StopLocalNetRing(void)
{
    if (0 != s_network_ring_timer)
    {
        //SCI_TRACE_LOW:"ccapp:StopLocalNetRing"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8240_112_2_18_2_10_23_220,(uint8*)"");
        MMK_StopTimer(s_network_ring_timer);
        s_network_ring_timer = 0;
        MMISRVAUD_StopTone(MMISRVAUD_TYPE_TONE_GENERIC);
    }
}



/*****************************************************************************/
//  Description :judge whether the mpty can be setup
//  Global resource dependence : none
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMptySetupAvailable(void)
{
    BOOLEAN is_mpty_setup_available = FALSE;
    uint8 call_index = 0;
    
    //电话数目有2个或以上，并且有电话不在mpty（多方通话）里面，则判定可以建立多方通话
    if (s_call_context.call_number > 1)
    {
        for( call_index = 0; call_index < s_call_context.call_number ; call_index++ )
        {
            if (!s_call_context.call_info[ call_index ].is_in_mpty)
            {
                is_mpty_setup_available = TRUE;
                break;
            }
        }
    }
    //SCI_TRACE_LOW:"ccapp:CC_IsMptySetupAvailable %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8270_112_2_18_2_10_23_221,(uint8*)"d", is_mpty_setup_available);
    return is_mpty_setup_available;
}

/*****************************************************************************/
//  Description :judge whether the mpty can be split
//  Global resource dependence : none
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsMptySplitAvailable(void)
{
    BOOLEAN is_mpty_split_available = FALSE;
    
    //存在mpty（多方通话），并且无法建立mpty，则分离mpty是可行的,如果一个mpty，而且是held，也不能分离
    if (MMICC_IsExistMPTY() && !MMICC_IsMptySetupAvailable() && MMICC_IsExistActiveCall())
    {
        is_mpty_split_available = TRUE;   
    }
    //SCI_TRACE_LOW:"ccapp:CC_IsMptySplitAvailable %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8287_112_2_18_2_10_23_222,(uint8*)"d", is_mpty_split_available);
    return is_mpty_split_available;
}

/*****************************************************************************/
// 	Description : EnableSpeakerMode 
//	Global resource dependence : 
//  Author:bin.ji
//	Note: TRUE to turnon Speaker, TRUE to turnoff Speaker
/*****************************************************************************/
LOCAL BOOLEAN EnableSpeakerMode(
                                BOOLEAN is_turnon
                                )
{
#ifdef WIN32
    return TRUE;
#else   
    BOOLEAN is_enable = !is_turnon;
    return MMICC_EnableRemoteMute(is_enable);
#endif
}

/*****************************************************************************/
//  Description : Release current call: BT Dialer Call or SIM Call
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchAPICC_ReleaseCurrentCall(void)
{
    uint8 is_phone = 1;

    is_phone = ual_bt_get_product_role();
    SCI_TRACE_LOW("ccapp.c WatchAPICC_ReleaseCurrentCall, current product mode = %d", is_phone);

    if(0 == is_phone)
    {
#ifdef UAL_BT_HFU_SUPPORT
        MMIAPICC_ReleaseBtCall();
#endif
    }
    else
    {
        if ((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus())
             || (CC_WAITING_STATE == MMICC_GetCurrentCallStatus()))
        {
            CC_ReleaseCallReq(s_call_context.call_info[s_call_context.current_call].dual_sys, s_call_context.current_call, MN_CAUSE_USER_BUSY);
        }

        if (CC_CALLING_STATE == MMICC_GetCurrentCallStatus()
            || CC_CALL_STATE == MMICC_GetCurrentCallStatus())
        {
            MMICC_ClearWantCallInfo();
            CC_ReleaseCallReq(s_call_context.call_info[s_call_context.current_call].dual_sys, s_call_context.current_call, MN_CAUSE_NORMAL_CLEARING);
        }
    }
}

/*****************************************************************************/
// 	Description : Release call by red key: release incoming and outgoing call in high priority;
//                active call normal priority; hold call low priority
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:2004.10.14
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_ReleaseCallByRedkey(void)
{    
    //如果有来电去电，优先挂断
    //SCI_TRACE_LOW:"ccapp:MMIAPICC_ReleaseCallByRedkey()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8316_112_2_18_2_10_23_223,(uint8*)"");
    if ((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()) ||
    	(CC_WAITING_STATE == MMICC_GetCurrentCallStatus()))
    {
        return CC_ReleaseCallReq(s_call_context.call_info[s_call_context.current_call].dual_sys, s_call_context.current_call, MN_CAUSE_USER_BUSY);
    }

    if (CC_CALLING_STATE == MMICC_GetCurrentCallStatus())
    {
        MMICC_ClearWantCallInfo();
        return CC_ReleaseCallReq(s_call_context.call_info[s_call_context.current_call].dual_sys, s_call_context.current_call, MN_CAUSE_NORMAL_CLEARING);
	}
    
    //激活电话存在，挂断
    if (MMICC_IsExistActiveCall())
    {
        return CC_ReleaseActiveCall();
    }
    
    //最后挂断保留电话
    if (MMICC_IsExistHoldCall())
    {
        return CC_ReleaseHoldCall();
    }

    if( MMK_IsOpenWin(MMICC_AUTO_DIAL_WIN_ID) )
    {
        MMICC_ClearAutoRedialInfo();
        MMK_CloseWin( MMICC_AUTO_DIAL_WIN_ID );
    }
    
    return CC_RESULT_ERROR;
}


/*****************************************************************************/
// 	Description : 获取当前呼叫的类型
//	Global resource dependence : 
//  Author:xiongxiaoyan 
//	Note:20060404
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_GetCallDualSys(void)
{
    return s_call_context.dual_sys;
}

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICC_GetMissedCallNumber(void)
{
    return s_call_context.missed_call_num;
}

/*****************************************************************************/
// 	Description : dec missed call number
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_DecMissedCallNumber(uint32 call_times)
{
    s_call_context.missed_call_num -= call_times;
    if (s_call_context.missed_call_num == 0)
    {
        //关闭idle状态下提示未接电话的窗口以及状态栏图标
        MMIAPICC_ClearMissedCallNum();
    }
    MMIAPICC_SaveMissedCallNumToNv();
    MMIAPICC_ShowMCHint();
}

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_SaveMissedCallNumToNv(void)
{
    MMINV_WRITE(MMI_CC_MISSED_CALL_NUM, (&(s_call_context.missed_call_num)));
}

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPICC_LoadMissedCallNumFromNv(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    uint32 missed_call_num = 0;

    MMINV_READ(MMI_CC_MISSED_CALL_NUM, &missed_call_num, return_value);
    
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMI_CC_MISSED_CALL_NUM, &missed_call_num);
    }

    s_call_context.missed_call_num = missed_call_num;

    if (missed_call_num > 0)
    {
        MAIN_SetMissedCallState(TRUE);
    }
    else //missed_call_num == 0
    {
        MAIN_SetMissedCallState(FALSE);
    }
    
    return return_value;        
}




/*****************************************************************************/
// 	Description : 接收ps消息的时候，对于at cmd发起的电话，mmi不做任何处理，界面需要返回idle
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 返回true,代表是at cmd发起的call，则消息不做处理，反之，是mmi发起call.
/*****************************************************************************/
LOCAL BOOLEAN FilterMsgFromATCmdCall( 
                                      MMI_MESSAGE_ID_E    msg_id ,
                                      DPARAM  param
                                      )
{
    BOOLEAN result = FALSE;
    if (s_dial_method >= CC_DIAL_METHOD_MAX)
    {
        //SCI_TRACE_LOW:"ccapp: dial method error %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8438_112_2_18_2_10_24_224,(uint8*)"d", s_dial_method);
        return FALSE;
    }

    switch(msg_id) 
    {
        case APP_MN_CALL_START_IND:
            if (CC_DIAL_METHOD_MMI == s_dial_method)
            {
                s_dial_method = CC_DIAL_METHOD_NULL;
                result = FALSE;
            }
            else if(CC_DIAL_METHOD_OTHER == s_dial_method)
            {
                s_call_context.call_number++;
                result = TRUE;
            }
            else if(CC_DIAL_METHOD_NULL == s_dial_method)
            {
//Bug 1312367 - [UMS9117][FIH_DUN2][Queen2][6798] 112 call fail with China Telecom
#ifdef MMI_VOLTE_SUPPORT
                if(PNULL != param)
                {
                    APP_MN_CALL_START_IND_T	*sig_ptr = (APP_MN_CALL_START_IND_T*)param;
                    SCI_TRACE_LOW("volte ccapp.c receive APP_MN_CALL_START_IND multiple, update call path: dual_sys=%d, is_ip_call=%d", 
                        sig_ptr->dual_sys, sig_ptr->is_ip_call);
                    CC_SetCallPathStatus(sig_ptr->dual_sys,sig_ptr->is_ip_call);
                }
#else
                s_dial_method = CC_DIAL_METHOD_OTHER;
                s_call_context.call_number++;
                //reset cc para & return to idle win
                resetCallPara();
                MMK_ReturnAppointedWin(MAIN_IDLE_WIN_ID);
                //MMK_CloseMiddleWin(MAIN_IDLE_WIN_ID, MMICC_STATUS_WIN_ID);
#endif
                result = TRUE;
            }
    	    break;

        case APP_MN_SETUP_IND:
            if (CC_DIAL_METHOD_OTHER == s_dial_method)
            {
                s_call_context.call_number++;
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
            break;

        case APP_MN_CALL_DISCONNECTED_IND:
        case APP_MN_CALL_ERR_IND:
            if (CC_DIAL_METHOD_OTHER == s_dial_method)
            {
                s_call_context.call_number--;
                if (0 == s_call_context.call_number)
                {
                    s_dial_method = CC_DIAL_METHOD_NULL;
                    MAIN_SetCallingState(FALSE);
                }
                result = TRUE;
            }
            else if (CC_DIAL_METHOD_MMI == s_dial_method)
            {
                s_dial_method = CC_DIAL_METHOD_NULL;
                MAIN_SetCallingState(FALSE);
                result = FALSE;
            }
            else
            {
                result = FALSE;
            }
            break;
        default:
            if (CC_DIAL_METHOD_OTHER == s_dial_method)
            {
                result = TRUE;
            }
            else
            {
                result = FALSE;
            }
            break;
    }
    return result;

}

/*****************************************************************************/
// 	Description : 重置cc的各项参数，当判定为at cmd发起的call时候进行调用
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL void resetCallPara( void )
{
    int index = 0;
    s_call_context.exist_call.exist_active_call_num = 0;
    s_call_context.exist_call.exist_hold_call_num = 0;
    s_call_context.exist_call.exist_incoming_call = FALSE;
    s_call_context.exist_call.exist_mpty = FALSE;
    s_call_context.exist_call.exist_mtfw = FALSE;
    s_call_context.exist_call.exist_outgoing_call = FALSE;

    s_call_context.incoming_call_index = CC_INVALID_INDEX;

    s_call_context.call_time_count = 0;

    s_call_context.current_call = CC_INVALID_INDEX;

    for (index = 0; index < s_call_context.call_number; index++)
    {
        SCI_MEMSET(&s_call_context.call_info[index], 0, sizeof(s_call_context.call_info[index]));
        s_call_context.call_info[index].call_id = CC_INVALID_ID;
    }

}



#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : 处理含'+'  的电话号码
//	Global resource dependence : 
//  Author: koki
//	Note: 
/*****************************************************************************/
LOCAL void MMICC_TTSValidNum ( wchar	        *innumber_str,			//in
                                                                uint32          instr_len,			//in
                                                                wchar		*outnumber_str,		//out
                                                                uint32	        outstr_len		//in
                                                            )
{

        uint32		i = 0;
        uint32		dest_str_len = 0;
        if(PNULL == innumber_str || PNULL == outnumber_str)
        {
            return ;
        }
        for (i=0; i<instr_len; i++)
        {
            outnumber_str[dest_str_len] = innumber_str[i];
            dest_str_len++;
            if(outstr_len < dest_str_len)
            {
                break;
            }
            if ( '+' == innumber_str[i])
            {
                outnumber_str[dest_str_len] =' ';
                dest_str_len++;
                if(outstr_len < dest_str_len)
                {
                    break;
                }
            }
        }
}


/*****************************************************************************/
//  Description : play incoming call name, play name and 来电
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IncomingCallName_To_Speech(void)
{
    MMI_STRING_T   no_name_str       = {0};
    //I_STRING_T   unknow_num_str    = {0}; //@fen.xie MS00176958
    
    wchar temp_wchar[CC_MAX_PBNAME_LEN+1] = {0};
    wchar callnum_wchar[CC_MAX_PBNAME_LEN+1] = {0};
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2] = {0};    
    uint8 tel_num_len = 0;
    
    wchar  play_text[CC_MAX_PBNAME_LEN + 13] = {0};
    uint16 play_len = 0;
    wchar* name_str_ptr = PNULL;
    MMI_STRING_T incoming_call = {0};
    //wchar  incoming_call[6] = {0x0020, 0x6765, 0x7535, 0x0020, 0xFF0C, 0}; // 来电...
    MMI_STRING_T current_name_str = {0};
    MMITTS_PLAY_PARAM_T play_param = {0};

    BOOLEAN is_call_name_exist = FALSE;
    BOOLEAN is_call_number_exist = FALSE;

    SCI_MEMSET(&incoming_call, 0, sizeof(MMI_STRING_T));
    MMI_GetLabelTextByLang(TXT_TTS_INCOMING_CALL_STRING, &incoming_call);
    MMIAPITTS_StopPlayTextByType(TTS_RING_TYPE_CALL);
    
    SCI_MEMSET(play_text, 0, sizeof(play_text));

    is_call_name_exist = MMICC_GetCallNameStrByIndex(&current_name_str, MMICC_GetCurrentCallIndex())
                                    &&(!MMIAPIPB_IsUseDefaultName(current_name_str.wstr_ptr))
                                    &&(0 != current_name_str.wstr_len);
    if(is_call_name_exist)
    {
        name_str_ptr = temp_wchar;
        MMICC_TTSValidNum(current_name_str.wstr_ptr, CC_MAX_PBNAME_LEN, name_str_ptr, CC_MAX_PBNAME_LEN);
        play_len = MMIAPICOM_Wstrlen(name_str_ptr);
    }
    else
    {
        is_call_number_exist = MMICC_GetCallNumStrByIndex(tele_num, &tel_num_len, MMICC_GetCurrentCallIndex());
        if(is_call_number_exist)
        {
            //if (MMIAPICOM_IsValidNumberString((char*)tele_num, tel_num_len))
            if (0 != tel_num_len)
            {
                no_name_str.wstr_ptr = temp_wchar;        
                no_name_str.wstr_len = tel_num_len;
                MMI_STRNTOWSTR(no_name_str.wstr_ptr, no_name_str.wstr_len, tele_num,  tel_num_len, tel_num_len); /*lint !e64*/
                name_str_ptr = callnum_wchar;
                MMICC_TTSValidNum(no_name_str.wstr_ptr, no_name_str.wstr_len, name_str_ptr, CC_MAX_PBNAME_LEN);
                play_len = MMIAPICOM_Wstrlen(name_str_ptr);            
            }
#if 0
            else
            {
                //SCI_TRACE_LOW:"ccapp IncomingCallName_To_Speech error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8641_112_2_18_2_10_24_225,(uint8*)"");
                return FALSE;
            }
#endif
        }
        else
        {
            return FALSE;
            /* YOUNGTONE的tts不支持中文，这种情况下无声
            // 隐藏号码的手机来电,语音播报"未知号码"
            MMI_GetLabelTextByLang(TXT_UNKNOW_NUM, &unknow_num_str);
            name_str_ptr = unknow_num_str.wstr_ptr;
            play_len = unknow_num_str.wstr_len;*/
        }
    }
    
    MMIAPICOM_Wstrncpy(play_text, name_str_ptr, play_len);
    MMIAPICOM_Wstrncat(play_text, incoming_call.wstr_ptr,incoming_call.wstr_len);
    play_len = MMIAPICOM_Wstrlen(play_text)*2;
    
    play_param.txt_addr = (uint8 *)play_text;
    play_param.txt_len_in_byte = play_len;
    play_param.is_ucs2 = TRUE;
    play_param.play_times = 0;
    play_param.tts_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_CALL);
    play_param.ring_type = TTS_RING_TYPE_CALL;
	play_param.tts_route = MMISRVAUD_ROUTE_EARFREE;
	play_param.digit_mode = TTS_DIGIT_NUMBER;
	play_param.punc_mode = TTS_PUNC_OFF;	
    return MMIAPITTS_PlayText(&play_param);	
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IncomingCallName_To_Speech(void)
{
    BOOLEAN ret_value = FALSE;
#ifdef VT_SUPPORT
    MMI_STRING_T   call_name = {0};
    MMI_STRING_T   unknow_num_str = {0};
    uint16   play_len       = 0;
    wchar   report_wchar[CC_MAX_PBNAME_LEN + 13] = {0};
    wchar   temp_wchar[CC_MAX_PBNAME_LEN+1] = {0};
    wchar   incoming_call[6] = {0x0020, 0x6765, 0x7535, 0x0020, 0x002e, 0}; 
    MMITTS_PLAY_PARAM_T play_param = {0};
#endif

    if(!MMIAPISET_GetTTSInfo())
    {
        return FALSE;
    }

#ifdef VT_SUPPORT
     if (MMIAPIVT_IsVtCalling())
    {
        MMIAPIVT_GetCallName(&call_name);
        if (MMIAPIPB_IsUseDefaultName(call_name.wstr_ptr))
        {
            uint8 tele_num[MMIVT_PHONE_NUM_MAX_LEN + 2] = {0};
            uint8 tele_len;
            MMI_STRING_T   no_name_str   = {0}; 
            no_name_str.wstr_ptr=temp_wchar;
            MMIVT_GetCallNormalNumber(tele_num,&tele_len);
            if (tele_len >0)
            {
                no_name_str.wstr_len = tele_len;
                MMI_STRNTOWSTR(no_name_str.wstr_ptr, no_name_str.wstr_len, tele_num,  tele_len, tele_len); /*lint !e64*/
            }
            else
            {
                return FALSE;
            }
            MMICC_TTSValidNum(no_name_str.wstr_ptr, no_name_str.wstr_len, report_wchar, CC_MAX_PBNAME_LEN);
        }
        else if(0 == call_name.wstr_len)
        {
            MMI_GetLabelTextByLang(TXT_UNKNOW_NUM, &unknow_num_str);
            MMIAPICOM_Wstrncat(report_wchar, unknow_num_str.wstr_ptr,unknow_num_str.wstr_len);
        }
        else 
        {
            MMIAPICOM_Wstrcat(report_wchar, call_name.wstr_ptr);
        }
        MMIAPICOM_Wstrcat(report_wchar, incoming_call);
        play_len = MMIAPICOM_Wstrlen(report_wchar)*2;
        play_param.txt_addr = (uint8 *)report_wchar;
        play_param.txt_len_in_byte = play_len;
        play_param.is_ucs2 = TRUE;
        play_param.play_times = 0;
        play_param.tts_vol = MMIAPIENVSET_GetActiveModeRingVol(MMISET_RING_TYPE_CALL);
        play_param.ring_type = TTS_RING_TYPE_CALL;
		play_param.tts_route = MMISRVAUD_ROUTE_EARFREE;
		play_param.digit_mode = TTS_DIGIT_NUMBER;
		play_param.punc_mode = TTS_PUNC_OFF;
		
        ret_value = MMIAPITTS_PlayText(&play_param);	
     }
     else
 #endif
    {
         ret_value = IncomingCallName_To_Speech();
    }
     return ret_value;
}

#endif
/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsCallByMMI(void)
{
    return ((s_dial_method != CC_DIAL_METHOD_OTHER) && (s_dial_method != CC_DIAL_METHOD_MAX));
}

/*****************************************************************************/
// 	Description : 没有VT功能的时候，收到VT来电，需要挂断电话，并回CAUSE:88
//	Global resource dependence : 
//  Author: SAMBOO
//	Note: return True means vt incoming call is rejected
/*****************************************************************************/
LOCAL BOOLEAN HandleIncomingMsgWhenVTNotSupport( uint16 msg_id, DPARAM param )
{
    BOOLEAN result = FALSE;
    //param 不为NULL
    if (PNULL == param)
    {
        //SCI_TRACE_LOW:"mmivt null param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8760_112_2_18_2_10_24_226,(uint8*)"");
        return FALSE;
    }
    if (APP_MN_SETUP_IND == msg_id)
    {
        APP_MN_SETUP_IND_T* info = (APP_MN_SETUP_IND_T*)param;
        if (MN_CALL_TYPE_DATA == info->call_type)
        {
            MNCALL_DisconnectCallEx(info->dual_sys, info->call_id, MN_CAUSE_INCOMPAT_DEST, PNULL);
            result = TRUE;
        }
    }
    return result;    
}

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 当收到APP_MN_SYNC_IND,没有收到APP_MN_SETUP_COMPLETE_CNF和APP_MN_PROGRESS_IND时
//  设置为下行静音,收到APP_MN_SETUP_COMPLETE_CNF和APP_MN_PROGRESS_IND后,关闭静音
/*****************************************************************************/
LOCAL BOOLEAN  HandleMoMuteAndLocalRingState(uint16 msg_id)
{
    //只在第一MO电话进行处理
    if((s_call_context.call_number != 1) || (CC_CALLING_STATE != s_call_context.call_info[s_call_context.current_call].call_state))
    {
        s_is_downlnk_mute_full_circle = FALSE;
        return FALSE;
    }
	
    if (!s_is_downlnk_mute_full_circle)
    {
        return FALSE;
    }
    SCI_TRACE_LOW("volte ccapp HandleMoMuteAndLocalRingState s_is_downlnk_mute_mocalling_flag =%d", s_is_downlnk_mute_mocalling_flag);
    switch(msg_id)
    {
    case APP_MN_SYNC_IND:
        if(MO_CALL_STATE_INIT == s_is_downlnk_mute_mocalling_flag)
        {
            MMICC_EnableNetRemoteMute(TRUE);
            s_is_downlnk_mute_mocalling_flag  = MO_CALL_STATE_SYNC;
        }				
        else
        {
            s_is_downlnk_mute_mocalling_flag |= MO_CALL_STATE_SYNC;
        }
        //处理先来sync_ind消息比setup_complete_cnf/progress_ind消息来得晚的情况
        if(MO_CALL_STATE_DONE == s_is_downlnk_mute_mocalling_flag)
        {
            MMICC_EnableNetRemoteMute(FALSE);			
            StopLocalNetRing();
        }
        break;

    case APP_MN_PROGRESS_IND:
        if(MO_CALL_STATE_SYNC == s_is_downlnk_mute_mocalling_flag)
        {
            MMICC_EnableNetRemoteMute(FALSE);			
            s_is_downlnk_mute_mocalling_flag = MO_CALL_STATE_DONE;
            StopLocalNetRing();
        }
        else
        {
            s_is_downlnk_mute_mocalling_flag |= MO_CALL_STATE_SETUP_OR_PROGRESS;
        }
        break;

    case APP_MN_SETUP_COMPLETE_CNF:
        MMICC_EnableNetRemoteMute(FALSE);			
        s_is_downlnk_mute_mocalling_flag = MO_CALL_STATE_DONE;
        StopLocalNetRing();
        break;


    //恢复初试值
    case APP_MN_CALL_DISCONNECTED_IND:
    case APP_MN_CALL_ERR_IND:
        if(MO_CALL_STATE_SYNC == s_is_downlnk_mute_mocalling_flag)
        {
            MMICC_EnableNetRemoteMute(FALSE);
        }
        s_is_downlnk_mute_mocalling_flag = MO_CALL_STATE_INIT;
        StopLocalNetRing();
        break;

    case APP_MN_ALERTING_IND:
        if (MO_CALL_STATE_DONE != s_is_downlnk_mute_mocalling_flag)
        {
            StartLocalNetRing();
        }
        break;

    default:
        //do nothing
        break;
    }

    //SCI_TRACE_LOW:"ccapp HandleMoMuteAndLocalRingState %d,msg_id= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8840_112_2_18_2_10_25_227,(uint8*)"dd",s_is_downlnk_mute_mocalling_flag,msg_id);

    return TRUE;
}

/*****************************************************************************/
//  Discription: get the message ID_STATUS_HFAG_HFU_HOLD
//  Global resource dependence: none 
//  Author: kelly.li 
//	Note:
//
/*****************************************************************************/

PUBLIC void MMIAPICC_HandleBtHfuHoldInd(BT_MSG_T     *msg_body_ptr )
{
#ifdef	BLUETOOTH_SUPPORT
    
    BT_DIAL_ACTION_T		hold_info = *((BT_DIAL_ACTION_T* )msg_body_ptr->body_ptr);
    
    //SCI_TRACE_LOW:"CC_HandleBtHfuHoldInd: the hf hold action is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8857_112_2_18_2_10_25_228,(uint8*)"d",hold_info.hold_action);
    //call cc function according hold action
    if (hold_info.hold_action == BT_HOLD_RELEASE_HELD_CALLS)
    {
        //CC_ReleaseHoldCall();
        // hangup the incoming call
        SCI_TRACE_LOW("zyx,receive at+chld=0");
        if((CC_INCOMING_CALL_STATE == MMICC_GetCurrentCallStatus()) ||
            (CC_WAITING_STATE == MMICC_GetCurrentCallStatus()))
        {
            CC_ReleaseCallReq(MMIAPICC_GetCallDualSys(), MMICC_GetCurrentCallIndex(), MN_CAUSE_USER_BUSY);
            SCI_TRACE_LOW("zyx,update call setup status");
#ifdef BT_BQB_SUPPORT
            MMIAPIBT_CIEVCommandIndicator(BT_CALL_SETUP_STATUS_INDICATOR,0);
#endif
        }
        else
        {
            SCI_TRACE_LOW("zyx,CC_ReleaseHoldCall");
            CC_ReleaseHoldCall();
        }
        //SCI_TRACE_LOW:"CC_HandleBtHfuHoldInd: release incoming call"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8871_112_2_18_2_10_25_229,(uint8*)"");
    }
    else if (hold_info.hold_action == BT_HOLD_RELEASE_ACTIVE_CALLS)
    {
        //此处暂时调用CRSS相关方法来完成，需要关注该宏MMI_CRSS_SUPPORT的开关!
#ifdef MMI_CRSS_SUPPORT
        CC_CRSSReleaseAndAnswer();
#else
        SCI_TRACE_LOW("CRSS no longer supported! We should rewrite the func for BT_HOLD_RELEASE_ACTIVE_CALLS.");
#endif
    }
    else if (hold_info.hold_action == BT_HOLD_HOLD_ACTIVE_CALLS)
    {
        //Places all active calls (if any exist) on hold and accepts the other (held or waiting) call.
        if (s_call_context.exist_call.exist_incoming_call)
    {
            CC_HoldAndAnswerCW();
            //SCI_TRACE_LOW:"CC_HandleBtHfuHoldInd: CC_HoldAndAnswerCW"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8892_112_2_18_2_10_25_232,(uint8*)"");
    }
        else if(s_call_context.exist_call.exist_active_call_num > 0 
            || s_call_context.exist_call.exist_hold_call_num > 0
            && s_call_context.exist_call.exist_outgoing_call == 0)
    {
            CC_HoldCall();   
            //SCI_TRACE_LOW:"CC_HandleBtHfuHoldInd: CC_HoldCall"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8899_112_2_18_2_10_25_233,(uint8*)"");
        }
        else
        {
            //SCI_TRACE_LOW:"CC_HandleBtHfuHoldInd: the hold action is not support!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8903_112_2_18_2_10_25_234,(uint8*)"");
        }
    }

	else if (hold_info.hold_action == BT_HOLD_ADD_HELD_CALL)
	{
		if(MMICC_IsExistActiveAndHoldCalls())
		{
			CC_BuildMPTYCall();
			SCI_TRACE_LOW("CC_HandleBtHfuHoldInd: BT_HOLD_ADD_HELD_CALL");
		}
		else
		{
			SCI_TRACE_LOW("CC_HandleBtHfuHoldInd: no active call or hold call");
		}
	}
    else
    {
        //SCI_TRACE_LOW:"CC_HandleBtHfuHoldInd: the hold action is not support!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8908_112_2_18_2_10_25_235,(uint8*)"");
        }        
#endif
}



/*****************************************************************************/
// 	Description : 是否包含限制呼叫的号码
//	Global resource dependence : s_eng_call_barring_num
//  Author:fen.xie
//	Note:MS00186848
/*****************************************************************************/
LOCAL BOOLEAN IsIncludeBarringNumbers(	
                            const uint8* tel_num_str,		// input phone number string in ASCII
                            const uint8  tel_num_length  // string length
                            )
{
    const uint8 *temp_num = PNULL;
    uint32 i = 0, j = 0;
    uint8 flag_len = 0;

    //SCI_TRACE_LOW:"***IsIncludeBarringNumbers()***"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_8928_112_2_18_2_10_25_236,(uint8*)"");

    if ((PNULL == tel_num_str) || (0 == tel_num_length))
    {
        return FALSE;
    }
    
    for (i = 0; i < ARR_SIZE(s_eng_call_barring_num); i++)
    {
        flag_len = strlen(s_eng_call_barring_num[i]);

        temp_num = tel_num_str;
        
        for (j = 0; j < tel_num_length; j++)
        {
            if (strncmp((char *)temp_num, s_eng_call_barring_num[i], flag_len) == 0)
            {
                return TRUE;
            }

            if ((tel_num_length - j) <= flag_len)
            {
                break;//not found
            }

            temp_num++;
        }
    }
    
    return FALSE;
    
}
/*****************************************************************************/
// 	Description : 是否要正常对待当前的这通来电...
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsAcceptCurCallAsNormal(void)
{
    BOOLEAN  ret_result = TRUE;
#ifdef ASP_SUPPORT
    // 看看用户有没有接听电话....
    ret_result = s_accept_call;

    s_accept_call = FALSE;
#endif

    return ret_result;
}


#ifdef ASP_SUPPORT
/*****************************************************************************/
// 	Description : 是否要正常对待当前的这通来电...
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICC_SetAcceptCurCall(BOOLEAN is_accept_call)
{
    s_accept_call = is_accept_call;
}
#endif
/*****************************************************************************/
// 	Description : to get the number of call
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetCallNumber( 
                                   uint8* number_ptr, //out
                                   uint8 number_max_len
                                   )
{
    MN_CALLED_NUMBER_T   cur_call_number = {0};
    uint8                tel_num_len     = 0;
    
    if(CC_MAX_TELE_NUM_LEN + 2 > number_max_len)
    {
        //SCI_TRACE_LOW:"MMIASP MMIAPICC_GetCallNumber number str len mayber is not enough !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_9002_112_2_18_2_10_25_237,(uint8*)"");
        SCI_MEMSET(number_ptr, 0, number_max_len);
    }
    else
    {

        cur_call_number = s_call_context.call_info[s_call_context.current_call].address;
        
        tel_num_len =  MMIAPICOM_GenNetDispNumber(cur_call_number.number_type,
            (uint8)cur_call_number.num_len,
            (uint8*)cur_call_number.party_num,
            number_ptr,
            CC_MAX_TELE_NUM_LEN + 2
            );
        
        //电话号码赋值
        if (0 == tel_num_len)
        {
            SCI_MEMSET(number_ptr, 0, number_max_len);
            tel_num_len = 0;
        }
    }

    return tel_num_len;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_AnswerCall(void)
{
    uint8 is_phone = 1;
    BOOLEAN ret = FALSE;
    ual_bt_status_e	bt_status = UAL_BT_STATUS_ERROR;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif

    MMICC_StopRingOrVibrateBeforeCCing(); //modified by jun.hu for 此处对TTS采用异步停止的方式，以解决接听要2S的问题

    if( 1 == is_phone )
    {
        if( !MMICC_IsCurrentCallIDValid() || CC_IsExistOperatedCall() )
        {
            //can not do other handle in this state
            return FALSE;
       }
        //根据当前CALL的状态进行相应的处理
        switch(MMICC_GetCurrentCallStatus())
        {
            case CC_CALLING_STATE:
#ifdef CMCC_UI_STYLE
        {
            uint8   tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
            uint16  tel_num_len = 0;
            BT_ADDRESS   address = {0};
            
            tel_num_len = MMIAPICOM_GenDispNumber(s_call_context.want_mo_call.address.number_type, (uint8)s_call_context.want_mo_call.address.num_len, s_call_context.want_mo_call.address.party_num,tel_num,(uint8)sizeof(tel_num));
                
            if (MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, s_call_context.want_mo_call.dual_sys, TRUE)
                && s_call_context.exist_call.exist_outgoing_call
                && !MMIAPIBT_GetActivatedHS(&address))
            {
                MMICC_OpenEmergency_OptionMenu();
            }
        }
#endif
            break;

            case CC_WAITING_STATE:

		        if(1 != MMICC_GetCallNum())
                {
                    MMICC_OpenCCMenu();
                }
            break;

            case CC_INCOMING_CALL_STATE:
            {    
#ifdef MV_SUPPORT
                //只有第一通电话时才主动打开魔音选择窗口
                if (IsCanUseMagicVoice(TRUE) && !MMK_IsFocusWin(MMICC_MAGIC_VOICE_MAIN_SELECT_WIN_ID)&& (1 == MMICC_GetCallNum()))
                {
                    MMICC_OpenMVBeforeCallConnected(NULL);//OpenMagicVoiceSelectWindow(NULL);
#ifdef ASP_SUPPORT
                    //stop asp timer to avoid answering call when setting mv
                    MMIAPIASP_StopWaitTimer();
#endif
                    return FALSE;
                }
#endif
                //accept call
                if(CC_RESULT_ERROR == CC_ConnectCallReq())			
                {
                   //显示失败
                }
            }
            break;

            default:
            //SCI_TRACE_LOW:"ccapp.c: CC_HandleCcWinMsg() call_state = %d"
              SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_9084_112_2_18_2_10_25_238,(uint8*)"d", MMICC_GetCurrentCallStatus());
            break;
        }
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT    
        bt_status = ual_bt_anwser_call();
#endif
#ifndef WIN32
        SCI_TRACE_LOW("%s: bt_status = %d ", __FUNCTION__,bt_status);
#endif
    }
    return TRUE;
}
#ifdef ASP_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:jun.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_AnswerASPCall(void)
{
    MMICC_StopRingOrVibrateBeforeCCing(); //modified by jun.hu for 此处对TTS采用异步停止的方式，以解决接听要2S的问题
    
    MMIAPIASP_StopWaitTimer();
    if(MMIAPIASP_IsMachineWorking())
    {
        MMIAPIASP_StopCallAnswer();
    }
    
    if( ( !MMICC_IsCurrentCallIDValid()) || CC_IsExistOperatedCall() )
    {
        //can not do other handle in this state
        return FALSE;
    }
    
    //accept call
    if(CC_RESULT_ERROR == CC_ConnectCallReq())			
    {
        //SCI_TRACE_LOW:"AnswerASPCall fail "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_9114_112_2_18_2_10_25_239,(uint8*)"");
    }
    
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description : response call control
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ResponseCallCtl(MN_DUAL_SYS_E dual_sys,
                                                                                    MMI_STRING_T *alert_str_ptr,
                                                                                    MMI_STRING_T *display_str_ptr,
                                                                                    MN_CALLED_NUMBER_T *cc_address,
                                                                                    SIMAT_SIM_CONTROL_RESULT_E result,
                                                                                    SIMAT_SIMC_DATA_TYPE_E data_type,
                                                                                    BOOLEAN is_need_modified)
{
    //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_ResponseCallCtl dual_sys=%d, is_need_modified=%d, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_9133_112_2_18_2_10_25_240,(uint8*)"ddd", dual_sys, is_need_modified, result);

    if (SIMAT_SIMC_ADDRESS == data_type && PNULL != cc_address && cc_address->num_len > 0 && is_need_modified)
    {
        wchar temp_wchar[MMICC_PHONE_NUM_MAX_LEN + 3] = {0};
        uint8 tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
        uint8 tel_num_len = 0;
        uint8 copy_len = 0;
        BOOLEAN b_search_result = FALSE;
        MMI_STRING_T	name = {0};
        uint8 pb_group_id = 0;
        uint16	 pb_entry_id = 0;
        MMI_STRING_T prompt_str= {0};
        BOOLEAN is_emergency_call = FALSE;
        
        if (SIMAT_SIMC_ALLOWED_AND_MODIFIED == result)
        {
            MMIFDN_CONTEXT_T *fdn_context_ptr = PNULL;
            
            fdn_context_ptr = MMIAPIFDN_GetFDNContext(dual_sys);
            
            if (fdn_context_ptr->is_fdn_enable)//fdn enable&modified call not in FDN list
            {
                uint32 i = 0;
                PHONEBOOK_ENTRY_T *ldn_entry_ptr = PNULL;
                
                ldn_entry_ptr = SCI_ALLOCA(sizeof(PHONEBOOK_ENTRY_T));
                
                if (PNULL == ldn_entry_ptr)
                {
                    //SCI_TRACE_LOW:"MMICC: MMIAPICC_ResponseCallCtl No Memory"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_9163_112_2_18_2_10_25_241,(uint8*)"");
                    
                    return FALSE;
                }
                
                for (i = 1; i <= fdn_context_ptr->fdn_info.max_record_num; i++)
                {
                    if (fdn_context_ptr->entry_state[i - 1])
                    {
                        SCI_MEMSET(ldn_entry_ptr, 0x00, sizeof(PHONEBOOK_ENTRY_T));
                        
                        ldn_entry_ptr->entry_id = i;
                        
                        if (ERR_MNDATAMAG_NO_ERR == PHONEBOOK_ReadPhonebookEntryEx(dual_sys, PHONEBOOK_FDN_STORAGE, ldn_entry_ptr))
                        {
                            if (ldn_entry_ptr->number_len == cc_address->num_len
                                && ldn_entry_ptr->npi == cc_address->number_plan
                                && ldn_entry_ptr->ton == cc_address->number_type
                                && (0 == memcmp(ldn_entry_ptr->number, cc_address->party_num, ldn_entry_ptr->number_len)))
                            {
                                s_is_call_modified_store = TRUE;
                                
                                break;
                            }
                        }
                    }
                }
                
                SCI_FREE(ldn_entry_ptr);
            }
        }
        
        if (s_is_call_modified_store)
        {
            SCI_MEMSET(&s_call_modified_address, 0x00, sizeof(s_call_modified_address));
            SCI_MEMCPY(&s_call_modified_address, &s_call_context.want_mo_call.address, sizeof(s_call_modified_address));
        }
        
        tel_num_len = MMIAPICOM_GenNetDispNumber(
            cc_address->number_type, 
            (uint8)cc_address->num_len, 
            cc_address->party_num,
            tel_num,
            sizeof(tel_num)
            );
        
        s_call_context.want_mo_call.address.number_type = cc_address->number_type ;
        s_call_context.want_mo_call.address.number_plan = cc_address->number_plan;
        
        SCI_MEMSET((void*)s_call_context.want_mo_call.address.party_num, 0xFF, sizeof(s_call_context.want_mo_call.address.party_num));
        
        copy_len = ( cc_address->num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: cc_address->num_len;
        MMI_MEMCPY(
            (void*)s_call_context.want_mo_call.address.party_num,
            sizeof(s_call_context.want_mo_call.address.party_num),
            (void*)cc_address->party_num,
            sizeof(cc_address->party_num),
            copy_len
            );
        s_call_context.want_mo_call.address.num_len = copy_len ;
        
        //调用pb接口获得相应的电话信息
        SCI_MEMSET(s_call_context.want_mo_call.name, 0x00, sizeof(s_call_context.want_mo_call.name));
        name.wstr_ptr = s_call_context.want_mo_call.name;
        
        is_emergency_call = MMIAPICC_IsEmergencyNum(tel_num, tel_num_len, dual_sys, FALSE);
        
        if(is_emergency_call)
        {
            MMI_STRING_T clear_str = {0};
            
            MMI_GetLabelTextByLang(
                TXT_KL_PSW_EMERGENCY_CALL,
                &prompt_str
                );
            
#ifndef MMI_GUI_STYLE_TYPICAL
            GUILABEL_SetText(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, &clear_str, TRUE);
            GUILABEL_SetText(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, &clear_str, TRUE);
            GUILABEL_SetText(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, &prompt_str, TRUE);
            GUILABEL_SetText(MMICC_CONNECTED_LOCAT_LABEL_CTRL_ID, &clear_str, TRUE);
#else
            GUILABEL_SetText(MMICC_CALLING_STATE_LABEL_CTRL_ID, &clear_str, TRUE);
            GUILABEL_SetText(MMICC_CALLING_NAME_LABEL_CTRL_ID, &clear_str, TRUE);
            GUILABEL_SetText(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, &prompt_str, TRUE);
            GUILABEL_SetText(MMICC_CALLING_LOCAT_LABEL_CTRL_ID, &clear_str, TRUE);
#endif
        }
        else
        {
            //查找pb
            b_search_result = GetNameFromPb(&s_call_context.want_mo_call, tel_num, tel_num_len, &name, &pb_entry_id, &pb_group_id); 
            
            //PB中没有该号码
            if( !b_search_result ||
                ( 0 == name.wstr_len) )
            {
                s_call_context.want_mo_call.is_name_exist = FALSE;
                
                s_call_context.want_mo_call.name_len = tel_num_len;
#ifdef MMI_DUALMODE_ENABLE
                copy_len = tel_num_len;
#else
                copy_len = ( tel_num_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : tel_num_len;
#endif
                MMI_STRNTOWSTR(
                    s_call_context.want_mo_call.name,
                    s_call_context.want_mo_call.name_len,
                    tel_num,
                    tel_num_len,
                    copy_len
                    );
                
                s_call_context.want_mo_call.name_len = copy_len;
            }
            else
            {
                s_call_context.want_mo_call.is_name_exist = TRUE;
                
#ifdef MMI_DUALMODE_ENABLE
                copy_len = name.wstr_len;
#else
                copy_len = ( name.wstr_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : name.wstr_len;
#endif
                
                s_call_context.want_mo_call.name_len = copy_len;
            }
            
            if (s_call_context.want_mo_call.is_name_exist)
            {
                prompt_str.wstr_ptr = (wchar *)s_call_context.want_mo_call.name;
                prompt_str.wstr_len = s_call_context.want_mo_call.name_len;
            }
            else
            {
                prompt_str.wstr_ptr = PNULL;
                prompt_str.wstr_len = 0;
            }
            
#ifndef MMI_GUI_STYLE_TYPICAL
            GUILABEL_SetText(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, &prompt_str, TRUE);
#else
            GUILABEL_SetText(MMICC_CALLING_NAME_LABEL_CTRL_ID, &prompt_str, TRUE);
#endif
            SCI_MEMSET(&prompt_str, 0x00, sizeof(prompt_str));
            
            if (tel_num_len >0)
            {
                prompt_str.wstr_ptr = (wchar *)temp_wchar;
                prompt_str.wstr_len = tel_num_len;
                
                MMI_STRNTOWSTR(prompt_str.wstr_ptr,
                    prompt_str.wstr_len,
                    (uint8 *)tel_num,
                    tel_num_len,
                    tel_num_len);
            }
            else
            {
                //the number wstr_len is 0.display unknown number info
                MMI_GetLabelTextByLang( (MMI_TEXT_ID_T)TXT_UNKNOW_NUM,  &prompt_str);
            }
            
#ifndef MMI_GUI_STYLE_TYPICAL
            if(s_call_context.want_mo_call.is_name_exist)
            {
                GUILABEL_SetText(MMICC_CONNECTED_NUMBER_LABEL_CTRL_ID, &prompt_str, TRUE);
            }
            else
            {
                GUILABEL_SetText(MMICC_CONNECTED_NAME_LABEL_CTRL_ID, &prompt_str, TRUE);
            } 
#else
            GUILABEL_SetText(MMICC_CALLING_NUMBER_LABEL_CTRL_ID, &prompt_str, TRUE);
#endif
        }
    }
    
    if (PNULL != display_str_ptr && display_str_ptr->wstr_len > 0)
    {
        if (SIMAT_SIMC_ADDRESS == data_type)
        {
#ifndef MMI_GUI_STYLE_TYPICAL
            GUILABEL_SetText(MMICC_CONNECTED_STATE_LABEL_CTRL_ID, display_str_ptr, TRUE);
#else
            GUILABEL_SetText(MMICC_CALLING_STATE_LABEL_CTRL_ID, display_str_ptr, TRUE);
#endif
        }
        else if (SIMAT_SIMC_USSD_STRING == data_type && MMK_IsOpenWin(MMIUSSD_SEND_WIN_ID))
        {
            MMIPUB_SetWaitWinTextEx(TRUE, 0, display_str_ptr, MMIUSSD_SEND_WIN_ID);
        }
        else if (SIMAT_SIMC_SS_STRING == data_type && MMK_IsOpenWin(MMISET_SET_CONNECT_NETWORK_WIN_ID))
        {
            MMIPUB_SetWaitWinTextEx(TRUE, 0, display_str_ptr, MMISET_SET_CONNECT_NETWORK_WIN_ID);
        }
    }
    
    if (PNULL != alert_str_ptr && alert_str_ptr->wstr_len > 0)
    {
        MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;

        MMIPUB_CloseAlertWin();
        MMIPUB_OpenAlertWinByTextPtr(PNULL, alert_str_ptr, PNULL, IMAGE_PUBWIN_WARNING, PNULL, &win_priority, MMIPUB_SOFTKEY_ONE, PNULL);  
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN CC_IsCallIDValid(uint8 call_id)
{
    return (call_id < CC_MAX_CALL_NUM);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsCurrentCallIDValid(void)
{
    return (s_call_context.current_call < CC_MAX_CALL_NUM);
}

/*****************************************************************************/
//  Description : Get Calling SIM(Not In call return MMI_DUAL_SYS_MAX)
//  Global resource dependence : none
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_GetCallingSIM(void)
{
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
#ifdef VT_SUPPORT
//bug339497
        if(MMIAPIVT_IsVtCalling())
        {
            SCI_TRACE_LOW("MMIAPICC_GetCallingSIM vt calling retrun MN_DUAL_SYS_1");
            return MN_DUAL_SYS_1;
        }
#endif
        return s_call_context.dual_sys;
    }

    return MMI_DUAL_SYS_MAX;
}

/*****************************************************************************/
// 	Description : 909 problem, call disconnect msg is coming, but the voice  is being
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMICC_HandleDownlinkChanel(void)
{
    if(!MMICC_IsRemoteMute())
    {
        if(MMICC_IsExistHoldCall() && !MMICC_IsExistActiveCall())
        {
            MMISRVAUD_SetMuteNohandle(TRUE);
        }
        else
        {
            MMISRVAUD_SetMuteNohandle(FALSE);
        }
    }
}

#ifdef DPHONE_SUPPORT
/*****************************************************************************/
// 	Description : reset call handfree state
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
LOCAL void ResetCallHandfreeState(void)
{
    AUDIO_DEVICE_MODE_TYPE_E  mode = AUDIO_DEVICE_MODE_HANDFREE;
    if (!MMIDEFAULT_GetHookStates()) //&& !MMIDEFAULT_GetHandFreeStates())
    {
        mode = AUDIO_DEVICE_MODE_HANDFREE;
    }
    else
    {
        mode = AUDIO_DEVICE_MODE_HANDHOLD;
    }

    CC_SetDevModeAndVolume(mode);
}

/*****************************************************************************/
// 	Description : set device mode and volume
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC  void CC_SetDevModeAndVolume(AUDIO_DEVICE_MODE_TYPE_E mode)
{
    uint8 volume = MMIAPISET_GetCallVolume();
    MCU_MODE_E iRet = 0; 

	 // CR 154449 Wikeyli modified for settle calibration passert problem.<< 
    iRet = POWER_GetResetMode(); 
    if (CALIBRATION_POST_MODE == iRet) 
    { 
        //SCI_TRACE_LOW:"<CC_SetDevModeAndVolume>CALIBRATION_POST_MODE == iRet, Return mmi audio action to match the layer1_audio modify!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_9447_112_2_18_2_10_26_242,(uint8*)"");
        return ; 
    } 
    
    {
        volume = (0 == volume)? 1:volume;
        MMISRVAUD_SetVolume(GetAudioHandle(), (uint32)volume);
    }

    return ;
}

#endif

/*****************************************************************************/
//  Description : get current existed  hod call count
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetHoldCallCount(void)
{
    SCI_TRACE_LOW("ccapp MMIAPICC_GetHoldCallCount: exist_hold_call_num is %d", s_call_context.exist_call.exist_hold_call_num);
    return (s_call_context.exist_call.exist_hold_call_num);
}

/*****************************************************************************/
//  Description : get current existed  active call count
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetActiveCallCount(void)
{
    SCI_TRACE_LOW("ccapp MMIAPICC_GetActiveCallCount: exist_active_call_num is %d", s_call_context.exist_call.exist_active_call_num);
    return (s_call_context.exist_call.exist_active_call_num);
}

/*****************************************************************************/
//  Description : get current existed  mpty call count
//  Global resource dependence : none
//  Author: taoxue
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetMPTYCallCount(void)
{
    if (1 < MMIAPICC_GetActiveCallCount())
    {
        return s_call_context.exist_call.exist_active_call_num;
    }
    else if (1 < MMIAPICC_GetHoldCallCount())
    {
        return s_call_context.exist_call.exist_hold_call_num;
    }
    else
    {
        return 0;
    }
}

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)

/*****************************************************************************/
//Description : get the call status by the given call state for BT BQB
//Global resource dependence : 
//Author: arvin.zhang
//Note:暂时对呼出电话不区分alerting和dialing
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetCallStatus(CC_CALL_STATE_E call_state)
{
    uint8 call_status = 0;
    
    switch(call_state)
    {
        case CC_CALL_STATE:
            call_status = 0;
            break;

        case CC_HOLD_STATE:
            call_status = 1;
            break;

        //Dialing and Alerting暂不作区分
        //统一设置为 2
        case CC_CALLING_STATE:
            call_status = 2;
            break;

        case CC_INCOMING_CALL_STATE:
            call_status = 4;
            break;

        case CC_WAITING_STATE:
            call_status = 5;
            break;

        default:
            call_status = 0;
            break;
    }

    SCI_TRACE_LOW("MMIAPICC_GetCallStatus: call_state = %d,call_status = %d",call_state,call_status);

    return call_status;
}

/*****************************************************************************/
// Description : send call info to bt headset
//Global resource dependence : 
//  Author: louis.wei
//Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SendCallInfo2HFG(void)
{
    uint8 call_index = 0;
    uint8 call_dir = 0;
    uint8 call_status = 0;
    uint8 tele_number[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint32 i  =0;

    for(i = 0; i < s_call_context.call_number; i++)
    {
        call_index = i;
        if(CC_MT_CALL_TYPE == s_call_context.call_info[i].call_type)
        {
            call_dir = 1;
        }
        else
        {
            call_dir = 0;
        }
        
        call_status = MMIAPICC_GetCallStatus(s_call_context.call_info[i].call_state);
        
        SCI_MEMSET(tele_number, 0x00, (CC_MAX_TELE_NUM_LEN + 2)*sizeof(uint8));
        
        MMIAPICOM_GenDispNumber(
                s_call_context.call_info[call_index].address.number_type, 
                (uint8)s_call_context.call_info[call_index].address.num_len, 
                s_call_context.call_info[call_index].address.party_num,
                tele_number,
                sizeof(tele_number));
        
        MMIAPIBT_SetCallInfo(call_index,
                call_dir,//MO call
                call_status,//Active call
                CC_CALL_NORMAL,
                0,
                tele_number,
                PNULL);
    }

    MMIAPIBT_SetCallSuccess();

}

/*****************************************************************************/
// pescription : if there is call coming, then info BT handfree set
// Global resource dependence : 
// Author: qing.yu
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_IsCallingAndInfoBt(void)
{
    MN_CALLED_NUMBER_T   cur_call_number = {0};
    uint8	tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};    
    uint8	tel_num_len = 0;
    MMI_STRING_T	prompt_str_array = {0};
	wchar	prompt_tel_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    prompt_str_array.wstr_ptr = prompt_tel_num;
		
    //SCI_TRACE_LOW:"[MMIAPICC_IsCallingAndInfoBt]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_9547_112_2_18_2_10_27_245,(uint8*)"");
    if(MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
    {
        cur_call_number = s_call_context.call_info[s_call_context.current_call].address;
        
        tel_num_len =  MMIAPICOM_GenNetDispNumber(cur_call_number.number_type,
            (uint8)cur_call_number.num_len,
            (uint8*)cur_call_number.party_num,
            tel_num,
            CC_MAX_TELE_NUM_LEN + 2
            );
        
        //电话号码赋值
        //if (MMIAPICOM_IsValidNumberString((char *)tel_num, tel_num_len))
        if (0 != tel_num_len)
        {

            prompt_str_array.wstr_len = tel_num_len;
            MMI_STRNTOWSTR(prompt_str_array.wstr_ptr,
                prompt_str_array.wstr_len,
                (uint8 *)tel_num,
                tel_num_len,
                tel_num_len);
            MMIAPIBT_IncomingCall(prompt_str_array);

        }
        else
        {
            prompt_str_array.wstr_len = 0;
            SCI_TRACE_LOW("MMIAPICC_IsCallingAndInfoBt 111");
            MMIAPIBT_IncomingCall(prompt_str_array);
        }
    }
}
#endif //BT_BQB_SUPPORT

/*****************************************************************************/
// Description : get current call string
//Global resource dependence : 
//  Author: bin.ji
//Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCurrentCallString(
                                          MMI_STRING_T *str_ptr
                                          )
{
    if (PNULL != str_ptr)
    {
        if(MMICC_IsCallIndexInMPTY(s_call_context.current_call))
        {
            MMI_GetLabelTextByLang(TXT_CC_MULTICALL, str_ptr);
        }
        else
        {
            str_ptr->wstr_ptr = s_call_context.call_info[s_call_context.current_call].name;
            str_ptr->wstr_len = s_call_context.call_info[s_call_context.current_call].name_len;
        }
    }
}



/*****************************************************************************/
// 	Description : clear s_call_context.want_mo_call
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
LOCAL void ClearWantCallInfo(void)
{
    SCI_MEMSET(&s_call_context.want_mo_call, 0x00, sizeof(s_call_context.want_mo_call));
    s_call_context.want_mo_call.is_want_mo_call = FALSE;   
}   


/*****************************************************************************/
// 	Description : judge whether allowed to send dtmf when in outgoing or incoming call
//	Global resource dependence : 
//  Author : samboo.shen
//	Note: not for connected state
/*****************************************************************************/
PUBLIC void MMICC_ClearWantCallInfo(void)
{
    ClearWantCallInfo();
}

/*****************************************************************************/
// 	Description : judge whether allowed to send dtmf when in outgoing or incoming call
//	Global resource dependence : 
//  Author : samboo.shen
//	Note: not for connected state
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAllowSendDtmf(void)
{
    BOOLEAN is_allowed = FALSE;
    if(CC_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state)
    {
        is_allowed = TRUE;
    }
    else if((CC_CALLING_STATE == s_call_context.call_info[s_call_context.current_call].call_state)
        ||(CC_INCOMING_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state)
        ||(CC_WAITING_STATE == s_call_context.call_info[s_call_context.current_call].call_state))
    {
        is_allowed = ((DTMF_MN_SYNC_IND_BIT | DTMF_MN_PROGRESS_IND_BIT) == s_call_context.dtmf_state.allow_send_flag);
    }
    else
    {
        is_allowed = FALSE;
    }
        
    return is_allowed;
}

/*****************************************************************************/
// 	Description : judge whether allowed to add new call
//	Global resource dependence : 
//  Author : tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsAllowAddNewCall(void)
{
    BOOLEAN is_allowed = TRUE;

    if ((MMICC_IsExistOutgoingCall()
        ||CC_CALLING_STATE == s_call_context.call_info[s_call_context.current_call].call_state
        ||CC_INCOMING_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state
        ||CC_WAITING_STATE == s_call_context.call_info[s_call_context.current_call].call_state
        ||MMICC_IsExistActiveAndHoldCalls())
        ||MMIAPIDM_GetHandsetLock())
    {
        is_allowed = FALSE;
    }
    return is_allowed;
}

/*****************************************************************************/
// 	Description : judge whether allowed to send dtmf when in outgoing or incoming call
//	Global resource dependence : 
//  Author : samboo.shen
//	Note: not for connected state
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsSendingStoredDtmf(void)
{
    return s_call_context.dtmf_state.is_dtmf;
}


/*****************************************************************************/
// 	Description : get call num
//	Global resource dependence : 
//  Author : samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICC_GetCallNum(void)
{
    return s_call_context.call_number;
}

 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICC_GetCurrentCallIndex(void)
{
    return s_call_context.current_call;
}

 #ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICC_GetLastActiveCallIndex(void)
{
    return s_current_index;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC void MMICC_SetLastActiveCallIndex(void)
{
    s_current_index = CC_INVALID_INDEX;
}
#endif

 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: num_str_ptr must be allocated or be fixed length array, 
//   and the size must be more than MMICC_PHONE_NUM_MAX_LEN + 2
//   num_str_ptr & num_len_ptr [OUT]
/*****************************************************************************/
PUBLIC void MMICC_GetWantCallNumStr(uint8* num_str_ptr, uint8* num_len_ptr)
{
    uint8 max_tele_len = CC_MAX_TELE_NUM_LEN + 2;
    uint8 is_phone = 1;

    if((PNULL == num_str_ptr) || (PNULL == num_len_ptr))
    {
        return;
    }

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif

    if(1 == is_phone)
    {
        *num_len_ptr = MMIAPICOM_GenDispNumber(
            s_call_context.want_mo_call.address.number_type,
            (uint8)s_call_context.want_mo_call.address.num_len,
            s_call_context.want_mo_call.address.party_num,
            num_str_ptr,
            max_tele_len
            );
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT
         ual_bt_get_call_info(num_str_ptr, num_len_ptr);
#endif
    }
    return;
}

  /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: return value means if the call specified str exists
//   spec_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetWantCallSpecifiedStr(MMI_STRING_T* spec_str_ptr)
{
    BOOLEAN result = FALSE;
    if (s_call_context.want_mo_call.is_exist_specified_str && s_call_context.want_mo_call.specified_str_len > 0) //显示特定字符串
    {
        spec_str_ptr->wstr_len = s_call_context.want_mo_call.specified_str_len;
        spec_str_ptr->wstr_ptr = s_call_context.want_mo_call.specified_str;
        result = TRUE;
    }

    return result;
}


 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: return value means if the call name exists
//   name_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetWantCallNameStr(MMI_STRING_T* name_str_ptr)
{
    BOOLEAN is_name_exist = FALSE;

    if(s_call_context.want_mo_call.is_name_exist && s_call_context.want_mo_call.name_len > 0)
        {
        is_name_exist = TRUE;
        if(PNULL != name_str_ptr)
        {
            name_str_ptr->wstr_ptr = s_call_context.want_mo_call.name;
            name_str_ptr->wstr_len = s_call_context.want_mo_call.name_len;
        }
    }
        else
        {
        is_name_exist = FALSE;
        }
    return is_name_exist;
}

 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsWantCallExist(void)
{
    return s_call_context.want_mo_call.is_want_mo_call;
}

  /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMICC_GetWantCallDualSys(void)
{
    return s_call_context.want_mo_call.dual_sys;
}

 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: num_str_ptr must be allocated or be fixed length array, 
//   and the size must be more than MMICC_PHONE_NUM_MAX_LEN + 2
//   num_str_ptr & num_len_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetCallNumStrByIndex(uint8* num_str_ptr, uint8* num_len_ptr, uint8 call_index)
{
    uint8 max_tele_len = CC_MAX_TELE_NUM_LEN + 2;
    BOOLEAN result = FALSE;
    uint8 is_phone = 0;

    if((PNULL == num_str_ptr) || (PNULL == num_len_ptr))
    {
        return FALSE;
    }

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif

    if(1 == is_phone)
    {
        if(TRUE == CC_IsCallIDValid(call_index))
        {
            if( s_call_context.call_info[call_index].address.num_len > 0)
            {

                *num_len_ptr = MMIAPICOM_GenNetDispNumber(
                    s_call_context.call_info[call_index].address.number_type,
                    (uint8)s_call_context.call_info[call_index].address.num_len,
                    s_call_context.call_info[call_index].address.party_num,
                    num_str_ptr,
                    max_tele_len
                    );
                result = TRUE;
            }
        }
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT
        result = ual_bt_get_call_info(num_str_ptr, num_len_ptr);
#endif
    }
#ifndef WIN32
    SCI_TRACE_LOW("%s: result = %d ", __FUNCTION__,result);
#endif

    return result;
}


 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC MMIPB_BCD_NUMBER_T MMICC_GetPbNumByIndex(uint8 call_index)
{
    MMIPB_BCD_NUMBER_T pb_num = {0};
    uint16 copy_len = 0;
    
    pb_num.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(s_call_context.call_info[call_index].address.number_type, s_call_context.call_info[call_index].address.number_plan );
    copy_len = ( s_call_context.call_info[call_index].address.num_len > MMIPB_BCD_NUMBER_MAX_LEN ) ? MMIPB_BCD_NUMBER_MAX_LEN : s_call_context.call_info[call_index].address.num_len;
    pb_num.number_len = copy_len;
    MMI_MEMCPY(
        pb_num.number,
        sizeof(pb_num.number),
        s_call_context.call_info[call_index].address.party_num,
        sizeof(s_call_context.call_info[call_index].address.party_num),
        copy_len
        );         

    return pb_num;
}


 /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: return value means if the call name exists
//   name_str_ptr [OUT]
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetCallNameStrByIndex(MMI_STRING_T* name_str_ptr, uint8 call_index)
{
    BOOLEAN is_name_exist = FALSE;
    if(!CC_IsCallIDValid(call_index))
    {
        return FALSE;
    }

    if(s_call_context.call_info[call_index].is_name_exist && s_call_context.call_info[call_index].name_len > 0)
    {
        is_name_exist = TRUE;
        if(PNULL != name_str_ptr)
        {

            name_str_ptr->wstr_ptr = s_call_context.call_info[call_index].name;
            name_str_ptr->wstr_len = s_call_context.call_info[call_index].name_len;
    }
    }
    else
    {
        is_name_exist = FALSE;
    }
    return is_name_exist;
}
 
 /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_CALL_STATE_E	MMICC_GetCallStatusByIndex(uint8 call_index)
{
    if(CC_MAX_CALL_NUM <= call_index)
    {
        return CC_NULL_STATE;
    }
    else
    {
        return	(s_call_context.call_info[call_index].call_state);
    }
}

 /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//	Note:
/*****************************************************************************/
PUBLIC CC_CALL_STATE_E	MMICC_GetCurrentCallStatus(void)
{
    if(CC_MAX_CALL_NUM <= s_call_context.current_call)
    {
        return CC_NULL_STATE;
    }
    else
    {
        return	(s_call_context.call_info[s_call_context.current_call].call_state);
    }
}

 /*****************************************************************************/
//  Description : get call operation
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_OPERATED_BY_USER_E	MMICC_GetCallOperatedByIndex(uint8 call_index)
{
    if(CC_IsCallIDValid(call_index))
    {
        return s_call_context.call_info[call_index].operated_by_user;
    }
    else
    {
        return CC_NO_OPER;
    }
}

  /*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMICC_IsCallIndexInMPTY(uint8 call_index)
{
    if(CC_IsCallIDValid(call_index))
    {
        return s_call_context.call_info[call_index].is_in_mpty;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Select SIM API of CC app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
/*
PUBLIC MMI_RESULT_E MMICC_SelectSIMFunc(PROCESSMSG_FUNC	MMICCSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA		add_data
                                        )
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T	selectSimWinID = MMICC_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)append_type;
    caller_data.callback_func =MMICCSelectSIMWinCallback ;
    caller_data.is_Support_WIFI = FALSE;

    MMIAPIPHONE_OpenSelectSimWin( &selectSimWinID, 
                                MMICC_COM_SELECT_SIM_CTRL_ID, 
                                MMICCSelectSIMWinCallback,
                                &caller_data, 
                                add_data);

#endif
    return MMI_RESULT_TRUE;
}
*/


    /*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8	MMICC_GetCallDirection(uint8 call_index)
{
    if(CC_IsCallIDValid(call_index))
    {
        return s_call_context.call_info[call_index].call_type;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC CC_DTMF_STATE_T* MMICC_GetStoredDtmfInfo(void)
{
    return &s_call_context.dtmf_state;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint32	MMICC_GetCallTimeCount(void)
{
    return s_call_context.call_time_count;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_SetCallTimeCount(uint32 count)
{
     s_call_context.call_time_count = count;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMICC_GetIsNewMissedCall(void)
{
    return s_is_new_missed_call;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_SetIsNewMissedCall(BOOLEAN is_new_missed_call)
{
     s_is_new_missed_call = is_new_missed_call;
}

#ifdef MMI_CRSS_SUPPORT
/*****************************************************************************/
// 	Description : 是否允许ECT转接
//	Global resource dependence : 
//  Author:
//	Note:
//  Parameter: TRUE: 允许进行ECT转接； FALSE：不允许进行ECT转接
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_ECTIsPermit(void)
{
    BOOLEAN ret = FALSE;
    uint8 call_index = 0;
    CC_CALL_STATE_E all_status[CC_CALL_STATE_MAX] = {0};

    for (call_index = 0; call_index < s_call_context.call_number; call_index++)
    {
        if (s_call_context.call_info[call_index].is_in_mpty)
        {
            return FALSE;
        }

        if (s_call_context.call_info[call_index].call_state < CC_CALL_STATE_MAX)
        {
            all_status[s_call_context.call_info[call_index].call_state]++;
        }
    }

    if (all_status[CC_INCOMING_CALL_STATE] > 0)
    {
        //只要存在incoming call不能进行ECT转接
        return FALSE;
    }
    
    if (1 == all_status[CC_CALL_STATE])//one active call
    {
        if (1 == all_status[CC_HOLD_STATE])//one holde call
        {
            if (0 == all_status[CC_CALLING_STATE] && all_status[CC_WAITING_STATE] <= 1)
            {
                //CASE 1: one active call and one hold call : all_status[CC_WAITING_STATE] == 0
                //CASE 3: one active call, one held call and one waiting call:all_status[CC_WAITING_STATE] ==1
                ret = TRUE;
            }
        }
        else if (0 == all_status[CC_HOLD_STATE])
        {
            //CASE 4: one active call and one waiting call
            if (0 == all_status[CC_CALLING_STATE] && 1 == all_status[CC_WAITING_STATE])
            {
                ret = TRUE;
            }
        }
    }

    if (!ret)
    {
        //不满足case 1和case 3, case 4
        
        if (1 == all_status[CC_HOLD_STATE] && (1 == all_status[CC_CALLING_STATE]))
        {
            ////CASE 2: one hold call and one alerting call
            if (0 == all_status[CC_CALL_STATE] && 0 == all_status[CC_WAITING_STATE])
            {
                //active call and waiting call not exist
                ret = TRUE;
            }
        }
    }

    //SCI_TRACE_LOW:"ccapp.c: MMICC_ECTIsPermit, ret = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10098_112_2_18_2_10_28_246,(uint8*)"d",ret);

    return ret;
}

/*****************************************************************************/
// 	Description : get active call index
//	Global resource dependence : s_call_context
//  Author:
//	Note: get a active call or held call index
/*****************************************************************************/
LOCAL uint8 CC_GetActiveCallIndex(void)
{
    uint8   call_index = 0;
    for(call_index = 0; call_index < s_call_context.call_number; call_index++)
    {
        if (s_call_context.call_info[call_index].call_state == CC_CALL_STATE 
           || s_call_context.call_info[call_index].call_state == CC_HOLD_STATE
          )
        {
            break;
        }
    }
    return call_index;
}

/*****************************************************************************/
// 	Description : process ect transfer
//	Global resource dependence : s_call_context
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ECTTransfer(void)
{
    uint8   active_index = CC_INVALID_INDEX;
    //MMI_STRING_T status_str = {0};
    //SCI_TRACE_LOW:"ccapp.c:enter function MMICC_ECTTransfer()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10130_112_2_18_2_10_28_247,(uint8*)"");
    
    //whether the call module has not call
    if (CC_NO_NUM != s_call_context.call_number)
    {
        //current call ect
        active_index = CC_GetActiveCallIndex();//s_call_context.current_call; //FindStateCall( CC_CALL_STATE );

        if (active_index >= s_call_context.call_number)
        {
            //SCI_TRACE_LOW:"ccapp MMICC_ECTTransfer: invalid active idx: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10140_112_2_18_2_10_28_248,(uint8*)"d", active_index);

            return CC_RESULT_ERROR;
        }
        
        //the operation is to shuttle the active call and the hold call
        if (MN_RETURN_SUCCESS != MNCALL_ECTInvokeEx(
            s_call_context.dual_sys,       
            s_call_context.call_info[active_index].call_id))    
        {
            //SCI_TRACE_MID:"CC_HoldCall: MNCALL_ECTInvokeEx is fault"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10150_112_2_18_2_10_28_249,(uint8*)"");

            return CC_RESULT_ERROR;
        }

        //update the cc window
        //status_str
        //MMI_GetLabelTextByLang(TXT_CC_ECT_PROCESS, &status_str);
        //MMICC_UpdateCurrentCallProcessing(&status_str);    //call processing
        MMICC_UpdateCurrentCallProcessing();    //call processing
    }
    else
    {
        //use the msgbox to prompt the operation is fail
        MMIPUB_OpenAlertWarningWin(TXT_CC_OPER_FAIL);
        
        return CC_RESULT_ERROR;
    }
    
    return CC_RESULT_SUCCESS;
}
#endif

/*****************************************************************************/
// 	Description : ECT Transfer Indication: active, Rdn or alering
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleECTInd(DPARAM param)
{
    APP_MN_ECT_INVOKE_IND_T *ect_ind_ptr = (APP_MN_ECT_INVOKE_IND_T *)param;
    uint8 tele_num[CC_MAX_TELE_NUM_LEN + 2]= {0};
    wchar name_wstr[CC_MAX_PBNAME_LEN + 1] = {0};
    MMIPB_BCD_NUMBER_T pb_num = {0};
    MMI_STRING_T name =  {0};
    MMI_STRING_T tranfer_str =  {0};
    uint16 tel_num_len = 0;
    uint8 copy_len = 0;
    MMI_RESULT_E is_exist_in_pb = FALSE;
    
    //SCI_TRACE_LOW:"ccapp.c HandleECTInd"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10188_112_2_18_2_10_28_250,(uint8*)"");

    if (ect_ind_ptr != PNULL)
    {
       if (ect_ind_ptr->party_num_present)
       {
           //get number  
           tel_num_len =  MMIAPICOM_GenNetDispNumber(ect_ind_ptr->party_num.number_type,
               (uint8)ect_ind_ptr->party_num.num_len,
               (uint8*)ect_ind_ptr->party_num.party_num,
               tele_num ,
               (CC_MAX_TELE_NUM_LEN + 2));
            //获得号码
            pb_num.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(ect_ind_ptr->party_num.number_type, ect_ind_ptr->party_num.number_plan );
            
            copy_len = (ect_ind_ptr->party_num.num_len > MMIPB_BCD_NUMBER_MAX_LEN ) ? MMIPB_BCD_NUMBER_MAX_LEN : ect_ind_ptr->party_num.num_len;
            pb_num.number_len = copy_len;

            MMI_MEMCPY(
                pb_num.number,
                sizeof(pb_num.number),
                ect_ind_ptr->party_num.party_num,
                sizeof(ect_ind_ptr->party_num.party_num),
                copy_len
                );

            name.wstr_ptr = name_wstr;
            is_exist_in_pb = MMIAPIPB_GetNameByNumber(&pb_num, &name, CC_MAX_PBNAME_LEN, FALSE) ;/*lint !e506 */
            if ( !is_exist_in_pb ||
               ( 0 == name.wstr_len ))   //电话本中不存在或者名字为空,显示电话号码
            {
                tel_num_len = MIN(CC_MAX_PBNAME_LEN, tel_num_len);
                name.wstr_len = tel_num_len;
                MMI_STRNTOWSTR(
                    (void*)name.wstr_ptr,
                    tel_num_len,
                    (void*)tele_num,
                    tel_num_len,
                    tel_num_len
                    );                 
            }

            //replace phonenumber by call id
            ReplaceCallNumberByCallId(ect_ind_ptr, &name, is_exist_in_pb);  
       }

       if (ect_ind_ptr->ect_call_state == MN_ECT_ALLERTING)
       {
           //alerting
           MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_ECT_TRANSFERING,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,MMICC_HandleECTAlertWinMsg);            
       }
       else
       {
            MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
            //active
            MMI_GetLabelTextByLang(TXT_CC_ECT_TRANSFER_TO, &tranfer_str);
            MMIPUB_OpenAlertWinByTextPtr(PNULL,&tranfer_str ,&name,IMAGE_PUBWIN_SUCCESS,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,MMICC_HandleECTAlertWinMsg);            
       }
       //SCI_TRACE_LOW:"ccapp.c party_num_present %d call id %d num_len %d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10245_112_2_18_2_10_28_251,(uint8*)"ddd", ect_ind_ptr->party_num_present , ect_ind_ptr->call_id, ect_ind_ptr->party_num.num_len);
    }
}

/*****************************************************************************/
// 	Description : ECT Transfer confirm
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleECTCnf( DPARAM param)
{
    MMI_WIN_PRIORITY_E win_priority = WIN_THREE_LEVEL;
    APP_MN_ECT_INVOKE_CNF_T *ect_cnf_ptr = (APP_MN_ECT_INVOKE_CNF_T *)param;
    
    //SCI_TRACE_LOW:"ccapp.c HandleECTCnf"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10257_112_2_18_2_10_28_252,(uint8*)"");

    if (ect_cnf_ptr != PNULL && ect_cnf_ptr->req_is_accepted)
    {
        //success
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_ECT_CONFIRM,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,MMICC_HandleECTAlertWinMsg);
    }
    else
    {
        //reject
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CC_ECT_CONFIRM_REJECT,TXT_NULL,IMAGE_PUBWIN_FAIL,PNULL,&win_priority,MMIPUB_SOFTKEY_ONE,MMICC_HandleECTAlertWinMsg);
    }
}

/*****************************************************************************/
// 	Description : ECT Transfer Indication: active, Rdn or alering
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void ReplaceCallNumberByCallId(APP_MN_ECT_INVOKE_IND_T *call_info_ptr, MMI_STRING_T *name_ptr, BOOLEAN is_exist_in_pb)
{
    uint8 call_index = 0;
    //BOOLEAN is_find = FALSE;
    
    if (PNULL == call_info_ptr)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"ccapp.c ReplaceCallNumberByCallId call_id %d dual_sys %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10285_112_2_18_2_10_28_253,(uint8*)"dd", call_info_ptr->call_id, call_info_ptr->dual_sys);
    
    for(call_index = 0; call_index < s_call_context.call_number; call_index++)
    {
        if (s_call_context.call_info[call_index].call_id ==  call_info_ptr->call_id && s_call_context.call_info[call_index].dual_sys == call_info_ptr->dual_sys)
        {
            //find this call id
            //SCI_TRACE_LOW:"ccapp.c ReplaceCallNumberByCallId find it "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10292_112_2_18_2_10_28_254,(uint8*)"");
            break;
        }
    }
    
    if (call_index < CC_MAX_CALL_NUM)
    {
        if (call_index >=  s_call_context.call_number)
        {
            //no find a call id in exist calls
            s_call_context.call_number++;
            s_call_context.incoming_call_index = call_index;
            s_call_context.exist_call.exist_incoming_call = TRUE;
            s_call_context.call_info[call_index].call_id = call_info_ptr->call_id;
            s_call_context.call_info[call_index].dual_sys = call_info_ptr->dual_sys;
        }

        s_call_context.call_info[call_index].address.number_type = call_info_ptr->party_num.number_type;
        s_call_context.call_info[call_index].address.number_plan = call_info_ptr->party_num.number_plan;
        s_call_context.call_info[call_index].address.num_len = call_info_ptr->party_num.num_len;
        
        MMI_MEMCPY(
            s_call_context.call_info[call_index].address.party_num,
            sizeof(s_call_context.call_info[call_index].address.party_num),
            call_info_ptr->party_num.party_num,
            sizeof(call_info_ptr->party_num.party_num),
            sizeof(s_call_context.call_info[call_index].address.party_num)
            );
        
        SCI_MEMSET(s_call_context.call_info[call_index].name, 0x00, sizeof(s_call_context.call_info[call_index].name));
        s_call_context.call_info[call_index].name_len = 0;
        
        if (name_ptr != PNULL && name_ptr->wstr_ptr != PNULL && name_ptr->wstr_len > 0)
        {
            s_call_context.call_info[call_index].is_name_exist = TRUE;
            
            s_call_context.call_info[call_index].name_len = MIN(CC_MAX_PBNAME_LEN, name_ptr->wstr_len);
            
            MMIAPICOM_Wstrncpy(s_call_context.call_info[call_index].name, name_ptr->wstr_ptr, s_call_context.call_info[call_index].name_len);
        }
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : s_call_context
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsCurrentCallMute(void)
{
    BOOLEAN is_mute = FALSE;
    uint8 is_phone = 1;
    BOOLEAN ret = FALSE;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif
    if( 1 == is_phone )
    {
        if (CC_NO_NUM == s_call_context.call_number
            || s_call_context.call_number > CC_MAX_CALL_NUM
            || s_call_context.current_call >= s_call_context.call_number)
        {
            //SCI_TRACE_LOW:"ccapp IsCurrentCallMute: %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10360_112_2_18_2_10_29_255,(uint8*)"d", is_mute);
            return is_mute;
        }

        //if current call is in mpty
        if(s_call_context.call_info[s_call_context.current_call].is_in_mpty)
        {
            //any call in mpty is mute,then the mpty is mute
            int8 i = 0;
            for(; i < s_call_context.call_number; i++)
            {
                if(s_call_context.call_info[i].is_in_mpty && s_call_context.call_info[i].is_mute)
                {
                    is_mute = TRUE;
                    break;
                }
            }
        }
        else
        {
            is_mute = s_call_context.call_info[s_call_context.current_call].is_mute;
        }
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT
        is_mute = ual_bt_hfu_get_is_mute();
#endif
    }
    //SCI_TRACE_LOW:"ccapp IsCurrentCallMute: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10360_112_2_18_2_10_29_255,(uint8*)"d", is_mute);
    return is_mute;
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : s_call_context
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_SetCurrentCallMute(BOOLEAN is_mute)
{
    BOOLEAN result = FALSE;
    uint8 is_phone = 1;
    BOOLEAN ret = FALSE;

    is_phone = ual_bt_get_product_role();

#ifndef WIN32
    SCI_TRACE_LOW("%s: current product mode = %d ", __FUNCTION__,is_phone);
#endif
    //SCI_TRACE_LOW:"ccapp SetCurrentCallMute: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_10371_112_2_18_2_10_29_256,(uint8*)"d", is_mute);
    if( 1 == is_phone )
    {
        result = MMICC_EnableMute(is_mute);
        if (result)
        {
            //if current call is in mpty
            if(s_call_context.call_info[s_call_context.current_call].is_in_mpty)
            {
                //set every call mute
                int8 i = 0;
                for(; i < s_call_context.call_number; i++)
                {
                    if(s_call_context.call_info[i].is_in_mpty)
                    {
                        s_call_context.call_info[i].is_mute = is_mute;
                    }
                }
            }
            else
            {
                s_call_context.call_info[s_call_context.current_call].is_mute = is_mute;
            }    
        }
    }
    else
    {
#ifdef UAL_BT_HFU_SUPPORT
        result = ual_bt_hfu_set_mute(UAL_BT_HFU_AUD_LOCAL, is_mute);
#endif
    }
#ifndef WIN32
    SCI_TRACE_LOW("%s: set mute result = %d ", __FUNCTION__,result);
#endif
    return result;
}

#ifdef MMI_CSP_SUPPORT
  /*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMICC_GetWantCallDualLine(void)
{
    return s_call_context.want_mo_call.line;
}

 /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8	MMICC_GetCallLineByIndex(uint8 call_index)
{
    if(CC_MAX_CALL_NUM <= call_index)
    {
        return 0;
    }
    else
    {
        return	(s_call_context.call_info[call_index].line);
    }
}

 /*****************************************************************************/
//  Description : get current call status
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8	MMICC_GetCurrentCallLine(uint8 call_index)
{
    return MMICC_GetCallLineByIndex(s_call_context.current_call);
}

#endif

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
// 	Description : Parse CSC SOS Number
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseCSCSosNum(uint8 *sos_number_ptr, MMICC_CSC_SOS_NUM_T *cc_csc_sos_ptr)
{
    MMICC_CSC_SOS_NUM_INFO_T *temp_csc_sos_ptr = PNULL;
    MMICC_CSC_SOS_NUM_INFO_T *cur_csc_sos_ptr = PNULL;

    if (PNULL == sos_number_ptr || PNULL == cc_csc_sos_ptr)
    {
        return FALSE;
    }

    if (PNULL != cc_csc_sos_ptr->sos_num_info_ptr)
    {
        cur_csc_sos_ptr = cc_csc_sos_ptr->sos_num_info_ptr;

        while (PNULL != cur_csc_sos_ptr)
        {
            temp_csc_sos_ptr = cur_csc_sos_ptr;
        
            if (PNULL != cur_csc_sos_ptr->sos_num_ptr)
            {
                SCI_FREE(cur_csc_sos_ptr->sos_num_ptr);
                cur_csc_sos_ptr->sos_num_ptr = PNULL;
            }

            cur_csc_sos_ptr = cur_csc_sos_ptr->next;

            SCI_FREE(temp_csc_sos_ptr);
            temp_csc_sos_ptr = PNULL;
        }
    }

    SCI_MEMSET(cc_csc_sos_ptr, 0x00, sizeof(MMICC_CSC_SOS_NUM_T));

    {
        wchar *p = PNULL;
        int32 pos = 0;
        int32 cnt = 0;
        int32 pn_confirm_len = 0;
        int32 pn_len = 0;
        BOOLEAN is_pn = FALSE;
        wchar *str_ptr = PNULL;

        str_ptr = SCI_ALLOCAZ((XML_TAG_MESSAGE_MAX_LEN+ 1)*sizeof(wchar));

        if (PNULL == str_ptr)
        {
            return FALSE;
        }

        MMI_STRNTOWSTR(str_ptr, XML_TAG_MESSAGE_MAX_LEN, sos_number_ptr, XML_TAG_MESSAGE_MAX_LEN, XML_TAG_MESSAGE_MAX_LEN);
        
        while (*(str_ptr + pos))
        {
            p = str_ptr + pos;

            do
            {
                if (pn_confirm_len <= pos)
                {
                    is_pn = (URL_IsPhoneNumber(p, &pn_len));

                    if (!is_pn && pn_len > 0)
                    {
                        uint8 *temp_p = PNULL;

                        temp_p = SCI_ALLOCAZ(pn_len + 1);

                        if (PNULL == temp_p)
                        {
                            return FALSE;
                        }

                        MMI_WSTRNTOSTR(temp_p, pn_len, p, pn_len, pn_len);

                        if (MMIAPICOM_IsPhoneNumber(temp_p, pn_len))
                        {
                            is_pn = TRUE;
                        }

                        SCI_FREE(temp_p);
                    }

                    pn_confirm_len = pos + pn_len;

                    if (is_pn)
                    {
                        cur_csc_sos_ptr = SCI_ALLOCAZ(sizeof(MMICC_CSC_SOS_NUM_INFO_T));

                        if (PNULL == cur_csc_sos_ptr)
                        {
                            return FALSE;
                        }

                        cur_csc_sos_ptr->sos_num_ptr = SCI_ALLOCAZ(pn_len + 1);

                        if (PNULL == cur_csc_sos_ptr->sos_num_ptr)
                        {
                            SCI_FREE(cur_csc_sos_ptr);

                            return FALSE;
                        }

                        MMI_WSTRNTOSTR(cur_csc_sos_ptr->sos_num_ptr, pn_len, p, pn_len, pn_len);

                        temp_csc_sos_ptr = cc_csc_sos_ptr->sos_num_info_ptr;

                        if (PNULL == temp_csc_sos_ptr)
                        {
                            cc_csc_sos_ptr->sos_num_info_ptr = cur_csc_sos_ptr;
                        }
                        else
                        {
                            while (PNULL != temp_csc_sos_ptr->next)
                            {
                                temp_csc_sos_ptr = temp_csc_sos_ptr->next;
                            }

                            temp_csc_sos_ptr->next = cur_csc_sos_ptr;
                        }

                        cnt++;
                        pos += pn_len;

                        cc_csc_sos_ptr->sos_num++;

                        break;
                    }
                }

                pos++;

            }
            while (0);
        }

        SCI_FREE(str_ptr);
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description : Is CSC SOS Number
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCSCSosNum(uint8 *tele_num_ptr, uint16 tele_len, MMICC_CSC_SOS_NUM_T *cc_csc_sos_ptr)
{
    MMICC_CSC_SOS_NUM_INFO_T *cur_csc_sos_ptr = PNULL;

    if (PNULL == tele_num_ptr || PNULL == cc_csc_sos_ptr || 0 == tele_len)
    {
        return FALSE;
    }

    if (0 == cc_csc_sos_ptr->sos_num)
    {
        return FALSE;
    }

    cur_csc_sos_ptr = cc_csc_sos_ptr->sos_num_info_ptr;

    while (PNULL != cur_csc_sos_ptr)
    {
        if (PNULL != cur_csc_sos_ptr->sos_num_ptr
            && (tele_len == strlen((char *)cur_csc_sos_ptr->sos_num_ptr))
            && (0 == strncmp((char*)tele_num_ptr, (char *)cur_csc_sos_ptr->sos_num_ptr, tele_len)))
        {
            return TRUE;
        }

        cur_csc_sos_ptr = cur_csc_sos_ptr->next;
    }

    return FALSE;
}

/*****************************************************************************/
// 	Description : Set the SOS Number from csc
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_SetSOSByCSC(MMICUS_CSC_SETTING_MAIN_SOS_NUM_T *csc_sos_ptr)
{
    if (PNULL == csc_sos_ptr)
    {
        return FALSE;
    }

    //Parse SOS
    ParseCSCSosNum(csc_sos_ptr->sos_number, &s_cc_csc_sos);

    ParseCSCSosNum(csc_sos_ptr->sos_number_nosim, &s_cc_csc_no_sim_sos);

    return TRUE;
}
#endif

/*****************************************************************************/
// 	Description : Handle Net work Emg Ind(APP_MN_EMG_NUM_LIST_IND)
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
LOCAL void HandleEmgNumListInd(DPARAM param)
{
    APP_MN_EMG_NUM_LIST_IND_T *emg_num_list_ptr = (APP_MN_EMG_NUM_LIST_IND_T *)((uint32)param - sizeof(xSignalHeaderRec));
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    if (PNULL != emg_num_list_ptr)
    {
        SCI_TRACE_LOW("HandleEmgNumListInd dual_sys=%d, mcc=%d, list_num=%d", emg_num_list_ptr->dual_sys, emg_num_list_ptr->mcc, emg_num_list_ptr->list_num);

        dual_sys = emg_num_list_ptr->dual_sys;

        if (dual_sys < MMI_DUAL_SYS_MAX)
        {
            if (PNULL != s_cc_network_emg_num_list_ptr[dual_sys])
            {
                SCI_MEMSET(s_cc_network_emg_num_list_ptr[dual_sys], 0x00, sizeof(APP_MN_EMG_NUM_LIST_IND_T));
            }
            else
            {
                s_cc_network_emg_num_list_ptr[dual_sys] = SCI_ALLOCAZ(sizeof(APP_MN_EMG_NUM_LIST_IND_T));
            }

            if (PNULL == s_cc_network_emg_num_list_ptr[dual_sys])
            {
                return;
            }

            SCI_MEMCPY(s_cc_network_emg_num_list_ptr[dual_sys], emg_num_list_ptr, sizeof(APP_MN_EMG_NUM_LIST_IND_T));
        }
    }
}

/*****************************************************************************/
// 	Description : is existing unciphering call 
//	Global resource dependence : 
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsExistUncipheringCall(void)
{
    uint8 call_id = 0;
    BOOLEAN is_exist_unciphering_call = FALSE;

    for (call_id = 0; call_id < s_call_context.call_number; call_id++)
    {
        if (s_call_context.call_info[call_id].is_unciphering_call)
        {
            is_exist_unciphering_call = TRUE;
        }
    }

    return is_exist_unciphering_call;
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//  Description : Is SIM Service Available
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsSimServiceAvail(MN_DUAL_SYS_E dual_sys)
{
    MN_DUAL_SYS_E calling_sys = MMIAPICC_GetCallingSIM();

    if (MMI_DUAL_SYS_MAX != calling_sys
        && calling_sys != dual_sys)
    {
        return FALSE;
    }

    return TRUE;
}
#endif
#ifndef SIM_PLUG_IN_SUPPORT //bug 184713
/*****************************************************************************/
//  Description : if sim removed,exit mo call
//  Global resource dependence : none
//  Author:chunjie liu
//  Note:cucc support conctrol,befor mo call request,make a request to sim
/*****************************************************************************/
PUBLIC void MMIAPICC_ExitMoCall(MN_DUAL_SYS_E dual_sys)
{
    MN_DUAL_SYS_E calling_sys = MMIAPICC_GetCallingSIM();

   if(MMI_DUAL_SYS_MAX != calling_sys
        && calling_sys == dual_sys)
   {
        if(MMK_IsOpenWin( MMICC_STATUS_WIN_ID ))
        {
            MMK_CloseWin(MMICC_STATUS_WIN_ID);
        }
        s_dial_method = CC_DIAL_METHOD_NULL;
        s_call_context.exist_call.exist_outgoing_call = FALSE;
        CC_ClearStoredDtmf();
        //clear s_call_context.want_mo_call
        SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call));
        s_call_context.want_mo_call.is_want_mo_call = FALSE;
        if(PNULL != MMICC_GetAppletInstance() )
        {
            if(MMICC_GetAppletInstance()->cc_instance.is_original_phone_lock)
            {
                MMICC_GetAppletInstance()->cc_instance.is_original_phone_lock = FALSE;     
            }
               
            //stop cc applet
            MMICC_DestroyAudioHandle();
            MMK_PostMsg( MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
        }

        //状态栏显示通话状态图标 
        MAIN_SetCallingState(FALSE);
   }
}
#endif

#if defined(CALL_REC_FILE_SEPERATE)
PUBLIC MN_DUAL_SYS_E MMICC_GetCurrCallDualsys(void)
{
    return s_call_context.call_info[s_call_context.current_call].dual_sys;
}
#endif

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : handle send sco request
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMICC_HandleScoTimer(uint8 timer_id, uint32 param)
{
    SCI_TRACE_LOW("mmicc_app: MMICC_HandleScoTimer s_bt_sco_link_is_exist = %d", s_bt_sco_link_is_exist);

    if ((!s_bt_sco_link_is_exist))
    {
        MMIAPIBT_HfuSwitchAudio(TRUE);
        SCI_TRACE_LOW("mmicc_app: MMICC_HandleScoTimer MMIAPIBT_HfuSwitchAudio !!!");	
    }
}
#endif

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : the process message function of the BT CC application
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL MMI_RESULT_E MMICC_HandleBtPsMsg( PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;   
    BT_MSG_T     *msg_ptr = &((BT_SIGNAL*)param)->content;
	
    //check the param
    SCI_TRACE_LOW("ccapp.c:enter function MMICC_HandleBtPsMsg(), msg_id = %d,start_msg_id = %d",msg_id,APP_BT_HFU_DIAL_NUMBER_RESULT_IND);
    
    if(PNULL== app_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: MMICC_HandleBtPsMsg() app_ptr == PNULL");
        return MMI_RESULT_FALSE;
    }
    
    switch( msg_id )
    {
    case APP_BT_HFU_CIND_IND:
        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_CIND_IND");
        CC_BtCallCindInd(msg_ptr);
        break;
    case APP_BT_HFU_CIEV_IND:
        //收到call setup status message
        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_CIEV_IND");
        CC_BtCallCievInd(msg_ptr);
        break;
    case APP_BT_HFU_RING_IND:

        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_RING_IND");
        if(MMICC_GetBtCallFlag())  //cr226039
        {
            MMICC_EnableVoiceCodec(FALSE);
        }
        break;
    case APP_BT_HFU_CLIP_IND:

        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_CLIP_IND");
        CC_BtCallClipInd(msg_ptr);
        break;  
    case APP_BT_HFU_ATA_RESULT_IND:

        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_ATA_RESULT_IND");
        if(PNULL != msg_ptr && BT_SUCCESS == msg_ptr->status)
        {
            //stop ring
            MMIAPISET_StopRing(MMISET_RING_TYPE_CALL);
            MMIAPISET_StopRing(MMISET_RING_TYPE_CALL_IN_CALL);
            
            //修改CALL LOG
            //更新显示
            //s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_CONNECT_OPER;
        }
        break;
    
    case APP_BT_HFU_HANGUP_RESULT_IND:

        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_HANGUP_RESULT_IND");
        if(PNULL != msg_ptr && BT_SUCCESS != msg_ptr->status)
        {
            SCI_TRACE_LOW("MMICC_HandleBtPsMsg param->status= %d ", msg_ptr->status);
            s_call_context.exist_call.exist_outgoing_call = FALSE;
	   		CC_BtDisconnectedCall();	
            MMICC_SetBtCallFlag(FALSE);
            ClearWantCallInfo();
            MMK_CloseWin(MMICC_STATUS_WIN_ID);
            MMIAPIIDLE_CloseDialMenuWin();
            MMIAPIIDLE_CloseDialWin();
        }
        if(MMICC_GetBtCallFlag())  // operated_by_user related to time refresh
        {
            //check the param
            if(PNULL != msg_ptr && BT_SUCCESS == msg_ptr->status)
            {
                if(CC_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state)
                {
                    s_call_context.call_info[s_call_context.current_call].operated_by_user = CC_RELEASE_OPER;  
                }
            }
        }
        
        break;
    case APP_BT_HFU_CALLHOLD_RESULT_IND:
	{
        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_CALLHOLD_RESULT_IND");
        CC_BtHfuHoldCfnInd(msg_ptr);		  
    }
    break;
    case APP_BT_HFU_AUDIO_CONNECTED:
    {
        BT_STATUS  bt_status = BT_ERROR;
        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_AUDIO_CONNECTED");
        s_bt_audconnind_flag = TRUE;

        SCI_TRACE_LOW("ccapp.c: APP_BT_HFU_AUDIO_CONNECTED s_sco_request_timer = %d", s_sco_request_timer);

        {
            s_bt_sco_link_is_exist = TRUE;
        }
        if(PNULL != msg_ptr && BT_SUCCESS == msg_ptr->status && MMICC_GetBtCallFlag())
        {
            s_bt_audconnind_flag = FALSE;
            if(PNULL == MMICC_GetAppletInstance())
            {
                MMICC_StartCCApplet();
            }

            MMICC_ConstructAudioHandle();
            CC_OpenAudioTunnel(TRUE);
            //cr230070
            MMICC_UpdateStatusWin();

            bt_status = MMIAPIBT_HfuListCurrentCalls();//CLCC+APP_BT_HFU_CLCC_RECORD_IND//bug960623

            if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
            {
                SCI_TRACE_LOW("ccapp.c: APP_BT_HFU_AUDIO_CONNECTED  MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
            }
        }
    }
        break;
	case APP_BT_HFU_DIAL_NUMBER_RESULT_IND: 
        
        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_DIAL_NUMBER_RESULT_IND");
        if(PNULL != msg_ptr && BT_SUCCESS == msg_ptr->status)
        {
                s_call_context.exist_call.exist_outgoing_call = TRUE;
                MMICC_SetBtCallFlag(TRUE);
#ifdef FM_SUPPORT
                MMIAPIFM_StopRecord();
                MMIAPIFM_StopAutoSearch();
#endif
            	
                MMIAPIFMM_ExitFileOption();
                
                MMIAPIDC_Interrupt();
#ifdef MMI_RECORD_SUPPORT
                MMIAPIRECORD_StopRecordOrPlay(); //打出电话的时候停止录音
#endif    
                MMIAPIPICVIEW_StopDeleteFiles();
                //not necessary here
#if defined(VIDEO_PLAYER_SUPPORT) && !defined(MMI_VIDEOPLAYER_MINI_FUNCTION)
                MMIAPIVP_NotifyVPStop();
#endif

#ifdef MOBILE_VIDEO_SUPPORT
                if(MMIAPIMV_IsLiving())
                {
                    MMIAPIMV_Exit();
                }
#endif           

                MMICC_EnableRemoteMute(FALSE);
                MMICC_UpdateCallStatusDisplay(MMICC_MO_START_IND);
        }
        else if(PNULL != msg_ptr && BT_SUCCESS != msg_ptr->status)
        {
            SCI_TRACE_LOW("ccapp.c: MMICC_HandleBtPsMsg APP_BT_HFU_DIAL_NUMBER_RESULT_IND param->status=%d ", msg_ptr->status);
            ClearWantCallInfo();
            MMICC_SetBtCallFlag(FALSE);
        }
        break;
	case APP_BT_HFU_AUDIO_DISCONNECTED:
        //cr132401 没有处于通话中，保持中和呼出中的电话，则关闭voice codec
        
        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_AUDIO_DISCONNECTED");
        SCI_TRACE_LOW("ccapp: MMICC_HandleBtPsMsg APP_BT_HFU_AUDIO_DISCONNECTED  MMICC_GetBtCallFlag:%d,active: %d, hold: %d, outgoing: %d,s_bt_current_call_index:%d", 
            MMICC_GetBtCallFlag(), s_call_context.exist_call.exist_active_call_num, s_call_context.exist_call.exist_hold_call_num,s_call_context.exist_call.exist_outgoing_call,s_bt_current_call_index);
        s_bt_audconnind_flag = FALSE;
#if 0 //cr237057        
        if(MMICC_GetBtCallFlag() && (s_bt_current_call_index < CC_MAX_CALL_NUM))
#endif 
        SCI_TRACE_LOW("ccapp: MMICC_HandleBtPsMsg APP_BT_HFU_AUDIO_DISCONNECTED MMIAPICC_IsInState(CC_IN_CALL_STATE):%d,CC_IsExistOperatedCall:%d",
            MMIAPICC_IsInState(CC_IN_CALL_STATE),CC_IsExistOperatedCall());
        if(!(!MMICC_GetBtCallFlag() && MMIAPICC_IsInState(CC_IN_CALL_STATE)))
        {
            SCI_TRACE_LOW("ccapp: MMICC_HandleBtPsMsg APP_BT_HFU_AUDIO_DISCONNECTED msg_ptr->status:%d", msg_ptr->status);
            if(PNULL != msg_ptr && BT_SUCCESS == msg_ptr->status)
            {        
                //lzk begin 2018.10.12
                //bug 944220
                SCI_TRACE_LOW("lzk log. exist_active_call_num = %d", s_call_context.exist_call.exist_active_call_num);
                SCI_TRACE_LOW("lzk log. exist_hold_call_num = %d", s_call_context.exist_call.exist_hold_call_num);
                SCI_TRACE_LOW("lzk log. exist_incoming_call = %d", s_call_context.exist_call.exist_incoming_call);
                SCI_TRACE_LOW("lzk log. exist_mpty = %d", s_call_context.exist_call.exist_mpty);
                SCI_TRACE_LOW("lzk log. exist_mtfw = %d", s_call_context.exist_call.exist_mtfw);
                SCI_TRACE_LOW("lzk log. exist_outgoing_call = %d", s_call_context.exist_call.exist_outgoing_call);

                {
                    int current_global_calls_count = s_call_context.exist_call.exist_active_call_num
                                                   + s_call_context.exist_call.exist_hold_call_num
                                                   + s_call_context.exist_call.exist_incoming_call
                                                   + s_call_context.exist_call.exist_outgoing_call;
                    SCI_TRACE_LOW("lzk log. current_global_calls_count = %d.", current_global_calls_count);
                    if (current_global_calls_count > 1)
                    {
                    }
                    else
                    {
                        MMICC_EnableVoiceCodec(FALSE);
                        MMICC_DestroyVoiceCodec(); 
                        SCI_TRACE_LOW("lzk log. MMICC_DestroyAudioHandle");
                        MMICC_DestroyAudioHandle();
                    }

                }
                //944220
                //lzk end 2018.10.12
				
                //233618
                MMICC_UpdateStatusWin();
            }
            else
            {
                SCI_TRACE_LOW("ccapp: MMICC_HandleBtPsMsg APP_BT_HFU_AUDIO_DISCONNECTED bt Faild");
            }
        }
        break;
	case APP_BT_HFU_CLCC_RECORD_IND:

        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_CLCC_RECORD_IND");
        if (PNULL != msg_ptr && BT_SUCCESS == msg_ptr->status && PNULL == MMICC_GetAppletInstance())
        {
            MMICC_StartCCApplet();
            
            s_bt_setupind_flag = TRUE;

            MMICC_SetBtCallFlag(TRUE);

            CC_OpenAudioTunnel(TRUE);
            
            MMICC_UpdateStatusWin();
        }

        CC_BtCallClccRecordInd(msg_ptr);
        break;
        
    case APP_BT_HFU_CLCC_RESULT_IND:

        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_CLCC_RESULT_IND");
        CC_BtCallClccResultInd(msg_ptr);
        break;
    case APP_BT_HFU_CCWA_IND:

        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_CCWA_IND");
        CC_BtCallCcwaInd(msg_ptr);
        break;
    case APP_BT_HFU_SPK_VOL_IND:

        SCI_TRACE_LOW("[MMICC] MMICC_HandleBtPsMsg APP_BT_HFU_SPK_VOL_IND");
        CC_BtCallSpkVolInd(msg_ptr);
        break;

    default:
        result = MMI_RESULT_FALSE;
        SCI_TRACE_LOW("ccapp.c: MMICC_HandleBtPsMsg() CC isn't process, msg_id = %d",msg_id);
        break;
    }
    
    //Free msg space
    if (msg_ptr != PNULL && msg_ptr->body_ptr != PNULL && msg_ptr->body_size >0)
    {
	    #ifdef WIN32
	    SCI_FREE(msg_ptr->body_ptr);
	    #else		
	    OS_Pfree(msg_ptr->body_ptr); /*lint !e718 */
	    #endif
	    msg_ptr->body_ptr = PNULL;
    }
    
    return result;
}
/*****************************************************************************/
// 	Description : get whether exist  waiting call bt connetion
//	Global resource dependence : 
//  Author : chunjie.liu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetBtConnWaitCallFlag(void)
{
    return s_bt_connect_wait_exist;
}

/*****************************************************************************/
// 	Description : set whether exist  waiting call bt connetion
//	Global resource dependence : 
//  Author : chunjie.liu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICC_SetBtConnWaitCallFlag(BOOLEAN wating_call_flag)
{
    s_bt_connect_wait_exist = wating_call_flag;
}
/*****************************************************************************/
// 	Description : get whether exist  bt call
//	Global resource dependence : 
//  Author : chunjie.liu
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_GetBtCallFlag(void)
{
    return s_bt_call_flag;
}

/*****************************************************************************/
// 	Description : set whether exist  bt call
//	Global resource dependence : 
//  Author : chunjie.liu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICC_SetBtCallFlag(BOOLEAN bt_call_flag)
{
    s_bt_call_flag = bt_call_flag;
}

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: chunjie.liu
//	Note:
/*****************************************************************************/
LOCAL CC_RESULT_E CC_BTSendDtmfByChar(uint8 dtmf_ch)
{
    CC_RESULT_E result = CC_RESULT_ERROR;
    BT_STATUS bt_status = BT_ERROR;

    //check the param
    if (!CC_IsCallIDValid(s_call_context.current_call))
    {
        SCI_TRACE_LOW("ccapp.c:MMIAPICC_SendDtmfByChar() current_call invalid %d",s_call_context.current_call);
        return CC_RESULT_ERROR;
    }

    if(!((dtmf_ch >= '0' && dtmf_ch <= '9') || 
         (dtmf_ch >= 'A' && dtmf_ch <= 'D') ||
          dtmf_ch == '#' ||
          dtmf_ch =='*')
        )
    {
        dtmf_ch = CC_INVALID_DTMF_VALUE;
    }

  //if the pressed key is valid
    if(CC_INVALID_DTMF_VALUE != dtmf_ch)
    {
#if !defined(MMI_DIALPANEL_DTMF_SUPPORT)
        //播放tone音
        MMISRVAUD_PlayToneEx(MMISRVAUD_PRI_HIGH, MMISRVAUD_TYPE_TONE_GENERIC, MMI_NT_RING_CON_ID, MMI_DTMF_TONE_PLAY_TIME,MMISRVAUD_TONE_FREQ_DEFAULT);
#endif

        bt_status = MMIAPIBT_HfuSendDtmf(dtmf_ch);
        if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c:CC_BTSendDtmfByChar() send dtmf failure");
            return CC_RESULT_ERROR;
        }
        
        return CC_RESULT_SUCCESS;
    }
    return result;
}

/*****************************************************************************/
// 	Description : handle MT Call disconnect
//	Global resource dependence : 
//  Author:chunjie liu
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandleSimMTCallDisc(DPARAM param)
{
    APP_MN_SETUP_IND_T	*pSig = (APP_MN_SETUP_IND_T*)param;
    uint8				tele_num[CC_MAX_TELE_NUM_LEN + 2]= {0};
    uint8	tel_num_len = 0;

    if(PNULL== pSig)
    {
        SCI_TRACE_LOW("ccapp.c: HandleSimMTCallDisc() pSig == PNULL");
        return FALSE;
    }

    //check the param
    SCI_TRACE_LOW("ccapp.c:enter function HandleSimMTCallDisc(), s_call_context.dual_sys = %d, s_call_context.call_number = %d",pSig->dual_sys, s_call_context.call_number);
    
    if(  pSig->call_id >= CC_MAX_CALL_NUM ) 
    {
        SCI_TRACE_LOW("ccapp.c: HandleSimMTCallDisc() , call_id = %d",pSig->call_id);
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_3229_112_2_18_2_10_13_84,(uint8*)"d", pSig->call_id);
        return FALSE;
    }

    if( pSig->calling_num_present )
    {        
        tel_num_len =  MMIAPICOM_GenNetDispNumber( pSig->calling_num.number_type,
            (uint8)pSig->calling_num.num_len,
            (uint8*)pSig->calling_num.party_num,
            tele_num ,
            (CC_MAX_TELE_NUM_LEN + 2));
    }
{
        MMICL_CALL_INFO_T		cl_info = {0};
        
        SCI_TRACE_LOW("HandleSimMTCallDisc s_call_context.call_number= %d ", s_call_context.call_number);
        if(MN_RETURN_SUCCESS != MNCALL_DisconnectCallEx(pSig->dual_sys, pSig->call_id, MN_CAUSE_USER_BUSY, NULL))
        {
            SCI_TRACE_LOW("ccapp.c:HandleSimMTCallDisc() MNCALL_DisconnectCallEx fail");
            return FALSE;
        }
        SCI_MEMSET(&cl_info, 0, sizeof(cl_info));
        //copy call_info.call_time
        cl_info.call_time.start_time = cl_info.call_time.stop_time
            = MMIAPICOM_GetCurTime();
        if (MN_CALL_TYPE_DATA == pSig->call_type)
        {
            cl_info.call_type = CC_CALL_VIDEO;
        }
        else
        {
            cl_info.call_type = CC_CALL_NORMAL;
        }
        if(pSig->calling_num_present)
        //copy call_info.call_num.is_num_present
        {
            if(pSig->calling_num.num_len > 0)
            {
                cl_info.call_num.is_num_present = TRUE;
            }
            else
            {
                cl_info.call_num.is_num_present = FALSE;
            }

             //copy call_info.call_num.number
            cl_info.call_num.number.number_type = pSig->calling_num.number_type;
            cl_info.call_num.number.number_plan = pSig->calling_num.number_plan;
	     cl_info.call_num.number.num_len = (pSig->calling_num.num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: pSig->calling_num.num_len;
            MMI_MEMCPY(
            (void*)cl_info.call_num.number.party_num,
            sizeof(cl_info.call_num.number.party_num),
            (void*)pSig->calling_num.party_num,
            sizeof(pSig->calling_num.party_num),
            cl_info.call_num.number.num_len
            );
            cl_info.is_emergency_call = MMIAPICC_IsEmergencyNum(tele_num, tel_num_len, pSig->dual_sys, FALSE);
        }
#ifdef MMI_CSP_SUPPORT
            if(!cl_info.is_emergency_call)
            {
                cl_info.line = (MN_BEARER_CAPABILITY_LINE_2 == pSig->bc_type)?1:0;
            }
#endif
            MMIAPICL_RecordCallInfo(pSig->dual_sys, MMICL_CALL_MISSED, cl_info);
            s_call_context.missed_call_num++;
            MAIN_SetMissedCallState(TRUE);
            MMIAPICC_SaveMissedCallNumToNv();
            s_is_new_missed_call = TRUE;  

    }
    return TRUE;
}

/*****************************************************************************/
// 	Description : process when received signal ciev call == 0
//	Global resource dependence : 
//  Author:chunjie liu
//	Note:2013.06.08
/*****************************************************************************/
LOCAL void CC_BtSaveCLInfo(MMICL_CALL_INFO_T*  cl_info_ptr, MMICL_CALL_TYPE_E * calllog_type_ptr,uint8 call_index)
{
    uint8			        tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8			        tel_num_len = 0;
    SCI_MEMSET(cl_info_ptr, 0, sizeof(*cl_info_ptr));
    
    //s_call_context.call_info[ call_index ].call_time.stop_time = MMIAPICOM_GetCurTime();
    
    MMI_MEMCPY(
        &cl_info_ptr->call_time,
        sizeof(cl_info_ptr->call_time),
        &s_call_context.call_info[call_index].call_time,
        sizeof(s_call_context.call_info[call_index].call_time),
        sizeof(CC_CALL_TIME_T)
        );
    
    if( s_call_context.call_info[call_index].name_len > 0 )
    {
        cl_info_ptr->call_num.is_num_present = TRUE;
    }
    else
    {
        cl_info_ptr->call_num.is_num_present = FALSE;
    }

    cl_info_ptr->is_bt_dail = TRUE;
#if defined(CL_SAVE_NAME_SUPPORT) 
    cl_info_ptr->name.is_use_nv_name = s_call_context.call_info[call_index].is_save_name;
    
    if (s_call_context.call_info[call_index].is_save_name && s_call_context.call_info[call_index].is_name_exist)
    {
        cl_info_ptr->name.wstr_len = MIN(s_call_context.call_info[call_index].name_len, MMICL_NAME_MAX_LEN);
        MMI_WSTRNCPY(
            cl_info_ptr->name.wstr,
            MMICL_NAME_MAX_LEN,
            s_call_context.call_info[call_index].name,
            CC_MAX_PBNAME_LEN,
            cl_info_ptr->name.wstr_len
            );  
    }
#endif

    s_is_call_modified_store = FALSE;
     //for ip call
    if (s_call_context.call_info[call_index].is_call_modified_store)
    {
        s_call_context.call_info[call_index].is_call_modified_store = FALSE;

        GetCallInfoNumber(cl_info_ptr, &s_call_context.call_info[call_index].call_modified_address);

        //是否是紧急呼叫
        if (CC_MO_CALL_TYPE == s_call_context.call_info[call_index].call_type)
        {
            tel_num_len = MMIAPICOM_GenDispNumber(
                s_call_context.call_info[call_index].call_modified_address.number_type, 
                (uint8)s_call_context.call_info[call_index].call_modified_address.num_len, 
                s_call_context.call_info[call_index].call_modified_address.party_num,
                tel_num,
                sizeof(tel_num)
                );
            cl_info_ptr->is_emergency_call = FALSE;
        }
    }
    else
    {
        GetCallInfoNumber(cl_info_ptr, &s_call_context.call_info[call_index].address);

        //是否是紧急呼叫
        if (CC_MO_CALL_TYPE == s_call_context.call_info[call_index].call_type)
        {
            tel_num_len = MMIAPICOM_GenDispNumber(
                s_call_context.call_info[call_index].address.number_type, 
                (uint8)s_call_context.call_info[call_index].address.num_len, 
                s_call_context.call_info[call_index].address.party_num,
                tel_num,
                sizeof(tel_num)
                );
            cl_info_ptr->is_emergency_call = FALSE;
        }
    }
    SCI_TRACE_LOW("mmiccapp:CC_BtSaveCLInfo,s_call_context.call_info[%d].call_type = %d",call_index,s_call_context.call_info[ call_index ].call_type);
    switch(  s_call_context.call_info[ call_index ].call_type )
    {
    case CC_MO_CALL_TYPE:
        // 已拨电话//@samboo 建议将电话纪录单独提取出来，在disconnect_ind收到后处理，而不是分布在各个地方进行处理
        //call the interface of calllog  
        *calllog_type_ptr = MMICL_CALL_DIALED;
        break;
        
    case CC_MT_CALL_TYPE:
        SCI_TRACE_LOW("mmiccapp:CC_BtSaveCLInfo,operated_by_user = %d, call_state = %d",s_call_context.call_info[ call_index ].operated_by_user, s_call_context.call_info[call_index].call_state);
        if (((CC_NO_OPER == s_call_context.call_info[ call_index ].operated_by_user) &&
            ((CC_INCOMING_CALL_STATE == s_call_context.call_info[call_index].call_state) ||
            (CC_WAITING_STATE == s_call_context.call_info[call_index].call_state)))
            )
        {
            //未接来电
            *calllog_type_ptr = MMICL_CALL_MISSED;

            s_call_context.missed_call_num++;
            MAIN_SetMissedCallState(TRUE);
            
            MMIAPICC_SaveMissedCallNumToNv();
            s_is_new_missed_call = TRUE;
        }
        else
        {
            //已接电话
            *calllog_type_ptr = MMICL_CALL_RECEIVED;
        }
        break;
        
    default:
        break;
        
    }    
}

/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CIEV_IND callsetup=2
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.06.09
/*****************************************************************************/
LOCAL void CC_BtCallStartInd(void)
{
    uint8				copy_len = 0;

    SCI_TRACE_LOW("ccapp.c: CC_BtCallStartInd()  s_call_context.call_number= %d,s_call_context.want_mo_call.is_want_mo_call:%d",s_call_context.call_number,s_call_context.want_mo_call.is_want_mo_call);
        
    if(CC_INVALID_INDEX != s_bt_current_call_index)
    {
        if(s_call_context.want_mo_call.is_want_mo_call)
        {
            s_call_context.exist_call.exist_outgoing_call = FALSE;
        }
        return;
    }
    if(!s_call_context.want_mo_call.is_want_mo_call)
    {
        BT_STATUS  bt_status = MMIAPIBT_HfuListCurrentCalls();
        SCI_TRACE_LOW("ccapp.c: CC_BtCallStartInd() HfuList done.");
        if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
        {   
            SCI_TRACE_LOW("ccapp.c: CC_BtCallStartInd() MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
        }
        s_call_context.exist_call.exist_outgoing_call =TRUE;
        MMICC_SetBtCallFlag(TRUE);
    }
    
    //modify global: s_call_context	
    s_call_context.call_number++;
    s_call_context.current_call = s_call_context.call_number - 0x01;
    s_bt_current_call_index = s_call_context.current_call;
    //get name from control
    //SCI_MEMCPY(s_call_context.call_info[s_call_context.current_call].name, name, strlen(name));
    s_call_context.call_info[s_call_context.current_call].call_state = CC_CALLING_STATE;
    SCI_TRACE_LOW("ccapp.c: CC_BtCallStartInd()  s_call_context.exist_active_call_num=%d,s_call_context.call_number=%d, s_call_context.current_call=%d, s_bt_current_call_index=%d, s_call_context.call_info[s_bt_current_call_index].call_state:%d",s_call_context.exist_call.exist_active_call_num,s_call_context.call_number,
        s_call_context.current_call,s_bt_current_call_index, s_call_context.call_info[s_bt_current_call_index].call_state);

    if(s_call_context.want_mo_call.is_want_mo_call)
    {
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
        copy_len = s_call_context.want_mo_call.name_len;
#else    
        copy_len = ( s_call_context.want_mo_call.name_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : s_call_context.want_mo_call.name_len;/*lint !e506 */
#endif
        MMI_WSTRNCPY(
            s_call_context.call_info[s_call_context.current_call].name,
            sizeof(s_call_context.call_info[s_call_context.current_call].name),
            s_call_context.want_mo_call.name,
            sizeof(s_call_context.want_mo_call.name),
            copy_len
            );
        s_call_context.call_info[s_call_context.current_call].name_len = copy_len;
        s_call_context.call_info[s_call_context.current_call].address = s_call_context.want_mo_call.address;
        s_call_context.call_info[s_call_context.current_call].is_save_name = s_call_context.want_mo_call.is_save_name;
        s_call_context.call_info[s_call_context.current_call].is_name_exist = s_call_context.want_mo_call.is_name_exist;
    }
    s_call_context.call_info[s_call_context.current_call].call_type = CC_MO_CALL_TYPE;
    s_call_context.call_info[s_call_context.current_call].call_time.start_time = MMIAPICOM_GetCurTime( );
    s_call_context.call_info[s_call_context.current_call].call_time.stop_time
        = s_call_context.call_info[s_call_context.current_call].call_time.start_time;
    s_call_context.call_info[s_call_context.current_call].is_mute = FALSE;

    //Save modified number
    if (s_is_call_modified_store)
    {
        s_is_call_modified_store = FALSE;

        s_call_context.call_info[s_call_context.current_call].is_call_modified_store = TRUE;

        SCI_MEMSET(&s_call_context.call_info[s_call_context.current_call].call_modified_address, 0x00, sizeof(s_call_context.call_info[s_call_context.current_call].call_modified_address));

        SCI_MEMCPY(&s_call_context.call_info[s_call_context.current_call].call_modified_address,
                            &s_call_modified_address,
                            sizeof(s_call_context.call_info[s_call_context.current_call].call_modified_address));
    }

    MMICC_EnableMute(MMICC_IsCurrentCallMute());
}

/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CIND_IND
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallCindInd(BT_MSG_T *msg_ptr)
{
    BT_HFG_INDICATOR_T  *msg_body_ptr = PNULL;
    BT_STATUS  bt_status = BT_ERROR;

    //check the param
    if(PNULL== msg_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd() msg_ptr == PNULL");
        return;
    }
    msg_body_ptr = (BT_HFG_INDICATOR_T*)(msg_ptr->body_ptr);
    if(PNULL== msg_body_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd() msg_body_ptr == PNULL");
        return;
    }
    if(BT_SUCCESS != msg_ptr->status)
    {
        SCI_TRACE_LOW("ccapp.c:CC_BtCallCievInd msg_ptr->status %d ", msg_ptr->status);
        return;
    }
    SCI_TRACE_LOW("ccapp.c:CC_BtCallCievInd() msg_body_ptr->call_setup = %d,msg_body_ptr->hold_call = %d,msg_body_ptr->active_calls = %d",msg_body_ptr->call_setup,msg_body_ptr->hold_call,msg_body_ptr->active_calls);
     if(CC_INVALID_INDEX != s_bt_current_call_index)
    {
        SCI_TRACE_LOW("CC_BtCallClipInd CC_INVALID_INDEX != s_bt_current_call_index s_call_context.call_number= %d ", s_call_context.call_number);
        return;
    }
     //there is  sim call, ag make  bt connection
    if(!MMICC_GetBtCallFlag() && MMIAPICC_IsInState(CC_IN_CALL_STATE) && !(msg_body_ptr->active_calls == 0 && msg_body_ptr->hold_call == 0))
    {
        //SPCSS00056683 if there is sim call, ag make bt connection, should hangup but not release local call
        //MMIAPICC_ReleaseCallByRedkey();
        bt_status = MMIAPIBT_HfuHangup();
        if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c:CC_BtCallCievInd()  MMIAPIBT_HfuHangup fail");
        }
    }
    
    if(((BT_HFP_IND_CALLSETUP_OUTGOING == msg_body_ptr->call_setup) || (BT_HFP_IND_CALLSETUP_REMOTE_ALERTED == msg_body_ptr->call_setup))
        || (BT_HFP_IND_CALLSETUP_INCOMING == msg_body_ptr->call_setup)
        ||((BT_HFP_IND_CALLSETUP_NOT_IN_CALLSETUP == msg_body_ptr->call_setup) && (BT_HFP_IND_CALL_CALL_ACTIVE == msg_body_ptr->active_calls))
        )
    {
        if(PNULL == MMICC_GetAppletInstance())
        {
            MMICC_StartCCApplet();
        }
        bt_status = MMIAPIBT_HfuListCurrentCalls();
        if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
        {   
            SCI_TRACE_LOW("ccapp.c: CC_BtCallStartInd() MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
        }
        MMICC_SetBtCallFlag(TRUE);
        if((BT_HFP_IND_CALLSETUP_INCOMING == msg_body_ptr->call_setup) 
            && (BT_HFP_IND_CALL_CALL_ACTIVE == msg_body_ptr->active_calls) 
            && (BT_HFP_IND_CALLHELD_NO_CALL_HELD == msg_body_ptr->hold_call))
        {
            MMICC_SetBtConnWaitCallFlag(TRUE);
        }
#ifdef FM_SUPPORT
        MMIAPIFM_StopRecord();
        MMIAPIFM_StopAutoSearch();
#endif
    	
        MMIAPIFMM_ExitFileOption();
        
        MMIAPIDC_Interrupt();
#ifdef MMI_RECORD_SUPPORT
        MMIAPIRECORD_StopRecordOrPlay(); //打出电话的时候停止录音
#endif    
        MMIAPIPICVIEW_StopDeleteFiles();
        //not necessary here
#if defined(VIDEO_PLAYER_SUPPORT) && !defined(MMI_VIDEOPLAYER_MINI_FUNCTION)
        MMIAPIVP_NotifyVPStop();
#endif

#ifdef MOBILE_VIDEO_SUPPORT
        if(MMIAPIMV_IsLiving())
        {
            MMIAPIMV_Exit();
        }
#endif
    }     
}

/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CIEV_IND
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallCievInd(BT_MSG_T *msg_ptr)
{
    BT_HFU_CIEV_T  *msg_body_ptr = PNULL;
    BT_STATUS bt_status = BT_ERROR;
    uint32 i = 0;
    uint8 hold_call_num = 0;

    //check the param
    if(PNULL== msg_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd() msg_ptr == PNULL");
        return;
    }
    msg_body_ptr = (BT_HFU_CIEV_T*)(msg_ptr->body_ptr);
    if(PNULL== msg_body_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd() msg_body_ptr == PNULL");
        return;
    }
    if(BT_SUCCESS != msg_ptr->status)
    {
        SCI_TRACE_LOW("ccapp.c:CC_BtCallCievInd msg_ptr->status %d ", msg_ptr->status);
        return;
    }
    SCI_TRACE_LOW("ccapp.c:CC_BtCallCievInd() msg_body_ptr->ind_type = %d,msg_body_ptr->ind_value = %d",msg_body_ptr->ind_type,msg_body_ptr->ind_value);

    SCI_MEMCPY((void*)&s_bt_ciev_info , (void*)msg_body_ptr , sizeof(BT_HFU_CIEV_T));

    switch(msg_body_ptr->ind_type)
    {
    case BT_CALL_SETUP_STATUS_INDICATOR:
        switch(msg_body_ptr->ind_value)
        {
        case BT_HFP_IND_CALLSETUP_INCOMING://<value>=1 means an incoming call process ongoing 
            //if there is a sim call , a bt call incoming ,save call log in message clip
            if(!MMICC_GetBtCallFlag() && MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {                
                SCI_TRACE_LOW("CC_BtCallCievInd s_call_context.call_number= %d ", s_call_context.call_number);
                bt_status = MMIAPIBT_HfuHangup();
                if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
                {
                    SCI_TRACE_LOW("ccapp.c:CC_BtCallCievInd() MMIAPIBT_HfuHangup fail");
                }
                return;
            }
		//if there is a bt call , a bt call incoming ,ccwa message is befor this message
            if(!MMICC_GetBtCallFlag())
            {
                s_bt_setupind_flag = TRUE;
                MMICC_SetBtCallFlag(TRUE);
                if(PNULL == MMICC_GetAppletInstance())
                {
                    MMICC_StartCCApplet();
                }
            }
            break;
        case BT_HFP_IND_CALLSETUP_NOT_IN_CALLSETUP://<value>=0 means not currently in call set up.
            SCI_TRACE_LOW("ccapp.c:CC_BtCallCievInd()--(3, 0)--start exist_active_call_num=%d, call_number=%d, current_call=%d, s_bt_current_call_index=%d, Flag:%d",
                s_call_context.exist_call.exist_active_call_num,s_call_context.call_number, s_call_context.current_call,s_bt_current_call_index, MMICC_GetBtCallFlag());
            if((s_bt_current_call_index < CC_MAX_CALL_NUM)  
                && ((CC_CALLING_STATE == s_call_context.call_info[s_bt_current_call_index].call_state) 
                || (CC_INCOMING_CALL_STATE == s_call_context.call_info[s_bt_current_call_index].call_state)))
            {
                if(CC_CALLING_STATE == s_call_context.call_info[s_bt_current_call_index].call_state)
                {
                    ClearWantCallInfo();
                }
                CC_BtDisconnectedCall();
            }
            else if(MMICC_GetBtCallFlag() && !s_call_context.call_number && !s_call_context.exist_call.exist_active_call_num)
            {
                s_bt_setupind_flag = FALSE;
                MMICC_SetBtCallFlag(FALSE);
                     
                MMICC_EnableVoiceCodec(FALSE);
                MMICC_DestroyVoiceCodec();      
                MMICC_UpdateStatusWin();
                    
                if(PNULL != MMICC_GetAppletInstance())
                {
                    MMK_PostMsg( MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
                }                
            }
#if defined(MMI_MULTI_SIM_SYS_NONE) 
            else if((1 < s_call_context.call_number) && (CC_WAITING_STATE == s_call_context.call_info[s_bt_current_call_index].call_state))
            {
                CC_BtDisconnectCallByIndex(s_bt_current_call_index);
            }
#endif
            bt_status = MMIAPIBT_HfuListCurrentCalls();//CLCC+APP_BT_HFU_CLCC_RECORD_IND//bug924697
            if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
            {   
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd BT_HFP_IND_CALL_NO_CALL_ACTIVE (3,0) MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
            }
            SCI_TRACE_LOW("ccapp.c CC_BtCallCievInd--(3, 0)--end,s_call_context.exist_active_call_num=%d,s_call_context.call_number=%d, s_call_context.current_call=%d, s_bt_current_call_index=%d, s_call_context.call_info[s_bt_current_call_index].call_state:%d,MMICC_GetBtCallFlag:%d",
                s_call_context.exist_call.exist_active_call_num,s_call_context.call_number, s_call_context.current_call,s_bt_current_call_index, s_call_context.call_info[s_bt_current_call_index].call_state, MMICC_GetBtCallFlag());
            break;   
        case BT_HFP_IND_CALLSETUP_OUTGOING://<value>=2 means an outgoing call set up is ongoing. 
            SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd--(3, 2)--CallFlag=%d, CallState=%d, s_bt_audconnind_flag=%d", MMICC_GetBtCallFlag(), MMIAPICC_IsInState(CC_IN_CALL_STATE), s_bt_audconnind_flag);
            //there is  sim call, ag make a bt call 
            if(!MMICC_GetBtCallFlag() && MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {
                s_current_index = s_call_context.current_call;
                g_call_disc_by_bt = TRUE;
                MMIAPICC_ReleaseCallByRedkey();
                CC_OpenAudioTunnel(FALSE);    
            }
            if(PNULL == MMICC_GetAppletInstance())
            {
                MMICC_StartCCApplet();
            }
            CC_BtCallStartInd();
            
            //233980
            if(s_bt_audconnind_flag && MMICC_GetBtCallFlag())
            {
                s_bt_audconnind_flag = FALSE;
                CC_OpenAudioTunnel(TRUE);
            }
            break;
        case BT_HFP_IND_CALLSETUP_REMOTE_ALERTED://<value>=3 means remote party being alerted in an outgoing call.
            //CC_OpenAudioTunnel(TRUE); //mt mas ring before connect
            SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd--(3, 3)");
            bt_status = MMIAPIBT_HfuListCurrentCalls();//CLCC+APP_BT_HFU_CLCC_RECORD_IND//bug924697
            if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
            {   
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd BT_HFP_IND_CALL_NO_CALL_ACTIVE 3,3 MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
            }
            break;
        default:
            break;    
    
        }
        break;
    case BT_CALL_STATUS_INDICATOR:
        SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd--(2,%d):MMICC_GetBtCallFlag():%d,s_bt_current_call_index:%d,s_call_context.call_info[s_bt_current_call_index].call_state:%d",
            msg_body_ptr->ind_value,MMICC_GetBtCallFlag(),s_bt_current_call_index,s_call_context.call_info[s_bt_current_call_index].call_state);
        switch(msg_body_ptr->ind_value)
        {
        case BT_HFP_IND_CALL_CALL_ACTIVE://<value>=1 means a call is active.     
            if(!MMICC_GetBtCallFlag())//225866 when hold and back case
            {
                if(MMIAPICC_IsInState(CC_IN_CALL_STATE) )
                {
				   //cr230066
                   bt_status = MMIAPIBT_HfuHangup();
                   if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
                   {
                       SCI_TRACE_LOW("ccapp.c:CC_BtCallCievInd BT_HFP_IND_CALL_CALL_ACTIVE2,1 MMIAPIBT_HfuHangup fail");
                   }
                   bt_status = MMIAPIBT_HfuListCurrentCalls();//CLCC+APP_BT_HFU_CLCC_RECORD_IND//bug924697
                   if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
                   {   
                       SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd BT_HFP_IND_CALL_NO_CALL_ACTIVE (2,1) MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
                   }
                   SCI_TRACE_LOW("ccapp.c CC_BtCallCievInd BT_HFP_IND_CALL_CALL_ACTIVE2,1 : SIM call exist!!!");
                   break;
                }            
            
                if(PNULL == MMICC_GetAppletInstance())
                {
                    MMICC_StartCCApplet();
                }
                bt_status = MMIAPIBT_HfuListCurrentCalls();
                if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
                {   
                    SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd() MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
                }
                MMICC_SetBtCallFlag(TRUE);        
            }
            else           
            {
                if ((!s_bt_sco_link_is_exist))
                {
                    SCI_TRACE_LOW("s_bt_sco_link is not exist, need call MMIAPIBT_HfuSwitchAudio here!");	
                    MMIAPIBT_HfuSwitchAudio(TRUE);
                }
                //238887
                if((!MMICC_IsExistActiveCall()) && (!MMICC_IsExistHoldCall()))
                {
                    CC_SetupComplete();
                }
            }            
            break;
        case BT_HFP_IND_CALL_NO_CALL_ACTIVE://<value>=0 means no call active. 
            SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd 2,0 MMICC_GetBtCallFlag:%d,s_bt_current_call_index:%d,s_call_context.call_info[s_bt_current_call_index].call_state:%d", 
                  MMICC_GetBtCallFlag(),s_bt_current_call_index,s_call_context.call_info[s_bt_current_call_index].call_state);
        	if(CC_CALLING_STATE == s_call_context.call_info[s_bt_current_call_index].call_state)
            {
                ClearWantCallInfo();
            }
#if defined(MMI_MULTI_SIM_SYS_NONE) 
            else if((1 < s_call_context.call_number) && 
                !MMICC_IsExistMPTY() &&
                ((CC_WAITING_STATE == s_call_context.call_info[s_bt_current_call_index].call_state) || 
                (CC_CALL_STATE == s_call_context.call_info[s_bt_current_call_index].call_state))
                )
            {
                // the active call is disconnected , and some incoming call is waiting ~~
                uint8 incall_idx = FindStateCall(CC_CALL_STATE);
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd (2,0)-1 incall_idx:%d, s_call_context.call_number=%d", incall_idx, s_call_context.call_number);
                if (incall_idx < s_call_context.call_number)
                {
                    s_bt_current_call_index = s_call_context.current_call = incall_idx;
                    CC_BtDisconnectCallByIndex(s_bt_current_call_index);
                }		
            } else
#endif
            if(MMICC_GetBtCallFlag() && (s_bt_current_call_index < CC_MAX_CALL_NUM) )
            {
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd (2,0)-2 s_bt_current_call_index:%d", s_bt_current_call_index);
                //cr237057
                MMICC_EnableVoiceCodec(FALSE);
                MMICC_DestroyVoiceCodec();      
                MMICC_UpdateStatusWin();
                
                CC_BtDisconnectedCall();
            }
            bt_status = MMIAPIBT_HfuListCurrentCalls();//CLCC+APP_BT_HFU_CLCC_RECORD_IND//bug924697
            if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
            {
            	SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd BT_HFP_IND_CALL_NO_CALL_ACTIVE 2,0 MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
            }
            break;            
        default:
            break;    
    
        }
        break;        
    case BT_CALL_HELD_INDICATOR:
        switch(msg_body_ptr->ind_value)
        {
        case BT_HFP_IND_CALLHELD_NO_CALL_HELD://No calls held
             SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd()--(7, 0)");
		
            if(CC_RELEASE_OPER == s_call_context.call_info[s_call_context.current_call].operated_by_user)
            {
                s_call_context.call_info[s_call_context.current_call].operated_by_user = CC_NO_OPER;
            }
            if((0 < s_call_context.exist_call.exist_hold_call_num) && (0 < s_call_context.exist_call.exist_active_call_num))//有挂起 的电话且有正在通话的电话
            {
                hold_call_num = s_call_context.exist_call.exist_hold_call_num;
                for(i=0; i<hold_call_num; i++)
                {
                    CC_BtDisconnectCallByIndex(FindStateCall(CC_HOLD_STATE));
                }
            }
            else
            {
                bt_status = MMIAPIBT_HfuListCurrentCalls();
                if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
                {   
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd BT_HFP_IND_CALL_NO_CALL_ACTIVE 7,0 MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
                }
            }
            break;
#if defined(MMI_MULTI_SIM_SYS_NONE) || defined(BT_DIALER_SUPPORT)//bug924697
        case BT_HFP_IND_CALLHELD_ONE_HOLD_ONE_ACTIVE://<value>=1 , Call is placed on hold or active/held calls swapped
            {
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd 7,1 MMICC_GetBtCallFlag:%d,s_bt_current_call_index:%d,s_call_context.call_info[s_bt_current_call_index].call_state:%d", 
                    MMICC_GetBtCallFlag(),s_bt_current_call_index,s_call_context.call_info[s_bt_current_call_index].call_state);	
                
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd 7,1 s_call_context.call_info[s_call_context.current_call].operated_by_user :%d", 
                    s_call_context.call_info[s_call_context.current_call].operated_by_user);
                //MT case
                if((s_call_context.incoming_call_index < s_call_context.call_number) && 
                    (CC_WAITING_STATE == s_call_context.call_info[s_call_context.incoming_call_index].call_state))
                {
                    s_bt_current_call_index = s_call_context.incoming_call_index;
                    s_call_context.incoming_call_index = 0;
                    CC_SetupComplete();
                }
                //MO case
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd (7,1)-3 is_want_mo_call=%d, tele_len=%d", s_call_context.want_mo_call.is_want_mo_call, s_bt_make_call_info.tele_len); 
                if((TRUE == s_call_context.want_mo_call.is_want_mo_call) && 
                    (0 != s_bt_make_call_info.tele_len))			
                {                            
                    s_call_context.call_number++;
                    s_bt_current_call_index = s_call_context.call_number - 1;
                    CC_SetupComplete();   
                    SCI_MEMSET(&s_bt_make_call_info , 0x00 , sizeof(CC_MAKE_CALL_INFO));
                }
                // call swap
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd (7,1)-4.1 exist_active_call_num=%d, exist_hold_call_num=%d", s_call_context.exist_call.exist_active_call_num, s_call_context.exist_call.exist_hold_call_num); 
                if( ( CC_NO_NUM != s_call_context.exist_call.exist_active_call_num ) &&
                    ( CC_NO_NUM != s_call_context.exist_call.exist_hold_call_num ) &&
                    (CC_SHUTTLE_OPER == s_call_context.call_info[s_call_context.current_call].operated_by_user))
                {
                    SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd (7,1)-4.2 exist_active_call_num=%d, exist_hold_call_num=%d", s_call_context.exist_call.exist_active_call_num, s_call_context.exist_call.exist_hold_call_num); 
                    //modify the information in the s_call_context
                    for( i = 0 ; i < s_call_context.call_number ; i++ )
                    {
                        if( CC_WAITING_STATE != s_call_context.call_info[i].call_state )
                        {
                            if( CC_HOLD_STATE == s_call_context.call_info[i].call_state )
                            {
                                s_call_context.call_info[i].call_state = CC_CALL_STATE;
                                s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
                                s_call_context.exist_call.exist_active_call_num ++;
                                s_call_context.exist_call.exist_hold_call_num --;
                                s_call_context.current_call = s_bt_current_call_index = i;
                            }
                            else
                            {
                                s_call_context.call_info[i].call_state = CC_HOLD_STATE;
                                s_call_context.call_info[i].operated_by_user = CC_NO_OPER;
                                s_call_context.exist_call.exist_hold_call_num ++;
                                s_call_context.exist_call.exist_active_call_num --;
                            }
                        }
                    }
                    MMICC_UpdateCallStatusDisplay(MMICC_RETRIEVE_SUC_IND);					
                }
                bt_status = MMIAPIBT_HfuListCurrentCalls();//CLCC+APP_BT_HFU_CLCC_RECORD_IND//bug924697
                if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
                {   
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd BT_HFP_IND_CALL_NO_CALL_ACTIVE (7,1) MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
                }
            }
            break;     
        case BT_HFP_IND_CALLHELD_ONLY_HOlD_NO_ACTIVE://<value>=2 , Call on hold, no active call
            {			
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd--(7,2) MMICC_GetBtCallFlag:%d,s_bt_current_call_index:%d,s_call_context.call_info[s_bt_current_call_index].call_state:%d", 
                    MMICC_GetBtCallFlag(),s_bt_current_call_index,s_call_context.call_info[s_bt_current_call_index].call_state);
                SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd--(7,2)-1.1 exist_hold_call_num=%d, exist_active_call_num=%d, call_number=%d", s_call_context.exist_call.exist_hold_call_num, s_call_context.exist_call.exist_active_call_num, s_call_context.call_number);
                if((s_call_context.exist_call.exist_hold_call_num > 0) && (CC_CALL_STATE ==s_call_context.call_info[s_bt_current_call_index].call_state))  //means active call is released, 第二通电话没有接通之前不会走到此case
                {
                    SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd--(7,2)-2 s_bt_current_call_index:%d", s_bt_current_call_index);
                    CC_BtDisconnectCallByIndex(s_bt_current_call_index);
                }
                else if((CC_HOLD_STATE ==s_call_context.call_info[s_bt_current_call_index].call_state) &&
                    (TRUE ==s_call_context.want_mo_call.is_want_mo_call) &&
                    (0<s_bt_make_call_info.tele_len)
                    )
                {
                    SCI_TRACE_LOW("ccapp.c: CC_BtCallCievInd--(7,2)-3 ");
                    MMIAPICC_MakeBtCall(s_bt_make_call_info.tele_num , s_bt_make_call_info.tele_len);
                }
            }
            break;     			
#endif
        default:
            break;    
    
        }
        break;  
    default:
        break;    
    
    }
      
}

/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CLIP_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallClipInd(BT_MSG_T *msg_ptr)
{
    BT_HFU_CALLERID_PARAMS_T  *msg_body_ptr = PNULL;
    uint8				mt_call = CC_INVALID_INDEX;
    MN_CALLING_NUMBER_T call_num ={0};
    MMI_PARTY_NUMBER_T  party_num= {0};
    MMI_PARTY_NUMBER_T * party_num_ptr= &party_num;
    BT_STATUS bt_status = BT_ERROR;

    //check the param
    if(PNULL== msg_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClipInd() msg_ptr == PNULL");
        return;
    }
    msg_body_ptr = (BT_HFU_CALLERID_PARAMS_T*)msg_ptr->body_ptr;
    if(PNULL== msg_body_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClipInd() msg_body_ptr == PNULL");
        return;
    }
    if(BT_SUCCESS != msg_ptr->status)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClipInd status %d ", msg_ptr->status);
        return;
    }

    SCI_TRACE_LOW("ccapp.c: CC_BtCallClipInd() s_bt_current_call_index:%d,MMICC_GetBtCallFlag():%d,MMIAPICC_IsInState(CC_IN_CALL_STATE):%d,CC_IsExistOperatedCall:%d",
        s_bt_current_call_index,MMICC_GetBtCallFlag(),MMIAPICC_IsInState(CC_IN_CALL_STATE),CC_IsExistOperatedCall());
#if defined(MMI_BLACKLIST_SUPPORT)
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClipInd() CC_MTCallFireWall(MN_DUAL_SYS_1,&call_num,0,MN_CALL_TYPE_NORMAL,MN_BEARER_CAPABILITY_LINE_1):%d",
        CC_MTCallFireWall(MN_DUAL_SYS_1,&call_num,0,MN_CALL_TYPE_NORMAL,MN_BEARER_CAPABILITY_LINE_1));
#endif


if((!MMICC_GetBtCallFlag() && MMIAPICC_IsInState(CC_IN_CALL_STATE)) || CC_IsExistOperatedCall()
#if defined(MMI_BLACKLIST_SUPPORT)
        || CC_MTCallFireWall(MN_DUAL_SYS_1,&call_num,0,MN_CALL_TYPE_NORMAL,MN_BEARER_CAPABILITY_LINE_1)
#endif
        )
    {
        SCI_TRACE_LOW("ccapp.c: CC_MTCallFireWall: the call is disconnected");
        bt_status = MMIAPIBT_HfuHangup();
        if(BT_SUCCESS !=  bt_status&& BT_PENDING != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c:CC_BtCallClipInd() MMIAPIBT_HfuHangup fail");
        }

        if( !((!MMICC_GetBtCallFlag() && MMIAPICC_IsInState(CC_IN_CALL_STATE)) || CC_IsExistOperatedCall()) 
#if defined(MMI_BLACKLIST_SUPPORT)            
            && CC_MTCallFireWall(MN_DUAL_SYS_1,&call_num,0,MN_CALL_TYPE_NORMAL,MN_BEARER_CAPABILITY_LINE_1)
#endif            
            )
        {
            s_bt_setupind_flag = FALSE;
            MMICC_SetBtCallFlag(FALSE);
            MMICC_EnableVoiceCodec(FALSE);
            MMICC_DestroyVoiceCodec();      
            MMICC_UpdateStatusWin();
            if(PNULL != MMICC_GetAppletInstance())
            {
                MMK_PostMsg( MMICC_GetAppletInstance()->caf_applet.app_handle, MSG_CC_STOP_APPLET, PNULL, 0 );
            }
        }
        return;          
    }

    if(CC_INVALID_INDEX != s_bt_current_call_index ||  !MMICC_GetBtCallFlag())
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClipInd CC_INVALID_INDEX != s_bt_current_call_index s_call_context.call_number= %d ", s_call_context.call_number);
        //cr237299 using for second BT MT or SIM MT then return
        return;
    }
    
    MMIAPICOM_GenPartyNumber(msg_body_ptr->number, strlen((char*)msg_body_ptr->number), party_num_ptr);
    call_num.number_type = party_num_ptr->number_type;
    call_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
    call_num.num_len =(party_num_ptr->num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN:party_num_ptr->num_len;
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClipInd: party_num_ptr->num_len =%d,call_num.num_len=%d,strlen(msg_body_ptr->number)=%d",party_num_ptr->num_len,call_num.num_len,strlen((char*)msg_body_ptr->number));
    MMI_MEMCPY(
        call_num.party_num,
        sizeof(call_num.party_num),
        party_num_ptr->bcd_num,
        sizeof(party_num_ptr->bcd_num),
        call_num.num_len
        );

    //if there is a sim call , a bt call incoming ,save call log in message clip
    if(!MMICC_GetBtCallFlag())
    {
        MMICL_CALL_INFO_T		cl_info = {0};
        
        SCI_TRACE_LOW("CC_BtCallClipInd s_call_context.call_number= %d ", s_call_context.call_number);
        SCI_MEMSET(&cl_info, 0, sizeof(cl_info));
        //copy call_info.call_time
        cl_info.call_time.start_time = cl_info.call_time.stop_time
            = MMIAPICOM_GetCurTime();
        //copy call_info.call_num.is_num_present
        if(strlen((char*)msg_body_ptr->number) > 0)
        {
            cl_info.call_num.is_num_present = TRUE;
        }
        else
        {
            cl_info.call_num.is_num_present = FALSE;
        }
        cl_info.call_type = CC_CALL_NORMAL;

         //copy call_info.call_num.number
        cl_info.call_num.number.number_type = party_num_ptr->number_type;
        cl_info.call_num.number.number_plan = MN_NUM_PLAN_ISDN_TELE;
        cl_info.call_num.number.num_len = ( party_num_ptr->num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num_ptr->num_len;
        MMI_MEMCPY(
        (void*)cl_info.call_num.number.party_num,
        sizeof(cl_info.call_num.number.party_num),
        (void*)party_num_ptr->bcd_num,
        sizeof(party_num_ptr->bcd_num),
        cl_info.call_num.number.num_len
        );
        cl_info.is_bt_dail = TRUE;

        MMIAPICL_RecordCallInfo(MN_DUAL_SYS_1, MMICL_CALL_MISSED, cl_info);
        s_call_context.missed_call_num++;
        MAIN_SetMissedCallState(TRUE);
        MMIAPICC_SaveMissedCallNumToNv();
        s_is_new_missed_call = TRUE;

        return;
    }
    //update s_call_context
    {
        //s_call_context.dual_sys = pSig->dual_sys;
        s_call_context.call_number++;
        s_call_context.incoming_call_index = s_call_context.call_number - 1;
        mt_call = s_call_context.incoming_call_index;
        s_call_context.exist_call.exist_incoming_call = TRUE;
        //s_call_context.call_info[mt_call].call_id = pSig->call_id;
        //s_call_context.call_info[mt_call].dual_sys = pSig->dual_sys;
        //s_call_context.call_info[mt_call].call_mode = CC_CALL_NORMAL;
        if( CC_NO_NUM == s_call_context.call_number -1 )//判断之前有没有电话
        {
            s_call_context.call_info[mt_call].call_state = CC_INCOMING_CALL_STATE;
        }
        else
        {
            s_call_context.call_info[mt_call].call_state = CC_WAITING_STATE;
        }    
        
        /*s_call_context.call_info[mt_call].address.number_type
            =  pSig->calling_num.number_type;
        s_call_context.call_info[mt_call].address.number_plan
            =  pSig->calling_num.number_plan;*/
        
        s_call_context.call_info[mt_call].call_type = CC_MT_CALL_TYPE;
        s_call_context.call_info[mt_call].call_time.start_time = MMIAPICOM_GetCurTime( );
        
        s_call_context.current_call = FindStateCall(CC_INCOMING_CALL_STATE);
        if (s_call_context.current_call >= s_call_context.call_number)
        {
            s_call_context.current_call = FindStateCall(CC_WAITING_STATE);
        }
        s_bt_current_call_index = s_call_context.current_call;
        
        //MO的时候MT进来，MT优先，断掉MO
        s_call_context.want_mo_call.is_want_mo_call = FALSE;
        SCI_TRACE_LOW("ccapp.c CC_BtCallClipInd,s_call_context.call_info[%d].call_type=%d,",s_bt_current_call_index,s_call_context.call_info[s_bt_current_call_index].call_type);
    }
     // copy the number to address 
    if( 0 != strlen((char*)msg_body_ptr->number))
    {
        BOOLEAN     result = FALSE;
        MMI_STRING_T		name = {0};
        MMIPB_BCD_NUMBER_T	pb_num = {0};
        uint8				copy_len = 0;

        s_call_context.call_info[mt_call].address.num_len
            =  party_num_ptr->num_len;
        MMI_MEMCPY(
            s_call_context.call_info[mt_call].address.party_num,
            sizeof(s_call_context.call_info[mt_call].address.party_num),
            party_num_ptr->bcd_num,
            sizeof(party_num_ptr->bcd_num),
            sizeof(s_call_context.call_info[mt_call].address.party_num)
            );
        
        //根据号码搜索电话簿
        {
            SCI_MEMSET((void*)s_call_context.call_info[mt_call].name, 0, sizeof(s_call_context.call_info[mt_call].name) );
            SCI_MEMSET( &name, 0, sizeof(name) );
            name.wstr_ptr = s_call_context.call_info[mt_call].name;    
            
            //获得号码
            pb_num.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(party_num_ptr->number_type, MN_NUM_PLAN_ISDN_TELE);
            
            copy_len = ( party_num_ptr->num_len > MMIPB_BCD_NUMBER_MAX_LEN ) ? MMIPB_BCD_NUMBER_MAX_LEN : party_num_ptr->num_len;
            pb_num.number_len = copy_len;
            MMI_MEMCPY(
                pb_num.number,
                sizeof(pb_num.number),
                party_num_ptr->bcd_num,
                sizeof(party_num_ptr->bcd_num),
                copy_len
                );
            
            //在电话本中查找
            result = MMIAPIPB_GetNameByNumber(&pb_num, &name, CC_MAX_PBNAME_LEN, FALSE) ;/*lint !e506 */
            SCI_TRACE_BUF("ccapp.c, CC_BtCallClipInd pb_num=%x", pb_num.number, sizeof(pb_num.number));
            SCI_TRACE_LOW("ccapp.c, CC_BtCallClipInd search_result=%d, name.wstr_len=%d", result, name.wstr_len);
        }

        s_call_context.call_info[mt_call].is_mute = FALSE;
        s_call_context.want_mo_call.is_save_name = FALSE;
        
        //名字
        if (result && (0 != name.wstr_len))//电话本中存在且名字不为空
        {
            s_call_context.call_info[mt_call].is_name_exist = TRUE;
            
            s_call_context.call_info[mt_call].name_len = name.wstr_len ;
        }
        else
        {
            uint8	str_len = strlen((char*)msg_body_ptr->number);

            s_call_context.call_info[mt_call].is_name_exist = FALSE;
            
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
            copy_len = str_len;
#else            
            copy_len = ( str_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : str_len;/*lint !e506 */
#endif
            s_call_context.call_info[mt_call].name_len = copy_len;
            SCI_TRACE_LOW("%d,%d,%d",s_call_context.call_info[mt_call].name_len,str_len,copy_len);
            MMI_STRNTOWSTR(
                (void*)s_call_context.call_info[mt_call].name,
                s_call_context.call_info[mt_call].name_len,
                (void*)msg_body_ptr->number,
                str_len,
                copy_len
                );
        }
        }
    else
    {
        s_call_context.call_info[mt_call].name_len = 0;
    }
    MMIAPIFMM_ExitFileOption();
    
    MMIAPIDC_Interrupt();

    //MMIAPIRECORD_Stop();
    
    MMIAPIPICVIEW_StopDeleteFiles();
    //not necessary here
#if defined(VIDEO_PLAYER_SUPPORT) && !defined(MMI_VIDEOPLAYER_MINI_FUNCTION)
    MMIAPIVP_NotifyVPStop();
#endif

#ifdef MOBILE_VIDEO_SUPPORT
    if(MMIAPIMV_IsLiving())
    {
        MMIAPIMV_Exit();
    }
#endif    
    
#ifdef VT_SUPPORT
    if (MMIAPISET_GetVTOneTimeRingOption()
        && MMIAPIVT_IsFallBackCall(s_call_context.call_info[s_call_context.current_call].address))
    {
        //回落电话自动接听
        CC_ConnectCallReq();
    }
    else
#endif
    {
        MMICC_UpdateCallStatusDisplay(MMICC_MT_START_IND);
    }

    //check close stk display text win
    MMIAPISTK_CheckCloseDisplayTextIRWin();

    MMICC_EnableMute(MMICC_IsCurrentCallMute());
      
}

/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CCWA_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallCcwaInd(BT_MSG_T *msg_ptr)
{
    BT_HFU_CALLERID_PARAMS_T  *msg_body_ptr = PNULL;
    MN_CALLING_NUMBER_T call_num ={0};
    MMI_PARTY_NUMBER_T  party_num= {0};
    MMI_PARTY_NUMBER_T * party_num_ptr= &party_num;
    BT_STATUS bt_status = BT_ERROR;
#ifdef MMI_MULTI_SIM_SYS_NONE
    uint8				mt_call = CC_INVALID_INDEX;
#endif

    //check the param
    if(PNULL== msg_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallCcwaInd() msg_ptr == PNULL");
        return;
    }
    msg_body_ptr = (BT_HFU_CALLERID_PARAMS_T*)msg_ptr->body_ptr;
    if(PNULL== msg_body_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallCcwaInd() msg_body_ptr == PNULL");
        return;
    }
    if(BT_SUCCESS != msg_ptr->status)
    {
        SCI_TRACE_LOW("CC_BtCallCcwaInd status %d ", msg_ptr->status);
        return;
    }

    MMIAPICOM_GenPartyNumber(msg_body_ptr->number, strlen((char*)msg_body_ptr->number), party_num_ptr);
    call_num.number_type = party_num_ptr->number_type;
    call_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
    call_num.num_len =(party_num_ptr->num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN:party_num_ptr->num_len;
    SCI_TRACE_LOW("CC_BtCallCcwaInd: party_num_ptr->num_len =%d,call_num.num_len=%d,strlen(msg_body_ptr->number)=%d",party_num_ptr->num_len,call_num.num_len,strlen((char*)msg_body_ptr->number));
    MMI_MEMCPY(
        call_num.party_num,
        sizeof(call_num.party_num),
        party_num_ptr->bcd_num,
        sizeof(party_num_ptr->bcd_num),
        call_num.num_len
        );

    if(CC_IsExistOperatedCall()
#if defined(MMI_BLACKLIST_SUPPORT)
        || CC_MTCallFireWall(MN_DUAL_SYS_1,&call_num,0,MN_CALL_TYPE_NORMAL,MN_BEARER_CAPABILITY_LINE_1)
#endif
        )
    {
        SCI_TRACE_LOW("CC_MTCallFireWall: the call is disconnected");
        bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_RELEASE_HELD_CALLS);
        if(BT_SUCCESS !=  bt_status && BT_PENDING != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c:CC_BtCallCcwaInd() MMIAPIBT_HfuCallHold fail");
        }
        s_bt_setupind_flag = FALSE;
        MMICC_SetBtCallFlag(FALSE);
            return;          
    }
    //if there is a bt call , a bt call coming 
    if(MMICC_GetBtCallFlag())
#ifndef MMI_MULTI_SIM_SYS_NONE		
    {
        MMICL_CALL_INFO_T		cl_info = {0};
        
        SCI_TRACE_LOW("CC_BtCallCcwaInd s_call_context.call_number= %d ", s_call_context.call_number);
        //CC_BtHandleMTCallDisc(DPARAM param);
        bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_RELEASE_HELD_CALLS);
        if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
        {
            SCI_TRACE_LOW("ccapp.c:CC_BtCallCcwaInd() MMIAPIBT_HfuCallHold fail");
            return;
        }
        SCI_MEMSET(&cl_info, 0, sizeof(cl_info));
        //copy call_info.call_time
        cl_info.call_time.start_time = cl_info.call_time.stop_time
            = MMIAPICOM_GetCurTime();
        //copy call_info.call_num.is_num_present
        if(strlen((char*)msg_body_ptr->number) > 0)
        {
            cl_info.call_num.is_num_present = TRUE;
        }
        else
        {
            cl_info.call_num.is_num_present = FALSE;
        }
        cl_info.call_type = CC_CALL_NORMAL;

         //copy call_info.call_num.number
        cl_info.call_num.number.number_type = party_num_ptr->number_type;
        cl_info.call_num.number.number_plan = MN_NUM_PLAN_ISDN_TELE;
        cl_info.call_num.number.num_len = ( party_num_ptr->num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN: party_num_ptr->num_len;
        MMI_MEMCPY(
        (void*)cl_info.call_num.number.party_num,
        sizeof(cl_info.call_num.number.party_num),
        (void*)party_num_ptr->bcd_num,
        sizeof(party_num_ptr->bcd_num),
        cl_info.call_num.number.num_len
        );
        cl_info.is_bt_dail = TRUE;

        MMIAPICL_RecordCallInfo(MN_DUAL_SYS_1, MMICL_CALL_MISSED, cl_info);
        s_call_context.missed_call_num++;
        MAIN_SetMissedCallState(TRUE);
        MMIAPICC_SaveMissedCallNumToNv();
        s_is_new_missed_call = TRUE;   
        //s_bt_setupind_flag = FALSE;
        //MMICC_SetBtCallFlag(FALSE);
    }
#else
    {
        SCI_TRACE_LOW("ccapp.c:CC_BtCallCcwaInd() there is a bt call , a bt call coming ");

 //update s_call_context
    {
        //s_call_context.dual_sys = pSig->dual_sys;
        s_call_context.call_number++;
        s_call_context.incoming_call_index = s_call_context.call_number - 1;
        mt_call = s_call_context.incoming_call_index;
        s_call_context.exist_call.exist_incoming_call = TRUE;
		
        if( CC_NO_NUM == s_call_context.call_number -1 )//判断之前有没有电话
        {
            s_call_context.call_info[mt_call].call_state = CC_INCOMING_CALL_STATE;
        }
        else
        {
            s_call_context.call_info[mt_call].call_state = CC_WAITING_STATE;
        }    
        
        s_call_context.call_info[mt_call].call_type = CC_MT_CALL_TYPE;
        s_call_context.call_info[mt_call].call_time.start_time = MMIAPICOM_GetCurTime( );
        
        s_call_context.current_call = FindStateCall(CC_INCOMING_CALL_STATE);
        if (s_call_context.current_call >= s_call_context.call_number)
        {
            s_call_context.current_call = FindStateCall(CC_WAITING_STATE);
        }
        s_bt_current_call_index = s_call_context.current_call;
        
        //MO的时候MT进来，MT优先，断掉MO
        s_call_context.want_mo_call.is_want_mo_call = FALSE;
        SCI_TRACE_LOW("ccapp.c CC_BtCallCcwaInd ,s_call_context.call_info[%d].call_type=%d,",s_bt_current_call_index,s_call_context.call_info[s_bt_current_call_index].call_type);
    }
		
     // copy the number to address 
    if( 0 != strlen((char*)msg_body_ptr->number))
    {
        BOOLEAN     result = FALSE;
        MMI_STRING_T		name = {0};
        MMIPB_BCD_NUMBER_T	pb_num = {0};
        uint8				copy_len = 0;

        s_call_context.call_info[mt_call].address.num_len
            =  party_num_ptr->num_len;
        MMI_MEMCPY(
            s_call_context.call_info[mt_call].address.party_num,
            sizeof(s_call_context.call_info[mt_call].address.party_num),
            party_num_ptr->bcd_num,
            sizeof(party_num_ptr->bcd_num),
            sizeof(s_call_context.call_info[mt_call].address.party_num)
            );


        SCI_TRACE_LOW("ccapp.c, CC_BtCallCcwaInd mt_call =%d", mt_call);
        SCI_TRACE_BUF("ccapp.c, CC_BtCallCcwaInd s_call_context.call_info[mt_call].address.party_num =%x", s_call_context.call_info[mt_call].address.party_num,
			sizeof(s_call_context.call_info[mt_call].address.party_num));
		
        //根据号码搜索电话簿
        {
            SCI_MEMSET((void*)s_call_context.call_info[mt_call].name, 0, sizeof(s_call_context.call_info[mt_call].name) );
            SCI_MEMSET( &name, 0, sizeof(name) );
            name.wstr_ptr = s_call_context.call_info[mt_call].name;    
            
            //获得号码
            pb_num.npi_ton =  MMIAPICOM_ChangeTypePlanToUint8(party_num_ptr->number_type, MN_NUM_PLAN_ISDN_TELE);
            
            copy_len = ( party_num_ptr->num_len > MMIPB_BCD_NUMBER_MAX_LEN ) ? MMIPB_BCD_NUMBER_MAX_LEN : party_num_ptr->num_len;
            pb_num.number_len = copy_len;
            MMI_MEMCPY(
                pb_num.number,
                sizeof(pb_num.number),
                party_num_ptr->bcd_num,
                sizeof(party_num_ptr->bcd_num),
                copy_len
                );
            
            //在电话本中查找
            result = MMIAPIPB_GetNameByNumber(&pb_num, &name, CC_MAX_PBNAME_LEN, FALSE) ;/*lint !e506 */
            SCI_TRACE_BUF("ccapp.c, CC_BtCallCcwaInd pb_num=%x", pb_num.number, sizeof(pb_num.number));
            SCI_TRACE_LOW("ccapp.c, CC_BtCallCcwaInd search_result=%d, name.wstr_len=%d", result, name.wstr_len);
        }

        s_call_context.call_info[mt_call].is_mute = FALSE;
        s_call_context.want_mo_call.is_save_name = FALSE;
        
        //名字
        if (result && (0 != name.wstr_len))//电话本中存在且名字不为空
        {
            s_call_context.call_info[mt_call].is_name_exist = TRUE;
            
            s_call_context.call_info[mt_call].name_len = name.wstr_len ;
        }
        else
        {
            uint8	str_len = strlen((char*)msg_body_ptr->number);

            s_call_context.call_info[mt_call].is_name_exist = FALSE;
            
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
            copy_len = str_len;
#else            
            copy_len = ( str_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : str_len;/*lint !e506 */
#endif
            s_call_context.call_info[mt_call].name_len = copy_len;
            SCI_TRACE_LOW("%d,%d,%d",s_call_context.call_info[mt_call].name_len,str_len,copy_len);
            MMI_STRNTOWSTR(
                (void*)s_call_context.call_info[mt_call].name,
                s_call_context.call_info[mt_call].name_len,
                (void*)msg_body_ptr->number,
                str_len,
                copy_len
                );
        }
        }
    else
    {
        s_call_context.call_info[mt_call].name_len = 0;
    }		
	
        MMICC_UpdateCallStatusDisplay(MMICC_MT_START_IND);		
    }
#endif
}
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_SPK_VOL_IND
//	Global resource dependence : s_call_context
//  Author:kay.chen
//	Note:2014.9.3
/*****************************************************************************/
LOCAL void CC_BtCallSpkVolInd(BT_MSG_T *msg_ptr)
{
    BT_HFU_SPK_VOL_T *msg_body_ptr = PNULL;
    uint8 volume = 0;

    //check the param
    if(PNULL== msg_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallSpkVolInd() msg_ptr == PNULL");
        return;
    }
    
    msg_body_ptr = (BT_HFU_SPK_VOL_T*)(msg_ptr->body_ptr);

    if(PNULL== msg_body_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallSpkVolInd() msg_body_ptr == PNULL");
        return;
    }

    volume = msg_body_ptr->gain;

    SCI_TRACE_LOW("ccapp.c: CC_BtCallSpkVolInd() volume:%u",volume);
    MMIAPICC_SetCallVolume(volume);
}
/*****************************************************************************/
// 	Description : process when received signal setup=0
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:2012.06.06
/*****************************************************************************/
LOCAL void CC_SetupComplete(void)
{
    BT_STATUS  bt_status = BT_SUCCESS;  
 
    //update s_call_context
    if(s_bt_current_call_index < s_call_context.call_number)
    {
        s_call_context.current_call = s_bt_current_call_index;
    }
    else
    {
        if(s_call_context.call_number)//cr224224
        {
            s_call_context.current_call = s_call_context.call_number - 1;
        }
        else
        {
            s_call_context.current_call = 0;
        }
        s_bt_current_call_index = s_call_context.current_call;
    }
    if(s_call_context.exist_call.exist_incoming_call)
    {
        s_bt_setupind_flag = FALSE;
       //s_call_context.current_call = FindStateCall(CC_INCOMING_CALL_STATE) ;
       s_call_context.exist_call.exist_incoming_call = FALSE;
       s_call_context.call_info[s_call_context.current_call].call_type = CC_MT_CALL_TYPE;

    }
    else if(s_call_context.exist_call.exist_outgoing_call)
    {
      // s_call_context.current_call = FindStateCall(CC_CALLING_STATE) ;
       s_call_context.exist_call.exist_outgoing_call = FALSE;
       SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call));
        s_call_context.want_mo_call.is_want_mo_call = FALSE;
       s_call_context.call_info[s_call_context.current_call].call_type = CC_MO_CALL_TYPE;
       if(1 == s_call_context.call_number)
       {
           bt_status = MMIAPIBT_HfuListCurrentCalls();
            if(BT_PENDING != bt_status && BT_SUCCESS != bt_status)
            {   
                SCI_TRACE_LOW("ccapp.c: CC_BtCallStartInd() MMIAPIBT_HfuListCurrentCalls fail bt_status= %d",bt_status);
            }
        }
    }
    s_call_context.exist_call.exist_active_call_num++;
    s_call_context.call_info[s_call_context.current_call].call_state = CC_CALL_STATE;
    s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_NO_OPER;
    s_call_context.call_info[s_call_context.current_call].call_time.start_time = MMIAPICOM_GetCurTime( );
    s_call_context.call_info[s_call_context.current_call].call_time.stop_time
        = s_call_context.call_info[s_call_context.current_call].call_time.start_time;

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
    MMIAPIBT_SetCallHeldStatus();
#endif

    
    //reset call time if only one call
    if (1 == s_call_context.call_number) 
    {
        s_call_context.call_time_count = 0;
    }    
    s_call_context.call_info[s_call_context.current_call].start_call_time_count = s_call_context.call_time_count;
    
    // 处理当前的来电为正常来电，而非做答录处理时，才建立起通话的画面

    if(CC_MT_CALL_TYPE == s_call_context.call_info[s_call_context.current_call].call_type)
    {
        MMICC_UpdateCallStatusDisplay(MMICC_MT_CONNECTED_IND);
        CC_OpenAudioTunnel(TRUE);
    }
    else if(CC_MO_CALL_TYPE == s_call_context.call_info[s_call_context.current_call].call_type)
    {
         //vibrate for call connect prompt
        MMICC_VibrateForConnectPrompt();
        EnableSpeakerMode(TRUE);
        MMICC_UpdateCallStatusDisplay(MMICC_MO_CONNECTED_IND);
        //MMIICC_StopDtmfTimerId();
    
    }
        
    MMICC_EnableMute(MMICC_IsCurrentCallMute());
    SCI_TRACE_LOW("ccapp.c CC_SetupComplete,s_call_context.exist_active_call_num=%d,s_call_context.call_number=%d, s_call_context.current_call=%d, s_bt_current_call_index=%d",s_call_context.exist_call.exist_active_call_num,s_call_context.call_number,
        s_call_context.current_call,s_bt_current_call_index);
    
}

/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CLCC_RECORD_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallClccRecordInd(BT_MSG_T *msg_ptr)
{
    BT_HFU_CLCC_T  *msg_body_ptr = PNULL;
    uint8				copy_len = 0;
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd() enter current_call:%d", s_call_context.current_call);
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd() enter s_call_context.call_info[0].call_state:%d", s_call_context.call_info[0].call_state);
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd() enter s_call_context.call_info[1].call_state:%d", s_call_context.call_info[1].call_state);

    //check the param
    if(PNULL== msg_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd() msg_ptr == PNULL");
        return;
    }
    msg_body_ptr = (BT_HFU_CLCC_T*)(msg_ptr->body_ptr);
    if(PNULL== msg_body_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd() msg_body_ptr == PNULL");
        return;
    }
    if(BT_SUCCESS != msg_ptr->status)
    {
        SCI_TRACE_LOW("CC_BtCallClccRecordInd status %d ", msg_ptr->status);
        return;
    }

    //Bug 837505 检查号码中是否包含特殊字符，若有，则需要剔除，否则导致显示有问题
    if((NULL != msg_body_ptr->number) && (0 != strlen((char*)msg_body_ptr->number)))
    {
        int16           tele_len = 0;
        uint32          i = 0;
        
        tele_len = strlen((char*)msg_body_ptr->number);
        if(!MMIAPICOM_IsValidNumberString(msg_body_ptr->number,tele_len))
        {
            uint32 temp_len = 0;
            uint8*  temp_tele_num = PNULL;
            temp_tele_num = (uint8*)SCI_ALLOC_APP(tele_len + 1);
            SCI_MEMSET(temp_tele_num, 0, (tele_len + 1));
            for(i=0;i<tele_len;i++)
            {
                if(MMIAPICOM_IsTelephoneNum(msg_body_ptr->number[i]))
                {
                    temp_tele_num[temp_len]= msg_body_ptr->number[i];
                    temp_len++;
                }
            }
            SCI_MEMSET(msg_body_ptr->number, 0, tele_len);
            MMI_MEMCPY(msg_body_ptr->number, tele_len,temp_tele_num,temp_len,temp_len);
            tele_len = temp_len;
            SCI_FREE(temp_tele_num);
            SCI_TRACE_LOW("cc_app.c:CC_BtCallClccRecordInd tele num have special character!!!,tele_len:%d,strlen:%d\n",tele_len,strlen((char*)msg_body_ptr->number));
        }
    }
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd state = %d", msg_body_ptr->state);
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd index = %d", msg_body_ptr->index);		
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd tele_len = %d", strlen((char*)msg_body_ptr->number));		
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd  num %s.",(char*)msg_body_ptr->number);

    if ((HFU_CALL_STATUS_ACTIVE == msg_body_ptr->state))
    {
        {
	        if(FALSE == s_bt_sco_link_is_exist)
        {
            uint32 time_out = MMI_SEND_SCO_REQUEST_TIMEOUT;

            SCI_TRACE_LOW("ccapp.c: CreateTimer s_sco_request_timer");

            s_sco_request_timer = MMK_CreateTimerCallback(time_out, MMICC_HandleScoTimer, PNULL, FALSE);
        }
		 SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd  sco link doesn't exist !!!! , set timer to enable audio !!!");					
	 }
    }

    s_bt_clcc_record_count = msg_body_ptr->index;	
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd s_bt_clcc_record_count = %d, msg_body_ptr->index=%d, s_call_context.call_number=%d", 
		s_bt_clcc_record_count, msg_body_ptr->index,s_call_context.call_number);
//#ifndef MMI_MULTI_SIM_SYS_NONE
#if !defined(MMI_MULTI_SIM_SYS_NONE) && !defined(BT_DIALER_SUPPORT)//bug924697
    if(s_bt_clcc_record_count < 2)  // for ps version , only one bt call is supported
#endif		
    {
#if !defined(MMI_MULTI_SIM_SYS_NONE) && !defined(BT_DIALER_SUPPORT)//bug924697
        if(s_bt_current_call_index < s_call_context.call_number)
        {
            s_call_context.current_call = s_bt_current_call_index;
        }
        else
        {
            if(s_call_context.call_number)
            {
                s_call_context.current_call = s_call_context.call_number - 1;
            }
            else
            {
                s_call_context.current_call = 0;
            }
            s_bt_current_call_index = s_call_context.current_call;
        }
#else              
        s_bt_current_call_index = msg_body_ptr->index - 1;
        s_call_context.current_call = s_bt_current_call_index;// = FindStateCall(CC_CALL_STATE);
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd() enter s_call_context.call_info[%d].call_state:%d", s_call_context.current_call,s_call_context.call_info[s_call_context.current_call].call_state);
#endif
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd s_bt_current_call_index=%d, s_call_context.current_call=%d, call_number=%d, s_bt_clcc_record_count=%d", 
		s_bt_current_call_index, s_call_context.current_call, s_call_context.call_number, s_bt_clcc_record_count);

	switch (s_call_context.call_info[s_call_context.current_call].call_state)
	{
		SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd s_call_context.call_info[%d].call_state=%d", s_call_context.current_call, s_call_context.call_info[s_call_context.current_call].call_state); 
		case  CC_NULL_STATE :
		{
			CC_WANT_MO_CALL_T want_mo_call = {0};
			switch (msg_body_ptr->state)
			{
				case HFU_CALL_STATUS_ACTIVE:
				case HFU_CALL_STATUS_HELD:
				{
		                s_call_context.call_number++;
		                s_call_context.current_call = s_call_context.call_number - 1;
		                //s_call_context.call_info[s_call_context.current_call].call_state = CC_CALL_STATE; //when record ind comes,set call_state
		                s_bt_current_call_index = s_call_context.current_call;
		                s_call_context.exist_call.exist_active_call_num++;
		                s_call_context.call_info[s_call_context.current_call].call_state = CC_CALL_STATE;                
		                s_call_context.call_info[ s_call_context.current_call ].operated_by_user = CC_NO_OPER;
		                s_call_context.call_info[s_call_context.current_call].call_time.start_time = MMIAPICOM_GetCurTime( );
		                s_call_context.call_info[s_call_context.current_call].call_time.stop_time
		                = s_call_context.call_info[s_call_context.current_call].call_time.start_time;
		            
		                //reset call time if only one call
		                if (1 == s_call_context.call_number) 
		                {
		                    s_call_context.call_time_count = 0;
		                }    
		                s_call_context.call_info[s_call_context.current_call].start_call_time_count = s_call_context.call_time_count;  

		                MMICC_EnableMute(MMICC_IsCurrentCallMute());
 				  s_call_context.call_info[s_call_context.current_call].call_type = (0 == msg_body_ptr->dir) ? CC_MO_CALL_TYPE : CC_MT_CALL_TYPE;
#ifdef MMI_DUALMODE_ENABLE  //g应用中 CC_MAX_PBNAME_LEN大于255//bug924697,第二通电话号码保存到全局变量中
			            copy_len = s_call_context.want_mo_call.name_len;
#else    
			            copy_len = ( s_call_context.want_mo_call.name_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : s_call_context.want_mo_call.name_len;/*lint !e506 */
#endif
			            MMI_WSTRNCPY(
			                s_call_context.call_info[s_call_context.current_call].name,
			                sizeof(s_call_context.call_info[s_call_context.current_call].name),
			                s_call_context.want_mo_call.name,
			                sizeof(s_call_context.want_mo_call.name),
			                copy_len
			                );
			            s_call_context.call_info[s_call_context.current_call].name_len = copy_len;
			            s_call_context.call_info[s_call_context.current_call].address = s_call_context.want_mo_call.address;
			            s_call_context.call_info[s_call_context.current_call].is_save_name = s_call_context.want_mo_call.is_save_name;
			            s_call_context.call_info[s_call_context.current_call].is_name_exist = s_call_context.want_mo_call.is_name_exist; 
		              }
				break;
				case HFU_CALL_STATUS_INCOMING:
				case HFU_CALL_STATUS_WAITING:
				{
			                MN_CALLING_NUMBER_T call_num ={0};
			                MMI_PARTY_NUMBER_T  party_num= {0};
			                MMI_PARTY_NUMBER_T * party_num_ptr= &party_num;
			                BT_STATUS bt_status = BT_ERROR;
			                
			                MMIAPICOM_GenPartyNumber(msg_body_ptr->number, strlen((char*)msg_body_ptr->number), party_num_ptr);
			                call_num.number_type = party_num_ptr->number_type;
			                call_num.number_plan = MN_NUM_PLAN_ISDN_TELE;
			                call_num.num_len =(party_num_ptr->num_len > MN_MAX_ADDR_BCD_LEN) ? MN_MAX_ADDR_BCD_LEN:party_num_ptr->num_len;
			                SCI_TRACE_LOW("CC_BtCallClccRecordInd: party_num_ptr->num_len =%d,call_num.num_len=%d,strlen(msg_body_ptr->number)=%d",party_num_ptr->num_len,call_num.num_len,strlen((char*)msg_body_ptr->number));
			                MMI_MEMCPY(
			                    call_num.party_num,
			                    sizeof(call_num.party_num),
			                    party_num_ptr->bcd_num,
			                    sizeof(party_num_ptr->bcd_num),
			                    call_num.num_len
			                    );

			                if(CC_IsExistOperatedCall()
#if defined(MMI_BLACKLIST_SUPPORT)
			                    || CC_MTCallFireWall(MN_DUAL_SYS_1,&call_num,0,MN_CALL_TYPE_NORMAL,MN_BEARER_CAPABILITY_LINE_1)
#endif
			                    )
			                {
			                    SCI_TRACE_LOW("CC_MTCallFireWall: the call is disconnected");
			                    bt_status = MMIAPIBT_HfuHangup();
			                    if(BT_SUCCESS !=  bt_status&& BT_PENDING != bt_status)
			                    {
			                        SCI_TRACE_LOW("ccapp.c:CC_BtCallClccRecordInd() MMIAPIBT_HfuHangup fail");
			                    }
			                        return;          
			                }

			                s_bt_setupind_flag = TRUE;
			                s_call_context.call_number++;
			                s_call_context.incoming_call_index = s_call_context.call_number - 1;
			                s_call_context.current_call = s_call_context.incoming_call_index;
			                s_call_context.exist_call.exist_incoming_call = TRUE;
			                s_call_context.call_info[s_call_context.current_call].call_state = CC_INCOMING_CALL_STATE;        
			                s_call_context.call_info[s_call_context.current_call].call_type = CC_MT_CALL_TYPE;
			                s_call_context.call_info[s_call_context.current_call].call_time.start_time = MMIAPICOM_GetCurTime( );       
			                s_bt_current_call_index = s_call_context.current_call;
			                
			                //MO的时候MT进来，MT优先，断掉MO
			                s_call_context.want_mo_call.is_want_mo_call = FALSE;
			                SCI_TRACE_LOW("ccapp.c CC_BtCallClccRecordInd,s_call_context.call_info[%d].call_type=%d,",s_bt_current_call_index,s_call_context.call_info[s_bt_current_call_index].call_type);
			            }
				break;
 			       case HFU_CALL_STATUS_DIALING:
				case HFU_CALL_STATUS_ALERTING:
				{
			            SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call)); 
			            s_call_context.want_mo_call.call_type = CC_MO_CALL_TYPE;
			            if (!GetWantMoCallInfo(MN_DUAL_SYS_1,
			                                    msg_body_ptr->number,
			                                    strlen((char*)msg_body_ptr->number),
			                                    PNULL,
			                                    PNULL,
			                                    PNULL,
			                                    &s_call_context.want_mo_call
			                                    ))
			            {
			                SCI_TRACE_LOW("mmiccapp:CC_BtCallClccRecordInd s_call_context.want_mo_call.address.num_len = %d, tele_num = %s, tele_len = %d", s_call_context.want_mo_call.address.num_len, msg_body_ptr->number, strlen((char*)msg_body_ptr->number));
			                return;
			            }
#ifdef MMI_DUALMODE_ENABLE  //g应用中 CC_MAX_PBNAME_LEN大于255//bug924697
			            copy_len = s_call_context.want_mo_call.name_len;
#else    					
			            copy_len = (want_mo_call.name_len > CC_MAX_PBNAME_LEN) ? CC_MAX_PBNAME_LEN : want_mo_call.name_len;/*lint !e506 */
#endif
			            s_call_context.call_info[s_call_context.current_call].is_name_exist = want_mo_call.is_name_exist;

			            MMI_WSTRNCPY(
			                s_call_context.call_info[s_call_context.current_call].name,
			                sizeof(s_call_context.call_info[s_call_context.current_call].name),
			                want_mo_call.name,
			                sizeof(want_mo_call.name),
			                copy_len
			                );
			            s_call_context.want_mo_call.is_want_mo_call= TRUE;
			            CC_BtCallStartInd();
			            s_call_context.want_mo_call.is_want_mo_call = FALSE;
				}
				break;
				default:
				break;
				}

				if( (HFU_CALL_STATUS_DIALING != msg_body_ptr->state) && (HFU_CALL_STATUS_ALERTING != msg_body_ptr->state))
				{
			            GetWantMoCallInfo(MN_DUAL_SYS_1,
			                                                msg_body_ptr->number,
			                                                strlen((char*)msg_body_ptr->number),
			                                                PNULL,
			                                                PNULL,
			                                                PNULL,
			                                                &want_mo_call
			                                                );

			            copy_len = (want_mo_call.name_len > CC_MAX_PBNAME_LEN) ? CC_MAX_PBNAME_LEN : want_mo_call.name_len;/*lint !e506 */

			            s_call_context.call_info[s_call_context.current_call].is_name_exist = want_mo_call.is_name_exist;

			            MMI_WSTRNCPY(
			                s_call_context.call_info[s_call_context.current_call].name,
			                sizeof(s_call_context.call_info[s_call_context.current_call].name),
			                want_mo_call.name,
			                sizeof(want_mo_call.name),
			                copy_len
			                );

			            s_call_context.call_info[s_call_context.current_call].name_len = copy_len;
			            s_call_context.call_info[s_call_context.current_call].address = want_mo_call.address;  
				}
		              SCI_TRACE_LOW("ccapp.c CC_BtCallClccRecordInd,s_call_context.exist_active_call_num=%d,s_call_context.call_number=%d",s_call_context.exist_call.exist_active_call_num,s_call_context.call_number);				
	        	}			
			break;
		case  CC_CALLING_STATE:
			{
				SCI_TRACE_LOW("ccapp.c CC_BtCallClccRecordInd CC_CALLING_STATE msg_body_ptr->state = %d, s_call_context.want_mo_call.is_want_mo_call = %d", msg_body_ptr->state, s_call_context.want_mo_call.is_want_mo_call);
				if((HFU_CALL_STATUS_DIALING == msg_body_ptr->state || HFU_CALL_STATUS_ALERTING == msg_body_ptr->state) && (!s_call_context.want_mo_call.is_want_mo_call ))
				{
			            SCI_MEMSET(((char*)&s_call_context.want_mo_call), 0, sizeof(s_call_context.want_mo_call)); 
			            s_call_context.want_mo_call.call_type = CC_MO_CALL_TYPE;
			            if (!GetWantMoCallInfo(MN_DUAL_SYS_1,
			                                    msg_body_ptr->number,
			                                    strlen((char*)msg_body_ptr->number),
			                                    PNULL,
			                                    PNULL,
			                                    PNULL,
			                                    &s_call_context.want_mo_call
			                                    ))
			            {
			                SCI_TRACE_LOW("mmiccapp:CC_BtCallClccRecordInd s_call_context.want_mo_call.address.num_len = %d, tele_num = %s, tele_len = %d", s_call_context.want_mo_call.address.num_len, msg_body_ptr->number, strlen((char*)msg_body_ptr->number));
			                return;
			            }
			            
#ifdef MMI_DUALMODE_ENABLE  //3//g应用中 CC_MAX_PBNAME_LEN大于255
			            copy_len = s_call_context.want_mo_call.name_len;
#else    
			            copy_len = ( s_call_context.want_mo_call.name_len > CC_MAX_PBNAME_LEN ) ? CC_MAX_PBNAME_LEN : s_call_context.want_mo_call.name_len;/*lint !e506 */
#endif
			            MMI_WSTRNCPY(
			                s_call_context.call_info[s_call_context.current_call].name,
			                sizeof(s_call_context.call_info[s_call_context.current_call].name),
			                s_call_context.want_mo_call.name,
			                sizeof(s_call_context.want_mo_call.name),
			                copy_len
			                );
			            s_call_context.call_info[s_call_context.current_call].name_len = copy_len;
			            s_call_context.call_info[s_call_context.current_call].address = s_call_context.want_mo_call.address;
			            s_call_context.call_info[s_call_context.current_call].is_save_name = s_call_context.want_mo_call.is_save_name;
			            s_call_context.call_info[s_call_context.current_call].is_name_exist = s_call_context.want_mo_call.is_name_exist; 
			        }
			}
			break;
		case  CC_CALL_STATE: 
			{
		            MMI_PARTY_NUMBER_T party_num = {0};
		            //convert str to BCD
		            switch(msg_body_ptr->state)
		            {
				case HFU_CALL_STATUS_ACTIVE : //means same state 
				{
				     if (!MMIAPICOM_GenPartyNumber(msg_body_ptr->number,strlen((char*)msg_body_ptr->number), &party_num))
			            {
			                SCI_TRACE_LOW("ccapp.c:CC_BtCallClccRecordInd() CC_GenPartyNumber return failure");
			                return;
			            }
			            SCI_TRACE_LOW("ccapp.c:CC_BtCallClccRecordInd() 3--num_len=%d, %d", party_num.num_len, s_call_context.call_info[s_call_context.current_call].address.num_len);
			            SCI_TRACE_LOW("ccapp.c:CC_BtCallClccRecordInd() 3--number_type=%d, %d", party_num.number_type, s_call_context.call_info[s_call_context.current_call].address.number_type);
			            if(party_num.num_len != s_call_context.call_info[s_call_context.current_call].address.num_len
			                || party_num.number_type != s_call_context.call_info[s_call_context.current_call].address.number_type
			                || 0 != memcmp(party_num.bcd_num, (uint8 *)s_call_context.call_info[s_call_context.current_call].address.party_num, party_num.num_len))
			            {
			                CC_WANT_MO_CALL_T want_mo_call = {0};
			                //保存原始号码
			                s_call_context.call_info[s_call_context.current_call].is_call_modified_store = TRUE;
			                SCI_MEMSET(&s_call_context.call_info[s_call_context.current_call].call_modified_address, 0x00, sizeof(s_call_context.call_info[s_call_context.current_call].call_modified_address));

			                SCI_MEMCPY(&s_call_context.call_info[s_call_context.current_call].call_modified_address,
			                                    &s_call_context.call_info[s_call_context.current_call].address,
			                                    sizeof(s_call_context.call_info[s_call_context.current_call].call_modified_address));
			                GetWantMoCallInfo(MN_DUAL_SYS_1,
			                                            msg_body_ptr->number,
			                                            strlen((char*)msg_body_ptr->number),
			                                            PNULL,
			                                            PNULL,
			                                            PNULL,
			                                            &want_mo_call
			                                            );

			                copy_len = (want_mo_call.name_len > CC_MAX_PBNAME_LEN) ? CC_MAX_PBNAME_LEN : want_mo_call.name_len;/*lint !e506 */

			                s_call_context.call_info[s_call_context.current_call].is_name_exist = want_mo_call.is_name_exist;

			                MMI_WSTRNCPY(
			                    s_call_context.call_info[s_call_context.current_call].name,
			                    sizeof(s_call_context.call_info[s_call_context.current_call].name),
			                    want_mo_call.name,
			                    sizeof(want_mo_call.name),
			                    copy_len
			                    );

			                s_call_context.call_info[s_call_context.current_call].name_len = copy_len;
			                s_call_context.call_info[s_call_context.current_call].address = want_mo_call.address;
			            }
			  	}
				break;
				case HFU_CALL_STATUS_HELD:// 第一通或第二通电话由接通状态变为hold 状态//bug924697
				{
                                SCI_TRACE_LOW("ccapp.c:CC_BtCallClccRecordInd (3,1) call_info[%d].call_state=%d", s_call_context.current_call, s_call_context.call_info[s_call_context.current_call].call_state);
                                if(CC_HOLD_STATE != s_call_context.call_info[s_call_context.current_call].call_state)
                                {
                                    s_call_context.call_info[s_call_context.current_call].call_state = CC_HOLD_STATE;
                                    s_call_context.exist_call.exist_active_call_num--;
                                    s_call_context.exist_call.exist_hold_call_num++;
                                }
                                SCI_TRACE_LOW("ccapp.c:CC_BtCallClccRecordInd (3,1) exist_active_call_num=%d, exist_hold_call_num=%d", s_call_context.exist_call.exist_active_call_num, s_call_context.exist_call.exist_hold_call_num);
				}
				break;
				default:
				break;				
			     }		            		            
		        }			
			break;
			case  CC_HOLD_STATE: //bug924697
			{
				switch(msg_body_ptr->state)
				{
					case HFU_CALL_STATUS_ACTIVE:// 第一通或第二通电话由hold 状态变为接通状态
					{
						if(CC_CALL_STATE != s_call_context.call_info[s_call_context.current_call].call_state)
						{
							s_call_context.call_info[s_call_context.current_call].call_state = CC_CALL_STATE;
							s_call_context.exist_call.exist_active_call_num++;
							s_call_context.exist_call.exist_hold_call_num--;
							SCI_TRACE_LOW("ccapp.c:CC_BtCallClccRecordInd (5,0) s_bt_clcc_record_count=%d, s_call_context.call_number=%d",s_bt_clcc_record_count, s_call_context.call_number);
							SCI_TRACE_LOW("ccapp.c:CC_BtCallClccRecordInd (5,0) exist_active_call_num=%d, exist_hold_call_num=%d", s_call_context.exist_call.exist_active_call_num, s_call_context.exist_call.exist_hold_call_num);
						}
					}
					break;
				default:
				break;
			     }
		        }
			break;
				default:
				break;
				}


    }
    SCI_TRACE_LOW("ccapp.c CC_BtCallClccRecordInd,s_call_context.exist_active_call_num=%d,s_call_context.call_number=%d, s_call_context.current_call=%d, s_bt_current_call_index=%d",s_call_context.exist_call.exist_active_call_num,s_call_context.call_number,
        s_call_context.current_call,s_bt_current_call_index);
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccRecordInd() leave s_call_context.call_info[%d].call_state:%d", s_call_context.current_call,s_call_context.call_info[s_call_context.current_call].call_state);        
    
}
/*****************************************************************************/
// 	Description : process when received signal APP_BT_HFU_CLCC_RESULT_IND
//	Global resource dependence : s_call_context
//  Author:chunjie.liu
//	Note:2013.6.5
/*****************************************************************************/
LOCAL void CC_BtCallClccResultInd(BT_MSG_T *msg_ptr)
{
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd() enter");
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd() ind_type=%d, ind_value=%d", s_bt_ciev_info.ind_type, s_bt_ciev_info.ind_value);
    //check hfu connection
    if(FALSE == MMICC_GetBtCallFlag())
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd() no hfu connection");
        return;
    }

    //check the param
    if(PNULL== msg_ptr)
    {
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd() msg_ptr == PNULL");
        return;
    }

    if(BT_SUCCESS != msg_ptr->status)
    {
        SCI_TRACE_LOW("CC_BtCallClccResultInd status %d ", msg_ptr->status);
        return;
    }
#if 0    //bug924697    
     if(s_bt_current_call_index < s_call_context.call_number)
    {
        s_call_context.current_call = s_bt_current_call_index;
    }
    else
#endif
    {
        if(s_call_context.call_number)
        {
            s_call_context.current_call = s_call_context.call_number - 1;
        }
        else
        {
            s_call_context.current_call = 0;
        }
        s_bt_current_call_index = s_call_context.current_call;
    }
    if(0 == s_call_context.current_call)//第一通电话为hold, 第二通电话为active
    {
         if((CC_CALL_STATE == s_call_context.call_info[s_call_context.current_call+1].call_state)
		&& (CC_HOLD_STATE == s_call_context.call_info[s_call_context.current_call].call_state))
         {
             s_call_context.current_call = s_bt_current_call_index = 1;
         }
    }
    else if(1 == s_call_context.current_call)//第二通电话为hold, 第一通电话为active
    {
         if((CC_CALL_STATE == s_call_context.call_info[s_call_context.current_call-1].call_state)
		&& (CC_HOLD_STATE == s_call_context.call_info[s_call_context.current_call].call_state))
         {
             s_call_context.current_call = s_bt_current_call_index = 0;
         }
    }
    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd() current_call=%d, s_bt_current_call_index=%d, call_number=%d, call_state=%d", s_call_context.current_call, s_bt_current_call_index, s_call_context.call_number, s_call_context.call_info[s_call_context.current_call].call_state);

    switch(s_bt_ciev_info.ind_type)
    {
	case BT_CALL_HELD_INDICATOR:
	{
		if(BT_HFP_IND_CALLHELD_NO_CALL_HELD == s_bt_ciev_info.ind_value)
		{
#if defined(MMI_MULTI_SIM_SYS_NONE) || defined(BT_DIALER_SUPPORT)//bug924697
            SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd ciev (7 , 0) !!!");
            SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd ciev (7 , 0) s_bt_clcc_record_count=%d, exist_hold_call_num=%d, exist_active_call_num=%d", s_bt_clcc_record_count, s_call_context.exist_call.exist_hold_call_num, s_call_context.exist_call.exist_active_call_num);
            SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd ciev (7 , 0) s_call_context.call_info[0].call_state=%d", s_call_context.call_info[0].call_state);
            //case 1 : call merge
            //case 2 : 1 active 1 hold , but hold is disconnected
            //case 3:  1 hold call is changed to active
            //case 4: only 1 hold call , and disconnected
            if((1 < s_bt_clcc_record_count) && (0 == s_call_context.exist_call.exist_hold_call_num)) //call merge, 接通第二通 电话后第一通电话主动挂断不应进入此case//bug924697
            {
                int i = 0;
                SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd ciev (7 , 0)-1 s_bt_clcc_record_count=%d, s_call_context.call_number=%d", s_bt_clcc_record_count, s_call_context.call_number);
                for( i = 0; i < s_call_context.call_number ; i++ )
                {
                    if( CC_WAITING_STATE != s_call_context.call_info[ i ].call_state )
                    {
                        s_call_context.call_info[ i ].is_in_mpty = TRUE;
                        SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd ciev (7 , 0)-2 s_call_context.call_info[%d].call_state=%d", i, s_call_context.call_info[i].call_state);                        
                        //modify the call state in the s_call_context
                        if( CC_HOLD_STATE == s_call_context.call_info[i].call_state)
                        {
                            s_call_context.call_info[i].call_state = CC_CALL_STATE;
                            s_call_context.exist_call.exist_active_call_num++;
                            s_call_context.exist_call.exist_hold_call_num--;
                        }
                    }
                }
                MMICC_UpdateCallStatusDisplay(MMICC_BUILD_MPTY_SUC_IND);				
            }
            else if(0 != s_call_context.exist_call.exist_hold_call_num)
            {
                uint8 active_index = FindStateCall(CC_CALL_STATE);
                uint8 hold_index = FindStateCall(CC_HOLD_STATE);
                SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd ciev (7 , 0)-3 active_index=%d, hold_index=%d", active_index, hold_index);
                if ((active_index >= s_call_context.call_number) || (hold_index >= s_call_context.call_number) )
                {
                    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd BT_HFP_IND_CALL_NO_CALL_ACTIVE 7,0 invalid hold_index !!!");
                    return;
                }
                s_bt_current_call_index = active_index;
                CC_BtDisconnectCallByIndex(hold_index);
            }
            SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd ciev (7 , 0)-4 s_bt_clcc_record_count=%d, s_call_context.call_number=%d", s_bt_clcc_record_count, s_call_context.call_number);
            //check call number is same as before or not 
            if(s_call_context.call_number != s_bt_clcc_record_count)
            {
                SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd call number changed from %d to %d !!" ,s_call_context.call_number, s_bt_clcc_record_count);		
                //s_call_context.call_number = s_bt_clcc_record_count;//bug924697
            }
            else if(0 == s_bt_clcc_record_count) //case 4: only 1 hold call , and disconnected
            {
                SCI_TRACE_LOW("ccapp.c:  hold call is disconnected !!!");
                s_call_context.current_call = s_bt_current_call_index = CC_INVALID_INDEX;
            }
            MMIICC_StarCallTimingTimer();//930459 start timer to display call time		
            MMICC_UpdateCurrentCallStatus(TRUE);//update window bug924697
#endif
		}
		else if(BT_HFP_IND_CALLHELD_ONE_HOLD_ONE_ACTIVE == s_bt_ciev_info.ind_value)//第二通电话拨通后设置通话状态为连接//bug924697
		{
                    uint8 display_type = (MMICC_GetAppletInstance()->cc_instance).cc_display_type;
                    if(display_type != CC_DISPLAY_HOT_BUTTONS)
                        (MMICC_GetAppletInstance()->cc_instance).cc_display_type = CC_DISPLAY_HOT_BUTTONS;
                    SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd ciev (7 , 1) s_call_context.current_call=%d", s_call_context.current_call);
                    MMICC_UpdateCurrentCallStatus(TRUE);//update window bug924697
		}
	}
	break;
	default:
	{
    if((CC_CALLING_STATE == s_call_context.call_info[s_call_context.current_call].call_state) && !s_call_context.want_mo_call.is_want_mo_call)
    {
        MMICC_UpdateCallStatusDisplay(MMICC_MO_START_IND);
        s_call_context.want_mo_call.is_want_mo_call = TRUE;
    }
    else if(CC_INCOMING_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state)
    {
        MMICC_UpdateCallStatusDisplay(MMICC_MT_START_IND);
    }
    else if((BT_CALL_SETUP_STATUS_INDICATOR == s_bt_ciev_info.ind_type) 
        && (BT_HFP_IND_CALLSETUP_REMOTE_ALERTED == s_bt_ciev_info.ind_value))//拨打第二通电话后未接通之前设置通话状态为拨出//bug924697
    {
        uint8 display_type = (MMICC_GetAppletInstance()->cc_instance).cc_display_type;
        SCI_TRACE_LOW("ccapp.c: CC_BtCallClccResultInd() 3, 3"); 
        if(display_type != CC_DISPLAY_HOT_BUTTONS)
            (MMICC_GetAppletInstance()->cc_instance).cc_display_type = CC_DISPLAY_HOT_BUTTONS;
        MMICC_UpdateCallStatusDisplay(MMICC_MO_START_IND);
    }
    else if((CC_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state) 
        && !MMK_IsOpenWin( MMICC_STATUS_WIN_ID ))
    {
        uint8 display_type = (MMICC_GetAppletInstance()->cc_instance).cc_display_type;
        if(display_type != CC_DISPLAY_HOT_BUTTONS)
		(MMICC_GetAppletInstance()->cc_instance).cc_display_type = CC_DISPLAY_HOT_BUTTONS;
		
        MMICC_UpdateCallStatusDisplay(MMICC_MO_CONNECTED_IND);
    }
    else if((CC_CALL_STATE == s_call_context.call_info[s_call_context.current_call].call_state) 
        && s_call_context.call_info[s_call_context.current_call].is_call_modified_store)
    {
        uint8 display_type = (MMICC_GetAppletInstance()->cc_instance).cc_display_type;
        if(display_type != CC_DISPLAY_HOT_BUTTONS)
		(MMICC_GetAppletInstance()->cc_instance).cc_display_type = CC_DISPLAY_HOT_BUTTONS;
		
        MMICC_UpdateCallStatusDisplay(MMICC_MO_CONNECTED_IND);
    }
	}
	break;
    }	

    SCI_TRACE_LOW("ccapp.c CC_BtCallClccResultInd,s_call_context.exist_active_call_num=%d,s_call_context.call_number=%d, s_call_context.current_call=%d, s_bt_current_call_index=%d, status:%d , CC_INSTANCE.cc_display_type %d",s_call_context.exist_call.exist_active_call_num,s_call_context.call_number,
        s_call_context.current_call,s_bt_current_call_index, s_call_context.call_info[s_call_context.current_call].call_state , (MMICC_GetAppletInstance()->cc_instance).cc_display_type);
}

/*****************************************************************************/
// 	Description : process when received signal ciev call == 0
//	Global resource dependence : 
//  Author:chunjie liu
//	Note:2013.06.08
/*****************************************************************************/
LOCAL void CC_BtDisconnectedCall(void)
{
    MMICL_CALL_INFO_T		cl_info = {0};
    uint8					call_index = CC_INVALID_INDEX;
    uint32 call_time = 0;
    uint8			        tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8			        tel_num_len = 0; 
    MMICL_CALL_TYPE_E calllog_type = MMICL_CALL_DIALED;

    SCI_TRACE_LOW("[MMICC] CC_BtDisconnectedCall");
    //update s_call_context  cr222299
    if(s_bt_current_call_index < s_call_context.call_number)
    {
        s_call_context.current_call = s_bt_current_call_index;
    }
    else
    {
        if(s_call_context.call_number)//cr224224
        {
            s_call_context.current_call = s_call_context.call_number - 1;
        }
        else
        {
            s_call_context.current_call = 0;
        }
    }
    //cr237299
    s_bt_current_call_index = CC_INVALID_INDEX;
   
    call_index = s_call_context.current_call;
    SCI_TRACE_LOW("ccapp.c CC_BtDisconnectedCall  BT_HFP_IND_CALLSETUP_NOT_IN_CALLSETUP,s_call_context.exist_active_call_num=%d,s_call_context.call_number=%d, s_call_context.current_call=%d, s_bt_current_call_index=%d,s_call_context.call_info[call_index].call_state:%d",s_call_context.exist_call.exist_active_call_num,s_call_context.call_number,
        s_call_context.current_call,s_bt_current_call_index,s_call_context.call_info[call_index].call_state);
    if(!CC_IsCallIDValid(call_index))
    {
        SCI_TRACE_LOW("ccapp.c ERROR:CC_BtDisconnectedCall() current_call invalid %d",call_index);
        MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
        return;
    } 

    //SPCSS00056682
    s_bt_setupind_flag = FALSE;
    
     if (s_call_context.call_info[call_index].is_mtfw_reject)
    {
        SCI_TRACE_LOW("ccapp.c ERROR:CC_BtDisconnectedCall() s_call_context.call_info[call_index].is_mtfw_reject is TRUE");
        CC_DeleteCallInfo(call_index);
        s_call_context.exist_call.exist_mtfw = FALSE;
        //SPCSS00056682
        //s_bt_setupind_flag = FALSE;
        MMICC_SetBtCallFlag(FALSE);
        return;
    }
     
    MMIDEFAULT_TurnOnBackLight();

    switch(s_call_context.call_info[call_index].call_state)
    {
    case CC_INIT_STATE:
        break;
        
    case CC_CALLING_STATE:
        s_call_context.exist_call.exist_outgoing_call = FALSE;
        break;
        
    case CC_CALL_STATE:
        s_call_context.exist_call.exist_active_call_num--;
        break;
        
    case CC_INCOMING_CALL_STATE:
    case CC_WAITING_STATE:
        s_call_context.exist_call.exist_incoming_call = FALSE;
        s_call_context.incoming_call_index = CC_INVALID_INDEX;
        s_bt_setupind_flag = FALSE;
        
        //if mt call is released, the operate for waiting call is invalid
        if (OTHER_OPERATE != s_call_context.operate_waiting_call)
        {
            s_call_context.operate_waiting_call = OTHER_OPERATE;
        }   
        break;
        
    case CC_HOLD_STATE:
        s_call_context.exist_call.exist_hold_call_num--;
        break;
        
    default:
        SCI_TRACE_LOW("ccapp.c:CC_DisconnectedCallByIndex() call_state = %d", s_call_context.call_info[s_call_context.current_call].call_state);
        break;       
    }

    //处理窗口前应先对通话结束的时间赋值,否则通话时间计算有误
    if (CC_INCOMING_CALL_STATE == s_call_context.call_info[call_index].call_state
        ||CC_WAITING_STATE == s_call_context.call_info[call_index].call_state
        ||CC_CALLING_STATE == s_call_context.call_info[call_index].call_state
        )
    {
        //未接通
        s_call_context.call_info[call_index].call_time.stop_time
            = s_call_context.call_info[call_index].call_time.start_time;
    }
    else
    {
        s_call_context.call_info[ call_index ].call_time.stop_time = MMIAPICOM_GetCurTime(); 
    }  
    //处理窗口 begin    
    MMICC_UpdateCallStatusDisplay(MMICC_DISCONNECTED_IND);
    call_time = MMIAPICL_GetCallDurationTime(s_call_context.call_info[call_index].call_time.start_time,
                            s_call_context.call_info[call_index].call_time.stop_time);//s_call_context.call_info[call_index].call_time.stop_time - s_call_context.call_info[call_index].call_time.start_time;
    //display disconnect window
    MMICC_OpenDisconnectWin(call_index, MN_CAUSE_NORMAL_CLEARING, call_time);
    //close window
    MMK_CloseWin( MMICC_HOLDMENU_WIN_ID );
    MMK_CloseWin( MMICC_STATUS_WIN_ID );
    MMIAPIIDLE_CloseDialMenuWin();
    if((CC_INCOMING_CALL_STATE == s_call_context.call_info[call_index].call_state)
        ||(CC_WAITING_STATE == s_call_context.call_info[call_index].call_state))
    {
        MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
    }
    s_bt_current_call_index = CC_INVALID_INDEX;

    //close dtmf sending win
    if (s_call_context.call_info[call_index].call_id == s_current_dtmf_call_id)
    {
        CC_ClearStoredDtmf();
        MMK_CloseWin(WATCHCC_SEND_DTMF_NOTE_WIN_ID);
        MMK_CloseWin(WATCHCC_SEND_DTMF_QUERY_WIN_ID);
    }

    //记录call log
    CC_BtSaveCLInfo(&cl_info, &calllog_type, call_index);
    CC_HandleDisconnectedInMPTY( call_index, MN_CAUSE_NORMAL_CLEARING );    
    //cr230071
    if(MMICC_GetBtCallFlag())
    {
        if ((!MMICC_IsExistIncommingCall())
            && (0 == MMICC_GetCallNum()) //用户已经接听来电，正在连接中，此时不应该关闭audio,但是要关闭codec
            )
        {
            CC_OpenAudioTunnel(FALSE);
        }
        else
        {
            MMICC_EnableVoiceCodec(FALSE);
            MMICC_DestroyVoiceCodec();
        }
        //233618
        MMICC_UpdateStatusWin();
        //close speaker and microphone
    }

    //MMICC_HandleDownlinkChanel();
    if(!MMICC_GetBtCallFlag())
    {
        MMIAPICL_RecordCallInfo(s_call_context.dual_sys, calllog_type, cl_info);
    }
    /*cr128956 renwei add*/
    #ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT   
    if(MMICL_CALL_MISSED == calllog_type)
    {
        MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL);
    }
    #endif
    /*cr128956 renwei add*/
    SCI_MEMSET(tel_num, 0x00, sizeof(tel_num));
    if (MMICL_CALL_DIALED == calllog_type)
    {
        tel_num_len = MMIAPICOM_GenDispNumber(
                                                cl_info.call_num.number.number_type , 
                                                (uint8)cl_info.call_num.number.num_len, 
                                                cl_info.call_num.number.party_num,
                                                tel_num,
                                                sizeof(tel_num)
                                                );
    }
    else
    {
        tel_num_len = MMIAPICOM_GenNetDispNumber(
                                                cl_info.call_num.number.number_type , 
                                                (uint8)cl_info.call_num.number.num_len, 
                                                cl_info.call_num.number.party_num,
                                                tel_num,
                                                sizeof(tel_num)
                                                );
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    MMIAPIPB_SetNumberUsed(tel_num, tel_num_len);
#endif
    /* Bug fix: cr00198061. when a call disconnected, allow key auto lock */
    MMIDEFAULT_AllowTurnOffBackLight(TRUE);

    //if don't play end hit, excute call end operation
#ifdef MMI_MULTI_SIM_SYS_NONE
      resetCallPara();
      s_call_context.call_number = 0;
      MMICC_SetBtCallFlag(FALSE);
      MAIN_SetCallingState(FALSE);
      MMICC_DestroyAudioHandle();
	s_bt_current_call_index = CC_INVALID_INDEX;
	s_bt_call_flag = FALSE;
	s_bt_setupind_flag = FALSE;
	s_bt_connect_wait_exist =FALSE;
	s_bt_audconnind_flag =FALSE;
	s_current_index      = CC_INVALID_INDEX;
	g_call_disc_by_bt    = FALSE;
	s_sco_request_timer = 0;
	s_bt_sco_link_is_exist = FALSE;	
    //       MMICC_HandleOperAfterCallEnd();	
#else
    if(MMICC_GetBtCallFlag())
    {		
        MMICC_SetBtCallFlag(FALSE);
    }
    
    if (0 != MMICC_GetCallNum())
    {
        MAIN_SetCallingState(TRUE);
    }
#endif
}

/*****************************************************************************/
// 	Description : Hfu call hold result ind
//	Global resource dependence : 
//  Author:wenny cheng
//	Note:2013.06.08
/*****************************************************************************/
LOCAL void CC_BtHfuHoldCfnInd(BT_MSG_T  *msg_ptr)
{
    uint8 hold_type = s_call_context.call_info[s_bt_current_call_index].operated_by_user;
    if(PNULL != msg_ptr && BT_SUCCESS == msg_ptr->status)
    {
        switch(hold_type)
        {
        case CC_HOLD_OPER :
            {
                SCI_TRACE_LOW("MMICC_HandleBtPsMsg MMICC_HOLD_SUC_IND SUCCESS !!! s_bt_current_call_index %d",s_bt_current_call_index);
                s_call_context.call_info[ s_bt_current_call_index ].call_state = CC_HOLD_STATE;
                s_call_context.call_info[s_bt_current_call_index].operated_by_user = CC_NO_OPER;
                s_call_context.exist_call.exist_hold_call_num ++;
                s_call_context.exist_call.exist_active_call_num --;
                s_call_context.current_call = s_bt_current_call_index;
                MMICC_UpdateCallStatusDisplay(MMICC_HOLD_SUC_IND);		   	
            }
            break;

        case CC_SPLIT_OPER:
            {
                s_call_context.call_info[ s_bt_current_call_index ].call_state = CC_HOLD_STATE;
                s_call_context.call_info[s_bt_current_call_index].operated_by_user = CC_NO_OPER;
                s_call_context.exist_call.exist_hold_call_num ++;
                s_call_context.exist_call.exist_active_call_num --;
                s_call_context.call_info[s_bt_current_call_index].is_in_mpty = FALSE;				
                s_call_context.current_call = s_bt_current_call_index = FindStateCall(CC_CALL_STATE);
                MMICC_UpdateCallStatusDisplay(MMICC_HOLD_SUC_IND);	
            }
	    break;

        case CC_RETRIEVE_OPER:
            {
                s_call_context.call_info[ s_bt_current_call_index ].call_state = CC_CALL_STATE;
                s_call_context.call_info[s_bt_current_call_index].operated_by_user = CC_NO_OPER;
                s_call_context.exist_call.exist_active_call_num ++;
                s_call_context.exist_call.exist_hold_call_num --;
                s_call_context.current_call = s_bt_current_call_index;
                MMICC_UpdateCallStatusDisplay(MMICC_RETRIEVE_SUC_IND);		   	
            }
            break;
        default:
            break;
        }
    }
}

#if defined(MMI_MULTI_SIM_SYS_NONE) || defined(BT_DIALER_SUPPORT)//bug924697
LOCAL void CC_BtDisconnectCallByIndex(uint8 call_index)
{
    MMICL_CALL_INFO_T		cl_info = {0};
    //uint8					call_index = CC_INVALID_INDEX;
    uint32 call_time = 0;
    uint8			        tel_num[MMICC_PHONE_NUM_MAX_LEN + 2] = {0};
    uint8			        tel_num_len = 0; 
    MMICL_CALL_TYPE_E calllog_type = MMICL_CALL_DIALED;

    if(call_index >= s_call_context.call_number)
    {
        SCI_TRACE_LOW("ccapp.c ERROR:CC_BtDisconnectCallByIndex() call_index >= s_call_context.call_number");
    }

    if(!CC_IsCallIDValid(call_index))
    {
        SCI_TRACE_LOW("ccapp.c ERROR:CC_BtDisconnectedCall() current_call invalid %d",call_index);
        MMK_CloseWin(MMICC_PROCESSING_WIN_ID);
        return;
    }
	 
    MMIDEFAULT_TurnOnBackLight();
    switch(s_call_context.call_info[call_index].call_state)
    {
    case CC_INIT_STATE:
        break;
        
    case CC_CALLING_STATE:
        s_call_context.exist_call.exist_outgoing_call = FALSE;
        break;
        
    case CC_CALL_STATE:
        s_call_context.exist_call.exist_active_call_num--;
        break;
        
    case CC_INCOMING_CALL_STATE:
    case CC_WAITING_STATE:
        s_call_context.exist_call.exist_incoming_call = FALSE;
        s_call_context.incoming_call_index = CC_INVALID_INDEX;
        s_bt_setupind_flag = FALSE;
        
        //if mt call is released, the operate for waiting call is invalid
        if (OTHER_OPERATE != s_call_context.operate_waiting_call)
        {
            s_call_context.operate_waiting_call = OTHER_OPERATE;
        }   
        break;
        
    case CC_HOLD_STATE:
        s_call_context.exist_call.exist_hold_call_num--;
        break;
        
    default:
        SCI_TRACE_LOW("ccapp.c:CC_DisconnectedCallByIndex() call_state = %d", s_call_context.call_info[s_call_context.current_call].call_state);
        break;       
    }

    //处理窗口前应先对通话结束的时间赋值,否则通话时间计算有误
    if (CC_INCOMING_CALL_STATE == s_call_context.call_info[call_index].call_state
        ||CC_WAITING_STATE == s_call_context.call_info[call_index].call_state
        ||CC_CALLING_STATE == s_call_context.call_info[call_index].call_state
        )
    {
        //未接通
        s_call_context.call_info[call_index].call_time.stop_time
            = s_call_context.call_info[call_index].call_time.start_time;
    }
    else
    {
        s_call_context.call_info[ call_index ].call_time.stop_time = MMIAPICOM_GetCurTime(); 
    }

    //处理窗口 begin    
    MMICC_UpdateCallStatusDisplay(MMICC_DISCONNECTED_IND);
    call_time = MMIAPICL_GetCallDurationTime(s_call_context.call_info[call_index].call_time.start_time,
                            s_call_context.call_info[call_index].call_time.stop_time);//s_call_context.call_info[call_index].call_time.stop_time - s_call_context.call_info[call_index].call_time.start_time;
    //display disconnect window
    MMICC_OpenDisconnectWin(call_index, MN_CAUSE_NORMAL_CLEARING, call_time);

    if((CC_INCOMING_CALL_STATE == s_call_context.call_info[call_index].call_state)
        ||(CC_WAITING_STATE == s_call_context.call_info[call_index].call_state))
    {
        MMK_CloseWin(MMICC_ANIMATION_WIN_ID);
    }

   //s_bt_current_call_index = hold_index;
   
    //close dtmf sending win
    if (s_call_context.call_info[call_index].call_id == s_current_dtmf_call_id)
    {
        CC_ClearStoredDtmf();
        MMK_CloseWin(WATCHCC_SEND_DTMF_NOTE_WIN_ID);
        MMK_CloseWin(WATCHCC_SEND_DTMF_QUERY_WIN_ID);
    }

    //记录call log
    CC_BtSaveCLInfo(&cl_info, &calllog_type, call_index);
    CC_HandleDisconnectedInMPTY( call_index, MN_CAUSE_NORMAL_CLEARING );    
        
    MMIAPICL_RecordCallInfo(s_call_context.dual_sys, calllog_type, cl_info);
    /*cr128956 renwei add*/
    #ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT   
    if(MMICL_CALL_MISSED == calllog_type)
    {
        MMIMAIN_AppendStbScrollItem(GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL);
    }
    #endif
    /*cr128956 renwei add*/
    SCI_MEMSET(tel_num, 0x00, sizeof(tel_num));
    if (MMICL_CALL_DIALED == calllog_type)
    {
        tel_num_len = MMIAPICOM_GenDispNumber(
                                                cl_info.call_num.number.number_type , 
                                                (uint8)cl_info.call_num.number.num_len, 
                                                cl_info.call_num.number.party_num,
                                                tel_num,
                                                sizeof(tel_num)
                                                );
    }
    else
    {
        tel_num_len = MMIAPICOM_GenNetDispNumber(
                                                cl_info.call_num.number.number_type , 
                                                (uint8)cl_info.call_num.number.num_len, 
                                                cl_info.call_num.number.party_num,
                                                tel_num,
                                                sizeof(tel_num)
                                                );
    }
#ifdef MMIPB_MOST_USED_SUPPORT
    MMIAPIPB_SetNumberUsed(tel_num, tel_num_len);
#endif
    /* Bug fix: cr00198061. when a call disconnected, allow key auto lock */
    MMIDEFAULT_AllowTurnOffBackLight(TRUE);

    //if don't play end hit, excute call end operation
    if(!MMICC_GetIsPlayCallEndHintFlag() && !MMK_IsOpenWin(MMICC_DISCONNECT_WIN_ID))
    {
        MMICC_HandleOperAfterCallEnd();
    }
}
#endif

/*****************************************************************************/
// 	Description : release BT Call Interface
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_ReleaseBtCall(void)
{
    if(MMICC_GetBtCallFlag())
    {
        CC_BtDisconnectedCall();
    }
}

#ifdef MMI_MULTI_SIM_SYS_NONE
/*****************************************************************************/
// 	Description : Make BT Disconnect Call By Index
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_BtDisconnectCallByIndex(uint8 call_index)
{
    if(MMICC_GetBtCallFlag())
    {
        CC_BtDisconnectCallByIndex(call_index);
    }
    return CC_RESULT_SUCCESS;
}
/*****************************************************************************/
// 	Description : MMIAPICC_HandleBtHangupButton
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_HandleBtHangupButton(void)
{
	uint8 call_number = MMICC_GetCallNum();
	uint8 disconnect_idx = CC_INVALID_INDEX;
	CC_CALL_STATE_E call_state = MMICC_GetCurrentCallStatus();
	BT_STATUS bt_status = BT_ERROR;

	if(1 < call_number)
	{									
		switch(call_state)
		{
		case CC_CALL_STATE:
		{
			if(!MMICC_IsExistMPTY()) //active call
			{
				uint8 hold_idx = MMICC_FindStateCall(CC_HOLD_STATE);
				disconnect_idx = MMICC_FindStateCall(CC_CALL_STATE);
				bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_RELEASE_ACTIVE_CALLS);
				MMIAPICC_BtDisconnectCallByIndex(disconnect_idx);
				//if(hold_idx <= s_call_context.call_number)
				{
					s_call_context.call_info[s_call_context.current_call].call_state = CC_CALL_STATE;
					//s_call_context.call_info[hold_idx].operated_by_user = CC_NO_OPER;
					s_call_context.exist_call.exist_active_call_num ++;
					s_call_context.exist_call.exist_hold_call_num --;
					//s_call_context.current_call = s_bt_current_call_index = hold_idx;					
				}
				return;
			}
			else //multi-party call
			{
				BOOLEAN bSupport_Split = FALSE;
				BOOLEAN bSupport_end_specific_call = FALSE;		
				
				MMIAPIBT_HfuIsAGSupportRelSpecCallIndex(&bSupport_end_specific_call);
				MMIAPIBT_HfuIsAGSupportPrivateConsultMode(&bSupport_Split);

				SCI_TRACE_LOW("ccapp.c: HandleCCWinPenOk  bSupport_Split %d ,  bSupport_end_specific_call %d  !!!!!" , bSupport_Split , bSupport_end_specific_call);
				if(bSupport_Split && bSupport_end_specific_call)
				{
					MMIAPICC_OpenMultiCallList();
				}
				else
				{
					bt_status = MMIAPIBT_HfuHangup();
					SCI_TRACE_LOW("ccapp.c: HandleCCWinPenOk  MMIAPIBT_HfuHangup  !!!!!");						
					if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
					{
						SCI_TRACE_LOW("ccapp.c:HandleCCWinPenOk() MMIAPIBT_HfuHangup fail");
					}
				}
				return;
			}
		}
			break;
		case CC_WAITING_STATE: //incoming call or hold call
		case CC_HOLD_STATE:
			disconnect_idx = (call_state==CC_WAITING_STATE)?MMICC_FindStateCall(CC_WAITING_STATE):MMICC_FindStateCall(CC_HOLD_STATE);
			bt_status = MMIAPIBT_HfuCallHold(BT_HOLD_RELEASE_HELD_CALLS);
			break;
		default:
			break;
		}
		SCI_TRACE_LOW("ccapp.c:HandleCCWinPenOk() call_state %d" , call_state);
		MMIAPICC_BtDisconnectCallByIndex(disconnect_idx);
	}
	else
	{
		bt_status = MMIAPIBT_HfuHangup();
		SCI_TRACE_LOW("ccapp.c: HandleCCWinPenOk  MMIAPIBT_HfuHangup  !!!!!");		
		
		if(BT_SUCCESS != bt_status && BT_PENDING != bt_status)
		{
			SCI_TRACE_LOW("ccapp.c:HandleCCWinPenOk() MMIAPIBT_HfuHangup fail");
		}
	}            
}

/*****************************************************************************/
// 	Description : MMIAPICC_HandleBtHangupButton
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_HandleMPTY_ByIndex(BOOLEAN bSplit , uint8 call_index)
{
	if(bSplit)
	{
		int i=0;

		s_bt_current_call_index = call_index;
		s_call_context.call_info[s_bt_current_call_index].operated_by_user = CC_SPLIT_OPER;									
		MMIAPIBT_HfuPrivateConsultaionMode(call_index + 1); //1 based , so plus 1 ~~
	}
	else
	{
		MMIAPIBT_HfuReleaseSpecialCallIndex(call_index + 1); //1 based , so plus 1 ~~
		MMIAPICC_BtDisconnectCallByIndex(call_index);
	}		
}
#endif
#endif
#ifdef UAL_BT_HFU_SUPPORT
/*****************************************************************************/
//  Description : tel number string valid check
//  Global resource dependence : 
//  Author: 
//  Note:
//  Return: TRUE: valid tel number
//          FALSE:invalid tel number
//       if mmicom_data.c MMIAPICOM_IsValidNumberString check_ssc_string check_tel_string modified
//       the function must modified
/*****************************************************************************/
LOCAL BOOLEAN check_tel_string(char *pStr)
{
    uint8 i, len;
    int8 plus_index;

    if(pStr == PNULL)
    {
        return FALSE;
}
    len = strlen((char*)pStr);

    //以"+p""+w""+P""+W""p""w""P""W"开头的无效
    if((!strncmp(pStr, "+p", strlen("+p")))
       ||(!strncmp(pStr, "+w", strlen("+w")))
       ||(!strncmp(pStr, "+P", strlen("+P")))
       ||(!strncmp(pStr, "+W", strlen("+W")))
       ||(!strncmp(pStr, "p", strlen("p")))
       ||(!strncmp(pStr, "w", strlen("w")))
       ||(!strncmp(pStr, "P", strlen("P")))
       ||(!strncmp(pStr, "W", strlen("W")))
       )
    {
        return FALSE;
    }

    plus_index = -1;
    //检查开头+的个数
    for ( i=0; i<len ; i++)
    {
        if (pStr[i] == '+' )
        {
            plus_index = i;
        }
    }

    // 中间出现"+"是无效的
    if ((plus_index > 0)&&(plus_index < (len - 1)))
    {
        return FALSE;
    }
    //这个号码是“+”无效
    else if (plus_index == 0 && len == 1)
    {
        return FALSE;
    }

    // * 和 # 前面只有+或者以* 和 #开头，无效
    //本条规则放宽，去掉
    /*plus_index++;
    if(pStr[plus_index] == '#' || pStr[plus_index] == '*')
    {
        return FALSE;
    }*/

    return TRUE;
}
/*****************************************************************************/
//  Description : IsValidTelenum
//  Global resource dependence : 
//  Author: chunjie.liu
//  Note:if mmicom_data.c MMIAPICOM_IsValidNumberString check_ssc_string check_tel_string modified
//       the function must modified
/*****************************************************************************/
LOCAL BOOLEAN  IsValidTelenum(
                      char *tele_num,   // String的头指针
                      uint8 tele_len  // String的长度
                            )
{
    BOOLEAN is_num_valid = FALSE;
	
    if(PNULL == tele_num)
    {
        SCI_TRACE_LOW("ccapp.c:enter function MMIAPICC_MakeBtCall(), tele_num is NULL ");
        return FALSE;
    }
    if(0 == tele_len)
    {
        SCI_TRACE_LOW("ccapp.c:MMIAPICC_MakeBtCall() tele_len = 0");
        return FALSE;
    }
    if(MMIAPICOM_IsValidNumberString(tele_num,tele_len))
    {
        /*1. 如果是以“*”“#”“**” “*#”或者“##”开始，以"#"结尾的，或者形似“7X”，X为0-9之间的一个数字，认为是ssc号码，进行ssc号码有效性判断（70--79就是ussd字串；前面的需要继续进行判断）；
        2. 否则认为是普通号码，进行普通号码的有效性判断；*/
	//以*,#,**,*#,##开头的，且以"#"结尾的号码是ssc号码
	if(!(((!strncmp(tele_num, "*", strlen("*")) 
		||!strncmp(tele_num, "#", strlen("#")) 
		||!strncmp(tele_num, "**", strlen("**"))
		||!strncmp(tele_num, "*#", strlen("*#")) 
		||!strncmp(tele_num, "##", strlen("##")))
		&&(tele_num[tele_len - 1] == '#'))
		|| (!strncmp(tele_num, "7", strlen("7")) && (tele_num[1] <= 0x39) && (tele_num[1] >= 0x30) && (2 == tele_len)))
		)
	    {
	        is_num_valid = check_tel_string(tele_num);
	    }
    
    }
    else
    {
        SCI_TRACE_LOW("ccapp.c:IsValidTelenum() tele_num is not valid number");
    }
    return is_num_valid;

}
/*****************************************************************************/
// Description : Make BT Call Interface
//Global resource dependence :
//Author:yonghua.zhang
//Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_MakeBtCall(
   uint8 *tele_num,//phone number
   uint8 tele_len//phone number len
   )
{
    CC_RESULT_E cc_result = CC_RESULT_SUCCESS;
    ual_bt_status_e bt_status= UAL_BT_STATUS_ERROR;

    if(PNULL == tele_num)
    {
        SCI_TRACE_LOW("ccapp.c:enter function MMIAPICC_MakeBtCall(), tele_num is NULL ");
        return CC_RESULT_ERROR;
    }

    if(0 == tele_len)
    {
        SCI_TRACE_LOW("ccapp.c:MMIAPICC_MakeBtCall() tele_len = 0");
        return CC_RESULT_ERROR;
    }

    if(MMIAPIPARSE_ParseString(tele_num, tele_len))
    {
        //SCI_TRACE_LOW:"ccapp.c: MMIAPICC_MakeCall Parsed in ParseString"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICC_APP_7565_112_2_18_2_10_22_199,(uint8*)"");
        return cc_result;
    }

    SCI_TRACE_LOW("mmiccapp:MMIAPICC_MakeBtCall  tele_num = %s, tele_len = %d", tele_num, tele_len);
     //MS00186848 用于自动测试中,避免呼出一些报警电话等
    if ((MMIAPIENG_GetCallBarring() && IsIncludeBarringNumbers(tele_num, tele_len))
        || MMICC_IsExistActiveCall()
        )
    {
       return CC_RESULT_ERROR;
    }

    if(CC_INVALID_INDEX != ual_bt_get_current_index() && ual_bt_get_bt_call_flag())
    {
        SCI_TRACE_LOW("ccapp.c: MMIAPICC_MakeBtCall()  s_bt_current_call_index = %d", ual_bt_get_current_index());
        return CC_RESULT_ERROR;
    }

    if(!IsValidTelenum((char*)tele_num, tele_len)
        || (1 == tele_len) // used ussd or speed dail
        ///1x is used phone number, other is used ussd
        || ((2 == tele_len) && (tele_num[0] != 0x31))
        )
    {
        return CC_RESULT_ERROR;
    }

    //if setup_ind msg 已经收到，拨号失败
    if (TRUE == ual_bt_is_exist_incoming_call())
    {
        SCI_TRACE_LOW("ccapp.c:MMIAPICC_MakeBtCall() setup_ind msg is being");
        return CC_RESULT_ERROR;
    }

    bt_status = ual_bt_make_call(tele_num);

    SCI_TRACE_LOW("ccapp.c:ual_bt_make_call() bt_status = %d", bt_status);

    if(UAL_BT_STATUS_PENDING != bt_status && UAL_BT_STATUS_SUCCESS != bt_status)
    {
        return CC_RESULT_ERROR;
    }

    return cc_result;
}

/*****************************************************************************/
//Description : release BT Call Interface
//Global resource dependence : 
//Author:chunjie.liu
//Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_ReleaseBtCall(void)
{
    ual_bt_status_e bt_status= UAL_BT_STATUS_ERROR;

    if(TRUE == ual_bt_get_bt_call_flag())
    {
        bt_status = ual_bt_release_call();
    }

    SCI_TRACE_LOW("ccapp.c:MMIAPICC_ReleaseBtCall() bt_status = %d", bt_status);
}

#endif

#ifdef MMI_RECORD_SUPPORT

/*****************************************************************************/
//  Description : CC_GetCurrentDev
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
LOCAL MMIFILE_DEVICE_E CC_GetCurrentDev(void)
{
    MMIFILE_DEVICE_E    fileDev = MMI_DEVICE_NUM;
    wchar*              filePathPtr = MMIRECORDSRV_GetRecordFilePath(s_cc_watch_record_handle);

    if(PNULL != filePathPtr)
    {
        fileDev = MMIAPIFMM_GetDeviceTypeByPath(filePathPtr, MMIFILE_DEVICE_LEN);
    }

    if(MMI_DEVICE_NUM <= fileDev)
    {
        fileDev= MMIAPIRECORD_GetRecordStorage();
    }

    SCI_TRACE_LOW("[CC_GetCurrentDev] file_dev = %d",fileDev);
    return fileDev;
}

/*****************************************************************************/
//  Description : EventNotifyCb_RecordInCall
//  Global resource dependence :
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
LOCAL void EventNotifyCb_RecordInCall(RECORD_SRV_HANDLE record_srv_handle,
                                                MMIRECORD_SRV_SUPPORT_TYPE_E support_type,
                                                MMIRECORD_SRV_RESULT_E result )
{
    SCI_TRACE_LOW("[EventNotifyCb_RecordInCall] record_srv_handle = %d, s_cc_watch_record_handle = %d, result = %d, support_type = %d",
                                                     record_srv_handle, s_cc_watch_record_handle, result, support_type);

	if (record_srv_handle == s_cc_watch_record_handle
		&& MMIRECORD_SRV_SUPPORT_RECORD == support_type)
	{
		if(!MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_FIXED ,CC_GetCurrentDev(), MMIRECORD_SRV_MIN_NEED_SPACE, PNULL))
		{
			result = MMIRECORD_SRV_RESULT_NO_SPACE;
		}
        switch (result)
        {
            case MMIRECORD_SRV_RESULT_SUCCESS:
                break;
            case MMIRECORD_SRV_RESULT_NO_SPACE:
                break;
            case MMIRECORD_SRV_RESULT_FOLDER_FULL:
                break;
            default:
            {
                MMIRECORDSRV_DeleteRecordFile(s_cc_watch_record_handle,TRUE);
                break;
            }
        }
        if(MMIRECORD_SRV_RESULT_SUCCESS != result)
        {
            MMICC_StopRecordInCall();
        }
	}
}

/*****************************************************************************/
//  Description : start record in call
//  Parameter: [In] None
//             [Out] None
//             [Return] Result
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMIRECORD_SRV_RESULT_E MMICC_StartRecordInCall(void)
{
    MMIRECORD_SRV_RECORD_PARAM_T    recordParam = {0};
    MMIFILE_DEVICE_E                recordDev = MMIAPIRECORD_GetRecordStorage();
    MMI_STRING_T                    nameStr = {0};

    wchar         telname[CC_MAX_PBNAME_LEN + 1]   = {0};
    wchar         name[CC_MAX_PBNAME_LEN + 1]   = {0};
    uint8         cur_call_num[CC_MAX_TELE_NUM_LEN + 2] = {0};
    uint8         tel_num_len = CC_MAX_TELE_NUM_LEN + 2;

    if (PNULL != s_cc_watch_record_handle)
    {
        return MMIRECORD_SRV_RESULT_ERROR;
    }

    s_cc_watch_record_handle = MMIRECORDSRV_RequestRecordHandle(EventNotifyCb_RecordInCall);
    if (PNULL == s_cc_watch_record_handle)
    {
        return MMIRECORD_SRV_RESULT_ERROR;
    }

    MMIAPICC_GetCallName(&nameStr);
    tel_num_len = MMIAPICC_GetCallNumber(cur_call_num, tel_num_len);
    if(MMIAPIPB_IsUseDefaultName(nameStr.wstr_ptr)||!MMIAPICOM_FileNameIsValid(&nameStr))
    {
        MMIAPICOM_StrToWstr(cur_call_num, telname);
        if(MMIAPICOM_GetValidFileName(FALSE, telname, tel_num_len, name, CC_MAX_PBNAME_LEN))
        {
            nameStr.wstr_ptr = name;
            nameStr.wstr_len = MMIAPICOM_Wstrlen(name);
        }
    }
    recordParam.fmt = MMIAPIRECORD_GetRecordFmt();
    recordParam.is_create_file_name = TRUE;
    recordParam.prefix_ptr = PNULL;
    recordParam.record_dev_ptr = &recordDev;
    recordParam.record_file_id_ptr = PNULL;
    recordParam.record_name_ptr = &nameStr;
    recordParam.source = MMISRVAUD_RECORD_SOURCE_CALL;
    recordParam.record_param_ptr = PNULL;
    recordParam.volume = MMIAPISET_GetMultimVolume();
    return MMIRECORDSRV_StartRecord(s_cc_watch_record_handle,recordParam);
}

/*****************************************************************************/
//  Description : stop record in call
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void MMICC_StopRecordInCall(void)
{
    if (PNULL != s_cc_watch_record_handle)
    {
        MMIRECORDSRV_StopRecord(s_cc_watch_record_handle);
        MMIRECORDSRV_FreeRecordHandle(s_cc_watch_record_handle);
        s_cc_watch_record_handle = 0;
    }
}

/*****************************************************************************/
//  Description : if is in recording
//  Parameter: [In] None
//             [Out] None
//             [Return] true or false
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsRecording(void)
{
    if (PNULL != s_cc_watch_record_handle)
    {
        return TRUE;
    }
    return FALSE;
}

#endif

