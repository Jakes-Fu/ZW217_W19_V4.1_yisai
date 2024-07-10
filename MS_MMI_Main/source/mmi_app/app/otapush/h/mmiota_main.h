/*****************************************************************************
** File Name:      mmiota_main.h                                                *
** Author:         minghu.mao                                                          *
** Date:           12/2008                                                   *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe PUSH OTA               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2008       minghu.mao        Create 
** 12/2008       minghu.mao        Modify
******************************************************************************/
#ifndef _MMIOTA_MAIN_H_    
#define _MMIOTA_MAIN_H_   

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmiota_id.h"
#include "mmiota_export.h"
#include "mmidisplay_data.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef MMI_OTA_SUPPORT
#if !defined WIN32 && !defined CHIP_ENDIAN_LITTLE
#define MMIOTA_LIST_VER		        "MMSVER.00.00.03_BE"  //Version
#else
#define MMIOTA_LIST_VER		        "MMSVER.00.00.03_LE"  //Version
#endif
#define MMIOTA_LIST_VER_LEN	        18

#define MMIOTA_MAX_LONG_PUSH_NUM                20
#define MMIOTA_MINUTE_TIME                      60
#define MMIOTA_MAX_USER_VALID_DATA_LENGTH       (2*1024)
//#define MMIMMS_ONEHOUR_SECONDS                  (60*60)
#endif
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/
#define MMIOTA_UCS2_DIR                      (g_mmiota_dir_wstr)                //"\x00\x4d\x00\x4d\x00\x53\x00\x00"
#define MMIOTA_UCS2_DIR_LEN                   3 
#define OTA_UCS2_NAME_MAX_LEN                 32     //OTA file dir len
typedef int32 (*Brw_Cbf)(BOOLEAN result,void* param);
#define READ_FILE_RESP                        1
#define WRITE_FILE_RESP                       2
#define	MMICMSBRW_URL_HEAD					  "http://"						//URL的头
#define	MMICMSBRW_URL_HEAD_LEN			      7								//URL的头的长度
#define MSG_CMSBRW_BRW_OP				      (0x300 + 136)


#define  WBXML_VARIABLE_VALUE_MAX_LENGTH      5
#define  SMS_PDU_USER_DATA_MAX_LENGTH         160                           //单条短消息用户数据最大长度

#define   OTA_USER_DATA_MAX_LEN               4096                          //OTA数据不超过1K
#define   OTA_WSP_HEADER_DATA_MAX_LEN         SMS_PDU_USER_DATA_MAX_LENGTH   //wsp头和数据wsp头不能超过1个pud的udl（140）
#define   OTA_MAX_DATE_LEN                    32

/**--------------------------------------------------------------------------*
**解析 结构定义                                                          *
**--------------------------------------------------------------------------*/ 
typedef struct   ota_attr_tag{             //描述tag的数据结构
    uint8 * att_name;                      //描述type类型，如<value = "111">，这个att_name表示value
    uint8 * att_value;                     //描述value值是多少，value表示"111",或者<name = "NAME">中的"NAME"
    struct ota_attr_tag *next;             //描述同tag里面的下一个属性，<name = "NAME" value = "111">第一个表示name，那么next表示value
}OTA_ATTR_T;                               //命名为ota的别名

typedef struct   ota_tag_item_tag{         //OTA使用的xml树结构
    OTA_ATTR_T     *tag_property;          //标签属性链表
    uint8		   *tag_add_content;       //标签附件内容    
    uint16	       index;                  //树的层次索引
    uint8	       *tag_name;              //标签类型，如<parm name = "NAME" value = "111">中的parm
    struct	ota_tag_item_tag   *parent;
    struct  ota_tag_item_tag   *child;
    struct  ota_tag_item_tag   *next;
}OTA_TAG_ITEM_T;

typedef struct{                                               //ota  wbxml data struct	 
	uint32 length;                                            // the length of ota vavid data
    uint8  *ota_user_data_arr;                                // the data buf of ota vavid data
}OTA_USER_DATA_T;

typedef struct {                                              //ota wsp data struct	 
    uint32 length;                                            // the length of ota wsp header data lenghth
    uint8  *ota_wsp_header_data_arr;                          // the data buf of ota wsp header data
} OTA_WSP_HEADER_DATA_T;

