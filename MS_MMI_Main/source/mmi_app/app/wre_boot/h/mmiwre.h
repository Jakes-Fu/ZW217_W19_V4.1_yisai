/*****************************************************************************
** File Name:      mmiwre.h                                                   *
** Author:                                                                   *
** Date:           04/03/2006                                               *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                   *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2006      annie.an         Create
******************************************************************************/

#ifndef _MMIWRE_H_
#define _MMIWRE_H_
#ifdef WRE_SUPPORT
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmiwre_nv.h"

//#define WRE_WREKERNEL_PRELOAD
//#define WRE_WRESTORE_PRELOAD
//#define WRE_WREAPP_PRELOAD

#include "mmiwre_preload.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define MMIWRE_RUNTIME_PATH_MAX          32
#define MMIWRE_MEM_CONFLICT_SOLUTION

enum
{
    RUNTIMEPATH_CUR,
    RUNTIMEPATH_PREV,

    RUNTIMEPATH_TOTAL,
};

//User Settings info
typedef struct _MMIWRE_RUNTIME_PATH
{
    uint16  kernel_path[RUNTIMEPATH_TOTAL][MMIWRE_RUNTIME_PATH_MAX];
    uint16  store_path[RUNTIMEPATH_TOTAL][MMIWRE_RUNTIME_PATH_MAX];
    uint16  app_path[RUNTIMEPATH_TOTAL][MMIWRE_RUNTIME_PATH_MAX];
    uint16  runtime_flag;
} MMIWRE_RUNTIME_PATH_T;

#ifdef WRE_VIRTUALDISK_SUPPORT

#define WRE_ROM_DISK_NAME   L"C"
#define WRE_RAM_DISK_NAME   L"H"

#endif

typedef struct _tagSNS_APP_INFO_T
{
    uint32  fixed_id;
    uint16 *guid;
    uint16 *store_app_id;
    uint16  *filepath;
    uint16  *appname;
    uint16  *iconpath;
}SNS_APP_INFO_T,*PSNS_APP_INFO_T;

typedef enum
{
    WRE_FIX_ID_DIGGOLD,
    WRE_FIX_ID_SINA,   
    WRE_FIX_ID_FACEBOOK,
    WRE_FIX_ID_TWITTER,
    //WRE_FIX_ID_FRUIT,
#ifdef NES_SUPPORT
#ifdef WRE_CONTRA_SUPPORT  // 魂斗罗
    WRE_FIX_ID_CONTRA,
#endif
#ifdef WRE_SUPERMARIO_SUPPORT  // 超级玛丽
    WRE_FIX_ID_SUPERMARIO,
#endif
#ifdef WRE_TSLAND_SUPPORT  // 冒险岛
    WRE_FIX_ID_TSLAND,
#endif
#endif
    MAX_FIXED_APP_NUM
}WRE_APP_ID_E;

/**--------------------------------------------------------------------------*
 **                         INTERFACE TO OTHER MMI MODULE                                      *
 **--------------------------------------------------------------------------*/
PUBLIC BOOLEAN ShowLoading(BOOLEAN bStartup);
PUBLIC void MMIAPIWRE_InitModule(void);
PUBLIC BOOLEAN WRE_CheckUDiskRunState(void);
PUBLIC void MMIAPIWRE_Init(void);
PUBLIC void MMIAPIWRE_Reset(void);
PUBLIC void MMIWRE_ReadNV(MMIWRE_NV_ITEM_E nv_item);
PUBLIC void MMIWRE_WriteNV(MMIWRE_NV_ITEM_E nv_item);    

#define   ONEKEY_DOWNLOAD_STEP_PART1    0 
#define   ONEKEY_DOWNLOAD_STEP_PART2    1
#define   ONEKEY_DOWNLOAD_STEP_FINISH   2

#define WRE_HTTP_HEADER_FILE_NAME      L"datahead.dat"
#define WRE_HTTP_CONTENT_FILE_NAME     L"wrepart1.wpk"

#define WRE_PART2_HTTP_HEADER_FILE_NAME      L"datahead2.dat"
#define WRE_PART2_HTTP_CONTENT_FILE_NAME     L"wrepart2.wpk"

//#define ACCEPT_STRING "*/*"
#define WRE_ACCEPT_STRING "*.*"

#define http_hdr_wre_imei "WRE_IMEI"
#define http_hdr_wre_imsi "WRE_IMSI"
#define http_hdr_wre_cellid "WRE_CELLID"
#define http_hdr_wre_wreversion "WRE_WREVESION"
#define http_hdr_wre_bootversion "WRE_BOOTVERSION"
#define http_hdr_wre_chipversion "WRE_CHIPVERSION"
#define http_hdr_wre_mocorversion "WRE_MOCORVERSION"
#define http_hdr_wre_idhid "WRE_IDHID"
#define http_hdr_wre_scrensize "WRE_SCREENSIZE"
#define http_hdr_wre_deviceid "WRE_DEVICEID"
#define http_hdr_wre_range    "Range"
#define http_hdr_wre_md5      "Md5"

#define WRE_UPDATE_MD5_FILE L"\\dloadwre.dat"

#define MD5_STRING_LEN               32
#define GATEWAY_HTTP_PROXY_NONE      "0.0.0.0"   //假入网关地址不等于该值则用代理方式下在WRE Kernel

#define   WRE_ONE_KEY_FLAG     0x54671098   //一键下载标志

typedef struct _tagWRE_MD5_LIST
{
	uint32 onekeyflag;
	char   part1md5[MD5_STRING_LEN+4];
	char   part2md5[MD5_STRING_LEN+4];
}WRE_MD5_LIST,*PWRE_MD5_LIST;

#define ONEKEY_DOWNLOAD_SUPPORT 
typedef struct _tagWRE_HTTP_PRO_IND_T
{
    uint32   totalnum;
    uint32   recvnum;
}WRE_HTTP_PRO_IND_T;

typedef struct _tagWRE_HTTP_T
{
    uint32         http_state;   //HTTP 命令状态
    BOOLEAN        is_task_on;   //task 是否运行
    uint32         http_task_id; //task id  
    uint32         win_id;
    
    uint32         old_file_size;  //old file size for range.
    uint32         reload_num;     //reload times.
    uint32         refresh_num;    //readed bytes.
    uint32         refresh_tick;   //readed bytes refresh time.
    BOOLEAN        reloading;      //reloading for fail.
    SCI_TIMER_PTR  timer_handle;

	WRE_HTTP_PRO_IND_T proc_ind;  //进度状态

}WRE_HTTP_T,*PWRE_HTTP_T;

PUBLIC char * WRE_GetPart2Url(void);
PUBLIC BOOLEAN WRE_SaveMd5(uint32 step,char* md5string);
PUBLIC BOOLEAN WRE_GetMd5(uint32 step,char *md5string);
PUBLIC void WRE_DeleteMd5File(void);
PUBLIC BOOLEAN WRE_GetOnekeyDownLoadState(void);
PUBLIC BOOLEAN WRE_SetOnekeyDownLoadState(BOOLEAN bStartup);
PUBLIC uint8 MiniIntaller_Is_Store_OK(char * url);

typedef struct
{
    uint8 appType;
    uint32 appId;
    uint16 *appName;
    uint32 appNameLen;
    void *data;
    uint32 dataLen;
}MMIWRE_INSTANCE_T;

typedef struct 
{
    CAF_APPLET_T caf_applet;
    MMIWRE_INSTANCE_T instance;   
}MMIWRE_APPLET_T;   

#ifdef   __cplusplus
    }
#endif

#endif

#endif

