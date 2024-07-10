/*****************************************************************************
** File Name:      mmiset_func.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    kelly.li          Create
******************************************************************************/

#ifndef _MMISET_FUNC_H_
#define _MMISET_FUNC_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiset_export.h"



/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMISET_DEFAULT_KEYLOCK_TIME MMISET_KEYLOCK_CLOSE //锁屏的默认设置

#define MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_90  0x20 //cr227614 maryxiao
#define MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_180  0x10
#define MMISET_SENSOR_OPEN_FLAG_LANDSCAPE_270  0x8
#define MMISET_SENSOR_OPEN_FLAG_CALL_SILENCE  0x4
#define MMISET_SENSOR_OPEN_FLAG_REPLACE_WALLPAPER  0x2
#define MMISET_SENSOR_OPEN_FLAG_NEXT_SONG  0x1
//#define MMISET_SENSOR_OPEN_FLAG_VIDEO_FULLSCREEN  0x10
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef enum
{
#ifdef MMI_RES_ORIENT_BOTH
	MMISET_SENSOR_TYPE_LANDSCAPE_90,//自动横竖屏显示 cr227614 maryxiao
	MMISET_SENSOR_TYPE_LANDSCAPE_180,
	MMISET_SENSOR_TYPE_LANDSCAPE_270,
#endif
	MMISET_SENSOR_TYPE_CALL_SILENCE,//电话静音
	MMISET_SENSOR_TYPE_REPLACE_WALLPAPER,//更换墙纸
	MMISET_SENSOR_TYPE_NEXT_SONG,//播发下一曲
	//MMISET_SENSOR_TYPE_VIDEO_FULLSCREEN,
	
}MMISET_SENSOR_TYPE_E;

#define MMISET_SENSOR_TYPE_MAX  4//CR 227614 MARYXIAO
#define MMISET_SENSOR_AUTOROTATION_MAX 3


typedef struct
{
    uint16   lang_valid[MMISET_MAX_LANGUAGE + 1];
    uint16   lang_valid_num;
}MMISET_LANG_VALID_INFO_T;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/




/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : Set the mode of anykey and reject
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_SetAnykeyRejectMode(
                                       MMI_CTRL_ID_T ctrl_id
                                       );

/*****************************************************************************/
//  Description : reset factory setting 
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : 添加语言选择的radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendLanguageListItem(
                                          MMI_CTRL_ID_T     ctrl_id
                                          );

/*****************************************************************************/
//  Description : 设置语言选择的结果
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectLanguage(
                                     MMI_CTRL_ID_T      ctrl_id
                                     );

/*****************************************************************************/
//  Description : 添加键盘灯时间到radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendKeyboardLightListItem(
                                               MMI_CTRL_ID_T        ctrl_id
                                               );

/*****************************************************************************/
//  Description : 设置键盘灯选择的结果
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetKeyboardLight(
                                    MMI_CTRL_ID_T       ctrl_id
                                    );

/*****************************************************************************/
//  Description : 添加move style到radio list的item
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendMoveStyleListItem(
                                           MMI_CTRL_ID_T        ctrl_id
                                           );

/*****************************************************************************/
//  Description : 设置move style选择的结果
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMoveStyle(
                                MMI_CTRL_ID_T       ctrl_id
                                );

/*****************************************************************************/
//  Description : 添加开启关闭的单选list items 
//  Global resource dependence :  
//  Author: jibin
//  Note: 通过funcGet从nv得到当前开启关闭状态 
/*****************************************************************************/ 
PUBLIC void MMISET_AppendOpenCloseListItem( 
                                           MMI_CTRL_ID_T ctrl_id,
                                           BOOLEAN (*funcGet)(void)
                                           );

/*****************************************************************************/
//  Description : set is auto boardkey
//  Global resource dependence : none
//  Author: jibin
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetAutoBoardKey(MMISET_KEYLOCK_TYPE_E auto_boardkey_time);

/*****************************************************************************/
//  Description : 添加网络选择方式到radio list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSelectNetworkListItem(
                                               MN_DUAL_SYS_E dual_sys,
                                               MMI_CTRL_ID_T        ctrl_id
                                               );
#ifdef MMI_VOLTE_SUPPORT
PUBLIC void MMISET_AppendVoLTEOptionItem(
                                         MN_DUAL_SYS_E dual_sys,
                                         MMI_CTRL_ID_T        ctrl_id
                                         );
#endif
/*****************************************************************************/
//  Description : 设置网络选择方式
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetSelectNetworkMode(
                                        MN_DUAL_SYS_E dual_sys,
                                        MMI_CTRL_ID_T       ctrl_id
                                        );

/*****************************************************************************/
//  Description : 选择指定的网络
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SelectNetwork(
                                 MN_DUAL_SYS_E dual_sys,
                                 MMI_CTRL_ID_T      ctrl_id
                                 );

/*****************************************************************************/
//  Description : 添加频段选择方式到list的item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendSelectBansListItem(
                                            MMI_CTRL_ID_T   ctrl_id
                                            );

