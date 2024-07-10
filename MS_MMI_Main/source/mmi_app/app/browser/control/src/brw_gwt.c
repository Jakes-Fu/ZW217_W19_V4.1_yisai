/****************************************************************************************
** File Name:      brw_gwt.c								                            *
** Author:         juan.zhang                                                           *
** Date:           2009.6.29                                                            *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                  *
** Description:    This file is for google transcode					                *
*****************************************************************************************
**                         Important Edit History                                       *
** -------------------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                          *
** 2009.6        juan.zhang         Create                                              *
**                                                                                      *
*****************************************************************************************/
#if defined(MMIDORADO_GOOGLE_TRANSCODE_SUPPORT) || defined(MMIDORADO_WRE_TRANSCODE_SUPPORT)
#include "brw_control_api.h"
#include "brw_plinterface.h"
#include "brw_utility.h"
#ifndef MMIDORADO_WRE_TRANSCODE_SUPPORT
#define GOOGLE_URL					"http://www.google.com/gwt/n?"
#else
#define GOOGLE_URL					"http://location.52wre.com/?"//for sprd wre transcoding
#endif

#define	GOOGLE_LANG_EN				"hl=en"
#define	GOOGLE_LANG_CH				"hl=zh-cn"
#ifndef MMIDORADO_WRE_TRANSCODE_SUPPORT
#define	GOOGLE_QUERY				"u="
#else
#define	GOOGLE_QUERY				"url="//for sprd wre transcoding
#endif

#define MBROWSER_ID					"client=ms-aff-mBrowser"
#define FISRT_PAGE_ID				"_gwt_pg=0"
#define URL_AND						"&"
#define HTTP_WAP					"http://wap."

LOCAL BOOLEAN CheckIfUrlCanChange(char *url_ptr)
{
	char *scheme_ptr = 0;

	if(0==url_ptr)
	{
		return FALSE;
	}
	scheme_ptr = BRWUtil_GetUrlScheme(url_ptr);
	if(0==scheme_ptr)
	{
		return FALSE;
	}
	if(0!=BRW_Stricmp((const uint8*)scheme_ptr,(const uint8*)"http"))
	{
		return FALSE;
	}
	BRWMem_Free(scheme_ptr);
	return TRUE;
}
/***************************************************************************************/
/***GOOGLE_URL+GOOGLE_QUERY+src_url_ptr+&+MBROWSER_ID+&+FISRT_PAGE_ID+&+GOOGLE_LANG_CH**/
/***************************************************************************************/
PUBLIC char* BrwGwtChangeUrl(char *src_url_ptr,BRW_LANG_E lang)
{
    char *changed_url_ptr = 0;
    char *lang_ptr = 0;
    uint32 len = 0;

    if(CheckIfUrlCanChange(src_url_ptr))
    {
        //the src url is gwt url
        if(0==BRW_Strncmp((const uint8*)src_url_ptr, (const uint8*)GOOGLE_URL, strlen(GOOGLE_URL)))
        {
            len = BRW_Strlen((const uint8*)src_url_ptr);
            changed_url_ptr = BRWMem_Alloc(len+1);
            if(changed_url_ptr!=0)
            {
                BRW_Memset(changed_url_ptr,0,len+1);
                BRW_Strcpy((uint8*)changed_url_ptr, (const uint8*)src_url_ptr);
            }
            return changed_url_ptr;
        }
        if(lang==BRW_LANG_CH)
        {
            lang_ptr = GOOGLE_LANG_CH;
        }
        else
        {
            lang_ptr = GOOGLE_LANG_EN;
        }
#ifndef MMIDORADO_WRE_TRANSCODE_SUPPORT
        len = BRW_Strlen((const uint8*)src_url_ptr)+BRW_Strlen((const uint8*)GOOGLE_URL)+BRW_Strlen((const uint8*)GOOGLE_QUERY)
                +BRW_Strlen((const uint8*)URL_AND)+BRW_Strlen((const uint8*)MBROWSER_ID)+BRW_Strlen((const uint8*)URL_AND)
                +BRW_Strlen((const uint8*)FISRT_PAGE_ID)+BRW_Strlen((const uint8*)URL_AND)+BRW_Strlen((const uint8*)lang_ptr);
#else//for sprd wre transcoding
        len = BRW_Strlen((const uint8*)src_url_ptr)+BRW_Strlen((const uint8*)GOOGLE_URL)+BRW_Strlen((const uint8*)GOOGLE_QUERY);
#endif

        changed_url_ptr = BRWMem_Alloc(len+1);
        if(changed_url_ptr!=0)
        {
            BRW_Memset(changed_url_ptr,0,len+1);
            BRW_Strcpy((uint8*)changed_url_ptr,(const uint8*)GOOGLE_URL);
            BRW_Strcat((uint8*)changed_url_ptr,(const uint8*)GOOGLE_QUERY);
            BRW_Strcat((uint8*)changed_url_ptr,(const uint8*)src_url_ptr);
#ifndef MMIDORADO_WRE_TRANSCODE_SUPPORT
            BRW_Strcat((uint8*)changed_url_ptr,(const uint8*)URL_AND);
            BRW_Strcat((uint8*)changed_url_ptr,(const uint8*)MBROWSER_ID);
            BRW_Strcat((uint8*)changed_url_ptr,(const uint8*)URL_AND);
            BRW_Strcat((uint8*)changed_url_ptr,(const uint8*)FISRT_PAGE_ID);
            BRW_Strcat((uint8*)changed_url_ptr,(const uint8*)URL_AND);
            BRW_Strcat((uint8*)changed_url_ptr,(const uint8*)lang_ptr);
#endif			
        }
    }
    return changed_url_ptr;
}

PUBLIC BOOLEAN BrwGwtIsUsingGwt(char *url_ptr)
{
	if(0==url_ptr)
	{
		return FALSE;
	}
	else
	{
		return (0==BRW_Strncmp((const uint8*)url_ptr, (const uint8*)GOOGLE_URL, strlen(GOOGLE_URL)));
	}
}

#ifdef MMIDORADO_WRE_TRANSCODE_SUPPORT
/*****************************************************************************/
// Description : move to wre's transcoding string end.
// Global resource dependence :
// Author: fen.xie
// Note: only for sprd wre transcoding     
/*****************************************************************************/
PUBLIC uint8* BrwGwtMovetoGwtEnd(uint8 *absolute_url_ptr)
{
    uint8 *end_ptr = absolute_url_ptr;
    const uint8 *temp_ptr = PNULL;

    if (absolute_url_ptr != PNULL)
    {
        temp_ptr = BRW_CaseStrstr(end_ptr, (const uint8 *)GOOGLE_URL);
        if (temp_ptr == end_ptr)
        {
            end_ptr += BRW_Strlen((const uint8 *)GOOGLE_URL);
            temp_ptr = BRW_CaseStrstr(end_ptr, (const uint8 *)GOOGLE_QUERY);
            if (temp_ptr == end_ptr)
            {
                end_ptr += BRW_Strlen((const uint8 *)GOOGLE_QUERY);
                return end_ptr;
            }
        }

    }

    return absolute_url_ptr;
}
#endif
#endif
