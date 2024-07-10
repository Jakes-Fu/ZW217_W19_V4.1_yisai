//
//  demo_platform.h
//  MbedtlsDemo
//
//  Created by cuinacai on 2019/6/20.
//  Copyright © 2019 cuinacai. All rights reserved.
//

#ifndef demo_platform_h
#define demo_platform_h
#include "alipay_iot_net_define.h"
#include <stdio.h>
typedef struct t_m{
    long tm_usec;
    unsigned int tm_time; //time in seconds
    int    tm_sec;        /* seconds after the minute [0-60] */
    int    tm_min;        /* minutes after the hour [0-59] */
    int    tm_hour;    /* hours since midnight [0-23] */
    int    tm_mday;    /* day of the month [1-31] */
    int    tm_mon;        /* months since January [0-11] */
    int    tm_year;    /* years since 1900 */
}TM_t;
/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
void BOX_init(void);
/*------------------------------------------------------------------------*/
void BOX_track(void       *arg,
               ALIPAY_IOT_LVL_t   lvl,
               const char *tag,
               const char *fmt,
               ...);
/*------------------------------------------------------------------------*/
void BOX_track_log(void       *arg,
                   ALIPAY_IOT_LVL_t   lvl,
                   const char *tag,
                   const char *fmt,
                   ...);
/*------------------------------------------------------------------------*/
void *BOX_mem_ctl(void      *arg,
                  void      *ptr,
                  ALIPAY_IOT_size_t size_old,
                  ALIPAY_IOT_size_t size_new);
/*------------------------------------------------------------------------*/
void BOX_mem_set(void *arg, void *ptr, int val, ALIPAY_IOT_size_t size);
void BOX_mem_cpy(void *arg, void *dst, const void *src, ALIPAY_IOT_size_t size);
void BOX_mem_mov(void *arg, void *dst, const void *src, ALIPAY_IOT_size_t size);
/*------------------------------------------------------------------------*/
ALIPAY_IOT_size_t BOX_str_len(void *arg, const char *str);
/*------------------------------------------------------------------------*/
unsigned int BOX_rnd_num(void *arg);
/*------------------------------------------------------------------------*/
ALIPAY_IOT_intptr_t BOX_sk_open_udp(void *arg);
ALIPAY_IOT_intptr_t BOX_sk_open_tcp(void *arg);
void BOX_sk_close(void *arg, ALIPAY_IOT_intptr_t sk);
ALIPAY_IOT_bool_t BOX_sk_bind(void *arg, ALIPAY_IOT_intptr_t sk, const ALIPAY_IOT_ADDR_t *addr);
ALIPAY_IOT_bool_t BOX_sk_connect(void              *arg,
                          ALIPAY_IOT_intptr_t      sk,
                          const ALIPAY_IOT_ADDR_t *addr);
ALIPAY_IOT_bool_t BOX_sk_post(void             *arg,
                       ALIPAY_IOT_intptr_t      sk,
                       const ALIPAY_IOT_ADDR_t *addr,
                       ALIPAY_IOT_BIN_t        *data);
ALIPAY_IOT_bool_t BOX_sk_send(void *arg, ALIPAY_IOT_intptr_t sk, ALIPAY_IOT_BIN_t *data);
ALIPAY_IOT_bool_t BOX_sk_obtain(void        *arg,
                         ALIPAY_IOT_intptr_t sk,
                         ALIPAY_IOT_ADDR_t  *addr,
                         ALIPAY_IOT_BIN_t   *data);
ALIPAY_IOT_bool_t BOX_sk_receive(void *arg, ALIPAY_IOT_intptr_t sk, ALIPAY_IOT_BIN_t *data);
ALIPAY_IOT_bool_t BOX_sk_nblk(void *arg, ALIPAY_IOT_intptr_t sk);
ALIPAY_IOT_bool_t BOX_sk_ndly(void *arg, ALIPAY_IOT_intptr_t sk);
ALIPAY_IOT_bool_t BOX_sk_rude(void *arg, ALIPAY_IOT_intptr_t sk);
ALIPAY_IOT_bool_t BOX_sk_polite(void *arg, ALIPAY_IOT_intptr_t sk);
/*------------------------------------------------------------------------*/
ALIPAY_IOT_bool_t BOX_ssl_open(void        *arg,
                        ALIPAY_IOT_intptr_t sk,
                        void        *ctx,
                        void       (*output)(void *, const ALIPAY_IOT_BIN_t *),
                        void       (*input)(void *, ALIPAY_IOT_BIN_t *),
                        const char  *host,
                        const char  *alpn[],
                        void        **tkt);
ALIPAY_IOT_bool_t BOX_ssl_handshake(void *arg, ALIPAY_IOT_intptr_t sk, ALIPAY_IOT_bool_t *fin);
ALIPAY_IOT_bool_t BOX_ssl_output(void *arg, ALIPAY_IOT_intptr_t sk, ALIPAY_IOT_BIN_t *data);
ALIPAY_IOT_bool_t BOX_ssl_input(void *arg, ALIPAY_IOT_intptr_t sk, ALIPAY_IOT_BIN_t *data);
/*------------------------------------------------------------------------*/
void BOX_tkt_drop(void *arg, void *tkt);
/*------------------------------------------------------------------------*/
void BOX_current(void *arg, ALIPAY_IOT_TIME_t *now);
void BOX_drive(void *arg, ALIPAY_IOT_EVENT_t *event, ALIPAY_IOT_size_t *count, int ms);
//get localtime
void BOX_LOG_time(void *arg,TM_t *time);
/*--------------------------------------------------------------------*/
//strstr 字符串查找
const char* BOX_LOG_strstr(void *arg,const char*str1,const char* str2);
/*--------------------------------------------------------------------*/
ALIPAY_IOT_bool_t BOX_dns(void *arg, const ALIPAY_IOT_BIN_t *name, unsigned char ip[]);
#endif /* demo_platform_h */
