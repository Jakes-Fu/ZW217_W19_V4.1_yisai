/****************************************************************************
** File Name:      mail_edit.c                                            *
** Author:                                                                    *
** Date:           11/10/2010                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:                                                               *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2010      Jiaoyou.wu        Create
** 
****************************************************************************/
#define _MAIL_EDIT_C_  

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_push_mail_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "window_parse.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmi_text.h"
#include "mmidisplay_data.h"
#include "mmimail_internal.h"
#include "mmimail_wintab.h"
#include "mmimail_read.h"
#include "mmimail_text.h"
#include "mmimail_image.h"
#include "mmimail_id.h"
#include "mmimail_menutable.h"
#include "guitab.h"
#include "guilistbox.h"
#include "guirichtext.h"
#include "mmipub.h"
#include "mmi_appmsg.h"
#include "mmipb_text.h"
#include "mmipb_export.h"
#include "mmifmm_export.h"
#include "mmiudisk_export.h"
#include "mail_main.h"
#include "mail_setting.h"
#include "mail_util.h"
#include "mail_mem.h"
#include "mail_fldr.h"
#include "mail_file.h"
//#include "mmipb_wintab.h" 
//#include "mmiset.h"//for ����ģʽ�ж�MS00217230 liangshun20110105
#include "mmiset_export.h"
#include "mail_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIMAIL_REPLY_SUFFIX        L"Re:"  //�ظ��ʼ�����ǰ׺
#define MMIMAIL_FORWARD_SUFFIX      L"Fw:"  //ת���ʼ�����ǰ׺
#define MMIMAIL_CONTENT_SEPERATOR   L"\n----------------------------------\n" //�ʼ����ķָ���
#define MMIMAIL_SIGN_SEPERATOR      L"\n\n" //�ʼ�ǩ���ָ���
//�ʼ�����Ϊ��ʱĬ����ʾ����
#ifdef MAINLCD_SIZE_240X320
#define MMIMAIL_EMPTY_CONTENT       L"\n\n\n\n" 
#elif defined MAINLCD_SIZE_240X400
#define MMIMAIL_EMPTY_CONTENT       L"\n\n\n\n\n\n\n"
#else
#define MMIMAIL_EMPTY_CONTENT       L"\n\n\n\n" 
#endif

#define MMIMAIL_REPLY_SUFFIX1        L"RE:"  //�ظ��ʼ�����ǰ׺
#define MMIMAIL_REPLY_SUFFIX2        {0x7b54,0x590d,0x003a,0}  //�ظ��ʼ�����ǰ׺ ��: Ӣ��:
#define MMIMAIL_REPLY_SUFFIX3        {0x7b54,0x590d,0xff1a,0}  //�ظ��ʼ�����ǰ׺ ��: ����:

//����ռ�
#define MMIMAIL_ALLOC(ptr, size) \
do\
{\
    ptr = comm_alloc((size));\
    if (PNULL != ptr)\
    {\
        SCI_MEMSET(ptr, 0, (size));\
    }\
    else\
    {\
        SCI_PASSERT(SCI_FALSE, ("MMIMAIL==> malloc sz : %d fail!", (size)));/*assert to do*/\
    }\
} while(0)

//����uint8 *������
#define MMIMAIL_COPY_STR(dest, src, dest_max) \
do\
{\
    SCI_ASSERT(PNULL != dest);/*assert to do*/\
    if (PNULL != src)\
    {\
        uint32 copy_len = 0;\
        copy_len = SCI_STRLEN((const char *)src);\
        copy_len = MIN(copy_len, (dest_max));\
        SCI_MEMSET(dest, 0, copy_len);\
        strncpy((char *)dest, (const char *)src, copy_len);\
    }\
} while(0)

//����MMI_STRING_T��������
#define MMIMAIL_COPY_MMI_STRING_T(dest, src, dest_max) \
do\
{\
    SCI_ASSERT(PNULL != dest.wstr_ptr);/*assert to do*/\
    if ((src.wstr_len > 0) && (PNULL != src.wstr_ptr))\
    {\
        uint32 copy_len = 0;\
        SCI_MEMSET(dest.wstr_ptr, 0, (dest_max) * sizeof(wchar));\
        copy_len = MIN(src.wstr_len, (dest_max));\
        MMIAPICOM_Wstrncpy(dest.wstr_ptr, (const wchar *)src.wstr_ptr, copy_len);\
        dest.wstr_len = (uint16)copy_len;\
    }\
} while(0)

//����MAIL_ADDRESS_T * ������
#define MMIMAIL_COPY_MAIL_ADDR_T(dest, dest_count, src, src_count, i) \
do\
{\
    SCI_ASSERT(PNULL != dest);/*assert to do*/\
    if ((src_count) > 0)\
    {\
        (dest_count) = 0;\
        for (i = 0; i < (src_count); i++)\
        {\
            SCI_ASSERT(PNULL != dest[i]);/*assert to do*/\
            MMIMAIL_COPY_STR(dest[i]->email, src[i]->email, MMIMAIL_MAX_ADDRESS_LEN);\
            MMIMAIL_COPY_MMI_STRING_T(dest[i]->name, src[i]->name, MMIMAIL_MAX_NAME_LEN);\
            (dest_count)++;\
        }\
    }\
} while(0)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//richtext itemö��
typedef enum
{
    MMIMAIL_RICHTEXT_TO,
    MMIMAIL_RICHTEXT_CC,
    MMIMAIL_RICHTEXT_BCC,
    MMIMAIL_RICHTEXT_SUBJECT,
    MMIMAIL_RICHTEXT_ACCESSORY,
    MMIMAIL_RICHTEXT_CONTENT,

    MMIMAIL_RICHTEXT_MAX
}MMIMAIL_RICHTEXT_ITEM_INDEX_E; 

//��richtext���еĲ�������
typedef enum
{
    MMIMAIL_RICHTEXT_ADD_ITEM,  //richedit add new item
    MMIMAIL_RICHTEXT_MOD_ITEM,  //richedit mod item

    MMIMAIL_RICHTEXT_OP_MAX
}MMIMAIL_RICHTEXT_OP_TYPE;  

//�༭����ѡ��˵�
typedef enum
{
    MMIMAIL_NODE_EDIT_ACC_OPT_NULL,
    MMIMAIL_NODE_EDIT_ACC_OPT_ADD,
    MMIMAIL_NODE_EDIT_ACC_OPT_DEL,

    MMIMAIL_NODE_EDIT_ACC_OPT_MAX
}MMIMAIL_NODE_EDIT_ACC_OPT_E;

//�༭ѡ�����
typedef enum
{
    MMIMAIL_EDIT_OPER_NULL,
    MMIMAIL_EDIT_OPER_SEND,
    MMIMAIL_EDIT_OPER_SAVE,
    MMIMAIL_EDIT_OPER_CANCEL,

    MMIMAIL_EDIT_OPER_MAX
}MMIMAIL_EDIT_OPER_E;

//�ʼ��༭ȫ�ֱ����ṹ
typedef struct 
{
    uint32                          total_size;     //�ʼ��ܴ�С
    uint32                          total_acc_size; //�����ܴ�С
    uint32                          org_text_size;  //ԭ�ʼ����Ĵ�С
    uint32                          sign_text_size; //ǩ����С

    BOOLEAN                         is_other_free;  //�Ƿ��������߳��ͷ��ڴ�
    BOOLEAN                         is_inc_org_text;//�Ƿ����ԭ�ʼ����ģ�����ԭ���Ĺ����޷�ֱ�ӱ༭�����Ƿ���ʱ׷�ӵ��༭���ĺ���
    
    MMIMAIL_RICHTEXT_ITEM_INDEX_E   focus_index;    //��ǰ������
    MMIMAIL_EDIT_OPER_E             cur_oper;       //��¼��ǰѡ��˵�����
    MMIMAIL_OPEN_EDIT_WIN_TYPE_E    open_type;      //�򿪱༭���ڵ�����
    
    MAIL_MESSAGE_T                  *editbody_ptr;  //�ʼ��༭����
    
}MMIMAIL_EDIT_GLOBAL_VARIABLE_T;

//�༭�ʼ�������Ϣ
typedef struct
{
    wchar               full_name[MMIFILE_FILE_NAME_MAX_LEN+1];
    uint16              full_name_len;

    wchar               file_name[MMIFILE_FILE_NAME_MAX_LEN+1];
    uint16              file_name_len;

    uint16              file_size;
}MMIMAIL_EDIT_FILE_INFO_T;

//��MMIMAIL_RICHTEXT_ITEM_INDEX_E����˳����ͬ
LOCAL MMIMAIL_RICHTEXT_ITEM_INDEX_E     s_mail_edit_richtext_item_type[MMIMAIL_RICHTEXT_MAX] = 
{
    MMIMAIL_RICHTEXT_TO,
    MMIMAIL_RICHTEXT_CC,
    MMIMAIL_RICHTEXT_BCC,
    MMIMAIL_RICHTEXT_SUBJECT,
    MMIMAIL_RICHTEXT_ACCESSORY,
    MMIMAIL_RICHTEXT_CONTENT    
};

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL MMIMAIL_EDIT_GLOBAL_VARIABLE_T    s_mail_edit_global = {0};
LOCAL MAIL_MESSAGE_PART_T       *s_cur_edit_acc = PNULL;    //��ǰ���ڱ༭�ĸ���
LOCAL MMIMAIL_EDIT_FILE_INFO_T   s_init_acc_info = {0};     //��ʼ��������Ϣ�����ļ����ñ༭�ʼ�����ʱʹ��

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/ 

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//	Description : �༭�ʼ���ʼ��
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_EditInit(
                                MAIL_MESSAGE_T * editbody_ptr,      //�༭�ʼ���ṹ
                                MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                                );

/*****************************************************************************/
//	Description : �򿪱༭�ʼ�������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CreateEditWin(void);

/*****************************************************************************/
//  Description : ����Ƿ���Խ����ʼ��༭ģ��
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL MMIMAIL_ERROR_E MMIMAIL_EnterEditCheck(void);

/*****************************************************************************/
//  Description : ���ݲ�ͬ����������ʾ�û�
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EditMailAlertWin(MMIMAIL_ERROR_E mail_error_e);

/*****************************************************************************/
//  Description : ��ȡ�ʼ��༭���ڵĴ�����
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL MMIMAIL_OPEN_EDIT_WIN_TYPE_E MMIMAIL_GetEditWinOpenType(void);

/*****************************************************************************/
//  Description : �����ʼ��༭���ڵĴ�����
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetEditWinOpenType(MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type);

/*****************************************************************************/
//	Description : �����ʼ����ݸ���
//	Global resource dependence : none
//	Author:yuanl
//	Note: TRUE: ���ڱ����ʼ�  FALSE: ����Ҫ����
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_SaveMail(MAIL_MESSAGE_T *editbody_ptr);

/*****************************************************************************/
//	Description : Is Mail empty
//	Global resource dependence : none
//	Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsEmpty(MAIL_MESSAGE_T *editbody_ptr);

/*****************************************************************************/
//	Description : ����MAIL_MESSAGE_T *�ṹ�壬�����в���ָ���漰���ڴ�
//	Global resource dependence : none
//	Author: yuanl
//	Note:Ԥ�����˷����ˡ��ռ��ˡ����͡����͡����⼰���ĵĿռ�
/*****************************************************************************/
LOCAL MAIL_MESSAGE_T * CreateMailEditBody(void);

/*****************************************************************************/
//	Description : ������ CreateMailEditBody ������ڴ�
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void DestroyMailEditBody(MAIL_MESSAGE_T *editbody_ptr);

/*****************************************************************************/
//	Description : ����ʼ�ǩ��
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailSign(MMI_STRING_T *content_ptr);

/*****************************************************************************/
//	Description : ���ԭ�ʼ�����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailOrgText(
                            MMI_STRING_T *dest_content_ptr,
                            MMI_STRING_T *src_content_ptr
                            );

/*****************************************************************************/
//	Description : ����ԭʼ�ʼ�����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void CopyMailAcc(
                            MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                            MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                            );

/*****************************************************************************/
//	Description : �½��ʼ�
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void NewMail(
                    MAIL_MESSAGE_T * edit_mail_ptr     //�༭�ʼ���ṹ
                    );

/*****************************************************************************/
//	Description : ����ԭʼ�ʼ����༭�ʼ��ṹ��
//	Global resource dependence : none
//	Author:yuanl
//	Note:������Ҫ�����ڴ�
/*****************************************************************************/
LOCAL void CopyExistMail(
                            MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                            MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                            );

/*****************************************************************************/
//	Description : �ظ��ʼ�������ԭʼ�ʼ��������ݵ��༭�ʼ��ṹ��
//	Global resource dependence : none
//	Author: yuanl
//	Note:������Ҫ�����ڴ�
/*****************************************************************************/
LOCAL void ReplyMail(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        );
                        
/*****************************************************************************/
//	Description : �ظ�ȫ���ʼ�������ԭʼ�ʼ��������ݵ��༭�ʼ��ṹ��
//	Global resource dependence : none
//	Author: yuanl
//	Note:������Ҫ�����ڴ�
/*****************************************************************************/
LOCAL void ReplyAllMail(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        );

/*****************************************************************************/
//	Description : ת��ȫ�����������ĺ͸���
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void ForwardAll(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        );

/*****************************************************************************/
//	Description : ת������
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void ForwardContent(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        );
                        
/*****************************************************************************/
//	Description : ������ת������
//	Global resource dependence : none
//	Author: yuanl
//	Note:�����ɷ������Զ���ӣ����ر༭ʱ�����ԭ����
/*****************************************************************************/
LOCAL void ServerForwardAcc(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        );
                        
/*****************************************************************************/
//	Description : ������ת��ȫ�����������ĺ͸���
//	Global resource dependence : none
//	Author: yuanl
//	Note:�����ɷ������Զ���ӣ����ر༭ʱ�����ԭ����
/*****************************************************************************/
LOCAL void ServerForwardAll(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        );

/*****************************************************************************/
//  Description :��ȡ��ǰѡ��˵��Ĳ���
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMIMAIL_EDIT_OPER_E GetMailEditCurOper(void);

/*****************************************************************************/
//  Description :���õ�ǰѡ��˵�����
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL void SetMailEditCurOper(MMIMAIL_EDIT_OPER_E cur_oper);

/*****************************************************************************/
//  Description :�༭�����ڴ�����
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailEditMainWinMsg(
                                                 MMI_WIN_ID_T       win_id, 
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                               );
                                               
/*****************************************************************************/
//	Description : �ʼ��༭ѡ��˵�������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          );
                                          
/*****************************************************************************/
//	Description : �ȴ������ʼ����ڴ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSaveEditWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     );

/*****************************************************************************/
//	Description : �����д��ڴ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendEditWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     );

/*****************************************************************************/
//	Description : ͨ��id�����д��ڴ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendMailByIdWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     );

/*****************************************************************************/
//  Description :��д�ռ����б�����
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditRecverListWinMsg(
                                             MMI_WIN_ID_T       win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
                                             
/*****************************************************************************/
//	Description : �ı��༭���洦����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditTextWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             );
                                             
/*****************************************************************************/
//	Description : �༭�����б��ڴ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditAccListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      );
                                      
/*****************************************************************************/
// 	Description : �༭�����б����ѡ��˵�������
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditAccMenuOptWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               );

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
//�༭������
WINDOW_TABLE(MMIMAIL_EDIT_MAIN_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMailEditMainWinMsg),    
    WIN_ID(MMIMAIL_EDIT_MAIN_WIN_ID),
    WIN_TITLE(TXT_MAIL_NEW),
    WIN_SOFTKEY(STXT_OPTION, TXT_EDIT, STXT_RETURN),
    CREATE_RICHTEXT_CTRL(MMIMAIL_EDIT_RICHTEXT_CTRL_ID),
    END_WIN
};

//�༭����ѡ��˵�
WINDOW_TABLE(MMIMAIL_EDIT_OPT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleMailEditOptWinMsg),    
    WIN_ID(MMIMAIL_EDIT_OPT_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    CREATE_POPMENU_CTRL(MENU_MAIL_EDIT_OPT,MMIMAIL_EDIT_OPT_MENU_CTRL_ID),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

//�༭�ռ����б���
WINDOW_TABLE(MMIMAIL_EDIT_RECVER_LIST_TAB) = 
{
    WIN_TITLE(TXT_MAIL_TO),
    WIN_FUNC((uint32)HandleMailEditRecverListWinMsg),    
    WIN_ID(MMIMAIL_EDIT_RECVER_LIST_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, STXT_MAIN_PB, STXT_RETURN),
    CREATE_EDIT_LIST_CTRL(MMIMAIL_MAX_RECEIVER_NUM, MMIMAIL_MAX_ADDRESS_LEN, MMIMAIL_EDIT_RECVER_LIST_CTRL_ID),
    END_WIN
};

//�༭���ⴰ��
WINDOW_TABLE(MMIMAIL_EDIT_SUBJECT_TAB) = 
{
    WIN_TITLE(TXT_COMMON_SUBJECT),
    WIN_FUNC((uint32)HandleMailEditTextWinMsg),    
    WIN_ID(MMIMAIL_EDIT_SUBJECT_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIMAIL_MAX_SUBJECT_LEN, MMIMAIL_EDIT_TEXT_CTRL_ID),
    END_WIN
};

//�༭���Ĵ���
WINDOW_TABLE(MMIMAIL_EDIT_CONTENT_TAB) = 
{
    WIN_TITLE(TXT_MAIL_CONTENT),
    WIN_FUNC((uint32)HandleMailEditTextWinMsg),    
    WIN_ID(MMIMAIL_EDIT_CONTENT_WIN_ID),
    WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIMAIL_MAX_CONTENT_LEN,
                            MMIMAIL_EDIT_TEXT_CTRL_ID),
    END_WIN
};

//�༭�����б����
WINDOW_TABLE( MMIMAIL_EDIT_ACC_LIST_WIN_TAB ) = 
{
    WIN_FUNC((uint32) HandleEditAccListWinMsg),    
    WIN_ID( MMIMAIL_EDIT_ACC_LIST_WIN_ID ),
    WIN_TITLE(TXT_MAIL_ACCESSORY_LIST),
    CREATE_LISTBOX_CTRL(GUILIST_TEXTLIST_E, MMIMAIL_EDIT_ACC_LISTBOX_CTRL_ID),
    WIN_SOFTKEY(STXT_OPTION,TXT_NULL,STXT_RETURN),
    END_WIN
};

//�༭�����б����ѡ��˵�
WINDOW_TABLE( MMIMAIL_EDIT_ACC_OPT_MENU_WIN_TAB ) = 
{
    WIN_FUNC( (uint32)HandleEditAccMenuOptWinMsg),
    WIN_ID(MMIMAIL_EDIT_ACC_OPT_MENU_WIN_ID),
    WIN_STYLE( WS_HAS_TRANSPARENT),
    WIN_SOFTKEY(TXT_COMMON_OK,TXT_NULL,STXT_RETURN),
    END_WIN
};

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description : ���ʼ��༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddReciveContactCallback(MMIPB_HANDLE_T handle)
{
     MMK_SendMsg(MMIMAIL_EDIT_RECVER_LIST_WIN_ID, MMI_SELECT_CONTACT_CNF, handle);      
}

/*****************************************************************************/
//	Description : ���ʼ��༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_OpenEditWin(
                                                MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type, 
                                                MAIL_MESSAGE_T *whole_mail_ptr
                                               )
{
    BOOLEAN result = FALSE;
    MMIMAIL_ERROR_E edit_err = MMIMAIL_NO_ERROR;
    MAIL_MESSAGE_T *editbody_ptr = PNULL;

    edit_err = MMIMAIL_EnterEditCheck();
    
    if (MMIMAIL_NO_ERROR != edit_err)
    {
        MMIMAIL_EditMailAlertWin(edit_err);
        result = FALSE;
    }
    else
    {
        editbody_ptr = CreateMailEditBody();
        if (editbody_ptr)
        {
            if (PNULL != s_mail_edit_global.editbody_ptr)
            {
                //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_OpenEditWin: s_mail_edit_global.editbody_ptr is not NULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_616_112_2_18_2_48_6_197,(uint8*)"");
                DestroyMailEditBody(s_mail_edit_global.editbody_ptr);
                s_mail_edit_global.editbody_ptr = PNULL;
            }

            //��ʼ��ȫ�ֱ���
            SCI_MEMSET(&s_mail_edit_global, 0, sizeof(MMIMAIL_EDIT_GLOBAL_VARIABLE_T));
            
            s_mail_edit_global.editbody_ptr = editbody_ptr;
            
            MMIMAIL_SetEditWinOpenType(open_type);
            MMIMAIL_EditInit(editbody_ptr, whole_mail_ptr);
            MMIMAIL_CreateEditWin(); 
            result = TRUE;
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_OpenEditWin: malloc MAIL_MESSAGE_T fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_633_112_2_18_2_48_6_198,(uint8*)"");
            MMIMAIL_EditMailAlertWin(MMIMAIL_ERROR_FAIL);
            result = FALSE;
        }    
    } 

    return result;
}

