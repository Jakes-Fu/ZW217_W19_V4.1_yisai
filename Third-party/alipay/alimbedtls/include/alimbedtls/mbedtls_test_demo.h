//
//  mbedtls_test_demo.h
//  MbedtlsDemo
//
//  Created by cuinacai on 2019/6/20.
//  Copyright ? 2019 cuinacai. All rights reserved.
//

#ifndef mbedtls_test_demo_h
#define mbedtls_test_demo_h

#include <stdio.h>

typedef enum ALIPAY_IOT_NETWOERK_errcode {
    ALIPAY_IOT_NETWOERK_ERR_ok=0,        //无错。
    ALIPAY_IOT_NETWOERK_ERR_alloc_mem=1, //分配内存失败。
    ALIPAY_IOT_NETWOERK_ERR_socket=2,    //操作套接口失败。
    ALIPAY_IOT_NETWOERK_ERR_bind=3,      //绑定本地地址失败。
    ALIPAY_IOT_NETWOERK_ERR_connect=4,   //建连失败。
    ALIPAY_IOT_NETWOERK_ERR_output=5,    //发送数据失败。
    ALIPAY_IOT_NETWOERK_ERR_input=6,     //接收数据失败。
    ALIPAY_IOT_NETWOERK_ERR_link=7,      //连接断开。
    ALIPAY_IOT_NETWOERK_ERR_ssl_init=8,  //SSL初始化失败。
    ALIPAY_IOT_NETWOERK_ERR_ssl_hs=9,    //SSL握手失败。
    ALIPAY_IOT_NETWOERK_ERR_ssl_en=10,    //SSL加密失败。
    ALIPAY_IOT_NETWOERK_ERR_ssl_de=11,    //SSL解密失败。
    ALIPAY_IOT_NETWOERK_ERR_app=12,       //上层应用处理出错。
    ALIPAY_IOT_NETWOERK_ERR_dns=13,       //DNS解析失败
    ALIPAY_IOT_NETWOERK_ERR_timeout=14    //超时
} ALIPAY_IOT_NETWOERK_errcode_t;

ALIPAY_IOT_NETWOERK_errcode_t do_mbedtls_demo(void);

#endif /* mbedtls_test_demo_h */
