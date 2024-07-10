/*****************************************************************************
** File Name:      Mmienvset.c                                               *
** Author:                                                                   *
** Date:           12/12/2004                                                *
** Copyright:      2004 Gionee, Incorporated. All Rights Reserved.            *
** Description:    This file is used to describe environment setting         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2004        maqianlin        Create
******************************************************************************/

#define     _MMIENVSET_C_                    


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_envset_trc.h"
#include    "guilistbox.h"
#include    "mmi_menutable.h"
#include    "audio_config.h"
#include    "os_api.h"
#include    "mmk_timer.h"
#include    "mmiidle_export.h"
#include    "mmi_common.h"
#include    "mmialarm_export.h"
#include    "mmi_text.h"
#include    "mmk_type.h"
#include    "mmiphone_export.h"
#include    "mmi_nv.h"
#include    "mn_type.h"
#include    "mmienvset_internal.h"
#include    "mmi_textfun.h"//@cr36254 jian.ma
//#include    "mmiset.h"
#include "mmiset_export.h"
#include    "mmiset_text.h"
#include    "mmiset_id.h"
#include    "mmiset_nv.h"
#include "mmienvset_export.h"
//#include    "mmiset_wintab.h"
#include    "mmipub.h"
#include    "mmiudisk_export.h"
//#include    "mmieng.h"
#include    "mmieng_export.h"
#include	"guiedit.h"
#include    "mmibt_export.h"
#include    "gpio_prod_api.h"
#include    "mmi_appmsg.h"
#include    "mmiidle_statusbar.h"
#ifdef PDA_UI_DROPDOWN_WIN
#include    "mmidropdownwin_export.h"
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define  MMIENVSET_DEFUALT_MODE                 MMIENVSET_STANDARD_MODE  //默认的环境是普通环境
#define  MMIENVSET_MORE_RING_INI_FILE_NAME      L"envset.ini"
#define  MMIENVSET_MORE_RING_DIR_LEN            11
#define  MMIENVSET_MORE_RING_DIR         (g_mminvset_more_ring_dir_wstr)
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL uint8       s_onetime_vibra_timer_id  = 0;   //震动器每次震动的timer 
LOCAL BOOLEAN     s_is_vibra_start          = FALSE; //震动器是否启动
LOCAL BOOLEAN     s_is_play_finished        =   FALSE; //播放/预览的铃声是否完毕
const wchar       g_mminvset_more_ring_dir_wstr[] = {'m', 'o', 'r', 'e', 'r', 'i', 'n', 'g', 's', 'e', 't', 0};	//来电更多铃声存储的路径
LOCAL BOOLEAN     s_enhance_ring_state = FALSE;


//前一次环境模式ID  最后一次的静音或者非静音的模式
LOCAL uint8        s_noisy_pre_mode = MMIENVSET_STANDARD_MODE;/*lint -esym(551, s_noisy_pre_mode) */
LOCAL uint8        s_silent_pre_mode = MMIENVSET_MEETING_MODE;/*lint -esym(551, s_silent_pre_mode) */

LOCAL uint8* s_act_mode_value_ptr = PNULL;


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMIENVSET_MORE_RING_CALL,
    MMIENVSET_MORE_RING_MSG,
    MMIENVSET_MORE_RING_MAX_INDEX
}MMIENVSET_MORE_RING_INDEX_E;
#if 0// defined MMI_PDA_SUPPORT
const LOCAL MMIENVSET_SETTING_T mmienvset_user_setting =    
{
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_STANDARD_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_STANDARD_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_STANDARD_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_STANDARD_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_STANDARD_MODE,            // 闹钟铃声音量
        MMIENVSET_TKEY_RING_VOL_STANDARD_MODE,          //触屏音音量
        MMIENVSET_POWER_RING_VOL_STANDARD_MODE,         //开关机音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_STANDARD_MODE,          //菜单朗读音量
#endif
        
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },
        
        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_STANDARD_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型

#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE,     // 低电压报警开关
        MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE,      //开关机声音的方式
        //name_default_str_t,
        {
            {0},                        
                //FALSE,
                0
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE,
#endif
#if defined MMI_PDA_SUPPORT
        FALSE,
#endif
};
#endif
// 默认设置
const MMIENVSET_SETTING_T mmienvset_default_setting[MMIENVSET_TOTAL_MODE]=
{
    // 普通环境
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_STANDARD_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_STANDARD_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_STANDARD_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_STANDARD_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_STANDARD_MODE,            // 闹钟铃声音量
        MMIENVSET_TKEY_RING_VOL_STANDARD_MODE,          //触屏音音量
        MMIENVSET_POWER_RING_VOL_STANDARD_MODE,         //开关机音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_STANDARD_MODE,          //菜单朗读音量
#endif
        
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif      
        },
        
        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_STANDARD_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型

#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_STANDARD_MODE,   // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE,      //开关机声音的方式
        //name_default_str_t,
        {
            {0},                        
                //FALSE,
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE,
#endif
        TRUE,
    },
        
        // 安静环境
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_SILENT_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_SILENT_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_SILENT_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_SILENT_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_SILENT_MODE,            // 闹钟铃声音量
        MMIENVSET_TKEY_RING_VOL_SILENT_MODE,             //触屏音音量   
        MMIENVSET_POWER_RING_VOL_SILENT_MODE,           //开关机音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_SILENT_MODE,          //菜单朗读音量
#endif


        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_SILENT_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_SILENT_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_SILENT_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_SILENT_MODE
#endif
#endif
#endif
        },

        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_SILENT_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_SILENT_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_SILENT_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_SILENT_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_SILENT_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_SILENT_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_SILENT_MODE ,       // 翻合盖类型
         MMIENVSET_FLIP_DOWN_RING_TYPE_SILENT_MODE ,       // 翻合盖类型
#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_SILENT_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_SILENT_MODE ,       // 滑盖关闭类型
#endif
       // MMIENVSET_ALARM_RING_TYPE_SILENT_MODE,       // 闹钟提示方式
        MMIENVSET_ALARM_RING_TYPE_MEETING_MODE,//CR178369注意：闹钟提示方式与别的提示方式均不同。及时静音模式下也需要有提示（震动）
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_SILENT_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_SILENT_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_SILENT_MODE,     // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_SILENT_MODE,        //开关机声音的方式
        //name_default_str_t,
        {
            {0},
                //FALSE,
                0           
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
         {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_SILENT_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_SILENT_MODE,
#endif
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_SILENT_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_SILENT_MODE,
#endif
        TRUE,
    },
        
        // 会议环境
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_MEETING_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_MEETING_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_MEETING_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_MEETING_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_MEETING_MODE,            // 按键音量
        MMIENVSET_TKEY_RING_VOL_MEETING_MODE,          //触屏音量
        MMIENVSET_POWER_RING_VOL_MEETING_MODE,           //开关机声音音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_MEETING_MODE,          //菜单朗读音量
#endif

        
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_MEETING_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_MEETING_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_MEETING_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_MEETING_MODE
#endif
#endif
#endif
        },

        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_MEETING_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_MEETING_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_MEETING_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_MEETING_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_MEETING_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_MEETING_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_MEETING_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_MEETING_MODE ,       // 翻合盖类型
#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_MEETING_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_MEETING_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_MEETING_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_MEETING_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_MEETING_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_MEETING_MODE,    // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_MEETING_MODE,       //开关机声音的方式
        //name_default_str_t,
        {
            {0},
                //FALSE,        
                0           
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
         {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_MEETING_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_MEETING_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_MEETING_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_MEETING_MODE,
#endif
        TRUE,
    },
        
        // 车内环境
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_INSIDE_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_INSIDE_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_INSIDE_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_INSIDE_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_INSIDE_MODE,            // 按键音量
        MMIENVSET_TKEY_RING_VOL_INSIDE_MODE,       //触屏音音量
        MMIENVSET_POWER_RING_VOL_INSIDE_MODE,     //开关机声音音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_INSIDE_MODE,          //菜单朗读音量
#endif

        
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_INSIDE_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_INSIDE_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_INSIDE_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_INSIDE_MODE
#endif
#endif
#endif
        },

        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_INSIDE_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_INSIDE_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_INSIDE_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_INSIDE_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_INSIDE_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_INSIDE_MODE ,        // 按键类型
        MMIENVSET_FLIP_RING_TYPE_INSIDE_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_INSIDE_MODE ,       // 翻合盖类型
#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_INSIDE_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_INSIDE_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_INSIDE_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_INSIDE_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_INSIDE_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_INSIDE_MODE,     // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_INSIDE_MODE,        //开关机声音的方式
        
        //name_default_str_t,
        {
            {0},
                //FALSE,        
                0           
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
         {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_INSIDE_MODE, 
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_INSIDE_MODE, 
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_INSIDE_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_INSIDE_MODE,
#endif
        TRUE,
    },
        
        // 户外环境
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_NOISY_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_NOISY_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_NOISY_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_NOISY_MODE,            // 按键 音量
        MMIENVSET_ALARM_RING_VOL_NOISY_MODE,            // 按键 音量
        MMIENVSET_TKEY_RING_VOL_NOISY_MODE,        //触屏音音量
        MMIENVSET_POWER_RING_VOL_NOISY_MODE,      //开关机声音音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_NOISY_MODE,          //菜单朗读音量
#endif

        
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_NOISY_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_NOISY_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_NOISY_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_NOISY_MODE
#endif
#endif
#endif
        },

        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_NOISY_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_NOISY_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_NOISY_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_NOISY_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_NOISY_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_NOISY_MODE ,        // 按键类型
        MMIENVSET_FLIP_RING_TYPE_NOISY_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_NOISY_MODE ,       // 翻合盖类型
#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_NOISY_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_NOISY_MODE ,       // 滑盖关闭类型
#endif  
        MMIENVSET_ALARM_RING_TYPE_NOISY_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_NOISY_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_NOISY_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_NOISY_MODE,      // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_NOISY_MODE,         //开关机声音的方式
        
        //name_default_str_t,
        {
            {0},
               // FALSE,            
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_NOISY_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_NOISY_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_NOISY_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_NOISY_MODE,
#endif
        TRUE,
    },
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
   //耳机环境
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_EARPHONE_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_EARPHONE_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_EARPHONE_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_EARPHONE_MODE,            // 按键 音量
        MMIENVSET_ALARM_RING_VOL_EARPHONE_MODE,            // 按键 音量
        MMIENVSET_TKEY_RING_VOL_EARPHONE_MODE,        //触屏音音量
        MMIENVSET_POWER_RING_VOL_EARPHONE_MODE,      //开关机声音音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_EARPHONE_MODE,          //菜单朗读音量
#endif

        
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_EARPHONE_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_EARPHONE_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_EARPHONE_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_EARPHONE_MODE
#endif
#endif
#endif
        },
        
        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_EARPHONE_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_EARPHONE_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_EARPHONE_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_EARPHONE_MODE
#endif
#endif
#endif
        },       

        MMIENVSET_KEY_RING_TYPE_EARPHONE_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_EARPHONE_MODE ,        // 按键类型
        MMIENVSET_FLIP_RING_TYPE_EARPHONE_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_EARPHONE_MODE ,       // 翻合盖类型
#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_EARPHONE_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_EARPHONE_MODE ,       // 滑盖关闭类型
#endif  
        MMIENVSET_ALARM_RING_TYPE_EARPHONE_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_EARPHONE_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_EARPHONE_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_EARPHONE_MODE,       // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_EARPHONE_MODE,          //开关机声音的方式
        
        //name_default_str_t,
        {
            {0},
               // FALSE,            
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED, /*lint !e64*/
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,  /*lint !e64*/
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,  /*lint !e64*/
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif            
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_EARPHONE_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_EARPHONE_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_EARPHONE_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_EARPHONE_MODE,
#endif
        TRUE,
    },
//蓝牙环境
{
#ifndef MMI_MULTI_SIM_SYS_SINGLE
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_BLUETOOTH_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_BLUETOOTH_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_BLUETOOTH_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_BLUETOOTH_MODE,            // 按键 音量
        MMIENVSET_ALARM_RING_VOL_BLUETOOTH_MODE,            // 按键 音量
        MMIENVSET_TKEY_RING_VOL_BLUETOOTH_MODE,        //触屏音音量
        MMIENVSET_POWER_RING_VOL_BLUETOOTH_MODE,      //开关机声音音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_BLUETOOTH_MODE,          //菜单朗读音量
#endif


        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_BLUETOOTH_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_BLUETOOTH_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_BLUETOOTH_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_BLUETOOTH_MODE
#endif
#endif
#endif
        },
        
        {        // 来电提示方式
        MMIENVSET_MSG_RING_TYPE_BLUETOOTH_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_BLUETOOTH_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_BLUETOOTH_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_BLUETOOTH_MODE
#endif
#endif
#endif
        },        
        MMIENVSET_KEY_RING_TYPE_BLUETOOTH_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_BLUETOOTH_MODE ,        // 按键类型
        MMIENVSET_FLIP_RING_TYPE_BLUETOOTH_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_BLUETOOTH_MODE ,       // 翻合盖类型
#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_BLUETOOTH_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_BLUETOOTH_MODE ,       // 滑盖关闭类型
#endif  
        MMIENVSET_ALARM_RING_TYPE_BLUETOOTH_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_BLUETOOTH_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_BLUETOOTH_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_BLUETOOTH_MODE,      // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_BLUETOOTH_MODE,         //开关机声音的方式
        
        //name_default_str_t,
        {
            {0},
               // FALSE,            
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,  /*lint !e64*/
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,  /*lint !e64*/
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,  /*lint !e64*/
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
                
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_BLUETOOTH_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_BLUETOOTH_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_BLUETOOTH_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_BLUETOOTH_MODE,
#endif
        TRUE,
    },
#endif //#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
//CR244608 2011-5-23
    //自定义模式1
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_STANDARD_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_STANDARD_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_STANDARD_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_STANDARD_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_STANDARD_MODE,            // 闹钟铃声音量
        MMIENVSET_TKEY_RING_VOL_STANDARD_MODE,          //触屏音音量
        MMIENVSET_POWER_RING_VOL_STANDARD_MODE,         //开关机音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_STANDARD_MODE,          //菜单朗读音量
#endif
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },
        
        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_STANDARD_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型

#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_STANDARD_MODE,   // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE,      //开关机声音的方式
        //name_default_str_t,
        {
            {0},                        
                //FALSE,
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE,
#endif
        FALSE,

    },

    //自定义模式2
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_STANDARD_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_STANDARD_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_STANDARD_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_STANDARD_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_STANDARD_MODE,            // 闹钟铃声音量
        MMIENVSET_TKEY_RING_VOL_STANDARD_MODE,          //触屏音音量
        MMIENVSET_POWER_RING_VOL_STANDARD_MODE,         //开关机音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_STANDARD_MODE,          //菜单朗读音量
#endif
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },
        
        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_STANDARD_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型
         MMIENVSET_FLIP_DOWN_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型

#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_STANDARD_MODE,   // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE,      //开关机声音的方式
        //name_default_str_t,
        {
            {0},                        
                //FALSE,
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE,
#endif
        FALSE,
    },

    //自定义模式3
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_STANDARD_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_STANDARD_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_STANDARD_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_STANDARD_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_STANDARD_MODE,            // 闹钟铃声音量
        MMIENVSET_TKEY_RING_VOL_STANDARD_MODE,          //触屏音音量
        MMIENVSET_POWER_RING_VOL_STANDARD_MODE,         //开关机音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_STANDARD_MODE,          //菜单朗读音量
#endif
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },
        
        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_STANDARD_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型

#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_STANDARD_MODE,   // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE,      //开关机声音的方式
        //name_default_str_t,
        {
            {0},                        
                //FALSE,
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE,
#endif
        FALSE,
    },
#ifndef DPHONE_SUPPORT     
    //自定义模式4
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_STANDARD_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_STANDARD_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_STANDARD_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_STANDARD_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_STANDARD_MODE,            // 闹钟铃声音量
        MMIENVSET_TKEY_RING_VOL_STANDARD_MODE,          //触屏音音量
        MMIENVSET_POWER_RING_VOL_STANDARD_MODE,         //开关机音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_STANDARD_MODE,          //菜单朗读音量
#endif
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },
        
        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_STANDARD_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_STANDARD_MODE,

#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_STANDARD_MODE,   // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE,      //开关机声音的方式
        //name_default_str_t,
        {
            {0},                        
                //FALSE,
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE,
#endif
        FALSE,
    },
    //自定义模式5
    {
#if !defined(MMI_MULTI_SIM_SYS_SINGLE)
        MN_DUAL_SYS_1,
#endif
        MMIENVSET_CALL_RING_VOL_STANDARD_MODE,           // 来电铃声
        MMIENVSET_MSG_RING_VOL_STANDARD_MODE,            // 短信铃声        
        MMIENVSET_COMM_VOL_STANDARD_MODE,                // 通话音量
        MMIENVSET_KEY_RING_VOL_STANDARD_MODE,            // 按键音量
        MMIENVSET_ALARM_RING_VOL_STANDARD_MODE,            // 闹钟铃声音量
        MMIENVSET_TKEY_RING_VOL_STANDARD_MODE,          //触屏音音量
        MMIENVSET_POWER_RING_VOL_STANDARD_MODE,         //开关机音量
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_VOL_STANDARD_MODE,          //菜单朗读音量
#endif
        {        // 来电提示方式
        MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_CALL_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },
        
        {         // 短信提示方式
        MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#if defined(MMI_MULTI_SIM_SYS_QUAD)
        ,MMIENVSET_MSG_RING_TYPE_STANDARD_MODE
#endif
#endif
#endif
        },

        MMIENVSET_KEY_RING_TYPE_STANDARD_MODE ,        // 按键类型
        MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE ,        // 触屏类型
        MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE ,       // 翻合盖类型
        MMIENVSET_FLIP_DOWN_RING_TYPE_STANDARD_MODE,

#ifdef LCD_SLIDE_SUPPORT
            MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE ,       // 滑盖打开类型
            MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE ,       // 滑盖关闭类型
#endif
        MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE,       // 闹钟提示方式
        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE,     // 下载提示音开关
        MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE,     // 低电压报警开关
        MMIENVSET_CHARGE_NOTIFY_SWITCH_STANDARD_MODE,   // 开始充电提示
        MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE,      //开关机声音的方式
        //name_default_str_t,
        {
            {0},                        
                //FALSE,
                0
        },
        {
            MMISET_CALL_RING_FIXED,
            MMISET_ALARM_RING_DEFAULT_ID
        },
        {
            {
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_CALL_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        {
            {
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID,
              
            }
#if defined(MMI_MULTI_SIM_SYS_DUAL) || defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#if defined(MMI_MULTI_SIM_SYS_QUAD)
            ,{
                MMISET_MSG_RING_FIXED,
                MMISET_RING_DEFAULT_ID              
            }
#endif
#endif
#endif
        },
        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE,
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)		
        MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE,
#endif		
        MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE,
#ifdef MMI_READMENU_ENABLE
        MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE,
#endif
        FALSE,
    }
#endif
    };

// 环境设置的文字资源ID
const MMI_TEXT_ID_T g_mmienvset_text_id[] = {   TXT_ENVSET_STANDARD,
                                                TXT_ENVSET_SILENT,
                                                TXT_ENVSET_MEETING,
												TXT_ENVSET_INSIDE,
                                                TXT_ENVSET_NOISY,
												#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
												TXT_ENVSET_EARPHONE,
                                                TXT_ENVSET_BLUETOOTH,
												#endif
						                        TXT_SETTINGS_ENVIONMENT,
                                                TXT_SETTINGS_ENVIONMENT,
                                                TXT_SETTINGS_ENVIONMENT,
                                                TXT_SETTINGS_ENVIONMENT,
                                                TXT_SETTINGS_ENVIONMENT
												};
