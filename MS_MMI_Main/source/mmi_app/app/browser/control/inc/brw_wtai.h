/*****************************************************************
 **  File Name: 	brw_wtai.h	   			     			     *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:implemention of wtai                            *
 **  feature list:                                               *
 **  1)PUBLIC WTAI:WTAPublic.makeCall WTAPublic.sendDTMF         *
 **                WTAPublic.addPBEntry                          *
 **  2)PUBLIC WTAI URI functions:wtai://wp/mc  wtai://wp/sd      *
 **                              wtai://wp/ap                    *
 **  3)参考文档:WAP-170-WTAI-20000707-a.pdf                      *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  22/04/2009		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef BRW_WTAI_H
#define BRW_WTAI_H

#if 0
#include "brw_utility.h"
#include "brw_control_api.h"
#include "brw_control.h"

typedef enum
{
    WTAI_FUNC_NULL=0,
    WTAI_FUNC_MC,
    WTAI_FUNC_SD,
    WTAI_FUNC_AP,
    WTAI_FUNC_SMS,
    WTAI_FUNC_SMSTO,
    WTAI_FUNC_MMS,
    WTAI_FUNC_MMSTO,
    WTAI_FUNC_MAILTO
    
}BRW_WTAI_FUNC_E;
//wtai信息
typedef struct tagBRW_WTAI_INFO_T
{
    BRW_WTAI_FUNC_E func;
    union
    {
        uint8 phone_number[BRW_WTAI_MAX_PHONE_NUM_LEN+1];
        uint8 dtmf[BRW_WTAI_MAX_DTMF_LEN+1];
        uint8 email_name[BRW_WTAI_MAX_EMAIL_NAME_LEN+1];
    }param;
    uint8 sms_content[BRW_WTAI_MAX_SMS_CONTENT_LEN+1];
    uint8 name[BRW_WTAI_MAX_PB_NAME_LEN];
}BRW_WTAI_INFO_T;
/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        phone_num_ptr phone number string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPMakeCall(BRW_INFO_T* brw_info_ptr,uint8* phone_num_ptr);
/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        dtmf_ptr      dtmf string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPSendDTMF(BRW_INFO_T* brw_info_ptr,uint8* dtmf_ptr);
/*
 *	desc:WTAPublic:addPBEntry
 *  param:wap_req_id    页面请求id
 *        phone_num_ptr 电话号码
 *        name_ptr      名字(unicode)
 *        name_len      名字长度
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPAddPBEntry(BRW_INFO_T* brw_info_ptr,uint8* phone_num_ptr,
                               uint8* name_ptr);
/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        phone_num_ptr phone number string
 *        sms_content_ptr sms content string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPSendSms(BRW_INFO_T* brw_info_ptr, uint8* phone_num_ptr, uint8* sms_content_ptr);

/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        phone_num_ptr phone number string
 *        sms_content_ptr sms content string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPSendSmsTo(BRW_INFO_T* brw_info_ptr, uint8* phone_num_ptr, uint8* sms_content_ptr);

/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        phone_num_ptr phone number string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPSendMms(BRW_INFO_T* brw_info_ptr,uint8* phone_num_ptr);

/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        phone_num_ptr phone number string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPSendMmsTo(BRW_INFO_T* brw_info_ptr,uint8* phone_num_ptr);

/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        email_name_ptr email name string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPSendMailTo(BRW_INFO_T* brw_info_ptr,uint8* email_name_ptr);

/*
 *	desc:解析url,获取wtai相关信息
 *  param:url_ptr  url字串
 *        wtai_info_ptr[out]  wtai的信息结构体指针
 *  return:不是WTAI返回FALSE,否则返回TRUE
 */
PUBLIC BOOLEAN BrwGetWTAIInfo(const uint8* url_ptr,BRW_WTAI_INFO_T* wtai_info_ptr);


PUBLIC void BrwGetNumFromURL(char *str_dest, const char* str_src, char *str, BOOLEAN is_dtmf);

/*
 *	desc:解析url,获取短信内容
 *  param:str_dest[in]  
 *        str_src[in]
 *        str[in]
 *  return: 
 */
PUBLIC void BrwGetSmsContentFromURL(uint8 *str_dest, const uint8* str_src, const char *str);

/*
 *	desc:解析url,获取email地址
 *  param:str_dest[in]  
 *        str_src[in]
 *        str[in]  
 *  return: 
 */
PUBLIC void BrwGetEmailAddrFromURL(uint8 *str_dest, const uint8* str_src, const char *str);
#endif
#endif
