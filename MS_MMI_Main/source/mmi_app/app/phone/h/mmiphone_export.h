/*****************************************************************************
** File Name:      mmiphone_export.h                                                *
** Author:                                                                   *
** Date:           12/2003                                                   *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2003       Jassmine       Create
** 03/2004       Jassmine.Meng     Modify
******************************************************************************/

#ifndef _PHONEAPP_H_    
#define _PHONEAPP_H_   


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "mmi_text.h"
#include "mmk_type.h"
//@maggie add begin   for cr79064
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
//@maggie add end   for cr79064
#include "mmk_app.h"
#ifdef CSC_XML_SUPPORT
#include "mmicus_export.h"
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
    // PIN, PUK 最多和最少个数
#define MMI_PIN_MIN_LENGTH  4
#define MMI_PIN_MAX_LENGTH  8
    
#define MMI_PUK_MODIFY_PIN_MIN_LENGTH       20
#define MMI_PUK_MODIFY_PIN_MAX_LENGTH       32
#define MMI_PUK2_MODIFY_PIN2_MIN_LENGTH     21
#define MMI_PUK2_MODIFY_PIN2_MAX_LENGTH     33
    
#define PHONE_PIN_BLOCKED_TIME              1000
#define PHONE_ANIM_TIME_INTERVAL 	        500    // 以毫秒为单位的值
    
#define PHONE_RX_BAT_LEVEL_SPAN			10  
//ref报上电量的百分比，分别报上0，20，
//40，60，80, 100六个等级。
//目前为六个等级上面五个等级分别对应12345格，
//当报上warning信号时，信号量变成空！
    
    // 用于判断输入的位数是否正确。
#define PIN_LEN_IS_IN_RANGE(_LEN)               \
    ((MMI_PIN_MIN_LENGTH <= (_LEN))         \
    && (MMI_PIN_MAX_LENGTH >= (_LEN)))
    
    // 用于判断输入的PUK modify PIN位数是否正确。
#define PUK_MODIFY_PIN_LEN_IS_IN_RANGE(_LEN)            \
    ((MMI_PUK_MODIFY_PIN_MIN_LENGTH <= (_LEN))      \
    && (MMI_PUK_MODIFY_PIN_MAX_LENGTH >= (_LEN)))
    
    // 用于判断输入的PUK modify PIN位数是否正确。
#define PUK2_MODIFY_PIN2_LEN_IS_IN_RANGE(_LEN)            \
    ((MMI_PUK2_MODIFY_PIN2_MIN_LENGTH <= (_LEN))      \
    && (MMI_PUK2_MODIFY_PIN2_MAX_LENGTH >= (_LEN)))
    
#define MMIPHONE_ALARM_ANIM_LEFT            35
#define MMIPHONE_ALARM_ANIM_TOP             15
#define MMIPHONE_ALARM_ANIM_RIGHT           96
#define MMIPHONE_ALARM_ANIM_BOTTOM          71
    
#define MMIPHONE_NET_SEARCH_LABEL_TOP       100
#define MMIPHONE_NET_SEARCH_LABEL_BOTTOM    115
#define MMIPHONE_NET_SEARCH_ANIM_LEFT       50
#define MMIPHONE_NET_SEARCH_ANIM_TOP        16
#define MMIPHONE_NET_SEARCH_ANIM_RIGHT      91
#define MMIPHONE_NET_SEARCH_ANIM_BOTTOM     63
    
#define MMIPHONE_SIM_ERROR_LABEL_LEFT       0
#define MMIPHONE_SIM_ERROR_LABEL_TOP        114
#define MMIPHONE_SIM_ERROR_LABEL_RIGHT      127
#define MMIPHONE_SIM_ERROR_LABEL_BOTTOM     129
#define MMIPHONE_NETWORK_NAME_LEN            15

#define MAX_ACI_INFO_LEN     3

#define MMIPHONE_MAX_OPER_NAME_LEN  (MAX_ACI_INFO_LEN + MAX_ONSHORTFORM_LEN + MN_SPN_ID_NUM_LEN + 2) //opn_spn
#ifdef FLASH_SUPPORT
#define MAX_SUPPORT_SCR_NUMS    5
#endif

#ifdef MMI_SIM_LOCK_SUPPORT
#define MMIPHONE_SIM_UNLOCK_PCK_MIN_KEY_LEN         6    // PCK: 6 to 16 digits
#define MMIPHONE_SIM_UNLOCK_OTHER_KEY_MIN_LEN       8    // NCK,NSCK,SPCK,CCK: 8 to 16 digits
#define MMIPHONE_SIM_UNLOCK_CODE_MAX_LEN 16         //最大sim unlock 密码长度为16位
//MMI sim lock type
#define MMI_SIM_LOCK_TYPE_NUM                   5
#define MMI_SIM_LOCK_NONE                   0
#define MMI_SIM_NETWORK_LOCK                1
#define MMI_SIM_NETWORK_SUBSET_LOCK   (1 << 1)
#define MMI_SIM_SP_LOCK               (1 << 2)
#define MMI_SIM_CORPORATE_LOCK        (1 << 3)
#define MMI_SIM_USER_LOCK             (1 << 4)
#define MMI_SIM_LOCK_UNDEFINED           0xFF
    // 用于判断输入的位数是否正确。
#define SIM_UNLOCK_LEN_IS_IN_RANGE(_LEN)               \
    ((MMIPHONE_SIM_UNLOCK_PCK_MIN_KEY_LEN <= (_LEN))         \
    && (MMIPHONE_SIM_UNLOCK_CODE_MAX_LEN >= (_LEN)))

// operate SIM Lock 
typedef struct
{
    BOOLEAN    is_enable;   //enable or disable the lock Type
    uint32       lock_type;//
    MN_DUAL_SYS_E    dual_sys;
}MMI_OPERATE_SIM_LOCK_T;

typedef struct
{
    BOOLEAN need_handle_simlock[MMI_DUAL_SYS_MAX];		//need to handle sim unlock
    BOOLEAN is_handling_simlock[MMI_DUAL_SYS_MAX];		//is handling sim lock
    MN_DUAL_SYS_E   cur_handling_sim_id;					//current handling sim id
    APP_MN_SIM_NOT_READY_IND_T   sim_not_ready_ind[MMI_DUAL_SYS_MAX];	        //save SIM_NOT_READY_IND for SIM Lock
}MMIPHONE_HANDLE_SIMLOCK_STATUS_T;                                  //add for dual sim

#endif
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
#if defined(W217_UWS6121EG_NOGNSS_PATCH_20231019)
//启机过程中的状态
typedef enum
{
    MMI_STARTUP_STATE_NORMAL_INIT,  //启机后进入MMI的初始状态

#if defined TOUCH_PANEL_SUPPORT && !defined TOUCHPANEL_TYPE_MULTITP
    MMI_STARTUP_STATE_COORDINATE,   //第一次启机，坐标校准窗口，是否要保留???
#endif
    MMI_STARTUP_STATE_BOOTPWD,      //启机密码
    MMI_STARTUP_STATE_FLYMODE,      //飞行模式设置
    MMI_STARTUP_STATE_ANIMATION,    //开机动画
    MMI_STARTUP_STATE_PINPUK,       //Pin or Puk
    MMI_STARTUP_STATE_END,          //启机完成的最后状态，进入Idle或者Launcher
    MMI_STARTUP_STATE_MAX
}MMI_STARTUP_STATE_E;
#endif

//需要延迟的事件类型
typedef enum
{
	MMI_NONE_DELAY,
    MMI_USB_DELAY,
    MMI_CHARGE_PLUG_IN_DELAY,
	MMI_CHARGE_PLUG_OUT_DELAY,
	MMI_CHARGE_END_DELAY,
    MMI_SET_DATA_DELAY,
#if defined IM_ENGINE_SOGOU     //sogou输入法初始化比较慢,耗时约3.7s
    MMI_SOGOU_INIT_DELAY,
#endif

    MMI_DELAIED_EVENT_NUM
}MMI_DELAIED_EVENT_E;
	
// 根据REF设定的开机条件 
typedef enum
{
    STARTUP_NONE,
    STARTUP_NORMAL,
    STARTUP_ALARM,
    STARTUP_CHARGE,
    STARTUP_ASSERT,
    STARTUP_MAX
}PHONE_STARTUP_CONDITION_E;