typedef struct
{
    uint16              cur_mode;
    MMIENVSET_SETTING_T last_setting;
}MMIENVSET_CURRENT_SETING_T;

LOCAL MMIENVSET_CURRENT_SETING_T s_mmienvset_setting_info = {0};
/**-------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
// 初始化时候保存从NV读取的设置值,方便以后的读写操作
//MMIENVSET_SETTING_T * g_mmienvset_mode_setting_ptr = PNULL;

// 当前环境模式ID
LOCAL uint8               s_mmienvset_cur_mode = MMIENVSET_DEFUALT_MODE;

// 启动的环境ID
LOCAL uint8               s_mmienvset_active_mode = MMIENVSET_DEFUALT_MODE;
LOCAL uint8               s_mmienvset_pre_active_mode = MMIENVSET_DEFUALT_MODE;
LOCAL BOOLEAN             s_mute_state = FALSE; //only in earphone or bt mode 
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 获得预览用的铃声  ID 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:来电提示铃声选取第0组
/*****************************************************************************/
LOCAL uint8 GetCurModeRingId(
                             MN_DUAL_SYS_E          dual_sys,
                             MMISET_ALL_RING_TYPE_E ring_type
                             );

/*****************************************************************************/
//  Description : 获得预览用的铃声 音量 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
LOCAL uint8 GetRingVol(
                       uint8                   mode_id,
                       MMISET_ALL_RING_TYPE_E  ring_type
                       );
/*****************************************************************************/
//  Description : 使用环境设置的指定值播放铃声
//  Global resource dependence : none
//  Author: maqianlin
//  Note:只给MMIENVSET_PlayRingByListItem 调用
/*****************************************************************************/
LOCAL void PreviewRingBySetting(
                                MN_DUAL_SYS_E               dual_sys,
                                MMISET_ALL_RING_TYPE_E      ring_type,  
                                uint8                       play_times,   //播放次数
                                MMISET_RING_CALLBACK_PFUNC  callback_func
                                );
/*****************************************************************************/
//  Description : 获得指定模式的设置项值
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
LOCAL uint8 GetEnvSetOptValue(
                              MN_DUAL_SYS_E             dual_sys,
                              uint8                     mode_id,
                              MMIENVSET_SET_OPT_E       set_opt
                              );

/*****************************************************************************/
//  Description : 获得指定环境模式的铃声  ID 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:来电提示铃声选取第0组
/*****************************************************************************/
LOCAL uint8 GetRingID(
                      MN_DUAL_SYS_E                 dual_sys,
                      uint8                         mode_id,
                      MMISET_ALL_RING_TYPE_E        ring_type
                      );

/*****************************************************************************/
//  Description :设置启动环境设置模式ID 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
LOCAL void   SetActiveModeId(uint8 mode_id);

/*****************************************************************************/
//  Description :按照上一次设置的情景模式设置启动环境设置模式ID 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void   SetPreActiveModeId(uint8 mode_id);


/*****************************************************************************/
//  Description : to stop alarm vibrate
//  Global resource dependence : none
//  Author: maryxiao
//  Note:
/*****************************************************************************/
LOCAL void MMIAPISET_AlarmRingCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);

/*****************************************************************************/
//  Description : 由当前阴影条指定播放不同的铃声或震动
//  Global resource dependence : none
//  Author: maqianlin
//  Note:处理短信或闹钟铃声
/*****************************************************************************/
LOCAL void MMIENVSET_PlayMsgAlarmRingByListItem(
                                                MN_DUAL_SYS_E               dual_sys,
                                                MMISET_MSG_RING_TYPE_E      index,
                                                MMISET_ALL_RING_TYPE_E      ring_type,   //铃声类型
                                                uint8                       play_times   //播放次数
                                                );

/*****************************************************************************/
//  Description : 由当前阴影条指定播放不同的铃声或震动
//  Global resource dependence : none
//  Author: maqianlin
//  Note:处理来电铃声
/*****************************************************************************/
LOCAL void PlayCallRingByListItem(
                                MN_DUAL_SYS_E               dual_sys,
                                MMISET_CALL_RING_TYPE_E     index,
                                MMISET_ALL_RING_TYPE_E      ring_type,   //铃声类型
                                uint8                       play_times   //播放次数
                                );

/*****************************************************************************/
//   Description : 列出来电铃声类型
//   Global resource dependence : 
//   Author: maqianlin
//   Note:
/*****************************************************************************/
LOCAL void AppendCallRingType(
                            MMI_CTRL_ID_T     ctrl_id
                            );

/*****************************************************************************/
//   Description : 列出短信/闹钟提示类型
//   Global resource dependence : 
//   Author: maqianlin
//   Note:
/*****************************************************************************/
LOCAL void AppendMsgRingType(
                           MMI_CTRL_ID_T       ctrl_id
                           );

/*****************************************************************************/
//   Description : 列出按键音/翻合盖提示音/开关机声音类型
//   Global resource dependence : 
//   Author: maqianlin
//   Note:
/*****************************************************************************/
LOCAL void AppendKeyRingType(
                           MMI_CTRL_ID_T     ctrl_id
                           );
/*****************************************************************************/
//  Description : alert ring play result
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleRingPlayResult(
                                MMISRVAUD_REPORT_RESULT_E result, 
                                DPARAM          param
                                );
/*****************************************************************************/
//  Description : 从nv中获取相应的情景模式数据(内部接口)
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIENVSET_SETTING_T* GetModeValue(uint16 cur_mode);

/*****************************************************************************/
//  Description : update current setting info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurrentSetingInfo(uint16 mode_id,MMIENVSET_SETTING_T* mode_setting_ptr);

/*****************************************************************************/
//  Description : 从文件中获取更多铃声信息
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadMoreRing(
                          MN_DUAL_SYS_E dual_sys,
                          uint8 mode_id,
                          MMISET_CALL_MORE_RING_T *ring_info,
                          MMIENVSET_MORE_RING_INDEX_E index,
                          wchar *filename
                          );
/*****************************************************************************/
//  Description : 写入文件更多铃声信息
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WriteMoreRing(
                           MN_DUAL_SYS_E dual_sys,
                           uint8 mode_id,
                           MMISET_CALL_MORE_RING_T ring_info,
                           MMIENVSET_MORE_RING_INDEX_E index,
                           wchar *filename
                           );
/*****************************************************************************/
//  Description : 组成更多铃声文件fullpath
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CombineMoreRingFullPath(                           
                           const wchar  *filename
                           );
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
// 	Description : set enhance ring state to nv
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL void SetEnhanceRingState(BOOLEAN enhance_ring_state);

/*****************************************************************************/
// 	Description : get enhance ring state form nv
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN GetEnhanceRingState(void);
#endif

/*****************************************************************************/
//  Description : 判断情景模式是否重名
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIENVSET_HasSameName(
                                    const MMI_STRING_T *str_ptr// IN
                                    );
/*****************************************************************************/
// 	Description : 获得情景模式下拨号盘振动的值
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
LOCAL void GetDefaultEnvsetNameStr(uint16 mode_id, MMI_STRING_T * name_str_ptr);

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 赋默认值,初始化
//  Global resource dependence : none
//  Author: maqianlin
//  Note:在mmiset_other.c的void MMIAPISET_AllInit(void)中调用
/*****************************************************************************/
void MMISET_EnvSetInit(void)
{
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    BOOLEAN         is_bt_headset = MMIAPIBT_GetActiveBTHeadset();
    BOOLEAN         is_earphone = GPIO_CheckHeadsetStatus();
#endif

    // 启动的模式
    MMINV_READ(MMIENVSET_SETTING_PRECHOOSE,&s_mmienvset_pre_active_mode,return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_mmienvset_pre_active_mode = MMIENVSET_DEFUALT_MODE;
        s_mmienvset_cur_mode = MMIENVSET_DEFUALT_MODE;
        MMINV_WRITE(MMIENVSET_SETTING_PRECHOOSE,&s_mmienvset_pre_active_mode);
    }
    MMINV_READ(MMIENVSET_SETTING_CHOOSE,&s_mmienvset_active_mode,return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_mmienvset_active_mode = MMIENVSET_DEFUALT_MODE;
        MMINV_WRITE(MMIENVSET_SETTING_CHOOSE,&s_mmienvset_active_mode);
    }

#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    if (is_earphone)
    {
        SetActiveModeId(MMIENVSET_EARPHONE_MODE);
        s_mmienvset_cur_mode = MMIENVSET_EARPHONE_MODE;
    }
    else if (MMIENVSET_EARPHONE_MODE == s_mmienvset_active_mode)//如果是耳机模式，同时耳机未插入
    {
        if(is_bt_headset)//如果蓝牙耳机连接
        {
            SetActiveModeId(MMIENVSET_BLUETOOTH_MODE);
    	    s_mmienvset_cur_mode = MMIENVSET_BLUETOOTH_MODE;
        }
        else
        {
            SetActiveModeId(s_mmienvset_pre_active_mode);
            s_mmienvset_cur_mode = s_mmienvset_pre_active_mode;
        }
    }
    else if (MMIENVSET_BLUETOOTH_MODE == s_mmienvset_active_mode && !is_bt_headset)//如果蓝牙耳机模式同时蓝牙耳机未连接
    {
        SetActiveModeId(s_mmienvset_pre_active_mode);
        s_mmienvset_cur_mode = s_mmienvset_pre_active_mode;
    }
    else
#endif
    {
        s_mmienvset_cur_mode = MMIENVSET_DEFUALT_MODE;
    }
    MMINV_READ(MMIENVSET_SETTING_MUTE_STATE,&s_mute_state,return_value);
    
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_mute_state = FALSE;
        MMINV_WRITE(MMIENVSET_SETTING_MUTE_STATE,&s_mute_state);
    }
    if(MMIENVSET_CreateMoreRingDir())
    {
        MMIENVSET_CreateMoreRingFile();
    }
    else
    {
        //no u disk, no sd card
    }
    s_mmienvset_setting_info.cur_mode = MMIENVSET_TOTAL_MODE;
    //init s_enhance_ring_state
#ifndef MMI_ENVSET_MINI_SUPPORT
    GetEnhanceRingState();
    #endif

}

/*****************************************************************************/
//  Description : 打开环境设置主菜单                        
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_OpenMainMenuWindow(void) 
{
    if (MMIAPIENG_GetIQModeStatus())
    {
        MMIPUB_OpenAlertWarningWin(TXT_IQ_DATA_PROCESSING);
        return;
    }
    MMK_CreateWin((uint32 *)MMIENVSET_MAIN_MENU_WIN_TAB, PNULL);
}

/*****************************************************************************/
//  Description : 初始化修改名称的 编辑框                        
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
void ENVSET_InitChangeNameTextEditBox(
                                      MMI_CTRL_ID_T ctrl_id
                                      )
{
    ENVSET_NAME_STR_INFO_T              name_str_t          =  {0};    
    MMI_STRING_T                        temp_str            =  {0};   
    uint8                               cur_select          =  0;    
    MMIENVSET_SETTING_T                 *mode_setting_ptr   =  PNULL;
    wchar new_name[MMIENVSET_ENVNAME_MAX_LEN+1] = {0};
    
    cur_select = MMIENVSET_GetCurModeId();
    mode_setting_ptr = GetModeValue(cur_select);
    name_str_t = mode_setting_ptr->name_str_t;
    
    // 出厂设置
    if( 0 == name_str_t.wstr_len ) 
    {
        // 由文本资源获得字符串并 把字符串在编辑框输出
        if (cur_select < MMIENVSET_USER_MODE1)
        {
            MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(g_mmienvset_text_id[cur_select]),&temp_str);
        }
        else
        {
            MMI_STRING_T  string = {0};
            uint16 string_len = 0;
            char num_char[5] = {0};
            uint16 num_len = 0;
            
            
            if (cur_select<ARR_SIZE(g_mmienvset_text_id))
            {
                MMI_GetLabelTextByLang(g_mmienvset_text_id[cur_select], &string);
            }
            else
            {
                MMI_GetLabelTextByLang(g_mmienvset_text_id[0], &string);
            }
            string_len = string.wstr_len;

            sprintf(num_char, "%d",  (cur_select - MMIENVSET_USER_MODE1 + 1));
            num_len = (uint16)strlen(num_char); 

            SCI_MEMSET(new_name, 0, ((MMIENVSET_ENVNAME_MAX_LEN+1)*sizeof(wchar)));
            MMI_WSTRNCPY( new_name, MMIENVSET_ENVNAME_MAX_LEN, string.wstr_ptr, string.wstr_len, string.wstr_len);
            MMI_STRNTOWSTR( &new_name[string_len], MMIENVSET_ENVNAME_MAX_LEN - string_len, (uint8*)num_char, num_len, num_len );

            temp_str.wstr_ptr = new_name;/*lint !e733 */
            temp_str.wstr_len = string_len + num_len;
        }
        GUIEDIT_SetString(ctrl_id,temp_str.wstr_ptr,temp_str.wstr_len ); 
    }
    // 修改过名称的字符串长度为0
    else
    {
        // 把字符串在编辑框输出  
        GUIEDIT_SetString(ctrl_id,name_str_t.wstr,name_str_t.wstr_len );
    } 
}

/*****************************************************************************/
//  Description : 把新的环境名称写入nv
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
BOOLEAN MMIENVSET_EnvName_WriteInNV(
                                    MMI_CTRL_ID_T ctrl_id
                                    )
{
    BOOLEAN                         result              =   TRUE;
    SET_NV_ITEM_E                   nv_item_id          =   MMIENVSET_SETTING_BEGIN;  // nv枚举值
    MMI_STRING_T                    edit_str_t          =   {0};  // 编辑框控件的字符处理结构体  
    uint16                          cur_select          =   0;
    MMI_STRING_T                    temp_str            =   {0};
    MMI_STRING_T                    temp_str1           =   {0};   
    MMIENVSET_SETTING_T             *mode_setting_ptr   =   PNULL;
    wchar                           *name_ptr           =   PNULL;
    
    // 获得当前环境模式ID
    cur_select = MMIENVSET_GetCurModeId();

    mode_setting_ptr = SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
    SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));
    MMIAPIENVSET_GetModeValue(cur_select,mode_setting_ptr);

    // 由文本资源获得字符串                          
    GetDefaultEnvsetNameStr(cur_select, &temp_str);

    name_ptr = SCI_ALLOCA(sizeof(wchar)*MMIENVSET_ENVNAME_MAX_LEN);
    
    if(PNULL!=name_ptr && (cur_select >= MMIENVSET_USER_MODE1) && (cur_select < MMIENVSET_TOTAL_MODE))
    {
        char num_char [5] = {0};
        uint16 num_len = 0;
        sprintf(num_char, "%d",  (cur_select - MMIENVSET_USER_MODE1 + 1));
        num_len = (uint16)strlen(num_char); 

        SCI_MEMSET(name_ptr, 0, ((MMIENVSET_ENVNAME_MAX_LEN)*sizeof(wchar)));
        MMI_WSTRNCPY( name_ptr, MMIENVSET_ENVNAME_MAX_LEN, temp_str.wstr_ptr, temp_str.wstr_len, temp_str.wstr_len);
        MMI_STRNTOWSTR( &name_ptr[temp_str.wstr_len], MMIENVSET_ENVNAME_MAX_LEN - temp_str.wstr_len, (uint8*)num_char, num_len, num_len );
        temp_str.wstr_ptr = name_ptr;
        temp_str.wstr_len+=num_len;
    }

    //返回编辑的字符串
    GUIEDIT_GetString(ctrl_id,&edit_str_t);
    
    // 判断编辑返回的字符串长度是否超出范围
    //SCI_ASSERT(MMIENVSET_ENVNAME_MAX_LEN >= edit_str_t.wstr_len);
    if (edit_str_t.wstr_len >MMIENVSET_ENVNAME_MAX_LEN)
    {
        //SCI_TRACE_LOW:"MMIENVSET_EnvName_WriteInNV change edit len form %d to %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2078_112_2_18_2_18_58_0,(uint8*)"dd",edit_str_t.wstr_len,MMIENVSET_ENVNAME_MAX_LEN);
        edit_str_t.wstr_len = MMIENVSET_ENVNAME_MAX_LEN;
    }
    //判断编辑字符串的长度是否为零, 长度为零不做任何处理直接返回
    if(0 != edit_str_t.wstr_len)
    {
        //temp_str1.is_ucs2 = edit_str_t.is_ucs2;
        temp_str1.wstr_len = edit_str_t.wstr_len;
        temp_str1.wstr_ptr = edit_str_t.wstr_ptr;
        if(MMIENVSET_HasSameName(&temp_str1))
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_RENAME);
            result = FALSE;
        }
        else
        {
            // 假如只按确定键直接返回,没有做任何修该或再次修改为与默认环境设置名称相同的字符串
            if ((cur_select < MMIENVSET_TOTAL_MODE) && (MMIAPICOM_StrCompare(&temp_str,&temp_str1)))
            {
                //size = sizeof(ENVSET_NAME_STR_INFO_T);
                SCI_MEMSET(&mode_setting_ptr->name_str_t,0,sizeof(ENVSET_NAME_STR_INFO_T));
            
            }
            //否则其它情况都保存到结构体ENVSET_NAME_STR_INFO_T
            else
            {
                //保存到结构体ENVSET_NAME_STR_INFO_T
                MMI_WSTRNCPY( mode_setting_ptr->name_str_t.wstr,
                                MMIENVSET_ENVNAME_MAX_LEN,
                                edit_str_t.wstr_ptr,
                                edit_str_t.wstr_len,
                                edit_str_t.wstr_len
                                );
            
                //g_mmienvset_mode_setting[cur_select].name_str_t.is_ucs2 = edit_str_t.is_ucs2;
                mode_setting_ptr->name_str_t.wstr_len = edit_str_t.wstr_len;            
            
            }
        
            nv_item_id = nv_item_id + cur_select;/*lint !e64*/
            //SCI_TRACE_LOW:"MMIENVSET_EnvName_WriteInNV nv_item_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2118_112_2_18_2_18_58_1,(uint8*)"d",nv_item_id);
            //SCI_ASSERT(MMIENVSET_SETTING_END >= nv_item_id);/*assert verified*/        
            MMINV_WRITE(nv_item_id,mode_setting_ptr);
            UpdateCurrentSetingInfo(cur_select,mode_setting_ptr);
            MMIENVSET_MainMenu_Append_LBItem(MMIENVSET_MAIN_MENU_CTRL_ID);
        }

    }
    else
    {
        //提示名字为空
        result = FALSE;
#ifdef MMI_PDA_SUPPORT
        MMIPUB_OpenAlertWarningWin(TXT_ENVSET_EMPTY_NAME);
#endif
    }
    if (PNULL != mode_setting_ptr)/*lint !e774 */
    {
        SCI_FREE(mode_setting_ptr);
        mode_setting_ptr = PNULL;
    }
    if (PNULL != name_ptr)/*lint !e774 */
    {
        SCI_FREE(name_ptr);
        name_ptr = PNULL;
    }
    return (result);
}