/*****************************************************************************/
//	Description : �����ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SendMail(MAIL_MESSAGE_T *whole_mail_ptr)
{
    BOOLEAN     result = FALSE;
    uint16      acc_id = MMIMAIL_GetDefAccountId();
    char        *sender = mail_SenderAddr(acc_id);
    SCI_DATE_T  sys_date = {0};
    SCI_TIME_T  sys_time = {0};
    MMI_TM_T    send_time = {0};
    MAIL_MESSAGE_T * cur_disp_mail = MMIMAIL_GetCurDispMail();

    if (MMIAPIUDISK_UdiskIsRun()) //U��ʹ����,�޷�����
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }
        
    if (PNULL == whole_mail_ptr)
    {
        //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_SendMail: whole_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_664_112_2_18_2_48_6_199,(uint8*)"");
        return FALSE;
    }

    if (PNULL == sender)
    {
        //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_SendMail: sender is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_670_112_2_18_2_48_6_200,(uint8*)"");
        return FALSE;
    }
    
    //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_SendMail: Enter send mail! is_inc_org_text is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_674_112_2_18_2_48_7_201,(uint8*)"d",s_mail_edit_global.is_inc_org_text);
    
    //��ӷ�����
    whole_mail_ptr->from_count = 1;
    strcpy((char *)whole_mail_ptr->from[0]->email, sender);

    //��ӷ���ʱ��   
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);
    send_time.tm_year = sys_date.year;
    send_time.tm_mon = sys_date.mon;
    send_time.tm_mday = sys_date.mday;
    send_time.tm_wday = sys_date.wday;
    send_time.tm_hour = sys_time.hour;
    send_time.tm_min = sys_time.min;
    send_time.tm_sec = sys_time.sec;
    if (PNULL != whole_mail_ptr->date)
    {
        comm_free(whole_mail_ptr->date);
    }
    whole_mail_ptr->date = (uint8 *)mail_DateTimeToStr(&send_time);

    //����ԭ�ʼ����Ĺ�����޷�ֱ�ӱ༭���ʼ�Ҫ������ԭ�ʼ�����
    if (s_mail_edit_global.is_inc_org_text)
    {
        wchar   *new_text_ptr = PNULL;
        wchar   seper_str[] = MMIMAIL_CONTENT_SEPERATOR;
        uint32  seper_len = MMIAPICOM_Wstrlen(seper_str);
        uint32  len = 0;
        
        if (PNULL != cur_disp_mail)
        {
            len = whole_mail_ptr->textbody.wstr_len + cur_disp_mail->textbody.wstr_len + seper_len + 1;
            new_text_ptr = mail_realloc(whole_mail_ptr->textbody.wstr_ptr, 
                                        ((whole_mail_ptr->textbody.wstr_len) * sizeof(wchar)),
                                        (len * sizeof(wchar)));
            if (PNULL != new_text_ptr)
            {
                //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_SendMail: realloc success"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_712_112_2_18_2_48_7_202,(uint8*)"");
                whole_mail_ptr->textbody.wstr_ptr = new_text_ptr;
                AddMailOrgText(&(whole_mail_ptr->textbody), &(cur_disp_mail->textbody));
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_SendMail: realloc size [%d] fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_718_112_2_18_2_48_7_203,(uint8*)"d", (len * sizeof(wchar)));
            }
        }
    
        s_mail_edit_global.is_inc_org_text = FALSE;
    }
    
    //�����ʼ�
    if (MMIMAIL_IsHaveDestAddr(whole_mail_ptr))
    {
        if ((MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ACC == MMIMAIL_GetEditWinOpenType())
            || (MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ALL == MMIMAIL_GetEditWinOpenType()))
        {           
            if (PNULL != cur_disp_mail)
            {
                mail_sendMailStartSave(acc_id, whole_mail_ptr, (char *)cur_disp_mail->uidl);
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_SendMail: cur_disp_mail is NULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_737_112_2_18_2_48_7_204,(uint8*)"");
                mail_sendMailStartSave(acc_id, whole_mail_ptr, PNULL);
            }
        }
        else
        {
            //����Ƿ��͵Ĳݸ�����ʼ�����ɾ�����ʼ�
            if ((MMIMAIL_DRAFTBOX == MMIMAIL_GetCurBoxType())
                && (MMIMAIL_OPEN_TYPE_ORIGINAL == MMIMAIL_GetEditWinOpenType()))
            {
                deleteMail(MMIMAIL_GetCurDispMailRecordId());
            }

            mail_sendMailStartSave(acc_id, whole_mail_ptr, PNULL);
        }
        
        result = TRUE;
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_SendMail: no dest address"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_757_112_2_18_2_48_7_205,(uint8*)"");
        MMIMAIL_OpenMsgBox(TXT_MAIL_ADD_RECEIVER, IMAGE_PUBWIN_WARNING);
    }

    return result;
}

/*****************************************************************************/
//	Description : ����ָ��id���ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_SendMailById(uint32 record_id)
{
    MMI_WIN_ID_T send_win_id = MMIMAIL_SEND_WAITING_WIN_ID;
    if (MMIAPIUDISK_UdiskIsRun()) //U��ʹ����,�޷�����
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }

    if(MMIAPISET_GetFlyMode())//for ����ģʽ�ж�MS00217230 liangshun20110105
    {
        MMIMAIL_OpenMsgBox(TXT_MAIL_NOT_AVAILABLE, IMAGE_PUBWIN_WARNING);
        //δ�༭���ʼ�����Ҫ���浽�ݸ�����
        return FALSE;
    }

    //��139�����˳�������s_mmimail_info(MMIMAIL_Exit())
    //����ÿ�η����ʼ�ǰ��Ҫ���»�ȡĬ���˻�����Ϣ,
    //�����ʼ����ܻᷢ��ʧ�ܡ�
    MMIMAIL_GetDefAccount(MMIMAIL_GetDefAccountInfo());
    mail_sendMailSendByMsgId(MMIMAIL_GetDefAccountId(), record_id);
    MMIMAIL_SetSendingById(TRUE);
    
    // ��ʾ�����У�3���Զ��رգ�תΪ��̨����
    MMIPUB_OpenAlertWinByTextId(PNULL, 
                                TXT_MAIL_SENDING, 
                                TXT_NULL, 
                                IMAGE_PUBWIN_WARNING, 
                                &send_win_id, 
                                PNULL, 
                                MMIPUB_SOFTKEY_ONE, 
                                HandleSendMailByIdWaitingWinMsg);

    return TRUE;
}

/*****************************************************************************/
//	Description : ȡ������ָ��id���ʼ�
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMAIL_CancelSendById(uint32 record_id)
{

    //�ȴ�һ����ֹͣ
    SCI_Sleep(500);
    
    mail_CancelSendingManual();

    if(!mail_GetSendingState())
    {
        //�ñ��λ
        MMIMAIL_MAIL_SUMMERY_T * cur_mail_ptr = MMIMAIL_GetCurOperationMail();

        if (MMIMAIL_STATE_SENDING == MMIMAIL_GetMailState(cur_mail_ptr))
        {
            //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_CancelSendById: current mail id = [%d], state [%d] is valid!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_823_112_2_18_2_48_7_206,(uint8*)"dd", record_id, MMIMAIL_GetMailState(cur_mail_ptr));
            MMIMAIL_SetMailState(cur_mail_ptr, MMIMAIL_STATE_SEND_FAIL);
            MMIMAIL_OpenMsgBox(TXT_COMM_SEND_FAILED, IMAGE_PUBWIN_WARNING);
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> MMIAPIMAIL_CancelSendById: current mail id = [%d], state [%d] is invalid!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_829_112_2_18_2_48_7_207,(uint8*)"dd", record_id, MMIMAIL_GetMailState(cur_mail_ptr));
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : ���ļ������ʼ��༭����
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIMAIL_EditMailFromFile(
                                        wchar   *full_path_name, //ȫ·��
                                        uint16  full_path_len    //ȫ·������
                                        )
{
    MMIFILE_FILE_INFO_T file_info = {0};
    
    if (full_path_len == 0 || PNULL == full_path_name)
    {
        //SCI_TRACE_LOW:"MMIMail MMIAPIMAIL_EditMailFromFile full_path_len <= 0 OR full_path_name is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_849_112_2_18_2_48_7_208,(uint8*)"");
        return ;
    }

    MMIAPIFMM_GetFileInfoFormFullPath(full_path_name,full_path_len,&file_info);
    
    if(0 == file_info.file_size)
    {
        MMIMAIL_OpenMsgBox(TXT_EMPTY_FILE, IMAGE_PUBWIN_WARNING);
    }
    else if (file_info.file_size > MMIMAIL_MAX_ACC_SIZE)
    {
        MMIMAIL_OpenMsgBox(TXT_FILESIZE_TOOBIG, IMAGE_PUBWIN_WARNING);
    }
    else
    {
        if (MMIAPIMAIL_Enter(MAIL_ENTER_NEWMAIL, PNULL))
        {
            //��������ɹ����򱣴渽����Ϣ
            SCI_MEMSET(&s_init_acc_info, 0, sizeof(MMIMAIL_EDIT_FILE_INFO_T));
            
            SCI_MEMCPY(s_init_acc_info.full_name, full_path_name, full_path_len*sizeof(wchar));
            s_init_acc_info.full_name_len = full_path_len;
            
            SCI_MEMCPY(s_init_acc_info.file_name, file_info.file_name, file_info.file_name_len *sizeof(wchar));
            s_init_acc_info.file_name_len = file_info.file_name_len;

            s_init_acc_info.file_size = (uint16)file_info.file_size;
        }
    }
}

/*****************************************************************************/
//	Description : �����ʼ���������
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SaveFinished(BOOLEAN is_success)
{
    MMIMAIL_EDIT_OPER_E cur_oper_type = GetMailEditCurOper();
    
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_SaveFinished: current operate type is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_889_112_2_18_2_48_7_209,(uint8*)"d", cur_oper_type);
    
    if (MMIMAIL_EDIT_OPER_SAVE == cur_oper_type)
    {
        //�ر����ڱ�����ʾ��
        if (MMK_IsOpenWin(MMIMAIL_SAVE_MAIL_WAITING_WIN_ID))
        {
            MMK_CloseWin(MMIMAIL_SAVE_MAIL_WAITING_WIN_ID);
            
            //��ʾ�ѱ��浽�ݸ���
            if (is_success) 
            {
                MMIMAIL_OpenMsgBox(TXT_COMMON_MAIL_SAVED, IMAGE_PUBWIN_SUCCESS);
            }
            else
            {
                MMIMAIL_OpenMsgBox(TXT_MMI_SAVEFAILED, IMAGE_PUBWIN_WARNING);
            }
        }
        
        //�رձ༭����
        if (MMK_IsOpenWin(MMIMAIL_EDIT_MAIN_WIN_ID))
        {       
            MMK_CloseWin(MMIMAIL_EDIT_MAIN_WIN_ID);
        }
        
        //��յ�ǰ����
        SetMailEditCurOper(MMIMAIL_EDIT_OPER_NULL);        
    }

    if (((MAIL_ENTER_NEWMAIL == MMIMAIL_GetOpenType())
        || (MAIL_ENTER_REPLYMAIL == MMIMAIL_GetOpenType()))
        && (MMIMAIL_EDIT_OPER_SEND != cur_oper_type))
    {
        //ֱ�������༭���ڱ������ʼ����˳�����
        MMIAPIMAIL_Exit();
    }
}

/*****************************************************************************/
//	Description : �����ʼ���������
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_SendFinished(BOOLEAN is_success)
{
    if (MMIMAIL_EDIT_OPER_SEND == GetMailEditCurOper())
    {
        //�ر����ڷ�����ʾ��
        if (MMK_IsOpenWin(MMIMAIL_SEND_WAITING_WIN_ID))
        {
            MMK_CloseWin(MMIMAIL_SEND_WAITING_WIN_ID);
        }
        
        //�رձ༭����
        if (MMK_IsOpenWin(MMIMAIL_EDIT_MAIN_WIN_ID))
        {       
            MMK_CloseWin(MMIMAIL_EDIT_MAIN_WIN_ID);
        }

        //��յ�ǰ������ֻ�����һ���ʼ��ķ��ͽ��
        SetMailEditCurOper(MMIMAIL_EDIT_OPER_NULL); 
        
        //��������Ӧ����ʱ��ʾ���ͽ��
        if ((MMK_IsOpenWin(MMIMAIL_MAIN_MENU_WIN_ID))
            || (MMK_IsOpenWin(MMIMAIL_MAILBOX_MAIN_WIN_ID))
            || (MMIMAIL_IsSmsOpen())) //�ڶ�����ʱ��ʾ���ͽ��
        {
            //��ʾ���ͳɹ���ʧ��
            if (is_success)
            {
                //COMM_TRACE:"MMIMAIL_SendFinished Mail sent succeeded!---MMIMAIL_EDIT_OPER_SEND"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_959_112_2_18_2_48_7_210,(uint8*)"");
                MMIMAIL_OpenMsgBox(TXT_COMM_SEND_SUCCEED, IMAGE_PUBWIN_SUCCESS);
            }
            else
            {
                //COMM_TRACE:"MMIMAIL_SendFinished Mail sent failed!---MMIMAIL_EDIT_OPER_SEND"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_964_112_2_18_2_48_7_211,(uint8*)"");
                MMIMAIL_OpenMsgBox(TXT_COMM_SEND_FAILED, IMAGE_PUBWIN_WARNING);
            }
        }
        else
        {
            //ֱ�������༭���ڷ������ʼ����˳�����
            MMIAPIMAIL_Exit();
        }
    }
    else if (MMIMAIL_IsSendingById())
    {
        //�ָ����λ
        MMIMAIL_SetSendingById(FALSE);
        
        //�ر����ڷ�����ʾ��
        if (MMK_IsOpenWin(MMIMAIL_SEND_WAITING_WIN_ID))
        {
            MMK_CloseWin(MMIMAIL_SEND_WAITING_WIN_ID);
        }
        
        //��������Ӧ����ʱ��ʾ���ͽ��
        if ((MMK_IsOpenWin(MMIMAIL_MAIN_MENU_WIN_ID))
            || (MMK_IsOpenWin(MMIMAIL_MAILBOX_MAIN_WIN_ID))
            || (MMIMAIL_IsSmsOpen()))//�ڶ�����ʱ��ʾ���ͽ��
        {
            //��ʾ���ͳɹ���ʧ��
            if (is_success)
            {
                //COMM_TRACE:"MMIMAIL_SendFinished Mail sent succeeded!---Sending by ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_993_112_2_18_2_48_7_212,(uint8*)"");
                MMIMAIL_OpenMsgBox(TXT_COMM_SEND_SUCCEED, IMAGE_PUBWIN_SUCCESS);
            }
            else
            {
                //COMM_TRACE:"MMIMAIL_SendFinished Mail sent failed!---Sending by ID"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_998_112_2_18_2_48_7_213,(uint8*)"");
                MMIMAIL_OpenMsgBox(TXT_COMM_SEND_FAILED, IMAGE_PUBWIN_WARNING);
            }
        }
        else
        {
            //ֱ�������༭���ڷ������ʼ����˳�����
            MMIAPIMAIL_Exit();
        }
    }

    //�رշ�����ѡ��˵�
    if (MMK_IsOpenWin(MMIMAIL_OUTBOX_MENU_OPT_WIN_ID))
    {
        MMK_CloseWin(MMIMAIL_OUTBOX_MENU_OPT_WIN_ID);
    }
    if (MMK_IsOpenWin(MMIMAIL_OUTBOX_MENU_OPT_EXT_WIN_ID))
    {
        MMK_CloseWin(MMIMAIL_OUTBOX_MENU_OPT_EXT_WIN_ID);
    }

    
}

/*****************************************************************************/
//	Description : ���� editbody ռ�õĿռ�
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
PUBLIC void MMIMAIL_DestroyEditBody(MAIL_MESSAGE_T * editbody_ptr)
{
    //COMM_TRACE:"MMIMail==> MMIMAIL_DestroyEditBody "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1028_112_2_18_2_48_7_214,(uint8*)"");
    DestroyMailEditBody(editbody_ptr);
}

/*****************************************************************************/
//	Description : �༭�ʼ���ʼ��
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_EditInit(
                                MAIL_MESSAGE_T * editbody_ptr,      //�༭�ʼ���ṹ
                                MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                                )
{
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail MMIMAIL_EditInit PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1043_112_2_18_2_48_7_215,(uint8*)"");
        return ;
    }
    
    //COMM_TRACE:"MMIMAIL==> MMIMAIL_EditInit: Open type is [%d]"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1047_112_2_18_2_48_7_216,(uint8*)"d", s_mail_edit_global.open_type);
    
    switch (s_mail_edit_global.open_type)
    {
        case MMIMAIL_OPEN_TYPE_DEFAULT:
        case MMIMAIL_OPEN_TYPE_NEW_MAIL:
            if (PNULL != browser_mail_ptr)
            {
                MMIMAIL_COPY_STR(editbody_ptr->to[0]->email, browser_mail_ptr, MMIMAIL_MAX_ADDRESS_LEN); /*lint !e506 !e774 !e666*/
                editbody_ptr->to_count ++;
            }
            //�½��ʼ�
            NewMail(editbody_ptr);
            break;

        case MMIMAIL_OPEN_TYPE_ORIGINAL:
        {
            MMIMAIL_MAIL_SUMMERY_T *cur_mail_ptr = MMIMAIL_GetCurOperationMail();

            if (PNULL != cur_mail_ptr)
            {
                //���浱ǰ���ڱ༭�ʼ���record id
                MMIMAIL_SetCurDispMailRecordId(cur_mail_ptr->record_id);
            }
            
            //�༭ԭʼ�ʼ�
            CopyExistMail(editbody_ptr, browser_mail_ptr);
            break;
        }
        
        case MMIMAIL_OPEN_TYPE_REPLY:
            //�ظ�������
            ReplyMail(editbody_ptr, browser_mail_ptr);
            break;

        case MMIMAIL_OPEN_TYPE_REPLY_ALL:
            //�ظ������˼�ȫ���ռ���
            ReplyAllMail(editbody_ptr, browser_mail_ptr);
            break;
            
        case MMIMAIL_OPEN_TYPE_FORWARD_ALL:
            //ת��ȫ��
            ForwardAll(editbody_ptr, browser_mail_ptr);
            break;

        case MMIMAIL_OPEN_TYPE_FORWARD_CONTENT:
            //ת������
            ForwardContent(editbody_ptr, browser_mail_ptr);
            break;

        case MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ACC:
            //������ת������
            ServerForwardAcc(editbody_ptr, browser_mail_ptr);
            break;

        case MMIMAIL_OPEN_TYPE_SERVER_FORWARD_ALL:
            //������ת��ȫ��
            ServerForwardAll(editbody_ptr, browser_mail_ptr);
            break;

        default:
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_EditInit: Open type [%d] is invalid!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1108_112_2_18_2_48_7_217,(uint8*)"d", s_mail_edit_global.open_type);
            break;
    }    
}

/*****************************************************************************/
//	Description : �򿪱༭�ʼ�������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CreateEditWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_EDIT_MAIN_WIN_TAB,PNULL);
}

/*****************************************************************************/
//	Description : ���ռ��˱༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CreateToListEditWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_EDIT_RECVER_LIST_TAB,PNULL);
}

/*****************************************************************************/
//	Description : �򿪳����˱༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CreateCcListEditWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_EDIT_RECVER_LIST_TAB,PNULL);
}

/*****************************************************************************/
//	Description : �������˱༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CreateBccListEditWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_EDIT_RECVER_LIST_TAB,PNULL);
}

/*****************************************************************************/
//	Description : ������༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CreateSubjectEditWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_EDIT_SUBJECT_TAB,PNULL);
}

/*****************************************************************************/
//	Description : �����ı༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CreateContentEditWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_EDIT_CONTENT_TAB,PNULL);
}

/*****************************************************************************/
//	Description : �򿪸����༭����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void MMIMAIL_CreateAccListEditWin(void)
{
    MMK_CreateWin((uint32 *)MMIMAIL_EDIT_ACC_LIST_WIN_TAB,PNULL);
}

/*****************************************************************************/
//  Description : ����Ƿ���Խ����ʼ��༭ģ��
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL MMIMAIL_ERROR_E MMIMAIL_EnterEditCheck(void)
{
    MMIMAIL_ERROR_E result_val = MMIMAIL_NO_ERROR;

    if(!MMIMAIL_IsReady()) //�ʼ�δ��ʼ�����
    {
        result_val = MMIMAIL_ERROR_NOT_INIT;
    }
    else if(MMIAPIUDISK_UdiskIsRun()) //U��ʹ����
    {
        result_val = MMIMAIL_ERROR_UDISK_USING;
    }
    else if (mail_IsMailBoxFull()) //�ʼ���
    {
        result_val = MMIMAIL_ERROR_NO_SPACE;
    }
    else if (0 == mail_FileFreeSpace()) //U��ʣ��ռ䲻��
    {
        result_val = MMIMAIL_ERROR_NO_MEMORY;
    }
    
    return result_val;
}

