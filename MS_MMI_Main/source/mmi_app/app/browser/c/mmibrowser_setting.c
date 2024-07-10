/*****************************************************************************
** File Name:      mmibrowser_setting.c                                      *
** Author:          li.li                                                    *
** Date:           23/06/2009                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser setting         *
*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_browser_trc.h"
#include "mmibrowser_export.h"
#include "mmi_nv.h"
#include "mmibrowser_setting.h"
#include "mmibrowser_id.h"
#include "browser_anim.h"
#include "mmibrowser_nv.h"
#include "cfl_wchar.h"
#include "cfl_char.h"
#include "mmiconnection_export.h"
#include "mmibrowser_bookmark.h"
#include "mmibrowser_func.h"

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
#ifdef CSTAR_CHINESE_LIMITATION_SUPPORT
	#define MMIBROWSER_DEFAULT_FONT  MMIBROWSER_FONT_BIG
#else
	#define MMIBROWSER_DEFAULT_FONT  MMIBROWSER_FONT_MIDDLE
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIBROWSER_START_CONTEXT_T s_browser_start_info = {0};//the info for start browser
LOCAL MMIBROWSER_CONTEXT_T s_browser_context = {0};
LOCAL MMIBROWSER_NV_SETTING_T s_nv_setting = {0};
LOCAL wchar *s_browser_dl_fullname_ptr = PNULL;
LOCAL const uint8 s_browser_custom_default_homepage[] = "http://116.228.149.59/WAP/Download/index_download.html";

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description : return browser context
/*****************************************************************************/
//MMIBROWSER_CONTEXT_T *MMIBROWSER_GetContextPtr(void)
//{
//    return &s_browser_context;
//}
/*****************************************************************************/
//  Description : set s_nv_setting from NV(MMIBROWSER_SETTING)
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_InitSetting(void)
{
    MN_RETURN_RESULT_E          nv_result = MN_RETURN_FAILURE;
    
    MMINV_READ(MMIBROWSER_SETTING, &s_nv_setting, nv_result);
    if(MN_RETURN_SUCCESS != nv_result)
    {
        MMIBROWSER_ResetNvSetting();
    }
}

/*****************************************************************************/
//  Description : get nv setting of browser
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: after MMIBROWSER_InitSetting
/*****************************************************************************/
PUBLIC MMIBROWSER_NV_SETTING_T* MMIBROWSER_GetNVSetting(void)
{
    return &s_nv_setting;
}

/*****************************************************************************/
//  Description : update nv setting(MMIBROWSER_SETTING) from s_nv_setting
//  Global resource dependence : s_nv_setting
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateNVSetting(void)
{
    MMINV_WRITE(MMIBROWSER_SETTING, &s_nv_setting);
}

#ifndef MMI_PDA_SUPPORT
/*****************************************************************************/
//  Description : set image setting
//  Global resource dependence : s_nv_setting
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetImageSetting(BOOLEAN is_image_loaded)
{
    s_nv_setting.is_image_loaded = is_image_loaded;
}

/*****************************************************************************/
//  Description : set bgsound setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetBgsoundSetting(BOOLEAN is_sound_played)
{
    s_nv_setting.is_sound_played = is_sound_played;
}

#ifdef MMIDORADO_GOOGLE_TRANSCODE_SUPPORT
/*****************************************************************************/
//  Description : set google change setting
//  Global resource dependence : s_nv_setting
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetGoogleSetting(BOOLEAN is_google_change_enable)
{
    s_nv_setting.is_google_change_enable = is_google_change_enable;
}
#endif
#endif

/*****************************************************************************/
//  Description : set font setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetFontSetting(MMIBROWSER_FONT_E font_type)
{
    s_nv_setting.font = font_type;
}

/*****************************************************************************/
//  Description : get font setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_FONT_E MMIBROWSER_GetFontSetting(void)
{
    return s_nv_setting.font;
}

/*****************************************************************************/
//  Description : get net setting index
//  Global resource dependence : s_nv_setting
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint8 MMIDORADO_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys)
{
    uint8               index = 0;
    
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"[BRW]W MMIDORADO_GetNetSettingIndex dual_sys =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_140_112_2_18_2_5_50_90,(uint8*)"d", dual_sys);
        return 0;
    }
    
    index = s_nv_setting.sim_net_index[dual_sys];
    
    return index;
}

/*****************************************************************************/
//  Description : set sim's network accout index setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetNetIndexSetting(MN_DUAL_SYS_E dual_sys, uint8 index)
{
    if ((dual_sys >= MMI_DUAL_SYS_MAX) || (index >= MMIAPICONNECTION_GetLinkSettingNum(dual_sys)))
    {
        return;
    }
    
    s_nv_setting.sim_net_index[dual_sys] = index;
}


/*****************************************************************************/
//  Description :set homepage to s_nv_setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SetHomepageSetting(const MMI_STRING_T *string_ptr)
{
    BOOLEAN return_val = FALSE;

    if (PNULL != string_ptr)
    {
        uint16 url_len = MIN(string_ptr->wstr_len, MMIBROWSER_HOMEPAGE_MAX_URL_LEN);
    
        SCI_MEMSET(s_nv_setting.homepage, 0, sizeof(s_nv_setting.homepage));

        if (url_len > 0)
        {
            MMI_WSTRNTOSTR(s_nv_setting.homepage, MMIBROWSER_HOMEPAGE_MAX_URL_LEN, 
                string_ptr->wstr_ptr, url_len, url_len);
        }
        else
        {
            //clean null
        }
        return_val = TRUE;
    }

    return return_val;
}

/*****************************************************************************/
//  Description :Get homepage
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC uint8* MMIBROWSER_GetHomepageSetting(void)
{
    return s_nv_setting.homepage;
}

/*****************************************************************************/
//  Description :set homepage mode to s_nv_setting
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_SetHomepageModeSetting(MMIBROWSER_HOMEPAGE_MODE_E homepage_mode)
{
    BOOLEAN return_val = FALSE;

    if ((MMIBROWSER_HOMEPAGE_MODE_DEFAULT <= homepage_mode) && (MMIBROWSER_HOMEPAGE_MODE_MAX > homepage_mode)) 
    {
        s_nv_setting.homepage_mode = homepage_mode;
        return_val = TRUE;
    }
    else
    {
        //do nothing
    }

    return return_val;
}

/*****************************************************************************/
//  Description :Get homepage mode
//  Global resource dependence : s_nv_setting
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_HOMEPAGE_MODE_E MMIBROWSER_GetHomepageModeSetting(void)
{
    return s_nv_setting.homepage_mode;
}

