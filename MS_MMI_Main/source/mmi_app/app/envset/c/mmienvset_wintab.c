/*****************************************************************************
** File Name:      Mmienvset_wintab.c                                        *
** Author:         maqianlin                                                 *
** Date:           13/12/2004                                                *
** Copyright:      2004 Gionee, Incorporated. All Rights Reserved.            *
** Description:    This file is used to hanlde Env Setting Window            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 13/12/2004     maqianlin        Create
******************************************************************************/

#define _MMIENVSET_WINTAB_C

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_envset_trc.h"
#ifdef WIN32
#include    "std_header.h"
#endif
#include    "window_parse.h"
#include    "mmidisplay_data.h"  
#include    "guilistbox.h"       
#include    "mmk_type.h"
#include    "mmi_nv.h"
#include    "guimenu.h"
#include    "mmimp3_export.h"
#include    "mmi_menutable.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmienvset_export.h"
#include    "mmiset_text.h"
#include    "mmiset_id.h"
//#include    "mmiset_nv.h"
#include    "mmiset_menutable.h"
#include    "mmienvset_internal.h"
//#include    "mmiset_wintab.h"
#include    "mmiset_image.h"
//#include    "guibutton.h"
//#include    "mmiset_set_ring.h"
#include    "mmipub.h"
#include    "mmi_default.h"
//#include    "mmienvset_position.h"
#include "mmi_mainmenu_export.h"
#include    "mmisms_export.h"
#include    "mmipb_export.h"
#include    "guires.h"
#include	"guiedit.h"
#include    "guiform.h"
#include    "guilabel.h"
#include    "guisetlist.h"
//#include    "mmiset_call.h"
#include "mmiset_call_export.h"
#include  "mmiset_func.h"
#include 	"mmi_appmsg.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmicom_panel.h"
#include "mmicc_export.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

//for dual sim
#define MMIENVSET_SIM_MAX_NUM           MMI_DUAL_SYS_MAX
#define CALL_VIBRATE_TIME               2


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
//CR244608 2011-5-23
typedef enum
{
    MMIEVN_OPTMENU_NODE_NULL,
    MMIEVN_OPTMENU_NODE_ACTIVE,
    MMIEVN_OPTMENU_NODE_SET,     
    MMIEVN_OPTMENU_NODE_ADD,     
    MMIEVN_OPTMENU_NODE_DEL
} MMIEVN_OPTMENU_NODE_TYPE_E;

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/
//extern uint8   g_mmiset_vibra_timer_id;    //设置中振动器振动的时间id 

//extern MMIENVSET_SETTING_T * g_mmienvset_mode_setting_ptr;
extern const MMI_TEXT_ID_T g_mmienvset_text_id[];

/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MN_DUAL_SYS_E             s_dual_sys = MN_DUAL_SYS_MAX; //选择的sim卡：卡1，卡2
LOCAL MMIENVSET_SET_OPT_E       s_ring_type = 0; /*lint !e64*///需要设置的铃声类型：来电铃声类型，信息铃声类型
LOCAL MMISET_ALL_RING_TYPE_E    s_set_ring = MMISET_RING_TYPE_CALL;//用于音量预览，意义同s_ring_type，但属于不同枚举
LOCAL int32 s_g_vibrate = 0;
/*****************************************************************************/
//Description :来电提示方式的选择窗口处理函数 
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleCallRingTypeWindow(
                                             MMI_WIN_ID_T    win_id,  // 窗口ID
                                             MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                             DPARAM             param    // 消息参数
                                             );

/*****************************************************************************/
//Description : 信息提示方式的选择窗口处理函数
//Global resource dependence : 
//Author:maqianlin
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMsgRingTypeWindow(
                                            MMI_WIN_ID_T    win_id,  // 窗口ID
                                            MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                            DPARAM             param    // 消息参数
                                            );

/*****************************************************************************/
//Description :  闹钟提示方式的选择窗口处理函数
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleAlarmRingTypeWindow(
                                              MMI_WIN_ID_T    win_id,  // 窗口ID
                                              MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                              DPARAM             param    // 消息参数
                                              );

/*****************************************************************************/
//Description :  按键音的选择窗口处理函数
//Global resource dependence : 
//Author:  maqianlin
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleKeyRingTypeWindow(
                                            MMI_WIN_ID_T    win_id,  // 窗口ID
                                            MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                            DPARAM             param    // 消息参数
                                            );

#ifdef LCD_SLIDE_SUPPORT

/*****************************************************************************/
//Description : // 滑盖音的选择窗口处理函数
//Global resource dependence : 
//Author:  jianhuiluo
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSlideRingSwitchWindow(
                                                     MMI_WIN_ID_T    win_id,  // 窗口ID
                                                     MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                     DPARAM             param    // 消息参数
                                                     );
/*****************************************************************************/
//Description : // 开盖音的选择窗口处理函数
//Global resource dependence : 
//Author:  jianhuiluo
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSlideRingOnWindow(
                                                     MMI_WIN_ID_T    win_id,  // 窗口ID
                                                     MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                     DPARAM             param    // 消息参数
                                                     );

/*****************************************************************************/
//Description : // 合盖音的选择窗口处理函数
//Global resource dependence : 
//Author:  jianhuiluo
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleSlideRingOffWindow(
                                                     MMI_WIN_ID_T    win_id,  // 窗口ID
                                                     MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                     DPARAM             param    // 消息参数
                                                     );

#endif

//*****************************************************************************/
//Description :  触屏音的选择窗口处理函数
//Global resource dependence : 
//Author: jian.ma
//Note: 
/*****************************************************************************/

LOCAL     MMI_RESULT_E  HandleTKeyRingTypeWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                );

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone. 
/*****************************************************************************/
//Description : 翻盖提示音的选择窗口处理函数
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleFlipRingTypeWindow(
                                             MMI_WIN_ID_T    win_id,  // 窗口ID
                                             MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                             DPARAM             param    // 消息参数
                                             );
/*****************************************************************************/
//Description : 翻盖提示音的选择窗口处理函数
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/

LOCAL     MMI_RESULT_E  HandleFlipDownRingTypeWindow(
                                                 MMI_WIN_ID_T    win_id,  // 窗口ID
                                                 MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                 DPARAM             param    // 消息参数
                                                 );

/*****************************************************************************/
//Description :按键音的参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SetFlipRingParam(void);
#endif

/*****************************************************************************/
//Description : 开关机铃声的选择窗口处理函数
//Global resource dependence : 
//Author:  maqianlin
//Note: 
/*****************************************************************************/

//LOCAL MMI_RESULT_E  HandlePowerRingTypeWindow(
//                                              MMI_WIN_ID_T    win_id,  // 窗口ID
//                                              MMI_MESSAGE_ID_E   msg_id,  // 消息ID
//                                              DPARAM             param    // 消息参数
//                                               );
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description : // 下载完成提示音的选择窗口处理函数
//Global resource dependence : 
//Author:  Jiaoyou.wu
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleDownloadNotifySwitchWindow(
                                                     MMI_WIN_ID_T    win_id,  // 窗口ID
                                                     MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                     DPARAM             param    // 消息参数
                                                     );

/*****************************************************************************/
//Description : // 低电压警告音的选择窗口处理函数
//Global resource dependence : 
//Author:  maqianlin
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleLowVoltageWarnSwitchWindow(
                                                     MMI_WIN_ID_T    win_id,  // 窗口ID
                                                     MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                     DPARAM             param    // 消息参数
                                                     );


#endif
/*****************************************************************************/
//Description : 环境选择列表的窗口处理函数
//Global resource dependence : none
//Author:maqianlin 
//Note: 在正在使用的环境列表项前打勾
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleEnvSetMainMenuWindow(
                                               MMI_WIN_ID_T    win_id, //窗口ID
                                               MMI_MESSAGE_ID_E   msg_id, //消息ID
                                               DPARAM             param   //消息的参数
                                               );

#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description : 开关机声音开启和关闭的窗口处理函数
//Global resource dependence : 
//Author: kelly.li
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePowerRingSwitchWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                );

/*****************************************************************************/
//Description : 增强声音开启和关闭的窗口处理函数
//Global resource dependence : 
//Author: kelly.li
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnhanceRingSwitchWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                );
/*****************************************************************************/
//Description :操作菜单窗口处理函数(启动、个性化设置以及修改名称) 
//Global resource dependence : none
//Author:maqianlin 
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnvSetOperationWindow(
                                                MMI_WIN_ID_T    win_id, //窗口ID
                                                MMI_MESSAGE_ID_E   msg_id, //消息ID
                                                DPARAM             param   //消息的参数
                                                );
#endif
/*****************************************************************************/
// Description : prompt the success win
//  Global resource dependence : none
// Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void PromptSuccessWin(void);

/*****************************************************************************/
// Description : 对个性化设置子菜单以及启动、修改名字设置的处理 
//  Global resource dependence : none
// Author: maqianlin
//  Note:
/*****************************************************************************/
LOCAL void   HandleOperationMenu(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//Description :   修改环境名称的EditBox
//Global resource dependence :  none
//Author: maqianlin
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleChangeEnvNameWindow(
                                              MMI_WIN_ID_T    win_id,   // 窗口ID
                                              MMI_MESSAGE_ID_E   msg_id,   // 消息ID
                                              DPARAM             param     // 消息参数
                                              );

/*****************************************************************************/
//Description :   修改环境名称的EditBox
//Global resource dependence :  none
//Author: maqianlin
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingOptWindow(
                                           MMI_WIN_ID_T    win_id,   // 窗口ID
                                           MMI_MESSAGE_ID_E   msg_id,   // 消息ID
                                           DPARAM             param     // 消息参数
                                           );
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description : 所有铃声音量处理函数
//Global resource dependence : 
//Author: maqianlin
//Note:被HandleRingVolumeWindow调用 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllRingVolWindow(
                                           MMI_WIN_ID_T            win_id,               // 窗口ID
                                           MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                           MMISET_ALL_RING_TYPE_E     ring_type,
                                           MMIENVSET_SET_OPT_E        set_opt,
                                           DPARAM                     param
                                           );


/*****************************************************************************/
//Description : 铃声音量处理窗口
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRingVolumeWindow(
                                          MMI_WIN_ID_T            win_id,               // 窗口ID
                                          MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                          DPARAM                     param
                                          );

/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  EnvSetSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param);
#endif
/*****************************************************************************/
//  Description : 调节音量
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void MMIENVSET_AdjustAudioVol(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    uint8                   volume
                    );

/*****************************************************************************/
//Description : 音量调节窗口:来电铃声，信息铃声...都在这个窗口进
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllRingVolumeWindow(
                                          MMI_WIN_ID_T            win_id,               // 窗口ID
                                          MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                          DPARAM                     param
                                          );
/*****************************************************************************/
//Description : 设置音量调节label的参数
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL  void  SetAllRingVolParam(void);

/*****************************************************************************/
//Description : 响铃方式窗口处理函数
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRingTypeSetWindow(
                                          MMI_WIN_ID_T            win_id,               // 窗口ID
                                          MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                          DPARAM                     param
                                          );
/*****************************************************************************/
//Description : 设置响铃方式的控件值
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SetRingTypeParam(MMI_WIN_ID_T         win_id);

/*****************************************************************************/
//Description : other ring set 
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOtherRingSetWindow(
                                          MMI_WIN_ID_T            win_id,               // 窗口ID
                                          MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                          DPARAM                     param
                                          );
/*****************************************************************************/
//Description : :按键音的参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SetKeypadRingParam(void);

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
/*****************************************************************************/
//Description : 拨号音参数设置
//Global resource dependence : 
//Author:jixin.xu
//Note: 
/*****************************************************************************/
LOCAL void  SetDialTone(void);
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//Description : 支持触摸屏的情况下，触屏音参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void  SetTouchpanelRingParam(void);


/*****************************************************************************/
//Description : 支持触摸屏的情况下，拨号盘触屏振动参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void  SetDialPanelTouchVibrate(void);

/*****************************************************************************/
//Description : 支持触摸屏的情况下，屏幕锁定提示音参数设置
//Global resource dependence : 
//Author:jian.ma
//Note: 
/*****************************************************************************/
LOCAL void SetTouchKeyLockRingParam(void);
#endif

#ifdef MMI_READMENU_ENABLE
/*****************************************************************************/
//Description : 支持菜单朗读的情况下，菜单朗读开关设置
//Global resource dependence : 
//Author:violent.wei
//Note: 
/*****************************************************************************/
LOCAL void  SetMenuRead(void);
#endif


/*****************************************************************************/
//Description : 开关机铃声参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SetPowerOnOffRingParam(void);

/*****************************************************************************/
//Description : 低电压告警铃声参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
 LOCAL void  SetLowBatteryRingParam(void);    

/*****************************************************************************/
//Description : 保存所有设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SaveOtherRingTypeSet(void);
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description : 修改好铃声音量后，设置label的内容
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void   SetVolumeLabelcontent(MMIENVSET_SET_OPT_E     set_opt,uint8  s_cur_vol);
#endif
//CR244608 2011-5-23
/*****************************************************************************/
//  Description :情景模式选项菜单操作
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnvOptMenuWindow(
                                           MMI_WIN_ID_T        win_id,        //IN:
                                           MMI_MESSAGE_ID_E msg_id,        //IN:
                                           DPARAM            param        //IN:
                                           );

/*****************************************************************************/
//  Description :get add index
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetEnvActionIndex(
                                uint8 active_index //当前激活的模式
                                );

#if defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get add index
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/

LOCAL uint8 GetEnvValidIndex(void);
/*****************************************************************************/
//  Description : insert dyna menu node by label
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T      text_id,       //插入的节点的文本
                             uint16             node_id,        //节点编号
                             uint16             parent_node_id, //父节点编号
                             uint16             index,           //位置
                             BOOLEAN            is_grayed        //是否灰显
                             );

/*****************************************************************************/
//Description :open env setting win
//Global resource dependence : none
//Author:jian.ma
//Note: 
/*****************************************************************************/
LOCAL void OpenEnvSettingWin(
                             MMI_MESSAGE_ID_E   msg_id
                             );


/*****************************************************************************/
//  Description : handle env setting context menu window message
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnvSettingWinMsg(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        );
#endif

/*****************************************************************************/
// Description: 调节来电铃声音量面板的回调函数
// Global resource dependence: NONE
// Author: 
// Note:
/*****************************************************************************/
LOCAL void SetRingVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr);

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
#if defined MMI_PDA_SUPPORT

//setting option menu
WINDOW_TABLE( MMIENV_SETTING_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEnvSettingWinMsg),
    WIN_ID(MMIENVSET_SETTING_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif

//情景模式主窗口
WINDOW_TABLE( MMIENVSET_MAIN_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32) HandleEnvSetMainMenuWindow),
    WIN_ID( MMIENVSET_MAIN_MENU_WIN_ID ),
    WIN_TITLE( TXT_SETTINGS_ENVIONMENT ),       
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_MAIN_MENU_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION, TXT_ACTIVE_OPERATIVE, STXT_RETURN),
    END_WIN
};

//情景模式的选项菜单
WINDOW_TABLE( MMIENVSET_MAIN_OPT_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEnvOptMenuWindow),
    WIN_ID( MMIENVSET_MAIN_OPT_WIN_ID ),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN

};

#ifndef MMI_ENVSET_MINI_SUPPORT
//创建操作菜单(启动、设置以及改名称) 
WINDOW_TABLE( MMIENVSET_OPERATION_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEnvSetOperationWindow),
    WIN_ID(MMIENVSET_OP_POPMENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),

	
//added by lxf for dphone
#ifdef  MAINLCD_SIZE_128X64		
    CREATE_MENU_CTRL(MENU_ENVSET_OPERATION,MENU_POPUP_CTRL_ID),
#else
    CREATE_POPMENU_CTRL(MENU_ENVSET_OPERATION,MENU_POPUP_CTRL_ID),
#endif

    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};
#endif
//创建操作菜单(启动、设置以及改名称) 
WINDOW_TABLE( MMIENVSET_SETTING_OPT_WIN_TAB ) =
{
    //窗口处理函数
    WIN_FUNC((uint32)HandleSettingOptWindow ),
    //win_id
    WIN_ID( MMIENVSET_OPERATION_WIN_ID),
    //窗口的标题
    WIN_TITLE( STXT_MAIN_SETTING ),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    //创建控件
    CREATE_MENU_CTRL(MENU_ENVSET_SETTING,MMIENVSET_SETTING_MENU_CTRL_ID),
    
    //结束宏定义
    END_WIN   
        
};

// 改名称 EditbBox 窗口
WINDOW_TABLE(MMIENVSET_CHANGE_ENV_NAME_WIN_TAB) = 
{
    //窗口处理函数
    WIN_FUNC((uint32)HandleChangeEnvNameWindow),
    //win_id
    WIN_ID(MMIENVSET_CHANGE_ENV_NAME_WIN_ID),
    //窗口的标题
    WIN_TITLE(TXT_COMMON_RENAME ),

#ifdef DPHONE_SUPPORT
    CREATE_EDIT_TEXT_CTRL(MMIENVSET_ENVNAME_MAX_LEN, MMIENVSET_CHANGE_ENV_NAME_EDITBOX_CTRL_ID) ,
#else      
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIENVSET_CHANGE_NAME_FORM_CTRL_ID),
    CHILD_EDIT_TEXT_CTRL(TRUE, MMIENVSET_ENVNAME_MAX_LEN, MMIENVSET_CHANGE_ENV_NAME_EDITBOX_CTRL_ID, MMIENVSET_CHANGE_NAME_FORM_CTRL_ID),
#ifdef MMI_PDA_SUPPORT
    CHILD_SOFTKEY_CTRL(STXT_SAVE, TXT_NULL, STXT_RETURN,MMICOMMON_SOFTKEY_CTRL_ID,MMIENVSET_CHANGE_NAME_FORM_CTRL_ID),
#endif
#endif
    WIN_SOFTKEY(TXT_NULL, TXT_NULL, STXT_RETURN),
    END_WIN
};
#ifndef MMI_ENVSET_MINI_SUPPORT
//铃声音量大小窗口
WINDOW_TABLE(MMIENVSET_RING_VOL_WIN_TAB) = 
{
    //清屏
    //CLEAR_LCD,    
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //  WIN_BACKGROUND_ID(IMAGE_LIST_BG),
        //窗口处理函数
        WIN_FUNC((uint32)HandleRingVolumeWindow),
        //win_id
        WIN_ID(MMIENVSET_RING_VOL_WIN_ID),
        WIN_TITLE(TXT_NULL),
#ifdef MMI_PDA_SUPPORT
        WIN_STYLE(WS_HAS_BUTTON_SOFTKEY),
#endif
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        
        //结束宏定义
        END_WIN
};
#endif
//音量调节窗口
WINDOW_TABLE(MMIENVSET_ALL_RING_VOL_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleAllRingVolumeWindow),
    //win_id
    WIN_ID(MMIENVSET_ALL_RING_VOL_WIN_ID),
    WIN_TITLE(TXT_RING_VOLUME),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_CALL_RING_VOL_SET_FORM_CTRL_ID, MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_CALL_RING_VOL_SET_LABEL_CTRL_ID, MMIENVSET_CALL_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_CALL_RING_VOL_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_CALL_RING_VOL_SET_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_MSG_RING_VOL_SET_FORM_CTRL_ID, MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_MSG_RING_VOL_SET_LABEL_CTRL_ID, MMIENVSET_MSG_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_MSG_RING_VOL_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_MSG_RING_VOL_SET_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_ALARM_RING_VOL_SET_FORM_CTRL_ID, MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_ALARM_RING_VOL_SET_LABEL_CTRL_ID, MMIENVSET_ALARM_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_ALARM_RING_VOL_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_ALARM_RING_VOL_SET_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_COM_RING_VOL_SET_FORM_CTRL_ID, MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_COM_RING_VOL_SET_LABEL_CTRL_ID, MMIENVSET_COM_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_COM_RING_VOL_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_COM_RING_VOL_SET_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_POWER_ONOFF_RING_VOL_SET_FORM_CTRL_ID, MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_POWER_ONOFF_RING_VOL_SET_LABEL_CTRL_ID, MMIENVSET_POWER_ONOFF_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_POWER_ONOFF_RING_VOL_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_POWER_ONOFF_RING_VOL_SET_FORM_CTRL_ID),

