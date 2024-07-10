/*************************************************************************
 ** File Name:      mmidm_app.h                                          *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/02                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about dm function     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/02     jian.ma          Create.                              *
*************************************************************************/

#ifndef _MMIDM_APP_H_    
#define  _MMIDM_APP_H_   

/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "sci_types.h"
//#include "mmibrowser.h"
#include "mmisms_app.h"
#include "mmidm_export.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/
#define MMIDM_PATH_NAME             (g_mmidm_dir_wstr)                      //path name:dm
#define MMIDM_PATH_NAME_LEN         2                                       //path name length 

#define MMIDM_APN_MAX_LEN           (50 + 1)             //define dm apn max length
#define MMIDM_GATEWAY_IP_MAX_LEN    255                                     //define dm gateway ip len
#define MMIDM_PORT_MAX_LEN          30                                      //define dm port max len
#define MMIDM_SRNUM_MAX_LEN         30                                      //define dm server self register number
#define MMIDM_SRPORT_MAX_LEN        30                                      //define dm server self register port

#define DM_TASK_RUN_MESSAGE             0x1000
#define DM_TASK_EXIT_MESSAGE            0x1001
#define DM_TASK_START_MESSAGE           0x1002
#define DM_TASK_DL_CANCEL               0x1003
#define DM_TASK_DL_CLOSE                0x1004
#define DM_TASK_DM_CANCEL               0x1005
#define DM_TASK_DM_CLOSE                0x1006
#define DM_TASK_DM_EXIT                 0x1007
#define DM_TASK_DM_BROKEN               0x1008
// #define DM_TASK_DM_TIME_OUT             0x1008

#define DM_RADIX_TYPE                       10
#define DM_MAX_ID_STR                       32
#define DM_STRING_LENGTH                    255
#define DM_STRING_LENGTH_OLD                127
#ifdef DM_SUPPORT_REDBENCH
#define DM_SCOMO_APPID_MAX_NUMBER           500   
#else
#define DM_SCOMO_APPID_MAX_NUMBER           2    //只有redbend 支持SCOMO ，因此没必要太多
#endif
#define DM_MAX_DOWNLOAD_BUFFER_LEN      (20*1024)
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef enum 
{
     MMIDM_DEFER_FIVE_MINUTES,  //defer for 5 minutes     
     MMIDM_DEFER_ONE_HOUR,      //defer for 1 hour 
     MMIDM_DEFER_TWOFOUR_HOURS  //defer for 24 hour 
}MMIDM_DEFER_INDEX_E;


typedef enum  
{
    MMIDM_ALERT_NONE,          
    MMIDM_ALERT_PL_DISPLAY,      //display to user
    MMIDM_ALERT_PL_CONFIRM,      //need user to confirm
    MMIDM_ALERT_NIA_DISPLAY,     //nia display to user
    MMIDM_ALERT_NIA_CONFIRM,     //nia user to confirm
    MMIDM_ALERT_FUMO_CONFIRM,     //fumo need user to confirm
    MMIDM_ALERT_FUMO_UPDATE,      //fumo need user to update
    MMIDM_ALERT_SCOMO_CONFIRM,     //fumo display to user
    MMIDM_ALERT_SINGLE_CHOICE,     //single choice
    MMIDM_ALERT_MULTI_CHOICE,      //multi choice
    MMIDM_ALERT_MAX
} MMIDM_ALERT_TYPE_E;

typedef enum 
{
    MMIDM_MODE_NOTSPECIFIED,        //mode is not specified
    MMIDM_MODE_BACKGROUND,          //mode is background, need not to inform user
    MMIDM_MODE_INFORMATIVE,         //inform to user
    MMIDM_MODE_USERINTERACTION      //Need user to interaction
} MMIDM_UIMODE_E;

typedef enum  
{
    DM_SML_UNDEF = 0,
    DM_SML_WBXML,
    DM_SML_XML
} MMIDM_SMLENCODING_E;

