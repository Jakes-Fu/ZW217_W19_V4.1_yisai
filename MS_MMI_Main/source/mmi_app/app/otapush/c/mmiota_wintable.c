/****************************************************************************
** File Name:      mmimms_wintable.c                                       *
** Author:                                                                 *
** Date:           2004.8.23                                               *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the MMS                   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2004.8        Bruce.Chi         Create
** 
****************************************************************************/

/**-------------------------------------------------------------------------*
**                        Include Files                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_otapush_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmk_type.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmipb_export.h"
#include "mmi_default.h"
#include "guirichtext.h"
#include "guimenu.h"
#include "mmifmm_export.h"
#include "guibutton.h"
#if (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#include "mmiwidget_export.h"
#endif
//#include "mmipic_image.h"
#include "mmi_image.h"
#include "mmi_text.h"
//#include "guires.h"
#include "mmisms_export.h"
#include "mmisms_id.h"
#include "mmiconnection_export.h"
#include "gui_ucs2b_converter.h"
//#include "mmimms_internal.h"
#include "mmicc_export.h"
#include "mmipb_export.h"
#include "sysMemFile.h"
#include "wsp_header_api.h"
#include "mmiidle_export.h"
#include "mmiidle_statusbar.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "guiedit.h"
#include "mmidc_export.h"
#include "mmimms_setting.h"
#include "guitext.h"
#include "mmimms_text.h"
#include "mmimms_export.h"
#include "mmimms_control_main.h"
#include "mmiota_export.h"
#include "mmiota_id.h"
#include "mmiota_menutable.h"
#include "mmiota_main.h"
#include "mmiota_text.h"
#include "mmisms_text.h"
#ifdef MMI_RECORD_SUPPORT
#include "mmirecord_export.h"
#endif
#include "mmidc_export.h"
#include "mmiudisk_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#ifdef VIDEO_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef MOBILE_VIDEO_SUPPORT
#include "mmimv_export.h"
#endif

#ifdef MMIWIDGET_SUPPORT
#include "mmiwidget.h"
#include"mmiwidget_id.h"
#endif
#include "mmiconnection_text.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmicom_extract.h"
#include "mmienvset_export.h"
#ifdef ATV_SUPPORT
#include "mmiatv_export.h"
#endif
#include "mmialarm_export.h"

#ifdef MMI_OTA_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define OTA_CONFIG_INFO_NUM_MAX          MMICMSBRW_MAX_ITEM_NUM      //ota info 数量
#define OTA_CONFIG_SENDER_MAX_LEN        20            //ota收件人最大长度
#define PUSH_SI_TIME_LEN                 10

#define WAP_PUSH_NAME_MAX_LEN            24
#define OTA_MAX_URL_LEN                  512
#define OTA_MAX_CONTENT_LEN              100
#define OTA_RESERVED_SPACE               (5*1024)

#define CURR_DIRECTORY                "\\"
#define MANA_FLASH_OTAFILE          "cmsotapath.ini"   //ota file存储文件
#define MANAOTAFILE                 "cmsotapath.cnf"   
#define MMIMMS_MMS_DIR              "MMS"
#define MMIOTA_WAP_PUSHMSG_FILE_NAME "cmspushmsg.dat"
#define	MMICMSBRW_WAP_PUSHMSG_FILE_NAME	     (g_mmiwap_pushmsg_file_name_str)	//pushmsg存储的文件名
const wchar g_mmiwap_dir_wstr[] = { 'M', 'M', 'S', 0 };
#define		MMICMSBRW_WAP_UCS2_DIR						(g_mmiwap_dir_wstr)//"\x00\x63\x00\x6d\x00\x73\x00\x62\x00\x72\x00\x77\x00\x00"  //cmsbrw
#define		MMICMSBRW_WAP_UCS2_DIR_LEN					3                  //in the unit of uint8

#define MMIOTA_TIME_STR_LEN 32
#ifdef OMA_CP_SEC_SUPPORT
#define MMIOTA_USERPIN_INPUT_MAX_TIMES 4
#endif

extern int stricmp (const char *a, const char *b);
/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct 
{
    BOOLEAN               is_locked;
    MMISMS_FOLDER_TYPE_E  folder_type;//信箱类型
    BOOLEAN               is_unload;
    MN_DUAL_SYS_E         dual_sys;
    MMISMS_MO_MT_TYPE_E   mo_mt_type;
    MMIOTA_SETTING_TYPE_E ota_push_type;
    uint8                 cur_select;
    uint8                 sender[OTA_CONFIG_SENDER_MAX_LEN];
    uint16                index;
    uint32                time;    
    MMIOTA_SETTING_T    ota_setting_ptr;
}MMIOTA_CONFIG_INFO_T, *P_MMIOTA_CONFIG_INFO_T;

typedef struct 
{ 
    MMIOTA_CONFIG_INFO_T    ota_config_info[OTA_CONFIG_INFO_NUM_MAX];
}MMIOTA_CONFIG_TOTAL_INFO_T, *P_MMIOTA_CONFIG_TOTAL_INFO_T;

typedef enum
{
    MMIHOMEPAGE,
    MMIGATEWAY,
//    MMIPORT,
    MMINAME,
//    MMIPASSWORD,
    MMIAPN
}MMICONFIGSTATUS;

typedef struct
{
    wchar					name[WAP_PUSH_NAME_MAX_LEN + 1];		//item标题
    uint16					name_len;								//item标题长度
    uint8					url[OTA_MAX_URL_LEN + 1];				//item网址
    uint16					url_len;								//item网址长度
    uint8                   si_id[OTA_MAX_URL_LEN + 1];             //si_id
    uint16                  si_id_len;
    wchar                   content[OTA_MAX_CONTENT_LEN + 1];       //内容
    uint16                  content_len;
    int32                   dual_sys;
    int32			        list_num;	                            //当前item所处的列表框位置,-1表示当前数组成员没有使用(用0表示数组成员没有被使用)
    int32                   time;
    int32                   expired_time;
    uint8                   sender[OTA_CONFIG_SENDER_MAX_LEN];
    uint8					is_unload;								//item的总数量
    BOOLEAN                 is_locked;//shifou 锁定
    MMISMS_FOLDER_TYPE_E    folder_type;//信箱类型
    MMIOTA_SETTING_TYPE_E   wap_push_type;
    MMIOTA_SI_SIGNAL_ACTION_E  si_action;
    MMIOTA_SL_EXECUTE_ACTION_E sl_action;
}MMICMSBRW_ITEM_DETAIL_T;//item的结构

typedef struct
{
    uint8						item_num;								//item的总数量
    MMICMSBRW_ITEM_DETAIL_T		item_detail[MMICMSBRW_MAX_ITEM_NUM];	//item的信息
}MMICMSBRW_ITEM_T;//item的结构

typedef struct
{
    MMICMSBRW_ITEM_DETAIL_T		temp_detail;	//表示执行文件操作时的暂存,如果总数小于最大设定值,该结构各成员均设定为0
    int32						last_temp_num;	//表示当前执行文件操作时的数组下表，在文件操作失败的时候用来被恢复
}MMICMSBRW_ITEM_TEMP_T;//item的暂存结构
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

P_MMIOTA_CONFIG_TOTAL_INFO_T g_ota_config_ptr = PNULL;
//MMIOTA_CONFIG_INFO_T g_ota_config_item[OTA_CONFIG_INFO_NUM_MAX];
MMICMSBRW_ITEM_T * g_push_info_ptr = PNULL;
//MMICMSBRW_ITEM_T  g_cmsbrw_pushmsg_info = {0};		//pushmsg

//MMICMSBRW_ITEM_TEMP_T  g_cmsbrw_item_temp_info;	//item的暂存结构

uint8       g_defaut_now_saving_config = 0; /*lint -esym(552,g_defaut_now_saving_config) */
MMIFILE_DEVICE_E g_filetype = MMI_DEVICE_UDISK;
MMIFILE_DEVICE_E g_ffsType;
LOCAL   uint16  g_push_record_id=0;//弹出浏览器消息提示的push的record id
LOCAL BOOLEAN is_idle_read_push = FALSE;
const wchar g_mmiwap_pushmsg_file_name_str[] = {'c', 'm', 's', 'p', 'u', 's', 'h', 'm', 's', 'g', '.', 'd', 'a', 't', 0};	//pushmsg存储的文件名
#ifdef OMA_CP_SEC_SUPPORT
uint16 g_ota_userpin_remain_times = MMIOTA_USERPIN_INPUT_MAX_TIMES;
#endif
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :insert ota into sms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_InsertOTAToSms(uint16 index);

/*****************************************************************************/
//  Description :store ota file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_StoreOTAPUSH(void);
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify OTA
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyOTAFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);
#endif
#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//     Description : to handle window message
//    Global resource dependence : 
//  Author:wancan.you
//    Note: 
/*****************************************************************************/
// LOCAL MMI_RESULT_E  HandleOTASelectSimWinMsg(
//                                          MMI_WIN_ID_T win_id, 
//                                          MMI_MESSAGE_ID_E msg_id, 
//                                          DPARAM param
//                                          );
#endif

/*****************************************************************************/
//  Description :save ota config to link list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_SaveConfig(MN_DUAL_SYS_E dual_sys, uint16 cur_sel);

/*****************************************************************************/
//  Description :save ota config setting value
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  SaveOTASettingValue(MN_DUAL_SYS_E dual_sys, MMIOTA_SETTING_T *ota_setting_ptr);

/*****************************************************************************/
//  Description :save ota config bookmark value
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  SaveOTABookmarkValue(MMIOTA_SETTING_T *ota_setting_ptr);

/*****************************************************************************/
//     Description : config info detail  window message handler
//    Global resource dependence : 
//  Author:nihongf
//    Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOTAConfigInfoDetailWinMsg(
                                                    MMIOTA_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Description : if it is mms ota
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_IsMMSOTA(int16 index);

/*****************************************************************************/
//  Description : show detail of ota config
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL void MMSConfigDetailEntry(MMI_CTRL_ID_T ctrl_id, MMIOTA_CONFIG_INFO_T *ota_item_ptr, BOOLEAN mms_ota_yes);

/*****************************************************************************/
//  Description : show ota detail add items
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL void MMSSettingsDetailEntry(MMI_CTRL_ID_T ctrl_id, MMIOTA_SETTING_T *ota_setting_ptr, BOOLEAN mms_ota_yes);

/*****************************************************************************/
//  Description : form richtext string
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Detail_FormConfigInfo(
                                    MMI_STRING_T    *str_dst,        //out.
                                    uint8*                cur_str,
                                    MMICONFIGSTATUS     state,
                                    BOOLEAN mms_ota_yes
                                    );

/*****************************************************************************/
//  Description : config info detail  window option menu handle function
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOTAConfigSetOptionWinMsg(
                                                   MMIOTA_WINDOW_ID_E win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                                   );

/*****************************************************************************/
//  Description : config info  window detail entry
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL void OTAConfigItemDetailEntry(MMI_CTRL_ID_T ctrl_id, MMIOTA_CONFIG_INFO_T *ota_item_ptr);

/*****************************************************************************/
//  Description : config info  window detail such as from
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOTAConfigItemDetailWinMsg(
                                                    MMIOTA_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    );

/*****************************************************************************/
//  Description :read ota file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_ReadOtaFile(void);

/*****************************************************************************/
//  Description :read ota item from file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_ReadOtaItem(uint8 *data_ptr, uint32 total_size);


/*****************************************************************************/
//  Description :deal with the si push
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_DealWapPush(MMIOTA_SETTING_T  *mmi_setting_ptr, uint8 *sender_ptr, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// Description : Handle SL Push Prompt Win
// Global resource dependence :
// Author: 
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSLPushPromptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description :save data to wap push globe struct
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL int32 MMIOTA_WriteWapPushDataToItemStruct(MMICMSBRW_ITEM_T *item_info, 
                                                wchar*content_ptr,
                                                wchar*pTitle, 
                                                char* pURL,
                                                BOOLEAN ErrorProtect,
                                                BOOLEAN is_wappush
                                                );

/*****************************************************************************/
//  Description :insert wap push into sms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_InsertWapPushToSms(uint16 index);

/*****************************************************************************/
//  Description :save wap push file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL CMSBRW_FS_OPERATE_RESULT MMIOTA_WriteWapPushFile(uint32 source_win_id, 
                                                       const wchar* flash_filename, 
                                                       uint8* savebuf, 
                                                       int32 filelen, 
                                                       uint32 win_id, 
                                                       BOOLEAN IsJumpDialog, 
                                                       CMSBRW_FS_OPERATE_TYPE fileType
                                                       );

/*****************************************************************************/
//  Description :set wap push record id
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_SetPushRecordID(uint16 push_record_id);

/*****************************************************************************/
//  Description :get wap push record id
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16  MMIOTA_GetPushRecordID( void);

/*****************************************************************************/
//  Description :new wap push alert win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIOTA_HandleWapPushNewMsgWin(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  );

/*****************************************************************************/
//  Description :delete ota push and write file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_WriteWapPushFileWithCallBack(const wchar* file_name, 
                                                  CMSBRW_FS_OPERATE_TYPE file_type,
                                                  uint8* buf,
                                                  int32 buf_len,
                                                  Brw_Cbf cbf,
                                                  void *param);

/*****************************************************************************/
//  Description :get flag lock
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_FileOpCbf(SFS_ERROR_E efs_error, //error 
                            uint32 param,  //param
                            uint32 result, //size writed
                            uint32 param2
                               );

/*****************************************************************************/
//  Description :show wap push handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowPushmsgWinMsg(
                                           MMIOTA_WINDOW_ID_E	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM				param
                                           );

/*****************************************************************************/
//  Description :show wap push menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePushmsgMenuWinMsg(
                                           MMIOTA_WINDOW_ID_E		win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM				param
                                           );

/*****************************************************************************/
//  Description : get ota items total number
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL uint16 MMIOTA_GetOTAItemNum(void);

/*****************************************************************************/
//  Description :init wap push file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_InitWapPush(void);

/*****************************************************************************/
//  Description : 清除所有push条目
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void MMIOTA_FreePushList(void);

/*****************************************************************************/
//  Description :cat tow string
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OTACatString(MMI_STRING_T *dst_ptr, MMI_STRING_T *src1_ptr, MMI_STRING_T *src2_ptr);

/*****************************************************************************/
//  Description :detail time
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void OTADetailTimeString(MMI_STRING_T *str_dst, uint32 time, MMI_TEXT_ID_T text_id);

/*****************************************************************************/
//  Description :free item data
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FreeItemDetailData(MMI_STRING_T *str_dst);

/*****************************************************************************/
//  Description :show wap push richtext entity
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void WapPushShowEntry(MMI_CTRL_ID_T ctrl_id, uint16 index);

/*****************************************************************************/
//  Description :wap push update state to be read to have read
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  MMIOTA_WapPushUpdateState(uint16 ID);

/*****************************************************************************/
//  Description :inbox enter ota and update the state to be read to have read
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_OTAUpdateState(uint16 id);

/*****************************************************************************/
//  Description :get si time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint32 MMIOTA_GetSiTime(uint8 *si_time_ptr, uint16 si_time_len);

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsForbitNewMsgWin(void);

/*****************************************************************************/
//  Description : Select SIM API of OTA app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIOTA_SelectSIMFunc(ADD_DATA    add_data);

#ifdef OMA_CP_SEC_SUPPORT
/*****************************************************************************/
// 	Description : 处理PIN INPUT的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOTAUserPinInputWindow (
                                         MMI_WIN_ID_T     win_id, 	// PIN INPUTING窗口的ID
                                         MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                         DPARAM 		        param		// 相应消息的参数
                                         );
#endif
/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
WINDOW_TABLE(MMIOTA_CONFIG_INFO_DETAIL_WIN) = 
{
    WIN_TITLE(TXT_MMS_CONFIG_INFO),
    WIN_FUNC((uint32) HandleOTAConfigInfoDetailWinMsg),    
    WIN_ID(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID),    
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIOTA_CONFIG_INFO_DETAIL_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIOTA_CONFIG_SET_OPTION_TAB) = 
{
    WIN_FUNC((uint32)HandleOTAConfigSetOptionWinMsg),      
    WIN_ID(MMIOTA_CONFIG_SET_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    //WIN_TITLE(TXT_NULL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_OTA_CONFIG_SET_OPTION, MMIOTA_CONFIG_SET_MENU_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIOTA_CONFIG_ITEM_DETAIL_WIN) = 
{
    WIN_TITLE(TXT_MMS_CONFIG_INFO),
    WIN_FUNC((uint32) HandleOTAConfigItemDetailWinMsg),    
    WIN_ID(MMIOTA_CONFIG_ITEM_DETAIL_WIN_ID),    
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIOTA_CONFIG_ITEM_DETAIL_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMICMSBRW_SHOWPUSHMSG_WIN_TAB) = 
{
    WIN_FUNC((uint32) HandleShowPushmsgWinMsg),    
    WIN_ID(MMICMSBRW_SHOWPUSHMSG_WIN_ID),
    WIN_TITLE(TXT_OTA_PUSHMSG_SERVICE),
    WIN_SOFTKEY(STXT_OPTION, TXT_NULL, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMICMSBRW_PUSHMSGMENU_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePushmsgMenuWinMsg),
    WIN_ID(MMICMSBRW_PUSHMSGMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_POPMENU_CTRL(MENU_CMSBRW_PUSHMSG, MMIOTA_MENU_PUSHMSG_CTRL_ID),
    END_WIN
};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*	//Sam.hua	use SelectSimFunc 
WINDOW_TABLE(MMIOTA_SELECT_SIM_WIN_TAB) = 
{
    WIN_TITLE(TXT_SIM_SEL),
    WIN_FUNC((uint32) HandleOTASelectSimWinMsg),    
    WIN_ID( MMIOTA_SELECT_SIM_WIN_ID ),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIOTA_SELECT_SIM_CTRL_ID),

    END_WIN
};
*/
#endif
#if defined(OMA_CP_SEC_SUPPORT)
// the window for input pin,startup
WINDOW_TABLE( OTA_USER_PIN_INPUTING_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandleOTAUserPinInputWindow ),    
    WIN_ID( OTA_USER_PIN_INPUT_WIN_ID ),
    WIN_HIDE_STATUS,
    WIN_TITLE(TXT_NULL),
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE,MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID),
    CREATE_EDIT_PASSWORD_CTRL(MMI_PIN_MAX_LENGTH,PHONE_PIN_EDITBOX_CTRL_ID),
    WIN_STYLE(WS_DISABLE_HWICON | WS_DISABLE_RETURN_WIN),
#ifdef MMI_PDA_SUPPORT
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, TXT_NULL),                        
#else
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),                        
#endif
    END_WIN
};

#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN  1
#define MMIPHONE_PIN_PUK_REMAIN_HEIGHT      12
#else
#define MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN  5
#define MMIPHONE_PIN_PUK_REMAIN_HEIGHT      22
#endif

/*****************************************************************************/
// 	Description : PIN输入界面，alert界面\不允许red key,flip返回
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePinAlertWindow(
                                        MMI_WIN_ID_T     win_id, 	// 窗口的ID
                                        MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                        DPARAM 		        param		// 相应消息的参数
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id)
    {
    case MSG_APP_RED://red and flip不允许返回
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.
    case MSG_APP_FLIP:
        MMK_CloseWin(win_id);
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif        

    default:
        result = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (result);
}

/*****************************************************************************/
// 	Description : PIN/PUK input window softkey
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN PinPukInputWinSoftkey(
                                      wchar         *str_ptr,   //in
                                      uint16        str_len,    //in
                                      MMI_TEXT_ID_T *text_id_ptr//in/out:
                                      )
{
    BOOLEAN  result = FALSE;

    if (str_len >= MMI_PIN_MIN_LENGTH)
    {
        *text_id_ptr = TXT_COMMON_OK;
        result = TRUE;
    }

    return (result);
}
/*****************************************************************************/
// 	Description : GetPinPukEditRect
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL GUI_BOTH_RECT_T GetPinPukEditRect(MMI_WIN_ID_T win_id, BOOLEAN is_need_label)
{
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
	return GetPasswordEditBothRect(is_need_label);
#else
    GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

    both_rect.h_rect.left = both_rect.v_rect.left = MMI_PASSWORD_EDIT_LEFT;
    if (is_need_label)
    {
        both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN + MMIPHONE_PIN_PUK_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN + MMIPHONE_PIN_PUK_REMAIN_HEIGHT + MMI_PASSWORD_EDIT_TOP_MARGIN;
    }
    else
    {
        both_rect.h_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
        both_rect.v_rect.top += MMI_PASSWORD_EDIT_TOP_MARGIN;
    }

    both_rect.h_rect.bottom = both_rect.h_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.h_rect.right = both_rect.h_rect.right - MMI_PASSWORD_EDIT_LEFT;
    both_rect.v_rect.bottom = both_rect.v_rect.top + MMI_PASSWORD_EDIT_HEIGHT;
    both_rect.v_rect.right = both_rect.v_rect.right - MMI_PASSWORD_EDIT_LEFT;

    return both_rect;
#endif 
		
}
#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
// 	Description : 设置输入密码窗口的背景色，image 和title
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL void SetPasswordBackground(
                                 MMI_WIN_ID_T    win_id, //窗口的ID
                                 MMI_CTRL_ID_T   ctrl_id,   //the id of control
                                 BOOLEAN is_need_label
                                 )
{
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    GUI_RECT_T bg_rect = MMITHEME_GetClientRect();
    GUI_BOTH_RECT_T  both_client_rect = MMITHEME_GetWinClientBothRect(win_id);
#ifdef MONOCHROME_LCD_SUPPORT 
#else
    GUI_POINT_T dis_point = {0};
    uint16 icon_width = 0;
    uint16 icon_height = 0;	
    //GUI_BOTH_RECT_T  both_rect = {0};
    GUI_BOTH_RECT_T edit_both_rect = {0};
#endif 
    
    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    
    if(MMITHEME_IsMainScreenLandscape())
    {
        bg_rect = both_client_rect.h_rect;
    }
    else
    {
        bg_rect = both_client_rect.v_rect;
    }

#ifdef MONOCHROME_LCD_SUPPORT 
    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_WHITE_COLOR);
#else
    LCD_FillRect(&lcd_dev_info, bg_rect, MMI_BLACK_COLOR);
    GUIRES_GetImgWidthHeight(&icon_width, &icon_height, IMAGE_NUMBERINPUT_PIN, win_id);

    //设置image id
    IMG_EnableTransparentColor(TRUE);
    GUIRES_DisplayImg(PNULL,
            &bg_rect,
            &bg_rect,
            win_id,
            IMAGE_COMMON_BG,
            &lcd_dev_info);

    edit_both_rect = GetPinPukEditRect(win_id, is_need_label);

    dis_point.x = (bg_rect.right + bg_rect.left - icon_width) / 2;
    dis_point.y = (bg_rect.bottom + edit_both_rect.v_rect.bottom - icon_height) / 2;
    GUIRES_DisplayImg(&dis_point,
            PNULL,
            PNULL,
            win_id,
            IMAGE_NUMBERINPUT_PIN,
            &lcd_dev_info);
    
    IMG_EnableTransparentColor(FALSE); 
#endif 
}
#endif

PUBLIC uint16 getUserPinRemainStr(void)
{
	return g_ota_userpin_remain_times;
}

PUBLIC void setUserPinRemainStr(uint16 remain)
{
	g_ota_userpin_remain_times=remain;
}

LOCAL void MMIPHONE_GetUserPinRemainStr(MMI_STRING_T *pin_puk_str_ptr)
{
    uint16 pin_puk_remain = 0;
    MMI_STRING_T remain_str = {0};
    MMI_STRING_T times_str = {0};
    uint8 pin_puk_remain_ptr[8] = {0};
    uint16 length = 0;

    if(PNULL == pin_puk_str_ptr)
    {
        return;
    }

    pin_puk_remain = getUserPinRemainStr();

    MMI_GetLabelTextByLang(TXT_PIN_PUK_REMAIN, &remain_str);
    MMI_GetLabelTextByLang(TXT_PIN_PUK_TIMES, &times_str);

    sprintf((char *)pin_puk_remain_ptr, " %d ", pin_puk_remain);

    length = strlen( (char *)pin_puk_remain_ptr );

    pin_puk_str_ptr->wstr_ptr = SCI_ALLOC_APPZ((remain_str.wstr_len + length + times_str.wstr_len) * sizeof(wchar));

    MMI_WSTRNCPY(pin_puk_str_ptr->wstr_ptr, remain_str.wstr_len, remain_str.wstr_ptr, remain_str.wstr_len, remain_str.wstr_len);
    pin_puk_str_ptr->wstr_len = remain_str.wstr_len;
    
    MMI_STRNTOWSTR( pin_puk_str_ptr->wstr_ptr+pin_puk_str_ptr->wstr_len, length, (uint8 *)pin_puk_remain_ptr, 8, length );
    pin_puk_str_ptr->wstr_len += length;
    
    MMI_WSTRNCPY((pin_puk_str_ptr->wstr_ptr+pin_puk_str_ptr->wstr_len), times_str.wstr_len, times_str.wstr_ptr, times_str.wstr_len, times_str.wstr_len);
    pin_puk_str_ptr->wstr_len += times_str.wstr_len;
}
/*****************************************************************************/
// 	Description : 处理PIN INPUT的消息
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOTAUserPinInputWindow (
                                         MMI_WIN_ID_T     win_id, 	// PIN INPUTING窗口的ID
                                         MMI_MESSAGE_ID_E 	msg_id, 	// 窗口的内部消息ID
                                         DPARAM 		        param		// 相应消息的参数
                                         )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_STRING_T pin_puk_remain_str = {0};
    MMI_STRING_T pin_value = {0};   
    GUI_BOTH_RECT_T edit_both_rect = {0};
    OTA_WIN_DATA_T* add_data_ptr =(OTA_WIN_DATA_T *) MMK_GetWinAddDataPtr(win_id);
    static BOOLEAN userpin_is_right = FALSE;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
#ifdef MAINLCD_SIZE_128X64 //added by taiping.lai 2011/01/03
        {
            GUI_BOTH_RECT_T both_rect = GetPasswordLabelBothRect();
		 	  GUI_RECT_T title_rect = GetPasswordTitleRect();

		     GUIWIN_SetTitleRect(win_id, title_rect);	
            GUIAPICTRL_SetBothRect(MMIPHONE_PIN_INPUT_REMAIN_LABEL_CTRL_ID, &both_rect);
		}
