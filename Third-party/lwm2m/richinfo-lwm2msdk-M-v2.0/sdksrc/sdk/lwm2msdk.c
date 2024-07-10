#include "liblwm2m.h"
#include "connection.h"

#include <string.h>
#include <stdlib.h>
#if 0 //modify by unisoc
#include <unistd.h>
#endif
#include <stdio.h>
#if 0 //modify by unisoc
#include <ctype.h>
#include <sys/select.h>
#include <sys/types.h>
#endif
#include <sys/socket.h>
#if 0 //modify by unisoc
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#endif
#include "j_aes.h"
#include "j_base64.h"
#include <pthread.h>
#include "lwm2msdk.h"
#include "lwm2mclient.h"
#if 1 //modify by unisoc
#include "in_message.h"
#include "mmipdp_export.h"
#endif

#define MAX_PACKET_SIZE 1024
#define OBJ_COUNT 4

//lwm2m_object_t * objArray[OBJ_COUNT];
int g_reboot = 0;
static int g_quit = -1;//

pthread_t 	g_lwsdktid;
static  int	s_thrd_flag = 0;
int 		g_pip_ios_fd[2] ;
char g_szEPI[256]="";
Options  g_opt = {"CMEI_IMEI","CMEI_IMEI2","IMSI", "v1.0","appkey123456","",4,"127.0.0.1",5683,300,56380,0};
OptFuncs g_sdk_opt_funcs = {NULL};
LOCAL BLOCK_ID g_lwm2m_task_id = SCI_INVALID_BLOCK_ID;
LOCAL SCI_TIMER_PTR g_lwm2m_timer = PNULL;

#if 1 //modify by unisoc
#define SIGPIPE (1)
#define LWM2M_THREAD_PRIORITY           (68) // priority should be lower than DAPS(current is 67)
typedef enum _UNISOC_LWM2MSDK_SIG_E
{
    UNISOC_LWM2MSDK_REGISTRATION_UPDATE_SIG,
    UNISOC_LWM2MSDK_UPDATE_SOCKET_SIG,
    UNISOC_LWM2MSDK_TIMER_SIG,
    UNISOC_LWM2MSDK_EXIT_SIG,
    UNISOC_PDP_DEACTIVE_SIG
}UNISOC_LWM2MSDK_SIG_E;
#endif

typedef struct
{
    lwm2m_object_t * securityObjP;
    int sock;
    connection_t * connList;
    int addressFamily;
} client_data_t;


//the message body define begin
typedef struct _UNISOC_LWM2MSDK_SIG_T
{
    _SIGNAL_VARS
}UNISOC_LWM2MSDK_SIG_T;

LOCAL void LWM2M_CreateTimer(int msec);
LOCAL void LWM2M_StopTimer();
LOCAL void LWM2M_SDK_SEND_MSG(uint32 MSG);

void handle_sigint(int signum)
{
    g_quit = 1;
}

void * lwm2m_connect_server(uint16_t secObjInstID,
                            void * userData)
{
    client_data_t * dataP;
    char * uri;
    char * host;
    char * port;
    connection_t * newConnP = NULL;

    dataP = (client_data_t *)userData;

    uri = get_server_uri(dataP->securityObjP, secObjInstID);

    if (uri == NULL) return NULL;

    LOG_M("Connecting to %s\r\n", uri);

    // parse uri in the form "coaps://[host]:[port]"
    if (0 == strncmp(uri, "coaps://", strlen("coaps://")))
    {
        host = uri+strlen("coaps://");
    }
    else if (0 == strncmp(uri, "coap://", strlen("coap://")))
    {
        host = uri+strlen("coap://");
    }
    else
    {
        LOG_S("Error");
        goto exit;
    }
    port = strrchr(host, ':');
    if (port == NULL) 
    {
        LOG_S("Error port == NULL");
        goto exit;
    }
    // remove brackets
    if (host[0] == '[')
    {
        host++;
        if (*(port - 1) == ']')
        {
            *(port - 1) = 0;
        }
        else
        {        
            LOG_S("Error");
            goto exit;
        }
    }
    // split strings
    *port = 0;
    port++;

    newConnP = connection_create(dataP->connList, dataP->sock, host, port, dataP->addressFamily);
    if (newConnP == NULL) {
        LOG_S( "Connection creation failed.\r\n");
    }
    else {
        dataP->connList = newConnP;
    }

exit:
    lwm2m_free(uri);
    LOG_M("lwm2m_connect_server newConnP=%p", newConnP);
    return (void *)newConnP;
}

void lwm2m_close_connection(void * sessionH,
                            void * userData)
{
    client_data_t * app_data;
    connection_t * targetP;

    app_data = (client_data_t *)userData;
    targetP = (connection_t *)sessionH;

    if (targetP == app_data->connList)
    {
        app_data->connList = targetP->next;
        lwm2m_free(targetP);
    }
    else
    {
        connection_t * parentP;

        parentP = app_data->connList;
        while (parentP != NULL && parentP->next != targetP)
        {
            parentP = parentP->next;
        }
        if (parentP != NULL)
        {
            parentP->next = targetP->next;
            lwm2m_free(targetP);
        }
    }
}