/*****************************************************************************/
//  Description : ���ݲ�ͬ����������ʾ�û�
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_EditMailAlertWin(MMIMAIL_ERROR_E mail_error_e)
{
    switch(mail_error_e)
    {
        case MMIMAIL_ERROR_NOT_INIT: //�ʼ�δ��ʼ�����
            MMIMAIL_OpenMsgBox(TXT_MAIL_NOT_INIT, IMAGE_PUBWIN_WARNING);
        	break;
            
        case MMIMAIL_ERROR_UDISK_USING: //U��ʹ����
            MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
        	break;
            
        case MMIMAIL_ERROR_INVALID_SIZE: //�ʼ���С��Ч
            MMIMAIL_OpenMsgBox(TXT_MAIL_INVALID_SIZE, IMAGE_PUBWIN_WARNING);
        	break;
            
        case MMIMAIL_ERROR_NO_MEMORY: //U��ʣ��ռ䲻��
            MMIMAIL_OpenMsgBox(TXT_MAIL_NO_MEMORY, IMAGE_PUBWIN_WARNING);
        	break;
            
        case MMIMAIL_ERROR_SFS_ERROR: //�ļ�ϵͳ������ʾʧ��
        case MMIMAIL_ERROR_ENCODE_ERROR: //���������ʾʧ��
        case MMIMAIL_ERROR_FAIL: //ʧ��
        case MMIMAIL_ERROR_FATAL:
            MMIMAIL_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        	break;
            
        case MMIMAIL_ERROR_SENDRECVING: //ʧ��
            MMIMAIL_OpenMsgBox(TXT_ERROR, IMAGE_PUBWIN_WARNING);
        	break;
            
        case MMIMAIL_ERROR_NO_SPACE: //�ʼ���
            MMIMAIL_OpenMsgBox(TXT_MAIL_FULLSPACE, IMAGE_PUBWIN_WARNING);
            break;
            
        default:
            break;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : ��ȡ�ʼ��༭���ڵĴ�����
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL MMIMAIL_OPEN_EDIT_WIN_TYPE_E MMIMAIL_GetEditWinOpenType(void)
{
    return s_mail_edit_global.open_type;
}

/*****************************************************************************/
//  Description : �����ʼ��༭���ڵĴ�����
//  Global resource dependence : none
//  Author: yuanl
//  Note:
/*****************************************************************************/
LOCAL void MMIMAIL_SetEditWinOpenType(MMIMAIL_OPEN_EDIT_WIN_TYPE_E open_type)
{
    s_mail_edit_global.open_type = open_type;
}

/*****************************************************************************/
//	Description : �����ʼ����ݸ���
//	Global resource dependence : none
//	Author: yuanl
//	Note: TRUE: ���ڱ����ʼ�  FALSE: ����Ҫ����
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_SaveMail(MAIL_MESSAGE_T *editbody_ptr)
{
    BOOLEAN result = FALSE;
    SCI_DATE_T  sys_date = {0};
    SCI_TIME_T  sys_time = {0};
    MMI_TM_T    edit_time = {0};
    uint16      acc_id = MMIMAIL_GetDefAccountId();
    char        *sender = mail_SenderAddr(acc_id);

    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail MMIMAIL_SaveMail PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1283_112_2_18_2_48_8_218,(uint8*)"");
        return FALSE;
    }

    if (MMIAPIUDISK_UdiskIsRun()) //U��ʹ����,�޷�����
    {
        MMIMAIL_OpenMsgBox(TXT_COMMON_UDISK_USING, IMAGE_PUBWIN_WARNING);
        return FALSE;
    }
    
    if (!MMIMAIL_IsEmpty(editbody_ptr))
    {
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_SaveMail: Enter save mail! is_inc_org_text is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1295_112_2_18_2_48_8_219,(uint8*)"d",s_mail_edit_global.is_inc_org_text);

        if (PNULL != sender)
        {
            //��ӷ�����
            editbody_ptr->from_count = 1;
            strcpy((char *)editbody_ptr->from[0]->email, sender);
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> MMIMAIL_SaveMail: sender is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1305_112_2_18_2_48_8_220,(uint8*)"");
        }
        
        //��ӱ���ʱ��   
        TM_GetSysDate(&sys_date);
        TM_GetSysTime(&sys_time);
        edit_time.tm_year = sys_date.year;
        edit_time.tm_mon = sys_date.mon;
        edit_time.tm_mday = sys_date.mday;
        edit_time.tm_wday = sys_date.wday;
        edit_time.tm_hour = sys_time.hour;
        edit_time.tm_min = sys_time.min;
        edit_time.tm_sec = sys_time.sec;
        if (PNULL != editbody_ptr->date)
        {
            comm_free(editbody_ptr->date);
        }
        editbody_ptr->date = (uint8 *)mail_DateTimeToStr(&edit_time);
    
        //����ԭ�ʼ����Ĺ�����޷�ֱ�ӱ༭���ʼ�Ҫ������ԭ�ʼ�����
        if (s_mail_edit_global.is_inc_org_text)
        {
            wchar   *new_text_ptr = PNULL;
            wchar   seper_str[] = MMIMAIL_CONTENT_SEPERATOR;
            uint32  seper_len = MMIAPICOM_Wstrlen(seper_str);
            uint32  len = 0;
            MAIL_MESSAGE_T * cur_disp_mail = MMIMAIL_GetCurDispMail();

            if (PNULL != cur_disp_mail)
            {
                len = editbody_ptr->textbody.wstr_len + cur_disp_mail->textbody.wstr_len + seper_len + 1;
                if (len <= MMIMAIL_MAX_CONTENT_SIZE) //����ԭ�ʼ����ĵĳ��Ȳ�����uint16�ܱ�ʾ����󳤶ȣ��򿽱�ԭ�ʼ�����
                {
                    new_text_ptr = mail_realloc(editbody_ptr->textbody.wstr_ptr, 
                                                ((editbody_ptr->textbody.wstr_len) * sizeof(wchar)),
                                                (len * sizeof(wchar)));
                    if (PNULL != new_text_ptr)
                    {
                        //COMM_TRACE:"MMIMAIL==> MMIMAIL_SaveMail: realloc success"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1343_112_2_18_2_48_8_221,(uint8*)"");
                        editbody_ptr->textbody.wstr_ptr = new_text_ptr;
                        AddMailOrgText(&(editbody_ptr->textbody), &(cur_disp_mail->textbody));
                    }
                    else
                    {
                        //COMM_TRACE:"MMIMAIL==> MMIMAIL_SaveMail: realloc size [%d] fail"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1349_112_2_18_2_48_8_222,(uint8*)"d", (len * sizeof(wchar)));
                    }
                }
                else
                {
                    //COMM_TRACE:"MMIMAIL==> MMIMAIL_SaveMail: new content size [%d] include org content is larger than 65535"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1354_112_2_18_2_48_8_223,(uint8*)"d", (len * sizeof(wchar)));
                }
            }
        
            s_mail_edit_global.is_inc_org_text = FALSE;
        }

        //����Ǳ༭�Ĳݸ�����ʼ����������ʼ���ɾ�����ʼ�
        if ((MMIMAIL_DRAFTBOX == MMIMAIL_GetCurBoxType())
            && (MMIMAIL_OPEN_TYPE_ORIGINAL == MMIMAIL_GetEditWinOpenType()))
        {
#ifdef PUSH_EMAIL_3_IN_1_SUPPORT//3 in 1 support     
            if(mail_GetMailLockFlag(MMIMAIL_GetCurDispMailRecordId()))
            {
                //�����ǰ�༭�ʼ�����������Ҫ��¼������־λ��
               mail_SetCurMailLockFlag(TRUE);
            }
            else
            {
                mail_SetCurMailLockFlag(FALSE);
            }
#endif
            deleteMail(MMIMAIL_GetCurDispMailRecordId());
        }
        
        //�ʼ���Ϊ�գ��򱣴��ʼ����ݸ���
        mail_saveMailToDrafts(MMIMAIL_GetDefAccountId(), editbody_ptr);
        result = TRUE;
    }
    else
    {
        //�ʼ�Ϊ�գ�����Ҫ����
        //COMM_TRACE:"MMIMAIL==> MMIMAIL_SaveMail: mail is empty, need not save!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1386_112_2_18_2_48_8_224,(uint8*)"");
    }

    return result;
}

/*****************************************************************************/
//	Description : ����MAIL_MESSAGE_T *�ṹ�壬�����в���ָ���漰���ڴ�
//	Global resource dependence : none
//	Author: yuanl
//	Note:Ԥ�����˷����ˡ��ռ��ˡ����͡����͡����⼰���ĵĿռ�
/*****************************************************************************/
LOCAL MAIL_MESSAGE_T * CreateMailEditBody(void)
{
    MAIL_MESSAGE_T * mail_ptr = PNULL;
    uint16  i = 0;
    uint32  total_size = 0;

    //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc MAIL_MESSAGE_T"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1402_112_2_18_2_48_8_225,(uint8*)"");
    mail_ptr = comm_alloc(sizeof(MAIL_MESSAGE_T));
    if (PNULL != mail_ptr)
    {
        total_size += sizeof(MAIL_MESSAGE_T);
        SCI_MEMSET(mail_ptr, 0, sizeof(MAIL_MESSAGE_T));
        
        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc from"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1409_112_2_18_2_48_8_226,(uint8*)"");
        MMIMAIL_ALLOC(mail_ptr->from[0], sizeof(MAIL_ADDRESS_T));
        total_size += sizeof(MAIL_ADDRESS_T);
        MMIMAIL_ALLOC(mail_ptr->from[0]->email, (MMIMAIL_MAX_ADDRESS_LEN + 1));
        total_size += MMIMAIL_MAX_ADDRESS_LEN + 1;
        MMIMAIL_ALLOC(mail_ptr->from[0]->name.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1)));
        total_size += sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1);
        
        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc to"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1417_112_2_18_2_48_8_227,(uint8*)"");
        for (i = 0; i < MMIMAIL_MAX_RECEIVER_NUM; i++)
        {
            MMIMAIL_ALLOC(mail_ptr->to[i], sizeof(MAIL_ADDRESS_T));
            total_size += sizeof(MAIL_ADDRESS_T);
            MMIMAIL_ALLOC(mail_ptr->to[i]->email, (MMIMAIL_MAX_ADDRESS_LEN + 1));
            total_size += MMIMAIL_MAX_ADDRESS_LEN + 1;
            MMIMAIL_ALLOC(mail_ptr->to[i]->name.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1)));
            total_size += sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1);
        }
        
        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc cc"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1428_112_2_18_2_48_8_228,(uint8*)"");
        for (i = 0; i < MMIMAIL_MAX_RECEIVER_NUM; i++)
        {
            MMIMAIL_ALLOC(mail_ptr->cc[i], sizeof(MAIL_ADDRESS_T));
            total_size += sizeof(MAIL_ADDRESS_T);
            MMIMAIL_ALLOC(mail_ptr->cc[i]->email, (MMIMAIL_MAX_ADDRESS_LEN + 1));
            total_size += MMIMAIL_MAX_ADDRESS_LEN + 1;
            MMIMAIL_ALLOC(mail_ptr->cc[i]->name.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1)));
            total_size += sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1);
        }

        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc bcc"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1439_112_2_18_2_48_8_229,(uint8*)"");
        for (i = 0; i < MMIMAIL_MAX_RECEIVER_NUM; i++)
        {
            MMIMAIL_ALLOC(mail_ptr->bcc[i], sizeof(MAIL_ADDRESS_T));
            total_size += sizeof(MAIL_ADDRESS_T);
            MMIMAIL_ALLOC(mail_ptr->bcc[i]->email, (MMIMAIL_MAX_ADDRESS_LEN + 1));
            total_size += MMIMAIL_MAX_ADDRESS_LEN + 1;
            MMIMAIL_ALLOC(mail_ptr->bcc[i]->name.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1)));
            total_size += sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1);
        }
        
#if 0//del by yuanl  2010.6.1 Ĭ�ϲ���λ reply to
        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc reply to"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1451_112_2_18_2_48_8_230,(uint8*)"");
        MMIMAIL_ALLOC(mail_ptr->replyto[0], sizeof(MAIL_ADDRESS_T));
        total_size += sizeof(MAIL_ADDRESS_T);
        MMIMAIL_ALLOC(mail_ptr->replyto[0]->email, (MMIMAIL_MAX_ADDRESS_LEN + 1));
        total_size += MMIMAIL_MAX_ADDRESS_LEN + 1;
        MMIMAIL_ALLOC(mail_ptr->replyto[0]->name.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1)));
        total_size += sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1);
#endif

        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc subject"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1460_112_2_18_2_48_8_231,(uint8*)"");
        MMIMAIL_ALLOC(mail_ptr->subject.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_SUBJECT_LEN + 1)));
        total_size += sizeof(wchar) * (MMIMAIL_MAX_SUBJECT_LEN + 1);
        
        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc content"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1464_112_2_18_2_48_8_232,(uint8*)"");
        MMIMAIL_ALLOC(mail_ptr->textbody.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_CONTENT_LEN + 1)));
        total_size += sizeof(wchar) * (MMIMAIL_MAX_CONTENT_LEN + 1);

        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: malloc acc"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1468_112_2_18_2_48_8_233,(uint8*)"");
        for (i = 0; i < MMIMAIL_MAX_ACC_NUM; i++)
        {
            MMIMAIL_ALLOC(mail_ptr->mailmessageparts[i], sizeof(MAIL_MESSAGE_PART_T));
            total_size += sizeof(MAIL_MESSAGE_PART_T);
            MMIMAIL_ALLOC(mail_ptr->mailmessageparts[i]->filename.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_FILENAME_LEN + 1)));
            total_size += (sizeof(wchar) * (MMIMAIL_MAX_FILENAME_LEN + 1));
            MMIMAIL_ALLOC(mail_ptr->mailmessageparts[i]->fullname.wstr_ptr, (sizeof(wchar) * (MMIMAIL_MAX_FULLNAME_LEN + 1)));
            total_size += (sizeof(wchar) * (MMIMAIL_MAX_FULLNAME_LEN + 1));
        }

        //COMM_TRACE:"MMIMAIL==> CreateMailEditBody: total malloc size = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1479_112_2_18_2_48_8_234,(uint8*)"d", total_size);
    }
    else
    {
        SCI_PASSERT(SCI_FALSE, ("MMIMAIL==> CreateMailEditBody: malloc MAIL_MESSAGE_T fail!"));/*assert to do*/
    }

    return mail_ptr;
}

/*****************************************************************************/
//	Description : ������ CreateMailEditBody ������ڴ�
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void DestroyMailEditBody(MAIL_MESSAGE_T *editbody_ptr)
{
    int i;

    //COMM_TRACE:"MMIMail==> DestroyMailEditBody"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1497_112_2_18_2_48_8_235,(uint8*)"");

    if(PNULL != editbody_ptr)
    {
        if(editbody_ptr->uidl)
        {
            comm_free(editbody_ptr->uidl);    
        }

        if(editbody_ptr->msgid)
        {
            comm_free(editbody_ptr->msgid);
        }       

        if(editbody_ptr->from[0])
        {
            if(editbody_ptr->from[0]->name.wstr_ptr)
            {
                comm_free(editbody_ptr->from[0]->name.wstr_ptr);
            }
            if(editbody_ptr->from[0]->email)
            {
                comm_free(editbody_ptr->from[0]->email);
            }
            comm_free(editbody_ptr->from[0]);
        }    

        for(i=0; i<MMIMAIL_MAX_RECEIVER_NUM; i++)
        {
            if(editbody_ptr->to[i])
            {
                if(editbody_ptr->to[i]->name.wstr_ptr)
                {
                    comm_free(editbody_ptr->to[i]->name.wstr_ptr);
                }
                if(editbody_ptr->to[i]->email)
                {
                    comm_free(editbody_ptr->to[i]->email);
                }
                comm_free(editbody_ptr->to[i]);
            }
        }    

        for(i=0; i<MMIMAIL_MAX_RECEIVER_NUM; i++)
        {
            if(editbody_ptr->cc[i])
            {
                if(editbody_ptr->cc[i]->name.wstr_ptr)
                {
                    comm_free(editbody_ptr->cc[i]->name.wstr_ptr);
                }
                if(editbody_ptr->cc[i]->email)
                {
                    comm_free(editbody_ptr->cc[i]->email);
                }
                comm_free(editbody_ptr->cc[i]);
            }
        }    

        for(i=0; i<MMIMAIL_MAX_RECEIVER_NUM; i++)
        {
            if(editbody_ptr->bcc[i])
            {
                if(editbody_ptr->bcc[i]->name.wstr_ptr)
                {
                    comm_free(editbody_ptr->bcc[i]->name.wstr_ptr);
                }
                if(editbody_ptr->bcc[i]->email)
                {
                    comm_free(editbody_ptr->bcc[i]->email);
                }
                 comm_free(editbody_ptr->bcc[i]);

            }
        }

        if(editbody_ptr->subject.wstr_ptr)
        {
            comm_free(editbody_ptr->subject.wstr_ptr);            
        }
        if(editbody_ptr->replyto[0])
        {
            if(editbody_ptr->replyto[0]->name.wstr_ptr)
            {
                comm_free(editbody_ptr->replyto[0]->name.wstr_ptr);
            }
            if(editbody_ptr->replyto[0]->email)
            {
                comm_free(editbody_ptr->replyto[0]->email);
            }
            comm_free(editbody_ptr->replyto[0]);
        }

        if(editbody_ptr->date)
        {
            comm_free(editbody_ptr->date);
        }
        if(editbody_ptr->mimevertion)
        {
            comm_free(editbody_ptr->mimevertion);
        }
        if(editbody_ptr->contenttype)
        {
            comm_free(editbody_ptr->contenttype);
        }
        if(editbody_ptr->priority)
        {
            comm_free(editbody_ptr->priority);
        }
        if(editbody_ptr->mailer)
        {
            comm_free(editbody_ptr->mailer);
        }
        if(editbody_ptr->boundary)
        {
            comm_free(editbody_ptr->boundary);
        }
        if(editbody_ptr->mailheader)
        {
            comm_free(editbody_ptr->mailheader);
        }
        if(editbody_ptr->mailbody)
        {
            comm_free(editbody_ptr->mailbody);
        }
        if(editbody_ptr->content)
        {
            comm_free(editbody_ptr->content);
        }
        if(editbody_ptr->textbody.wstr_ptr)
        {
            comm_free(editbody_ptr->textbody.wstr_ptr);
        }
        if(editbody_ptr->textcontent)
        {
            comm_free(editbody_ptr->textcontent);
        }
        if(editbody_ptr->htmlbody)
        {
            comm_free(editbody_ptr->htmlbody);
        }
                
        for(i=0; i<MMIMAIL_MAX_ACC_NUM; i++)
        {
            if(editbody_ptr->mailmessageparts[i])
            {
                if(editbody_ptr->mailmessageparts[i]->content)
                {
                    comm_free(editbody_ptr->mailmessageparts[i]->content);
                }
                if(editbody_ptr->mailmessageparts[i]->contenttype)
                {
                    comm_free(editbody_ptr->mailmessageparts[i]->contenttype);
                }
                if(editbody_ptr->mailmessageparts[i]->contentdisposiation)
                {
                    comm_free(editbody_ptr->mailmessageparts[i]->contentdisposiation);
                }
                if(editbody_ptr->mailmessageparts[i]->name.wstr_ptr)
                {
                    comm_free(editbody_ptr->mailmessageparts[i]->name.wstr_ptr);
                }
                if(editbody_ptr->mailmessageparts[i]->filename.wstr_ptr)
                {
                    comm_free(editbody_ptr->mailmessageparts[i]->filename.wstr_ptr);
                }
                if(editbody_ptr->mailmessageparts[i]->fullname.wstr_ptr)
                {
                    comm_free(editbody_ptr->mailmessageparts[i]->fullname.wstr_ptr);
                }                
                if(editbody_ptr->mailmessageparts[i]->contentid)
                {
                    comm_free(editbody_ptr->mailmessageparts[i]->contentid);
                }
                if(editbody_ptr->mailmessageparts[i]->contentlocation)
                {
                    comm_free(editbody_ptr->mailmessageparts[i]->contentlocation);
                }
                comm_free(editbody_ptr->mailmessageparts[i]);
            }
        }
        comm_free(editbody_ptr);/*lint !e774*/
    }
}

/*****************************************************************************/
//	Description : ����ʼ�ǩ��
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailSign(MMI_STRING_T *content_ptr)
{
    BOOLEAN is_add = FALSE;

    s_mail_edit_global.sign_text_size = 0;
    
    if ((PNULL == content_ptr) || (PNULL == content_ptr->wstr_ptr))
    {
        //COMM_TRACE:"MMIMAIL==> AddMailSign:content_ptr or content_ptr->wstr_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1694_112_2_18_2_48_9_236,(uint8*)"");
        return;
    }

    is_add = mail_GetAddSignature();
    if (is_add)
    {
        wchar   seper_str[] = MMIMAIL_SIGN_SEPERATOR;
        wchar  *sign_text = mail_GetSignature_text();
        uint32  len = 0;
        
        //COMM_TRACE:"MMIMAIL==> AddMailSign:mail_GetAddSignature return TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1705_112_2_18_2_48_9_237,(uint8*)"");
        if ((PNULL != sign_text)
            && ((len = MMIAPICOM_Wstrlen(sign_text)) > 0)
            && (content_ptr->wstr_len < MMIMAIL_MAX_CONTENT_LEN)
            && (len < (uint32)(MMIMAIL_MAX_CONTENT_LEN - content_ptr->wstr_len)))
        {
            len = MMIAPICOM_Wstrlen((const wchar *)seper_str);
            MMIAPICOM_Wstrncat(content_ptr->wstr_ptr, 
                                (const wchar *)seper_str, 
                                len);
            content_ptr->wstr_len += len;
            s_mail_edit_global.sign_text_size += len;

            len = MMIAPICOM_Wstrlen(sign_text);
            MMIAPICOM_Wstrncat(content_ptr->wstr_ptr, 
                                (const wchar *)sign_text, 
                                len);
            content_ptr->wstr_len += len;
            s_mail_edit_global.sign_text_size += len;
        }
    }

    return;
}

/*****************************************************************************/
//	Description : ���ԭ�ʼ�����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void AddMailOrgText(
                            MMI_STRING_T *dest_content_ptr,
                            MMI_STRING_T *src_content_ptr
                            )
{
    BOOLEAN is_inc = TRUE;
    
    s_mail_edit_global.org_text_size = 0;
    
    if ((PNULL == dest_content_ptr) || (PNULL == dest_content_ptr->wstr_ptr))
    {
        //COMM_TRACE:"MMIMAIL==> AddMailOrgText:dest_content_ptr or dest_content_ptr->wstr_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1745_112_2_18_2_48_9_238,(uint8*)"");
        return;
    }
    
    if ((PNULL == src_content_ptr) || (PNULL == src_content_ptr->wstr_ptr))
    {
        //COMM_TRACE:"MMIMAIL==> AddMailOrgText:src_content_ptr or src_content_ptr->wstr_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1751_112_2_18_2_48_9_239,(uint8*)"");
        return;
    }

    if ((MMIMAIL_OPEN_TYPE_REPLY == MMIMAIL_GetEditWinOpenType())
        || (MMIMAIL_OPEN_TYPE_REPLY_ALL == MMIMAIL_GetEditWinOpenType()))
    {    
        //�ظ��ʼ�ʱ��������������Ƿ����ԭ�ʼ�����
        is_inc = mail_GetReplyWithOriginalCont();
    }
    
    if (is_inc)
    {
        wchar   seper_str[] = MMIMAIL_CONTENT_SEPERATOR;
        uint32  len = 0;
        
        //COMM_TRACE:"MMIMAIL==> AddMailOrgText: mail_GetReplyWithOriginalCont return TRUE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1767_112_2_18_2_48_9_240,(uint8*)"");

        if ((src_content_ptr->wstr_len > (MMIMAIL_MAX_CONTENT_LEN - MMIMAIL_MIN_EDIT_CONTENT_LEN))
            && (!s_mail_edit_global.is_inc_org_text))
        {
            s_mail_edit_global.is_inc_org_text = TRUE;
        }
        else
        {
            len = MMIAPICOM_Wstrlen((const wchar *)seper_str);
            MMIAPICOM_Wstrncat(dest_content_ptr->wstr_ptr, 
                                (const wchar *)seper_str, 
                                len);
            dest_content_ptr->wstr_len += len;
            s_mail_edit_global.org_text_size += len;
            
            len = src_content_ptr->wstr_len;
            MMIAPICOM_Wstrncat(dest_content_ptr->wstr_ptr, 
                                (const wchar *)src_content_ptr->wstr_ptr, 
                                len);
            dest_content_ptr->wstr_len += len;
            s_mail_edit_global.org_text_size += len;
        }
    }    

    return;
}

