#include "lwm2msdk.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if 0 //modify by unisoc
#include <unistd.h>
#else
#include "os_api.h"
#include "pthread.h"
#include "mmi_module.h"
#include "mmipdp_export.h"
#include "mmiconnection_export.h"
#endif

#define TRYTIMES 5   //失败重试，短间隔重试次数
static int s_quit = 0; //是否退出
static int s_state = 0;//运行状态
static int s_trytimes=TRYTIMES; //可以重试次数

#if 1 //modify by unisoc
#define printf          SCI_TraceLow
static int g_test_is_run = 0;
#define THIS_APP_MODULE_ID  MMI_MODULE_MMIBROWSER
#define SIM_ID                          (0)
char lwm2m_pdp_is_active = 0;
char is_sdk_operation_finshed = 0; //Before APP deactive pdp must wait for SDK transport finished!
int lwm2m_socket_pdp(void);
#endif

//主动发信号处理退出
void handle_sig(int signum)
{
    s_quit = 1;
    LWM2M_SDK_STOP();
}

void  myNotifyMsg(OptNotifyParam *optNotifyParam)
{
   if(optNotifyParam==NULL)
    return ;

    printf("[lwm2m]myNotifyMsg:type=%0x,code=%d,msg=%s\n",optNotifyParam->notify_type,optNotifyParam->notify_code,optNotifyParam->notify_msg);
    switch (optNotifyParam->notify_type)
    {
    case NOTIFY_TYPE_SYS:
        if (optNotifyParam->notify_code == NOTIFY_CODE_OK )//成功
        {
            is_sdk_operation_finshed = 1;
        }
    break;
    case NOTIFY_TYPE_REGISTER://注册结果通知
        if(optNotifyParam->notify_code == NOTIFY_CODE_OK )//成功
        {
            s_state = 0;
            s_trytimes=TRYTIMES;
        }
        else//失败
        {
            s_state = 1;
        }
    break;
    case NOTIFY_TYPE_DEREGISTER://心跳结果通知
         s_state = 3;
    break;
    case NOTIFY_TYPE_REG_UPDATE:
        if(optNotifyParam->notify_code == NOTIFY_CODE_OK )//成功
        {
            s_state = 0;
            s_trytimes=TRYTIMES;
        }
        else//失败
        {
            s_state = 2;
        }
    break;
    default:
    break;  
    }
    
}


int myDMReadInfo(int resId,char **outbuff)
{
    int iret = 0;
    *outbuff = NULL;
///////////////////////////////////////

    char buff[512];
    int  buflen = sizeof(buff);
    memset(buff,0,sizeof(buff));
    //字段赋值参看 多形态终端上报接口规范V2.0.3(多形态二期).docx
    //不支持:unSupport
    //没有值:unKnown
    snprintf(buff,buflen,"%s","unknown");//默认无法取值
    
    switch (resId)
    {
    case 6601://devinfo
        snprintf(buff,buflen,"%s","");  
        break;
    case 6602://appinfo
        snprintf(buff,buflen,"app_name1|pak_name1|1234000|3\r\napp_name2|pak_name2|2234000|5\r\n");  
        break;
    case 6603://mac
        snprintf(buff,buflen,"00:50:56:C0:00:01");
        break;
    case 6604://rom
        snprintf(buff,buflen,"4GB");
        break;
    case 6605://ram
        snprintf(buff,buflen,"2GB");
        break;
    case 6606://CPU
        snprintf(buff,buflen,"ARM");
        break;
    case 6607://SYS VERSION
        snprintf(buff,buflen,"LINUX 2.6.18");
        break;
    case 6608://FIRMWARE VERSION
        snprintf(buff,buflen,"1.20.1");
        break;
    case 6609://FIRMWARE NAME
        snprintf(buff,buflen,"X-A10");
        break;    
    case 6610://Volte
        snprintf(buff,buflen,"0");
        break;
    case 6611://NetType
        snprintf(buff,buflen,"4G");
        break;
    case 6612://Net Account
        snprintf(buff,buflen,"Account");
        break;
    case 6613://PhoneNumber
        snprintf(buff,buflen,"13812345678");
        break;
    case 6614://Location
        snprintf(buff,buflen,"25.77701556036132,123.52958679200002,1");
        break;

    default:
        iret = -1;
        break;    
    }

///////////////////////////////
    if(iret==0)
    {
        //申请的空间,sdk会释放
        *outbuff = (char*)malloc(strlen(buff)+1);
        strcpy(*outbuff,buff);
    }
    
    printf("[lwm2m]myDMReadInfo,ID=%d,value=%s,iret=%d\n",resId,buff,iret);
    return iret;
}