#ifdef MMI_READMENU_ENABLE
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_MENU_READ_VOL_SET_FORM_CTRL_ID, MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_MENU_READ_VOL_SET_LABEL_CTRL_ID, MMIENVSET_MENU_READ_VOL_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_MENU_READ_VOL_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_MENU_READ_VOL_SET_FORM_CTRL_ID),
#endif

    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
     WIN_STYLE(WS_DISPATCH_TO_CHILDWIN),  
    //结束宏定义
    END_WIN
};

//响铃方式窗口
WINDOW_TABLE(MMIENVSET_RING_TYPE_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleRingTypeSetWindow),
    //win_id
    WIN_ID(MMIENVSET_RING_TYPE_WIN_ID),
    WIN_TITLE(TXT_SET_RING_TYPE),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIENVSET_RING_TYPE_FORM_CTRL_ID),
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
    //结束宏定义
    END_WIN
};


WINDOW_TABLE(MMIENVSET_OTHER_RING_SET_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleOtherRingSetWindow),
    //win_id
    WIN_ID(MMIENVSET_OTHER_RING_SET_WIN_ID),
    WIN_TITLE(TXT_SET_RING_OTHER),
    CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        //key ring
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_KEYPAD_RING_SET_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_KEYPAD_RING_SET_LABEL_CTRL_ID, MMIENVSET_KEYPAD_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_KEYPAD_RING_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_KEYPAD_RING_SET_FORM_CTRL_ID),
        //tp ring
#ifdef TOUCH_PANEL_SUPPORT 
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_TOUCH_PANEL_RING_SET_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_TOUCH_PANEL_RING_SET_LABEL_CTRL_ID, MMIENVSET_TOUCH_PANEL_RING_SET_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIENVSET_TOUCH_PANEL_RING_SET_SETLIST_CTRL_ID, MMIENVSET_TOUCH_PANEL_RING_SET_FORM_CTRL_ID),
#endif
        //low battery ring
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_LOW_BATTERY_RING_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_LOW_BATTERY_RING_LABEL_CTRL_ID, MMIENVSET_LOW_BATTERY_RING_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIENVSET_LOW_BATTERY_RING_SETLIST_CTRL_ID, MMIENVSET_LOW_BATTERY_RING_FORM_CTRL_ID),
        //power on/off ring
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_POWER_ONOFF_RING_SET_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_POWER_ONOFF_RING_SET_LABEL_CTRL_ID, MMIENVSET_POWER_ONOFF_RING_SET_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIENVSET_POWER_ONOFF_RING_SETLIST_CTRL_ID, MMIENVSET_POWER_ONOFF_RING_SET_FORM_CTRL_ID),
        //key lock ring
#ifdef TOUCH_PANEL_SUPPORT
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_TOUCH_KEY_LOCK_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_TOUCH_KEY_LOCK_LABEL_CTRL_ID, MMIENVSET_TOUCH_KEY_LOCK_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIENVSET_TOUCH_KEY_LOCK_SETLIST_CTRL_ID, MMIENVSET_TOUCH_KEY_LOCK_FORM_CTRL_ID),
#endif

    //dial tp vibrate
#ifdef TOUCH_PANEL_SUPPORT
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_TOUCH_VIBRATE_SET_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_TOUCH_VIBRATE_SET_LABEL_CTRL_ID, MMIENVSET_TOUCH_VIBRATE_SET_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIENVSET_TOUCH_VIBRATE_SETLIST_CTRL_ID, MMIENVSET_TOUCH_VIBRATE_SET_FORM_CTRL_ID),
#endif
//dial tone
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_DIAL_TONE_SET_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_DIAL_TONE_SET_LABEL_CTRL_ID, MMIENVSET_DIAL_TONE_SET_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIENVSET_DIAL_TONE_SETLIST_CTRL_ID, MMIENVSET_DIAL_TONE_SET_FORM_CTRL_ID),
#endif
#ifdef MMI_READMENU_ENABLE
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_MENU_READ_SET_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_MENU_READ_SET_LABEL_CTRL_ID, MMIENVSET_MENU_READ_SET_FORM_CTRL_ID),
        CHILD_SETLIST_CTRL(TRUE, MMIENVSET_MENU_READ_SETLIST_CTRL_ID, MMIENVSET_MENU_READ_SET_FORM_CTRL_ID),
#endif
#ifdef FLIP_PHONE_SUPPORT
        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_FLIP_RING_SET_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_FLIP_RING_SET_LABEL_CTRL_ID, MMIENVSET_FLIP_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_FLIP_RING_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_FLIP_RING_SET_FORM_CTRL_ID),

        CHILD_FORM_CTRL(TRUE, GUIFORM_LAYOUT_ORDER, MMIENVSET_FLIP_DOWN_RING_SET_FORM_CTRL_ID, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENVSET_FLIP_DOWN_RING_SET_LABEL_CTRL_ID, MMIENVSET_FLIP_DOWN_RING_SET_FORM_CTRL_ID),
        CHILD_LABEL_CTRL(GUILABEL_ALIGN_DEFAULT, TRUE, MMIENVSET_FLIP_DOWN_RING_SET_EDIT_LABEL_CTRL_ID, MMIENVSET_FLIP_DOWN_RING_SET_FORM_CTRL_ID),
#endif
    WIN_SOFTKEY(STXT_SELECT, TXT_NULL, STXT_FINISH),
    //结束宏定义
    END_WIN
};


//来电提示方式
WINDOW_TABLE(MMIENVSET_CALL_RING_TYPE_WIN_TAB) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleCallRingTypeWindow),
        //win_id
        WIN_ID(MMIENVSET_CALL_RING_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_CALL_TYPE),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_CALL_RING_TYPE_LISTBOX__CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

//短信提示方式    
WINDOW_TABLE( MMIENVSET_MSG_RING_TYPE_WIN_TAB ) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleMsgRingTypeWindow),
        //win_id
        WIN_ID( MMIENVSET_MSG_RING_TYPE_WIN_ID ),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_MSG_TYPE),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_MSG_RING_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

// 闹钟提示方式的选择窗口
WINDOW_TABLE( MMIENVSET_ALARM_RING_TYPE_WIN_TAB ) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleAlarmRingTypeWindow),
        //win_id
        WIN_ID(MMIENVSET_ALARM_RING_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ENVSET_ALARM_RING_TYPE),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_RING_ALARM_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

// 按键音的选择窗口
WINDOW_TABLE(MMIENVSET_KEY_RING_TYPE_WIN_TAB ) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleKeyRingTypeWindow),
        //win_id
        WIN_ID(MMIENVSET_KEY_RING_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_KEY_TONE),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_KEY_RING_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};
#ifdef FLIP_PHONE_SUPPORT
WINDOW_TABLE(MMIENVSET_FLIP_RING_TYPE_WIN_TAB ) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleFlipRingTypeWindow),
        //win_id
        WIN_ID(MMIENVSET_FLIP_RING_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_FLIP_RING),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_FLIP_RING_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};
WINDOW_TABLE(MMIENVSET_FLIP_DOWN_RING_TYPE_WIN_TAB ) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleFlipDownRingTypeWindow),
        //win_id
        WIN_ID(MMIENVSET_FLIP_DOWN_RING_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_FLIP_DOWN_RING),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_FLIP_DOWN_RING_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};
#endif
// 触屏音的选择窗口
WINDOW_TABLE(MMIENVSET_TKEY_RING_TYPE_WIN_TAB ) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleTKeyRingTypeWindow),
        //win_id
        WIN_ID(MMIENVSET_TKEY_RING_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_TKEY_TONE),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_TKEY_RING_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

/*#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone. 
// 翻盖提示音的选择窗口
WINDOW_TABLE(MMIENVSET_FLIP_RING_TYPE_WIN_TAB ) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleFlipRingTypeWindow),
        //win_id
        WIN_ID(MMIENVSET_FLIP_RING_TYPE_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_FLIP_RING),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_FLIP_RING_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};
#endif*/

// 开关机铃声的选择窗口
//WINDOW_TABLE(MMIENVSET_POWER_RING_TYPE_WIN_TAB ) = 
//{
//    //清屏
//    //CLEAR_LCD,
//        //优先级
//        //WIN_PRIO( WIN_ONE_LEVEL ),
//        //窗口处理函数
//        WIN_FUNC((uint32)HandlePowerRingTypeWindow),
//        //win_id
//        WIN_ID(MMIENVSET_POWER_RING_TYPE_WIN_ID),
//        //窗口的标题
//        WIN_TITLE(TXT_ENVSET_POWER_RING_TYPE),
//        //创建  控件
//        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_POWER_RING_TYPE_LISTBOX_CTRL_ID),
//        //结束宏定义
//        END_WIN
// };
#ifndef MMI_ENVSET_MINI_SUPPORT
//下载成功提示音开启和关闭
WINDOW_TABLE(MMIENVSET_DOWNLOAD_NOTIFY_SWICTH_WIN_TAB) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleDownloadNotifySwitchWindow),
        //win_id
        WIN_ID(MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_DOWNLOAD_NOTIFY),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

//低电 报警声音开启和关闭
WINDOW_TABLE(MMIENVSET_LOW_VOLTAGE_WARN_SWICTH_WIN_TAB) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleLowVoltageWarnSwitchWindow),
        //win_id
        WIN_ID(MMIENVSET_LOW_VOLTAGE_WARN_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_VOLTAGE_WARNING),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_LOW_VOLTAGE_WARN_SWITCH_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};


//开关机声音开启关闭选择窗口
WINDOW_TABLE(MMIENVSET_POWER_RING_SWICTH_WIN_TAB) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandlePowerRingSwitchWindow),
        //win_id
        WIN_ID(MMIENVSET_POWER_RING_SWICTH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_ENVSET_POWER_RING_TYPE),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_POWER_RING_SWICTH_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

//增强铃声开启关闭选择窗口
WINDOW_TABLE(MMIENVSET_ENHANCE_RING_SWICTH_WIN_TAB) = 
{
        //窗口处理函数
        WIN_FUNC((uint32)HandleEnhanceRingSwitchWindow),
        //win_id
        WIN_ID(MMIENVSET_ENHANCE_RING_SWICTH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_ENHANCE_RING),
        //创建  控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_ENHANCE_RING_SWICTH_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};
#endif
/*	//Sam.hua	use MMISET_SelectSimFunc 
// for dual_sim
WINDOW_TABLE( MMIENVSET_SELECT_SIM_WIN_TAB ) = 
{
    //CLEAR_LCD,
        //WIN_PRIO( WIN_ONE_LEVEL ),
        WIN_FUNC((uint32)HandleEnvSetSelectSimWinMsg),    
        WIN_ID(MMIENVSET_SELECT_SIM_WIN_ID),
        WIN_TITLE(TXT_SIM_SEL),
        CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIENVSET_SELECT_SIM__CTRL_ID),
        WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
        END_WIN
};
*/
#ifdef LCD_SLIDE_SUPPORT
//滑盖音开合
WINDOW_TABLE(MMIENVSET_SLIDE_RING_TYPE_WIN_TAB) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleSlideRingSwitchWindow),
        //win_id
        WIN_ID(MMIENVSET_SLIDE_RING_SWITCH_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_RING_SLIDE_TONE),
        //创建 控件
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        CREATE_MENU_CTRL(MENU_ENVSET_SLIDE_TONE_OPTION,MMIENVSET_SLIDE_TONE_MENU_CTRL_ID),
        //结束宏定义
        END_WIN
};

//开盖音
WINDOW_TABLE(MMIENVSET_SLIDE_RING_ON_TYPE_WIN_TAB) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleSlideRingOnWindow),
        //win_id
        WIN_ID(MMIENVSET_SLIDE_RING_ON_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_SLIDE_TONE_ENABLE),
        //创建 控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_SLIDE_RING_ON_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

//合盖音
WINDOW_TABLE(MMIENVSET_SLIDE_RING_OFF_TYPE_WIN_TAB) = 
{
    //清屏
    //CLEAR_LCD,
        //优先级
        //WIN_PRIO( WIN_ONE_LEVEL ),
        //窗口处理函数
        WIN_FUNC((uint32)HandleSlideRingOffWindow),
        //win_id
        WIN_ID(MMIENVSET_SLIDE_RING_OFF_WIN_ID),
        //窗口的标题
        WIN_TITLE(TXT_SET_SLIDE_TONE_DISABLE),
        //创建 控件
        CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENVSET_SLIDE_RING_OFF_TYPE_LISTBOX_CTRL_ID),
        WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
        //结束宏定义
        END_WIN
};

#endif

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
//2011-5-23
/*****************************************************************************/
//  Description :get add index
//  Global resource dependence : 
//  Author:panvs.zhong
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetEnvAddIndex(void)
{
    MMIENVSET_SETTING_T    mode_setting = {0};
    uint8 i = 0;
    
    for(i=MMIENVSET_USER_MODE1; i<MMIENVSET_TOTAL_MODE; i++)
    {
        SCI_MEMSET(&mode_setting,0,sizeof(MMIENVSET_SETTING_T));
        MMIAPIENVSET_GetModeValue(i, &mode_setting);
        if( 0 == mode_setting.name_str_t.wstr_len )  
        {
            break;
        }
    }

    return i;
}

/*****************************************************************************/
//  Description :insert dynamenu node by label
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL void InsertOptMenuNode(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T   	text_id,        //插入的节点的文本
                             uint16          	node_id,        //节点编号
                             uint16          	parent_node_id, //父节点编号
#ifdef MMI_PDA_SUPPORT
                             MMI_IMAGE_ID_T     image_id,
#endif                             
                             uint16          	index,          //位置
                             BOOLEAN            is_graged
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = is_graged;
#ifdef MMI_PDA_SUPPORT
    node_item.select_icon_id = image_id;
#endif    
    GUIMENU_InsertNode(index,node_id,parent_node_id,&node_item,ctrl_id);
}

/*****************************************************************************/
//  Description : 打开情景模式选项菜单
//  Global resource dependence : none
//  Author: Panvs.Zhong
//  Note: 
/*****************************************************************************/
LOCAL void  OpenEnvOptMenuWin(
                                            uint8 item_count,//IN 当前总数
                                            uint8 cur_index//IN 当前光标位置
                                            )
{
    MMI_CTRL_ID_T    ctrl_id = MMIENVSET_MAIN_OPT_CTRL_ID;
    uint16          i = 0;
    BOOLEAN is_gray = FALSE;

    if (MMK_IsOpenWin(MMIENVSET_MAIN_OPT_WIN_ID))
    {
        MMK_CloseWin(MMIENVSET_MAIN_OPT_WIN_ID);
    }
    MMK_CreateWin((uint32 *)MMIENVSET_MAIN_OPT_WIN_TAB, (ADD_DATA)cur_index);
    
    //creat dynamic menu
    GUIMENU_CreatDynamic(PNULL, MMIENVSET_MAIN_OPT_WIN_ID, ctrl_id, GUIMENU_STYLE_POPUP);
        
    //add
    if (item_count < MMIENVSET_TOTAL_MODE)
    {
        is_gray = FALSE;
    }
    else
    {
        is_gray = TRUE;
    }    
    InsertOptMenuNode(
                    ctrl_id,
                    TXT_ADDNEW, 
                    MMIEVN_OPTMENU_NODE_ADD,
                    0,
#ifdef MMI_PDA_SUPPORT                        
                    IMAGE_COMMON_OPTMENU_ADD,
#endif                        
                    i++,
                    is_gray
                    );
    
    //del
    if (cur_index >= MMIENVSET_USER_MODE1)
    {
        is_gray = FALSE;
    }
    else
    {
        is_gray = TRUE;
    }
    InsertOptMenuNode(
                    ctrl_id,
                    TXT_DELETE,
                    MMIEVN_OPTMENU_NODE_DEL,
                    0,
#ifdef MMI_PDA_SUPPORT                        
                    IMAGE_COMMON_OPTMENU_REMOVE_ALL,
#endif                        
                    i++,
                    is_gray
                    );
    
    //set
    InsertOptMenuNode(
                    ctrl_id,
                    STXT_MAIN_SETTING, 
                    MMIEVN_OPTMENU_NODE_SET,
                    0,
#ifdef MMI_PDA_SUPPORT                      
                    IMAGE_COMMON_OPTMENU_SET,
#endif                    
                    i++,
                    FALSE
                    );
}

/*****************************************************************************/
//  Description :情景模式选项菜单操作
//  Global resource dependence : none
//  Author: Panvs.Zhong
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnvOptMenuWindow(
                                           MMI_WIN_ID_T        win_id,        //IN:
                                           MMI_MESSAGE_ID_E msg_id,        //IN:
                                           DPARAM            param        //IN:
                                           )
{
    MMI_RESULT_E            recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T          ctrl_id  = MMIENVSET_MAIN_OPT_CTRL_ID;
    uint8                           cur_select_index = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
        case MSG_OPEN_WINDOW:    
            MMK_SetAtvCtrl(win_id, ctrl_id);
            break;
            
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_CTL_MIDSK:
        case MSG_CTL_OK:
        {
            switch (GUIMENU_GetCurNodeId(ctrl_id))
            {
                case MMIEVN_OPTMENU_NODE_SET:
                    {
                        // set
                        MMIENVSET_SetCurModeId((uint8)MMIENVSET_GetEnvRealIndex(
                            cur_select_index));
                        MMK_CreateWin((uint32 *)MMIENVSET_SETTING_OPT_WIN_TAB, PNULL);
                        MMK_CloseWin(win_id);
                    }
                    break;
                    
                case MMIEVN_OPTMENU_NODE_ADD:
                    {
                        // add                        
                        uint8 add_index = GetEnvAddIndex();
                        if(add_index > 0)
                        {
                            MMIENVSET_SetCurModeId((uint8)add_index);
                            MMK_CreateWin((uint32 *)MMIENVSET_SETTING_OPT_WIN_TAB, PNULL);       
                        }
                        MMK_CloseWin(win_id);
                    }
                    break;
                    
                case MMIEVN_OPTMENU_NODE_DEL:
                    // del 
                    MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, IMAGE_PUBWIN_QUERY, PNULL, PNULL);
                    break;

                default:
                    //SCI_TRACE_MID:" the menu id is error:HandleEnvMainOptMenuWindow"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_WINTAB_1208_112_2_18_2_19_7_69,(uint8*)"");
                    break;
            }
        }
            break;

    case MSG_PROMPTWIN_OK:
        if (MMIEVN_OPTMENU_NODE_DEL == GUIMENU_GetCurNodeId(ctrl_id))
        {
            MMIENVSET_HandleDeleteItem(
                MMIENVSET_GetEnvRealIndex(cur_select_index));                
            MMIPUB_CloseQuerytWin(PNULL);
        }
        MMK_CloseWin(win_id);
        break;

        case MSG_PROMPTWIN_CANCEL:
            MMIPUB_CloseQuerytWin(PNULL);
            MMK_CloseWin(win_id);
            break;

        case MSG_CTL_CANCEL:
            MMK_CloseWin( win_id );      
            break;

        default:
            recode = MMI_RESULT_FALSE;
            break;
    }
    return recode;
}

