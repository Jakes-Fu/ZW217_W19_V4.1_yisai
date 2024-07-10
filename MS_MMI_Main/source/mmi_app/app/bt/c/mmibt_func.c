/****************************************************************************
** File Name:     mmibt_func.c                                        *
** Author:         Wenming.feng                                            *
** Date:           12/14/2005                                              *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the cryp display menu     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** Dec/14/2005    Wenming.feng     Create                                  *
**                                                                         *
****************************************************************************/
#define _MMIBT_FUNC_C_
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                     *
**---------------------------------------------------------------------------*/
#include "mmi_app_bt_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_timer.h"
#include "mmibt_func.h"
#include "mmibt_mainwin.h"
#include "mmibt_app.h"
#include "mmipub.h"
#include "guilabel.h"
#include "mmifmm_export.h"   //bt_abs.h中用到ffs.h中的结构
#include "bt_abs.h"
#include "bt_cfg.h"
#include "mmibt_text.h"
#include "mmi_default.h"
#include "mmiset_export.h"
#include "guitext.h"

#include "mmivcard_export.h"
#include "mmieng_export.h"
#include "mmicl_export.h"
#include "mmiphone_export.h"
#include "mmibt_a2dp.h"
#include "mmiudisk_export.h"
#include "mmisd_export.h"
#include "mmibt_nv.h"
#ifdef KURO_SUPPORT
#include "mmikur.h"
#endif
#include "mmipicview_export.h"
#include "mmialarm_export.h"
#include "mmi_mainmenu_export.h"
#include "mmifmm_text.h"
#include "guimenu.h"
#include "guires.h"
#include "mmidc_export.h"
#include "guiedit.h"
#include "mmiidle_export.h"
#include "mmimtv_export.h"
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmieng_id.h"
#include "guictrl_api.h"
#include "mmieng_text.h"
#include "mmienvset_export.h"
#include "mime_type.h"
#include "mmisms_export.h"

#ifdef DRM_SUPPORT
#include "mmidrm.h"
#endif
#include "mmikl_export.h"
#ifdef WRE_SUPPORT
#include "mmiwre_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmiidle_statusbar.h"

#include "mmimultim_image.h"
#include "mmisrvaud_api.h"
#include "mmi_nv.h"
#include "mmibt_headset.h"

#if defined(BT_MAP_SUPPORT)
#include "mmisms_id.h"
#include "mmisms_app.h"
#endif

#ifdef BT_PBAP_SUPPORT
#include "mmicc_id.h"
#endif

#ifdef BLUETOOTH_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMIBT_SCROLL_BAR_DEAFULT_LEN    2

#define  MMIBT_VIBRA_DEFAULT_TIME           1500
    
#define MMIBT_BT_DIR_NAME               "Bt"
#define MMIBT_REPEAT_NAME_SUFFIX_LEN     16

#define MMIBT_FILE_NAME_NO_SUFFIX_LEN   100

#define MMIBT_FTP_ALL_ROOT          "*"
#define MMIBT_DIR_BT_LEN                2   


#define  MMIBT_PIC_TYPE_MAX_NUM           10            //The number of picture file type
#define  MMIBT_RING_TYPE_MAX_NUM          14            //The number of ring file type 
#define  MMIBT_MOVIE_TYPE_MAX_NUM         4             //The number of movie file type  
#define  MMIBT_TEXT_TYPE_MAX_NUM          2             //The number of text file type
#define  MMIBT_JAVA_TYPE_MAX_NUM          4             //The number of java file type
#define  MMIBT_VCARD_TYPE_MAX_NUM         2             //The number of vcard file type

#define MMIBT_SET_VISIBILITY_HIDDEN     0
#define MMIBT_SET_VISIBILITY_VISIBLE     1

#define MMIBT_FTP_REQ_DEVICE_MAX_LEN        20
#define MMIBT_FTP_REQ_FILE_MAX_LEN          40

//"Ebook"       
//#define MMIBT_SIZE_INFO_RECT            {MMIBT_LABEL_LEFT,MMIBT_LABEL_SIZE_Y,MMI_MAINSCREEN_RIGHT_MAX_PIXEL, (MMIBT_LABEL_SIZE_Y + MMIBT_NEW_FILE_LINE_HEIGHT)}
//define all menu item number


#define MMIBT_MEUN_ID_INDEX             0
#define MMIBT_MEUN_TEXT_INDEX           1
#define MMIBT_ANIM_PLAY                 1
#define MMIBT_MAX_QUERY_NUM             20//所允许打开的最大询问的窗口数目

#define MMIBT_DISCONNECT_RETRY_COUNT    5

#ifdef BT_PBAP_SUPPORT
LOCAL const char s_pb_name[] = {'t','e','l','e','c','o','m','\/','p','b','.','v','c','f',0}; /*lint !e606*/
LOCAL const char s_sim_name[] = {'S','I','M','1','\/','t','e','l','e','c','o','m','\/','p','b','.','v','c','f',0}; /*lint !e606*/
#define MMIBT_PBAP_VCARD_FORMAT_21         BT_VCARD_FORMAT_21         
LOCAL     BT_PBAP_DATA_IND_T  pbap = {0};   //for test    
#endif

//#define MMIBT_FTP_BUSY      	        3
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                */
/*---------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
                  

extern HAUDIODEV hPCMDev; /*lint !e762*/
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
static  BOOLEAN     s_fts_del_wallpaper_file = FALSE;


//BT Interaction begin
static  MMIBT_TRANSFER_FILE_INFO_T    s_transfer_file_info = {0};            //info of transfer file: file name, file size, service type
static MMIBT_TRANSFER_STATUS_TYPE_E s_bt_transfer_status = MMIBT_TRANSFER_END;
BT_DEVICE_INFO     g_current_bt = {0};

#ifdef BT_DUN_SUPPORT
static  BOOLEAN                      s_is_vdun_on = FALSE;
#endif

LOCAL BOOLEAN s_is_permit_open = FALSE;

#ifdef BT_DIALER_SUPPORT
LOCAL uint8 s_hfu_reco_timer = 0;
#define BT_HFU_RECO_TIMER_OUT        (6*1000)        //HFU重连间隔时长
LOCAL uint8 s_hfu_reco_count = 0;
#define BT_HFU_RECO_COUNT            20
LOCAL BOOLEAN s_is_auto_reco = FALSE;
#define BT_HFU_AUTO_CONNECT_TIMER_OUT        (10*1000)        //HFU重连间隔时长
LOCAL uint8 s_bt_hfu_auto_connect_timer = 0;
#if (defined MMI_SMS_SYN_SUPPORT) && (defined BT_SPP_SUPPORT)
LOCAL BOOLEAN s_is_sms_sync = FALSE;
#endif
#endif

#ifdef BT_PBAP_SUPPORT
LOCAL BOOLEAN s_is_cancel_pbap_pull = FALSE; 
LOCAL BT_PBAP_TYPE_E s_pbap_connect_type = BT_PBAP_TYPE_NONE; 
LOCAL uint8 s_bt_connect_timeout_timer = 0;
LOCAL BOOLEAN s_is_pbap_pull_phone = TRUE;  
#endif

#if defined(BT_MAP_SUPPORT)
LOCAL BOOLEAN s_is_map_connect = FALSE; 
#endif

#ifdef BT_AV_SNK_SUPPORT
LOCAL BOOLEAN s_a2dp_sink_connected = FALSE;
MMISRV_HANDLE_T s_a2dp_sink_handle = 0;
LOCAL uint32 s_a2dp_sink_sample_rate = 0;
LOCAL uint8 s_a2dp_sink_channel_num = 0;
LOCAL uint16 s_a2dp_request_timer_id = 0;
LOCAL uint8 s_a2dp_retry_count = 0;
LOCAL uint16 s_a2dp_retry_timer_id = 0;
LOCAL BOOLEAN s_a2dp_sink_request_lock = FALSE;
LOCAL uint8 s_a2dp_remote_play_status = 0x00;
LOCAL uint8 s_a2dp_local_play_status = 0x00;
LOCAL BOOLEAN is_need_resume = FALSE;
LOCAL BOOLEAN s_a2dp_sink_resume = FALSE;
LOCAL void AvrcpPanelKeyRelease(uint8 timerid,uint16 op);
LOCAL void A2dpSinkResumeRetry(void);
LOCAL BOOLEAN A2dpSinkPlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param);
#endif

#ifdef BT_AV_CT_SUPPORT
LOCAL BOOLEAN s_avrcp_ct_connected = FALSE;
#endif

/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription:  clean all hide device
//  Global 
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
// LOCAL void CleanAllHideDevice(void);

/*****************************************************************************/
//  Description : is premit open this file or not
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/


LOCAL BOOLEAN Is_PermitOpenFile(void);

#ifdef BT_DUN_SUPPORT
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDUNWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    );

/*****************************************************************************/
//  Discription: Handle try pair again or not win msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleConnectDUNOrNotMsg(
                                                   MMI_WIN_ID_T win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param 
                                                   );
#endif

/*****************************************************************************/
//  Discription:  close send file windows
//  Global resource dependence: None
//  Author:
//  Note : if current opertation is opp_send, close inquery list and device list
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN  CloseSendWin(void);



/*****************************************************************************/
//  Discription:display memory or udisk not exist or sd card not exist
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMemoryInformation(MMIFILE_DEVICE_E filedev);


/*****************************************************************************/
//  Discription: Handle send connect waiting win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendConnectWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Discription: Get the show string of lock status 
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleListMenuWinMsg(
                                          MMI_WIN_ID_T win_id, 
                                          MMI_MESSAGE_ID_E msg_id, 
                                          DPARAM param             
                                          );
#endif
/*****************************************************************************/
//  Discription: Handle blue tooth ui test result win msg
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL   MMI_RESULT_E HandleBtUiResultWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            );


/*****************************************************************************/
//  Discription: Handle blue tooth incoming file win msg
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleIncomingFileWinMsg(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             );

/*****************************************************************************/
//  Discription: Handle blue tooth new file win msg
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleNewFileWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            );

/*****************************************************************************/
//  Discription:start vibrator
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
//LOCAL void StartVibrator(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Discription: set bt operation to backgroup runing
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
//LOCAL void  SetBackgroupRun(BOOLEAN is_enable);

///*****************************************************************************/
////  Discription:  get bt backgroup runing or not
////  Global resource dependence: None
////  Author: kelly.li 
////  Note : 
////  Return:
///*****************************************************************************/
// LOCAL BOOLEAN  GetIsBackgroupRun(void);
/*****************************************************************************/
//  Discription:stop vibrator timer
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
//LOCAL void StopVibratorTimer(void);

/*****************************************************************************/
//  Discription: Handle ftp device serive list win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E   HandleTextBoxWinMsg(
                                                      MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param 
                                                      );

/*****************************************************************************/
//  Discription: set if delete wallpaper file by fts
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL void  SetIsDelWallpaperFile(BOOLEAN   is_del_wallpaper);

/*****************************************************************************/
//  Discription: get if delete wallpaper file by fts
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN  GetIsDelWallpaperFile(void);

/*****************************************************************************/
//  Discription: Set selected blue tooth visibility 
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL void  SetBtVisibility(MMIBT_VISIBBILE_TYPE  type);
// @baokun fix cr96895 begin
/*****************************************************************************/
//  Discription: Handle no space for incoming file alert win msg
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoSpaceForFileAlertMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param 
                                               );
// @baokun fix cr96895 end


/*****************************************************************************/
//  Discription: Handle delete paired device or not win msg  
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL void DeletePairedDevice(BOOLEAN is_all,MMIBT_OPERTATION_TYPE_E  current_opt);
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN MMIBT_IsEnoughSpaceToRecieve(MMIFILE_DEVICE_E file_dev);

/*****************************************************************************/
//  Description : open direct and get user selected dir
//  Global resource dependence : 
//  Author: 
//  Note: after user selected a dir, it will be set to shaired directory
/*****************************************************************************/
PUBLIC void MMIBT_SetFileLocationCallBack( BOOLEAN     is_success,
                                    FILEARRAY   file_array
                                    );


/*****************************************************************************/
//  Discription: None
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleSaveNewFileFailedWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            );

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_GetPairedDevAddr
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_GetPairedDevAddr(BT_ADDRESS *dev_addr);

/*****************************************************************************/
//  Description : MMIBT_StartRecoTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_StartRecoTimer(void);

/*****************************************************************************/
//  Description : MMIBT_StopRecoTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_StopRecoTimer(void);

/*****************************************************************************/
//  Description : MMIBT_HFUReconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_RecoTimeOut(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : MMIBT_HFUReconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_HFUReconnect();

/*****************************************************************************/
//  Description : handle headset close result
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void handleHandsetGatewayCloseResult(BT_STATUS bt_status, BT_ADDRESS *headset_addr_ptr);

/*****************************************************************************/
//  Description : ConnectHandsetGatewayService
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ConnectHandsetGatewayService(void);

/*****************************************************************************/
//  Description : MMIBT_ConnectHandsetGatewayProfile
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_ConnectHandsetGatewayService(void);

/*****************************************************************************/
//  Description : StopAutoConnectHandsetGatewayTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StopAutoConnectHandsetGatewayTimer(void);

/*****************************************************************************/
//  Description : StartAutoConnectHandsetGatewayTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StartAutoConnectHandsetGatewayTimer(void);

/*****************************************************************************/
//  Description : close headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BT_STATUS CloseHandsetGateway(const BT_ADDRESS *src_addr);

#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_PBPbapParserCallback(MMIPB_PBAP_PARSER_RESULT_T* result);

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIBT_PbapDisconnect(void);

/*******************************************************************************/
//  Description : generate whole display string
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_GenerateDisplayInfo(wchar *info_ptr,uint32 sync_contact_count);

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinDirectConnectWaitingMsg(
                                                       MMI_WIN_ID_T         win_id, 
                                                       MMI_MESSAGE_ID_E    msg_id, 
                                                       DPARAM              param
                                                       );
#endif

#ifdef BT_MAP_SUPPORT
/*****************************************************************************/
//  Discription: sync confirm implementation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_DirectConnectMapService(void);
#endif

/*---------------------------------------------------------------------------*3
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                            LOCAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Discription: Set the status that: blue tooth is hidden
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
//LOCAL void SetHiddenBtDevice(BOOLEAN    hidden_bt_device)
//{
//    s_bt_hidden_for_app = hidden_bt_device;
// }

/*****************************************************************************/
//  Discription: get the status that: blue tooth is hidden because of other application
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
//LOCAL BOOLEAN  GetIsBtHiddenForApp(void)
//{
//    return (s_bt_hidden_for_app);
//}
/*****************************************************************************/
/*****************************************************************************/
//  Discription: close all pb window except main window
//  Global resource dependence: none 
//  Author: mary.xiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CloseAllWindow(void)
{
    BT_CancelSearch();
    if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_PIN_INPUT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_PIN_INPUT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SET_SAVE_LOCATION_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SET_SAVE_LOCATION_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_RECEIVING_FILE_WIN_ID))
    {
        MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_INCOMING_FILE_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_DEVICE_DETAILS_WIN_ID))
    {
        MMK_CloseWin(MMIBT_DEVICE_DETAILS_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_INQUIRY_OPT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_INQUIRY_OPT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_TEXT_BOX_WIN_ID))
    {
        MMK_CloseWin(MMIBT_TEXT_BOX_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SET_VISIBILITY_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SET_VISIBILITY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SHARED_DIRECTORY_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SHARED_DIRECTORY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SHARED_AUTHORITY_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SHARED_AUTHORITY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SHARED_AUTHORITY_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SHARED_AUTHORITY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SHARED_FILE_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SHARED_FILE_WIN_ID);
    }
}
/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandlePubwinWaitingMsg(
                                          MMI_WIN_ID_T         win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_KEYDOWN_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif

    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

/*****************************************************************************/
//  Discription: Set selected blue tooth visibility 
//  Global resource dependence: none 
//  Author: kelly.li
//  Note:
//
/*****************************************************************************/
LOCAL void  SetBtVisibility(MMIBT_VISIBBILE_TYPE  type)
{
    BT_STATUS               ret_val = BT_SUCCESS;
    MMI_STRING_T            wait_text = {0};
    
    MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text); 
    //if(0 == cur_select)
    if(MMI_BT_HIDE == type)
    {
        ret_val = BT_SetVisibility(BT_PAGE_ENABLE);//set blue tooth hidden
    }
    else
    {
        //Set Visibility
        ret_val = BT_SetVisibility((BT_INQUIRY_ENABLE | BT_PAGE_ENABLE));
    }
    if ( BT_PENDING == ret_val)
    {
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_VISIBLE_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
    }
    else
    {
    	MMIDEFAULT_SetBackLight(TRUE);
#ifndef MMI_PDA_SUPPORT
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
#endif
    }
} 
/*****************************************************************************/
//  Discription: send next file with blue tooth 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SendNextFile(
                            	const wchar     *filepath_name,      //in
                            	uint16           filepath_len,       //in
                            	uint32           file_size,           //in
					BOOLEAN          is_temp_file        //in
                           )
{
	if (PNULL == filepath_name)
	{
		//SCI_TRACE_LOW:"MMIBT_SendNextFile PNULL == filepath_name!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_3882_112_2_18_2_8_8_241,(uint8*)"");
		return ;
	}
	//SCI_TRACE_LOW:"[&&&]MMIBT_SendNextFile start,file_size = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_3885_112_2_18_2_8_8_242,(uint8*)"d",file_size);
	//get file  name info       
	SCI_MEMSET(s_transfer_file_info.transfer_file_name, 0x00, sizeof(s_transfer_file_info.transfer_file_name));
	MMI_WSTRNCPY(s_transfer_file_info.transfer_file_name, MMIFILE_FULL_PATH_MAX_LEN, filepath_name, filepath_len, MIN(filepath_len, MMIFILE_FULL_PATH_MAX_LEN));
    s_transfer_file_info.is_temp_file = is_temp_file;
	//store send file size info
	s_transfer_file_info.total_file_size = file_size;
    MMIBT_SetOpertor(MMIBT_OPC_OPP_SEND, MMIBT_OPC_OPP_SEND);
	DoOperation(MMIBT_OPC_OPP_SEND,TRUE);
}

/*****************************************************************************/
//  Discription: send file with blue tooth
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_SendFile(
                            const wchar     *filepath_name,      //in
                            uint16           filepath_len,       //in
                            uint32           file_size,           //in
							BOOLEAN          is_temp_file        //in
                           )
{
    MMI_STRING_T         wait_text = {0};

    if (PNULL == filepath_name)
    {
        //SCI_TRACE_LOW:"MMIBT_SendFile PNULL == filepath_name!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_3917_112_2_18_2_8_8_243,(uint8*)"");
        return ;
    }
    if(MMIAPIBT_IsOpenFileWin())
	{
		MMIPUB_OpenAlertWarningWin(TXT_BT_TRANSFER);
		return;
	}
    if(MMK_IsOpenWin(MMI_BT_QUERY_WIN_ID))
    {
        MMK_SendMsg(MSG_APP_CANCEL, MMI_BT_QUERY_WIN_ID, PNULL);
        MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
    }
    //get file  name info       
    SCI_MEMSET(s_transfer_file_info.transfer_file_name, 0x00, sizeof(s_transfer_file_info.transfer_file_name));
    MMI_WSTRNCPY(s_transfer_file_info.transfer_file_name, MMIFILE_FULL_PATH_MAX_LEN, filepath_name, filepath_len, MIN(filepath_len, MMIFILE_FULL_PATH_MAX_LEN));
    s_transfer_file_info.is_temp_file = is_temp_file;

    //store send file size info
    s_transfer_file_info.total_file_size = file_size;

    SCI_TRACE_BUF("[MMIBT] [MMIBT_SendFile] transfer_file_name = ", s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)*sizeof(wchar));
    //check whether the blue tooth device is turn on
    if(BT_GetState())//blue tooth is power on
    {   
        //open paired blue tooth device
        MMIBT_EnterDeviceListWin(&s_transfer_file_info, 
                            MMIBT_OPC_OPP_SEND, 
                            BT_SERVICE_FTP);
    }
    else//blue tooth is power off
    {
        //power on BT first
        MMI_GetLabelTextByLang(TXT_TURN_ON_OR_NOT, &wait_text);
        OpenQueryWin(MMI_BT_QUERY_WIN_ID, &wait_text, PNULL, MMIBT_OPC_POWER_ONOFF);   
    }
}

/*****************************************************************************/
//  Discription: set if delete wallpaper file by fts
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL void  SetIsDelWallpaperFile(BOOLEAN   is_del_wallpaper)
{
    s_fts_del_wallpaper_file = is_del_wallpaper;
    //SCI_TRACE_LOW:"SetIsDelWallpaperFile: Set del file is wallpaper = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4000_112_2_18_2_8_8_244,(uint8*)"d",is_del_wallpaper);
}

/*****************************************************************************/
//  Discription: get if delete wallpaper file by fts
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL BOOLEAN  GetIsDelWallpaperFile(void)
{
    //SCI_TRACE_LOW:"GetIsDelWallpaperFile: Get file is wallpaper = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4010_112_2_18_2_8_8_245,(uint8*)"d",s_fts_del_wallpaper_file);
    return(s_fts_del_wallpaper_file);
}
#ifdef MMIBT_FTS_SUPPORT
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsDelReq(BT_MSG_T    *msg_body_ptr)
{
    BT_FILE_INFO    file_info  = {0}; 
    MMI_STRING_T    del_query_text = {0};
	MMI_STRING_T    name_text = {0};
    uint16          file_name_len = 0;
    uint16          file_path_len = 0;
    BOOLEAN       be_authorize = FALSE;

    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_FtsDelReq PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4030_112_2_18_2_8_8_246,(uint8*)"");
        return ;
    }

	file_info  = *((BT_FILE_INFO*) msg_body_ptr->body_ptr);
    if(MMK_IsFocusWin(MMIBT_SENDING_FILE_WAIT_WIN_ID) 
		|| MMK_IsFocusWin(MMIBT_RECEIVING_FILE_WIN_ID)
		|| MMIAPICC_IsInState(CC_MT_CONNECTING_STATE)
		|| MMIAPICC_IsInState(CC_MO_CONNECTING_STATE)
		)
    {
        //In sending or Recieving file, cancel delete atcion
        BT_DeleteFile(FALSE);
    }
    else
    {
        CloseSendWin();
        //del_query_text.wstr_ptr = del_buf;
        //turn on the back light
        MMIDEFAULT_SetBackLight(TRUE);
        //get prompt string: Delete + name ?
        //ConnectUSC2String(TXT_DELETE, file_info.name, TXT_NULL, (MMIBT_FULL_PATH_LEN + MMIBT_FTP_REQ_DEVICE_MAX_LEN), &del_query_text);
        //set if is delete wallpaper file 
        file_name_len = MMIAPICOM_Wstrlen((wchar*)file_info.name);
#if defined MMI_WALLPAPER_SUPPORT
        if(MMIAPIFMM_IsWallPaperFile((wchar*)file_info.name, file_name_len))
        {
            SetIsDelWallpaperFile(TRUE);
        }
#endif
        MMI_GetLabelTextByLang(TXT_BT_DELETE_QUERY, &del_query_text);
        name_text.wstr_ptr = file_info.name;
        name_text.wstr_len = file_name_len;
        MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_FTS_DLE);
        SCI_MEMSET(s_transfer_file_info.transfer_file_name, 0x00, sizeof(s_transfer_file_info.transfer_file_name));
        
        MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name,file_info.name,file_name_len);
        
        BT_GetDeviceAuthorisation(&(file_info.remote_dev.addr),&be_authorize);
        if(be_authorize)
        {
            DoOperation(MMIBT_OPC_FTS_DLE, TRUE);
        }
        else
        {   
            OpenQueryWin(MMI_BT_FTS_QUERY_WIN_ID, &del_query_text, &name_text, MMIBT_OPC_FTS_DLE); 
        }
    }
}
/*****************************************************************************/
//  Discription: the request of delete file/dir by FTP
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_FtsConnectReq(BT_MSG_T *msg_body_ptr)
{
    DoOperation(MMIBT_FTS_CONNECT_REQ,TRUE);   
}
#endif//MMIBT_FTS_SUPPORT
                                        
// @baokun fix cr96895 begin
/*****************************************************************************/
//  Discription: Handle no space for incoming file alert win msg
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleNoSpaceForFileAlertMsg(
                                               MMI_WIN_ID_T win_id, 
                                               MMI_MESSAGE_ID_E msg_id, 
                                               DPARAM param 
                                               )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE; 
	static uint8  timer_id = 0;
    
    switch (msg_id)
    {
	case MSG_OPEN_WINDOW:
	    timer_id = MMK_CreateTimer(MMIBT_VIBRA_DEFAULT_TIME, FALSE);
		result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);;
        MMIPUB_SetWinSoftkey(win_id, TXT_COMMON_OK, TXT_NULL, FALSE);
		break;
    case MSG_APP_RED:
        MMIBT_CancelFileTransfer(TRUE);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        MMIBT_OpenFMMDirectory();
		MMK_CloseWin(win_id);
        break;
     case MSG_LOSE_FOCUS:
		 break;
    case MSG_TIMER:
        if (timer_id == *(uint8*)param)
        {
            MMIBT_OpenFMMDirectory();
			MMK_CloseWin(win_id);
        }
		else
		{
			result = MMI_RESULT_FALSE;
		}
        //result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		break;
	case MSG_CLOSE_WINDOW:
		MMK_StopTimer(timer_id);
		timer_id = 0;
		result = MMI_RESULT_FALSE;
		break;
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);;
        break;
    }
    
    return result;             
} 
                                                
/*****************************************************************************/
//  Description : set ftp shared directory
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MMIBT_SETTING_RET MMIBT_OpenFMMDirectory(void)
{
    MMIFMM_FILTER_T filter = {0};
    uint32          path_len = 0;
    FILEARRAY       ret_array = NULL;
    MMIBT_SETTING_RET result = MMIBT_SUCCESS;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/

    if(MMIAPIUDISK_UdiskIsRun())
    {
        result = MMIBT_UDISK_RUNING;
    }

    if((MMIAPIFMM_GetFirstValidDevice() >= MMI_DEVICE_NUM))
    {
        result = MMIBT_UDISK_SDCARD_NOT_EXIST;
    }    
    
    //selected folder
    MMIBT_GetFileUserSetting(MMINV_BT_SHAIRED_DIRECTORY, &setting);
    path_len = MMIAPICOM_Wstrlen((wchar *)setting.dir.shaired_path_name);
    SCI_MEMCPY(filter.filter_str, "*", strlen("*"));/*lint !e666*/
    //get directory from FMM
    ret_array = MMIAPIFILEARRAY_Create();
    if(FMM_OPENWIN_SUCCESS != MMIAPIFMM_OpenFileWinByLayer(setting.dir.shaired_path_name, path_len, 
               &filter, FUNC_FIND_FOLDER,FALSE, NULL, ret_array, MMIBT_SetFileLocationCallBack,FALSE))
    {
        MMIAPIFILEARRAY_Destroy(&ret_array);
    }
    //SCI_TRACE_LOW:"MMIBT_OpenFMMDirectory: result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11309_112_2_18_2_8_23_361,(uint8*)"d",result);
    return result;  
}

/*****************************************************************************/
//  Discription: Handle send connect waiting win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSendConnectWindow(
                                             MMI_WIN_ID_T win_id, 
                                             MMI_MESSAGE_ID_E msg_id, 
                                             DPARAM param 
                                             )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    BT_STATUS   return_value = BT_PENDING;
    GUI_BOTH_RECT_T both_rect = {0};
	
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	    both_rect = MMITHEME_GetFullScreenBothRect();
        MMK_SetBothRect(win_id, &both_rect);
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
		recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
		MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_CANCEL, FALSE);
		recode = MMI_RESULT_FALSE;
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
    case MSG_APP_RED:
        //User refuse to accept the file
        return_value = MMIBT_CancelFileTransfer(TRUE);
        BtResumeBGPlay(MMIBT_OPP_SUSPEND);
         if(BT_PENDING == return_value)
         {
                //need wait
               OpenCancelWaitingWin();
         }
         else
         {
             if(BT_SUCCESS ==return_value)
             {
                MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);
             }
             else
             {
                //SCI_TRACE_LOW:"[&&&]HandleSendConnectWindow"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4464_112_2_18_2_8_9_250,(uint8*)"");
                MMIPUB_OpenAlertFailWin(TXT_COMM_SEND_FAILED);
             }
         }
#ifdef MMI_PDA_SUPPORT
         if(msg_id==MSG_APP_RED)
         {
             //need back to idle
            recode = MMI_RESULT_FALSE;
         }
         else
         {
            MMK_CloseWin( win_id );
         }
#else
         MMK_CloseWin( win_id );
#endif
        break;
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
		recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  

}                                      
/*****************************************************************************/
//  Discription: Handle receive file waiting win msg
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN  AssignReceiveFileCfg(MMIBT_FILE_SERVER_TYPE_E       service_type, BOOLEAN is_overwrite)
{
    BT_STATUS           return_result = BT_SUCCESS;     
    //wchar               file_full_path_ptr[MMIBT_FULL_PATH_LEN +1] = {0};
	MMI_STRING_T        query_text = {0};

    //SCI_TRACE_LOW:"AssignReceiveFileCfg service_type %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4542_112_2_18_2_8_9_251,(uint8*)"d",service_type);
    BtPauseBGPlay(MMIBT_FTP_SUSPEND);
    if (MMIBT_FILE_SERVER_OPP_RECIEVE == service_type)
    {
        //OPP Recieved
		if (!is_overwrite && MMIAPIFMM_IsFileExist(s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)))
		{
			
			MMI_GetLabelTextByLang(TXT_FILE_EXIST_OVERWRITE, &query_text);
			OpenQueryWin(MMI_BT_QUERY_WIN_ID, &query_text, PNULL, MMIBT_OPC_RECIEVE);  
			return FALSE;
		} 
		else
		{
			//call api to receive file
			return_result = BT_OppReceiveFile((uint16*)s_transfer_file_info.transfer_file_name);
		}
    }
    else if (MMIBT_FILE_SERVER_FTP_RECIEVE == service_type)
    {
        return_result = BT_FtpReceiveFile((uint16*)s_transfer_file_info.transfer_file_name);
    }
    if (BT_PENDING == return_result)
    {       

         MMIBT_SetTransferStatus(MMIBT_RECEIVEING_FILE_FOREGROUND);
		 MMIBT_UpdateMainMenu(TRUE);
        //set hidden for blue tooth for receive file
        //MMIBT_HiddenBtForAPP(MMIBT_MODULE_BLUETOOTH,BT_PAGE_ENABLE);
    }
    else
    {
        //receive file error
        BtResumeBGPlay(MMIBT_FTP_SUSPEND);
        MMIPUB_OpenAlertFailWin(TXT_COMMON_RECEIVE_FAIL); 
    }
	return TRUE;
}
/*****************************************************************************/
//  Discription: open the window of new file win 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_DeleteDeviceCnf( 
                                  BT_MSG_T *msg_body_ptr 
                                  )
{
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_DeleteDeviceCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4763_112_2_18_2_8_9_252,(uint8*)"");
#ifdef BT_DIALER_SUPPORT
        MMIBT_StopOperationWin(operation_info.current_opertion);