/*****************************************************************************/
//  Description : current sim's index is modfied with default setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: MS00243346
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsSimIndexModifyByDefault(MN_DUAL_SYS_E cur_sim_type)
{
    uint8   sim_default_account_index = 0;
    
    if (cur_sim_type >= MMI_DUAL_SYS_MAX)
    {
        return FALSE;
    }

    sim_default_account_index = MMIAPICONNECTION_GetAccountDefaultIndex(cur_sim_type, MMICONNECTION_ACCOUNT_BROWSER);

#ifndef MMI_PDA_SUPPORT
    if (sim_default_account_index != MMIDORADO_GetNetSettingIndex(cur_sim_type))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    {
        MMIBROWSER_NV_SETTING_T     nv_setting = {0};
        MN_RETURN_RESULT_E          nv_result = MN_RETURN_FAILURE;

        MMINV_READ(MMIBROWSER_SETTING, &nv_setting, nv_result);
  
        if ((MN_RETURN_SUCCESS != nv_result) || (sim_default_account_index != nv_setting.sim_net_index[cur_sim_type]))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

#endif
}

/*****************************************************************************/
//  Description : current font is modfied with default setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_IsFontModifyByDefault(void)
{
#ifndef MMI_PDA_SUPPORT
    if (MMIBROWSER_DEFAULT_FONT != s_nv_setting.font)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    MMIBROWSER_NV_SETTING_T     nv_setting = {0};
    MN_RETURN_RESULT_E          nv_result = MN_RETURN_FAILURE;

    MMINV_READ(MMIBROWSER_SETTING, &nv_setting, nv_result);

    if ((MN_RETURN_SUCCESS != nv_result) || (MMIBROWSER_DEFAULT_FONT != nv_setting.font))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

#endif
}

/*****************************************************************************/
//  Description : update browser net setting when the index list item is deleted  
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_Setting_ConnectionCallback(MMICONNECTION_NOTIFY_EVENT_E notify_event, uint32 param, MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN                 need_write = FALSE;
    uint8                   index = (uint8)param;

    //SCI_TRACE_LOW:"MMIBROWSER_Setting_ConnectionCallback,index=%d,dual_sys=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_317_112_2_18_2_5_51_91,(uint8*)"dd",index, dual_sys);
    
    switch(notify_event)
    {
    case MMICONNECTION_NOTIFY_EVENT_DEL_ITEM:
        if (dual_sys < MMI_DUAL_SYS_MAX)
        {
            //小于的情况不用处理
            if(s_nv_setting.sim_net_index[dual_sys] == index)
            {
                need_write = TRUE;
                s_nv_setting.sim_net_index[dual_sys] = MMIBROWSER_SETTING_DEFAULT_INDEX;
            }
            else if(s_nv_setting.sim_net_index[dual_sys] > index)
            {
                need_write = TRUE;
                s_nv_setting.sim_net_index[dual_sys]--;
            }
        }
        
        if(need_write)
        {
            MMIBROWSER_UpdateNVSetting();
            MMIAPICONNECTION_AdaptingAccount(dual_sys, MMICONNECTION_ACCOUNT_BROWSER);
        }
        break;

    case MMICONNECTION_NOTIFY_EVENT_EDIT_ITEM:
        if (dual_sys < MMI_DUAL_SYS_MAX)
        {
            MN_DUAL_SYS_E sim_type = MMIBROWSER_GetSim();

            //选中的已被修改
            if(sim_type < MMI_DUAL_SYS_MAX
                && sim_type == dual_sys
                && s_nv_setting.sim_net_index[dual_sys] == index)
            {
                MMIBROWSER_SetSettingChange(TRUE);
            }
        }
        break;

    default:
        break;
    }

}

/*****************************************************************************/
//  Description : set is download media
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetDownloadMedia(BOOLEAN is_download_media)
{
    s_browser_context.is_download_media = is_download_media;
}

/*****************************************************************************/
//  Description : update recv data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateRecvDataLen(uint32 curr_recv_data_len)
{
    s_browser_context.recv_data_len += curr_recv_data_len;
}

/*****************************************************************************/
//  Description : get recv data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetRecvDataLen(void)
{
    return s_browser_context.recv_data_len;
}

/*****************************************************************************/
//  Description : update curr media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateCurrMediaNum(uint32 curr_media_num)
{
    s_browser_context.curr_media_num += curr_media_num;
}

/*****************************************************************************/
//  Description : set curr media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetCurrMediaNum(uint32 curr_media_num)
{
    s_browser_context.curr_media_num = curr_media_num;
}

/*****************************************************************************/
//  Description : get current media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetCurrMediaNum(void)
{
    return s_browser_context.curr_media_num;
}

/*****************************************************************************/
//  Description : update req handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateReqHandle(uint32 req_handle)
{
    s_browser_context.req_handle = req_handle;
}

/*****************************************************************************/
//  Description : get req handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetReqHandle(void)
{
    return s_browser_context.req_handle;
}

/*****************************************************************************/
//  Description : clear recv data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ClearRecvDataLen(void)
{
    s_browser_context.recv_data_len = 0;
}

/*****************************************************************************/
//  Description : get is download media
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetDownloadMedia(void)
{
    return s_browser_context.is_download_media;
}

/*****************************************************************************/
//  Description : set setting change
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetSettingChange(BOOLEAN is_setting_changed)
{
    s_browser_context.is_setting_changed = is_setting_changed;
}

/*****************************************************************************/
//  Description : get setting change
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetSettingChange(void)
{
    return s_browser_context.is_setting_changed;
}

/*****************************************************************************/
//  Description : reset the param for reset factory setting
//  Global resource dependence : s_nv_setting,MMIBROWSER_SETTING
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ResetNvSetting(void)
{
    uint32 i =0;
    uint32  default_homapage_len = SCI_STRLEN((char*)s_browser_custom_default_homepage);

    SCI_MEMSET(&s_nv_setting, 0x00, sizeof(s_nv_setting));
    s_nv_setting.font = MMIBROWSER_DEFAULT_FONT;
    s_nv_setting.is_cache_enable = TRUE;
    s_nv_setting.is_cookie_enable = TRUE;
    s_nv_setting.is_google_change_enable = FALSE;
    s_nv_setting.is_image_loaded = TRUE;
    s_nv_setting.is_sound_played = TRUE;
    s_nv_setting.is_ssl_warning_enable = TRUE;
    
    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
#ifndef WIN32    
        s_nv_setting.sim_net_index[i] = MMIAPICONNECTION_GetAccountDefaultIndex((MN_DUAL_SYS_E)i, MMICONNECTION_ACCOUNT_BROWSER);
#else
        s_nv_setting.sim_net_index[i] = 2;//cmnet for simulator
#endif
    }

    default_homapage_len = MIN(MMIBROWSER_HOMEPAGE_MAX_URL_LEN, default_homapage_len);
    SCI_MEMCPY(s_nv_setting.homepage, s_browser_custom_default_homepage, default_homapage_len);
    
    MMINV_WRITE(MMIBROWSER_SETTING, &s_nv_setting);
}

/*****************************************************************************/
//  Description : reset the param for reset factory setting
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_ResetFactorySetting(void)
{
    MMIBROWSER_ResetNvSetting();
    
#ifdef BROWSER_SEARCH_SUPPORT
    MMIBROWSER_SetSearchEngine(MMIBROWSER_SEARCH_ENGINE_DEFAULT);
#endif
    MMIAPICONNECTION_AdaptingAccount(MMI_DUAL_SYS_MAX, MMICONNECTION_ACCOUNT_BROWSER);
    MMIBRW_SetDefaultBookmark();//reset bookmark after reset factory default or first power on phone
}

