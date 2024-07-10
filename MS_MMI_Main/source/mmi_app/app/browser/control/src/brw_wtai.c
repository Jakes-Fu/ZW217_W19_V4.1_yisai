/*****************************************************************
 **  File Name: 	brw_wtai.c	   			     			     *
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
#if 0
#include "mmi_app_browser_trc.h"
#include "brw_wtai.h"
#include "brw_control.h"

#define isHexDigit(c)  (((c)>='0'&&(c)<='9')||((c)>='a'&&(c)<='f')||((c)>='A'&&(c)<='F'))

/*****************************************************************************/
//  Description : char to integer
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
LOCAL uint8 ctoi(uint8 c)
{
    if ('0' <= c && c <= '9')
    {
        return c - '0';
    }
    else if ('A' <= c && c <= 'F')
    {
        return c - 'A' + 0x0A;
    }
    else if ('a' <= c && c <= 'f')
    {
        return c - 'a' + 0x0A;
    }
    else
    {
        return 0xFF;
    }
}

/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        phone_num_ptr phone number string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPMakeCall(BRW_INFO_T* brw_info_ptr,uint8* phone_num_ptr)
{
    BRW_MSG_MAKE_CALL_REQ_T req={0};
    uint16 phone_num_len=0;

    phone_num_len=(uint16)BRW_Strlen(phone_num_ptr);
    
    if(PNULL==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwWPMakeCall:phone_num_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_62_112_2_18_2_7_32_859,(uint8*)"d",phone_num_len);
        return FALSE;
    }
	if(0!=phone_num_ptr && 0!=phone_num_len)
	{
		BRW_Memcpy(req.phone_num_arr,phone_num_ptr,phone_num_len);
	}
    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_MAKECALL_REQ,&req,sizeof(req));
    return TRUE;
}
/*
 *	desc:wap_req_id    页面请求id
 *  param:instance      浏览器句柄
 *        dtmf_ptr      dtmf string
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPSendDTMF(BRW_INFO_T* brw_info_ptr,uint8* dtmf_ptr)
{
    BRW_MSG_SEND_DTMF_REQ_T req={0};
    uint16 dtmf_len=0;

    dtmf_len=(uint16)BRW_Strlen(dtmf_ptr);
    
    if(PNULL==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwWPSendDTMF:dtmf_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_86_112_2_18_2_7_32_860,(uint8*)"d",dtmf_len);
        return FALSE;
    }
	if(0!=dtmf_ptr && 0!=dtmf_len)
	{
		BRW_Memcpy(req.dtmf_arr,dtmf_ptr,dtmf_len);
	}
    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_SENDDTMF_REQ,&req,sizeof(req));
    return TRUE;
}
/*
 *	desc:WTAPublic:addPBEntry
 *  param:wap_req_id    页面请求id
 *        phone_num_ptr 电话号码
 *        name_ptr      名字
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwWPAddPBEntry(BRW_INFO_T* brw_info_ptr,uint8* phone_num_ptr,
                               uint8* name_ptr)
{
    BRW_MSG_ADD_PB_REQ_T req={0};
    uint16 phone_num_len=0;
    uint16 name_len=0;

    phone_num_len=(uint16)BRW_Strlen(phone_num_ptr);
    name_len=(uint16)BRW_Strlen(name_ptr);
    
    if(PNULL==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwWPMakeCall:phone_num_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_114_112_2_18_2_7_32_861,(uint8*)"d",phone_num_len);
        return FALSE;
    }
	if(0!=phone_num_len && 0!=phone_num_ptr)
	{
		BRW_Memcpy(req.phone_num_arr,phone_num_ptr,phone_num_len);
	}
    if(0!=name_ptr && 0!=name_len)
    {
        BRW_Memcpy(req.name,name_ptr,name_len);
    }
    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_ADD_PBITEM_REQ,&req,sizeof(req));
    return TRUE;
}


/*****************************************************************************/
//  Description : 网页中发送短信
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwWPSendSms(BRW_INFO_T* brw_info_ptr, uint8* phone_num_ptr, uint8* sms_content_ptr)
{
    BRW_MSG_SEND_SMS_REQ_T req = {0};
    uint16 phone_num_len = 0;
    uint32 sms_content_len = 0;

    phone_num_len=(uint16)BRW_Strlen(phone_num_ptr);
    sms_content_len=(uint16)BRW_Strlen(sms_content_ptr);
    
    if(PNULL==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwWPSendSms: phone_num_len=%d,sms_content_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_145_112_2_18_2_7_32_862,(uint8*)"dd", phone_num_len, sms_content_len);
        return FALSE;
    }
    
    if (phone_num_len > BRW_WTAI_MAX_PHONE_NUM_LEN)
    {
        //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_151_112_2_18_2_7_33_863,(uint8*)"d",phone_num_len);
        phone_num_len = MIN(phone_num_len, BRW_WTAI_MAX_PHONE_NUM_LEN);
    }
    if (sms_content_len > BRW_WTAI_MAX_SMS_CONTENT_LEN)
    {
        //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb sms content length too long!len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_156_112_2_18_2_7_33_864,(uint8*)"d",sms_content_len);
        sms_content_len = MIN(sms_content_len, BRW_WTAI_MAX_SMS_CONTENT_LEN);
    }
    
	if(PNULL!=phone_num_ptr && 0!=phone_num_len)
	{
		BRW_Memcpy(req.phone_num_arr, phone_num_ptr, phone_num_len);
	}
	if(PNULL!=sms_content_ptr && 0!=sms_content_len)
	{
		BRW_Memcpy(req.sms_content_arr, sms_content_ptr, sms_content_len);
	}

    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_SENDSMS_REQ,&req,sizeof(req));
    return TRUE;
}