#endif
        return ;
    }
    //SCI_TRACE_LOW:"MMIBT_DeleteDeviceCnf: The action result is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4766_112_2_18_2_8_9_253,(uint8*)"d",msg_body_ptr->status);
    MMIDEFAULT_SetBackLight(TRUE);
    if (BT_SUCCESS == msg_body_ptr->status)
    {
		if (MMK_IsOpenWin( MMIBT_DELETE_DEVICE_WAIT_WIN_ID ))
		{
			//user delete device
			if(MMIBT_OPC_DEVICE_DELETE == operation_info.current_opertion)
			{
				//delete one cnf
				if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
				{
					MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
				}
				else if(MMK_IsOpenWin(MMIBT_HANDFREE_MENU_WIN_ID))
				{
					MMK_SendMsg(MMIBT_HANDFREE_MENU_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
				}
				
				{
					MMIBT_StopOperationWin(operation_info.current_opertion);						
				}
				
			}
			else if(MMIBT_OPC_DEVICE_DELETE_ALL == operation_info.current_opertion)
			{
				//delete all cnf, delete next or other operation
				DoOperation(operation_info.current_opertion,FALSE);
			}
			else
			{
				if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
				{
					MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
				}
				else if(MMK_IsOpenWin(MMIBT_HANDFREE_MENU_WIN_ID))
				{
					MMK_SendMsg(MMIBT_HANDFREE_MENU_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
				}
				
				MMIBT_StopOperationWin(operation_info.current_opertion);
			}
		}
    }
    else
    {
        if (MMK_IsOpenWin( MMIBT_DELETE_DEVICE_WAIT_WIN_ID ))
        {
            ClosePrmAndOpenDoneWin(
                                    MMIBT_DELETE_DEVICE_WAIT_WIN_ID,
                                    TXT_ERROR,
                                    IMAGE_PUBWIN_FAIL,
                                    MMI_2SECONDS
                                    );
#ifdef BT_DIALER_SUPPORT
            MMIBT_StopOperationWin(operation_info.current_opertion);
#endif
        }
    }
}
/*****************************************************************************/
//  Discription: open the window of new file win 
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_OpenNewReceivedFileWin( 
                                          BT_MSG_T *msg_body_ptr
                                          )
{   
    uint32        time_period = 0;
	MMI_WIN_ID_T alert_id = MMI_BT_NEW_FILE_WIN_ID;
    uint32       softkey_type = 0;  
    //turn on the back light
    MMIDEFAULT_SetBackLight(TRUE);
	MMK_SendMsg(MMIBT_RECEIVING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
	MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
//     MMIBT_RecoveryBtStatus(MMIBT_MODULE_BLUETOOTH);
    //if(!MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        if(MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name) > MMIFILE_FILE_NAME_MAX_LEN)
        {
            //file name too large can not open fmm
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_FILE_NAME_TOO_LARGE,TXT_NULL,IMAGE_PUBWIN_WARNING, &alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            
        }
        else
        {
            s_is_permit_open = Is_PermitOpenFile();
            if(!s_is_permit_open)
            {
                softkey_type = MMIPUB_SOFTKEY_ONE;
                time_period = MMI_3SECONDS;
            }
            else
            {
                softkey_type = MMIPUB_SOFTKEY_OKEXIT;
            }
            MMIPUB_OpenAlertWinByTextIdEx(
                MMK_GetFirstAppletHandle(),
                &time_period,
                TXT_SUCCESS,TXT_NULL,
                IMAGE_PUBWIN_SUCCESS,
                &alert_id,PNULL,                                            
                MMIPUB_SOFTKEY_CUSTOMER,
                HandleNewFileWinMsg, 
                softkey_type);
        }
    }
	
}


/*****************************************************************************/
//  Discription: delete one or all pair device 
//  Global resource dependence: none 
//  Author:maryxiao
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBTAPI_DeletePairedDevice(BOOLEAN is_all)
{    
}
/*****************************************************************************/
//  Discription: delete one or all pair device 
//  Global resource dependence: none 
//  Author:
//  Note:
//
/*****************************************************************************/
LOCAL void DeletePairedDevice(BOOLEAN is_all,MMIBT_OPERTATION_TYPE_E  current_opt)	
{
	uint16         select_index = 0;
	int            device_number = 0;
	int            i = 0;
    BOOLEAN        is_deleted = FALSE;
	BT_STATUS      delete_result = BT_ERROR;
	BT_DEVICE_INFO dev_info = {0};
	MMI_STRING_T   wait_text = {0};
	uint8          paired_device_index[MMIBT_MAX_DEV_ITEMS] = {0};

	//get paired device index
	if (MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
	{
		select_index =	GUILIST_GetCurItemIndex(MMIBT_DEVICELIST_LIST_CTRL_ID);
	}
	if(!is_all)
	{
		//delete one
		//first check this device permit to be delete
		MMIBT_StopOperationWin(current_opt);
        if(select_index == 0)
		{
			return;
		}
#ifdef MMI_PDA_SUPPORT
         if(select_index == 1)
		{
			return;
		}
        select_index = select_index-1;
#endif
        SCI_MEMSET(paired_device_index,0,sizeof(paired_device_index));        
        //get the paired device and append to the list 
        MMIBT_GetPairedDevList(MMIBT_GetCurrentSevice(), paired_device_index);  
        BT_GetPairedDeviceInfo(MMIBT_GetCurrentSevice(), paired_device_index[select_index - 1], &dev_info);
		if(MMIBT_DeviceIsConnectedHeadset(&dev_info.addr))
		{
			//can not  delete  connected headset
			MMIPUB_OpenAlertWarningWin(TXT_DELETE_HEADSET_ALERT);
			return;
		}
#ifdef BT_DIALER_SUPPORT
        else if(BT_HfuGetConnStatus(&dev_info.addr))
        {
            //can not  delete  connected phone
			MMIPUB_OpenAlertWarningWin(TXT_HFU_CONNECT_ALERT);
			return;
        }
#endif
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
		else if(MMIBT_DeviceIsConnectedFTP(&dev_info.addr))
        {
            //can not delete connected ftp device
			MMIPUB_OpenAlertWarningWin(TXT_CONNECT_FTP_ALERT);
			return;
        }
#endif
        else if(MMIBT_DeviceIsOnTransferStatus(&dev_info.addr,&s_transfer_file_info.device_info.addr))
        {
            //can not delete transfer file device
			MMIPUB_OpenAlertWarningWin(TXT_TRANSFER_FILE_ALERT);
			return;
        }
		else
		{
            //can be delete
            delete_result = BT_RemovePairedDevice( &dev_info.addr);
            is_deleted = TRUE;
		}
	
	}
	else
	{
        //delete all
#ifdef BT_DIALER_SUPPORT
        MMIBT_StopOperationWin(current_opt);
#endif
        device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);
		for(i =device_number; i>= 1; i--)
		{
			 delete_result = BT_GetPairedDeviceInfo(BT_SERVICE_ALL, i, &dev_info);
			if(!MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr))
#ifdef BT_DIALER_SUPPORT
                &&(!BT_HfuGetConnStatus(&dev_info.addr))
#endif
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
				&&(!MMIBT_DeviceIsConnectedFTP(&(dev_info.addr)))
#endif
                &&(!MMIBT_DeviceIsOnTransferStatus(&(dev_info.addr),&s_transfer_file_info.device_info.addr))
               )
			{
				delete_result = BT_RemovePairedDevice(&(dev_info.addr));
				is_deleted = TRUE;
                break;
			}
		}	
		if(!is_deleted)
		{
            //delete all fininshed 
			//update device list
			if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
			{
			   MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
			}
			if(device_number)
			{
				//exist not delete device
		        MMIPUB_OpenAlertSuccessWin(TXT_BT_DELETE_ALL_EXIST);
			}
			//SCI_TRACE_LOW:"[MMIBT] DeletePairedDevice, close MMIBT_DELETE_DEVICE_WAIT_WIN_ID"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4974_112_2_18_2_8_10_254,(uint8*)"");
//lzk mark
#ifdef BT_DIALER_SUPPORT
            //MMIBT_StopOperationWin(current_opt);
#else
            MMIBT_StopOperationWin(current_opt);
#endif

			return;
		}
		
	}
	
    //SCI_TRACE_LOW:"[MMIBT] DeletePairedDevice is_all %d delete_result %d is_deleted %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4981_112_2_18_2_8_10_255,(uint8*)"ddd", is_all, delete_result, is_deleted);
	if (BT_PENDING == delete_result && (!MMK_IsOpenWin(MMIBT_DELETE_DEVICE_WAIT_WIN_ID)))
	{			
		MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);		
		MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_DELETE_DEVICE_WAIT_WIN_ID,IMAGE_NULL,
			ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
        MMIBT_StartOperationWin(MMIBT_DELETE_DEVICE_WAIT_WIN_ID,current_opt,CreatePubWinCallBack);
	}
	else if(BT_PENDING != delete_result)
	{
		MMIPUB_OpenAlertFailWin(TXT_ERROR);
        MMIBT_StopOperationWin(current_opt);
	}						
}
/*****************************************************************************/
//  Discription: Handle blue tooth new file win msg
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleNewFileWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
	wchar               *file_name_ptr = PNULL;
    wchar               *path_name_ptr = PNULL;
    MMIPUB_INFO_T       *pubwin_info_ptr = PNULL;
	//static uint16       transfer_file_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};    

	uint16  path_len = 0;//MMIAPICOM_Wstrlen((uint16 *)s_transfer_file_info.transfer_path_name);
	uint16  file_len = 0;
    uint32  softkey_type = 0;
	//go to FMM dirctory	
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        pubwin_info_ptr = MMIPUB_GetWinAddDataPtr(win_id);
        if(PNULL != pubwin_info_ptr)
        {
            softkey_type = pubwin_info_ptr->user_data;
        }
        if(MMIPUB_SOFTKEY_OKEXIT == softkey_type)
        {

            MMIPUB_SetWinSoftkey(win_id, TXT_COMM_OPEN, STXT_EXIT, FALSE);
        }
        else
        {
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        }
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        //accept this operation
        if(s_is_permit_open)
        {	file_name_ptr = SCI_ALLOCA((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));    
            if(file_name_ptr != PNULL)
            {
                path_name_ptr = SCI_ALLOCA((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)); 
                if(path_name_ptr != PNULL)
                {
                    SCI_MEMSET(file_name_ptr, 0x00, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));
                    SCI_MEMSET(path_name_ptr, 0x00, ((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar)));
                    MMIAPICOM_GetLongFileNameByFullPath(s_transfer_file_info.transfer_file_name, file_name_ptr);
                    MMIAPICOM_GetLongFilePathNameByFullPath(s_transfer_file_info.transfer_file_name, path_name_ptr);
                     path_len = MMIAPICOM_Wstrlen((uint16 *)path_name_ptr);
                     file_len = MMIAPICOM_Wstrlen((uint16 *)file_name_ptr);
                    
                    if(MMIAPIFMM_IsFolderExist(path_name_ptr, path_len))
                    {
                        MMIAPIFMM_OpenExplorerExt((uint16 *)path_name_ptr, path_len,
                            file_name_ptr, file_len, TRUE,MMIFMM_FILE_ALL);
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_FOLDER_NOT_EXIST);
                    }
                    SCI_Free(path_name_ptr);
                }
                SCI_Free(file_name_ptr);                
            }
            MMK_CloseWin(win_id);
        }
        break;
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
		s_is_permit_open = FALSE;
		break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return result;     
}

/*****************************************************************************/
//  Discription: get FTP server request  
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note://对方要通过FTP取走文件的提示框
//
/*****************************************************************************/
PUBLIC void MMIBT_GetFtpReq(BT_MSG_T* msg_body_ptr)
{
    BT_FILE_INFO    file_info = {0};
    MMI_STRING_T    temp_str = {0}, query_text = {0};
    BOOLEAN       be_authorize = FALSE;
    
    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_GetFtpReq PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5405_112_2_18_2_8_11_257,(uint8*)"");
        return ;
    }

	file_info = *(( BT_FILE_INFO *) msg_body_ptr->body_ptr);
    if(MMIAPICC_IsInState(CC_MO_CONNECTING_STATE) || MMIAPICC_IsInState(CC_MT_CONNECTING_STATE))
    {
         s_transfer_file_info.server_type = MMIBT_FILE_SERVER_FTP_SEND;
         MMIBT_CancelFileTransfer(TRUE);
         //MMIBT_CancelTransfer(MMIBT_FILE_SERVER_FTP_SEND);
         return;
    }
	MMK_CloseWin(MMI_BT_ALERT_WIN_ID);
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_BT_FET_REQ_ALERT, &temp_str);
	query_text.wstr_ptr = file_info.name;
    query_text.wstr_len = MMIAPICOM_Wstrlen(file_info.name);
    //store ftp server req file info 
    SCI_MEMSET(&s_transfer_file_info, 0x00, sizeof(s_transfer_file_info));
    s_transfer_file_info.server_type = MMIBT_FILE_SERVER_FTP_SEND;
    SCI_MEMCPY(&s_transfer_file_info.device_info,&file_info.remote_dev,sizeof(BT_DEVICE_INFO));
    MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name, file_info.name, MIN(MMIAPICOM_Wstrlen(file_info.name), MMIBT_FULL_PATH_LEN));/*lint !e666*/
    s_transfer_file_info.total_file_size = file_info.size;
    //turn on the back light
	CloseSendWin();
    MMIDEFAULT_SetBackLight(TRUE);
    BT_GetDeviceAuthorisation(&(file_info.remote_dev.addr),&be_authorize);
    if(be_authorize)
    {
        DoOperation(MMIBT_OPC_FTP_SEND, TRUE);
    }
    else
    {
        OpenQueryWin(MMI_BT_FTS_QUERY_WIN_ID,&temp_str, &query_text, MMIBT_OPC_FTP_SEND);
    }
}

/*****************************************************************************/
//  Discription: Open the blue tooth incoming file win    
//  Global resource dependence: none 
//  Author:kelly.li
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_OpenIncomingFileWin(
                                       BT_MSG_T                     *msg_body_ptr,
                                       MMIBT_FILE_SERVER_TYPE_E     service_type
                                       )
                                       
{
    BT_FILE_INFO         file_info  = {0}; 
    uint16               name_len = 0;
    MMIBT_USER_SETTING_U setting = {0};/*lint !e64*/
    
    if ((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        //SCI_TRACE_LOW:"MMIBT_OpenIncomingFileWin PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5565_112_2_18_2_8_11_262,(uint8*)"");
        return;
    }    
    //接收文件有可能是两三个文件连续接收的，需要把之前一个新接收文件的提示窗口关掉

	file_info  = *((BT_FILE_INFO*) msg_body_ptr->body_ptr);
    name_len = MMIAPICOM_Wstrlen((wchar*)file_info.name);   
    //Set receive file server type
    if(name_len > MMIBT_FULL_PATH_LEN)
    {
        //file name too long
        //SCI_TRACE_LOW:"MMIBT_OpenIncomingFileWin:name_len is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5576_112_2_18_2_8_11_263,(uint8*)"d",name_len);
        MMIBT_CancelFileTransfer(TRUE);
        return;
    }
	MMK_CloseWin(MMI_BT_NEW_FILE_WIN_ID);
    MMK_CloseWin(MMI_BT_ALERT_WIN_ID);
    //if(MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT != MMIBT_GetOpertor().current_opertion)
    {	
        if(MMK_IsOpenWin(MMI_BT_QUERY_WIN_ID))
        {
            MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_APP_CANCEL, PNULL);
            MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
        }  
    }
    CloseSendWin();
    MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_RECIEVE);
    s_transfer_file_info.is_temp_file = FALSE;
    SCI_MEMSET(&s_transfer_file_info, 0x00, sizeof(s_transfer_file_info));  
    s_transfer_file_info.server_type = service_type;
    SCI_MEMSET(s_transfer_file_info.transfer_file_name,0x00, sizeof(s_transfer_file_info.transfer_file_name));
	
    //get file name and suffix info
    SCI_MEMCPY(&s_transfer_file_info.device_info, &file_info.remote_dev, sizeof(BT_DEVICE_INFO));
    SCI_MEMSET(s_transfer_file_info.device_info.name,0x00, sizeof(s_transfer_file_info.device_info.name));
    BT_GetPairedDeviceName(&(file_info.remote_dev.addr) ,s_transfer_file_info.device_info.name);
    s_transfer_file_info.total_file_size = file_info.size;
    if(MMIBT_FILE_SERVER_OPP_RECIEVE == s_transfer_file_info.server_type)
    {
        wchar  file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};   //912951 后缀名错误
        uint16 path_name_len= 0;

        //opp receive file
        MMIBT_GetFileUserSetting(MMINV_BT_FILE_LOCATION, &setting);
         path_name_len = name_len;

         //remove file path info, and reserve the filename,fix bug711918
         do
        {
              if(file_info.name[path_name_len-1]=='/') break;
              path_name_len--;
        } while(path_name_len);

        MMIAPICOM_Wstrncpy(file_name, &file_info.name[path_name_len], MIN((name_len - path_name_len), MMIFILE_FULL_PATH_MAX_LEN));

        MMIAPICOM_CombineDefaultFullPathByFilename(setting.filedev,
            file_name,
            MMIAPICOM_Wstrlen(file_name),
            s_transfer_file_info.transfer_file_name,
            MMIFILE_FULL_PATH_MAX_LEN
            );
    }
    else
    {
        //ftp receive file
        MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name, file_info.name, name_len);
        setting.filedev = MMIAPIFMM_GetDeviceTypeByPath(
                            s_transfer_file_info.transfer_file_name, 
                            MMIFILE_DEVICE_LEN
                            );
    }
    
    //all disk not exist
    if(MMIAPIUDISK_UdiskIsRun())
    {
        //usb runing
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING); 
        MMIBT_CancelFileTransfer(TRUE);
        
    }
    else if(!MMIAPIUDISK_IsDeviceReady())
    {
        //all disk not exist       
        MMIBT_CancelFileTransfer(TRUE);
    }
    else if(MMIBT_IsEnoughSpaceToRecieve(setting.filedev))
    {       
        //memory enough
        //overwrite file, space enough
        s_transfer_file_info.filedev = setting.filedev;
	  SCI_MEMSET(&g_current_bt,0x00, sizeof(BT_DEVICE_INFO));	
	  SCI_MEMCPY(&g_current_bt, &file_info.remote_dev, sizeof(BT_DEVICE_INFO));       
        CheckAuthorizeAndOpenIncomingWin();
    }
    else
    {
        MMI_TEXT_ID_T text_id = TXT_CMSBRW_UDISK_FULL_CHANGE_STORE;
        MMI_WIN_ID_T  alert_id = MMI_BT_ALERT_WIN_ID;

        if(MMI_DEVICE_UDISK != setting.filedev)
        {
            text_id = TXT_CMSBRW_SD_FULL_CHANGE_STORE;
        }        
        if(MMIBT_FILE_SERVER_OPP_RECIEVE == s_transfer_file_info.server_type)
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleNoSpaceForFileAlertMsg);
        }
        else
        {
            //ftp need not auto change device
            DisplayMemoryInformation(setting.filedev);
            MMIBT_CancelFileTransfer(TRUE);
        }
    }   
}

/**--------------------------------------------------------------------------*
**                          BLUETOOTH FUCNTIONS NOT IN WIN TAB
**                          BlueTooth Interface Functions implement
**                          *I**N**T**E**R**F**A**C**E*
**--------------------------------------------------------------------------**/
/*****************************************************************************/
//  Discription: receive bt on cnf 
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetBtOnOffCnf( BT_MSG_T     *msg_body_ptr, BOOLEAN is_on)
{
    MMIBT_OPERATION_INFO_T  bt_opc = MMIBT_GetOpertor();
    uint32          node_id = 0;       //node id,from 1
    MMI_STRING_T    node_text = {0};
    MMI_TEXT_ID_T   text_id = TXT_NULL;
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_IMAGE_ID_T              imgae_id = IMAGE_NULL;
    BOOLEAN                     isgray = FALSE;
#endif
    MMI_TEXT_ID_T          left_text_id = TXT_NULL;
        
    //SCI_TRACE_LOW:"MMIBT_GetBtOnOffCnf status %d current_opertion %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5692_112_2_18_2_8_11_264,(uint8*)"dd",msg_body_ptr->status, bt_opc.current_opertion);
	
	MMIAPISET_FuncFinishedInd();
	if(MMIAPISET_GetFlyMode() && is_on)
	{
		//in fly mode need close bt
		MMIBT_OpenBluetooth(FALSE, FALSE);
	}
	else
	{
		//update blue tooth idle icon
		MMIBT_UpdateBtIdleIcon(BT_GetState(),MMIBT_GetIsDeviceVisible());
	}
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        BT_UpdateNV();
        if(is_on)
        {
            //start cnf
            text_id = TXT_SET_BT_OFF;
            left_text_id = TXT_COMMON_CLOSE;
            node_id = ID_SET_BT_OFF;
#ifndef MMI_GUI_STYLE_TYPICAL
            imgae_id = IMAGE_CHECK_SELECTED_ICON;
            isgray   =FALSE;
#endif
            //set bt on confirm
            SetFtpRootFolderAfterPowerOn();
#ifdef BT_BQB_SUPPORT
            //+++ mdy by qing.yu@for cr238585
            MMIAPIBT_SetNetWorkRoamStatus(MMIAPIPHONE_GetIsNetworkRoaming());
            MMIAPIBT_SetOperatorName();		
//lzk mark			
#ifdef BT_DIALER_SUPPORT	
/*            MMIAPIBT_SetMyPhoneNumber();*/
#else
            MMIAPIBT_SetMyPhoneNumber();
#endif
			//--- mdy by qing.yu@for cr238585
#endif //BT_BQB_SUPPORT            
            if(MMIBT_OPC_AUTO_TEST == bt_opc.root_opertion)
            {
                //auto test
                MMIBT_SetOpertor(MMIBT_OPC_NOT_CHANGE, MMIBT_OPC_SEARCH);
                MMIBT_OpenSearchDeviceWaitWin(BT_SERVICE_ALL);
            }
            else if(MMIBT_OPC_OPP_SEND == bt_opc.root_opertion)
            {                
                DoOperation(MMIBT_OPC_LIST_PAIRED,FALSE);
            }
            else if(MMIBT_OPC_POWER_ONOFF != bt_opc.current_opertion)
            {
                //not power on from setting, so do current option directly
                DoOperation(bt_opc.current_opertion, FALSE);
            }
            else
            {
                if(MMK_IsOpenWin(MMIBT_BTSTART_WAIT_WIN_ID))
                {
                    //finished opertion 
                    MMIDEFAULT_SetBackLight(TRUE);
                }
            }
#ifdef BT_DIALER_SUPPORT			
            StartAutoConnectHandsetGatewayTimer();
#endif
        }   
        else
        {
            text_id = TXT_BT_SET_ON;
            left_text_id = TXT_OPEN;
            node_id = ID_SET_BT_ON;
#ifndef MMI_GUI_STYLE_TYPICAL
            imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
            isgray   = TRUE;
#endif
            //close bt cnf
            if(MMK_IsOpenWin(MMIBT_BTSTART_WAIT_WIN_ID))
            {
                MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
                if(MMIBT_OPC_AUTO_TEST != bt_opc.root_opertion)                
                {
                    //close BT confirm
                    MMIDEFAULT_SetBackLight(TRUE);
                }
				 MMK_CloseWin( MMIBT_BTSTART_WAIT_WIN_ID);
            }
        }

        if(MMK_IsOpenWin(MMIBT_MAIN_MENU_WIN_ID))
        {
#ifndef MMI_GUI_STYLE_TYPICAL
            MMIBT_AppendOneLineTextIconListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_BLUETOOTH,imgae_id,BT_INDEX_OPEN,left_text_id,TRUE);           
            MMIBT_UpdateMainMenu(TRUE);
#else
            //update main menu
            MMI_GetLabelTextByLang(text_id, &node_text);
            GUIMENU_ReplaceNode(0, node_id, 0, &node_text, MMIBT_MAIN_MENU_CTRL_ID);
#endif
        }
        if(TRUE == MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
        {
            //BTMainUpdateUI(is_on);
        }
#ifdef BT_DIALER_SUPPORT
		if(MMK_IsOpenWin(MMIBT_BTPHONE_WIN_ID))
        {
			MMIBT_UpdateBtPhoneWin(TRUE);
        }
#endif
        //close waiting win
        MMK_CloseWin(MMIBT_BTSTART_WAIT_WIN_ID);
        
    }
    else
    {
        //open and close fail
        if(MMIBT_OPC_AUTO_TEST == bt_opc.root_opertion)
        {
            MMIDEFAULT_SetBackLight(TRUE);
            //auto test
            if(is_on)
            {
                MMIBT_CreateUitestWin();
            }
            else
            {
                //MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);			
            }
        }
        else if(MMIBT_OPC_INIT != bt_opc.root_opertion)
        {
            MMIDEFAULT_SetBackLight(TRUE);
#ifndef MMI_PDA_SUPPORT
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
#endif
            //MMK_CloseWin(MMIBT_BTSTART_WAIT_WIN_ID);
        }   
		MMK_CloseWin( MMIBT_BTSTART_WAIT_WIN_ID);
    }
    //Invoke Event    
}


/*****************************************************************************/
//  Discription: receive bt visible cnf 
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetVisibleCnf(BT_MSG_T *msg_body_ptr, BOOLEAN is_visible)
{
#ifndef MMI_GUI_STYLE_TYPICAL
    MMI_IMAGE_ID_T              imgae_id = IMAGE_NULL;
#endif
    MMI_TEXT_ID_T          left_text_id = TXT_NULL;
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_GetVisibleCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_5854_112_2_18_2_8_11_265,(uint8*)"");
        return;
    }
    if (BT_SUCCESS == msg_body_ptr->status)
    {
        BT_UpdateNV();
        MMIBT_UpdateBtIdleIcon( BT_GetState(), is_visible);
        
        if (MMK_IsOpenWin( MMIBT_VISIBLE_WAIT_WIN_ID ))
        {
            MMIDEFAULT_SetBackLight(TRUE);
            MMK_CloseWin(MMIBT_VISIBLE_WAIT_WIN_ID);
        }
    }
    else
    {
#ifdef MMI_GUI_STYLE_TYPICAL
        if(MMK_IsOpenWin( MMIBT_SET_VISIBILITY_WIN_ID ))

        {
            if(MMIBT_GetIsDeviceVisible())
            {
                GUILIST_SetCurItemIndex(MMIBT_SET_VISIBILITY_CTRL_ID,0);
                GUILIST_SetSelectedItem(MMIBT_SET_VISIBILITY_CTRL_ID, 0, TRUE);
            }
            else
            {
                GUILIST_SetCurItemIndex(MMIBT_SET_VISIBILITY_CTRL_ID,1);
                GUILIST_SetSelectedItem(MMIBT_SET_VISIBILITY_CTRL_ID, 1, TRUE);
            }
        }
#endif
        if (MMK_IsOpenWin( MMIBT_VISIBLE_WAIT_WIN_ID ))
        {
            MMIDEFAULT_SetBackLight(TRUE);
            ClosePrmAndOpenDoneWin(
                                MMIBT_VISIBLE_WAIT_WIN_ID,
                                TXT_ERROR,
                                IMAGE_PUBWIN_FAIL,
                                MMI_2SECONDS
                                );  
        }   
    } 
#ifndef MMI_GUI_STYLE_TYPICAL
    if(MMIBT_GetIsDeviceVisible())
    {
        imgae_id = IMAGE_CHECK_SELECTED_ICON;
        left_text_id = TXT_COMMON_CLOSE;
    }
    else
    {
        imgae_id = IMAGE_CHECK_UNSELECTED_ICON;
         left_text_id = TXT_OPEN;
    }
    MMIBT_AppendOneLineTextIconListItem(MMIBT_MAIN_LIST_CTRL_ID,TXT_MY_BT_VISIBILITY,imgae_id,BT_INDEX_VISIBLE,left_text_id,TRUE);
    if(MMK_IsFocusWin(MMIBT_MAIN_MENU_WIN_ID))  //bug920735
    {
        MMK_SendMsg(MMIBT_MAIN_MENU_WIN_ID, MSG_FULL_PAINT, PNULL);
    }
#ifdef BT_DIALER_SUPPORT
    MMIBT_AppendOneLineTextIconListItem(MMIBT_SETTING_LIST_CTRL_ID,TXT_MY_BT_VISIBILITY,imgae_id,BT_SETTING_INDEX_VISIBLE,left_text_id,TRUE);
