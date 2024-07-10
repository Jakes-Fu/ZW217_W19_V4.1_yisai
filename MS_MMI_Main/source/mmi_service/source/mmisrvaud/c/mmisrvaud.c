/****************************************************************************
** File Name:      mmisrvaud.c                                                              *
** Author:          Yintang.ren                                                               *
** Date:             28/04/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to describe the data struct of                 
**                     AUDIO service module.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 28/04/2011       Yintang.ren         Create
** 
****************************************************************************/
#include "mmi_service_trc.h"
#include "mmisrvaud.h"
#include "mmisrvaud_drv.h"
#include "mmisrvaud_util.h"
#include "os_import.h" /* only for dump audio info */

#ifdef BT_AV_SNK_SUPPORT
#include "mmibt_export.h"
#endif

/* Used  internal only */
#define VIDEO_WP_SUPPORT
#define TONE_CONFLICT_SUPPORT

#define TRACK_BUF_SIZE_PLAYER         (19400) //(80 * 150)
#define TRACK_BUF_SIZE_APP              (0 * 150)
#define DECODE_OUTPUT_BUF_SIZE      (TRACK_BUF_SIZE_APP + TRACK_BUF_SIZE_PLAYER)

#define IS_SRV_ACTIVE(x) (MMISRVMGR_SERVICE_STATE_ACTIVE == (x)->service.srv_state)
#define IS_SRV_SUSPEND(x) (MMISRVMGR_SERVICE_STATE_SUSPEND == (x)->service.srv_state || MMISRVMGR_SERVICE_STATE_BEEN_SUSPENDED == (x)->service.srv_state)
#define IS_SRV_STANDING(x) (MMISRVMGR_SERVICE_STATE_STANDING == (x)->service.srv_state)

#define IS_AUD_PLAY(x) (AUDIO_STATE_PLAY == (x)->audio.audio_state)
#define IS_AUD_STOP(x) (AUDIO_STATE_STOP == (x)->audio.audio_state)
#define IS_AUD_PAUSE(x) (AUDIO_STATE_PAUSE == (x)->audio.audio_state)
#define IS_AUD_PENDING(x) (AUDIO_STATE_PENDING == (x)->audio.audio_state)
/* cur route is BT route */
#define IS_CUR_BT_ROUTE(x)          (MMISRVAUD_ROUTE_BLUETOOTH == (x)->audio.cur_route)
#define IS_SHOULD_BT_ROUTE(x)    (MMISRVAUD_ROUTE_BLUETOOTH == GetShouldBeRoute(x))

#define FL(x) (1 << x)

#define SET_ASYNC_MODE(x)   \
{   \
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: SET_ASYNC_MODE, old=%d,new=%d", s_is_async_mode, (x)));    \
    (s_is_async_mode = (x)); \
}

#define IS_ASYNC_MODE         (s_is_async_mode)

#define BT_CLOSE_DELAY_TIME     (1 * 1000) //fix bug1940037

#define BT_SET_CUR(_entity_addr, event, rate)   \
{   \
    s_bt_info.cur_event.entity_addr = _entity_addr;    \
    s_bt_info.cur_event.event_data = event;  \
    s_bt_info.cur_event.type = ((SRVAUD_ENTITY_INFO_T *)_entity_addr)->audio.audio_info.info.type;  \
    s_bt_info.cur_event.sample_rate = (rate);  \
}

#define BT_SET_WAIT(_entity_addr, event, rate)   \
{   \
    s_bt_info.wait_event.entity_addr = _entity_addr;    \
    s_bt_info.wait_event.event_data = event;  \
    s_bt_info.wait_event.type = ((SRVAUD_ENTITY_INFO_T *)_entity_addr)->audio.audio_info.info.type;  \
    s_bt_info.wait_event.sample_rate = (rate);  \
}

#define BT_SET_STATE(x)     \
{   \
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: BT_SET_STATE, s_bt_info.state=%d,new_state=%d", s_bt_info.state, (x)));  \
    s_bt_info.state = x;    \
}

#define BT_GET_STATE (s_bt_info.state)

#define BT_CLEAR_CUR    \
{   \
    SCI_MEMSET(&s_bt_info.cur_event, 0, sizeof(SRV_BT_EVENT_T));  \
}

#define BT_CLEAR_WAIT    \
{   \
    SCI_MEMSET(&s_bt_info.wait_event, 0, sizeof(SRV_BT_EVENT_T));  \
}

#define BT_TRANSF_NEXT  \
{   \
    s_bt_info.cur_event = s_bt_info.wait_event; \
    SCI_MEMSET(&s_bt_info.wait_event, 0, sizeof(SRV_BT_EVENT_T));  \
}

#define BT_CLEAR_TIMER  \
{   \
    if(s_bt_info.timer != 0)    \
    {   \
        MMISRVAUD_StopTimer(s_bt_info.timer);   \
        s_bt_info.timer = 0;    \
    }   \
}

#define BT_START_TIMER  \
{   \
    if(s_bt_info.timer != 0)    \
    {   \
        MMISRVAUD_StopTimer(s_bt_info.timer);   \
        s_bt_info.timer = 0;    \
    }   \
    s_bt_info.timer = MMISRVAUD_CreateTimer(BT_CLOSE_DELAY_TIME, (MMISRVAUD_TIMER_CB_FUNC)HandleBTCloseDelayTimer, 0, FALSE);    \
}

#define LOCK_SERVICE MMISRVAUD_GetMutext(s_aud_service_mutex)
#define UNLOCK_SERVICE MMISRVAUD_PutMutext(s_aud_service_mutex)

#ifdef TYPE_DEF
#undef TYPE_DEF
#endif

#define AUDIO_STATE_DEF \
    TYPE_DEF(AUDIO_STATE_STOP),   \
    TYPE_DEF(AUDIO_STATE_PLAY),   \
    TYPE_DEF(AUDIO_STATE_PAUSE),  \
    TYPE_DEF(AUDIO_STATE_PENDING),    \
    TYPE_DEF(AUDIO_STATE_MAX)
    
#define TYPE_DEF(x) x
typedef enum
{
    AUDIO_STATE_DEF
}SRVAUD_AUDIO_STATE_E;
#undef TYPE_DEF

typedef enum
{
    SRV_WORK_MODE_NONE,
    SRV_WORK_MODE_SINGLE,
    SRV_WORK_MODE_MIXING,
    SRV_WORK_MODE_COEXIST
}SRV_WORK_MODE_E;

typedef enum
{
    SRV_CREATE_MODE_NONE,
    SRV_CREATE_MODE_SINGLE,
    SRV_CREATE_MODE_SWITCH,
    SRV_CREATE_MODE_MIXING,
    SRV_CREATE_MODE_STANDING,
    SRV_CREATE_MODE_COEXIST
}SRV_CREATE_MODE_E;

#define BT_STATE_DEF    \
    TYPE_DEF(BT_STATE_NONE),  \
    TYPE_DEF(BT_STATE_OPEN_WAIT), \
    TYPE_DEF(BT_STATE_OPENED),    \
    TYPE_DEF(BT_STATE_CLOSE_WAIT),    \
    TYPE_DEF(BT_STATE_DELAY_TIMER_WAIT), \
    TYPE_DEF(BT_STATE_MAX)

#define TYPE_DEF(x) x
typedef enum
{
    BT_STATE_DEF
}SRV_BT_STATE_E;
#undef TYPE_DEF

typedef struct
{
    SRVAUD_AUDIO_STATE_E audio_state;
    //MMISRVAUD_NOTIFIER_FUNC_T notify;
    MMISRVAUD_TYPE_T audio_info;
    MMISRVAUD_ROUTE_T manual_route;
    MMISRVAUD_ROUTE_T cur_route;
    uint8 timer;
    uint8 interval_timer;
}SRVAUD_ENTITY_AUDIO_INFO_T;

typedef struct
{
    MMISRVMGR_SERVICE_STATE_E srv_state;
    uint32 suspend_service;
    uint32 suspend_by_service;
    uint32 ind_data;
    MMISRVMGR_NOTIFY_FUNC notify;

    uint16 pri;
    BOOLEAN is_async;
    BOOLEAN is_auto_resume_off;
    BOOLEAN is_auto_free;
}SRVAUD_ENTITY_SERVICE_INFO_T;

typedef struct IMMISRV_AUD_T  IMMISRV_AUD_T;

MMISRV_INTERFACE(IMMISRV_AUD_T)
{
    IMMISRV_INHERIT_BASE
};

typedef struct
{
    MMISRV_VTBL_T(IMMISRV_AUD_T)  *vtbl_ptr;    // 虚拟函数表
    MMISRV_HANDLE_T handle;
    uint32 drv_handle;
    SRVAUD_ENTITY_SERVICE_INFO_T service;
    SRVAUD_ENTITY_AUDIO_INFO_T audio;
    char client_name[MMISRVAUD_STR_CLIENT_NAME_LEN + 1];
}SRVAUD_ENTITY_INFO_T;

typedef struct
{
    uint16 max;
    //uint16 total;
    //uint16 cur;
    BOOLEAN is_enable;
    BOOLEAN is_mute;
    //BOOLEAN is_earphone_on;
    //BOOLEAN is_bt_on;
    //BOOLEAN is_wifi_on;
    //MMISRVAUD_ENTITY_INFO_T entity[10];
    SRVAUD_ENTITY_INFO_T *active_entity;
}SRVAUD_INTERNAL_STATUES_T;

typedef struct
{
    uint32 handle[MMISRVAUD_SERVICE_NUM_MAX];
    uint32 entity_addr[MMISRVAUD_SERVICE_NUM_MAX];
    uint32 num;
}SRVAUD_HANDLE_BOX_T;

typedef struct
{
    uint32 entity_addr;
    MMISRVAUD_EVENT_T event_data;
    MMISRVAUD_TYPE_E type;
    uint32 sample_rate;
}SRV_BT_EVENT_T;

typedef struct
{
    SRV_BT_STATE_E state;
    SRV_BT_EVENT_T cur_event;
    SRV_BT_EVENT_T wait_event;
    uint8 timer;
}SRV_BT_INFO_T;

#ifdef MMI_AUTOTEST_SUPPORT

#define TYPE_DEF(x)  #x
const char *s_unknown_str = "UNKNOWN";
const char *srv_type_name[] =
{
    MMISRVAUD_TYPE_DEF
};

const char *srv_aud_state_name[] = 
{
    AUDIO_STATE_DEF
};

const char *bt_state_name[] = 
{
    BT_STATE_DEF
};

#undef TYPE_DEF

#endif

/*****************************************************************************/
//  Description : Create a new audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T SrvCreate(MMISRVMGR_SERVICE_REQ_T *req_data, uint32 ext_data);

/*****************************************************************************/
//  Description : Resume a audio service by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvResume(IMMISRV_UNKNOWN_T *srv_ptr);

/*****************************************************************************/
//  Description : Suspend a audio service by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvSuspend(IMMISRV_UNKNOWN_T *srv_ptr);

/*****************************************************************************/
//  Description : Destroy a audio service by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvFree(IMMISRV_UNKNOWN_T *srv_ptr);

/*****************************************************************************/
//  Description : Set audio service priority.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvSetPri(IMMISRV_UNKNOWN_T *srv_ptr, uint16 pri);

/*****************************************************************************/
//  Description : Set audio service notify function.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvSetNotify(IMMISRV_UNKNOWN_T *srv_ptr, MMISRVMGR_NOTIFY_FUNC notify);

/*****************************************************************************/
//  Description : Data comunication between client and audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvIoCtrl(IMMISRV_UNKNOWN_T *srv_ptr, uint32 data, void *res);

/*****************************************************************************/
//  Description : Handle BT delay time out.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void HandleBTCloseDelayTimer(uint8 timer, uint32 param);

/*****************************************************************************/
//  Description : Audio play start.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL MMISRVAUD_RET_E AudPlay(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr);

/*****************************************************************************/
//  Description : Audio resume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AudResume(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr);

/*****************************************************************************/
//  Description : Audio pause.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AudPause(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr, BOOLEAN is_change_state);

/*****************************************************************************/
//  Description : Handle BT event.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void BtEventHandle(MMISRVAUD_BT_EVENT_E event);

/*****************************************************************************/
//  Description : Handle BT event.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void EarphoneEventHandle(MMISRVAUD_EARPHONE_EVENT_ID event);

/*****************************************************************************/
//  Description : Audio stop.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AudStop(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr);

/*****************************************************************************/
//  Description : Set audio manual_route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetManualRoute(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_ROUTE_T manual_route);

/*****************************************************************************/
//  Description : Get bt open sample rate.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint32 BtGetSampleRate(SRVAUD_ENTITY_INFO_T *entity_ptr);

/*****************************************************************************/
//  Description : Make a judgement if there is some service in playing state.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsActiveAudPlaying(void);

/*****************************************************************************/
//  Description : Stop all standing service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void StopStandingAud(void);

LOCAL uint8 s_aud_service_id = 0;
LOCAL MMISRVAUD_MUTEX_PTR s_aud_service_mutex = PNULL;

LOCAL SRVAUD_INTERNAL_STATUES_T s_mmisrv_aud_statues = {0};
LOCAL SRVAUD_HANDLE_BOX_T s_handle_box = {0};

LOCAL SRV_BT_INFO_T s_bt_info = {0};
LOCAL BOOLEAN s_is_async_mode = FALSE;

LOCAL const MMISRV_VTBL_T(IMMISRV_AUD_T) s_mmisrv_aud_vtbl =
{
    SrvResume,
    SrvSuspend,
    SrvFree,
    SrvSetPri,
    SrvSetNotify,
    SrvIoCtrl
};

PUBLIC uint32 VBDecodeOuputBuf[DECODE_OUTPUT_BUF_SIZE] = {0};
#ifdef VIRTUAL_VIBRATE_FUNC
PUBLIC uint32 VBDecode_VibrateOuputBuf[DECODE_OUTPUT_BUF_SIZE] = {0};
#endif

/*****************************************************************************/
//  Description : Init audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void InitSrvAud(void)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio InitSrvAud() entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_328_112_2_18_3_23_44_92,(uint8*)""));
    
    s_mmisrv_aud_statues.is_mute = FALSE;
    s_mmisrv_aud_statues.max = MMISRVAUD_SERVICE_NUM_MAX;

    if(MMISRVAUD_LinkCreate())
    {
        s_mmisrv_aud_statues.is_enable = TRUE;
        s_aud_service_mutex = MMISRVAUD_CreateMutex();
    }
    else
    {
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio InitSrvAud() failed!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_340_112_2_18_3_23_44_93,(uint8*)"");
        s_mmisrv_aud_statues.is_enable = FALSE;
    }
}

/*****************************************************************************/
//  Description : Make a judgetment to determine if the entity is a valid entity.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEntityValid(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    BOOLEAN result = FALSE;
    MMISRV_HANDLE_T handle = 0;
    uint32 index = 0;
//#define IS_ENTITY_VALID(x) if((x) != PNULL && (x)->handle == s_handle_box
    if(entity_ptr != PNULL)
    {
        handle = entity_ptr->handle;
        index = handle & 0xFFFF;
        if(index < MMISRVAUD_SERVICE_NUM_MAX)
        {
            if(s_handle_box.handle[index] == handle && s_handle_box.entity_addr[index] == (uint32)entity_ptr)
            {
                result = TRUE;
            }
        }
    }
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio IsEntityValid entity=0x%x, result=%d", entity_ptr, result));
    return result;
}

/*****************************************************************************/
//  Description : Add a new handle to handle box.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint32 GenAndAddHandleToBox(uint32 entity_addr)
{
    uint32 i = 0;
    uint32 handle_gen = 0;
    static uint8 loop_index = 0;
    
    if(entity_addr > 0)
    {
        while(i < MMISRVAUD_SERVICE_NUM_MAX)
        {
            if(0 == s_handle_box.handle[i])
            {
                handle_gen = (s_aud_service_id << 24) | ((loop_index++) << 16) | i;
                s_handle_box.handle[i] = handle_gen;
                s_handle_box.entity_addr[i] = entity_addr;
                s_handle_box.num++;
                break;
            }
            i++;
        }
    }
    return handle_gen;
}


/*****************************************************************************/
//  Description : Delete a handle from handle box.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DeleteFromHandleBoxByEntityAddr(uint32 entity_addr)
{
    uint32 i = 0;
    BOOLEAN res = FALSE;
    if(entity_addr > 0)
    {
        while(i < MMISRVAUD_SERVICE_NUM_MAX)
        {
            if(entity_addr == s_handle_box.entity_addr[i])
            {
                s_handle_box.handle[i] = 0;
                s_handle_box.entity_addr[i] = 0;
                s_handle_box.num--;
                res = TRUE;
                break;
            }
            i++;
        }
    }
    return res;
}

/*****************************************************************************/
//  Description : Get handle by entity address.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T GetHandleByEntityAddr(uint32 entity_addr)
{
    MMISRV_HANDLE_T handle = 0;
    uint32 total = 0;
    uint i = 0;
    if(entity_addr > 0)
    {
    #if 1 
        while(i < MMISRVAUD_SERVICE_NUM_MAX && total < s_handle_box.num)
        {
            if(s_handle_box.entity_addr[i] != 0)
            {
                if(entity_addr == s_handle_box.entity_addr[i])
                {
                    handle = s_handle_box.handle[i];
                    break;
                }
                total++;
            }
            i++;
        }
    #else
        /* This way is not safty, entity_addr maybe a invalidated address */
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetHandleByEntityAddr() entry,entity_addr=0x%x, handle=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_427_112_2_18_3_23_44_94,(uint8*)"dd", entity_addr, ((SRVAUD_ENTITY_INFO_T *)entity_addr)->handle));
        return ((SRVAUD_ENTITY_INFO_T *)entity_addr)->handle;
    #endif
    }
    return handle;
}

/*****************************************************************************/
//  Description : Get handle by entity address.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL uint32 GetEntityAddrByHandle(MMISRV_HANDLE_T handle)
{
    uint32 i = 0;
    uint32 entity_addr = PNULL;
    if(handle > 0)
    {
        while(i < MMISRVAUD_SERVICE_NUM_MAX)
        {
            if(handle == s_handle_box.handle[i])
            {
                entity_addr = s_handle_box.entity_addr[i];
                break;
            }
            i++;
        }
    }
    return entity_addr;
}

/*****************************************************************************/
//  Description : Get handle by client name.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T GetHandleByClientName(char *client_name)
{
    uint32 i = 0;
    MMISRV_HANDLE_T handle = 0;
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    uint32 num = 0;
    if(client_name != PNULL)
    {
        while(i < MMISRVAUD_SERVICE_NUM_MAX && num < s_handle_box.num)
        {
            if(s_handle_box.entity_addr[i] != 0)
            {
                entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_handle_box.entity_addr[i];
                if(entity_ptr != PNULL)
                {
                    if(strcmp(client_name, entity_ptr->client_name) == 0)
                    {
                        handle = entity_ptr->handle;
                        break;
                    }
                }
                num++;
            }
            i++;
        }
    }
    return handle;

}

/*****************************************************************************/
//  Description : Get handle by volume type.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T GetHandleByVolumeType(MMISRVAUD_VOLUME_TYPE_E volume_type)
{
    SRVAUD_ENTITY_INFO_T *head_ptr = PNULL;
    SRVAUD_ENTITY_INFO_T *next_ptr = PNULL;
    MMISRV_HANDLE_T handle = 0;
    
    head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
    if(PNULL == head_ptr)
    {
        return 0;
    }
    
    next_ptr = head_ptr;
    do
    {
        if(next_ptr->audio.audio_info.volume_type == volume_type)
        {
            handle = next_ptr->handle;
            break;
        }
    }while((next_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetNextNode()) != head_ptr && next_ptr != PNULL);

    return handle;
}

/*****************************************************************************/
//  Description : Notify client.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SrvNotifyClient(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVMGR_NOTIFY_EVENT_E event, uint32 data)
{
    BOOLEAN result = FALSE;
    MMISRV_HANDLE_T handle = 0;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvNotifyClient() entry, entity_ptr=0x%x, entity_ptr->service.notify=0x%x,event=%d, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_528_112_2_18_3_23_44_95,(uint8*)"dddd", entity_ptr, entity_ptr->service.notify, event, data));
    
    if(entity_ptr != PNULL && entity_ptr->service.notify != PNULL && event < MMISRVMGR_NOTIFY_EVENT_MAX)
    {
        MMISRVMGR_NOTIFY_PARAM_T param = {0};
        param.event = event;
        param.ind_data = entity_ptr->service.ind_data;
        param.data = data;
        handle = GetHandleByEntityAddr((uint32)entity_ptr);
        if(handle != 0)
        {
            result = entity_ptr->service.notify(handle, &param);
        }
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvNotifyClient() exit,handle=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_541_112_2_18_3_23_44_96,(uint8*)"d", handle));
    
    return result;
}

/*****************************************************************************/
//  Description : Notify client extend event.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL void SrvNotifyClientExt(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_REPORT_TYPE_E rpt, uint32 data)
{
    MMISRVAUD_REPORT_T rpt_data = {0};
    rpt_data.report = rpt;
    rpt_data.data1 = data;
    SrvNotifyClient(entity_ptr, MMISRVMGR_NOTIFY_EXT, (uint32)&rpt_data);
}

/*****************************************************************************/
//  Description : Get mmi service handle by audio driver handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL SRVAUD_ENTITY_INFO_T* GetEntityByDrvHandle(DRVAUD_HANDLE drv_handle)
{
    uint32 i = 0;
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    if(drv_handle > 0)
    {
        while(i < MMISRVAUD_SERVICE_NUM_MAX)
        {
            if(s_handle_box.entity_addr[i] != 0)
            {
                entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_handle_box.entity_addr[i];
                if(entity_ptr->drv_handle == drv_handle)
                {
                    break;
                }
            }
            i++;
        }
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetEntityByDrvHandle() exit, drv handle=0x%x, i=%d, entity_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_581_112_2_18_3_23_44_97,(uint8*)"ddd", drv_handle, i, entity_ptr));
    
    if(i < MMISRVAUD_SERVICE_NUM_MAX)
    {
        return entity_ptr;
    }
    else
    {
        return PNULL;
    }
}


/*****************************************************************************/
//  Description : Get mmi service handle by timer id.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL SRVAUD_ENTITY_INFO_T* GetEntityByTimer(uint8 timer)
{
    uint32 i = 0;
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    if(timer > 0)
    {
        while(i < MMISRVAUD_SERVICE_NUM_MAX)
        {
            if(s_handle_box.entity_addr[i] != 0)
            {
                entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_handle_box.entity_addr[i];
                if(entity_ptr->audio.timer == timer)
                {
                    break;
                }
            }
            i++;
        }
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetEntityByTimer() exit, timer=%d, i=%d, entity_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_617_112_2_18_3_23_44_98,(uint8*)"ddd", timer, i, entity_ptr));
    
    if(i < MMISRVAUD_SERVICE_NUM_MAX)
    {
        return entity_ptr;
    }
    else
    {
        return PNULL;
    }
}

#if 0
/*****************************************************************************
//  Description : make a judgement if audio service is can provide these two services at the same time.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:  In these cases, service can coexist:
                    call    record
                    call    play recorded sound
                    call    Tone(DTMF and other replaced sound)
                    fm     record
                    sound    vibrate
                    vibrate sound
    Now, we don't consider the audio route, but we should consider it in the later.
*****************************************************************************/
LOCAL BOOLEAN IsSrvCoexist(MMISRVAUD_TYPE_E active_type, MMISRVAUD_TYPE_E req_type)
{
    BOOLEAN result = FALSE;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsSrvAudCoexist() entry, active_type=0x%x, req_type=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_645_112_2_18_3_23_44_99,(uint8*)"dd", active_type, req_type));

    switch(active_type)
    {
    case MMISRVAUD_VOICE:
        switch(req_type)
        {
        case MMISRVAUD_RECORD_FILE:
        case MMISRVAUD_RECORD_BUF:
        case MMISRVAUD_TONE_DTMF:
        case MMISRVAUD_TONE_GENERIC:
        case MMISRVAUD_TYPE_VIBERATE:
            result = TRUE;
            break;
        default:
            break;
        }
        break;
    case MMISRVAUD_FM:
        switch(req_type)
        {
        case MMISRVAUD_RECORD_FILE:
        case MMISRVAUD_RECORD_BUF:
        case MMISRVAUD_TYPE_VIBERATE:
            result = TRUE;
            break;
        default:            
            break;
        }
        break;
    case MMISRVAUD_TYPE_VIBERATE:
        if(req_type != MMISRVAUD_TYPE_VIBERATE)
        {
            result = TRUE;
        }
        break;
    default:
        break;
    }

    return result;
}
#endif

#ifdef VIDEO_WP_SUPPORT
/*****************************************************************************/
//  Description : If is video wallpaper co-exist mode, and the existing service is a service with sound.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsVwpCoexist(void)
{
    uint32 num = 0;
    BOOLEAN res = FALSE;

    BOOLEAN is_vwp = FALSE;
    BOOLEAN is_sound_service = FALSE;
    MMISRVAUD_TYPE_E type = 0;

    
    num = MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE);
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsVwpCoexist() entry, active link num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_706_112_2_18_3_23_44_100,(uint8*)"d", num));
    
    if(num >= 2)
    {
        SRVAUD_ENTITY_INFO_T *head_ptr = PNULL;
        SRVAUD_ENTITY_INFO_T *next_ptr = PNULL;

        head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
        if(head_ptr != PNULL)
        {
            next_ptr = head_ptr;
            do
            {
                type = next_ptr->audio.audio_info.info.type;
                switch(type)
                {
                case MMISRVAUD_TYPE_VIDEO_WP:
                    is_vwp = TRUE;
                    break;
                case MMISRVAUD_TYPE_RING_FILE:
                case MMISRVAUD_TYPE_RING_BUF:
                case MMISRVAUD_TYPE_FM:
                    is_sound_service = TRUE;
                    break;
                default:
                    break;
                }
                if(is_vwp && is_sound_service)
                {
                    res = TRUE;
                    break;
                }
            }while((next_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetNextNode()) != head_ptr && next_ptr != PNULL);
        }
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsVwpCoexist() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_742_112_2_18_3_23_44_101,(uint8*)"d", res));
    
    return res;
}