#if 0 //modify by unisoc
int main(int argc, char *argv[])
#else
LOCAL int test_case = 1;
LOCAL int main(int argc, char *argv[])
#endif
{
    
    Options opt = 
    {
    .szCMEI_IMEI="861111003929110",//"861111003929109", //CMEI/IMEI  ;和IMEI2选一个填，参考规范文档;设备没有的话向移动申请；
    .szCMEI_IMEI2="",                 //CMEI/IMEI2 ;和IMEI选一个填，参考规范文档
    .szIMSI="460041850403690",  //IMSI;没有可设为空
    .szDMv="v2.0",             //DM版本号;固定v2.0
    .szAppKey="M100000001",       //appkey;  需申请
    .szPwd="ff8lv9P6I64fB51Y30n128K858rob353",//pwd秘钥; 需申请
    .nAddressFamily=4,                  //4或6； 表示ipv4，ipv6; 固定设为4;
    .szSrvIP="shipei.fxltsbl.com", //服务器host or ip : shipei.fxltsbl.com(适配)/m.fxltsbl.com(商用)
    .nSrvPort=5683,               //服务器端口：一般为5683
    .nLifetime=300,                //更新间隔，心跳，生命周期：单位s,商用设为一天86400s
    .nLocalPort=LWM2M_SDK_LOCAL_PORT,   //本地端口绑定
    .nBootstrap=0,                   //是否booststrap 0,not use; other n,use; 当前固定设为0；
    .bUseSDKRegUpdate = 0,
    };
    
	OptFuncs optfuncs={NULL,NULL};
    optfuncs.NotifyMsg  = myNotifyMsg;//sdk消息通知函数--no block
    optfuncs.DMReadInfo = myDMReadInfo;//采集设备信息的读函数--函数不能长时阻塞

	int nrunmode = 1; // thread

       g_test_is_run = 1;
    
	if(argc>1)
		nrunmode = atoi(argv[1]);
	
	if(argc>2)
	{
		strcpy(opt.szSrvIP,argv[2]);
	}
	
	if(argc>3)
	{
		strcpy(opt.szPwd,argv[3]);
	}

#if 0 //modify by unisoc
	signal(SIGINT, handle_sig);
#endif

	//init config
	if(strcmp(opt.szSrvIP,"shipei.fxltsbl.com")==0)//适配版15分钟
    {
    #if 0 //modify by unisoc
        if(opt.nLifetime<900) opt.nLifetime=900;
    #else
        if(opt.nLifetime<300) opt.nLifetime=300;
    #endif
    }
    if(strcmp(opt.szSrvIP,"m.fxltsbl.com")==0)//商用版24小时
    {
        if(opt.nLifetime<86400) opt.nLifetime=86400; 
    }
    
	int ret = LWM2M_SDK_INIT(&opt,&optfuncs);

	if(nrunmode==0)
	{
		while(s_quit==0)
		{
		    printf("[lwm2m][%s:%d]to run in main thread\n",__func__,__LINE__);
		    int sleeptime=60;
            if(s_trytimes<0) sleeptime=10000;
            
		    ret = LWM2M_SDK_STOP();
		    ret = LWM2M_SDK_RUN(0);
		    if(s_quit==0 && s_state != 0)
		    {
		        printf("[lwm2m]state=%d|sleep=%d,s_trytimes=%d\n",s_state,sleeptime,s_trytimes);
                #if 0 //modify by unisoc
		        sleep(sleeptime);//休眠一段时间，重新注册
                #else
                      SCI_Sleep(sleeptime*1000);
                #endif
		        s_trytimes--;
                    s_state=0;
		        continue;
		    }	    	
		    
		    
		}
	}
	else
	{
            printf("[lwm2m]to run in created thread\n");
            while(s_quit==0)
            {
                int sleeptime=60;
                if(s_trytimes<0) sleeptime=10000;

                if (!lwm2m_pdp_is_active)
                {
                    lwm2m_socket_pdp();  //Before send registration update must active PDP first
                    SCI_Sleep(10 *1000);
                }

                //Phase 1 registration
                is_sdk_operation_finshed = 0; //clear SDK operation status
                ret = LWM2M_SDK_RUN(1); //First registration

                SCI_Sleep(10 *1000);

                while (!is_sdk_operation_finshed && (s_state == 0)) //SDK is doing registration wait for registration finished.
                {
                    printf("[lwm2m] Wait lwm2m SDK operation finished! %d\n", __LINE__);
                    SCI_Sleep(10 *1000);
                }

                if (is_sdk_operation_finshed) //SDK has finished registration.
                {
                    printf("[lwm2m] SDK operation finished! %d\n", __LINE__);
                    LWM2M_SDK_FINI();
                    SCI_Sleep(2 *1000);
                    lwm2m_testsdk_exit(); //Deactive PDP
                }
                    
                printf("[lwm2m]to run in test thread,test_case = %d, s_trytimes=%d\n", test_case, s_trytimes);
                while(s_quit==0)
                {
                     SCI_Sleep(10 *1000);

                    //Phase 2 registration update
                    if (opt.bUseSDKRegUpdate == 0)//APP do regsitration update
                    {                        
                        printf("[lwm2m]Start update registration ,%d \n", __LINE__);

                        lwm2m_socket_pdp();  //Before send registration update must active PDP first

                        while (!lwm2m_pdp_is_active) //Make sure pdp is active success.
                        {
                            printf("[lwm2m] Wait PDP active success");
                            SCI_Sleep(5*1000);
                        }

                        printf("[lwm2m]SKD start do update registration ,%d \n", __LINE__);

                        is_sdk_operation_finshed = 0; //clear SDK operation status
                        LWM2M_SDK_UPDATE_REG(); //Registration update

                        while (!is_sdk_operation_finshed && (s_state == 0)) //SDK is doing update registration wait for update registration finished.
                        {
                            printf("[lwm2m] Wait lwm2m SDK operation finished!%d\n", __LINE__);
                            SCI_Sleep(5*1000);
                        }

                        printf("[lwm2m]SKD finished update registration ,%d \n", __LINE__);

                        if (is_sdk_operation_finshed) //SDK has finished  update registration.
                        {
                            LWM2M_SDK_FINI();
                            lwm2m_testsdk_exit(); //Deactive PDP
                        }

                        printf("[lwm2m] End update registration ,%d \n", __LINE__);
                    }

                    if (test_case == 0)  //assume one error occur
                    {
                        s_state = 1;
                        test_case++;
                    }

                    if(s_state != 0)
                    {
                        ret = LWM2M_SDK_STOP(); //SDK error! should stop and run the SDK again
                        printf("[lwm2m]satae error,state=%d|sleep=%d,s_trytimes=%d\n",s_state,sleeptime,s_trytimes);
#if 0 //modify by unisoc
                        sleep(sleeptime);//休眠一段时间，重新注册
#else
                        SCI_Sleep(sleeptime*1000);
#endif
                        s_trytimes--;
                        s_state=0;
                        break;
                    }
                    else
                    {
                        printf("[lwm2m]state=%d|sleep=%d,s_trytimes=%d\n",s_state,sleeptime,s_trytimes);                    
#if 0 //modify by unisoc
                        sleep(60);//休眠
#else
                        SCI_Sleep(sleeptime*1000);
#endif
                    }
                }
            }
	}

       g_test_is_run = 0;
	printf("[lwm2m]main thread to stop workthread\n");
	LWM2M_SDK_STOP();
	
	LWM2M_SDK_FINI();
	printf("[lwm2m]main quit \n");
	return 0;
}

