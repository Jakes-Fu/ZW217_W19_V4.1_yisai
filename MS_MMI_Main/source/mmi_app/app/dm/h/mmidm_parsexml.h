#ifndef DM_PARSEXML_H_
#define DM_PARSEXML_H_


#include "sci_types.h"

#ifdef __cplusplus
    extern   "C"
    {
#endif



#ifdef WIN32
#define DM_RECEIVE_PACKET1_PATH      "C:/DM/dm_receive1.txt"
#define DM_RECEIVE_PACKET2_PATH      "C:/DM/dm_receive2.txt"
#define DM_RECEIVE_PACKET3_PATH      "C:/DM/dm_receive3.txt"
#define DM_SEND_PACKET_PATH      "C:/DM/dm_response.log"
#endif



#define CRED_SUSS_DATA     "212"
#define CRED_FAIL1_DATA     "401"
#define CRED_FAIL2_DATA     "407"
#define STATUS_OK           "200"
#define STATUS_ERR          "204"
#define STATUS_CANCEL       "304"
#define DIALOG_DATA         "1101"
#define EXIT_DATA           "1223"
#define CONNECT_DATA        "1200"
#define MAXDT               "MAXDT="  
#define MINDT               "MINDT="     
#define ARR_METINFO         "xmlns=\'syncml:metinf\'"
#define COM_NONCE           "Po+n03S8ff3poIWmOcNFfg=="
#define FORMAT_CHAR         "chr"
#define DM_FAC_RESET        "./LAWMO/Operations/FactoryReset"
#define DM_LOCK             "./LAWMO/Operations/PartiallyLock"
#define DM_UNLOCK           "./LAWMO/Operations/UnLock"
#define DM_VERDTD_VER       "1.2"
#define DM_VERPROTO_VER     "DM/1.2"
#define DM_LOCNAME          "mvpdm"
#define DM_CRED_FORMAT      "b64"
#define DM_CRED_TYPE        "syncml:auth-md5"
#define DM_MAX_MSGID        "9000"
#define DM_MAXOBJ_SIZE      "524288"
#define DM_DEV_MOD          "./DevInfo/Mod"
#define DM_DEV_MAN          "./DevInfo/Man"
#define DM_DEV_LANG         "./DevInfo/Lang"
#define DM_DEV_VESION       "./DevInfo/DmV"
#define DM_DEV_DEVID        "./DevInfo/DevId"        
#define ARR_SYNCML_VER         "xmlns=\'SYNCML:SYNCML1.2\'"
#define RDM_MAX_SEND_SIZE      (8*1024)
#define MAX_RESURI_LEN  128
#define MAX_TAG_NAME_LEN  128
#define MAX_NONCE_LEN  64
#define B64_CRED_LEN  16
#define DM_COMMON_BUFSIZE    128
#define DM_LOCURL_LEN     32



#define MAX_XML_BUF_SIZE   (1024*9)
#define MAX_TAG_BUF_SIZE   1024
#define MAX_TAG_ARR_LEN    32

#define MAX_TARGET_BUF_SIZE  128
#define MAX_ITEM_BUF_SIZE  320
#define MAX_CMDID_BUF_SIZE  32
#define MAX_LOCURI_BUF_SIZE  128
#define MAX_SESSIONID_BUF_SIZE  16

#define MAX_DATA_BUF_SIZE  218

#define MAX_TAG_CONTENT_SIZE  64

#define DM_SESSIONID_1        27
#define DM_SESSIONID_2        28
#define DM_ALERT_ID        23
#define DM_MAX_TARGETREF_NUM  6

#define DM_TAGTYPE_LEN       32


typedef void (*DMDealTagDataFunc)(char* userdata);
/*****************************************************************************/
//  Description : THE STEP DM SESSION IS IN

/*****************************************************************************/
typedef enum
{
    STEP_GETNONCE = 0,
    STEP_GETNONCE_AGAIN,
    STEP_INIT,
    STEP_CREDED,
    STEP_ABORT,
} MMI_DM_STEP_E;

/*****************************************************************************/
//  Description : THE TAG SYNCML WILL BE USE
/*****************************************************************************/
typedef enum {
    TAG_VERDTD = 0,
    TAG_VERPROTO,
    TAG_SESSIONID,
    TAG_MSGID,
    TAG_TARGETID,
    TAG_SOURCEID,
    TAG_LOCURIID,
    TAG_LOCNAMEID,
    TAG_CREDID,
    TAG_METAID,
    TAG_MAXMSGID,
    TAG_MAXOBJID,
    TAG_DATAID,
    TAG_SYNCHDRID,
    TAG_SYNCBODYID,
    TAG_ALERTID,
    TAG_REPLACEID,
    TAG_STATUSID,
    TAG_RESULTID,
    TAG_CMDID,
    TAG_CMDIDID,
    TAG_MSGREFID,
    TAG_CMDREFID,
    TAG_TARGETREFID,
    TAG_SOURCEREFID,
    TAG_CHALID,
    TAG_ITEMID,
    TAG_FORMATID,
    TAG_TYPEID,
    TAG_NEXTNONCEID,
    TAG_RESURIID,
    TAG_GETID,
    TAG_SYNCMLID,
    TAG_EXECID,
    TAG_MAXID,
} MMI_DM_TAGID_E;



/*****************************************************************************/
//  Description : THE BASIC TAG TYPE STRUCT
/*****************************************************************************/
typedef struct
{
    char                     tagtype[DM_TAGTYPE_LEN];
    MMI_DM_TAGID_E                tagid;
} DMXML_TAG_INFO_T;



typedef struct DMXML_TAG
{   
    MMI_DM_TAGID_E                   tagId;
    char*               tagContent;
    char                tagArr[MAX_TAG_ARR_LEN+1];  
    BOOLEAN                      hasChildTag; 
    struct 	DMXML_TAG* next;
} DMXML_TAG_T;





/*****************************************************************************/
//  Description : THE META TAG STRUCT 
/*****************************************************************************/
typedef struct
{
    DMXML_TAG_T                   format;
    DMXML_TAG_T                   type;
    DMXML_TAG_T                   nextnonce;
} DMXML_TAG_META_T;
/*****************************************************************************/
//  Description : THE CHAL TAG STRUCT 
/*****************************************************************************/
typedef struct
{
    DMXML_TAG_META_T             meta;  
} DMXML_TAG_CHAL_T;
/*****************************************************************************/
//  Description : THE SOURCE TAG STRUCT 
/*****************************************************************************/
typedef struct
{
    DMXML_TAG_T                   locuri;
    DMXML_TAG_T                   locname;
} DMXML_TAG_SOURCE_T;
/*****************************************************************************/
//  Description : THE ITEM TAG STRUCT 
/*****************************************************************************/
typedef struct DMXML_TAG_ITEM
{
    DMXML_TAG_SOURCE_T            source;
    DMXML_TAG_META_T              meta;
    DMXML_TAG_T                   data;
    struct DMXML_TAG_ITEM*        next;
} DMXML_TAG_ITEM_T;
/*****************************************************************************/
//  Description : THE ALERT TAG STRUCT 
/*****************************************************************************/
typedef struct DMXML_TAG_ALERT
{
    DMXML_TAG_T                   CmdId;
    DMXML_TAG_T                   DATA;
    struct DMXML_TAG_ALERT*        next;
} DMXML_TAG_ALERT_T;

/*****************************************************************************/
//  Description : THE STATUS TAG STRUCT 
/*****************************************************************************/
typedef struct DMXML_TAG_STATUS
{
    DMXML_TAG_T                   CmdId;
    DMXML_TAG_T                   msgRef;
    DMXML_TAG_T                   cmdRef;
    DMXML_TAG_T                   cmd;
    DMXML_TAG_T                   targetRef[DM_MAX_TARGETREF_NUM];
    DMXML_TAG_T                   sourceRef;
    DMXML_TAG_CHAL_T              chal;
    DMXML_TAG_T                   data;
    struct DMXML_TAG_STATUS*        next;
} DMXML_TAG_STATUS_T;

/*****************************************************************************/
//  Description : THE RESULT TAG STRUCT 
/*****************************************************************************/
typedef struct DMXML_TAG_RESULT
{
    DMXML_TAG_T                   CmdId;
    DMXML_TAG_T                   msgRef;
    DMXML_TAG_T                   cmdRef;
    DMXML_TAG_T                   cmd;
    DMXML_TAG_T                   targetRef;
    DMXML_TAG_T                   sourceRef;
    DMXML_TAG_ITEM_T*              item_ptr;
    struct DMXML_TAG_RESULT*        next;
} DMXML_TAG_RESULT_T;


/*****************************************************************************/
//  Description : THE REPLACE TAG STRUCT 
/*****************************************************************************/

typedef struct DMXML_TAG_REPLACE
{
    DMXML_TAG_T                   CmdId;
    DMXML_TAG_ITEM_T*              item_ptr;
    struct DMXML_TAG_REPLACE*        next;
} DMXML_TAG_REPLACE_T;


typedef struct DMXML_TAG_STATUS_DATA
{
    char cmd[MAX_CMDID_BUF_SIZE + 1];
    char cmdref[MAX_CMDID_BUF_SIZE + 1];
    char targetref[MAX_TARGET_BUF_SIZE + 1];
    char sourceref[MAX_TARGET_BUF_SIZE + 1];
    char data[MAX_CMDID_BUF_SIZE + 1];
    char msgref[MAX_CMDID_BUF_SIZE + 1];
}DMXML_TAG_STATUS_DATA_T;



typedef struct DMXML_TAG_GET_DATA
{
    char cmdid[MAX_CMDID_BUF_SIZE + 1];
    char target_uri[DM_MAX_TARGETREF_NUM][MAX_TARGET_BUF_SIZE + 1];
    uint8  uri_index;
}DMXML_TAG_GET_DATA_T;


typedef struct DMXML_TAG_REPLACE_DATA
{
    char cmdid[MAX_CMDID_BUF_SIZE + 1];
    char target_uri[DM_MAX_TARGETREF_NUM][MAX_TARGET_BUF_SIZE + 1];
    char data[DM_MAX_TARGETREF_NUM][MAX_DATA_BUF_SIZE + 1];
    uint8  uri_index;
    uint8  data_index;
}DMXML_TAG_REPLACE_DATA_T;


typedef struct DMXML_TAG_ALERT_DATA_T
{
    char cmdid[MAX_CMDID_BUF_SIZE + 1];
    char minitime[DM_COMMON_BUFSIZE + 1];
    char maxtime[DM_COMMON_BUFSIZE + 1];
    char data[DM_COMMON_BUFSIZE+1];
    BOOLEAN is_dialog;

}DMXML_TAG_ALERT_DATA_T;



typedef struct DMXML_TAG_EXEC_DATA_T
{
    char cmdid[MAX_CMDID_BUF_SIZE + 1];
    char target_uri[MAX_TARGET_BUF_SIZE + 1];
}DMXML_TAG_EXEC_DATA_T;



typedef struct _DM_STATE_DATA_T
{
    uint32 s_g_SessionID;
    uint32 s_g_MsgID;
    uint32 s_g_CmdID;
    MMI_DM_STEP_E s_g_step;
    BOOLEAN   s_g_needreplay;
    BOOLEAN   s_g_callClearFunc;
    BOOLEAN   s_g_callLockFunc;
    BOOLEAN   s_g_callUnlockFunc;
    char   s_g_resUri[MAX_RESURI_LEN + 1] ;
    char   s_g_nonce[MAX_NONCE_LEN + 1];
}DM_STATE_DATA_T;


typedef struct _DM_STORE_DATA_T
{
    DMXML_TAG_STATUS_DATA_T* s_status_store_data_ptr;
    DMXML_TAG_GET_DATA_T * s_get_store_data_ptr;
    DMXML_TAG_REPLACE_DATA_T* s_replace_store_data_ptr;
    DMXML_TAG_ALERT_DATA_T* s_alert_store_data_ptr;
    DMXML_TAG_EXEC_DATA_T*  s_exec_store_data_ptr;
    char* s_cur_tagname[MAX_TAG_NAME_LEN+1];
    DMDealTagDataFunc  curDealTagFunc;
}DM_STORE_DATA_T;


typedef struct _DM_PARSE_DATA_T
{
    DMXML_TAG_ALERT_T* s_alerTag_head;
    DMXML_TAG_ALERT_T* s_alerTag_tail;
    DMXML_TAG_STATUS_T* s_statusTag_head;
    DMXML_TAG_STATUS_T* s_statusTag_tail;
    DMXML_TAG_STATUS_T* s_statusTag_cur;
    DMXML_TAG_RESULT_T* s_resultTag_head;
    DMXML_TAG_RESULT_T* s_resultTag_tail;
    DMXML_TAG_REPLACE_T* s_replaceTag_head;
    DMXML_TAG_REPLACE_T* s_replaceTag_tail;
}DM_PARSE_DATA_T;

/*****************************************************************************/
//  Description :通过id获得tag的name str
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIDM_GetTagStr(MMI_DM_TAGID_E id);
/*****************************************************************************/
//  Description :get a uri which to send 
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC char* MMIDM_getResUri(void);
/*****************************************************************************/
//  Description : 根据参数，组建一个tag
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ConstrucTag(DMXML_TAG_T* tag_ptr, 
                                MMI_DM_TAGID_E tag_id,
                                char* tagArr,
                                char* tagContent,
                                BOOLEAN hasChildTag);
/*****************************************************************************/
//  Description : when a session ended clear the data
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_clearDmXmlData(void);
/*****************************************************************************/
//  Description : 打印trace的函数
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_PrintTrace(char*buf, int size);
/*****************************************************************************/
//  Description : GET THE CONTENT OF THE TAG
//  Global resource dependence : 
//  Author:MARY.XIAO
//  xmlbuf: CONNTENT NEED TO PARSE  
//  tagid: THE FIRST TAG WHICH CONTENT WANT TO GET
//  content: OUT: THE TAG CONTENT
//  return:  return the left content 
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_generateXMLData(char* sendbuf, int buf_size);
/*****************************************************************************/
//  Description :设置session的id
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_setSessionId(uint32 id);

/*****************************************************************************/
//  Description :设置s_g_needreplay flag
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetIsNeedReplay(BOOLEAN is_need_replay);

/*****************************************************************************/
//  Description :设置session的step
//  Global resource dependence : 
//  Author:MARY.XIAO
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_setSessionStep(MMI_DM_STEP_E step);
/*****************************************************************************/
//  Description : 结束dm
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_EndDm(void);
/*****************************************************************************/
//  Description : 产生xml包并发送
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SendDmData(void);
/*****************************************************************************/
//  Description : for the alert confirm window to callback which result the user choose
//  Global resource dependence : 
//  iscontinue: true: yes
//              false: no
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_NotifyAlertResult(BOOL iscontinue);
/*****************************************************************************/
//  Description : 开启一个dm的http
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_BeginDmTansport(void);
/*****************************************************************************/
//  Description : 解析收到的xml包，放入相应链表
//  Global resource dependence : 
//  Author:mary.xiao
//  Modify:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_ParseReceiveData(char* data);

/******************************************************************* 
** 函数名:mmidm_calc_b64_cred
** 功能描述：计算b64 authentication所用的cred值
** 输  出: 
** 作  者:mary.xiao

*******************************************************************/
PUBLIC void mmidm_calc_b64_cred(char* creddata, unsigned long cbLength);
/******************************************************************* 
** 函数名:mmidm_calc_md5_cred
** 功能描述：计算md5 authentication所用的cred值
** 输  出: 
** 作  者:MARYXIAO

*******************************************************************/
PUBLIC char*  mmidm_calc_md5_cred(char* creddata);
#ifdef   __cplusplus
    }
#endif


#endif