/*****************************************************************************/
//	Description : ����ԭʼ�ʼ�����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void CopyMailAcc(
                            MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                            MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                            )
{
    uint16 i = 0;
    
    if(PNULL == edit_mail_ptr ||PNULL == browser_mail_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> CopyMailAcc:edit_mail_ptr OR browser_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1808_112_2_18_2_48_9_241,(uint8*)"");
        return;
    }
    //COMM_TRACE:"MMIMAIL==> CopyMailAcc: acc num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1811_112_2_18_2_48_9_242,(uint8*)"d", browser_mail_ptr->partcount);
    for (i = 0; i < browser_mail_ptr->partcount; i++)
    {
        if (i < MMIMAIL_MAX_ACC_NUM)
        {
            MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->mailmessageparts[i]->filename, 
                                        browser_mail_ptr->mailmessageparts[i]->filename,
                                        MMIMAIL_MAX_FILENAME_LEN);
            MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->mailmessageparts[i]->fullname, 
                                        browser_mail_ptr->mailmessageparts[i]->fullname,
                                        MMIMAIL_MAX_FULLNAME_LEN);
            edit_mail_ptr->mailmessageparts[i]->size = browser_mail_ptr->mailmessageparts[i]->size;
            edit_mail_ptr->partcount++;

            if ((PNULL != browser_mail_ptr->mailmessageparts[i]->content)
                && (browser_mail_ptr->mailmessageparts[i]->content_l > 0))
            {
                edit_mail_ptr->mailmessageparts[i]->content = comm_alloc(browser_mail_ptr->mailmessageparts[i]->content_l + 1);
                MMIMAIL_COPY_STR(edit_mail_ptr->mailmessageparts[i]->content, 
                                    browser_mail_ptr->mailmessageparts[i]->content,
                                    browser_mail_ptr->mailmessageparts[i]->content_l);
                edit_mail_ptr->mailmessageparts[i]->content_l = browser_mail_ptr->mailmessageparts[i]->content_l;
            }

            edit_mail_ptr->mailmessageparts[i]->attr_has_data = browser_mail_ptr->mailmessageparts[i]->attr_has_data;
        }
    }
}                            

/*****************************************************************************/
//	Description : �½��ʼ�
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void NewMail(
                    MAIL_MESSAGE_T * edit_mail_ptr     //�༭�ʼ���ṹ
                    )
{    
    if(PNULL == edit_mail_ptr)
    {
        //COMM_TRACE:"MMIMAIL==> NewMail:edit_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1850_112_2_18_2_48_9_243,(uint8*)"");
        return;
    }

    //��������ȷ���Ƿ����ǩ����Ϣ
    AddMailSign(&(edit_mail_ptr->textbody));
}

/*****************************************************************************/
//	Description : ����ԭʼ�ʼ����༭�ʼ��ṹ��
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void CopyExistMail(
                            MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                            MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                            )
{
    uint16 i = 0;
    uint32 count = 0;
    
    if(PNULL == edit_mail_ptr ||PNULL == browser_mail_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> CopyExistMail:edit_mail_ptr OR browser_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1872_112_2_18_2_48_9_244,(uint8*)"");
        return;
    }

    //����������
    count = MIN(browser_mail_ptr->from_count, 1);
    MMIMAIL_COPY_MAIL_ADDR_T(edit_mail_ptr->from, edit_mail_ptr->from_count, browser_mail_ptr->from, count, i);

    //�����ռ���
    count = MIN(browser_mail_ptr->to_count, MMIMAIL_MAX_RECEIVER_NUM);
    MMIMAIL_COPY_MAIL_ADDR_T(edit_mail_ptr->to, edit_mail_ptr->to_count, browser_mail_ptr->to, count, i);

    //��������
    count = MIN(browser_mail_ptr->cc_count, MMIMAIL_MAX_RECEIVER_NUM);
    MMIMAIL_COPY_MAIL_ADDR_T(edit_mail_ptr->cc, edit_mail_ptr->cc_count, browser_mail_ptr->cc, count, i);

    //��������
    count = MIN(browser_mail_ptr->bcc_count, MMIMAIL_MAX_RECEIVER_NUM);
    MMIMAIL_COPY_MAIL_ADDR_T(edit_mail_ptr->bcc, edit_mail_ptr->bcc_count, browser_mail_ptr->bcc, count, i);

    //��������
    MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->subject, browser_mail_ptr->subject, MMIMAIL_MAX_SUBJECT_LEN);

    //��������
    MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->textbody, browser_mail_ptr->textbody, MMIMAIL_MAX_CONTENT_LEN);

    //��������
    CopyMailAcc(edit_mail_ptr, browser_mail_ptr);
}

/*****************************************************************************/
//	Description : �ж��Ƿ����ָ����ǰ׺
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN IsIncludeAppoinedPrefix(MMI_STRING_T   content, wchar *prefix, uint16 prefix_len)
{
    BOOLEAN is_inc = FALSE;

    if ((prefix_len > 0)
        && (content.wstr_len >= prefix_len)
        && (PNULL != content.wstr_ptr)
        && (PNULL != prefix)
        && (MMIAPICOM_Wstrnstr((const wchar *)content.wstr_ptr, prefix_len, (const wchar *)prefix, prefix_len)))
    {
        is_inc = TRUE;
    }
    
    return is_inc;
}

/*****************************************************************************/
//	Description : �ظ��ʼ�������ԭʼ�ʼ��������ݵ��༭�ʼ��ṹ��
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void ReplyMail(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        )
{
    uint16  i = 0;
    uint32  count = 0;
    wchar   *temp_str = PNULL;
    wchar   reply_str[] = MMIMAIL_REPLY_SUFFIX;
    uint32  len = MMIAPICOM_Wstrlen((const wchar *)reply_str);
    wchar   reply_str1[] = MMIMAIL_REPLY_SUFFIX1;
    wchar   reply_str2[] = MMIMAIL_REPLY_SUFFIX2;
    wchar   reply_str3[] = MMIMAIL_REPLY_SUFFIX3;
    
    if(PNULL == edit_mail_ptr ||PNULL == browser_mail_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> ReplyMail:edit_mail_ptr OR browser_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1942_112_2_18_2_48_9_245,(uint8*)"");
        return;
    }

    if (browser_mail_ptr->replyto_count > 0)
    {
        //�����ظ���ַ���ռ�������
        count = MIN(browser_mail_ptr->replyto_count, 1);
        MMIMAIL_COPY_MAIL_ADDR_T(edit_mail_ptr->to, edit_mail_ptr->to_count, browser_mail_ptr->replyto, count, i);
    }
    else
    {
        //���������˵��ռ�������
        count = MIN(browser_mail_ptr->from_count, 1);
        MMIMAIL_COPY_MAIL_ADDR_T(edit_mail_ptr->to, edit_mail_ptr->to_count, browser_mail_ptr->from, count, i);
    }
    
    //��������
    MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->subject, browser_mail_ptr->subject, MMIMAIL_MAX_SUBJECT_LEN);

    //�ж��Ƿ���Ҫ���Re:ǰ׺
    if ((IsIncludeAppoinedPrefix(browser_mail_ptr->subject, reply_str, len))
        || (IsIncludeAppoinedPrefix(browser_mail_ptr->subject, reply_str1, (uint16)MMIAPICOM_Wstrlen(reply_str1)))
        || (IsIncludeAppoinedPrefix(browser_mail_ptr->subject, reply_str2, (uint16)MMIAPICOM_Wstrlen(reply_str2)))
        || (IsIncludeAppoinedPrefix(browser_mail_ptr->subject, reply_str3, (uint16)MMIAPICOM_Wstrlen(reply_str3))))
    {
        //COMM_TRACE:"MMIMAIL==> ReplyMail: The original mail subject has include Re: string!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1968_112_2_18_2_48_9_246,(uint8*)"");
    }
    else
    {
        //������ǰ���Re:ǰ׺
        if (edit_mail_ptr->subject.wstr_len < (MMIMAIL_MAX_SUBJECT_LEN - len))
        {
            len = edit_mail_ptr->subject.wstr_len + len + 1;
            temp_str = comm_alloc(sizeof(wchar) * len);
            if (PNULL != temp_str)
            {
                SCI_MEMSET(temp_str, 0, len);
                MMIAPICOM_Wstrcpy(temp_str, (const wchar *)reply_str);
                MMIAPICOM_Wstrcat(temp_str, (const wchar *)edit_mail_ptr->subject.wstr_ptr);
                
                SCI_MEMSET(edit_mail_ptr->subject.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_SUBJECT_LEN + 1)));
                MMIAPICOM_Wstrcpy(edit_mail_ptr->subject.wstr_ptr, (const wchar *)temp_str);
                edit_mail_ptr->subject.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)edit_mail_ptr->subject.wstr_ptr);

                comm_free(temp_str);/*lint !e774*/
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> ReplyMail: malloc temp_str fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_1991_112_2_18_2_48_9_247,(uint8*)"");
            }
        }
    }
    
    //��������ȷ���Ƿ����ǩ����Ϣ
    AddMailSign(&(edit_mail_ptr->textbody));

    //��������ȷ���ظ��ʼ�ʱ�Ƿ����ԭ�ʼ�����
    AddMailOrgText(&(edit_mail_ptr->textbody), &(browser_mail_ptr->textbody));
}

/*****************************************************************************/
//	Description : �ظ�ȫ���ʼ�������ԭʼ�ʼ��������ݵ��༭�ʼ��ṹ��
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void ReplyAllMail(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        )
{
    uint16  i = 0;
    uint32  count = 0;
    wchar   *temp_str = PNULL;
    wchar   reply_str[] = MMIMAIL_REPLY_SUFFIX;
    uint32  len = MMIAPICOM_Wstrlen((const wchar *)reply_str);
    char    *self_addr = PNULL;
    wchar   reply_str1[] = MMIMAIL_REPLY_SUFFIX1;
    wchar   reply_str2[] = MMIMAIL_REPLY_SUFFIX2;
    wchar   reply_str3[] = MMIMAIL_REPLY_SUFFIX3;
    
    if(PNULL == edit_mail_ptr ||PNULL == browser_mail_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> ReplyAllMail:edit_mail_ptr OR browser_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2024_112_2_18_2_48_9_248,(uint8*)"");
        return;
    }

    self_addr = mail_SenderAddr(MMIMAIL_GetDefAccountId()); //��ȡ�Լ����ʼ���ַ
    
    if (browser_mail_ptr->replyto_count > 0)
    {
        //�����ظ���ַ���ռ�������
        count = MIN(browser_mail_ptr->replyto_count, 1);
        MMIMAIL_COPY_MAIL_ADDR_T(edit_mail_ptr->to, edit_mail_ptr->to_count, browser_mail_ptr->replyto, count, i);
    }
    else
    {
        //���������˵��ռ�������
        count = MIN(browser_mail_ptr->from_count, 1);
        MMIMAIL_COPY_MAIL_ADDR_T(edit_mail_ptr->to, edit_mail_ptr->to_count, browser_mail_ptr->from, count, i);
    }

    //�������Լ�֮����ռ��˵��ռ�������
    for (i = 0; i < browser_mail_ptr->to_count; i++)
    {
        if ((edit_mail_ptr->to_count < MMIMAIL_MAX_RECEIVER_NUM)
            && (0 != strcmp((const char *)self_addr, (const char *)browser_mail_ptr->to[i]->email)))
        {
            MMIMAIL_COPY_STR(edit_mail_ptr->to[edit_mail_ptr->to_count]->email, browser_mail_ptr->to[i]->email, MMIMAIL_MAX_ADDRESS_LEN);
            MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->to[edit_mail_ptr->to_count]->name, browser_mail_ptr->to[i]->name, MMIMAIL_MAX_NAME_LEN);
            edit_mail_ptr->to_count++;
        }
    }

    //�������Լ�֮��ĳ���
    for (i = 0; i < browser_mail_ptr->cc_count; i++)
    {
        if ((edit_mail_ptr->cc_count < MMIMAIL_MAX_RECEIVER_NUM)
            && (0 != strcmp((const char *)self_addr, (const char *)browser_mail_ptr->cc[i]->email)))
        {
            MMIMAIL_COPY_STR(edit_mail_ptr->cc[edit_mail_ptr->cc_count]->email, browser_mail_ptr->cc[i]->email, MMIMAIL_MAX_ADDRESS_LEN);
            MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->cc[edit_mail_ptr->cc_count]->name, browser_mail_ptr->cc[i]->name, MMIMAIL_MAX_NAME_LEN);
            edit_mail_ptr->cc_count++;
        }
    }

    //�������Լ�֮�������,Ӧ���ǳ����Լ����������˵ĵ�ַ�����ÿ���������ͳһ������ȷ�����
    for (i = 0; i < browser_mail_ptr->bcc_count; i++)
    {
        if ((edit_mail_ptr->bcc_count < MMIMAIL_MAX_RECEIVER_NUM)
            && (0 != strcmp((const char *)self_addr, (const char *)browser_mail_ptr->bcc[i]->email)))
        {
            MMIMAIL_COPY_STR(edit_mail_ptr->bcc[edit_mail_ptr->bcc_count]->email, browser_mail_ptr->bcc[i]->email, MMIMAIL_MAX_ADDRESS_LEN);
            MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->bcc[edit_mail_ptr->bcc_count]->name, browser_mail_ptr->bcc[i]->name, MMIMAIL_MAX_NAME_LEN);
            edit_mail_ptr->bcc_count++;
        }
    }

    //��������
    MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->subject, browser_mail_ptr->subject, MMIMAIL_MAX_SUBJECT_LEN);

    //�ж��Ƿ���Ҫ���Re:ǰ׺
    if ((MMIAPICOM_Wstrnstr((const wchar *)browser_mail_ptr->subject.wstr_ptr, browser_mail_ptr->subject.wstr_len, (const wchar *)reply_str, (uint16)len))
        || (MMIAPICOM_Wstrnstr((const wchar *)browser_mail_ptr->subject.wstr_ptr, browser_mail_ptr->subject.wstr_len, (const wchar *)reply_str1, (uint16)MMIAPICOM_Wstrlen(reply_str1)))
        || (MMIAPICOM_Wstrnstr((const wchar *)browser_mail_ptr->subject.wstr_ptr, browser_mail_ptr->subject.wstr_len, (const wchar *)reply_str2, (uint16)MMIAPICOM_Wstrlen(reply_str2)))
        || (MMIAPICOM_Wstrnstr((const wchar *)browser_mail_ptr->subject.wstr_ptr, browser_mail_ptr->subject.wstr_len, (const wchar *)reply_str3, (uint16)MMIAPICOM_Wstrlen(reply_str3))))
    {
        //COMM_TRACE:"MMIMAIL==> ReplyAllMail: The original mail subject has include Re: string!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2088_112_2_18_2_48_9_249,(uint8*)"");
    }
    else
    {
        //������ǰ���Re:ǰ׺
        if (edit_mail_ptr->subject.wstr_len < (MMIMAIL_MAX_SUBJECT_LEN - len))
        {
            len = edit_mail_ptr->subject.wstr_len + len + 1;
            temp_str = comm_alloc(sizeof(wchar) * len);
            if (PNULL != temp_str)
            {
                SCI_MEMSET(temp_str, 0, sizeof(wchar) * len);
                MMIAPICOM_Wstrcpy(temp_str, (const wchar *)reply_str);
                MMIAPICOM_Wstrcat(temp_str, (const wchar *)edit_mail_ptr->subject.wstr_ptr);
                
                SCI_MEMSET(edit_mail_ptr->subject.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_SUBJECT_LEN + 1)));
                MMIAPICOM_Wstrcpy(edit_mail_ptr->subject.wstr_ptr, (const wchar *)temp_str);
                edit_mail_ptr->subject.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)edit_mail_ptr->subject.wstr_ptr);

                comm_free(temp_str);/*lint !e774*/
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> ReplyAllMail: malloc temp_str fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2111_112_2_18_2_48_9_250,(uint8*)"");
            }
        }
    }
    
    //��������ȷ���Ƿ����ǩ����Ϣ
    AddMailSign(&(edit_mail_ptr->textbody));

    //��������ȷ���ظ��ʼ�ʱ�Ƿ����ԭ�ʼ�����
    AddMailOrgText(&(edit_mail_ptr->textbody), &(browser_mail_ptr->textbody));

}

/*****************************************************************************/
//	Description : ת��ȫ�����������ĺ͸���
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void ForwardAll(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        )
{
    //uint16  i = 0;
    wchar   *temp_str = PNULL;
    wchar   reply_str[] = MMIMAIL_FORWARD_SUFFIX;
    uint32  len = MMIAPICOM_Wstrlen((const wchar *)reply_str);
    
    if(PNULL == edit_mail_ptr ||PNULL == browser_mail_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> ForwardAll:edit_mail_ptr OR browser_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2140_112_2_18_2_48_9_251,(uint8*)"");
        return;
    }

    //��������
    MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->subject, browser_mail_ptr->subject, MMIMAIL_MAX_SUBJECT_LEN);

    //������ǰ���Fw:ǰ׺
    if (edit_mail_ptr->subject.wstr_len < (MMIMAIL_MAX_SUBJECT_LEN - len))
    {
        len = edit_mail_ptr->subject.wstr_len + len + 1;
        temp_str = comm_alloc(sizeof(wchar) * len);
        if (PNULL != temp_str)
        {
            SCI_MEMSET(temp_str, 0, sizeof(wchar) * len);
            MMIAPICOM_Wstrcpy(temp_str, (const wchar *)reply_str);
            MMIAPICOM_Wstrcat(temp_str, (const wchar *)edit_mail_ptr->subject.wstr_ptr);
            
            SCI_MEMSET(edit_mail_ptr->subject.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_SUBJECT_LEN + 1)));
            MMIAPICOM_Wstrcpy(edit_mail_ptr->subject.wstr_ptr, (const wchar *)temp_str);
            edit_mail_ptr->subject.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)edit_mail_ptr->subject.wstr_ptr);

            comm_free(temp_str);/*lint !e774*/
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> ForwardAll: malloc temp_str fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2166_112_2_18_2_48_9_252,(uint8*)"");
        }
    }

    //��������ȷ���Ƿ����ǩ����Ϣ
    AddMailSign(&(edit_mail_ptr->textbody));

    //���ԭ�ʼ�����
    AddMailOrgText(&(edit_mail_ptr->textbody), &(browser_mail_ptr->textbody));

    //����ԭ�ʼ�����
    CopyMailAcc(edit_mail_ptr, browser_mail_ptr);
}

/*****************************************************************************/
//	Description : ת������
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void ForwardContent(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        )
{
    wchar   *temp_str = PNULL;
    wchar   reply_str[] = MMIMAIL_FORWARD_SUFFIX;
    uint32  len = MMIAPICOM_Wstrlen((const wchar *)reply_str);
    
    if(PNULL == edit_mail_ptr ||PNULL == browser_mail_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> ForwardContent:edit_mail_ptr OR browser_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2195_112_2_18_2_48_10_253,(uint8*)"");
        return;
    }

    //��������
    MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->subject, browser_mail_ptr->subject, MMIMAIL_MAX_SUBJECT_LEN);

    //������ǰ���Fw:ǰ׺
    if (edit_mail_ptr->subject.wstr_len < (MMIMAIL_MAX_SUBJECT_LEN - len))
    {
        len = edit_mail_ptr->subject.wstr_len + len + 1;
        temp_str = comm_alloc(sizeof(wchar) * len);
        if (PNULL != temp_str)
        {
            SCI_MEMSET(temp_str, 0, sizeof(wchar) * len);
            MMIAPICOM_Wstrcpy(temp_str, (const wchar *)reply_str);
            MMIAPICOM_Wstrcat(temp_str, (const wchar *)edit_mail_ptr->subject.wstr_ptr);
            
            SCI_MEMSET(edit_mail_ptr->subject.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_SUBJECT_LEN + 1)));
            MMIAPICOM_Wstrcpy(edit_mail_ptr->subject.wstr_ptr, (const wchar *)temp_str);
            edit_mail_ptr->subject.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)edit_mail_ptr->subject.wstr_ptr);

            comm_free(temp_str);/*lint !e774*/
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> ForwardContent: malloc temp_str fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2221_112_2_18_2_48_10_254,(uint8*)"");
        }
    }

    //��������ȷ���Ƿ����ǩ����Ϣ
    AddMailSign(&(edit_mail_ptr->textbody));

    //���ԭ�ʼ�����
    AddMailOrgText(&(edit_mail_ptr->textbody), &(browser_mail_ptr->textbody));
}

/*****************************************************************************/
//	Description : ������ת������
//	Global resource dependence : none
//	Author: yuanl
//	Note:�����ɷ������Զ���ӣ����ر༭ʱ�����ԭ����
/*****************************************************************************/
LOCAL void ServerForwardAcc(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        )
{
    wchar   *temp_str = PNULL;
    wchar   reply_str[] = MMIMAIL_FORWARD_SUFFIX;
    uint32  len = MMIAPICOM_Wstrlen((const wchar *)reply_str);
    
    if(PNULL == edit_mail_ptr ||PNULL == browser_mail_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> ServerForwardAcc:edit_mail_ptr OR browser_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2247_112_2_18_2_48_10_255,(uint8*)"");
        return;
    }

    //��������
    MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->subject, browser_mail_ptr->subject, MMIMAIL_MAX_SUBJECT_LEN);

    //������ǰ���Fw:ǰ׺
    if (edit_mail_ptr->subject.wstr_len < (MMIMAIL_MAX_SUBJECT_LEN - len))
    {
        len = edit_mail_ptr->subject.wstr_len + len + 1;
        temp_str = comm_alloc(sizeof(wchar) * len);
        if (PNULL != temp_str)
        {
            SCI_MEMSET(temp_str, 0, len);
            MMIAPICOM_Wstrcpy(temp_str, (const wchar *)reply_str);
            MMIAPICOM_Wstrcat(temp_str, (const wchar *)edit_mail_ptr->subject.wstr_ptr);
            
            SCI_MEMSET(edit_mail_ptr->subject.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_SUBJECT_LEN + 1)));
            MMIAPICOM_Wstrcpy(edit_mail_ptr->subject.wstr_ptr, (const wchar *)temp_str);
            edit_mail_ptr->subject.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)edit_mail_ptr->subject.wstr_ptr);

            comm_free(temp_str);/*lint !e774*/
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> ServerForwardAll: malloc temp_str fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2273_112_2_18_2_48_10_256,(uint8*)"");
        }
    }

    //��������ȷ���Ƿ����ǩ����Ϣ
    AddMailSign(&(edit_mail_ptr->textbody));
}

