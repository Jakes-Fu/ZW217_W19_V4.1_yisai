/****************************************************************************
** File Name:      mmidl.c                                            *
** Author:                                                                    *
** Date:           07/18/2009                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe the download module           *
*****************************************************************************
**                         Important Edit History                            *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 07/2009        wancan.you        Create
** 
****************************************************************************/
#define _MMIDL_C_  

/**-------------------------------------------------------------------------*
**                         Dependencies                                     *
**--------------------------------------------------------------------------*/
#include "mmi_app_dl_trc.h"
#ifdef WIN32
#include "std_header.h"
#include "sig_code.h"
#endif
#include <string.h>
#include "mmk_app.h"
#include "mmi_applet_table.h"
#include "in_message.h"
#include "http_api.h"
#include "mmidl_export.h"
#include "mmidl_internal.h"
#include "mmidl_nv.h"
#include "mmidl_text.h"
#include "mmidl_menutab.h"
#include "mmifmm_export.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmipdp_export.h"
#include "mmiphone_export.h"
#include "mmi_appmsg.h"
#include "mmienvset_export.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#include "mmicc_export.h"
#endif
#include "mmidl_id.h"
#include "mmiudisk_export.h"
#include "mmieng_export.h"
#ifdef MMI_WIFI_SUPPORT
#include "mmiwifi_export.h"
#endif
#include "freq_cfg.h"
#ifdef STREAMING_HS_SUPPORT
#include "mmismp_export.h"
#endif
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#ifdef OMADL_SUPPORT
#include "odm_api.h"
#endif

#ifdef DL_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMIDL_HTTP_STATUS_CODE_SUCCESS                  (200)
#define MMIDL_HTTP_STATUS_CODE_PARTIAL_CONTENT          (206)
#define MMIDL_HTTP_STATUS_CODE_MOVED_PERMANENTLY        (301)
#define MMIDL_HTTP_STATUS_CODE_FOUND                    (302)
#define MMIDL_HTTP_STATUS_CODE_TEMPORARY_REDIRECT       (307)
#define MMIDL_HTTP_STATUS_CODE_UNAUTHORIZED             (401)
#define MMIDL_HTTP_STATUS_CODE_UTHENTICATION_REQUIRED   (407)
#define MMIDL_HTTP_STATUS_CODE_NOT_FOUND                (404)

#define MMIDL_ACCEPT_STRING "ACCEPT: text/html, application/xml;q=0.9, application/java-archive, text/vnd.sun.j2me.app-descriptor, application/xhtml+xml, image/png, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1"//"*/*"

#define MMIDL_ACCEPT_LANGUAGE_STRING "zh-cn"

#define MMIDL_ACCEPT_CHARSET_STRING "iso-8859-5"

#define MMIDL_RANGE_HEADER_LEN  40
#define MMIDL_RANGE_HEADER_PREFIX  "bytes="

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct _MMIDL_APPLET_PARAM_T_
{
    MMIDL_INTERFACE_TYPE_E type;
    DPARAM param;
}MMIDL_APPLET_PARAM_T;

const DL_MIME_INFO_T mime_table[] =
{
    {MIME_TEXT_HTML,                "html"},
    {MIME_TEXT_HTML,                "htm"},
    {MIME_APPLICATION_XHTML,        "xht"},
    {MIME_APPLICATION_XHTML,        "xhtml"},
    {MIME_TEXT_PLAIN,               "txt"},
    {MIME_TEXT_XML,                 "xml"},
    {MIME_IMAGE_GIF,                "gif"},
    {MIME_IMAGE_JPEG,               "jpeg"},
    {MIME_IMAGE_JPEG,               "jpg"},
    {MIME_IMAGE_JPEG,               "jpe"},
    {MIME_IMAGE_PNG,       "png"},
    {MIME_IMAGE_BMP,       "bmp"},
    {MIME_VIDEO_X_MNG,       "mng"},
    {MIME_TEXT_CSS,       "css"},
    {MIME_TEXT_JAVASCRIPT,       "js"},
    {MIME_APPLICATION_ECMASCRIPT,       "es"},
    {MIME_MESSAGE_RFC822,       "mht"},
    {MIME_MESSAGE_RFC822,       "mhtl"},
    {MIME_MESSAGE_RFC822,       "eml"},
    {MIME_MESSAGE_RFC822,       "nws"},
    {MIME_APPLICATION_SMIL,       "smi"},
    {MIME_APPLICATION_SMIL,       "smil"},
    {MIME_APPLICATION_SMIL,       "sml"},
    {MIME_APPLICATION_SMIL,       "mms"},
    {MIME_APPLICATION_RDF,       "rdf"},
    {MIME_APPLICATION_RSS,       "rss"},
    {MIME_APPLICATION_ATOM,       "atom"},
    {MIME_APPLICATION_OPML,       "opml"},
    {MIME_APPLICATION_X_NETCDF,       "cdf"},
    {MIME_APPLICATION_X_XBEL,       "xbel"},
    {MIME_APPLICATION_X_URL,       "url"},
    {MIME_TEXT_WML,       "wml"},
    {MIME_IMAGE_WBMP,       "wbmp"},
    {MIME_TEXT_WMLSCRIPT,       "wmls"},
    {MIME_APPLICATION_XHTMLMP,       "xhtmlmp"},
    {MIME_APPLICATION_WBXML,       "wbxml"},
    {MIME_APPLICATION_WMLC,       "wmlc"},
    {MIME_APPLICATION_WMLSC,       "wmlsc"},
    {MIME_IMAGE_SVG,       "svg"},
    {MIME_IMAGE_SVG,       "svgz"},
    {MIME_APPLICATION_PDF,       "pdf"},
    {MIME_APPLICATION_X_SHOCKWAVE_FLASH,       "swf"},
    {MIME_APPLICATION_X_SHOCKWAVE_FLASH,       "swfl"},
    {MIME_APPLICATION_FUTURESPLASH,       "spl"},
    {MIME_AUDIO_X_PN_REALAUDIO,       "ra"},
    {MIME_AUDIO_X_PN_REALAUDIO,       "rm"},
    {MIME_AUDIO_X_PN_REALAUDIO,       "ram"},
    {MIME_APPLICATION_X_JAVA,       "class"},
    {MIME_AUDIO_MIDI,       "mid"},
    {MIME_AUDIO_MIDI,       "midi"},
    {MIME_AUDIO_MIDI,       "rmi"},
    {MIME_AUDIO_X_WAV,       "wav"},
    {MIME_AUDIO_X_AIFF,       "aif"},
    {MIME_AUDIO_X_AIFF,       "aifc"},
    {MIME_AUDIO_X_AIFF,       "aiff"},
    {MIME_AUDIO_BASIC,       "au"},
    {MIME_AUDIO_BASIC,       "snd"},
    {MIME_VIDEO_MPEG,       "mpeg"},
    {MIME_VIDEO_MPEG,       "mpg"},
    {MIME_VIDEO_MPEG,       "mpe"},
    {MIME_AUDIO_MPEG,       "mp3"},
    {MIME_AUDIO_MPEG,       "mp2"},
    {MIME_AUDIO_MPEG,       "mpga"},
    {MIME_AUDIO_MPEG,       "mpega"},
    {MIME_APPLICATION_OCTET_STREAM,       "bin"},
    {MIME_APPLICATION_OCTET_STREAM,       "exe"},
    {MIME_APPLICATION_X_GZIP,       "gz"},
    {MIME_APPLICATION_X_GZIP,       "gzip"},
    {MIME_APPLICATION_X_TAR,       "tar"},
    {MIME_APPLICATION_ZIP,       "zip"},
    {MIME_APPLICATION_X_DIRECTOR,       "dcl"},
    {MIME_APPLICATION_X_DIRECTOR,       "dir"},
    {MIME_APPLICATION_X_DIRECTOR,       "dxr"},
    {MIME_VIDEO_X_MSVIDEO,       "avi"},
    {MIME_VIDEO_QUICKTIME,       "qt"},
    {MIME_VIDEO_QUICKTIME,       "mov"},
    {MIME_VIDEO_3GPP,       "3gp"},
    {MIME_VIDEO_3GPP2,       "3g2"},
    {MIME_VIDEO_X_MS_WMV,       "wmv"},
    {MIME_VIDEO_X_MS_ASF,       "asf"},
    {MIME_VIDEO_X_MS_ASX,       "asx"},
    {MIME_VIDEO_X_MS_WVX,       "wvx"},
    {MIME_VIDEO_X_MS_WM,       "wm"},
    {MIME_AUDIO_X_MS_WMA,       "wma"},
    {MIME_AUDIO_X_MS_WAX,       "wax"},
    {MIME_AUDIO_X_GSM,       "gsm"},
    {MIME_AUDIO_X_GSM,       "gsd"},
    {MIME_TEXT_XSL,       "xsl"},
    {MIME_IMAGE_TIFF,       "tif"},
    {MIME_IMAGE_TIFF,       "tiff"},
    {MIME_IMAGE_X_ICON,       "ico"},
    {MIME_IMAGE_X_XPIXMAP,       "xpm"},
    {MIME_IMAGE_X_XBITMAP,       "xbm"},
    {MIME_MODEL_VRML,       "vrm"},
    {MIME_MODEL_VRML,       "vrml"},
    {MIME_MODEL_VRML,       "wrl"},
    {MIME_APPLICATION_X_BINARY_JAVASCRIPT,       "bjs"},
    {MIME_APPLICATION_MSWORD,       "doc"},
    {MIME_APPLICATION_VND_MS_EXCEL,       "xls"},
    {MIME_TEXT_CSV,       "csv"},
    {MIME_APPLICATION_MSPOWERPOINT,       "ppt"},
    {MIME_OMA_DRM_MESSAGE,       "dm"},
    {MIME_OMA_DRM_CONTENT,       "dcf"},
    {MIME_EXTEND_DD,              "dd"},
    {MIME_EXTEND_DD2,              "dd"},
    {MIME_EXTEND_JAD,              "jad"},
    {MIME_EXTEND_APP_JAR,              "jar"},
    {MIME_EXTEND_APP_JAR_ARCHIVE,              "jar"},
    {MIME_EXTEND_APP_X_JAR_ARCHIVE,              "jar"},
    {MIME_EXTEND_APP_SDP,              "sdp"},
    {MIME_EXTEND_APP_X_SDP,              "sdp"},
    {MIME_EXTEND_APP_VND_SDP,              "sdp"},
    {MIME_EXTEND_APPLICATION_VND_SMAF,              "smaf"},
    {MIME_EXTEND_APPLICATION_X_SMAF,              "smaf"},
    {MIME_EXTEND_AUDIO_MP3,              "mp3"},
    {MIME_EXTEND_AUDIO_AAC,              "aac"},
    {MIME_EXTEND_VIDEO_MP4,              "mp4"}
};

/**--------------------------------------------------------------------------*
 **                          EXTERN DECLARE                                *
 **--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_download_app = {0};  //g_download_app的实体定义
static MMIDL_MIME_CALLBACK g_mime_callback[MMIBROWSER_MIME_TYPE_MAX] = {0}; //g_mime_callback的实体定义

extern int stricmp(const char *, const char *);/*lint !e526*/
extern void MMIDL_OMAChangeDownloadTaskFileName(MMIDL_DOWNLOAD_MANAGER_T *dl_manager , uint32 index);


/**-------------------------------------------------------------------------*
**                          GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifndef WIN32
LOCAL uint32 s_mmidl_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
#endif

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : handle download msg
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Create Download Message
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateDownloadMessage(MMIDL_DOWNLOAD_INFO_T *download_info_ptr);

/*****************************************************************************/
//  Description : Destroy Download Param
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyDownloadMessage(MMIDL_DOWNLOAD_INFO_T *download_info_ptr);

/*****************************************************************************/
//  Description : Get Unused Download Task Id
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL int32 GetUnusedDownloadTaskId(MMIDL_DOWNLOAD_INSTANCE_T *download_instance);

/*****************************************************************************/
//  Description : Create Download Task
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void CreateDownloadTask(uint32 task_id,
                                                            MMIDL_DOWNLOAD_INSTANCE_T *download_instance,
                                                            MMIDL_DOWNLOAD_INFO_T *download_info_ptr);

/*****************************************************************************/
//  Description : Create Download Task By Index
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void CreateDownloadTaskByIndex(uint32 task_id,
                                                                            MMIDL_DOWNLOAD_INSTANCE_T *download_instance,
                                                                            uint32 task_index);

/*****************************************************************************/
//  Description : Destory Download Task
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void DestoryDownloadTask(uint32 task_id, MMIDL_DOWNLOAD_INSTANCE_T *download_instance);

/*****************************************************************************/
//  Description : the process message function of the Download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DownloadApplet_HandleEvent(IAPPLET_T* iapplet_ptr, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Create New Download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Download_CreateNew(MMIDL_DOWNLOAD_INFO_T *download_info_ptr);

/*****************************************************************************/
//  Description : Create New Download By Index
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Download_CreateNewByIndex(uint32 task_index);

/*****************************************************************************/
//  Description : Initialize download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Download_Initialize(void);

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size);

/*****************************************************************************/
//  Description : Get Download Task by Context Id
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIDL_DOWNLOAD_TASK_T *GetDownloadTaskByContextId(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, HTTP_CONTEXT_ID_T context_id);

/*****************************************************************************/
//  Description : Get Download Task by Session Id
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMIDL_DOWNLOAD_TASK_T *GetDownloadTaskBySessionId(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, uint32 session_id);

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpInitCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Need Auth Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpNeedAuthInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpRequestIdInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpGetCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Head Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpHeadCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Data Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpDataInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpHeaderInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpErrorInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpCloseCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Cancel Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpCancelInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Net Prog Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpNetProgInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpRedirectInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpPostCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : Handle Http SSL Cert Untrust Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpSSLCertUntrustInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);
#endif

//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODMMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_START_DOWNLOAD_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Start_Download_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_CANCEL_DOWNLOAD_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Cancel_Download_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_CONFIRMATION_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Confirmation_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_AUTH_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Auth_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_PROGRESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Progress_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_SUCCESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Success_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_SUCCESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Header_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_ERROR_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Error_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_GET_SESSION_LIST_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Get_Session_list_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_GET_SESSION_INFO_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Get_Session_Info_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_SUBSCRIBE_INDICATION_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Subscribe_Indication_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_DESUBSCRIBE_INDICATION_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Desubscribe_Indication_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_PROGRESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Manager_Progress_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_SUCCESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Manager_Success_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_ERROR_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Manager_Error_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param);

/*****************************************************************************/

/*****************************************************************************/
//  Description : Handle Download Fail
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleDownloadFail(MMIDL_DOWNLOAD_TASK_T *download_task_ptr);

#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
/*****************************************************************************/
//  Description : Net ConnectEx
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL int32 MMIDL_NetConnectEx(MMIDL_DOWNLOAD_INSTANCE_T *self);

/*****************************************************************************/
// 	Description : process ps signal
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void HandleDLPsMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle download pdp active cnf
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleDLPDPActiveCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle download pdp deactive cnf
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleDLPDPDeactiveCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle download pdp deactive Ind
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleDLPDPDeactiveInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : handle download pdp connect Ind
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleDLPDPConnectInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
//  Description : Handle Http Waiting Task Id
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void DownloadHandleHttpWatingTaskId(uint32 task_id, BOOLEAN is_increase);

/*****************************************************************************/
//  Description : Handle PDP Active Success
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPActiveSuccess(MMIDL_DOWNLOAD_INSTANCE_T *self);

/*****************************************************************************/
//  Description : Handle PDP Active Fail
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPActiveFail(MMIDL_DOWNLOAD_INSTANCE_T *self,BOOLEAN is_prompt);
#endif

/*****************************************************************************/
//  Description : Set applet BG State
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDownloadAppletBGState(BOOLEAN is_bg);

/*****************************************************************************/
//  Description : Set systerm frequency for mmi dl
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL uint32 MMIDL_SetFreq(void);
 
/*****************************************************************************/
//  Description : Get systerm frequency for mmi dl
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/ 
LOCAL uint32 MMIDL_RestoreFreq(void);

/**---------------------------------------------------------------------------*
**                         Constant Variables                                *
**---------------------------------------------------------------------------*/
//applet的数组
LOCAL const CAF_STATIC_APPLET_T s_mmidownload_applet_array[] = 
{
    { GUID_NAME_DEF(SPRD_DOWNLOAD_APPLET_ID), sizeof(DOWNLOAD_APPLET_T), DownloadApplet_HandleEvent, 0, 0 },
};

//applet的信息
LOCAL const CAF_STATIC_APPLET_INFO_T s_mmidownload_applet_info = 
{
    (CAF_STATIC_APPLET_T*)s_mmidownload_applet_array,
    ARR_SIZE(s_mmidownload_applet_array)
};

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : register mmidl module applet info
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void RegAppletInfo(void)
{
    MMI_RegAppletInfo( MMI_MODULE_DL, &s_mmidownload_applet_info );
}

