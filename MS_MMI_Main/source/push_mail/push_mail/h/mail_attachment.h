/*====*====*====*====*====*====*====*====*====*====*====*====*
 *	Filename		: mail_attachment.h
 *	Author		: sunguochen    
 *	Description	: 附件管理
 *	Vesion		: 1.0        
 *	Date			: 2010-5-20  
 *	History		: 
 * --------    ---------    ----------  ------------------------------------
 *  sunguochen 2010-5-20      1.0           初步编写完成
 *====*====*====*====*====*====*====*====*====*====*====*====*/



#ifndef _MAIL_ATTACHMENT_H_
#define _MAIL_ATTACHMENT_H_
#include "mail_pop3.h"


//单个文件属性
typedef struct _mail_att_file_info_
{
    int                   file_size;
    comm_string_t  file_name;

}mail_att_file_info;


//转换后全部文件属性
typedef struct _mail_att_multi_file_list_
{
    int     file_num;
    mail_att_file_info *file_info;

}mail_att_multi_file_list;


/*==========================================================
 * Function 	: mail_createAttachTmpFile
 * Author		: sunguochen      
 * Description : 创建下载附件的临时文件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_createAttachTmpFile(char *temp_filePath, char* uidl, char* contentid);



/*==========================================================
 * Function 	: mail_IsAttachFileExist
 * Author		: sunguochen      
 * Description : 判断附件是否已存在
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_IsAttachFileExist(char* uidl, char* contentid, MMI_STRING_T* filename);



/*==========================================================
 * Function 	: mail_AttachFileName
 * Author		: sunguochen      
 * Description  : 获得附件文件名
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	    : created  5/20/2010 
 ==========================================================*/
wchar* mail_AttachFileName(char* uidl, char* contentid, comm_string_t* filename);



/*==========================================================
 * Function 	: mail_DeleteMailAttchMent
 * Author		: sunguochen      
 * Description : 删除邮件的附件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/21/2010 
 ==========================================================*/
void mail_DeleteMailAttchment(char* uidl);



/*==========================================================
 * Function 	: mail_NextRetrAttachId
 * Author		: sunguochen      
 * Description : 获得下一个要接收的附件id
                      因为附件可能已经接收，
                      为保证不重复接收已接收的附件和
                      某一个附件接收失败又重复接收，
                      通过cur_index 来控制
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/22/2010 
 ==========================================================*/
char *mail_NextRetrAttachId(char* uidl, MAIL_ATT_TYPE *type, mail_retr_method_t *retr_method, int *cur_index);



/*==========================================================
 * Function 	: mail_NextRetrAttachSize
 * Author		: sunguochen      
 * Description : 获得下一个要接收的附件大小
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  8/13/2010 
 ==========================================================*/
int mail_NextRetrAttachSize(char* uidl, mail_retr_method_t *retr_method, int *cur_index);



/*==========================================================
 * Function 	: mail_CurRetrAttachId
 * Author		: sunguochen      
 * Description : 当前正在下载的附件id
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
char *mail_CurRetrAttachId(char* uidl, mail_retr_method_t *retr_method, int *cur_index);



/*==========================================================
 * Function 	: mail_CurRetrFileName
 * Author		: sunguochen      
 * Description : 获得文件名
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
MMI_STRING_T *mail_CurRetrFileName(mail_retr_method_t *retr_method, int *cur_index);



/*==========================================================
 * Function 	: mail_StartParseAttachment
 * Author		: sunguochen      
 * Description : 解析并另存附件
                      为简化操作，将此放在接收线程中
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
comm_boolean mail_StartParseAttachment(wchar * file_name, MMI_STRING_T *attach_name, int att_size, char* attachFileFullNamePath);



/*==========================================================
 * Function 	: mail_StartParseAttachList
 * Author		: sunguochen      
 * Description : 解析附件列表
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/24/2010 
 ==========================================================*/
MAIL_CONVERT_ATTRS_T * mail_StartParseAttachList(wchar * file_name, int att_size);



/*==========================================================
 * Function 	: mail_createAttachListTmpFile
 * Author		: sunguochen      
 * Description : 创建下载附件的临时文件
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/20/2010 
 ==========================================================*/
comm_boolean mail_createAttachListTmpFile(char *temp_filePath, char* uidl, char* contentid, MAIL_CONVERT_ATTR_T * attrs);



/*==========================================================
 * Function 	: mail_StartParseAttachListCont
 * Author		: sunguochen      
 * Description : 解析并另存附件
                 为简化操作，将此放在接收线程中
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  5/24/2010 
 ==========================================================*/
comm_boolean mail_StartParseAttachListCont(wchar * file_name, comm_string_t *attach_name);



/*==========================================================
 * Function 	: mail_CleanTmpAttFolder
 * Author		: sunguochen      
 * Description : 清空临时目录
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/24/2010 
 ==========================================================*/
void mail_CleanTmpAttFolder(char* uidl, char* contentid);



/*==========================================================
 * Function 	: mail_IsAttachMultiFile
 * Author		: sunguochen      
 * Description : 判断附件是否已存在返回附件个数
                      0说明附件格式未转换
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/25/2010 
 ==========================================================*/
int mail_IsAttachMultiFile(char* uidl, char* contentid, comm_string_t* filename);



/*==========================================================
 * Function 	: mail_AttachMultiContent
 * Author		: sunguochen      
 * Description : 附件转换为多个后的名称
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/25/2010 
 ==========================================================*/
mail_att_multi_file_list *mail_AttachMultiContent(char* uidl, char* contentid, comm_string_t* filename);



/*==========================================================
 * Function 	: mail_freeMultiFileList
 * Author		: sunguochen      
 * Description : 释放mail_AttachMultiContent数据
 * Parameter	: type          parameter       Description
 * Return		: Void
 * Effect		: NO
 * History	: created  6/25/2010 
 ==========================================================*/
void mail_freeMultiFileList(mail_att_multi_file_list *file_list);




#endif 