void print_state(lwm2m_context_t * lwm2mH)
{

#ifdef LWM2M_WITH_LOGS

    lwm2m_server_t * targetP;

    LOG_S( "Context State: ");
    switch(lwm2mH->state)
    {
    case STATE_INITIAL:
        #if 0 //modify by unisoc
        fprintf(stderr, "STATE_INITIAL");
        #else
        LOG_S("STATE_INITIAL");
        #endif
        break;
    case STATE_BOOTSTRAP_REQUIRED:
        #if 0 //modify by unisoc
        fprintf(stderr, "STATE_BOOTSTRAP_REQUIRED");
        #else
        LOG_S("STATE_BOOTSTRAP_REQUIRED");
        #endif
        break;
    case STATE_BOOTSTRAPPING:
        #if 0 //modify by unisoc
        fprintf(stderr, "STATE_BOOTSTRAPPING");
        #else
        LOG_S("STATE_BOOTSTRAPPING");
        #endif
        break;
    case STATE_REGISTER_REQUIRED:
        #if 0 //modify by unisoc
        fprintf(stderr, "STATE_REGISTER_REQUIRED");
        #else
        LOG_S("STATE_REGISTER_REQUIRED");
        #endif
        break;
    case STATE_REGISTERING:
        #if 0 //modify by unisoc
        fprintf(stderr, "STATE_REGISTERING");
        #else
        LOG_S("STATE_REGISTERING");
        #endif
        break;
    case STATE_READY:
        #if 0 //modify by unisoc
        fprintf(stderr, "STATE_READY");
        #else
        LOG_S("STATE_READY");
        #endif
        break;
    default:
        #if 0 //modify by unisoc
        fprintf(stderr, "Unknown !");
        #else
        LOG_S("Unknown1");
        #endif
        break;
    }
    #if 0 //modify by unisoc
    fprintf(stderr, "\r\n");
    #endif

    targetP = lwm2mH->bootstrapServerList;

    if (lwm2mH->bootstrapServerList == NULL)
    {
        LOG_S( "No Bootstrap Server.\r\n");
    }
    else
    {
        LOG_S("Bootstrap Servers:\r\n");
        for (targetP = lwm2mH->bootstrapServerList ; targetP != NULL ; targetP = targetP->next)
        {
            #if 0 //modify by unisoc
            fprintf(stderr, " - Security Object ID %d", targetP->secObjInstID);
            fprintf(stderr, "\tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
            fprintf(stderr, "\tstatus: ");
            #else
            LOG_M(" - Security Object ID %d", targetP->secObjInstID);
            LOG_M("Hold Off Time: %lu s", (unsigned long)targetP->lifetime);
            LOG_S("status: ");
            #endif
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                #if 0 //modify by unisoc
                fprintf(stderr, "DEREGISTERED\r\n");
                #else
                LOG_S("DEREGISTERED");
                #endif
                break;
            case STATE_BS_HOLD_OFF:
                #if 0 //modify by unisoc
                fprintf(stderr, "CLIENT HOLD OFF\r\n");
                #else
                LOG_S("CLIENT HOLD OFF");
                #endif
                break;
            case STATE_BS_INITIATED:
                #if 0 //modify by unisoc
                fprintf(stderr, "BOOTSTRAP INITIATED\r\n");
                #else
                LOG_S("BOOTSTRAP INITIATED");
                #endif
                break;
            case STATE_BS_PENDING:
                #if 0 //modify by unisoc
                fprintf(stderr, "BOOTSTRAP PENDING\r\n");
                #else
                LOG_S("BOOTSTRAP PENDING");
                #endif
                break;
            case STATE_BS_FINISHED:
                #if 0 //modify by unisoc
                fprintf(stderr, "BOOTSTRAP FINISHED\r\n");
                #else
                LOG_S("BOOTSTRAP FINISHED");
                #endif
                break;
            case STATE_BS_FAILED:
                #if 0 //modify by unisoc
                fprintf(stderr, "BOOTSTRAP FAILED\r\n");
                #else
                LOG_S("BOOTSTRAP FAILED");
                #endif
                break;
            default:
                #if 0 //modify by unisoc
                fprintf(stderr, "INVALID (%d)\r\n", (int)targetP->status);
                #else
                LOG_M("INVALID (%d)", (int)targetP->status);
                #endif
            }
            #if 0 //modify by unisoc
            fprintf(stderr, "\r\n");
            #endif
        }
    }

    if (lwm2mH->serverList == NULL)
    {
        LOG_S( "No LWM2M Server.\r\n");
    }
    else
    {
        LOG_S( "LWM2M Servers:\r\n");
        for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
        {
            #if 0 //modify by unisoc
            fprintf(stderr, " - Server ID %d", targetP->shortID);
            fprintf(stderr, "\tstatus: ");
            #else
            LOG_M(" - Server ID %d", targetP->shortID);
            LOG_S("\tserverList status: ");
            #endif
            switch(targetP->status)
            {
            case STATE_DEREGISTERED:
                #if 0 //modify by unisoc
                fprintf(stderr, "DEREGISTERED\r\n");
                #else
                LOG_S("DEREGISTERED");
                #endif
                break;
            case STATE_REG_PENDING:
                #if 0 //modify by unisoc
                fprintf(stderr, "REGISTRATION PENDING\r\n");
                #else
                LOG_S("REGISTRATION PENDING");
                #endif
                break;
            case STATE_REGISTERED:
                #if 0 //modify by unisoc
                fprintf(stderr, "REGISTERED\tlocation: \"%s\"\tLifetime: %lus\r\n", targetP->location, (unsigned long)targetP->lifetime);
                #else
                LOG_M("REGISTERED \tlocation: \"%s\" \tLifetime: %lus", targetP->location, (unsigned long)targetP->lifetime);
                #endif
                break;
            case STATE_REG_UPDATE_PENDING:
                #if 0 //modify by unisoc
                fprintf(stderr, "REGISTRATION UPDATE PENDING\r\n");
                #else
                LOG_S("REGISTRATION UPDATE PENDING");
                #endif
                break;
            case STATE_REG_UPDATE_NEEDED:
                #if 0 //modify by unisoc
                fprintf(stderr, "REGISTRATION UPDATE REQUIRED\r\n");
                #else
                LOG_S("REGISTRATION UPDATE REQUIRED");
                #endif
                break;
            case STATE_DEREG_PENDING:
                #if 0 //modify by unisoc
                fprintf(stderr, "DEREGISTRATION PENDING\r\n");
                #else
                LOG_S("DEREGISTRATION PENDING");
                #endif
                break;
            case STATE_REG_FAILED:
                #if 0 //modify by unisoc
                fprintf(stderr, "REGISTRATION FAILED\r\n");
                #else
                LOG_S("REGISTRATION FAILED");
                #endif
                break;
            default:
                #if 0 //modify by unisoc
                fprintf(stderr, "INVALID (%d)\r\n", (int)targetP->status);
                #else
                LOG_M("INVALID (%d)", (int)targetP->status);
                #endif
            }
            #if 0 //modify by unisoc
            fprintf(stderr, "\r\n");
            #endif
        }
    }

#endif

}