/*****************************************************************************/
//	Description : ������ת��ȫ�����������ĺ͸���
//	Global resource dependence : none
//	Author: yuanl
//	Note:�����ɷ������Զ���ӣ����ر༭ʱ�����ԭ����
/*****************************************************************************/
LOCAL void ServerForwardAll(
                        MAIL_MESSAGE_T * edit_mail_ptr,     //�༭�ʼ���ṹ
                        MAIL_MESSAGE_T * browser_mail_ptr   //����ʼ��ṹ
                        )
{
    wchar   *temp_str = PNULL;
    wchar   reply_str[] = MMIMAIL_FORWARD_SUFFIX;
    uint32  len = MMIAPICOM_Wstrlen((const wchar *)reply_str);
    
    if(PNULL == edit_mail_ptr ||PNULL == browser_mail_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> ServerForwardAll:edit_mail_ptr OR browser_mail_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2296_112_2_18_2_48_10_257,(uint8*)"");
        return;
    }

    //��������
    MMIMAIL_COPY_MMI_STRING_T(edit_mail_ptr->subject, browser_mail_ptr->subject, MMIMAIL_MAX_SUBJECT_LEN);

    //������ǰ���Fw:ǰ׺
    if (edit_mail_ptr->subject.wstr_len < (MMIMAIL_MAX_SUBJECT_LEN - len))
    {
        len = edit_mail_ptr->subject.wstr_len + len + 1;
        temp_str = comm_alloc(sizeof(wchar) * len);
        if (PNULL != temp_str)
        {
            SCI_MEMSET(temp_str, 0, len);
            MMIAPICOM_Wstrcpy(temp_str, (const wchar *)reply_str);
            MMIAPICOM_Wstrcat(temp_str, (const wchar *)edit_mail_ptr->subject.wstr_ptr);
            
            SCI_MEMSET(edit_mail_ptr->subject.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_SUBJECT_LEN + 1)));
            MMIAPICOM_Wstrcpy(edit_mail_ptr->subject.wstr_ptr, (const wchar *)temp_str);
            edit_mail_ptr->subject.wstr_len = (uint16)MMIAPICOM_Wstrlen((const wchar *)edit_mail_ptr->subject.wstr_ptr);

            comm_free(temp_str);/*lint !e774*/
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> ServerForwardAll: malloc temp_str fail!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2322_112_2_18_2_48_10_258,(uint8*)"");
        }
    }

    //��������ȷ���Ƿ����ǩ����Ϣ
    AddMailSign(&(edit_mail_ptr->textbody));

    //���ԭ�ʼ�����
    AddMailOrgText(&(edit_mail_ptr->textbody), &(browser_mail_ptr->textbody));
}

/*****************************************************************************/
//  Description :��ȡ����index
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMIMAIL_RICHTEXT_ITEM_INDEX_E GetMailEditFocusIndex(void)
{
    return s_mail_edit_global.focus_index;
}

/*****************************************************************************/
//  Description :���ý���index
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL void SetMailEditFocusIndex(MMIMAIL_RICHTEXT_ITEM_INDEX_E focus_index)
{
    s_mail_edit_global.focus_index = focus_index;
}

/*****************************************************************************/
//  Description :��ȡ��ǰѡ��˵��Ĳ���
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMIMAIL_EDIT_OPER_E GetMailEditCurOper(void)
{
    //COMM_TRACE:"MMIMAIL==> GetMailEditCurOper: s_mail_edit_global.cur_oper = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2357_112_2_18_2_48_10_259,(uint8*)"d", s_mail_edit_global.cur_oper);
    return s_mail_edit_global.cur_oper;
}

/*****************************************************************************/
//  Description :���õ�ǰѡ��˵�����
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL void SetMailEditCurOper(MMIMAIL_EDIT_OPER_E cur_oper)
{
    s_mail_edit_global.cur_oper = cur_oper;
}

/*****************************************************************************/
//  Description : ��ӻ��޸���ϵ��
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void AddEditContact(
                        MMI_CTRL_ID_T           ctrl_id,
                        uint16                  *index_ptr,
                        MAIL_ADDRESS_T          **contact_addr,
                        uint16                  contact_count,
                        MMIMAIL_ICON_ENUM_E     icon_type,
                        MMIMAIL_RICHTEXT_OP_TYPE is_add
                        )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    uint32              i = 0;
    uint32              string_len = 0;
    uint32              total_len = 0;
    wchar               *addr_str_ptr = PNULL;
    wchar               semicolon[] = L";";
    wchar               temp_wch[]  = L" ";
    wchar               wstr_addr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
    
    if(PNULL == contact_addr ||PNULL == index_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> AddEditContact:contact_addr OR index_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2394_112_2_18_2_48_10_260,(uint8*)"");
        return;
    }
    if (MMIMAIL_RICHTEXT_OP_MAX <= is_add)
    {
        //SCI_TRACE_LOW:"MMIMail AddEditContact MMIMAIL_RICHTEXT_OP_MAX <= is_add"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2399_112_2_18_2_48_10_261,(uint8*)"");
        return ;
    }
    
    item_data.img_type        = GUIRICHTEXT_IMAGE_RES;
    item_data.img_data.res.id = MMIMAIL_GetIconByType(icon_type);
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;

    //COMM_TRACE:"MMIMAIL==> AddEditContact: total num = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2407_112_2_18_2_48_10_262,(uint8*)"d", contact_count);
    
    if (0 == contact_count)
    {
        //COMM_TRACE:"MMIMAIL==> AddEditContact: receiver is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2411_112_2_18_2_48_10_263,(uint8*)"");
        
        item_data.text_data.buf.str_ptr	= temp_wch;
        item_data.text_data.buf.len	    = (uint16)MMIAPICOM_Wstrlen((const wchar *)temp_wch);
        item_data.tag_type	            = GUIRICHTEXT_TAG_TXT_NORMAL;
        item_data.text_type	            = GUIRICHTEXT_TEXT_BUF;
        if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
        {
            GUIRICHTEXT_AddItem(
                ctrl_id,
                &item_data,
                index_ptr
                );
        }
        else
        {
            GUIRICHTEXT_ModifyItem(
                ctrl_id,
                *index_ptr,
                &item_data
                );
        }
    }
    else if (1 == contact_count)
    {
        if ((PNULL != contact_addr[i])
            && (PNULL != contact_addr[i]->email)
            && ((string_len = strlen((const char *)contact_addr[i]->email)) > 0))
        {
            if (contact_addr[i]->name.wstr_len > 0)
            {
                //��ʾ���� 
                item_data.text_data.buf.str_ptr = contact_addr[i]->name.wstr_ptr;
                item_data.text_data.buf.len    = contact_addr[i]->name.wstr_len;
            }
            else
            {
                //��ʾ�ʼ���ַ
                if (string_len <= MMIMAIL_MAX_ADDRESS_LEN)
                {
                    MMIAPICOM_StrToWstr((const uint8 *)contact_addr[i]->email, wstr_addr);
                    item_data.text_data.buf.str_ptr = wstr_addr;
                    item_data.text_data.buf.len    = string_len;
                }
                else
                {
                    //COMM_TRACE:"MMIMAIL==> AddEditContact: ==error== Email addr [%s] len > 128"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2457_112_2_18_2_48_10_264,(uint8*)"s", contact_addr[i]->email);
                }
            }
            
            item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
            item_data.text_type = GUIRICHTEXT_TEXT_BUF;

            if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
            {
                GUIRICHTEXT_AddItem(
                    ctrl_id,
                    &item_data,
                    index_ptr
                    );
            }
            else
            {
                GUIRICHTEXT_ModifyItem(
                    ctrl_id,
                    *index_ptr,
                    &item_data
                    );
            }
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> AddEditContact: receiver is empty!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2483_112_2_18_2_48_10_265,(uint8*)"");
            
            item_data.text_data.buf.str_ptr	= temp_wch;
            item_data.text_data.buf.len	    = (uint16)MMIAPICOM_Wstrlen((const wchar *)temp_wch);
            item_data.tag_type	            = GUIRICHTEXT_TAG_TXT_NORMAL;
            item_data.text_type	            = GUIRICHTEXT_TEXT_BUF;
            if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
            {
                GUIRICHTEXT_AddItem(
                    ctrl_id,
                    &item_data,
                    index_ptr
                    );
            }
            else
            {
                GUIRICHTEXT_ModifyItem(
                    ctrl_id,
                    *index_ptr,
                    &item_data
                    );
            }
        }        
    }
    else
    {
        //�ϳɶ���ռ���, ��;�ָ�
        for (i = 0; i < contact_count; i++)
        {
            if ((PNULL != contact_addr[i])
                && (PNULL != contact_addr[i]->email)
                && ((string_len = strlen((const char *)contact_addr[i]->email)) > 0) 
                && (total_len < GUIRICHTEXT_TEXT_BUF_MAX_LEN))
            {
                if (contact_addr[i]->name.wstr_len > 0)
                {
                    //��ʾ���� 
                    total_len += contact_addr[i]->name.wstr_len + 1;//���Ϸָ���Ŀռ�
                }
                else
                {
                    //��ʾ�ʼ���ַ
                    total_len += string_len + 1;//���Ϸָ���Ŀռ�
                }
            }
            else
            {
                break;
            }
        }

        //�ռ��˲�Ϊ��
        if (total_len > 0)
        {
            addr_str_ptr = SCI_ALLOC_APP(total_len * 2 + 2);
            
            if (PNULL != addr_str_ptr)
            {
                SCI_MEMSET(addr_str_ptr, 0, total_len * 2 + 2);
                
                total_len = 0;
                for (i = 0; i < contact_count; i++)
                {
                    if ((PNULL != contact_addr[i])
                        && (PNULL != contact_addr[i]->email)
                        && ((string_len = strlen((const char *)contact_addr[i]->email)) > 0) 
                        && (total_len < GUIRICHTEXT_TEXT_BUF_MAX_LEN))
                    {
                        if (contact_addr[i]->name.wstr_len > 0)
                        {
                            //��ʾ���� 
                            MMIAPICOM_Wstrncat(addr_str_ptr, (const wchar *)contact_addr[i]->name.wstr_ptr,contact_addr[i]->name.wstr_len);
                            MMIAPICOM_Wstrncat(addr_str_ptr, (const wchar *)semicolon, 1);
                            total_len += contact_addr[i]->name.wstr_len + 1;//���Ϸָ���Ŀռ�
                        }
                        else
                        {
                            //��ʾ�ʼ���ַ
                            if (string_len <= MMIMAIL_MAX_ADDRESS_LEN)
                            {
                                MMIAPICOM_StrToWstr((const uint8 *)contact_addr[i]->email, wstr_addr);
                                MMIAPICOM_Wstrncat(addr_str_ptr, (const wchar *)wstr_addr, string_len);
                                MMIAPICOM_Wstrncat(addr_str_ptr, (const wchar *)semicolon, 1);
                                total_len += string_len + 1;//���Ϸָ���Ŀռ�
                            }
                            else
                            {
                                //COMM_TRACE:"MMIMAIL==> AddEditContact: ==error== Email addr [%s] len > 128"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2570_112_2_18_2_48_10_266,(uint8*)"s", contact_addr[i]->email);
                            }
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                
                item_data.text_data.buf.str_ptr = addr_str_ptr;
                item_data.text_data.buf.len    = (uint16)total_len;
                item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
                item_data.text_type = GUIRICHTEXT_TEXT_BUF;

                if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
                {
                    GUIRICHTEXT_AddItem(
                        ctrl_id,
                        &item_data,
                        index_ptr
                        );
                }
                else
                {
                    GUIRICHTEXT_ModifyItem(
                        ctrl_id,
                        *index_ptr,
                        &item_data
                        );
                }
                
                //�ͷſռ�
                SCI_FREE(addr_str_ptr);
                addr_str_ptr = PNULL;
            }
            else
            {
                //COMM_TRACE:"MMIMAIL==> AddEditContact: malloc memory fail, malloc size = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2608_112_2_18_2_48_10_267,(uint8*)"d", (total_len * 2 + 2));
            }
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> AddEditContact: receiver is empty!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2613_112_2_18_2_48_10_268,(uint8*)"");
            
            item_data.text_data.buf.str_ptr	= temp_wch;
            item_data.text_data.buf.len	    = (uint16)MMIAPICOM_Wstrlen((const wchar *)temp_wch);
            item_data.tag_type	            = GUIRICHTEXT_TAG_TXT_NORMAL;
            item_data.text_type	            = GUIRICHTEXT_TEXT_BUF;
            if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
            {
                GUIRICHTEXT_AddItem(
                    ctrl_id,
                    &item_data,
                    index_ptr
                    );
            }
            else
            {
                GUIRICHTEXT_ModifyItem(
                    ctrl_id,
                    *index_ptr,
                    &item_data
                    );
            }
        }
    }
}

/*****************************************************************************/
//  Description : ��ӻ��޸��ռ���
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void AddEditTo(
                        MMI_CTRL_ID_T           ctrl_id,
                        uint16                  *index_ptr,
                        MAIL_MESSAGE_T          *editbody_ptr,
                        MMIMAIL_RICHTEXT_OP_TYPE is_add
                        )
{
    if(PNULL == editbody_ptr ||PNULL == index_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> AddEditTo:editbody_ptr OR index_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2652_112_2_18_2_48_10_269,(uint8*)"");
        return;
    }
    if (MMIMAIL_RICHTEXT_OP_MAX <= is_add)
    {
        //SCI_TRACE_LOW:"MMIMail AddEditTo MMIMAIL_RICHTEXT_OP_MAX <= is_add"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2657_112_2_18_2_48_10_270,(uint8*)"");
        return ;
    }
    
    //COMM_TRACE:"MMIMAIL==> AddEditTo enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2661_112_2_18_2_48_10_271,(uint8*)"");
    AddEditContact(ctrl_id, index_ptr, editbody_ptr->to, editbody_ptr->to_count, MMIMAIL_ICON_TO, is_add);
    return;
}

/*****************************************************************************/
//  Description : ��ӻ��޸ĳ���
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void AddEditCc(
                        MMI_CTRL_ID_T           ctrl_id,
                        uint16                  *index_ptr,
                        MAIL_MESSAGE_T          *editbody_ptr,
                        MMIMAIL_RICHTEXT_OP_TYPE is_add
                        )
{
    if(PNULL == editbody_ptr ||PNULL == index_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> AddEditCc:editbody_ptr OR index_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2679_112_2_18_2_48_10_272,(uint8*)"");
        return;
    }
    if (MMIMAIL_RICHTEXT_OP_MAX <= is_add)
    {
        //SCI_TRACE_LOW:"MMIMail AddEditCc MMIMAIL_RICHTEXT_OP_MAX <= is_add"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2684_112_2_18_2_48_10_273,(uint8*)"");
        return ;
    }

    //COMM_TRACE:"MMIMAIL==> AddEditCc enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2688_112_2_18_2_48_10_274,(uint8*)"");
    AddEditContact(ctrl_id, index_ptr, editbody_ptr->cc, editbody_ptr->cc_count, MMIMAIL_ICON_CC, is_add);
    return;
}

/*****************************************************************************/
//  Description : ��ӻ��޸�����
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void AddEditBcc(
                        MMI_CTRL_ID_T           ctrl_id,
                        uint16                  *index_ptr,
                        MAIL_MESSAGE_T          *editbody_ptr,
                        MMIMAIL_RICHTEXT_OP_TYPE is_add
                        )
{
    if(PNULL == editbody_ptr ||PNULL == index_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> AddEditBcc:editbody_ptr OR index_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2706_112_2_18_2_48_11_275,(uint8*)"");
        return;
    }
    if (MMIMAIL_RICHTEXT_OP_MAX <= is_add)
    {
        //SCI_TRACE_LOW:"MMIMail AddEditBcc MMIMAIL_RICHTEXT_OP_MAX <= is_add"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2711_112_2_18_2_48_11_276,(uint8*)"");
        return ;
    }

    //COMM_TRACE:"MMIMAIL==> AddEditBcc enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2715_112_2_18_2_48_11_277,(uint8*)"");
    AddEditContact(ctrl_id, index_ptr, editbody_ptr->bcc, editbody_ptr->bcc_count, MMIMAIL_ICON_BCC, is_add);
    return;
}

/*****************************************************************************/
//  Description : ��ӻ��޸�����
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void AddEditSubject(
                        MMI_CTRL_ID_T           ctrl_id,
                        uint16                  *index_ptr,
                        MAIL_MESSAGE_T          *editbody_ptr,
                        MMIMAIL_RICHTEXT_OP_TYPE is_add
                        )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    wchar               temp_wch[]= L" ";

    if(PNULL == editbody_ptr ||PNULL == index_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> AddEditSubject:editbody_ptr OR index_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2736_112_2_18_2_48_11_278,(uint8*)"");
        return;
    }
    if (MMIMAIL_RICHTEXT_OP_MAX <= is_add)
    {
        //SCI_TRACE_LOW:"MMIMail AddEditSubject MMIMAIL_RICHTEXT_OP_MAX <= is_add"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2741_112_2_18_2_48_11_279,(uint8*)"");
        return ;
    }
    
    item_data.img_type = GUIRICHTEXT_IMAGE_RES;
    item_data.img_data.res.id    = MMIMAIL_GetIconByType(MMIMAIL_ICON_SUBJECT);
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    if (editbody_ptr->subject.wstr_len > 0)
    {
        item_data.text_data.buf.str_ptr = editbody_ptr->subject.wstr_ptr;
        item_data.text_data.buf.len     = editbody_ptr->subject.wstr_len;
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> AddEditSubject: subject is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2756_112_2_18_2_48_11_280,(uint8*)"");
        item_data.text_data.buf.str_ptr = temp_wch;
        item_data.text_data.buf.len     = (uint16)MMIAPICOM_Wstrlen((const wchar *)temp_wch);
    }

    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
    {
        GUIRICHTEXT_AddItem (
            ctrl_id,
            &item_data,
            index_ptr
            );
    }
    else
    {
        GUIRICHTEXT_ModifyItem(
            ctrl_id,
            *index_ptr,
            &item_data
            );
    }    
}

/*****************************************************************************/
//  Description : ��ӻ��޸�����
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void AddEditContent(
                        MMI_CTRL_ID_T           ctrl_id,
                        uint16                  *index_ptr,
                        MAIL_MESSAGE_T          *editbody_ptr,
                        MMIMAIL_RICHTEXT_OP_TYPE is_add
                        )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    wchar               temp_wch[]= MMIMAIL_EMPTY_CONTENT;
/*    uint16              line_num = 0;
    wchar               *tmp_str = PNULL;
    wchar               temp_wch1[]= L"\n\n\n";
    wchar               temp_wch2[]= L"\n\n";
    uint16              len = 0;*/
    
    if(PNULL == editbody_ptr ||PNULL == index_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> AddEditContent:editbody_ptr OR index_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2801_112_2_18_2_48_11_281,(uint8*)"");
        return;
    }
    if (MMIMAIL_RICHTEXT_OP_MAX <= is_add)
    {
        //SCI_TRACE_LOW:"MMIMail AddEditContent MMIMAIL_RICHTEXT_OP_MAX <= is_add"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2806_112_2_18_2_48_11_282,(uint8*)"");
        return ;
    }

    item_data.img_type = GUIRICHTEXT_IMAGE_NONE;
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    if (editbody_ptr->textbody.wstr_len > 0)
    {        
        item_data.text_data.buf.str_ptr = editbody_ptr->textbody.wstr_ptr;
        item_data.text_data.buf.len     = editbody_ptr->textbody.wstr_len;
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> AddEditContent: content is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2820_112_2_18_2_48_11_283,(uint8*)"");
        item_data.text_data.buf.str_ptr = temp_wch;
        item_data.text_data.buf.len     = (uint16)MMIAPICOM_Wstrlen((const wchar *)temp_wch);
    }
    
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
    {
        GUIRICHTEXT_AddItem (
            ctrl_id,
            &item_data,
            index_ptr
            );
    }
    else
    {
        GUIRICHTEXT_ModifyItem(
            ctrl_id,
            *index_ptr,
            &item_data
            );
    }
#if 0
    //����������ʾ����������ʾ����
    GUIRICHTEXT_GetItemLineNum(ctrl_id, *index_ptr, &line_num);
    switch(line_num)
    {
    case 1:
        len = sizeof(wchar) * (item_data.text_data.buf.len + 4 + 1);
        tmp_str = comm_alloc(len);
        if (PNULL != tmp_str)
        {
            MMIAPICOM_Wstrncpy(tmp_str, (const wchar *)item_data.text_data.buf.str_ptr, item_data.text_data.buf.len);
            MMIAPICOM_Wstrncat(tmp_str, (const wchar *)temp_wch, 4);
            
            item_data.text_data.buf.str_ptr = tmp_str;
            item_data.text_data.buf.len     = MMIAPICOM_Wstrlen(tmp_str);
            GUIRICHTEXT_ModifyItem(
                ctrl_id,
                *index_ptr,
                &item_data
                );

            comm_free(tmp_str);
        }
        break;
        
    case 2:
        len = sizeof(wchar) * (item_data.text_data.buf.len + 3 + 1);
        tmp_str = comm_alloc(len);
        if (PNULL != tmp_str)
        {
            MMIAPICOM_Wstrncpy(tmp_str, (const wchar *)item_data.text_data.buf.str_ptr, item_data.text_data.buf.len);
            MMIAPICOM_Wstrncat(tmp_str, (const wchar *)temp_wch1, 3);
            
            item_data.text_data.buf.str_ptr = tmp_str;
            item_data.text_data.buf.len     = MMIAPICOM_Wstrlen(tmp_str);
            GUIRICHTEXT_ModifyItem(
                ctrl_id,
                *index_ptr,
                &item_data
                );

            comm_free(tmp_str);
        }
        break;
        
    case 3:
        len = sizeof(wchar) * (item_data.text_data.buf.len + 2 + 1);
        tmp_str = comm_alloc(len);
        if (PNULL != tmp_str)
        {
            MMIAPICOM_Wstrncpy(tmp_str, (const wchar *)item_data.text_data.buf.str_ptr, item_data.text_data.buf.len);
            MMIAPICOM_Wstrncat(tmp_str, (const wchar *)temp_wch2, 2);
            
            item_data.text_data.buf.str_ptr = tmp_str;
            item_data.text_data.buf.len     = MMIAPICOM_Wstrlen(tmp_str);
            GUIRICHTEXT_ModifyItem(
                ctrl_id,
                *index_ptr,
                &item_data
                );

            comm_free(tmp_str);
        }
        break;
        
    default:
        break;
    } 
