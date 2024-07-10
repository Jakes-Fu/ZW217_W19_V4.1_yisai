
/******************************************************************************
文件名称:mmipim_wintab.c
文件描述:UI窗口管理
文件说明:
开发人员:王晓林
首次开发日期:2007-9.26
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2007-09-26         王晓林                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/


#define _MMIPIM_WINTAB_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pim_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#include "window_parse.h"
#include "mmk_app.h" 
#include "guiim.h"
#include "guimenu.h"
//#include "guieditbox.h"
#include "guilistbox.h"
#include "guimsgbox.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmk_timer.h"
#include "mmi_menutable.h"

#include "mn_api.h"
#include "mmi_text.h"

#include "mmi_default.h"
#include "mmidisplay_data.h"

#include "guitext.h"
#include "mmi_textfun.h"

//#include "guibutton.h"
#include "mmipb_id.h"
#include "mmipb_export.h" 
#include "mmipub.h"

#include "mmipim.h"
#include "mmipim_menutable.h"
#include "pimdatasynctask.h"
#include "pimsyncmlcomm.h"
#include "mmipim_wintab.h"
#include "mmipim_text.h"
#include "mmipim_nv.h" // chenxiang 20071101 pim_setting
#include "pimsmlerr.h"
#include "mmipim_pb.h"

#include"mmi_appmsg.h"

#ifdef FEA_HS_APP_DCD//added by liangshun 20090804
#include "hisense_dcd.h"
#endif	

#ifdef FEA_HS_DM
#include "mmidm_api.h"
#endif
#include "mmipb_export.h"

#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmipim.h"//wangzhefeng add 20090910
//#include "chng_freq.h" //add by wangxiaolin
#include "mmipim_id.h"
#include "guiwin.h"
#include "guiedit.h"
#include "guires.h"
#include "Vcalendar_parse.h"
#ifdef FEA_HS_PB_NEW_STYLE // wangxiaolin 20091220
//#include "mmipb_internal.h"
#endif
#include "mmi_custom_define.h"
#include "mmitheme_label.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "guictrl_api.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define	MMIPIM_SYNC_FILL_RECT()                               \
{                                                       \
	GUI_RECT_T rect;                                    \
	GUI_LCD_DEV_INFO	lcd_dev_info = {0};				\
	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;				\
	lcd_dev_info.block_id = GUI_BLOCK_MAIN;	\
	rect.left = 0;                                      \
	rect.top = MMI_TITLE_HEIGHT;                        \
	rect.right = (MMI_MAINSCREEN_WIDTH - 1);         \
	rect.bottom = (MMI_MAINSCREEN_HEIGHT - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT);       \
	LCD_FillRect(&lcd_dev_info, rect, MMI_WINDOW_BACKGROUND_COLOR); \
}                  

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef enum
{
    PIM_CUSTOM_NONE,
    PIM_CUSTOM_MAN,
    PIM_CUSTOM_OEM,
    PIM_CUSTOM_MOD,
    PIM_CUSTOM_UA,
    PIM_CUSTOM_MAX
} MMIPIM_CUSTOM_UA_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
 
extern PIM_T g_pim;
extern  task_relay_info_type*     hs_task_relay_info ;
extern MMI_STRING_T pim_note_s;
extern  task_protocol_step_type* hs_protocol_step;
#ifdef HS_PIM_DISPALY_MALLOC_NUM
extern uint32 sml_malloc_num; // chenxiang_memory 20080314
#endif
//extern BOOLEAN g_is_pim_sync;//是否正在同步//del by wangxiaolin 
//extern PUBLIC int HS_PIM_socket_select_handle(void);


 /*extern void MMIAPISET_AppendListItemByTextId(									
                                   MMI_TEXT_ID_T        text_id,
                                   MMI_TEXT_ID_T        left_softkey_id,
                                   MMI_CTRL_ID_T		ctrl_id,
                                   GUIITEM_STYLE_E      item_style
                                   );*/
 
/*#ifndef WIN32
extern uint32 MMI_GetFreqHandler(void);
#endif*/
 /**-------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 #if 0 // chenxiang 20080806
char phone_replace[8]={0x7E, 0xC8, 0x7A, 0xEF, 0x66, 0xF4, 0x65,0xB0}; // 终端更新
char phone_delete[8]={0x7E,0xC8, 0x7A, 0xEF, 0x52, 0x20, 0x96, 0x64}; // 终端删除
char phone_add[8]={0x7E,0xC8, 0x7A, 0xEF, 0x58, 0x9E, 0x52, 0xA0};  // 终端添加
char server_replace[10]={0x67, 0x0D, 0x52, 0xA1, 0x56, 0x68, 0x66, 0xF4, 0x65,0xB0}; // 服务器更新
char server_delete[10]={0x67, 0x0D, 0x52, 0xA1, 0x56, 0x68, 0x52, 0x20, 0x96, 0x64}; // 服务器删除
char server_add[10]={0x67, 0x0D, 0x52, 0xA1, 0x56, 0x68, 0x58, 0x9E, 0x52, 0xA0};  // 服务器添加
char send_progress[6]={0x4E, 0x0A, 0x4F, 0x20, 0x00, 0x3A};    // 上传:
char receive_progress[6]={0x4E, 0x0B, 0x8F, 0x7D, 0x00, 0x3A}; // 下载:
char pim_item[2]={0x67, 0x61}; // 条
char sequence[2]={0x7B, 0x2C}; // 第
#endif
wchar divide_char[1]={0x002F};//  /
wchar colon_char[1]={0x003A}; //  :
wchar zuokuohao[1]={0x0028}; //  (
wchar youkuohao[1]={0x0029}; //  )

LOCAL MMIPIM_CUSTOM_UA_TYPE_E s_custom_ua_type = PIM_CUSTOM_NONE;

#ifndef WIN32  
LOCAL uint32 s_mmipim_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
#endif
LOCAL MN_DUAL_SYS_E  s_pim_sim=MN_DUAL_SYS_1;
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*==========================================================
 * 开发人员	: wangxiaolin      
 * 窗口名称	: HandlePIMMainMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家窗口处理函数
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMMainMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);


/*==========================================================
 * 开发人员	: wangxiaolin      
 * 窗口名称	: HandlePIMSyncEnterMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家次级窗口处理函数
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncEnterMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);


/*==========================================================
 * 开发人员	: wangxiaolin      
 * 窗口名称	: HandlePIMSyncBeginWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家同步处理及显示窗口
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncBeginWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家设定窗口处理函数  chenxiang 20071030 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMNetSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网络承载设定窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMNetSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMGateWayMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网关设定窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMGateWayMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMProxyMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家使用代理窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMProxyWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家SyncML协议参数窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandleDebugSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定窗口处理函数  chenxiang 20080318
 ==========================================================*/
LOCAL MMI_RESULT_E  HandleDebugSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);
/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandleDebugServerSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家调试参数设定-- 预置服务器窗口处理函数  chenxiang 20080318 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandleDebugServerSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMWapUrlMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMWapUrlMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingCodeWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingCodeWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingAuthWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingAuthWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16 for 
/*==========================================================
 * author		: wangxiaolin      
 * function 	: LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingSyncTypeWinMsg(
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/16/2009 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingSyncTypeWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */
#ifdef HS_PIM_UA_SETTING
/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingUAWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家窗口处理函数  chenxiang 20080318
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingUAWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

/*****************************************************************************/
//  Description : handle debug setting menu win msg
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  PimCustomUASetMenuWinHandleMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

/*****************************************************************************/
// 	Description : PimCustomUASetParaWinHandleMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E PimCustomUASetParaWinHandleMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param
                                                  );
/*****************************************************************************/
//  Description : set pim para info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void PimSetCustomUAParaInfo(MMIPIM_CUSTOM_UA_TYPE_E type,char* string,uint32 len);

/*****************************************************************************/
//  Description : Get PIM para info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void PimGetCustomUAParaInfo(MMIPIM_CUSTOM_UA_TYPE_E type,char* string);
#endif
/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMNetSettingAPNWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网络承载设定--APN 窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMNetSettingAPNWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMNetSettingUserWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网络承载设定--用户名 窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMNetSettingUserWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMNetSettingPasswordWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网络承载设定--密码窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMNetSettingPasswordWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMGateWayIpWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网关设定--IP 窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMGateWayIpWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMGateWayPortWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网关设定--端口窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMGateWayPortWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMGateWayUserWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网关设定--用户名窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMGateWayUserWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMGateWayPasswordWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网关设定--密码窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMGateWayPasswordWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingServerWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家SyncML协议参数--PIM服务器窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingServerWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingPortWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家SyncML协议参数--端口窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingPortWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingUserWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家SyncML协议参数--用户名窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingUserWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingPasswordWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家SyncML协议参数--用户名窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingPasswordWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingPBWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家SyncML协议参数--用户名窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingPBWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingPBWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家SyncML协议参数--用户名窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingVCALWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );
#endif /* HS_PIM_VCAL */


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMWapUrlRegCancelWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定--业务开通/ 取消 窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMWapUrlRegCancelWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMWapUrlNetQueryWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定--网络查询窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMWapUrlNetQueryWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMWapUrlPayIndexWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定--资费说明窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMWapUrlPayIndexWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 );

/*==========================================================
 * author		: wangxiaolin      
 * function 	: HandlePIMIntroduceWinMsg
 * decr		: 业务介绍窗口
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  10/16/2009 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMIntroduceWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

/*==========================================================
 * author		: renyi.hu     
 * function 	: HandlePIMSyncMLSettingMenuSyncTypeWinMsg
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/16/2009 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingMenuSyncTypeWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										);

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
MMI_RESULT_E HandlePIMSIMSelectWinMsg(
    MMI_WIN_ID_T     win_id, 
    MMI_MESSAGE_ID_E    msg_id, 
    DPARAM              param
    );

/*****************************************************************************/
//  Description :设置当前使用的sim 卡
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void MMIPIM_SetSim(MN_DUAL_SYS_E sim);

/*****************************************************************************/
//  Description :打开同步窗口
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void MMIPIM_OpenSyncWin(void);

/*****************************************************************************/
//  Description : Select SIM API of PIM app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIPIM_SelectSIMFunc(void);


/*****************************************************************************/
//  Description : Get sim select result
//  Global resource dependence : 
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
MMI_RESULT_E PIMSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);

/*---------------------------------------------------------------------------*/
/*                          WINDOWS DEFINITION                           */
/*---------------------------------------------------------------------------*/

//号簿管家主菜单窗口
WINDOW_TABLE(MMIPIM_MAINMENU_WIN_TAB) = 
{
    WIN_STATUSBAR,
	WIN_TITLE(TXT_PIM),
	WIN_FUNC((uint32)HandlePIMMainMenuWinMsg),    
	WIN_ID(MMIPIM_MAINMENU_WIN_ID),
	CREATE_MENU_CTRL(MMI_PIM_MAIN_MENU, MMIPIM_MAINMENU_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN), 
	//WIN_TIPS,
	END_WIN
};



//号簿管家次级主菜单窗口
WINDOW_TABLE(MMIPIM_SYNC_ENTER_MENU_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
	WIN_FUNC((uint32)HandlePIMSyncEnterMenuWinMsg), 
	WIN_TITLE(TXT_PIM),
	WIN_ID(MMIPIM_SYNC_ENTER_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MMI_PIM_SYNC_ENTER_MENU, MMIPIM_SYNC_ENTER_MENU_CTRL_ID),
	END_WIN
};



//号簿管家设定菜单窗口 chenxiang 20071030 pim_setting
WINDOW_TABLE(MMIPIM_SETTING_MENU_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
	WIN_TITLE(STXT_MAIN_SETTING),
	WIN_FUNC((uint32)HandlePIMSyncSettingMenuWinMsg),    
	WIN_ID(MMIPIM_SETTING_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MMI_PIM_SETTING_MENU, MMIPIM_SETTINGMENU_CTRL_ID),
	END_WIN
};

//号簿管家网络承载设定菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_NET_SETTING_WIN_TAB) = 
{
	//CLEAR_LCD,
	WIN_PRIO(WIN_ONE_LEVEL),
	WIN_TITLE(TXT_PIM_NET_SETTING),
	WIN_FUNC((uint32)HandlePIMNetSettingMenuWinMsg),    
	WIN_ID(MMIPIM_NET_SETTING_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MMI_PIM_NET_SETTING_MENU, MMIPIM_NET_SETTING_MENU_CTRL_ID),
	END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家网络承载-- APN 菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_NET_SETTING_APN_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_FUNC((uint32)HandlePIMNetSettingAPNWinMsg),    
    WIN_ID(MMIPIM_NET_SETTING_APN_WIN_ID),
    WIN_TITLE(TXT_PIM_APN),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_APN_LEN,MMIPIM_NET_SETTING_APN_CTRL_ID),
    END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家网络承载-- 用户名 菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_NET_SETTING_USER_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMNetSettingUserWinMsg),    
    WIN_ID(MMIPIM_NET_SETTING_USER_WIN_ID),
    WIN_TITLE(TXT_COMMON_USERNAME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_USER_LEN ,MMIPIM_NET_SETTING_USER_CTRL_ID),
    END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家网络承载-- 密码菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_NET_SETTING_PASSWORD_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMNetSettingPasswordWinMsg),    
    WIN_ID(MMIPIM_NET_SETTING_PASSWORD_WIN_ID),
    WIN_TITLE(TXT_COMMON_PASSWORD),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_PASSWORD_CTRL(MMIPIM_MAX_PASSWORD_LEN ,MMIPIM_NET_SETTING_PASSWORD_CTRL_ID),
    END_WIN
};

//号簿管家网关设定菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_GATEWAY_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
	WIN_TITLE(TXT_PIM_GATEWAY),
	WIN_FUNC((uint32)HandlePIMGateWayMenuWinMsg),    
	WIN_ID(MMIPIM_GATEWAY_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MMI_PIM_GATEWAY_MENU, MMIPIM_GATEWAY_MENU_CTRL_ID),
	END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家网关设定-- IP 菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_GATEWAY_IP_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMGateWayIpWinMsg),    
    WIN_ID(MMIPIM_GATEWAY_IP_WIN_ID),
    WIN_TITLE(TXT_PIM_IP),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_IP_LEN ,MMIPIM_GATEWAY_IP_CTRL_ID),
    END_WIN
};


/////////////////////////////////////////////////////////////////////////////////////
//号簿管家网关设定-- 端口 菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_GATEWAY_PORT_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMGateWayPortWinMsg),    
    WIN_ID(MMIPIM_GATEWAY_PORT_WIN_ID),
    WIN_TITLE(TXT_COMM_PORT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(MMIPIM_MAX_PORT_LEN ,MMIPIM_GATEWAY_PORT_CTRL_ID),
    END_WIN
};


/////////////////////////////////////////////////////////////////////////////////////
//号簿管家网关设定-- 用户名 菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_GATEWAY_USER_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMGateWayUserWinMsg),    
    WIN_ID(MMIPIM_GATEWAY_USER_WIN_ID),
    WIN_TITLE(TXT_COMMON_USERNAME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_USER_LEN ,MMIPIM_GATEWAY_USER_CTRL_ID),
    END_WIN
};


/////////////////////////////////////////////////////////////////////////////////////
//号簿管家网关设定-- 密码菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_GATEWAY_PASSWORD_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMGateWayPasswordWinMsg),    
    WIN_ID(MMIPIM_GATEWAY_PASSWORD_WIN_ID),
    WIN_TITLE(TXT_COMMON_PASSWORD),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_PASSWORD_CTRL(MMIPIM_MAX_PASSWORD_LEN ,MMIPIM_GATEWAY_PASSWORD_CTRL_ID),
    END_WIN
};


//号簿管家使用代理菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE( MMIPIM_PROXY_WIN_TAB ) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePIMProxyWinMsg),    
    WIN_ID( MMIPIM_PROXY_MENU_WIN_ID ),
    WIN_TITLE( TXT_PIM_PROXY),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPIM_PROXY_RADIO_CTRL_ID ),
	//WIN_STYLE( WS_HAS_SOFTKEY ),                     
    END_WIN
};

//号簿管家SyncML协议参数菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
	WIN_TITLE( TXT_PIM_SYNCML_SETTING),
	WIN_FUNC((uint32)HandlePIMSyncMLSettingMenuWinMsg),    
	WIN_ID(MMIPIM_SYNCML_SETTING_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MMI_PIM_SYNCML_SETTING_MENU, MMIPIM_SYNCML_SETTING_MENU_CTRL_ID),
	END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家SyncML协议参数--PIM服务器窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_SERVER_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMSyncMLSettingServerWinMsg),    
    WIN_ID(MMIPIM_SYNCML_SETTING_SERVER_WIN_ID),
    WIN_TITLE(TXT_PIM_SERVER_ADDR),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_SERVER_ADDR_LEN ,MMIPIM_SYNCML_SETTING_SERVER_CTRL_ID),
    END_WIN
};


/////////////////////////////////////////////////////////////////////////////////////
//号簿管家SyncML协议参数--端口窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_PORT_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMSyncMLSettingPortWinMsg),    
    WIN_ID(MMIPIM_SYNCML_SETTING_PORT_WIN_ID),
    WIN_TITLE(TXT_COMM_PORT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_PORT_LEN ,MMIPIM_SYNCML_SETTING_PORT_CTRL_ID),
    END_WIN
};


/////////////////////////////////////////////////////////////////////////////////////
//号簿管家SyncML协议参数--用户名窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_USER_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMSyncMLSettingUserWinMsg),    
    WIN_ID(MMIPIM_SYNCML_SETTING_USER_WIN_ID),
    WIN_TITLE(TXT_COMMON_USERNAME),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_USER_LEN ,MMIPIM_SYNCML_SETTING_USER_CTRL_ID),
    END_WIN
};


