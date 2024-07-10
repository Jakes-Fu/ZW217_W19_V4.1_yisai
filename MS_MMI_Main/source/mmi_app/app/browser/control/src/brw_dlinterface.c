/*****************************************************************
 **  File Name: 	brw_dlinterface.c							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:implemention of download interface              *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  14/01/2009		yuzhi.chen      Create. 					 *
*****************************************************************/
#include "mmi_app_browser_trc.h"
#include "brw_utility.h"
#include "brw_control_api.h"
#include "brw_control.h"
#include "brw_dlinterface.h"
#include "odm_api.h"
#include "gui_ucs2b_converter.h"
#include "mmk_app.h"
#include "mmifmm_export.h"
#ifdef OMADL_SUPPORT
#include "in_message.h"
#endif
#include "mmibrowser_wintable.h"

#define DL_TEMP_FILE_NAME_LEN 10
#define DL_TEMP_FILE_FULL_NAME_LEN 14
#define DL_TEMP_FILE_SUFFIX		".tmp"

//fen.xie add for MS00193526 begin
/*down load部分的文件名部分的实现，基本都参考了mmidl模块的*/
#define DL_RANDOM_FILENAME_LEN  50
#define DL_UNKNOWN_EXTNAME   "err"

#define DL_HTTP_HEADER_CONTENT_TYPE "Content-Type: "
#define DL_HTTP_HEADER_CONTENT_DISPOSITION "Content-Disposition: "
#define DL_HTTP_HEADER_FILENAME_SCHEME "filename=\""
#define DL_HTTP_HEADER_FILENAME_SCHEME2 "filename="
#define DL_HTTP_CRLF_STRING "\r\n"
#define DL_HTTP_COLON "\""
#ifdef DRM_SUPPORT
#define DL_HTTP_HEADER_SD_EXPIRED_TIME "X-Oma-Drm-Separate-Delivery:"
#endif

//download
//完整文件名的最大长度("*.*"不包括路径)
#define BRW_DL_FILE_FULL_PATH_MAX_LEN   MMIBROWSER_DL_FILE_FULL_PATH_MAX_LEN
//文件名后缀的最大长度
#define BRW_DL_SUFFIX_MAX_LEN   MMIBROWSER_DL_SUFFIX_MAX_LEN
#define BRW_DL_EXT_NAME_MAX_LEN (1 + BRW_DL_SUFFIX_MAX_LEN)//".*"
//文件名字的最大长度
#define BRW_DL_FILE_NAME_MAX_LEN (BRW_DL_FILE_FULL_PATH_MAX_LEN - BRW_DL_EXT_NAME_MAX_LEN)

typedef struct _BRW_DL_MIME_INFO_T_
{
  BRW_MIME_TYPE_E type;
  const char* ext_ptr;
}BRW_DL_MIME_INFO_T;

typedef struct _BRW_DL_MIME_TYPE_T_
{
  BRW_MIME_TYPE_E brw_type;
  MIME_TYPE_E type;
}BRW_DL_MIME_TYPE_T;


LOCAL const BRW_DL_MIME_INFO_T s_dl_mime_type[] = 
{
    //{BRW_MIME_UNKOWN, "err"},

    {BRW_MIME_HTML,  "html"},
    {BRW_MIME_WML,  "wml"},
    {BRW_MIME_WMLS,  "wmls"},
    {BRW_MIME_WMLSC, "wmlsc"},
    {BRW_MIME_WMLC, "wmlc"},
    {BRW_MIME_PLAINTEXT, "txt"},
    {BRW_MIME_CSS, "css"},
    /*-------------image-----------------*/
    {BRW_MIME_IMG_BMP, "bmp"},
    {BRW_MIME_IMG_IEF, "ief"},
    {BRW_MIME_IMG_GIF, "gif"},
    {BRW_MIME_IMG_JPEG, "jpg"},
    {BRW_MIME_IMG_PNG, "png"},
    {BRW_MIME_IMG_TIF,"tif"},
    {BRW_MIME_IMG_WBMP,"wbmp"},
    {BRW_MIME_IMG_PCX,"pcx"},
    //{BRW_MIME_IMG_UNKNOWN, "err"},
    /*-------------video------------------*/
    {BRW_MIME_VIDEO_3GP,"3gp"},
    {BRW_MIME_VIDEO_MP4, "mp4"},
    {BRW_MIME_VIDEO_MPEG, "mp4"},
    {BRW_MIME_VIDEO_WMV, "wmv"},

    //{BRW_MIME_VIDEO_UNKNOWN, "err"},
    /*-------------audio-----------------*/
    {BRW_MIME_AUDIO_AMR,"amr"},
    {BRW_MIME_AUDIO_MIDI,"mid"},
    {BRW_MIME_AUDIO_MID,"mid"},
    {BRW_MIME_AUDIO_MPA, "mpa"},
    {BRW_MIME_AUDIO_MPA_ROBUST, "mpa"},
    {BRW_MIME_AUDIO_MPA_LATM,    "mpa"},
    {BRW_MIME_AUDIO_MPEG, "mp3"},
    {BRW_MIME_AUDIO_TONE, "tone"},
    {BRW_MIME_AUDIO_RM,"rm"},
    {BRW_MIME_AUDIO_WAV,"wav"},
    {BRW_MIME_AUDIO_3GP,"3gp"},
#ifdef AAC_SUPPORT
    {BRW_MIME_AUDIO_X_AAC,"aac"},
#endif    

    {BRW_MIME_AUDIO_WMA,"wma"},
    //{BRW_MIME_AUDIO_UNKNOWN,"err"},
    /*-------------oma --------------------*/
#ifdef OMADL_SUPPORT
    {BRW_MIME_OMA_DD,"dd"},
#endif    
#ifdef DRM_SUPPORT    
    {BRW_MIME_OMA_DRM_MESSAGE,"dm"},
    {BRW_MIME_OMA_DRM_CONTENT,"dcf"   },
    {BRW_MIME_OMA_DRM_RIGHTS_XML,"dr"    },
    {BRW_MIME_OMA_DRM_RIGHTS_WBXML,"drc"   },
#endif    

	/*-------------java --------------------*/
    {BRW_MIME_JAD,"jad"},
    {BRW_MIME_JAR,"jar"},
    {BRW_MIME_SDP,"sdp"}
#ifdef QBTHEME_SUPPORT
	,
	{BRW_MIME_QBT, "qbt"}
#endif
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
	,
	{BRW_MIME_PNL,"pnl"}
#endif
#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
	,
	{BRW_MIME_NTAC,"ntac"}
#endif

};
//fen.xie add for MS00193526 end
LOCAL const BRW_DL_MIME_TYPE_T s_mime_type_table[] = 
{
    {BRW_MIME_UNKOWN, MIME_TYPE_UNKNOWN},
    {BRW_MIME_HTML,  MIME_TYPE_TEXT_HTML},
    {BRW_MIME_WML,  MIME_TYPE_TEXT_ANY},
    {BRW_MIME_WMLS,  MIME_TYPE_TEXT_ANY},
    {BRW_MIME_WMLSC, MIME_TYPE_APPLICATION_ANY},
    {BRW_MIME_WMLC, MIME_TYPE_APPLICATION_ANY},
    {BRW_MIME_PLAINTEXT, MIME_TYPE_TEXT_PLAIN},
    {BRW_MIME_CSS, MIME_TYPE_TEXT_ANY},

    /*-------------image-----------------*/
    {BRW_MIME_IMG_BMP, MIME_TYPE_IMAGE_BMP},
    {BRW_MIME_IMG_IEF, MIME_TYPE_IMAGE_ANY},
    {BRW_MIME_IMG_GIF, MIME_TYPE_IMAGE_GIF},
    {BRW_MIME_IMG_JPEG, MIME_TYPE_IMAGE_JPEG},
    {BRW_MIME_IMG_PNG, MIME_TYPE_IMAGE_PNG},
    {BRW_MIME_IMG_TIF,MIME_TYPE_IMAGE_TIFF},
    {BRW_MIME_IMG_WBMP,MIME_TYPE_IMAGE_VND_WAP_WBMP},
    {BRW_MIME_IMG_PCX,MIME_TYPE_IMAGE_ANY},

    /*-------------video------------------*/
    {BRW_MIME_VIDEO_3GP,MIME_TYPE_VIDEO_3GPP},
    {BRW_MIME_VIDEO_MP4,MIME_TYPE_VIDEO_MP4},
    {BRW_MIME_VIDEO_MPEG, MIME_TYPE_VIDEO_MPEG},
    {BRW_MIME_VIDEO_WMV, MIME_TYPE_VIDEO_WMV},

    /*-------------audio-----------------*/
    {BRW_MIME_AUDIO_AMR,MIME_TYPE_AUDIO_AMR},
    {BRW_MIME_AUDIO_MIDI,MIME_TYPE_AUDIO_MIDI},
    {BRW_MIME_AUDIO_MID,MIME_TYPE_AUDIO_MID},
    {BRW_MIME_AUDIO_MPA, MIME_TYPE_AUDIO_ANY},
    {BRW_MIME_AUDIO_MPA_ROBUST, MIME_TYPE_AUDIO_ANY},
    {BRW_MIME_AUDIO_MPA_LATM,    MIME_TYPE_AUDIO_ANY},
    {BRW_MIME_AUDIO_MPEG, MIME_TYPE_AUDIO_MPEG},
    {BRW_MIME_AUDIO_TONE, MIME_TYPE_AUDIO_ANY},
    {BRW_MIME_AUDIO_RM, MIME_TYPE_AUDIO_ANY},
    {BRW_MIME_AUDIO_WAV, MIME_TYPE_AUDIO_X_WAV},
    {BRW_MIME_AUDIO_3GP, MIME_TYPE_AUDIO_ANY},
#ifdef AAC_SUPPORT
    {BRW_MIME_AUDIO_X_AAC, MIME_TYPE_AUDIO_X_AAC},
#endif
    {BRW_MIME_AUDIO_WMA, MIME_TYPE_AUDIO_WMA},
    /*-------------oma --------------------*/
    {BRW_MIME_OMA_DD, MIME_TYPE_APPLICATION_VND_OMA_DD},
    {BRW_MIME_OMA_DRM_MESSAGE, MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE},
    {BRW_MIME_OMA_DRM_CONTENT, MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT},
    {BRW_MIME_OMA_DRM_RIGHTS_XML, MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML},
    {BRW_MIME_OMA_DRM_RIGHTS_WBXML, MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML},

	/*-------------java --------------------*/
    {BRW_MIME_JAD, MIME_TYPE_APPLICATION_JAVA_JAD},
    {BRW_MIME_JAR, MIME_TYPE_APPLICATION_JAVA_JAR},
    {BRW_MIME_SDP, MIME_TYPE_APPLICATION_SDP}

};