#else					
        {
            GUI_BOTH_RECT_T both_rect = MMITHEME_GetWinClientBothRect(win_id);

            both_rect.h_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.v_rect.top += MMIPHONE_PIN_PUK_REMAIN_TOP_MARGIN;
            both_rect.h_rect.bottom = both_rect.h_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;
            both_rect.v_rect.bottom = both_rect.v_rect.top + MMIPHONE_PIN_PUK_REMAIN_HEIGHT;

            GUIAPICTRL_SetBothRect(MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID, &both_rect);
        }
#endif
        IGUICTRL_SetState(MMK_GetCtrlPtr(MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID), GUICTRL_STATE_DISABLE_ACTIVE, TRUE);

        edit_both_rect = GetPinPukEditRect(win_id, TRUE);
        GUIAPICTRL_SetBothRect(PHONE_PIN_EDITBOX_CTRL_ID, &edit_both_rect);

        GUIEDIT_SetPasswordStyle(PHONE_PIN_EDITBOX_CTRL_ID,GUIEDIT_PASSWORD_STYLE_DIGITAL_PIN);
        
        GUIWIN_SetTitleTextId(win_id,TXT_ENTER_PIN,FALSE);

        GUIEDIT_SetSoftkey(PHONE_PIN_EDITBOX_CTRL_ID, 0, MMI_PIN_MIN_LENGTH,TXT_NULL,TXT_COMMON_OK, PinPukInputWinSoftkey);

#if defined(MMI_PDA_SUPPORT) && defined(MMIPUBWIN_PDASTYLE_SUPPORT)
        // PUBEDIT负责softkey的显示，PDA下需要禁止右键
        GUIEDIT_SetSoftkey(PHONE_PIN_EDITBOX_CTRL_ID, 2, 0,TXT_NULL,TXT_NULL, PNULL);
#endif

        SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
        MMIPHONE_GetUserPinRemainStr(&pin_puk_remain_str);

        if (PNULL != pin_puk_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID,&pin_puk_remain_str,FALSE);

            SCI_FREE(pin_puk_remain_str.wstr_ptr);
            pin_puk_remain_str.wstr_ptr = PNULL;
        }
        MMK_SetWinDisplayStyleState(win_id, WS_DISABLE_RETURN_WIN, FALSE);
        
        MMK_SetAtvCtrl(win_id,PHONE_PIN_EDITBOX_CTRL_ID);
        break;


    case MSG_FULL_PAINT:
#ifndef MMI_PDA_SUPPORT
        SetPasswordBackground(win_id,PHONE_PIN_EDITBOX_CTRL_ID,TRUE);
#endif
#ifndef SUBLCD_SIZE_NONE       
        //刷新小屏的显示，输入PIN码
        MMISUB_IsPermitUpdate(TRUE);
        MMISUB_SetSubLcdDisplay(FALSE,TRUE,SUB_CONTENT_PIN,PNULL);
#endif
        SCI_MEMSET(&pin_puk_remain_str, 0x00, sizeof(MMI_STRING_T));
        MMIPHONE_GetUserPinRemainStr(&pin_puk_remain_str);

        if (PNULL != pin_puk_remain_str.wstr_ptr)
        {
            GUILABEL_SetText(MMIPHONE_PIN_REMAIN_LABEL_CTRL_ID,&pin_puk_remain_str,FALSE);

            SCI_FREE(pin_puk_remain_str.wstr_ptr);
            pin_puk_remain_str.wstr_ptr = PNULL;
        }
        break;
        
    case MSG_GET_FOCUS:
        MMIDEFAULT_AllowOpenKeylocWin(FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        // get the value about PIN
        GUIEDIT_GetString(PHONE_PIN_EDITBOX_CTRL_ID,&pin_value);

       if (PIN_LEN_IS_IN_RANGE(pin_value.wstr_len) )
        {
            unsigned char		hashValue[20];
            unsigned char		hashValueString[40];
            uint8 hashValueStringLen  = 0;
            unsigned char		hmacKey[20];
            uint32				hmacKeyLen;
            int i=0;
            uint8 pinValue[16]={0};
	     uint8 key[32]={0};
            uint8 keyLen=0;
            MN_IMSI_T imsi = {0};

            for (i=0;i<pin_value.wstr_len;i++)
            {
                pinValue[i] = pin_value.wstr_ptr[i];
            }
			
            imsi = MNSIM_GetImsiEx(add_data_ptr->security_info.dual_sys);	

	     if (add_data_ptr->security_info.sec == 2)
	     {
		     memset(key,0,sizeof(key));
		     memcpy(key,imsi.imsi_val,imsi.imsi_len);	  
		     keyLen+=imsi.imsi_len;		   
		     memcpy(key+keyLen,pinValue,pin_value.wstr_len);
		     keyLen+=pin_value.wstr_len;
	     }	else //==1
	     {
		     memset(key,0,sizeof(key));
		     memcpy(key,pinValue,pin_value.wstr_len);
		     keyLen+=pin_value.wstr_len;
	     }
			
            HmacSha1((unsigned char *)key, 
                        keyLen,
                        (const unsigned char *)add_data_ptr->security_info.wbxml,
                        add_data_ptr->security_info.wbxml_length, 
                        hashValue, hmacKey,&hmacKeyLen); 
            for (i=0;i<20;i++){
                    sprintf(hashValueString+hashValueStringLen,"%02X",hashValue[i]);
                    hashValueStringLen += 2;
            }
            SCI_TRACE_LOW("[OTA] mac:%s",add_data_ptr->security_info.mac);			
            SCI_TRACE_LOW("[OTA] hashValueString:%s",hashValueString);
            if (0==strncmp(hashValueString,add_data_ptr->security_info.mac,40))
            {
                userpin_is_right = TRUE;
            }	else
            {
                userpin_is_right = FALSE;
            }
            if (userpin_is_right)
            {
                SCI_TRACE_LOW("[OTA] input user pin :USERPIN TRUE");		  
                
		  MMIAPIOTA_ParseWBXMLWithoutSecurityCheck(add_data_ptr->push_data_ptr, add_data_ptr->push_data_len,
		  	add_data_ptr->provision_setting, add_data_ptr->send_num, add_data_ptr->security_info.dual_sys);
		  MMIPUB_OpenAlertSuccessWin(TXT_SUCCESS);
		  MMK_CloseWin(win_id);
            }	
            else
            {
                GUIEDIT_ClearAllStr(PHONE_PIN_EDITBOX_CTRL_ID);
		  setUserPinRemainStr(getUserPinRemainStr()-1>0?(getUserPinRemainStr()-1):0);		
		  if (getUserPinRemainStr() > 0)
		  {
			MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
		  }else{
		  	MMIPUB_OpenAlertFailWin(TXT_ERROR);
			MMK_CloseWin(win_id);
		  }
		  
                SCI_TRACE_LOW("[OTA] input user pin :USERPIN FALSE");
            }					
        }
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
    case MSG_APP_RED://允许取消
        //MMIPUB_OpenAlertWinByTextId(PNULL,TXT_PLS_INPUT_PIN,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,HandlePinAlertWindow);
        MMK_CloseWin(win_id);
        userpin_is_right = FALSE;	
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.        
    case MSG_APP_FLIP:
        MMIDEFAULT_HandleFlipKey(FALSE);
        break;
#endif
        
    case MSG_CLOSE_WINDOW:
        MMIDEFAULT_AllowOpenKeylocWin(TRUE);
#ifndef SUBLCD_SIZE_NONE
        //刷新小屏的显示，关闭输入PIN码
        MMISUB_SetSubLcdDisplay(FALSE,FALSE,SUB_CONTENT_PIN,PNULL);
#endif
#if 0
        if (!userpin_is_right)
        {
        SCI_TRACE_LOW("[OTA] input user pin :delete ota config count:%d",add_data_ptr->count);
        MMIAPIOTA_DeleteOTAPUSH(add_data_ptr->count);
//                    if(g_ota_config_ptr->ota_config_info[i].ota_setting_ptr.setting_type == MMIOTA_MMS_SETTING)
//                    {
//                        MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MMS_OTA,i);
//                    }
//                    else
        {            
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_OTA,add_data_ptr->count);
        }
        }
#endif		
	 setUserPinRemainStr(MMIOTA_USERPIN_INPUT_MAX_TIMES);	
	 if (PNULL != add_data_ptr->security_info.wbxml)
	 {
		SCI_FREE(add_data_ptr->security_info.wbxml);
	 }
	 if (PNULL != add_data_ptr->push_data_ptr)
	 {
		SCI_FREE(add_data_ptr->push_data_ptr);
	 }	 
	 MMK_FreeWinAddData(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return (result);
}

PUBLIC void enterOTAPinInputWin(ADD_DATA add_data_ptr)
{
	MMK_CreateWin(OTA_USER_PIN_INPUTING_WIN_TAB,  add_data_ptr);
}
#endif
#ifdef SIM_PLUG_IN_SUPPORT
/*****************************************************************************/
//     Description : Handle SIM Plug Notify OTA
//    Global resource dependence : 
//  Author: wancan.you
//    Note:
/*****************************************************************************/
LOCAL void HandleSIMPlugNotifyOTAFunc(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param)
{
    //SCI_TRACE_LOW:"[MMISMS]: HandleSIMPlugNotifyOTAFunc dual_sys=%d, notify_event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_551_112_2_18_2_42_46_8,(uint8*)"dd", dual_sys, notify_event);

    switch (notify_event)
    {
    case MMIPHONE_NOTIFY_SIM_PLUG_IN:
    case MMIPHONE_NOTIFY_SIM_PLUG_OUT:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MMK_CloseWin(MMIOTA_COM_SELECT_SIM_WIN_ID);
#endif
        break;

    default:
        break;
    }
}
#endif
/*****************************************************************************/
//  Description :init ota push
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_InitOTAPUSH(void)
{
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T sim_plug_notify_info = {0};
    
    MMIOTA_RegWinIdNameArr();    
    MMIOTA_RegMenuGroup();
    MMIOTA_FreePushList();
    MMIOTA_ReadOtaFile();
    MMIOTA_InitWapPush();
    MMIAPIOTA_LoadWapPushListToSms();

    sim_plug_notify_info.module_id = MMI_MODULE_OTA;
#ifdef SIM_PLUG_IN_SUPPORT
    sim_plug_notify_info.notify_func = HandleSIMPlugNotifyOTAFunc;
    MMIAPIPHONE_RegisterSIMPlugNotifyInfo(&sim_plug_notify_info);
#endif
#ifdef MMI_OTA_SUPPORT
    MMIOTA_PushInit();
#endif
}

/*****************************************************************************/
//  Description : 清除所有push条目
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
LOCAL void MMIOTA_FreePushList(void)
{
    uint16 i                        = 0;
    uint8    count                  = 0;
    
    if (PNULL != g_push_info_ptr)
    {
        for(;i<MMICMSBRW_MAX_ITEM_NUM;i++)
        {
            if(g_push_info_ptr->item_detail[i].list_num != -1)
            {
                //SCI_TRACE_LOW:"MMIOTA_FreePushList Sms index=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_602_112_2_18_2_42_46_9,(uint8*)"d",i);
                MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH,i);            
                SCI_MEMSET(&g_push_info_ptr->item_detail[i], 0, sizeof(MMICMSBRW_ITEM_DETAIL_T));
                g_push_info_ptr->item_detail[i].list_num = -1;
            }
        }
        SCI_FREE(g_push_info_ptr);
        g_push_info_ptr = PNULL;
    }    

    if (PNULL != g_ota_config_ptr)
    {
        for (count = 0; count < OTA_CONFIG_INFO_NUM_MAX; count++)
        {
            if (g_ota_config_ptr->ota_config_info[count].index != 0)
            {
                MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_OTA,count);
                g_ota_config_ptr->ota_config_info[count].index = 0;
            }
        }
        SCI_FREE(g_ota_config_ptr);
        g_ota_config_ptr = PNULL;
    }    
}


/*****************************************************************************/
//  Description :read ota file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_ReadOtaFile(void)
{
    uint32    size                          = 0;
    uint32   transmitted                    = 0;
    FFS_READFILE_RESULT_T   *param_read_ptr = PNULL;
    MMIFILE_HANDLE  cur_hefs                = SFS_INVALID_HANDLE;
    // FILE_HANDLE      cmshandle              = 0 ;
    
    
    //SCI_TRACE_LOW:"MMIOTA_ReadOtaFile IN -------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_641_112_2_18_2_42_46_10,(uint8*)"");
    
    if(MMIOTA_CheckFileExist((const uint8*)MANA_FLASH_OTAFILE) == TRUE)
    {
        //SCI_TRACE_LOW:"fnh ota.cnf is exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_645_112_2_18_2_42_46_11,(uint8*)"");
        cur_hefs = MMIOTA_FilecCreate((const uint8*)MANA_FLASH_OTAFILE, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING);/*lint !e655*/
        //SCI_TRACE_LOW:"fnh cur_hefs = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_647_112_2_18_2_42_46_12,(uint8*)"d",cur_hefs);
        
        if(SFS_INVALID_HANDLE != cur_hefs) 
        {
            SFS_ERROR_E        ffs_error = SFS_NO_ERROR;
            uint32 real_size = sizeof(MMIOTA_CONFIG_TOTAL_INFO_T);
            
            size = MMIOTA_FileGetLength(cur_hefs);
            if (0 == size || size != real_size)
            {
                wchar ucs2_name[OTA_UCS2_NAME_MAX_LEN] = {0};
                
                //SCI_TRACE_LOW:"fnh read ota file  size is zero!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_655_112_2_18_2_42_46_13,(uint8*)"");
                GUI_GBToWstr(ucs2_name, (uint8 *)MANA_FLASH_OTAFILE,(uint16)SCI_STRLEN((char*)MANA_FLASH_OTAFILE));
                MMIOTA_FileClose(cur_hefs);
                MMIOTA_FileDelete(ucs2_name, PNULL);                
                cur_hefs = SFS_INVALID_HANDLE;
                return ;
            }
            param_read_ptr =(FFS_READFILE_RESULT_T *)SCI_ALLOCAZ(sizeof(FFS_READFILE_RESULT_T));
            if (param_read_ptr == PNULL)
            {
                //SCI_TRACE_LOW:"fnh malloc param_read_ptr fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_663_112_2_18_2_42_46_14,(uint8*)"");
                MMIOTA_FileClose(cur_hefs);
                cur_hefs = SFS_INVALID_HANDLE;
                return ;
            }
            param_read_ptr->hefs = cur_hefs; //传递的参数
            param_read_ptr->buf_size = size;
            //SCI_TRACE_LOW:"fnh size is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_670_112_2_18_2_42_46_15,(uint8*)"d",size);
            
            param_read_ptr->buf_ptr = (uint8*)SCI_ALLOCAZ(param_read_ptr->buf_size);
            if (param_read_ptr->buf_ptr == PNULL)
            {
                //SCI_TRACE_LOW:"fnh malloc param_read_ptr->buf_ptr fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_675_112_2_18_2_42_46_16,(uint8*)"");
                SCI_FREE(param_read_ptr);
                param_read_ptr = NULL;
                MMIOTA_FileClose(cur_hefs);
                cur_hefs = SFS_INVALID_HANDLE;
                return ;
            }
            ffs_error = MMIOTA_FileRead(
                cur_hefs, 
                param_read_ptr->buf_ptr,
                param_read_ptr->buf_size, 
                &transmitted, 
                NULL);    //这里暂时定为同步
            
            //SCI_TRACE_LOW:"fnh after FILE_Read returnvalue = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_689_112_2_18_2_42_46_17,(uint8*)"d",ffs_error);
            MMIOTA_FileClose(cur_hefs);
            cur_hefs = SFS_INVALID_HANDLE;
            
            if( ( SFS_ERROR_IO_PENDING == ffs_error )||(SFS_NO_ERROR == ffs_error))
            {
                MMIOTA_ReadOtaItem(param_read_ptr->buf_ptr, param_read_ptr->buf_size);
            }
            
            if (param_read_ptr->buf_ptr!= NULL)
            {
                SCI_FREE(param_read_ptr->buf_ptr);
                param_read_ptr->buf_ptr = NULL;
            }
            SCI_FREE(param_read_ptr);
            param_read_ptr = NULL;
        }
        else
        {
            //SCI_TRACE_LOW:"FNH THE ota FILE IS failed!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_708_112_2_18_2_42_46_18,(uint8*)"");
            return;
        }
    }
    else
    {
        //SCI_TRACE_LOW:"FNH THE ota FILE IS not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_714_112_2_18_2_42_46_19,(uint8*)"");
        return;
    }
}

/*****************************************************************************/
//  Description :read ota item from file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_ReadOtaItem(uint8 *data_ptr, uint32 total_size)
{
    uint32 config_len                          = 0;
    uint32 setting_len                         = 0;
    uint32 len_local                           = 0;
    uint32 left_len                            = 0;
    uint16 config_index                        = 0;
    
    //SCI_TRACE_LOW:"MMIOTA_ReadOtaItem IN ----"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_731_112_2_18_2_42_46_20,(uint8*)"");
    setting_len = sizeof(MMIOTA_SETTING_T);
    config_len = sizeof(MMIOTA_CONFIG_INFO_T);
    
    if (PNULL == data_ptr || 0 == total_size)
    {
        return;
    }
    if (0 != (total_size - (total_size/config_len)*config_len))
    {
        return;
    }
    if (PNULL != g_ota_config_ptr)
    {
        SCI_FREE(g_ota_config_ptr);
        g_ota_config_ptr = PNULL;
    }
    g_ota_config_ptr = (P_MMIOTA_CONFIG_TOTAL_INFO_T)SCI_ALLOCAZ(sizeof(MMIOTA_CONFIG_TOTAL_INFO_T));
    if (PNULL == g_ota_config_ptr)
    {
        return;
    }
    left_len = total_size;
    config_index = 0;
    len_local = 0;
    while (left_len > 0 && len_local <= total_size)
    {       
        SCI_MEMCPY((char *)&(g_ota_config_ptr->ota_config_info[config_index]), data_ptr + len_local, config_len);
        len_local = len_local + config_len;
        if (left_len >= config_len)
        {
            left_len = left_len - config_len;
        }
        else
        {
            break;
        }
        MMIOTA_InsertOTAToSms(config_index);
        config_index++;
    }    
}

/*****************************************************************************/
//  Description :ota push callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_AddOTAPUSH(MMIOTA_SETTING_T  *mmi_setting_ptr, uint8 *sender_ptr, MN_DUAL_SYS_E dual_sys)
{  
    BOOLEAN  result                           = FALSE;
    BOOLEAN  is_ota_config_full               = TRUE;
    uint8    count                            = 0;
    uint8    min_index_count                  = 0;
    uint16   max_index                        = 0;
    uint16   min_index                        = 0;
    uint16   setting_size                     = sizeof(MMIOTA_SETTING_T);
    MMIOTA_SETTING_T *temp_setting_ptr        = PNULL;
    MMIOTA_SETTING_T *temp_config_setting_ptr = PNULL;
    uint16  sender_sr_len =0;
#if defined(MMI_BLACKLIST_SUPPORT)
    MMIPB_BCD_NUMBER_T black_bcd_num = {0};
    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
#endif

#if defined(MMI_BLACKLIST_SUPPORT)
    if (PNULL != sender_ptr)
    {
        sender_sr_len = SCI_STRLEN((char *)sender_ptr);
        if (0 < sender_sr_len && MMIAPICOM_GenPartyNumber(sender_ptr, sender_sr_len, &party_num))
        {
            black_bcd_num.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
            black_bcd_num.number_len = (uint8)MIN(MMIPB_BCD_NUMBER_MAX_LEN, party_num.num_len);
            SCI_MEMCPY(black_bcd_num.number, party_num.bcd_num, black_bcd_num.number_len);
            if(MMIAPISET_IsSMSBlacklist(&black_bcd_num))
            {
                //SCI_TRACE_LOW:"[MMIMMS]:MMIAPIOTA_AddOTAPUSH push sender is in black lists!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_805_112_2_18_2_42_46_21,(uint8*)"");
                return 0;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMMS]:MMIAPIOTA_AddOTAPUSH sender_sr_len is d%!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_811_112_2_18_2_42_46_22,(uint8*)"", sender_sr_len);
        }
    }
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIAPIOTA_AddOTAPUSH sender_ptr is PNULL!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_815_112_2_18_2_42_46_23,(uint8*)"");
    }
#endif

    if (PNULL == mmi_setting_ptr)
    {
        return FALSE;
    }
    if (mmi_setting_ptr->setting_type == MMIOTA_SETTING_MAX)
    {
        return FALSE;
    }
    if (mmi_setting_ptr->setting_type == MMIOTA_BRW_WAP_SI
        || mmi_setting_ptr->setting_type == MMIOTA_BRW_WAP_CO
        || mmi_setting_ptr->setting_type == MMIOTA_BRW_WAP_SL)
    {   //处理si类型
        if (!MMIAPISMS_GetPushSmsBrowserInfo())
        {
            return FALSE;
        }
        if (MMIOTA_DealWapPush(mmi_setting_ptr, sender_ptr, dual_sys))
        {
            result = TRUE;
        }
        return result;
    }
    
    if (!MMIOTA_IsEnoughSpace(OTA_RESERVED_SPACE))
    {
        MMIPUB_OpenAlertWarningWin(TXT_NO_SPACE);
        return FALSE;
    }
    if (PNULL != g_ota_config_ptr)
    {
        max_index = g_ota_config_ptr->ota_config_info[0].index;    
        for (count = 0; count < OTA_CONFIG_INFO_NUM_MAX; count++)//find the oldest and latest ota index
        {
            if (g_ota_config_ptr->ota_config_info[count].index == 0)
            {
                is_ota_config_full = FALSE;
            }
            if (g_ota_config_ptr->ota_config_info[count].index > max_index)
            {
                max_index = g_ota_config_ptr->ota_config_info[count].index;
            }        
        }  
        min_index = max_index;
        for (count = 0; count < OTA_CONFIG_INFO_NUM_MAX; count++)
        {
            if (g_ota_config_ptr->ota_config_info[count].index <= min_index && !g_ota_config_ptr->ota_config_info[count].is_locked)
            {
                min_index = g_ota_config_ptr->ota_config_info[count].index;
                min_index_count = count;
            }
        }
    }
    else
    {
        g_ota_config_ptr = (P_MMIOTA_CONFIG_TOTAL_INFO_T)SCI_ALLOCAZ(sizeof(MMIOTA_CONFIG_TOTAL_INFO_T));
        if (PNULL == g_ota_config_ptr)
        {
            return FALSE;
        }
        is_ota_config_full = FALSE;
    }
    
    if (!is_ota_config_full)//20条信息不满的时候
    {
        for (count = 0; count < OTA_CONFIG_INFO_NUM_MAX; count++)
        {
            if (g_ota_config_ptr->ota_config_info[count].index == 0 && (mmi_setting_ptr != PNULL))//当有空的index，同时配置和书签不同时空
            { 
                temp_setting_ptr = mmi_setting_ptr;
                temp_config_setting_ptr = (MMIOTA_SETTING_T *)&(g_ota_config_ptr->ota_config_info[count].ota_setting_ptr);
                SCI_MEMCPY(temp_config_setting_ptr, (char *)temp_setting_ptr, setting_size);
                if (PNULL != sender_ptr)
                {
                    sender_sr_len = SCI_STRLEN((char *)sender_ptr);
                    SCI_MEMCPY((char *)g_ota_config_ptr->ota_config_info[count].sender, (char *)sender_ptr, MIN(OTA_CONFIG_SENDER_MAX_LEN, sender_sr_len));
                }
                g_ota_config_ptr->ota_config_info[count].time = MMIAPICOM_GetCurTime();                
                g_ota_config_ptr->ota_config_info[count].index = ++max_index;
                g_ota_config_ptr->ota_config_info[count].is_unload = TRUE;
                g_ota_config_ptr->ota_config_info[count].cur_select = 0; 
                g_ota_config_ptr->ota_config_info[count].dual_sys = dual_sys;
                MMIOTA_InsertOTAToSms(count);
                MAIN_SetMsgState(TRUE);
                break;
            }
        }
        
    }
    else if (count <= OTA_CONFIG_INFO_NUM_MAX)
    {   
        if (g_ota_config_ptr->ota_config_info[min_index_count].is_locked)//锁定的ota不能够替换
        {
            return TRUE;
        }            
        MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_OTA,min_index_count);
        //连接设置处理
        temp_setting_ptr = mmi_setting_ptr;
        temp_config_setting_ptr = (MMIOTA_SETTING_T* )&(g_ota_config_ptr->ota_config_info[min_index_count].ota_setting_ptr);
        SCI_MEMCPY(temp_config_setting_ptr, (char *)temp_setting_ptr,setting_size); 
        if (PNULL != sender_ptr)
        {
            sender_sr_len = SCI_STRLEN((char *)sender_ptr);
            SCI_MEMCPY((char *)g_ota_config_ptr->ota_config_info[min_index_count].sender, (char *)sender_ptr, MIN(OTA_CONFIG_SENDER_MAX_LEN, sender_sr_len));
        }
        g_ota_config_ptr->ota_config_info[min_index_count].time = MMIAPICOM_GetCurTime();
        g_ota_config_ptr->ota_config_info[min_index_count].index = ++max_index;
        g_ota_config_ptr->ota_config_info[min_index_count].is_unload = TRUE;
        g_ota_config_ptr->ota_config_info[min_index_count].cur_select = 0;
        g_ota_config_ptr->ota_config_info[min_index_count].dual_sys = dual_sys;
        MMIOTA_InsertOTAToSms(min_index_count);
        MAIN_SetMsgState(TRUE);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIOTA_otapush error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_932_112_2_18_2_42_47_24,(uint8*)"");
        return FALSE;
    }
    //open alert win
    {
        uint32              time_out     = 0;
        MMI_WIN_PRIORITY_E  win_priority = WIN_LOWEST_LEVEL;
        MMI_WIN_ID_T		alert_win_id = MMICMSBRW_NEWPUSH_WIN_ID;
        MMI_STRING_T  prompt_str         = {0};

        //SCI_TRACE_LOW:"MMIOTA_otapush alert"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_942_112_2_18_2_42_47_25,(uint8*)"");
        MMI_GetLabelTextByLang(TXT_MMS_RECEIVE_CONFIG_PUSH, &prompt_str);
        if(IsForbitNewMsgWin())
        {
            win_priority = WIN_LOWEST_LEVEL;
        }
        else
        {
            win_priority = WIN_ONE_LEVEL;
        }
        MMIDEFAULT_SetBackLight(TRUE);
#ifdef MMIWIDGET_SUPPORT 
        if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
        {
           MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_NEWSMS_ID, FALSE, TRUE);           
        }

        if(MMK_IsFocusWin(MAIN_IDLE_WIN_ID) && (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()))
        {
        }
        else