/*****************************************************************************/
//  Description :get add index
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL uint16 GetEnvActionIndex(
                                uint8 active_index //当前激活的模式
                                )
{
    MMIENVSET_SETTING_T    mode_setting = {0};
    uint16 i = 0;
    uint16 real_active_index = active_index;

    if (active_index  >= MMIENVSET_USER_MODE1)
    {
        for(i=MMIENVSET_USER_MODE1; i<=active_index; i++)
        {
            SCI_MEMSET(&mode_setting,0,sizeof(MMIENVSET_SETTING_T));
            MMIAPIENVSET_GetModeValue(i, &mode_setting);
            if(!mode_setting.is_valid)  
            {
                real_active_index--;
            }
        }
    }
    return real_active_index;
}

#ifdef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description :get add index
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL uint8 GetEnvValidIndex(void)
{
    MMIENVSET_SETTING_T    mode_setting = {0};
    uint8 i = 0;
    
    for(i=MMIENVSET_USER_MODE1; i<MMIENVSET_TOTAL_MODE; i++)
    {
        SCI_MEMSET(&mode_setting,0,sizeof(MMIENVSET_SETTING_T));
        MMIAPIENVSET_GetModeValue(i, &mode_setting);
        if(!mode_setting.is_valid)  
        {
            break;
        }
    }

    return i;
}

/*****************************************************************************/
//  Description : insert dyna menu node by label
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void InsertNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //控件id
                             MMI_TEXT_ID_T      text_id,       //插入的节点的文本
                             uint16             node_id,        //节点编号
                             uint16             parent_node_id, //父节点编号
                             uint16             index,           //位置
                             BOOLEAN            is_grayed        //是否灰显
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};
    
    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    node_item.is_grayed = is_grayed;
    
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
} 

/*****************************************************************************/
//Description :open env setting win
//Global resource dependence : none
//Author:jian.ma
//Note: 
/*****************************************************************************/
LOCAL void OpenEnvSettingWin(MMI_MESSAGE_ID_E   msg_id)
{
    MMI_WIN_ID_T            win_id  = MMIENVSET_SETTING_WIN_ID;
    MMI_CTRL_ID_T           opt_ctrl_id = MMIENVSET_SETTING_OPT_CTRL_ID;
    MMI_STRING_T            kstring = {0};
    uint8                   cur_select = 0;
    uint32                  i = 0;
    MMIENVSET_SETTING_T     mode_setting_ptr   =   {0};

    if (MMK_IsOpenWin(win_id))
    {
        MMK_CloseWin(win_id);
    }
    cur_select = GUILIST_GetCurItemIndex( MMIENVSET_MAIN_MENU_CTRL_ID );
    
    MMIENVSET_SetCurModeId(cur_select);

    MMK_CreateWin((uint32 *)MMIENV_SETTING_OPT_WIN_TAB,(ADD_DATA)cur_select);
    if (MSG_CTL_PENOK == msg_id)
    {
        uint16 real_id = 0;        
        real_id = MMIENVSET_GetEnvRealIndex(cur_select);
            
        // 由文本资源获得字符串
        MMIAPIENVSET_GetModeValue(real_id,&mode_setting_ptr);
        if( 0 == mode_setting_ptr.name_str_t.wstr_len ) 
        {
            // 由文本资源获得字符串
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(g_mmienvset_text_id[real_id]),&kstring);    
        }
        else
        {
            kstring.wstr_ptr = mode_setting_ptr.name_str_t.wstr;
            kstring.wstr_len = mode_setting_ptr.name_str_t.wstr_len;
        }

        GUIMENU_CreatDynamic(PNULL,win_id,opt_ctrl_id,GUIMENU_STYLE_POPUP_AUTO);
        //active
        InsertNodeByLabel(opt_ctrl_id,TXT_ACTIVE_OPERATIVE,MMIEVN_OPTMENU_NODE_ACTIVE,0,i++,FALSE);

        //setting
        InsertNodeByLabel(opt_ctrl_id,STXT_MAIN_SETTING,MMIEVN_OPTMENU_NODE_SET,0,i++,FALSE);

        //del
        if (cur_select >= MMIENVSET_USER_MODE1)
        {
            InsertNodeByLabel(opt_ctrl_id,TXT_DELETE,MMIEVN_OPTMENU_NODE_DEL,0,i++,FALSE);
        }

	    GUIMENU_SetMenuTitle(&kstring,MMIENVSET_SETTING_OPT_CTRL_ID);
    }
    else
    {
        //creat dynamic menu
        GUIMENU_CreatDynamic(PNULL, win_id, opt_ctrl_id, GUIMENU_STYLE_POPUP);
        //add
        if (GUILIST_GetTotalItemNum(MMIENVSET_MAIN_MENU_CTRL_ID) < MMIENVSET_TOTAL_MODE)
        {
            InsertNodeByLabel(opt_ctrl_id,TXT_ADD,MMIEVN_OPTMENU_NODE_ADD,0,i++,FALSE);
        }
        else
        {
            InsertNodeByLabel(opt_ctrl_id,TXT_ADD,MMIEVN_OPTMENU_NODE_ADD,0,i++,TRUE);
        }
    
    }
}

/*****************************************************************************/
//  Description : handle env setting context menu window message
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleEnvSettingWinMsg(
                                        MMI_WIN_ID_T        win_id,     
                                        MMI_MESSAGE_ID_E    msg_id, 
                                        DPARAM              param
                                        )
{
    MMI_RESULT_E        result  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIENVSET_SETTING_OPT_CTRL_ID;
    MMI_WIN_ID_T        query_win_id = MMIENVSET_SETTING_QUERY_WIN_ID;
    uint8               select_id = (uint32)MMK_GetWinAddDataPtr(win_id);

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        switch (GUIMENU_GetCurNodeId(ctrl_id))
        {
            case MMIEVN_OPTMENU_NODE_ACTIVE:
            {
                // 启动该环境模式的设置，写入nv和全局变量
                uint8  mode_id = (uint8)MMIENVSET_GetEnvRealIndex(select_id);
                MMIAPIENVSET_ActiveMode(mode_id);
                MMIENVSET_SetPreActiveMode(mode_id);  
                MMK_CloseWin(win_id);
            }
            break;
            case MMIEVN_OPTMENU_NODE_SET:
            {
                // 进入设置的环境模式
                MMIENVSET_SetCurModeId((uint8)MMIENVSET_GetEnvRealIndex(select_id));
                MMK_CreateWin((uint32 *)MMIENVSET_SETTING_OPT_WIN_TAB, (ADD_DATA)select_id);
                MMK_CloseWin(win_id);
            }
            break;
            case MMIEVN_OPTMENU_NODE_ADD:
            {
                // add                        
                uint8 add_index = GetEnvValidIndex();
                if(add_index < MMIENVSET_TOTAL_MODE)
                {
                    MMIENVSET_SetCurModeId((uint8)add_index);
                    MMK_CreateWin((uint32 *)MMIENVSET_SETTING_OPT_WIN_TAB, (ADD_DATA)add_index);       
                }
                MMK_CloseWin(win_id);
            }
            break;
                
            case MMIEVN_OPTMENU_NODE_DEL:// del 
                {
                    MMIPUB_OpenQueryWinByTextId(TXT_DELETEQUERY, 
                        IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
                }
                break;

            default:
                //SCI_TRACE_LOW:" the menu id is error:HandleEnvMainOptMenuWindow"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_WINTAB_1439_112_2_18_2_19_7_70,(uint8*)"");
                break;
        }
        break;
    case MSG_PROMPTWIN_OK:
        if (MMIEVN_OPTMENU_NODE_DEL == GUIMENU_GetCurNodeId(ctrl_id))
        {
            //MMI_STRING_T    string1 = {0};
            MMI_STRING_T    string2 = {0};
            //MMI_STRING_T    string3 = {0};
            MMIENVSET_SETTING_T  mode_setting = {0};
            
            MMIAPIENVSET_GetModeValue(MMIENVSET_GetCurModeId(),&mode_setting);

            if( 0 == mode_setting.name_str_t.wstr_len ) 
            {
                // 由文本资源获得字符串
                MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(g_mmienvset_text_id[MMIENVSET_GetCurModeId()]),&string2);    
            }
            else
            {
                string2.wstr_ptr = mode_setting.name_str_t.wstr;
                string2.wstr_len = mode_setting.name_str_t.wstr_len;
            }

            MMIENVSET_HandleDeleteItem(MMIENVSET_GetEnvRealIndex(select_id));

          //  MMI_GetLabelTextByLang(TXT_DELETE, &string1);
          //  MMI_GetLabelTextByLang(TXT_DELETED, &string3);
            
            MMIPUB_CloseQuerytWin(&query_win_id);
            
           // MMIPUB_OpenAlertWinByTextPtr(PNULL,&string3,PNULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

         //   MMIPUB_SetWinTitleText(MMIPUB_ALERT_WIN_ID,&string1,TRUE);
        }
        MMK_CloseWin(win_id);
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(&query_win_id);
        MMK_CloseWin(win_id);
        break;
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
            
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
#endif

/*****************************************************************************/
//Description : 情景模式窗口处理函数
//Global resource dependence : none
//Author:maqianlin 
//Note: 在正在使用的环境列表项前打勾
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleEnvSetMainMenuWindow(
                                               MMI_WIN_ID_T    win_id, //窗口ID
                                               MMI_MESSAGE_ID_E   msg_id, //消息ID
                                               DPARAM             param   //消息的参数
                                               )
{
    uint16         cur_select_id   =   0;
    MMI_CTRL_ID_T   ctrl_id  = MMIENVSET_MAIN_MENU_CTRL_ID;
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    MMI_WIN_ID_T    query_win_id = MMIENVSET_QUERY_WIN_ID;
    MMI_STRING_T    query_text = {0};
    static uint8    pre_index  = 0;
#endif
    uint8           active_mode_id = MMIAPIENVSET_GetActiveModeId();
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    MMISET_POWER_SETTING_ITEM_T  power_sav_setting = MMIAPISET_GetPowerSavingSetting();
#endif
#if defined MMI_PDA_SUPPORT
    static          BOOLEAN is_long_tp =  FALSE;
#endif
	static uint16 current_index = 0;
    switch (msg_id)     
    {
    case MSG_OPEN_WINDOW:
        {
            MMIENVSET_SetCurModeId(active_mode_id);
            //列出所有模式
            MMIENVSET_MainMenu_Append_LBItem(ctrl_id);            

#ifdef MMI_PDA_SUPPORT
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
            GUIWIN_SetTitleButtonTextId(win_id, GUITITLE_BUTTON_RIGHT, TXT_NULL, FALSE);
            GUIWIN_SetTitleButtonImage(win_id, GUITITLE_BUTTON_RIGHT, IMAGE_ISTYLE_TITLE_ADD, IMAGE_ISTYLE_TITLE_ADD_DOWN, FALSE);
            GUIWIN_SetTitleButtonHandleMsgInfo(win_id, GUITITLE_BUTTON_RIGHT, MSG_NOTIFY_TITLE_MENU_ICON, ctrl_id);
#endif

            current_index = GetEnvActionIndex(active_mode_id);//获得当前的情景模式
            MMK_SetAtvCtrl(win_id,ctrl_id);
        }
        break;
        
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    case MSG_PROMPTWIN_OK://确认修改情景模式
        {
            uint8 envset_mode = 0;
            uint16 real_id = 0;
            
            if(pre_index != active_mode_id)
            {
                real_id =  MMIENVSET_GetEnvRealIndex(pre_index);
                MMIENVSET_SetCurModeId((uint8)real_id);
                envset_mode = MMIENVSET_GetCurModeId();

                MMIAPIENVSET_ActiveMode(envset_mode);
                GUILIST_SetSelectedItem(ctrl_id, pre_index, TRUE );
                MMIAPIENVSET_ResetActModeOtherRingSet();
                MMK_SendMsg(ctrl_id, MSG_CTL_PAINT,PNULL);
            }
            MMK_CloseWin(query_win_id);
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMK_CloseWin(query_win_id);
        break;
#endif
        
    case MSG_FULL_PAINT:
        cur_select_id = GetEnvActionIndex(active_mode_id);
        GUILIST_SetSelectedItem( MMIENVSET_MAIN_MENU_CTRL_ID,  cur_select_id, TRUE );
        GUILIST_SetCurItemIndex( MMIENVSET_MAIN_MENU_CTRL_ID,  current_index );
#if defined MMI_PDA_SUPPORT
        if (GUILIST_GetTotalItemNum(MMIENVSET_MAIN_MENU_CTRL_ID) < MMIENVSET_TOTAL_MODE)
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
        }
        else
        {
            GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_INVISIBLE, FALSE);
        }
#endif
        break;
#if defined MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        if(!is_long_tp)
        {
            OpenEnvSettingWin(msg_id);
        }
        else
        {
            is_long_tp = FALSE;
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527        
#else
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        {
            // 激活操作
            uint8 envset_mode = 0;
            uint16 real_id = 0;
            cur_select_id = GUILIST_GetCurItemIndex( ctrl_id );
            real_id = MMIENVSET_GetEnvRealIndex(cur_select_id);
#if !defined DPHONE_SUPPORT 
#if defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
            if(MMIENVSET_EARPHONE_MODE == active_mode_id
              || MMIENVSET_BLUETOOTH_MODE == active_mode_id)
            {
                break;
            }  
            else  if(MMIENVSET_EARPHONE_MODE != real_id && MMIENVSET_BLUETOOTH_MODE != real_id)    
#endif            
            {

               // 进入设置的环境模式
                MMIENVSET_SetCurModeId((uint8)real_id);
                envset_mode = MMIENVSET_GetCurModeId();

                if (envset_mode != active_mode_id)
                {
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
                    if(power_sav_setting.is_active)//如果省电模式打开，则需要打开提示框
                    {
                        pre_index = cur_select_id;
                        MMI_GetLabelTextByLang(TXT_ENVSET_QUERY,&query_text);
                        MMIPUB_OpenQueryWinByTextPtr(&query_text, PNULL, IMAGE_PUBWIN_QUERY, &query_win_id, PNULL);
                        break;
                    }
                    else
#endif
                    {
                        // 启动该环境模式的设置，写入nv和全局变量
                        MMIAPIENVSET_ActiveMode(envset_mode);
                        MMIENVSET_SetPreActiveMode(active_mode_id);   
                    }
                }
                //GUILIST_SetSelectedItem( ctrl_id, cur_select_id, TRUE );
                //MMK_SendMsg(ctrl_id, MSG_CTL_PAINT,PNULL);

                PromptSuccessWin();
            }
#endif
        }
        break;
        
#if defined MMI_PDA_SUPPORT
    case MSG_NOTIFY_TITLE_MENU_ICON:
        {
            // add                        
            uint8 add_index = GetEnvAddIndex();

            if (add_index > 0 && add_index<MMIENVSET_TOTAL_MODE)
            {
                MMIENVSET_SetCurModeId((uint8)add_index);
                MMK_CreateWin((uint32 *)MMIENVSET_SETTING_OPT_WIN_TAB, PNULL);
            }
        }
        break;
    case MSG_CTL_LIST_LONGOK:
        is_long_tp = TRUE;
       break;
    case MSG_APP_MENU:               
        {
            //打开选项菜单
            OpenEnvOptMenuWin(GUILIST_GetTotalItemNum(ctrl_id), GUILIST_GetCurItemIndex(ctrl_id));
        }
        break;
#else
    case MSG_CTL_OK:
    case MSG_APP_OK:
        {
            //打开选项菜单
            OpenEnvOptMenuWin(GUILIST_GetTotalItemNum(ctrl_id), GUILIST_GetCurItemIndex(ctrl_id));
        }
        break;
#endif     
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    case MSG_KEYDOWN_HEADSET_DETECT:
        MMIPUB_OpenAlertWarningWin(TXT_SET_PROFILES_EARPHONE_PLUG_IN);

        if(MMIENVSET_BLUETOOTH_MODE != MMIAPIENVSET_GetActiveModeId())//蓝牙优先 
        {
            MMIAPIENVSET_SetEarphoneMode();
            MMK_SendMsg(ctrl_id,MSG_CTL_PAINT,PNULL);
        }        
        break;
    case MSG_KEYUP_HEADSET_DETECT:
        MMIPUB_OpenAlertWarningWin(TXT_SET_PROFILES_EARPHONE_PLUG_OUT);
        if(MMIENVSET_EARPHONE_MODE == MMIAPIENVSET_GetActiveModeId())
        {
            MMIAPIENVSET_UnSetEarphoneMode();

            MMK_SendMsg(ctrl_id,MSG_CTL_PAINT,PNULL);
        }        
        break;
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:

        MMK_CloseWin( win_id );      
        break;
	case MSG_LOSE_FOCUS:
		current_index = GUILIST_GetCurItemIndex(MMIENVSET_MAIN_MENU_CTRL_ID);
		break;
    case MSG_CLOSE_WINDOW:
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
        pre_index = 0;
#endif
        break;        
        
    default:        
        recode = MMI_RESULT_FALSE;
        break;
    }    

    return recode;
} 

#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description :操作菜单窗口处理函数(启动、个性化设置以及修改名称) 
//Global resource dependence : none
//Author:maqianlin 
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleEnvSetOperationWindow(
                                                MMI_WIN_ID_T    win_id, //窗口ID
                                                MMI_MESSAGE_ID_E   msg_id, //消息ID
                                                DPARAM             param   //消息参数
                                                )
{
    MMI_RESULT_E        recode      =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id     =   MENU_POPUP_CTRL_ID;
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    uint32              focus_mode_id     =  (uint32)MMK_GetWinAddDataPtr(win_id);
    uint8               active_mode_id       =  MMIAPIENVSET_GetActiveModeId();
#endif
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
        if (MMIENVSET_EARPHONE_MODE == focus_mode_id 
           || MMIENVSET_BLUETOOTH_MODE == focus_mode_id
           ||MMIENVSET_EARPHONE_MODE == active_mode_id 
           || MMIENVSET_BLUETOOTH_MODE == active_mode_id)
        {
            GUIMENU_SetItemVisible(
            ctrl_id,    
            MENU_ENVSET_OPERATION,  
            ID_ENVSET_ACTIVE,   
            FALSE);
        }
#endif
        //激活控件
        MMK_SetAtvCtrl(win_id, ctrl_id);		
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK: //触笔
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
        HandleOperationMenu(ctrl_id);
        MMK_CloseWin( win_id );
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
} 
#endif
/*****************************************************************************/
// Description : prompt the success win
//  Global resource dependence : none
// Author: xiangjie
//  Note:
/*****************************************************************************/
LOCAL void PromptSuccessWin(void)//@zhaohui modify 使启动模式时，显示的内容与该模式的命名一致,cr102315
{
    MMI_STRING_T        prompt_str1         =   {0}; 
    MMI_STRING_T        prompt_str2         =   {0};

    uint8               cur_select          =   0;
    MMIENVSET_SETTING_T *mode_setting_ptr   =   PNULL;
#ifdef MMI_GUI_STYLE_MINISCREEN
    uint32              time_period         =   400;
#endif

    cur_select = MMIENVSET_GetCurModeId();

    mode_setting_ptr = SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
    if(mode_setting_ptr == PNULL)
    {
        return;
    }
    SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));
    MMIAPIENVSET_GetModeValue(cur_select,mode_setting_ptr);

    // 出厂设置
    if( 0 == mode_setting_ptr->name_str_t.wstr_len ) 
    {
        // 由文本资源获得字符串
        if(cur_select < MMIENVSET_TOTAL_MODE)
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(g_mmienvset_text_id[cur_select]),&prompt_str1);   
        }
    }
    else
    {
        prompt_str1.wstr_ptr = mode_setting_ptr->name_str_t.wstr;
        //prompt_str1.is_ucs2 = g_mmienvset_mode_setting[cur_select].name_str_t.is_ucs2;
        prompt_str1.wstr_len = mode_setting_ptr->name_str_t.wstr_len;
    }

    if (PNULL == prompt_str1.wstr_ptr)
    {
        SCI_TRACE_LOW("prompt_str1.wstr_ptr != PNULL");
        return;
    }
    
    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_SET_PROFILES_ACTIVATED, &prompt_str2);