#endif
#endif
}
/*****************************************************************************/
//  Discription: get inquired device complete confirm
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void  MMIBT_GetInqCompCnf(void)
{
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
    AnimatePlayCtrlByState(MMIBT_ANIM_STOP);
    MMIDEFAULT_AllowOpenKeylocWin(TRUE);
    if (MMK_IsOpenWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID))
    {
        //no device fond
        //turn on the back light
        MMIDEFAULT_SetBackLight(TRUE);

        MMIPUB_OpenAlertWarningWin(TXT_BT_NO_DEVICE);
		MMK_SendMsg(MMIBT_SEARCH_DEVICE_PUBWIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
		MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
        MMK_CloseWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID);
#ifdef BT_DIALER_SUPPORT
        MMIBT_StopOperationWin(MMIBT_OPC_SEARCH);
#endif
    }
    else if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
    {
        //inform search list
        MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_FINISHED, PNULL);
		MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
    }else if(MMK_IsOpenWin(MMIBT_MAIN_WIN_ID))
	{
		MMK_SendMsg(MMIBT_MAIN_WIN_ID, MSG_BT_SEARCH_FINISHED, PNULL);
	}
    //进行蓝牙UI TesT测试，搜索结束消息
    if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
    {
        MMIBT_CreateUitestWin();
        MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
        MMK_CloseWin(MMIENG_UITESTBLUETOOTH_WIN_ID);
    }
}

/*****************************************************************************/
//  Discription: send file to other device failed ind
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SendFileFailedInd(void)
{   
    //send file fail
    //SCI_MEMSET(&g_current_bt, 0x00, sizeof(g_current_bt));
	MMK_CloseWin(MMI_BT_ALERT_WIN_ID);
    BtResumeBGPlay(MMIBT_FTP_SUSPEND);

    //if(MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID) || MMK_IsOpenWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID))
    {
		MMIDEFAULT_SetBackLight(TRUE);
		//if(!MMIAPICC_IsInState(CC_IN_CALL_STATE))
		{
		    //SCI_TRACE_LOW:"[&&&]MMIBT_SendFileFailedInd"
		    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_6948_112_2_18_2_8_14_284,(uint8*)"");
			MMIPUB_OpenAlertFailWin(TXT_COMM_SEND_FAILED);
		}
        MMK_CloseWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID);
		MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
//     MMIBT_RecoveryBtStatus(MMIBT_MODULE_BLUETOOTH);
	MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
    MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
}

/*****************************************************************************/
//  Discription: send file to other device failed ind
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_TransferCancelled(BOOLEAN is_send)
{   
    MMIDEFAULT_SetBackLight(TRUE);
    BtResumeBGPlay(MMIBT_FTP_SUSPEND);
    MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);    
    if(is_send) 
    {
        //send file canceled
		MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
    else
    {
        //recevied file canceled 
		MMIBT_UpdateCurrentFileList();
		MMK_SendMsg(MMIBT_INCOMING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
		MMK_SendMsg(MMIBT_RECEIVING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
    }
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
    MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
    
}

/*****************************************************************************/
//  Discription: get receive file fail indication
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_ReceiveFileFail(void)
{
    //reset blue tooth status
    BtResumeBGPlay(MMIBT_FTP_SUSPEND);
    MMIBT_UpdateCurrentFileList();
    UpdateCurrentPictureViewList();
	MMK_CloseWin(MMI_BT_ALERT_WIN_ID);
    //SCI_TRACE_LOW:"MMIBT_ReceiveFileFail s_transfer_file_info.server_type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7067_112_2_18_2_8_14_286,(uint8*)"d",s_transfer_file_info.server_type);

    MMIDEFAULT_SetBackLight(TRUE);
    MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);
    if(MMK_IsOpenWin(MMIBT_SENDING_FILE_WAIT_WIN_ID) || MMK_IsOpenWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID))
    {			 
        MMK_CloseWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID);
        MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
    }
    //receive file error    
    else if ( MMK_IsOpenWin(MMIBT_RECEIVING_FILE_WIN_ID))
    {
		MMIBT_UpdateCurrentFileList();
		MMK_SendMsg(MMIBT_RECEIVING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
    }   
    if (MMK_IsOpenWin(MMIBT_INCOMING_FILE_WIN_ID))
    {
		MMIBT_UpdateCurrentFileList();
		MMK_SendMsg(MMIBT_INCOMING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
        MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
    }
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
    MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
    MMK_CloseWin(MMI_BT_FTS_QUERY_WIN_ID);
}

/*****************************************************************************/
//  Discription: cancel blue tooth ftp function when is in call(MT & MO)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_CancelFileTransfer(BOOLEAN is_close_query)
{
     BT_STATUS bt_status = 0;

    if (MMIBT_FILE_SERVER_OPP_SEND == s_transfer_file_info.server_type)
    {
        bt_status = BT_CancelOppSendFile();
    }
    else if (MMIBT_FILE_SERVER_OPP_RECIEVE == s_transfer_file_info.server_type)    
    {
        bt_status = BT_CancelOppReceiveFile();
    }
    else if (MMIBT_FILE_SERVER_FTP_RECIEVE== s_transfer_file_info.server_type)
    {
        bt_status = BT_CancelFtpTransfer();
    }
    else if (MMIBT_FILE_SERVER_FTP_SEND == s_transfer_file_info.server_type)
    {
        bt_status = BT_CancelFtpTransfer();
    }
    if(BT_PENDING != bt_status)
    {
      s_transfer_file_info.server_type = MMIBT_FILE_SERVER_MAX;
	  if(BT_SUCCESS == bt_status)
	  {
		  //cancel success
		  MMIBT_UpdateCurrentFileList();
		  MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);		  
	  }
	  else
	  {
		  //cancel error
		  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7151_112_2_18_2_8_14_287,(uint8*)"");
		  MMIPUB_OpenAlertFailWin(TXT_ERROR);            
	  }
	  MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
      MMK_CloseWin(MMIBT_SENDING_FILE_WAIT_WIN_ID);
	  MMK_SendMsg(MMIBT_RECEIVING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
      MMK_CloseWin(MMIBT_RECEIVING_FILE_WIN_ID);
	  MMK_SendMsg(MMIBT_INCOMING_FILE_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
      MMK_CloseWin(MMIBT_INCOMING_FILE_WIN_ID);
      MMK_CloseWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID);	  
    }  
	if(is_close_query)
	{
		MMK_SendMsg(MMI_BT_QUERY_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
		MMK_CloseWin(MMI_BT_QUERY_WIN_ID);
		MMK_CloseWin(MMI_BT_FTS_QUERY_WIN_ID);
	}

    return bt_status;
}

/*****************************************************************************/
//  Discription: receive bt hidden cnf 
//  Global resource dependence: None
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void  MMIBT_CancelSearchCnf(BT_MSG_T *msg_body_ptr)
{
    BT_STATUS return_value= BT_SUCCESS;
    MMI_TEXT_ID_T ui_text = TXT_NULL;
    MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
    MMIDEFAULT_SetBackLight(TRUE);
    if(PNULL != msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_CancelSearchCnf  status is = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7185_112_2_18_2_8_14_288,(uint8*)"d",msg_body_ptr->status);
        
        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            //inform search list
            MMK_SendMsg(MMIBT_INQUIRED_DEV_LIST_WIN_ID, MSG_BT_SEARCH_CANCELED, PNULL);
        }
        else if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
        {
            //finished error
            if(!MMK_IsOpenWin(MMIBT_UITEST_RESULT_WIN_ID) 
                && (MMK_IsOpenWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID)
                ||MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID)
                ||MMK_IsOpenWin(MMIENG_BT_CANCEL_WAIT_WIN_ID)
                )
                )
            {
                if(BT_SUCCESS == msg_body_ptr->status)
                {
                    ui_text = TXT_UITEST_SUCCESS_BT;
                }
                else
                {
                    ui_text = TXT_UITEST_FAIL_BT;
                }
                MMIBT_CreateUitestWin();
            }
        }
        if(BT_SUCCESS == msg_body_ptr->status)
        {
            if (MMK_IsOpenWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID))
            {
                //start seaching device
                return_value = BT_SearchDevice(MMIBT_GetCurrentSevice());
            }
            else
            {
                //real cancel
                MMIDEFAULT_AllowOpenKeylocWin(TRUE);
                BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
            }
        }
        else
        {
            //cancel search error !
            if(MMIBT_OPC_AUTO_TEST != operation_info.root_opertion)
            {
                MMIPUB_OpenAlertFailWin(TXT_ERROR);   
            }
			MMK_SendMsg(MMIBT_SEARCH_DEVICE_PUBWIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
            MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
            MMK_CloseWin(MMIBT_SEARCH_DEVICE_PUBWIN_ID); 
            BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
        }
    }

    if(MMIBT_OPC_AUTO_TEST == operation_info.root_opertion)
    {
        MMK_CloseWin(MMIENG_UITESTBLUETOOTH_WIN_ID);
    }

    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
#ifdef BT_DIALER_SUPPORT
    MMIBT_StopOperationWin(MMIBT_OPC_SEARCH);
#endif
}
/*****************************************************************************/
//  Discription: phone is ready or not
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsPhoneReady(void)
{
    BOOLEAN               is_ready = FALSE;
    if (!MMK_IsOpenWin(PHONE_POWER_OFF_WIN_ID) 
        && (!MMK_IsOpenWin(PHONE_STARTUP_NORMAL_WIN_ID))
        )
    {
        is_ready = TRUE;
    }
    //SCI_TRACE_LOW:"MMIBT_IsPhoneReady is_ready %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7816_112_2_18_2_8_16_292,(uint8*)"d",is_ready);
    return is_ready;
}

/*****************************************************************************/
//  Discription: open or close BT
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenBluetooth(BOOLEAN set_on, BOOLEAN is_need_cnf)
{
	BOOLEAN    result = FALSE;
    BT_STATUS  ret_val = BT_SUCCESS;
    BT_MSG_T   msg_body= {0};
    MMI_STRING_T wait_text = {0};
    MMIBT_OPERATION_INFO_T opt = MMIBT_GetOpertor();
    if(set_on)
    {
        //启动蓝牙
        if(MMIAPISET_GetFlyMode())// fly mode on
        {
            //not permit to open BT 
            if(MMIBT_OPC_AUTO_TEST == opt.root_opertion)
            {
                //auto test finished bt test and run next step
                MMIBT_CreateUitestWin();
                //GUILABEL_SetTextById(MMIBT_UI_RESULT_LABEL_CTRL_ID,TXT_CLOSE_FLY_MODE_FIRST,FALSE);
            }
            else
            {
                MMIPUB_OpenAlertWarningWin(TXT_CLOSE_FLY_MODE_FIRST); 
            }
            return result;
        }
        else if(!BT_GetState())
        {
            ret_val = BT_Start();
        }
    }
    else 
    {
    	//首先检测有无文件在传输(OPP/FTP)，如果正在传输文件，则提示用户先自行取消
        if(MMIAPIBT_IsOpenFileWin())
        {
            MMIPUB_OpenAlertWarningWin(TXT_BT_CLOSE_WARNING);
            return result;
        }
		else
		{
			if(BT_GetState())
	        {
	            ret_val = BT_Stop();
	        }   
		}
    }
    //SCI_TRACE_LOW:"[MMIBT]MMIBT_OpenBluetooth set_on %d ret_val %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_7999_112_2_18_2_8_16_295,(uint8*)"dd",set_on, ret_val);
    //打开等待窗口;
    if(BT_PENDING == ret_val && is_need_cnf)
    {
     
       MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
       MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_BTSTART_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
    }
    else
    {
        //need not wait callback
        if(is_need_cnf)
        {
            msg_body.status = ret_val;
            MMIBT_GetBtOnOffCnf(&msg_body, set_on);
        }
    }
	if(BT_PENDING == ret_val)
	{
		result = TRUE;
	}
    return result;
}



/*****************************************************************************/
//  Discription: Handle query win msg 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMI_RESULT_E HandleQueryWinMsg(
                                           MMI_WIN_ID_T win_id, 
                                           MMI_MESSAGE_ID_E msg_id, 
                                           DPARAM param 
                                           )    
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE; 
	static BOOLEAN  s_is_cancelled = FALSE;
	static MMIBT_OPERTATION_TYPE_E s_quer_opc_arr[MMIBT_MAX_QUERY_NUM] = {0};
	static uint8  s_current_query_num = 0;
	MMIBT_OPERTATION_TYPE_E  current_opc = MMIBT_OPC_INIT;
     wchar               file_full_path_ptr[MMIBT_FULL_PATH_LEN +1] = {0};
     wchar               vp_file_full_path_ptr[MMIBT_FULL_PATH_LEN +1] = {0};//add by CR233033
     MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();

	//SCI_TRACE_LOW:"[MMIBT] HandleQueryWinMsg win_id 0x%x, msg_id 0x%x s_current_query_num %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8123_112_2_18_2_8_16_299,(uint8*)"ddd", win_id, msg_id, s_current_query_num);

    switch(msg_id) 
    {   
    case MSG_OPEN_WINDOW:
		s_is_cancelled = FALSE;
		if(s_current_query_num < MMIBT_MAX_QUERY_NUM)
		{
			s_quer_opc_arr[s_current_query_num]= MMIBT_GetInterruptOper();
			s_current_query_num ++;
		}
        //StartVibrator(win_id);
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
            
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //accept this operation
		s_is_cancelled = TRUE;
		if(s_current_query_num > 0 && (s_current_query_num <= MMIBT_MAX_QUERY_NUM))
		{
			current_opc = s_quer_opc_arr[s_current_query_num -1];
		}
		else
		{
			current_opc = MMIBT_GetInterruptOper();
		}
        //SCI_TRACE_LOW:"OpenQueryWin MSG_APP_OK current_opc %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8162_112_2_18_2_8_16_300,(uint8*)"d",current_opc);
		if(MMIBT_OPC_RECIEVE == current_opc)
		{
            
            #ifdef VIDEO_PLAYER_SUPPORT
           
            MMIAPIVP_GetPlayingFilename(vp_file_full_path_ptr);//add by CR233033

            #endif
            
            //SCI_TRACE_LOW:"MMIBT file name %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8175_112_2_18_2_8_16_301,(uint8*)"s", file_full_path_ptr);
			//if(SFS_ERROR_NONE != MMIAPIFMM_DeleteFile(file_full_path_ptr, PNULL))//MARYXIAO CR230219
            if(IsFileUsed(s_transfer_file_info.transfer_file_name)||0 == MMIAPICOM_Wstrcmp(s_transfer_file_info.transfer_file_name,vp_file_full_path_ptr))//modified by CR233033
            {
                MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_USING);
                CancelOperation(current_opc);
                result = MMI_RESULT_FALSE;
            }
			else if(AssignReceiveFileCfg(s_transfer_file_info.server_type, TRUE))
			{
				//enter receiving file win
				MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                               s_transfer_file_info.server_type,
                               s_transfer_file_info.transfered_size,
                               s_transfer_file_info.total_file_size);
			}		
		}        
		else
		{
            if(MMIBT_OPC_FTS_DLE == current_opc)
            {                
                if(IsFileUsed(s_transfer_file_info.transfer_file_name))
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COM_FILE_USING);
                    CancelOperation(current_opc);
                    result = MMI_RESULT_FALSE;
                }
            }
            if(MMI_RESULT_TRUE == result)
            {
			    DoOperation(current_opc, IsNeedBTOn(current_opc));
            }
		}
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
		s_is_cancelled = TRUE;
        //cancel this operation
		if(s_current_query_num > 0 && (s_current_query_num <= MMIBT_MAX_QUERY_NUM))
		{
			current_opc = s_quer_opc_arr[s_current_query_num -1];
		}
		else
		{
			current_opc = MMIBT_GetInterruptOper();
		}
		if(MMIBT_OPC_OPP_SEND == operation_info.root_opertion)
		{
			if(s_transfer_file_info.is_temp_file)
			{
				MMIAPIFMM_DeleteFile(s_transfer_file_info.transfer_file_name, PNULL);
			}
		}		
        CancelOperation(current_opc);
        result = MMI_RESULT_FALSE;
        MMK_CloseWin(win_id);
        break;
	case MSG_BT_AUTO_CLOSE_WIN:
        s_is_cancelled = TRUE;
		break;
    case MSG_CLOSE_WINDOW:
		if(!s_is_cancelled)
		{
			if(s_current_query_num > 0 && (s_current_query_num <= MMIBT_MAX_QUERY_NUM))
			{
				current_opc = s_quer_opc_arr[s_current_query_num -1];
			}
			else
			{
				current_opc = MMIBT_GetInterruptOper();
			}
			CancelOperation(current_opc);
			if(MMIBT_OPC_OPP_SEND == operation_info.root_opertion)
			{
				if(s_transfer_file_info.is_temp_file)
				{
					MMIAPIFMM_DeleteFile(s_transfer_file_info.transfer_file_name, PNULL);
				}
			}
		}
		if(s_current_query_num > 0)
		{
			s_current_query_num --;
		}
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return result;
}

/*****************************************************************************/
//  Discription: do operation by  MMIBT_OPERTATION_TYPE_E
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN DoOperation(MMIBT_OPERTATION_TYPE_E  current_opt, BOOLEAN is_poweron_first)
{    
   BT_STATUS return_value = 0;
   MMI_STRING_T disp_text1 = {0}; /*lint !e64*/
   MMI_STRING_T disp_text2 = {0}; /*lint !e64*/
   //wchar        *text1_buf_ptr = PNULL;
   wchar        *text2_buf_ptr = PNULL;
#if (defined BT_HL_PATCH_SUPPORT) || (!defined BT_CHIP_NONE)
   BT_ADDRESS addr = {0};
#endif   
   BT_DEVICE_INFO  pin_request_info = {0};
#ifdef BT_DIALER_SUPPORT
   BT_ADDRESS   dev_addr = {0};
#endif
   
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8285_112_2_18_2_8_17_302,(uint8*)"dd",current_opt, is_poweron_first);
  
   if(is_poweron_first && (!BT_GetState()))
   {
      //power on bt first
       MMIBT_OpenBluetooth(TRUE, TRUE);
       return TRUE;
   }
   switch(current_opt)
   {
   case MMIBT_OPC_POWER_ONOFF:
       //open bt
       MMIBT_OpenBluetooth(TRUE, TRUE);
       break;
   case MMIBT_OPC_SEARCH:
        //search
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
        //HL added it for not support search while blue headset connected
        if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ACTIVE_HEADSET_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
		//add for not support search while ftp connected
		if(MMIBT_IsConnectedFTP())
		{
			MMIPUB_OpenAlertWinByTextId(PNULL,TXT_CONNECT_FTP_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);	
			break;
		}
		//added tonny.chen for HL bt
        if(MMIBT_IsOnTransferStatus())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_TRANSFER_FILE_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
        //end added 
#endif
        
#ifdef BT_DIALER_SUPPORT
        if(MMIBT_IsConnectDevice(&dev_addr))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#endif

        //pause BG play
        BtPauseBGPlay(MMIBT_SEARCH_SUSPEND);
        //open waiting win
        MMIBT_OpenSearchDeviceWaitWin(MMIBT_GetCurrentSevice()); 
        break;
   case MMIBT_OPC_VISIBILITY:
       MMIBT_EnterSetVisibilityWin();
       break;
#ifdef MMIBT_FTS_SUPPORT       
   case MMIBT_OPC_SHARED_FILE_SETTING:
        //shared file setting
        MMIBT_EnterSharedWin();
       break;
#endif
   case MMIBT_OPC_OPP_SEND:
       //send file
#if (defined BT_HL_PATCH_SUPPORT) || (!defined BT_CHIP_NONE)
       //HL added it for not support search while blue headset connected
        if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ACTIVE_HEADSET_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
		//added by tonnychen for HL bt
		if(MMIBT_IsOnTransferStatus())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_TRANSFER_FILE_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
		//end added 
#endif 

#ifdef BT_DIALER_SUPPORT
        if((MMIBT_IsConnectDevice(&dev_addr)) && (0 != memcmp(&dev_addr, &g_current_bt.addr, sizeof(BT_ADDRESS))))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#endif
        s_transfer_file_info.server_type = MMIBT_FILE_SERVER_OPP_SEND;
        SCI_MEMCPY(&s_transfer_file_info.device_info,&g_current_bt, sizeof(BT_DEVICE_INFO));
        //call bt api to send file to paired device，目前还都是OPP Client方式的发送 
        BtPauseBGPlay(MMIBT_FTP_SUSPEND);

		SCI_TRACE_BUF("[MMIBT] [DoOperation: MMIBT_OPC_OPP_SEND] transfer_file_name = ", s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)*sizeof(wchar));
		return_value = BT_OppSendFile(&g_current_bt.addr,s_transfer_file_info.transfer_file_name);
        if (BT_PENDING == return_value)
        {
            //sending file suspend MP3
			MMI_GetLabelTextByLang(TXT_CONNECTING, &disp_text1);
			text2_buf_ptr = (wchar *)SCI_ALLOC_APP(((BT_DEVICE_NAME_SIZE + 1)*sizeof(wchar)));
			if(text2_buf_ptr != PNULL)
			{
				SCI_MEMSET(text2_buf_ptr, 0x00, ((BT_DEVICE_NAME_SIZE + 1)*sizeof(wchar)));
				disp_text2.wstr_ptr = text2_buf_ptr;
				Get_Transfer_DeviceName(&disp_text2, BT_DEVICE_NAME_SIZE);
			}            
            //set hidden of blue tooth for send file
            MMIPUB_OpenWaitWin(2,&disp_text1, &disp_text2,PNULL,MMIBT_CONNECT_DEVICE_WAIT_WIN_ID,IMAGE_NULL,
                ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER, HandleSendConnectWindow);//HandleSendConnectWindow);
			if(text2_buf_ptr != PNULL)
			{
				SCI_Free(text2_buf_ptr);
				text2_buf_ptr = PNULL;
			}            
        }
        else
        {
            BtResumeBGPlay(MMIBT_FTP_SUSPEND);
            //SCI_TRACE_LOW:"[&&&]DoOperation start,OPC_OPP_SEND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8372_112_2_18_2_8_17_304,(uint8*)"");
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }
       break;
   case MMIBT_OPC_RECIEVE:
       //create incoming window
       MMIBT_EnterFileIncomingWin(&s_transfer_file_info);
       break;
       
   case MMIBT_OPC_PAIR:
       //pair
       pin_request_info = MMIBT_GetPinRequestInfo();
       return_value = BT_PairDevice( &pin_request_info.addr);
       if (BT_PENDING != return_value)
       {
           MMIPUB_OpenAlertFailWin(TXT_ERROR);
       }
       else
       {
           MMIBT_SetSelfPair(TRUE);
           MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &disp_text1);
           MMIPUB_OpenWaitWin(1,&disp_text1,PNULL,PNULL,MMIBT_PAIR_REQUEST_WAIT_WIN_ID,IMAGE_NULL,
               ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
           MMIBT_StartOperationWin(MMIBT_PAIR_REQUEST_WAIT_WIN_ID,MMIBT_OPC_PAIR,CreatePubWinCallBack);
       }   
        break;
   
   case MMIBT_OPC_FTP_SEND:
        //ftp send file, get file from device to other dir
        BtPauseBGPlay(MMIBT_FTP_SUSPEND);
        return_value = BT_FtpSendFile((uint16*)s_transfer_file_info.transfer_file_name);
        if(BT_PENDING == return_value)
        {
        	MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_FOREGROUND);
            //show sending progress
            MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                           MMIBT_FILE_SERVER_FTP_SEND,
                           s_transfer_file_info.transfered_size,
                           s_transfer_file_info.total_file_size);
        }
        else if(BT_SUCCESS == return_value)
        {
            //send success :show finished 
            BtResumeBGPlay(MMIBT_FTP_SUSPEND);
            //send file success 
            MMK_SendMsg(MMIBT_SENDING_FILE_WAIT_WIN_ID, MSG_BT_AUTO_CLOSE_WIN, PNULL);
            ClosePrmAndOpenDoneWin(
                                MMIBT_SENDING_FILE_WAIT_WIN_ID,
                                TXT_COMM_SEND_SUCCEED, 
                                IMAGE_PUBWIN_SUCCESS,
                                MMI_2SECONDS
                                );
			MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
        }
        else
        {
            //ftp send fail
            MMIBT_SendFileFailedInd();
			MMIBT_SetTransferStatus(MMIBT_TRANSFER_END);
        }
       break;
   case MMIBT_OPC_LIST_PAIRED:
       //list paired device
       MMIBT_EnterDeviceListWin(PNULL,MMIBT_OPC_OPP_SEND,BT_SERVICE_FTP);
       break;
   case MMIBT_OPC_VISIBLE:
       //set visibile
	   //CleanAllHideDevice();
       SetBtVisibility(MMI_BT_VISIBILE);
       break;
   case MMIBT_OPC_HIDE:
       //set visibile
	   //CleanAllHideDevice();
       SetBtVisibility(MMI_BT_HIDE);
       break;
   case MMIBT_OPC_SAVE_LOCATION:
       //set file save location
        MMIBT_EnterStorageSelectWin();
        break;
   case MMIBT_OPC_INPUT_PIN:
       //Input paired pin
	   BtPauseBGPlay(MMIBT_INPUTPIN_SUSPEND);
       pin_request_info = MMIBT_GetPinRequestInfo();
       MMIBT_CreatePinInputWin(&pin_request_info);
       break;
   case MMIBT_OPC_NUMERIC_PASSKEY_CHECK:
       //numeric passkey check
	   MMIBT_NumbericPasskeyCheck();
       break;
   case MMIBT_OPC_DEVICE_DETAILS:
       //device details
       MMIBT_EnterTextBoxWin(&(g_current_bt.addr),MMIBT_OPC_DEVICE_DETAILS);
       break;
   case MMIBT_OPC_OPEN_HEADSET:
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
		if(MMIBT_IsOnTransferStatus())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_TRANSFER_FILE_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#endif
	   //Open Headset
       OpenHeadset(&(g_current_bt.addr));
       break;
   case MMIBT_OPC_CLOSE_HEADSET:
       //Close Headset
       return_value = CloseHeadset(&(g_current_bt.addr));
       handleHeadsetCloseResult(return_value, &(g_current_bt.addr));
       break;
   case MMIBT_FTS_CONNECT_REQ:
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
        //HL added it for not support search while blue headset connected
        if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ACTIVE_HEADSET_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            BT_FtsConnect(FALSE);
            break;
        }
		//added by tonny.chen for HL bt
        if(MMIBT_IsOnTransferStatus())
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_TRANSFER_FILE_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            break;
        }
#ifdef BT_HL_PATCH_SUPPORT
        MMIBT_SetTransferStatus(MMIBT_TRANSFER_FTS_CONNECTED);
		MMIBT_UpdateMainMenu(TRUE);
        //end added 
#endif
#endif
        BT_FtsConnect(TRUE);
        break;
   case MMIBT_OPS_CONNECT_REQ:
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
        //HL added it for not support search while blue headset connected
        if(MMIAPIBT_GetActiveBTHeadset() || BT_GetActivatedHS(&addr))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_ACTIVE_HEADSET_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            BT_OpsConnect(FALSE);
            break;
        }
#endif 
        BT_OpsConnect(TRUE);
       break;
   case MMIBT_OPC_FTS_DLE:
       //FTP Del
        //Exit DC,DV
        MMIAPIDC_Exit();
        //MMIAPIPB_CloseVCardWin();
        //Mp4

#ifdef VIDEO_PLAYER_SUPPORT
        //MMIAPIVP_ExitVideoPlayer(); //cr246138  @andy.he 20100616
#endif

        return_value = BT_DeleteFile(TRUE);
        //if delete file is wallpaper should reset wallpaper info
        if (BT_SUCCESS == return_value && GetIsDelWallpaperFile())
        {
#if defined MMI_WALLPAPER_SUPPORT
            MMIAPIFMM_ResetWallpaperSetting();
#endif
        }

        if (BT_SUCCESS == return_value )
        {
            //Del success
            MMIBT_UpdateCurrentFileList();
            UpdateCurrentPictureViewList();
            MMIDEFAULT_SetBackLight(TRUE);
            //if any file(folder) has been deleted, then close new file window(querywin) to avoid
            //open the file which has been deleted by remote control device(i.e. pc) (bug79303)
            if(MMK_IsOpenWin(MMI_BT_NEW_FILE_WIN_ID))
            {
                MMK_CloseWin(MMI_BT_NEW_FILE_WIN_ID);
            }
        }
        else
        {
            //Del fail
            MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
        }    
       break;
   case MMIBT_OPC_DEVICE_DELETE:
       DeletePairedDevice(FALSE,MMIBT_OPC_DEVICE_DELETE);
       break;
   case MMIBT_OPC_DEVICE_DELETE_ALL:
       DeletePairedDevice(TRUE,MMIBT_OPC_DEVICE_DELETE_ALL);
       break;
   case MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT:
	{
		BT_SetDeviceAuthorisation(&g_current_bt.addr,TRUE);
		CheckAuthorizeAndOpenIncomingWin();
	}
       break;
#ifdef MMIBT_HEADSET_AUTHORIZE       
   case MMIBT_HFG_AUDIO_CONNECT:
        BT_AcceptAuthorisation(&g_current_bt.addr, TRUE);
    break;
#endif
#ifdef BT_DIALER_SUPPORT
   case MMIBT_OPC_OPEN_HANDSET_GATEWAY:
       //Open Headset
       OpenHandsetGateway(&(g_current_bt.addr));
       break;
   case MMIBT_OPC_CLOSE_HANDSET_GATEWAY:
       //Close Headset
       return_value = CloseHandsetGateway(&(g_current_bt.addr));
       handleHandsetGatewayCloseResult(return_value, &(g_current_bt.addr));
       break;
#endif
   default:
       break;
   }
   return TRUE;
}