#ifdef BROWSER_GZIP_SUPPORT
/*for cancel the download by user(when http is using the file), the dl temp need to delete later*/
LOCAL uint16    *s_dl_temp_file_name_ptr = PNULL;//下载临时文件名
#endif

LOCAL BOOLEAN BrwGenerateDlTempName(uint16 *name_full_path_ptr);

extern int stricmp (const char *a, const char *b);
#ifdef OMADL_SUPPORT
extern BRW_INSTANCE_LIST_STRUCT g_brw_instance_list_info;
#endif

#ifdef DRM_SUPPORT
/*****************************************************************************/
// Description : GetDrmSDExpiredTime
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL uint32 GetDrmSDExpiredTime(const uint8 *http_header_ptr);
#endif

//fen.xie add for MS00193526 begin
#define isHexDigit(c)  (((c)>='0'&&(c)<='9')||((c)>='a'&&(c)<='f')||((c)>='A'&&(c)<='F'))

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
// Description: get extension of a special mime type(BRW_MIME_TYPE_E)
// Global resource dependence: 
// Author: fenxie
// Note: 
/*****************************************************************************/
PUBLIC const char *BrwMimeToExt( uint32 mime_value )
{
    uint32 i = 0;

    for( i = 0; i < (sizeof(s_dl_mime_type)/sizeof(BRW_DL_MIME_INFO_T)); i++) 
    {
        if (s_dl_mime_type[i].type == mime_value)
        {
            return s_dl_mime_type[i].ext_ptr;
        }
    }

    return PNULL;
}

/*****************************************************************************/
// Description : Gen File Name for dl(if null by random, and combined the suffix by mime_type)
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN DLGenFileName(wchar *file_name_ptr, 
                            uint32 name_len, 
                            uint32 mime_type, 
                            wchar *default_file_wname_ptr,
                            BOOLEAN is_use_default_file_suffix//whether use the default file's suffix,not by mime type
                            )
{
    uint8 temp_buf[DL_RANDOM_FILENAME_LEN+ 1] = {0};
    uint32 random = 0;
    uint32 time = 0;
    const char *suffix_name_ptr = PNULL;
    uint16 default_len = 0;
    wchar  wchar_value = 0;
    uint16 ext_name_len=0;   
    uint16 suffix_name_len=0;   
    uint16 file_name_max_len=0;   
    uint16 flash_wchar[2] = {MMIFILE_DOT,0x00};
    BOOLEAN is_exit_suffix = FALSE;
    BOOLEAN is_without_file_name = FALSE;
    int32 ext_begin_pos=0; //".***"  
    int32 i = 0;

    if (PNULL == file_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]W DLGenFileName ::PNULL != file_name"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_285_112_2_18_2_7_10_581,(uint8*)"");
        return FALSE;
    }
    
    //get the suffix name
    suffix_name_ptr = BrwMimeToExt(mime_type);
    if (PNULL == suffix_name_ptr)
    {
        suffix_name_ptr = (const char *)DL_UNKNOWN_EXTNAME;
    }
    suffix_name_len = BRW_Strlen((const uint8 *)suffix_name_ptr);

    if (suffix_name_len > BRW_DL_SUFFIX_MAX_LEN)
    {
        //BRWDebug_Printf:"[BRW]W DLGenFileName:(ext_name_len=%d) > (BRW_DL_SUFFIX_MAX_LEN=%d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_299_112_2_18_2_7_10_582,(uint8*)"dd", suffix_name_len, BRW_DL_SUFFIX_MAX_LEN);
        return FALSE;
    }

    if (name_len <= (suffix_name_len + 1))
    {
        //BRWDebug_Printf:"[BRW]W DLGenFileName:(ext_name_len=%d),(name_len=%d)"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_305_112_2_18_2_7_10_583,(uint8*)"dd", suffix_name_len, name_len);
        return FALSE;
    }

    if (name_len > BRW_DL_FILE_FULL_PATH_MAX_LEN)
    {
        //BRWDebug_Printf:"[BRW]DLGenFileName:name_len=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_311_112_2_18_2_7_10_584,(uint8*)"d", name_len);
        file_name_max_len = BRW_DL_FILE_FULL_PATH_MAX_LEN - suffix_name_len -1;//means "filename" without the "."
    }
    else
    {
        file_name_max_len = name_len - suffix_name_len -1;//means "filename" without the "."
    }

    if ((PNULL != default_file_wname_ptr) 
        && (default_len = BRW_WStrlen(default_file_wname_ptr)) > 0)
    {

        //find DOT in file_name_ptr
        for (i = default_len - 1; i >= 0; i--)
        {
            wchar_value = *(default_file_wname_ptr + i);
    		
            if(MMIFILE_DOT == wchar_value)
            {
                is_exit_suffix = TRUE;
                break;
            }
        }
        
        ext_begin_pos = i;
        
        if (is_exit_suffix)
        {
            if (ext_begin_pos > 0)
            {
                MMIAPICOM_Wstrncpy(file_name_ptr, default_file_wname_ptr, MIN(ext_begin_pos, file_name_max_len));
            }
            else//for examplt ".txt"
            {
                is_without_file_name = TRUE;
            }
            
            if (is_use_default_file_suffix)//use the default file suffix, not by the mime type
            {
                ext_name_len = default_len - ext_begin_pos;
                MMIAPICOM_Wstrncat(file_name_ptr, (default_file_wname_ptr + ext_begin_pos), MIN(ext_name_len, BRW_DL_EXT_NAME_MAX_LEN));
                return TRUE;
            }
        }
        else//the file name without DOT
        {
            MMIAPICOM_Wstrncpy(file_name_ptr, default_file_wname_ptr, MIN(default_len, file_name_max_len));       
        }
    }
    else
    {
        is_without_file_name = TRUE;
    }

    if (is_without_file_name)//get random filename
    {
        time = SCI_GetTickCount();
        srand(time);
        random = (uint32)rand();
        sprintf((char *)temp_buf, "%ld_%ld", random, time);

        BRWPL_WstrStrnCatWithAsciiStr(file_name_ptr, (const char *)temp_buf, MIN(file_name_max_len, (DL_RANDOM_FILENAME_LEN - suffix_name_len - 1)));
    }
    
    //combined the suffix by mime type
    MMIAPICOM_Wstrncat(file_name_ptr, flash_wchar, 1);
    BRWPL_WstrStrnCatWithAsciiStr(file_name_ptr, suffix_name_ptr, name_len);

    return TRUE;
}