/////////////////////////////////////////////////////////////////////////////////////
//号簿管家SyncML协议参数--密码窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_PASSWORD_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMSyncMLSettingPasswordWinMsg),    
    WIN_ID(MMIPIM_SYNCML_SETTING_PASSWORD_WIN_ID),
     WIN_TITLE(TXT_COMMON_PASSWORD),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_PASSWORD_CTRL(MMIPIM_MAX_PASSWORD_LEN ,MMIPIM_SYNCML_SETTING_PASSWORD_CTRL_ID),
    END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家SyncML协议参数--地址本数据库窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_PB_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMSyncMLSettingPBWinMsg),    
    WIN_ID(MMIPIM_SYNCML_SETTING_PB_WIN_ID),
    WIN_TITLE(TXT_PIM_CONTACT),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_DATABASE_LEN ,MMIPIM_SYNCML_SETTING_PB_CTRL_ID),
    END_WIN
};

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
/////////////////////////////////////////////////////////////////////////////////////
//号簿管家SyncML协议参数--地址本数据库窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_VCAL_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMSyncMLSettingVCALWinMsg),    
    WIN_ID(MMIPIM_SYNCML_SETTING_VCAL_WIN_ID),
    WIN_TITLE(TXT_PIM_CALENDAR),
    //WIN_STYLE(WS_HAS_SOFTKEY),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_DATABASE_LEN ,MMIPIM_SYNCML_SETTING_VCAL_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif /* HS_PIM_VCAL */

//号簿管家SyncML协议参数--编码方式窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_CODE_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePIMSyncMLSettingCodeWinMsg),    
    WIN_ID( MMIPIM_SYNCML_SETTING_CODE_WIN_ID ),
    WIN_TITLE( TXT_CODING_TYPE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,  MMIPIM_SYNCML_SETTING_CODE_RADIO_CTRL_ID ),
	//WIN_STYLE( WS_HAS_SOFTKEY ),         
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//号簿管家SyncML协议参数--鉴权方式窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_AUTH_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePIMSyncMLSettingAuthWinMsg),    
    WIN_ID( MMIPIM_SYNCML_SETTING_AUTH_WIN_ID ),
    WIN_TITLE( TXT_PIM_SYNC_AUTH),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,  MMIPIM_SYNCML_SETTING_AUTH_RADIO_CTRL_ID ),
	//WIN_STYLE( WS_HAS_SOFTKEY ),  
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16 for
//号簿管家SyncML协议参数--同步方式窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_SYNC_TYPE_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePIMSyncMLSettingSyncTypeWinMsg),    
    WIN_ID( MMIPIM_SYNCML_SETTING_SYNC_TYPE_WIN_ID ),
    WIN_TITLE( TXT_PIM_SYNC_TYPE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,  MMIPIM_SYNCML_SETTING_SYNC_TYPE_RADIO_CTRL_ID ),
	//WIN_STYLE( WS_HAS_SOFTKEY ),  
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
#endif

//号簿管家SyncML协议参数--同步方式窗口 ,非工程模式下的,只有双向同步和慢同步
WINDOW_TABLE(MMIPIM_SYNCML_SETTING_MENU_SYNC_TYPE_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePIMSyncMLSettingMenuSyncTypeWinMsg),    
    WIN_ID( MMIPIM_SYNCML_SETTING_SYNC_TYPE_WIN_ID ),
    WIN_TITLE( TXT_PIM_SYNC_TYPE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,  MMIPIM_SYNCML_SETTING_SYNC_TYPE_RADIO_CTRL_ID ),
	//WIN_STYLE( WS_HAS_SOFTKEY ),  
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

//号簿管家PIM业务WAP地址设定菜单窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_WAP_URL_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
    WIN_TITLE( TXT_PIM_WAP_URL),
	WIN_FUNC((uint32)HandlePIMWapUrlMenuWinMsg),    
	WIN_ID(MMIPIM_WAP_URL_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MMI_PIM_WAP_URL_MENU, MMIPIM_WAP_URL_MENU_CTRL_ID),
	END_WIN
};
#ifdef HS_PIM_UA_SETTING
//号簿管家UA设定窗口 chenxiang 20080318
WINDOW_TABLE(MMIPIM_UA_SETTING_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC( (uint32)HandlePIMSyncMLSettingUAWinMsg),    
    WIN_ID( MMIPIM_SYNCML_SETTING_UA_WIN_ID ),
    WIN_TITLE( TXT_PIM_UA_SETTING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPIM_SYNCML_SETTING_UA_RADIO_CTRL_ID ),
	//WIN_STYLE( WS_HAS_SOFTKEY ), 
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};

WINDOW_TABLE(MMIPIM_UA_SETTING_CUSTOM_WIN_TAB) = 
{
	WIN_FUNC((uint32)PimCustomUASetMenuWinHandleMsg),
	WIN_ID(MMIPIM_UA_SETTING_CUSTOM_WIN_ID),
	WIN_TITLE(TXT_PIM_UA_CUSTOM),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MMI_PIM_CUSTOM_UA_MENU, MMIPIM_DEBUG_SETTING_CUSTOM_UA_MENU_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIPIM_UA_SETTING_CUSTOM_PARA_WIN_TAB) = 
{
	WIN_FUNC((uint32)PimCustomUASetParaWinHandleMsg),
	WIN_ID(MMIPIM_UA_SETTING_CUSTOM_PARA_WIN_ID),
	WIN_TITLE(TXT_PIM_UA_CUSTOM),
	CREATE_EDIT_TEXT_CTRL(MMIPIM_UA_CUSTOM_MAX_INPUT_LEN,MMIPIM_DEBUG_SETTING_CUSTOM_UA_EDIT_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	END_WIN
};

#endif
//号簿管家调试参数设定菜单窗口 chenxiang 20080318
WINDOW_TABLE(MMIPIM_DEBUG_SETTING_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
	WIN_TITLE(TXT_PIM_DEBUG_SETTING),
	WIN_FUNC((uint32)HandleDebugSettingMenuWinMsg),    
	WIN_ID(MMIPIM_DEBUG_SETTING_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MMI_PIM_DEBUG_SETTING_MENU, MMIPIM_DEBUG_SETTING_MENU_CTRL_ID),
	END_WIN
};

//号簿管家调试参数--服务器地址预置设定菜单窗口 chenxiang 20080318
WINDOW_TABLE(MMIPIM_DEBUG_SERVER_SETTING_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO(WIN_ONE_LEVEL),
	WIN_TITLE(TXT_PIM_DEBUG_SERVER_SETTING),
	WIN_FUNC((uint32)HandleDebugServerSettingMenuWinMsg),    
	WIN_ID(MMIPIM_DEBUG_SERVER_SETTING_MENU_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	//MS00216980 cheney            				
       CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIPIM_DEBUG_SERVER_SETTING_RADIO_CTRL_ID ),

	END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家PIM业务WAP地址设定-- 业务开通/ 取消 窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_WAP_URL_REG_CANCEL_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMWapUrlRegCancelWinMsg),    
    WIN_ID(MMIPIM_WAP_URL_REG_CANCEL_WIN_ID),
    WIN_TITLE(TXT_PIM_SYNC_REGISTER_OR_CANCEL),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_URL_LEN ,MMIPIM_WAP_URL_REG_CANCEL_CTRL_ID),
    END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家PIM业务WAP地址设定-- 网络查询窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_WAP_URL_NET_QUERY_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMWapUrlNetQueryWinMsg),    
    WIN_ID(MMIPIM_WAP_URL_NET_QUERY_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_PIM_SYNC_NET_QUERY),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_URL_LEN ,MMIPIM_WAP_URL_NET_QUERY_CTRL_ID),
    END_WIN
};

/////////////////////////////////////////////////////////////////////////////////////
//号簿管家PIM业务WAP地址设定-- 资费说明窗口 chenxiang 20071101 pim_setting
WINDOW_TABLE(MMIPIM_WAP_URL_PAY_INDEX_WIN_TAB) = 
{
    //CLEAR_LCD,
    WIN_PRIO(WIN_ONE_LEVEL),
    WIN_FUNC((uint32)HandlePIMWapUrlPayIndexWinMsg),    
    WIN_ID(MMIPIM_WAP_URL_PAY_INDEX_WIN_ID),
    WIN_TITLE(TXT_PIM_PAY_INDEX),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIPIM_MAX_URL_LEN ,MMIPIM_WAP_URL_PAY_INDEX_CTRL_ID),
    END_WIN
};

//号簿管家同步界面窗口
WINDOW_TABLE(MMIPIM_SYNC_BEGIN_WIN_TAB) = 
{
	//CLEAR_LCD,
	WIN_PRIO(WIN_ONE_LEVEL),
       WIN_BACKGROUND_ID(IMAGE_COMMON_BG),
       //WIN_TITLE(TXT_NULL),
	WIN_FUNC((uint32)HandlePIMSyncBeginWinMsg),
	WIN_ID(MMIPIM_SYNC_BEGIN_WIN_ID),
	WIN_STATUSBAR,
	
	CREATE_PRGBOX_CTRL(GUIPRGBOX_STYLE_HORIZONTAL_PROCESS, MMIPIM_SYNC_PRGBOX_CTRL_ID),
	
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPIM_SYNC_PROC_MSG_SHOW_CTRL_ID),
	                      
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID1),
	                      
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID2),
	                      
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3),
	                      
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID4),
	                      
	CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID5),
	                      
    CREATE_LABEL_CTRL(GUILABEL_ALIGN_MIDDLE, MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID6),
   
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),

	END_WIN
};


//业务介绍窗口
WINDOW_TABLE(MMIPIM_SERVICE_INTRODUCE_WIN_TAB) = 
{ 
    WIN_FUNC((uint32) HandlePIMIntroduceWinMsg ),    
    WIN_ID(MMIPIM_SERVICE_INTRODUCE_WIN_ID),
    WIN_TITLE(TXT_PIM_SERVICE_INDEX),
    CREATE_TEXT_CTRL(MMIPIM_SERVICE_INTRODUCE_TEXT_BOX_CTRL_ID),
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN    
};

/*	//Sam.hua	use SelectSimFunc 
WINDOW_TABLE( MMIPIM_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandlePIMSIMSelectWinMsg),    
    WIN_ID(MMIPIM_SELECT_SIM_WIN_ID),
    WIN_TITLE(TXT_SIM_SEL),
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E,MMIPIM_SELECT_SIM__CTRL_ID),
    END_WIN
};
*/
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

LOCAL void MMIPIM_OpenWebWin( char* url_ptr)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    SCI_TRACE_LOW("mmipim_wintab.c:MMIPIM_OpenWebWin() url_ptr = %s", url_ptr);//wangzhefeng add 20091029
    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = url_ptr;
    entry_param.user_agent_ptr = PNULL;
    entry_param.is_connection_customized = TRUE;
    entry_param.connection_index = 0;
    entry_param.is_browser_type_customized = TRUE;
    entry_param.browser_type = MMIBROWSER_TYPE_DORADO;
    
    MMIAPIBROWSER_Entry(&entry_param);
#endif
}


/*==========================================================
 * author		: wangxiaolin      
 * function 	: HandlePIMIntroduceWinMsg
 * decr		: 业务介绍窗口
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  10/16/2009 
 ==========================================================*/
LOCAL MMI_RESULT_E   HandlePIMIntroduceWinMsg(
                                         MMI_WIN_ID_T win_id, 
                                         MMI_MESSAGE_ID_E msg_id, 
                                         DPARAM param 
                                         )
{
    MMI_RESULT_E      result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T     ctrl_id = MMIPIM_SERVICE_INTRODUCE_TEXT_BOX_CTRL_ID;
   // MMI_STRING_T		title_str = {0};
    MMI_STRING_T        content_str = {0};

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMI_GetLabelTextByLang(TXT_PIM_SERVICE_INTRODUCE_CONTENT, &content_str);

            GUITEXT_SetCircularHandle(FALSE,ctrl_id);

            //display text
            GUITEXT_SetString(ctrl_id, content_str.wstr_ptr, content_str.wstr_len,FALSE);

            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;

        case MSG_FULL_PAINT:
        {
            GUI_RECT_T rect = MMITHEME_GetClientRect();
            GUI_LCD_DEV_INFO    lcd_dev_info = {0};

            lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
            lcd_dev_info.block_id = GUI_BLOCK_MAIN;

            LCD_FillRect(&lcd_dev_info, rect, MMI_WINDOW_BACKGROUND_COLOR);   
        }
        break;

        case MSG_CTL_CANCEL:
        case MSG_APP_CANCEL:
        {
            MMK_CloseWin(win_id);
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

/*==========================================================
 * 开发人员	: wangxiaolin      
 * 窗口名称	: HandleGoldHouseMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家窗口处理函数
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMMainMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIPIM_MAINMENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        
        GUIMENU_GetId( MMIPIM_MAINMENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
        case MMIPIM_MENU_ID_SERVICE_INDEX:
               // MMIPIM_OpenWebWin((uint8 *)g_pim.m_url_setting.pay_index); 
               MMK_CreateWin((uint32 *) MMIPIM_SERVICE_INTRODUCE_WIN_TAB, PNULL);
            break;

       case  MMIPIM_MENU_ID_PAY_INDEX:
                MMIPIM_OpenWebWin(g_pim.m_url_setting.pay_index); 
	    break;
	case  MMIPIM_MENU_ID_SYNC_ENTER:
         MMK_CreateWin((uint32*)MMIPIM_SYNC_ENTER_MENU_WIN_TAB, PNULL);
		
		break;

	 default:
             
		break;
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:
		// close the option window
		MMK_CloseWin(MMIPIM_MAINMENU_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}

/*==========================================================
 * 开发人员	: wangxiaolin      
 * 窗口名称	: HandlePIMSyncEnterMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家次级窗口处理函数
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncEnterMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;

    switch(msg_id)
    {
        case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, MMIPIM_SYNC_ENTER_MENU_CTRL_ID);
            GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
            break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_OK:
        case MSG_APP_OK:
        case MSG_APP_WEB:
	    case MSG_CTL_MIDSK:

            GUIMENU_GetId( MMIPIM_SYNC_ENTER_MENU_CTRL_ID, &group_id, &menu_id);
            switch ( menu_id )
            {
                case MMIPIM_MENU_ID_SYNC_REGISTER:
                    MMIPIM_OpenWebWin(g_pim.m_url_setting.reg_cancel); // chenxiang 20071225
                    break;
    
                case  MMIPIM_MENU_ID_SYNC_BEGIN:
                    if (MMIPB_IsPbReady()) // 电话本是否准备好
                    {
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
                            PIM_T_P  pMe = MMIPIM_GetPIM();
                            pMe->m_sync_mode = SYNC_MODE_PB;
#endif /* HS_PIM_VCAL */
                             MMIPIM_OpenSyncWin();
                    }
                    else//电话簿正在使用
                    {
                        MMIPUB_OpenAlertWaitingWin(TXT_COMMON_WAITING);
                    }
                    break;

                 case MMIPIM_MENU_ID_SYNC_PHONEBOOK:
                  MMIAPIPB_OpenListAllGroupWin();
                  break;
                  
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
                case  MMIPIM_MENU_ID_SYNC_BEGIN_VCAL:
                    {
                        PIM_T_P  pMe = MMIPIM_GetPIM();
                        pMe->m_sync_mode = SYNC_MODE_VCAL;

                         MMIPIM_OpenSyncWin();
                        break;
                    }
#endif /* HS_PIM_VCAL */
                case  MMIPIM_MENU_ID_SYNC_NETQUERY:
                    MMIPIM_OpenWebWin(g_pim.m_url_setting.net_query); // chenxiang 20071225
                    break;

                case  MMIPIM_MENU_ID_SYNC_SETTING: //参数预置
                    MMK_CreateWin((uint32*)MMIPIM_SETTING_MENU_WIN_TAB, PNULL); // chenxiang 20071030 pim_setting
                    break;
                    
                default:
                    break;
        }
        break;
        case MSG_APP_CANCEL:    
        case MSG_CTL_CANCEL:
            // close the option window
            MMK_CloseWin(MMIPIM_SYNC_ENTER_MENU_WIN_ID);
            MMK_CloseWin(win_id);
            break;
		
       
        default:
            err_code = MMI_RESULT_FALSE;
            break;
    }
    
    return err_code;
}


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家设定窗口处理函数  chenxiang 20071030 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIPIM_SETTINGMENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( MMIPIM_SETTINGMENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE 
    	case  MMIPIM_MENU_ID_SYNC_TYPE: // 同步方式
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_MENU_SYNC_TYPE_WIN_TAB, PNULL);
		break;
#endif /*  */
        case MMIPIM_MENU_ID_NET_SETTING: // 网络承载参数
		MMK_CreateWin((uint32*)MMIPIM_NET_SETTING_WIN_TAB, PNULL); // chenxiang 20071101 pim_setting
            break;
    
       case  MMIPIM_MENU_ID_GATEWAY: // 网关参数
		MMK_CreateWin((uint32*)MMIPIM_GATEWAY_WIN_TAB, PNULL); // chenxiang 20071101 pim_setting
	    break;
		
	case  MMIPIM_MENU_ID_PROXY: // 使用代理
		MMK_CreateWin((uint32*)MMIPIM_PROXY_WIN_TAB, PNULL); // chenxiang 20071101 pim_setting
		break;
		
	case  MMIPIM_MENU_ID_SYNCML_SETTING: // SyncML协议参数
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_WIN_TAB, PNULL); // chenxiang 20071101 pim_setting
		break;
        
	case  MMIPIM_MENU_ID_WAP_URL: // PIM业务WAP地址
		MMK_CreateWin((uint32*)MMIPIM_WAP_URL_WIN_TAB, PNULL); // chenxiang 20071101 pim_setting
		break;
		
	case  MMIPIM_MENU_ID_RESET_SETTING: // 恢复默认
#if 0//del by wangxiaolin   2010.1.5
		MMIPIM_ResetSetting();
		//提示用户 chenxiang 20080114 add
		MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE); 
#else  //弹出提示框提示用户确认
            MMIPUB_OpenQueryWinByTextId(TXT_PIM_RESTORE_DEFAULT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
#endif
		break;

	 default:
		break;
        }
        break;
  case MSG_PROMPTWIN_CANCEL:
           MMIPUB_CloseQuerytWin(PNULL);
           break;
   
   case MSG_PROMPTWIN_OK://恢复默认设置的处理
         //SCI_TRACE_LOW:"MMIPIM HandlePIMSyncSettingMenuWinMsg,reset param! "
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_1535_112_2_18_2_47_9_175,(uint8*)"");
   		
     	   MMIPIM_ResetSetting();

          MMIPUB_CloseQuerytWin(PNULL); 
	   MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE); 

        break;    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:
     
		// close the option window
		MMK_CloseWin(MMIPIM_SETTING_MENU_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}



