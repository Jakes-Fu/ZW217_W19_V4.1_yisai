
#include "zfb_app.h"
#include "alipay_kal.h"
#include "production_test.h"
#include "tcpip_types.h"
#include "time.h"

#define ZFB_MAX_SOCKET_NUM  ALIPAY_IOT_MAX_IP_SOCKET_NUM

#define	SOC_FD_SET(s, p)            ((p)->fds_bits[s] |= 0x01)
#define   SOC_FD_WANT(s, p)    ((p)->fds_bits[s] & 0x01)
#define   SOC_FD_SET_READY(s, p)   ((p)->fds_bits[s] |= 0x02)
/*
 * <GROUP  Macro_Funcs>
 * 
 * clear the socket id which you don't want to select 
 */
#define	SOC_FD_CLR(s, p)            ((p)->fds_bits[s] &= ~(0x01))

/*
 * <GROUP  Macro_Funcs>
 * 
 * check if the socket id is selected 
 */
#define	SOC_FD_ISSET(s, p)          ((p)->fds_bits[s] & 0x02)

/*
 * <GROUP  Macro_Funcs>
 *  
 * clear the select set 
 */
#define	SOC_FD_ZERO(p)              SCI_MEMSET(p, 0x00, sizeof(alipay_iot_fd_set))

/* socket linger timeout structure */
typedef struct
{
    BOOLEAN   onoff; /* KAL_TRUE: enable linger timeout KAL_FALSE: disable linger timeout */
    uint16 linger_time; /* linger timeout (unit in seconds) */
} soc_linger_struct;

struct linger 
{
	int	 l_onoff;            // option on/off
	int	 l_linger;           // linger time
};

struct sadb_x_policy {
  uint16 sadb_x_policy_len;
  uint16 sadb_x_policy_exttype;
  uint16 sadb_x_policy_type;		/* See policy type of ipsec.h */
  uint8 sadb_x_policy_dir;		/* direction, see ipsec.h */
  uint8 sadb_x_policy_reserved;
  uint32 sadb_x_policy_id;
  uint32 sadb_x_policy_reserved2;
};

alipay_iot_log_lvl_t g_cur_alipay_log_level = ALIPAY_IOT_LOG_LVL_INF;

typedef struct _ZFB_SOCKET_TYPE_T
{
    int domain;
    int type;
    int protocol;
    TCPIP_SOCKET_T sockfd;
}ZFB_SOCKET_TYPE_T;

static ZFB_SOCKET_TYPE_T sockfd_arry[ZFB_MAX_SOCKET_NUM] = 
{
    {0,0,0,TCPIP_SOCKET_INVALID}
#if 0
    {0,0,0,TCPIP_SOCKET_INVALID},
    {0,0,0,TCPIP_SOCKET_INVALID},
    {0,0,0,TCPIP_SOCKET_INVALID},
    {0,0,0,TCPIP_SOCKET_INVALID},
    {0,0,0,TCPIP_SOCKET_INVALID},
    {0,0,0,TCPIP_SOCKET_INVALID},
    {0,0,0,TCPIP_SOCKET_INVALID},
#endif
};

const char * zdt_iot_soc_opt[]=
{
    "SOC_OOBINLINE",
    "SOC_LINGER",
    "SOC_NBIO",
    "SOC_ASYNC",
    "SOC_NODELAY",
    "SOC_KEEPALIVE",
    "SOC_RCVBUF",
    "SOC_SENDBUF",
    "SOC_NREAD",
    "SOC_PKT_SIZE",
    "SOC_SILENT_LISTEN",
    "SOC_QOS",
    "SOC_TCP_MAXSEG",
    "SOC_IP_TTL",
    "SOC_LISTEN_BEARER",
    "SOC_UDP_ANY_FPORT",
    "SOC_WIFI_NOWAKEUP",
    "SOC_UDP_NEED_ICMP",
    "SOC_IP_HDRINCL",
    "SOC_IPSEC_POLICY",
    "SOC_TCP_ACKED_DATA",
    "SOC_TCP_DELAYED_ACK",
    "SOC_TCP_SACK",
    "SOC_TCP_TIME_STAMP",
    "SOC_TCP_ACK_MSEG",
};

struct sci_sockaddr sockfd_dest_addr[ZFB_MAX_SOCKET_NUM] = {0};
static struct linger s_optset_val_linger = {0};
static int s_optset_val_nodelay = 0;
extern int zfb_sockfd_add(int domain,int type,int protocol,TCPIP_SOCKET_T sockfd)
{
    int fd = -1;
    uint8 i = 0;
    
    for(i = 0; i < ZFB_MAX_SOCKET_NUM; i++)
    {
        if(sockfd_arry[i].sockfd == sockfd)
        {
            return i;
        }
    }
    for(i = 0; i < ZFB_MAX_SOCKET_NUM; i++)
    {
        if(sockfd_arry[i].sockfd == TCPIP_SOCKET_INVALID)
        {
            sockfd_arry[i].domain = domain;
            sockfd_arry[i].type = type;
            sockfd_arry[i].protocol = protocol;
            sockfd_arry[i].sockfd = sockfd;
            fd = i;
            break;
        }
    }
    return fd;
}

extern TCPIP_SOCKET_T zfb_sockfd_get(int fd)
{
    if(fd < 0 || fd >= ZFB_MAX_SOCKET_NUM)
    {
        SCI_TRACE_LOW("zfb_sockfd_get ERR fd =%d",fd);
        return -1;

    }
    return sockfd_arry[fd].sockfd;
}

extern int zfb_sockfd_type_get(int fd)
{
    if(fd < 0 || fd >= ZFB_MAX_SOCKET_NUM)
    {
        SCI_TRACE_LOW("zfb_sockfd_type_get ERR fd =%d",fd);
        return -1;

    }
    return sockfd_arry[fd].type;
}

extern int zfb_sockfd_domain_get(int fd)
{
    if(fd < 0 || fd >= ZFB_MAX_SOCKET_NUM)
    {
        SCI_TRACE_LOW("zfb_sockfd_domain_get ERR fd =%d",fd);
        return -1;
    }
    return sockfd_arry[fd].domain;
}

extern int zfb_sockfd_protocol_get(int fd)
{
    if(fd < 0 || fd >= ZFB_MAX_SOCKET_NUM)
    {
        SCI_TRACE_LOW("zfb_sockfd_protocol_get ERR fd =%d",fd);
        return -1;

    }
    return sockfd_arry[fd].protocol;
}

extern ZFB_SOCKET_TYPE_T * zfb_sockfd_all_get(int fd)
{
    if(fd < 0 || fd >= ZFB_MAX_SOCKET_NUM)
    {
        SCI_TRACE_LOW("zfb_sockfd_all_get ERR fd =%d",fd);
        return NULL;
    }
    return &sockfd_arry[fd];
}

extern void zfb_sockfd_set(int fd, TCPIP_SOCKET_T sockfd)
{
    if(fd < 0 || fd >= ZFB_MAX_SOCKET_NUM)
    {
        SCI_TRACE_LOW("zfb_sockfd_set ERR fd =%d,sockfd=0x%x",fd,sockfd);
        return;
    }
    sockfd_arry[fd].sockfd = sockfd;
    return;
}

