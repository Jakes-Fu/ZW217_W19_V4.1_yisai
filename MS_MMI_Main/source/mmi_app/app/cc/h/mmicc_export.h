/****************************************************************************
** File Name:      mmicc_export.h                                                 *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the message               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003        figo.feng         Create
** 
****************************************************************************/
#ifndef  _MMICC_H_    
#define  _MMICC_H_   

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmibt_export.h" // baokun add
#include "mmipb_export.h" // baokun add
#include "mn_type.h"
#include "mmicc_handfree.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#ifdef CSC_XML_SUPPORT
#include "mmicus_export.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#ifdef VT_SUPPORT
#define MMIVT_FALBACK_WAITING_TIMEOUT 15000	//@samboo.shen vt回落等待15秒

#endif

#define CC_MAX_CALL_NUM      				7		//最多同时存在的CALL的数量
#define CC_MAX_PBNAME_LEN MMIPB_MAX_STRING_LEN //最大电话号码名字长读
#define CC_MAX_TELE_NUM_LEN (MMIPB_BCD_NUMBER_MAX_LEN << 1) //最大电话号码的长度
#define CC_DISPLAY_TIME_LEN					9		//显示通话时间字串的长度
#define CC_INVALID_INDEX					255		//非法索引

#define CC_MO_CALL_TYPE                     0
#define CC_MT_CALL_TYPE                     1

#define CC_CALL_NORMAL_CALL    0x00
#define CC_CALL_IP_CALL             0x01
#define CC_CALL_VIDEO_CALL       0x02
#define CC_CALL_STK_CALL           0x04

//lzk mark
//lzk mark
#ifdef BT_DIALER_SUPPORT
#define CC_CALL_BT_CALL        0x05
#define CC_CALL_DEFAULT_CALL   0x06
#else
#define CC_CALL_DEFAULT_CALL   0x05
#endif

#define CC_CALL_EMERGENCY_CALL   "112"

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    CC_RESULT_SUCCESS,	//函数调用成功
    CC_RESULT_ERROR		//函数调用错误
}CC_RESULT_E;			//CC函数的返回结果

typedef enum
{
    CC_SIM1_CALL,  //SIM1呼出
    CC_SIM2_CALL,  //SIM2呼出
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    CC_SIM3_CALL,  //SIM3呼出
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    CC_SIM4_CALL,  //SIM4呼出
#endif
#endif
    CC_MAJOR_SIM_CALL,  //主卡呼出
    CC_SECONDARY_SIM_CALL,//副卡呼出
    CC_CALL_SIM_MAX//不指定SIM卡呼出
}CC_CALL_SIM_TYPE_E;

typedef enum
{
    CC_RELEASE_ALL_NORMAL,      //关闭call的窗口，但不关闭其他窗口，显示释放提示及通话时间
    CC_RELEASE_ALL_PROMPT,      //退回idle，显示释放提示及通话时间
    CC_RELEASE_ALL_NOPROMPT,	//关闭call的窗口，但不关闭其他窗口，不显示释放提示及通话时间
    CC_RELEASE_ALL_TYPE_MAX
}CC_RELEASE_ALL_TYPE_E;

typedef enum
{
    CC_NULL_STATE,          //空闲状态
    CC_INIT_STATE,			//初始状态
    CC_CALLING_STATE,		//呼叫状态
    CC_CALL_STATE,			//通话状态
    CC_INCOMING_CALL_STATE, //来电状态
    CC_HOLD_STATE,			//保持状态
    CC_WAITING_STATE,
    CC_CALL_STATE_MAX
} CC_CALL_STATE_E;			//每通CALL的状态

//CRSS处理优先级:如果同时存在waiting call和hold call，则优先处理waiting call，一个CRSS不会同时处理两个类型的电话
typedef enum
{
    CC_CRSS_RELEASE_DEACTIVE,   //挂断等待中或者保持电话
    CC_CRSS_RELEASE_ACTIVE_AND_ANSWER,       //挂断已激活电话并接通等待中或者保持电话
    CC_CRSS_HOLD_ACTIVE_AND_ANSWER, //保持所有已激活电话，接听保持或者等待中电话
    CC_CRSS_ADD_HOLD_CALL_TO_MPTY, //将保持电话加入到电话会议中
    CC_CRSS_ECT,//进行ECT切换
    CC_CRSS_RESERVED2,
    CC_CRSS_MAX
} CC_CALL_CRSS_E;

typedef struct
{
    MN_DUAL_SYS_E   dual_sys;				//标示当前的使用SIM卡ID
    uint32	start_time;	//开始通话时间
    uint32	stop_time;	//结束通话时间
}CC_CALL_TIME_T;		//通话时间