/*****************************************************************************/
//  Description : Clean Usert Data Info
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_CleanUserInfo(void)
{
    MMIDORADO_ResetFactorySetting();
#ifdef MMIDORADO_MY_NAV_SUPPORT
    MMIBROWSER_ResetMyNavInfo();
#endif

    BRW_ClearAllCache();//MS00218502删除所有的cache文件
    BRW_ClearAllCookie();
    BRW_ClearVisitHis();//删除历史记录
#ifdef MMIDORADO_INPUT_HISTORY_SUPPORT
    BRW_ClearInputHis();//MS00248587
#endif
    
#ifdef BROWSER_SEARCH_SUPPORT
    MMIBROWSER_CleanKeyInfo();
#endif    
}

/*****************************************************************************/
//  Description : set sim type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_SetSim(MN_DUAL_SYS_E sim)
{
    s_browser_start_info.sim_type = sim;
    return MMIBROWSER_SUCC;
}

/*****************************************************************************/
//  Description : get sim type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIBROWSER_GetSim(void)
{
    return s_browser_start_info.sim_type;
}

/*****************************************************************************/
//  Description : update the edit url in web
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateUrl(char* url_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;

    do 
    {
        if (PNULL == url_ptr)
        {
            error_code = MMIBROWSER_ERROR_INPUT;
            break;
        }
        else
        {
            MMIBROWSER_FreeUrl();//protect
            
            len = strlen(url_ptr);
            s_browser_context.url_ptr = SCI_ALLOCA(len + 1);
            if (PNULL == s_browser_context.url_ptr)
            {
                //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateUrl ALLOC FAIL len =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_583_112_2_18_2_5_51_92,(uint8*)"d", len);
                error_code = MMIBROWSER_FAIL;
                break;
            }
            
            SCI_MEMSET(s_browser_context.url_ptr,0,len + 1);
            strncpy(s_browser_context.url_ptr,url_ptr,len); 
        }
        
    } while(0);

    //SCI_TRACE_LOW:"MMIBROWSER_UpdateUrl error_code= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_594_112_2_18_2_5_51_93,(uint8*)"d", error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : update the user agent
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateUserAgent(char* user_agent_ptr)
{
    uint16 len = 0;

    if (PNULL == user_agent_ptr)
    {
        return MMIBROWSER_ERROR_INPUT;
    }
    else
    {
        MMIBROWSER_FreeUserAgent();//protect
        
        len = SCI_STRLEN(user_agent_ptr);
        s_browser_start_info.user_agent_ptr = SCI_ALLOCA(len + 1);
        if (PNULL == s_browser_start_info.user_agent_ptr)
        {
            //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateUserAgent ALLOC FAIL len =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_619_112_2_18_2_5_51_94,(uint8*)"d", len);
            return MMIBROWSER_FAIL;
        }
        SCI_MEMSET(s_browser_start_info.user_agent_ptr,0,len + 1);
        SCI_MEMCPY(s_browser_start_info.user_agent_ptr,user_agent_ptr,len);
    }
    return MMIBROWSER_SUCC;
}

/*****************************************************************************/
//  Description : update the ua profile
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateUaProfile(char* ua_profile_ptr)
{
    uint16 len = 0;

    if (PNULL == ua_profile_ptr)
    {
        return MMIBROWSER_ERROR_INPUT;
    }
    else
    {
        MMIBROWSER_FreeUaProfile();//protect
        
        len = SCI_STRLEN(ua_profile_ptr);
        s_browser_start_info.ua_profile_ptr = SCI_ALLOCA(len + 1);
        if (PNULL == s_browser_start_info.ua_profile_ptr)
        {
            //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateUaProfile ALLOC FAIL len =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_648_112_2_18_2_5_51_95,(uint8*)"d", len);
            return MMIBROWSER_FAIL;
        }
        SCI_MEMSET(s_browser_start_info.ua_profile_ptr,0,len + 1);
        SCI_MEMCPY(s_browser_start_info.ua_profile_ptr,ua_profile_ptr,len);
    }
    return MMIBROWSER_SUCC;
}
/*****************************************************************************/
//  Description : update the user name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateUserName(wchar* user_name_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;
    uint32 utf8_buf_len = 0;
    
    do 
    {
        if (PNULL == user_name_ptr)
        {
            error_code = MMIBROWSER_FAIL;
            break;
        }
        else
        {
            MMIBROWSER_FreeUserName();//protect
            
            len = MMIAPICOM_Wstrlen(user_name_ptr);
            utf8_buf_len= (len * 3 + 1);
            s_browser_context.user_name_ptr = SCI_ALLOCA(utf8_buf_len);
            if (PNULL == s_browser_context.user_name_ptr)
            {
                //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateUserName ALLOC FAIL len =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_679_112_2_18_2_5_51_96,(uint8*)"d", len);
                error_code = MMIBROWSER_FAIL;
                break;
            }

            SCI_MEMSET(s_browser_context.user_name_ptr, 0, utf8_buf_len);
            GUI_WstrToUTF8((uint8*)s_browser_context.user_name_ptr, utf8_buf_len, user_name_ptr, len);
            s_browser_context.is_setting_username = TRUE;
        }
        
    } while(0);
    return error_code;
}