static uint32 OptConvert(int iot_val)
{
	switch (iot_val)
	{
        	case SOC_OOBINLINE:
        		return SO_OOBINLINE;
                
        	case SOC_LINGER:
        		return SO_LINGER;
                
        	case SOC_NBIO:
        		return SO_NBIO;
                
        	case SOC_ASYNC:
        		return SS_ASYNC;
                
        	case SOC_NODELAY:
        		return TCP_NODELAY;
                
        	case SOC_KEEPALIVE:
        		return SO_KEEPALIVE;
                
        	case SOC_RCVBUF:
        		return SO_RCVBUF;
                
        	case SOC_SENDBUF:
        		return SO_SNDBUF;
                
        	case SOC_NREAD:
        		return SO_RXDATA;
                
        	case SOC_PKT_SIZE:
        		return SO_MAXMSG;
                
        	case SOC_SILENT_LISTEN:
        		return 0;
                
        	case SOC_QOS:
        		return 0;
                
        	case SOC_TCP_MAXSEG:
        		return TCP_MAXSEG;
                
        	case SOC_IP_TTL:
        		return 0;
                
        	case SOC_LISTEN_BEARER:
        		return 0;
                
        	case SOC_UDP_ANY_FPORT:
        		return 0;
                
        	case SOC_WIFI_NOWAKEUP:
        		return 0;
                
        	case SOC_UDP_NEED_ICMP:
        		return 0;
                
              case SOC_IP_HDRINCL:            /* no file table ref any more */
                  return 0;
                  
              case SOC_IPSEC_POLICY:            
                  return 0;
                  
              case SOC_TCP_ACKED_DATA:       /* socket connected to a peer */
                  return TCP_PEERACKED;
                  
              case SOC_TCP_DELAYED_ACK:       /* socket connected to a peer */
                  return TCP_ACKDELAYTIME;
                  
              case SOC_TCP_SACK:      /* in process of connecting to peer */
                  return SO_TCPSACK;
                  
              case SOC_TCP_TIME_STAMP:   /*  in process  of disconnecting */
                  return SO_TIMESTAMP;
                  
              case SOC_TCP_ACK_MSEG:      /* can't send more data to peer */
                  return 0;
                  
              default:
                  return;
	}
      return 0;
}

/**
 * 设置套接口的参数。
 * level无效。
 * optname取值如下之一：
 *     SOC_OOBINLINE，目前不支持
 *     SOC_LINGER，opval指向soc_linger_struct
 *     SOC_NBIO，opval指向kal_bool
 *     SOC_ASYNC，opval指向kal_uint8
 *     SOC_NODELAY，opval指向kal_bool
 *     SOC_KEEPALIVE，opval指向kal_bool
 *     SOC_RCVBUF，opval指向kal_uint32
 *     SOC_SENDBUF，opval指向kal_uint32
 *     SOC_PKT_SIZE，opval指向kal_uint32
 *     SOC_SILENT_LISTEN，opval指向kal_bool
 *     SOC_QOS，opval指向kal_uint8
 *     SOC_TCP_MAXSEG，opval指向kal_uint8
 *     SOC_IP_TTL，opval指向kal_uint8
 *     SOC_LISTEN_BEARER，opval指向kal_uint8
 *     SOC_UDP_ANY_FPORT，opval指向kal_bool
 *     SOC_WIFI_NOWAKEUP，opval指向kal_bool
 *     SOC_UDP_NEED_ICMP，opval指向kal_bool
 *     SOC_IP_HDRINCL，opval指向kal_bool
 *     SOC_IPSEC_POLICY，opval指向sadb_x_policy
 *     SOC_TCP_ACKED_DATA，opval指向kal_uint32
 *     SOC_TCP_DELAYED_ACK，opval指向kal_bool
 *     SOC_TCP_SACK，opval指向kal_bool
 *     SOC_TCP_TIME_STAMP，opval指向kal_bool
 *     SOC_TCP_ACK_MSEG，opval指向kal_bool
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
static void * OptSetVal_Convert(int optname, void * opt_val)
{
    uint8 cur_8_val = 0;
    uint32 cur_32_val = 0;
    soc_linger_struct linger_val = {0};
    struct sadb_x_policy policy_val = {0};
    if(opt_val == NULL)
    {
        return NULL;
    }
	switch (optname)
	{
        	case SOC_OOBINLINE:
             {
                  cur_8_val = *((uint8 *)opt_val);
                  //SCI_TRACE_LOW("zfb OptValSet_Print SOC_OOBINLINE val=%d",cur_8_val);
                  break;
        	}
                
        	case SOC_LINGER:
             {
                   linger_val = *((soc_linger_struct *)opt_val);
                   s_optset_val_linger.l_onoff = linger_val.onoff;
                   s_optset_val_linger.l_linger = linger_val.linger_time;
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_LINGER onoff=%d,linger_time=%d",linger_val.onoff,linger_val.linger_time);
        		return (void *)&s_optset_val_linger;
        	}
                
        	case SOC_NBIO:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_NBIO val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_ASYNC:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_ASYNC val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_NODELAY:
             {
                  cur_8_val = *((uint8 *)opt_val);
                  s_optset_val_nodelay = cur_8_val;
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_NODELAY val=%d",cur_8_val);
        		return (void *)&s_optset_val_nodelay;
        	}
                
        	case SOC_KEEPALIVE:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_KEEPALIVE val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_RCVBUF:
             {
                  cur_32_val = *((uint32 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_RCVBUF val=%d",cur_32_val);
        		break;
        	}
                
        	case SOC_SENDBUF:
             {
                  cur_32_val = *((uint32 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_SENDBUF val=%d",cur_32_val);
        		break;
        	}
                
        	case SOC_NREAD:
             {
                  cur_32_val = *((uint32 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_NREAD val=%d",cur_32_val);
        		break;
        	}
                
        	case SOC_PKT_SIZE:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_PKT_SIZE val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_SILENT_LISTEN:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_SILENT_LISTEN val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_QOS:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_QOS val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_TCP_MAXSEG:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_TCP_MAXSEG val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_IP_TTL:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_IP_TTL val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_LISTEN_BEARER:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_LISTEN_BEARER val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_UDP_ANY_FPORT:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_UDP_ANY_FPORT val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_WIFI_NOWAKEUP:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_WIFI_NOWAKEUP val=%d",cur_8_val);
        		break;
        	}
                
        	case SOC_UDP_NEED_ICMP:
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_UDP_NEED_ICMP val=%d",cur_8_val);
        		break;
        	}
                
              case SOC_IP_HDRINCL:            /* no file table ref any more */
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_IP_HDRINCL val=%d",cur_8_val);
        		break;
        	}
              
              case SOC_IPSEC_POLICY:            
             {
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_IPSEC_POLICY");
        		break;
        	}
                  
              case SOC_TCP_ACKED_DATA:       /* socket connected to a peer */
             {
                  cur_32_val = *((uint32 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_TCP_ACKED_DATA val=%d",cur_32_val);
        		break;
        	}
                  
              case SOC_TCP_DELAYED_ACK:       /* socket connected to a peer */
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_TCP_DELAYED_ACK val=%d",cur_8_val);
        		break;
        	}
                  
              case SOC_TCP_SACK:      /* in process of connecting to peer */
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_TCP_SACK val=%d",cur_8_val);
        		break;
        	}
                  
              case SOC_TCP_TIME_STAMP:   /*  in process  of disconnecting */
             {
                  cur_8_val = *((uint8 *)opt_val);
                   //SCI_TRACE_LOW("zfb OptValSet_Print SOC_TCP_TIME_STAMP val=%d",cur_8_val);
        		break;
        	}
                  
              case SOC_TCP_ACK_MSEG:      /* can't send more data to peer */
             {
                    cur_8_val = *((uint8 *)opt_val);
                    //SCI_TRACE_LOW("zfb OptValSet_Print SOC_TCP_ACK_MSEG val=%d",cur_8_val);
        		break;
        	}
                  
              default:
                  break;;
	}
      return NULL;
}
extern int zfb_sockfd_opt_convert(int optname)
{
    int ret = 0;
    uint8 i = 0;
    uint32 fd_val = 0;
    uint32 tmp_val = 0;
    
    //SCI_TRACE_LOW("zfb_sockfd_opt_convert optname=0x%x",optname);
    
    fd_val = optname;
    for(i = 0; i < 24; i++)
    {
        tmp_val = (fd_val >> i) & 0x00000001;
        if(tmp_val)
        {
            //SCI_TRACE_LOW("zfb: %s",zdt_iot_soc_opt[i]);
            ret |= OptConvert((0x00000001 << i));
        }
    }
    return ret;
}

