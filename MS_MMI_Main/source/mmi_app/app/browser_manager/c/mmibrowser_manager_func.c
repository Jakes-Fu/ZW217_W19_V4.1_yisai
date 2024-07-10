/*****************************************************************************
** File Name:      mmibrowser_manager_func.c                                 *
** Author:          fen.xie                                                  *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_browser_manager_trc.h"
#include "window_parse.h"
#include "sci_types.h" 
#include "mmi_module.h"
#include "mmiidle_export.h"
#include "mmipb_export.h"
#include "cfl_char.h"
#include "gui_ucs2b_converter.h"
#include "cafcontrol.h"
#include "mmi_resource.h"
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#include "mime_type.h"
#include "mmibrowser_manager_func.h"
#include "mmibrowser_manager_nv.h"
#include "mmibrowser_manager_menutable.h"
#include "mmibrowser_manager_wintable.h"
#include "browser_manager_text.h"
#include "mmimms_export.h"
#include "mmi_common.h"
#if defined(VT_SUPPORT)||defined(MMI_IP_CALL_SUPPORT)
#else
#include "mmicc_export.h"
#endif
#include "mmi_appmsg.h"
#ifdef MMIEMAIL_SUPPORT
#include "mmiemail_export.h"
#endif
#ifdef PUSH_EMAIL_SUPPORT
#include "mmimail_export.h"
#endif /* PUSH_EMAIL_SUPPORT  */
#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
#ifdef DL_SUPPORT
#include "mmidl_export.h"
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                *
 **--------------------------------------------------------------------------*/
#define BROWSER_WTAI_MAX_EMAIL_NAME_LEN   255
#define BROWSER_WTAI_MAX_SMS_CONTENT_LEN   160
#define BROWSER_WTAI_MAX_EMAIL_SUBJECT_LEN   255
#define BROWSER_WTAI_MAX_EMAIL_BODY_LEN   1024
#define isHexDigit(c)  (((c)>='0'&&(c)<='9')||((c)>='a'&&(c)<='f')||((c)>='A'&&(c)<='F'))
#define MMIBM_BOOKMARK_FLAG_LEN (24)

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
typedef struct
{
    uint8    *subject_ptr;
    uint32  subject_size;
    uint8    *body_ptr;
    uint32  body_size;
}MMIBM_EMAIL_FIELD_INFO_T;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIBROWSER_ENTRY_PARAM s_browser_manager_entry = {0};//the entry param for browser manager
LOCAL MMIBROWSER_MIME_DOWNLOAD_PARAM_T s_mime_download_param[MMIBROWSER_MIME_TYPE_MAX] = {0};
LOCAL const char s_bookmark_flag_arr[MMIBM_BOOKMARK_FLAG_LEN][2] = 
{
    {'[', '['},
    {'I', 'i'},
    {'N', 'n'},
    {'T', 't'},
    {'E', 'e'},
    {'R', 'r'},
    {'N', 'n'},
    {'E', 'e'},
    {'T', 't'},
    {'S', 's'},
    {'H', 'h'},
    {'O', 'o'},
    {'R', 'r'},
    {'T', 't'},
    {'C', 'c'},
    {'U', 'u'},
    {'T', 't'},
    {']', ']'},
    {0x0D, 0x0D},
    {0x0A, 0x0A},
    {'U', 'u'},
    {'R', 'r'},
    {'L', 'l'},
    {'=', '='},
};

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init browser manager module
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_InitModule(void)
{
    MMIBM_RegMenuGroup();
    MMIBM_RegNv();
}

/*****************************************************************************/
//  Description : create the entry param for browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_CreateEntryParam(const MMIBROWSER_ENTRY_PARAM entry_param)
{
    uint32  temp_len = 0;
    
    MMIBM_DestoryEntryParam();
    
    if ((entry_param.url_ptr != PNULL) 
        && (temp_len = SCI_STRLEN(entry_param.url_ptr)) > 0)
    {
        s_browser_manager_entry.url_ptr = SCI_ALLOCA(temp_len + 1);
        if (s_browser_manager_entry.url_ptr != PNULL)
        {
            SCI_MEMCPY(s_browser_manager_entry.url_ptr, entry_param.url_ptr, (temp_len + 1));
        }
    }
    
    if ((entry_param.url_title_ptr != PNULL) 
        && (temp_len = SCI_STRLEN(entry_param.url_title_ptr)) > 0)
    {
        s_browser_manager_entry.url_title_ptr = SCI_ALLOCA(temp_len + 1);
        if (s_browser_manager_entry.url_title_ptr != PNULL)
        {
            SCI_MEMCPY(s_browser_manager_entry.url_title_ptr, entry_param.url_title_ptr, (temp_len + 1));
        }
    }
    
    if ((entry_param.user_agent_ptr != PNULL) 
        && (temp_len = SCI_STRLEN(entry_param.user_agent_ptr)) > 0)
    {
        s_browser_manager_entry.user_agent_ptr = SCI_ALLOCA(temp_len + 1);
        if (s_browser_manager_entry.user_agent_ptr != PNULL)
        {
            SCI_MEMCPY(s_browser_manager_entry.user_agent_ptr, entry_param.user_agent_ptr, (temp_len + 1));
        }
    }

    s_browser_manager_entry.browser_type = entry_param.browser_type;
    s_browser_manager_entry.type = entry_param.type;
    s_browser_manager_entry.dual_sys = entry_param.dual_sys;
    s_browser_manager_entry.is_connection_customized = entry_param.is_connection_customized;
    s_browser_manager_entry.is_use_wifi = entry_param.is_use_wifi;
    s_browser_manager_entry.connection_index = entry_param.connection_index;
}