/*****************************************************************************/
//  Description : start download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_StartDownload(MMIDL_INTERFACE_TYPE_E type, DPARAM param)
{
    MMI_APPLET_START_T start ={0};
    MMIDL_APPLET_PARAM_T applet_param = {0};/*lint !e64*/
    DOWNLOAD_APPLET_T* download_applet_ptr = PNULL;

    if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
    {
        MMIPUB_OpenAlertWarningWin(TXT_COMMON_UDISK_USING);
        return;
    }

    if (MMIDL_CREATE_SAVE_AS == type)
    {
        //SCI_TRACE_LOW:"DL_Star mime_type = %d, is_chunk = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_617_112_2_18_2_13_19_0,(uint8*)"dd", ((MMIDL_DOWNLOAD_INFO_T *)param)->mime_type, ((MMIDL_DOWNLOAD_INFO_T *)param)->is_chunk);
        //SCI_TRACE_LOW:"DL_Star status_code = %d, method = %d, buf_len = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_618_112_2_18_2_13_19_1,(uint8*)"ddd", ((MMIDL_DOWNLOAD_INFO_T *)param)->status_code, ((MMIDL_DOWNLOAD_INFO_T *)param)->method, ((MMIDL_DOWNLOAD_INFO_T *)param)->buf_len);
        //SCI_TRACE_LOW:"DL_Star http_header = %x, url = %x, buf = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_619_112_2_18_2_13_19_2,(uint8*)"ddd", ((MMIDL_DOWNLOAD_INFO_T *)param)->http_header, ((MMIDL_DOWNLOAD_INFO_T *)param)->url, ((MMIDL_DOWNLOAD_INFO_T *)param)->buf);

        MMIDL_OpenDownloadSaveAsWin(param);
        return;
    }

    download_applet_ptr = MMIDL_APPLETINSTANCE();

    if (PNULL == download_applet_ptr)
    {
        applet_param.type = type;
        applet_param.param = param;    

        start.guid = SPRD_DOWNLOAD_APPLET_ID;
        start.param_ptr = &applet_param;
        start.size_of_param = sizeof(MMIDL_APPLET_PARAM_T);

        if (PNULL == MMK_StartApplet(&start))
        {
            return;
        }
    }

    MMIDL_TidyExistedWin();

    switch (type)
    {
    case MMIDL_CREATE_NEW:
        CreateDownloadMessage((MMIDL_DOWNLOAD_INFO_T *)param);
        break;

    case MMIDL_OPEN_DOWNLOAD_MANAGER:
        MMIDL_OpenDownloadManagerWin();
        break;

    default:
        break;
    }
}

/*****************************************************************************/
//  Description : Create Download Message
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateDownloadMessage(MMIDL_DOWNLOAD_INFO_T *download_info_ptr)
{
    MMIDL_DOWNLOAD_INFO_T download_info = {0};
    DOWNLOAD_APPLET_T* download_applet_ptr = PNULL;
    MMI_HANDLE_T applet_handle = 0;
    uint32 len = 0;

    SCI_MEMSET(&download_info, 0x00, sizeof(download_info));

    download_info.mime_type = download_info_ptr->mime_type;
    download_info.is_chunk = download_info_ptr->is_chunk;
    download_info.status_code = download_info_ptr->status_code;
    download_info.method = download_info_ptr->method;
    download_info.dual_sys = download_info_ptr->dual_sys;
#ifdef MMI_WIFI_SUPPORT
    if (MMIPDP_INTERFACE_WIFI == download_info_ptr->bearer_type)
    {
        download_info.bearer_type = MMIPDP_INTERFACE_WIFI;
    }
    else
    {
        download_info.bearer_type = MMIPDP_INTERFACE_GPRS;
    }
#else
    download_info.bearer_type = MMIPDP_INTERFACE_GPRS;
#endif

    if (PNULL != download_info_ptr->http_header)
    {
        len = strlen((char *)download_info_ptr->http_header);

        download_info.http_header = SCI_ALLOCAZ(len + 1);
        if(PNULL ==download_info.http_header)
        {
            return FALSE;
        }
        //SCI_MEMSET(download_info.http_header, 0x00, (len + 1));
        SCI_MEMCPY(download_info.http_header, download_info_ptr->http_header, len);
    }
    else
    {
        download_info.http_header = PNULL;
    }

    if (PNULL != download_info_ptr->url)
    {
        len = strlen((char *)download_info_ptr->url);

        download_info.url = SCI_ALLOCAZ(len + 1);
        if(PNULL ==download_info.url)
        {
            if(PNULL != download_info.http_header)
            {   
                SCI_FREE(download_info.http_header);
            }
            return FALSE;
        }
        //SCI_MEMSET(download_info.url, 0x00, (len + 1));
        SCI_MEMCPY(download_info.url, download_info_ptr->url, len);
    }
    else
    {
        download_info.url = PNULL;
    }

    if (PNULL != download_info_ptr->buf)
    {
        len = strlen((char *)download_info_ptr->buf);

        download_info.buf = SCI_ALLOCAZ(len + 1);
        if(PNULL ==download_info.buf)
        {
            if(PNULL != download_info.http_header)
            {   
                SCI_FREE(download_info.http_header);
            }
            if(PNULL != download_info.url)
            {   
                SCI_FREE(download_info.url);
            }
            return FALSE;
        }
        //SCI_MEMSET(download_info.buf, 0x00, (len + 1));
        SCI_MEMCPY(download_info.buf, download_info_ptr->buf, len);
    }
    else
    {
        download_info.buf = PNULL;
    }

    download_applet_ptr = MMIDL_APPLETINSTANCE();
    applet_handle = MMIDL_HANDLE(download_applet_ptr);

    download_applet_ptr->download_instance.pdp_info.bearer_type = download_info.bearer_type;
    download_applet_ptr->download_instance.pdp_info.dual_sys = download_info.dual_sys;

    MMK_PostMsg(applet_handle, MSG_DL_DOWNLOAD_START, &download_info, sizeof(download_info));

    return TRUE;
}

/*****************************************************************************/
//  Description : Destroy Download Param
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DestroyDownloadMessage(MMIDL_DOWNLOAD_INFO_T *download_info_ptr)
{
    if (PNULL == download_info_ptr)
    {
        return FALSE;
    }

    if (PNULL != download_info_ptr->http_header)
    {
        SCI_FREE(download_info_ptr->http_header);
        download_info_ptr->http_header = PNULL;
    }

    if (PNULL != download_info_ptr->url)
    {
        SCI_FREE(download_info_ptr->url);
        download_info_ptr->url = PNULL;
    }

    if (PNULL != download_info_ptr->buf)
    {
        SCI_FREE(download_info_ptr->buf);
        download_info_ptr->buf = PNULL;
    }

    return TRUE;
}

/*****************************************************************************/
//  Description : stop download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_StopDownload(uint32 task_index, BOOLEAN is_all)
{
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;

    dl_manager = MMIDL_GetDownloadManager();
    if (PNULL != dl_manager)
    {
        if (is_all)
        {
            MMIDL_StopDownloadManagerTask(dl_manager, 0, TRUE);
            MMIDL_StopDownload(0, TRUE);
        }
        else
        {
            MMIDL_StopDownloadManagerTask(dl_manager, task_index, FALSE);
            MMIDL_DeleteDownloadTask(task_index);
        }
    }
}

/*****************************************************************************/
//  Description : Init download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_InitDownload(void)
{
    g_download_app.ProcessMsg = HandleDownloadMsg;

    MMIDL_RegMenuGroup();

    MMIDL_InitDownloadTaskStatus();

#ifndef WIN32
    if (CHNG_FREQ_REQ_HANDLER_NULL == s_mmidl_chng_freq_req_handler)
    {
        s_mmidl_chng_freq_req_handler = CHNG_FREQ_GetReqHandler("mmidl");
    }   
#endif
}

/*****************************************************************************/
//  Description : Set systerm frequency for mmi dl
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
LOCAL uint32 MMIDL_SetFreq(void)
{
#ifndef WIN32
    if (CHNG_FREQ_REQ_HANDLER_NULL != s_mmidl_chng_freq_req_handler)
    {
        CHNG_FREQ_SetArmClk(s_mmidl_chng_freq_req_handler, (CHNG_FREQ_INDEX_E)FREQ_INDEX_WWW);       
    }
#endif
    //SCI_TRACE_LOW:"\"\"  MMIDL_SetFreq   \"#"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_848_112_2_18_2_13_19_3,(uint8*)"");
    
    return 1;
}
 
/*****************************************************************************/
//  Description : Get systerm frequency for mmi dl
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/ 
LOCAL uint32 MMIDL_RestoreFreq(void)
{
#ifndef WIN32
     if (CHNG_FREQ_REQ_HANDLER_NULL != s_mmidl_chng_freq_req_handler)
    {
        CHNG_FREQ_RestoreARMClk(s_mmidl_chng_freq_req_handler);
    }       
#endif
    //SCI_TRACE_LOW:"\"\"  MMIDL_RestoreFreq   \"#"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_865_112_2_18_2_13_19_4,(uint8*)"");

    return 1;
}

/*****************************************************************************/
//  Description : register DL module nv len and max item
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_InitModule(void)
{
    MMIDL_RegNv();
    RegAppletInfo();
}

/*****************************************************************************/
//  Description : Create New Download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Download_CreateNew(MMIDL_DOWNLOAD_INFO_T *download_info_ptr)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    int32 unused_id = -1;
    uint32 total_task_num = 0; 
    BOOLEAN is_same_dest_url = 0; 
    uint32 same_task_index = 0; 

    if (PNULL == download_info_ptr)
    {
        return FALSE;
    }

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"Download_CreateNew download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_899_112_2_18_2_13_19_5,(uint8*)"");
        return FALSE;
    }

    //SCI_TRACE_LOW:"Download_CreateNew mime_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_903_112_2_18_2_13_19_6,(uint8*)"d", download_info_ptr->mime_type);

    //Open Download Manager
    MMIDL_OpenDownloadManagerWin();

    if (PNULL == download_info_ptr->url)
    {
        MMIPUB_OpenAlertWarningWin( TXT_DL_DOWNLOAD_URL_EMPTY);
        return FALSE;
    }

    is_same_dest_url = MMIDL_IsSameDownloadDestUrl(download_instance->downoad_manager, (char *)download_info_ptr->url, &same_task_index);

    if (is_same_dest_url)
    {
        MMIDL_DLTaskListSetFocusItem((uint16)same_task_index);

        if(MMIDL_DOWNLOAD_DOWNLOADING == download_instance->downoad_manager->task_info[same_task_index].status)
        {
            MMIPUB_OpenAlertTextWinByTextId(PNULL, TXT_DL_TASK_DOWNLOADING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        }
        else
        {
            MMIDL_OpenDownloadQueryWin(same_task_index);
        }

        return FALSE;//TODO
    }

    total_task_num = MMIDL_GetDownloadManagerTotalTaskNum(download_instance->downoad_manager);

    if (total_task_num >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        //SCI_TRACE_LOW:"Download_CreateNew Exceed Max Task!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_927_112_2_18_2_13_19_7,(uint8*)"");
        MMK_PostMsg(MMIDL_HANDLE(MMIDL_APPLETINSTANCE()), MSG_DL_EXCEED_TASK_NUM, NULL, 0);
        return FALSE;
    }

    unused_id = GetUnusedDownloadTaskId(download_instance);

    if (-1 == unused_id)
    {
        return FALSE;//TODO
    }

    CreateDownloadTask(unused_id, download_instance, download_info_ptr);

    MMIDL_OpenDownloadSaveWin(unused_id);

    return TRUE;
}

/*****************************************************************************/
//  Description : Set applet BG State
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetDownloadAppletBGState(BOOLEAN is_bg)
{
    DOWNLOAD_APPLET_T* download_applet_ptr = PNULL;
    MMI_HANDLE_T applet_handle = 0;

    download_applet_ptr = MMIDL_APPLETINSTANCE();
    applet_handle = MMIDL_HANDLE(download_applet_ptr);

    //SCI_TRACE_LOW:"SetDownloadAppletBGState is_bg=%d,is_freq_change=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_958_112_2_18_2_13_19_8,(uint8*)"dd", is_bg, download_applet_ptr->download_instance.is_freq_change);

    if (is_bg)
    {
        if (!download_applet_ptr->download_instance.is_freq_change)
        {
            download_applet_ptr->download_instance.is_freq_change = TRUE;
            MMIDL_SetFreq();//set freq
        }
    }
    else
    {
        if (download_applet_ptr->download_instance.is_freq_change)
        {
            download_applet_ptr->download_instance.is_freq_change = FALSE;
            MMIDL_RestoreFreq();//restore freq
        }
    }

    MMK_SetAppletState(applet_handle, MMI_APPLET_STATE_BG, is_bg);
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Create New Download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_StartHttpRequest(uint32 task_id)
{
    HTTP_INIT_PARAM_T* init_param_ptr = PNULL;
    HTTP_ERROR_E http_error = HTTP_SUCCESS;
    MMIDL_PDP_INFO_T pdp_info = {0};
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
#ifdef MMI_WIFI_SUPPORT
    MMIWLAN_PROXY_SETTING_DETAIL_T detail_struct = {0};
#endif

    //SCI_TRACE_LOW:"MMIDL_StartHttpRequest task_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_996_112_2_18_2_13_19_9,(uint8*)"d", task_id);

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_StartHttpRequest download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1000_112_2_18_2_13_19_10,(uint8*)"");
        return FALSE;
    }

    SetDownloadAppletBGState(TRUE);

#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
    if (!MMIDL_NetworkIsReady(download_instance))
    {
        //SCI_TRACE_LOW:"MMIDL_StartHttpRequest Network Not Ready"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1009_112_2_18_2_13_19_11,(uint8*)"");

        DownloadHandleHttpWatingTaskId(task_id, TRUE);

        MMIDL_NETCONNECT();//active GPRS

        return FALSE;
    }
#endif

    //Exceed total task num
    if (MMIDL_GetMultiTaskNum() <= download_instance->download_task_num)
    {
        //SCI_TRACE_LOW:"MMIDL_StartHttpRequest multi_task_num=%d,task num=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1022_112_2_18_2_13_19_12,(uint8*)"dd", download_instance->multi_task_num, download_instance->download_task_num);

        MMIDL_ChangeDownloadTaskStatus(download_instance->downoad_manager, download_instance->download_task[task_id].manager_index, MMIDL_DOWNLOAD_WAITING);

        return FALSE;
    }

    init_param_ptr = SCI_ALLOCA(sizeof(HTTP_INIT_PARAM_T));

    if (PNULL == init_param_ptr)
    {
        //SCI_TRACE_LOW:"MMIDL_StartHttpRequest No Memory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1033_112_2_18_2_13_19_13,(uint8*)"");
        return FALSE;
    }

    SCI_MEMSET(init_param_ptr, 0, sizeof(HTTP_INIT_PARAM_T));
    init_param_ptr->is_cookie_enable = FALSE;
    init_param_ptr->is_cache_enable = FALSE;
    init_param_ptr->net_id = (uint32)download_instance->pdp_info.net_id;

    MMIDL_GetPDPInfo(&pdp_info);

    //SCI_TRACE_LOW:"MMIDL_StartHttpRequest gateway=%s, gateway_len=%d, port=%d, bearer_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1045_112_2_18_2_13_19_14,(uint8*)"sddd",pdp_info.connect_setting.gateway, pdp_info.connect_setting.gateway_len, pdp_info.connect_setting.port, pdp_info.bearer_type);

#ifndef WIN32
    if (MMIPDP_INTERFACE_GPRS == pdp_info.bearer_type
        && 0 != pdp_info.connect_setting.gateway_len
        && 0 != strcmp((char *)pdp_info.connect_setting.gateway, (char *)MMIDL_INVALID_GATEWAY))
    {
        init_param_ptr->proxy.is_use_proxy = TRUE;

        init_param_ptr->proxy.user_name_ptr = (char *)pdp_info.connect_setting.username;
        init_param_ptr->proxy.password_ptr = (char *)pdp_info.connect_setting.password;
        init_param_ptr->proxy.proxy_host_ptr = (char *)pdp_info.connect_setting.gateway;
        init_param_ptr->proxy.http_port = pdp_info.connect_setting.port;
    }
#ifdef MMI_WIFI_SUPPORT
    else if(MMIPDP_INTERFACE_WIFI == pdp_info.bearer_type)
    {
        MMIAPIWIFI_GetProxySettingDetail(&detail_struct);

        if(detail_struct.is_use_proxy && !MMIAPIWIFI_IsExistInExcludingList((char *)download_instance->download_task[task_id].download_info.url))
        {
            init_param_ptr->proxy.is_use_proxy = TRUE;

            init_param_ptr->proxy.user_name_ptr = (char *)detail_struct.username;
            init_param_ptr->proxy.password_ptr = (char *)detail_struct.password;
            init_param_ptr->proxy.proxy_host_ptr = (char *)detail_struct.gateway;
            init_param_ptr->proxy.http_port = detail_struct.port;
        }
        else
        {
            init_param_ptr->proxy.is_use_proxy = FALSE;
        }
    }
#endif
    else
#endif
    {
        init_param_ptr->proxy.is_use_proxy = FALSE;
    }

    MMIDL_ChangeDownloadTaskStatus(download_instance->downoad_manager, download_instance->download_task[task_id].manager_index, MMIDL_DOWNLOAD_WAITING_HTTP);

    http_error = HTTP_InitRequest(MMIDL_MODULE_ID, task_id, init_param_ptr);

    SCI_FREE(init_param_ptr);
    //SCI_TRACE_LOW:"MMIDL_StartHttpRequest http_error=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1090_112_2_18_2_13_20_15,(uint8*)"d", http_error);

    if (HTTP_SUCCESS != http_error)
    {
        return FALSE;
    }

    return  TRUE;
}

