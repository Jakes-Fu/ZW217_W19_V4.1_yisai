#ifndef ALIPAY_IOT_KAL_H
#define ALIPAY_IOT_KAL_H

#include "alipay_iot_type.h"
#include "iot_err.h"
//#include "hc_log.h"


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
    long tv_sec;//��
    long tv_usec;//��λ΢��
} alipay_iot_timeval;


/*
 * @brief: ��ȡUTCʱ��
 * note:  1.��ȷ����,΢���
 *        2.��ϵͳʱ���޸�Ӱ��
 *        3.���ص������Ǵ�1970��1��1��0ʱ0��0�뿪ʼ
 */
extern void alipay_iot_gettimeofday(alipay_iot_timeval *now);

/*
 * ��ȡϵͳ��ǰʱ��
 * month Ϊ1��ʾ1�£�day Ϊ1��ʾ1�ţ�yearΪ2018��ʾ2018�ꡣ
 */
extern void alipay_iot_get_local_time(alipay_iot_local_time *time);

/*
 * @brief: ��ȡϵͳ������������������ʱ��(����)
 * @return: 0 ��ȡʧ��
 *          ��0 ��ȡ�ɹ���ֵ�ǻ�ȡϵͳ������������������ʱ��(����)
 */

extern uint32_t alipay_iot_get_bootup_time(void);

#ifdef __LINUX__
#include <stdlib.h>

#define alipay_iot_malloc malloc
#define alipay_iot_calloc calloc
#define alipay_iot_std_free free

#else

/*
 * @brief: ����һ��������ָ����С���ڴ��
 * @input: size �ڴ��Ĵ�С����λ�ֽ�
 * @return: ��NULL �������ڴ��ָ��(�˴洢���еĳ�ʼֵ��ȷ��)
 *			NULL ����ʧ��
 */
extern void *alipay_iot_malloc(size_t size);

/*
 * @brief: ���ڴ�Ķ�̬�洢���з���nmemb������Ϊsize�������ռ�
 * @input: nmemb �ڴ��ĸ���
 * @input: size ÿ���ڴ��Ĵ�С����λ�ֽ�
 * @return: ��NULL �������ڴ��ָ��(�˴洢���еĳ�ʼֵ�Զ���ʼ��Ϊ0)
 *			NULL ����ʧ��
 */
extern void *alipay_iot_calloc(size_t nmemb, size_t size);

/*
 * @brief: �ͷŶ�̬������ڴ�
 * @input: s �ڴ����׵�ַ
 */
extern void alipay_iot_std_free(void *ptr);
#endif

/*
 * @brief: �ͷŶ�̬������ڴ棬����ָ����NULL
 * @input: s �ڴ����׵�ַ
 */
#define alipay_iot_free(ptr) do{\
	if(ptr != NULL)\
	{\
		alipay_iot_std_free(ptr);\
		ptr = NULL;\
	}\
}while(0)

#ifdef __LINUX__
#include <string.h>
#define alipay_iot_memset memset
#define alipay_iot_memcpy memcpy
#define alipay_iot_strcpy strcpy
#define alipay_iot_strncpy strncpy
#define alipay_iot_strcat strcat
#define alipay_iot_strncat strncat
#define alipay_iot_strchr strchr
#define alipay_iot_strcmp strcmp
#define alipay_iot_strlen strlen
#define alipay_iot_strtok strtok
#define alipay_iot_snprintf snprintf

#include <unistd.h>
inline void alipay_iot_sleep(unsigned long milliseconds) {
    usleep(milliseconds *1000);
}

inline void alipay_task_sleep(uint32_t ms) {
    alipay_iot_sleep(ms);
}

#include <pthread.h>
typedef pthread_mutex_t alipay_iot_mutex;

inline alipay_iot_mutex alipay_iot_mutex_create(const char* mutex_name) {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    return mutex;
}

inline int alipay_iot_mutex_delete(alipay_iot_mutex mutex) {
    pthread_mutex_destroy(&mutex);
    return 0;
}

