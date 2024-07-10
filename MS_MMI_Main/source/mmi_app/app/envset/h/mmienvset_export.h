/*****************************************************************************
** File Name:      mmienvset_export.h                                        *
** Author:                                                                   *
** Date:           06/02/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe environment setting         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2004       maqianlin          Create
** 02/2012       xiyuan.ma          Edit
******************************************************************************/

#ifndef _MMIENVSET_EXPORT_H_
#define _MMIENVSET_EXPORT_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_custom_define.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
** Macro Declaration *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
// ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT
    
#define MMIENVSET_ActiveMode   MMIAPIENVSET_ActiveMode

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
#define  MMIENVSET_PREVIEW_PLAY_RING_TIMES      1      //Ԥ�������Ĳ��Ŵ���
#define  MMIENVSET_PREVIEW_PLAY_DEFAUT_TIMES    255     //Ԥ�������Ĳ��Ű���ƽ̨Ĭ��ʱ������
#define  MMIENVSET_PREVIEW_PLAY_RING_DUATION    45000   //ƽ̨Ĭ��ʱ������45s
typedef enum
{
    MMIENVSET_STANDARD_MODE=0,  // ����
    MMIENVSET_SILENT_MODE,      // ��������
    MMIENVSET_MEETING_MODE,     // �񶯻���
    MMIENVSET_INSIDE_MODE,      // ��������
    MMIENVSET_NOISY_MODE,       // ��������
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    MMIENVSET_EARPHONE_MODE,
    MMIENVSET_BLUETOOTH_MODE,
#endif 
//#if defined MMI_PDA_SUPPORT
    MMIENVSET_USER_MODE1 ,
    MMIENVSET_USER_MODE2,
    MMIENVSET_USER_MODE3,
#ifndef DPHONE_SUPPORT
    MMIENVSET_USER_MODE4,
    MMIENVSET_USER_MODE5,
#endif
 //   MMIENVSET_USER_MODE_MAX = MMIENVSET_USER_MODE1+MMIENVSET_SETTING_USER_NUM-1,
//#endif
    MMIENVSET_TOTAL_MODE,
}MMIENVSET_ENV_MODE_E;

// ���������� 
typedef enum{
    CALL_RING_VOL = 0,  // ������������
    CALL_RING_TYPE,     // ������ʾ����
    MSG_RING_VOL,       // ������������
    MSG_RING_TYPE,      // ������ʾ����
    COMM_VOL,           // ͨ������
    KEY_RING_VOL,       // ���������� 
    KEY_RING_TYPE,      // ����������
    TKEY_RING_VOL,    //����������
    TKEY_RING_TYPE,     // ����������
    FLIP_RING_TYPE,     // ���ϸ��������� 
    FLIP_DOWN_RING_TYPE,     // ���ϸ��������� 
    DOWNLOAD_NOTIFY, //���������ʾ��
    VOLTAGE_WARN,       // �͵�ѹ���� o = off  1 = on
    CHARGE_NOTIFY,      // ��ʼ�����ʾ
    ALARM_RING_TYPE,    // ������ʾ����
    ALARM_RING_VOL,     // ������ʾ������С
    POWER_RING_VOL, //���ػ���������
#ifdef MMI_READMENU_ENABLE
    MENUREAD_VOL,
#endif
    POWER_RING_SWITCH,  // ���ػ���������
#ifdef LCD_SLIDE_SUPPORT
    SLIDE_RING_ON_TYPE,//����������
    SLIDE_RING_OFF_TYPE,//�ϸ�������
#endif
    ENVSET_NAME, //������
    ENVSET_DIAL_TP_VIBRATE_SWITCH,//��������,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    ENVSET_DIAL_TONE_SWITCH,//������,
#endif	
    ENVSET_TP_KEY_LOCK_SWITCH, //��Ļ������ʾ��,
#ifdef MMI_READMENU_ENABLE
    ENVSET_MENU_READ_SWITCH,// �˵��ʶ�
#endif
    ENVSET_MAX_OPT
}MMIENVSET_SET_OPT_E;

 //�龰ģʽ����
typedef struct 
{
    wchar       wstr[MMIENVSET_ENVNAME_MAX_LEN + 1];
    uint16      wstr_len;
}ENVSET_NAME_STR_INFO_T;