/*****************************************************************************/
//  Description : If need mute video wallpaper's sound.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
//LOCAL BOOLEAN IsNeedMuteVwp()
//{
//}

LOCAL void VwpSetSound(BOOLEAN is_sound_on)
{
    MMISRVAUD_SetVideoSound(is_sound_on);
}

/*****************************************************************************/
//  Description :Get active entity by type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL SRVAUD_ENTITY_INFO_T* GetActiveEntityByType(MMISRVAUD_TYPE_E type)
{
    uint32 num = 0;

    num = MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE);
        
    if(num >= 2)
    {
        SRVAUD_ENTITY_INFO_T *head_ptr = PNULL;
        SRVAUD_ENTITY_INFO_T *next_ptr = PNULL;

        head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
        if(head_ptr != PNULL)
        {
            next_ptr = head_ptr;
            do
            {
                if(type == next_ptr->audio.audio_info.info.type)
                {
                    return next_ptr;
                }
            }while((next_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetNextNode()) != head_ptr && next_ptr != PNULL);
        }
    }    

    return PNULL;
}

/*****************************************************************************/
//  Description : Get video wallpaper entity ptr.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only return the ptr when it's in video wallpaer co-exist state. Please pay more attention.
/*****************************************************************************/
LOCAL SRVAUD_ENTITY_INFO_T *GetVwpPtr(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    BOOLEAN is_get_vwp = FALSE;
    SRVAUD_ENTITY_INFO_T *vwp_ptr = PNULL;
    MMISRVAUD_TYPE_E cur_type = 0;

    if(entity_ptr != PNULL)
    {
        cur_type = entity_ptr->audio.audio_info.info.type;
        if(IsVwpCoexist())
        {
            vwp_ptr = GetActiveEntityByType(MMISRVAUD_TYPE_VIDEO_WP);
            if(vwp_ptr != PNULL)
            {
                switch(cur_type)
                {
                case MMISRVAUD_TYPE_RING_BUF:
                case MMISRVAUD_TYPE_RING_FILE:
                case MMISRVAUD_TYPE_FM:
                    is_get_vwp = MMISRVAUD_LinkGetAndDelete(SRVAUD_LINK_ACTIVE, (uint32)vwp_ptr);
                    break;
                default:
                    break;
                }
            }
        }
    }
    
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio GetVwpPtr(), is_get_vwp=%d, vwp_ptr=0x%x, cur_type=%d, entity_ptr=0x%x", is_get_vwp, vwp_ptr, cur_type, entity_ptr));

    if(is_get_vwp)
    {
        return vwp_ptr;
    }
    else
    {
        return PNULL;
    }
}
#endif

/*******************************************************************/
//  Interface:		IsUnCoexist
//  Description : 	IsUnCoexist
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN IsUnCoexist(uint32 flag, MMISRVAUD_TYPE_U* type_ptr)
{
    BOOLEAN bret = FALSE;
    
    if (PNULL == type_ptr)
    {
        return FALSE;
    }
    
    // first FM,then recorder
    if ( FL(MMISRVAUD_TYPE_FM) == (flag & FL(MMISRVAUD_TYPE_FM)))
    {
        MMISRVAUD_RECORD_SOURCE_E record_src = MMISRVAUD_RECORD_SOURCE_FM;
        
        if(MMISRVAUD_TYPE_RECORD_BUF == type_ptr->type)
        {
            record_src = type_ptr->record_buf.source;
        }
        else if(MMISRVAUD_TYPE_RECORD_FILE == type_ptr->type)
        {
            record_src = type_ptr->record_file.source;
        }
        
        if(MMISRVAUD_RECORD_SOURCE_FM != record_src)
        {
            bret = TRUE;
        }
    }

    // first recorder,then FM
    else if (FL(MMISRVAUD_TYPE_RECORD_FILE) == (flag & FL(MMISRVAUD_TYPE_RECORD_FILE))
        || FL(MMISRVAUD_TYPE_RECORD_BUF) == (flag & FL(MMISRVAUD_TYPE_RECORD_BUF)))
    {
        if (MMISRVAUD_TYPE_FM == type_ptr->type)
        {
            bret = TRUE;
        }
    }
    
    return bret;
}

/*****************************************************************************/
//  Description : make a judgement if the new service can co-exist with the current active services.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: if there is a active service whose type is same with new type, we think it can not be co-exist.
/*****************************************************************************/
LOCAL BOOLEAN IsNewSrvCoexist(MMISRVAUD_TYPE_U* type_ptr)
{
#ifdef VIRTUAL_VIBRATE_FUNC
    const uint32 co_table[] = 
    {
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_FILE) |FL(MMISRVAUD_TYPE_TONE_DTMF) /*| FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_FILE) |FL(MMISRVAUD_TYPE_TONE_GENERIC) /*| FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_FILE) |FL(MMISRVAUD_TYPE_TONE_CUSTOM) /*| FL(MMISRVAUD_TYPE_VIBERATE)*/,

        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_BUF) | FL(MMISRVAUD_TYPE_TONE_DTMF) /*| FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_BUF) | FL(MMISRVAUD_TYPE_TONE_GENERIC) /* | FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_BUF) | FL(MMISRVAUD_TYPE_TONE_CUSTOM) /* | FL(MMISRVAUD_TYPE_VIBERATE)*/,

        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_DTMF) /* | FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_GENERIC) /* | FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_CUSTOM) /* | FL(MMISRVAUD_TYPE_VIBERATE)*/,

#if 0 //TONE_CONFLICT_SUPPORT        
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_TONE_DTMF) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_TONE_GENERIC) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_TONE_CUSTOM) | FL(MMISRVAUD_TYPE_VIBERATE),
#endif        
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_ASP_BUF)/* | FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_ASP_FILE) /*| FL(MMISRVAUD_TYPE_VIBERATE)*/,
        
        FL(MMISRVAUD_TYPE_FM) | FL(MMISRVAUD_TYPE_RECORD_FILE),
        FL(MMISRVAUD_TYPE_FM) | FL(MMISRVAUD_TYPE_RECORD_BUF) ,
#if defined(VIRTUAL_VIBRATE_FUNC)
        FL(MMISRVAUD_TYPE_RING_FILE) | FL(MMISRVAUD_TYPE_VIBERATE),        
        FL(MMISRVAUD_TYPE_RING_BUF) | FL(MMISRVAUD_TYPE_VIBERATE),        
#else
        FL(MMISRVAUD_TYPE_RING_FILE) |FL(MMISRVAUD_TYPE_FM) | FL(MMISRVAUD_TYPE_VIBERATE),        
        FL(MMISRVAUD_TYPE_RING_BUF) |FL(MMISRVAUD_TYPE_FM) | FL(MMISRVAUD_TYPE_VIBERATE),  
#endif		     
        FL(MMISRVAUD_TYPE_STREAMING) /*| FL(MMISRVAUD_TYPE_VIBERATE)*/,        
        
#ifdef VIDEO_WP_SUPPORT
        FL(MMISRVAUD_TYPE_VIDEO_WP) | FL(MMISRVAUD_TYPE_RING_FILE),        
        FL(MMISRVAUD_TYPE_VIDEO_WP) | FL(MMISRVAUD_TYPE_RING_BUF) ,        
        FL(MMISRVAUD_TYPE_VIDEO_WP) | FL(MMISRVAUD_TYPE_FM),                
#endif

#ifdef UAL_BT_HFU_SUPPORT
		FL(MMISRVAUD_TYPE_VOICE_BTCALL) | FL(MMISRVAUD_TYPE_IQ)/* | FL(MMISRVAUD_TYPE_TONE_DTMF) | FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE_BTCALL) | FL(MMISRVAUD_TYPE_IQ)/* | FL(MMISRVAUD_TYPE_TONE_GENERIC) | FL(MMISRVAUD_TYPE_VIBERATE)*/,
        FL(MMISRVAUD_TYPE_VOICE_BTCALL) | FL(MMISRVAUD_TYPE_IQ)/* | FL(MMISRVAUD_TYPE_TONE_CUSTOM) | FL(MMISRVAUD_TYPE_VIBERATE)*/,
#endif

		/* These  lines is for testing */
        //FL(MMISRVAUD_TYPE_RING_FILE) | FL(MMISRVAUD_TYPE_VIBERATE) | FL(MMISRVAUD_TYPE_RECORD_BUF),        
        //FL(MMISRVAUD_TYPE_RING_BUF) | FL(MMISRVAUD_TYPE_VIBERATE) | FL(MMISRVAUD_TYPE_RECORD_BUF),        
        //FL(MMISRVAUD_TYPE_RING_FILE) | FL(MMISRVAUD_TYPE_VIBERATE) | FL(MMISRVAUD_TYPE_RECORD_FILE),        
        //FL(MMISRVAUD_TYPE_RING_BUF) | FL(MMISRVAUD_TYPE_VIBERATE) | FL(MMISRVAUD_TYPE_RECORD_FILE),        
        0    
    };

#else
    const uint32 co_table[] = 
    {
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_FILE) |FL(MMISRVAUD_TYPE_TONE_DTMF) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_FILE) |FL(MMISRVAUD_TYPE_TONE_GENERIC) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_FILE) |FL(MMISRVAUD_TYPE_TONE_CUSTOM) | FL(MMISRVAUD_TYPE_VIBERATE),

        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_BUF) | FL(MMISRVAUD_TYPE_TONE_DTMF) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_BUF) | FL(MMISRVAUD_TYPE_TONE_GENERIC) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_RECORD_BUF) | FL(MMISRVAUD_TYPE_TONE_CUSTOM) | FL(MMISRVAUD_TYPE_VIBERATE),

        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_DTMF) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_GENERIC) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_CUSTOM) | FL(MMISRVAUD_TYPE_VIBERATE),

#if 0 //TONE_CONFLICT_SUPPORT        
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_TONE_DTMF) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_TONE_GENERIC) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_TONE_CUSTOM) | FL(MMISRVAUD_TYPE_VIBERATE),
#endif        
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_ASP_BUF) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE) | FL(MMISRVAUD_TYPE_ASP_FILE) | FL(MMISRVAUD_TYPE_VIBERATE),
        
        FL(MMISRVAUD_TYPE_FM) | FL(MMISRVAUD_TYPE_RECORD_FILE) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_FM) | FL(MMISRVAUD_TYPE_RECORD_BUF) | FL(MMISRVAUD_TYPE_VIBERATE),

        FL(MMISRVAUD_TYPE_RING_FILE) | FL(MMISRVAUD_TYPE_VIBERATE),        
        FL(MMISRVAUD_TYPE_RING_BUF) | FL(MMISRVAUD_TYPE_VIBERATE),        
        FL(MMISRVAUD_TYPE_STREAMING) | FL(MMISRVAUD_TYPE_VIBERATE),        
        
#ifdef VIDEO_WP_SUPPORT
        FL(MMISRVAUD_TYPE_VIDEO_WP) | FL(MMISRVAUD_TYPE_RING_FILE) | FL(MMISRVAUD_TYPE_VIBERATE),        
        FL(MMISRVAUD_TYPE_VIDEO_WP) | FL(MMISRVAUD_TYPE_RING_BUF) | FL(MMISRVAUD_TYPE_VIBERATE),        
        FL(MMISRVAUD_TYPE_VIDEO_WP) | FL(MMISRVAUD_TYPE_FM) | FL(MMISRVAUD_TYPE_VIBERATE),                
#endif

#ifdef UAL_BT_HFU_SUPPORT
        FL(MMISRVAUD_TYPE_VOICE_BTCALL) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_DTMF) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE_BTCALL) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_GENERIC) | FL(MMISRVAUD_TYPE_VIBERATE),
        FL(MMISRVAUD_TYPE_VOICE_BTCALL) | FL(MMISRVAUD_TYPE_IQ) | FL(MMISRVAUD_TYPE_TONE_CUSTOM) | FL(MMISRVAUD_TYPE_VIBERATE),
#endif

#ifdef BT_AV_SNK_SUPPORT
        FL(MMISRVAUD_TYPE_BT_MUSIC) | FL(MMISRVAUD_TYPE_VIBERATE),
#endif
        /* These  lines is for testing */
        //FL(MMISRVAUD_TYPE_RING_FILE) | FL(MMISRVAUD_TYPE_VIBERATE) | FL(MMISRVAUD_TYPE_RECORD_BUF),        
        //FL(MMISRVAUD_TYPE_RING_BUF) | FL(MMISRVAUD_TYPE_VIBERATE) | FL(MMISRVAUD_TYPE_RECORD_BUF),        
        //FL(MMISRVAUD_TYPE_RING_FILE) | FL(MMISRVAUD_TYPE_VIBERATE) | FL(MMISRVAUD_TYPE_RECORD_FILE),        
        //FL(MMISRVAUD_TYPE_RING_BUF) | FL(MMISRVAUD_TYPE_VIBERATE) | FL(MMISRVAUD_TYPE_RECORD_FILE),        
        0    
    };
#endif

    SRVAUD_ENTITY_INFO_T *head_ptr = PNULL;
    SRVAUD_ENTITY_INFO_T *next_ptr = PNULL;
    uint32 flag = 0;
    MMISRVAUD_TYPE_E active_type = 0;
    BOOLEAN is_type_same = FALSE;
    uint i = 0;
    MMISRVAUD_TYPE_E type = 0;

    if (PNULL == type_ptr)
    {
        return FALSE;
    }    
    type = type_ptr->type;    

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsNewSrvCoexist() entry, type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_810_112_2_18_3_23_45_102,(uint8*)"d", type));
    
    head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
    active_type = head_ptr->audio.audio_info.info.type;
    if(active_type == type)
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsNewSrvCoexist(), head node's type is same with type!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_816_112_2_18_3_23_45_103,(uint8*)""));
        return FALSE;
    }
    
    flag = FL(active_type) | FL(type);
    /* travel the active link, find all the service type */
    while((next_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetNextNode()) != head_ptr && next_ptr != PNULL)
    {
        active_type = next_ptr->audio.audio_info.info.type;
        /*if some active service type is same with the new, stop travel */
        if(active_type == type)
        {
            is_type_same = TRUE;
            break;
        }
        flag |= FL(active_type);
    }

    /* if find out type is same, we think it not be co-exist */
    if(is_type_same)
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsNewSrvCoexist(), type is same"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_837_112_2_18_3_23_45_104,(uint8*)""));
        return FALSE;
    }

    while(co_table[i] != 0)
    {
        /* if flag in a sub-range of co_table[i], we think it can be co-exist */
        if( (flag & co_table[i]) == flag)
        {
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsNewSrvCoexist(), Congradulations, It's can be co-exist!!!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_846_112_2_18_3_23_45_105,(uint8*)""));
            
            if (IsUnCoexist(flag, type_ptr))
            {
                return FALSE;
            }
            return TRUE;
        }
        i++;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsNewSrvCoexist(), Sorry, can't be co-exist!!!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_853_112_2_18_3_23_45_106,(uint8*)""));
    return FALSE;
}

/*****************************************************************************/
//  Description : make a judgement if audio service has been init and works well.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSrvEnable(void)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsSrvAudEnable() entry, is_enable=%d, s_aud_service_id=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_863_112_2_18_3_23_45_107,(uint8*)"dd", s_mmisrv_aud_statues.is_enable, s_aud_service_id));
    return (s_mmisrv_aud_statues.is_enable && s_aud_service_id != 0);
}

/*****************************************************************************/
//  Description : make a judgement if need switch
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsSrvNeedSwitch(void)
{
    int32 num = MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE);
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsSrvNeedSwitch() entry, active service num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_874_112_2_18_3_23_45_108,(uint8*)"d", num));

    if (num > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : make a judgement if audio service type is standing service type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: we regard TONE type as standing type.
/*****************************************************************************/
LOCAL BOOLEAN IsSrvTypeStanding(MMISRVAUD_TYPE_E type)
{
#ifdef TONE_CONFLICT_SUPPORT
#ifdef VIRTUAL_VIBRATE_FUNC
   if( MMISRVAUD_TYPE_VIBERATE == type)
    {
        SRVAUD_ENTITY_INFO_T *entity_active = PNULL;
        entity_active = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
        if(PNULL == entity_active)
        {
            return FALSE;
        }
        if( entity_active->audio.audio_info.info.type==MMISRVAUD_TYPE_VIRTUAL )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
    	return FALSE;
    }

#else
    if(MMISRVAUD_TYPE_VIBERATE == type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
#else
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsSrvTypeStanding() entry, type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_902_112_2_18_3_23_45_109,(uint8*)"d", type));

    if(MMISRVAUD_TYPE_TONE_DTMF == type || MMISRVAUD_TYPE_TONE_GENERIC == type || MMISRVAUD_TYPE_TONE_CUSTOM == type)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif    
}


/*****************************************************************************/
//  Description : make a judgement if the two type is same
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsTypeSame(MMISRVAUD_TYPE_E type1, MMISRVAUD_TYPE_E type2)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsTypeSame() entry, type1=%d, type2=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_922_112_2_18_3_23_45_110,(uint8*)"dd", type1, type2));

    if(type1 == type2)
    {
        return TRUE;
    }
    else
    {
        if((MMISRVAUD_TYPE_RING_FILE == type1 && MMISRVAUD_TYPE_RING_BUF == type2) 
            || (MMISRVAUD_TYPE_RING_BUF == type1 && MMISRVAUD_TYPE_RING_FILE == type2)
        )
        {
            return TRUE;
        }
        /* This function is used to make a judgement that some two service can be mixed,
            So we don't regard MMISRVAUD_RECORD_FILE and MMISRVAUD_RECORD_BUF as the same type.
        */
    }
    return FALSE;
}

/*****************************************************************************/
//  Description : Get the current service work mode.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL SRV_WORK_MODE_E GetCurrentWorkMode(void)
{
    uint32 num = 0;
    SRV_WORK_MODE_E mode = SRV_WORK_MODE_NONE;
    
    num = MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE);
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetCurrentWorkMode() entry, active link num=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_954_112_2_18_3_23_45_111,(uint8*)"d", num));
    
    if(0 == num)
    {
        return SRV_WORK_MODE_NONE;
    }
    else if(1 == num)
    {
        return SRV_WORK_MODE_SINGLE;
    }
    else
    {
        SRVAUD_ENTITY_INFO_T *head_ptr = PNULL;
        SRVAUD_ENTITY_INFO_T *next_ptr = PNULL;
        mode = SRV_WORK_MODE_MIXING;
        head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
        while((next_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetNextNode()) != head_ptr && next_ptr != PNULL)
        {
            if(head_ptr->audio.audio_info.info.type != next_ptr->audio.audio_info.info.type)
            {
                mode = SRV_WORK_MODE_COEXIST;
                break;
            }
        }
        
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetCurrentWorkMode() exit, mode=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_979_112_2_18_3_23_45_112,(uint8*)"d", mode));
        
        return mode;
    }
}

/*****************************************************************************/
//  Description : Get the create mode when create a new service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL SRV_CREATE_MODE_E GetCreateMode(MMISRVMGR_SERVICE_REQ_T *req_data, MMISRVAUD_TYPE_U *type_data, BOOLEAN is_mixing_enable)
{
    SRV_WORK_MODE_E work_mode = SRV_WORK_MODE_NONE;
    SRV_CREATE_MODE_E mode = SRV_CREATE_MODE_NONE;
    SRVAUD_ENTITY_INFO_T *entity_active = PNULL;
    MMISRVAUD_TYPE_U *audio_req = PNULL;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetCreateMode() entry, req_data=0x%x, type_data=0x%x, is_mixing_enable=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_996_112_2_18_3_23_45_113,(uint8*)"ddd", req_data, type_data, is_mixing_enable));
    
    if(!IsSrvEnable())
    {
        return SRV_CREATE_MODE_NONE;
    }
    
    if(PNULL == req_data || PNULL == type_data )
    {
        return SRV_CREATE_MODE_NONE;
    }

    audio_req = type_data;
    if(audio_req->type == MMISRVAUD_TYPE_NONE || audio_req->type >= MMISRVAUD_TYPE_MAX)
    {
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio GetCreateMode(), audio_req->type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1011_112_2_18_3_23_45_114,(uint8*)"d", audio_req->type);
        return SRV_CREATE_MODE_NONE;
    }
    /*  if current total service is exeeded the max, do not create a new service */
    if(s_handle_box.num >= s_mmisrv_aud_statues.max)
    {
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio GetCreateMode(), service num is exeeded the limit!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1017_112_2_18_3_23_45_115,(uint8*)"");
        return SRV_CREATE_MODE_NONE;
    }
    if(IsSrvTypeStanding(audio_req->type))
    {
        return SRV_CREATE_MODE_STANDING;
    }
    
    work_mode = GetCurrentWorkMode();
    switch(work_mode)
    {
    case SRV_WORK_MODE_NONE:
        mode = SRV_CREATE_MODE_SINGLE;
        break;
    case SRV_WORK_MODE_SINGLE:
    case SRV_WORK_MODE_MIXING:        
        entity_active = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
        
		SCI_TRACE_LOW("GetCreateMode entity_active->service.pri = %d", entity_active->service.pri);
        SCI_TRACE_LOW("GetCreateMode entity_active->handle = 0x%x", entity_active->handle);
        SCI_TRACE_LOW("GetCreateMode entity_active->drv_handle = 0x%x", entity_active->drv_handle);
        /* If type is same, we should determin by mixing, and then priority */
        if(IsTypeSame(audio_req->type, entity_active->audio.audio_info.info.type))
        {
        #ifdef AUDIO_MIXING_SUPPORT            
            /* If enable mixing both, we should provide service */
            if(entity_active->audio.audio_info.is_mixing_enable && is_mixing_enable)
            {
                mode = SRV_CREATE_MODE_MIXING;
            }
            /* else, we should make determin by priority */
            else
        #endif
            {
                /* if the pri is higher than or equal to the active sevice pri, we should provide the service */ 
                if(req_data->pri >= entity_active->service.pri)
                {
                    mode = SRV_CREATE_MODE_SWITCH;
                }
                else
                {
                    mode = SRV_CREATE_MODE_NONE;
                }
            }
        }
        /* if type is different, we should to make sure if we can provide services at same time,
            sometime we should provide two or more service at the same time, for example, 
            we answer a call automatically with playing the recorded sound.
        */
        else
        {
            if(IsNewSrvCoexist(audio_req))
            {
                mode = SRV_CREATE_MODE_COEXIST;
            }
            else
            {
                /* if the pri is higher than or equal to the active sevice pri, we should provide the service */ 
                if(req_data->pri >= entity_active->service.pri)
                {
                    mode = SRV_CREATE_MODE_SWITCH;
                }
                else
                {
                    mode = SRV_CREATE_MODE_NONE;
                }
            }
        }
        break;
    case SRV_WORK_MODE_COEXIST:
        entity_active = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);        
        if(IsNewSrvCoexist(audio_req))
        {
            mode = SRV_CREATE_MODE_COEXIST;
        }
        else
        {
            /* if the pri is higher than or equal to the active sevice pri, we should provide the service */ 
            if(req_data->pri >= entity_active->service.pri)
            {
                mode = SRV_CREATE_MODE_SWITCH;
            }
            else
            {
                mode = SRV_CREATE_MODE_NONE;
            }
        }
        break;
    default:
        break;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetCreateMode() exit, pri=%d, type=%d, mode=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1106_112_2_18_3_23_45_116,(uint8*)"ddd", req_data->pri, audio_req->type, mode));
    
    return mode;
}

#if 0
/*****************************************************************************/
//  Description : make a judgement if audio service is can create a new service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCanCreateNewService(MMISRVMGR_SERVICE_REQ_T *req_data, void *ext_data)
{
    BOOLEAN result = TRUE;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsCanCreateNewService() entry, req_data=0x%x, ext_data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1119_112_2_18_3_23_45_117,(uint8*)"dd", req_data, ext_data));
    
    if(!IsSrvEnable())
    {
        return FALSE;
    }
    
    if(PNULL == req_data || PNULL == ext_data )
    {
        return FALSE;
    }

    /*  if current total service is exeeded the max, do not create a new service */
    if(s_mmisrv_aud_statues.total >= s_mmisrv_aud_statues.max)
    {
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio IsCanCreateNewService(), service exeed tha max value!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1134_112_2_18_3_23_45_118,(uint8*)"");
        return FALSE;
    }

    if(MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE) > 0)
    {
        SRVAUD_ENTITY_INFO_T *entity_active = PNULL;
        MMISRVAUD_TYPE_T *audio_req = (MMISRVAUD_TYPE_T *)ext_data;
        
        entity_active = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
        if(entity_active != PNULL)
        {

            /* if type is same, we should compare priority */
            if(audio_req->info.type == entity_active->audio.audio_info.info.type)
            {
                /* if the pri is higher than or equal to the active sevice pri, we should provide the service,
                    otherwise, we should regard that if the plateform support audio mixing.  
                */ 
                if(req_data->pri < entity_active->service.pri)
                {
                    _DBG(                    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsCanCreateNewService(), pri is lower than current!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1155_112_2_18_3_23_45_119,(uint8*)""));

                    #if AUDIO_MIXING_SUPPORT
                    
                    /* we should consider if support audio mixing */
                    if(!(entity_active->audio.audio_info.is_mixing_enable && audio_req->is_mixing_enable))
                    {
                        result = FALSE;
                    }
                    
                    #else
                    result = FALSE;
                    #endif
                }
            }
            /* if type is different, we should to make sure if we can provide services at same time,
                sometime we should provide two or more service at the same time, for example, 
                we answer a call automatically with playing the recorded sound.
            */
            else
            {
                /* Maybe we should consider the audio ROUTE later!!!!!!!!*/
                return IsSrvCoexist(entity_active->audio.audio_info.info.type, audio_req->info.type);
            }
        }
        else
        {
            //MMISRV_TRACE_ERR:"[MMISRV]: Audio IsCanCreateNewService(), entiry_ptr is NULL!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1182_112_2_18_3_23_45_120,(uint8*)"");
            result = FALSE;
        }
    }

    return result;
}
#endif

