#ifndef _MMIWRE_PRELOAD_H_
#define _MMIWRE_PRELOAD_H_

#define WRE_PRELOAD_SZIP                        0x01
#define WRE_PRELOAD_EXE                         0x02
#define WRE_PRELOAD_WPK                         0x04


/************************************************************************

   �綨���˴˺꣬��WRE����ʱ�������ƻ�ʱ��ѡ������Ŀ¼�ָ���
   ����Ĭ�ϴ���������WRE���л���

************************************************************************/
#define WRE_RUNTIME_ENVIRONMENT_PRELOAD     


/************************************************************************/
/* KernelĿ¼���                                                       */
/************************************************************************/
//����Ŀ¼
#if defined(WRE_WREKERNEL_PRELOAD)||defined(WRE_WREKERNEL_NORFLASH)
#define  WRE_WREKERNEL_PRELOAD_PATH             L"e:\\wre"
#define  WRE_WREKERNEL_PRELOAD_TYPE             (WRE_PRELOAD_EXE|WRE_PRELOAD_SZIP)
#endif

/************************************************************************/
/* StoreĿ¼���                                                       */
/************************************************************************/
//����Ŀ¼
#ifdef WRE_WRESTORE_PRELOAD
#define  WRE_WRESTORE_PRELOAD_PATH              L"e:\\wre"
#define  WRE_WRESTORE_PRELOAD_TYPE              (WRE_PRELOAD_EXE|WRE_PRELOAD_SZIP)
#endif

/************************************************************************/
/* AppĿ¼���                                                       */
/************************************************************************/
//����Ŀ¼
#ifdef WRE_WREAPP_PRELOAD
#define  WRE_WREAPP_PRELOAD_PATH                L"e:\\wreapp" 
#define  WRE_WREAPP_PRELOAD_TYPE                (WRE_PRELOAD_EXE)

#define WRE_PREINSTALL_APP_SUPPORT
#endif
  
typedef enum
{
#ifdef SNS_PULLING_SUPPORT
    WRE_FIX_ID_SINAWB,
    WRE_FIX_ID_FACBK,
    WRE_FIX_ID_TWITR,
    WRE_FIX_ID_UTUBE,
#else
    //WRE_FIX_ID_DIGGOLD = 0,
    //WRE_FIX_ID_SINA,    
    //WRE_FIX_ID_FACEBOOK,
    //WRE_FIX_ID_TWITTER,
    WRE_FIX_ID_FRUIT,
#endif //SNS_PULLING_SUPPORT
    MAX_WRE_FIXED_APP_NUM,//���ڼ��������ж���
}WRE_FIXED_APP_ID_E;



#endif