/*****************************************************************************/
//  Description : Create New Download By Index
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Download_CreateNewByIndex(uint32 task_index)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    int32 unused_id = -1;

    //SCI_TRACE_LOW:"Download_CreateNewByIndex index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1109_112_2_18_2_13_20_16,(uint8*)"d", task_index);

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"Download_CreateNewByIndex download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1113_112_2_18_2_13_20_17,(uint8*)"");
        return FALSE;
    }

    unused_id = GetUnusedDownloadTaskId(download_instance);

    if (-1 == unused_id)
    {
        return FALSE;//TODO
    }

    CreateDownloadTaskByIndex(unused_id, download_instance, task_index);

    MMIDL_StartHttpRequest(unused_id);

    return TRUE;
}

/*****************************************************************************/
//  Description : Initialize download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN Download_Initialize(void)
{
    //SCI_TRACE_LOW:"Download_Initialize"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1137_112_2_18_2_13_20_18,(uint8*)"");
#if 0
    MMIDL_InitDownloadFolder();
    MMIDL_InitDownloadTaskStatus();
#endif
    MMIDL_SetMultiTaskNum();

    return TRUE;
}

/*****************************************************************************/
//  Description : Get Unused Download Task Id
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL int32 GetUnusedDownloadTaskId(MMIDL_DOWNLOAD_INSTANCE_T *download_instance)
{
    int32 unused_id = -1;
    int32 i = 0;

    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (!download_instance->download_task[i].is_used)
        {
            unused_id = i;

            return unused_id;
        }
    }

    return unused_id;
}

/*****************************************************************************/
//  Description : Get Download Applet Instance
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC DOWNLOAD_APPLET_T *MMIDL_GetAppletInstance(void)
{
    DOWNLOAD_APPLET_T* download_applet_ptr = NULL;
    download_applet_ptr = (DOWNLOAD_APPLET_T*)MMK_GetAppletPtr(SPRD_DOWNLOAD_APPLET_ID);

    return download_applet_ptr;
}

/*****************************************************************************/
//  Description : Get Download Instance
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMIDL_DOWNLOAD_INSTANCE_T *MMIDL_GetDownloadInstance(void)
{
    DOWNLOAD_APPLET_T* download_applet_ptr = PNULL;
    download_applet_ptr = MMIDL_APPLETINSTANCE();

    if (PNULL == download_applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIDL_GetDownloadInstance download applet not existed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1190_112_2_18_2_13_20_19,(uint8*)"");

        return PNULL;
    }

    return &download_applet_ptr->download_instance;
}

/*****************************************************************************/
//  Description : the process message function of the Download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E  DownloadApplet_HandleEvent(    
                                                            IAPPLET_T*          iapplet_ptr,
                                                            MMI_MESSAGE_ID_E    msg_id, 
                                                            DPARAM              param
                                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    DOWNLOAD_APPLET_T* download_applet_ptr = (DOWNLOAD_APPLET_T*)iapplet_ptr;

    //SCI_TRACE_LOW:"DownloadApplet_HandleEvent msg_id = 0x%04x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1211_112_2_18_2_13_20_20,(uint8*)"d", msg_id);

    switch (msg_id)
    {
    case MSG_APPLET_START:
        {
            MMIDL_APPLET_PARAM_T *applet_param = (MMIDL_APPLET_PARAM_T *)param;
            MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;

            if (PNULL == applet_param)
            {
                break;
            }

            dl_manager = SCI_ALLOCAZ(sizeof(MMIDL_DOWNLOAD_MANAGER_T));
            if (PNULL == dl_manager)
            {
                MMK_CloseApplet(download_applet_ptr->caf_applet.app_handle);
                return MMI_RESULT_TRUE;
            }
            MMIDL_SetDownloadManager(dl_manager);
            SCI_MEMSET(dl_manager, 0x00, sizeof(MMIDL_DOWNLOAD_MANAGER_T));
            MMIDL_ReadDownloadManager(dl_manager);
            //SCI_TRACE_LOW:"DownloadApplet_HandleEvent type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1234_112_2_18_2_13_20_21,(uint8*)"d", applet_param->type);
            Download_Initialize();
        }
        break;

    case MSG_APPLET_STOP:
            {
                MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;

                MMIDL_StopDownload(0, TRUE);

                dl_manager = MMIDL_GetDownloadManager();
                if (PNULL != dl_manager)
                {
                    SCI_FREE(dl_manager);
                    MMIDL_SetDownloadManager(PNULL);
                }
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
                MMIDL_NETDISCONNECT();
#endif

                if (download_applet_ptr->download_instance.is_freq_change)
                {
                    download_applet_ptr->download_instance.is_freq_change = FALSE;
                    MMIDL_RestoreFreq();//restore freq
                }
            }
        break;

    case MSG_DL_DOWNLOAD_START:
        Download_CreateNew((MMIDL_DOWNLOAD_INFO_T *)param);
        DestroyDownloadMessage((MMIDL_DOWNLOAD_INFO_T *)param);
        break;

    case MSG_DL_EXCEED_TASK_NUM:
        MMIPUB_OpenAlertWarningWin( TXT_DL_TASK_FULL);
        break;

    case MSG_DL_PDP_CONNECT_IND:
        MMIDL_HandlePDP_Change();
        break;
#if 0
    case MSG_DL_PDP_DEACTIVE_IND:
         MMIDL_OpenPDPDeactiveQueryWin();
        break;
#endif

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return (result);
}

/*****************************************************************************/
//  Description : Create Download Task
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void CreateDownloadTask(uint32 task_id,
                                                            MMIDL_DOWNLOAD_INSTANCE_T *download_instance,
                                                            MMIDL_DOWNLOAD_INFO_T *download_info_ptr)
{
    uint16 download_infor_str_len =0;
    
    if (task_id >= MMIDL_MAX_DOWNLOAD_TASK_NUM || PNULL == download_instance || PNULL == download_info_ptr)
    {
        return;
    }

    SCI_MEMSET(&download_instance->download_task[task_id], 0x00, sizeof(MMIDL_DOWNLOAD_TASK_T));

    //URL
    if (PNULL != download_info_ptr->url)
    {
        download_instance->download_task[task_id].download_info.url = SCI_ALLOCAZ(strlen((char *)download_info_ptr->url) + 1);
        if(PNULL == download_instance->download_task[task_id].download_info.url )
        {
            return; //TODO
        }
        download_infor_str_len = SCI_STRLEN((char *)download_info_ptr->url);
        //SCI_MEMSET(download_instance->download_task[task_id].download_info.url, 0x00, (download_infor_str_len + 1));
        SCI_MEMCPY(download_instance->download_task[task_id].download_info.url, download_info_ptr->url, download_infor_str_len + 1);
    }
    else
    {
        return; //TODO
    }

    //http_header
    if (PNULL != download_info_ptr->http_header)
    {
        download_instance->download_task[task_id].download_info.http_header = SCI_ALLOCAZ(strlen((char *)download_info_ptr->http_header) + 1);
        if(PNULL == download_instance->download_task[task_id].download_info.http_header )
        {
            SCI_FREE(download_instance->download_task[task_id].download_info.url);
            return; //TODO
        }
        download_infor_str_len = SCI_STRLEN((char *)download_info_ptr->http_header);
        //SCI_MEMSET(download_instance->download_task[task_id].download_info.http_header, 0x00, (download_infor_str_len + 1));
        SCI_MEMCPY(download_instance->download_task[task_id].download_info.http_header, download_info_ptr->http_header, (download_infor_str_len+ 1));
    }

    //mime_type
    download_instance->download_task[task_id].download_info.mime_type = download_info_ptr->mime_type;

    //is_chunk
    download_instance->download_task[task_id].download_info.is_chunk = download_info_ptr->is_chunk;

    //status_code
    download_instance->download_task[task_id].download_info.status_code = download_info_ptr->status_code;

    //method
    download_instance->download_task[task_id].download_info.method = download_info_ptr->method;

    //bearer
    download_instance->download_task[task_id].download_info.bearer_type = download_info_ptr->bearer_type;

    //dual_sys
    download_instance->download_task[task_id].download_info.dual_sys = download_info_ptr->dual_sys;

    //app_param
    download_instance->download_task[task_id].app_param = task_id;
    
    //module_id
    download_instance->download_task[task_id].module_id = MMIDL_MODULE_ID;

    //download_instance
    download_instance->download_task[task_id].app_instance = (uint32)download_instance;

    //is_old_url
    download_instance->download_task[task_id].is_old_url = FALSE;

    //is_used
    download_instance->download_task[task_id].is_used = TRUE;

    //is_old_url
    download_instance->download_task[task_id].is_need_reload = FALSE;
}