/*****************************************************************************/
//  Description : Get Default file name by URL
//  Global resource dependence : 
//  Author:wancan.you
//  Note: 
/*****************************************************************************/
LOCAL uint8* GetDownloadFileByURL(const uint8 * url_ptr, BOOLEAN is_can_without_slash)
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

        if (i == (int32)(url_len - 1))
        {
            return PNULL;
        }

        if ((i < 0) && !is_can_without_slash)//the url without_slash
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
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_515_112_2_18_2_7_11_585,(uint8*)"ds", i, rtn_url_ext);

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
// Description : GetFileNameByContentDispositon
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetFileNameByContentDispositon(wchar *file_name_ptr, uint32 max_file_name_len, const uint8 *http_header_ptr)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint8 * dest_str = PNULL;
    uint32 dest_len = 0;

    if ((PNULL == file_name_ptr) || (PNULL == http_header_ptr))
    {
        return FALSE;
    }

    dest_str1 = (uint8 *)strstr((char *)http_header_ptr, DL_HTTP_HEADER_CONTENT_DISPOSITION);

    if (PNULL != dest_str1)
    {
        dest_str1 = (uint8 *)strstr((char *)http_header_ptr, DL_HTTP_HEADER_FILENAME_SCHEME);
        if (PNULL != dest_str1)
        {
            dest_str1 += strlen(DL_HTTP_HEADER_FILENAME_SCHEME);
            dest_str2 = (uint8 *)strstr((char *)dest_str1, DL_HTTP_CRLF_STRING);
            if (PNULL != dest_str2)
            {
                dest_str2 = (uint8 *)strstr((char *)dest_str1, DL_HTTP_COLON);
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
                            GUI_UTF8ToWstr(file_name_ptr,
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
            dest_str1 = (uint8 *)strstr((char *)http_header_ptr, DL_HTTP_HEADER_FILENAME_SCHEME2);
            if (PNULL != dest_str1)
            {
                dest_str1 += strlen(DL_HTTP_HEADER_FILENAME_SCHEME2);
                dest_str2 = (uint8 *)strstr((char *)dest_str1, DL_HTTP_CRLF_STRING);
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
                            GUI_UTF8ToWstr(file_name_ptr,
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

#if 0
/*****************************************************************************/
// Description : GetMimeTypeByhttpHeader
// Global resource dependence :
// Author: wancan.you
// Note: 
/*****************************************************************************/
LOCAL uint32 GetMimeTypeByhttpHeader(const uint8 * http_header_ptr)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint8 * dest_str = PNULL;
    uint32 dest_len = 0;
    uint32 mime_type = BRW_MIME_UNKOWN;

    if (PNULL == http_header_ptr)
    {
        return BRW_MIME_UNKOWN;
    }

    dest_str1 = (uint8 *)strstr((char *)http_header_ptr, DL_HTTP_HEADER_CONTENT_TYPE);

    if (PNULL != dest_str1)
    {
        dest_str1 += strlen(DL_HTTP_HEADER_CONTENT_TYPE);

        dest_str2 = (uint8 *)strstr((char *)dest_str1, DL_HTTP_CRLF_STRING);

        if (PNULL != dest_str2)
        {
            dest_len = dest_str2 - dest_str1;

            dest_str = SCI_ALLOCA(dest_len + 1);
            if (PNULL != dest_str)
            {
                SCI_MEMSET(dest_str, 0x00, (dest_len + 1));
                SCI_MEMCPY(dest_str, dest_str1, dest_len);
                mime_type = BRW_GetMineType(dest_str);
                SCI_FREE(dest_str);
            }
        }
    }
    
    //BRWDebug_Printf:"[BRW]GetMimeTypeByhttpHeader mime_type =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_640_112_2_18_2_7_11_586,(uint8*)"d", mime_type);

    return mime_type;
}
#endif

/*****************************************************************************/
// Description : Get Default File Name
// Global resource dependence :
// Author: fen.xie
// Note:  copy from :MMIDL_GetDefaultFileName
/*****************************************************************************/
PUBLIC BOOLEAN Brw_DLGetDefaultFileName(wchar *file_name_ptr, 
                uint32 name_len, 
                const uint8 * url_ptr, 
                BOOLEAN is_url_can_without_slash,
                const uint8 * http_header_ptr, 
                uint32 mime_type)
{
    BOOLEAN ret = FALSE;
    uint32 type = mime_type;
    uint8 *url_file_name_ptr = PNULL;
    wchar   *url_file_wname_ptr  = PNULL;
    uint16 url_file_name_len = 0;

    if(PNULL == file_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]W Brw_DLGetDefaultFileName:PNULL == file_name"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_660_112_2_18_2_7_11_587,(uint8*)"");
        return FALSE;
    }

    if (GetFileNameByContentDispositon(file_name_ptr, name_len, http_header_ptr))
    {
        //BRWDebug_Printf:"[BRW]Brw_DLGetDefaultFileName GetFileNameByContentDispositon TURE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_666_112_2_18_2_7_11_588,(uint8*)"");
    }
    else
    {
        url_file_name_ptr = GetDownloadFileByURL(url_ptr, is_url_can_without_slash);

        //mime_type已经是 通过解析heater的content type得到的,故此处不需要再解析了 
        //type = GetMimeTypeByhttpHeader(http_header_ptr);
        //BRWDebug_Printf:"[BRW]Brw_DLGetDefaultFileName type =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_674_112_2_18_2_7_11_589,(uint8*)"d", type);
        
        if (PNULL != url_file_name_ptr)//convert to wstr
        {
            url_file_name_len = strlen((char *)url_file_name_ptr);
            if (url_file_name_len > 0)
            {
                url_file_wname_ptr = SCI_ALLOCA((url_file_name_len+1) * sizeof(wchar));
                if (PNULL != url_file_wname_ptr)
                {
                    SCI_MEMSET(url_file_wname_ptr, 0x00, (url_file_name_len+1) * sizeof(wchar));
                    GUI_UTF8ToWstr(url_file_wname_ptr,url_file_name_len,url_file_name_ptr,url_file_name_len);
                }
                else
                {
                    //BRWDebug_Printf:"[BRW]Brw_DLGetDefaultFileName default_file_wname_ptr ALLOC Fail"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_689_112_2_18_2_7_11_590,(uint8*)"");
                }
            }
            else
            {
                //BRWDebug_Printf:"[BRW]Brw_DLGetDefaultFileName url_file_name_len err"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_694_112_2_18_2_7_11_591,(uint8*)"");
            }

            SCI_FREE(url_file_name_ptr);
       }

        //get mime type,and it is valid 
        if (((BRW_MIME_HTML <= type) && (BRW_MIME_CSS >= type)) 
            || ((BRW_MIME_IMG_BMP <= type) && (BRW_MIME_IMG_UNKNOWN > type))
            || ((BRW_MIME_AUDIO_AMR <= type) && (BRW_MIME_AUDIO_UNKNOWN > type))
            || ((BRW_MIME_VIDEO_3GP <= type) && (BRW_MIME_VIDEO_UNKNOWN > type))
            || ((BRW_MIME_OMA_DD <= type) && (BRW_MIME_OMA_END > type))
            || ((BRW_MIME_JAD <= type) && (BRW_MIME_APP_END >= type))
            )
        {
            DLGenFileName(file_name_ptr, name_len, type, url_file_wname_ptr, FALSE);
            ret = TRUE;//表示get的file name是根据MIME TYPE的,外部无需再根据后缀名重现查找MIME type
        }
        else
        {
            DLGenFileName(file_name_ptr, name_len, mime_type, url_file_wname_ptr, TRUE);
        }

        if (PNULL != url_file_wname_ptr)
        {
            SCI_FREE(url_file_wname_ptr);
        }
    }
    return ret;
}
/*****************************************************************************/
// Description : Get mime type by file name's suffix
// Global resource dependence :
// Author: fen.xie
// Note:  copy from :BrwDLGetMimeTypeBySuffix
/*****************************************************************************/
LOCAL BRW_MIME_TYPE_E BrwDLGetMimeTypeBySuffix(wchar *file_name, uint32 file_name_len)
{
    BRW_MIME_TYPE_E mime_type = BRW_MIME_UNKOWN;
    uint16  suffix_len = DL_TEMP_FILE_FULL_NAME_LEN;
    wchar  suffix_name[DL_TEMP_FILE_FULL_NAME_LEN+1] = {0};
    uint8   suffix_name_char[DL_TEMP_FILE_FULL_NAME_LEN+1] = {0};
    uint32 i = 0;

    if ((PNULL != file_name) && (0 != file_name_len))
    {
        MMIAPIFMM_SplitFileName(file_name, file_name_len, PNULL, PNULL, suffix_name, &suffix_len);
        
        GUI_WstrToUTF8(suffix_name_char, DL_TEMP_FILE_FULL_NAME_LEN, suffix_name, suffix_len);

        for( i = 0; i < (sizeof(s_dl_mime_type)/sizeof(BRW_DL_MIME_INFO_T)); i++) 
        {
            if (stricmp((char *)s_dl_mime_type[i].ext_ptr, (char *)suffix_name_char ) == 0)
            {
                mime_type = s_dl_mime_type[i].type;
                break;
            }
        }
    }

    //BRWDebug_Printf:"[BRW]BrwDLGetMimeTypeBySuffix mime_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_752_112_2_18_2_7_11_592,(uint8*)"d", mime_type);

    return mime_type;
}


//fen.xie add for MS00193526 end


LOCAL BOOLEAN BrwGenerateDlTempName(uint16 *name_full_path_ptr)
{
    uint16 name_with_suffix[DL_TEMP_FILE_FULL_NAME_LEN+1] = {0};
    char  name_without_suffix[DL_TEMP_FILE_NAME_LEN+1] = {0};

    _snprintf(name_without_suffix,DL_TEMP_FILE_NAME_LEN,"%d",SCI_GetTickCount());
    BRWPL_WstrStrnCatWithAsciiStr(name_with_suffix, name_without_suffix,DL_TEMP_FILE_FULL_NAME_LEN+1);
    BRWPL_WstrStrnCatWithAsciiStr(name_with_suffix, DL_TEMP_FILE_SUFFIX,DL_TEMP_FILE_FULL_NAME_LEN+1);
    BRWUtil_GetBrwRootPath(name_full_path_ptr,BRW_FULL_PATH_MAX_LEN);
    if(BRW_WStrlen(name_full_path_ptr)+DL_TEMP_FILE_FULL_NAME_LEN+1 > BRW_FULL_PATH_MAX_LEN)
    {
        //BRWDebug_Printf:"BrwGenerateDlTempName length exceed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_772_112_2_18_2_7_11_593,(uint8*)"");
        return FALSE;
    }
    BRWPL_WstrStrnCatWithAsciiStr(name_full_path_ptr, BRW_FILE_SLASH, BRW_FULL_PATH_MAX_LEN+1);
    BRW_WstrCat(name_full_path_ptr,name_with_suffix);
    return TRUE;
}
/*
 *	desc:start a download request
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 *  return:TRUE if success,eles FALSE
 */
PUBLIC BOOLEAN BrwStartDL(BRW_INFO_T* brw_info_ptr, WAP_REQUEST_T* req_ptr, char *hear_ptr)
{        
    BRW_MIME_TYPE_E mime_type = BRW_MIME_UNKOWN;
#ifdef BROWSER_GZIP_SUPPORT
    BOOLEAN is_gzip = FALSE;
#endif
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();

    if((PNULL == req_ptr) || (PNULL == brw_info_ptr) || (PNULL == req_ptr->http_req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwStartDL:req_ptr=0x%x,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_794_112_2_18_2_7_11_594,(uint8*)"dd", req_ptr, brw_info_ptr);
        return FALSE;
    }
    
    mime_type = req_ptr->http_req_ptr->mime_type;
#ifdef BROWSER_GZIP_SUPPORT
    //BRWDebug_Printf:"[BRW]BrwStartDL,:transfer_encoding = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_800_112_2_18_2_7_11_595,(uint8*)"d", req_ptr->http_req_ptr->transfer_encoding);
    is_gzip = (BRW_HTTP_ENCODING_TYPE_GZIP == req_ptr->http_req_ptr->transfer_encoding)? TRUE: FALSE;
#endif

#ifndef OMADL_SUPPORT
    if (BRW_MIME_OMA_DD == mime_type)//不支持OMADL的下载
    {
        BrwProcErr(brw_info_ptr, BRW_ERR_PAGE_UNSUPPORT);
        return TRUE;
    }
#endif 
    
    if(!BRWList_AppendObject(&brw_info_ptr->dl_req_list, req_ptr))//add to download req list
    {
        BRWDebug_PrintWarning("[BRW]BrwStartDL:BRWList_AppendObject failed");
        BrwProcErr(brw_info_ptr, BRW_ERR_NO_MEM);
        return TRUE;
    }
    
    //update it to a download req 
    BrwSetWapReqState(req_ptr, BRW_STATE_DOWNLOADING);
    brw_info_ptr->cur_req_ptr = brw_info_ptr->cur_display_req_ptr;
    
    //creat temp file to save the download data
#ifndef BROWSER_GZIP_SUPPORT
    if (BRW_PROTOCOL_WAP == public_profile_ptr->protocol)//only use for wap,if it isn't supported with gzip
#else
    if ((BRW_PROTOCOL_WAP == public_profile_ptr->protocol) 
        || (is_gzip && (BRW_MIME_OMA_DD != mime_type)))
#endif        
    {
        uint16  *temp_file_path_ptr = PNULL;

        temp_file_path_ptr = BRWMem_Alloc(sizeof(uint16) * (BRW_FULL_PATH_MAX_LEN + 1));
        if (PNULL == temp_file_path_ptr)
        {
            BRWDebug_PrintWarning("[BRW]BrwStartDL alloc temp file name fail");
            BrwProcDownloadErr(brw_info_ptr, req_ptr, BRW_ERR_NO_MEM);
            return TRUE;
        }
        BRW_Memset(temp_file_path_ptr, 0, (sizeof(uint16)*(BRW_FULL_PATH_MAX_LEN+1)));
        BrwGenerateDlTempName(temp_file_path_ptr);
#ifdef BROWSER_GZIP_SUPPORT
        BrwSetDlTempFilename(temp_file_path_ptr, BRW_WStrlen(temp_file_path_ptr));
#endif

#ifdef BROWSER_GZIP_SUPPORT
        if (BRW_PROTOCOL_WAP == public_profile_ptr->protocol)
#endif        
        {
            req_ptr->ffs_handler = BRWPL_CreatFile(temp_file_path_ptr);
            if (0 == req_ptr->ffs_handler)
            {
                BRWMem_Free(temp_file_path_ptr);
                BRWDebug_PrintWarning("[BRW]BrwStartDL creat temp file handle fail");
                BrwProcDownloadErr(brw_info_ptr, req_ptr, BRW_ERR_UNKNOWN);
                return TRUE;
            }
        }

        req_ptr->wsp_dl_temp_file_name_ptr = temp_file_path_ptr;
    }

    //create BRW_MSG_ENTER_FILENM_REQ_T to UI
    {
        BRW_MSG_ENTER_FILENM_REQ_T req ={0};

        req.handle = req_ptr->req_id;
        req.content_len = req_ptr->http_req_ptr->http_content_len;
        
#ifdef DRM_SUPPORT
        if (BRW_MIME_OMA_DRM_CONTENT == mime_type)//DCF
        {
            req.expired_time = GetDrmSDExpiredTime((const uint8 *)hear_ptr);
            //BRWDebug_Printf:"[BRW]BrwStartDL:expired_time = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_874_112_2_18_2_7_11_596,(uint8*)"d", req.expired_time);
        }
#endif    

        //BRWDebug_Printf:"[BRW]BrwStartDL mime_type =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_878_112_2_18_2_7_11_597,(uint8*)"d", mime_type);

#ifdef OMADL_SUPPORT
        if (BRW_MIME_OMA_DD == mime_type)
        {
            //待DD使用OTA方式下载完毕后,再启动OMADL下载MO文件
            req_ptr->download_state = BRW_DL_STATE_OMA_DD;
            req.enter_filenm_cbf = BrwEnterDdFileNmCbf;
            req.dl_type = BRW_DL_TYPE_OMA_DD;
            
            //cancel the dd http request, use OMADL for DD file
            BrwCancelHttpRequest(req_ptr->http_req_ptr,
            			brw_info_ptr->http_context_id,
            			public_profile_ptr->protocol);
            BrwFreeHttpRequest(req_ptr->http_req_ptr);
            req_ptr->http_req_ptr = PNULL;
        }
        else
#endif        
        {
            req_ptr->download_state = BRW_DL_STATE_HTTP_CONTENT;
            req.enter_filenm_cbf = BrwEnterFileNmCbf;
            req.dl_type = BRW_DL_TYPE_HTTP_CONTENT;
        }
                
        if (Brw_DLGetDefaultFileName(req.res_name_arr, BRW_MAX_URL_LEN, req_ptr->absolute_url, FALSE, (const uint8 *)hear_ptr, mime_type))
        {
            req.mime_type = mime_type;
        }
        else
        {
            req.mime_type = BrwDLGetMimeTypeBySuffix(req.res_name_arr, MMIAPICOM_Wstrlen(req.res_name_arr));
            if (BRW_MIME_UNKOWN == req.mime_type)
            {
                switch (mime_type)//尽量确保各类型(不支持的)的文件放置再对应的文件夹中
                {
                case BRW_MIME_AUDIO_UNKNOWN:
                case BRW_MIME_IMG_UNKNOWN:
                case BRW_MIME_VIDEO_UNKNOWN:
                    req.mime_type = mime_type;
                    break;
                    
                default:
                    break;
                }
            }
        }
        
        //BRWDebug_Printf:"[BRW]BrwStartDL req.mime_type =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_926_112_2_18_2_7_11_598,(uint8*)"d", req.mime_type);

        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_ENTER_FILENM_REQ, &req, sizeof(BRW_MSG_ENTER_FILENM_REQ_T));
    }    
    return TRUE;
}
/*
 *	desc:cancel a download request
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 *  return:TRUE if success,eles FALSE
 */
PUBLIC BOOLEAN  BrwCancelDL(WAP_REQUEST_T* req_ptr,BRW_INFO_T* brw_info_ptr)
{
    if ((PNULL==req_ptr) || (PNULL==brw_info_ptr))
    {
        //BRWDebug_Printf:"[BRW]W--BrwCancelDL:req_ptr=0x%x,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_941_112_2_18_2_7_11_599,(uint8*)"dd",req_ptr,brw_info_ptr);
        return FALSE;
    }
    BrwFreeWapReqStruct(req_ptr);
    BRWList_RemoveObject(&brw_info_ptr->dl_req_list,req_ptr);
    return TRUE;
}
/*
 *	desc: free download relevant resource
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 */
PUBLIC  void BrwFreeDLResource(WAP_REQUEST_T* req_ptr,BRW_INFO_T* brw_info_ptr)
{
    if ((PNULL == req_ptr) || (PNULL == brw_info_ptr))
    {
        //BRWDebug_Printf:"[BRW]W--BrwFreeDLResource:req_ptr=0x%x,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_956_112_2_18_2_7_11_600,(uint8*)"dd",req_ptr,brw_info_ptr);
        return;
    }
    
    if (req_ptr->ffs_handler != 0)
    {
        BRWPL_CloseFile(req_ptr->ffs_handler);
        req_ptr->ffs_handler = 0;
    }

    if (BrwGetWapReqState(req_ptr) != BRW_STATE_DOWNLOADING)
    {
        //BRWDebug_Printf:"BrwFreeDLResource:isn't downloading state"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_968_112_2_18_2_7_11_601,(uint8*)"");
        return;
    }

    //取消OMA 内容的下载(媒体信息)
#ifdef OMADL_SUPPORT
    if (((BRW_DL_STATE_OMA_CONTENT == req_ptr->download_state)
            || (BRW_DL_STATE_OMA_DD == req_ptr->download_state))
        && (req_ptr->oma_dl_session_id != BRW_INVALID_OMA_DL_SESSION_ID))  
    {
        ODM_CancelDownload(req_ptr->oma_dl_session_id);
    }
#endif
}
/*
 *	desc:process download success case(http download ,not include the oma download)
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 */
PUBLIC void BrwProcDLSuccess(WAP_REQUEST_T* req_ptr,BRW_INFO_T* brw_info_ptr)
{
    if ((PNULL == req_ptr) || (PNULL == brw_info_ptr ))
    {
        //BRWDebug_Printf:"[BRW]W BrwProcDLSuccess:req_ptr=0x%x,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_990_112_2_18_2_7_11_602,(uint8*)"dd",req_ptr,brw_info_ptr);
        return;
    }
    
    if (brw_info_ptr->profile.msg_proc_func != PNULL)
    {
        BRW_MSG_DL_OK_IND_T ind={0};
        
        ind.handle = req_ptr->req_id;
        //告知调用者下载完成
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_DOWNLOAD_OK_IND,&ind,sizeof(ind));
    }		
    req_ptr->oma_dl_session_id = BRW_INVALID_OMA_DL_SESSION_ID;
    //释放资源
    BrwFreeWapReqStruct(req_ptr);
    //从下载链表中删除
    BRWList_RemoveObject(&brw_info_ptr->dl_req_list,req_ptr);
}

PUBLIC BOOLEAN BrwWspDownloadDataIn(
									WAP_REQUEST_T *wap_req_ptr,
									uint8 *buf_ptr,
									uint32 data_size,
									HTTP_REQ_INFO_T *http_req_ptr)
{
    BRWPL_FFS_HANDLE ffs_handler = 0;

    if ((PNULL == wap_req_ptr)|| (PNULL == http_req_ptr))
    {
        //BRWDebug_Printf:"[BRW]W BrwWspDownloadDataIn, invalid input,wap_req_ptr=%d,http_req_ptr=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1019_112_2_18_2_7_12_603,(uint8*)"dd",wap_req_ptr,http_req_ptr);
        return FALSE;
    }
    
    ffs_handler = wap_req_ptr->ffs_handler;
    if ((0 == ffs_handler) || (PNULL == buf_ptr) || (0 == data_size))
    {
        //BRWDebug_Printf:"[BRW]W BrwWspDownloadDataIn ERR:ffs_handler=%d,buf_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1026_112_2_18_2_7_12_604,(uint8*)"dd",ffs_handler, buf_ptr);
        return FALSE;
    }
        
    if(!BRWPL_AppendWriteFileSync(ffs_handler,buf_ptr,data_size,http_req_ptr->resp_buf_now_len))
    {
        BRWDebug_PrintWarning("BrwWspDownloadDataIn,write fail=0");
        return FALSE;
    }
    
    http_req_ptr->resp_buf_now_len += data_size;
    return TRUE;
}

#ifdef DRM_SUPPORT
/*****************************************************************************/
// Description : GetDrmSDExpiredTime ms
// Global resource dependence :
// Author: fen.xie
// Note: 
/*****************************************************************************/
LOCAL uint32 GetDrmSDExpiredTime(const uint8 *http_header_ptr)
{
    uint8 * dest_str1 = PNULL;
    uint8 * dest_str2 = PNULL;
    uint8 * dest_str = PNULL;
    uint32 dest_len = 0;
    uint32  expired_time = 0;

    if (PNULL == http_header_ptr)
    {
        //BRWDebug_Printf:"[BRW]W GetDrmSDExpiredTime http_header_ptr PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1055_112_2_18_2_7_12_605,(uint8*)"");
        return 0;
    }

    dest_str1 = (uint8 *)strstr((char *)http_header_ptr, DL_HTTP_HEADER_SD_EXPIRED_TIME);
    
    //BRWDebug_Printf:"[BRW]GetDrmSDExpiredTime dest_str1=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1061_112_2_18_2_7_12_606,(uint8*)"s", dest_str1);
    
    if (PNULL != dest_str1)
    {
        dest_len = strlen((char *)dest_str1) - strlen((char *)DL_HTTP_HEADER_SD_EXPIRED_TIME);
        dest_str2 = dest_str1 + strlen((char *)DL_HTTP_HEADER_SD_EXPIRED_TIME);
        
        if (dest_len > 0)
        {
            dest_str = SCI_ALLOCA(dest_len + 1);
            if (PNULL != dest_str)
            {
                SCI_MEMSET(dest_str, 0x00, (dest_len + 1));
                SCI_MEMCPY(dest_str, dest_str2, dest_len);
                expired_time = atoi((char *)dest_str);
                SCI_FREE(dest_str);
            }
            else
            {
                //BRWDebug_Printf:"[BRW]W GetDrmSDExpiredTime ALLOC PNULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1080_112_2_18_2_7_12_607,(uint8*)"");
            }
        }            
    }

    //BRWDebug_Printf:"[BRW]GetDrmSDExpiredTime expired_time=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1085_112_2_18_2_7_12_608,(uint8*)"d", expired_time);

    return (expired_time * 1000);//second->ms
}
#endif

#ifdef OMADL_SUPPORT
/*
 *	desc:start a oma download
 *  param:req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwStartOMADL(WAP_REQUEST_T* req_ptr,
            BRW_INFO_T* brw_info_ptr,
            const uint16* file_name_ptr,
            uint16 file_name_len)
{
    ODM_START_DOWNLOAD_PARAM_T oma_dl_req = {0};
    int32 oma_ret = ERR_ODM_FAILED;
    //BRW_CHARSET_E charset = BRW_CHARSET_UNKNOWN;
    BRW_PUBLIC_PROFILE_T    *public_profile_ptr = BrwGetPublicProfile();
    uint8 *buffer_ptr = PNULL;
    uint32  url_len = 0;
    uint32  dd_file_name_size = 0;
    
    do
    {
        if ((PNULL == req_ptr) || (PNULL == brw_info_ptr))
        {
            //BRWDebug_Printf:"[BRW]W BrwStartOMADL:req_ptr=0x%x,brw_info_ptr=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1114_112_2_18_2_7_12_609,(uint8*)"dd",req_ptr, brw_info_ptr);
            break;
        }
        
        if ((file_name_ptr != PNULL) && (0 != file_name_len))
        {
            dd_file_name_size = MIN(sizeof(oma_dl_req.dd_file_name), (file_name_len +1) * sizeof(uint16)) - sizeof(uint16);
            SCI_MEMCPY(oma_dl_req.dd_file_name, file_name_ptr, dd_file_name_size);
        }
        else
        {
           //BRWDebug_Printf:"[BRW]W MMIDL_StartOMADownload file_name_ptr PNULL or file_name_len =%d"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1125_112_2_18_2_7_12_610,(uint8*)"d", file_name_len);
           break;
        }

        url_len = BRW_Strlen(req_ptr->absolute_url) + 1;
        buffer_ptr = SCI_ALLOCA(url_len);/*该buffer供ODM DL使用,故需使用SCI来alloc*/
        if (PNULL == buffer_ptr)
        {
            BRWDebug_PrintWarning("MMIDL_StartOMADownload No mem");
            break;
        }
    
        SCI_MEMSET(buffer_ptr, 0x00, url_len);
        SCI_MEMCPY(buffer_ptr, req_ptr->absolute_url, url_len);
        
        oma_dl_req.content_encoding=ODM_ENCODING_UTF8;//目前ODM协议支持UTF8的
        
        switch (public_profile_ptr->protocol)
        {
        case BRW_PROTOCOL_HTTP:
            oma_dl_req.connection_type=ODM_CONNECTION_TYPE_HTTP;
            break;
        case BRW_PROTOCOL_WAP:
            oma_dl_req.connection_type=ODM_CONNECTION_TYPE_WSP;
            break;
        default:
            oma_dl_req.connection_type=ODM_CONNECTION_TYPE_MAX;
            break;
        }

        oma_dl_req.content = buffer_ptr;    
        oma_dl_req.length = url_len;
        oma_dl_req.is_dd_url = TRUE;
        oma_dl_req.magic = req_ptr->req_id;

        //set proxy info
		if(public_profile_ptr->proxy_opt.is_use_proxy)
		{
			oma_dl_req.proxy_info.is_use_proxy = TRUE;
			oma_dl_req.proxy_info.proxy_port = public_profile_ptr->proxy_opt.proxy_port;
			oma_dl_req.proxy_info.proxy_host_ptr = (char *)public_profile_ptr->proxy_opt.proxy_addr_ptr;
		} 

        oma_dl_req.net_id = g_brw_instance_list_info.net_id;
        oma_ret = ODM_StartDownload(&oma_dl_req);
        if(ODM_SUCCESS != oma_ret)
        {
            //BRWDebug_Printf:"[BRW]W MMIDL_StartOMADownload ODM_StartDownload Failed oma_ret = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1172_112_2_18_2_7_12_611,(uint8*)"d", oma_ret);
            break;
        }

        req_ptr->download_state = BRW_DL_STATE_OMA_DD;
    }
    while (0);

    if ((ODM_SUCCESS != oma_ret) && (PNULL != buffer_ptr))/*启动ODM DL失败,释放buffer;成功则该buffer由ODM DL控制*/
    {
        SCI_FREE(buffer_ptr);
    }

    return (BOOLEAN)(ODM_SUCCESS == oma_ret);
}

