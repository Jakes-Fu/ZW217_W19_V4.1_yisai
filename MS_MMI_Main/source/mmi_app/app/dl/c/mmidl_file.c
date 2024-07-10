/******************************************************************************
** File Name:      mmidl_file.c                                    *
** Author:                                                                    *
** Date:           07/28/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe file operation of the download *
**                 Browser                                                    *
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 07/2009        wancan.you        Create                                    *
******************************************************************************/
#define _MMIDL_FILE_C_  

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#include "mmi_app_dl_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "gui_ucs2b_converter.h"
#include "mmifmm_export.h"
#include "mmisd_export.h"
#include "mmidl_internal.h"
#include "mime_type.h"
#include "mmidl_nv.h"
#include <ctype.h>
#include "mmicom_string.h"

#ifdef DL_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef CMCC_UI_STYLE
#define MMIDL_DIR  ""
#define MMIDL_DIR_LEN  0//in the unit of uint8
#else
#ifdef MMIDL_USE_SUBFOLDER
#define MMIDL_DIR  (g_mmidl_dir_str)
#define MMIDL_DIR_LEN  9//in the unit of uint8
#else
#define MMIDL_DIR  ""
#define MMIDL_DIR_LEN  0
#endif /* MMIDL_USE_SUBFOLDER */

#endif

#define MMIDL_DEFAULT_AUDIO_DIR (g_mmidl_default_audio_dir_str)
#define MMIDL_DEFAULT_AUDIO_DIR_LEN  5//in the unit of uint8

#define MMIDL_DEFAULT_VIDEO_DIR  (g_mmidl_default_video_dir_str)
#define MMIDL_DEFAULT_VIDEO_DIR_LEN  5//in the unit of uint8

#define MMIDL_DEFAULT_PICTURE_DIR  (g_mmidl_default_picture_dir_str)
#define MMIDL_DEFAULT_PICTURE_DIR_LEN  7//in the unit of uint8

#define MMIDL_DEFAULT_TEXT_DIR  (g_mmidl_default_text_dir_str)
#define MMIDL_DEFAULT_TEXT_DIR_LEN  4//in the unit of uint8

#define MMIDL_DEFAULT_OTHER_DIR  (g_mmidl_default_other_dir_str)
#define MMIDL_DEFAULT_OTHER_DIR_LEN  5//in the unit of uint8

#define MMIDL_UNKNOWN_EXTNAME   "err"

#define MMIDL_RANDOM_FILENAME_LEN    50

#define MMIDL_HTTP_HEADER_CONTENT_TYPE "Content-Type: "
#define MMIDL_HTTP_HEADER_CONTENT_DISPOSITION "Content-Disposition: "
#define MMIDL_HTTP_HEADER_FILENAME_SCHEME "filename=\""
#define MMIDL_HTTP_HEADER_FILENAME_SCHEME2 "filename="
#define MMIDL_HTTP_HEADER_CHARSET "charset="
#define MMIDL_HTTP_CRLF_STRING "\r\n"
#define MMIDL_HTTP_COLON "\""

#define isHexDigit(c)  (((c)>='0'&&(c)<='9')||((c)>='a'&&(c)<='f')||((c)>='A'&&(c)<='F'))

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
const wchar g_mmidl_dir_str[] = { 'W', 'D', 'o', 'w', 'n', 'l', 'o', 'a', 'd', 0 };//wdownload
const wchar g_mmidl_default_audio_dir_str[] = { 'A', 'u', 'd', 'i', 'o', 0 };//audio
const wchar g_mmidl_default_video_dir_str[] = { 'V', 'i', 'd', 'e', 'o', 0 };//video
const wchar g_mmidl_default_picture_dir_str[] = { 'P', 'i', 'c', 't', 'u', 'r', 'e', 0 };//picture
const wchar g_mmidl_default_text_dir_str[] = { 'T', 'e', 'x', 't', 0 };//text
const wchar g_mmidl_default_other_dir_str[] = { 'O', 't', 'h', 'e', 'r', 0 };//other

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get device name
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL wchar *DownloadFile_GetDeviceName(uint16 *device_name_len);

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DownloadFile_CombineFullPath(
                                                                     wchar *file_name,
                                                                     uint16 file_name_len,
                                                                     wchar      *full_path_ptr,     //out
                                                                     uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                                                     );

/*****************************************************************************/
//  Description : Get Default file name by URL
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL uint8* GetDownloadFileByURL(const uint8 * url_ptr);

/*****************************************************************************/
// Description : Gen Random File Name
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
LOCAL void GenRandomFileName(wchar *file_name, uint32 name_len, uint32 mime_type, uint8 *default_file_name_ptr, BOOLEAN is_nf_type, MMIDL_CHARSET_T default_charset);

/*****************************************************************************/
// Description : GetFileNameByContentDispositon
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileNameByContentDispositon(wchar *file_name, uint32 max_file_name_len, const uint8 *http_header_ptr);

/*****************************************************************************/
//  Description : char to integer
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL uint8 ctoi(uint8 c);

LOCAL uint8 CharToHex(char chr);
LOCAL char* StrToLower(char* str);
LOCAL int DecodeToUTF8(unsigned char* pDst, const char* pSrc, int nSrcLen);
LOCAL int DecodeToGB2312(unsigned char* pDst, const char* pSrc, int nSrcLen);

LOCAL int MMIDL_ParseFileNameToWstr(wchar* file_name/*out*/,
                                        uint16 file_name_len/*in*/,
                                        uint8* original_file_name/*in*/,
                                        uint16 original_file_name_len/*in*/,
                                        MMIDL_CHARSET_T file_name_charset);

