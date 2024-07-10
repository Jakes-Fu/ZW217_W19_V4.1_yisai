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
#define MMIVT_FALBACK_WAITING_TIMEOUT 15000	//@samboo.shen vt����ȴ�15��

#endif

#define CC_MAX_CALL_NUM      				7		//���ͬʱ���ڵ�CALL������
#define CC_MAX_PBNAME_LEN MMIPB_MAX_STRING_LEN //���绰�������ֳ���
#define CC_MAX_TELE_NUM_LEN (MMIPB_BCD_NUMBER_MAX_LEN << 1) //���绰����ĳ���
#define CC_DISPLAY_TIME_LEN					9		//��ʾͨ��ʱ���ִ��ĳ���
#define CC_INVALID_INDEX					255		//�Ƿ�����

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
    CC_RESULT_SUCCESS,	//�������óɹ�
    CC_RESULT_ERROR		//�������ô���
}CC_RESULT_E;			//CC�����ķ��ؽ��

typedef enum
{
    CC_SIM1_CALL,  //SIM1����
    CC_SIM2_CALL,  //SIM2����
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)
    CC_SIM3_CALL,  //SIM3����
#if defined(MMI_MULTI_SIM_SYS_QUAD)
    CC_SIM4_CALL,  //SIM4����
#endif
#endif
    CC_MAJOR_SIM_CALL,  //��������
    CC_SECONDARY_SIM_CALL,//��������
    CC_CALL_SIM_MAX//��ָ��SIM������
}CC_CALL_SIM_TYPE_E;

typedef enum
{
    CC_RELEASE_ALL_NORMAL,      //�ر�call�Ĵ��ڣ������ر��������ڣ���ʾ�ͷ���ʾ��ͨ��ʱ��
    CC_RELEASE_ALL_PROMPT,      //�˻�idle����ʾ�ͷ���ʾ��ͨ��ʱ��
    CC_RELEASE_ALL_NOPROMPT,	//�ر�call�Ĵ��ڣ������ر��������ڣ�����ʾ�ͷ���ʾ��ͨ��ʱ��
    CC_RELEASE_ALL_TYPE_MAX
}CC_RELEASE_ALL_TYPE_E;

typedef enum
{
    CC_NULL_STATE,          //����״̬
    CC_INIT_STATE,			//��ʼ״̬
    CC_CALLING_STATE,		//����״̬
    CC_CALL_STATE,			//ͨ��״̬
    CC_INCOMING_CALL_STATE, //����״̬
    CC_HOLD_STATE,			//����״̬
    CC_WAITING_STATE,
    CC_CALL_STATE_MAX
} CC_CALL_STATE_E;			//ÿͨCALL��״̬

//CRSS�������ȼ�:���ͬʱ����waiting call��hold call�������ȴ���waiting call��һ��CRSS����ͬʱ�����������͵ĵ绰
typedef enum
{
    CC_CRSS_RELEASE_DEACTIVE,   //�Ҷϵȴ��л��߱��ֵ绰
    CC_CRSS_RELEASE_ACTIVE_AND_ANSWER,       //�Ҷ��Ѽ���绰����ͨ�ȴ��л��߱��ֵ绰
    CC_CRSS_HOLD_ACTIVE_AND_ANSWER, //���������Ѽ���绰���������ֻ��ߵȴ��е绰
    CC_CRSS_ADD_HOLD_CALL_TO_MPTY, //�����ֵ绰���뵽�绰������
    CC_CRSS_ECT,//����ECT�л�
    CC_CRSS_RESERVED2,
    CC_CRSS_MAX
} CC_CALL_CRSS_E;

typedef struct
{
    MN_DUAL_SYS_E   dual_sys;				//��ʾ��ǰ��ʹ��SIM��ID
    uint32	start_time;	//��ʼͨ��ʱ��
    uint32	stop_time;	//����ͨ��ʱ��
}CC_CALL_TIME_T;		//ͨ��ʱ��

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
    PHOTO_TYPE_CURRENT, //��ǰ�绰��ͷ��
    PHOTO_TYPE_ACTIVE, //����绰��ͷ��
    PHOTO_TYPE_HOLD, //���ֵ绰��ͷ��
#ifndef MMI_GUI_STYLE_TYPICAL
    PHOTO_TYPE_OTHER, //�Ҷϵ绰�绰��ͷ��
#endif
    PHOTO_TYPE_MAX
}CC_PHOTO_TYPE_E;


typedef struct
{
	BOOLEAN			exist_incoming_call;	//�Ƿ����INCOMING��CALL
	BOOLEAN			exist_outgoing_call;	//�Ƿ����׼��������CALL
	BOOLEAN			exist_mpty;				//�Ƿ����MPTY
	uint8			exist_active_call_num;	//����ACTIVE��CALL������
	uint8			exist_hold_call_num;	//����HOLD��CALL������
	BOOLEAN exist_mtfw;
}CC_EXIST_CALL_T;							//��ǰ���ڵ�����CALL��״̬

typedef enum
{
    OTHER_OPERATE,					//��������
    RELEASE_ALL_ACTIVE_AND_ACCEPT,	//�ͷ�����ACTIVE��CALL��������
    HOLD_ALL_ACTIVE_AND_ACCEPT,		//HOLD����ACTIVE��CALL��������
    BUILD_MPTY_AND_ACCEPT,
    RELEASE_ALL_CALL_AND_MAKE_CALL		//Release all call and make a new call
}CC_OPERATE_WAITING_CALL_E;			//�û�������Ĳ���

typedef enum
{
    CRSS_OTHER_OPERATE,	
    RELEASE_ALL_ACTIVE_AND_ACCEPT_WAIT,//�Ҷ���active��call����������
    RELEASE_ALL_ACTIVE_AND_RETRIEVE_HOLD,//�Ҷ���active��call���ָ�hold��call
}CC_OPERATE_CRSS_CODE_E;			//�û�������Ĳ���

typedef struct
{
	BOOLEAN   is_remote_mpty;		//���Է�����һ��MPTY
	BOOLEAN   is_remote_split;		//���Է�SPLIT
	BOOLEAN   is_remote_hold;		//���Է�HOLD
}CC_OPERATED_BY_REMOTE_T;			//���Է��Ĳ���

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
    MN_DUAL_SYS_E dual_sys;		//��ʾ��ǰ��ʹ��SIM��ID����ǰ���ں���״̬������SIM
#ifdef MMI_CSP_SUPPORT
    uint8 line;  //the call from line1 or line2
#endif   
    uint8 call_id;				//��ͨCALL��ID
    BOOLEAN is_mtfw_reject;			//��ͨCALL�Ƿ�����mtfw�ж�
    BOOLEAN is_in_mpty;				//��ͨCALL�Ƿ���һ��MPTY��
    uint8 call_type;

    wchar name[CC_MAX_PBNAME_LEN + 1];//��ͨCALL��Ӧ������
    uint8 name_len;
    BOOLEAN is_save_name;                   //��ADN/FDN/SDN�����ͨ��������Ҫȥƥ��PB�еļ�¼��this is a call from ADN/FDN/SDN list,search from pb is not needed
    BOOLEAN is_name_exist;			//TRUE:���ֲ�Ϊ��,FALSE:����Ϊ�գ�name�д洢����
    BOOLEAN is_stk_call;

    CC_OPERATED_BY_REMOTE_T	operated_by_remote;		//���Է��Ĳ���
    CC_CALL_STATE_E call_state;				//��ͨCALL��״̬
    CC_OPERATED_BY_USER_E operated_by_user;
    BOOLEAN is_release_all;			//�Ƿ��ǹҶ����е绰
    CC_RELEASE_ALL_TYPE_E release_all_type;		//�Ҷ����е绰��ԭ��
    MN_CALLED_NUMBER_T address;				//�������Ϣ
    CC_CALL_TIME_T call_time;				//ͨ����ֹʱ��
    uint32 start_call_time_count;  //��ʼ�Ķ�ʱ��ʱ��
    MN_CALLED_NUMBER_T call_modified_address;//Control�޸ĺ���
    BOOLEAN is_mute; //����һͨ�绰����һ������״̬
    BOOLEAN is_dtmf_sent;
    BOOLEAN is_call_modified_store;
    BOOLEAN is_unciphering_call; //ͨ���Ƿ���δ���ܵ�
}CC_CALL_INFO_T; //ÿͨCALL����Ϣ