#endif
//         if (   MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle()
//             || MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle()
//             || WIN_ONE_LEVEL == win_priority)
        {
            if(MMIAPISMS_IsInSMSWin()
               || MMIAPIMMS_IsInMMSWin()
               || MMIAPIOTA_IsInOTAWin())
            {
                MMIPUB_OpenAlertWinByTextPtr(&time_out, &prompt_str, PNULL, IMAGE_PUBWIN_NEWMSG, &alert_win_id, &win_priority, MMIPUB_SOFTKEY_ONE, (MMIPUB_HANDLE_FUNC)MMIOTA_HandleWapPushNewMsgWin);
            }
            else
            {
                MMIPUB_OpenAlertWinByTextPtr(&time_out, &prompt_str, PNULL, IMAGE_PUBWIN_NEWMSG, &alert_win_id, &win_priority, MMIPUB_SOFTKEY_CUSTOMER, (MMIPUB_HANDLE_FUNC)MMIOTA_HandleWapPushNewMsgWin);
            }
            MMIPUB_SetWinTitleTextId(MMICMSBRW_NEWPUSH_WIN_ID, TXT_COMMON_PROMPT, FALSE);
        }
        
        MMIAPIMMS_PlayMMSSound(dual_sys);
/*wei.ren cr124504 add*/        
#ifdef PDA_UI_DROPDOWN_WIN
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_PUSH);
#else
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_MESSAGE);
#endif
#endif
/*wei.ren cr124504 add*/ 
    }
    MMIOTA_StoreOTAPUSH();    
    return TRUE;
}

/*****************************************************************************/
//  Description :inbox enter ota
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_OTAEnter(uint16 id)
{
    uint16 i                         = 0;

    if(id >= MMICMSBRW_MAX_ITEM_NUM || PNULL == g_ota_config_ptr)
    {
        return FALSE;
    }
    MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID); 
    if (g_ota_config_ptr->ota_config_info[id].is_unload)
    {
        MMK_CloseWin(MMICMSBRW_NEWPUSH_WIN_ID);
    }
#ifdef MMI_PDA_SUPPORT
    if (g_ota_config_ptr->ota_config_info[id].is_unload)
    {
        MMIOTA_OTAUpdateState(id);
    }
#endif
    for (i = 0; i < OTA_CONFIG_INFO_NUM_MAX; i++)
    {
        if(g_ota_config_ptr->ota_config_info[i].index != 0)
        {
            g_ota_config_ptr->ota_config_info[i].cur_select = 0;
        }
    } 
    g_ota_config_ptr->ota_config_info[id].cur_select = 1;    
    MMIAPIOTA_EnterconfigInfoWinMsg();

    return TRUE;    
}

/*****************************************************************************/
//  Description :inbox enter ota and update the state to be read to have read
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_OTAUpdateState(uint16 id)
{
    MMISMS_ORDER_INFO_T  current_mms = {0};
    wchar  temp_buf[MMIOTA_BOOKMARK_NAME_MAX_LEN + 1] = {0};
    uint8  name_len                  = 0;   
    uint16 name_wstr_len              = 0;
    wchar *name_wstr                 = PNULL;
    uint16  sender_str_len = 0;
    
    //SCI_TRACE_LOW:"MMIOTA_OTAUpdateState,id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1042_112_2_18_2_42_47_26,(uint8*)"d",id);
    if (PNULL == g_ota_config_ptr)
    {
        return FALSE;
    }

    g_ota_config_ptr->ota_config_info[id].is_unload = FALSE;

    current_mms.mo_mt_type = MMISMS_MT_HAVE_READ;
    sender_str_len = SCI_STRLEN((char *)g_ota_config_ptr->ota_config_info[id].sender);
    SCI_MEMCPY((char *)current_mms.sender, (char *)g_ota_config_ptr->ota_config_info[id].sender, sender_str_len);
    current_mms.dual_sys = g_ota_config_ptr->ota_config_info[id].dual_sys;
    current_mms.record_id = id;        
    current_mms.time = g_ota_config_ptr->ota_config_info[id].time;
    current_mms.is_lock=g_ota_config_ptr->ota_config_info[id].is_locked;
    current_mms.folder_type = g_ota_config_ptr->ota_config_info[id].folder_type;
    current_mms.msg_type = MMISMS_TYPE_WAP_OTA;

    if (g_ota_config_ptr->ota_config_info[id].ota_setting_ptr.setting_type == MMIOTA_BRW_BOOKMARK)
    {
        name_len = g_ota_config_ptr->ota_config_info[id].ota_setting_ptr.bookmark.bookmark_name_len;
        name_wstr = (wchar *)SCI_ALLOCA((name_len + 1) * sizeof(wchar));
        if (PNULL == name_wstr)
        {
            return FALSE;
        }
        SCI_MEMSET(name_wstr, 0, ((name_len + 1) * sizeof(wchar)));
        name_wstr_len = GUI_UTF8ToWstr(name_wstr, name_len, (uint8 *)g_ota_config_ptr->ota_config_info[id].ota_setting_ptr.bookmark.bookmark_name, name_len);
        
        name_wstr_len = MIN(name_wstr_len, MMIOTA_BOOKMARK_NAME_MAX_LEN);
        MMI_WSTRNCPY(temp_buf, MMIOTA_BOOKMARK_NAME_MAX_LEN, name_wstr, MMIOTA_BOOKMARK_NAME_MAX_LEN, name_wstr_len);
        SCI_FREE(name_wstr);
        name_wstr = PNULL;
    }
    else
    {
        name_len = g_ota_config_ptr->ota_config_info[id].ota_setting_ptr.name_len;
        MMIAPICOM_StrToWstr((uint8 *)g_ota_config_ptr->ota_config_info[id].ota_setting_ptr.name,temp_buf);
    } 
    current_mms.display_content.content_len = MMIAPICOM_Wstrlen(temp_buf);
    current_mms.display_content.content_len = MIN( MMIMESSAGE_DIAPLAY_LEN, current_mms.display_content.content_len );    
    MMI_WSTRNCPY( current_mms.display_content.content, MMIMESSAGE_DIAPLAY_LEN, temp_buf, 50, current_mms.display_content.content_len );    
    
    MMIAPISMS_UpdateMsgStatusInOrder(&current_mms);
    MMIOTA_StoreOTAPUSH();
    return TRUE;
}

/*****************************************************************************/
//  Description :insert ota into sms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_InsertOTAToSms(uint16 index)
{
    MMISMS_ORDER_INFO_T  current_mms = {0};
    wchar temp_buf [MMIOTA_BOOKMARK_NAME_MAX_LEN + 1]              = {0};
    uint8 name_len                   = 0;
    wchar *name_wstr                 = PNULL;
    wchar name_wstr_len              = 0;
    uint16  sender_str_len = 0;
    
    //SCI_TRACE_LOW:"MMIOTA_InsertOTAToSms,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1103_112_2_18_2_42_47_27,(uint8*)"d",index);
    if(index < OTA_CONFIG_INFO_NUM_MAX && PNULL != g_ota_config_ptr)
    {
        if(g_ota_config_ptr->ota_config_info[index].index!=0)
        {
            current_mms.dual_sys = g_ota_config_ptr->ota_config_info[index].dual_sys;
            current_mms.record_id = index;
            if (g_ota_config_ptr->ota_config_info[index].is_unload == TRUE)
            {
                current_mms.mo_mt_type = MMISMS_MT_TO_BE_READ; 
            }
            else
            {
                current_mms.mo_mt_type = MMISMS_MT_HAVE_READ;
            }                       
            current_mms.time = g_ota_config_ptr->ota_config_info[index].time;
            
            //            if(g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.setting_type == MMIOTA_MMS_SETTING)
            //            {
            //                current_mms.msg_type = MMISMS_TYPE_MMS_OTA;
            //            }
            //            else 
            {
                current_mms.msg_type = MMISMS_TYPE_WAP_OTA;
            }
            sender_str_len = SCI_STRLEN((char *)g_ota_config_ptr->ota_config_info[index].sender);
            SCI_MEMCPY((char *)current_mms.sender, (char *)g_ota_config_ptr->ota_config_info[index].sender, sender_str_len);
            
            if (g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.setting_type == MMIOTA_BRW_BOOKMARK)
            {
                name_len = g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.bookmark.bookmark_name_len;
                name_wstr = (wchar *)SCI_ALLOCA((name_len + 1) * sizeof(wchar));
                if (PNULL == name_wstr)
                {
                    return;
                }
                SCI_MEMSET(name_wstr, 0, ((name_len + 1) * sizeof(wchar)));
                name_wstr_len = GUI_UTF8ToWstr(name_wstr, name_len, (uint8 *)g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.bookmark.bookmark_name, name_len);

                name_wstr_len = MIN(name_wstr_len, MMIOTA_BOOKMARK_NAME_MAX_LEN);
                MMI_WSTRNCPY(temp_buf, MMIOTA_BOOKMARK_NAME_MAX_LEN, name_wstr, MMIOTA_BOOKMARK_NAME_MAX_LEN, name_wstr_len);
                SCI_FREE(name_wstr);
                name_wstr = PNULL;
                //name_len = g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.bookmark.bookmark_name_len;
                //MMIAPICOM_StrToWstr((char *)g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.bookmark.bookmark_name,temp_buf);
            }
            else
            {
                name_len = g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.name_len;
                MMIAPICOM_StrToWstr((uint8 *)g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.name,temp_buf);
            }
            current_mms.folder_type = g_ota_config_ptr->ota_config_info[index].folder_type;
            current_mms.display_content.content_len = name_len;
            current_mms.display_content.content_len = MIN( MMIMESSAGE_DIAPLAY_LEN, current_mms.display_content.content_len );            
            MMI_WSTRNCPY( current_mms.display_content.content, MMIMESSAGE_DIAPLAY_LEN, temp_buf, MMIOTA_SETTING_MAX_NAME_LEN, current_mms.display_content.content_len );
            
            current_mms.is_lock=g_ota_config_ptr->ota_config_info[index].is_locked;
            if (0 != current_mms.time && MMISMS_TYPE_WAP_OTA == current_mms.msg_type)
            {
                MMIAPISMS_InsertOneMsgToOrder(&current_mms);
            }
        }
    }    
}

/*****************************************************************************/
//  Description :store ota virtual file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_StoreOTAPUSH(void)
{
    // FILE_HANDLE fp                         = 0;
    // uint8        pathname[256]             = {0};
    uint16 i                               = 0;
    uint16 ota_item_total_num              = 0;
    uint16 ota_total_num                   = 0;
    uint32 config_len                      = 0;
    uint32 setting_len                     = 0;
    uint32 len_local                       = 0;
    uint8*  tempbuf                        = NULL;
    uint32   filelen                       = 0;
    SFS_ERROR_E   efs_error                = SFS_NO_ERROR;    
    MMIFILE_HANDLE     cur_hefs            = SFS_NO_ERROR;
    uint32    transmit                     = 0;    
    MMIOTA_SETTING_T *temp_setting_ptr     = PNULL;    
    wchar ucs2_name[OTA_UCS2_NAME_MAX_LEN] = {0};
    int  ucs2_name_len                     = 0;
    MMIFILE_ERROR_E ffs_error              = SFS_NO_ERROR;
    uint8    count                         = 0;
    
    //SCI_TRACE_LOW:"MMIOTA_StoreOTAPUSH  IN"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1193_112_2_18_2_42_47_28,(uint8*)"");
    ota_item_total_num = MMIOTA_GetOTAItemNum();
    ota_total_num = MMIAPIOTA_GetOTATotalNum();
    setting_len = sizeof(MMIOTA_SETTING_T);
    config_len = sizeof(MMIOTA_CONFIG_INFO_T);

    if (ota_item_total_num < ota_total_num)
    {
        return;
    }
    filelen = ota_total_num * config_len + (ota_item_total_num - ota_total_num) * setting_len;
    if (filelen > 0 && PNULL != g_ota_config_ptr)//有ota文件数据
    {
        MMIOTA_FilecCreateDir((char *)MMIMMS_MMS_DIR);        
        cur_hefs = MMIOTA_FilecCreate((uint8*)MANA_FLASH_OTAFILE, SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE);/*lint !e655*/
        if (cur_hefs == SFS_INVALID_HANDLE)
        {
            return;
        }

        tempbuf = SCI_ALLOCAZ(filelen + setting_len + config_len);
        if (PNULL == tempbuf)
        {
            //SCI_TRACE_LOW:"fnh param_save_ptr malloc fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1216_112_2_18_2_42_47_29,(uint8*)"");
            MMIOTA_FileClose(cur_hefs);
            cur_hefs = SFS_INVALID_HANDLE;
            return;
        }
        for (i = 0; i < OTA_CONFIG_INFO_NUM_MAX; i++)
        {
            if (0 != g_ota_config_ptr->ota_config_info[i].index 
                && 0 < ota_total_num 
                && 0 != g_ota_config_ptr->ota_config_info[i].time
                && len_local < filelen)
            {
                SCI_MEMCPY(tempbuf + len_local, &g_ota_config_ptr->ota_config_info[i], config_len);
                ota_total_num--;
                len_local = len_local + config_len;
                temp_setting_ptr = &g_ota_config_ptr->ota_config_info[i].ota_setting_ptr;
                while (PNULL != temp_setting_ptr->next 
                    && 0 < ota_item_total_num
                    && len_local < filelen)
                {
                    SCI_MEMCPY(tempbuf + len_local, temp_setting_ptr->next, setting_len);
                    ota_item_total_num--;
                    len_local = len_local + setting_len;
                    temp_setting_ptr = temp_setting_ptr->next;
                }
            }
        }

        efs_error = MMIOTA_FileWrite(cur_hefs, tempbuf, filelen,  &transmit, NULL);
        //SCI_TRACE_LOW:"fnh MMIMMS_SaveOTAPASHFile efs_error is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1245_112_2_18_2_42_47_30,(uint8*)"d",efs_error);
        MMIOTA_FileClose(cur_hefs);
        cur_hefs = SFS_INVALID_HANDLE;
        SCI_FREE(tempbuf);
        tempbuf = PNULL;
    }
    else
    {
        if(MMIOTA_CheckFileExist((const uint8*)MANA_FLASH_OTAFILE) == TRUE)
        {            
            ucs2_name_len = GUI_GBToWstr(ucs2_name, (uint8 *)MANA_FLASH_OTAFILE,(uint16)SCI_STRLEN((char*)MANA_FLASH_OTAFILE));
            ffs_error = MMIOTA_FileDelete(ucs2_name, PNULL);
            //SCI_TRACE_LOW:"VC:DeleteVideoFile FFS_AYS_Delete = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1257_112_2_18_2_42_47_31,(uint8*)"d", efs_error);
        }
        if (PNULL != g_ota_config_ptr)
        {
            for (count = 0; count < OTA_CONFIG_INFO_NUM_MAX; count++)
            {
                if (g_ota_config_ptr->ota_config_info[count].index != 0)
                {
                    MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_OTA,count);
                    g_ota_config_ptr->ota_config_info[count].index = 0;
                }
            }
            SCI_FREE(g_ota_config_ptr);
            g_ota_config_ptr = PNULL;
        }
        //SCI_TRACE_LOW:"VC: MMIVC_CallBackDeleteMMSFile run"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1272_112_2_18_2_42_47_32,(uint8*)"");
        MMIAPICOM_TriggerMMITask();
    }     
    //SCI_TRACE_LOW:"MMIOTA_StoreOTAPUSH OUT -----------"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1275_112_2_18_2_42_47_33,(uint8*)"");
}

/*****************************************************************************/
//  Description :delete ota push
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_DeleteOTAPUSH(int16 del_index)
{
    if (PNULL == g_ota_config_ptr)
    {
        return;
    }
    if (   0 == del_index 
        || 0 == g_ota_config_ptr->ota_config_info[del_index].time
        || MMISMS_TYPE_WAP_OTA == g_ota_config_ptr->ota_config_info[del_index].mo_mt_type)
    {
        SCI_MEMSET(&g_ota_config_ptr->ota_config_info[del_index], 0, sizeof(MMIOTA_CONFIG_INFO_T));
        MMIOTA_StoreOTAPUSH();
        return;
    }

    g_ota_config_ptr->ota_config_info[del_index].index = 0;
    g_ota_config_ptr->ota_config_info[del_index].cur_select = 0;
    SCI_MEMSET(&g_ota_config_ptr->ota_config_info[del_index], 0, sizeof(MMIOTA_CONFIG_INFO_T));
    MMIOTA_StoreOTAPUSH();
}

/*****************************************************************************/
//  Description :set locked flag
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_SetOTAPUSHLockFlag(uint32 record_id,BOOLEAN is_locked)
{
    if (PNULL == g_ota_config_ptr)
    {
        return;
    }
    if(record_id < MMICMSBRW_MAX_ITEM_NUM)
    {
        g_ota_config_ptr->ota_config_info[record_id].is_locked = is_locked;        
    }    
}

/*****************************************************************************/
//  Description :update ota file info
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_UpdateOTAFileInfo(void)
{
    MMIOTA_StoreOTAPUSH();
}

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
//    Description : Select Sim callback
//    Global resource dependence : 
//    Author:wancan.you
//    Note: sam.hua@dec 27
/*****************************************************************************/
LOCAL MMI_RESULT_E  OTASelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E recode  = MMI_RESULT_TRUE;
    uint32 cur_sel = (uint32)MMK_GetWinAddDataPtr(MMK_GetFocusWinId());
    if (isReturnOK)
    {
        if (MMIOTA_SaveConfig((MN_DUAL_SYS_E)dual_sys, cur_sel))
        {
            if (PNULL != g_ota_config_ptr && g_ota_config_ptr->ota_config_info[cur_sel].ota_setting_ptr.setting_type != MMIOTA_BRW_BOOKMARK)
            {
                MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
            }                        
        }
        else
        {
            MMIPUB_OpenAlertWarningWin(TXT_MMS_NET_SETTING_FULL);
        }
    }
    return recode;
}
#endif

/*****************************************************************************/
//  Description :save ota config to link list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_SaveConfig(MN_DUAL_SYS_E dual_sys, uint16 cur_sel)
{
    MMIOTA_SETTING_T *temp_setting_ptr = PNULL;
    BOOLEAN         ret=TRUE;

    if (PNULL == g_ota_config_ptr)
    {
        return FALSE;
    }
    temp_setting_ptr = &g_ota_config_ptr->ota_config_info[cur_sel].ota_setting_ptr;
    
    while (temp_setting_ptr != PNULL)
    {
        if (temp_setting_ptr->setting_type != MMIOTA_BRW_BOOKMARK)
        {
            if (!SaveOTASettingValue(dual_sys, temp_setting_ptr))
            {
                ret = FALSE;
                break;
            }

            //MMIPUB_OpenQueryWinByTextId(TXT_MMS_IF_DEFAULT_SETTING,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            if (MMIOTA_MMS_SETTING == temp_setting_ptr->setting_type)
            {
#ifdef MMS_SUPPORT
                MMIAPIMMS_Setting_SetNetSettingIndex(dual_sys, g_defaut_now_saving_config);
#endif
            }
            else// if (MMIOTA_BRW_SETTING == ota_setting_ptr->setting_type)
            {
#if defined(BROWSER_SUPPORT_DORADO)
                MMIAPIBROWSER_SetNetSettingIndexEx(dual_sys, g_defaut_now_saving_config, MMIBROWSER_TYPE_DORADO);
#elif defined(OPERA_MINI_SUPPORT)
                MMIAPIBROWSER_SetNetSettingIndexEx(dual_sys, g_defaut_now_saving_config, MMIBROWSER_TYPE_OPERA_MINI);
#elif defined(BROWSER_SUPPORT_NETFRONT)
                MMIAPIBROWSER_SetNetSettingIndexEx(dual_sys, g_defaut_now_saving_config, MMIBROWSER_TYPE_NF);
#endif
            }
        }
        else
        {
            if (!SaveOTABookmarkValue(temp_setting_ptr))
            {
                ret = FALSE;
                break;
            }
        }
        
        temp_setting_ptr = temp_setting_ptr->next;
    }

    return ret;
}

/*****************************************************************************/
//  Description :save ota config setting value
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  SaveOTASettingValue(MN_DUAL_SYS_E dual_sys, MMIOTA_SETTING_T *ota_setting_ptr)
{
    // uint8      j                                         = 0;
    BOOLEAN   ret                                        = TRUE;
    MMICONNECTION_LINKSETTING_DETAIL_T* link_setting_ptr = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T link_setting_info = {0};
    uint8 index                                          = 0;
    uint16 temp_len                                      = 0;
    wchar temp_name[MMICONNECTION_MAX_LINKMODENAME_LEN + 1] = {0};

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"SaveOTASettingValue dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1412_112_2_18_2_42_48_34,(uint8*)"d", dual_sys);

        return FALSE;
    }
    
    g_defaut_now_saving_config = 0;    

    if (PNULL == ota_setting_ptr)
    {
        return FALSE;
    }
    temp_len = MIN(ota_setting_ptr->name_len, MMICONNECTION_MAX_LINKMODENAME_LEN);
    if (0 < temp_len)
    {
        MMI_STRNTOWSTR(temp_name, MMICONNECTION_MAX_LINKMODENAME_LEN, ota_setting_ptr->name, temp_len, temp_len);
    }    
    for (index = 0; index < MMIAPICONNECTION_GetLinkSettingNum(dual_sys); index++)
    {
        link_setting_ptr = MMIAPICONNECTION_GetLinkSettingItemByIndex(dual_sys,index);
        if( PNULL != link_setting_ptr &&
            (0 < ota_setting_ptr->name_len && ota_setting_ptr->name_len ==link_setting_ptr->name_len  && !MMIAPICOM_Wstrncmp(link_setting_ptr->name, temp_name, MMIAPICOM_Wstrlen(temp_name))) &&
            (0 < ota_setting_ptr->apn_len && !stricmp((const char*)link_setting_ptr->apn, (char *)ota_setting_ptr->apn))&&
            (0 < ota_setting_ptr->gateway_len && !strcmp((const char*)link_setting_ptr->gateway, (char *)ota_setting_ptr->gateway))&&
            (0 < ota_setting_ptr->homepage_len && !strcmp((const char*)link_setting_ptr->homepage, (char *)ota_setting_ptr->homepage))&&
            (link_setting_ptr->port == ota_setting_ptr->port)
            )
        {
            g_defaut_now_saving_config = index;
            break;
        }
    }    

    SCI_TRACE_LOW("SaveOTASettingValue index = %d, GetLinkSettingNum = %d", index, MMIAPICONNECTION_GetLinkSettingNum(dual_sys));
    if(index == MMIAPICONNECTION_GetLinkSettingNum(dual_sys))
    {
        if(MMIAPICONNECTION_GetLinkSettingNum(dual_sys) >= MMICONNECTION_MAX_SETTING_NUM)
        {
            ret = FALSE;
        }
        else
        {
            PHONE_PLMN_NAME_E hplmn = PLMN_INVALID;

            if (0 < ota_setting_ptr->homepage_len)
            {
                temp_len = SCI_STRLEN((char *)ota_setting_ptr->homepage);
                link_setting_info.homepage_len = MIN(temp_len, MMICONNECTION_HOMEPAGE_MAX_URL_LEN);
                SCI_MEMCPY((char *)link_setting_info.homepage,(char *)ota_setting_ptr->homepage, link_setting_info.homepage_len);
            }            
            
            if (0 < ota_setting_ptr->gateway_len)
            {
                temp_len = SCI_STRLEN((char *)ota_setting_ptr->gateway);
                link_setting_info.gateway_len = MIN(temp_len, MMICONNECTION_MAX_GATEWAY_LEN);
                link_setting_info.gateway_len = MIN(temp_len, MMIOTA_SETTING_MAX_GATEWAY_LEN);
                SCI_MEMCPY((char *)link_setting_info.gateway,(char *)ota_setting_ptr->gateway, link_setting_info.gateway_len); 
            }            
            
            if (ota_setting_ptr->port == 9201 ||
                ota_setting_ptr->port == 9202 ||
                ota_setting_ptr->port == 9203 ||
                ota_setting_ptr->port == 9204)
            {
                link_setting_info.access_type= 0;
            }
            else
            {
                link_setting_info.access_type= 1;
            }
            
            link_setting_info.auth_type = ota_setting_ptr->auth_type;
            
            if (0 > ota_setting_ptr->port || 65535 < ota_setting_ptr->port)
            {
                link_setting_info.port = 80;
            }
            else
            {
                link_setting_info.port = (uint16)ota_setting_ptr->port;
            }
            if (0 < ota_setting_ptr->username_len)
            {
                temp_len = SCI_STRLEN((char *)ota_setting_ptr->username);
                link_setting_info.username_len = MIN(temp_len, MMICONNECTION_MAX_USERNAME_LEN);
                SCI_MEMCPY((char *)link_setting_info.username,(char *)ota_setting_ptr->username, link_setting_info.username_len);                
            }            
            
            if (0 < ota_setting_ptr->name_len)
            {
                link_setting_info.name_len = MIN(ota_setting_ptr->name_len, MMICONNECTION_MAX_LINKMODENAME_LEN);
                MMI_STRNTOWSTR(link_setting_info.name, ota_setting_ptr->name_len, (uint8 *)ota_setting_ptr->name, ota_setting_ptr->name_len,  link_setting_info.name_len);                
            }            
            if (0 < ota_setting_ptr->password_len)
            {
                temp_len = SCI_STRLEN((char *)ota_setting_ptr->password);
                link_setting_info.password_len = MIN(temp_len, MMICONNECTION_MAX_PASSWORD_LEN);
                link_setting_info.password_len = MIN(temp_len, MMIOTA_SETTING_MAX_PASSWORD_LEN);          
                SCI_MEMCPY((char *)link_setting_info.password,(char *)ota_setting_ptr->password, link_setting_info.password_len);/* parasoft-suppress BD-RES-INVFREE "有判断检查" */                
            }            
            if (0 < ota_setting_ptr->apn_len)
            {
                temp_len = SCI_STRLEN((char *)ota_setting_ptr->apn);
                link_setting_info.apn_len = MIN(temp_len, MMICONNECTION_MAX_APN_LEN);
                SCI_MEMCPY((char *)link_setting_info.apn,(char *)ota_setting_ptr->apn, link_setting_info.apn_len);                
            }           
            if (0 < ota_setting_ptr->dnsaddress_len)
            {
                SCI_MEMCPY((char *)link_setting_info.dns,(char *)ota_setting_ptr->dnsaddress, MIN(ota_setting_ptr->dnsaddress_len, MMICONNECTION_MAX_DNS_LEN));
            }            

            MMIAPIPHONE_GetFullPlmn(dual_sys, PNULL, &hplmn);

            link_setting_info.is_customed_detail = FALSE;
            link_setting_info.is_default_unchange = FALSE;
            link_setting_info.hplmn = hplmn;

            if (MMIOTA_MMS_SETTING == ota_setting_ptr->setting_type)
            {
                link_setting_info.account_type = MMICONNECTION_ACCOUNT_MMS;
            }
            else// if (MMIOTA_BRW_SETTING == ota_setting_ptr->setting_type)
            {
                link_setting_info.account_type = MMICONNECTION_ACCOUNT_BROWSER;
            }

            MMIAPICONNECTION_AddNewItem(dual_sys,&link_setting_info);
           
            g_defaut_now_saving_config = MMIAPICONNECTION_GetLinkSettingNum(dual_sys) - 1;
        }
        
    }//if(j == g_cmsbrw_setting_info.settinglink_num)
    return ret;
}

