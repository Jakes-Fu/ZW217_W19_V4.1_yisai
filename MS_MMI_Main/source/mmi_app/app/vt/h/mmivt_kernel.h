/*****************************************************************************
** File Name:      mmivt_kernel.h                                                   *
** Author:                                                                   *
** Date:           21/12/2009                                               *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                 *
/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 21/12/2009     samboo.shen       Create
******************************************************************************/

#ifndef _MMIVT_KERNEL_H_
#define _MMIVT_KERNEL_H_ 

#ifdef VT_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmicc_export.h"
#include "mn_type.h"
#include "mn_api.h"
#include "sig_code.h"
#include "mmi_appmsg.h"
#include "mmivt_export.h"
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
#define MMIVT_PHONE_NUM_MAX_LEN (MMIPB_BCD_NUMBER_MAX_LEN << 1) //BCD号码最大长度
#define MMIVT_MAX_PBNAME_LEN MMIPB_MAX_STRING_LEN //最大电话号码名字长读

#define MMIVT_KERNEL_INFO_INCOMING_CALL_T APP_MN_SETUP_IND_T

#define MMIVT_KERNEL_INFO_DISCONNECTED_IND_T APP_MN_CALL_DISCONNECTED_IND_T
    
typedef enum
{
    MMIVT_KERNEL_MSG_USER_DIAL,        //拨出电话
    MMIVT_KERNEL_MSG_SETUP_COMPLETE_IND,   //信道建立完成（MO）
    MMIVT_KERNEL_MSG_START_MEDIA_IND,      //324建立完成（开始声视频传输）
    MMIVT_KERNEL_MSG_USER_HANGUP,      //挂断电话
    MMIVT_KERNEL_MSG_324_CLOSED_IND,   //324开始断开
    MMIVT_KERNEL_MSG_324_CLOSED_CNF,   //324挂断完成
    MMIVT_KERNEL_MSG_DISCONNECTED_IND, //PS挂断完成
    MMIVT_KERNEL_MSG_INCOMING_CALL_IND,    //对方来电
    MMIVT_KERNEL_MSG_ALERTING_IND,     //本地振铃
    MMIVT_KERNEL_MSG_CONNECT_CALL,     //接听电话
    MMIVT_KERNEL_MSG_CONNECT_IND,      //信道建立完成（MT）
    MMIVT_KERNEL_MSG_MAX
}MMIVT_KERNEL_MSG_E;

typedef enum
{
    MMIVT_KERNEL_STATE_IDLE,              //空闲状态
    MMIVT_KERNEL_STATE_PS_CONNECTING,      //PS信道连接中
    MMIVT_KERNEL_STATE_H324_CONNECTING,    //324连接中
    MMIVT_KERNEL_STATE_CALL_CONNECTED,     //通话中
    MMIVT_KERNEL_STATE_ALERTING,          //振铃中(MT)
    MMIVT_KERNEL_STATE_CONNECT_PENDING,   //接听请求已经发出，信道正在建立中
    MMIVT_KERNEL_STATE_DISCONNECTING,     //正在挂断中
    MMIVT_KERNEL_STATE_MAX
}MMIVT_KERNEL_STATE_E;

typedef enum
{
    MMVT_KERNEL_SUBSTATE_NORMAL,
    MMVT_KERNEL_SUBSTATE_REMOTE_ALERTING,
    MMVT_KERNEL_SUBSTATE_MMR, //多媒体彩铃状态
    MMVT_KERNEL_SUBSTATE_MAX
}MMVT_KERNEL_SUBSTATE_E; //子状态

typedef	enum//电话是MO还是MT
{
    MMIVT_CALL_TYPE_MO_CALL,
    MMIVT_CALL_TYPE_MT_CALL,
    MMIVT_CALL_TYPE_CALL_MAX
}MMIVT_CALL_TYPE_E;