//the information of envset setting
typedef struct{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
    MN_DUAL_SYS_E           dual_sys;           //  sim select
#endif
    uint8                   call_ring_vol;      //  MMIENVSET_CALL_RING_VOL
    uint8                   msg_ring_vol;       //  MMIENVSET_MSG_RING_VOL
    
    uint8                   comm_vol;           //  MMIENVSET_COMM_VOL
    uint8                   key_ring_vol;       //  MMIENV_KEY_RING_VOL 
    uint8                   alarm_ring_vol;       //  MMIENV_ALARM_RING_VOL 
    uint8                   tkey_ring_vol;   //����������
    uint8                   power_ring_vol;  //���ػ���������
#ifdef MMI_READMENU_ENABLE
    uint8                   menuread_vol;
#endif
    
    uint8                   call_ring_type[MMI_DUAL_SYS_MAX];     // MMINV_SET_CALL_RING_TYPE
    uint8                   msg_ring_type[MMI_DUAL_SYS_MAX];      // MMINV_SET_MSG_RING_TYPE
    uint8                   key_ring_type;      // MMINV_SET_KEY_RING_INFO
    uint8                   tkey_ring_type;      // MMINV_SET_TKEY_RING_INFO
    uint8                   flip_ring_type;     // MMINV_SET_FLIP_RING
    uint8                   flip_down_ring_type;     // MMINV_SET_FLIP_RING
#ifdef LCD_SLIDE_SUPPORT
    uint8  		            slide_on_ring_type;     // MMINV_SET_SLIDE_ON_RING
    uint8  		            slide_off_ring_type;     // MMINV_SET_SLIDE_OFF_RING
#endif
    uint8                   alarm_ring_type;    // MMIENV_SET_ALARM_TYPE               
    uint8                   download_notify;
    uint8                   low_voltage_warn;   // MMIENV_SET_VOLTAGE_WARNING
    uint8                   charge_notify;
    uint8                   power_ring_swithch; // MMIENVSET_POWER_RING_SWITCH
    ENVSET_NAME_STR_INFO_T  name_str_t;         // MMIENV_SET_NAME  
    MMISET_ALARM_RING_INFO_T     alarm_ring_info;                   //��������������Ϣ
    MMISET_CALL_RING_INFO_T      call_ring_info[MMI_DUAL_SYS_MAX];  //�绰����������Ϣ
    MMISET_MSG_RING_INFO_T       msg_ring_info[MMI_DUAL_SYS_MAX];   //��Ϣ����������Ϣ
    uint8                 is_dial_tp_vibrator_on;
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    uint8                 is_dial_tone_off;
#endif	
    uint8                 is_tp_key_lock_on;   //key lock ring 
#ifdef MMI_READMENU_ENABLE
    uint8                 is_menu_read; //�˵��ʶ�
#endif
    uint8                 is_valid; // ���龰ģʽ�Ƿ���Ч
}MMIENVSET_SETTING_T;

typedef struct
{
    MN_DUAL_SYS_E     s_dual_sys;
    MMI_CTRL_ID_T     ctrl_id;
}MMIENVSET_ADD_DATA_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ���ָ����������ֵ
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetCurModeOptValue(
    MN_DUAL_SYS_E dual_sys,
    MMIENVSET_SET_OPT_E  set_opt
    );

/*****************************************************************************/
//  Description : �������ģʽ�õ��������� 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetActiveModeRingVol(
                MMISET_ALL_RING_TYPE_E ring_type
                            );


/*****************************************************************************/
//  Description : �������ģʽʹ�õ� ������ֵ
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetActiveModeOptValue(
    MN_DUAL_SYS_E dual_sys,
    MMIENVSET_SET_OPT_E  set_opt
    );

/*****************************************************************************/
// Description :��nvֵ�з�������ʹ�õĻ�������ģʽ 
//  Global resource dependence : none
// Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC uint8  MMIAPIENVSET_GetActiveModeId(void);

/*****************************************************************************/
//  Description : ��IDLE��������������
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetActiveModeForIdle(uint8 mode_id);
#if defined(MMI_WIDGET_SHORTCUT_SET) || defined(PDA_UI_DROPDOWN_WIN)
/*****************************************************************************/
//  Description : change to noise mode,means not silent mode
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_ChangeToNoiseMode(void);
/*****************************************************************************/
//  Description : change to silent moede 
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_ChangeToSilentMode(void);
#endif

/*****************************************************************************/
//Description : �ж��𶯼�ʱ���Ƿ��� 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMIENVSET_IsOnetimeVibTimerActive(void);
/*****************************************************************************/
//  Description : ��ù̶�����������id
//  Global resource dependence : none
//  Author: maqianlin
//  Note:������ʾ����ѡȡ��0��
/*****************************************************************************/
PUBLIC  uint16  MMIAPIENVSET_GetFixCallRingId(
                              MN_DUAL_SYS_E dual_sys,
                              uint8 mode_id
                              );