/*****************************************************************************/
//  Description : destory the entry param for browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_DestoryEntryParam(void)
{
    if (s_browser_manager_entry.url_ptr != PNULL)
    {
        SCI_FREE(s_browser_manager_entry.url_ptr);
    }
    if (s_browser_manager_entry.url_title_ptr != PNULL)
    {
        SCI_FREE(s_browser_manager_entry.url_title_ptr);
    }    
    if (s_browser_manager_entry.user_agent_ptr != PNULL)
    {
        SCI_FREE(s_browser_manager_entry.user_agent_ptr);
    }    
    SCI_MEMSET(&s_browser_manager_entry, 0x00, sizeof(MMIBROWSER_ENTRY_PARAM));
}

/*****************************************************************************/
//  Description : get the entry param for browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ENTRY_PARAM* MMIBM_GetEntryParam(void)
{
    return &s_browser_manager_entry;
}

/*****************************************************************************/
//  Description : get the default browser type setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_BROWSER_TYPE_E MMIBM_GetDefaultBrowser(void)
{
    MMIBROWSER_BROWSER_TYPE_E   default_browser_type = MMIBROWSER_TYPE_MAX;
    MN_RETURN_RESULT_E  nv_result = MN_RETURN_SUCCESS;
    
    nv_result = MMI_ReadNVItem(MMINV_BM_DEFAULT_BROWSER, &default_browser_type);
    if (nv_result != MN_RETURN_SUCCESS)
    {
        default_browser_type = MMIBROWSER_TYPE_MAX;//default value
        MMI_WriteNVItem(MMINV_BM_DEFAULT_BROWSER, &default_browser_type);
    }

    return default_browser_type;
}

/*****************************************************************************/
//  Description : set the default browser type setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_SetDefaultBrowser(MMIBROWSER_BROWSER_TYPE_E default_browser_type)
{
    MMI_WriteNVItem(MMINV_BM_DEFAULT_BROWSER, &default_browser_type);
}

/*****************************************************************************/
//  Description : reset factory setting of browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_ResetFactorySetting(void)
{
    MMIBM_SetDefaultBrowser(MMIBROWSER_TYPE_MAX);
}

/*****************************************************************************/
//  Description : char to integer
//  Global resource dependence : 
//  Author:fen.xie
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

/*****************************************************************************/
//  Description : Get num from URL
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void GetPhoneNumFromURL(char *phone_num_ptr, 
                uint16  phone_num_len,
                const char* str_src_ptr, 
                char *head_ptr, 
                BOOLEAN is_dtmf)
{
    uint8 i = 0;
    uint8 head_str_len = 0;
    char *str_cpy = phone_num_ptr;

    if((PNULL == str_cpy) || (0 == phone_num_len) || (PNULL == str_src_ptr) || (PNULL == head_ptr))
    {
        return;
    }
    
    head_str_len = SCI_STRLEN(head_ptr);

    for(i = 0; i < head_str_len; i++)
    {
        str_src_ptr++;
    }
    while((*str_src_ptr >= '0' && *str_src_ptr <= '9')||(*str_src_ptr == '*')||(*str_src_ptr == '#')
        ||(*str_src_ptr == 'W')||(*str_src_ptr == 'P')||(*str_src_ptr == 'w')||(*str_src_ptr == 'p'))
    {
        if (0 == phone_num_len )
        {
            break;
        }
        
        if (is_dtmf && (*str_src_ptr == '*'))//dtmf string
        {
            *str_cpy++ = 'P';
            str_src_ptr++;
        }
        else
        {
            *str_cpy++ = *str_src_ptr++;
        }
        phone_num_len--;
    }

    return;
}

/*****************************************************************************/
//  Description : ¼ÆËã×ªÒë×Ö·û
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BmDecode(const uint8* src_str_ptr, 
                uint16  src_str_len,
                uint8* dest_str_ptr,                 
                uint16  dest_str_len,
                const char  field_end)
{
    uint32  i = 0;
    uint32  j = 0;
    uint8 temp_conv_utf8 = 0;
    
    if( (PNULL == src_str_ptr) || (PNULL == dest_str_ptr) || (0 == src_str_len) || (0 == dest_str_len))
    {
        return;
    }
    
    for (i = 0; i < src_str_len; i++, j++)//¼ÆËã×ªÒë×Ö·û
    {
        if ((j >= dest_str_len) || (field_end == *(src_str_ptr+i)))
        {
            break;
        }
        
        if ((0x25 == *(src_str_ptr+ i)) && (i + 2 < src_str_len)
            && isHexDigit(*(src_str_ptr + i + 1)) && isHexDigit(*(src_str_ptr+ i + 2))) // %
        {
            temp_conv_utf8 = ctoi(*(src_str_ptr+ i + 1)) * 16 + ctoi(*(src_str_ptr + i + 2));
            *(dest_str_ptr + j) = temp_conv_utf8;
            i += 2;
        }
        else
        {
            *(dest_str_ptr + j) = *(src_str_ptr+ i);
        }
    }
}