LOCAL MMIDL_CHARSET_T GetCharsetByhttpHeader(const uint8 * http_header_ptr);
/**--------------------------------------------------------------------------*
 **                         Constant Variables                               *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get device name
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL wchar *DownloadFile_GetDeviceName(uint16 *device_name_len)
{
    MMIFILE_DEVICE_E dev  = MMIAPIFMM_GetFirstValidDevice();
    wchar           *path = PNULL;
    
    *device_name_len = 0;
    if(dev < MMI_DEVICE_NUM)
    {
           path  = MMIAPIFMM_GetDevicePath(dev);
           *device_name_len  = MMIAPIFMM_GetDevicePathLen(dev);
    }
    return path;
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DownloadFile_CombineFullPath(
                                                                     wchar *file_name,
                                                                     uint16 file_name_len,
                                                                     wchar      *full_path_ptr,     //out
                                                                     uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                                                     )
{
    wchar* device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    BOOLEAN ret = TRUE;

    if (PNULL == file_name || PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        return FALSE;
    }

    device_name_ptr = DownloadFile_GetDeviceName(&device_name_len);

    if (PNULL == device_name_ptr || 0 == device_name_len)
    {
        return FALSE;
    }

    if (!MMIAPIFMM_CombineFullPath(device_name_ptr, device_name_len,
                                                (wchar*)MMIDL_DIR, MMIDL_DIR_LEN,
                                                file_name, file_name_len,
                                                full_path_ptr, full_path_len_ptr))
    {
        ret = FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetDefaultDownloadPath(
                                                                                     MMIDL_DOWNLOAD_TYPE type,
                                                                                     wchar      *full_path_ptr,     //out
                                                                                     uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                                                                     )
{
    wchar* device_name_ptr = PNULL;
    uint16 device_name_len = 0;
    const wchar *storage_dir_ptr = PNULL;
    uint16 storage_dir_len = 0;
    BOOLEAN ret = TRUE;

    if (PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        return FALSE;
    }

    device_name_ptr = DownloadFile_GetDeviceName(&device_name_len);

    if (PNULL == device_name_ptr || 0 == device_name_len)
    {
        return FALSE;
    }

    switch (type)
    {
    case MMIDL_DOWNLOAD_AUDIO_TYPE:
        storage_dir_ptr = MMIDL_DEFAULT_AUDIO_DIR;
        storage_dir_len = MMIDL_DEFAULT_AUDIO_DIR_LEN;
        break;

    case MMIDL_DOWNLOAD_VIDEO_TYPE:
        storage_dir_ptr = MMIDL_DEFAULT_VIDEO_DIR;
        storage_dir_len = MMIDL_DEFAULT_VIDEO_DIR_LEN;
        break;

    case MMIDL_DOWNLOAD_PICTURE_TYPE:
        storage_dir_ptr = MMIDL_DEFAULT_PICTURE_DIR;
        storage_dir_len = MMIDL_DEFAULT_PICTURE_DIR_LEN;
        break;

    case MMIDL_DOWNLOAD_TEXT_TYPE: /* go throgh*/
/*
        storage_dir_ptr = MMIDL_DEFAULT_TEXT_DIR;
        storage_dir_len = MMIDL_DEFAULT_TEXT_DIR_LEN;
        break;
*/
    case MMIDL_DOWNLOAD_OTHER_TYPE:
        storage_dir_ptr = MMIDL_DEFAULT_OTHER_DIR;
        storage_dir_len = MMIDL_DEFAULT_OTHER_DIR_LEN;
        break;

    default:
        storage_dir_ptr = PNULL;
        storage_dir_len = 0;
        break;
    }

    if (!MMIAPIFMM_CombineFullPath(device_name_ptr, device_name_len,
                                                (wchar*)MMIDL_DIR, MMIDL_DIR_LEN,
                                                storage_dir_ptr, storage_dir_len,
                                                full_path_ptr, full_path_len_ptr))
    {
        ret = FALSE;
    }

    return ret;
}

/*****************************************************************************/
//  Description : split filename
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDL_SplitFileName(
                                                                                     wchar      *full_file_name,
                                                                                     wchar      *file_name,
                                                                                     uint16     file_name_maxlen,
                                                                                     wchar      *file_ext,
                                                                                     uint16     file_ext_maxlen
                                                                                     )
{
    int32 full_file_len = 0;
    uint16 file_ext_len = 0;

    if (PNULL == full_file_name || PNULL == file_name || PNULL == file_ext)
    {
        return;
    }

    full_file_len = (int32)(MMIAPICOM_Wstrlen((const wchar *)full_file_name) - 1);

    for (;full_file_len >= 0; full_file_len--)
    {
        file_ext_len ++;
        if(MMIFILE_DOT == full_file_name[full_file_len])
        {
            MMIAPICOM_Wstrncpy(file_ext, full_file_name + full_file_len, MIN(file_ext_maxlen, file_ext_len));
            MMIAPICOM_Wstrncpy(file_name, full_file_name, MIN(full_file_len, file_name_maxlen));
            return;
        }
    }
    full_file_len = MMIAPICOM_Wstrlen((const wchar *)full_file_name);
    MMIAPICOM_Wstrncpy(file_name, full_file_name, MIN(full_file_len, file_name_maxlen));
}