typedef enum
{
    NO_DATA_SERVICE_TYPE,   //无任何数据业务
    GPRS_DATA_SERVICE_TYPE, //gprs业务
    EDGE_DATA_SERVICE_TYPE, //edge业务
    TD_DATA_SERVICE_TYPE,   //3g下普通数据业务
    HSDPA_DATA_SERVICE_TYPE,//hspda业务
    DATA_SERVICE_TYPE_MAX   //系统保留
}MMIPHONE_DATA_SERVICE_TYPE_E;

typedef enum
{
    MMIPHONE_PIN_PUK_STATE_WAIT = 0,
    MMIPHONE_PIN_PUK_STATE_INPUT,
    MMIPHONE_PIN_PUK_STATE_FINISH,
    MMIPHONE_PIN_PUK_STATE_MAX
}MMIPHONE_PIN_PUK_STATE_TYPE_E;

typedef enum
{
    NET_SERVICE_TYPE_G,   //GSM
    NET_SERVICE_TYPE_E,   //EDGE
    NET_SERVICE_TYPE_3G,  //TD
    NET_SERVICE_TYPE_4G,
    NET_SERVICE_TYPE_MAX  //
}MMIPHONE_NET_SERVICE_TYPE_E;

// 记录当前网络的状态。
typedef struct
{
    uint8 rssiLevel;
    BOOLEAN is_first_rssi;
    uint8 batteryLevel;
    BOOLEAN isInCharging;
    
    BOOLEAN isRoaming;    // if plmn is roaming plmn
    BOOLEAN is_slecting;  // 
    MN_PLMN_T plmn;
    uint16 lac;
    uint16 cell_id;
    MN_PHONE_PLMN_STATUS_E plmn_status;  // plmn statue	
    BOOLEAN gprs_support;
    BOOLEAN mbms_support;		//TRUE means support
    BOOLEAN edge_support;		//TRUE means support
    BOOLEAN hsdpa_support;		//TRUE means support
    BOOLEAN hsupa_support;		//TRUE means support, always FALSE for Tianlong R5    

    /*the following item is added for report operators's name*/
#if !defined(MMI_NETWORK_NAME_SUPPORT)
    BOOLEAN                         is_spn_need;  /*if the SPN exist, MMI will indicate if this item is true */
    uint8                                spn_len;  /*length of spn content*/
    MNBCD                            spn[MN_SPN_ID_NUM_LEN];  /*spn content*/
	
    BOOLEAN                         is_opn_need;  /*if the OPN need to indicate by MMI */
    BOOLEAN                         is_opn_exist;  /*if the OPN exist  in this struct*/
    uint8                                opn_len;                 /*length of opn content*/
    uint8                                opn[MAX_ONSHORTFORM_LEN];  /*opn content, short name length:SIM_EF_MAX_ONSHORTFORM_LEN  10*/
    MN_OPN_DCS_E                opn_dcs;                  /*dcs of the opn text string*/
    BOOLEAN is_add_ci;                  //whether add the country initials
#endif    
    MMI_GMMREG_RAT_E rat;
}PHONE_SERVICE_STATUS_T;

// 记录 SIM 卡的状态(简单)
typedef enum
{
    SIM_STATUS_OK,
    SIM_STATUS_REJECTED,
    SIM_STATUS_REGISTRATION_FAILED,
    SIM_STATUS_PIN_BLOCKED,
    SIM_STATUS_NOT_SUPPORT,//@zhaohui,cr109170	
    SIM_STATUS_PUK_BLOCKED,
    SIM_STATUS_PUK2_BLOCKED,
    SIM_STATUS_NO_SIM,
    SIM_NOT_INITED,
#if defined(MMI_SIM_LOCK_SUPPORT)
    SIM_LOCKED,
#endif
    SIM_STATUS_MAX
}SIM_STATUS_E;

// 记录 SIM 卡的状态(详细)
typedef struct
{
    SIM_STATUS_E simStatus;
    BOOLEAN fdnIsEnable;
    BOOLEAN pin1_is_enabled; 	    // PIN1 enabled/disabled when power on
    BOOLEAN pin1_is_blocked;		    // is pin1 blocked?  
    MNSIM_PIN_STATUS_T pin1_status;             // pin1 status
    MNSIM_PIN_STATUS_T pin2_status;             // pin2 status
    MNSIM_PIN_STATUS_T unblock_pin1_status;	    // blocked PIN1 status
    MNSIM_PIN_STATUS_T unblock_pin2_Status;	    // blocked PIN2 status
    BOOLEAN is_pin2_required_acm_max;// PIN2 is req'd for ACMmax update 
    BOOLEAN is_pin2_Required_acm;	// PIN2 is req'd for Acm update 
    BOOLEAN is_pin2_Required_puct;	// PIN2 is req'd for PUCT update 
    BOOLEAN is_aoc_support;                 //is AOC support
    MN_IMSI_T imsi;			        // international mobile subscriber id 
    MN_PLMN_T hplmn;
    MN_LOCI_T location_info;		    // Location area information 
    MNSIM_PHASE_E phase;			        // phase 1 or 2 
    MNSIM_SST_T sim_service;		        // sim service table 
    MNSIM_ADMIN_T admin;               
    BOOLEAN is_usim;
}PHONE_SIM_STATUS_T;

// 记录 NITZ network name
typedef struct
{
    BOOLEAN                         is_opn_exist;           
    uint8                                opn_len;                 /*length of opn content*/
    uint8                                opn[MAX_ONSHORTFORM_LEN];  /*opn content, short name length:SIM_EF_MAX_ONSHORTFORM_LEN  10*/
    MN_OPN_DCS_E              opn_dcs;                  /*dcs of the opn text string*/
    BOOLEAN                         is_add_ci;      /*is need add country index*/
}MMIPHONE_NV_NETWORK_NAME_T;

// 记录 SIM 卡network name
typedef struct
{
	/*NOTES:pnn opn ons must be alignment(2),because maybe a ucs2 string*/
    uint8                                pnn[MAX_ONSHORTFORM_LEN+MAX_ONSHORTFORM_LEN%2];  /*opn content, short name length:SIM_EF_MAX_ONSHORTFORM_LEN  10*/
    uint8                                opn[MAX_ONSHORTFORM_LEN+MAX_ONSHORTFORM_LEN%2];  /*opn content, short name length:SIM_EF_MAX_ONSHORTFORM_LEN  10*/
    uint8                                ons[MAX_ONSHORTFORM_LEN+MAX_ONSHORTFORM_LEN%2];  /*ons content, short name length SIM_EF_MAX_ONSHORTFORM_LEN  10*/
    /*NOTES:pnn opn ons must be alignment(2),because maybe a ucs2 string*/

    MN_PLMN_T                     cur_plmn;                                 //PLMN
    BOOLEAN                         is_nw_name_files_ready;           /*is SPN/ONS can read, not care PNN, read PNN after SIM Ready*/
    BOOLEAN                         is_add_ci;      /*is need add country index*/
    uint16                              lac;         
    
    /*service  provider name*/
    BOOLEAN                         display_condition;      
    BOOLEAN                         is_spn_supported;                   /*is spn support*/
    uint8                           spn_len;                                 /*length of spn content*/
    MNBCD                           spn[MN_SPN_ID_NUM_LEN];  /*spn content*/
	/*SPN icon (Reserved TODO ) */
#if defined(MMI_SIM_ICON_SUPPORT)       //usim only 
    BOOLEAN                         is_spni_ok;         //is spni ok 
    MN_SIM_ICON_INFO_T    spni;                //spni content
#endif
    /*following items are operator name's TLV, Type -Length -Value*/
    //this item is from PLMN network name
    MN_OPN_DCS_E              pnn_dcs;                  /*dcs of the pnn text string*/
    uint8                                pnn_len;                 /*length of PLMN network name content*/
#if defined(MMI_SIM_ICON_SUPPORT)       //usim only 
    MN_SIM_ICON_INFO_T    pnni;                     /*pnni content item*/
#endif
    //this item is from NITZ(network info)
    MN_OPN_DCS_E              opn_dcs;                  /*dcs of the opn text string*/
    uint8                                opn_len;                 /*length of opn content*/

    //this item is from CPHS(common PCN(personal communication network) handset specification)
    BOOLEAN                         is_cphs_exist;         /**/
    MN_OPN_DCS_E              ons_dcs;                  /*dcs of the ons(OPERATOR NAME STRING) text string*/
    uint8                                ons_len;                 /*length of ons content*/
}MMIPHONE_NETWORK_NAME_T;

