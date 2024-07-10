#ifndef _ALIPAY_KAL_H
#define _ALIPAY_KAL_H

#ifdef WIN32
#include "alipay_iot_type.h"
#endif
//#include "iot_err.h"

typedef enum
{
    SOC_OOBINLINE = 0x01 << 0,        /* not support yet */
    SOC_LINGER = 0x01 << 1,           /* linger on close */
    SOC_NBIO = 0x01 << 2,             /* Nonblocking */
    SOC_ASYNC = 0x01 << 3,            /* Asynchronous notification */
    SOC_NODELAY = 0x01 << 4,          /* disable Nagle algorithm or not */
    SOC_KEEPALIVE = 0x01 << 5,        /* enable/disable the keepalive */
    SOC_RCVBUF = 0x01 << 6,           /* set the socket receive buffer size */
    SOC_SENDBUF = 0x01 << 7,          /* set the socket send buffer size */
    SOC_NREAD = 0x01 << 8,            /* no. of bytes for read, only for soc_getsockopt */
    SOC_PKT_SIZE = 0x01 << 9,         /* datagram max packet size */
    SOC_SILENT_LISTEN = 0x01 << 10,   /* SOC_SOCK_SMS property */
    SOC_QOS = 0x01 << 11,             /* set the socket qos */
    SOC_TCP_MAXSEG = 0x01 << 12,      /* set the max segmemnt size */
    SOC_IP_TTL = 0x01 << 13,          /* set the IP TTL value */
    SOC_LISTEN_BEARER = 0x01 << 14,   /* enable listen bearer */
    SOC_UDP_ANY_FPORT = 0x01 << 15,   /* enable UDP any foreign port */
    SOC_WIFI_NOWAKEUP = 0x01 << 16,   /* send packet in power saving mode */
    SOC_UDP_NEED_ICMP = 0x01 << 17,   /* deliver NOTIFY(close) for ICMP error */
    SOC_IP_HDRINCL = 0x01 << 18,      /* IP header included for raw sockets */
    SOC_IPSEC_POLICY = 0x01 << 19,    /* ip security policy */
    SOC_TCP_ACKED_DATA = 0x01 << 20,  /* TCPIP acked data */
    SOC_TCP_DELAYED_ACK = 0x01 << 21, /* TCP delayed ack */
    SOC_TCP_SACK = 0x01 << 22,        /* TCP selective ack */
    SOC_TCP_TIME_STAMP = 0x01 << 23,  /* TCP time stamp */
    SOC_TCP_ACK_MSEG = 0x01 << 24     /* TCP ACK multiple segment */
}alipay_iot_soc_opt;

typedef struct {
  int32_t year;
  int32_t month;
  int32_t wday;
  int32_t day;
  int32_t hour;
  int32_t minute;
  int32_t second;
  int32_t isdst;
} alipay_iot_local_time;

typedef struct
{
    long tv_sec;//秒
    long tv_usec;//单位微妙
} alipay_iot_timeval;

/*
* @brief: 获取UTC时间
* note:  1.精确级别,微妙级别
*        2.受系统时间修改影响
*        3.返回的秒数是从1970年1月1日0时0分0秒开始
*/
extern void alipay_iot_gettimeofday(alipay_iot_timeval *now);

/*
* 获取系统当前时间
* month 为1表示1月，day 为1表示1号，year为2018表示2018年。
*/
extern void alipay_iot_get_local_time(alipay_iot_local_time *time);

/*
 * @brief: 获取系统从启动到现在所花的时间(毫秒)
 * @return: 0 获取失败
 *          非0 获取成功，值是获取系统从启动到现在所花的时间(毫秒)
 */
extern uint32_t alipay_iot_get_bootup_time(void);

/*
 * @brief: 申请一块连续的指定大小的内存块
 * @input: size 内存块的大小，单位字节
 * @return: 非NULL 被分配内存的指针(此存储区中的初始值不确定)
 *			NULL 分配失败
 */
extern void *alipay_iot_malloc(size_t size);

/*
 * @brief: 在内存的动态存储区中分配nmemb个长度为size的连续空间
 * @input: nmemb 内存块的个数
 * @input: size 每个内存块的大小，单位字节
 * @return: 非NULL 被分配内存的指针(此存储区中的初始值自动初始化为0)
 *			NULL 分配失败
 */
extern void *alipay_iot_calloc(size_t nmemb, size_t size);

/*
 * @brief: 释放动态分配的内存
 * @input: s 内存块的首地址
 */