void lwm2m_sdk_notify(SDK_NOTIFY_TYPE type,SDK_NOTIFY_CODE code,int ret,const char *notify_msg)
{
   OptNotifyParam optNotifyParam;
   char szmsg[1024];
   
    if(g_sdk_opt_funcs.NotifyMsg==NULL)
    return ;
    
    optNotifyParam.notify_type = type;
    optNotifyParam.notify_code = code;
    if(code == NOTIFY_CODE_UNKNOWEN)
    {
        if(ret==0)
        optNotifyParam.notify_code=NOTIFY_CODE_OK;
        else if(ret<0)
        optNotifyParam.notify_code=NOTIFY_CODE_FAILED;
    }
 
    memset(szmsg,0,sizeof(szmsg));
    sprintf(szmsg,"{ret=%0x,msg=%s}",ret,notify_msg);
    optNotifyParam.notify_msg = szmsg;
    g_sdk_opt_funcs.NotifyMsg(&optNotifyParam);
}

void lwm2m_sdk_notify_sys(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,notify_msg);
}

void lwm2m_sdk_notify_reg_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REGISTER,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_reg_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REGISTER,NOTIFY_CODE_FAILED,ret,notify_msg);
}
void lwm2m_sdk_notify_dereg_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_DEREGISTER,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_dereg_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_DEREGISTER,NOTIFY_CODE_FAILED,ret,notify_msg);
}
void lwm2m_sdk_notify_regup_success(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REG_UPDATE,NOTIFY_CODE_OK,ret,notify_msg);
}
void lwm2m_sdk_notify_regup_failed(int ret,const char *notify_msg)
{
    lwm2m_sdk_notify(NOTIFY_TYPE_REG_UPDATE,NOTIFY_CODE_FAILED,ret,notify_msg);
}


int to_encode(char *szin,char **szout)
{
	char shaout[256];
	int i;
	unsigned char *pciphertext;
	unsigned char *testbase64;
	unsigned char *encData;
	unsigned int encDataLen;
	int ret;
	unsigned char key[64] = {0};
	unsigned char iv[16] = {0};
	char *name = szin;
	int ciphertext_len=0;
	char *plaintext =  "The quick brown fox jumps over the lazy dog1234";
    //AES CBC
	/* A 256 bit key */  
	char *passwd = "00000000000000000000000000000000";
	if(strlen(g_opt.szPwd)>0)
	{
		passwd = g_opt.szPwd;
	}
	else
	{
		LOG_M("pwd is null,use default pwd is:%s~", passwd);
	}
	//
	memset(key,0,sizeof(key));
	
	memset(shaout,0,sizeof(shaout));
	StrSHA256(passwd,strlen(passwd), shaout);
	//printf("pwd=%s\n",passwd);
	//printf("shaout=%s\n",shaout);
	HexStrToByte(shaout,strlen(shaout),key);
	//hex_print("16sha:",key,32);
	
	/* A 128 bit IV */  
	memset(iv,0,16);
	i=0;
	for (i=0; i<16; ++i) {  
        iv[i] = 0;  
    } 
	   
	/* Buffer for the decrypted text */  
	//unsigned char decryptedtext[264]={0}; 

	//int decryptedtext_len=0,
	
	plaintext = name;
	//printf("Plaintext is:\n%s~\n", plaintext);

       pciphertext=NULL;
	ciphertext_len=EncryptionAES( plaintext, strlen(plaintext), key,&pciphertext);	
 
	//printf("Ciphertext is %d bytes long:\n", ciphertext_len);
	//hex_print("Ciphertext:",ciphertext,ciphertext_len);
	
	//name = ciphertext;
	
	//base64
	testbase64=(unsigned char *)"123456789";//MTIzNDU2Nzg5
	testbase64=pciphertext;
	
	encData=0;
	//unsigned char *decData=0;
	encDataLen=0;
	//unsigned int decDataLen=0;
	//printf("base64 inputlen=%d,strlen=%d\n",ciphertext_len,strlen(testbase64) );
	ret= j_base64_encode(testbase64, ciphertext_len, &encData, &encDataLen);
	if(ret<0)
	{
	    LOG_M("ret=%d~\n", ret);
	}
	//printf("base64 encData=%s~\n", encData);
	
	//memcpy(szout,encData,encDataLen);
	*szout = encData;
	
    //j_base64_free(encData, encDataLen);
    free(pciphertext);	
    return encDataLen;
	

}

int lwm2m_sdk_dm_get_devinfo(uint16_t resId,char **outbuff)
{ 
    char *szvalbuf = NULL;
    
    int iret = 0;
    *outbuff = NULL;
	
    if(g_sdk_opt_funcs.DMReadInfo==NULL)
    {
        iret = 111;
        LOG_M("resId=%u,g_sdk_opt_funcs.DMReadInfo==NULL,ret=%d",resId, iret);
        return iret;
    }
     
    do{
        int outlen;
        iret = g_sdk_opt_funcs.DMReadInfo(resId,&szvalbuf);
        if( iret!=0 || szvalbuf == NULL)
        {
            iret = 222;
            LOG_M("resId=%u,g_sdk_opt_funcs.DMReadInfo,ret=%d",resId, iret);
            break;
        }

        if(strlen(szvalbuf) == 0)
        {
            LOG_M("resId=%u,strlen(szvalbuf) == 0,ret=%d",resId, iret);
            break;
        }
        
        //data encode
        outlen = to_encode(szvalbuf,outbuff);
        //snprintf(buff,buflen,"resval_%d",resId);
        LOG_M("resId=%u,val=%s,encode_val=%s~",resId,szvalbuf,*outbuff);
   
    }while(0);
    
    if(szvalbuf) {free(szvalbuf); szvalbuf = NULL;}
    
    
    return iret;
    
}

#if 1 //modify by unisoc
int lwm2m_is_registered(lwm2m_context_t * lwm2mH)
{
    lwm2m_server_t * targetP;
    int isRegistered = 0;
    int isServerRegistered = 1;

    if (lwm2mH->serverList == NULL)
    {
        LOG_S( "No LWM2M Server.\r\n");
        return 0;
    }

    for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
    {
        if (targetP->status != STATE_REGISTERED)
        {
            isServerRegistered = 0;
        }
    }
    
    if ((lwm2mH->state == STATE_READY) && isServerRegistered)
    {
        isRegistered = 1;
    }
    LOG_M(" lwm2mH->state=%d, isServerRegistered=%d, isRegistered=%d", lwm2mH->state, isServerRegistered, isRegistered);
    return isRegistered;
}

