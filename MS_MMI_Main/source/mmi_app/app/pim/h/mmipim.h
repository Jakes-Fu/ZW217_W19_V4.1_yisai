


/******************************************************************************
                                   海信TD-SCDMA  自主业务模块
        本模块设计基于OMA  联盟提出的基于syncml 协议的数据和终
        端管理，实现PIM业务。 模块开发参考OMA成员共同开发的
        SRT_c  demo。 目前模块计划开发支持OMA  syncML 协议1.1

        版权声明:本软件版权属于海信通信，保留一切法律权利
        开发人员:王晓林
        创建日期:2007年9月22日。
******************************************************************************/


/******************************************************************************
文件名称:mmipim.h
文件描述:PIM应用头文件。
文件说明:应用注册初始化以及对其他应用提供接口。
开发人员:王晓林
首次开发日期:2007-9.22
******************************************************************************/


/******************************************************************************
                                                            修改记录
  日期                   作者                修改原因
  -----------------------------------------------------------------------------
  2007-09-22            王晓林                第一次开发编码。
  -----------------------------------------------------------------------------
  *****************************************************************************/

  #ifndef _MMIPIM_H_
#define _MMIPIM_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
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

/****************************************************************
                             FEATURE 定义
*****************************************************************/
#define FEATURE_PIM_DEBUG // chenxiang 20080310
//#define HS_PIM_DISPALY_MALLOC_NUM //是否在同步界面显示当前使用的动态内存个数
//#define PIM_MEMORY_LEAK_DEBUG  //可以检查内存泄露，将泄露的内存在下次同步时通过log输出

#define HS_PIM_LOG_FILE
#define HS_PIM_UA_SETTING 
#define HS_PIM_CACHE_MAP // wangxiaolin 20090804 保存未上传的map信息, 在下次同步时上传
#define  HS_PIM_SUPPORT_OTHER_SYNC_TYPE
#define  HS_PIM_SUPPORT_ALL_FIELD
#define PIM_SUPPORT_LARGE_OBJECT_PROC

//MS00216980 cheney
#ifdef HS_PIM_UA_SETTING
#define PIM_UA_TYPE_DEFAULT  0
#define PIM_UA_TYPE_CUSTOM 1
#endif

//MS00216980 cheney
#define PIM_SRV_ADDR_URL              "http://pim.monternet.com" 
#define PIM_SRV_ADDR_URL_TEST    "http://218.206.176.241:8888/sync"

//当前动态内存问题已经不大，但对于上传的map，服务器只对第一个包有效
#define PIM_ONLY_CACHE_LAST_MESSAGE_MAP 

#define PIM_CMCC_TEST_1001 //中国移动常规测试1001解决方案

/*Start of  wangzhefeng 2009.9.2 for hisense vcalendar */
//增加vcalendar功能
//#define HS_PIM_VCAL  //add by wangzhefeng 2009.09.02
/*End   of  wangzhefeng 2009.9.2 for hisense vcalendar */


#define MMIPIM_MAX_STR_LEN 255
#define MMIPIM_MAX_APN_LEN	50
#define MMIPIM_MAX_USER_LEN	50
#define MMIPIM_MAX_PASSWORD_LEN	 20
#define MMIPIM_MAX_IP_LEN	20
#define MMIPIM_MAX_PORT_LEN	5
#define MMIPIM_MAX_SERVER_ADDR_LEN	100
#define MMIPIM_MAX_DATABASE_LEN	30
#define MMIPIM_MAX_URL_LEN	200
#define MMIPIM_ANCHOR_LEN 40
#define MMIPIM_NEXT_NONCE 50

#define PIM_MAX_MSG_SIZE                    10240
 //每个message的最大值


#define PIM_MAX_HEAD_DATA_LEN         2000     //每个message buffer中除vcard数据外的长度

#define PIM_MAX_VCARD_DATA_LEN (PIM_MAX_MSG_SIZE-PIM_MAX_HEAD_DATA_LEN)  //每个message buffer中最大vcard数据长度

