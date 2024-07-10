/*****************************************************************************
** File Name:      mmimtv_main.c                                             *
** Author:                                                                   *
** Date:           2008-12	                                                 *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe mtv control module          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008        xin.li           Create									 *
*****************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"
#include "mmi_app_mobiletv_trc.h"
#ifdef CMMB_SUPPORT
#include "mtv_api.h"
#include "tasks_id.h"
#include "guilcd.h"
#include "guires.h"
#include "gui_ucs2b_converter.h"
#include "guiedit.h"
#include "guitab.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "mmk_msg.h"
#include "mmidisplay_data.h"
#include "mmi_text.h"
#include "block_mem.h"
#include "block_mem_def.h"
#include "mmicc_export.h"
#include "mmisms_export.h"
#include "mmifmm_id.h"
#include "mmieng_win.h"
#include "mmimtv_main.h"
#include "mmimtv_internal.h"
#include "mmimtv_id.h"
#include "mmimtv_text.h"
#include "mmimtv_osd_display.h"
#include "mmimtv_data.h"
#include "mmimtv_nv.h"
#include "lcd_tv.h"
#include "mmi_default.h"

//#include "mmimtv_simu.h"
#include "dal_lcd.h"
#include "mmiidle_export.h"
#include "mmimtv_image.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmicom_time.h"
#include "mmi_solarlunar.h"
#include "jpeg_interface.h"
#include "sig_code.h"
#include "mmiphone_export.h"

#ifdef MBBMS_SUPPORT
#include "mbbms_service.h"
#include "mbbms_auth.h"
#include "mbbms_sg_api.h"
#include "mmimbbms_main.h"
#include "mmimbbms_wintab.h"
#include "mmi_appmsg.h"
//#ifdef WIN32
#include "mbbms_api_dummy.h"
#include "Mua_cmd.h"
#include "mbbms_sub.h"
//#endif
#endif
#include "mmimtv_menutable.h"
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MTV_DECEMBER        12
#define MS_PER_SECOND       1000



/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
LOCAL LCDTV_SPEC_T                  s_lcdtv_spec    = {0};
LOCAL LCDTV_OSD_SPEC_T              s_lcdosd_spec   = {0,0,MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_MAINSCREEN_BOTTOM_MAX_PIXEL, \
                                                        MMIMTV_TRANSPARENT_COLOR,MMIMTV_TRANSPARENT_COLOR,255,FALSE};
LOCAL LCDTV_VIDEO_SPEC_T                  s_lcdvideo_spec = {0,0,MMI_MAINSCREEN_RIGHT_MAX_PIXEL, MMI_MAINSCREEN_BOTTOM_MAX_PIXEL, \
                                                        0,0,LCDTV_ANGLE_90,TRUE,TRUE,TRUE,0,FALSE};
LOCAL const GUI_LCD_DEV_INFO              s_dev_info      = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};

LOCAL MMIMTV_INFO_T                 s_mtv_current_para = {0};           //当前正在播放的节目参数及信息
LOCAL MMIMTV_PROG_ID_T              s_mtv_last_prog_ifo = {0};          //最近一次播放的节目信息，重新进入时播放
LOCAL MMIMTV_NUM_SWITCH_INFO_T      s_num_swith_info = {0};             //数字键切换频道信息
LOCAL MMIMTV_OSD_SCENE_TYPE_E       s_focus_osd = 0;                    //当前打开的OSD场景
LOCAL MMIMTV_OSD_SCENE_TYPE_E       s_pre_osd = OSD_NONE;               //记录需要返回的场景
LOCAL MMIMTV_OSD_SCENE_TYPE_E       s_source_osd = OSD_NONE;            //记录来源场景，适用与多个入口的情况
LOCAL uint32                        s_osd_restore_param = 0;            //恢复场景参数
LOCAL MMIMTV_SAVE_NETEID_INFO_T     *s_select_net_node_ptr = PNULL;     //当前选中的网络
LOCAL MMIMTV_SAVE_SERVICE_INFO_T    *s_select_service_node_ptr = PNULL; //当前选中的频道
LOCAL BOOLEAN                       s_is_mtv_open    = FALSE;           //MTV是否开启
LOCAL MMIMTV_PROG_HINT_INFO_T       *s_mtv_prog_hint_info_ptr = PNULL;  //节目提示信息
LOCAL MMIMTV_NETWORK_SEARCH_MODE_T  s_search_mode = {0};                //搜索模式
LOCAL MMIMTV_NV_CTL_T               *s_mtv_nv_ctl_table_ptr = PNULL;    //搜索到的所有网络的控制信息
LOCAL uint32                        s_mtv_record_time_count = 0;        //回放录像计时器
LOCAL uint32                        s_mtv_record_file_total_time = 0;   //录像文件的总时长
LOCAL MMIMTV_ESG_LIST_SCENE_T		s_esglist_param = {0};				//ESG参数信息 
LOCAL BOOLEAN                       s_is_mbbms_available = FALSE;       //MBBMS功能当前是否可用（此状态由MBBMS根据自身信号状况更新此状态）
LOCAL BOOLEAN                       s_is_delay_poweroff =FALSE;

#ifdef MBBMS_SUPPORT
LOCAL BOOLEAN     s_is_need_play_service = TRUE;      //MBBMS 是否需发起播放
LOCAL uint32      s_msk_key_domain_id = 0;
LOCAL uint32      s_msk_id = 0;
LOCAL MMIMBBMS_FLOW_STATE_E s_msk_update_status = MMIMBBMS_FLOW_STATE_IDLE;
#endif

LOCAL MMISRV_HANDLE_T s_mtv_audio_handle = 0;

/*lint -e526*/
// extern KDuint32 CMMB_GetEsgDateRange(KDuint16     network_id,   /*lint !e526 */
//                               KDuint16     service_id,
//                               MTV_DATE_T  *sdate_ptr,
//                               MTV_DATE_T  *edate_ptr);
// #ifndef WIN32
// extern uint32 MMI_GetFreqHandler(void);
// #endif
/*lint -e526*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/***********************************************************************/
// 	Description : This function is used to callback. 
//	Global resource dependence : none
//  Author:
//	Note:
/***********************************************************************/
LOCAL void MMIMTV_CallBack(uint32 ack_id, uint32 para0, uint32 para1, uint32 para2);

/*****************************************************************************/
// 	Description : send signal to task
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void SendSignalToTask(uint16 sig_code,void *signal_data_ptr,uint32 task_id);

/*****************************************************************************/
// 	Description : set is_recived_stop_ack when received MTV_STOP_ACK msg
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void SetIsReceivedStopACK(void);

/*****************************************************************************/
// 	Description : get the last played net and service id 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetLastServiceId(void);

/*****************************************************************************/
// 	Description : save the last played net and service id 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SaveLastServiceId(void);

/*****************************************************************************/
// 	Description : read network control table data from NV
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadNetCtrlTable(void);

/*****************************************************************************/
// 	Description : save network control table data to NV
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void WriteNetCtrlTable(void);

/*****************************************************************************/
// 	Description : set network control table data
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void SetNetCtrlTableData(void);

/*****************************************************************************/
// 	Description : parse network info after search
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void ParseNetworkInfoListData(
                                    MTV_STIMI_NET_INFO_LIST_T network_info_list //network info list
                                    );

/*****************************************************************************/
// 	Description : parse program hint info data
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void ParseProgramHintData(
                                ESG_PROG_HINT_INFO_T *prog_hintinfo //pointer of program hint info struct
                                );

/*****************************************************************************/
// 	Description : parse program hint info data
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseEmergencyBroadcastData(MTV_STIMI_SIG_DATA_T *sig_data, 
                                          uint32 *eb_grade);   //pointer of stimi signal data struct

/*****************************************************************************/
// 	Description : play default service after search
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN PlayDefaultServiceAfterSearch(void); /*lint !e528 */

/*****************************************************************************/
// 	Description : resume play service after search esg or program
//	Global resource dependence : none
//  Author: xin.li
//	Note: 搜索ESG或搜网失败后恢复节目播放
/*****************************************************************************/
LOCAL void ResumePlayServiceAfterSearch(void);

/*****************************************************************************/
// 	Description : get next service id in current network
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNextServiceId(void);

/*****************************************************************************/
// 	Description : get previous service id in current network
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPreServiceId(void);

/*****************************************************************************/
// 	Description : switch channel(service) play
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SwitchServicePlay(void);

/*****************************************************************************/
// 	Description : stop current playing
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StopServicePlay(void);

/*****************************************************************************/
// 	Description : 获取数字键切台信息
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMIMTV_NUM_SWITCH_INFO_T* GetNumSwitchInfo(void);

/*****************************************************************************/
// 	Description : start record tip timer
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void StartRecordTipTimer(void);

/*****************************************************************************/
// 	Description : stop record tip timer
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StopRecordTipTimer(void);

/*****************************************************************************/
// 	Description : play record file
//	Global resource dependence : none
//  Author: xin.li
//	Note: 播放本地录制文件
/*****************************************************************************/
LOCAL void PlayRecordfile(void);

/*****************************************************************************/
// 	Description : launch auto search (自动全搜索)
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StartAutoSearch(void);

/*****************************************************************************/
// 	Description : launch frequency search (手动搜索/按频点搜索)
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StartFrequencySearch(void);

/*****************************************************************************/
// 	Description : esg search
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StartESGSearch(void);

/*****************************************************************************/
// 	Description : ESG parse
//	Global resource dependence : none
//  Author: xin.li
//	Note: 解析是异步的, 解析完成MTV_ESG_PARSE_RESULT_IND
/*****************************************************************************/
LOCAL void StartESGParse(uint32 net_id);

/*****************************************************************************/
// 	Description : reset and quit
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void ResetAndQuit(void);

/*****************************************************************************/
// 	Description : reset CMMB module
//	Global resource dependence : none
//  Author: xin.li
//	Note: 播放录制节目前重置CMMB模块，解决回放卡，易ASSERT，定屏等问题
/*****************************************************************************/
LOCAL void ResetCMMBModule(void);

/*****************************************************************************/
// 	Description : whether current osd scene contain editbox
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsFocusOsdWithEditbox(void);

/*****************************************************************************/
// 	Description : 提示播放错误，需重启
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenPlayNetworkErrScene(void);

/*****************************************************************************/
// 	Description : 提示播放录像文件错误
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenPlayStreamErrScene(void);

/*****************************************************************************/
// 	Description : 提示搜索网络失败
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenSearchProgramErrScene(void);

/*****************************************************************************/
// 	Description : open play connecting scene
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenPlayConnectingScene(void);

/*****************************************************************************/
// 	Description : OSD_NONE 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleNoneSceneMsg(
                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                              DPARAM                param   //the param of the message
                              );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN AllocAndGetESGInfo(
                                 SCI_DATE_T date,               //date for esg
                                 MMIMTV_ESG_LIST_SCENE_T  *esg_param    //service info
                                 );


/*****************************************************************************/
// 	Description : OSD_ENTER_WARNING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleEnterWarningSceneMsg(
                                      MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                      DPARAM                param   //the param of the message
                                      );

/*****************************************************************************/
// 	Description : OSD_INITIALIZING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleInitializingSceneMsg(
                                      MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                      DPARAM                param   //the param of the message
                                      );

/*****************************************************************************/
// 	Description : OSD_NO_LAST_SERVICE 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleNoLastServiceSceneMsg(
                                       MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                       DPARAM                param   //the param of the message
                                       );

/*****************************************************************************/
// 	Description : OSD_IDLE 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleIdleSceneMsg(
                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                              DPARAM                param   //the param of the message
                              );

/*****************************************************************************/
// 	Description : OSD_SHORTCUT_ICON 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleShortcutIconSceneMsg(
                                      MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                      DPARAM                param   //the param of the message
                                      );

/*****************************************************************************/
// 	Description : OSD_MAIN_MENU 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleMainMenuSceneMsg(
                                  MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                  DPARAM                param   //the param of the message
                                  );

/*****************************************************************************/
// 	Description : OSD_QUIT_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleQuitQuerySceneMsg(
                                   MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                   DPARAM                param   //the param of the message
                                   );

/*****************************************************************************/
// 	Description : OSD_NET_LIST 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleNetListSceneMsg(
                                 MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                 DPARAM                 param   //the param of the message
                                 );

/*****************************************************************************/
// 	Description : OSD_NET_LIST_OPT 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleNetListOptSceneMsg(
                                    MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                    DPARAM                  param   //the param of the message
                                    );

/*****************************************************************************/
// 	Description : OSD_DEL_NET_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleDelNetQuerySceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                     DPARAM                 param   //the param of the message
                                     );

/*****************************************************************************/
// 	Description : OSD_DEL_NET_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleDelEBQuerySceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                     DPARAM                 param   //the param of the message
                                     );

/*****************************************************************************/
// 	Description : OSD_DEL_NET_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleDelAllEBQuerySceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                     DPARAM                 param   //the param of the message
                                     );

/*****************************************************************************/
// 	Description : OSD_SERVICE_LIST 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleServiceListSceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                     DPARAM                 param   //the param of the message
                                     );

/*****************************************************************************/
// 	Description : OSD_SERVICE_LIST_OPT 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleServiceListOptSceneMsg(
                                        MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                        DPARAM                  param   //the param of the message
                                        );

/*****************************************************************************/
// 	Description : OSD_DEL_SERVICE_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleDelServiceQuerySceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                         DPARAM                 param   //the param of the message
                                         );


/*****************************************************************************/
// 	Description : OSD_SEARCH_WAITING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchWaitingSceneMsg(
                                       MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                       DPARAM                   param   //the param of the message
                                       );

/*****************************************************************************/
// 	Description : OSD_SEARCH_INVALID_FRQ 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchInvalidFrqSceneMsg(
                                          MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                          DPARAM                param   //the param of the message
                                          );

/*****************************************************************************/
// 	Description : OSD_SEARCH_MENU 搜索菜单场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchMenuSceneMsg(
                                    MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                    DPARAM                  param   //the param of the message
                                    );

/*****************************************************************************/
// 	Description : OSD_MANUAL_SEARCH 手动搜索场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleManualSearchSceneMsg(
                                      MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                      DPARAM                param); //the param of the message

/*****************************************************************************/
// 	Description : OSD_EB_LIST 紧急广播列表场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleEBListSceneMsg(
                                MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                DPARAM                  param   //the param of the message
                                );

/*****************************************************************************/
// 	Description : OSD_EB_LIST 紧急广播列表场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleEBListOptSceneMsg(
                                MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                DPARAM                  param   //the param of the message
                                );

/*****************************************************************************/
// 	Description : OSD_EB_LIST 紧急广播列表场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleEBDetailSceneMsg(
                                MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                DPARAM                  param   //the param of the message
                                );

/*****************************************************************************/
// 	Description : OSD_BOOKING_RECORD 预约录制场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleBookingRecordSceneMsg(
                                       MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                       DPARAM                   param   //the param of the message
                                       );

/*****************************************************************************/
// 	Description : OSD_ESG_LIST 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleESGListSceneMsg(
                                 MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                 DPARAM                   param   //the param of the message
                                 );

/*****************************************************************************/
// 	Description : OSD_SEARCH_ESG_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchESGQuerySceneMsg(
                                        MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                        DPARAM                   param   //the param of the message
                                        );

/*****************************************************************************/
// 	Description : OSD_SEARCH_ESG_WAITING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchESGWaitingSceneMsg(
                                          MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                          DPARAM                   param   //the param of the message
                                          );

/*****************************************************************************/
// 	Description : OSD_PUB 场景处理函数
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void HandlePubSceneMsg(
                             MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                             DPARAM                   param   //the param of the message
                             );

/*****************************************************************************/
// 	Description : OSD_TO_OTHER_APP 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleToOtherAppSceneMsg(
                                    MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                    DPARAM                  param   //the param of the message
                                    );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingSceneMsg(
                                 MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                 DPARAM                 param   //the param of the message
                                 );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingAudioLanguageSceneMsg(
                                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                              DPARAM                param   //the param of the message
                                              );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingSubtitleSceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                         DPARAM                 param   //the param of the message
                                         );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingAudioModeSceneMsg(
                                          MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                          DPARAM                param   //the param of the message
                                          );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingZoomModeSceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E msg_id,  //the identify of message
                                         DPARAM                param    //the param of the message
                                         );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingScreenAdjustSceneMsg(
                                             MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                             DPARAM                 param   //the param of the message
                                             );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingRecordStoreSceneMsg(
                                            MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                            DPARAM                  param   //the param of the message
                                            );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingSnapshotStoreSceneMsg(
                                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                              DPARAM                param   //the param of the message
                                              );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingTransparencySceneMsg(
                                             MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                             DPARAM                 param   //the param of the message
                                             );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingDisplayTimeSceneMsg(
                                            MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                            DPARAM                  param   //the param of the message
                                            );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
// LOCAL void HandleSettingWatchTipSceneMsg(
//                                          MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
//                                          DPARAM                 param   //the param of the message
//                                          );

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingBroadcastBGpicSceneMsg(
                                               MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                               DPARAM                   param   //the param of the message
                                               );

/*****************************************************************************/
// 	Description : OSD_HELP 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleHelpSceneMsg(
                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                              DPARAM                param   //the param of the message
                              );

/*****************************************************************************/
// 	Description : OSD_RECORD_LOADING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 录像加载等待，主要是播放录像前RESET CMMB需要数秒时间
/*****************************************************************************/
LOCAL void HandleRecordLoadingSceneMsg(
									   MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
									   DPARAM                param   //the param of the message
									   );

/*****************************************************************************/
// 	Description : OSD_RECORD_PLAY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 本地回放场景处理函数
/*****************************************************************************/
LOCAL void HandleRecordPlaySceneMsg(
                                    MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                    DPARAM                param   //the param of the message
                                    );

/*****************************************************************************/
// 	Description : OSD_PLAY_CONNECTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: MBBMS 播放连接等待
/*****************************************************************************/
LOCAL void HandlePlayConnectingSceneMsg(
                                        MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                        DPARAM                param   //the param of the message
                                        );

#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : OSD_FAVORITE_OPT 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleFavoriteServiceOptSceneMsg(
                                            MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                            DPARAM                param   //the param of the message
                                            );

/*****************************************************************************/
// 	Description : OSD_READ_EB_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 低优先级广播察看询问
/*****************************************************************************/
LOCAL void HandleReadEBQuerySceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                     DPARAM                param   //the param of the message
                                     );

/*****************************************************************************/
// 	Description : OSD_SUBSCRIBLE_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: MBBMS套餐订购询问
/*****************************************************************************/
LOCAL void HandleSubscribleQuerySceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                         DPARAM                param   //the param of the message
                                         );

/*****************************************************************************/
// 	Description : OSD_CMMB_MODE_WARNING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 无SIM卡或无网络提示场景，确认后消失
/*****************************************************************************/
LOCAL void HandleCMMBModeWarningSceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                         DPARAM                param   //the param of the message
                                         );

/*****************************************************************************/
// 	Description : show add/del favorite service result tip
//	Global resource dependence : none
//  Author: xin.li
//	Note: 收藏/取消收藏 当前频道提示
/*****************************************************************************/
LOCAL void ShowFavoriteOptTipMsg(MMIMBBMS_OPER opt_type);

/*****************************************************************************/
//  Description : set update status of MSK   
//  Global resource dependence : 
//  Author: xiaoxiang.huang
//  Note: 
/*****************************************************************************/
LOCAL MMIMBBMS_FLOW_STATE_E GetMSKUpdateStatus(void);

#endif

/*---------------------------------------------------------------------------*/
/*                          CONSTANT VARIABLES                               */
/*---------------------------------------------------------------------------*/
//OSD scene msg handle table
LOCAL const MMIMTV_OSD_MSG_PFUNC s_osd_msg_handle_tab[OSD_ID_MAX + 1] = {
    HandleNoneSceneMsg,
    HandleEnterWarningSceneMsg,
    HandleInitializingSceneMsg,
    HandleNoLastServiceSceneMsg,
    HandleIdleSceneMsg,
    HandleShortcutIconSceneMsg,
    HandleMainMenuSceneMsg,
    HandleQuitQuerySceneMsg,
    HandleNetListSceneMsg,
    HandleNetListOptSceneMsg,
    HandleDelNetQuerySceneMsg,
    HandleServiceListSceneMsg,
    HandleServiceListOptSceneMsg,
    HandleDelServiceQuerySceneMsg,
    HandleSearchWaitingSceneMsg,
    HandleSearchInvalidFrqSceneMsg,
    HandleSearchMenuSceneMsg,
    HandleManualSearchSceneMsg,
    PNULL,  //OSD_CITY_SEARCH
    PNULL,  //OSD_CITY_SEARCH_OPT
    PNULL,  //OSD_CITY_EDIT
    PNULL,  //OSD_CITY_ADD
    PNULL,  //OSD_CITY_DEL_QUERY
    HandleEBListSceneMsg,
    HandleEBListOptSceneMsg,  //OSD_EB_LIST_OPT
    HandleEBDetailSceneMsg,  //OSD_EB_DETAIL
    HandleDelEBQuerySceneMsg,
    HandleDelAllEBQuerySceneMsg,
    HandleBookingRecordSceneMsg,
    PNULL,  //OSD_BOOKING_NAME
    PNULL,  //OSD_BOOKING_SERVICE_LIST
    PNULL,  //OSD_BOOKING_TIME_INFO
    HandleESGListSceneMsg,
    HandleSearchESGQuerySceneMsg,
    HandleSearchESGWaitingSceneMsg,
    HandlePubSceneMsg,
    HandleToOtherAppSceneMsg,
    HandleSettingSceneMsg,
    HandleSettingAudioLanguageSceneMsg,
    HandleSettingSubtitleSceneMsg,
    HandleSettingAudioModeSceneMsg,
    HandleSettingZoomModeSceneMsg,
    HandleSettingScreenAdjustSceneMsg,
    HandleSettingRecordStoreSceneMsg,
    HandleSettingSnapshotStoreSceneMsg,
    HandleSettingTransparencySceneMsg,
    HandleSettingDisplayTimeSceneMsg,
    //HandleSettingWatchTipSceneMsg,
    HandleSettingBroadcastBGpicSceneMsg,
    HandleHelpSceneMsg,
	HandleRecordLoadingSceneMsg,
    HandleRecordPlaySceneMsg,
    HandlePlayConnectingSceneMsg,
#ifdef MBBMS_SUPPORT
    HandleFavoriteServiceOptSceneMsg,
    HandleReadEBQuerySceneMsg,
    HandleSubscribleQuerySceneMsg,
    HandleCMMBModeWarningSceneMsg,
#endif
    PNULL
};



/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : enter mtv
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_EnterMTV(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_EnterMTV"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_860_112_2_18_2_40_23_601,(uint8*)"");
    s_is_mtv_open = TRUE;
    MAIN_SetTVSignalState();
#ifndef MBBMS_SUPPORT
    MMIMTV_SetFocusOSDScene(OSD_ENTER_WARNING);
#else
    MMIMTV_SetFocusOSDScene(OSD_CMMB_MODE_WARNING);
#endif
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
}

/*****************************************************************************/
// 	Description : exit mtv
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ExitMTV(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_ExitMTV  current play status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_877_112_2_18_2_40_23_602,(uint8*)"d", s_mtv_current_para.play_status);
    //直接关闭当前场景, 确保控件被注销
    MMIMTV_CloseFocusScene();
    //不再显示TV信号强度
    MAIN_SetTVSignalFalse();
    
    if(MMK_IsOpenWin(MMIMTV_MAIN_WIN_ID))
    {
        //SCI_TRACE_LOW:"[MMIMBBMS]: MMIMBBMS_CloseCMMB MTV WIN is opened!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_885_112_2_18_2_40_23_603,(uint8*)"");
        //关闭CMMB播放窗口
        MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
    }
    
    //确保关闭录制节目列表窗口
    if(MMK_IsOpenWin(MMIFMM_MAIN_WIN_ID))
    {
        MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
    }
    
    ResetAndQuit();
}

/*****************************************************************************/
// 	Description : whether mtv player is opened
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsMTVOpen(void)
{
    return s_is_mtv_open;
}

/*****************************************************************************/
// 	Description : handle key msg etc.
//	Global resource dependence :
//  Author:xin.li
//	Note: 将映射后的场景消息直接分发至当前OSD场景处理
//        注意：TV窗口失去焦点时，消息被丢弃
/*****************************************************************************/
PUBLIC void MMIMTV_HandleMsg(
                             MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                             DPARAM                 param   //the param of the message
                             )
{
    //SCI_TRACE_LOW("[MMIMTV]: MMIMTV_HandleMsg s_focus_osd = %d  msg_id = 0x%x", s_focus_osd, msg_id);
    if((s_focus_osd >= OSD_ID_MAX)
        || (PNULL == s_osd_msg_handle_tab[s_focus_osd]))
    {
        return;
    }
    
    if(MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
    {
        s_osd_msg_handle_tab[s_focus_osd](msg_id, param);
        if(MMIMTV_OPEN_SCENE == msg_id)
        {
            MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
        }
    }
}

/*****************************************************************************/
// 	Description : handle timer msg
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleTimerMsg(
                                  uint8 timer_id    //timer id
                                  )
{
    //SCI_TRACE_LOW("[MMIMTV]: MMIMTV_HandleTimerMsg, timer_id = 0x%x", timer_id);
    
    if(MMIMTV_GetDisappearTimer() == timer_id)
    {
        MMIMTV_HandleSystimer();
    }
    else if(s_mtv_current_para.record_tip_timer_id == timer_id)
    {
        s_mtv_current_para.record_tip_time_count++;
        MMIMTV_DisplayRecordTip(s_mtv_current_para.record_tip_time_count);
    }
    else if(MMIMTV_GetSwitchPlayTimerId() == timer_id)
    {
        MMIMTV_HandleSwitchPlayTimer();
    }
    else if(MMIMTV_GetStbDisableTimerId() == timer_id)
    {
        MMIMTV_HandleStbDisableTimer();
    }    
}

/*****************************************************************************/
// 	Description : handle timer msg
//	Global resource dependence :
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleTPMsg(
                               DPARAM param     ////the param of the message
                               )
{
    GUI_POINT_T point = {0};
    GUI_RECT_T left_rect = {0};
    GUI_RECT_T right_rect = {0};
    GUI_RECT_T middle_rect = {0};
    GUI_RECT_T left_arrow = {0};
    GUI_RECT_T right_arrow = {0};    
    GUI_RECT_T left_esg_date_rect = {0};
    GUI_RECT_T right_esg_date_rect = {0};
    GUI_RECT_T left_esg_service_rect = {0};
    GUI_RECT_T right_esg_service_rect = {0};
    GUI_RECT_T volume_rect = {0};

    point.x = MMK_GET_TP_X(param);
    point.y = MMK_GET_TP_Y(param);
    //SCI_TRACE_LOW:"MMIMTV_HandleTPMsg point.x = %d, point.y = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_986_112_2_18_2_40_23_604,(uint8*)"dd", point.x, point.y);
    
    MMIMTV_GetSoftKeyRect(&left_rect, &middle_rect, &right_rect); 
    MMIMTV_GetChangeServiceArrowRect(&left_arrow, &right_arrow);
    MMIMTV_GetChangeESGDateArrowRect(&left_esg_date_rect, &right_esg_date_rect);
    MMIMTV_GetChangeESGServiceArrowRect(&left_esg_service_rect, &right_esg_service_rect);
    MMIMTV_GetVolumeProgressRect(&volume_rect);
    
    if(OSD_IDLE == MMIMTV_GetFocusOSDScene())   //OSD_IDLE场景特殊，无软键需特殊处理
    {
        //频道号左箭头图标
        if(GUI_PointIsInRect(point, left_arrow))
        {
            if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
            {
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                //当前正在录制，提示无法切换频道
                MMIMTV_SetPreOSDScene(OSD_SHORTCUT_ICON);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_TIP_RECORDING);
            }
            else
            {
                //play pre service
                MMK_PostMsg(MMIMTV_MAIN_WIN_ID, MSG_APP_LEFT, PNULL, NULL);
            }
        }
        //频道号右箭头图标
        else if(GUI_PointIsInRect(point, right_arrow))
        {
            if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
            {
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                //当前正在录制，提示无法切换频道
                MMIMTV_SetPreOSDScene(OSD_SHORTCUT_ICON);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_TIP_RECORDING);
            }
            else
            {
                //play next service
                MMK_PostMsg(MMIMTV_MAIN_WIN_ID, MSG_APP_RIGHT, PNULL, NULL);
            }
        }
        //音量调节
        else if(GUI_PointIsInRect(point, volume_rect))
        {
            if(point.y >= volume_rect.top && point.y < (volume_rect.top + MMIMTV_INCREASE_ICON_WIDTH))
            {
                MMIMTV_TurnUpVolume();
            }
            else if(point.y < volume_rect.bottom && point.y >= (volume_rect.bottom - MMIMTV_INCREASE_ICON_WIDTH))
            {
                MMIMTV_TurnDownVolume();
            }
        }
        else
        {
            //否则打开OSD_SHORTCUT_ICON
            MMIMTV_HandleMsg(MMIMTV_APP_WEB, param);
        }
    }
    else if(GUI_PointIsInRect(point, left_rect))    //左软键
    {
        MMIMTV_HandleMsg(MMIMTV_APP_OK, param);
    }
    else if(GUI_PointIsInRect(point, middle_rect))  //中间软键
    {
        MMIMTV_HandleMsg(MMIMTV_APP_WEB, param);
    }
    else if(GUI_PointIsInRect(point, right_rect))   //右软键
    {
        MMIMTV_HandleMsg(MMIMTV_APP_CANCEL, param);
    }
    else if(GUI_PointIsInRect(point, left_arrow))   //频道号左箭头图标
    {
        if(OSD_SHORTCUT_ICON == MMIMTV_GetFocusOSDScene())
        {
            //打开频道切换栏
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            
            MMIMTV_DisplayServiceNumberAlways();
            MMK_PostMsg(MMIMTV_MAIN_WIN_ID, MSG_APP_LEFT, PNULL, NULL);
        }
        else
        {
            //play pre service
            MMK_PostMsg(MMIMTV_MAIN_WIN_ID, MSG_APP_LEFT, PNULL, NULL);
        }    
    }
    else if(GUI_PointIsInRect(point, right_arrow))  //频道号右箭头图标
    {
        if(OSD_SHORTCUT_ICON == MMIMTV_GetFocusOSDScene())
        {
            //打开频道切换栏
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            
            MMIMTV_DisplayServiceNumberAlways();
            MMK_PostMsg(MMIMTV_MAIN_WIN_ID, MSG_APP_RIGHT, PNULL, NULL);
        }
        else
        {
            //play next service
            MMK_PostMsg(MMIMTV_MAIN_WIN_ID, MSG_APP_RIGHT, PNULL, NULL);
        }
    }
    else if(GUI_PointIsInRect(point, left_esg_date_rect))
    {
        MMIMTV_HandleMsg(MMIMTV_APP_LEFT, PNULL);
    }
    else if(GUI_PointIsInRect(point, right_esg_date_rect))
    {
        MMIMTV_HandleMsg(MMIMTV_APP_RIGHT, PNULL);
    }
    else if(GUI_PointIsInRect(point, left_esg_service_rect))
    {
        MMIMTV_HandleMsg(MMIMTV_APP_STAR, PNULL);
    }
    else if(GUI_PointIsInRect(point, right_esg_service_rect))
    {
        MMIMTV_HandleMsg(MMIMTV_APP_HASH, PNULL);
    }
    else
    {
        //分发给前场景处理
        MMIMTV_HandleMsg(MMIMTV_APP_TP_DOWN, param);
    }
}