void lwm2m_set_status_to_ready(lwm2m_context_t * lwm2mH)
{
    lwm2m_server_t * targetP;

    if (lwm2mH->serverList == NULL)
    {
        LOG_S( "No LWM2M Server.\r\n");
        return;
    }

    for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
    {
        targetP->status = STATE_REGISTERED;
    }

    lwm2mH->state = STATE_READY;
    LOG_S("Set SDK status to STATE_REGISTERED!");
    return;
}

int lwm2m_get_server_status(lwm2m_context_t *lwm2mH)
{
    if (lwm2mH->serverList == NULL)
    {
        LOG_S( "No LWM2M Server.\r\n");
        return 0;
    }

    LOG_M("lwm2mH->serverList->status=%d", lwm2mH->serverList->status);

    return lwm2mH->serverList->status;
}

int LWM2M_SDK_RUN_PARAM(int argc, char *argv[])
{
    client_data_t data;
    lwm2m_context_t * lwm2mH = NULL;
    lwm2m_object_t * objArray[OBJ_COUNT];
    char * server = "127.0.0.1";
    char * serverPort = "5683";
    char * localPort = "56830";
    //char * initname = "testlwm2mclient";
    char * name = "";
    int lifetime=300;
    int result=0;
    char * multiVer="v2.0";
    char imeiFlag[65];
    xSignalHeaderRec *sig_ptr = PNULL;
    lwm2m_server_t *targetP = NULL;
    connection_t *conn = NULL;
    char szsrvport[10]={0};
    char szlocalport[10]={0};
    char *szStars="****";
    char szEpname[256]={0};
    char szEPIsrc[256]={0};
    char szEpnameCode[256] = {0};
    int nEpnamecodeLen = 0;
    char *encode_out = NULL;
    char epnametmp[256];
    char serverUri[50];
    int serverId = 123;
    struct timeval tv;
    int timeout_cnt = 0;

    g_lwm2m_task_id = SCI_IdentifyThread();
    //int opt;
    memset(&data, 0, sizeof(client_data_t));

    data.addressFamily = AF_INET6;
    //localPort = argv[opt];
    //param set by optional	
    if (strlen(g_opt.szSrvIP)>=0)
    {
        server=g_opt.szSrvIP;
    }
	
    if (g_opt.nSrvPort > 80 ) 
    {
    	sprintf(szsrvport,"%d",g_opt.nSrvPort);
    	serverPort = szsrvport;		
    }
	
    if(g_opt.nAddressFamily==4) 
    {
        data.addressFamily = AF_INET;
    }

    if ( g_opt.nLifetime > 10 ) 
    {
        lifetime=g_opt.nLifetime;
    }

    if (g_opt.nLocalPort > 49152 ) 
    {
    	sprintf(szlocalport,"%d",g_opt.nLocalPort);
    	localPort = szlocalport;		
    }

    if(strlen(g_opt.szCMEI_IMEI)>0)
    {    
        strcpy(imeiFlag,g_opt.szCMEI_IMEI);
    }
    else
    {    
        strcpy(imeiFlag,g_opt.szCMEI_IMEI2);
    }
    LOG_M("IMEI:%s", imeiFlag);
    sprintf(szEpname,"%s-%s-%s-%s",
    strlen(g_opt.szCMEI_IMEI)>0?g_opt.szCMEI_IMEI:szStars,
    strlen(g_opt.szCMEI_IMEI2)>0?g_opt.szCMEI_IMEI2:szStars,
    strlen(g_opt.szIMSI)>0?g_opt.szIMSI:szStars,	
    strlen(g_opt.szDMv)>0?g_opt.szDMv:szStars
    );

    sprintf(szEPIsrc,"%s-%s-%s-%s",
	strlen(g_opt.szCMEI_IMEI)>0?g_opt.szCMEI_IMEI:szStars,
	strlen(g_opt.szCMEI_IMEI2)>0?g_opt.szCMEI_IMEI2:szStars,
	strlen(g_opt.szIMSI)>0?g_opt.szIMSI:szStars,
	szStars
	);

    multiVer = strlen(g_opt.szDMv)>0?g_opt.szDMv:szStars;

    memset(szEpnameCode,0,sizeof(szEpnameCode));
    memset(g_szEPI,0,sizeof(g_szEPI));
    name = szEpname;

    nEpnamecodeLen = to_encode(szEpname,&encode_out);
    memcpy(szEpnameCode,encode_out,nEpnamecodeLen);
    free(encode_out);
   
    memset(epnametmp,0,sizeof(epnametmp));
    sprintf(epnametmp,"I@#@%s@#@%s@#@%s@#@%s",szEpnameCode,g_opt.szAppKey,imeiFlag,multiVer);

    //strcpy(epnametmp,"L@#@M+D2vyO9kk/IPVyzehd6Biypy8R69yTlHdaIQiTp6klbXqJmESZJ6QXoDlQkqOC8@#@M100000006");
    LOG_M("epname=%s~\n", epnametmp);
    name = epnametmp;

    nEpnamecodeLen = to_encode(szEPIsrc,&encode_out);
    sprintf(g_szEPI,"I@#@%s@#@%s@#@%s@#@%s",encode_out,g_opt.szAppKey,imeiFlag,multiVer);
    free(encode_out);
	
    //strcpy(g_szEPI,"L@#@M+D2vyO9kk/IPVyzehd6Bs2GeOZOGcEHYX1mWBROKSg=@#@M100000006");
	
    LOG_M("szEPI=%s~\n", g_szEPI);  

///////////////

    /*
     *This call an internal function that create an IPv6 socket on the port 5683.
     */
    LOG_M( "Trying to bind LWM2M Client to port %s\r\n", localPort);
    data.sock = create_socket(localPort, data.addressFamily);
    if (data.sock < 0)
    {
        LOG_S("Failed to open socket\r\n");
        return -1;
    }

    sprintf (serverUri, "coap://%s:%s", server, serverPort);

    //objArray[0] = get_security_object();
    objArray[0] = get_security_object(serverId, serverUri);
    if (NULL == objArray[0])
    {
        LOG_S( "Failed to create security object\r\n");
        return -1;
    }
    data.securityObjP = objArray[0];

    objArray[1] = get_server_object(serverId, lifetime);
    if (NULL == objArray[1])
    {
        LOG_S( "Failed to create server object\r\n");
        return -1;
    }

    objArray[2] = get_object_device();
    if (NULL == objArray[2])
    {
        LOG_S("Failed to create Device object\r\n");
        return -1;
    }

    objArray[3] = get_object_dm();
    if (NULL == objArray[2])
    {
        LOG_S("Failed to create Device object\r\n");
        return -1;
    }

    lwm2mH = lwm2m_init(&data);
    if (NULL == lwm2mH)
    {
        LOG_S( "lwm2m_init() failed\r\n");
        return -1;
    }
    lwm2mH->bUseSDKRegUpdate = g_opt.bUseSDKRegUpdate;
    result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray);
    if (result != 0)
    {
        LOG_M( "lwm2m_configure() failed: 0x%X\r\n", result);
        return -1;
    }

    //signal(SIGINT, handle_sigint);
    LOG_M("LWM2M Client \"%s\" started on port %s.\r\nUse Ctrl-C to exit.\r\n\n", name, localPort);
  
    /*
     * We now enter in a while loop that will handle the communications from the server
     */   
    tv.tv_sec = 60;
    tv.tv_usec = 0;
    print_state(lwm2mH);
    result = lwm2m_step(lwm2mH, &(tv.tv_sec));       
    LOG_M("lwm2m_step result=%d, tv.tv_sec=%d", result, tv.tv_sec);
    if (result != 0)
    {
        LOG_M( "lwm2m_step() failed: 0x%X\r\n", result);
    }
    LWM2M_CreateTimer( tv.tv_sec*1000); 

    g_quit = 0;
    while (0 == g_quit)
    {
        // Get sig From Queue
        LOG_S("Wait Signal");
        print_state(lwm2mH);
        sig_ptr = SCI_GetSignal( SCI_IdentifyThread());   
        if (sig_ptr == PNULL)
        {
            break;
        }
        LOG_M("SCI_GetSignalsig_ptr->SignalCode =%d", sig_ptr->SignalCode);
        // Process sig
        switch( sig_ptr->SignalCode )
        {
            case UNISOC_LWM2MSDK_TIMER_SIG:
            {
                LOG_S("UNISOC_LWM2MSDK_TIMER_SIG");
                tv.tv_sec = 60;
                tv.tv_usec = 0;
                result = lwm2m_step(lwm2mH, &(tv.tv_sec));
                LOG_M("lwm2m_step result=%d, tv.tv_sec=%d", result, tv.tv_sec);
                if (result != 0)
                {
                    LOG_M( "lwm2m_step() failed: 0x%X\r\n", result);
                    if (result == COAP_503_SERVICE_UNAVAILABLE)
                    {
                        timeout_cnt++;
                        LOG_M("503 Service Unavaliable Timeout! %d\r\n", timeout_cnt);
                        if (timeout_cnt > 2)
                        {
                            lwm2m_sdk_notify_reg_failed(NOTIFY_CODE_FAILED, "503 Service Unavaliable Timeout!");
                            break;
                        }
                    }
                }
                if (lwm2mH->bUseSDKRegUpdate || !lwm2m_is_registered(lwm2mH))
                {
                    LWM2M_CreateTimer( tv.tv_sec*1000); 
                }
                if (!lwm2mH->bUseSDKRegUpdate && lwm2m_is_registered(lwm2mH))
                {
                    lwm2m_sdk_notify_sys(NOTIFY_CODE_OK, "Registered-OK");
                }
            }
            break;
            case UNISOC_LWM2MSDK_EXIT_SIG:
            {
                LOG_S("UNISOC_LWM2MSDK_EXIT_SIG");
                g_quit =1;
            }
            break;    
            case UNISOC_LWM2MSDK_REGISTRATION_UPDATE_SIG:
            {                
                LOG_S("UNISOC_LWM2MSDK_REGISTRATION_UPDATE_SIG");
                if (!lwm2m_is_registered(lwm2mH))
                {
                    lwm2m_set_status_to_ready(lwm2mH);
                }
                tv.tv_sec = 60;
                tv.tv_usec = 0;
                lwm2mH->bDoRegUpdate = 1;
                result = lwm2m_step(lwm2mH, &(tv.tv_sec));
                lwm2mH->bDoRegUpdate = 0;
                LOG_M("lwm2m_step result=%d, tv.tv_sec=%d", result, tv.tv_sec);
                if (result != 0)
                {
                    LOG_M( "lwm2m_step() failed: 0x%X\r\n", result);
                }
                LWM2M_CreateTimer( tv.tv_sec*1000); 
            }
            break;
            case UNISOC_LWM2MSDK_UPDATE_SOCKET_SIG:
            {
                LOG_S("UNISOC_LWM2MSDK_UPDATE_SOCKET_SIG");
                // clear old socket
                dm_close(data.sock);

                //create new socket
                data.sock = create_socket(localPort, data.addressFamily);
                if (data.sock < 0)
                {
                    LOG_S("Failed to open socket:\r\n");
                    data.sock = 0;
                }
                targetP = lwm2mH->serverList;
                while (targetP != NULL)
                {
                    conn = targetP->sessionH;
                    if (conn != NULL)
                    {
                        conn->sock = data.sock;
                    }
                    else
                    {
                        LOG_S( "connection is NULL \r\n");
                    }
                    targetP = targetP->next;
               }
            }
            break;
            case SOCKET_READ_EVENT_IND:
            case SOCKET_READ_BUFFER_STATUS_EVENT_IND:
            {
                uint8_t buffer[MAX_PACKET_SIZE] = {0};
                int numBytes;
		  struct sockaddr_storage addr;
                socklen_t addrLen;
                LOG_S("SOCKET_READ_EVENT_IND/SOCKET_READ_BUFFER_STATUS_EVENT_IND");

                if (data.sock <= 0)
                {
                    LOG_S("socket closed!");
                    break;
                }
                /*
                * If an event happens on the socket
                */
                

                memset(&addr, 0, sizeof(addr));
                addrLen = sizeof(addr);
                /*
                 * We retrieve the data received
                 */
                while ((numBytes = dm_recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen)) > 0)
                {
                      connection_t * connP;
					  
			LOG_M("Data recv numBytes=%d", numBytes);

                    connP = connection_find(data.connList, &addr, addrLen);
                    if (connP != NULL)
                    {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
                    }
                    else
                    {
                        /*
                         * This packet comes from an unknown peer
                         */
                        LOG_S( "received bytes ignored!\r\n");
                    }
                }
            }
            break;
            case UNISOC_PDP_DEACTIVE_SIG:
                LOG_S("UNISOC_PDP_DEACTIVE_SIG");
                dm_close(data.sock);
                data.sock = 0;
                targetP = lwm2mH->serverList;
                while (targetP != NULL)
                {
                    conn = targetP->sessionH;
                    if (conn != NULL)
                    {
                        conn->sock = data.sock;
                    }
                    else
                    {
                        LOG_S( "connection is NULL \r\n");
                    }
                    targetP = targetP->next;
                }
                socket_SetNetId(0);
                break;
            default:
                break;
        }

        lwm2m_free(sig_ptr);
        sig_ptr = NULL;
    }

    /*
     * Finally when the loop is left, we unregister our client from it
     */
    lwm2m_close(lwm2mH);
    lwm2m_sdk_notify_dereg_success(0,"dereg");
    dm_close(data.sock);
    connection_free(data.connList);

    free_security_object(objArray[0]);
    free_server_object(objArray[1]);
    free_object_device(objArray[2]);
    free_object_dm(objArray[3]);
    g_lwm2m_task_id = SCI_INVALID_BLOCK_ID;
    LWM2M_StopTimer();

    LOG_S( "loop func quit\r\n\n");
    return 0;
}
#else
int LWM2M_SDK_RUN_PARAM(int argc, char *argv[])
{
    client_data_t data;
    lwm2m_context_t * lwm2mH = NULL;
    lwm2m_object_t * objArray[OBJ_COUNT];
	char * server = "127.0.0.1";
	char * serverPort = "5683";
    char * localPort = "56830";
	//char * initname = "testlwm2mclient";
	char * name = "";
	int lifetime=300;
    int result=0;
    char * multiVer="v2.0";
    char imeiFlag[65];
    //int opt;

    memset(&data, 0, sizeof(client_data_t));

    data.addressFamily = AF_INET6;
	//localPort = argv[opt];
	//param set by optional	
	if (strlen(g_opt.szSrvIP)>=0) server=g_opt.szSrvIP;
	
	char szsrvport[10]={0};
	if (g_opt.nSrvPort > 80 ) 
	{
		sprintf(szsrvport,"%d",g_opt.nSrvPort);
		serverPort = szsrvport;		
	}
	
	if(g_opt.nAddressFamily==4) data.addressFamily = AF_INET;
	
	if ( g_opt.nLifetime > 10 ) lifetime=g_opt.nLifetime ;

    char szlocalport[10]={0};
	if (g_opt.nLocalPort > 49152 ) 
	{
		sprintf(szlocalport,"%d",g_opt.nLocalPort);
		localPort = szlocalport;		
	}

	
	if(strlen(g_opt.szCMEI_IMEI)>0)
	    strcpy(imeiFlag,g_opt.szCMEI_IMEI);
	else
	    strcpy(imeiFlag,g_opt.szCMEI_IMEI2);
	
	char *szStars="****";
	char szEpname[256]={0};
	sprintf(szEpname,"%s-%s-%s-%s",
	strlen(g_opt.szCMEI_IMEI)>0?g_opt.szCMEI_IMEI:szStars,
	strlen(g_opt.szCMEI_IMEI2)>0?g_opt.szCMEI_IMEI2:szStars,
	strlen(g_opt.szIMSI)>0?g_opt.szIMSI:szStars,	
	strlen(g_opt.szDMv)>0?g_opt.szDMv:szStars
	);

    char szEPIsrc[256]={0};
    sprintf(szEPIsrc,"%s-%s-%s-%s",
	strlen(g_opt.szCMEI_IMEI)>0?g_opt.szCMEI_IMEI:szStars,
	strlen(g_opt.szCMEI_IMEI2)>0?g_opt.szCMEI_IMEI2:szStars,
	strlen(g_opt.szIMSI)>0?g_opt.szIMSI:szStars,
	szStars
	);

	multiVer = strlen(g_opt.szDMv)>0?g_opt.szDMv:szStars;

    char szEpnameCode[256]={0};    
    memset(szEpnameCode,0,sizeof(szEpnameCode));
    memset(g_szEPI,0,sizeof(g_szEPI));
    name=szEpname;


    int nEpnamecodeLen = 0;
    char *encode_out = NULL;
    nEpnamecodeLen = to_encode(szEpname,&encode_out);
    memcpy(szEpnameCode,encode_out,nEpnamecodeLen);
    free(encode_out);
    
	char epnametmp[256];
	memset(epnametmp,0,sizeof(epnametmp));
	sprintf(epnametmp,"L@#@%s@#@%s@#@%s@#@%s",szEpnameCode,g_opt.szAppKey,imeiFlag,multiVer);

    //strcpy(epnametmp,"L@#@M+D2vyO9kk/IPVyzehd6Biypy8R69yTlHdaIQiTp6klbXqJmESZJ6QXoDlQkqOC8@#@M100000006");
	LOG_M("epname=%s~\n", epnametmp);
	name = epnametmp;

    nEpnamecodeLen = to_encode(szEPIsrc,&encode_out);
	sprintf(g_szEPI,"L@#@%s@#@%s@#@%s@#@%s",encode_out,g_opt.szAppKey,imeiFlag,multiVer);
	free(encode_out);
	
    //strcpy(g_szEPI,"L@#@M+D2vyO9kk/IPVyzehd6Bs2GeOZOGcEHYX1mWBROKSg=@#@M100000006");
	
    LOG_M("szEPI=%s~\n", g_szEPI);  

///////////////

    /*
     *This call an internal function that create an IPv6 socket on the port 5683.
     */
    LOG_M( "Trying to bind LWM2M Client to port %s\r\n", localPort);
    data.sock = create_socket(localPort, data.addressFamily);
    if (data.sock < 0)
    {
        LOG_M("Failed to open socket: %d %s\r\n", errno, strerror(errno));
        return -1;
    }
	
	char serverUri[50];
    int serverId = 123;
	sprintf (serverUri, "coap://%s:%s", server, serverPort);

    //objArray[0] = get_security_object();
	objArray[0] = get_security_object(serverId, serverUri);
    if (NULL == objArray[0])
    {
        LOG_S( "Failed to create security object\r\n");
        return -1;
    }
    data.securityObjP = objArray[0];

    objArray[1] = get_server_object(serverId, lifetime);
    if (NULL == objArray[1])
    {
        LOG_S( "Failed to create server object\r\n");
        return -1;
    }

    objArray[2] = get_object_device();
    if (NULL == objArray[2])
    {
        LOG_S("Failed to create Device object\r\n");
        return -1;
    }

    objArray[3] = get_object_dm();
    if (NULL == objArray[2])
    {
        LOG_S("Failed to create Device object\r\n");
        return -1;
    }

    lwm2mH = lwm2m_init(&data);
    if (NULL == lwm2mH)
    {
        LOG_S( "lwm2m_init() failed\r\n");
        return -1;
    }

   
    result = lwm2m_configure(lwm2mH, name, NULL, NULL, OBJ_COUNT, objArray);
    if (result != 0)
    {
        LOG_M( "lwm2m_configure() failed: 0x%X\r\n", result);
        return -1;
    }


    //signal(SIGINT, handle_sigint);
    LOG_M("LWM2M Client \"%s\" started on port %s.\r\nUse Ctrl-C to exit.\r\n\n", name, localPort);
    g_quit = 0;
    
    /*
     * We now enter in a while loop that will handle the communications from the server
     */
    while (0 == g_quit)
    {
        struct timeval tv;
#if 0 //modify by unisoc
        fd_set readfds;
#else
        int timeout = 0;
        sci_fd_set readfds;
#endif
        tv.tv_sec = 60;
        tv.tv_usec = 0;
#if 0 //modify by unisoc
        FD_ZERO(&readfds);
        FD_SET(data.sock, &readfds);
	 FD_SET(g_pip_ios_fd[0],&readfds);
#else        
        SCI_FD_ZERO(&readfds);
        SCI_FD_SET(TO_SCI_SOCKET_ID(data.sock), &readfds);
#endif
        print_state(lwm2mH);
        
        result = lwm2m_step(lwm2mH, &(tv.tv_sec));       
        LOG_M("lwm2m_step result=%d, tv.tv_sec=%d", result, tv.tv_sec);
        if (result != 0)
        {
            LOG_M( "lwm2m_step() failed: 0x%X\r\n", result);
            g_quit = 1;//é€?å‡?
            break;
            //continue;
            return -1;
        }

        /*
         * This part wait for an event on the socket until "tv" timed out (set
         * with the precedent function)
         */
#if 0 //modify by unisoc
        result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);
#else
        timeout = tv.tv_sec * 10;
        result = sci_sock_select(&readfds, NULL, NULL, timeout);
        LOG_M("sci_sock_select ret=%d, timeout=%d", result, timeout);
#endif
        if (result < 0)
        {
            if (errno != EINTR)
            {
               LOG_M("Error in select(): %d %s\r\n", errno, strerror(errno));
            }
        }
        else if (result > 0)
        {
            uint8_t buffer[MAX_PACKET_SIZE];
            int numBytes;

            /*
             * If an event happens on the socket
             */
#if 0 //modify by unisoc
            if (FD_ISSET(data.sock, &readfds))
#else
            if (SCI_FD_ISSET(TO_SCI_SOCKET_ID(data.sock), &readfds))
#endif
            {
                struct sockaddr_storage addr;
                socklen_t addrLen;

                addrLen = sizeof(addr);
                #if 1 //modify by unisoc
                memset(&addr, 0, sizeof(addr));
                #endif
                /*
                 * We retrieve the data received
                 */
                numBytes = recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);
                LOG_M("Data recv numBytes=%d", numBytes);

                if (0 > numBytes)
                {
                    LOG_M("Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
                }
                else if (0 < numBytes)
                {
                    connection_t * connP;

                    connP = connection_find(data.connList, &addr, addrLen);
                    if (connP != NULL)
                    {
                        /*
                         * Let liblwm2m respond to the query depending on the context
                         */
                        lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
                    }
                    else
                    {
                        /*
                         * This packet comes from an unknown peer
                         */
                        LOG_S( "received bytes ignored!\r\n");
                    }
                }
            }
#if 0 //modify by unisoc
			else if(FD_ISSET(g_pip_ios_fd[0], &readfds))
			{
				char c[2];  
				read(g_pip_ios_fd[0], c, 1);
				g_quit = 1;
			}
#endif
        }
    }

    /*
     * Finally when the loop is left, we unregister our client from it
     */
    lwm2m_close(lwm2mH);
    lwm2m_sdk_notify_dereg_success(0,"dereg");
    close(data.sock);
    connection_free(data.connList);

    free_security_object(objArray[0]);
    free_server_object(objArray[1]);
    free_object_device(objArray[2]);
    free_object_dm(objArray[3]);

    LOG_S( "loop func quit\r\n\n");

    return 0;
}
#endif
void * runthreadfunc(void * arg)
{
	int ret = LWM2M_SDK_RUN_PARAM(0,NULL);	
	lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,"LWM2M_SDK_RUN_PARAM");
	LOG_S("runthreadfunc end");
	s_thrd_flag = 0;
	return 0;
}