/*****************************************************************************/
//  Description : 设置频段选择方式
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSelectBandType(
                                        MMI_CTRL_ID_T       ctrl_id
                                        );

/*****************************************************************************/
//  Description : 设置开机密码开启或者关闭
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetPowerOnPwdStatus(
                                          BOOLEAN  is_open
                                          );
/*****************************************************************************/
//  Description : 获得开机密码长度，用来判断开机密码是否已经设置
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_IsSetPowerOnPwd(void);

/*****************************************************************************/
//  Description : 网络设置中，自动搜索和手动搜索从LIST中选择网络的过程中，不允许取消！
//只有在手动搜索 --------------〉网络LIST,这中间才可以取消（包括red key, flip）
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleQuitSelNetworkConnectWindow(
                                                             MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                             MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                             DPARAM             param       // 相应消息的参数
                                                             );
/*****************************************************************************/
//  Description : 网络设置中，自动搜索和手动搜索从LIST中选择网络的过程中，不允许取消！
//只有在手动搜索 --------------〉网络LIST,这中间才可以取消（包括red key, flip）
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_HandleSelNetworkConnectWindow(
                                                         MMI_WIN_ID_T    win_id,    // 窗口的ID
                                                         MMI_MESSAGE_ID_E   msg_id,     // 窗口的内部消息ID
                                                         DPARAM             param       // 相应消息的参数
                                                         );

/*****************************************************************************/
// Description : Get Is Allow Open Query Fly mode
// Global resource dependence :
// return :  
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetIsQueryOpenFlyMode(void);

/*****************************************************************************/
// Description : Stop PS Application
// Global resource dependence :
// return :  
// Author: wancan.you
// Note:
/*****************************************************************************/
PUBLIC void MMISET_StopPSApplication(void);
#if 0
/*****************************************************************************/
//  Description : 添加语言选择的dropdownlist的item
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendShowLanguageItem(
                                          MMI_CTRL_ID_T     ctrl_id
                                          );

/*****************************************************************************/
//  Description : 添加语言输入的dropdownlist的item
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_AppendInputLanguageItem(
											   MMI_CTRL_ID_T ctrl_id, 
											   MMISET_LANGUAGE_TYPE_E language_type
											   );
#endif
/*****************************************************************************/
//  Description : 设置输入语言的结果
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetInputLanguage(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description :是否相关sensor功能开启
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
BOOLEAN MMISET_IsSensorOpen(MMISET_SENSOR_TYPE_E type);

/*****************************************************************************/
//  Description :设置sensor是否开启flag
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISET_SetIsSensorOpenFlag(uint32 open_flag);

/*****************************************************************************/
//  Description :获取sensor是否开启flag
//  Global resource dependence : 
//  Author: renyi.hu
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMISET_GetIsSensorOpenFlag(void);

/*****************************************************************************/
//  Description :设置语言可active label的值
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetLanguageCtrlContent(void);

/*****************************************************************************/
//  Description :设置输入法语言可active label的text
//  Global resource dependence : 
//  Author: juan.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_SetInputLanguageCtrlContent(void);

#if defined(MMI_BLACKLIST_SUPPORT)
/*****************************************************************************/
//  Description : load blacklist
//  Global resource dependence : 
//  Author:fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_GetBlackList(void);

/*****************************************************************************/
//  Description : set blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_SetBlackList(MMISET_BLACKLIST_T *blacklist);

/*****************************************************************************/
//  Description : init blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC BOOLEAN MMISET_InitBlacklistParam(void);

/*****************************************************************************/
//  Description : free blacklist
//  Global resource dependence : none
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/ 
PUBLIC void MMISET_FreeBlacklistParam(void);
#endif
/*****************************************************************************/
//  Description : init date display type
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMISET_InitDateDisplayType(void);

/*****************************************************************************/
//  Description : get valid language number
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_GetValidLanguageNum(void);

/*****************************************************************************/

//  Description : 
//  Global resource dependence : 
//  Author:maryxiao
//  Modify:wei.zhou, for wre use
//  Note: 
/*****************************************************************************/
PUBLIC void MMISET_ReturnToAngleSupport(uint32 sensor_open_flag);
/*****************************************************************************/
//  Description :获取当前系统语言
//  Global resource dependence : 
//  Author: yuantao.xu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMISET_GetCurrentLanguageType(void);
/*****************************************************************************/
//  Description : Select SIM API of SET app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_SelectSIMFunc(MMIAPIPHONE_SELECTSIM_FUNC MMISetSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E    append_type,
                                        ADD_DATA        	        add_data
                                        );
/*****************************************************************************/
//  Description : Select NETWORK API of SET app
//  Global resource dependence : 
//  Author:sam.hua
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISET_SelectNetworkFunc(MMIAPIPHONE_SELECTSIM_FUNC MMISetSelectSIMWinCallback,
                                        MMISET_APPEND_SIM_TYPE_E append_type,
                                        ADD_DATA        add_data
                                        );

#endif