/*****************************************************************************/
//  Description : 判断情景模式是否重名
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIENVSET_HasSameName(
                                    const MMI_STRING_T *str_ptr// IN
                                    )
{
    BOOLEAN                            res = FALSE;
    #if 0  //for NEWMS00126224
    MMIENVSET_SETTING_T       mode_setting = {0};
    MMI_STRING_T              temp_str     = {0};
    uint8 cur_select = MMIENVSET_GetCurModeId();
    uint8 i = 0;
    
    for(i=MMIENVSET_STANDARD_MODE; i< MMIENVSET_TOTAL_MODE; i++)
    {   
        if(i != cur_select)
        {
            SCI_MEMSET(&mode_setting,0,sizeof(MMIENVSET_SETTING_T));
            MMIAPIENVSET_GetModeValue(i, &mode_setting);
            
            SCI_MEMSET(&temp_str,0,sizeof(MMI_STRING_T));     
            temp_str.wstr_ptr = mode_setting.name_str_t.wstr;
            temp_str.wstr_len = mode_setting.name_str_t.wstr_len;
            if(0 == mode_setting.name_str_t.wstr_len)
            {
                if(i < MMIENVSET_USER_MODE1)
                {
                    MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(g_mmienvset_text_id[i]),&temp_str);
                }
                else if((i >= MMIENVSET_USER_MODE1) && (i <MMIENVSET_TOTAL_MODE))
                {
                    GetDefaultEnvsetNameStr(i,&temp_str);
                
                }
            }
            if(MMIAPICOM_StrCompare(str_ptr,&temp_str))
            {
                res = TRUE;
                break;
            }
        }
    }
    #endif 
    return res;
}
//#if defined MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : 删除自定义情景模式
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_HandleDeleteItem(
                                    uint16 item_index // IN
                                    )
{
    SET_NV_ITEM_E               nv_item_id   = MMIENVSET_SETTING_BEGIN;  // nv枚举值
    MMIENVSET_SETTING_T         mode_setting = {0};

    if(item_index == MMIAPIENVSET_GetActiveModeId())
    {
        //如果删除的是当前正在使用的模式，则把常规模式做为默认模式
        MMIENVSET_SetCurModeId((uint8)MMIENVSET_STANDARD_MODE);
        MMIAPIENVSET_ActiveMode((uint8)MMIENVSET_STANDARD_MODE);
        MMIENVSET_SetPreActiveMode(MMIAPIENVSET_GetActiveModeId()); 
    }
    
    SCI_MEMSET(&mode_setting,0,sizeof(MMIENVSET_SETTING_T));    
    MMIAPIENVSET_GetModeValue(item_index, &mode_setting);
    SCI_MEMSET(&mode_setting.name_str_t,0,sizeof(ENVSET_NAME_STR_INFO_T));
    mode_setting.name_str_t.wstr_len = 0;
            
    nv_item_id = nv_item_id + item_index;    
    //SCI_TRACE_LOW:"MMIENVSET_HandleDeleteItem nv_item_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2223_112_2_18_2_18_58_2,(uint8*)"d",nv_item_id);
//#if defined MMI_PDA_SUPPORT
    mode_setting.is_valid = FALSE;
//#endif
    
    MMINV_WRITE(nv_item_id, &mode_setting);
    UpdateCurrentSetingInfo(item_index,&mode_setting);
    MMIENVSET_SetCurModeId(MMIAPIENVSET_GetActiveModeId());
    MMIENVSET_MainMenu_Append_LBItem(MMIENVSET_MAIN_MENU_CTRL_ID);
}

/*****************************************************************************/
//  Description : 无修改添加自定义情景模式
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_HandleAddItem(void)
{
    BOOLEAN                     result =   TRUE;
    SET_NV_ITEM_E               nv_item_id =   MMIENVSET_SETTING_BEGIN;  // nv枚举值
    uint16                      cur_select =   MMIENVSET_GetCurModeId();
    MMIENVSET_SETTING_T         mode_setting = {0};
    MMI_STRING_T                temp_str     =   {0};

    SCI_MEMSET(&mode_setting,0,sizeof(MMIENVSET_SETTING_T));
    MMIAPIENVSET_GetModeValue(cur_select,&mode_setting);
    
    if(0 == mode_setting.name_str_t.wstr_len)
    {
        wchar new_name[MMIENVSET_ENVNAME_MAX_LEN+1] = {0};
        MMI_STRING_T  string = {0};
        uint16 string_len = 0;
        char num_char[5] = {0};
        uint16 num_len = 0;

        MMI_GetLabelTextByLang(TXT_SETTINGS_ENVIONMENT, &string);
        string_len = string.wstr_len;

        sprintf(num_char, "%d",  (cur_select - MMIENVSET_USER_MODE1 + 1));
        num_len = (uint16)strlen(num_char); 

        SCI_MEMSET(new_name, 0, ((MMIENVSET_ENVNAME_MAX_LEN+1)*sizeof(wchar)));
        MMI_WSTRNCPY( new_name, MMIENVSET_ENVNAME_MAX_LEN, string.wstr_ptr, string.wstr_len, string.wstr_len);
        MMI_STRNTOWSTR( &new_name[string_len], MMIENVSET_ENVNAME_MAX_LEN - string_len, (uint8*)num_char, num_len, num_len );

        MMI_WSTRNCPY( mode_setting.name_str_t.wstr,
                                MMIENVSET_ENVNAME_MAX_LEN,
                                new_name,
                                string_len+num_len,
                                string_len+num_len
                                );
        mode_setting.name_str_t.wstr_len = string_len+num_len;  

        temp_str.wstr_ptr = mode_setting.name_str_t.wstr;
        temp_str.wstr_len = mode_setting.name_str_t.wstr_len;
        if(MMIENVSET_HasSameName(&temp_str))
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMMON_RENAME);
            result = FALSE;
            return result;
        }
    }

    nv_item_id = nv_item_id + cur_select;   
//#if defined MMI_PDA_SUPPORT    
    mode_setting.is_valid = TRUE;
//#endif
    
    //SCI_TRACE_LOW:"MMIENVSET_HandleAddItem nv_item_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2290_112_2_18_2_18_58_3,(uint8*)"d",nv_item_id);
    MMINV_WRITE(nv_item_id, &mode_setting);
    UpdateCurrentSetingInfo(cur_select,&mode_setting);
    MMIENVSET_MainMenu_Append_LBItem(MMIENVSET_MAIN_MENU_CTRL_ID);
    
    return (result);
}

/*****************************************************************************/
//  Description : 获取真正的模式ID
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: 根据原有设计，自建模式可能会被删除，中间会漏空，引用原函数GetEnvRealIndex
/*****************************************************************************/
PUBLIC uint16 MMIENVSET_GetEnvRealIndex(uint8 select_index)
{
    MMIENVSET_SETTING_T mode_setting = {0};
    uint16 i = 0;

    if (select_index < MMIENVSET_USER_MODE1)
    {
        i = select_index;
    }
    else
    {
        select_index = select_index - MMIENVSET_USER_MODE1;
                    
        for(i = MMIENVSET_USER_MODE1; i < MMIENVSET_TOTAL_MODE; i++)
        {
            SCI_MEMSET(&mode_setting, 0, sizeof(MMIENVSET_SETTING_T));
            MMIAPIENVSET_GetModeValue(i, &mode_setting);
            
//#ifdef MMI_PDA_SUPPORT
            if (mode_setting.is_valid)
//#else
//            if (0 != mode_setting.name_str_t.wstr_len)
//#endif            
            {
                if (0 == select_index)
                {
                    break;
                }            
                select_index--;
            }
        }
    }
    
    return i;
}

#if !defined MMI_GUI_STYLE_TYPICAL
/*************************************************************************************/
//  Description : 在环境设置的一级菜单(listbox)里追加(append)列表项
//  Global resource dependence : none
//  Author: jian.ma
//  Note:  
/**************************************************************************************/
void MMIENVSET_MainMenu_Append_LBItem(
                                      MMI_CTRL_ID_T ctrl_id
                                      )
{  
    GUILIST_ITEM_T              item_t              = {0};/*lint !e64*/
    GUILIST_ITEM_DATA_T         item_data           = {0};/*lint !e64*/
    MMIENVSET_SETTING_T         mode_setting_ptr    = {0};
    uint8                       mode_id             = 0;
    
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, MMIENVSET_TOTAL_MODE, FALSE );//max item 5
    GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_OPTION;
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
    item_data.softkey_id[MIDDLE_BUTTON]   = (MMI_IMAGE_ID_T)IMAGE_COMMON_SOFTKEY_OK_ICON;
#else
    item_data.softkey_id[MIDDLE_BUTTON]   = (MMI_TEXT_ID_T)TXT_ACTIVE_OPERATIVE;
#endif
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;
    
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    if(MMIENVSET_EARPHONE_MODE == MMIAPIENVSET_GetActiveModeId()
       ||MMIENVSET_BLUETOOTH_MODE == MMIAPIENVSET_GetActiveModeId())
    {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        item_data.softkey_id[MIDDLE_BUTTON]   = (MMI_IMAGE_ID_T)IMAGE_NULL;
#else
        item_data.image_id[MIDDLE_BUTTON]   = (MMI_TEXT_ID_T)TXT_NULL;
#endif
    }
    else
    {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
        item_data.softkey_id[MIDDLE_BUTTON]   = (MMI_IMAGE_ID_T)IMAGE_COMMON_SOFTKEY_OK_ICON;
#else
        item_data.image_id[MIDDLE_BUTTON]   = (MMI_TEXT_ID_T)TXT_ACTIVE_OPERATIVE;
#endif
    }
#endif
    //item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_MAIN_SETTING;
    //item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;
    
    //顺序添加五个列表项
    for( mode_id = 0; mode_id < MMIENVSET_TOTAL_MODE; mode_id++ )
    {
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
        if(MMIENVSET_EARPHONE_MODE == mode_id || MMIENVSET_BLUETOOTH_MODE == mode_id)
        {
#ifdef MID_SOFTKEY_USE_IMG_SUPPORT
            item_data.softkey_id[MIDDLE_BUTTON]   = (MMI_IMAGE_ID_T)IMAGE_NULL;
#else
            item_data.image_id[MIDDLE_BUTTON]   = (MMI_TEXT_ID_T)TXT_NULL;
#endif
        }
#endif
        SCI_MEMSET(&mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));
        MMIAPIENVSET_GetModeValue(mode_id,&mode_setting_ptr);
        if (mode_setting_ptr.is_valid)
        {
            // 读取出错 或 仍是出厂设置
            if( 0 == mode_setting_ptr.name_str_t.wstr_len )  
            {
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = g_mmienvset_text_id[mode_id];            
            }
            //修改后的名称
            else
            {
                item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
            
                item_data.item_content[0].item_data.text_buffer.wstr_len = mode_setting_ptr.name_str_t.wstr_len;  
            
                item_data.item_content[0].item_data.text_buffer.wstr_ptr = mode_setting_ptr.name_str_t.wstr;
            }
        
            GUILIST_AppendItem(ctrl_id,&item_t);
        }
    } //end for
}
/*************************************************************************************/
//  Description : update main menu list
//  Global resource dependence : none
//  Author: jian.ma
//  Note:  
/**************************************************************************************/
PUBLIC void MMIENVSET_UpdateMainMenuList(MMI_CTRL_ID_T ctrl_id)
{
    GUILIST_RemoveAllItems(ctrl_id);

    MMIENVSET_MainMenu_Append_LBItem(ctrl_id);
}
#else
/*************************************************************************************/
//  Description : 在环境设置的一级菜单(listbox)里追加(append)列表项
//  Global resource dependence : none
//  Author: maqianlin
//  Note:  这个listbox是文本列表，显示:普通环境、会议环境、安静环境、车内环境、户外环境
/**************************************************************************************/
void MMIENVSET_MainMenu_Append_LBItem(
                                      MMI_CTRL_ID_T ctrl_id
                                      )
{
    GUILIST_ITEM_T                        item_t  = {0};
    GUILIST_ITEM_DATA_T                   item_data  = {0};
    uint8                                 mode_id  = 0;
    uint8                                 list_index  = 0;
    uint8                                 select_index  = 0;
    uint8                                 focus_index  = 0;
    MMIENVSET_SETTING_T    mode_setting = {0};
    
    //设置最大项数
    GUILIST_SetMaxItem( ctrl_id, MMIENVSET_TOTAL_MODE, FALSE );
    GUILIST_SetOwnSofterKey( ctrl_id, TRUE );
    
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
    item_t.item_data_ptr = &item_data;
    item_data.softkey_id[LEFT_BUTTON] = (MMI_TEXT_ID_T)STXT_OPTION;
    item_data.softkey_id[MIDDLE_BUTTON]   = (MMI_TEXT_ID_T)TXT_ACTIVE_OPERATIVE;
    item_data.softkey_id[RIGHT_BUTTON]  = (MMI_TEXT_ID_T)STXT_RETURN;

        //添加列表项
    for( mode_id = 0; mode_id < MMIENVSET_TOTAL_MODE; mode_id++ )
    {
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
        if(MMIENVSET_EARPHONE_MODE == mode_id || MMIENVSET_BLUETOOTH_MODE == mode_id)
        {
            item_data.softkey_id[MIDDLE_BUTTON]   = (MMI_TEXT_ID_T)TXT_NULL;
        }
        else
#endif
        {
            item_data.softkey_id[MIDDLE_BUTTON]   = (MMI_TEXT_ID_T)TXT_ACTIVE_OPERATIVE;
        }
    
        SCI_MEMSET(&mode_setting,0,sizeof(MMIENVSET_SETTING_T));
        MMIAPIENVSET_GetModeValue(mode_id, &mode_setting);
        
        if( 0 == mode_setting.name_str_t.wstr_len )  
        {
            if(mode_id < MMIENVSET_USER_MODE1)
            {
                item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;
                item_data.item_content[0].item_data.text_id = g_mmienvset_text_id[mode_id];      
                GUILIST_AppendItem(ctrl_id,&item_t);   
                if(MMIAPIENVSET_GetActiveModeId() == mode_id)
                {
                    select_index = list_index;
                }
                if(MMIENVSET_GetCurModeId() == mode_id)
                {
                    focus_index = list_index;
                }
                list_index++;
            }
            //else说明还未添加
        }
        else
        {
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;            
            item_data.item_content[0].item_data.text_buffer.wstr_len = mode_setting.name_str_t.wstr_len;              
            item_data.item_content[0].item_data.text_buffer.wstr_ptr = mode_setting.name_str_t.wstr;
            GUILIST_AppendItem(ctrl_id,&item_t);
   
            if(MMIAPIENVSET_GetActiveModeId() == mode_id)
            {
                select_index = list_index;
            }
            if(MMIENVSET_GetCurModeId() == mode_id)
            {
                focus_index = list_index;
            }
            list_index++;
        }
          
    }       
    GUILIST_SetCurItemIndex(ctrl_id,  focus_index);
    GUILIST_SetSelectedItem(ctrl_id, select_index, TRUE);   
} //end for
#endif
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//  Description :设置启动环境设置的图标 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
void MMIENVSET_SetActiveModeIcon(void)
{
    uint16  index = MMIAPIENVSET_GetActiveModeId(); 
    
    GUILIST_SetSelectedItem( MMIENVSET_MAIN_MENU_CTRL_ID, index, TRUE );
}
#endif
/*****************************************************************************/
//  Description :判断现设置的环境模式是否正在使用的环境模式,是返回true ，不是返回false 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
BOOLEAN MMIENVSET_IsActiveMode(uint8 mode_id)
{
    if(mode_id == s_mmienvset_active_mode)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description :从nv值中返回正在使用的环境设置模式 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetActiveModeId(void)
{
    return s_mmienvset_active_mode;
}

/*****************************************************************************/
//  Description :从nv值中返回正在使用的环境设置模式 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
LOCAL void   SetActiveModeId(uint8 mode_id)
{
    s_mmienvset_active_mode = mode_id;

    if(MMIENVSET_SILENT_MODE == s_mmienvset_active_mode ||
       MMIENVSET_MEETING_MODE == s_mmienvset_active_mode)
    {
        s_silent_pre_mode = s_mmienvset_active_mode;
    }
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    else if(MMIENVSET_EARPHONE_MODE == s_mmienvset_active_mode ||
       MMIENVSET_BLUETOOTH_MODE == s_mmienvset_active_mode)
    {
        s_noisy_pre_mode = MMIENVSET_STANDARD_MODE;
    }
#endif
    else
    {
        s_noisy_pre_mode = s_mmienvset_active_mode;
    }
    MMINV_WRITE(MMIENVSET_SETTING_CHOOSE,&s_mmienvset_active_mode);    
}


/*****************************************************************************/
//  Description :get play ring mode id
//  Global resource dependence : none
//  Author: jian.ma
//  Note:if earphone or bt mode and in mute state,change mode to pre active mode
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetPlayRingModeId(void)
{
    uint8 mode_id = s_mmienvset_active_mode;
    
    if (MMIAPIENVSET_GetMuteState() 
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
        &&(MMIENVSET_EARPHONE_MODE ==mode_id|| MMIENVSET_BLUETOOTH_MODE ==mode_id)
#endif 
       )
    {
        mode_id = s_mmienvset_pre_active_mode;
    }
    //SCI_TRACE_LOW:"MMIAPIENVSET_GetPlayRingModeId mode_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2585_112_2_18_2_18_59_4,(uint8*)"d",mode_id);
    return mode_id;
}

/*****************************************************************************/
//  Description :get pre active mode id
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetPreActiveMode(void)
{
    return s_mmienvset_pre_active_mode;
}
/*****************************************************************************/
//  Description :get mute state
//  Global resource dependence : none
//  Author: jian.ma
//  Note:only valid for earphone and bluetooth mode
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_GetMuteState(void)
{
    //SCI_TRACE_LOW:"MMIAPIENVSET_GetMuteState s_mute_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2612_112_2_18_2_18_59_5,(uint8*)"d",s_mute_state);
    return s_mute_state;
}

/*****************************************************************************/
//  Description :set mute state
//  Global resource dependence : none
//  Author: jian.ma
//  Note:only valid for earphone and bluetooth mode
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetMuteState(BOOLEAN state)
{
    s_mute_state = state;
    MMINV_WRITE(MMIENVSET_SETTING_MUTE_STATE,&s_mute_state);  
    //SCI_TRACE_LOW:"MMIAPIENVSET_SetMuteState s_mute_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2624_112_2_18_2_18_59_6,(uint8*)"d",s_mute_state);
}

/*****************************************************************************/
//  Description :按照上一次设置的情景模式设置启动环境设置模式ID 
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void   SetPreActiveModeId(uint8 mode_id)
{
    s_mmienvset_pre_active_mode = mode_id;
    MMINV_WRITE(MMIENVSET_SETTING_PRECHOOSE,&s_mmienvset_pre_active_mode);    
}

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
                                   )
{
    uint8       result      =   0;

    result = GetEnvSetOptValue(dual_sys, mode_id,set_opt);
    
    //SCI_TRACE_LOW:"MMIAPIENVSET_GetModeOptValue result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2651_112_2_18_2_18_59_7,(uint8*)"d",result);

    return result;
}
/*****************************************************************************/
//   Description : 启动环境设置
//   Global resource dependence : none
//   Author: maqianlin
//   Note:
//*****************************************************************************/
void MMIAPIENVSET_ActiveMode(uint8 mode_id)
{
    if(MMIENVSET_IsActiveMode(mode_id)) 
    {
        return;
    }
    else
    {  

        SetActiveModeId(mode_id);
        // 来电提示方式
        MMIAPISET_ActiveModeSetCallRingTypeIcon(mode_id);
        MAIN_UpdateIdleEnvsetState();
    }
}
/*****************************************************************************/
//  Description : 在IDLE下启动环境设置
//  Global resource dependence : none
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetActiveModeForIdle(uint8 mode_id)
{
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	 
    if (MMIENVSET_EARPHONE_MODE == MMIAPIENVSET_GetActiveModeId() 
        || MMIENVSET_BLUETOOTH_MODE== MMIAPIENVSET_GetActiveModeId())
    {
        SetPreActiveModeId(mode_id);
    }
    else
#endif
    {
        SetActiveModeId(mode_id);
    }

    // 来电提示方式
    MMIAPISET_ActiveModeSetCallRingTypeIcon(mode_id);
}
#if defined(MMI_WIDGET_SHORTCUT_SET) || defined(PDA_UI_DROPDOWN_WIN)
/*****************************************************************************/
//  Description : change to noise mode,means not silent mode
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_ChangeToNoiseMode(void)
{
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    if (MMIENVSET_EARPHONE_MODE == MMIAPIENVSET_GetActiveModeId() 
        || MMIENVSET_BLUETOOTH_MODE== MMIAPIENVSET_GetActiveModeId())
    {
        MMIAPIENVSET_SetMuteState(TRUE);
    }
    else
#endif
    {
        MMIAPIENVSET_ActiveMode(s_noisy_pre_mode);
    } 
}