int LWM2M_SDK_RUN(int nMode)
{
	//LWM2M_SDK_RUN(0,NULL);
	int ret = 0;
	if(g_quit==0)
	{
	    LOG_S("has runned");
	    return 0;
	}
	
	if(nMode==0)
	{
		ret = LWM2M_SDK_RUN_PARAM(0,NULL);
		lwm2m_sdk_notify(NOTIFY_TYPE_SYS,NOTIFY_CODE_UNKNOWEN,ret,"LWM2M_SDK_RUN_PARAM");
		s_thrd_flag = 0;
	}
	else
	{
		if(s_thrd_flag==0)
		{
            pthread_attr_t attr;

            dm_pthread_attr_init(&attr );
            attr.stack_size = 32  * 1024;
            attr.sched_priority = LWM2M_THREAD_PRIORITY;
            dm_pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			if (0 != dm_pthread_create(&g_lwsdktid, &attr, runthreadfunc,NULL))
			{
				LOG_S("msg=failed to start up thread.|");
				s_thrd_flag = 0;
				
				ret = -1;
				return -1;
			}
			s_thrd_flag = 1;
			
		}
		else
		{
			LOG_S("msg=hss started up thread.|");
		}
	}

	//pthread_detach(g_lwsdktid); 	
	return ret;
	
}