/*****************************************************************************/
//  Description :save ota config bookmark value
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  SaveOTABookmarkValue(MMIOTA_SETTING_T *ota_setting_ptr)
{
    BOOLEAN         ret = TRUE;

#ifdef BROWSER_SUPPORT
    {
        MMIBROWSER_ENTRY_PARAM entry_param = {0};/*lint !e64*/
        
        entry_param.type = MMIBROWSER_ADD_BOOKMARK;
        entry_param.dual_sys = MN_DUAL_SYS_MAX;
        entry_param.url_ptr = (char *)ota_setting_ptr->bookmark.bookmark_url;
        entry_param.url_title_ptr = (char *)ota_setting_ptr->bookmark.bookmark_name;
        entry_param.user_agent_ptr = PNULL;
        MMIAPIBROWSER_Entry(&entry_param);
    }                
#endif

    return ret;
}

/*****************************************************************************/
//  Description :open ota read win
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_EnterconfigInfoWinMsg(void)
{
    return MMK_CreateWin((uint32 *)MMIOTA_CONFIG_INFO_DETAIL_WIN,(ADD_DATA)PNULL);
}

/*****************************************************************************/
//  Description :out save push file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_SavePushFileInfo(void)
{
    if (PNULL != g_push_info_ptr)
    {
        MMIOTA_WriteWapPushFile(BEGIN_WIN_ID,
            MMICMSBRW_WAP_PUSHMSG_FILE_NAME,
            (uint8*)g_push_info_ptr,
            sizeof(MMICMSBRW_ITEM_T),
            VIRTUAL_WIN_ID,
            FALSE,
            CMSBRW_FS_PUSHMSG_TYPE);
    }    
}

/*****************************************************************************/
//  Description : config info detail  window handle function
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOTAConfigInfoDetailWinMsg(
                                                    MMIOTA_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;    
    uint16 i = 0;
    int16 cur_sel = -1;
    BOOLEAN mms_ota_yes=FALSE;
    wchar space_ch = ' ';    
    MMI_STRING_T empty_string = {0};   
    
    if (PNULL == g_ota_config_ptr)
    {
        return err_code;
    }
    empty_string.wstr_len = 1;
    empty_string.wstr_ptr = &space_ch;//(uint8*)" ";
    
    for (i = 0; i < OTA_CONFIG_INFO_NUM_MAX; i++)
    {
        if(g_ota_config_ptr->ota_config_info[i].index !=0 &&(g_ota_config_ptr->ota_config_info[i].cur_select == 1))
        {
            cur_sel = i;
            break;
        }
    }    
    //SCI_TRACE_LOW:"HandleMMSConfigInfoDetailWinMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1614_112_2_18_2_42_48_35,(uint8*)"");
    
    switch(msg_id)
    {
    case MSG_FULL_PAINT:
        {            
            //set sofkey
            GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);
        }
        break;
        
    case MSG_OPEN_WINDOW:
        /*changed by minghu for detail 090205*/  
#ifndef MMI_PDA_SUPPORT
        GUIWIN_SetTitleRollArrow(win_id, TRUE);
#endif
        if(i<OTA_CONFIG_INFO_NUM_MAX)
        {
            mms_ota_yes = MMIOTA_IsMMSOTA(cur_sel);
            //SCI_TRACE_LOW:"cur_sel=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_1633_112_2_18_2_42_48_36,(uint8*)"d",cur_sel);
            if (cur_sel != -1)
            {
                MMSConfigDetailEntry(MMIOTA_CONFIG_INFO_DETAIL_CTRL_ID, &(g_ota_config_ptr->ota_config_info[cur_sel]), mms_ota_yes);
            }//if (cur_sel != -1) 
        }
        
        GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, FALSE);

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        
        //set active control        
        MMK_SetAtvCtrl(win_id, MMIOTA_CONFIG_INFO_DETAIL_CTRL_ID);        
        break;    
        
    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_CTL_TITLE_MOVE_NEXT:
    case MSG_CTL_TITLE_MOVE_PREV:
        if (MSG_CTL_TITLE_MOVE_NEXT == msg_id)
        {
            msg_id = MSG_APP_RIGHT;
        }
        else if (MSG_CTL_TITLE_MOVE_PREV == msg_id)
        {
            msg_id = MSG_APP_LEFT;
        }
        if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
        {           
            MMIAPISMS_HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
        }
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:            
    case MSG_APP_MENU:
        {          
            if( i<OTA_CONFIG_INFO_NUM_MAX && (cur_sel != -1))
            {   
                //opt menu
                MMK_CreateWin((uint32 *)MMIOTA_CONFIG_SET_OPTION_TAB,(ADD_DATA)i);               
            }//if(i<OTA_CONFIG_INFO_NUM_MAX)                
        }
        break;
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
   case MSG_PROMPTWIN_OK: 
       MMIPUB_CloseQuerytWin(PNULL);
       if (-1 != cur_sel)
       {
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
           if (MMISMS_FOLDER_NORMAL == g_ota_config_ptr->ota_config_info[cur_sel].folder_type)
           {
               MMIAPIMMS_MoveSecurityOTA(cur_sel, TRUE);
           } 
           else
           {
               MMIAPIMMS_MoveSecurityOTA(cur_sel, FALSE);
           }
#endif
           if (MMK_IsOpenWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID))
           {
               MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);
           }
           MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
       }
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);//here should close the win and open the waitwin for delete.
        break;     
    case MSG_CLOSE_WINDOW:
        if (MMK_IsOpenWin(MMIOTA_CONFIG_SET_MENU_WIN_ID))
        {
            MMK_CloseWin(MMIOTA_CONFIG_SET_MENU_WIN_ID);
        }
#ifndef MMI_PDA_SUPPORT
        if (cur_sel != -1 && g_ota_config_ptr->ota_config_info[cur_sel].is_unload)
        {
            MMIOTA_OTAUpdateState(cur_sel);
        }
#endif
        break;
        
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code;    
}

/*****************************************************************************/
//  Description : if it is mms ota
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_IsMMSOTA(int16 index)
{
    if (-1 == index || PNULL == g_ota_config_ptr)
    {
        return FALSE;
    }
    if(g_ota_config_ptr->ota_config_info[index].ota_setting_ptr.setting_type == MMIOTA_MMS_SETTING)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : show detail of ota config
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL void MMSConfigDetailEntry(MMI_CTRL_ID_T ctrl_id, MMIOTA_CONFIG_INFO_T *ota_item_ptr, BOOLEAN mms_ota_yes)
{
    MMIOTA_SETTING_T *temp_setting_ptr = &(ota_item_ptr->ota_setting_ptr);
    while (temp_setting_ptr != PNULL)
    {
        MMSSettingsDetailEntry(ctrl_id,temp_setting_ptr,mms_ota_yes);
        temp_setting_ptr = temp_setting_ptr->next;
    }       
}

/*****************************************************************************/
//  Description : show ota detail add items
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL void MMSSettingsDetailEntry(MMI_CTRL_ID_T ctrl_id, MMIOTA_SETTING_T *ota_setting_ptr, BOOLEAN mms_ota_yes)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    MMI_STRING_T         temp_str = {0};
    MMI_STRING_T     text_res_str = {0};
    wchar                 colon[] = {':', '\n', 0};
    MMI_STRING_T        colon_str = {0};
    MMI_TEXT_ID_T         text_id = TXT_NULL;    
    MMI_STRING_T		homepage  = {0};
    MMI_STRING_T        gateway   = {0};
    MMI_STRING_T        apn       = {0};
    MMI_STRING_T        name      = {0};
    uint16                  index = 0;     
    uint8                port[16] = {0};
    int32               temp_port = 0;
    wchar            port_str[10] = {0};
    int8                 port_len = 0;
    wchar           username_buf[MMIOTA_SETTING_MAX_USERNAME_LEN + 1] = {0};
    uint16           username_len = 0;
    wchar           password_buf[MMIOTA_SETTING_MAX_PASSWORD_LEN + 1] = {0};
    uint16           password_len = 0;
    uint8  name_len               = 0;   
    wchar name_wstr_len           = 0;
    wchar *name_wstr              = PNULL;
 
    colon_str.wstr_len = MMIAPICOM_Wstrlen(colon); 
    colon_str.wstr_ptr = colon;
    
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    
    //name  
    MMI_GetLabelTextByLang(TXT_COMMON_NAME, &text_res_str);
    if (ota_setting_ptr->setting_type == MMIOTA_BRW_BOOKMARK)
    {
        name_len = ota_setting_ptr->bookmark.bookmark_name_len;
    }
    else
    {
        Detail_FormConfigInfo(&name,(uint8 *)ota_setting_ptr->name,MMINAME,mms_ota_yes);
        name_len = name.wstr_len;
    }
    temp_str.wstr_len = text_res_str.wstr_len + colon_str.wstr_len + name_len;
    temp_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((temp_str.wstr_len + 1) * sizeof(wchar));
    if (PNULL == temp_str.wstr_ptr)
    {
        return;
    }
    MMIAPICOM_Wstrncpy(temp_str.wstr_ptr, text_res_str.wstr_ptr, text_res_str.wstr_len);
    MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len);
    if (ota_setting_ptr->setting_type == MMIOTA_BRW_BOOKMARK)
    {
        name_wstr = (wchar *)SCI_ALLOCA((name_len + 1) * sizeof(wchar));
        if (PNULL == name_wstr)
        {
            SCI_Free(temp_str.wstr_ptr);
            temp_str.wstr_ptr = PNULL;
            temp_str.wstr_len = 0;
            return;
        }
        SCI_MEMSET(name_wstr, 0, ((name_len + 1) * sizeof(wchar)));
        name_wstr_len = GUI_UTF8ToWstr(name_wstr, name_len, (uint8 *)ota_setting_ptr->bookmark.bookmark_name, name_len);
        MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, name_wstr, name_wstr_len);
        SCI_FREE(name_wstr);
        name_wstr = PNULL; 
    }
    else
    {
        MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, name.wstr_ptr, name.wstr_len);
        if (PNULL != name.wstr_ptr)
        {
            SCI_FREE(name.wstr_ptr);
            name.wstr_ptr = PNULL;
            name.wstr_len = 0;
        }
    }
    item_data.text_data.buf.len = MMIAPICOM_Wstrlen(temp_str.wstr_ptr);//@MS00211656
    item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_Free(temp_str.wstr_ptr);
    temp_str.wstr_ptr = PNULL;
    temp_str.wstr_len = 0;    
    
    //homepage 
    if (mms_ota_yes)
    {
        text_id = TXT_MMS_SETTING_MMSC;
    }
    else
    {
        text_id = TXT_COMMON_HOMEPAGE;
    }
    MMI_GetLabelTextByLang(text_id, &text_res_str);
    if (ota_setting_ptr->setting_type == MMIOTA_BRW_BOOKMARK)
    {
        Detail_FormConfigInfo(&homepage,(uint8 *)ota_setting_ptr->bookmark.bookmark_url,MMIHOMEPAGE,mms_ota_yes);
    }
    else
    {
        Detail_FormConfigInfo(&homepage,(uint8 *)ota_setting_ptr->homepage,MMIHOMEPAGE,mms_ota_yes);
    }
    temp_str.wstr_len = text_res_str.wstr_len + colon_str.wstr_len + homepage.wstr_len;
    temp_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((temp_str.wstr_len + 1) * sizeof(wchar));
    if (PNULL == temp_str.wstr_ptr)
    {
        return;
    }
    MMIAPICOM_Wstrncpy(temp_str.wstr_ptr, text_res_str.wstr_ptr, text_res_str.wstr_len);
    MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len);
    MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, homepage.wstr_ptr, homepage.wstr_len);
    item_data.text_data.buf.len = temp_str.wstr_len;
    item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if (PNULL != homepage.wstr_ptr)
    {
        SCI_FREE(homepage.wstr_ptr);
        homepage.wstr_ptr = PNULL;
        homepage.wstr_len = 0;
    }
    SCI_Free(temp_str.wstr_ptr);
    temp_str.wstr_ptr = PNULL;
    temp_str.wstr_len = 0;   
    
    if (ota_setting_ptr->setting_type != MMIOTA_BRW_BOOKMARK)
    {
        //gatewag
        if (ota_setting_ptr->gateway_len > 0)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_GATEWAY, &text_res_str);
            Detail_FormConfigInfo(&gateway,(uint8 *)ota_setting_ptr->gateway,MMIGATEWAY,mms_ota_yes);
            temp_str.wstr_len = text_res_str.wstr_len + colon_str.wstr_len + gateway.wstr_len;
            temp_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((temp_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL == temp_str.wstr_ptr)
            {
                return;
            }
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr, text_res_str.wstr_ptr, text_res_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, gateway.wstr_ptr, gateway.wstr_len);
            item_data.text_data.buf.len = temp_str.wstr_len;
            item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
            if (PNULL != gateway.wstr_ptr)
            {
                SCI_FREE(gateway.wstr_ptr);
                gateway.wstr_ptr = PNULL;
                gateway.wstr_len = 0;
            }
            SCI_Free(temp_str.wstr_ptr);
            temp_str.wstr_ptr = PNULL;
            temp_str.wstr_len = 0; 
        }                 
        
        //port
        if (0 <=ota_setting_ptr->port)
        {
            MMI_GetLabelTextByLang(TXT_COMM_PORT, &text_res_str);
            temp_port = ota_setting_ptr->port;
            port_len = sprintf((char *)port,"%d",temp_port);
            MMIAPICOM_StrToWstr( (uint8 *)port, port_str);
            temp_str.wstr_len = text_res_str.wstr_len + colon_str.wstr_len + port_len;
            temp_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((temp_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL == temp_str.wstr_ptr)
            {
                return;
            }
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr, text_res_str.wstr_ptr, text_res_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, port_str, port_len);
            item_data.text_data.buf.len = temp_str.wstr_len;
            item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
            SCI_Free(temp_str.wstr_ptr);
            temp_str.wstr_ptr = PNULL;
            temp_str.wstr_len = 0;
        }        
        
        //username
        if (ota_setting_ptr->username_len > 0)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_USERNAME, &text_res_str);
            username_len = ota_setting_ptr->username_len;
            MMIAPICOM_StrToWstr( (uint8 *)ota_setting_ptr->username, username_buf);       
            temp_str.wstr_len = text_res_str.wstr_len + colon_str.wstr_len + username_len;
            temp_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((temp_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL == temp_str.wstr_ptr)
            {
                return;
            }
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr, text_res_str.wstr_ptr, text_res_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, username_buf, username_len);
      
            item_data.text_data.buf.len = temp_str.wstr_len;
            item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
            SCI_Free(temp_str.wstr_ptr);
            temp_str.wstr_ptr = PNULL;
            temp_str.wstr_len = 0; 
        }        
        
        //password
        if (ota_setting_ptr->password_len > 0)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_PASSWORD, &text_res_str);
            password_len = ota_setting_ptr->password_len;
            MMIAPICOM_StrToWstr( (uint8 *)ota_setting_ptr->password, password_buf);
       
            temp_str.wstr_len = text_res_str.wstr_len + colon_str.wstr_len + password_len;
            temp_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((temp_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL == temp_str.wstr_ptr)
            {
                return;
            }
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr, text_res_str.wstr_ptr, text_res_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, password_buf, password_len);
     
            item_data.text_data.buf.len = temp_str.wstr_len;
            item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
            SCI_Free(temp_str.wstr_ptr);
            temp_str.wstr_ptr = PNULL;
            temp_str.wstr_len = 0; 
        }                
        
        //apn
        if (ota_setting_ptr->apn_len > 0)
        {
            MMI_GetLabelTextByLang(TXT_COMMON_APN_COMN, &text_res_str);
            Detail_FormConfigInfo(&apn,(uint8 *)ota_setting_ptr->apn,MMIAPN,mms_ota_yes);
            temp_str.wstr_len = text_res_str.wstr_len + colon_str.wstr_len + apn.wstr_len;
            temp_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((temp_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL == temp_str.wstr_ptr)
            {
                return;
            }
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr, text_res_str.wstr_ptr, text_res_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, apn.wstr_ptr, apn.wstr_len);
            item_data.text_data.buf.len = temp_str.wstr_len;
            item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
            if (PNULL != apn.wstr_ptr)
            {
                SCI_FREE(apn.wstr_ptr);
                apn.wstr_ptr = PNULL;
                apn.wstr_len = 0;
            }
            SCI_Free(temp_str.wstr_ptr);
            temp_str.wstr_ptr = PNULL;
            temp_str.wstr_len = 0;
        }
        
        //DNS
        if (ota_setting_ptr->dnsaddress_len > 0)
        {
            MMI_GetLabelTextByLang(TXT_CONNECTION_DNS, &text_res_str);
            Detail_FormConfigInfo(&apn,(uint8 *)ota_setting_ptr->dnsaddress,MMIAPN,mms_ota_yes);
            temp_str.wstr_len = text_res_str.wstr_len + colon_str.wstr_len + apn.wstr_len;
            temp_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((temp_str.wstr_len + 1) * sizeof(wchar));
            if (PNULL == temp_str.wstr_ptr)
            {
                return;
            }
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr, text_res_str.wstr_ptr, text_res_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len);
            MMIAPICOM_Wstrncpy(temp_str.wstr_ptr + text_res_str.wstr_len + colon_str.wstr_len, apn.wstr_ptr, apn.wstr_len);
            item_data.text_data.buf.len = temp_str.wstr_len;
            item_data.text_data.buf.str_ptr = temp_str.wstr_ptr;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
            if (PNULL != apn.wstr_ptr)
            {
                SCI_FREE(apn.wstr_ptr);
                apn.wstr_ptr = PNULL;
                apn.wstr_len = 0;
            }
            SCI_Free(temp_str.wstr_ptr);
            temp_str.wstr_ptr = PNULL;
            temp_str.wstr_len = 0;
        } 
    }
}

/*****************************************************************************/
//  Description : form richtext string
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN Detail_FormConfigInfo(
                                    MMI_STRING_T    *str_dst,        //out.
                                    uint8*                cur_str,
                                    MMICONFIGSTATUS     state,
                                    BOOLEAN mms_ota_yes
                                    )
{
    char* label_str2_ptr = PNULL;
    uint32 label_str2_len = 0;
    MMI_STRING_T label_string2;
    
    label_str2_len = 512;
    label_str2_ptr = (char*)SCI_ALLOC(label_str2_len + 1);
    if (PNULL == label_str2_ptr)
    {
        //SCI_TRACE_LOW:"malloc Detail_FormConfigInfo failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2069_112_2_18_2_42_49_37,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(label_str2_ptr, 0, (label_str2_len + 1));
    if(cur_str)
    {
        sprintf(label_str2_ptr, 
            "%s\r",cur_str);
    }
    else
    {
        sprintf(label_str2_ptr, 
            ":%s\r"," ");
    }
    
    label_string2.wstr_len =strlen(label_str2_ptr);
    label_string2.wstr_ptr = SCI_ALLOCAZ((label_string2.wstr_len + 1) * sizeof(wchar) );
    if(PNULL == label_string2.wstr_ptr)
    {
        //SCI_TRACE_LOW:"Detail_FormConfigInfo  label_string2.wstr_ptr alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2088_112_2_18_2_42_49_38,(uint8*)"");
        return FALSE;
    }
    MMI_STRNTOWSTR( label_string2.wstr_ptr, label_string2.wstr_len, (uint8 *)label_str2_ptr, label_string2.wstr_len, label_string2.wstr_len );
    
    str_dst->wstr_ptr = (wchar*)SCI_ALLOCAZ(label_string2.wstr_len*sizeof(wchar));
    if(PNULL == str_dst->wstr_ptr)
    {
        SCI_FREE(label_string2.wstr_ptr);
        //SCI_TRACE_LOW:"Detail_FormConfigInfo  str_dst->wstr_ptr alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2097_112_2_18_2_42_49_39,(uint8*)"");
        return FALSE;
    }
    MMI_WSTRNCPY(str_dst->wstr_ptr, label_string2.wstr_len, label_string2.wstr_ptr, label_string2.wstr_len, label_string2.wstr_len);
    str_dst->wstr_len = label_string2.wstr_len;    
    
    SCI_FREE( label_string2.wstr_ptr );
    SCI_FREE(label_str2_ptr);
    label_str2_ptr = PNULL;
    return TRUE;
}

/*****************************************************************************/
//  Description : config info detail  window option menu handle function
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOTAConfigSetOptionWinMsg(
                                                   MMIOTA_WINDOW_ID_E win_id, 
                                                   MMI_MESSAGE_ID_E msg_id, 
                                                   DPARAM param
                                                   )
{
    MMI_RESULT_E  err_code        = MMI_RESULT_TRUE;    
    uint32        group_id        = 0;
    uint32        menu_id         = 0;
    uint16	      list_index      = 0;    
    BOOLEAN       is_locked       = FALSE;
    uint16        i               = 0;
    uint16        temp_len        = 0;
    uint16       cur_sel          = 0;
    uint8 num_len                 = 0;
    CC_RESULT_E cc_result         = CC_RESULT_SUCCESS;
    MMIPB_BCD_NUMBER_T  pb_bcd    = {0};
    MMI_PARTY_NUMBER_T  party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
    uint8 * sender_ptr            = PNULL;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_NORMAL;

    if (PNULL == g_ota_config_ptr)
    {
        return err_code;
    }

    //SCI_TRACE_LOW:"HandleOTAConfigSetOptionWinMsg() msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2139_112_2_18_2_42_49_40,(uint8*)"d", msg_id);
    list_index = (uint32)(MMK_GetWinAddDataPtr(win_id));
    for (i = 0;i < OTA_CONFIG_INFO_NUM_MAX;i++)
    {
        if(g_ota_config_ptr->ota_config_info[i].index !=0 &&(g_ota_config_ptr->ota_config_info[i].cur_select == 1))
        {
            cur_sel = i;
            break;
        }
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_locked = g_ota_config_ptr->ota_config_info[list_index].is_locked;        
        folder_type = g_ota_config_ptr->ota_config_info[list_index].folder_type;
        if (!is_locked)
        {
            GUIMENU_SetItemGrayed(MMIOTA_CONFIG_SET_MENU_CTRL_ID,MENU_OTA_CONFIG_SET_OPTION,ID_OTA_CONFIG_SET_DELETE,FALSE);
        }
        else
        {
            GUIMENU_SetItemGrayed(MMIOTA_CONFIG_SET_MENU_CTRL_ID,MENU_OTA_CONFIG_SET_OPTION,ID_OTA_CONFIG_SET_DELETE,TRUE);
        }
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        if (MMISMS_FOLDER_SECURITY == folder_type)
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_PUSH_SECURITY_SELECT,ID_PUSH_MENU_BOX_SECURITY_ITEM,TRUE);
            MMIAPICOM_EnableGrayed(win_id,MENU_PUSH_SECURITY_SELECT,ID_PUSH_MENU_BOX_PREVIEW_ITEM,FALSE);
        }
        else
        {
            MMIAPICOM_EnableGrayed(win_id,MENU_PUSH_SECURITY_SELECT,ID_PUSH_MENU_BOX_SECURITY_ITEM,FALSE);
            MMIAPICOM_EnableGrayed(win_id,MENU_PUSH_SECURITY_SELECT,ID_PUSH_MENU_BOX_PREVIEW_ITEM,TRUE);
        }
#endif
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK,TXT_NULL,STXT_RETURN,FALSE);        
        MMK_SetAtvCtrl(win_id,MMIOTA_CONFIG_SET_MENU_CTRL_ID);
        break;

    case MSG_PROMPTWIN_OK:        
        GUIMENU_GetId(MMIOTA_CONFIG_SET_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch(menu_id)
        {
        case ID_OTA_CONFIG_SET_DELETE:
            { 
                if(i < OTA_CONFIG_INFO_NUM_MAX)
                {
                    MMIAPIOTA_DeleteOTAPUSH(i);
//                    if(g_ota_config_ptr->ota_config_info[i].ota_setting_ptr.setting_type == MMIOTA_MMS_SETTING)
//                    {
//                        MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_MMS_OTA,i);
//                    }
//                    else
                    {            
                        MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_OTA,i);
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"warning i>OTA_CONFIG_INFO_NUM_MAX"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2199_112_2_18_2_42_49_41,(uint8*)"");
                }                              
            }
            //here should close the win and open the waitwin for delete.
            MMIPUB_CloseQuerytWin(PNULL); 
            MMIPUB_OpenAlertSuccessWin(TXT_DELETED);
            MMK_CloseWin(win_id);
            MMK_CloseWin(MMIOTA_CONFIG_INFO_DETAIL_WIN_ID);            
            break;
#if 0
        case ID_OTA_CONFIG_SET_SAVE:
#ifdef MMS_SUPPORT
            MMIAPIMMS_Setting_SetNetSettingIndex(MN_DUAL_SYS_1,g_defaut_now_saving_config);            
#endif
            //here should close the win and open the waitwin for delete.
            MMK_CloseWin(win_id);
            MMIPUB_CloseQuerytWin(PNULL); 
            break; 
#endif

        default:
            break;
        }
        
        break;
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);//here should close the win and open the waitwin for delete.
        MMK_CloseWin(win_id);
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId(MMIOTA_CONFIG_SET_MENU_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));
        switch ( menu_id)
        {
        case ID_OTA_CONFIG_SET_DELETE:
            //提示是否删除。
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY, IMAGE_PUBWIN_QUERY, PNULL, PNULL);        
            break;
        case ID_OTA_CONFIG_SET_SAVE:
            {
                if(i<OTA_CONFIG_INFO_NUM_MAX)
                {
                    uint16 sim_num = 0;
                    uint16 sim_ok = 0;

                    //SCI_TRACE_LOW:"save OTA config setting_type=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2245_112_2_18_2_42_49_42,(uint8*)"d", g_ota_config_ptr->ota_config_info[cur_sel].ota_setting_ptr.setting_type);

                    if (g_ota_config_ptr->ota_config_info[cur_sel].ota_setting_ptr.setting_type != MMIOTA_BRW_BOOKMARK)
                    {
                        sim_num = MMIAPIPHONE_GetSimCardOkNum(&sim_ok, 1);

                        if (0 == sim_num)
                        {
                            MMIAPIPHONE_AlertSimNorOKStatus();
                        }
                        else if (1 == sim_num)
                        {
                            if (MMIOTA_SaveConfig((MN_DUAL_SYS_E)sim_ok, cur_sel))
                            {
                                //if (g_ota_config_ptr->ota_config_info[cur_sel].ota_setting_ptr.setting_type != MMIOTA_BRW_BOOKMARK)
                                //{
                                    MMIPUB_OpenAlertSuccessWin(TXT_COMM_SAVE_SUCCESS);
                                //}                        
                            }
                            else
                            {
                                MMIPUB_OpenAlertWarningWin(TXT_MMS_NET_SETTING_FULL);
                            }
                        }
#ifndef MMI_MULTI_SIM_SYS_SINGLE
                        else
                        {
                            MMIOTA_SelectSIMFunc((ADD_DATA)cur_sel);
                        }
#endif

                        MMK_CloseWin(win_id);
                    }
                    else if(MMIOTA_SaveConfig(MMI_DUAL_SYS_MAX, cur_sel))
                    {
                         //SCI_TRACE_LOW:"save bookmark!!"
                         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2280_112_2_18_2_42_49_43,(uint8*)"");
                    }
                    else
                    {
                        MMIPUB_OpenAlertWarningWin(TXT_MMS_NET_SETTING_FULL);
                    }
                    MMK_CloseWin(win_id);
                }//if(i<OTA_CONFIG_INFO_NUM_MAX)
                else
                {
                    //SCI_TRACE_LOW:"warning i>OTA_CONFIG_INFO_NUM_MAX"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2290_112_2_18_2_42_49_44,(uint8*)"");
                }
            }
            //提示是否保存。
            break;
        case ID_OTA_CONFIG_SET_DETAIL: 
            MMK_CreateWin((uint32 *)MMIOTA_CONFIG_ITEM_DETAIL_WIN,(ADD_DATA)PNULL);
            MMK_CloseWin(win_id);
            //显示详情界面。
            break;
        case ID_OTA_CONFIG_SET_CALL:
//        case ID_MMS_IP_DIAL:
//        case ID_MMS_DIAL:
            if (i < OTA_CONFIG_INFO_NUM_MAX)
            {
                //sender_ptr = GetOTSSender();
                //if (PNULL != sender_ptr)
                //{
                //    SCI_FREE(sender_ptr);
                //    sender_ptr = PNULL;
                //}
                temp_len = OTA_CONFIG_SENDER_MAX_LEN + 1;
                sender_ptr = SCI_ALLOCAZ(temp_len);
                if (PNULL == sender_ptr)
                {
                    break;
                }
                SCI_MEMCPY(sender_ptr, g_ota_config_ptr->ota_config_info[i].sender, OTA_CONFIG_SENDER_MAX_LEN);
                num_len = (uint8)SCI_STRLEN((char *)sender_ptr);
//                if(ID_MMS_DIAL == menu_id)
//                {                        
//                    cc_result = MMIAPICC_MakeCall(
//                        g_ota_config_ptr->ota_config_info[i].dual_sys,
//                        g_ota_config_ptr->ota_config_info[i].sender,
//                        num_len,
//                        PNULL,
//                        CC_CALL_SIM_MAX,
//                        CC_CALL_NORMAL_CALL
//                        );
//                }
//                else
                {
                    cc_result = MMIAPICC_MakeCall(
                        g_ota_config_ptr->ota_config_info[i].dual_sys,
                        g_ota_config_ptr->ota_config_info[i].sender,
                        num_len,
                        PNULL,
                        PNULL,
                        CC_CALL_SIM_MAX,
                        CC_CALL_IP_CALL,
                        PNULL
                        );
                }
                SCI_FREE(sender_ptr);
                sender_ptr = PNULL;
                MMK_CloseWin(win_id);
            }            
            break;
        case ID_OTA_CONFIG_SET_ADD2PB:
            if (i < OTA_CONFIG_INFO_NUM_MAX)
            {
                // save to phonebook
                //if (PNULL != sender_ptr)
                //{
                //    SCI_FREE(sender_ptr);
                //    sender_ptr = PNULL;
                //}
                temp_len = OTA_CONFIG_SENDER_MAX_LEN + 1;
                sender_ptr = SCI_ALLOCAZ(temp_len);
                if (PNULL == sender_ptr)
                {
                    break;
                }
                SCI_MEMCPY(sender_ptr, g_ota_config_ptr->ota_config_info[i].sender, OTA_CONFIG_SENDER_MAX_LEN);
                num_len = (uint8)SCI_STRLEN((char *)sender_ptr);

                MMIAPICOM_GenPartyNumber(sender_ptr,num_len, &party_num);
                
                pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );
                MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
                //MMIAPIPB_OpenAddMenu(&pb_bcd,PB_GROUP_ALL);
                SCI_FREE(sender_ptr);
                sender_ptr = PNULL;
                MMK_CloseWin(win_id);
            }
            break;
#ifdef MMIPB_ADD_EXIST_CONTACT_SUPPORT
        case ID_OTA_CONFIG_SET_EXISTNUM:
            if (i < OTA_CONFIG_INFO_NUM_MAX)
            {
                // add to exited name
                //if (PNULL != sender_ptr)
                //{
                //    SCI_FREE(sender_ptr);
                //    sender_ptr = PNULL;
                //}
                temp_len = OTA_CONFIG_SENDER_MAX_LEN + 1;
                sender_ptr = SCI_ALLOCAZ(temp_len);
                if (PNULL == sender_ptr)
                {
                    break;
                }
                SCI_MEMCPY(sender_ptr, g_ota_config_ptr->ota_config_info[i].sender, OTA_CONFIG_SENDER_MAX_LEN);
                num_len = (uint8)SCI_STRLEN((char *)sender_ptr);

                MMIAPICOM_GenPartyNumber(sender_ptr,num_len, &party_num);                
                pb_bcd.npi_ton      = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( 
                    pb_bcd.number,  
                    party_num.bcd_num, 
                    pb_bcd.number_len 
                    );
                SCI_FREE(sender_ptr);
                sender_ptr = PNULL;
                if ( pb_bcd.number_len > 0 )
                {
                    MMIAPIPB_AddToExistContact(&pb_bcd, MMIPB_ADD_TYPE_NUMBER, PNULL); //(&pb_bcd);
                }
                else
                {
                    //提示窗口没有号码
                    MMIPUB_OpenAlertWarningWin(TXT_CL_EMPTY_NUM);
                }
                MMK_CloseWin(win_id);
            }
            break;
#endif            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case ID_PUSH_MENU_BOX_SECURITY_ITEM:
        case ID_PUSH_MENU_BOX_PREVIEW_ITEM:
            MMK_CloseWin(win_id);
            MMIPUB_OpenQueryWinByTextId(TXT_MOVE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            break;
#endif
            
        default:
            break;
        }///switch ( menu_id)
        break;
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);       
        break;
    case MSG_CLOSE_WINDOW:        /* 处理关闭窗口消息 */
        break;
        
    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }
    return err_code;
}

/*****************************************************************************/
//  Description : config info  window detail such as from
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleOTAConfigItemDetailWinMsg(
                                                    MMIOTA_WINDOW_ID_E win_id, 
                                                    MMI_MESSAGE_ID_E msg_id, 
                                                    DPARAM param
                                                    )
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint8        i = 0;
    uint16       cur_sel = 0;
    
    if (PNULL == g_ota_config_ptr)
    {
        return err_code;
    }
    //SCI_TRACE_LOW:"HandleMMSConfigInfoDetailWinMsg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2470_112_2_18_2_42_50_45,(uint8*)"");
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        /*changed by minghu for detail 090205*/
        for (i = 0; i < OTA_CONFIG_INFO_NUM_MAX; i++)
        {
            if(g_ota_config_ptr->ota_config_info[i].index !=0 &&(g_ota_config_ptr->ota_config_info[i].cur_select == 1))
            {
                cur_sel = i;
                break;
            }
        }
        if(i < OTA_CONFIG_INFO_NUM_MAX)
        {
            //SCI_TRACE_LOW:"cur_sel=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2486_112_2_18_2_42_50_46,(uint8*)"d",cur_sel);
            OTAConfigItemDetailEntry(MMIOTA_CONFIG_ITEM_DETAIL_CTRL_ID, &(g_ota_config_ptr->ota_config_info[cur_sel]));
        }
        
        //set active control
        MMK_SetAtvCtrl(win_id, MMIOTA_CONFIG_ITEM_DETAIL_CTRL_ID);
        
        break;    
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
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
//  Description : config info  window detail entry
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL void OTAConfigItemDetailEntry(MMI_CTRL_ID_T ctrl_id, MMIOTA_CONFIG_INFO_T *ota_item_ptr)
{
    GUIRICHTEXT_ITEM_T  item_data = {0};/*lint !e64*/
    MMI_STRING_T        temp_str = {0};
    wchar               colon[] = {':', '\n', 0};
    MMI_STRING_T        colon_str = {0};
    uint16              index = 0;
    MMI_STRING_T        sender_str = {0};
    MMI_STRING_T        date_str = {0};
    MMI_STRING_T        type_str = {0};
    MMI_STRING_T        type_text_str = {0};
    // MMI_STRING_T        time_str = {0};
    // char                time_tm[OTA_MAX_DATE_LEN] = {0};
    // uint8               date[30] = {0};
    // uint8               time[12] = {0};
    // MMI_TM_T            tm = {0};
    
    colon_str.wstr_len = MMIAPICOM_Wstrlen(colon); 
    colon_str.wstr_ptr = colon;
    
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;

    MMIAPIMMS_DetailSenderString(&sender_str, ota_item_ptr->sender, TXT_MMS_DETAIL_FROM);       
    item_data.text_data.buf.len = MIN(sender_str.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = sender_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    if (PNULL != sender_str.wstr_ptr)
    {
        SCI_FREE(sender_str.wstr_ptr);
        sender_str.wstr_ptr = PNULL;
        sender_str.wstr_len = 0;
    }
    
    MMIAPIMMS_DetailTimeString(&date_str,ota_item_ptr->time);    
    item_data.text_data.buf.len = MIN(date_str.wstr_len, GUIRICHTEXT_TEXT_MAX_LEN);
    item_data.text_data.buf.str_ptr = date_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
     if (PNULL != date_str.wstr_ptr)
    {
        SCI_FREE(date_str.wstr_ptr);
        date_str.wstr_ptr = PNULL;
        date_str.wstr_len = 0;
    }
    
    MMI_GetLabelTextByLang(TXT_COMMON_TYPE, &temp_str);
    if (ota_item_ptr->ota_setting_ptr.setting_type == MMIOTA_BRW_BOOKMARK)
    {
        MMI_GetLabelTextByLang(TXT_COMMON_BOOKMARKS, &type_text_str);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_OTA_LINK_CONFIG, &type_text_str);
    } 
    type_str.wstr_len = temp_str.wstr_len + colon_str.wstr_len + type_text_str.wstr_len;
    type_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((type_str.wstr_len + 1) * sizeof(wchar));
    if (PNULL == type_str.wstr_ptr)
    {
        return;
    }
    MMIAPICOM_Wstrncpy(type_str.wstr_ptr, temp_str.wstr_ptr, temp_str.wstr_len);
    MMIAPICOM_Wstrncpy(type_str.wstr_ptr + temp_str.wstr_len, colon_str.wstr_ptr, colon_str.wstr_len); 
    MMIAPICOM_Wstrncpy(type_str.wstr_ptr + temp_str.wstr_len + colon_str.wstr_len, type_text_str.wstr_ptr, type_text_str.wstr_len);
    item_data.text_data.buf.len = type_str.wstr_len;
    item_data.text_data.buf.str_ptr = type_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(type_str.wstr_ptr);
    type_str.wstr_ptr = PNULL;
    type_str.wstr_len = 0;    
}

/*****************************************************************************/
//  Description : delete ota by id
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_DelOTAByID(int32 id,int32 (*cbf)(BOOLEAN result,void* param),void *param)
{
    MMIAPIOTA_DeleteOTAPUSH((int16)id);
    if (PNULL != cbf)
    {
        cbf(TRUE,param);
    }    
}

/*****************************************************************************/
//  Description : get ota total number
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIAPIOTA_GetOTATotalNum(void)
{
    uint16 i=0;
    uint16 message_num=0;

    if (PNULL == g_ota_config_ptr)
    {
        return 0;
    }
    for(;i<OTA_CONFIG_INFO_NUM_MAX;i++)
    {
        if (g_ota_config_ptr->ota_config_info[i].index != 0)
        {
            message_num++;
        }
    }
    return message_num;
}

/*****************************************************************************/
//  Description : get ota items total number
//  Global resource dependence : 
//  Author:nihongf
//  Note: 
/*****************************************************************************/
LOCAL uint16 MMIOTA_GetOTAItemNum(void)
{
    uint16 index                      = 0;
    uint16 message_num                = 0;
    MMIOTA_SETTING_T *temp_config_ptr = PNULL;

    if (PNULL == g_ota_config_ptr)
    {
        return 0;
    }
    for(;index<OTA_CONFIG_INFO_NUM_MAX;index++)
    {
        if (0 != g_ota_config_ptr->ota_config_info[index].index)
        {
            message_num++;
            temp_config_ptr = &g_ota_config_ptr->ota_config_info[index].ota_setting_ptr;
            while (PNULL != temp_config_ptr->next)
            {
                message_num++;
                temp_config_ptr = temp_config_ptr->next;
            }
        }
    }
    return message_num;
}

/*****************************************************************************/
//  Description :get si time
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint32 MMIOTA_GetSiTime(uint8 *si_time_ptr, uint16 si_time_len)
{
    uint8 year[PUSH_SI_TIME_LEN]    = {0};
    uint8 month[PUSH_SI_TIME_LEN]   = {0};
    uint8 day[PUSH_SI_TIME_LEN]     = {0};
    uint8 hour[PUSH_SI_TIME_LEN]    = {0};
    uint8 minite[PUSH_SI_TIME_LEN]  = {0};
    uint8 seconds[PUSH_SI_TIME_LEN] = {0};
    uint32 time[6] = {0};
    uint32 ret_time = 0;

    //si time格式YYYY-MM-DDThh:mm:ssZ
    if (si_time_ptr == PNULL || si_time_len < PUSH_SI_TIME_LEN*2)
    {
        return 0;
    }
    SCI_MEMCPY(year, si_time_ptr, 4);
    SCI_MEMCPY(month,si_time_ptr+5, 2);
    SCI_MEMCPY(day, si_time_ptr+8, 2);
    SCI_MEMCPY(hour, si_time_ptr+11, 2);
    SCI_MEMCPY(minite, si_time_ptr+14, 2);
    SCI_MEMCPY(seconds, si_time_ptr+17, 2);
    time[0] = atoi((char *)seconds);
    time[1] = atoi((char *)minite);
    time[2] = atoi((char *)hour);
    time[3] = atoi((char *)day);
    time[4] = atoi((char *)month);
    time[5] = atoi((char *)year);

    ret_time = MMIAPICOM_Tm2Second(time[0], time[1], time[2], time[3], time[4], time[5]);

    return ret_time;
}

/*****************************************************************************/
//  Description :deal with the si push
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_DealWapPush(MMIOTA_SETTING_T  *mmi_setting_ptr, uint8 *sender_ptr, MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN       result     = TRUE;
    int32         index      = 0;
    uint16        i          = 0;
    MMI_STRING_T  prompt_str = {0};
    wchar  temp_title[]      = {'W','A','P',' ','P','U','S','H',0};
    wchar *push_title_ptr    = PNULL;
    uint16 sender_len        = 0;
    BOOLEAN is_alert         = TRUE;
    BOOLEAN is_save         = TRUE;
    BOOLEAN is_play_sound   = FALSE;
    BOOLEAN is_read_si       = FALSE;
    BOOLEAN is_enter_broswer = FALSE;
    uint32 sys_time          = 0;
    uint32 expired_time      = 0;
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ERROR_E  enter_err = MMIBROWSER_SUCC;
    MMIBROWSER_ENTRY_PARAM brw_parm = {0};/*lint !e64*/
#endif
    MMISMS_SL_OPERATION_E oper = MMIAPISMS_GetPushSmsSLOperation();

    sys_time = MMIAPICOM_GetCurTime();
    //SCI_TRACE_LOW:"MMIOTA_DealWapPush() IN"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2714_112_2_18_2_42_50_47,(uint8*)"");
    if (PNULL == mmi_setting_ptr)
    {
        return FALSE;
    }
    if (MMIOTA_BRW_WAP_SL == mmi_setting_ptr->setting_type)
    {        
        //SCI_TRACE_LOW:"MMIOTA_DealWapPush() execute_action = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2721_112_2_18_2_42_50_48,(uint8*)"d", mmi_setting_ptr->wap_sl.execute_action);
        if (MMIOTA_SL_EXECUTE_ACTION_LOW == mmi_setting_ptr->wap_sl.execute_action)
        {//提示用户，保存
            is_save = TRUE;
            is_alert = TRUE;
        }
        else if (MMIOTA_SL_EXECUTE_ACTION_HIGH == mmi_setting_ptr->wap_sl.execute_action)
        {//idle直接进入浏览器，其他界面提示，并且保存
#ifdef BROWSER_SUPPORT  
#ifdef MMI_KEY_LOCK_SUPPORT
            if (!MMIAPIKL_IsPhoneLocked())
            {
                is_save = TRUE;
                is_alert = TRUE;
                is_enter_broswer = TRUE;
            }
            else
#endif
#endif    
            {//提示用户
                is_save = TRUE;
                is_alert = TRUE;
            }
        }
        else if (MMIOTA_SL_EXECUTE_ACTION_CACHE == mmi_setting_ptr->wap_sl.execute_action)
        {//操作cache，把url加入cache中
            is_alert = FALSE;
            is_save = TRUE;
        }
    }
    else if(MMIOTA_BRW_WAP_SI == mmi_setting_ptr->setting_type)
    {        
        expired_time = MMIOTA_GetSiTime(mmi_setting_ptr->wap_si.si_expired_time, mmi_setting_ptr->wap_si.si_expired_time_len);
        if(expired_time != 0 && sys_time > expired_time)//过期直接丢弃
        {
            return TRUE;
        }        
        //SCI_TRACE_LOW:"MMIOTA_DealWapPush() signal_action = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2756_112_2_18_2_42_50_49,(uint8*)"d", mmi_setting_ptr->wap_si.signal_action);
        if (MMIOTA_SI_SIGNAL_ACTION_NONE == mmi_setting_ptr->wap_si.signal_action)
        {//不做任何处理返回
            //SCI_TRACE_LOW:"MMIOTA_DealWapPush() MMIOTA_SI_SIGNAL_ACTION_NONE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2759_112_2_18_2_42_50_50,(uint8*)"");
            is_save = FALSE;
            is_alert = FALSE;
        }
        else if (MMIOTA_SI_SIGNAL_ACTION_DELETE == mmi_setting_ptr->wap_si.signal_action)
        {//不处理当前这条push，同时删除相同id的push
            is_save = FALSE;
            is_alert = FALSE;
            if (PNULL == g_push_info_ptr)
            {
                //SCI_TRACE_LOW:"MMIOTA_DealWapPush() PNULL = g_push_info_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2769_112_2_18_2_42_50_51,(uint8*)"");
                return FALSE;
            }
            if (g_push_info_ptr->item_num != 0)
            {
                for(i = 0; i < MMICMSBRW_MAX_ITEM_NUM; i++)
                {
                    if(g_push_info_ptr->item_detail[i].list_num != -1 
                        && (0 < g_push_info_ptr->item_detail[i].si_id_len && g_push_info_ptr->item_detail[i].si_id_len == mmi_setting_ptr->wap_si.si_id_len)
                        && 0 == strcmp((char *)g_push_info_ptr->item_detail[i].si_id, (char *)mmi_setting_ptr->wap_si.si_id)
                        )
                    {
                        //SCI_TRACE_LOW:"MMIOTA_DealWapPush delete index =%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2781_112_2_18_2_42_50_52,(uint8*)"d",i);
                        MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH,i);
                        SCI_MEMSET(&g_push_info_ptr->item_detail[i], 0, sizeof(MMICMSBRW_ITEM_DETAIL_T));
                        g_push_info_ptr->item_detail[i].list_num = -1;
                        g_push_info_ptr->item_num--;
                    }
                }
            }
            //保存pushmsg的File
            MMIAPIOTA_SavePushFileInfo();
        }
        else if (MMIOTA_SI_SIGNAL_ACTION_LOW == mmi_setting_ptr->wap_si.signal_action)
        {//不提示，但是在收件箱保存
            is_alert = FALSE;
            is_save = TRUE;
        }
        else if (MMIOTA_SI_SIGNAL_ACTION_MEDIUM == mmi_setting_ptr->wap_si.signal_action)
        {//提示，在收件箱保存
            is_save = TRUE;
            is_alert = TRUE;
        }
        else if (MMIOTA_SI_SIGNAL_ACTION_HIGH == mmi_setting_ptr->wap_si.signal_action)
        {//在idle直接阅读，其他界面提示，同时收件箱保存    
            if(MMK_IsFocusWin(MAIN_IDLE_WIN_ID))
            {
                is_save = TRUE;
                is_alert = FALSE;
                is_read_si = TRUE;
                is_play_sound = TRUE;                
            }
            else
            {
                is_save = TRUE;
                is_alert = TRUE;
                is_read_si = FALSE;
            }
        }        
    }
    else if (MMIOTA_BRW_WAP_CO == mmi_setting_ptr->setting_type)
    {//对相应服务器的url cache进行删除
        //SCI_TRACE_LOW:"MMIOTA_DealWapPush() co"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2821_112_2_18_2_42_50_53,(uint8*)"");
#ifdef BROWSER_SUPPORT
        MMIAPIBROWSER_CleanCache(mmi_setting_ptr->wap_co.co_herf, mmi_setting_ptr->wap_co.co_herf_len);
#endif
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIOTA_DealWapPush() error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2829_112_2_18_2_42_50_54,(uint8*)"");
        return FALSE;
    }
    if (is_save)
    {
        //SCI_TRACE_LOW:"MMIOTA_DealWapPush() save"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2834_112_2_18_2_42_50_55,(uint8*)"");
      
        if (PNULL == g_push_info_ptr)
        {
            g_push_info_ptr = (MMICMSBRW_ITEM_T *)SCI_ALLOCAZ(sizeof(MMICMSBRW_ITEM_T));
            if (PNULL == g_push_info_ptr)
            {
                //SCI_TRACE_LOW:"MMIOTA_DealWapPush() PNULL == g_push_info_ptr"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2841_112_2_18_2_42_50_56,(uint8*)"");
                return FALSE;
            }
            for(i=0; i < MMICMSBRW_MAX_ITEM_NUM; i++)
            {
                g_push_info_ptr->item_detail[i].list_num = -1;
            }
        }
        if (MMIOTA_BRW_WAP_SI == mmi_setting_ptr->setting_type)
        {
            if (0 == mmi_setting_ptr->wap_si.si_name_len)
            {
                push_title_ptr = temp_title;
            }
            else
            {
                push_title_ptr = SCI_ALLOCAZ((mmi_setting_ptr->wap_si.si_name_len + 1) * sizeof(wchar));
                if (PNULL == push_title_ptr)
                {
                    return FALSE;
                }
                MMI_STRNTOWSTR(push_title_ptr, WAP_PUSH_NAME_MAX_LEN,(uint8 *)mmi_setting_ptr->wap_si.si_name, mmi_setting_ptr->wap_si.si_name_len, mmi_setting_ptr->wap_si.si_name_len);
            }
            index = MMIOTA_WriteWapPushDataToItemStruct(g_push_info_ptr, (wchar*)mmi_setting_ptr->wap_si.si_content,
                push_title_ptr, (char*)mmi_setting_ptr->wap_si.si_herf, TRUE, TRUE);
            g_push_info_ptr->item_detail[index].si_action = mmi_setting_ptr->wap_si.signal_action;
            g_push_info_ptr->item_detail[index].si_id_len = mmi_setting_ptr->wap_si.si_id_len;
            if (mmi_setting_ptr->wap_si.si_id_len != 0)
            {
                SCI_MEMCPY((char *)g_push_info_ptr->item_detail[index].si_id, 
                    (char *)mmi_setting_ptr->wap_si.si_id, mmi_setting_ptr->wap_si.si_id_len);
            }
            if (0 != mmi_setting_ptr->wap_si.si_name_len)
            {
                SCI_FREE(push_title_ptr);
                push_title_ptr = PNULL;
            }
        }
        else if (MMIOTA_BRW_WAP_SL == mmi_setting_ptr->setting_type)
        {
            if (0 == mmi_setting_ptr->wap_sl.sl_name_len)
            {
                push_title_ptr = temp_title;
            }
            else
            {
                push_title_ptr = SCI_ALLOCAZ((mmi_setting_ptr->wap_sl.sl_name_len + 1) * sizeof(wchar));
                if (PNULL == push_title_ptr)
                {
                    return FALSE;
                }
                MMI_STRNTOWSTR(push_title_ptr, WAP_PUSH_NAME_MAX_LEN,(uint8 *)mmi_setting_ptr->wap_sl.sl_name, mmi_setting_ptr->wap_sl.sl_name_len, mmi_setting_ptr->wap_sl.sl_name_len);
            }
            index = MMIOTA_WriteWapPushDataToItemStruct(g_push_info_ptr, (wchar*)mmi_setting_ptr->wap_sl.sl_service,
            push_title_ptr, (char*)mmi_setting_ptr->wap_sl.sl_herf, TRUE, TRUE);
            g_push_info_ptr->item_detail[index].sl_action = mmi_setting_ptr->wap_sl.execute_action;
            if (0 != mmi_setting_ptr->wap_sl.sl_name_len)
            {
                SCI_FREE(push_title_ptr);
                push_title_ptr = PNULL;
            }
        }
        g_push_info_ptr->item_detail[index].dual_sys = dual_sys;
        g_push_info_ptr->item_detail[index].time = sys_time;
        g_push_info_ptr->item_detail[index].expired_time = expired_time;
        g_push_info_ptr->item_detail[index].wap_push_type = mmi_setting_ptr->setting_type;
        //获得发件人
        SCI_MEMSET(g_push_info_ptr->item_detail[index].sender, 0, sizeof(g_push_info_ptr->item_detail[index].sender));
        if (PNULL != sender_ptr)
        {
            sender_len = SCI_STRLEN((char *)sender_ptr);
            sender_len = MIN(sender_len, OTA_CONFIG_SENDER_MAX_LEN);
            SCI_MEMCPY(g_push_info_ptr->item_detail[index].sender, sender_ptr, sender_len);
        }   
        
        //SCI_TRACE_LOW:"g_cmsbrw_pushmsg_info.item_detail[index].time=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2915_112_2_18_2_42_51_57,(uint8*)"d",g_push_info_ptr->item_detail[index].time);
        MMIOTA_InsertWapPushToSms((uint16)index);
        MAIN_SetMsgState(TRUE);
        //保存pushmsg的File
        MMIOTA_WriteWapPushFile(BEGIN_WIN_ID, MMICMSBRW_WAP_PUSHMSG_FILE_NAME,
            (uint8*)g_push_info_ptr, sizeof(MMICMSBRW_ITEM_T), VIRTUAL_WIN_ID, FALSE, CMSBRW_FS_PUSHMSG_TYPE);
    }
    if(is_alert)
    {
        uint32              time_out     = 0;
        MMI_WIN_PRIORITY_E  win_priority = WIN_LOWEST_LEVEL;
        MMI_WIN_ID_T		alert_win_id = MMICMSBRW_NEWPUSH_WIN_ID;

        //SCI_TRACE_LOW:"MMIOTA_DealWapPush() alert"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2928_112_2_18_2_42_51_58,(uint8*)"");
        MMI_GetLabelTextByLang(TXT_OTA_PUSH_ARRIVEED, &prompt_str);
        
        if(IsForbitNewMsgWin())
        {
            win_priority = WIN_LOWEST_LEVEL;
        }
        else
        {
            win_priority = WIN_ONE_LEVEL;
        }
        MMIOTA_SetPushRecordID((uint16)index);
 
        MMIDEFAULT_SetBackLight(TRUE);
        //if(!MMIAPIBROWSER_IsRunning())
        {
#ifdef MMIWIDGET_SUPPORT 
            if (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle())
            {
                MMIAPIWIDGET_AddDynamicItem(MMIWIDGET_CTRL_IDLE_ID, MMIWIDGET_NEWSMS_ID, FALSE, TRUE);                    
            }

            if(MMK_IsFocusWin(MAIN_IDLE_WIN_ID) && (MMISET_IDLE_STYLE_WIDGET == MMIAPISET_GetIdleStyle()))
            {                
            }
            else
#endif
//             if (   MMISET_IDLE_STYLE_COMMON == MMIAPISET_GetIdleStyle()
//                 || MMISET_IDLE_STYLE_SMART == MMIAPISET_GetIdleStyle()
//                 || WIN_ONE_LEVEL == win_priority)

            if (is_enter_broswer && MMIOTA_BRW_WAP_SL == mmi_setting_ptr->setting_type)
            {
                MMIAPIMMS_PlayMMSSound(dual_sys);
            }
            else
            {
                if(MMIAPISMS_IsInSMSWin()
                    || MMIAPIMMS_IsInMMSWin()
                    || MMIAPIOTA_IsInOTAWin())
                {
                    MMIPUB_OpenAlertWinByTextPtr(&time_out, &prompt_str, PNULL, IMAGE_PUBWIN_NEWMSG, &alert_win_id, &win_priority, MMIPUB_SOFTKEY_ONE, (MMIPUB_HANDLE_FUNC)MMIOTA_HandleWapPushNewMsgWin);
                }
                else
                {
                    MMIPUB_OpenAlertWinByTextPtr(&time_out, &prompt_str, PNULL, IMAGE_PUBWIN_NEWMSG, &alert_win_id, &win_priority, MMIPUB_SOFTKEY_CUSTOMER, (MMIPUB_HANDLE_FUNC)MMIOTA_HandleWapPushNewMsgWin);
                }
                MMIPUB_SetWinTitleTextId(MMICMSBRW_NEWPUSH_WIN_ID, TXT_COMMON_PROMPT, FALSE);
            }
        }
/*wei.ren cr124504 add*/        
#ifdef PDA_UI_DROPDOWN_WIN
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_PUSH);
#else
    MMIDROPDOWNWIN_AddNotifyRecord(DROPDOWN_NOTIFY_NEW_MESSAGE);
