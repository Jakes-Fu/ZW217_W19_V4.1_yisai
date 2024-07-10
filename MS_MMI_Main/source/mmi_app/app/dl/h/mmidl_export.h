/***************************************************************************
** File Name:      mmidl_export.h                                                                               *
** Author:                                                                                                                    *
** Date:           28/07/2009                                                                                          *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                             *
** Description:    This file is used to download define                                      *
****************************************************************************
**                         Important Edit History                                                                       *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                  *
** 07/2009        wancan.you    Create
****************************************************************************/
#ifndef  _MMIDL_EXPORT_H_    
#define  _MMIDL_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "mmipdp_export.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMIDL_MAX_DOWNLOAD_TASK_NUM 20
#define MMIDL_URL_MAX   (1024)
#define MIME_EXTEND_ID_START			(10000)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef struct _MMIDL_DOWNLOAD_INFO_T_
{
    uint32 mime_type;              ///< MIME type of content being downloaded.
    uint8* http_header;      ///< Header string of HTTP connection of content being downloaded.
    uint8* url;             ///< URL of content being downloaded.
    BOOLEAN is_chunk;          ///< Whether HTTP connection of content being downloaded is in chunk format.
    uint32 status_code;        ///< Status code.
    uint32 method;            ///< Method of HTTP request when error occurs.
    uint8* buf;
    uint32 buf_len;
    MMIPDP_INTERFACE_E bearer_type;
    MN_DUAL_SYS_E dual_sys;
}MMIDL_DOWNLOAD_INFO_T;

typedef struct _MMIDL_DOWNLOAD_SAVE_AS_INFO_T_
{
    uint8* url;
    uint8* buf;
    uint32 buf_len;
    uint32 mime_type;
} MMIDL_DOWNLOAD_SAVE_AS_INFO_T;

typedef enum
{
    MMIDL_CREATE_NEW,
    MMIDL_CREATE_SAVE_AS,
    MMIDL_OPEN_DOWNLOAD_MANAGER,
    MMIDL_INTERFACE_TYPE_MAX
}MMIDL_INTERFACE_TYPE_E;

typedef enum
{
    MMIDL_DOWNLOAD_AUDIO_TYPE,
    MMIDL_DOWNLOAD_VIDEO_TYPE,
    MMIDL_DOWNLOAD_PICTURE_TYPE,
    MMIDL_DOWNLOAD_TEXT_TYPE,
    MMIDL_DOWNLOAD_OTHER_TYPE,
    MMIDL_DOWNLOAD_TYPE_MAX
}MMIDL_DOWNLOAD_TYPE;

//From Netfront NF_mime_types.h