/*****************************************************************************/
//  Description : combine full path
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_CombineDownloadFile(
                                                                                     MMI_STRING_T *path_ptr,
                                                                                     MMI_STRING_T *name_ptr,
                                                                                     wchar      *full_path_ptr,     //out
                                                                                     uint16     *full_path_len_ptr,  //in/out, 双字节为单位
                                                                                     BOOLEAN is_combine_tmp_file
                                                                                     )
{
    wchar dir_mark[] = {'\\', 0};
    uint16 dir_mark_len = 1;
    uint16 tmp_ext_len = 0;

    if (PNULL == path_ptr || PNULL == name_ptr || PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIDL_CombineDownloadFilename_ptr->wstr_len = %d,  path_ptr->wstr_len = %d,is_combine_tmp_file=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_349_112_2_18_2_13_28_119,(uint8*)"ddd", name_ptr->wstr_len, path_ptr->wstr_len,is_combine_tmp_file);

    if (is_combine_tmp_file)
    {
        tmp_ext_len = strlen(MMIDL_TMPFILE_EXT);
    }

    if (((name_ptr->wstr_len + path_ptr->wstr_len + dir_mark_len + tmp_ext_len)) < MMIFILE_FULL_PATH_MAX_LEN)
    {
        MMI_WSTRNCPY(full_path_ptr, 
                                MMIFILE_FULL_PATH_MAX_LEN,
                                path_ptr->wstr_ptr,
                                path_ptr->wstr_len,
                                path_ptr->wstr_len);

        *full_path_len_ptr = path_ptr->wstr_len;

        MMI_WSTRNCPY(full_path_ptr + *full_path_len_ptr, 
                                MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
                                dir_mark,
                                dir_mark_len,
                                dir_mark_len);

        *full_path_len_ptr += dir_mark_len;

        MMI_WSTRNCPY(full_path_ptr + *full_path_len_ptr, 
                                MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
                                name_ptr->wstr_ptr,
                                name_ptr->wstr_len,
                                name_ptr->wstr_len);

        *full_path_len_ptr += name_ptr->wstr_len;

        if (is_combine_tmp_file)
        {
            MMI_STRNTOWSTR(full_path_ptr + *full_path_len_ptr, 
                                    MMIFILE_FULL_PATH_MAX_LEN - *full_path_len_ptr,
                                    (uint8 *)MMIDL_TMPFILE_EXT,
                                    tmp_ext_len,
                                    tmp_ext_len);

            *full_path_len_ptr += tmp_ext_len;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : Get file type by content
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MIME_TYPE_E MMIDL_GetDownloadFileType(const char * content_type_ptr)
{
    MIME_TYPE_E mime_type = MIME_TYPE_UNKNOWN;

    if (PNULL == content_type_ptr)
    {
        return mime_type;
    }

    mime_type = (MIME_TYPE_E)mime_to_uint(content_type_ptr);

    return mime_type;
}

/*****************************************************************************/
//  Description : char to integer
//  Global resource dependence : 
//  Author:wancan.you
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
//  Description : Get Default file name by URL
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL uint8* GetDownloadFileByURL(const uint8 * url_ptr)
{
    uint32 url_len = 0;
    uint8* rtn_url_ptr = PNULL;
    char* rtn_url_ext = PNULL;
    const char *invalid_ext[] = {"asp", "php", "jsp", "aspx"};
    const char invalid_char[] = {'&'};

    if (PNULL != url_ptr)
    {
        int32  i = 0;
        uint32 j = 0;
        uint8*  temp = PNULL;
        uint8*  query_str_ptr = PNULL;
        uint8*  fragment_ptr = PNULL;
        uint8* tail_ptr = PNULL;
        uint32 len = 0;

        url_len = strlen((char *)url_ptr);

        for (i = url_len - 1; i >= 0; i--)
        {
            temp = (uint8 *)(url_ptr + i);

            if ('/' == *temp)
            {
                if (i > 0 && ('/' == *(temp - 1)))
                {
                    return PNULL;
                }

                break;
            }
        }

        if (i < 0 || i == (int32)(url_len - 1))
        {
            return PNULL;
        }

        i++;//'/'

        temp = (uint8 *)(url_ptr + i);
        query_str_ptr = (uint8 *)strchr((char *)temp, '?');
        fragment_ptr = (uint8 *)strchr((char *)temp, '#');

        if (PNULL != query_str_ptr)
        {
            if (PNULL != fragment_ptr)
            {
                tail_ptr = (query_str_ptr > fragment_ptr) ? fragment_ptr : query_str_ptr;
            }
            else
            {
                tail_ptr = query_str_ptr;
            }
        }
        else
        {
            tail_ptr = fragment_ptr;
        }
        
        if (PNULL != tail_ptr)
        {
            len = url_len - (uint32)i - strlen((char *)tail_ptr);
        }
        else
        {
            len = url_len - (uint32)i;
        }

        if (0 == len)
        {
            return PNULL;
        }

        rtn_url_ptr = SCI_ALLOCA(len + 1);

        if (PNULL == rtn_url_ptr)
        {
            return PNULL;
        }

        SCI_MEMSET(rtn_url_ptr, 0x00, (len + 1));

        j = 0;

        for (i = 0; i < (int)len; i++, j++)//计算转译字符
        {
            if (0x25 == *(temp + i) && (i + 2 < (int)len)
                && isHexDigit(*(temp + i + 1)) && isHexDigit(*(temp + i + 2))) // %
            {
                uint8 temp_conv_ascii = ctoi(*(temp + i + 1)) * 16 + ctoi(*(temp + i + 2));

                if (temp_conv_ascii < 0x80) //ASCII
                {
                    *(rtn_url_ptr + j) = (uint8)temp_conv_ascii;
                    i += 2;
                }
                else
                {
                    *(rtn_url_ptr + j) = *(temp + i);
                }
            }
            else
            {
                *(rtn_url_ptr + j) = *(temp + i);
            }
        }

        //SCI_MEMCPY(rtn_url_ptr, temp, len);

        *(rtn_url_ptr + len) = 0;

        for (i = len - 1; i > 0; i--)
        {
            if ('.' == *(rtn_url_ptr + i))
            {
                break;
            }
        }

        if (i > 0 && i < (int32)(len - 1))
        {
            i = i + 1;
        }

        rtn_url_ext = (char *)(rtn_url_ptr + i);

        //SCI_TRACE_LOW:"GetDownloadFileByURL i=%d, rtn_url_ext=%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_576_112_2_18_2_13_29_120,(uint8*)"ds", i, rtn_url_ext);

        if (0 == i)
        {
            SCI_FREE(rtn_url_ptr);
            return PNULL;
        }
        for (i = 0; i < (int32)(sizeof(invalid_ext) / sizeof(invalid_ext[0])); i++)
        {
            if (0 == strcmp(invalid_ext[i], rtn_url_ext))
            {
                SCI_FREE(rtn_url_ptr);
                return PNULL;
            }
        }
        for (i = 0; i < (int32)(sizeof(invalid_char) / sizeof(invalid_char[0])); i++)
        {
            if (PNULL != strchr(rtn_url_ext, invalid_char[i]))
            {
                SCI_FREE(rtn_url_ptr);
                return PNULL;
            }
        }
        return rtn_url_ptr;
    }

    return PNULL;    
}

/*****************************************************************************/
// Description : Gen Random File Name
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
LOCAL void GenRandomFileName(wchar *file_name, uint32 name_len, uint32 mime_type, uint8 *default_file_name_ptr, BOOLEAN is_nf_type, MMIDL_CHARSET_T default_charset)
{
    uint8 temp_buf[MMIDL_RANDOM_FILENAME_LEN] = {0};
    uint32 len=0;   
    const char *ext_name = PNULL;
    BOOLEAN is_random_name = FALSE;

    if (PNULL == file_name)
    {
        return;
    }

    if (is_nf_type)
    {
        ext_name = MMIAPIDL_NFMimeTypeToExt(mime_type);
    }
    else
    {
        ext_name = mime_to_ext(mime_type);
    }

    if (PNULL == ext_name || 0 == strlen(ext_name))
    {
        ext_name = (const char *)MMIDL_UNKNOWN_EXTNAME;
    }

    if (PNULL != default_file_name_ptr)
    {
        uint16 default_len = strlen((char *)default_file_name_ptr);
        uint16 i = 0;
        
        for (i = 0; i < default_len; i++)
        {
            if ('.' == *(default_file_name_ptr + i))
            {
                break;
            }
        }

        SCI_MEMCPY(temp_buf, default_file_name_ptr, MIN(MMIDL_RANDOM_FILENAME_LEN - 4, i));
        strcat((char *)temp_buf, ".");
        is_random_name = FALSE;
    }
    else
    {
        SCI_DATE_T  cur_date = {0};
        SCI_TIME_T  cur_time = {0};

        TM_GetSysDate(&cur_date);
        TM_GetSysTime(&cur_time);

        _snprintf((char*)temp_buf, MMIDL_RANDOM_FILENAME_LEN,"%04d%02d%02d%s%02d%02d%02d%s", cur_date.year, cur_date.mon, cur_date.mday, "_", cur_time.hour, cur_time.min, cur_time.sec, ".");
        is_random_name = TRUE;
    }
    strcat((char *)temp_buf, ext_name);

    len = (uint16)strlen((char *)temp_buf);

    if (len > name_len)
    {
        len = name_len - 1;
    }

    if(is_random_name)
    {
        MMI_STRNTOWSTR(file_name,
                      (uint16)name_len,
                      (const uint8 *)temp_buf,
                      len,
                      MIN(len, name_len));
    }
    else
	{
        MMIDL_ParseFileNameToWstr(file_name,
                                  (uint16)name_len,
                                  temp_buf,
                                  sizeof(temp_buf),
                                  default_charset);
	}
}

/*****************************************************************************/
// Description : GetMimeTypeByhttpHeader
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetMimeTypeByhttpHeader(const uint8 * http_header_ptr)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint8 * dest_str = PNULL;
    uint8 * dest_str3 = PNULL;
    uint32 dest_len = 0;
    uint32 mime_type = MIME_TYPE_UNKNOWN;

    if (PNULL == http_header_ptr)
    {
        return MIME_TYPE_UNKNOWN;
    }
    //SCI_TRACE_LOW:"GetMimeTypeByhttpHeader enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_706_112_2_18_2_13_29_121,(uint8*)"");

    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)http_header_ptr, MMIDL_HTTP_HEADER_CONTENT_TYPE);

    if (PNULL != dest_str1)
    {
        dest_str1 += strlen(MMIDL_HTTP_HEADER_CONTENT_TYPE);
        //SCI_TRACE_LOW:"GetMimeTypeByhttpHeader dest_str1=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_713_112_2_18_2_13_29_122,(uint8*)"d", dest_str1);

        dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, MMIDL_HTTP_CRLF_STRING);
        //SCI_TRACE_LOW:"GetMimeTypeByhttpHeader dest_str2=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_716_112_2_18_2_13_29_123,(uint8*)"d", dest_str2);
        
        if (PNULL != dest_str2)
        {
            //Content-Type: 中包含"charset="时的兼容性处理，
            //例如:Content-Type:audio/mpeg; charset=gb2312
            dest_str3 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, ";");
            //SCI_TRACE_LOW:"GetMimeTypeByhttpHeader dest_str3=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_723_112_2_18_2_13_29_124,(uint8*)"d", dest_str3);

            if(PNULL != dest_str3)
            {
                dest_str2 = (dest_str3 < dest_str2) ? dest_str3 : dest_str2;
            }
            
            dest_len = dest_str2 - dest_str1;

            //SCI_TRACE_LOW:"GetMimeTypeByhttpHeader dest_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_732_112_2_18_2_13_29_125,(uint8*)"d", dest_len);
            dest_str = SCI_ALLOCA(dest_len + 1);
            if (PNULL != dest_str)
            {
                SCI_MEMSET(dest_str, 0x00, (dest_len + 1));
                SCI_MEMCPY(dest_str, dest_str1, dest_len);
                //SCI_TRACE_LOW:"GetMimeTypeByhttpHeader dest_str=%s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_738_112_2_18_2_13_29_126,(uint8*)"s", dest_str);
                mime_type = mime_to_uint((char *)dest_str);
                SCI_FREE(dest_str);
            }
        }
    }

    return mime_type;
}