/*****************************************************************************/
//  Discription: cancel operation by  MMIBT_OPERTATION_TYPE_E
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BT_STATUS CancelOperation(MMIBT_OPERTATION_TYPE_E  current_opt)
{
   BT_STATUS bt_status = BT_SUCCESS;
   BT_DEVICE_INFO  pin_request_info = {0};
   
   switch(current_opt)
   {
   case MMIBT_OPC_PAIR:
       BtResumeBGPlay(MMIBT_PAIR_SUSPEND);
       MMIBT_SetPinCancel(TRUE);
       pin_request_info = MMIBT_GetPinRequestInfo();
       bt_status = BT_CancelPair(&pin_request_info.addr);
#ifdef BT_DIALER_SUPPORT
       MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
#endif
	   break;
   case MMIBT_OPC_SEARCH:
       bt_status = BT_CancelSearch();
       BtResumeBGPlay(MMIBT_SEARCH_SUSPEND);
       break;
   case MMIBT_OPC_INPUT_PIN:
       MMIBT_SetPinCancel(TRUE);
       pin_request_info = MMIBT_GetPinRequestInfo();
       bt_status = BT_CancelPair(&pin_request_info.addr);
#ifdef BT_DIALER_SUPPORT
       MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
#endif
	   BtResumeBGPlay(MMIBT_INPUTPIN_SUSPEND);
       break;
   case MMIBT_OPC_NUMERIC_PASSKEY_CHECK:
       MMIBT_CancelNumricPair();
       break;    
   case MMIBT_OPC_RECIEVE:
   case MMIBT_OPC_OPP_SEND://Send 
   case MMIBT_OPC_FTP_SEND:
        bt_status = MMIBT_CancelFileTransfer(TRUE);
        if(BT_PENDING != bt_status)
        {
            s_transfer_file_info.server_type = MMIBT_FILE_SERVER_MAX;
            if(BT_SUCCESS == bt_status)
            {
                //cancel success
                MMIPUB_OpenAlertSuccessWin(TXT_CANCELED);
            }
            else
            {
                //cancel error
                //SCI_TRACE_LOW:"[&&&]DoOperation start,CancelOperation MMIBT_OPC_OPP_SEND"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_8551_112_2_18_2_8_17_305,(uint8*)"");
                MMIPUB_OpenAlertFailWin(TXT_ERROR);            
            }
        }
        else
        {
            OpenCancelWaitingWin();
        }
        break;
   case MMIBT_FTS_CONNECT_REQ: 
        BT_FtsConnect(FALSE);
        break;
   case MMIBT_OPS_CONNECT_REQ:
        BT_OpsConnect(FALSE);
        break;
   case MMIBT_OPC_FTS_DLE:
       //cancel ftp del
        bt_status = BT_DeleteFile(FALSE); 
       break;
    case MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT:
	{
		BT_SetDeviceAuthorisation(&g_current_bt.addr,FALSE);
		CheckAuthorizeAndOpenIncomingWin();
	}
        break;
#ifdef MMIBT_HEADSET_AUTHORIZE         
   case MMIBT_HFG_AUDIO_CONNECT:
        BT_AcceptAuthorisation(&g_current_bt.addr,FALSE);
        break;     
#endif
   default:
       break;
   }
   return bt_status;
}

 

/*****************************************************************************/
//  Description : setting the state of connecting animate ctrl
//  Global resource dependence :
//  Author: wanqiangzh
//  Note:   state 联网动画当前需要设定的状态:1,动画;2,静止
///*****************************************************************************/
PUBLIC void AnimatePlayCtrlByState(int32 state)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};

    ctrl_info.is_ctrl_id = TRUE;
    ctrl_info.ctrl_id = MMIBT_ANIMATE_SEARCHING_CTRL_ID;
    data_info.anim_id =  AINM_BT_SEARCHING;//IMAGE_CMSBRW_CONNECTION_ANIM;

    display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
    display_info.is_update = FALSE;
    if(MMIBT_ANIM_INIT == state)
    {
        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        }
    }
    else if(MMIBT_ANIM_PLAY == state)
    {
        //play 
        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            GUIANIM_ResumeAnim(MMIBT_ANIMATE_SEARCHING_CTRL_ID);
        }
    }
    else if(MMIBT_ANIM_STOP == state)
    {
        //stop
        if(MMK_IsOpenWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID))
        {
            GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
            GUIANIM_PauseAnim(MMIBT_ANIMATE_SEARCHING_CTRL_ID);
        }
    }   
}

#ifdef BT_DUN_SUPPORT
// @baokun DUN
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: baokun.yin
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_OpenDUNWin(BT_MSG_T* msg_body_ptr)
{
    MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_OpenDUNWin fail PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9731_112_2_18_2_8_20_322,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMIBT_OpenDUNWin msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9734_112_2_18_2_8_20_323,(uint8*)"d",msg_body_ptr->status);
        //turn on the back light
    MMIDEFAULT_SetBackLight(TRUE);
    if (BT_SUCCESS == msg_body_ptr->status)
    {    
        uint32 i = MN_DUAL_SYS_1;
        BOOLEAN is_sim_ok = FALSE;

        for(i = MN_DUAL_SYS_1; i < MMI_DUAL_SYS_MAX; i++)
        {            
           if(SIM_STATUS_OK == MMIAPIPHONE_GetSimStatus(i))
           {
                is_sim_ok = TRUE;
                // 此处只是用来判断是否有sim status OK,一旦有sim OK,直接break
                break; 
           }
        }          

        if(SIO_UserPortBusy())
        {
         //SCI_TRACE_LOW:"UART selected SIO_UserPortBusy"
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9754_112_2_18_2_8_20_324,(uint8*)"");
          MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMM_UCOM_BUSY,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }          
        //PC数据通信包含了对SMS和PB的操作,No sim 不允许操作
        else if((MMIAPISMS_IsOrderOk()) && (MMIAPIPB_IsPbReady()) && (is_sim_ok))
        {
             MMI_STRING_T query_text = {0};
             uint32       alert_time_period = 0;
             MMI_WIN_ID_T alert_win_id = MMIBT_DUN_CONNECT_QUERY_WIN_ID;
             //SCI_TRACE_LOW:"UART selected"
             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9763_112_2_18_2_8_20_325,(uint8*)"");
             //MMK_CreateWin((uint32 *)MMIBT_UCOM_CONNECT_REQ_TAB, PNULL);  
             //Open alert window
              MMI_GetLabelTextByLang(TXT_BT_ACCEPT_DUN_OR_NOT, &query_text);
              MMIPUB_OpenAlertWinByTextPtr(&alert_time_period,&query_text,PNULL,IMAGE_PUBWIN_QUERY,&alert_win_id,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleConnectDUNOrNotMsg);
        }
        else if(SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_1))
        {
            //SCI_TRACE_LOW:"MMIAPIPHONE_GetSimStatus not ready"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9771_112_2_18_2_8_20_326,(uint8*)"");
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_NO_SIM,TXT_NULL,IMAGE_PUBWIN_WARNING,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else
        {
            //SCI_TRACE_LOW:"MMIAPIBT_OpenDUNWin error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9776_112_2_18_2_8_20_327,(uint8*)"");
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }

}

 /*****************************************************************************/
//  Discription: 
//  
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_DUNOpenedInd(BT_MSG_T* msg_body_ptr)
{
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_DUNOpenedInd PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9794_112_2_18_2_8_20_328,(uint8*)"");
        return;
    }
      //SCI_TRACE_LOW:"MMIBT_DUNOpenedInd msg_body_ptr->status %d"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9797_112_2_18_2_8_20_329,(uint8*)"d",msg_body_ptr->status);
        //turn on the back light
    MMIDEFAULT_SetBackLight(TRUE);
      if (BT_SUCCESS == msg_body_ptr->status)
     {
          s_is_vdun_on = TRUE;
          if (MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
            {
        ClosePrmAndOpenDoneWin(
                    MMIBT_DUN_WAITING_PUBWIN_ID,
                    TXT_BT_DUN_CONNECTED,
                    IMAGE_PUBWIN_SUCCESS,
                    MMI_2SECONDS
                    );              
            
            }
      }
      else if(MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
     {
          ClosePrmAndOpenDoneWin(
                      MMIBT_DUN_WAITING_PUBWIN_ID,
                      TXT_ERROR,
                      IMAGE_PUBWIN_FAIL,
                      MMI_2SECONDS
                      );              
      }

}

  /*****************************************************************************/
//  Discription: 
//  
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_DUNClosedInd(BT_MSG_T* msg_body_ptr)
{
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_DUNClosedInd PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9837_112_2_18_2_8_20_330,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"MMIBT_DUNClosedInd msg_body_ptr->status %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9840_112_2_18_2_8_20_331,(uint8*)"d",msg_body_ptr->status);
    //turn on the back light
    MMIDEFAULT_SetBackLight(TRUE);
    MMK_CloseWin(MMIBT_DUN_CONNECT_QUERY_WIN_ID);
    if (BT_SUCCESS == msg_body_ptr->status)
    {

      s_is_vdun_on = FALSE;
     // if (MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
        {
            ClosePrmAndOpenDoneWin(
                    MMIBT_DUN_WAITING_PUBWIN_ID,
                    TXT_BT_DUN_DISCONNECT,
                    IMAGE_PUBWIN_SUCCESS,
                    MMI_3SECONDS
                    );  
        }
    }
    else //if(MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
    {
        ClosePrmAndOpenDoneWin(
                    MMIBT_DUN_WAITING_PUBWIN_ID,
                    TXT_ERROR,
                    IMAGE_PUBWIN_FAIL,
                    MMI_3SECONDS
                    );              
    }

}
/*****************************************************************************/
//  Discription: Handle try pair again or not win msg  
//  Global resource dependence: none 
//  Author: Wenming.feng 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E      HandleConnectDUNOrNotMsg(
                                                   MMI_WIN_ID_T win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param 
                                                   )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    MMI_STRING_T        wait_text = {0};    
    
    switch (msg_id)
    {
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //SCI_TRACE_LOW:"HandleConnectDUNOrNotMsg Cancel"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9888_112_2_18_2_8_20_332,(uint8*)"");
        BT_RejectDunReq();
        MMI_GetLabelTextByLang(TXT_BT_DUN_CANCEL, &wait_text);
        //s_is_vdun_on = FALSE;
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_DUN_WAITING_PUBWIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleDUNWaitingWinMsg);
        MMK_CloseWin(win_id);
        break;
    
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //SCI_TRACE_LOW:"HandleConnectDUNOrNotMsg Accept"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9900_112_2_18_2_8_20_333,(uint8*)"");
        BT_AcceptDunReq();
        //s_is_vdun_on = TRUE;
        MMI_GetLabelTextByLang(TXT_DUN_CONNECT, &wait_text);
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_DUN_WAITING_PUBWIN_ID,IMAGE_NULL,
           ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_ONE,HandleDUNWaitingWinMsg);
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);;
        break;
    }
    return(result);
}
/*****************************************************************************/
//  Description :get vuart run status 
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_DUNIsRun(void)
{
    return s_is_vdun_on;
}
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleDUNWaitingWinMsg(
                                                    MMI_WIN_ID_T win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param 
                                                    )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
        
    case MSG_APP_RED:     
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        //User refuse to accept the file
        if(s_is_vdun_on)
          {
               BT_RejectDunReq();
          }
          MMK_CloseWin( win_id );
        break;
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}
/*****************************************************************************/
//  Discription:
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_CloseDUNWin(void)
{
      if(MMK_IsOpenWin(MMIBT_DUN_CONNECT_QUERY_WIN_ID))
      {
          MMK_CloseWin( MMIBT_DUN_CONNECT_QUERY_WIN_ID );
      }
      if(MMK_IsOpenWin(MMIBT_DUN_WAITING_PUBWIN_ID))
      {
          MMK_CloseWin( MMIBT_DUN_WAITING_PUBWIN_ID );
      }    
}
#endif

/*****************************************************************************/
//  Discription: Open CancelW aiting Window
//  Parameter: 
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void OpenCancelWaitingWin(void)
{
    MMI_STRING_T wait_text = {0};

    MMI_GetLabelTextByLang(TXT_BT_CANCELLING, &wait_text);
    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIENG_BT_CANCEL_WAIT_WIN_ID,IMAGE_NULL,
        ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
}

/*****************************************************************************/
//  Discription:check file exist or not, if file not exist, open incoming window;
//              if file already exist,query user overwrite or not if user selected 
//              "Yes", open incoming window, if user select "No", cancel recevie file
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void CheckAuthorizeAndOpenIncomingWin(void)
{
    BOOLEAN       be_authorize = FALSE;
    
    BT_GetDeviceAuthorisation(&g_current_bt.addr,&be_authorize);
	
	if(MMIBT_OPC_DEVICE_AUTO_CONNECT_HINT == MMIBT_GetOpertor().current_opertion)
	{
		return;
	}
  	else if(MMIBT_FTS_CONNECT_REQ == MMIBT_GetOpertor().current_opertion
           ||MMIBT_OPS_CONNECT_REQ == MMIBT_GetOpertor().current_opertion
           ||MMIBT_OPC_RECIEVE == MMIBT_GetOpertor().current_opertion
        )
	{	
		if(be_authorize)
		{
            if(AssignReceiveFileCfg(s_transfer_file_info.server_type, FALSE))
            {
                //enter receiving file win
                MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                               s_transfer_file_info.server_type,
                               s_transfer_file_info.transfered_size,
                               s_transfer_file_info.total_file_size);
            }
		}
		else
		{
			DoOperation(MMIBT_OPC_RECIEVE, FALSE);
		}
	}
}

/*****************************************************************************/
//  Discription:display memory or udisk not exist or sd card not exist
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL void DisplayMemoryInformation(MMIFILE_DEVICE_E filedev)
{
    MMI_TEXT_ID_T text_id1 = TXT_NULL;
    
    if(!MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(filedev), 
                                MMIAPIFMM_GetDevicePathLen(filedev)))
    {
        //sd card not exist
        text_id1 = MMIAPIFMM_GetAlertTextIDByDev(filedev);
    }
    else
    {
        text_id1 = TXT_CMSBRW_FULL_NO_STORE;
    }
                           
    MMIPUB_OpenAlertWinByTextId(PNULL,text_id1,PNULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_CUSTOMER,HandleSaveNewFileFailedWinMsg); 
}
/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_Reset(void)
{
    MMIBT_USER_SETTING_U  setting = {0};/*lint !e64*/
    //MMIFILE_DEVICE_E filedev = MMI_DEVICE_UDISK;
    MMIFILE_DEVICE_E dev = MMI_DEVICE_UDISK;    
    //SCI_TRACE_LOW:"MMIBT_Reset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10155_112_2_18_2_8_21_336,(uint8*)"");
	MMIAPISET_SetWaitFlag();
    MMIBT_SetOpertor(MMIBT_OPC_INIT, MMIBT_OPC_INIT);
    //reset bt local name
    BT_SetLocalName(BT_DEFAULT_LOCAL_NAME);
    //hide bt
    BT_SetVisibility(BT_PAGE_ENABLE);
    //set defaul shairded dir
    
    strcpy((char*)setting.authority.file_mode, MMIBT_FTP_RW_MODE);
    MMINV_WRITE(MMINV_BT_SHAIRED_AUTHORITY,&setting);
    SCI_MEMSET(&setting, 0x00, sizeof(MMIBT_USER_SETTING_U ));
    dev = MMIAPIFMM_GetDefaultDisk();

    MMIAPICOM_Wstrncpy(
        setting.dir.shaired_path_name,
        MMIAPIFMM_GetDevicePath(dev),
        MMIAPIFMM_GetDevicePathLen(dev)
        );
    setting.dir.shaired_path_name[1] = MMIFILE_COLON;
 
    MMINV_WRITE(MMINV_BT_SHAIRED_DIRECTORY,&setting);
    BT_SetFtpRootFolder((uint16*)setting.dir.shaired_path_name, (char*)MMIBT_FTP_RW_MODE);
    //set opp file location
    SCI_MEMSET(&setting, 0x00, sizeof(MMIBT_USER_SETTING_U));
    setting.filedev = dev;
    MMINV_WRITE(MMINV_BT_FILE_LOCATION,&setting);
    MMIBT_SetFileSaveLocation();
	
    //delete all
    MMIBT_ClearAllDevice(FALSE);
    if(!MMIBT_OpenBluetooth(FALSE, FALSE))
	{
		//need not to wait confirm
		MMIAPISET_FuncFinishedInd();
	}
}

/*****************************************************************************/
//  Description : reset bt config
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ClearAllDevice(BOOLEAN is_delect_connect)
{
    int            device_number = 0;
    int            i = 0;
    BT_DEVICE_INFO dev_info = {0};
	
    //delete all
    device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);
	for(i =device_number; i>= 1; i--)
	{
		BT_GetPairedDeviceInfo(BT_SERVICE_ALL, i, &dev_info);
		if(is_delect_connect
            ||(!MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr))
#if (defined BT_HL_PATCH_SUPPORT) || (defined BT_CHIP_BEKEN)
               &&(!MMIBT_DeviceIsConnectedFTP(&(dev_info.addr)))
#endif
               &&(!MMIBT_DeviceIsOnTransferStatus(&(dev_info.addr),&s_transfer_file_info.device_info.addr))
              )
           )
		{
			BT_RemovePairedDevice(&(dev_info.addr));
		}
	}
}





/*****************************************************************************/
//  Description : is premit open this file or not
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Is_PermitOpenFile(void)
{
    BOOLEAN      result = FALSE;
    MMI_WIN_ID_T   previous_win_id = 0;

    previous_win_id = MMK_GetFocusWinId();
    //previous_win_id = MMK_GetPrevWinId(previous_win_id);
    if(MMIKL_CLOCK_DISP_WIN_ID == previous_win_id)
    {
        previous_win_id = MMK_GetPrevWinId(MMIKL_CLOCK_DISP_WIN_ID);
    }
     //if current in idle, mainmenu or BT mainmenu, can permit open the recieved file
     if(MAIN_IDLE_WIN_ID == previous_win_id || MAIN_MAINMENU_WIN_ID == previous_win_id || MMIBT_MAIN_MENU_WIN_ID == previous_win_id)
     {
         result = TRUE;
     }
     else
     {
         result = FALSE;
     }
     //SCI_TRACE_LOW:"Is_PermitOpenFile result %d previous_win_id 0x%x "
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10608_112_2_18_2_8_22_341,(uint8*)"dd", result, previous_win_id);
     return result;
}

/*****************************************************************************/
//  Discription:  close send file windows
//  Global resource dependence: None
//  Author:
//  Note : if current opertation is opp_send, close inquery list and device list
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN  CloseSendWin(void)
{
   MMIBT_OPERATION_INFO_T operation_info = MMIBT_GetOpertor();
    
   if(MMIBT_OPC_OPP_SEND == operation_info.root_opertion)
   {
	  MMK_CloseWin(MMIBT_INQUIRED_DEV_LIST_WIN_ID);
	  MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
	  MMK_SendMsg(MMIBT_PIN_INPUT_WIN_ID, MSG_APP_CANCEL, PNULL);
   }
   return TRUE;
}

/*****************************************************************************/
//  Discription:  MMIBT_CloseStorageMenu
//  Global 
//  Author:
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_CloseStorageMenu(void)
{
	MMK_CloseWin(MMIBT_SHARED_DIRECTORY_WIN_ID);
	MMK_CloseWin(MMIBT_SET_SAVE_LOCATION_WIN_ID);
}

/*****************************************************************************/
//  Discription: 
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL BOOLEAN MMIBT_IsEnoughSpaceToRecieve(MMIFILE_DEVICE_E file_dev)
{
    BOOLEAN result = FALSE;

     MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name,          
         MMIAPIFMM_GetDevicePath(file_dev),
         MMIAPIFMM_GetDevicePathLen(file_dev));

    if (MMIAPIFMM_IsDevEnoughSpace(s_transfer_file_info.total_file_size, file_dev)
            ||( MMIAPIFMM_IsFileExist(s_transfer_file_info.transfer_file_name, MMIAPICOM_Wstrlen(s_transfer_file_info.transfer_file_name)) 
                && MMIBT_DiskEnoughSpaceForOverWrite(s_transfer_file_info.transfer_file_name, s_transfer_file_info.total_file_size,file_dev))
            )    
    {
        result = TRUE;
    }
    
    return result; 
    
}

/*****************************************************************************/
//  Description : open direct and get user selected dir
//  Global resource dependence : 
//  Author: 
//  Note: after user selected a dir, it will be set to shaired directory
/*****************************************************************************/
PUBLIC void MMIBT_SetFileLocationCallBack( BOOLEAN     is_success,
                                    FILEARRAY   file_array
                                    )
{
    uint32  file_num = 0;
    FILEARRAY_DATA_T file_array_data = {0};
    wchar   full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    
    //SCI_TRACE_LOW:"MMIBT_SetFileLocationCallBack, Enter,is_success = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11325_112_2_18_2_8_23_362,(uint8*)"d",is_success);
    if(is_success)
    {
        file_num = MMIAPIFILEARRAY_GetArraySize(file_array);
        //SCI_TRACE_LOW:"MMIBT_SetFileLocationCallBack, file_num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_11329_112_2_18_2_8_23_363,(uint8*)"d", file_num);
        if(file_num > 0)
        {
            //read user selected dir
            if(MMIAPIFILEARRAY_Read(file_array, 0, &file_array_data))
			{	
				//set shaired directory
				MMIDEFAULT_SetBackLight(TRUE);
				MMIAPICOM_GetLongFileNameByFullPath(s_transfer_file_info.transfer_file_name, full_path_name);//arvin zhang fixed for Bug 4288(full_path_name存储文件全名，i.e. sample.jpg)
                SCI_MEMSET(s_transfer_file_info.transfer_file_name,0x00, sizeof(s_transfer_file_info.transfer_file_name));
                MMIAPICOM_Wstrncpy(s_transfer_file_info.transfer_file_name,file_array_data.filename,MIN(file_array_data.name_len, MMIFILE_FULL_PATH_MAX_LEN )); 
                MMIAPICOM_Wstrcat(s_transfer_file_info.transfer_file_name, L"\\");
                MMIAPICOM_Wstrcat(s_transfer_file_info.transfer_file_name, full_path_name);
                s_transfer_file_info.filedev = MMIAPIFMM_GetDeviceTypeByPath(s_transfer_file_info.transfer_file_name, MMIFILE_DEVICE_LEN);
                
                if (MMIBT_IsEnoughSpaceToRecieve(s_transfer_file_info.filedev))                
                {
    				//overwirte space enough
                    CheckAuthorizeAndOpenIncomingWin(); 
    			}
                else
                {
                    //other memory space is not enough too
                    DisplayMemoryInformation(s_transfer_file_info.filedev);
                    MMIBT_CancelFileTransfer(TRUE);
                }
            }			
            else
            {
                //folder not exist or other error
                MMIPUB_OpenAlertWarningWin(TXT_FOLDER_NOT_EXIST);
            }
        }
    }
    else
    {
        MMIBT_CancelFileTransfer(TRUE);
    }
    //释放列表
    MMIAPIFILEARRAY_Destroy(&file_array);
    file_array = PNULL;
}
/*****************************************************************************/
//  Discription: None
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSaveNewFileFailedWinMsg(
                                            MMI_WIN_ID_T win_id, 
                                            MMI_MESSAGE_ID_E msg_id, 
                                            DPARAM param 
                                            )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
        MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    case MSG_CTL_CANCEL:
    case MSG_APP_RED:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
        
    default:
        result = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return result;     
}

/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetCurBtInfo(BT_DEVICE_INFO *device_info)
{   
    SCI_MEMSET(&g_current_bt,0x00, sizeof(BT_DEVICE_INFO));	
    if(PNULL != device_info)
    {   
    	SCI_MEMCPY(&g_current_bt, device_info, sizeof(BT_DEVICE_INFO));
    }
}
/*****************************************************************************/
//  Discription: open searching device waiting window
//  Global resource dependence: none 
//  Author: yuantao.xu
//  Note:
//
/*****************************************************************************/
PUBLIC BT_DEVICE_INFO  MMIBT_GetCurBtInfo(void)
{
    return g_current_bt;
}
/*****************************************************************************/
//  Discription: get bluetooth transfer file status.(foreground/background)
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC MMIBT_TRANSFER_STATUS_TYPE_E  MMIBT_GetTransferStatus(void)
{
    return s_bt_transfer_status;        
}
/*****************************************************************************/
//  Discription: set blue tooth send or receive file status
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetTransferStatus(MMIBT_TRANSFER_STATUS_TYPE_E transfer_status)
{    
    SCI_TRACE_LOW("[MMIBT] MMIBT_SetTransferStatus transfer_status %d", transfer_status);
    s_bt_transfer_status = transfer_status;
    if((MMIBT_TRANSFER_END == transfer_status) || (MMIBT_TRANSFER_FREE == transfer_status))
    {
        SCI_MEMSET(&s_transfer_file_info.device_info,0x00, sizeof(s_transfer_file_info.device_info));
        s_transfer_file_info.transfered_size = 0;
    }
}
/*****************************************************************************/
//  Discription: set blue tooth send or receive file Transfered Size
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC void  MMIBT_SetTransferedSize(uint32 size)
{    
    SCI_TRACE_LOW("[MMIBT] MMIBT_SetTransferedSize transfered_size %d", size);
    s_transfer_file_info.transfered_size = size;
}
/*****************************************************************************/
//  Discription: if blue tooth on transfer status
//  Global resource dependence: none 
//  Author: 
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN  MMIBT_IsOnTransferStatus(void)
{
    BOOLEAN 					 result = FALSE;
	MMIBT_TRANSFER_STATUS_TYPE_E transfer_stats = MMIBT_TRANSFER_END;

	transfer_stats = MMIBT_GetTransferStatus();	
    if((MMIBT_TRANSFER_END != transfer_stats) && (MMIBT_TRANSFER_FREE != transfer_stats))
        result = TRUE;      
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_IsOnTransferStatus result %d", result);    
    return result;
}
/*****************************************************************************/
//  Description : Open BT transfer file win(send/receive)
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_OpenTransferWin(void)
{   
    if(MMIBT_SENDING_FILE_BACKGROUND == MMIBT_GetTransferStatus())
    {
        MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_FOREGROUND);
    }
    else if(MMIBT_REVEIVEING_FILE_BACKGROUND == MMIBT_GetTransferStatus())
    {
        MMIBT_SetTransferStatus(MMIBT_RECEIVEING_FILE_FOREGROUND);
    }
    MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                               s_transfer_file_info.server_type,
                               s_transfer_file_info.transfered_size,
                               s_transfer_file_info.total_file_size);
}

/*****************************************************************************/
//  Description : display receive file ratio info
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void DisplayFtpFileSizeInfo(
                                  uint32    file_size,
                                  MMI_CTRL_ID_T ctrl_id,
								  MMI_WIN_ID_T win_id
                                  )
{
	MMI_STRING_T        size_str = {0};
    wchar               wsize_info[(MMIBT_FILE_LENGTH_INFO << 1) + 1] = {0};
	uint16              wstr_len = 0;

	//get tranfered size string
	MMIAPIFMM_GetFileSizeString(file_size, wsize_info, MMIBT_FILE_LENGTH_INFO, FALSE);
	wstr_len = MMIAPICOM_Wstrlen(wsize_info);
	if(wstr_len + 1 < (MMIBT_FILE_LENGTH_INFO << 1))
	{
		wsize_info[wstr_len] = '/';
	}
    wstr_len ++;
	//get total file size string
	if(wstr_len  < (MMIBT_FILE_LENGTH_INFO << 1))
	{
		MMIAPIFMM_GetFileSizeString(s_transfer_file_info.total_file_size, &wsize_info[wstr_len], (MMIBT_FILE_LENGTH_INFO << 1) - wstr_len, FALSE);
	}
    size_str.wstr_ptr  = wsize_info;
	size_str.wstr_len = MMIAPICOM_Wstrlen(wsize_info);
    GUILABEL_SetText(ctrl_id,&size_str, TRUE);    
}
/*****************************************************************************/
//  Description : get transfer file name
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Get_Transfer_FileName(MMI_STRING_T *file_name)
{
    if(PNULL ==file_name || PNULL == file_name->wstr_ptr)
    {
        //SCI_TRACE_LOW:"file_name string invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10357_112_2_18_2_8_21_339,(uint8*)"");
        return FALSE;
    }
    MMIAPICOM_GetLongFileNameByFullPath(s_transfer_file_info.transfer_file_name, file_name->wstr_ptr);
    file_name->wstr_len = MMIAPICOM_Wstrlen( file_name->wstr_ptr);  
    return TRUE;
}
/*****************************************************************************/
//  Description : get transfer device name
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN Get_Transfer_DeviceName(MMI_STRING_T *device_name, uint16 max_str_len)
{
    if(PNULL ==device_name || PNULL == device_name->wstr_ptr)
    {
        //SCI_TRACE_LOW:"Get_Transfer_DeviceName string invalid !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10341_112_2_18_2_8_21_338,(uint8*)"");
        return FALSE;
    }
    device_name->wstr_len = MIN(MMIAPICOM_Wstrlen(s_transfer_file_info.device_info.name), max_str_len);/*lint !e666*/
    MMIAPICOM_Wstrncpy(device_name->wstr_ptr, s_transfer_file_info.device_info.name, device_name->wstr_len);
    return TRUE;
}
/*****************************************************************************/
//  Discription: Update current picture view list
//  Global resource dependence: none 
//  Author: haiwu.chen
//  Note: update picture view list when is needed
/*****************************************************************************/
PUBLIC void UpdateCurrentPictureViewList(void)
{
    MMI_STRING_T string = {0};
    wchar *path_name_ptr = PNULL;
    //SCI_TRACE_LOW:"UpdateCurrentPictureViewList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10130_112_2_18_2_8_21_335,(uint8*)"");
    
    path_name_ptr = SCI_ALLOCA((MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));    

    if(path_name_ptr != PNULL)
    {
        SCI_MEMSET(path_name_ptr, 0x00, (MMIFILE_FILE_NAME_MAX_LEN + 1)*sizeof(wchar));
        MMIAPICOM_GetLongFilePathNameByFullPath(s_transfer_file_info.transfer_file_name, path_name_ptr);
        //检查图片保存的路径是否是默认的图片浏览打开的目录
        string.wstr_ptr = path_name_ptr;
        string.wstr_len = MMIAPICOM_Wstrlen(path_name_ptr);
        if (MMIAPIPICVIEW_IsDefaultPath(string))
        {
            MMIAPIPICVIEW_ReloadFile();            
        }  
        SCI_FREE(path_name_ptr);
    }   
}
/*****************************************************************************/
//  Discription: connect device to send file result ind
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDeviceResCnf( BT_MSG_T* msg_body_ptr )
{
    if (PNULL == msg_body_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_ConnectDeviceResCnf PNULL == msg_body_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4130_112_2_18_2_8_8_247,(uint8*)"");
        return ;
    }

    MMIDEFAULT_SetBackLight(TRUE);
    if (BT_SUCCESS == msg_body_ptr->status)
    {
		if(MMK_IsOpenWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID))
		{	
            MMIBT_EnterFileTransferWin(s_transfer_file_info.transfer_file_name,
                               MMIBT_FILE_SERVER_OPP_SEND,
                               s_transfer_file_info.transfered_size,
                               s_transfer_file_info.total_file_size);
			//MMIBT_SetTransferStatus(MMIBT_SENDING_FILE_FOREGROUND);
			MMIBT_UpdateMainMenu(TRUE);
			MMK_CloseWin(MMIBT_CONNECT_DEVICE_WAIT_WIN_ID);
		}
		else
		{
			BtResumeBGPlay(MMIBT_FTP_SUSPEND);
			MMIBT_CancelFileTransfer(TRUE);
		}
    }
    else
    {
        //connect device fail, resume mp3
        BtResumeBGPlay(MMIBT_FTP_SUSPEND);
        //reset blue tooth status
        //MMIBT_RecoveryBtStatus(MMIBT_MODULE_BLUETOOTH);
        
        if ( MMK_IsOpenWin( MMIBT_CONNECT_DEVICE_WAIT_WIN_ID ))
        {
            //SCI_TRACE_LOW:"[&&&]MMIBT_ConnectDeviceResCnf"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_4161_112_2_18_2_8_8_248,(uint8*)"");
            MMIPUB_OpenAlertFailWin(TXT_COMM_SEND_FAILED);
            MMK_CloseWin( MMIBT_CONNECT_DEVICE_WAIT_WIN_ID );
        }
    }
    MMK_CloseWin(MMIENG_BT_CANCEL_WAIT_WIN_ID);
	if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
    {
        MMK_CloseWin(MMIBT_DEVICE_LIST_WIN_ID);
    }
}