/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMNetSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家网络承载设定窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMNetSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIPIM_NET_SETTING_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( MMIPIM_NET_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
            case MMIPIM_MENU_ID_NET_SETTING_APN: //APN
    		MMK_CreateWin((uint32*)MMIPIM_NET_SETTING_APN_WIN_TAB, PNULL);
            break;
    
            case  MMIPIM_MENU_ID_NET_SETTING_USER: // 用户名
    		MMK_CreateWin((uint32*)MMIPIM_NET_SETTING_USER_WIN_TAB, PNULL);
    	    break;
		
	        case  MMIPIM_MENU_ID_NET_SETTING_PASSWORD: // 密码
		    MMK_CreateWin((uint32*)MMIPIM_NET_SETTING_PASSWORD_WIN_TAB, PNULL);
		    break;
		
	        default:
		    break;
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(MMIPIM_NET_SETTING_MENU_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}



/*==========================================================
 * 开发人员	: wangxiaolin     
 * 窗口名称	: HandlePIMNetSettingAPNWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家网络承载设定--APN 窗口处理函数  
 * 日期		:2009.07.02
 ==========================================================*/

LOCAL MMI_RESULT_E HandlePIMNetSettingAPNWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	net_setting_info_type	nv_sync_net_info={0};
    const net_setting_info_type	default_sync_net_info = {"CMWAP","",""};//wangzhefeng add 20091029
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_NET_SETTING_APN_CTRL_ID;
	int str_len=0;

	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
    
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		//get net setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_NET_INFO, &nv_sync_net_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_net_info,0,sizeof(net_setting_info_type));
                    hs_smlLibMemcpy(&nv_sync_net_info,&default_sync_net_info,sizeof(net_setting_info_type));
			MMI_WriteNVItem(MMINV_PIM_NET_INFO,&nv_sync_net_info);
		}
        
        MMIAPICOM_StrToWstr((uint8*)nv_sync_net_info.apn, ucs2buf);
		GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_net_info.apn));
		
		//set im
        GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);
		MMK_SetAtvCtrl(win_id,ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
          
        GUIEDIT_GetString(ctrl_id,&string_info);
        
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
        
        //get net setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_NET_INFO, &nv_sync_net_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_net_info,0,sizeof(net_setting_info_type));
                    hs_smlLibMemcpy(&nv_sync_net_info,&default_sync_net_info,sizeof(net_setting_info_type));
		}
        
		hs_smlLibMemset(nv_sync_net_info.apn, 0, MMIPIM_MAX_APN_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_net_info.apn,(uint8*)ucs2buf, MIN(str_len,(MMIPIM_MAX_APN_LEN + 1)));
        
		//保存NV 项
		MMI_WriteNVItem(MMINV_PIM_NET_INFO,&nv_sync_net_info);
        
		// 保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_net_setting.apn,(uint8*)ucs2buf, MIN(str_len,(MMIPIM_MAX_APN_LEN + 1)));
		//提示用户
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMNetSettingAPNWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_1714_112_2_18_2_47_9_176,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}


 /*==========================================================
 * 开发人员	: wangxiaolin     
 * 窗口名称	: HandlePIMNetSettingUserWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家网络承载设定--用户名 窗口处理函数
 * 日期		:2009.07.02
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMNetSettingUserWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	net_setting_info_type	nv_sync_net_info = {0};
    const net_setting_info_type	default_sync_net_info = {"CMWAP","",""};//wangzhefeng add 20091029
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_NET_SETTING_USER_CTRL_ID;
	int str_len=0;
    
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
		 //get net setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_NET_INFO, &nv_sync_net_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_net_info,0,sizeof(net_setting_info_type));
                    hs_smlLibMemcpy(&nv_sync_net_info,&default_sync_net_info,sizeof(net_setting_info_type));
			MMI_WriteNVItem(MMINV_PIM_NET_INFO,&nv_sync_net_info);
		}
        
	        MMIAPICOM_StrToWstr((uint8*)nv_sync_net_info.username, ucs2buf);
	        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_net_info.username));
               GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

		MMK_SetAtvCtrl(	win_id,	ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_OK:
     case MSG_APP_OK:
     case MSG_APP_WEB:
     case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
        
        //get net setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_NET_INFO, &nv_sync_net_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_net_info,0,sizeof(net_setting_info_type));
                    hs_smlLibMemcpy(&nv_sync_net_info,&default_sync_net_info,sizeof(net_setting_info_type));
		}

		hs_smlLibMemset(nv_sync_net_info.username, 0, MMIPIM_MAX_USER_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_net_info.username,(uint8*)ucs2buf, MIN(str_len,(MMIPIM_MAX_USER_LEN + 1)));
        
		//保存NV 项
		MMI_WriteNVItem(MMINV_PIM_NET_INFO,&nv_sync_net_info);
        
		// 保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_net_setting.username,(uint8*)ucs2buf, MIN(str_len,(MMIPIM_MAX_USER_LEN + 1)));
        
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMNetSettingUserWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_1809_112_2_18_2_47_9_177,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}




/*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMNetSettingPasswordWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家网络承载设定--密码窗口处理函数  
 * 日期		:2009.07.02
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMNetSettingPasswordWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	net_setting_info_type	nv_sync_net_info = {0};
    const net_setting_info_type	default_sync_net_info = {"CMWAP","",""};//wangzhefeng add 20091029
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_NET_SETTING_PASSWORD_CTRL_ID;
	int str_len=0;
	BOOLEAN         is_disp_im_icon = TRUE;
	BOOLEAN         is_disp_num_info = TRUE;
 
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUIEDIT_SetDispImIcon(ctrl_id, &is_disp_im_icon, &is_disp_num_info);
        GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
        //  GUIEDIT_SetPasswordStyle(ctrl_id, GUIEDIT_PASSWORD_STYLE_ALL); //masked for cr234438
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_MULTI); //add for cr234438
		 //get net setting from NV
        return_value = MMI_ReadNVItem(MMINV_PIM_NET_INFO, &nv_sync_net_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_net_info,0,sizeof(net_setting_info_type));
                    hs_smlLibMemcpy(&nv_sync_net_info,&default_sync_net_info,sizeof(net_setting_info_type));
			MMI_WriteNVItem(MMINV_PIM_NET_INFO,&nv_sync_net_info);
		}
        
        MMIAPICOM_StrToWstr((uint8*)nv_sync_net_info.password, ucs2buf);
        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_net_info.password));
              //GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

		MMK_SetAtvCtrl(	win_id,	ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
     case MSG_CTL_OK:
     case MSG_APP_OK:
     case MSG_APP_WEB:
     case MSG_CTL_MIDSK:

        GUIEDIT_GetString(ctrl_id,&string_info);
        
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
                
        //get net setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_NET_INFO, &nv_sync_net_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_net_info,0,sizeof(net_setting_info_type));
                    hs_smlLibMemcpy(&nv_sync_net_info,&default_sync_net_info,sizeof(net_setting_info_type));
		}

		hs_smlLibMemset(nv_sync_net_info.password, 0, MMIPIM_MAX_PASSWORD_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_net_info.password,(uint8*)ucs2buf, MIN(str_len,(MMIPIM_MAX_PASSWORD_LEN + 1)));
        
		//保存NV 项
		MMI_WriteNVItem(MMINV_PIM_NET_INFO,&nv_sync_net_info);
        
		// 保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_net_setting.password,(uint8*)ucs2buf, MIN(str_len,(MMIPIM_MAX_PASSWORD_LEN + 1)));

        //提示用户
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMNetSettingPasswordWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_1912_112_2_18_2_47_10_178,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}


/*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMGateWayMenuWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家网关设定窗口处理函数  
 * 日期		:2009.07.02
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMGateWayMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIPIM_GATEWAY_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( MMIPIM_GATEWAY_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
            case MMIPIM_MENU_ID_GATEWAY_IP: // IP
    		MMK_CreateWin((uint32*)MMIPIM_GATEWAY_IP_WIN_TAB, PNULL);
            break;
    
           case  MMIPIM_MENU_ID_GATEWAY_PORT: // 端口
    		MMK_CreateWin((uint32*)MMIPIM_GATEWAY_PORT_WIN_TAB, PNULL);
    	    break;
		
        	case  MMIPIM_MENU_ID_GATEWAY_USER: // 用户名
        	MMK_CreateWin((uint32*)MMIPIM_GATEWAY_USER_WIN_TAB, PNULL);
        	break;
		
	        case  MMIPIM_MENU_ID_GATEWAY_PASSWORD: // 密码
		    MMK_CreateWin((uint32*)MMIPIM_GATEWAY_PASSWORD_WIN_TAB, PNULL);
    		break;
		
        	default:
		    break;
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:
     
		// close the option window
		MMK_CloseWin(win_id);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}


/*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMGateWayIpWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家网关设定--IP 窗口处理函数  
 * 日期		:2009.07.02
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMGateWayIpWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	proxy_setting_info_type	nv_sync_proxy_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_GATEWAY_IP_CTRL_ID;
    const proxy_setting_info_type default_sync_proxy_info = {1, "10.0.0.172", "80", "", ""};//wangzhefeng add 20091029
	wchar                   dot_str[] = {'.'};
	int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	 //get proxy setting from NV
	return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
	if (MN_RETURN_SUCCESS != return_value)
	{
            //wangzhefeng mod 20091029
		//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
		MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
	}
		
		GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
      	GUIEDIT_ConfigImCustomKey(ctrl_id, GUIIM_TYPE_DIGITAL, GUIIM_CUSTOM_KEY_STAR, dot_str, 1);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_DOT_NUM);
        MMIAPICOM_StrToWstr((uint8*)nv_sync_proxy_info.addr, ucs2buf);
		GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_proxy_info.addr));
		MMK_SetAtvCtrl(	win_id,	ctrl_id);
        //GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

        

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
        
		 //get proxy setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
		}
        
		hs_smlLibMemset(nv_sync_proxy_info.addr, 0, MMIPIM_MAX_IP_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_proxy_info.addr, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_IP_LEN + 1)));

        //保存NV 项
		MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
        
		//保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_proxy_setting.addr, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_IP_LEN + 1)));

        //提示用户
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMGateWayIpWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_2080_112_2_18_2_47_10_179,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}



/*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMGateWayPortWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家网关设定--端口窗口处理函数 
 * 日期		:2009.07.02
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMGateWayPortWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	proxy_setting_info_type	nv_sync_proxy_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_GATEWAY_PORT_CTRL_ID;
    const proxy_setting_info_type default_sync_proxy_info = {1, "10.0.0.172", "80", "", ""};//wangzhefeng add 20091029
	int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 //get proxy setting from NV
        return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
			MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_proxy_info.port, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_proxy_info.port));
          GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
          GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_PURE_NUM);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:

        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
        
		 //get proxy setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
		}
        
        hs_smlLibMemset(nv_sync_proxy_info.port, 0, MMIPIM_MAX_PORT_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_proxy_info.port, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_PORT_LEN + 1)));
		
		MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
		// 保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_proxy_setting.port, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_PORT_LEN + 1)));

		//提示用户
              MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMGateWayPortWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_2174_112_2_18_2_47_10_180,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}


/*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMGateWayUserWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家网关设定--用户名窗口处理函数 
 * 日期		:2009.07.02
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMGateWayUserWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	proxy_setting_info_type	nv_sync_proxy_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_GATEWAY_USER_CTRL_ID;
    const proxy_setting_info_type default_sync_proxy_info = {1, "10.0.0.172", "80", "", ""};//wangzhefeng add 20091029
    int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 //get proxy setting from NV
        return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
			MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_proxy_info.username, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_proxy_info.username));
           GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:

        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (char*)ucs2buf);/*lint !e64*/
        
		 //get proxy setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
		}
        
		hs_smlLibMemset(nv_sync_proxy_info.username, 0, MMIPIM_MAX_USER_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_proxy_info.username, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_USER_LEN + 1)));       
	
		//保存NV 项
		MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
		//保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_proxy_setting.username, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_USER_LEN + 1)));       
		//提示用户
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMGateWayUserWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_2266_112_2_18_2_47_10_181,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}

/*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMGateWayPasswordWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家网关设定--密码窗口处理函数 
 * 日期		:2009.07.02
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMGateWayPasswordWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	proxy_setting_info_type	nv_sync_proxy_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_GATEWAY_PASSWORD_CTRL_ID;
    const proxy_setting_info_type default_sync_proxy_info = {1, "10.0.0.172", "80", "", ""};//wangzhefeng add 20091029
	int str_len=0;
	BOOLEAN         is_disp_im_icon = TRUE;
	BOOLEAN         is_disp_num_info = TRUE;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUIEDIT_SetDispImIcon(ctrl_id, &is_disp_im_icon, &is_disp_num_info);
        GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
      //  GUIEDIT_SetPasswordStyle(ctrl_id, GUIEDIT_PASSWORD_STYLE_ALL); //masked for cr234438
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_MULTI); //add for cr234438
		 //get proxy setting from NV
        return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
			MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_proxy_info.password, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_proxy_info.password));
            //GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
        
		 //get proxy setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
		}
        
		hs_smlLibMemset(nv_sync_proxy_info.password, 0, MMIPIM_MAX_PASSWORD_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_proxy_info.password, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_PASSWORD_LEN + 1)));       
		
		//保存NV 项
		MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
		//保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_proxy_setting.password, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_PASSWORD_LEN + 1)));
		//提示用户
              MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMGateWayPasswordWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_2361_112_2_18_2_47_11_182,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}




/*==========================================================
 * 开发人员	: wangxiaolin      
 * 窗口名称	: HandlePIMProxyWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家使用代理窗口处理函数 
 * 日期		:2009.07.03
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMProxyWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    //MMI_MENU_GROUP_ID_T	group_id = 0;
    //MMI_MENU_ID_T  		menu_id = 0;
    uint16                      cur_selection = 0;
    MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	proxy_setting_info_type	nv_sync_proxy_info={0};
    MMI_CTRL_ID_T ctrl_id=MMIPIM_PROXY_RADIO_CTRL_ID;
    const proxy_setting_info_type default_sync_proxy_info = {1, "10.0.0.172", "80", "", ""};//wangzhefeng add 20091029
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        //GUILIST_SetOwnSofterKey( ctrl_id, TRUE );

        GUILIST_SetMaxItem( ctrl_id, 2, FALSE );
		MMIAPISET_AppendListItemByTextId(TXT_PROXY_ON,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_COMM_PROXY_OFF,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

		 //get encode type from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(SmlEncoding_t));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
			MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
		}
		
		if (1 == nv_sync_proxy_info.use_proxy)
			cur_selection = 0;
		else
			cur_selection = 1;
		

		//set selected item
		GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

		//set current item
		GUILIST_SetCurItemIndex(ctrl_id,cur_selection);

		
		MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
    case MSG_FULL_PAINT:
      
        break;    
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:

        cur_selection = GUILIST_GetCurItemIndex( ctrl_id );
		
		 //get proxy setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_PROXY_INFO, &nv_sync_proxy_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                    //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_proxy_info,0,sizeof(proxy_setting_info_type));
        		hs_smlLibMemcpy(&nv_sync_proxy_info,&default_sync_proxy_info,sizeof(proxy_setting_info_type));
		}
		switch (cur_selection)
		{
		case 0: // 使用代理
			nv_sync_proxy_info.use_proxy = 1;
			g_pim.is_proxy = TRUE;
			break;
		case 1: // 不使用代理
			nv_sync_proxy_info.use_proxy = 0;
			g_pim.is_proxy = FALSE;
			break;
		default:
			break;
		}
        //success!
		MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,&nv_sync_proxy_info);
		g_pim.m_proxy_setting.use_proxy = nv_sync_proxy_info.use_proxy;
		
		MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

		MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(MMIPIM_PROXY_MENU_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}





/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家SyncML协议参数窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIPIM_SYNCML_SETTING_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( MMIPIM_SYNCML_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
        case MMIPIM_MENU_ID_SYNCML_SETTING_SERVER: // PIM服务器
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_SERVER_WIN_TAB, PNULL);

            break;
       case  MMIPIM_MENU_ID_SYNCML_SETTING_PORT: // 端口
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_PORT_WIN_TAB, PNULL);
	    break;

	case  MMIPIM_MENU_ID_SYNCML_SETTING_USER: // 用户名
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_USER_WIN_TAB, PNULL);

		break;
		
	case  MMIPIM_MENU_ID_SYNCML_SETTING_PASSWORD: // 密码
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_PASSWORD_WIN_TAB, PNULL);

		break;
		
	case  MMIPIM_MENU_ID_SYNCML_SETTING_PB: // 地址本数据库
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_PB_WIN_TAB, PNULL);

		break;
       
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
	case  MMIPIM_MENU_ID_SYNCML_SETTING_VCAL: // 日程表数据库
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_VCAL_WIN_TAB, PNULL);

		break;
#endif /* HS_PIM_VCAL */
#if 0
	case  MMIPIM_MENU_ID_SYNCML_SETTING_CODE: //编码方式
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_CODE_WIN_TAB, PNULL);
		break;
		
	case  MMIPIM_MENU_ID_SYNCML_SETTING_AUTH: // 鉴权方式
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_AUTH_WIN_TAB, PNULL);
		break;
#endif
	 default:
		break;
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(MMIPIM_SYNCML_SETTING_MENU_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}