//@xiongxiaoyan add 20060406 begin
typedef enum
{
    CC_CALL_NORMAL,
    CC_CALL_VIDEO,
    CC_CALL_MODE_MAX
} CC_CALL_MODE_E;
//@xiongxiaoyan add 20060406 end

typedef enum
{
    PHOTO_TYPE_CURRENT, //当前电话大头贴
    PHOTO_TYPE_ACTIVE, //激活电话大头贴
    PHOTO_TYPE_HOLD, //保持电话大头贴
#ifndef MMI_GUI_STYLE_TYPICAL
    PHOTO_TYPE_OTHER, //挂断电话电话大头贴
#endif
    PHOTO_TYPE_MAX
}CC_PHOTO_TYPE_E;


typedef struct
{
	BOOLEAN			exist_incoming_call;	//是否存在INCOMING的CALL
	BOOLEAN			exist_outgoing_call;	//是否存在准备呼出的CALL
	BOOLEAN			exist_mpty;				//是否存在MPTY
	uint8			exist_active_call_num;	//存在ACTIVE的CALL的数量
	uint8			exist_hold_call_num;	//存在HOLD的CALL的数量
	BOOLEAN exist_mtfw;
}CC_EXIST_CALL_T;							//当前存在的所有CALL的状态

typedef enum
{
    OTHER_OPERATE,					//其它操作
    RELEASE_ALL_ACTIVE_AND_ACCEPT,	//释放所有ACTIVE的CALL并接来电
    HOLD_ALL_ACTIVE_AND_ACCEPT,		//HOLD所有ACTIVE的CALL并接来电
    BUILD_MPTY_AND_ACCEPT,
    RELEASE_ALL_CALL_AND_MAKE_CALL		//Release all call and make a new call
}CC_OPERATE_WAITING_CALL_E;			//用户对来电的操作

typedef enum
{
    CRSS_OTHER_OPERATE,	
    RELEASE_ALL_ACTIVE_AND_ACCEPT_WAIT,//挂断已active的call并接听来电
    RELEASE_ALL_ACTIVE_AND_RETRIEVE_HOLD,//挂断已active的call并恢复hold的call
}CC_OPERATE_CRSS_CODE_E;			//用户对来电的操作

typedef struct
{
	BOOLEAN   is_remote_mpty;		//被对方加入一个MPTY
	BOOLEAN   is_remote_split;		//被对方SPLIT
	BOOLEAN   is_remote_hold;		//被对方HOLD
}CC_OPERATED_BY_REMOTE_T;			//被对方的操作

typedef enum
{
    CC_NO_OPER,
    CC_HOLD_OPER,		//hold the call
    CC_RETRIEVE_OPER,
    CC_SHUTTLE_OPER,
    CC_BUILDMPTY_OPER,	//build mpty
    CC_SPLIT_OPER,		//split call
    CC_CONNECT_OPER,	//connect call
    CC_RELEASE_OPER
}CC_OPERATED_BY_USER_E;

typedef struct
{
    MN_DUAL_SYS_E dual_sys;		//标示当前的使用SIM卡ID，当前处于呼叫状态的那张SIM
#ifdef MMI_CSP_SUPPORT
    uint8 line;  //the call from line1 or line2
#endif   
    uint8 call_id;				//本通CALL的ID
    BOOLEAN is_mtfw_reject;			//本通CALL是否正被mtfw中断
    BOOLEAN is_in_mpty;				//本通CALL是否在一个MPTY中
    uint8 call_type;

    wchar name[CC_MAX_PBNAME_LEN + 1];//本通CALL相应的名字
    uint8 name_len;
    BOOLEAN is_save_name;                   //从ADN/FDN/SDN发起的通话，不需要去匹配PB中的记录。this is a call from ADN/FDN/SDN list,search from pb is not needed
    BOOLEAN is_name_exist;			//TRUE:名字不为空,FALSE:名字为空，name中存储号码
    BOOLEAN is_stk_call;

    CC_OPERATED_BY_REMOTE_T	operated_by_remote;		//被对方的操作
    CC_CALL_STATE_E call_state;				//本通CALL的状态
    CC_OPERATED_BY_USER_E operated_by_user;
    BOOLEAN is_release_all;			//是否是挂断所有电话
    CC_RELEASE_ALL_TYPE_E release_all_type;		//挂断所有电话的原因
    MN_CALLED_NUMBER_T address;				//号码的信息
    CC_CALL_TIME_T call_time;				//通话起止时间
    uint32 start_call_time_count;  //起始的定时器时间
    MN_CALLED_NUMBER_T call_modified_address;//Control修改号码
    BOOLEAN is_mute; //现在一通电话就有一个静音状态
    BOOLEAN is_dtmf_sent;
    BOOLEAN is_call_modified_store;
    BOOLEAN is_unciphering_call; //通话是否是未加密的
}CC_CALL_INFO_T; //每通CALL的信息