/*****************************************************************************/
// 	Description : handle lose/get focus msg
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_HandleFocusMsg(
                                  MMI_MESSAGE_ID_E msg_id   //the identify of message
                                  )
{
    MMISRV_HANDLE_T audio_handle = MMIMTV_GetAudioHandle();

    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_HandleFocusMsg, msg_id = 0x%x, s_focus_osd = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1129_112_2_18_2_40_23_605,(uint8*)"dd", msg_id, s_focus_osd);
    
    if(MSG_LOSE_FOCUS == msg_id)
    {
        //TV静音, 关闭当前的OSD场景
        if(MMIMTV_IsVRunning())
        {
            CMMB_Mute();
            //MMIAUDIO_SetMute(TRUE);
            if(audio_handle > 0)
            {
                MMISRVAUD_SetVolume(audio_handle, 0);
            }
        }
        
        //to do 快速停止播放
        
        if(!IsFocusOsdWithEditbox())
        {
            //直接关闭当前场景
            MMIMTV_CloseFocusScene();
            //关闭场景后,最后一帧不刷
            MMITHEME_SetUpdateDelayCount(1);
        }
    }
    else if(MSG_GET_FOCUS == msg_id)
    {
        //恢复声音，恢复场景
        if(MMIMTV_IsVRunning() &&  FALSE == MMIMTV_IsMute())
        {
            CMMB_UnMute();
            //MMIAUDIO_SetMute(FALSE);
            if(audio_handle > 0)
            {
                MMISRVAUD_SetVolume(audio_handle, MMIMTV_GetVolume());
            }
        }
        MMIMTV_ClearScreen();
        if(OSD_EB_DETAIL == MMIMTV_GetFocusOSDScene())
        {
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        else
        {
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)s_osd_restore_param);
        }
    }
    else
    {
        return;
    }
    
    return;
}

/*****************************************************************************/
// 	Description : handle MTV REF/PS msg
//	Global resource dependence :
//  Author: xin.li
//	Note: 处理与REF消息交互
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIMTV_HandlePSMsg(
                                       PWND     app_ptr,    //pointer of application
                                       uint16   msg_id,     //the identify of message
                                       DPARAM   param       //the param of the message
                                       )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32       stimi_ret = 0;
    uint32       set_pos_result = 0;
    uint32       i = 0;
    //ESG_ERROR_E  esg_parse_return = ESG_SUCCESS;
    MTV_STIMI_SIG_DATA_T     *sig_data = PNULL;
    ESG_PROG_HINT_INFO_T     *prog_hintinfo = PNULL;
    //MTVSTIMI_BROADCAST_MSG_T *boadcast_msg = PNULL;
    uint32       eb_grade = 0;
    uint8        signal_level = 0;

    MBBMS_SERVICE_INFO_T *sig_info_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_HandlePSMsg: msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1207_112_2_18_2_40_23_606,(uint8*)"d",msg_id);
    
    //mbbms2.0
    if(!MMIAPIMTV_IsMTVOpen())
    {
        //模块已关闭，忽略消息
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_HandlePSMsg app is not open!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1213_112_2_18_2_40_23_607,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    switch(msg_id)
    {
        //TV信号上报
    case MTV_RSSI_CHANGED:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_RSSI_CHANGED msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1221_112_2_18_2_40_23_608,(uint8*)"");
        sig_data     = (MTV_STIMI_SIG_DATA_T *)param;
        
        //calculate signal level 
        signal_level = MMIMTV_CalcTVSignalLevel(sig_data->signal_value);
        //save level value
        MMIMTV_SaveTVSignalLevel(signal_level);

	    //update win status bar
	    MAIN_SetTVSignalState();
        break;
        
    case MTV_STIMI_SEARCH_PROG_END_CNF:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_SEARCH_PROG_END_CNF msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1234_112_2_18_2_40_23_609,(uint8*)"");
        {
            MTV_STIMI_NET_INFO_LIST_T   network_info_list = {0};
            MMIMTV_SAVE_NETEID_INFO_T*  net_node_ptr      = PNULL;
            BOOLEAN play_network_ret = TRUE;
            
            //update net info
            if(MMIMTV_AUTO_SEARCH == s_search_mode.search_type)
            {
                //clearup all net nodes
                MMIMTV_ClearNetList();
            }
            else if(MMIMTV_MANUAL_SEARCH == s_search_mode.search_type || MMIMTV_CITY_SEARCH == s_search_mode.search_type)
            {
                //delete corresponding net node via frequency
                net_node_ptr = MMIMTV_GetNetNodeViaFrequency(s_search_mode.center_freq);
                if(PNULL != net_node_ptr)
                {
                    MMIMTV_DeleteNetNode(net_node_ptr);
                }
            }
            
            //get network info
            CMMB_GetNetworkInfoList(&network_info_list);
            //SCI_TRACE_LOW:"[MMIMTV]: MTVSTIMI_GetNetworkInfoTab network_number = %d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1258_112_2_18_2_40_23_610,(uint8*)"d", network_info_list.network_number);
            if(network_info_list.network_number > 0)
            {
                //get network control table info and save to NV
                SCI_MEMSET(s_mtv_nv_ctl_table_ptr, 0, (sizeof(MMIMTV_NV_CTL_T)*MMIMTV_NV_CTRL_TABLE_MAX_NUM));
                for(i=0; i<network_info_list.network_number && i<MMIMTV_NV_CTRL_TABLE_MAX_NUM; i++)
                {
                    s_mtv_nv_ctl_table_ptr[i].net_id      = network_info_list.network_info_ptr[i].network_info.network_id;
                    s_mtv_nv_ctl_table_ptr[i].center_freq = network_info_list.network_info_ptr[i].network_info.center_frequency;
                    s_mtv_nv_ctl_table_ptr[i].data_len    = CMMB_GetControlTabData(s_mtv_nv_ctl_table_ptr[i].net_id,
                                                                MMIMTV_NV_CTRL_TABLE_MAX_LENGTH,
                                                                s_mtv_nv_ctl_table_ptr[i].ctl_data);
                }
                WriteNetCtrlTable();
                
                //parse net/service info
                ParseNetworkInfoListData(network_info_list);
                
            }
            CMMB_FreeNetworkInfoList(&network_info_list);
            //SCI_MEMSET(&s_search_mode, 0, sizeof(s_search_mode));
            
            //play
            play_network_ret = PlayDefaultServiceAfterSearch();
            
            if(play_network_ret)/*lint !e774 */
            {
                //save net and list data
                if(MMIMTV_WriteNSFile())
                {
                    //save success, open net list
                    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                    MMIMTV_SetFocusOSDScene(OSD_NET_LIST);
                    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
                }
                else
                {
                    //save failed
                    MMIMTV_SetPreOSDScene(OSD_NET_LIST);            //设置OSD_PUB返回的场景
                    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                    MMIMTV_SetFocusOSDScene(OSD_PUB);
                    //保存OSD_PUB场景参数，保证外部中断返回正常显示OSD_PUB场景
                    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_NO_SPACE_TO_SAVE);
                    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_NO_SPACE_TO_SAVE);
                }    
            }
        }
        break;
        
    case MTV_STIMI_SEARCH_PROG_ERR_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_SEARCH_PROG_ERR_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1308_112_2_18_2_40_24_611,(uint8*)"");
        
        MMIMTV_SetPreOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_PUB);
        //保存OSD_PUB场景参数，保证外部中断返回正常显示OSD_PUB场景
        MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_SEARCH_NETWORK_ERROR);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_SEARCH_NETWORK_ERROR);
        
        //resume play network
        ResumePlayServiceAfterSearch();
        break;
        
    case MTV_STIMI_SEARCH_ESG_END_CNF:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_SEARCH_ESG_END_CNF msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1322_112_2_18_2_40_24_612,(uint8*)"");
        s_mtv_current_para.esg_parse_status = MMIMTV_ESG_PARSE_PARSED;
		
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);

		//resume play network
		ResumePlayServiceAfterSearch();
        break;
        
    case MTV_STIMI_SEARCH_ESG_ERR_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_SEARCH_ESG_ERR_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1334_112_2_18_2_40_24_613,(uint8*)"");
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetPreOSDScene(OSD_ESG_LIST);
        MMIMTV_SetFocusOSDScene(OSD_PUB);
        MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_SEARCH_ESG_FAIL);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_SEARCH_ESG_FAIL);
        
        //resume play network
        ResumePlayServiceAfterSearch();
        break;
        
    case MTV_ESG_PARSE_RESULT_IND:
        sig_data = (MTV_STIMI_SIG_DATA_T *)param;
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_ESG_PARSE_RESULT_IND  CMMB_ParseESG retrun: %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1347_112_2_18_2_40_24_614,(uint8*)"d", sig_data->data_type);
        s_mtv_current_para.esg_parse_status = MMIMTV_ESG_PARSE_PARSED;
        break;

    #if 0
    case MSG_MTV_ESG_SUC_CNF:   //ESG解析成功
        //SCI_TRACE_LOW:"[MMIMTV]: received MSG_MTV_ESG_SUC_CNF msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1353_112_2_18_2_40_24_615,(uint8*)"");
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;

    case MSG_MTV_ESG_ERR_IND:   //ESG解析失败
        //SCI_TRACE_LOW:"[MMIMTV]: received MSG_MTV_ESG_ERR_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1360_112_2_18_2_40_24_616,(uint8*)"");
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetPreOSDScene(OSD_ESG_LIST);
        MMIMTV_SetFocusOSDScene(OSD_PUB);
        MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_SEARCH_ESG_FAIL);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_SEARCH_ESG_FAIL);
        break;
    #endif

        
    case MTV_STIMI_EMERGENCY_BROADCAST_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_EMERGENCY_BROADCAST_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1371_112_2_18_2_40_24_617,(uint8*)"");
        sig_data = (MTV_STIMI_SIG_DATA_T *)param;
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_EMERGENCY_BROADCAST_IND msg sig_data->data_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1373_112_2_18_2_40_24_618,(uint8*)"d", sig_data->data_type);
        {
            MTV_STIMI_BROADCAST_U    bc_data = {0};
            
            if (CMMB_GetEmergBroadcast(sig_data->data_type, sig_data->data_len, &bc_data))
            {
                sig_data->data_ptr = (uint8*)&(bc_data.msg_content);
                //已达到上限
                if(MMIMTV_EB_MAX_NUMBER == MMIMTV_GetEBListNumber())
                {
                    MMIMTV_EBSortByTime();
                    if(ParseEmergencyBroadcastData(sig_data,&eb_grade))
                    {
                        //记录来源场景用于返回
                        if(OSD_PUB != MMIMTV_GetFocusOSDScene())
                        {
                            s_source_osd = MMIMTV_GetFocusOSDScene();
                        }
                        else
                        {
                            s_source_osd = OSD_SHORTCUT_ICON;
                        }
                        
                        //删除最旧的一条紧急广播信息
                        MMIMTV_DeleteEBNodeViaIndex(MMIMTV_EB_MAX_NUMBER - 1);
                        //设置PUB_WIN返回的场景
                        if(eb_grade < 3)
                        {
                            MMIMTV_SetPreOSDScene(OSD_EB_DETAIL); //提示空间不足后,返回 OSD_EB_DETAIL 场景
                        }
                        else
                        {
                            MAIN_SetMsgState(TRUE);
                        }
                        
                        //提示已达到最大条数，如当前场景包含编辑框则不提示
                        if(!IsFocusOsdWithEditbox())
                        {
                            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                            MMIMTV_SetFocusOSDScene(OSD_PUB);
                            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_EB_FULL);
                            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_EB_FULL);
                        }
                        
                        //保存紧急广播信息
                        MMIMTV_WriteEBFile();
                    }
                }
                else
                {
                    if(ParseEmergencyBroadcastData(sig_data,&eb_grade))
                    {
                        //记录来源场景用于返回
                        if(OSD_PUB != MMIMTV_GetFocusOSDScene())
                        {
                            s_source_osd = MMIMTV_GetFocusOSDScene();
                        }
                        else
                        {
                            s_source_osd = OSD_SHORTCUT_ICON;
                        }
                        
                        if(eb_grade < 3)
                        {
                            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                            //MMIMTV_SetLastOSDScene();
                            
                            MMIMTV_SetFocusOSDScene(OSD_EB_DETAIL);
                            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
                        }
                        else
                        {
                            MAIN_SetMsgState(TRUE);
                        }
                        //保存紧急广播信息
                        MMIMTV_WriteEBFile();
                    }
                }
            }
            CMMB_FreeBroadcastMsg(&bc_data);
        }
        break;
        
    case MTV_STIMI_PROGRAM_HINT_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_PROGRAM_HINT_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1457_112_2_18_2_40_24_619,(uint8*)"");
        sig_data      = (MTV_STIMI_SIG_DATA_T*)param;
        prog_hintinfo = (ESG_PROG_HINT_INFO_T*)sig_data->data_ptr;
        ParseProgramHintData(prog_hintinfo);
        SCI_FREE(prog_hintinfo);
        
        if(MMIMTV_IsDisplayProgramName())
        {
            //display program hint info
            MMIMTV_DisplayProgramHint(TRUE);
        }
        break;
        
    case MTV_STIMI_CTLTAB_UPDATE_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_CTLTAB_UPDATE_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1471_112_2_18_2_40_24_620,(uint8*)"");
        break;
        
    case MTV_STIMI_RCV_CTLTAB_ERR_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STIMI_RCV_CTLTAB_ERR_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1475_112_2_18_2_40_24_621,(uint8*)"");
        break;
        
    case MTV_PLAY_ACK:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_PLAY_ACK msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1479_112_2_18_2_40_24_622,(uint8*)"");
        if(MMIMTV_PLAY_STOPPING == s_mtv_current_para.play_status)
        {
            //可能出现调用stop后先收到play_ack的情况
            break;
        }
        s_mtv_current_para.play_status = MMIMTV_PLAY_PLAYING;
        //直播模式
        if(MMIMTV_PLAY_TYPE_NETWORK == s_mtv_current_para.play_type)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: received MTV_PLAY_ACK msg  MMIMTV_PLAY_TYPE_NETWORK"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1489_112_2_18_2_40_24_623,(uint8*)"");
            //save program info to NV
            SaveLastServiceId();
        }
        //本地回放
        else if(MMIMTV_PLAY_TYPE_RECORD == s_mtv_current_para.play_type)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: received MTV_PLAY_ACK msg  MMIMTV_PLAY_TYPE_RECORD"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1496_112_2_18_2_40_24_624,(uint8*)"");
            
            //打开本地回放场景 OSD_RECORD_PLAY
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_RECORD_PLAY);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        
        if(OSD_PLAY_CONNECTING == MMIMTV_GetFocusOSDScene())
        {
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        break;
        
    case MTV_PLAY_ERROR:  //播放错误
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_PLAY_ERROR msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1513_112_2_18_2_40_24_625,(uint8*)"");
        s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPED;
        //含有编辑框的场景特殊，这些场景下忽略此消息，避免显示冲突
        if(!IsFocusOsdWithEditbox())
        {
            MMIMTV_SetLastOSDScene();
			//保存OSD_PUB场景参数，保证中断返回正常显示OSD_PUB场景
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);

            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
            
            if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
            {
                MMIMTV_SetPreOSDScene(OSD_IDLE);
            }
        }
        break;
        
    case MTV_PLAY_ERROR_IND:  //播放过程中超时错误，原因无信号或无节目
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_PLAY_ERROR_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1534_112_2_18_2_40_24_626,(uint8*)"");
        s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
        //含有编辑框的场景特殊，这些场景下忽略此消息，避免显示冲突
        if(!IsFocusOsdWithEditbox())
        {
            MMIMTV_SetLastOSDScene();
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            //保存OSD_PUB场景参数，保证中断返回正常显示OSD_PUB场景
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_SIGNAL_ERROR);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_SIGNAL_ERROR);
            
            if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
            {
                MMIMTV_SetPreOSDScene(OSD_IDLE);
            }
        }
        break;

    case MTV_BAD_SIGNAL:  //TV信号弱
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_BAD_SIGNAL msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1554_112_2_18_2_40_24_627,(uint8*)"");
        s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
        //含有编辑框的场景特殊，这些场景下忽略此消息，避免显示冲突
        if(!IsFocusOsdWithEditbox())
        {
            MMIMTV_SetLastOSDScene();
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            //保存OSD_PUB场景参数，保证中断返回正常显示OSD_PUB场景
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_SIGNAL_WEAK);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_SIGNAL_WEAK);
            
            if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
            {
                MMIMTV_SetPreOSDScene(OSD_IDLE);
            }
        }
        break;

    case MTV_SIGNAL_RECOVERED:  //TV信号已恢复
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_SIGNAL_RECOVERED msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1574_112_2_18_2_40_24_628,(uint8*)"");
        if(MMIMTV_PLAY_TYPE_NETWORK == s_mtv_current_para.play_type)
        {
            s_mtv_current_para.play_status = MMIMTV_PLAY_PLAYING;
        }

        //TV信号已恢复
        if(!IsFocusOsdWithEditbox())
        {
            MMIMTV_SetLastOSDScene();
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            //保存OSD_PUB场景参数，保证中断返回正常显示OSD_PUB场景
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_SIGNAL_RECOVER);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_SIGNAL_RECOVER);
            
            if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
            {
                MMIMTV_SetPreOSDScene(OSD_IDLE);
            }
        }
        break;

    case MTV_RELOCKING:
        //重新发起播放
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_RELOCKING msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1599_112_2_18_2_40_24_629,(uint8*)"");
        if(MMIMTV_PLAY_WAITING == s_mtv_current_para.play_status
            && MMK_IsFocusWin(MMIMTV_MAIN_WIN_ID))
        {
            SwitchServicePlay();
        }
        break;
        
    case MTV_PLAY_AV_IND:
        {
            uint32 av_ind = 0;
            uint32 video_ind = 0;
            uint32 audio_ind = 0;
            
            sig_data = (MTV_STIMI_SIG_DATA_T*)param;
            av_ind = sig_data->data_type;
            video_ind = av_ind >> 16;
            audio_ind = av_ind & 0x0ffff;
            
            //SCI_TRACE_LOW:" [MMIMTV]: received MTV_PLAY_AV_IND msg video ind=%d, audio ind=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1618_112_2_18_2_40_24_630,(uint8*)"dd", video_ind, audio_ind);
        }
        break;
        
    case MTV_PLAY_DRA_IND:  //播放的为音频节目
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_PLAY_DRA_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1623_112_2_18_2_40_24_631,(uint8*)"");
        s_mtv_current_para.is_playing_broadcast = TRUE;
        //音频广播允许关闭背光
        MMIDEFAULT_AllowTurnOffBackLight(TRUE);
        //音频节目降频节电
#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMI_GetFreqHandler());
#endif
        //设置广播背景图片
        MMIMTV_SetBMPtoVChip(IMAGE_MTV_BG1 + MMIMTV_GetBroadcastBG());
        break;
        
    case MTV_CA_NOT_GRANTED_IND:
        {
            //uint8 error_type = 0;
            //SCI_TRACE_LOW:"[MMIMTV]: received MTV_CA_NOT_GRANTED_IND msg"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1638_112_2_18_2_40_24_632,(uint8*)"");
            s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
            
            //含有编辑框的场景特殊，这些场景下忽略此消息，避免显示冲突
            if(!IsFocusOsdWithEditbox())
            {
                MMIMTV_SetLastOSDScene();
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                //保存OSD_PUB场景参数，保证中断返回正常显示OSD_PUB场景
                MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_CA_UNAUTHORIZED);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_CA_UNAUTHORIZED);
                
                if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
                {
                    MMIMTV_SetPreOSDScene(OSD_IDLE);
                }
            }
        }
        break;
        
    case MTV_CA_CARD_NOT_PRESENT_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_CA_CARD_NOT_PRESENT_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1660_112_2_18_2_40_24_633,(uint8*)"");
        s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
        //含有编辑框的场景特殊，这些场景下忽略此消息，避免显示冲突
        if(!IsFocusOsdWithEditbox())
        {
            MMIMTV_SetLastOSDScene();
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            //保存OSD_PUB场景参数，保证中断返回正常显示OSD_PUB场景
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_INSERT_CA_CARD);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_INSERT_CA_CARD);
            
            if(OSD_PLAY_CONNECTING == MMIMTV_GetPreOSDScene())
            {
                MMIMTV_SetPreOSDScene(OSD_IDLE);
            }
        }
        break;
        
    case MTV_STOP_ACK:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1680_112_2_18_2_40_24_634,(uint8*)"");
        s_mtv_current_para.play_status  = MMIMTV_PLAY_STOPPED;
        s_mtv_record_time_count         = 0;
        
        if(s_mtv_current_para.is_playing_broadcast)
        {
            s_mtv_current_para.is_playing_broadcast = FALSE;
            //mute
            if(!MMIMTV_IsMute())
            {
                CMMB_UnMute();
            }
            //非音频节目禁止自动关背光
            MMIDEFAULT_TurnOnBackLight(); 
            MMIDEFAULT_AllowTurnOffBackLight(FALSE);
            
#ifndef WIN32
            //升频
            CHNG_FREQ_SetArmClk(MMI_GetFreqHandler(),(CHNG_FREQ_INDEX_E)FREQ_INDEX_APP_HIGH);
#endif
        }
        
        switch(s_mtv_current_para.op_type)
        {
        case MMIMTV_OP_EXIT:
        	break;

        case MMIMTV_OP_PLAY_NEXT:
            //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_PLAY_NEXT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1708_112_2_18_2_40_24_635,(uint8*)"");
            //for CR157456 回放录像切换到直播也需要RESET Vchip
            if(s_mtv_current_para.is_need_reset_Vchip)
            {
                s_mtv_current_para.is_need_reset_Vchip = FALSE;
                ResetCMMBModule();
            }
            
            if(CMMB_PlayNetwork(s_mtv_current_para.net_eid, s_mtv_current_para.service_id, s_mtv_current_para.audio_language))
            {
                //SCI_TRACE_LOW:"[MMIMTV]: MTV_STOP_ACK PlayNetwork error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1718_112_2_18_2_40_24_636,(uint8*)"");
                //提示播放失败，尝试重新搜网
                OpenPlayNetworkErrScene();
            }
            else
            {
                s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
            }   
            break;
            
        case MMIMTV_OP_PLAY_RECORD:
            //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_PLAY_RECORD"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1729_112_2_18_2_40_24_637,(uint8*)"");
            //网络直播切换到录像播放时需要reset Vchip
            ResetCMMBModule();
            if(CMMB_PlayStream(s_mtv_current_para.record_file_full_name,
                s_mtv_current_para.audio_language))
            {
                //SCI_TRACE_LOW:"[MMIMTV]: PlayStream error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1735_112_2_18_2_40_24_638,(uint8*)"");
                //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
                MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
                
                //播放录像文件错误，提示
                OpenPlayStreamErrScene();

                //恢复直播
                SwitchServicePlay();
            }
            else
            {
                s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
            }
            break;
            
        case MMIMTV_OP_SLIDE:
            //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_SLIDE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1752_112_2_18_2_40_24_639,(uint8*)"");
            //get time offset
            s_mtv_record_time_count = (s_mtv_current_para.tp_offset * s_mtv_record_file_total_time) / MMIMTV_RECORD_PLAY_PROGRESS_WIDTH;
            if(s_mtv_record_time_count != s_mtv_record_file_total_time)
            {
                //set stream pos
                set_pos_result = CMMB_SetStreamPos(s_mtv_record_time_count);
                //SCI_TRACE_LOW:"[MMIMTV]: SetStreamPos set_pos_result = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1759_112_2_18_2_40_24_640,(uint8*)"d", set_pos_result);
                //play
                if(CMMB_PlayStream(s_mtv_current_para.record_file_full_name,
                    s_mtv_current_para.audio_language))
                {
                    //SCI_TRACE_LOW:"[MMIMTV]: PlayStream error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1764_112_2_18_2_40_24_641,(uint8*)"");
                    //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
                    MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
                    
                    //播放录像文件错误，提示
                    OpenPlayStreamErrScene();
                    
                    //恢复直播
                    SwitchServicePlay();
                }
                else
                {
                    s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
                }
            }
            break;
            
        case MMIMTV_OP_SEARCH:
            //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK MMIMTV_OP_SEARCH search type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1782_112_2_18_2_40_25_642,(uint8*)"d", s_search_mode.search_type);
            
            //网络直播切换搜网reset Vchip
            ResetCMMBModule();

            if(MMIMTV_AUTO_SEARCH == s_search_mode.search_type)
            {
                //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK CMMB_SearchProgram"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1789_112_2_18_2_40_25_643,(uint8*)"");
                //launch auto search
                stimi_ret = CMMB_SearchProgram();
                if(MTV_STIMI_ERR_NONE != stimi_ret)
                {
                    //SCI_TRACE_LOW:"[MMIMTV]: CMMB_SearchProgram error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1794_112_2_18_2_40_25_644,(uint8*)"");
                    //提示搜网失败
                    OpenSearchProgramErrScene();
                    //恢复节目播放
                    ResumePlayServiceAfterSearch();
                }
            }
            else if(MMIMTV_MANUAL_SEARCH == s_search_mode.search_type)
            {
                //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK CMMB_SearchCenterFrq"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1803_112_2_18_2_40_25_645,(uint8*)"");
                //launch frequency search
                stimi_ret = CMMB_SearchCenterFrq(s_search_mode.center_freq);
                if(MTV_STIMI_ERR_NONE != stimi_ret)
                {
                    if(MTV_STIMI_INVALID_CENTER_FRQ == stimi_ret)
                    {
                        //invalid frequency input
                        MMIMTV_SetPreOSDScene(OSD_MANUAL_SEARCH);
                        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                        MMIMTV_SetFocusOSDScene(OSD_SEARCH_INVALID_FRQ);
                        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"[MMIMTV]: CMMB_SearchCenterFrq error"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1818_112_2_18_2_40_25_646,(uint8*)"");
                        //提示搜网失败
                        OpenSearchProgramErrScene();
                    }
                    //恢复节目播放
                    ResumePlayServiceAfterSearch();
                }
            }
            else if( MMIMTV_ESG_SEARCH == s_search_mode.search_type)
            {
                //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK CMMB_SearchESGData"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1828_112_2_18_2_40_25_647,(uint8*)"");
                
                //ESG解析中，先停止
                if(MMIMTV_ESG_PARSE_PARSING == s_mtv_current_para.esg_parse_status)
                {
                    CMMB_AbortESG();
                    s_mtv_current_para.esg_parse_status = MMIMTV_ESG_PARSE_NONE;
                }
                
                //launch esg search
                MMIMTV_SetESGPath();
                stimi_ret = CMMB_SearchESGData(s_esglist_param.net_id);
                //SCI_TRACE_LOW:"[MMIMTV]: StartESGSearch stimi_ret = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1840_112_2_18_2_40_25_648,(uint8*)"d", stimi_ret);
                if(MTV_STIMI_ERR_NONE != stimi_ret)
                {
                    //搜索ESG失败
                    MMIMTV_SetPreOSDScene(OSD_ESG_LIST);
                    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                    MMIMTV_SetFocusOSDScene(OSD_PUB);
                    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_SEARCH_ESG_FAIL);
                    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_SEARCH_ESG_FAIL);
                    //恢复节目播放
                    ResumePlayServiceAfterSearch();
                }
            }
            break;
            
        default:
            if(MMIMTV_PLAY_TYPE_RECORD == s_mtv_current_para.play_type)
            {
                //录像文件播放完毕
                //SCI_TRACE_LOW:"[MMIMTV]: received MTV_STOP_ACK  MTV_PLAY_STREAM_END !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1859_112_2_18_2_40_25_649,(uint8*)"");
                //更新回放场景显示
                MMIMTV_ClearScreen();
                MMIMTV_OpenRecordPlayScene(NULL);
            }
            break;
        }
        s_mtv_current_para.op_type = MMIMTV_OP_NONE;
        break;
        
    case MTV_STOP_ERROR:
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_HandlePSMsg MTV_STOP_ERROR !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1870_112_2_18_2_40_25_650,(uint8*)"");
        s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPED;
        
        if(MMIMTV_OP_EXIT != s_mtv_current_para.op_type)
        {
            //serious error, must quit
            MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
        }
        
        break;
        
    case MTV_CA_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_CA_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1883_112_2_18_2_40_25_651,(uint8*)"");
        break;
        
    case MTV_PLAY_STREAM_END:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_PLAY_STREAM_END msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1887_112_2_18_2_40_25_652,(uint8*)"");
        s_mtv_current_para.is_stream_end = TRUE;
        //停止播放
        if(SCI_SUCCESS != CMMB_Stop())
        {
            //SCI_TRACE_LOW:"[MMIMTV]: Stop error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1892_112_2_18_2_40_25_653,(uint8*)"");
            s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
            //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            //MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
        }
        else
        {
            s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPING;
        }
        break;
        
    case MTV_PLAY_TIME_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_PLAY_TIME_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1904_112_2_18_2_40_25_654,(uint8*)"");
        //回放录像计时器
        s_mtv_record_time_count++;

        if(MMIMTV_PLAY_TYPE_NETWORK == s_mtv_current_para.play_type)
        {
            //nothing to do
        }
        else if(MMIMTV_PLAY_TYPE_RECORD == s_mtv_current_para.play_type)
        {
            //回放录像状态
            MMIMTV_DisplayRecordPlayTime(s_mtv_record_time_count);
            MMIMTV_DisplayRecordPlayProgress(s_mtv_record_time_count, s_mtv_record_file_total_time);
        }
        else
        {
            //SCI_TRACE_LOW:"[MMIMTV]: s_mtv_current_para.play_type %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1920_112_2_18_2_40_25_655,(uint8*)"d",s_mtv_current_para.play_type);
        }
        break;
        
    case MTV_RECORD_FILE_SAVED:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_RECORD_FILE_SAVED msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1925_112_2_18_2_40_25_656,(uint8*)"");
        //提示录制文件已保存
        s_mtv_current_para.has_been_recorded = TRUE;
        if(OSD_SHORTCUT_ICON == MMIMTV_GetFocusOSDScene())
        {
            //关闭OSD_SHORTCUT_ICON场景，避免显示冲突
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            MMIMTV_DisplayRecordSaveTip();
        }
        break;
        
    case MTV_RECORD_FILE_TOOSMALL:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_RECORD_FILE_TOOSMALL msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1939_112_2_18_2_40_25_657,(uint8*)"");
        //录制文件太小无法保存，一般由录制中信号弱或快速停止录制引起
        s_mtv_current_para.has_been_recorded   = FALSE;

        //提示录制文件无法保存
        MMIMTV_SetLastOSDScene();
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_PUB);
        MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_RECORD_FILE_ERROR);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_RECORD_FILE_ERROR);
        break;
        
    case MTV_DISK_WRITE_ERROR:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_DISK_WRITE_ERROR msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1952_112_2_18_2_40_25_658,(uint8*)"");
        if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
        {
            //stop record 无空间须立即停止录制
            MMIMTV_StopRecord();
            
            //含有编辑框的场景特殊，这些场景下忽略此消息，避免显示冲突
            if(!IsFocusOsdWithEditbox())
            {
                //提示磁盘空间已满
                MMIMTV_SetLastOSDScene();
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                //保存OSD_PUB场景参数，保证中断返回正常显示OSD_PUB场景
                MMIMTV_SetOSDRestoreParam((uint32)TXT_NO_SPACE);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_NO_SPACE);
            }
        }
        break;
        
    case MTV_DTL_ERROR:
        //SCI_TRACE_LOW:"[MMIMTV]: received MTV_DTL_ERROR msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1973_112_2_18_2_40_25_659,(uint8*)"");
        //V发生assert，MMI需退出应用!
        MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
        break;
            
