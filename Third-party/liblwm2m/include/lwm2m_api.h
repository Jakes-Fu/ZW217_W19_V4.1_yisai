#ifndef _LWM2M_API_H
#define _LWM2M_API_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include "liblwm2m_config.h"



#define LWM2MTASK_PRIORITY (231)
#define LWM2MTASK_STACK_SIZE (1024 * 16)
#define LWM2MTASK_EVENT_QUEUE_SIZE (32)


#define MAX_REF_NUM 0x02

#define LWM2M_EVENT_BASE 0
#define LWM2M_EVENT_BOOTSTRAP_START LWM2M_EVENT_BASE + 1
#define LWM2M_EVENT_BOOTSTRAP_SUCCESS LWM2M_EVENT_BASE + 2
#define LWM2M_EVENT_BOOTSTRAP_FAILED LWM2M_EVENT_BASE + 3
#define LWM2M_EVENT_CONNECT_SUCCESS LWM2M_EVENT_BASE + 4
#define LWM2M_EVENT_CONNECT_FAILED LWM2M_EVENT_BASE + 5

#define LWM2M_EVENT_REG_SUCCESS LWM2M_EVENT_BASE + 6
#define LWM2M_EVENT_REG_FAILED LWM2M_EVENT_BASE + 7
#define LWM2M_EVENT_REG_TIMEOUT LWM2M_EVENT_BASE + 8

#define LWM2M_EVENT_LIFETIME_TIMEOUT LWM2M_EVENT_BASE + 9
#define LWM2M_EVENT_STATUS_HALT LWM2M_EVENT_BASE + 10
#define LWM2M_EVENT_UPDATE_SUCCESS LWM2M_EVENT_BASE + 11
#define LWM2M_EVENT_UPDATE_FAILED LWM2M_EVENT_BASE + 12
#define LWM2M_EVENT_UPDATE_TIMEOUT LWM2M_EVENT_BASE + 13

#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
#define LWM2M_EVENT_TIMEOUT_AND_RETRYING LWM2M_EVENT_BASE + 18
#define LWM2M_EVENT_NOTIFY_SENT_RESET LWM2M_EVENT_BASE + 19
#endif
#define LWM2M_EVENT_RESPONSE_FAILED LWM2M_EVENT_BASE + 20
#define LWM2M_EVENT_NOTIFY_FAILED LWM2M_EVENT_BASE + 21
#define LWM2M_EVENT_NOTIFY_SEND_SUCCESS LWM2M_EVENT_BASE + 22
#define LWM2M_EVENT_NOTIFY_SENT_SUCCESS LWM2M_EVENT_BASE + 23
#define LWM2M_EVENT_NOTIFY_SENT_TIMEOUT LWM2M_EVENT_BASE + 24

#ifdef CONFIG_LWM2M_CTWING_SUPPORT
#define LWM2M_EVENT_UPDATE_BINDING_SEND LWM2M_EVENT_BASE + 25
#define LWM2M_EVENT_UPDATE_BINDING_SUCCESS LWM2M_EVENT_BASE + 26
#define LWM2M_EVENT_UPDATE_BINDING_FAILED LWM2M_EVENT_BASE + 27
#define LWM2M_EVENT_UPDATE_BINDING_TIMEOUT LWM2M_EVENT_BASE + 28