typedef struct
{
    MN_DUAL_SYS_E dual_sys;			//��ʾ��ǰ��ʹ������SIM����
#ifdef MMI_CSP_SUPPORT
    uint8 line;  //the call from line1 or line2
#endif    
    MN_CALLED_NUMBER_T address;						//�������Ϣ
    wchar name[CC_MAX_PBNAME_LEN + 1];	//��ͨCALL��Ӧ������
    uint16 name_len;
    wchar specified_str[CC_MAX_PBNAME_LEN + 1];	//��ͨCALL��Ӧ������
    uint8 specified_str_len;
    BOOLEAN is_want_mo_call;				//�Ƿ�����Ҫ������CALL
    BOOLEAN is_save_name;                   //��ADN/FDN/SDN�����ͨ��������Ҫȥƥ��PB�еļ�¼��this is a call from ADN/FDN/SDN list,search from pb is not needed
    BOOLEAN is_name_exist;                  //TRUE:���ֲ�Ϊ��,FALSE:����Ϊ�գ�name�д洢���롣True means there is a name in name[] field. otherwise. there is a number replaced.
    BOOLEAN is_exist_specified_str;
    uint32 call_type;
    BOOLEAN clir_present;
    uint8 clir_type;
    BOOLEAN is_sub_addr_present;
    MN_SUBADDR_T sub_addr;
}CC_WANT_MO_CALL_T; //��Ҫ������CALL����Ϣ������ͨ���в��º���


typedef struct
{
    BOOLEAN is_dtmf;	//if there is dtmf in calling number string
    uint8 dtmf_len;	//dtmf string len
    uint8 dtmf_str[CC_MAX_TELE_NUM_LEN]; //��������ֻ�����
    uint8 allow_send_flag; //SPEC�涨���������MMI���Է���dtmf�����ڶ�����������յ�APP_MN_SYNC_IND��APP_MN_PROGRESS_IND��Ϣ֮�󡱣�
}CC_DTMF_STATE_T;//DTMF��״̬

#ifdef MMI_VOLTE_SUPPORT
typedef struct
{
    BOOLEAN is_ps_call;//TRUE current call is volte call
    BOOLEAN is_volte_codec_on;//TRUE volte codec is on
}CC_PSCALL_INFO_T;
#endif