#ifdef MBBMS_SUPPORT

    //mbbms2.0 start 
    case MBBMS_SERVICE_PDP_ACTIVE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_PDP_ACTIVE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1982_112_2_18_2_40_25_660,(uint8*)"");
        {
            MTVSERVICE_PDP_ACTIVE_T *active_info_ptr = PNULL;
            
            sig_info_ptr = (MBBMS_SERVICE_INFO_T*)param;
            active_info_ptr = (MTVSERVICE_PDP_ACTIVE_T*)sig_info_ptr->data_ptr;
            
            //通过MMI激活PDP
            MMIMBBMS_ActivePDPContext(active_info_ptr);
            
            MTVSERVICE_FreePDPActivePtr(active_info_ptr);
        }
        break;
        
    case MBBMS_SERVICE_PDP_DEACTIVE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_PDP_DEACTIVE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_1997_112_2_18_2_40_25_661,(uint8*)"");
        MMIMBBMS_DeactivePDPContext();
        break;
        
    case MBBMS_SERVICE_PARSE_SG:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_PARSE_SG msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2002_112_2_18_2_40_25_662,(uint8*)"");
        MMIMBBMS_HandleParseSGSignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_PARSE_SG:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_PARSE_SG msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2007_112_2_18_2_40_25_663,(uint8*)"");
        MMIMBBMS_HandleStopParseSGSignal(param);
        break;
        
    case MBBMS_SERVICE_PARSE_ESG:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_PARSE_ESG msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2012_112_2_18_2_40_25_664,(uint8*)"");
        MMIMBBMS_HandleParseESGSignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_PARSE_ESG:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_PARSE_ESG msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2017_112_2_18_2_40_25_665,(uint8*)"");
        MMIMBBMS_HandleStopParseESGSignal(param);
        break;
        
    case MBBMS_SERVICE_ACTIVE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_ACTIVE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2022_112_2_18_2_40_25_666,(uint8*)"");
        MMIMBBMS_HandleActiveMBBMSSignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_ACTIVE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_ACTIVE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2027_112_2_18_2_40_25_667,(uint8*)"");
        MMIMBBMS_HandleStopActiveMBBMSSignal(param);
        break;
        
    case MBBMS_SERVICE_UPDATE_TABLE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_UPDATE_TABLE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2032_112_2_18_2_40_25_668,(uint8*)"");
        MMIMBBMS_HandleUpdateSubTableSignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_UPDATE_TABLE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_UPDATE_TABLE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2037_112_2_18_2_40_25_669,(uint8*)"");
        MMIMBBMS_HandleStopUpdateSubTableSignal(param);
        break;
        
    case MBBMS_SERVICE_GBA:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_GBA msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2042_112_2_18_2_40_25_670,(uint8*)"");
        MMIMBBMS_HandleGBAOperationSignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_GBA:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_GBA msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2047_112_2_18_2_40_25_671,(uint8*)"");
        MMIMBBMS_HandleStopGBAOperationSignal(param);
        break;
        
    case MBBMS_SERVICE_PLAY:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_PLAY msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2052_112_2_18_2_40_25_672,(uint8*)"");
        MMIMBBMS_HandleStartPlaySignal(param);
        break;
        
    case MBBMS_SERVICE_STOP:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2057_112_2_18_2_40_25_673,(uint8*)"");
        MMIMBBMS_HandleStopPlaySignal(param);
        break;
        
        //Yelianna确认本地搜索流程暂时不变，消息不通过MTVSERVICE
    case MBBMS_SGAPI_GET_DATA_IND:
        //SCI_TRACE_LOW:"[MMIMBBMS]:receive sg get complexe data!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2063_112_2_18_2_40_25_674,(uint8*)"");
        MMIMBBMS_HandleSGGetComplex(param);
        break;
    case MBBMS_SGAPI_STOP_RES:
        //SCI_TRACE_LOW:"[MMIMBBMS]:receive sg stop getting complexe data!\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2067_112_2_18_2_40_25_675,(uint8*)"");
        MMIMBBMS_HandleSGStopComplex(param);
        break;
        
    case MBBMS_SERVICE_SUBSCRIBE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_SUBSCRIBE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2072_112_2_18_2_40_25_676,(uint8*)"");
        MMIMBBMS_HandleSubscribeSignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_SUBSCRIBE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_SUBSCRIBE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2077_112_2_18_2_40_25_677,(uint8*)"");
        MMIMBBMS_HandleStopSubscribeSignal(param);
        break;
        
    case MBBMS_SERVICE_UNSUBSCRIBE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_UNSUBSCRIBE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2082_112_2_18_2_40_25_678,(uint8*)"");
        MMIMBBMS_HandleUnSubscribeSignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_UNSUBSCRIBE:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_UNSUBSCRIBE msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2087_112_2_18_2_40_25_679,(uint8*)"");
        MMIMBBMS_HandleStopUnSubscribeSignal(param);
        break;
        
    case MBBMS_SERVICE_ACCOUNT_QUIRY:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_ACCOUNT_QUIRY msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2092_112_2_18_2_40_25_680,(uint8*)"");
        MMIMBBMS_HandleAccountInquirySignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_ACCOUNT_QUIRY:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_ACCOUNT_QUIRY msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2097_112_2_18_2_40_25_681,(uint8*)"");
        MMIMBBMS_HandleStopAccountInquirySignal(param);
        break;
        
    case MBBMS_SERVICE_MANUAL_SG:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_MANUAL_SG msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2102_112_2_18_2_40_25_682,(uint8*)"");
        MMIMBBMS_HandleManualUpdateSGSignal(param);
        break;
        
    case MBBMS_SERVICE_STOP_MANUAL_SG:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_STOP_MANUAL_SG msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2107_112_2_18_2_40_25_683,(uint8*)"");
        MMIMBBMS_HandleStopManualUpdateSGSignal(param);
        break;
        
    case MBBMS_SERVICE_CA_NOT_GRANTED_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: received MBBMS_SERVICE_CA_NOT_GRANTED_IND msg"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2112_112_2_18_2_40_25_684,(uint8*)"");
        MMIMBBMS_HandleMSKupdateSignal(param);
        break;
        //mbbms2.0 end  
#endif
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Get Program hint info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC const MMIMTV_PROG_HINT_INFO_T* MMIMTV_GetProgramHintInfo(void)
{
    return s_mtv_prog_hint_info_ptr;
}

/*****************************************************************************/
//  Description : Get Record play time info
//  Global resource dependence : 
//  Author: xin.li
//  Note: 用于回放录制节目时获取时间信息,包括当前播放时间及节目总时长
/*****************************************************************************/
PUBLIC void MMIMTV_GetRecordPlayTimeInfo(uint32 *time_count, uint32 *time_total)
{
    if(PNULL == time_count
        || PNULL == time_total)
    {
        return;
    }
    
    *time_count = s_mtv_record_time_count;
    *time_total = s_mtv_record_file_total_time;
}

/*****************************************************************************/
//  Description : Get frequency user input
//  Global resource dependence : 
//  Author: xin.li
//  Note: 获取用户之前输入的频点
/*****************************************************************************/
PUBLIC uint32 MMIMTV_GetLastInputFreq(void)
{
    return s_search_mode.center_freq;
}

/*****************************************************************************/
// 	Description : turn up volume
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_TurnUpVolume(void)
{
    MMIMTV_VOLUME_E          volume_value = 0;
    AUDIO_DEVICE_MODE_TYPE_E audio_mode = 0;
     MMISRV_HANDLE_T          audio_handle = MMIMTV_GetAudioHandle();
    
    //SCI_TRACE_LOW:"[MMIMTV]: TurnUpVolume"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2169_112_2_18_2_40_25_685,(uint8*)"");
    //if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status)
    //{
    volume_value = MMIMTV_GetPanelVolume();
    if(volume_value < MMIMTV_VOLUME_LEVEL_7)
    {
        volume_value ++;
        MMIMTV_SetPanelVolume(volume_value);
        MMIMTV_SetVolume(volume_value);
        
        CMMB_UnMute();
        //MMIAUDIO_SetMute(FALSE);
        MMIMTV_SetIsMute(FALSE);

        if(audio_handle > 0)
        {
            MMISRVAUD_SetVolume(audio_handle, volume_value);
        }        
        //同时更新MMIAUDIO的volume，防止插拔耳机时音量异常
        //MMIAPISET_GetDeviceMode(&audio_mode);
        //MMIAUDIO_SetVolume(audio_mode, volume_value);
    }
    
    if(OSD_SHORTCUT_ICON == MMIMTV_GetFocusOSDScene())
    {
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
    }
    
    //display volume process panel
    if(OSD_IDLE == MMIMTV_GetFocusOSDScene()
        || OSD_RECORD_PLAY == MMIMTV_GetFocusOSDScene())
    {
        MMIMTV_DisplayVolumeProgress(MMIMTV_GetVolume());
    }
    //}
}

/*****************************************************************************/
// 	Description : turn down volume
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_TurnDownVolume(void)
{
    MMIMTV_VOLUME_E          volume_value = 0;
    AUDIO_DEVICE_MODE_TYPE_E audio_mode = 0;
    MMISRV_HANDLE_T          audio_handle = MMIMTV_GetAudioHandle();
    
    //SCI_TRACE_LOW:"[MMIMTV]: TurnDownVolume"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2218_112_2_18_2_40_25_686,(uint8*)"");
    //if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status)
    //{
    volume_value = MMIMTV_GetVolume();
    if(volume_value > MMIMTV_VOLUME_LEVEL_0)
    {
        volume_value--;
        MMIMTV_SetPanelVolume(volume_value);
        MMIMTV_SetVolume(volume_value);
        
        if(volume_value == MMIMTV_VOLUME_LEVEL_0)
        {
            CMMB_Mute();
            //MMIAUDIO_SetMute(TRUE);
            volume_value = 0;
            MMIMTV_SetIsMute(TRUE);
        }
        else
        {
            CMMB_UnMute();
            //MMIAUDIO_SetMute(FALSE);
            MMIMTV_SetIsMute(FALSE);
        }
        
        if(audio_handle > 0)
        {
            MMISRVAUD_SetVolume(audio_handle, volume_value);
        }
        
        //同时更新MMIAUDIO的volume，防止插拔耳机时音量异常
        //MMIAPISET_GetDeviceMode(&audio_mode);
        //MMIAUDIO_SetVolume(audio_mode, MMIMTV_GetVolume());
    }
    
    if(OSD_SHORTCUT_ICON == MMIMTV_GetFocusOSDScene())
    {
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
    }
    
    //display volume panel
    if(OSD_IDLE == MMIMTV_GetFocusOSDScene()
        || OSD_RECORD_PLAY == MMIMTV_GetFocusOSDScene())
    {
        MMIMTV_DisplayVolumeProgress(MMIMTV_GetVolume());
    }
    //}
}

/*****************************************************************************/
// 	Description : get the last played program info 
//	Global resource dependence : none
//  Author: xin.li
//	Note: 从NV获取上次播放的频道
/*****************************************************************************/
LOCAL BOOLEAN GetLastServiceId(void)
{
    //read the last played program info from NV
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_MTV_PROG_ID, &s_mtv_last_prog_ifo, return_value);
    if(MN_RETURN_SUCCESS == return_value)
    {
        s_mtv_current_para.net_eid          = s_mtv_last_prog_ifo.net_id;
        s_mtv_current_para.service_id       = s_mtv_last_prog_ifo.service_id;
        s_mtv_current_para.audio_language   = s_mtv_last_prog_ifo.audio_language;
        s_mtv_current_para.sub_language     = s_mtv_last_prog_ifo.sub_language;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : save the last played net and service id 
//	Global resource dependence : none
//  Author: xin.li
//	Note: 保存最后一次播放的频道
/*****************************************************************************/
LOCAL void SaveLastServiceId(void)
{
    s_mtv_last_prog_ifo.net_id          = s_mtv_current_para.net_eid;
    s_mtv_last_prog_ifo.service_id      = s_mtv_current_para.service_id;
    s_mtv_last_prog_ifo.audio_language  = s_mtv_current_para.audio_language;
    s_mtv_last_prog_ifo.sub_language    = s_mtv_current_para.sub_language;
    
    //save the last played program info
    MMINV_WRITE(MMINV_MTV_PROG_ID, &s_mtv_last_prog_ifo);
}

/*****************************************************************************/
// 	Description : read network control table data from NV
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadNetCtrlTable(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    SCI_MEMSET(s_mtv_nv_ctl_table_ptr, 0, (sizeof(MMIMTV_NV_CTL_T)*MMIMTV_NV_CTRL_TABLE_MAX_NUM));
    MMINV_READ(MMINV_MTV_CTL_TAB, s_mtv_nv_ctl_table_ptr, return_value);
    if(MN_RETURN_SUCCESS == return_value)
    {
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"[MMIMTV]: ReadNetCtrlTable error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2324_112_2_18_2_40_26_687,(uint8*)"");
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : save network control table data to NV
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void WriteNetCtrlTable(void)
{
    MMINV_WRITE(MMINV_MTV_CTL_TAB, s_mtv_nv_ctl_table_ptr);
}

/*****************************************************************************/
// 	Description : save network control table data
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SetNetCtrlTableData(void)
{
    uint16 i = 0;
    //SCI_TRACE_LOW:"[MMIMTV]: SetNetCtrlTableData"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2345_112_2_18_2_40_26_688,(uint8*)"");
    
    ReadNetCtrlTable();
    
    for(i = 0; i<MMIMTV_NV_CTRL_TABLE_MAX_NUM; i++)
    {
        //对有效的网络都将网络控制数据设下去
        if(0 != s_mtv_nv_ctl_table_ptr[i].net_id && s_mtv_nv_ctl_table_ptr[i].data_len > 0)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: SetNetCtrlTableData net_id=%d data_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2355_112_2_18_2_40_26_689,(uint8*)"dd", s_mtv_nv_ctl_table_ptr[i].net_id, s_mtv_nv_ctl_table_ptr[i].data_len);
            //set control table data
            CMMB_SetControlTabData(s_mtv_nv_ctl_table_ptr[i].net_id,
                s_mtv_nv_ctl_table_ptr[i].data_len,
                s_mtv_nv_ctl_table_ptr[i].ctl_data);
        }
    }
}

/*****************************************************************************/
// 	Description : parse network info after search
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void ParseNetworkInfoListData(
                                    MTV_STIMI_NET_INFO_LIST_T network_info_list
                                    )
{
    uint16  i = 0;
    uint16  j = 0;
    uint16  k = 0;
    wchar   net_name_temp[MMIMTV_MAX_NAME_LEN + 1] = {0};
    uint16  net_name_len = 0;
    uint16  ucs_net_name_len = 0;
    
    wchar   service_name_temp[MMIMTV_MAX_NAME_LEN + 1] = {0};
    uint16  service_name_len = 0;
    uint16  ucs_service_name_len = 0;
    
    wchar   shortservice_name_temp[MMIMTV_MAX_NAME_LEN +1] = {0};
    uint16  shortservice_name_len = 0;
    uint16  ucs_shortservice_name_len = 0;
    
    MMIMTV_SAVE_NETEID_INFO_T   net_node = {0};
    MMIMTV_SAVE_SERVICE_INFO_T  service_node = {0};
    MMIMTV_SAVE_SERVICE_INFO_T  shortservice_node = {0};

    uint8   str_service_name[MMIMTV_MAX_NAME_LEN] = {0};
    
    //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData network_number = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2393_112_2_18_2_40_26_690,(uint8*)"d", network_info_list.network_number);
    for(i=0; i<network_info_list.network_number && i<(MMIMTV_NET_NUMBER - MMIMTV_GetNetListNumber()); i++)
    {
        //alloc net node
        SCI_MEMSET(&net_node, 0, sizeof(MMIMTV_SAVE_NETEID_INFO_T));
        
        //get net node info
        net_node.net_id                = network_info_list.network_info_ptr[i].network_info.network_id;
        net_node.center_freqquency     = network_info_list.network_info_ptr[i].network_info.center_frequency;
        
        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData network_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2403_112_2_18_2_40_26_691,(uint8*)"d",     net_node.net_id);
        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData frequency_no = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2404_112_2_18_2_40_26_692,(uint8*)"d",   network_info_list.network_info_ptr[i].network_info.frequency_no);
        //SCI_TRACE_LOW("[MMIMTV]: ParseNetworkInfoListData bandwidth = %d",      network_info_list.network_info_ptr[i].network_info.bandwidth);
        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData center_frequency = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2406_112_2_18_2_40_26_693,(uint8*)"d", network_info_list.network_info_ptr[i].network_info.center_frequency);
        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData service_number = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2407_112_2_18_2_40_26_694,(uint8*)"d",   network_info_list.network_info_ptr[i].service_list.service_number);
        
        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData code_table = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2409_112_2_18_2_40_26_695,(uint8*)"d",       network_info_list.network_info_ptr[i].code_table);
        //SCI_TRACE_LOW("[MMIMTV]: ParseNetworkInfoListData network_name_len = %d", network_info_list.network_info_ptr[i].network_name_len);
        net_name_len = MMI_MIN(network_info_list.network_info_ptr[i].network_name_len, MMIMTV_MAX_NAME_LEN);
        //SCI_TRACE_LOW("[MMIMTV]: ParseNetworkInfoListData MMI_MIN net_name_len = %d",net_name_len);
        switch(network_info_list.network_info_ptr[i].code_table)
        {
        case MTV_STIMI_GB_13000_1:
            MMI_WSTRNCPY(net_node.net_name,
                MMIMTV_MAX_NAME_LEN,
                (wchar*)network_info_list.network_info_ptr[i].network_name_arr,
                network_info_list.network_info_ptr[i].network_name_len,
                net_name_len);
            break;
            
        case MTV_STIMI_KSC_5601:
            break;
            
        case MTV_STIMI_GB_2312:
            SCI_MEMSET(net_name_temp, 0, sizeof(net_name_temp));
            ucs_net_name_len = GUI_GBToWstr(net_name_temp,
                network_info_list.network_info_ptr[i].network_name_arr,
                net_name_len);
            //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_net_name_len = %d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2431_112_2_18_2_40_26_696,(uint8*)"d", ucs_net_name_len);
            MMI_WSTRNCPY(net_node.net_name,
                MMIMTV_MAX_NAME_LEN,
                net_name_temp,
                MMIMTV_MAX_NAME_LEN,
                ucs_net_name_len);
            break;
            
        case MTV_STIMI_UTF8:
            SCI_MEMSET(net_name_temp, 0, sizeof(net_name_temp));
            ucs_net_name_len = GUI_UTF8ToWstr(net_name_temp,
                MMIMTV_MAX_NAME_LEN,
                network_info_list.network_info_ptr[i].network_name_arr,
                net_name_len);
            //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_net_name_len = %d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2445_112_2_18_2_40_26_697,(uint8*)"d", ucs_net_name_len);
            MMI_WSTRNCPY(net_node.net_name,
                MMIMTV_MAX_NAME_LEN,
                net_name_temp,
                MMIMTV_MAX_NAME_LEN,
                ucs_net_name_len);
            break;
            
        default:    //ascii
            MMI_STRNTOWSTR(net_node.net_name,
                MMIMTV_MAX_NAME_LEN,
                network_info_list.network_info_ptr[i].network_name_arr,
                network_info_list.network_info_ptr[i].network_name_len,
                net_name_len);
            break;
        }
        //add net node
        MMIMTV_AddNetNode(&net_node);
        
        //解析service信息
        if(network_info_list.network_info_ptr[i].service_list.service_number > 0)
        {
            for(k=0; k<network_info_list.network_info_ptr[i].service_list.service_number && k<MMIMTV_SERVICE_NUMBER; k++)
            {
                //alloc service node
                SCI_MEMSET(&service_node, 0, sizeof(MMIMTV_SAVE_SERVICE_INFO_T));
                
                //get service node info
                service_node.net_id     = net_node.net_id;
                service_node.service_id = network_info_list.network_info_ptr[i].service_list.service_arr[k].service_id;
                service_node.service_type = (MTV_STIMI_SERVICE_CLASS_E)network_info_list.network_info_ptr[i].service_list.service_arr[k].ServiceClass;
                //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData network_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2476_112_2_18_2_40_26_698,(uint8*)"d", net_node.net_id);
                //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData service_id = %d, service_type=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2477_112_2_18_2_40_26_699,(uint8*)"dd", service_node.service_id, service_node.service_type);
                
                service_name_len = MMI_MIN(network_info_list.network_info_ptr[i].service_list.service_arr[k].service_name_len, MMIMTV_MAX_NAME_LEN);
                if(service_name_len > 0)
                {
                    switch(network_info_list.network_info_ptr[i].service_list.char_set)
                    {
                    case MTV_STIMI_GB_13000_1:
                        MMI_WSTRNCPY(service_node.service_name,
                            MMIMTV_MAX_NAME_LEN,
                            (wchar*)network_info_list.network_info_ptr[i].service_list.service_arr[k].service_name_arr,
                            network_info_list.network_info_ptr[i].service_list.service_arr[k].service_name_len,
                            service_name_len);
                        break;
                    
                    case MTV_STIMI_KSC_5601:
                        break;
                    
                    case MTV_STIMI_GB_2312:
                        SCI_MEMSET(service_name_temp, 0, sizeof(service_name_temp));
                        ucs_service_name_len = GUI_GBToWstr(service_name_temp,
                            network_info_list.network_info_ptr[i].service_list.service_arr[k].service_name_arr,
                            service_name_len);
                        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_service_name_len = %d."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2500_112_2_18_2_40_26_700,(uint8*)"d", ucs_service_name_len);
                        MMI_WSTRNCPY(service_node.service_name,
                            MMIMTV_MAX_NAME_LEN,
                            service_name_temp,
                            MMIMTV_MAX_NAME_LEN,
                            ucs_service_name_len);
                        break;
                    
                    case MTV_STIMI_UTF8:
                        SCI_MEMSET(service_name_temp, 0, sizeof(service_name_temp));
                        ucs_service_name_len = GUI_UTF8ToWstr(service_name_temp,
                            MMIMTV_MAX_NAME_LEN,
                            network_info_list.network_info_ptr[i].service_list.service_arr[k].service_name_arr,
                            service_name_len);
                        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_service_name_len = %d."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2514_112_2_18_2_40_26_701,(uint8*)"d", ucs_service_name_len);
                        MMI_WSTRNCPY(service_node.service_name,
                            MMIMTV_MAX_NAME_LEN,
                            service_name_temp,
                            MMIMTV_MAX_NAME_LEN,
                            ucs_service_name_len);
                        break;
                    
                    default:    //ascii
                        MMI_STRNTOWSTR(service_node.service_name,
                            MMIMTV_MAX_NAME_LEN,
                            network_info_list.network_info_ptr[i].service_list.service_arr[k].service_name_arr,
                            network_info_list.network_info_ptr[i].service_list.service_arr[k].service_name_len,
                            service_name_len);
                        break;
                    }
                }
                else if(0 == service_name_len)
                {
                    //处理频道名为空的情况，多见音频广播频道名
                    SCI_MEMSET(str_service_name, 0, sizeof(str_service_name));
                    service_name_len = sprintf((char*)str_service_name, "%d", service_node.service_id);
                    MMI_STRNTOWSTR(service_node.service_name,
                        MMIMTV_MAX_NAME_LEN,
                        str_service_name,
                        MMIMTV_MAX_NAME_LEN,
                        service_name_len);
                }

                //add service node
                MMIMTV_AddServiceNode(&net_node, &service_node);
            }
        }

        //解析shortservice信息
        if(network_info_list.network_info_ptr[i].service_list.shortservice_number > 0)
        {
            for(j=0; j<network_info_list.network_info_ptr[i].service_list.shortservice_number && j<(MMIMTV_SERVICE_NUMBER - network_info_list.network_info_ptr[i].service_list.service_number); j++)
            {
                SCI_MEMSET(&shortservice_node, 0, sizeof(MMIMTV_SAVE_SERVICE_INFO_T));
                
                //get shortservice node info
                shortservice_node.net_id       = net_node.net_id;
                shortservice_node.service_id   = network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].service_id;
                shortservice_node.service_type   = (MTV_STIMI_SERVICE_CLASS_E)network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].ServiceClass;
                //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData network_id = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2559_112_2_18_2_40_26_702,(uint8*)"d", net_node.net_id);
                //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData shortservice_id = %d, type=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2560_112_2_18_2_40_26_703,(uint8*)"dd", shortservice_node.service_id, shortservice_node.service_type);
                
                shortservice_name_len = MMI_MIN(network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].service_name_len, MMIMTV_MAX_NAME_LEN);
                if(shortservice_name_len > 0)
                {
                    switch(network_info_list.network_info_ptr[i].service_list.char_set)
                    {
                    case MTV_STIMI_GB_13000_1:
                        MMI_WSTRNCPY(shortservice_node.service_name,
                            MMIMTV_MAX_NAME_LEN,
                            (wchar*)network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].service_name_arr,
                            network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].service_name_len,
                            shortservice_name_len);
                        break;
                    
                    case MTV_STIMI_KSC_5601:
                        break;
                    
                    case MTV_STIMI_GB_2312:
                        SCI_MEMSET(shortservice_name_temp, 0, sizeof(shortservice_name_temp));
                        ucs_shortservice_name_len = GUI_GBToWstr(shortservice_name_temp,
                            network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].service_name_arr,
                            shortservice_name_len);
                        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_GB_2312 ucs_shortservice_name_len = %d."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2583_112_2_18_2_40_26_704,(uint8*)"d", ucs_shortservice_name_len);
                        MMI_WSTRNCPY(shortservice_node.service_name,
                            MMIMTV_MAX_NAME_LEN,
                            shortservice_name_temp,
                            MMIMTV_MAX_NAME_LEN,
                            ucs_shortservice_name_len);
                        break;
                    
                    case MTV_STIMI_UTF8:
                        SCI_MEMSET(shortservice_name_temp, 0, sizeof(shortservice_name_temp));
                        ucs_service_name_len = GUI_UTF8ToWstr(service_name_temp,
                            MMIMTV_MAX_NAME_LEN,
                            network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].service_name_arr,
                            shortservice_name_len);
                        //SCI_TRACE_LOW:"[MMIMTV]: ParseNetworkInfoListData MTV_STIMI_UTF8 ucs_shortservice_name_len = %d."
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2597_112_2_18_2_40_26_705,(uint8*)"d", ucs_shortservice_name_len);
                        MMI_WSTRNCPY(shortservice_node.service_name,
                            MMIMTV_MAX_NAME_LEN,
                            shortservice_name_temp,
                            MMIMTV_MAX_NAME_LEN,
                            ucs_shortservice_name_len);
                        break;
                    
                    default:    //ascii
                        MMI_STRNTOWSTR(shortservice_node.service_name,
                            MMIMTV_MAX_NAME_LEN,
                            network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].service_name_arr,
                            network_info_list.network_info_ptr[i].service_list.shortservice_arr[j].service_name_len,
                            shortservice_name_len);
                        break;
                    }
                }
                else if(0 == shortservice_name_len)
                {
                    //处理频道名为空的情况，多见音频广播频道名
                    SCI_MEMSET(str_service_name, 0, sizeof(str_service_name));
                    shortservice_name_len = sprintf((char*)str_service_name, "%d", shortservice_node.service_id);
                    MMI_STRNTOWSTR(shortservice_node.service_name,
                        MMIMTV_MAX_NAME_LEN,
                        str_service_name,
                        MMIMTV_MAX_NAME_LEN,
                        shortservice_name_len);
                }

                //add service node
                MMIMTV_AddServiceNode(&net_node, &shortservice_node);
            }
        }
    }
}

/*****************************************************************************/
// 	Description : parse program hint info data 
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void ParseProgramHintData(
                                ESG_PROG_HINT_INFO_T *prog_hintinfo //pointer of program hint info struct
                                )
{
    uint16 msg_len = 0;
    uint16 ucs_len = 0;
    wchar  ucs_str[MMIMTV_PROG_HINT_STR_LEN +1] = {0};
    
    if(PNULL == prog_hintinfo
        || PNULL == s_mtv_prog_hint_info_ptr)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2651_112_2_18_2_40_26_706,(uint8*)"");
    if(1 == prog_hintinfo->num || 2 == prog_hintinfo->num)
    {
        SCI_MEMSET(s_mtv_prog_hint_info_ptr, 0, sizeof(MMIMTV_PROG_HINT_INFO_T));
        
        //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData prog_hintinfo->num = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2656_112_2_18_2_40_26_707,(uint8*)"d", prog_hintinfo->num);
        //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData hint_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2657_112_2_18_2_40_26_708,(uint8*)"d", prog_hintinfo->hint_info_arr[0].hint_len);
        msg_len = MMI_MIN(prog_hintinfo->hint_info_arr[0].hint_len, MMIMTV_PROG_HINT_STR_LEN);
        //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData MMI_MIN hint_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2659_112_2_18_2_40_26_709,(uint8*)"d", msg_len);
        if(msg_len > 0)
        {
            s_mtv_prog_hint_info_ptr->service_id = s_mtv_current_para.service_id;
            if(0 == prog_hintinfo->code_char_set)   //GB2312 code
            {
                ucs_len = GUI_GBToWstr(ucs_str,
                    prog_hintinfo->hint_info_arr[0].hint_arr,
                    msg_len);
                //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData ucs_len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2668_112_2_18_2_40_26_710,(uint8*)"d", ucs_len);
                MMI_WSTRNCPY(s_mtv_prog_hint_info_ptr->hint_current,
                    MMIMTV_PROG_HINT_STR_LEN,
                    ucs_str,
                    MMIMTV_PROG_HINT_STR_LEN,
                    ucs_len);
            }
            else if(3 == prog_hintinfo->code_char_set)  //UTF8 code
            {
                ucs_len = GUI_UTF8ToWstr(ucs_str,
                    MMIMTV_PROG_HINT_STR_LEN,
                    prog_hintinfo->hint_info_arr[0].hint_arr,
                    msg_len);
                //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData ucs_len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2681_112_2_18_2_40_26_711,(uint8*)"d", ucs_len);
                MMI_WSTRNCPY(s_mtv_prog_hint_info_ptr->hint_current,
                    MMIMTV_PROG_HINT_STR_LEN,
                    ucs_str,
                    MMIMTV_PROG_HINT_STR_LEN,
                    ucs_len);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData code_char_set = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2690_112_2_18_2_40_26_712,(uint8*)"d", prog_hintinfo->code_char_set);
            }
        }
        
        if(2 == prog_hintinfo->num)
        {
            SCI_MEMSET(ucs_str, 0, sizeof(ucs_str));
            
            //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData hint_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2698_112_2_18_2_40_26_713,(uint8*)"d", prog_hintinfo->hint_info_arr[1].hint_len);
            msg_len = MMI_MIN(prog_hintinfo->hint_info_arr[1].hint_len, MMIMTV_PROG_HINT_STR_LEN);
            //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData MMI_MIN hint_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2700_112_2_18_2_40_26_714,(uint8*)"d", msg_len);
            if(msg_len > 0)
            {
                if(0 == prog_hintinfo->code_char_set)   //GB2312 code
                {
                    ucs_len = GUI_GBToWstr(ucs_str,
                        prog_hintinfo->hint_info_arr[1].hint_arr,
                        msg_len);
                    //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData ucs_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2708_112_2_18_2_40_26_715,(uint8*)"d", ucs_len);
                    MMI_WSTRNCPY(s_mtv_prog_hint_info_ptr->hint_next,
                        MMIMTV_PROG_HINT_STR_LEN,
                        ucs_str,
                        MMIMTV_PROG_HINT_STR_LEN,
                        ucs_len);
                }
                else if(3 == prog_hintinfo->code_char_set)  //UTF8 code
                {
                    ucs_len = GUI_UTF8ToWstr(ucs_str,
                        MMIMTV_PROG_HINT_STR_LEN,
                        prog_hintinfo->hint_info_arr[1].hint_arr,
                        msg_len);
                    //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData ucs_len = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2721_112_2_18_2_40_26_716,(uint8*)"d", ucs_len);
                    MMI_WSTRNCPY(s_mtv_prog_hint_info_ptr->hint_next,
                        MMIMTV_PROG_HINT_STR_LEN,
                        ucs_str,
                        MMIMTV_PROG_HINT_STR_LEN,
                        ucs_len);
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIMTV]: ParseProgramHintData code_char_set = %d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2730_112_2_18_2_40_26_717,(uint8*)"d", prog_hintinfo->code_char_set);
                }
            }
        }
    }
}