//typedef enum
//{
//    VT_DISC_MODE_BY_USER,	//用户主动挂断
//    VT_DISC_MODE_BY_324,		//324断开造成电话挂断
//    VT_DISC_MODE_BY_PS,	//PS断开造成电话挂断
//    VT_DISC_MODE_BY_FALLBACK,	//用户发起回落请求
//    VT_DISC_MODE_MAX
//}VT_DISC_MODE_E; //VT挂断的原因，主要由对方传来的原因值决定

typedef struct
{
    BOOLEAN is_number_present;  //对方号码是否显示
    BOOLEAN is_exit_in_pb;  //是否在存在于PB中
    uint16 name_len;    //PB中对应名字长度
    wchar name[MMIVT_MAX_PBNAME_LEN + 1]; //PB对应名字
    //uint8 pb_group_id;  
    //uint16 pb_entry_id;
    uint8 tele_len; //十进制号码长度
    uint8 tele_num[MMIVT_PHONE_NUM_MAX_LEN + 2]; //十进制号码内容
    MN_CALLED_NUMBER_T bcd_num; //BCD码信息
}MMIVT_ADDRESS_INFO_T;//VT号码的相关信息

typedef struct 
{
    MN_DUAL_SYS_E dual_sys; //发出电话的卡号
    uint8* tele_num;         //拨打电话号码
    uint8 tele_len;          //电话号码长度
}MMIVT_KERNEL_INFO_USER_DIAL_T; //MMIVT_KERNEL_MSG_USER_DIAL的数据结构

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
    
    
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : MMIVT_HandleKernelMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:处理来自ps和ui的操作，进行相应的状态跃迁。
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIVT_HandleKernelMsg(
    MMIVT_KERNEL_MSG_E msg,	//需要处理的msg
    DPARAM parm	//msg相关信息
    );

/*****************************************************************************/
//  Description : MMIVT_HandleExternalMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:接收来自CC和324的消息。
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIVT_HandleExternalMsg( PWND app_ptr, uint16 msg_id, DPARAM param);


/*****************************************************************************/
//  Description : MMIVT_SetAddressInfoByNormalNum
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:according to normal num set VT address info 
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIVT_SetAddressInfoByNormalNum( uint8* tele_num, uint8 tele_len);
/*****************************************************************************/
//  Description : MMIVT_GetCallState
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_KERNEL_STATE_E MMIVT_GetCallState( void);

/*****************************************************************************/
//  Description : MMIVT_GetSubState
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMVT_KERNEL_SUBSTATE_E MMIVT_GetSubState( void);
/*****************************************************************************/
//  Description : MMIVT_GetCallType
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MMIVT_CALL_TYPE_E MMIVT_GetCallType( void);
/*****************************************************************************/
//  Description : MMIVT_GetDualSys
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIVT_GetDualSys( void);

/*****************************************************************************/
//  Description : MMIVT_SetDualSys
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIVT_SetDualSys(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : MMIVT_GetCallBCDNumber
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_GetCallBCDNumber( MN_CALLED_NUMBER_T *bcd_number_ptr);

/*****************************************************************************/
//  Description : MMIVT_GetCallBCDNumber
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: out parameter
/*****************************************************************************/
PUBLIC void MMIVT_GetCallNormalNumber( uint8* out_num_ptr, uint8* out_num_len_ptr);

/*****************************************************************************/
//  Description : MMIVT_GetCallStartTime
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVT_GetCallStartTime( void);

/*****************************************************************************/
//  Description : MMIVT_GetCallDuration
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIVT_GetCallDuration( void);

/*****************************************************************************/
//  Description : MMIVT_GetDiscCause
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC MN_CALL_DISCONNECT_CALL_CAUSE_E MMIVT_GetDiscCause( void);

/*****************************************************************************/
//  Description : 接受回落请求，发起语音呼叫
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:
/*****************************************************************************/
PUBLIC void MMIVT_MakeFBCall( void);

#ifdef   __cplusplus
    }
#endif
    
#endif //VT_SUPPORT
    
#endif //_MMIVT_KERNEL_H_