typedef enum 
{
    DM_SESSION_NONE, 
    DM_SESSION_USER, 
    DM_SESSION_CLIENT, 
    DM_SESSION_SERVER, 
    DM_SESSION_FUMO_RESUME,
    DM_SESSION_SCOMO_RESUME,
    DM_SESSION_FOTA,
    DM_SESSION_REMOVE,
    DM_SESSION_ADD,
    DM_SESSION_OTHER
} DM_SESSION_TYPE;


typedef struct _DM_MSG_Tag
{
    DM_SESSION_TYPE type;
    char*       msg_body;
    uint32      msg_size;
} DM_MSG_T;

typedef struct  
{
    SIGNAL_VARS                         /*!<Signal Header.*/
    DM_MSG_T        content;            // message content
} DM_SIGNAL_T;

typedef enum 
{
    DM_WAP_APN,
    DM_WAP_USER_NAME,
    DM_WAP_PW,
    DM_WAP_PORT_NUM,
    DM_WAP_PROXY_ADDR,
    DM_WAP_HOME_PAGE,
} DM_WAP_INFO_TYPE;

typedef enum 
{
    DM_CMNET,
    DM_CMWAP,
} DM_CON_INFO_TYPE;

typedef enum 
{
    DM_MMS_MMSC, 
    DM_MMS_CONN_PROFILE,
} DM_MMS_INFO_TYPE;

typedef enum 
{
    DM_PIM_ADDR, 
    DM_PIM_ADDRESSBOOKURL,
    DM_PIM_CALENDARURL,
    DM_PIM_CONNPROFILE,
} DM_PIM_INFO_TYPE;

//streaming info
typedef enum 
{
    DM_ST_NAME, 
    DM_ST_PROXY,
    DM_ST_PROXYPORT,
    DM_ST_APN,
    DM_ST_NETINFO,
    DM_ST_MIN_UDP_PORT,
    DM_ST_MAX_UDP_PORT,
} DM_STREAMING_INFO_TYPE;

typedef struct  
{
    uint32      inCurrentProgress;
    uint32      inMaxProgress;
} MMIDM_DOWNLOAD_PROCESS_T;

typedef enum 
{
     DM_SESSION_BOOT,           //Bootstrap session     
     DM_SESSION_DM,             //DM session 
     DM_SESSION_DL,             //DM session
     DM_SESSION_DS              //DS session
}MMIDM_SESSION_TYPE_E;

typedef enum 
{
     DM_SESSION_START,          //Session has started     
     DM_SESSION_COMPLETE,       //Session completed successfully 
     DM_SESSION_ABORT           //Session aborted
}MMIDM_SESSION_STATE_E;

typedef enum 
{
     DL_SESSION_START,          //Session has started     
     DL_SESSION_COMPLETE,       //Session completed successfully 
     DL_SESSION_ABORT           //Session aborted
}MMIDL_SESSION_STATE_E;

typedef struct  
{
    MMIDM_SESSION_TYPE_E     inType;     
    MMIDM_SESSION_STATE_E    inState;
    uint32                   error;
} MMIDM_SESSION_CHANGE_T;

typedef struct  
{
    MMIDM_ALERT_TYPE_E       type;
    uint8*                   string;
    uint16                   str_len;
    uint32                   max_timer;
} MMIDM_NOTIFY_ALERT_T;

//define socket state
typedef enum 
{
    SOCKET_STATE_NONE,
    SOCKET_STATE_CONNECT,
    SOCKET_STATE_READ,
    SOCKET_STATE_WRITE,
    SOCKET_STATE_CLOSE,
    SOCKET_STATE_MAX,
}MMIDM_SOCKET_STATE;

//#ifndef MMI_DM_DEBUG
#define MMIDM_DEBUG_LOG     SCI_TRACE_LOW
//#else
//#define MMIDM_DEBUG_LOG
//#endif

//define pdp connect type
typedef enum 
{
    CONNECT_CMWAP,
    CONNECT_CMNET,
    CONNECT_UNKNOW
}MMIDM_PDP_CONNECT_TYPE;

