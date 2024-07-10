/*****************************************************************
 **  File Name: 	brw_export.h    							 *
 **  Author:		            								 *
 **  Date:		         										 *
 **  Description:definition of export structure and api of       *
     browser engine                                              *
******************************************************************
******************************************************************
**  Edit History											     *
**---------------------------------------------------------------*
 **  DATE			NAME			DESCRIPTION				     *
 **  20/12/2008		yuzhi.chen      Create. 					 *
*****************************************************************/
#ifndef BRW_EXPORT_H
#define BRW_EXPORT_H

#define BRW_WIN_HANDLE  uint32

typedef enum
{
	BRW_LANG_CH,
	BRW_LANG_EN,
	BRW_LANG_MAX
}BRW_LANG_E;

typedef struct tagBRW_SCREEN_OPTION_T
{
    uint16      width;
    uint16      height;
    uint16      left_top_x;
    uint16      left_top_y;    
}BRW_SCREEN_OPTION_T;

typedef enum  //browser key event
{
    BRW_KEY_EVENT_0=0,
    BRW_KEY_EVENT_1,
    BRW_KEY_EVENT_2,
    BRW_KEY_EVENT_3,
    BRW_KEY_EVENT_4,
    BRW_KEY_EVENT_5,
    BRW_KEY_EVENT_6,
    BRW_KEY_EVENT_7,
    BRW_KEY_EVENT_8,
    BRW_KEY_EVENT_9,
    BRW_KEY_EVENT_HASH,
    BRW_KEY_EVENT_STAR,
    BRW_KEY_EVENT_LEFT,
    BRW_KEY_EVENT_RIGHT,
    BRW_KEY_EVENT_UP,
    BRW_KEY_EVENT_DOWN,
    BRW_KEY_EVENT_LEFT_REPEAT,
    BRW_KEY_EVENT_RIGHT_REPEAT,
    BRW_KEY_EVENT_UP_REPEAT,
    BRW_KEY_EVENT_DOWN_REPEAT,
    BRW_KEY_EVENT_OK
}BRW_KEY_EVENT_E;
typedef enum //page navigate event type
{
    BRW_NAV_EVENT_NULL=0,
    BRW_NAV_EVENT_BACKWARD,
    BRW_NAV_EVENT_FORWARD,
    BRW_NAV_EVENT_REFRESH
}BRW_NAVIGATE_EVENT_E;
typedef enum   //page scroll event
{
    BRW_SCROLL_EVENT_UPLINE=1,
    BRW_SCROLL_EVENT_DOWNLINE,
    BRW_SCROLL_EVENT_UPPAGE,
    BRW_SCROLL_EVENT_DOWNPAGE,
    BRW_SCROLL_EVENT_MOVE_VERTICAL
}BRW_SCROLL_EVENT_E;