/*****************************************************************************/
// 	Description : parse emergency broadcast data
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN ParseEmergencyBroadcastData(MTV_STIMI_SIG_DATA_T *sig_data, uint32 *eb_grade)   //pointer of stimi signal data struct
{
    MTVSTIMI_BROADCAST_MSG_T* broadcast_msg = PNULL;
    //MMIMTV_EB_NODE_T eb_node = {0};
    uint16 msg_len = 0;
    uint16 result_len = 0;
    MMIMTV_EB_NODE_T node = {0};
    
    if(PNULL == sig_data)
    {
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"[MMIMTV]: ParseEmergencyBroadcastData sig_data->data_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2754_112_2_18_2_40_26_718,(uint8*)"d", sig_data->data_type);
    broadcast_msg = (MTVSTIMI_BROADCAST_MSG_T *)sig_data->data_ptr;
    
    if(MTV_STIMI_BROADCAST_MSG == sig_data->data_type && PNULL != broadcast_msg)
    {
        if(PNULL != broadcast_msg->lang_content_ptr)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: ParseEmergencyBroadcastData origin text_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2761_112_2_18_2_40_27_719,(uint8*)"d", broadcast_msg->lang_content_ptr->text_len);
            msg_len = MIN(broadcast_msg->lang_content_ptr->text_len, MMIMTV_EB_CONTENT_MAX);
            //SCI_TRACE_LOW:"[MMIMTV]: ParseEmergencyBroadcastData msg_len = %d,msg_level = %d,code_char_set=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2764_112_2_18_2_40_27_720,(uint8*)"ddd", msg_len,broadcast_msg->level,broadcast_msg->code_char_set);
            if(msg_len > 0 && PNULL != broadcast_msg->lang_content_ptr->text_ptr)
            {
                result_len = GUI_GBToWstr(node.eb_content, broadcast_msg->lang_content_ptr->text_ptr, msg_len);
                //SCI_TRACE_LOW:"[MMIMTV]: ParseEmergencyBroadcastData node.eb_content[MMIMTV_EB_CONTENT_MAX] = %d,result_len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2769_112_2_18_2_40_27_721,(uint8*)"dd", node.eb_content[MMIMTV_EB_CONTENT_MAX],result_len);
            } 
            //for cr159184 159057 level传到MMI的值可能不在MMI所定义的1-5范围
            if(0 == broadcast_msg->level)
            {
                node.eb_grade = 1;
            }
            else if(MMIMTV_BROADCAST_MAX_LEVEL < broadcast_msg->level)
            {
                node.eb_grade = MMIMTV_BROADCAST_MAX_LEVEL;
            }
            else
            {
                node.eb_grade = broadcast_msg->level;
            }
            node.eb_content_len = result_len;
            node.eb_is_read = FALSE;        
            node.eb_service_id = s_mtv_current_para.service_id;    
            node.eb_save_time = MMIAPICOM_Tm2Second(broadcast_msg->time.sec, broadcast_msg->time.min, 
                broadcast_msg->time.hour, broadcast_msg->date.mday, broadcast_msg->date.mon, broadcast_msg->date.year);
            MMIMTV_AddEBNode(&node);
            *eb_grade = node.eb_grade;
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : parse emergency broadcast data
//	Global resource dependence : none
//  Author: xin.li
//	Note: for mbbms
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_ParseEmergencyBroadcastData(MTVSERVICE_CMMB_BROADCAST_IND_T *sig_data, 
                                                    uint32 *eb_grade)   //pointer of stimi signal data struct
{
    MTVSTIMI_BROADCAST_MSG_T* broadcast_msg = PNULL;
    //MMIMTV_EB_NODE_T eb_node = {0};
    uint16 msg_len = 0;
    uint16 result_len = 0;
    MMIMTV_EB_NODE_T node = {0};
    
    if(PNULL == sig_data)
    {
        return FALSE;
    }
    
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData sig_data->data_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2815_112_2_18_2_40_27_722,(uint8*)"d", sig_data->data_type);
    broadcast_msg = (MTVSTIMI_BROADCAST_MSG_T *)sig_data->data_ptr;
    
    if(MTV_STIMI_BROADCAST_MSG == sig_data->data_type && PNULL != broadcast_msg)
    {
        if(PNULL != broadcast_msg->lang_content_ptr)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData origin text_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2822_112_2_18_2_40_27_723,(uint8*)"d", broadcast_msg->lang_content_ptr->text_len);
            msg_len = MIN(broadcast_msg->lang_content_ptr->text_len, MMIMTV_EB_CONTENT_MAX);
            //SCI_TRACE_LOW:"[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData msg_len = %d,msg_level = %d,code_char_set=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2825_112_2_18_2_40_27_724,(uint8*)"ddd", msg_len,broadcast_msg->level,broadcast_msg->code_char_set);
            if(msg_len > 0 && PNULL != broadcast_msg->lang_content_ptr->text_ptr)
            {
                result_len = GUI_GBToWstr(node.eb_content, broadcast_msg->lang_content_ptr->text_ptr, msg_len);
                //SCI_TRACE_LOW:"[MMIMTV]: MMIMBBMS_ParseEmergencyBroadcastData node.eb_content[MMIMTV_EB_CONTENT_MAX] = %d,result_len = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2830_112_2_18_2_40_27_725,(uint8*)"dd", node.eb_content[MMIMTV_EB_CONTENT_MAX],result_len);
            } 
            //for cr159184 159057 level传到MMI的值可能不在MMI所定义的1-5范围
            if(0 == broadcast_msg->level)
            {
                node.eb_grade = 1;
            }
            else if(MMIMTV_BROADCAST_MAX_LEVEL < broadcast_msg->level)
            {
                node.eb_grade = MMIMTV_BROADCAST_MAX_LEVEL;
            }
            else
            {
                node.eb_grade = broadcast_msg->level;
            }
            node.eb_content_len = result_len;
            node.eb_is_read = FALSE;        
            node.eb_service_id = s_mtv_current_para.service_id;    
            node.eb_save_time = MMIAPICOM_Tm2Second(broadcast_msg->time.sec, broadcast_msg->time.min, 
                broadcast_msg->time.hour, broadcast_msg->date.mday, broadcast_msg->date.mon, broadcast_msg->date.year);
            MMIMTV_AddEBNode(&node);
            *eb_grade = node.eb_grade;
            return TRUE;
        }
    }
    return FALSE;
}

/*****************************************************************************/
// 	Description : play default service after search
//	Global resource dependence : none
//  Author: xin.li
//	Note: 搜网完成后默认播放首个频道
/*****************************************************************************/
/*lint -e528*/
LOCAL BOOLEAN PlayDefaultServiceAfterSearch(void)
{
    const MMIMTV_SAVE_NETEID_INFO_T*  net_node_ptr     = MMIMTV_GetPlayingNetNode();
    const MMIMTV_SAVE_SERVICE_INFO_T* service_node_ptr = MMIMTV_GetPlayingServiceNode();
    
    //SCI_TRACE_LOW:"[MMIMTV]: PlayDefaultServiceAfterSearch"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2868_112_2_18_2_40_27_726,(uint8*)"");
    if(PNULL != net_node_ptr)
    {
        SetNetCtrlTableData();
        
        if(PNULL != service_node_ptr)
        {
            //获取首个频道成功
            s_mtv_current_para.net_eid      = service_node_ptr->net_id;
            s_mtv_current_para.service_id   = service_node_ptr->service_id;
            SaveLastServiceId();
            //SCI_TRACE_LOW:"[MMIMTV]: PlayDefaultServiceAfterSearch SaveLastServiceId"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2879_112_2_18_2_40_27_727,(uint8*)"");
            
            //发起播放
            if(MMIMTV_PLAY_STOPPED == s_mtv_current_para.play_status || MMIMTV_PLAY_NONE == s_mtv_current_para.play_status)
            {
                if(CMMB_PlayNetwork(s_mtv_current_para.net_eid,
                    s_mtv_current_para.service_id,
                    s_mtv_current_para.audio_language))
                {
                    //SCI_TRACE_LOW:"[MMIMTV]:PlayDefaultServiceAfterSearch PlayNetwork error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2888_112_2_18_2_40_27_728,(uint8*)"");
                    //提示播放失败，尝试重新搜网
                    OpenPlayNetworkErrScene();
                    return FALSE;
                }
                else
                {
                    //SCI_TRACE_LOW:"[MMIMTV]: PlayDefaultServiceAfterSearch net_id=%d, service_id=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2896_112_2_18_2_40_27_729,(uint8*)"dd", s_mtv_current_para.net_eid,s_mtv_current_para.service_id);
                    s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
                }
            }
            else
            {
                //播放初始状态错误
                //SCI_TRACE_LOW:"[MMIMTV]: PlayDefaultServiceAfterSearch play_status error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2903_112_2_18_2_40_27_730,(uint8*)"");
            }
        }
        else
        {
            //获取首个频道失败，多见于搜网成功但实际数据为空的情况
            //SCI_TRACE_LOW:"[MMIMTV]: PlayDefaultServiceAfterSearch service Failed !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2909_112_2_18_2_40_27_731,(uint8*)"");
        }
    }
    else
    {
        //获取首个网络失败，多见于搜网成功但实际数据为空的情况
        //SCI_TRACE_LOW:"[MMIMTV]: PlayDefaultServiceAfterSearch network Failed !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2915_112_2_18_2_40_27_732,(uint8*)"");
    }

    return TRUE;
}
/*lint -e528*/

/*****************************************************************************/
// 	Description : resume play service after search esg or program
//	Global resource dependence : none
//  Author: xin.li
//	Note: 搜索ESG或搜网失败后恢复节目播放
/*****************************************************************************/
LOCAL void ResumePlayServiceAfterSearch(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: ResumePlayServiceAfterSearch play_status = %d  net_id = %d service_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2928_112_2_18_2_40_27_733,(uint8*)"ddd", s_mtv_current_para.play_status, s_mtv_current_para.net_eid, s_mtv_current_para.service_id);
    
    //重启CMMB后需要重新设置网络控制数据
    SetNetCtrlTableData();
	
	if(0 != s_mtv_current_para.net_eid)
	{
		//发起播放
		if(MMIMTV_PLAY_STOPPED == s_mtv_current_para.play_status || MMIMTV_PLAY_NONE == s_mtv_current_para.play_status)
		{
			if(CMMB_PlayNetwork(s_mtv_current_para.net_eid,
				s_mtv_current_para.service_id,
                s_mtv_current_para.audio_language))
            {
                //SCI_TRACE_LOW:"[MMIMTV]:ResumePlayServiceAfterSearch PlayNetwork error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2942_112_2_18_2_40_27_734,(uint8*)"");
                //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
                //提示播放失败，尝试重新搜网
                OpenPlayNetworkErrScene();
            }
			else
			{
				s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
			}
		}
		else
		{
			//播放初始状态错误
			//SCI_TRACE_LOW:"[MMIMTV]:ResumePlayServiceAfterSearch play_status error"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_2955_112_2_18_2_40_27_735,(uint8*)"");
		}
	}

}

/*****************************************************************************/
// 	Description : set next playing service id
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetNextServiceId(void)
{
    const MMIMTV_SAVE_SERVICE_INFO_T* service_info_ptr = MMIMTV_ChangeService(TRUE);
    
    if(PNULL != service_info_ptr)
    {
        s_mtv_current_para.net_eid      = service_info_ptr->net_id;
        s_mtv_current_para.service_id   = service_info_ptr->service_id;
        s_mtv_current_para.service_type = service_info_ptr->service_type;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : set previous playing service id
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetPreServiceId(void)
{
    const MMIMTV_SAVE_SERVICE_INFO_T* service_info_ptr = MMIMTV_ChangeService(FALSE);
    
    if(PNULL != service_info_ptr)
    {
        s_mtv_current_para.net_eid      = service_info_ptr->net_id;
        s_mtv_current_para.service_id   = service_info_ptr->service_id;
        s_mtv_current_para.service_type = service_info_ptr->service_type;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : switch channel(service) play
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void SwitchServicePlay(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: SwitchServicePlay current play_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3011_112_2_18_2_40_27_736,(uint8*)"d", s_mtv_current_para.play_status);

    s_mtv_current_para.play_type = MMIMTV_PLAY_TYPE_NETWORK;
    
    if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status
        || MMIMTV_PLAY_WAITING == s_mtv_current_para.play_status)
    {
        if(MMIMTV_RECORD_NONE != s_mtv_current_para.record_status)
        {
            //nothing to do
        }
        else
        {
            //stop current play
            if(SCI_SUCCESS != CMMB_Stop())
            {
                //SCI_TRACE_LOW:"[MMIMTV]: Stop error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3027_112_2_18_2_40_27_737,(uint8*)"");
                s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
                //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            }
            else
            {
                s_mtv_current_para.op_type = MMIMTV_OP_PLAY_NEXT;
                s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPING;
            }
        }
    }
    else if(MMIMTV_PLAY_STOPPED == s_mtv_current_para.play_status
            || MMIMTV_PLAY_NONE == s_mtv_current_para.play_status
            || MMIMTV_PLAY_PAUSED == s_mtv_current_para.play_status)
    {
        //SCI_TRACE_LOW:"[MMIMTV]: Switch Channel now play state is stopped"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3042_112_2_18_2_40_27_738,(uint8*)"");
        
        if(s_mtv_current_para.is_need_reset_Vchip)
        {
            s_mtv_current_para.is_need_reset_Vchip = FALSE;
            ResetCMMBModule();
        }

        if(CMMB_PlayNetwork(s_mtv_current_para.net_eid,
            s_mtv_current_para.service_id,
            s_mtv_current_para.audio_language))
        {
            //SCI_TRACE_LOW:"[MMIMTV]: PlayNetwork error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3054_112_2_18_2_40_27_739,(uint8*)"");
            //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            //提示播放失败，尝试重新搜网
            OpenPlayNetworkErrScene();
        }
        else
        {
            s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
        }
    }
}

/*****************************************************************************/
// 	Description : stop current playing
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StopServicePlay(void)
{
    //SCI_TRACE_LOW:"[MMIMBBMS]: StopServicePlay play_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3072_112_2_18_2_40_27_740,(uint8*)"d", s_mtv_current_para.play_status);
    
    if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status || MMIMTV_PLAY_WAITING == s_mtv_current_para.play_status)
    {
        //stop current play
        if(SCI_SUCCESS != CMMB_Stop())
        {
            //SCI_TRACE_LOW:"[MMIMTV]: Stop error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3079_112_2_18_2_40_27_741,(uint8*)"");
            s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
        }
        else
        {
            //s_mtv_current_para.op_type = MMIMTV_OP_PLAY_NEXT;
            s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPING;
        }
    }
}

/*****************************************************************************/
// 	Description : start record tip timer
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void StartRecordTipTimer(void)
{
    if(0 == s_mtv_current_para.record_tip_timer_id)
    {
        s_mtv_current_para.record_tip_timer_id = MMK_CreateWinTimer(MMIMTV_MAIN_WIN_ID, MS_PER_SECOND, TRUE);
    }
}

/*****************************************************************************/
// 	Description : stop record tip timer
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StopRecordTipTimer(void)
{
    if(s_mtv_current_para.record_tip_timer_id > 0)
    {
        MMK_StopTimer(s_mtv_current_para.record_tip_timer_id);
        s_mtv_current_para.record_tip_timer_id = 0;
    }
}

/*****************************************************************************/
// 	Description : play record file
//	Global resource dependence : none
//  Author: xin.li
//	Note: 播放本地录制文件
/*****************************************************************************/
LOCAL void PlayRecordfile(void)
{
    //获取录像文件的总时长
    s_mtv_record_file_total_time = CMMB_GetStreamLength(s_mtv_current_para.record_file_full_name);
    //SCI_TRACE_LOW:"[MMIMTV]: PlayRecordfile record fiel total time = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3123_112_2_18_2_40_27_742,(uint8*)"d", s_mtv_record_file_total_time);
    
    //设置播放类型
    s_mtv_current_para.play_type = MMIMTV_PLAY_TYPE_RECORD;
    if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status || MMIMTV_PLAY_WAITING == s_mtv_current_para.play_status)
    {
        //stop current play
        if(SCI_SUCCESS != CMMB_Stop())
        {
            //SCI_TRACE_LOW:"[MMIMTV]: Stop error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3132_112_2_18_2_40_27_743,(uint8*)"");
            s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
            //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
        }
        else
        {
            s_mtv_current_para.op_type = MMIMTV_OP_PLAY_RECORD;
            s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPING;
        }
    }
    else if(MMIMTV_PLAY_STOPPED == s_mtv_current_para.play_status || MMIMTV_PLAY_NONE == s_mtv_current_para.play_status)
    {
        //SCI_TRACE_LOW:"[MMIMTV]: PlayRecordfile now play state is stopped"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3144_112_2_18_2_40_27_744,(uint8*)"");
        //reset CMMB first
        ResetCMMBModule();
        if(CMMB_PlayStream(s_mtv_current_para.record_file_full_name,
            s_mtv_current_para.audio_language))
        {
            //SCI_TRACE_LOW:"[MMIMTV]: PlayStream error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3150_112_2_18_2_40_27_745,(uint8*)"");
            //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
            
            //播放录像文件错误，提示
            OpenPlayStreamErrScene();
            
            //恢复直播
            SwitchServicePlay();
        }
        else
        {
            s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
        }
    }
}

/*****************************************************************************/
// 	Description : launch auto search (自动全搜索)
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StartAutoSearch(void)
{
    uint32  stimi_ret = 0;           
    //SCI_TRACE_LOW:"[MMIMTV]: StartAutoSearch current_play_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3174_112_2_18_2_40_27_746,(uint8*)"d", s_mtv_current_para.play_status);
    
    s_search_mode.search_type = MMIMTV_AUTO_SEARCH;
    //display search waiting tip
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_SEARCH_WAITING);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
    
    //launch auto search
    if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status || MMIMTV_PLAY_WAITING == s_mtv_current_para.play_status)
    {
        if(MMIMTV_RECORD_NONE != s_mtv_current_para.record_status)
        {
            //nothing to do
        }
        else
        {
            //stop current play
            if(SCI_SUCCESS != CMMB_Stop())
            {
                //SCI_TRACE_LOW:"[MMIMTV]: Stop error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3194_112_2_18_2_40_27_747,(uint8*)"");
                s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
                //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            }
            else
            {
                s_mtv_current_para.op_type = MMIMTV_OP_SEARCH;
                s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPING;
            }
        }
    }
    else if(MMIMTV_PLAY_STOPPED == s_mtv_current_para.play_status || MMIMTV_PLAY_NONE == s_mtv_current_para.play_status)
    {
        //reset CMMB first
#ifndef WIN32
        ResetCMMBModule();
#endif

        //launch auto search
        stimi_ret = CMMB_SearchProgram();
        if(MTV_STIMI_ERR_NONE != stimi_ret)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: CMMB_SearchProgram error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3216_112_2_18_2_40_27_748,(uint8*)"");
            
            //提示搜网失败
            OpenSearchProgramErrScene();
            
            //恢复节目播放
            ResumePlayServiceAfterSearch();
        }
    }
}

/*****************************************************************************/
// 	Description : launch frequency search (手动搜索/按频点搜索)
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StartFrequencySearch(void)
{
    uint32  stimi_ret = 0;           
    //SCI_TRACE_LOW:"[MMIMTV]: StartFrequencySearch current_play_status = %d  frequency = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3235_112_2_18_2_40_27_749,(uint8*)"dd", s_mtv_current_para.play_status, s_search_mode.center_freq);
    
    s_search_mode.search_type = MMIMTV_MANUAL_SEARCH;
    
    //display search waiting tip
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_SEARCH_WAITING);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);

    //launch frequency search
    if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status || MMIMTV_PLAY_WAITING == s_mtv_current_para.play_status)
    {
        if(MMIMTV_RECORD_NONE != s_mtv_current_para.record_status)
        {
            //nothing to do
        }
        else
        {
            //stop current play
            if(SCI_SUCCESS != CMMB_Stop())
            {
                //SCI_TRACE_LOW:"[MMIMTV]: Stop error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3256_112_2_18_2_40_28_750,(uint8*)"");
                s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
                //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            }
            else
            {
                s_mtv_current_para.op_type = MMIMTV_OP_SEARCH;
                s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPING;
            }
        }
    }
    else if(MMIMTV_PLAY_STOPPED == s_mtv_current_para.play_status || MMIMTV_PLAY_NONE == s_mtv_current_para.play_status)
    {
        //reset CMMB first
        ResetCMMBModule();

        stimi_ret = CMMB_SearchCenterFrq(s_search_mode.center_freq);
        if(MTV_STIMI_ERR_NONE != stimi_ret)
        {
            if(MTV_STIMI_INVALID_CENTER_FRQ == stimi_ret)
            {
                //invalid frequency input
                MMIMTV_SetPreOSDScene(OSD_MANUAL_SEARCH);
                
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_SEARCH_INVALID_FRQ);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV]: CMMB_SearchCenterFrq error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3286_112_2_18_2_40_28_751,(uint8*)"");
                //提示搜网失败
                OpenSearchProgramErrScene();
            }
            //恢复节目播放
            ResumePlayServiceAfterSearch();
        }
    }
}

/*****************************************************************************/
// 	Description : esg search
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void StartESGSearch(void)
{
	uint32  stimi_ret = 0;           

    //SCI_TRACE_LOW:"[MMIMTV]: StartESGSearch current_play_status = %d  net_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3305_112_2_18_2_40_28_752,(uint8*)"dd", s_mtv_current_para.play_status, s_esglist_param.net_id);
    
    s_search_mode.search_type = MMIMTV_ESG_SEARCH;

    //ESG解析中，先停止
    if(MMIMTV_ESG_PARSE_PARSING == s_mtv_current_para.esg_parse_status)
    {
        CMMB_AbortESG();
        s_mtv_current_para.esg_parse_status = MMIMTV_ESG_PARSE_NONE;
    }

	//set esg path
	if(!MMIMTV_IsEnoughESGSpace())
	{
		//无足够的存储空间保存ESG
		MMIMTV_SetLastOSDScene();
		MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
		MMIMTV_SetFocusOSDScene(OSD_PUB);
		MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_ESG_NO_SPACE);
		MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_ESG_NO_SPACE);
		
		return;
	}
	//MMIMTV_SetESGPath();
	
    //display search waiting tip
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_SEARCH_WAITING);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
    
    if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status || MMIMTV_PLAY_WAITING == s_mtv_current_para.play_status)
    {
        if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
        {
            //当前正在录制，提示无法进入
            MMIMTV_SetPreOSDScene(OSD_ESG_LIST);
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_TIP_RECORDING);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_TIP_RECORDING);
        }
        else
        {
            //stop current play first
            if(SCI_SUCCESS != CMMB_Stop())
            {
                //SCI_TRACE_LOW:"[MMIMTV]: Stop error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3351_112_2_18_2_40_28_753,(uint8*)"");
                s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
                //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            }
            else
            {
                s_mtv_current_para.op_type = MMIMTV_OP_SEARCH;
                s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPING;
            }
        }
    }
    else if(MMIMTV_PLAY_STOPPED == s_mtv_current_para.play_status || MMIMTV_PLAY_NONE == s_mtv_current_para.play_status)
    {
        //launch esg search
        MMIMTV_SetESGPath();
        stimi_ret = CMMB_SearchESGData(s_esglist_param.net_id);
        //SCI_TRACE_LOW:"[MMIMTV]: StartESGSearch stimi_ret = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3367_112_2_18_2_40_28_754,(uint8*)"d", stimi_ret);
        if(MTV_STIMI_ERR_NONE != stimi_ret)
        {
            //搜索ESG失败
            MMIMTV_SetPreOSDScene(OSD_ESG_LIST);
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_SEARCH_ESG_FAIL);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_SEARCH_ESG_FAIL);

            //恢复节目播放
            ResumePlayServiceAfterSearch();
        }
    }
}

/*****************************************************************************/
// 	Description : ESG parse
//	Global resource dependence : none
//  Author: xin.li
//	Note: 解析是异步的, 解析完成MTV_ESG_PARSE_RESULT_IND
/*****************************************************************************/
LOCAL void StartESGParse(uint32 net_id)
{
    ESG_ERROR_E esg_parse_return = ESG_SUCCESS;
    
    //SCI_TRACE_LOW:"[MMIMTV]: StartESGParse net_id = %d, parse_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3391_112_2_18_2_40_28_755,(uint8*)"dd", net_id, s_mtv_current_para.esg_parse_status);
    
    if(0 != net_id)
    {
        s_search_mode.esg_netid = net_id;
        s_esglist_param.net_id  = net_id;
        
        //设置ESG路径
        if(!MMIMTV_IsEnoughESGSpace())
        {
            //SCI_TRACE_LOW:"[MMIMTV]: StartESGParse no enough space"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3401_112_2_18_2_40_28_756,(uint8*)"");
        }
        MMIMTV_SetESGPath();
        
        if(MMIMTV_ESG_PARSE_PARSING == s_mtv_current_para.esg_parse_status)
        {
            CMMB_AbortESG();
            s_mtv_current_para.esg_parse_status = MMIMTV_ESG_PARSE_NONE;
        }
        //发起解析
        esg_parse_return = CMMB_ParseESG(s_esglist_param.net_id);
        //SCI_TRACE_LOW:"[MMIMTV]: CMMB_ParseESG return = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3412_112_2_18_2_40_28_757,(uint8*)"d", esg_parse_return);
        s_mtv_current_para.esg_parse_status = MMIMTV_ESG_PARSE_PARSING;
    }
}

/*****************************************************************************/
// 	Description : reset and quit
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void ResetAndQuit(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: ResetAndQuit !"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3423_112_2_18_2_40_28_758,(uint8*)"");
    
    //program hint mem free
    if(PNULL != s_mtv_prog_hint_info_ptr)
    {
        SCI_FREE(s_mtv_prog_hint_info_ptr);
        s_mtv_prog_hint_info_ptr = PNULL;
    }
    
    //network control table mem free
    if(PNULL != s_mtv_nv_ctl_table_ptr)
    {
        SCI_FREE(s_mtv_nv_ctl_table_ptr);
        s_mtv_nv_ctl_table_ptr = PNULL;
    }
    
    //reset status
    s_mtv_current_para.op_type = MMIMTV_OP_NONE;
    s_mtv_current_para.play_status          = MMIMTV_PLAY_NONE;
    s_mtv_current_para.play_type            = MMIMTV_PLAY_TYPE_NETWORK;
    s_mtv_current_para.is_playing_broadcast = FALSE;
    s_mtv_current_para.is_recived_stop_ack  = FALSE;
    s_mtv_current_para.esg_parse_status     = MMIMTV_ESG_PARSE_NONE;
    s_mtv_current_para.is_need_reset_Vchip  = FALSE;
    
    s_focus_osd         = OSD_NONE;
    s_pre_osd           = OSD_NONE;
    s_osd_restore_param = 0;
    s_is_mtv_open       = FALSE;
    
    //通知调用关机函数
    if(s_is_delay_poweroff)
    {
        MMIMTV_SetDelayPowerOff(FALSE);
        MMIAPIPHONE_PowerOffEx();
    }
}

/*****************************************************************************/
// 	Description : reset CMMB module
//	Global resource dependence : none
//  Author: xin.li
//	Note: 播放录制节目前重置CMMB模块，解决回放卡，易ASSERT，定屏等问题
/*****************************************************************************/
LOCAL void ResetCMMBModule(void)
{
	uint32	init_ret = 0;
    AUDIO_DEVICE_MODE_TYPE_E audio_mode = 0;
    MMISRV_HANDLE_T          audio_handle = MMIMTV_GetAudioHandle();
	
	//SCI_TRACE_LOW:"[MMIMTV]: ResetCMMBModule !"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3471_112_2_18_2_40_28_759,(uint8*)"");

    //ESG异步解析中
    if(MMIMTV_ESG_PARSE_PARSING == s_mtv_current_para.esg_parse_status)
    {
        CMMB_AbortESG();
        s_mtv_current_para.esg_parse_status = MMIMTV_ESG_PARSE_NONE;
    }

	//CMMB and MTV clearup
    CMMB_Cleanup();
    MTV_Close();

	//init mtv
	init_ret = MTV_Initialize((MTV_MODE_E)MTV_MODE_CMMB,(MTV_CALLBACK)MMIMTV_CallBack,PNULL);
	if(0 != init_ret)
	{
		//SCI_TRACE_LOW:"[MMIMTV]: MTV_Initialize error"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3488_112_2_18_2_40_28_760,(uint8*)"");
		MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
	}
	
	//set lcd
	s_lcdtv_spec.back_color = MMI_BLUE_COLOR;
	s_lcdtv_spec.order	= LCDTV_LAYER_ORDER_OSD;
	s_lcdosd_spec.alpha = MMIMTV_GetTransparencyValue(MMIMTV_GetTransparency());
	LCD_Init();
	LCDTV_SetLCDSpec(&s_lcdtv_spec);
	LCDTV_SetOSDSpec(&s_lcdosd_spec);
    MMIMTV_SetVideoSpec(MMIMTV_GetZoomMode());
    LCDTV_Invalidate(0);
    LCDTV_EnableLCDSpec(TRUE);
    
    //init cmmb
    CMMB_Init(1);
    
    //set audio volume
    //SCI_TRACE_LOW:"[MMIMTV]: HandleInitializingSceneMsg, volume_value = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3507_112_2_18_2_40_28_761,(uint8*)"d", MMIMTV_GetVolume());
    CMMB_SetVolume(MMIMTV_DSP_VOLUME, MMIMTV_GetVolume());
//     MMIAPISET_GetDeviceMode(&audio_mode);
//     MMIAUDIO_SetMute(FALSE);
//     MMIAUDIO_SetVolume(audio_mode, MMIMTV_GetVolume());
    if(audio_handle > 0)
    {
        MMISRVAUD_SetVolume(audio_handle, MMIMTV_GetVolume());
    }
    
    if(MMIMTV_IsMute())
    {
        CMMB_Mute();
    }
    
    //设置背景图片
    MMIMTV_SetBMPtoVChip(IMAGE_MTV_INIT_BG);
    
    //set net control data
    SetNetCtrlTableData();
    
    //重新启动ESG异步解析
    StartESGParse(s_mtv_current_para.net_eid);
    
    //reset play status
    s_mtv_current_para.net_eid = 0;
    s_mtv_current_para.service_id = 0;
    s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
}

/*****************************************************************************/
// 	Description : set video spec
//	Global resource dependence : none
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_SetVideoSpec(
                        MMIMTV_ZOOM_MODE_E zoom_mode   //video layer size mode
                        )
{
    uint32 zoom_height  = MMI_MAINSCREEN_HEIGHT;
    uint32 zoom_width   = MMI_MAINSCREEN_WIDTH;
    
    /*lint -e774*/
    switch(zoom_mode) 
    {
    case MMIMTV_ZOOM_16_9:
        if(zoom_width >zoom_height *9/16)
        {
            zoom_height = MMI_MAINSCREEN_HEIGHT;
            zoom_width  = MMI_MAINSCREEN_HEIGHT *9/16;
            //确保4字节对齐
            if(0 != zoom_width%4)
            {
                zoom_width -= zoom_width%4;
            }
            
            s_lcdvideo_spec.lcd_disp_left   = (MMI_MAINSCREEN_WIDTH - zoom_width)/2;
            s_lcdvideo_spec.lcd_disp_top    = 0;
            s_lcdvideo_spec.lcd_disp_right  = s_lcdvideo_spec.lcd_disp_left + zoom_width -1;
            s_lcdvideo_spec.lcd_disp_bottom = MMI_MAINSCREEN_HEIGHT -1;
        }
        else
        {
            zoom_height = MMI_MAINSCREEN_WIDTH *16/9;
            zoom_width  = MMI_MAINSCREEN_WIDTH;
            //确保4字节对齐
            if(0 != zoom_height%4)
            {
                zoom_height -= zoom_height%4;
            }
            
            s_lcdvideo_spec.lcd_disp_left   = 0;
            s_lcdvideo_spec.lcd_disp_top    = (MMI_MAINSCREEN_HEIGHT - zoom_height)/2;
            s_lcdvideo_spec.lcd_disp_right  = MMI_MAINSCREEN_WIDTH -1;
            s_lcdvideo_spec.lcd_disp_bottom = s_lcdvideo_spec.lcd_disp_top + zoom_height -1;
        }
        break;
        
    case MMIMTV_ZOOM_4_3:
        if(zoom_width >zoom_height *3/4)
        {
            zoom_height = MMI_MAINSCREEN_HEIGHT;
            zoom_width  = MMI_MAINSCREEN_HEIGHT *3/4;
            //确保4字节对齐
            if(0 != zoom_width%4)
            {
                zoom_width -= zoom_width%4;
            }
            
            s_lcdvideo_spec.lcd_disp_left   = (MMI_MAINSCREEN_WIDTH - zoom_width)/2;
            s_lcdvideo_spec.lcd_disp_top    = 0;
            s_lcdvideo_spec.lcd_disp_right  =  s_lcdvideo_spec.lcd_disp_left + zoom_width -1;
            s_lcdvideo_spec.lcd_disp_bottom = MMI_MAINSCREEN_HEIGHT -1;
        }
        else
        {
            zoom_height = MMI_MAINSCREEN_WIDTH *4/3;
            zoom_width  = MMI_MAINSCREEN_WIDTH;
            //确保4字节对齐
            if(0 != zoom_height%4)
            {
                zoom_height -= zoom_height%4;
            }
            
            s_lcdvideo_spec.lcd_disp_left   = 0;
            s_lcdvideo_spec.lcd_disp_top    = (MMI_MAINSCREEN_HEIGHT - zoom_height)/2;
            s_lcdvideo_spec.lcd_disp_right  = MMI_MAINSCREEN_WIDTH -1;
            s_lcdvideo_spec.lcd_disp_bottom = s_lcdvideo_spec.lcd_disp_top + zoom_height -1;
        }
        break;
        
    case MMIMTV_ZOOM_FRAME:
        s_lcdvideo_spec.lcd_disp_left   = 0;
        s_lcdvideo_spec.lcd_disp_top    = 0;
        s_lcdvideo_spec.lcd_disp_right  = MMI_MAINSCREEN_WIDTH - 1;
        s_lcdvideo_spec.lcd_disp_bottom = MMI_MAINSCREEN_HEIGHT - 1;
        break;
        
    default:
        break;
    }
    /*lint -e774*/
    LCDTV_SetVideoSpec(&s_lcdvideo_spec);
}