/*****************************************************************************/
//  Description : make a judgement if an audio service is can be resumed.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsCanResume(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    BOOLEAN result = TRUE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsCanResume() entry, entity_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1199_112_2_18_3_23_45_121,(uint8*)"d", entity_ptr));
    
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }

    /* if active link node is empty, we think it can be resumed */
    if(MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE) > 0)
    {
        SRVAUD_ENTITY_INFO_T *entity_active = PNULL;
        SRVAUD_ENTITY_SERVICE_INFO_T *service_ptr = &entity_ptr->service;
        
        entity_active = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);

        /* if active service is NULL, it is an error */
        if(entity_active != PNULL)
        {
            /* Firstly, we make a judgement if can co-exist */
            if(IsNewSrvCoexist(&entity_ptr->audio.audio_info.info))
            {
                /* Maybe we should consider the audio ROUTE later!!!!!!!!*/
                result = TRUE;
            }
            /* if cannot co-exit, we should compare priority */
            else
            {
                /* if the pri is higher than or equal to the active sevice pri, we should provide the service,
                    otherwise, we should regard that if the plateform support audio mixing.  
                */ 
                if(service_ptr->pri < entity_active->service.pri)
                {
                    _DBG(                    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsCanResume(), pri is lower than current!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1231_112_2_18_3_23_45_122,(uint8*)""));

                    #ifdef AUDIO_MIXING_SUPPORT
                    
                    /* we should consider if support audio mixing */
                    if(entity_active->audio.audio_info.is_mixing_enable && service_ptr->is_mixing_enable)
                    {
                        result = TRUE;
                    }
                    else
                    #else
                    {
                        result = FALSE;
                    }
                    #endif
                }
            }
        }
        else
        {
            //MMISRV_TRACE_ERR:"[MMISRV]: Audio IsCanResume(), error!!!! entity_active is NULL!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1251_112_2_18_3_23_45_123,(uint8*)"");
            result = FALSE;
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsCanResume() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1256_112_2_18_3_23_45_124,(uint8*)"d", result));

    return result;
}

/*****************************************************************************/
//  Description : Make a judgment if the audio type need to set EQ mode.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetEQMode(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    BOOLEAN res = FALSE;
    if(entity_ptr != PNULL)
    {
        switch(entity_ptr->audio.audio_info.info.type)
        {
        case MMISRVAUD_TYPE_RING_FILE:
        case MMISRVAUD_TYPE_RING_BUF:
        case MMISRVAUD_TYPE_STREAMING:
#ifdef VIRTUAL_VIBRATE_FUNC
        case MMISRVAUD_TYPE_VIBERATE: // 2013 0902
#endif
            res = TRUE;
            break;
        default:
            break;
        }
    }
    return res;
}

/*****************************************************************************/
//  Description : Make a judgment if the audio type need to set play rate.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetRate(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    BOOLEAN res = FALSE;
    if(entity_ptr != PNULL)
    {
        switch(entity_ptr->audio.audio_info.info.type)
        {
        case MMISRVAUD_TYPE_RING_FILE:
        case MMISRVAUD_TYPE_RING_BUF:
        case MMISRVAUD_TYPE_STREAMING:
            res = TRUE;
            break;
        default:
            break;
        }
    }
    return res;
}

/*****************************************************************************/
//  Description : Make a judgment if the audio type need to set volume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetVolume(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    BOOLEAN res = FALSE;
    if(entity_ptr != PNULL)
    {
        switch(entity_ptr->audio.audio_info.info.type)
        {
        case MMISRVAUD_TYPE_RING_FILE:
        case MMISRVAUD_TYPE_RING_BUF:
        case MMISRVAUD_TYPE_STREAMING:
        //case MMISRVAUD_TYPE_TONE_DTMF:
        //case MMISRVAUD_TYPE_TONE_GENERIC:
        //case MMISRVAUD_TYPE_TONE_CUSTOM:
        case MMISRVAUD_TYPE_VOICE:
        case MMISRVAUD_TYPE_VOICE_BTCALL:
		case MMISRVAUD_TYPE_BT_MUSIC:
        case MMISRVAUD_TYPE_VOICE_LOOPBACK:
        case MMISRVAUD_TYPE_FM:
        //case MMISRVAUD_TYPE_ASP_FILE:
        //case MMISRVAUD_TYPE_ASP_BUF:
        case MMISRVAUD_TYPE_VIDEO:
        case MMISRVAUD_TYPE_VIDEO_WP:
        case MMISRVAUD_TYPE_ATV:
        case MMISRVAUD_TYPE_MV:    
#ifdef VIRTUAL_VIBRATE_FUNC
        case MMISRVAUD_TYPE_VIBERATE: // 2013 0902
#endif

            res = TRUE;
            break;
        default:
            break;
        }
    }
    return res;
}

/*****************************************************************************/
//  Description : Make a judgment if the audio type need to set route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsNeedSetRoute(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    BOOLEAN res = FALSE;
    if(entity_ptr != PNULL)
    {
        switch(entity_ptr->audio.audio_info.info.type)
        {
        case MMISRVAUD_TYPE_RING_FILE:
        case MMISRVAUD_TYPE_RING_BUF:
        case MMISRVAUD_TYPE_STREAMING:
        //case MMISRVAUD_TYPE_TONE_DTMF:
        //case MMISRVAUD_TYPE_TONE_GENERIC:
        //case MMISRVAUD_TYPE_TONE_CUSTOM:
        case MMISRVAUD_TYPE_VOICE:
        case MMISRVAUD_TYPE_VOICE_BTCALL:
		case MMISRVAUD_TYPE_BT_MUSIC:
        case MMISRVAUD_TYPE_VOICE_LOOPBACK:
        case MMISRVAUD_TYPE_FM:
        case MMISRVAUD_TYPE_VIDEO:
        case MMISRVAUD_TYPE_ATV:
        case MMISRVAUD_TYPE_MV:
        case MMISRVAUD_TYPE_VIRTUAL:
#ifdef VIRTUAL_VIBRATE_FUNC
        case MMISRVAUD_TYPE_VIBERATE: // 2013 0902
#endif		
            res = TRUE;
            break;
#ifdef VIDEO_WP_SUPPORT            
        case MMISRVAUD_TYPE_VIDEO_WP:
            if(!IsVwpCoexist())
            {
                res = TRUE;
            }
            break;
#endif            
        default:
            break;
        }
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsNeedSetRoute(), entity_ptr=0x%x,type=%d,res=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1372_112_2_18_3_23_46_125,(uint8*)"ddd", entity_ptr, entity_ptr->audio.audio_info.info.type, res));
    }
    else
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsNeedSetRoute(), entity_ptr=NULL, Error!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1376_112_2_18_3_23_46_126,(uint8*)""));
    }
    
    return res;
}

/*****************************************************************************/
//  Description : To make a judgement if the current type support BT.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:Some plateform don't support BT in order to reduce the code size, e.g 6530. 
/*****************************************************************************/
LOCAL BOOLEAN IsTypeSupportBt(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    BOOLEAN res = TRUE;
    MMISRVAUD_RING_FMT_E fmt = 0;
    
    if(entity_ptr != PNULL)
    {    
        switch(entity_ptr->audio.audio_info.info.type)
        {
#ifndef AMR_A2DP_SUPPORT
        case MMISRVAUD_TYPE_VIDEO:
        case MMISRVAUD_TYPE_VIDEO_WP:
            res = FALSE;
            break;
        case MMISRVAUD_TYPE_RING_BUF:
            fmt = entity_ptr->audio.audio_info.info.ring_buf.fmt;
            if(MMISRVAUD_RING_FMT_AMR == fmt)
            {
                res = FALSE;
            }
            break;
        case MMISRVAUD_TYPE_RING_FILE:
            fmt = entity_ptr->audio.audio_info.info.ring_file.fmt;
            
            if(MMISRVAUD_RING_FMT_AMR == fmt)
            {
                res = FALSE;
            }
            break;
        case MMISRVAUD_TYPE_STREAMING:
            fmt = entity_ptr->audio.audio_info.info.streaming.fmt;
            if(MMISRVAUD_RING_FMT_AMR == fmt)
            {
                res = FALSE;
            }
            break;
#else
#ifdef  ADULT_WATCH_SUPPORT  //fix bug1595770
        case MMISRVAUD_TYPE_RING_BUF:
        case MMISRVAUD_TYPE_RING_FILE:
        case MMISRVAUD_TYPE_STREAMING:
        {
            if(FALSE == MMISRVAUD_BtIsA2dp()) //fix bug1595770
            {
                res = FALSE;
            }
            break;
        }
#endif
#endif
        default:
            break;
        }
                _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsTypeSupportBt() exit, type=%d, fmt=%d, res=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1427_112_2_18_3_23_46_127,(uint8*)"ddd",entity_ptr->audio.audio_info.info.type, fmt, res));
    }

    return res;
}

/*****************************************************************************/
//  Description : Get the route that it should be by all_support_route, default_route, and manual route. 
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL MMISRVAUD_ROUTE_T GetShouldBeRoute(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;

    
    if(PNULL == entity_ptr)
    {
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetShouldBeRoute() exit, entity_ptr is NULL!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1444_112_2_18_3_23_46_128,(uint8*)""));
        return MMISRVAUD_ROUTE_NONE;
    }

        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetShouldBeRoute() entry, entity_ptr=0x%x, all_support_route=%d, manual_route=%d, default_route=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1449_112_2_18_3_23_46_129,(uint8*)"dddd",entity_ptr, entity_ptr->audio.audio_info.all_support_route, entity_ptr->audio.manual_route, entity_ptr->audio.audio_info.default_route));

    if(MMISRVAUD_TYPE_VIRTUAL == entity_ptr->audio.audio_info.info.type)
    {
#ifdef VT_SUPPORT
	if(MMIAPIVT_IsVtCalling()||MMIAPIVT_IsVtConnecting()||MMIAPIVT_IsVtMoCalling())
	{
		if(IsTypeSupportBt(entity_ptr) && MMISRVAUD_IsBTHeadSetActive())
		{
			route = MMISRVAUD_ROUTE_BLUETOOTH;
            	}
		else
		{
			route = MMISRVAUD_ROUTE_SPEAKER;
		}
		SCI_TRACE_LOW("GetShouldBeRoute  VT_CALLING route=%d", route);
		return route;

	}
	else
#endif
	{
	        route = MMISRVAUD_ROUTE_SPEAKER;
	}
    }

    else if(entity_ptr->audio.manual_route != 0)
    {
        route = entity_ptr->audio.manual_route;
    }
    else
    {
        if(MMISRVAUD_ROUTE_AUTO == entity_ptr->audio.audio_info.all_support_route)
        {
            if(IsTypeSupportBt(entity_ptr) && MMISRVAUD_IsBTHeadSetActive())
            {
                route = MMISRVAUD_ROUTE_BLUETOOTH;
            }            
            else if(MMISRVAUD_IsHeadSetPlugIn())
            {
                route = MMISRVAUD_ROUTE_EARPHONE;
#ifdef VIRTUAL_VIBRATE_FUNC
                if(entity_ptr->audio.audio_info.info.type==MMISRVAUD_TYPE_VIBERATE)//MMISRVAUD_SearchHandleByType(MMISRVAUD_TYPE_VIBERATE) != 0)
                {
                    route = MMISRVAUD_ROUTE_SPEAKER;
                }
#endif
            }
            else
            {
                route = MMISRVAUD_ROUTE_SPEAKER;
            }
        }
        else
        {
            /*if default route is not 0, we set to default directly */
            if(entity_ptr->audio.audio_info.default_route != 0)
            {
                route = entity_ptr->audio.audio_info.default_route;
            }
            /*  But, if defautl route is 0, we look for the route from low bit to high bit by all_support_route */
            else
            {
                uint32 i = 1;
                uint32 val = entity_ptr->audio.audio_info.all_support_route;
                while( i <= val)
                {
                    if(val & i)
                    {
                        switch(i)
                        {
                        case MMISRVAUD_ROUTE_BLUETOOTH:
                            if(IsTypeSupportBt(entity_ptr) && MMISRVAUD_IsBTHeadSetActive())
                            {
                                route = MMISRVAUD_ROUTE_BLUETOOTH;
                            }
                            break;
                        case MMISRVAUD_ROUTE_EARPHONE:
                            if(MMISRVAUD_IsHeadSetPlugIn())
                            {
                                route = MMISRVAUD_ROUTE_EARPHONE;
#ifdef VIRTUAL_VIBRATE_FUNC
                                if(entity_ptr->audio.audio_info.info.type==MMISRVAUD_TYPE_VIBERATE)//MMISRVAUD_SearchHandleByType(MMISRVAUD_TYPE_VIBERATE) != 0)
		                   {
                                    route = MMISRVAUD_ROUTE_SPEAKER;
		                   }
#endif
                            }
                            break;
                        case MMISRVAUD_ROUTE_EARFREE:
                            if(MMISRVAUD_IsHeadSetPlugIn())
                            {
                                route = MMISRVAUD_ROUTE_EARFREE;
#ifdef VIRTUAL_VIBRATE_FUNC
                                if(entity_ptr->audio.audio_info.info.type==MMISRVAUD_TYPE_VIBERATE)//MMISRVAUD_SearchHandleByType(MMISRVAUD_TYPE_VIBERATE) != 0)
                                {
                                    route = MMISRVAUD_ROUTE_EARFREE;
                                }
#endif
                            }        
                            break;
                        case MMISRVAUD_ROUTE_SPEAKER:
                        case MMISRVAUD_ROUTE_HANDHOLD:
                        case MMISRVAUD_ROUTE_TVOUT:
                            route = i;
                            break;
                        case MMISRVAUD_ROUTE_BLUEPHONE:
                            if(MMISRVAUD_IsBTHeadSetActive())
                            {
                                route = MMISRVAUD_ROUTE_BLUETOOTH;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    if(route != 0)
                    {
                        break;
                    }
                    i = (i << 1);  
                }
            }
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio GetShouldBeRoute() exit, the route should be %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1538_112_2_18_3_23_46_130,(uint8*)"d", route));
    
    return route;
}

/*****************************************************************************/
//  Description : Set audio route and eq.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetRouteWithEq(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_ROUTE_T route)
{
    MMISRVAUD_ROUTE_T old_route = MMISRVAUD_ROUTE_NONE;
    MMISRVAUD_EQ_MODE_E eq = 0;

    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio SetRouteWithEq() entry, entity_ptr=0x%x,route=%d", entity_ptr,route));
    
    if(PNULL == entity_ptr)
    {
        return;
    }
    eq = DRVAUD_GetEQMode(entity_ptr->drv_handle);

    if(route != MMISRVAUD_ROUTE_NONE)
    {
        if(0 == entity_ptr->audio.cur_route)
        {
            DRVAUD_GetRoute(0, &old_route);
            if(old_route != route)
            {
                if(DRVAUD_SetRoute(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, route))
                {
                    DRVAUD_SetEQMode(entity_ptr->drv_handle, entity_ptr->audio.audio_info.eq_mode);
                    entity_ptr->audio.cur_route = route;
                }
            }
            else
            {
                entity_ptr->audio.cur_route = route;
            }
        }
        else
        {
            if(DRVAUD_SetRoute(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, route))
            {
                if((route != entity_ptr->audio.cur_route || eq != entity_ptr->audio.audio_info.eq_mode ) && IsNeedSetEQMode(entity_ptr))
                {                
                    DRVAUD_SetEQMode(entity_ptr->drv_handle, entity_ptr->audio.audio_info.eq_mode);
                }
                entity_ptr->audio.cur_route = route;            
            }            
        }
    }
}

/*****************************************************************************/
//  Description : Set audio route by all_support_route, default_route, and manual route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetRoute(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;

    if(PNULL == entity_ptr)
    {
        return;
    }
    route = GetShouldBeRoute(entity_ptr);

    if(route != MMISRVAUD_ROUTE_NONE)
    {
        SetRouteWithEq(entity_ptr, route);
    }

    /* do not change */
}


/*****************************************************************************/
//  Description : Set audio route by all_support_route, default_route, and manual route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SwichRoute(uint32 data, uint32 param)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = 0;
    MMISRVAUD_ROUTE_T old_route = 0;
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SwichRoute() entry, data=0x%x, s_bt_info.state=%d, is async=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1576_112_2_18_3_23_46_131,(uint8*)"ddd", data, s_bt_info.state, IS_ASYNC_MODE));

    entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;

    if(0 == data)
    {
        return;
    }
    if(IsNeedSetRoute(entity_ptr))
    {
        old_route = entity_ptr->audio.cur_route;
        //SetRoute(entity_ptr);
        /* replaced by the following code */
        {
            route = GetShouldBeRoute(entity_ptr);

            if(MMISRVAUD_ROUTE_BLUETOOTH == route)
            {
                if(IS_SRV_ACTIVE(entity_ptr) && IS_AUD_PLAY(entity_ptr))
                {
                    SetRouteWithEq(entity_ptr, route);
                }
                else
                {
                    return;
                }
            }
            else if(route != MMISRVAUD_ROUTE_NONE)
            {
                SetRouteWithEq(entity_ptr, route);
            }        
        }
#if 1        
        if(IS_SRV_ACTIVE(entity_ptr) && IS_AUD_PLAY(entity_ptr))
        {
            if(entity_ptr->audio.cur_route & MMISRVAUD_ROUTE_BLUETOOTH)
            {
                if(!IS_ASYNC_MODE)
                {
                    MMISRVAUD_EVENT_T event_data = {0};
                    SET_ASYNC_MODE(TRUE);
                    BT_SET_STATE(BT_STATE_OPENED);
                    event_data.event = MMISRVAUD_EVENT_PLAY;                    
                    BT_SET_CUR((uint32)entity_ptr, event_data, BtGetSampleRate(entity_ptr));
                }
            }
            else if(old_route & MMISRVAUD_ROUTE_BLUETOOTH)
            {
                if(IS_ASYNC_MODE)
                {
                    SET_ASYNC_MODE(FALSE);
                }
                if(BT_STATE_OPENED == BT_GET_STATE)
                {
                    /* we don't start the timer to close BT now, and close it when we stop it or it play end */
                    //BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
                    //BT_START_TIMER;
                }          
            }
        }
#else
        /* do nothing, the BT state should be processed after the current audio stop/pause/finedshed.*/
#endif
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SwichRoute() exit,old route=%d, new route=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1614_112_2_18_3_23_46_132,(uint8*)"dd", old_route, entity_ptr->audio.cur_route));
    
}

/*****************************************************************************/
//  Description : Set audio route by user appointed route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetAppointRoute(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_ROUTE_T route)
{
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }

    _DBG(MMISRV_TRACE_LOW("[MMISRV]: SetAppointRoute entry, IS_ASYNC_MODE=%d, bt state=%d, route=%d", IS_ASYNC_MODE, BT_GET_STATE, route));
    
    if(IS_SRV_ACTIVE(entity_ptr) || IS_SRV_STANDING(entity_ptr))
    {
        MMISRVAUD_ROUTE_T cur_route = 0;
        MMISRVAUD_ROUTE_T old_route = 0;
        
        DRVAUD_GetRoute(0, &cur_route);
        if(entity_ptr->audio.cur_route != route || cur_route != route)
        {
            if(MMISRVAUD_ROUTE_BLUETOOTH == route && !IS_ASYNC_MODE && BT_STATE_NONE == BT_GET_STATE)
            {
                MMISRVAUD_BtOpen(&entity_ptr->audio.audio_info.info, BtGetSampleRate(entity_ptr));
            }
            else
            {
                old_route = entity_ptr->audio.cur_route;
                SetRouteWithEq(entity_ptr, route);

                if(entity_ptr->audio.cur_route & MMISRVAUD_ROUTE_BLUETOOTH)
                {
                    if(!IS_ASYNC_MODE)
                    {
                        MMISRVAUD_EVENT_T event_data = {0};
                        SET_ASYNC_MODE(TRUE);
                        BT_SET_STATE(BT_STATE_OPENED);
                        event_data.event = MMISRVAUD_EVENT_PLAY;                    
                        BT_SET_CUR((uint32)entity_ptr, event_data, BtGetSampleRate(entity_ptr));
                    }
                }
                else if(old_route & MMISRVAUD_ROUTE_BLUETOOTH)
                {
                    if(IS_ASYNC_MODE)
                    {
                        SET_ASYNC_MODE(FALSE);
                    }
                    if(BT_STATE_OPENED == BT_GET_STATE)
                    {
                        /* we don't start the timer to close BT now, and close it when we stop it or it play end */
                        //BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
                        //BT_START_TIMER;
                    }          
                }

            }
        }
    }
    entity_ptr->audio.manual_route = route;
    return TRUE;
}

/*****************************************************************************/
//  Description : Set audio all support route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetAllSupportRoute(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_ROUTE_T all_support_route)
{
    BOOLEAN result = FALSE;
    MMISRVAUD_ROUTE_T route = 0;
    if(IsNeedSetRoute(entity_ptr))
    {
        entity_ptr->audio.audio_info.all_support_route = all_support_route;
        route = GetShouldBeRoute(entity_ptr);
        if(IS_SRV_ACTIVE(entity_ptr) && IS_AUD_PLAY(entity_ptr))
        {
            if(MMISRVAUD_ROUTE_BLUETOOTH == route && !IS_ASYNC_MODE && BT_STATE_NONE == BT_GET_STATE)
            {
                MMISRVAUD_BtOpen(&entity_ptr->audio.audio_info.info, BtGetSampleRate(entity_ptr));
            }
            else
            {
                SwichRoute((uint32)entity_ptr, 0);
            }
        }
        result = TRUE;
    }
    return result;
}

/*****************************************************************************/
//  Description : Set audio manual_route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN SetManualRoute(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_ROUTE_T manual_route)
{
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }
	
    entity_ptr->audio.manual_route = manual_route;
	
    return TRUE;
}
#if 0
/*****************************************************************************/
//  Description : Set audio route by all_support_route, default_route, and manual route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used as just time that BT active.
/*****************************************************************************/
LOCAL void BtSetRoute(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    MMISRVAUD_ROUTE_T route = MMISRVAUD_ROUTE_NONE;

    if(PNULL == entity_ptr)
    {
        return;
    }
    route = GetShouldBeRoute(entity_ptr);

    if(route != MMISRVAUD_ROUTE_NONE)
    {
        if(DRVAUD_SetRoute(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, route))
        {
            entity_ptr->audio.cur_route = route;
        }

        if(route & MMISRVAUD_ROUTE_BLUETOOTH)
        {
            if(IS_SRV_ACTIVE(entity_ptr) && IS_AUD_PLAY())
            {
                if(!IS_ASYNC_MODE)
                {
                    SET_ASYNC_MODE(TRUE);
                    BT_SET_STATE(BT_STATE_OPENED);
                }
            }
        }
    }
}

/*****************************************************************************/
//  Description : Set audio route by all_support_route, default_route, and manual route.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used as just time that BT active.
/*****************************************************************************/
LOCAL void BTSwichRoute(uint32 data, uint32 param)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = 0;
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SwichRoute() entry, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1663_112_2_18_3_23_46_133,(uint8*)"d", data));
    
    if(IsNeedSetRoute(entity_ptr))
    {
        BtSetRoute(entity_ptr);
    }
}
#endif

/*****************************************************************************/
//  Description : Set audio volume by volume type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetVolumeByType(MMISRVAUD_VOLUME_TYPE_E type, MMISRVAUD_VOLUME_T volume)
{
    uint32 i = 0;
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    uint32 num = 0;
    BOOLEAN flag = FALSE;

    /* if this type is need not update, return immediately */
    switch(type)
    {
    /* Media type is need update */
    case MMISRVAUD_VOLUME_TYPE_MED:
        break;
    /* Other type is not update now */
    default:
        return FALSE;
    }

    while(i < MMISRVAUD_SERVICE_NUM_MAX && num < s_handle_box.num)
    {
        if(s_handle_box.entity_addr[i] != 0)
        {
            entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_handle_box.entity_addr[i];
            if(entity_ptr != PNULL)
            {
                if(type == entity_ptr->audio.audio_info.volume_type)
                {
                    entity_ptr->audio.audio_info.volume = volume;
                }
                if(!flag)
                {
                    if(IS_AUD_PLAY(entity_ptr) && !IS_SRV_SUSPEND(entity_ptr))
                    {
                        DRVAUD_SetVolume(entity_ptr->drv_handle, volume);       
                        flag = TRUE;
                    }
                }
            }
            num++;
        }
        i++;
    }
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Set audio service's parameters
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SetAudParam(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    if(entity_ptr != PNULL)
    {
        if(IsNeedSetRate(entity_ptr))
        {
            DRVAUD_SetPlayRate(entity_ptr->drv_handle, entity_ptr->audio.audio_info.rate);
        }
        if(IsNeedSetVolume(entity_ptr))
        {
            DRVAUD_SetVolume(entity_ptr->drv_handle, entity_ptr->audio.audio_info.volume);
        }
        if(IsNeedSetRoute(entity_ptr))
        {
            SetRoute(entity_ptr);
        }

#ifdef VIDEO_WP_SUPPORT
        if(IsVwpCoexist())
        {
            VwpSetSound(FALSE);
        }
        else if(MMISRVAUD_SearchHandleByType(MMISRVAUD_TYPE_VIDEO_WP) != 0)
        {
            VwpSetSound(TRUE);
        }
#endif
        
//        if(DRVAUD_GetRoute(entity_ptr->drv_handle, &route))
//        {
//            if(IsNeedSetRoute(entity_ptr))
//            {
//                MMISRVAUD_EQ_MODE_E eq = DRVAUD_GetEQMode(entity_ptr->drv_handle);
//                SetRoute(entity_ptr);
//                if((route != entity_ptr->audio.cur_route || eq != entity_ptr->audio.audio_info.eq_mode ) && IsNeedSetEQMode(entity_ptr))
//                {                
//                    DRVAUD_SetEQMode(entity_ptr->drv_handle, entity_ptr->audio.audio_info.eq_mode);
//                }
//            }
//        }
    }

}

/*****************************************************************************/
//  Description : Pause audio when switch service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void PauseAudio(uint32 data)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio PauseAudio() entry, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1705_112_2_18_3_23_47_134,(uint8*)"d", data));

    if(data > 0)
    {
        SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;
        
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio PauseAudio(), drv_handle=0x%x, audio_state=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1711_112_2_18_3_23_47_135,(uint8*)"dd", entity_ptr->drv_handle, entity_ptr->audio.audio_state));
        
        if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
        {
            AudPause(entity_ptr, PNULL, FALSE);
        }
        /* If the audio has start a timer, we don't stop the timer.
            if it's required in some situation, we do it in that time.
        */
    }
}