/*
 *	desc:oma download confirm response
 *  param:wap_req_ptr  wap request object pointer
 *        brw_info_ptr browser instance object pointer
 *        file_name_ptr unicode format file name pointer
 *        file_name_len length of file name
 *  return:TRUE if success,else FALSE
 */
PUBLIC BOOLEAN BrwOMADLResp(WAP_REQUEST_T* wap_req_ptr,BRW_INFO_T* brw_info_ptr,const uint16* file_name_ptr,uint16 file_name_len)
{
    ODM_DOWNLOAD_CONFIRMATION_PARAM_T param={0};
    
    if ((PNULL==wap_req_ptr) || (PNULL==brw_info_ptr) || (BRW_INVALID_OMA_DL_SESSION_ID==wap_req_ptr->oma_dl_session_id))
    {
        //BRWDebug_Printf:"[BRW]W--BrwOMADLResp:wap_req_ptr=0x%x,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1201_112_2_18_2_7_12_612,(uint8*)"dd",wap_req_ptr,brw_info_ptr);
        return FALSE;
    }
    
    param.session_id = wap_req_ptr->oma_dl_session_id;
    if ((PNULL != file_name_ptr) && (file_name_len > 0) && (file_name_len <= ODM_FILE_FULL_PATH_MAX_LEN))
    {
        param.is_user_allowed =TRUE;
        param.session_id = wap_req_ptr->oma_dl_session_id;
        param.file_name_len = file_name_len;
        BRW_Memcpy(param.file_name, (void*)file_name_ptr, file_name_len*sizeof(uint16));
    }
    else
    {
        param.is_user_allowed=FALSE;
    }

    ODM_DownloadConfirmationResponse(&param);
    return TRUE;
}