extern int alipay_iot_get_module_sn(char *inOut, size_t size)
{
    PRODUCTION_TEST_INFO_T s_mmieng_product = {0};
    char*           temp_ptr = NULL;
    int          temp_len = -1;
    uint8 text_buf[101] = {0};
    
    if (inOut == NULL) {
        return -1;
    }

    #ifdef WIN32
        temp_len = 10;
        if(temp_len > size)
        {
            temp_len = size;
        }
        SCI_MEMCPY(inOut,"YS8910FFZW",temp_len);
    #else
    if (SCI_SUCCESS == SCI_GetProductionTestInfo(&s_mmieng_product))
    {
        temp_len = strlen(s_mmieng_product.serial_num);
        if(temp_len > 0)
        {
            if(temp_len > size)
            {
                temp_len = size;
            }
            SCI_MEMCPY(inOut,s_mmieng_product.serial_num,temp_len);
        }
        else
        {
            temp_len = 10;
            if(temp_len > size)
            {
                temp_len = size;
            }
            SCI_MEMCPY(inOut,"YS8910FFZW",temp_len);
        }
    }
    #endif
    return temp_len;
}

/**
* 获取一个随机数
*/
extern int alipay_iot_rand(void)
{
    static int ali_rand_offset = 16345;
    int radom = 0;
    srand(SCI_GetTickCount()+ali_rand_offset);
    ali_rand_offset++;
    radom = rand();
    //SCI_TRACE_LOW("alipay_iot_rand radom=%d",radom);
    return radom;
}

/**
* 设定随机数种子
*/
extern void alipay_iot_srand(unsigned int seed)
{
    //SCI_TRACE_LOW("alipay_iot_srand seed=%d",seed);
    srand(seed);
    return;
}



/*
* @brief: 获取UTC时间
* note:  1.精确级别,微妙级别
*        2.受系统时间修改影响
*        3.返回的秒数是从1970年1月1日0时0分0秒开始
*/
#ifdef WIN32
extern void alipay_iot_gettimeofday(alipay_iot_timeval *now)
{
    //char tmp_buf[200] = {0};
    long cur_s = 0;
    long cur_us = 0;
    int64 temp = 0;
    int64  cur_ms = ZFBGetSystemTimeMS();
    temp = cur_ms/1000;
    cur_s = temp;
    temp = cur_ms%1000;
    temp = temp*1000;
    cur_us = temp;
    now->tv_sec = cur_s;
    now->tv_usec = cur_us;
    //sprintf(tmp_buf,"alipay_iot_gettimeofday cur_ms=%lld",cur_ms);
    //SCI_TRACE_LOW(tmp_buf);
    //SCI_TRACE_LOW("alipay_iot_gettimeofday3 tv_sec=%ld,tv_usec=%ld",now->tv_sec,now->tv_usec);
    return;
}
#else
extern void alipay_iot_gettimeofday(alipay_iot_timeval *now)
{
#ifdef ALIPAY_DEBUG
    char tmp_buf[200] = {0};
#endif
    long cur_s = 0;
    long cur_us = 0;
    long long temp = 0;
    long long  cur_ms = ZFBGetSystemTimeMS();
    temp = cur_ms/1000;
    cur_s = temp;
    temp = cur_ms%1000;
    temp = temp*1000;
    cur_us = temp;
    now->tv_sec = cur_s;
    now->tv_usec = cur_us;
#ifdef ALIPAY_DEBUG
    sprintf(tmp_buf,"alipay_iot_gettimeofday cur_ms=%lld",cur_ms);
    SCI_TRACE_LOW(tmp_buf);
    SCI_TRACE_LOW("alipay_iot_gettimeofday tv_sec=%ld,tv_usec=%ld",now->tv_sec,now->tv_usec);
#endif
    return;
}
#endif

/*
* 获取系统当前时间
* month 为1表示1月，day 为1表示1号，year为2018表示2018年。
*/
extern void alipay_iot_get_local_time(alipay_iot_local_time *time)
    {
    SCI_DATE_T dt;
    SCI_TIME_T tm;
    ERR_TM_E tError = 0, dError = 0;

    tError = TM_GetSysTime(&tm);
    dError = TM_GetSysDate(&dt);

    if(tError == ERR_TM_NONE && dError == ERR_TM_NONE)
    {
        time->day = dt.mday;
        time->month = dt.mon;
        time->year = dt.year;
        time->hour = tm.hour;
        time->minute = tm.min;
        time->second = tm.sec;
        //time->isdst = 
#ifdef ALIPAY_DEBUG
        SCI_TRACE_LOW("alipay_iot_get_local_time nDay=%d, nMonth=%d, nYear=%d, hour=%d,min=%d,sec=%d",dt.mday,
        		dt.mon, dt.year, tm.hour, tm.min, tm.sec );
#endif
    }

    }


/*
 * @brief: 获取系统从启动到现在所花的时间(毫秒)
 * @return: 0 获取失败
 *          非0 获取成功，值是获取系统从启动到现在所花的时间(毫秒)
 */
extern uint32_t alipay_iot_get_bootup_time(void)
{
    return SCI_GetTickCount();
}


/*
 * @brief: 申请一块连续的指定大小的内存块
 * @input: size 内存块的大小，单位字节
 * @return: 非NULL 被分配内存的指针(此存储区中的初始值不确定)
 *			NULL 分配失败
 */
extern void *alipay_iot_malloc(size_t size)
{
    return SCI_ALLOC_APP(size);
}


