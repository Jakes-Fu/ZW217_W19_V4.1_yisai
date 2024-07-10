0.编译
原环境 linux_2.6_x86_64
依赖于库pthread;

makefile编译命令:
make all
make clean
make testsdk
make install

makefile编译说明:
-DLWM2M_CLIENT_MODE: 使用lwm2m客户端模式
-DLWM2M_SUPPORT_JSON: 在程序中支持json数据类型
-DLWM2M_LITTLE_ENDIAN: 编译平台的字节顺序为小端; //若为大端,请使用宏-DLWM2M_BIG_ENDIAN
-DLWM2M_WITH_LOGS: 显示lwm2m底层日志;


1.目录
core：lwm2m sources
sdksrc/sdk: sdk sources
sdksrc/testsdk: test use sdk example
sdksrc/shared:  common sources

2.函数参数说明
LWM2M_SDK_INIT传入参数
typedef struct
{
	char szCMEI_IMEI[64]; 	//CMEI/IMEI ;和IMEI2选一个填，参考规范文档
	char szCMEI_IMEI2[64]; 	//CMEI/IMEI2;
	char szIMSI[64];		//IMSI;
	char szDMv[16]; 		//DM版本号;固定v2.0
	char szAppKey[64]; 		//appkey;  需申请
	char szPwd[64];			//pwd秘钥; 需申请
	
	int  nAddressFamily; 	//4或6； 表示ipv4，ipv6; 固定设为4;
	char szSrvIP[32];		//服务器ip地址或host；如 shipei.fxltsbl.com 或 127.0.0.1；
    int  nSrvPort;   		//5683; 服务器端口
	int  nLifetime;    		//300s ;定期更新注册时间；单位秒 
	int  nLocalPort;       	//本地端口绑定,默认为56830
	int  nBootstrap; 		//是否booststrap 0,not use; other n,use; 当前固定设为0；
	
} Options;

typedef struct
{
    //sdk消息通知函数--no block
	//OptNotifyParam 通知类型和结果
	void  (*NotifyMsg) (OptNotifyParam *optNotifyParam);
	
	//采集设备信息的读函数--函数不能长时阻塞
	//resId  : 设备信息资源号；见规范文档；如6602等；
	//outbuff: resId对应的设备的字符串值； 需要函数内部malloc字符串的存储空间；sdk内部使用完会释放；
	//return： 0；成功； 其他失败
	int   (*DMReadInfo) (int resId,char **outbuff);
	
} OptFuncs;


//初始化Options:参数,optfuncs:回调函数
int LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs); 

//启动sdk，nMode=0,阻塞在当前线程运行；nMode!=0,在新线程中运行
int LWM2M_SDK_RUN(int nMode);

//退出SDK的循环；注销lwm2m服务
int LWM2M_SDK_STOP();

//清理sdk,参数等
int LWM2M_SDK_FINI();


//版本说明
M180525: 
自定义实现的aes加密算法(基于openssl的sdk不再同步更新); 
加入了IMEI2字段传入(个别厂商需求); 
参数可传入本地绑定端口;
断网bug修复;

M180606: 
消除部分编译告警信息

M180906: 
增加二阶段新字段的采集；DMReadInfo回调函数；
修复sha256在大端系统上的bug；

M190304: 
修复日志显示问题