/*****************************************************************************/
//  Description : 网页中发送短信
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwWPSendSmsTo(BRW_INFO_T* brw_info_ptr, uint8* phone_num_ptr, uint8* sms_content_ptr)
{
    BRW_MSG_SEND_SMS_REQ_T req = {0};
    uint16 phone_num_len = 0;
    uint32 sms_content_len = 0;

    phone_num_len=(uint16)BRW_Strlen(phone_num_ptr);
    sms_content_len=(uint16)BRW_Strlen(sms_content_ptr);
    
    if(PNULL==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwWPSendSmsTo: phone_num_len=%d,sms_content_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_189_112_2_18_2_7_33_865,(uint8*)"dd", phone_num_len, sms_content_len);
        return FALSE;
    }
    
    if (phone_num_len > BRW_WTAI_MAX_PHONE_NUM_LEN)
    {
        //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_195_112_2_18_2_7_33_866,(uint8*)"d",phone_num_len);
        phone_num_len = MIN(phone_num_len, BRW_WTAI_MAX_PHONE_NUM_LEN);
    }
    if (sms_content_len > BRW_WTAI_MAX_SMS_CONTENT_LEN)
    {
        //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb sms content length too long!len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_200_112_2_18_2_7_33_867,(uint8*)"d",sms_content_len);
        sms_content_len = MIN(sms_content_len, BRW_WTAI_MAX_SMS_CONTENT_LEN);
    }
    
	if(PNULL!=phone_num_ptr && 0!=phone_num_len)
	{
		BRW_Memcpy(req.phone_num_arr, phone_num_ptr, phone_num_len);
	}
	if(PNULL!=sms_content_ptr && 0!=sms_content_len)
	{
		BRW_Memcpy(req.sms_content_arr, sms_content_ptr, sms_content_len);
	}

    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_SENDSMS_REQ,&req,sizeof(req));
    return TRUE;
}

/*****************************************************************************/
//  Description : 网页中发送彩信
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwWPSendMms(BRW_INFO_T* brw_info_ptr,uint8* phone_num_ptr)
{
    BRW_MSG_MAKE_CALL_REQ_T req={0};
    uint16 phone_num_len=0;

    phone_num_len=(uint16)BRW_Strlen(phone_num_ptr);
    
    if(PNULL==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwWPSendMms:phone_num_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_230_112_2_18_2_7_33_868,(uint8*)"d",phone_num_len);
        return FALSE;
    }
    if (phone_num_len > BRW_WTAI_MAX_PHONE_NUM_LEN)
    {
        //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_235_112_2_18_2_7_33_869,(uint8*)"d",phone_num_len);
        phone_num_len = MIN(phone_num_len, BRW_WTAI_MAX_PHONE_NUM_LEN);
    }
	if(PNULL!=phone_num_ptr && 0!=phone_num_len)
	{
		BRW_Memcpy(req.phone_num_arr,phone_num_ptr,phone_num_len);
	}
    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_SENDMMS_REQ,&req,sizeof(req));
    return TRUE;
}