/*
 * @brief: 在内存的动态存储区中分配nmemb个长度为size的连续空间
 * @input: nmemb 内存块的个数
 * @input: size 每个内存块的大小，单位字节
 * @return: 非NULL 被分配内存的指针(此存储区中的初始值自动初始化为0)
 *			NULL 分配失败
 */
extern void *alipay_iot_calloc(size_t nmemb, size_t size)
{
    return ((void *)SCI_CALLOC(nmemb,size));
}


/*
 * @brief: 释放动态分配的内存
 * @input: s 内存块的首地址
 */
extern void alipay_iot_std_free(void *ptr)
{
    if(ptr != NULL)
    {
        SCI_FREE(ptr);
    }
    return;
}

/*
 * @brief: 将s所指向的某一块内存中的前n个字节的内容全部设置为c
 * @input: s 内存块的首地址
 * @input: c 要设置的值
 * @input: n 要设置的字节个数
 * @return: 内存块的首地址
 */
extern void *alipay_iot_memset(void *s, int c, size_t n)
{
    return memset(s,c,n);
}


/*
 * @brief: 从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
 * @input: dest 目标内存块的首地址
 * @input: src 源内存块的首地址
 * @input: n 要拷贝的字节个数
 * @return: 指向dest的指针
 */
extern void *alipay_iot_memcpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest,src,n);
}


/*
 * @brief: 将参数src字符串拷贝至参数dest所指的地址
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @return: 指向dest的指针
 * @note:  如果参数dest所指的内存空间不够大，可能会造成缓冲溢出的错误情况，
 *         在编写程序时需特别留意，或者用strncpy()来取代
 */
extern char *alipay_iot_strcpy(char *dest, const char *src)
{
    return strcpy(dest,src);
}


/*
 * @brief: 将字符串src前n个字符拷贝到字符串dest
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @input: n 要拷贝的字符个数
 * @return: 指向dest的指针
 * @note:  不像strcpy()，strncpy()不会向dest追加结束标记'\0'；
 *         src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置n个字符
 */
extern char *alipay_iot_strncpy(char *dest, const char *src, size_t n)
{
    return strncpy(dest,src,n);
}


/*
 * @brief: 把src所指字符串添加到dest结尾处(覆盖dest结尾处的'\0')
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @return: 指向dest的指针
 * @note:  src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置src的字符串
 */
extern char *alipay_iot_strcat(char *dest, const char *src)
{
    return strcat(dest,src);
}


/*
 * @brief: 把src所指字符串的前n个字符添加到dest所指字符串的结尾处，并覆盖dest所指字符串结尾的'\0'
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @input: n 要拷贝的字符个数
 * @return: 指向dest的指针
 * @note:  src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置要拷贝的字符串
 */
extern char *alipay_iot_strncat(char *dest, const char *src, size_t n)
{
    return strncat(dest,src,n);
}


/*
 * @brief: 在参数s所指向的字符串中搜索第一次出现字符c的位置
 * @param: s 要被检索的字符串
 * @param: c 要被检索的字符
 * @return: c在str中第一次出现的位置，如果未找到，则返回NULL
 */
extern char *alipay_iot_strchr(const char *s, int c)
{
    return strchr(s,c);
}


/*
 * @brief: 把 s1 所指向的字符串和 s2 所指向的字符串进行比较
 * @input: s1 要进行比较的第一个字符
 * @input: s2 要进行比较的第二个字符
 * @return: 如果返回值 < 0，则表示 s1 < s2
 *          如果返回值 = 0，则表示 s1 = s2
 *          如果返回值 > 0，则表示 s1 > s2
 */
extern int alipay_iot_strcmp(const char *s1, const char *s2)
{
    return strcmp(s1,s2);
}


/*
 * @brief: 计算字符串s的长度，直到空结束字符，但不包括空结束字符
 * @input: s 要计算长度的字符串
 * @return: 字符串的长度
 */
extern size_t alipay_iot_strlen(const char *s)
{
    return strlen(s);
}


/*
 * @brief: 分解字符串 str 为一组字符串，delim 为分隔符。
 * @input: str 要被分解成一组小字符串的字符串。
 * @input: delim 包含分隔符的 C 字符串。
 * @return: 返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针。
 */
extern char *alipay_iot_strtok(char *str, const char *delim)
{
    return strtok(str,delim);
}


extern int alipay_iot_snprintf(char *str, size_t size, const char *format, ...)
{
    int ret = 0;
    va_list argptr;
    va_start(argptr, format);
    #ifdef WIN32
    ret = vsprintf(str,format, argptr);
    #else
    ret = vsnprintf(str,size,format, argptr);
    #endif
    va_end(argptr);
    return ret;
}


/*
* @brief: 使任务休眠milliseconds毫秒
* @input: milliseconds 休眠的时间，单位毫秒
*/
extern void alipay_iot_sleep(unsigned long milliseconds)
{
    SCI_SLEEP(milliseconds);
    return;
}

/*
* @brief: 创建互斥量
* @input: mutex_name 互斥量的名称
* @return: 其他 创建成功
*          NULL 创建失败
*/
extern alipay_iot_mutex alipay_iot_mutex_create(const char* mutex_name)
{
    return SCI_CreateMutex(mutex_name, SCI_NO_INHERIT);
}


/*
* @brief: 对互斥量加锁
* @input: mutex 要加锁的互斥量
* @return: 0 加锁成功
*          -1 加锁失败
*/
extern int alipay_iot_mutex_lock(alipay_iot_mutex mutex)
{
	if(mutex != NULL)
		SCI_GetMutex(mutex, SCI_WAIT_FOREVER);
    return 0;
}


/*
* @brief: 对互斥量解锁
* @input: mutex 要解锁的互斥量
* @return: 0 解锁成功
*          -1 解锁失败
*/
extern int alipay_iot_mutex_unlock(alipay_iot_mutex mutex)
{
    if(mutex != NULL)
        	SCI_PutMutex(mutex);
    return 0;
}


/*
* @brief: 删除互斥量
* @input: mutex 要删除的互斥量
* @return: 0 删除成功
*          -1 删除失败
*/
extern int alipay_iot_mutex_delete(alipay_iot_mutex mutex)
{
    return SCI_DeleteMutex((SCI_MUTEX_PTR)mutex);
}

/*
* @input: levle 日志过滤等级
*/
extern void alipay_iot_log_level_set(alipay_iot_log_lvl_t level)
{
    g_cur_alipay_log_level = level;
    return;
}


/*
 * @brief: 日志输出
 * @input: file_name 日志打印位置所在的文件名
 * @input: line      日志打印位置的行数
 * @input: level     日志等级
 * @input: level_name日志等级名称
 * @input: format    日志格式
 * @input: ...       和format对应的参数
 */
 #ifdef WIN32
extern void alipay_iot_log_win(
               const char *tag,
               const char* format, ...)
 {
    return;
 }