/*****************************************************************************/
//  Description : Pause audio when switch service and notify client.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void PauseAudioAndNotify(uint32 data, uint32 param)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio PauseAudioAndNotify() entry, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1729_112_2_18_3_23_47_136,(uint8*)"d", data));

    if(data > 0)
    {
        SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;
        
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio PauseAudioAndNotify(), drv_handle=0x%x, audio_state=%d, is_auto_free=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1735_112_2_18_3_23_47_137,(uint8*)"ddd", entity_ptr->drv_handle, entity_ptr->audio.audio_state, entity_ptr->service.is_auto_free));

        /* If the service is auto free, we stop it and free it directly */
        if(entity_ptr->service.is_auto_free)
        {
            AudStop(entity_ptr, PNULL);
            DRVAUD_CloseHandle(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
            //DeleteFromHandleBoxByEntityAddr((uint32)entity_ptr);
            MMISRVAUD_LinkGetAndDelete(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);

#ifndef VIRTUAL_VIBRATE_FUNC
            /* if is vibrate, we call the vibrate call back */
            if(MMISRVAUD_TYPE_VIBERATE == entity_ptr->audio.audio_info.info.type)
            {
                MMISRVAUD_VIBRATE_CB cb = entity_ptr->audio.audio_info.info.vibrate.cb;
                _DBG(                //MMISRV_TRACE_LOW:"[MMISRV]: Audio PauseAudioAndNotify(), vibrate cb=0x%x"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1749_112_2_18_3_23_47_138,(uint8*)"d", cb));
                if(cb != PNULL)
                {
                    cb();
                }
            }
            /* else we notify client that it is ended successfully */
            else
#endif
            {
                SrvNotifyClientExt(entity_ptr, MMISRVAUD_REPORT_END, (uint32)MMISRVAUD_REPORT_RESULT_SUCESS);
            }

            if(DeleteFromHandleBoxByEntityAddr((uint32)entity_ptr))
            {
                SCI_FREE(entity_ptr);
            }
            
        }
        /* If the service is not auto free, we suspend it and notify client */
        else
        {
            if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
            {
                AudPause(entity_ptr, PNULL, FALSE);
            }
            entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_BEEN_SUSPENDED;
            SrvNotifyClient(entity_ptr, MMISRVMGR_NOTIFY_SUSPEND, 0);
        }
    }
}

/*****************************************************************************/
//  Description : resume audio when switch service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void ResumeAudio(uint32 data)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio ResumeAudio() entry, data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1788_112_2_18_3_23_47_139,(uint8*)"d", data));

    if(data > 0)
    {
        SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;
        
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio ResumeAudio(), drv_handle=0x%x, audio_state=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1794_112_2_18_3_23_47_140,(uint8*)"dd", entity_ptr->drv_handle, entity_ptr->audio.audio_state));
        
        //SetAudParam(entity_ptr);
        if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
        {
            AudResume(entity_ptr, PNULL);
        }
    }
}

/*****************************************************************************/
//  Description : resume audio when switch service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void ResumeAudioAndNotify(uint32 data, uint32 param)
{
    if(data > 0)
    {
        SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;
        entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_ACTIVE;
        ResumeAudio(data);
        SrvNotifyClient(entity_ptr, MMISRVMGR_NOTIFY_RESUME, 0);
    }
}

/*****************************************************************************/
//  Description : Create a new audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void SwitchActiveSrv(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
#ifdef VIDEO_WP_SUPPORT
    SRVAUD_ENTITY_INFO_T *vwp_ptr = PNULL;
#endif    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvActiveSwitch() entry, entity=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1825_112_2_18_3_23_47_141,(uint8*)"d", entity_ptr));

    if(PNULL == entity_ptr)
    {
        return;
    }

#ifdef VIDEO_WP_SUPPORT
    vwp_ptr = GetVwpPtr(entity_ptr);
#endif    
    /* pause all the service that is in playing state */
    MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, PauseAudioAndNotify, 0);

    /* push current active service to stack */
    MMISRVAUD_StackPush();

    /* Add the new service to active link */
    MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);

#ifdef VIDEO_WP_SUPPORT
    if(vwp_ptr != PNULL)
    {
        MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)vwp_ptr);
    }
#endif    


    /* reset the active service */
    s_mmisrv_aud_statues.active_entity = entity_ptr;
    entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_ACTIVE;
}

/*****************************************************************************/
//  Description : Set audio Info if need.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SetAudioInfo(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_TYPE_U *type_info)
{
    BOOLEAN res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SetAudioInfo() entry, entity_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1854_112_2_18_3_23_47_142,(uint8*)"d", entity_ptr));
    
    if(PNULL == entity_ptr || PNULL == type_info)
    {
        return FALSE;
    }

        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SetAudioInfo(), old type=%d, new type=%d, service state=%d, audio state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1862_112_2_18_3_23_47_143,(uint8*)"dddd", entity_ptr->audio.audio_info.info.type, type_info->type, entity_ptr->service.srv_state, entity_ptr->audio.audio_state));
    
    if(entity_ptr->audio.audio_info.info.type != MMISRVAUD_TYPE_VIRTUAL 
        && entity_ptr->audio.audio_info.info.type != type_info->type)
    {
        return FALSE;
    }
    if(entity_ptr->service.srv_state != MMISRVMGR_SERVICE_STATE_ACTIVE 
        && entity_ptr->service.srv_state != MMISRVMGR_SERVICE_STATE_STANDING
		&& entity_ptr->service.srv_state != MMISRVMGR_SERVICE_STATE_BEEN_SUSPENDED
    )
    {
        return FALSE;
    }

    if(entity_ptr->audio.audio_state != AUDIO_STATE_STOP)
    {
        return FALSE;
    }
    
    /* Don't need this condition */
    /*
    if(0 == entity_ptr->drv_handle)
    {
        return FALSE;
    }
    */
    switch(entity_ptr->audio.audio_info.info.type)
    {
    case MMISRVAUD_TYPE_VIRTUAL:    
    case MMISRVAUD_TYPE_RING_FILE:
    case MMISRVAUD_TYPE_RING_BUF:
    case MMISRVAUD_TYPE_STREAMING:
    //case MMISRVAUD_TYPE_RECORD_FILE:
    //case MMISRVAUD_TYPE_RECORD_BUF:
    //case MMISRVAUD_TYPE_VOICE:
    //case MMISRVAUD_TYPE_FM:
    case MMISRVAUD_TYPE_ATV:
#ifndef VIRTUAL_VIBRATE_FUNC
    case MMISRVAUD_TYPE_VIBERATE:
#endif
    case MMISRVAUD_TYPE_VIDEO:
    case MMISRVAUD_TYPE_VIDEO_WP:
    {
        DRVAUD_HANDLE handle = 0;

        /* Don't needed now for the reason of BT */
        #if 0
        if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
        {
            DRVAUD_Stop(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
        }
        #endif
        DRVAUD_CloseHandle(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
        entity_ptr->drv_handle = 0;
        handle = DRVAUD_CreateHandle(type_info, FALSE, (uint32 *)&VBDecodeOuputBuf, DECODE_OUTPUT_BUF_SIZE);
        if(handle > 0)
        {
            entity_ptr->drv_handle = handle;
            SCI_MEMCPY(&entity_ptr->audio.audio_info.info, type_info, sizeof(MMISRVAUD_TYPE_U));
            
            /* To be maked sure if need set audio param at this time */
            SetAudParam(entity_ptr);
            
            /* Don't needed now for the reason of BT */
            #if 0
            if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
            {
                DRVAUD_Play(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, 0);
            }
            #endif
            res = TRUE;
        }
        break;
    }
#ifdef VIRTUAL_VIBRATE_FUNC
    case MMISRVAUD_TYPE_VIBERATE:
{
        DRVAUD_HANDLE handle = 0;

        /* Don't needed now for the reason of BT */
        #if 0
        if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
        {
            DRVAUD_Stop(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
        }
        #endif
        DRVAUD_CloseHandle(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
        entity_ptr->drv_handle = 0;
        handle = DRVAUD_CreateHandle(type_info, FALSE, (uint32 *)&VBDecode_VibrateOuputBuf, DECODE_OUTPUT_BUF_SIZE);
        if(handle > 0)
        {
            entity_ptr->drv_handle = handle;
            SCI_MEMCPY(&entity_ptr->audio.audio_info.info, type_info, sizeof(MMISRVAUD_TYPE_U));
            
            /* To be maked sure if need set audio param at this time */
            SetAudParam(entity_ptr);
            
            /* Don't needed now for the reason of BT */
            #if 0
            if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
            {
                DRVAUD_Play(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, 0);
            }
            #endif
            res = TRUE;
        }
        break;
    }
#endif
    case MMISRVAUD_TYPE_TONE_DTMF:
    case MMISRVAUD_TYPE_TONE_GENERIC:
    case MMISRVAUD_TYPE_TONE_CUSTOM:
        SCI_MEMCPY(&entity_ptr->audio.audio_info.info, type_info, sizeof(MMISRVAUD_TYPE_U));
        res = TRUE;
        break;
    default:
        break;    
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SetAudioInfo() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1943_112_2_18_3_23_47_144,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Handle audio timer end.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void HandleAudioTimer(uint8 timer, uint32 param)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    MMISRVAUD_REPORT_T rpt_data = {0};
    
    entity_ptr = GetEntityByTimer(timer);
    if(entity_ptr != PNULL)
    {
        rpt_data.report = MMISRVAUD_REPORT_TIMER_END;
        rpt_data.data1 = MMISRVAUD_REPORT_RESULT_SUCESS;
        MMISRVAUD_HandleAudioReport(entity_ptr->drv_handle, &rpt_data);
        entity_ptr->audio.timer = 0;
    }
}

LOCAL void HandleAudioIntervalTimer(uint8 timer, uint32 param)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)param;
    MMISRVAUD_REPORT_T rpt_data = {0};
    
    if(entity_ptr != PNULL)
    {
        rpt_data.report = MMISRVAUD_REPORT_INTERVAL_TIMER_END;
        MMISRVAUD_HandleAudioReport(entity_ptr->drv_handle, &rpt_data);
        entity_ptr->audio.interval_timer = 0;
    }
}

/*****************************************************************************/
//  Description : Resume service only, whitch means don't resume the audio.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvResumeOnly(IMMISRV_UNKNOWN_T *srv_ptr)
{
    /* 
        Please attention, the service to be resumed maybe co-exit with the active service, 
    but now we don't do co-exist, we'll do that later if needed.
       If do so, IsCanResume() function should return the work mode, and we do resume by
    the work mode.
    */
    
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    BOOLEAN res = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvResumeOnly() entry, srv_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_1983_112_2_18_3_23_47_145,(uint8*)"d", srv_ptr));
    
    if(PNULL == srv_ptr)
    {
        return FALSE;
    }
        
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)srv_ptr;

    /* if this service is suspended manually, we get it from suspend link and resume it */
    if(MMISRVMGR_SERVICE_STATE_SUSPEND == entity_ptr->service.srv_state)
    {
        if(IsCanResume(entity_ptr))
        {
            if(MMISRVAUD_LinkGetAndDelete(SRVAUD_LINK_SUSPEND, (uint32)entity_ptr) > 0)
            {
                res = MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);
            }
            else
            {
                res = FALSE;
            }
        }
    }
    
    /* else, if this service is suspended automaticlly, we resume it from stack */
    else if(MMISRVMGR_SERVICE_STATE_BEEN_SUSPENDED == entity_ptr->service.srv_state)
    {
        if(IsCanResume(entity_ptr))
        {
            if(MMISRVAUD_StackGetAndDelete((uint32)entity_ptr))
            {
#ifdef VIDEO_WP_SUPPORT            
                SRVAUD_ENTITY_INFO_T *vwp_ptr = PNULL;
                vwp_ptr = GetVwpPtr(entity_ptr);
#endif             
                /* Pause all the service that is in playing state */
                MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, PauseAudioAndNotify, 0);
                /* Push current active to stack */
                MMISRVAUD_StackPush();
                /* Put the service to active link */
                MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);
                entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_ACTIVE;

#ifdef VIDEO_WP_SUPPORT            
                if(vwp_ptr != PNULL)
                {
                    MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)vwp_ptr);
                }
#endif                
                res = TRUE;
            }
        }
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvResumeOnly() exit, resume res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2026_112_2_18_3_23_47_146,(uint8*)"d", res));

    return res;
}

/*****************************************************************************/
//  Description : Create a new audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T SrvCreate(MMISRVMGR_SERVICE_REQ_T *req_data, uint32 ext_data)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    DRVAUD_HANDLE drv_handle = 0;
    SRV_CREATE_MODE_E mode = SRV_CREATE_MODE_NONE;
    //BOOLEAN is_a2dp = FALSE;
    uint32 num = 0;
    MMISRVAUD_TYPE_T *type_ptr = PNULL;
    uint32 handle_gen = 0;
    
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio SrvCreate() entry, req_data=0x%x, ext_data=0x%x,total=%d",req_data, ext_data, s_handle_box.num));

    if(req_data == PNULL)
    {
        return 0;
    }

    if(s_handle_box.num >= MMISRVAUD_SERVICE_NUM_MAX)
    {
        return 0;
    }
    LOCK_SERVICE;
    num = MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE);    
    type_ptr = (MMISRVAUD_TYPE_T*)ext_data;    
    if(type_ptr->is_standing)
    {   
        /* if active service are all not in play state, we'll play it */
        if(num > 0 && IsActiveAudPlaying())
        {   
            UNLOCK_SERVICE;
            return 0;
        }
        else
        {
            mode = SRV_CREATE_MODE_STANDING;
        }
    }
    else
    {
        if(num > 0)
        {
            SRVAUD_ENTITY_INFO_T *head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);

            /* if active service's priority is 0, free it unconditionly */        
            if(0 == head_ptr->service.pri)
            {
                SrvFree((IMMISRV_UNKNOWN_T *)head_ptr);
            }
            /* else if request service's priority is 0, return 0 handle unconditionly */
            else if(0 == req_data->pri)
            {
                UNLOCK_SERVICE;
                return 0;
            }
        }
        mode = GetCreateMode(req_data, &type_ptr->info, type_ptr->is_mixing_enable);
    }


    if(SRV_CREATE_MODE_NONE == mode)
    {
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio SrvCreate(), Can't create new service!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2078_112_2_18_3_23_48_148,(uint8*)"");
        
        mode = GetCreateMode(req_data, &type_ptr->replace_info, type_ptr->is_mixing_enable);

        if(SRV_CREATE_MODE_NONE == mode)
        {
            //MMISRV_TRACE_ERR:"[MMISRV]: Audio SrvCreate(), Can't create new service with replace info!!!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2084_112_2_18_3_23_48_149,(uint8*)"");
            UNLOCK_SERVICE;
            return 0;
        }
    }

    //if(( type_ptr->route & MMISRVAUD_ROUTE_BLUETOOTH) != 0)
    //{
    //    is_a2dp = TRUE;
    //}
#ifdef VIRTUAL_VIBRATE_FUNC
  	 if(type_ptr->info.type ==MMISRVAUD_TYPE_VIBERATE )
   	{
    		drv_handle = DRVAUD_CreateHandle(&type_ptr->info, FALSE, (uint32 *)&VBDecode_VibrateOuputBuf, DECODE_OUTPUT_BUF_SIZE);
   	}
  	 else
	{
	      drv_handle = DRVAUD_CreateHandle(&type_ptr->info, FALSE, (uint32 *)&VBDecodeOuputBuf, DECODE_OUTPUT_BUF_SIZE);
	}

#else
	{
    		drv_handle = DRVAUD_CreateHandle(&type_ptr->info, FALSE, (uint32 *)&VBDecodeOuputBuf, DECODE_OUTPUT_BUF_SIZE);
   	}
#endif

    if(drv_handle == 0)
    {
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio SrvCreate(), driver create handle failed!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2097_112_2_18_3_23_48_150,(uint8*)"");
        UNLOCK_SERVICE;
        return 0;
    }

    entity_ptr = (SRVAUD_ENTITY_INFO_T *)SCI_ALLOC_APP(sizeof(SRVAUD_ENTITY_INFO_T));

    if(PNULL == entity_ptr)
    {
        DRVAUD_CloseHandle(drv_handle, &type_ptr->info);
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio SrvCreate(), memery malloc failed!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2106_112_2_18_3_23_48_151,(uint8*)"");
        UNLOCK_SERVICE;
        return 0;
    }

    SCI_MEMSET(entity_ptr, 0, sizeof(SRVAUD_ENTITY_INFO_T));

    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvCreate(), start to create a new service"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2112_112_2_18_3_23_48_152,(uint8*)"");
    
    /* we should save the service data at this time */
    if(req_data->client_name != PNULL)
    {
        //#define MIN(x, y) ((x < y)?x:y)
        uint32 len = strlen(req_data->client_name);
        if(len > MMISRVAUD_STR_CLIENT_NAME_LEN)
        {
            len = MMISRVAUD_STR_CLIENT_NAME_LEN;
        }
        strncpy(entity_ptr->client_name, req_data->client_name, len);
    }
    /* service */
    entity_ptr->service.pri = req_data->pri;
    entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_ACTIVE;
    entity_ptr->service.suspend_by_service = 0;
    entity_ptr->service.suspend_service = 0;
    entity_ptr->service.is_async = req_data->is_async;
    entity_ptr->service.is_auto_resume_off = req_data->is_auto_resume_off;
    entity_ptr->service.is_auto_free = req_data->is_auto_free;
    entity_ptr->service.notify = req_data->notify;
    entity_ptr->service.ind_data = req_data->ind_data;
    /* audio */
    SCI_MEMCPY(&entity_ptr->audio.audio_info, type_ptr, sizeof(MMISRVAUD_TYPE_T));
    entity_ptr->audio.audio_state = AUDIO_STATE_STOP;

    /* other */
    entity_ptr->drv_handle = drv_handle;
    entity_ptr->vtbl_ptr = (MMISRV_VTBL_T(IMMISRV_AUD_T) *)&s_mmisrv_aud_vtbl;

    handle_gen = GenAndAddHandleToBox((uint32)entity_ptr);
    if(handle_gen != 0)
    {
        entity_ptr->handle = handle_gen;

#if 0
        if(IsSrvTypeStanding(((MMISRVAUD_TYPE_T *)ext_data)->info.type))
        {
            MMISRVAUD_LinkPush(SRVAUD_LINK_STANDING, (uint32)entity_ptr);
            entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_STANDING;
        }
        else
        {
            /* switch active service */
            if(IsSrvNeedSwitch())
            {
                SwitchActiveSrv(entity_ptr);
            }
            /* else, Add it to active service link */
            else
            {
                 MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);
                 s_mmisrv_aud_statues.active_entity = entity_ptr;
            }
        }
#else
        switch(mode)
        {
        case SRV_CREATE_MODE_SINGLE:
            MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);
            
            /* if there is suspended service, we would resume it when the current service is free */
            if(MMISRVAUD_LinkGetNum(SRVAUD_LINK_SUSPEND) > 0)
            {
                /* record to the suspend _service to resume the suspended service */
                entity_ptr->service.suspend_service = MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_SUSPEND);
            }
            break;
        case SRV_CREATE_MODE_MIXING:
            MMISRVAUD_LinkAppend(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);
            break;
        case SRV_CREATE_MODE_COEXIST:
            MMISRVAUD_LinkAppend(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);
//#ifdef VIDEO_WP_SUPPORT
//            if(IsVwpCoexist())
//            {
//                VwpSetSound(FALSE);
//            }
//#endif                
            break;            
        case SRV_CREATE_MODE_SWITCH:
            SwitchActiveSrv(entity_ptr);
            break;
        case SRV_CREATE_MODE_STANDING:
            MMISRVAUD_LinkAppend(SRVAUD_LINK_STANDING, (uint32)entity_ptr);
            entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_STANDING;
            break;
        default:
            break;
        }
#endif

        /* Virutal type is a special type, it could not play, but it need set param immedaiatly sometime */
        if(MMISRVAUD_TYPE_VIRTUAL == entity_ptr->audio.audio_info.info.type)
        {
            /* stop standing audo firstly, because the route may be changed */
            StopStandingAud();            
            SetAudParam(entity_ptr);
        }
    }
    else
    {
        DRVAUD_CloseHandle(drv_handle, &entity_ptr->audio.audio_info.info);
        SCI_FREE(entity_ptr);
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio SrvCreate(), add to handle box failed!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2212_112_2_18_3_23_48_153,(uint8*)"");
    }

    UNLOCK_SERVICE;
    return (MMISRV_HANDLE_T)handle_gen;
}

/*****************************************************************************/
//  Description : Resume a audio service by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvResume(IMMISRV_UNKNOWN_T *srv_ptr)
{
#if 1
    SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)srv_ptr;
    BOOLEAN res = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvResume() entry, srv_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2228_112_2_18_3_23_48_154,(uint8*)"d", srv_ptr));
    
    if(PNULL == srv_ptr)
    {
        return FALSE;
    }

    LOCK_SERVICE;
    res = SrvResumeOnly(srv_ptr);
    if(res)
    {
        ResumeAudio((uint32)entity_ptr);    
    }
    UNLOCK_SERVICE;
    return res;
#else
    /* 
        Please attention, the service to be resumed maybe co-exit with the active service, 
    but now we don't do co-exist, we'll do that later if needed.
       If do so, IsCanResume() function should return the work mode, and we do resume by
    the work mode.
    */
    
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    BOOLEAN res = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvResume() entry, srv_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2252_112_2_18_3_23_48_155,(uint8*)"d", srv_ptr));
    
    if(PNULL == srv_ptr)
    {
        return FALSE;
    }
        
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)srv_ptr;

    /* if this service is suspended manually, we get it from suspend link and resume it */
    if(MMISRVMGR_SERVICE_STATE_SUSPEND == entity_ptr->service.srv_state)
    {
        if(IsCanResume(entity_ptr))
        {
            if(MMISRVAUD_LinkGetAndDelete(SRVAUD_LINK_SUSPEND, (uint32)entity_ptr) > 0)
            {
                res = MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);
                if(res)
                {
                    entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_ACTIVE;
                    ResumeAudio((uint32)entity_ptr);
                }
            }
            else
            {
                res = FALSE;
            }
        }
    }
    
    /* else, if this service is suspended automaticlly, we resume it from stack */
    else if(MMISRVMGR_SERVICE_STATE_BEEN_SUSPENDED == entity_ptr->service.srv_state)
    {
        if(IsCanResume(entity_ptr))
        {
            if(MMISRVAUD_StackGetAndDelete((uint32)entity_ptr))
            {
                /* Pause all the service that is in playing state */
                MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, PauseAudioAndNotify, 0);
                /* Push current active to stack */
                MMISRVAUD_StackPush();
                /* Put the service to active link */
                MMISRVAUD_LinkPush(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr);
                entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_ACTIVE;
                /* Resume audio if it's playing state */
                if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
                {
                    if(!DRVAUD_Resume(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info))
                    {
                        entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
                    }
                }
                res = TRUE;
            }
        }
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvResume() exit, resume res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2308_112_2_18_3_23_48_156,(uint8*)"d", res));

    return res;
#endif    
}

