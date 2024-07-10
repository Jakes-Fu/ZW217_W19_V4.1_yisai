#include "mmk_timer.h"
#include "mmk_app.h"
#include "mn_type.h"
#include "mmi_id.h"
#include "mmicc_export.h"
#if defined(__OD_SC6530_SWITCH__) || defined(__OD_SC6531_SWITCH__)
#ifdef BROWSER_SUPPORT
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#include "mmibrowser_export.h"
#endif
#include "mmikl_export.h"
#include "mmiset_export.h"
#else
#ifdef BROWSER_SUPPORT
#include "mmipdp_api.h"
#include "mmiconnection.h"
#include "mmibrowser_api.h"
#endif
#include "mmiset.h"
#include "mmi_keylock.h"
#endif
#include "socket_api.h"
#include "od_adapter_gprs.h"
#include "od_adapter_log.h"



OD_BOOL Od_IsGprsEnable(void)
{
#if WIN32
    return OD_TRUE;
#endif
    //电话
    if (MMIAPICC_IsInState(CC_IN_CALL_STATE))
    {
        OD_LOG("in call");
        return OD_FALSE;
    }
    if (MMIDEFAULT_IsPermitPlayNewMsgRing()==OD_FALSE) //CMMB播放中
    {
        OD_LOG("ring deny");
        return OD_FALSE;
    }
#ifdef BROWSER_SUPPORT
    if(MMIAPIBROWSER_IsActive())
    {
        OD_LOG("in browser");
        return OD_FALSE;
    }
#endif
    if(MMIAPISET_GetFlyMode())
    {
        OD_LOG("in flymode");
        return OD_FALSE;
    }

    return OD_TRUE;
}



OD_BOOL od_is_in_idle_screen(void)
{
    return (MMK_IsFocusWin(MAIN_IDLE_WIN_ID)
#ifdef MMI_KEY_LOCK_SUPPORT
#if defined(__OD_SC6530_SWITCH__) || defined(__OD_SC6531_SWITCH__)
            ||MMIAPIKL_IsPhoneLocked()
#else
            || MMIKL_IsPhoneLocked()
#endif
#endif
           );
}