#define  VCARD_LEN_EXCLUDE_PHOTO_SEGMENT 2048

#define MAX_NUM_IN_ONE_MSG_BUFFER  50

#ifdef PIM_SUPPORT_LARGE_OBJECT_PROC  //Added by wangxiaolin  2009.7.9 for 大对象
#define BASE64_BUF_LEN (20*1024)
#define PHOTO_FILE_LEN (15*1024)   //将上传图片大小限定为15K
#define MAX_DATA_BUF_LEN (BASE64_BUF_LEN+VCARD_LEN_EXCLUDE_PHOTO_SEGMENT)
#endif /* PIM_SUPPORT_LARGE_OBJECT_PROC */


#define MSG_MED_LOG PIM_Log 

#ifdef FEATURE_PIM_DEBUG
            //#define syncml_message(x)  DBGPRINTF x
           #define syncml_message_log(xx_fmt)  MSG_MED_LOG(__FILE__,__LINE__,(char*)xx_fmt)
            
            #define syncml_message(x)  SCI_TRACE_LOW x

            //打开efs存储的调试信息
            #define FEATURE_EFS_LOG
            
            //打开callback的调试信息
            #define FEATURE_CALLBACK_LOG
            
            //打开http层的调试信息
            #define FEATURE_HTTP_LOG
            
            //打开通讯封装层的调试信息
            #define FEATURE_COMM_LOG
            
            //打开主模块动作的调试信息
            #define FEATURE_TASK_LOG
            
            //打开syncml core动作的调试信息
            #define FEATURE_SYNCML_CORE_LOG

            //编解码子模块的控制宏
           // #define  FEATURE_SYNCML_CODEC_LOG

            //vcard解析控制
          #define  FEATURE_VCARD_PARSE_LOG
          
          #ifdef FEATURE_EFS_LOG
                #define syncml_efs_message(x) syncml_message(x)
          #else
                #define syncml_efs_message(x)  
          #endif
          
          #ifdef FEATURE_CALLBACK_LOG
               #define syncml_cb_message(x) syncml_message(x)
          #else
               #define syncml_cb_message(x)
          #endif
          
          #ifdef FEATURE_HTTP_LOG
               #define syncml_http_message(x) syncml_message(x)
          #else
               #define syncml_http_message(x)
          #endif
          
          #ifdef FEATURE_COMM_LOG
               #define syncml_comm_message(x) syncml_message(x)
          #else
               #define syncml_comm_message(x)
          #endif
          
          #ifdef FEATURE_TASK_LOG
                #define syncml_task_message(x) syncml_message(x)
          #else
               #define syncml_task_message(x)
          #endif

          #ifdef FEATURE_SYNCML_CORE_LOG
                #define syncml_core_message(x) syncml_message(x)
          #else
               #define syncml_core_message(x)
          #endif

         #ifdef FEATURE_SYNCML_CODEC_LOG
                #define syncml_codec_message(x) syncml_message(x)
          #else
               #define syncml_codec_message(x)
          #endif
          
         #ifdef FEATURE_VCARD_PARSE_LOG
                #define syncml_vcard_parse_message(x) syncml_message(x)
          #else
               #define syncml_vcard_parse_message(x)
          #endif
          
  #else
    #define syncml_message(x)
    #define syncml_efs_message(x)
    #define syncml_cb_message(x)
    #define syncml_http_message(x)
    #define syncml_comm_message(x)
    #define syncml_task_message(x)
    #define syncml_core_message(x)
    #define syncml_codec_message(x)
    #define syncml_vcard_parse_message(x)
  #endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    PIM_GPRS_DEACTIVE_OK,	//DEACTIVE完成
    PIM_GPRS_ACTIVING,      //active processing
    PIM_GPRS_ACTIVE_OK,		//ACTIVE完成
    PIM_GPRS_DEACTIVEING,		//DEACTIVE进行中
    PIM_GPRS_PDP_REJECT,
    PIM_GPRS_MAX
}PIM_GPRS_STATE_E;//GPRS的状态

