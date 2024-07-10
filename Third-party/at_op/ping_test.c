/****************************************************************************
** File Name:      ping_test.c                                              *                                                *
** Date:           2020/09/03                                              *
** Copyright:      2020 uinsoc, Incorporated. All Rights Reserved.         *
** Description:                                                            *
****************************************************************************/
/**------------------------------------------------------------------------*
 **                         Include Files                                  *
 **------------------------------------------------------------------------*/
#include "app_tcp_if.h"


 #define PING_RETRY_TIMES        1   /* ping retry times */
LOCAL char* g_addr_ptr = PNULL;
LOCAL TCPIP_PING_HANDLE g_ping_hdl = 0;
LOCAL int32 s_ping_send_times = 0;
LOCAL int32 s_ping_recv_times = 0;
LOCAL uint8 link_id = 0;
LOCAL uint32 s_ping_times = 0;
/*****************************************************************************/
//  Description : This function handle the AT+TESTPING command
//                     used for ping test.
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC int32 AT_PingStop(void);

PUBLIC void pingcallback(
        int32 res,/* ping result, 0 - succeed; other - fail */
        uint32 time_delay,/* ping time delay, only valid when success, unit: ms */
        TCPIP_PING_HANDLE ping_handle,/* ping handle */
        uint8 ttl,
        char* ipaddr,
        uint8 time_is_out)
{
    uint8* resp_data_ptr = NULL;

    resp_data_ptr = SCI_ALLOC_APPZ(500);
    if (NULL == resp_data_ptr)
    {
        SCI_TRACE_LOW("ping_callback alloc faile");
        return;
    }

    SCI_TRACE_LOW("ping_callback res: %d, time_delay: %d, ping_handle: %d, ttl: %d, ipaddr: %s, s_ping_send_times: %d, s_ping_recv_times: %d, s_ping_times:%d, time_is_out:%d",
        res, time_delay, ping_handle, ttl, ipaddr, s_ping_send_times, s_ping_recv_times, s_ping_times, !time_is_out);
    switch (res)
    {
    case PINGRES_SUCCESS:
        s_ping_send_times++;
        s_ping_recv_times++;
        if (resp_data_ptr)
        {
            snprintf(resp_data_ptr, 500, "%s %s %s %s%d %s%d\r\n","From",ipaddr, "response:", "time=", time_delay, "TTL=",ttl);
#ifdef PPP_USB
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr),ATC_GetSioPort());
#else
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr));
#endif
        }
        break;
    case PINGRES_DNS_TIMEOUT:
        if (resp_data_ptr && g_addr_ptr)
        {
            snprintf(resp_data_ptr, 500, "%s%s\r\n","Can not find the host: ",g_addr_ptr);
#ifdef PPP_USB
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr),ATC_GetSioPort());
#else
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr));
#endif
        }
        break;
    case PINGRES_DNS_ERROR:
        if (resp_data_ptr && g_addr_ptr)
        {
            snprintf(resp_data_ptr, 500, "%s%s\r\n","Can not find the host: ",g_addr_ptr);
#ifdef PPP_USB
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr),ATC_GetSioPort());
#else
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr));
#endif
        }
        break;
    case PINGRES_ICMP_TIMEOUT:
        s_ping_send_times++;
        if (resp_data_ptr)
        {
            snprintf(resp_data_ptr, 500, "%s\r\n","Reqeust timeout");
#ifdef PPP_USB
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr),ATC_GetSioPort());
#else
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr));
#endif
        }
        break;
    case PINGRES_ICMP_ERROR:
        if (resp_data_ptr)
        {
            snprintf(resp_data_ptr, 500, "%s\r\n","Reqeust error");
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr));
        }
        break;
    case PINGRES_TIMING_END:
        break;
    default:
        if (resp_data_ptr)
        {
            snprintf(resp_data_ptr, 500, "\r\n%s\r\n","Other error");
#ifdef PPP_USB
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr),ATC_GetSioPort());
#else
            SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr));