#define LWM2M_EVENT_DEREG_FAILED LWM2M_EVENT_BASE + 30
#define LWM2M_EVENT_DEREG_TIMEOUT LWM2M_EVENT_BASE + 31
#endif
#define LWM2M_EVENT_DEREG_SUCCESS LWM2M_EVENT_BASE + 29

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
#define LWM2M_EVENT_UPDATE_NEED LWM2M_EVENT_BASE + 32
#ifdef CONFIG_LWM2M_ONENET_UPDATE
#define LWM2M_EVENT_FIRMWARE_DOWNLOADING LWM2M_EVENT_BASE + 40         //enter to downloading
#define LWM2M_EVENT_FIRMWARE_DOWNLOAD_FAILED LWM2M_EVENT_BASE + 41     //enter downloading error
#define LWM2M_EVENT_FIRMWARE_DOWNLOADED LWM2M_EVENT_BASE + 42          //download finish
#define LWM2M_EVENT_FIRMWARE_UPDATING LWM2M_EVENT_BASE + 43            //updating
#define LWM2M_EVENT_FIRMWARE_UPDATE_SUCCESS LWM2M_EVENT_BASE + 44      //update success
#define LWM2M_EVENT_FIRMWARE_UPDATE_FAILED LWM2M_EVENT_BASE + 45       //update fail
#define LWM2M_EVENT_FIRMWARE_UPDATE_OVER LWM2M_EVENT_BASE + 46         //update finish
#define LWM2M_EVENT_FIRMWARE_DOWNLOAD_DISCONNECT LWM2M_EVENT_BASE + 47 //update error
#define LWM2M_EVENT_FIRMWARE_ERASE_SUCCESS LWM2M_EVENT_BASE + 48
#define LWM2M_EVENT_FIRMWARE_ERASE_FAIL LWM2M_EVENT_BASE + 49
#define LWM2M_EVENT_FIRMWARE_TRIGGER LWM2M_EVENT_BASE + 50

#define LWM2M_EVENT_SOTA_DOWNLOADING LWM2M_EVENT_BASE + 51
#define LWM2M_EVENT_SOTA_DOWNLOAED LWM2M_EVENT_BASE + 52
#define LWM2M_EVENT_SOTA_FLASHERASE LWM2M_EVENT_BASE + 53
#define LWM2M_EVENT_SOTA_UPDATING LWM2M_EVENT_BASE + 54
#endif
#endif

#define LWM2M_CASE(nEvId) #nEvId

#define LWM2M_SERVER_OC 0
#define LWM2M_SERVER_AEP 1
#define LWM2M_SERVER_ONENET 2
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
#define LWM2M_SERVER_OC_CUS 3
#endif

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
#define ONENET_COAP_IGNORE (uint8_t)0x01

#define ONENET_COAP_204_CHANGED (uint8_t)0x44
#define ONENET_COAP_205_CONTENT (uint8_t)0x45
#define ONENET_COAP_206_CONFORM (uint8_t)0x46
#define ONENET_COAP_231_CONTINUE (uint8_t)0x5F
#define ONENET_COAP_400_BAD_REQUEST (uint8_t)0x80
#define ONENET_COAP_401_UNAUTHORIZED (uint8_t)0x81
#define ONENET_COAP_404_NOT_FOUND (uint8_t)0x84
#define ONENET_COAP_405_METHOD_NOT_ALLOWED (uint8_t)0x85
#define ONENET_COAP_406_NOT_ACCEPTABLE (uint8_t)0x86
#define ONENET_COAP_503_SERVICE_UNAVAILABLE (uint8_t)0xA3

#define ONENET_RESPONSE_READ ONENET_COAP_205_CONTENT
#define ONENET_RESPONSE_WRITE ONENET_COAP_204_CHANGED
#define ONENET_RESPONSE_EXECUTE ONENET_COAP_204_CHANGED
#define ONENET_RESPONSE_OBSERVE ONENET_COAP_205_CONTENT
#define ONENET_RESPONSE_OBSERVE_PARAMS ONENET_COAP_204_CHANGED
#define ONENET_RESPONSE_DISCOVER ONENET_COAP_205_CONTENT
#define ONENET_RESPONSE_CONTINUE ONENET_COAP_231_CONTINUE
#define ONENET_RESPONSE_BAD_REQUEST ONENET_COAP_400_BAD_REQUEST
#define ONENET_RESPONSE_UNAUTHORIZED ONENET_COAP_401_UNAUTHORIZED
#define ONENET_RESPONSE_NOT_FOUND ONENET_COAP_404_NOT_FOUND
#define ONENET_RESPONSE_METHOD_NOT_ALLOWED ONENET_COAP_405_METHOD_NOT_ALLOWED
#define ONENET_RESPONSE_NOT_ACCEPTABLE ONENET_COAP_406_NOT_ACCEPTABLE
#define ONENET_RESPONSE_SERVICE_UNAVAILABLE ONENET_COAP_503_SERVICE_UNAVAILABLE
#define ONENET_NOTIFY_CONTENT ONENET_COAP_205_CONTENT
#define ONENET_NOTIFY_CONTINUE ONENET_COAP_231_CONTINUE
#endif