// 记录 SIM 卡PNN and OPL data
typedef struct
{
    MN_PLMN_T                     cur_plmn;
    /*PLMN network name content*/
    BOOLEAN                         is_pnn_read_ok;             //is PNN init OK
    uint8                                curr_pnn_index;            //is array index, from 0
    uint8                                pnn_num;                       //total pnn num
    MN_APP_NET_NAME_INFO_T*  pnn_ptr;                       //pnn content header ptr    

    /*operator PLMN list*/
    BOOLEAN                         is_opl_read_ok;             //is OPL init OK
    uint8                                is_opl_empty;            //is record, default is FALSE
    uint8                                opl_num;                       //total opl num
    MN_SIM_OPL_ENTRY_T*  opl_ptr;                       //opl content header ptr    

    //following is reserved TO DO
    /*PNN icon list*/
#if defined(MMI_SIM_ICON_SUPPORT) 
    BOOLEAN                         is_pnni_read_ok;             //is PNNi init OK
    uint8                                pnni_num;                       //total pnni num
    MN_SIM_ICON_INFO_T*  pnni_ptr;                         //pnni content header ptr   
#endif    
    /*info num */
    BOOLEAN                         is_infonum_read_ok;             //is infonum init OK
    uint8                                infonum_num;                             //total infonum num
    MN_SIM_INFONUM_T*     infonum_ptr;                //infonum content header ptr    

}MMIPHONE_PNN_T;

// 记录 SIM  Present
typedef enum
{
    SIM_QUERY_STATUS_NONE,
    SIM_QUERY_STATUS_START,
    SIM_QUERY_STATUS_END,
    SIM_QUERY_STATUS_MAX
}SIM_QUERY_STATUS_E;

//PLMN NAME
typedef enum
{
    PLMN_TEST,
    #include "TS25_plmn_enum.h"

    PLMN_MAX_NUM,
    PLMN_INVALID
}PHONE_PLMN_NAME_E;

typedef enum
{
    MMIPHONE_ROAMING_STATUS_NO_ROMING,   //非漫游状态
    MMIPHONE_ROAMING_STATUS_NATIVE_ROMING, //国内跨网漫游
    MMIPHONE_ROAMING_STATUS_NATIONAL_ROMING, //国际漫游
    MMIPHONE_ROAMING_STATUS_MAX
}MMIPHONE_ROAMING_STATUS_E;
// 网络
typedef struct
{
    uint16 mcc;
    uint16 mnc;
    MMI_TEXT_ID_T name_index;
    PHONE_PLMN_NAME_E name;
}MMI_NETWORK_NAME_T;

typedef struct
{
    uint16 mcc;
    uint16 mnc;
    char ims_call_icon;
    char ims_icon_status;
    char ims_toggle_status;
    char reserved;
}MMI_IMS_TOGGLE_MNC_T;

typedef struct _MMI_IMS_TOGGLE_ST {
         uint16   flag;  // 0xface valid
         uint16   len;
         uint16   reserved[6];
}MMI_IMS_TOGGLE_ST;

typedef struct _MMI_NETWORK_ACI_INFO_T_
{
    uint16 mcc;
    char   aci_str[MAX_ACI_INFO_LEN + 1];
}MMI_NETWORK_ACI_INFO_T;

// operate PIN
typedef struct
{
    MNSIM_PIN_REQ_TYPE_E       operate_type;//PIN的操作标识
    MNSIM_PIN_INDICATION_E     pin_num;
}MMI_OPERATE_PIN_T;

// 记录解析的pin和puk的值。
typedef struct
{
    MN_BLOCK_T      pin_value;
    MN_BLOCK_T      ext_pin_value;		
}MMI_PIN_INFO_T;

typedef enum
{
    MMIPHONE_QUERY_POWER_OFF_DOWNLOAD,
    MMIPHONE_QUERY_POWER_OFF_MAX
}MMIPHONE_QUERY_POWER_OFF_TYPE_E;

typedef enum
{
    MMIPHONE_NOTIFY_EVENT_CS_AVAIL,
    MMIPHONE_NOTIFY_EVENT_CS_UNAVAIL,
    MMIPHONE_NOTIFY_EVENT_PS_AVAIL,
    MMIPHONE_NOTIFY_EVENT_PS_UNAVAIL,
    MMIPHONE_NOTIFY_EVENT_MAX
}MMIPHONE_NOTIFY_EVENT_E;

//MMIPHONE_NETWORK_SELECTION_TYPE_E
typedef enum
{
    MMIPHONE_SELECTION_SIM_ONLY =0x01,      //0b000001
    MMIPHONE_SELECTION_WLAN_ONLY= 0x02,     //0b000010
#ifdef BT_DIALER_SUPPORT
	MMIPHONE_SELECTION_SIM_NO_BT= 0x04,     //0b000100
#endif
    MMIPHONE_SELECTION_SIM_WLAN = (MMIPHONE_SELECTION_SIM_ONLY|MMIPHONE_SELECTION_WLAN_ONLY),
#ifdef BT_DIALER_SUPPORT
    MMIPHONE_SELECTION_SIM_ONLY_NO_BT = (MMIPHONE_SELECTION_SIM_ONLY|MMIPHONE_SELECTION_SIM_NO_BT),
#endif
    MMIPHONE_SELECT_NETWORK_MAX
}MMIPHONE_NETWORK_SELECTION_TYPE_E;

//MMIPHONE_SELECT_NETWORK_RESULT    if not SIM 
typedef enum
{
    MMIPHONE_WLAN_SELECTION =MN_DUAL_SYS_MAX,
    MMIPHONE_NET_SELECTION_MAX
}MMIPHONE_NET_SELECTION_RESULT_E;


typedef void (*MMIPHONE_NOTIFY_FUNC)(MN_DUAL_SYS_E dual_sys, MMIPHONE_NOTIFY_EVENT_E notify_event, uint32 param);

typedef struct 
{
    uint32 module_id;
    MMIPHONE_NOTIFY_FUNC notify_func;
}MMIPHONE_NOTIFY_INFO_T;

typedef struct MMIPHONE_NOTIFY_LIST_T_tag
{
    MMIPHONE_NOTIFY_INFO_T notify_info;
    struct MMIPHONE_NOTIFY_LIST_T_tag *next;
}MMIPHONE_NOTIFY_LIST_T;

typedef enum
{
    MMIPHONE_NOTIFY_SIM_PLUG_IN,
    MMIPHONE_NOTIFY_SIM_PLUG_OUT,
    MMIPHONE_NOTIFY_SIM_PLUG_MAX
}MMIPHONE_SIM_PLUG_EVENT_E;

typedef void (*MMIPHONE_SIM_PLUG_FUNC)(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E notify_event, uint32 param);

typedef struct 
{
    uint32 module_id;
    MMIPHONE_SIM_PLUG_FUNC notify_func;
}MMIPHONE_SIM_PLUG_NOTIFY_INFO_T;

typedef struct MMIPHONE_SIM_PLUG_NOTIFY_LIST_T_tag
{
    MMIPHONE_SIM_PLUG_NOTIFY_INFO_T notify_info;
    struct MMIPHONE_SIM_PLUG_NOTIFY_LIST_T_tag *next;
}MMIPHONE_SIM_PLUG_NOTIFY_LIST_T;

#ifdef FLASH_SUPPORT
typedef void (*pFunc)(void);
typedef struct 
{
  BOOLEAN is_valid;
  MMI_HANDLE_T win_id;
  pFunc func;
}scrDelCallBackType;
#endif


//sam.hua@Dec20 add data struct for select sim common api
typedef MMI_RESULT_E (* MMIAPIPHONE_SELECTSIM_FUNC)(uint32 dual_sys, BOOLEAN isOK, DPARAM param);

typedef struct 
{
    MMI_WIN_ID_T                    *win_id_ptr;    //win id,default is MMIPUB_ALERT_WIN_ID
    MMI_CTRL_ID_T                   publist_id;     //win id,default is MMIPUB_ALERT_WIN_ID
    uint32                          append_type;
    MMIAPIPHONE_SELECTSIM_FUNC      callback_func;
    MMIPHONE_NETWORK_SELECTION_TYPE_E  select_type;
    ADD_DATA                        user_data;
    ADD_DATA                        extra_data;
}MMIPHONE_SELECT_SIM_DATA_T;

//=================================VOLTE START===============================//
typedef enum
{
    MMIPHONE_VOLTE_ICON_VOLTE,
    MMIPHONE_VOLTE_ICON_HD,
    MMIPHONE_VOLTE_ICON_MAX
}MMIPHONE_VOLTE_ICON_STYLE_E;