typedef enum{        
        MIME_OTACONNECTIVITY = 0,          //ota connectivity used in OMA specification
        MIME_BROWERSETTING,                //ota browser setting used in NOKIA OTA specification
        MIME_BROWERBOOKMARK,               //ota bookmark setting used in NOKIA OTA specification
        MIME_WAP_PUSH_SI,                  //ota wap si push
        MIME_WAP_PUSH_CO,
        MIME_WAP_PUSH_SL,
        MIME_OMA_DCD,                      //OMA DCD
		MIME_DRM_RIGHTS,                   //drm right
        MIME_MAX                           //unknown ota setting content type
}OTA_MIME_TYPE_E;

typedef struct{                            //描述ota数据的数据结构包括wsp头和数据和ota wbxml数据
    OTA_WSP_HEADER_DATA_T   wsp;           //wsp header data of ota data in wbxml type
    OTA_USER_DATA_T body;                  //vavid data of ota data in wbxml type
}OTA_DATA_T;

typedef struct wsp_header_tag{             //wsp header data struct                        
    uint8                    *field;       //save the type of result of decode of wsp header, mostly content is "contenttype" 
    uint8                    *value;       //save the mime type  of the PushOTA in char type
    struct wsp_header_tag    *next;    
}WSP_HEADER_T;

/**--------------------------------------------------------------------------*
**Get Value结构定义(主要定义协议映射结构)                                    *
**--------------------------------------------------------------------------*/ 
typedef enum{                              //tag type 
    CHARACTERISTIC = 0,                    // characteristic type
    PARM,                                  // parm type
    ELEMENT_TYPE_MAX
}ELEMENT_TYPE_E;

typedef struct ota_tag_node_tag{           //ota tag name node
    ELEMENT_TYPE_E    element_type;        //当前node是parm还是characteristic,0表示characteristic,1表示parm
    uint8             *tag_name;           //tag的名称
    struct ota_tag_node_tag     *next;
}OTA_TAG_NODE_T;

typedef enum{                               // ota setting type
        MMIOTA_SETTING_TYPE = 0,            //设置类型
        MMIOTA_ACCESS_TYPE,                 //0 表示wap1.2, 1表示wap2.0
        MMIOTA_NEED_SECURITY_CONNECT,       //是否使用安全连接
        MMIOTA_PROXY_PORT,      			//端口号
        MMIOTA_SEETTING_NAME,	            //连接设置的名字,如中国移动GPRS设置
        MMIOTA_PROXY_ADDRESS,               //网关地址
        MMIOTA_APN_USER_NAME,               //APN用户名
        MMIOTA_APN_PASSWORD,                //APN密码
        MMIOTA_APN,                  		//接入点
        MMIOTA_HOMEPAGE,	                //主页网址，对于MMS，此为彩信中心
        MMIOTA_BOOMARK_NAME,		        //书签设置的名字
        MMIOTA_BOOKMARK_URL,	            //书签URL
        MMIOTA_WAP_SI_NAME,		            //书签设置的名字
        MMIOTA_WAP_SI_URL,	                //书签URL
        MMIOTA_WAP_SI_ID,
        MMIOTA_WAP_SI_CREATED_TIME,
        MMIOTA_WAP_SI_EXPIRED_TIME,
        MMIOTA_WAP_SI_SIGNAL_ACTION,
        MMIOTA_WAP_SL_NAME,
        MMIOTA_WAP_SL_URL,
        MMIOTA_WAP_SL_EXECUTE_ACITON,
        MMIOTA_WAP_CO_NAME,
        MMIOTA_WAP_CO_URL,
        MMIOTA_WAP_CO_SERVICE,
        MMIOTA_PARAMETER_MAX
}MMIOTA_SETTING_PARAMETER_E;

typedef struct {                            //ota tag path node
    OTA_TAG_NODE_T                 *path_node;           //path node link  
    MMIOTA_SETTING_PARAMETER_E     parameter_name;       //tag setting type
}OTA_TAG_PATH_T;

typedef struct ota_setting_path_tag{                        //tag path link                  
    OTA_TAG_PATH_T                   *tag_path;
    struct ota_setting_path_tag      *next;
}OTA_SETTING_PATH_T;