#ifndef MMI_GUI_STYLE_MINISCREEN
    MMIPUB_OpenAlertWinByTextPtr(PNULL,&prompt_str2,&prompt_str1,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#else
    MMIPUB_OpenAlertWinByTextPtr(&time_period,&prompt_str2,&prompt_str1,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
#endif
#ifdef MMI_PDA_SUPPORT
    MMIPUB_SetWinTitleText(MMIPUB_ALERT_WIN_ID,&prompt_str1,TRUE);   
#endif
    
    SCI_FREE(mode_setting_ptr);
    mode_setting_ptr = PNULL;
    
}
/*****************************************************************************/
// Description : 对个性化设置子菜单以及启动、修改名字设置的处理 
//  Global resource dependence : none
// Author: maqianlin
//  Note:
/*****************************************************************************/
LOCAL void   HandleOperationMenu(
                                 MMI_CTRL_ID_T  ctrl_id
                                 )
{
    uint32      group_id    =   0 ;
    uint32      menu_id     =   0;
#ifndef MMI_ENVSET_MINI_SUPPORT
    uint8       envset_mode =   0;
#endif
//    uint16 fly_mode_switch = 0;
    
    //获得menu_id
    GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
    
    //子菜单项功能实现
    switch(menu_id)
    {
    case ID_SET_RING:
        MMISET_EnterRingSettingMenu();
        break;
        
        // setting
#ifndef MMI_ENVSET_MINI_SUPPORT
    case  ID_ENVSET_SETTING_OPT:
        MMK_CreateWin((uint32 *)MMIENVSET_SETTING_OPT_WIN_TAB, PNULL);
        break;
        
        // 启动设置
    case ID_ENVSET_ACTIVE:     
        envset_mode = MMIENVSET_GetCurModeId();

        if (envset_mode != MMIAPIENVSET_GetActiveModeId())
        {
            // 启动该环境模式的设置，写入nv和全局变量
            MMIAPIENVSET_ActiveMode(envset_mode);
            MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId());   
        }
        
        //success message box
        PromptSuccessWin();
        MMK_CloseWin(MMIENVSET_OPERATION_WIN_ID);           
        break;
 #endif        
    // 改名称
    case  ID_ENVSET_CHANGE_NAME:
        MMK_CreateWin((uint32 *)MMIENVSET_CHANGE_ENV_NAME_WIN_TAB,PNULL);
        break;
#ifdef PLATFORM_SC6600L
    case ID_ENVSET_ENHANCE_RING:
        MMK_CreateWin((uint32 *)MMIENVSET_ENHANCE_RING_SWICTH_WIN_TAB,PNULL);
        break;
#endif
#ifndef MMI_ENVSET_MINI_SUPPORT
        // 铃声音量
    case   ID_ENVSET_CALL_RING_VOL:
    case   ID_ENVSET_MSG_RING_VOL:
    case   ID_ENVSET_COMM_VOL:
    case   ID_ENVSET_KEY_RING_VOL:
    case   ID_ENVSET_ALARM_RING_VOL:
    case   ID_ENVSET_POWER_RING_VOL:       
    case ID_ENVSET_TKEY_RING_VOL:
        MMK_CreateWin((uint32 *)MMIENVSET_RING_VOL_WIN_TAB,PNULL);
        break;
        
        // 来电铃声类型
    case   ID_ENVSET_CALL_RING_TYPE:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        s_ring_type = CALL_RING_TYPE;
       // MMK_CreatePubListWin((uint32*)MMIENVSET_SELECT_SIM_WIN_TAB, PNULL);
       MMISET_SelectSIMFunc(EnvSetSelectSimCallback,MMISET_APPEND_SIM_ALL,PNULL);
#else
        MMK_CreatePubListWin((uint32 *)MMIENVSET_CALL_RING_TYPE_WIN_TAB,PNULL);    
#endif
        break;
        
        //  短信铃声类型
    case    ID_ENVSET_MSG_RING_TYPE:
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        s_ring_type = MSG_RING_TYPE;
       MMISET_SelectSIMFunc(EnvSetSelectSimCallback,MMISET_APPEND_SIM_ALL,PNULL);
#else
        MMK_CreatePubListWin((uint32 *)MMIENVSET_MSG_RING_TYPE_WIN_TAB,PNULL);    
#endif
        break;
        
        //  闹钟铃声类型
    case   ID_ENVSET_ALARM_RING_TYPE:
        MMK_CreatePubListWin((uint32 *)MMIENVSET_ALARM_RING_TYPE_WIN_TAB,PNULL);
        break;
        
        //  按键音类型
    case   ID_ENVSET_KEY_RING_TYPE:
        MMK_CreatePubListWin((uint32 *)MMIENVSET_KEY_RING_TYPE_WIN_TAB,PNULL);
        break;
#endif  
#ifdef TOUCH_PANEL_SUPPORT
         //  触屏音类型
    case  ID_ENVSET_TKEY_RING_TYPE: 
        MMK_CreateWin((uint32 *)MMIENVSET_TKEY_RING_TYPE_WIN_TAB,PNULL);
        break;
#endif
#ifdef LCD_SLIDE_SUPPORT        
          //  滑盖音
	case  ID_ENVSET_SLIDE_RING_TYPE:	
        MMK_CreateWin((uint32 *)MMIENVSET_SLIDE_RING_TYPE_WIN_TAB,PNULL);
		break; 
           
    case  ID_ENVSET_SLIDE_RING_ON:	
        MMK_CreateWin((uint32 *)MMIENVSET_SLIDE_RING_ON_TYPE_WIN_TAB,PNULL);
		break;  
    case  ID_ENVSET_SLIDE_RING_OFF:	
        MMK_CreateWin((uint32 *)MMIENVSET_SLIDE_RING_OFF_TYPE_WIN_TAB,PNULL);
		break;  
#endif

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone. 
        //  翻合盖音乐类型
    case    ID_ENVSET_FLIP_RING_TYPE:
        MMK_CreateWin((uint32 *)MMIENVSET_FLIP_RING_TYPE_WIN_TAB,PNULL);
        break;
#endif
#ifndef MMI_ENVSET_MINI_SUPPORT
    case   ID_ENVSET_DOWNLOAD_COMPLETE_NOTIFY_SWITCH:
        MMK_CreateWin((uint32 *)MMIENVSET_DOWNLOAD_NOTIFY_SWICTH_WIN_TAB,PNULL);
        break;

        //  低电压报警开关
    case   ID_ENVSET_LOW_VOLTAGE_WARN_SWITCH:
        MMK_CreateWin((uint32 *)MMIENVSET_LOW_VOLTAGE_WARN_SWICTH_WIN_TAB,PNULL);
        break;
        
        
    case ID_ENVSET_POWER_RING_SWITCH:
        MMK_CreateWin((uint32 *)MMIENVSET_POWER_RING_SWICTH_WIN_TAB,PNULL);
        break;
#endif        
    case ID_ENVSET_RING_VOL:
        MMK_CreateWin((uint32 *)MMIENVSET_ALL_RING_VOL_WIN_TAB,PNULL);
        break;
        
    case ID_ENVSET_RING_TYPE:
        MMK_CreateWin((uint32 *)MMIENVSET_RING_TYPE_WIN_TAB,PNULL);
        break;
        
    case ID_ENVSET_RING_OTHER:
        MMK_CreateWin((uint32 *)MMIENVSET_OTHER_RING_SET_WIN_TAB,PNULL);
        break;
        
    default:
        //SCI_PASSERT(0,("HandleOperationMenu menu_id = %d", menu_id));     
        break;
    } 
} 

/*****************************************************************************/
//Description :   setting menu
//Global resource dependence :  none
//Author: guanyong.zhang
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSettingOptWindow(
                                           MMI_WIN_ID_T    win_id,   // 窗口ID
                                           MMI_MESSAGE_ID_E   msg_id,   // 消息ID
                                           DPARAM             param     // 消息参数
                                           )
{
    MMI_RESULT_E    recode      = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id     = MMIENVSET_SETTING_MENU_CTRL_ID;    
    uint16          select_id   =   0;
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
    MMI_MENU_GROUP_ID_T  group_id = 0;  //group id
    MMI_MENU_ID_T        menu_id  = 0;  //menu id
    MMISET_POWER_SETTING_ITEM_T power_sav_setting = MMIAPISET_GetPowerSavingSetting();
    
    GUIMENU_GetId(ctrl_id,&group_id,&menu_id);
#endif

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            MMIENVSET_SETTING_T  mode_setting = {0};
            //CR244608 2011-5-23
            select_id = MMIENVSET_GetCurModeId();
            
#ifdef MMI_POWER_SAVING_MODE_SUPPORT
            //如果省电模式开启，且，当前选择的id为省电模式开启时active的mode，则要把"其它铃声"设置项灰化
            if(MMIAPISET_GetPowerSavingSwitch() 
                && power_sav_setting.is_active
                && (select_id == MMIENVSET_GetPowerSavENVMode()))
            {
                GUIMENU_SetItemGrayed( ctrl_id,  group_id,  ID_ENVSET_RING_OTHER,  TRUE);
            }
#endif
            SCI_MEMSET(&mode_setting,0,sizeof(MMIENVSET_SETTING_T));
            MMIAPIENVSET_GetModeValue(select_id,&mode_setting);

            GUIMENU_SetState(ctrl_id, GUIMENU_STATE_CUSTOMER_TITLE, TRUE);
                        
            //新需求，要求设置就标题为设置，如果是新加的则为新加，不必写模式名
            if(0 != mode_setting.name_str_t.wstr_len)
            {
                //设置
                GUIWIN_SetTitleTextId(win_id, STXT_MAIN_SETTING, FALSE);
            }
            else
            {
                //新增
                if (select_id < MMIENVSET_USER_MODE1)
                {
                    GUIWIN_SetTitleTextId(win_id, STXT_MAIN_SETTING, FALSE);
                }
                else
                {
                    GUIWIN_SetTitleTextId(win_id, TXT_ADDNEW, FALSE);                   
                }
            }
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT            
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
            GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
            GUIWIN_SetSoftkeyTextId(win_id,  STXT_SELECT, TXT_NULL, STXT_FINISH, FALSE);
#endif
            //激活控件
            MMK_SetAtvCtrl(win_id, ctrl_id);
        }
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        HandleOperationMenu(ctrl_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        if((MMIENVSET_GetCurModeId() >= MMIENVSET_USER_MODE1))
        {
            MMIENVSET_HandleAddItem();
        }
        MMK_CloseWin( win_id );
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
} 

/*****************************************************************************/
//Description :   修改环境名称的EditBox
//Global resource dependence :  none
//Author: maqianlin
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleChangeEnvNameWindow(
                                              MMI_WIN_ID_T    win_id,   // 窗口ID
                                              MMI_MESSAGE_ID_E   msg_id,   // 消息ID
                                              DPARAM             param     // 消息参数
                                              )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIENVSET_CHANGE_ENV_NAME_EDITBOX_CTRL_ID;
    
    param = param;
    
    switch (msg_id)
    {                             
    case MSG_OPEN_WINDOW:
        // 初始文本编辑框
        ENVSET_InitChangeNameTextEditBox(ctrl_id);

        //+CR229294
        #ifdef DPHONE_SUPPORT
        {
            GUI_RECT_T edit_rect = {0};

            edit_rect = MMITHEME_GetClientRect();
            edit_rect.top += MMI_STATUSBAR_HEIGHT  ;
            GUIEDIT_SetRect(ctrl_id, &edit_rect);
        }       
        #endif
        //-CR229294  
        
		//set left softkey
		GUIEDIT_SetSoftkey(ctrl_id,0,1,TXT_NULL,TXT_COMMON_OK,PNULL);
        MMK_SetAtvCtrl(win_id,ctrl_id);        
        break;

    //+CR229294
    #ifdef DPHONE_SUPPORT
    case MSG_FULL_PAINT:
        {
            GUI_LCD_DEV_INFO    lcd_dev_info    = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
            GUI_RECT_T rect = {0};

            rect = MMITHEME_GetClientRect();
            GUI_FillRect(&lcd_dev_info, rect, MMI_BACKGROUND_COLOR);
        }       
        break;
    #endif
    //-CR229294
#ifdef MMI_PDA_SUPPORT
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case  MSG_CTL_PENOK:
        {
            MMI_CTRL_ID_T     softkey_ctrl_id = 0;
            softkey_ctrl_id = ((MMI_NOTIFY_T*)param)->src_id;
            if(MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == softkey_ctrl_id)
            {
                //把新的环境名称写入nv 
                if (MMIENVSET_EnvName_WriteInNV(ctrl_id))
                {
                    MMK_CloseWin(win_id);
                }
            }
            else if(MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == softkey_ctrl_id)
            {
                MMK_CloseWin( win_id );
            }
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
        //把新的环境名称写入nv 
        if (MMIENVSET_EnvName_WriteInNV(ctrl_id))
        {
            MMK_CloseWin(win_id);
        }
        break;
        
    case MSG_CTL_CANCEL: 
    case MSG_APP_CANCEL:    
        MMK_CloseWin( win_id );      
        break;
        
    default:
        
        recode = MMI_RESULT_FALSE;
        break;
    } 
    
    return recode;
} 
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description :  铃声音量处理窗口
//Global resource dependence : 
//Author:  maqianlin
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRingVolumeWindow(
                                          MMI_WIN_ID_T            win_id,               // 窗口ID
                                          MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                          DPARAM                     param
                                          )
{                               
    uint32                    menu_id  = 0;
    MMISET_ALL_RING_TYPE_E    ring_type = MMISET_RING_TYPE_CALL;
    MMIENVSET_SET_OPT_E       set_opt = CALL_RING_VOL;
    MMI_RESULT_E              recode = MMI_RESULT_TRUE;
    MMI_TEXT_ID_T             win_title_id = TXT_NULL;
    
    menu_id = (uint32)MMK_GetWinAddDataPtr(win_id);
    switch(menu_id)
    {
        // 来电铃声音量
    case   ID_ENVSET_CALL_RING_VOL:
        set_opt = CALL_RING_VOL;
        ring_type = MMISET_RING_TYPE_CALL;
        win_title_id = TXT_SET_RING_SELECT_CALL;
        break;
        
        // 短信铃声音量
    case   ID_ENVSET_MSG_RING_VOL:
        set_opt = MSG_RING_VOL;
        ring_type = MMISET_RING_TYPE_MSG;
        win_title_id = TXT_SET_RING_SELECT_MSG;
        break;
        
        // 通话音量
    case   ID_ENVSET_COMM_VOL:
        set_opt = COMM_VOL;
        ring_type = MMISET_RING_TYPE_NONE;
        win_title_id = TXT_SET_RING_VOLUME_COMM;
        break;
        
        // 按键音量
    case   ID_ENVSET_KEY_RING_VOL:
        set_opt = KEY_RING_VOL;
        ring_type = MMISET_RING_TYPE_KEY;
        win_title_id = TXT_SET_RING_KEY_TONE;
        break;
        
    case ID_ENVSET_ALARM_RING_VOL:
        set_opt = ALARM_RING_VOL;
        ring_type = MMISET_RING_TYPE_ALARM;
        win_title_id = TXT_RING_SELECT_ALARM;
        break;
    case ID_ENVSET_TKEY_RING_VOL:
        set_opt = TKEY_RING_VOL;
        ring_type = MMISET_RING_TYPE_TKEY;
        win_title_id = TXT_SET_RING_TKEY_TONE;
        break;
    case ID_ENVSET_POWER_RING_VOL:
        set_opt = POWER_RING_VOL;
        ring_type = MMISET_RING_TYPE_POWER;
        win_title_id = TXT_ENVSET_POWER_RING_TYPE;
        break;

#ifdef MMI_READMENU_ENABLE
    case ID_ENVSET_MENUREAD_VOL:
        set_opt = MENUREAD_VOL;
        ring_type = MMISET_RING_TYPE_CALL;
        win_title_id = TXT_SET_RING_VOLUME_MENUREAD;
        break;        
#endif

    default:
        //SCI_TRACE_LOW:"HandleRingVolumeWindow:menu_id is error\n!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_WINTAB_2219_112_2_18_2_19_9_71,(uint8*)"");
        break;
        
    }
    
    //set win title
    if(MSG_OPEN_WINDOW == msg_id)
    {
        GUIWIN_SetTitleTextId(win_id,win_title_id,FALSE);
    }
    recode = HandleAllRingVolWindow(
        win_id,               
        msg_id,               
        ring_type,
        set_opt,
        param
        );
    return  recode; 
}
#endif
/*****************************************************************************/
// Description: callback function for volume setting
// Global resource dependence: NONE
// Author: leon.wang
// Note:
/*****************************************************************************/
LOCAL void SetRingVolumeCB(MMICOM_PANEL_CALLBACK_T *para_ptr) 
{
    if(PNULL == para_ptr) return;
    
    MMIENVSET_SetCurModeOptValue(s_ring_type, para_ptr->cur_value, MN_DUAL_SYS_1);
   
    //display the vol
    if (MMIAPISET_IsPlayingAppRing())
     {
         MMIENVSET_AdjustAudioVol(s_set_ring, para_ptr->cur_value);            
     }
     else
     {
		 if(s_set_ring !=  MMISET_RING_TYPE_NONE )
		 {
			 MMISRVAUD_SetMuteNohandle(TRUE);
			 MMIENVSET_PreviewRingByVol(MN_DUAL_SYS_1, s_set_ring, para_ptr->cur_value, MMIENVSET_PREVIEW_PLAY_RING_TIMES);            
			 MMISRVAUD_SetMuteNohandle(FALSE);
		 }
     }

     if (MMK_IsOpenWin(MMIENVSET_ALL_RING_VOL_WIN_ID))
     {
         SetAllRingVolParam();
		 MMK_PostMsg(MMIENVSET_ALL_RING_VOL_WIN_ID, MSG_FULL_PAINT, PNULL, NULL);
     }

    return;
}
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description : 所有铃声音量处理函数
//Global resource dependence : 
//Author: maqianlin
//Note:被HandleRingVolumeWindow调用 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllRingVolWindow(
                                           MMI_WIN_ID_T            win_id,               // 窗口ID
                                           MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                           MMISET_ALL_RING_TYPE_E     ring_type,
                                           MMIENVSET_SET_OPT_E        set_opt,
                                           DPARAM                     param
                                           )
{
    MMI_RESULT_E        recode              =   MMI_RESULT_TRUE;
    static  uint8       s_cur_vol           =   0;
#ifdef TOUCH_PANEL_SUPPORT    
    MMI_CTRL_ID_T       ctrl_id             =   0;
#endif
    //static BOOLEAN      is_keyring_stopped  =   FALSE;
    //static BOOLEAN      is_tpring_stopped   =   FALSE;
#ifdef MONOCHROME_LCD_SUPPORT 
#else		
    GUI_RECT_T          image_rect          =   MMITHEME_GetClientRect();
#endif
#ifdef TOUCH_PANEL_SUPPORT
	uint8               adj_vol               =   0;
	GUI_POINT_T         point                  =   {0};
#endif    
    //SCI_TRACE_LOW:"HandleAllRingVolWindow msg_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_WINTAB_2263_112_2_18_2_19_9_72,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        /*
        if (MMIDEFAULT_GetEnableKeyRing())
        {            
            MMIDEFAULT_EnableKeyRing(FALSE);
            MMIAPISET_StopRing(MMISET_RING_TYPE_KEY);
            is_keyring_stopped = TRUE;            
        }
        if (MMIDEFAULT_GetEnableTpRing())
        {            
            MMIDEFAULT_EnableTpRing(FALSE);
            MMIAPISET_StopRing(MMISET_RING_TYPE_TKEY);
            is_tpring_stopped = TRUE;
            
        }
        */
        //MMIAPICountVolBlockRect(win_id);
        // 设置软键
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_COMMON_OK,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN,FALSE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
		GUIWIN_SetSoftkeyTextId(win_id, (MMI_TEXT_ID_T)TXT_COMMON_OK,(MMI_TEXT_ID_T)TXT_NULL,(MMI_TEXT_ID_T)STXT_RETURN,FALSE);
#endif
        // 获得指定铃声类型的音量
        s_cur_vol = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1, set_opt);
        // 根据当前音量的大小播放铃声
        MMIENVSET_PreviewRingByVol(MN_DUAL_SYS_1, ring_type,s_cur_vol,MMIENVSET_PREVIEW_PLAY_RING_TIMES);

