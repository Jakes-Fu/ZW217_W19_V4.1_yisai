/*****************************************************************************
** File Name:      mmiidle_statusbar.h                                       *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Creat
******************************************************************************/
#ifndef _MMIIDLE_STATUSBAR_H_
#define _MMIIDLE_STATUSBAR_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_font.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guistring.h"
#include "mn_type.h"
#include "mmi_signal_ext.h"
#include "mmipdp_export.h"
#include "mmiidle_func.h"
#include "guistatusbar_scrollmsg.h"
#include "dualbat_drvapi.h"

#ifdef MRAPP_SUPPORT
#include "mmimrapp_export.h"
#endif
#ifdef HERO_ENGINE_SUPPORT 
#include "heroEngineExport.h"
#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
#ifdef WRE_SUPPORT
#define WRE_ICON_MAX 6  //add for wre_background by heng.xiao 20120208
#endif
/*
�޸�������Ŀ�����ĳ����Ŀ��״̬ʱ����Ҫע�⣬statusbar����ʾλ��Ҫô����Ҫô���ҡ�
��statusbar�ڲ���ʾ���߼��Ǵ����һ��item��ʼ(ö�ٵ����һ��Ԫ��)�ݼ�����item��
��������ĳ����Ŀ�ǿ�����ʾ������Ϊδ��������item���ǿ�����ʾ����δ����������Ŀ����ǿ�����ʾ
�ͻ���ʾ�������������߼��ɲο�statusbar�ڲ����ƴ��롣
*/
typedef enum
{
#ifdef PDA_UI_DROPDOWN_WIN
  MMI_WIN_TEXT_DATE,          // MAIN LCD����
#endif
#ifdef MMI_VOLTE_SUPPORT
  MMI_WIN_ICON_VOLTE,
#endif
  MMI_WIN_ICON_SIGNAL,        // MAIN LCD sim1�ź�����ʾ
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#ifdef MMI_VOLTE_SUPPORT
  MMI_WIN_ICON_VOLTE_2,
#endif
  MMI_WIN_ICON_SIGNAL_2,        //MAIN LCD sim2�ź�����ʾ
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
  MMI_WIN_ICON_SIGNAL_3,        //MAIN LCD sim3�ź�����ʾ
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
  MMI_WIN_ICON_SIGNAL_4,        //MAIN LCD sim4�ź�����ʾ
#endif
#endif
  MMI_WIN_NETWORK_NAME,         // ��ʾ��Ӫ������  Bug-1334911 
#if !defined CMCC_UI_STYLE && !defined NET_ICON_UI_STYLE
#ifdef PRODUCT_DM
  MMI_WIN_ICON_GPRS,          // 3G gprs������
#endif
#endif
  MMI_WIN_ICON_ROAMING,       // MAIN LCD������ʾ

  MMI_WIN_ICON_CALLING,       // MAIN LCD����ͨ����
  MMI_WIN_ICON_MISSED_CALL,   // MAIN LCDδ������
  MMI_WIN_ICON_MESSAGE,       // MAIN LCD������ʾ
#ifdef MMI_IDLE_FDN_ICON_SUPPORT
  MMI_WIN_ICON_FDN,
#endif
MMI_WIN_ICON_GPS,			  //��ʾGPSͼ��
#ifdef MMI_WIFI_SUPPORT
  MMI_WIN_ICON_WIFI,          // MAIN LCD WIFI
#endif
#if defined(CMMB_SUPPORT) || defined(ATV_SUPPORT)
  MMI_WIN_ICON_CMMB_SIGNAL,   // CMMB Signal
#endif
#ifdef MCARE_V31_SUPPORT
    MMI_WIN_ICON_MCARE_APP,
#endif  
#ifdef QQ_SUPPORT
  MMI_WIN_ICON_SPREADTRUM_QQ, // SPRD_QQ ͼ����ʾ
#endif
#ifdef MRAPP_SUPPORT
  MMI_WIN_ICON_MRAPP,         //MAIN LCDð��
#endif
#ifdef JAVA_SUPPORT
  MMI_WIN_JAVA,               // JAVA ����������ʾ
#endif
#ifdef SNS_PULLING_SUPPORT
  MMI_WIN_ICON_SNS_FB,
  MMI_WIN_ICON_SNS_TWIT,
#endif //SNS_PULLING_SUPPORT
#ifdef ASP_SUPPORT
  MMI_WIN_ICON_ASP,           // ��¼����ͼ��
#endif
  MMI_WIN_ICON_CALL_FORWARD,  // MAIN LCD����ת��
#ifdef MMI_CSP_SUPPORT    
  MMI_WIN_ICON_LINE_INFO,     // MAIN LCDͨ����·״̬
#endif  //MMI_CSP_SUPPORT
#ifdef BLUETOOTH_SUPPORT
  MMI_WIN_ICON_BLUE_TOOTH,    // MAIN LCD����
#endif
  MMI_WIN_ICON_ALARM,         // MAIN LCD������ʾ
  MMI_WIN_ICON_ENVSET,        // MAIN LCD�龰ģʽ:����,����,����ʾ

  MMI_WIN_ICON_EARPHONE,      // MAIN LCD������ʾ
#ifdef WRE_SUPPORT   //add for wre_background by heng.xiao 20120208 
    MMI_WIN_WRE,
    MMI_WIN_WRE_MAX = MMI_WIN_WRE + WRE_ICON_MAX-1,
#endif

  // �������ȱ�洢��״̬��ʾ
#ifdef MET_MEX_QQ_SUPPORT
	MEX_MASTER_QQ_STATUS,		//QQ
	MEX_SLAVE_QQ_STATUS,		
#endif
#ifdef MET_MEX_DOUBLE_QQ_SUPPORT 
  MEX_MASTER_DOUBLE_QQ_STATUS,		//QQ
  MEX_SLAVE_DOUBLE_QQ_STATUS,	 
#endif
#ifdef MET_MEX_MSN_SUPPORT     //MSN
  MEX_MSN_STATUS,
#endif
  // �������ȱ�洢��״̬��ʾ
#ifdef MET_MEX_FETION_SUPPORT     //FETION
	MEX_FETION_STATUS,
#endif
  MMI_WIN_ICON_HD_CALL,
#if defined(HERO_ENGINE_SUPPORT)
	MMI_WIN_ICON_HERO,
#endif

#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
    MMI_WIN_ICON_BATTERY_PERCENT,
#endif
  // ��������������ʾ
  MMI_WIN_ICON_BATTERY,       // MAIN LCD�����ʾ
#ifdef MMI_DUAL_BATTERY_SUPPORT
    MMI_WIN_ICON_BATTERY_AUX,
#endif
    MMI_WIN_TEXT_TIME,          // MAIN LCDʱ��
  // end
  MMI_WIN_ICON_MAX
}MMI_WIN_ICON_E;