/*****************************************************************************/
//  Description : Create Download Task By Index
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void CreateDownloadTaskByIndex(uint32 task_id,
                                                                            MMIDL_DOWNLOAD_INSTANCE_T *download_instance,
                                                                            uint32 task_index)
{
    if (task_id >= MMIDL_MAX_DOWNLOAD_TASK_NUM || PNULL == download_instance || task_index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    SCI_TRACE_LOW("DestoryDownloadTask task_id = %d, manager_index = %d ", task_id, task_index);

    SCI_MEMSET(&download_instance->download_task[task_id], 0x00, sizeof(MMIDL_DOWNLOAD_TASK_T));

    //manager_index
    download_instance->download_task[task_id].manager_index = task_index;

    //app_param
    download_instance->download_task[task_id].app_param = task_id;
    
    //module_id
    download_instance->download_task[task_id].module_id = MMIDL_MODULE_ID;

    //download_instance
    download_instance->download_task[task_id].app_instance = (uint32)download_instance;

    //is_old_url
    download_instance->download_task[task_id].is_old_url = TRUE;

    //is_used
    download_instance->download_task[task_id].is_used = TRUE;

    download_instance->download_task[task_id].is_need_reload = FALSE;
}

/*****************************************************************************/
//  Description : Destory Download Task
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_DestoryDownloadTask(uint32 task_id, MMIDL_DOWNLOAD_INSTANCE_T *download_instance)
{
    if (PNULL == download_instance)
    {
        return;
    }

    DestoryDownloadTask(task_id, download_instance);
}

/*****************************************************************************/
//  Description : Destory Download Task
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void DestoryDownloadTask(uint32 task_id, MMIDL_DOWNLOAD_INSTANCE_T *download_instance)
{
    HTTP_CONTEXT_ID_T context_id = 0;

    if (task_id >= MMIDL_MAX_DOWNLOAD_TASK_NUM || PNULL == download_instance)
    {
        return;
    }

    //SCI_TRACE_LOW:"DestoryDownloadTask task_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1434_112_2_18_2_13_20_22,(uint8*)"d", task_id);

    if (download_instance->download_task[task_id].is_used)
    {
        //URL
        if (PNULL != download_instance->download_task[task_id].download_info.url)
        {
            SCI_FREE(download_instance->download_task[task_id].download_info.url);
            download_instance->download_task[task_id].download_info.url = PNULL;
        }

        //http_header
        if (PNULL != download_instance->download_task[task_id].download_info.http_header)
        {
            SCI_FREE(download_instance->download_task[task_id].download_info.http_header);
            download_instance->download_task[task_id].download_info.http_header = PNULL;
        }

#ifdef MMIDL_MMI_BUFFER_SUPPORT
        if (0 != download_instance->download_task[task_id].data_buf)
        {
            SCI_FREE(download_instance->download_task[task_id].data_buf);
            download_instance->download_task[task_id].data_buf = PNULL;
        }
#endif

        context_id = download_instance->download_task[task_id].context_id;

        SCI_MEMSET(&download_instance->download_task[task_id].download_info, 0x00, sizeof(MMIDL_DOWNLOAD_INFO_T));
        SCI_MEMSET(&download_instance->download_task[task_id], 0x00, sizeof(MMIDL_DOWNLOAD_TASK_T));

        download_instance->download_task[task_id].context_id = context_id;
        
        //is_used
        download_instance->download_task[task_id].is_used = FALSE;
    }
}

/*****************************************************************************/
//  Description : Handle ODM_START_DOWNLOAD_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Start_Download_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_START_DOWNLOAD_CNF_MSG_T dl_cnf_msg = {0};
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    uint32 task_num = 0;
    int32 unused_id = -1;

    GetSignalStruct(param, &dl_cnf_msg, sizeof(ODM_START_DOWNLOAD_CNF_MSG_T));

    task_num = MMIDL_GetDownloadManagerTotalTaskNum(download_instance->downoad_manager);
    unused_id = GetUnusedDownloadTaskId(download_instance);

    if (task_num >= MMIDL_MAX_DOWNLOAD_TASK_NUM || -1 == unused_id)
    {

        //SCI_TRACE_LOW:"HandleODM_Start_Download_Cnf Exceed Max Task!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1493_112_2_18_2_13_21_23,(uint8*)"");

        MMIPUB_OpenAlertWarningWin( TXT_DL_TASK_FULL);
        ODM_CancelDownload(dl_cnf_msg.session_id);
    }
    else
    {
        download_task_ptr = &download_instance->download_task[unused_id];
        download_task_ptr->is_oma_dl = TRUE;
        download_task_ptr->session_id = dl_cnf_msg.session_id;
        download_task_ptr->app_param = unused_id;
        download_task_ptr->module_id = MMIDL_MODULE_ID;
        download_task_ptr->app_instance = (uint32)download_instance;
        download_task_ptr->is_old_url = FALSE;
        download_task_ptr->is_used = TRUE;
    }

    //SCI_TRACE_LOW:"HandleODM_Start_Download_Cnf magic = %d session_id = %d ret = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1510_112_2_18_2_13_21_24,(uint8*)"ddd", dl_cnf_msg.magic, dl_cnf_msg.session_id, dl_cnf_msg.result);

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_CANCEL_DOWNLOAD_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Cancel_Download_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_CANCEL_DOWNLOAD_CNF_MSG_T dl_cnf_msg = {0};

    GetSignalStruct(param, &dl_cnf_msg, sizeof(ODM_CANCEL_DOWNLOAD_CNF_MSG_T));

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_CONFIRMATION_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Confirmation_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    MMIDL_OMA_DD_INFO_T dd_info = {0};
    int32 i = 0;
    uint16 uri_len = 0;

    SCI_MEMCPY(&dd_info, param, sizeof(MMIDL_OMA_DD_INFO_T)); /* task_id is set to session_id*/
    download_task_ptr = GetDownloadTaskBySessionId(download_instance, dd_info.task_id);
    //SCI_TRACE_LOW:"HandleODM_Download_Confirmation_Ind session id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1551_112_2_18_2_13_21_25,(uint8*)"d", dd_info.task_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_TRUE;
    }
    //URL
    if (0 != (*dd_info.dd_content.object_uri))
    {
        uri_len = strlen((char *)dd_info.dd_content.object_uri);
        download_task_ptr->download_info.url = SCI_ALLOCAZ(uri_len + 1);
        if(PNULL == download_task_ptr->download_info.url)
        {
            return MMI_RESULT_TRUE; //TODO
        }
        //SCI_MEMSET(download_task_ptr->download_info.url, 0x00, uri_len + 1);
        SCI_MEMCPY(download_task_ptr->download_info.url, dd_info.dd_content.object_uri, uri_len);
    }
    else
    {
        ODM_CancelDownload(dd_info.task_id);
        MMIDL_DestoryDownloadTask(download_task_ptr->app_param, &(((DOWNLOAD_APPLET_T *)MMIDL_APPLETINSTANCE())->download_instance));

        //SCI_TRACE_LOW:"HandleODM_Download_Confirmation_Ind Uri is incorrect"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1574_112_2_18_2_13_21_26,(uint8*)"");
    }
    download_task_ptr->download_info.mime_type = MIME_TYPE_UNKNOWN;
    dd_info.task_id = download_task_ptr->app_param;
    //SCI_TRACE_LOW:"HandleODM_Download_Confirmation_Ind task id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1578_112_2_18_2_13_21_27,(uint8*)"d", download_task_ptr->app_param);
    for (i = 0; i < ODM_DD_TYPE_MAX_NUM; i++)
    {
        if (PNULL != dd_info.dd_content.type[i])
        {
            download_task_ptr->download_info.mime_type = mime_to_uint(dd_info.dd_content.type[i]);
            //SCI_TRACE_LOW:"HandleODM_Download_Confirmation_Ind type = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1584_112_2_18_2_13_21_28,(uint8*)"s", dd_info.dd_content.type[i]);
            //SCI_TRACE_LOW:"HandleODM_Download_Confirmation_Ind mime_type id = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1585_112_2_18_2_13_21_29,(uint8*)"d", download_task_ptr->download_info.mime_type);
            break;
        }
    }

    MMIDL_OpenDDPropertyWin(&dd_info);
    MMIDL_UpdateDownloadManagerWin();
    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_AUTH_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Auth_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_DOWNLOAD_AUTH_IND_MSG_T auth_msg = {0};

    GetSignalStruct(param, &auth_msg, sizeof(ODM_DOWNLOAD_AUTH_IND_MSG_T));

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_PROGRESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Progress_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    ODM_DOWNLOAD_PROGRESS_IND_MSG_T pro_msg = {0};

    GetSignalStruct(param, &pro_msg, sizeof(ODM_DOWNLOAD_PROGRESS_IND_MSG_T));
    download_task_ptr = GetDownloadTaskBySessionId(download_instance, pro_msg.session_id);
    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_TRUE;
    }
	//SCI_TRACE_LOW:"HandleODM_Download_Progress_Ind param =%d, session_id = %d, stage = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1633_112_2_18_2_13_21_30,(uint8*)"ddd", download_task_ptr->app_param, pro_msg.session_id, pro_msg.stage);

    switch(pro_msg.stage) {
    case ODM_DUA_STAGE_INIT:
        /* Cann't get download_task_ptr */
    	break;

    case ODM_DUA_STAGE_DOWNLOAD_DD:
    	break;

    case ODM_DUA_STAGE_PARSE_DD:
    	break;

    case ODM_DUA_STAGE_CAPABILITY_CHECK:
    	break;

    case ODM_DUA_STAGE_DOWNLOAD_MO:
        download_task_ptr->cur_len += pro_msg.current_len;
        if (download_task_ptr->total_len == 0 && pro_msg.content_len != 0)
        {
            download_task_ptr->total_len = pro_msg.content_len;
            MMIDL_ChangeDownloadTaskStatus(download_instance->downoad_manager, download_task_ptr->manager_index, MMIDL_DOWNLOAD_DOWNLOADING);
            MMIDL_ChangeDownloadTaskTotalLen(download_instance->downoad_manager, download_task_ptr->manager_index, download_task_ptr->total_len);

        }

        MMIDL_ChangeDownloadTaskCurLen(download_instance->downoad_manager, download_task_ptr->manager_index, download_task_ptr->cur_len);
        MMIDL_UpdateDownloadManagerWin();
    	break;

    case ODM_DUA_STAGE_INSTALL_MO:
    	break;

    case ODM_DUA_STAGE_REPORT:
    	break;

    case ODM_DUA_STAGE_COMPLETED:
    	break;

    default:
    	break;
    }

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_SUCCESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Header_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_DOWNLOAD_HEADER_IND_MSG_T header_msg = {0};
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    uint32  mime_type = 0;

    GetSignalStruct(param, &header_msg, sizeof(ODM_DOWNLOAD_HEADER_IND_MSG_T));
    download_task_ptr = GetDownloadTaskBySessionId(download_instance, header_msg.session_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    mime_type = MMIDL_GetMimeTypeByhttpHeader((const uint8 *)header_msg.header_ptr);

    if (MIME_TYPE_UNKNOWN != mime_type)
    {
        download_task_ptr->mime_type = mime_type;

        MMIDL_ChangeDownloadTaskMimeType(download_instance->downoad_manager, download_task_ptr->manager_index, mime_type);
    }
    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_SUCCESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Success_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_DOWNLOAD_SUCCESS_IND_MSG_T succ_msg = {0};
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    GetSignalStruct(param, &succ_msg, sizeof(ODM_DOWNLOAD_SUCCESS_IND_MSG_T));
    download_task_ptr = GetDownloadTaskBySessionId(download_instance, succ_msg.session_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_TRUE;
    }

    MMIDL_ChangeDownloadTaskStatus(download_instance->downoad_manager, download_task_ptr->manager_index, MMIDL_DOWNLOAD_DOWNLOADED);
    MMIDL_OMAChangeDownloadTaskFileName(download_instance->downoad_manager, download_task_ptr->manager_index);

    MMIDL_UpdateDownloadManagerWin();
    MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_SUCCESS);//added for cr240847
    MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, TRUE);
    DestoryDownloadTask(download_task_ptr->app_param, download_instance);

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_ERROR_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
#ifdef WRE_SUPPORT
extern PUBLIC BOOLEAN WRE_IsFocusRunWin(void);
#endif
LOCAL MMI_RESULT_E HandleODM_Download_Error_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_DOWNLOAD_ERROR_IND_MSG_T err_msg = {0};
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};

    GetSignalStruct(param, &err_msg, sizeof(ODM_DOWNLOAD_ERROR_IND_MSG_T));
    //SCI_TRACE_LOW:"HandleODM_Download_Error_Ind err_id = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1732_112_2_18_2_13_21_31,(uint8*)"d", err_msg.error_no);

    download_task_ptr = GetDownloadTaskBySessionId(download_instance, err_msg.session_id);
    dl_manager = MMIDL_GetDownloadManager();

    if (PNULL == download_task_ptr || PNULL == dl_manager)
    {
        return MMI_RESULT_TRUE;
    }
    
    //SCI_TRACE_LOW:"HandleODM_Download_Error_Ind manager_index=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1742_112_2_18_2_13_21_32,(uint8*)"d", download_task_ptr->manager_index);
    if(-1 != download_task_ptr->manager_index)//added for cr237035
    {
        MMIDL_GetManagerTaskInfo(dl_manager, download_task_ptr->manager_index, &task_info);

        //SCI_TRACE_LOW:"HandleODM_Download_Error_Ind task_info.url=%s"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1748_112_2_18_2_13_21_33,(uint8*)"s", (char*)task_info.url);
        MMIAPIFMM_DeleteFile((const wchar *)task_info.file, PNULL);
        MMIDL_DeleteDownloadManagerTask(dl_manager, download_task_ptr->manager_index, FALSE);
        MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_GENERAL);//added for cr240847
        MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);
    }
    /*added for cr237035 begin*/
    else
    {
        MMI_TEXT_ID_T string_dl_status_id = TXT_NULL;
        MMI_STRING_T string_dl_status = {0};
        MMI_TEXT_ID_T status_info_id = TXT_NULL;
        MMI_STRING_T status_info = {0};
        BOOLEAN sound_notify = FALSE;

        switch (err_msg.error_no)
        {
        case ERR_ODM_LOSS_OF_SERVICE:
        case ERR_ODM_LOADER_ERROR:
            string_dl_status_id = TXT_COMMON_CANNOT_DOWNLOAD;
            status_info_id = TXT_COMMON_CONTENT_MISMATCH;
            break;

        case ERR_ODM_ATTRIBUTE_MISMATCH:
        case ERR_ODM_NON_ACCEPTABLE_CONTENT:
            string_dl_status_id = TXT_COMMON_CANNOT_DOWNLOAD;
            status_info_id = TXT_COMMON_UNSUPPORT_FILE_TYPE;
            break;

        case ERR_ODM_INVALID_DD:
        case ERR_ODM_INVALID_DD_VERSION:
        case ERR_ODM_INVALID_DEVICE_ABORTED:
            string_dl_status_id = TXT_COMMON_CANNOT_DOWNLOAD;
            status_info_id = TXT_COMMON_INVALID_DESCRIPTOR;
            break;

        default:
            string_dl_status_id = TXT_DOWNLOAD_FAILED;
            status_info_id = TXT_NULL;
            break;
        }

        MMI_GetLabelTextByLang(string_dl_status_id, &string_dl_status);
        MMI_GetLabelTextByLang(status_info_id, &status_info);

        if (MMK_IsOpenWin(MMIDL_DOWNLOAD_ALERT_WIN_ID))
        {
            //SCI_TRACE_LOW:"HandleODM_Download_Error_Ind close MMIDL_DOWNLOAD_ALERT_WIN_ID"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1764_112_2_18_2_13_21_34,(uint8*)"");
            MMK_CloseWin(MMIDL_DOWNLOAD_ALERT_WIN_ID);
        }
        MMIPUB_OpenAlertWinByTextPtr(PNULL, &string_dl_status, &status_info, IMAGE_PUBWIN_WARNING, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
        MMIDEFAULT_TurnOnBackLight();

        sound_notify = (0 == MMIAPIENVSET_GetActiveModeOptValue(MN_DUAL_SYS_1, DOWNLOAD_NOTIFY));
        if (sound_notify)
        {
            if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
            {
                //SCI_TRACE_LOW:"MMISMS: PlayNewSMSRing call MMIAPICC_IsInCallConnected"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1775_112_2_18_2_13_21_35,(uint8*)"");
                MMIAPISET_PlayRing(MN_DUAL_SYS_1, FALSE, 0, 1, MMISET_RING_TYPE_MSG_IN_CALL,PNULL);
            }
            else if (MMIAPIUDISK_UdiskIsRun() 
#ifdef JAVA_SUPPORT
			        || MMIAPIJAVA_IsFocusJAVARunWin() 
#endif
#ifdef WRE_SUPPORT
			        ||WRE_IsFocusRunWin()
#endif
			        || MMIAPIUdisk_IsOpenPcCamera()
#ifdef STREAMING_HS_SUPPORT
                    || MMIAPISMP_IsStreamPlaying()
#endif /* STREAMING_HS_SUPPORT */ 
                    )
            {
                MMIAPISET_StopAllVibrator();
                MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, FALSE, MMIDL_VIBRA_TIMEROUT);        
            }
            else
            {
                MMIAPISET_PlayRing(MN_DUAL_SYS_1, FALSE, 0, 1, MMISET_RING_TYPE_DL, PNULL);
            }
        }//if (sound_notify)
        
    }//if(-1 != download_task_ptr->manager_index)
    /*added for cr237035 end*/

    DestoryDownloadTask(download_task_ptr->app_param, download_instance);
    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_GET_SESSION_LIST_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Get_Session_list_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_DOWNLOAD_GET_SESSION_LIST_CNF_MSG_T msg = {0};

    GetSignalStruct(param, &msg, sizeof(ODM_DOWNLOAD_GET_SESSION_LIST_CNF_MSG_T));

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DOWNLOAD_GET_SESSION_INFO_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Download_Get_Session_Info_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_DOWNLOAD_GET_SESSION_INFO_CNF_MSG_T msg = {0};

    GetSignalStruct(param, &msg, sizeof(ODM_DOWNLOAD_GET_SESSION_INFO_CNF_MSG_T));

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_SUBSCRIBE_INDICATION_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Subscribe_Indication_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_SUBSCRIBE_INDICATION_CNF_MSG_T msg = {0};

    GetSignalStruct(param, &msg, sizeof(ODM_SUBSCRIBE_INDICATION_CNF_MSG_T));

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_DESUBSCRIBE_INDICATION_CNF
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Desubscribe_Indication_Cnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_DESUBSCRIBE_INDICATION_CNF_MSG_T msg = {0};

    GetSignalStruct(param, &msg, sizeof(ODM_DESUBSCRIBE_INDICATION_CNF_MSG_T));

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_PROGRESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Manager_Progress_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_MANAGER_PROGRESS_IND_MSG_T msg = {0};

    GetSignalStruct(param, &msg, sizeof(ODM_MANAGER_PROGRESS_IND_MSG_T));

    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_SUCCESS_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Manager_Success_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_MANAGER_SUCCESS_IND_MSG_T msg = {0};

    GetSignalStruct(param, &msg, sizeof(ODM_MANAGER_SUCCESS_IND_MSG_T)); 
    
    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle ODM_MANAGER_ERROR_IND
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODM_Manager_Error_Ind(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
#ifdef OMADL_SUPPORT
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    ODM_MANAGER_ERROR_IND_MSG_T msg = {0};

    GetSignalStruct(param, &msg, sizeof(ODM_MANAGER_ERROR_IND_MSG_T));
     
    return result;
#else
    return MMI_RESULT_FALSE;
#endif
}

/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleODMMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    DOWNLOAD_APPLET_T* download_applet_ptr = PNULL;
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = PNULL;

    download_applet_ptr = MMIDL_APPLETINSTANCE();

    if (PNULL == download_applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    download_instance = &download_applet_ptr->download_instance;

    switch (msg_id)
    {
    case ODM_START_DOWNLOAD_CNF:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_START_DOWNLOAD_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1958_112_2_18_2_13_22_36,(uint8*)"");
        result = HandleODM_Start_Download_Cnf(download_instance, param);
        break;

    case ODM_CANCEL_DOWNLOAD_CNF:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_CANCEL_DOWNLOAD_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1963_112_2_18_2_13_22_37,(uint8*)"");
        result = HandleODM_Cancel_Download_Cnf(download_instance, param);
        break;

    case ODM_DOWNLOAD_CONFIRMATION_IND:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_DOWNLOAD_CONFIRMATION_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1968_112_2_18_2_13_22_38,(uint8*)"");
        result = HandleODM_Download_Confirmation_Ind(download_instance, param);
        break;

    case ODM_DOWNLOAD_AUTH_IND:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_DOWNLOAD_AUTH_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1973_112_2_18_2_13_22_39,(uint8*)"");
        result = HandleODM_Download_Auth_Ind(download_instance, param);
        break;

    case ODM_DOWNLOAD_PROGRESS_IND:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_DOWNLOAD_PROGRESS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1978_112_2_18_2_13_22_40,(uint8*)"");
        result = HandleODM_Download_Progress_Ind(download_instance, param);
        break;

    case ODM_DOWNLOAD_HEADER_IND:
        SCI_TRACE_LOW("HandleODMMsg receive ODM_DOWNLOAD_HEADER_IND");
        result = HandleODM_Download_Header_Ind(download_instance, param);
        break;

    case ODM_DOWNLOAD_SUCCESS_IND:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_DOWNLOAD_SUCCESS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1983_112_2_18_2_13_22_41,(uint8*)"");
        result = HandleODM_Download_Success_Ind(download_instance, param);
        break;

    case ODM_DOWNLOAD_ERROR_IND:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_DOWNLOAD_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1988_112_2_18_2_13_22_42,(uint8*)"");
        result = HandleODM_Download_Error_Ind(download_instance, param);
        break;

    case ODM_DOWNLOAD_GET_SESSION_LIST_CNF:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_DOWNLOAD_GET_SESSION_LIST_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1993_112_2_18_2_13_22_43,(uint8*)"");
        result = HandleODM_Download_Get_Session_list_Cnf(download_instance, param);
        break;

    case ODM_DOWNLOAD_GET_SESSION_INFO_CNF:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_DOWNLOAD_GET_SESSION_INFO_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_1998_112_2_18_2_13_22_44,(uint8*)"");
        result = HandleODM_Download_Get_Session_Info_Cnf(download_instance, param);
        break;

    case ODM_SUBSCRIBE_INDICATION_CNF:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_START_DOWNLOAD_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2003_112_2_18_2_13_22_45,(uint8*)"");
        result = HandleODM_Subscribe_Indication_Cnf(download_instance, param);
        break;

    case ODM_DESUBSCRIBE_INDICATION_CNF:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_DESUBSCRIBE_INDICATION_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2008_112_2_18_2_13_22_46,(uint8*)"");
        result = HandleODM_Desubscribe_Indication_Cnf(download_instance, param);
        break;

    case ODM_MANAGER_PROGRESS_IND:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_MANAGER_PROGRESS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2013_112_2_18_2_13_22_47,(uint8*)"");
        result = HandleODM_Manager_Progress_Ind(download_instance, param);
        break;

    case ODM_MANAGER_SUCCESS_IND:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_MANAGER_SUCCESS_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2018_112_2_18_2_13_22_48,(uint8*)"");
        result = HandleODM_Manager_Success_Ind(download_instance, param);
        break;

    case ODM_MANAGER_ERROR_IND:
        //SCI_TRACE_LOW:"HandleODMMsg receive ODM_MANAGER_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2023_112_2_18_2_13_22_49,(uint8*)"");
        result = HandleODM_Manager_Error_Ind(download_instance, param);
        break;

    default:
        //SCI_TRACE_LOW:"HandleODMMsg Not handle msg_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2028_112_2_18_2_13_22_50,(uint8*)"d", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }
    
    return result;
}