/*****************************************************************************/
//  Description : ��ù̶���Ϣ������id
//  Global resource dependence : none
//  Author: maqianlin
//  Note:������ʾ����ѡȡ��0��
/*****************************************************************************/
PUBLIC  uint16  MMIAPIENVSET_GetFixMsgRingId(
                              MN_DUAL_SYS_E dual_sys,
                              uint8 mode_id
                              );
/*****************************************************************************/
//  Description : get call ring info including file name and ring type
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_CALL_RING_T MMIAPIENVSET_GetCallRingInfo(MN_DUAL_SYS_E dual_sys,uint8 mode_id );
/*****************************************************************************/
//  Description : get msg ring info including file name and ring type
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_MSG_RING_T MMIAPIENVSET_GetMsgRingInfo(MN_DUAL_SYS_E dual_sys, uint8 mode_id );
/*****************************************************************************/
//  Description : ������������������󣬽���Ϣ����nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_SetMoreCallRingInfo(
                                          MN_DUAL_SYS_E dual_sys,
                                          MMISET_CALL_MORE_RING_T ring_info,
                                          uint8 mode_id
                                          );
/*****************************************************************************/
//  Description : �����������Ϣ�����󣬽���Ϣ����nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_SetMoreMsgRingInfo(
                                          MN_DUAL_SYS_E dual_sys,
                                          MMISET_CALL_MORE_RING_T ring_info,
                                          uint8 mode_id
                                          );
/*****************************************************************************/
//  Description : ������̶������󣬽�ID��Ϣ����nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetFixedCallRingId(
                                      MN_DUAL_SYS_E dual_sys,
                                      uint16 ring_id,
                                      uint8 mode_id
                                      );
/*****************************************************************************/
//  Description : ������̶������󣬽�ID��Ϣ����nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetFixedMsgRingId(
                                      MN_DUAL_SYS_E dual_sys,
                                      uint16 ring_id,
                                      uint8 mode_id
                                      );
/*****************************************************************************/
//  Description : ������̶������󣬽�ID��Ϣ����nv
//  Global resource dependence : 
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetFixedAlarmRingId(
                                      uint16 ring_id,
                                      uint8 mode_id
                                      );

/*****************************************************************************/
//  Description : ��nv�л�ȡ��Ӧ���龰ģʽ����
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_GetModeValue(
                                      uint16 cur_mode,
                                      MMIENVSET_SETTING_T *mode_setting_ptr //[OUT]
                                      );
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
// 	Description : set enhance ring state to nv
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetEnhanceRingState(BOOLEAN enhance_ring_state);

/*****************************************************************************/
// 	Description : get enhance ring state form nv
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_GetEnhanceRingState(void);
#endif
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
/*****************************************************************************/
// 	Description : set profile as earphone 
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_SetEarphoneMode(void);

/*****************************************************************************/
// 	Description : set profile as bluetooth
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_SetBluetoothMode(void);
/*****************************************************************************/
// 	Description : undo set profile as earphone 
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_UnSetEarphoneMode(void);
/*****************************************************************************/
// 	Description : set profile as bluetooth
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_UnSetBluetoothMode(void);
#endif
/*****************************************************************************/
// 	Description : ����龰ģʽ�²������񶯵�ֵ
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetModeDialTPVibrate(uint8 mode_id);

/*****************************************************************************/
//  Description :get pre active mode id
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetPreActiveMode(void);

/*****************************************************************************/
//  Description :get mute state
//  Global resource dependence : none
//  Author: jian.ma
//  Note:only valid for earphone and bluetooth mode
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_GetMuteState(void);

/*****************************************************************************/
//  Description :set mute state
//  Global resource dependence : none
//  Author: jian.ma
//  Note:only valid for earphone and bluetooth mode
/*****************************************************************************/
PUBLIC void  MMIAPIENVSET_SetMuteState(BOOLEAN state);

/*****************************************************************************/
//  Description : get mode opt value
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetModeOptValue(
                                   uint8 mode_id,
                                   MN_DUAL_SYS_E dual_sys,
                                   MMIENVSET_SET_OPT_E  set_opt
                                   );

/*****************************************************************************/
//  Description :get play ring mode id
//  Global resource dependence : none
//  Author: jian.ma
//  Note:if earphone or bt mode and in mute state,change mode to pre active mode
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetPlayRingModeId(void);