/*****************************************************************************/
// 	Description : 提示播放错误，需重启
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenPlayNetworkErrScene(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: OpenPlayNetworkErrScene net_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3637_112_2_18_2_40_28_762,(uint8*)"d", s_mtv_current_para.net_eid, s_mtv_current_para.service_id);

    MMIMTV_SetPreOSDScene(OSD_SHORTCUT_ICON);
    
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PUB);
    //保存OSD_PUB场景参数，保证外部中断返回正常显示OSD_PUB场景
    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_NETWORK_ERROR_NEED_RETRY);
}

/*****************************************************************************/
// 	Description : 提示播放录像文件错误
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenPlayStreamErrScene(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: OpenPlayStreamErrScene"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3654_112_2_18_2_40_28_763,(uint8*)"");
    
    MMIMTV_SetPreOSDScene(OSD_SHORTCUT_ICON);
    
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PUB);
    //保存OSD_PUB场景参数，保证外部中断返回正常显示OSD_PUB场景
    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_PLAY_STREAM_ERROR);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_PLAY_STREAM_ERROR);
    
    //重置播放类型
    s_mtv_current_para.play_type = MMIMTV_PLAY_TYPE_NETWORK;
}

/*****************************************************************************/
// 	Description : 提示搜索网络失败
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenSearchProgramErrScene(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: OpenSearchProgramErrScene"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3674_112_2_18_2_40_28_764,(uint8*)"");
    
    MMIMTV_SetPreOSDScene(OSD_SHORTCUT_ICON);
    
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PUB);
    //保存OSD_PUB场景参数，保证外部中断返回正常显示OSD_PUB场景
    MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_SEARCH_NETWORK_ERROR);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_SEARCH_NETWORK_ERROR);
}

/*****************************************************************************/
// 	Description : open play connecting scene
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void OpenPlayConnectingScene(void)
{
    //连接等待场景
    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
    MMIMTV_SetFocusOSDScene(OSD_PLAY_CONNECTING);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
}

/*****************************************************************************/
// 	Description : OSD_NONE 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 空场景，不响应用户操作
/*****************************************************************************/
LOCAL void HandleNoneSceneMsg(
                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                              DPARAM                param   //the param of the message
                              )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleNoneSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3706_112_2_18_2_40_28_765,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_ClearScreen();
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_ClearScreen();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_ENTER_WARNING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleEnterWarningSceneMsg(
                                      MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                      DPARAM                param   //the param of the message
                                      )
{
    //GUI_POINT_T      dis_point = {0};
    //GUI_LCD_DEV_INFO dev_info = {0};	
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleWarningSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3735_112_2_18_2_40_28_766,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_ClearScreen();
        MMIMTV_OpenCommonQuerySceneWithSRExit(TXT_MTV_WARNING);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //close OSD_ENTER_WARNING, enter OSD_INITIALIZING
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_INITIALIZING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_CANCEL:
        //退出，关闭MTV窗口
        MMIMTV_CloseCommonMsgBox();
        MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_INITIALIZING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 初始化完毕，播放成功后OSD效果才生效
/*****************************************************************************/
LOCAL void HandleInitializingSceneMsg(
                                      MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                      DPARAM                param   //the param of the message
                                      )
{
    //uint32              init_ret = 0;
    //GUI_POINT_T         dis_point = {0};
    //AUDIO_DEVICE_MODE_TYPE_E audio_mode = 0;
    //GUI_LCD_DEV_INFO    dev_info = {0};
    //uint32              volume_value = 0;

    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleInitializingSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3782_112_2_18_2_40_29_767,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonWaitWithNoCancelScene(TXT_COMMON_WAITING);

        //网络列表为空时，提示进行CMMB 搜网
        if(MMIMTV_GetNetListNumber() > 0)
        {
            //set network ctrl table data
            SetNetCtrlTableData();
            
            if(GetLastServiceId())
            {
                //set current service position in net/service list
                MMIMTV_SetPlayingNetServiceNodeViaID(s_mtv_current_para.net_eid, s_mtv_current_para.service_id);
                
                if(CMMB_PlayNetwork(s_mtv_current_para.net_eid,
                    s_mtv_current_para.service_id,
                    s_mtv_current_para.audio_language))
                {
                    //SCI_TRACE_LOW:"[MMIMTV]: HandleInitializingSceneMsg PlayNetwork error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3803_112_2_18_2_40_29_768,(uint8*)"");
                    s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
                    //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
                    //提示播放失败，尝试重新搜网
                    OpenPlayNetworkErrScene();
                    return;
                }
                else
                {
                    s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
                }
            }
            
            //在初始化完毕，收到播放相关消息前用户可操作场景
            MMIMTV_SetPreOSDScene(OSD_IDLE);	//初始需设置默认的前一个场景,以处理初次进入播放异常的情况
#ifndef MBBMS_SUPPORT
            MMIMTV_SetFocusOSDScene(OSD_IDLE); //初次进入打开OSD_SHORTCUT_ICON场景
#else
            MMIMTV_SetFocusOSDScene(OSD_CMMB_MODE_WARNING); //无SIM卡或无网警告
#endif
        }
        else
        {
            //s_is_need_reset_netctrldata = TRUE;
            //no last played service
            MMIMTV_SetFocusOSDScene(OSD_NO_LAST_SERVICE);
        }
        //关闭OSD_INITIALIZING,进入下一场景
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_ClearScreen();  //确保清除BB端之前的背景图
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        MMIMTV_CreateWatchTimeTipTimer();
        StartESGParse(s_mtv_current_para.net_eid);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_NO_LAST_SERVICE 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleNoLastServiceSceneMsg(
                                       MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                       DPARAM                param   //the param of the message
                                       )
{
    uint32 stimi_ret = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleNoLastServiceSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3858_112_2_18_2_40_29_769,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonQuerySceneWithSRExit(TXT_NO_SERVICE_SEARCH);
        break;

    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        //display search waiting tip
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_SEARCH_WAITING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        
        //launch auto search
        s_search_mode.search_type = MMIMTV_AUTO_SEARCH;
        stimi_ret = CMMB_SearchProgram();
        if(MTV_STIMI_ERR_NONE != stimi_ret)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: CMMB_SearchProgram error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3877_112_2_18_2_40_29_770,(uint8*)"");
            //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL uint32 ConvertMtvDigiMsgToNum(MMIMTV_KEY_MAP_TYPE_E msg_id)
{
    uint32 num = 0;
    
    switch(msg_id)
    {
    case MMIMTV_APP_0:
        num = 0;
        break;
        
    case MMIMTV_APP_1:
        num = 1;
        break;
        
    case MMIMTV_APP_2:
        num = 2;
        break;
        
    case MMIMTV_APP_3:
        num = 3;
        break;
        
    case MMIMTV_APP_4:
        num = 4;
        break;
        
    case MMIMTV_APP_5:
        num = 5;
        break;
        
    case MMIMTV_APP_6:
        num = 6;
        break;
        
    case MMIMTV_APP_7:
        num = 7;
        break;
        
    case MMIMTV_APP_8:
        num = 8;
        break;
        
    case MMIMTV_APP_9:
        num = 9;
        break;
        
    default:
        break;
    }

    return num;
}

/*****************************************************************************/
// 	Description : OSD_IDLE 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 全屏播放场景，无实际内容显示
/*****************************************************************************/
LOCAL void HandleIdleSceneMsg(
                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                              DPARAM                param   //the param of the message
                              )
{
    //MMIMTV_SAVE_SERVICE_INFO_T* service_node_ptr = PNULL;
    uint32  service_index = 0;
    //MMI_WIN_ID_T  pre_win_id = 0;
    //MMI_HANDLE_T  pre_win_handle = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleIdleSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_3967_112_2_18_2_40_29_771,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_ClearScreen();
        MMIMBBMS_ClearNumSwitchInfo();
        break;
        
    case MMIMTV_APP_OK:
        MMIMTV_StopStbDisableTimer();
        if(MMIMTV_IsOpenSwitchServiceTab())
        {
            MMIMTV_CloseSwitchServiceTab();
        }
        MMIMTV_ClearScreen();
        MMIMTV_SetFocusOSDScene(OSD_SHORTCUT_ICON);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_WEB:
        MMIMTV_StopStbDisableTimer();
        if(MMIMTV_IsOpenSwitchServiceTab())
        {
            //频道切换栏播放
            service_index = GUITAB_GetCurSel(MTV_SWITCH_PLAY_TAB_CTRL_ID);
            
            MMIMTV_CloseSwitchServiceTab();
            MMIMTV_ClearScreen();
	        if(MMIMBBMS_FLOW_STATE_IDLE == GetMSKUpdateStatus()||
				MMIMBBMS_FLOW_STATE_STOPPING == GetMSKUpdateStatus())
			{	
	            MMIMBBMS_PlayServiceByIndex((uint16)service_index);
			}	
            else
			{
	            MMIMTV_SetPreOSDScene(OSD_IDLE);
	            MMIMTV_SetFocusOSDScene(OSD_PUB);
	            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_MSK_UPDATE_WAIT);
	            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_MSK_UPDATE_WAIT);		
			}          
        }
        else
        {
            MMIMTV_ClearScreen();
            MMIMTV_SetFocusOSDScene(OSD_SHORTCUT_ICON);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        break;
        
    case MMIMTV_APP_DOWN:
        MMIMTV_StopStbDisableTimer();
        MMIMTV_StartStbDisableTimer();     
        MMIMTV_EnableStb();        
        MMIMTV_ResetDisappearTimer();
        MMIMTV_TurnDownVolume();
        
        break;
    case MMIMTV_APP_UP:
        MMIMTV_StopStbDisableTimer();        
        MMIMTV_StartStbDisableTimer();      
        MMIMTV_EnableStb();        
        MMIMTV_ResetDisappearTimer();
        MMIMTV_TurnUpVolume();
        break;
        
    case MMIMTV_APP_LEFT:
        //左右键打开频道切换栏
        MMIMTV_StopStbDisableTimer();
        if(!MMIMTV_IsOpenSwitchServiceTab())
        {
            MMIMTV_OpenSwitchServiceTab(NULL);
        }
        break;
        
    case MMIMTV_APP_RIGHT:
        //左右键打开频道切换栏
        MMIMTV_StopStbDisableTimer();
        if(!MMIMTV_IsOpenSwitchServiceTab())
        {
            MMIMTV_OpenSwitchServiceTab(NULL);
        }
        break;
        
    case MMIMTV_APP_CAMERA:
        #ifndef MBBMS_SUPPORT
        //录制节目
        MMIMTV_StartRecord();
        #endif
        break;
        
    case MMIMTV_APP_0:
    case MMIMTV_APP_1:
    case MMIMTV_APP_2:
    case MMIMTV_APP_3:
    case MMIMTV_APP_4:
    case MMIMTV_APP_5:
    case MMIMTV_APP_6:
    case MMIMTV_APP_7:
    case MMIMTV_APP_8:
    case MMIMTV_APP_9:
        //数字键切台，最高支持三位数
        {
            uint32  num = 0;
            
            MMIMTV_StopStbDisableTimer();
            
            if(MMIMTV_IsOpenSwitchServiceTab())
            {
                MMIMTV_CloseSwitchServiceTab();
            }
            MMIMTV_ClearScreen();
            
            num = ConvertMtvDigiMsgToNum(msg_id);
            //input num count
            GetNumSwitchInfo()->input_count ++;
            if(GetNumSwitchInfo()->input_count <= 3)
            {
                GetNumSwitchInfo()->input_num = (GetNumSwitchInfo()->input_num)*10 + num;
                if(GetNumSwitchInfo()->input_count < 3)
                {
                    MMIMTV_StartNumSwitchTimer();
                    MMIMTV_DisplayInputNum(GetNumSwitchInfo()->input_num);
                }
                else
                {
                    //input num full, launch play directly
                    MMIMTV_StopNumSwitchTimer();
                    MMIMTV_DisplayInputNum(GetNumSwitchInfo()->input_num);
                    SCI_Sleep(200);
                    
                    MMIMTV_NumSwitchPlay();
                }
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV]: HandleIdleSceneMsg, num switch error! input_count %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4102_112_2_18_2_40_29_772,(uint8*)"d", GetNumSwitchInfo()->input_count);
            }
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        if(MMIMTV_IsOpenSwitchServiceTab())
        {
            MMIMTV_CloseSwitchServiceTab();
        }
        MMIMTV_ClearScreen();
        
        //停止播放、关闭播放窗
        MMIMBBMS_StopPlay();
        MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
        break;
        
    case MMIMTV_APP_TP_DOWN:
        break;
        
    case MMIMTV_CLOSE_SCENE:
        if(MMIMTV_IsOpenSwitchServiceTab())
        {
            MMIMTV_CloseSwitchServiceTab();
        }
        //MMIMTV_CloseNoneScene();
        MMIMTV_ClearVolumeProgress();
        MMIMTV_ClearScreen();
        MMIMTV_StopNumSwitchTimer();
        MMIMTV_StopStbDisableTimer();
        break;
        
    default:
        break;
    }
    
}

/*****************************************************************************/
// 	Description : 频道切换栏超到时播放
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_TimeOutPlay(void)
{
    //MMIMTV_SAVE_SERVICE_INFO_T* service_node_ptr = PNULL;
    uint32  service_index = 0;
	
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_TimeOutPlay"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4149_112_2_18_2_40_29_773,(uint8*)"");
    if(MMIMTV_IsOpenSwitchServiceTab())
    {
        //频道切换栏播放
        service_index = GUITAB_GetCurSel(MTV_SWITCH_PLAY_TAB_CTRL_ID);
        
        MMIMTV_CloseSwitchServiceTab();
        MMIMTV_ClearScreen();
        if(MMIMBBMS_FLOW_STATE_IDLE == GetMSKUpdateStatus()||
			MMIMBBMS_FLOW_STATE_STOPPING == GetMSKUpdateStatus())
		{	
            MMIMBBMS_PlayServiceByIndex((uint16)service_index);
        }
        else
		{
            MMIMTV_SetPreOSDScene(OSD_IDLE);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_MSK_UPDATE_WAIT);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_MSK_UPDATE_WAIT);		
		}
    }
}

/*****************************************************************************/
// 	Description : 数字键切换频道播放
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_NumSwitchPlay(void)
{
    uint32  service_index = 0;
    
    service_index = GetNumSwitchInfo()->input_num;
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_NumSwitchPlay input_num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4181_112_2_18_2_40_29_774,(uint8*)"d", GetNumSwitchInfo()->input_num);
    
    if(OSD_IDLE != MMIMTV_GetFocusOSDScene())
    {
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_NumSwitchPlay !OSD_IDLE osd=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4185_112_2_18_2_40_29_775,(uint8*)"d", MMIMTV_GetFocusOSDScene());
        return;
    }
    
    //check service index
    if(MMIMBBMS_IsServiceIndexValid(service_index))
    {
        if(MMIMBBMS_FLOW_STATE_IDLE == GetMSKUpdateStatus()||
			MMIMBBMS_FLOW_STATE_STOPPING == GetMSKUpdateStatus())
		{
            MMIMBBMS_PlayServiceByIndex((uint16)service_index);		
		}	
        else
		{
            MMIMTV_SetPreOSDScene(OSD_IDLE);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_MSK_UPDATE_WAIT);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_MSK_UPDATE_WAIT);		
		}
    }
    else
    {
        //invalid num
        MMIMTV_SetPreOSDScene(OSD_IDLE);
        MMIMTV_SetFocusOSDScene(OSD_PUB);
        MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_SERVICE_NOT_EXIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_SERVICE_NOT_EXIST);
    }
}

/*****************************************************************************/
// 	Description : 获取数字键切台信息
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL MMIMTV_NUM_SWITCH_INFO_T* GetNumSwitchInfo(void)
{
    return &s_num_swith_info;
}

/*****************************************************************************/
// 	Description : 清空数字键切台信息
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMBBMS_ClearNumSwitchInfo(void)
{
    SCI_MEMSET(&s_num_swith_info, 0, sizeof(MMIMTV_NUM_SWITCH_INFO_T));
}

/*****************************************************************************/
// 	Description : OSD_SHORTCUT_ICON 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleShortcutIconSceneMsg(
                                      MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                      DPARAM                param   //the param of the message
                                      )
{
    GUI_POINT_T point = {0};
    GUI_RECT_T  brightness_rect = {0};
    GUI_RECT_T  volume_rect = {0};
    //uint32      service_index = 0;
    //MMIMTV_SAVE_SERVICE_INFO_T* service_node_ptr = PNULL;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleShortcutIconSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4248_112_2_18_2_40_29_776,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        if(PNULL == param)
        { 
            MMIMTV_OpenShortcutIconScene(0);
        }
        else
        {
            MMIMTV_OpenShortcutIconScene((uint32)param);
        }
        break;
        
    case MMIMTV_APP_OK:
        //enter OSD_MAIN_MENU
        MMIMTV_CloseShortcutIconScene();
        MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_WEB:
        //执行当前图标功能
        MMIMTV_GetIconHandle()->IconHandleKeyDown(OK_KEY);
        MMIMTV_GetIconHandle()->IconDisplay(); 
        MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
        break;
        
    case MMIMTV_APP_UP:
        //向上切换焦点
        MMIMTV_ClearFileSavedTip(); //切换焦点前确保此区域已清空
        MMIMTV_GetIconHandle()->IconHandleKeyDown(UP_KEY);
        MMIMTV_GetIconHandle()->IconDisplay(); 
        MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
        break;
        
    case MMIMTV_APP_DOWN:
        //向下切换焦点
        MMIMTV_ClearFileSavedTip(); //切换焦点前确保此区域已清空
        MMIMTV_GetIconHandle()->IconHandleKeyDown(DOWN_KEY);
        MMIMTV_GetIconHandle()->IconDisplay();
        MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
        break;
        
    case MMIMTV_APP_LEFT:
        //亮度调节，仅当亮度调节面板打开时
        MMIMTV_GetProgressRect(&brightness_rect);
        if(MMIMTV_BRIGHTNESS_LEVEL_0 < MMIMTV_GetBrightness() && 
            brightness_rect.left != brightness_rect.right)
        {
            MMIMTV_SetBrightness(MMIMTV_GetBrightness() - 1);
            MMIMTV_GetIconHandle()->IconFocus(MMIMTV_ICON_BRIGHTNESS);
            MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
        }
        break;
        
    case MMIMTV_APP_RIGHT:
        //亮度调节，仅当亮度调节面板打开时
        MMIMTV_GetProgressRect(&brightness_rect);
        if(MMIMTV_BRIGHTNESS_LEVEL_7 > MMIMTV_GetBrightness() && 
            brightness_rect.left != brightness_rect.right)
        {
            MMIMTV_SetBrightness(MMIMTV_GetBrightness() + 1);
            MMIMTV_GetIconHandle()->IconFocus(MMIMTV_ICON_BRIGHTNESS);
            MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
        }
        break;
        
    case MMIMTV_APP_UPSIDE:
        break;
        
    case MMIMTV_APP_DOWNSIDE:
        break;

    case MMIMTV_APP_CAMERA:
#ifndef MBBMS_SUPPORT
        //录制节目
        MMIMTV_StartRecord();
#endif
        break;
        
    case MMIMTV_APP_TP_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        MMIMTV_GetProgressRect(&brightness_rect);
        MMIMTV_GetVolumeProgressRect(&volume_rect);
        
        if(MMIMTV_GetIconHandle()->IconHandleTpDown(point.x, point.y))
        {            
            MMIMTV_GetIconHandle()->IconDisplay();
            MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
        }
        //对比度调节
        else if(GUI_PointIsInRect(point, brightness_rect))
        {
            if(point.x <= brightness_rect.right && point.x > (brightness_rect.right - MMIMTV_INCREASE_ICON_WIDTH))
            {
                HandleShortcutIconSceneMsg(MMIMTV_APP_RIGHT, PNULL);
            }
            else if(point.x <= (brightness_rect.left + MMIMTV_INCREASE_ICON_WIDTH) && point.x > brightness_rect.left)
            {
                HandleShortcutIconSceneMsg(MMIMTV_APP_LEFT, PNULL);
            }
        }
        //音量调节
        else if(GUI_PointIsInRect(point, volume_rect))
        {
            if(point.y >= volume_rect.top && point.y < (volume_rect.top + MMIMTV_INCREASE_ICON_WIDTH))
            {
                MMIMTV_TurnUpVolume();
            }
            else if(point.y < volume_rect.bottom && point.y >= (volume_rect.bottom - MMIMTV_INCREASE_ICON_WIDTH))
            {
                MMIMTV_TurnDownVolume();
            }
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        //enter OSD_IDLE
        MMIMTV_CloseShortcutIconScene();
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseShortcutIconScene();
        break;
        
    default:
        break;
    }
    
}

/*****************************************************************************/
// 	Description : OSD_MAIN_MENU 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleMainMenuSceneMsg(
                                  MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                  DPARAM                param   //the param of the message
                                  )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    //ESG_ERROR_E         ret = ESG_SUCCESS;
    //MMIMTV_SERVICE_T    proglist_ptr = {0};
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleMainMenuSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4397_112_2_18_2_40_30_777,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        if(PNULL == param)
        {
#ifndef MBBMS_SUPPORT
            MMIMTV_OpenMainMenuScene(MMIMTV_NET_LIST);
#else
			MMIMTV_OpenMainMenuScene(MMIMTV_ADD_FAVORITE);
#endif
        }
        else
        {
            MMIMTV_OpenMainMenuScene((uint32)param);
        }
        s_osd_restore_param = 0;
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //get current menu id
        GUIMENU_GetId(MTV_MENU_CTRL_ID, &group_id, &menu_id);
        switch(menu_id)
        {
        case MTV_ID_NET_LIST_MENU:
            MMIMTV_CloseMainMenuScene();
            if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
            {
                //当前正在录制，提示无法进入
                MMIMTV_SetPreOSDScene(OSD_MAIN_MENU);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_TIP_RECORDING);
            }
            else
            {
                MMIMTV_SetFocusOSDScene(OSD_NET_LIST);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            }
            break;
            
        case MTV_ID_SEARCH_MENU:
            MMIMTV_CloseMainMenuScene();
            if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
            {
                //当前正在录制，提示无法进入
                MMIMTV_SetPreOSDScene(OSD_MAIN_MENU);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_TIP_RECORDING);
            }
            else
            {
                MMIMTV_SetFocusOSDScene(OSD_SEARCH_MENU);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            }
            break;
            
        case MTV_ID_RECORDED_PROGRAM_LIST_MENU:
            MMIMTV_CloseMainMenuScene();
            if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
            {
                //当前正在录制，提示无法进入
                MMIMTV_SetPreOSDScene(OSD_MAIN_MENU);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_TIP_RECORDING);
            }
            else
            {
                //打开录制节目列表, 直接打开资源管理器!
                MMIMTV_OpenRecordListScene();
            }
            break;
            
        case MTV_ID_EMERGENCY_BROADCASTING_MENU:
            MMIMTV_CloseMainMenuScene();
#ifndef MBBMS_SUPPORT
            MMIMTV_SetFocusOSDScene(OSD_EB_LIST);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
#else
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            
            //mbbms style eb list
            MMIMBBMS_OpenEBListWin();
#endif
            break;
            
        case MTV_ID_BOOKING_RECORD_MENU:
            MMIMTV_CloseMainMenuScene();
            MMIMTV_SetFocusOSDScene(OSD_BOOKING_RECORD);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;
            
        case MTV_ID_OTHER_FUNCTIONS_MENU:
            MMIMTV_CloseMainMenuScene();
            MMIMTV_SetFocusOSDScene(OSD_TO_OTHER_APP);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;
            
        case MTV_ID_SETTING_MENU:
            MMIMTV_CloseMainMenuScene();
            MMIMTV_SetFocusOSDScene(OSD_SETTING);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;
            
        case MTV_ID_HELP_MENU:
            MMIMTV_CloseMainMenuScene();
            MMIMTV_SetFocusOSDScene(OSD_HELP);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;
            
        case MTV_ID_EXIT_MENU:
            MMIMTV_CloseMainMenuScene();
            if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
            {
                //当前正在录制，提示无法进入
                MMIMTV_SetPreOSDScene(OSD_MAIN_MENU);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_TIP_RECORDING);
            }
            else
            {
                MMIMTV_SetFocusOSDScene(OSD_QUIT_QUERY);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            }
            break;
			
#ifdef MBBMS_SUPPORT
        case MTV_ID_MBBMS_ADD_FAVORITE_MENU:
            MMIMTV_CloseMainMenuScene();
            if(MMIMBBMS_IsCurPlayServiceInFavoriteList())
            {
                //取消收藏
                MMIMBBMS_DelCurPlayServiceFromFavoriteList();
                ShowFavoriteOptTipMsg(MMIMBBMS_OPER_DEL);
                
                MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
            }
            else
            {
                //添加收藏
                MMIMBBMS_AddCurPlayServiceIntoFavoriteList();
                ShowFavoriteOptTipMsg(MMIMBBMS_OPER_ADD);
                
                MMK_PostMsg(MMIMBBMS_ALL_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
                MMK_PostMsg(MMIMBBMS_FAVORITE_SERVICE_CHILD_WIN_ID, MSG_APP_MBBMS_REFRESH_LIST, NULL, NULL);
            }
            break;
			
		case MTV_ID_MBBMS_MAIN_INTERFACE_MENU:
            MMIMTV_CloseMainMenuScene();
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            
            //停止播放
            MMIMBBMS_StopPlay();
            MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            
            //MBBMS 主界面
            if(MMK_IsOpenWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID))
            {
                MMK_WinGrabFocus(MMIMBBMS_MAIN_INTERFACE_WIN_ID);
            }
            else
            {
                MMIMBBMS_OpenMainInterfaceWin();
            }
            break;
#endif
            
        default:
            //SCI_TRACE_LOW:"[MMIMTV]: HandleMainMenuSceneMsg:menu_id 0x%x is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4568_112_2_18_2_40_30_778,(uint8*)"d", msg_id);
            break;
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseMainMenuScene();
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseMainMenuScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_QUIT_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleQuitQuerySceneMsg(
                                   MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                   DPARAM                param   //the param of the message
                                   )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleQuitQuerySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4597_112_2_18_2_40_30_779,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonQueryScene(TXT_MTV_EXIT_MTV_QUERY);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
		//关闭MBBMS模块		
		MMK_CloseWin(MMIMBBMS_MAIN_INTERFACE_WIN_ID);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_NET_LIST 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleNetListSceneMsg(
                                 MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                 DPARAM                 param   //the param of the message
                                 )
{
    uint16  net_index = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleNetListSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4636_112_2_18_2_40_30_780,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenNetListScene(NULL);
        break;
        
    case MMIMTV_APP_OK:
        //get current net node
        net_index = GUILIST_GetCurItemIndex(MTV_NET_LIST_CTRL_ID);
        s_select_net_node_ptr = MMIMTV_GetNetNodeViaIndex(net_index);
        
        MMIMTV_CloseNetListScene();
        if(PNULL != s_select_net_node_ptr)
        {
            //open net list opt
            MMIMTV_SetFocusOSDScene(OSD_NET_LIST_OPT);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        else
        {
            //launch auto search
            StartAutoSearch();
        }
        break;
        
    case MMIMTV_APP_WEB:
        //get current net node
        net_index = GUILIST_GetCurItemIndex(MTV_NET_LIST_CTRL_ID);
        s_select_net_node_ptr = MMIMTV_GetNetNodeViaIndex(net_index);
        
        //open service list
        if(PNULL != s_select_net_node_ptr)
        {
            MMIMTV_CloseNetListScene();
            MMIMTV_SetFocusOSDScene(OSD_SERVICE_LIST);
            //open service list via s_select_net_node_ptr
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        else
        {
            //launch auto search
            StartAutoSearch();
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseNetListScene();
        MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
#ifndef MBBMS_SUPPORT
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_NET_LIST);
#else
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_ADD_FAVORITE);
#endif
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseNetListScene();
        break;
        
    default:
        break;
    }
    
}