/*****************************************************************************/
//  Discription: get transfered file size
//  Global resource dependence: none 
//  Author: yang.yang 
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 MMIBT_GetTransferedSize(void)
{
    return s_transfer_file_info.transfered_size;
}

/*****************************************************************************/
//  Description : MMIBT_GetDeviceTypeByService
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_GetDeviceType(BT_DEVICE_INFO * dev_info, uint16 * dev_type)
{
#ifndef BT_DIALER_SUPPORT
    return BT_GetDeviceType(&dev_info, &dev_type);
#else
    uint32       dev_service = 0;
    BT_STATUS    ret_val = BT_SUCCESS;
    
    dev_service = BT_GetPairedDeviceService(&dev_info->addr);
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType dev_service = 0x%x", dev_service);
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType dev_service = 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x", dev_info->addr.addr[0],dev_info->addr.addr[1],dev_info->addr.addr[2],dev_info->addr.addr[3],dev_info->addr.addr[4],dev_info->addr.addr[5]);
    
    ret_val = BT_GetDeviceType(dev_info, dev_type);
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType dev_type = 0x%x", *dev_type);
    
    if(BT_DEVICE_CLASS_COMPUTER == *dev_type)
    {
        return ret_val;
    }
    
    if ((BT_SERVICE_HANDFREE_GATEWAY |BT_SERVICE_HEADSET_GATEWAY) & dev_service)
    {
        *dev_type |= BT_DEVICE_CLASS_PHONE;
        SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType with HFG dev_type = 0x%x", *dev_type);
    }
    
    if ((BT_SERVICE_MAP_SERVER |BT_SERVICE_PBAP_SERVER) & dev_service)
    {
        *dev_type |= BT_DEVICE_CLASS_PHONE;
        SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType with HFG dev_type = 0x%x", *dev_type);
    }
    
    if ((BT_SERVICE_HANDSET |BT_SERVICE_HANDFREE) & dev_service)
    {
        *dev_type |= BT_DEVICE_CLASS_AUDIO;
        SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType with HFC dev_type = 0x%x", *dev_type);
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_GetDeviceType final dev_type = 0x%x", *dev_type);
    
    return ret_val;
#endif
}

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
//  Description : receive msg: AG device connected
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDeviceCnf(BT_MSG_T* msg_body_ptr)
{
    BT_ADDRESS     address = {0};
    MMI_WIN_ID_T   alert_id = MMI_BT_DEV_CONNECT_WIN_ID;
    
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf PNULL == msg_body_ptr!");
        return;
    }
    
    address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
    SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf address = %x, %x, %x, %x, %x, %x",
        address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
    SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf msg_body_ptr->status %d", msg_body_ptr->status);
    
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        s_hfu_reco_count = 0;
        s_is_auto_reco = FALSE;
        MMIBT_StopRecoTimer();
        
        MMINV_WRITE(MMINV_BT_ADDRESS_INFO,&address);
        SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf Device connect success!");
        //MMIPUB_OpenAlertSuccessWin(TXT_BT_DEVICE_CONNECTED);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_CONNECTED,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        /*if(MMK_IsOpenWin(MMIBT_BTPHONE_WIN_ID))
        {
            AppendOneLineTextIconListItem(MMIBT_BTPHONE_LIST_CTRL_ID,TXT_BT_CONNECTION,IMAGE_CHECK_SELECTED_ICON,BT_PHONE_INDEX_CONNECTION,TXT_BT_DEVICE_DISCONNECT,TRUE);
        }*/
        MMIBT_GetBtStatusForIcon();
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }

#ifndef WIN32
#if defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
        BT_SppConnect(&address, SPP_PORT_SPEED_115200);
#endif
#endif

        /* Connect to other HFG profile */
        ConnectHandsetGatewayService();
#ifdef BT_PBAP_SUPPORT
        MMIAPIPB_SetAllowSync(TRUE);
#endif
    }
    else if(BT_ERROR == msg_body_ptr->status)
    {
        if(s_is_auto_reco)
        {
            //bug 917715
            SCI_TRACE_LOW("lzk log.MMK_CloseWin(MMIBT_CONNECT_WAIT_WIN_ID)");
            MMK_CloseWin(MMIBT_CONNECT_WAIT_WIN_ID);	
            //bug 917715
            return;
        }
        SCI_TRACE_LOW("MMIBT_ConnectDeviceCnf Device connect fail!");
        //MMIPUB_OpenAlertFailWin(TXT_BT_DEVICE_CONNECT_ERROR);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_CONNECT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    
    MMK_CloseWin(MMIBT_CONNECT_WAIT_WIN_ID);
}

/*****************************************************************************/
//  Description : receive msg: AG device disconnected
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_DisconnectDeviceCnf(BT_MSG_T* msg_body_ptr)
{
    BT_ADDRESS     address = {0};
    MMI_WIN_ID_T   alert_id = MMI_BT_DEV_DISCONNECT_WIN_ID;
    BT_ADDRESS            dev_addr = {0};
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_DisconnectDeviceCnf");

    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf PNULL == msg_body_ptr!");
        return;
    }
    
    address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;
    SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf address = %x, %x, %x, %x, %x, %x", 
        address.addr[0],address.addr[1],address.addr[2],address.addr[3],address.addr[4],address.addr[5]);
    SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf msg_body_ptr->status %d", msg_body_ptr->status);
    
    if((BT_SUCCESS == msg_body_ptr->status) || (BT_TIMEOUT == msg_body_ptr->status))
    {
        SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf Device disconnect success!");
        //MMIPUB_OpenAlertSuccessWin(TXT_BT_DEVICE_DISCONNECTED);

        if(!MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECTED,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        /*if(MMK_IsOpenWin(MMIBT_BTPHONE_WIN_ID))
        {
            AppendOneLineTextIconListItem(MMIBT_BTPHONE_LIST_CTRL_ID,TXT_BT_CONNECTION,IMAGE_CHECK_UNSELECTED_ICON,BT_PHONE_INDEX_CONNECTION,TXT_COMMON_CONNECT_COMN,TRUE);
        }*/

        if(BT_TIMEOUT != msg_body_ptr->status)
        {
            /* Clean connected bt address data */
            SCI_MEMSET(&dev_addr,0,sizeof(BT_ADDRESS)); 
            MMINV_WRITE(MMINV_BT_ADDRESS_INFO,&dev_addr);
        }

        MMIBT_GetBtStatusForIcon();
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            //Update Device list
            MMK_PostMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, PNULL, PNULL);
        }

#if (defined MMI_SMS_SYN_SUPPORT) && (defined BT_SPP_SUPPORT)
#ifndef WIN32
        BT_SppDisconnect();
#endif 
        MMK_CloseWin(MMIBT_SYNC_SMS_WAIT_WIN_ID);
        s_is_sms_sync = FALSE;
#endif

        if(BT_TIMEOUT == msg_body_ptr->status)
        {
            MMIBT_HFUReconnect();
        }
    }
    else if(BT_ERROR == msg_body_ptr->status)
    {
        SCI_TRACE_LOW("MMIBT_DisconnectDeviceCnf Device disconnect error!");
        //MMIPUB_OpenAlertFailWin(TXT_BT_DEVICE_DISCONNECT_ERROR);
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
    
    MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    MMK_CloseWin(MMIBT_DISCONNECT_WAIT_WIN_ID);
#ifdef BT_PBAP_SUPPORT
    MMICL_ClosePBAPWaitWin();
    MMIBT_StopPbapSyncTimer();

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
    MMIAPISMS_StopMapSyncTimer();
#endif
    
    MMIBT_PbapDisconnect();
    //MMIBT_PbapClientDeregister();

#ifdef BT_HID_SUPPORT
    MMIAPIBT_HIDDisconnect();
#endif

    if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
    }
    if(MMK_IsOpenWin(MMIBT_SYNC_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
    }
#endif
}

/*****************************************************************************/
//  Description : MMIBT_DisconnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_DisconnectDevice(void)
{
#ifdef WIN32

#else
    MMI_STRING_T wait_text = {0};
	BT_STATUS    ret_val = BT_SUCCESS;
	MMI_WIN_ID_T alert_id = MMI_BT_DEV_DISCONNECT_WIN_ID;

	ret_val = BT_HfuDisconnect();

    SCI_TRACE_LOW("[MMIBT] MMIBT_DisconnectDevice ret_val = %d", ret_val);
	if(BT_PENDING == ret_val)
    {
     
       MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
       MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_DISCONNECT_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
    }
	else if(BT_SUCCESS == ret_val)
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECTED,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		MMIBT_GetBtStatusForIcon();
	}
	else if(BT_ERROR == ret_val)
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	}

#if defined(BT_MAP_SUPPORT)
    if(MMIAPIBT_MapIsConnected())
    {
        //MMIAPISMS_MapDisconnect();
    }
#endif

#endif
}

/*****************************************************************************/
//  Description : MMIBT_ConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_ConnectDevice(BT_ADDRESS *dev_addr)
{
#ifdef WIN32

#else
    MMI_STRING_T wait_text = {0};
	BT_STATUS    ret_val = BT_SUCCESS;
	MMI_WIN_ID_T alert_id = MMI_BT_DEV_CONNECT_WIN_ID;

	ret_val = BT_HfuConnect(dev_addr);
	if(BT_PENDING == ret_val)
    {
     
       MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &wait_text);
       MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);
    }
	else if(BT_SUCCESS == ret_val)
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_CONNECTED,TXT_NULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		MMIBT_GetBtStatusForIcon();
	}
	else if(BT_ERROR == ret_val)
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_CONNECT_ERROR,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
	}
#endif
}

/*****************************************************************************/
//  Description : MMIBT_IsConnectDevice
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectDevice(BT_ADDRESS *dev_addr)
{
#ifdef WIN32
    return TRUE;
#else
    BOOLEAN is_connect = FALSE;

    if(MMIBT_HasConnected(dev_addr))
    {
		is_connect = BT_HfuGetConnStatus(dev_addr);
	}

    SCI_TRACE_LOW("[MMIBT] MMIBT_IsConnectDevice is_connect = %d", is_connect);
	return is_connect;
#endif
}

/*****************************************************************************/
//  Description : MMIBT_HasConnected
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_HasConnected(BT_ADDRESS *dev_addr)
{
    uint8    default_addr[BT_ADDRESS_SIZE] = {0};
    BOOLEAN  has_connect = TRUE;
	
	MMIBT_GetPairedDevAddr(dev_addr);

	if(0 == memcmp(dev_addr, default_addr, sizeof(BT_ADDRESS)))
	{
    	has_connect = FALSE;
	}

    SCI_TRACE_LOW("[MMIBT] MMIBT_HasConnected has_connect = %d", has_connect);
	return has_connect;
}

/*****************************************************************************/
//  Description : MMIBT_IsBluetoothMode
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBT_IsBluetoothMode(void)
{
    BOOLEAN is_bt_mode = FALSE;
    MN_RETURN_RESULT_E    return_value             = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_BT_IS_BTMODE,&is_bt_mode,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_bt_mode = FALSE;
        MMINV_WRITE(MMINV_BT_IS_BTMODE,&is_bt_mode);
    }
    return is_bt_mode;
}

/*****************************************************************************/
//  Description : MMIBT_GetPairedDevAddr
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_GetPairedDevAddr(BT_ADDRESS *dev_addr)
{
	MN_RETURN_RESULT_E    return_value             = MN_RETURN_FAILURE;
    
    SCI_MEMSET(dev_addr, 0 , sizeof(BT_ADDRESS));
    MMINV_READ(MMINV_BT_ADDRESS_INFO,dev_addr,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(dev_addr,0,sizeof(BT_ADDRESS)); 
        MMINV_WRITE(MMINV_BT_ADDRESS_INFO,dev_addr);
    }
}

/*****************************************************************************/
//  Description : MMIBT_Init_NVItems
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_Init_NVItems(void)
{
    MN_RETURN_RESULT_E    return_value = MN_RETURN_FAILURE;
    BT_ADDRESS            dev_addr = {0};
	
    MMINV_READ(MMINV_BT_ADDRESS_INFO,&dev_addr,return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(&dev_addr,0,sizeof(BT_ADDRESS)); 
        MMINV_WRITE(MMINV_BT_ADDRESS_INFO,&dev_addr);
    }
}

/*****************************************************************************/
//  Discription: Clean BT Dialer Sync Data  
//  
//  Global resource dependence: None
//  Author: Cheney.Wu
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_CleanBtDialerSyncData(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIAPIBT_CleanBtDialerSyncData");
#ifdef BT_PBAP_SUPPORT
    MMIAPIPB_DeleteAllPbapSyncData();
    MMIAPICL_DeleteAllPbapSyncData();
#endif
#ifdef BT_MAP_SUPPORT
    MMIAPISMS_DeleteAllMapSyncInfo();
#endif
}

/*****************************************************************************/
//  Description : MMIBT_StartRecoTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_StartRecoTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_StartRecoTimer enter");

	if(s_hfu_reco_timer != 0) 
	{
		MMIBT_StopRecoTimer();
	}
    
	s_hfu_reco_timer = MMK_CreateTimerCallback(BT_HFU_RECO_TIMER_OUT, MMIBT_RecoTimeOut, 0, FALSE);
	
    SCI_TRACE_LOW("[MMIBT] MMIBT_StartRecoTimer exit, timer_id = %d", s_hfu_reco_timer);
}

/*****************************************************************************/
//  Description : MMIBT_StopRecoTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_StopRecoTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopRecoTimer enter");
    
    if(s_hfu_reco_timer != 0)    
    {   
        MMK_StopTimer(s_hfu_reco_timer);   
        s_hfu_reco_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_StopRecoTimer exit, timer_id = %d", s_hfu_reco_timer);
}

/*****************************************************************************/
//  Description : MMIBT_HFUReconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_RecoTimeOut(uint8 timer_id, uint32 param)
{
    MMIBT_StopRecoTimer();
    s_hfu_reco_count++;
    SCI_TRACE_LOW("[MMIBT] MMIBT_RecoTimeOut, s_hfu_reco_count = %d", s_hfu_reco_count);
    
    if(s_hfu_reco_count < BT_HFU_RECO_COUNT)
    {
        MMIAPIBT_ConnectDevice();
        MMIBT_StartRecoTimer();
    }
    else
    {
        s_is_auto_reco = FALSE;
    }
}

/*****************************************************************************/
//  Description : MMIBT_HFUReconnect
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_HFUReconnect()
{
    s_hfu_reco_count = 0;
    s_is_auto_reco = TRUE;
    MMIAPIBT_ConnectDevice();
    MMIBT_StartRecoTimer();
}

/*****************************************************************************/
//  Discription: get the device is active earphone by address of addr_ptr
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsDeviceActivedHandsetGateway(
                                                   const BT_ADDRESS    *addr_ptr
                                                  )
{
    BOOLEAN         connected = FALSE;
    
    if (PNULL == addr_ptr)
    {
        //SCI_TRACE_LOW:"MMIBT_IsDeviceActivedEarphone PNULL == addr_ptr!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_1633_112_2_18_2_8_2_239,(uint8*)"");
        return FALSE;
    }

#ifndef WIN32
    connected = BT_HfuGetConnStatus(addr_ptr);/*lint !e605 */
#endif
    
    SCI_TRACE_LOW("MMIBT_IsDeviceActivedHandsetGateway: return value is %d", connected);
    
    return  connected;
}