/*****************************************************************************/
//  Description : change to silent moede 
//  Global resource dependence : none
//  Author: murphy.xie
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_ChangeToSilentMode(void)
{
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    if (MMIENVSET_EARPHONE_MODE == MMIAPIENVSET_GetActiveModeId() 
        || MMIENVSET_BLUETOOTH_MODE== MMIAPIENVSET_GetActiveModeId())
    {
        MMIAPIENVSET_SetMuteState(FALSE);
    }
    else
#endif
    {
        MMIAPIENVSET_ActiveMode(s_silent_pre_mode);
    } 
}
#endif
/*****************************************************************************/
//  Description : 获得进入设置的环境模式
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
uint8 MMIENVSET_GetCurModeId(void)
{    
    return s_mmienvset_cur_mode;
}


/*****************************************************************************/
//  Description : 获得进入设置的环境模式
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
void  MMIENVSET_SetCurModeId(uint8 mode_id)
{   
    s_mmienvset_cur_mode = mode_id;
}

/*****************************************************************************/
//  Description : 获得set_opt对应的设置项值
//  Global resource dependence : none
//  Author: maqianlin
//  Note:还要指定环境模式id
/*****************************************************************************/
LOCAL uint8 GetEnvSetOptValue(
                              MN_DUAL_SYS_E dual_sys,
                              uint8 mode_id,
                              MMIENVSET_SET_OPT_E  set_opt
                              )
{
    uint8               return_value                    =   0;
    MMIENVSET_SETTING_T *mode_setting_ptr               =   PNULL;

    mode_setting_ptr = GetModeValue(mode_id);

    switch(set_opt)
    {
        // 来电铃声音量
    case CALL_RING_VOL:
        return_value = mode_setting_ptr->call_ring_vol;
        break;
        
        // 来电铃声提示类型
    case CALL_RING_TYPE:
        return_value = mode_setting_ptr->call_ring_type[dual_sys];
        break;
        
        // 短信铃声音量
    case MSG_RING_VOL:
        return_value = mode_setting_ptr->msg_ring_vol;
        break;
        
        // 短信提示类型
    case MSG_RING_TYPE:
        return_value = mode_setting_ptr->msg_ring_type[dual_sys];
        break;
        
        // 通话音量
    case COMM_VOL:
        return_value = mode_setting_ptr->comm_vol;
        break;
        
        // 按键音量
    case KEY_RING_VOL:
        return_value = mode_setting_ptr->key_ring_vol;
        break;
        
#ifdef MMI_READMENU_ENABLE
    case MENUREAD_VOL:
        return_value = mode_setting_ptr->menuread_vol;
        break;
#endif
        
        // 按键音类型
    case KEY_RING_TYPE:
        return_value = mode_setting_ptr->key_ring_type;
        break;
        // 触屏音类型
    case TKEY_RING_TYPE:
        return_value = mode_setting_ptr->tkey_ring_type;
        break;
    case TKEY_RING_VOL:
        return_value = mode_setting_ptr->tkey_ring_vol;
        break;
        // 翻合盖铃声类型
    case FLIP_RING_TYPE:
        return_value = mode_setting_ptr->flip_ring_type;
        break;
    case FLIP_DOWN_RING_TYPE:
        return_value = mode_setting_ptr->flip_down_ring_type;
        break;
        
#ifdef LCD_SLIDE_SUPPORT        
        // 滑盖铃声类型
    case SLIDE_RING_ON_TYPE:
        return_value = mode_setting_ptr->slide_on_ring_type;
        break;     
    case SLIDE_RING_OFF_TYPE:
        return_value = mode_setting_ptr->slide_off_ring_type;
        break;        
#endif
        
        // 闹钟铃声音量
    case ALARM_RING_VOL:
        return_value = mode_setting_ptr->alarm_ring_vol;
        break;
        
        // 闹钟提示类型类型
    case ALARM_RING_TYPE:
        return_value = mode_setting_ptr->alarm_ring_type;
        break;
        
        // 下载完成提示音开关
    case DOWNLOAD_NOTIFY:
        return_value = mode_setting_ptr->download_notify;
        break;

        // 低电压报警开关
    case VOLTAGE_WARN:
        return_value = mode_setting_ptr->low_voltage_warn;
        break;

        // 开始充电提示
    case CHARGE_NOTIFY:
        return_value = mode_setting_ptr->charge_notify;
        break;

        //开关机铃声选项
    case POWER_RING_SWITCH:
        return_value = mode_setting_ptr->power_ring_swithch;
        break;
    case POWER_RING_VOL:
        return_value = mode_setting_ptr->power_ring_vol;
        break;

    case ENVSET_DIAL_TP_VIBRATE_SWITCH:
        return_value = mode_setting_ptr->is_dial_tp_vibrator_on;
        break;
		
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    case ENVSET_DIAL_TONE_SWITCH:
        return_value = mode_setting_ptr->is_dial_tone_off;
        break;
#endif

    case ENVSET_TP_KEY_LOCK_SWITCH:
        return_value = mode_setting_ptr->is_tp_key_lock_on;
        break;

#ifdef MMI_READMENU_ENABLE
    case ENVSET_MENU_READ_SWITCH:
        return_value = mode_setting_ptr->is_menu_read;
        break;
#endif

    default:
        //SCI_TRACE_LOW:"GetEnvSetOptValue:set_opt is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2884_112_2_18_2_18_59_8,(uint8*)"");
        break;
    }
    //SCI_TRACE_LOW:"GetEnvSetOptValue return_value=%d mode_id=%d,set_opt=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2892_112_2_18_2_18_59_9,(uint8*)"ddd",return_value,mode_id,set_opt);
    return return_value;
}

/*****************************************************************************/
//  Description : 设置set_opt指定的设置项
//    Global resource dependence : none
//  Author: maqianlin
//  Note:还要指定环境模式id
/*****************************************************************************/
LOCAL void SetEnvSetOptValue(
                             uint8                   mode_id,
                             MMIENVSET_SET_OPT_E     set_opt,
                             uint8                  set_value,
                             MN_DUAL_SYS_E           dual_sys
                             )
{
    
    SET_NV_ITEM_E           nv_item_id                  =   MMIENVSET_SETTING_BEGIN;     
    MMIENVSET_SETTING_T     *mode_setting_ptr           =   PNULL;
    
    mode_setting_ptr = SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
    SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));
    MMIAPIENVSET_GetModeValue(mode_id,mode_setting_ptr);

    SCI_TRACE_LOW("SetEnvSetOptValue mode_id %d set_opt %d set_value %d dual_sys %d", mode_id, set_opt, set_value, dual_sys);

    switch(set_opt)
    {
        // 来电铃声音量
    case CALL_RING_VOL:
        if(MMISET_VOL_MAX >= set_value)
        {
            mode_setting_ptr->call_ring_vol = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2925_112_2_18_2_19_0_10,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
        // 来电铃声提示类型
    case CALL_RING_TYPE:
        
        if(MMISET_MAX_CALL_TYPE > set_value)
        {
            mode_setting_ptr->call_ring_type[dual_sys] = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2938_112_2_18_2_19_0_11,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
        // 短信铃声音量
    case MSG_RING_VOL:
        if(MMISET_VOL_MAX >= set_value)
        {
            mode_setting_ptr->msg_ring_vol = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2950_112_2_18_2_19_0_12,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
        // 短信提示类型
    case MSG_RING_TYPE:
        
        if(MMISET_MAX_MSG_TYPE > set_value)
        {
            mode_setting_ptr->msg_ring_type[dual_sys] = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2963_112_2_18_2_19_0_13,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
        // 通话音量
    case COMM_VOL:
        if(MMISET_VOL_MAX >= set_value)
        {
            mode_setting_ptr->comm_vol = set_value; 
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2975_112_2_18_2_19_0_14,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
        // 按键音量
    case KEY_RING_VOL:
        if(MMISET_VOL_MAX >= set_value)
        {
            mode_setting_ptr->key_ring_vol = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2987_112_2_18_2_19_0_15,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
        // 按键音类型
    case KEY_RING_TYPE:
        if (MMISET_MAX_OTHER_RING > set_value)
        {
            mode_setting_ptr->key_ring_type = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_2999_112_2_18_2_19_0_16,(uint8*)"dd",set_opt,set_value);
        }
        break;
        // 触屏音类型
    case TKEY_RING_TYPE:
        if(MMISET_MAX_OTHER_RING > set_value)
        {
            mode_setting_ptr->tkey_ring_type = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3010_112_2_18_2_19_0_17,(uint8*)"dd",set_opt,set_value);
        }
        break;      
    case TKEY_RING_VOL:
        if(MMISET_VOL_MAX >= set_value)
        {
            mode_setting_ptr->tkey_ring_vol = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3020_112_2_18_2_19_0_18,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
        // 翻合盖铃声类型
    case FLIP_RING_TYPE:
        if(MMISET_MAX_OTHER_RING > set_value)
        {
            mode_setting_ptr->flip_ring_type = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3032_112_2_18_2_19_0_19,(uint8*)"dd",set_opt,set_value);
        }
        break;
     case FLIP_DOWN_RING_TYPE:
        if(MMISET_MAX_OTHER_RING > set_value)
        {
            mode_setting_ptr->flip_down_ring_type = set_value;
        }
        else
        {
            SCI_TRACE_LOW("SetEnvSetOptValue fial set_opt =%d,set_value = %d",set_opt,set_value);
            
        }
        break;
        
#ifdef LCD_SLIDE_SUPPORT        
        // 滑盖铃声类型
    case SLIDE_RING_ON_TYPE:
        if(MMISET_MAX_OTHER_RING > set_value)
        {
            mode_setting_ptr->slide_on_ring_type = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3045_112_2_18_2_19_0_20,(uint8*)"dd",set_opt,set_value);
        }
        break;

    case SLIDE_RING_OFF_TYPE:
        if(MMISET_MAX_OTHER_RING > set_value)
        {
            mode_setting_ptr->slide_off_ring_type = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3056_112_2_18_2_19_0_21,(uint8*)"dd",set_opt,set_value);
        }
        break;
#endif
        
        //闹钟铃声的音量
    case ALARM_RING_VOL:
        if(MMISET_VOL_MAX >= set_value)
        {
            mode_setting_ptr->alarm_ring_vol = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3069_112_2_18_2_19_0_22,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
        // 闹钟提示类型类型
    case ALARM_RING_TYPE:
        if(MMISET_MAX_MSG_TYPE > set_value)
        {
            mode_setting_ptr->alarm_ring_type = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3081_112_2_18_2_19_0_23,(uint8*)"dd",set_opt,set_value);
        }
        break;

        // 下载完成提示音开关
    case DOWNLOAD_NOTIFY:
        if(MMISET_OPEN_CLOSE_ITEM > set_value)
        {
            mode_setting_ptr->download_notify = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3093_112_2_18_2_19_0_24,(uint8*)"dd",set_opt,set_value);
        }
        break;

        // 低电压报警开关
    case VOLTAGE_WARN:
        if(MMISET_OPEN_CLOSE_ITEM > set_value)
        {
            mode_setting_ptr->low_voltage_warn = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3105_112_2_18_2_19_0_25,(uint8*)"dd",set_opt,set_value);
        }
        break;

    // 开始充电提示
    case CHARGE_NOTIFY:
    {
        if(MMISET_OPEN_CLOSE_ITEM > set_value)
        {
            mode_setting_ptr->charge_notify = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_LOW("SetEnvSetOptValue fial set_opt =%d,set_value = %d",set_opt,set_value);
        }
        break;
    }
        
        //开关机铃声选项
    case POWER_RING_SWITCH:
        if(MMISET_OPEN_CLOSE_ITEM > set_value)
        {
            mode_setting_ptr->power_ring_swithch = set_value; 
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3117_112_2_18_2_19_0_26,(uint8*)"dd",set_opt,set_value);
        }
        break;
        
#ifdef MMI_READMENU_ENABLE
    case MENUREAD_VOL:
        if(MMISET_VOL_MAX >= set_value)
        {
            mode_setting_ptr->menuread_vol = set_value;  
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3129_112_2_18_2_19_0_27,(uint8*)"dd",set_opt,set_value);
        }
        break;
#endif
        
    case POWER_RING_VOL:
        if(MMISET_VOL_MAX >= set_value)
        {
            mode_setting_ptr->power_ring_vol = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3141_112_2_18_2_19_0_28,(uint8*)"dd",set_opt,set_value);
        }
        break;

    case ENVSET_DIAL_TP_VIBRATE_SWITCH:
        if(MMISET_OPEN_CLOSE_ITEM >= set_value)
        {
            mode_setting_ptr->is_dial_tp_vibrator_on = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3152_112_2_18_2_19_0_29,(uint8*)"dd",set_opt,set_value);
        }
        break; 

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    case ENVSET_DIAL_TONE_SWITCH:
        if(MMISET_OPEN_CLOSE_ITEM >= set_value)
        {
            mode_setting_ptr->is_dial_tone_off = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3164_112_2_18_2_19_0_30,(uint8*)"dd",set_opt,set_value);
        }
        break;
 #endif
        
    case ENVSET_TP_KEY_LOCK_SWITCH:
        if(MMISET_OPEN_CLOSE_ITEM >= set_value)
        {
            mode_setting_ptr->is_tp_key_lock_on = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3176_112_2_18_2_19_0_31,(uint8*)"dd",set_opt,set_value);
        }
        break;

#ifdef MMI_READMENU_ENABLE
    case ENVSET_MENU_READ_SWITCH:
        if (MMISET_OPEN_CLOSE_ITEM >= set_value)
        {
            mode_setting_ptr->is_menu_read = set_value;
        }
        else
        {
            //SCI_TRACE_LOW:"SetEnvSetOptValue fial set_opt =%d,set_value = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3188_112_2_18_2_19_0_32,(uint8*)"dd",set_opt,set_value);
        }
        break;