/*****************************************************************************/
// 	Description : ������������
//	 Global resource dependence : none
//  Author: maqianlin
//	 Note:
//*****************************************************************************/
void MMIAPIENVSET_ActiveMode(uint8 mode_id);

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : ������Ӧ���龰ģʽ����
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetModeValue(
                                      uint16 cur_mode,
                                      MMIENVSET_SETTING_T *mode_setting_ptr //[OUT]
                                      );

#endif

/*****************************************************************************/
//Description : �ж��𶯼�ʱ���Ƿ��� 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_GetVibStatus(void);

/*****************************************************************************/
//Description : �ж��𶯼�ʱ���Ƿ��� 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetVibStatus(BOOLEAN is_vibra_start);

/*****************************************************************************/
//  Description : �򿪻����������˵�                        
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_OpenMainMenuWindow(void);
/*****************************************************************************/
// 	Description : ��¼ǰһ�ε��龰ģʽ�����ⲿʹ��
//	 Global resource dependence : none
//  Author: juan.wu
//	 Note:
//*****************************************************************************/
PUBLIC void MMIENVSET_SetPreActiveMode(uint8 mode_id);
/*****************************************************************************/
//  Description : ����ָ���Ļ����������ֵ
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_SetCurModeOptValue(
                                  MMIENVSET_SET_OPT_E     set_opt,
                                  uint8				    set_value,
                                  MN_DUAL_SYS_E           dual_sys
                                  );
/*****************************************************************************/
//  Description : ��õ�ǰģʽ�õ��������� 
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetCurModeRingVol(
                              MMISET_ALL_RING_TYPE_E ring_type
                              );
/*****************************************************************************/
//  Description : ���Ԥ���õ�����  ID 
//	Global resource dependence : none
//  Author: maqianlin
//	Note:������ʾ����ѡȡ��0��
/*****************************************************************************/
uint8 MMIENVSET_GetActiveModeRingId(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMISET_ALL_RING_TYPE_E  ring_type
                                    );

/*****************************************************************************/
//  Description : ����ָ���Ļ����������ֵ
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void MMIENVSET_SetActiveModeOptValue(
                                     MMIENVSET_SET_OPT_E     set_opt,
                                     uint8				    set_value,
                                     MN_DUAL_SYS_E           dual_sys
                                     );
/*****************************************************************************/
//  Description : ��ý������õĻ���ģʽ
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
uint8 MMIENVSET_GetCurModeId(void);

/*****************************************************************************/
//  Description : ��Ĭ��ֵ,��ʼ��
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void MMISET_EnvSetInit(void);


/*****************************************************************************/
//  Description : �ָ�Ĭ��ֵ,��������
//	Global resource dependence : none
//  Author: maqianlin
//	Note:��mmiset_other.c��void MMISET_ResetFactorySetting(void)�е���
/*****************************************************************************/
void MMIENVSET_ResetEnvSetSetting(void);

/*****************************************************************************/
//Description : �ر��𶯼�ʱ��,�ü�ʱ����ʾÿ����
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StopOnetimeVibTimer(void);
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
/*****************************************************************************/
// 	Description : reset active mode setting
//	Global resource dependence : 
//  Author: juan.wu
//	Note:�رյ�ǰʹ��ģʽ��"������ʾ��"
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_CloseActModeOherRingSet(void);
/*****************************************************************************/
// 	Description : reset active mode setting
//	Global resource dependence : 
//  Author: juan.wu
//	Note:�ָ���ǰʹ��ģʽ"������ʾ����Ĭ��ֵ"
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_ResetActModeOtherRingSet(void);
/*****************************************************************************/
// 	Description : MMIENVSET_GetPowerSavENVMode
//	Global resource dependence : 
//  Author: juan.wu
//	Note:�ָ���ǰʹ��ģʽ"������ʾ����Ĭ��ֵ"
/*****************************************************************************/
PUBLIC uint8 MMIENVSET_GetPowerSavENVMode(void);
#endif
/*****************************************************************************/
// 	Description : close active mode setting
//	Global resource dependence : 
//  Author: yongsheng.wang
//	Note:�رյ�ǰʹ��ģʽ��key ring
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_CloseActModeKeyRingSet(void);
/*****************************************************************************/
// 	Description : reset active mode setting
//	Global resource dependence : 
//  Author: yongsheng.wang
//	Note:�ָ���ǰʹ��ģʽkey ring
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_ResetActModeKeyRingSet(void);

/*****************************************************************************/
//  Description : ��ý������õĻ���ģʽ
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void  MMIENVSET_SetCurModeId(uint8 mode_id);

/*****************************************************************************/
//  Description : ��ù̶�����������id
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC  uint16  MMIAPIENVSET_GetFixAlarmRingId( uint8 mode_id );

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