#endif    
}

/*****************************************************************************/
//  Description : ��ӻ��޸ĸ���
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void AddEditAcc(
                        MMI_CTRL_ID_T           ctrl_id,
                        uint16                  *index_ptr,
                        MAIL_MESSAGE_T          *editbody_ptr,
                        MMIMAIL_RICHTEXT_OP_TYPE is_add
                        )
{
    GUIRICHTEXT_ITEM_T  item_data = {0};
    wchar               temp_wch[]= L" ";
    char                acc_str[10] = {0};
    wchar               acc_wstr[20] = {0};

    if(PNULL == editbody_ptr ||PNULL == index_ptr )
    {
        //COMM_TRACE:"MMIMAIL==> AddEditContent:editbody_ptr OR index_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2932_112_2_18_2_48_11_284,(uint8*)"");
        return;
    }
    if (MMIMAIL_RICHTEXT_OP_MAX <= is_add)
    {
        //SCI_TRACE_LOW:"MMIMail AddEditAcc MMIMAIL_RICHTEXT_OP_MAX <= is_add"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2937_112_2_18_2_48_11_285,(uint8*)"");
        return ;
    }
    
    item_data.img_type = GUIRICHTEXT_IMAGE_RES;
    item_data.img_data.res.id    = MMIMAIL_GetIconByType(MMIMAIL_ICON_ACCESSORY);
    item_data.text_frame_type = GUIRICHTEXT_FRAME_NORMAL;
    
    if (editbody_ptr->partcount > 0)
    {
        sprintf(acc_str, "(%d)", editbody_ptr->partcount);
        MMIAPICOM_StrToWstr((const uint8 *)acc_str, acc_wstr);
        item_data.text_data.buf.str_ptr = acc_wstr;
        item_data.text_data.buf.len     = (uint16)MMIAPICOM_Wstrlen((const wchar *)acc_wstr);
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> AddEditAcc: acc is empty!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2954_112_2_18_2_48_11_286,(uint8*)"");
        item_data.text_data.buf.str_ptr = temp_wch;
        item_data.text_data.buf.len     = (uint16)MMIAPICOM_Wstrlen((const wchar *)temp_wch);
    }
    
    item_data.tag_type = GUIRICHTEXT_TAG_TXT_NORMAL;
    item_data.text_type = GUIRICHTEXT_TEXT_BUF;
    if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
    {
        GUIRICHTEXT_AddItem (
            ctrl_id,
            &item_data,
            index_ptr
            );
    }
    else
    {
        GUIRICHTEXT_ModifyItem(
            ctrl_id,
            *index_ptr,
            &item_data
            );
    }
}

/*****************************************************************************/
//  Description :��ӻ��޸�richtext item
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN MailEditEntry(
                                MMI_CTRL_ID_T               ctrl_id,
                                MAIL_MESSAGE_T              *editbody_ptr,
                                MMIMAIL_RICHTEXT_OP_TYPE    is_add
                              )
{
    uint16 index = 0;
    
    if(PNULL == editbody_ptr)
    {
        //COMM_TRACE:"MMIMAIL==> MailEditEntry:editbody_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_2993_112_2_18_2_48_11_287,(uint8*)"");
        return FALSE;
    }

    if (is_add >= MMIMAIL_RICHTEXT_OP_MAX)
    {
        return FALSE;
    }
    
    if (MMIMAIL_RICHTEXT_ADD_ITEM == is_add)
    {
        AddEditTo(ctrl_id, &index, editbody_ptr, is_add);
        AddEditCc(ctrl_id, &index, editbody_ptr, is_add);
        AddEditBcc(ctrl_id, &index, editbody_ptr, is_add);
        AddEditSubject(ctrl_id, &index, editbody_ptr, is_add);
        AddEditAcc(ctrl_id, &index, editbody_ptr, is_add);
        AddEditContent(ctrl_id, &index, editbody_ptr, is_add);
    }
    else
    {
        //mod
        MMIMAIL_RICHTEXT_ITEM_INDEX_E focus_index = GetMailEditFocusIndex();
        index = (uint16)focus_index;
        
        switch (focus_index)
        {
        case MMIMAIL_RICHTEXT_TO:
            AddEditTo(ctrl_id, &index, editbody_ptr, is_add);
            break;
            
        case MMIMAIL_RICHTEXT_CC:
            AddEditCc(ctrl_id, &index, editbody_ptr, is_add);
            break;
            
        case MMIMAIL_RICHTEXT_BCC:
            AddEditBcc(ctrl_id, &index, editbody_ptr, is_add);
            break;
            
        case MMIMAIL_RICHTEXT_SUBJECT:
            AddEditSubject(ctrl_id, &index, editbody_ptr, is_add);
            break;
            
        case MMIMAIL_RICHTEXT_ACCESSORY:
            AddEditAcc(ctrl_id, &index, editbody_ptr, is_add);
            break;
            
        case MMIMAIL_RICHTEXT_CONTENT:
            AddEditContent(ctrl_id, &index, editbody_ptr, is_add);
            break;
            
        default:
            //COMM_TRACE:"MMIMAIL==> MailEditEntry: focus_index [%d] is invalid!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3044_112_2_18_2_48_11_288,(uint8*)"d", focus_index);
            break;        
        }
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description :����richtext��Ӧ��ı༭����
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL void MailEditEnterCorrespondItem(MMI_CTRL_ID_T ctrl_id)
{
    uint16 richedit_index = 0;
    
    if (!GUIRICHTEXT_GetFocusItemIndex(ctrl_id, &richedit_index))
    {
        //���û�н�������ֱ���˳�
        return;
    }
    
    switch (s_mail_edit_richtext_item_type[richedit_index])
    {
    case MMIMAIL_RICHTEXT_TO:
        MMIMAIL_CreateToListEditWin();
        break;
        
    case MMIMAIL_RICHTEXT_CC:
        MMIMAIL_CreateCcListEditWin();
        break;
        
    case MMIMAIL_RICHTEXT_BCC:
        MMIMAIL_CreateBccListEditWin();
        break;
        
    case MMIMAIL_RICHTEXT_SUBJECT:
        MMIMAIL_CreateSubjectEditWin();
        break;
        
    case MMIMAIL_RICHTEXT_CONTENT:
        MMIMAIL_CreateContentEditWin();
        break;
        
    case MMIMAIL_RICHTEXT_ACCESSORY:
        MMIMAIL_CreateAccListEditWin();
        break;
        
    default:
        //COMM_TRACE:"MMIMAIL==> MailEditEnterCorrespondItem: richedit_index [%d] is invalid!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3093_112_2_18_2_48_11_289,(uint8*)"d", richedit_index);
        break;
    }

    //���浱ǰ������
    SetMailEditFocusIndex(s_mail_edit_richtext_item_type[richedit_index]);
    
    return;
}

/*****************************************************************************/
//	Description : ��ʼ��������Ϣ
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void MailInitAccInfo(MAIL_MESSAGE_T  *editbody_ptr)
{
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail MailInitAccInfo PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3111_112_2_18_2_48_11_290,(uint8*)"");
        return ;
    }
    
    if (s_init_acc_info.file_name_len > 0)
    {
        MMIAPICOM_Wstrncpy(editbody_ptr->mailmessageparts[0]->filename.wstr_ptr,
                            s_init_acc_info.file_name,
                            s_init_acc_info.file_name_len);
        editbody_ptr->mailmessageparts[0]->filename.wstr_len = s_init_acc_info.file_name_len;
                                    
        MMIAPICOM_Wstrncpy(editbody_ptr->mailmessageparts[0]->fullname.wstr_ptr,
                            s_init_acc_info.full_name,
                            s_init_acc_info.full_name_len);
        editbody_ptr->mailmessageparts[0]->fullname.wstr_len = s_init_acc_info.full_name_len;

        editbody_ptr->mailmessageparts[0]->size = s_init_acc_info.file_size;
        editbody_ptr->partcount++;
    }

    //���ȫ�ֱ������´��ٴ����༭����ʱ����ʹ�ñ��εĸ�����Ϣ
    SCI_MEMSET(&s_init_acc_info, 0, sizeof(MMIMAIL_EDIT_FILE_INFO_T));
    
}

/*****************************************************************************/
//  Description :�༭�����ڴ�����
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleMailEditMainWinMsg(
                                                 MMI_WIN_ID_T       win_id,  
                                                 MMI_MESSAGE_ID_E   msg_id, 
                                                 DPARAM             param
                                               )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_RICHTEXT_CTRL_ID;
    uint16          get_focus_index = 0;    
    MAIL_MESSAGE_T  *editbody_ptr = s_mail_edit_global.editbody_ptr;
    LOCAL BOOLEAN   is_save = TRUE;

    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleMailEditMainWinMsg PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3154_112_2_18_2_48_11_291,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    //COMM_TRACE:"MMIMAIL==> HandleMailEditMainWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3158_112_2_18_2_48_11_292,(uint8*)"d", msg_id);
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        is_save = TRUE;
        MailInitAccInfo(editbody_ptr);
        MailEditEntry(ctrl_id, editbody_ptr, MMIMAIL_RICHTEXT_ADD_ITEM);
        GUIRICHTEXT_SetFocusMode(ctrl_id, GUIRICHTEXT_FOCUS_SHIFT_ADVANCED);
        GUIRICHTEXT_SetBorderColor(ctrl_id, MMI_GRAY_WHITE_COLOR);
        GUIRICHTEXT_SetFocusBgColor(ctrl_id, MMI_DARK_GRAY_COLOR);
        GUIRICHTEXT_SetFocusItem(ctrl_id, get_focus_index);
        SetMailEditFocusIndex(s_mail_edit_richtext_item_type[get_focus_index]);
        SetMailEditCurOper(MMIMAIL_EDIT_OPER_NULL);
        MMK_SetAtvCtrl(win_id, ctrl_id); 

        if (s_mail_edit_global.is_inc_org_text)
        {
            //��ʾԭ�ʼ����Ĺ����޷�ֱ�ӱ༭
            MMIMAIL_OpenMsgBox(TXT_MAIL_ORIG_CONTENT_TOO_LARGE, IMAGE_PUBWIN_WARNING);
        }
        break;

    case MSG_APP_OK:
    case MSG_CTL_OK:
        MMK_CreateWin((uint32 *)MMIMAIL_EDIT_OPT_WIN_TAB,PNULL);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        MailEditEnterCorrespondItem(ctrl_id);
        break;

//    case MSG_KEYLONG_RED:
//    case MSG_APP_RED:
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        {
            MMIMAIL_OPEN_EDIT_WIN_TYPE_E edit_type = MMIMAIL_OPEN_TYPE_DEFAULT;

            edit_type = MMIMAIL_GetEditWinOpenType();

            if (MMIMAIL_OPEN_TYPE_NEW_MAIL != edit_type && !MMIMAIL_IsEmpty(editbody_ptr)) /* Do Not need prompt when new mail*/
            {
                MMIPUB_OpenQueryWinByTextId(TXT_EXIT_WITHOUT_SAVE,IMAGE_PUBWIN_QUERY,PNULL,PNULL);
            }
            else
            {
                MMK_CloseWin(win_id);
                if ((MMIMAIL_DRAFTBOX == MMIMAIL_GetCurBoxType())
                    && (MMIMAIL_OPEN_TYPE_ORIGINAL == edit_type))
                {
                    //ֻ�б���ݸ����е��ʼ�ʱ��Ҫ�ر��ʼ��Ķ�����
                    if (MMK_IsOpenWin(MMIMAIL_READ_WIN_ID))
                    {       
                        MMK_CloseWin(MMIMAIL_READ_WIN_ID);
                    }
                }
            }
        }
        break;

    case MSG_PROMPTWIN_OK:
        MMIPUB_CloseQuerytWin(PNULL);
        is_save = FALSE;

        MMK_CloseWin(win_id);
        if ((MMIMAIL_DRAFTBOX == MMIMAIL_GetCurBoxType())
            && (MMIMAIL_OPEN_TYPE_ORIGINAL == MMIMAIL_GetEditWinOpenType()))
        {
            //ֻ�б���ݸ����е��ʼ�ʱ��Ҫ�ر��ʼ��Ķ�����
            if (MMK_IsOpenWin(MMIMAIL_READ_WIN_ID))
            {       
                MMK_CloseWin(MMIMAIL_READ_WIN_ID);
            }
        }
        break;

    case MSG_PROMPTWIN_CANCEL:
        MMIPUB_CloseQuerytWin(PNULL);
        break;

    case MSG_CLOSE_WINDOW:
    {
        BOOLEAN is_need_save = FALSE;

        if (MMIMAIL_OPER_OPEN_EDIT_WIN == GetHandleOkMenuOperation())
        {
            MAIL_MESSAGE_T * cur_disp_mail = MMIMAIL_GetCurDispMail();

            if (PNULL != cur_disp_mail)
            {
                MMIEMAIL_DestroyMessage(cur_disp_mail);
                
                //���õ�ǰ��ʾ�ʼ�Ϊ��
                MMIMAIL_SetCurDispMail(PNULL);
            }
            
            SetHandleOkMenuOperation(MMIMAIL_OPER_NULL);
        }
        
        if (is_save
            && (MMIMAIL_EDIT_OPER_SAVE != GetMailEditCurOper())
            && (MMIMAIL_EDIT_OPER_SEND != GetMailEditCurOper())
            && (MMIMAIL_EDIT_OPER_CANCEL != GetMailEditCurOper()))
        {
            //�Զ������ʼ����ݸ��䣬��̨���棬û�н�����ʾ
            is_need_save = MMIMAIL_SaveMail(editbody_ptr);
            if (is_need_save)
            {
                s_mail_edit_global.is_other_free = TRUE;
            }
        }
        else
        {
            //COMM_TRACE:"MMIMail==> HandleMailEditMainWinMsg: GetMailEditCurOper()=%d "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3274_112_2_18_2_48_12_293,(uint8*)"d", GetMailEditCurOper());
        }
        
        if (s_mail_edit_global.is_other_free)
        {
            //COMM_TRACE:"MMIMail==> HandleMailEditMainWinMsg: close window , is_other_free is TRUE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3279_112_2_18_2_48_12_294,(uint8*)"");
            s_mail_edit_global.is_other_free = FALSE; //�ָ�ȫ�ֱ���
        }
        else
        {
            //COMM_TRACE:"MMIMail==> HandleMailEditMainWinMsg: close window , is_other_free is FALSE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3284_112_2_18_2_48_12_295,(uint8*)"");
            DestroyMailEditBody(s_mail_edit_global.editbody_ptr);
        }
        
        //�������߳��ͷ��ڴ�Ҳֱ�ӽ�ָ�븳�գ������ʼ����ں�̨����ʱ���½���༭ʱ���ڴ����ͷ�
        s_mail_edit_global.editbody_ptr = PNULL;

        if ((MAIL_ENTER_NEWMAIL == MMIMAIL_GetOpenType())
            || (MAIL_ENTER_REPLYMAIL == MMIMAIL_GetOpenType()))
        {
            if ((MMIMAIL_EDIT_OPER_CANCEL == GetMailEditCurOper())
                || ((MMIMAIL_EDIT_OPER_SAVE != GetMailEditCurOper())
                && (MMIMAIL_EDIT_OPER_SEND != GetMailEditCurOper())
                && !is_need_save))
            {
                //������������Ϊֱ�ӽ���༭���棬���˳��༭����ͬʱ�˳�����
                MMIAPIMAIL_Exit();
            }
        }
        is_save = TRUE;
        break;
    }
    
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }

    
    return err_code;
}

/*****************************************************************************/
//	Description : �ʼ��༭ѡ��˵�������
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditOptWinMsg(
                                          MMI_WIN_ID_T        win_id, 
                                          MMI_MESSAGE_ID_E    msg_id, 
                                          DPARAM              param
                                          )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T       ctrl_id = MMIMAIL_EDIT_OPT_MENU_CTRL_ID;
    MMI_MENU_GROUP_ID_T group_id = 0;
    MMI_MENU_ID_T       menu_id = 0;
    MAIL_MESSAGE_T      *editbody_ptr = s_mail_edit_global.editbody_ptr;
    MMI_WIN_ID_T        send_win_id = MMIMAIL_SEND_WAITING_WIN_ID;

    //COMM_TRACE:"MMIMAIL==> HandleMailEditOptWinMsg, msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3333_112_2_18_2_48_12_296,(uint8*)"d", msg_id);
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
        switch (menu_id)
        {
        case MMIMAIL_MENU_EDIT_SEND_ITEM_ID:
            if(MMIAPISET_GetFlyMode())//for ����ģʽ�ж�MS00217230 liangshun20110105
            {
                MMIMAIL_OpenMsgBox(TXT_MAIL_NOT_AVAILABLE, IMAGE_PUBWIN_WARNING);
                //�ʼ��༭������
                //�Զ������ʼ����ݸ��䣬��̨���棬û�н�����ʾ
                if(MMIMAIL_SaveMail(editbody_ptr))
                {
                    SetMailEditCurOper(MMIMAIL_EDIT_OPER_SAVE);
                    s_mail_edit_global.is_other_free = TRUE;
                    
                    MMK_CloseWin(win_id);
                    
                    //�رձ༭����
                    if (MMK_IsOpenWin(MMIMAIL_EDIT_MAIN_WIN_ID))
                    {       
                        MMK_CloseWin(MMIMAIL_EDIT_MAIN_WIN_ID);
                    }
                    if(MMIMAIL_OPEN_TYPE_ORIGINAL == MMIMAIL_GetEditWinOpenType())
                    {
                        //�༭�����з��ͷ������ݸ����е��ʼ�ʱ��Ҫ�ر��ʼ��Ķ�����
                        if (MMK_IsOpenWin(MMIMAIL_READ_WIN_ID))
                        {       
                            MMK_CloseWin(MMIMAIL_READ_WIN_ID);
                        }
                    }
                }
                else
                {
                    //�ʼ�Ϊ�գ�������ֱ���˳��ʼ��༭����
                    MMK_CloseWin(win_id);
                    MMK_CloseWin(MMIMAIL_EDIT_MAIN_WIN_ID);
                }
            }
            else
            {
            if (MMIAPIMAIL_SendMail(editbody_ptr))
            {
                SetMailEditCurOper(MMIMAIL_EDIT_OPER_SEND);
                s_mail_edit_global.is_other_free = TRUE;
                
                // 3���Զ��رգ�תΪ��̨����
                MMIPUB_OpenAlertWinByTextId(PNULL, 
                                            TXT_MAIL_SENDING, 
                                            TXT_NULL, 
                                            IMAGE_PUBWIN_WARNING, 
                                            &send_win_id, 
                                            PNULL, 
                                            MMIPUB_SOFTKEY_ONE, 
                                            HandleSendEditWaitingWinMsg);
            }
            MMK_CloseWin(win_id);
            }
            break;

        case MMIMAIL_MENU_EDIT_SAVE_ITEM_ID:
            if (MMIMAIL_SaveMail(editbody_ptr))
            {
                MMI_STRING_T    prompt_str = {0};

                SetMailEditCurOper(MMIMAIL_EDIT_OPER_SAVE);
                s_mail_edit_global.is_other_free = TRUE;
                
                MMK_CloseWin(win_id);
                
                //��ʾ���ڱ���
                MMI_GetLabelTextByLang(TXT_COMMON_WAITING, &prompt_str);
                MMIPUB_OpenWaitWin(1, &prompt_str, PNULL, PNULL, MMIMAIL_SAVE_MAIL_WAITING_WIN_ID, IMAGE_NULL,
                    ANIM_PUBWIN_WAIT, WIN_ONE_LEVEL, MMIPUB_SOFTKEY_NONE, HandleSaveEditWaitingWinMsg);

            }
            else
            {
                //�ʼ�Ϊ�գ�������ֱ���˳��ʼ��༭����
                MMK_CloseWin(win_id);
                MMK_CloseWin(MMIMAIL_EDIT_MAIN_WIN_ID);
            }
            break;
            
        case MMIMAIL_MENU_EDIT_CANCEL_ITEM_ID:
            SetMailEditCurOper(MMIMAIL_EDIT_OPER_CANCEL);
            MMK_CloseWin(win_id);
            //�رձ༭���ڣ�������ֱ���˳�
            MMK_CloseWin(MMIMAIL_EDIT_MAIN_WIN_ID);
            break;
            
        default:
            //COMM_TRACE:"MMIMAIL==> HandleMailEditOptWinMsg, menu_id = %d is invalid"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3436_112_2_18_2_48_12_297,(uint8*)"d", menu_id);
            break;
        }
        
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
//	Description : �ȴ������ʼ����ڴ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSaveEditWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_APP_RED:
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;

    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        break;

    case MSG_CLOSE_WINDOW:
        //�رձ༭����
        if (MMK_IsOpenWin(MMIMAIL_EDIT_MAIN_WIN_ID))
        {       
            MMK_CloseWin(MMIMAIL_EDIT_MAIN_WIN_ID);
        }
        if ((MMIMAIL_DRAFTBOX == MMIMAIL_GetCurBoxType())
            && (MMIMAIL_OPEN_TYPE_ORIGINAL == MMIMAIL_GetEditWinOpenType()))
        {
            //ֻ�б���ݸ����е��ʼ�ʱ��Ҫ�ر��ʼ��Ķ�����
            if (MMK_IsOpenWin(MMIMAIL_READ_WIN_ID))
            {       
                MMK_CloseWin(MMIMAIL_READ_WIN_ID);
            }
        }
        break;
            
    default:
        recode = MMIPUB_HandleWaitWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//	Description : �����д��ڴ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendEditWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, TRUE);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;

    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //ȡ������       
        MMIAPIMAIL_CancelSendById(0);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        //�رձ༭����
        if (MMK_IsOpenWin(MMIMAIL_EDIT_MAIN_WIN_ID))
        {       
            MMK_CloseWin(MMIMAIL_EDIT_MAIN_WIN_ID);
        }
        if(MMIMAIL_OPEN_TYPE_ORIGINAL == MMIMAIL_GetEditWinOpenType())
        {
            //�༭�����з��ͷ������ݸ����е��ʼ�ʱ��Ҫ�ر��ʼ��Ķ�����
            if (MMK_IsOpenWin(MMIMAIL_READ_WIN_ID))
            {       
                MMK_CloseWin(MMIMAIL_READ_WIN_ID);
            }
        }
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//	Description : ͨ��id�����д��ڴ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSendMailByIdWaitingWinMsg(
                                     MMI_WIN_ID_T        win_id,    //IN:
                                     MMI_MESSAGE_ID_E    msg_id,    //IN:
                                     DPARAM              param      //IN:
                                     )
{
    MMI_RESULT_E recode = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUIWIN_SetSoftkeyTextId(win_id, TXT_NULL, TXT_NULL, STXT_CANCEL, TRUE);
        break;
        
    case MSG_APP_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        break;

    case MSG_APP_RED:
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        //ȡ������       
        MMIAPIMAIL_CancelSendById(0);
        MMK_CloseWin(win_id);
        break;

    case MSG_CLOSE_WINDOW:
        if(MMIMAIL_OPEN_TYPE_ORIGINAL == MMIMAIL_GetEditWinOpenType())
        {
            //�ʼ��Ķ������з��ͷ������ݸ����е��ʼ�ʱ��Ҫ�ر��ʼ��Ķ�����
            if (MMK_IsOpenWin(MMIMAIL_READ_WIN_ID))
            {       
                MMK_CloseWin(MMIMAIL_READ_WIN_ID);
            }
        }
        break;

    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id, msg_id, param);
        break;
    }
    return (recode);
}