//define mmi dm state
typedef enum 
{
    DM_NONE,        //dm init state
    DM_START,       //dm starte state
    DM_RUN,         //dm run state
    DM_CANCEL,      //dm cancel state
}MMIDM_DM_STATE;

typedef enum
{
    DM_NONE_SET,
    DM_APN_SET,
    DM_MON_SET,
    DM_DEV_VERSION_SET,
    DM_MAN_SET,
    DM_IMEI_SET,
    DM_SRV_ADDR,
    DM_SELF_REG_NUM,
    DM_SELF_REGPORT_NUM,
    DM_CNNECT_SERVER,
} MMIDM_DEBUG_TYPE_E;

typedef enum
{
    RESUME_TYPE_FUMO,
    RESUME_TYPE_SCOMO,
    RESUME_TYPE_CMCC_MENU,
    RESUME_TYPE_FUMO_DATA,
    RESUME_TYPE_SCOMO_DATA,
    RESUME_TYPE_MAX,
}MMIDM_RESUME_TYPE;

typedef enum
{
    DM_BROKEN_TYPE_NONE,
    DM_BROKEN_TYPE_DM,
    DM_BROKEN_TYPE_DL,
    DM_BROKEN_TYPE_MAX,
}MMIDM_BROKEN_TYPE;


typedef enum
{
    DM_PL_ALERT_NONE,
    DM_PL_ALERT_1100,
    DM_PL_ALERT_1101,
    DM_PL_ALERT_1102,
    DM_PL_ALERT_1103,
    DM_PL_ALERT_1104,
    DM_PL_ALERT_MAX,
}MMIDM_PL_ALERT_TYPE;

typedef struct  
{
    char*               message_buffer;
    uint32              msg_size;
    DM_SESSION_TYPE     type;
} MMIDM_SESSION_T;

typedef enum 
{
    DM_SELF_REG_NONE,
    DM_SELF_REG_SUCC,
    DM_SELF_REG_FAIL,
}MMIDM_SELF_REG_E;


typedef enum 
{
    DM_SPREADTRUM,
    DM_LGOPHONE,
    DM_NEWPOST,
    DM_OTHER,
}MMIDM_TEST_SERVER_E;

typedef struct
{
    wchar   path[MMIFMM_FILE_FILE_NAME_MAX_LEN+1];
    uint32  path_len;
    wchar   name[MMIFMM_FILE_FILE_NAME_MAX_LEN+1];
    uint32  name_len;
} MMIDM_EXT_FILE_INFO_T;

//define mmi fota update state
typedef enum 
{
    DM_FOTA_UPDATA_NONE,
    DM_FOTA_UPDATA_START,
    DM_FOTA_UPDATA_DONE,
    DM_FOTA_UPDATA_FAIL,
}FOTA_UPDATA_STATE_E;

typedef struct  
{
    uint32  all_size;
    uint8   name[DM_STRING_LENGTH_OLD + 1];
    uint8   version[DM_STRING_LENGTH_OLD + 1];
    uint8   provider[DM_STRING_LENGTH_OLD + 1];
    uint32  jad_size;
 }MMIDM_APP_PAKEAGE_OLD_T;

typedef struct  
{       
    uint32  all_size;                       //表示包的大小,包括magic，struct_vesion 和 padd
    uint32  magic;                           //0x444D4150， "DMAP"，"DM Application Package"
    uint16  package_version;                //包结构的版本，当前版本0.1,即0x0001
    uint16  padd;                            //reserved.
    wchar   name[DM_STRING_LENGTH+1];       //Java文件的文件名
    wchar   version[DM_STRING_LENGTH + 1];   //Java文件的版本
    wchar   provider[DM_STRING_LENGTH + 1];  //Java提供者
    uint32  jad_size;                       //Jad的size,用于计算jar的起点位置
}MMIDM_APP_PAKEAGE_T;