/*****************************************************************************/
// Description : GetFileNameByContentDispositon
// Global resource dependence :
// Author: Jiaoyou.wu
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileNameByContentDispositon(wchar *file_name, uint32 max_file_name_len, const uint8 *http_header_ptr)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint8 * dest_str = PNULL;
    uint32 dest_len = 0;

    if (PNULL == file_name || PNULL == http_header_ptr)
    {
        return FALSE;
    }

    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)http_header_ptr, MMIDL_HTTP_HEADER_CONTENT_DISPOSITION);

    if (PNULL != dest_str1)
    {
        dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)http_header_ptr, MMIDL_HTTP_HEADER_FILENAME_SCHEME);
        if (PNULL != dest_str1)
        {
            dest_str1 += strlen(MMIDL_HTTP_HEADER_FILENAME_SCHEME);
            dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, MMIDL_HTTP_CRLF_STRING);
            if (PNULL != dest_str2)
            {
                dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, MMIDL_HTTP_COLON);
                if (PNULL != dest_str2)
                {
                    dest_len = dest_str2 - dest_str1;
                    if (dest_len > 0)
                    {
                        dest_str = SCI_ALLOCA(dest_len + 1);
                        if (PNULL != dest_str)
                        {
                            SCI_MEMSET(dest_str, 0x00, (dest_len + 1));
                            SCI_MEMCPY(dest_str, dest_str1, dest_len);
                            GUI_UTF8ToWstr(file_name,
                                                        (uint16)max_file_name_len,
                                                        dest_str,
                                                        (uint16)MIN(max_file_name_len, dest_len));

                            SCI_FREE(dest_str);
                            return TRUE;
                        }
                    }
                }
            }
        }
        else
        {
            dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)http_header_ptr, MMIDL_HTTP_HEADER_FILENAME_SCHEME2);
            if (PNULL != dest_str1)
            {
                dest_str1 += strlen(MMIDL_HTTP_HEADER_FILENAME_SCHEME2);
                dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, MMIDL_HTTP_CRLF_STRING);
                if (PNULL != dest_str2)
                {
                    dest_len = dest_str2 - dest_str1;
                    if (dest_len > 0)
                    {
                        dest_str = SCI_ALLOCA(dest_len + 1);
                        if (PNULL != dest_str)
                        {
                            SCI_MEMSET(dest_str, 0x00, (dest_len + 1));
                            SCI_MEMCPY(dest_str, dest_str1, dest_len);
                            GUI_UTF8ToWstr(file_name,
                                                        (uint16)max_file_name_len,
                                                        dest_str,
                                                        (uint16)MIN(max_file_name_len, dest_len));

                            SCI_FREE(dest_str);
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    return FALSE;
}

/*****************************************************************************/
// Description : Get Default File Name
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetDefaultFileName(wchar *file_name, uint32 name_len, const uint8 * url_ptr, const uint8 * http_header_ptr, uint32 mime_type)
{
    BOOLEAN ret = FALSE;
    uint8 *default_file_name_ptr = PNULL;
    uint32 type = 0;
    MMIDL_CHARSET_T dl_charset = MMIDL_CHARSET_NONE;

    if (PNULL == file_name)
    {
        return FALSE;
    }

    ret = GetFileNameByContentDispositon(file_name, name_len, http_header_ptr);
    if (!ret)
    {
        default_file_name_ptr = GetDownloadFileByURL(url_ptr);

        type = MMIDL_GetMimeTypeByhttpHeader(http_header_ptr);

        dl_charset = GetCharsetByhttpHeader(http_header_ptr);
        //SCI_TRACE_LOW:"MMIDL_GetDefaultFileName type = %d,dl_charset=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_827_112_2_18_2_13_29_127,(uint8*)"dd", type, dl_charset);

        if (PNULL != default_file_name_ptr)
        {
            uint16 default_file_name_len = strlen((char *)default_file_name_ptr);
            MMIDL_ParseFileNameToWstr(file_name, 
                                      (uint16)name_len,
                                      default_file_name_ptr,
                                      (uint16)MIN(default_file_name_len, name_len),
                                      dl_charset);
        }
        else
        {
#ifdef BROWSER_SUPPORT_NETFRONT
            GenRandomFileName(file_name, name_len, mime_type, default_file_name_ptr, TRUE, dl_charset);
#else
            GenRandomFileName(file_name, name_len, mime_type, default_file_name_ptr, FALSE, dl_charset);
#endif
        }

        if (PNULL != default_file_name_ptr)
        {
            SCI_FREE(default_file_name_ptr);
            default_file_name_ptr = PNULL;
        }

        ret = TRUE;
    }
    return ret;
}

/*****************************************************************************/
//  Description : Get Download type
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC MMIDL_DOWNLOAD_TYPE MMIDL_GetDownloadType(uint32 mime_type, BOOLEAN is_nf_type)
{
    MMIDL_DOWNLOAD_TYPE download_type = MMIDL_DOWNLOAD_TYPE_MAX;

    //SCI_TRACE_LOW:"MMIDL_GetDownloadType mime_type=%d, is_nf_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_863_112_2_18_2_13_29_128,(uint8*)"dd", mime_type, is_nf_type);

    if (is_nf_type)
    {
        switch (mime_type)
        {
        case MIME_AUDIO_MIDI:
        case MIME_AUDIO_X_WAV:
        case MIME_AUDIO_BASIC:
        case MIME_AUDIO_MPEG:
        case MIME_AUDIO_X_MS_WMA:
        case MIME_EXTEND_AUDIO_MP3:
        case MIME_EXTEND_AUDIO_AAC:
            download_type = MMIDL_DOWNLOAD_AUDIO_TYPE;
            break;

        case MIME_VIDEO_X_MNG:
        case MIME_APPLICATION_X_SHOCKWAVE_FLASH:
        case MIME_APPLICATION_FUTURESPLASH:
        case MIME_VIDEO_MPEG:
        case MIME_VIDEO_X_MS_WMV:
        case MIME_VIDEO_X_MS_WM:
        case MIME_EXTEND_VIDEO_MP4:
            download_type = MMIDL_DOWNLOAD_VIDEO_TYPE;
            break;

        case MIME_IMAGE_GIF:
        case MIME_IMAGE_JPEG:
        case MIME_IMAGE_PNG:
        case MIME_IMAGE_BMP:
        case MIME_IMAGE_WBMP:
        case MIME_IMAGE_SVG:
        case MIME_IMAGE_X_ICON:
            download_type = MMIDL_DOWNLOAD_PICTURE_TYPE;
            break;

        case MIME_TEXT_HTML:
        case MIME_APPLICATION_XHTML:
        case MIME_TEXT_PLAIN:
        case MIME_TEXT_XML:
        case MIME_TEXT_WML:
        case MIME_APPLICATION_XHTMLMP:
            download_type = MMIDL_DOWNLOAD_TEXT_TYPE;
            break;

        default:
            download_type = MMIDL_DOWNLOAD_OTHER_TYPE;
            break;
        }    
    }
    else
    {
        switch (mime_type)
        {
        case MIME_TYPE_TEXT_ANY:
        case MIME_TYPE_TEXT_HTML:
        case MIME_TYPE_TEXT_PLAIN:
        case MIME_TYPE_TEXT_X_VCALENDAR:
        case MIME_TYPE_TEXT_X_VCARD:
        case MIME_TYPE_TEXT_XML:
        case MIME_TYPE_TEXT_WMLS:
        case MIME_TYPE_TEXT_WMLSC:
        case MIME_TYPE_TEXT_WMLC:
            download_type = MMIDL_DOWNLOAD_TEXT_TYPE;
            break;

        case MIME_TYPE_IMAGE_ANY:
        case MIME_TYPE_IMAGE_GIF:
        case MIME_TYPE_IMAGE_JPEG:
        case MIME_TYPE_IMAGE_TIFF:
        case MIME_TYPE_IMAGE_PNG:
        case MIME_TYPE_IMAGE_VND_WAP_WBMP:
        case MIME_TYPE_IMAGE_SVG:
        case MIME_TYPE_IMAGE_BMP:
        case MIME_TYPE_IMAGE_X_UP_WPNG:
        case MIME_TYPE_IMAGE_IEF:
            download_type = MMIDL_DOWNLOAD_PICTURE_TYPE;
            break;

        case MIME_TYPE_VIDEO_MPEG:
        case MIME_TYPE_VIDEO_3GPP:
        case MIME_TYPE_VIDEO_AVI:
        case MIME_TYPE_VIDEO_WMV:    
        case MIME_TYPE_VIDEO_MP4:
            download_type = MMIDL_DOWNLOAD_VIDEO_TYPE;
            break;

        case MIME_TYPE_AUDIO_ANY:
        case MIME_TYPE_AUDIO_MID:
        case MIME_TYPE_AUDIO_MPEG:
        case MIME_TYPE_AUDIO_X_WAV:
        case MIME_TYPE_AUDIO_AMR:
        case MIME_TYPE_AUDIO_MIDI:
        case MIME_TYPE_AUDIO_X_MIDI:
        case MIME_TYPE_AUDIO_SP_MIDI:
        case MIME_TYPE_AUDIO_IMELODY:
        case MIME_TYPE_AUDIO_MPA:
        case MIME_TYPE_AUDIO_MPA_ROBUST:
        case MIME_TYPE_AUDIO_MPA_LATM:
        case MIME_TYPE_AUDIO_TONE:
        case MIME_TYPE_AUDIO_RM:
        case MIME_TYPE_AUDIO_3GP:
            download_type = MMIDL_DOWNLOAD_AUDIO_TYPE;
            break;

        case MIME_TYPE_APPLICATION_ANY:
        case MIME_TYPE_APPLICATION_JAVA_VM:
        case MIME_TYPE_APPLICATION_JAVA_JAR:
        case MIME_TYPE_APPLICATION_JAVA_JAD:
        case MIME_TYPE_APPLICATION_VND_WAP_WBXML:
        case MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE:
        case MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT:
        case MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML:
        case MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML:
        case MIME_TYPE_APPLICATION_VND_SMAF:
        case MIME_TYPE_APPLICATION_X_SMAF:
        case MIME_TYPE_APPLICATION_SMIL:
        case MIME_TYPE_APPLICATION_X_SHOCKWAVE_FLASH:
        case MIME_TYPE_APPLICATION_ZIP:
        case MIME_TYPE_ANY_ANY:
        case MIME_TYPE_APPLICATION_QBT:
        case MIME_TYPE_APPLICATION_PNL:
        case MIME_TYPE_APPLICATION_NATC:
        default:
            download_type = MMIDL_DOWNLOAD_OTHER_TYPE;
            break;
        }
    }

    return download_type;
}

/*****************************************************************************/
//  Description : get download path
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetDownloadPath(
                                            MMIDL_DOWNLOAD_TYPE type,
                                            wchar      *full_path_ptr,     //out
                                            uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                            )
{
    MMIDL_DOWNLOAD_PATH_TYPE path_type = MMIDL_DOWNLOAD_PATH_DEFAULT;

    MMIDL_ReadDownloadPathType(&path_type);
    if (MMIDL_DOWNLOAD_PATH_CUSTOM == path_type)
    {
        return MMIDL_GetCustomizedDownloadPath(type, full_path_ptr, full_path_len_ptr);
    }
    else
    {
        return MMIDL_GetDefaultDownloadPath(type, full_path_ptr, full_path_len_ptr);
    }
}
/*****************************************************************************/
//  Description : get download path assigned by user
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_GetCustomizedDownloadPath(
                                            MMIDL_DOWNLOAD_TYPE type,
                                            wchar      *full_path_ptr,     //out
                                            uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                            )
{
    if (PNULL == full_path_ptr || PNULL == full_path_len_ptr)
    {
        return FALSE;
    }

    switch (type)
    {
    case MMIDL_DOWNLOAD_AUDIO_TYPE:
        MMIDL_ReadDownloadMusicPath(full_path_ptr);
        break;

    case MMIDL_DOWNLOAD_VIDEO_TYPE:
        MMIDL_ReadDownloadVideoPath(full_path_ptr);
        break;

    case MMIDL_DOWNLOAD_PICTURE_TYPE:
        MMIDL_ReadDownloadPicturePath(full_path_ptr);
        break;

    case MMIDL_DOWNLOAD_TEXT_TYPE:
    case MMIDL_DOWNLOAD_OTHER_TYPE:
    default:
        MMIDL_ReadDownloadOthersPath(full_path_ptr);
        break;
    }

    *full_path_len_ptr = MMIAPICOM_Wstrlen(full_path_ptr);

    //SCI_TRACE_LOW:"MMIDL_GetCustomizedDownloadPath len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_1042_112_2_18_2_13_30_129,(uint8*)"d", *full_path_len_ptr);

    return TRUE;
}

/*****************************************************************************/
//  Description : Init default download folder
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_InitDownloadFolder(void)
{
    wchar            full_path_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
    uint16           full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    const wchar	    *file_dev_ptr = PNULL;
    uint16	         file_dev_len = 0;
    MMIFILE_DEVICE_E dev    = MMI_DEVICE_UDISK;
    //BOOLEAN          dev_ok = FALSE;
    

    dev = MMIAPIFMM_GetFirstValidDevice();
    if(dev < MMI_DEVICE_NUM)
    {
        file_dev_ptr = MMIAPIFMM_GetDevicePath(dev);
        file_dev_len = MMIAPIFMM_GetDevicePathLen(dev);
    }
    else
    {
        return;
    }
    
    if (MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
                                                        (wchar*)MMIDL_DIR, MMIDL_DIR_LEN, 
                                                        NULL, 0, 
                                                        full_path_name, &full_path_len))
    {
        if (!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
    }

    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    SCI_MEMSET(full_path_name, 0x0, sizeof(full_path_name));
    if (MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
                                                        (wchar*)MMIDL_DIR, MMIDL_DIR_LEN, 
                                                        MMIDL_DEFAULT_PICTURE_DIR, MMIDL_DEFAULT_PICTURE_DIR_LEN, 
                                                        full_path_name, &full_path_len))
    {
        if (!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
    }

    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    SCI_MEMSET(full_path_name, 0x0, sizeof(full_path_name));
    if (MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
                                                        (wchar*)MMIDL_DIR, MMIDL_DIR_LEN, 
                                                        MMIDL_DEFAULT_AUDIO_DIR, MMIDL_DEFAULT_AUDIO_DIR_LEN, 
                                                        full_path_name, &full_path_len))
    {
        if (!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
    }

    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    SCI_MEMSET(full_path_name, 0x0, sizeof(full_path_name));
    if (MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
                                                        (wchar*)MMIDL_DIR, MMIDL_DIR_LEN, 
                                                        MMIDL_DEFAULT_VIDEO_DIR, MMIDL_DEFAULT_VIDEO_DIR_LEN, 
                                                        full_path_name, &full_path_len))
    {
        if (!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
    }

    full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    SCI_MEMSET(full_path_name, 0x0, sizeof(full_path_name));
    if (MMIAPIFMM_CombineFullPath(file_dev_ptr, file_dev_len,
                                                        (wchar*)MMIDL_DIR, MMIDL_DIR_LEN, 
                                                        MMIDL_DEFAULT_OTHER_DIR, MMIDL_DEFAULT_OTHER_DIR_LEN, 
                                                        full_path_name, &full_path_len))
    {
        if (!MMIAPIFMM_IsFolderExist(full_path_name, full_path_len))
        {
            MMIAPIFMM_CreateDir(full_path_name, full_path_len);
        }
    }
}

/*****************************************************************************/
//  Description : Create Download File
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIDL_CreateDownloadFile(MMIDL_DOWNLOAD_TASK_T *download_task_ptr, BOOLEAN *is_file_exist)
{
    MMIFILE_HANDLE file_handle = 0;
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};

    if (PNULL == is_file_exist)
    {
        return 0;
    }

    *is_file_exist = FALSE;

    if (PNULL == download_task_ptr)
    {
        return 0;
    }

    MMIDL_GetManagerTaskInfo(MMIDL_GetDownloadManager(), download_task_ptr->manager_index, &task_info);

    if (0 != MMIAPICOM_Wstrlen((const wchar *)task_info.file))
    {
        *is_file_exist = TRUE;

        file_handle = MMIAPIFMM_CreateFile((const wchar*)task_info.file, SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS | SFS_MODE_APPEND, NULL, NULL);/*lint !e655*/
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDL_CreateDownloadFile file not exist!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_1167_112_2_18_2_13_30_130,(uint8*)"");
    }

    return file_handle;
}