/*****************************************************************************/
//  Description : MMIBT_IsConnectToAddr
//  Global resource dependence : 
//  Author:Cheney.Wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsConnectToAddr(BT_ADDRESS *dev_addr)
{
#ifdef WIN32
    return FALSE;
#else
    BOOLEAN     is_connect = FALSE;
    BT_ADDRESS  default_addr = {0};
    
    if(MMIBT_HasConnected(&default_addr))
    {
        if(0 == memcmp(dev_addr, &default_addr, sizeof(BT_ADDRESS)))
        {
            is_connect = BT_HfuGetConnStatus(dev_addr);
        }
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_IsConnectToAddr is_connect = %d", is_connect);
    return is_connect;
#endif
}

/*****************************************************************************/
//  Discription:check the deviece is connected headset
//  Global resource dependence: none 
//  Author: 
//  Note: if addr == NULL, it will check all paired device to find exist Connected Headset,
//        the connected headset will return in  out_addr
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_DeviceIsConnectedHandsetGateway(BT_ADDRESS * in_addr)
{
    BOOLEAN ret = FALSE;
    BOOLEAN is_single_service = TRUE;
    uint32  service_type = 0;

    if(PNULL != in_addr)
    {
        //only check in_addr is connected headset or not
        //get current earphone status, if is active do not allow to delete device
        service_type = BT_GetPairedDeviceService(in_addr);
        if ((BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY) & service_type)
        {
            ret = MMIBT_IsDeviceActivedHandsetGateway(in_addr);
            is_single_service = FALSE;
        }

// Not necessary to check PBAP and MAP connection. 
#if 0
#ifdef BT_PBAP_SUPPORT
        if(!MMIBT_IsConnectToAddr(in_addr))
        {
            return FALSE;
        }
        // Not necessary to check PBAP connection. 
        if (BT_SERVICE_PBAP_SERVER & service_type)
        {
            if(is_single_service)
            {
                ret =  MMIAPIBT_PbapIsConnected();
            }
            else
            {
                ret &= MMIAPIBT_PbapIsConnected();
            }
            SCI_TRACE_LOW("[MMIBT] MMIBT_DeviceIsConnectedHandsetGateway: is connect to PBAP = %d", ret);
        }
#endif
        // Not necessary to check MAP connection. 
#ifdef BT_MAP_SUPPORT
        if (BT_SERVICE_MAP_SERVER & service_type)
        {
            if(is_single_service)
            {
                ret =  MMIAPIBT_MapIsConnected();
            }
            else
            {
                ret &= MMIAPIBT_MapIsConnected();
            }
            SCI_TRACE_LOW("[MMIBT] MMIBT_DeviceIsConnectedHandsetGateway: is connect to MAP = %d", ret);
        }
#endif
#endif
    }
    //SCI_TRACE_LOW:"MMIBT_DeviceIsConnectedHeadset ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9016_112_2_18_2_8_18_312,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//  Discription:check exist opened headset or not
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_OpenedHandsetGatwayIsExist(BT_DEVICE_INFO *out_dev)
{
    uint32 device_num = BT_GetPairedDeviceCount(BT_SERVICE_HANDFREE_GATEWAY);    
    uint32 i = 0;
    BT_STATUS status = BT_SUCCESS;
    BT_DEVICE_INFO dev_info = {0};
    BOOLEAN   exist = FALSE;

    for(i = 1; i <= device_num; i++)
    {
        status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDFREE_GATEWAY, i, &dev_info);
        if(BT_SUCCESS == status)
        {
            if(MMIBT_DeviceIsConnectedHeadset(&(dev_info.addr)))
            {
                exist = TRUE;
                if(PNULL != out_dev)
                {
                    SCI_MEMCPY(out_dev, &dev_info, sizeof(BT_DEVICE_INFO));
                }
                break;
            }
        }
        
    }//f
    //SCI_TRACE_LOW:"MMIBT_OpenedHeadsetIsExist exist %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9077_112_2_18_2_8_18_315,(uint8*)"d",exist);
    return exist;
}

/*****************************************************************************/
//  Discription:check headset all service is close
//  Global resource dependence: none 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN HandsetGatewayIsClosed(BT_ADDRESS * in_addr, BT_ADDRESS *out_addr)
{
    BOOLEAN ret = TRUE;
    BOOLEAN is_active = FALSE;
    uint32      service_type = 0;
    BT_DEVICE_INFO dev_info = {0};
    BT_STATUS    status = 0;
    uint32      device_num = 0;
    uint32       i = 0;
    
    if(PNULL != in_addr)
    {
        //only check in_addr is connected headset or not
        //get current earphone status, if is active do not allow to delete device
        service_type = BT_GetPairedDeviceService(in_addr);

        if ((BT_SERVICE_HEADSET_GATEWAY|BT_SERVICE_HANDFREE_GATEWAY) & service_type)
        {
            is_active = MMIBT_IsDeviceActivedHandsetGateway(in_addr);//MMIBT_GetCurrentEarphoneStatus(in_addr);
        }
        if (BT_SERVICE_A2DP_SRC & service_type)
        {
//            is_active |= MMIBT_GetCurrentA2dpStatus(in_addr);
        }
        if (BT_SERVICE_A2DP_SRC & service_type)
        {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
        }
        if (BT_SERVICE_PBAP_SERVER & service_type)
        {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
        }
        if (BT_SERVICE_MAP_SERVER & service_type)
        {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
        }
        if (BT_SERVICE_AVRCP_TG & service_type)
        {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
        }
        if(is_active)
        {
            if(PNULL != out_addr)
            {
                SCI_MEMCPY(out_addr, in_addr, sizeof(BT_ADDRESS));
            }
        }
    }
    else
    {
        //check all paired device to find exist Connected Headset
        device_num = BT_GetPairedDeviceCount(BT_SERVICE_HANDFREE_GATEWAY);   
        for(i = 1; i <= device_num; i++)
        {
            status = BT_GetPairedDeviceInfo(BT_SERVICE_HANDFREE_GATEWAY, i, &dev_info);
            if(BT_SUCCESS == status)
            {
                service_type = BT_GetPairedDeviceService(&dev_info.addr);
                if (BT_SERVICE_HANDFREE_GATEWAY & service_type)
                {
                    is_active = MMIBT_IsDeviceActivedEarphone(&dev_info.addr);//MMIBT_GetCurrentEarphoneStatus(&dev_info.addr);
                }
                if (BT_SERVICE_A2DP_SRC & service_type)
                {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
                if (BT_SERVICE_PBAP_SERVER & service_type)
                {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
                if (BT_SERVICE_MAP_SERVER & service_type)
                {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
                if (BT_SERVICE_AVRCP_TG & service_type)
                {
//                    is_active |= MMIBT_GetCurrentA2dpStatus(&dev_info.addr);
                }
                if(is_active)
                {
                    if(PNULL != out_addr)
                    {
                        SCI_MEMCPY(out_addr, &dev_info.addr, sizeof(BT_ADDRESS));
                    }                   
                    break;
                }
            }//if(BT_SUCCESS == status)
        }//for(i = 1; i <= device_num; i++)
    }
    ret = !is_active;
    //SCI_TRACE_LOW:"HeadsetIsClosed ret %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_9149_112_2_18_2_8_19_316,(uint8*)"d",ret);
    return ret;
}

/*****************************************************************************/
//  Description : handle headset close result
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void handleHandsetGatewayCloseResult(BT_STATUS bt_status, BT_ADDRESS *headset_addr_ptr)
{
    //wchar wait_text_buf[MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH] = {0};
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    BT_ADDRESS   connect_addr = {0};
    MMIBT_OPERATION_INFO_T bt_opertor = {0};

	bt_opertor = MMIBT_GetOpertor();
	
    //wait_text.wstr_ptr = wait_text_buf;    
    if((BT_SUCCESS == bt_status) || (BT_NO_CONNECTION == bt_status))
    {
        //close finished
        if(MMIBT_OPC_OPEN_HANDSET_GATEWAY != bt_opertor.current_opertion)
        {
            //xingdong_bt
            //no HFG and A2DP need to active, show "Already Closed"
            if(HandsetGatewayIsClosed(PNULL, PNULL))
            {
                MMIDEFAULT_SetBackLight(TRUE);
                MMI_GetLabelTextByLang(TXT_BT_HEADSET_CLOSED, &wait_text);
                
                //ConnectUSC2String(TXT_NULL, g_current_bt.name, TXT_BT_HEADSET_ALREADY_CLOSED, MMIBT_PAIR_QUERY_TEXT_MAX_LENGTH, &wait_text);
                MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                MMIBT_GetBtStatusForIcon();
                MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
            }
            //SetBackgroupRun(FALSE);
        }
        else
        { 
            //current operation not close headset, need continue to do current_opertion
            //xingdong_bt
            //SetBackgroupRun(FALSE);
            DoOperation(bt_opertor.current_opertion, FALSE);
        }
    }
    else if (BT_PENDING == bt_status)
    { //Closing...
        //xingdong_bt
        
        if(HeadsetIsClosed(PNULL, &connect_addr) || (!memcmp(headset_addr_ptr, &connect_addr, sizeof(BT_ADDRESS))))
        {
            if(!MMK_IsOpenWin(MMIBT_EARCONN_WAIT_WIN_ID))// && (!GetIsBackgroupRun()))
            {
                if(MMIBT_OPC_OPEN_HANDSET_GATEWAY == bt_opertor.current_opertion)
                {
                    MMI_GetLabelTextByLang(TXT_BT_ACTIVATING, &wait_text);
                }
                else
                {
                    //Dective
                    MMI_GetLabelTextByLang(TXT_BT_DISCONNECTING, &wait_text);
                }
                MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_EARCONN_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_NONE,HandlePubwinWaitingMsg);           
            }
        }
    }
    else
    {
        //xingdong_bt
        //close error
        //if(!GetIsBackgroupRun())
        {
            MMIDEFAULT_SetBackLight(TRUE);
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_CANNOT_FIND_DEVICE,TXT_NULL,IMAGE_PUBWIN_FAIL,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
        }
        //SetBackgroupRun(FALSE);
    } 
}

/*****************************************************************************/
//  Description : open headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void OpenHandsetGateway(const BT_ADDRESS *src_addr)
{
    uint32 dev_service = 0;
    MMI_STRING_T wait_text = {0};
    MMI_WIN_ID_T alert_id = MMI_BT_REMOTE_ALERT_WIN_ID;
    MMI_WIN_ID_T connect_alert_id = MMI_BT_CONNECT_CNF_WIN_ID;
    BT_ADDRESS   dev_addr = {0};

    //open headset
    //SCI_TRACE_LOW:"OpenHeadset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10737_112_2_18_2_8_22_343,(uint8*)"");

#ifdef BT_DIALER_SUPPORT
    if(MMIBT_IsConnectDevice(&dev_addr) && (memcmp(src_addr, &dev_addr, sizeof(BT_ADDRESS))))
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_HFU_CONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
#endif

    if(!HeadsetIsClosed(PNULL, &dev_addr))
    {
        //exist not closed service, close it first
        BT_STATUS return_value = CloseHeadset(&dev_addr);
        handleHandsetGatewayCloseResult(return_value, &dev_addr);
        return;
    }

#ifndef WIN32
     if(BT_HfuGetConnStatus((BT_ADDRESS *)src_addr))
     {
         ConnectHandsetGatewayService();
         return;
     }
#endif

    dev_service = BT_GetPairedDeviceService(src_addr);
    
    MMIDEFAULT_SetBackLight(TRUE);
    MMK_CloseWin(alert_id);
    MMI_GetLabelTextByLang(TXT_BT_HEADSET_OPENED, &wait_text);
    MMIPUB_OpenAlertWinByTextPtr(PNULL,&wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,
        &connect_alert_id,PNULL,MMIPUB_SOFTKEY_ONE,HandleConnectCnfAlertWin);
    MMIBT_GetBtStatusForIcon();

    MMK_CloseWin(MMIBT_EARCONN_WAIT_WIN_ID);
    
    MMIBT_ConnectDevice((BT_ADDRESS *)src_addr);
}

/*****************************************************************************/
//  Description : close headset
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL BT_STATUS CloseHandsetGateway(const BT_ADDRESS *src_addr)
{
    uint32 dev_service = BT_GetPairedDeviceService(src_addr);
    BT_STATUS  return_value = BT_ERROR;
    int8       retry_count = 0;
    
    //SCI_TRACE_LOW:"CloseHeadset enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBT_WINTAB_10694_112_2_18_2_8_22_342,(uint8*)"");

#ifndef WIN32
#if defined(BT_PBAP_SUPPORT)
    if((BT_SERVICE_PBAP_SERVER & dev_service) && (TRUE == BT_PbapIsConnected()))
    {
        //HFG need to close
        do 
        {
            return_value =  BT_PbapDisconnect();
            if((BT_SUCCESS == return_value) || (BT_NO_CONNECTION == return_value))
            {
                break;
            }
            SCI_Sleep(100);
            retry_count++;
            if((retry_count > MMIBT_DISCONNECT_RETRY_COUNT) || (FALSE == BT_PbapIsConnected()))
            {
                break;
            }
            SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting PBAP return_value = 0x%d", return_value);
        } while(BT_PENDING != return_value);
    }
#endif
    
#if defined(BT_MAP_SUPPORT)
    if((BT_SERVICE_MAP_SERVER & dev_service) && (TRUE == BT_MapIsConnected()))
    {
        retry_count = 0;
        //HFG need to close
        do 
        {
            return_value =  BT_MapDisconnect();
            if((BT_SUCCESS == return_value) || (BT_NO_CONNECTION == return_value))
            {
                break;
            }
            SCI_Sleep(100);
            retry_count++;
            if((retry_count > MMIBT_DISCONNECT_RETRY_COUNT) || (FALSE == BT_MapIsConnected()))
            {
                break;
            }
            SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting MAP return_value = 0x%d", return_value);
        } while(BT_PENDING != return_value);
    }
#endif

#if defined(BT_AV_SNK_SUPPORT)
    if((BT_SERVICE_AVRCP_TG & dev_service) && (TRUE == MMIAPIBT_IsAvrcpCtConnected()))
    {
        retry_count = 0;
        //HFG need to close
        do 
        {
            return_value =  AVRCP_CT_Disconnect();
            if((BT_SUCCESS == return_value) || (BT_NO_CONNECTION == return_value))
            {
                break;
            }
            SCI_Sleep(100);
            retry_count++;
            if((retry_count > MMIBT_DISCONNECT_RETRY_COUNT) || (FALSE == MMIAPIBT_IsAvrcpCtConnected()))
            {
                break;
            }
            SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting AVRCP CT return_value = 0x%d", return_value);
        } while(BT_PENDING != return_value);
    }
#endif

#endif
    
    if((BT_SERVICE_HANDFREE_GATEWAY |BT_SERVICE_HEADSET_GATEWAY) & dev_service && MMIBT_IsDeviceActivedHandsetGateway(src_addr))
    {
        //HFG need to close
        SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting HFu return_value = 0x%d", return_value);
#ifndef WIN32
        return_value =  BT_HfuDisconnect();
#endif
    }
    else
    {
        return_value = BT_SUCCESS;
    }

#ifndef WIN32
#if defined(BT_AV_SNK_SUPPORT)
    if((BT_SERVICE_A2DP_SRC & dev_service) && (TRUE == MMIAPIBT_IsA2dpSinkConnected()))
    {
        retry_count = 0;
        //HFG need to close
        do 
        {
            return_value =  A2DP_Sink_Disconnect();
            if((BT_SUCCESS == return_value) || (BT_NO_CONNECTION == return_value))
            {
                break;
            }
            SCI_Sleep(100);
            retry_count++;
            if((retry_count > MMIBT_DISCONNECT_RETRY_COUNT) || (FALSE == MMIAPIBT_IsA2dpSinkConnected()))
            {
                break;
            }
            SCI_TRACE_LOW("[MMIBT] CloseHandsetGateway disconnecting A2DP SINK return_value = 0x%d", return_value);
        } while(BT_PENDING != return_value);
    }
#endif
#endif
    
    return return_value;    
}

/*****************************************************************************/
//  Description : ConnectHandsetGatewayService
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void ConnectHandsetGatewayService(void)
{
//     uint16  msg_id = MMI_BT_HFG_SERVICE_CONNECT;
//     DPARAM  param = SCI_ALLOC_APP(10);
// 
//     SCI_TRACE_LOW("[MMIBT] ConnectHandsetGatewayService send signal MMI_BT_HFG_SERVICE_CONNECT");
//     MMIBT_SendSignal(msg_id, param);
    MMIBT_ConnectHandsetGatewayService();
}

/*****************************************************************************/
//  Description : MMIBT_ConnectHandsetGatewayProfile
//  Global resource dependence : none
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_ConnectHandsetGatewayService(void)
{
#ifndef WIN32
    uint32       dev_service = 0;
    BT_ADDRESS   dev_addr = {0};

    MMIAPIBT_GetHfuConnectedAddress(&dev_addr);
    SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService entry");
    if(BT_HfuGetConnStatus(&dev_addr))
    {
        dev_service = BT_GetPairedDeviceService(&dev_addr);

        SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService dev_service = 0x%x", dev_service);

        if((BT_SERVICE_PBAP_SERVER & dev_service) && (FALSE == MMIAPIBT_PbapIsConnected()))
        {
            SCI_Sleep(100);
            SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService connecting to PBAP");
            MMIAPIBT_DirectConnectPbapService();
        }
#ifdef BT_AV_SNK_SUPPORT
        else if((BT_SERVICE_A2DP_SRC & dev_service) && (FALSE == MMIAPIBT_IsA2dpSinkConnected()))
        {
            SCI_Sleep(100);
            SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService connecting to A2DP Sink");
            MMIAPIBT_A2dpSinkConnect(dev_addr);
        }
#endif
#ifdef BT_HID_SUPPORT
        else if(FALSE == MMIAPIBT_IsHIDConnected())
        {
            SCI_Sleep(100);
            SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService connecting to HID");
            MMIAPIBT_HIDConnect(dev_addr);
        }
#endif
#ifdef BT_MAP_SUPPORT
        else if((BT_SERVICE_MAP_SERVER & dev_service) && (FALSE == MMIAPIBT_MapIsConnected()))
        {
            SCI_Sleep(100);
            SCI_TRACE_LOW("[MMIBT] MMIBT_ConnectHandsetGatewayService connecting to MAP");
            MMIAPIBT_DirectConnectMapService();
        }
#endif
    }
#endif
}

/*****************************************************************************/
//  Description : Auto connect to handset gateway if authorization
//  Global resource dependence : none
//  Author: Cheney.Wu
//  Note:
/*****************************************************************************/
LOCAL void AutoConnectHandsetGateway(uint8 timer_id, uint32 param)
{
    BOOLEAN     be_authorize = FALSE;
    BT_ADDRESS  dev_addr = {0};
    
    if(MMIBT_HasConnected(&dev_addr))
    {
        BT_GetDeviceAuthorisation(&dev_addr, &be_authorize);
        
        if(be_authorize)
        {
            MMIAPIBT_ConnectDevice();
        }
    }
}

/*****************************************************************************/
//  Description : StopAutoConnectHandsetGatewayTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StopAutoConnectHandsetGatewayTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] StopAutoConnectHandsetGatewayTimer enter");
    
    if(s_bt_hfu_auto_connect_timer != 0)    
    {   
        MMK_StopTimer(s_bt_hfu_auto_connect_timer);   
        s_bt_hfu_auto_connect_timer = 0;    
    }   
    
    SCI_TRACE_LOW("[MMIBT] StopAutoConnectHandsetGatewayTimer exit, timer_id = %d", s_bt_hfu_auto_connect_timer);
}

/*****************************************************************************/
//  Description : StartAutoConnectHandsetGatewayTimer
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void StartAutoConnectHandsetGatewayTimer(void)
{
    SCI_TRACE_LOW("[MMIBT] StartAutoConnectHandsetGatewayTimer enter");
    
    if(s_bt_hfu_auto_connect_timer != 0) 
    {
        StopAutoConnectHandsetGatewayTimer();
    }
    
    s_bt_hfu_auto_connect_timer = MMK_CreateTimerCallback(BT_HFU_AUTO_CONNECT_TIMER_OUT, AutoConnectHandsetGateway, 0, FALSE);
    
    SCI_TRACE_LOW("[MMIBT] StartAutoConnectHandsetGatewayTimer exit, timer_id = %d", s_bt_hfu_auto_connect_timer);
}

/*****************************************************************************/
//  Description: Is open BT Remote Alert WIN  
//  Global resource dependence: none 
//  Author: Cheney Wu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_IsCloseingHeadsetGateway(void)
{

    MMIBT_OPERATION_INFO_T  bt_opc = MMIBT_GetOpertor();
    if(MMIBT_OPC_CLOSE_HANDSET_GATEWAY == bt_opc.current_opertion)
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : receive msg: MMIBT_GetHfuConnectedAddress
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetHfuConnectedAddress(BT_ADDRESS *addr)
{
    uint16	device_number =0;
    BT_DEVICE_INFO dev = {0};
    uint16      index =0;
    BOOLEAN connected = FALSE;
    
    device_number = BT_GetPairedDeviceCount(BT_SERVICE_ALL);
    
    for (index = 1; index <= device_number; index++)
    {
        BT_GetPairedDeviceInfo(BT_SERVICE_ALL, index, &dev);
        SCI_TRACE_LOW("BT_AV, paired address: %02X-%02X-%02X-%02X-%02X-%02X", dev.addr.addr[0], dev.addr.addr[1], dev.addr.addr[2], dev.addr.addr[3], dev.addr.addr[4], dev.addr.addr[5]);
        
#ifndef WIN32
        connected = BT_HfuGetConnStatus(&dev.addr);
        SCI_TRACE_LOW("BT_AV, is connected: %d", connected);
#endif
        
        if(connected)
        {
            SCI_TRACE_LOW("BT_AV, connected address: %02X-%02X-%02X-%02X-%02X-%02X", dev.addr.addr[0], dev.addr.addr[1], dev.addr.addr[2], dev.addr.addr[3], dev.addr.addr[4], dev.addr.addr[5]);
            memcpy(addr, &dev.addr, sizeof(BT_ADDRESS));
            return TRUE;
        }
    }
    
    for (index = 0; index < BT_ADDRESS_SIZE; index++)
    {
        dev.addr.addr[index] = 0;
        memcpy(addr, &dev.addr, sizeof(BT_ADDRESS));
    }
    
    return FALSE;
}

#endif

#ifdef BT_SPP_SUPPORT
/*****************************************************************************/
//  Description : MMIBT_SPPConnectInd
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SppConnectInd(BT_MSG_T* msg_body_ptr)
{
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("MMIBT_SPPConnectInd PNULL == msg_body_ptr!");
        return;
    }

    if(BT_SUCCESS != msg_body_ptr->status)
    {
        SCI_TRACE_LOW("MMIBT_SppConnectInd bt_status = %d, != BT_SUCCESS !!!", msg_body_ptr->status);
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
}

/*****************************************************************************/
//  Description : MMIBT_SPPStatusInd
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SppStatusInd(BT_MSG_T* msg_body_ptr)
{
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("MMIBT_SPPStatusInd PNULL == msg_body_ptr!");
        return;
    }

    if(BT_SUCCESS == msg_body_ptr->status)
    {
        SCI_TRACE_LOW("MMIBT_SppStatusInd The max data value is %d", *((int*)(msg_body_ptr->body_ptr)));
#ifdef MMI_SMS_SYN_SUPPORT
        if(s_is_sms_sync)
        {
            MMIAPISMS_StartSynSMS();
        }
#endif
    }
    else
    {
        SCI_TRACE_LOW("MMIBT_SppStatusInd bt_status = %d, != BT_SUCCESS !!!", msg_body_ptr->status);
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
}

/*****************************************************************************/
//  Description : MMIBT_SppDataRecieveInd
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SppDataRecieveInd(BT_MSG_T* msg_body_ptr)
{
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("MMIBT_SppDataRecieveInd PNULL == msg_body_ptr!");
        return;
    }

    if(BT_SUCCESS == msg_body_ptr->status)
    {
#ifdef MMI_SMS_SYN_SUPPORT
        MMIAPISMS_GetData(msg_body_ptr->body_ptr);
#endif
    }
    else
    {
        SCI_TRACE_LOW("MMIBT_SppDataRecieveInd bt_status = %d, != BT_SUCCESS !!!", msg_body_ptr->status);
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
}
#endif

#if defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT) && defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT)
/*****************************************************************************/
//  Description : MMIBT_SMSSyncBegin
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SMSSyncBegin(void)
{
    MMI_STRING_T    wait_text = {0};
    BT_ADDRESS      dev_addr  = {0};
    BT_STATUS       bt_status  = BT_ERROR;
    uint32       dev_service = 0;

    MMIAPIBT_GetHfuConnectedAddress(&dev_addr);
    if(BT_HfuGetConnStatus(&dev_addr))
    {
        dev_service = BT_GetPairedDeviceService(&dev_addr);

        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSSyncBegin dev_service = 0x%x", dev_service);

       if(BT_SERVICE_MAP_SERVER & dev_service)
        {
		   if( FALSE == MMIAPIBT_MapIsConnected())
		   {
			   SCI_Sleep(100);
			   SCI_TRACE_LOW("[MMIBT] MMIBT_SMSSyncBegin connecting to MAP");
			   MMIAPIBT_DirectConnectMapService();
		   }
        }
       else
        {
            if(!MMK_IsOpenWin(MMIBT_SYNC_SMS_WAIT_WIN_ID))
            {
                MMI_GetLabelTextByLang(TXT_BT_SYNC_SMS_WAITING, &wait_text); 
				MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_SYNC_SMS_WAIT_WIN_ID,IMAGE_NULL,
                    ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,PNULL); 
                //MMIPUB_SetWinSoftkey(MMIBT_SYNC_SMS_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
                s_is_sms_sync = TRUE;
                MMIAPISMS_SetSMSSyncType(MMISMS_SYNC_TYPE_SPP);
            }

            if(MMIBT_IsConnectDevice(&dev_addr))
            {
#ifndef WIN32

                bt_status = BT_SppConnect(&dev_addr, SPP_PORT_SPEED_115200);
                SCI_TRACE_LOW("[MMIBT] MMIBT_SMSSyncBegin BT_SppConnect result = %d", bt_status);
                if(bt_status == BT_SUCCESS)
                {
					MMISMS_DeleteBTInboxMail();
                    MMIAPISMS_StartSynSMS();   
                }
                else if (bt_status != BT_PENDING)
                {
                    MMIPUB_OpenAlertFailWin(TXT_ERROR);
                    MMK_CloseWin(MMIBT_SYNC_SMS_WAIT_WIN_ID);
                    s_is_sms_sync = FALSE;           
                }
#endif /* WIN32 */
            }        
        }
    }
}
#else
#if defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
/*****************************************************************************/
//  Description : MMIBT_SMSSyncBegin
//  Global resource dependence : 
//  Author:yang.yang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SMSSyncBegin(void)
{
    MMI_STRING_T    wait_text = {0};
    BT_ADDRESS      dev_addr  = {0};
    BT_STATUS       bt_status  = BT_ERROR;
    
    if(!MMK_IsOpenWin(MMIBT_SYNC_SMS_WAIT_WIN_ID))
    {
        MMI_GetLabelTextByLang(TXT_BT_SYNC_SMS_WAITING, &wait_text); 
	    MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_SYNC_SMS_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,PNULL); 
        //MMIPUB_SetWinSoftkey(MMIBT_SYNC_SMS_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
        s_is_sms_sync = TRUE;
    }

    if(MMIBT_IsConnectDevice(&dev_addr))
    {
#ifndef WIN32
        bt_status = BT_SppConnect(&dev_addr, SPP_PORT_SPEED_115200);
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSSyncBegin BT_SppConnect result = %d", bt_status);
        if(bt_status == BT_SUCCESS)
        {
            MMIAPISMS_StartSynSMS();   
        }
        else if (bt_status != BT_PENDING)
        {
            MMIPUB_OpenAlertFailWin(TXT_ERROR);
            MMK_CloseWin(MMIBT_SYNC_SMS_WAIT_WIN_ID);
            s_is_sms_sync = FALSE;           
        }
#endif /* WIN32 */
    }
}
#endif
#endif

#ifdef MMI_SMS_SYN_SUPPORT
/*****************************************************************************/
//  Discription: 
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetSmsSynValue(void)
{
    return s_is_sms_sync;
}
#endif

#ifdef BT_PBAP_SUPPORT
/*****************************************************************************/
//  Description : Create a transport connection to the specified device 
//                and issue an OBEX Connect Request
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapConnect(const BT_ADDRESS *addr)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;

    ret_val = BT_PbapConnect(addr);
    SCI_TRACE_LOW("[MMIBT_PbapConnect] ret_val = 0x%x", ret_val);
    
    return ret_val;
#else
	return BT_SUCCESS;
#endif
}