typedef enum
{
	MIME_DEFAULT_DEFAULT = 0,                               // */*	
	MIME_TEXT_DEFAULT,                                // text/*
	MIME_APPLICATION_DEFAULT,                         // application/*
	MIME_IMAGE_DEFAULT,                               // image/*
	MIME_AUDIO_DEFAULT,                               // audio/*
	MIME_VIDEO_DEFAULT,                               // video/*
	MIME_MODEL_DEFAULT,                               // model/*
	MIME_MESSAGE_DEFAULT,                             // message/*
	MIME_MULTIPART_DEFAULT,                           // multipart/*								
	MIME_X_WORLD,                             // x-world/*								
	MIME_TEXT_HTML,                             // text/html								
	MIME_APPLICATION_XHTML,                 // application/xhtml+xml					
	MIME_TEXT_PLAIN,                            // text/plain								
	MIME_TEXT_XML,                              // text/xml								    
	MIME_APPLICATION_XML,                       // application/xml							
	MIME_IMAGE_GIF,                             // image/gif								
	MIME_IMAGE_JPEG,                            // image/jpeg								
	MIME_IMAGE_PJPEG,                           // image/pjpeg								
	MIME_IMAGE_PNG,                             // image/png								
	MIME_IMAGE_BMP,                             // image/bmp								
	MIME_VIDEO_X_MNG,                           // video/x-mng								
	MIME_TEXT_CSS,                              // text/css								    
	MIME_TEXT_JAVASCRIPT,                       // text/javascript							
	MIME_APPLICATION_X_JAVASCRIPT,              // application/x-javascript				    
	MIME_APPLICATION_SMIL,                      // application/smil						    
	MIME_MESSAGE_RFC822,                        // message/rfc822							
	MIME_APPLICATION_JAVASCRIPT,                // application/javascript					
	MIME_TEXT_ECMASCRIPT,                       // text/ecmascript							
	MIME_APPLICATION_ECMASCRIPT,                // application/ecmascript					
	MIME_MULTIPART_MIXED,                       // multipart/mixed							
	MIME_MULTIPART_RELATED,                     // multipart/related						
	MIME_MULTIPART_ALTERNATIVE,                 // multipart/alternative					
	MIME_MULTIPART_BYTERANGES,                  // multipart/byteranges					    
	MIME_APPLICATION_RDF,                   // application/rdf+xml						
	MIME_APPLICATION_RSS,                   // application/rss+xml						
	MIME_APPLICATION_ATOM,                  // application/atom+xml					    
	MIME_APPLICATION_OPML,                  // application/opml+xml					    
	MIME_APPLICATION_X_NETCDF,                  // application/x-netcdf					    
	MIME_APPLICATION_X_XBEL,                    // application/x-xbel						
	MIME_APPLICATION_X_URL,                     // application/x-url						
	MIME_TEXT_WML,                      // text/vnd.wap.wml						    
	MIME_IMAGE_WBMP,                    // image/vnd.wap.wbmp						
	MIME_TEXT_WMLSCRIPT,                // text/vnd.wap.wmlscript					
	MIME_APPLICATION_XHTMLMP,         // application/vnd.wap.xhtml+xml			
	MIME_APPLICATION_WBXML,             // application/vnd.wap.wbxml				
	MIME_APPLICATION_WMLC,              // application/vnd.wap.wmlc				    
	MIME_APPLICATION_WMLSC,             // application/vnd.wap.wmlsc				
	MIME_APPLICATION_WMLSCRIPTC,        // application/vnd.wap.wmlscriptc			
	MIME_IMAGE_SVG,                         // image/svg+xml							
	MIME_APPLICATION_PDF,                       // application/pdf							
	MIME_APPLICATION_X_SHOCKWAVE_FLASH,         // application/x-shockwave-flash			
	MIME_APPLICATION_FUTURESPLASH,              // application/futuresplash				    
	MIME_APPLICATION_X_DIRECTOR,                // application/x-director					
	MIME_AUDIO_X_PN_REALAUDIO,                  // audio/x-pn-realaudio					    
	MIME_APPLICATION_X_JAVA,                    // application/x-java						
	MIME_AUDIO_MIDI,                            // audio/midi								
	MIME_AUDIO_X_WAV,                           // audio/x-wav								
	MIME_AUDIO_X_AIFF,                          // audio/x-aiff							    
	MIME_AUDIO_BASIC,                           // audio/basic								
	MIME_VIDEO_MPEG,                            // video/mpeg								
	MIME_AUDIO_MPEG,                            // audio/mpeg								
	MIME_OMA_DRM_MESSAGE,       // application/vnd.oma.drm.message			
	MIME_OMA_DRM_RIGHTS_XML,    // application/vnd.oma.drm.rights+xml		
	MIME_OMA_DRM_RIGHTS_WBXML,  // application/vnd.oma.drm.rights+wbxml	    
	MIME_OMA_DRM_CONTENT,       // application/vnd.oma.drm.content			
	MIME_APPLICATION_X_WWW_FORM_URLENCODED,     // application/x-www-form-urlencoded		
	MIME_MULTIPART_FORM_DATA,                   // multipart/form-data						
	MIME_APPLICATION_OCTET_STREAM,              // application/octet-stream				    
	MIME_APPLICATION_X_GZIP,                    // application/x-gzip						
	MIME_APPLICATION_X_TAR,                     // application/x-tar						
	MIME_APPLICATION_ZIP,                       // application/zip							
	MIME_VIDEO_X_MSVIDEO,                       // video/x-msvideo							
	MIME_VIDEO_QUICKTIME,                       // video/quicktime							
	MIME_VIDEO_3GPP,                            // video/3gpp								
	MIME_VIDEO_3GPP2,                           // video/3gpp2								
	MIME_AUDIO_3GPP,                            // audio/3gpp								
	MIME_AUDIO_3GPP2,                           // audio/3gpp2								
	MIME_VIDEO_X_MS_WMV,                        // video/x-ms-wmv							
	MIME_VIDEO_X_MS_ASF,                        // video/x-ms-asf							
	MIME_VIDEO_X_MS_ASX,                        // video/x-ms-asx							
	MIME_VIDEO_X_MS_WVX,                        // video/x-ms-wvx							
	MIME_VIDEO_X_MS_WM,                         // video/x-ms-wm							
	MIME_AUDIO_X_MS_WMA,                        // audio/x-ms-wma							
	MIME_AUDIO_X_MS_WAX,                        // audio/x-ms-wax							
	MIME_AUDIO_X_GSM,                           // audio/x-gsm								
	MIME_TEXT_XSL,                              // text/xsl								    
	MIME_IMAGE_TIFF,                            // image/tiff								
	MIME_IMAGE_X_ICON,                          // image/x-icon							    
	MIME_IMAGE_X_XPIXMAP,                       // image/x-xpixmap							
	MIME_IMAGE_X_XBITMAP,                       // image/x-xbitmap							
	MIME_MODEL_VRML,                            // model/vrml								
	MIME_X_WORLD_X_VRML,                        // x-world/x-vrml							
	MIME_APPLICATION_X_BINARY_JAVASCRIPT,       // application/x-binary-javascript			
	MIME_APPLICATION_WORD,                      // application/word						    
	MIME_APPLICATION_X_WORD,                    // application/x-word						
	MIME_APPLICATION_MSWORD,                    // application/msword						
	MIME_APPLICATION_X_MSWORD,                  // application/x-msword					    
	MIME_APPLICATION_VND_MS_WORD,               // application/vnd.ms-word					
	MIME_APPLICATION_EXCEL,                     // application/excel						
	MIME_APPLICATION_X_EXCEL,                   // application/x-excel						
	MIME_APPLICATION_MSEXCEL,                   // application/msexcel						
	MIME_APPLICATION_X_MSEXCEL,                 // application/x-msexcel					
	MIME_APPLICATION_VND_MS_EXCEL,              // application/vnd.ms-excel				    
	MIME_TEXT_CSV,                              // text/csv								    
	MIME_TEXT_COMMA_SEPARATED_VALUES,           // text/comma-separated-values				
	MIME_APPLICATION_POWERPOINT,                // application/powerpoint					
	MIME_APPLICATION_X_POWERPOINT,              // application/x-powerpoint				    
	MIME_APPLICATION_MSPOWERPOINT,              // application/mspowerpoint				    
	MIME_APPLICATION_X_MSPOWERPOINT,            // application/x-mspowerpoint				
	MIME_APPLICATION_VND_MS_POWERPOINT,         // application/vnd.ms-powerpoint			

	MIME_EXTEND_DD = MIME_EXTEND_ID_START + 20,
	MIME_EXTEND_DD2,
	MIME_EXTEND_JAD,
	MIME_EXTEND_APP_JAR,             
	MIME_EXTEND_APP_JAR_ARCHIVE,     
	MIME_EXTEND_APP_X_JAR_ARCHIVE,   
	MIME_EXTEND_APP_SDP,
	MIME_EXTEND_APP_X_SDP,
	MIME_EXTEND_APP_VND_SDP,
	MIME_EXTEND_APPLICATION_VND_SMAF,
	MIME_EXTEND_APPLICATION_X_SMAF,
	MIME_EXTEND_AUDIO_MP3,
	MIME_EXTEND_AUDIO_AAC,
	MIME_EXTEND_VIDEO_MP4,
	MIME_EXTEND_DCF, //add by magicmoon on 2010-03-11 for dealing with 'dcf' download
	MIME_EXTEND_VCF, // added by FG on 2010-08-11 for 'vcf' download
	MIME_EXTEND_DCF2,
	MIME_EXTEND_DCF3,
	MIME_EXTEND_DCF4,
	MIME_EXTEND_DCF5,
	MIME_EXTEND_AUDIO_DD,

    DL_MIME_TYPE_MAX
}DL_MIME_TYPE_E;

