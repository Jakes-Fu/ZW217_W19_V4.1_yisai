/***************************************************************************
** File Name:      mmimail_internal.h                                      *
** Author:         liyuanyuan                                              *
** Date:           2010/05/07                                              *
** Copyright:      2010 Hisense, Incorporated. All Rights Reserved.        *
** Description:    This file is used to define module internal relative    *
**                 contents.                                               *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 2010/05/07     yuanl            Create                                  *
**                                                                         *
****************************************************************************/
#ifndef _MMIMAIL_INTERNEL_H_    
#define  _MMIMAIL_INTERNEL_H_   

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmimail_export.h"
#include "comm_debug.h"
#include "comm_memory.h"
#include "mmifmm_export.h"
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
#define MMIMAIL_MAX_CONTENT_LEN             (50 * 1024)     //���Ŀ������������ 50K
#define MMIMAIL_MIN_EDIT_CONTENT_LEN        (10 * 1024)     //���Ŀɱ༭����Ԥ������ 10K
#define MMIMAIL_MAX_SUBJECT_LEN             (1 * 1024)      //���������������� 1K
#define MMIMAIL_MAX_RECEIVER_NUM            20              //�ռ���������,����to/cc/bcc (��Χ:1~30)
#define MMIMAIL_MAX_ADDRESS_LEN             128             //�ʼ���ַ����󳤶ȣ�ͬMAIL_SETTING_INFO_T�ʺŶ���
#define MMIMAIL_ALL_ADDR_LEN                (MMIMAIL_MAX_RECEIVER_NUM * MMIMAIL_MAX_ADDRESS_LEN)
#define MMIMAIL_MAX_NAME_LEN                64              //�ʼ���ʾ���ֵ���󳤶�
#define MMIMAIL_MAX_ACC_NUM                 10              //�����ܸ���
#define MMIMAIL_MAX_ACC_SIZE                (1024 * 1024)   //�����ܴ�С: 1M
#define MMIMAIL_MAX_FILENAME_LEN            MMIFMM_FILENAME_LEN         //�ļ�������󳤶�    
#define MMIMAIL_MAX_FULLNAME_LEN            MMIFMM_FULL_FILENAME_LEN    //ȫ·������󳤶�

#define MMIMAIL_MAX_ACC_NAME_LEN            128             //�ʺ�������󳤶�
#define MMIMAIL_MAX_PASSWORD_LEN            16              //ͬMAIL_SETTING_INFO_T���붨��

#define MMIMAIL_MAX_LIST_DISP_LEN           50              //�б�������ʾ����󳤶�

#define MMIMAIL_FILE_SIZE_STR_LEN           10              //�ļ���С�ַ�����󳤶�

#define MMIMAIL_MAX_CONTENT_SIZE            65535           //�ʼ������ܱ�ʾ�����ֵ����Ϊ���ĳ���Ϊ uint16 ����

#define MMIMAIL_MAX_MEMSTATE_LABEL_LEN      (50)

#define MMIMAIL_FULLSCREEN_CLIENT_LEFT      0 
#define MMIMAIL_FULLSCREEN_CLIENT_TOP       MMI_TITLE_HEIGHT
#define MMIMAIL_FULLSCREEN_CLIENT_RIGHT     MMI_MAINSCREEN_RIGHT_MAX_PIXEL
#define MMIMAIL_FULLSCREEN_CLIENT_BOTTOM    MMI_CLIENT_RECT_BOTTOM

#define MMIMAIL_APPLY_MSG                   L"KTYX5"    //��ͨ139�������
#define MMIMAIL_APPLY_NUM                   "10086"     //��ͨ139�������

#define MMIMAIL_LOGOUT_MSG                  L"QXYX5"    //ע��139�������

#define MMIMAIL_CHECK_MSG                   L"JCZH PT=TM001 VER=1.0B20100610"//�˺ż������
#define MMIMAIL_CHECK_NUM                   "10658172"  //�˺ż�����

/**----------------------------------------------------------------------*
 **                         TYPE AND STRUCT                              *
 **----------------------------------------------------------------------*/