typedef enum
{
	PB_NO_MODIFY = 0,		//未修改的记录
	PB_RECENT_ADD,		//新加的记录
	PB_RECENT_REPLACE,	//新修改的记录
	PB_RECENT_DELETE,		//新删除的记录
	PB_RECENT_INVALID
}PB_PIM_STATUS_E;

#ifdef HS_PIM_VCAL  //Added by wangzhefeng  2009.9.4
typedef enum
{
	VCAL_NO_MODIFY = PB_NO_MODIFY,//未修改的记录
	VCAL_RECENT_ADD = PB_RECENT_ADD,//新加的记录
	VCAL_RECENT_REPLACE = PB_RECENT_REPLACE,//新修改的记录
	VCAL_RECENT_DELETE = PB_RECENT_DELETE,//新删除的记录
	VCAL_RECENT_INVALID = PB_RECENT_INVALID
}VCAL_PIM_STATUS_E;
#endif /* HS_PIM_VCAL */
//chenxiang 20071101 pim_setting
typedef struct
{     char  apn[MMIPIM_MAX_APN_LEN + 1];                //APN
	char  username[MMIPIM_MAX_USER_LEN + 1];  //用户
	char  password[MMIPIM_MAX_PASSWORD_LEN + 1];   //密码
 }net_setting_info_type;

//add by wangxiaolin
typedef struct
{     int8     use_proxy;
	char  addr[MMIPIM_MAX_IP_LEN + 1];                //使用代理情况下代理服务器IP地址
	char  port[MMIPIM_MAX_PORT_LEN + 1];            //使用代理情况下代理服务器端口
	char  username[MMIPIM_MAX_USER_LEN + 1];  //使用代理情况下代理服务器用户名
	char  password[MMIPIM_MAX_PASSWORD_LEN + 1];   //使用代理情况下代理服务器用户密码
 }proxy_setting_info_type;

typedef struct
{
       char pbdb[MMIPIM_MAX_DATABASE_LEN + 1];
	char cddb[MMIPIM_MAX_DATABASE_LEN + 1];
	char  addr[MMIPIM_MAX_URL_LEN + 1];
	char  port[MMIPIM_MAX_PORT_LEN + 1];
	char  username[MMIPIM_MAX_USER_LEN + 1];
	char  password[MMIPIM_MAX_PASSWORD_LEN + 1];
 }server_setting_info_type;

//chenxiang 20071101 pim_setting
typedef struct
{
       char reg_cancel[MMIPIM_MAX_URL_LEN + 1];
	char net_query[MMIPIM_MAX_URL_LEN + 1];
	char  pay_index[MMIPIM_MAX_URL_LEN + 1];
 }wap_url_info_type;

typedef enum
{
      AUTH_DUMMY=0,
	AUTH_B64,
	AUTH_MD5
}auth_type;

  /** Type of used encoding **/
typedef enum {
	SML_UNDEF = 0,
        SML_WBXML,
        SML_XML
} SmlEncoding_t;
//chenxiang 20071206
typedef struct
{
    uint16 recent_add_count; // 上次同步成功以来新添加的电话簿记录个数
    uint16 recent_replace_count; // 上次同步成功以来新替换的电话簿记录个数
    uint16 recent_delete_count; // 上次同步成功以来新删除的电话簿记录个数
 }recent_status_count_type;

/*supported sync type*/
typedef enum
{
   SYNC_TYPE_DUMMY=0,          //0:dummy
   SYNC_TYPE_2WAY,                // 1: two way sync
   SYNC_TYPE_S2WAY,              // 2: slow two way sync
   SYNC_TYPE_1WAYFCLI,        // 3: one way sync from client only
   SYNC_TYPE_RFCLI,               // 4: refresh sync from client only
   SYNC_TYPE_1WAYFSER,       // 5: one way sync from server only
   SYNC_TYPE_RFSER,              // 6: refresh sync from server only
   SYNC_TYPE_SERALERT        // 7: server alerted sync
}sync_type;