#endif
        
    default:
        //SCI_TRACE_LOW:"SetEnvSetOptValue:set_opt is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3194_112_2_18_2_19_0_33,(uint8*)"");
        break;
        
    }
    
    nv_item_id = MMIENVSET_SETTING_BEGIN + mode_id;/*lint !e64*/
    //SCI_TRACE_LOW:"SetEnvSetOptValue nv_item_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3200_112_2_18_2_19_0_34,(uint8*)"d",nv_item_id);

    MMINV_WRITE(nv_item_id,mode_setting_ptr);
    UpdateCurrentSetingInfo(mode_id,mode_setting_ptr);
    if (PNULL != mode_setting_ptr)/*lint !e774 */
    {
        SCI_FREE(mode_setting_ptr);
        mode_setting_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : 获得当前环境模式的设置项值
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetCurModeOptValue(
                                             MN_DUAL_SYS_E dual_sys,
                                             MMIENVSET_SET_OPT_E  set_opt
                                             )
{
    uint8       mode_id     =   0;
    uint8       result      =   0;
    
    // 返回当前的环境模式id
    mode_id = MMIENVSET_GetCurModeId();
    
    result = GetEnvSetOptValue(dual_sys, mode_id,set_opt);  
    
    return result;
}


/*****************************************************************************/
//  Description : 获得启动环境模式使用的 设置项值
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
uint8 MMIAPIENVSET_GetActiveModeOptValue(
                                         MN_DUAL_SYS_E dual_sys,
                                         MMIENVSET_SET_OPT_E  set_opt
                                         )
{
    uint8       mode_id     =   0;
    uint8       result      =   0;
    
    // 返回启动的环境模式id
    mode_id = MMIAPIENVSET_GetPlayRingModeId();//MMIAPIENVSET_GetActiveModeId();
    result = GetEnvSetOptValue(dual_sys, mode_id,set_opt);  

    //SCI_TRACE_LOW:"MMIAPIENVSET_GetActiveModeOptValue result=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_3249_112_2_18_2_19_0_35,(uint8*)"d",result);
  
    return result;
}


/*****************************************************************************/
//  Description : 设置当前的环境设置项
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
void MMIENVSET_SetCurModeOptValue(
                                  MMIENVSET_SET_OPT_E  set_opt,
                                  uint8             set_value,
                                  MN_DUAL_SYS_E        dual_sys
                                  )
{
    uint8       mode_id     =   0;
    
    // 返回当前的环境模式id 
    mode_id = MMIENVSET_GetCurModeId();
    
    SetEnvSetOptValue(mode_id,set_opt,set_value, dual_sys);
    
}


/*****************************************************************************/
//  Description : 设置指定的环境设置项的值
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
void MMIENVSET_SetActiveModeOptValue(
                                     MMIENVSET_SET_OPT_E  set_opt,
                                     uint8              set_value,
                                     MN_DUAL_SYS_E       dual_sys
                                     )
{
    uint8       mode_id     =   0;
    
    // 返回启动的环境模式id 
    mode_id = MMIAPIENVSET_GetActiveModeId();
    
    SetEnvSetOptValue(mode_id,set_opt,set_value, dual_sys);
    
}


/*****************************************************************************/
//  Description : 获得指定模式的铃声音量 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
LOCAL   uint8 GetRingVol(
                         uint8                  mode_id,
                         MMISET_ALL_RING_TYPE_E     ring_type
                         )
{
    
    switch(ring_type)
    {
        // 来电铃声VOL 
    case MMISET_RING_TYPE_CALL:
    case MMISET_RING_TYPE_OTHER:
        
        return GetEnvSetOptValue(MN_DUAL_SYS_1, mode_id,CALL_RING_VOL);
        //break;
        
        // 短信铃声VOL
    case MMISET_RING_TYPE_MSG:
        
        return GetEnvSetOptValue(MN_DUAL_SYS_1,mode_id,MSG_RING_VOL);
        //break;
        
        // 按键音VOL
    case MMISET_RING_TYPE_KEY:  

        return GetEnvSetOptValue(MN_DUAL_SYS_1,mode_id,KEY_RING_VOL);
        //break;
        
    case MMISET_RING_TYPE_TKEY:  
        return GetEnvSetOptValue(MN_DUAL_SYS_1,mode_id,TKEY_RING_VOL);
    
//        break;
        // 开关机VOL
    case MMISET_RING_TYPE_POWER:
    #ifdef CMCC_UI_STYLE
    case MMISET_RING_TYPE_POWER_OFF:
    #endif
        
        return GetEnvSetOptValue(MN_DUAL_SYS_1,mode_id,POWER_RING_VOL);
        //break;
        
        
        // 闹钟提示音VOL
    case MMISET_RING_TYPE_ALARM:
#ifdef MMI_AZAN_SUPPORT
	case MMISET_RING_TYPE_AZAN:
#endif
        return GetEnvSetOptValue(MN_DUAL_SYS_1,mode_id,ALARM_RING_VOL);
        //return MMISET_ALARM_DEFAULT_VOL;
        //break;
#ifdef MMI_ETWS_SUPPORT
    case MMISET_RING_TYPE_ETWS_PWS:
        return GetEnvSetOptValue(MN_DUAL_SYS_1,mode_id,MSG_RING_VOL);

    case MMISET_RING_TYPE_ETWS:
        return MMISET_VOL_MAX;
#endif

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.         
        // 翻合盖铃声VOL
    case MMISET_RING_TYPE_FLIP:
    case MMISET_RING_TYPE_FLIP_DOWN:    
        
        return MMISET_FLIP_RING_DEFAULT_VOL;
        //break;
#endif        
        
#ifdef LCD_SLIDE_SUPPORT
        // 滑盖铃声VOL
    case MMISET_RING_TYPE_SLIDE_ON:
    case MMISET_RING_TYPE_SLIDE_OFF:	
        
        return MMISET_SLIDE_RING_DEFAULT_VOL;
        //break;
#endif
        
        // DC ring VOL
    case MMISET_RING_TYPE_DC:
        
        return MMISET_DC_DEFAULT_VOL;
       // break;
       
#ifdef ASP_SUPPORT
    case MMISET_RING_TYPE_ASP:
        return MMISET_DC_DEFAULT_VOL;
#endif

    case MMISET_RING_TYPE_DC_COUNT:
        return MMISET_DC_DEFAULT_VOL;
        
    case MMISET_RING_TYPE_DC_FINAL_COUNT:
        return MMISET_ALARM_DEFAULT_VOL;

        // BAT ring VOL
    case MMISET_RING_TYPE_BAT:
        
        return MMISET_BAT_DEFAULT_VOL;

    case MMISET_RING_TYPE_DL:
        return MMISET_DL_DEFAULT_VOL;
#ifdef MMI_KEY_LOCK_SUPPORT
    case MMISET_RING_TYPE_KEY_LOCK:
        return MMISET_KEY_LOCK_RING_DEFAULT_VOL;
#endif
#ifdef MMI_READMENU_ENABLE
    case MMISET_RING_TYPE_READMENU:
       return GetEnvSetOptValue(MN_DUAL_SYS_1,mode_id,MENUREAD_VOL);
#endif

#ifdef MMI_INDIAN_SOS_SUPPORT
    case MMISET_RING_TYPE_SOS:
        return MMISET_VOL_MAX;
#endif

    default:        
        return 0;
        //break;    
    }
}

/*****************************************************************************/
//  Description : 获得指定环境模式的铃声ID
//  Global resource dependence : none
//  Author: maqianlin
//  Note:来电提示铃声选取第0组
/*****************************************************************************/
LOCAL uint8 GetRingID(
                      MN_DUAL_SYS_E             dual_sys,
                      uint8                     mode_id,
                      MMISET_ALL_RING_TYPE_E    ring_type
                      )
                      
{
    uint8                   ring_id         =   0;
    uint16                  group_id        =   0;
    MMISET_CALL_RING_T      call_ring_info  =   {MMISET_CALL_RING_FIXED, 0, 0};
    MMISET_MSG_RING_T       msg_ring_info   =   {MMISET_MSG_RING_FIXED, 0, 0};
    
    switch(ring_type)
    {
        // 来电铃声ID 
    case MMISET_RING_TYPE_CALL: 
        //get ring info of call ring
        call_ring_info = MMIAPISET_GetCallRingInfo(dual_sys,mode_id);
        if (MMISET_CALL_RING_MORE_RING == call_ring_info.call_ring_type)
        {
            if (MMIAPIUDISK_UdiskIsRun())
            {
                ring_id = MMISET_RING_DEFAULT_ID;
            }
            else
            {
                ring_id = MMISET_MAX_RING_ID;
            }
        }
        else
        {
           // ring_id = MMISET_GetFixedCallRingId(dual_sys);
            ring_id = (uint8)MMIAPIENVSET_GetFixCallRingId(dual_sys,mode_id);

        }
        break;
        
        // 短信铃声ID
    case MMISET_RING_TYPE_MSG:
        //get ring info of msg ring
        msg_ring_info = MMIAPISET_GetMsgRingInfo(dual_sys,mode_id);
        if (MMISET_MSG_RING_MORE_RING == msg_ring_info.msg_ring_type)
        {
            if (MMIAPIUDISK_UdiskIsRun())
            {
                ring_id = MMISET_RING_DEFAULT_ID;
            }
            else
            {
                ring_id = MMISET_MAX_RING_ID;
            }
        }
        else
        {
            ring_id = (uint8)MMIAPIENVSET_GetFixMsgRingId(dual_sys,mode_id);
        }       
        break;
        
        // 按键铃声ID
    case MMISET_RING_TYPE_KEY:
        ring_id = GetEnvSetOptValue(0,mode_id,KEY_RING_TYPE);/*lint !e64*/
        break;              
     // 触屏铃声ID
    case MMISET_RING_TYPE_TKEY:
        ring_id = GetEnvSetOptValue(0,mode_id,TKEY_RING_TYPE);/*lint !e64*/
        break;
        // 开关机铃声ID
    case MMISET_RING_TYPE_POWER:
    #ifdef CMCC_UI_STYLE
    case MMISET_RING_TYPE_POWER_OFF:
    #endif
        ring_id = MMISET_RING_DEFAULT_ID;
        break;
        
        // 闹钟ID
    case MMISET_RING_TYPE_ALARM:
        // 返回闹钟选择的ring_id,(铃声列表中 震动 的ring_id 为 0)
        ring_id = MMIAPIALM_GetRingId(group_id);
        break;

#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.         
        // 翻合盖ID
    case MMISET_RING_TYPE_FLIP:
        ring_id = GetEnvSetOptValue(0,mode_id,FLIP_RING_TYPE);/*lint !e64*/
        break;
     case MMISET_RING_TYPE_FLIP_DOWN:
        ring_id = GetEnvSetOptValue(0,mode_id,FLIP_DOWN_RING_TYPE);/*lint !e64*/
        break;
#endif
        
#ifdef LCD_SLIDE_SUPPORT
        // 滑盖ID
    case MMISET_RING_TYPE_SLIDE_ON:
        ring_id = GetEnvSetOptValue(0,mode_id,SLIDE_RING_ON_TYPE);
        break;
    case MMISET_RING_TYPE_SLIDE_OFF:
        ring_id = GetEnvSetOptValue(0,mode_id,SLIDE_RING_OFF_TYPE);
        break;
#endif
        
        // DC ID
    case MMISET_RING_TYPE_DC:
        ring_id = MMISET_DC_DEFAULT_RING_ID;
        break;

    case MMISET_RING_TYPE_DC_COUNT:
        ring_id = MMISET_DC_DEFAULT_RING_ID;
        break;

    case MMISET_RING_TYPE_DC_FINAL_COUNT:
        ring_id = MMISET_DC_DEFAULT_RING_ID;
        break;      
		
#ifdef  ASP_SUPPORT
    case MMISET_RING_TYPE_ASP:
        return MMISET_DC_DEFAULT_RING_ID;
#endif 

#ifdef MMI_INDIAN_SOS_SUPPORT
    case MMISET_RING_TYPE_SOS:
        ring_id = MMISET_RING_DEFAULT_ID;
        break;
#endif

        // BAT ID
    case MMISET_RING_TYPE_BAT:
        ring_id = MMISET_BAT_DEFAULT_RING_ID;
        break;        

    case MMISET_RING_TYPE_DL:
        ring_id = MMISET_DL_DEFAULT_RING_ID;
        break;
#ifdef MMI_KEY_LOCK_SUPPORT
    case MMISET_RING_TYPE_KEY_LOCK:
        ring_id = MMISET_KEYLOCK_DEFAULT_RING_ID;
        break;
#endif
#ifdef MMI_ETWS_SUPPORT
    case MMISET_RING_TYPE_ETWS:
        ring_id = MMISET_ETWS_DEFAULT_ID;
        break;

    case MMISET_RING_TYPE_ETWS_PWS:
        ring_id = MMISET_ETWS_PWS_DEFAULT_ID;
        break;
#endif
    default:
        ring_id = 0;
        break;
    }
    
    return (ring_id);
}

/*****************************************************************************/
//  Description : 获得预览用的铃声  ID 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:来电提示铃声选取第0组
/*****************************************************************************/
LOCAL  uint8 GetCurModeRingId(
                              MN_DUAL_SYS_E dual_sys,
                              MMISET_ALL_RING_TYPE_E ring_type
                              )
{
    uint8   mode_id     =   0;
    uint8   result      =   0;
    
    mode_id = MMIENVSET_GetCurModeId();
    
    result =  GetRingID(dual_sys, mode_id,ring_type);
    
    return result;
}


/*****************************************************************************/
//  Description : 获得当前模式用的铃声音量 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetCurModeRingVol(
                              MMISET_ALL_RING_TYPE_E ring_type
                              )
{
    uint8       mode_id     =   0;
    uint8       result      =   0;
    
    mode_id = MMIENVSET_GetCurModeId();
    
    result =  GetRingVol(mode_id,ring_type);
    
    return result;  
    
}


/*****************************************************************************/
//  Description : 获得启动模式用的铃声  ID 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:来电提示铃声还要由group_id决定
/*****************************************************************************/
uint8 MMIENVSET_GetActiveModeRingId(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMISET_ALL_RING_TYPE_E ring_type
                                    )
{
    uint8   mode_id     =   0;
    uint8   result      =   0;
    
    mode_id = MMIAPIENVSET_GetPlayRingModeId();//MMIAPIENVSET_GetActiveModeId();
    
    result = GetRingID(dual_sys, mode_id,  ring_type);
    
    return result;
}





/*****************************************************************************/
//  Description : 获得启动模式用的铃声音量 
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
uint8 MMIAPIENVSET_GetActiveModeRingVol(
                                        MMISET_ALL_RING_TYPE_E ring_type
                                        )
{
    uint8   mode_id     =    0;
    uint8   result      =    0;
    
    mode_id = MMIAPIENVSET_GetPlayRingModeId();//MMIAPIENVSET_GetActiveModeId();
    
    result = GetRingVol( mode_id,  ring_type);
    
    return result;  
    
}

/*****************************************************************************/
//  Description : alert ring play result
//  Global resource dependence : none
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
LOCAL void HandleRingPlayResult(
                                MMISRVAUD_REPORT_RESULT_E result, 
                                DPARAM param
                                )
{
    if(result != MMISRVAUD_REPORT_RESULT_STOP)
    {
        s_is_play_finished = TRUE;
    }
}
/*****************************************************************************/
//  Description : 通过指定音量预览铃声
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_GetPlayResult(void)
{   
    return s_is_play_finished;
}
/*****************************************************************************/
//  Description : 通过指定音量预览铃声
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_SetPlayResult(BOOLEAN result)
{   
    s_is_play_finished = result;
}

/*****************************************************************************/
//  Description : 通过指定音量预览铃声
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
void MMIENVSET_PreviewRingByVol(
                                MN_DUAL_SYS_E           dual_sys,
                                MMISET_ALL_RING_TYPE_E  ring_type,
                                uint8                   ring_vol,
                                uint8                   play_times   //播放次数
                                )
{
    uint8           ring_id         =   0;
    BOOLEAN         alm_is_fixed    =   TRUE;
    uint16          alm_ring_id     =   0;
    uint16          alm_fname_len   =   0;
    uint16          alm_fname[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};
    
    // 获得不同铃声类型的ring_id                                 
    ring_id = GetCurModeRingId(MN_DUAL_SYS_1, ring_type);
    
    // 预览铃声(ring_id为0的请况,在playring内部处理)
    if(MMISET_RING_TYPE_ALARM != ring_type)
    {   
        if ((MMISET_RING_TYPE_KEY ==ring_type)&&(0 != ring_vol)&&(0 == ring_id))
        {
            ring_id = 1;
        }
        if ((MMISET_RING_TYPE_TKEY ==ring_type)&&(0 != ring_vol)&&(0 != ring_id))
        {
            ring_id = 0;
        }
        
        MMIAPISET_PreviewRing(MN_DUAL_SYS_1, ring_id,ring_vol,play_times,ring_type,HandleRingPlayResult);
    }
    else 
    {
        MMIAPIALM_GetFirstAlarmRing(&alm_is_fixed, &alm_ring_id, &alm_fname_len, alm_fname);
        if(alm_is_fixed)
        {
            MMIAPISET_PreviewRing(dual_sys, alm_ring_id,ring_vol,play_times,ring_type,HandleRingPlayResult);
        }
        else
        {
            if(!MMIAPIFMM_PlayMusicFile(MMISET_RING_TYPE_OTHER, FALSE,alm_fname, alm_fname_len, MMIENVSET_PREVIEW_PLAY_RING_TIMES,
                0, ring_vol, HandleRingPlayResult))
            {
                MMIAPISET_PreviewRing(dual_sys, MMISET_RING_DEFAULT_ID,ring_vol,play_times,ring_type,HandleRingPlayResult);

            }
            s_is_play_finished = FALSE;
        }
    }
}


/*****************************************************************************/
//  Description :通过指定ID 播放铃声
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
void MMIENVSET_PreviewRingById(
                               MMISET_ALL_RING_TYPE_E  ring_type,   
                               uint8                    ring_id,
                               uint8                    play_times   //播放次数
                               )
{
    uint8   ring_vol    =   0 ;
    
    // 获得不同铃声类型的ring_vol
    ring_vol = MMIAPIENVSET_GetCurModeRingVol(ring_type);
    
    // 预览铃声(ring_id为0的请况,在playring内部处理)
    MMIAPISET_PreviewRing(MN_DUAL_SYS_1, ring_id,ring_vol,play_times,ring_type,PNULL);
    
}



/*****************************************************************************/
//  Description : 使用环境设置的指定值播放铃声
//  Global resource dependence : none
//  Author: maqianlin
//  Note:只给MMIENVSET_PlayRingByListItem 调用
/*****************************************************************************/
LOCAL void PreviewRingBySetting(
                                MN_DUAL_SYS_E dual_sys,
                                MMISET_ALL_RING_TYPE_E  ring_type,  
                                uint8                   play_times,   //播放次数
                                MMISET_RING_CALLBACK_PFUNC callback_func
                                )
{
    uint8   ring_vol    =   0 ;
    uint8   ring_id     =   0;
    
    // 获得不同铃声类型的ring_id
    ring_id = GetCurModeRingId(dual_sys, ring_type);    
    
    // 获得不同铃声类型的ring_vol
    ring_vol = MMIAPIENVSET_GetCurModeRingVol(ring_type);
    
    // 预览铃声(ring_id为0的请况,在playring内部处理)
    MMIAPISET_PreviewRing(dual_sys, ring_id,ring_vol,play_times,ring_type,callback_func);
    
}