typedef struct
{
    uint32  max_len;
    uint32  buf_len;
    uint32  cur_len;
    uint32  total_len;
    uint8*  buffer_ptr;
    wchar   file_name[MMIFMM_FILE_FILE_NAME_MAX_LEN+1];
}MMIDM_DOWNLOAD_DATA_T;

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jian.ma
//  Note
/*****************************************************************************/
PUBLIC void MMIDM_Init(void);

/*****************************************************************************/
//  Description : Register dm menu group
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_RegMenuGroup(void);

/*****************************************************************************/
//  Description : register dm module nv len and max item
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_RegDmNv(void);

/*****************************************************************************/
//  Discription: This function is to get dm state
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  MMIDM_DM_STATE  MMIDM_GetDmState(void);

/*****************************************************************************/
//  Discription: This function is to set dm state
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void  MMIDM_SetDmState(MMIDM_DM_STATE state);

/*****************************************************************************/
//  Description : create dm task
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDM_CreateTask(void);

/*****************************************************************************/
//  Discription: send signal to DM Task
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIDM_SendSigToDmTask(
                                      uint32    event_id, 
                                      uint32    task_id,
                                      MMIDM_SESSION_T *session_info
                                      );

/*****************************************************************************/
//  Description : get pdp status
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_PDP_STATUS_E  MMIDM_GetPdpActiveStatus(void);


/*****************************************************************************/
//  Discription: handle sms MT ind
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/ 
PUBLIC  BOOLEAN MMIDM_HandleSmsMtInd(
                                        MN_DUAL_SYS_E dual_sys,
                                        uint8   *number_ptr,
                                        uint16   num_len,
                                        APP_SMS_USER_DATA_T  sms_user_data
                                       );

/*****************************************************************************/
//  Description : get is self register ready
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_SELF_REG_E  MMIDM_GetIsSrReady(void);


/*****************************************************************************/
//  Description : Set is self register ready
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetIsSrReady(MMIDM_SELF_REG_E  is_ready);

/*****************************************************************************/
//  Description : 判断当前SIM卡跟IMEI号是否已经注册，若是，那么就不用再次发送自注册短信，若否，
//          则发送如下格式的自注册短信:
//          IMEI:IMEI number/manufacture/mode/sw version
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SendSelfRegisterSMS(MN_DUAL_SYS_E      dual_sys);

/*****************************************************************************/
//  Description : start dm session
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_StartDm(DM_SESSION_TYPE type,char* msg_body, uint32 msg_size);

/*****************************************************************************/
//  Description : exit dm session
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ExitDM(void);


/*********************************************************************************
//  Description : broken dm session
//  Global resource dependence : 
//  Author: 
//  Note:
*********************************************************************************/
PUBLIC void MMIDM_BrokenDMSession(void);

/*****************************************************************************/
// Description : This function retrieves the device Manufacturer from
//          Non-volatile memory.
// return String that holds the name of the Manufacturer. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetMan(void);

/*****************************************************************************/
// Description : This function retrieves the device Model from Non-volatile memory.
// return String that holds the name of the device Model.  
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetModel(void);

/*****************************************************************************/
// Description : This function retrieves the software version information
//          from Non-volatile memory.
// return String that holds the software version information.  
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetSoftwareVersion(void);

/*****************************************************************************/
// Description : This function retrieves the DM client version from Non-volatile memory
// return String that holds the DM version. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetDMVersion(void);

/*****************************************************************************/
// Description : This function returns the regional language. 
// return String that holds the language the handset is configured. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetLanguage(void);

/*****************************************************************************/
// Description : This function retrieves the device type from Non-volatile memory.
//          Possible values are PDA, pager, or phone. 
// return String that holds the device type. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetDeviceType(void);

/*****************************************************************************/
// Description : This function retrieves the OEM identifier from Non-volatile memory.
// return String that holds the OEM identifier. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetOEM(void);

/*****************************************************************************/
// Description : This function retrieves the firmware version information from
//          Non-volatile memory.
// return String that holds the firmware version information. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetFirmwareVersion(void);