#if  1 //modify by unisoc
#define THIS_APP_MODULE_ID  MMI_MODULE_MMIBROWSER
#define SIM_ID                          (0)

void lwm2m_testsdk_create_thread(void)
{
    SCI_TRACE_LOW("[lwm2m]Start %s\n", __FUNCTION__);
    pthread_t thread_id = 0;
    pthread_attr_t attr;

    if (g_test_is_run)
    {
         SCI_TRACE_LOW("[lwm2m]test is runing %s\n", __FUNCTION__);
         return;
    }
    dm_pthread_attr_init(&attr );
    attr.stack_size = 32  * 1024;

    dm_pthread_create(&thread_id, &attr, main, NULL);
    SCI_TRACE_LOW("[lwm2m] End %s thread_id=%d\n", __FUNCTION__, thread_id);
}

/*****************************************************************************/
//  Description : This is PDP callback
//                    When platform Actived/Deactived will send Event to this function.                     
//  Global resource dependence : 
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
LOCAL void lwm2m_socket_Handle_PdpMsg(MMIPDP_CNF_INFO_T *msg_ptr)
{
    int simindex = 0;

    SCI_TRACE_LOW("[lwm2m socket demo debug ------------>]%s       %d\n",__FUNCTION__,__LINE__);
    if(PNULL == msg_ptr)
    {
        SCI_TRACE_LOW("[lwm2m socket demo error]%s       %d\n",__FUNCTION__,__LINE__);
        return;
    }

    SCI_TRACE_LOW("[lwm2m socket demo debug]msg_ptr->msg_id = %d.", msg_ptr->msg_id);
    if (msg_ptr->app_handler != THIS_APP_MODULE_ID)
    {
        SCI_TRACE_LOW("[lwm2m socket demo error]%s       %d\n",__FUNCTION__,__LINE__);
        return;
    }
    
    switch(msg_ptr->msg_id)
    {
        /*Physical link established successfully
            we get IP/DSN ip  by nsapi.
        */
        case MMIPDP_ACTIVE_CNF:
            if(MMIPDP_RESULT_SUCC == msg_ptr->result)
            {
                lwm2m_pdp_is_active = 1;
                socket_SetNetId(msg_ptr->nsapi);
                SCI_TRACE_LOW("[lwm2m pdp active success, nsapi = %d]%s       %d\n",msg_ptr->nsapi,__FUNCTION__,__LINE__);
                //start socket task 
                if (LWM2M_SDK_IS_RUN())
                {
                }
                else
                {
                    lwm2m_testsdk_create_thread();
                }
            }
            else
            {
                MMIAPIPDP_Deactive(THIS_APP_MODULE_ID);
            }
            break;
        case MMIPDP_DEACTIVE_CNF:
            SCI_TRACE_LOW("[lwm2m socket demo debug deactive ok]%s       %d\n",__FUNCTION__,__LINE__);
            lwm2m_pdp_is_active = 0;
            break;
        case MMIPDP_DEACTIVE_IND:
            SCI_TRACE_LOW("[lwm2m socket demo debug start deactive]%s       %d\n",__FUNCTION__,__LINE__);
            lwm2m_pdp_is_active = 0;
            //MMIAPIPDP_Deactive(THIS_APP_MODULE_ID);
            break;
        default:
            break;
    }
    SCI_TRACE_LOW("[lwm2m socket demo debug <------------]%s       %d\n",__FUNCTION__,__LINE__);
}