/*****************************************************************************/
//  Description : update the password
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdatePassword(wchar* password_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;
    do 
    {
        if (PNULL == password_ptr)
        {
            error_code = MMIBROWSER_FAIL;
            break;
        }
        else
        {
            MMIBROWSER_FreePassword();//protect
            
            len = MMIAPICOM_Wstrlen(password_ptr);
            s_browser_context.password_ptr = SCI_ALLOCA(len + 1);
            if (PNULL == s_browser_context.password_ptr)
            {
                //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdatePassword ALLOC FAIL len =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_716_112_2_18_2_5_51_97,(uint8*)"d", len);
                error_code = MMIBROWSER_FAIL;
                break;
            }
            
            SCI_MEMSET(s_browser_context.password_ptr,0,len + 1);
            MMIAPICOM_WstrToStr(password_ptr, (uint8*)s_browser_context.password_ptr);
            s_browser_context.is_setting_password = TRUE;
        }
        
    } while(0);
    return error_code;
}
/*****************************************************************************/
//  Description : free the edit url in web
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeUrl(void)
{
    if (PNULL != s_browser_context.url_ptr)
    {
        SCI_FREE(s_browser_context.url_ptr);
        s_browser_context.url_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : free the user name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeUserName(void)
{
    if (PNULL != s_browser_context.user_name_ptr)
    {
        SCI_FREE(s_browser_context.user_name_ptr);
        s_browser_context.user_name_ptr = PNULL;
    }
    s_browser_context.is_setting_username = FALSE;
}

/*****************************************************************************/
//  Description : free the password
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreePassword(void)
{
    if (PNULL != s_browser_context.password_ptr)
    {
        SCI_FREE(s_browser_context.password_ptr);
        s_browser_context.password_ptr = PNULL;
    }
    s_browser_context.is_setting_password = FALSE;
}

/*****************************************************************************/
//  Description : free the user_agent_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeUserAgent(void)
{
    if (PNULL != s_browser_start_info.user_agent_ptr)
    {
        SCI_FREE(s_browser_start_info.user_agent_ptr);
        s_browser_start_info.user_agent_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : free the user_agent_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeUaProfile(void)
{
    if (PNULL != s_browser_start_info.ua_profile_ptr)
    {
        SCI_FREE(s_browser_start_info.ua_profile_ptr);
        s_browser_start_info.ua_profile_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : get password setting
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetPasswordSetting(void)
{
    return s_browser_context.is_setting_password;
}

/*****************************************************************************/
//  Description : get user name setting
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetUserNameSetting(void)
{
    return s_browser_context.is_setting_username;
}

/*****************************************************************************/
//  Description : get the edit url
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetUrl(void)
{
#ifndef MMIDORADO_WRE_TRANSCODE_SUPPORT
    return s_browser_context.url_ptr;
#else
    return (char*)BrwGwtMovetoGwtEnd((uint8*)(s_browser_context.url_ptr));//for sprd wre transcoding
#endif

}

/*****************************************************************************/
//  Description : get the username
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetUserName(void)
{
    return s_browser_context.user_name_ptr;
}

/*****************************************************************************/
//  Description : get the password
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetPassword(void)
{
    return s_browser_context.password_ptr;
}

/*****************************************************************************/
//  Description : set browser init
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetKernelInit(BOOLEAN is_inited)
{
    s_browser_start_info.is_browser_inited = is_inited;
}

/*****************************************************************************/
//  Description : get browser is inited
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetKernelInit(void)
{
    return s_browser_start_info.is_browser_inited;
}

/*****************************************************************************/
//  Description : set browser instance init
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIntanceInit(BOOLEAN is_inited)
{
    s_browser_context.is_instance_inited = is_inited;
}

/*****************************************************************************/
//  Description : get browser instance is inited
//  Global resource dependence : 
//  Author: fen,xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIntanceInit(void)
{
    return s_browser_context.is_instance_inited;
}

/*****************************************************************************/
//  Description : set instance
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetInstance(BRW_INSTANCE brw_instace)
{
    s_browser_context.browser_instance = brw_instace;
}

/*****************************************************************************/
//  Description : get instance
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_INSTANCE MMIBROWSER_GetInstance(void)
{
    return s_browser_context.browser_instance;
}

/*****************************************************************************/
//  Description : set fixed instance
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetFixedInstance(BRW_INSTANCE brw_instace)
{
    s_browser_start_info.fixed_browser_instance = brw_instace;
}

/*****************************************************************************/
//  Description : get fixed instance
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BRW_INSTANCE MMIBROWSER_GetFixedInstance(void)
{
    return s_browser_start_info.fixed_browser_instance;
}

/*****************************************************************************/
//  Description : set total data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetTotalDataLen(uint32 total_data_len)
{
    s_browser_context.total_data_len = total_data_len;
}

/*****************************************************************************/
//  Description : get total data len
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetTotalDataLen(void)
{
    return s_browser_context.total_data_len;
}

/*****************************************************************************/
//  Description : set total media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetTotalMediaNum(uint32 total_media_num)
{
    s_browser_context.total_media_num = total_media_num;
}

/*****************************************************************************/
//  Description : get total media num
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetTotalMediaNum(void)
{
    return s_browser_context.total_media_num;
}

/*****************************************************************************/
//  Description : set browser is loading
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetLoading(BOOLEAN is_loading)
{
    s_browser_context.is_loading = is_loading;
}

/*****************************************************************************/
//  Description : set browser is save to phone
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
/*PUBLIC void MMIBROWSER_SetSaveToPhone(BOOLEAN is_save_to_phone)
{
    s_browser_context.is_save_to_phone = is_save_to_phone;
}*/

/*****************************************************************************/
//  Description : get the browser is save to phone
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
/*PUBLIC BOOLEAN MMIBROWSER_GetSaveToPhone(void)
{
    return s_browser_context.is_save_to_phone;
}*/

/*****************************************************************************/
//  Description : set browser is save to phone
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetSaveDeviceType(MMIFILE_DEVICE_E dev)
{
    s_browser_context.save_device= dev;
}

/*****************************************************************************/
//  Description : get the browser is save to phone
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIBROWSER_GetSaveDeviceType(void)
{
    return s_browser_context.save_device;
}

/*****************************************************************************/
//  Description : get the browser is loading
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetLoading(void)
{
    return s_browser_context.is_loading;
}

/*****************************************************************************/
//  Description : get the browser is need clear(clear all and repait, when open, lose focus, lcd switch...)
//  Global resource dependence : 
//  Author: li.li
//  Note: for MS00206081--is_need_clear is not useful(因地址栏显示的流程完善后不再依赖full paint消息,故该变量已无效)
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsNeedClear(void)
{
    return s_browser_context.is_need_clear;
}

/*****************************************************************************/
//  Description : set the browser is need clear(clear all and repait, when open, lose focus, lcd switch...)
//  Global resource dependence : 
//  Author: li.li
//  Note: for MS00206081--is_need_clear is not useful(因地址栏显示的流程完善后不再依赖full paint消息,故该变量已无效)
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsNeedClear(BOOLEAN is_need_clear)
{
    s_browser_context.is_need_clear = is_need_clear;
}

/*****************************************************************************/
//  Description : set browser bookmark state
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetBookmarkState(MMIBROWSER_BOOKMARK_STATE_E bookmark_state)
{
    s_browser_start_info.bookmark_state = bookmark_state;
}
/*****************************************************************************/
//  Description : get the browser bookmark state
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_BOOKMARK_STATE_E MMIBROWSER_GetBookmarkState(void)
{
    return s_browser_start_info.bookmark_state;
}


/*****************************************************************************/
//  Description : set the browser is need display
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetNeedDisplay(BOOLEAN is_need_display)
{
    s_browser_context.is_need_display = is_need_display;
}

/*****************************************************************************/
//  Description : get the browser is need display
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetNeedDisplay(void)
{
    return s_browser_context.is_need_display;
}

/*****************************************************************************/
//  Description : set animate connect control state
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetAnimateConnectCtrlState(MMIBROWSER_ANIMATE_STATE_E state, BOOLEAN is_update)
{
    GUIANIM_CTRL_INFO_T     ctrl_info = {0};
    GUIANIM_DATA_INFO_T     data_info = {0};
    GUIANIM_DISPLAY_INFO_T  display_info = {0};
    char    *url_ptr = PNULL;
    MMI_CTRL_ID_T anim_ctrl_id = MMIBROWSER_ANIMATE_CONNECTING_CTRL_ID;
    
    if (MMK_IsOpenWin(MMIBROWSER_MAIN_WIN_ID))
    {
        url_ptr = MMIBROWSER_GetUrl();
        
        //SCI_TRACE_LOW:"MMIBROWSER_SetAnimateConnectCtrlState:state = %d,s_browser_context.animate_state = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1073_112_2_18_2_5_52_98,(uint8*)"dd", state, s_browser_context.animate_state);
        if(state >= MMIBROWSER_ANIMATE_STATE_MAX)
        {
            return;
        }

        ctrl_info.is_ctrl_id = TRUE;
        ctrl_info.ctrl_id = anim_ctrl_id;
        
        if (PNULL != url_ptr && (0 == CFLCHAR_StrNICmp(url_ptr,MMIBROWSER_HTTPS_STRING,strlen(MMIBROWSER_HTTPS_STRING))) )
        {
            data_info.anim_id = IMAGE_BROWSER_SSL_CONNECTION_ANIM;
        }
        else
        {
            data_info.anim_id = IMAGE_BROWSER_CONNECTION_ANIM;
        }
        
        display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;
        display_info.is_update   = (MMIBROWSER_IsMainActive())? is_update:FALSE;
        display_info.is_bg_buf   = TRUE;
        
        s_browser_context.animate_state = state;
        //设置动画
        GUIANIM_SetParam(&ctrl_info,&data_info,PNULL,&display_info);
        
        switch (state)
        {
        case MMIBROWSER_ANIMATE_STATE_NONE:
            GUIANIM_PauseAnim(anim_ctrl_id);
            break;

        case MMIBROWSER_ANIMATE_STATE_ANIM:
            GUIANIM_ResumeAnim(anim_ctrl_id);
            break;

        case MMIBROWSER_ANIMATE_STATE_STOP:
            GUIANIM_PauseAnim(anim_ctrl_id);
            break;

        default:
            break;
        }
    }
}

/*****************************************************************************/
//  Description : update auth req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateAuthReq(BRW_MSG_AUTH_REQ_T* req_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;

#if 0//delete addr_arr by fen.xie for useless    
    uint16 len = 0;

    if (PNULL != req_ptr && PNULL != req_ptr->auth_cbf && 0 != strlen((char*)(req_ptr->addr_arr)))
    {
        SCI_MEMSET(s_browser_context.auth_req.addr_arr,0,sizeof(s_browser_context.auth_req.addr_arr));
        len = strlen((char*)(req_ptr->addr_arr));
        strncpy((char*)(s_browser_context.auth_req.addr_arr),(char*)(req_ptr->addr_arr), len); 
        s_browser_context.auth_req.auth_cbf = req_ptr->auth_cbf;
        s_browser_context.auth_req.handle = req_ptr->handle;
        error_code = MMIBROWSER_SUCC;
    }
#else
    if ((PNULL != req_ptr) && (PNULL != req_ptr->auth_cbf))
    {
        s_browser_context.auth_req.auth_cbf = req_ptr->auth_cbf;
        s_browser_context.auth_req.handle = req_ptr->handle;
        s_browser_context.auth_req.auth_type = req_ptr->auth_type;
        error_code = MMIBROWSER_SUCC;
    }
#endif
    else
    {
        error_code = MMIBROWSER_FAIL;
    }
    
    return error_code;
}
/*****************************************************************************/
//  Description : update enter file name req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateEnterFileName(uint16 *res_name_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;
    
    if ((PNULL != res_name_ptr) &&  ((len = MMIAPICOM_Wstrlen(res_name_ptr)) > 0))
    {        
        SCI_MEMSET(&s_browser_context.enter_file_name.res_name_arr, 0x00, sizeof(s_browser_context.enter_file_name.res_name_arr));//reset
        MMI_WSTRNCPY(s_browser_context.enter_file_name.res_name_arr, 
            BRW_MAX_URL_LEN, 
            res_name_ptr, 
            len , 
            MIN(len, BRW_MAX_URL_LEN));
        
        error_code = MMIBROWSER_SUCC;
    }
    else
    {
        error_code = MMIBROWSER_FAIL;
    }

    return error_code;
}

/*****************************************************************************/
//  Description : update enter file name req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateFileNameReq(BRW_MSG_ENTER_FILENM_REQ_T* req_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;

#ifdef OMADL_SUPPORT
    if (s_browser_context.enter_file_name.next_url_ptr != PNULL)
    {
        SCI_FREE(s_browser_context.enter_file_name.next_url_ptr);
        s_browser_context.enter_file_name.next_url_ptr = PNULL;
    }
#endif    
    SCI_MEMSET(&s_browser_context.enter_file_name, 0, sizeof(s_browser_context.enter_file_name));//reset

    if ((PNULL != req_ptr) && (PNULL != req_ptr->enter_filenm_cbf) 
        &&  ((len = MMIAPICOM_Wstrlen(req_ptr->res_name_arr)) > 0)
        )
    {
        SCI_MEMCPY(&s_browser_context.enter_file_name, req_ptr, sizeof(BRW_MSG_ENTER_FILENM_REQ_T));

#ifdef OMADL_SUPPORT
        s_browser_context.enter_file_name.next_url_ptr = PNULL;

        if (req_ptr->next_url_ptr != PNULL)
        {
            uint32  next_url_len = 0;
            uint8   *next_url_ptr = PNULL;
            
            next_url_len = SCI_STRLEN((char *)req_ptr->next_url_ptr);
            if (next_url_len > 0)
            {
                next_url_ptr = SCI_ALLOCA(next_url_len + 1);
                if (next_url_ptr != PNULL)
                {
                    SCI_MEMSET(next_url_ptr, 0x00, (next_url_len + 1));
                    SCI_MEMCPY(next_url_ptr, req_ptr->next_url_ptr, next_url_len);
                }
                else
                {
                    //SCI_TRACE_LOW:"[BRW]W MMIBROWSER_UpdateFileNameReq next_url_ptr ALLOC Fail!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1226_112_2_18_2_5_53_99,(uint8*)"");
                }                
            }
            s_browser_context.enter_file_name.next_url_ptr = next_url_ptr;
        }
#endif
        
        error_code = MMIBROWSER_SUCC;
    }
    else
    {
        error_code = MMIBROWSER_FAIL;
    }
    
    return error_code;
}
#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : update ssl cert
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateSSLCert(BRW_MSG_SSL_CERT_IND_T* ssl_cert_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    
    
    if (PNULL == ssl_cert_ptr || PNULL == ssl_cert_ptr->cert_cbf)
    {
        error_code = MMIBROWSER_FAIL;
    }
    else
    {
        s_browser_context.ssl_cert.cert_cbf = ssl_cert_ptr->cert_cbf;
        s_browser_context.ssl_cert.handle = ssl_cert_ptr->handle;
        error_code = MMIBROWSER_SUCC;
    }
    
    return error_code;
}

/*****************************************************************************/
//  Description : get ssl cert
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_MSG_SSL_CERT_IND_T* MMIBROWSER_GetSSLCert(void)
{
    return &(s_browser_context.ssl_cert);
}
#endif
/*****************************************************************************/
//  Description : update bookmark edit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateBookmarkEdit(MMIBROWSER_BOOKMARK_EDIT_T* bookmark_edit_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    //int32 len = 0;
    do 
    {
        if (PNULL == bookmark_edit_ptr)
        {
            error_code = MMIBROWSER_FAIL;
            break;
        }
        else
        {
            if (PNULL != bookmark_edit_ptr->title_ptr && bookmark_edit_ptr->title_len > 0)
            {
                if (PNULL != s_browser_start_info.bookmark_edit.title_ptr)
                {
                    SCI_FREE(s_browser_start_info.bookmark_edit.title_ptr);
                    s_browser_start_info.bookmark_edit.title_ptr = PNULL;
                }
                s_browser_start_info.bookmark_edit.title_len = 0;
                
                s_browser_start_info.bookmark_edit.title_ptr = SCI_ALLOCA((bookmark_edit_ptr->title_len + 1) * sizeof(wchar));
                if (PNULL == s_browser_start_info.bookmark_edit.title_ptr)
                {
                    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateBookmarkEdit title ALLOC FAIL len =%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1304_112_2_18_2_5_53_100,(uint8*)"d", bookmark_edit_ptr->title_len);
                    error_code = MMIBROWSER_FAIL;
                    break;
                }
                SCI_MEMSET(s_browser_start_info.bookmark_edit.title_ptr,0, (bookmark_edit_ptr->title_len + 1) * sizeof(wchar));
                MMIAPICOM_Wstrncpy(s_browser_start_info.bookmark_edit.title_ptr,bookmark_edit_ptr->title_ptr,bookmark_edit_ptr->title_len);
                s_browser_start_info.bookmark_edit.title_len = bookmark_edit_ptr->title_len;
            }
            
            if (PNULL != bookmark_edit_ptr->url_ptr && bookmark_edit_ptr->url_len > 0)
            {
                if (PNULL != s_browser_start_info.bookmark_edit.url_ptr)
                {
                    SCI_FREE(s_browser_start_info.bookmark_edit.url_ptr);
                    s_browser_start_info.bookmark_edit.url_ptr = PNULL;
                }
                s_browser_start_info.bookmark_edit.url_len = 0;
                
                s_browser_start_info.bookmark_edit.url_ptr = SCI_ALLOCA((bookmark_edit_ptr->url_len + 1) * sizeof(wchar));
                if (PNULL == s_browser_start_info.bookmark_edit.url_ptr)
                {
                    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateBookmarkEdit URL ALLOC FAIL len =%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1325_112_2_18_2_5_53_101,(uint8*)"d", bookmark_edit_ptr->url_len);
                    error_code = MMIBROWSER_FAIL;
                    break;
                }
                SCI_MEMSET(s_browser_start_info.bookmark_edit.url_ptr, 0, (bookmark_edit_ptr->url_len + 1) * sizeof(wchar));
                MMIAPICOM_Wstrncpy(s_browser_start_info.bookmark_edit.url_ptr,bookmark_edit_ptr->url_ptr,bookmark_edit_ptr->url_len);
                s_browser_start_info.bookmark_edit.url_len = bookmark_edit_ptr->url_len;
            }
            
        }
        
    } while(0);
    return error_code;
}