/*****************************************************************************/
//   Description : 列出来电铃声类型
//   Global resource dependence : 
//   Author: maqianlin
//   Note:
/*****************************************************************************/
LOCAL void AppendCallRingType(
                            MMI_CTRL_ID_T     ctrl_id
                            )
{
    
    //添加列表项
    MMIAPISET_AppendListItemByTextId(TXT_SET_RING_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//响铃
    MMIAPISET_AppendListItemByTextId(TXT_RING_VIBRA,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//振动
    MMIAPISET_AppendListItemByTextId(TXT_RING_SILENT,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//静音
    MMIAPISET_AppendListItemByTextId(TXT_SET_RING_VIBRA_BEFORE_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//先振动后响铃
    MMIAPISET_AppendListItemByTextId(TXT_SET_RING_VIBRA_AND_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//响铃并振动
    
}

/*****************************************************************************/
//   Description : 列出短信/闹钟提示类型
//   Global resource dependence : 
//   Author: maqianlin
//   Note:
/*****************************************************************************/
LOCAL void AppendMsgRingType(
                                       MMI_CTRL_ID_T       ctrl_id
                                       )
{
    //添加列表项
    MMIAPISET_AppendListItemByTextId(TXT_SET_RING_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//响铃 
    MMIAPISET_AppendListItemByTextId(TXT_RING_VIBRA,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//振动
    MMIAPISET_AppendListItemByTextId(TXT_RING_SILENT,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//静音
    MMIAPISET_AppendListItemByTextId(TXT_SET_RING_VIBRA_AND_RING,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//响铃并振动
}

/*****************************************************************************/
//   Description : 列出按键音/翻合盖提示音/开关机声音类型
//   Global resource dependence : 
//   Author: maqianlin
//   Note:
/*****************************************************************************/
LOCAL void AppendKeyRingType(
                           MMI_CTRL_ID_T     ctrl_id
                           )
{
    //添加列表项
    MMIAPISET_AppendListItemByTextId(TXT_RING_SILENT,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//静音
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_RING_RING1,STXT_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//铃声1
#ifndef MMI_LOW_MEMORY_RING
    MMIAPISET_AppendListItemByTextId(TXT_COMMON_RING_RING2,STXT_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);//铃声2
#endif

}

/*****************************************************************************/
//   Description : 列出所有的铃声类型
//   Global resource dependence : 
//   Author: maqianlin
//   Note:
/*****************************************************************************/
void MMIENVSET_RingType_AppendListItem(
                                       MN_DUAL_SYS_E dual_sys,
                                       MMI_CTRL_ID_T            ctrl_id,
                                       MMISET_ALL_RING_TYPE_E   ring_type,
                                       MMIENVSET_SET_OPT_E      set_opt
                                       )
{
    uint16      cur_selection   =   0;
    
    switch (ring_type)
    {
        //来电提示
    case MMISET_RING_TYPE_CALL:
        
        AppendCallRingType(ctrl_id);
        break;
        
        //消息提示
    case MMISET_RING_TYPE_MSG:
        //闹钟提示方式     
    case MMISET_RING_TYPE_ALARM:
        
        AppendMsgRingType(ctrl_id);
        break;
        
        //按键音
    case MMISET_RING_TYPE_KEY:
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.     
        //翻盖提示音    
    case MMISET_RING_TYPE_FLIP:
    case MMISET_RING_TYPE_FLIP_DOWN:
#endif    
        //开关机声音
#ifdef LCD_SLIDE_SUPPORT        
        //滑盖提示音	
    case MMISET_RING_TYPE_SLIDE_ON:
        //滑盖提示音	
    case MMISET_RING_TYPE_SLIDE_OFF:
#endif
    case MMISET_RING_TYPE_POWER:    
#ifdef CMCC_UI_STYLE
    case MMISET_RING_TYPE_POWER_OFF:
#endif
        
        AppendKeyRingType(ctrl_id);
        break;
    case MMISET_RING_TYPE_TKEY:
        MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
        MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);     
        break;
        
    default:
        
        break;
    }
    
    // 获得铃声类型
    cur_selection = (uint16)MMIAPIENVSET_GetCurModeOptValue(dual_sys, set_opt);
    
    // 显示选中项
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    // 显示阴影条
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    //GUILIST_UpdateListBox(ctrl_id);
    
}
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//   Description : 下载完成开启和关闭列表
//   Global resource dependence : none
//   Author: jiaoyou.wu
//   Note:
//*****************************************************************************/
void MMIENVSET_DownloadCompleteNotify_AppendListItem(
                                       MMI_CTRL_ID_T        ctrl_id
                                       )
{
    uint16      cur_selection   =   0;

    MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);

    cur_selection = MMIAPIENVSET_GetCurModeOptValue(MMIAPISET_GetActiveSim(), DOWNLOAD_NOTIFY);

    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);

    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
}

/*****************************************************************************/
//   Description : 低电压报警开启和关闭列表
//   Global resource dependence : none
//   Author: maqianlin
//   Note:
//*****************************************************************************/
void MMIENVSET_VoltWarn_AppendListItem(
                                       MMI_CTRL_ID_T        ctrl_id
                                       )
{
    uint16      cur_selection   =   0;
    
    // 添加列表项
    MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    // 获得低电压报警开关值
    cur_selection = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1, VOLTAGE_WARN);
    
    // 显示选中项
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    // 显示阴影条
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    
    //GUILIST_UpdateListBox(ctrl_id);
    
}

/*****************************************************************************/
//   Description : 设置开关机声音选项的列表
//   Global resource dependence : none
//   Author: kelly.li
//   Note:
//*****************************************************************************/
PUBLIC void MMIENVSET_AppendPowerRingSwitchListItem(
                                                    MMI_CTRL_ID_T       ctrl_id
                                                    )
{
    uint16      cur_selection   =   0;
    
    // 添加列表项
    MMIAPISET_AppendListItemByTextId(TXT_OPEN,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    MMIAPISET_AppendListItemByTextId(TXT_CLOSE,TXT_COMMON_OK,ctrl_id, GUIITEM_STYLE_ONE_LINE_RADIO);
    
    // 获得开关机声音的选项值
    cur_selection = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_1, POWER_RING_SWITCH);
    
    // 显示选中项
    GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
    
    // 显示阴影条
    GUILIST_SetCurItemIndex(ctrl_id,cur_selection);
    
    //GUILIST_UpdateListBox(ctrl_id);
    
}
#endif

/*****************************************************************************/
//  Description : 由当前阴影条指定播放不同的铃声或震动
//  Global resource dependence : none
//  Author: maqianlin
//  Note:处理来电铃声
/*****************************************************************************/
LOCAL void PlayCallRingByListItem(
                                MN_DUAL_SYS_E dual_sys,
                                MMISET_CALL_RING_TYPE_E     call_ring_type,
                                MMISET_ALL_RING_TYPE_E      ring_type,   //铃声类型
                                uint8                       play_times   //播放次数
                                )
{
    switch(call_ring_type)
    {
        // 响铃
    case MMISET_CALL_RING:
        // 切换提示方式时候关闭铃声和震动   
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
        {
            if(MMIAPISET_IsVibratTimerActive())
                MMIAPISET_StopVibratTimer();
            // 预览铃声     
            PreviewRingBySetting(dual_sys, ring_type,play_times,PNULL);
        }
        break;
        
        // 静音
    case MMISET_CALL_SILENT:
        
        // 预览静音时，关闭之前的震动或铃声
        MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER);   
        
        if(MMIAPISET_IsVibratTimerActive())
            MMIAPISET_StopVibratTimer();
#ifdef VIRTUAL_VIBRATE_FUNC
        MMISRVAUD_StopVibrate();
#endif
        break;
        
        // 震动
    case MMISET_CALL_VIBRA:
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
        {
            if(MMIAPISET_IsVibratTimerActive())
            {
                MMIAPISET_StopVibratTimer();
            }
            
            // 计时器在关闭窗口时调用MMIENVSET_StopVibraAndRing()关闭
            MMIAPISET_StartVibratTimer();
            // 开启震动
            MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_OTHER);
            
        }
        /*if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
        {
            if(MMIAPISET_IsVibratTimerActive())
                MMIAPISET_StopVibratTimer();
            
            // 计时器在关闭窗口时调用MMIENVSET_StopVibraAndRing()关闭
           // MMIAPISET_StartVibratTimer();
           MMIENVSET_StartOnetimeVibTimer();
           MMIENVSET_SetVibStatus(TRUE);
            // 开启震动
            MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_OTHER);
            
        }  */      
        break;
        // 先震动后响铃
    case MMISET_CALL_VIBRA_BEFORE_RING:
        // 切换提示方式时候关闭铃声和震动   
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
        {
            if(MMIAPISET_IsVibratTimerActive())
                MMIAPISET_StopVibratTimer();
            
            // 计时器在关闭窗口时调用MMIENVSET_StopVibraAndRing()关闭
           // MMIAPISET_StartVibratTimer();
            MMIENVSET_StartOnetimeVibTimer();
            MMIAPIENVSET_SetVibStatus(TRUE);
            // 开启震动
            MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_OTHER);
            
        }
        break;
        
        // 震动并响铃
    case MMISET_CALL_VIBRA_AND_RING:
        // 切换提示方式时候关闭铃声和震动   
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
        {
            if(MMIAPISET_IsVibratTimerActive())
                MMIAPISET_StopVibratTimer();
            
            // 预览铃声
            PreviewRingBySetting(dual_sys, ring_type,play_times,MMIAPISET_RingCallBack);
            // 开启震动         
            // s_is_vibra_start = TRUE;
            MMIENVSET_StartOnetimeVibTimer();
            MMIAPIENVSET_SetVibStatus(TRUE);
            MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_OTHER);
        }
        break;
        
    default:
        
        //SCI_TRACE_LOW:"PlayCallRingByListItem:call_ring_type is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4059_112_2_18_2_19_2_36,(uint8*)"");
        break;
        
    }
}

/*****************************************************************************/
//  Description : to stop alarm vibrate
//  Global resource dependence : none
//  Author: maryxiao
//  Note:
/*****************************************************************************/
LOCAL void MMIAPISET_AlarmRingCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)//cr227433 maryxiao
{
    MMIAPISET_StopVibrator(MMISET_RING_TYPE_OTHER);
    MMIENVSET_StopOnetimeVibTimer();
    MMIAPIENVSET_SetVibStatus(FALSE);
}

/*****************************************************************************/
//  Description : 由当前阴影条指定播放不同的铃声或震动
//  Global resource dependence : none
//  Author: maqianlin
//  Note:处理短信或闹钟铃声
/*****************************************************************************/
LOCAL void MMIENVSET_PlayMsgAlarmRingByListItem(
                                                MN_DUAL_SYS_E dual_sys,
                                                MMISET_MSG_RING_TYPE_E      msg_ring_type,
                                                MMISET_ALL_RING_TYPE_E      ring_type,   //铃声类型
                                                uint8                       play_times   //播放次数
                                                )
{
    switch(msg_ring_type)
    {
        // 响铃
    case MMISET_MSG_RING:
        // 切换提示方式时候关闭铃声和震动   
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
        {   
            if(MMIAPISET_IsVibratTimerActive())
                MMIAPISET_StopVibratTimer();
            
            // 预览铃声     
            PreviewRingBySetting(dual_sys, ring_type,play_times,PNULL);
        }
        break;
        
        // 震动
    case MMISET_MSG_VIBRA:
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
        {
            
            if(MMIAPISET_IsVibratTimerActive())
                MMIAPISET_StopVibratTimer();
            
            // 计时器在关闭窗口时调用MMIENVSET_StopVibraAndRing()关闭
            MMIAPISET_StartVibratTimer();
            // 开启震动
            MMIAPIENVSET_StartVibrator(MMISET_RING_TYPE_OTHER);
            
        }
        break;
        
        // 静音
    case MMISET_MSG_SILENT:
        // 预览静音时，关闭之前的震动或铃声
        MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER);       
        if(MMIAPISET_IsVibratTimerActive())
        {
            MMIAPISET_StopVibratTimer();
        }
        break;
        
        // 震动并响铃
    case MMISET_MSG_VIBRA_AND_RING:         
        if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_OTHER))
        {
            if(MMIAPISET_IsVibratTimerActive())
            {
                MMIAPISET_StopVibratTimer();
            }
            //#discuss
            if((0 == MMIAPIENVSET_GetCurModeOptValue(0,MSG_RING_VOL)) && (MMISET_RING_TYPE_MSG == ring_type))/*lint !e64*/
            {
                MMIAPISET_StartVibratTimer();
            }
            else
            {
                // 预览铃声
                //shipeng.lv add for cr150549 如果预览其他铃声也出现此情况，请参考处理
                if(MMISET_RING_TYPE_ALARM == ring_type)
                {
                    PreviewRingBySetting(dual_sys,ring_type,play_times,MMIAPISET_AlarmRingCallBack);//cr227433 maryxiao
                }
                else
                {
                    PreviewRingBySetting(dual_sys,ring_type,play_times,MMIAPISET_RingCallBack);
                }
                //end shipeng
#ifndef VIRTUAL_VIBRATE_FUNC
               MMIENVSET_StartOnetimeVibTimer();
#endif
            }
            // 开启震动
        }
        break;
        
    default:
        
        //SCI_TRACE_LOW:"MMIENVSET_PlayMsgAlarmRingByListItem:msg_ring_type is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4164_112_2_18_2_19_2_37,(uint8*)"");
        break;
    }
    
}

/*****************************************************************************/
//  Description : 由当前阴影条指定播放不同的铃声或震动
//  Global resource dependence : none
//  Author: maqianlin
//  Note:
/*****************************************************************************/
void   MMIENVSET_PlayRingByListItem(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMISET_ALL_RING_TYPE_E      ring_type,   //铃声类型
                                    MMI_CTRL_ID_T               ctrl_id,      //控件ID
                                    uint8                       play_times   //播放次数
                                    )
{
    uint32      index   =   0;
    
    index = GUILIST_GetCurItemIndex(ctrl_id);
    
    switch(ring_type)
    {
        // 来电铃声
    case MMISET_RING_TYPE_CALL:
        
        PlayCallRingByListItem(dual_sys, (MMISET_CALL_RING_TYPE_E)index,ring_type,play_times);
        break;
        
        // 短信或闹钟
    case MMISET_RING_TYPE_MSG:
    case MMISET_RING_TYPE_ALARM:
        
        MMIENVSET_PlayMsgAlarmRingByListItem(dual_sys, (MMISET_MSG_RING_TYPE_E)index,ring_type,play_times);
        break;
        
        // 开关机铃声/按键音/翻合盖/滑盖提示音
    case MMISET_RING_TYPE_POWER:
    case MMISET_RING_TYPE_KEY:
#ifdef FLIP_PHONE_SUPPORT  //close flip ring codes, just open for flip phone.     
    case MMISET_RING_TYPE_FLIP:
    case MMISET_RING_TYPE_FLIP_DOWN:
#endif    
    #ifdef CMCC_UI_STYLE
    case MMISET_RING_TYPE_POWER_OFF:
    #endif
#ifdef LCD_SLIDE_SUPPORT
    case MMISET_RING_TYPE_SLIDE_ON:
    case MMISET_RING_TYPE_SLIDE_OFF:
#endif        
        MMIENVSET_PreviewRingById(ring_type,(uint8)index,play_times);       
        break;
        
    default:
        
        //SCI_TRACE_LOW:"MMIENVSET_PlayRingByListItem:ring_type is error!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4218_112_2_18_2_19_2_38,(uint8*)"");
        break;              
    }
}

/*****************************************************************************/
//Description : 震动时间到 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
void   MMIENVSET_VibraTimerOut(
                               MN_DUAL_SYS_E dual_sys,
                               MMISET_ALL_RING_TYPE_E   ring_type,   //铃声类型
                               MMI_CTRL_ID_T           ctrl_id     //控件ID
                               )
{
    uint16        cur_item  =    0;
    
    switch(ring_type)
    {
        // 来电铃声
    case MMISET_RING_TYPE_CALL:
        {
            // 关闭震动
            MMIAPISET_StopVibrator(MMISET_RING_TYPE_OTHER);//关振动
            
            cur_item = GUILIST_GetCurItemIndex(ctrl_id);
            
            //(先震动后铃声)
            if (MMISET_CALL_VIBRA_BEFORE_RING == (MMISET_CALL_RING_TYPE_E)cur_item)
            { 
                PreviewRingBySetting(dual_sys, ring_type,MMIENVSET_PREVIEW_PLAY_RING_TIMES,PNULL);
            }
            
        }
        break;
        
        
    case MMISET_RING_TYPE_MSG:
    case MMISET_RING_TYPE_ALARM:    
        
        // 关闭震动
        MMIAPISET_StopVibrator(MMISET_RING_TYPE_OTHER);//关振动
        break;
        
    default:
        
        //SCI_TRACE_LOW:"MMIENVSET_VibraTimerOut: the ring_type is %d !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4264_112_2_18_2_19_2_39,(uint8*)"d",ring_type);
        break;
        
    }//end switch     
}

/*****************************************************************************/
//Description : 关闭窗口之前关闭所有的震动和铃声 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StopVibraAndRing(
                                  MMISET_ALL_RING_TYPE_E  ring_type
                                  )
{
    MMIAPISET_StopAppRing();
    MMIAPISET_StopVibrator(MMISET_RING_TYPE_OTHER);
    if(MMIAPISET_IsVibratTimerActive())
    {
        MMIAPISET_StopVibratTimer();
    }  
}

/*****************************************************************************/
//   Description : 待机状态#键切换静音,关闭所有铃声
//   Global resource dependence : none
//   Author: maqianlin
//   Note:
//*****************************************************************************/
//PUBLIC void MMIENVSET_SwitchToSilent(void)
//{
//    if(g_mmienvset_idle_switch_silent) 
//        g_mmienvset_idle_switch_silent = FALSE;
//    else
//        g_mmienvset_idle_switch_silent = TRUE;
//}

/*****************************************************************************/
//   Description : 判断待机状态#键切换静音(闭所有铃声)的状态
//   Global resource dependence : none
//   Author: maqianlin
//   Note:
//*****************************************************************************/
//BOOLEAN MMIENVSET_IsSilent(void)
//{
//    return  g_mmienvset_idle_switch_silent; 
// }

/*****************************************************************************/
//  Description : 恢复默认值,出厂设置
//  Global resource dependence : none
//  Author: maqianlin
//  Note:在mmiset_other.c的void MMISET_ResetFactorySetting(void)中调用
/*****************************************************************************/
void MMIENVSET_ResetEnvSetSetting(void)
{
    uint32                  i                   =   0;
    SET_NV_ITEM_E           nv_item_id          =   MMIENVSET_SETTING_BEGIN;
    MMIENVSET_SETTING_T     *mode_setting_ptr    =   PNULL;
    
    for(i = 0; i < MMIENVSET_TOTAL_MODE; i++)
    {
        mode_setting_ptr = &mmienvset_default_setting[i]; /*lint !e605*/
        nv_item_id = MMIENVSET_SETTING_BEGIN + i;/*lint !e64*/
        if(MMIENVSET_SETTING_END >= nv_item_id)
        {
            MMINV_WRITE(nv_item_id,mode_setting_ptr);
        }
    }
    // 启动的模式
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
    if(GPIO_CheckHeadsetStatus())
    {
        s_mmienvset_active_mode = MMIENVSET_EARPHONE_MODE;
        s_mmienvset_cur_mode = MMIENVSET_EARPHONE_MODE;
    }
    else
#endif
    {
        s_mmienvset_active_mode = MMIENVSET_DEFUALT_MODE;
        s_mmienvset_cur_mode = MMIENVSET_DEFUALT_MODE;
    }
    
    s_mmienvset_pre_active_mode = MMIENVSET_DEFUALT_MODE;
    
    MMINV_WRITE(MMIENVSET_SETTING_CHOOSE,&s_mmienvset_active_mode);
    MMINV_WRITE(MMIENVSET_SETTING_PRECHOOSE,&s_mmienvset_pre_active_mode);
    // 修改图标
    MMIAPISET_ActiveModeSetCallRingTypeIcon(s_mmienvset_active_mode);      
    MAIN_UpdateIdleEnvsetState();
    
    s_mmienvset_setting_info.cur_mode = MMIENVSET_TOTAL_MODE;

}

/*****************************************************************************/
//Description : 开启震动计时器,该计时器表示每次震动
//Global resource dependence : 
//Author: xiaoqing.lu 
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StartOnetimeVibTimer(void)
{
    MMIENVSET_StopOnetimeVibTimer();
    s_onetime_vibra_timer_id = MMK_CreateTimer(MMI_1SECONDS,FALSE);
}

/*****************************************************************************/
//Description : 开启震动计时器 , 该计时器表示每次震动
//Global resource dependence : 
//Author: maryxiao
//Note://cr231219
/*****************************************************************************/
PUBLIC void MMIENVSET_StartWinOnetimeVibTimer(MMI_HANDLE_T win_id)
{
    
    s_onetime_vibra_timer_id = MMK_CreateWinTimer(win_id,MMI_1SECONDS,FALSE);
   // MMK_StartWinTimer(win_id, s_onetime_vibra_timer_id, MMI_3SECONDS, FALSE);
    //SCI_TRACE_LOW:"MMIENVSET_StartWinOnetimeVibTimer %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4383_112_2_18_2_19_3_42,(uint8*)"d", s_onetime_vibra_timer_id);
}
/*****************************************************************************/
//Description : 关闭震动计时器,该计时器表示每次震动
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StopOnetimeVibTimer(void)
{
    
    if (s_onetime_vibra_timer_id != 0)
    {
        MMK_StopTimer(s_onetime_vibra_timer_id);
        s_onetime_vibra_timer_id = 0;
    }
}

/*****************************************************************************/
//Description : 判断是否震动计时器时间到 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_IsOnetimeVibTimerOut(DPARAM param)
{
    if (PNULL == param)
    {
        //MMI_ASSERT_WARNING("PNULL != param");/*assert verified*/
        return FALSE;
    }
   
    if(s_onetime_vibra_timer_id == (*(uint8 *)param))
    {
        s_onetime_vibra_timer_id = 0;
        return TRUE;
    }
    else
    {
        return FALSE; 
    }
}


/*****************************************************************************/
//Description : 判断震动计时器是否开启 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMIENVSET_IsOnetimeVibTimerActive(void)
//{
//    if ( 0 == s_onetime_vibra_timer_id )
//    {
//        return FALSE;
//    }
//    else
//    {
//        return TRUE; 
//    }
// }


/*****************************************************************************/
//Description : 判断震动计时器是否开启 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_GetVibStatus(void)
{
    return s_is_vibra_start;
}

/*****************************************************************************/
//Description : 判断震动计时器是否开启 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetVibStatus(BOOLEAN is_vibra_start)
{
    s_is_vibra_start = is_vibra_start;
}
/*****************************************************************************/
//  Description : 获得固定来电铃声的id
//  Global resource dependence : none
//  Author: maqianlin
//  Note:来电提示铃声选取第0组
/*****************************************************************************/
PUBLIC  uint16  MMIAPIENVSET_GetFixCallRingId(
                              MN_DUAL_SYS_E dual_sys,   
                              uint8 mode_id
                              )
{   
    MMIENVSET_SETTING_T *mode_setting_ptr   = PNULL;
    uint16              fix_ring_index      = 0;
    
    mode_setting_ptr = GetModeValue(mode_id);
    fix_ring_index = mode_setting_ptr->call_ring_info[dual_sys].fix_ring_index;

    return fix_ring_index;
}
/*****************************************************************************/
//  Description : 获得固定信息铃声的id
//  Global resource dependence : none
//  Author: maqianlin
//  Note:来电提示铃声选取第0组
/*****************************************************************************/
PUBLIC  uint16  MMIAPIENVSET_GetFixMsgRingId(
                              MN_DUAL_SYS_E dual_sys,
                              uint8 mode_id
                              )
{          
    MMIENVSET_SETTING_T *mode_setting_ptr   = PNULL;
    uint16              fix_ring_index      = 0;

    mode_setting_ptr = GetModeValue(mode_id);
    fix_ring_index = mode_setting_ptr->msg_ring_info[dual_sys].fix_ring_index;
    
    return fix_ring_index;
}