inline int alipay_iot_mutex_lock(alipay_iot_mutex mutex) {
    pthread_mutex_lock(&mutex);
    return 0;
}


inline int alipay_iot_mutex_unlock(alipay_iot_mutex mutex){
    pthread_mutex_unlock(&mutex);
    return 0;
}



#else
/*
 * @brief: ��s��ָ���ĳһ���ڴ��е�ǰn���ֽڵ�����ȫ������Ϊc
 * @input: s �ڴ����׵�ַ
 * @input: c Ҫ���õ�ֵ
 * @input: n Ҫ���õ��ֽڸ���
 * @return: �ڴ����׵�ַ
 */
extern void *alipay_iot_memset(void *s, int c, size_t n);

/*
 * @brief: ��Դsrc��ָ���ڴ��ַ����ʼλ�ÿ�ʼ����n���ֽڵ�Ŀ��dest��ָ���ڴ��ַ����ʼλ����
 * @input: dest Ŀ���ڴ����׵�ַ
 * @input: src Դ�ڴ����׵�ַ
 * @input: n Ҫ�������ֽڸ���
 * @return: ָ��dest��ָ��
 */
extern void *alipay_iot_memcpy(void *dest, const void *src, size_t n);

/*
 * @brief: ������src�ַ�������������dest��ָ�ĵ�ַ
 * @input: dest Ŀ���ַ������׵�ַ
 * @input: src Դ�ַ������׵�ַ
 * @return: ָ��dest��ָ��
 * @note:  �������dest��ָ���ڴ�ռ䲻���󣬿��ܻ���ɻ�������Ĵ��������
 *         �ڱ�д����ʱ���ر����⣬������strncpy()��ȡ��
 */
extern char *alipay_iot_strcpy(char *dest, const char *src);

/*
 * @brief: ���ַ���srcǰn���ַ��������ַ���dest
 * @input: dest Ŀ���ַ������׵�ַ
 * @input: src Դ�ַ������׵�ַ
 * @input: n Ҫ�������ַ�����
 * @return: ָ��dest��ָ��
 * @note:  ����strcpy()��strncpy()������dest׷�ӽ������'\0'��
 *         src��dest��ָ���ڴ��������ص�����dest�������㹻�Ŀռ����n���ַ�
 */
extern char *alipay_iot_strncpy(char *dest, const char *src, size_t n);

/*
 * @brief: ��src��ָ�ַ������ӵ�dest��β��(����dest��β����'\0')
 * @input: dest Ŀ���ַ������׵�ַ
 * @input: src Դ�ַ������׵�ַ
 * @return: ָ��dest��ָ��
 * @note:  src��dest��ָ���ڴ��������ص�����dest�������㹻�Ŀռ����src���ַ���
 */
extern char *alipay_iot_strcat(char *dest, const char *src);

/*
 * @brief: ��src��ָ�ַ�����ǰn���ַ����ӵ�dest��ָ�ַ����Ľ�β����������dest��ָ�ַ�����β��'\0'
 * @input: dest Ŀ���ַ������׵�ַ
 * @input: src Դ�ַ������׵�ַ
 * @input: n Ҫ�������ַ�����
 * @return: ָ��dest��ָ��
 * @note:  src��dest��ָ���ڴ��������ص�����dest�������㹻�Ŀռ����Ҫ�������ַ���
 */
extern char *alipay_iot_strncat(char *dest, const char *src, size_t n);

/*
 * @brief: �ڲ���s��ָ����ַ�����������һ�γ����ַ�c��λ��
 * @param: s Ҫ���������ַ���
 * @param: c Ҫ���������ַ�
 * @return: c��str�е�һ�γ��ֵ�λ�ã����δ�ҵ����򷵻�NULL
 */
extern char *alipay_iot_strchr(const char *s, int c);