#ifdef MMI_PDA_SUPPORT
        GUIWIN_SetTitleButtonState(win_id, GUITITLE_BUTTON_RIGHT, TITLE_BUTTON_NORMAL, FALSE);
#endif

//        if (0 == s_cur_vol)
//        {
//            MMIENVSET_SetPlayResult(TRUE);
//        }
//        else
//        {        
//             MMIENVSET_SetPlayResult(FALSE);
//        }
        break;
        
    case MSG_FULL_PAINT:    
#ifdef MONOCHROME_LCD_SUPPORT  
#else			
        GUIRES_DisplayImg(PNULL,
            &image_rect,
            &image_rect,
            win_id,
            IMAGE_COMMON_BG,
            MMITHEME_GetDefaultLcdDev());
#endif 
        // 根据当前音量的大小显示音量图片
        MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
        break;
        
    case MSG_APP_LEFT:
    case MSG_APP_DOWN:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:

    //@guanyong.zhang cr86756
        //通话音量等级为1-7级, 不允许调节为0级
#ifdef MONOCHROME_LCD_SUPPORT 
        MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#else        
        MMIAPISET_DisplayVolumeBlockBgPicture(s_cur_vol,win_id,IMAGE_ADJUST_VOL_BLOCK_SINGLE_BG);
#endif
        if (COMM_VOL == set_opt)
        {
            if (MMISET_VOL_ONE < s_cur_vol)
            {
                s_cur_vol--;               
            }
        }
        else
        {
            if (MMISET_VOL_ZERO < s_cur_vol)
            {
                s_cur_vol--;               
            }
        }
        
        // 根据当前音量的大小播放铃声
        if (MMIAPISET_IsPlayingAppRing())
        {
            MMIENVSET_AdjustAudioVol(ring_type,s_cur_vol);            
        }
        else
        {
            MMIENVSET_PreviewRingByVol(MN_DUAL_SYS_1, ring_type,s_cur_vol,MMIENVSET_PREVIEW_PLAY_RING_TIMES);            
        }
//        if (0 == s_cur_vol)
//        {
//            MMIENVSET_SetPlayResult(TRUE);
//        }
//        else
//        {        
//             MMIENVSET_SetPlayResult(FALSE);
//        }
        
        // 根据当前音量的大小显示音量图片
        //MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#ifdef MONOCHROME_LCD_SUPPORT 
        MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#else        
        MMIAPISET_DisplayVolumeBlockBgPicture(s_cur_vol,win_id,IMAGE_ADJUST_VOL_BLOCK_FG);
#endif
        break;
        
    case MSG_APP_RIGHT:
    case MSG_APP_UP:
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
#ifdef MONOCHROME_LCD_SUPPORT 
        MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#else        
		MMIAPISET_DisplayVolumeBlockBgPicture(s_cur_vol,win_id,IMAGE_ADJUST_VOL_BLOCK_SINGLE_BG);
#endif
        if (MMISET_VOL_MAX > s_cur_vol)
        {
            s_cur_vol++;
        }
        // 根据当前音量的大小播放铃声
        if (MMIAPISET_IsPlayingAppRing())
        {
            MMIENVSET_AdjustAudioVol(ring_type,s_cur_vol);            
        }
        else
        {
            MMIENVSET_PreviewRingByVol(MN_DUAL_SYS_1, ring_type,s_cur_vol,MMIENVSET_PREVIEW_PLAY_RING_TIMES);            
        }
//        if (0 == s_cur_vol)
//        {
//            MMIENVSET_SetPlayResult(TRUE);
//        }
//        else
//        {        
//             MMIENVSET_SetPlayResult(FALSE);
//        }
        
        // 根据当前音量的大小显示音量图片
        //MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#ifdef MONOCHROME_LCD_SUPPORT 
        MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#else        
        MMIAPISET_DisplayVolumeBlockBgPicture(s_cur_vol,win_id,IMAGE_ADJUST_VOL_BLOCK_FG);        
#endif
        //SCI_TRACE_LOW:"HandleAllRingVolWindow s_cur_vol = %d "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_WINTAB_2411_112_2_18_2_19_9_73,(uint8*)"d", s_cur_vol);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        if (MMIENVSET_ADJUST_RING_VOL_DOWN_BUTTON_CTRL_ID == ctrl_id)
        {
            MMK_PostMsg(win_id, MSG_APP_DOWN, PNULL, 0);
        }
        else if (MMIENVSET_ADJUST_RING_VOL_UP_BUTTON_CTRL_ID == ctrl_id)
        {
            MMK_PostMsg(win_id, MSG_APP_UP, PNULL, 0);
        } 
#ifdef MMI_PDA_SUPPORT
        else if(MMICOMMON_BUTTON_SOFTKEY_LEFT_CTRL_ID == ctrl_id)
        {
            MMK_SendMsg(win_id, MSG_APP_OK, PNULL);
        }
        else if(MMICOMMON_BUTTON_SOFTKEY_RIGHT_CTRL_ID == ctrl_id)
        {
            MMK_SendMsg(win_id, MSG_APP_CANCEL, PNULL);
        }
#endif        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
    case MSG_APP_MENU:    
        MMIAPISET_StopRing(MMISET_RING_TYPE_OTHER);
        //保存设定的音量的大小
        MMIENVSET_SetCurModeOptValue(set_opt,s_cur_vol, MMIAPISET_GetActiveSim());
        SetVolumeLabelcontent(set_opt,s_cur_vol);
        MMK_CloseWin( win_id );
        break;                     
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_LOSE_FOCUS:
        // 停止播放
        MMIAPISET_StopAppRing();
//        MMIENVSET_SetPlayResult(TRUE);
        /*
        if (is_keyring_stopped)
        {
            MMIDEFAULT_EnableKeyRing(TRUE);
            is_keyring_stopped = FALSE;
        }
        if (is_tpring_stopped)
        {
            MMIDEFAULT_EnableTpRing(TRUE);
            is_tpring_stopped = FALSE;
        }
        */
        break;

    case MSG_GET_FOCUS:
        /*
        if (MMIDEFAULT_GetEnableKeyRing())
        {            
            MMIDEFAULT_EnableKeyRing(FALSE);
            MMIAPISET_StopRing(MMISET_RING_TYPE_KEY);
            is_keyring_stopped = TRUE;            
        }
        if (MMIDEFAULT_GetEnableTpRing())
        {            
            MMIDEFAULT_EnableTpRing(FALSE);
            MMIAPISET_StopRing(MMISET_RING_TYPE_TKEY);
            is_tpring_stopped = TRUE;            
        }        
        */
        break;

       
        
    case MSG_CLOSE_WINDOW:
        // 停止播放
        MMIAPISET_StopAppRing();
     //   MMIENVSET_SetPlayResult(TRUE);
        
        /*
        if (is_keyring_stopped)
        {
            MMIDEFAULT_EnableKeyRing(TRUE);
        }
        if (is_tpring_stopped)
        {
            MMIDEFAULT_EnableTpRing(TRUE);
        }
        is_keyring_stopped = FALSE;
        is_keyring_stopped = FALSE;
        */
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);  
        adj_vol = MMIAPISET_TPAdjustVolume(point, s_cur_vol, MMISET_VOL_MAX, win_id);
        if (COMM_VOL == set_opt)
        {
            if (MMISET_VOL_ZERO == adj_vol)
            {
                adj_vol = 1;               
            }
        }
        // 根据当前音量的大小播放铃声
        if (adj_vol != s_cur_vol)
        {
            s_cur_vol = adj_vol;
            if (MMIAPISET_IsPlayingAppRing())
            {
                MMIENVSET_AdjustAudioVol(ring_type,s_cur_vol);
            }
            else
            {
                MMIENVSET_PreviewRingByVol(MN_DUAL_SYS_1, ring_type,s_cur_vol,MMIENVSET_PREVIEW_PLAY_RING_TIMES);
            }
            // 根据当前音量的大小显示音量图片
#ifdef MONOCHROME_LCD_SUPPORT 
		      MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);
#else        
            MMIAPISET_DisplayVolumeBlockBgPicture(MMIPHONE_ADJUST_VOL_MAX_STEPS,win_id,IMAGE_ADJUST_VOL_BLOCK_SINGLE_BG);
            MMIAPISET_DisplayVolumeBlockBgPicture(s_cur_vol,win_id,IMAGE_ADJUST_VOL_BLOCK_FG);      
#endif
            //MMIAPISET_DisplayVolumePicture(s_cur_vol,win_id);

        }      
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    default:
        recode = MMI_RESULT_FALSE;
        break;
    } //end switch
    
    return recode;
}
#endif
/*****************************************************************************/
//  Description :处理所有ring_type 函数
//   Global resource dependence : 
//  Author:maql 
//   Note: 2004.12.3
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleAllRingTypeWindow(
                                            MMI_WIN_ID_T         win_id,      //窗口ID
                                            MMI_MESSAGE_ID_E        msg_id,      //消息ID
                                            MMISET_ALL_RING_TYPE_E  ring_type,   //铃声类型
                                            MMIENVSET_SET_OPT_E     set_opt,     //设置项
                                            MMI_CTRL_ID_T        ctrl_id,     //控件ID
                                            DPARAM                  param        //参数
                                            )
{
    MMI_RESULT_E            recode               =   MMI_RESULT_TRUE;
    uint16                  cur_selection        =   0;
    uint16                  cur_item = 0;
    MMIENVSET_ADD_DATA_T*       add_data_ptr     =  (MMIENVSET_ADD_DATA_T*) MMK_GetWinAddDataPtr(win_id); 
    uint32          key_text_id[]       =
	{
	    TXT_RING_SILENT,
		TXT_COMMON_RING_RING1,
		TXT_COMMON_RING_RING2
	};
    uint32      call_text_id[]   =
    {
        TXT_SET_RING_RING,
        TXT_RING_VIBRA,
        TXT_RING_SILENT,
        TXT_SET_RING_VIBRA_BEFORE_RING,
        TXT_SET_RING_VIBRA_AND_RING
    };
	uint32  msg_text_id[] =
	{
		TXT_SET_RING_RING,
	    TXT_RING_VIBRA,
		TXT_RING_SILENT,
		TXT_SET_RING_VIBRA_AND_RING
			
	};
	uint32 dual_sys = 0;
    if(PNULL != add_data_ptr)
	{
        dual_sys =  add_data_ptr->s_dual_sys;
	}
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:        
        
        // 设置最大列表项
        GUILIST_SetMaxItem(ctrl_id,MMISET_MAX_CALL_TYPE, FALSE );//max item 5
        //append listbox 
        MMIENVSET_RingType_AppendListItem(dual_sys, ctrl_id,ring_type,set_opt);/*lint !e64*/
        //根据radio list当前处于焦点的提示方式/铃声类型，来播放相应的铃声，振动或者静音

        if(MMISET_RING_TYPE_KEY == ring_type && MMIAPISET_IsPlayingRingType(MMISET_RING_TYPE_KEY))
        {
            //nothing to do 
            //如果正在播放按键音、触屏幕，则 open win 时不需要再次播放。
            //按键音不能立即停止再重新播放，声音刺耳，播放中就不再重新播放。
        }
        else
        {
            MMIENVSET_PlayRingByListItem(dual_sys, ring_type,ctrl_id,MMIENVSET_PREVIEW_PLAY_RING_TIMES);/*lint !e64*/
        }
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
    
//    case MSG_KEYDOWN_UP:
//    case MSG_KEYDOWN_DOWN:
//        MMIENVSET_StopVibraAndRing(ring_type);
//        MMIENVSET_StopOnetimeVibTimer();
//        MMIENVSET_SetVibStatus(FALSE);
//         break;
    //此窗口不响应侧按键
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        break;

    case MSG_CTL_LIST_MOVETOP:
    case MSG_CTL_LIST_MOVEBOTTOM:
    case MSG_CTL_LIST_MOVEUP:
    case MSG_CTL_LIST_MOVEDOWN:
#ifndef MMI_PDA_SUPPORT 
   // case MSG_TP_PRESS_DOWN://上下键按键down停止铃声，up播放铃声
        MMIENVSET_StopVibraAndRing(ring_type);
        MMIENVSET_StopOnetimeVibTimer();
        MMIAPIENVSET_SetVibStatus(FALSE);
        MMIENVSET_PlayRingByListItem(dual_sys, ring_type,ctrl_id,MMIENVSET_PREVIEW_PLAY_RING_TIMES);/*lint !e64*/
#endif
        break;

//    case MSG_KEYUP_UP:
//    case MSG_KEYUP_DOWN:
//    case MSG_KEYPRESSUP_UP:
//    case MSG_KEYPRESSUP_DOWN:
//        //根据radio list当前处于焦点的提示方式/铃声类型，来播放相应的铃声，振动或者静音
//        MMIENVSET_PlayRingByListItem(dual_sys, ring_type,ctrl_id,MMIENVSET_PREVIEW_PLAY_RING_TIMES);
//         break;
        
    case MSG_TIMER: 
        // 判断是否指定的TimerId 
        if (MMIAPISET_IsVibratTimerOut(param))
        {
            // 振动的时间完
            MMIENVSET_VibraTimerOut(dual_sys, ring_type,ctrl_id);/*lint !e64*/
            
        }
        else if (MMIENVSET_IsOnetimeVibTimerOut(param))
        {
            if (MMIAPIENVSET_GetVibStatus())
            {
                MMIAPISET_StopVibrator(MMISET_RING_TYPE_OTHER);//关振动
                MMIAPIENVSET_SetVibStatus(FALSE);
              //  MMIENVSET_StartOnetimeVibTimer();
                if(ring_type == MMISET_RING_TYPE_CALL)
                {
                  cur_item = GUILIST_GetCurItemIndex(ctrl_id);
                  //SCI_TRACE_LOW:"mary HandleAllRingTypeWindow %d"
                  SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_WINTAB_2665_112_2_18_2_19_10_74,(uint8*)"d",ring_type);
                   if (MMISET_CALL_VIBRA_BEFORE_RING == (MMISET_CALL_RING_TYPE_E)cur_item)
                    {
                        if(s_g_vibrate <CALL_VIBRATE_TIME)
                        {
                            s_g_vibrate++;
                            MMIENVSET_StartOnetimeVibTimer();
                        }
                        else
                        {
                            
                             MMIENVSET_VibraTimerOut(dual_sys, ring_type,ctrl_id);/*lint !e64*/
                             s_g_vibrate = 0;
                        }
                    }
                    else
                    {
                        MMIENVSET_StartOnetimeVibTimer();
                    }                    
                 }
                else
                {
                    MMIENVSET_StartOnetimeVibTimer();
                }
            }
            else
            {
                MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_OTHER);
                MMIAPIENVSET_SetVibStatus(TRUE);
                MMIENVSET_StartOnetimeVibTimer();
            }
            
        }
        else                          
        {
            recode = MMI_RESULT_FALSE;
        }
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        //如果是list单击, 直接返回; 只响应双击
        if ( MMK_IsSingleClick(msg_id, param))
        {
#ifdef MMI_PDA_SUPPORT
            MMIENVSET_StopVibraAndRing(ring_type);
            MMIENVSET_StopOnetimeVibTimer();
            MMIAPIENVSET_SetVibStatus(FALSE);
            MMIENVSET_PlayRingByListItem(dual_sys, ring_type,ctrl_id,MMIENVSET_PREVIEW_PLAY_RING_TIMES);/*lint !e64*/
#endif
            break;
        }
        MMIENVSET_StopVibraAndRing(ring_type);
        //get the selected item
        GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
        
        // 如果是设置来电提示方式并且是设置启动的环境模式,相应的修改待机图标,该函数(MMISET_SetCallRingType)还被mainapp.c中切换来电提示方式的函数调用    
        if(ring_type == MMISET_RING_TYPE_CALL && MMIENVSET_IsActiveMode(MMIENVSET_GetCurModeId()) ) 
        {
            MMIAPISET_SetCallRingType((MMISET_CALL_RING_TYPE_E)cur_selection, dual_sys);/*lint !e64*/
        }
        else if(KEY_RING_TYPE == set_opt
            ||FLIP_RING_TYPE == set_opt
            ||FLIP_DOWN_RING_TYPE == set_opt)
        {
            //如果是list单击, 直接返回; 只响应双击
            if ( MMK_IsSingleClick(msg_id, param))//crNEWMS00115351
            {
                break;
            }
            else
            {
                MMIENVSET_SetCurModeOptValue(set_opt,(uint8)cur_selection, dual_sys); 
                if(KEY_RING_TYPE == set_opt)
                {
                    GUILABEL_SetTextById(MMIENVSET_KEYPAD_RING_SET_EDIT_LABEL_CTRL_ID,key_text_id[cur_selection],FALSE);
                }
                else if(FLIP_RING_TYPE == set_opt)
                {
                    GUILABEL_SetTextById(MMIENVSET_FLIP_RING_SET_EDIT_LABEL_CTRL_ID,key_text_id[cur_selection],FALSE);
                }
                else if(FLIP_DOWN_RING_TYPE == set_opt)
                {
                    GUILABEL_SetTextById(MMIENVSET_FLIP_DOWN_RING_SET_EDIT_LABEL_CTRL_ID,key_text_id[cur_selection],FALSE);
                }
            }
        }
        
        // 否则保存对应的铃声类型选择
        else 
        {
            MMIENVSET_SetCurModeOptValue(set_opt,(uint8)cur_selection, dual_sys); /*lint !e64*/ 
        }
        if(PNULL !=add_data_ptr)
        {
            if(CALL_RING_TYPE == set_opt)
            {
                GUILABEL_SetTextById(add_data_ptr->ctrl_id, call_text_id[cur_selection], FALSE);
            }
            else if(MSG_RING_TYPE == set_opt || ALARM_RING_TYPE == set_opt)
            {
                GUILABEL_SetTextById(add_data_ptr->ctrl_id, msg_text_id[cur_selection], FALSE);
            }
        }
       
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    case MSG_LOSE_FOCUS:
        //关闭窗口之前关闭所有的震动和铃声
        MMIENVSET_StopOnetimeVibTimer();
        MMIENVSET_StopVibraAndRing(ring_type);
        /*
        if (MMISET_RING_TYPE_TKEY != ring_type)
        {                   
            MMIDEFAULT_EnableKeyRing(TRUE);
            MMIDEFAULT_EnableTpRing(TRUE);
        }
        */
        break;
        
    case MSG_GET_FOCUS:
        /*
        if (MMISET_RING_TYPE_TKEY != ring_type)
        {
            MMIDEFAULT_EnableKeyRing(FALSE);
            MMIDEFAULT_EnableTpRing(FALSE);            
        }
        */
        break;
        
    case MSG_CLOSE_WINDOW:
        //关闭窗口之前关闭所有的震动和铃声
        MMIENVSET_StopVibraAndRing(ring_type);
        MMIAPISET_StopAppRing();

        MMK_FreeWinAddData(win_id);
        
        break;
        
    default:
        
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
} 