int  LWM2M_SDK_INIT(Options * options,OptFuncs *optfuncs)
{	
	int ret = 0;
	int nisbig;
	strcpy(g_opt.szCMEI_IMEI,options->szCMEI_IMEI);
	strcpy(g_opt.szCMEI_IMEI2,options->szCMEI_IMEI2);
	strcpy(g_opt.szIMSI,options->szIMSI);	
	strcpy(g_opt.szDMv,options->szDMv);
	strcpy(g_opt.szAppKey,options->szAppKey);
	strcpy(g_opt.szPwd,options->szPwd);
	
	strcpy(g_opt.szSrvIP,options->szSrvIP);
	g_opt.nAddressFamily = options->nAddressFamily;
	g_opt.nSrvPort = options->nSrvPort;
	g_opt.nLifetime = options->nLifetime;
	g_opt.nBootstrap = options->nBootstrap;	
	g_opt.nLocalPort = options->nLocalPort;
#if 1 //modify by unisoc
    g_opt.bUseSDKRegUpdate = options->bUseSDKRegUpdate;
#endif
    g_sdk_opt_funcs = *optfuncs;
//other valid check
    if(strcmp(g_opt.szSrvIP,"shipei.fxltsbl.com")==0)
    {
        if(g_opt.nLifetime<120) g_opt.nLifetime=120;
    }
    if(strcmp(g_opt.szSrvIP,"m.fxltsbl.com")==0)
    {
        if(g_opt.nLifetime<86400) g_opt.nLifetime=86400; 
    }
    
#if 0 //modify by unisoc
	ret = pipe(g_pip_ios_fd);
	signal(SIGPIPE,SIG_IGN);
#endif

	g_quit = -1;
	s_thrd_flag = 0;

	//WRITE_LOG_START;

	nisbig = isbigendian();
	LOG_M("isbigendian=%d",nisbig);
	
	
	LOG_S("init sucess");
	return ret;
	
}