/*****************************************************************************/
// Description : This function retrieves the device Hardware version information
//          from Non-volatile memory.
// return String that holds the Hardware version information. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetHardwareVersion(void);

/*****************************************************************************/
//  Description : get device imei str
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC  char* DM_GetDevImeiInfo(void);

/*****************************************************************************/
//  Description : get dm profile
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetDmProfile(void);

/*****************************************************************************/
//  Description : set dm profile
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetDmProfile( char* info,uint32 length);

/*****************************************************************************/
//  Description : get cmnet/cmwap  apn info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetGPRSInfo(DM_WAP_INFO_TYPE type,DM_CON_INFO_TYPE con_type);

/*****************************************************************************/
//  Description : set cmnet/cmwap  apn info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetGprsInfo(DM_WAP_INFO_TYPE type,char* info,uint32 length,DM_CON_INFO_TYPE con_type);

/*****************************************************************************/
//  Description : get Get dm server url
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetServerUrl(void);

/*****************************************************************************/
//  Description : get wap info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* DM_GetWAPInfo(DM_WAP_INFO_TYPE type);

/*****************************************************************************/
//  Description : set wap info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DM_SetWAPInfo(DM_WAP_INFO_TYPE type, char* info,uint32 length);

/*****************************************************************************/
//  Description : get mmsc info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetMMSCInfo(DM_MMS_INFO_TYPE type);

/*****************************************************************************/
//  Description : set mmsc info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetMMSCInfo(DM_MMS_INFO_TYPE type, char* info,uint32 length);

/*****************************************************************************/
//  Description : get PIM info 
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_GetPIMInfo(DM_PIM_INFO_TYPE type, char *str_ptr, uint16 max_count);

/*****************************************************************************/
//  Description : set PIM info 
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetPIMInfo(DM_PIM_INFO_TYPE type,char* info,uint32 length);


/*****************************************************************************/
//  Description : get streaming info 
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetStreamingInfo(DM_STREAMING_INFO_TYPE type);

/*****************************************************************************/
//  Description : set streaming info 
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetStreamingInfo(DM_STREAMING_INFO_TYPE type,char* info,uint32 length);

/*****************************************************************************/
//  Description : get file full path
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetFileFullPath(
                                     wchar *file_name,      //in
                                     uint16 file_name_len,  //in
                                     wchar* full_path,      //out
                                     uint16* full_path_len  //out
                                     );

/*****************************************************************************/
//  Description : open dm main menu win
//  Global resource dependence : 
//  Author:jian.ma
/*****************************************************************************/
PUBLIC void MMIDM_OpenDmMainMenu(void);

/*****************************************************************************/
//  Description : close waiting win             
//  Global resource dependence : 
//  Author: jian.ma
//  Note:   
/*****************************************************************************/
PUBLIC void MMIDM_CloseWaitingWin(void);

/*****************************************************************************/
//  Description : get Dm task id             
//  Global resource dependence : 
//  Author: jian.ma
//  Note:   
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetDmTaskID(void);

/*****************************************************************************/
//  Description : active pdp context
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ActivePdpContext(void);

/*****************************************************************************/
//  Description : active pdp context
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_DeactivePdpContext(void);

/*****************************************************************************/
//  Description : active pdp connect
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ActivePdpConnect(void);

/*****************************************************************************/
//  Description : 处理进度条更新，在DM_TASK中将数据发给APP_TASK处理
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ProgressUpdateCallBack(MMIDM_DOWNLOAD_PROCESS_T *data);

/*****************************************************************************/
//  Description : deal with dm server lock message
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ServerLockCallBack(void);

/*****************************************************************************/
//  Description : deal with dm server unlock message
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ServerUnlockCallBack(void);


/*****************************************************************************/
//  Description : handle dm message
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_HandleDmMessage(DPARAM     sig_data_ptr);

/*****************************************************************************/
//  Description : reset dm factory setting
//  Global resource dependence : none
//  Author:jianhui.luo 
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : trace string content
//  Global resource dependence : none
//  Author:jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_TraceString(char* inBuffer, uint32 bufferlen);

