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
    ALIPAY_IOT_NETWOERK_ERR_ok=0,        //�޴�
    ALIPAY_IOT_NETWOERK_ERR_alloc_mem=1, //�����ڴ�ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_socket=2,    //�����׽ӿ�ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_bind=3,      //�󶨱��ص�ַʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_connect=4,   //����ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_output=5,    //��������ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_input=6,     //��������ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_link=7,      //���ӶϿ���
    ALIPAY_IOT_NETWOERK_ERR_ssl_init=8,  //SSL��ʼ��ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_ssl_hs=9,    //SSL����ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_ssl_en=10,    //SSL����ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_ssl_de=11,    //SSL����ʧ�ܡ�
    ALIPAY_IOT_NETWOERK_ERR_app=12,       //�ϲ�Ӧ�ô������
    ALIPAY_IOT_NETWOERK_ERR_dns=13,       //DNS����ʧ��
    ALIPAY_IOT_NETWOERK_ERR_timeout=14    //��ʱ
} ALIPAY_IOT_NETWOERK_errcode_t;

ALIPAY_IOT_NETWOERK_errcode_t do_mbedtls_demo(void);

#endif /* mbedtls_test_demo_h */