/*****************************************************************************/
// 	Description : OSD_NET_LIST_OPT 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleNetListOptSceneMsg(
                                    MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                    DPARAM                  param   //the param of the message
                                    )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleNetListOptSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4714_112_2_18_2_40_30_781,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenNetListOptScene(NULL);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //get current menu id
        GUIMENU_GetId(MTV_NET_OPTION_MENU_CTRL_ID, &group_id, &menu_id);
        switch(menu_id)
        {
        case MTV_ID_UPDATE_NET:
            //launch auto search
            StartAutoSearch();
            break;
            
        case MTV_ID_DELETE_NET:
            //open delete query scene
            if(PNULL != s_select_net_node_ptr)
            {
                MMIMTV_CloseNetListOptScene();
                MMIMTV_SetFocusOSDScene(OSD_DEL_NET_QUERY);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            }
            break;
            
        default:
            break;
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseNetListOptScene();
        MMIMTV_SetFocusOSDScene(OSD_NET_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseNetListOptScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_DEL_NET_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleDelNetQuerySceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                     DPARAM                 param   //the param of the message
                                     )
{
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleDelNetQuerySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4772_112_2_18_2_40_31_782,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonQueryScene(TXT_MTV_DLETE_NET);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //delete select net node
        MMIMTV_DeleteNetNode(s_select_net_node_ptr);
        s_select_net_node_ptr = PNULL;
        //update NS file
        MMIMTV_WriteNSFile();
        
        //return to net list scene
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_NET_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_NET_LIST_OPT);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SERVICE_LIST 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleServiceListSceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                     DPARAM                 param   //the param of the message
                                     )
{
    uint16 service_index = 0;
    BOOLEAN is_need_play = TRUE;
    GUI_POINT_T         point = {0};
    GUI_RECT_T left_rect = {0};
    GUI_RECT_T right_rect = {0};
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleServiceListSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4823_112_2_18_2_40_31_783,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenServiceListScene((uint32)s_select_net_node_ptr);
        break;
        
    case MMIMTV_APP_OK:
        //get selected service node
        service_index             = GUILIST_GetCurItemIndex(MTV_SERVICE_LIST_CTRL_ID);
        s_select_service_node_ptr = MMIMTV_GetServiceNodeViaIndex(s_select_net_node_ptr, service_index);
        
        if(PNULL != s_select_service_node_ptr)
        {
            //open service list opt scene
            MMIMTV_CloseServiceListScene();
            MMIMTV_SetFocusOSDScene(OSD_SERVICE_LIST_OPT);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        else
        {
            s_search_mode.search_type = MMIMTV_MANUAL_SEARCH;
            s_search_mode.center_freq = s_select_net_node_ptr->center_freqquency;
            
            //launch frequency search
            StartFrequencySearch();
        }
        
        break;
        
    case MMIMTV_APP_WEB:
        //get selected service node
        service_index             = GUILIST_GetCurItemIndex(MTV_SERVICE_LIST_CTRL_ID);
        s_select_service_node_ptr = MMIMTV_GetServiceNodeViaIndex(s_select_net_node_ptr, service_index);
        
        if(PNULL != s_select_service_node_ptr)
        {
            //close service list, open OSD_IDLE
            MMIMTV_CloseServiceListScene();
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            
            MMIMTV_SetPlayingNetNode(s_select_net_node_ptr);
            MMIMTV_SetPlayingServiceNode(s_select_service_node_ptr);
            
            //update s_mtv_current_para
            if(s_select_service_node_ptr->net_id == s_mtv_current_para.net_eid 
                && s_select_service_node_ptr->service_id == s_mtv_current_para.service_id
                && MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status)
            {
                is_need_play = FALSE;
            }
            else
            {
                s_mtv_current_para.net_eid      = s_select_service_node_ptr->net_id;
                s_mtv_current_para.service_id   = s_select_service_node_ptr->service_id;
                s_mtv_current_para.service_type = s_select_service_node_ptr->service_type;
            }
            
            //play
            if(is_need_play)
            {
                //switch channel
                OpenPlayConnectingScene();
                SwitchServicePlay();
            }
            
            //display service number
            //MMIMTV_DisplayServiceNumber();
        }
        else
        {
            s_search_mode.search_type = MMIMTV_MANUAL_SEARCH;
            s_search_mode.center_freq = s_select_net_node_ptr->center_freqquency;
            
            //launch frequency search
            StartFrequencySearch();
        }
        break;
        
    case MMIMTV_APP_LEFT:
        //change net
        break;
        
    case MMIMTV_APP_RIGHT:
        //change net
        break;
        
    case MMIMTV_APP_TP_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        MMIMTV_GetChangeNetArrowRect(&left_rect, &right_rect);
        if(GUI_PointIsInRect(point, left_rect))
        {
            HandleServiceListSceneMsg(MMIMTV_APP_LEFT, PNULL);
        }
        else if(GUI_PointIsInRect(point, right_rect))
        {
            HandleServiceListSceneMsg(MMIMTV_APP_RIGHT, PNULL);
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseServiceListScene();
        MMIMTV_SetFocusOSDScene(OSD_NET_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseServiceListScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SERVICE_LIST_OPT 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleServiceListOptSceneMsg(
                                        MMIMTV_KEY_MAP_TYPE_E msg_id,   //the identify of message
                                        DPARAM                param     //the param of the message
                                        )
{  
    //int32               comp_result = 0;
    //ESG_ERROR_E         esg_result = 0;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    BOOLEAN             is_need_play = TRUE;
    //SCI_DATE_T          current_date = {0};
    //MMIMTV_SERVICE_T    proglist = {0};
    //MMIMTV_ESG_LIST_SCENE_T esglist_param = {0};
    
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleServiceListOptSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_4959_112_2_18_2_40_31_784,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenServiceListOptScene(0);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //get current menu id
        GUIMENU_GetId(MTV_SERVICE_OPTION_MENU_CTRL_ID, &group_id, &menu_id);
        switch(menu_id)
        {
        case MTV_ID_PLAY_MENU:
            //play select service
            if(PNULL != s_select_service_node_ptr)
            {
                //close service list opt scene, open OSD_IDLE
                MMIMTV_CloseServiceListOptScene();
                MMIMTV_SetFocusOSDScene(OSD_IDLE);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
                
                MMIMTV_SetPlayingNetNode(s_select_net_node_ptr);
                MMIMTV_SetPlayingServiceNode(s_select_service_node_ptr);
   
                //update s_mtv_current_para
                if(s_select_service_node_ptr->net_id == s_mtv_current_para.net_eid 
                    && s_select_service_node_ptr->service_id == s_mtv_current_para.service_id
                    && MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status)
                {
                    is_need_play = FALSE;
                }
                else
                {
                    s_mtv_current_para.net_eid      = s_select_service_node_ptr->net_id;
                    s_mtv_current_para.service_id   = s_select_service_node_ptr->service_id;
                    s_mtv_current_para.service_type = s_select_service_node_ptr->service_type;
                }
                
                //play
                if(is_need_play)
                {
                    //switch channel
                    OpenPlayConnectingScene();
                    SwitchServicePlay();
                }
                
                //display service number
                //MMIMTV_DisplayServiceNumber();
            }
            break;
            
        case MTV_ID_ESG:
            if(PNULL == s_select_service_node_ptr)
            {
                break;
            }
            s_search_mode.esg_netid = s_select_service_node_ptr->net_id;
            s_search_mode.esg_serviceid = s_select_service_node_ptr->service_id;
            
            MMIMTV_CloseServiceListOptScene();
            s_source_osd = OSD_IDLE;
            MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;

        case MTV_ID_UPDATE_SERVICE:
            s_search_mode.search_type = MMIMTV_MANUAL_SEARCH;
            s_search_mode.center_freq = s_select_net_node_ptr->center_freqquency;
            
            //launch frequency search
            StartFrequencySearch();
            break;
            
        case MTV_ID_DELETE_SERVICE:
            //open delete service query scene
            if(PNULL != s_select_service_node_ptr)
            {
                MMIMTV_CloseServiceListOptScene();
                MMIMTV_SetFocusOSDScene(OSD_DEL_SERVICE_QUERY);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            }
            break;
            
        default:
            break;
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseServiceListOptScene();
        MMIMTV_SetFocusOSDScene(OSD_SERVICE_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseServiceListOptScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_DEL_SERVICE_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleDelServiceQuerySceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                         DPARAM                 param   //the param of the message
                                         )
{
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleDelServiceQuerySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5073_112_2_18_2_40_31_785,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonQueryScene(TXT_COMMON_DELETE_CHANNEL);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //delete select service node
        if(MMIMTV_GetPlayingServiceNode() == s_select_service_node_ptr)
        {
            //要删除的频道当前正在播放，则将当前正在播放的节点移到下一个频道
            MMIMTV_SetPlayingServiceNode(MMIMTV_ChangeService(TRUE));
        }
        MMIMTV_DeleteServiceNode(s_select_net_node_ptr, s_select_service_node_ptr);
        s_select_service_node_ptr = PNULL;
        //update NS file
        MMIMTV_WriteNSFile();
        
        //return to service list scene
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_SERVICE_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, s_select_net_node_ptr);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_SERVICE_LIST_OPT);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SEARCH_WAITING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchWaitingSceneMsg(
                                       MMIMTV_KEY_MAP_TYPE_E msg_id,    //the identify of message
                                       DPARAM                param      //the param of the message
                                       )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSearchWaitingSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5123_112_2_18_2_40_31_786,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonWaitScene(TXT_PROCESS_SEARCHING_NETWORK);
        break;
        
    case MMIMTV_APP_CANCEL:
        //SCI_TRACE_LOW:"[MMIMTV]: Now stop search !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5131_112_2_18_2_40_31_787,(uint8*)"");
        CMMB_StopSearchESGData();
        CMMB_StopSearchProgram();
        //need reset control data
        SetNetCtrlTableData();
        
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_SHORTCUT_ICON);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SEARCH_INVALID_FRQ 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchInvalidFrqSceneMsg(
                                          MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                          DPARAM                param   //the param of the message
                                          )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSearchInvalidFrqSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5160_112_2_18_2_40_31_788,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonTipScene(TXT_MTV_FREQUENCY_ERROR);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetPreOSDScene(OSD_IDLE);
        MMIMTV_SetFocusOSDScene(OSD_MANUAL_SEARCH);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SEARCH_MENU 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchMenuSceneMsg(
                                    MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                    DPARAM                  param   //the param of the message
                                    )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSearchMenuSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5195_112_2_18_2_40_32_789,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenSearchMenuScene(NULL);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //get current menu id
        GUIMENU_GetId(MTV_SEARCH_CTRL_ID, &group_id, &menu_id);
        switch(menu_id)
        {
        case MTV_ID_AUTO_SEARCH_MENU:
            //launch auto search
            StartAutoSearch();
            break;
            
        case MTV_ID_MANUAL_SEARCH_MENU:
            MMIMTV_CloseSearchMenuScene();
            MMIMTV_SetFocusOSDScene(OSD_MANUAL_SEARCH);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;
            
            //case MTV_ID_CITY_SEARCH_MENU:
            //  打开按城市搜索场景
            //  break;
            
        default:
            //SCI_TRACE_LOW:"[MMIMTV]: HandleSearchMenuSceneMsg:menu_id %d is error!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5224_112_2_18_2_40_32_790,(uint8*)"d", menu_id);
            break;
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseSearchMenuScene();
        MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
#ifndef MBBMS_SUPPORT
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_SEARCH);
#else
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_MAIN_INTERFACE);
#endif  
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseSearchMenuScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_MANUAL_SEARCH 手动搜索场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleManualSearchSceneMsg(
                                      MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                      DPARAM                param   //the param of the message
                                      )
{
    MMI_STRING_T text_str = {0};
    //uint32       frequency_value = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleManualSearchSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5260_112_2_18_2_40_32_791,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenManualSearchInputScene(NULL);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        s_search_mode.search_type = MMIMTV_MANUAL_SEARCH;
        
        //get frequency value
        GUIEDIT_GetString(MTV_EDIT_MANUAL_SEARCH_TEXT_CTRL_ID, &text_str);
        MMIAPICOM_WstrToNum(text_str.wstr_ptr, text_str.wstr_len, &s_search_mode.center_freq);
        //SCI_TRACE_LOW:"[MMIMTV]: HandleManualSearchSceneMsg, s_search_mode.center_freq = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5274_112_2_18_2_40_32_792,(uint8*)"d", s_search_mode.center_freq);
        
        //launch frequency search
        StartFrequencySearch();
        break;

    case MMIMTV_CTL_EDITBOX_UPDATE_STRNUM:
        //更新场景softkey
        MMIMTV_UpdateManualSearchInputScene();
        break;
        
    case MMIMTV_APP_CANCEL:
        //返回上级菜单
        MMIMTV_CloseManualSearchInputScene();
        MMIMTV_SetFocusOSDScene(OSD_SEARCH_MENU);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseManualSearchInputScene();
        break;
        
    default:
        break;
    }
}



/*****************************************************************************/
// 	Description : OSD_EB_LIST 紧急广播列表场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleEBListSceneMsg(
                                MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                DPARAM                  param   //the param of the message
                                )
{
    uint32 index = 0;

    //SCI_TRACE_LOW:"[MMIMTV]: HandleEBListSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5314_112_2_18_2_40_32_793,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        if(PNULL != param)
        {
            index = *((uint32*)param);
        }
        MMIMTV_OpenEBListScene(index);
        break;

	case MMIMTV_CTL_LISTBOX_INDEX_MOVED:
		//更新Title上当前位置信息
		if(MMIMTV_GetEBListNumber() > 0)
		{
			index = GUILIST_GetCurItemIndex(MTV_ID_EB_LIST_CTRL_ID);
			MMIMTV_DisplayEBCountInfo(index + 1, MMIMTV_GetEBListNumber());
		}
		break;
        
    case MMIMTV_APP_WEB:
        if(MMIMTV_GetEBListNumber() > 0)
        {
            index = GUILIST_GetCurItemIndex(MTV_ID_EB_LIST_CTRL_ID);
            MMIMTV_CloseEBListScene();
            //记录来源场景用于返回
            s_source_osd = OSD_EB_LIST;
            MMIMTV_SetFocusOSDScene(OSD_EB_DETAIL);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, &index);
        }
        break;
        
    case MMIMTV_APP_OK:
        if(MMIMTV_GetEBListNumber() > 0)
        {
            index = GUILIST_GetCurItemIndex(MTV_ID_EB_LIST_CTRL_ID);
            MMIMTV_CloseEBListScene();
            MMIMTV_SetFocusOSDScene(OSD_EB_LIST_OPT);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, &index);
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseEBListScene();
        MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_EB_LIST);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseEBListScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_EB_LIST 紧急广播列表场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleEBListOptSceneMsg(
                                MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                DPARAM                  param   //the param of the message
                                )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    static uint32 index = 0;

    //SCI_TRACE_LOW:"[MMIMTV]: HandleEBListOptSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5384_112_2_18_2_40_32_794,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        if(PNULL != param)
        {
            index = *((uint32*)param);
        }
        else
        {
            index = 0;
        }
        MMIMTV_OpenEBListOptScene(NULL);
        break;
        
    case MMIMTV_APP_WEB:        
    case MMIMTV_APP_OK:
        GUIMENU_GetId(MTV_ID_EB_OPTION_CTRL_MENU, &group_id, &menu_id);
        switch(menu_id) 
        {
        case MTV_ID_EB_OPTION_SORT_BY_TIME:
            MMIMTV_EBSortByTime();
            MMIMTV_CloseEBListOptScene();
            MMIMTV_SetFocusOSDScene(OSD_EB_LIST);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;
            
        case MTV_ID_EB_OPTION_SORT_BY_GRADE:
            MMIMTV_EBSortByGrade();
            MMIMTV_CloseEBListOptScene();
            MMIMTV_SetFocusOSDScene(OSD_EB_LIST);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;

        case MTV_ID_EB_OPTION_DELETE:
            MMIMTV_CloseEBListOptScene();
            MMIMTV_SetFocusOSDScene(OSD_DEL_EB_QUERY);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, &index);
            break;    

        case MTV_ID_EB_OPTION_DELETE_ALL:
            MMIMTV_CloseEBListOptScene();
            MMIMTV_SetFocusOSDScene(OSD_DEL_ALL_EB_QUERY);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            break;
            
        default:
            break;
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseEBListOptScene();
        MMIMTV_SetFocusOSDScene(OSD_EB_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, &index);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseEBListOptScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_DEL_NET_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleDelEBQuerySceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                     DPARAM                 param   //the param of the message
                                     )
{
    static uint32 index = 0;
    //uint16 eb_num = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleDelEBQuerySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5462_112_2_18_2_40_32_795,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        if(PNULL != param)
        {
            index = *((uint32*)param);
        }
        else
        {
            index = 0;
        }
        MMIMTV_OpenCommonQueryScene(TXT_DELETE_QUERY);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        MMIMTV_DeleteEBNodeViaIndex(index);
        if(!MMIMTV_IsHaveTobeReadEBMsg())
        {
            //清除EB提示图标
            MAIN_SetMsgState(TRUE);
        }
        MMIMTV_WriteEBFile();
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_EB_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_EB_LIST_OPT);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_DEL_NET_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleDelAllEBQuerySceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                     DPARAM                 param   //the param of the message
                                     )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleDelAllEBQuerySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5515_112_2_18_2_40_32_796,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonQueryScene(TXT_DELETE_ALL_QUERY);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        MMIMTV_ClearEBList();
        MAIN_SetMsgState(TRUE);
        MMIMTV_WriteEBFile();
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_EB_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_EB_LIST_OPT);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_EB_DETAIL 紧急广播详情场景
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleEBDetailSceneMsg(
                                MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                DPARAM                  param   //the param of the message
                                )
{
    static uint32 index = 0;
    GUI_POINT_T   point = {0};
    GUI_RECT_T    eb_left_arrow = {MMIMTV_LEFT_ARROW_LEFT, MMIMTV_LEFT_ARROW_TOP, MMIMTV_LEFT_ARROW_RIGHT, MMIMTV_LEFT_ARROW_BOTTOM};
    GUI_RECT_T    eb_right_arrow = {MMIMTV_RIGHT_ARROW_LEFT,MMIMTV_RIGHT_ARROW_TOP,MMIMTV_RIGHT_ARROW_RIGHT,MMIMTV_RIGHT_ARROW_BOTTOM};

    //SCI_TRACE_LOW:"[MMIMTV]: HandleEBDetailSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5561_112_2_18_2_40_32_797,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        if(PNULL != param)
        {
            index = *((uint32*)param);
            //SCI_TRACE_LOW:"[MMIMTV]: HandleEBDetailSceneMsg, index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5568_112_2_18_2_40_32_798,(uint8*)"d", index);
        }
        else
        {
            index = 0;
        }
        MMIMTV_OpenEBListContentScene(index);
        if(!MMIMTV_IsHaveTobeReadEBMsg())
        {
            //清除EB提示图标
            MAIN_SetMsgState(TRUE);
        }
        MMIMTV_WriteEBFile();
        break;
                
    case MMIMTV_APP_CANCEL:
        //MBBMS 1,2级紧急广播要停止当前播放, 并在返回后重新发起播放
        
#ifdef MBBMS_SUPPORT
        if(OSD_EB_LIST != s_source_osd &&( MMIMTV_GetEbStopPlayFlag() == TRUE))
        {
            //播放加载界面
            MMIMTV_CloseEBListContentScene();
            MMIMTV_SetFocusOSDScene(OSD_PLAY_CONNECTING);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            
            MMIMBBMS_StartPlay();
            break;
        }
#endif

        MMIMTV_CloseEBListContentScene();
        //返回来源场景
        //SCI_TRACE_LOW:"[MMIMTV]: HandleEBDetailSceneMsg, s_source_osd = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5601_112_2_18_2_40_32_799,(uint8*)"d", s_source_osd);
        if(OSD_EB_LIST == s_source_osd)
        {
        	MMIMTV_SetFocusOSDScene(OSD_EB_LIST);
        	MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, &index);
        }
        else
        {
            if(OSD_PUB == s_source_osd
                || OSD_PLAY_CONNECTING == s_source_osd
                || OSD_EB_DETAIL == s_source_osd
                || OSD_READ_EB_QUERY == s_source_osd
                )
            {
                s_source_osd = OSD_IDLE;
            }
        	MMIMTV_SetFocusOSDScene(s_source_osd);
        	MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        }
        break;
    
    case MMIMTV_APP_LEFT:   //切到前一条
        if(index > 0)
        {
            index --;
        }
        else
        {
            index = MMIMTV_GetEBListNumber() - 1;
        }
        MMIMTV_CloseEBListContentScene();
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, &index);
        break;

    case MMIMTV_APP_RIGHT:  //切到后一条
        if(index < MMIMTV_GetEBListNumber() - 1)
        {
            index ++;
        }
        else
        {
            index = 0;
        }
        MMIMTV_CloseEBListContentScene();
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, &index);
        break;

    case MMIMTV_APP_TP_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        if(GUI_PointIsInRect(point, eb_left_arrow))
        {
            //切到前一条
            HandleEBDetailSceneMsg(MMIMTV_APP_LEFT, PNULL);
        }
        else if(GUI_PointIsInRect(point, eb_right_arrow))
        {
            //切到后一条
            HandleEBDetailSceneMsg(MMIMTV_APP_RIGHT, PNULL);
        }
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseEBListContentScene();
        MMIMTV_SetOSDRestoreParam(index);
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_BOOKING_RECORD 预约录制场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleBookingRecordSceneMsg(
                                       MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                       DPARAM                   param   //the param of the message
                                       )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleBookingRecordSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5682_112_2_18_2_40_33_800,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenBookRecordScene(NULL);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //到相应项编辑
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseBookRecordScene();
        MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_BOOKING_RECORD);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseBookRecordScene();
        break;
        
    default:
        break;
    }
}


/*****************************************************************************/
// 	Description : OSD_ESG_LIST 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleESGListSceneMsg(
                                 MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                 DPARAM                   param   //the param of the message
                                 )
{
    //uint32      stimi_ret = 0;
    ESG_ERROR_E esg_result = 0;
    //int32       comp_result = 0;
    SCI_DATE_T  current_date = {0};
    SCI_DATE_T  move_date = {0};
    //ESG_ERROR_E esg_parse_return = ESG_SUCCESS;
    //static MMIMTV_ESG_LIST_SCENE_T esglist_param = {0};
    //SCI_TRACE_LOW:"[MMIMTV]: HandleESGListSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5726_112_2_18_2_40_33_801,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        //SCI_TRACE_LOW:"[MMIMTV]: HandleESGListSceneMsg MMIMTV_OPEN_SCENE  parse_status = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5730_112_2_18_2_40_33_802,(uint8*)"d", s_mtv_current_para.esg_parse_status);
        if(MMIMTV_ESG_PARSE_PARSED == s_mtv_current_para.esg_parse_status)
        {
            //设置ESG路径
            MMIMTV_SetESGPath();
            TM_GetSysDate(&current_date);  
            s_esglist_param.net_id = s_search_mode.esg_netid;
            s_esglist_param.service_id = s_search_mode.esg_serviceid;
            
            esg_result=CMMB_GetEsgDateRange((uint16)s_esglist_param.net_id,
                                            (uint16)s_esglist_param.service_id,
                                            (MTV_DATE_T*)&s_esglist_param.min_date,
                                            (MTV_DATE_T*)&s_esglist_param.max_date);
            if(ESG_SUCCESS == esg_result)
            {
                s_esglist_param.is_exist = TRUE;
                if(1 == MMIMTV_DateCompare(s_esglist_param.min_date, current_date))
                {
                    SCI_MEMCPY(&s_esglist_param.current_date, &s_esglist_param.min_date, sizeof(SCI_DATE_T));
                }
                else if(1 == MMIMTV_DateCompare(current_date, s_esglist_param.max_date))
                {
                    SCI_MEMCPY(&s_esglist_param.current_date, &s_esglist_param.max_date, sizeof(SCI_DATE_T));
                }
                else
                {
                    SCI_MEMCPY(&s_esglist_param.current_date, &current_date, sizeof(SCI_DATE_T));
                }
                AllocAndGetESGInfo(current_date,&s_esglist_param);
                //SCI_TRACE_LOW:"[MMIMTV]: HandleESGListSceneMsg NumOfProg = %d,ProgTotal=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5761_112_2_18_2_40_33_803,(uint8*)"dd",s_esglist_param.proglist.NumOfProg,s_esglist_param.proglist.ProgTotal);
                MMIMTV_OpenESGListScene((uint32)&s_esglist_param);
                CMMB_FreeAllProg(&(s_esglist_param.proglist));
            }
            else
            {
                //SCI_TRACE_LOW:"[MMIMTV]: HandleESGListSceneMsg, CMMB_GetEsgDateRange return: %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5767_112_2_18_2_40_33_804,(uint8*)"d", esg_result);
                s_esglist_param.is_exist = FALSE;
                MMIMTV_OpenESGListScene((uint32)&s_esglist_param);  //空列表
            }
        }
        else
        {
            if(MMIMTV_ESG_PARSE_NONE == s_mtv_current_para.esg_parse_status)
            {
                StartESGParse(s_mtv_current_para.net_eid);
            }

            //提示ESG解析中
            MMIMTV_SetPreOSDScene(s_source_osd);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_ESG_PARSEING);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_ESG_PARSEING);
        }
        break;

    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        if(s_esglist_param.is_exist)
        {
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_SEARCH_ESG_QUERY);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
            
        }
        else if(0 != s_esglist_param.net_id)
        {
			//search esg
			StartESGSearch();
        }
        else
        {
            //提示无法搜索
            MMIMTV_SetPreOSDScene(OSD_ESG_LIST);
            MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
            MMIMTV_SetFocusOSDScene(OSD_PUB);
            MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_SERVICE_NOT_EXIST);
        }
        
        break;

    case MMIMTV_APP_LEFT:
        if(s_esglist_param.is_exist)
        {
            MMIMTV_MovePrevDay(s_esglist_param.current_date, &move_date);
            if(1 != MMIMTV_DateCompare(s_esglist_param.min_date, move_date)
                && 1 != MMIMTV_DateCompare(move_date, s_esglist_param.max_date))
            {
                SCI_MEMCPY(&s_esglist_param.current_date, &move_date, sizeof(SCI_DATE_T));
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
                AllocAndGetESGInfo(s_esglist_param.current_date,&s_esglist_param);
                MMIMTV_OpenESGListScene((uint32)&s_esglist_param);
                CMMB_FreeAllProg(&(s_esglist_param.proglist));
                MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
            }
        }
        break;

    case MMIMTV_APP_RIGHT:
        if(s_esglist_param.is_exist)
        {
            MMIMTV_MoveNextDay(s_esglist_param.current_date, &move_date);
            if(1 != MMIMTV_DateCompare(s_esglist_param.min_date, move_date)
                && 1 != MMIMTV_DateCompare(move_date, s_esglist_param.max_date))
            {
                SCI_MEMCPY(&s_esglist_param.current_date, &move_date, sizeof(SCI_DATE_T));
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
                AllocAndGetESGInfo(s_esglist_param.current_date,&s_esglist_param);
                MMIMTV_OpenESGListScene((uint32)&s_esglist_param);
                CMMB_FreeAllProg(&(s_esglist_param.proglist));
                MMK_SendMsg(MMIMTV_MAIN_WIN_ID, MSG_FULL_PAINT, 0);
            }
        }
        break;

    case MMIMTV_APP_CANCEL:
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        if(OSD_SERVICE_LIST_OPT == s_source_osd)
        {
            MMIMTV_SetFocusOSDScene(OSD_SERVICE_LIST_OPT);
        }
        else
        {
            MMIMTV_SetFocusOSDScene(OSD_IDLE);
        }
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;

    case MMIMTV_APP_STAR:
        s_esglist_param.service_id = MMIMTV_GetPreServiceID(s_esglist_param.net_id, s_esglist_param.service_id);
        s_search_mode.esg_serviceid = s_esglist_param.service_id;
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;

    case MMIMTV_APP_HASH:
        s_esglist_param.service_id = MMIMTV_GetNextServiceID(s_esglist_param.net_id, s_esglist_param.service_id);
        s_search_mode.esg_serviceid = s_esglist_param.service_id;
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;

    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseESGListScene(0);
        break;

    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SEARCH_ESG_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchESGQuerySceneMsg(
                                        MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                        DPARAM                   param   //the param of the message
                                        )
{
    //uint32 stimi_ret = 0;

    //SCI_TRACE_LOW:"[MMIMTV]: HandleSearchESGQuerySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5897_112_2_18_2_40_33_805,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonQueryScene(TXT_MTV_UPDATE_ESG);
        break;

    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        if(0 != s_search_mode.esg_netid)
        {
			//start search esg
			StartESGSearch();
        }
        break;

    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;

    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;

    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SEARCH_ESG_WAITING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSearchESGWaitingSceneMsg(
                                          MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                          DPARAM                   param   //the param of the message
                                          )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSearchESGWaitingSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5937_112_2_18_2_40_33_806,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenCommonWaitScene(TXT_COMMON_UPDATING);
        break;

    case MMIMTV_APP_CANCEL:
        //SCI_TRACE_LOW:"[MMIMTV]: Now stop search !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5945_112_2_18_2_40_33_807,(uint8*)"");
        CMMB_StopSearchESGData();    
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE,PNULL);
        break;

    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;

    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_PUB 场景处理函数
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
LOCAL void HandlePubSceneMsg(
                                   MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                   DPARAM                   param   //the param of the message
                                   )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandlePubSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5970_112_2_18_2_40_33_808,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        if(PNULL == param)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: HandlePubSceneMsg param is null"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_5976_112_2_18_2_40_33_809,(uint8*)"");
            break;
        }
        MMIMTV_SetOSDRestoreParam((uint32)param);
        MMIMTV_OpenCommonTipScene((uint32)param);
        break;
        
    case MMIMTV_APP_CANCEL:
        //恢复被中断的场景
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(MMIMTV_GetPreOSDScene());
        MMIMTV_SetPreOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_TO_OTHER_APP 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleToOtherAppSceneMsg(
                                    MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                    DPARAM                  param   //the param of the message
                                    )
{
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    static MMIMTV_TO_OTHER_APP_MENU_E cur_item_index = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleToOtherAppSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6013_112_2_18_2_40_33_810,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenOtherFunctionScene((uint32)cur_item_index);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        //到相应项编辑
        GUIMENU_GetId(MTV_ID_OTHER_FUNCTIONS_CTRL_MENU,&group_id,&menu_id);
        switch(menu_id) 
        {
        case MTV_ID_DIAL_MENU:
            cur_item_index = MMIMTV_TO_OTHER_APP_DIAL;
            MMIMTV_CloseOtherFunctionScene();
            MMIAPIIDLE_EnterDialWin();
            break;
            
        case MTV_ID_SMS_MENU:
            cur_item_index = MMIMTV_TO_OTHER_APP_SMS;
            MMIMTV_CloseOtherFunctionScene();
            MMIAPISMS_OpenMessageMainMenuWin();
            break;
            
        case MTV_ID_PHONEBOOK_MENU:
            cur_item_index = MMIMTV_TO_OTHER_APP_PHONEBOOK;
            MMIMTV_CloseOtherFunctionScene();
            MMIAPIPB_OpenListAllGroupWin();
            break;
            
        default:
            break;
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        cur_item_index = MMIMTV_TO_OTHER_APP_DIAL;
        MMIMTV_CloseOtherFunctionScene();
        MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_OTHER_APP);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseOtherFunctionScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingSceneMsg(
                                 MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                 DPARAM                 param   //the param of the message
                                 )
{
    MMI_CTRL_ID_T   ctrl_id = MTV_SETTING_LIST_CTRL_ID;
    MMIMTV_SETTING_LIST_E cur_selection = MMIMTV_SETTING_MAX;
    MMIMTV_SETTING_LIST_E list_id = MMIMTV_SETTING_MAX;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6078_112_2_18_2_40_33_811,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenSettingScene((uint32)param);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseSettingScene();
        MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_SETTING);
        break;

    case MMIMTV_APP_1:
    case MMIMTV_APP_2:
    case MMIMTV_APP_3:
    case MMIMTV_APP_4:
    case MMIMTV_APP_5:
    case MMIMTV_APP_6:
    case MMIMTV_APP_7:
    case MMIMTV_APP_8:
    case MMIMTV_APP_9:
        {
            uint32 num = 0;
            
            num = ConvertMtvDigiMsgToNum(msg_id);
            num --;
            
            if(num< MMIMTV_SETTING_MAX)
            {
                cur_selection = (MMIMTV_SETTING_LIST_E)num;
                GUILIST_SetCurItemIndex(ctrl_id, cur_selection);
                HandleSettingSceneMsg(MMIMTV_APP_WEB,PNULL);
            }
        }
        break;

    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        cur_selection = (MMIMTV_SETTING_LIST_E)GUILIST_GetCurItemIndex(ctrl_id);

        //获取当前list item的ID信息
        GUILIST_GetItemData(ctrl_id, cur_selection, &list_id);
        switch(list_id)
        {
        //目前网络中伴音语言未知，暂屏蔽此项
        //case MMIMTV_AUDIO_LANGUAGE:
        //    MMIMTV_SetFocusOSDScene(OSD_SETTING_AUDIO_LANGUAGE);
        //    break;
        
        case MMIMTV_SUBTITLE_STREAM:
            MMIMTV_SetFocusOSDScene(OSD_SETTING_SUBTITLE);
            break;
        
        case MMIMTV_AUDIO_MODE:
            MMIMTV_SetFocusOSDScene(OSD_SETTING_AUDIO_MODE);
            break;
        
//         case MMIMTV_ZOOM_MODE:
//             MMIMTV_SetFocusOSDScene(OSD_SETTING_ZOOM_MODE);
//             break;
        
            //case MMIMTV_SCREEN_ADJUST:
            //    break;
        
//         case MMIMTV_RECORD_STORE:
//             MMIMTV_SetFocusOSDScene(OSD_SETTING_RECORD_STORE);
//             break;
        
        case MMIMTV_SNAPSHOT_STORE:
            MMIMTV_SetFocusOSDScene(OSD_SETTING_SNAPSHOT_STORE);
            break;
        
        case MMIMTV_OSD_TRANSPARENCY:
            MMIMTV_SetFocusOSDScene(OSD_SETTING_TRANSPARENCY);
            break;
        
        case MMIMTV_OSD_DISPLAY_TIME:
            MMIMTV_SetFocusOSDScene(OSD_SETTING_DISPLAY_TIME);
            break;
        
//         case MMIMTV_WATCH_TIME_TIP:
//             MMIMTV_SetFocusOSDScene(OSD_SETTING_WATCH_TIP);
//             break;
        
//         case MMIMTV_BROADCAST_BG_PIC:
//             MMIMTV_SetFocusOSDScene(OSD_SETTING_BROADCAST_BG_PIC);
//             break;
        
        default:
            break;
        }
        //close OSD_SETTING scene, open detail setting scene
        MMIMTV_CloseSettingScene();
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseSettingScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingAudioLanguageSceneMsg(
                                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                              DPARAM                param   //the param of the message
                                              )
{
    uint32 cur_selection = 0;
    //uint32 audio_lang_count = 0;
    //uint32 *lang_code_ptr = PNULL;
    static MMIMTV_AUDIO_LANGUAGE_SCENE_T   scene_param = {0};
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingAudioLanguageSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6199_112_2_18_2_40_34_812,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        //SCI_TRACE_LOW:"[MMIMTV]:HandleSettingAudioLanguageSceneMsg, play_status=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6204_112_2_18_2_40_34_813,(uint8*)"d", s_mtv_current_para.play_status);
        if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status)
        {
            scene_param.audio_lang_count = CMMB_EnumAudioLanguages(NULL);
            //SCI_TRACE_LOW:"[MMIMTV]:HandleSettingAudioLanguageSceneMsg audio_lang_count=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6208_112_2_18_2_40_34_814,(uint8*)"d",scene_param.audio_lang_count);
            if(scene_param.audio_lang_count > 0)
            {
                scene_param.lang_code_ptr = SCI_ALLOC(scene_param.audio_lang_count * sizeof(uint32));
                if (scene_param.lang_code_ptr != NULL)
                {
                    CMMB_EnumAudioLanguages((KDuint32*)scene_param.lang_code_ptr);
                }
            }
            scene_param.audio_language_code = CMMB_GetAudioLanguage();
            MMIMTV_OpenAudioLanguageScene((uint32)&scene_param);
            if(PNULL != scene_param.lang_code_ptr)
            {
                SCI_FREE(scene_param.lang_code_ptr);
            }
        }
        else
        {
            SCI_MEMSET(&scene_param,0,sizeof(MMIMTV_AUDIO_LANGUAGE_SCENE_T));
            MMIMTV_OpenAudioLanguageScene((uint32)&scene_param);
        }
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseAudioLanguageScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_AUDIO_LANGUAGE);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status)
        {
            cur_selection = GUILIST_GetCurItemIndex(MTV_LANGUAGE_LIST_CTRL_ID);
            
            scene_param.audio_lang_count = CMMB_EnumAudioLanguages(NULL);
            scene_param.lang_code_ptr = SCI_ALLOC(scene_param.audio_lang_count * sizeof(uint32));
            if (PNULL != scene_param.lang_code_ptr)
            {
                CMMB_EnumAudioLanguages((KDuint32*)scene_param.lang_code_ptr);
            }
            if(scene_param.audio_lang_count > cur_selection && PNULL != scene_param.lang_code_ptr)
            {
                CMMB_SetAudioLanguage(scene_param.lang_code_ptr[cur_selection]);
                //    s_mtv_current_para.audio_language = lang_code_ptr[cur_selection];
            }
            if (PNULL != scene_param.lang_code_ptr)
            {
                SCI_FREE(scene_param.lang_code_ptr);
            }  
        }
        else
        {
            //SCI_TRACE_LOW:"HandleSettingAudioLanguageSceneMsg play_status = %d is wrong"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6261_112_2_18_2_40_34_815,(uint8*)"d",s_mtv_current_para.play_status);
        }
        MMIMTV_CloseAudioLanguageScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_AUDIO_LANGUAGE);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseAudioLanguageScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingSubtitleSceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                         DPARAM                 param   //the param of the message
                                         )
{
    uint32 cur_selection = 0;
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingSubtitleSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6287_112_2_18_2_40_34_816,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenSubtitleSettingScene(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseSubtitleSettingScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_SUBTITLE_STREAM);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        cur_selection = GUILIST_GetCurItemIndex(MTV_SUBTITLE_LIST_CTRL_ID);
        switch(cur_selection) 
        {
        case 0:
            MMIMTV_SetSubtitleStreams(TRUE);
        	break;

        case 1:
            MMIMTV_SetSubtitleStreams(FALSE);
            break;
            
        default:
            break;
        }
        MMIMTV_CloseSubtitleSettingScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_SUBTITLE_STREAM);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseSubtitleSettingScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingAudioModeSceneMsg(
                                          MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                          DPARAM                param   //the param of the message
                                          )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingAudioModeSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6340_112_2_18_2_40_34_817,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenAudioModeScene(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseAudioModeScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_AUDIO_MODE);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        MMIMTV_SetAudioMode((MMIMTV_AUDIO_MODE_E)GUILIST_GetCurItemIndex(MTV_AUDIO_MODE_LIST_CTRL_ID));
        MMIMTV_CloseAudioModeScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_AUDIO_MODE);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseAudioModeScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingZoomModeSceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                         DPARAM                 param   //the param of the message
                                         )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingZoomModeSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6380_112_2_18_2_40_34_818,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenZoomModeScene(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseZoomModeScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_ZOOM_MODE);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        MMIMTV_SetZoomMode((MMIMTV_ZOOM_MODE_E)GUILIST_GetCurItemIndex(MTV_ZOOM_MODE_LIST_CTRL_ID));
        MMIMTV_CloseZoomModeScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_ZOOM_MODE);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseZoomModeScene();
        break;
        
    default:
        break;
    }
}