/*****************************************************************************/
//  Description : deal with dm session state change
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SessionChangeCallBack(MMIDM_SESSION_CHANGE_T *data);



/*****************************************************************************/
//  Description : DM task send signal to MMI task
//  Global resource dependence : 
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SendSignalToMMITask(uint16 sig_id,uint16 event_id, void *data_ptr);

/*****************************************************************************/
//  Description : Notify alert call back
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_NotifyAlertCallBack(MMIDM_NOTIFY_ALERT_T *data);

/*****************************************************************************/
//  Description :handle socket message
//  Global resource dependence : 
//  Author:jianhui.luo
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_HandleSocketMessage(MMIDM_SOCKET_STATE type,int32 sock_id);

/*****************************************************************************/
//  Description : set dm para info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetDmParaInfo(MMIDM_DEBUG_TYPE_E type,char* string,uint32 len);

/*****************************************************************************/
//  Description : Get dm para info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_GetDmParaInfo(MMIDM_DEBUG_TYPE_E type,char* string);

/*****************************************************************************/
//  Description : set pdp connect type
//  Global resource dependence : 
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_PDP_CONNECT_TYPE MMIDM_GetPdpConnectType(void);

/*****************************************************************************/
//  Description : Get dev free space
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetDevFreeSpace(void);

/*****************************************************************************/
//  Description : error alert
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ErrorAlert(uint32 error,MMIDM_SESSION_TYPE_E type);

/*****************************************************************************/
//  Description : open dm debug win
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_OpenDebugMenuWin(void);

/*****************************************************************************/
//  Description : get pdp net id
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetPdpNetID(void);

/*****************************************************************************/
//  Description : vdm update package
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_InstallUpdatePackage(void);

/*****************************************************************************/
//  Description : check update package exist
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_CheckUpdateFile(void);


/*****************************************************************************/
// Description : set nv handset lock state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetNVHandsetLock(BOOLEAN state);

/*****************************************************************************/
// Description :get  handset lock state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetHandsetLock(void);

/*****************************************************************************/
//  Description :close  lock win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_CloseLockWin(void);

/*****************************************************************************/
//  Description : handle fumo update timer 
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandleDmTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : get fumo update timer id
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsDmTimerId(uint32 timer_id);

/*****************************************************************************/
// Description : set nv down load finish flag
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetNVDownloadFinish(BOOLEAN is_finish);

/*****************************************************************************/
// Description : get nv down load finish flag
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetNVDownloadFinish(void);

/*****************************************************************************/
// Description : set debug mode
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetDebugMode(BOOLEAN mode);
/*****************************************************************************/
// Description :DM Is debug mode 
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsDebugMode(void);

/*****************************************************************************/
// Description :get idasdec type
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetIdasdecType(void);

/*****************************************************************************/
//  Description : get dl state
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMIDM_DM_STATE MMIDM_GetDlState(void);

/*****************************************************************************/
//  Description : set dl state
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetDlState(MMIDM_DM_STATE state);

/*****************************************************************************/
//  Description : get fota update state
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC FOTA_UPDATA_STATE_E MMIDM_GetFotaUpdateState(void);

/*****************************************************************************/
//  Description : set fota update state
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetFotaUpdateState(FOTA_UPDATA_STATE_E state);

/*****************************************************************************/
//  Description : create java app pak
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CreateJavaAppPak(void);

/*****************************************************************************/
//  Description : open dm alert win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ScomoInstallCallback(int suiteID,
                                       uint16*version,
                                       uint16 version_len,
                                       uint16* name,
                                       uint16 name_len);

/*****************************************************************************/
//  Description : java remove callback
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ScomoRemoveCallback(int suiteID);

/*****************************************************************************/
//  Description : scomo install
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ScomoInstall(BOOLEAN isBackgroup);

/*****************************************************************************/
//  Description : get test server type
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC  MMIDM_TEST_SERVER_E  MMIDM_GetTestServerType(void);