/*****************************************************************************/
//  Description : Get field content from URL by field flag
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
PUBLIC void GetFieldContentFromURL(uint8 *field_content_ptr, 
                uint16  field_content_len,
                const uint8* src_str_ptr, 
                const char *field_ptr,
                const char field_end)
{
    uint32 str_len = 0;
    uint8 *content_str_ptr = PNULL;

    if( (PNULL == src_str_ptr) || (PNULL == field_ptr) || (0 == field_content_len) || (PNULL == field_content_ptr))
    {
        return;
    }

    content_str_ptr = (uint8*)strstr((char*)src_str_ptr, field_ptr);
    if (PNULL == content_str_ptr)
    {
        return;
    }
    else
    {
        content_str_ptr += SCI_STRLEN(field_ptr);//set to the field content begin pos
        str_len = SCI_STRLEN((char*)content_str_ptr);

        BmDecode(content_str_ptr,str_len,field_content_ptr,field_content_len,field_end);
    }
}

#if defined(MMIEMAIL_SUPPORT)
/*****************************************************************************/
//  Description : Get field content for email
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void GetFieldContentForEmail(MMIBM_EMAIL_FIELD_INFO_T *field_info_ptr, //[IN/OUT]
                const uint8* src_str_ptr
                )
{
	char * query = PNULL;
	char * end = PNULL;
	char * start= (char*)src_str_ptr;

    if( (PNULL == src_str_ptr) || (PNULL == field_info_ptr))
    {
        return;
    }
    
    end = (char*)start + SCI_STRLEN(start);
    query = strchr(start, '?');

    if(query == PNULL )
    {
        query = end;
    }

    if(query != end)
    {
        char *substart = query + 1;

        for(;;)
        {
            char *subend = strchr(substart, '&');
            char *eq = strchr(substart, '=');

            if (PNULL == subend)
            {
                subend = end;
            }

            if(eq != PNULL)
            {
                char * value_start= eq + 1;

                if(MMIAPICOM_Stristr(substart, "subject") == substart)
                {
                    BmDecode((uint8*)value_start,subend - value_start,field_info_ptr->subject_ptr,field_info_ptr->subject_size,'&');
                }
                else if(MMIAPICOM_Stristr(substart, "body") == substart)
                {
                    BmDecode((uint8*)value_start,subend - value_start,field_info_ptr->body_ptr,field_info_ptr->body_size,'&');
                }
            }
            
            if(subend == end)
            {
                break;
            }
            substart = subend;
            ++ substart;
        }
    }
}