#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
//增加同步的类型:电话簿/日程
typedef enum
{
    SYNC_MODE_PB, //同步电话簿
    SYNC_MODE_VCAL, //同步日程
    SYNC_MODE_MAX 
 }SYNC_MODE;
#endif /* HS_PIM_VCAL */

//用来显示同步操作的结果
typedef struct 
{
    BOOLEAN delete_is_final;
    BOOLEAN add_is_final;
    BOOLEAN replace_is_final;
    uint16 receive_num;
    uint16 send_num;
    uint16 send_num_bak;
    uint16 current_num_for_add;
    uint16 current_num_for_replace;
    uint16 current_num_for_delete;
    uint16 deleted_num_for_server;
    uint16 added_num_for_server;
    uint16 replaced_num_for_server;
    uint16 deleted_num_for_phone;
    uint16 added_num_for_phone;
    uint16 replaced_num_for_phone;   
    uint16 server_number_of_change; // chenxiang_pim_ui 20080306
    uint16 client_number_of_change; // chenxiang_pim_ui 20080306
    uint16 real_deleted_num_for_server;
    uint16 real_added_num_for_server;
    uint16 real_replaced_num_for_server;
} pim_sync_result;
 

typedef  struct
{
    BOOLEAN	is_init;
    BOOLEAN	is_proxy;

    SCI_TIMER_PTR  m_psocket_select_timer;
    /* 网络激活状态*/
    int               m_pISocket;
    PIM_GPRS_STATE_E	gprs_state;
    pim_sync_result g_sync_result;
    uint32    m_netid;    /* net interface ID, cr166376, @yifeng.wang */

    /* 业务设置相关选项(nv) */
    auth_type   m_authtype;
    SmlEncoding_t  m_sml_encoding_type;
    net_setting_info_type m_net_setting; // chenxiang pim_setting
    proxy_setting_info_type m_proxy_setting;
    server_setting_info_type m_server_setting;
    wap_url_info_type m_url_setting; // chenxiang pim_setting
    recent_status_count_type m_recent_status_count; // chenxiang 20071206
    uint32 m_ua_type; // chenxiang 20080318
    sync_type m_sync_type;//add by wangxiaolin 2009.07.16
#ifdef HS_PIM_VCAL //Added by wangzhefeng  2009.9.10
    recent_status_count_type m_vcal_recent_status_count; // chenxiang 20071206
    SYNC_MODE  m_sync_mode;//0:同步电话簿1:同步日程
#endif /* HS_PIM_VCAL */
}PIM_T,*PIM_T_P;

typedef enum _PIM_ENTRY_TYPE_
{
    PIM_ENTRY_FROM_MAIN = 0,					
    PIM_ENTRY_FROM_PB					
} PIM_ENTRY_TYPE;


#define MMIPIM_UA_CUSTOM_MAX_INPUT_LEN 512

typedef struct  
{
    char man[MMIPIM_UA_CUSTOM_MAX_INPUT_LEN+1];
    char oem[MMIPIM_UA_CUSTOM_MAX_INPUT_LEN+1];
    char mod[MMIPIM_UA_CUSTOM_MAX_INPUT_LEN+1];
    char ua[MMIPIM_UA_CUSTOM_MAX_INPUT_LEN+1];
}MMIPIM_UA_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 初始化Pim的全局变量、回调函数等
//  Global resource dependence : 
//  Author: wangxiaolin
//  Note: 如果需要和网络MN层交互，需要在回调函数中处理消息
/*****************************************************************************/

PUBLIC void MMIPIM_InitApp 
(
	void
);

/*****************************************************************************/
//  Description : 号簿管家入口函数
//  Global resource dependence : 
//  Author: 王晓林
//  Note:
/*****************************************************************************/
PUBLIC void MMIPIM_Main(PIM_ENTRY_TYPE entry_type);

/*****************************************************************************/
//  FUNCTION:      MMIAPIPIM_InitModule
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIPIM_InitModule(void);