typedef struct _DL_MIME_INFO_T_
{
  DL_MIME_TYPE_E value;
  const char* ext_ptr;
}DL_MIME_INFO_T;

//added for cr240847 begin
typedef enum
{
    MMIDL_DOWNLOAD_SUCCESS,             //成功
    MMIDL_DOWNLOAD_ERROR_NO_SPACE,      //磁盘空间不足
    MMIDL_DOWNLOAD_ERROR_NO_MEMORY,     //内存不足
    MMIDL_DOWNLOAD_ERROR_HTTP,          //HTTP错误
    MMIDL_DOWNLOAD_ERROR_ODM,           //ODM错误
    MMIDL_DOWNLOAD_ERROR_GENERAL,       //通用错误
    MMIDL_DOWNLOAD_ERROR_STOP,          //停止下载
    MMIDL_DOWNLOAD_RESULT_MAX
}MMIDL_DOWNLOAD_RESULT_E;
//added for cr240847 end

typedef struct _MMIDL_MIME_CALLBACK_PARAM_T_
{
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN + 1]; /*Including path*/
    uint32  mime_type;
    uint8 *url_ptr;//modified for cr240847
    void *user_data;
    BOOLEAN   download_result;
    MMIDL_DOWNLOAD_RESULT_E result_code;//added for cr240847
}MMIDL_MIME_CALLBACK_PARAM_T; /* Urge: Modify MMIBROWSER_MIME_CALLBACK_PARAM_T simultaneously */