extern void alipay_iot_std_free(void *ptr);

/*
 * @brief: 释放动态分配的内存，并将指针置NULL
 * @input: s 内存块的首地址
 */
#define alipay_iot_free(ptr) do{\
        if(ptr != NULL)\
        {\
                alipay_iot_std_free(ptr);\
                ptr = NULL;\
        }\
}while(0)

/*
 * @brief: 将s所指向的某一块内存中的前n个字节的内容全部设置为c
 * @input: s 内存块的首地址
 * @input: c 要设置的值
 * @input: n 要设置的字节个数
 * @return: 内存块的首地址
 */
extern void *alipay_iot_memset(void *s, int c, size_t n);

/*
 * @brief: 从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
 * @input: dest 目标内存块的首地址
 * @input: src 源内存块的首地址
 * @input: n 要拷贝的字节个数
 * @return: 指向dest的指针
 */
extern void *alipay_iot_memcpy(void *dest, const void *src, size_t n);

/*
 * @brief: 将参数src字符串拷贝至参数dest所指的地址
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @return: 指向dest的指针
 * @note:  如果参数dest所指的内存空间不够大，可能会造成缓冲溢出的错误情况，
 *         在编写程序时需特别留意，或者用strncpy()来取代
 */
extern char *alipay_iot_strcpy(char *dest, const char *src);

/*
 * @brief: 将字符串src前n个字符拷贝到字符串dest
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @input: n 要拷贝的字符个数
 * @return: 指向dest的指针
 * @note:  不像strcpy()，strncpy()不会向dest追加结束标记'\0'；
 *         src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置n个字符
 */
extern char *alipay_iot_strncpy(char *dest, const char *src, size_t n);

/*
 * @brief: 把src所指字符串添加到dest结尾处(覆盖dest结尾处的'\0')
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @return: 指向dest的指针
 * @note:  src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置src的字符串
 */
extern char *alipay_iot_strcat(char *dest, const char *src);

/*
 * @brief: 把src所指字符串的前n个字符添加到dest所指字符串的结尾处，并覆盖dest所指字符串结尾的'\0'
 * @input: dest 目标字符串的首地址
 * @input: src 源字符串的首地址
 * @input: n 要拷贝的字符个数
 * @return: 指向dest的指针
 * @note:  src和dest所指的内存区域不能重叠，且dest必须有足够的空间放置要拷贝的字符串
 */
extern char *alipay_iot_strncat(char *dest, const char *src, size_t n);

/*
 * @brief: 在参数s所指向的字符串中搜索第一次出现字符c的位置
 * @param: s 要被检索的字符串
 * @param: c 要被检索的字符
 * @return: c在str中第一次出现的位置，如果未找到，则返回NULL
 */
extern char *alipay_iot_strchr(const char *s, int c);

/*
 * @brief: 把 s1 所指向的字符串和 s2 所指向的字符串进行比较
 * @input: s1 要进行比较的第一个字符
 * @input: s2 要进行比较的第二个字符
 * @return: 如果返回值 < 0，则表示 s1 < s2
 *          如果返回值 = 0，则表示 s1 = s2
 *          如果返回值 > 0，则表示 s1 > s2
 */
extern int alipay_iot_strcmp(const char *s1, const char *s2);

/*
 * @brief: 计算字符串s的长度，直到空结束字符，但不包括空结束字符
 * @input: s 要计算长度的字符串
 * @return: 字符串的长度
 */
extern size_t alipay_iot_strlen(const char *s);

/*
 * @brief: 分解字符串 str 为一组字符串，delim 为分隔符。
 * @input: str 要被分解成一组小字符串的字符串。
 * @input: delim 包含分隔符的 C 字符串。
 * @return: 返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针。
 */
extern char *alipay_iot_strtok(char *str, const char *delim);

extern int alipay_iot_snprintf(char *str, size_t size, const char *format, ...);

/*
* @brief: 使任务休眠milliseconds毫秒
* @input: milliseconds 休眠的时间，单位毫秒
*/
extern void alipay_iot_sleep(unsigned long milliseconds);

typedef void * alipay_iot_mutex;
/*
* @brief: 创建互斥量
* @input: mutex_name 互斥量的名称
* @return: 其他 创建成功
*          NULL 创建失败
*/
extern alipay_iot_mutex alipay_iot_mutex_create(const char* mutex_name);

/*
* @brief: 对互斥量加锁
* @input: mutex 要加锁的互斥量
* @return: 0 加锁成功
*          -1 加锁失败
*/
extern int alipay_iot_mutex_lock(alipay_iot_mutex mutex);

