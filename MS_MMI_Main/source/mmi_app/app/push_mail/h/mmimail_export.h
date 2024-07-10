/***************************************************************************
** File Name:      mmimail_export.h                                               *
** Author:         liyuanyuan                                              *
** Date:           2010/05/07                                              *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.        *
** Description:    This file is used to declare common variables and       *
**                 functions.                                              *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIMAIL_H_    
#define  _MMIMAIL_H_   

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "mail_message.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**----------------------------------------------------------------------*
 **                         MACRO DEFINITION                             *
 **----------------------------------------------------------------------*/


/**----------------------------------------------------------------------*
 **                         TYPE AND STRUCT                              *
 **----------------------------------------------------------------------*/
//����������ʽ
typedef enum
{
    MAIL_ENTER_NULL,
    MAIL_ENTER_MAINMENU,    //���˵�����
    MAIL_ENTER_SMS,         //����������
    MAIL_ENTER_INBOX,       //�����ռ������
    MAIL_ENTER_NEWMAIL,     //�����½�����
    MAIL_ENTER_REPLYMAIL,   //����ظ�����
    MAIL_ENTER_FORWARDMAIL, //����ת���༭����
    MAIL_ENTER_EDITMAIL,    //����༭ԭʼ�ʼ�����
    MAIL_ENTER_SETTINGWIN,  //�������ý���
    MAIL_ENTER_READWIN,     //�����ʼ��Ķ�����
    MAIL_ENTER_MAX
}MAIL_ENTER_TYPE_E;

//��������
typedef enum
{
    MMIMAIL_BOX_NONE,
    MMIMAIL_INBOX,      //�ռ���
    MMIMAIL_OUTBOX,     //������
    MMIMAIL_SENTBOX,    //�ѷ�����
    MMIMAIL_DRAFTBOX,   //�ݸ���
    MMIMAIL_BOX_MAX
}MMIMAIL_BOX_TYPE_E;

//�༭�ʼ����ڴ�����
typedef enum
{
    MMIMAIL_OPEN_TYPE_DEFAULT,
    MMIMAIL_OPEN_TYPE_NEW_MAIL,         //�½��ʼ�
    MMIMAIL_OPEN_TYPE_ORIGINAL,         //�༭ԭʼ�ʼ�
    MMIMAIL_OPEN_TYPE_REPLY,            //�ظ��ʼ�
    MMIMAIL_OPEN_TYPE_REPLY_ALL,        //ȫ���ظ�
    MMIMAIL_OPEN_TYPE_FORWARD_CONTENT,  //ת������
    MMIMAIL_OPEN_TYPE_FORWARD_ALL,      //ת��ȫ��
    MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ACC,  //������ת������
    MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ALL   //������ת��ȫ��
}MMIMAIL_OPEN_EDIT_WIN_TYPE_E;