/*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMSyncMLSettingServerWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家SyncML协议参数--PIM服务器窗口处理函数  
 * 日期		:2009.07.02
 ==========================================================*/ 
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingServerWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	server_setting_info_type	nv_sync_syncml_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_SYNCML_SETTING_SERVER_CTRL_ID;
    const server_setting_info_type default_sync_syncml_info = {"./contact", "./calendar", "http://pim.monternet.com", "80", "", ""};//wangzhfeng add 20091029
	int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 //get syncml setting from NV
		return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
			MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_syncml_info.addr, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_syncml_info.addr));
               GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);
               GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);
   
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:

        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
        
		 //get syncml setting from NV
		return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
		}

        hs_smlLibMemset(nv_sync_syncml_info.addr, 0, MMIPIM_MAX_SERVER_ADDR_LEN + 1);		
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_syncml_info.addr, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_SERVER_ADDR_LEN + 1)));
		//保存NV 项
		MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		// 保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_server_setting.addr, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_SERVER_ADDR_LEN + 1)));

		//提示用户
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMSyncMLSettingServerWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_2661_112_2_18_2_47_11_183,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}

 /*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMSyncMLSettingPortWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家SyncML协议参数--端口窗口处理函数  
 * 日期		:2009.07.02
 ==========================================================*/ 
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingPortWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	server_setting_info_type	nv_sync_syncml_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_SYNCML_SETTING_PORT_CTRL_ID;
    const server_setting_info_type default_sync_syncml_info = {"./contact", "./calendar", "http://pim.monternet.com", "80", "", ""};//wangzhfeng add 20091029
    int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 //get syncml setting from NV      
		return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
			MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_syncml_info.port, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_syncml_info.port));
          GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_DIGITAL,GUIIM_TYPE_DIGITAL);
          GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_PURE_NUM);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:

        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
		 //get syncml setting from NV
		return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
		} 


        hs_smlLibMemset(nv_sync_syncml_info.port, 0, MMIPIM_MAX_PORT_LEN + 1);	
        str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_syncml_info.port, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_PORT_LEN + 1)));   
	
		//保存NV 项
		MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		//保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_server_setting.port, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_PORT_LEN + 1))); 
		//提示用户
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMSyncMLSettingPortWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_2753_112_2_18_2_47_11_184,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}
 
 /*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMSyncMLSettingUserWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家SyncML协议参数--用户名窗口处理函数 
 * 日期		:2009.07.02
 ==========================================================*/ 
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingUserWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
    server_setting_info_type	nv_sync_syncml_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_SYNCML_SETTING_USER_CTRL_ID;
    const server_setting_info_type default_sync_syncml_info = {"./contact", "./calendar", "http://pim.monternet.com", "80", "", ""};//wangzhfeng add 20091029
	int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		//get syncml setting from NV
        return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
			MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_syncml_info.username, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_syncml_info.username));
               GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
		 //get syncml setting from NV
		return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
		} 

		hs_smlLibMemset(nv_sync_syncml_info.username, 0, MMIPIM_MAX_USER_LEN + 1);
        str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_syncml_info.username, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_USER_LEN + 1)));   
 		
 		//保存NV 项
		MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		// 保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_server_setting.username, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_USER_LEN + 1)));
		//提示用户
              MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMSyncMLSettingUserWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_2843_112_2_18_2_47_11_185,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}

/*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMSyncMLSettingPasswordWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家SyncML协议参数--密码窗口处理函数 
 * 日期		:2009.07.02
 ==========================================================*/ 
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingPasswordWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	server_setting_info_type	nv_sync_syncml_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_SYNCML_SETTING_PASSWORD_CTRL_ID;
    const server_setting_info_type default_sync_syncml_info = {"./contact", "./calendar", "http://pim.monternet.com", "80", "", ""};//wangzhfeng add 20091029
	int str_len=0;
	BOOLEAN         is_disp_im_icon = TRUE;
	BOOLEAN         is_disp_num_info = TRUE;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		GUIEDIT_SetDispImIcon(ctrl_id, &is_disp_im_icon, &is_disp_num_info);
        GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_ABC | GUIIM_TYPE_ENGLISH | GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
         //  GUIEDIT_SetPasswordStyle(ctrl_id, GUIEDIT_PASSWORD_STYLE_ALL); //masked for cr234438
        GUIEDIT_SetStyle(ctrl_id, GUIEDIT_STYLE_MULTI); //add for cr234438
		 //get syncml setting from NV
        return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
			MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_syncml_info.password, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_syncml_info.password));
          //GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
		 //get syncml setting from NV
		return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
		} 
		hs_smlLibMemset(nv_sync_syncml_info.password, 0, MMIPIM_MAX_PASSWORD_LEN + 1);
        str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_syncml_info.password, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_PASSWORD_LEN + 1)));   

 		//保存NV 项
		MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
        
		//保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_server_setting.password,(uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_PASSWORD_LEN + 1)));   

		//提示用户
              MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMSyncMLSettingPasswordWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_2939_112_2_18_2_47_12_186,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}





 /*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMSyncMLSettingPBWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家SyncML协议参数--数据库设置窗口处理函数 
 * 日期		:2009.07.02
 ==========================================================*/ 
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingPBWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	server_setting_info_type	nv_sync_syncml_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_SYNCML_SETTING_PB_CTRL_ID;
    const server_setting_info_type default_sync_syncml_info = {"./contact", "./calendar", "http://pim.monternet.com", "80", "", ""};//wangzhfeng add 20091029
    int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 //get syncml setting from NV
        return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
			MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_syncml_info.pbdb, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_syncml_info.pbdb));
               GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);

         //get syncml setting from NV
		return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
		} 
		hs_smlLibMemset(nv_sync_syncml_info.pbdb, 0, MMIPIM_MAX_DATABASE_LEN + 1);
        str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_syncml_info.pbdb, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_DATABASE_LEN + 1)));   
  		
 		//保存NV 项
		MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		// 保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_server_setting.pbdb, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_DATABASE_LEN + 1))); 
		//提示用户
              MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMSyncMLSettingPBWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_3033_112_2_18_2_47_12_187,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}


#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10

 /*==========================================================
 * 开发人员	: wangxiaolin       
 * 窗口名称	: HandlePIMSyncMLSettingVCALWinMsg
 * 窗口ID	: 
 * 处理函数	: 
 * 控件ID	: 
 * 说明		: 号簿管家SyncML协议参数--数据库设置窗口处理函数 
 * 日期		:2009.07.02
 ==========================================================*/ 
LOCAL MMI_RESULT_E HandlePIMSyncMLSettingVCALWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	server_setting_info_type	nv_sync_syncml_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_SYNCML_SETTING_VCAL_CTRL_ID;
    const server_setting_info_type default_sync_syncml_info = {"./contact", "./calendar", "http://pim.monternet.com", "80", "", ""};//wangzhfeng add 20091029
    int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 //get syncml setting from NV
        return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
			MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_syncml_info.cddb, ucs2buf);

        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_syncml_info.cddb));
               GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);

         //get syncml setting from NV
		return_value = MMI_ReadNVItem(MMI_PIM_SERVER_INFO, &nv_sync_syncml_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_syncml_info,0,sizeof(server_setting_info_type));
			hs_smlLibMemcpy(&nv_sync_syncml_info,&default_sync_syncml_info,sizeof(server_setting_info_type));
		} 
		hs_smlLibMemset(nv_sync_syncml_info.cddb, 0, MMIPIM_MAX_DATABASE_LEN + 1);
        str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_syncml_info.cddb, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_DATABASE_LEN + 1)));   
  		
 		//保存NV 项
		MMI_WriteNVItem(MMI_PIM_SERVER_INFO,&nv_sync_syncml_info);
		// 保存至全局变量g_pim
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_server_setting.cddb, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_DATABASE_LEN + 1))); 
		//提示用户
              MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMSyncMLSettingVCALWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_3126_112_2_18_2_47_12_188,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}