typedef struct
{
    uint16 mcc;
    uint16 mnc;
    BOOLEAN isGmust;
    BOOLEAN isLTEmust;
}MMI_RATICON_G_PLMN_T;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

#ifdef JAVA_SUPPORT
/*****************************************************************************/
//  Description : handle java
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_JavaCallBack(void);
#endif

#ifdef QQ_SUPPORT
/*****************************************************************************/
//  Description : handle qq
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_QqCallBack(void);
#endif

/*****************************************************************************/
//  Description : handle mcareqq
//  Global resource dependence : none
//  Author:wei.ren
//  Note: 
/*****************************************************************************/

#ifdef MCARE_V31_SUPPORT
PUBLIC BOOLEAN MMIMAIN_MCARE_QQCallBack(void);
PUBLIC BOOLEAN MMIMAIN_MCARE_QCCallBack(void);
PUBLIC uint32 Mcare_idle_iconid(void);
#endif//#ifdef MCARE_V31_SUPPORT


/*****************************************************************************/
// Description : ��ʾͼ��
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
PUBLIC void MMIDILE_StatusBarInit(void);

PUBLIC BOOLEAN MMIAPICOM_IsExistInRatIconTable(MN_PLMN_T * cur_plmn_ptr, BOOLEAN * isGmust, BOOLEAN * isLTEmust);

/*****************************************************************************/
//  Description : handle miss call
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_MissCallCallBack(void);

/*****************************************************************************/
//  Description : handle miss message
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_MissMessageCallBack(void);

#ifdef MMISMS_CHATMODE_ONLY_SUPPORT
/*****************************************************************************/
//  Description : handle miss message
//  Global resource dependence : none
//  Author: Paul.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_ReadPushCallBack(void);
#endif

/*****************************************************************************/
// Description : ��ʾ�ź�ͼ��
// Global resource dependence : 
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_SetSimStatusBarInfo(
                                MN_DUAL_SYS_E dual_sys,
                                uint32 item_index,
                                BOOLEAN is_append
                                );

/*****************************************************************************/
//  Description :  �����ı�����
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: �ı�����Ϊʱ��
/*****************************************************************************/
PUBLIC void MAIN_SetStbDispTime(void);

/*****************************************************************************/
//  Description :  ��ȡ״̬��timer�Ƿ�ɼ�
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: �ı�����Ϊʱ��
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_IsStbDispTime(void);

/*****************************************************************************/
//  Description :  ����״̬���Ƿ���ʾtimer
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: �ı�����Ϊʱ��
/*****************************************************************************/
PUBLIC void MAIN_SetStbTimeVisible(
                                   BOOLEAN is_true
                                   );