//�ʼ�������ش�����
typedef enum
{
    MMIMAIL_NO_ERROR = 0,     
    MMIMAIL_NO_OPERATION,
    MMIMAIL_ERROR_FATAL,
    MMIMAIL_ERROR_FAIL,
    MMIMAIL_ERROR_NOT_INIT,
    MMIMAIL_ERROR_UDISK_USING, 
    MMIMAIL_ERROR_CC_IN_CALL,
    MMIMAIL_ERROR_CMMB_RUNNING,
    MMIMAIL_ERROR_EDITING,
    MMIMAIL_ERROR_SENDRECVING,
    MMIMAIL_ERROR_NO_CONTENT, 
    MMIMAIL_ERROR_NO_MEMORY, 
    MMIMAIL_ERROR_NO_SPACE, 
    MMIMAIL_ERROR_INVALID_DATA,
    MMIMAIL_ERROR_INVALID_SIZE,
    MMIMAIL_ERROR_CONFLICT,    
    MMIMAIL_ERROR_MAX_SLIDE,
    MMIMAIL_ERROR_NOT_SUPPORT,
    MMIMAIL_ERROR_SFS_ERROR, 
    MMIMAIL_ERROR_SIM_ERROR,     
    MMIMAIL_ERROR_ENCODE_ERROR,     
    MMIMAIL_ERROR_SETTING_ERROR,
    MMIMAIL_ERROR_INVALID_URL,     
    MMIMAIL_ERROR_PUSH_NOT_EXIST,    
    MMIMAIL_ERROR_CANCEL, 
    MMIMAIL_ERROR_NETLINK_FAILED,   
    MMIMAIL_ERROR_NETLINK_DISCONNECT,  
    MMIMAIL_ERROR_GATEWAY_CONNECT_FAILED, 
    MMIMAIL_ERROR_NETCONNECT_FAILED,   
    MMIMAIL_ERROR_INVALIDPERIOD,   
    MMIMAIL_ERROR_SERVICE_DENIED,  
    MMIMAIL_ERROR_FORMAT_CORRUPT,   
    MMIMAIL_ERROR_ADDRESS_UNRESOLVED,   
    MMIMAIL_ERROR_MESSAGE_NOT_FOUND, 
    MMIMAIL_ERROR_SERVER_BUSY,
    MMIMAIL_ERROR_NOT_ACCEPTED,
    MMIMAIL_ERROR_UNSUPORTED_MESSAGE, 
    MMIMAIL_ERROR_RECV_NEED_AUTH,
    MMIMAIL_ERROR_RECV_ERROR_DATA,
    MMIMAIL_ERROR_MAX  
}MMIMAIL_ERROR_E;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DECLARATION                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : Pushmail��ʼ��
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_Init(void);

/*****************************************************************************/
//	Description : ģ���ʼ��
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_InitModule(void);

/*****************************************************************************/
// 	Description : Pushmail ���
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_Enter(MAIL_ENTER_TYPE_E   enter_type, DPARAM param);

/*****************************************************************************/
//	Description : �˳��ֻ�����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_Exit(void);

/*****************************************************************************/
// 	Description : �����ʷ���Ϣ
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SetChargeInfo(MMI_STRING_T *charge_info);

/*****************************************************************************/
// 	Description : ��ȡ�ʺ���Ϣ�ص�
//	Global resource dependence : none
//  Author:yuanl
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_GetAccountCallback(
                                                    BOOLEAN is_open, //��ǰ���Ƿ�ͨ��139����
                                                    BOOLEAN is_white_sim //��ǰ���Ƿ��ǰ׿�(��Ƿ�ѿ�)
                                                    );

/*****************************************************************************/
//  Description : ���û�����ʼ�����ر����ʼ���ʾ����
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_CloseNewMailPromptWin(void);

/*****************************************************************************/
//  Description : �Ƿ���δ���ʼ�
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsHaveUnreadMail(void);

/*****************************************************************************/
//  Description : �ʼ��Ƿ���
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsMailFull(void);

/*****************************************************************************/
//  Description : ���ö���ģ��׼������
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_SetSmsOk(BOOLEAN is_sms_ok);

/*****************************************************************************/
//	Description : ���ʼ��༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_OpenEditWin(
                                                MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type, 
                                                MAIL_MESSAGE_T *whole_mail_ptr
                                               );

/*****************************************************************************/
//	Description : �����ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SendMail(MAIL_MESSAGE_T *whole_mail_ptr);

/*****************************************************************************/
//	Description : ����ָ��id���ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SendMailById(uint32 record_id);

/*****************************************************************************/
//	Description : ȡ������ָ��id���ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_CancelSendById(uint32 record_id);

/*****************************************************************************/
//  Description : ���ļ������ʼ��༭����
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_EditMailFromFile(
                                        wchar   *full_path_name, //ȫ·��
                                        uint16  full_path_len    //ȫ·������
                                        );

/*****************************************************************************/
//	Description : ��ȡָ���ʼ�����ʼ�������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_ReadMail(uint32 record_id);

/*****************************************************************************/
//	Description : ���ʼ��Ķ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_OpenReadWin(uint32 record_id);

/*****************************************************************************/
//	Description : �ʼ�Ӧ���Ƿ������ϲ�
//	Global resource dependence : none
//	Author:jiaoyou.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_IsActive(void);

/*****************************************************************************/
//  Description : insert string to sms edit
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_InsertMessage(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             );

#ifdef   __cplusplus
    }
#endif
#endif