#endif /* HS_PIM_VCAL */


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingCodeWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingCodeWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    //MMI_MENU_GROUP_ID_T	group_id = 0;
    //MMI_MENU_ID_T  		menu_id = 0;
    uint16                      cur_selection = 0;
    MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
    SmlEncoding_t  nv_sync_encode_info={0};
    MMI_CTRL_ID_T ctrl_id=MMIPIM_SYNCML_SETTING_CODE_RADIO_CTRL_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

        GUILIST_SetMaxItem( ctrl_id, 2, FALSE );

		MMIAPISET_AppendListItemByTextId(TXT_PIM_CODE_XML,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_PIM_CODE_WBXML,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		
		 //get encode type from NV
		return_value = MMI_ReadNVItem(MMI_PIM_ENCODE_TYPE, &nv_sync_encode_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
			hs_smlLibMemset(&nv_sync_encode_info,0,sizeof(SmlEncoding_t));
			MMI_WriteNVItem(MMI_PIM_ENCODE_TYPE,&nv_sync_encode_info);
		}
		
		if (SML_XML == nv_sync_encode_info)
		{
			cur_selection = 0;
		}
		else
		{
			cur_selection = 1;
		}
		
		//set selected item
		GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

		//set current item
		GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
	
		MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
		
        cur_selection = GUILIST_GetCurItemIndex( ctrl_id );
		 //get encode type from NV
		return_value = MMI_ReadNVItem(MMI_PIM_ENCODE_TYPE, &nv_sync_encode_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
			hs_smlLibMemset(&nv_sync_encode_info,0,sizeof(SmlEncoding_t));
			MMI_WriteNVItem(MMI_PIM_ENCODE_TYPE,&nv_sync_encode_info);
		}
		
		switch (cur_selection)
		{
		case 0: 
			nv_sync_encode_info = SML_XML;
			break;
		case 1: 
			nv_sync_encode_info = SML_WBXML;
			break;
		default:
			break;
		}
		MMI_WriteNVItem(MMI_PIM_ENCODE_TYPE,&nv_sync_encode_info);
		// 保存至全局变量g_pim
		g_pim.m_sml_encoding_type = nv_sync_encode_info;
        //success!
		MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

		MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(MMIPIM_SYNCML_SETTING_CODE_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}





/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingAuthWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingAuthWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint16                      cur_selection = 0;
    MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
    auth_type  nv_sync_auth_info={0};
    MMI_CTRL_ID_T ctrl_id=MMIPIM_SYNCML_SETTING_AUTH_RADIO_CTRL_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem( ctrl_id, 2, FALSE );

		MMIAPISET_AppendListItemByTextId(TXT_PIM_AUTH_B64,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_PIM_AUTH_MD5,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

		 //get encode type from NV
		return_value = MMI_ReadNVItem(MMI_PIM_AUTH_TYPE, &nv_sync_auth_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
			hs_smlLibMemset(&nv_sync_auth_info,0,sizeof(SmlEncoding_t));
			MMI_WriteNVItem(MMI_PIM_AUTH_TYPE,&nv_sync_auth_info);
		}
		
		if (AUTH_B64 == nv_sync_auth_info)
		{
			cur_selection = 0;
		}
		else
		{
			cur_selection = 1;
		}
		
		//set selected item
		GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

		//set current item
		GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
	
		MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
		
        cur_selection = GUILIST_GetCurItemIndex( ctrl_id );		
        
		//get encode type from NV
		return_value = MMI_ReadNVItem(MMI_PIM_AUTH_TYPE, &nv_sync_auth_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
			hs_smlLibMemset(&nv_sync_auth_info,0,sizeof(SmlEncoding_t));
			MMI_WriteNVItem(MMI_PIM_AUTH_TYPE,&nv_sync_auth_info);
		}
		
		switch (cur_selection)
		{
		case 0: 
			nv_sync_auth_info = AUTH_B64;
			break;
		case 1: 
			nv_sync_auth_info = AUTH_MD5;
			break;
		default:
			break;
		}
		//修改NV 项
		MMI_WriteNVItem(MMI_PIM_AUTH_TYPE,&nv_sync_auth_info);
		// 保存至全局变量g_pim
		g_pim.m_authtype = nv_sync_auth_info;
        //success!
		MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

		MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(MMIPIM_SYNCML_SETTING_AUTH_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}


#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16 for
/*==========================================================
 * author		: wangxiaolin      
 * function 	: HandlePIMSyncMLSettingSyncTypeWinMsg
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/16/2009 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingSyncTypeWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint16                      cur_selection = 0;
    MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
    sync_type  nv_sync_sync_type={0};
    MMI_CTRL_ID_T ctrl_id=MMIPIM_SYNCML_SETTING_SYNC_TYPE_RADIO_CTRL_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem( ctrl_id, 6, FALSE );

		MMIAPISET_AppendListItemByTextId(TXT_PIM_STR_SYNC_TYPE_2WAY,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_PIM_STR_SYNC_TYPE_S2WAY,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_PIM_STR_SYNC_TYPE_1WFCLI,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_PIM_STR_SYNC_TYPE_RFCLI,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
             MMIAPISET_AppendListItemByTextId(TXT_PIM_STR_SYNC_TYPE_1WFSVR,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_PIM_STR_SYNC_TYPE_RFSVR,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		 //get encode type from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_SYNC_TYPE, &nv_sync_sync_type);
		if (MN_RETURN_SUCCESS != return_value)
		{
			hs_smlLibMemset(&nv_sync_sync_type,0,sizeof(sync_type));
			MMI_WriteNVItem(MMINV_PIM_SYNC_TYPE,&nv_sync_sync_type);
		}


              cur_selection=(uint16)nv_sync_sync_type-1;
              
		
		//set selected item
		GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

		//set current item
		GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
	
		MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
		
           cur_selection = GUILIST_GetCurItemIndex( ctrl_id );		

           nv_sync_sync_type=(sync_type)cur_selection+1;

		//修改NV 项
		MMI_WriteNVItem(MMINV_PIM_SYNC_TYPE,&nv_sync_sync_type);
		// 保存至全局变量g_pim
		g_pim.m_sync_type = nv_sync_sync_type;
		MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

		MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(win_id);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}
#endif /* HS_PIM_SUPPORT_OTHER_SYNC_TYPE */



#ifdef HS_PIM_UA_SETTING
/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMSyncMLSettingUAWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家窗口处理函数  chenxiang 20080318 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingUAWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    //MMI_MENU_GROUP_ID_T	group_id = 0;
    //MMI_MENU_ID_T  		menu_id = 0;
    uint16                      cur_selection = 0;
    MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
    uint32 nv_ua_type=0;
    MMI_CTRL_ID_T ctrl_id=MMIPIM_SYNCML_SETTING_UA_RADIO_CTRL_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem( ctrl_id, 3, FALSE );
        MMIAPISET_AppendListItemByTextId(TXT_PIM_UA_DEFAULT,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        //MS00216980 cheney
        //MMIAPISET_AppendListItemByTextId(TXT_PIM_UA_OTHER,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_PIM_UA_CUSTOM,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		//MMIAPISET_AppendListItemByTextId(TXT_PIM_UA_HS_N71,STXT_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

		 //get encode type from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_UA_TYPE, &nv_ua_type);
		if (MN_RETURN_SUCCESS != return_value)
		{
			hs_smlLibMemset(&nv_ua_type,PIM_UA_TYPE_DEFAULT,sizeof(uint32));
			MMI_WriteNVItem(MMINV_PIM_UA_TYPE,&nv_ua_type);
		}
		
		cur_selection = (uint16)nv_ua_type;
		
		//set selected item
		GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

		//set current item
		GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
	
		MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
		
        cur_selection = GUILIST_GetCurItemIndex( ctrl_id );			
		 //get encode type from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_UA_TYPE, &nv_ua_type);
		if (MN_RETURN_SUCCESS != return_value)
		{
			hs_smlLibMemset(&nv_ua_type,PIM_UA_TYPE_DEFAULT,sizeof(uint32));
			MMI_WriteNVItem(MMINV_PIM_UA_TYPE,&nv_ua_type);
		}
		
		nv_ua_type = (uint32)cur_selection;
		
		//修改NV 项
		MMI_WriteNVItem(MMINV_PIM_UA_TYPE,&nv_ua_type);
		// 保存至全局变量g_pim
		g_pim.m_ua_type = nv_ua_type;
        //success!
        //MS00216980 cheney
		if (PIM_UA_TYPE_CUSTOM != cur_selection)
        {
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		    MMK_CloseWin( win_id );
        }
        else
        {
            MMK_CreateWin((uint32*)MMIPIM_UA_SETTING_CUSTOM_WIN_TAB, PNULL);
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(MMIPIM_SYNCML_SETTING_UA_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}



/*****************************************************************************/
//  Description : handle debug setting menu win msg
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  PimCustomUASetMenuWinHandleMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    //SCI_TRACE_LOW:"MMIDM==> enter PimCustomUASetMenuWinHandleMsg "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_3550_112_2_18_2_47_13_189,(uint8*)"");

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIPIM_DEBUG_SETTING_CUSTOM_UA_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( MMIPIM_DEBUG_SETTING_CUSTOM_UA_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
            case MMIPIM_MENU_ID_MAN:
                s_custom_ua_type = PIM_CUSTOM_MAN;
                MMK_CreateWin((uint32*)MMIPIM_UA_SETTING_CUSTOM_PARA_WIN_TAB, PNULL);
                break;
            case MMIPIM_MENU_ID_OEM:
                s_custom_ua_type = PIM_CUSTOM_OEM;
                MMK_CreateWin((uint32*)MMIPIM_UA_SETTING_CUSTOM_PARA_WIN_TAB, PNULL);
                break;
            case MMIPIM_MENU_ID_MOD:
                s_custom_ua_type =PIM_CUSTOM_MOD;
                MMK_CreateWin((uint32*)MMIPIM_UA_SETTING_CUSTOM_PARA_WIN_TAB, PNULL);
                break;
            case MMIPIM_MENU_ID_UA:
                s_custom_ua_type = PIM_CUSTOM_UA;
                MMK_CreateWin((uint32*)MMIPIM_UA_SETTING_CUSTOM_PARA_WIN_TAB, PNULL);
                break;
            default:
                break;
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(win_id);
		break;
			
	default:
		result = MMI_RESULT_FALSE;
		break;
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : PimCustomUASetParaWinHandleMsg
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E PimCustomUASetParaWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_CTRL_ID_T		editbox_ctrl_id = MMIPIM_DEBUG_SETTING_CUSTOM_UA_EDIT_CTRL_ID;
	MMI_RESULT_E		result = MMI_RESULT_TRUE;
	MMI_STRING_T        string_info = {0};
    char                str[MMIPIM_UA_CUSTOM_MAX_INPUT_LEN] = {0};
    wchar               wstr[MMIPIM_UA_CUSTOM_MAX_INPUT_LEN] = {0};
    uint32              str_len = 0;
    MMI_TEXT_ID_T       text_id = TXT_PIM_UA_CUSTOM;
    uint32              text_max_len = MMIPIM_UA_CUSTOM_MAX_INPUT_LEN;
    GUIIM_TYPE_T        allow_im = GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL;
    GUIIM_TYPE_T        default_im = GUIIM_TYPE_ENGLISH;
	switch (msg_id)
	{
	case MSG_OPEN_WINDOW:
        switch(s_custom_ua_type) 
        {
            case PIM_CUSTOM_MAN:
        	    text_id = TXT_PIM_CUSTOM_MAN;;
                break;
            case PIM_CUSTOM_OEM:
        	    text_id = TXT_PIM_CUSTOM_OEM;
                break;
            case PIM_CUSTOM_MOD:
        	    text_id = TXT_PIM_CUSTOM_MOD;
                break;
            case PIM_CUSTOM_UA:
                text_id = TXT_PIM_CUSTOM_UA;
        	    break;
            default:
                break;
        }
        GUIEDIT_SetTextMaxLen(editbox_ctrl_id,text_max_len/sizeof(wchar), text_max_len);
        GUIWIN_SetTitleTextId(win_id,text_id,FALSE); 
		//set im
        GUIEDIT_SetIm(editbox_ctrl_id,allow_im,default_im);
        PimGetCustomUAParaInfo(s_custom_ua_type,str);
        str_len = strlen(str);
        MMI_STRNTOWSTR( wstr, MMIPIM_UA_CUSTOM_MAX_INPUT_LEN, str, str_len, str_len );/*lint !e64*/

        GUIEDIT_SetString(editbox_ctrl_id,wstr,str_len);
		MMK_SetAtvCtrl(win_id,editbox_ctrl_id);
		break;
    case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
	case MSG_CTL_OK:
	case MSG_APP_WEB:
		//save to nv
		SCI_MEMSET(&string_info,0,sizeof(MMI_STRING_T));
		GUIEDIT_GetString(editbox_ctrl_id,&string_info);

        MMI_WSTRNTOSTR(str, MMIPIM_UA_CUSTOM_MAX_INPUT_LEN, string_info.wstr_ptr, string_info.wstr_len, string_info.wstr_len );/*lint !e64*/

        PimSetCustomUAParaInfo(s_custom_ua_type,str,strlen(str));

		MMK_CloseWin(win_id);
		break;

	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);
}

/*****************************************************************************/
//  Description : set pim para info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void PimSetCustomUAParaInfo(MMIPIM_CUSTOM_UA_TYPE_E type,char* string,uint32 len)
{
    uint32 max_len = 0;
    MMIPIM_UA_INFO_T ua_info = {0};

    if (PNULL!=string && 0!=len)
    {
        MMI_ReadNVItem(MMINV_PIM_UA_INFO, &ua_info);
        max_len = MIN(len,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
        switch(type)
        {
        case PIM_CUSTOM_MAN:
            SCI_MEMSET(ua_info.man,0,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
            SCI_MEMCPY(ua_info.man,string,max_len);
            break;
        case PIM_CUSTOM_OEM:
            SCI_MEMSET(ua_info.oem,0,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
            SCI_MEMCPY(ua_info.oem,string,max_len);
            break;
        case PIM_CUSTOM_MOD:
            SCI_MEMSET(ua_info.mod,0,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
            SCI_MEMCPY(ua_info.mod,string,max_len);
            break;
        case PIM_CUSTOM_UA:
            SCI_MEMSET(ua_info.ua,0,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
            SCI_MEMCPY(ua_info.ua,string,max_len);
            break;
        default:
            break;
        }
        MMINV_WRITE(MMINV_PIM_UA_INFO, &ua_info);
    }
}
/*****************************************************************************/
//  Description : Get PIM para info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL void PimGetCustomUAParaInfo(MMIPIM_CUSTOM_UA_TYPE_E type,char* string)
{
    //MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMIPIM_UA_INFO_T ua_info = {0};
    
    if (PNULL != string)
    {
        MMI_ReadNVItem(MMINV_PIM_UA_INFO, &ua_info);
    
        switch(type)
        {
        case PIM_CUSTOM_MAN:
            SCI_MEMCPY(string,ua_info.man,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
            break;
        case PIM_CUSTOM_OEM:
            SCI_MEMCPY(string,ua_info.oem,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
            break;
        case PIM_CUSTOM_MOD:
            SCI_MEMCPY(string,ua_info.mod,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
            break;
        case PIM_CUSTOM_UA:
            SCI_MEMCPY(string,ua_info.ua,MMIPIM_UA_CUSTOM_MAX_INPUT_LEN);
            break;
        default:
            break;
        }
    }
    //SCI_TRACE_LOW:"MMIPIM PimGetCustomUAParaInfo string=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_3743_112_2_18_2_47_13_190,(uint8*)"s",string);
}
#endif
/*****************************************************************************/
//  Description : Get PIM custom ua info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIM_GetUAInfo(uint32 index,MMIPIM_UA_INFO_T * ua_info_ptr)
{
    BOOLEAN result = FALSE;
    MMIPIM_UA_INFO_T ua_info = {0};
	int len=0;
    //MS00216980 cheney
    if(PNULL!= ua_info_ptr)
    {
#ifdef HS_PIM_UA_SETTING
	 if ( PIM_UA_TYPE_CUSTOM== index)
        {
            MMI_ReadNVItem(MMINV_PIM_UA_INFO, &ua_info);
            SCI_MEMCPY(ua_info_ptr,&ua_info,sizeof(MMIPIM_UA_INFO_T));
        }
        else // index = PIM_UA_TYPE_DEFAULT
#endif
        {
            SCI_MEMCPY(ua_info_ptr->man,PIM_DEFAULT_MAN,sizeof(PIM_DEFAULT_MAN));
            SCI_MEMCPY(ua_info_ptr->oem,PIM_DEFAULT_OEM,sizeof(PIM_DEFAULT_OEM));
            SCI_MEMCPY(ua_info_ptr->mod,PIM_DEFAULT_MOD,sizeof(PIM_DEFAULT_MOD));
			len=strlen(MMIAPICOM_GetUserAgent());
            SCI_MEMCPY(ua_info_ptr->ua,MMIAPICOM_GetUserAgent(),len);
        }
        result = TRUE;
    }
//    SCI_TRACE_LOW("MMIPIM MMIPIM_GetCustomUAInfo ua_info->man=%s",ua_info_ptr->man);
//    SCI_TRACE_LOW("MMIPIM MMIPIM_GetCustomUAInfo ua_info->oem=%s",ua_info_ptr->oem);
//    SCI_TRACE_LOW("MMIPIM MMIPIM_GetCustomUAInfo ua_info->mod=%s",ua_info_ptr->mod);
//    SCI_TRACE_LOW("MMIPIM MMIPIM_GetCustomUAInfo ua_info->ua=%s",ua_info_ptr->ua);
    //SCI_TRACE_LOW:"MMIPIM MMIPIM_GetCustomUAInfo result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_3779_112_2_18_2_47_13_191,(uint8*)"d",result);
    return result;
}
/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandleDebugSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家调试参数设定窗口处理函数  chenxiang 20080318 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandleDebugSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    //SCI_TRACE_LOW:"MMIPIM enter HandleDebugSettingMenuWinMsg "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_3798_112_2_18_2_47_13_192,(uint8*)"");

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIPIM_DEBUG_SETTING_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( MMIPIM_DEBUG_SETTING_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
	case  MMIPIM_MENU_ID_SYNCML_SETTING_CODE: //编码方式
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_CODE_WIN_TAB, PNULL);
		break;
		
	case  MMIPIM_MENU_ID_SYNCML_SETTING_AUTH: // 鉴权方式
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_AUTH_WIN_TAB, PNULL);
		break;
        
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16 f
    	case  MMIPIM_MENU_ID_SYNC_TYPE: // 同步方式
		MMK_CreateWin((uint32*)MMIPIM_SYNCML_SETTING_SYNC_TYPE_WIN_TAB, PNULL);
		break;
#endif /*  */
        
       case  MMIPIM_MENU_ID_UA_SETTING: // UA设置
            //SCI_TRACE_LOW:"MMIPIM enter HandleDebugSettingMenuWinMsg ,UA_SETTING "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_3830_112_2_18_2_47_13_193,(uint8*)"");
#ifdef HS_PIM_UA_SETTING
		MMK_CreateWin((uint32*)MMIPIM_UA_SETTING_WIN_TAB, PNULL);
#endif
	    break;
		
	case  MMIPIM_MENU_ID_DEBUG_SERVER_SETTING: // 预置服务器
		MMK_CreateWin((uint32*)MMIPIM_DEBUG_SERVER_SETTING_WIN_TAB, PNULL);
		break;

	case  MMIPIM_MENU_ID_DELETE_ANCHOR: // 删除时间戳
	{
            char  nv_lastanchor[MMIPIM_ANCHOR_LEN]={0};
            hs_smlLibMemset(nv_lastanchor,0,sizeof(nv_lastanchor));
            MMI_WriteNVItem(MMIPIM_LAST_ANCHOR,(void *)nv_lastanchor);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.12.8
            MMI_WriteNVItem(MMIPIM_VCAL_LAST_ANCHOR,(void *)nv_lastanchor);
#endif /* HS_PIM_VCAL */
            MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
       }
		break;
                
	 default:
		break;
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(MMIPIM_DEBUG_SETTING_MENU_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}



/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandleDebugServerSettingMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家调试参数设定-- 预置服务器窗口处理函数  chenxiang 20080318 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandleDebugServerSettingMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    //MS00216980 cheney
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    //MMI_MENU_GROUP_ID_T	group_id = 0;
    //MMI_MENU_ID_T  		menu_id = 0;
    MMI_CTRL_ID_T ctrl_id=MMIPIM_DEBUG_SERVER_SETTING_RADIO_CTRL_ID;
    //MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
    uint32 cur_selection = 0;
    server_setting_info_type server_info = {0};

    const server_setting_info_type nv_sync_syncml_info = {"./contact", "./calendar", PIM_SRV_ADDR_URL, "80", "", ""};
    const server_setting_info_type nv_sync_syncml_info_test = {"./contact", "./calendar", PIM_SRV_ADDR_URL_TEST, "80", "", ""};

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
	GUILIST_SetMaxItem( ctrl_id, 2, FALSE );
	MMIAPISET_AppendListItemByTextId(TXT_PIM_SERVER_ADD,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
	MMIAPISET_AppendListItemByTextId(TXT_PIM_SERVER_ADD_TEST,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);

	MMIPIM_GetServerConfig(&server_info);
	if (strcmp(server_info.addr, PIM_SRV_ADDR_URL) == 0)
	{/*monternet*/
		cur_selection = 0;
	}
	else
	{/*218.206.176.241*/
		cur_selection = 1;
	}

	//set selected item
	GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
	//set current item
	GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
	MMK_SetAtvCtrl(win_id, ctrl_id); 
       break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
	cur_selection = GUILIST_GetCurItemIndex( ctrl_id );	
	if(0==cur_selection)
	{
		MMI_WriteNVItem(MMI_PIM_SERVER_INFO,(void *)&nv_sync_syncml_info);
	}
	else
	{
		MMI_WriteNVItem(MMI_PIM_SERVER_INFO,(void *)&nv_sync_syncml_info_test);
	}

	MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
	/*Start of  wangzhefeng 2009.11.27 for 关闭窗口 */
	MMK_CloseWin(win_id);
	/*End   of  wangzhefeng 2009.11.27 for 关闭窗口 */
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(win_id);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}


/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMWapUrlMenuWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMWapUrlMenuWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    MMI_MENU_GROUP_ID_T	group_id = 0;
    MMI_MENU_ID_T  		menu_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, MMIPIM_WAP_URL_MENU_CTRL_ID);
        GUIWIN_SetSoftkeyTextId(win_id, TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId( MMIPIM_WAP_URL_MENU_CTRL_ID, &group_id, &menu_id);
        switch ( menu_id )
        {
        case MMIPIM_MENU_ID_WAP_URL_REGISTER_OR_CANCEL: // 业务开通/注销
		MMK_CreateWin((uint32*)MMIPIM_WAP_URL_REG_CANCEL_WIN_TAB, PNULL);

            break;
    
       case  MMIPIM_MENU_ID_WAP_URL_NET_QUERY: // 网络查询
		MMK_CreateWin((uint32*)MMIPIM_WAP_URL_NET_QUERY_WIN_TAB, PNULL);

	    break;
		
	case  MMIPIM_MENU_ID_WAP_URL_PAY_INDEX: // 资费说明
		MMK_CreateWin((uint32*)MMIPIM_WAP_URL_PAY_INDEX_WIN_TAB, PNULL);

		break;
		
	 default:
		break;
        }
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(MMIPIM_WAP_URL_MENU_WIN_ID);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}



/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMWapUrlRegCancelWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定--业务开通/ 取消 窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMWapUrlRegCancelWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	wap_url_info_type	nv_sync_url_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_WAP_URL_REG_CANCEL_CTRL_ID;
    const wap_url_info_type default_sync_url_info = {"http://pim1.monternet.com/kt.wml", "http://pim1.monternet.com/cx.wml", "http://pim1.monternet.com/zf.wml"};//wangzhefeng add 20091029
    int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        
		 //get url setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_WAP_URL_INFO, &nv_sync_url_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_url_info,0,sizeof(wap_url_info_type));
                    hs_smlLibMemcpy(&nv_sync_url_info, &default_sync_url_info, sizeof(wap_url_info_type));
			MMI_WriteNVItem(MMINV_PIM_WAP_URL_INFO,&nv_sync_url_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_url_info.reg_cancel, ucs2buf);
        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_url_info.reg_cancel));
               GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);
               GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);

        MMK_SetAtvCtrl(	win_id,	ctrl_id);   
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
   
		 //get url setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_WAP_URL_INFO, &nv_sync_url_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_url_info,0,sizeof(wap_url_info_type));
                    hs_smlLibMemcpy(&nv_sync_url_info, &default_sync_url_info, sizeof(wap_url_info_type));
		}
        
		hs_smlLibMemset(nv_sync_url_info.reg_cancel, 0, MMIPIM_MAX_URL_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_url_info.reg_cancel,(uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_URL_LEN + 1)));
		//保存NV 项
		MMI_WriteNVItem(MMINV_PIM_WAP_URL_INFO,&nv_sync_url_info);
        
		//保存至全局变量g_pim
		hs_smlLibMemset(g_pim.m_url_setting.reg_cancel, 0, MMIPIM_MAX_URL_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_url_setting.reg_cancel,(uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_URL_LEN + 1)));
        //提示用户
        MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMWapUrlRegCancelWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4108_112_2_18_2_47_14_194,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}




/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMWapUrlNetQueryWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定--网络查询窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMWapUrlNetQueryWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	wap_url_info_type	nv_sync_url_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_WAP_URL_NET_QUERY_CTRL_ID;  
    const wap_url_info_type default_sync_url_info = {"http://pim1.monternet.com/kt.wml", "http://pim1.monternet.com/cx.wml", "http://pim1.monternet.com/zf.wml"};//wangzhefeng add 20091029
	int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 //get url setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_WAP_URL_INFO, &nv_sync_url_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_url_info,0,sizeof(wap_url_info_type));
                    hs_smlLibMemcpy(&nv_sync_url_info, &default_sync_url_info, sizeof(wap_url_info_type));
			MMI_WriteNVItem(MMINV_PIM_WAP_URL_INFO,&nv_sync_url_info);
		}

        MMIAPICOM_StrToWstr((uint8*)nv_sync_url_info.net_query, ucs2buf);
        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_url_info.net_query));
               GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);
               GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);

        MMK_SetAtvCtrl(	win_id,	ctrl_id); 
		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
        
		 //get url setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_WAP_URL_INFO, &nv_sync_url_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_url_info,0,sizeof(wap_url_info_type));
                    hs_smlLibMemcpy(&nv_sync_url_info, &default_sync_url_info, sizeof(wap_url_info_type));
		}
		hs_smlLibMemset(nv_sync_url_info.net_query, 0, MMIPIM_MAX_URL_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_url_info.net_query, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_URL_LEN + 1)));
		//保存NV 项
		MMI_WriteNVItem(MMINV_PIM_WAP_URL_INFO,&nv_sync_url_info);
		// 保存至全局变量g_pim
		hs_smlLibMemset(g_pim.m_url_setting.net_query, 0, MMIPIM_MAX_URL_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_url_setting.net_query, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_URL_LEN + 1)));
		//提示用户
              MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;
    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMWapUrlNetQueryWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4199_112_2_18_2_47_14_195,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}




/*==========================================================
 * 开发人员	: chenxiang      
 * 窗口名称	: HandlePIMWapUrlPayIndexWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家PIM业务WAP地址设定--资费说明窗口处理函数  chenxiang 20071101 pim_setting
 ==========================================================*/