LOCAL uint8 CharToHex(char chr)
{
    uint8 hex = 0xff;
    if ( chr >= 'a' && chr <= 'f' )
    {
        hex =(uint8)(chr - 'a' + 10);
    }
    else if ( chr >= '0' && chr <= '9' )
    {
        hex = (uint8)(chr - '0');
    }
    else if ( chr >= 'A' && chr <= 'F' )
    {
        hex =(uint8)( chr - 'A' +10 );
    }

    return hex;
}

LOCAL char* StrToLower(char* str)
{
    int i = 0;
    for(i=0; str[i]!='\0'; i++)
    {
        str[i] = tolower(str[i]);
    }

    return str;
}
LOCAL int DecodeToUTF8(unsigned char* pDst, const char* pSrc, int nSrcLen)
{
    unsigned char TempChr1 = 0xff;
    unsigned char TempChr2 = 0xff;
    int nDstLen = 0;        // 输出的字符计数
    int per_symbol_num = 0; //编码中百分号的数目
    int i = 0;

    while (i < nSrcLen)
    {
        if (strncmp(pSrc, "%\r\n", 3) == 0)        // 软回车，跳过
        {
            pSrc += 3;
            i += 3;
        }
        else
        {
            if (*pSrc == '%')        // 是编码字节
            {
                per_symbol_num++;
                TempChr1 = CharToHex(*++pSrc);
                TempChr2 = CharToHex(*++pSrc);
                
                if (TempChr1 == 0xff || TempChr2 == 0xff)
                {
                    //容错处理，如果出现超长的字符(可能为中文，英文和数字的混合)，此时我们截下的长度内
                    //最后一个字符可能不是一个 完整的汉字，就会出现(TempChr1 == 0xff || TempChr2 == 0xff)的情况，           
                    //如果遇到这种情况，则直接结束解码过程；
                    //UTF-8编码中每个中文字符的编码包括三个"%"，例如"你"的编码是%E4%BD%A0; "你A1"的编码是%E4%BD%A0A1;
                    //"A1你"的编码是A1%E4%BD%A0;
                    //如果不做下列if处理,则最后一个字符可能是错误的字符;
                    //参考 cr223206 begin
                    if ((per_symbol_num % 3) == 0) 
                    {
                        //当第三个"%"出错时，前面两个"%"的处理无效，然后结束该解码过程；
                        nDstLen = nDstLen - 2;
                        pDst = pDst - 2;
                    }
                    else if ((per_symbol_num % 3) == 2)
                    {
                        //当第二个"%"出错时，前面一个"%"的处理无效，然后结束该解码过程；
                        nDstLen = nDstLen - 1;
                        pDst = pDst - 1;
                    }
                    else if ((per_symbol_num % 3) == 1)
                    {
                        //当第一个"%"出错时，直接结束该解码流程；
                    }
                     // 输出加个结束符
                    *pDst = '\0';
                    return nDstLen;  
                    //参考 cr223206 end         
                }
                else
                {
                    *pDst++ = (unsigned char)(TempChr1 << 4 | TempChr2);
                    pSrc++;
                }
                
                i += 3;
            }
            else        // 非编码字节
            {
                *pDst++ = (unsigned char)*pSrc++;
                i++;
            }

            nDstLen++;
        }
    }

    // 输出加个结束符
    *pDst = '\0';

    return nDstLen;
}
LOCAL int DecodeToGB2312(unsigned char* pDst, const char* pSrc, int nSrcLen)
{
    unsigned char TempChr1 = 0xff;
    unsigned char TempChr2 = 0xff;
    int nDstLen = 0;        // 输出的字符计数
    int per_symbol_num = 0; //编码中百分号的数目
    int i = 0;

    while (i < nSrcLen)
    {
        if (strncmp(pSrc, "%\r\n", 3) == 0)        // 软回车，跳过
        {
            pSrc += 2;
            i += 2;
        }
        else
        {
            if (*pSrc == '%')        // 是编码字节
            {
                per_symbol_num++;
                TempChr1 = CharToHex(*++pSrc);
                TempChr2 = CharToHex(*++pSrc);
                
                if (TempChr1 == 0xff || TempChr2 == 0xff)
                {
                    //容错处理，如果出现超长的字符(可能为中文，英文和数字的混合)，此时我们截下的长度内
                    //最后一个字符可能不是一个 完整的汉字，就会出现(TempChr1 == 0xff || TempChr2 == 0xff)的情况，           
                    //如果遇到这种情况，则直接结束解码过程；
                    //GB2312每个中文字符的编码包括两个"%"，例如"你"的编码是%C4%E3; "你A1"的编码是%C4%E3A1;
                    //"A1你"的编码是A1%C4%E3;
                    //如果不做下列if处理,则最后一个字符可能是错误的字符;
                    //参考 cr223206 begin
                    if ((per_symbol_num % 2) == 0)
                    {
                        //当第二个等号出错时，前面一个等号的处理无效，然后结束该解码过程；
                        nDstLen = nDstLen - 1;
                        pDst = pDst - 1;
                    }
                    else if ((per_symbol_num % 2) == 1)
                    {
                        //当第一个等号出错时，直接结束该解码流程；
                    }
                     // 输出加个结束符
                    *pDst = '\0';
                    return nDstLen;  
                    //参考 cr223206 end
                }
                else
                {
                    *pDst++ = (unsigned char)(TempChr1 << 4 | TempChr2);
                    pSrc++;
                }
                
                i += 2;
            }
            else        // 非编码字节
            {
                *pDst++ = (unsigned char)*pSrc++;
                i++;
            }

            nDstLen++;
        }
    }

    // 输出加个结束符
    *pDst = '\0';

    return nDstLen;
}