/*****************************************************************************/
//Description :来电提示方式的选择窗口处理函数 
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/
LOCAL     MMI_RESULT_E  HandleCallRingTypeWindow(
                                                 MMI_WIN_ID_T    win_id,  // 窗口ID
                                                 MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                 DPARAM             param    // 消息参数
                                                 )
{
    MMI_RESULT_E  temp = MMI_RESULT_TRUE;
    temp = HandleAllRingTypeWindow(win_id,
                                    msg_id,
                                    MMISET_RING_TYPE_CALL,
                                    CALL_RING_TYPE,
                                    MMIENVSET_CALL_RING_TYPE_LISTBOX__CTRL_ID,
                                    param);
    return temp;
}

/*****************************************************************************/
//Description : 信息提示方式的选择窗口处理函数
//Global resource dependence : 
//Author:maqianlin
//Note: 
/*****************************************************************************/
LOCAL      MMI_RESULT_E  HandleMsgRingTypeWindow(
                                                 MMI_WIN_ID_T    win_id,  // 窗口ID
                                                 MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                 DPARAM             param    // 消息参数
                                                 )
{
    MMI_RESULT_E  temp = MMI_RESULT_TRUE;
    
    temp = HandleAllRingTypeWindow(win_id,
        msg_id,
        MMISET_RING_TYPE_MSG,
        MSG_RING_TYPE,
        MMIENVSET_MSG_RING_TYPE_LISTBOX_CTRL_ID,
        param);
    return temp;
}



/*****************************************************************************/
//Description :  闹钟提示方式的选择窗口处理函数
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/

LOCAL     MMI_RESULT_E  HandleAlarmRingTypeWindow(
                                                  MMI_WIN_ID_T    win_id,  // 窗口ID
                                                  MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                  DPARAM             param    // 消息参数
                                                  )
{
    MMI_RESULT_E  temp = MMI_RESULT_TRUE;
    temp =HandleAllRingTypeWindow(win_id,
                                msg_id,
                                MMISET_RING_TYPE_ALARM,
                                ALARM_RING_TYPE,
                                MMIENVSET_RING_ALARM_TYPE_LISTBOX_CTRL_ID,
                                param);
    return temp;
    
}


//*****************************************************************************/
//Description :  按键音的选择窗口处理函数
//Global resource dependence : 
//Author:  maqianlin
//Note: 
/*****************************************************************************/

LOCAL     MMI_RESULT_E  HandleKeyRingTypeWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_RESULT_E    temp    =   MMI_RESULT_TRUE;
    temp =  HandleAllRingTypeWindow(win_id,
                                    msg_id,
                                    MMISET_RING_TYPE_KEY,
                                    KEY_RING_TYPE,
                                    MMIENVSET_KEY_RING_TYPE_LISTBOX_CTRL_ID,
                                    param);
    return temp ;
}

#ifdef LCD_SLIDE_SUPPORT

/*****************************************************************************/
//Description : // 滑盖音的选择窗口处理函数
//Global resource dependence : 
//Author:  jianhuiluo
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleSlideRingSwitchWindow(
                                                     MMI_WIN_ID_T    win_id,  // 窗口ID
                                                     MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                     DPARAM             param    // 消息参数
                                                     )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIENVSET_SLIDE_TONE_MENU_CTRL_ID;
    
    param = param;
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
		GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
		GUIWIN_SeSoftkeytButtonIconId(win_id, IMAGE_NULL, 1, FALSE);
#else
		GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
#endif
        //激活控件
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
	case MSG_CTL_MIDSK:
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        
        HandleOperationMenu(ctrl_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

//*****************************************************************************/
//Description :  开盖音的选择窗口处理函数
//Global resource dependence : 
//Author: jianhui.luo
//Note: 
/*****************************************************************************/
LOCAL     MMI_RESULT_E  HandleSlideRingOnWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_RESULT_E  temp = MMI_RESULT_TRUE;
    temp =	HandleAllRingTypeWindow(win_id,
        msg_id,
        MMISET_RING_TYPE_SLIDE_ON,
        SLIDE_RING_ON_TYPE,
        MMIENVSET_SLIDE_RING_ON_TYPE_LISTBOX_CTRL_ID,
        param);
    return temp ;
}

//*****************************************************************************/
//Description :  合盖音的选择窗口处理函数
//Global resource dependence : 
//Author: jianhui.luo
//Note: 
/*****************************************************************************/
LOCAL     MMI_RESULT_E  HandleSlideRingOffWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_RESULT_E  temp = MMI_RESULT_TRUE;
    temp =	HandleAllRingTypeWindow(win_id,
        msg_id,
        MMISET_RING_TYPE_SLIDE_OFF,
        SLIDE_RING_OFF_TYPE,
        MMIENVSET_SLIDE_RING_OFF_TYPE_LISTBOX_CTRL_ID,
        param);
    return temp ;
}

#endif

//*****************************************************************************/
//Description :  触屏音的选择窗口处理函数
//Global resource dependence : 
//Author: jian.ma
//Note: 
/*****************************************************************************/
LOCAL     MMI_RESULT_E  HandleTKeyRingTypeWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_RESULT_E    temp    =   MMI_RESULT_TRUE;
    temp =  HandleAllRingTypeWindow(win_id,
                                    msg_id,
                                    MMISET_RING_TYPE_TKEY,
                                    TKEY_RING_TYPE,
                                    MMIENVSET_TKEY_RING_TYPE_LISTBOX_CTRL_ID,
                                    param);
    return temp ;
}

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone. 
/*****************************************************************************/
//Description : 翻盖提示音的选择窗口处理函数
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/

LOCAL     MMI_RESULT_E  HandleFlipRingTypeWindow(
                                                 MMI_WIN_ID_T    win_id,  // 窗口ID
                                                 MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                 DPARAM             param    // 消息参数
                                                 )
{
    MMI_RESULT_E    temp    =   MMI_RESULT_TRUE;
    temp =  HandleAllRingTypeWindow(win_id,
                                    msg_id,
                                    MMISET_RING_TYPE_FLIP,
                                    FLIP_RING_TYPE,
                                    MMIENVSET_FLIP_RING_TYPE_LISTBOX_CTRL_ID,
                                    param);
    return temp;
}
/*****************************************************************************/
//Description : 翻盖提示音的选择窗口处理函数
//Global resource dependence : 
//Author: maqianlin
//Note: 
/*****************************************************************************/

LOCAL     MMI_RESULT_E  HandleFlipDownRingTypeWindow(
                                                 MMI_WIN_ID_T    win_id,  // 窗口ID
                                                 MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                 DPARAM             param    // 消息参数
                                                 )
{
    MMI_RESULT_E    temp    =   MMI_RESULT_TRUE;
    temp =  HandleAllRingTypeWindow(win_id,
                                    msg_id,
                                    MMISET_RING_TYPE_FLIP_DOWN,
                                    FLIP_DOWN_RING_TYPE,
                                    MMIENVSET_FLIP_DOWN_RING_TYPE_LISTBOX_CTRL_ID,
                                    param);
    return temp;
}
#endif

/*****************************************************************************/
//Description : 开关机铃声的选择窗口处理函数
//Global resource dependence : 
//Author:  maqianlin
//Note: 
/*****************************************************************************/
//
//LOCAL      MMI_RESULT_E  HandlePowerRingTypeWindow(
//                                                   MMI_WIN_ID_T    win_id,  // 窗口ID
//                                                   MMI_MESSAGE_ID_E   msg_id,  // 消息ID
//                                                   DPARAM             param    // 消息参数
//                                                   )
//{
//    MMI_RESULT_E  temp = MMI_RESULT_TRUE;
//    /*
//    temp =    HandleAllRingTypeWindow(win_id,
//    msg_id,
//    MMISET_RING_TYPE_POWER,
//    POWER_RING_TYPE,
//    MMIENVSET_POWER_RING_TYPE_LISTBOX_CTRL_ID,
//    param);
//    */
//    return temp;
//}
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description : 开关机声音开启和关闭的窗口处理函数
//Global resource dependence : 
//Author: kelly.li
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandlePowerRingSwitchWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    uint16          cur_selection   =   0;  
    
    //avoid warning
    param = param; 
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        // 设置最大列表项
        GUILIST_SetMaxItem(MMIENVSET_POWER_RING_SWICTH_LISTBOX_CTRL_ID,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        // append listbox
        MMIENVSET_AppendPowerRingSwitchListItem(MMIENVSET_POWER_RING_SWICTH_LISTBOX_CTRL_ID);
        
        MMK_SetAtvCtrl(win_id,MMIENVSET_POWER_RING_SWICTH_LISTBOX_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        // 获得选择的开关量 
        GUILIST_GetSelectedItemIndex(MMIENVSET_POWER_RING_SWICTH_LISTBOX_CTRL_ID,&cur_selection,1);
        // 保存开关机声音的选项值
        MMIENVSET_SetCurModeOptValue(POWER_RING_SWITCH,(uint8)cur_selection, MMIAPISET_GetActiveSim());
        //success messagebox
       // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:                                               
        
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}



/*****************************************************************************/
//Description : 增强声音开启和关闭的窗口处理函数
//Global resource dependence : 
//Author: kelly.li
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEnhanceRingSwitchWindow(
                                                MMI_WIN_ID_T    win_id,  // 窗口ID
                                                MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                DPARAM             param    // 消息参数
                                                )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    uint16          cur_selection = 0;	
    MMI_CTRL_ID_T ctrl_id = MMIENVSET_ENHANCE_RING_SWICTH_LISTBOX_CTRL_ID;
    //avoid warning
    param = param; 
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        // 设置最大列表项
        GUILIST_SetMaxItem(ctrl_id,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        // 添加列表项
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
        // 获得开关机声音的选项值
        cur_selection = MMIAPIENVSET_GetEnhanceRingState();
    
        // 显示选中项
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
        // 显示阴影条
        GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
        
        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        // 获得选择的开关量 
        GUILIST_GetSelectedItemIndex(ctrl_id,&cur_selection,1);
        // 保存开关机声音的选项值
        MMIAPIENVSET_SetEnhanceRingState(cur_selection);

        if(MMIAPIENVSET_GetEnhanceRingState())
        {
            //if(AUDIO_DEVICE_MODE_HANDFREE == MMIAUDIO_GetCurDevMode())
            {
                //MMIAPISET_InitEqMode();
                //MMIAUDIO_StartAGC();
            }
        }
        else
        {
            //MMIAUDIO_StopAGC();
            //MMIAPIKUR_RestoreEqSetting();
            //MMIAPIMP3_RestoreEqSetting();
        }
        //success messagebox
       // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:                                               
        
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}
/*****************************************************************************/
//Description : // 下载完成提示音的选择窗口处理函数
//Global resource dependence : 
//Author:  Jiaoyou.wu
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleDownloadNotifySwitchWindow(
                                                     MMI_WIN_ID_T    win_id,  // 窗口ID
                                                     MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                     DPARAM             param    // 消息参数
                                                     )

{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    uint16          cur_selection   =   0;  

    //avoid warning
    param = param; 

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetMaxItem(MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_LISTBOX_CTRL_ID,MMISET_OPEN_CLOSE_ITEM, FALSE );
        MMIENVSET_DownloadCompleteNotify_AppendListItem(MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_LISTBOX_CTRL_ID);
        MMK_SetAtvCtrl(win_id, MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_LISTBOX_CTRL_ID);
        break;

    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        GUILIST_GetSelectedItemIndex(MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_LISTBOX_CTRL_ID, &cur_selection, 1);
        MMIENVSET_SetCurModeOptValue(DOWNLOAD_NOTIFY, (uint8)cur_selection, MMIAPISET_GetActiveSim());
      //  MMIPUB_OpenAlertSuccessWin( TXT_COMPLETE);
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);      
        break;

    default:                                               
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//Description : // 低电压警告音的选择窗口处理函数
//Global resource dependence : 
//Author:  maqianlin
//Note: 
/*****************************************************************************/

LOCAL MMI_RESULT_E  HandleLowVoltageWarnSwitchWindow(
                                                     MMI_WIN_ID_T    win_id,  // 窗口ID
                                                     MMI_MESSAGE_ID_E   msg_id,  // 消息ID
                                                     DPARAM             param    // 消息参数
                                                     )
{
    MMI_RESULT_E    recode          =   MMI_RESULT_TRUE;
    uint16          cur_selection   =   0;  
    
    //avoid warning
    param = param; 
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        // 设置最大列表项
        GUILIST_SetMaxItem(MMIENVSET_LOW_VOLTAGE_WARN_SWITCH_LISTBOX_CTRL_ID,MMISET_OPEN_CLOSE_ITEM, FALSE );//max item 2
        // append listbox
        MMIENVSET_VoltWarn_AppendListItem(MMIENVSET_LOW_VOLTAGE_WARN_SWITCH_LISTBOX_CTRL_ID);
        
        MMK_SetAtvCtrl(win_id,MMIENVSET_LOW_VOLTAGE_WARN_SWITCH_LISTBOX_CTRL_ID);
        break;
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
        // 获得选择的开关量 
        GUILIST_GetSelectedItemIndex(MMIENVSET_LOW_VOLTAGE_WARN_SWITCH_LISTBOX_CTRL_ID,&cur_selection,1);
        // 保存低电压报警开关量
        MMIENVSET_SetCurModeOptValue(VOLTAGE_WARN,(uint8)cur_selection, MMIAPISET_GetActiveSim());
        //success messagebox
       // MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
        
        MMK_CloseWin( win_id );
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin( win_id );      
        break;
        
    default:                                               
        
        recode = MMI_RESULT_FALSE;
        break;
    }
    
    return recode;
}

/*****************************************************************************/
//  Description : to handle window message
//  Global resource dependence : 
//  Author:bown.zhang
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  EnvSetSelectSimCallback(uint32 dual_sys, BOOLEAN isReturnOK, DPARAM param)
{
    MMI_RESULT_E        recode          =   MMI_RESULT_TRUE;

    if (isReturnOK)
    {
        s_dual_sys = (MN_DUAL_SYS_E)dual_sys;
        if (s_ring_type == MSG_RING_TYPE)
        {
            MMK_CreatePubListWin((uint32 *)MMIENVSET_MSG_RING_TYPE_WIN_TAB, (ADD_DATA)s_dual_sys);
        }
        else if (CALL_RING_TYPE == s_ring_type)
        {
            MMK_CreatePubListWin((uint32 *)MMIENVSET_CALL_RING_TYPE_WIN_TAB, (ADD_DATA)s_dual_sys);
        }
    }
    
    return (recode);
}
#endif
/*****************************************************************************/
//  Description : play music file 
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
LOCAL void MMIENVSET_AdjustAudioVol(
                    MMISET_ALL_RING_TYPE_E  ring_type,
                    uint8                   volume
                    )
{
    MMIAPISET_InitAudioDeviceExt(ring_type,volume);
}