LOCAL MMI_RESULT_E HandlePIMWapUrlPayIndexWinMsg(
											 MMI_WIN_ID_T	win_id,
											 MMI_MESSAGE_ID_E	msg_id,
											 DPARAM				param
											 )
{
	MMI_RESULT_E	recode = MMI_RESULT_TRUE;
	MMI_STRING_T string_info = {0};
	MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
	wap_url_info_type	nv_sync_url_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIPIM_WAP_URL_PAY_INDEX_CTRL_ID;  
    const wap_url_info_type default_sync_url_info = {"http://pim1.monternet.com/kt.wml", "http://pim1.monternet.com/cx.wml", "http://pim1.monternet.com/zf.wml"};//wangzhefeng add 20091029
	int str_len=0;
	hs_smlLibMemset(&string_info, 0, sizeof(MMI_STRING_T));
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
		 //get url setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_WAP_URL_INFO, &nv_sync_url_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_url_info,0,sizeof(wap_url_info_type));
                    hs_smlLibMemcpy(&nv_sync_url_info, &default_sync_url_info, sizeof(wap_url_info_type));
			MMI_WriteNVItem(MMINV_PIM_WAP_URL_INFO,&nv_sync_url_info);
		}
        
        MMIAPICOM_StrToWstr((uint8*)nv_sync_url_info.pay_index, ucs2buf);
        GUIEDIT_SetString(ctrl_id,ucs2buf,(uint16)strlen(nv_sync_url_info.pay_index));
               GUIEDIT_SetIm(ctrl_id,GUIIM_TYPE_ENGLISH|GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ENGLISH);
               GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_URL);

        MMK_SetAtvCtrl(	win_id,	ctrl_id); 

		break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
      case MSG_CTL_OK:
      case MSG_APP_OK:
      case MSG_APP_WEB:
      case MSG_CTL_MIDSK:
        
        GUIEDIT_GetString(ctrl_id,&string_info);
        MMIAPICOM_WstrToStr(string_info.wstr_ptr, (uint8*)ucs2buf);
        
		 //get url setting from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_WAP_URL_INFO, &nv_sync_url_info);
		if (MN_RETURN_SUCCESS != return_value)
		{
                  //wangzhefeng mod 20091029
			//hs_smlLibMemset(&nv_sync_url_info,0,sizeof(wap_url_info_type));
                    hs_smlLibMemcpy(&nv_sync_url_info, &default_sync_url_info, sizeof(wap_url_info_type));
		}
		hs_smlLibMemset(nv_sync_url_info.pay_index, 0, MMIPIM_MAX_URL_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(nv_sync_url_info.pay_index, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_URL_LEN + 1)));
		//保存NV 项
		MMI_WriteNVItem(MMINV_PIM_WAP_URL_INFO,&nv_sync_url_info);
		// 保存至全局变量g_pim
		//如果不重新设置，可能会残留之前的设置
		hs_smlLibMemset(g_pim.m_url_setting.pay_index, 0, MMIPIM_MAX_URL_LEN + 1);
		str_len=strlen((char*)ucs2buf);
		hs_smlLibMemcpy(g_pim.m_url_setting.pay_index, (uint8*)ucs2buf, MIN(str_len, (MMIPIM_MAX_URL_LEN + 1)));
		//提示用户
              MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
		MMK_CloseWin(win_id);
		break;

    
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:        
		MMK_CloseWin(win_id);//返回上一级
        break;

	default:
		//SCI_TRACE_LOW:"mmipim_wintab.c:HandlePIMWapUrlPayIndexWinMsg() msg_id = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4293_112_2_18_2_47_14_196,(uint8*)"d", msg_id);
		recode = MMI_RESULT_FALSE;
		break;
	}
	return recode;

}

uint16 MMIPIM_GenDisplayNum(wchar * chr_ucs2,uint16 num)
{
    uint8 num_utf8[10]={0};
    uint16 chr_len=0;
    uint8 i=0;
	
	MMISET_LANGUAGE_TYPE_E lang_type = MMISET_LANGUAGE_ENGLISH;
    MMIAPISET_GetLanguageType(&lang_type);
	if( MMISET_LANGUAGE_SIMP_CHINESE == lang_type
        || MMISET_LANGUAGE_TRAD_CHINESE == lang_type 
        )
    {
		sprintf( (char*)num_utf8, "%d", num);
    }
	else
	{
    	sprintf( (char*)num_utf8, " %d ", num);
	}
     chr_len= (uint16)strlen((char*)num_utf8);
      for(i=0;i<chr_len;i++)
     {
         chr_ucs2[i]=(uint16)(0x00FF&num_utf8[i]);
     }
     return chr_len;
}

/*==========================================================
 * author		: wangxiaolin       
 * function 	: MMIPIM_DisplayWinBackground
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  11/9/2009 
 ==========================================================*/
LOCAL void MMIPIM_DisplayWinBackground(
										  MMI_IMAGE_ID_T	image_id,	//IN:
										  MMI_WIN_ID_T win_id
										  )
{
	GUI_RECT_T	rect = {0, MMI_STATUSBAR_HEIGHT, MMI_MAINSCREEN_RIGHT_MAX_PIXEL,
                                                MMI_MAINSCREEN_BOTTOM_MAX_PIXEL - MMI_FULLSCREEN_SOFTKEY_BAR_HEIGHT};
       GUI_POINT_T         dis_point = {0};
	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    
	if (IMAGE_NULL != image_id)
	{
        dis_point.x = rect.left;
        dis_point.y = rect.top;
        GUIRES_DisplayImg(&dis_point,
                PNULL,
                PNULL,
                win_id,
                image_id,
                &lcd_dev_info);
	}
}


/*==========================================================
 * 开发人员	: wangxiaolin      
 * 窗口名称	: HandlePIMSyncBeginWinMsg
 * 窗口ID		: 
 * 处理函数	: 
 * 控件ID		: 
 * 说明		: 号簿管家同步窗口处理函数
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncBeginWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    //GUI_RECT_T      prg_rect = {PIM_PRGBOX_LEFT, PIM_PRGBOX_TOP, PIM_PRGBOX_RIGHT, PIM_PRGBOX_BOTTOM};
    static BOOLEAN  is_first_up,is_first_down;
	GUI_BOTH_RECT_T     both_rect = {0};
	GUI_BOTH_RECT_T both_prg_rect={0};

    switch( msg_id )
    {
    case MSG_OPEN_WINDOW:
	{
		GUI_BOTH_RECT_T title_both_rect=MMITHEME_GetWinTitleBothRect(win_id);
		GUI_BOTH_RECT_T screen_both_rect=MMITHEME_GetFullScreenBothRect();
		uint16 height = GUIFONT_GetHeight(MMI_DEFAULT_NORMAL_FONT);
		MMI_STRING_T	dis_str = {0};
		MMITHEME_LABEL_T    theme_ptr={0};
		MMITHEME_GetLabelTheme(&theme_ptr);
		
		both_rect.v_rect.left=0;
	    both_rect.v_rect.right =screen_both_rect.v_rect.right ;
		both_rect.h_rect.left = 0;
		both_rect.h_rect.right =screen_both_rect.h_rect.right ;
		
        {
			both_rect.v_rect.top=title_both_rect.v_rect.bottom+1;
			both_rect.v_rect.bottom =both_rect.v_rect.top+2+height;
			both_rect.h_rect.top = title_both_rect.h_rect.bottom+1;
		    both_rect.h_rect.bottom =both_rect.h_rect.top+2+height; 
		 
			//GUILABEL_SetRect(MMIPIM_SYNC_PROC_MSG_SHOW_CTRL_ID, &lab_rect, FALSE);
			GUIAPICTRL_SetBothRect(MMIPIM_SYNC_PROC_MSG_SHOW_CTRL_ID,&both_rect);
			GUILABEL_SetFont(MMIPIM_SYNC_PROC_MSG_SHOW_CTRL_ID,MMI_DEFAULT_NORMAL_FONT,theme_ptr.font.color);
        }	                      
        {
			both_rect.v_rect.top=title_both_rect.v_rect.bottom+1+(height+2);
			both_rect.v_rect.bottom =both_rect.v_rect.top+(2+height);
			both_rect.h_rect.top = title_both_rect.h_rect.bottom+1+(height+2);
		    both_rect.h_rect.bottom =both_rect.h_rect.top+(2+height); 
			GUIAPICTRL_SetBothRect(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID1, &both_rect);
			GUILABEL_SetFont(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID1,MMI_DEFAULT_NORMAL_FONT,theme_ptr.font.color);
        }	                      
        {
        	both_rect.v_rect.top=title_both_rect.v_rect.bottom+1+2*(height+2);
			both_rect.v_rect.bottom =both_rect.v_rect.top+(2+height);
			both_rect.h_rect.top = title_both_rect.h_rect.bottom+1+2*(height+2);
		    both_rect.h_rect.bottom =both_rect.h_rect.top+(2+height); 

			GUIAPICTRL_SetBothRect(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID2, &both_rect);
			GUILABEL_SetFont(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID2,MMI_DEFAULT_NORMAL_FONT,theme_ptr.font.color);
        }	                      
        {
        	both_rect.v_rect.top=title_both_rect.v_rect.bottom+1+3*(height+2);
			both_rect.v_rect.bottom =both_rect.v_rect.top+(2+height);
			both_rect.h_rect.top = title_both_rect.h_rect.bottom+1+3*(height+2);
		    both_rect.h_rect.bottom =both_rect.h_rect.top+(2+height); 

			GUIAPICTRL_SetBothRect(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &both_rect);
			GUILABEL_SetFont(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3,MMI_DEFAULT_NORMAL_FONT,theme_ptr.font.color);
        }	                      
	                      
        {
        	both_rect.v_rect.top=title_both_rect.v_rect.bottom+1+4*(height+2);
			both_rect.v_rect.bottom =both_rect.v_rect.top+(2+height);
			both_rect.h_rect.top = title_both_rect.h_rect.bottom+1+4*(height+2);
		    both_rect.h_rect.bottom =both_rect.h_rect.top+(2+height);

			GUIAPICTRL_SetBothRect(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID4, &both_rect);
			GUILABEL_SetFont(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID4,MMI_DEFAULT_NORMAL_FONT,theme_ptr.font.color);
        }	                      
        {
        	both_rect.v_rect.top=title_both_rect.v_rect.bottom+1+5*(height+2);
			both_rect.v_rect.bottom =both_rect.v_rect.top+(2+height);
			both_rect.h_rect.top = title_both_rect.h_rect.bottom+1+5*(height+2);
		    both_rect.h_rect.bottom =both_rect.h_rect.top+(2+height);

			GUIAPICTRL_SetBothRect(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID5, &both_rect);
			GUILABEL_SetFont(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID5,MMI_DEFAULT_NORMAL_FONT,theme_ptr.font.color);
        }	                      
		{
        	both_rect.v_rect.top=title_both_rect.v_rect.bottom+1+6*(height+2);
			both_rect.v_rect.bottom =both_rect.v_rect.top+(2+height);
			both_rect.h_rect.top = title_both_rect.h_rect.bottom+1+6*(height+2);
		    both_rect.h_rect.bottom =both_rect.h_rect.top+(2+height);

			GUIAPICTRL_SetBothRect(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID6, &both_rect);
			GUILABEL_SetFont(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID6,MMI_DEFAULT_NORMAL_FONT,theme_ptr.font.color);
        }
		{
			both_prg_rect.v_rect.left=15;
			both_prg_rect.v_rect.right =screen_both_rect.v_rect.right-both_prg_rect.v_rect.left ;
			both_prg_rect.v_rect.top=title_both_rect.v_rect.bottom+1+7*(height+2);
			both_prg_rect.v_rect.bottom =both_prg_rect.v_rect.top+2+height;
			
			both_prg_rect.h_rect.left = 15;
			both_prg_rect.h_rect.right =screen_both_rect.h_rect.right-both_prg_rect.h_rect.left  ;
			both_prg_rect.h_rect.top = title_both_rect.h_rect.bottom+1+7*(height+2);
			both_prg_rect.h_rect.bottom =both_prg_rect.h_rect.top+2+height;
			//GUIPRGBOX_SetRect(MMIPIM_SYNC_PRGBOX_CTRL_ID,&prg_rect);
			GUIAPICTRL_SetBothRect(MMIPIM_SYNC_PRGBOX_CTRL_ID,&both_prg_rect);
        }
		//PIM_T_P  pMe = MMIPIM_GetPIM();
		//g_is_pim_sync=TRUE;//del by wangxiaolin 
             is_first_up=TRUE;
             is_first_down=TRUE;
#ifdef FEA_HS_APP_DCD//added by liangshun 20090804
		dcd_notify_pim_happen();
#endif
#ifdef FEA_HS_DM
		if(MMIAPIDM_IsInDmProcess())
		{
			MMIPUB_OpenAlertWinByTextId(PNULL,
										TXT_COMMON_WAITING,
										TXT_NULL,
										IMAGE_PUBWIN_WAIT,
										PNULL,
										PNULL,
										MMIPUB_SOFTKEY_ONE,PNULL);
		}
		else
#endif		
		{

                   GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, FALSE);
            
                   GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, 1, 1);
                   GUIPRGBOX_SetPosByCtrl(FALSE, 0, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
            
			//MMI_GetLabelTextByLang(TXT_PIM_INITIALIZE, &dis_str);
                    MMI_GetLabelTextByLang(TXT_CONNECTING, &dis_str);
			GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &dis_str, FALSE);


/*Start of  wangxiaolin  2009.11.13 for 为并发处理跳频 */
    #ifndef WIN32
		    if (CHNG_FREQ_REQ_HANDLER_NULL == s_mmipim_chng_freq_req_handler)
		    {
		        s_mmipim_chng_freq_req_handler = CHNG_FREQ_GetReqHandler("mmipim");
		        if(CHNG_FREQ_REQ_HANDLER_NULL == s_mmipim_chng_freq_req_handler)
		        {
		            //SCI_TRACE_LOW:"MMIPIM:s_mmipim_chng_freq_req_handler==NULL, init fail!"
		            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4509_112_2_18_2_47_15_197,(uint8*)"");
		        }
		    } 
			
            if (CHNG_FREQ_REQ_HANDLER_NULL != s_mmipim_chng_freq_req_handler)
            {
	             //SCI_TRACE_LOW:"MMIPIM  start change freq!"
	             SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4515_112_2_18_2_47_15_198,(uint8*)"");
                  CHNG_FREQ_SetArmClk(s_mmipim_chng_freq_req_handler,(CHNG_FREQ_INDEX_E)FREQ_INDEX_WWW);
            }
            else
            {
                //SCI_TRACE_LOW:"MMIPIM_GetFreqHandler Fail !!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4520_112_2_18_2_47_15_199,(uint8*)"");
            }

    #endif
/*End   of  wangxiaolin  2009.11.13 for 为并发处理跳频 */
			MMIPIM_InitGlobleRef(); // chenxiang_20080311

			if(HS_SML_ERR_OK!=hs_Data_Sync_main())
			{
                   		MMK_PostMsg(MMIPIM_SYNC_BEGIN_WIN_ID, MSG_PDP_ACTIVE_FAIL,NULL, NULL);
			}
			
		}
	}
    break;
	
    case MSG_FULL_PAINT: 
		//SCI_TRACE_LOW("MMIPIM enter MSG_FULL_PAINT ");
#ifdef FEA_HS_DM
		if(MMIAPIDM_IsInDmProcess())
		{
			MMK_CloseWin(win_id);
		}
		else
