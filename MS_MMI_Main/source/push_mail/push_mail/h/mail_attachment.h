/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_attachment.h
 *	Author		: sunguochen    
 *	Description	: ��������
 *	Vesion		: 1.0        
 *	Date			: 2010-5-20  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-5-20      1.0           ������д���
 *====*====*====*====*====*====*====*====*====*====*====*====*/



#ifndef _MAIL_ATTACHMENT_H_
#define _MAIL_ATTACHMENT_H_
#include "mail_pop3.h"


//�����ļ�����
typedef struct _mail_att_file_info_
{
    int                   file_size;
    comm_string_t  file_name;

}mail_att_file_info;


//ת����ȫ���ļ�����
typedef struct _mail_att_multi_file_list_
{
    int     file_num;
    mail_att_file_info *file_info;

}mail_att_multi_file_list;


/*==========================================================
 * Function 	: mail_createAttachTmpFile
 * Author		: sunguochen      
 * Description : �������ظ�������ʱ�ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_createAttachTmpFile(char *temp_filePath, char* uidl, char* contentid);



/*==========================================================
 * Function 	: mail_IsAttachFileExist
 * Author		: sunguochen      
 * Description : �жϸ����Ƿ��Ѵ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_IsAttachFileExist(char* uidl, char* contentid, MMI_STRING_T* filename);



/*==========================================================
 * Function 	: mail_AttachFileName
 * Author		: sunguochen      
 * Description  : ��ø����ļ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	    : created  5/20/2010 
 ==========================================================*/
wchar* mail_AttachFileName(char* uidl, char* contentid, comm_string_t* filename);



/*==========================================================
 * Function 	: mail_DeleteMailAttchMent
 * Author		: sunguochen      
 * Description : ɾ���ʼ��ĸ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
void mail_DeleteMailAttchment(char* uidl);



/*==========================================================
 * Function 	: mail_NextRetrAttachId
 * Author		: sunguochen      
 * Description : �����һ��Ҫ���յĸ���id
                      ��Ϊ���������Ѿ����գ�
                      Ϊ��֤���ظ������ѽ��յĸ�����
                      ĳһ����������ʧ�����ظ����գ�
                      ͨ��cur_index ������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
char *mail_NextRetrAttachId(char* uidl, MAIL_ATT_TYPE *type, mail_retr_method_t *retr_method, int *cur_index);



/*==========================================================
 * Function 	: mail_NextRetrAttachSize
 * Author		: sunguochen      
 * Description : �����һ��Ҫ���յĸ�����С
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/13/2010 
 ==========================================================*/
int mail_NextRetrAttachSize(char* uidl, mail_retr_method_t *retr_method, int *cur_index);



/*==========================================================
 * Function 	: mail_CurRetrAttachId
 * Author		: sunguochen      
 * Description : ��ǰ�������صĸ���id
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
char *mail_CurRetrAttachId(char* uidl, mail_retr_method_t *retr_method, int *cur_index);



/*==========================================================
 * Function 	: mail_CurRetrFileName
 * Author		: sunguochen      
 * Description : ����ļ���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
MMI_STRING_T *mail_CurRetrFileName(mail_retr_method_t *retr_method, int *cur_index);



/*==========================================================
 * Function 	: mail_StartParseAttachment
 * Author		: sunguochen      
 * Description : ��������渽��
                      Ϊ�򻯲��������˷��ڽ����߳���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
comm_boolean mail_StartParseAttachment(wchar * file_name, MMI_STRING_T *attach_name, int att_size, char* attachFileFullNamePath);



/*==========================================================
 * Function 	: mail_StartParseAttachList
 * Author		: sunguochen      
 * Description : ���������б�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/24/2010 
 ==========================================================*/
MAIL_CONVERT_ATTRS_T * mail_StartParseAttachList(wchar * file_name, int att_size);



/*==========================================================
 * Function 	: mail_createAttachListTmpFile
 * Author		: sunguochen      
 * Description : �������ظ�������ʱ�ļ�
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_createAttachListTmpFile(char *temp_filePath, char* uidl, char* contentid, MAIL_CONVERT_ATTR_T * attrs);



/*==========================================================
 * Function 	: mail_StartParseAttachListCont
 * Author		: sunguochen      
 * Description : ��������渽��
                 Ϊ�򻯲��������˷��ڽ����߳���
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
comm_boolean mail_StartParseAttachListCont(wchar * file_name, comm_string_t *attach_name);



/*==========================================================
 * Function 	: mail_CleanTmpAttFolder
 * Author		: sunguochen      
 * Description : �����ʱĿ¼
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/24/2010 
 ==========================================================*/
void mail_CleanTmpAttFolder(char* uidl, char* contentid);



/*==========================================================
 * Function 	: mail_IsAttachMultiFile
 * Author		: sunguochen      
 * Description : �жϸ����Ƿ��Ѵ��ڷ��ظ�������
                      0˵��������ʽδת��
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/25/2010 
 ==========================================================*/
int mail_IsAttachMultiFile(char* uidl, char* contentid, comm_string_t* filename);



/*==========================================================
 * Function 	: mail_AttachMultiContent
 * Author		: sunguochen      
 * Description : ����ת��Ϊ����������
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/25/2010 
 ==========================================================*/
mail_att_multi_file_list *mail_AttachMultiContent(char* uidl, char* contentid, comm_string_t* filename);



/*==========================================================
 * Function 	: mail_freeMultiFileList
 * Author		: sunguochen      
 * Description : �ͷ�mail_AttachMultiContent����
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/25/2010 
 ==========================================================*/
void mail_freeMultiFileList(mail_att_multi_file_list *file_list);




#endif 