/*****************************************************************************/
//  Description : �����ռ����б�
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void SetRecverList(MMI_CTRL_ID_T ctrl_id, MAIL_MESSAGE_T *editbody_ptr)
{
    uint16          i = 0;
    uint16          addr_count = 0;
    uint32          len = 0;
    MAIL_ADDRESS_T  **mail_addr_list = PNULL;
    wchar           addr_wstr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail SetRecverList PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3622_112_2_18_2_48_12_298,(uint8*)"");
        return ;
    }
    
    switch (GetMailEditFocusIndex())
    {
    case MMIMAIL_RICHTEXT_TO:
        mail_addr_list = editbody_ptr->to;
        addr_count = editbody_ptr->to_count;
        break;

    case MMIMAIL_RICHTEXT_CC:
        mail_addr_list = editbody_ptr->cc;
        addr_count = editbody_ptr->cc_count;
        break;
        
    case MMIMAIL_RICHTEXT_BCC:
        mail_addr_list = editbody_ptr->bcc;
        addr_count = editbody_ptr->bcc_count;
        break;

    default:
        //COMM_TRACE:"MMIMAIL==> SetRecverList: current edit item [%d] is invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3644_112_2_18_2_48_12_299,(uint8*)"d", GetMailEditFocusIndex());
        return;
    }
    
    if (0 == addr_count)
    {
        //COMM_TRACE:"MMIMAIL==> SetRecverList: mail_addr_list:[%d],addr_count:[%d]"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3650_112_2_18_2_48_12_300,(uint8*)"dd", mail_addr_list, addr_count);
        return;
    }

    GUIEDIT_ClearAllStr(ctrl_id);
    
    for (i = 0; i < addr_count; i++)
    {
        MMIAPICOM_StrToWstr((const uint8 *)mail_addr_list[i]->email, addr_wstr);
        len = MMIAPICOM_Wstrlen((const wchar *)addr_wstr);
        GUIEDIT_AddListItem(ctrl_id, addr_wstr, (uint16)len);
    }

}

/*****************************************************************************/
//	Description : �����ռ����б�����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void SaveRecverList(
                                GUIEDIT_LIST_ITEM_STR_T *item_str,  //�б�����ָ��
                                uint16                  list_num,   //�б������
                                MAIL_MESSAGE_T          *editbody_ptr
                             )
{
    uint16          i = 0;
    uint16          *count_ptr = PNULL;
    MAIL_ADDRESS_T  **mail_addr_list = PNULL;
    uint8           addr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
    uint8           alert_flag = 0;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail SaveRecverList PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3683_112_2_18_2_48_12_301,(uint8*)"");
        return ;
    }

    switch (GetMailEditFocusIndex())
    {
    case MMIMAIL_RICHTEXT_TO:
        mail_addr_list = editbody_ptr->to;
        count_ptr = &(editbody_ptr->to_count);
        break;

    case MMIMAIL_RICHTEXT_CC:
        mail_addr_list = editbody_ptr->cc;
        count_ptr = &(editbody_ptr->cc_count);
        break;
        
    case MMIMAIL_RICHTEXT_BCC:
        mail_addr_list = editbody_ptr->bcc;
        count_ptr = &(editbody_ptr->bcc_count);
        break;

    default:
        //COMM_TRACE:"MMIMAIL==> SaveRecverList: current edit item [%d] is invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3705_112_2_18_2_48_12_302,(uint8*)"d", GetMailEditFocusIndex());
        return;
    }

    //����ռ����б�
    for (i = 0; i < *count_ptr; i++)
    {
        SCI_MEMSET(mail_addr_list[i]->email, 0, MMIMAIL_MAX_ADDRESS_LEN + 1);
        SCI_MEMSET(mail_addr_list[i]->name.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_NAME_LEN + 1)));/*lint !e774*/
        mail_addr_list[i]->name.wstr_len = 0;
    }
    
    //����ռ��˸���
    *count_ptr = 0;
    
    if (list_num > 0)
    {
        if (PNULL == item_str)
        {
            //SCI_TRACE_LOW:"MMIMail SaveRecverList PNULL == item_str"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3724_112_2_18_2_48_13_303,(uint8*)"");
            return ;
        }
        //���Ʊ༭���ռ����б�
        for (i = 0; i < list_num; i++)
        {
            if (item_str->is_valid) //��ַ�Ƿ���Ч
            {
                item_str[i].str_ptr[item_str[i].str_len] = 0;
                
                SCI_MEMSET(addr, 0, MMIMAIL_MAX_ADDRESS_LEN + 1);
                MMIAPICOM_WstrToStr((const wchar *)item_str[i].str_ptr, (uint8 *)addr);
                if (mail_isValidEmailAddress((char *)addr))
                {
                    SCI_STRCPY((char *)mail_addr_list[*count_ptr]->email, (char *)addr);
                    (*count_ptr)++;
                    alert_flag |= 1;
                }
                else
                {
                    alert_flag |= 2;
                }
            }
        }
        
        if (2 == alert_flag)
        {
            MMIMAIL_OpenMsgBox(TXT_COMMON_MAIL_ADDR_INVALID, IMAGE_PUBWIN_WARNING);
        }
        else if (3 == alert_flag)
        {
            MMIMAIL_OpenMsgBox(TXT_MAIL_PART_ADDR_INVALID, IMAGE_PUBWIN_WARNING);
        }
        else
        {
            //no need alert
        }
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> SaveRecverList: list_num = %d, clean all receivers address"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3764_112_2_18_2_48_13_304,(uint8*)"d", list_num);
    }
    return;
}

/*****************************************************************************/
//	Description : ��ͨѶ¼�б�
//	Global resource dependence : none
//	Author: yuanl
//	Note:���г�������email��ַ�ļ�¼
/*****************************************************************************/
LOCAL void OpenPbEmailList(void)
{
   /* MMIAPIPB_OpenNumberList(
        MMIPB_LIST_FOR_MMS,
        MMIPB_LIST_TYPE_MUTIPLE_FORALL,
        (uint16)MMIMAIL_MAX_RECEIVER_NUM,
        PNULL
        );*/
    MMIAPIPB_OpenMainWin(
            MMIPB_ALL_CONTACT,//
            MMIPB_LIST_CHECK, //IN: �����б������
            MMIPB_LIST_FIELD_MAIL,
            MMIMAIL_MAX_RECEIVER_NUM,//����Ƕ�ѡʱ�����������˶�ѡ��������
            PNULL,
            AddReciveContactCallback
            );      
}

/*****************************************************************************/
//  Description :list box add string
//  Global resource dependence : 
//  Author:yuanl
//  Note: 
/*****************************************************************************/
LOCAL void ToListBoxSetString(MMI_CTRL_ID_T ctrl_id, const wchar *str_ptr, const uint16 str_len)
{
    wchar * cur_wstr                              = PNULL;
    uint16 cur_to_len                             = 0;
    wchar cur_to_wstr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar *list_wstr                              = PNULL;
    uint16 temp_len                               = 0;
    uint16 num_len                                = 0;

    if (PNULL == str_ptr || 0 == str_len)
    {
        return;
    }
    temp_len = (str_len + 1) * sizeof(wchar);
    list_wstr = SCI_ALLOC_APP(temp_len);
    SCI_MEMSET(list_wstr, 0, temp_len);
    MMI_WSTRNCPY(list_wstr, str_len, str_ptr, str_len, str_len);

    cur_wstr = list_wstr;
    GUIEDIT_ClearAllStr(ctrl_id);
    while ('\0' != *cur_wstr)
    {
        cur_to_len++;
        if (';' == *cur_wstr || '\0' == *(cur_wstr + 1))
        {
            if (';' != *cur_wstr && '\0' == *(cur_wstr + 1))
            {
                num_len = MIN(MMIMAIL_MAX_ADDRESS_LEN, cur_to_len);
                MMI_WSTRNCPY(cur_to_wstr, MMIMAIL_MAX_ADDRESS_LEN, cur_wstr - cur_to_len + 1, 
                    MMIMAIL_MAX_ADDRESS_LEN, num_len);
                GUIEDIT_AddListItem(ctrl_id, cur_to_wstr, num_len);
                cur_to_len = 0;
            }
            else
            {
                num_len = MIN(MMIMAIL_MAX_ADDRESS_LEN, cur_to_len - 1);
                MMI_WSTRNCPY(cur_to_wstr, MMIMAIL_MAX_ADDRESS_LEN, cur_wstr - cur_to_len + 1, 
                    MMIMAIL_MAX_ADDRESS_LEN, num_len);
                GUIEDIT_AddListItem(ctrl_id, cur_to_wstr, num_len);
                cur_to_len = 0;
            }
        }
        cur_wstr = cur_wstr + 1;        
    }
    
    SCI_FREE(list_wstr);
    list_wstr = PNULL;
}

/*****************************************************************************/
//	Description : ��ȡ�绰��ѡ����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void GetPbSelReturn(MMIPB_HANDLE_T handle, MMI_CTRL_ID_T ctrl_id)
{
    uint16 pb_select_num = 0;
    uint16 to_number = 0;
    uint16 all_number_len = 0;
    uint16 index = 0;
    uint16 tele_len = 0;
    uint32 temp_len = 0;
    wchar *all_number_wstr = PNULL;
    GUIEDIT_LIST_ITEM_STR_T *num_str_ptr = PNULL;
    wchar  tele_wstr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
    wchar  semicolon[] = L";";
    MMI_STRING_T email_addr = {0};
    char   addr[MMIMAIL_MAX_ADDRESS_LEN + 1] = {0};
    
    pb_select_num = MMIAPIPB_GetSelectCount(handle);
    if (pb_select_num > 0)
    {
        temp_len = (MMIMAIL_ALL_ADDR_LEN + MMIMAIL_MAX_RECEIVER_NUM + 1) * sizeof(wchar);
        all_number_wstr = (wchar *)comm_alloc(temp_len);
        if (PNULL == all_number_wstr)
        {
            //COMM_TRACE:"MMIMAIL==> GetPbSelReturn: malloc all_number_wstr failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3870_112_2_18_2_48_13_305,(uint8*)"");
            MMIMAIL_OpenMsgBox(TXT_MAIL_NO_MEMORY, IMAGE_PUBWIN_WARNING);
            return;
        }
        SCI_MEMSET(all_number_wstr, 0, temp_len);

        {//����ϴεĺ���β��û�зֺ�����
            to_number = GUIEDIT_GetListItemNum(ctrl_id);
            if (0 < to_number)
            {
                num_str_ptr = comm_alloc(to_number*sizeof(GUIEDIT_LIST_ITEM_STR_T));
                if (PNULL == num_str_ptr)
                {
                    comm_free(all_number_wstr);/*lint !e774*/
                    //COMM_TRACE:"MMIMAIL==> GetPbSelReturn: malloc num_str_ptr failed"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3884_112_2_18_2_48_13_306,(uint8*)"");
                    return;
                }
                GUIEDIT_GetListItemString(ctrl_id,to_number,num_str_ptr);
                all_number_len = 0;
                for (index = 0; index < to_number; index++)
                {
                    tele_len = MIN(MMIMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_len);
                    MMI_WSTRNCPY(tele_wstr, MMIMAIL_MAX_ADDRESS_LEN, num_str_ptr[index].str_ptr, MMIMAIL_MAX_ADDRESS_LEN, tele_len);                
                    if (tele_len > 0)
                    {
                        MMI_WSTRNCPY(all_number_wstr + all_number_len, MMIMAIL_ALL_ADDR_LEN, 
                            tele_wstr, MMIMAIL_MAX_ADDRESS_LEN, tele_len);                
                        all_number_len = tele_len + all_number_len;
                        if (MMIAPICOM_Wstrcmp(all_number_wstr + all_number_len - 1, semicolon) != 0)
                        {
                            MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                            all_number_len++;
                        }
                    }
                    SCI_MEMSET((char *)tele_wstr, 0, sizeof(tele_wstr));
                }
                comm_free(num_str_ptr);/*lint !e774*/
            }
        }
        
        SCI_MEMSET(&email_addr, 0, sizeof(email_addr));
        email_addr.wstr_ptr = (wchar *)comm_alloc((MMIMAIL_MAX_ADDRESS_LEN + 1)*sizeof(wchar));
        if (PNULL == email_addr.wstr_ptr)
        {
            //COMM_TRACE:"MMIMAIL==> GetPbSelReturn: malloc email_addr failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3914_112_2_18_2_48_13_307,(uint8*)"");
            comm_free(all_number_wstr);/*lint !e774*/
            MMIMAIL_OpenMsgBox(TXT_MAIL_NO_MEMORY, IMAGE_PUBWIN_WARNING);
            return;
        }
        SCI_MEMSET(email_addr.wstr_ptr, 0, ((MMIMAIL_MAX_ADDRESS_LEN + 1) * sizeof(wchar)));           
  
        for (index = 0; index < MIN(MMIMAIL_MAX_RECEIVER_NUM, pb_select_num); index++)
        {
            MMIAPIPB_GetSelectEmail(handle, &email_addr, MMIMAIL_MAX_ADDRESS_LEN, index);
            if (0 != email_addr.wstr_len)
            {
                if (MMIAPICOM_IsASCIIString(email_addr.wstr_ptr, email_addr.wstr_len))
                {     
                    MMIAPICOM_WstrToStr(email_addr.wstr_ptr, (uint8 *)addr);
                    if (mail_isValidEmailAddress(addr))
                    {
                        MMIAPICOM_Wstrncat(all_number_wstr, email_addr.wstr_ptr, email_addr.wstr_len);
                        all_number_len = all_number_len + email_addr.wstr_len;
                        MMIAPICOM_Wstrncat(all_number_wstr, semicolon, 1);
                        all_number_len = all_number_len +1;
                    }
                    else
                    {
                        MMIMAIL_OpenMsgBox(TXT_COMMON_MAIL_ADDR_INVALID, IMAGE_PUBWIN_WARNING);
                    }
                }
            }                
        }  

        //��ʾ��Ӻ���
        ToListBoxSetString(
            ctrl_id,
            all_number_wstr,
            all_number_len
            );

        //�ͷ�email_addr
        if (PNULL != email_addr.wstr_ptr)
        {
            comm_free(email_addr.wstr_ptr);
        }
        comm_free(all_number_wstr);/*lint !e774*/
    }    

    return;
}

/*****************************************************************************/
//  Description :��д�ռ����б�����
//  Global resource dependence : 
//  Author:yuanl
//  Note:    
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditRecverListWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_RECVER_LIST_CTRL_ID;
    MAIL_MESSAGE_T   *editbody_ptr = s_mail_edit_global.editbody_ptr;
    MMI_TEXT_ID_T   text_id = TXT_NULL;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleMailEditRecverListWinMsg PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_3979_112_2_18_2_48_13_308,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        //���ñ༭���
        GUIEDIT_SetListStyle(ctrl_id, GUIEDIT_LIST_STYLE_ALL);

        GUIEDIT_SetIm(ctrl_id, GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL, GUIIM_TYPE_ABC);
        GUIEDIT_SetImTag(ctrl_id, GUIIM_TAG_EMAIL);
        
        //�����б�����
        SetRecverList(ctrl_id, editbody_ptr);
        
        //������ʱ��ʾ ͨѶ¼| |���أ�������ʱ��ʾȷ��|ͨѶ¼|ɾ��
//    	GUIEDIT_SetSoftkey(ctrl_id, 0, 1, STXT_MAIN_PB, STXT_OK, PNULL);//set left softkey
//    	GUIEDIT_SetSoftkey(ctrl_id, 1, 1, TXT_NULL, STXT_MAIN_PB, PNULL);//set right softkey

        //���ñ���
        if (MMIMAIL_RICHTEXT_CC == GetMailEditFocusIndex())
        {
            text_id = TXT_MAIL_CC;
        }
        else if (MMIMAIL_RICHTEXT_BCC == GetMailEditFocusIndex())
        {
            text_id = TXT_MAIL_BCC;
        }
        else
        {
            text_id = TXT_MAIL_TO;
        }
        GUIWIN_SetTitleTextId(win_id, text_id, TRUE);
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        OpenPbEmailList();
        break; 
    
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    {
        uint16          list_num = 0;
        GUIEDIT_LIST_ITEM_STR_T   *item_str = PNULL;
        
        list_num = GUIEDIT_GetListItemNum(ctrl_id);

        if (list_num > 0)
        {
            if (list_num > MMIMAIL_MAX_RECEIVER_NUM)
            {
                MMIMAIL_OpenMsgBox(TXT_MAIL_ADD_RECVER_FULL, IMAGE_PUBWIN_WARNING);
                break;
            }

            item_str = comm_alloc(list_num * sizeof(GUIEDIT_LIST_ITEM_STR_T));
            if (PNULL == item_str)
            {
                //COMM_TRACE:"MMIMAIL==> HandleMailEditRecverListWinMsg: alloc GUIEDIT_LIST_ITEM_STR_T fail"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4042_112_2_18_2_48_13_309,(uint8*)"");
                break;
            }
            GUIEDIT_GetListItemString(ctrl_id,list_num,item_str);

            //����༭����
            SaveRecverList(item_str, list_num, editbody_ptr);
            comm_free(item_str);/*lint !e774*/
        }
        else
        {
            //��������ռ���
            SaveRecverList(PNULL, 0, editbody_ptr);
        }  

        MailEditEntry(MMIMAIL_EDIT_RICHTEXT_CTRL_ID, editbody_ptr, MMIMAIL_RICHTEXT_MOD_ITEM);

        MMK_CloseWin(win_id);
        
        break;
    }
    
    case MMI_SELECT_CONTACT_CNF:
        GetPbSelReturn((MMIPB_HANDLE_T)param, ctrl_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    default:
        err_code = MMI_RESULT_FALSE;
        break;
    }

    return err_code;
}                                             

/*****************************************************************************/
//  Description : �����ı�����(���������)
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void SetMailText(MMI_CTRL_ID_T ctrl_id, MAIL_MESSAGE_T *editbody_ptr)
{
    wchar   *text_ptr = PNULL;
    uint16  text_len = 0;
    uint16  len = 0;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail SetMailText PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4093_112_2_18_2_48_14_310,(uint8*)"");
        return ;
    }
    
    switch (GetMailEditFocusIndex())
    {
    case MMIMAIL_RICHTEXT_SUBJECT:
        text_ptr = editbody_ptr->subject.wstr_ptr;
        text_len = editbody_ptr->subject.wstr_len;
        break;

    case MMIMAIL_RICHTEXT_CONTENT:
        text_ptr = editbody_ptr->textbody.wstr_ptr;
        text_len = editbody_ptr->textbody.wstr_len;

        //�༭ԭʼ�ʼ�ʱ�����������ݵ����
        //�½����ظ���ת��ʱ��������ǩ����ԭ�ʼ�����֮ǰ
        if (MMIMAIL_OPEN_TYPE_ORIGINAL != s_mail_edit_global.open_type)
        {
            len += s_mail_edit_global.sign_text_size;
            
            if ((MMIMAIL_OPEN_TYPE_DEFAULT != s_mail_edit_global.open_type)
                && (MMIMAIL_OPEN_TYPE_NEW_MAIL != s_mail_edit_global.open_type))
            {
                len += s_mail_edit_global.org_text_size;
            }
        }
        break;
        
    default:
        //COMM_TRACE:"MMIMAIL==> SetMailText: current edit item [%d] is invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4123_112_2_18_2_48_14_311,(uint8*)"d", GetMailEditFocusIndex());
        return;
    }
    
    if ((PNULL != text_ptr) && (text_len > 0))
    {
        GUIEDIT_SetString(ctrl_id, text_ptr, text_len);

        text_len = (text_len >= len) ? (text_len - len) : 0;
        
        GUIEDIT_SetCursorCurPos(ctrl_id, text_len);
    }
}

/*****************************************************************************/
//  Description : �����ı�����(���������)
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL void SaveMailText(MMI_CTRL_ID_T ctrl_id, MAIL_MESSAGE_T *editbody_ptr)
{
    MMI_STRING_T *text_ptr = PNULL;
    MMI_STRING_T tmp_str = {0};
    uint32       max_len = 0;
    uint32       copy_len = 0;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail SaveMailText PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4150_112_2_18_2_48_14_312,(uint8*)"");
        return ;
    }
    
    switch (GetMailEditFocusIndex())
    {
    case MMIMAIL_RICHTEXT_SUBJECT:
        text_ptr = &(editbody_ptr->subject);
        max_len = MMIMAIL_MAX_SUBJECT_LEN;
        break;

    case MMIMAIL_RICHTEXT_CONTENT:
        text_ptr = &(editbody_ptr->textbody);
        max_len = MMIMAIL_MAX_CONTENT_LEN;
        break;
        
    default:
        //COMM_TRACE:"MMIMAIL==> SaveMailText: current edit item [%d] is invalid"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4167_112_2_18_2_48_14_313,(uint8*)"d", GetMailEditFocusIndex());
        return;
    }
    
    GUIEDIT_GetString(ctrl_id, &tmp_str);
    
    SCI_MEMSET(text_ptr->wstr_ptr, 0, (sizeof(wchar) * (max_len + 1)));
    text_ptr->wstr_len = 0;
    copy_len = MIN(max_len, tmp_str.wstr_len);
    MMIAPICOM_Wstrncpy(text_ptr->wstr_ptr, (const wchar *)tmp_str.wstr_ptr, copy_len);
    text_ptr->wstr_len = copy_len;
}