/*****************************************************************************/
//  Description : 网页中发送彩信
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwWPSendMmsTo(BRW_INFO_T* brw_info_ptr,uint8* phone_num_ptr)
{
    BRW_MSG_MAKE_CALL_REQ_T req={0};
    uint16 phone_num_len=0;

    phone_num_len=(uint16)BRW_Strlen(phone_num_ptr);
    
    if(PNULL==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwWPSendMmsTo:phone_num_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_259_112_2_18_2_7_33_870,(uint8*)"d",phone_num_len);
        return FALSE;
    }
    if (phone_num_len > BRW_WTAI_MAX_PHONE_NUM_LEN)
    {
        //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_264_112_2_18_2_7_33_871,(uint8*)"d",phone_num_len);
        phone_num_len = MIN(phone_num_len, BRW_WTAI_MAX_PHONE_NUM_LEN);
    }
	if(PNULL!=phone_num_ptr && 0!=phone_num_len)
	{
		BRW_Memcpy(req.phone_num_arr,phone_num_ptr,phone_num_len);
	}
    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_SENDMMSTO_REQ,&req,sizeof(req));
    return TRUE;
}

/*****************************************************************************/
//  Description : 网页中发送邮件
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwWPSendMailTo(BRW_INFO_T* brw_info_ptr,uint8* email_name_ptr)
{
    BRW_MSG_SEND_MAILTO_REQ_T req={0};
    uint16 email_name_len=0;

    email_name_len=(uint16)BRW_Strlen(email_name_ptr);
    
    if(PNULL==brw_info_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwWPSendMail:phone_num_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_288_112_2_18_2_7_33_872,(uint8*)"d", email_name_len);
        return FALSE;
    }
    if (email_name_len > BRW_WTAI_MAX_EMAIL_NAME_LEN)
    {
        //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_293_112_2_18_2_7_33_873,(uint8*)"d",email_name_len);
        email_name_len = MIN(email_name_len, BRW_WTAI_MAX_EMAIL_NAME_LEN);
    }
	if(PNULL!=email_name_ptr && 0!=email_name_len)
	{
		BRW_Memcpy(req.email_name_arr, email_name_ptr, email_name_len);
	}
    brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_SENDMAILTO_REQ, &req, sizeof(req));
    return TRUE;
}

/*****************************************************************************/
//  Description : Get num from URL
//  Global resource dependence : none
//  Author: rong.gu
//  Note:
/*****************************************************************************/
PUBLIC void BrwGetNumFromURL(char *str_dest, const char* str_src, char *str, BOOLEAN is_dtmf)
{
    uint8 i = 0;
    uint8 str_len = 0;
    char *str_cpy = str_dest;

    if( (PNULL == str_src) || (PNULL == str) )
    {
        return;
    }
    
    str_len = strlen(str);

    for(i = 0; i < str_len; i++)
    {
        str_src++;
    }
    while((*str_src >= '0' && *str_src <= '9')||(*str_src == '*')||(*str_src == '#')
        ||(*str_src == 'W')||(*str_src == 'P')||(*str_src == 'w')||(*str_src == 'p'))
    {
        if (is_dtmf && (*str_src == '*'))//dtmf string
        {
            *str_cpy++ = 'P';
            *str_src++;
        }
        else
        {
            *str_cpy++ = *str_src++;
        }
    }

    return;
}

/*****************************************************************************/
//  Description : Get sms content from URL
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC void BrwGetSmsContentFromURL(uint8 *str_dest, const uint8* str_src, const char *str)
{
    uint32  i = 0;
    uint32 j = 0;
    uint32 str_len = 0;
    uint8 *content_str_ptr = PNULL;

    if( (PNULL == str_src) || (PNULL == str) || (PNULL == str_dest))
    {
        return;
    }
    
	if (PNULL == BRW_Strstr(str_src, (const uint8*)str))
	{
		return;
	}
    else 
    {
        content_str_ptr = BRW_Strstr(str_src, (const uint8*)str) + BRW_Strlen((const uint8*)str);
		str_len = BRW_Strlen((const uint8 *)content_str_ptr);

        for (i = 0; i < str_len; i++, j++)//计算转译字符
        {
            if (0x25 == *(content_str_ptr+ i) && (i + 2 < str_len)
                && isHexDigit(*(content_str_ptr+ i + 1)) && isHexDigit(*(content_str_ptr+ i + 2))) // %
            {
                uint8 temp_conv_utf8 = ctoi(*(content_str_ptr+ i + 1)) * 16 + ctoi(*(content_str_ptr+ i + 2));
                *(str_dest + j) = temp_conv_utf8;
                i += 2;
            }
            else
            {
                *(str_dest + j) = *(content_str_ptr+ i);
            }
        }
    }
}