/*****************************************************************************/
//  Description : Suspend a audio service by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvSuspend(IMMISRV_UNKNOWN_T *srv_ptr)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    BOOLEAN res = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvSuspend() entry, srv_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2323_112_2_18_3_23_48_157,(uint8*)"d", srv_ptr));
    
    if(PNULL == srv_ptr)
    {
        return FALSE;
    }
        
    LOCK_SERVICE;
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)srv_ptr;

    /* Only active service can be suspend */
    if(MMISRVMGR_SERVICE_STATE_ACTIVE == entity_ptr->service.srv_state)
    {
        /* if audio is in play state, pause it firstly, but don't change the audio state. */
        PauseAudio((uint32)srv_ptr);

        /* Delete the service from acitve link */
        if(MMISRVAUD_LinkGetAndDelete(SRVAUD_LINK_ACTIVE, (uint32)entity_ptr))
        {
            /* push to suspend link */
            if(MMISRVAUD_LinkPush(SRVAUD_LINK_SUSPEND, (uint32)entity_ptr))
            {
                entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_SUSPEND;
                res = TRUE;
            }
            else
            {
                res = FALSE;
            }
        }
        /* if the service is not in active link, search from stack and delete it */
        else
        {
            if(MMISRVAUD_StackGetAndDelete((uint32)entity_ptr))
            {
                /* push to suspend link */
                if(MMISRVAUD_LinkPush(SRVAUD_LINK_SUSPEND, (uint32)entity_ptr))
                {
                    entity_ptr->service.srv_state = MMISRVMGR_SERVICE_STATE_SUSPEND;
                    res = TRUE;
                }
                else
                {
                    res = FALSE;
                }
            }
        }
    }   
    else
    {
        res = TRUE;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvSuspend() entry, entity_ptr->service.srv_state=%d, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2374_112_2_18_3_23_48_158,(uint8*)"dd", entity_ptr->service.srv_state, res));
    
    UNLOCK_SERVICE;
    return res;
}

/*****************************************************************************/
//  Description : Destroy a audio service by handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvFree(IMMISRV_UNKNOWN_T *srv_ptr)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    BOOLEAN result = FALSE;
    SRVAUD_LINK_E link_type = SRVAUD_LINK_ACTIVE;
    uint32 num = 0;
    uint32 suspend_srv = 0;
    uint32 is_auto_resume_off = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvFree() entry, srv_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2392_112_2_18_3_23_48_159,(uint8*)"d", srv_ptr));
    
    if(PNULL == srv_ptr)
    {
        return FALSE;
    }
        
    LOCK_SERVICE;
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)srv_ptr;

    switch(entity_ptr->service.srv_state)
    {
    case MMISRVMGR_SERVICE_STATE_ACTIVE:
        link_type = SRVAUD_LINK_ACTIVE;
        break;
    case MMISRVMGR_SERVICE_STATE_SUSPEND:
        link_type = SRVAUD_LINK_SUSPEND;
        break;
    case MMISRVMGR_SERVICE_STATE_READY:
        link_type = SRVAUD_LINK_READY;
        break;
    case MMISRVMGR_SERVICE_STATE_STANDING:
        link_type = SRVAUD_LINK_STANDING;
        break;
    default:
        break;
    }
	MMI_CheckAllocatedMemInfo();  //zhouyanxian bug112641
	
    AudStop(entity_ptr, PNULL);

	MMI_CheckAllocatedMemInfo();  //zhouyanxian bug112641
	
    DRVAUD_CloseHandle(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);

	MMI_CheckAllocatedMemInfo();  //zhouyanxian bug112641
    
    /* Delete from link or stack, and free memory! 
        if link type is not active, we do not suspend other service;
    */
    if(MMISRVAUD_LinkGetAndDelete(link_type, (uint32)entity_ptr))
    {
        if(SRVAUD_LINK_ACTIVE == link_type)
        {
            is_auto_resume_off = entity_ptr->service.is_auto_resume_off;
                
            _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvFree(), srv free Ok! is_auto_resume_off=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2429_112_2_18_3_23_48_160,(uint8*)"d", is_auto_resume_off));

            if(!is_auto_resume_off)
            {
                num = MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE);
                _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvFree(), suspend_srv=%d, active service num=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2434_112_2_18_3_23_48_161,(uint8*)"dd",suspend_srv, num));
                
                /* if no other service is in active state, we resume the top link of stack directly */
                if(0 == num)
                {
                    uint32 link_head = MMISRVAUD_StackPop();
                    
                    if(MMISRVAUD_LinkResume(link_head))
                    {
                        /* travel the active link, if some audio is in play state, resume the audio */
                        MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, ResumeAudioAndNotify, 0);
                    }
                }
            }
            if(DeleteFromHandleBoxByEntityAddr((uint32)entity_ptr))
            {
                SCI_FREE(entity_ptr);
            }
#ifdef VIDEO_WP_SUPPORT
            if(IsVwpCoexist())
            {
                VwpSetSound(FALSE);
            }
            else if(MMISRVAUD_SearchHandleByType(MMISRVAUD_TYPE_VIDEO_WP) != 0)
            {
                VwpSetSound(TRUE);
            }
#endif
            UNLOCK_SERVICE;
            return TRUE;            
        }
    }
    /* if delete from stack, Don't do any resume */
    else
    {
        if(MMISRVAUD_StackGetAndDelete((uint32)entity_ptr))
        {
            result = TRUE;
        }
    }
    
    if(DeleteFromHandleBoxByEntityAddr((uint32)entity_ptr))
    {
        SCI_FREE(entity_ptr);
    }
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio SrvFree() exit, result=%d", result));
    
    UNLOCK_SERVICE;
    return result;            
}

/*****************************************************************************/
//  Description : Set audio service priority.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvSetPri(IMMISRV_UNKNOWN_T *srv_ptr, uint16 pri)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvSetPri() entry, srv_ptr=0x%x,pri=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2493_112_2_18_3_23_48_164,(uint8*)"dd", srv_ptr,pri));
    
    if(PNULL == srv_ptr)
    {
        return FALSE;
    }
        
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)srv_ptr;

    entity_ptr->service.pri = pri;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Set audio service notify function.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvSetNotify(IMMISRV_UNKNOWN_T *srv_ptr, MMISRVMGR_NOTIFY_FUNC notify)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvSetNotify() entry, srv_ptr=0x%x, notify=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2515_112_2_18_3_23_48_165,(uint8*)"dd", srv_ptr, notify));
    
    if(PNULL == srv_ptr || PNULL == notify)
    {
        return FALSE;
    }
        
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)srv_ptr;

    entity_ptr->service.notify = notify;
    
    return TRUE;
}

/*****************************************************************************/
//  Description : Get active audio service handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GetActiveHandle(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_TYPE_T *audio_info)
{
    /* This function is not used now */
    /* To be done later */
    /*
    ---------------------
    ---------------------
    ---------------------
    ---------------------
    */
    return TRUE;
}

/*****************************************************************************/
//  Description : Get handle by volume type.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL void NotifyToAdjustVolume(BOOLEAN is_increase)
{
    SRVAUD_ENTITY_INFO_T *head_ptr = PNULL;
    SRVAUD_ENTITY_INFO_T *next_ptr = PNULL;
    
    head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
    if(PNULL == head_ptr)
    {
        return;
    }
    next_ptr = head_ptr;
    do
    {
        if(MMISRVAUD_VOLUME_TYPE_SELF == next_ptr->audio.audio_info.volume_type)
        {
            SrvNotifyClientExt(next_ptr, MMISRVAUD_REPORT_VOLUME_ADJUST, (uint32)is_increase);
            break;
        }
    }while((next_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetNextNode()) != head_ptr && next_ptr != PNULL);
}

#if 1//def XXXX
/*****************************************************************************/
//  Description : Stop standing service and notify client.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void StopStandingAndNotify(uint32 data, uint32 param)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio StopStandingAndNotify() entry, data=0x%x", data));

    if(PNULL == data)
    {
        return;
    }

    if(entity_ptr->audio.audio_info.is_standing)
    {
        AudStop(entity_ptr, PNULL);
        DRVAUD_CloseHandle(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
        //DeleteFromHandleBoxByEntityAddr((uint32)entity_ptr);
        MMISRVAUD_LinkGetAndDelete(SRVAUD_LINK_STANDING, (uint32)entity_ptr);

#ifndef VIRTUAL_VIBRATE_FUNC
        /* if is vibrate, we call the vibrate call back */
        if(MMISRVAUD_TYPE_VIBERATE == entity_ptr->audio.audio_info.info.type)
        {
            MMISRVAUD_VIBRATE_CB cb = entity_ptr->audio.audio_info.info.vibrate.cb;
            if(cb != PNULL)
            {
                cb();
            }
        }
        /* else we notify client that it is ended successfully */
        else
#endif
        {
            SrvNotifyClientExt(entity_ptr, MMISRVAUD_REPORT_END, (uint32)MMISRVAUD_REPORT_RESULT_SUCESS);
        }

        if(DeleteFromHandleBoxByEntityAddr((uint32)entity_ptr))
        {
            SCI_FREE(entity_ptr);
        }
    }
}

/*****************************************************************************/
//  Description : Stop all standing service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void StopStandingAud(void)
{
    MMISRVAUD_LinkTravel(SRVAUD_LINK_STANDING, StopStandingAndNotify, 0);
}

/*****************************************************************************/
//  Description : Make a judgement if a service is in playing state.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsEntityPlaying(uint32 data, uint32 param, uint32 *res)
{
    if(data != PNULL)
    {
        SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;
        return (IS_AUD_PLAY(entity_ptr));
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Make a judgement if there is some service in playing state.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsActiveAudPlaying(void)
{
    return MMISRVAUD_LinkTravelEx(SRVAUD_LINK_ACTIVE, IsEntityPlaying, 0, 0);
}
#endif

/*****************************************************************************/
//  Description : 通过driver handle 获取 mmi audio service handle
//  Global resource dependence :
//  Author:feiyue.ji
//  Note:
/*****************************************************************************/
LOCAL MMISRV_HANDLE_T GetSrvHandleByDrvHandle(DRVAUD_HANDLE drv_handle)
{
    SRVAUD_ENTITY_INFO_T* p_entity = PNULL;
    MMISRV_HANDLE_T handle = 0;

    p_entity = GetEntityByDrvHandle(drv_handle);
    if(!IsEntityValid(p_entity))
    {
        SCI_TraceLow("entity is invalid!");
    }
    else
    {
        handle = p_entity->handle;
    }
    SCI_TraceLow("drv_handle:%0x, srv handle:%0x", drv_handle, handle);
    return handle;
}

/*****************************************************************************/
//  Description : 通过 mmi audio service handle获取driver handle
//  Global resource dependence :
//  Author:zirui.li
//  Note:
/*****************************************************************************/
LOCAL DRVAUD_HANDLE GetDrvHandleBySrvHandle(MMISRV_HANDLE_T srv_handle)
{
    SRVAUD_ENTITY_INFO_T* p_entity = PNULL;
    DRVAUD_HANDLE drv_handle = 0;

    p_entity = (SRVAUD_ENTITY_INFO_T*)GetEntityAddrByHandle(srv_handle);
    if(!IsEntityValid(p_entity))
    {
        SCI_TraceLow("entity is invalid!");
    }
    else
    {
        drv_handle = p_entity->drv_handle;
    }
    SCI_TraceLow("drv_handle:%0x, srv handle:%0x", drv_handle, srv_handle);
    return drv_handle;
}

/*****************************************************************************/
//  Description : Data comunication between client and audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvIoCtrl(IMMISRV_UNKNOWN_T *srv_ptr, uint32 data, void *ret_value)
{

    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    MMISRVAUD_EVENT_T *event_data = (MMISRVAUD_EVENT_T *)data;
    MMISRVAUD_EVENT_RES_T *ret_ptr = ret_value;
    BOOLEAN result = FALSE;
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvIoCtrl() entry, srv_ptr=0x%x, data=0x%x, ret_value=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2583_112_2_18_3_23_49_166,(uint8*)"ddd", srv_ptr, data, ret_value));
    
    if(PNULL == srv_ptr || PNULL == event_data)
    {
        return FALSE;
    }
        
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)srv_ptr;

    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvIoCtrl(), event_data->event=%d, drv_handle=0x%x, service_state=%d, aud_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2593_112_2_18_3_23_49_167,(uint8*)"dddd", event_data->event, entity_ptr->drv_handle, entity_ptr->service.srv_state, entity_ptr->audio.audio_state);

    LOCK_SERVICE;
    
    switch(event_data->event)
    {
    case MMISRVAUD_EVENT_PLAY:
    case MMISRVAUD_EVENT_PLAY_ASY:
        if(ret_ptr != PNULL)
        {
            ret_ptr->data1 = AudPlay(entity_ptr, event_data);
            result = TRUE;
        }
        break;
    case MMISRVAUD_EVENT_PAUSE:
        result = AudPause(entity_ptr, event_data, TRUE);
        break;
    case MMISRVAUD_EVENT_STOP:
        result = AudStop(entity_ptr, event_data);
        break;
    case MMISRVAUD_EVENT_RESUME:
        result = AudResume(entity_ptr, event_data);
        break;
    case MMISRVAUD_EVENT_SEEK:
        //if(IS_SRV_SUSPEND(entity_ptr))// && IS_AUD_PLAY(entity_ptr))
        //{
        //    SrvResume(entity_ptr);
        //}
        result = DRVAUD_Seek(entity_ptr->drv_handle, event_data->data1);
        break;
    case MMISRVAUD_EVENT_SET_AUDIOINFO:
        result = SetAudioInfo(entity_ptr, (MMISRVAUD_TYPE_U *)event_data->data1);
        break;
    case MMISRVAUD_EVENT_GET_CONTENT_INFO:
        result = DRVAUD_GetContentInfo(entity_ptr->drv_handle, (MMISRVAUD_CONTENT_INFO_T *)event_data->data1);
        break;
    case MMISRVAUD_EVENT_GET_PLAYING_INFO:
        result = DRVAUD_GetPlayingInfo(entity_ptr->drv_handle, (MMISRVAUD_PLAY_INFO_T *)event_data->data1);
        break;
    case MMISRVAUD_EVENT_GET_RECORD_INFO:
        result = DRVAUD_GetRecordInfo(entity_ptr->drv_handle, (MMISRVAUD_RECORD_INFO_T *)event_data->data1);
        break;
    case MMISRVAUD_EVENT_GET_RECORD_SIZE: 
        result = DRVAUD_GetRecordSize(entity_ptr->drv_handle, (MMISRVAUD_RECORD_SIZE_T *)event_data->data1);
        break;        
    case MMISRVAUD_EVENT_SET_PLAY_SPEED:
        if(IS_SRV_ACTIVE(entity_ptr))
        {
            if(entity_ptr->audio.audio_info.speed != (MMISRVAUD_SPEED_T)event_data->data1)
            {
                result = DRVAUD_SetPlaySpeed(entity_ptr->drv_handle, (MMISRVAUD_SPEED_T)event_data->data1);
                if(result)
                {
                    entity_ptr->audio.audio_info.speed = (MMISRVAUD_SPEED_T)event_data->data1;
                }
            }
            else
            {
                result = TRUE;
            }
        }        
        break;
    case MMISRVAUD_EVENT_SET_EQ_MODE:
        if(IS_SRV_ACTIVE(entity_ptr))
        {
            if(entity_ptr->audio.audio_info.eq_mode != (MMISRVAUD_EQ_MODE_E)event_data->data1)
            {
                result = DRVAUD_SetEQMode(entity_ptr->drv_handle, (MMISRVAUD_EQ_MODE_E)event_data->data1);
                if(result)
                {
                    entity_ptr->audio.audio_info.eq_mode = (MMISRVAUD_EQ_MODE_E)event_data->data1;
                }
            }
            else
            {
                result = TRUE;
            }
        }
        break;
    case MMISRVAUD_EVENT_GET_VOLUME:
        if(ret_ptr != PNULL)
        {
            ret_ptr->data1 = (uint32)entity_ptr->audio.audio_info.volume;
            result = TRUE;
        }
        break;
    case MMISRVAUD_EVENT_SET_VOLUME:
        if(entity_ptr->audio.audio_info.volume!= (MMISRVAUD_VOLUME_T)event_data->data1)
        {
            result = SetVolumeByType(entity_ptr->audio.audio_info.volume_type, (MMISRVAUD_VOLUME_T)event_data->data1);
            if(!result)
            {
                if(IS_SRV_ACTIVE(entity_ptr) || IS_SRV_STANDING(entity_ptr))
                {        
                    result = DRVAUD_SetVolume(entity_ptr->drv_handle, (MMISRVAUD_VOLUME_T)event_data->data1);
                    if(result)
                    {
                        entity_ptr->audio.audio_info.volume = (MMISRVAUD_VOLUME_T)event_data->data1;
                    }
                }
                /* save the volume only */
                else
                {
                    entity_ptr->audio.audio_info.volume = (MMISRVAUD_VOLUME_T)event_data->data1;
                }
            }
        }
        else
        {
            result = TRUE;
        }
        break;
    case MMISRVAUD_EVENT_GET_ROUTE:
        if(ret_ptr != PNULL)
        {
            ret_ptr->data1 = (uint32)entity_ptr->audio.cur_route;
            result = TRUE;
        }
        break;
    case MMISRVAUD_EVENT_SET_ROUTE:    
        result = SetAppointRoute(entity_ptr,  (MMISRVAUD_ROUTE_T)event_data->data1);
        break;
    case MMISRVAUD_EVENT_SET_ALL_SURPPORT_ROUTE:
        result = SetAllSupportRoute(entity_ptr, (MMISRVAUD_ROUTE_T)event_data->data1); 
        break;
	case MMISRVAUD_EVENT_SET_MANUAL_ROUTE:
		result = SetManualRoute(entity_ptr, (MMISRVAUD_ROUTE_T)event_data->data1);
		break;
    case MMISRVAUD_EVENT_IS_SET_ROUTE_MANULLY:
        if(entity_ptr->audio.manual_route != 0)
        {
            ret_ptr->data1 = (uint32)TRUE;
        }
        else
        {
            ret_ptr->data1 = (uint32)FALSE;
        }
        result = TRUE;
        break;
    case MMISRVAUD_EVENT_SET_TIMES:
        entity_ptr->audio.audio_info.play_times = (uint16)event_data->data1;
        result = TRUE;
        break;
    case MMISRVAUD_EVENT_IS_AUDIO_END:
        if(ret_ptr != PNULL)
        {
            if(entity_ptr->audio.audio_state == AUDIO_STATE_STOP)
            {
                ret_ptr->data1 = (uint32)TRUE;
            }
            else
            {
                ret_ptr->data1 = (uint32)FALSE;
            }
            result = TRUE;
        }
        break;
    case MMISRVAUD_EVENT_GET_SPECTRUM:
        if(IS_SRV_ACTIVE(entity_ptr))
        {        
            result = DRVAUD_GetSpectrum(entity_ptr->drv_handle, (MMISRVAUD_SPECTRUM_INFO_T *)event_data->data1);
        }
        break;
    case MMISRVAUD_EVENT_OPEN_SPECTRUM:
        if(IS_SRV_ACTIVE(entity_ptr))
        {        
            result = DRVAUD_OpenSpectrum(entity_ptr->drv_handle);
        }
        break;
    case MMISRVAUD_EVENT_CLOSE_SPECTRUM:
        //if(IS_SRV_ACTIVE(entity_ptr))
        {                
            result = DRVAUD_CloseSpectrum(entity_ptr->drv_handle);
        }
        break;
    case MMISRVAUD_EVENT_SET_VOICE_LB:
        if(IS_SRV_ACTIVE(entity_ptr))
        {
            result = DRVAUD_SetVoiceLB(entity_ptr->drv_handle, (BOOLEAN)event_data->data1);
        }
        break;
    case MMISRVAUD_EVENT_GET_RECORD_DB:
        result = DRVAUD_GetRecordDb(entity_ptr->drv_handle, (uint32 *)(&ret_ptr->data1));
        break;
    case MMISRVAUD_EVENT_CHECK_IS_PLAYING:
        if(ret_ptr != PNULL)
        {
            if(IS_AUD_PLAY(entity_ptr))
            {
                ret_ptr->data1 = (uint32)TRUE;
            }
            else
            {
                ret_ptr->data1 = (uint32)FALSE;
            }
            result = TRUE;
        }
        break;
    case MMISRVAUD_EVENT_PAUSE_SLIGHTLY:
        if(IS_AUD_PLAY(entity_ptr))
        {
            result = DRVAUD_PauseSlight(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
			if(result)
			{
				entity_ptr->audio.audio_state = AUDIO_STATE_PAUSE;
			}
        }
        break;
    case MMISRVAUD_EVENT_RESUME_SLIGHTLY:
        //result = DRVAUD_ResumeSlight(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
		if(IS_AUD_PAUSE(entity_ptr))
        {
			SetAudParam(entity_ptr);
        	result = DRVAUD_ResumeSlight(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
            if(result)
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_PLAY;
            }
            else
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
            }
		}
        break;
    case MMISRVAUD_EVENT_SET_ATV_SERVICE_ID:
        entity_ptr->audio.audio_info.info.atv.service_id = event_data->data1;
        result = TRUE;
        break;
    case MMISRVAUD_EVENT_SET_ATV_REGION_ID:
        entity_ptr->audio.audio_info.info.atv.region_id = event_data->data1;
        result = TRUE;
        break;
    case MMISRVAUD_EVENT_START_RECORD_ATV:
        result = DRVAUD_StartRecordAtv(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, (void *)event_data->data1);
        break;
    case MMISRVAUD_EVENT_STOP_RECORD_ATV:
        result = DRVAUD_StopRecordAtv(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
        break;
    case MMISRVAUD_EVENT_GET_SRV_PRI:
        if(ret_ptr != PNULL)
        {
            ret_ptr->data1 = (uint32)entity_ptr->service.pri;
            result = TRUE;
        }
        break;
    case MMISRVAUD_EVENT_CLOSE:
        if(entity_ptr->drv_handle > 0)
        {
            if(DRVAUD_CloseHandle(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info))
            {
                entity_ptr->drv_handle = 0;
                result = TRUE;
            }
        }
        break;
    default:
        result = FALSE;
        break;
    }

    UNLOCK_SERVICE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvIoCtrl() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2842_112_2_18_3_23_49_168,(uint8*)"d", result));
    
    return result;
}