int LWM2M_SDK_STOP()
{	
    int ret = 0;
    LOG_S("to stop");

    if(g_quit != 0)
    {
        LOG_S("is stopped");
        return 0;
    }

#if 0 //modify by unisoc
	ret = write(g_pip_ios_fd[1], "x", 1);
#else
    LWM2M_StopTimer();
    LWM2M_SDK_SEND_MSG(UNISOC_LWM2MSDK_EXIT_SIG);
#endif

#if 0 //modify by unisoc
    //wait for thread quit	
    if(s_thrd_flag==1)
    {	
        pthread_join(g_lwsdktid,NULL);
    }
#endif
    s_thrd_flag = 0;

    //WRITE_LOG_FLUSH;
    return ret;
	
}

int LWM2M_SDK_FINI()
{	
#if 0 //modify by unisoc
	g_quit = -1;
	s_thrd_flag = 0;
	close(g_pip_ios_fd[0]);
	close(g_pip_ios_fd[1]);
#endif
    LWM2M_StopTimer();
    LWM2M_SDK_SEND_MSG(UNISOC_PDP_DEACTIVE_SIG);
	LOG_S("end");
	//WRITE_LOG_EXIT;
	return 0;
	
}
#if 1 //modify by unisoc
int LWM2M_SDK_IS_RUN(void)
{
    LOG_M("sdk is run:%d", g_quit == 0);
    return g_quit == 0;
}