#endif
extern void alipay_iot_log(const char *file_name,
               uint32_t line,
               alipay_iot_log_lvl_t level,
               const char *tag,
               const char* format, ...)
{
    char buf[1024] = {0};
    uint32 tmp_len = 0;
    va_list va_args;
    #if 0
    if(level > g_cur_alipay_log_level)
    {
        return;
    }
    #endif
    
    va_start(va_args, format);
    #ifdef WIN32
    if (vsprintf(buf,format, va_args) >= 0) 
    #else
    if (vsnprintf(buf,1023,format, va_args) >= 0) 
    #endif
    {
        if(file_name != NULL)
        {
            if(tag != NULL)
            {
                ZFB_AlipayTrace("[%s]line=%d:%s",tag,line,buf);
            }
            else
            {
                ZFB_AlipayTrace("line=%d:%s",line,buf);
            }
        }
        else
        {
            if(tag != NULL)
            {
                ZFB_AlipayTrace("[%s]:%s",tag,buf);
            }
            else
            {
                ZFB_AlipayTrace(":%s",buf);
            }
        }
    }
    va_end(va_args);
}

extern void alipay_iot_fd_zero(alipay_iot_fd_set* fdset)
{
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_fd_zero fdset = 0x%x",fdset);
#endif
    SOC_FD_ZERO(fdset);
    return;
}

extern void alipay_iot_fd_setbit(int fd, alipay_iot_fd_set* fdset)
{
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_fd_setbit fd=%d, fdset = 0x%x",fd,fdset);
#endif
    if(fdset != NULL)
    {
        if(fd < ZFB_MAX_SOCKET_NUM)
        {
            //SCI_TRACE_LOW("alipay_iot_fd_setbit fd=%d, bit=0x%x",fd,fdset->fds_bits[fd]);
            SOC_FD_SET(fd,fdset);
        }
    }
    return;
}

extern void alipay_iot_fd_clrbit(int fd, alipay_iot_fd_set* fdset)
{
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_fd_clrbit fd=%d, fdset = 0x%x",fd,fdset);
#endif
    if(fd < ZFB_MAX_SOCKET_NUM && fdset != NULL)
    {
        //SCI_TRACE_LOW("alipay_iot_fd_clrbit fd=%d, bit=0x%x",fd,fdset->fds_bits[fd]);
        SOC_FD_CLR(fd,fdset);
    }
    return;
}

/*
* 检查fdset中对应的fd是否有被selected
*/
extern int  alipay_iot_fd_isset(int fd, alipay_iot_fd_set* fdset)
{
    int ret = 0;
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_fd_isset fd=%d, fdset = 0x%x",fd,fdset);
#endif
    if(fd < ZFB_MAX_SOCKET_NUM && fdset != NULL)
    {        
#ifdef ALIPAY_DEBUG
        SCI_TRACE_LOW("alipay_iot_fd_isset fd=%d, bit=0x%x",fd,fdset->fds_bits[fd]);
#endif
        if(SOC_FD_ISSET(fd,fdset))
        {
#ifdef ALIPAY_DEBUG
            SCI_TRACE_LOW("alipay_iot_fd_isset fd=%d, OK",fd);
#endif
            ret = 1;
        }
        else
        {
#ifdef ALIPAY_DEBUG
            SCI_TRACE_LOW("alipay_iot_fd_isset fd=%d, FAIL",fd);
#endif
        }
    }
    return ret;
}


/**
 * 返回-1表示失败，返回-2表示应再次尝试，
 * 返回0表示没有事件，返回正数表示有事件的句柄数。
 * 不会返回别的值。
 */
extern int alipay_iot_select(int          maxfdp1,
                      alipay_iot_fd_set  *readset,
                      alipay_iot_fd_set  *writeset,
                      alipay_iot_fd_set  *exceptset,
                      alipay_iot_timeval *timeout)
{
    int ret = -1;
    int s;
    uint16 i = 0;
    int max_ndesc = -1;
    alipay_iot_fd_set *fd[3];
    sci_fd_set sprd_fd[3];
    long tv = 0;
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_select maxfdp1=%d r=0x%x,w=0x%x,e=0x%x",maxfdp1,readset,writeset,exceptset);
#endif
    if(timeout != PNULL)
    {
#ifdef ALIPAY_DEBUG
        SCI_TRACE_LOW("alipay_iot_select tv_sec=%d,tv_usec=%d",timeout->tv_sec,timeout->tv_usec);
#endif
        tv = timeout->tv_sec*10;
    }
    
    SCI_FD_ZERO(&sprd_fd[0]);
    SCI_FD_ZERO(&sprd_fd[1]);
    SCI_FD_ZERO(&sprd_fd[2]);
    
    fd[0] = readset;
    fd[1] = writeset;
    fd[2] = exceptset;

    for (s = 0; s < ZFB_MAX_SOCKET_NUM; s++)
    {
        for (i = 0; i < 3; i++)
        {
            if (fd[i] && SOC_FD_WANT(s, fd[i]))   /* SOC_FD_SET */
            {
                SCI_FD_SET(sockfd_arry[s].sockfd,&sprd_fd[i]);
            }
        }
    }

    ret = sci_sock_select(
                (readset ? &sprd_fd[0] : NULL),
                (writeset ? &sprd_fd[1] : NULL),
                (exceptset ? &sprd_fd[2] : NULL),
                tv);
    
    if(ret > 0)
    {
        for (s = 0; s < ZFB_MAX_SOCKET_NUM; s++)
        {
            for (i = 0; i < 3; i++)
            {
                if (fd[i] && SCI_FD_ISSET(sockfd_arry[s].sockfd, &sprd_fd[i]))
                {
#ifdef ALIPAY_DEBUG
                    SCI_TRACE_LOW("alipay_iot_select ISSET(%d,%d)  0x%x ", i, s, sockfd_arry[s].sockfd);
#endif
                    SOC_FD_SET_READY(s, fd[i]);
                }
            }
        }
    }    
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_select  ret: %d", ret);
#endif
    return ret;
}

/**
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */

extern int alipay_iot_dns(const char *name, unsigned char ip[4])
{
    int ret = -1;
    TCPIP_IPADDR_T send_addr = 0;
    struct sci_hostent * host = sci_gethostbyname(name);
    
    SCI_TRACE_LOW("alipay_iot_dns %s",name);
    
    if(host != NULL)
    {
        char * ip_str = NULL;
        if(sizeof(TCPIP_IPADDR_T) == host->h_length)
        {
            SCI_MEMCPY(&send_addr, host->h_addr_list[0], host->h_length );
            SCI_MEMCPY(ip, host->h_addr_list[0], 4 );
            ip_str = inet_ntoa(send_addr);
            if(ip_str != NULL)
            {
                SCI_TRACE_LOW("alipay_iot_dns 0x%x,ip_str=%s",send_addr,ip_str);
            }
            ret = 0;
            SCI_TRACE_LOW("alipay_iot_dns dns=0x%x",send_addr);
        }
        else
        {
            SCI_TRACE_LOW("alipay_iot_dns DNS ERR Length");
        }
    }
    else
    {
        SCI_TRACE_LOW("alipay_iot_dns DNS ERR");
    }
    return ret;
}