#endif
		{
			IMG_EnableTransparentColor(TRUE); 

			MMIPIM_DisplayWinBackground(IMAGE_COMMON_BG, win_id); 

		}
        break;
        
    case MSG_GET_FOCUS:
    {
       PIM_T_P  pMe = MMIPIM_GetPIM();
       //SCI_TRACE_LOW:"MMIPIM  Sync win ,get focus! server_num=%d,rec_num=%d,client_num=%d,send_num=%d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4557_112_2_18_2_47_15_200,(uint8*)"dddd",pMe->g_sync_result.server_number_of_change,pMe->g_sync_result.receive_num,pMe->g_sync_result.client_number_of_change,pMe->g_sync_result.send_num);

/*Start of  wangxiaolin 2009.11.3 for 解决并发后界面刷新问题 */
		if((STEP_SYNC_OVER==*hs_protocol_step)||(STEP_SYNC_ERROR==*hs_protocol_step))
		{
			if(STEP_SYNC_OVER==*hs_protocol_step)
			{
				GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, 2, 1);
				GUIPRGBOX_SetPosByCtrl(TRUE, 1, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
			}
			else
			{
				if (0 != pMe->g_sync_result.server_number_of_change)
				{
					if((1 == pMe->g_sync_result.server_number_of_change))
					{
						if((is_first_down)&&(pMe->g_sync_result.client_number_of_change==0))
						{
							GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, pMe->g_sync_result.server_number_of_change, 1);
							is_first_down=FALSE;
							GUIPRGBOX_SetPosByCtrl(TRUE, pMe->g_sync_result.receive_num, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
						}
					}
					else
					{
						GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, pMe->g_sync_result.server_number_of_change, 1);
						GUIPRGBOX_SetPosByCtrl(TRUE, pMe->g_sync_result.receive_num, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
					}
				}
				else if (0 != pMe->g_sync_result.client_number_of_change)
				{
					if(1== pMe->g_sync_result.client_number_of_change)
					{
						if(is_first_up)
						{
							GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, pMe->g_sync_result.client_number_of_change, 1);
							is_first_up=FALSE;
							GUIPRGBOX_SetPosByCtrl(TRUE, pMe->g_sync_result.send_num, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
						}
					}
					else
					{
						GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, pMe->g_sync_result.client_number_of_change, 1);
						GUIPRGBOX_SetPosByCtrl(TRUE, pMe->g_sync_result.send_num, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
					}
				}
				else
				{

				}
			}
		}
/*End   of  wangxiaolin 2009.11.3 for 解决并发后界面刷新问题 */
      }
        break;
              
    case MSG_LOSE_FOCUS:
		IMG_EnableTransparentColor(FALSE);
		break;
        
    case PIM_MSG_STOP:
	{
		MMIPUB_OpenAlertWinByTextId(PNULL,
		                    TXT_COMMON_WAITING,
		                    TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
		MMK_PostMsg(MMIPIM_SYNC_BEGIN_WIN_ID, MSG_APP_CANCEL,NULL, NULL);
		break;
	}

     case MSG_PDP_ACTIVE_SUCCESS:
     {
        
        //SCI_TRACE_LOW:"MMIPIM enter MSG_PDP_ACTIVE_SUCCESS ,time=%d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4627_112_2_18_2_47_15_201,(uint8*)"d",PIM_GetCurTime());
        hs_Data_Sync_task();
     }
        break;
        
     case PIM_MSG_SELECT_TIMER:
     {
       int socket_select_result=0;
       //SCI_TRACE_LOW:"MMIPIM enter PIM_MSG_SELECT_TIMER "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4635_112_2_18_2_47_15_202,(uint8*)"");
       socket_select_result=HS_PIM_socket_select_handle();
       if(socket_select_result==-1)
       {
        
        MMI_STRING_T	dis_str = {0};
        MMI_GetLabelTextByLang(TXT_PIM_TIME_OUT, &dis_str);
        GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &dis_str, TRUE);    
         //MMIPIM_DisplayProc();
         *hs_protocol_step=STEP_SYNC_ERROR;
       }
     }
        break;
       
        
     case MSG_PDP_ACTIVE_FAIL:
     {
        //给出提示，pdp激活失败
        MMI_STRING_T	dis_str = {0};
        MMI_GetLabelTextByLang(TXT_PIM_PDP_ACTIVE_FAIL, &dis_str);
        GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &dis_str, TRUE);
         //MMIPIM_DisplayProc();
        //SCI_TRACE_LOW:"MMIPIM enter MSG_PDP_ACTIVE_FAIL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4657_112_2_18_2_47_15_203,(uint8*)"");
         *hs_protocol_step=STEP_SYNC_ERROR;
     }
        break;
        
     case PIM_MSG_SYNC_FAIL:
    {
        int error_status;
        MMI_STRING_T	dis_str = {0};
        //char temp_str[20]={0};
        //wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};        
        error_status = *(int *)param;
        
        MMI_GetLabelTextByLang(TXT_PIM_SYNC_FAIL, &dis_str);

        GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &dis_str, TRUE);
        
        switch(error_status)
        {

           case 400:
                //由于请求命令的语法或句法错误，无法执行,同步失败
                MMI_GetLabelTextByLang(TXT_PIM_SYNC_FAIL, &dis_str);
            break; 
            
            case 401:
                // 请检查用户名或密码是否正确
                MMI_GetLabelTextByLang(TXT_PIM_AUTH_FAIL, &dis_str);
            break;
            
            case 403:
                // 本号码已有同步正在进行,请稍候再试
                MMI_GetLabelTextByLang(TXT_PIM_FAIL_403, &dis_str);
            break;
            
            case 406: 
                //同步失败，不支持该同步方式
                MMI_GetLabelTextByLang(TXT_PIM_ERR_SYNC_TYPE, &dis_str);
            break;
                
            case 407:
                // 请输入用户名和密码
                MMI_GetLabelTextByLang(TXT_COMMON_INPUT_USERNAME_PASSWORD, &dis_str);
            break;

            case 408:
                // 会话超时,请稍候再试
                MMI_GetLabelTextByLang(TXT_PIM_FAIL_408, &dis_str);
            break;
            
            case 412:
                //同步失败，不完整的同步命令
                MMI_GetLabelTextByLang(TXT_PIM_ERR_CMD_INVALID, &dis_str);
            break;
            
            case 420:
                // 电话簿已满
                MMI_GetLabelTextByLang(TXT_PIM_FAIL_420, &dis_str);
            break;
            
            case 500:
                // 服务器发生未知错误，同步失败
                MMI_GetLabelTextByLang(TXT_PIM_ERR_SVR_FAIL, &dis_str);
            break;    
            
            case 501:
                //服务器不支持客户端请求的命令,同步失败
                MMI_GetLabelTextByLang(TXT_PIM_ERR_CMD_INVALID, &dis_str);
            break; 

            case 503:
                // 服务器忙,请稍后再试
                MMI_GetLabelTextByLang(TXT_PIM_FAIL_503, &dis_str);
            break;
            
            case 505:
               //SyncML DTD版本号不支持,同步失败
                MMI_GetLabelTextByLang(TXT_PIM_ERR_VER_NOT_SUPP, &dis_str);
            break; 

            case 510:
                // 请检查数据库设置
                MMI_GetLabelTextByLang(TXT_PIM_FAIL_510, &dis_str);
            break;
            
            case 511:
                //同步失败，服务器故障
                MMI_GetLabelTextByLang(TXT_PIM_ERR_SVR_INNER_ERR, &dis_str);
            break;  
            
            case 512:
                // 同步失败(512)
                MMI_GetLabelTextByLang(TXT_PIM_ERR_SVR_FAIL, &dis_str);
            break; 
            
            case 513:
                // 同步协议不支持
                MMI_GetLabelTextByLang(TXT_PIM_FAIL_513, &dis_str);
            break;
        
            default:
                 MMI_GetLabelTextByLang(TXT_PIM_ERR_SVR_FAIL, &dis_str);
            break;
        }
        GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID4, &dis_str, TRUE);
        //SCI_TRACE_LOW:"MMIPIM enter PIM_MSG_SYNC_FAIL "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4762_112_2_18_2_47_15_204,(uint8*)"");
    }
    break;
    
     case PIM_MSG_COMM_ERROR:
     {
        //给出提示，通信错误
        MMI_STRING_T	dis_str = {0};
        MMI_GetLabelTextByLang(TXT_PIM_COMMUNICATION_ERROR, &dis_str);
        GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &dis_str, TRUE);
        //SCI_TRACE_LOW:"MMIPIM enter PIM_MSG_COMM_ERROR "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4772_112_2_18_2_47_15_205,(uint8*)"");
        *hs_protocol_step=STEP_SYNC_ERROR;
        //hs_Data_Sync_terminate();
     }
        break;
      
    case PIM_MSG_CONN_EST:
    {
        //给出提示，激活成功开始同步
        MMI_STRING_T	dis_str = {0};
        if ((*hs_protocol_step == STEP_CLIENT_INIT_SYNC) || (*hs_protocol_step == STEP_SERVER_INIT_SYNC) )
        {
           
            MMI_GetLabelTextByLang(TXT_PIM_SYNC, &dis_str);
        	GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &dis_str, TRUE);
            //MMIPIM_DisplayProc();
        }
       //SCI_TRACE_LOW:"MMIPIM enter PIM_MSG_CONN_EST "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4789_112_2_18_2_47_15_206,(uint8*)"");
        hs_syncml_Comm_SendInterface();	
     }
    break;
    case PIM_MSG_DATA_SENT:
    {
         //SCI_TRACE_LOW:"MMIPIM enter PIM_MSG_DATA_SENT "
         SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4795_112_2_18_2_47_15_207,(uint8*)"");

        hs_syncml_Comm_SendInterface();	
    }
	break;

   case PIM_MSG_RCV_DONE:
    {
        //SCI_TRACE_LOW:"MMIPIM enter PIM_MSG_RCV_DONE "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4803_112_2_18_2_47_15_208,(uint8*)"");

         hs_syncml_Comm_RecInterface();
   }
	break;
   case PIM_MSG_NEXT_STEP:
   	//相当于data_sync 的主调度函数
   	  //SCI_TRACE_LOW:"MMIPIM enter PIM_MSG_NEXT_STEP "
   	  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4810_112_2_18_2_47_15_209,(uint8*)"");
       {
          MMI_STRING_T	dis_str = {0};
          MMI_STRING_T	dis_str_1 = {0}; // chenxiang
          switch(*hs_protocol_step)
          {
            case STEP_CLIENT_INIT_SYNC:
                break;
             case STEP_SERVER_INIT_SYNC:
                break;
                
             case STEP_CLIENT_MODI:           
             case STEP_SERVER_CONF_MODI:
             case STEP_CLIENT_CONF_MAP:
             case STEP_SERVER_MORE: // chenxiang_pim_ui 20080306
             case STEP_SERVER_MAPACK:
                {
#ifdef HS_PIM_DISPALY_MALLOC_NUM
                char num_usc2[10]={0};
                uint8 ucs2_len=0;
#endif
                wchar ucs2_num1[10]={0};
                uint16 ucs2_len1=0;
                wchar ucs2_num2[10]={0};
                uint16 ucs2_len2=0;
                PIM_T_P  pMe = MMIPIM_GetPIM();
                
                MMI_GetLabelTextByLang(TXT_PIM_SYNC, &dis_str);
#ifdef HS_PIM_DISPALY_MALLOC_NUM
                ucs2_len= MMIPIM_GenDisplayNum(num_usc2, sml_malloc_num);
                MMIPIM_GenDisplayChr(dis_str.wstr_ptr, dis_str.wstr_len, zuokuohao, 1, num_usc2, ucs2_len, youkuohao, 1);
                GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &pim_note_s, TRUE);
                //syncml_task_message:"MMIPIM  ============ malloc_num: %d ================"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4842_112_2_18_2_47_15_210,(uint8*)"d", sml_malloc_num);
#else
                GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &dis_str, TRUE);
#endif
                //SCI_TRACE_LOW:"MMIPIM  Sync win , server_num=%d,rec_num=%d,client_num=%d,send_num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4848_112_2_18_2_47_15_211,(uint8*)"dddd",pMe->g_sync_result.server_number_of_change,pMe->g_sync_result.receive_num,pMe->g_sync_result.client_number_of_change,pMe->g_sync_result.send_num);

                if (0 != pMe->g_sync_result.server_number_of_change)
                {
                    ucs2_len1= MMIPIM_GenDisplayNum(ucs2_num1,pMe->g_sync_result.receive_num);
                    ucs2_len2= MMIPIM_GenDisplayNum(ucs2_num2,pMe->g_sync_result.server_number_of_change);
#if 0//del by wangxiaolin   2010.1.7
                    MMI_GetLabelTextByLang(TXT_PIM_DOWNLOAD, &dis_str); 
#else
                    MMI_GetLabelTextByLang(TXT_PIM_SERVER_TO_PHONE, &dis_str); 
#endif

                    //MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,ucs2_num1,ucs2_len1,divide_char,1,ucs2_num2,ucs2_len2);
                    MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,ucs2_num1,ucs2_len1,PNULL,0,PNULL,0);
          
                    GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID6, &pim_note_s, TRUE);
                    //SCI_TRACE_LOW:"MMIPIM display process download!step=%d "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4864_112_2_18_2_47_15_212,(uint8*)"d",*hs_protocol_step);
                    
                    if (MMK_IsFocusWin(MMIPIM_SYNC_BEGIN_WIN_ID))
                    {
                        if((1 == pMe->g_sync_result.server_number_of_change))
                        {

                            if((is_first_down)&&(pMe->g_sync_result.client_number_of_change==0))
                            {
                                GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, pMe->g_sync_result.server_number_of_change, 1);
                                is_first_down=FALSE;
                                GUIPRGBOX_SetPosByCtrl(TRUE, pMe->g_sync_result.receive_num, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);

                            }

                        }
                        else
                        {
                                GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, pMe->g_sync_result.server_number_of_change, 1);
                                GUIPRGBOX_SetPosByCtrl(TRUE, pMe->g_sync_result.receive_num, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
                        }
                            
                    }

                }
                else if (0 != pMe->g_sync_result.client_number_of_change)
                {
                    ucs2_len1= MMIPIM_GenDisplayNum(ucs2_num1,pMe->g_sync_result.send_num);
                    ucs2_len2= MMIPIM_GenDisplayNum(ucs2_num2,pMe->g_sync_result.client_number_of_change);
#if 0//del by wangxiaolin   2010.1.7
                    MMI_GetLabelTextByLang(TXT_PIM_UPLOAD, &dis_str); // chenxiang 20080806
#else
                    MMI_GetLabelTextByLang(TXT_PIM_PHONE_TO_SERVER, &dis_str); 
#endif
                    MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,ucs2_num1,ucs2_len1,divide_char,1,ucs2_num2,ucs2_len2);

                    GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID6, &pim_note_s, TRUE);
                    //SCI_TRACE_LOW:"MMIPIM display process upload!step=%d  "
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_4901_112_2_18_2_47_16_213,(uint8*)"d",*hs_protocol_step);
                    
                    if (MMK_IsFocusWin(MMIPIM_SYNC_BEGIN_WIN_ID))
                    {

                        if(1== pMe->g_sync_result.client_number_of_change)
                        {
                            if(is_first_up)
                            {
                                GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, pMe->g_sync_result.client_number_of_change, 1);
                                is_first_up=FALSE;
                                GUIPRGBOX_SetPosByCtrl(TRUE, pMe->g_sync_result.send_num, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
                            }
                        }
                        else
                        {
                                GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, pMe->g_sync_result.client_number_of_change, 1);
                                GUIPRGBOX_SetPosByCtrl(TRUE, pMe->g_sync_result.send_num, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
                        }
                    }

                }
                else
                {
                    hs_smlLibMemset((char *)pim_note_s.wstr_ptr,0,70);
                    pim_note_s.wstr_len = 0;
                    GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID6, &pim_note_s, TRUE);
                }
             }
                break;
             case STEP_SYNC_OVER:
             {
                wchar num_usc2[10]={0};
                uint16 ucs2_len=0;
                PIM_T_P  pMe = MMIPIM_GetPIM();
				 
				if (MMK_IsFocusWin(MMIPIM_SYNC_BEGIN_WIN_ID))  
				{
					GUIPRGBOX_SetParamByCtrl(MMIPIM_SYNC_PRGBOX_CTRL_ID, 2, 1);
					GUIPRGBOX_SetPosByCtrl(TRUE, 1, 0, MMIPIM_SYNC_PRGBOX_CTRL_ID);
				}

		   GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_RETURN, TRUE);

                if(pMe->g_sync_result.real_deleted_num_for_server>1) //解决英文模式单复数问题，wangxiaolin 2010.01.05
                {
                    MMI_GetLabelTextByLang(TXT_ITEMS, &dis_str_1); 
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_PIM_ITEM, &dis_str_1); 
                }

                MMI_GetLabelTextByLang(TXT_PIM_SERVER_DELETED, &dis_str); // chenxiang 20080806

                ucs2_len= MMIPIM_GenDisplayNum(num_usc2,pMe->g_sync_result.real_deleted_num_for_server);
                MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,num_usc2,ucs2_len,dis_str_1.wstr_ptr,dis_str_1.wstr_len,0,0);
         
                GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID1, &pim_note_s, TRUE);

                MMI_GetLabelTextByLang(TXT_PIM_SERVER_REPLACED, &dis_str); // chenxiang 20080806
                
                if(pMe->g_sync_result.real_replaced_num_for_server>1) //解决英文模式单复数问题，wangxiaolin 2010.01.05
                {
                    MMI_GetLabelTextByLang(TXT_ITEMS, &dis_str_1); 
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_PIM_ITEM, &dis_str_1); 
                }

                ucs2_len= MMIPIM_GenDisplayNum(num_usc2,pMe->g_sync_result.real_replaced_num_for_server);
                MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,num_usc2,ucs2_len,dis_str_1.wstr_ptr,dis_str_1.wstr_len,0,0);

                GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID2, &pim_note_s, TRUE);

                MMI_GetLabelTextByLang(TXT_PIM_SERVER_ADDED, &dis_str); // chenxiang 20080806
                if(pMe->g_sync_result.real_added_num_for_server>1) //解决英文模式单复数问题，wangxiaolin 2010.01.05
                {
                    MMI_GetLabelTextByLang(TXT_ITEMS, &dis_str_1); 
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_PIM_ITEM, &dis_str_1); 
                }
                
                ucs2_len= MMIPIM_GenDisplayNum(num_usc2,pMe->g_sync_result.real_added_num_for_server);
                MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,num_usc2,ucs2_len,dis_str_1.wstr_ptr,dis_str_1.wstr_len,0,0);
        
                GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID3, &pim_note_s, TRUE);

                MMI_GetLabelTextByLang(TXT_PIM_PHONE_DELETED, &dis_str); 
                
                if(pMe->g_sync_result.deleted_num_for_phone>1) //解决英文模式单复数问题，wangxiaolin 2010.01.05
                {
                    MMI_GetLabelTextByLang(TXT_ITEMS, &dis_str_1); 
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_PIM_ITEM, &dis_str_1);
                }
                
                ucs2_len= MMIPIM_GenDisplayNum(num_usc2,pMe->g_sync_result.deleted_num_for_phone);
                MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,num_usc2,ucs2_len,dis_str_1.wstr_ptr,dis_str_1.wstr_len,0,0);
 
                GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID4, &pim_note_s, TRUE);

                MMI_GetLabelTextByLang(TXT_PIM_PHONE_REPLACED, &dis_str); 
                if(pMe->g_sync_result.replaced_num_for_phone>1) //解决英文模式单复数问题，wangxiaolin 2010.01.05
                {
                    MMI_GetLabelTextByLang(TXT_ITEMS, &dis_str_1); 
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_PIM_ITEM, &dis_str_1); 
                }
                
                ucs2_len= MMIPIM_GenDisplayNum(num_usc2,pMe->g_sync_result.replaced_num_for_phone);
                MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,num_usc2,ucs2_len,dis_str_1.wstr_ptr,dis_str_1.wstr_len,0,0);
 
                GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID5, &pim_note_s, TRUE);

                MMI_GetLabelTextByLang(TXT_PIM_PHONE_ADDED, &dis_str); 
                if(pMe->g_sync_result.added_num_for_phone>1) //解决英文模式单复数问题，wangxiaolin 2010.01.05
                {
                    MMI_GetLabelTextByLang(TXT_ITEMS, &dis_str_1); 
                }
                else
                {
                    MMI_GetLabelTextByLang(TXT_PIM_ITEM, &dis_str_1); 
                }
                ucs2_len= MMIPIM_GenDisplayNum(num_usc2,pMe->g_sync_result.added_num_for_phone);
                MMIPIM_GenDisplayChr(dis_str.wstr_ptr,dis_str.wstr_len,num_usc2,ucs2_len,dis_str_1.wstr_ptr,dis_str_1.wstr_len,0,0);

                GUILABEL_SetText(MMIPIM_SYNC_PROC_SYNC_RESULT_MSG_SHOW_CTRL_ID6, &pim_note_s, TRUE);


                if (SYNC_TYPE_S2WAY==hs_task_relay_info->synctype) 
                {
            #ifdef HS_PIM_VCAL //Added by wangxiaolin   2009.9.17
                if(SYNC_MODE_PB == g_pim.m_sync_mode)
                {
            #endif /* HS_PIM_VCAL */ 
                    MMIPB_UpdateAllPimStatus(PB_NO_MODIFY);
           #ifdef HS_PIM_VCAL //Added by wangxiaolin   2009.9.17
                }
                else   if(SYNC_MODE_VCAL== g_pim.m_sync_mode)
                {
                    MMIVCAL_UpdateAllPimStatus((VCAL_PIM_STATUS_E)PB_NO_MODIFY);
                }
             #endif /* HS_PIM_VCAL */ 
               }
                
/*Start of  wangxiaolin  2010.1.11 for 每个message之后发送map，但缓存所有map，在成功之后清除 */
           #ifdef HS_PIM_CACHE_MAP
                hs_myClearCacheMap();
           #endif
