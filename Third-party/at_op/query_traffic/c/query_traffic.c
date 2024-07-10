/****************************************************************************
** File Name:      query_traffic.c                                              *                                                *
** Date:           2022/10/19                                              *
** Copyright:      2020 uinsoc, Incorporated. All Rights Reserved.         *
** Description:                                                            *
****************************************************************************/
/**------------------------------------------------------------------------*
 **                         Include Files                                  *
 **------------------------------------------------------------------------*/
/**------------------------------------------------------------------------*
 **                         Include Files                                  *
 **------------------------------------------------------------------------*/
#include "socket_api.h"
#include "socket_types.h"
#include "ssl_api.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "in_message.h"
#include "sci_api.h"

#define MAX_FLOW_RATE_BUF_LEN   100
#define MAX_RESULT_BUFFER_LEN       100
LOCAL char g_traffic[MAX_FLOW_RATE_BUF_LEN] = {'0'};

#define SIZE_UNIT       1024

extern PUBLIC BOOLEAN MMIAPIPDP_GetFlowrate(MMIPDP_FLOWRATE_T *flowrate_ptr, MN_DUAL_SYS_E dual_sys);
extern PUBLIC void MMIAPIPDP_ClearFlowrate(MN_DUAL_SYS_E dual_sys);

LOCAL void int_to_str(uint64 value, uint8 *buffer, uint32 len)
{
    uint32 i = 0;
    uint8 rem = 0;
    uint8 str_len;

    while (value / 10)
    {
        rem = value % 10;
        if (i < len)
        {
            buffer[i++] = '0' + rem;
        }

        value = value / 10;

        if (value < 10 && i < len)
        {
            buffer[i++] = '0' + value;
        }
    }

    if (i < len)
    {
        buffer[i] = 0;
    }
    else
    {
        buffer[len-1] = 0;
    }

    i = 0;
    str_len = strlen(buffer);
    while (i < str_len/2)
    {
        rem = buffer[i];
        buffer[i] = buffer[str_len - i - 1];
        buffer[str_len - i - 1] = rem;
        i++;
    }
}

PUBLIC int query_get_traffic(void)
{
    MMIPDP_FLOWRATE_T gprs_flowrate = {0};
    uint8 buf[MAX_FLOW_RATE_BUF_LEN] = {0};
    SCI_TRACE_LOW("[traffic] %s %d\n",__FUNCTION__,__LINE__);
    memset(g_traffic, 0, MAX_FLOW_RATE_BUF_LEN);
    
    MMIAPIPDP_GetFlowrate(&gprs_flowrate, 0);
    
    SCI_TRACE_LOW("[traffic] %s %d gprs_flowrate.total_recv is %d\n",__FUNCTION__,__LINE__, gprs_flowrate.total_recv);
    
    int_to_str(gprs_flowrate.total_recv, buf, MAX_FLOW_RATE_BUF_LEN);
    snprintf(g_traffic, MAX_FLOW_RATE_BUF_LEN, "sim recv:%s, ", buf);
    SCI_TRACE_LOW("[traffic] %s %d, total_recv:%s\n",__FUNCTION__,__LINE__, buf);

    int_to_str(gprs_flowrate.total_send, buf, MAX_FLOW_RATE_BUF_LEN);        
    snprintf(g_traffic + strlen(g_traffic), MAX_FLOW_RATE_BUF_LEN - strlen(g_traffic), "sim send:%s", buf);
    SCI_TRACE_LOW("[traffic] %s %d, total_send:%s\n",__FUNCTION__,__LINE__, buf);

    SCI_TRACE_LOW("[traffic] %s %d, g_traffic:%s\n",__FUNCTION__,__LINE__, g_traffic);
    return 0;
}

PUBLIC int query_clear_traffic(void)
{
    SCI_TRACE_LOW("[traffic] %s %d\n",__FUNCTION__,__LINE__);
    memset(g_traffic, 0, MAX_FLOW_RATE_BUF_LEN);
    //Todo clear sim flow rate
    MMIAPIPDP_ClearFlowrate(0);
    return 0;
}

PUBLIC int query_traffic_get_result(char *at_rsp)
{
    if (at_rsp == NULL)
    {
        return -1;
    }
    snprintf(at_rsp, MAX_RESULT_BUFFER_LEN, "%s", g_traffic);
    SCI_TRACE_LOW("[traffic]%s %d g_traffic:%s\n",__FUNCTION__,__LINE__, g_traffic);
    return 0;
}

