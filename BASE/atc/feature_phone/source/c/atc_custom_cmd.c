
#include "atc.h"

#include "atc_custom_cmd.h"

extern MN_DUAL_SYS_E       g_current_card_id;

AT_CMD_FUNC(ATC_ProcessGLPDNS)
{
#if defined(_ATC_UIX8910_ENABLE_)
    ATC_STATUS     status = S_ATC_DEFAULT_HANDLE;
    MN_DUAL_SYS_E   dual_sys = ATC_CURRENT_SIM_ID_MUX;
    uint8 nCid = 0;
    uint32 result = 0;

    SCI_TRACE_LOW("ATC_ProcessGLPDNS ATC_GET_CMD_TYPE: %d", ATC_GET_CMD_TYPE);
    switch (ATC_GET_CMD_TYPE)
    {
        case ATC_CMD_TYPE_SET:
            if (PARAM1_FLAG)
            {
                nCid =  PARA1.num;
            }

            if (PARAM2_FLAG)
            {
                dual_sys =  PARA2.num;
            }

            result = CFW_GprsIpDnsInfo(nCid, dual_sys);
            SCI_TRACE_LOW("ATC_ProcessGLPDNS!! result: %d", result);
            if (result != 0)
            {
                status = S_ATC_FAIL;
                break;
            }
            status = S_ATC_SUCCESS;
            break;
        case ATC_CMD_TYPE_EXECUTE:
            CFW_GprsClrIpDns();
            status = S_ATC_SUCCESS;
            break;
        default:
            SCI_TRACE_LOW("ATC_ProcessGLPDNS error!!");
             status = S_ATC_FAIL;
        break;
    }

    return status;
#else
    return S_ATC_FAIL;
#endif
}