/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingScreenAdjustSceneMsg(
                                             MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                             DPARAM                 param   //the param of the message
                                             )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingScreenAdjustSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6422_112_2_18_2_40_34_819,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        //MMIMTV_Open(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        //MMIMTV_CloseAudioLanguageScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        
        break;
        
    case MMIMTV_CLOSE_SCENE:
        //MMIMTV_CloseAudioLanguageScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingRecordStoreSceneMsg(
                                            MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                            DPARAM                  param   //the param of the message
                                            )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingRecordStoreSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6459_112_2_18_2_40_34_820,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenRecordStoreScene(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseRecordStoreScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_RECORD_STORE);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        MMIMTV_SetRecordStore((MMIFILE_DEVICE_E)GUILIST_GetCurItemIndex(MTV_RECORD_STORE_LIST_CTRL_ID));
        MMIMTV_CloseRecordStoreScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_RECORD_STORE);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseRecordStoreScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingSnapshotStoreSceneMsg(
                                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                              DPARAM                param   //the param of the message
                                              )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingSnapshotStoreSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6499_112_2_18_2_40_34_821,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenSnapShotStoreScene(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseSnapShotStoreScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_SNAPSHOT_STORE);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        MMIMTV_SetSnapshotStore((MMIFILE_DEVICE_E)GUILIST_GetCurItemIndex(MTV_SNAPSHOT_STORE_LIST_CTRL_ID));
        MMIMTV_CloseSnapShotStoreScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_SNAPSHOT_STORE);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseSnapShotStoreScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : update transparency
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void UpdateTransparency(void)
{
    GUI_COLOR_T color_key= MMIMTV_TRANSPARENT_COLOR;
    uint32      alpha = MMIMTV_GetTransparencyValue(MMIMTV_GetTransparency());
    
    GUIBLOCK_SetType(alpha, color_key, GUIBLOCK_TYPE_OSD, (GUI_LCD_DEV_INFO*)MMITHEME_GetDefaultLcdDev());
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingTransparencySceneMsg(
                                             MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
                                             DPARAM                 param   //the param of the message
                                             )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingTransparencySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6551_112_2_18_2_40_34_822,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenTransparencySettingScene(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseTransparencySettingScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_OSD_TRANSPARENCY);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        MMIMTV_SetTransparency((MMIMTV_OSD_TRANSPARENCY_E)GUILIST_GetCurItemIndex(MTV_TRANSPARENCY_LIST_CTRL_ID));
        s_lcdosd_spec.alpha = MMIMTV_GetTransparencyValue(MMIMTV_GetTransparency());
        
        //update
        UpdateTransparency();
        if(MMIMBBMS_IsBgOSDLayerOpen())
        {
            UILAYER_SetLayerAlpha(MMIMBBMS_GetBgOSDLayerDev(), MMIMTV_GetTransparencyValue(MMIMTV_GetTransparency()));	
        }
        
        MMIMTV_CloseTransparencySettingScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_OSD_TRANSPARENCY);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseTransparencySettingScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingDisplayTimeSceneMsg(
                                            MMIMTV_KEY_MAP_TYPE_E   msg_id, //the identify of message
                                            DPARAM                  param   //the param of the message
                                            )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingDisplayTimeSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6600_112_2_18_2_40_34_823,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenDisplayTimeSettingScene(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseDisplayTimeSettingScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_OSD_DISPLAY_TIME);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        MMIMTV_SetOSDDisplayTime((MMIMTV_OSD_DISPLAY_TIME_E)GUILIST_GetCurItemIndex(MTV_OSD_DISPLAY_TIME_LIST_CTRL_ID));
        MMIMTV_CloseDisplayTimeSettingScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_OSD_DISPLAY_TIME);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseDisplayTimeSettingScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
// LOCAL void HandleSettingWatchTipSceneMsg(
//                                          MMIMTV_KEY_MAP_TYPE_E  msg_id, //the identify of message
//                                          DPARAM                 param   //the param of the message
//                                          )
// {
//     SCI_TRACE_LOW("[MMIMTV]: HandleSettingWatchTipSceneMsg, msg_id = 0x%x", msg_id);
//     
//     switch(msg_id)
//     {
//     case MMIMTV_OPEN_SCENE:
//         MMIMTV_OpenWatchTipSettingScene(0);
//         break;
//         
//     case MMIMTV_APP_CANCEL:
//         MMIMTV_CloseWatchTipSettingScene();
//         MMIMTV_SetFocusOSDScene(OSD_SETTING);
//         MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_WATCH_TIME_TIP);
//         break;
//         
//     case MMIMTV_APP_WEB:
//     case MMIMTV_APP_OK:
//         MMIMTV_SetWatchTimeTip((MMIMTV_WATCH_TIME_TIP_E)GUILIST_GetCurItemIndex(MTV_WATCH_TIP_LIST_CTRL_ID));
//         MMIMTV_CloseWatchTipSettingScene();
//         MMIMTV_SetFocusOSDScene(OSD_SETTING);
//         MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_WATCH_TIME_TIP);
//         break;
//         
//     case MMIMTV_CLOSE_SCENE:
//         MMIMTV_CloseWatchTipSettingScene();
//         break;
//         
//     default:
//         break;
//     }
// }

/*****************************************************************************/
// 	Description : OSD_SETTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleSettingBroadcastBGpicSceneMsg(
                                               MMIMTV_KEY_MAP_TYPE_E    msg_id, //the identify of message
                                               DPARAM                   param   //the param of the message
                                               )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleSettingBroadcastBGpicSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6680_112_2_18_2_40_35_824,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenBroadcastBgPicSettingScene(0);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseBroadcastBgPicSettingScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_BROADCAST_BG_PIC);
        break;
        
    case MMIMTV_APP_WEB:
    case MMIMTV_APP_OK:
        MMIMTV_SetBroadcastBG((MMIMTV_BROADCAST_BG_PIC_E)GUILIST_GetCurItemIndex(MTV_BROADCAST_BG_PIC_LIST_CTRL_ID));
        if(s_mtv_current_para.is_playing_broadcast)
        {
            //支持广播播放中，更改背景图片
            MMIMTV_SetBMPtoVChip(IMAGE_MTV_BG1 + MMIMTV_GetBroadcastBG());
        }
        MMIMTV_CloseBroadcastBgPicSettingScene();
        MMIMTV_SetFocusOSDScene(OSD_SETTING);
        //MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_BROADCAST_BG_PIC);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseBroadcastBgPicSettingScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_HELP 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
LOCAL void HandleHelpSceneMsg(
                              MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                              DPARAM                param   //the param of the message
                              )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleHelpSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6725_112_2_18_2_40_35_825,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenHelpScene(NULL);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseHelpScene();
        MMIMTV_SetFocusOSDScene(OSD_MAIN_MENU);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)MMIMTV_HELP);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseHelpScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_RECORD_LOADING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 录像加载等待，主要是播放录像前RESET CMMB需要数秒时间
/*****************************************************************************/
LOCAL void HandleRecordLoadingSceneMsg(
									   MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
									   DPARAM                param   //the param of the message
									   )
{
	//SCI_TRACE_LOW:"[MMIMTV]: HandleRecordLoadingSceneMsg, msg_id = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6756_112_2_18_2_40_35_826,(uint8*)"d", msg_id);
	
	switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
		MMIMTV_OpenCommonWaitWithNoCancelScene(TXT_MTV_RECORD_LOADING);
		MMITHEME_UpdateRect();
        break;
		
	case MMIMTV_CLOSE_SCENE:
		MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
	}
}

/*****************************************************************************/
// 	Description : OSD_RECORD_PLAY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 本地回放场景处理函数
/*****************************************************************************/
LOCAL void HandleRecordPlaySceneMsg(
                                    MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                    DPARAM                param   //the param of the message
                                    )
{
    GUI_POINT_T tp_point = {0};
    GUI_RECT_T  progress_rect ={MMIMTV_RECORD_PLAY_PROGRESS_LEFT, \
                                MMIMTV_RECORD_PLAY_PROGRESS_TOP, \
                                MMIMTV_RECORD_PLAY_PROGRESS_LEFT + MMIMTV_RECORD_PLAY_PROGRESS_WIDTH -1, \
                                MMIMTV_RECORD_PLAY_PROGRESS_TOP + MMIMTV_RECORD_PLAY_PROGRESS_HEITHT};

    //SCI_TRACE_LOW:"[MMIMTV]: HandleRecordPlaySceneMsg, msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6789_112_2_18_2_40_35_827,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenRecordPlayScene(NULL);
        break;
        
    case MMIMTV_APP_WEB:    //播放/暂停
    case MMIMTV_APP_OK:
        if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status)
        {
            //SCI_TRACE_LOW:"HandleRecordPlaySceneMsg now pause play"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6800_112_2_18_2_40_35_828,(uint8*)"");
            //暂停
            CMMB_Pause();
            s_mtv_current_para.play_status = MMIMTV_PLAY_PAUSED;
            //更新场景显示
            MMIMTV_UpdateRecordPlayScene();
        }
        else if(MMIMTV_PLAY_PAUSED == s_mtv_current_para.play_status)
        {
            //SCI_TRACE_LOW:"HandleRecordPlaySceneMsg now resume play"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6809_112_2_18_2_40_35_829,(uint8*)"");
            //恢复
            if(CMMB_Resume())
            {
                //SCI_TRACE_LOW:"[MMIMTV]: Resume error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6813_112_2_18_2_40_35_830,(uint8*)"");
                MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
            }
            else
            {
                s_mtv_current_para.play_status = MMIMTV_PLAY_PLAYING;
                //更新场景显示
                MMIMTV_UpdateRecordPlayScene();
            }
        }
        else if(MMIMTV_PLAY_STOPPED == s_mtv_current_para.play_status)
        {
            s_mtv_record_time_count = 0;
            //重新发起播放
            if(CMMB_PlayStream(s_mtv_current_para.record_file_full_name,
                s_mtv_current_para.audio_language))
            {
                //SCI_TRACE_LOW:"[MMIMTV]: PlayStream error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6830_112_2_18_2_40_35_831,(uint8*)"");
                //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
                MMK_CloseWin(MMIFMM_MAIN_WIN_ID);
                
                //播放录像文件错误，提示
                OpenPlayStreamErrScene();
                
                //恢复直播
                SwitchServicePlay();
            }
            else
            {
                s_mtv_current_para.play_status = MMIMTV_PLAY_WAITING;
            }
        }
        break;
        
    case MMIMTV_APP_TP_DOWN: //拖动播放
        tp_point.x = MMK_GET_TP_X(param);
        tp_point.y = MMK_GET_TP_Y(param);
        if(GUI_PointIsInRect(tp_point, progress_rect))
        {
            //get tp offset 
            s_mtv_current_para.tp_offset = tp_point.x - progress_rect.left;

            //slide play
            if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status || MMIMTV_PLAY_WAITING == s_mtv_current_para.play_status)
            {
                //stop current play
                if(SCI_SUCCESS != CMMB_Stop())
                {
                    //SCI_TRACE_LOW:"[MMIMTV]: Stop error"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6861_112_2_18_2_40_35_832,(uint8*)"");
                    s_mtv_current_para.play_status = MMIMTV_PLAY_NONE;
                    //MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
                }
                else
                {
                    s_mtv_current_para.op_type = MMIMTV_OP_SLIDE;
                    s_mtv_current_para.play_status = MMIMTV_PLAY_STOPPING;
                }
            }
        }
        break;
        
    case MMIMTV_APP_CANCEL: //返回至录制节目列表
        MMIMTV_CloseRecordPlayScene();
        MMIMTV_SetFocusOSDScene(OSD_NONE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        
        //Focus FMM窗口
        MMK_WinGrabFocus(MMIFMM_MAIN_WIN_ID);
        
        //放录像切换到直播也需要RESET Vchip
        s_mtv_current_para.is_need_reset_Vchip = TRUE;
        SwitchServicePlay();
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseRecordPlayScene();
        break;
        
    default:
        break;
    }
}


/*****************************************************************************/
// 	Description : OSD_PLAY_CONNECTING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: MBBMS 播放连接等待
/*****************************************************************************/
LOCAL void HandlePlayConnectingSceneMsg(
                                        MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                        DPARAM                param   //the param of the message
                                        )
{
    //MMI_HANDLE_T  pre_win_handle = 0;

    //SCI_TRACE_LOW:"[MMIMTV]: HandlePlayConnectingSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6908_112_2_18_2_40_35_833,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_ClearScreen();
        MMIMTV_OpenPlayConnectingScene();
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_ClosePlayConnectingScene();
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        
        //停止播放
        MMIMBBMS_StopPlay();
        MMK_CloseWin(MMIMTV_MAIN_WIN_ID);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_ClosePlayConnectingScene();
        break;
        
    default:
        break;
    }
}

#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : OSD_FAVORITE_OPT 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL void HandleFavoriteServiceOptSceneMsg(
                                            MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                            DPARAM                param   //the param of the message
                                            )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleFavoriteServiceOptSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6944_112_2_18_2_40_35_834,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        //MMIMTV_OpenCommonTipScene((uint32)param);
        break;
        
    case MMIMTV_APP_CANCEL:
        //恢复被中断的场景
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_READ_EB_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 低优先级广播察看询问
/*****************************************************************************/
LOCAL void HandleReadEBQuerySceneMsg(
                                     MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                     DPARAM                param   //the param of the message
                                     )
{
    //SCI_TRACE_LOW:"[MMIMTV]: HandleReadEBQuerySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_6976_112_2_18_2_40_35_835,(uint8*)"d", msg_id);
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_OpenNewEBQueryScene();
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        MMIMTV_CloseNewEBQueryScene();
        MMIMTV_SetFocusOSDScene(OSD_EB_DETAIL);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseNewEBQueryScene();
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseNewEBQueryScene();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_SUBSCRIBLE_QUERY 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: MBBMS套餐订购询问
/*****************************************************************************/
LOCAL void HandleSubscribleQuerySceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                         DPARAM                param   //the param of the message
                                         )
{
    MMI_STRING_T    text = {0};
    wchar wchar_strbuf[MMIMBBMS_COMMON_MAX_LENS +1] = {0};

    //SCI_TRACE_LOW:"[MMIMTV]: HandleSubscribleQuerySceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7017_112_2_18_2_40_35_836,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMBBMS_GetSubscriblePromptText(wchar_strbuf);
        text.wstr_ptr = wchar_strbuf;
        text.wstr_len = MMIAPICOM_Wstrlen(wchar_strbuf);
        MMIMTV_OpenCommonQuerySceneByTextPtr(&text);
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        //open subscrible main win
        //MMIMBBMS_OpenSubscribeTypeMainWin();
        MMIMBBMS_SetSubscribeFromType(MMIMBBMS_SUBSCRIBE_FROM_PLAY);
        MMIMBBMS_SetIfEnterSubFromPlay(TRUE);
        MMIMBBMS_OpenAccountInquiryRequestWin();
        break;
        
    case MMIMTV_APP_CANCEL:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_IDLE);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;

    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : OSD_CMMB_MODE_WARNING 场景处理函数
//	Global resource dependence : none
//  Author: xin.li
//	Note: 无SIM卡或无网络提示场景，确认后消失
/*****************************************************************************/
LOCAL void HandleCMMBModeWarningSceneMsg(
                                         MMIMTV_KEY_MAP_TYPE_E msg_id, //the identify of message
                                         DPARAM                param   //the param of the message
                                         )
{
    MMI_TEXT_ID_T   text_id = TXT_MBBMS_NO_SIM;
    uint32          sim_num = 0;
    uint16          sim_sys  = MN_DUAL_SYS_1;
    
    //SCI_TRACE_LOW:"[MMIMTV]: HandleCMMBModeWarningSceneMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7068_112_2_18_2_40_35_837,(uint8*)"d", msg_id);
    
    //获取可用SIM数目
    sim_num = MMIAPIPHONE_GetSimAvailableNum(&sim_sys, 1);
    
    if(0 == sim_num)    //没有可用的SIM卡
    {
        text_id = TXT_MBBMS_NO_SIM;
    }
    else
    {
        text_id = TXT_MBBMS_NO_SIGNAL;
    }
    
    switch(msg_id)
    {
    case MMIMTV_OPEN_SCENE:
        MMIMTV_ClearScreen();
        MMIMTV_OpenCommonConfirmScene(text_id);
        //init ctrl data
        SetNetCtrlTableData();
        break;
        
    case MMIMTV_APP_OK:
    case MMIMTV_APP_WEB:
        MMIMTV_CloseCommonMsgBox();
        MMIMTV_SetFocusOSDScene(OSD_NET_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
        break;
        
    case MMIMTV_APP_CANCEL:
        break;
        
    case MMIMTV_CLOSE_SCENE:
        MMIMTV_CloseCommonMsgBox();
        break;
        
    default:
        break;
    }
}

/*****************************************************************************/
// 	Description : show add/del favorite service result tip
//	Global resource dependence : none
//  Author: xin.li
//	Note: 收藏/取消收藏 当前频道提示
/*****************************************************************************/
LOCAL void ShowFavoriteOptTipMsg(MMIMBBMS_OPER opt_type)
{
    wchar wchar_name[MMIMBBMS_SERVICE_NAME_MAX_LEN +1] = {0};
    wchar wchar_dstbuf[MMIMBBMS_SERVICE_NAME_MAX_LEN*2 +1] = {0};
    MMI_STRING_T prompt_str1 = {0};
    MMI_STRING_T prompt_str2 = {0};
    MMI_STRING_T text_str = {0};
    uint16 curr_len = 0;

    if (MMIMBBMS_OPER_ADD == opt_type)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_HAVE_FAVORITE1, &prompt_str1);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_HAVE_CANCEL_FAVORITE1, &prompt_str1);
    }

    if (MMIMBBMS_OPER_ADD == opt_type)
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_HAVE_FAVORITE2, &prompt_str2);
    }
    else
    {
        MMI_GetLabelTextByLang(TXT_MBBMS_HAVE_CANCEL_FAVORITE2, &prompt_str2);
    }

    //service name
    MMIMBBMS_GetCurrServiceName(wchar_name, MMIMBBMS_SERVICE_NAME_MAX_LEN);

    MMIAPICOM_Wstrncpy(wchar_dstbuf, prompt_str1.wstr_ptr, prompt_str1.wstr_len);
    MMIAPICOM_Wstrcat(wchar_dstbuf, wchar_name);

    curr_len = (uint16)MMIAPICOM_Wstrlen(wchar_dstbuf);
    MMIAPICOM_Wstrncpy(wchar_dstbuf+curr_len, prompt_str2.wstr_ptr, prompt_str2.wstr_len);

    text_str.wstr_ptr = wchar_dstbuf;
    text_str.wstr_len = curr_len + prompt_str2.wstr_len;

    MMIMTV_SetPreOSDScene(OSD_IDLE);
    MMIMTV_SetFocusOSDScene(OSD_FAVORITE_OPT);
    MMIMTV_OpenCommonTipSceneByTextPtr(&text_str);
}

#endif


/*****************************************************************************/
//  Description : Get current audio language
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC MTV_MMI_TEXT_ID_E MMIMTV_GetCurrentAudioLanguage(void)
{
    MTV_MMI_TEXT_ID_E value = TXT_COMMON_LANG_CHINESE;
    uint32 audio_language = 0;
    
    audio_language = CMMB_GetAudioLanguage();
    //SCI_TRACE_LOW:"MMIMTV_GetCurrentAudioLanguage audio_language=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7171_112_2_18_2_40_36_838,(uint8*)"d",audio_language);
    switch(audio_language)
    {
    case 0:
        value = TXT_EMPTY;
        break;
        
    case MMIMTV_CHINESE_CODE:
        value = TXT_COMMON_LANG_CHINESE;
        break;
        
    case MMIMTV_ENGLISH_CODE:
        value = TXT_LANG_ENGLISH;
        break;
        
    default:
        value = (MTV_MMI_TEXT_ID_E)TXT_UNKNOWN;
        break;
    }
    return value;
}

/*****************************************************************************/
//  Description : get current transparency value
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIMTV_GetTransparencyValue(MMIMTV_OSD_TRANSPARENCY_E transparency)
{
    uint8 alpha = 0;
    
    switch(transparency)
    {
    case MMIMTV_OSD_TRANSPARENCY_15:
        alpha = MMIMTV_TRANSPARENT_COLOR_15;
        break;
        
    case MMIMTV_OSD_TRANSPARENCY_25:
        alpha = MMIMTV_TRANSPARENT_COLOR_25;
        break;
        
    case MMIMTV_OSD_TRANSPARENCY_50:
        alpha = MMIMTV_TRANSPARENT_COLOR_50;
        break;
        
    case MMIMTV_OSD_TRANSPARENCY_75:
        alpha = MMIMTV_TRANSPARENT_COLOR_75;
        break;
        
    default:
        alpha = MMIMTV_TRANSPARENT_COLOR_50;
        break;
    }
    return alpha;
}

/*****************************************************************************/
// 	Description : whether current osd scene contain editbox
//	Global resource dependence : none
//  Author: 
//	Note: 当前的OSD场景是否含有编辑框
//        注意：含有编辑框的场景比较特殊，在TV窗口失去焦点时也不能关闭
/*****************************************************************************/
LOCAL BOOLEAN IsFocusOsdWithEditbox(void)
{
    if((OSD_MANUAL_SEARCH == MMIMTV_GetFocusOSDScene()) 
        ||(OSD_BOOKING_NAME == MMIMTV_GetFocusOSDScene()))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
LOCAL BOOLEAN AllocAndGetESGInfo(
                                 SCI_DATE_T date,               //date for esg
                                 MMIMTV_ESG_LIST_SCENE_T  *esg_param    //service info
                                 )
{
    BOOLEAN     result = TRUE;
    MTV_DATE_T  mtv_date = {0};
    uint16      network_id = 0;
    uint16      service_id = 0;
    
    if(PNULL == esg_param)
    {
        return FALSE;
    }
    
    network_id=(uint16)esg_param->net_id;
    service_id=(uint16)esg_param->service_id;
    //SCI_TRACE_LOW:"AllocAndGetESGInfo networkID = %d, serviceID = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7265_112_2_18_2_40_36_839,(uint8*)"dd",network_id,service_id);
    SCI_MEMCPY(&mtv_date,&date,sizeof(MTV_DATE_T));
    if(ESG_SUCCESS != CMMB_GetAllProg(network_id,service_id,mtv_date, &esg_param->proglist))
    {
        result = FALSE;
    }
    return result;
}

/*****************************************************************************/
// 	Description : 开始录制节目
//	Global resource dependence : none
//  Author: xin.li
//	Note: 只有在播放时才可录制节目
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_StartRecord(void)
{
    BOOLEAN return_val = FALSE;
    
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_StartRecord play_status = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7282_112_2_18_2_40_36_840,(uint8*)"d", s_mtv_current_para.play_status);
    s_mtv_current_para.record_tip_time_count = 0;
    if(MMIMTV_PLAY_PLAYING == s_mtv_current_para.play_status)
    {
        if(MMIMTV_RECORD_NONE == s_mtv_current_para.record_status)
        {
            if(MMIMTV_CreateRecordFileName())
            {
                //create record file name ok, launch record
                if(SCI_ERROR == CMMB_EnableRecord(MTV_ENABLE_RECORD, (uint16*)MMIMTV_GetRecordFullFilePath()))
                {
                    //发起录制失败，提示错误
                    s_mtv_current_para.record_status = MMIMTV_RECORD_NONE;
                    MMIMTV_SetLastOSDScene();
                    MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                    MMIMTV_SetFocusOSDScene(OSD_PUB);
                    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_RECORD_FAIL);
                }
                else
                {
                    s_mtv_current_para.record_status     = MMIMTV_RECORD_RECORDING;
                    StartRecordTipTimer();
                    return_val = TRUE;
                }
            }
            else
            {
                //create record file name fail, no disk or no space
                //提示无足够空间
                s_mtv_current_para.record_status = MMIMTV_RECORD_NONE;
                MMIMTV_SetLastOSDScene();
                MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
                MMIMTV_SetFocusOSDScene(OSD_PUB);
                MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_NO_SPACE);
            }
        }
    }
    
    return return_val;
}

/*****************************************************************************/
// 	Description : 停止录制节目
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_StopRecord(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_StopRecord"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7329_112_2_18_2_40_36_841,(uint8*)"");
    if(MMIMTV_RECORD_RECORDING == s_mtv_current_para.record_status)
    {
        //stop record
        CMMB_EnableRecord(MTV_DISABLE_RECORD, PNULL);
        s_mtv_current_para.record_status = MMIMTV_RECORD_NONE;
        //stop record tip timer and clear tip
        StopRecordTipTimer();
        MMIMTV_ClearRecordTip();
    }
    
    return TRUE;
}

/*****************************************************************************/
// 	Description : play record file
//	Global resource dependence : none
//  Author: xin.li
//	Note: 资源管理器callback函数, 播放录像文件
/*****************************************************************************/
PUBLIC void MMIMTV_PlayRecordCallBack(MMIFMM_DATA_INFO_T *list_data_ptr, uint32 index)
{
    wchar*  file_name_ptr = s_mtv_current_para.record_file_full_name;
    uint16  full_name_len = 0;
    
    if(PNULL == list_data_ptr)
    {
        return;
    }
    
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_PlayRecordCallBack"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7357_112_2_18_2_40_36_842,(uint8*)"");
    if(list_data_ptr->file_num > 0)
    {
        //获取待播放录像文件全路经
        SCI_MEMSET(file_name_ptr, 0, (MMIMTV_FULL_PATH_MAX_LEN * sizeof(wchar)));
        MMI_WSTRNCPY(file_name_ptr,
            MMIMTV_FULL_PATH_MAX_LEN,
            list_data_ptr->pathname,
            list_data_ptr->pathname_len,
            list_data_ptr->pathname_len);

        //插入"\"符
        full_name_len = list_data_ptr->pathname_len;
        *(file_name_ptr + full_name_len) = MMIFILE_SLASH;
        full_name_len++;

        MMI_WSTRNCPY(file_name_ptr + full_name_len,
            MMIMTV_FULL_PATH_MAX_LEN - full_name_len,
            list_data_ptr->data[index]->filename_ptr,
            list_data_ptr->data[index]->filename_len,
            list_data_ptr->data[index]->filename_len);

        //播放录像文件
        MMIMTV_CloseFocusScene();				//场景关闭及重置处理
        MMIMTV_SetFocusOSDScene(OSD_RECORD_LOADING);		//OSD_RECORD_LOADING
        MMIMTV_SetPreOSDScene(OSD_IDLE);
        MMK_WinGrabFocus(MMIMTV_MAIN_WIN_ID);   //不关闭FMM窗口,仅切换Focus窗口
        PlayRecordfile();                       //launch play record file
    }
}