/*
 *	desc:发送OMA DL Auth resp消息进行鉴权
 *  参数：req_ptr  http请求指针
 *        
 */
PUBLIC void BrwOMADLAuthResp(uint32 wap_req_id, uint8 *user_name_ptr, uint8 *pwd_ptr, BOOLEAN result)
{
    WAP_REQUEST_T   *wap_req_ptr=PNULL;    
    BRW_INFO_T  *brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(wap_req_id);
    
    wap_req_ptr = BrwGetWapReq(wap_req_id, brw_info_ptr);
    if ((PNULL==wap_req_ptr) || (PNULL==brw_info_ptr) || (BRW_INVALID_OMA_DL_SESSION_ID == wap_req_ptr->oma_dl_session_id))
    {
        //BRWDebug_Printf:"[BRW]W BrwOMADLAuthResp:wap_req_ptr=0x%x,brw_info_ptr=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1234_112_2_18_2_7_12_613,(uint8*)"dd",wap_req_ptr,brw_info_ptr);
        return;
    }
    
    //BRWDebug_Printf:"[BRW]BrwOMADLAuthResp:result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1238_112_2_18_2_7_12_614,(uint8*)"d", result);

    if (result)
    {
        ODM_DOWNLOAD_AUTH_RSP_PARAM_T auth_param = {0};
        uint32  user_name_len = 0;
        uint32  pwd_len = 0;
        
        auth_param.session_id = wap_req_ptr->oma_dl_session_id;

        if (PNULL != user_name_ptr)
        {
            user_name_len = BRW_Strlen(user_name_ptr);
            user_name_len = MIN(user_name_len, ODM_USER_NAME_BUFFER_SIZE);
            BRW_Memcpy(auth_param.user_name, user_name_ptr, user_name_len);
        }
        
        if (PNULL != pwd_ptr)
        {
            pwd_len = BRW_Strlen(pwd_ptr);
            pwd_len = MIN(pwd_len, ODM_USER_NAME_BUFFER_SIZE);
            BRW_Memcpy(auth_param.password, pwd_ptr, pwd_len);
        }
        
        ODM_AuthResponse(&auth_param);
    }
    else
    {
        BrwProcDownloadErr(brw_info_ptr, wap_req_ptr, BRW_ERR_UNKNOWN);
    }    
}