/*****************************************************************************/
//  Description : set test server type
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIDM_SetTestServerType( MMIDM_TEST_SERVER_E  type);

/*****************************************************************************/
// Description : set log state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetLogState(BOOLEAN mode);

/*****************************************************************************/
// Description :DM get log state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetLogState(void);

/*****************************************************************************/
//  Description : is dm app install
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetIsDmAppInstall(void);

/*****************************************************************************/
//  Description : set dm app install
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetDmAppInstall(BOOLEAN is_install);


/*****************************************************************************/
// Description : set fota state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetFotaState(BOOLEAN mode);

/*****************************************************************************/
// Description :DM get fota state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetFotaState(void);

/*****************************************************************************/
//  Description : dl suspend
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsNeedDelayDMSession(void);

/*****************************************************************************/
// Description : set tree state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetTreeState(BOOLEAN mode);

/*****************************************************************************/
// Description :DM get tree state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetTreeState(void);

/*****************************************************************************/
//  Description : dm open alert message
//  Global resource dependence : 
//  Author:jian.ma
/*****************************************************************************/
PUBLIC void MMIDM_OpenDmSessionAlertWin(
                                        MMIDM_ALERT_TYPE_E mode, 
                                        const uint8* text,
                                        uint16 len,
                                        uint32 max_dis_timer
                                        );

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:Jiaoyou.wu
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsInConform(void);

/*****************************************************************************/
//  Description : to handle dm idle option
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_HandleDmIdleOption(void);

/*****************************************************************************/
//  Description : SendFotaReport
//  Global resource dependence : none
//  Author: jian.ma
//  Note
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SendFotaReport(void);

/*****************************************************************************/
// Description : set dm reg switch state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetDmRegSwitchState(BOOLEAN mode);

/*****************************************************************************/
// Description :DM dm reg switch state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetDmRefSwitchState(void);

/*****************************************************************************/
// Description :DM get reg times
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetRegTimes(void);

/*****************************************************************************/
//  Description : clean install file
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_CleanInstallFile(void);

/*****************************************************************************/
//  Description : delete dm folder files
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_DelDmFolderFiles(void);

/*****************************************************************************/
//  Discription: This function is to get dm session type
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  DM_SESSION_TYPE  MMIDM_GetDmSessionType(void);
/*****************************************************************************/
//  Description : set is need exit dm session
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetExitDmState(BOOLEAN exit_state);

/*****************************************************************************/
//  Description : add scomo id to tab
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_AddScomoIdToTab(int id);

/*****************************************************************************/
//  Description : del scomo id to tab
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_DelScomoIdToTab(int id);

/*****************************************************************************/
//  Description : init scomo id to tab
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_InitScomoIdToTab(void);

/*****************************************************************************/
//  Description : check dl space enouge
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_CheckIsDlSpaceEnouge(void);

/*****************************************************************************/
//  Description : to create dm run check timer
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CreateDmRunCheckTimer(void);

/*****************************************************************************/
//  Description : to stop dm run check timer
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_StopDmRunCheckTimer(void);

/*****************************************************************************/
//  Discription: This function is dm run
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIDM_IsDmRun(void);

/*****************************************************************************/
//  Discription: This function restart dm session
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ReStartDmSession(void);

/*****************************************************************************/
//  Discription: This function reset dm push info
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIDM_ReSetDmPushInfo(void);

/*****************************************************************************/
//  Description :handle dm rx change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandDmRxChange(uint8 rx_level);


/*****************************************************************************/
//  Description : handle create no sig timer
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_CreateNoSigTimer(void);

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandDmCallClose(void);

/*****************************************************************************/
//  Description :set is need restart flag
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetIsNeedRestart(BOOLEAN is_need_restart);

/*****************************************************************************/
//  Description :get need restart flag
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetIsNeedRestart(void);

/*****************************************************************************/
// 	Description : to create dm open csoket check timer
//	Global resource dependence : 
//  Author:mary.xiao
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CreateDmSocketCheckTimer(void);