/*****************************************************************************/
//  Description : issue an OBEX Disconnect Request. When the OBEX Disconnect 
//  is completed, the transport connection will be disconnected automatically.
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIBT_PbapDisconnect(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapDisconnect();

	SCI_TRACE_LOW("[MMIBT_PbapDisconnect] ret_val = 0x%x", ret_val);
	if(BT_SUCCESS == ret_val)
	{
		MMIBT_PbapClientDeregister();
	}
	else if((BT_PENDING != ret_val) && (BT_BUSY != ret_val))
	{
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
	}
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : to initiate a transport disconnection. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note: BT_PbapDisconnect will cause the transport to be brought down automatically, 
//  but if the transport must be brought down immediately, perhaps due an error condition, 
//  this routine may be used. 
/*****************************************************************************/
PUBLIC void MMIBT_PbapClientTpDisconnect(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapClientTpDisconnect();

    SCI_TRACE_LOW("[MMIBT_PbapClientTpDisconnect] ret_val = 0x%x", ret_val);
    if(BT_SUCCESS == ret_val)
    {
        MMIBT_PbapDisconnect();
    }
    else if(BT_PENDING != ret_val)
    {
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : Initiates the OBEX "Get" operation to retrieve a phonebook object 
//                from the remote Phonebook Access Server. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapPullPhonebook(void)
{
#ifndef WIN32
    BT_PBAP_VCARD_FILTER_T  pbap_filter = {0};
    BT_PBAP_VCARD_FORMAT_T  format = MMIBT_PBAP_VCARD_FORMAT_21;//BT_VCARD_FORMAT_30;//
    uint16                  max_list_count = MMIPB_NV_MAX_RECORD_NUM;
    uint16                  list_offset = 0;
    
//#ifdef MMI_CONTACT_PHOTO_SUPPORT
    //SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x8d\x00\x00\x00\x00\x00\x00\x00", BT_PBAP_FILTER_SIZE*sizeof(uint8));
//#else
    SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x85\x00\x00\x00\x00\x00\x00\x00", BT_PBAP_FILTER_SIZE*sizeof(uint8));
//#endif
    return BT_PbapPullPhonebook(
                        s_pb_name,
                        &pbap_filter,
                        format,
                        max_list_count,
                        list_offset
                        );
#else
	return BT_PENDING;
#endif
}

/*****************************************************************************/
//  Description : Initiates the OBEX "Get" operation to retrieve a phonebook object 
//                from the remote Phonebook Access Server. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapPullSIMPhonebook(void)
{
#ifndef WIN32
    BT_PBAP_VCARD_FILTER_T  pbap_filter = {0};
    BT_PBAP_VCARD_FORMAT_T  format = MMIBT_PBAP_VCARD_FORMAT_21;//BT_VCARD_FORMAT_30;//
    uint16                  max_list_count = MMIPB_NV_MAX_RECORD_NUM;
    uint16                  list_offset = 0;
//#ifdef MMI_CONTACT_PHOTO_SUPPORT
    //SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x8d\x00\x00\x00\x00\x00\x00\x00", BT_PBAP_FILTER_SIZE*sizeof(uint8));
//#else
    SCI_MEMCPY(&(pbap_filter.byte[0]), (const uint8*)"\x85\x00\x00\x00\x00\x00\x00\x00", BT_PBAP_FILTER_SIZE*sizeof(uint8));
//#endif
    return BT_PbapPullPhonebook(
                        s_sim_name,
                        &pbap_filter,
                        format,
                        max_list_count,
                        list_offset
                        );
#else
	return BT_PENDING;
#endif
}

/*****************************************************************************/
//  Description : Registers the Phonebook Access Client
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
PUBLIC BT_STATUS MMIBT_PbapClientRegister(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;

    ret_val = BT_PbapRegisterClient();
    SCI_TRACE_LOW("[MMIBT_PbapClientRegister] ret_val = 0x%x", ret_val);
    
    return ret_val;
#else
	return BT_SUCCESS;
#endif
}

/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note:
/*****************************************************************************/
LOCAL void MMIBT_PbapClientDeregister(void)
{
#ifndef WIN32
	BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapDeregisterClient();

	SCI_TRACE_LOW("[MMIBT_PbapClientDeregister] ret_val = 0x%x", ret_val);
	if((BT_SUCCESS != ret_val) &&(BT_PENDING != ret_val))
	{
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
	}
	
#else
	return;
#endif
}

/*****************************************************************************/
//  Description : Aborts the current client operation. 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Note: The completion event will signal the status of the operation in progress, 
//  either COMPLETE or ABORTED. 
/*****************************************************************************/
PUBLIC void MMIBT_PbapAbortClient(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_PbapAbortClient();

    SCI_TRACE_LOW("[MMIBT_PbapAbortClient] ret_val = 0x%x", ret_val);
    if(BT_SUCCESS == ret_val)
    {
        MMIBT_PbapDisconnect();
    }
    else
    {
        MMIPUB_OpenAlertFailWin(TXT_BT_PULL_PHONEBOOK_ABORT_ERROR);
    }
#else
    return;
#endif
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapDataInd(BT_MSG_T* msg_body_ptr)
{
	if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapDataInd] PNULL == msg_body_ptr!");
        return;
    }

	SCI_TRACE_LOW("[MMIBT_PbapDataInd] Data has been received!");
    SCI_TRACE_LOW("[MMIBT_PbapDataInd] s_is_cancel_pbap_pull = %d", s_is_cancel_pbap_pull);
	if(!s_is_cancel_pbap_pull)
    {
        switch(s_pbap_connect_type)
        {
	        case BT_PBAP_TYPE_PB:
	        case BT_PBAP_TYPE_SIM:
	            if(!s_is_cancel_pbap_pull)
		        {
		            MMIBT_AddVcardToContact(msg_body_ptr);
		        }
	            break;
	        case BT_PBAP_TYPE_CL:
	            MMIBT_AddVcardToCalllog(msg_body_ptr);
	            break;
	        default:
	            break;
    	}
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapParamsRx(BT_MSG_T* msg_body_ptr)
{
	if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapParamsRx] PNULL == msg_body_ptr!");
        return;
    }

	SCI_TRACE_LOW("[MMIBT_PbapParamsRx] The Application Parameters header information has been fully parsed!");
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapAbort(BT_MSG_T* msg_body_ptr)
{
    BT_PBAP_OP_T        oper_type = BT_PBAPOP_NONE;
    BT_PBAP_RESP_CODE_T resp_code = BT_PBRC_STATUS_SUCCESS;
    
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapAbort] PNULL == msg_body_ptr!");
        return;
    }
    
    oper_type = (BT_PBAP_OP_T)(((BT_PBAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->oper_type);
    resp_code = (BT_PBAP_RESP_CODE_T)(((BT_PBAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->resp_code);
    
    SCI_TRACE_LOW("[MMIBT_PbapAbort] The operation is 0x%x, fail reason is 0x%x.", oper_type, resp_code);
    if(BT_PBAPOP_PULL_PHONEBOOK == oper_type)
    {
        if(BT_PBRC_USER_ABORT ==resp_code)
        {
// #ifndef BT_DIALER_DIRECT_CONNECT
//             MMIBT_PbapDisconnect();
// #endif
        }
        else if(BT_PBRC_LINK_DISCONNECT !=resp_code) 
        {
            if(BT_PBAP_TYPE_SIM == MMIBT_GetPbapRequestType())
            {
                 MMIAPIPB_RefreshList();
            }
            else
            {
                MMIPUB_OpenAlertFailWin(TXT_BT_PULL_PHONEBOOK_ERROR);
// #ifndef BT_DIALER_DIRECT_CONNECT
//                 MMIBT_PbapDisconnect();
// #endif
            }
        }
        
        s_is_cancel_pbap_pull = FALSE;

        SCI_TRACE_LOW("[MMIBT_PbapAbort] MMIBT_GetPbapRequestType() = %d", MMIBT_GetPbapRequestType());
        if(BT_PBAP_TYPE_CL == MMIBT_GetPbapRequestType())
        {
            MMICL_FreeAllPbapData();
        }
        else
        {
             MMIPB_FreeAllPbapData();
        }

        MMIBT_SetPbapRequestType(BT_PBAP_TYPE_NONE);
        if(MMK_IsOpenWin(MMIBT_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_WAIT_WIN_ID);
            MMIPUB_OpenAlertSuccessWin(STXT_CANCEL);
        }
        //需要确认complete情况
        
        if(MMK_IsOpenWin(MMIBT_SYNC_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
        }
        
        if(MMK_IsOpenWin(MMICL_SYNC_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMICL_SYNC_WAIT_WIN_ID);
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapComplete(BT_MSG_T* msg_body_ptr)
{
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapComplete] PNULL == msg_body_ptr!");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT_PbapComplete] The operation is 0x%x.", *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)));

    //Bug 944362 
    //lzk begin 2018.10.12
    //if pbap is connected, stop bt sync timer. just do it
    if (BT_PBAPOP_CONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
        }
        SCI_TRACE_LOW("lzk log. Pbap connect ok. stop timer.");
        MMIBT_StopPbapSyncTimer();
    }
    //lzk end 2018.10.12
    //Bug 944362 
    if(BT_PBAP_TYPE_CL == MMIBT_GetPbapRequestType())
    {
        if(BT_PBAPOP_CONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS to Calllog");
            MMK_SendMsg(MMIAPICL_GetActivateBTWinId(), MSG_BT_PBAP_TP_CONNECT_SUCCESS, PNULL);
        }
        else if(BT_PBAPOP_PULL_PHONEBOOK == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("[MMIBT_PbapComplete] MMICL_NotifyPBStartParser");
#if defined(TRACE_MAP_BUFFER)
            DEBUG_MakeList("CALLLOG");
#endif

            MMICL_NotifyPBStartParser((MMICL_PBAP_PARSER_CALLBACK_FUNC)MMIBT_PBPbapParserCallback);
        }
        else if(BT_PBAPOP_DISCONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
#if 0 //bug 926655 do PbapClientDeregister will fail here, move it to MMIBT_PbapTpDisconnect
            SCI_TRACE_LOW("[MMIBT_PbapComplete] MMICL_PbapClientDeregister");
            MMICL_PbapClientDeregister();
#endif			
        }
    }
    else
    {
        if(BT_PBAPOP_CONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("[MMIBT_PbapComplete] send MSG_BT_PBAP_TP_CONNECT_SUCCESS to BT UI");

            if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
            {
                MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
            }
             MMIAPIPB_SetAllowSync(TRUE);

             if(0 != MMIAPIPB_GetActivateBTWinId())
            {
                 MMIAPIPB_RefreshList();
            }  

            MMIBT_StopPbapSyncTimer();

            if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
            {
                MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
            }

            /* Connect to other service */
            ConnectHandsetGatewayService();
        }
        else if(BT_PBAPOP_PULL_PHONEBOOK == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
             MMIAPIPB_NotifyPBStartParser(MMIBT_PBPbapParserCallback);
        }
        else if(BT_PBAPOP_DISCONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
#if 0 //bug 926655 do PbapClientDeregister will fail here, move it to MMIBT_PbapTpDisconnect
#ifdef BT_DIALER_ORIG_SUPPORT
            MMIBT_PbapClientDeregister();
#else
             MMIAPIPB_PbapClientDeregister();
#endif
#endif
        }
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_PbapTpDisconnect(BT_MSG_T* msg_body_ptr)
{
    BT_PBAP_OP_T        oper_type = BT_PBAPOP_NONE;
#if MTOBEX_STACK_VERSION <= 380    /*lint !e553*/
    BT_PBAP_RESP_CODE_T resp_code = BT_PBRC_STATUS_SUCCESS;
#endif
    SCI_TRACE_LOW("MMIBT_PbapTpDisconnect");
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] PNULL == msg_body_ptr!");
        return;
    }
    
    oper_type = (BT_PBAP_OP_T)(((BT_PBAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->oper_type);
    SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] The operation is 0x%x", oper_type);
#if MTOBEX_STACK_VERSION <= 380    /*lint !e553*/
    resp_code = (BT_PBAP_RESP_CODE_T)(((BT_PBAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->resp_code);
    SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] The response is 0x%x", resp_code);
#endif
    SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] oper_type:%d",oper_type);
    if(BT_PBAPOP_NONE == oper_type)
    {
        MMIBT_StopPbapSyncTimer();
        MMIAPIPB_StopPBAPTimer();
        MMIAPICL_StopPbapTimer();
        if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
        {
        MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
        }
        if(MMK_IsOpenWin(MMIBT_SYNC_WAIT_WIN_ID))
        {
        MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
        }
         MMICL_ClosePBAPWaitWin();
        SCI_TRACE_LOW("[MMIBT_PbapTpDisconnect] MMIBT_GetPbapRequestType:%d",MMIBT_GetPbapRequestType());
//         if(BT_PBAP_TYPE_NONE == MMIBT_GetPbapRequestType())
//         {
//             MMIPUB_OpenAlertWarningWin(TXT_BT_PBAP_CONNECT_FAIL);
//         }
//bug 926655 do PbapClientDeregister here instead of in pbapcomplete
 #ifdef BT_DIALER_ORIG_SUPPORT
            MMIBT_PbapClientDeregister();
#else
             MMIAPIPB_PbapClientDeregister();
#endif
       
        s_is_cancel_pbap_pull = FALSE;
        MMIBT_SetPbapRequestType(BT_PBAP_TYPE_NONE);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
LOCAL void MMIBT_PBPbapParserCallback(MMIPB_PBAP_PARSER_RESULT_T *result)
{
    switch(result->ret)
    {
        case MMIPB_PBAP_PARSER_FINISHED:
			switch(s_pbap_connect_type)
	        {
	        case BT_PBAP_TYPE_PB:
	            SCI_TRACE_LOW("[MMIBT_PBPbapParserCallback] BT_PBAP_TYPE_PB");
	            // Get SIM data again
	            MMIBT_PbapPullSIMPhonebook();

	            break;

	        case BT_PBAP_TYPE_SIM:
	            SCI_TRACE_LOW("[MMIBT_PBPbapParserCallback] BT_PBAP_TYPE_SIM");
	            MMIPUB_OpenAlertSuccessWin(TXT_BT_SYNC_FINISHED);
	            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
	// #ifndef BT_DIALER_DIRECT_CONNECT
	//             MMIBT_PbapDisconnect();
	// #endif
	            MMIPB_FreeAllPbapData();
	            MMIAPIPB_RefreshList();
	            break;

	        case BT_PBAP_TYPE_CL:
 	            MMICL_ClosePBAPWaitWin();
	//            MMIPUB_OpenAlertSuccessWin(TXT_BT_SYNC_FINISHED);
	            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
	// #ifndef BT_DIALER_DIRECT_CONNECT
	//             MMIBT_PbapDisconnect();
	// #endif
	            break;

	        default:
 	            MMICL_ClosePBAPWaitWin();
	            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
 	            MMIAPIPB_RefreshList();
	// #ifndef BT_DIALER_DIRECT_CONNECT
	//             MMIBT_PbapDisconnect();
	// #endif
	            break;
	        }

			if(MMK_IsOpenWin(MMIBT_WAIT_WIN_ID))
	        {
	            MMK_CloseWin(MMIBT_WAIT_WIN_ID);
	        }

	        if(BT_PBAP_TYPE_PB == s_pbap_connect_type)
	        {
	            MMIBT_SetPbapRequestType(BT_PBAP_TYPE_SIM);
	        }
	        s_is_cancel_pbap_pull = FALSE;
	        break;
          
        case MMIPB_PBAP_PARSER_NO_SPACE:
            {
        		MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
                MMI_STRING_T    display_str = {0};
                wchar           temp_wchar[60] = {0};
        		
                SCI_TRACE_LOW("[MMIBT] MMIBT_PBPbapParserCallback MMIPB_PBAP_PARSER_NO_SPACE");
        		SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact sync_count:%d", result->sync_count);

            	/************************************************************************/
            	/* 拼接提示字符: 已同步XXX个联系人，空间已满                            */
            	/************************************************************************/ 
                display_str.wstr_ptr = temp_wchar;

                MMIBT_GenerateDisplayInfo(display_str.wstr_ptr,result->sync_count);
                display_str.wstr_len = MMIAPICOM_Wstrlen(display_str.wstr_ptr);

                /*MMIPUB_OpenAlertTextWinByTextPtr(PNULL,
        			&display_str,
        			PNULL,
        			PNULL,
        			MMIPUB_SOFTKEY_ONE,
        			PNULL);*/
                
        		MMIPUB_OpenAlertWinByTextPtr(PNULL,&display_str,PNULL,IMAGE_PUBWIN_WARNING,
                        &alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        		MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);

                MMICL_ClosePBAPWaitWin();
                //MMIBT_PbapAbortClient();
                MMIPB_FreeAllPbapData();
                MMIAPIPB_RefreshList();

        		s_is_cancel_pbap_pull = TRUE;
            }
            break;

        case MMIPB_PBAP_PARSER_CANCELED:
            MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
            MMICL_ClosePBAPWaitWin();
            //MMIBT_PbapAbortClient();
            MMIPB_FreeAllPbapData();
            MMIAPIPB_RefreshList();

    		s_is_cancel_pbap_pull = TRUE;
            SCI_TRACE_LOW("[MMIBT] HandlePubwinWaitingMsg sync has been canceled");
            break;
    }
}

/*******************************************************************************/
//  Description : generate whole display string
//  Parameter: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_GenerateDisplayInfo(wchar *info_ptr,uint32 sync_contact_count)
{
    MMI_STRING_T        text_str1                       = {0};
    MMI_STRING_T        text_str2                       = {0};
    char                count_str[5] = {0};
    uint32              count_len = 0;
    wchar               count_wstr[5] = {0};
	uint32              content_len                     = 0; 

    //left string    
    MMI_GetLabelTextByLang(TXT_BT_HAS_SYNC, &text_str1); 
	if(text_str1.wstr_ptr != PNULL)
	{
        SCI_MEMCPY(info_ptr, text_str1.wstr_ptr, text_str1.wstr_len * sizeof(wchar));
	}

    //middle string
    MMIAPICOM_Wstrcat(info_ptr, L" ");
    sprintf((char *)count_str, "%d", sync_contact_count);
    count_len = strlen((char *)count_str);
    MMIAPICOM_StrToWstr((uint8 *)count_str, count_wstr);
    MMIAPICOM_Wstrncat(info_ptr, count_wstr, MMIAPICOM_Wstrlen(count_wstr));
    MMIAPICOM_Wstrcat(info_ptr, L" ");
    
    //right string
    MMI_GetLabelTextByLang(TXT_BT_SYNC_TIP, &text_str2); 
    MMIAPICOM_Wstrncat(info_ptr, text_str2.wstr_ptr, text_str2.wstr_len * sizeof(wchar));
    
    content_len = MMIAPICOM_Wstrlen(info_ptr);    
    
    SCI_TRACE_LOW("MMIBT_GenerateDisplayInfo info len %d",content_len);
}

#ifdef WIN32
/*******************************************************************************/
//  Description :MMIBT_SimuNotifyCLStartParse
//  Parameter: void
//  Global resource dependence :                                
//  Author: Cheney.Wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_SimuNotifyCLStartParse(void)
{
    MMICL_NotifyPBStartParser((MMICL_PBAP_PARSER_CALLBACK_FUNC)MMIBT_PBPbapParserCallback);
}

/*******************************************************************************/
//  Description :MMIBT_SimuNotifyPBStartParse
//  Parameter: void
//  Global resource dependence :                                
//  Author: Cheney.Wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_SimuNotifyPBStartParse(void)
{
     MMIAPIPB_NotifyPBStartParser(MMIBT_PBPbapParserCallback);
}
/*******************************************************************************/
//  Description :init  BT_MSG_T  *msg_body_ptr, be used to test
//  Parameter: msg_body[in][out]: 
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : for test
/*****************************************************************************/
PUBLIC void MMIBT_InitPbapData(BT_MSG_T   *msg_body)
{

    pbap.oper_type = BT_PBAPOP_PULL_PHONEBOOK;
    pbap.is_first_block = TRUE;
    pbap.buff = SCI_ALLOC_APP(1042 * sizeof(char));
    SCI_MEMSET(pbap.buff, 0x00,(1042 * sizeof(char)));
	
	//if (PNULL == s_vcard_buff_ptr)
	{	
		sprintf(pbap.buff, "%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
                        "BEGIN:VCARD",
                        "VERSION:2.1",
                        "N:;aaa;;;",
						"TEL;CELL:55536",
                        "END:VCARD",
                        "BEGIN:VCARD",
                        "VERSION:2.1",
                        "N:;yanyan;;;",
                        "TEL;CELL:123456789",
                        "END:VCARD"
                        );
	}
//     else
// 	{
// 		sprintf(pbap.buff, "%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
//                          "IN:VCARD",
//                          "VERSION:2.1",     
// 						 "N:;ryjgfvngxhl;;;",
// 						 "TEL;CELL:1898556",
// 						 "END:VCARD",
// 						 "BEGIN:VCARD",
// 						 "VERSION:2.1",
// 						 "N:;yanyan.an;;;",
// 						 "TEL;CELL:123456789",
// 						 "END:VCARD"
//                         );
// 
// 	}
    pbap.len = strlen((char*)pbap.buff);

    msg_body->body_size = sizeof(BT_PBAP_DATA_IND_T);
    msg_body->status = BT_SUCCESS;
    msg_body->body_ptr = (BT_PBAP_DATA_IND_T*)&pbap;

}
#endif

/*******************************************************************************/
//  Description : MMIBT_SetPbapRequestType
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney wu
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_SetPbapRequestType(BT_PBAP_TYPE_E pbapType)
{
    s_pbap_connect_type = pbapType;
}

/*******************************************************************************/
//  Description : MMIBT_GetPbapRequestType
//  Parameter: 
//  Global resource dependence :                                
//  Author: cheney wu
//  Return : 
/*****************************************************************************/
PUBLIC BT_PBAP_TYPE_E MMIBT_GetPbapRequestType(void)
{
    return s_pbap_connect_type;
}

/*****************************************************************************/
//  Description : MMIBT_GetPbapCancelValue
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBT_GetPbapCancelValue(void)
{
    return s_is_cancel_pbap_pull;
}

/*****************************************************************************/
//  Description : MMIBT_SetPbapCancelValue
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_SetPbapCancelValue(BOOLEAN is_cancel)
{
    s_is_cancel_pbap_pull = is_cancel;
}

/*******************************************************************************/
//  Description :添加一条电话本记录
//  Parameter: msg_body_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			   vcard_size[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
PUBLIC void MMIBT_AddVcardToContact(BT_MSG_T  *msg_body_ptr)
{
	if(PNULL == msg_body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact msg_body_ptr is null");
        return;
    }

	if (PNULL == msg_body_ptr->body_ptr)
	{
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact msg_body_ptr->body_ptr is null");
        return;
	}

	if (BT_SUCCESS != msg_body_ptr->status)
	{
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact msg_body_ptr->status isnot success");
        return;
	}

    SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact oper_type:%d, len:%d, is_first_block:%d", 
                        (BT_PBAP_OP_T)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->oper_type,
                        (uint16)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len, 
                        (BOOLEAN)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->is_first_block);
    
	if (BT_PBAPOP_PULL_PHONEBOOK != (BT_PBAP_OP_T)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->oper_type)
	{
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact oper_type Error");
        return;
	}

	/************************************************************************/
	/* 将待解析的数据构建成一个静态链表 	                                    */
	/************************************************************************/    

    MMIAPIPB_InsertPBAPDataToList((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff,
    								(uint16)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len);

	if((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff != PNULL)
	{
		#ifdef WIN32
		SCI_FREE((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff);
		#else		
		OS_Pfree((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff); /*lint !e718 */
		#endif
		(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff = PNULL;
	}
	//SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact ret:%d, bt_status:d", ret, bt_status);
}

/*******************************************************************************/
//  Description :添加一条电话本记录
//  Parameter: msg_body_ptr[in]: 用于保存UTF-8编码的vCard条目数据，编码符合vCard2.1规范的规定。
//			   vcard_size[in]: 该条vCard记录的大小
//  Global resource dependence :                                
//  Author: yanyan.an
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_AddVcardToCalllog(BT_MSG_T  *msg_body_ptr)
{
    
    if(PNULL == msg_body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToCalllog msg_body_ptr is null");
        return;
    }
    
    if (PNULL == msg_body_ptr->body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToCalllog msg_body_ptr->body_ptr is null");
        return;
    }
    
    if (BT_SUCCESS != msg_body_ptr->status)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToCalllog msg_body_ptr->status isnot success");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToCalllog oper_type:%d, len:%d, is_first_block:%d", 
        (BT_PBAP_OP_T)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->oper_type,
        (uint16)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len, 
        (BOOLEAN)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->is_first_block);
    
    if (BT_PBAPOP_PULL_PHONEBOOK != (BT_PBAP_OP_T)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->oper_type)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToCalllog oper_type Error");
        return;
    }
    
    /************************************************************************/
    /* 将待解析的数据构建成一个静态链表 	                                    */
    /************************************************************************/  
    
    MMICL_InsertPBAPDataToList((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff,
        (uint16)(( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len);
    
    if((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff != PNULL)
    {
#ifdef WIN32
        SCI_FREE((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff);
#else		
        OS_Pfree((( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff); /*lint !e718 */
#endif
        (( BT_PBAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff = PNULL;
    }
    //SCI_TRACE_LOW("[MMIBT] MMIBT_AddVcardToContact ret:%d, bt_status:d", ret, bt_status);
}

/*****************************************************************************/
//  Discription: sync confirm implementation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_DirectConnectPbapService(void)
{
    BT_ADDRESS                  dev_addr = {0};
    BOOLEAN                     is_dev_connect = FALSE;
    BT_STATUS                   register_ret = BT_ERROR;
    BT_STATUS                   connect_ret = BT_ERROR;
    MMI_STRING_T                wait_text = {0};

    is_dev_connect = MMIBT_IsConnectDevice(&dev_addr);
   
    if(!is_dev_connect)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }

    /************************************************************************/
    /*Registers the Phonebook Access Client                                  */
    /************************************************************************/
    register_ret = MMIBT_PbapClientRegister();
    SCI_TRACE_LOW("[MMIBT] MMIAPIBT_DirectConnectPbapService register_ret:%d", register_ret);

    if((BT_SUCCESS != register_ret) && (BT_PENDING != register_ret))
    {
        MMIBT_OpenAlertTipWin(TXT_BT_REGISTER_FAIL); 
        return;
    }
    
    /************************************************************************/
    /* Create a transport connection to the specified device and issue an  
    OBEX Connect Request */
    /************************************************************************/
    if(!MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
    {
        MMI_GetLabelTextByLang(TXT_BT_CONNECT_WAITING, &wait_text); 
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PBAP_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
             ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinDirectConnectWaitingMsg); 
        MMIPUB_SetWinSoftkey(MMIBT_PBAP_CONNECT_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
    }

    MMIBT_CreatePbapSyncTimer();

    connect_ret = MMIBT_PbapConnect(&dev_addr);
    
    SCI_TRACE_LOW("[MMIBT] MSG_BT_CONTACT_SYNC_CNF connect_ret:%d", connect_ret);

    if(BT_PENDING != connect_ret)
    {
    	 if(BT_BUSY == connect_ret)
    	 {
    	 	SCI_TRACE_LOW("pbap connected staus is busy wait.....");
    	 }
	     else
	     {
            MMIBT_OpenAlertTipWin(TXT_BT_PBAP_CONNECT_FAIL); 
	     }
    }
}

/*****************************************************************************/
//  Discription: handle the message of pubwin including timer
//  Global resource dependence: none 
//  Author:  
//  Note:
//
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePubwinDirectConnectWaitingMsg(
                                                       MMI_WIN_ID_T         win_id, 
                                                       MMI_MESSAGE_ID_E    msg_id, 
                                                       DPARAM              param
                                                       )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
        MMIBT_StopPbapSyncTimer();
	    MMK_CloseWin(win_id);
	    break;
    
    case MSG_APP_RED:
        break;
        
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    
    return recode;  
}

#endif

#ifdef BT_MAP_SUPPORT
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
void MMIBT_MapParserCallback(MMISMS_MAP_PARSER_RESULT_T *result)
{
    MMISMS_CloseMapWaitWin();
}

/*****************************************************************************/
//  Discription: sync confirm implementation
//  Global resource dependence: None
//  Author: 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIBT_DirectConnectMapService(void)
{
    BT_ADDRESS            dev_addr = {0};
    BOOLEAN               is_dev_connect = FALSE;
    BOOLEAN               register_ret = FALSE;
    BT_STATUS             connect_ret = BT_ERROR;
    MMI_STRING_T          wait_text = {0};
    uint32                service_type = 0;
    
    is_dev_connect = MMIBT_IsConnectDevice(&dev_addr);
    
    if(!is_dev_connect)
    {
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_BT_DEVICE_DISCONNECT_ALERT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        return;
    }
    
    //only check in_addr is connected headset or not
    //get current earphone status, if is active do not allow to delete device
    service_type = BT_GetPairedDeviceService(&dev_addr);
    if(0 == (BT_SERVICE_MAP_SERVER & service_type))
    {
        MMIBT_OpenAlertTipWin(TXT_BT_MAP_NOT_SUPPORT); 
        return;
    }
    
    /************************************************************************/
    /*Registers the Message Access Client                                  */
    /************************************************************************/
    register_ret = MMISMS_MapRegisterClient();
    SCI_TRACE_LOW("[MMIBT] MMIAPIBT_DirectConnectMapService register_ret:%d", register_ret);
    
    if(FALSE == register_ret)
    {
        MMIBT_OpenAlertTipWin(TXT_BT_REGISTER_FAIL); 
        return;
    }
    
    /************************************************************************/
    /* Create a transport connection to the specified device and issue an  
    OBEX Connect Request */
    /************************************************************************/
    if(!MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
    {
        MMI_GetLabelTextByLang(TXT_BT_CONNECT_WAITING, &wait_text); 
        MMIPUB_OpenWaitWin(1,&wait_text,PNULL,PNULL,MMIBT_PBAP_CONNECT_WAIT_WIN_ID,IMAGE_NULL,
            ANIM_PUBWIN_WAIT,WIN_ONE_LEVEL,MMIPUB_SOFTKEY_CUSTOMER,HandlePubwinDirectConnectWaitingMsg); 
        MMIPUB_SetWinSoftkey(MMIBT_PBAP_CONNECT_WAIT_WIN_ID, TXT_NULL, STXT_CANCEL, TRUE);
    }
    
    MMIAPISMS_CreateMapSyncTimer();
    
#ifndef WIN32
    connect_ret = BT_MapConnect(&dev_addr);
#else
    connect_ret = BT_PENDING;
#endif
    
    SCI_TRACE_LOW("[MMIBT] MSG_BT_CONTACT_SYNC_CNF connect_ret:%d", connect_ret);
    
    if(BT_PENDING != connect_ret)
    {
        MMIBT_OpenAlertTipWin(TXT_BT_PBAP_CONNECT_FAIL);                    
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapTpDisconnect(BT_MSG_T* msg_body_ptr)
{
    BT_MAP_OP_T        oper_type = BT_MAPOP_NONE;
    BT_MAP_RESP_CODE_T resp_code = BT_MRC_STATUS_SUCCESS;

    oper_type = (BT_MAP_OP_T)(((BT_MAP_TP_DISCONNECTED_INFO_T *)msg_body_ptr->body_ptr)->oper_type);
    resp_code = (BT_MAP_RESP_CODE_T)(((BT_MAP_TP_DISCONNECTED_INFO_T *)msg_body_ptr->body_ptr)->resp_code);
    SCI_TRACE_LOW("[MMIBT_MapTpDisconnect] The operation is 0x%x, fail reason is 0x%x.", oper_type, resp_code);

    MMISMS_CloseMapWaitWin();
    MMIAPISMS_SetSendAddrCount(0);
    MMIAPISMS_SetIsSendMultiSms(FALSE);
    MMIAPISMS_ClearTableList();

    if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
    }

    SCI_TRACE_LOW("[MMIBT_MapTpDisconnect] MMIAPISMS_GetCurOperIsConnect() = %d", MMIAPISMS_GetCurOperIsConnect());
    if(MMIAPISMS_GetCurOperIsConnect() && (BT_MRC_LINK_DISCONNECT == resp_code))
    {
        MMIAPISMS_SetCurOperIsConnect(FALSE);
        MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_NOT_SUPPORT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapComplete(BT_MSG_T* msg_body_ptr)
{
    
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] PNULL == msg_body_ptr!");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT_MapComplete] The operation is 0x%x.", *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)));
    
    if(BT_MAPOP_CONNECT == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_CONNECT");

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT) && defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
        MMIAPISMS_SetSMSSyncType(MMISMS_SYNC_TYPE_MAP);
#endif        
        MMIAPISMS_SetCurOperIsConnect(FALSE);
        MMIAPICOM_SendSignalToMMITask(MMISMS_BT_REGISTER_NOTIFY);
        MMIAPISMS_StopMapSyncTimer();
        if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
        {
            MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
        }
        if(MMK_IsOpenWin(MMIBT_DEVICE_LIST_WIN_ID))
        {
            MMK_SendMsg(MMIBT_DEVICE_LIST_WIN_ID, MSG_BT_DEVICE_LIST_UPDATE, NULL);
        }

        /* Connect to other service */
        //ConnectHandsetGatewayService();
    }
    else if(BT_MAPOP_SET_REGISTRATION == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_SET_REGISTRATION");
        if(MMIAPISMS_GetCurOperIsPush())
        {
            SCI_TRACE_LOW("MMIAPISMS_GetCurOperIsPush() MSG_BT_MAP_CONNECT_SUCCESS");
            MMK_SendMsg(MMISMS_EDIT_SMS_WIN_ID, MSG_BT_MAP_CONNECT_SUCCESS, PNULL);
        }
        else
        {
            switch( MMIAPISMS_GetCurBoxType() )
            {
            case MMISMS_BT_BOX_MT:
                MMK_SendMsg(MMISMS_BT_MTBOX_CHILD_WIN_ID, MSG_BT_MAP_CONNECT_SUCCESS, PNULL);
                break;
                
            case MMISMS_BT_BOX_SENDSUCC:
                MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MSG_BT_MAP_CONNECT_SUCCESS, PNULL);
                break;
                
            case MMISMS_BT_BOX_SENDFAIL:
                MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_BT_MAP_CONNECT_SUCCESS, PNULL);
                break;
                
            case MMISMS_BT_BOX_NOSEND:
                MMK_SendMsg(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID, MSG_BT_MAP_CONNECT_SUCCESS, PNULL);
                break;
                
            default:
                break;
            }
            
            MMIAPISMS_StopMapSyncTimer();
        }
    }
    else if(BT_MAPOP_PULL_FOLDER_LISTING == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_PULL_FOLDER_LISTING");
        MMIAPISMS_NotifySMSStartParser(MMIBT_MapParserCallback);
    }
    else if(BT_MAPOP_SET_FOLDER == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_SET_FOLDER");
        
        if(MMIAPISMS_GetCurOperIsPush())
        {
            SCI_TRACE_LOW("MMIAPISMS_GetCurOperIsPush() == MMISMS_WRITE_NEW_SMS");
            if(MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID))
            {
                SCI_TRACE_LOW("MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID)");
                MMK_SendMsg(MMISMS_EDIT_SMS_WIN_ID, MSG_BT_SET_FOLDER_SUCCESS, PNULL);
            }
            else
            {
                SCI_TRACE_LOW("MMK_IsOpenWin(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID)");
                MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_BT_SET_FOLDER_SUCCESS, PNULL);
            }
            
        }
        else
        {
            switch( MMIAPISMS_GetCurBoxType() )
            {
            case MMISMS_BT_BOX_MT:
                MMK_SendMsg(MMISMS_BT_MTBOX_CHILD_WIN_ID, MSG_BT_SET_FOLDER_SUCCESS, PNULL);
                break;
                
            case MMISMS_BT_BOX_SENDSUCC:
                MMK_SendMsg(MMISMS_BT_SENDSUCCBOX_CHILD_WIN_ID, MSG_BT_SET_FOLDER_SUCCESS, PNULL);
                break;
                
            case MMISMS_BT_BOX_SENDFAIL:
                MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_BT_SET_FOLDER_SUCCESS, PNULL);
                break;
                
            case MMISMS_BT_BOX_NOSEND:
                MMK_SendMsg(MMISMS_BT_NOSENDBOX_CHILD_WIN_ID, MSG_BT_SET_FOLDER_SUCCESS, PNULL);
                break;
                
            default:
                break;
            }
        }
    }    
    else if(BT_MAPOP_PULL_MESSAGE_LISTING == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_PULL_MESSAGE_LISTING");
        MMIAPISMS_NotifyGetMsgListStartParser(MMIBT_MapParserCallback);
    }    
    else if(BT_MAPOP_PULL_MESSAGE == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_PULL_MESSAGE");
#ifdef WIN32
        MMIAPISMS_SimuMAPDataToList(1);
#endif // #ifdef WIN32
        MMIAPISMS_NotifyGetMessageStartParser(MMIBT_MapParserCallback);
    }
    else if(BT_MAPOP_DISCONNECT == *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_DISCONNECT");
        MMIBT_MapClientDeregister();
    } 
    else if(BT_MAPOP_PUSH_MESSAGE == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_PUSH_MESSAGE");
        if(MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID))
        {
            SCI_TRACE_LOW("MMK_IsOpenWin(MMISMS_EDIT_SMS_WIN_ID)");
            MMK_SendMsg(MMISMS_EDIT_SMS_WIN_ID, MSG_BT_PUSH_SUCCESS, PNULL);
        }
        else
        {
            SCI_TRACE_LOW("MMK_IsOpenWin(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID)");
            MMK_SendMsg(MMISMS_BT_SENDFAILBOX_CHILD_WIN_ID, MSG_BT_PUSH_SUCCESS, PNULL);
        }
    }
    else if(BT_MAPOP_PUSH == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_PUSH");
    }
    else if(BT_MAPOP_SET_MESSAGE_STATUS == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_SET_MESSAGE_STATUS");

        MMIAPISMS_StopMapSyncTimer();
        if(MMISMS_DELETE_MSG == MMIAPISMS_GetCurOperStatus())
        {
            SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_SET_MESSAGE_STATUS DELETE");
            MMIAPISMS_HandleMsgDeleteStatus();
        }
        else
        {
            SCI_TRACE_LOW("[MMIBT_MapComplete] BT_MAPOP_SET_MESSAGE_STATUS READ");
            MMIAPISMS_HandleMsgReadStatus();
        }
    }
    else
    {
        SCI_TRACE_LOW("[MMIBT_MapComplete] message id is %d", *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)));
        MMIAPISMS_NotifySMSStartParser(MMIBT_MapParserCallback);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapAbort(BT_MSG_T* msg_body_ptr)
{
    BT_MAP_OP_T        oper_type = BT_MAPOP_NONE;
    BT_MAP_RESP_CODE_T resp_code = BT_MRC_STATUS_SUCCESS;
    
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_MapAbort] PNULL == msg_body_ptr!");
        return;
    }
    
    oper_type = (BT_MAP_OP_T)(((BT_MAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->oper_type);
    resp_code = (BT_MAP_RESP_CODE_T)(((BT_MAP_ABORT_INFO_T *)msg_body_ptr->body_ptr)->resp_code);
    
    SCI_TRACE_LOW("[MMIBT_MapAbort] The operation is 0x%x, fail reason is 0x%x.", oper_type, resp_code);
    
    if(BT_MAPOP_CONNECT == oper_type)
    {
        SCI_TRACE_LOW("[MMIBT_MapAbort] BT_MAPOP_CONNECT");
        MMIAPISMS_SetCurOperIsConnect(FALSE);
    }
    else if(BT_MAPOP_PULL_MESSAGE_LISTING == oper_type)
    {
        SCI_TRACE_LOW("[MMIBT_MapAbort] BT_MAPOP_PULL_MESSAGE_LISTING");
        if(BT_MRC_SERVICE_UNAVAILABLE == resp_code)
        {
            MMISMS_CloseMapWaitWin();

            if(MMK_IsOpenWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID))
            {
                MMK_CloseWin(MMIBT_PBAP_CONNECT_WAIT_WIN_ID);
            }

            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_MAP_NOT_SUPPORT,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
    }
    else if(BT_MAPOP_PUSH_MESSAGE == oper_type)
    {
        SCI_TRACE_LOW("[MMIBT_MapAbort] BT_MAPOP_PUSH_MESSAGE");
        MMIAPISMS_SetSendAddrCount(0);
        MMIAPISMS_SetIsSendMultiSms(FALSE);
        MMISMS_CloseMapWaitWin();

#if defined(BT_DIALER_SUPPORT) && defined(BT_MAP_SUPPORT) && defined(MMI_SMS_SYN_SUPPORT) && defined(BT_SPP_SUPPORT)
        MMIAPISMS_StopSmsSyncTimer();
        MMIAPISMS_SetSMSSyncType(MMISMS_SYNC_TYPE_NONE);
#endif
    }
    else if(BT_MAPOP_SET_MESSAGE_STATUS == oper_type)
    {
        SCI_TRACE_LOW("[MMIBT_MapAbort] BT_MAPOP_SET_MESSAGE_STATUS");
        MMIAPISMS_StopMapSyncTimer();
        MMIAPISMS_HandleSetMsgStatusAbort(resp_code);
        
    }

    if(MMK_IsOpenWin(MMIBT_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_WAIT_WIN_ID);
        MMIPUB_OpenAlertSuccessWin(STXT_CANCEL);
    }
    //需要确认complete情况
    
    if(MMK_IsOpenWin(MMIBT_SYNC_WAIT_WIN_ID))
    {
        MMK_CloseWin(MMIBT_SYNC_WAIT_WIN_ID);
    }
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapParamsRx(BT_MSG_T* msg_body_ptr)
{
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_MapParamsRx] PNULL == msg_body_ptr!");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT_MapParamsRx] The Application Parameters header information has been fully parsed!");
}


/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapDataInd(BT_MSG_T* msg_body_ptr)
{

    SCI_TRACE_LOW("[MMIBT_MapDataInd] The operation is 0x%x.", *((BT_PBAP_OP_T*)(msg_body_ptr->body_ptr)));
    
    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_MapDataInd] PNULL == msg_body_ptr!");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT_MapDataInd] Data has been received!");
    SCI_TRACE_LOW("[MMIBT_MapDataInd] s_is_cancel_pbap_pull = %d", s_is_cancel_pbap_pull);
    if(!s_is_cancel_pbap_pull)
    {
        
        if(BT_MAPOP_PULL_FOLDER_LISTING == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("MMIBT_MapDataInd, BT_MAPOP_PULL_FOLDER_LISTING");	    
        }
        else if(BT_MAPOP_PULL_MESSAGE_LISTING == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("MMIBT_MapDataInd, BT_MAPOP_PULL_MESSAGE_LISTING");
        }    
        else if(BT_MAPOP_PULL_MESSAGE == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
        {
            SCI_TRACE_LOW("MMIBT_MapDataInd, BT_MAPOP_PULL_MESSAGE");
        }    
        
	    MMIBT_SMSDataReceive(msg_body_ptr);
	}
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBT_MapServerDataInd(BT_MSG_T* msg_body_ptr)
{
    SCI_TRACE_LOW("[MMIBT_MapServerDataInd] Data has been received!");

    if((PNULL == msg_body_ptr) || (PNULL == msg_body_ptr->body_ptr))
    {
        SCI_TRACE_LOW("[MMIBT_MapServerDataInd] PNULL == msg_body_ptr!");
        return;
    }

    MMIBT_SMSServerDataReceive(msg_body_ptr);
}

/*****************************************************************************/
//  Description : Deregisters the Phonebook Access Client
//  Global resource dependence :                                
//  Author: Candice
//  Note:
/*****************************************************************************/
PUBLIC void MMIBT_MapClientDeregister(void)
{
#ifndef WIN32
    BT_STATUS ret_val = BT_ERROR;
    ret_val = BT_MapDeregisterClient();
    
    SCI_TRACE_LOW("[MMIBT_MapClientDeregister] ret_val = 0x%x", ret_val);
    if(BT_SUCCESS != ret_val)
    {
        MMIPUB_OpenAlertFailWin(TXT_ERROR);
    }
    
#else
    return;
#endif
}

/*******************************************************************************/
//  Description :
//  Parameter: 
//  Global resource dependence :                                
//  Author:
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_SMSDataReceive(BT_MSG_T  *msg_body_ptr)
{
    
    if(PNULL == msg_body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSDataReceive msg_body_ptr is null");
        return;
    }
    
    if (PNULL == msg_body_ptr->body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSDataReceive msg_body_ptr->body_ptr is null");
        return;
    }
    
    if (BT_SUCCESS != msg_body_ptr->status)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSDataReceive msg_body_ptr->status isnot success");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_SMSDataReceive oper_type:%d, len:%d", 
        (BT_MAP_OP_T)(( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->oper_type,
        (uint16)(( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len);
    
    /************************************************************************/
    /* 将待解析的数据构建成一个静态链表 	                                    */
    /************************************************************************/    
    
    
    if(BT_MAPOP_PULL_FOLDER_LISTING == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        SCI_TRACE_LOW("MMIBT_SMSDataReceive, BT_MAPOP_PULL_FOLDER_LISTING");	    
    }
    else if(BT_MAPOP_PULL_MESSAGE_LISTING == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        MMIAPISMS_InsertMAPDataToList((( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff,
    								(uint16)(( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len);
    }    
    else if(BT_MAPOP_PULL_MESSAGE == *((BT_MAP_OP_T*)(msg_body_ptr->body_ptr)))
    {
        MMIAPISMS_InsertMessageDataToList((( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff,
            (uint16)(( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->len);        
    }    
    
    if((( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff != PNULL)
    {
#ifdef WIN32
        SCI_FREE((( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff);
#else		
        OS_Pfree((( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff); /*lint !e718 */
#endif
        (( BT_MAP_DATA_IND_T * )msg_body_ptr->body_ptr)->buff = PNULL;
    }
}

/*******************************************************************************/
//  Description : MMIBT_SMSServerDataReceive
//  Parameter: 
//  Global resource dependence :                                
//  Author: Naomi Huang
//  Return : 
/*****************************************************************************/
LOCAL void MMIBT_SMSServerDataReceive(BT_MSG_T  *msg_body_ptr)
{
    SCI_TRACE_LOW("[MMIBT] SMSServerDataReceive");
    
    if(PNULL == msg_body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSServerDataReceive msg_body_ptr is null");
        return;
    }
    
    if (PNULL == msg_body_ptr->body_ptr)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSServerDataReceive msg_body_ptr->body_ptr is null");
        return;
    }
    
    if (BT_SUCCESS != msg_body_ptr->status)
    {
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSServerDataReceive msg_body_ptr->status isnot success");
        return;
    }
    
    SCI_TRACE_LOW("[MMIBT] MMIBT_SMSDataReceive len:%d", 
        (uint16)(( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body_ptr->body_ptr)->len);
    
    MMIAPISMS_HandleServerMsgData((( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body_ptr->body_ptr)->buff,
        (uint16)(( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body_ptr->body_ptr)->len);
    
    if((( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body_ptr->body_ptr)->buff != PNULL)
    {
#ifdef WIN32
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSServerDataReceive SCI_FREE (msg_body_ptr->body_ptr)->buff");
        SCI_FREE((( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body_ptr->body_ptr)->buff);
        
#else		
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSServerDataReceive OS_Pfree (msg_body_ptr->body_ptr)->buff");
        OS_Pfree((( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body_ptr->body_ptr)->buff); /*lint !e718 */
#endif
        
        SCI_TRACE_LOW("[MMIBT] MMIBT_SMSServerDataReceive (msg_body_ptr->body_ptr)->buff = null");
        (( BT_MAP_MNS_SERVER_DATA_IND_T * )msg_body_ptr->body_ptr)->buff = PNULL;
    }
}

#endif

#ifdef BT_AV_SNK_SUPPORT
PUBLIC BT_STATUS MMIBT_A2dpSinkConnect(BT_ADDRESS addr)
{
#ifndef WIN32
    return A2DP_Sink_Connect(addr); 
#endif
}

PUBLIC BT_STATUS MMIBT_A2dpSinkDisconnect(void)
{
#ifndef WIN32
    return A2DP_Sink_Disconnect();
#endif
}

PUBLIC BOOLEAN MMIBT_IsA2dpSinkConnected(void)
{
    return s_a2dp_sink_connected;
}

PUBLIC void MMIBT_HandleA2dpSinkConnected(BT_MSG_T* msg_body_ptr)
{
	MMI_STRING_T wait_text = {0};
	MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    BT_ADDRESS     connected_address = {0};
    connected_address = *(BT_ADDRESS *)msg_body_ptr->body_ptr;

    SCI_TRACE_LOW("[MMIBT] MMIBT_HandleA2dpSinkConnected");
    SCI_TRACE_LOW("BT_AV, connected address: %02X-%02X-%02X-%02X-%02X-%02X", connected_address.addr[5], connected_address.addr[4], connected_address.addr[3], connected_address.addr[2], connected_address.addr[1], connected_address.addr[0]);
    
    s_a2dp_remote_play_status = 0x00;
    s_a2dp_local_play_status = 0x00;
    s_a2dp_sink_connected = TRUE;
    MMIAPIMp3_UpdateA2dpAvrcpConnectionStatus(s_a2dp_sink_connected,s_avrcp_ct_connected);

#ifndef WIN32
    AVRCP_CT_Connect(connected_address);
    A2DP_Sink_GetAudioConfig(&s_a2dp_sink_sample_rate,&s_a2dp_sink_channel_num);
#endif
    SCI_TRACE_LOW("BT_AV, MMIBT_HandleA2dpSinkConnected s_a2dp_sink_sample_rate:%d, s_a2dp_sink_channel_num:%d",s_a2dp_sink_sample_rate,s_a2dp_sink_channel_num);

	MMI_GetLabelTextByLang(TXT_BT_HEADSET_OPENED, &wait_text);
	MMIPUB_OpenAlertWinByTextPtr(PNULL, &wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

	/* Connect to other service */
        ConnectHandsetGatewayService();
}

PUBLIC void MMIBT_HandleA2dpSinkDisconnected(BT_MSG_T* msg_body_ptr)
{
	MMI_STRING_T wait_text = {0};
	MMI_WIN_ID_T alert_id = MMI_BT_ALERT_WIN_ID;
    uint32      service_type = 0;
    MMIBT_OPERATION_INFO_T  bt_opc = MMIBT_GetOpertor();

    SCI_TRACE_LOW("[MMIBT] MMIBT_HandleA2dpSinkDisconnected");

    s_a2dp_sink_connected = FALSE;
    s_a2dp_remote_play_status = 0x00;
    s_a2dp_local_play_status = 0x00;
    MMIBT_ReleaseA2dpSinkAudio();

//    service_type = BT_GetPairedDeviceService(&g_current_bt.addr);
//    if (BT_SERVICE_A2DP_SRC & service_type)

    if(MMIBT_OPC_CLOSE_HANDSET_GATEWAY != bt_opc.current_opertion)
    {
        SCI_TRACE_LOW("[Cheney] MMIBT_HandleA2dpSinkDisconnected: 3");
        MMI_GetLabelTextByLang(TXT_BT_HEADSET_CLOSED, &wait_text);
        MMIPUB_OpenAlertWinByTextPtr(PNULL, &wait_text,PNULL,IMAGE_PUBWIN_SUCCESS,&alert_id,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
    }

    MMIAPIMp3_UpdateA2dpAvrcpConnectionStatus(s_a2dp_sink_connected,s_avrcp_ct_connected);
}

PUBLIC void MMIBT_HandleA2dpStreamStarted(BT_MSG_T* msg_body_ptr)
{
    /*media 0x00 : STOPPED
     *		0x01 : PLAYING
     *		0x02 : PAUSED
     *		0x03 : FWD_SEEK
     *		0x04 : REV_SEEK
     *		0xFF : ERROR
     */
    SCI_TRACE_LOW("BT_AV, MMIBT_HandleA2dpStreamStarted s_a2dp_remote_play_status:%u, s_a2dp_local_play_status:%u ",s_a2dp_remote_play_status,s_a2dp_local_play_status);
   
    if(0x00 == s_a2dp_remote_play_status)  //STOPPED
    {
        if(FALSE == MMIBT_RequestA2dpSinkAudio())
        {
            s_a2dp_local_play_status = 0x00;
        }
        else
        {
            s_a2dp_local_play_status = 0x01;  //PLAYING
            MMIBT_SetA2dpSinkVolume(MMIAPISET_GetMultimVolume());
        }
    }
    else if(0x02 == s_a2dp_remote_play_status && 0x01 != s_a2dp_local_play_status) //PAUSED
    {
        if(0 != s_a2dp_sink_handle)
        {
            BOOLEAN retVal = FALSE;
            SCI_TRACE_LOW("BT_AV, MMIBT_HandleA2dpStreamStarted MMISRVAUD_Resume");
            retVal = MMISRVAUD_Resume(s_a2dp_sink_handle);
            SCI_TRACE_LOW("BT_AV, MMIBT_HandleA2dpStreamStarted MMISRVAUD_Resume:%u",retVal);

            //for a2dp sink resume after call ended
            if(FALSE == retVal)
            {
                if (0 != s_a2dp_retry_timer_id)
                {
                    SCI_TRACE_LOW("BT_AV, stop a2dp sink resume timer");
                    MMK_StopTimer(s_a2dp_retry_timer_id);
                    s_a2dp_retry_timer_id = 0;
                }
                s_a2dp_retry_count = 1;
                s_a2dp_retry_timer_id = MMK_CreateTimerCallback(1000, A2dpSinkResumeRetry, NULL, FALSE);
            }
            else
            {
                s_a2dp_local_play_status = 0x01;  //PLAYING
            }
        }
        else
        {
            //a2dp sink audio priority has adjusted to MMISRVAUD_PRI_NORMAL_N10, it will not reuqest audio correctly if local audio is using like video player
            //so we request a2dp sink audio here
            if(FALSE == MMIBT_RequestA2dpSinkAudio())
            {
                s_a2dp_local_play_status = 0x00;
            }
            else
            {
                s_a2dp_local_play_status = 0x01;  //PLAYING
                MMIBT_SetA2dpSinkVolume(MMIAPISET_GetMultimVolume());
            }
        }
    }

    s_a2dp_remote_play_status = 0x01;  //PLAYING
    MMIAPIMp3_UpdatePlayback(s_a2dp_remote_play_status);
}

PUBLIC void MMIBT_HandleA2dpStreamSuspended(BT_MSG_T* msg_body_ptr)
{
    SCI_TRACE_LOW("BT_AV, MMIBT_HandleA2dpStreamSuspended s_a2dp_remote_play_status:%u",s_a2dp_remote_play_status);
    if(0 != s_a2dp_sink_handle)
    {
        BOOLEAN retVal = FALSE;
        SCI_TRACE_LOW("BT_AV, MMIBT_HandleA2dpStreamSuspended MMISRVAUD_Pause");
        retVal = MMISRVAUD_Pause(s_a2dp_sink_handle);
        SCI_TRACE_LOW("BT_AV, MMIBT_HandleA2dpStreamSuspended MMISRVAUD_Pause:%u",retVal);
        s_a2dp_local_play_status = 0x02;  //PAUSED
    }
    s_a2dp_remote_play_status = 0x02;  //PAUSED
    MMIAPIMp3_UpdatePlayback(s_a2dp_remote_play_status);
}

PUBLIC BT_STATUS MMIBT_AvrcpCtConnect(BT_ADDRESS addr)
{
#ifndef WIN32
    return AVRCP_CT_Connect(addr);
#endif
}

PUBLIC BT_STATUS MMIBT_AvrcpCtDisconnect(void)
{
#ifndef WIN32
    return AVRCP_CT_Disconnect();
#endif
}

PUBLIC BOOLEAN MMIBT_IsAvrcpCtConnected(void)
{
    return s_avrcp_ct_connected;
}

PUBLIC BT_STATUS MMIBT_SetAvrcpPanelKey(uint16 op, BOOLEAN press, BOOLEAN auto_release)
{
    BT_STATUS retVal = BT_ERROR;
    AvrcpCtPanelOperation  operation;
    
    operation.Op = op;
    operation.press = press;

    if(FALSE == s_avrcp_ct_connected)
    {
        SCI_TRACE_LOW("BT_AV, no avrcp connection!");
        return retVal;
    }

#ifndef WIN32
    SCI_TRACE_LOW("BT_AV, AVRCP_CT_SetPanelKey: %X, press:%d, auto_release:%d",op,press,auto_release);
    retVal = AVRCP_CT_SetPanelKey(operation,auto_release);
    SCI_TRACE_LOW("BT_AV, AVRCP_CT_SetPanelKey retVal: %d",retVal);
    
    //if(auto_release)
    //{
        //MMK_CreateTimerCallback(0, AvrcpPanelKeyRelease, (uint16)op, FALSE);
        //operation.press = FALSE;
        //SCI_TRACE_LOW("BT_AV, AVRCP_CT_SetPanelKey: %X, %d",op,operation.press);
        //retVal = AVRCP_CT_SetPanelKey(operation);
        //SCI_TRACE_LOW("BT_AV, AVRCP_CT_SetPanelKey retVal: %d",retVal);
    //}
#endif
    
    return retVal;
}

PUBLIC BT_STATUS MMIBT_GetMdaPlayStatus(void)
{
    BT_STATUS retVal = BT_ERROR;
#ifndef WIN32
    retVal = AVRCP_CT_GetMdaPlayStatus();
    SCI_TRACE_LOW("BT_AV, AVRCP_CT_GetMdaPlayStatus retVal:%d",retVal);
    return retVal;
#endif
}

PUBLIC BOOLEAN MMIBT_RequestA2dpSinkAudio(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
	MMISRVAUD_TYPE_T audio_srv = {0};

	SCI_TRACE_LOW("BT_AV, MMIBT_RequestA2dpSinkAudio: %d, %d",s_a2dp_sink_handle,s_a2dp_sink_connected);

	if((0 == s_a2dp_sink_handle) && (TRUE == s_a2dp_sink_connected) && (FALSE == s_a2dp_sink_request_lock))
	{
		SCI_TRACE_LOW("BT_AV, gain a2dp sink audio");

        s_a2dp_sink_request_lock = TRUE;

		req.ind_data = 0;
		req.is_async = FALSE;
		req.is_auto_resume_off = FALSE;
		req.notify = A2dpSinkPlayCallBack;
		req.pri = MMISRVAUD_PRI_NORMAL_N10;

		audio_srv.default_route = MMISRVAUD_ROUTE_AUTO;
		audio_srv.info.type = MMISRVAUD_TYPE_BT_MUSIC;
		audio_srv.volume_type = MMISRVAUD_VOLUME_TYPE_MED;
		audio_srv.all_support_route = MMISRVAUD_ROUTE_AUTO;
		audio_srv.play_times = 1;
		audio_srv.volume = MMIAPISET_GetMultimVolume();

		s_a2dp_sink_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

		if(0 == s_a2dp_sink_handle)
		{
			SCI_TRACE_LOW("BT_AV, MMIBT_RequestA2dpSinkAudio create handle failed");
            s_a2dp_sink_request_lock = FALSE;
			return FALSE;
		}

        SCI_TRACE_LOW("BT_AV, MMIBT_RequestA2dpSinkAudio MMISRVAUD_Play");
        MMISRVAUD_Play(s_a2dp_sink_handle, 0);

        s_a2dp_sink_request_lock = FALSE;
        return TRUE;
	}
    return TRUE;
}

PUBLIC void MMIBT_ReleaseA2dpSinkAudio(void)
{
    SCI_TRACE_LOW("BT_AV, release a2dp sink audio: %d",s_a2dp_sink_handle);
    if(0 != s_a2dp_sink_handle)
    {
	    SCI_TRACE_LOW("BT_AV, a2dp sink stop and free");
	    MMISRVAUD_Stop(s_a2dp_sink_handle);
        MMISRVMGR_Free(s_a2dp_sink_handle);
	    s_a2dp_sink_handle = 0;
    }
}

PUBLIC void MMIBT_SetA2dpSinkVolume(uint32 volume)
{
	SCI_TRACE_LOW("BT_AV, MMIBT_SetA2dpSinkVolume %d:",volume);
	if(s_a2dp_sink_handle != PNULL)
	{
		SCI_TRACE_LOW("BT_AV, MMISRVAUD_SetVolume %d:",volume);
		MMISRVAUD_SetVolume(s_a2dp_sink_handle,volume);
	}
}

PUBLIC uint32 MMIBT_GetA2dpSinkSampleRate(void)
{
	return s_a2dp_sink_sample_rate;
}

PUBLIC uint8 MMIBT_GetA2dpSinkChannelNum(void)
{	
	return s_a2dp_sink_channel_num;
}

PUBLIC void MMIBT_RouteA2dpSinkAudio(void)
{
    BOOLEAN retVal = FALSE;

    SCI_TRACE_LOW("BT_AV, MMIBT_RouteA2dpSinkAudio s_a2dp_sink_handle:%d, remote play status:%d, local play status:%d",s_a2dp_sink_handle,s_a2dp_remote_play_status,s_a2dp_local_play_status);
    if(0 != s_a2dp_sink_handle && 0x01 == s_a2dp_remote_play_status && 0x02 == s_a2dp_local_play_status)
    {
        SCI_TRACE_LOW("BT_AV, MMIBT_RouteA2dpSinkAudio MMISRVAUD_Resume");
        retVal = MMISRVAUD_Resume(s_a2dp_sink_handle);
        if(TRUE == retVal)
        {
            s_a2dp_local_play_status = 0x01;    //PLAYING
        }
        SCI_TRACE_LOW("BT_AV, MMIBT_RouteA2dpSinkAudio MMISRVAUD_Resume:%u",retVal);
    }
}

LOCAL void AvrcpPanelKeyRelease(uint8 timerid,uint16 op)
{
    SCI_TRACE_LOW("BT_AV, AvrcpPanelKeyRelease op:%X",op);
    MMIBT_SetAvrcpPanelKey(op,FALSE,FALSE);
}

LOCAL void A2dpSinkResumeRetry(void)
{
    BOOLEAN retVal = FALSE;

    SCI_TRACE_LOW("BT_AV, A2dpSinkResumeRetry:%d",s_a2dp_retry_count);

    if(s_a2dp_retry_count > 5)
    {
        s_a2dp_retry_count = 0;
        return;
    }

    if(0 != s_a2dp_sink_handle && 0x02 == s_a2dp_local_play_status)
    {
        SCI_TRACE_LOW("BT_AV, retry MMISRVAUD_Resume");
        retVal = MMISRVAUD_Resume(s_a2dp_sink_handle);
        SCI_TRACE_LOW("BT_AV, retry MMISRVAUD_Resume:%d",retVal);
        if(FALSE == retVal)
        {
            s_a2dp_retry_count++;
            s_a2dp_retry_timer_id = MMK_CreateTimerCallback(1000, A2dpSinkResumeRetry, NULL, FALSE);
        }
        else
        {
            s_a2dp_local_play_status = 0x01;  //PLAYING
        }
    }
}

LOCAL BOOLEAN A2dpSinkPlayCallBack(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    BOOLEAN retVal = FALSE;

    SCI_TRACE_LOW("BT_AV, A2dpSinkPlayCallBack handle:%d, s_a2dp_sink_handle:%d",handle,s_a2dp_sink_handle);

    if(handle == s_a2dp_sink_handle && PNULL != param)
    {
        SCI_TRACE_LOW("BT_AV, A2dpSinkPlayCallBack event:%d",param->event);
        switch(param->event)
        {
        case MMISRVMGR_NOTIFY_SUSPEND:
            {
                SCI_TRACE_LOW("BT_AV, MMISRVMGR_NOTIFY_SUSPEND s_a2dp_remote_play_status:%d, s_a2dp_local_play_status:%d",s_a2dp_remote_play_status,s_a2dp_local_play_status);
                if(0x01 == s_a2dp_local_play_status)   //PLAYING
                {
                    SCI_TRACE_LOW("BT_AV, A2dpSinkPlayCallBack MMISRVAUD_Pause");
                    retVal = MMISRVAUD_Pause(s_a2dp_sink_handle);
                    SCI_TRACE_LOW("BT_AV, A2dpSinkPlayCallBack MMISRVAUD_Pause:%u",retVal);
                    s_a2dp_local_play_status = 0x02;  //PAUSED
                    s_a2dp_sink_resume = TRUE;
                    if (!MMICC_GetBtCallFlag())
                    {
                        is_need_resume = TRUE;
                        SCI_TRACE_LOW("A2dpSinkPlayCallBack not bt call, need pause music");
                        //bug 924734
                        //MMIBT_SetAvrcpPanelKey(0x0046,TRUE,TRUE);
                        //bug 924734
                    }
                }
            }
            break;
        case MMISRVMGR_NOTIFY_RESUME:
            {
                SCI_TRACE_LOW("BT_AV, MMISRVMGR_NOTIFY_RESUME s_a2dp_sink_remote_play_status:%d, s_a2dp_local_play_status:%d",s_a2dp_remote_play_status,s_a2dp_local_play_status);
                if(TRUE == s_a2dp_sink_resume) //PAUSED
                {
                    if(0x02 == s_a2dp_local_play_status)
                    {
                        if (0 != s_a2dp_retry_timer_id)
                        {
                            SCI_TRACE_LOW("BT_AV, stop a2dp sink resume timer");
                            MMK_StopTimer(s_a2dp_retry_timer_id);
                            s_a2dp_retry_timer_id = 0;
                        }
                        SCI_TRACE_LOW("BT_AV, A2dpSinkPlayCallBack MMISRVAUD_Resume");
                        retVal = MMISRVAUD_Resume(s_a2dp_sink_handle);
                        SCI_TRACE_LOW("BT_AV, A2dpSinkPlayCallBack MMISRVAUD_Resume:%u",retVal);
                        if(TRUE == retVal)
                        {
                            s_a2dp_local_play_status = 0x01;    //PLAYING
                        }
                    }
                    s_a2dp_sink_resume = FALSE;
                    if (is_need_resume)
                    {
                        is_need_resume = FALSE;//reset FLAG
                        SCI_TRACE_LOW("A2dpSinkPlayCallBack not bt call, need resume music");
                        //bug 924734
                        //MMIBT_SetAvrcpPanelKey(0x0044,TRUE,TRUE);
                        //bug 924734
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    return TRUE;
}

PUBLIC void MMIBT_HandleAvrcpCtConnected(BT_MSG_T* msg_body_ptr)
{
	BT_STATUS retVal = BT_ERROR;

	SCI_TRACE_LOW("BT_AV, handleAvrcpCtConnected");
	s_avrcp_ct_connected = TRUE;

	#ifndef WIN32
    //first -> list supported player settings
    retVal = AVRCP_CT_ListMdaPlayerSettingAttrs();
    SCI_TRACE_LOW("BT_AV, AVRCP_CT_ListMdaPlayerSettingAttrs retVal:%d",retVal);
	#endif
	//MMIAPIMp3_UpdateA2dpAvrcpConnectionStatus(s_a2dp_sink_connected,s_avrcp_ct_connected);
}

PUBLIC void MMIBT_HandleAvrcpCtDisconnected(BT_MSG_T* msg_body_ptr)
{
    s_avrcp_ct_connected = FALSE;
    //MMIAPIMp3_UpdateA2dpAvrcpConnectionStatus(s_a2dp_sink_connected,s_avrcp_ct_connected);
}

PUBLIC void MMIBT_HandleAvrcpPlayStatus(BT_MSG_T* msg_body_ptr)
{
	BT_STATUS retVal = BT_ERROR;
    AvrcpCtPlayStatus play_status;

    SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpPlayStatus status:%d",msg_body_ptr->status);
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        play_status = *(AvrcpCtPlayStatus *)msg_body_ptr->body_ptr;
        //avrcp will not guarantee media status, we use a2dp stream status instead
        play_status.mediaStatus = s_a2dp_remote_play_status;
        MMIAPIMp3_UpdatePlayerStatus(play_status);
    }
}

PUBLIC void MMIBT_HandleAvrcpMediaAttributes(BT_MSG_T* msg_body_ptr)
{
	BT_STATUS retVal = BT_ERROR;

    SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpMediaAttributes status:%d",msg_body_ptr->status);
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        #ifndef WIN32
        //third -> get play status
	    retVal = AVRCP_CT_GetMdaPlayStatus();
	    SCI_TRACE_LOW("BT_AV, AVRCP_CT_GetMdaPlayStatus retVal:%d",retVal);
        #endif
        MMIAPIMp3_UpdatePlayerAttributes(*(AvrcpCtElementAttributes *)msg_body_ptr->body_ptr);
    }
}

PUBLIC void MMIBT_HandleAvrcpTrackChanged(BT_MSG_T* msg_body_ptr)
{
	BT_STATUS retVal = BT_ERROR;

    SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpTrackChanged status:%d",msg_body_ptr->status);
    if(BT_SUCCESS == msg_body_ptr->status)
    {
#ifndef WIN32
        SCI_SLEEP(5); 
        retVal = AVRCP_CT_GetMdaMediaInfo(0x7F);
        SCI_TRACE_LOW("BT_AV, AVRCP_CT_GetMdaMediaInfo retVal:%d",retVal);
#endif
        //MMIAPIMp3_AvrcpTrackChanged();
    }
}

PUBLIC void MMIBT_HandleAvrcpPlaybackChanged(BT_MSG_T* msg_body_ptr)
{
    uint8 playback;
    BT_STATUS retVal = BT_ERROR;

    SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpPlaybackChanged status:%d",msg_body_ptr->status);
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        playback = *(uint8*)msg_body_ptr->body_ptr;
        SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpPlaybackChanged playback:%d",playback);
        if(0x01 == playback)
        {
            if(0 == s_a2dp_sink_handle)
            {
                //a2dp sink audio priority has adjusted to MMISRVAUD_PRI_NORMAL_N10, it will not reuqest audio correctly if local audio is using like video player
                //so we request a2dp sink audio here
                if(FALSE == MMIBT_RequestA2dpSinkAudio())
                {
                    s_a2dp_local_play_status = 0x00;
                }
                else
                {
                    s_a2dp_local_play_status = 0x01;  //PLAYING
                    MMIBT_SetA2dpSinkVolume(MMIAPISET_GetMultimVolume());
                }
            }
            else
            {
                if(0x01 == s_a2dp_remote_play_status && 0x02 == s_a2dp_local_play_status)
                {
                    BOOLEAN retVal = FALSE;
                    SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpPlaybackChanged MMISRVAUD_Resume");
                    retVal = MMISRVAUD_Resume(s_a2dp_sink_handle);
                    SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpPlaybackChanged MMISRVAUD_Resume:%u",retVal);
                    
                    if(TRUE == retVal)
                    {
                        s_a2dp_local_play_status = 0x01;
                    }
                }
            }
        }

//912106
#ifndef WIN32
        SCI_SLEEP(5); 
        retVal = AVRCP_CT_GetMdaMediaInfo(0x7F);
        SCI_TRACE_LOW("lzk log.BT_AV, MMIBT_HandleAvrcpPlaybackChanged retVal:%d",retVal);
#endif
//912106
        MMIAPIMp3_UpdatePlayback(playback);
    }
}

PUBLIC void MMIBT_HandleAvrcpPlayPositionChanged(BT_MSG_T* msg_body_ptr)
{
	uint32 position;

    SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpPlayPositionChanged status:%d",msg_body_ptr->status);
    if(BT_SUCCESS == msg_body_ptr->status)
    {
        position = *(uint32*)msg_body_ptr->body_ptr;
        SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpPlayPositionChanged position:%d",position);
        if(position != 0xFFFFFFFF && position > 0)
            MMIAPIMp3_UpdatePlayPosition(position);
    }
}

PUBLIC void MMIBT_HandleAvrcpPlayerApplicationChanged(BT_MSG_T* msg_body_ptr)
{
	SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpPlayerApplicationChanged");
	//MMIAPIMp3_UpdatePlayPosition(*(uint32*)msg_body_ptr->body_ptr);
}

PUBLIC void MMIBT_HandleAvrcpSupportedPlayerAttributes(BT_MSG_T* msg_body_ptr)
{
    BT_STATUS retVal = BT_ERROR;

	SCI_TRACE_LOW("BT_AV, MMIBT_HandleAvrcpSupportedPlayerAttributes attributes:%X",*(uint8*)msg_body_ptr->body_ptr);
    #ifndef WIN32
    //second -> get media attributes
    retVal = AVRCP_CT_GetMdaMediaInfo(0x7F);
    SCI_TRACE_LOW("BT_AV, AVRCP_CT_GetMdaMediaInfo retVal:%d",retVal);
	#endif
}
#endif

#ifdef BT_HID_SUPPORT
PUBLIC void MMIBT_HandleHIDConnected(BT_MSG_T* msg_body_ptr)
{
#ifndef WIN32
#ifdef BT_DIALER_SUPPORT
    ConnectHandsetGatewayService();
#endif
#endif
}
#endif

#endif