/*****************************************************************************/
//  Description : 获得固定闹铃铃声的id
//  Global resource dependence : none
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC  uint16  MMIAPIENVSET_GetFixAlarmRingId( uint8 mode_id )
{
    MMIENVSET_SETTING_T *mode_setting_ptr   = PNULL;
    uint16              fix_ring_index      = 0;

    mode_setting_ptr = GetModeValue( mode_id );
    fix_ring_index = mode_setting_ptr->alarm_ring_info.fix_ring_index;

    return fix_ring_index;
}

/*****************************************************************************/
//  Description : get call ring info including file name and ring type
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_CALL_RING_T MMIAPIENVSET_GetCallRingInfo(MN_DUAL_SYS_E dual_sys, uint8 mode_id)
{
    MMIENVSET_SETTING_T *mode_setting_ptr   = PNULL;
    MMISET_CALL_RING_T   call_ring_info     = {0};   /*lint !e64*/


    mode_setting_ptr = GetModeValue(mode_id);
    call_ring_info.call_ring_type = mode_setting_ptr->call_ring_info[dual_sys].call_ring_type;
    call_ring_info.fix_ring_index = mode_setting_ptr->call_ring_info[dual_sys].fix_ring_index;

    if (MMISET_CALL_RING_MORE_RING == mode_setting_ptr->call_ring_info[dual_sys].call_ring_type)
    {
        
        wchar                   filename[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};

        CombineMoreRingFullPath(filename);
       
        if(!ReadMoreRing(dual_sys,mode_id,&(call_ring_info.more_ring_info),MMIENVSET_MORE_RING_CALL,filename))
        {
            call_ring_info.call_ring_type = MMISET_CALL_RING_FIXED;
            call_ring_info.fix_ring_index = MMISET_RING_DEFAULT_ID;
        }
    }

    return call_ring_info;

}
/*****************************************************************************/
//  Description : get msg ring info including file name and ring type
//  Global resource dependence : 
//  Author:kelly.li
//  Note: 
/*****************************************************************************/
PUBLIC MMISET_MSG_RING_T MMIAPIENVSET_GetMsgRingInfo(MN_DUAL_SYS_E dual_sys,uint8 mode_id)
{   
    MMIENVSET_SETTING_T *mode_setting_ptr   = PNULL;
    MMISET_MSG_RING_T    msg_ring_info      = {0};/*lint !e64*/

    mode_setting_ptr = GetModeValue(mode_id);
    msg_ring_info.msg_ring_type = mode_setting_ptr->msg_ring_info[dual_sys].msg_ring_type;
    msg_ring_info.fix_ring_index = mode_setting_ptr->msg_ring_info[dual_sys].fix_ring_index;

    if (MMISET_MSG_RING_MORE_RING == mode_setting_ptr->msg_ring_info[dual_sys].msg_ring_type)
    {
        
        wchar  filename[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};

        CombineMoreRingFullPath(filename);
  
        if(!ReadMoreRing(dual_sys,mode_id,&(msg_ring_info.more_ring_info),MMIENVSET_MORE_RING_MSG,filename))
        {
            msg_ring_info.msg_ring_type = MMISET_MSG_RING_FIXED;
            msg_ring_info.fix_ring_index = MMISET_RING_DEFAULT_ID;
        }
    }

    return msg_ring_info;
}

/*****************************************************************************/
//  Description : 设置完更多来电铃声后，将信息存入nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_SetMoreCallRingInfo(
                                          MN_DUAL_SYS_E dual_sys,
                                          MMISET_CALL_MORE_RING_T ring_info,
                                          uint8 mode_id
                                          )
{   
    MMIENVSET_SETTING_T *mode_setting_ptr = PNULL;   
    wchar                   filename[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};

   CombineMoreRingFullPath(filename);
   
    if(WriteMoreRing(dual_sys,mode_id,ring_info,MMIENVSET_MORE_RING_CALL,filename))
    {
        mode_setting_ptr = SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
        if(mode_setting_ptr == PNULL)
        {
            return FALSE;
        }
        SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));   
        MMIAPIENVSET_GetModeValue(mode_id,mode_setting_ptr);   
        mode_setting_ptr->call_ring_info[dual_sys].call_ring_type = MMISET_CALL_RING_MORE_RING;  
        mode_setting_ptr->call_ring_info[dual_sys].fix_ring_index = MMISET_MSG_RING_DEFAULT_ID;
        //SCI_TRACE_LOW:"MMIAPIENVSET_SetMoreCallRingInfo nv_item_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4601_112_2_18_2_19_3_43,(uint8*)"d",mode_id);
   
        MMINV_WRITE((MMIENVSET_SETTING_BEGIN + mode_id),mode_setting_ptr);
        
        UpdateCurrentSetingInfo(mode_id,mode_setting_ptr);
        

        
        SCI_FREE(mode_setting_ptr);
        mode_setting_ptr = PNULL;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
    
}
/*****************************************************************************/
//  Description : 设置完更多信息铃声后，将信息存入nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_SetMoreMsgRingInfo(
                                          MN_DUAL_SYS_E dual_sys,
                                          MMISET_CALL_MORE_RING_T ring_info,
                                          uint8 mode_id
                                          )
{
    MMIENVSET_SETTING_T *mode_setting_ptr = PNULL;   
    wchar                   filename[MMIFILE_FULL_PATH_MAX_LEN+2] = {0};

   CombineMoreRingFullPath(filename);
   
   if(WriteMoreRing(dual_sys,mode_id,ring_info,MMIENVSET_MORE_RING_MSG,filename))
   {
        mode_setting_ptr = SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
        if(mode_setting_ptr == PNULL)
        {
            return FALSE;
        }
        SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));
        MMIAPIENVSET_GetModeValue(mode_id,mode_setting_ptr);
        mode_setting_ptr->msg_ring_info[dual_sys].msg_ring_type = MMISET_MSG_RING_MORE_RING;        
        mode_setting_ptr->msg_ring_info[dual_sys].fix_ring_index = MMISET_MSG_RING_DEFAULT_ID;
        //SCI_TRACE_LOW:"MMIAPIENVSET_SetMoreMsgRingInfo nv_item_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4640_112_2_18_2_19_3_44,(uint8*)"d",mode_id);
        MMINV_WRITE((MMIENVSET_SETTING_BEGIN+mode_id),mode_setting_ptr);
        UpdateCurrentSetingInfo(mode_id,mode_setting_ptr);

        
        SCI_FREE(mode_setting_ptr);
        mode_setting_ptr = PNULL;
        
        return TRUE;
   }
   else
   {
       return FALSE;
   }
}
/*****************************************************************************/
//  Description : 设置完固定铃声后，将ID信息存如nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetFixedCallRingId(
                                      MN_DUAL_SYS_E dual_sys,
                                      uint16 ring_id,
                                      uint8 mode_id
                                      )
{     
    MMIENVSET_SETTING_T *mode_setting_ptr = PNULL;
    mode_setting_ptr = SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
    if(mode_setting_ptr == PNULL)
    {
        return;
    }
    SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));
    MMIAPIENVSET_GetModeValue(mode_id,mode_setting_ptr);
    mode_setting_ptr->call_ring_info[dual_sys].call_ring_type = MMISET_CALL_RING_FIXED;
    mode_setting_ptr->call_ring_info[dual_sys].fix_ring_index = ring_id;  

    //SCI_TRACE_LOW:"MMIAPIENVSET_SetFixedCallRingId nv_item_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4671_112_2_18_2_19_3_45,(uint8*)"d",mode_id);
    MMINV_WRITE((MMIENVSET_SETTING_BEGIN+mode_id),mode_setting_ptr);
    UpdateCurrentSetingInfo(mode_id,mode_setting_ptr);
      
    SCI_FREE(mode_setting_ptr);
    mode_setting_ptr = PNULL;
}
/*****************************************************************************/
//  Description : 设置完固定铃声后，将ID信息存如nv
//  Global resource dependence : 
//  Author: kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetFixedMsgRingId(
                                      MN_DUAL_SYS_E dual_sys,
                                      uint16 ring_id,
                                      uint8 mode_id
                                      )
{
    MMIENVSET_SETTING_T *mode_setting_ptr = PNULL;
    mode_setting_ptr = SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
    if(mode_setting_ptr == PNULL)
    {
        return;
    }
    SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));
    MMIAPIENVSET_GetModeValue(mode_id,mode_setting_ptr);
    if((0 < ring_id) && (MMISET_MAX_RING_ID >= ring_id))  
    {
        mode_setting_ptr->msg_ring_info[dual_sys].msg_ring_type = MMISET_MSG_RING_FIXED;
        mode_setting_ptr->msg_ring_info[dual_sys].fix_ring_index = ring_id;  
    }
    else
    {
        mode_setting_ptr->msg_ring_info[dual_sys].msg_ring_type = MMISET_MSG_RING_FIXED;
        mode_setting_ptr->msg_ring_info[dual_sys].fix_ring_index = 1; 
        //SCI_TRACE_LOW:"MMIAPIENVSET_SetFixedMsgRingId fail ring_id=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4702_112_2_18_2_19_3_46,(uint8*)"d",ring_id);
    }

    //SCI_TRACE_LOW:"MMIAPIENVSET_SetFixedMsgRingId nv_item_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4705_112_2_18_2_19_3_47,(uint8*)"d",mode_id);
    MMINV_WRITE((MMIENVSET_SETTING_BEGIN+mode_id),mode_setting_ptr);
    UpdateCurrentSetingInfo(mode_id,mode_setting_ptr);


    SCI_FREE(mode_setting_ptr);
    mode_setting_ptr = PNULL;
    MMK_SendMsg(MMISET_RING_SELECT_MSG_WIN_ID, MSG_SET_SELECT_FIX_MSG_RING_RETURN, (ADD_DATA)ring_id);
}

/*****************************************************************************/
//  Description : 设置完固定铃声后，将ID信息存如nv
//  Global resource dependence : 
//  Author: bin.wang1
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetFixedAlarmRingId(
                                      uint16 ring_id,
                                      uint8 mode_id
                                      )
{
    MMIENVSET_SETTING_T *mode_setting_ptr = PNULL;

    mode_setting_ptr = (MMIENVSET_SETTING_T*)SCI_ALLOC_APP(sizeof(MMIENVSET_SETTING_T));
    if(mode_setting_ptr == PNULL)
    {
        return;
    }
    SCI_MEMSET(mode_setting_ptr,0,sizeof(MMIENVSET_SETTING_T));

    MMIAPIENVSET_GetModeValue(mode_id,mode_setting_ptr);
    if( (MMISET_ALARM_RING_MAX_NUM >= ring_id) && (ring_id > 0) )
    {
        mode_setting_ptr->alarm_ring_info.alarm_ring_type   = MMISET_MSG_RING_FIXED;
        mode_setting_ptr->alarm_ring_info.fix_ring_index    = ring_id;
    }
    else
    {
        mode_setting_ptr->alarm_ring_info.alarm_ring_type   = MMISET_MSG_RING_FIXED;
        mode_setting_ptr->alarm_ring_info.fix_ring_index    = MMISET_ALARM_RING_DEFAULT_ID;
        SCI_TRACE_LOW("MMIAPIENVSET_SetFixedAlarmRingId ringId error reset to default!!");
    }

    SCI_TRACE_LOW("MMIAPIENVSET_SetFixedAlarmRingId fix_ring_index = %u", mode_setting_ptr->alarm_ring_info.fix_ring_index);

    MMINV_WRITE((MMIENVSET_SETTING_BEGIN+mode_id),mode_setting_ptr);
    UpdateCurrentSetingInfo(mode_id,mode_setting_ptr);

    SCI_FREE(mode_setting_ptr);
    mode_setting_ptr = PNULL;
}

/*****************************************************************************/
//  Description : 从nv中获取相应的情景模式数据
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_GetModeValue(
                                      uint16 cur_mode,
                                      MMIENVSET_SETTING_T *mode_setting_ptr //[OUT]
                                      )
{
    MMIENVSET_SETTING_T     *setting_ptr    =   PNULL;

    //SCI_TRACE_LOW:"MMIAPIENVSET_GetModeValue cur_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4726_112_2_18_2_19_3_48,(uint8*)"d",cur_mode);
    if (PNULL == mode_setting_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIENVSET_GetModeValue PNULL == mode_setting_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4730_112_2_18_2_19_3_49,(uint8*)"");
        return ;
    }
    setting_ptr = GetModeValue(cur_mode);
    if(PNULL!=setting_ptr)
    {
        SCI_MEMCPY(mode_setting_ptr,setting_ptr,sizeof(MMIENVSET_SETTING_T));
    }
}

/*****************************************************************************/
//  Description : 从nv中获取相应的情景模式数据(内部接口)
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL MMIENVSET_SETTING_T* GetModeValue(uint16 cur_mode)
{
    MN_RETURN_RESULT_E      return_value    =   MN_RETURN_FAILURE;
    SET_NV_ITEM_E           nv_item_id      =   MMIENVSET_SETTING_BEGIN;    
   
    //SCI_TRACE_LOW:"MMIAPIENVSET_GetModeValue cur_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4726_112_2_18_2_19_3_48,(uint8*)"d",cur_mode);
    nv_item_id = nv_item_id + cur_mode;/*lint !e64*/

    if (cur_mode == s_mmienvset_setting_info.cur_mode)
    {
        return &s_mmienvset_setting_info.last_setting;
    }
    
    if(MMIENVSET_SETTING_END >= nv_item_id)
    {
        SCI_MEMSET(&s_mmienvset_setting_info.last_setting,0x00,sizeof(MMIENVSET_SETTING_T));
        MMINV_READ(nv_item_id, &s_mmienvset_setting_info.last_setting, return_value);
        
        //SCI_TRACE_LOW:"MMIAPIENVSET_GetModeValue nv_item_id = %d, return_value = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4737_112_2_18_2_19_3_50,(uint8*)"dd",nv_item_id,return_value);

        if(MN_RETURN_SUCCESS != return_value)
        {   
            SCI_MEMSET(&s_mmienvset_setting_info.last_setting,0x00,sizeof(MMIENVSET_SETTING_T));
            SCI_MEMCPY(&s_mmienvset_setting_info.last_setting,&mmienvset_default_setting[cur_mode],sizeof(MMIENVSET_SETTING_T));  
            MMINV_WRITE(nv_item_id,&s_mmienvset_setting_info.last_setting);        
        }  
        s_mmienvset_setting_info.cur_mode = cur_mode;
    }
    return &s_mmienvset_setting_info.last_setting;
}
/*****************************************************************************/
//  Description : update current setting info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void UpdateCurrentSetingInfo(uint16 mode_id,MMIENVSET_SETTING_T* mode_setting_ptr)
{
    if (mode_id == s_mmienvset_setting_info.cur_mode && PNULL != mode_setting_ptr)
    {
        SCI_MEMCPY(&s_mmienvset_setting_info.last_setting,mode_setting_ptr,sizeof(MMIENVSET_SETTING_T));
    }
}
/*****************************************************************************/
//  Description : 从文件中获取更多铃声信息
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ReadMoreRing(
                          MN_DUAL_SYS_E dual_sys,
                          uint8 mode_id,
                          MMISET_CALL_MORE_RING_T *ring_info,
                          MMIENVSET_MORE_RING_INDEX_E index,
                          wchar *filename
                          )
{
    MMIFILE_HANDLE          sfs_handle          = 0;
    BOOLEAN                 result              = FALSE;
    int32                   offset              = 0;
    uint32                  readed_size         = 0;

    sfs_handle = MMIAPIFMM_CreateFile(filename, SFS_MODE_READ | SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
    
    if (SFS_INVALID_HANDLE != sfs_handle)
    {
        offset = (mode_id * (MN_DUAL_SYS_MAX+MMIENVSET_MORE_RING_MAX_INDEX) + index * MN_DUAL_SYS_MAX + dual_sys) * sizeof(MMISET_CALL_MORE_RING_T);
        //SCI_TRACE_LOW:"ReadMoreRing mod_id = %d, index = %d, dual_sys = %d, offset = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4785_112_2_18_2_19_3_52,(uint8*)"dddd",mode_id,index,dual_sys,offset);
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(sfs_handle, offset, SFS_SEEK_BEGIN))
        {        
            if(SFS_ERROR_NONE == MMIAPIFMM_ReadFile(sfs_handle,ring_info,(uint32)sizeof(MMISET_CALL_MORE_RING_T),&readed_size,NULL))
            {
                //SCI_TRACE_LOW:"ReadMoreRing readed_size = %d,offset = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4790_112_2_18_2_19_3_53,(uint8*)"dd",readed_size,offset);
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"ReadMoreRing read file error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4795_112_2_18_2_19_3_54,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"ReadMoreRing set file pointer error offset = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4800_112_2_18_2_19_3_55,(uint8*)"d",offset);
        }
        MMIAPIFMM_CloseFile(sfs_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"ReadMoreRing createfile error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4806_112_2_18_2_19_3_56,(uint8*)"");
    }
    
    return result;

}
/*****************************************************************************/
//  Description : 写入文件更多铃声信息
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN WriteMoreRing(
                           MN_DUAL_SYS_E dual_sys,
                           uint8 mode_id,
                           MMISET_CALL_MORE_RING_T ring_info,
                           MMIENVSET_MORE_RING_INDEX_E index,
                           wchar                   *filename
                           )
{
    MMIFILE_HANDLE          file_handle          = 0;
    BOOLEAN                 result               = FALSE;
    uint32                  offset               = 0;
    uint32                  write_size           = 0;    

    
    file_handle = MMIAPIFMM_CreateFile(filename,SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, NULL, NULL);/*lint !e655*/
    if (SFS_INVALID_HANDLE != file_handle)
    {
        offset = (mode_id * (MN_DUAL_SYS_MAX+MMIENVSET_MORE_RING_MAX_INDEX) + index * MN_DUAL_SYS_MAX + dual_sys) * sizeof(MMISET_CALL_MORE_RING_T);
        //SCI_TRACE_LOW:"WriteMoreRing mod_id = %d, index = %d, dual_sys = %d, offset = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4834_112_2_18_2_19_4_57,(uint8*)"dddd",mode_id,index,dual_sys,offset);
        if (offset > (uint32)MMIAPIFMM_GetFileSize(file_handle))
        {
            MMIAPIFMM_SetFileSize(file_handle,(uint32)(offset+sizeof(MMISET_CALL_MORE_RING_T)));
        }
        if(SFS_ERROR_NONE == MMIAPIFMM_SetFilePointer(file_handle, offset, SFS_SEEK_BEGIN))
        {        
            if(SFS_ERROR_NONE == MMIAPIFMM_WriteFile(file_handle,&ring_info,(uint32)sizeof(MMISET_CALL_MORE_RING_T),&write_size,NULL))
            {
                //SCI_TRACE_LOW:"WriteMoreRing readed_size = %d,offset = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4843_112_2_18_2_19_4_58,(uint8*)"dd",write_size,offset);
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"WriteMoreRing read file error"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4848_112_2_18_2_19_4_59,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"WriteMoreRing set file pointer error offset = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4853_112_2_18_2_19_4_60,(uint8*)"d",offset);
        }
        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"WriteMoreRing createfile error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4859_112_2_18_2_19_4_61,(uint8*)"");
    }
    
    return result;

}

/*****************************************************************************/
//  Description : 组成更多铃声文件fullpath
//  Global resource dependence : 
//  Author: applezhang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CombineMoreRingFullPath(                           
                           const wchar  *filename
                           )
{
    uint16                  full_path_len        = MMIFILE_FULL_PATH_MAX_LEN+2;    
    BOOLEAN                 result = FALSE;

    result = MMIAPIFMM_GetSysFileFullPathForWrite(
                                                           TRUE,
                                                           MMIENVSET_MORE_RING_DIR,
                                                           MMIENVSET_MORE_RING_DIR_LEN,
                                                           MMIENVSET_MORE_RING_INI_FILE_NAME,
                                                           MMIAPICOM_Wstrlen(MMIENVSET_MORE_RING_INI_FILE_NAME),
                                                           0,
                                                           filename,    /*lint !e605*/
                                                           &full_path_len
                                                           );

    return result;    

}

/*****************************************************************************/
//  Description : create more ring dir if not exist
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_CreateMoreRingDir(void)
{
    BOOLEAN return_val = FALSE;
    
    return_val = MMIAPIFMM_CreateSysFileDir(MMIAPIFMM_GetDefaultDisk(),MMIENVSET_MORE_RING_DIR, MMIENVSET_MORE_RING_DIR_LEN,PNULL);
    
    return return_val;
}

/*****************************************************************************/
//  Description : create more ring ini file if not exist
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_CreateMoreRingFile(void)
{
    MMIFILE_HANDLE          file_handle          = 0;
    wchar  full_path_name[MMIFILE_FILE_NAME_MAX_LEN + 1] = {0};
    uint16 full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
 
    BOOLEAN                 result = FALSE;

    result = MMIAPIFMM_GetSysFileFullPathForWrite(
                                                           TRUE,
                                                           MMIENVSET_MORE_RING_DIR,
                                                           MMIENVSET_MORE_RING_DIR_LEN,
                                                           MMIENVSET_MORE_RING_INI_FILE_NAME,
                                                           MMIAPICOM_Wstrlen(MMIENVSET_MORE_RING_INI_FILE_NAME),
                                                           0,
                                                           full_path_name,
                                                           &full_path_len
                                                           );
    
    if(result)
    {
        file_handle = MMIAPIFMM_CreateFile(full_path_name, SFS_MODE_WRITE|SFS_MODE_READ|SFS_MODE_OPEN_ALWAYS, PNULL, PNULL);/*lint !e655*/
        
        if(SFS_INVALID_HANDLE != file_handle) 
        {
            MMIAPIFMM_SetFileSize(file_handle, (uint32)MMIENVSET_TOTAL_MODE*(MN_DUAL_SYS_MAX+MMIENVSET_MORE_RING_MAX_INDEX)*(sizeof(MMISET_CALL_MORE_RING_T)));
        }
        else
        {
            //SCI_TRACE_LOW:"MMIENVSET_CreateMoreRingFile: create file  error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4938_112_2_18_2_19_4_62,(uint8*)"");
        }
        MMIAPIFMM_CloseFile(file_handle);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIENVSET_CreateMoreRingFile: get device error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4944_112_2_18_2_19_4_63,(uint8*)"");
    }
}
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
// 	Description : set enhance ring state to nv
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void SetEnhanceRingState(BOOLEAN enhance_ring_state)
{
    s_enhance_ring_state = enhance_ring_state;
    MMINV_WRITE(MMIENVSET_ENHANCE_RING,&enhance_ring_state);
}
/*****************************************************************************/
// 	Description : get enhance ring state form nv
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN GetEnhanceRingState(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMIENVSET_ENHANCE_RING, &s_enhance_ring_state, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_enhance_ring_state = FALSE;
        MMINV_WRITE(MMIENVSET_ENHANCE_RING,&s_enhance_ring_state);
    }
    //SCI_TRACE_LOW:"MMIENVSET GetEnhanceRingState %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4970_112_2_18_2_19_4_64,(uint8*)"d",s_enhance_ring_state);
    return s_enhance_ring_state;
}