typedef struct {                            //map element
    char     *characteristic;               //多个characteristic的是用/间隔，表示层次
    char     *parm;
    MMIOTA_SETTING_PARAMETER_E   parameter_name;
}OTA_ELEMENT_PATH_T;  

typedef enum
{
    SAVEFS_READ_SUCCEED = 4,
    SAVEFS_READ_FAIL    = 5
}SAVEFS_READ_RESULT;

typedef struct  
{
    MMIFILE_HANDLE        hefs;        //EFS_Close    
    MMIOTA_WINDOW_ID_E    win_id;      //pass to EFS_OVERLAPPED_RESULT_T, to MMK_PostMsg
    SAVEFS_READ_RESULT    read_result; //response of EFS task
    uint32                buf_size;    //to save in nv
    uint8                *buf_ptr;     //to free it
}FFS_READFILE_RESULT_T;


typedef enum
{
    SAVEFS_NOT_ENOUGH_SPACE = 0,
    SAVEFS_WRITE_FAIL = 1,
    SAVEFS_WRITE_SUCCEED = 2,
    SAVEFS_WRITE_PANDING = 3,
    SAVEFS_WIRTE_INVALID
}SAVEFS_WRITE_RESULT;

typedef struct
{
    MMIFILE_HANDLE       hefs;        //EFS_Close    
    MMIFILE_DEVICE_E     dev_type;
    MMI_WIN_ID_T         win_id;        //pass to EFS_OVERLAPPED_RESULT_T, to MMK_PostMsg
    SAVEFS_WRITE_RESULT  write_result;    //response of EFS task
    uint32               buf_size;    //to save in nv
    uint8                *buf_ptr;    //data ptr
    BOOLEAN              is_buffer_need_freed; //is buf_ptr need freed. TRUE: need be freed here.
}FFS_SAVEFILE_RESULT_T;

typedef enum
{
    CMSBRW_FS_NOT_ENOUGH_SPACE	= 0,
	CMSBRW_FS_NOT_EXIT			= 1,

	CMSBRW_FS_WRITE_FAIL		= 2,
	CMSBRW_FS_WRITE_SUCCEED		= 3,
	CMSBRW_FS_WRITE_PANDING		= 4,
	CMSBRW_FS_WIRTE_INVALID		= 5,

	CMSBRW_FS_READ_SUCCEED		= 6,
	CMSBRW_FS_READ_FAIL			= 7,
	CMSBRW_FS_READ_PANDING		= 8,

	CMSBRW_FS_DELETE_SUCCEED	= 9,
	CMSBRW_FS_DELETE_FAIL		= 10
}CMSBRW_FS_OPERATE_RESULT;

typedef enum
{
    CMSBRW_FS_NOT_TYPE,
	CMSBRW_FS_BOOKMARK_TYPE,
	CMSBRW_FS_HISTORY_TYPE,
	CMSBRW_FS_SNAPSHOT_STRUCT_TYPE,
	CMSBRW_FS_SNAPSHOT_DATA_TYPE,
	CMSBRW_FS_SNAPSHOT_DATA_ONEFORALL_TYPE,
	CMSBRW_FS_PUSHMSG_INIT_TYPE,
	CMSBRW_FS_PUSHMSG_TYPE,
	CMSBRW_FS_PUSHMSG_DELETE_STRUCT_MEMBER_TYPE,
	CMSBRW_FS_SETTING_TYPE,

	CMSBRW_FS_BRW_CACHE_TYPE,
	CMSBRW_FS_BRW_COOKIE_TYPE,
	CMSBRW_FS_BRW_JAR_TYPE,
	MMIMMS_FS_LONG_PUSH_TYPE,

	CMSBRW_FS_BRW_MAX_TYPE
}CMSBRW_FS_OPERATE_TYPE;

typedef struct
{
    uint32      				source_win_id;	//进行文件操作的窗口的ID，如果当前操作确实不需要则填-1
    MMIFILE_HANDLE				hefs;			//
    uint32			        	fs_win_id;		//文件处理窗口，pass to EFS_OVERLAPPED_RESULT_T, to MMK_PostMsg
    CMSBRW_FS_OPERATE_RESULT	result;			//response of EFS task
    uint32						buf_size;		//to save in nv
    uint8						*buf_ptr;		//to free it
    BOOLEAN						IsJumpDialog;	//保存文件过程中是否弹出提示对话框
    CMSBRW_FS_OPERATE_TYPE		fileType;		//文件操作的类型
}FFS_FILE_RESULT_T;