/*****************************************************************************/
// 	Description : Register PIM menu group
//	Global resource dependence : none
//  Author: 王晓林
//	Note:
/*****************************************************************************/
PUBLIC void MMIPIM_RegMenuGroup(void);


/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: 王晓林
//	Note:
/*****************************************************************************/
PUBLIC void MMIPIM_RegNv(void);

PUBLIC PIM_T_P MMIPIM_GetPIM(void);

PUBLIC int* MMIPIM_GetpISocket(void);


/*=====================================================================
  FUNCTION :  MMIPIM_ConnectNetwork
  DESC        : pdp 激活
  INPUT:        none
  RETURN:      
  AUTHER:      wangxiaolin
======================================================================*/
PUBLIC void MMIPIM_ConnectNetwork(int* presult);

/*=====================================================================
  FUNCTION : MMIPIM_IsPimInitialized
  DESC        :
  INPUT:        none
  RETURN:      
  AUTHER:      wangxiaolin
======================================================================*/
PUBLIC BOOLEAN MMIPIM_IsPimInitialized(void);

/*****************************************************************************/
//  FUNCTION:      MMIPIM_DisplayProc
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIPIM_DisplayProc(void);//char * display_chr);


/*****************************************************************************/
//  FUNCTION:      MMIPIM_GenDisplayChr
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIPIM_GenDisplayChr(uint16 * display_chr1,
                                      uint16 chr1_len,
                                      uint16 * display_chr2,
                                      uint16 chr2_len,
                                      uint16 * display_chr3,
                                      uint16 chr3_len,
                                      uint16 * display_chr4, // chenxiang_pim_ui 20080306
                                      uint16 chr4_len
                                      );

/*****************************************************************************/
//  FUNCTION:      MMIPIM_GetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIM_GetServerConfig(server_setting_info_type * p_server_setting);


/*****************************************************************************/
//  FUNCTION:      MMIPIM_SetServerConfig
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note:   
/*****************************************************************************/
PUBLIC void MMIPIM_SetServerConfig(server_setting_info_type * p_server_setting);

/*****************************************************************************/
//  FUNCTION:      MMIPIM_IsSyncProcing
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note: 判断pim是否正在同步
/*****************************************************************************/
BOOLEAN MMIPIM_IsSyncProcing(void);


PUBLIC PIM_GPRS_STATE_E  MMIPIM_GetPIMGprsState(void);

/*****************************************************************************/
//  FUNCTION:    pim_notify_dm_happen  
// 	Description :   
//	Global resource dependence : 
//  Author:   王晓林
//	Note: notify pim that dm will running
/*****************************************************************************/
PUBLIC void pim_notify_dm_happen(void);

 /*****************************************************************************/
//  Description : 初始化模块用到的全局变量
//  Global resource dependence : g_pim
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIM_InitGlobleRef(void);

PUBLIC  void  MMIPIM_InitPbStatus(void);

 PUBLIC void MMIPIM_ResetSetting(void);

 PUBLIC void MMIPIM_ResetFactorySetting(void);

 /*==========================================================
 * author		: wangxiaolin      
 * function 	: MMIPIM_PostMsgToSyncWin
 * decr		: 
 * parameter	: 
 * return		: 
 * effect		: NO
 * history	:created  7/29/2009 
 ==========================================================*/
PUBLIC BOOLEAN MMIPIM_PostMsgToSyncWin(
                    MMI_MESSAGE_ID_E    msg_id,         //the identify of message
                    DPARAM              param_ptr,      //the param of the message
                    uint32              size_of_param   //the size of the param
                    );

 PUBLIC uint32 PIM_GetCurTime(void);
 /*****************************************************************************/
//  Description : Get PIM custom ua info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIM_GetUAInfo(uint32 index,MMIPIM_UA_INFO_T * ua_info_ptr);

PUBLIC void MMIPIM_SaveLastAnchor(char * nv_lastanchor);

PUBLIC void  MMIPIM_DeactivePDP(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

  
