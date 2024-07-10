#ifndef LWM2M_SDK_H  
#define LWM2M_SDK_H  
  
#include <stdio.h>  
  
#ifdef __cplusplus
extern "C" {
#endif

#define LWM2M_SDK_LOCAL_PORT    (56831)

//通知类型
enum _SDK_NOTIFY_TYPE
{
      NOTIFY_TYPE_SYS           = 0x0000  //SDK运行
    , NOTIFY_TYPE_REGISTER      = 0x0100  //注册
    , NOTIFY_TYPE_DEREGISTER              //注销
    , NOTIFY_TYPE_REG_UPDATE              //更新
};
typedef enum _SDK_NOTIFY_TYPE SDK_NOTIFY_TYPE;


enum _SDK_NOTIFY_CODE
{
      NOTIFY_CODE_OK      = 0x0000 //成功
    , NOTIFY_CODE_FAILED 
    , NOTIFY_CODE_UNKNOWEN 
};
typedef enum _SDK_NOTIFY_CODE SDK_NOTIFY_CODE;

//sdk消息通知函数参数
typedef struct
{

	SDK_NOTIFY_TYPE notify_type;
	SDK_NOTIFY_CODE notify_code;
	const char *    notify_msg;
	
} OptNotifyParam;


typedef struct
{
    //sdk消息通知函数--no block
	void  (*NotifyMsg) (OptNotifyParam *optNotifyParam);
	
	//采集设备信息的读函数--函数不能长时阻塞
	//resId  : 设备信息资源号；见规范文档；如6602等；
	//outbuff: resId对应的设备的字符串值； 需要函数内部malloc字符串的存储空间；sdk内部使用完会释放；
	//return： 0；成功； 其他失败
	int   (*DMReadInfo) (int resId,char **outbuff);
	
} OptFuncs;


typedef struct
{
	char szCMEI_IMEI[64]; 	//CMEI/IMEI
	char szCMEI_IMEI2[64]; 	//CMEI/IMEI2
	char szIMSI[64];		//IMSI
	char szDMv[16]; 		//DM版本号
	char szAppKey[64]; 		//appkey
	char szPwd[64];			//pwd秘钥
	
	int  nAddressFamily; 	//4//或6  ipv4，ipv6
	char szSrvIP[32];		//127.0.0.1
    int  nSrvPort;   		//5683
	int  nLifetime;    		//300
	int  nLocalPort;        //本地端口绑定,默认为56830
	int  nBootstrap; 		//是否booststrap 0,not use; other n,use;
#if 1 //modify by unisoc
       char bUseSDKRegUpdate;//use sdk registration update
#endif	
} Options;

//初始化Options:参数,optfuncs:回调函数
int LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs); 
//启动sdk，nMode=0,阻塞在当前线程运行；nMode!=0,在新线程中运行;
int LWM2M_SDK_RUN(int nMode);
//退出SDK的循环；注销lwm2m服务
int LWM2M_SDK_STOP();
//清理sdk,参数等
int LWM2M_SDK_FINI();

#if 1 //modify by unisoc
/* FUNCTION: LWM2M_SDK_UPDATE_REG()
 * Option.bUseSDKRegUpdate = 0: when the lifetime arrives, APP calls this
 * interface to perform registration update. Successfully callback myNotifyMsg 
 * will return NOTIFY_TYPE_REG_UPDATE with success code.
 * RETURNS: 
 */
void LWM2M_SDK_UPDATE_REG(void);

/* FUNCTION: LWM2M_SDK_UPDATE_SOCKET()
 * SDK first successfully registered: if the data link is disconnected, it will
 * be called after successful reactivation of pdp, the SDK will execute to 
 * recreate the soket.
 * RETURNS: 
 */
void LWM2M_SDK_UPDATE_SOCKET(void);

/* FUNCTION: LWM2M_SDK_IS_RUN()
 * Used to determine whether the lwm2m SKD is running
 * RETURNS: 
 */
int LWM2M_SDK_IS_RUN(void);
#endif

#ifdef __cplusplus
}
#endif
  
#endif