/*****************************************************************************/
//	Description : �ı��༭���洦����
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleMailEditTextWinMsg(
                                             MMI_WIN_ID_T       win_id,  
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    err_code = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_TEXT_CTRL_ID;
    MAIL_MESSAGE_T   *editbody_ptr = s_mail_edit_global.editbody_ptr;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleMailEditTextWinMsg PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4196_112_2_18_2_48_14_314,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    //COMM_TRACE:"MMIMAIL==> HandleMailEditTextWinMsg msg_id = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4200_112_2_18_2_48_14_315,(uint8*)"d", msg_id);
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        //��ʾ�ı�
        SetMailText(ctrl_id, editbody_ptr);
        GUIEDIT_SetHandleRedKey(FALSE, ctrl_id);
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
    case MSG_CTL_OK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_APP_OK:
    case MSG_APP_RED:
        SaveMailText(ctrl_id, editbody_ptr);
        MailEditEntry(MMIMAIL_EDIT_RICHTEXT_CTRL_ID, editbody_ptr, MMIMAIL_RICHTEXT_MOD_ITEM);

        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    default :
        err_code = MMI_RESULT_FALSE;
        break;
    }    
    
    return err_code;
}

/*****************************************************************************/
//  Description : ��Ӹ����б���
//  Global resource dependence : 
//  Author: yuanl
//  Note:  
/*****************************************************************************/
LOCAL void AppendEditAccListItem(
                                            MMI_CTRL_ID_T           ctrl_id,
                                            MMI_TEXT_ID_T           left_softkey_id,
                                            MMI_TEXT_ID_T           mid_softkey_id,
                                            MMI_TEXT_ID_T           right_softkey_id,
                                            const wchar*            filename,
                                            uint16                  filename_len,
                                        	uint32                  filesize
                                          )
{
    GUILIST_ITEM_T       item_t    =  {0};
    GUILIST_ITEM_DATA_T  item_data = {0};
    wchar temp_wstr[GUILIST_STRING_MAX_NUM + 1] = {0};
    
    if (filename_len == 0)
    {
        //SCI_TRACE_LOW:"MMIMail AppendEditAccListItem filename_len <= 0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4255_112_2_18_2_48_14_316,(uint8*)"");
        return ;
    }
    
    if (PNULL == filename)
    {
        //SCI_TRACE_LOW:"MMIMail AppendEditAccListItem PNULL == filename"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4261_112_2_18_2_48_14_317,(uint8*)"");
        return ;
    }
    
	item_t.item_style = GUIITEM_STYLE_ONE_LINE_ICON_TWO_TEXT;    
    item_t.item_data_ptr = &item_data;

    //����softkey
    item_data.softkey_id[0] = left_softkey_id;
    item_data.softkey_id[1] = mid_softkey_id;
    item_data.softkey_id[2] = right_softkey_id;
    
    //�����ļ���������ͼ��
    item_data.item_content[0].item_data_type     = GUIITEM_DATA_IMAGE_ID;
    item_data.item_content[0].item_data.image_id = MMIAPIFMM_GetIconIdByFileType( MMIFMM_FILE_TYPE_NORMAL, MMI_DEVICE_UDISK );

    //�����ļ���
    item_data.item_content[1].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    item_data.item_content[1].item_data.text_buffer.wstr_ptr = (wchar *)filename;
    item_data.item_content[1].item_data.text_buffer.wstr_len = filename_len;

    //�����ļ���С
    item_data.item_content[2].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
    MMIAPIFMM_GetFileSizeString( filesize, temp_wstr, MMIMAIL_FILE_SIZE_STR_LEN, TRUE);
    item_data.item_content[2].item_data.text_buffer.wstr_ptr = temp_wstr;
    item_data.item_content[2].item_data.text_buffer.wstr_len = (uint16)MMIAPICOM_Wstrlen( temp_wstr );

    GUILIST_AppendItem(ctrl_id, &item_t);
}

/*****************************************************************************/
//  Description : ���ø����б�
//  Global resource dependence : 
//  Author: yuanl
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetEditAccList(MMI_CTRL_ID_T ctrl_id, MAIL_MESSAGE_T *editbody_ptr)
{
    uint16          i = 0;
    BOOLEAN         result = FALSE;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail SetEditAccList PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4302_112_2_18_2_48_14_318,(uint8*)"");
        return FALSE;
    }
    
    GUILIST_RemoveAllItems(ctrl_id);
    
    s_mail_edit_global.total_acc_size = 0;
    
    if (editbody_ptr->partcount > 0)
    {
        for (i = 0; i < editbody_ptr->partcount; i++)
        {
            AppendEditAccListItem(ctrl_id,
                                    STXT_OPTION,
                                    TXT_NULL,
                                    STXT_RETURN,
                                    editbody_ptr->mailmessageparts[i]->filename.wstr_ptr,
                                    editbody_ptr->mailmessageparts[i]->filename.wstr_len,
                                    editbody_ptr->mailmessageparts[i]->size
                                    );
            s_mail_edit_global.total_acc_size += editbody_ptr->mailmessageparts[i]->size;                        
        }
        result = TRUE;
    }
    else
    {
        MMIMAIL_AppendEmptyStr2List(ctrl_id, TXT_BLANK, STXT_ADD);
    }

    return result;
}

/*****************************************************************************/
//	Description : ����menu���
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void InsertOptMenuNodeByLabel(
                             MMI_CTRL_ID_T      ctrl_id,        //�ؼ�id
                             MMI_TEXT_ID_T      text_id,        //����Ľڵ���ı�
                             uint16             node_id,        //�ڵ���
                             uint16             parent_node_id, //���ڵ���
                             uint16             index           //λ��
                             )
{
    MMI_STRING_T            kstring = {0};
    GUIMENU_DYNA_ITEM_T     node_item = {0};

    MMI_GetLabelTextByLang(text_id, &kstring);
    node_item.item_text_ptr = &kstring;
    GUIMENU_InsertNode(index, node_id, parent_node_id, &node_item, ctrl_id);
}  

/*****************************************************************************/
//	Description : �򿪸����༭ѡ���
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL void  OpenEditAccOptWin(void)
{
    MMI_WIN_ID_T                win_id = MMIMAIL_EDIT_ACC_OPT_MENU_WIN_ID;
    MMI_CTRL_ID_T               ctrl_id = MMIMAIL_EDIT_ACC_OPT_MENU_CTRL_ID;

    MMK_CreateWin((uint32 *)MMIMAIL_EDIT_ACC_OPT_MENU_WIN_TAB,PNULL);
    
    //������̬ѡ��˵�
    GUIMENU_CreatDynamic(PNULL, win_id, ctrl_id, GUIMENU_STYLE_POPUP);

    InsertOptMenuNodeByLabel(ctrl_id, TXT_COMMON_MAIL_ADDR_INVALID, MMIMAIL_NODE_EDIT_ACC_OPT_ADD, 0, 0);
    InsertOptMenuNodeByLabel(ctrl_id, TXT_DELETE, MMIMAIL_NODE_EDIT_ACC_OPT_DEL, 0, 1);

    return;
}

/*****************************************************************************/
//	Description : �򿪸���ѡ�񴰿�
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL void OpenSelectAccWin(MMI_WIN_ID_T pre_win_id)
{
    MMIFMM_SELECT_INFO_T  select_info = {MMIFMM_FILE_ALL,0,0,0,PNULL,0,-1};
    
    if (s_mail_edit_global.total_acc_size > MMIMAIL_MAX_ACC_SIZE)
    {
        MMIPUB_OpenAlertWarningWin(TXT_MAIL_REACH_MAXSIZE_LIMIT);
        return;
    }

    select_info.max_size = MMIMAIL_MAX_ACC_SIZE - s_mail_edit_global.total_acc_size;
    select_info.win_id = pre_win_id;
    select_info.ring_vol = -1;
    
    if(!MMIAPIFMM_OpenSelectAllFileWin(&select_info))
    {
        MMIMAIL_OpenMsgBox(TXT_ADD_FAIL, IMAGE_PUBWIN_WARNING);
    } 
    
    return;
}

/*****************************************************************************/
//	Description : ������ѡ���ļ���Ϣ
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN SaveSelectAccFile(
                                    MMIFMM_SELECT_RETURN_T  *selected_info,
                                    MAIL_MESSAGE_T          *editbody_ptr
                                    )
{
    BOOLEAN result = FALSE;
    MAIL_MESSAGE_PART_T *acc_ptr = PNULL;
    MMIFILE_FILE_INFO_T file_info = {0};
    uint32 insert_total_size = 0;
    uint32 copy_len = 0;
    int16  free_index = -1;
    uint16 i = 0;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail SaveSelectAccFile PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4419_112_2_18_2_48_14_319,(uint8*)"");
        return FALSE;
    }

    if (PNULL == selected_info)
    {
        //COMM_TRACE:"MMIMAIL==> SaveSelectAccFile: selected_info is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4425_112_2_18_2_48_14_320,(uint8*)"");
        return result;
    }
    
    MMIAPIFMM_GetFileInfoFormFullPath(selected_info->file_name_ptr,selected_info->file_name_len,&file_info);

    insert_total_size = s_mail_edit_global.total_acc_size + file_info.file_size;
    if (0 == file_info.file_size)
    {
        MMIMAIL_OpenMsgBox(TXT_ADD_FAIL, IMAGE_PUBWIN_WARNING);
    }
    else if (insert_total_size > MMIMAIL_MAX_ACC_SIZE)
    {
        MMIMAIL_OpenMsgBox(TXT_MAIL_REACH_MAXSIZE_LIMIT, IMAGE_PUBWIN_WARNING);
    }
    else
    {
        s_mail_edit_global.total_acc_size += file_info.file_size;

        //��������λ��
        for (i = 0; i < MMIMAIL_MAX_ACC_NUM; i++)
        {
            if (0 == editbody_ptr->mailmessageparts[i]->filename.wstr_len)
            {
                free_index = i;
                break;
            }
        }

        if ((free_index >= 0) && (free_index < MMIMAIL_MAX_ACC_NUM))
        {
            //����ѡ���ļ�����Ϣ
            acc_ptr = editbody_ptr->mailmessageparts[free_index];
            
            copy_len = MIN(file_info.file_name_len, MMIMAIL_MAX_FILENAME_LEN);
            MMIAPICOM_Wstrncpy(acc_ptr->filename.wstr_ptr, 
                                (const wchar *)file_info.file_name, 
                                copy_len);
            acc_ptr->filename.wstr_len = (uint16)MMIAPICOM_Wstrlen(acc_ptr->filename.wstr_ptr);
            
            copy_len = MIN(selected_info->file_name_len, MMIMAIL_MAX_FULLNAME_LEN);
            MMIAPICOM_Wstrncpy(acc_ptr->fullname.wstr_ptr, 
                                (const wchar *)selected_info->file_name_ptr, 
                                copy_len);
            acc_ptr->fullname.wstr_len = (uint16)MMIAPICOM_Wstrlen(acc_ptr->fullname.wstr_ptr);
            
            acc_ptr->size = file_info.file_size;
            
            editbody_ptr->partcount ++;

            result = TRUE;
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> SaveSelectAccFile: free_index = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4479_112_2_18_2_48_14_321,(uint8*)"d", free_index);
            MMIMAIL_OpenMsgBox(TXT_MAIL_REACH_MAXSIZE_LIMIT, IMAGE_PUBWIN_WARNING);
        }
    }

    return result;
}

/*****************************************************************************/
//	Description : ɾ��ѡ��ĸ���
//	Global resource dependence : none
//	Author: yuanl
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN DelSelectAcc(MMI_WIN_ID_T   pre_win_id)
{
    BOOLEAN result = FALSE;
    MAIL_MESSAGE_T   *editbody_ptr = s_mail_edit_global.editbody_ptr;
    int16   index = -1;
    uint32  i = 0;
    MAIL_MESSAGE_PART_T *acc_ptr = PNULL;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail DelSelectAcc PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4501_112_2_18_2_48_14_322,(uint8*)"");
        return FALSE;
    }
    
    if ((PNULL != s_cur_edit_acc) && (editbody_ptr->partcount >= 1))
    {
        //Ѱ�ҵ�ǰѡ�񸽼���index
        for (i = 0; i < editbody_ptr->partcount; i++)
        {
            if (s_cur_edit_acc == editbody_ptr->mailmessageparts[i])
            {
                index = i;
                break;
            }
        }

        if ((index >= 0) && (index < editbody_ptr->partcount))
        {
            //�ҵ�ƥ��ĸ���,��յ�ǰѡ�и���
            SCI_MEMSET(s_cur_edit_acc->filename.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_FILENAME_LEN + 1)));
            s_cur_edit_acc->filename.wstr_len = 0;

            SCI_MEMSET(s_cur_edit_acc->fullname.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_FULLNAME_LEN + 1)));
            s_cur_edit_acc->fullname.wstr_len = 0;

            s_cur_edit_acc->size = 0;

            if (PNULL != s_cur_edit_acc->content)
            {
                comm_free(s_cur_edit_acc->content);
                s_cur_edit_acc->content_l = 0;
            }
            s_cur_edit_acc->attr_has_data = FALSE;
            s_cur_edit_acc = PNULL;
            
            //������ĸ�����ǰŲ
            if ((index + 1) < editbody_ptr->partcount) //���滹�и���
            {
                for (i = index + 1; i < editbody_ptr->partcount; i++)
                {
                    if (editbody_ptr->mailmessageparts[i]->filename.wstr_len > 0)
                    {
                        SCI_MEMSET(editbody_ptr->mailmessageparts[i-1]->filename.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_FILENAME_LEN + 1)));
                        MMIAPICOM_Wstrncpy(editbody_ptr->mailmessageparts[i-1]->filename.wstr_ptr, 
                            (const wchar *)editbody_ptr->mailmessageparts[i]->filename.wstr_ptr, 
                            editbody_ptr->mailmessageparts[i]->filename.wstr_len);
                        editbody_ptr->mailmessageparts[i-1]->filename.wstr_len = editbody_ptr->mailmessageparts[i]->filename.wstr_len;
                    }
                    
                    if (editbody_ptr->mailmessageparts[i]->fullname.wstr_len > 0)
                    {
                        SCI_MEMSET(editbody_ptr->mailmessageparts[i-1]->fullname.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_FULLNAME_LEN + 1)));
                        MMIAPICOM_Wstrncpy(editbody_ptr->mailmessageparts[i-1]->fullname.wstr_ptr, 
                            (const wchar *)editbody_ptr->mailmessageparts[i]->fullname.wstr_ptr, 
                            editbody_ptr->mailmessageparts[i]->fullname.wstr_len);
                        editbody_ptr->mailmessageparts[i-1]->fullname.wstr_len = editbody_ptr->mailmessageparts[i]->fullname.wstr_len;
                    }

                    editbody_ptr->mailmessageparts[i-1]->size = editbody_ptr->mailmessageparts[i]->size;
                }

                //������һ��
                acc_ptr = editbody_ptr->mailmessageparts[editbody_ptr->partcount-1];
                SCI_MEMSET(acc_ptr->filename.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_FILENAME_LEN + 1)));
                acc_ptr->filename.wstr_len = 0;

                SCI_MEMSET(acc_ptr->fullname.wstr_ptr, 0, (sizeof(wchar) * (MMIMAIL_MAX_FULLNAME_LEN + 1)));
                acc_ptr->fullname.wstr_len = 0;

                acc_ptr->size = 0;
            }
            
            //����������1
            editbody_ptr->partcount--;

            //֪ͨ�б�������
            i = (editbody_ptr->partcount > 0) ? MIN(index, editbody_ptr->partcount-1) : 0;
            MMK_SendMsg(pre_win_id, MSG_MAIL_UPDATE_ACC_LIST, (DPARAM)i);
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> DelSelectAcc: index = %d is invalid"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4582_112_2_18_2_48_14_323,(uint8*)"d", index);
        }
        
    }
    else
    {
        //COMM_TRACE:"MMIMAIL==> DelSelectAcc: current edit acc is null!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4588_112_2_18_2_48_15_324,(uint8*)"");
        //COMM_TRACE:"MMIMAIL==> DelSelectAcc: editbody_ptr->partcount = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4589_112_2_18_2_48_15_325,(uint8*)"d", editbody_ptr->partcount);
    }

    return result;
}

/*****************************************************************************/
//	Description : �༭�����б��ڴ�����
//	Global resource dependence : none
//	Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditAccListWinMsg(
                                      MMI_WIN_ID_T        win_id,        //IN:
                                      MMI_MESSAGE_ID_E    msg_id,        //IN:
                                      DPARAM              param          //IN:
                                      )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_ACC_LISTBOX_CTRL_ID;
    uint16          list_item_num = 0;
    LOCAL BOOLEAN   have_items = FALSE;
    uint32          focus_index = 0;
    MAIL_MESSAGE_T   *editbody_ptr = s_mail_edit_global.editbody_ptr;
    
    if (PNULL == editbody_ptr)
    {
        //SCI_TRACE_LOW:"MMIMail HandleEditAccListWinMsg PNULL == editbody_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4614_112_2_18_2_48_15_326,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    
    switch (msg_id)
    {
    case MSG_OPEN_WINDOW:
        GUILIST_SetNeedPrgbar(ctrl_id, TRUE);
        list_item_num = MAX(editbody_ptr->partcount, 1);
        GUILIST_SetMaxItem(ctrl_id, list_item_num, FALSE);

        //���ظ����б���
        have_items = SetEditAccList(ctrl_id, editbody_ptr);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        s_cur_edit_acc = PNULL;
        
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
        if ( have_items )
        {
            uint16 selected_index = 0;
            
            selected_index = GUILIST_GetCurItemIndex(ctrl_id);

            //���浱ǰ���ڲ����ĸ���
            s_cur_edit_acc = editbody_ptr->mailmessageparts[selected_index];

            //��ѡ��˵�
            OpenEditAccOptWin();
        }
        else
        {
            //ִ����Ӹ�������
            OpenSelectAccWin(win_id);
        }

        break;

    case MSG_MULTIM_SELECTED_RETURN:
    {
        MMIFMM_SELECT_RETURN_T *selected_info = (MMIFMM_SELECT_RETURN_T *)param;       
        BOOLEAN is_success = FALSE;
        
        if (PNULL != selected_info)
        {
            //������ѡ�ļ�
            is_success = SaveSelectAccFile(selected_info, editbody_ptr);

            //�����б�
            if (is_success)
            {
                MMK_SendMsg(win_id, MSG_MAIL_UPDATE_ACC_LIST, (DPARAM)(editbody_ptr->partcount - 1));
            }
        }
        else
        {
            //COMM_TRACE:"MMIMAIL==> HandleEditAccListWinMsg: param is NULL"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4685_112_2_18_2_48_15_327,(uint8*)"");
        }
        break;
    }  

    case MSG_MAIL_UPDATE_ACC_LIST:
        focus_index = (uint32)param;
        list_item_num = MAX(editbody_ptr->partcount, 1);
        GUILIST_SetMaxItem(ctrl_id, list_item_num, FALSE);

        //���ظ����б���
        have_items = SetEditAccList(ctrl_id, editbody_ptr);

        if (have_items)
        {            
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_DEFAULT );
        }
        else
        {
            GUILIST_SetTitleIndexType( ctrl_id, GUILIST_TITLE_INDEX_EXCLUDE_FIRST );
        }

        GUILIST_SetCurItemIndex(ctrl_id, focus_index);

        MailEditEntry(MMIMAIL_EDIT_RICHTEXT_CTRL_ID, editbody_ptr, MMIMAIL_RICHTEXT_MOD_ITEM);
        break;
        
    case MSG_CTL_CANCEL:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CLOSE_WINDOW:
        break;
        
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

	return recode;
}

/*****************************************************************************/
// 	Description : �༭�����б����ѡ��˵�������
//	Global resource dependence : none
//  Author:yuanl
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleEditAccMenuOptWinMsg(
                                               MMI_WIN_ID_T        win_id, 
                                               MMI_MESSAGE_ID_E    msg_id, 
                                               DPARAM              param
                                               )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIMAIL_EDIT_ACC_OPT_MENU_CTRL_ID;
    uint32          node_id = 0;
    
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        MMK_SetAtvCtrl(win_id, ctrl_id);
        break;
        
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_OK:
    case MSG_APP_OK:
    case MSG_APP_WEB:
    case MSG_CTL_MIDSK:
        node_id = GUIMENU_GetCurNodeId(ctrl_id);
        switch (node_id)
        {
        case MMIMAIL_NODE_EDIT_ACC_OPT_ADD:
            OpenSelectAccWin(MMIMAIL_EDIT_ACC_LIST_WIN_ID);
            break;
        
        case MMIMAIL_NODE_EDIT_ACC_OPT_DEL:
            DelSelectAcc(MMIMAIL_EDIT_ACC_LIST_WIN_ID);
            break;
            
        default: 
            //SCI_TRACE_LOW:"MMIMail HandleEditAccMenuOptWinMsg node_id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMAIL_EDIT_4765_112_2_18_2_48_15_328,(uint8*)"d", node_id);
            return MMI_RESULT_FALSE;
        }
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
//	Description : Is Mail empty
//	Global resource dependence : none
//	Author:Jiaoyou.wu
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMAIL_IsEmpty(MAIL_MESSAGE_T *editbody_ptr)
{
    BOOLEAN b_ret = TRUE;

    if (PNULL == editbody_ptr)
    {
        return TRUE;
    }

    if ((editbody_ptr->to_count > 0) || (editbody_ptr->cc_count > 0) 
        || (editbody_ptr->bcc_count > 0) || (editbody_ptr->partcount > 0)
        || (editbody_ptr->subject.wstr_len > 0) || (editbody_ptr->textbody.wstr_len > 0))
    {
       b_ret = FALSE;
    }

    return b_ret;
}


/*Edit by script, ignore 6 case. Fri Apr 27 09:38:53 2012*/ //IGNORE9527