/*****************************************************************************/
//  Description : get auth req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_MSG_AUTH_REQ_T* MMIBROWSER_GetAuthReq(void)
{
    return &(s_browser_context.auth_req);
}

/*****************************************************************************/
//  Description : get entry file name req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BRW_MSG_ENTER_FILENM_REQ_T* MMIBROWSER_GetFileNameReq(void)
{
    return &(s_browser_context.enter_file_name);
}
/*****************************************************************************/
//  Description : get bookmark edit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_BOOKMARK_EDIT_T* MMIBROWSER_GetBookmarkEdit(void)
{
    return &(s_browser_start_info.bookmark_edit);
}
/*****************************************************************************/
//  Description : free auth req
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeAuthReq(void)
{
    SCI_MEMSET(&s_browser_context.auth_req, 0, sizeof(s_browser_context.auth_req));
}

/*****************************************************************************/
//  Description : free bookmark edit
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeBookmarkEdit(void)
{
    if (PNULL != s_browser_start_info.bookmark_edit.title_ptr)
    {
        SCI_FREE(s_browser_start_info.bookmark_edit.title_ptr);
        s_browser_start_info.bookmark_edit.title_ptr = PNULL;
    }
    s_browser_start_info.bookmark_edit.title_len = 0;

    if (PNULL != s_browser_start_info.bookmark_edit.url_ptr)
    {
        SCI_FREE(s_browser_start_info.bookmark_edit.url_ptr);
        s_browser_start_info.bookmark_edit.url_ptr = PNULL;
    }
    s_browser_start_info.bookmark_edit.url_len = 0;
}