#endif
        }
        break;
    }

    if (s_ping_send_times >= s_ping_times&& !time_is_out || s_ping_send_times == 0 || res == PINGRES_ICMP_ERROR )
    {
        snprintf(resp_data_ptr, 500, "    %s%d%s%d\r\n","Ping result infomation: send=",s_ping_send_times, ",receive=", s_ping_recv_times);
#ifdef PPP_USB
        SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr),ATC_GetSioPort());
#else
        SIO_ATC_WriteCmdRes(resp_data_ptr, strlen((char*)resp_data_ptr));
#endif
        AT_PingStop();
    }

    if (resp_data_ptr)
        SCI_FREE(resp_data_ptr);
}

PUBLIC int32 AT_PingStart(
    char* ipevr,
    char* host_name,
    TCPIP_PING_OPTION_PARAM_T optvalue,
    uint32          data_size,
    uint8          s_link_id,
    uint32        netid,
    uint32          times)
{
    uint32 ipver = 0;

    link_id = s_link_id;
    SCI_TRACE_LOW("testping: link_id=%d----->AT_PingStart\r\n",link_id);
    if (ipevr == NULL || host_name == NULL || data_size == 0)
    {
        SCI_TRACE_LOW("ping test input param error");
        pingcallback(PINGRES_ICMP_ERROR + 1, 0, 0, 0, 0,0);
        return -1;
    }

    if (s_ping_send_times != 0 || s_ping_recv_times != 0 || g_ping_hdl != 0 || g_addr_ptr != NULL)
    {
        SCI_TRACE_LOW("ping test prev test has not stoped");
        pingcallback(PINGRES_ICMP_ERROR + 1, 0, 0, 0, 0,0);
        return -1;
    }

    if( optvalue.spend_time == 0 )
    {
        s_ping_times = ( 0 == times )?
                        PING_RETRY_TIMES : times;
    }
    else
        s_ping_times = PING_RETRY_TIMES;

    g_addr_ptr = SCI_ALLOC(data_size + 1);

    if (g_addr_ptr)
    {
        SCI_MEMSET(g_addr_ptr, 0, data_size + 1);
        if (strncasecmp(ipevr, "V4", 2) == 0)
        {
            ipver = 4;
        }
        else if (strncasecmp(ipevr, "V6", 2) == 0)
        {
            ipver = 6;
        }
        SCI_TRACE_LOW("ping test  ip version:%s, ipver=%d\n", ipevr, ipver);

        SCI_MEMCPY(g_addr_ptr, host_name, data_size);
        SCI_TRACE_LOW("ping test  host:%s, g_addr_ptr=%s\n", host_name, g_addr_ptr);
        
        if (ipver == 4)
        {
            SCI_TRACE_LOW("ping test V4 datalen: %d, except spend time: %d s, g_addr_ptr: %s\n",optvalue.data_len, optvalue.spend_time, g_addr_ptr);
            g_ping_hdl = sci_ping4_request(g_addr_ptr, optvalue, 0, times,  (TCPIP_PING_CALLBACK_EX_FPTR) pingcallback, netid);
            SCI_TRACE_LOW("testping: sci_ping4_request end");
        }
        else if (ipver == 6)
        {
            SCI_TRACE_LOW("ping test V6 datalen: %d, except spend time: %d s, g_addr_ptr: %s\n",optvalue.data_len, optvalue.spend_time, g_addr_ptr);
            g_ping_hdl = sci_ping6_request(g_addr_ptr, optvalue, 0, times, pingcallback, netid);
        }

        if ((0 == g_ping_hdl) || (ipver == 0))
        {
            pingcallback(PINGRES_ICMP_ERROR + 1, 0, 0, 0, 0, 0);
        }

        optvalue.data_len = 0;
        return 0;
    }
    else
    {
        pingcallback(PINGRES_ICMP_ERROR + 1, 0, 0, 0, 0, 0);
        return -1;
    }
}

PUBLIC int32 AT_PingStop(void)
{
    SCI_TRACE_LOW("ping test PingTestStop");
    sci_ping_cancel(g_ping_hdl);
    if (g_addr_ptr)
    {
        SCI_FREE(g_addr_ptr);
        g_addr_ptr = PNULL;
    }
    g_ping_hdl = 0;
    s_ping_send_times = 0;
    s_ping_recv_times = 0;

    return 0;
}