typedef struct
{
    uint16 mcc;
    uint16 mnc;
    BOOLEAN is_volte_icon_on;
    BOOLEAN is_volte_default_on;
    BOOLEAN is_volte_set_display_on;
    MMIPHONE_VOLTE_ICON_STYLE_E volte_icon_style;
    uint8 reserved;
}MMIPHONE_VOLTE_CONFIG_INFO;
//=================================VOLTE END=================================//

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 正常开机界面的初始化
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_NormalInit(void);

/*****************************************************************************/
// 	Description : 正常开机输入开机密码后界面的初始化
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_NormalInitAfterPwd(void);

/*****************************************************************************/
// 	Description : 闹铃启动开机界面的初始化
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AlarmInit(void);

/*****************************************************************************/
// 	Description : 充电启动开机界面的初始化
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_ChargeInit(void);

/*****************************************************************************/
// 	Description : ASSERT启动的初始化,直接进入idle界面
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AssertInit(void);
PUBLIC char MMIAPIPHONE_4g_icon_display_style(void);
PUBLIC char MMIAPIPHONE_prefer_network_display_style(void);
PUBLIC BOOLEAN MMIAPIPHONE_IsIndiaSOSSupport(void);
/*****************************************************************************/
// 	Description : init the phone application
//	Global resource dependence : g_service_status,g_phone_app
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AppInit(void);

/*****************************************************************************/
// 	Description : according to ps status, judge if wap & mms is available
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_ROAMING_STATUS_E MMIAPIPHONE_GetRoamingStatus(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : power off
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOff(void);

/*****************************************************************************/
// 	Description : power off directly
//	Global resource dependence : 
//  Author: Jassmine
//	Note: sometimes we should poweroff directly
/*****************************************************************************/
void MMIAPIPHONE_PowerOffEx(void);

/*****************************************************************************/
// 	Description : power reset the phone should de_init the BT
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerReset(void);

/*****************************************************************************/
// 	Description : get capacity level(0-5)
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetBatCapacity(void);

/*****************************************************************************/
// 	Description : handle pin and puk flow
//	Global resource dependence : 
//  Author:jassmine.meng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_HandlePinAndPukFlow(
                             MNSIM_PIN_REQ_TYPE_E       operate_type,   //PIN的操作标识
                             MNSIM_PIN_INDICATION_E     pin_num,        //MNSIM_PIN1 or MNSIM_PIN2
                             uint32                     win_tab,
                             BOOLEAN is_special_func
                             );