/*****************************************************************************/
//  Description : Get field content from URL by field flag
//  Global resource dependence : none
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL void BMHandleEmail(const uint8* src_str_ptr, const char *field_ptr)
{
    uint8 email_name[BROWSER_WTAI_MAX_EMAIL_NAME_LEN+1] = {0};
    uint32 name_len = 0;
    uint32 subject_len = 0;
    uint32 body_len = 0;
    EMAOPT_T pstOpts[4] = {0};
    wchar *email_wname_ptr = PNULL;
    wchar *email_wsubject_ptr = PNULL;
    wchar *email_wbody_ptr = PNULL;
    BOOLEAN result = FALSE;
    uint8   opt_index = 0;
    MMIBM_EMAIL_FIELD_INFO_T    email_field = {0};
        
    GetFieldContentFromURL(email_name,BROWSER_WTAI_MAX_EMAIL_NAME_LEN, src_str_ptr, field_ptr, '?');

//     if (0 == (name_len = SCI_STRLEN((char *)email_name)))
//     {
//         if(!MMIAPIEMAIL_IsRunning())
//     	{
// 		    MMIAPIEMAIL_Entry();//CR175410
//     	}
//       //  MMIPUB_OpenAlertWarningWin(TXT_COMMON_MAIL_ADDR_INVALID);
//         return;
//     }
    
    if ((email_field.subject_ptr = SCI_ALLOCA(BROWSER_WTAI_MAX_SMS_CONTENT_LEN+1)) != PNULL)
    {
        SCI_MEMSET(email_field.subject_ptr, 0x00, (BROWSER_WTAI_MAX_SMS_CONTENT_LEN+1));
        email_field.subject_size = BROWSER_WTAI_MAX_SMS_CONTENT_LEN;
    }
    else
    {
        //SCI_TRACE_LOW:"[BM]W BMHandleEmail:email_subject_ptr alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_444_112_2_18_2_7_38_4,(uint8*)"");
    }
    if ((email_field.body_ptr = SCI_ALLOCA(BROWSER_WTAI_MAX_EMAIL_BODY_LEN+1)) != PNULL)
    {
        SCI_MEMSET(email_field.body_ptr, 0x00, (BROWSER_WTAI_MAX_EMAIL_BODY_LEN+1));
        email_field.body_size = BROWSER_WTAI_MAX_EMAIL_BODY_LEN;
    }
    else
    {
        //SCI_TRACE_LOW:"[BM]W BMHandleEmail:email_body_ptr alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_453_112_2_18_2_7_38_5,(uint8*)"");
    }   

    GetFieldContentForEmail(&email_field, src_str_ptr);
    if (email_field.subject_ptr != PNULL)
    {
        subject_len = SCI_STRLEN((char *)email_field.subject_ptr);
    }
    if (email_field.body_ptr != PNULL)
    {
        body_len = SCI_STRLEN((char *)email_field.body_ptr);
    }

    email_wname_ptr = SCI_ALLOCA((name_len+1) * sizeof(wchar));
    if (PNULL != email_wname_ptr)
    {            
        //name
        opt_index = 0;
        SCI_MEMSET(email_wname_ptr, 0, (name_len+1) * sizeof(wchar));
        GUI_UTF8ToWstr(email_wname_ptr, name_len, email_name, name_len);
        pstOpts[opt_index].opt_id = EMA_OPT_TO;
        pstOpts[opt_index].val_ptr = (void*)email_wname_ptr;
        opt_index++;

        //subject
        if ((subject_len != 0) 
            && (email_wsubject_ptr = SCI_ALLOCA((subject_len+1) * sizeof(wchar))) != PNULL)
        {
            SCI_MEMSET(email_wsubject_ptr, 0, (subject_len+1) * sizeof(wchar));
            GUI_UTF8ToWstr(email_wsubject_ptr, subject_len, email_field.subject_ptr, subject_len);
            pstOpts[opt_index].opt_id = EMA_OPT_SUBJECT;
            pstOpts[opt_index].val_ptr = (void*)email_wsubject_ptr;
            opt_index++;
        }
        else
        {
            //SCI_TRACE_LOW:"[BM]BMHandleEmail:subject_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_489_112_2_18_2_7_38_6,(uint8*)"d" ,subject_len);
        }
        if (email_field.subject_ptr != PNULL)
        {
            SCI_FREE(email_field.subject_ptr);
        }

        //body
        if ((body_len != 0) 
            && (email_wbody_ptr = SCI_ALLOCA((body_len+1) * sizeof(wchar))) != PNULL)
        {
            SCI_MEMSET(email_wbody_ptr, 0, (body_len+1) * sizeof(wchar));
            GUI_UTF8ToWstr(email_wbody_ptr, body_len, email_field.body_ptr, body_len);
            pstOpts[opt_index].opt_id = EMA_OPT_BOBY;
            pstOpts[opt_index].val_ptr = (void*)email_wbody_ptr;
            opt_index++;
        }      
        else
        {
            //SCI_TRACE_LOW:"[BM]BMHandleEmail:body_len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_508_112_2_18_2_7_38_7,(uint8*)"d" ,body_len);
        }
        if (email_field.body_ptr != PNULL)
        {
            SCI_FREE(email_field.body_ptr);
        }

        //end
        pstOpts[opt_index].opt_id = EMA_OPT_END;
        pstOpts[opt_index].val_ptr = PNULL;

        MMIAPIEMAIL_EditEmailEx(pstOpts);  

        result = TRUE;
        SCI_FREE(email_wname_ptr);
        if (email_wsubject_ptr != PNULL)
        {
            SCI_FREE(email_wsubject_ptr);
        }
        if (email_wbody_ptr != PNULL)
        {
            SCI_FREE(email_wbody_ptr);
        }            
    }
    else
    {
        //SCI_TRACE_LOW:"[BM]BMHandleEmail:temp_ptr ALLOC Fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_534_112_2_18_2_7_38_8,(uint8*)"");
    }

    if (!result)
    {
        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
    }
}
#endif

/*****************************************************************************/
//  Description : Handle special scheme(wati,rtsp......)
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIBROWSER_HandleExScheme(uint8 *in_url)
{
    uint16 url_len = 0;
    uint8 *temp_str_ptr = PNULL;
    BOOLEAN result = TRUE;

    if (PNULL == in_url)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIAPIBROWSER_HandleExScheme: in_url =%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_559_112_2_18_2_7_38_9,(uint8*)"s", in_url);

    while(' ' == *in_url)
    {
        in_url++;
    }

    url_len = SCI_STRLEN((char *)in_url);
    if (0 == url_len)
    {
        //SCI_TRACE_LOW:"MMIAPIBROWSER_HandleExScheme: in_url empty"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_569_112_2_18_2_7_38_10,(uint8*)"");
        return FALSE;
    }

    temp_str_ptr= SCI_ALLOCAZ(url_len + 1);
    if (PNULL == temp_str_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIBROWSER_HandleExScheme: Err No memory "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_576_112_2_18_2_7_38_11,(uint8*)"");
        return FALSE;
    }
    SCI_MEMCPY(temp_str_ptr, in_url, url_len);
    temp_str_ptr = (uint8 *)MMIAPICOM_Strlower((char *)temp_str_ptr);

    if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_TEL_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_TEL_HEADER)) == 0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_TEL_REQ, in_url, (url_len + 1));
    }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_WP_MC_EMPTY_HEADER, SCI_STRLEN((char *)in_url)) == 0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_MAKECALL_REQ, PNULL, 0);
    }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_WP_MC_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_WP_MC_HEADER)) == 0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_MAKECALL_REQ, in_url, (url_len + 1));
   }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_WP_SD_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_WP_SD_HEADER)) == 0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_SENDDTMF_REQ, in_url, (url_len + 1));
    }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_WP_AP_EMPTY_HEADER, SCI_STRLEN((char *)in_url)) ==0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_ADD_PBITEM_REQ, PNULL, 0);
    }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_WP_AP_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_WP_AP_HEADER)) ==0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_ADD_PBITEM_REQ, in_url, (url_len + 1));
    }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_SMS_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_SMS_HEADER)) ==0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_SENDSMS_REQ, in_url, (url_len + 1));
    }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_SMS_TO_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_SMS_TO_HEADER)) ==0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_SENDSMSTO_REQ, in_url, (url_len + 1));
    }
    else if(0 != strncmp((char *)temp_str_ptr, MMICOM_URL_MMS_HEADER, SCI_STRLEN(MMICOM_URL_MMS_HEADER)) && strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_MMS_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_MMS_HEADER)) ==0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_SENDMMS_REQ, in_url, (url_len + 1));
    }
    else if( strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_MMS_TO_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_MMS_TO_HEADER)) ==0 )
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_SENDMMSTO_REQ, in_url, (url_len + 1));
    }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_MAIL_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_MAIL_HEADER)) ==0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_SENDMAIL_REQ, in_url, (url_len + 1));
    }
    else if(strncmp((char *)temp_str_ptr, MMICOM_URL_WTAI_MAIL_TO_HEADER, SCI_STRLEN(MMICOM_URL_WTAI_MAIL_TO_HEADER)) ==0)
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_SENDMAILTO_REQ, in_url, (url_len + 1));
    }
    else if( strncmp((char *)temp_str_ptr, MMICOM_URL_RTSP_HEADER, SCI_STRLEN(MMICOM_URL_RTSP_HEADER)) ==0 )
    {
        MMK_PostMsg(VIRTUAL_WIN_ID, MSG_BROWSER_MANAGER_OPEN_RTSP_IND, in_url, (url_len + 1));
    }
    else
    {
        result = FALSE;
    }

    SCI_FREE(temp_str_ptr);
    return result;
}