/*
 * @brief: �� s1 ��ָ����ַ����� s2 ��ָ����ַ������бȽ�
 * @input: s1 Ҫ���бȽϵĵ�һ���ַ�
 * @input: s2 Ҫ���бȽϵĵڶ����ַ�
 * @return: �������ֵ < 0�����ʾ s1 < s2
 *          �������ֵ = 0�����ʾ s1 = s2
 *          �������ֵ > 0�����ʾ s1 > s2
 */
extern int alipay_iot_strcmp(const char *s1, const char *s2);

/*
 * @brief: �����ַ���s�ĳ��ȣ�ֱ���ս����ַ������������ս����ַ�
 * @input: s Ҫ���㳤�ȵ��ַ���
 * @return: �ַ����ĳ���
 */
extern size_t alipay_iot_strlen(const char *s);

/*
 * @brief: �ֽ��ַ��� str Ϊһ���ַ�����delim Ϊ�ָ�����
 * @input: str Ҫ���ֽ��һ��С�ַ������ַ�����
 * @input: delim �����ָ����� C �ַ�����
 * @return: ���ر��ֽ�ĵ�һ�����ַ��������û�пɼ������ַ������򷵻�һ����ָ�롣
 */
extern char *alipay_iot_strtok(char *str, const char *delim);

extern int alipay_iot_snprintf(char *str, size_t size, const char *format, ...);

/*
* @brief: ʹ��������milliseconds����
* @input: milliseconds ���ߵ�ʱ�䣬��λ����
*/
extern void alipay_iot_sleep(unsigned long milliseconds);

extern void alipay_task_sleep(uint32_t ms);


typedef void * alipay_iot_mutex;
/*
* @brief: ����������
* @input: mutex_name ������������
* @return: ���� �����ɹ�
*          NULL ����ʧ��
*/
extern alipay_iot_mutex alipay_iot_mutex_create(const char* mutex_name);

/*
* @brief: �Ի���������
* @input: mutex Ҫ�����Ļ�����
* @return: 0 �����ɹ�
*          -1 ����ʧ��
*/
extern int alipay_iot_mutex_lock(alipay_iot_mutex mutex);

/*
* @brief: �Ի���������
* @input: mutex Ҫ�����Ļ�����
* @return: 0 �����ɹ�
*          -1 ����ʧ��
*/
extern int alipay_iot_mutex_unlock(alipay_iot_mutex mutex);

/*
* @brief: ɾ��������
* @input: mutex Ҫɾ���Ļ�����
* @return: 0 ɾ���ɹ�
*          -1 ɾ��ʧ��
*/
extern int alipay_iot_mutex_delete(alipay_iot_mutex mutex);

#endif

typedef enum alipay_iot_log_lvl {
    ALIPAY_IOT_LOG_LVL_VBS, /**verbose*/
    ALIPAY_IOT_LOG_LVL_DBG, /**debug*/
    ALIPAY_IOT_LOG_LVL_INF, /**inform*/
    ALIPAY_IOT_LOG_LVL_WRN, /**warn*/
    ALIPAY_IOT_LOG_LVL_ERR, /**error*/
    ALIPAY_IOT_LOG_LVL_FTL  /**fatal*/
} alipay_iot_log_lvl_t;

/*
* @input: levle ��־���˵ȼ�
*/
extern void alipay_iot_log_level_set(alipay_iot_log_lvl_t level);

/*
 * @brief: ��־���
 * @input: file_name ��־��ӡλ�����ڵ��ļ���
 * @input: line      ��־��ӡλ�õ�����
 * @input: level     ��־�ȼ�
 * @input: level_name��־�ȼ�����
 * @input: format    ��־��ʽ
 * @input: ...       ��format��Ӧ�Ĳ���
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
 * @brief: ���ֵȼ�����־���
 * @input: format ��־��ʽ
 * @input: ... ��format��Ӧ�Ĳ���
 * @note:  ÿ����־�Զ����У�����ĸ�ʽΪ: ʱ�� �ļ��� ���� ��־�ȼ� ��־���ݣ���: 2018-08-18 18:06:09|main.c|69|INFO| hello world
 */