/*****************************************************************************/
//  Description : Respond the ext command that can't not be realised by Ioctrl.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SrvCmdFunc(MMISRVMGR_SERVICE_CMD_PARAM_T *param, uint32 *res_data)
{
    MMISRVMGR_SERVICE_CMD_E cmd = MMISRVMGR_SERVICE_CMD_NONE;
    MMISRV_HANDLE_T handle = 0;
    uint32 entity_addr = 0;    
    MMISRVAUD_EXT_CMD_T *cmd_param = PNULL;
    MMISRVAUD_EXT_CMD_RES_T *cmd_res = (MMISRVAUD_EXT_CMD_RES_T *)res_data;
    uint32 res = FALSE;

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvCmdFunc() entry, cmd=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2860_112_2_18_3_23_49_169,(uint8*)"d", cmd));

    if(param == PNULL)
    {
        return FALSE;
    }

    cmd = param->cmd;
    cmd_param = (MMISRVAUD_EXT_CMD_T *)param->data;

    LOCK_SERVICE;
    
    switch(cmd)
    {
    case MMISRVMGR_SERVICE_CMD_REQ:
        handle = SrvCreate((MMISRVMGR_SERVICE_REQ_T *)param->data, param->ext_data);
        if(handle > 0)
        {
            *res_data = handle;
            res = TRUE;
        }
        break;
    case MMISRVMGR_SERVICE_CMD_GET_ENTITY:
        handle = param->data;
        entity_addr = GetEntityAddrByHandle(handle);
        if(entity_addr != 0)
        {
            *res_data = entity_addr;
            res = TRUE;
        }
        break;
    case MMISRVMGR_SERVICE_CMD_EXT:
        if(PNULL == cmd_param)
        {
            break;
        }
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvCmdFunc() ,ext cmd=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_2893_112_2_18_3_23_49_170,(uint8*)"d", cmd_param->cmd));
        switch(cmd_param->cmd)
        {
        case MMISRVAUD_EXT_CMD_SET_EQMODE:
            res = DRVAUD_SetEQMode(0, (MMISRVAUD_EQ_MODE_E)cmd_param->data1);
            break;
        case MMISRVAUD_EXT_CMD_GET_VOLUME:
            /* if no active audio service, or active service is LOWEST, return FALSE */
            if(cmd_res != PNULL)
            {
                if(MMISRVAUD_LinkGetNum(SRVAUD_LINK_ACTIVE) > 0)
                {
                    SRVAUD_ENTITY_INFO_T *head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
                    if(head_ptr->service.pri > MMISRVAUD_PRI_LOW)
                    {
                        res = DRVAUD_GetVolume(0, (uint32 *)&cmd_res->data1);
                    }
                }    
            }
            break;
        case MMISRVAUD_EXT_CMD_SET_VOLUME:
            res = DRVAUD_SetVolume(0, (uint32)cmd_param->data1);
            break;
        case MMISRVAUD_EXT_CMD_GET_ROUTE:
            res = DRVAUD_GetRoute(0, (MMISRVAUD_ROUTE_T *)&cmd_res->data1);
            break;
        case MMISRVAUD_EXT_CMD_SET_ROUTE:
            res = DRVAUD_SetRoute(0, PNULL, (MMISRVAUD_ROUTE_T)cmd_param->data1);
            break;
        case MMISRVAUD_EXT_CMD_GET_ACTIVE_HANDLE:
            cmd_res->data1 = (uint32)s_mmisrv_aud_statues.active_entity;
            res = TRUE;
            break;
        case MMISRVAUD_EXT_CMD_GET_CFG:
            res = DRVAUD_GetAudCfg(0, (MMISRVAUD_AUD_CFG_T *)res_data);
            break;
        case MMISRVAUD_EXT_CMD_SET_MUTE:
            res = DRVAUD_SetMute(0, (BOOLEAN)cmd_param->data1);
            if(res)
            {
                s_mmisrv_aud_statues.is_mute = (BOOLEAN)cmd_param->data1;
            }
            break;
        case MMISRVAUD_EXT_CMD_GET_MUTE:
            res = DRVAUD_GetMute(0, (BOOLEAN *)&cmd_res->data1);
            break;
        case MMISRVAUD_EXT_CMD_SET_UPLINK_MUTE:
            res = DRVAUD_SetUplinkMute(0, (BOOLEAN)cmd_param->data1);
            break;
        case MMISRVAUD_EXT_CMD_SET_PLAY_RATE:
            res = DRVAUD_SetPlayRate(0, (MMISRVAUD_PLAY_RATE_E)cmd_param->data1);
            break;
        case MMISRVAUD_EXT_CMD_GET_FILE_INFO:
        case MMISRVAUD_EXT_CMD_GET_BUF_INFO:            
            {
                DRVAUD_HANDLE drv_handle = 0;
                MMISRVAUD_EXT_CMD_T *cmd_ptr = PNULL;
                if(PNULL == param->data)
                {
                    break;
                }
                cmd_ptr = (MMISRVAUD_EXT_CMD_T *)param->data;
                if(cmd_ptr->data1 != 0)
                {
#ifdef VIRTUAL_VIBRATE_FUNC
                    MMISRVAUD_TYPE_U* type_ptr = (MMISRVAUD_TYPE_U *)cmd_ptr->data1;
                    if((type_ptr->type  == MMISRVAUD_TYPE_VIBERATE))
                    {
                        drv_handle = DRVAUD_CreateHandle((MMISRVAUD_TYPE_U *)cmd_ptr->data1, FALSE, (uint32 *)&VBDecode_VibrateOuputBuf, DECODE_OUTPUT_BUF_SIZE);
                    }
                    else
                    {
                        drv_handle = DRVAUD_CreateHandle((MMISRVAUD_TYPE_U *)cmd_ptr->data1, FALSE, (uint32 *)&VBDecodeOuputBuf, DECODE_OUTPUT_BUF_SIZE);
                    }
#else
                    {
                        drv_handle = DRVAUD_CreateHandle((MMISRVAUD_TYPE_U *)cmd_ptr->data1, FALSE, (uint32 *)&VBDecodeOuputBuf, DECODE_OUTPUT_BUF_SIZE);
                    }
#endif                    
                    if(drv_handle > 0)
                    {
                        res = DRVAUD_GetContentInfo(drv_handle, (MMISRVAUD_CONTENT_INFO_T *)res_data);
                        DRVAUD_CloseHandle(drv_handle, (MMISRVAUD_TYPE_U *)cmd_ptr->data1);
                    }
                }
            }
            break;
        case MMISRVAUD_EXT_CMD_EXT_DEV_CHG:
            MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, SwichRoute, 0);
            break;
        case MMISRVAUD_EXT_CMD_BT_EVENT:
            BtEventHandle((MMISRVAUD_BT_EVENT_E)cmd_param->data1);
            break;
        case MMISRVAUD_EXT_CMD_EARPHONE_EVENT:
            EarphoneEventHandle((MMISRVAUD_EARPHONE_EVENT_ID)cmd_param->data1);
            break;
        case MMISRVAUD_EXT_CMD_GET_HANDLE_BY_NAME:
            if(cmd_res != PNULL)
            {            
                cmd_res->data1 = GetHandleByClientName((char *)cmd_param->data1);
                res = TRUE;
            }
            else
            {
                res = FALSE;
            }
            break;
        case MMISRVAUD_EXT_CMD_GET_HANDLE_BY_VOLUME_TYPE:
            if(cmd_res != PNULL)
            {
                cmd_res->data1 = GetHandleByVolumeType((MMISRVAUD_VOLUME_TYPE_E)cmd_param->data1);
                res = TRUE;
            }
            else
            {
                res = FALSE;
            }
            break;
        case MMISRVAUD_EXT_CMD_VOLUME_ADJUST:
            NotifyToAdjustVolume((BOOLEAN)cmd_param->data1);
            res = TRUE;
            break;
        case MMISRVAUD_EXT_CMD_SET_VOLUME_BY_TYPE:
            res = SetVolumeByType((MMISRVAUD_VOLUME_TYPE_E)cmd_param->data1, (MMISRVAUD_VOLUME_T)cmd_param->data2);
            break;
        case MMISRVAUD_EXT_CMD_DRV_CALLBACK:
            MMISRVAUD_HandleDrvCallBackMsg((void *)cmd_param->data1);
            break;
        case MMISRVAUD_EXT_CMD_GET_HANDLE_BY_DRV_HANDLE:
            if(cmd_res != PNULL)
            {
                cmd_res->data1 = GetSrvHandleByDrvHandle(cmd_param->data1);
                res = TRUE;
            }
            else
            {
                res = FALSE;
            }
            break;
        case MMISRVAUD_EXT_CMD_GET_DRV_HANDLE_BY_SRV_HANDLE:   //bug1989634
            if(cmd_res != PNULL)
            {
                cmd_res->data1 = GetDrvHandleBySrvHandle(cmd_param->data1);
                res = TRUE;
            }
            else
            {
                res = FALSE;
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }

    UNLOCK_SERVICE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio SrvCmdFunc() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3007_112_2_18_3_23_49_171,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Search drv handle by type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only to search Video, ATV, CMMB etc. type, whitch only one entity at the same time.
//           These function is only used to distinguish the different callback.
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_SearchHandleByType(MMISRVAUD_TYPE_E type)
{
    uint32 i = 0;
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    while(i < MMISRVAUD_SERVICE_NUM_MAX)
    {
        if(s_handle_box.entity_addr[i] != 0)
        {
            entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_handle_box.entity_addr[i];
            if(entity_ptr->audio.audio_info.info.type == type)
            {
                break;
            }
        }
        i++;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, type=%d, i=%d, entity_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3033_112_2_18_3_23_49_172,(uint8*)"ddd", type, i, entity_ptr));
    
    if(i < MMISRVAUD_SERVICE_NUM_MAX && entity_ptr!= PNULL)
    {
        return entity_ptr->drv_handle;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : Get type info by type.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: Only used for MV call back.
/*****************************************************************************/
PUBLIC MMISRVAUD_TYPE_U *MMISRVAUD_SearchTypeInfo(MMISRVAUD_TYPE_E type)
{
    uint32 i = 0;
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    while(i < MMISRVAUD_SERVICE_NUM_MAX)
    {
        if(s_handle_box.entity_addr[i] != 0)
        {
            entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_handle_box.entity_addr[i];
            if(entity_ptr->audio.audio_info.info.type == type)
            {
                break;
            }
        }
        i++;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, type%d, i=%d, entity_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3065_112_2_18_3_23_49_173,(uint8*)"ddd", type, i, entity_ptr));
    
    if(i < MMISRVAUD_SERVICE_NUM_MAX && entity_ptr != PNULL)
    {
        return &entity_ptr->audio.audio_info.info;
    }
    else
    {
        return PNULL;
    }    
}

/*****************************************************************************/
//  Description : Regist audio service.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
PUBLIC void MMISRVAUD_RegistService(void)
{
    MMISRVMGR_SERVICE_CREATE_T create = {0};
    uint16 len = 0;
    InitSrvAud();

    len = strlen(STR_SRV_AUD_NAME);
    if(len > MMISRVMGR_SERVICE_NAME_LEN) /*lint !e774*/
    {
        len = MMISRVMGR_SERVICE_NAME_LEN;
    }
    SCI_MEMCPY(create.name, STR_SRV_AUD_NAME, len);
    
    create.max_num = MMISRVAUD_SERVICE_NUM_MAX;
    create.pri_num = MMISRVAUD_PRI_NUM_MAX;
    create.cmd_func = SrvCmdFunc;
    
    s_aud_service_id = MMISRVMGR_AddService(&create, PNULL, 0);
}

/*****************************************************************************/
//  Description : Handle audio report message.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL void HandleAudioReport(uint32 drv_handle, MMISRVAUD_REPORT_T *rpt_data)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    BOOLEAN is_free_service = FALSE;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_HandleAudioReport() entry, drv_handle=0x%x, rpt_data=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3110_112_2_18_3_23_50_174,(uint8*)"dd", drv_handle, rpt_data));
    
    if(PNULL != rpt_data)
    {
        entity_ptr = GetEntityByDrvHandle(drv_handle);
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), entity_ptr=0x%x,report=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3115_112_2_18_3_23_50_175,(uint8*)"dd", entity_ptr,rpt_data->report));

        if(PNULL == entity_ptr)
        {
            return;
        }
        switch(rpt_data->report)
        {
        case MMISRVAUD_REPORT_END:
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), left play times=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3124_112_2_18_3_23_50_176,(uint8*)"d", entity_ptr->audio.audio_info.play_times));

            if(entity_ptr->audio.audio_info.play_times > 1)
            {
                entity_ptr->audio.audio_info.play_times--;

                if(AUDIO_STATE_PLAY == entity_ptr->audio.audio_state)
                {
                    DRVAUD_Play(drv_handle, &entity_ptr->audio.audio_info.info, 0, entity_ptr->audio.cur_route);
                    if(entity_ptr->audio.audio_info.duation > 0)
                    {
                        entity_ptr->audio.timer = MMISRVAUD_CreateTimer(entity_ptr->audio.audio_info.duation, (void *)HandleAudioTimer, 0, FALSE); /*lint !e611*/
                    }
                }
                
                /* Don't notify client, return directly */
                return;
            }
            else
            {
                /* if timer END indication is not reached, play audio again */
                if(entity_ptr->audio.timer != 0)
                {
                    DRVAUD_Play(drv_handle, &entity_ptr->audio.audio_info.info, 0, entity_ptr->audio.cur_route);
                    return;
                }
                else
                {
                    if(entity_ptr->service.is_auto_free)
                    {
                        /* Do free */
                        //SrvFree((IMMISRV_UNKNOWN_T *)entity_ptr);
                        is_free_service = TRUE;
                        /* return immediately, do not notify APP */
                        //return;
                    }
                    else
                    {
                        entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
                        entity_ptr->audio.audio_info.play_times = 0;
                    }
                    #ifdef AUDIO_MIXING_SUPPORT
                    #error "Here should be finished!!"
                    #else
                    if(IS_ASYNC_MODE && IS_SRV_ACTIVE(entity_ptr) && MMISRVAUD_ROUTE_BLUETOOTH == entity_ptr->audio.cur_route)
                    #endif
                    {
                        BT_START_TIMER;
                        BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
                        //BT_CLEAR_CUR;
                        //BT_CLEAR_WAIT;
                    }
                    else if(IS_SRV_ACTIVE(entity_ptr)&&s_bt_info.cur_event.entity_addr==(uint32)entity_ptr&&s_bt_info.state==BT_STATE_OPENED)
                    //else if(IS_SRV_ACTIVE(entity_ptr) && MMISRVAUD_ROUTE_BLUETOOTH != entity_ptr->audio.cur_route  && (uint32)entity_ptr == s_bt_info.cur_event.entity_addr)
                    {
                        BT_START_TIMER;
                        BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);                        
                    }
                    /*
                    else if(IsNeedSetRoute(entity_ptr))
                    {
                        BT_CLEAR_TIMER;
                        BT_CLEAR_CUR;
                        BT_CLEAR_WAIT;
                        BT_SET_STATE(BT_STATE_NONE);
                        //SET_ASYNC_MODE(FALSE);                    
                    }
                    */
                }
            }
            break;
        case MMISRVAUD_REPORT_TIMER_END:
            //AudStop(entity_ptr, PNULL);
            DRVAUD_Stop(drv_handle, &entity_ptr->audio.audio_info.info);
            if(entity_ptr->audio.audio_info.play_times > 1)
            {
                entity_ptr->audio.audio_info.play_times--;
                entity_ptr->audio.interval_timer = MMISRVAUD_CreateTimer(entity_ptr->audio.audio_info.interval, (void *)HandleAudioIntervalTimer, (uint32)entity_ptr, FALSE); /*lint !e611*/
                return;
            }
            else
            {
                is_free_service = entity_ptr->service.is_auto_free;
            }
            break;
        case MMISRVAUD_REPORT_INTERVAL_TIMER_END:
            if(entity_ptr->audio.audio_info.play_times > 1)
            {
                /* only for vibrate */
                DRVAUD_Play(drv_handle, &entity_ptr->audio.audio_info.info, entity_ptr->audio.audio_info.duation, entity_ptr->audio.cur_route);       
                //AudPlay(entity_ptr, PNULL);
                entity_ptr->audio.timer = MMISRVAUD_CreateTimer(entity_ptr->audio.audio_info.duation, (void *)HandleAudioTimer, 0, FALSE); /*lint !e611*/                
                return;
            }
            else
            {
                is_free_service = entity_ptr->service.is_auto_free;
            }            
            break;
        default:
            break;
        }

#ifndef VIRTUAL_VIBRATE_FUNC
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), entity_ptr->service.notify=0x%x, type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3187_112_2_18_3_23_50_177,(uint8*)"dd", entity_ptr->service.notify, entity_ptr->audio.audio_info.info.type));
        /* notity the client */
        if(MMISRVAUD_TYPE_VIBERATE == entity_ptr->audio.audio_info.info.type)
        {
            MMISRVAUD_VIBRATE_CB cb = entity_ptr->audio.audio_info.info.vibrate.cb;
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_HandleAudioReport(), vibrate cb=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3192_112_2_18_3_23_50_178,(uint8*)"d", cb));
            if(cb != PNULL)
            {
                cb();
            }
        }
        else
#endif
        {
            SrvNotifyClient(entity_ptr, MMISRVMGR_NOTIFY_EXT, (uint32)rpt_data);
        }
        if(is_free_service)
        {
            /* To gurrantee the service is not be freed in callback function, we check the entity again */
            if(GetHandleByEntityAddr((uint32)entity_ptr) != 0)
            {
                SrvFree((IMMISRV_UNKNOWN_T *)entity_ptr);
            }
        }
        //else if(entity_ptr->service.notify != PNULL)
        //{
        //    MMISRVMGR_NOTIFY_PARAM_T param = {0};
        //    param.event = MMISRVMGR_NOTIFY_EXT;
        //    param.data = (uint32)rpt_data;
        //    
        //    handle = (uint32)entity_ptr;
        //    entity_ptr->service.notify(handle, &param);
        //}
    }
}
/*****************************************************************************/
//  Description : Handle audio report message.
//  Global resource dependence : none
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_HandleAudioReport(uint32 drv_handle, MMISRVAUD_REPORT_T *rpt_data)
{
    //LOCK_SERVICE;
    HandleAudioReport(drv_handle, rpt_data);
    //UNLOCK_SERVICE;
}

/*****************************************************************************/
//  Description : Handle BT delay time out.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void HandleBTCloseDelayTimer(uint8 timer, uint32 param)
{
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio HandleBTCloseDelayTimer() entry, timer=%d,s_bt_info.timer=%d,s_bt_info.state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3228_112_2_18_3_23_50_179,(uint8*)"ddd", timer, s_bt_info.timer, s_bt_info.state));

    if(timer == s_bt_info.timer)
    {
        s_bt_info.timer = 0;
        switch(s_bt_info.state)
        {
        /* This state is what we want, we can close BT directly. */
        case BT_STATE_DELAY_TIMER_WAIT:
            MMISRVAUD_BtClose();
            BT_CLEAR_CUR;
            BT_CLEAR_WAIT;
            BT_CLEAR_TIMER;
            BT_SET_STATE(BT_STATE_CLOSE_WAIT);
            break;
        /* BT_STATE_NONE is mean BT is in close, it should not be this state this time. We close BT again to gurrante it is closed */
        case BT_STATE_NONE:
            MMISRVAUD_BtClose();
            BT_SET_STATE(BT_STATE_NONE);
            break;
        /* This state is maybe have any mistake in this time, do nothing now and to be determined later. */
        case BT_STATE_OPENED:
        case BT_STATE_OPEN_WAIT:
        case BT_STATE_CLOSE_WAIT:
        default:
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio HandleBTCloseDelayTimer(), Nothing to do in these state"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3253_112_2_18_3_23_50_180,(uint8*)""));
            break;
        }
    }
}

/*****************************************************************************/
//  Description : Make a judgement if need to re-open bluetooth.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsBtNeedReOpen(SRVAUD_ENTITY_INFO_T *entity_ptr, uint32 sample_rate)
{
    SRVAUD_ENTITY_INFO_T *cur_entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_bt_info.cur_event.entity_addr;
    BOOLEAN res = FALSE;
        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsBtNeedReOpen() entry, entity_ptr=0x%x, sample_rate=%d, cur_entity_ptr=0x%x, cur sample_rate=%d, s_bt_info.state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3268_112_2_18_3_23_50_181,(uint8*)"ddddd", entity_ptr, sample_rate, cur_entity_ptr, s_bt_info.cur_event.sample_rate, s_bt_info.state));

    if((s_bt_info.state != BT_STATE_NONE) && (s_bt_info.state != BT_STATE_CLOSE_WAIT))
    {
        if(cur_entity_ptr != PNULL && entity_ptr != PNULL)
        {
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsBtNeedReOpen(), new type=%d, old type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3274_112_2_18_3_23_50_182,(uint8*)"dd", entity_ptr->audio.audio_info.info.type, s_bt_info.cur_event.type));
        
            switch(entity_ptr->audio.audio_info.info.type)
            {
            case MMISRVAUD_TYPE_ATV:
            case MMISRVAUD_TYPE_VOICE:
                if(s_bt_info.cur_event.type != entity_ptr->audio.audio_info.info.type)
                {
                    res = TRUE;
                }
                break;
#ifdef VT_SUPPORT
		case MMISRVAUD_TYPE_VIRTUAL:
			if(MMIAPIVT_IsVtCalling()||MMIAPIVT_IsVtConnecting()||MMIAPIVT_IsVtMoCalling())
			{
				if(s_bt_info.cur_event.type != entity_ptr->audio.audio_info.info.type)
				{
				    res = TRUE;
				}
			}
			break;
#endif

            default:
                switch(s_bt_info.cur_event.type)
                {
                case MMISRVAUD_TYPE_ATV:
                case MMISRVAUD_TYPE_VOICE:
                    if(s_bt_info.cur_event.type != entity_ptr->audio.audio_info.info.type)
                    {
                        res = TRUE;
                    }
                    break;
#ifdef VT_SUPPORT
		case MMISRVAUD_TYPE_VIRTUAL:
			if(MMIAPIVT_IsVtCalling()||MMIAPIVT_IsVtConnecting()||MMIAPIVT_IsVtMoCalling())
			{
				if(s_bt_info.cur_event.type != entity_ptr->audio.audio_info.info.type)
				{
				    res = TRUE;
				}
			}
			break;
#endif
                default:
                    if(sample_rate != s_bt_info.cur_event.sample_rate)
                    {
                        res = TRUE;
                    }
                    break;
                }
                break;
            }
        }
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio IsBtNeedReOpen() exit, res=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3306_112_2_18_3_23_50_183,(uint8*)"d", res));
    
    return res;
}

/*****************************************************************************/
//  Description : Get bt open sample rate.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL uint32 BtGetSampleRate(SRVAUD_ENTITY_INFO_T *entity_ptr)
{
    MMISRVAUD_TYPE_E type = 0;
    uint32 sample_rate = 0;
    
    if(PNULL == entity_ptr)
    {
        return 0;
    }
    type = entity_ptr->audio.audio_info.info.type;
    switch(type)
    {
    case MMISRVAUD_TYPE_RING_FILE:
    case MMISRVAUD_TYPE_RING_BUF:
    case MMISRVAUD_TYPE_STREAMING:
    case MMISRVAUD_TYPE_VIDEO:
    case MMISRVAUD_TYPE_VIDEO_WP:
    case MMISRVAUD_TYPE_TONE_CUSTOM:
    case MMISRVAUD_TYPE_TONE_DTMF:
    case MMISRVAUD_TYPE_TONE_GENERIC:
        sample_rate = MMISRVAUD_SAMPLE_RATE_DEFAULT;
        break;
    case MMISRVAUD_TYPE_ATV:
        sample_rate = MMISRVAUD_SAMPLE_RATE_ATV;
        break;
    case MMISRVAUD_TYPE_MV:
        sample_rate = MMISRVAUD_SAMPLE_RATE_MV;
        break;
    default:
        break;
    }
    return sample_rate;
}

/*****************************************************************************/
//  Description : Play audio throught BT route, maybe we should connect BT firstly.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL MMISRVAUD_RET_E BtPrePlay(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr)
{
    MMISRVAUD_CONTENT_INFO_T content_info = {0};
    BOOLEAN result = FALSE;
    MMISRVAUD_EVENT_T event_data = {0};
    MMISRVAUD_RET_E ret_val = MMISRVAUD_RET_PENDING;
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePlay() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3322_112_2_18_3_23_50_184,(uint8*)"ddd", entity_ptr, event_ptr, s_bt_info.state));
    if(PNULL == entity_ptr)
    {
        return MMISRVAUD_RET_ERROR;
    }

    if(event_ptr != PNULL)
    {
        event_data = *event_ptr;
    }
    event_data.event = MMISRVAUD_EVENT_PLAY;

    content_info.samplerate = BtGetSampleRate(entity_ptr);
    
    /* set to async mode */    
    SET_ASYNC_MODE(TRUE);        
        
    switch(s_bt_info.state)
    {
    case BT_STATE_NONE:
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_NONE"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3344_112_2_18_3_23_50_185,(uint8*)""));
        if(MMISRVAUD_BtOpen(&entity_ptr->audio.audio_info.info, content_info.samplerate))
        {
            BT_SET_STATE(BT_STATE_OPEN_WAIT);
            BT_SET_CUR((uint32)entity_ptr, event_data, content_info.samplerate);
            BT_CLEAR_WAIT;        

            entity_ptr->audio.audio_state = AUDIO_STATE_PENDING;
        }
        else
        {
            ret_val = MMISRVAUD_RET_ERROR;
        }
        break;
    case BT_STATE_OPEN_WAIT:
    case BT_STATE_CLOSE_WAIT:
        /* If bt is in wait state, do nothing, and save the event to wait event */
        _DBG(        //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_OPEN_WAIT or BT_STATE_CLOSE_WAIT "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3361_112_2_18_3_23_50_186,(uint8*)""));
        BT_SET_WAIT((uint32)entity_ptr, event_data, content_info.samplerate);
        entity_ptr->audio.audio_state = AUDIO_STATE_PENDING;
        break;
    case BT_STATE_OPENED:
        /* If bt is in PLAY state, it should be an errer, 
            because the service should be suspended and audio should have been paused or stoped before the current service play.
            Even if the service to be played  is the playing service, the bt state should not PLAY state still.
        */
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtPrePlay(), error!!! BT should not be BT_STATE_PLAY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3370_112_2_18_3_23_50_187,(uint8*)"");
        ret_val = MMISRVAUD_RET_ERROR;
        break;
    case BT_STATE_DELAY_TIMER_WAIT:
        if(IsBtNeedReOpen(entity_ptr, content_info.samplerate))
        {
            BT_CLEAR_TIMER;            
            MMISRVAUD_BtClose();
            BT_SET_STATE(BT_STATE_OPEN_WAIT);
            BT_SET_CUR((uint32)entity_ptr, event_data, content_info.samplerate);
            BT_CLEAR_WAIT;       
            entity_ptr->audio.audio_state = AUDIO_STATE_PENDING;
        }
        else
        {
            /* switch route */
            _DBG(            //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePlay(), s_bt_info.state=BT_STATE_DELAY_TIMER_WAIT"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3386_112_2_18_3_23_50_188,(uint8*)""));
            
            SetAudParam(entity_ptr);
//            result = DRVAUD_Play(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, s_bt_info.cur_event.event_data.data1, entity_ptr->audio.cur_route);
			result = DRVAUD_Play(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, event_data.data1, entity_ptr->audio.cur_route);							
            if(result)
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_PLAY;
                BT_SET_STATE(BT_STATE_OPENED);
                BT_CLEAR_TIMER;
                ret_val = MMISRVAUD_RET_OK;        
            }
            else
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
                BT_START_TIMER;            
                BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
                ret_val = MMISRVAUD_RET_ERROR;        
            }
            BT_SET_CUR((uint32)entity_ptr, event_data, content_info.samplerate);
            BT_CLEAR_WAIT;
        }
        break;
    default:
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtPrePlay(), error!!! No this state!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3409_112_2_18_3_23_50_189,(uint8*)"");
        ret_val = MMISRVAUD_RET_ERROR;                
        break;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePlay() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3414_112_2_18_3_23_50_190,(uint8*)"d", result));
    
    return ret_val;
}