/*****************************************************************************/
//  Description : Get Email Address from URL
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC void BrwGetEmailAddrFromURL(uint8 *str_dest, const uint8* str_src, const char *str)
{
    uint32  i = 0;
    uint32 j = 0;
    uint32 str_len = 0;
    uint8 *content_str_ptr = PNULL;

    if( (PNULL == str_src) || (PNULL == str) || (PNULL == str_dest))
    {
        return;
    }
    
	if (PNULL == BRW_Strstr(str_src, (const uint8*)str))
	{
		return;
	}
    else 
    {
        content_str_ptr = BRW_Strstr(str_src, (const uint8*)str) + BRW_Strlen((const uint8*)str);
		str_len = BRW_Strlen((const uint8 *)content_str_ptr);

        for (i = 0; i < str_len; i++, j++)//计算转译字符
        {
            if (0x25 == *(content_str_ptr+ i) && (i + 2 < str_len)
                && isHexDigit(*(content_str_ptr+ i + 1)) && isHexDigit(*(content_str_ptr+ i + 2))) // %
            {
                uint8 temp_conv_utf8 = ctoi(*(content_str_ptr+ i + 1)) * 16 + ctoi(*(content_str_ptr+ i + 2));
                *(str_dest + j) = temp_conv_utf8;
                i += 2;
            }
            else
            {
                *(str_dest + j) = *(content_str_ptr+ i);
            }
        }
    }
}

/*
 *	desc:解析url,获取wtai相关信息
 *  param:url_ptr  url字串
 *        wtai_info_ptr[out]  wtai的信息结构体指针
 *  return:不是WTAI返回FALSE,否则返回TRUE
 */