/*
* @brief: 对互斥量解锁
* @input: mutex 要解锁的互斥量
* @return: 0 解锁成功
*          -1 解锁失败
*/
extern int alipay_iot_mutex_unlock(alipay_iot_mutex mutex);

/*
* @brief: 删除互斥量
* @input: mutex 要删除的互斥量
* @return: 0 删除成功
*          -1 删除失败
*/
extern int alipay_iot_mutex_delete(alipay_iot_mutex mutex);


typedef enum alipay_iot_log_lvl {
    ALIPAY_IOT_LOG_LVL_VBS, /**verbose*/
    ALIPAY_IOT_LOG_LVL_DBG, /**debug*/
    ALIPAY_IOT_LOG_LVL_INF, /**inform*/
    ALIPAY_IOT_LOG_LVL_WRN, /**warn*/
    ALIPAY_IOT_LOG_LVL_ERR, /**error*/
    ALIPAY_IOT_LOG_LVL_FTL  /**fatal*/
} alipay_iot_log_lvl_t;

/*
* @input: levle 日志过滤等级
*/
extern void alipay_iot_log_level_set(alipay_iot_log_lvl_t level);

/*
 * @brief: 日志输出
 * @input: file_name 日志打印位置所在的文件名
 * @input: line      日志打印位置的行数
 * @input: level     日志等级
 * @input: level_name日志等级名称
 * @input: format    日志格式
 * @input: ...       和format对应的参数
 */
extern void alipay_iot_log(const char *file_name,
               uint32_t line,
               alipay_iot_log_lvl_t level,
               const char *tag,
               const char* format, ...);

#ifndef __FILE__
#define __FILE__    ""
#endif
#ifndef __LINE__
#define __LINE__    ""
#endif
 /*
 * @brief: 各种等级的日志输出
 * @input: format 日志格式
 * @input: ... 和format对应的参数
 * @note:  每条日志自动换行，建议的格式为: 时间 文件名 行数 日志等级 日志内容，如: 2018-08-18 18:06:09|main.c|69|INFO| hello world
 */
#ifdef WIN32
#define ALIPAY_IOT_LOG_VERBOSE  alipay_iot_log_win
#define ALIPAY_IOT_LOG_DEBUG alipay_iot_log_win
#define ALIPAY_IOT_LOG_INFO alipay_iot_log_win
#define ALIPAY_IOT_LOG_WARN alipay_iot_log_win
#define ALIPAY_IOT_LOG_ERROR alipay_iot_log_win
#define ALIPAY_IOT_LOG_FATAL  alipay_iot_log_win
#else
#define ALIPAY_IOT_LOG_VERBOSE(tag,format,...)      alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_VBS, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_DEBUG(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_DBG, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_INFO(tag,format,...)         alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_INF, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_WARN(tag,format,...)         alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_WRN, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_ERROR(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_ERR, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_FATAL(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_FTL, tag, format, ##__VA_ARGS__)
#endif
/**@brief get internet HW mac address  必须实现本接口
 *
 * @param[out]   mac_addr      6字节mac地址
 * @return       0       success
 *               others  fail
 */
#if 0
int alipay_iot_get_mac_byte(uint8_t mac_addr[IOT_MAC_BYTE_SIZE]);
#endif

/**
 * 获取设备MAC地址
 * 参数：
 * 	inOut，输出buffer，由调用方申请内存空间，被调用方填写内存值，格式通常为6个字节的二进制代码（以6组16进制数表示）如：“00-01-6C-06-A6-29”
 *  size，buffer最大长度
 * 返回值：
 * 	小于0，表示错误码；
 * 	大于等于0，表示写入buf的数据实际长度
 */
extern int alipay_iot_get_mac(char *inOut, size_t size);

/**
 * 获取可用内存大小
 * 返回值：
 * 	可用内存大小，单位是字节
 */
extern long alipay_iot_get_available_memory();


/**
 * 获取sdk所在模组的的序列号
 * 参数：
 * 	inOut，输出buffer，由调用方申请内存空间，被调用方填写内存值
 *  size，buffer最大长度
 * 返回值：
 * 	小于0，表示错误码；
 * 	大于等于0，表示写入buf的数据实际长度
 */
extern int alipay_iot_get_module_sn(char *inOut, size_t size);