// #define ALIPAY_IOT_LOG_VERBOSE(tag,format,...)      alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_VBS, tag, format, ##__VA_ARGS__)
// #define ALIPAY_IOT_LOG_DEBUG(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_DBG, tag, format, ##__VA_ARGS__)
// #define ALIPAY_IOT_LOG_INFO(tag,format,...)         alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_INF, tag, format, ##__VA_ARGS__)
// #define ALIPAY_IOT_LOG_WARN(tag,format,...)         alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_WRN, tag, format, ##__VA_ARGS__)
// #define ALIPAY_IOT_LOG_ERROR(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_ERR, tag, format, ##__VA_ARGS__)
// #define ALIPAY_IOT_LOG_FATAL(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_FTL, tag, format, ##__VA_ARGS__)

#define ALIPAY_IOT_LOG_VERBOSE(tag,format,...)      alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_VBS, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_DEBUG(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_DBG, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_INFO(tag,format,...)         alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_INF, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_WARN(tag,format,...)         alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_WRN, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_ERROR(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_ERR, tag, format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_FATAL(tag,format,...)        alipay_iot_log(__FILE__, __LINE__, ALIPAY_IOT_LOG_LVL_FTL, tag, format, ##__VA_ARGS__)

#if 0
#define ALIPAY_IOT_LOG_VERBOSE(tag,format,...)      LOG_DBG(format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_DEBUG(tag,format,...)        LOG_DBG(format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_INFO(tag,format,...)         LOG_INFO(format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_WARN(tag,format,...)         LOG_WARN(format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_ERROR(tag,format,...)        LOG_ERROR(format, ##__VA_ARGS__)
#define ALIPAY_IOT_LOG_FATAL(tag,format,...)        LOG_FATAL(format, ##__VA_ARGS__)
#endif

/**@brief get internet HW mac address  ����ʵ�ֱ��ӿ�
 *
 * @param[out]   mac_addr      6�ֽ�mac��ַ
 * @return       0       success
 *               others  fail
 */

int alipay_iot_get_mac_byte(uint8_t mac_addr[IOT_MAC_BYTE_SIZE]);

/**
 * ��ȡ�豸MAC��ַ
 * ������
 * 	inOut�����buffer���ɵ��÷������ڴ�ռ䣬�����÷���д�ڴ�ֵ����ʽͨ��Ϊ6���ֽڵĶ����ƴ��루��6��16��������ʾ���磺��00-01-6C-06-A6-29��
 *  size��buffer��󳤶�
 * ����ֵ��
 * 	С��0����ʾ�����룻
 * 	���ڵ���0����ʾд��buf������ʵ�ʳ���
 */
extern int alipay_iot_get_mac(char *inOut, size_t size);

/**
 * ��ȡ�����ڴ��С
 * ����ֵ��
 * 	�����ڴ��С����λ���ֽ�
 */
extern long alipay_iot_get_available_memory();


/**
 * ��ȡsdk����ģ��ĵ����к�
 * ������
 * 	inOut�����buffer���ɵ��÷������ڴ�ռ䣬�����÷���д�ڴ�ֵ
 *  size��buffer��󳤶�
 * ����ֵ��
 * 	С��0����ʾ�����룻
 * 	���ڵ���0����ʾд��buf������ʵ�ʳ���
 */
extern int alipay_iot_get_module_sn(char *inOut, size_t size);

/**
 * ��ȡ���ƶ�������룩�豸IMEI���˽ӿڿ�ѡʵ�֣���ʵ�֣�����NULL���������ƶ��������������豸��ѡ
 * ������
 *  inOut�����buffer���ɵ��÷������ڴ�ռ䣬�����÷���д�ڴ�ֵ
 *  size��buffer��󳤶�
 * ����ֵ��
 *  С��0����ʾ�����룻
 *  ���ڵ���0����ʾд��buf������ʵ�ʳ���
 */
