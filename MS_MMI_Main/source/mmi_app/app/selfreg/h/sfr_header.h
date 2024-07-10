#ifndef __SFR_HEADER_H__
#define __SFR_HEADER_H__

typedef struct _SFR_HTTP_CONTEXT
{
    HTTP_APP_MODULE_ID_T module_id;
    HTTP_CONTEXT_ID_T    context_id;
    HTTP_REQUEST_ID_T    request_id;
    HTTP_APP_INSTANCE_T  app_instance;
} SFR_HTTP_CONTEXT;

#define SFR_HTTP_APPFLAG (0xca99)

typedef struct _SFR_LINKER
{
    BOOLEAN flags;
    MN_DUAL_SYS_E sim_sys;
    uint32 net_id;
    SFR_HTTP_CONTEXT http_ctx;
    SFR_CARRIER *carrier;
    int http_return;
} SFR_LINKER;

extern SFR_LINKER g_sfr_linker;

#define SFR_ICCID_LENGTH (20)

typedef struct _SFR_ICCD
{
    char num[SFR_ICCID_LENGTH];
} SFR_ICCID;

PUBLIC MMI_RESULT_E SlfReg_HandleHttpMsg(PWND app_ptr, uint16 msg_id, DPARAM param, SFR_HTTP_CONTEXT *http_context_ptr);

#endif // __SFR_HEADER_H__