typedef void (*MMIDL_MIME_CALLBACK)(MMIDL_MIME_CALLBACK_PARAM_T* param_ptr);

typedef struct _MMIDL_MIME_DOWNLOAD_PARAM_T_
{
    uint32 mime_type;
    MMIDL_MIME_CALLBACK call_back;
}MMIDL_MIME_DOWNLOAD_PARAM_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : start download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_StartDownload(MMIDL_INTERFACE_TYPE_E type, DPARAM param);

/*****************************************************************************/
//  Description : stop download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_StopDownload(uint32 task_index, BOOLEAN is_all);

/*****************************************************************************/
//  Description : Init download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_InitDownload(void);

/*****************************************************************************/
//  Description : register DL module nv len and max item
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_InitModule(void);

/*****************************************************************************/
// Description: convert mime string to mimi type
// Global resource dependence: 
// Author: jiaoyou.wu
// Note:Index means Mime type
/*****************************************************************************/
PUBLIC uint32 MMIAPIDL_MimeExtToMimeIndex(const char *mime_ext);

/*****************************************************************************/
// Description: convert Netfront mime type to WWW mimi type
// Global resource dependence: 
// Author: jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC DL_MIME_TYPE_E MMIAPIDL_NFMimeTypeToDLmimeType(const uint32 in_type);

/*****************************************************************************/
// Description: convert Netfront mime type to ext
// Global resource dependence: 
// Author: jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC const char *MMIAPIDL_NFMimeTypeToExt(const uint32 in_type);

/*****************************************************************************/
//  Description : reg mime func
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDL_RegMimeCallback(MMIDL_MIME_DOWNLOAD_PARAM_T* param_ptr);

/*****************************************************************************/
//  Description : Get audio type by nf type
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIDL_GetAudioTypeByNFMimeType(uint32 mime_type);

/*****************************************************************************/
//  Description : Get download Status
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDL_GetDownloadStatus(void);

/*****************************************************************************/
//  Description : set download setting to default
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDL_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : Delete completed download task 
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_DeleteCompletedDownloadTask(uint32 index);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : Is dl at top
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDL_IsDownloadActive(void);

/*****************************************************************************/
//  Description : Get Download Path
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDL_GetDownloadPath(
                                                                                    MMIDL_DOWNLOAD_TYPE type,
                                                                                    wchar      *full_path_ptr,     //out
                                                                                    uint16     *full_path_len_ptr  //in/out, 双字节为单位
                                                                                    );

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_HandleServiceChange(MMIPDP_INTERFACE_E bearer_type);

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define 	MMIDL_mimeExt_to_mimeIndex	            MMIAPIDL_MimeExtToMimeIndex

#define 	MMIDL_NFmimeType_to_DLmimeType   MMIAPIDL_NFMimeTypeToDLmimeType

#define 	MMIDL_NFmimeType_to_Ext		    MMIAPIDL_NFMimeTypeToExt

#define 	MMIDL_HandleServiceChange		    MMIAPIDL_HandleServiceChange

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef __cplusplus
    }
#endif

#endif