/*****************************************************************************/
//  Description : Handle ODM_START_DOWNLOAD_CNF
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMStartDlCnf(void *param)
{
    BRW_INFO_T  *brw_info_ptr = PNULL;
    WAP_REQUEST_T   *wap_req_ptr=PNULL;
    ODM_START_DOWNLOAD_CNF_MSG_T *cnf_ptr = PNULL;

    if (PNULL == param)
    {
        return FALSE;
    }
 
    do
    {
        cnf_ptr = (ODM_START_DOWNLOAD_CNF_MSG_T*)param;
        
        //BRWDebug_Printf:"[BRW]BrwOMADLAuthResp:cnf_ptr->result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1290_112_2_18_2_7_12_615,(uint8*)"d", cnf_ptr->result);

        //根据magic得到brw info,并记录session id，从而做到一一对应
        brw_info_ptr = BrwGetBrwInfoPtrByWapReqID(cnf_ptr->magic);
        if (PNULL == brw_info_ptr)
        {
            break;//abort
        }
        
        wap_req_ptr = BrwGetWapReq(cnf_ptr->magic, brw_info_ptr);
        if (PNULL == wap_req_ptr)
        {
            break;//abort
        }

        if (ODM_SUCCESS == cnf_ptr->result)
        {
            wap_req_ptr->oma_dl_session_id = cnf_ptr->session_id;
        }
        else
        {
            wap_req_ptr->oma_dl_session_id = BRW_INVALID_OMA_DL_SESSION_ID;
            BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
        }

        return TRUE;
    }while(0);

    BRWDebug_PrintWarning("[BRW]BrwOMADLAuthResp abort");
    //handle the abort
    if (ODM_SUCCESS == cnf_ptr->result)
    {
        ODM_CancelDownload(cnf_ptr->session_id);
    }
    else
    {
        //do nothing
    }
    
    return TRUE;
    
}