LOCAL int MMIDL_ParseFileNameToWstr(wchar* file_name/*out*/,
                                        uint16 file_name_len/*in*/,
                                        uint8* original_file_name/*in*/,
                                        uint16 original_file_name_len/*in*/,
                                        MMIDL_CHARSET_T file_name_charset)
{
    uint16 temp_buffer_len = original_file_name_len+1;
    uint8 *temp_buffer_ptr = PNULL;
    uint16 temp_str_len = 0;

    if(PNULL == file_name || PNULL == original_file_name)
    {
        return -1;
    }

    temp_buffer_ptr = SCI_ALLOCA(temp_buffer_len);
    if(PNULL == temp_buffer_ptr)
    {
        return -1;
    }
    SCI_MEMSET(temp_buffer_ptr, 0, temp_buffer_len);

    if(MMIDL_CHARSET_GB2312 == file_name_charset)
    {
        temp_str_len = DecodeToGB2312(temp_buffer_ptr, (const char*)original_file_name, original_file_name_len);

        //SCI_MEMCPY((void*)file_name, (void*)temp_buffer_ptr, MIN(temp_str_len, file_name_len));
        GUI_GBToWstr(file_name, temp_buffer_ptr, MIN(temp_str_len, file_name_len));
        
    }
    else
    {
        /*convert file's name from network to utf-8*/
        temp_str_len = DecodeToUTF8(temp_buffer_ptr, (const char*)original_file_name, original_file_name_len);
    
        /*utf8 ----> wstr*/
        GUI_UTF8ToWstr(file_name, (uint16)file_name_len, temp_buffer_ptr, MIN(temp_str_len, file_name_len));
    }

    SCI_FREE(temp_buffer_ptr);

    return temp_str_len;
}