#endif
#endif
/*wei.ren cr124504 add*/
        MMIAPIMMS_PlayMMSSound(dual_sys);
    }
    if (is_read_si && MMIOTA_BRW_WAP_SI == mmi_setting_ptr->setting_type)
    {//读取sipush
        //SCI_TRACE_LOW:"MMIOTA_DealWapPush() read si"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2982_112_2_18_2_42_51_59,(uint8*)"");
        is_idle_read_push = TRUE;
        MMIAPIOTA_WapPushEnter(index);
        if (is_play_sound)
        {
            MMIAPIMMS_PlayMMSSound(dual_sys);
        }
    }
    if (is_enter_broswer && MMIOTA_BRW_WAP_SL == mmi_setting_ptr->setting_type)
    {
        if (PNULL == g_push_info_ptr)
        {
            return FALSE;
        }

        if (MMISMS_SL_OPERATION_ALWAYS == oper)
        {
            //SCI_TRACE_LOW:"MMIOTA_DealWapPush() enter broswer"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_2996_112_2_18_2_42_51_60,(uint8*)"");
#ifdef BROWSER_SUPPORT
            brw_parm.dual_sys = dual_sys;
            brw_parm.url_ptr = (char *)g_push_info_ptr->item_detail[index].url;
            brw_parm.type = MMIBROWSER_ACCESS_URL;
#ifdef  CMCC_UI_STYLE
            brw_parm.is_alert = FALSE;
#endif
            brw_parm.user_agent_ptr = PNULL;
            enter_err = MMIAPIBROWSER_Entry(&brw_parm);
            if (MMIBROWSER_SUCC == enter_err)
            {
                //SCI_TRACE_LOW:"MMIOTA_DealWapPush() succed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3005_112_2_18_2_42_51_61,(uint8*)"");
                result = TRUE;
            }

            MMIOTA_WapPushUpdateState(index);
#endif
        }
        else if (MMISMS_SL_OPERATION_PROMPT == oper)
        {
            MMI_WIN_ID_T prompt_win_id = MMIOTA_SL_PUSH_PROMPT_WIN_ID;
            MMI_STRING_T connect_string = {0};
            MMI_STRING_T prompt_string = {0};
            uint16 url_len = strlen((char *)g_push_info_ptr->item_detail[index].url);
            wchar space_ch = 0x20;
            
            MMI_GetLabelTextByLang(TXT_COMMON_CONNECT_COMN, &connect_string);

            prompt_string.wstr_len = connect_string.wstr_len + url_len + 1;
            prompt_string.wstr_ptr = SCI_ALLOCAZ((prompt_string.wstr_len  + 1)*sizeof(wchar));

            if (PNULL == prompt_string.wstr_ptr)
            {
                return FALSE;
            }

            MMI_WSTRNCPY(prompt_string.wstr_ptr, prompt_string.wstr_len,
                                        connect_string.wstr_ptr, connect_string.wstr_len, connect_string.wstr_len);

            MMI_WSTRNCPY(prompt_string.wstr_ptr + connect_string.wstr_len, prompt_string.wstr_len,
                                        &space_ch, 1, 1);

            MMI_STRNTOWSTR(prompt_string.wstr_ptr + connect_string.wstr_len + 1, url_len,
                                        g_push_info_ptr->item_detail[index].url, url_len, url_len);

            MMK_CloseWin(prompt_win_id);
        
            MMIPUB_OpenQueryWinByTextPtrEx(MMK_GetFirstAppletHandle(),
                                                                        &prompt_string,
                                                                        PNULL,
                                                                        IMAGE_PUBWIN_QUERY,
                                                                        &prompt_win_id,
                                                                        HandleSLPushPromptWinMsg,
                                                                        (uint32)index);

            SCI_FREE(prompt_string.wstr_ptr);

            result = TRUE;
        }
    }
    return result;
}

/*****************************************************************************/
// Description : Handle SL Push Prompt Win
// Global resource dependence :
// Author: 
// Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSLPushPromptWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;   
    int32 index = 0;
    MMIPUB_INFO_T *win_info_ptr = PNULL;
    
    win_info_ptr = (MMIPUB_INFO_T *)MMIPUB_GetWinAddDataPtr(win_id);

    if (PNULL != win_info_ptr)
    {
        index = (int32)win_info_ptr->user_data;
    }
 
    switch(msg_id)
    {  
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_YES, TXT_NULL, TXT_COMM_NO, FALSE);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_CTL_PENOK:
        {
#ifdef BROWSER_SUPPORT
            if (PNULL != g_push_info_ptr)
            {
                MMIBROWSER_ENTRY_PARAM brw_parm = {0};/*lint !e64*/

                brw_parm.dual_sys = g_push_info_ptr->item_detail[index].dual_sys;
                brw_parm.url_ptr = (char *)g_push_info_ptr->item_detail[index].url;
                brw_parm.type = MMIBROWSER_ACCESS_URL;
#ifdef  CMCC_UI_STYLE
                brw_parm.is_alert = FALSE;
#endif
                brw_parm.user_agent_ptr = PNULL;
                MMIAPIBROWSER_Entry(&brw_parm);
            }

            MMIOTA_WapPushUpdateState(index);
#endif

            MMK_CloseWin(win_id);
        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        result = MMIPUB_HandleQueryWinMsg(win_id, msg_id, param);
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description :save data to wap push globe struct
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL int32 MMIOTA_WriteWapPushDataToItemStruct(MMICMSBRW_ITEM_T *item_info,
                                                wchar*content_ptr,
                                                wchar*pTitle, 
                                                char* pURL,
                                                BOOLEAN ErrorProtect,
                                                BOOLEAN is_wappush
                                                )
{
    int32				index = 0;
    int32				i = 0;
    uint16				TextLen = 0;	//字符串的长度，如果是UCS2编码，它是相应字符个数的2倍
    int32				MaxLenLimit = 0;//字符串的长度的最大限制
    int32				numCmp = 0;
    BOOLEAN             unread = TRUE;
    uint16                  url_str_len = 0;
    
    //SCI_TRACE_LOW:"item item_num %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3033_112_2_18_2_42_51_62,(uint8*)"d", item_info->item_num);
    if (PNULL == pURL)
    {
        return -1;
    }
    if(item_info->item_num >= MMICMSBRW_MAX_ITEM_NUM)
    {
        //SCI_TRACE_LOW:"MMICMS_WriteDataToItemStruct() maxnum>=MMICMSBRW_MAX_ITEM_NUM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3040_112_2_18_2_42_51_63,(uint8*)"");
        //寻找最久远的那个数组成员
        for(i=0;i<MMICMSBRW_MAX_ITEM_NUM;i++)
        {
            if (item_info->item_detail[i].is_unload==0)
            {
                unread = FALSE;
                if (item_info->item_detail[i].list_num >= numCmp)
                {
                    index =i;
                    numCmp = item_info->item_detail[i].list_num;
                }
            }
            else if (item_info->item_detail[i].is_unload==1&&unread)
            {
                if (item_info->item_detail[i].list_num >= numCmp)
                {
                    index =i;
                    numCmp = item_info->item_detail[i].list_num;
                }
            }
            
        }
        //SCI_TRACE_LOW:"the latest listitem %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3063_112_2_18_2_42_51_64,(uint8*)"d", numCmp);
        //if the end item is reused the other item's list num must plus 1
        for(i=0;i<MMICMSBRW_MAX_ITEM_NUM;i++)
        {
            if (index != i)
                item_info->item_detail[i].list_num++;
        }
        
        //SCI_TRACE_LOW:"MMICMS_WriteDataToItemStruct() maxnum>=MMICMSBRW_MAX_ITEM_NUM index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3071_112_2_18_2_42_51_65,(uint8*)"d", index);
        item_info->item_detail[index].list_num = 0;
        if(is_wappush)
        {
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH,index);
        }		
        //将当前要覆盖的数组成员保存到暂存中去
//         if(ErrorProtect)
//         {
//             g_cmsbrw_item_temp_info.temp_detail = item_info->item_detail[index];
//         }
    }
    else
    {
        //SCI_TRACE_LOW:"MMICMS_WriteDataToItemStruct() maxnum<MMICMSBRW_MAX_ITEM_NUM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3085_112_2_18_2_42_51_66,(uint8*)"");
        //寻找还没有使用的数组成员
        for(i=MMICMSBRW_MAX_ITEM_NUM-1;i>=0;i--)
        {
            if(item_info->item_detail[i].list_num < 0)
            {
                index = i;
            }
            else
            {
                item_info->item_detail[i].list_num++;
            }
        }
        //SCI_TRACE_LOW:"MMICMS_WriteDataToItemStruct() index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3098_112_2_18_2_42_51_67,(uint8*)"d", index);
        item_info->item_detail[index].list_num = 0;
        
        //将暂存清空，表示当前操作不涉及到暂存
//         if(ErrorProtect)
//         {
//             SCI_MEMSET(&g_cmsbrw_item_temp_info.temp_detail,
//                 0,
//                 sizeof(MMICMSBRW_ITEM_DETAIL_T));
//         }
        item_info->item_num++;
    }
    //保存当前文件操作的数组下标，以备文件操作失败时恢复使用
//     if(ErrorProtect)
//     {
//         g_cmsbrw_item_temp_info.last_temp_num = index;
//     }
    
    //获得当前Title
    SCI_MEMSET(item_info->item_detail[index].name, 0, sizeof(item_info->item_detail[index].name));    
    TextLen = MMIAPICOM_Wstrlen(pTitle);
    MaxLenLimit = WAP_PUSH_NAME_MAX_LEN;
    if(TextLen <= MaxLenLimit)
    {
        MMI_WSTRNCPY(item_info->item_detail[index].name,WAP_PUSH_NAME_MAX_LEN,
            pTitle,TextLen,
            TextLen);
        item_info->item_detail[index].name_len = TextLen;
    }
    else
    {
        MMI_WSTRNCPY(item_info->item_detail[index].name,WAP_PUSH_NAME_MAX_LEN,
            pTitle,MaxLenLimit,
            MaxLenLimit);
        item_info->item_detail[index].name_len = MaxLenLimit;
    }
    
    //获得push内容
    item_info->item_detail[index].content_len = MMIAPICOM_Wstrlen(content_ptr);
    item_info->item_detail[index].content_len = MIN(item_info->item_detail[index].content_len, OTA_MAX_CONTENT_LEN);
    MMI_WSTRNCPY(item_info->item_detail[index].content, OTA_MAX_CONTENT_LEN, 
        content_ptr, item_info->item_detail[index].content_len, item_info->item_detail[index].content_len);   

    //获得当前URL
    SCI_MEMSET(item_info->item_detail[index].url, 0, sizeof(item_info->item_detail[index].url));
    url_str_len = SCI_STRLEN(pURL);
    MMI_MEMCPY((char*)item_info->item_detail[index].url,OTA_MAX_URL_LEN,
        pURL,url_str_len,url_str_len);
    item_info->item_detail[index].url_len = SCI_STRLEN(pURL);
    item_info->item_detail[index].is_unload = 1;
    item_info->item_detail[index].is_locked = FALSE;
    
    return index;
}

/*****************************************************************************/
//  Description :insert wap push into sms
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_InsertWapPushToSms(uint16 index)
{
    MMISMS_ORDER_INFO_T  current_mms = {0};
    MMICMSBRW_ITEM_DETAIL_T *cur_item=PNULL;
    uint16 real_len =0;
    uint16 sender_len = 0;

    if (PNULL == g_push_info_ptr)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"MMIAPIBRW_InsertWapPushToSms,index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3169_112_2_18_2_42_51_68,(uint8*)"d",index);
    if(index>=MMICMSBRW_MAX_ITEM_NUM)
    {
        return;
    }
    cur_item = (MMICMSBRW_ITEM_DETAIL_T*)&g_push_info_ptr->item_detail[index];
    if(cur_item->list_num==-1)
    {
        return;
    }
    current_mms.dual_sys = (MN_DUAL_SYS_E)cur_item->dual_sys;
    current_mms.record_id = index;
    if(cur_item->is_unload)
    {
        current_mms.mo_mt_type = MMISMS_MT_TO_BE_READ;
    }
    else
    {
        current_mms.mo_mt_type = MMISMS_MT_HAVE_READ;
    }
    current_mms.time = cur_item->time;
    //SCI_TRACE_LOW:"time=%d,cur_item->time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3190_112_2_18_2_42_51_69,(uint8*)"dd",current_mms.time ,cur_item->time);
    current_mms.msg_type = MMISMS_TYPE_WAP_PUSH;
    //号码暂时置为空
    //current_mms.sender = PNULL;
    
    /* added by jhpeng for cr118069 */
    /* 锁定标志被遗漏 */
    current_mms.is_lock = cur_item->is_locked;    
    
    SCI_MEMSET(current_mms.sender, 0, sizeof(current_mms.sender));
    sender_len = SCI_STRLEN((char *)cur_item->sender);
    sender_len = MIN(sender_len, MMISMS_NUMBER_MAX_LEN);
    sender_len = MIN(sender_len, OTA_CONFIG_SENDER_MAX_LEN);
    SCI_MEMCPY((char*)current_mms.sender, (char*)cur_item->sender, sender_len);
    if (0 < cur_item->content_len)
    {
        real_len = MIN(cur_item->content_len, MMIMESSAGE_DIAPLAY_LEN);
        MMIAPICOM_Wstrncpy(current_mms.display_content.content, cur_item->content, real_len);
        current_mms.display_content.content_len = real_len;
    }   
    current_mms.folder_type = cur_item->folder_type;
    
    if (-1 != current_mms.record_id && 0 != current_mms.time && MMISMS_TYPE_WAP_PUSH == current_mms.msg_type)
    {
        MMIAPISMS_InsertOneMsgToOrder(&current_mms);
    }
    else
    {
        memset(&g_push_info_ptr->item_detail[index],0,sizeof(MMICMSBRW_ITEM_DETAIL_T));
        g_push_info_ptr->item_detail[index].list_num = -1;
        g_push_info_ptr->item_num--;
        MMIAPIOTA_SavePushFileInfo();
    }
}

/*****************************************************************************/
//  Description :save wap push file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL CMSBRW_FS_OPERATE_RESULT MMIOTA_WriteWapPushFile(uint32 source_win_id, 
                                                       const wchar* flash_filename, 
                                                       uint8* savebuf, 
                                                       int32 filelen, 
                                                       uint32 win_id, 
                                                       BOOLEAN IsJumpDialog, 
                                                       CMSBRW_FS_OPERATE_TYPE fileType
                                                       )
{
    MMIFILE_HANDLE		cur_hefs           = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E		efs_error          = SFS_NO_ERROR;
    uint32				transmit           = 0;
    wchar ucs2_name[OTA_UCS2_NAME_MAX_LEN] = {0};
    int  ucs2_name_len                     = 0;
    
    //SCI_TRACE_LOW:"MMIOTA_WriteWapPushFile() in!filelen=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3243_112_2_18_2_42_51_70,(uint8*)"d",filelen);
    //SCI_TRACE_LOW:"MMIOTA_WriteWapPushFile:flash_filename=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3244_112_2_18_2_42_51_71,(uint8*)"s",flash_filename);
        
    g_ffsType = MMIOTA_GetCurrentFFS();
    // if there is no U-disk or SD card return error;
    if (g_ffsType >= MMI_DEVICE_NUM)
    {
        return CMSBRW_FS_WRITE_FAIL;
    }
    
    if (PNULL != g_push_info_ptr && g_push_info_ptr->item_num > 0)
    {
        MMIOTA_FilecCreateDir((char *)MMIMMS_MMS_DIR);
        cur_hefs = MMIOTA_FilecCreate((uint8*)MMIOTA_WAP_PUSHMSG_FILE_NAME, SFS_MODE_WRITE | SFS_MODE_CREATE_ALWAYS);/*lint !e655*/
        
        //SCI_TRACE_LOW:"MMIOTA_WriteWapPushFile,create cur_hefs1"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3258_112_2_18_2_42_51_72,(uint8*)"");
        if (cur_hefs == SFS_INVALID_HANDLE)
        {
            //cur_hefs = MMIOTA_FilecCreate((char*)MMIOTA_WAP_PUSHMSG_FILE_NAME, SFS_MODE_OPEN_ALWAYS | SFS_MODE_WRITE);
            //SCI_TRACE_LOW:"MMIOTA_WriteWapPushFilecreate cur_hefs2=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3262_112_2_18_2_42_51_73,(uint8*)"d",cur_hefs);
            //SCI_TRACE_LOW:"return fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3263_112_2_18_2_42_51_74,(uint8*)"");
            return CMSBRW_FS_WRITE_FAIL;
        }        
        
        if (PNULL != savebuf )
        {
            efs_error = MMIOTA_FileWrite(cur_hefs, savebuf, filelen, &transmit, PNULL);
        }
        else
        {
            //close file
            MMIOTA_FileClose(cur_hefs);
            MMIOTA_FileDelete(flash_filename,PNULL);
            return CMSBRW_FS_WRITE_FAIL;
        }
        //SCI_TRACE_LOW:"MMIOTA_WriteWapPushFile error=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3278_112_2_18_2_42_51_75,(uint8*)"d", efs_error);
        MMIOTA_FileClose(cur_hefs);
        cur_hefs = SFS_INVALID_HANDLE;
    }
    else 
    {
        if(MMIOTA_CheckFileExist((const uint8*)MMIOTA_WAP_PUSHMSG_FILE_NAME) == TRUE)
        {            
            ucs2_name_len = GUI_GBToWstr(ucs2_name, (uint8 *)MMIOTA_WAP_PUSHMSG_FILE_NAME,(uint16)SCI_STRLEN((char*)MMIOTA_WAP_PUSHMSG_FILE_NAME));
            efs_error = MMIOTA_FileDelete(ucs2_name, PNULL);
            //SCI_TRACE_LOW:"VC:DeleteVideoFile FFS_AYS_Delete = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3288_112_2_18_2_42_51_76,(uint8*)"d", efs_error);
        }
        if (PNULL != g_push_info_ptr)
        {
            SCI_FREE(g_push_info_ptr);
            g_push_info_ptr = PNULL;
        }
    }
    return CMSBRW_FS_WRITE_SUCCEED;
}

/*****************************************************************************/
//  Description :set wap push record id
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_SetPushRecordID(uint16 push_record_id)
{
    g_push_record_id = push_record_id;
}

/*****************************************************************************/
//  Description :get wap push record id
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL uint16  MMIOTA_GetPushRecordID( void)
{
    return g_push_record_id;
}

/*****************************************************************************/
//  Description :new wap push alert win handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  MMIOTA_HandleWapPushNewMsgWin(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;
    
    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
        //SCI_TRACE_LOW:"MMIMMS_HandleNewMsgWin(),OPEN"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3332_112_2_18_2_42_51_77,(uint8*)"");
        break;
        
    case MSG_FULL_PAINT:
        if ( 
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_IsPhoneLocked() ||
#endif
            MMIAPISMS_IsInSMSWin()
            || MMIAPIMMS_IsInMMSWin()
            || MMIAPIOTA_IsInOTAWin())
        {
            MMIPUB_SetWinSoftkey(win_id, TXT_NULL, STXT_EXIT, FALSE);
        }
        else
        {
            MMIPUB_SetWinSoftkey(win_id, STXT_SELECT, STXT_EXIT, FALSE);
        }
        
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
        if (
#ifdef MMI_KEY_LOCK_SUPPORT
            MMIAPIKL_IsPhoneLocked() ||
#endif
            MMIAPISMS_IsInSMSWin()
            || MMIAPIMMS_IsInMMSWin()
            || MMIAPIOTA_IsInOTAWin())
        {
            //if key locked, then only close this window
            MMK_CloseWin(win_id);
        }
        else
        {
            // uint16 push_id = MMIOTA_GetPushRecordID();
#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
            MMIAPISMS_OpenPushMsgBox();
#else  
            MMIAPISMS_OpenMtBox();
#endif            
//            if(push_id!=MMICMSBRW_MAX_ITEM_NUM)
//            {
//                if(!MMIAPIOTA_WapPushEnter(push_id))
//                {
//                    SCI_TRACE_LOW("MMIMMS_HandleNewMsgWin(),show failed");
//                }
//            }
            //open mms inbox
            MMK_CloseWin( win_id );
        }
        break;
        
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CLOSE_WINDOW:                       
        MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);
        MMIAPISET_StopVibrator(MMISET_RING_TYPE_MSG);
        MMIENVSET_StopOnetimeVibTimer();
        MMIAPIENVSET_SetVibStatus(FALSE);

        MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
        
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
        
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    
    return (recode);
}

/*****************************************************************************/
//  Description :wap push enter
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIOTA_WapPushEnter(uint16 ID)
{
    BOOLEAN ret= TRUE;

    if (PNULL == g_push_info_ptr)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIOTA_WapPushEnter,ID=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3411_112_2_18_2_42_52_78,(uint8*)"d",ID);
    if(ID >= MMICMSBRW_MAX_ITEM_NUM)
    {
        //SCI_TRACE_LOW:"MMIOTA_WapPushEnter,ID is too large"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3414_112_2_18_2_42_52_79,(uint8*)"");
        return FALSE;
    }
    if(ID == MMIOTA_GetPushRecordID())
    {
        MMIOTA_SetPushRecordID(MMICMSBRW_MAX_ITEM_NUM);
        MMK_CloseWin(MMICMSBRW_NEWPUSH_WIN_ID);
    }
    //打开显示书签信息窗口
    if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
    {
        MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
    }