/*****************************************************************************/
//  Description : handle download msg
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleDownloadMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        result = HandleHttpMsg(app_ptr, msg_id, param);
    }
    else if (ODM_SIGNAL_START <= msg_id && msg_id <= ODM_SIGNAL_END)
    {
        result = HandleODMMsg(app_ptr, msg_id, param);
    }
    else
    {
        result = MMI_RESULT_FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Get Signal Struct
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void GetSignalStruct(DPARAM param, void *signal_struct_ptr, uint16 struct_size)
{
    if (PNULL == param || PNULL == signal_struct_ptr || struct_size < sizeof(xSignalHeaderRec))
    {
        return;
    }
    
    SCI_MEMCPY((uint8 *)((uint32)signal_struct_ptr), param, (struct_size));
}

/*****************************************************************************/
//  Description : Get Download Task by Context Id
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMIDL_DOWNLOAD_TASK_T *GetDownloadTaskByContextId(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, HTTP_CONTEXT_ID_T context_id)
{
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    int32 i = 0;

    if (PNULL == download_instance)
    {
        return PNULL;
    }

    for(i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (!download_instance->download_task[i].is_oma_dl)
        {
            if (context_id == download_instance->download_task[i].context_id)
            {
                download_task_ptr = &download_instance->download_task[i];
                break;
            }
        }
    }

    return download_task_ptr;
}

/*****************************************************************************/
//  Description : Get Download Task by Session Id
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMIDL_DOWNLOAD_TASK_T *GetDownloadTaskBySessionId(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, uint32 session_id)
{
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    int32 i = 0;

    if (PNULL == download_instance)
    {
        return PNULL;
    }

    for(i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (download_instance->download_task[i].is_oma_dl)
        {
            if (session_id == download_instance->download_task[i].session_id)
            {
                download_task_ptr = &download_instance->download_task[i];
                break;
            }
        }
    }

    return download_task_ptr;
}

/*****************************************************************************/
//  Description : Handle Http Init Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpInitCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_INIT_CNF_SIG_T signal = {0};
    HTTP_APP_MODULE_ID_T module_id = 0;
    HTTP_APP_PARAM_T app_param = 0;
    HTTP_CONTEXT_ID_T context_id = 0;
    uint32 app_instance = 0;
    HTTP_GET_PARAM_T* get_param_ptr = PNULL;
    uint32 len = 0;
    MMIENG_UA_T ua = {0};
    MMIDL_PDP_INFO_T pdp_info = {0};
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
    uint16 full_file_len = 0;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_INIT_CNF_SIG_T));

    module_id = signal.module_id;
    app_param = signal.app_param;
    context_id = signal.context_id;

    if (MMIDL_MODULE_ID != module_id || MMIDL_MAX_DOWNLOAD_TASK_NUM <= app_param)
    {
        //SCI_TRACE_LOW:"HandleHttpInitCnf not download task"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2165_112_2_18_2_13_22_51,(uint8*)"");

        return MMI_RESULT_FALSE;
    }

    if (app_param != download_instance->download_task[app_param].app_param
        || !download_instance->download_task[app_param].is_used
        || ((uint32)download_instance != download_instance->download_task[app_param].app_instance))
    {
        //SCI_TRACE_LOW:"HandleHttpInitCnf not this download instance"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2174_112_2_18_2_13_22_52,(uint8*)"");

        return MMI_RESULT_FALSE;
    }

    app_instance = (uint32)download_instance;
    download_instance->download_task[app_param].context_id = context_id;

    get_param_ptr = SCI_ALLOCAZ(sizeof(HTTP_GET_PARAM_T));

    if(PNULL == get_param_ptr)
    {
        //SCI_TRACE_LOW:"HandleHttpInitCnf no memory"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2186_112_2_18_2_13_22_53,(uint8*)"");

        return MMI_RESULT_FALSE;
    }

    //SCI_MEMSET(get_param_ptr, 0x00, sizeof(HTTP_GET_PARAM_T));

    get_param_ptr->accept.accept_ptr = MMIDL_ACCEPT_STRING;

    //20110527 begin
    get_param_ptr->accept_language.accept_language_ptr = MMIDL_ACCEPT_LANGUAGE_STRING;
    //20110527 end

    MMIAPIENG_GetUa(&ua);
    MMIDL_GetPDPInfo(&pdp_info);

    if (0 != SCI_STRLEN((char *)ua.user_agent))
    {
        get_param_ptr->user_agent.user_agent_ptr = (char *)ua.user_agent;
    }
    else
    {
        get_param_ptr->user_agent.user_agent_ptr = (char *)MMIAPICOM_GetUserAgent();
    }


    //get_param_ptr->ua_profile.ua_profile_ptr = (char *)ua.uaprofile;

    get_param_ptr->connection = HTTP_CONNECTION_CLOSE;

    MMIDL_GetManagerTaskInfo(download_instance->downoad_manager, download_instance->download_task[app_param].manager_index, &task_info);

    if (!download_instance->download_task[app_param].is_old_url)
    {
        get_param_ptr->uri.uri_ptr = (char *)download_instance->download_task[app_param].download_info.url;
    }
    else
    {
        char begin_bytes[MMIDL_RANGE_HEADER_LEN] = {0};

        download_instance->download_task[app_param].total_len = task_info.total_len;
        download_instance->download_task[app_param].cur_len = task_info.cur_len;

        get_param_ptr->uri.uri_ptr = (char *)task_info.url;

        get_param_ptr->extern_header.header_num = 1;
        get_param_ptr->extern_header.other_header_ptr = SCI_ALLOCAZ(sizeof(HTTP_OTHER_HEADER_T));
        //SCI_MEMSET(get_param_ptr->extern_header.other_header_ptr, 0, sizeof(HTTP_OTHER_HEADER_T));
        if(PNULL ==  get_param_ptr->extern_header.other_header_ptr)
        {
            return MMI_RESULT_FALSE; //TODO
        }

        len = strlen("Range");
        get_param_ptr->extern_header.other_header_ptr->header_name_ptr = SCI_ALLOCAZ(len + 1);
        //SCI_MEMSET(get_param_ptr->extern_header.other_header_ptr->header_name_ptr, 0, (len + 1));
        if(PNULL ==  get_param_ptr->extern_header.other_header_ptr->header_name_ptr)
        {
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
            return MMI_RESULT_FALSE; //TODO
        }
        strcpy(get_param_ptr->extern_header.other_header_ptr->header_name_ptr, "Range");

        sprintf(begin_bytes, "%d-", task_info.cur_len);
        len = strlen(MMIDL_RANGE_HEADER_PREFIX) + strlen(begin_bytes);
        get_param_ptr->extern_header.other_header_ptr->header_value_ptr = SCI_ALLOCAZ(len + 1);
        //SCI_MEMSET(get_param_ptr->extern_header.other_header_ptr->header_value_ptr, 0, (len + 1));
        if(PNULL ==  get_param_ptr->extern_header.other_header_ptr->header_value_ptr)
        {
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr->header_name_ptr);
            return MMI_RESULT_FALSE; //TODO
        }
        strcpy(get_param_ptr->extern_header.other_header_ptr->header_value_ptr, MMIDL_RANGE_HEADER_PREFIX);
        strcat(get_param_ptr->extern_header.other_header_ptr->header_value_ptr, begin_bytes);
    }

    get_param_ptr->accept_encoding = HTTP_ENCODING_TYPE_NULL;
    get_param_ptr->get_data.style = HTTP_DATA_TRANS_STYLE_FILE;

    get_param_ptr->get_data.is_header_use_file = FALSE;
    get_param_ptr->need_net_prog_ind = TRUE;

    full_file_len = MMIAPICOM_Wstrlen(task_info.file);
    full_file_len = MIN(HTTP_MAX_FILE_NAME_LEN, full_file_len) + 1;
    SCI_MEMCPY(get_param_ptr->get_data.content_file_name, task_info.file, full_file_len * sizeof(HTTP_UNICODE_T));

#ifndef WIN32
    if (MMIPDP_INTERFACE_GPRS == pdp_info.bearer_type
        && 0 != pdp_info.connect_setting.gateway_len
        && 0 != strcmp((char *)pdp_info.connect_setting.gateway, (char *)MMIDL_INVALID_GATEWAY))
    {
        get_param_ptr->is_use_relative_url = FALSE;
    }
    else
#endif
    {
        get_param_ptr->is_use_relative_url = TRUE;
    }

    HTTP_GetRequest(context_id, get_param_ptr, app_instance);

    if (PNULL != get_param_ptr->extern_header.other_header_ptr)
    {
        if (PNULL != get_param_ptr->extern_header.other_header_ptr->header_name_ptr)
        {
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr->header_name_ptr);
        }

        if (PNULL != get_param_ptr->extern_header.other_header_ptr->header_value_ptr)
        {
            SCI_FREE(get_param_ptr->extern_header.other_header_ptr->header_value_ptr);
        }

        SCI_FREE(get_param_ptr->extern_header.other_header_ptr);
    }

    SCI_FREE(get_param_ptr);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Need Auth Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpNeedAuthInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_AUTH_IND_SIG_T signal = {0};
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    HTTP_CONTEXT_ID_T context_id = 0;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_AUTH_IND_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    MMIDL_OpenDownloadAuthWin((uint32)download_task_ptr->app_param);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Request Id Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpRequestIdInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_REQUEST_ID_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    HTTP_REQUEST_ID_T request_id = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_REQUEST_ID_IND_SIG_T));

    context_id = signal.context_id;
    request_id = signal.request_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    download_task_ptr->request_id = request_id;

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Get Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpGetCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_GET_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    //MMIDL_MIME_CALLBACK_PARAM_T callback_param = {0};
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
    int32 temp_manager_index = 0;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_GET_CNF_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (0 < download_instance->download_task_num && download_task_ptr->is_downloading)
    {
        download_instance->download_task_num--;
    }

    SCI_TRACE_LOW("HandleHttpGetCnf manager_index = %d result = %d", download_task_ptr->manager_index, signal.result);

    if (HTTP_SUCCESS != signal.result)
    {
        MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_HTTP);
        MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);

        HandleDownloadFail(download_task_ptr);

        MMIDL_UpdateDownloadManagerWin();

        DestoryDownloadTask(download_task_ptr->app_param, download_instance);
        HTTP_CloseRequest(context_id);

        MMIDL_CheckNextDownloadTask(download_instance->downoad_manager);
        if (!MMIDL_IsNeedDownLoadTask(download_instance->downoad_manager))
        {
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
            MMIDL_NETDISCONNECT();
#endif

            SetDownloadAppletBGState(FALSE);
        }
    }
    else
    {
        MMIDL_ChangeDownloadTaskStatus(download_instance->downoad_manager, download_task_ptr->manager_index, MMIDL_DOWNLOAD_DOWNLOADED);
        MMIDL_ChangeDownloadTaskFileName(download_instance->downoad_manager, download_task_ptr->manager_index);

        MMIDL_UpdateDownloadManagerWin();
        MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_SUCCESS);//added for cr240847
        MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, TRUE);
        MMIDL_GetManagerTaskInfo(download_instance->downoad_manager, download_task_ptr->manager_index, &task_info);
        #if 0
        MMI_WSTRNCPY(callback_param.file_name, MMIFILE_FULL_PATH_MAX_LEN , task_info.file, MMIFILE_FULL_PATH_MAX_LEN, MMIAPICOM_Wstrlen(task_info.file));
        callback_param.mime_type = task_info.mime_type;//download_task_ptr->mime_type;
        callback_param.url = (uint8 *)task_info.url;
        callback_param.user_data = (void *)download_task_ptr->manager_index;
        callback_param.download_result = TRUE;
        //SCI_TRACE_LOW:"HandleHttpGetCnf mime_type is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2422_112_2_18_2_13_23_54,(uint8*)"d",callback_param.mime_type);
        MMIDL_MimeCallBack(&callback_param);
        #endif
        temp_manager_index = download_task_ptr->manager_index;//added for cr242594
        DestoryDownloadTask(download_task_ptr->app_param, download_instance);
        HTTP_CloseRequest(context_id);

            if (MIME_TYPE_APPLICATION_VND_OMA_DD == task_info.mime_type)
        {
            MMIDL_StartOMADownload(download_instance, temp_manager_index);
            download_task_ptr->manager_index = -1;//added for cr237035
        }
        
        MMIDL_CheckNextDownloadTask(download_instance->downoad_manager);

        if (!MMIDL_IsNeedDownLoadTask(download_instance->downoad_manager) && MIME_TYPE_APPLICATION_VND_OMA_DD != task_info.mime_type)
        {
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
            MMIDL_NETDISCONNECT();
#endif

            SetDownloadAppletBGState(FALSE);
        }

    }
    return result;
}

/*****************************************************************************/
//  Description : Handle Http Head Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpHeadCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_HEAD_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_HEAD_CNF_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    HTTP_CloseRequest(context_id);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Data Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpDataInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_DATA_IND_SIG_T signal = {0};
    HTTP_REQUEST_ID_T request_id = 0;
    HTTP_CONTEXT_ID_T context_id = 0;
    uint32 response_code = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    SFS_ERROR_E error = SFS_NO_ERROR;
    uint32 bytes_written = 0;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_DATA_IND_SIG_T));

    request_id = signal.request_id;
    context_id = signal.context_id;
    response_code = signal.response_code;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (MMIDL_HTTP_STATUS_CODE_SUCCESS == response_code
        || MMIDL_HTTP_STATUS_CODE_PARTIAL_CONTENT == response_code
        || MMIDL_HTTP_STATUS_CODE_MOVED_PERMANENTLY == response_code
        || MMIDL_HTTP_STATUS_CODE_FOUND == response_code
        || MMIDL_HTTP_STATUS_CODE_TEMPORARY_REDIRECT == response_code
        || MMIDL_HTTP_STATUS_CODE_UNAUTHORIZED == response_code
        || MMIDL_HTTP_STATUS_CODE_UTHENTICATION_REQUIRED == response_code)
    {
        // Alloc buffer for save data
#ifdef MMIDL_MMI_BUFFER_SUPPORT
        if (PNULL == download_task_ptr->data_buf)
        {
            download_task_ptr->max_buf_len = MMIDL_MAX_DL_BUF_SIZE;
            download_task_ptr->data_buf = SCI_ALLOCAZ(download_task_ptr->max_buf_len);
            download_task_ptr->buf_len = 0;
        }

        if (PNULL == download_task_ptr->data_buf)
        {
            MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_NO_MEMORY);//added for cr240847
            MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);
            
            MMIDL_StopDownload(download_task_ptr->app_param, FALSE);

            HandleDownloadFail(download_task_ptr);
        }
        else if (download_task_ptr->is_downloading)
        {
            BOOLEAN is_file_exist = FALSE;
            MMIFILE_HANDLE file_handle = 0;

            file_handle = MMIDL_CreateDownloadFile(download_task_ptr, &is_file_exist);

            if (0 == download_task_ptr->buf_len)
            {
                SCI_MEMSET(download_task_ptr->data_buf, 0x00, download_task_ptr->max_buf_len);
            }

            //SCI_TRACE_LOW:"HandleHttpDataInd buf_len=%d, data_len=%d, cur_len=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2552_112_2_18_2_13_23_55,(uint8*)"ddd", download_task_ptr->buf_len, signal.data_len, download_task_ptr->cur_len);

            if (download_task_ptr->buf_len + signal.data_len >= download_task_ptr->max_buf_len)//one block full,save data
            {
                if (0 != file_handle)
                {
                    error = MMIAPIFMM_WriteFile(file_handle,
                                                            download_task_ptr->data_buf,
                                                            download_task_ptr->buf_len,
                                                            &bytes_written,
                                                            PNULL);
                }
                else
                {
                    error = SFS_ERROR_NO_SPACE;
                }

                if (SFS_NO_ERROR == error)
                {
                    download_task_ptr->cur_len += download_task_ptr->buf_len;
                    MMIDL_ChangeDownloadTaskCurLen(download_instance->downoad_manager, download_task_ptr->manager_index, download_task_ptr->cur_len);
                    MMIDL_UpdateDownloadManagerWin();
                }

                SCI_MEMSET(download_task_ptr->data_buf, 0x00, download_task_ptr->max_buf_len);
                download_task_ptr->buf_len = 0;
            }

            SCI_MEMCPY(download_task_ptr->data_buf + download_task_ptr->buf_len, signal.data_ptr, MIN(signal.data_len, download_task_ptr->max_buf_len));
            download_task_ptr->buf_len += MIN(signal.data_len, download_task_ptr->max_buf_len);

            if (download_task_ptr->cur_len + download_task_ptr->buf_len >= download_task_ptr->total_len && (SFS_NO_ERROR == error))//data receive complete
            {
                if (0 != file_handle)
                {
                    error = MMIAPIFMM_WriteFile(file_handle,
                                                            download_task_ptr->data_buf,
                                                            download_task_ptr->buf_len,
                                                            &bytes_written,
                                                            PNULL);
                }
                else
                {
                    error = SFS_ERROR_NO_SPACE;
                }

                if (SFS_NO_ERROR == error)
                {
                    download_task_ptr->cur_len += download_task_ptr->buf_len;
                    MMIDL_ChangeDownloadTaskCurLen(download_instance->downoad_manager, download_task_ptr->manager_index, download_task_ptr->cur_len);
                    MMIDL_UpdateDownloadManagerWin();
                }
            }

            MMIAPIFMM_CloseFile(file_handle);
        
            if (SFS_NO_ERROR != error && is_file_exist)
            {
                MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_NO_SPACE);//added for cr240847
                MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);

                MMIPUB_OpenAlertWarningWin( TXT_COMM_NO_SPACE);

                MMIDL_StopDownload(download_task_ptr->app_param, FALSE);

                HandleDownloadFail(download_task_ptr);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"HandleHttpDataInd is_downloading=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2622_112_2_18_2_13_23_56,(uint8*)"d", download_task_ptr->is_downloading);
        }
#else
        if (download_task_ptr->is_downloading)
        {
            BOOLEAN is_file_exist = FALSE;
            MMIFILE_HANDLE file_handle = 0;

            file_handle = MMIDL_CreateDownloadFile(download_task_ptr, &is_file_exist);

            //SCI_TRACE_LOW:"HandleHttpDataInd data_len=%d, file_handle=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2632_112_2_18_2_13_23_57,(uint8*)"dd", signal.data_len, file_handle);

            if (0 != file_handle)
            {
                error = MMIAPIFMM_WriteFile(file_handle,
                                                        signal.data_ptr,
                                                        signal.data_len,
                                                        &bytes_written,
                                                        PNULL);

                MMIAPIFMM_CloseFile(file_handle);
            }
            else
            {
                error = SFS_ERROR_NO_SPACE;
            }
        
            if (SFS_NO_ERROR == error)
            {
                download_task_ptr->cur_len += signal.data_len;
                MMIDL_ChangeDownloadTaskCurLen(download_instance->downoad_manager, download_task_ptr->manager_index, download_task_ptr->cur_len);
                MMIDL_UpdateDownloadManagerWin();
            }
            else if (is_file_exist)
            {
                MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_NO_SPACE);//added for cr240847
                MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);

                MMIPUB_OpenAlertWarningWin( TXT_COMM_NO_SPACE);

                MMIDL_StopDownload(download_task_ptr->app_param, FALSE);

                HandleDownloadFail(download_task_ptr);
            }
        }
        else
        {
            //SCI_TRACE_LOW:"HandleHttpDataInd is_downloading=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2669_112_2_18_2_13_23_58,(uint8*)"d", download_task_ptr->is_downloading);
        }