PUBLIC BOOLEAN BrwGetWTAIInfo(const uint8* url_ptr,BRW_WTAI_INFO_T* wtai_info_ptr)
{
    BOOLEAN ret =TRUE;
    const uint8 *temp_str_ptr = PNULL;
    uint32 flag_len = 0;
    uint32 url_len = 0;
    uint32 copy_len = 0;

    if ((PNULL==url_ptr) || (PNULL==wtai_info_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwGetWTAIInfo:url_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_442_112_2_18_2_7_33_874,(uint8*)"d",url_ptr);
        return FALSE;
    }
    url_len = BRW_Strlen(url_ptr);   

    flag_len = BRW_Strlen((const uint8*)"tel:");
    if(0==BRW_Strnicmp(url_ptr,(const uint8*)"tel:",flag_len))
    {
        copy_len = MIN((url_len - flag_len), BRW_WTAI_MAX_PHONE_NUM_LEN);
        BRW_Memcpy(wtai_info_ptr->param.phone_number,(url_ptr+flag_len), copy_len);
        wtai_info_ptr->func=WTAI_FUNC_MC;
        return TRUE;
    }
    
    flag_len = BRW_Strlen((const uint8*)"sms");
    if(0 == BRW_Strnicmp(url_ptr,(const uint8*)"sms",flag_len))
    {
    	temp_str_ptr = url_ptr+flag_len;
        if(0 == BRW_Strnicmp(temp_str_ptr, (const uint8*)":", 1))
        {     
            wtai_info_ptr->func=WTAI_FUNC_SMS;
            BrwGetNumFromURL((char*)wtai_info_ptr->param.phone_number, (const char*)url_ptr, "sms:", FALSE);
            BrwGetSmsContentFromURL(wtai_info_ptr->sms_content, url_ptr, "?body=");
        }
        else if(0 == BRW_Strnicmp(temp_str_ptr, (const uint8*)"to:",3))
        {
			wtai_info_ptr->func=WTAI_FUNC_SMSTO;
            BrwGetNumFromURL((char*)wtai_info_ptr->param.phone_number, (const char*)url_ptr, "smsto:", FALSE);
            BrwGetSmsContentFromURL(wtai_info_ptr->sms_content, url_ptr, "?body=");
        }
		return TRUE;
    }
    
    flag_len = BRW_Strlen((const uint8*)"mms");
    if(0 == BRW_Strnicmp(url_ptr,(const uint8*)"mms",flag_len))
    {
    	temp_str_ptr = url_ptr+flag_len;
        if(0 == BRW_Strnicmp(temp_str_ptr,(const uint8*)":", 1))
        {     
            wtai_info_ptr->func=WTAI_FUNC_MMS;
            BrwGetNumFromURL((char*)wtai_info_ptr->param.phone_number, (const char*)url_ptr, "mms:", FALSE);
        }
        else if(0 == BRW_Strnicmp(temp_str_ptr,(const uint8*)"to:", 3))
        {
            wtai_info_ptr->func=WTAI_FUNC_MMSTO;
            BrwGetNumFromURL((char*)wtai_info_ptr->param.phone_number,  (const char*)url_ptr, "mmsto:", FALSE);
        }
		return TRUE;
    }
    
    flag_len = BRW_Strlen((const uint8*)"mailto:");
    if(0 == BRW_Strnicmp(url_ptr,(const uint8*)"mailto:",flag_len))
    {
    	temp_str_ptr = url_ptr+flag_len;
        copy_len = BRW_Strlen(temp_str_ptr);
        
        if(copy_len > BRW_WTAI_MAX_EMAIL_NAME_LEN)
        {
            //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err email addr length too long!len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_500_112_2_18_2_7_33_875,(uint8*)"d",copy_len);
        }
        wtai_info_ptr->func=WTAI_FUNC_MAILTO;
        BrwGetEmailAddrFromURL(wtai_info_ptr->param.email_name, url_ptr, "mailto:");

		return TRUE;
    }
    
    flag_len=BRW_Strlen((const uint8*)"wtai://wp/");
    if(BRW_Strnicmp(url_ptr,(const uint8*)"wtai://wp/",flag_len)==0)
    {
        temp_str_ptr = url_ptr+flag_len;
        if(BRW_Strnicmp(temp_str_ptr,(const uint8*)"mc",2)==0)
        {     
            wtai_info_ptr->func=WTAI_FUNC_MC;
            BrwGetNumFromURL((char*)wtai_info_ptr->param.phone_number, (const char*)url_ptr, "wtai://wp/mc;", FALSE);
        }
        else if(BRW_Strnicmp(temp_str_ptr,(const uint8*)"sd",2)==0)
        {
            wtai_info_ptr->func=WTAI_FUNC_SD;
            BrwGetNumFromURL((char*)wtai_info_ptr->param.dtmf,  (const char*)url_ptr, "wtai://wp/sd;", TRUE);
        }
        else if(BRW_Strnicmp(temp_str_ptr,(const uint8*)"ap",2)==0)//"wtai://wp/ap;10086;mobile"
        {
            const uint8* name_ptr=PNULL;

            wtai_info_ptr->func=WTAI_FUNC_AP;
            temp_str_ptr += 2;
            //has phone number
            if (';' == *temp_str_ptr)
            {
                temp_str_ptr++;
                //calculate the len of phone number
                name_ptr = BRW_Strchr(temp_str_ptr,';');
                if (PNULL == name_ptr)
                {
                    name_ptr = temp_str_ptr + BRW_Strlen(temp_str_ptr)*sizeof(uint8);
                }
                copy_len = name_ptr-temp_str_ptr;
                if (copy_len > BRW_WTAI_MAX_PHONE_NUM_LEN)
                {
                    //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb phonenumber length too long!len=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_541_112_2_18_2_7_33_876,(uint8*)"d",copy_len);
                }
                copy_len = MIN(copy_len, BRW_WTAI_MAX_PHONE_NUM_LEN);
                BRW_Memcpy(wtai_info_ptr->param.phone_number,temp_str_ptr,copy_len);
            }
            
            temp_str_ptr += copy_len;
            //has name
            if (';' == *temp_str_ptr)
            {
                temp_str_ptr++;
                copy_len = BRW_Strlen(temp_str_ptr);
                if(copy_len > BRW_WTAI_MAX_PB_NAME_LEN)
                {
                    //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:err pb name length too long!len=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_555_112_2_18_2_7_33_877,(uint8*)"d",copy_len);
                }
                copy_len = MIN(copy_len, BRW_WTAI_MAX_PB_NAME_LEN);
                BRW_Memcpy(wtai_info_ptr->name,temp_str_ptr,copy_len);
            }
        }
        else
        {
            //BRWDebug_Printf:"[BRW]BrwGetWTAIInfo:not support"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_WTAI_563_112_2_18_2_7_33_878,(uint8*)"");
            return FALSE;
        }
    }
    else
    {
        ret = FALSE;
    }
    return ret;
}
#endif