/*****************************************************************************/
// 	Description : the current sim card is same to the last sim card
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsSameSim(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : 当sim卡注册失败和注册成功的时候都应该可以设置pin1
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPermitSetPin1(MN_DUAL_SYS_E    dual_sys);

/*****************************************************************************/
// 	Description :获取设置pin1 的sim 个数
//  Author: Michael.Shi
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetPermitSetPin1Num(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : get plmn name, cmcc, unicom or others
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*****************************************************************************/
PUBLIC PHONE_PLMN_NAME_E MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_E dual_sim);

/*****************************************************************************/
// 	Description : get the network name
//	Global resource dependence : 
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
MMI_TEXT_ID_T MMIAPIPHONE_GetNetWorkNameId(MMI_GMMREG_RAT_E rat, MN_PLMN_T *plmn_ptr);

/*****************************************************************************/
// 	Description : get the network id
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC PHONE_PLMN_NAME_E MMIAPIPHONE_GetNetWorkId(MN_PLMN_T *plmn_ptr);


/*****************************************************************************/
// 	Description : get the network info
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:in gprs data account,we only use one mnc to represent one operator,but it may have other mnc
//           if so,in auto adapting ,we will not find the right network id;so we use mcc and networkname index to get network name
/*****************************************************************************/
PUBLIC const MMI_NETWORK_NAME_T *MMIAPIPHONE_GetNetWorkInfo(PHONE_PLMN_NAME_E plmn_name);

/*****************************************************************************/
// 	Description : get full plmn name, cmcc, unicom or others
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetFullPlmn(MN_DUAL_SYS_E dual_sys, PHONE_PLMN_NAME_E *plmn_name, PHONE_PLMN_NAME_E *hplmn_name);

/*****************************************************************************/
// 	Description : Sim卡是否OK，包括PS
//	Global resource dependence : 
//  Author:ycd / michael
//	Note:  SIM OK 的状态: 'SIM_STATUS_OK' and 'PS_Ready_Normal' and '!fly_Mode'
//     Pay attention to this interface used,should better considered MMIAPIPHONE_IsSimAvailable 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsSimOk(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Sim卡是否OK，包括PS
//	Global resource dependence : 
//  Author:dave.ruanl
//	Note:  this for push mail lib func
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_IsSimOk(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Sim卡是否可用
//	Global resource dependence : 
//  Author: Michael.shi
//	Note:  SIM 卡是否可用的判断条件 : sim 卡存在并且处于待机设置！！
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsSimAvailable(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Sim卡是否OK，不包括PS
//	Global resource dependence : 
//  Author: michael
//	Note:  SIM Card OK 的状态包括两种: SIM_STATUS_OK / SIM_STATUS_REGISTRATION_FAILED
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsSimCardOk(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : get phone start up condition
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC PHONE_STARTUP_CONDITION_E MMIAPIPHONE_GetStartUpCondition(void);

/*****************************************************************************/
// 	Description : set phone start up condition
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetStartUpCondition(PHONE_STARTUP_CONDITION_E e_start_cond);

/*****************************************************************************/
// 	Description : get sim card and plmn service status string to display in idle
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetSimAndServiceString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T *network_str_ptr);

/*****************************************************************************/
// 	Description : 可用的Sim卡个数
//	Global resource dependence : 
//  Author: Michael.Shi
//	Note:  获取存在并且设置为待机的SIM卡的个数
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetSimAvailableNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : Sim卡OK个数和卡，包括PS
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetSimOkNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : Sim卡OK个数和卡，不包括PS
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetSimCardOkNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : Get sim card existed status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetSimExistedNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : Get ps ready status and num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetPSReadyNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : get sys ok num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetSysOkNum(BOOLEAN  *p, uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : get national roaming num
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetNationalRoamingNum(uint16 *idx_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : pop up an alert window to show sim wrong status
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AlertSimNorOKStatus(void);

/*****************************************************************************/
// 	Description : pop up an alert window to show sim wrong status
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AlertSimNorOKStatusII(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Is Pin2 Required ACM
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPin2RequiredACM(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Is Pin2 Required ACM Max
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPin2RequiredACMMax(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Is Pin2 Required Puct
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPin2RequiredPuct(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Is aoc support
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsAocSupport(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : get plmn status
//	Global resource dependence : 
//  Author:bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC MN_PHONE_PLMN_STATUS_E MMIAPIPHONE_GetPlmnStatus(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : 获得sim card状态
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC SIM_STATUS_E MMIAPIPHONE_GetSimStatus(MN_DUAL_SYS_E dual_sys);


/*****************************************************************************/
//  Description : creat and open Select SIM window by parameters
//  Global resource dependence : 
//  Author: sam.hua
//  Note  sam.hua@Dec20 add API declare for select sim common api
/*****************************************************************************/
PUBLIC	MMI_RESULT_E MMIAPIPHONE_OpenSelectSimWin(MMIPHONE_SELECT_SIM_DATA_T* selectsim_input_ptr);

/*****************************************************************************/
//     Description : 正常开机校准后界面的初始化
//    Global resource dependence : 
//  Author: Robert
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_NormalInitAfterCoordinate(void);

/*****************************************************************************/
// 	Description : get power on status when mp4 is playing
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPowerOnStatus(void);

//@maggie add begin for cr79064
/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_InitTDOrGsm(void);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_TDOrGsm(APP_MN_NETWORK_STATUS_IND_T network_status_ind);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
PUBLIC MMI_GMMREG_RAT_E MMIAPIPHONE_GetTDOrGsm(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//     Description : 
//    Global resource dependence : 
//  Author: maggie ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_StopSTTDOrGsm(void);

/*****************************************************************************/
//  Description : init a virtual window
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_VirtualWinInit(void);

/*****************************************************************************/
// 	Description : ????????
//	Global resource dependence : 
//  Author: yongwei.he
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetCurPLMN(
                                MN_PLMN_T * cur_plmn_ptr,
                                MN_DUAL_SYS_E dual_sys
                                );
                                
/*****************************************************************************/
// 	Description : get the sim hplmn
//	Global resource dependence : 
//  Author: fen.xie
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetSimHPLMN(
                                MN_PLMN_T * cur_plmn_ptr,
                                MN_DUAL_SYS_E dual_sys
                                );

/*****************************************************************************/
// 	Description : Gen PLMN Display
//	Global resource dependence : 
//  Author: wancan.you
//	Note:is_display_digit_format:TRUE Display 460 00
//          is_display_digit_format:FALSE Display CMCC
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GenPLMNDisplay(
                                                                            MMI_GMMREG_RAT_E rat,//IN
                                                                            MN_PLMN_T *plmn_ptr,//IN
                                                                            wchar *display_str,//[IN/OUT]
                                                                            uint16 *display_str_len,//[IN/OUT]
                                                                            BOOLEAN is_display_digit_format);

/*****************************************************************************/
// 	Description : set sim card exited status
//	Global resource dependence : 
//  Author:kelly.li
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetSimExistedStatus(
                                            MN_DUAL_SYS_E dual_sys,
                                            BOOLEAN       is_existed
                                            );

/*****************************************************************************/
// 	Description : start up ps according to setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_StartUpPsAccordingToSetting(BOOLEAN is_power_on, BOOLEAN is_init_sim);

/*****************************************************************************/
// 	Description : deactive ps according to setting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_DeactivePsAccordingToSetting(void);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetIsPsDeactiveCnf
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIsPsDeactiveCnf(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ps_deactive_cnf);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsDeactiveCnf
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsDeactiveCnf(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetIsPsReady
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIsPsReady(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ps_ready);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsReady
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsReady(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOffPs
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnPs(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_PowerOffPs
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOffPs(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : get dual sim status
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetDualSimStatus(BOOLEAN *is_sim1_ok_ptr, BOOLEAN *is_sim2_ok_ptr);

/*****************************************************************************/
// 	Description : open normal start up window
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenNormalStartUpWin(void);

/*****************************************************************************/
// 	Description : Get sim card exited status
//	Global resource dependence : 
//  Author:kelly.li
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetSimExistedStatus(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : get current handling Pin Ind sim id(response)
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetCurResponsePinSimID(void);

/*****************************************************************************/
// 	Description : get current handling sim id(operate pin)
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetCurHandlingPinSimID(void);

/*****************************************************************************/
// 	Description : set current handling sim id
//	Global resource dependence : 
//  Author:
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetCurHandlingPinSimID(MN_DUAL_SYS_E  cur_sim_id);

/*****************************************************************************/
// 	Description : open bettry capability win
//	Global resource dependence : 
//  Author: xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenBtryCapWin(void);

/*****************************************************************************/
// 	Description : Set the mpeg4 playing flag as power anim.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetPowerAnimMpeg4Flag(
                    BOOLEAN is_playing
                    );

/*****************************************************************************/
// 	Description : Get the mpeg4 playing flag as power anim.
//	Global resource dependence : 
//  Author: xingdong.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPowerAnimMpeg4Flag(
                    void
                    );

/*****************************************************************************/
// 	Description : get plmn name, cmcc, unicom or others
//	Global resource dependence : 
//  Author: Lin.Lin
//	Note:
/*****************************************************************************/
PUBLIC PHONE_PLMN_NAME_E MMIAPIPHONE_GetNetworkName(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSAttach
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSAttach(void);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSDetach
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSDetach(void);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSAttachMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSAttachMode(void);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_SetGPRSDetachEx
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetGPRSDetachEx(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : start charge timer
//	Global resource dependence : 
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_StartChargeTimer(void);

/*****************************************************************************/
// 	Description : stop 
//	Global resource dependence : 
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_StopChargeTimer(void);

/*****************************************************************************/
// 	Description : Interrogate Cfu for idle icon display
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_InterrogateCfuForIdle(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : according to ps status, judge if wap & mms is available
//	Global resource dependence : 
//  Author: samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPsServiceAvailble(void);

/*****************************************************************************/
// 	Description : 设置IDLE GPRS显示状态
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetIdleGprsState(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : is network selecting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsNetworkSelecting(MN_DUAL_SYS_E dual_sys);


/*****************************************************************************/
// 	Description :get current service status
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetCurServiceStatus(
                                            MN_DUAL_SYS_E dual_sys,
                                            PHONE_SERVICE_STATUS_T *status_ptr
                                            );

/*****************************************************************************/
// 	Description :get current service status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC PHONE_SERVICE_STATUS_T* MMIAPIPHONE_GetServiceStatus(void);

/*****************************************************************************/
// 	Description :get current data service type
//	Global resource dependence : 
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC MMIPHONE_DATA_SERVICE_TYPE_E MMIAPIPHONE_GetDataServiceType( MN_DUAL_SYS_E dual_sys );

/*****************************************************************************/
// 	Description : Open IMEI Display Win
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_OpenIMEIDisplayWin(void);

/*****************************************************************************/
// 	Description : register common module applet info
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_CommonMduRegAppletInfo(void);

/*****************************************************************************/
// 	Description : register PS/CS Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_RegisterNotifyInfo(MMIPHONE_NOTIFY_INFO_T *notify_info_ptr);

/*****************************************************************************/
// 	Description : Unregister PS/CS Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UnregisterNotifyInfo(uint32 module_id);

/*****************************************************************************/
// 	Description : Is CS Available
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsCSAvail(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Is PS Available
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsPSAvail(MN_DUAL_SYS_E dual_sys);



/*****************************************************************************/
// 	Description : handle udisk exception
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_HandleUdiskException(
                                          BOOLEAN   is_assert
                                          );

#ifndef MMI_MULTI_SIM_SYS_SINGLE

/*****************************************************************************/
// 	Description : MMIPHONE_PowerOffPsEx
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnPsEx(void);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsPsReadyBySys
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsPsReadyBySys(MN_DUAL_SYS_E sys_type);

#endif

/*****************************************************************************/
// 	Description : register SIM Plug Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_RegisterSIMPlugNotifyInfo(MMIPHONE_SIM_PLUG_NOTIFY_INFO_T *notify_info_ptr);

/*****************************************************************************/
// 	Description : Unregister SIM Plug Notify info
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UnregisterSIMPlugNotifyInfo(uint32 module_id);

/*****************************************************************************/
// 	Description : Power On SIM
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnSIM(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : Deactive PS
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_DeactivePS(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
// 	Description : Power off  PS or SIM
//	Global resource dependence : none
//  Author: chunjie.liu
//	Note:if ps is ready,power off ps,else power off sim
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PoweroffPsOrSim(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set standby mode
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetStandByMode(MN_DUAL_SYS_E dual_sys, MMIPHONE_SIM_PLUG_EVENT_E plug_event);

#if defined(MMI_UPDATE_TIME_SUPPORT)
/*****************************************************************************/
// 	Description : 当前SIM是否注册网络 成功
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsRegNetSuccess(MN_DUAL_SYS_E cur_sim_index);

/*****************************************************************************/
// 	Description : 是否所有SIM卡搜网 成功
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsAllSIMSelectNetSucc(void);

/*****************************************************************************/
// 	Description : 当前SIM是否Attach gprs 成功
//	Global resource dependence : 
//  Author: xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsAttachGprsSuccess(MN_DUAL_SYS_E cur_sim_index);

/*****************************************************************************/
// 	Description : 利用当前tick和NITZ时间设置系统时间
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetSysTimeWithNitzAndTickTime(void);

#endif /*MMI_UPDATE_TIME_SUPPORT*/

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)
/*****************************************************************************/
// 	Description : MMIAPIPHONE_OpeartorTextID
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MMI_TEXT_ID_T MMIAPIPHONE_OpeartorTextID(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : MMIAPIPHONE_GetIsNetworkRoaming
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsNetworkRoaming(void);
#endif

/*****************************************************************************/
// 	Description : Get Update NetworkStatus Win
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPIPHONE_GetUpdateNetworkStatusWin(void);

/*****************************************************************************/
// 	Description : is network selecting
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetNeedUpdateNetworkStatusWin(MMI_HANDLE_T win_handle);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: wancan.you
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_UpdateNetworkStatusWin(void);

/*****************************************************************************/
// 	Description : get event is delay or not
//	Global resource dependence : none
//  Author: 
//	Note: return TRUE: delay, FALSE:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetEventDelaied(MMI_DELAIED_EVENT_E event);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetEventDelaied(MMI_DELAIED_EVENT_E event);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_CleanEventDelaied(MMI_DELAIED_EVENT_E event);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_ResumeDelaiedEvent(MMI_DELAIED_EVENT_E event);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_ResumeAllDelaiedEvent(void);

/*****************************************************************************/
// 	Description : check event must delay
//	Global resource dependence : none
//  Author: 
//	Note: return TRUE: delay, FALSE:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsNeedDelayEvent(MMI_DELAIED_EVENT_E event);

#ifdef FLASH_SUPPORT
/*****************************************************************************/
// 	Description : 设置一个标志变量，表示正常的开机动画播放完成后没有进入IDLE，其他APP将负责启动IDLE
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetOpenIdleFlag(BOOLEAN flag);
/*****************************************************************************/
// 	Description : 查询上面函数设置的标志变量
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetOpenIdleFlag(void);
/*****************************************************************************/
// 	Description : 设置屏幕删除回调函数
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetScrDelCallBack(MMI_HANDLE_T win_id,pFunc func);
/*****************************************************************************/
// 	Description : 执行屏幕删除回调函数
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/

PUBLIC void MMIAPIPHONE_ScrDelCallBack(MMI_HANDLE_T win_id);
/*****************************************************************************/
// 	Description : 尝试进入idle
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AtemptEnterIdle(void);
/*****************************************************************************/
// 	Description : 尝试进入idle ---用于usb
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AtemptEnterIdleForUsb(void);
/*****************************************************************************/
// 	Description : 尝试进入idle ---用于防盗
//	Global resource dependence : 
//  Author: dayong.yang
//	Note:  
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AtemptEnterIdleForLock(void);

#endif

#ifdef MMI_DUAL_BATTERY_SUPPORT
/*****************************************************************************/
// 	Description : get capacity level(0-5)
//	Global resource dependence : 
//  Author: renyi.hu
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetBatLevel(uint32 level);
#endif

#ifdef MMI_SIM_LOCK_SUPPORT
/*****************************************************************************/
// 	Description : MMIPHONE_Get SIM lock Config Data Ptr
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MN_SIM_LOCK_STATUS_T *MMIAPIPHONE_GetSIMUnlockConfigDataPtr(void);

/*****************************************************************************/
// 	Description : MMIPHONE Update SIM Unlock Data if MMI update sim lock data to MN
//	Global resource dependence : none
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_UpdateSIMUnlockData(void);

/*****************************************************************************/
// 	Description : handle SIM Lock set menu Flow
//	Global resource dependence : 
//  Author:dave.ruan
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_HandleSIMLockSettingMenuFlow(
                             uint32       lock_type,   //SIM Lock Type
                             MN_DUAL_SYS_E    dual_sys,        //dual sim card reserved
                             BOOLEAN    is_enable   //enable or disable the lock Type
                             );
                             
/*****************************************************************************/
// 	Description : Get SIM lock Remain trials and timers
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_GetSIMLockRemainTrialsTimer(
                                uint32 unlock_type,      //IN: Network/NetworkSubset/SP/CORPORATE/USER
                                uint32 *unlock_trails,      //out: sim unlock trails
                                uint32  *unlock_timer       //out:sim unlock timer
                                );
#endif

#ifdef MMI_PREFER_PLMN_SUPPORT
/*****************************************************************************/
// 	Description : append plmn list,构建平台提供的所有默认网络列表.
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AppendPlmnList(
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                MMI_CTRL_ID_T ctrl_id,
                                                                                uint16 index);

/*****************************************************************************/
// 	Description : Append Plmn List，构建优选网络列表.
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_AppendPreferPlmnList(
                                                                                MN_DUAL_SYS_E dual_sys,
                                                                                MMI_CTRL_ID_T ctrl_id,
                                                                                MN_PLMN_T *prefer_network,
                                                                                uint16 index);
#endif

/*****************************************************************************/
// 	Description : get plmn list total num.
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPHONE_GetPlmnListNum(void);

/*****************************************************************************/
// 	Description : get plmn info by network_table index.
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPlmnInfoByIndex(
                                                                                MN_PLMN_T *prefer_network,
                                                                                uint32 index);

#ifdef MMI_SIM_LANGUAGE_SUPPORT
/*****************************************************************************/
// 	Description : 判断是哪张sim卡在做优选语言。
//	Global resource dependence : 
//  Author: michael.shi
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIPHONE_GetPreferLangSim(void);
#endif

#ifdef DPHONE_SUPPORT 
/*****************************************************************************/
// 	Description : Get Pin Lock Hook Handfree Status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetPinLockHookHandfreeStatus(void);

/*****************************************************************************/
// 	Description : Set Pin Lock Hook Handfree Status
//	Global resource dependence : 
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetPinLockHookHandfreeStatus(BOOLEAN hook_locked);
#endif

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
// 	Description : Set the network name from csc
//	Global resource dependence : 
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_SetNetWorkNameByCSC(MMICUS_CSC_GENERALINFO_NETWORKINFO_T *csc_network_name_ptr,
                                                                                                            uint16 network_num);

/*****************************************************************************/
// 	Description : Get the network name from csc
//	Global resource dependence : 
//  Author: minghu.mao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetNetWorkNameByCSC(uint16 mcc, 
                                                                                                            uint16 mnc,
                                                                                                            MMI_STRING_T *name_string_str);
#endif

/*****************************************************************************/
// 	Description : 获取状态正常的SIM卡总数和具体卡的位置
//	Global resource dependence : 
//  Author: dave.ruan
//	Note:   sim_idex_ptr :point to ok sim index,if null,do not set sim index
//              array_len :ok sim index len
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetAllSimStateOKCount(uint16 *sim_idex_ptr, uint16 array_len);

/*****************************************************************************/
// 	Description : is ofm(Operational Feature Monitor) actived, if it is, then the ciphering off indicator should displaying 
//    when the call connection is not encrypted
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsOfmActived(MN_DUAL_SYS_E dual_sys);

#ifdef MMI_NETWORK_NAME_SUPPORT
/*****************************************************************************/
//  Description : Check if has the same SPN String
//  Global resource dependence : none
//  Author: CBK
//  Note:res spn VS sim spn
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_CheckSPNString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *inSpn);
PUBLIC BOOLEAN MMIPHONE_CheckGIDString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_gid_string);
PUBLIC BOOLEAN MMIPHONE_CheckPNNString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_pnn_string);
PUBLIC BOOLEAN MMIPHONE_CheckIMSIString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_imsi_string);
PUBLIC BOOLEAN MMIPHONE_CheckIMSIRString(MN_DUAL_SYS_E dual_sys, MMI_STRING_T  *res_imsir_string);
#endif

#ifdef MMI_UPDATE_TIME_WHEN_STARTUP
/*****************************************************************************/
// 	Description : start timer to get NITZ time manual
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_StartTimerToGetNitzTime(void);

/*****************************************************************************/
// 	Description : start get NITZ time manual
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_StartGetNitzTime(void);

/*****************************************************************************/
// 	Description : get NV Network Name Info
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_GetNitzTime(MN_DUAL_SYS_E attaching_sim);

/*****************************************************************************/
// 	Description : get last sim card id which exist nitz
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E  MMIAPIPHONE_GetExistNitzSIM(void);

/*****************************************************************************/
// 	Description : get last sim card id which exist nitz
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_SetExistNitzSIM(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : get open update time flag
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIPHONE_IsAttachTimeFinished(void);

/*****************************************************************************/
// 	Description : set open update time flag
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_SetAttachNitzTimeFinish(BOOLEAN is_finished);

/*****************************************************************************/
// 	Description : get attaching nitz SIM index
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E  MMIAPIPHONE_GetAttachingNitzSIM(void);

/*****************************************************************************/
// 	Description : set attaching nitz SIM index
//	Global resource dependence : none
//    Author: dave.ruan
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPIPHONE_SetAttachingNitzSIM(MN_DUAL_SYS_E dual_sys);
#endif /*MMI_UPDATE_TIME_WHEN_STARTUP*/
#ifdef MMI_BATTERY_PERCENT_IN_STATUSBAR
/*****************************************************************************/
//  Description : MMIAPIPHONE_SetInitBatteryCap
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetPreBatteryCap(uint32 cap);

/*****************************************************************************/
//  Description : MMIAPIPHONE_GetPreBatteryCap
//  Global resource dependence : 
//  Author:juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPIPHONE_GetPreBatteryCap(void);
#endif

#ifdef MMI_LDN_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPHONE_CheckLdnReadyInd(void);
#endif

#if (defined MMI_SIM_LOCK_SUPPORT) && (defined MMI_RESET_PHONE_AUTO_PIN)
/*****************************************************************************/
// 	Description : Set whether use pin in nv for SIM lock
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_SetUsePinInNV(
                      BOOLEAN  use_pin_in_nv);
#endif

//启机时的事件
typedef enum
{
    MMI_STARTUP_NORMAL_INIT_ENTER,

    MMI_STARTUP_EVENT_COORDINATE_FINISH,
    MMI_STARTUP_EVENT_BOOTPWD_FINISH,
    MMI_STARTUP_EVENT_FLYMODE_FINISH,
    MMI_STARTUP_EVENT_ANIMATION_FINISH,
    MMI_STARTUP_EVENT_PINPUK_FINISH,

    MMI_STARTUP_EVENT_MAX
}MMI_STARTUP_EVENT_E;
/*****************************************************************************/
//  Description : 启机流程的控制函数
//  Parameter: [In]     event: 当前完成的事件
//             [Out]    None
//             [Return] None
//  Author: liheng.wei
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPI_StartupCtrl( MMI_STARTUP_EVENT_E event );

/*****************************************************************************/
//  Description : MMIPHONE_SetPinPukState
//  Parameter: [In] pinpuk_state
//             [Out] None
//             [Return] None
//  Author: jichuan.zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPHONE_SetPinPukState(MMIPHONE_PIN_PUK_STATE_TYPE_E pinpuk_state);

/*****************************************************************************/
//  Description : get pin or puk ready status
//  Parameter: [In] None
//             [Out] None
//             [Return] is_needPinPuk : TRUE(input pin or puk win);FALSE(not input pin or puk win)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_IsNeedPinPuk( void );

/*****************************************************************************/
//  Description : enter pin or puk win
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPHONE_OpenPinPukWin( void );

//=================================VOLTE START===============================//

/*****************************************************************************/
//  Description : get Volte status
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return]TRUE :Open,FALSE :Close;
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetVolteStatus(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Reset Ims status
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_ResetVolteInfo( void );

/*****************************************************************************/
//  Description : get volte info
//  Parameter: [In] None
//             [Out] None
//             [Return] 0:Open,1:Close;  Sim1:Bit0,Sim2:Bit1
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIPHONE_GetVolteInfo(void);

/*****************************************************************************/
//  Description : set volte info
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] volte_on:the status of volte
//             [Return] None
//  Author: feiyue.ji
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPHONE_SetVolteInfoNotToNV(MN_DUAL_SYS_E dual_sys,BOOLEAN volte_on);

/*****************************************************************************/
//  Description : set volte info
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] volte_on:the status of volte
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPHONE_SetVolteInfo(MN_DUAL_SYS_E dual_sys,BOOLEAN volte_on);

/*****************************************************************************/
//  Description : Get volte icon display style
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] volte icon display style
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMIPHONE_VOLTE_ICON_STYLE_E MMIAPIPHONE_GetVolteIconDisplayStyle(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get volte icon display status
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] volte icon display status
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsVolteIconDisplay(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get volte set display status
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] volte menu display status
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_IsVolteSetDisplay(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : Get volte set MMIAPIPHONE_VolteFactoryReset status
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: mingwei.jia
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_VolteFactoryReset();

//===================================VOLTE END===============================//

/*****************************************************************************/
//  Description : power on ps
//  Parameter: [In] dual_sys: the value of the sim card
//             [Out] None
//             [Return] None
//  Author: yanli.yang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnPsByUsim(MN_DUAL_SYS_E dual_sys);
/*****************************************************************************/
//    Description : get is sim power on 
//    Global resource dependence : 
//    [In] dual_sys sim id
//    [Out] None
//    [Return] is sim card already power on :TRUE, power on;FALSE, not power on
//    Author: yonghua.zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPHONE_GetIsSimPowerOn(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//    Description : power on Modem, increase counter s_poweron_modem_counter
//    Global resource dependence : 
//    [In] None
//    [Out] None
//    [Return] None
//    Author: yonghua.zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOnModem( void );

/*****************************************************************************/
//    Description : power off modem , decrease counter s_poweron_modem_counter,
//                  modem is power off only when counter is <= 0
//    Global resource dependence :
//    [In] None
//    [Out] None
//    [Return] None
//    Author: yonghua.zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_PowerOffModem( void );

#ifdef SIM_PLUG_IN_SUPPORT_LOGIC
/*****************************************************************************/
//Description : 轮询是否有SIM热插拔
//Global resource dependence :
//Author:
//Note:
/*****************************************************************************/
PUBLIC void MMIAPIPHONE_QuerySimisPlugIn(void);
#endif
PUBLIC void MMIAPIPHONE_SyncSimSPN(MN_DUAL_SYS_E dual_sys);
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define 	MMIPHONE_NormalInit	   		        MMIAPIPHONE_NormalInit
#define 	MMIPHONE_NormalInitAfterPwd   	MMIAPIPHONE_NormalInitAfterPwd
#define 	MMIPHONE_AlarmInit			        MMIAPIPHONE_AlarmInit
#define 	MMIPHONE_ChargeInit			        MMIAPIPHONE_ChargeInit
#define 	MMIPHONE_AssertInit			        MMIAPIPHONE_AssertInit
#define 	MMIPHONE_AppInit		                MMIAPIPHONE_AppInit
//#define 	MMIPHONE_PowerOff		                MMIAPIPHONE_PowerOffEx
#define 	MMIPHONE_GetBatCapacity			MMIAPIPHONE_GetBatCapacity
#define 	MMI_HandlePinAndPukFlow		        MMIAPIPHONE_HandlePinAndPukFlow
#define 	MMIPHONE_IsSameSim		        MMIAPIPHONE_IsSameSim
#define 	MMIPHONE_IsPermitSetPin1		MMIAPIPHONE_IsPermitSetPin1
#define 	MMIPHONE_GetPermitSetPin1Num		MMIAPIPHONE_GetPermitSetPin1Num
#define 	MMIPHONE_GetNetworkName		MMIAPIPHONE_GetNetworkName
#define 	MMIPHONE_GetNetWorkNameId		MMIAPIPHONE_GetNetWorkNameId
#define 	MMIPHONE_GetFullPlmn		        MMIAPIPHONE_GetFullPlmn
#define 	MMIPHONE_IsSimAvailable			MMIAPIPHONE_IsSimAvailable
#define 	MMIPHONE_IsSimCardOk		        MMIAPIPHONE_IsSimCardOk
#define 	MMIPHONE_GetStartUpCondition		MMIAPIPHONE_GetStartUpCondition
#define 	MMIPHONE_SetStartUpCondition		MMIAPIPHONE_SetStartUpCondition
#define 	MMIPHONE_GetSimAndServiceString		MMIAPIPHONE_GetSimAndServiceString
#define 	MMIPHONE_GetSimAvailableNum		MMIAPIPHONE_GetSimAvailableNum
#define 	MMIPHONE_GetSimOkNum		        MMIAPIPHONE_GetSimOkNum
#define 	MMIPHONE_GetSimCardOkNum		MMIAPIPHONE_GetSimCardOkNum
#define 	MMIPHONE_GetSimExistedNum		MMIAPIPHONE_GetSimExistedNum
#define 	MMIPHONE_GetPSReadyNum		MMIAPIPHONE_GetPSReadyNum
#define 	MMIPHONE_GetSysOkNum		        MMIAPIPHONE_GetSysOkNum
#define 	MMIPHONE_GetNationalRoamingNum		MMIAPIPHONE_GetNationalRoamingNum
#define 	MMIPHONE_AlertSimNorOKStatus		MMIAPIPHONE_AlertSimNorOKStatus
#define 	MMIPHONE_AlertSimNorOKStatusII		MMIAPIPHONE_AlertSimNorOKStatusII
#define 	MMIPHONE_IsPin2RequiredACM		MMIAPIPHONE_IsPin2RequiredACM
#define 	MMIPHONE_IsPin2RequiredACMMax		MMIAPIPHONE_IsPin2RequiredACMMax
#define 	MMIPHONE_IsPin2RequiredPuct			MMIAPIPHONE_IsPin2RequiredPuct
#define 	MMIPHONE_IsAocSupport		        MMIAPIPHONE_IsAocSupport
#define 	MMIPHONE_GetPlmnStatus		        MMIAPIPHONE_GetPlmnStatus
#define 	MMIPHONE_GetSimStatus		        MMIAPIPHONE_GetSimStatus
#define 	MMIPHONE_OpenSelectSimWin			MMIAPIPHONE_OpenSelectSimWin
#define 	MMIPHONE_NormalInitAfterCoordinate	MMIAPIPHONE_NormalInitAfterCoordinate
#define 	MMIPHONE_GetPowerOnStatus		MMIAPIPHONE_GetPowerOnStatus
#define 	MMIAPIInit_TDOrGsm		                MMIAPIPHONE_InitTDOrGsm
#define 	MMIAPISET_TDOrGsm			        MMIAPIPHONE_TDOrGsm
#define 	MMIAPIGET_TDOrGsm		                MMIAPIPHONE_GetTDOrGsm
#define 	MMIAPISTOPST_TDOrGsm		        MMIAPIPHONE_StopSTTDOrGsm
#define 	MMI_VirtualWinInit		                        MMIAPIPHONE_VirtualWinInit
#define 	MMIPHONE_GetCurPLMN			MMIAPIPHONE_GetCurPLMN
#define 	MMIPHONE_GetSimHPLMN		        MMIAPIPHONE_GetSimHPLMN
#define 	MMIPHONE_GenPLMNDisplay		MMIAPIPHONE_GenPLMNDisplay
#define 	MMIPHONE_SetSimExistedStatus		MMIAPIPHONE_SetSimExistedStatus
#define 	MMIPHONE_StartUpPsAccordingToSetting		MMIAPIPHONE_StartUpPsAccordingToSetting
#define 	MMIPHONE_DeactivePsAccordingToSetting		MMIAPIPHONE_DeactivePsAccordingToSetting
#define 	MMIPHONE_SetIsPsDeactiveCnf		MMIAPIPHONE_SetIsPsDeactiveCnf
#define 	MMIPHONE_GetIsPsDeactiveCnf		MMIAPIPHONE_GetIsPsDeactiveCnf
#define 	MMIPHONE_SetIsPsReady			MMIAPIPHONE_SetIsPsReady
#define 	MMIPHONE_GetIsPsReady		        MMIAPIPHONE_GetIsPsReady
#define 	MMIPHONE_PowerOnPs		        MMIAPIPHONE_PowerOnPs
#define 	MMIPHONE_PowerOffPs		        MMIAPIPHONE_PowerOffPs
#define 	MMIPHONE_GetDualSimStatus			MMIAPIPHONE_GetDualSimStatus
#define 	MMIPHONE_OpenNormalStartUpWin		MMIAPIPHONE_OpenNormalStartUpWin
#define 	MMIPHONE_GetSimExistedStatus		MMIAPIPHONE_GetSimExistedStatus
#define 	MMIPHONE_GetCurResponsePinSimID		MMIAPIPHONE_GetCurResponsePinSimID
#define 	MMIPHONE_GetCurHandlingPinSimID		MMIAPIPHONE_GetCurHandlingPinSimID
#define 	MMIPHONE_SetCurHandlingPinSimID		MMIAPIPHONE_SetCurHandlingPinSimID
#define 	MMIPHONE_OpenBtryCapWin		MMIAPIPHONE_OpenBtryCapWin
#define 	MMIPHONE_SetPowerAnimMpeg4Flag		MMIAPIPHONE_SetPowerAnimMpeg4Flag
#define 	MMIPHONE_GetPowerAnimMpeg4Flag		MMIAPIPHONE_GetPowerAnimMpeg4Flag
#define 	MMIPHONE_GetNetworkName		MMIAPIPHONE_GetNetworkName
#define 	MMIPHONE_SetGPRSAttach		        MMIAPIPHONE_SetGPRSAttach
#define 	MMIPHONE_SetGPRSDetach		        MMIAPIPHONE_SetGPRSDetach
#define 	MMIPHONE_SetGPRSAttachMode			MMIAPIPHONE_SetGPRSAttachMode
#define 	MMIPHONE_SetGPRSDetachEx		MMIAPIPHONE_SetGPRSDetachEx
#define 	MMIPHONE_StartChargeTimer		MMIAPIPHONE_StartChargeTimer
#define 	MMIPHONE_StopChargeTimer		MMIAPIPHONE_StopChargeTimer
#define 	MMIPHONE_InterrogateCfuForIdle		MMIAPIPHONE_InterrogateCfuForIdle
#define 	MMIPHONE_IsPsServiceAvailble		MMIAPIPHONE_IsPsServiceAvailble
#define 	MMIPHONE_SetIdleGprsState		MMIAPIPHONE_SetIdleGprsState
#define 	MMIPHONE_IsNetworkSelecting		MMIAPIPHONE_IsNetworkSelecting
#define 	MMIPHONE_GetCurServiceStatus			MMIAPIPHONE_GetCurServiceStatus
#define 	MMIPHONE_GetDataServiceType		MMIAPIPHONE_GetDataServiceType
#define 	MMICOMMON_RegAppletInfo		MMIAPIPHONE_CommonMduRegAppletInfo
#define 	MMIPHONE_RegisterNotifyInfo		MMIAPIPHONE_RegisterNotifyInfo
#define 	MMIPHONE_UnregisterNotifyInfo			MMIAPIPHONE_UnregisterNotifyInfo
#define 	MMIPHONE_IsCSAvail		                MMIAPIPHONE_IsCSAvail
#define 	MMIPHONE_IsPSAvail		                MMIAPIPHONE_IsPSAvail
#define 	MMIPHONE_HandleUdiskException	MMIAPIPHONE_HandleUdiskException
#ifndef MMI_MULTI_SIM_SYS_SINGLE
#define 	MMIPHONE_PowerOnPsEx			MMIAPIPHONE_PowerOnPsEx
#define 	MMIPHONE_GetIsPsReadyBySys		MMIAPIPHONE_GetIsPsReadyBySys
#endif
#define 	MMIPHONE_RegisterSIMPlugNotifyInfo		MMIAPIPHONE_RegisterSIMPlugNotifyInfo
#define 	MMIPHONE_UnregisterSIMPlugNotifyInfo		MMIAPIPHONE_UnregisterSIMPlugNotifyInfo
#define 	MMIPHONE_PowerOnSIM			MMIAPIPHONE_PowerOnSIM
#define 	MMIPHONE_DeactivePS		                MMIAPIPHONE_DeactivePS
#define 	MMIPHONE_SetStandByMode		MMIAPIPHONE_SetStandByMode
#define 	MMIPHONE_OpeartorTextID			MMIAPIPHONE_OpeartorTextID
#define 	MMIPHONE_GetIsNetworkRoaming		MMIAPIPHONE_GetIsNetworkRoaming
#ifdef FLASH_SUPPORT
#define 	MMIPHONE_SetOpenIdleFlag		MMIAPIPHONE_SetOpenIdleFlag
#define 	MMIPHONE_GetOpenIdleFlag		MMIAPIPHONE_GetOpenIdleFlag
#define 	MMIPHONE_SetScrDelCallBack		MMIAPIPHONE_SetScrDelCallBack
#define 	MMIPHONE_ScrDelCallBack		        MMIAPIPHONE_ScrDelCallBack
#define 	MMIPHONE_AtemptEnterIdle		MMIAPIPHONE_AtemptEnterIdle
#define 	MMIPHONE_AtemptEnterIdle_for_usb		MMIAPIPHONE_AtemptEnterIdleForUsb
#define 	MMIPHONE_AtemptEnterIdle_for_lock	MMIAPIPHONE_AtemptEnterIdleForLock
#endif
#ifdef MMI_DUAL_BATTERY_SUPPORT
#define 	MMIPHONE_GetBatLevel		        MMIAPIPHONE_GetBatLevel
#endif
#ifdef MMI_SIM_LOCK_SUPPORT
#define 	MMIPHONE_GetSIMUnlockCinfigDataPtr		MMIAPIPHONE_GetSIMUnlockConfigDataPtr
#define 	MMIPHONE_UpdateSIMUnlockData		        MMIAPIPHONE_UpdateSIMUnlockData
#define 	MMI_HandleSIMLockSettingMenuFlow			MMIAPIPHONE_HandleSIMLockSettingMenuFlow
#define 	MMIPHONE_GetSIMLockRemainTrialsTimer		MMIAPIPHONE_GetSIMLockRemainTrialsTimer
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