/*****************************************************************************/
//  Description : Handle ODM_CANCEL_DOWNLOAD_CNF
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMCancelDlCnf(void *param)
{
    BOOLEAN result = FALSE;
    //ODM_CANCEL_DOWNLOAD_CNF_MSG_T *msg_data_ptr = PNULL;

    //do nothing

    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_CONFIRMATION_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDlConfirmationInd(void *param)
{
    BOOLEAN result = FALSE;
    BRW_INFO_T  *brw_info_ptr = PNULL;
    WAP_REQUEST_T* wap_req_ptr=PNULL;
    ODM_DOWNLOAD_CONFIRMATION_IND_MSG_T *ind_ptr = PNULL;
    BRW_MSG_OMADL_CONFIRM_REQ_T req={0};  
    BRW_MIME_TYPE_E mime_type[ODM_DD_TYPE_MAX_NUM] = {0}; //BRW_MIME_UNKOWN=0, 
    uint32  temp_size = 0;
    uint32  i=0;
#ifdef DRM_SUPPORT    
    BOOLEAN is_download_drm = FALSE;
    uint32  drm_mime_type_item = 0;
#endif

    if (PNULL == param)
    {
        return FALSE;
    }

    do
    {
        ind_ptr = (ODM_DOWNLOAD_CONFIRMATION_IND_MSG_T*)param;

        brw_info_ptr = BrwGetBrwInfoPtrByDLSessionID(ind_ptr->session_id);
        if (PNULL == brw_info_ptr)
        {
            break;//Abort
        }
        
        wap_req_ptr = BrwGetWapReqByDLSessionID(brw_info_ptr, ind_ptr->session_id);
        if (PNULL == wap_req_ptr)
        {
            break;//Abort
        }

        result = TRUE;
        wap_req_ptr->download_state = BRW_DL_STATE_OMA_CONTENT;

        req.handle = wap_req_ptr->req_id;
        req.size = ind_ptr->basic_dd_content.size;
#ifdef DRM_SUPPORT    
        req.expired_time = wap_req_ptr->drm_sd_time_out;
#endif

        /*#define ODM_DD_TYPE_MAX_NUM 2,协议中的type有可能有两个(DRM and Media type);
            或一个Media type*/
        for (i = 0; i < ODM_DD_TYPE_MAX_NUM; i++)
        {
            if (BRW_Strlen((uint8 *)(ind_ptr->basic_dd_content.type[i])) > 0)
            {
                mime_type[i] = BRW_GetMineType((uint8 *)(ind_ptr->basic_dd_content.type[i]));
                //BRWDebug_Printf:"[BRW] HandleODMDlConfirmationInd mime_type[%d] =%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1404_112_2_18_2_7_12_616,(uint8*)"dd", i, mime_type[i]);
#ifdef DRM_SUPPORT    
                if ((BRW_MIME_OMA_DRM_MESSAGE == mime_type[i]) || (BRW_MIME_OMA_DRM_CONTENT == mime_type[i]))
                {
                    is_download_drm = TRUE;
                    req.mime_type = mime_type[i];
                    drm_mime_type_item = i;
                }
#endif                    
            }
        }

#ifdef DRM_SUPPORT    
        if (is_download_drm)
        {
            for (i = 0; i < ODM_DD_TYPE_MAX_NUM; i++)
            {
                if (i != drm_mime_type_item)
                {
                    req.drm_mo_mime_type = mime_type[i];
                    break;
                }
            }
        }
        else
#endif            
        {
            /*to do:协议上有提到DD中可能有多个mime type属性,需要判断device是否都支持,并告知用户*/
            for (i = 0; i < ODM_DD_TYPE_MAX_NUM; i++)
            {
                if (BRW_MIME_UNKOWN != mime_type[i])
                {
                    req.mime_type = mime_type[i];
                    temp_size = BRW_Strlen((uint8 *)(ind_ptr->basic_dd_content.type[i]));
                    temp_size = MIN((sizeof(req.type) - 1), temp_size); 
                    BRW_Memcpy(req.type, ind_ptr->basic_dd_content.type[i], temp_size);
                    break;
                }
            }
        }

        //BRWDebug_Printf:"[BRW] HandleODMDlConfirmationInd mime_type =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1445_112_2_18_2_7_12_617,(uint8*)"d", req.mime_type);
#ifdef DRM_SUPPORT    
        //BRWDebug_Printf:"[BRW] HandleODMDlConfirmationInd drm_content_mime_type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1447_112_2_18_2_7_12_618,(uint8*)"d", req.drm_mo_mime_type);
#endif

        temp_size = MIN(sizeof(req.name_arr), sizeof(ind_ptr->basic_dd_content.name));  /*lint !e506*/
        if (BRW_WStrlen(ind_ptr->basic_dd_content.name) > 0)
        {
            BRW_Memcpy(req.name_arr, ind_ptr->basic_dd_content.name, temp_size);
        }
        else
        {
            DLGenFileName(req.name_arr, ODM_DD_SCHEMA_SHORT_STRING, req.mime_type, PNULL, FALSE);
        }
        
        temp_size = MIN(sizeof(req.description_arr), sizeof(ind_ptr->basic_dd_content.description));    /*lint !e506*/
        BRW_Memcpy(req.description_arr, ind_ptr->basic_dd_content.description, temp_size);

        temp_size = MIN(sizeof(req.vendor_arr), sizeof(ind_ptr->basic_dd_content.vendor));  /*lint !e506*/
        BRW_Memcpy(req.vendor_arr, ind_ptr->basic_dd_content.vendor, temp_size);

        temp_size = BRW_Strlen((uint8 *)(ind_ptr->basic_dd_content.dd_version));
        temp_size = MIN((sizeof(req.dd_version_arr) - 1), temp_size); 
        BRW_Memcpy(req.dd_version_arr, ind_ptr->basic_dd_content.dd_version, temp_size);

        temp_size =  BRW_Strlen((uint8 *)(ind_ptr->basic_dd_content.next_url)); 
        temp_size = MIN((sizeof(req.next_url) - 1), temp_size); 
        BRW_Memcpy(req.next_url, ind_ptr->basic_dd_content.next_url, temp_size);

        req.dl_confirm_cbf = BrwOMADLConfirmCbf;
        brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_OMA_DL_CONFIRM_REQ, &req, sizeof(BRW_MSG_OMADL_CONFIRM_REQ_T));
    }
    while(0);
    
    if (!result)
    {
        //BRWDebug_Printf:"[BRW]W HandleODMDlConfirmationInd:session_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1481_112_2_18_2_7_12_619,(uint8*)"d", ind_ptr->session_id);
        ODM_CancelDownload(ind_ptr->session_id);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_AUTH_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDlAuthInd(void *param)
{
    BOOLEAN result = FALSE;
    BRW_INFO_T  *brw_info_ptr = PNULL;
    WAP_REQUEST_T* wap_req_ptr=PNULL;
    ODM_DOWNLOAD_AUTH_IND_MSG_T *ind_ptr =  PNULL;

    if (PNULL == param)
    {
        return FALSE;
    }

    do
    {
        ind_ptr = (ODM_DOWNLOAD_AUTH_IND_MSG_T*)param;

        brw_info_ptr = BrwGetBrwInfoPtrByDLSessionID(ind_ptr->session_id);
        if (PNULL == brw_info_ptr)
        {
            break;//Abort
        }
        
        wap_req_ptr = BrwGetWapReqByDLSessionID(brw_info_ptr, ind_ptr->session_id);
        if (PNULL == wap_req_ptr)
        {
            break;//Abort
        }

        result = TRUE;
        {
            BRW_MSG_AUTH_REQ_T auth_req={0};
            
            auth_req.handle = wap_req_ptr->req_id;
            auth_req.auth_cbf = BrwOMADLAuthResp;
            auth_req.auth_type = ind_ptr->auth_type;
            brw_info_ptr->profile.msg_proc_func(BRW_MSG_TYPE_AUTH_REQ, &auth_req, sizeof(auth_req));
        }
    }
    while(0);
    
    if (!result)
    {
        //BRWDebug_Printf:"[BRW]W HandleODMDlAuthInd:session_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1535_112_2_18_2_7_13_620,(uint8*)"d", ind_ptr->session_id);
        ODM_CancelDownload(ind_ptr->session_id);
    }
    
    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_PROGRESS_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDlProgressInd(void *param)
{
    BOOLEAN result = FALSE;
    ODM_DOWNLOAD_PROGRESS_IND_MSG_T *ind_ptr = PNULL;
    BRW_INFO_T  *brw_info_ptr = PNULL;
    WAP_REQUEST_T   *wap_req_ptr=PNULL;

    if (PNULL == param)
    {
        return FALSE;
    }
    
    ind_ptr = (ODM_DOWNLOAD_PROGRESS_IND_MSG_T*)param;
    //BRWDebug_Printf:"[BRW]HandleODMDlProgressInd:stage =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1559_112_2_18_2_7_13_621,(uint8*)"d", ind_ptr->stage);

    switch(ind_ptr->stage) 
    {
    case ODM_DUA_STAGE_INIT:
        /* Cann't get download_task_ptr */
    	break;

    case ODM_DUA_STAGE_PARSE_DD:
    	break;

    case ODM_DUA_STAGE_CAPABILITY_CHECK:
    	break;

    case ODM_DUA_STAGE_DOWNLOAD_DD:
    case ODM_DUA_STAGE_DOWNLOAD_MO:
    {
        do
        {
            brw_info_ptr = BrwGetBrwInfoPtrByDLSessionID(ind_ptr->session_id);
            if (PNULL == brw_info_ptr)
            {
                break;//Abort
            }
            
            wap_req_ptr = BrwGetWapReqByDLSessionID(brw_info_ptr, ind_ptr->session_id);
            if (PNULL == wap_req_ptr)
            {
                break;//Abort
            }

            result = TRUE;
            BrwUpdateProgressInfo(brw_info_ptr,wap_req_ptr,ind_ptr->current_len,ind_ptr->content_len);
        }while(0);
    
        if (!result)
        {
            //BRWDebug_Printf:"[BRW]W HandleODMDlProgressInd:session_id = 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1596_112_2_18_2_7_13_622,(uint8*)"d", ind_ptr->session_id);
            ODM_CancelDownload(ind_ptr->session_id);
        }
    }
        break;

    case ODM_DUA_STAGE_INSTALL_MO://ODM也暂未具体实现
    	break;

    case ODM_DUA_STAGE_REPORT:
    	break;

    case ODM_DUA_STAGE_COMPLETED:
    	break;

    default:
    	break;
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_SUCCESS_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDlSuccessInd(void *param)
{
    BOOLEAN result = FALSE;
    ODM_DOWNLOAD_SUCCESS_IND_MSG_T *ind_ptr = PNULL;
    BRW_INFO_T  *brw_info_ptr = PNULL;
    WAP_REQUEST_T   *wap_req_ptr=PNULL;
    
    if (PNULL == param)
    {
        return FALSE;
    }

    do
    {
        ind_ptr = (ODM_DOWNLOAD_SUCCESS_IND_MSG_T*)param;
        brw_info_ptr = BrwGetBrwInfoPtrByDLSessionID(ind_ptr->session_id);
        if (PNULL == brw_info_ptr)
        {
            break;//Abort
        }
        
        wap_req_ptr = BrwGetWapReqByDLSessionID(brw_info_ptr, ind_ptr->session_id);
        if ((PNULL == wap_req_ptr) || (wap_req_ptr->download_state != BRW_DL_STATE_OMA_CONTENT))
        {
            break;//Abort
        }

        result = TRUE;
        
        BrwProcDLSuccess(wap_req_ptr, brw_info_ptr);
    }while(0);

    if (!result)
    {
        //BRWDebug_Printf:"[BRW]W HandleODMDlSuccessInd:session_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1656_112_2_18_2_7_13_623,(uint8*)"d", ind_ptr->session_id);
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_ERROR_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDlErrorInd(void *param)
{
    BOOLEAN result = FALSE;
    ODM_DOWNLOAD_ERROR_IND_MSG_T *ind_ptr = PNULL;
    BRW_INFO_T  *brw_info_ptr = PNULL;
    WAP_REQUEST_T   *wap_req_ptr=PNULL;
    
    if (PNULL == param)
    {
        return FALSE;
    }
    
    do
    {
        ind_ptr = (ODM_DOWNLOAD_ERROR_IND_MSG_T*)param;
        //BRWDebug_Printf:"HandleODMDlErrorInd error_no =%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1681_112_2_18_2_7_13_624,(uint8*)"d", ind_ptr->error_no);
    
        brw_info_ptr = BrwGetBrwInfoPtrByDLSessionID(ind_ptr->session_id);
        if (PNULL == brw_info_ptr)
        {
            break;//Abort
        }
        
        wap_req_ptr = BrwGetWapReqByDLSessionID(brw_info_ptr, ind_ptr->session_id);
        if (PNULL == wap_req_ptr)
        {
            break;//Abort
        }

        result = TRUE;
        wap_req_ptr->state = BRW_STATE_ALL_FINISHED;//end
//        BrwProcDownloadErr(brw_info_ptr,wap_req_ptr,BRW_ERR_UNKNOWN);
        //103592
        switch(ind_ptr->error_no)
        {
        case ERR_ODM_INVALID_DD:
            BrwProcDownloadErr(brw_info_ptr, wap_req_ptr, BRW_ERR_INVALID_DD);
            break;
        default:
            BrwProcDownloadErr(brw_info_ptr, wap_req_ptr, BRW_ERR_UNKNOWN);
            break;
        }
    }while(0);

    //BRWDebug_Printf:"HandleODMDlErrorInd result =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1700_112_2_18_2_7_13_625,(uint8*)"d", result);

    return result;
}
/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_HEADER_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDlHeaderInd(void *param)
{
#ifdef DRM_SUPPORT
    BOOLEAN result = FALSE;
    ODM_DOWNLOAD_HEADER_IND_MSG_T *ind_ptr = PNULL;
    BRW_INFO_T  *brw_info_ptr = PNULL;
    WAP_REQUEST_T   *wap_req_ptr=PNULL;
    uint32  time_out = 0;

    if (PNULL == param)
    {
        return FALSE;
    }

    do
    {
        ind_ptr = (ODM_DOWNLOAD_HEADER_IND_MSG_T*)param;
        brw_info_ptr = BrwGetBrwInfoPtrByDLSessionID(ind_ptr->session_id);
        if (PNULL == brw_info_ptr)
        {
            break;//Abort
        }
        
        wap_req_ptr = BrwGetWapReqByDLSessionID(brw_info_ptr, ind_ptr->session_id);
        if (PNULL == wap_req_ptr)
        {
            break;//Abort
        }
        result = TRUE;
        
        time_out = GetDrmSDExpiredTime((const uint8 *)ind_ptr->header_ptr);
        //BRWDebug_Printf:"[BRW]HandleODMDlHeaderInd:time_out = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1739_112_2_18_2_7_13_626,(uint8*)"d", time_out);
        wap_req_ptr->drm_sd_time_out = time_out;

    }while(0);

    if (!result)
    {
        //do nothing
    }
    return result;

#else    
    return TRUE;
#endif    
}

#if 0//待扩展
/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_GET_SESSION_LIST_CNF
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDlGetSessionListCnf(void *param)
{
    BOOLEAN result = FALSE;
    ODM_DOWNLOAD_GET_SESSION_LIST_CNF_MSG_T *msg_data_ptr = PNULL;

    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_GET_SESSION_INFO_CNF
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDlGetSessionInfoCnf(void *param)
{
    BOOLEAN result = FALSE;
    ODM_DOWNLOAD_GET_SESSION_INFO_CNF_MSG_T *msg_data_ptr = PNULL;

    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_SUBSCRIBE_INDICATION_CNF
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMSubscribeIndicationCnf(void *param)
{
    BOOLEAN result = FALSE;
    ODM_SUBSCRIBE_INDICATION_CNF_MSG_T *msg_data_ptr = PNULL;

    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_DESUBSCRIBE_INDICATION_CNF
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMDesubscribeIndicationCnf(void *param)
{
    BOOLEAN result = FALSE;
    ODM_DESUBSCRIBE_INDICATION_CNF_MSG_T *msg_data_ptr = PNULL;


    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_PROGRESS_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMManagerProgressInd(void *param)
{
    BOOLEAN result = FALSE;
    ODM_MANAGER_PROGRESS_IND_MSG_T *msg_data_ptr = PNULL;

    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_SUCCESS_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMManagerSuccessInd(void *param)
{
    BOOLEAN result = FALSE;
    ODM_MANAGER_SUCCESS_IND_MSG_T *msg_data_ptr = PNULL;
    
    return result;
}

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_ERROR_IND
//  Global resource dependence : 
//  Author: fen.xie
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN HandleODMManagerErrorInd(void *param)
{
    BOOLEAN result = FALSE;
    ODM_MANAGER_ERROR_IND_MSG_T *msg_data_ptr = PNULL;
    
    return result;
}
#endif

/*
 *	desc:process http task's signal:OMA Download
 *  param:msg_id  message id
 *        param   message parameter
 *  TRUE if msg is http's message,else FALSE
 */
PUBLIC BOOLEAN BrwProcODMMsg(uint32 msg_id,void* param)
{
    BOOLEAN result = TRUE;
    void    *original_param = PNULL;

    if (PNULL == param)
    {
        //BRWDebug_Printf:"[BRW]W BrwProcODMMsg param is PNULL msg_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1854_112_2_18_2_7_13_627,(uint8*)"d", msg_id);
        return FALSE;
    }
    
    original_param = (void*)((uint32)param);
    switch (msg_id)
    {
    case ODM_START_DOWNLOAD_CNF:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_START_DOWNLOAD_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1862_112_2_18_2_7_13_628,(uint8*)"");
        result = HandleODMStartDlCnf(original_param);
        break;

    case ODM_CANCEL_DOWNLOAD_CNF:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_CANCEL_DOWNLOAD_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1867_112_2_18_2_7_13_629,(uint8*)"");
        result = HandleODMCancelDlCnf(original_param);
        break;

    case ODM_DOWNLOAD_CONFIRMATION_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_CONFIRMATION_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1872_112_2_18_2_7_13_630,(uint8*)"");
        result = HandleODMDlConfirmationInd(original_param);
        break;

    case ODM_DOWNLOAD_AUTH_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_AUTH_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1877_112_2_18_2_7_13_631,(uint8*)"");
        result = HandleODMDlAuthInd(original_param);
        break;

    case ODM_DOWNLOAD_PROGRESS_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_PROGRESS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1882_112_2_18_2_7_13_632,(uint8*)"");
        result = HandleODMDlProgressInd(original_param);
        break;

    case ODM_DOWNLOAD_SUCCESS_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_SUCCESS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1887_112_2_18_2_7_13_633,(uint8*)"");
        result = HandleODMDlSuccessInd(original_param);
        break;

    case ODM_DOWNLOAD_ERROR_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1892_112_2_18_2_7_13_634,(uint8*)"");
        result = HandleODMDlErrorInd(original_param);
        break;

    case ODM_DOWNLOAD_HEADER_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1897_112_2_18_2_7_13_635,(uint8*)"");
        result = HandleODMDlHeaderInd(original_param);
        break;
        
#if 0//待扩展
    case ODM_DOWNLOAD_GET_SESSION_LIST_CNF:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_GET_SESSION_LIST_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1903_112_2_18_2_7_13_636,(uint8*)"");
        result = HandleODMDlGetSessionListCnf(original_param);
        break;

    case ODM_DOWNLOAD_GET_SESSION_INFO_CNF:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DOWNLOAD_GET_SESSION_INFO_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1908_112_2_18_2_7_13_637,(uint8*)"");
        result = HandleODMDlGetSessionInfoCnf(original_param);
        break;

    case ODM_SUBSCRIBE_INDICATION_CNF:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_START_DOWNLOAD_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1913_112_2_18_2_7_13_638,(uint8*)"");
        result = HandleODMSubscribeIndicationCnf(original_param);
        break;

    case ODM_DESUBSCRIBE_INDICATION_CNF:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_DESUBSCRIBE_INDICATION_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1918_112_2_18_2_7_13_639,(uint8*)"");
        result = HandleODMDesubscribeIndicationCnf(original_param);
        break;

    case ODM_MANAGER_PROGRESS_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_MANAGER_PROGRESS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1923_112_2_18_2_7_13_640,(uint8*)"");
        result = HandleODMManagerProgressInd(original_param);
        break;

    case ODM_MANAGER_SUCCESS_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_MANAGER_SUCCESS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1928_112_2_18_2_7_13_641,(uint8*)"");
        result = HandleODMManagerSuccessInd(original_param);
        break;

    case ODM_MANAGER_ERROR_IND:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg receive ODM_MANAGER_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1933_112_2_18_2_7_13_642,(uint8*)"");
        result = HandleODMManagerErrorInd(original_param);
        break;