/*****************************************************************************/
// 	Description : to stop dm run check timer
//	Global resource dependence : 
//  Author:jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_StopDmSocketCheckTimer(void);

/*****************************************************************************/
//  Description : to create dm delay check timer
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CreateDelayTimer(uint32 timer);

/*****************************************************************************/
//  Description : to close wap check  timer
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CloseDelayTimer(void);

/*****************************************************************************/
//  Description : to close NoSig check  timer
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CloseNoSigTimer(void);

/*****************************************************************************/
//  Description :handle cc close 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandWapClose(void);

/*****************************************************************************/
//  Discription: This function check is wap active
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsWapActive(void);

/*****************************************************************************/
//  Description : handle create no sig timer
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_CreateFotaReportTimer(void);

/*****************************************************************************/
//  Description : close dl session progress win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/****************************************************************************/
PUBLIC void MMIDM_CloseDLSessionProgressWin(void);

/*****************************************************************************/
//  Description :handle network change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandleNetChange(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : to get NoSig check  timerid
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetNoSigTimerId(void);

/*****************************************************************************/
//  Description : clean resume nv
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIDM_CleanResumeNv(void);

/*****************************************************************************/
//  Description : get resume type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMIDM_RESUME_TYPE MMIDM_GetResumeType(void);



/*****************************************************************************/
//  Description : Set dm broken type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetBrokenType(MMIDM_BROKEN_TYPE type);
/*****************************************************************************/
//  Description : Get dm broken type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMIDM_BROKEN_TYPE MMIDM_GetBrokemType(void);

/*****************************************************************************/
//  Description : to get delay timerid
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetDelayTimerId(void);

/*****************************************************************************/
//  Description : to handle dm option
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_HandeDmOption(void);

/*****************************************************************************/
//  Description : get srv add URL
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetSrvAddURL(void);

/*****************************************************************************/
//  Description : dm close all win
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_CloseAllWin(void);

/*****************************************************************************/
//  Description : is need handle delay event
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsNeedHandleDelayEvent(void);

/*****************************************************************************/
//  Description : set need handle delay event
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetNeedHandleDelayEvent(BOOLEAN is_need_handle);

/*****************************************************************************/
//  Description : set sms ready
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetSmsReady(BOOLEAN is_ready);

/*****************************************************************************/
// 	Description : set the sim dm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/ 

PUBLIC void MMIDM_SetMainSim(MN_DUAL_SYS_E sim);

/*****************************************************************************/
// 	Description : get the sim dm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/  
PUBLIC MN_DUAL_SYS_E MMIDM_GetMainSim(void);

/*****************************************************************************/
//  Description : set net ready
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetNetReady(BOOLEAN is_ready);
/*****************************************************************************/
//  Description : get LOCK STATUS CODE
//  Global resource dependence : none
//  Author: mary.xiao 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetLockStatusCode(void);
/*+++ cr251729*/
/*****************************************************************************/
// 	Description : handle create broken session timer
//	Global resource dependence : none
//  Author:qing.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_CreateBrokenSessionTimer(void);

/*****************************************************************************/
// 	Description : to close broken session timer
//	Global resource dependence : 
//  Author:qing.yu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CloseBrokenSessionTimer(void);

/*****************************************************************************/
// 	Description : to broken session timer
//	Global resource dependence : 
//  Author:qing.yu
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetBrokenTimerId(void);
/*--- cr251729*/

/*****************************************************************************/
//  Description : clean dm data
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CleanDmData(void); //NEWMS00125131

/*****************************************************************************/
// 	Description : Checking whether scomo file is exist
//	Global resource dependence : none
//  Author:qing.yu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsScomoFileExist(void);

/*****************************************************************************/
// 	Description : dm  MMIDM_Config_GetEncodeWBXMLMsg
//	Global resource dependence : 
//  Author:taiping.lai
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_Config_GetEncodeWBXMLMsg(void);

#ifdef   __cplusplus
    }
#endif

#endif