LOCAL void LWM2M_SDK_SEND_MSG(uint32 MSG)
{
    UNISOC_LWM2MSDK_SIG_T    *send_sig = PNULL;
    
    send_sig =  lwm2m_malloc(sizeof(UNISOC_LWM2MSDK_SIG_T));
    SCI_MEMSET(send_sig, 0x00,sizeof(UNISOC_LWM2MSDK_SIG_T));
    
    //make signal header
    send_sig->Sender = SCI_IdentifyThread();
    send_sig->SignalCode = MSG;
    send_sig->SignalSize = sizeof(UNISOC_LWM2MSDK_SIG_T);
    LOG_M("[lwm2m send message_id %d]\n",MSG);
    SCI_SendSignal((xSignalHeader)(send_sig), g_lwm2m_task_id);
}

void LWM2M_SDK_UPDATE_REG(void)
{
    //PDP is already active , create socket 
    LOG_S("Send UNISOC_LWM2MSDK_UPDATE_SOCKET_SIG");
    LWM2M_SDK_SEND_MSG(UNISOC_LWM2MSDK_UPDATE_SOCKET_SIG);

    LOG_S("Send UNISOC_LWM2MSDK_REGISTRATION_UPDATE_SIG");
    LWM2M_SDK_SEND_MSG(UNISOC_LWM2MSDK_REGISTRATION_UPDATE_SIG);
}

void LWM2M_SDK_UPDATE_SOCKET(void)
{
    LOG_S("Send UNISOC_LWM2MSDK_UPDATE_SOCKET_SIG");
    LWM2M_SDK_SEND_MSG(UNISOC_LWM2MSDK_UPDATE_SOCKET_SIG);
}

LOCAL void LWM2M_CreateTimer(int msec)
{
    LOG_M("msec=%d", msec);
    LWM2M_StopTimer();
    g_lwm2m_timer = SCI_CreateTimer("LwM2M_Timer", 
    LWM2M_SDK_SEND_MSG, UNISOC_LWM2MSDK_TIMER_SIG, msec, SCI_AUTO_ACTIVATE);      
    
    return;
}

LOCAL void LWM2M_StopTimer()
{
    if (g_lwm2m_timer == PNULL)
    {
        return;
    }

    if (SCI_IsTimerActive(g_lwm2m_timer))
    {
        SCI_DeactiveTimer(g_lwm2m_timer);
    }
    SCI_DeleteTimer(g_lwm2m_timer);
    g_lwm2m_timer = PNULL;

    return;
}
#endif