/*****************************************************************************/
//  Description :  set status bar time info
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_SetStbDispTimeinfo(wchar *time_buf,uint16 time_len);

/*****************************************************************************/
// Description : ���ʱ���ַ���
// Global resource dependence : 
// Author: xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC void MAIN_StatusbarGetSysTime(
                               wchar *time_wstr, 
                               uint16 wlen
                               );

#ifdef MMI_CSP_SUPPORT
/*****************************************************************************/
// 	Description : update the state of line
//	Global resource dependence :  
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateLineState(void);

/*****************************************************************************/
//  Description : get the state of line
//  Global resource dependence :  
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleLineState(
                                  uint16 line_state,
                                  MN_DUAL_SYS_E dual_sys
                                  );

#endif //MMI_CSP_SUPPORT

/*****************************************************************************/
//  Description : set msg state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetMsgState(BOOLEAN is_draw);

/*****************************************************************************/
//  Description : set the call forward state of the idle window
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleForwardState(
                                     MN_DUAL_SYS_E dual_sys,
                                     BOOLEAN forward_state
                                     );

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateIdleEnvsetState(void);

/*****************************************************************************/
// 	Description : set the roaming state of the idle window
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleRoamingState(
                                   MN_DUAL_SYS_E	e_dual_sys,
                                   BOOLEAN is_roaming
                                   );

/*****************************************************************************/
//  Description : ����GPSͼ��
//	Global resource dependence :
//  Author: qi.liu1
//	Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleGpsState(
                                   BOOLEAN is_enable
                                   );


/*****************************************************************************/
// Description : ��ʾͼ��
// Global resource dependence : 
// Author: Ming.Song
// Note:2007-8-3
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetGprsInfo(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMIPDP_NOTIFY_EVENT_E pdp_event,
                                    BOOLEAN is_append
                                    );
/*****************************************************************************/
//  Description : set the gprs state of the idle window
//  Global resource dependence : 
//  Author: figo.feng
//  Note: DUAL_MODE��ֻ���ر�ͼ����
/*****************************************************************************/
PUBLIC void MAIN_SetIdleGprsState(
                                   MN_DUAL_SYS_E    e_dual_sys,
                                   BOOLEAN          gprs_state
                                   );
/*****************************************************************************/
//  Description : set the rx level of the idle window
//    is_direct:TRUEֱ����ʾ�ź����䣻FALSEƽ����ʾ�ź�����
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleRxLevel(MN_DUAL_SYS_E e_dual_sys, uint8 rx_level, BOOLEAN is_direct);

/*****************************************************************************/
//  Description : get the rx level
//  
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIMAIN_GetRxLevel(MN_DUAL_SYS_E e_dual_sys);

/*****************************************************************************/
//  Description : set the battery level of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBatLevel(
                                 uint8 bat_level
                                 );

/*****************************************************************************/
//  Description : handle rxlevel bound timer
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_HandleRxLevelTimer(uint8 timer_id, uint32 param);
/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleChargeState(
                                    BOOLEAN is_charge
                                    );
#ifdef MMI_DUAL_BATTERY_SUPPORT
/*****************************************************************************/
// Description : 
// Global resource dependence : 
// Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetStbBatteryInfo(BOOLEAN is_main_battery);

/*****************************************************************************/
//  Description : set the battery level of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleDbatLevel(DBAT_STATE_INFO_T* dbat_info);

/*****************************************************************************/
// Description : 
// Global resource dependence : 
// Author: renyi.hu
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIIDLE_IsChangeChargeState(void);
#endif

#ifdef WIN32
/*****************************************************************************/
//  Description : dummy function of REF
//  Global resource dependence : 
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN CHGMNG_IsChargeConnect(void);

/*****************************************************************************/
//  Description : dummy function of REF
//  Global resource dependence : 
//  Author: bruce.chi
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GPIO_CheckHeadsetStatus(void);
#endif


/*****************************************************************************/
//  Description : ����ʱ��ӹ�����Ϣ
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
PUBLIC void MMIMAIN_PowerOnAddStbScrollItem(void);
#endif

#ifdef MMI_VOLTE_SUPPORT
/*****************************************************************************/
//  Description : set the VOLTE state of the idle window
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIdleVolteState(MN_DUAL_SYS_E dual_sys);
#endif

#ifdef MMI_WIFI_SUPPORT
/*****************************************************************************/
// 	Description : set the wifi icon of the idle window
//	Global resource dependence : 
//  Author: li.li
//	Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleWifiIcon(
                                       MMI_IMAGE_ID_T image_id
                                       );
                                       
/*****************************************************************************/
// 	Description : get the wifi icon of the idle window
//	Global resource dependence : 
//  Author: jixin.xu
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MAIN_GetIdleWifiIcon(void);

#endif