//modify by xiameng.yang
/**
 * function type of callback
 */
typedef void (*osiCallback_t)(uint32 argc, void* argv);

typedef enum
{
    lwm2m_string = 1,
    lwm2m_opaque = 2,
    lwm2m_integer = 3,
    lwm2m_float = 4,
    lwm2m_bool = 5,
} lwm2m_tpye_t;

typedef struct
{
    uint16_t objid;
    uint16_t insid;
    uint16_t resid;
    uint16_t count;
} read_param_t;

typedef struct _write_param_t
{
    uint16_t objid;
    uint16_t insid;
    uint16_t resid;
    lwm2m_tpye_t type;
    uint16_t len;
    uint8_t *data;
    struct _write_param_t *next;
} write_param_t;

typedef struct
{
    uint16_t objid;
    uint16_t insid;
    uint16_t resid;
    int length;
    uint8_t *buffer;
} exec_param_t;

typedef struct
{
    uint16_t objid;
    uint16_t insid;
    uint16_t resid;
    uint8_t *param;
} setParam_t;

typedef enum
{
    EVETN_IND,
    OBJ_READ_IND,
    OBJ_WRITE_IND,
    OBJ_EXE_IND,
    OBJ_OBSERVE_IND,
    OBJ_DISCOVER_IND,
    OBJ_SETPARAM_IND,
    OBJ_OPERATE_RSP_IND,
    SERVER_REGCMD_RESULT_IND,
    SERVER_QUIT_IND,
    FOTA_DOWNLOADING_IND,
    FOTA_DOWNLOAD_FAILED_IND,
    FOTA_DOWNLOAD_SUCCESS_IND,
    FOTA_PACKAGE_CHECK_IND,
    FOTA_UPGRADING_IND,
    FOTA_UPGRADE_OK_IND,
    DATA_SEND_IND,
#ifdef CONFIG_LWM2M_NOTIFY_SEQ_SUPPORT
    OBJ_OBSERVE_NOTIFY_IND,
#endif
#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
    FOTA_DOWNLOAD_REQ_IND,
    FOTA_UPGRADE_REQ_IND,
    DTLS_HANDSHAKE_SUCCESS_IND,
    DTLS_HANDSHAKE_FAILED_IND,
#endif
#ifdef CONFIG_LWM2M_ONENET_SOTA
    SOTA_RSP_IND,
#endif
    MESSAGE_TASK_EXIT_IND
} LWM2M_EVENT_IND_T;

typedef enum
{
    LWM2M_RET_ERROR = -1,
    LWM2M_RET_OK,
    LWM2M_RET_PENDING,
} lwm2m_ret_t;

#define MAX_INIT_OBJECTS 10

#if defined(CONFIG_LWM2M_REDUCE_MEMORY)
#define MAX_DYNAMIC_OBJECTS 5
#define MAX_RESOURCE 15
#else
#define MAX_DYNAMIC_OBJECTS 20
#define MAX_RESOURCE 50
#endif

typedef enum
{
    TYPE_RW,
    TYPE_R,
    TYPE_W,
    TYPE_EXE,
} operate_type_t;

typedef struct
{
    uint16_t resId;
    operate_type_t operate_type;
    lwm2m_tpye_t value_type;
} ipso_res_t;

typedef struct
{
    uint16_t objId;
    bool isMultInst;
    uint8_t resCount;
    bool isResReady;
    bool isDiscover;
    ipso_res_t resArray[MAX_RESOURCE];
} ipso_obj_t;

typedef struct
{
    uint16_t objId;
    uint16_t resCount;
    uint16_t actCount;
    uint8_t *bitmap;
    uint32_t instCount;
} obj_arry_t;