/*****************************************************************************/
//  Description : handle app message in browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBM_HandleAppMsg(MMI_MESSAGE_ID_E msg_id,void* param)
{
    uint16 url_len = 0;
    uint8 *phone_number_ptr = PNULL;
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
    MMIBM_CC_INFO_T cc_info = {0};
#endif
    BOOLEAN result = TRUE;
    uint8    *url_ptr = (uint8*)param;
    
    //SCI_TRACE_LOW:"[BM]MMIBM_HandleAppMsg,msg_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_655_112_2_18_2_7_38_12,(uint8*)"d", msg_id);
    
    if (url_ptr != PNULL)
    {
        url_len = SCI_STRLEN((char *)url_ptr);
    }
    
    switch (msg_id)
    {
    case MSG_BROWSER_MANAGER_OPEN_RTSP_IND:
    {
#ifdef MMI_STREAMING_PLAYER_SUPPORT       
        MMIAPIVP_PlayVideoFromVideoPlayer(VP_ENTRY_FROM_NET,PNULL,0,url_ptr);        
#else
        MMIPUB_OpenAlertWarningWin(TXT_COMM_TYPE_NOT_SUPPORT);
#endif
    } 
        break;
        
    case MSG_BROWSER_MANAGER_TEL_REQ:
    {
        if (url_ptr != PNULL)
        {
            phone_number_ptr = SCI_ALLOCAZ(url_len + 1);
            if (PNULL != phone_number_ptr)
            {
                GetPhoneNumFromURL((char *)phone_number_ptr, url_len, (char *)url_ptr, MMICOM_URL_WTAI_TEL_HEADER, FALSE);
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
                cc_info.cc_option = MMIBM_CC_OPTION_VOICE | MMIBM_CC_OPTION_IP;/*lint !e655*/
                cc_info.phone_num_ptr = phone_number_ptr;
                MMIBM_OpenCCSelWin(&cc_info);    
#else
                MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, phone_number_ptr, strlen((char*)phone_number_ptr), PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
#endif
                break;//successfull
            }
        }
        
        //SCI_TRACE_LOW:"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_689_112_2_18_2_7_38_13,(uint8*)"d", url_ptr);
        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
    }
        break;
        
    case MSG_BROWSER_MANAGER_MAKECALL_REQ:
    {
        if (url_ptr != PNULL)
        {
            phone_number_ptr = SCI_ALLOCAZ(url_len + 1);
            if (PNULL != phone_number_ptr)
            {
                GetPhoneNumFromURL((char *)phone_number_ptr, url_len, (char *)url_ptr, MMICOM_URL_WTAI_WP_MC_HEADER, FALSE);
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
                cc_info.cc_option = MMIBM_CC_OPTION_VOICE | MMIBM_CC_OPTION_IP | MMIBM_CC_OPTION_VIDEO;/*lint !e655*/
                cc_info.phone_num_ptr = phone_number_ptr;
                MMIBM_OpenCCSelWin(&cc_info);
#else
                MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, phone_number_ptr, strlen((char*)phone_number_ptr), PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
#endif
            }
            else
            {
                //SCI_TRACE_LOW:"[BM]W MMIBM_HandleAppMsg: Err No memory"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_708_112_2_18_2_7_38_14,(uint8*)"");
                MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
            }
        }
        else
        {
            MMIAPIIDLE_OpenIdleWin();
        }
    }
        break;
        
    case MSG_BROWSER_MANAGER_SENDDTMF_REQ:
    {
        if (url_ptr != PNULL)
        {
            phone_number_ptr = SCI_ALLOCAZ(url_len + 1);
            if (PNULL != phone_number_ptr)
            {
                GetPhoneNumFromURL((char *)phone_number_ptr, url_len, (char *)url_ptr, MMICOM_URL_WTAI_WP_SD_HEADER, TRUE);
#if defined(MMI_IP_CALL_SUPPORT)||defined(VT_SUPPORT)
                cc_info.cc_option = MMIBM_CC_OPTION_VOICE | MMIBM_CC_OPTION_IP;/*lint !e655*/
                cc_info.phone_num_ptr = phone_number_ptr;
                MMIBM_OpenCCSelWin(&cc_info);
#else
                MMIAPICC_MakeCall(MN_DUAL_SYS_MAX, phone_number_ptr, strlen((char*)phone_number_ptr), PNULL, PNULL, CC_CALL_SIM_MAX, CC_CALL_NORMAL_CALL, PNULL);
#endif
                break;//successfull
            }
        }
        
        //SCI_TRACE_LOW:"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_734_112_2_18_2_7_38_15,(uint8*)"d", url_ptr);
        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
    }
        break;
        
    case MSG_BROWSER_MANAGER_ADD_PBITEM_REQ:
    {
        MMIPB_BCD_NUMBER_T pb_bcd = {0};

        if (url_ptr != PNULL)
        {
            MMI_PARTY_NUMBER_T party_num = {MN_NUM_TYPE_UNKNOW, 0, 0};
            
            phone_number_ptr = SCI_ALLOCAZ(url_len + 1);
            if (PNULL != phone_number_ptr)
            {
                GetPhoneNumFromURL((char *)phone_number_ptr, url_len, (char *)url_ptr, MMICOM_URL_WTAI_WP_AP_HEADER, FALSE);
                MMIAPICOM_GenPartyNumber(phone_number_ptr, SCI_STRLEN((char *)phone_number_ptr), &party_num);
                pb_bcd.npi_ton = MMIAPICOM_ChangeTypePlanToUint8(party_num.number_type, MN_NUM_PLAN_UNKNOW);
                pb_bcd.number_len = MIN(party_num.num_len, MMIPB_BCD_NUMBER_MAX_LEN);
                SCI_MEMCPY( pb_bcd.number,  party_num.bcd_num, pb_bcd.number_len );
                MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
            }
            else
            {
                //SCI_TRACE_LOW:"[BM]W MMIBM_HandleAppMsg: Err No memory"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_759_112_2_18_2_7_38_16,(uint8*)"");
                MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
            }
        }
        else
        {
            MMIAPIPB_AddContactWin(&pb_bcd,PB_GROUP_ALL,MMIPB_ADD_TYPE_NUMBER);
        }
    }
        break;

    case MSG_BROWSER_MANAGER_SENDSMS_REQ:
    {
        if (url_ptr != PNULL)
        {
            phone_number_ptr = SCI_ALLOCAZ(url_len + 1);
            if (PNULL != phone_number_ptr)
            {
                MMI_STRING_T    sms_content_str = {0};
                uint8   sms_content[BROWSER_WTAI_MAX_SMS_CONTENT_LEN + 1] = {0};
                wchar   sms_w_content[BROWSER_WTAI_MAX_SMS_CONTENT_LEN + 1] = {0};

                GetPhoneNumFromURL((char *)phone_number_ptr, url_len, (char *)url_ptr, MMICOM_URL_WTAI_SMS_HEADER, FALSE);
                GetFieldContentFromURL(sms_content,BROWSER_WTAI_MAX_SMS_CONTENT_LEN,  url_ptr, "?body=", 0);
                GUI_UTF8ToWstr(sms_w_content,BROWSER_WTAI_MAX_SMS_CONTENT_LEN,sms_content,BROWSER_WTAI_MAX_SMS_CONTENT_LEN);
                sms_content_str.wstr_ptr = sms_w_content;
                sms_content_str.wstr_len = MMIAPICOM_Wstrlen(sms_w_content);
                MMIAPISMS_WriteNewMessage( MN_DUAL_SYS_MAX, &sms_content_str, phone_number_ptr, SCI_STRLEN((char *)phone_number_ptr) );
                break;//successfull
            }
        }
        
        //SCI_TRACE_LOW:"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_791_112_2_18_2_7_38_17,(uint8*)"d", url_ptr);
        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
    }        
        break;

    case MSG_BROWSER_MANAGER_SENDSMSTO_REQ:
    {
        if (url_ptr != PNULL)
        {
            phone_number_ptr = SCI_ALLOCAZ(url_len + 1);
            if (PNULL != phone_number_ptr)
            {
                MMI_STRING_T    sms_content_str = {0};
                uint8   sms_content[BROWSER_WTAI_MAX_SMS_CONTENT_LEN + 1] = {0};
                wchar   sms_w_content[BROWSER_WTAI_MAX_SMS_CONTENT_LEN + 1] = {0};
                
                GetPhoneNumFromURL((char *)phone_number_ptr, url_len, (char *)url_ptr, MMICOM_URL_WTAI_SMS_TO_HEADER, FALSE);
                GetFieldContentFromURL(sms_content, BROWSER_WTAI_MAX_SMS_CONTENT_LEN, url_ptr, "?body=", 0);
                GUI_UTF8ToWstr(sms_w_content,BROWSER_WTAI_MAX_SMS_CONTENT_LEN,sms_content,BROWSER_WTAI_MAX_SMS_CONTENT_LEN);
                sms_content_str.wstr_ptr = sms_w_content;
                sms_content_str.wstr_len = MMIAPICOM_Wstrlen(sms_w_content);
                MMIAPISMS_WriteNewMessage( MN_DUAL_SYS_MAX, &sms_content_str, phone_number_ptr, SCI_STRLEN((char *)phone_number_ptr) );
                break;//successfull
            }
        }
        
        //SCI_TRACE_LOW:"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_817_112_2_18_2_7_39_18,(uint8*)"d", url_ptr);
        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
    }
        break;

    case MSG_BROWSER_MANAGER_SENDMMS_REQ:
    {
#ifdef MMS_SUPPORT
        if (url_ptr != PNULL)
        {
            phone_number_ptr = SCI_ALLOCAZ(url_len + 1);
            if (PNULL != phone_number_ptr)
            {
                MMI_STRING_T string_to = {0};
                uint32  temp_len = 0;
                
                GetPhoneNumFromURL((char *)phone_number_ptr, url_len, (char *)url_ptr, MMICOM_URL_WTAI_MMS_HEADER, FALSE);
                if ((temp_len = SCI_STRLEN((char *)phone_number_ptr)) > 0)
                {
                    string_to.wstr_ptr = (wchar*)SCI_ALLOCA((temp_len + 1) * sizeof(wchar));
                    if (PNULL != string_to.wstr_ptr)
                    {
                        SCI_MEMSET(string_to.wstr_ptr, 0x00, ((temp_len + 1) * sizeof(wchar)));
                        
                        MMI_STRNTOWSTR(string_to.wstr_ptr, temp_len, 
                            phone_number_ptr, temp_len, 
                            temp_len);
                        string_to.wstr_len = MMIAPICOM_Wstrlen(string_to.wstr_ptr);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIAPIBROWSER_HandleExScheme: mms alloc fail!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_848_112_2_18_2_7_39_19,(uint8*)"");
                    }
                }
                
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
                MMIAPIMMS_AnswerMMS(MN_DUAL_SYS_MAX,PNULL, &string_to);
                
                if (PNULL != string_to.wstr_ptr)
                {
                    SCI_FREE(string_to.wstr_ptr); 
                }
                break;//successfull
            }
        }
        
        //SCI_TRACE_LOW:"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_863_112_2_18_2_7_39_20,(uint8*)"d", url_ptr);
        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
#else
        MMIPUB_OpenAlertWarningWin(TXT_COMM_TYPE_NOT_SUPPORT);
#endif
    }
        break;

    case MSG_BROWSER_MANAGER_SENDMMSTO_REQ:
    {
#ifdef MMS_SUPPORT
        if (url_ptr != PNULL)
        {
            phone_number_ptr = SCI_ALLOCAZ(url_len + 1);
            if (PNULL != phone_number_ptr)
            {
                MMI_STRING_T string_to = {0};
                uint32  temp_len = 0;
                
                GetPhoneNumFromURL((char *)phone_number_ptr, url_len, (char *)url_ptr, MMICOM_URL_WTAI_MMS_TO_HEADER, FALSE);
                if ((temp_len = SCI_STRLEN((char *)phone_number_ptr)) > 0)
                {
                    string_to.wstr_ptr = (wchar*)SCI_ALLOCA((temp_len + 1) * sizeof(wchar));
                    if (PNULL != string_to.wstr_ptr)
                    {
                        SCI_MEMSET(string_to.wstr_ptr, 0x00, ((temp_len + 1) * sizeof(wchar)));
                        
                        MMI_STRNTOWSTR(string_to.wstr_ptr, temp_len, 
                            phone_number_ptr, temp_len, 
                            temp_len);
                        string_to.wstr_len = MMIAPICOM_Wstrlen(string_to.wstr_ptr);
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIAPIBROWSER_HandleExScheme: mms alloc fail!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_897_112_2_18_2_7_39_21,(uint8*)"");
                    }
                }
                
                MMIAPIMMS_SetMMSEditEnterReason(MMIMMS_EDIT_FROM_PB_SEND);
                MMIAPIMMS_AnswerMMS(MN_DUAL_SYS_MAX,PNULL, &string_to);
                
                if (PNULL != string_to.wstr_ptr)
                {
                    SCI_FREE(string_to.wstr_ptr); 
                }
                break;//successfull
            }
        }
        
        //SCI_TRACE_LOW:"[BM]W MMIBM_HandleAppMsg:url_ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_MANAGER_FUNC_912_112_2_18_2_7_39_22,(uint8*)"d", url_ptr);
        MMIPUB_OpenAlertWarningWin(TXT_UNKNOWN_ERROR);
#else
        MMIPUB_OpenAlertWarningWin(TXT_COMM_TYPE_NOT_SUPPORT);
#endif
    }
        break;

    case MSG_BROWSER_MANAGER_SENDMAIL_REQ:
#ifdef MMIEMAIL_SUPPORT
        BMHandleEmail(url_ptr, MMICOM_URL_WTAI_MAIL_HEADER);
#elif defined(PUSH_EMAIL_SUPPORT)
        MMIAPIMAIL_Enter(MAIL_ENTER_NEWMAIL, PNULL);
#else
        MMIPUB_OpenAlertWarningWin(TXT_COMM_TYPE_NOT_SUPPORT);
#endif /* PUSH_EMAIL_SUPPORT  */
        break;
    
    case MSG_BROWSER_MANAGER_SENDMAILTO_REQ:
#ifdef MMIEMAIL_SUPPORT
        BMHandleEmail(url_ptr, MMICOM_URL_WTAI_MAIL_TO_HEADER);
#elif defined(PUSH_EMAIL_SUPPORT)
        {
            uint8 email_name[BROWSER_WTAI_MAX_EMAIL_NAME_LEN+1] = {0};

            GetFieldContentFromURL(email_name,BROWSER_WTAI_MAX_EMAIL_NAME_LEN, url_ptr, MMICOM_URL_WTAI_MAIL_TO_HEADER, 0);
            MMIAPIMAIL_Enter(MAIL_ENTER_NEWMAIL, SCI_STRLEN((char*)email_name) > 0 ? email_name: PNULL);
        }