//�����ʺ���Ϣ
typedef struct
{
    uint16  accountid;                              //�˻�id
    wchar   name[MMIMAIL_MAX_ACC_NAME_LEN + 1];     //�ʺ�����
    wchar   addr[MMIMAIL_MAX_ADDRESS_LEN + 1];      //�ʺŵ�ַ
    wchar   password[MMIMAIL_MAX_PASSWORD_LEN + 1]; //�ʺ�����
    wchar   addr_name[MMIMAIL_MAX_ADDRESS_LEN + 1]; //�û����ơ���ַ����
    
}MMIMAIL_ACCOUNT_INFO_T;

//�ʼ�״̬
typedef enum
{
    MMIMAIL_STATE_NULL,
    MMIMAIL_STATE_READ,              //�Ѷ�
    MMIMAIL_STATE_READ_WITH_ACC,     //�Ѷ��и���
    MMIMAIL_STATE_UNREAD,            //δ��
    MMIMAIL_STATE_UNREAD_WITH_ACC,   //δ���и���
    MMIMAIL_STATE_SERVER_REJECT,     //�������ܾ�
    MMIMAIL_STATE_SEND_FAIL,         //���Ͳ��ɹ�
    MMIMAIL_STATE_SENDING,           //���ڷ���
    MMIMAIL_STATE_WAIT_SEND,         //�ȴ�����
    MMIMAIL_STATE_SENT,              //�ѷ�����
    MMIMAIL_STATE_SENT_WITH_ACC,     //�ѷ������и���
    MMIMAIL_STATE_DRAFT,             //�ݸ���
    MMIMAIL_STATE_DRAFT_WITH_ACC,    //�ݸ����и���
    MMIMAIL_STATE_MAX
}MMIMAIL_STATE_E;

//�ʼ�ͼ��
typedef enum
{
    MMIMAIL_ICON_FROM,          //������
    MMIMAIL_ICON_TO,            //�ռ���
    MMIMAIL_ICON_CC,            //����
    MMIMAIL_ICON_BCC,           //����
    MMIMAIL_ICON_SUBJECT,       //����
    MMIMAIL_ICON_CONTENT,       //����
    MMIMAIL_ICON_ACCESSORY,     //����
    MMIMAIL_ICON_TIME,          //ʱ��
    MMIMAIL_ICON_ACCOUNT,       //�����ʺ�
    MMIMAIL_ICON_MAX
}MMIMAIL_ICON_ENUM_E;

//�����ʼ���Ҫ��Ϣ
typedef struct MMIMAIL_MAIL_SUMMERY_T_TAG
{
    uint32              record_id;                  //����
    uint32              time;                       //ʱ��
    MMIMAIL_BOX_TYPE_E  box_type;                   //��������
    MMIMAIL_STATE_E     state;                      //�ʼ�״̬
    BOOLEAN             is_marked;                  //�Ƿ���   
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT
    uint32               item_index;             //����list �ؼ�����
#endif

    wchar   addr[MMIMAIL_MAX_LIST_DISP_LEN + 1];    //�ʼ���ַ
    wchar   *name;                                  //��ϵ��������Ŀǰû��ʹ�ã��ʼ��б���ʾ������ϵ������
    wchar   subject[MMIMAIL_MAX_LIST_DISP_LEN + 1]; //����

    struct MMIMAIL_MAIL_SUMMERY_T_TAG *next_ptr;
    struct MMIMAIL_MAIL_SUMMERY_T_TAG *pre_ptr;
    
}MMIMAIL_MAIL_SUMMERY_T;

//���������Ϣ
typedef struct
{
    uint16  total_inbox_num;                    //�ռ������ʼ���
    uint16  total_outbox_num;                   //���������ʼ���
    uint16  total_sentbox_num;                  //�ѷ��ʼ�����
    uint16  total_draftbox_num;                 //�ݸ������ʼ���

    uint16  total_inbox_unread_num;             //δ���ʼ�����

    MMIMAIL_MAIL_SUMMERY_T *inbox_mail_list;     //�ռ����ʼ�����
    MMIMAIL_MAIL_SUMMERY_T *outbox_mail_list;    //�������ʼ�����
    MMIMAIL_MAIL_SUMMERY_T *sentbox_mail_list;   //�ѷ��ʼ�����
    MMIMAIL_MAIL_SUMMERY_T *draftbox_mail_list;  //�ݸ����ʼ�����
    
}MMIMAIL_BOX_INFO_T;

