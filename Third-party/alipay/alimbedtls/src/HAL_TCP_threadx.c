
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "tcpip6/socket_api.h"
#include "mbedtls_import.h"
#include "mbedtls_import_dtls.h"
#include "tasks_id.h"
#include "dal_time.h"

static uint64_t _threadx_get_time_ms(void)
{
    return ALIPAY_MBEDTLS_SYMBOL(HAL_time)();
}

static uint64_t _threadx_time_left(uint64_t t_end, uint64_t t_now)
{
    uint64_t t_left;

    if (t_end > t_now) {
        t_left = t_end - t_now;
    } else {
        t_left = 0;
    }

    return t_left;
}

//modified by cheng.du of sprd
unsigned int  ALIPAY_MBEDTLS_SYMBOL(HAL_time)(void)
{
    SCI_TM_T realtime;
    uint32 msec = 0;
    TM_GetTime(&realtime);
    msec += (realtime.tm_year - 1970) * 12 * 30 * 24 * 60 * 60;
    msec += realtime.tm_mon * 30 * 24 * 60 * 60;
    msec += realtime.tm_mday* 24 * 60 * 60;
    msec += realtime.tm_hour * 60;
    msec += realtime.tm_min *60;
    msec += realtime.tm_sec;
    return msec;
}

//…Ë÷√net_id
static unsigned int tcp_net_id = 0;
void  HAL_TCP_set_net_id(unsigned int id) {
    //DTLS_TRC("set netid = %d",id);
    tcp_net_id = id;
}


static int tcp_need_read = 0;
int  HAL_TCP_get_need_read() {
    //DTLS_TRC("HAL_TCP_get_need_read = %d", tcp_need_read);
    return tcp_need_read;
}


int32_t HAL_TCP_Read(uintptr_t fd, char *buf, uint32_t len)
{
    int ret, left_len;
    //DTLS_TRC("HAL_TCP_Read begin, fd=%d, len = %d", fd, len );
    ret = sci_sock_asyncrecv( fd, buf, len, 0, &left_len);

    if( ret < 0 )
        return -2;

    if(left_len != 0){
        tcp_need_read = 1;
		//DTLS_TRC("!!!HAL_TCP_Read success, but left_len=%d", left_len);
		if(TCPIP_SOCKET_ERROR == sci_sock_asyncselect(fd,
		SCI_IdentifyThread(), AS_CONNECT|AS_CLOSE|AS_READ|AS_WRITE))
		{
                SCI_TRACE_LOW("asyncslect failed - %d", sci_sock_errno(fd));
             }
	} else {
	    tcp_need_read = 0;
    }

    //DTLS_TRC("HAL_TCP_Read success, ret=%d", ret);
    return ret;
}