/*****************************************************************************/
//  Description : free bookmark edit title
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeBookmarkEditTitle(void)
{
    if (PNULL != s_browser_start_info.bookmark_edit.title_ptr)
    {
        SCI_FREE(s_browser_start_info.bookmark_edit.title_ptr);
        s_browser_start_info.bookmark_edit.title_ptr = PNULL;
    }
    s_browser_start_info.bookmark_edit.title_len = 0;
}
/*****************************************************************************/
//  Description : free enter file name
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeEnterFileName(void)
{
#ifdef OMADL_SUPPORT
    if (s_browser_context.enter_file_name.next_url_ptr != PNULL)
    {
        SCI_FREE(s_browser_context.enter_file_name.next_url_ptr);
        s_browser_context.enter_file_name.next_url_ptr = PNULL;
    }
#endif    

    SCI_MEMSET(&s_browser_context.enter_file_name, 0, sizeof(s_browser_context.enter_file_name));
}
/*****************************************************************************/
//  Description : free save file
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeSaveFile(void)
{
    if (PNULL != s_browser_context.save_file.wstr_ptr)
    {
        SCI_FREE(s_browser_context.save_file.wstr_ptr);
        s_browser_context.save_file.wstr_ptr = PNULL;
    }
    s_browser_context.save_file.wstr_len = 0;
}
/*****************************************************************************/
//  Description : update save file
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateSaveFile(MMI_STRING_T save_file)
{
    if ((save_file.wstr_len > 0) && (PNULL != save_file.wstr_ptr))
    {
        MMIBROWSER_FreeSaveFile();//reset    
        
        s_browser_context.save_file.wstr_ptr = SCI_ALLOCA((save_file.wstr_len + 1) * sizeof(wchar));
        if (PNULL != s_browser_context.save_file.wstr_ptr)
        {
            SCI_MEMSET(s_browser_context.save_file.wstr_ptr, 0, (save_file.wstr_len + 1) * sizeof(wchar));
            MMIAPICOM_Wstrncpy(s_browser_context.save_file.wstr_ptr, save_file.wstr_ptr, save_file.wstr_len);
            s_browser_context.save_file.wstr_len = save_file.wstr_len;
        }
        else
        {
            //SCI_TRACE_LOW:"[BRW] MMIBROWSER_UpdateSaveFile ALLOC FAIL LEN=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1456_112_2_18_2_5_53_102,(uint8*)"d",save_file.wstr_len);
        }
    }
}

/*****************************************************************************/
//  Description : get save file
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMI_STRING_T* MMIBROWSER_GetSaveFile(void)
{
    return &(s_browser_context.save_file);
}