#ifdef MMI_PDA_SUPPORT
    if (1 == g_push_info_ptr->item_detail[ID].is_unload)
    {
        MMIOTA_WapPushUpdateState(ID);
    }
#endif
    MMK_CreateWin((uint32*)MMICMSBRW_SHOWPUSHMSG_WIN_TAB, (ADD_DATA)ID);    
    return ret;	
}

/*****************************************************************************/
//  Description :wap push update state to be read to have read
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN  MMIOTA_WapPushUpdateState(uint16 ID)
{
    BOOLEAN ret= TRUE;
    MMISMS_ORDER_INFO_T  current_mms = {0};	
    MMICMSBRW_ITEM_DETAIL_T *cur_item=PNULL;
    uint16 real_len = 0;
    uint16 sender_len = 0;

    if (PNULL == g_push_info_ptr)
    {
        return FALSE;
    }

     //SCI_TRACE_LOW:"MMIAPIOTA_WapPushUpdateState,ID=%d"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3454_112_2_18_2_42_52_80,(uint8*)"d",ID);
    //保存pushmsg的File
    g_push_info_ptr->item_detail[ID].is_unload = 0;//标记为已读
    MMIOTA_WriteWapPushFile(BEGIN_WIN_ID,
        MMICMSBRW_WAP_PUSHMSG_FILE_NAME,
        (uint8*)g_push_info_ptr,
        sizeof(MMICMSBRW_ITEM_T),
        VIRTUAL_WIN_ID,
        FALSE,
        CMSBRW_FS_PUSHMSG_TYPE);
    cur_item = (MMICMSBRW_ITEM_DETAIL_T*)&g_push_info_ptr->item_detail[ID];
    current_mms.record_id = ID;
    current_mms.dual_sys=(MN_DUAL_SYS_E)cur_item->dual_sys;
    current_mms.mo_mt_type = MMISMS_MT_HAVE_READ;
    current_mms.time=cur_item->time;
    current_mms.msg_type = MMISMS_TYPE_WAP_PUSH;
    
    /* added by plum peng for cr125185 */
    current_mms.is_lock = cur_item->is_locked;
    current_mms.folder_type = cur_item->folder_type;
    //SCI_TRACE_LOW:"MMIAPIOTA_WapPushUpdateState,folder_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3474_112_2_18_2_42_52_81,(uint8*)"d",cur_item->folder_type);

    //获得发件人
    SCI_MEMSET(current_mms.sender, 0, sizeof(current_mms.sender));
    sender_len = SCI_STRLEN((char *)cur_item->sender);
    sender_len = MIN(sender_len, MMISMS_NUMBER_MAX_LEN);
    sender_len = MIN(sender_len, OTA_CONFIG_SENDER_MAX_LEN);
    SCI_MEMCPY((char*)current_mms.sender, (char*)cur_item->sender, sender_len);
    
    if (0 < cur_item->content_len)
    {
        real_len = MIN(cur_item->content_len, MMIMESSAGE_DIAPLAY_LEN);
        MMIAPICOM_Wstrncpy(current_mms.display_content.content, cur_item->content, real_len);
        current_mms.display_content.content_len = real_len;
    }     
    
    MMIAPISMS_UpdateMsgStatusInOrder(&current_mms);
    
    return ret;	
}

/*****************************************************************************/
//  Description :init wap push file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_InitWapPush(void)
{
    uint16 i=0;
    MMIFILE_DEVICE_E dev_type = MMI_DEVICE_NUM;
    
    if (PNULL != g_push_info_ptr)
    {
        SCI_FREE(g_push_info_ptr);
        g_push_info_ptr = PNULL;
    }
    
    dev_type = MMIOTA_GetCurrentFFS();
    if(dev_type != MMI_DEVICE_NUM)
    {        
        if(MMIOTA_CheckFileExist((const uint8*)MMIOTA_WAP_PUSHMSG_FILE_NAME) == TRUE)
        {
            //读文件
            uint32 file_len=0;
            uint32 bytes_read= 0;
            MMIFILE_ERROR_E error = SFS_NO_ERROR;
            wchar ucs2_name[OTA_UCS2_NAME_MAX_LEN] = {0};
            int  ucs2_name_len = 0;
            MMIFILE_HANDLE file_handle = MMIOTA_FilecCreate((const uint8*)MMIOTA_WAP_PUSHMSG_FILE_NAME,SFS_MODE_READ|SFS_MODE_OPEN_EXISTING);/*lint !e655*/
            
            if(SFS_INVALID_HANDLE != file_handle)
            {
                file_len = MMIOTA_FileGetLength(file_handle);
                ucs2_name_len = GUI_GBToWstr(ucs2_name, (uint8 *)MMIOTA_WAP_PUSHMSG_FILE_NAME,(uint16)SCI_STRLEN((char*)MMIOTA_WAP_PUSHMSG_FILE_NAME));
                
                if(file_len == sizeof(MMICMSBRW_ITEM_T))
                {
                    g_push_info_ptr = (MMICMSBRW_ITEM_T *)SCI_ALLOCAZ(sizeof(MMICMSBRW_ITEM_T));
                    if (PNULL == g_push_info_ptr)
                    {
                        return;
                    }    
                    for(; i < MMICMSBRW_MAX_ITEM_NUM; i++)
                    {
                        g_push_info_ptr->item_detail[i].list_num = -1;
                    }
                    error = MMIOTA_FileRead(file_handle,(uint8 *)g_push_info_ptr, file_len, &bytes_read, PNULL);//同步读文件
                    if((SFS_ERROR_NONE != error) || (g_push_info_ptr->item_num > MMICMSBRW_MAX_ITEM_NUM))
                    {
                        //如果item_num大于MMICMSBRW_MAX_ITEM_NUM，说明文件已经遭到破坏
                        MMIOTA_FileDelete(ucs2_name, PNULL); 
                        SCI_FREE(g_push_info_ptr);
                        g_push_info_ptr = PNULL;
                    }  
                    MMIOTA_FileClose(file_handle);
                }
                else//长度不对，文件已经遭到破坏
                {
                    MMIOTA_FileClose(file_handle);
                    MMIOTA_FileDelete(ucs2_name, PNULL);
                }                
            }//if(0!=file_handle)
        }//if(MMIAPIFMM_IsFileExist(full_path,full_path_len))
    }//if(dev_type!=FS_INVALID)
}

/*****************************************************************************/
//  Description :load wap push into sms list
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_LoadWapPushListToSms(void)
{
    uint16 i=0;

    if (PNULL == g_push_info_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIBRW_InsertWapPushToSms PNULL == g_push_info_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3567_112_2_18_2_42_52_82,(uint8*)"");
        return;
    }
    
    for(;i<MMICMSBRW_MAX_ITEM_NUM;i++)
    {
        if(g_push_info_ptr->item_detail[i].list_num != -1)
        {
            //SCI_TRACE_LOW:"MMIAPIBRW_InsertWapPushToSms list_num=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3575_112_2_18_2_42_52_83,(uint8*)"d",g_push_info_ptr->item_detail[i].list_num);
            if (g_push_info_ptr->item_detail[i].time != 0)
            {
                MMIOTA_InsertWapPushToSms(i);
            }
            else
            {
                //SCI_TRACE_LOW:"MMIAPIBRW_InsertWapPushToSms time is zero!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3582_112_2_18_2_42_52_84,(uint8*)"");
                g_push_info_ptr->item_detail[i].list_num = -1;
            }
        }
    }
}

/*****************************************************************************/
//  Description :delete ota push by id
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_DelPushByID(uint16 ID,int32 (*cbf)(BOOLEAN result,void* param),void *param)
{
    int16 j = 0;
    int16 list_num=0;
    BOOLEAN ret = TRUE;
    MMICMSBRW_ITEM_T *push_msg_list = PNULL;

    if (PNULL == g_push_info_ptr)
    {
        if(cbf!=NULL)
        {
            cbf(FALSE,param);
        } 
        return FALSE;
    }
    push_msg_list = g_push_info_ptr;

    //SCI_TRACE_LOW:"MMIAPIOTA_DelPushByID ID =%d, totalnum = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3610_112_2_18_2_42_52_85,(uint8*)"dd",ID, g_push_info_ptr->item_num);

    if(g_push_info_ptr->item_num == 0 || ID>=MMICMSBRW_MAX_ITEM_NUM)
    {
        if (g_push_info_ptr->item_num == 0)
        {
            for(j = 0; j < MMICMSBRW_MAX_ITEM_NUM; j++)
            {
                if(push_msg_list->item_detail[j].list_num != -1)
                {
                    //SCI_TRACE_LOW:"MMIAPIOTA_DelPushByID index =%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3620_112_2_18_2_42_52_86,(uint8*)"d",j);
                    MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH,j);
                    SCI_MEMSET(&push_msg_list->item_detail[j], 0, sizeof(MMICMSBRW_ITEM_DETAIL_T));
                    push_msg_list->item_detail[j].list_num = -1;
                }
            }
            MMIAPIOTA_SavePushFileInfo();
        }
        if(cbf!=NULL)
        {
            cbf(FALSE,param);
        }           
        return TRUE;
    } 
    list_num = g_push_info_ptr->item_detail[ID].list_num;
    memset(&g_push_info_ptr->item_detail[ID],0,sizeof(MMICMSBRW_ITEM_DETAIL_T));
    g_push_info_ptr->item_detail[ID].list_num =-1;
    g_push_info_ptr->item_num--;
    
    //SCI_TRACE_LOW:"g_cmsbrw_pushmsg_info.item_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3639_112_2_18_2_42_52_87,(uint8*)"d",g_push_info_ptr->item_num);
    for(j = 0; j < MMICMSBRW_MAX_ITEM_NUM; j++)
    {
        if(g_push_info_ptr->item_detail[j].list_num>list_num)
        {
            g_push_info_ptr->item_detail[j].list_num--;
        }
    }
    if(ID == MMIOTA_GetPushRecordID())
    {
        MMIOTA_SetPushRecordID(MMICMSBRW_MAX_ITEM_NUM);
        MMK_CloseWin(MMICMSBRW_NEWPUSH_WIN_ID);
    }
    //保存pushmsg的File
    ret = MMIOTA_WriteWapPushFileWithCallBack(MMICMSBRW_WAP_PUSHMSG_FILE_NAME,
        CMSBRW_FS_PUSHMSG_DELETE_STRUCT_MEMBER_TYPE,
        (uint8*)g_push_info_ptr,
        sizeof(MMICMSBRW_ITEM_T),
        cbf,
        param
        );
    return ret;
}

/*****************************************************************************/
//  Description :delete ota push and write file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIOTA_WriteWapPushFileWithCallBack(const wchar* file_name, 
                                                  CMSBRW_FS_OPERATE_TYPE file_type,
                                                  uint8* buf,
                                                  int32 buf_len,
                                                  Brw_Cbf cbf,
                                                  void *param)
{
    MMIFILE_HANDLE		cur_hefs = SFS_INVALID_HANDLE;
    MMIFILE_ERROR_E		efs_error = SFS_NO_ERROR;
    uint32 save_len = 0;
    uint8 file_name_str[OTA_UCS2_NAME_MAX_LEN] = {0};
    
    //SCI_TRACE_LOW:"MMIBRW_WriteFile_Ex,file_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3679_112_2_18_2_42_52_88,(uint8*)"d",file_type);
    if((MMIOTA_GetCurrentFFS()== MMI_DEVICE_NUM) || buf_len== 0 || buf==NULL|| file_name==NULL) 
    {
        //SCI_TRACE_LOW:"wrong param"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3682_112_2_18_2_42_52_89,(uint8*)"");
        if(cbf!=NULL)
        {
            cbf(FALSE,param);
        }        
        return FALSE;
    }
    
    MMI_WSTRNTOSTR((uint8 *)file_name_str, OTA_UCS2_NAME_MAX_LEN, file_name, MMIAPICOM_Wstrlen(file_name), MMIAPICOM_Wstrlen(file_name));
    if (PNULL != g_push_info_ptr && g_push_info_ptr->item_num > 0)
    {
        cur_hefs = MMIOTA_FilecCreate(file_name_str,SFS_MODE_CREATE_ALWAYS | SFS_MODE_WRITE);/*lint !e655*/
        if (cur_hefs == SFS_INVALID_HANDLE)
        {
            //SCI_TRACE_LOW:"cyz create file failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3693_112_2_18_2_42_52_90,(uint8*)"");
            cur_hefs = MMIOTA_FilecCreate(file_name_str, SFS_MODE_OPEN_ALWAYS | SFS_MODE_WRITE);/*lint !e655*/
            if(cur_hefs==SFS_INVALID_HANDLE)
            {
                //SCI_TRACE_LOW:"MMICMSWAP_FILE_Create failed"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3697_112_2_18_2_42_52_91,(uint8*)"");
                if(cbf!=NULL)
                {
                    cbf(FALSE,param);
                }        
                return FALSE;
            }        
        }
        
        efs_error = MMIOTA_FileWrite(cur_hefs, buf, buf_len, &save_len, PNULL);
        MMIOTA_FileClose(cur_hefs);
        //SCI_TRACE_LOW:"cyz write file error=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3708_112_2_18_2_42_52_92,(uint8*)"d",efs_error);
        if(SFS_ERROR_IO_PENDING == efs_error)
        {
            return TRUE;
        }
        else if(SFS_ERROR_NONE==efs_error)
        {
            MMIOTA_FileDelete((wchar*)file_name_str, PNULL); ; 
            if(cbf!=NULL)
            {
                cbf(TRUE,param);
            }  
            return TRUE;
        }
        else
        {
            MMIOTA_FileDelete((wchar*)file_name_str, PNULL);  
            if(cbf!=NULL)
            {
                cbf(FALSE,param);
            }  
            return TRUE; 
        }
    }
    else
    {
        if(cbf!=NULL)
        {
            cbf(FALSE,param);
        } 
        if(MMIOTA_CheckFileExist((const uint8*)MMIOTA_WAP_PUSHMSG_FILE_NAME) == TRUE)
        {            
            efs_error = MMIOTA_FileDelete(file_name, PNULL);
            //SCI_TRACE_LOW:"VC:DeleteVideoFile FFS_AYS_Delete = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3288_112_2_18_2_42_51_76,(uint8*)"d", efs_error);
        }
        if (PNULL != g_push_info_ptr)
        {
            SCI_FREE(g_push_info_ptr);
            g_push_info_ptr = PNULL;
        }
        return FALSE;
    }  
}

/*****************************************************************************/
//  Description :get flag lock
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void MMIOTA_FileOpCbf(SFS_ERROR_E efs_error, //error 
                               uint32 param,  //param
                               uint32 result, //size writed
                               uint32 param2
                               )
{
    MMIBRW_VIRWIN_MESSAGE_T      message={0};
    MMIBRW_FILE_OP_PARM_T *file_op_result=(MMIBRW_FILE_OP_PARM_T *)param;
    
    //SCI_TRACE_LOW:"MMIOTA_FileOpCbf,efs_err=%d,buf_size=%d,result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3746_112_2_18_2_42_52_93,(uint8*)"ddd",efs_error,file_op_result->buf_size,result);
    if(efs_error==SFS_ERROR_NONE && result == file_op_result->buf_size)
    {
        file_op_result->result = CMSBRW_FS_WRITE_SUCCEED;
    }
    else
    {
        file_op_result->result = CMSBRW_FS_WRITE_FAIL;
    }
    //SCI_TRACE_LOW:"cyz file_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3755_112_2_18_2_42_52_94,(uint8*)"d",file_op_result->fileType);
    //SCI_TRACE_LOW:"param addr=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3756_112_2_18_2_42_52_95,(uint8*)"d",file_op_result);
    message.message_type = (uint32)READ_FILE_RESP;
    SCI_MEMCPY(&message.param,file_op_result,sizeof(MMIBRW_FILE_OP_PARM_T));
    SCI_FREE(file_op_result);
    MMK_PostMsg(VIRTUAL_WIN_ID,MSG_CMSBRW_BRW_OP,&message,sizeof(MMIBRW_VIRWIN_MESSAGE_T));
}


/*****************************************************************************/
//  Description :get flag lock
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_GetPushLockFlag(uint32 record_id)
{
    if (PNULL == g_push_info_ptr)
    {
        return FALSE;
    }
    if(record_id < MMICMSBRW_MAX_ITEM_NUM)
    {
        return (g_push_info_ptr->item_detail[record_id].is_locked);
    }
    
    //SCI_TRACE_LOW:"MMIAPIOTA_GetPushLockFlag record_id ERROR = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3779_112_2_18_2_42_52_96,(uint8*)"d", record_id);
    
    return FALSE;
}

/*****************************************************************************/
//  Description :set lock flag
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIOTA_SetPushLockFlag(uint32 record_id,BOOLEAN is_locked)
{
    if (PNULL == g_push_info_ptr)
    {
        return;
    }
    if(record_id<MMICMSBRW_MAX_ITEM_NUM)
    {
        g_push_info_ptr->item_detail[record_id].is_locked = is_locked;
    }
}

/*****************************************************************************/
//  Description :get wap push total num
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIOTA_GetPushTotalNum(void)
{
    if (PNULL == g_push_info_ptr)
    {
        return 0;
    }
    return g_push_info_ptr->item_num;
}

/*****************************************************************************/
//  Description :cat tow string
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN OTACatString(MMI_STRING_T *dst_ptr, MMI_STRING_T *src1_ptr, MMI_STRING_T *src2_ptr)
{
    uint32    str1_len      = 0;
    wchar    *str1_text_ptr = PNULL;    
    uint32    str2_len      = 0;
    wchar    *str2_text_ptr = PNULL;
    
    if(PNULL==dst_ptr || PNULL==src1_ptr || PNULL==src2_ptr)
    {
        return FALSE;
    }
    if (0 == src1_ptr->wstr_len && 0 == src2_ptr->wstr_len)
    {
        return FALSE;
    }    
    str1_text_ptr    = src1_ptr->wstr_ptr;
    str1_len        = src1_ptr->wstr_len;    
    str2_text_ptr    = src2_ptr->wstr_ptr;
    str2_len        = src2_ptr->wstr_len;
    //merge
    dst_ptr->wstr_len = str1_len + str2_len;    
    dst_ptr->wstr_ptr = SCI_ALLOCAZ( (dst_ptr->wstr_len + 1) * sizeof(wchar) );
    if (PNULL == dst_ptr->wstr_ptr)
    {
        return FALSE;
    }

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
//  Description :detail time
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void OTADetailTimeString(MMI_STRING_T *str_dst, uint32 time, MMI_TEXT_ID_T text_id)
{
    uint32 time_temp = 0;
    MMI_TM_T time_st  = {0};
    uint8 date[MMISET_DATE_STR_LEN + 1] = {0};
    uint8 time_str[MMISET_TIME_STR_12HOURS_LEN + 1] = {0};
    uint8 time_show[MMIOTA_TIME_STR_LEN + 1] = {0};
    wchar time_wch[MMIOTA_TIME_STR_LEN + 1]  = {0};
    MMI_STRING_T time_string = {0};
    uint16 time_str_len = 0;
    MMI_STRING_T line_str = {0};
    wchar line_wch[] = {':', '\n', 0};

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
    MMI_STRNTOWSTR(time_wch, MMIOTA_TIME_STR_LEN, (uint8 *)time_show, MMIOTA_TIME_STR_LEN, strlen((char *)time_show));
    time_str_len = MMIAPICOM_Wstrlen(time_wch);

    MMI_GetLabelTextByLang(text_id, &time_string);
    
    OTACatString(&time_string, &time_string, &line_str);
    str_dst->wstr_len = time_string.wstr_len + time_str_len;
    str_dst->wstr_ptr = (wchar *)SCI_ALLOCAZ((str_dst->wstr_len + 1) * sizeof(wchar));    
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr, time_string.wstr_ptr, time_string.wstr_len);
    MMIAPICOM_Wstrncpy(str_dst->wstr_ptr + time_string.wstr_len, time_wch, time_str_len);
    if (PNULL != time_string.wstr_ptr)
    {
        SCI_FREE(time_string.wstr_ptr);
        time_string.wstr_ptr = PNULL;
        time_string.wstr_len = 0;
    }

    return;
}

/*****************************************************************************/
//  Description :free item data
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void FreeItemDetailData(MMI_STRING_T *str_dst)
{
    if (PNULL == str_dst)
    {
        return;
    }
    if (PNULL != str_dst->wstr_ptr)
    {
        SCI_FREE(str_dst->wstr_ptr);
        str_dst->wstr_ptr = PNULL;
        str_dst->wstr_len = 0;
    }
}

/*****************************************************************************/
//  Description :show wap push richtext entity
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void WapPushShowEntry(MMI_CTRL_ID_T ctrl_id, uint16 push_index)
{
    GUIRICHTEXT_ITEM_T  item_data       = {0};   /*lint !e64*/
    uint16  index                       = 0;
    MMI_STRING_T  push_url_str          = {0};
    MMI_STRING_T  push_str              = {0};
    MMI_STRING_T  push_contenrt_str     = {0};
    MMI_STRING_T  temp_str              = {0};
    MMI_STRING_T line_str               = {0};
    MMI_STRING_T expire_time_str               = {0};
    wchar line_wch[]                    = {':', '\n', 0};
    wchar url_wstr[GUIRICHTEXT_URL_MAX_LEN + 1] = {0}; 
    uint16 url_len                      = 0;

    MMI_TM_T tmTime = {0};
    uint8    date_str[MMISET_DATE_STR_LEN + 1] = {0};
    uint8    time_str[MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};
    uint8    len  = 0;
    uint8    time_tmp[MMISET_DATE_STR_LEN + MMIALM_TIME_STR_12HOURS_LEN + 1] = {0};

    if (PNULL == g_push_info_ptr)
    {
        return;
    }

    line_str.wstr_ptr = line_wch;
    line_str.wstr_len = MMIAPICOM_Wstrlen(line_wch);
    
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
#ifdef MAINLCD_SIZE_128X64
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
#else
    item_data.tag_type = GUIRICHTEXT_TAG_PARSE_PHONENUM;

#if defined(MMS_SUPPORT) || defined(MMIEMAIL_SUPPORT)
    item_data.tag_type = item_data.tag_type | GUIRICHTEXT_TAG_PARSE_EMAIL;/*lint !e655*/
#endif

#ifdef BROWSER_SUPPORT
    item_data.tag_type = item_data.tag_type | GUIRICHTEXT_TAG_PARSE_URL;/*lint !e655*/
#endif    
#endif

#if defined(MMI_SMS_FONT_SETTING_SUPPORT)
    item_data.text_set_font = TRUE;    
    item_data.setting_font = MMIAPISMS_GetWinFont();
#endif
    //URL
    MMI_GetLabelTextByLang(TXT_URL_ADRESS, &push_url_str);
    push_str.wstr_len = push_url_str.wstr_len + line_str.wstr_len + g_push_info_ptr->item_detail[push_index].url_len;
    push_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((push_str.wstr_len + 1) * sizeof(wchar));
    if (PNULL == push_str.wstr_ptr)
    {
        return;
    }
    MMIAPICOM_Wstrncpy(push_str.wstr_ptr, push_url_str.wstr_ptr, push_url_str.wstr_len);
    MMIAPICOM_Wstrncpy(push_str.wstr_ptr + push_url_str.wstr_len, line_str.wstr_ptr, line_str.wstr_len);
    url_len = MIN(g_push_info_ptr->item_detail[push_index].url_len, GUIRICHTEXT_URL_MAX_LEN);
    MMI_STRNTOWSTR(url_wstr, GUIRICHTEXT_URL_MAX_LEN, (uint8 *)g_push_info_ptr->item_detail[push_index].url, url_len, url_len);
    MMIAPICOM_Wstrncpy(push_str.wstr_ptr + push_url_str.wstr_len + line_str.wstr_len, url_wstr, push_str.wstr_len - push_url_str.wstr_len - line_str.wstr_len);
    
    item_data.text_data.buf.len = push_str.wstr_len;
    item_data.text_data.buf.str_ptr = push_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(push_str.wstr_ptr);

    //CONTENT
    if (0 != g_push_info_ptr->item_detail[push_index].content_len)
    {
        MMI_GetLabelTextByLang(STXT_MAIN_MESSAGE, &temp_str);
        push_contenrt_str.wstr_len = temp_str.wstr_len + line_str.wstr_len + g_push_info_ptr->item_detail[push_index].content_len;
        push_contenrt_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((push_contenrt_str.wstr_len + 1) * sizeof(wchar));
        if (PNULL == push_contenrt_str.wstr_ptr)
        {
            return;
        }
        MMIAPICOM_Wstrncpy(push_contenrt_str.wstr_ptr, temp_str.wstr_ptr, temp_str.wstr_len);
        MMIAPICOM_Wstrncpy(push_contenrt_str.wstr_ptr + temp_str.wstr_len, line_str.wstr_ptr, line_str.wstr_len);
        MMIAPICOM_Wstrncpy(push_contenrt_str.wstr_ptr + temp_str.wstr_len + line_str.wstr_len, g_push_info_ptr->item_detail[push_index].content, push_contenrt_str.wstr_len - temp_str.wstr_len - line_str.wstr_len);
        
        item_data.text_data.buf.len = push_contenrt_str.wstr_len;
        item_data.text_data.buf.str_ptr = push_contenrt_str.wstr_ptr;
        GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        SCI_FREE(push_contenrt_str.wstr_ptr);
    }

 // time data
    tmTime = MMIAPICOM_Second2Tm(g_push_info_ptr->item_detail[push_index].time);
    MMIAPISET_FormatDateStrByDateEx(tmTime.tm_year,tmTime.tm_mon, tmTime.tm_mday,
        '-',date_str,MMISET_DATE_STR_LEN,TRUE);
    MMIAPISET_FormatTimeStrByTime(tmTime.tm_hour, tmTime.tm_min,time_str,MMISET_TIME_STR_12HOURS_LEN + 1);
    //Add item SMS time
    len = sprintf((char*)time_tmp, "%s %s",date_str,time_str);     

    push_str.wstr_len = len;
    push_str.wstr_ptr = (wchar *)SCI_ALLOCAZ((len+1) * sizeof(wchar));
    if (PNULL == push_str.wstr_ptr)
    {
        return;
    }
	
    MMI_STRNTOWSTR(
		push_str.wstr_ptr,
		len,
		(const uint8 *)time_tmp,
		len,
		len
		);
	
    item_data.text_data.buf.len = push_str.wstr_len;
    item_data.text_data.buf.str_ptr = push_str.wstr_ptr;
    GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
    SCI_FREE(push_str.wstr_ptr);

    //Expire time
    if (MMIOTA_BRW_WAP_SI == g_push_info_ptr->item_detail[push_index].wap_push_type
        && g_push_info_ptr->item_detail[push_index].expired_time > 0)
    {
        uint32 sys_time = MMIAPICOM_GetCurTime();

        SCI_MEMSET(&expire_time_str, 0x00, sizeof(expire_time_str));

        OTADetailTimeString(&expire_time_str, (uint32)g_push_info_ptr->item_detail[push_index].expired_time, TXT_OTA_SI_EXPIRED_TIME);

        if (sys_time > (uint32)g_push_info_ptr->item_detail[push_index].expired_time)//expired
        {
            MMI_STRING_T display_string = {0};
            MMI_STRING_T expired_string = {0};
            wchar lb_wch[] = {' ', '(', 0};
            wchar rb_wch[] = {')', 0};
            uint16 lb_wch_len = 0;
            uint16 rb_wch_len = 0;

            MMI_GetLabelTextByLang(TXT_COMMON_EXPIRED, &expired_string);

            lb_wch_len = MMIAPICOM_Wstrlen(lb_wch);
            rb_wch_len = MMIAPICOM_Wstrlen(rb_wch);

            display_string.wstr_len = expire_time_str.wstr_len+ lb_wch_len + expired_string.wstr_len + rb_wch_len;
            display_string.wstr_ptr = SCI_ALLOCAZ((display_string.wstr_len + 1)*sizeof(wchar));

            if (PNULL == display_string.wstr_ptr)
            {
                FreeItemDetailData(&expire_time_str);

                return;
            }

            MMI_WSTRNCPY(display_string.wstr_ptr, display_string.wstr_len,
                                    expire_time_str.wstr_ptr, expire_time_str.wstr_len, expire_time_str.wstr_len);

            MMI_WSTRNCPY(display_string.wstr_ptr + expire_time_str.wstr_len, display_string.wstr_len,
                                    lb_wch, lb_wch_len, lb_wch_len);

            MMI_WSTRNCPY(display_string.wstr_ptr + expire_time_str.wstr_len + lb_wch_len, display_string.wstr_len,
                                    expired_string.wstr_ptr, expired_string.wstr_len, expired_string.wstr_len);

            MMI_WSTRNCPY(display_string.wstr_ptr + expire_time_str.wstr_len + lb_wch_len + expired_string.wstr_len, display_string.wstr_len,
                                    rb_wch, rb_wch_len, rb_wch_len);

            item_data.text_data.buf.len = display_string.wstr_len;
            item_data.text_data.buf.str_ptr = display_string.wstr_ptr;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);

            MMIPUB_OpenAlertWarningWin(TXT_COMMON_MESSAGE_EXPIRED);

            SCI_FREE(display_string.wstr_ptr);
        }
        else
        {
            item_data.text_data.buf.len = expire_time_str.wstr_len;
            item_data.text_data.buf.str_ptr = expire_time_str.wstr_ptr;
            GUIRICHTEXT_AddItem(ctrl_id, &item_data, &index);
        }

        FreeItemDetailData(&expire_time_str);
    }