/*****************************************************************************/
//  Description : Resume audio throught BT route, maybe we should connect BT firstly.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BtPreResume(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr)
{
    MMISRVAUD_CONTENT_INFO_T content_info = {0};
    BOOLEAN result = FALSE;
    MMISRVAUD_EVENT_T event_data = {0};
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPreResume() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3429_112_2_18_3_23_50_191,(uint8*)"ddd", entity_ptr, event_ptr, s_bt_info.state));
    
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }
    
    if(event_ptr != PNULL)
    {
        event_data = *event_ptr;
    }
    event_data.event = MMISRVAUD_EVENT_RESUME;

    content_info.samplerate = BtGetSampleRate(entity_ptr);

    /* set to async mode */    
    SET_ASYNC_MODE(TRUE);        

    switch(s_bt_info.state)
    {
    case BT_STATE_NONE:
        if(MMISRVAUD_BtOpen(&entity_ptr->audio.audio_info.info, content_info.samplerate))
        {
            BT_SET_STATE(BT_STATE_OPEN_WAIT);
            BT_SET_CUR((uint32)entity_ptr, event_data, content_info.samplerate);
            BT_CLEAR_WAIT;        
            result = TRUE;
        }
        break;
    case BT_STATE_OPEN_WAIT:
    case BT_STATE_CLOSE_WAIT:
        /* If bt is in wait state, do nothing, and save the event to wait event */ 
        BT_SET_WAIT((uint32)entity_ptr, event_data, content_info.samplerate);
        result = TRUE;
        break;
    case BT_STATE_OPENED:
        /* If bt is in PLAY state, it should be an errer, 
            because the service should be suspended and audio should have been paused or stoped before the current service play.
            Even if the service to be played  is the playing service, the bt state should not PLAY state still.
        */
        //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtPreResume(), error!!! BT state should not be BT_STATE_PLAY"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3471_112_2_18_3_23_50_192,(uint8*)"");
        break;
    case BT_STATE_DELAY_TIMER_WAIT:
    {
        if(IsBtNeedReOpen(entity_ptr, content_info.samplerate))
        {
            BT_CLEAR_TIMER;            
            MMISRVAUD_BtClose();
            BT_SET_STATE(BT_STATE_OPEN_WAIT);
            BT_SET_CUR((uint32)entity_ptr, event_data, content_info.samplerate);
            BT_CLEAR_WAIT; 
            result = TRUE; /* Maybe we should return PENDING, TBD */
        }
        else        
        {
            /* switch route */
            SetAudParam(entity_ptr);
            result = DRVAUD_Resume(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
            if(result)
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_PLAY;
                BT_SET_STATE(BT_STATE_OPENED);
                BT_CLEAR_TIMER;
            }
            else
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
                BT_START_TIMER;
                BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
            }
            BT_SET_CUR((uint32)entity_ptr, event_data, content_info.samplerate);
            BT_CLEAR_WAIT;
        }
        break;
    }
    default:
        break;
    }
    //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtPreResume() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3508_112_2_18_3_23_50_193,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : Pause audio throught BT route, maybe we should disconnect BT.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BtPrePause(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr)
{
    MMISRVAUD_CONTENT_INFO_T content_info = {0};
    BOOLEAN result = FALSE;
    MMISRVAUD_EVENT_T event_data = {0};
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePause() entry, entity_ptr=0x%x, event_ptr=0x%x, s_bt_info.state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3522_112_2_18_3_23_50_194,(uint8*)"ddd", entity_ptr, event_ptr, s_bt_info.state));
    
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }

    if(event_ptr != PNULL)
    {
        event_data = *event_ptr;
    }
    event_data.event = MMISRVAUD_EVENT_PAUSE;

/* Don't pause audio here !!! */
/*
    result = DRVAUD_Pause(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
    if(result)
    {
        entity_ptr->audio.audio_state = AUDIO_STATE_PAUSE;
    }        
*/

    switch(s_bt_info.state)
    {
    case BT_STATE_NONE:
        /* BT is none state, it dosen't need to do bt pause or close, so do nothing */
        break;
    case BT_STATE_OPENED:
        /* BT is in working, so need to stop it. 
            User pause the current ring, but he may want to play another, so we don't close bt at once. 
            we need stop bt sometime later, so start a timer now */
        BT_START_TIMER;
        BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
        break;
    case BT_STATE_OPEN_WAIT:
    case BT_STATE_CLOSE_WAIT:
        if(DRVAUD_GetContentInfo(entity_ptr->drv_handle, &content_info))
        {
            BT_SET_WAIT((uint32)entity_ptr, event_data, content_info.samplerate);
        }
        break;
    case BT_STATE_DELAY_TIMER_WAIT:
        break;
    default:
        break;
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePause() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3568_112_2_18_3_23_50_195,(uint8*)"d", result));
    
    return result;
}

/*****************************************************************************/
//  Description : Stop audio throught BT route, maybe we should disconnect BT.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BtPreStop(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *data)
{
    MMISRVAUD_CONTENT_INFO_T content_info = {0};
    BOOLEAN result = FALSE;
    MMISRVAUD_EVENT_T event_data = {0};
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePause() entry, entity_ptr=0x%x, data=0x%x, s_bt_info.state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3583_112_2_18_3_23_51_196,(uint8*)"ddd", entity_ptr, data, s_bt_info.state));
    
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }

    if(data != PNULL)
    {
        event_data = *data;
    }
    event_data.event = MMISRVAUD_EVENT_STOP;

    if(!IS_AUD_PENDING(entity_ptr))
    {
        /* Firstly, we should stop audio play */
        result = DRVAUD_Stop(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
        if(result)
        {
            entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
        }        
    }
    else
    {
        entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
    }
    /* And than, change bt state */
    switch(s_bt_info.state)
    {
    case BT_STATE_NONE:
        /* BT is none state, it dosen't need to do bt pause or close, so do nothing */
        result = TRUE;
        break;
    case BT_STATE_OPENED:
        /* BT is in working, so need to stop it. 
            User pause the current ring, but he may want to play another, so we don't close bt at once. 
            we need stop bt sometime later, so start a timer now */
        BT_START_TIMER;
        BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
        break;
    case BT_STATE_OPEN_WAIT:
    case BT_STATE_CLOSE_WAIT:
        content_info.samplerate = BtGetSampleRate(entity_ptr);
        BT_SET_WAIT((uint32)entity_ptr, event_data, content_info.samplerate);
        break;
    case BT_STATE_DELAY_TIMER_WAIT:
        break;
    default:
        break;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtPrePause() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3629_112_2_18_3_23_51_197,(uint8*)"d", result));
    return result;

}

/*****************************************************************************/
//  Description : Handle BT active event.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Travel all the active link to find if there are audio service in play state.
/*****************************************************************************/
LOCAL void BtActiveHandle(void)
{
    SRVAUD_ENTITY_INFO_T *head_ptr = PNULL;
    SRVAUD_ENTITY_INFO_T *next_ptr = PNULL;
    BOOLEAN is_switch_bt = FALSE;

    _DBG(    //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtActiveHandle() entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3644_112_2_18_3_23_51_198,(uint8*)""));
    
    head_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetHeadNode(SRVAUD_LINK_ACTIVE);
    next_ptr = head_ptr;
    while(next_ptr != PNULL)
    {
        if(IS_AUD_PLAY(next_ptr) && IsNeedSetRoute(next_ptr) && IS_SHOULD_BT_ROUTE(next_ptr))
        {
            is_switch_bt = TRUE;
            break;
        }
        next_ptr = (SRVAUD_ENTITY_INFO_T *)MMISRVAUD_LinkGetNextNode();
        if(next_ptr == head_ptr)
        {
            break;
        }
    }
    
    if(is_switch_bt && next_ptr != PNULL)
    {
        uint32 samplerate = 0;

        samplerate = BtGetSampleRate(next_ptr);
        MMISRVAUD_BtOpen(&next_ptr->audio.audio_info.info, samplerate);
    }
}

/*****************************************************************************/
//  Description : Handle BT deactive event.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Travel all the active link to find if there are audio service in play state.
/*****************************************************************************/
LOCAL void BtDeActiveHandle(void)
{    
    _DBG(    //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtDeActiveHandle() entry"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3680_112_2_18_3_23_51_199,(uint8*)""));

    MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, SwichRoute, 0);
    BT_CLEAR_TIMER;
    BT_CLEAR_CUR;
    BT_CLEAR_WAIT;
    BT_SET_STATE(BT_STATE_NONE);
    SET_ASYNC_MODE(FALSE);
}    

/*****************************************************************************/
//  Description : Handle BT connect event.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void BtOpenedHandle(void)
{
    BOOLEAN is_close_bt = FALSE;
    SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_bt_info.cur_event.entity_addr;
    SRV_BT_EVENT_T *event_ptr = &s_bt_info.cur_event;
    SRV_BT_EVENT_T *wait_event_ptr = &s_bt_info.wait_event;
    BOOLEAN result = FALSE;
    BOOLEAN is_resume_pause = FALSE;    //add for bug 111538
    
        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtOpenedHandle() entry, s_bt_info.state=%d, cur entity_addr=0x%x,event=0x%d,sample_rate=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3703_112_2_18_3_23_51_200,(uint8*)"dddd", s_bt_info.state, s_bt_info.cur_event.entity_addr, s_bt_info.cur_event.event_data.event, s_bt_info.cur_event.sample_rate));
        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtOpenedHandle(), wait entity_addr=0x%x,event=0x%d,sample_rate=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3705_112_2_18_3_23_51_201,(uint8*)"ddd",  s_bt_info.wait_event.entity_addr, s_bt_info.wait_event.event_data.event, s_bt_info.wait_event.sample_rate));

    /* if is sync mode, we should switch to BT route for each service in active link */
    if( !IS_ASYNC_MODE)
    { 
		MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, SwichRoute, 0);
        return;
    }

    if(!IsEntityValid((SRVAUD_ENTITY_INFO_T *)s_bt_info.wait_event.entity_addr))
    {
        BT_CLEAR_WAIT;
    }
    if(!IsEntityValid((SRVAUD_ENTITY_INFO_T *)s_bt_info.cur_event.entity_addr))
    {
        BT_CLEAR_CUR;
        BT_TRANSF_NEXT;
    }

    /* if current handle is 0, it means no event should be handled.
        we do nothing in this situation and waiting to close BT some time later */
    if(0 == s_bt_info.cur_event.entity_addr)
    {
        BT_CLEAR_CUR;
        BT_CLEAR_WAIT;
        BT_START_TIMER;
        BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
        return;
    }
    
    /* If there is waiting event, we should make a judgement that which event should be handled next. */
    if(s_bt_info.wait_event.entity_addr != 0 && s_bt_info.wait_event.event_data.event != 0)
    {
        
        switch(event_ptr->event_data.event)
        {
        /*---------------------------------------------------------------------------------------------------*/
        case MMISRVAUD_EVENT_PLAY:
        case MMISRVAUD_EVENT_PLAY_ASY:
        case MMISRVAUD_EVENT_RESUME:            
            switch(wait_event_ptr->event_data.event)
            {
            case MMISRVAUD_EVENT_PLAY:
            case MMISRVAUD_EVENT_PLAY_ASY:
            case MMISRVAUD_EVENT_RESUME:
                /* If current and waiting handle are differente, we should play waiting one */
                if(s_bt_info.cur_event.entity_addr != s_bt_info.wait_event.entity_addr)
                {
                    /* If cur event and the waiting event are both PLAY, we play the waiting one and ignore the current.
                        There are two situations:
                            the one is that these two sample rates are same, we can play the waiting one directly,
                            another is that these two sample rates are different, we should close BT and reconnect it 
                    */
                    BT_TRANSF_NEXT;

                    /* sample rate are different, close BT and reconnect it */
                    //if(event_ptr->sample_rate != wait_event_ptr->sample_rate)
                    IsBtNeedReOpen((SRVAUD_ENTITY_INFO_T *)wait_event_ptr->entity_addr, wait_event_ptr->sample_rate);
                    {
                        is_close_bt = TRUE;
                    }
                }
                /* if they are same, ignore the waiting one */
                else
                {
                    BT_CLEAR_WAIT;
                }
                break;
            case MMISRVAUD_EVENT_PAUSE:
                //add for bug 111538
		        if(s_bt_info.cur_event.entity_addr != s_bt_info.wait_event.entity_addr)
                {
                    BT_CLEAR_WAIT;
                }
                /* If handle same, ignore the current event*/
                else
                {
                    BT_TRANSF_NEXT;
			        is_resume_pause = TRUE;
                }
                break;			
            case MMISRVAUD_EVENT_STOP:
                /* If handle is not same, ignore the waiting event, and play the current. */
                if(s_bt_info.cur_event.entity_addr != s_bt_info.wait_event.entity_addr)
                {
                    BT_CLEAR_WAIT;
                }
                /* If handle same, ignore the current event*/
                else
                {
                    BT_TRANSF_NEXT;
                }
                break;
            default:
                _DBG(                //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3777_112_2_18_3_23_51_202,(uint8*)"d", event_ptr->event_data.event));
                break;
            }
            break;


        /*---------------------------------------------------------------------------------------------------*/
        case MMISRVAUD_EVENT_PAUSE:
        case MMISRVAUD_EVENT_STOP:
            switch(wait_event_ptr->event_data.event)
            {
            case MMISRVAUD_EVENT_PLAY:
            case MMISRVAUD_EVENT_PLAY_ASY:
            case MMISRVAUD_EVENT_RESUME:
                if(s_bt_info.cur_event.entity_addr != s_bt_info.wait_event.entity_addr)
                {
                    /* If the waiting event is play/resume, we ignore the current event whitch is pause/stop.*/
                    BT_TRANSF_NEXT;

                    /* sample rate are different, close BT and reconnect it */
                    //if(event_ptr->sample_rate != wait_event_ptr->sample_rate)
                    IsBtNeedReOpen((SRVAUD_ENTITY_INFO_T *)wait_event_ptr->entity_addr, wait_event_ptr->sample_rate);                    
                    {
                        is_close_bt = TRUE;
                    }
                }
                /* if they are same handle, ignore the current one */
                else
                {
                    BT_TRANSF_NEXT;
                }
                break;
            case MMISRVAUD_EVENT_PAUSE:
            case MMISRVAUD_EVENT_STOP:
                /* the two are both pause/stop, only handle once, we select the waiting one */
                BT_TRANSF_NEXT;
                break;
            default:
                _DBG(                //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3815_112_2_18_3_23_51_203,(uint8*)"d", event_ptr->event_data.event));
                break;
            }
            break;
            
        default:
            _DBG(            //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtOpenedHandle(), cur event error!! event=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3821_112_2_18_3_23_51_204,(uint8*)"d", event_ptr->event_data.event));
            BT_TRANSF_NEXT;
            break;
        }
    }
    
    /* Here, we have determind to handle which event, 
        but maybe current BT connecting is not we want, maybe should disconnect it and reconnect it */
    /* If need close BT, should disconnect it */
    if(is_close_bt)
    {
        MMISRVAUD_BtClose();
        BT_SET_STATE(BT_STATE_OPEN_WAIT);
    }
    /* else, if not need to disconnect, we handle the current event that have been determined */
    else
    {
        /* Get current entity again to garantee get the right entity */
        entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_bt_info.cur_event.entity_addr;
        switch(s_bt_info.state)
        {
        case BT_STATE_NONE:
            //BT_CLEAR_CUR;
            BT_CLEAR_WAIT;            
            BT_START_TIMER;
            BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
            break;
        case BT_STATE_OPEN_WAIT:
            /* switch route */
            SetAudParam(entity_ptr);
            switch(event_ptr->event_data.event)
            {
            case MMISRVAUD_EVENT_PLAY:
                result = DRVAUD_Play(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, event_ptr->event_data.data1, entity_ptr->audio.cur_route);
                break;
            case MMISRVAUD_EVENT_PLAY_ASY:
                result = DRVAUD_PlayAsy(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, event_ptr->event_data.data1);
                break;
            case MMISRVAUD_EVENT_RESUME:
                /* If audio is not in PAUSE state, it cannot be resumed.
                    To avoid in some situation it cannot be resumed, we should play it, not resume it.
                    e.g,  async play and wait bt, than paused, and than resume, this time, the audio is in stop state, not resume state.
                */
                if(IS_AUD_PAUSE(entity_ptr) || IS_AUD_PLAY(entity_ptr))
                {
                    result = DRVAUD_Resume(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
                }
                else if(IS_AUD_PENDING(entity_ptr) || IS_AUD_STOP(entity_ptr))
                {
                    result = DRVAUD_Play(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, 0, entity_ptr->audio.cur_route);
                }                
                break;
            default:
                result = FALSE;
                break;
            }

			SCI_TRACE_LOW("[MMISRV]BtOpenedHandle, is_resume_pause = %d,result = %d", is_resume_pause, result);
            
            //add for bug 111538
	        if(is_resume_pause)
	     	{
                BT_START_TIMER;
                BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);	     	 	
	     	}
            else if(result)
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_PLAY;
                BT_SET_STATE(BT_STATE_OPENED);
                BT_CLEAR_TIMER;
                SrvNotifyClientExt(entity_ptr, MMISRVAUD_REPORT_PLAY, (uint32)MMISRVAUD_RET_OK);
            }
            else
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
                BT_START_TIMER;
                BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
                //BT_CLEAR_CUR;
                BT_CLEAR_WAIT;
                SrvNotifyClientExt(entity_ptr, MMISRVAUD_REPORT_PLAY, (uint32)MMISRVAUD_RET_ERROR);                
            }
            
            break;
        case BT_STATE_OPENED:
            /* do nothing, ignore the event */
            break;
        case BT_STATE_CLOSE_WAIT:
        case BT_STATE_DELAY_TIMER_WAIT:
            /* should not get here normally */
            //BT_CLEAR_CUR;
            BT_CLEAR_WAIT;
            BT_START_TIMER;
            BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
            break;
        default:
            break;
        }
    }


    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio BtOpenedHandle() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3910_112_2_18_3_23_51_205,(uint8*)"d", result));
    
    //return result;
}