/*****************************************************************************/
// 	Description : get current playing info 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_INFO_T* MMIMTV_GetPlayInfo(void)
{
    return &s_mtv_current_para;
}

/*****************************************************************************/
// 	Description : set current focus OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetFocusOSDScene(MMIMTV_OSD_SCENE_TYPE_E scene)
{
    if(scene >= OSD_ID_MAX)
    {
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_SetFocusOSDScene scene >= OSD_ID_MAX"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7405_112_2_18_2_40_36_843,(uint8*)"");
        return;
    }
    
    s_focus_osd = scene;
}

/*****************************************************************************/
// 	Description : get focus OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMIMTV_OSD_SCENE_TYPE_E MMIMTV_GetFocusOSDScene(void)
{
    return s_focus_osd;
}

/*****************************************************************************/
// 	Description : close focus OSD scene directly
//	Global resource dependence : none
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_CloseFocusScene(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_CloseFocusScene s_focus_osd = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7427_112_2_18_2_40_36_844,(uint8*)"d", s_focus_osd);
    if((s_focus_osd >= OSD_ID_MAX)
        || (PNULL == s_osd_msg_handle_tab[s_focus_osd]))
    {
        return;
    }
    
    s_osd_msg_handle_tab[s_focus_osd](MMIMTV_CLOSE_SCENE, PNULL);
}

/*****************************************************************************/
// 	Description : set previous OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note:用于被中断场景的恢复
/*****************************************************************************/
PUBLIC void MMIMTV_SetPreOSDScene(MMIMTV_OSD_SCENE_TYPE_E scene)
{
    if(scene >= OSD_ID_MAX)
    {
        return;
    }
    
    s_pre_osd = scene;
}

/*****************************************************************************/
// 	Description : set LAST OSD scene
//	Global resource dependence : none
//  Author: 
//	Note:调用OSD_PUB前如果不确认该返回哪个场景，则需要调用此函数，存储当前的场景
/*****************************************************************************/
PUBLIC void MMIMTV_SetLastOSDScene(void)
{
    //如果当前场景是OSD_PUB或OSD_NONE则不用保存
    if(OSD_PUB != s_focus_osd && OSD_NONE != s_focus_osd)
    {
        s_pre_osd = s_focus_osd;
    }
    else if(0 == s_pre_osd)
    {
        s_pre_osd = OSD_IDLE;
    }
}

/*****************************************************************************/
// 	Description : get previous OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note:用于被中断场景的恢复
/*****************************************************************************/
PUBLIC MMIMTV_OSD_SCENE_TYPE_E MMIMTV_GetPreOSDScene(void)
{
    return s_pre_osd;
}

/*****************************************************************************/
// 	Description : set source OSD scene
//	Global resource dependence : none
//  Author: xin.li
//	Note: 标记来源场景，用于特殊场景的返回
/*****************************************************************************/
PUBLIC void MMIMTV_SetSourceOSDScene(MMIMTV_OSD_SCENE_TYPE_E scene)
{
    if(scene >= OSD_ID_MAX)
    {
        return;
    }
    
    s_source_osd = scene;
}

/*****************************************************************************/
// 	Description : set parameter for OSD scene restore
//	Global resource dependence : none
//  Author: xin.li
//	Note:设置场景恢复需要的参数
/*****************************************************************************/
PUBLIC void MMIMTV_SetOSDRestoreParam(uint32 param)
{
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_SetOSDRestoreParam param = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7497_112_2_18_2_40_36_845,(uint8*)"d", param);
    s_osd_restore_param = param;
}

/*****************************************************************************/
// 	Description : open OSD_TO_OTHER_APP scene  
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenOtherFunction(void)
{
    MMIMTV_CloseShortcutIconScene();
    MMIMTV_SetFocusOSDScene(OSD_TO_OTHER_APP);
    MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, PNULL);
}

/*****************************************************************************/
// 	Description : open OSD_ESG_LIST scene  
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_OpenESGList(void)
{
    if(0 != s_mtv_current_para.net_eid && MMIMTV_GetNetListNumber() >0)
    {
        s_search_mode.esg_netid = s_mtv_current_para.net_eid;
        s_search_mode.esg_serviceid = s_mtv_current_para.service_id;
        MMIMTV_CloseShortcutIconScene();
        s_source_osd = OSD_IDLE;
        MMIMTV_SetFocusOSDScene(OSD_ESG_LIST);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)OSD_IDLE);
    }
    else
    {
        //提示网络列表为空
        MMIMTV_SetPreOSDScene(OSD_SHORTCUT_ICON);            //设置OSD_PUB返回的场景
        MMIMTV_HandleMsg(MMIMTV_CLOSE_SCENE, PNULL);
        MMIMTV_SetFocusOSDScene(OSD_PUB);
        //保存OSD_PUB场景参数，保证外部中断返回正常显示OSD_PUB场景
        MMIMTV_SetOSDRestoreParam((uint32)TXT_MTV_ESG_NO_NETWORK);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)TXT_MTV_ESG_NO_NETWORK);
    }
}

/*****************************************************************************/
// 	Description : 重新设置背景图片（缩放切换时需要重设，否则会导致字体模糊
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_ResetBG(void)
{
    if(s_mtv_current_para.is_playing_broadcast)
    {
        MMIMTV_SetBMPtoVChip(IMAGE_MTV_BG1 + MMIMTV_GetBroadcastBG());
    }
}

/*****************************************************************************/
// 	Description : whether 6600V chip is running
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMTV_IsVRunning(void)
{
    return (s_mtv_nv_ctl_table_ptr != PNULL);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note: return value  1 date1>date2 ;   0 date1=date2 ; -1 date1<date2
/*****************************************************************************/
PUBLIC int32 MMIMTV_DateCompare(SCI_DATE_T date1,   
                                SCI_DATE_T date2
                                )
{
    int32 result = 0;

    if (date1.year > date2.year)
    {
        result = 1;
    }
    else if(date1.year == date2.year 
        && date1.mon > date2.mon)
    {
        result = 1;
    }
    else if(date1.year == date2.year 
        && date1.mon == date2.mon
        && date1.mday > date2.mday)
    {
        result = 1;
    }
    else if(date1.year == date2.year 
        && date1.mon == date2.mon
        && date1.mday == date2.mday)
    {
        result = 0;
    }
    else
    {
        result = -1;
    }
    return result;
}

/*****************************************************************************/
//    Description : move one day backward
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIMTV_MovePrevDay(
                               SCI_DATE_T  date_in,
                               SCI_DATE_T  *date_out_ptr
                               )
{
    uint16  year     = date_in.year;
    uint8   month    = date_in.mon;
    uint8   date     = date_in.mday;
    uint8   wday     = date_in.wday;
    
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (1 != date)   //不是第一天，移到前一天
    {
        date--;
    }
    else if (1 != month) //第一天且不是一月，移到上月最后一天
    {
        month--;
        date = MMICALENDAR_GetMonthday(year, month);
    }
    else if (QUERY_YEAR_BOTTOM < year)   //一月一日且年没超出范围，移到上年最后一天
    {
        year--;
        month = MTV_DECEMBER;
        date = MMICALENDAR_GetMonthday(year, month);
    }
    else if (QUERY_YEAR_BOTTOM == year)
    {
        year = QUERY_YEAR_TOP;
        month = MTV_DECEMBER;
        date = MMICALENDAR_GetMonthday(year, month);
    }
    date_out_ptr->wday = (wday+WEEKDAY_NUM-1)%WEEKDAY_NUM;
    date_out_ptr->year = year;
    date_out_ptr->mon = month;
    date_out_ptr->mday= date;
    
    return;
}


/*****************************************************************************/
//    Description : move one day forward
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIMTV_MoveNextDay(
                               SCI_DATE_T  date_in,
                               SCI_DATE_T  *date_out_ptr
                               )
{
    uint16      year    = date_in.year;
    uint8       month   = date_in.mon;
    uint8       date    = date_in.mday;
    uint8       wday    = date_in.wday;
    
    if(PNULL == date_out_ptr)
    {
        return;
    }
    
    if (MMICALENDAR_GetMonthday(year, month) != date)   //不是最后一天, 移到后一天
    {
        date++;
    }
    else if (MTV_DECEMBER !=month) //最后一天且不是十二月，移到下月第一天
    {
        month++;
        date = 1;
    }
    else if (QUERY_YEAR_TOP > year)   //十二月最后一天且年不超出范围，移到下年第一天
    {
        year++;
        month = 1;
        date = 1;
    }
    else if (QUERY_YEAR_TOP == year)
    {
        year = QUERY_YEAR_BOTTOM;
        month = 1;
        date = 1;
    }
    date_out_ptr->wday = (wday+1)%WEEKDAY_NUM;
    
    date_out_ptr->year = year;
    date_out_ptr->mon = month;
    date_out_ptr->mday = date;
    
    return;
}

/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
// LOCAL void MMIMTV_YUV422ToInterlacedYUV444(uint8 *src_y_buf,        //y data buffer of YUV422 data
//                                            uint8 *src_uv_buf,       //uv data buffer of YUV422 data
//                                            uint8 *dst_yuv_buf,      //yuv interlaced data of YUV444
//                                            uint32 width,            //image width,must be divided exactly by 2
//                                            uint32 height            //image height
//                                            )
// {
//     uint32 i = 0;
//     uint32 j = 0;
//     uint8 u = 0;
//     uint8 v = 0;
// 
//     SCI_ASSERT (PNULL != src_y_buf);
//     SCI_ASSERT (PNULL != src_uv_buf);
//     SCI_ASSERT (PNULL != dst_yuv_buf);
//     SCI_ASSERT (0 == (width & 0x1));
// 
//     for (i=0; i<height; i++)
//     {
//         for (j=0; j<width; j+=2)
//         {
//             *dst_yuv_buf++ = *src_y_buf++;
//             u = *src_uv_buf++;
//             v = *src_uv_buf++;
//             *dst_yuv_buf++ = u;
//             *dst_yuv_buf++ = v;
//         
//             *dst_yuv_buf++ = *src_y_buf++;
//             *dst_yuv_buf++ = u;
//             *dst_yuv_buf++ = v;
//         }
//     }    
// }

/*****************************************************************************/
//    Description : 
//    Global resource dependence : 
//    Author:
//    Note:
/*****************************************************************************/
PUBLIC MMIMTV_SNAPSHOT_ERROR_TYPE_E MMIMTV_Snapshot(void)
{
    MMIMTV_SNAPSHOT_ERROR_TYPE_E return_val = MMIMTV_SNAPSHOT_ERROR_NONE;
    uint8 *yuv422_buffer_ptr = PNULL;
    //uint8 *yuv444_buffer_ptr = PNULL;
    uint8 *y_buffer_ptr = PNULL;
    uint8 *uv_buffer_ptr = PNULL;
    uint8 *jpgencode_buffer_ptr = PNULL;
    uint8 *jpg_buffer_ptr = PNULL;
    uint32 width = MMIMTV_LCD_WIDTH;
    uint32 height = MMIMTV_LCD_HEIGHT;
    uint32 result = 0;
    uint32 write_size = 0;
    uint32 text_id = TXT_SUCCESS;
    JINF_RET_E jinf_ret = 0;
    SFS_HANDLE sfs_handle = 0;
    JINF_ENC_IN_PARAM_T enc_in_param = {0};
    JINF_ENC_OUT_PARAM_T enc_out_param = {0};
    
    
    if(MMIMTV_CreateSnapshotPictureName())
    {
        yuv422_buffer_ptr = BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_YUV422);
        if(PNULL == yuv422_buffer_ptr)
        {
            SCI_TRACE_LOW("[MMIMTV]: MMIMTV_Snapshot yuv422_buffer_ptr alloc fail");
            return MMIMTV_SNAPSHOT_ENCODE_ERROR;
        }
        
        result = CMMB_TakePicture(yuv422_buffer_ptr, 0, (KDuint32*)&width, (KDuint32*)&height);
        if(MTV_TAKE_PICTURE_ERROR != result)  //MTV_TAKE_PICTURE_ERROR 抓拍失败；其它则是VIDEO_DISP_DATATYP_E类型
        {  
            y_buffer_ptr = yuv422_buffer_ptr;
            uv_buffer_ptr = yuv422_buffer_ptr + width*height;
            
            jpg_buffer_ptr = BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_JPG);
            jpgencode_buffer_ptr = BL_Malloc((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_JPGENCODE);
            if(PNULL == jpg_buffer_ptr
                || PNULL == jpgencode_buffer_ptr)
            {
                SCI_TRACE_LOW("[MMIMTV]: MMIMTV_Snapshot BL_Malloc fail");
                return MMIMTV_SNAPSHOT_ENCODE_ERROR;
            }
            
            enc_in_param.quality = JINF_QUALITY_MIDDLE_HIGH;
            enc_in_param.src_width = width;
            enc_in_param.src_height = height;
            enc_in_param.target_buf_ptr = jpg_buffer_ptr;			
            enc_in_param.target_buf_size = BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_JPG);
            enc_in_param.yuv_info.data_endian = JINF_DATA_BIG_ENDIAN;
            enc_in_param.yuv_info.yuv_chn.y_chn_ptr = y_buffer_ptr;	//YUV422 data
            enc_in_param.yuv_info.yuv_chn.u_chn_ptr = uv_buffer_ptr;		
            enc_in_param.encode_buf_ptr = jpgencode_buffer_ptr;		
            enc_in_param.encode_buf_size = BL_GetSize((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_JPGENCODE);
            //input format, CMMB目前只有420格式数据
            enc_in_param.yuv_info.sample_format = JINF_FORMAT_YUV420;
            enc_in_param.yuv_info.data_pattern = JINF_YUV_PATTERN_CHN_Y_UV;
            
            jinf_ret = IMGJPEG_Encode(&enc_in_param, &enc_out_param);
            
            //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_Snapshot ret: %d, jpeg_buf_ptr:0x%x, jpeg_buf_size : 0x%x "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7799_112_2_18_2_40_37_848,(uint8*)"ddd", jinf_ret, enc_out_param.jpeg_buf_ptr, enc_out_param.jpeg_buf_size);
            
            if(JINF_SUCCESS == jinf_ret)
            {
                
                sfs_handle = MMIAPIFMM_CreateFile(MMIMTV_GetSnapshotPicturePath(), SFS_MODE_WRITE|SFS_MODE_CREATE_NEW, NULL, NULL); /*lint !e655*/
                if(SFS_INVALID_HANDLE == sfs_handle)
                {
                    //SCI_TRACE_LOW:"[MMIMTV]: error: MMIMTV_Snapshot create file failed!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7807_112_2_18_2_40_37_849,(uint8*)"");
                    return_val = MMIMTV_SNAPSHOT_ERROR_MAX;//MMIMTV_SNAPSHOT_ERROR_FILE
                    text_id = TXT_ERROR;
                }
                else
                {
                    if(PNULL != enc_out_param.jpeg_buf_ptr)
                    {
                        if(SFS_NO_ERROR == MMIAPIFMM_WriteFile(sfs_handle, enc_out_param.jpeg_buf_ptr, enc_out_param.jpeg_buf_size, &write_size, PNULL))
                        {
                            //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_Snapshot write file success"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7817_112_2_18_2_40_37_850,(uint8*)"");
                            return_val = MMIMTV_SNAPSHOT_ERROR_NONE;
                        }
                        else
                        {
                            //SCI_TRACE_LOW:"[MMIMTV]: error:MMIMTV_Snapshot write file failed!"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7822_112_2_18_2_40_37_851,(uint8*)"");
                            return_val = MMIMTV_SNAPSHOT_ERROR_MAX;// TODO:TBD add an error type MMIMTV_SNAPSHOT_ERROR_FILE
                            text_id = TXT_ERROR;
                        }
                    }
                    else
                    {
                        return_val = MMIMTV_SNAPSHOT_ENCODE_ERROR;
                        text_id = TXT_ERROR;
                    }
                    
                    SFS_CloseFile(sfs_handle);
                }
            }
            else
            {
                return_val = MMIMTV_SNAPSHOT_ENCODE_ERROR;
                text_id = TXT_ERROR;
            }
            
            BL_Free((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_JPG);
            BL_Free((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_JPGENCODE);
            BL_Free((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_YUV422);
        }
        else
        {
            BL_Free((BLOCK_MEM_ID_E)BLOCK_MEM_POOL_MTV_YUV422);
            return_val = MMIMTV_SNAPSHOT_ENCODE_ERROR;
            text_id = TXT_ERROR;
        }
        
        CMMB_ResumePicture();
    }
    else
    {
        return_val = MMIMTV_SNAPSHOT_NO_SPACE;
        text_id = TXT_NO_SPACE;
    }
    
    if(MMIMTV_SNAPSHOT_ERROR_NONE != return_val)
    {
        MMIMTV_CloseFocusScene();
        MMIMTV_SetPreOSDScene(OSD_IDLE);
        MMIMTV_SetFocusOSDScene(OSD_PUB);
        MMIMTV_HandleMsg(MMIMTV_OPEN_SCENE, (DPARAM)text_id);
    }
    
    return return_val;
}

/***********************************************************************/
// 	Description : This function is used to callback. 
//	Global resource dependence : nones
//  Author:
//	Note:
/***********************************************************************/
LOCAL void MMIMTV_CallBack(uint32 ack_id, uint32 para0, uint32 para1, uint32 para2)
{
    MTV_STIMI_SIG_DATA_T sig_data ={0};
    //SCI_TRACE_LOW("[MMIMTV]: MMIMTV_CallBack ack_id =0x%x,p0 =0x%x, p1 =0x%x\n ",ack_id,para0, para1);
    
    
    switch(ack_id)
    {
    case MTV_RSSI_CHANGED:
        sig_data.signal_value = para1;
        break;
        
    case MTV_PLAY_AV_IND:
        sig_data.data_type = para0;
        break;
        
    case MTV_STIMI_SEARCH_PROG_END_CNF:
        sig_data.operate_status = MTV_STIMI_ERR_NONE;
        break;
        
    case MTV_STIMI_RCV_CTLTAB_ERR_IND:
        sig_data.operate_status = para0;
        break;
        
    case MTV_STIMI_SEARCH_ESG_END_CNF:
        sig_data.operate_status = para0;
        break;
        
    case MTV_STIMI_SEARCH_ESG_ERR_IND:
        sig_data.operate_status = para0;
        break;
        
    case MTV_STIMI_COMPLETE_FREQUENCY_IND:
        sig_data.operate_status = MTV_STIMI_ERR_NONE;
        sig_data.data_type = para0;
        sig_data.data_len  = para1;
        break;
        
    case MTV_STIMI_PROGRAM_HINT_IND:
        sig_data.operate_status = MTV_STIMI_ERR_NONE;
        sig_data.data_type = para0;
        sig_data.data_len  = para1;
        break;
        
    case MTV_STIMI_EMERGENCY_BROADCAST_IND:
        sig_data.operate_status = MTV_STIMI_ERR_NONE;
        sig_data.data_type = para0;
        sig_data.data_len  = para1;
        break;

    case MTV_ESG_PARSE_RESULT_IND:
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_CallBack MTV_ESG_PARSE_RESULT_IND p0 = %d, p1 = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7927_112_2_18_2_40_37_852,(uint8*)"dd", para0, para1);
        sig_data.data_type = para0;
        break;

#ifdef MBBMS_SUPPORT
    //MSK key domain id
    case MTV_CA_NOT_GRANTED_IND:
        sig_data.key_domain_id = para0;
        break;
#endif
    case MTV_STOP_ACK:
    case MTV_STOP_ERROR:
        //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_CallBack received MTV_STOP_ACK op_type = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7939_112_2_18_2_40_37_853,(uint8*)"d", s_mtv_current_para.op_type);
        if(MMIMTV_OP_EXIT == s_mtv_current_para.op_type)
        {
            //异步停止成功
            SetIsReceivedStopACK();
        }
        break;
        
    default:
        break;   
    }
    //SendSignalToTask(ack_id, (void *)&sig_data, P_APP);
}

/*****************************************************************************/
// 	Description : send signal to task
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void SendSignalToTask(uint16 sig_code,void *signal_data_ptr,uint32 task_id)
{
#if 0
    MTV_STIMI_SIG_T *sig_to_mmi_ptr = PNULL;
    uint32 result = 0;
    
    //SCI_TRACE_LOW("[MMIMTV]: SendSignalToTask task id=%d,sig_code=%d\n ",task_id,sig_code);
    if(task_id == P_APP )
    {
        sig_to_mmi_ptr = (MTV_STIMI_SIG_T*)SCI_ALLOC(sizeof(MTV_STIMI_SIG_T));
        if(PNULL == sig_to_mmi_ptr)
        {
            //SCI_TRACE_LOW:"[MMIMTV]: SendSignalToTask alloc sig_to_mmi_ptr fail"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7969_112_2_18_2_40_37_854,(uint8*)"");
            return;
        }

        sig_to_mmi_ptr->SignalCode = sig_code;
       	sig_to_mmi_ptr->SignalSize = sizeof(MTV_STIMI_SIG_T);
       	sig_to_mmi_ptr->Sender = SCI_IdentifyThread();
       	sig_to_mmi_ptr->data = *(MTV_STIMI_SIG_DATA_T *)signal_data_ptr;
       	result = SCI_SendSignal((xSignalHeader)sig_to_mmi_ptr, task_id);
    } 
#endif
}

/*****************************************************************************/
// 	Description : set is_recived_stop_ack when received MTV_STOP_ACK msg
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
LOCAL void SetIsReceivedStopACK(void)
{
    //SCI_TRACE_LOW:"[MMIMTV]: SetIsReceivedStopACK"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_7987_112_2_18_2_40_37_855,(uint8*)"");
    
    s_mtv_current_para.is_recived_stop_ack = TRUE;
}

/*****************************************************************************/
// 	Description : 完成CMMB部分初始化，包括MMI，LCD_Init, CMMB声音及背景图片
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_InitCMMB(void)
{
    //uint32  init_ret = 0;
    AUDIO_DEVICE_MODE_TYPE_E audio_mode = 0;
    MMISRV_HANDLE_T          audio_handle = MMIMTV_GetAudioHandle();
    MMIMTV_VOLUME_E          ring_vol     = 0;
    
    //SCI_TRACE_LOW:"[MMIMTV]: MMIMTV_InitCMMB"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_8003_112_2_18_2_40_37_856,(uint8*)"");
    s_is_mtv_open = TRUE;
    
    //MMI相关初始化
    MMIAPIDC_AllocSettingMemory();
    MMIAPIDC_InitOSDMenuIcon();
    MMIMTV_NewOSDDisplayData();
    MMIMTV_NewDataData();
    MMIMTV_InitSetting();
    
    //load EmergencyBoradcast msg
    MMIMTV_ReadEBFile();
    
    //s_lcdtv_spec.back_color = MMI_BLUE_COLOR;
    //s_lcdtv_spec.order = LCDTV_LAYER_ORDER_OSD;
    //s_lcdosd_spec.alpha = MMIMTV_TRANSPARENT_COLOR_0;
    //LCD_Init();
    //LCDTV_SetLCDSpec(&s_lcdtv_spec);
    //LCDTV_SetOSDSpec(&s_lcdosd_spec);
    //MMIMTV_SetVideoSpec(MMIMTV_GetZoomMode());
    //LCDTV_Invalidate(0);
    //LCDTV_EnableLCDSpec(TRUE);
    //SCI_TRACE_LOW("[MMIMTV]: LCDTV ok");
    
    s_mtv_prog_hint_info_ptr = SCI_ALLOC_APP(sizeof(MMIMTV_PROG_HINT_INFO_T));
    if(PNULL != s_mtv_prog_hint_info_ptr)
    {
        SCI_MEMSET(s_mtv_prog_hint_info_ptr, 0, sizeof(MMIMTV_PROG_HINT_INFO_T));
    }
    
    s_mtv_nv_ctl_table_ptr = SCI_ALLOC_APP(MMIMTV_NV_CTRL_TABLE_MAX_NUM * sizeof(MMIMTV_NV_CTL_T ));
    //SCI_MEMSET(s_mtv_nv_ctl_table_ptr, 0, (sizeof(MMIMTV_NV_CTL_T)*MMIMTV_NV_CTRL_TABLE_MAX_NUM));
    
    //set audio volume
    //MMIAPISET_GetDeviceMode(&audio_mode);
    //MMIAUDIO_SetVolume(audio_mode, MMIMTV_GetVolume());
    ring_vol = MMIMTV_GetVolume();
    if(MMIMTV_IsMute())
    {
        CMMB_Mute();
        ring_vol = 0;
        //MMIAUDIO_SetMute(TRUE);
    }
    if(audio_handle > 0)
    {
        MMISRVAUD_SetVolume(audio_handle, ring_vol);
    }
    
    //MMIMTV_CreateWatchTimeTipTimer();
    
    return;
}

/*****************************************************************************/
// 	Description : 播放窗口失去焦点时的CMMB OSD透明度设置
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_SetLoseFocusOSDSpec(void)
{
    //OSD全透
    s_lcdosd_spec.alpha = MMIMTV_TRANSPARENT_COLOR_0;
    s_lcdosd_spec.colorkey_enable = FALSE;
    //LCDTV_SetOSDSpec(&s_lcdosd_spec);
}

/*****************************************************************************/
// 	Description : 播放窗口获得焦点时的CMMB OSD透明度设置
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMTV_SetGetFocusOSDSpec(void)
{
    //恢复OSD设置
    s_lcdosd_spec.alpha = MMIMTV_GetTransparencyValue(MMIMTV_GetTransparency());
    s_lcdosd_spec.colorkey_enable = TRUE;
    //LCDTV_SetOSDSpec(&s_lcdosd_spec);
}

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIMTV_SetDelayPowerOff(BOOLEAN is_delay)
{
    s_is_delay_poweroff = is_delay;
}

/*****************************************************************************/
// 	Description : 向频道切换栏添加频道项
//	Global resource dependence : 
//  Author: xin.li
//	Note: 依据当前播放网络下的频道列表
/*****************************************************************************/
PUBLIC void MMIMTV_AddServiceItemToTabCtrl(MMI_CTRL_ID_T ctrl_id)
{
    uint16          service_count = 0;
    uint32          index = 0;
    uint16          service_name_len = 0;
    uint16          min_len = 0;
    GUITAB_ITEM_T   item_t = {0};
    GUITAB_ITEM_CONTENT_T       item_data_t = {0};
    MMIMTV_SAVE_NETEID_INFO_T   *net_ptr = PNULL;
    MMIMTV_SAVE_SERVICE_INFO_T  *service_ptr = PNULL; 
    
    item_t.item_state = GUITAB_ITEMSTATE_SHOW_TEXT;
    item_t.item_data_ptr = &item_data_t;
    item_data_t.text_data.is_disp_font_bg = TRUE;
    item_data_t.text_data.text_type = TABITEM_DATA_TEXT_BUFFER;
    
    net_ptr = MMIMTV_GetPlayingNetNode();
    if(PNULL == net_ptr)
    {
        //未选定当前网络节点时，默认取首个网络
        net_ptr = MMIMTV_GetNetNodeViaIndex(0);
        if(PNULL == net_ptr)
        {
            //SCI_TRACE_LOW:"MMIMTV_AddServiceItemToTabCtrl net == 0"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_8116_112_2_18_2_40_38_857,(uint8*)"");
            return;
        }
    }
    
    
    service_count = MMIMTV_GetServiceListNumber(net_ptr);
    if (service_count > GUITAB_MAX_ITEM_NUM)
    {
        //SCI_TRACE_LOW:"MMIMTV_AddServiceItemToTabCtrl service_count = %d large than GUITAB_MAX_ITEM_NUM"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_8125_112_2_18_2_40_38_858,(uint8*)"d", service_count);
        service_count = GUITAB_MAX_ITEM_NUM;
    }
    
    GUITAB_SetMaxItemNum(ctrl_id, service_count);
    GUITAB_SetItemSpace(ctrl_id, MMIMBBMS_TAB_ITEM_SPACE, FALSE);
    
    
    service_ptr = net_ptr->service_head;
    while(PNULL != service_ptr)
    {
        service_name_len = MMIAPICOM_Wstrlen(service_ptr->service_name);
        min_len = MIN(service_name_len, GUITAB_MAX_ITEM_NAME_LENGTH);
        MMIAPICOM_Wstrncpy(item_data_t.text_data.text.text_buffer.wstr, service_ptr->service_name, min_len);
        item_data_t.text_data.text.text_buffer.wstr_len = min_len;
        
        GUITAB_AppendItem(ctrl_id, &item_t);
        SCI_MEMSET(item_data_t.text_data.text.text_buffer.wstr, 0, sizeof(item_data_t.text_data.text.text_buffer.wstr));
        
        service_ptr = service_ptr->service_next;
    }
    
    //设置当前位置
    index = MMIMTV_GetPlayingServiceIndexViaNode(MMIMTV_GetPlayingServiceNode());
    if(index < service_count)
    {
        GUITAB_SetCurSel(ctrl_id, index);
    }
    else
    {
        GUITAB_SetCurSel(ctrl_id, 0);
    }
    
    MMK_SetAtvCtrl(MMIMTV_MAIN_WIN_ID, ctrl_id);
}

#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : 关闭CMMB及相关资源的释放
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_CloseCMMB(void)
{
    //SCI_TRACE_LOW:"[MMIMBBMS]: MMIMBBMS_CloseCMMB"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMTV_MAIN_8168_112_2_18_2_40_38_859,(uint8*)"");
    
    MMIMTV_ExitMTV();
    
    //reset lcd
    LCD_Init();
    
    MMIMTV_ClearNetList();
    MMIMTV_ClearEBList();
    MMIMTV_StopSysTimer(MMIMTV_SYSTIME_ALL);
    MMIMTV_SaveSetting();
    MMIAPIDC_FreeGUI();
    MMIAPIDC_FreeSettingMemory();
    MMIMTV_DeleteOSDDisplayData();
    MMIMTV_DeleteDataData();
}

/*****************************************************************************/
// 	Description : 是否需发起播放
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsNeedPlayService(void)
{
    return s_is_need_play_service;
}

/*****************************************************************************/
// 	Description : 设置是否需播放标志
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_SetIsNeedPlayService(BOOLEAN is_need_play)
{
    s_is_need_play_service = is_need_play;
}

/*****************************************************************************/
// 	Description : 是否需恢复播放
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMBBMS_IsNeedResumePlay(void)
{
    return MMIMBBMS_GetPlayInfo()->is_need_resume_play;
}

/*****************************************************************************/
// 	Description : restore resume play flag
//	Global resource dependence : none
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIMBBMS_ResetResumePlayFlag(void)
{
    MMIMBBMS_GetPlayInfo()->is_need_resume_play = FALSE;
}

/*****************************************************************************/
//  Description : set update status of MSK   
//  Global resource dependence : 
//  Author: xiaoxiang.huang
//  Note: 
/*****************************************************************************/
PUBLIC void SetMSKUpdateStatus(uint32 mskupdatestatus)
{
	s_msk_update_status = mskupdatestatus;
}

/*****************************************************************************/
//  Description : set update status of MSK   
//  Global resource dependence : 
//  Author: xiaoxiang.huang
//  Note: 
/*****************************************************************************/
LOCAL MMIMBBMS_FLOW_STATE_E GetMSKUpdateStatus(void)
{
	return s_msk_update_status;
}
#endif

/*****************************************************************************/
//  Description : API of create audio file handle
//  Global resource dependence : none
//  Author: 
//  Note: SUCCESS - return the audio handle created
//        FAILURE - return SCI_NULL
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIMTV_RequestHandle(void)
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};

    req.pri = MMISRVAUD_PRI_NORMAL;
    audio_srv.info.type = MMISRVAUD_TYPE_VIRTUAL;
    
    if(audio_srv.volume > MMISET_VOL_MAX)
    {
        audio_srv.volume = MMISET_VOL_MAX;
    }

    return MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
}

/*****************************************************************************/
//  Description : Set current play handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIMTV_GetAudioHandle(void)
{
    return s_mtv_audio_handle;
}

/*****************************************************************************/
//  Description : Set current record handle
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC MMISRV_HANDLE_T MMIMTV_SetAudioHandle(MMISRV_HANDLE_T handle)
{
    return s_mtv_audio_handle = handle;
}

#endif