typedef struct
{
    MN_DUAL_SYS_E  dual_sys;				//MOʱ��ʾ���ſ�������MTʱ��ʾ���ſ����ڱ�����

    //uint8						    count_time_id;
    uint8   						call_number;			//��ǰCALL������,1-7
    uint8   						current_call;			//��ǰ��ʾ��CALL,0-6

    uint8                           incoming_call_index;
    //BOOLEAN                         need_dtmf;  //�绰����ͨ���и�ֵΪTRUE�����Է�DTMF
    //BOOLEAN                   is_dtmf_enable; //ͨ��֮���û���������,�绰״̬�ı�֮������ΪTRUE,���ǵ�ǰ�绰Ϊ�෽ͨ��ʱ���˵��������
    uint32							missed_call_num;		//δ�����������
    uint32							call_time_count;		//������¼��ʱ��ѭ���Ĵ���
    CC_DTMF_STATE_T					dtmf_state;				//DTMF��״̬
    CC_EXIST_CALL_T   				exist_call;				//��ǰ���ڵ�����CALL��״̬
    CC_OPERATE_WAITING_CALL_E		operate_waiting_call;	//�û�������Ĳ���
    CC_OPERATE_CRSS_CODE_E		operate_CRSS_code;			//CRSS ����
    CC_WANT_MO_CALL_T				want_mo_call;			//��Ҫ������CALL����Ϣ������ͨ���в��º���
    //CC_AUDIO_STATE_T				audio_state;			//�����ŵ���״̬
    CC_CALL_INFO_T   				call_info[CC_MAX_CALL_NUM];//ÿͨCALL����Ϣ
#ifdef MMI_VOLTE_SUPPORT    
    CC_PSCALL_INFO_T             pscall_info;
#endif
}CC_CALL_CONTEXT_T;//��ǰCCģ����������

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
    //MMICC_AUDIO_OPER_INIT,          //ͨ������ʱ������ʼ�����豸
    //MMICC_AUDIO_OPER_CLOSE,         //ͨ����������������豸
    MMICC_AUDIO_OPER_ENABLE,        //����ĳ�������豸ģʽ
    MMICC_AUDIO_OPER_DISABLE,       //�ر�ĳ�������豸ģʽ
    //MMICC_AUDIO_OPER_ENABLE_ASYNC,  //�첽�豸ģʽ������·������ɺ󣬽��в���
    //MMICC_AUDIO_OPER_DISABLE_ASYNC, //�첽�豸ģʽ������·�Ͽ���ɺ󣬽��в���
    //MMICC_AUDIO_OPER_ASYNC_FAIL,    //�첽�豸������·����ʧ�ܣ����к�������
    //MMICC_AUDIO_OPER_RESUME,        //�ָ���ǰ״ֵ̬�ָ������豸
    MMICC_AUDIO_OPER_MAX            //ϵͳ����������ʹ��
}MMICC_AUDIO_OPER_E;

typedef enum
{
    MMICC_AUDIO_DEV_HH,	        //��Ͳ
    MMICC_AUDIO_DEV_EP,         //����
    MMICC_AUDIO_DEV_HF,         //����
    MMICC_AUDIO_DEV_BT = 0x11,	//���� �첽�豸ģʽ 
    MMICC_AUDIO_DEV_MAX 
}MMICC_AUDIO_DEV_E;

typedef enum
{
    CC_MO_ERROR_NONE,
    CC_MO_DIAL_FAILED,          //����ʧ��
    CC_MO_EMPTY_NUM,            //����Ϊ��
    CC_MO_IN_FLY_MODE,          //�ȹرշ���ģʽ
    CC_MO_IS_BARRING_NUM,       //���ܺ��иú���
    CC_MO_NET_UNAVAIL,          //���粻����
    CC_MO_INVALID_NUM,          //��Ч����
    CC_MO_NO_SIM,               //��SIM��
    CC_MO_OPER_FAIL,            //����ʧ��
    CC_MO_FDN_ONLY,             //���޹̶�����
    CC_MO_NO_IP_NUM,          //δ����IP����
    CC_MO_SIM_CORRUPT,        //ʹ����һ�ſ�����
    CC_MO_MERGR_CALL_FIRST,   //��Ҫ�Ƚ���MPTY
    CC_MO_CMD_NO_SUPPORT,     //ָ�֧��
    CC_MO_FORBID_IN_VT,       //��Ƶͨ���в��ܺ���
    CC_MO_NO_VOLTE_CALL,      //����volte call
    CC_MO_ECC_CALL,           //��֧�ֽ�������
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
//					   uint8		call_id,//��ʱ���Σ�������vt�෽ͨ��
					   MMI_STRING_T	*name_str_ptr//OUT
						);

/*****************************************************************************/
// 	Description : ��ȡ��ǰͨ��ʱ����Ϣ
//	Global resource dependence : 
//  Author:xiongxiaoyan
//	Note:��ȡ��ǰͨ��ʱ����Ϣ
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
// 	Description : �Ҷϵ�ǰ��¼�绰
//	Global resource dependence : 
//  Author:koki gao
//	Note: 
/*****************************************************************************/
#ifdef ASP_SUPPORT
PUBLIC CC_RESULT_E MMIAPICC_ReleaseAspCallReq(MN_CALL_DISCONNECT_CALL_CAUSE_E cause);
#endif
/*****************************************************************************/
// 	Description : �Ҷϵ�ǰ�绰
//	Global resource dependence : 
//  Author:samboo.shen
//	Note:
/*****************************************************************************/
PUBLIC CC_RESULT_E MMICC_ReleaseCurrentCallReq(MN_CALL_DISCONNECT_CALL_CAUSE_E cause);