typedef struct
{
    MN_DUAL_SYS_E dual_sys;			//标示当前的使用那张SIM呼出
#ifdef MMI_CSP_SUPPORT
    uint8 line;  //the call from line1 or line2
#endif    
    MN_CALLED_NUMBER_T address;						//号码的信息
    wchar name[CC_MAX_PBNAME_LEN + 1];	//本通CALL相应的名字
    uint16 name_len;
    wchar specified_str[CC_MAX_PBNAME_LEN + 1];	//本通CALL相应的名字
    uint8 specified_str_len;
    BOOLEAN is_want_mo_call;				//是否有想要呼出的CALL
    BOOLEAN is_save_name;                   //从ADN/FDN/SDN发起的通话，不需要去匹配PB中的记录。this is a call from ADN/FDN/SDN list,search from pb is not needed
    BOOLEAN is_name_exist;                  //TRUE:名字不为空,FALSE:名字为空，name中存储号码。True means there is a name in name[] field. otherwise. there is a number replaced.
    BOOLEAN is_exist_specified_str;
    uint32 call_type;
    BOOLEAN clir_present;
    uint8 clir_type;
    BOOLEAN is_sub_addr_present;
    MN_SUBADDR_T sub_addr;
}CC_WANT_MO_CALL_T; //想要呼出的CALL的信息，用于通话中拨新号码


typedef struct
{
    BOOLEAN is_dtmf;	//if there is dtmf in calling number string
    uint8 dtmf_len;	//dtmf string len
    uint8 dtmf_str[CC_MAX_TELE_NUM_LEN]; //用来储存分机号码
    uint8 allow_send_flag; //SPEC规定，两种情况MMI可以发送dtmf音（第二种情况“在收到APP_MN_SYNC_IND和APP_MN_PROGRESS_IND消息之后”）
}CC_DTMF_STATE_T;//DTMF的状态

#ifdef MMI_VOLTE_SUPPORT
typedef struct
{
    BOOLEAN is_ps_call;//TRUE current call is volte call
    BOOLEAN is_volte_codec_on;//TRUE volte codec is on
}CC_PSCALL_INFO_T;
#endif

typedef struct
{
    MN_DUAL_SYS_E  dual_sys;				//MO时表示哪张卡呼出，MT时标示哪张卡正在被呼叫

    //uint8						    count_time_id;
    uint8   						call_number;			//当前CALL的总数,1-7
    uint8   						current_call;			//当前显示的CALL,0-6

    uint8                           incoming_call_index;
    //BOOLEAN                         need_dtmf;  //电话处于通话中该值为TRUE，可以发DTMF
    //BOOLEAN                   is_dtmf_enable; //通话之后用户可以设置,电话状态改变之后重置为TRUE,但是当前电话为多方通话时，菜单项不能设置
    uint32							missed_call_num;		//未接来电的数量
    uint32							call_time_count;		//用来记录定时器循环的次数
    CC_DTMF_STATE_T					dtmf_state;				//DTMF的状态
    CC_EXIST_CALL_T   				exist_call;				//当前存在的所有CALL的状态
    CC_OPERATE_WAITING_CALL_E		operate_waiting_call;	//用户对来电的操作
    CC_OPERATE_CRSS_CODE_E		operate_CRSS_code;			//CRSS 操作
    CC_WANT_MO_CALL_T				want_mo_call;			//想要呼出的CALL的信息，用于通话中拨新号码
    //CC_AUDIO_STATE_T				audio_state;			//声音信道的状态
    CC_CALL_INFO_T   				call_info[CC_MAX_CALL_NUM];//每通CALL的信息
#ifdef MMI_VOLTE_SUPPORT    
    CC_PSCALL_INFO_T             pscall_info;
#endif
}CC_CALL_CONTEXT_T;//当前CC模块的总体情况

typedef enum
{
    CC_CALL_CALLING_WIN,
    CC_CALL_CONNECT_WIN,
    CC_CALL_DISCONNECT_WIN,
    CC_CALL_WIN_MAX
}CC_CALL_WIN_E;

typedef enum
{
    CC_IN_CALL_STATE,
    CC_MO_CONNECTING_STATE,
    CC_MT_CONNECTING_STATE,
    CC_CALL_CONNECTED_STATE,
    CC_CALL_DISCONNECTED_STATE,
    CC_CALL_STATE_FOR_USING_MAX
}CC_CALL_STATE_FOR_USING_E;