#endif
    }
    else
    {
        MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_HTTP);//added for cr240847
        MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);

        MMIDL_StopDownload(download_task_ptr->app_param, FALSE);

        HandleDownloadFail(download_task_ptr);
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Header Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpHeaderInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_HEADER_IND_SIG_T signal = {0};
    HTTP_REQUEST_ID_T request_id = 0;
    HTTP_CONTEXT_ID_T context_id = 0;
    uint32 app_instance = 0;
    uint32 response_code = 0;
    int32 content_len = 0;
    BOOLEAN is_valid = TRUE;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_HEADER_IND_SIG_T));

    request_id = signal.request_id;
    context_id = signal.context_id;
    app_instance = signal.app_instance;
    response_code = signal.rsp_header_info.response_code;
    content_len = signal.rsp_header_info.content_length;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //SCI_TRACE_LOW:"HandleHttpHeaderInd response_code=%d, content_len=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2722_112_2_18_2_13_24_59,(uint8*)"dd", response_code, content_len);

    SCI_TRACE_LOW("HandleHttpHeaderInd manager_index = %d", download_task_ptr->manager_index);

    if (MMIDL_HTTP_STATUS_CODE_MOVED_PERMANENTLY == response_code
        || MMIDL_HTTP_STATUS_CODE_FOUND == response_code
        || MMIDL_HTTP_STATUS_CODE_TEMPORARY_REDIRECT == response_code
        || MMIDL_HTTP_STATUS_CODE_UNAUTHORIZED == response_code
//        || MMIDL_HTTP_STATUS_CODE_NOT_FOUND == response_code //CR137619
        || MMIDL_HTTP_STATUS_CODE_UTHENTICATION_REQUIRED == response_code)
    {
        return MMI_RESULT_TRUE;
    }

    if (content_len > 0 && download_task_ptr->total_len < (uint32)content_len)
    {
        MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
        wchar device_name[MMIFILE_DEVICE_NAME_MAX_LEN+1] = {0};
        uint16 device_name_len = 0;
        uint32 space_high = 0;
        uint32 space_low = 0;

        download_task_ptr->total_len = content_len;

        MMIDL_ChangeDownloadTaskTotalLen(download_instance->downoad_manager, download_task_ptr->manager_index, download_task_ptr->total_len);

        MMIDL_GetManagerTaskInfo(download_instance->downoad_manager, download_task_ptr->manager_index, &task_info);

        if (MMIAPIFMM_SplitFullPath((const wchar *)task_info.file, MMIAPICOM_Wstrlen((const wchar *)task_info.file),
                                            device_name, &device_name_len,
                                            PNULL, PNULL,
                                            PNULL, PNULL))
        {
            if (MMIAPIFMM_GetDeviceFreeSpace(device_name, device_name_len, &space_high, &space_low))
            {
                if (0 == space_high && space_low < (uint32)content_len)
                {
                    //SCI_TRACE_LOW:"HandleHttpHeaderInd space_high=%d, space_low=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2756_112_2_18_2_13_24_60,(uint8*)"dd", space_high, space_low);
                    is_valid = FALSE;//空间不足
                }
            }
            else
            {
                is_valid = FALSE;
            }
        }
        else
        {
            is_valid = FALSE;
        }
    }

    //SCI_TRACE_LOW:"HandleHttpHeaderInd is_valid=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_2771_112_2_18_2_13_24_61,(uint8*)"d", is_valid);

    if (is_valid)
    {
        if (MMIDL_HTTP_STATUS_CODE_SUCCESS == response_code
            || MMIDL_HTTP_STATUS_CODE_PARTIAL_CONTENT == response_code)
        {
            if (MMIDL_GetMultiTaskNum() > download_instance->download_task_num)
            {
                download_instance->download_task[download_task_ptr->app_param].is_downloading = TRUE;
                download_instance->download_task_num++;
                MMIDL_ChangeDownloadTaskStatus(download_instance->downoad_manager, download_task_ptr->manager_index, MMIDL_DOWNLOAD_DOWNLOADING);//Start downloading

            }
            else
            {
                MMIDL_ChangeDownloadTaskStatus(download_instance->downoad_manager, download_task_ptr->manager_index, MMIDL_DOWNLOAD_WAITING);

                MMIDL_StopDownload(download_task_ptr->app_param, FALSE);
            }
        }
        else //TODO
        {
            MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_HTTP);//added for cr240847
            MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);

            HandleDownloadFail(download_task_ptr);

            MMIDL_StopDownload(download_task_ptr->app_param, FALSE);
        }    
    }
    else
    {
        MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_NO_SPACE);//added for cr240847
        MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);

        MMIPUB_OpenAlertWarningWin( TXT_COMM_NO_SPACE);

        MMIDL_StopDownload(download_task_ptr->app_param, FALSE);

        HandleDownloadFail(download_task_ptr);
    }

    MMIDL_UpdateDownloadManagerWin();

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Error Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpErrorInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_ERROR_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_ERROR_IND_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (0 < download_instance->download_task_num && download_task_ptr->is_downloading)
    {
        download_instance->download_task_num--;
    }

    context_id = signal.context_id;

    SCI_TRACE_LOW("HandleHttpErrorInd manager_index = %d", download_task_ptr->manager_index);

    MMIDL_SetDownloadResult(download_task_ptr->manager_index, MMIDL_DOWNLOAD_ERROR_HTTP);//added for cr240847
    MMIDL_NotifyDownloadComplete(download_task_ptr->manager_index, FALSE);

    HandleDownloadFail(download_task_ptr);

    MMIDL_UpdateDownloadManagerWin();

    DestoryDownloadTask(download_task_ptr->app_param, download_instance);
    HTTP_CloseRequest(context_id);

    MMIDL_CheckNextDownloadTask(download_instance->downoad_manager);
    if (!MMIDL_IsNeedDownLoadTask(download_instance->downoad_manager))
    {
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
        MMIDL_NETDISCONNECT();
#endif

        SetDownloadAppletBGState(FALSE);
    }
    return result;
}

/*****************************************************************************/
//  Description : Handle Http Close Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpCloseCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_CLOSE_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_CLOSE_CNF_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    download_task_ptr->context_id = 0;

    //DestoryDownloadTask(download_task_ptr->app_param, download_instance);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Cancel Cnf
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpCancelInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_CANCEL_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
#ifndef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
#endif
    uint32 manager_index = 0;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_CANCEL_CNF_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    manager_index = download_task_ptr->manager_index;

    SCI_TRACE_LOW("HandleHttpCancelInd manager_index = %d, index = %d", download_task_ptr->manager_index, manager_index);

    if (download_task_ptr->is_need_reload)
    {
        MMIDL_ResumeDownload(download_instance->downoad_manager, manager_index);
        download_task_ptr->is_need_reload = FALSE;
    }
    else
    {//CR137619 :Cancel task 与delete task不一样。
         if (MMIDL_DOWNLOAD_NONE != download_instance->downoad_manager->task_info[manager_index].status)
         {
#ifndef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
            if (MMIDL_DOWNLOAD_DOWNLOADED != download_instance->downoad_manager->task_info[manager_index].status)
#else
            if (MMIDL_DOWNLOAD_PAUSED != download_instance->downoad_manager->task_info[manager_index].status
            && MMIDL_DOWNLOAD_DOWNLOADED != download_instance->downoad_manager->task_info[manager_index].status)
#endif
	 
            {
	#ifndef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
				MMIDL_GetManagerTaskInfo(download_instance->downoad_manager, manager_index, &task_info);

				MMIAPIFMM_DeleteFile((const wchar *)task_info.file, PNULL);
				MMIDL_ChangeDownloadTaskTotalLen(download_instance->downoad_manager, manager_index, 0);
				MMIDL_ChangeDownloadTaskCurLen(download_instance->downoad_manager, manager_index, 0);
	#endif
				MMIDL_DeleteDownloadManagerTask(download_instance->downoad_manager, manager_index, FALSE);//cr246515
            }
        }
        DestoryDownloadTask(download_task_ptr->app_param, download_instance);
        if (!MMIDL_IsNeedDownLoadTask(download_instance->downoad_manager))
        {
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
            MMIDL_NETDISCONNECT();
#endif
            SetDownloadAppletBGState(FALSE);
        }
    }

    HTTP_CloseRequest(context_id);

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Net Prog Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpNetProgInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_NET_PROG_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    NET_PROGRESS_E net_prog_op_code = NET_PROGRESS_CONNECTING;
    uint32 content_len = 0;
    uint32 current_len = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_NET_PROG_IND_SIG_T));

    context_id = signal.context_id;
    net_prog_op_code = signal.net_prog_op_code;
    content_len = signal.content_len;
    current_len = signal.current_len;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (NET_PROGRESS_RECEIVING != net_prog_op_code)
    {
        return result;
    }

    if (0 == download_task_ptr->total_len)
    {
        download_task_ptr->total_len = content_len;
        MMIDL_ChangeDownloadTaskTotalLen(download_instance->downoad_manager, download_task_ptr->manager_index, download_task_ptr->total_len);
    }
    download_task_ptr->cur_len += current_len;
    MMIDL_ChangeDownloadTaskCurLen(download_instance->downoad_manager, download_task_ptr->manager_index, download_task_ptr->cur_len);
    MMIDL_UpdateDownloadManagerWin();

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpRedirectInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_REDIRECT_IND_SIG_T signal = {0};
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;
    HTTP_CONTEXT_ID_T context_id = 0;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_REDIRECT_IND_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : Handle Http Redirect Ind
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpPostCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_POST_CNF_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_POST_CNF_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    HTTP_CloseRequest(context_id);

    return result;
}

#ifdef SSL_SUPPORT
/*****************************************************************************/
//  Description : Handle Http SSL Cert Untrust Ind
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpSSLCertUntrustInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    HTTP_SSL_CERT_UNTRUST_IND_SIG_T signal = {0};
    HTTP_CONTEXT_ID_T context_id = 0;
    MMIDL_DOWNLOAD_TASK_T *download_task_ptr = PNULL;

    if (PNULL == download_instance)
    {
        return MMI_RESULT_FALSE;
    }

    GetSignalStruct(param, &signal, sizeof(HTTP_SSL_CERT_UNTRUST_IND_SIG_T));

    context_id = signal.context_id;

    download_task_ptr = GetDownloadTaskByContextId(download_instance, context_id);

    if (PNULL == download_task_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    result = HTTP_SslCertResponse(context_id, download_task_ptr->app_instance, download_task_ptr->request_id, TRUE);

    return result;
}
#endif

/*****************************************************************************/
//  Description : Auth Response for Task id
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC HTTP_ERROR_E MMIDL_AuthResponse(uint32 task_id, MMI_STRING_T *account_str_ptr, MMI_STRING_T *password_str_ptr)
{
    HTTP_ERROR_E http_error = HTTP_SUCCESS;
    HTTP_AUTH_PARAM_T *auth_ptr = PNULL;
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    HTTP_REQUEST_ID_T request_id = 0;
    HTTP_CONTEXT_ID_T context_id = 0;
    uint32 app_instance = 0;

    if (PNULL == account_str_ptr || PNULL == password_str_ptr)
    {
        return HTTP_ERROR_INVALID_INPUT;
    }

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_AuthResponse download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3136_112_2_18_2_13_24_62,(uint8*)"");
        return HTTP_ERROR_INVALID_INPUT;
    }

    request_id = download_instance->download_task[task_id].request_id;
    context_id = download_instance->download_task[task_id].context_id;
    app_instance = download_instance->download_task[task_id].app_instance;

    auth_ptr = SCI_ALLOCA(sizeof(HTTP_AUTH_PARAM_T));
    SCI_MEMSET(auth_ptr, 0, sizeof(HTTP_AUTH_PARAM_T));
    auth_ptr->user_name_ptr = SCI_ALLOCA(account_str_ptr->wstr_len + 1);
    SCI_MEMSET(auth_ptr->user_name_ptr, 0, (account_str_ptr->wstr_len+1));
    MMI_WSTRNTOSTR((uint8 *)auth_ptr->user_name_ptr,
                                account_str_ptr->wstr_len,
                                account_str_ptr->wstr_ptr,
                                account_str_ptr->wstr_len,
                                account_str_ptr->wstr_len);
     
    auth_ptr->password_ptr = SCI_ALLOCA(password_str_ptr->wstr_len + 1);
    SCI_MEMSET(auth_ptr->password_ptr, 0, (password_str_ptr->wstr_len + 1));

    MMI_WSTRNTOSTR((uint8 *)auth_ptr->password_ptr,
                                password_str_ptr->wstr_len,
                                password_str_ptr->wstr_ptr,
                                password_str_ptr->wstr_len,
                                password_str_ptr->wstr_len);

    http_error = HTTP_AuthResponse(context_id, auth_ptr, app_instance, request_id);

    if (HTTP_SUCCESS != http_error)
    {
        MMIDL_StopDownload(task_id, FALSE);
        MMIPUB_OpenAlertWarningWin(TXT_DL_DOWNLOAD_AUTH_FAIL);
    }

    SCI_FREE(auth_ptr->user_name_ptr);
    SCI_FREE(auth_ptr->password_ptr);
    SCI_FREE(auth_ptr);

    return http_error;
}

/*****************************************************************************/
//  Description : Handle Http Msg
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    DOWNLOAD_APPLET_T* download_applet_ptr = PNULL;
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = PNULL;

    download_applet_ptr = MMIDL_APPLETINSTANCE();

    if (PNULL == download_applet_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    download_instance = &download_applet_ptr->download_instance;

    switch (msg_id)
    {
    case HTTP_SIG_INIT_CNF:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_INIT_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3200_112_2_18_2_13_25_63,(uint8*)"");
        result = HandleHttpInitCnf(download_instance, param);
        break;

    case HTTP_SIG_NEED_AUTH_IND:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_NEED_AUTH_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3205_112_2_18_2_13_25_64,(uint8*)"");
        result = HandleHttpNeedAuthInd(download_instance, param);
        break;

    case HTTP_SIG_REQUEST_ID_IND:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_REQUEST_ID_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3210_112_2_18_2_13_25_65,(uint8*)"");
        result = HandleHttpRequestIdInd(download_instance, param);
        break;

    case HTTP_SIG_GET_CNF:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_GET_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3215_112_2_18_2_13_25_66,(uint8*)"");
        result = HandleHttpGetCnf(download_instance, param);
        break;

    case HTTP_SIG_HEAD_CNF:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_HEAD_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3220_112_2_18_2_13_25_67,(uint8*)"");
        result = HandleHttpHeadCnf(download_instance, param);
        break;

    case HTTP_SIG_DATA_IND:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_DATA_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3225_112_2_18_2_13_25_68,(uint8*)"");
        result = HandleHttpDataInd(download_instance, param);
        break;

    case HTTP_SIG_HEADER_IND:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_HEADER_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3230_112_2_18_2_13_25_69,(uint8*)"");
        result = HandleHttpHeaderInd(download_instance, param);
        break;

    case HTTP_SIG_ERROR_IND:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_ERROR_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3235_112_2_18_2_13_25_70,(uint8*)"");
        result = HandleHttpErrorInd(download_instance, param);
        break;

    case HTTP_SIG_CLOSE_CNF:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_CLOSE_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3240_112_2_18_2_13_25_71,(uint8*)"");
        result = HandleHttpCloseCnf(download_instance, param);
        break;

    case HTTP_SIG_CANCEL_CNF:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_CANCEL_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3245_112_2_18_2_13_25_72,(uint8*)"");
        result = HandleHttpCancelInd(download_instance, param);
        break;

    case HTTP_SIG_NET_PROG_IND:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_NET_PROG_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3250_112_2_18_2_13_25_73,(uint8*)"");
        result = HandleHttpNetProgInd(download_instance, param);
        break;

    case HTTP_SIG_REDIRECT_IND:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_REDIRECT_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3255_112_2_18_2_13_25_74,(uint8*)"");
        result = HandleHttpRedirectInd(download_instance, param);
        break;

    case HTTP_SIG_POST_CNF:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_POST_CNF"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3260_112_2_18_2_13_25_75,(uint8*)"");
        result = HandleHttpPostCnf(download_instance, param);
        break;

#ifdef SSL_SUPPORT
    case HTTP_SIG_SSL_CERT_UNTRUST_IND:
        //SCI_TRACE_LOW:"HandleHttpMsg receive HTTP_SIG_SSL_CERT_UNTRUST_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3266_112_2_18_2_13_25_76,(uint8*)"");
        result = HandleHttpSSLCertUntrustInd(download_instance, param);
        break;
#endif

    default:
        //SCI_TRACE_LOW:"HandleHttpMsg Not handle msg_id = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3272_112_2_18_2_13_25_77,(uint8*)"d", msg_id);
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : Delete Download Task
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_DeleteDownloadTask(uint32 task_index)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    uint32 i =0;

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_DeleteDownloadTask download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3291_112_2_18_2_13_25_78,(uint8*)"");
        return;
    }

    for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
    {
        if (download_instance->download_task[i].is_used
            && task_index == (uint32)download_instance->download_task[i].manager_index)
        {
            MMIDL_StopDownload(i, FALSE);
        }
    }
}

/*****************************************************************************/
//  Description : Set download manager task index
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetDownloadManagerTaskIndex(uint32 index, uint32 task_index)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM || task_index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_SetDownloadManagerTaskIndex download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3320_112_2_18_2_13_25_79,(uint8*)"");
        return;
    }

    download_instance->download_manager_task_index[index] = task_index;
}

/*****************************************************************************/
//  Description : get download manager task index
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDL_GetDownloadManagerTaskIndex(uint32 index)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

    if (index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return 0;
    }

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_GetDownloadManagerTaskIndex download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3342_112_2_18_2_13_25_80,(uint8*)"");
        return 0;
    }

    return download_instance->download_manager_task_index[index];
}