extern int alipay_iot_get_device_imei(char *inOut, size_t size);

#ifdef __LINUX__
#include <stdlib.h>
#define alipay_iot_rand rand
#define alipay_iot_srand srand

// inline int alipay_iot_rand(void) {
//     return rand();
// }

// inline void alipay_iot_srand(unsigned int seed) {
//     srand(seed);
// }


#else

/**
 * ��ȡһ�������
 */
extern int alipay_iot_rand(void);
/**
* �趨���������
*/
extern void alipay_iot_srand(unsigned int seed);

#endif

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

#define ALIPAY_IOT_MAX_IP_SOCKET_NUM   1  //����ʵ������޸ģ����16

#ifdef __LINUX__
#include <sys/select.h>
#define alipay_iot_fd_set fd_set

#else 

typedef struct
{
    uint8_t fds_bits[ALIPAY_IOT_MAX_IP_SOCKET_NUM];
} alipay_iot_fd_set;

#endif




extern void alipay_iot_fd_zero(alipay_iot_fd_set* fdset);
extern void alipay_iot_fd_setbit(int fd, alipay_iot_fd_set* fdset);
extern void alipay_iot_fd_clrbit(int fd, alipay_iot_fd_set* fdset);
/*
* ���fdset�ж�Ӧ��fd�Ƿ��б�selected
*/
extern int  alipay_iot_fd_isset(int fd, alipay_iot_fd_set* fdset);

/**
 * ����-1��ʾʧ�ܣ�����-2��ʾӦ�ٴγ��ԣ�
 * ����0��ʾû���¼�������������ʾ���¼��ľ������
 * ���᷵�ر��ֵ��
 */
extern int alipay_iot_select(int          maxfdp1,
                      alipay_iot_fd_set  *readset,
                      alipay_iot_fd_set  *writeset,
                      alipay_iot_fd_set  *exceptset,
                      alipay_iot_timeval *timeout);

/**
 * ����-1��ʾʧ�ܣ�����0��ʾ�ɹ������᷵�ر��ֵ��
 */
extern int alipay_iot_dns(const char *name, unsigned char ip[4]);

/**
 * �����׽ӿڡ�
 * domainĿǰֻ֧��ALIPAY_IOT_SOC_PF_INET
 * typeΪSOC_SOCK_STREAM��ʾTCP��ΪSOC_SOCK_DGRAM��ʾUDP��
 * protocol��0������type����SOC_SOCK_RAW����Ҫָ������֮һ��
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
 * ����-1��ʾʧ�ܣ���������Ϊ�׽ӿڵ���������
 */
extern int alipay_iot_socket_create(int domain, alipay_socket_type_enum type, int protocol);

/**
 * �ر��׽ӿڡ�
 * ����-1��ʾʧ�ܣ�����0��ʾ�ɹ������᷵�ر��ֵ��
 */
extern int alipay_iot_socket_close(int s);


struct alipay_iot_sockaddr {
  unsigned char sa_len;      //��ֵ���ԡ�
  unsigned char sa_family;   //Ŀǰֻ֧��ALIPAY_IOT_SOC_PF_INET��
  union {
    struct {
        unsigned short port; //�����ֽ���
        unsigned char  ip[4];
    }    sin_data;
    char sa_data[14];
  }             data;
};

/**
 * �����׽ӿڵı��ص�ַ��
 * name->sa_len��Ч��
 * name->sa_family������SOC_PF_INET��
 * name->data.sin_data.port��name->data.sin_data.ip�ֱ�����˿ںź�IPv4��ַ��
 * �˿ںű����������ֽ��򣬼������
 * namelen��Ч��
 * ����-1��ʾʧ�ܣ�����0��ʾ�ɹ������᷵�ر��ֵ��
 */
extern int alipay_iot_socket_bind(int                        s,
                    const struct alipay_iot_sockaddr *name,
                    unsigned int               namelen);