typedef enum
{
    //MMICC_AUDIO_OPER_INIT,          //通话建立时启动初始声音设备
    //MMICC_AUDIO_OPER_CLOSE,         //通话结束后操作声音设备
    MMICC_AUDIO_OPER_ENABLE,        //启动某种声音设备模式
    MMICC_AUDIO_OPER_DISABLE,       //关闭某种声音设备模式
    //MMICC_AUDIO_OPER_ENABLE_ASYNC,  //异步设备模式声音链路建立完成后，进行操作
    //MMICC_AUDIO_OPER_DISABLE_ASYNC, //异步设备模式声音链路断开完成后，进行操作
    //MMICC_AUDIO_OPER_ASYNC_FAIL,    //异步设备声音链路操作失败，进行后续操作
    //MMICC_AUDIO_OPER_RESUME,        //恢复当前状态值恢复声音设备
    MMICC_AUDIO_OPER_MAX            //系统保留，不可使用
}MMICC_AUDIO_OPER_E;

typedef enum
{
    MMICC_AUDIO_DEV_HH,	        //听筒
    MMICC_AUDIO_DEV_EP,         //耳机
    MMICC_AUDIO_DEV_HF,         //免提
    MMICC_AUDIO_DEV_BT = 0x11,	//蓝牙 异步设备模式 
    MMICC_AUDIO_DEV_MAX 
}MMICC_AUDIO_DEV_E;

typedef enum
{
    CC_MO_ERROR_NONE,
    CC_MO_DIAL_FAILED,          //拨号失败
    CC_MO_EMPTY_NUM,            //号码为空
    CC_MO_IN_FLY_MODE,          //先关闭飞行模式
    CC_MO_IS_BARRING_NUM,       //不能呼叫该号码
    CC_MO_NET_UNAVAIL,          //网络不可用
    CC_MO_INVALID_NUM,          //无效号码
    CC_MO_NO_SIM,               //无SIM卡
    CC_MO_OPER_FAIL,            //操作失败
    CC_MO_FDN_ONLY,             //仅限固定拨号
    CC_MO_NO_IP_NUM,          //未设置IP号码
    CC_MO_SIM_CORRUPT,        //使用另一张卡呼叫
    CC_MO_MERGR_CALL_FIRST,   //需要先建立MPTY
    CC_MO_CMD_NO_SUPPORT,     //指令不支持
    CC_MO_FORBID_IN_VT,       //视频通过中不能呼出
    CC_MO_NO_VOLTE_CALL,      //不是volte call
    CC_MO_ECC_CALL,           //不支持紧急呼叫
    CC_MO_ERROR_TYPE_MAX
}MMICC_MO_CALL_ERROR_TYPE_E; //call error type

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init the cc application
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMIAPICC_Init(void);

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SendDtmf( uint32 msg_id);


/*****************************************************************************/
//  Description : check is in cc is on top
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsCcOnTopWin(void);

/*****************************************************************************/
//  Description : check is in call state, include incoming and outgoing,connected
//  Global resource dependence : none
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsInState(CC_CALL_STATE_FOR_USING_E call_state);

/*****************************************************************************/
// 	Description : Release all call
//	Global resource dependence : g_call_context
//  Author:jibin
//	Note:2004.10.14
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_ReleaseAllCallExt(
                                 CC_RELEASE_ALL_TYPE_E release_all_type
                                 );

/*****************************************************************************/
// 	Description : get the ascii from msg id
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_MsgidToAscii(
                             MMI_MESSAGE_ID_E msg_id
                             );

/*****************************************************************************/
// 	Description : open adjust volume win
//	Global resource dependence : 
//  Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_OpenAdjustVolumeWin(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
// 	Description : give a hint to user that there is a missed call
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMIAPICC_ShowMCHint(void);

/*****************************************************************************/
//  Description : clear missed call number
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ClearMissedCallNum(void);

/*****************************************************************************/
//  Description : reg cc, cl menu group and nv group
//  Global resource dependence : none
//  Author: bruce.chi
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_InitModule(void);


/*****************************************************************************/
//  Discription: the blue tooth headset cancel the call including(incoming and outgoing call)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPICC_BtCancelCall(void);

/*****************************************************************************/
//  Discription: the blue tooth headset  accept the incoming call
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPPICC_BtAcceptCall(void);

/*****************************************************************************/
//  Description : get current exited call count
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetExitedCallCount(void);

/*****************************************************************************/
//  Discription: get the message ID_STATUS_HFAG_HFU_HOLD
//  Global resource dependence: none 
//  Author: kelly.li 
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPICC_HandleBtHfuHoldInd(BT_MSG_T *msg_body_ptr );