/*****************************************************************************/
//  Description : Pause Download
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_PauseDownload(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 task_index)
{
    if (task_index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    MMIDL_ChangeDownloadTaskStatus(dl_manager, task_index, MMIDL_DOWNLOAD_PAUSED);

    MMIDL_DeleteDownloadTask(task_index);
}

/*****************************************************************************/
//  Description : Resume Download
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ResumeDownload(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 task_index)
{
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};

    if (task_index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    MMIDL_GetManagerTaskInfo(dl_manager, task_index, &task_info);

    if (MMIAPIFMM_IsFileExist((const wchar *)task_info.file, (uint16)MMIAPICOM_Wstrlen((const wchar *)task_info.file)))//exist file
    {
        Download_CreateNewByIndex(task_index);
        MMIDL_ChangeDownloadTaskStatus(dl_manager, task_index, MMIDL_DOWNLOAD_WAITING);
    }
    else
    {
        MMIDL_ReDownload(dl_manager, task_index);
    }
}

/*****************************************************************************/
//  Description : Re Download
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_ReDownload(MMIDL_DOWNLOAD_MANAGER_T *dl_manager, uint32 task_index)
{
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
    wchar file_name[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};

    if (task_index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    MMIDL_ChangeFileNameToTmpFile(dl_manager, task_index);

    MMIDL_GetManagerTaskInfo(dl_manager, task_index, &task_info);

    MMIDL_GetFileNameWithoutTemp(dl_manager, task_index, file_name, MMIFILE_FULL_PATH_MAX_LEN);
    if (MMIAPIFMM_IsFileExist(file_name, MMIFILE_FULL_PATH_MAX_LEN))
    {
        MMIAPIFMM_DeleteFile(file_name, PNULL);
    }
    if (MMIAPIFMM_IsFileExist(task_info.file, MMIFILE_FULL_PATH_MAX_LEN))
    {
        MMIAPIFMM_DeleteFile(task_info.file, PNULL);
    }

    MMIDL_ChangeDownloadTaskTotalLen(dl_manager, task_index, 0);
    MMIDL_ChangeDownloadTaskCurLen(dl_manager, task_index, 0);
    MMIDL_ChangeDownloadTaskStatus(dl_manager, task_index, MMIDL_DOWNLOAD_WAITING);

    Download_CreateNewByIndex(task_index);
}

/*****************************************************************************/
//  Description : stop download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_StopDownload(uint32 task_id, BOOLEAN is_all)
{
    DOWNLOAD_APPLET_T* download_applet_ptr = PNULL;
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = PNULL;
    MMIDL_MIME_CALLBACK_PARAM_T callback_param = {0};

    if (task_id >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIDL_StopDownload task_id=%d, is_all=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3441_112_2_18_2_13_25_81,(uint8*)"dd", task_id, is_all);

    download_applet_ptr = MMIDL_APPLETINSTANCE();

    if (PNULL == download_applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIDL_StopDownload No Download Task"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3447_112_2_18_2_13_25_82,(uint8*)"");
        return;
    }

    download_instance = &download_applet_ptr->download_instance;
    

    if (!is_all)
    {
        if (download_instance->download_task[task_id].is_used)
        {
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
            DownloadHandleHttpWatingTaskId(task_id, FALSE);
#endif
            if (download_instance->download_task[task_id].is_oma_dl)
            {
#ifdef OMADL_SUPPORT
                ODM_CancelDownload(download_instance->download_task[task_id].session_id);
#endif
            }
            else
            {
                HTTP_CancelRequest(download_instance->download_task[task_id].context_id,
                                                download_instance->download_task[task_id].app_instance,
                                                download_instance->download_task[task_id].request_id);
            }
            if (0 < download_instance->download_task_num && download_instance->download_task[task_id].is_downloading)
            {
                download_instance->download_task[task_id].is_downloading = FALSE;

                download_instance->download_task_num--;

                MMIDL_CheckNextDownloadTask(download_instance->downoad_manager);
            }
            
            callback_param.mime_type = download_instance->download_task[task_id].download_info.mime_type;;//download_task_ptr->mime_type;
            callback_param.download_result = FALSE;
            callback_param.result_code = MMIDL_DOWNLOAD_ERROR_STOP;//added for cr240847
            MMIDL_MimeCallBack(&callback_param);
            
        }
    }
    else
    {
        uint32 i = 0;

        for (i = 0; i < MMIDL_MAX_DOWNLOAD_TASK_NUM; i++)
        {
            if (download_instance->download_task[i].is_used)
            {
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
                DownloadHandleHttpWatingTaskId(i, FALSE);
#endif
                HTTP_CancelRequest(download_instance->download_task[i].context_id,
                                                download_instance->download_task[i].app_instance,
                                                download_instance->download_task[i].request_id);
                
                if (0 < download_instance->download_task_num && download_instance->download_task[i].is_downloading)
                {
                    download_instance->download_task[i].is_downloading = FALSE;

                    download_instance->download_task_num--;
                }
                
                callback_param.mime_type = download_instance->download_task[i].download_info.mime_type;//download_task_ptr->mime_type;
                callback_param.download_result = FALSE;
                callback_param.result_code = MMIDL_DOWNLOAD_ERROR_STOP;//added for cr240847
                MMIDL_MimeCallBack(&callback_param);
            }
        }
    }
    

}

/*****************************************************************************/
//  Description : stop download applet
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_StopDownloadApplet(void)
{
    DOWNLOAD_APPLET_T* download_applet_ptr = PNULL;
    MMI_HANDLE_T applet_handle = PNULL;

    //SCI_TRACE_LOW:"MMIDL_StopDownloadApplet "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3531_112_2_18_2_13_25_83,(uint8*)"");

    download_applet_ptr = MMIDL_APPLETINSTANCE();
    if (PNULL == download_applet_ptr)
    {
        //SCI_TRACE_LOW:"MMIDL_StopDownloadApplet No Download Task"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3536_112_2_18_2_13_25_84,(uint8*)"");
        return;
    }

    applet_handle = MMIDL_HANDLE(download_applet_ptr);
    MMK_CloseApplet(applet_handle);
}

/*****************************************************************************/
//  Description : Handle Download Fail
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleDownloadFail(MMIDL_DOWNLOAD_TASK_T *download_task_ptr)
{
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;
#ifndef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
    MMIDL_DOWNLOAD_MANAGER_INFO_T task_info = {0};
#endif

    dl_manager = MMIDL_GetDownloadManager();

    if (PNULL == download_task_ptr || PNULL == dl_manager)
    {
        return;
    }

#ifdef MMIDL_RESUME_BROKEN_TRANSFER_SUPPORT
    MMIDL_ChangeDownloadTaskStatus(dl_manager, download_task_ptr->manager_index, MMIDL_DOWNLOAD_FAILED);
#else
//delete file
    MMIDL_GetManagerTaskInfo(dl_manager, download_task_ptr->manager_index, &task_info);

    MMIAPIFMM_DeleteFile((const wchar *)task_info.file, PNULL);

    //delete task
    MMIDL_DeleteDownloadManagerTask(dl_manager, download_task_ptr->manager_index, FALSE);
#endif
}

#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
/*****************************************************************************/
//  Description : set PDP state
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_SetPDPState(MMIDL_DOWNLOAD_INSTANCE_T *self, MMIDL_PDP_STATE_E in_state)
{
    //SCI_TRACE_LOW:"MMIDL_SetPDPState:old %d-new %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3587_112_2_18_2_13_25_85,(uint8*)"dd", self->pdp_info.pdp_state, in_state);

    self->pdp_info.pdp_state = in_state;
}

/*****************************************************************************/
//  Description : Net ConnectEx
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL int32 MMIDL_NetConnectEx(MMIDL_DOWNLOAD_INSTANCE_T *self)
{
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    MMIPDP_ACTIVE_INFO_T app_info = {0};
    MMIDL_PDP_INFO_T pdp_info = {0};

    if (PNULL == self)
    {
        return -1;
    }

    dual_sys = self->pdp_info.dual_sys;

    //SCI_TRACE_LOW:"MMIDL_NetConnectEx bearer_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3602_112_2_18_2_13_25_86,(uint8*)"d", self->pdp_info.bearer_type);
    
#ifdef MMI_WIFI_SUPPORT
    if (MMIPDP_INTERFACE_WIFI == self->pdp_info.bearer_type)
    {    
        app_info.app_handler = MMIDL_MODULE_ID;
        app_info.handle_msg_callback = HandleDLPsMsg;
        app_info.ps_interface = MMIPDP_INTERFACE_WIFI;
    }
    else if (MMIPDP_INTERFACE_GPRS == self->pdp_info.bearer_type)
#endif
    {
        if (!MMIAPIPHONE_IsSimAvailable(dual_sys)) //TODO
        {
            //SCI_TRACE_LOW:"MMIDL_NetConnectEx:MMIAPIPHONE_IsSimOk not OK"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3616_112_2_18_2_13_25_87,(uint8*)"");
            MMIAPIPHONE_AlertSimNorOKStatus();
            MMIDL_SetPDPState(self, MMIDL_PDP_ACTIVE_FAILED);
            HandlePDPActiveFail(self, FALSE);

            SCI_MEMSET(self->http_waiting_task_id, 0x00, sizeof(self->http_waiting_task_id));
            self->http_waiting_task_id_num = 0;

            SetDownloadAppletBGState(FALSE);
            MMIDL_InitDownloadTaskStatus();

            return -1;
        }

        MMIDL_GetSettingsPDPInfo(&pdp_info, dual_sys);
        pdp_info.bearer_type = self->pdp_info.bearer_type; /* Dynamic */
        MMIDL_SetPDPInfo(&pdp_info);
        app_info.app_handler = MMIDL_MODULE_ID;
        app_info.auth_type = pdp_info.connect_setting.auth_type;
        app_info.apn_ptr = (char*)pdp_info.connect_setting.apn;
        app_info.user_name_ptr = (char*)pdp_info.connect_setting.username;
        app_info.psw_ptr = (char*)pdp_info.connect_setting.password;
        app_info.dual_sys = dual_sys;
        app_info.ps_service_type = DOWNLOAD_E;
        app_info.priority = 3;
        app_info.ps_service_rat = MN_TD_PREFER;
        app_info.handle_msg_callback = HandleDLPsMsg;
        app_info.ps_interface = self->pdp_info.bearer_type;
        app_info.storage = MN_GPRS_STORAGE_ALL;

    }

    MMIDL_SetPDPState(self, MMIDL_PDP_ACTIVING);

    if (!MMIAPIPDP_Active(&app_info))
    {       
        //SCI_TRACE_LOW:"MMIDL_NetConnectEx:MMIAPIPDP_Active fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3652_112_2_18_2_13_25_88,(uint8*)"");

        MMIDL_SetPDPState(self, MMIDL_PDP_ACTIVE_FAILED);
        HandlePDPActiveFail(self, TRUE);
        
        return -1;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : Net Connect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC int MMIDL_NetConnect(MMIDL_DOWNLOAD_INSTANCE_T *self)
{
    if (PNULL == self)
    {
        return -1;
    }

    //SCI_TRACE_LOW:"MMIDL_NetConnect pdp_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3671_112_2_18_2_13_26_89,(uint8*)"d",self->pdp_info.pdp_state);

    if ((MMIDL_PDP_NONE == self->pdp_info.pdp_state)
        ||(MMIDL_PDP_ACTIVE_FAILED == self->pdp_info.pdp_state)
        ||(MMIDL_PDP_DEACTIVING == self->pdp_info.pdp_state)
        ||(MMIDL_PDP_DEACTIVE_FAILED == self->pdp_info.pdp_state))
    {
        return MMIDL_NetConnectEx(self);
    }

    return 0;
}

/*****************************************************************************/
//  Description : Net Disconnect
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_NetDisconnect(MMIDL_DOWNLOAD_INSTANCE_T *self)
{
#ifndef WIN32
    if(PNULL == self)
    {
        return;
    }

    //SCI_TRACE_LOW:"MMIDL_NetDisconnect"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3696_112_2_18_2_13_26_90,(uint8*)"");
    //SCI_TRACE_LOW:"MMIDL_Network  = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3697_112_2_18_2_13_26_91,(uint8*)"d", self->pdp_info.pdp_state);
    if ((MMIDL_PDP_DEACTIVING == self->pdp_info.pdp_state)
        ||(MMIDL_PDP_DEACTIVE_OK == self->pdp_info.pdp_state)
        ||(MMIDL_PDP_DEACTIVE_FAILED == self->pdp_info.pdp_state))
    {
        return;
    }

    /* 断开GPRS拨号 */
    MMIAPIPDP_Deactive(MMIDL_MODULE_ID);
    MMIDL_SetPDPState(self, MMIDL_PDP_DEACTIVING);

    //SCI_TRACE_LOW:"MMIDL_NetDisconnect..."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3709_112_2_18_2_13_26_92,(uint8*)"");
#else
    MMIDL_SetPDPState(self, MMIDL_PDP_DEACTIVE_OK);
#endif
}

/*****************************************************************************/
// 	Description : process ps signal
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void HandleDLPsMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    DOWNLOAD_APPLET_T *download_applet_ptr = MMIDL_APPLETINSTANCE();
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = PNULL;

    if (PNULL == msg_ptr || PNULL == download_applet_ptr)
    {
        return;
    }

    download_instance = &download_applet_ptr->download_instance;
    
    //check the param
    //SCI_TRACE_LOW:"HandleDLPsMsg() enter msg_id= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3732_112_2_18_2_13_26_93,(uint8*)"d", msg_ptr->msg_id);
    
    switch(msg_ptr->msg_id)
    {
    case MMIPDP_ACTIVE_CNF:
        {
            //SCI_TRACE_LOW:"HandleDLPsMsg: MMIPDP_ACTIVE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3738_112_2_18_2_13_26_94,(uint8*)"");

            HandleDLPDPActiveCnf(download_instance, msg_ptr);
        }
        break;
        
    case MMIPDP_DEACTIVE_CNF:
        {
            //SCI_TRACE_LOW:"HandleDLPsMsg: MMIPDP_DEACTIVE_CNF"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3746_112_2_18_2_13_26_95,(uint8*)"");

            HandleDLPDPDeactiveCnf(download_instance, msg_ptr);
        }
        break;
        
    case MMIPDP_DEACTIVE_IND:
        {
            //SCI_TRACE_LOW:"HandleDLPsMsg: MMIPDP_DEACTIVE_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3754_112_2_18_2_13_26_96,(uint8*)"");

            HandleDLPDPDeactiveInd(download_instance, msg_ptr);
        }
        break;

    case MMIPDP_SERVICE_CHANGE_IND:
        {
            //SCI_TRACE_LOW:"HandleDLPsMsg: MMIPDP_DEACTIVE_IND"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3762_112_2_18_2_13_26_97,(uint8*)"");

            HandleDLPDPConnectInd(download_instance, msg_ptr);
        }
        break;

    default:
        break;
    }

    //SCI_FREE(msg_ptr);
}

/*****************************************************************************/
//  Description : handle download pdp active cnf
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleDLPDPActiveCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr || PNULL == download_instance)
    {
        return;
    }

    //SCI_TRACE_LOW:"HandleDLPDPActiveCnf() msg_ptr->result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3786_112_2_18_2_13_26_98,(uint8*)"d", msg_ptr->result);
    //SCI_TRACE_LOW:"HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3787_112_2_18_2_13_26_99,(uint8*)"dd", msg_ptr->ps_interface, download_instance->pdp_info.bearer_type);

    if (msg_ptr->ps_interface != download_instance->pdp_info.bearer_type)
    {
        return;
    }

    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        download_instance->pdp_info.net_id = msg_ptr->nsapi;
        MMIDL_SetPDPState(download_instance, MMIDL_PDP_ACTIVE_OK);
        HandlePDPActiveSuccess(download_instance);
    }
    else
    {
        MMIDL_SetPDPState(download_instance, MMIDL_PDP_ACTIVE_FAILED);
        HandlePDPActiveFail(download_instance, TRUE);
    }
}

/*****************************************************************************/
//  Description : handle download pdp deactive cnf
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleDLPDPDeactiveCnf(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr || PNULL == download_instance)
    {
        return;
    }

    //SCI_TRACE_LOW:"HandleDLPDPDeactiveCnf() msg_ptr->result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3818_112_2_18_2_13_26_100,(uint8*)"d", msg_ptr->result);
    //SCI_TRACE_LOW:"HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3819_112_2_18_2_13_26_101,(uint8*)"dd", msg_ptr->ps_interface, download_instance->pdp_info.bearer_type);

    if (msg_ptr->ps_interface != download_instance->pdp_info.bearer_type)
    {
        return;
    }

    if (MMIPDP_RESULT_SUCC == msg_ptr->result)
    {
        MMIDL_SetPDPState(download_instance, MMIDL_PDP_DEACTIVE_OK);
    }
    else
    {
        MMIDL_SetPDPState(download_instance, MMIDL_PDP_DEACTIVE_FAILED);
    }
}

/*****************************************************************************/
//  Description : handle download pdp deactive Ind
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandleDLPDPDeactiveInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == msg_ptr || PNULL == download_instance)
    {
        return;
    }

    //SCI_TRACE_LOW:"HandleDLPDPDeactiveInd() msg_ptr->result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3847_112_2_18_2_13_26_102,(uint8*)"d", msg_ptr->result);
    //SCI_TRACE_LOW:"HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3848_112_2_18_2_13_26_103,(uint8*)"dd", msg_ptr->ps_interface, download_instance->pdp_info.bearer_type);

    if (msg_ptr->ps_interface != download_instance->pdp_info.bearer_type)
    {
        return;
    }
    else
    {
        if (MMIPDP_RESULT_SUCC == msg_ptr->result)
        {
            MMIDL_SetPDPState(download_instance, MMIDL_PDP_DEACTIVE_OK);
        }
        else
        {
            MMIDL_SetPDPState(download_instance, MMIDL_PDP_DEACTIVE_FAILED);
        }

//        MMK_PostMsg(MMIDL_HANDLE(MMIDL_APPLETINSTANCE()), MSG_DL_PDP_DEACTIVE_IND, NULL, 0);
    }
}