/*****************************************************************************/
// 	Description : set enhance ring state to nv
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetEnhanceRingState(BOOLEAN enhance_ring_state)
{
    SetEnhanceRingState(enhance_ring_state);
}

/*****************************************************************************/
// 	Description : get enhance ring state form nv
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENVSET_GetEnhanceRingState(void)
{
    #ifdef PLATFORM_SC6600L
    //SCI_TRACE_LOW:"MMIAPIENVSET_GetEnhanceRingState %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_4990_112_2_18_2_19_4_65,(uint8*)"d",s_enhance_ring_state);
    return s_enhance_ring_state;
    #else
    return FALSE;
    #endif
}
#endif
#if !defined DPHONE_SUPPORT && defined MMIENVSET_EARPHONE_BLUETOOTH_SUPPORT	
/*****************************************************************************/
// 	Description : set profile as earphone 
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_SetEarphoneMode(void)
{
    SetActiveModeId(MMIENVSET_EARPHONE_MODE);
    MMIAPIENVSET_SetMuteState(FALSE);
    s_mmienvset_cur_mode = MMIENVSET_EARPHONE_MODE;
    MMIENVSET_MainMenu_Append_LBItem(MMIENVSET_MAIN_MENU_CTRL_ID);//耳机模式启动时，重新append list，更新左软键
    MAIN_UpdateIdleEarPhoneState();
    MAIN_UpdateIdleEnvsetState();
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_UpdateMainWin();
#endif  
    return s_mmienvset_active_mode;
}
/*****************************************************************************/
// 	Description : set profile as bluetooth
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_SetBluetoothMode(void)
{
    SetActiveModeId(MMIENVSET_BLUETOOTH_MODE);
    MMIAPIENVSET_SetMuteState(FALSE);
    s_mmienvset_cur_mode = MMIENVSET_BLUETOOTH_MODE;
    MMIENVSET_MainMenu_Append_LBItem(MMIENVSET_MAIN_MENU_CTRL_ID);
    MAIN_UpdateIdleEnvsetState();
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_UpdateMainWin();
#endif
    return s_mmienvset_active_mode;
}
/*****************************************************************************/
// 	Description : undo set profile as earphone 
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_UnSetEarphoneMode(void)
{
    if(MMIAPIBT_GetActiveBTHeadset())
    {
        SetActiveModeId(MMIENVSET_BLUETOOTH_MODE);
    	s_mmienvset_cur_mode = MMIENVSET_BLUETOOTH_MODE;
    }
    else
    {
        SetActiveModeId(s_mmienvset_pre_active_mode);
        s_mmienvset_cur_mode = s_mmienvset_pre_active_mode;
    }
    MMIAPIENVSET_SetMuteState(FALSE);
    MMIENVSET_MainMenu_Append_LBItem(MMIENVSET_MAIN_MENU_CTRL_ID);
    MAIN_UpdateIdleEarPhoneState();
    MAIN_UpdateIdleEnvsetState();
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_UpdateMainWin();
#endif
    return s_mmienvset_active_mode;
}
/*****************************************************************************/
// 	Description : undo set profile as bluetooth
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_UnSetBluetoothMode(void)
{

    if(GPIO_CheckHeadsetStatus())
    {
        SetActiveModeId(MMIENVSET_EARPHONE_MODE);
        s_mmienvset_cur_mode = MMIENVSET_EARPHONE_MODE;
    }
    else
    {
        SetActiveModeId(s_mmienvset_pre_active_mode);
        s_mmienvset_cur_mode = s_mmienvset_pre_active_mode;
    }
    MMIAPIENVSET_SetMuteState(FALSE);
    MMIENVSET_MainMenu_Append_LBItem(MMIENVSET_MAIN_MENU_CTRL_ID);
    MAIN_UpdateIdleEnvsetState();
#ifdef PDA_UI_DROPDOWN_WIN
    MMIDROPDOWNWIN_UpdateMainWin();
#endif
    return s_mmienvset_active_mode;
}
#endif
/*****************************************************************************/
// 	Description : 获得情景模式下拨号盘振动的值
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIENVSET_GetModeDialTPVibrate(uint8 mode_id)
{
    uint8    dial_tp_vibrate = 0;
    MMIENVSET_SETTING_T mode_setting = {0};
    
    MMIAPIENVSET_GetModeValue(mode_id,&mode_setting);
    dial_tp_vibrate = mode_setting.is_dial_tp_vibrator_on;
    return(dial_tp_vibrate);
}
/*****************************************************************************/
// 	Description : 记录前一次的情景模式，供外部使用
//	 Global resource dependence : none
//  Author: juan.wu
//	 Note:
//*****************************************************************************/
PUBLIC void MMIENVSET_SetPreActiveMode(uint8 mode_id)
{
    SetPreActiveModeId(mode_id);
}    
/*****************************************************************************/
// 	Description : 获得默认的情景模式的名称
//	Global resource dependence : 
//  Author: juan.wu
//	Note:
/*****************************************************************************/
LOCAL void GetDefaultEnvsetNameStr(uint16 mode_id, MMI_STRING_T * name_str_ptr)
{
    if (mode_id<ARR_SIZE(g_mmienvset_text_id))
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(g_mmienvset_text_id[mode_id]),name_str_ptr);
    }
    else
    {
        MMI_GetLabelTextByLang((MMI_TEXT_ID_T)(g_mmienvset_text_id[0]),name_str_ptr);
    }
}

#ifdef MMI_POWER_SAVING_MODE_SUPPORT
/*****************************************************************************/
// 	Description : reset active mode setting
//	Global resource dependence : 
//  Author: juan.wu
//	Note:关闭当前使用模式的"其它提示音",并使得"其它提示音设置菜单灰化"
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_CloseActModeOherRingSet(void)
{
    uint8 act_mode_id = MMIAPIENVSET_GetActiveModeId();
    
    MMIENVSET_SetActiveModeOptValue(KEY_RING_TYPE, MMISET_OTHER_SILENT, 0);
    MMIENVSET_SetActiveModeOptValue(TKEY_RING_TYPE, MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE, 0);
    MMIENVSET_SetActiveModeOptValue(VOLTAGE_WARN, MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE, 0);
    MMIENVSET_SetActiveModeOptValue(POWER_RING_SWITCH, MMIENVSET_POWER_RING_SWITCH_CLOSE, 0);    
    MMIENVSET_SetActiveModeOptValue(ENVSET_DIAL_TP_VIBRATE_SWITCH, MMIENVSET_DIAL_TP_VIBRATE_SWITCH_CLOSE, 0);
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    MMIENVSET_SetActiveModeOptValue(ENVSET_DIAL_TONE_SWITCH, MMIENVSET_DIAL_TONE_SWITCH_CLOSE, 0);
#endif
    MMIENVSET_SetActiveModeOptValue(ENVSET_TP_KEY_LOCK_SWITCH, MMIENVSET_TP_KEY_LOCK_RING_SWITCH_CLOSE, 0);
    MMINV_WRITE(MMINV_SET_POWER_SAVING_OPRATE_ENV_MODE, &act_mode_id);
    
}
/*****************************************************************************/
// 	Description : reset active mode setting
//	Global resource dependence : 
//  Author: juan.wu
//	Note:恢复当前使用模式"其它提示音的默认值"
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_ResetActModeOtherRingSet(void)
{
    uint8 mode_id = MMIENVSET_GetPowerSavENVMode();
    uint8 invalid_id = MMIENVSET_TOTAL_MODE;

    if(mode_id < MMIENVSET_TOTAL_MODE)
    {
        MMIENVSET_SetActiveModeOptValue(KEY_RING_TYPE, mmienvset_default_setting[mode_id].key_ring_type, 0);
        MMIENVSET_SetActiveModeOptValue(TKEY_RING_TYPE, mmienvset_default_setting[mode_id].tkey_ring_type, 0);
        MMIENVSET_SetActiveModeOptValue(VOLTAGE_WARN, mmienvset_default_setting[mode_id].low_voltage_warn, 0);
        MMIENVSET_SetActiveModeOptValue(POWER_RING_SWITCH, mmienvset_default_setting[mode_id].power_ring_swithch, 0);    
        MMIENVSET_SetActiveModeOptValue(ENVSET_DIAL_TP_VIBRATE_SWITCH, mmienvset_default_setting[mode_id].is_dial_tp_vibrator_on, 0);
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
        MMIENVSET_SetActiveModeOptValue(ENVSET_DIAL_TONE_SWITCH, mmienvset_default_setting[mode_id].is_dial_tone_off, 0);
#endif
        MMIENVSET_SetActiveModeOptValue(ENVSET_TP_KEY_LOCK_SWITCH, mmienvset_default_setting[mode_id].is_tp_key_lock_on, 0);
        MMINV_WRITE(MMINV_SET_POWER_SAVING_OPRATE_ENV_MODE, &invalid_id);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIAPIENVSET_ResetActModeOtherRingSet it already reset mode_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_5164_112_2_18_2_19_4_66,(uint8*)"d",mode_id);
    }
}
/*****************************************************************************/
// 	Description : MMIENVSET_GetPowerSavENVMode
//	Global resource dependence : 
//  Author: juan.wu
//	Note:恢复当前使用模式"其它提示音的默认值"
/*****************************************************************************/
PUBLIC uint8 MMIENVSET_GetPowerSavENVMode(void)
{
    MN_RETURN_RESULT_E          return_value    =   MN_RETURN_FAILURE;
    uint8                       mode_id         =   0;
    
    MMINV_READ(MMINV_SET_POWER_SAVING_OPRATE_ENV_MODE, &mode_id, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        mode_id = 0;
        MMINV_WRITE(MMINV_SET_POWER_SAVING_OPRATE_ENV_MODE, &mode_id);
    }
    return mode_id;
}
#endif

/*****************************************************************************/
// 	Description : close active mode setting
//	Global resource dependence : 
//  Author: yongsheng.wang
//	Note:关闭当前使用模式的key ring
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_CloseActModeKeyRingSet(void)
{
    uint8 act_mode_id = MMIAPIENVSET_GetActiveModeId();
    MMIENVSET_SETTING_T* act_mode_value_ptr = GetModeValue(act_mode_id);
    if(PNULL == s_act_mode_value_ptr)
    {
        s_act_mode_value_ptr = SCI_ALLOC_APPZ(sizeof(uint8));//纪录按键音
        if(PNULL == s_act_mode_value_ptr)
        {
            SCI_TRACE_LOW("mmienvset.c, CloseActModeOherRing failed");
            return;
        }
        SCI_MEMCPY(s_act_mode_value_ptr, &(act_mode_value_ptr->key_ring_type), sizeof(uint8));
    }
    else
    {
        SCI_TRACE_LOW("mmienvset.c, no need CloseActModeOherRing");
        return;
    }
  
    MMIENVSET_SetActiveModeOptValue(KEY_RING_TYPE, MMISET_OTHER_SILENT, 0);
   // MMIENVSET_SetActiveModeOptValue(TKEY_RING_TYPE, MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE, 0);
   // MMIENVSET_SetActiveModeOptValue(VOLTAGE_WARN, MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE, 0);
    //MMIENVSET_SetActiveModeOptValue(POWER_RING_SWITCH, MMIENVSET_POWER_RING_SWITCH_CLOSE, 0);    
   // MMIENVSET_SetActiveModeOptValue(ENVSET_DIAL_TP_VIBRATE_SWITCH, MMIENVSET_DIAL_TP_VIBRATE_SWITCH_CLOSE, 0);
//#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
    //MMIENVSET_SetActiveModeOptValue(ENVSET_DIAL_TONE_SWITCH, MMIENVSET_DIAL_TONE_SWITCH_CLOSE, 0);
//#endif
    //MMIENVSET_SetActiveModeOptValue(ENVSET_TP_KEY_LOCK_SWITCH, MMIENVSET_TP_KEY_LOCK_RING_SWITCH_CLOSE, 0);
    //MMINV_WRITE(MMINV_SET_POWER_SAVING_OPRATE_ENV_MODE, &act_mode_id);
    
}
/*****************************************************************************/
// 	Description : reset active mode setting
//	Global resource dependence : 
//  Author: yongsheng.wang
//	Note:恢复当前使用模式key ring
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_ResetActModeKeyRingSet(void)
{

    if(PNULL != s_act_mode_value_ptr)
    {
        MMIENVSET_SetActiveModeOptValue(KEY_RING_TYPE, *s_act_mode_value_ptr, 0);
        //MMIENVSET_SetActiveModeOptValue(TKEY_RING_TYPE, mmienvset_default_setting[mode_id].tkey_ring_type, 0);
        //MMIENVSET_SetActiveModeOptValue(VOLTAGE_WARN, s_act_mode_value_ptr->low_voltage_warn, 0);
        //MMIENVSET_SetActiveModeOptValue(POWER_RING_SWITCH, s_act_mode_value_ptr->power_ring_swithch, 0);    
       // MMIENVSET_SetActiveModeOptValue(ENVSET_DIAL_TP_VIBRATE_SWITCH, mmienvset_default_setting[mode_id].is_dial_tp_vibrator_on, 0);
// #if defined(MMI_DIALPANEL_DTMF_SUPPORT)
//         MMIENVSET_SetActiveModeOptValue(ENVSET_DIAL_TONE_SWITCH, s_act_mode_value_ptr->is_dial_tone_off, 0);
// #endif
        //MMIENVSET_SetActiveModeOptValue(ENVSET_TP_KEY_LOCK_SWITCH, s_act_mode_value_ptr->is_tp_key_lock_on, 0);
        //MMINV_WRITE(MMINV_SET_POWER_SAVING_OPRATE_ENV_MODE, &invalid_id);
        SCI_FREE(s_act_mode_value_ptr);
        s_act_mode_value_ptr = PNULL;
    } 
    else
    {
        SCI_TRACE_LOW("mmienvset.c,  ResetActModeOherRing already");
    }
}


#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : 设置相应的情景模式数据
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENVSET_SetModeValue(
                                      uint16 cur_mode,
                                      MMIENVSET_SETTING_T *mode_setting_ptr //[OUT]
                                      )
{
    SET_NV_ITEM_E nv_item_id = MMIENVSET_SETTING_BEGIN;    
   
    //SCI_TRACE_LOW:"MMIAPIENVSET_SetModeValue cur_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_5199_112_2_18_2_19_4_67,(uint8*)"d", cur_mode);



    if (PNULL == mode_setting_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIENVSET_SetModeValue PNULL == mode_setting_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENVSET_5205_112_2_18_2_19_4_68,(uint8*)"");

        return;
    }
    nv_item_id = nv_item_id + cur_mode;/*lint !e64*/
    if (MMIENVSET_SETTING_END >= nv_item_id)
    {
        MMINV_WRITE(nv_item_id, mode_setting_ptr); 
        UpdateCurrentSetingInfo(cur_mode,mode_setting_ptr);
    }
}

#endif
/*****************************************************************************/
//  Description : set silence mode for BBAT test
//	Global resource dependence : none
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIENVSET_SetTestMode()
{
    //MMIAPIENVSET_SetActiveModeForIdle(MMIENVSET_SILENT_MODE);   //comment  for ref stack low which hard to run more function
    SetActiveModeId(MMIENVSET_SILENT_MODE);
}
/*****************************************************************************/
//  Description : ret back from BBAT test mode
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIENVSET_ClearTestMode()
{
    SetActiveModeId(MMIENVSET_DEFUALT_MODE);
}