/*****************************************************************************/
//  Description : set the blue tooth state of when power on
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBTStateWhenPowerOn(
                                           BLUETOOTH_STATE_TYPE  bluetooth_state
                                           );

/*****************************************************************************/
//  Description : set the blue tooth state of the idle window
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBlueToothState(
                                       BLUETOOTH_STATE_TYPE  bluetooth_state
                                       );
/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleNetWorkName(void);

/*****************************************************************************/
//  Description : set the charge state
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateIdleSignalState(void);


/*****************************************************************************/
// 	Description : set state of tv 
//	Global resource dependence :  
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MAIN_SetTVSignalState(void);

/*****************************************************************************/
// 	Description : set state of tv 
//	Global resource dependence :  
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MAIN_SetTVSignalFalse(void);

/*****************************************************************************/
//  Description : set the battery level of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleBatLevel(
                                 uint8 bat_level
                                 );

/*****************************************************************************/
//  Description : set missed call state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetCallingState(BOOLEAN is_calling);

/*****************************************************************************/
//  Description : set missed call state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetMissedCallState(BOOLEAN is_missed_call);

/*****************************************************************************/
//  Description : set the td state of the idle window
//  Global resource dependence : 
//  Author: maggie
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleTdState(void);

/*****************************************************************************/
//  Description : set the unknown state of the idle window
//  Global resource dependence : 
//  Author: maggie
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleUnknownState(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetAllStbAppCallbackValid(
                                           BOOLEAN is_valid
                                           );

#ifdef SNS_PULLING_SUPPORT
/*****************************************************************************
*  Description : set the icon display state of the idle window
*  Global resource dependence : 
*  Author: David.Chen
*  Note:
******************************************************************************/
PUBLIC void MAIN_UpdateIdleSnsState(BOOLEAN isDisplay, int widget_id);
#endif //SNS_PULLING_SUPPORT

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleAlarmState(
                                   BOOLEAN alarm_state
                                   );


/*****************************************************************************/
//  Description : set the java state of the idle window
//  Global resource dependence : 
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateIdleJavaState(BOOLEAN java_state);

/*****************************************************************************/
//  Description : set the Alarm state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_UpdateIdleEarPhoneState(void);

/*****************************************************************************/
//  Description : set the vibrator state of the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleShakeState(
                                   BOOLEAN shake_state
                                   );

/*****************************************************************************/
//  Description : if is silent of call ring in the idle window
//  Global resource dependence : 
//  Author: Great.Tian
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleSilentState(
                                    BOOLEAN silent_state
                                    );
#ifdef HERO_ENGINE_SUPPORT
PUBLIC void MAIN_SetIdleHeroappState(BOOLEAN show_icon);
#endif

#ifdef QQ_SUPPORT
/*****************************************************************************/
// Description : ˢ��idle�µ�QQ icon
// Global resource dependence : 
// Author: fen.xie
// Note:
/*****************************************************************************/
PUBLIC  void MMIAPIIDLE_UpdateQQState(void);
#endif

#ifdef ASP_SUPPORT
/*****************************************************************************/
// Description : ˢ��idle�µ�asp icon
// Global resource dependence : 
// Author: jun.hu
// Note:
/*****************************************************************************/
PUBLIC  void MMIAPIIDLE_UpdateAspState(void);
#endif

/*****************************************************************************/
// Description : ����״̬������ͼ�����ʾ 
// Global resource dependence : 
// Author: juan.wu
// Note:
/*****************************************************************************/
PUBLIC void MMIAPIIDLE_SetStbEpDisplay(BOOLEAN is_display);

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT

/*****************************************************************************/
//  Description : ��ӹ�����Ϣ
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMAIN_SetScrollItemNewSMS(void);
                                        
/*****************************************************************************/
//  Description : ��ӹ�����Ϣ
//  Global resource dependence : none
//  Author: bin.ji
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMAIN_AppendStbScrollItem(
                                           GUISTBSCROLLMSG_SCROLL_ITEM_E item_type
                                           );
#endif

#ifdef MMI_MULTISIM_COLOR_SUPPORT_STATUSICON

/*****************************************************************************/
//  Description : Set sim color for statusbar
//  Global resource dependence : none
//  Author: sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC void MMIMAIN_UpdateStatusBarBySimColor(MN_DUAL_SYS_E dual_sys,GUI_COLOR_T color);

#endif

/*****************************************************************************/
//  Description : ������������Ϣͼ��
//  Global resource dependence :  
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetIdleVMFlagState( BOOLEAN vm_status);

#ifdef MMI_IDLE_FDN_ICON_SUPPORT
/*****************************************************************************/
//  Description : set FDN state
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MAIN_SetFDNState(BOOLEAN is_draw);
#endif

PUBLIC BOOLEAN MMIAPI_isPdpAvailable(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