/*****************************************************************************/
//  Description : handle download pdp connect Ind
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
LOCAL void HandleDLPDPConnectInd(MMIDL_DOWNLOAD_INSTANCE_T *download_instance, MMIPDP_CNF_INFO_T *msg_ptr)
{
    if (PNULL == download_instance)
    {
        return;
    }

    //SCI_TRACE_LOW:"HandleDLPDPDeactiveInd() msg_ptr->result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3880_112_2_18_2_13_26_104,(uint8*)"d", msg_ptr->result);
    //SCI_TRACE_LOW:"HandleDLPDPDeactiveCnf  interface = %d, bearer_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3881_112_2_18_2_13_26_105,(uint8*)"dd", msg_ptr->ps_interface, download_instance->pdp_info.bearer_type);

#if 0
    if (msg_ptr->ps_interface == download_instance->pdp_info.bearer_type)
    {
        return;
    }
    else
    {
        MMK_PostMsg(MMIDL_HANDLE(MMIDL_APPLETINSTANCE()), MSG_DL_PDP_CONNECT_IND, NULL, 0);
    }
#endif

}

/*****************************************************************************/
//  Description : Check Is Network Ready
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_NetworkIsReady(MMIDL_DOWNLOAD_INSTANCE_T *self)
{
    if (PNULL == self)
    {
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIDL_NetworkIsReady=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3902_112_2_18_2_13_26_106,(uint8*)"d",self->pdp_info.pdp_state);

    return (BOOLEAN)(MMIDL_PDP_ACTIVE_OK == self->pdp_info.pdp_state);  
}

/*****************************************************************************/
//  Description : Handle Http Waiting Task Id
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void DownloadHandleHttpWatingTaskId(uint32 task_id, BOOLEAN is_increase)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    uint32 i =0;
    uint32 j =0;

    if (task_id >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"DownloadHandleHttpWatingTaskId download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3924_112_2_18_2_13_26_107,(uint8*)"");
        return;
    }

    //SCI_TRACE_LOW:"DownloadHandleHttpWatingTaskId task_id=%d, is_increase=%d, num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3928_112_2_18_2_13_26_108,(uint8*)"ddd", task_id, is_increase, download_instance->http_waiting_task_id_num);

    if (is_increase)
    {
        if (download_instance->http_waiting_task_id_num < MMIDL_MAX_DOWNLOAD_TASK_NUM)
        {
            download_instance->http_waiting_task_id[download_instance->http_waiting_task_id_num] = task_id;
            download_instance->http_waiting_task_id_num ++;
        }
    }
    else
    {
        for (i = 0; i < download_instance->http_waiting_task_id_num; i++)
        {
            if (task_id == (uint32)download_instance->http_waiting_task_id[i])
            {
                if (MMIDL_MAX_DOWNLOAD_TASK_NUM - 1 != i)
                {
                    for (j = i; j < download_instance->http_waiting_task_id_num; j++)
                    {
                        download_instance->http_waiting_task_id[j] = download_instance->http_waiting_task_id[j + 1];
                    }

                    download_instance->http_waiting_task_id[download_instance->http_waiting_task_id_num] = 0;
                }
                else
                {
                    download_instance->http_waiting_task_id[i] = 0;
                }

                download_instance->http_waiting_task_id_num --;

                break;
            }
        }
    }
}

/*****************************************************************************/
//  Description : Handle PDP Active Success
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPActiveSuccess(MMIDL_DOWNLOAD_INSTANCE_T *self)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    uint32 i =0;

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"HandlePDPActiveSuccess download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3977_112_2_18_2_13_26_109,(uint8*)"");
        return;
    }

    if (!MMIDL_NetworkIsReady(download_instance))
    {
        SCI_TRACE_LOW("HandlePDPActiveSuccess Network not ready!");
        return;
    }

    //SCI_TRACE_LOW:"HandlePDPActiveSuccess http_waiting_task_id_num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_3987_112_2_18_2_13_26_110,(uint8*)"d", download_instance->http_waiting_task_id_num);

    if (0 == download_instance->http_waiting_task_id_num)
    {
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
        MMIDL_NETDISCONNECT();
#endif
        return;
    }

    for (i = 0; i < download_instance->http_waiting_task_id_num; i++)
    {
        MMIDL_StartHttpRequest(download_instance->http_waiting_task_id[i]);
    }

    SCI_MEMSET(download_instance->http_waiting_task_id, 0x00, sizeof(download_instance->http_waiting_task_id));
    download_instance->http_waiting_task_id_num = 0;
}

/*****************************************************************************/
//  Description : Handle PDP Active Fail
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
LOCAL void HandlePDPActiveFail(MMIDL_DOWNLOAD_INSTANCE_T *self, BOOLEAN is_prompt)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"HandlePDPActiveFail download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_4016_112_2_18_2_13_26_111,(uint8*)"");
        return;
    }

    if (MMIDL_NetworkIsReady(download_instance))
    {
        //SCI_TRACE_LOW:"HandlePDPActiveFail network not ready!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_4022_112_2_18_2_13_26_112,(uint8*)"");

        return;
    }

    SCI_MEMSET(download_instance->http_waiting_task_id, 0x00, sizeof(download_instance->http_waiting_task_id));
    download_instance->http_waiting_task_id_num = 0;

    SetDownloadAppletBGState(FALSE);

    if (is_prompt)
    {
        MMIPUB_OpenAlertWarningWin(TXT_DIAL_FAILED);
    }

    MMIDL_InitDownloadTaskStatus();
}
#endif

/*****************************************************************************/
// Description: convert mime string to mimi type
// Global resource dependence: 
// Author: jiaoyou.wu
// Note:Index means Mime type
/*****************************************************************************/
PUBLIC uint32 MMIAPIDL_MimeExtToMimeIndex(const char *mime_ext)
{
    DL_MIME_TYPE_E ret = DL_MIME_TYPE_MAX;

    uint32 i = 0;

    for (i = 0; i < sizeof(mime_table) / sizeof(mime_table[0]); i++) 
    {
        if (0 == stricmp(mime_table[i].ext_ptr, mime_ext)) 
        {
            ret = mime_table[i].value;
            break;
        }
    }

   return (uint32)ret;
}

/*****************************************************************************/
// Description: convert Netfront mime type to WWW mimi type
// Global resource dependence: 
// Author: jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC DL_MIME_TYPE_E MMIAPIDL_NFMimeTypeToDLmimeType(const uint32 in_type)
{
    return in_type;
}

/*****************************************************************************/
// Description: convert Netfront mime type to ext
// Global resource dependence: 
// Author: jiaoyou.wu
// Note: 
/*****************************************************************************/
PUBLIC const char *MMIAPIDL_NFMimeTypeToExt(const uint32 in_type)
{
    uint32 i = 0;
    const char * ext_ptr = PNULL;

    for (i = 0; i < sizeof(mime_table) / sizeof(mime_table[0]); i++) 
    {
        if (mime_table[i].value == in_type)
        {
            ext_ptr = mime_table[i].ext_ptr;
            break;
        }
    }

    return ext_ptr;
}

/*****************************************************************************/
//  Description : reg mime func
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDL_RegMimeCallback(MMIDL_MIME_DOWNLOAD_PARAM_T* param_ptr)
{
    if (PNULL !=  param_ptr && param_ptr->mime_type < MMIBROWSER_MIME_TYPE_MAX)
    {
        g_mime_callback[param_ptr->mime_type] = param_ptr->call_back;
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
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDL_MimeCallBack(MMIDL_MIME_CALLBACK_PARAM_T* param_ptr)
{
    if (PNULL != param_ptr)
    {
        MMIBROWSER_MIME_TYPE_E mime_type = MMIBROWSER_MIME_TYPE_MAX;

        if (MIME_TYPE_APPLICATION_JAVA_JAR == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_JAR;
        }
        else if (MIME_TYPE_APPLICATION_JAVA_JAD == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_JAD;
        }
        else if (MIME_TYPE_APPLICATION_SDP == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_SDP;
        }
#ifdef DRM_SUPPORT
        else if (MIME_TYPE_APPLICATION_VND_OMA_DRM_MESSAGE == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_DM;
        }
        else if (MIME_TYPE_APPLICATION_VND_OMA_DRM_CONTENT == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_DCF;
        }
        else if (MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_XML == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_DR;
        }
        else if (MIME_TYPE_APPLICATION_VND_OMA_DRM_RIGHTS_WBXML == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_DRC;
        }
#endif
#ifdef QBTHEME_SUPPORT
        else if (MIME_TYPE_APPLICATION_QBT == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_QBT;
        }
#endif
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
        else if (MIME_TYPE_APPLICATION_PNL == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_PNL;
        }
#endif
#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
        else if (MIME_TYPE_APPLICATION_NATC == param_ptr->mime_type)
        {
            mime_type = MMIBROWSER_MIME_TYPE_NTAC;
        }

#endif
        if (MMIBROWSER_MIME_TYPE_MAX != mime_type && PNULL != g_mime_callback[mime_type])
        {
            MMIDL_MIME_CALLBACK_PARAM_T temp_param_ptr = {0};
            SCI_MEMCPY(&temp_param_ptr, param_ptr, sizeof(MMIDL_MIME_CALLBACK_PARAM_T));
            temp_param_ptr.mime_type = mime_type;

            (*g_mime_callback[mime_type])(&temp_param_ptr);

            return TRUE;
        }
    }

    return FALSE;
}

/*****************************************************************************/
//  Description : Get audio type by nf type
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIDL_GetAudioTypeByNFMimeType(uint32 mime_type)
{
    DL_MIME_TYPE_E dl_mime_type = DL_MIME_TYPE_MAX;
    MMISRVAUD_RING_FMT_E audio_type = MMISRVAUD_RING_FMT_MAX;

    dl_mime_type = MMIAPIDL_NFMimeTypeToDLmimeType(mime_type);

    //SCI_TRACE_LOW:"MMIAPIDL_GetAudioTypeByNFMimeType mime_type=%d, dl_mime_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_4154_112_2_18_2_13_27_113,(uint8*)"dd", mime_type, dl_mime_type);

    switch (dl_mime_type)
    {
    case MIME_AUDIO_MIDI:
    case MIME_AUDIO_X_AIFF:
    case MIME_AUDIO_BASIC:
        audio_type = MMISRVAUD_RING_FMT_MIDI;
        break;

    case MIME_AUDIO_X_WAV:
        audio_type = MMISRVAUD_RING_FMT_WAVE;
        break;

    case MIME_AUDIO_MPEG:
        audio_type = MMISRVAUD_RING_FMT_MP3;
        break;

    case MIME_APPLICATION_OCTET_STREAM:
        audio_type = MMISRVAUD_RING_FMT_AMR;
        break;

    default:
        audio_type = MMISRVAUD_RING_FMT_MAX;
        break;
    }

    return audio_type;
}

/*****************************************************************************/
//  Description : Get download Status
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDL_GetDownloadStatus(void)
{
    return MMIDL_GetDownloadStatus();
}

/*****************************************************************************/
//  Description : set download setting to default
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDL_ResetFactorySetting(void)
{
    MMIDL_ResetDefaultSettings();
}

/*****************************************************************************/
//  Description : Delete completed download task 
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_DeleteCompletedDownloadTask(uint32 index)
{
    MMIDL_DOWNLOAD_MANAGER_T *dl_manager = PNULL;

    if ( index >= MMIDL_MAX_DOWNLOAD_TASK_NUM)
    {
        return;
    }

    dl_manager = MMIDL_GetDownloadManager();

    if (PNULL != dl_manager)
    {
        //SCI_TRACE_LOW:"MMIAPIDL_DeleteCompletedTask dl_instance exist status = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_4219_112_2_18_2_13_27_114,(uint8*)"d", dl_manager->task_info[index].status);
        if (dl_manager->task_info[index].status == MMIDL_DOWNLOAD_DOWNLOADED)
        {
            MMIDL_DeleteDownloadManagerTask(dl_manager, index, FALSE);
        }
    }
    else
    {
        dl_manager = SCI_ALLOCA(sizeof(MMIDL_DOWNLOAD_MANAGER_T));

        if (PNULL == dl_manager)
        {
            return;
        }
        SCI_MEMSET(dl_manager, 0x00, sizeof(MMIDL_DOWNLOAD_MANAGER_T));
        MMIDL_ReadDownloadManager(dl_manager);
        //SCI_TRACE_LOW:"MMIAPIDL_DeleteCompletedTask dl_instance Not exist status = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_4235_112_2_18_2_13_27_115,(uint8*)"d", dl_manager->task_info[index].status);
        if (dl_manager->task_info[index].status == MMIDL_DOWNLOAD_DOWNLOADED)
        {
            MMIDL_DeleteDownloadManagerTask(dl_manager, index, FALSE);
        }
        SCI_FREE(dl_manager);
    }
}

/*****************************************************************************/
//  Description : Is dl at top
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDL_IsDownloadActive(void)
{
    DOWNLOAD_APPLET_T *download_applet = MMIDL_APPLETINSTANCE();
    MMI_HANDLE_T applet_handle = PNULL;
    MMI_WIN_ID_T win_id = MMIDL_WIN_ID_START;

    if (PNULL != download_applet )
    {
        applet_handle = MMIDL_HANDLE(download_applet);
        if (PNULL != applet_handle)
        {
            for (win_id = MMIDL_WIN_ID_START + 1; win_id <  MMIDL_MAX_WIN_ID; win_id++)
            {
                if (MMK_IsFocusWin(MMK_GetWinHandle(applet_handle, win_id)))
                {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

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
                                                                                    )
{
    return MMIDL_GetDownloadPath(type, full_path_ptr, full_path_len_ptr);
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIDL_HandlePDP_Change(void)
{
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = MMIDL_INSTANCE();
    uint32 task_index = 0;

    if (PNULL == download_instance)
    {
        //SCI_TRACE_LOW:"MMIDL_HandlePDP_Change download_instance Null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_4296_112_2_18_2_13_27_116,(uint8*)"");
        return;
    }

    for (task_index = 0; task_index < MMIDL_MAX_DOWNLOAD_TASK_NUM; task_index++)
    {
        if (download_instance->download_task[task_index].is_used)
        {
#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
                DownloadHandleHttpWatingTaskId(task_index, FALSE);
#endif
                HTTP_CancelRequest(download_instance->download_task[task_index].context_id,
                                                download_instance->download_task[task_index].app_instance,
                                                download_instance->download_task[task_index].request_id);

                if (0 < download_instance->download_task_num && download_instance->download_task[task_index].is_downloading)
                {
                    download_instance->download_task[task_index].is_downloading = FALSE;

                    download_instance->download_task_num--;
                }
 
            download_instance->download_task[task_index].is_need_reload = TRUE;
        }
    }
    

#ifdef MMIDL_DOWNLOAD_PDP_SUPPORT
        MMIDL_NETDISCONNECT();
#endif

    MMIDL_SetPDPState(download_instance, MMIDL_PDP_DEACTIVE_OK); /* Set straight , Can not handle deactive CNF for bearer change  */

    if (MMIPDP_INTERFACE_GPRS == download_instance->pdp_info.bearer_type)
    {
        download_instance->pdp_info.bearer_type = MMIPDP_INTERFACE_WIFI;
    }
    else
    {
        download_instance->pdp_info.bearer_type = MMIPDP_INTERFACE_GPRS;
    }
}

/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_HandleServiceChange(MMIPDP_INTERFACE_E bearer_type)
{
    DOWNLOAD_APPLET_T *download_applet_ptr = MMIDL_APPLETINSTANCE();
    MMIDL_DOWNLOAD_INSTANCE_T *download_instance = PNULL;

    //SCI_TRACE_LOW:"MMIAPIDL_HandleServiceChange bearer_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_4348_112_2_18_2_13_27_117,(uint8*)"d", bearer_type);

    if (PNULL == download_applet_ptr)
    {
        return;
    }

    download_instance = &download_applet_ptr->download_instance;

    //SCI_TRACE_LOW:"MMIAPIDL_HandleServiceChange pdp_info.bearer_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDL_4357_112_2_18_2_13_27_118,(uint8*)"d", download_instance->pdp_info.bearer_type);

    if (bearer_type == download_instance->pdp_info.bearer_type)
    {
        return;
    }

    MMK_PostMsg(MMIDL_HANDLE(MMIDL_APPLETINSTANCE()), MSG_DL_PDP_CONNECT_IND, NULL, 0);
}

#else
/*****************************************************************************/
//  Description : start download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_StartDownload(MMIDL_INTERFACE_TYPE_E type, DPARAM param)
{

}

/*****************************************************************************/
//  Description : stop download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_StopDownload(uint32 task_index, BOOLEAN is_all)
{

}

/*****************************************************************************/
//  Description : Init download
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_InitDownload(void)
{

}

/*****************************************************************************/
//  Description : register DL module nv len and max item
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_InitModule(void)
{

}

/*****************************************************************************/
//  Description : Get audio type by nf type
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIDL_GetAudioTypeByNFMimeType(uint32 mime_type)
{
    return MMISRVAUD_RING_FMT_MAX;
}

/*****************************************************************************/
//  Description : Get download Status
//  Global resource dependence : 
//  Author:wancan.you
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDL_GetDownloadStatus(void)
{
    return FALSE;
}

/*****************************************************************************/
//  Description : set download setting to default
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIDL_ResetFactorySetting(void)
{
    
}

/*****************************************************************************/
//  Description : Delete completed download task 
//  Global resource dependence : none
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIDL_DeleteCompletedDownloadTask(uint32 index)
{

}

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
                                                                                    )
{
    return FALSE;
}
#endif //#ifdef DL_SUPPORT