typedef struct  {
    MMIFILE_HANDLE				hefs;//文件句柄
    CMSBRW_FS_OPERATE_RESULT	result;			//response of EFS task
    uint32						buf_size;		//to save in nv
    uint8						*buf_ptr;		//to free it
    CMSBRW_FS_OPERATE_TYPE		fileType;		//文件操作的类型
    Brw_Cbf                      cbf;//回调函数
    uint32                      param;//参数
}MMIBRW_FILE_OP_PARM_T;

typedef struct  {
    uint32 message_type;
    MMIBRW_FILE_OP_PARM_T param;
}MMIBRW_VIRWIN_MESSAGE_T;

#ifdef MMI_OTA_SUPPORT
typedef struct MMIOTA_PUSH_DETAIL_T_tag
{
    uint32                    push_time;
    uint16                    message_id;        //item id
    uint8                    index_id;
    uint16                    total_num;
    uint16                    length;
    uint8                    push_data[MN_SMS_MAX_USER_VALID_DATA_LENGTH];
    MN_DUAL_SYS_E           dual_sim;
    BOOLEAN                 is_vavid;
}MMIOTA_PUSH_DETAIL_T;

typedef struct MMIOTA_LONG_PUSH_T_tag
{
    uint8                       version[MMIOTA_LIST_VER_LEN+1];
    MMIOTA_PUSH_DETAIL_T        push_detail[MMIOTA_MAX_LONG_PUSH_NUM];    //item
}MMIOTA_LONG_PUSH_T;

typedef struct MMIOTA_USER_VALID_DATA_T_tag
{     
    uint16 length;                                          // the length
    uint8 user_valid_data_arr [ MMIOTA_MAX_USER_VALID_DATA_LENGTH ]; // the data buf 
} MMIOTA_USER_VALID_DATA_T;
#endif
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef MMI_OTA_SUPPORT
/*****************************************************************************/
//  Description :get current file system
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIOTA_GetCurrentFFS( void );

/*****************************************************************************/
//  Description :check if file is exist
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIOTA_CheckFileExist(const uint8 *file);

/*****************************************************************************/
//  Description :get device name
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC wchar *MMIOTA_GetDeviceName(uint16 *device_name_len);

/*****************************************************************************/
//  Description :file create
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_HANDLE MMIOTA_FilecCreate(const uint8 *name, uint32 mode);

/*****************************************************************************/
//  Description :file close
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FileClose(MMIFILE_HANDLE handle);

/*****************************************************************************/
//  Description :file write
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FileWrite(MMIFILE_HANDLE handle, 
                                        uint8 *buf,  
                                        uint32 len, 
                                        uint32 *transmitted, 
                                        MMIFILE_OVERLAPPED_T *overlapped_ptr
                                        );

/*****************************************************************************/
//  Description :file delete
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FileDelete(const wchar* ucs2_name_ptr, MMIFILE_OVERLAPPED_T *overlapped_ptr);

/*****************************************************************************/
//  Description :file read
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FileRead(MMIFILE_HANDLE handle, 
                                       uint8 *buf, 
                                       uint32 len, 
                                       uint32 *transmitted, 
                                       MMIFILE_OVERLAPPED_T *overlapped_ptr
                                       );

/*****************************************************************************/
//  Description :file get length
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIOTA_FileGetLength(MMIFILE_HANDLE handle);

/*****************************************************************************/
//  Description :create file dir
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIOTA_FilecCreateDir(const char *path);

/*****************************************************************************/
//  Description :is enough space to save ota file
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIOTA_IsEnoughSpace(uint32 size);

/*****************************************************************************/
//  Description : init ota push
//  Global resource dependence : 
//  Author:jixin.xu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIOTA_PushInit(void);

/*****************************************************************************/
//  Description : Decode ota push 
//  Global resource dependence : none
//  Author: jixin.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIOTA_DecodePush(
                              MN_DUAL_SYS_E dual_sys, 
                              APP_MMS_USER_DATA_T *sms_ptr,
                              uint8      *sender_ptr,
                              uint32     sender_len,
                              uint8      *sc_sender_ptr,
                              uint32     sc_sender_len
                              );
#endif
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }

#endif
#endif