/**
 * 创建套接口。
 * domain目前只支持ALIPAY_IOT_SOC_PF_INET
 * type为SOC_SOCK_STREAM表示TCP，为SOC_SOCK_DGRAM表示UDP。
 * protocol填0，除非type填了SOC_SOCK_RAW，则要指定如下之一：
 *     SOC_IPPROTO_IP
 *     SOC_IPPROTO_HOPOPTS
 *     SOC_IPPROTO_ICMP
 *     SOC_IPPROTO_IGMP
 *     SOC_IPPROTO_IPV4
 *     SOC_IPPROTO_IPIP
 *     SOC_IPPROTO_TCP
 *     SOC_IPPROTO_UDP
 *     SOC_IPPROTO_ESP
 *     SOC_IPPROTO_AH
 *     SOC_IPPROTO_IPCOMP
 *     SOC_IPPROTO_RAW
 *     SOC_IPPROTO_DONE
 * 返回-1表示失败，返回其他为套接口的描述符。
 */
extern int alipay_iot_socket_create(int domain, alipay_socket_type_enum type, int protocol)
{
    int fd = -1;
    TCPIP_SOCKET_T sockfd = TCPIP_SOCKET_INVALID;
    uint32 net_id = MMIZFB_Net_GetNetID();
    if(net_id == 0)
    {
        SCI_TRACE_LOW("alipay_iot_socket_create net_id ERR");
        return -1;
    }

#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_create domain=%d,type=%d,protocal=%d",domain,type,protocol);
#endif
    if(type == ALIPAY_IOT_SOC_SOCK_STREAM)
    {
        sockfd = sci_sock_socket(AF_INET,SOCK_STREAM,0, net_id);
        if(sockfd != TCPIP_SOCKET_INVALID)
        {
            fd = zfb_sockfd_add(domain,type,protocol,sockfd);
            if(fd == -1)
            {
                SCI_TRACE_LOW("alipay_iot_socket_create ERR1 full");
                sci_sock_socketclose(sockfd);
            }
            else
            {
                sci_sock_setsockopt(sockfd, SO_BIO, 0);
            }
        }
    }
    else if(type == ALIPAY_IOT_SOC_SOCK_DGRAM)
    {
        sockfd = sci_sock_socket(AF_INET,SOCK_DGRAM,0, net_id);
        if(sockfd != TCPIP_SOCKET_INVALID)
        {
            fd = zfb_sockfd_add(domain,type,protocol,sockfd);
            if(fd == -1)
            {
                SCI_TRACE_LOW("alipay_iot_socket_create ERR2 full");
                sci_sock_socketclose(sockfd);
            }
       }
    }
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_create net_id=%d, fd=%d,sockfd=0x%x",net_id,fd,sockfd);
#endif
    return fd;
}

/**
 * 关闭套接口。
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_close(int s)
{
    int ret = -1;
    TCPIP_SOCKET_T sockfd =  zfb_sockfd_get(s);
    
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_close fd=%d,sockfd=0x%x",s,sockfd);
#endif
    if(sockfd != TCPIP_SOCKET_INVALID)
    {
         ret = sci_sock_socketclose(sockfd);
         if(ret == 0)
         {
             zfb_sockfd_set(s,TCPIP_SOCKET_INVALID);
         }
         else
         {
            SCI_TRACE_LOW("alipay_iot_socket_close sci_sock_socketclose ERR ret=%d ",ret);
            ret = -1;
         }
    }
    return ret;

}

/**
 * 设置套接口的本地地址。
 * name->sa_len无效，
 * name->sa_family必须是SOC_PF_INET，
 * name->data.sin_data.port和name->data.sin_data.ip分别填入端口号和IPv4地址，
 * 端口号必须是网络字节序，即大端序。
 * namelen无效。
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_bind(int                        s,
                    const struct alipay_iot_sockaddr *name,
                    unsigned int               namelen)
{
    int ret = -1;
    TCPIP_SOCKET_T sockfd =  zfb_sockfd_get(s);
    if(name == NULL)
    {
        return -1;
    }
    //SCI_TRACE_LOW("alipay_iot_socket_bind(%d) fd=%d,sockfd=0x%x,ip=%d.%d.%d.%d:%d",namelen,s,name->data.sin_data.ip[0],name->data.sin_data.ip[1],name->data.sin_data.ip[2],name->data.sin_data.ip[3],name->data.sin_data.port);
    if(sockfd != TCPIP_SOCKET_INVALID)
    {
        struct sci_sockaddr addr = {0};
        addr.family = AF_INET;
        addr.port = name->data.sin_data.port;
        addr.ip_addr = *((uint32 *)name->data.sin_data.ip);
        //SCI_TRACE_LOW("alipay_iot_socket_bind ip_addr = 0x%x ",addr.ip_addr);
         ret = sci_sock_bind(sockfd,&addr,sizeof(struct sci_sockaddr));
         if(ret == 0)
         {
             //SCI_TRACE_LOW("alipay_iot_socket_bind sci_sock_bind OK");
         }
         else
         {
            SCI_TRACE_LOW("alipay_iot_socket_bind sci_sock_bind ERR ret=%d ",ret);
            ret = -1;
         }
    }
    return ret;
}


/**
 * 若是TCP套接口，则向指定地址发起连接；
 * 若是UDP套接口，则设置缺省的对端地址。
 * name->sa_len无效，
 * name->sa_family必须是SOC_PF_INET，
 * name->data.sin_data.port和name->data.sin_data.ip分别填入端口号和IPv4地址，
 * 端口号必须是网络字节序，即大端序。
 * namelen无效。
 * 返回-1表示失败，返回-2表示异步操作正在进行中，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_connect(int                            s,
                        const struct alipay_iot_sockaddr *name,
                        unsigned int                    namelen)
{
    int send_res = -1;
    int ret = -1;
    ZFB_SOCKET_TYPE_T * p_soc =  zfb_sockfd_all_get(s);
    if(name == NULL || p_soc == NULL)
    {
        SCI_TRACE_LOW("alipay_iot_socket_connect ERR param");
        return -1;
    }
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_connect(%d) fd=%d,sockfd=0x%x,ip=%d.%d.%d.%d:%d",namelen,s,p_soc->sockfd,name->data.sin_data.ip[0],name->data.sin_data.ip[1],name->data.sin_data.ip[2],name->data.sin_data.ip[3],name->data.sin_data.port);
#endif
    if(p_soc->sockfd != TCPIP_SOCKET_INVALID)
    {
        struct sci_sockaddr addr = {0};
        addr.family = AF_INET;
        addr.port = htons(name->data.sin_data.port);
        addr.ip_addr = *((uint32 *)name->data.sin_data.ip);
#ifdef ALIPAY_DEBUG
        SCI_TRACE_LOW("alipay_iot_socket_connect ip_addr = 0x%x ",addr.ip_addr);
#endif
        if(p_soc->type == ALIPAY_IOT_SOC_SOCK_DGRAM)
        {
            sockfd_dest_addr[s] = addr;
        }
        else
        {
            sockfd_dest_addr[s] = addr;
            send_res = sci_sock_connect(p_soc->sockfd,&addr,sizeof(struct sci_sockaddr));
            if(send_res == TCPIP_SOCKET_ERROR)
            {
                if(EINPROGRESS != sci_sock_errno(p_soc->sockfd) )
                {
                    SCI_TRACE_LOW("alipay_iot_socket_connect ERR -1");
                    ret = -1;
                }
                else
                {
#ifdef ALIPAY_DEBUG
                    SCI_TRACE_LOW("alipay_iot_socket_connect WAIT  EINPROGRESS -2");
#endif
                    ret = -2;
                }
            }
            else
            {
#ifdef ALIPAY_DEBUG
                SCI_TRACE_LOW("alipay_iot_socket_connect OK sockfd=0x%x, ip_addr=0x%x ",p_soc->sockfd,addr.ip_addr);
#endif
                ret = 0;
            }
        }
     }
    return ret;
}


/**
 * 向指定地址发送数据。一般用于UDP套接口。
 * flags无效。
 * to->sa_len无效，
 * to->sa_family必须是SOC_PF_INET，
 * to->data.sin_data.port和to->data.sin_data.ip分别填入端口号和IPv4地址，
 * 端口号必须是网络字节序，即大端序。
 * tolen无效。
 * 返回-1表示失败，返回-2表示缓冲区已满，返回0或正数表示成功发送的字节数。
 * 不会返回别的值。
 */