typedef struct
{
    uint8_t version;
    uint16_t mtu;
    uint8_t debugmode;
    uint8_t extendinfomode;
    uint8_t outputmode;
    uint8_t debug_level;
    uint8_t cmdline[350];
    uint8_t **argv;
    uint8_t argc;
    uint8_t isregisted;
    int ipc_socket;
    int ipc_data_socket;
    uint32_t lifetime;
    uint32_t timeout;
    uint32_t this;
    uint32_t event_handle;
    osiCallback_t callback;
    void *userparam;
    obj_arry_t objects[MAX_INIT_OBJECTS];
    ipso_obj_t obj_type_table[MAX_DYNAMIC_OBJECTS];
    bool pending_regcmd;
    bool use_dynamic_ipso;
    uint8_t nDLCI;
    bool observer;
    int isquit;
#ifdef CONFIG_LWM2M_CTWING_SUPPORT
    uint8_t bindingmode;
#endif
#ifdef CONFIG_LWM2M_ONENET_SUPPORT
    uint8_t ResMode; // 0 strictly match with rescount, 1 not larger than rescount
#endif

    bool clearstate;
} lwm2m_config_t;

typedef struct
{
    uint8_t ver;
    uint8_t cc;
    uint16_t size; // Config Size of Byte
    uint8_t cfgid;
    uint8_t cfgidsize;
    uint32_t lifetime;
    uint16_t mtu;
    uint8_t *apn;
    uint16_t apnlen;
    uint8_t *usname;
    uint16_t usnamelen;
    uint8_t *psw;
    uint16_t pswlen;
    uint8_t linktype;
    uint8_t bandtype;
    uint8_t *host;
    uint16_t hostlen;
    uint8_t *net_exdata;
    uint16_t net_exdatalen;
    uint8_t debugmode;
    uint8_t extendinfomode;
    uint8_t outputmode;
    uint8_t debug_level;
    uint16_t buffersize;
    uint8_t *sys_exdata;
    uint16_t sys_exdata_len;
    uint16_t objects_count;
} mipl_config_t;

#ifdef CONFIG_LWM2M_ONENET_UPDATE
typedef enum
{
    cissys_event_unknow = 0,
    cissys_event_fw_erase_success, //for erase, handle /5/0/26501
    cissys_event_fw_erase_fail,
    cissys_event_write_success, //for write
    cissys_event_write_fail,
    cissys_event_fw_validate_success, //for validate
    cissys_event_fw_validate_fail,
    cissys_event_fw_update_success, //for update
    cissys_event_fw_update_fail,
} cissys_event_t;
#endif

#define MIPL_HEADER_LENGTH 3
#define MIPL_CONFIG_ITEM_LENGTH 3
#define MIPL_CONFIG_INIT_INFO_LENGTH 4
#define MIPL_CONFIG_NO_EXT_DATA 0x00
#define MIPL_CONFIG_HAVE_EXT_DATA 0x01
#define MIPL_HEADER_VERSION_POSITION 5
#define MIPL_HEADER_CONFIG_COUNT_MASK 0x1F
#define MIPL_CONFIG_ID_POSITION 1
#define MIPL_CONFIG_SIZE_EXTEND_FLAG_MASK 0x01
#define MIPL_DEBUG_MODE_POSITION 7
#define MIPL_EXTEND_INFO_MODE_POSITION 6
#define MIPL_EXTEND_INFO_MODE_MASK 0X01
#define MIPL_OUTPUT_MODE_POSITION 4
#define MIPL_OUTPUT_MODE_MASK 0X03
#define MIPL_DEBUG_LEVEL_MASK 0X0F
#define MIPL_LIFETIME_MIN_VALUE 10

typedef enum enum_lwm2m_cfgid
{
    MIPL_CFGID_INIT = 0x01,
    MIPL_CFGID_NET = 0x02,
    MIPL_CFGID_SYS = 0x03,
    MIPL_CFGID_OBJECTS = 0x04,
} lwm2m_cfgid_t;

typedef struct lwm2mEventSIG
{
    _SIGNAL_VARS
    uint32 id;     ///< event identifier
    uint32 param1; ///< 1st parameter
    uint32 param2; ///< 2nd parameter
    uint32 param3; ///< 3rd parameter
} lwm2mEventSIG_t;


typedef struct lwm2mEvent
{    
    uint32 id;     ///< event identifier
    uint32 param1; ///< 1st parameter
    uint32 param2; ///< 2nd parameter
    uint32 param3; ///< 3rd parameter
} lwm2mEvent_t;