/**
 * ����TCP�׽ӿڣ�����ָ����ַ�������ӣ�
 * ����UDP�׽ӿڣ�������ȱʡ�ĶԶ˵�ַ��
 * name->sa_len��Ч��
 * name->sa_family������SOC_PF_INET��
 * name->data.sin_data.port��name->data.sin_data.ip�ֱ�����˿ںź�IPv4��ַ��
 * �˿ںű����������ֽ��򣬼������
 * namelen��Ч��
 * ����-1��ʾʧ�ܣ�����-2��ʾ�첽�������ڽ����У�����0��ʾ�ɹ������᷵�ر��ֵ��
 */
extern int alipay_iot_socket_connect(int                            s,
                        const struct alipay_iot_sockaddr *name,
                        unsigned int                    namelen);

/**
 * ��ָ����ַ�������ݡ�һ������UDP�׽ӿڡ�
 * flags��Ч��
 * to->sa_len��Ч��
 * to->sa_family������SOC_PF_INET��
 * to->data.sin_data.port��to->data.sin_data.ip�ֱ�����˿ںź�IPv4��ַ��
 * �˿ںű����������ֽ��򣬼������
 * tolen��Ч��
 * ����-1��ʾʧ�ܣ�����-2��ʾ����������������0��������ʾ�ɹ����͵��ֽ�����
 * ���᷵�ر��ֵ��
 */
extern int alipay_iot_socket_sendto(int                           s,
                        const void                  *dataptr,
                        size_t                          size,
                        int                            flags,
                        const struct alipay_iot_sockaddr *to,
                        unsigned int                    tolen);

/**
 * �������ݡ�
 * ����-1��ʾʧ�ܣ�����-2��ʾ����������������0��������ʾ�ɹ����͵��ֽ�����
 * ���᷵�ر��ֵ��
 */
extern int alipay_iot_socket_write(int s, const void *dataptr, size_t len);

/**
 * �������ݲ���֪�Զ˵�ַ��һ������UDP�׽ӿڡ�
 * flags�����û���������������ֵ��
 *     SOC_MSG_OOB
 *     SOC_MSG_PEEK
 *     SOC_MSG_DONTROUTE
 *     SOC_MSG_EOR
 *     SOC_MSG_TRUNC
 *     SOC_MSG_CTRUNC
 *     SOC_MSG_WAITALL
 *     SOC_MSG_DONTWAIT
 * from->sa_len��Ч��
 * from->sa_family������SOC_PF_INET��
 * from->data.sin_data.port��from->data.sin_data.ip�ֱ�����˿ںź�IPv4��ַ��
 * �˿ںű����������ֽ��򣬼������
 * fromlenָ���ֵ��Ч��
 * ����-1��ʾʧ�ܣ�����-2��ʾ�����ݿɶ�������0��ʾ�Զ��ѹر�д��
 * ����������ʾ�ɹ����յ��ֽ��������᷵�ر��ֵ��
 */
extern int alipay_iot_socket_recvfrom(int                     s,
                        void                        *mem,
                        size_t                       len,
                        int                        flags,
                        struct alipay_iot_sockaddr *from,
                        unsigned int            *fromlen);

/**
 * �������ݡ�
 * ����-1��ʾʧ�ܣ�����-2��ʾ�����ݿɶ�������0��ʾ�Զ��ѹر�д��
 * ����������ʾ�ɹ����յ��ֽ��������᷵�ر��ֵ��
 */
extern int alipay_iot_socket_read(int s, void *mem, size_t len);