extern int alipay_iot_socket_sendto(int                           s,
                        const void                  *dataptr,
                        size_t                          size,
                        int                            flags,
                        const struct alipay_iot_sockaddr *to,
                        unsigned int                    tolen)
{
    int send_res = -1;
    int ret = -1;
    ZFB_SOCKET_TYPE_T * p_soc =  zfb_sockfd_all_get(s);
    if(to == NULL || p_soc == NULL)
    {
        SCI_TRACE_LOW("alipay_iot_socket_sendto ERR param");
        return -1;
    }
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_sendto(%d) fd=%d,sockfd=0x%x,ip=%d.%d.%d.%d:%d",tolen,s,to->data.sin_data.ip[0],to->data.sin_data.ip[1],to->data.sin_data.ip[2],to->data.sin_data.ip[3],to->data.sin_data.port);
#endif
    if(p_soc->sockfd != TCPIP_SOCKET_INVALID)
    {
        struct sci_sockaddr addr = {0};
        addr.family = AF_INET;
        addr.port = to->data.sin_data.port;
        addr.ip_addr = *((uint32 *)to->data.sin_data.ip);
#ifdef ALIPAY_DEBUG
        SCI_TRACE_LOW("alipay_iot_socket_sendto ip_addr = 0x%x ",addr.ip_addr);
#endif
        send_res = sci_sock_sendto(p_soc->sockfd,dataptr,size,0,&addr);
        if(send_res == TCPIP_SOCKET_ERROR)
        {
            if(ENOBUFS != sci_sock_errno(p_soc->sockfd))
            {
                SCI_TRACE_LOW("alipay_iot_socket_sendto ERR -1");
                ret = -1;
            }
            else
            {
                SCI_TRACE_LOW("alipay_iot_socket_sendto ERR ENOBUFS -2");
                ret = -2;
            }
        }
        else
        {
            ret = 0;
        }
     }
    return ret;
}


/**
 * 发送数据。
 * 返回-1表示失败，返回-2表示缓冲区已满，返回0或正数表示成功发送的字节数。
 * 不会返回别的值。
 */
extern int alipay_iot_socket_write(int s, const void *dataptr, size_t len)
{
    int ret = -1;
    int send_len = 0;
    TCPIP_SOCKET_T sockfd =  zfb_sockfd_get(s);
    if(dataptr == NULL)
    {
        SCI_TRACE_LOW("alipay_iot_socket_write ERR param");
        return -1;
    }
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_write len=%d,fd=%d,sockfd=0x%x",len,s,sockfd);
#endif
    if(sockfd != TCPIP_SOCKET_INVALID)
    {
        send_len = sci_sock_send(sockfd,dataptr,len,0);
        if(send_len == TCPIP_SOCKET_ERROR)
        {
            int error = sci_sock_errno(sockfd);
            if(ENOBUFS != error)
            {
                SCI_TRACE_LOW("alipay_iot_socket_write ERR -1");
                ret = -1;
            }
            else
            {
                SCI_TRACE_LOW("alipay_iot_socket_write ERR ENOBUFS -2");
                 ret = -2;
            }
        }
        else
        {
            ret = send_len;
#ifdef ALIPAY_DEBUG
            SCI_TRACE_LOW("alipay_iot_socket_write OK sent_len=%d",send_len);
#endif
            //ZFB_TraceCR(dataptr,send_len);
            //ZFB_HexTraceU8(dataptr,send_len);
        }
    }
    return ret;
}


/**
 * 接收数据并告知对端地址。一般用于UDP套接口。
 * flags可以用或操作组合以下任意值：
 *     SOC_MSG_OOB
 *     SOC_MSG_PEEK
 *     SOC_MSG_DONTROUTE
 *     SOC_MSG_EOR
 *     SOC_MSG_TRUNC
 *     SOC_MSG_CTRUNC
 *     SOC_MSG_WAITALL
 *     SOC_MSG_DONTWAIT
 * from->sa_len无效，
 * from->sa_family必须是SOC_PF_INET，
 * from->data.sin_data.port和from->data.sin_data.ip分别填入端口号和IPv4地址，
 * 端口号必须是网络字节序，即大端序。
 * fromlen指向的值无效。
 * 返回-1表示失败，返回-2表示无数据可读，返回0表示对端已关闭写，
 * 返回正数表示成功接收的字节数。不会返回别的值。
 */
extern int alipay_iot_socket_recvfrom(int                     s,
                        void                        *mem,
                        size_t                       len,
                        int                        flags,
                        struct alipay_iot_sockaddr *from,
                        unsigned int            *fromlen)
{
    int ret = -1;
    int rcv_len = 0;
    TCPIP_SOCKET_T sockfd =  zfb_sockfd_get(s);
    if(mem == NULL)
    {
        SCI_TRACE_LOW("alipay_iot_socket_recvfrom ERR param");
        return -1;
    }
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_recvfrom(%d) fd=%d,sockfd=0x%x,ip=%d.%d.%d.%d:%d",len,s,from->data.sin_data.ip[0],from->data.sin_data.ip[1],from->data.sin_data.ip[2],from->data.sin_data.ip[3],from->data.sin_data.port);
#endif
    if(sockfd != TCPIP_SOCKET_INVALID)
    {
        struct sci_sockaddr addr = {0};
        addr.family = AF_INET;
        addr.port = from->data.sin_data.port;
        addr.ip_addr = *((uint32 *)from->data.sin_data.ip);
#ifdef ALIPAY_DEBUG
        SCI_TRACE_LOW("alipay_iot_socket_recvfrom ip_addr = 0x%x ",addr.ip_addr);
#endif
        rcv_len  = sci_sock_recvfrom(sockfd,mem,len,flags,&addr);
        if(rcv_len > 0)
        {
            ret = rcv_len;
#ifdef ALIPAY_DEBUG
            SCI_TRACE_LOW("alipay_iot_socket_recvfrom OK rcv_len=%d",rcv_len);
#endif
        }
        else if(rcv_len == 0)
        {
            ret = 0;
            SCI_TRACE_LOW("alipay_iot_socket_recvfrom ERR rcv_len=0");
        }
        else
        {
            int error = sci_sock_errno(sockfd);
            if(EWOULDBLOCK != error)
            {
                SCI_TRACE_LOW("alipay_iot_socket_recvfrom ERR -1");
                ret = -1;
            }
            else
            {
                SCI_TRACE_LOW("alipay_iot_socket_recvfrom ERR ENOBUFS -2");
                 ret = -2;
            }
        }
    }
    return ret;
}