//��������
typedef enum
{
    MMIMAIL_OPER_NULL,                        //��״̬
    MMIMAIL_OPER_OPEN_READ_WIN,     //���ʼ��Ķ�����
    MMIMAIL_OPER_OPEN_EDIT_WIN,     //���ʼ��༭����
    MMIMAIL_OPER_OPEN_POPMENU       //�򿪵����˵�
    
}MMIMAIL_OPERATION_T;

//����ʱ��ö��
typedef enum
{
    MMIMAIL_EDIT_SILENT_START_TIME, //��ʼʱ��
    MMIMAIL_EDIT_SILENT_END_TIME   //����ʱ��
    
}MMIMAIL_EDIT_SILENT_TIME_TYPE_E;

//������������Ϣ
typedef struct
{
    BOOLEAN                 is_have_account;        //�Ƿ��п�ͨ�ı����ʺ�
    BOOLEAN                 is_ready;               //�����Ƿ�׼����
    MAIL_ENTER_TYPE_E       enter_type;             //������������
    DPARAM                  enter_param;            //��������

    MMIMAIL_ACCOUNT_INFO_T  def_account;            //Ĭ���ʺ�
    MMIMAIL_BOX_INFO_T      def_account_box_info;   //Ĭ���ʺ�������Ϣ

    MMIMAIL_OPERATION_T     ok_operation;           //��ok���Ĳ�������
    
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_editwin_type;  //�򿪱༭���ڵ����ͣ������½���ת�����ظ��Ȳ�������
    
    MMIMAIL_BOX_TYPE_E      cur_box_type;           //��ǰ���ڲ�������������
    MMIMAIL_MAIL_SUMMERY_T  *cur_oper_mail;         //��ǰ���ڲ������ʼ���������Ҫ��Ϣ�������б���ʾ
    MAIL_MESSAGE_T          *cur_disp_mail;         //��ǰ������ʾ���ʼ��������ʼ�����
    uint32                  cur_disp_record_id;     //��ǰ������ʾ���ʼ� record_id
    BOOLEAN                 is_mail_sendby_id;      //��ǵ�ǰ�ʼ�����ͨ��id����
    
    uint32                  marked_mail_num;        //��ǵ��ʼ�����
    uint8                   mainmenu_win_timer_id;  //�������˵���ʱˢ�¶�ʱ��

    uint16                  cur_sel_account_idx;    //��ǰѡ����ʺ�index
    MMIMAIL_ACCOUNT_INFO_T  cur_edit_account;       //��ǰ���ڱ༭��account��Ϣ��������ʱ�༭����
    SCI_TIME_T              cur_edit_silent_start_time; //��ǰ���ڱ༭�ľ���ʱ�εĿ�ʼʱ��
    SCI_TIME_T              cur_edit_silent_end_time;   //��ǰ���ڱ༭�ľ���ʱ�εĽ���ʱ��
    MMIMAIL_EDIT_SILENT_TIME_TYPE_E cur_edit_time_type; //��ǰ���ڱ༭��ʱ������

    BOOLEAN                 is_account_num_change;  //�ʺ���Ŀ�Ƿ�仯
    BOOLEAN                 is_account_name_change; //�ʺ������Ƿ�仯
    
    BOOLEAN                 is_def_account_change;  //Ĭ���ʺ��Ƿ�仯

    BOOLEAN                 is_logout_oper;         //��ǰ�Ƿ�Ϊע������

    BOOLEAN                 is_del_account_oper_by_bomb;         //��ǰ�Ƿ�Ϊը����Ϣ������ɾ���˻�����
    
    BOOLEAN                 is_need_sort;           //�Ƿ���Ҫ��������
    
}MMIMAIL_MANAGE_INFO_T;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DECLARATION                              *
 **---------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif
#endif