/*****************************************************************************/
//Description : 音量调节窗口:来电铃声，信息铃声...都在这个窗口进
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleAllRingVolumeWindow(
                                          MMI_WIN_ID_T            win_id,               // 窗口ID
                                          MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                          DPARAM                     param
                                          )
{
    MMI_RESULT_E              recode  = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T             ctrl_id = 0;
#ifndef MMI_ENVSET_MINI_SUPPORT
    uint32                    menu_id = 0;
#endif
    MMIENVSET_SET_OPT_E       set_opt = ENVSET_MAX_OPT;
    MMICOM_PANEL_INFO_T       panel_info = {0};
    MMISET_ALL_RING_TYPE_E    ring_type = MMISET_RING_TYPE_CALL;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if !defined (MMI_PDA_SUPPORT)
		GUIFORM_SetCircularHandleUpDown(MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID,TRUE);
#endif
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        SetAllRingVolParam();
        MMK_SetAtvCtrl(win_id, MMIENVSET_CALL_RING_VOL_SET_EDIT_LABEL_CTRL_ID);
        break;
        
#if defined (MMI_PDA_SUPPORT) || defined(MMI_GUI_STYLE_MINISCREEN) //NEWMS00123716 and NEWMS00222755 
    case MSG_GET_FOCUS:
        SetAllRingVolParam();
        break;
#endif
    case MSG_KEYDOWN_UPSIDE:
    case MSG_KEYREPEAT_UPSIDE:
    case MSG_KEYDOWN_DOWNSIDE:
    case MSG_KEYREPEAT_DOWNSIDE:
        break;
    case MSG_FULL_PAINT:
       if(!MMIAPICOM_IsPanelOpened())
       {
           MMIAPISET_StopAppRing();
       }
        break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_APP_OK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT//coverity10066
        if(MSG_CTL_PENOK == msg_id)
        {
            ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
#endif
        {
            ctrl_id= MMK_GetActiveCtrlId(win_id);
        }


        switch(ctrl_id)
        {
        case MMIENVSET_CALL_RING_VOL_SET_EDIT_LABEL_CTRL_ID://来电
#ifndef MMI_ENVSET_MINI_SUPPORT
            menu_id = ID_ENVSET_CALL_RING_VOL;
#endif
            set_opt = CALL_RING_VOL;
            ring_type = MMISET_RING_TYPE_CALL;
            panel_info.min_value = MMISET_VOL_ZERO;
            panel_info.max_value = MMISET_VOL_MAX;
            break;
            
        case MMIENVSET_MSG_RING_VOL_SET_EDIT_LABEL_CTRL_ID://信息
#ifndef MMI_ENVSET_MINI_SUPPORT
            menu_id = ID_ENVSET_MSG_RING_VOL;
#endif
            set_opt = MSG_RING_VOL;
            ring_type = MMISET_RING_TYPE_MSG;
            panel_info.min_value = MMISET_VOL_ZERO;
            panel_info.max_value = MMISET_VOL_MAX;
            break;

        case MMIENVSET_ALARM_RING_VOL_SET_EDIT_LABEL_CTRL_ID://闹钟
#ifndef MMI_ENVSET_MINI_SUPPORT
            menu_id = ID_ENVSET_ALARM_RING_VOL;
#endif
            set_opt = ALARM_RING_VOL;
            ring_type = MMISET_RING_TYPE_ALARM;
            panel_info.min_value = MMISET_VOL_ZERO;
            panel_info.max_value = MMISET_VOL_MAX;
            break;

        case MMIENVSET_COM_RING_VOL_SET_EDIT_LABEL_CTRL_ID://通话
#ifndef MMI_ENVSET_MINI_SUPPORT
            menu_id = ID_ENVSET_COMM_VOL;
#endif
            set_opt = COMM_VOL;
            ring_type = MMISET_RING_TYPE_NONE;//默认?
            panel_info.min_value = MMISET_VOL_ONE;
            panel_info.max_value = MMISET_VOL_MAX;
            break;

        case MMIENVSET_POWER_ONOFF_RING_VOL_SET_EDIT_LABEL_CTRL_ID://开关机
#ifndef MMI_ENVSET_MINI_SUPPORT
            menu_id = ID_ENVSET_POWER_RING_VOL;
#endif
            if((MMIENVSET_SILENT_MODE != MMIENVSET_GetCurModeId())
               && (MMIENVSET_MEETING_MODE != MMIENVSET_GetCurModeId()))
            {
#ifndef MMI_ENVSET_MINI_SUPPORT
                menu_id = ID_ENVSET_POWER_RING_VOL;
#endif
                set_opt = POWER_RING_VOL;
                ring_type = MMISET_RING_TYPE_POWER;
                panel_info.min_value = MMISET_VOL_ZERO;
                panel_info.max_value = MMISET_VOL_MAX;
            }
            break;

#ifdef MMI_READMENU_ENABLE
        case MMIENVSET_MENU_READ_VOL_SET_EDIT_LABEL_CTRL_ID://菜单朗读
#ifndef MMI_ENVSET_MINI_SUPPORT
            menu_id = ID_ENVSET_MENUREAD_VOL;
#endif
            set_opt = MENUREAD_VOL;
            ring_type = MMISET_RING_TYPE_READMENU;
            panel_info.min_value = MMISET_VOL_ZERO;
            panel_info.max_value = MMISET_VOL_MAX;
            break;
#endif

        default:
            panel_info.min_value = MMISET_VOL_ZERO;
            panel_info.max_value = MMISET_VOL_MAX;
            break;
        }

        if(set_opt != ENVSET_MAX_OPT)
        {
            //s_dual_sys = MMIAPISET_GetActiveSim();
            panel_info.x = -1;
            panel_info.y = -1;
            panel_info.cur_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1, set_opt);
            panel_info.last_value = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1, set_opt);
            //panel_info.min_value = MMISET_VOL_ZERO;
            //panel_info.max_value = MMISET_VOL_MAX;
            panel_info.set_callback = SetRingVolumeCB;
     
            if(MMIAPICOM_IsPanelOpened()) 
            {
                MMIAPICOM_ClosePanelChildWin();
            }
            
            s_ring_type = set_opt;
            s_set_ring = ring_type;
            MMIAPICOM_OpenPanelChildWin(MMK_GetFocusWinId(), &panel_info);
			if(s_set_ring !=  MMISET_RING_TYPE_NONE)
			{
//				MMISRVAUD_SetMuteNohandle(TRUE);
				MMIENVSET_PreviewRingByVol(MN_DUAL_SYS_1, s_set_ring, panel_info.cur_value, MMIENVSET_PREVIEW_PLAY_DEFAUT_TIMES);
//				MMISRVAUD_SetMuteNohandle(FALSE);
			}

        }

        //MMK_CreateWin((uint32 *)MMIENVSET_RING_VOL_WIN_TAB,(ADD_DATA)menu_id);
        break;
#ifdef MMI_GUI_STYLE_MINISCREEN
    case MSG_KEYUP_DOWN:
    case MSG_KEYUP_UP:
        if(!MMIAPICOM_IsPanelOpened() && MMIAPISET_IsPlayingAppRing() && (MMISET_RING_TYPE_KEY != MMIAPISET_GetCurRingType())) 
        {
            MMIAPISET_StopAppRing();
        }
    break;
#endif
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        if(MMIAPISET_IsPlayingAppRing() && (MMISET_RING_TYPE_KEY != MMIAPISET_GetCurRingType()))
        {
            MMIAPISET_StopAppRing();
        }
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
/*****************************************************************************/
//Description : 设置音量调节label的参数
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL  void  SetAllRingVolParam(void)
{
    uint8                      s_cur_vol   =   0;
    uint16                     i           =   0;
    MMI_STRING_T               text        =   {0};
    wchar                      ch[3]       =   {0};

    MMI_CTRL_ID_T ctrl_id[]=
    {
        MMIENVSET_CALL_RING_VOL_SET_EDIT_LABEL_CTRL_ID,
        MMIENVSET_MSG_RING_VOL_SET_EDIT_LABEL_CTRL_ID,
        MMIENVSET_ALARM_RING_VOL_SET_EDIT_LABEL_CTRL_ID,
        MMIENVSET_COM_RING_VOL_SET_EDIT_LABEL_CTRL_ID,
        MMIENVSET_POWER_ONOFF_RING_VOL_SET_EDIT_LABEL_CTRL_ID,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_SET_EDIT_LABEL_CTRL_ID,
#endif
    };
    MMIENVSET_SET_OPT_E set_opt[]=
    {
        CALL_RING_VOL, 
        MSG_RING_VOL,
        ALARM_RING_VOL,
        COMM_VOL,
        POWER_RING_VOL,
#ifdef MMI_READMENU_ENABLE
        MENUREAD_VOL,
#endif
    };
    GUIFORM_SetStyle(MMIENVSET_CALL_RING_VOL_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIENVSET_MSG_RING_VOL_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIENVSET_ALARM_RING_VOL_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIENVSET_COM_RING_VOL_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIENVSET_POWER_ONOFF_RING_VOL_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#ifdef MMI_READMENU_ENABLE
    GUIFORM_SetStyle(MMIENVSET_MENU_READ_VOL_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
#endif

    for(i=0; i<ARR_SIZE(ctrl_id);i++)
    {
        s_cur_vol = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1, set_opt[i]);
        SCI_MEMSET(ch, 0, sizeof(ch));  
		if(s_cur_vol < 10)
        {
			ch[0] = s_cur_vol + '0';
			text.wstr_ptr = ch;
			text.wstr_len = 1;
		}
		else
		{
			ch[1] = (s_cur_vol-10) + '0';
			ch[0] = '1';
			text.wstr_ptr = ch;
			text.wstr_len = 2;
		}
        GUILABEL_SetText(ctrl_id[i], &text, FALSE);
    }
    GUILABEL_SetTextById(MMIENVSET_CALL_RING_VOL_SET_LABEL_CTRL_ID, TXT_SET_RING_SELECT_CALL, FALSE);
    GUILABEL_SetTextById(MMIENVSET_MSG_RING_VOL_SET_LABEL_CTRL_ID, TXT_SET_RING_SELECT_MSG, FALSE);
    GUILABEL_SetTextById(MMIENVSET_ALARM_RING_VOL_SET_LABEL_CTRL_ID, TXT_RING_SELECT_ALARM, FALSE);
    GUILABEL_SetTextById(MMIENVSET_COM_RING_VOL_SET_LABEL_CTRL_ID, TXT_SET_RING_VOLUME_COMM, FALSE);
    GUILABEL_SetTextById(MMIENVSET_POWER_ONOFF_RING_VOL_SET_LABEL_CTRL_ID, TXT_ENVSET_POWER_RING_TYPE, FALSE);
#ifdef MMI_READMENU_ENABLE
    GUILABEL_SetTextById(MMIENVSET_MENU_READ_VOL_SET_LABEL_CTRL_ID, TXT_SET_RING_VOLUME_MENUREAD, FALSE);    
#endif

    if((MMIENVSET_SILENT_MODE == MMIENVSET_GetCurModeId())
    ||(MMIENVSET_MEETING_MODE == MMIENVSET_GetCurModeId()))
    {
        GUIFORM_SetChildDisplay(MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID, MMIENVSET_POWER_ONOFF_RING_VOL_SET_FORM_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
    }
        
    if(MMIAPICC_IsInState(CC_IN_CALL_STATE))
    { 
        GUIFORM_SetChildDisplay(MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID, MMIENVSET_COM_RING_VOL_SET_FORM_CTRL_ID, GUIFORM_CHILD_DISP_GRAY);
    }
    else
    {
        GUIFORM_SetChildDisplay(MMIENVSET_ALL_RING_VOL_SET_FORM_CTRL_ID, MMIENVSET_COM_RING_VOL_SET_FORM_CTRL_ID, GUIFORM_CHILD_DISP_NORMAL);
    }
}
/*****************************************************************************/
//Description : 响铃方式窗口处理函数
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleRingTypeSetWindow(
                                          MMI_WIN_ID_T            win_id,               // 窗口ID
                                          MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                          DPARAM                     param
                                          )
{
    MMI_RESULT_E              recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T             ctrl_id         =   0;
    uint8                     offset          =   0;
    
	switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if !defined (MMI_PDA_SUPPORT)
		GUIFORM_SetCircularHandleUpDown(MMIENVSET_RING_TYPE_FORM_CTRL_ID,TRUE);
#endif
		SetRingTypeParam(win_id);
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMIENVSET_RING_TYPE_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
		MMK_SetAtvCtrl(win_id, MMIENVSET_SIM_CALL_RING_TYPE_LABEL_CTRL_ID+1);
        break;
        
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		{
			MMIENVSET_ADD_DATA_T *add_data_ptr = (MMIENVSET_ADD_DATA_T*)SCI_ALLOC_APP(sizeof(MMIENVSET_ADD_DATA_T));
#ifdef TOUCH_PANEL_SUPPORT //coverity 10068
			if(MSG_CTL_PENOK == msg_id)
			{
				ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
			}
			else
#endif
			{
				ctrl_id = MMK_GetActiveCtrlId(win_id);
			}
			add_data_ptr->ctrl_id = ctrl_id;
			offset = ctrl_id - MMIENVSET_SIM_CALL_RING_TYPE_LABEL_CTRL_ID;
			if(1 == offset%2)
			{
				if(offset/2 < MMI_DUAL_SYS_MAX)
				{
					s_dual_sys = offset/2;
					add_data_ptr->s_dual_sys = s_dual_sys;
					MMK_CreatePubListWin((uint32 *)MMIENVSET_CALL_RING_TYPE_WIN_TAB, add_data_ptr);    
				}
				else if(MMI_DUAL_SYS_MAX <= offset/2 && 2*MMI_DUAL_SYS_MAX > offset/2)
				{
                    s_dual_sys = offset/2 - MMI_DUAL_SYS_MAX;//cr227833 maryxiao 
					add_data_ptr->s_dual_sys = s_dual_sys;
					MMK_CreatePubListWin((uint32 *)MMIENVSET_MSG_RING_TYPE_WIN_TAB, add_data_ptr);    
				}
				else
				{
					MMK_CreatePubListWin((uint32 *)MMIENVSET_ALARM_RING_TYPE_WIN_TAB,add_data_ptr);
				}
			}
		}
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
	}
    return recode;
}

/*****************************************************************************/
//Description : 设置响铃方式的控件值
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SetRingTypeParam(MMI_WIN_ID_T            win_id)
{
    uint8 loop     =   0;
    uint8 index    =   0;
    GUIFORM_DYNA_CHILD_T  child_data = {0};
    GUIFORM_DYNA_CHILD_T  child_form_data = {0};
    GUILABEL_INIT_DATA_T text_label_init = {0};
    GUIFORM_INIT_DATA_T     init_data = {0};
    MMI_CTRL_ID_T   ctrl_id = MMIENVSET_RING_TYPE_FORM_CTRL_ID;
    #ifdef MMI_MULTI_SIM_SYS_SINGLE
    uint32 title_id[]=
    {
        TXT_SET_RING_CALL_TYPE,
        TXT_SET_RING_MSG_TYPE,    
        TXT_ENVSET_ALARM_RING_TYPE
    };
    #else
    uint32 title_id[]=
    {
        TXT_SET_SIM1_RING_CALL_TYPE,
        TXT_SET_SIM2_RING_CALL_TYPE,
        #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SET_SIM3_RING_CALL_TYPE,
        #ifdef MMI_MULTI_SIM_SYS_QUAD
        TXT_SET_SIM4_RING_CALL_TYPE,
        #endif
        #endif
        TXT_SET_SIM1_RING_MSG_TYPE,
        TXT_SET_SIM2_RING_MSG_TYPE,
        #if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        TXT_SET_SIM3_RING_MSG_TYPE,
        #ifdef MMI_MULTI_SIM_SYS_QUAD
        TXT_SET_SIM4_RING_MSG_TYPE,
        #endif
        #endif
        TXT_ENVSET_ALARM_RING_TYPE
    };
	#endif
    uint32 active_call_title_id[]=
    {
        TXT_SET_RING_RING,
        TXT_RING_VIBRA,
        TXT_RING_SILENT,
        TXT_SET_RING_VIBRA_BEFORE_RING,
        TXT_SET_RING_VIBRA_AND_RING
    };
    uint32 active_msg_title_id[]=
    {
        TXT_SET_RING_RING,
        TXT_RING_VIBRA,
        TXT_RING_SILENT,
        TXT_SET_RING_VIBRA_AND_RING
    };
    for(loop = MN_DUAL_SYS_1; loop <  (4 * MMI_DUAL_SYS_MAX +2); loop++)//日程闹钟铃声也在内，所以要"+2"
    {
        if(0 == loop%2) //当loop = 0，2，4，6...时，创建一个可focus的子form
        {
            child_form_data.child_handle = MMIENVSET_RING_TYPE_CHILD_FORM_CTRL_ID + loop/2;
            child_form_data.guid = SPRD_GUI_FORM_ID;
            child_form_data.is_get_active = TRUE;
            child_form_data.is_bg       =   FALSE;
            init_data.layout_type = GUIFORM_LAYOUT_ORDER;
            child_form_data.init_data_ptr = &init_data;
            GUIFORM_CreatDynaChildCtrl(win_id, ctrl_id, &child_form_data);
            GUIFORM_SetStyle(child_form_data.child_handle,GUIFORM_STYLE_UNIT);
        }
        //开始创建子form的子label：分为可active和不可active两种
        child_data.child_handle = MMIENVSET_SIM_CALL_RING_TYPE_LABEL_CTRL_ID + loop;
        child_data.guid = SPRD_GUI_LABEL_ID;
        if(0 == loop%2)//loop = 0,2,4,6...时，创建不可active的label
        {
            child_data.is_get_active = FALSE;
            text_label_init.align = GUILABEL_ALIGN_LEFT;
        }
        else if(1 == loop%2) //loop = 1,3,5,7...时，创建可active的label控件
        {
            child_data.is_get_active = TRUE;
            text_label_init.align = GUILABEL_ALIGN_DEFAULT;
        }
        child_data.is_bg       =   FALSE;
        child_data.init_data_ptr = &text_label_init;
        GUIFORM_CreatDynaChildCtrl(win_id, child_form_data.child_handle , &child_data);
        if(child_data.is_get_active)
        {
            if(loop/2 < MMI_DUAL_SYS_MAX)
            {
                s_dual_sys = loop/2;
                index = (uint16)MMIAPIENVSET_GetCurModeOptValue(s_dual_sys, CALL_RING_TYPE);
                GUILABEL_SetTextById(child_data.child_handle, active_call_title_id[index], FALSE);
            }
            else if(MMI_DUAL_SYS_MAX <= loop/2 && loop/2 < 2*MMI_DUAL_SYS_MAX)
            {
                s_dual_sys = loop/2 - MMI_DUAL_SYS_MAX;
                index = (uint16)MMIAPIENVSET_GetCurModeOptValue(s_dual_sys, MSG_RING_TYPE);
                GUILABEL_SetTextById(child_data.child_handle, active_msg_title_id[index], FALSE);
            }
            else
            {
                index = (uint16)MMIAPIENVSET_GetCurModeOptValue(s_dual_sys, ALARM_RING_TYPE);
                GUILABEL_SetTextById(child_data.child_handle, active_msg_title_id[index], FALSE);
            }
        }
        else
        {
            GUILABEL_SetTextById(child_data.child_handle, title_id[loop/2], FALSE);//title
        }
    }
}
/*****************************************************************************/
//Description : other ring set 
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleOtherRingSetWindow(
                                          MMI_WIN_ID_T            win_id,               // 窗口ID
                                          MMI_MESSAGE_ID_E           msg_id,               // 消息ID
                                          DPARAM                     param
                                          )
{
    MMI_RESULT_E              recode          =   MMI_RESULT_TRUE;
    MMI_CTRL_ID_T             ctrl_id         =   0;
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
#if !defined (MMI_PDA_SUPPORT)
		GUIFORM_SetCircularHandleUpDown(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,TRUE);
#endif        
#ifndef MMI_GUI_STYLE_TYPICAL
        GUIFORM_SetType(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,GUIFORM_TYPE_TP);
#endif
        SetKeypadRingParam();
#ifdef TOUCH_PANEL_SUPPORT
        SetTouchpanelRingParam();
        SetDialPanelTouchVibrate();
        SetTouchKeyLockRingParam();
#endif
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        SetDialTone();
#endif
#ifdef MMI_READMENU_ENABLE
        SetMenuRead();
#endif
#ifdef FLIP_PHONE_SUPPORT
        SetFlipRingParam();
#endif
        SetPowerOnOffRingParam();
        SetLowBatteryRingParam();    
        if((MMIENVSET_GetCurModeId() == MMIENVSET_SILENT_MODE)
        ||(MMIENVSET_GetCurModeId() == MMIENVSET_MEETING_MODE))
        {
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_KEYPAD_RING_SET_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#ifdef TOUCH_PANEL_SUPPORT
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_TOUCH_PANEL_RING_SET_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#endif           
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_LOW_BATTERY_RING_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_POWER_ONOFF_RING_SET_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#ifdef TOUCH_PANEL_SUPPORT
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_TOUCH_KEY_LOCK_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_DIAL_TONE_SET_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#endif
#ifdef FLIP_PHONE_SUPPORT
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_FLIP_RING_SET_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_FLIP_DOWN_RING_SET_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#endif
			MMK_SetAtvCtrl(win_id, MMIENVSET_TOUCH_VIBRATE_SETLIST_CTRL_ID);
#else
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
            GUIFORM_SetChildDisplay(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,MMIENVSET_DIAL_TONE_SET_FORM_CTRL_ID,GUIFORM_CHILD_DISP_GRAY);
#endif 
			MMK_SetAtvCtrl(win_id, MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID);
			GUIFORM_SetCircularHandleUpDown(MMIENVSET_OTHER_RING_SET_FORM_CTRL_ID,FALSE);           
#endif			
        }
        else
        {
            MMK_SetAtvCtrl(win_id, MMIENVSET_KEYPAD_RING_SET_EDIT_LABEL_CTRL_ID);
        }
        break;
    case MSG_GET_FOCUS://for cr NEWMS00131734
        SaveOtherRingTypeSet();
        break;        
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_NOTIFY_MIDSK:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
        
        if(MSG_CTL_PENOK == msg_id)
        {
            ctrl_id = ((MMI_NOTIFY_T*) param)->src_id;
        }
        else
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527 //coverity 10067
        {
            ctrl_id = MMK_GetActiveCtrlId(win_id);
        }
        switch(ctrl_id)
        {
        case MMIENVSET_KEYPAD_RING_SET_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubListWin((uint32 *)MMIENVSET_KEY_RING_TYPE_WIN_TAB,PNULL);
            break;
#ifdef FLIP_PHONE_SUPPORT
        case MMIENVSET_FLIP_RING_SET_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubListWin((uint32 *)MMIENVSET_FLIP_RING_TYPE_WIN_TAB,PNULL);
            break;
        case MMIENVSET_FLIP_DOWN_RING_SET_EDIT_LABEL_CTRL_ID:
            MMK_CreatePubListWin((uint32 *)MMIENVSET_FLIP_DOWN_RING_TYPE_WIN_TAB,PNULL);
            break;
#endif
        default:
            break;

        }
        break;

    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        SaveOtherRingTypeSet();
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_RED:
        SaveOtherRingTypeSet();
        recode = MMI_RESULT_FALSE;
        break;

    default:
        recode = MMI_RESULT_FALSE;
        break;
        

    }
    return recode;
}
/*****************************************************************************/
//Description :按键音的参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SetKeypadRingParam(void)
{
    uint16      cur_selection =0;
    uint32      text_id[]     =
	{
	    TXT_RING_SILENT,
		TXT_COMMON_RING_RING1,
		TXT_COMMON_RING_RING2
	};
    GUIFORM_SetStyle(MMIENVSET_KEYPAD_RING_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_KEYPAD_RING_SET_LABEL_CTRL_ID,TXT_SET_RING_KEY_TONE,FALSE);
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, KEY_RING_TYPE);
    GUILABEL_SetTextById(MMIENVSET_KEYPAD_RING_SET_EDIT_LABEL_CTRL_ID,text_id[cur_selection],FALSE);      
}
#ifdef FLIP_PHONE_SUPPORT
/*****************************************************************************/
//Description :按键音的参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SetFlipRingParam(void)
{
    uint16      cur_selection =0;
    uint32      text_id[]     =
	{
	    TXT_RING_SILENT,
		TXT_COMMON_RING_RING1,
		TXT_COMMON_RING_RING2
	};
    GUIFORM_SetStyle(MMIENVSET_FLIP_RING_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUIFORM_SetStyle(MMIENVSET_FLIP_DOWN_RING_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_FLIP_RING_SET_LABEL_CTRL_ID,TXT_SET_RING_FLIP_RING,FALSE);
    GUILABEL_SetTextById(MMIENVSET_FLIP_DOWN_RING_SET_LABEL_CTRL_ID,TXT_SET_RING_FLIP_DOWN_RING,FALSE);
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, FLIP_RING_TYPE);
    GUILABEL_SetTextById(MMIENVSET_FLIP_RING_SET_EDIT_LABEL_CTRL_ID,text_id[cur_selection],FALSE);
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, FLIP_DOWN_RING_TYPE);
    GUILABEL_SetTextById(MMIENVSET_FLIP_DOWN_RING_SET_EDIT_LABEL_CTRL_ID,text_id[cur_selection],FALSE);  
}
#endif
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
/*****************************************************************************/
//Description : 拨号音参数设置
//Global resource dependence : 
//Author:jixin.xu
//Note: 
/*****************************************************************************/
LOCAL void  SetDialTone(void)
{
    uint8 loop = 0;
    uint16 cur_selection =0;
    uint32 text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMIENVSET_DIAL_TONE_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_DIAL_TONE_SET_LABEL_CTRL_ID,TXT_SET_DIAL_TONE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMIENVSET_DIAL_TONE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        GUISETLIST_AddItemById(MMIENVSET_DIAL_TONE_SETLIST_CTRL_ID, text_id[loop]);
    }

    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, ENVSET_DIAL_TONE_SWITCH);//读取nv中当前环境的设置
    GUISETLIST_SetCtrlState(MMIENVSET_DIAL_TONE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIENVSET_DIAL_TONE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMIENVSET_DIAL_TONE_SETLIST_CTRL_ID,TXT_SET_DIAL_TONE);	
}
#endif

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//Description : 支持触摸屏的情况下，触屏音参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void  SetTouchpanelRingParam(void)
{
    uint8 loop = 0;
    uint16      cur_selection =0;
    uint32      text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };
    
    GUIFORM_SetStyle(MMIENVSET_TOUCH_PANEL_RING_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_TOUCH_PANEL_RING_SET_LABEL_CTRL_ID,TXT_SET_RING_TKEY_TONE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMIENVSET_TOUCH_PANEL_RING_SET_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
	    GUISETLIST_AddItemById(MMIENVSET_TOUCH_PANEL_RING_SET_SETLIST_CTRL_ID, text_id[loop]);
	}
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, TKEY_RING_TYPE);
	GUISETLIST_SetCtrlState(MMIENVSET_TOUCH_PANEL_RING_SET_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIENVSET_TOUCH_PANEL_RING_SET_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMIENVSET_TOUCH_PANEL_RING_SET_SETLIST_CTRL_ID,TXT_SET_RING_TKEY_TONE);	

}