/*****************************************************************************/
//  Description : Handle BT disconnect event.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 播放等待或者恢复等待时，被打断，如果仍就是stop状态，
//          就无法自动恢复了. TBD
/*****************************************************************************/
LOCAL void BtClosedHandle(void)
{
#define BT_CLOSE_REOPEN \
    {   \
        switch(event_ptr->event_data.event) \
        {   \
        case MMISRVAUD_EVENT_PLAY:  \
        case MMISRVAUD_EVENT_PLAY_ASY:  \
        case MMISRVAUD_EVENT_RESUME:    \
            if(entity_ptr != PNULL) \
            {   \
                MMISRVAUD_BtOpen(&entity_ptr->audio.audio_info.info, event_ptr->sample_rate);   \
                BT_SET_STATE(BT_STATE_OPEN_WAIT);   \
            }   \
            else    \
            {   \
                _DBG(                /*MMISRV_TRACE_ERR:"[MMISRV]: Audio BTClosedHandle() fatal error!! Entity should not be NULL"  */  \
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4049_112_2_18_3_23_51_211,(uint8*)""));   \
            }   \
            /*BT_SET_STATE(BT_STATE_OPEN_WAIT); */  \
            break;  \
        default:    \
            BT_CLEAR_CUR;   \
            BT_CLEAR_WAIT;  \
            BT_SET_STATE(BT_STATE_NONE);            \
            SET_ASYNC_MODE(FALSE);  \
            break;  \
        }   \
    }
    SRVAUD_ENTITY_INFO_T *entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_bt_info.cur_event.entity_addr;
    SRV_BT_EVENT_T *event_ptr = &s_bt_info.cur_event;
    SRV_BT_EVENT_T *wait_event_ptr = &s_bt_info.wait_event;
    BOOLEAN result = FALSE;
    BOOLEAN is_transfed = FALSE;
    
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio BTClosedHandle() entry, s_bt_info.state=%d, cur entity_addr=0x%x,event=0x%d,sample_rate=%d", 
        s_bt_info.state, s_bt_info.cur_event.entity_addr, s_bt_info.cur_event.event_data.event, s_bt_info.cur_event.sample_rate));

    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio BTClosedHandle(), wait entity_addr=0x%x,event=0x%d,sample_rate=%d", 
        s_bt_info.wait_event.entity_addr, s_bt_info.wait_event.event_data.event, s_bt_info.wait_event.sample_rate));

    if( !IS_ASYNC_MODE)
    {
        BT_CLEAR_CUR;
        BT_CLEAR_WAIT;
        BT_SET_STATE(BT_STATE_NONE);
        BT_CLEAR_TIMER;
        MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, SwichRoute, 0);
        return;
    }

    if(!IsEntityValid((SRVAUD_ENTITY_INFO_T *)s_bt_info.wait_event.entity_addr))
    {
        BT_CLEAR_WAIT;
    }
    if(!IsEntityValid((SRVAUD_ENTITY_INFO_T *)s_bt_info.cur_event.entity_addr))
    {
        BT_CLEAR_CUR;
        BT_TRANSF_NEXT;
        is_transfed = TRUE;
    }
    
    /* if current handle is 0, it means no event should be handled. */    
    if(0 == s_bt_info.cur_event.entity_addr)
    {
        /* BT is closed really, set BT state is NONE state, and chane async mode is FALSE */    
        BT_CLEAR_CUR;
        BT_CLEAR_WAIT;
        BT_SET_STATE(BT_STATE_NONE);
        MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, SwichRoute, 0);
        SET_ASYNC_MODE(FALSE);
        return;
    }
    BT_CLEAR_TIMER;

    /* If there is waiting event, we should make a judgement that which event should be handled next. */
    if(s_bt_info.wait_event.entity_addr != 0 && s_bt_info.wait_event.event_data.event != 0)
    {
        
        switch(event_ptr->event_data.event)
        {
        /*---------------------------------------------------------------------------------------------------*/
        case MMISRVAUD_EVENT_PLAY:
        case MMISRVAUD_EVENT_PLAY_ASY:
        case MMISRVAUD_EVENT_RESUME:            
            switch(wait_event_ptr->event_data.event)
            {
            case MMISRVAUD_EVENT_PLAY:
            case MMISRVAUD_EVENT_PLAY_ASY:
            case MMISRVAUD_EVENT_RESUME:
                /* If current and waiting handle are differente, we should play waiting one */
                if(s_bt_info.cur_event.entity_addr != s_bt_info.wait_event.entity_addr)
                {
                    BT_TRANSF_NEXT;
                }
                /* if they are same, ignore the waiting one */
                else
                {
                    BT_CLEAR_WAIT;
                }
                break;
            case MMISRVAUD_EVENT_PAUSE:
            case MMISRVAUD_EVENT_STOP:
                /* If handle is not same, ignore the waiting event, and play the current. */
                if(s_bt_info.cur_event.entity_addr != s_bt_info.wait_event.entity_addr)
                {
                    BT_CLEAR_WAIT;
                }
                /* If handle same, ignore the current event*/
                else
                {
                    BT_TRANSF_NEXT;
                }
                break;
            default:
                _DBG(                //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_3993_112_2_18_3_23_51_208,(uint8*)"d", event_ptr->event_data.event));
                break;
            }
            break;


        /*---------------------------------------------------------------------------------------------------*/
        case MMISRVAUD_EVENT_PAUSE:
        case MMISRVAUD_EVENT_STOP:
            switch(wait_event_ptr->event_data.event)
            {
            case MMISRVAUD_EVENT_PLAY:
            case MMISRVAUD_EVENT_PLAY_ASY:
            case MMISRVAUD_EVENT_RESUME:
                /* If the waiting event is play/resume, we ignore the current event whitch is pause/stop.*/
                BT_TRANSF_NEXT;
                break;
            case MMISRVAUD_EVENT_PAUSE:
            case MMISRVAUD_EVENT_STOP:
                /* the two are both pause/stop, only handle once, we select the waiting one */
                BT_TRANSF_NEXT;
                break;
            default:
                _DBG(                //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtOpenedHandle(), waiting event error!! event=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4016_112_2_18_3_23_51_209,(uint8*)"d", event_ptr->event_data.event));
                break;
            }
            break;
            
        default:
            _DBG(            //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtOpenedHandle(), cur event error!! event=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4022_112_2_18_3_23_51_210,(uint8*)"d", event_ptr->event_data.event));
            BT_TRANSF_NEXT;
            break;
        }
    }

    entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_bt_info.cur_event.entity_addr;
    switch(s_bt_info.state)
    {
    case BT_STATE_NONE:
        BT_CLEAR_CUR;
        BT_CLEAR_WAIT;
        SET_ASYNC_MODE(FALSE);
        break;

    case BT_STATE_OPEN_WAIT:
        BT_CLOSE_REOPEN;
        #if 0
        switch(event_ptr->event_data.event)
        {
        case MMISRVAUD_EVENT_PLAY:
        case MMISRVAUD_EVENT_PLAY_ASY:
        case MMISRVAUD_EVENT_RESUME:
            if(entity_ptr != PNULL)
            {
                MMISRVAUD_BtOpen(&entity_ptr->audio.audio_info.info, event_ptr->sample_rate);
            }
            else
            {
                _DBG(                //MMISRV_TRACE_ERR:"[MMISRV]: Audio BTClosedHandle() fatal error!! Entity should not be NULL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4049_112_2_18_3_23_51_211,(uint8*)""));
            }
            //BT_SET_STATE(BT_STATE_OPEN_WAIT);
            break;
        default:
            BT_CLEAR_CUR;
            BT_CLEAR_WAIT;
            BT_SET_STATE(BT_STATE_NONE);        
            SET_ASYNC_MODE(FALSE);
            break;
        }
        #endif
        /* In this time, route should have been switched to other route, so we don't switch route here */
        break;
        
    /* Normally, this state is we want */                
    case BT_STATE_CLOSE_WAIT:
        if(!is_transfed)
        {
            BT_CLEAR_CUR;
            BT_CLEAR_WAIT;
            BT_SET_STATE(BT_STATE_NONE);
            SET_ASYNC_MODE(FALSE);
        }
        else
        {
            BT_CLOSE_REOPEN;
        }
        break;
    case BT_STATE_DELAY_TIMER_WAIT:
    case BT_STATE_OPENED:
        _DBG(        //MMISRV_TRACE_ERR:"[MMISRV]: Audio BTClosedHandle() error bt state, may BT module error!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4072_112_2_18_3_23_52_212,(uint8*)""));
        BT_CLEAR_CUR;
        BT_CLEAR_WAIT;
        BT_SET_STATE(BT_STATE_NONE);
        SET_ASYNC_MODE(FALSE);
        /* 通道要全部切换,因为可能是兰牙耳机没电等原因导致的主动关闭*/
        MMISRVAUD_LinkTravel(SRVAUD_LINK_ACTIVE, SwichRoute, 0);
        break;
    default:
        result = FALSE;
        break;
    }
    
}

/*****************************************************************************/
//  Description : Notify bt event to app.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BtNotifyAppEvent(uint32 data, uint32 param, uint32 *res)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = 0;
    MMISRVAUD_ROUTE_T route = 0;
    if(0 == data)
    {
        _DBG(        //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtNotifyAppEvent() error!!! handle=0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4097_112_2_18_3_23_52_213,(uint8*)""));
        return FALSE;
    }
    
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;    
    if(IsNeedSetRoute(entity_ptr))
    {
        route = GetShouldBeRoute(entity_ptr);
    }

    _DBG(    //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtNotifyAppEvent() entry, route=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4107_112_2_18_3_23_52_214,(uint8*)"d", route));
    
    if(MMISRVAUD_ROUTE_BLUETOOTH == route)
    {
        SrvNotifyClientExt(entity_ptr, MMISRVAUD_REPORT_BT_EVENT, (uint32)param);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : Notify bt event to CC handle.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN BtNotifyCCEvent(uint32 data, uint32 param, uint32 *res)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = 0;
    BOOLEAN result = FALSE;
    _DBG(MMISRV_TRACE_ERR("[MMISRV]: Audio BtNotifyCCEvent() entry, data=0x%x, param=%d,res=%d",data, param, res));
    
    if(0 == data)
    {
        return FALSE;
    }
    
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;    
    if(MMISRVAUD_TYPE_VOICE == entity_ptr->audio.audio_info.info.type && IS_AUD_PLAY(entity_ptr))    
    {
        SrvNotifyClientExt(entity_ptr, MMISRVAUD_REPORT_BT_EVENT, (uint32)param);
        result = TRUE;
    }
    else
    {
        result = FALSE;
#ifdef VT_SUPPORT
	if(MMIAPIVT_IsVtCalling()||MMIAPIVT_IsVtConnecting()||MMIAPIVT_IsVtMoCalling())
	{
		if(MMISRVAUD_TYPE_VIRTUAL == entity_ptr->audio.audio_info.info.type && IS_AUD_PLAY(entity_ptr))    
		{
		    SrvNotifyClientExt(entity_ptr, MMISRVAUD_REPORT_BT_EVENT, (uint32)param);
		    result = TRUE;
		}
	}	
#endif
    }
    _DBG(MMISRV_TRACE_ERR("[MMISRV]: Audio BtNotifyCCEvent() exit,type=%d,is_play=%d,res=%d",entity_ptr->audio.audio_info.info.type, IS_AUD_PLAY(entity_ptr), result));    /*lint !e730*/

    return result;
}

/*****************************************************************************/
//  Description : Notify bt CC.
//  Global resource dependence : 
//  Author:Zirui.Li
//  Note:
/*****************************************************************************/
LOCAL void BtNotifyCC(MMISRVAUD_BT_EVENT_E event)
{
#ifdef ADULT_WATCH_SUPPORT
    MMISRVAUD_LinkTravelEx(SRVAUD_LINK_ACTIVE, BtNotifyCCEvent, event, 0);
#endif
}

/*****************************************************************************/
//  Description : Handle BT event.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL void BtEventHandle(MMISRVAUD_BT_EVENT_E event)
{
    _DBG(    //MMISRV_TRACE_ERR:"[MMISRV]: Audio BtEventHandle() entry, event=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4126_112_2_18_3_23_52_215,(uint8*)"d", event));

    if(event > MMISRVAUD_BT_EVENT_NONE && event < MMISRVAUD_EVENT_MAX) /*lint !e650*/
    {
        if(MMISRVAUD_BT_EVENT_ACTIVE == event || MMISRVAUD_BT_EVENT_DEACTIVE == event 
			|| MMISRVAUD_BT_EVENT_OPENED == event || MMISRVAUD_BT_EVENT_CLOSED == event)
        {
            if(MMISRVAUD_LinkTravelEx(SRVAUD_LINK_ACTIVE, BtNotifyCCEvent, event, 0))
            {
                /* if DEACTIVE EVENT, we should clear the current state, so do not return */
                if(MMISRVAUD_BT_EVENT_ACTIVE == event)
                {
                    return;
                }
            }
        }
        switch(event)
        {
        case MMISRVAUD_BT_EVENT_ACTIVE:
            BtActiveHandle();
            break;
        case MMISRVAUD_BT_EVENT_DEACTIVE:
            BtDeActiveHandle();
            break;
        case MMISRVAUD_BT_EVENT_OPENED:
            BtOpenedHandle();
            BtNotifyCC(event);  //Bug1668319
            break;
        case MMISRVAUD_BT_EVENT_CLOSED:
            BtClosedHandle();
            BtNotifyCC(event);  //Bug1668319
            break;
        case MMISRVAUD_BT_EVENT_KEY_PAUSE:
        case MMISRVAUD_BT_EVENT_KEY_RESUME:
        case MMISRVAUD_BT_EVENT_KEY_PREV:
        case MMISRVAUD_BT_EVENT_KEY_NEXT:
        case MMISRVAUD_BT_EVENT_KEY_STOP:
        case MMISRVAUD_BT_EVENT_KEY_VOL_UP:
        case MMISRVAUD_BT_EVENT_KEY_VOL_DOWN:
            if(!MMISRVAUD_LinkTravelEx(SRVAUD_LINK_ACTIVE, BtNotifyAppEvent, event, 0))
            {
                MMISRVAUD_SendBtMsg((uint32)event);
            }
            break;
        default:
            break;
        }
    }
}

/*******************************************************************/
//  Interface:		EarphoneNotifyAppEvent
//  Description : 	EarphoneNotifyAppEvent
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL BOOLEAN EarphoneNotifyAppEvent(uint32 data, uint32 param, uint32 *res)
{
    SRVAUD_ENTITY_INFO_T *entity_ptr = 0;
    MMISRVAUD_ROUTE_T route = 0;
    if(0 == data)
    {
        _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio EarphoneNotifyAppEvent() error!!! handle=0"));
        return FALSE;
    }
    
    entity_ptr = (SRVAUD_ENTITY_INFO_T *)data;    
    if(IsNeedSetRoute(entity_ptr))
    {
        route = GetShouldBeRoute(entity_ptr);
    }
    
    _DBG( MMISRV_TRACE_LOW("[MMISRV]: Audio EarphoneNotifyAppEvent() entry, route=%d", route));
    
    if(MMISRVAUD_ROUTE_EARPHONE == route || MMISRVAUD_ROUTE_EARFREE == route || MMISRVAUD_ROUTE_SPEAKER == route)
    {
        SrvNotifyClientExt(entity_ptr, MMISRVAUD_REPORT_EARPHONE_EVENT, (uint32)param);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*******************************************************************/
//  Interface:		EarphoneEventHandle
//  Description : 	EarphoneEventHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
LOCAL void EarphoneEventHandle(MMISRVAUD_EARPHONE_EVENT_ID event)
{
    _DBG(MMISRV_TRACE_LOW("[MMISRV]: Audio EarphoneEventHandle() entry, event=%d", event));
    
    switch(event & 0xff00)
    {
    case MMISRVAUD_EARPHONE_EVENT_CLK:
    case MMISRVAUD_EARPHONE_EVENT_LONG:
    case MMISRVAUD_EARPHONE_EVENT_DBLCLK:
        MMISRVAUD_LinkTravelEx(SRVAUD_LINK_ACTIVE, EarphoneNotifyAppEvent, event, 0);
        break;
    default:
        break;
    }
}

/*****************************************************************************/
//  Description : Audio play start.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL MMISRVAUD_RET_E AudPlay(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr)
{
    MMISRVAUD_RET_E ret_val = MMISRVAUD_RET_ERROR;
    BOOLEAN is_play_asy = FALSE;
    BOOLEAN result = FALSE;
    BOOLEAN is_bt_route = FALSE;
    
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }

        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio AudPlay() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4179_112_2_18_3_23_52_216,(uint8*)"dddd", entity_ptr, event_ptr,entity_ptr->service.srv_state, entity_ptr->audio.audio_state));

    if(entity_ptr->audio.audio_info.is_standing)
    {
        if(IsActiveAudPlaying())
        {
            return MMISRVAUD_RET_ERROR;
        }
    }
    else
    {
        StopStandingAud();
    }
    if(IS_SRV_SUSPEND(entity_ptr))
    {
        result = SrvResumeOnly((IMMISRV_UNKNOWN_T *)entity_ptr);
        
        /* here we resume audio only it is in pause state */
        if(result && IS_AUD_PLAY(entity_ptr))
        {
            result = AudResume(entity_ptr, event_ptr);
            if(result)
            {
                ret_val = MMISRVAUD_RET_OK;
            }
            return ret_val;
        }
        //result = AudResume(entity_ptr, event_ptr);
    }

    if((IS_SRV_ACTIVE(entity_ptr) || IS_SRV_STANDING(entity_ptr)) && !IS_AUD_PLAY(entity_ptr))
    {
        is_bt_route = (BOOLEAN)(IsNeedSetRoute(entity_ptr) && IS_SHOULD_BT_ROUTE(entity_ptr));
        if(is_bt_route)
        {
            ret_val = BtPrePlay(entity_ptr, event_ptr);
        }
        /* if bt open failed, we should play through other route */
        if(!is_bt_route || MMISRVAUD_RET_ERROR == ret_val)
        {
            uint32 offset = 0;
            
            if(PNULL == event_ptr)
            {
                offset = 0;
            }
            else
            {
                offset = event_ptr->data1;
                if(MMISRVAUD_EVENT_PLAY_ASY == event_ptr->event)
                {
                    is_play_asy = TRUE;
                }
            }
            SetAudParam(entity_ptr);
            if(is_play_asy)
            {
                result = DRVAUD_PlayAsy(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, offset);
            }
            else
            {
                result = DRVAUD_Play(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, offset, entity_ptr->audio.cur_route);
            }
            
            if(result)
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_PLAY;
                if(entity_ptr->audio.audio_info.duation > 0)
                {
                    entity_ptr->audio.timer = MMISRVAUD_CreateTimer(entity_ptr->audio.audio_info.duation, (void *)HandleAudioTimer, 0, FALSE); /*lint !e611*/
                }
                ret_val = MMISRVAUD_RET_OK;
            }
            else
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
                ret_val = MMISRVAUD_RET_ERROR;                
            }
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio AudPlay() exit, ret_val=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4238_112_2_18_3_23_52_217,(uint8*)"d", ret_val));
    
    return ret_val;
}

/*****************************************************************************/
//  Description : Audio resume.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AudResume(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr)
{
    BOOLEAN result = FALSE;

    if(PNULL == entity_ptr)
    {
        return FALSE;
    }

        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio AudResume() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4257_112_2_18_3_23_52_218,(uint8*)"dddd", entity_ptr, event_ptr,entity_ptr->service.srv_state, entity_ptr->audio.audio_state));

    if(entity_ptr->audio.audio_info.is_standing)
    {
        if(IsActiveAudPlaying())
        {
            return FALSE;
        }
    }
    else
    {
        StopStandingAud();
    }
    
    if(IS_SRV_SUSPEND(entity_ptr))
    {
        result = SrvResumeOnly((IMMISRV_UNKNOWN_T *)entity_ptr);
    }

    if((IS_SRV_ACTIVE(entity_ptr) || IS_SRV_STANDING(entity_ptr))&& (IS_AUD_PAUSE(entity_ptr) || IS_AUD_PLAY(entity_ptr)))
    {
        if(IsNeedSetRoute(entity_ptr) && IS_SHOULD_BT_ROUTE(entity_ptr))
        {
            result = BtPreResume(entity_ptr, event_ptr);
        }
        else
        {
            SetAudParam(entity_ptr);
            if(IS_AUD_PAUSE(entity_ptr) || IS_AUD_PLAY(entity_ptr))
            {
                result = DRVAUD_Resume(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
            }
            else if(IS_AUD_PENDING(entity_ptr) || IS_AUD_STOP(entity_ptr))
            {
                result = DRVAUD_Play(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info, 0, entity_ptr->audio.cur_route);                
            }
            if(result)
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_PLAY;
            }
            else
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
            }            
        }
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio AudResume() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4292_112_2_18_3_23_52_219,(uint8*)"d", result));
    
    return result;
}

/*****************************************************************************/
//  Description : Audio pause.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AudPause(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr, BOOLEAN is_change_state)
{
    BOOLEAN result = FALSE;

    if(PNULL == entity_ptr)
    {
        return FALSE;
    }

        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio AudPause() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d,is_change_state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4311_112_2_18_3_23_52_220,(uint8*)"ddddd", entity_ptr, event_ptr,entity_ptr->service.srv_state, entity_ptr->audio.audio_state, is_change_state));

    if((IS_SRV_ACTIVE(entity_ptr) || IS_SRV_STANDING(entity_ptr))&& IS_AUD_PLAY(entity_ptr))
    {
        #ifndef AUDIO_MIXING_SUPPORT
        result = DRVAUD_Pause(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
        if(result && is_change_state)
        {
            entity_ptr->audio.audio_state = AUDIO_STATE_PAUSE;
        }
        
        if(IS_CUR_BT_ROUTE(entity_ptr) || (BT_STATE_OPENED == BT_GET_STATE && s_bt_info.cur_event.entity_addr == (uint32)entity_ptr))
        {
            BtPrePause(entity_ptr, event_ptr);
        }
        else
        {
            /* Maybe these codes should be cutted,  would be tested in the future */
            if(BT_STATE_OPENED == BT_GET_STATE && s_bt_info.cur_event.entity_addr == (uint32)entity_ptr)
            {
                BT_CLEAR_TIMER;
                BT_CLEAR_CUR;
                BT_CLEAR_WAIT;
                BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
                //SET_ASYNC_MODE(FALSE);
            }        
        }
        #else
        /* Don't support this case, make it die */
        _DBG((        //MMISRV_TRACE_ERR:"[MMISRV]: Audio, Don't not surpport audio mixing now!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4328_112_2_18_3_23_52_221,(uint8*)"")));
        #endif

        if(entity_ptr->audio.timer != 0)
        {
            MMISRVAUD_StopTimer(entity_ptr->audio.timer);
            entity_ptr->audio.timer = 0;
        }
        if(entity_ptr->audio.interval_timer != 0)
        {
            MMISRVAUD_StopTimer(entity_ptr->audio.interval_timer);
            entity_ptr->audio.interval_timer = 0;        
        }
        
    }
    /* else, if audio state is PLAYING, it means the service was suspended by other service, set to PAUSE; 
        if not playing, it must be Pause or STOP, do not change state */
    else
    {
        if(IS_AUD_PLAY(entity_ptr))
        {
            entity_ptr->audio.audio_state = AUDIO_STATE_PAUSE;
        }
        result = TRUE;
    }

    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio AudPause() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4348_112_2_18_3_23_52_222,(uint8*)"d", result));
    
    return result;
}

/*****************************************************************************/
//  Description : Audio stop.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN AudStop(SRVAUD_ENTITY_INFO_T *entity_ptr, MMISRVAUD_EVENT_T *event_ptr)
{
    BOOLEAN result = FALSE;

    if(PNULL == entity_ptr)
    {
        return FALSE;
    }

        _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio AudStop() entry, entity_ptr=0x%x,event_ptr=0x%x,service state=%d, audio state=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4367_112_2_18_3_23_52_223,(uint8*)"dddd", entity_ptr, event_ptr,entity_ptr->service.srv_state, entity_ptr->audio.audio_state));

    if(entity_ptr->audio.timer != 0)
    {
        MMISRVAUD_StopTimer(entity_ptr->audio.timer);
        entity_ptr->audio.timer = 0;
    }    
    if(entity_ptr->audio.interval_timer != 0)
    {
        MMISRVAUD_StopTimer(entity_ptr->audio.interval_timer);
        entity_ptr->audio.interval_timer = 0;        
    }

    if((IS_SRV_ACTIVE(entity_ptr) || IS_SRV_STANDING(entity_ptr))&& (IS_AUD_PLAY(entity_ptr) || IS_AUD_PAUSE(entity_ptr) || IS_AUD_PENDING(entity_ptr)))
    {
        #ifndef AUDIO_MIXING_SUPPORT
        if(IS_CUR_BT_ROUTE(entity_ptr) || (BT_STATE_OPENED == BT_GET_STATE && s_bt_info.cur_event.entity_addr == (uint32)entity_ptr))
        {
            result = BtPreStop(entity_ptr, event_ptr);
        }
        else
        {
            result = DRVAUD_Stop(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
            if(result)
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
            }

            /* Maybe these codes should be cutted,  would be tested in the future */
            if(BT_STATE_OPENED == BT_GET_STATE && s_bt_info.cur_event.entity_addr == (uint32)entity_ptr)
            {
                BT_CLEAR_TIMER;
                BT_CLEAR_CUR;
                BT_CLEAR_WAIT;
                BT_SET_STATE(BT_STATE_DELAY_TIMER_WAIT);
                //SET_ASYNC_MODE(FALSE);
            }
        }
        #else
        /* Don't support this case, make it die */
        _DBG((        //MMISRV_TRACE_ERR:"[MMISRV]: Audio, Don't not surpport audio mixing now!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4386_112_2_18_3_23_52_224,(uint8*)"")));
        #endif

    }
    /* else, set its state to STOP directly */
    else
    {
        if(!IS_AUD_STOP(entity_ptr) && !IS_AUD_PENDING(entity_ptr))
        {
            result = DRVAUD_Stop(entity_ptr->drv_handle, &entity_ptr->audio.audio_info.info);
            if(result)
            {
                entity_ptr->audio.audio_state = AUDIO_STATE_STOP;
            }
        }
        else
        {
            entity_ptr->audio.audio_state = AUDIO_STATE_STOP;        
            result = TRUE;
        }    
    }
    
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio AudStop() exit, result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4413_112_2_18_3_23_52_225,(uint8*)"d", result));
    
    return result;
    
}

/*****************************************************************************/
//  Description : change the default route of entity_ptr
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: change the default route of entity_ptr after requested handle
//           
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetDefaultRouteByHandle(MMISRV_HANDLE_T audio_handle, MMISRVAUD_ROUTE_T audio_route) 
{
    
	SRVAUD_ENTITY_INFO_T *entity_ptr = GetEntityAddrByHandle(audio_handle);
	
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }
    entity_ptr->audio.audio_info.default_route = audio_route;   
    return TRUE;
}


/*****************************************************************************/
//  Description : set the audio state to play state  
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: this function is only used in video player
//           
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAudioPlayStateByHandle(MMISRV_HANDLE_T audio_handle) 
{
    
	SRVAUD_ENTITY_INFO_T *entity_ptr = GetEntityAddrByHandle(audio_handle);
	
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }
    entity_ptr->audio.audio_state= AUDIO_STATE_PLAY;   
	SCI_TRACE_LOW("[MMISRV]MMISRVAUD_SetAudioPlayStateByHandle, audid_state = %d", entity_ptr->audio.audio_state);
    return TRUE;
}


/*****************************************************************************/
//  Description : set the audio state to pause state 
//  Global resource dependence : 
//  Author:yanxian.zhou
//  Note: this function is only used in video player
//           
/*****************************************************************************/
PUBLIC BOOLEAN MMISRVAUD_SetAudioPauseStateByHandle(MMISRV_HANDLE_T audio_handle) 
{
    
	SRVAUD_ENTITY_INFO_T *entity_ptr = GetEntityAddrByHandle(audio_handle);
	
    if(PNULL == entity_ptr)
    {
        return FALSE;
    }
    entity_ptr->audio.audio_state= AUDIO_STATE_PAUSE;   
	SCI_TRACE_LOW("[MMISRV]MMISRVAUD_SetAudioPauseStateByHandle, audid_state = %d", entity_ptr->audio.audio_state);
    return TRUE;
}

/*****************************************************************************/
//  Description : Search drv handle by type.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only to search Video, ATV, CMMB etc. type, whitch only one entity at the same time.
//           These function is only used to distinguish the different callback.
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_SearchHandleByHandle(MMISRV_HANDLE_T handle)
{
    uint32 i = 0;
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;
    while(i < MMISRVAUD_SERVICE_NUM_MAX)
    {
        if(s_handle_box.entity_addr[i] != 0)
        {
            entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_handle_box.entity_addr[i];
            if(entity_ptr->handle == handle)
            {
                break;
            }
        }
        i++;
    }
    _DBG(    //MMISRV_TRACE_LOW:"[MMISRV]: Audio MMISRVAUD_SearchHandleByType() exit, handle%d, i=%d, entity_ptr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISRVAUD_4440_112_2_18_3_23_52_226,(uint8*)"ddd", handle, i, entity_ptr));
    
    if(i < MMISRVAUD_SERVICE_NUM_MAX && entity_ptr!= PNULL)
    {
        return entity_ptr->drv_handle;
    }
    else
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : MMISRVAUD_GetDecoderDataLen
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only to search Video, ATV, CMMB etc. type, whitch only one entity at the same time.
//           These function is only used to distinguish the different callback.
/*****************************************************************************/
PUBLIC uint32 MMISRVAUD_GetDecoderDataLen(MMISRV_HANDLE_T handle)
{
	return AUDIO_GetDecoderDataLen(MMISRVAUD_SearchHandleByHandle(handle));
}


#ifdef MMI_AUTOTEST_SUPPORT
/*****************************************************************************/
//  Description : Get audio type name.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL char *GetTypeName(MMISRVAUD_TYPE_E type)
{
    if(type <= MMISRVAUD_TYPE_MAX && type >= 0) /*lint !e568*/
    {
        return srv_type_name[type];/*lint !e605*/
    }
    else
    {
        return s_unknown_str;/*lint !e605*/
    }
}

/*****************************************************************************/
//  Description : Get audio state name.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL char *GetAudStateName(SRVAUD_AUDIO_STATE_E state)
{
    if(state < AUDIO_STATE_MAX && state >= 0)  /*lint !e568*/
    {
        return srv_aud_state_name[state];/*lint !e605*/
    }
    else
    {
        return s_unknown_str;/*lint !e605*/
    }
}

/*****************************************************************************/
//  Description : Get BT state name.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: 
/*****************************************************************************/
LOCAL char *GetBtStateName(SRV_BT_STATE_E state)
{
    if(state < BT_STATE_MAX && state >= 0)  /*lint !e568*/
    {
        return bt_state_name[state];/*lint !e605*/
    }
    else
    {
        return s_unknown_str;/*lint !e605*/
    }
}
#endif

/*****************************************************************************/
//  Description : printf audio service info.
//  Global resource dependence : 
//  Author:Yintang.Ren
//  Note: Only used to print the mmi audio service info when MS asserted.
/*****************************************************************************/
PUBLIC void MMISRVAUD_PrintInfo(void)
{
    uint i = 0;
    uint32 num = 0;
    SRVAUD_ENTITY_INFO_T *entity_ptr = PNULL;

    char text_buff[255] = {0};
    SRVAUD_ENTITY_SERVICE_INFO_T *srv_ptr = PNULL;
    SRVAUD_ENTITY_AUDIO_INFO_T *aud_ptr = PNULL;
    MMISRVAUD_TYPE_T *aud_info_ptr = PNULL;
    
    sprintf(text_buff, "mmi audio service info, total handle num:%d\n", s_handle_box.num);
    OS_DumpPrint(text_buff, strlen(text_buff));
    
    while(i < MMISRVAUD_SERVICE_NUM_MAX && num < s_handle_box.num)
    {
        if(s_handle_box.entity_addr[i] != 0)
        {
            entity_ptr = (SRVAUD_ENTITY_INFO_T *)s_handle_box.entity_addr[i];
            if(entity_ptr != PNULL)
            {
                sprintf(text_buff, "handle=0x%x, driver handle=0x%x, client name=%s", entity_ptr->handle, entity_ptr->drv_handle, entity_ptr->client_name);
                OS_DumpPrint(text_buff, strlen(text_buff));

                srv_ptr = &entity_ptr->service;
                sprintf(text_buff, "service: state=%d,pri=%d,notify=0x%x, is_auto_free=%d, is_auto_resume_off=%d", srv_ptr->srv_state, srv_ptr->pri, srv_ptr->notify, srv_ptr->is_auto_free, srv_ptr->is_auto_resume_off);/*lint !e626*/
                OS_DumpPrint(text_buff, strlen(text_buff));

                aud_ptr = &entity_ptr->audio;
                sprintf(text_buff,      
                    #ifdef MMI_AUTOTEST_SUPPORT
                    "aud:type=%s,state=%s,"
                    #else
                    "aud:type=%d,state=%d,"
                    #endif
                    "cur_route=%d,manul_route=%d,timer=%d", 
                    
                    #ifdef MMI_AUTOTEST_SUPPORT
                    GetTypeName(aud_ptr->audio_info.info.type), GetAudStateName(aud_ptr->audio_state),
                    #else
                    aud_ptr->audio_info.info.type, aud_ptr->audio_state, 
                    #endif
                    aud_ptr->cur_route, aud_ptr->manual_route, aud_ptr->timer);
                OS_DumpPrint(text_buff, strlen(text_buff));
                
                aud_info_ptr = &entity_ptr->audio.audio_info;
                sprintf(text_buff, "aud:all_support_route=%d,default_route=%d,volume=%d,volume_type=%d,duation=%d,play_times=%d\n",
                    aud_info_ptr->all_support_route, aud_info_ptr->default_route, aud_info_ptr->volume, aud_info_ptr->volume_type, aud_info_ptr->duation,aud_info_ptr->play_times);
                OS_DumpPrint(text_buff, strlen(text_buff));
            }
            num++;
        }
        i++;
    }    

    sprintf(text_buff, 
        #ifdef MMI_AUTOTEST_SUPPORT
        "BT:state=%s,"
        #else
        "BT:state=%d,"
        #endif
        "timer=%d,cur entity=0x%x,cur event=%d,wait entity=0x%x,wait event=%d\n", 
        
        #ifdef MMI_AUTOTEST_SUPPORT
        GetBtStateName(s_bt_info.state), 
        #else
        s_bt_info.state, 
        #endif
        s_bt_info.timer, s_bt_info.cur_event.entity_addr, s_bt_info.cur_event.event_data.event, s_bt_info.wait_event.entity_addr, s_bt_info.wait_event.event_data.event);
    OS_DumpPrint(text_buff, strlen(text_buff));

}