/**
 * 接收数据。
 * 返回-1表示失败，返回-2表示无数据可读，返回0表示对端已关闭写，
 * 返回正数表示成功接收的字节数。不会返回别的值。
 */
extern int alipay_iot_socket_read(int s, void *mem, size_t len)
{
    int ret = -1;
    int read_len = 0;
    TCPIP_SOCKET_T sockfd =  zfb_sockfd_get(s);
    if(mem == NULL)
    {
        SCI_TRACE_LOW("alipay_iot_socket_read ERR param");
        return -1;
    }
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_read len=%d,fd=%d,sockfd=0x%x",len,s,sockfd);
#endif
    if(sockfd != TCPIP_SOCKET_INVALID)
    {
        read_len = sci_sock_recv(sockfd,mem,len,0);
        if(read_len == TCPIP_SOCKET_ERROR)
        {
            int error = sci_sock_errno(sockfd);
            if(EWOULDBLOCK != error)
            {
                SCI_TRACE_LOW("alipay_iot_socket_read ERR -1");
                ret = -1;
            }
            else
            {
#ifdef ALIPAY_DEBUG
                SCI_TRACE_LOW("alipay_iot_socket_read NoData -2");
#endif
                 ret = -2;
            }
        }
        else if(read_len == 0)
        {
            SCI_TRACE_LOW("alipay_iot_socket_read ERR 0");
            ret = 0;
        }
        else
        {
            ret = read_len;
#ifdef ALIPAY_DEBUG
            SCI_TRACE_LOW("alipay_iot_socket_read OK read_len=%d",read_len);
#endif
            //ZFB_TraceCR(mem,read_len);
            //ZFB_HexTraceU8(mem,read_len);
        }
    }
    return ret;
}


/**
 * 设置套接口的参数。
 * level无效。
 * optname取值如下之一：
 *     SOC_OOBINLINE，目前不支持
 *     SOC_LINGER，opval指向soc_linger_struct
 *     SOC_NBIO，opval指向kal_bool
 *     SOC_ASYNC，opval指向kal_uint8
 *     SOC_NODELAY，opval指向kal_bool
 *     SOC_KEEPALIVE，opval指向kal_bool
 *     SOC_RCVBUF，opval指向kal_uint32
 *     SOC_SENDBUF，opval指向kal_uint32
 *     SOC_PKT_SIZE，opval指向kal_uint32
 *     SOC_SILENT_LISTEN，opval指向kal_bool
 *     SOC_QOS，opval指向kal_uint8
 *     SOC_TCP_MAXSEG，opval指向kal_uint8
 *     SOC_IP_TTL，opval指向kal_uint8
 *     SOC_LISTEN_BEARER，opval指向kal_uint8
 *     SOC_UDP_ANY_FPORT，opval指向kal_bool
 *     SOC_WIFI_NOWAKEUP，opval指向kal_bool
 *     SOC_UDP_NEED_ICMP，opval指向kal_bool
 *     SOC_IP_HDRINCL，opval指向kal_bool
 *     SOC_IPSEC_POLICY，opval指向sadb_x_policy
 *     SOC_TCP_ACKED_DATA，opval指向kal_uint32
 *     SOC_TCP_DELAYED_ACK，opval指向kal_bool
 *     SOC_TCP_SACK，opval指向kal_bool
 *     SOC_TCP_TIME_STAMP，opval指向kal_bool
 *     SOC_TCP_ACK_MSEG，opval指向kal_bool
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_setsockopt(int          s,
                          int          level,
                          int          optname,
                          const void  *opval,
                          unsigned int optlen)
{
    int ret = -1;
    int opt = 0;
    void * optval = NULL;
    TCPIP_SOCKET_T sockfd =  zfb_sockfd_get(s);
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_setsockopt s=%d,optname=0x%x,opval=0x%x,optlen=%d",s,optname,opval,optlen);
#endif
    if(sockfd != TCPIP_SOCKET_INVALID)
    {
        opt = zfb_sockfd_opt_convert(optname);
#ifdef ALIPAY_DEBUG
        SCI_TRACE_LOW("zfb_sockfd_opt_convert convert_opt=0x%x",opt);
#endif
        optval = OptSetVal_Convert(optname,opval);
        ret = sci_sock_setsockopt(sockfd, opt, optval);
    }
    return ret;
}

/**
 * 获取套接口的参数。
 * level无效。
 * optname取值如下之一：
 *     SOC_OOBINLINE，目前不支持
 *     SOC_LINGER，opval指向soc_linger_struct
 *     SOC_NBIO，opval指向kal_bool
 *     SOC_ASYNC，opval指向kal_uint8
 *     SOC_NODELAY，opval指向kal_bool
 *     SOC_KEEPALIVE，opval指向kal_bool
 *     SOC_RCVBUF，opval指向kal_uint32
 *     SOC_SENDBUF，opval指向kal_uint32
 *     SOC_NREAD，opval指向kal_uint32
 *     SOC_PKT_SIZE，opval指向kal_uint32
 *     SOC_SILENT_LISTEN，opval指向kal_bool
 *     SOC_QOS，opval指向kal_uint8
 *     SOC_TCP_MAXSEG，opval指向kal_uint8
 *     SOC_IP_TTL，opval指向kal_uint8
 *     SOC_LISTEN_BEARER，opval指向kal_uint8
 *     SOC_UDP_ANY_FPORT，opval指向kal_bool
 *     SOC_WIFI_NOWAKEUP，opval指向kal_bool
 *     SOC_UDP_NEED_ICMP，opval指向kal_bool
 *     SOC_IP_HDRINCL，opval指向kal_bool
 *     SOC_IPSEC_POLICY，opval指向sadb_x_policy
 *     SOC_TCP_ACKED_DATA，opval指向kal_uint32
 *     SOC_TCP_DELAYED_ACK，opval指向kal_bool
 *     SOC_TCP_SACK，opval指向kal_bool
 *     SOC_TCP_TIME_STAMP，opval指向kal_bool
 *     SOC_TCP_ACK_MSEG，opval指向kal_bool
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_getsockopt(int         s,
                          int            level,
                          int          optname,
                          void          *opval,
                          unsigned int *optlen)
{
    int ret = -1;
    int opt = 0;
    TCPIP_SOCKET_T sockfd =  zfb_sockfd_get(s);
#ifdef ALIPAY_DEBUG
    SCI_TRACE_LOW("alipay_iot_socket_getsockopt s=%d,optname=0x%x,opval=0x%x",s,optname,opval);
#endif
    if(sockfd != TCPIP_SOCKET_INVALID)
    {
        opt = zfb_sockfd_opt_convert(optname);
#ifdef ALIPAY_DEBUG
        SCI_TRACE_LOW("alipay_iot_socket_getsockopt convert_opt=0x%x",opt);
#endif
        ret = sci_sock_getsockopt(sockfd, opt, opval);
    }
    return ret;
}