/*****************************************************************************/
//Description : 支持触摸屏的情况下，拨号盘触屏振动参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void  SetDialPanelTouchVibrate(void)
{
    uint8 loop = 0;
    uint16      cur_selection =0;
    uint32      text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMIENVSET_TOUCH_VIBRATE_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_TOUCH_VIBRATE_SET_LABEL_CTRL_ID,TXT_SET_DIAL_PANEL_TOUCH_VIBRATE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMIENVSET_TOUCH_VIBRATE_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
	    GUISETLIST_AddItemById(MMIENVSET_TOUCH_VIBRATE_SETLIST_CTRL_ID, text_id[loop]);
	}
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, ENVSET_DIAL_TP_VIBRATE_SWITCH);
	GUISETLIST_SetCtrlState(MMIENVSET_TOUCH_VIBRATE_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIENVSET_TOUCH_VIBRATE_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMIENVSET_TOUCH_VIBRATE_SETLIST_CTRL_ID,TXT_SET_DIAL_PANEL_TOUCH_VIBRATE);	
}

/*****************************************************************************/
//Description : 支持触摸屏的情况下，屏幕锁定提示音参数设置
//Global resource dependence : 
//Author:jian.ma
//Note: 
/*****************************************************************************/
LOCAL void SetTouchKeyLockRingParam(void)
{
    uint8 loop = 0;
    uint16      cur_selection =0;
    uint32      text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMIENVSET_TOUCH_KEY_LOCK_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_TOUCH_KEY_LOCK_LABEL_CTRL_ID,TXT_SET_TP_KEY_LOCK_RING,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMIENVSET_TOUCH_KEY_LOCK_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop = 0; loop < ARR_SIZE(text_id); loop++)
	{
	    GUISETLIST_AddItemById(MMIENVSET_TOUCH_KEY_LOCK_SETLIST_CTRL_ID, text_id[loop]);
	}
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, ENVSET_TP_KEY_LOCK_SWITCH);
	GUISETLIST_SetCtrlState(MMIENVSET_TOUCH_KEY_LOCK_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    
    if (0 != cur_selection)
    {
        GUISETLIST_SetCurIndex(MMIENVSET_TOUCH_KEY_LOCK_SETLIST_CTRL_ID, 1);
    }
    else
    {
        GUISETLIST_SetCurIndex(MMIENVSET_TOUCH_KEY_LOCK_SETLIST_CTRL_ID, 0);
    }
    
    GUISETLIST_SetTitleTextId(MMIENVSET_TOUCH_KEY_LOCK_SETLIST_CTRL_ID,TXT_SET_TP_KEY_LOCK_RING);	
}
#endif

#ifdef MMI_READMENU_ENABLE
/*****************************************************************************/
//Description : 支持菜单朗读的情况下，菜单朗读开关设置
//Global resource dependence : 
//Author:violent.wei
//Note: 
/*****************************************************************************/
LOCAL void  SetMenuRead(void)
{
    uint8 loop = 0;
    MMI_STRING_T itemStr={0};
    GUISETLIST_ITEM_INFO_T item_info  ={0};
    uint16      cur_selection =0;
    uint32      text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };
    
    GUIFORM_SetStyle(MMIENVSET_MENU_READ_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_MENU_READ_SET_LABEL_CTRL_ID,TXT_SET_MENU_READ,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMIENVSET_MENU_READ_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
    {
        MMI_GetLabelTextByLang(text_id[loop], &itemStr);
        item_info.item_state = GUISETLIST_ITEM_STATE_TEXT;
        item_info.str_info.wstr_len = itemStr.wstr_len;
        item_info.str_info.wstr_ptr = itemStr.wstr_ptr;
        GUISETLIST_AddItem(MMIENVSET_MENU_READ_SETLIST_CTRL_ID, &item_info);
    }
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, ENVSET_MENU_READ_SWITCH);
    GUISETLIST_SetCtrlState(MMIENVSET_MENU_READ_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIENVSET_MENU_READ_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMIENVSET_MENU_READ_SETLIST_CTRL_ID,TXT_SET_MENU_READ);	
}
#endif

/*****************************************************************************/
//Description : 开关机铃声参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SetPowerOnOffRingParam(void)
{
    uint8 loop = 0;
    uint16      cur_selection =0;
    uint32      text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMIENVSET_POWER_ONOFF_RING_SET_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_POWER_ONOFF_RING_SET_LABEL_CTRL_ID,TXT_ENVSET_POWER_RING_TYPE,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMIENVSET_POWER_ONOFF_RING_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif
    for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
	    GUISETLIST_AddItemById(MMIENVSET_POWER_ONOFF_RING_SETLIST_CTRL_ID, text_id[loop]);
	}
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(0, POWER_RING_SWITCH);
	GUISETLIST_SetCtrlState(MMIENVSET_POWER_ONOFF_RING_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIENVSET_POWER_ONOFF_RING_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMIENVSET_POWER_ONOFF_RING_SETLIST_CTRL_ID,TXT_ENVSET_POWER_RING_TYPE);	

}

/*****************************************************************************/
//Description : 低电压告警铃声参数设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
 LOCAL void  SetLowBatteryRingParam(void)
{
    uint8 loop = 0;
    uint16      cur_selection =0;
    uint32      text_id[] =
    {
        TXT_OPEN,
        TXT_CLOSE
    };

    GUIFORM_SetStyle(MMIENVSET_LOW_BATTERY_RING_FORM_CTRL_ID,GUIFORM_STYLE_UNIT);
    GUILABEL_SetTextById(MMIENVSET_LOW_BATTERY_RING_LABEL_CTRL_ID,TXT_SET_RING_VOLTAGE_WARNING,FALSE);
#ifndef MMI_GUI_STYLE_TYPICAL
    GUISETLIST_SetFontAlign(MMIENVSET_LOW_BATTERY_RING_SETLIST_CTRL_ID,ALIGN_LVMIDDLE);
#endif

    for(loop=0;loop<ARR_SIZE(text_id);loop++)
	{
	    GUISETLIST_AddItemById(MMIENVSET_LOW_BATTERY_RING_SETLIST_CTRL_ID, text_id[loop]);
	}
    cur_selection = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1, VOLTAGE_WARN);
	GUISETLIST_SetCtrlState(MMIENVSET_LOW_BATTERY_RING_SETLIST_CTRL_ID, GUISETLIST_STATE_LEFTSK_SWITCH, TRUE);
    GUISETLIST_SetCurIndex(MMIENVSET_LOW_BATTERY_RING_SETLIST_CTRL_ID, cur_selection);
    GUISETLIST_SetTitleTextId(MMIENVSET_LOW_BATTERY_RING_SETLIST_CTRL_ID,TXT_SET_RING_VOLTAGE_WARNING);	

}
/*****************************************************************************/
//Description : 保存所有设置
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void SaveOtherRingTypeSet(void)
{
#ifdef TOUCH_PANEL_SUPPORT
    uint16         touch_panel_ring_index    =  0;
    uint16         dail_panel_touch_index    =  0;
    uint16         tp_key_lock_ring_index    =  0;
#endif
    uint16         low_battery_warning_index =  0;
    uint16         power_on_off_index        =  0;
#ifdef MMI_READMENU_ENABLE
    uint16         menu_read_index        =  0;
#endif
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    uint16         dail_tone_index    =  0;
#endif
#ifdef TOUCH_PANEL_SUPPORT
    touch_panel_ring_index = (uint16)GUISETLIST_GetCurIndex(MMIENVSET_TOUCH_PANEL_RING_SET_SETLIST_CTRL_ID);
    MMIENVSET_SetCurModeOptValue(TKEY_RING_TYPE,(uint8)touch_panel_ring_index, MMIAPISET_GetActiveSim());
    
    dail_panel_touch_index = (uint16)GUISETLIST_GetCurIndex(MMIENVSET_TOUCH_VIBRATE_SETLIST_CTRL_ID);
    MMIENVSET_SetCurModeOptValue(ENVSET_DIAL_TP_VIBRATE_SWITCH,(uint8)dail_panel_touch_index, MMIAPISET_GetActiveSim());
    tp_key_lock_ring_index = (uint16)GUISETLIST_GetCurIndex(MMIENVSET_TOUCH_KEY_LOCK_SETLIST_CTRL_ID);
    MMIENVSET_SetCurModeOptValue(ENVSET_TP_KEY_LOCK_SWITCH, tp_key_lock_ring_index, MMIAPISET_GetActiveSim());
#endif
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    dail_tone_index = (uint16)GUISETLIST_GetCurIndex(MMIENVSET_DIAL_TONE_SETLIST_CTRL_ID);
    MMIENVSET_SetCurModeOptValue(ENVSET_DIAL_TONE_SWITCH,(uint8)dail_tone_index, MMIAPISET_GetActiveSim());
#endif   

    low_battery_warning_index = (uint16)GUISETLIST_GetCurIndex(MMIENVSET_LOW_BATTERY_RING_SETLIST_CTRL_ID);
    MMIENVSET_SetCurModeOptValue(VOLTAGE_WARN,(uint8)low_battery_warning_index, MMIAPISET_GetActiveSim());

    power_on_off_index    =   (uint16)GUISETLIST_GetCurIndex(MMIENVSET_POWER_ONOFF_RING_SETLIST_CTRL_ID);
    MMIENVSET_SetCurModeOptValue(POWER_RING_SWITCH,(uint8)power_on_off_index, MMIAPISET_GetActiveSim());

#ifdef MMI_READMENU_ENABLE
    menu_read_index    =   (uint16)GUISETLIST_GetCurIndex(MMIENVSET_MENU_READ_SETLIST_CTRL_ID);
    MMIENVSET_SetCurModeOptValue(ENVSET_MENU_READ_SWITCH,(uint8)menu_read_index, MMIAPISET_GetActiveSim());
#endif
}
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//Description : 修改好铃声音量后，设置label的内容
//Global resource dependence : 
//Author:juan.wu
//Note: 
/*****************************************************************************/
LOCAL void   SetVolumeLabelcontent(MMIENVSET_SET_OPT_E set_opt, uint8  s_cur_vol)
{
    wchar ch[2] = {0};
    MMI_STRING_T text ={0};
    
    SCI_MEMSET(ch, 0, sizeof(ch)); 
    ch[0] = s_cur_vol + '0';
    text.wstr_ptr = ch;
    text.wstr_len = 1;
    switch(set_opt)
    {
    case CALL_RING_VOL:
        GUILABEL_SetText(MMIENVSET_CALL_RING_VOL_SET_EDIT_LABEL_CTRL_ID, &text, FALSE);   
        break;

    case MSG_RING_VOL:
        GUILABEL_SetText(MMIENVSET_MSG_RING_VOL_SET_EDIT_LABEL_CTRL_ID, &text, FALSE);
        break;

    case COMM_VOL:
        GUILABEL_SetText(MMIENVSET_COM_RING_VOL_SET_EDIT_LABEL_CTRL_ID, &text, FALSE);
        break;

    case ALARM_RING_VOL:
        GUILABEL_SetText(MMIENVSET_ALARM_RING_VOL_SET_EDIT_LABEL_CTRL_ID, &text, FALSE);
        break;

    case POWER_RING_VOL:
        GUILABEL_SetText(MMIENVSET_POWER_ONOFF_RING_VOL_SET_EDIT_LABEL_CTRL_ID, &text, FALSE);
        break;

#ifdef MMI_READMENU_ENABLE
    case MENUREAD_VOL:
        GUILABEL_SetText(MMIENVSET_MENU_READ_VOL_SET_EDIT_LABEL_CTRL_ID, &text, FALSE);
        break;
#endif

    default:
        //SCI_TRACE_LOW:"SetVolumeLabelcontent set_opt = %d,s_cur_vol = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_WINTAB_4117_112_2_18_2_19_13_75,(uint8*)"dd",set_opt,s_cur_vol);
        break;
    }
}
#endif


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:54 2012*/ //IGNORE9527


/*Edit by script, ignore 17 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