typedef enum _Mipl_Reg_State_t
{
    MIPL_NOT_REGISTER = 0,
    MIPL_IS_REGISTERING,
    MIPL_IS_DEREGISTERING,
    MIPL_HAD_REGISTERED
} Mipl_Reg_State_t;

typedef struct _Mipl_Cache_t
{
    char *SavedValue;
    uint32_t SavedLength;
    uint8_t LastIndex;
} Mipl_Cache_t;

typedef struct _Mipl_Ctx_t
{
    Mipl_Reg_State_t isRegister;
    Mipl_Cache_t ReadRspCache;
    Mipl_Cache_t NotifyCache;
} Mipl_Ctx_t;

extern lwm2m_config_t *lwm2m_configs[];

uint8_t *lwm2m_eventName(uint8_t nEvId);

int lwm2m_is_pending_regcmd(uint8_t ref);

void lwm2mPostEvent(uint8_t ref, uint32_t nEventId, uint32_t nResult, uint32_t nParam1, uint32_t nParam2);

int8_t lwm2m_new_config_xml(void *file_config, uint16_t size);

int8_t lwm2m_new_config(const uint8_t *cmdline);

lwm2m_ret_t lwm2m_free_config(uint8_t ref);

lwm2m_ret_t lwm2mcfg_set_handler(uint8_t ref, void *taskHandle, osiCallback_t callback, void *userparam);

lwm2m_ret_t lwm2mcfg_set_handler_ext(uint8_t ref, void *taskHandle, osiCallback_t callback, void *userparam, uint8_t nDLCI);

lwm2m_config_t *lwm2m_get_config(uint8_t configIndex);

uint8_t lwm2m_get_ipcSocket(uint8_t configIndex);

uint8_t lwm2m_get_ipcDataSocket(uint8_t configIndex);

void lwm2m_parse_buffer(char *buffer, void *value, uint8_t count, void *delim);

lwm2m_ret_t lwm2m_excute_cmd(uint8_t *data, uint32_t data_len, uint8_t ref);

lwm2m_ret_t lwm2m_excute_data_cmd(uint8_t *data, uint32_t data_len, uint8_t ref);

lwm2m_ret_t lwm2m_is_registed(uint8_t ref);

obj_arry_t *lwm2m_get_init_objs(uint8_t ref);

lwm2m_ret_t lwm2m_add_obj(uint16_t objId, uint32_t instCount, uint8_t *bitmap, uint16_t resCount, uint8_t ref);

lwm2m_ret_t lwm2m_del_obj(uint16_t objId, uint8_t ref);

obj_arry_t *lwm2m_find_obj(uint16_t objId, uint8_t ref);

lwm2m_ret_t lwm2m_read_rsp(uint16_t objId, uint16_t instId, uint16_t resId, lwm2m_tpye_t type,
                           const uint8_t *value, uint32_t value_length, bool isEnd, uint8_t result, uint8_t ref);

lwm2m_ret_t lwm2m_write_rsp(bool result, uint8_t ref);

lwm2m_ret_t lwm2m_exec_rsp(bool result, uint8_t ref);

lwm2m_ret_t lwm2m_discover_rsp(uint8_t result, const uint8_t *value, uint8_t ref);