#else
        MMIPUB_OpenAlertWarningWin(TXT_COMM_TYPE_NOT_SUPPORT);
#endif /* PUSH_EMAIL_SUPPORT  */
         break;
        
    default:
        result = FALSE;//don't handle
        break;
    }

    if (phone_number_ptr != PNULL)
    {
        SCI_FREE(phone_number_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : reg mime func
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBM_RegMimeFunc(MMIBROWSER_MIME_DOWNLOAD_PARAM_T* param_ptr)
{
    if ((PNULL != param_ptr) && (param_ptr->mime_type < MMIBROWSER_MIME_TYPE_MAX))
    {
#ifdef DL_SUPPORT
        MMIDL_MIME_DOWNLOAD_PARAM_T param = {0};

        param.mime_type = param_ptr->mime_type;
        param.call_back = (MMIDL_MIME_CALLBACK)param_ptr->call_back;

        MMIAPIDL_RegMimeCallback(&param);
#endif
        s_mime_download_param[param_ptr->mime_type] = *param_ptr;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : handle download callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_MimeDownlaodCallBack(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr)
{
    if (PNULL != param_ptr && param_ptr->mime_type < MMIBROWSER_MIME_TYPE_MAX)
    {
        SCI_TRACE_LOW("MMIBM_MIMEDownlaodCallBack mime=%d, url=%s", param_ptr->mime_type, param_ptr->url_ptr);

        if (s_mime_download_param[param_ptr->mime_type].call_back != 0)
        {
        	
            s_mime_download_param[param_ptr->mime_type].call_back(param_ptr);
        }
 	else
 	{
            SCI_TRACE_LOW("MMIBROWSER MIME CALLBACK PARAM ERROR callback");
 	}
    }
    else
    {
        SCI_TRACE_LOW("MMIBM_MIMEDownlaodCallBack PARAM ERROR param_ptr =%d", param_ptr);
    }
}

/*****************************************************************************/
// Description : BmStrCmp
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL int32 BmStrCmp(const uint8* const str1_ptr, //in
                    const uint8 symbol,
                    uint32 str1_len
                    )
{
    const uint8 * temp_ptr = str1_ptr;
    uint32 temp_str1_len = str1_len;

    while(temp_str1_len != 0)
    {
        if (symbol == *temp_ptr++)
        {
            break;
        }
        temp_str1_len--;
    }
    
    if ( 0 == temp_str1_len)//no find
    {
        return -1;     
    }
    else
    {
        return (str1_len - temp_str1_len);
    }

}

/*****************************************************************************/
// Description : get the url info by the url file buffer
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
PUBLIC uint8* MMIAPIBROWSER_GetUrlByFile(uint8* buf_ptr, //in
                    uint32 size,//in
                    uint32 *url_len_ptr
                    )
{
    uint32 url_len = 0;
    uint8 *temp_buf_ptr = PNULL;
    int32 pos = 0;
    uint32 remain_size = 0;
    const uint8* url_buf_ptr = PNULL;
    BOOLEAN result = FALSE;
    uint i=0;
    uint8 temp_char = 0;
    
    if ((PNULL == buf_ptr) || (0 == size) || (size <= MMIBM_BOOKMARK_FLAG_LEN) || (PNULL == url_len_ptr))
    {
        return PNULL;
    }
    
    *url_len_ptr = 0;
    
    temp_buf_ptr = buf_ptr;
    remain_size = size;

    pos = BmStrCmp(temp_buf_ptr, '[', remain_size);
    if (pos < 0)
    {
        return PNULL;
    }
    remain_size -= pos ;/*lint !e737*/
    
    while(remain_size > MMIBM_BOOKMARK_FLAG_LEN)
    {
        temp_buf_ptr += pos;
        for(i = 0; i < MMIBM_BOOKMARK_FLAG_LEN; i++)
        {
            temp_char = *(temp_buf_ptr + i);
            if ((temp_char != s_bookmark_flag_arr[i][0]) && (temp_char != s_bookmark_flag_arr[i][1]))
            {
                break;
            }
        }
                    
        if (MMIBM_BOOKMARK_FLAG_LEN == i)
        {
            result = TRUE;
            break;
        }

        remain_size--;
        temp_buf_ptr++;
        pos = BmStrCmp(temp_buf_ptr, '[', remain_size);
        if (pos < 0)
        {
            break;
        }

        remain_size -= pos ;/*lint !e737*/
    }

    if (result)
    {
        url_len = remain_size - MMIBM_BOOKMARK_FLAG_LEN;
        url_buf_ptr = temp_buf_ptr + MMIBM_BOOKMARK_FLAG_LEN;
        while(url_len-- > 0)
        {
            if ((0x0D == *url_buf_ptr) || (0x0A == *url_buf_ptr))
            {
                break;
            }
            url_buf_ptr++;
            (*url_len_ptr)++;    
        }

        return (temp_buf_ptr + MMIBM_BOOKMARK_FLAG_LEN);
    }
    else
    {
        return PNULL;
    }
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

