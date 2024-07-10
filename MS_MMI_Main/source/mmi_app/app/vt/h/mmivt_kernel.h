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
#define MMIVT_PHONE_NUM_MAX_LEN (MMIPB_BCD_NUMBER_MAX_LEN << 1) //BCD������󳤶�
#define MMIVT_MAX_PBNAME_LEN MMIPB_MAX_STRING_LEN //���绰�������ֳ���

#define MMIVT_KERNEL_INFO_INCOMING_CALL_T APP_MN_SETUP_IND_T

#define MMIVT_KERNEL_INFO_DISCONNECTED_IND_T APP_MN_CALL_DISCONNECTED_IND_T
    
typedef enum
{
    MMIVT_KERNEL_MSG_USER_DIAL,        //�����绰
    MMIVT_KERNEL_MSG_SETUP_COMPLETE_IND,   //�ŵ�������ɣ�MO��
    MMIVT_KERNEL_MSG_START_MEDIA_IND,      //324������ɣ���ʼ����Ƶ���䣩
    MMIVT_KERNEL_MSG_USER_HANGUP,      //�Ҷϵ绰
    MMIVT_KERNEL_MSG_324_CLOSED_IND,   //324��ʼ�Ͽ�
    MMIVT_KERNEL_MSG_324_CLOSED_CNF,   //324�Ҷ����
    MMIVT_KERNEL_MSG_DISCONNECTED_IND, //PS�Ҷ����
    MMIVT_KERNEL_MSG_INCOMING_CALL_IND,    //�Է�����
    MMIVT_KERNEL_MSG_ALERTING_IND,     //��������
    MMIVT_KERNEL_MSG_CONNECT_CALL,     //�����绰
    MMIVT_KERNEL_MSG_CONNECT_IND,      //�ŵ�������ɣ�MT��
    MMIVT_KERNEL_MSG_MAX
}MMIVT_KERNEL_MSG_E;

typedef enum
{
    MMIVT_KERNEL_STATE_IDLE,              //����״̬
    MMIVT_KERNEL_STATE_PS_CONNECTING,      //PS�ŵ�������
    MMIVT_KERNEL_STATE_H324_CONNECTING,    //324������
    MMIVT_KERNEL_STATE_CALL_CONNECTED,     //ͨ����
    MMIVT_KERNEL_STATE_ALERTING,          //������(MT)
    MMIVT_KERNEL_STATE_CONNECT_PENDING,   //���������Ѿ��������ŵ����ڽ�����
    MMIVT_KERNEL_STATE_DISCONNECTING,     //���ڹҶ���
    MMIVT_KERNEL_STATE_MAX
}MMIVT_KERNEL_STATE_E;

typedef enum
{
    MMVT_KERNEL_SUBSTATE_NORMAL,
    MMVT_KERNEL_SUBSTATE_REMOTE_ALERTING,
    MMVT_KERNEL_SUBSTATE_MMR, //��ý�����״̬
    MMVT_KERNEL_SUBSTATE_MAX
}MMVT_KERNEL_SUBSTATE_E; //��״̬

typedef	enum//�绰��MO����MT
{
    MMIVT_CALL_TYPE_MO_CALL,
    MMIVT_CALL_TYPE_MT_CALL,
    MMIVT_CALL_TYPE_CALL_MAX
}MMIVT_CALL_TYPE_E;


//typedef enum
//{
//    VT_DISC_MODE_BY_USER,	//�û������Ҷ�
//    VT_DISC_MODE_BY_324,		//324�Ͽ���ɵ绰�Ҷ�
//    VT_DISC_MODE_BY_PS,	//PS�Ͽ���ɵ绰�Ҷ�
//    VT_DISC_MODE_BY_FALLBACK,	//�û������������
//    VT_DISC_MODE_MAX
//}VT_DISC_MODE_E; //VT�Ҷϵ�ԭ����Ҫ�ɶԷ�������ԭ��ֵ����

typedef struct
{
    BOOLEAN is_number_present;  //�Է������Ƿ���ʾ
    BOOLEAN is_exit_in_pb;  //�Ƿ��ڴ�����PB��
    uint16 name_len;    //PB�ж�Ӧ���ֳ���
    wchar name[MMIVT_MAX_PBNAME_LEN + 1]; //PB��Ӧ����
    //uint8 pb_group_id;  
    //uint16 pb_entry_id;
    uint8 tele_len; //ʮ���ƺ��볤��
    uint8 tele_num[MMIVT_PHONE_NUM_MAX_LEN + 2]; //ʮ���ƺ�������
    MN_CALLED_NUMBER_T bcd_num; //BCD����Ϣ
}MMIVT_ADDRESS_INFO_T;//VT����������Ϣ

typedef struct 
{
    MN_DUAL_SYS_E dual_sys; //�����绰�Ŀ���
    uint8* tele_num;         //����绰����
    uint8 tele_len;          //�绰���볤��
}MMIVT_KERNEL_INFO_USER_DIAL_T; //MMIVT_KERNEL_MSG_USER_DIAL�����ݽṹ

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
//  Note:��������ps��ui�Ĳ�����������Ӧ��״̬ԾǨ��
/*****************************************************************************/
PUBLIC MMIVT_RESULT_E MMIVT_HandleKernelMsg(
    MMIVT_KERNEL_MSG_E msg,	//��Ҫ�����msg
    DPARAM parm	//msg�����Ϣ
    );

/*****************************************************************************/
//  Description : MMIVT_HandleExternalMsg
//  Global resource dependence : 
//  Author:samboo.shen
//  Note:��������CC��324����Ϣ��
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
//  Description : ���ܻ������󣬷�����������
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