#ifdef CONFIG_LWM2M_ONENET_SUPPORT
lwm2m_ret_t lwm2m_observe_rsp_onenet(uint16_t mid, uint8_t result, bool isAutoAck, uint8_t ref);
lwm2m_ret_t lwm2m_discover_rsp_onenet(uint16_t mid, uint8_t result, bool isAutoAck, const uint8_t *value, uint8_t ref);
lwm2m_ret_t lwm2m_notify_onenet(uint16_t mid, uint16_t objId, uint16_t instId, uint16_t resId, lwm2m_tpye_t type, const uint8_t *value, uint32_t value_length, uint8_t result, uint8_t coapType, uint8_t ref);
lwm2m_ret_t lwm2m_read_rsp_onenet(uint16_t mid, uint16_t objId, uint16_t instId, uint16_t resId, lwm2m_tpye_t type, const uint8_t *value, uint32_t value_length, uint8_t result, uint8_t ref);
lwm2m_ret_t lwm2m_write_rsp_onenet(uint16_t mid, uint8_t result, uint8_t ref);
lwm2m_ret_t lwm2m_execute_rsp_onenet(uint16_t mid, uint8_t result, uint8_t ref);
lwm2m_ret_t lwm2m_parameter_rsp_onenet(uint16_t mid, uint8_t result, uint8_t ref);
lwm2m_ret_t lwm2m_clearFwState_onenet(bool fwFlag, uint8_t ref);
lwm2m_ret_t lwm2m_SetResMode_onenet(uint8_t ResMode, uint8_t ref);
#ifdef CONFIG_LWM2M_ONENET_SOTA
lwm2m_ret_t lwm2m_set_sota_info_onenet(char *version, uint32_t upgrade_space, uint8_t ref);
lwm2m_ret_t lwm2m_notify_sota_result_onenet(cissys_event_t eventid, uint8_t ref);
#endif
#endif

lwm2m_ret_t lwm2m_notify(uint16_t objId, uint16_t instId, uint16_t resId, const uint8_t *value,
                         uint32_t value_length, lwm2m_tpye_t type, uint8_t ref);

lwm2m_ret_t lwm2m_notify_with_typeex(uint16_t objId, uint16_t instId, uint16_t resId, const uint8_t *value,
                                     uint32_t value_length, lwm2m_tpye_t type, uint8_t ref, uint8_t notifyType, uint8_t seqNo, uint8_t raiMode);

lwm2m_ret_t lwm2m_notify_with_type(uint16_t objId, uint16_t instId, uint16_t resId, const uint8_t *value,
                                   uint32_t value_length, lwm2m_tpye_t type, uint8_t ref, uint8_t notifyType);

lwm2m_ret_t lwm2m_update(uint32_t lifetime, bool withObject, uint8_t ref);

#ifdef CONFIG_LWM2M_CTWING_SUPPORT
lwm2m_ret_t lwm2m_update_binding(uint32_t binding, uint8_t ref);
#endif

lwm2m_ret_t lwm2m_set_parameter(uint16_t objId, uint16_t instId, uint16_t resId,
                                uint8_t *parameter, uint8_t ref);

lwm2m_ret_t lwm2m_register(int ref, uint32_t lifetime, uint32_t timeout);

lwm2m_ret_t lwm2m_unregister(int ref);

bool lwm2m_is_dynamic_ipso(int ref);

lwm2m_ret_t lwm2m_set_dynamic_ipso(int ref);

ipso_obj_t *lwm2m_get_ipso_obj(uint16_t objId, int16_t resId, ipso_res_t **ipso_res, uint8_t ref);

uint8_t lwm2m_isObjSupport(uint16_t objId, uint8_t ref);

bool lwm2m_isResReady(uint16_t objId, uint8_t ref);

void lwm2m_registNewObj(uint16_t objId, uint16_t resCount, uint8_t ref);

int lwm2m_registNewRes(uint16_t objId, uint8_t *resString, uint8_t ref);

lwm2m_ret_t lwm2m_set_send_flag(int ref, int flag);

lwm2m_ret_t lwm2m_get_observer(uint8_t ref);

lwm2m_ret_t lwm2m_set_observer(uint8_t ref, bool observer);

lwm2m_ret_t lwm2m_start_fota_download(const uint8_t *uri, uint8_t ref);

lwm2m_ret_t lwm2m_notify_fota_state(uint32_t state, uint32_t resulte, uint8_t ref);

#ifdef CONFIG_LWM2M_OCEANCONNECT_CUS_SUPPORT
uint8_t lwm2m_get_handshake_state();

bool lwm2m_reset_handshake_state();

bool lwm2m_send_critical_data_ok();

bool lwm2m_start_controlled_fota_download(uint8_t ref);

bool lwm2m_cancel_controlled_fota_download(uint8_t ref);

bool lwm2m_start_controlled_fota_update(uint8_t ref);

bool lwm2m_cancel_controlled_fota_update(uint8_t ref);
#endif

#endif