/**
 * 获取（移动网络接入）设备IMEI，此接口可选实现（空实现，返回NULL），对于移动数据网络接入的设备必选
 * 参数：
 *  inOut，输出buffer，由调用方申请内存空间，被调用方填写内存值
 *  size，buffer最大长度
 * 返回值：
 *  小于0，表示错误码；
 *  大于等于0，表示写入buf的数据实际长度
 */
extern int alipay_iot_get_device_imei(char *inOut, size_t size);

/**
* 获取一个随机数
*/
extern int alipay_iot_rand(void);
/**
* 设定随机数种子
*/
extern void alipay_iot_srand(unsigned int seed);


/*
 * Protocol Familty.  Currently, we only support Internet
 */
#define ALIPAY_IOT_SOC_PF_INET (0)

/* Socket Type */
typedef enum
{
    ALIPAY_IOT_SOC_SOCK_STREAM = 0,  /* stream socket, TCP */
    ALIPAY_IOT_SOC_SOCK_DGRAM,       /* datagram socket, UDP */
}alipay_socket_type_enum;

/* Socket Options */
typedef enum
{
    ALIPAY_IOT_SOC_OOBINLINE     = 0x01 << 0,  /* not support yet */
    ALIPAY_IOT_SOC_LINGER        = 0x01 << 1,  /* linger on close */
    ALIPAY_IOT_SOC_NBIO          = 0x01 << 2,  /* Nonblocking */
    ALIPAY_IOT_SOC_ASYNC         = 0x01 << 3,  /* Asynchronous notification */

    ALIPAY_IOT_SOC_NODELAY       = 0x01 << 4,  /* disable Nagle algorithm or not */
    ALIPAY_IOT_SOC_KEEPALIVE     = 0x01 << 5,  /* enable/disable the keepalive */
    ALIPAY_IOT_SOC_RCVBUF        = 0x01 << 6,  /* set the socket receive buffer size */
    ALIPAY_IOT_SOC_SENDBUF       = 0x01 << 7,  /* set the socket send buffer size */
}alipay_soc_option_enum;

#define ALIPAY_IOT_MAX_IP_SOCKET_NUM   1  //根据实际情况修改，最多16
typedef struct
{
    uint8_t fds_bits[ALIPAY_IOT_MAX_IP_SOCKET_NUM];
} alipay_iot_fd_set;


extern void alipay_iot_fd_zero(alipay_iot_fd_set* fdset);
extern void alipay_iot_fd_setbit(int fd, alipay_iot_fd_set* fdset);
extern void alipay_iot_fd_clrbit(int fd, alipay_iot_fd_set* fdset);
/*
* 检查fdset中对应的fd是否有被selected
*/
extern int  alipay_iot_fd_isset(int fd, alipay_iot_fd_set* fdset);

/**
 * 返回-1表示失败，返回-2表示应再次尝试，
 * 返回0表示没有事件，返回正数表示有事件的句柄数。
 * 不会返回别的值。
 */
extern int alipay_iot_select(int          maxfdp1,
                      alipay_iot_fd_set  *readset,
                      alipay_iot_fd_set  *writeset,
                      alipay_iot_fd_set  *exceptset,
                      alipay_iot_timeval *timeout);

/**
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_dns(const char *name, unsigned char ip[4]);

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
extern int alipay_iot_socket_create(int domain, alipay_socket_type_enum type, int protocol);

/**
 * 关闭套接口。
 * 返回-1表示失败，返回0表示成功。不会返回别的值。
 */
extern int alipay_iot_socket_close(int s);


struct alipay_iot_sockaddr {
  unsigned char sa_len;      //此值忽略。
  unsigned char sa_family;   //目前只支持ALIPAY_IOT_SOC_PF_INET。
  union {
    struct {
        unsigned short port; //网络字节序
        unsigned char  ip[4];
    }    sin_data;
    char sa_data[14];
  }             data;
};

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
                    unsigned int               namelen);

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
                        unsigned int                    namelen);

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
                        unsigned int                    tolen);

/**
 * 发送数据。
 * 返回-1表示失败，返回-2表示缓冲区已满，返回0或正数表示成功发送的字节数。
 * 不会返回别的值。
 */
extern int alipay_iot_socket_write(int s, const void *dataptr, size_t len);

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
                        unsigned int            *fromlen);

/**
 * 接收数据。
 * 返回-1表示失败，返回-2表示无数据可读，返回0表示对端已关闭写，
 * 返回正数表示成功接收的字节数。不会返回别的值。
 */
extern int alipay_iot_socket_read(int s, void *mem, size_t len);

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
                          unsigned int optlen);

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
                          unsigned int *optlen);


#endif // KAL_H