#endif

    default:
        //BRWDebug_Printf:"[BRW]BrwProcODMMsg Not handle msg_id = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1939_112_2_18_2_7_13_643,(uint8*)"d", msg_id);
        result = FALSE;
        break;
    }

    return result;
}
#endif

/*****************************************************************************/
// Description : convert browser internal mine type to mime type in platform
// Global resource dependence :
// Author:  fen.xie
// Note:
/*****************************************************************************/
PUBLIC MIME_TYPE_E BrwConvertMimeType(BRW_MIME_TYPE_E brw_mime_type)
{
    uint32 i = 0;

    for( i = 0; i < (sizeof(s_mime_type_table)/sizeof(BRW_DL_MIME_TYPE_T)); i++) 
    {
        if (s_mime_type_table[i].brw_type == brw_mime_type)
        {
            return s_mime_type_table[i].type;
        }
    }

    return MIME_TYPE_UNKNOWN;
}

/*****************************************************************************/
// Description : Get  File Name for download by mime type
// Global resource dependence :
// Author: fen.xie
// Note:  
/*****************************************************************************/
PUBLIC BOOLEAN BrwGetDlFileNameByMimeType(wchar *file_name_ptr, uint32 name_len, uint32 mime_type,wchar *default_file_name_ptr)
{
    return DLGenFileName(file_name_ptr, name_len, mime_type, default_file_name_ptr,FALSE);
}

#ifdef BROWSER_GZIP_SUPPORT
/*****************************************************************************/
// Description : set s_dl_temp_file_name_ptr(alloc memory)
// Global resource dependence :
// Author:  fen.xie
// Note:
/*****************************************************************************/
PUBLIC BOOLEAN BrwSetDlTempFilename(const uint16*file_name_ptr, uint16 file_name_len)
{
    if (PNULL != s_dl_temp_file_name_ptr)//protect
    {
        BRWPL_DelFileSyn(s_dl_temp_file_name_ptr, BRW_WStrlen(s_dl_temp_file_name_ptr));
        BRWMem_Free(s_dl_temp_file_name_ptr);
        s_dl_temp_file_name_ptr = PNULL;
    }

    s_dl_temp_file_name_ptr = BRWMem_Alloc(sizeof(uint16)*(file_name_len+1));
    if(PNULL == s_dl_temp_file_name_ptr)
    {
        //BRWDebug_Printf:"[BRW]W BrwSetDlTempFilename err, alloc fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_1993_112_2_18_2_7_13_644,(uint8*)"");
        return FALSE;
    }
    BRW_Memset(s_dl_temp_file_name_ptr,0,sizeof(uint16)*(file_name_len+1));
    BRW_WstrCat(s_dl_temp_file_name_ptr, file_name_ptr);

    return TRUE;
}

/*****************************************************************************/
// Description : delete the file of s_dl_temp_file_name_ptr and free it
// Global resource dependence :
// Author:  fen.xie
// Note:
/*****************************************************************************/
PUBLIC void BrwDelDlTempFile(void)
{
    BOOLEAN result = TRUE;
    
    if (PNULL != s_dl_temp_file_name_ptr)
    {
        if (MMIAPIFMM_IsFileExist(s_dl_temp_file_name_ptr, BRW_WStrlen(s_dl_temp_file_name_ptr)))
        {
            result = BRWPL_DelFileSyn(s_dl_temp_file_name_ptr, BRW_WStrlen(s_dl_temp_file_name_ptr));
            //BRWDebug_Printf:"[BRW]BrwDelDlTempFile:result = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,BRW_DLINTERFACE_2015_112_2_18_2_7_13_645,(uint8*)"d", result);
        }
        
        if (result)//delete success or the file isnot exist
        {
            BRWMem_Free(s_dl_temp_file_name_ptr);
            s_dl_temp_file_name_ptr = PNULL;
        }
    }
    
}
#endif

