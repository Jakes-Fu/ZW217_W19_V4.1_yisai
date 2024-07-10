#ifndef _ABUP_TYPEDEF_H_
#define _ABUP_TYPEDEF_H_

#ifndef abup_extern
#ifdef __cplusplus
#define abup_extern extern "C"
#else
#define abup_extern extern
#endif
#endif//abup_extern

#ifndef ABUP_FOTA_SUPPORT_TCARD
//#define FOTA_SUPPORT_ABUP_NVRAM
#endif

typedef char                    abup_char;

typedef unsigned short          abup_wchar;

typedef unsigned char           abup_uint8;

typedef signed char             abup_int8;

typedef unsigned short int      abup_uint16;

typedef signed short int        abup_int16;

typedef unsigned int            abup_uint32;
typedef unsigned int            size_t;

typedef signed int              abup_int32;

#if 1
typedef unsigned long int   abup_uint64;

typedef signed long int     abup_int64;

#else
typedef unsigned long long   abup_uint64;

typedef signed long long     abup_int64;
#endif

typedef abup_int32   abup_socint;


typedef enum 
{
    ABUP_FALSE,
    ABUP_TRUE
} ABUP_BOOL;

enum FotaState {
	STATE_INIT = 1,    
	STATE_CV,    
	STATE_DL,    
	STATE_RD,    
	STATE_UG,
	STATE_RU,
	STATE_RG
};

typedef enum
{
	FOTA_SUCCESS=1000,
	FOTA_PID_ERROR,
	FOTA_PROJECT_ERROR,
	FOTA_PARAM_INVAILD,
	FOTA_PARAM_LOST,
	FOTA_SYS_ERROR,
	FOTA_JSON_ERROR,
	FOTA_RG_SIGN_ERROR=2001,
	FOTA_CV_LAST_VERSION=2101,
	FOTA_CV_INVAILD_VERSION,
	FOTA_CV_UNREG_DEVICE,
	FOTA_DL_STATE_ERROR=2201,
	FOTA_DL_DELTAID_ERROR,
	FOTA_RP_DELTAID_ERROR=2301,
	FOTA_RP_UPGRADE_STATE_ERROR	
}FOTACODE;

enum DownloadStatus
{
	ABUP_DL_SUCCESS = 1,
	DL_NO_NETWORK,
	DL_CANCELED,
	DL_PATH_NOT_EXIST,
	DL_SERVER_ERROR,
	DL_TIMEOUT,
	DL_NETWORK_ERROR,
	DL_MD5_MIS_MATCH,
	DL_PARA_ERROR,
	DL_NO_SPACE,
};



enum NetStatusMachine {
	NET_STAT_SUCCESS = 2000,    
	NET_STAT_DNS_REQ,		//2001
	NET_STAT_DNS_GETTING,	 
	NET_STAT_DNS_SUCC,	 
	NET_STAT_NEWSOC_REQ,    //2004
	NET_STAT_NEWSOC_SUCC,	
	NET_STAT_NEWSOC_ERR,    
	NET_STAT_DNS_ERR,
	NET_STAT_CONN_REQ,		//2008
	NET_STAT_CONNECTING,
	NET_STAT_CONN_SUCC,
	NET_STAT_CONN_ERR,
	NET_STAT_SEND_REQ,		//2012
	NET_STAT_SENDING,
	NET_STAT_SEND_SUCC,
	NET_STAT_SEND_ERR,
	NET_STAT_RECV_REQ,		//2016
	NET_STAT_RECVING,
	NET_STAT_RECV_SUCC,
	NET_STAT_RECV_SUCC2,
	NET_STAT_RECV_ERR,
	NET_STAT_CLOSE_REQ,		//2021
	NET_STAT_CLOSE_SUCC,
	NET_STAT_CLOSE_ERR
};

typedef struct
{
    abup_uint16 nYear;
    abup_uint8 nMonth;
    abup_uint8 nDay;
    abup_uint8 nHour;
    abup_uint8 nMin;
    abup_uint8 nSec;
    abup_uint8 DayIndex; /* 0=Sunday */
} abup_time_struct;

typedef struct _VersionInfo {
	abup_int32  status;
	abup_char *msg;
	abup_char *versionName;
	abup_int32   fileSize;
	abup_char *deltaID;
	abup_char *md5sum;
	abup_char *deltaUrl;
	abup_char *publishDate;
	abup_char *releaseContent;
	abup_int32  storageSize;
	abup_int32 	battery;
	abup_int32  	download_status;
	abup_uint32 download_start;
	abup_uint32 download_end;
	abup_int32  update_status;
	abup_char *deviceSecret;
	abup_char *deviceId;
} VersionInfo;


typedef struct _MobileParamInfo {
	abup_char  *mid;
	abup_char  *version;
	abup_char  *oem;
	abup_char  *models;
	abup_char  *productid;
	abup_char  *productsec;	
	abup_char  *platform;
	abup_char  *device_type;
	abup_char  *mac;
} MobileParamInfo;