LOCAL MMIDL_CHARSET_T GetCharsetByhttpHeader(const uint8 * http_header_ptr)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint8 * content_type_ptr = PNULL;
    uint32 content_type_len = 0;
    MMIDL_CHARSET_T dl_charset = MMIDL_CHARSET_NONE;

    if (PNULL == http_header_ptr)
    {
        return MMIDL_CHARSET_NONE;
    }
    //SCI_TRACE_LOW:"GetCharsetByhttpHeader enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_1405_112_2_18_2_13_31_131,(uint8*)"");

    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)http_header_ptr, MMIDL_HTTP_HEADER_CONTENT_TYPE);
    if(PNULL == dest_str1)
    {
        return MMIDL_CHARSET_NONE;
    }

    dest_str1 += strlen(MMIDL_HTTP_HEADER_CONTENT_TYPE);

    dest_str2 = (uint8 *)MMIAPICOM_Stristr((char *)dest_str1, MMIDL_HTTP_CRLF_STRING);
    if(PNULL == dest_str2)
    {
        return MMIDL_CHARSET_NONE;
    }

    content_type_len = dest_str2 - dest_str1;
    //SCI_TRACE_LOW:"GetCharsetByhttpHeader content_type_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_1422_112_2_18_2_13_31_132,(uint8*)"d", content_type_len);
    
    content_type_ptr = (uint8 *)SCI_ALLOCA(content_type_len + 1);
    if(PNULL == content_type_ptr)
    {
        return MMIDL_CHARSET_NONE;
    }

    SCI_MEMSET((void *)content_type_ptr, 0x00, content_type_len + 1);
    SCI_MEMCPY((void *)content_type_ptr, dest_str1, content_type_len);
    StrToLower((char *)content_type_ptr);
    //SCI_TRACE_LOW:"GetCharsetByhttpHeader content_type_ptr=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_1433_112_2_18_2_13_31_133,(uint8*)"s", content_type_ptr);

    dest_str1 = PNULL;
    dest_str2 = PNULL;

    /*search "charset=" from content_type*/
    dest_str1 = (uint8 *)MMIAPICOM_Stristr((char *)content_type_ptr, MMIDL_HTTP_HEADER_CHARSET);

    if (PNULL != dest_str1) //include "charset="
    {
        dest_str1 += strlen(MMIDL_HTTP_HEADER_CHARSET);
        //SCI_TRACE_LOW:"GetCharsetByhttpHeader dest_str1=%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_FILE_1444_112_2_18_2_13_31_134,(uint8*)"s", dest_str1);

        //dest_str2 = (uint8 *)MMIAPICOM_Stristr(dest_str1, "\0");
        if(strncmp((char*)dest_str1, "gb2312", 6) == 0)
        {
            dl_charset = MMIDL_CHARSET_GB2312;
        }
        else if(strncmp((char*)dest_str1, "utf-8", 5) == 0)
        {
            dl_charset = MMIDL_CHARSET_UTF8;
        }
        else
        {
            dl_charset = MMIDL_CHARSET_UTF8;
        }
    }
    else //uninclude "charset="
    {
        /*default charset is UTF-8*/
        dl_charset = MMIDL_CHARSET_UTF8;
    }

    SCI_FREE(content_type_ptr);

    return dl_charset;
}
#endif //#ifdef DL_SUPPORT