/*****************************************************************************/
//  Description : set scroll handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetScrollHandle(uint32 scoll_handle)
{
    s_browser_context.scoll_handle = scoll_handle;
}

/*****************************************************************************/
//  Description : get scroll handle
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIBROWSER_GetScrollHandle(void)
{
    return s_browser_context.scoll_handle;
}

/*****************************************************************************/
//  Description : reset context
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ResetContext(void)
{
    MMIBROWSER_FreeAuthReq();
    MMIBROWSER_GetFileNameReq();
    MMIBROWSER_FreePassword();
    MMIBROWSER_FreeSaveFile();
    MMIBROWSER_FreeUrl();
    MMIBROWSER_FreeUserName();
    MMIBROWSER_FreeEnterFileName();
    MMIBROWSER_FreeSnapshotFilename();
    MMIBROWSER_FreeSnapshotUrl();
    MMIBROWSER_FreeIcoFilename();
    
    SCI_MEMSET(&s_browser_context,0,sizeof(s_browser_context));
}

/*****************************************************************************/
//  Description : set browser key timer id  
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetKeyTimerID(uint8 key_timer_id)
{
    s_browser_context.key_timer_id = key_timer_id;
}

/*****************************************************************************/
//  Description : set browser key msg
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetKeyMSG(MMI_MESSAGE_ID_E key_msg)
{
    s_browser_context.key_msg = key_msg;
}

/*****************************************************************************/
//  Description : get browser key timer id  
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIBROWSER_GetKeyTimerID(void)
{
    return s_browser_context.key_timer_id;
}

/*****************************************************************************/
//  Description : get browser key msg
//  Global resource dependence : none
//  Author: li.li
//  Note:
/*****************************************************************************/
PUBLIC MMI_MESSAGE_ID_E MMIBROWSER_GetKeyMSG(void)
{
    return s_browser_context.key_msg;
}

/*****************************************************************************/
//  Description : set begin download
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetBeginDownload(BOOLEAN is_begin_download)
{
    s_browser_context.is_begin_download = is_begin_download;
}

/*****************************************************************************/
//  Description : get begin download
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetBeginDownload(void)
{
    return s_browser_context.is_begin_download;
}

/*****************************************************************************/
//  Description : set is snapshot
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsSnapshot(BOOLEAN is_snapshot)
{
    s_browser_context.is_snapshot = is_snapshot;
}

/*****************************************************************************/
//  Description : get is snapshot
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsSnapshot(void)
{
    return s_browser_context.is_snapshot;
}

/*****************************************************************************/
//  Description : set mime type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetMimeType(MMIBROWSER_MIME_TYPE_E mime_type)
{
    s_browser_context.mime_type = mime_type;
}

/*****************************************************************************/
//  Description : get mime type
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_MIME_TYPE_E MMIBROWSER_GetMimeType(void)
{
    return s_browser_context.mime_type;
}

/*****************************************************************************/
//  Description : update file name
//  Global resource dependence : s_browser_dl_fullname_ptr
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_UpdateFileName(wchar* file_name_ptr)
{
    uint16  len = 0;

    if (s_browser_dl_fullname_ptr != PNULL)
    {
        SCI_FREE(s_browser_dl_fullname_ptr);
    }
    
    if ((file_name_ptr != PNULL) && ((len = MMIAPICOM_Wstrlen(file_name_ptr)) > 0))
    {
        len = MIN(len, MMIFILE_FULL_PATH_MAX_LEN);
        s_browser_dl_fullname_ptr = SCI_ALLOCAZ(sizeof(wchar) * (len+1));
        if (s_browser_dl_fullname_ptr != PNULL)
        {
            MMIAPICOM_Wstrncpy(s_browser_dl_fullname_ptr, file_name_ptr, len);
        }
        else
        {
            SCI_TRACE_LOW("[BRW]W MMIBROWSER_UpdateFileName: alloc fail");
        }
    }
}

/*****************************************************************************/
//  Description : get file name
//  Global resource dependence : s_browser_dl_fullname_ptr
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIBROWSER_GetFileName(void)
{
    return s_browser_dl_fullname_ptr;
}

/*****************************************************************************/
//  Description : get the user agent
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetUserAgent(void)
{
    return s_browser_start_info.user_agent_ptr;
}

/*****************************************************************************/
//  Description : get the ua profile
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetUaProfile(void)
{
    return s_browser_start_info.ua_profile_ptr;
}

/*****************************************************************************/
//  Description : set is use wifi
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetIsUseWifi(BOOLEAN is_use_wifi)
{
    s_browser_start_info.is_use_wifi = is_use_wifi;
    //SCI_TRACE_LOW:"BRW MMIBROWSER_SetIsUseWifi is_use_wifi = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1697_112_2_18_2_5_53_106,(uint8*)"d", is_use_wifi);
}

/*****************************************************************************/
//  Description : get is use wifi
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBROWSER_GetIsUseWifi(void)
{
    return s_browser_start_info.is_use_wifi;
}

/*****************************************************************************/
//  Description : update the snapshot file name in web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateSnapshotFilename(wchar* file_name_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;
    do 
    {
        if (PNULL == file_name_ptr)
        {
            error_code = MMIBROWSER_FAIL;
            break;
        }
        else
        {
            MMIBROWSER_FreeSnapshotFilename();//protect
            
            len = MMIAPICOM_Wstrlen(file_name_ptr);
            s_browser_context.snapshot_filename_ptr = SCI_ALLOCA((len + 1)*sizeof(wchar));
            if (PNULL == s_browser_context.snapshot_filename_ptr)
            {
                //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateSnapshotFilename ALLOC FAIL len =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1732_112_2_18_2_5_54_107,(uint8*)"d", len);
                error_code = MMIBROWSER_FAIL;
                break;
            }
            
            SCI_MEMSET(s_browser_context.snapshot_filename_ptr,0,(len + 1)*sizeof(wchar));
            MMIAPICOM_Wstrncpy(s_browser_context.snapshot_filename_ptr,file_name_ptr,len); 
        }
        
    } while(0);
    return error_code;
}

/*****************************************************************************/
//  Description : free the snapshot file name in web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeSnapshotFilename(void)
{
    if (PNULL != s_browser_context.snapshot_filename_ptr)
    {
        SCI_FREE(s_browser_context.snapshot_filename_ptr);
        s_browser_context.snapshot_filename_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : get the snapshot file name in web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIBROWSER_GetSnapshotFilename(void)
{
    return s_browser_context.snapshot_filename_ptr;
}

/*****************************************************************************/
//  Description : set entry_type
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetEntryType(MMIWWW_ENTRY_TYPE_E entry_type)
{
    s_browser_start_info.entry_type = entry_type;
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_SetEntryType entry_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1774_112_2_18_2_5_54_108,(uint8*)"d", entry_type);
}


/*****************************************************************************/
//  Description : get entry_type
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIWWW_ENTRY_TYPE_E MMIBROWSER_GetEntryType(void)
{
    return s_browser_start_info.entry_type;
}

/*****************************************************************************/
//  Description : set entry_type when open the web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_SetWebEntryType(MMIBROWSER_WEB_ENTRY_TYPE_E entry_type)
{
    s_browser_context.entry_type = entry_type;
    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_SetWebEntryType entry_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1794_112_2_18_2_5_54_109,(uint8*)"d", entry_type);
}