#if defined(MMI_SPEEDDIAL_SUPPORT)
/*****************************************************************************/
//	Description : open the window for speeddial List
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void  MMIAPICC_OpenSetSpeedDial(MNBCD *number_ptr, uint8 number_len, uint8 npi_ton);

/*****************************************************************************/
//	Description : open the window for speeddial
//	Global resource dependence : 
//	Author:bin.ji
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SpeedDial(
                  MN_DUAL_SYS_E    dual_sys,
                  uint16 key_num
                  );
#endif

/*****************************************************************************/
// 	Description : whether the current mode is hand free
//	Global resource dependence : 
//  Author:gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsHandFree(void);

/*****************************************************************************/
// 	Description : decide whether send dtmf or stop dtmf
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_SendDtmfByChar(MN_DUAL_SYS_E dual_sys, uint8 dtmf_ch);

/*****************************************************************************/
// 	Description : stop dtmf
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_StopDtmf(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:jibin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsVideoCall(void);

/*****************************************************************************/
//  Description : only dial 112 when locked
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_OpenEmergencyWin(MMI_MESSAGE_ID_E msg_id);

//@zhaohui add
/*****************************************************************************/
// 	Description : to get the name of call
//	Global resource dependence : 
//  Author:zhaohui
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCallName( 
//					   uint8		call_id,//暂时屏蔽，不考虑vt多方通话
					   MMI_STRING_T	*name_str_ptr//OUT
						);

/*****************************************************************************/
// 	Description : 获取当前通话时间信息
//	Global resource dependence : 
//  Author:xiongxiaoyan
//	Note:获取当前通话时间信息
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCallTimeStr(MMI_STRING_T *prompt_str);


/*****************************************************************************/
// 	Description : get MMICC_DISCONNECT_WIN_ID
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:2007.6.11
/*****************************************************************************/
PUBLIC void MMIAPICC_CloseDisconnectWin(void);

/*****************************************************************************/
// 	Description : start ring or vibrate
//	Global resource dependence : 
//  Author:bin.ji
//	Note:2004.9.14
/*****************************************************************************/
PUBLIC MMISET_CALL_RING_TYPE_E MMIAPICC_StartRingOrVibrate(BOOLEAN is_vt_call);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_StopRingOrVibrate(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_ConnectCallReq(void);
/*****************************************************************************/
// 	Description : 挂断当前答录电话
//	Global resource dependence : 
//  Author:koki gao
//	Note: 
/*****************************************************************************/
#ifdef ASP_SUPPORT
PUBLIC CC_RESULT_E MMIAPICC_ReleaseAspCallReq(MN_CALL_DISCONNECT_CALL_CAUSE_E cause);
#endif
/*****************************************************************************/
// 	Description : 挂断当前电话
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseCurrentCallReq(MN_CALL_DISCONNECT_CALL_CAUSE_E cause);


/*****************************************************************************/
// 	Description : 获取当前呼叫的类型
//	Global resource dependence : 
//  Author:xiongxiaoyan 
//	Note:20060404
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_GetCallDualSys(void);

/*****************************************************************************/
// 	Description : 打开/关闭免提模式
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_EnableHandfreeMode(BOOLEAN is_turnon);

/*****************************************************************************/
// 	Description : If ip number is included in telephone number, remove it
//	Global resource dependence : s_call_context
//  Author:lin.lin
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_RemoveIpNumber(
                                    uint8*  tele_num_ptr, 
                                    uint8   tele_len, 
                                    uint8*  origin_number_ptr,
                                    uint8   origin_number_len
                                    );

/*****************************************************************************/
// 	Description : get sim status when get call ready status msg
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_GetSimCallReady(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set sim status when switch card
//	Global resource dependence : 
//  Author:bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SetSimCallReady(MN_DUAL_SYS_E dual_sys, BOOLEAN is_ready);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: louis.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICC_PostMsgToDetachCnfWaitWin(void);

#ifdef MMI_INDIAN_SOS_SUPPORT
/*****************************************************************************/
// 	Description : Make MMICC_MakeSosCall
//	Global resource dependence : 
//  Author:bean.yu
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_MakeSosCall(void* param , MMISET_SOS_CALLBACK_PFUNC callback);
#endif

/*****************************************************************************/
// 	Description : Make Call Interface
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_MakeCall(
                                                                    MN_DUAL_SYS_E dual_sys,//current sim
                                                                    uint8 *tele_num,//phone number
                                                                    uint8 tele_len,//phone number length
                                                                    MMI_STRING_T* disp_ptr,//disp info ptr
                                                                    MMI_STRING_T* name_ptr,//name ptr
                                                                    CC_CALL_SIM_TYPE_E call_sim_type,//call  sim type
                                                                    uint32 call_type ,//call type
                                                                    MN_SUBADDR_T* sub_addr
                                                                );

#ifdef BT_DIALER_SUPPORT
/*****************************************************************************/
// 	Description : release BT Call Interface
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_ReleaseBtCall(void);
/*****************************************************************************/
// 	Description : Make BT Call Interface
//	Global resource dependence : 
//  Author:chunjie.liu
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_MakeBtCall(
   uint8 *tele_num,//phone number
   uint8 tele_len//phone number wstr_len
   );
#endif

/*****************************************************************************/
// 	Description : get call sim type
//	Global resource dependence : 
//  Author:wancan.you
//	Note:
/*****************************************************************************/
PUBLIC CC_CALL_SIM_TYPE_E MMIAPICC_GetCallSimType(MN_DUAL_SYS_E dual_sys);

#ifndef MMI_MULTI_GREEN_KEY_SINGLE
/*****************************************************************************/
// 	Description : get dual_sys according to call key
//	Global resource dependence : 
//  Author: Michael.shi
//	Note: especially for multisim.
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_CheckDualSysType(MMI_MESSAGE_ID_E msg_id, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : get call sim type according to call key
//	Global resource dependence : 
//  Author: Michael.shi
//	Note: especially for multisim.
/*****************************************************************************/
PUBLIC CC_CALL_SIM_TYPE_E MMIAPICC_CheckCallSimType(MMI_MESSAGE_ID_E msg_id, CC_CALL_SIM_TYPE_E sim_type);
#endif

/*****************************************************************************/
// 	Description : 在通话过程中，对声音设备进行操作
//	Global resource dependence : 
//  Author:SAMBOO.SHEN
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN	MMIAPICC_SwitchAudioDevice ( MMICC_AUDIO_OPER_E oper, MMICC_AUDIO_DEV_E dev);

/*****************************************************************************/
// 	Description : Hold the active call for preparing  for waiting call
//	Global resource dependence : s_call_context
//  Author:louis.wei
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_HoldAndAnswerCW(void);

/*****************************************************************************/
// 	Description : Release call by red key: release incoming and outgoing call in high priority;
//                active call normal priority; hold call low priority
//	Global resource dependence : s_call_context
//  Author:jibin
//	Note:2004.10.14
/*****************************************************************************/
PUBLIC CC_RESULT_E MMIAPICC_ReleaseCallByRedkey(void);

/*****************************************************************************/
// 	Description : when missed call number change,send messege to keylock window if is phone locked.
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_SendMissedCallNumberToKeylock(void);

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIAPICC_GetMissedCallNumber(void);

/*****************************************************************************/
// 	Description : dec missed call number
//	Global resource dependence : 
//  Author:tao.xue
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_DecMissedCallNumber(uint32 call_times);

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_SaveMissedCallNumToNv(void);

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPICC_LoadMissedCallNumFromNv(void);

/*****************************************************************************/
// 	Description : get missed call number
//	Global resource dependence : 
//  Author:jianhui.luo
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsCallByMMI(void);
/*****************************************************************************/
// 	Description : handle headset on or off in call
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_HandleHeadsetInCall(MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : 在通话过程中，调节音量
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_SetCallVolume(uint8 volume);

/*****************************************************************************/
//  Description : 在通话过程中，调节音量
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetCallVolume(void);

/*****************************************************************************/
// 	Description : handle MT Call disconnect
//	Global resource dependence : 
//  Author:figo.feng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_HandleMTCallDisc(DPARAM param);

#ifdef VT_SUPPORT
/*****************************************************************************/
// 	Description : MMIAPICC_HandleMissedCall
//	Global resource dependence : 
//  Author:figo.feng
//	Note:2003.12.01
/*****************************************************************************/
PUBLIC void MMIAPICC_HandleMissedCall(void);
#endif

#ifdef TTS_SUPPORT
/*****************************************************************************/
// 	Description : play incoming call name, play name and 来电
//	Global resource dependence : 
//  Author:barry peng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IncomingCallName_To_Speech(void);
#endif

#if defined(MMI_REJECT_CALL_BY_SMS_SUPPORT)
/*****************************************************************************/
// 	Description : MMIAPICC_SendSMSAfterRejectMTCall 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note: only export to VT
/*****************************************************************************/
PUBLIC void MMIAPICC_SendSMSAfterRejectMTCall(uint8 *name_ptr, uint16 name_len);
#endif

/*****************************************************************************/
//  Description : if the disc cause is "unobtainable destination-permanent/long term"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是号码错误,参见GSM 02.07
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsValideDest(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause);

/*****************************************************************************/
//  Description : if the disc cause is "busy destination" or "tempory"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:自动重拨时判断当前的断开条件是否时用户忙或者暂时无法接通,参见GSM 02.07
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsUserBusy(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause);

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:louis.wei
//	Note:从电话界面切换到浏览器界面
/*****************************************************************************/
PUBLIC void MMIAPICC_SwitchToBrowser( void );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:louis.wei
//	Note:从浏览器切换到电话界面
/*****************************************************************************/
PUBLIC void MMIAPICC_SwitchFromBrowser( void );
#endif

#ifdef  ASP_SUPPORT
/*****************************************************************************/
// 	Description : 此函数专供在答录界面用户接听时调用....
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_GotoRealCallState(uint32 asp_connected_time);

/*****************************************************************************/
// 	Description : 是否要正常对待当前的这通来电...
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC void MMICC_SetAcceptCurCall(BOOLEAN is_accept_call);
#endif

/*****************************************************************************/
// 	Description : to get the number of call
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetCallNumber( 
                                   uint8* number_ptr, //out
                                   uint8 number_max_len
                                   );
#ifdef MV_SUPPORT
/*****************************************************************************/
// 	Description : open main selection window when calling or just setting.
//	Global resource dependence : 
//  Author : 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_OpenMagicVoiceSetMenu(void);

/*****************************************************************************/
// 	Description : whether magic voice is working
//	Global resource dependence : 
//  Author : violent.wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsMagicVoiceWorking(void);

/*****************************************************************************/
// Description : enable magic voice
//Global resource dependence : 
//  Author: samboo.shen
//Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_EnableMV(void);
#endif

/*****************************************************************************/
// 	Description : emergency call win msg
//	Global resource dependence : 
//  Author:bownzhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsInputEmergencyCall(MMI_STRING_T *str_info_ptr, uint16 *emergency_call_len_ptr, MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : response call control
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ResponseCallCtl(MN_DUAL_SYS_E dual_sys,
                                                                                    MMI_STRING_T *alert_str_ptr,
                                                                                    MMI_STRING_T *display_str_ptr,
                                                                                    MN_CALLED_NUMBER_T *cc_address,
                                                                                    SIMAT_SIM_CONTROL_RESULT_E result,
                                                                                    SIMAT_SIMC_DATA_TYPE_E data_type,
                                                                                    BOOLEAN is_need_modified);

/*****************************************************************************/
// 	Description : 
// 1) 112和911任何情况下都是紧急呼叫
// 2) sim/usim卡上的ecc文件中的紧急呼叫号码呼出都是紧急呼叫
// 3) 无卡时，000, 08, 110, 999, 118 and 119 呼出都是紧急呼叫
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:is_only_for_display means in normal status the phone make NORMAL call by 000, 08, 110, 999, 118 and 119 num
// but display ENMERGENCY CALL in window 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsEmergencyNum(
                                 uint8 *tele_num, 
                                 uint8 tele_len,
                                 MN_DUAL_SYS_E dual_sys,
                                 BOOLEAN is_only_for_display
                                 );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsEmergencyCall(void);

/*****************************************************************************/
//  Description : Get Calling SIM(Not In call return MMI_DUAL_SYS_MAX)
//  Global resource dependence : none
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_GetCallingSIM(void);


/*****************************************************************************/
//  Description : get current existed  hod call count
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetHoldCallCount(void);

/*****************************************************************************/
//  Description : get current existed  active call count
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetActiveCallCount(void);

/*****************************************************************************/
//  Description : get current existed  mpty call count
//  Global resource dependence : none
//  Author: taoxue
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetMPTYCallCount(void);

/*****************************************************************************/
//  Description : 获取蓝牙设置的音量
//  Global resource dependence : 
//  Author: Qing.Yu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetBtSpeakerVol(void);

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)

/*****************************************************************************/
//  Description : 收到蓝牙调节音量的消息，显示音量调节窗口
//  Global resource dependence : 
//  Author: Qing.Yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_BTSpeakerVolChange(void);

/*****************************************************************************/
//Description : get the call status by the given call state for BT BQB
//Global resource dependence : 
//Author: arvin.zhang
//Note:暂时对呼出电话不区分alerting和dialing
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetCallStatus(CC_CALL_STATE_E call_state);

/*****************************************************************************/
// Description : send call info to bt headset
//Global resource dependence : 
//  Author: louis.wei
//Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_SendCallInfo2HFG(void);

/*****************************************************************************/
// pescription : if there is call coming, then info BT handfree set
// Global resource dependence : 
// Author: qing.yu
// Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_IsCallingAndInfoBt(void);
#endif

#ifndef MMI_GUI_STYLE_TYPICAL
/*****************************************************************************/
//  Description : check if need turn off quickily in call
//  Global resource dependence : none
//  Author:tao.xue
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsNeedTurnOffBLQuicklyInCall(void);
#endif

/*****************************************************************************/
// Description : get current call string
//Global resource dependence : 
//  Author: bin.ji
//Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_GetCurrentCallString(
                                          MMI_STRING_T *str_ptr
                                          );
                                          
/*****************************************************************************/
// 	Description : stop ring at incoming call
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICC_AdjustCallVolume(BOOLEAN is_increase);

/*****************************************************************************/
//  Description : 停止录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICC_StopCCRecord(void);

/*****************************************************************************/
// 	Description : stop ring at incoming call
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICC_StopMtCallPlayingRing(void);

 /*****************************************************************************/
// 	Description : if it is playing ring at incoming call
//	Global resource dependence : 
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPICC_IsMtCallPlayingRing(void);

 /*****************************************************************************/
//  Description : MMIAPICC_SendMsgToApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_SendMsgToApplet(MMI_MESSAGE_ID_E msg_id, DPARAM param_ptr);

#ifdef MMI_CALL_MINIMIZE_SUPPORT  
/*****************************************************************************/
//  Description : MMIAPICC_ActiveCCApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 让cc  applet获得焦点，返回值TRUE：代表cc applet存在，FALSE：表示applet不存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ActiveCCApplet(void);

/*****************************************************************************/
//  Description : MMIAPICC_HideCCApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 让cc  applet隐藏，返回值TRUE：代表cc applet存在，FALSE：表示applet不存在
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_HideCCApplet(void);

/*****************************************************************************/
//  Description : get cc applet handle
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIAPICC_GetCCAppletHandle(void);

/*****************************************************************************/
//  Description : get cc applet handle
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsMinimize(void);

#endif //MMI_CALL_MINIMIZE_SUPPORT


/*****************************************************************************/
// 	Description :create cc dial panel for sending dtmf
//	Global resource dependence : 
//    Author:tao.xue
//	Note:20111125
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsDtmfPanel(MMI_WIN_ID_T win_id);


/*****************************************************************************/
//  Description :close codec handle & audio handle when powering off for normally play poweroff audio or video
//  Global resource dependence : 
//  Author: samboo.shen
//  Note: just only use in power off procede
/*****************************************************************************/
PUBLIC void MMIAPICC_QuitForPowerOff ( void );

/*****************************************************************************/
// 	Description : cc nv size
//	Global resource dependence : none
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICC_GetNvSize(void);

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
// 	Description : Set the SOS Number from csc
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_SetSOSByCSC(MMICUS_CSC_SETTING_MAIN_SOS_NUM_T *csc_sos_ptr);
#endif
#if defined(MMI_WIDGET_SPEED_DIAL)
PUBLIC void OpenSpeedDialListWinSetCurItem(uint8 *cur_item);
#endif
#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//	Author: Jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIIDLE_IsPlayDialTone(MMI_MESSAGE_ID_E msg_id);
#endif
/*****************************************************************************/
// 	Description : is existing unciphering call 
//	Global resource dependence : 
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsExistUncipheringCall(void);

#ifndef MMI_MULTI_SIM_SYS_SINGLE
/*****************************************************************************/
// 	Description : Open SIM Calling Prompt
//	Global resource dependence : none
//	Author:
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_OpenSIMCallingPrompt(void);

/*****************************************************************************/
//  Description : Is Other Service Available
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsSimServiceAvail(MN_DUAL_SYS_E dual_sys);
#endif
#ifndef SIM_PLUG_IN_SUPPORT//bug 184713
/*****************************************************************************/
//  Description : if sim removed,exit mo call
//  Global resource dependence : none
//  Author:chunjie liu
//  Note:cucc support conctrol,befor mo call request,make a request to sim
/*****************************************************************************/
PUBLIC void MMIAPICC_ExitMoCall(MN_DUAL_SYS_E dual_sys);
#endif

/*****************************************************************************/
//  Description : get call error type
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMICC_MO_CALL_ERROR_TYPE_E MMICC_GetMoCallErrorType(void);
/*****************************************************************************/
// 	Description : 是否存在待播出的ecc号码
//	Global resource dependence : none
//	Author:yonghua.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICC_IsExistNoPSEccNumber(void);

/*****************************************************************************/
//  Description : Open cc dialpad window
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC void WatchCC_DialpadWin_Enter(void);

#endif