typedef struct _DownParamInfo {
	abup_char  *mid;
	abup_char  *token;
	abup_char  *deltaID;
	abup_char  *productid;
	abup_char  *productsec;	
	abup_int32         download_status;
	abup_time_struct      download_start;
	abup_time_struct      download_end;
	abup_uint32 download_size;
	abup_char *download_ip;
} DownParamInfo;

typedef struct _UpgradeParamInfo {
	abup_char  *mid;
	abup_char  *token;
	abup_char  *deltaID;
	abup_char  *productid;
	abup_char  *productsec;	
	abup_int32  update_status;
	abup_char   *extendString;
} UpgradeParamInfo;



typedef struct _streambuf {
    abup_char        *data;
    abup_int32         limit;
    abup_int32         size;
} streambuf;

typedef struct _request {
    streambuf   header;
    streambuf   body;
} Request;


typedef struct
{
   abup_char *mcu_id;
   abup_int16 (*abup_patch)(void);
}abupMcuPatch;



#ifndef NULL
#define NULL  (void *)0           /*  NULL    :   Null pointer */
#endif


#define E_ABUP_SUCCESS			0
#define E_ABUP_FAILURE			-1
#define E_ABUP_NOMEMORY			-2
#define E_ABUP_NOTEXIST			-3
#define E_ABUP_WOULDBLOCK		-4
#define E_ABUP_SIM_NOTEXIST		-5
#define E_ABUP_MID_ERROR		-6
#define E_ABUP_NETWORK_ERROR		-7
#define E_ABUP_DOWNLOAD_FAIL		-8
#define E_ABUP_LAST_VERSION		-9
#define E_ABUP_TOKEN_WORNG		-10
#define E_ABUP_PROJECT_WORONG		-11
#define E_ABUP_PARAM_WORONG		-12
#define E_ABUP_NET_CONNECTING	-13
#define E_ABUP_DOWNLOAD_CONNECTING		-14

#ifndef ABUP_DMEM_DEBUG
#define ABUP_MAX_DEBUG_BUF_LEN		1024
#else
#define ABUP_MAX_DEBUG_BUF_LEN		2048
#endif

#define RECV_BLOCK_SIZE 1024 //can not over the 1024 limit

#define ABUP_DIFF_PARAM_SIZE		(64*1024)


#define ABUP_TIMER_NAME_STRLEN		(32)

typedef void (*af_callback)(VersionInfo *info, abup_int32 state);

typedef void (*abup_notify)(abup_socint data);

//typedef void (*abup_timer_callback)(abup_uint32 param);
typedef void (*abup_timer_callback)(void);

typedef void (*abup_common_callback)(void);


typedef void (*abup_write)(abup_char *data, abup_uint32 len);

typedef enum 
{
	ABUP_READ_NOTIFY,
	ABUP_WRITE_NOTIFY,
	ABUP_CLOSE_NOTIFY,
	ABUP_ERROR_NOTIFY,
	ABUP_SOCMGR_END
} ABUP_SOCMGR_USR;


typedef struct 
{
	abup_notify abup_read_notify;
	abup_notify abup_write_notify;
	abup_notify abup_close_notify;
	abup_notify abup_net_error_notify;
	abup_int32 abup_soc_state;
	abup_int32 abup_request_id;
	abup_socint abup_tcp_socket_id;
	abup_int32 abup_net_sm;

} ABUP_SOCKET_MGR_STRUCT;


typedef enum 
{
	ABUP_READONLY	= 0x0001,
	ABUP_READWRITE	= 0x0002,
	ABUP_CREATE 	= 0x0004,
	ABUP_APPEND 	= 0x0008,
	ABUP_TRUNCATE = 0x0010
}ABUP_OpenMode;


typedef enum
{
	ABUP_SEEK_START,
	ABUP_SEEK_END,
	ABUP_SEEK_CURRENT
}ABUP_FileSeekType;


typedef struct
{
	abup_char timername[ABUP_TIMER_NAME_STRLEN];
	abup_uint32 timerid;
	abup_uint8 timermode;
	abup_uint32 delay;
	abup_timer_callback func_ptr;
	void *timer_ptr;
}ABUP_TIMER_STRUCT;

typedef enum 
{
	ABUP_DIFF_PATCH,
	ABUP_FULL_PATCH,
	ABUP_PATCH_END
} ABUP_PATCH_METHOD;

#include "nv_item_id.h"
typedef enum 
{
	MN_NV_ABUP_VERSION = MN_NV_ABUP_BASE,
	MN_NV_ABUP_LOGIN,
	MN_NV_ABUP_BLOCK,
	MN_NV_ABUP_IP,
}ABUP_NV_ITEM_ID;

typedef struct
{
	abup_uint16 state;
	abup_char versionbuff[200];
	//abup_char deltaID[50];
}ABUP_VERSION_STRUCT;

typedef struct
{
	abup_uint16 size;
	abup_char data[100];
}ABUP_LOGIN_STRUCT;

typedef struct
{
	abup_uint16 size;
	abup_char block_str[20];
}ABUP_BLOCK_STRUCT;

typedef struct
{
    abup_uint32 addr;
}ABUP_IP_STRUCT;

extern void abup_fota_upgrade(void);


#endif