/*****************************************************************************/
//  Description : This is PDP API.
//              Input: sim card . APN info and app module id.                                       
//  Global resource dependence : 
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
int lwm2m_socket_pdp(void)
{
    int i = 0;
    uint8 linkNum = 0;
    int result = -1;
    MMIPDP_ACTIVE_INFO_T    active_info = {0};
    char* apn_str = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T* linksetting = PNULL;

    SCI_TRACE_LOW("[lwm2m socket demo debug ------------>]%s       %d\n",__FUNCTION__,__LINE__);
    linkNum = MMIAPICONNECTION_GetLinkSettingNum(SIM_ID);
    for (i = 0; i < linkNum; i++)
    {
        linksetting = PNULL;
        linksetting = MMIAPICONNECTION_GetLinkSettingItemByIndex(SIM_ID, i);
        if(PNULL != linksetting && 0 != linksetting->apn_len)
        {
            apn_str = (char*)linksetting->apn;
            if (PNULL != apn_str)
            {
                break;
            }
        }
    }

    active_info.app_handler    = THIS_APP_MODULE_ID;
    active_info.dual_sys           = SIM_ID;
    active_info.apn_ptr           = apn_str;
    active_info.user_name_ptr   = NULL;
    active_info.psw_ptr            = NULL;
    active_info.priority             = 1;
    active_info.ps_service_rat   = MN_UNSPECIFIED;
    active_info.ps_interface      = MMIPDP_INTERFACE_GPRS;
    active_info.handle_msg_callback = lwm2m_socket_Handle_PdpMsg;
    active_info.ps_service_type = BROWSER_E;
    active_info.storage = MN_GPRS_STORAGE_ALL;
#ifdef IPVERSION_SUPPORT_V4_V6
    active_info.ip_type = MMICONNECTION_IP_V4;
#endif
    SCI_TRACE_LOW("[lwm2m socket demo debug simid %d,apn %s]%s                   %d\n",SIM_ID,apn_str,__FUNCTION__,__LINE__);
    result = MMIAPIPDP_Active(&active_info);
    SCI_TRACE_LOW("[lwm2m socket demo debug <------------]%s       %d, result=%d\n",__FUNCTION__,__LINE__, result);
    return result;
}

PUBLIC int lwm2m_testsdk_main(void)
{
    //Step1: Name - PDP actived
    SCI_TRACE_LOW("[lwm2m  mmideo debug Step1: Name - PDP actived]%s       %d\n",__FUNCTION__,__LINE__);
    return lwm2m_socket_pdp();
}

/*****************************************************************************/
//  Description :  exit function
//           This API must called in MMI thread.(May be User press some key)
//  Global resource dependence : 
//  Author: ak.wang
//  Note: 
/*****************************************************************************/
PUBLIC int lwm2m_testsdk_exit(void)
{
    //Step7: Name - PDP deactived
    int result = -1;

     lwm2m_pdp_is_active = 0;
     result = MMIAPIPDP_Deactive(THIS_APP_MODULE_ID);
     SCI_TRACE_LOW("[lwm2m  mmideo debug Step7: Name - PDP deactived]%s       %d result=%d\n",__FUNCTION__,__LINE__, result);

     return result;
}

#endif