/*****************************************************************************/
// 	Description : ��ȡ��ǰ���е�����
//	Global resource dependence : 
//  Author:xiongxiaoyan 
//	Note:20060404
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPICC_GetCallDualSys(void);

/*****************************************************************************/
// 	Description : ��/�ر�����ģʽ
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
// 	Description : ��ͨ�������У��������豸���в���
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
//  Description : ��ͨ�������У���������
//  Global resource dependence : 
//  Author:SAMBOO.SHEN
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_SetCallVolume(uint8 volume);

/*****************************************************************************/
//  Description : ��ͨ�������У���������
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
// 	Description : play incoming call name, play name and ����
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
//  Note:�Զ��ز�ʱ�жϵ�ǰ�ĶϿ������Ǻ������,�μ�GSM 02.07
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsValideDest(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause);

/*****************************************************************************/
//  Description : if the disc cause is "busy destination" or "tempory"
//  Global resource dependence : 
//  Author: bown.zhang
//  Note:�Զ��ز�ʱ�жϵ�ǰ�ĶϿ������Ƿ�ʱ�û�æ������ʱ�޷���ͨ,�μ�GSM 02.07
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_IsUserBusy(MN_CALL_DISCONNECT_CALL_CAUSE_E disc_cause);

#ifdef BROWSER_SUPPORT
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:louis.wei
//	Note:�ӵ绰�����л������������
/*****************************************************************************/
PUBLIC void MMIAPICC_SwitchToBrowser( void );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author:louis.wei
//	Note:��������л����绰����
/*****************************************************************************/
PUBLIC void MMIAPICC_SwitchFromBrowser( void );
#endif

#ifdef  ASP_SUPPORT
/*****************************************************************************/
// 	Description : �˺���ר���ڴ�¼�����û�����ʱ����....
//	Global resource dependence : 
//  Author:jun.hu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPICC_GotoRealCallState(uint32 asp_connected_time);

/*****************************************************************************/
// 	Description : �Ƿ�Ҫ�����Դ���ǰ����ͨ����...
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
// 1) 112��911�κ�����¶��ǽ�������
// 2) sim/usim���ϵ�ecc�ļ��еĽ������к���������ǽ�������
// 3) �޿�ʱ��000, 08, 110, 999, 118 and 119 �������ǽ�������
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
//  Description : ��ȡ�������õ�����
//  Global resource dependence : 
//  Author: Qing.Yu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPICC_GetBtSpeakerVol(void);

#if defined(BLUETOOTH_SUPPORT) && defined(BT_BQB_SUPPORT)

/*****************************************************************************/
//  Description : �յ�����������������Ϣ����ʾ�������ڴ���
//  Global resource dependence : 
//  Author: Qing.Yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_BTSpeakerVolChange(void);

/*****************************************************************************/
//Description : get the call status by the given call state for BT BQB
//Global resource dependence : 
//Author: arvin.zhang
//Note:��ʱ�Ժ����绰������alerting��dialing
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
//  Description : ֹͣ¼����ͨ��¼����
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
//  Note: ��cc  applet��ý��㣬����ֵTRUE������cc applet���ڣ�FALSE����ʾapplet������
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPICC_ActiveCCApplet(void);

/*****************************************************************************/
//  Description : MMIAPICC_HideCCApplet
//  Global resource dependence : 
//  Author:samboo.shen
//  Note: ��cc  applet���أ�����ֵTRUE������cc applet���ڣ�FALSE����ʾapplet������
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
// 	Description : �Ƿ���ڴ�������ecc����
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