/*****************************************************************************/
//  Description : get entry_type when open the web
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_WEB_ENTRY_TYPE_E MMIBROWSER_GetWebEntryType(void)
{
    return s_browser_context.entry_type;
}

/*****************************************************************************/
//  Description : free the url for entry browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeEntryUrl(void)
{
    if (PNULL != s_browser_start_info.entry_url_ptr)
    {
        SCI_FREE(s_browser_start_info.entry_url_ptr);
        s_browser_start_info.entry_url_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : update the url for entry browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateEntryUrl(char* url_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;

    do 
    {
        if (PNULL == url_ptr)
        {
            error_code = MMIBROWSER_ERROR_INPUT;
            break;
        }
        else
        {
            MMIBROWSER_FreeEntryUrl();//protect
            
            len = strlen(url_ptr);
            s_browser_start_info.entry_url_ptr = SCI_ALLOCA(len + 1);
            if (PNULL == s_browser_start_info.entry_url_ptr)
            {
                //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateEntryUrl ALLOC FAIL len =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1844_112_2_18_2_5_54_110,(uint8*)"d", len);
                error_code = MMIBROWSER_FAIL;
                break;
            }
            
            SCI_MEMSET(s_browser_start_info.entry_url_ptr,0,len + 1);
            strncpy(s_browser_start_info.entry_url_ptr,url_ptr,len); 
        }
        
    } while(0);

    //SCI_TRACE_LOW:"MMIBROWSER_UpdateEntryUrl error_code= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1855_112_2_18_2_5_54_111,(uint8*)"d", error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : get the url for entry browser
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetEntryUrl(void)
{
    return s_browser_start_info.entry_url_ptr;
}

/*****************************************************************************/
//  Description : reset browser start infomation
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_ResetStartInfo(void)
{
    MMIBROWSER_FreeBookmarkEdit();
    MMIBROWSER_FreeEntryUrl();
    MMIBROWSER_FreeUserAgent();    
    MMIBROWSER_FreeUaProfile();    

    SCI_MEMSET(&s_browser_start_info,0,sizeof(s_browser_start_info));
}

/*****************************************************************************/
//  Description : update the snapshot_url_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateSnapshotUrl(char* url_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;

    do 
    {
        if (PNULL == url_ptr)
        {
            error_code = MMIBROWSER_ERROR_INPUT;
            break;
        }
        else
        {
            MMIBROWSER_FreeSnapshotUrl();//protect
            
            len = strlen(url_ptr);
            if (len > 0)
            {
                s_browser_context.snapshot_url_ptr = SCI_ALLOCA(len + 1);
                if (PNULL == s_browser_context.snapshot_url_ptr)
                {
                    //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateSnapshotUrl ALLOC FAIL len =%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1909_112_2_18_2_5_54_112,(uint8*)"d", len);
                    error_code = MMIBROWSER_FAIL;
                    break;
                }
                
                SCI_MEMSET(s_browser_context.snapshot_url_ptr, 0, len + 1);
                strncpy(s_browser_context.snapshot_url_ptr, url_ptr, len); 
            }
        }
        
    } 
    while(0);

    //SCI_TRACE_LOW:"MMIBROWSER_UpdateSnapshotUrl error_code= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1922_112_2_18_2_5_54_113,(uint8*)"d", error_code);

    return error_code;
}

/*****************************************************************************/
//  Description : free the snapshot_url_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeSnapshotUrl(void)
{
    if (PNULL != s_browser_context.snapshot_url_ptr)
    {
        SCI_FREE(s_browser_context.snapshot_url_ptr);
        s_browser_context.snapshot_url_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : get the snapshot_url_ptr
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIBROWSER_GetSnapshotUrl(void)
{
    return s_browser_context.snapshot_url_ptr;
}


/*****************************************************************************/
//  Description : update the ico file name 
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIBROWSER_UpdateIcoFilename(wchar* file_name_ptr)
{
    MMIBROWSER_ERROR_E error_code = MMIBROWSER_SUCC;
    uint16 len = 0;
    do 
    {
        if (PNULL == file_name_ptr)
        {
            error_code = MMIBROWSER_FAIL;
            break;
        }
        else
        {
            MMIBROWSER_FreeIcoFilename();//protect
            
            len = MMIAPICOM_Wstrlen(file_name_ptr);
            s_browser_context.ico_filename_ptr = SCI_ALLOCAZ((len + 1)*sizeof(wchar));
            if (PNULL == s_browser_context.ico_filename_ptr)
            {
                //SCI_TRACE_LOW:"[BRW]MMIBROWSER_UpdateSnapshotFilename ALLOC FAIL len =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_1973_112_2_18_2_5_54_114,(uint8*)"d", len);
                error_code = MMIBROWSER_FAIL;
                break;
            }
            
            MMIAPICOM_Wstrncpy(s_browser_context.ico_filename_ptr,file_name_ptr,len); 
        }
        
    } while(0);
    return error_code;
}

/*****************************************************************************/
//  Description : free the ico file name
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBROWSER_FreeIcoFilename(void)
{
    if (PNULL != s_browser_context.ico_filename_ptr)
    {
        SCI_FREE(s_browser_context.ico_filename_ptr);
        s_browser_context.ico_filename_ptr = PNULL;
    }
}

/*****************************************************************************/
//  Description : get the ico file name
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC wchar* MMIBROWSER_GetIcoFilename(void)
{
    return s_browser_context.ico_filename_ptr;
}

/*****************************************************************************/
//  Description : set customized Connection Settings
//  Global resource dependence : s_browser_start_info
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDORADO_SetConnectionSettings(MMIDORADO_CUSTOMIZED_CONNECTION_T customized_connection)
{
    if (customized_connection.is_connection_customized 
        && (customized_connection.connection_index < MMICONNECTION_MAX_SETTING_NUM))//to do
    {
        SCI_MEMCPY(&s_browser_start_info.customized_connection, &customized_connection, sizeof(MMIDORADO_CUSTOMIZED_CONNECTION_T));
        //SCI_TRACE_LOW:"[BRW]MMIDORADO_SetConnectionSettings:connection_index=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIBROWSER_SETTING_2017_112_2_18_2_5_54_115,(uint8*)"d", customized_connection.connection_index);
    }
    else
    {
        SCI_MEMSET(&s_browser_start_info.customized_connection, 0x00, sizeof(MMIDORADO_CUSTOMIZED_CONNECTION_T));
    }
}

/*****************************************************************************/
//  Description : Get customized Connection Settings
//  Global resource dependence : s_browser_start_info
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIDORADO_CUSTOMIZED_CONNECTION_T MMIDORADO_GetConnectionSettings(void)
{
    return s_browser_start_info.customized_connection;
}


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