typedef enum 
{
    BRW_MIME_UNKOWN=0,            /*unknown*/  
    /*-------markup language------------*/
    BRW_MIME_HTML=1,       /*"text/html" or "application/vnd.wap.xhtml+xml"*/ 
    BRW_MIME_WML,          /*text/vnd.wap.wml*/
    BRW_MIME_WMLS,         /*text/vnd.wap.wmlscript */
    BRW_MIME_WMLSC,        /*application/vnd.wap.wmlscriptc*/
    BRW_MIME_WMLC,         /*application/vnd.wap.wmlc or application/vnd.wap.wbxml */
    BRW_MIME_PLAINTEXT,    /*text/plain"*/
    BRW_MIME_CSS,          /*text/css*/

    /*-------------image-----------------*/
    BRW_MIME_IMG_BMP=20,        /*image/bmp*/
    BRW_MIME_IMG_IEF,           /*image/ief*/
    BRW_MIME_IMG_GIF,           /*image/gif*/
    BRW_MIME_IMG_JPEG,          /*image/jpeg*/
    BRW_MIME_IMG_PNG,           /*image/png*/
    BRW_MIME_IMG_TIF,           /*image/tiff*/
    BRW_MIME_IMG_WBMP,          /*image/vnd.wap.wbmp*/
    BRW_MIME_IMG_PCX,
    BRW_MIME_IMG_UNKNOWN,       /*image*/
    
    /*-------------audio-----------------*/
    BRW_MIME_AUDIO_AMR=50,      /*audio/amr*/
    BRW_MIME_AUDIO_MIDI,        /*audio/midi*/
    BRW_MIME_AUDIO_MID,        /*audio/mid*/
    BRW_MIME_AUDIO_MPA,         /*audio/mpa*/
    BRW_MIME_AUDIO_MPA_ROBUST,  /*audio/mpa-robust*/
    BRW_MIME_AUDIO_MPA_LATM,    /*audio/mp4a-latm*/
    BRW_MIME_AUDIO_MPEG,        /*audio/mpeg */
    BRW_MIME_AUDIO_TONE,        /*audio/tone*/
    BRW_MIME_AUDIO_RM,          /*application/vnd.rn-realmedia*/
    BRW_MIME_AUDIO_WAV,         /*audio/x-wav*/
    BRW_MIME_AUDIO_3GP,         /*audio/3gp*/
    BRW_MIME_AUDIO_X_AAC,         /*audio/x-aac*/
    BRW_MIME_AUDIO_WMA,         /*audio/x-ms-wma*/
    BRW_MIME_AUDIO_UNKNOWN,     /*audio*/    

    /*-------------video------------------*/
    BRW_MIME_VIDEO_3GP=80,      /*video/3gpp*/
    BRW_MIME_VIDEO_MP4,        /*video/mp4*/
    BRW_MIME_VIDEO_MPEG,        /*video/mpeg*/
    BRW_MIME_VIDEO_WMV,        /*video/wmv*/
    BRW_MIME_VIDEO_UNKNOWN,     /*video*/

    /*-------------oma --------------------*/
    BRW_MIME_OMA_DD=100,      /*application/vnd.oma.dd+xml*/
    BRW_MIME_OMA_DRM,          /*pplication/vnd.oma.drm.message*/
    BRW_MIME_OMA_DRM_MESSAGE,          /*pplication/vnd.oma.drm.message*/
    BRW_MIME_OMA_DRM_CONTENT,       /*"application/vnd.oma.drm.content"*/
    BRW_MIME_OMA_DRM_RIGHTS_XML,        /*"application/vnd.oma.drm.rights+xml"*/
    BRW_MIME_OMA_DRM_RIGHTS_WBXML,      /*"application/vnd.oma.drm.rights+wbxml"*/
    BRW_MIME_OMA_END,
    
	/*-------------java --------------------*/
	BRW_MIME_JAD,			/*text/vnd.sun.j2me.app-descriptor*/
	BRW_MIME_JAR,			/*application/Java-archive*/
    BRW_MIME_SDP,
#ifdef MCARE_MINI_SUPPORT
    BRW_MIME_MCARE,
#endif
#ifdef QBTHEME_SUPPORT
    BRW_MIME_QBT,//fpr sprd
#endif
#ifdef HOME_LOCATION_INQUIRE_SUPPORT_TCARD
	BRW_MIME_PNL,
#endif
#ifdef DATA_ACCOUNT_IN_TCARD_SUPPORT
	BRW_MIME_NTAC,  //network account
#endif
    BRW_MIME_APP_END,

    BRW_MIME_MAX 
}BRW_MIME_TYPE_E;
//http方法,目前只支持get和post
typedef enum
{
    BRW_HTTP_METHOD_UNKNOWN=0,
    BRW_HTTP_METHOD_GET,
    BRW_HTTP_METHOD_POST,
    BRW_HTTP_METHOD_MAX
}HTTP_METHOD_E;

//data save mode
typedef enum 
{
    BRW_DATA_SAVE_IN_BUF,
    BRW_DATA_SAVE_IN_FILE
}BRW_DATA_SAVE_POS_E;

//HTTP body data info struct
typedef struct tagHTTP_BODY_DATA_T
{
  	BRW_DATA_SAVE_POS_E   mode;
  	union
	{
	     uint16*   file_name_ptr;//全路径文件名
	     uint8*   addr_ptr;//buf地址
	}data_src;
  	uint32    data_src_len;//buf方式的时候表示buf的大小,文件方式的时候表示文件内容的大小
  	uint8*    content_type_ptr;
}HTTP_BODY_DATA_T;
//pen down event relevant info
typedef struct tagBRW_PEN_CLICK_INFO_T
{
    uint16      x;
    uint16      y;
}BRW_PEN_CLICK_INFO_T;

#endif