/*End   of  wangxiaolin  2010.1.11 for 每个message之后发送map，但缓存所有map，在成功之后清除 */

                //add by wangxiaolin 2009.11.18 修改超过存储容量时的提示。
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.11.30 for 解决电话簿满时日程同步也提示空间满
                if(SYNC_MODE_PB == g_pim.m_sync_mode)
                {
#endif /* HS_PIM_VCAL */
                    if(0==MMIPB_GetFreeItemsCount())
                    {
                        MMI_GetLabelTextByLang(TXT_COMM_MEMORY_FULL, &dis_str);
                    }
                    else
                    {
                        MMI_GetLabelTextByLang(TXT_PIM_SYNC_SUCCESS, &dis_str);
                    }
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.11.30 for 解决电话簿满时日程同步也提示空间满
                }
                else if(SYNC_MODE_VCAL == g_pim.m_sync_mode)
                {
                    if(0==MMIVCAL_GetFreeItemsCount())
                    {
                        MMI_GetLabelTextByLang(TXT_COMM_MEMORY_FULL, &dis_str);
                    }
                    else
                    {
                        MMI_GetLabelTextByLang(TXT_PIM_SYNC_SUCCESS, &dis_str);
                    }                  
                }
                else
                {
                   //SCI_TRACE_LOW("MMIPIM win_tab sync_mode is error!!!! ");
                }
#endif /* HS_PIM_VCAL */
                GUILABEL_SetText(MMIPIM_SYNC_PROC_MSG_SHOW_CTRL_ID, &dis_str, TRUE);
             }
                break;
                
                default:
                    break;
                                   
          }
        }
         hs_Data_Sync_task();
#ifdef HS_PIM_DISPALY_MALLOC_NUM
         if (hs_protocol_step == NULL) // STEP_SYNC_OVER STEP_SYNC_ERROR
         {
            MMI_STRING_T	dis_mem_inf_str = {0};
            char num_usc2[10]={0};
            uint8 ucs2_len=0;
            BOOLEAN result;

            result = GUILABEL_GetText(MMIPIM_SYNC_PROC_MSG_SHOW_CTRL_ID, &dis_mem_inf_str);
            ucs2_len= MMIPIM_GenDisplayNum(num_usc2, sml_malloc_num);
            MMIPIM_GenDisplayChr(dis_mem_inf_str.wstr_ptr, (uint8)dis_mem_inf_str.wstr_len, zuokuohao, 1, num_usc2, ucs2_len, youkuohao, 1);
            GUILABEL_SetText(MMIPIM_SYNC_PROC_MSG_SHOW_CTRL_ID, &pim_note_s, TRUE);
         }
#endif
	break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
       //SCI_TRACE_LOW:"MMIPIM enter MSG_APP_CANCEL "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_5117_112_2_18_2_47_16_214,(uint8*)"");

       if((STEP_SYNC_OVER==*hs_protocol_step)||(STEP_SYNC_ERROR==*hs_protocol_step))
 	{
		//hs_Data_Sync_terminate();
		MMK_CloseWin(win_id);
		result = MMI_RESULT_FALSE;
  	}
       else
   	{
	   	MMIPUB_OpenQueryWinByTextId(TXT_PIM_CONFIRM_EXIT,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
   	}

        break;

    //按照展讯要求，电源键要返回待机 
    case MSG_APP_RED: // chenxiang 20090115
       //SCI_TRACE_LOW:"MMIPIM enter MSG_APP_RED "
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_5134_112_2_18_2_47_16_215,(uint8*)"");
      // hs_Data_Sync_terminate();
       MMK_CloseWin(win_id);
        result = MMI_RESULT_FALSE;
        break;

		
    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_PROMPTWIN_OK:
     //SCI_TRACE_LOW:"MMIPIM confirm cancel! "
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_5146_112_2_18_2_47_16_216,(uint8*)"");
       
		
      // hs_Data_Sync_terminate();
       MMK_CloseWin(win_id);
       MMIPUB_CloseQuerytWin(PNULL); 

        break;    
    case MSG_APP_OK:
    {
#if 0//del by wangxiaolin  2009.10.6
            //SCI_TRACE_LOW:"MMIPIM enter MSG_APP_OK "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_5157_112_2_18_2_47_16_217,(uint8*)"");
            hs_Data_Sync_terminate();
            MMK_CloseWin(win_id);
#endif
    }
        break;
      
    case MSG_CLOSE_WINDOW:
      //SCI_TRACE_LOW:"MMIPIM enter MSG_CLOSE_WINDOW "
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPIM_WINTAB_5165_112_2_18_2_47_16_218,(uint8*)"");
	hs_Data_Sync_terminate();
     // g_is_pim_sync=FALSE;//del by wangxiaolin 
#ifdef FEA_HS_PB_STYLE // chenxiang 20091202
	 MMIPB_ReloadPBList();
 #endif
 
/*Start of  wangxiaolin  2009.11.13 for 为并发处理跳频  */
#ifndef WIN32
        if (CHNG_FREQ_REQ_HANDLER_NULL != s_mmipim_chng_freq_req_handler)
	    {
	        CHNG_FREQ_RestoreARMClk(s_mmipim_chng_freq_req_handler);
			CHNG_FREQ_DeleteReqHandler(s_mmipim_chng_freq_req_handler);
			s_mmipim_chng_freq_req_handler=CHNG_FREQ_REQ_HANDLER_NULL;
	    }

#endif
/*End   of  wangxiaolin  2009.11.13 for 为并发处理跳频  */
         is_first_up=FALSE;
         is_first_down=FALSE;
#ifdef FEA_HS_APP_DCD//added by liangshun 20090804
      dcd_notify_pim_over();
#endif
        break;
       
            
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return result;
}

/*****************************************************************************/
//  Description :打开号簿管家主菜单窗口
//  Global resource dependence : none
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIM_OpenMainMenuWin(PIM_ENTRY_TYPE entry_type)
{
    if(PIM_ENTRY_FROM_MAIN==entry_type)
    {
        MMK_CreateWin((uint32*)MMIPIM_MAINMENU_WIN_TAB, PNULL);
    }
    else if(PIM_ENTRY_FROM_PB==entry_type)
    {
        MMIPIM_OpenSyncWin();
    }
    else
    {
        return;
    }
}

/*****************************************************************************/
//  Description :打开号簿管家调试参数设置窗口
//  Global resource dependence : none
//  Author: chenxiang
//  Note: 20080506
/*****************************************************************************/
PUBLIC void MMIPIM_OpenDebugMenuWin(void)
{
    MMK_CreateWin((uint32*)MMIPIM_DEBUG_SETTING_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description :恢复默认设定
//  Global resource dependence : none
//  Author: chenxiang
//  Note: chenxiang 20071101 pim_setting
/*****************************************************************************/
PUBLIC void MMIPIM_ResetSetting(void)
{
	const net_setting_info_type	nv_sync_net_info = {"CMWAP","",""};
	const proxy_setting_info_type nv_sync_proxy_info = {1, "10.0.0.172", "80", "", ""};
       const server_setting_info_type nv_sync_syncml_info = {"./contact", "./calendar", PIM_SRV_ADDR_URL, "80", "", ""};
	//MS00216980 cheney
	const server_setting_info_type nv_sync_syncml_info_test = {"./contact", "./calendar", PIM_SRV_ADDR_URL_TEST, "80", "", ""};
       const wap_url_info_type nv_sync_url_info = {"http://pim1.monternet.com/kt.wml", "http://pim1.monternet.com/cx.wml", "http://pim1.monternet.com/zf.wml"};
	const auth_type nv_sync_auth_info = AUTH_B64;
	const SmlEncoding_t nv_sync_encode_info = SML_WBXML;
       

#ifdef HS_PIM_UA_SETTING
    const uint32 nv_ua_type = 0;
    MMIPIM_UA_INFO_T ua_info = {0};
#endif
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16
       sync_type nv_sync_type=SYNC_TYPE_2WAY;
#endif
/*Start of  wangxiaolin  2009.11.14 for 防止当拷贝字符串比原字符串短时会有残留 */
       hs_smlLibMemset(&(g_pim.m_net_setting),0, sizeof(net_setting_info_type));
	hs_smlLibMemset(&g_pim.m_proxy_setting, 0, sizeof(proxy_setting_info_type));
	hs_smlLibMemset(&g_pim.m_server_setting, 0, sizeof(server_setting_info_type));
	hs_smlLibMemset(&g_pim.m_url_setting, 0, sizeof(wap_url_info_type));
	hs_smlLibMemset(&g_pim.m_authtype, 0, sizeof(auth_type));
	hs_smlLibMemset(&g_pim.m_sml_encoding_type, 0, sizeof(SmlEncoding_t));
       hs_smlLibMemset(&g_pim.m_ua_type, 0, sizeof(uint32));
/*End   of  wangxiaolin  2009.11.14 for 防止当拷贝字符串比原字符串短时会有残留 */

       hs_smlLibMemcpy(&(g_pim.m_net_setting),&nv_sync_net_info, sizeof(net_setting_info_type));
	hs_smlLibMemcpy(&g_pim.m_proxy_setting, &nv_sync_proxy_info, sizeof(proxy_setting_info_type));
	//MS00216980 cheney
	if(MMIAPIENG_GetCMCCTestFlag())
	{
		hs_smlLibMemcpy(&g_pim.m_server_setting, &nv_sync_syncml_info_test, sizeof(nv_sync_syncml_info_test));
	}
	else
	{
		hs_smlLibMemcpy(&g_pim.m_server_setting, &nv_sync_syncml_info, sizeof(server_setting_info_type));
	}

	hs_smlLibMemcpy(&g_pim.m_url_setting, &nv_sync_url_info, sizeof(wap_url_info_type));
	hs_smlLibMemcpy(&g_pim.m_authtype, &nv_sync_auth_info, sizeof(auth_type));
	hs_smlLibMemcpy(&g_pim.m_sml_encoding_type, &nv_sync_encode_info, sizeof(SmlEncoding_t));
#ifdef HS_PIM_UA_SETTING
	hs_smlLibMemcpy(&g_pim.m_ua_type, &nv_ua_type, sizeof(uint32));
#endif
#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16
    g_pim.m_sync_type=nv_sync_type;
#endif
	MMI_WriteNVItem(MMINV_PIM_NET_INFO,(void *)&nv_sync_net_info);
	MMI_WriteNVItem(MMINV_PIM_PROXY_INFO,(void *)&nv_sync_proxy_info);
	MMI_WriteNVItem(MMI_PIM_SERVER_INFO,(void *)&nv_sync_syncml_info);
	MMI_WriteNVItem(MMINV_PIM_WAP_URL_INFO,(void *)&nv_sync_url_info);
	MMI_WriteNVItem(MMI_PIM_AUTH_TYPE,(void *)&nv_sync_auth_info);
	MMI_WriteNVItem(MMI_PIM_ENCODE_TYPE,(void *)&nv_sync_encode_info);

#ifdef HS_PIM_UA_SETTING
	MMI_WriteNVItem(MMINV_PIM_UA_TYPE, (void *)&nv_ua_type);
    MMI_WriteNVItem(MMINV_PIM_UA_INFO,&ua_info);
#endif


#ifdef HS_PIM_SUPPORT_OTHER_SYNC_TYPE  //Added by wangxiaolin  2009.7.16
        MMI_WriteNVItem(MMINV_PIM_SYNC_TYPE, &nv_sync_type);
 #endif   
}

/*==========================================================
 * author		: wangxiaolin      
 * function 	: MMIPIM_ResetFactorySetting
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  8/17/2009 
 ==========================================================*/
 PUBLIC void MMIPIM_ResetFactorySetting(void)
 {

           uint8 pb_status_arr[MMIPIM_MAX_PHONEBOOK_RECORDS]={0};
           char  nv_anchor[MMIPIM_ANCHOR_LEN]={0};
           
            MMI_WriteNVItem(MMI_PIM_PB_LOG_INFO,pb_status_arr);
            
           MMI_WriteNVItem(MMIPIM_LAST_ANCHOR,(void *)nv_anchor);
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.12.8
           MMI_WriteNVItem(MMIPIM_VCAL_LAST_ANCHOR,(void *)nv_anchor);
#endif /* HS_PIM_VCAL */

           hs_smlLibMemset(nv_anchor,0,sizeof(nv_anchor));
           MMI_WriteNVItem(MMINV_PIM_NEXT_ANCHOR,(void *)nv_anchor);

           MMIPIM_ResetSetting();

 }

/**********************************************************************************
 *Function: MMIPIM_OpenNetQueryWinFromPB
 *
 *author: chenxiang 20091102
 * Purpose: 
 * Relation: 
 * Params: 
 *
 *   Name                Type            In/Out          Description
 * --------              ----            ------          ----------- 
 *
 * Return: 
 * Notes: NO
 **********************************************************************************/
PUBLIC void MMIPIM_OpenNetQueryWinFromPB(void)
{
    MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
    wap_url_info_type	nv_sync_url_info={0};
    wchar ucs2buf[MMIPIM_MAX_STR_LEN+1] = {0};
    const wap_url_info_type default_sync_url_info = {"http://pim1.monternet.com/kt.wml", "http://pim1.monternet.com/cx.wml", "http://pim1.monternet.com/zf.wml"};//wangzhefeng add 20091029

    //get url setting from NV
    return_value = MMI_ReadNVItem(MMINV_PIM_WAP_URL_INFO, &nv_sync_url_info);
    if (MN_RETURN_SUCCESS != return_value)
    {
        hs_smlLibMemcpy(&nv_sync_url_info, &default_sync_url_info, sizeof(wap_url_info_type));
        MMI_WriteNVItem(MMINV_PIM_WAP_URL_INFO,&nv_sync_url_info);
    }

    MMIAPICOM_StrToWstr((uint8*)nv_sync_url_info.net_query, ucs2buf);
    MMIPIM_OpenWebWin(nv_sync_url_info.net_query); // chenxiang 20071225
}

/*==========================================================
 * author		: renyi.hu     
 * function 	: HandlePIMSyncMLSettingMenuSyncTypeWinMsg
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/16/2009 
 ==========================================================*/
LOCAL MMI_RESULT_E  HandlePIMSyncMLSettingMenuSyncTypeWinMsg(
										MMI_WIN_ID_T win_id, 
										MMI_MESSAGE_ID_E msg_id, 
										DPARAM param
										)
{
    MMI_RESULT_E err_code = MMI_RESULT_TRUE;
    uint16                      cur_selection = 0;
    MN_RETURN_RESULT_E	    return_value = MN_RETURN_FAILURE;
    sync_type  nv_sync_sync_type={0};
    MMI_CTRL_ID_T ctrl_id=MMIPIM_SYNCML_SETTING_SYNC_TYPE_RADIO_CTRL_ID;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem( ctrl_id, 2, FALSE );

		MMIAPISET_AppendListItemByTextId(TXT_PIM_STR_SYNC_TYPE_2WAY,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_PIM_STR_SYNC_TYPE_S2WAY,TXT_COMMON_OK,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		
		 //get encode type from NV
		return_value = MMI_ReadNVItem(MMINV_PIM_SYNC_TYPE, &nv_sync_sync_type);
		if (MN_RETURN_SUCCESS != return_value)
		{
			nv_sync_sync_type=SYNC_TYPE_2WAY;
			MMI_WriteNVItem(MMINV_PIM_SYNC_TYPE,&nv_sync_sync_type);
		}

        cur_selection=(uint16)nv_sync_sync_type-1;
              
		if(cur_selection<2)
		{
			//set selected item
			GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
			//set current item
			GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
		}
		else//有可能在工程模式下设置其他同步模式，正式菜单中只有2种
		{
			GUILIST_SetCurItemIndex(ctrl_id,0);
		}
	
		MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
		
           cur_selection = GUILIST_GetCurItemIndex( ctrl_id );		

           nv_sync_sync_type=(sync_type)cur_selection+1;

		//修改NV 项
		MMI_WriteNVItem(MMINV_PIM_SYNC_TYPE,&nv_sync_sync_type);
		// 保存至全局变量g_pim
		g_pim.m_sync_type = nv_sync_sync_type;
		MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);

		MMK_CloseWin( win_id );
        break;
        
    case MSG_APP_CANCEL:    
    case MSG_CTL_CANCEL:     
		// close the option window
		MMK_CloseWin(win_id);
		break;
			
	default:
		err_code = MMI_RESULT_FALSE;
		break;
    }
    
    return err_code;
}

/*****************************************************************************/
//  Description : operate sim select
//  Global resource dependence : 
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
MMI_RESULT_E PIMSIMSelectCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    if(isReturnOK)
    {
        MMIPIM_SetSim((MN_DUAL_SYS_E)dual_sys);
        MMK_CreateWin((uint32*)MMIPIM_SYNC_BEGIN_WIN_TAB, PNULL);
    }
    return recode;
}
/*****************************************************************************/
//  Description :设置当前使用的sim 卡
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void MMIPIM_SetSim(MN_DUAL_SYS_E sim)
{
    if(sim < MMI_DUAL_SYS_MAX)
    {
        s_pim_sim=sim;
    }
}

/*****************************************************************************/
//  Description :设置当前使用的sim 卡
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPIM_GetSim(void)
{
    return s_pim_sim;
}
/*****************************************************************************/
//  Description :打开同步窗口
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
LOCAL void MMIPIM_OpenSyncWin(void)
{
    uint16  sim_ok = 0;
    uint32  sim_num = 0;

    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_ok, 1);
    if (0 == sim_num)
    {
        MMIAPIPHONE_AlertSimNorOKStatus();
    }
    else if (1 == sim_num)
    {
        MMIPIM_SetSim((MN_DUAL_SYS_E)sim_ok);
        MMK_CreateWin((uint32*)MMIPIM_SYNC_BEGIN_WIN_TAB, PNULL);
    }
    else
    {
        MMIPIM_SelectSIMFunc();
    }
}


/*****************************************************************************/
//  Description : Select SIM API of PIM app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIPIM_SelectSIMFunc(void)
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MMI_WIN_ID_T    selectSimWinID = MMIPIM_COM_SELECT_SIM_WIN_ID;
    MMIPHONE_SELECT_SIM_DATA_T  caller_data={0};

    caller_data.append_type = (uint32)MMISET_APPEND_SYS_OK;
    caller_data.callback_func =PIMSIMSelectCallback ;
    caller_data.select_type= MMIPHONE_SELECTION_SIM_ONLY;

    caller_data.publist_id = MMIPIM_COM_SELECT_SIM_CTRL_ID;
    caller_data.win_id_ptr = &selectSimWinID;
    caller_data.user_data  = PNULL;
    caller_data.extra_data = PNULL;
    

    MMIAPIPHONE_OpenSelectSimWin(&caller_data);

#endif
    return MMI_RESULT_TRUE;
}






/*Edit by script, ignore 32 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