#ifdef MAINLCD_SIZE_128X64
    GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_NONE);
#else
    GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
#endif
}


/*****************************************************************************/
//  Description :show wap push handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleShowPushmsgWinMsg(
                                           MMIOTA_WINDOW_ID_E	win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM				param
                                           )
{
    MMI_RESULT_E	recode = MMI_RESULT_TRUE;
    uint16			list_index = 0;
    int32			i = 0;
    GUIRICHTEXT_FOCUS_ELEM_T richtext_element = {0};/*lint !e64*/
    GUIRICHTEXT_FOCUS_TYPE_T focus_type = GUIRICHTEXT_FOCUS_NONE;
    uint8 *focus_data_str_ptr = PNULL;
    uint16 index = 0;

    if (PNULL == g_push_info_ptr)
    {
        return recode;
    }
    
    //SCI_TRACE_LOW:"HandlePushmsgWinMsg() msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_3904_112_2_18_2_42_53_97,(uint8*)"d", msg_id);
    list_index = (uint32)(MMK_GetWinAddDataPtr(win_id));
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        if (is_idle_read_push)
        {
#ifndef MMI_PDA_SUPPORT        
            GUIWIN_SetTitleRollArrow(win_id, FALSE);
#endif
            is_idle_read_push = FALSE;
        }
        else
        {
#ifndef MMI_PDA_SUPPORT
            GUIWIN_SetTitleRollArrow(win_id, TRUE);
#endif
        }
        MMK_SetAtvCtrl(MMICMSBRW_SHOWPUSHMSG_WIN_ID,  MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID);
        //GUIRICHTEXT_SetFocusMode(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID, GUIRICHTEXT_FOCUS_SHIFT_BASIC);
        //GUIRICHTEXT_SetItemSpace(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID, 0);
        //GUIRICHTEXT_SetTxtMargin(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID, 0);
        i=list_index;

        WapPushShowEntry(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID, list_index);        

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif
        break;

#ifndef MMI_PDA_SUPPORT
    case MSG_FULL_PAINT:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_KEY:
    case MSG_CTL_RICHTEXT_SWITCHFOCUS_BY_TP:
        {
            BOOLEAN is_need_update = FALSE;

            GUIRICHTEXT_GetFocusElement(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID, &richtext_element);
            focus_type = richtext_element.type;

            if (MSG_FULL_PAINT == msg_id)
            {
                is_need_update = FALSE;
            }
            else
            {
                is_need_update = TRUE;
            }
            
            switch(focus_type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_COMMON_NUMBER, STXT_RETURN, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_URL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_URL_ADRESS, STXT_RETURN, is_need_update);
                break;

            case GUIRICHTEXT_FOCUS_EMAIL:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_EMAIL_BOX, STXT_RETURN, is_need_update);
                break;

            default:
                GUIWIN_SetSoftkeyTextId(win_id, STXT_OPTION, TXT_NULL, STXT_RETURN, is_need_update);
                break;
            }
        }
        break;
#endif

    case MSG_KEYUP_CALL1:
    case MSG_KEYUP_CALL2:
    case MSG_KEYUP_CALL3:
    case MSG_KEYUP_CALL4:
        {
#ifdef MMI_MULTI_GREEN_KEY_SINGLE
            CC_CALL_SIM_TYPE_E sim_type = CC_MAJOR_SIM_CALL;
#else
            CC_CALL_SIM_TYPE_E sim_type = MMIAPICC_CheckCallSimType(msg_id, CC_CALL_SIM_MAX);
#endif
            uint32 call_type = CC_CALL_NORMAL_CALL;
            
            GUIRICHTEXT_GetFocusElement(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID, &richtext_element);
            focus_type = richtext_element.type;

            if (focus_type == GUIRICHTEXT_FOCUS_PHONENUM && list_index < MMICMSBRW_MAX_ITEM_NUM)
            {
                uint16 min_num_len = MIN(richtext_element.data.len,MMISMS_PBNUM_MAX_LEN);
                CC_RESULT_E cc_result = CC_RESULT_SUCCESS;

                focus_data_str_ptr = SCI_ALLOCAZ(min_num_len + 1);
                if (PNULL == focus_data_str_ptr)
                {
                    break;
                }
                MMI_WSTRNTOSTR((uint8 *)focus_data_str_ptr, MMISMS_PBNUM_MAX_LEN, richtext_element.data.str_ptr, richtext_element.data.len, min_num_len);

                cc_result = MMIAPICC_MakeCall(
                                                                g_push_info_ptr->item_detail[list_index].dual_sys,
                                                                focus_data_str_ptr,
                                                                richtext_element.data.len,
                                                                PNULL,
                                                                PNULL,
                                                                sim_type,
                                                                call_type,
                                                                PNULL
                                                                );

                SCI_FREE(focus_data_str_ptr);
                focus_data_str_ptr = PNULL;
            }
        }
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if (list_index < MMICMSBRW_MAX_ITEM_NUM)
        {
            GUIRICHTEXT_GetFocusElement(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID, &richtext_element);
            focus_type = richtext_element.type;
            GUIRICHTEXT_GetFocusItemIndex(MMIOTA_TEXT_PUSHMSGDETAIL_CTRL_ID, &index);

            switch(focus_type) 
            {
            case GUIRICHTEXT_FOCUS_PHONENUM:
                MMIAPICOM_OpenExtractNumberWin(MMIOTA_EXTRACT_NUMBER_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, g_push_info_ptr->item_detail[list_index].dual_sys);
                break;

            case GUIRICHTEXT_FOCUS_URL:
                MMIAPICOM_OpenExtractUrlWin(MMIOTA_EXTRACT_URL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, g_push_info_ptr->item_detail[list_index].dual_sys);
                break;

            case GUIRICHTEXT_FOCUS_EMAIL:
                MMIAPICOM_OpenExtractEmailWin(MMIOTA_EXTRACT_EMAIL_POP_WIN_ID, richtext_element.data.str_ptr, richtext_element.data.len, g_push_info_ptr->item_detail[list_index].dual_sys);
                break;
         
            case GUIRICHTEXT_FOCUS_NONE:
                MMK_CreateWin((uint32*)MMICMSBRW_PUSHMSGMENU_WIN_TAB, (ADD_DATA)list_index);       
                break;

            default:
                break;
            }
        }
        break;

    case MSG_APP_LEFT:
    case MSG_APP_RIGHT:
    case MSG_CTL_TITLE_MOVE_NEXT:
    case MSG_CTL_TITLE_MOVE_PREV:
        if (MSG_CTL_TITLE_MOVE_NEXT == msg_id)
        {
            msg_id = MSG_APP_RIGHT;
        }
        else if (MSG_CTL_TITLE_MOVE_PREV == msg_id)
        {
            msg_id = MSG_APP_LEFT;
        }
        if (MMK_IsOpenWin(MMISMS_MSGBOX_MAIN_WIN_ID))
        {           
            MMIAPISMS_HandleLeftRightMsgInShowSmsWin(win_id, msg_id);
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_MENU:
        MMK_CreateWin((uint32*)MMICMSBRW_PUSHMSGMENU_WIN_TAB, (ADD_DATA)list_index);       
        break;
        
    case MSG_PROMPTWIN_OK:
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        //移动push
        if (MMISMS_FOLDER_NORMAL == g_push_info_ptr->item_detail[list_index].folder_type)
        {
            MMIAPIMMS_MoveSecurityPush(list_index, TRUE);
        }
        else
        {
            MMIAPIMMS_MoveSecurityPush(list_index, FALSE);
        }
#endif
        MMIPUB_CloseQuerytWin(PNULL);            
        if (MMK_IsOpenWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID))
        {
            MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
        }
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        break;
        
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break; 
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
    case MSG_CLOSE_WINDOW:
        if (MMK_IsOpenWin(MMICMSBRW_PUSHMSGMENU_WIN_ID))
        {
            MMK_CloseWin(MMICMSBRW_PUSHMSGMENU_WIN_ID);
        }
#ifndef MMI_PDA_SUPPORT
        if (1 == g_push_info_ptr->item_detail[list_index].is_unload)
        {
            MMIOTA_WapPushUpdateState(list_index);
        }
#endif
        break;
        
    default:	
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}

/*****************************************************************************/
//  Description :show wap push menu opt menu handle fun
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandlePushmsgMenuWinMsg(
                                           MMIOTA_WINDOW_ID_E		win_id, 
                                           MMI_MESSAGE_ID_E	msg_id, 
                                           DPARAM				param
                                           )
{
    MMI_RESULT_E		recode = MMI_RESULT_TRUE;
    uint32				group_id = 0;
    uint32				menu_id = 0;
    uint16				list_index = 0;
    int32				i = 0;
    int32				j = 0;
    // BOOLEAN				acc_result = FALSE;
    BOOLEAN             is_locked  = FALSE;
    MMICMSBRW_ITEM_T *push_msg_list = PNULL;
    MMISMS_FOLDER_TYPE_E folder_type = MMISMS_FOLDER_NORMAL;
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ERROR_E  enter_err = MMIBROWSER_SUCC;
    MMIBROWSER_ENTRY_PARAM brw_parm = {0};/*lint !e64*/
#endif

    if (PNULL == g_push_info_ptr)
    {
        return recode;
    }
    push_msg_list = g_push_info_ptr;

    //SCI_TRACE_LOW:"HandlePushmsgMenuWinMsg() msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_4040_112_2_18_2_42_53_98,(uint8*)"d", msg_id);
    
    list_index = (uint32)(MMK_GetWinAddDataPtr(win_id));
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            /* modified by plum peng for cr125185 begin */
            is_locked = MMIAPIOTA_GetPushLockFlag(list_index);
            folder_type = g_push_info_ptr->item_detail[list_index].folder_type;
            //SCI_TRACE_LOW:"HandlePushmsgMenuWinMsg() open"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_4051_112_2_18_2_42_53_99,(uint8*)"");
            if (MMIAPIDC_IsOpened() || !MMIAPIMMS_IsPdpPermitUsing((MN_DUAL_SYS_E)g_push_info_ptr->item_detail[list_index].dual_sys))
            {
                //SCI_TRACE_LOW:"HandlePushmsgMenuWinMsg() true"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_4054_112_2_18_2_42_53_100,(uint8*)"");
                GUIMENU_SetItemGrayed(MMIOTA_MENU_PUSHMSG_CTRL_ID, MENU_CMSBRW_PUSHMSG, ID_CMSBRW_PUSHMSG_ENTER, TRUE);
            }
            else
            {
                //SCI_TRACE_LOW:"HandlePushmsgMenuWinMsg() false"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_4059_112_2_18_2_42_53_101,(uint8*)"");
                GUIMENU_SetItemGrayed(MMIOTA_MENU_PUSHMSG_CTRL_ID, MENU_CMSBRW_PUSHMSG, ID_CMSBRW_PUSHMSG_ENTER, FALSE);
            }            

            if (!is_locked)
            {
                GUIMENU_SetItemGrayed(MMIOTA_MENU_PUSHMSG_CTRL_ID, MENU_CMSBRW_PUSHMSG,ID_CMSBRW_PUSHMSG_DELETE,FALSE);
            }
            else
            {
                GUIMENU_SetItemGrayed(MMIOTA_MENU_PUSHMSG_CTRL_ID, MENU_CMSBRW_PUSHMSG,ID_CMSBRW_PUSHMSG_DELETE,TRUE);
            }

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
            if (MMISMS_FOLDER_SECURITY == folder_type)
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_PUSH_SECURITY_SELECT,ID_PUSH_MENU_BOX_SECURITY_ITEM,TRUE);
                MMIAPICOM_EnableGrayed(win_id,MENU_PUSH_SECURITY_SELECT,ID_PUSH_MENU_BOX_PREVIEW_ITEM,FALSE);
            }
            else
            {
                MMIAPICOM_EnableGrayed(win_id,MENU_PUSH_SECURITY_SELECT,ID_PUSH_MENU_BOX_SECURITY_ITEM,FALSE);
                MMIAPICOM_EnableGrayed(win_id,MENU_PUSH_SECURITY_SELECT,ID_PUSH_MENU_BOX_PREVIEW_ITEM,TRUE);
            }
#endif
            /* added by plum peng for cr125185 end */
            MMK_SetAtvCtrl(win_id,MMIOTA_MENU_PUSHMSG_CTRL_ID);
            GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            break;
        }
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUIMENU_GetId( MMIOTA_MENU_PUSHMSG_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));  
        switch(menu_id)//菜单节点ID
        {
        case ID_CMSBRW_PUSHMSG_ENTER://进入:
            //set the tag of is_load
            i = list_index;
            if(i < MMICMSBRW_MAX_ITEM_NUM)
            {		
                uint32 sys_time = MMIAPICOM_GetCurTime();

#if 0
                //g_cmsbrw_pushmsg_info.item_detail[i].is_unload = 0;//set to having loaded
                MMIOTA_WriteWapPushFile(win_id,
                    MMICMSBRW_WAP_PUSHMSG_FILE_NAME,
                    (uint8*)g_push_info_ptr,
                    sizeof(MMICMSBRW_ITEM_T),
                    VIRTUAL_WIN_ID,
                    FALSE,
                    CMSBRW_FS_PUSHMSG_DELETE_STRUCT_MEMBER_TYPE);
#endif

                if (MMIOTA_BRW_WAP_SI == g_push_info_ptr->item_detail[i].wap_push_type
                    && g_push_info_ptr->item_detail[i].expired_time > 0
                    && sys_time > (uint32)g_push_info_ptr->item_detail[i].expired_time)//expired
                {
                    MMIPUB_OpenAlertWarningWin(TXT_COMMON_MESSAGE_EXPIRED);

                    MMK_CloseWin(win_id);
                    break;
                }

#ifdef BROWSER_SUPPORT
                brw_parm.dual_sys = (MN_DUAL_SYS_E)g_push_info_ptr->item_detail[i].dual_sys;
                brw_parm.url_ptr = (char *)g_push_info_ptr->item_detail[i].url;
                brw_parm.type = MMIBROWSER_ACCESS_URL;
                brw_parm.user_agent_ptr = PNULL;
#ifdef CMCC_UI_STYLE        
                brw_parm.is_alert= TRUE;
#endif
                enter_err = MMIAPIBROWSER_Entry(&brw_parm);
                if (MMIBROWSER_SUCC == enter_err)
                {
                    MMK_CloseWin(win_id);
                }
#endif
            }
            break;   
            
        case ID_CMSBRW_PUSHMSG_DELETE://删除       
            MMIPUB_OpenQueryWinByTextId(TXT_DELETE_QUERY,IMAGE_PUBWIN_QUERY,PNULL,PNULL);//打开询问是否删除
            break;
            
#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
        case ID_PUSH_MENU_BOX_SECURITY_ITEM:
        case ID_PUSH_MENU_BOX_PREVIEW_ITEM:
            MMK_CloseWin(MMICMSBRW_PUSHMSGMENU_WIN_ID);
            MMIPUB_OpenQueryWinByTextId(TXT_MOVE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);            
            break;
#endif
            
        default:
            break;
        }
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);//返回上一级
        break;
        
    case MSG_PROMPTWIN_OK:
        //删除pushmsg
        GUIMENU_GetId( MMIOTA_MENU_PUSHMSG_CTRL_ID, (MMI_MENU_GROUP_ID_T *)(&group_id),(MMI_MENU_ID_T *) (&menu_id));  
        switch(menu_id)//菜单节点ID
        {
        case ID_CMSBRW_PUSHMSG_DELETE://删除:
            i=list_index;
            //SCI_TRACE_LOW:"ID_CMSBRW_PUSHMSG_DELETE index =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_4153_112_2_18_2_42_53_102,(uint8*)"d",list_index);
            for(j=0;j<MMICMSBRW_MAX_ITEM_NUM;j++)
            {
                if(list_index < g_push_info_ptr->item_detail[j].list_num)
                {
                    g_push_info_ptr->item_detail[j].list_num--;
                }
            }
            if (g_push_info_ptr->item_num == 0)
            {
                for(j = 0; j < MMICMSBRW_MAX_ITEM_NUM; j++)
                {
                    if(push_msg_list->item_detail[j].list_num != -1)
                    {
                        //SCI_TRACE_LOW:"ID_CMSBRW_PUSHMSG_DELETE index =%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIOTA_WINTABLE_4167_112_2_18_2_42_53_103,(uint8*)"d",j);
                        MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH,j);
                        SCI_MEMSET(&push_msg_list->item_detail[j], 0, sizeof(MMICMSBRW_ITEM_DETAIL_T));
                        push_msg_list->item_detail[j].list_num = -1;
                    }
                }
                MMIAPIOTA_SavePushFileInfo();
            }
            else if(i != -1)
            {			
                SCI_MEMSET(&g_push_info_ptr->item_detail[i], 0, sizeof(MMICMSBRW_ITEM_DETAIL_T));
                g_push_info_ptr->item_detail[i].list_num = -1;
                g_push_info_ptr->item_num--;
                //保存pushmsg的File
                MMIOTA_WriteWapPushFile(win_id,
                    MMICMSBRW_WAP_PUSHMSG_FILE_NAME,
                    (uint8*)g_push_info_ptr,
                    sizeof(MMICMSBRW_ITEM_T),
                    VIRTUAL_WIN_ID,
                    FALSE,
                    CMSBRW_FS_PUSHMSG_DELETE_STRUCT_MEMBER_TYPE);
                MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH,list_index);
                
            }
            MMIPUB_CloseQuerytWin(PNULL);

#ifdef MMI_GUI_STYLE_TYPICAL
            //提示用户
            MMIPUB_OpenAlertSuccessWin(TXT_DELETED);        
#endif
            MMK_CloseWin(MMICMSBRW_PUSHMSGMENU_WIN_ID);
            MMK_CloseWin(MMICMSBRW_SHOWPUSHMSG_WIN_ID);
            break;        
        default:
            break;
        }        
        break;        
    case MSG_PROMPTWIN_CANCEL:
         MMIPUB_CloseQuerytWin(PNULL);
         MMK_CloseWin(win_id);
         break;            
    default:	
        recode = MMI_RESULT_FALSE;
        break;
  }
    return recode;
}

#if defined (MMI_SMS_SECURITYBOX_SUPPORT)
/*****************************************************************************/
//  Description : set ota security flag
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_MoveSecurityOTA(uint32 index,
                                         BOOLEAN is_to_security)
{
    BOOLEAN ret_value = FALSE;

    if (PNULL == g_ota_config_ptr)
    {
        return ret_value;
    }
    if (is_to_security)
    {
        if ((index < OTA_CONFIG_INFO_NUM_MAX) && 
            (MMISMS_FOLDER_NORMAL == g_ota_config_ptr->ota_config_info[index].folder_type))
        {
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_OTA, index);
            g_ota_config_ptr->ota_config_info[index].folder_type = MMISMS_FOLDER_SECURITY;
            MMIOTA_InsertOTAToSms(index);
            MMIAPIOTA_UpdateOTAFileInfo();  
            ret_value = TRUE;
        }
    }
    else
    {
        if ((index < OTA_CONFIG_INFO_NUM_MAX) && 
            (MMISMS_FOLDER_SECURITY == g_ota_config_ptr->ota_config_info[index].folder_type))
        {
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_OTA, index);
            g_ota_config_ptr->ota_config_info[index].folder_type = MMISMS_FOLDER_NORMAL;
            MMIOTA_InsertOTAToSms(index);
            MMIAPIOTA_UpdateOTAFileInfo();  
            ret_value = TRUE;
        }
    }
    return ret_value;
}

/*****************************************************************************/
//  Description : set push security flag
//  Global resource dependence : none
//  Author: minghu.mao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMMS_MoveSecurityPush(uint32 index,
                                          BOOLEAN is_to_security)
{
    BOOLEAN ret_value = FALSE;

    if (PNULL == g_push_info_ptr)
    {
        return FALSE;
    }
    if (is_to_security)
    {
        if (MMISMS_FOLDER_NORMAL == g_push_info_ptr->item_detail[index].folder_type)
        {
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH, index);
            g_push_info_ptr->item_detail[index].folder_type = MMISMS_FOLDER_SECURITY;
            MMIOTA_InsertWapPushToSms(index);
            MMIAPIOTA_SavePushFileInfo(); 
            ret_value = TRUE;
        }
    }
    else
    {
        if (MMISMS_FOLDER_SECURITY == g_push_info_ptr->item_detail[index].folder_type)
        {
            MMIAPISMS_DeleteMsgInorder(MMISMS_TYPE_WAP_PUSH, index);
            g_push_info_ptr->item_detail[index].folder_type = MMISMS_FOLDER_NORMAL;
            MMIOTA_InsertWapPushToSms(index);
            MMIAPIOTA_SavePushFileInfo();    
            ret_value = TRUE;
        }
    }
    return ret_value;
}
#endif

/*****************************************************************************/
//  Description : is in ota moudle window
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOTA_IsInOTAWin(void)
{
    MMI_WIN_ID_T win_id = 0;
    BOOLEAN is_in_ota_win = FALSE;

    for (win_id = MMIOTA_WIN_ID_START; win_id < MMIOTA_MAX_WIN_ID; win_id++)
    {
        if (MMK_IsOpenWin(win_id) && !MMK_IsOpenWin(MMICMSBRW_NEWPUSH_WIN_ID))
        {
            is_in_ota_win = TRUE;
            break;
        }
    }

    return is_in_ota_win;
}

/*****************************************************************************/
//  Description : is forbit new msg win
//  Global resource dependence : 
//  Author:fengming.huang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsForbitNewMsgWin(void)
{
    if( MMIAPIUdisk_IsOpenPcCamera() 
#ifdef MMI_RECORD_SUPPORT
        || MMIAPIRECORD_IsRecording() 
#endif
#ifdef ATV_SUPPORT
        || MMIAPIATV_IsRecording()
#endif
#ifdef VIDEO_PLAYER_SUPPORT
        || MMIAPIVP_IsLiving()
#endif       
#ifdef MOBILE_VIDEO_SUPPORT
        || MMIAPIMV_IsLiving()
#endif
#if !defined(CMCC_TEST_FLAG)
        || MMIAPIDC_IsOpened()
#endif
        || MMK_IsFocusWin(MMIKL_CLOCK_DISP_WIN_ID)
#ifdef   MMI_PDA_SUPPORT
#ifdef MMI_KEY_LOCK_EFFECT_PSW_SUPPORT
        || MMK_IsFocusWin(MMIKL_PSW_WIN_ID)
#endif
#endif
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************/
//  Description : Select SIM API of OTA app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIOTA_SelectSIMFunc(ADD_DATA    add_data)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T    selectSimWinID = MMIOTA_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)MMISET_APPEND_SIM_OK;
    caller_data.callback_func =OTASelectSimCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;

    caller_data.publist_id = MMIOTA_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = add_data;
    caller_data.extra_data = PNULL;

    MMIAPIPHONE_OpenSelectSimWin(&caller_data);
    

#endif
    return MMI_RESULT_TRUE;
}
#endif


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:52 2012*/ //IGNORE9527
