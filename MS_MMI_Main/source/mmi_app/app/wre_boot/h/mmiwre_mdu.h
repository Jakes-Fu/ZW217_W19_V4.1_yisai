#ifndef  _MMIWRE_MDU_H_
#define  _MMIWRE_MDU_H_

#if 0

typedef enum
{
    WRE_MODULE_DIAL,
    WRE_MODULE_FILE,
    WRE_MODULE_SEMA,
    WRE_MODULE_SOCKET,
    WRE_MODULE_THREAD,
    WRE_MODULE_TIMER,
    WRE_MODULE_OTHER,
    WRE_MODULE_STDLIB,
    WRE_MODULE_MAX
}ADAPT_M_TO_W_MODULE_E;

typedef enum
{
    MMIAPICOM_OtherTaskToMMI_FUNC,
    MMK_CreateWindow_FUNC,
    MMK_GetFirstAppletHandle_FUNC,
    MMK_CloseWin_FUNC,
    MMK_GetWinAddDataPtr_FUNC,
    OTHER_FUNC_MAX
}OTHER_M_TO_W_FUNC_E;

typedef enum
{
    MMIAPIPDP_Active_FUNC,
    MMIAPIPDP_Deactive_FUNC,
    MMIAPIWIFI_OpenListWin_FUNC,
    MMIAPIWIFI_GetNetId_FUNC,
    MMIAPIWIFI_GetStatus_FUNC,
    DIAL_FUNC_MAX
}DIAL_M_TO_W_FUNC_E;

typedef enum
{
    FILE_FUNC_MAX
}FILE_M_TO_W_FUNC_E;

typedef enum
{
    SEMA_FUNC_MAX
}SEMA_M_TO_W_FUNC_E;

typedef enum
{
    SOCKET_FUNC_MAX
}SOCKET_M_TO_W_FUNC_E;

typedef enum
{
    THREAD_FUNC_MAX
}THREAD_M_TO_W_FUNC_E;

typedef enum
{
    TIMER_FUNC_MAX
}TIMER_M_TO_W_FUNC_E;

typedef enum
{
    memmove_FUNC,
    memset_FUNC,
    strcpy_FUNC,
    STDLIB_FUNC_MAX
}STDLIB_M_TO_W_FUNC_E;

#endif

typedef enum
{
    DispatchMwinMSG_FUNC,
    GetMwinMessageForMocor_FUNC,
    MwinWin_HandleMsg_FUNC,
    MWIN_XXX_PostMessage_FUNC,
    _vsnwprintf_FUNC,
    ClearThreadNormalQueue_FUNC,
    GetWreCurMwinThd_FUNC,
    IsMwinQuitMsg_FUNC,
    SetMwinMsg_FUNC,
    MWIN_XXX_HandleSocketMsg_FUNC,
    MWIN_GetAsyncTaskId_FUNC,
    Wifi_PostMessage_FUNC,
    MSG_USB_Handle_FUNC,
    MSG_Charge_Handle_FUNC,
    WRE_SmsReceiveCallBack_FUNC,
    WRE_SendSmsCallback_FUNC,
    WRE_SetDeepSleepStatus_FUNC,
     WRE_RegEventNotify_FUNC,
    WRE_LostFocusIsPenUped_FUNC,
    WRE_MiniBk_FUNCTBL,
    WRE_LD_GetMessage_FUNC,
    WRE_TO_MOCOR_MALLOC_FUNC,
    WRE_TO_MOCOR_FREE_FUNC,
    WRE_PowerOff_FUNC,
    WRE_TO_MOCOR_FUNC_MAX    
}ADAPT_W_TO_M_FUNC_E;

#endif