/**
 * �����׽ӿڵĲ�����
 * level��Ч��
 * optnameȡֵ����֮һ��
 *     SOC_OOBINLINE��Ŀǰ��֧��
 *     SOC_LINGER��opvalָ��soc_linger_struct
 *     SOC_NBIO��opvalָ��kal_bool
 *     SOC_ASYNC��opvalָ��kal_uint8
 *     SOC_NODELAY��opvalָ��kal_bool
 *     SOC_KEEPALIVE��opvalָ��kal_bool
 *     SOC_RCVBUF��opvalָ��kal_uint32
 *     SOC_SENDBUF��opvalָ��kal_uint32
 *     SOC_PKT_SIZE��opvalָ��kal_uint32
 *     SOC_SILENT_LISTEN��opvalָ��kal_bool
 *     SOC_QOS��opvalָ��kal_uint8
 *     SOC_TCP_MAXSEG��opvalָ��kal_uint8
 *     SOC_IP_TTL��opvalָ��kal_uint8
 *     SOC_LISTEN_BEARER��opvalָ��kal_uint8
 *     SOC_UDP_ANY_FPORT��opvalָ��kal_bool
 *     SOC_WIFI_NOWAKEUP��opvalָ��kal_bool
 *     SOC_UDP_NEED_ICMP��opvalָ��kal_bool
 *     SOC_IP_HDRINCL��opvalָ��kal_bool
 *     SOC_IPSEC_POLICY��opvalָ��sadb_x_policy
 *     SOC_TCP_ACKED_DATA��opvalָ��kal_uint32
 *     SOC_TCP_DELAYED_ACK��opvalָ��kal_bool
 *     SOC_TCP_SACK��opvalָ��kal_bool
 *     SOC_TCP_TIME_STAMP��opvalָ��kal_bool
 *     SOC_TCP_ACK_MSEG��opvalָ��kal_bool
 * ����-1��ʾʧ�ܣ�����0��ʾ�ɹ������᷵�ر��ֵ��
 */
extern int alipay_iot_socket_setsockopt(int          s,
                          int          level,
                          int          optname,
                          const void  *opval,
                          unsigned int optlen);

/**
 * ��ȡ�׽ӿڵĲ�����
 * level��Ч��
 * optnameȡֵ����֮һ��
 *     SOC_OOBINLINE��Ŀǰ��֧��
 *     SOC_LINGER��opvalָ��soc_linger_struct
 *     SOC_NBIO��opvalָ��kal_bool
 *     SOC_ASYNC��opvalָ��kal_uint8
 *     SOC_NODELAY��opvalָ��kal_bool
 *     SOC_KEEPALIVE��opvalָ��kal_bool
 *     SOC_RCVBUF��opvalָ��kal_uint32
 *     SOC_SENDBUF��opvalָ��kal_uint32
 *     SOC_NREAD��opvalָ��kal_uint32
 *     SOC_PKT_SIZE��opvalָ��kal_uint32
 *     SOC_SILENT_LISTEN��opvalָ��kal_bool
 *     SOC_QOS��opvalָ��kal_uint8
 *     SOC_TCP_MAXSEG��opvalָ��kal_uint8
 *     SOC_IP_TTL��opvalָ��kal_uint8
 *     SOC_LISTEN_BEARER��opvalָ��kal_uint8
 *     SOC_UDP_ANY_FPORT��opvalָ��kal_bool
 *     SOC_WIFI_NOWAKEUP��opvalָ��kal_bool
 *     SOC_UDP_NEED_ICMP��opvalָ��kal_bool
 *     SOC_IP_HDRINCL��opvalָ��kal_bool
 *     SOC_IPSEC_POLICY��opvalָ��sadb_x_policy
 *     SOC_TCP_ACKED_DATA��opvalָ��kal_uint32
 *     SOC_TCP_DELAYED_ACK��opvalָ��kal_bool
 *     SOC_TCP_SACK��opvalָ��kal_bool
 *     SOC_TCP_TIME_STAMP��opvalָ��kal_bool
 *     SOC_TCP_ACK_MSEG��opvalָ��kal_bool
 * ����-1��ʾʧ�ܣ�����0��ʾ�ɹ������᷵�ر��ֵ��
 */
extern int alipay_iot_socket_getsockopt(int         s,
                          int            level,
                          int          optname,
                          void          *opval,
                          unsigned int *optlen);

// #endif

#endif

