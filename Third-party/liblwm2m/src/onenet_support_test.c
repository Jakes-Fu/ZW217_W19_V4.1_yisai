/******************************************************************************
 ** File Name:      atc_daps_api.c                                           *
 ** Author:                                                                   *
 ** DATE:                                                                     *
 ** Copyright:                                                                *
 ** Description:    This file contains the functions that handle the api
 **                 used to sopport onenet platform  like  LWM2M_XXX()  and provide test examples  like 
                    LWM2M_TEST_XXX()
 ******************************************************************************/

#include "sci_types.h"
#include "sci_api.h"




#ifdef LIBLWM2M_SUPPORT
#include "lwm2m_api.h"
#endif

#if defined LIBLWM2M_SUPPORT
extern  Mipl_Ctx_t gMiplCtx;
uint32 lwm2m_netid = 0;
#define NBSYS_IMEI_MAXLENGTH (16)
#define NBSYS_IMSI_MAXLENGTH (16)
int observeid = 0;
int objid = 0;
int instanceid = 0;
int resid = 0;


#define ONENET_EVENT_BASE 0x00
#define ONENET_EVENT_UNREG_DONE (ONENET_EVENT_BASE + 15)
#define ONENET_EVENT_RESPONSE_FAILED (ONENET_EVENT_BASE + 20)
#define ONENET_EVENT_RESPONSE_SUCCESS (ONENET_EVENT_BASE + 21)
#define ONENET_EVENT_NOTIFY_FAILED (ONENET_EVENT_BASE + 25)
#define ONENET_EVENT_NOTIFY_SUCCESS (ONENET_EVENT_BASE + 26)

#define URI_INVALID ((uint16_t)0xFFFF)
#define URI_MAX_ID ((uint16_t)0xFFFF - 1)



#define MIPL_LIFETIME_LIMIT_MIN ((uint32_t)0x0000000F)
#define MIPL_LIFETIME_LIMIT_MAX ((uint32_t)0x0FFFFFFF)



Mipl_Ctx_t gMiplCtx;
//static mipl_cfg_context_t *gMiplCfgCtx = NULL;
lwm2m_config_t *lwm2m_configs[MAX_REF_NUM];
int8_t gMiplConnectRef = -1;
static bool gMiplIsClosing = false;
 extern BLOCK_ID g_liblwm2m_MESSAGE_TASK;
 
int g_test_observersp_enable = FALSE;
int g_test_discoverrsp_enable = FALSE;
int g_test_readrsp_enable = FALSE;
int g_test_writersp_enable = FALSE;
int g_test_notify_enable =FALSE;
int g_test_execute_enable =FALSE;
static int mipl_makeDeviceName(char *name,char * imei,char * imsi)
{
    if (imei != NULL && imsi != NULL)
    {
    memset(name, 0, NBSYS_IMEI_MAXLENGTH + NBSYS_IMSI_MAXLENGTH + 1);
    //uint8_t imei = mipl_getIMEI(name, NBSYS_IMEI_MAXLENGTH);
    
    memcpy(name,imei,NBSYS_IMEI_MAXLENGTH );
    
    //*((char *)(name + NBSYS_IMEI_MAXLENGTH)) = ';';
    //memcpy(name+NBSYS_IMEI_MAXLENGTH, ';',strlen( ';'));
    strcat(name,";");
    
    //uint8_t imsi = mipl_getIMSI(name + imei + 1, NBSYS_IMSI_MAXLENGTH);
    //memcpy(name+NBSYS_IMEI_MAXLENGTH+1,imsi,NBSYS_IMSI_MAXLENGTH);
    strcat(name,imsi);
    }
    if (strlen((char *)name) <= 0 )
    {
        SCI_TRACE_LOW(" LWM2MDEBUG  MIPL# ERROR:SET IMEI/IMSI ERROR");
        return 0;
    }
    SCI_TRACE_LOW(" LWM2MDEBUG  endpoint name  is : %s   strlen(name)  :  %d [%s  %d]\n",name,strlen((char *)name),__FUNCTION__,__LINE__);
    return strlen(name);
}


void mipl_server_ind_process_entry(uint32 argc, void* argv)
{
    int i = 0;
    
    xSignalHeaderRec *sig_ptr = NULL;
    lwm2mEventSIG_t* pEvent = NULL;
    uint8_t ref;
    lwm2mEvent_t *lwm2m_ev;
    uint32_t result;
    uint32_t result_param1;
    uint32_t result_param2;
    
    
    SCI_TRACE_LOW("[ LWM2MDEBUG]  %s  %d\n",__FUNCTION__,__LINE__);
    g_liblwm2m_MESSAGE_TASK = SCI_IdentifyThread();
    while (1)
    {
        SCI_TRACE_LOW("[ LWM2MDEBUG]   wait signal  [%s ,%d]\n",__FUNCTION__,__LINE__);
        sig_ptr = SCI_GetSignal(g_liblwm2m_MESSAGE_TASK);
        if (NULL != sig_ptr)
        {
            pEvent = (lwm2mEventSIG_t*) sig_ptr;            
        }
    
    
        //atCmdEngine_t *engine = (atCmdEngine_t *)pEvent->param1;
        ref = (uint8_t)pEvent->param2;
        lwm2m_ev = (lwm2mEvent_t *)pEvent->param3;

         result = lwm2m_ev->param1;
        result_param1 = lwm2m_ev->param2;
        result_param2 = lwm2m_ev->param3;

        SCI_TRACE_LOW(" LWM2MDEBUG MIPL# mipl_server_ind_process nEventId=%d", pEvent->id);
        SCI_TRACE_LOW("  LWM2MDEBUG MIPL# mipl_server_ind_process result=%d", result);
        SCI_TRACE_LOW("  LWM2MDEBUG MIPL# mipl_server_ind_process result_param1=%d", result_param1);
        SCI_TRACE_LOW("  LWM2MDEBUG MIPL# mipl_server_ind_process result_param2=%d", result_param2);

        //if (engine == NULL)
        //    return;

        switch (pEvent->id)
        {
            case EVETN_IND:
            {
                char refstr[50] = {0};

                if (result == LWM2M_EVENT_DEREG_SUCCESS)
                    result = ONENET_EVENT_UNREG_DONE;

                //sprintf(refstr, "+MIPLEVENT:%d,%ld", ref, result);
                SCI_TRACE_LOW( "+MIPLEVENT:%d,%ld [ %s  %d] \n", ref, result ,__FUNCTION__,__LINE__);
                //atCmdRespUrcText(engine, refstr);  
                
            }
            break;
            case SERVER_QUIT_IND:
            {
                if (gMiplIsClosing == true)
                {
                    gMiplIsClosing = false;
                    //atCmdRespOK(engine);
                }
                gMiplCtx.isRegister = MIPL_NOT_REGISTER;
            }
            break;
            case OBJ_OBSERVE_IND:
            {
                uint32_t msgid = result;
                uint16_t objid = result_param1 >> 16;
                int16_t insid = result_param1;
                int16_t resid = result_param2 >> 16;
                uint8_t isAddObserve = result_param2;
                char refstr[100] = {0};
                if (insid == -1)
                    SCI_TRACE_LOW( "  LWM2MDEBUG +MIPLOBSERVE:%d,%lu,%d,%d,-1,-1 [ %s  %d] \n", ref, msgid, isAddObserve, objid,__FUNCTION__,__LINE__);
                else
                    {
                        
                        SCI_TRACE_LOW( "  LWM2MDEBUG +MIPLOBSERVE:%d,%lu,%d,%d,%d,%d  [ %s  %d] \n", ref, msgid, isAddObserve, objid, insid, resid,__FUNCTION__,__LINE__);
                        if(g_test_observersp_enable ==TRUE)   //just for test 
                        {
                            observeid = msgid;
                            SCI_TRACE_LOW(" LWM2MDEBUG THEN SEND RSP   observeid :%d ",observeid);
                            if(LWM2M_MIPLOBSERVERSP(ref,msgid,1) != -1)
                            {
                                SCI_TRACE_LOW( "OBSERVERSP SEND OK  [%s  %d]\n",__FUNCTION__,__LINE__);
                            }
                            else
                            {
                                SCI_TRACE_LOW( "OBSERVERSP SEND error  [%s  %d]\n",__FUNCTION__,__LINE__);
                            }
                        }
                    }

                //atCmdRespUrcText(engine, refstr);
                
            }
            break;
            case OBJ_DISCOVER_IND:
            {
                uint32_t msgid = result;
                uint16_t objid = result_param1;
                char refstr[100] = {0};
                //sprintf(refstr, "+MIPLDISCOVER:%d,%lu,%d", ref, msgid, objid);
                SCI_TRACE_LOW( "+MIPLDISCOVER:%d,%lu,%d", ref, msgid, objid);

                if(g_test_discoverrsp_enable == TRUE)
                {                
                    if(LWM2M_MIPLDISCOVERRSP(ref,msgid,1,34,"5706;5805;5602;5603;5604;5701;5605")!=-1)
                    {
                        SCI_TRACE_LOW( " LWM2MDEBUG DISCOVERRSP SEND OK  [%s  %d]\n",__FUNCTION__,__LINE__);
                    }
                    else
                    {
                        SCI_TRACE_LOW( "LWM2MDEBUG DISCOVERRSP SEND error  [%s  %d]\n",__FUNCTION__,__LINE__);
                    }
                        
                }
                //atCmdRespUrcText(engine, refstr);
            }
            break;
            case OBJ_READ_IND:
            {
                uint32_t msgid = result;
                read_param_t *readparam = (read_param_t *)result_param1;
                char refstr[100] = {0};
                SCI_TRACE_LOW( "+MIPLREAD:%d,%ld,%d,%d,%d", ref, msgid, readparam->objid, (readparam->insid == 0xFFFF ? -1 : readparam->insid),
                        (readparam->resid == 0xFFFF ? -1 : readparam->resid));
                if(g_test_readrsp_enable ==TRUE)
                {
                    char  value[20] = "yxmtestrsp";  //string
                    //int value = 12;  //int
                    SCI_TRACE_LOW( "LWM2MDEBUG   [%s  %d]\n",__FUNCTION__,__LINE__);
                    if(LWM2M_MIPLREADRSP(ref,msgid,1,readparam->objid,(readparam->insid == 0xFFFF ? -1 : readparam->insid),(readparam->resid == 0xFFFF ? -1 : readparam->resid),1,10,value,0,0)!=-1)
                    {
                        SCI_TRACE_LOW( " LWM2MDEBUG READRSP SEND OK  [%s  %d]\n",__FUNCTION__,__LINE__);
                    }
                    else
                    {
                        SCI_TRACE_LOW( "LWM2MDEBUG READRSP SEND error  [%s  %d]\n",__FUNCTION__,__LINE__);
                    }
                }
                SCI_Sleep(10000);
                if(g_test_notify_enable ==TRUE)
                {
                    char  value[20] = "yxmtestNOTIFY";  //string
                    //int value = 12;  //int
                    SCI_TRACE_LOW( "LWM2MDEBUG  observeid :%d , [%s  %d]\n",observeid,__FUNCTION__,__LINE__);

                    objid = readparam->objid;
                    instanceid = (readparam->insid == 0xFFFF ? -1 : readparam->insid);
                    resid = (readparam->resid == 0xFFFF ? -1 : readparam->resid);
                    
                    if(LWM2M_MIPLNOTIFY(ref,observeid,readparam->objid,(readparam->insid == 0xFFFF ? -1 : readparam->insid),(readparam->resid == 0xFFFF ? -1 : readparam->resid),1,13,value,0,0)!=-1)
                    {
                        SCI_TRACE_LOW( " LWM2MDEBUG NOTIFY SEND OK  [%s  %d]\n",__FUNCTION__,__LINE__);
                    }
                    else
                    {
                        SCI_TRACE_LOW( "LWM2MDEBUG NOTIFY SEND error  [%s  %d]\n",__FUNCTION__,__LINE__);
                    }
                }

                
                //atCmdRespUrcText(engine, refstr);
                
                free(readparam);
            }
            break;
            case OBJ_WRITE_IND:
            {
                char *refstr, *fmtstr;
                uint32_t msgid = result;
                write_param_t *writeparam = (write_param_t *)result_param1;
                uint16_t count = (uint16_t)result_param2;
                
                while (writeparam != NULL && count-- > 0)
                {
                    write_param_t *delNode;
                    refstr = (char *)malloc(writeparam->len * 2 + 100);
                    if (refstr != NULL)
                    {
                        int i ;
                        int size;
                        memset(refstr, 0, writeparam->len * 2 + 100);
                        if (writeparam->type == lwm2m_opaque)
                        {
                            fmtstr = (char *)malloc(writeparam->len * 2 + 2);
                            if (fmtstr != NULL)
                            {
                                memset(fmtstr, 0, writeparam->len * 2 + 2);
                                for (i = 0; i < writeparam->len; i++)
                                    sprintf(fmtstr + 2 * i, "%02X", writeparam->data[i]);
                                SCI_TRACE_LOW("+MIPLWRITE:%d,%ld,%d,%d,%d,%d,%d,%s,0,%d", ref, msgid,
                                        writeparam->objid, writeparam->insid, writeparam->resid, writeparam->type,
                                        writeparam->len, fmtstr, count);
                                if(g_test_writersp_enable ==TRUE)
                                {
                                    if(LWM2M_MIPLWRITERSP(ref,msgid,2)!=-1)
                                    {
                                        SCI_TRACE_LOW( " LWM2MDEBUG WRITERSP SEND OK  [%s  %d]\n",__FUNCTION__,__LINE__);
                                    }
                                    else
                                    {
                                        SCI_TRACE_LOW( "LWM2MDEBUG WRITERSP SEND error  [%s  %d]\n",__FUNCTION__,__LINE__);
                                    }
                                }
                                free(fmtstr);
                            }
                        }
                        else
                        {
                            SCI_TRACE_LOW( "+MIPLWRITE:%d,%ld,%d,%d,%d,%d,%d,\"%s\",0,%d", ref, msgid,
                                    writeparam->objid, writeparam->insid, writeparam->resid, writeparam->type,
                                    writeparam->len, writeparam->data, count);
                            if(g_test_writersp_enable ==TRUE)
                                {
                                    if(LWM2M_MIPLWRITERSP(ref,msgid,2)!=-1)
                                    {
                                        SCI_TRACE_LOW( " LWM2MDEBUG WRITERSP SEND OK  [%s  %d]\n",__FUNCTION__,__LINE__);
                                    }
                                    else
                                    {
                                        SCI_TRACE_LOW( "LWM2MDEBUG WRITERSP SEND error  [%s  %d]\n",__FUNCTION__,__LINE__);
                                    }
                                }
                            
                        }
                        i = 0;
                        size = 800;
                        for (; i < (strlen(refstr) / size); i++)
                        {
                            //atCmdRespInfoNTextBegin(engine, refstr + (i * size), size);
                        }
                        //atCmdRespInfoNTextEnd(engine, refstr + (i * size), strlen(refstr + (i * size)));
                        free(refstr);
                    }

                    delNode = writeparam;
                    writeparam = writeparam->next;
                    free(delNode->data);
                    free(delNode);
                }
            }
            break;
            case OBJ_EXE_IND:
            {
                char *refstr = NULL;
                uint32_t msgid = result;
                exec_param_t *execparam = (exec_param_t *)result_param1;
                refstr = (char *)malloc(execparam->length + 100);
                if (refstr != NULL)
                {
                    SCI_TRACE_LOW( "+MIPLEXECUTE:%d,%ld,%d,%d,%d,%d,\"%s\"", ref, msgid, execparam->objid,
                            execparam->insid, execparam->resid, execparam->length, (char *)(execparam->buffer));
                    //atCmdRespUrcText(engine, refstr);

                    if(g_test_execute_enable == TRUE)
                    {
                        if(LWM2M_MIPLEXECUTERSP(ref,msgid,2)!=-1)
                        {
                            SCI_TRACE_LOW( " LWM2MDEBUG WRITERSP SEND OK  [%s  %d]\n",__FUNCTION__,__LINE__);
                        }
                        else
                        {
                            SCI_TRACE_LOW( "LWM2MDEBUG WRITERSP SEND error  [%s  %d]\n",__FUNCTION__,__LINE__);
                        }
                    }
                    free(refstr);
                }

                free(execparam->buffer);
                free(execparam);
            }
            break;
            case OBJ_SETPARAM_IND:
            {
                char *refstr = NULL;
                uint32_t msgid = result;
                setParam_t *setParam = (setParam_t *)result_param1;
                refstr = (char *)malloc(512);
                if (refstr != NULL)
                {
                    SCI_TRACE_LOW( "+MIPLPARAMETER:%d,%ld,%d,%d,%d,%d,%s", ref, msgid, setParam->objid, setParam->insid, setParam->resid,
                            strlen((void *)(setParam->param)), setParam->param);
                    //atCmdRespUrcText(engine, refstr);
                    free(refstr);
                }

                free(setParam->param);
                free(setParam);
            }
            break;
            case SERVER_REGCMD_RESULT_IND:
            {
                char refstr[50] = {0};

                if (result)
                {
                    memset(&gMiplCtx, 0, sizeof(Mipl_Ctx_t));
                    gMiplCtx.isRegister = MIPL_HAD_REGISTERED;
                    SCI_TRACE_LOW( "+MIPLEVENT:%d,%d", ref, LWM2M_EVENT_UPDATE_SUCCESS);
                    //atCmdRespUrcText(engine, refstr);
                }
            }
            break;
            case OBJ_OPERATE_RSP_IND:
            {
                char refstr[100] = {0};
                if (result == 0)
                {
                    //atCmdRespOK(engine);
                    SCI_TRACE_LOW( "+MIPLEVENT:%d,%d", ref, ONENET_EVENT_RESPONSE_SUCCESS);
                }
                else
                {
                    //atCmdRespCmeError(engine, ERR_AT_CME_EXE_FAIL);
                    SCI_TRACE_LOW( "+MIPLEVENT:%d,%d", ref, ONENET_EVENT_RESPONSE_FAILED);
                }

                //atCmdRespUrcText(engine, refstr);
            }
            break;
            case DATA_SEND_IND:
            {
                char refstr[100] = {0};
                if (result == LWM2M_EVENT_NOTIFY_SEND_SUCCESS)
                {
                    //atCmdRespOK(engine);
                    SCI_TRACE_LOW( "+MIPLEVENT:%d,%d", ref, ONENET_EVENT_NOTIFY_SUCCESS);
                }
                else
                {
                    //atCmdRespCmeError(engine, ERR_AT_CME_EXE_FAIL);
                    SCI_TRACE_LOW( "+MIPLEVENT:%d,%d", ref, ONENET_EVENT_NOTIFY_FAILED);
                }

                //atCmdRespUrcText(engine, refstr);
            }
            break;
#ifdef CONFIG_AT_LWM2M_MIPL_SOTA_SUPPORT
            case SOTA_RSP_IND:
            {
                if (result)
                    //atCmdRespOK(engine);
                else
                    //atCmdRespCmeError(engine, ERR_AT_CME_EXE_FAIL);
            }
            break;
#endif
            case MESSAGE_TASK_EXIT_IND:
                if(lwm2m_ev !=NULL)
                {
                    free(lwm2m_ev);
                    lwm2m_ev = NULL;
                }
                if(pEvent !=NULL)
                {
                    free(pEvent);                    
                }    
                SCI_TRACE_LOW(" LWM2MDEBUG     EXIT  MESSAGE TASK!!! [%s  %d]\n",__FUNCTION__,__LINE__);
                g_liblwm2m_MESSAGE_TASK = SCI_INVALID_BLOCK_ID ;
                lwm2m_Thread_stop();
                break;                
            default:
            {
                char refstr[50] = {0};
                SCI_TRACE_LOW( "+UNPROCESS EVENT:%ld", pEvent->id);
                //atCmdRespUrcText(engine, refstr);
            }
            break;
        }

        free(lwm2m_ev);
        lwm2m_ev = NULL;
        free(pEvent);
    }
}

int  LWM2M_MIPLCREATE(char *host,char *port,char * imei,char * imsi,char * pskId,char * psk,char * authcode,BOOLEAN bs_enable)
{
    char cmdline[250] = {0};
    //char *host = NULL;
    //char *port = NULL;
    char endpointName[NBSYS_IMEI_MAXLENGTH + NBSYS_IMSI_MAXLENGTH + 2] = {0};

    if(mipl_makeDeviceName(endpointName,imei,imsi) <=0  )
    {
        SCI_TRACE_LOW(" LWM2MDEBUG   make endpoint name  error[%s  %d]\n",__FUNCTION__,__LINE__);
        return -1;
    }

    sprintf(cmdline, "-e %d -n %s -h %s -p %s -4 -t 60 ", LWM2M_SERVER_ONENET, endpointName, host, port);

    if( pskId !=NULL &&psk !=NULL )
    {
        char  temp[100]  ={0};
        sprintf(temp," -i %s  -s %s ",pskId ,psk);
        strcat(cmdline, temp);
    }

     if( authcode !=NULL )
    {
        char  temp[20]  ={0};
        sprintf(temp," -A %s  ",authcode );
        strcat(cmdline, temp);
    }
    
     if (bs_enable == TRUE)
        strcat(cmdline, " -b");
     
    SCI_TRACE_LOW( " LWM2MDEBUG  MIPL# cmdline:%s [%s  %d]\n", cmdline,__FUNCTION__,__LINE__);
    gMiplConnectRef = lwm2m_new_config((uint8_t *)cmdline);
    if (gMiplConnectRef >= 0)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  MIPL# gMiplConnectRef:%d [%s  %d]\n",gMiplConnectRef,__FUNCTION__,__LINE__);   
        return gMiplConnectRef;
    }
    else
    {
        SCI_TRACE_LOW( "MIPL# lwm2m new config failed  ");
        return -1;
    }    
     
}

int LWM2M_MIPLDELETE (int ref)
{

    if(gMiplConnectRef !=ref )
    {
        SCI_TRACE_LOW( " LWM2MDEBUG gMiplConnectRef !=ref    ");
        return -1;
    }
     if ( gMiplConnectRef != -1)
    {
        //free(gMiplCfgCtx);
        //gMiplCfgCtx = NULL;
        lwm2m_free_config(gMiplConnectRef);
        gMiplConnectRef = -1;
        SCI_TRACE_LOW( " LWM2MDEBUG DELETE OK  gMiplConnectRef  ");
        return 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG DELETE error  gMiplConnectRef  ");
         return -1;
    }
}

int LWM2M_MIPLOPEN(int ref,uint32_t lifetime,uint32_t timeout)
{
    SCI_TRACE_LOW( " LWM2MDEBUG ref :%d  lifetime : %d   timeout: %d:[%s  %d]\n",ref,lifetime,timeout,__FUNCTION__,__LINE__);   
    if (gMiplConnectRef != ref)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  MIPL# ref:%d [%s  %d]\n",ref,__FUNCTION__,__LINE__);           
        return -1;
    }
    
    if (liblwm2m_MessageHandleTask((osiCallback_t)mipl_server_ind_process_entry) !=0)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG MessageHandleTask error:[%s  %d]\n",__FUNCTION__,__LINE__);     
        return -1;
    }

    if (lwm2m_register(gMiplConnectRef, lifetime, timeout) == LWM2M_RET_OK)
    {
        SCI_TRACE_LOW( "LWM2MDEBUG  lwm2m register success");            
    }
    else
    {
        SCI_TRACE_LOW( "LWM2MDEBUG lwm2m register failed");
        return -1;
    }
    return 0; 

}

int LWM2MW_MIPLCLOSE(int ref)
{   
int ret;
    if (gMiplConnectRef != ref)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG  MIPL# ref:%d [%s  %d]\n",ref,__FUNCTION__,__LINE__);           
            return -1;            
        }

        if ( gMiplConnectRef == -1)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG  err gMiplConnectRef:%d [%s  %d]\n",gMiplConnectRef,__FUNCTION__,__LINE__);           
            return -1;    
        }

        if (lwm2m_unregister(gMiplConnectRef) == LWM2M_RET_OK)
        {
            SCI_TRACE_LOW(  " LWM2MDEBUG  lwm2m unregister success");          
            ret =0;
        }
        else
        {
            SCI_TRACE_LOW(  " LWM2MDEBUG  lwm2m unregister failed");
            ret = -1;  
        }
        g_test_observersp_enable = FALSE;
        g_test_discoverrsp_enable = FALSE;
        g_test_readrsp_enable =FALSE;
        g_test_writersp_enable = FALSE;
         g_test_notify_enable = FALSE;
         g_test_execute_enable=FALSE;
        
        return ret;

}

int LWM2M_MIPLADD_OBJECT(int ref,uint16_t objid,uint32_t instCounts,const char *instBitmap,uint8_t attcount,uint8_t actcount)
{
    int i;
    int ret;
    for (i = 0; i < instCounts; i++)
    {
        if (instBitmap[i] != '1' && instBitmap[i] != '0')
        {
            SCI_TRACE_LOW( "MIPL# invalid instBitmap  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        }
    }
     if ( gMiplConnectRef == -1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  err gMiplConnectRef:%d [%s  %d]\n",gMiplConnectRef,__FUNCTION__,__LINE__);           
        return -1;    
    }

     ret = lwm2m_add_obj(objid, instCounts, (uint8_t *)instBitmap, (attcount + actcount), gMiplConnectRef);
    if (ret == LWM2M_RET_OK)
    {
        
        SCI_TRACE_LOW( "MIPL# lwm2m add obj success  [%s  %d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else if (ret == LWM2M_RET_ERROR)
    {
        SCI_TRACE_LOW( "MIPL# lwm2m add obj failed  [%s  %d]\n",__FUNCTION__,__LINE__);
        return -1;
    } 

}

int LWM2M_MIPLDEL_OBJECT(int ref,uint16_t objid)
{
    int ret;
    if ( gMiplConnectRef != ref)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  err gMiplConnectRef:%d [%s  %d]\n",gMiplConnectRef,__FUNCTION__,__LINE__);           
        return -1;    
    }

    ret = lwm2m_del_obj(objid, ref);
    if (ret == LWM2M_RET_OK)
    {        
        SCI_TRACE_LOW( "MIPL# lwm2m del obj success  [%s  %d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else if (ret == LWM2M_RET_ERROR)
    {
        SCI_TRACE_LOW( "MIPL# lwm2m del obj failed  [%s  %d]\n",__FUNCTION__,__LINE__);
        return -1;
    } 
}

int LWM2M_MIPLOBSERVERSP(int ref ,uint32_t msgid ,uint8_t result )
{
    uint8_t OnenetRet;
    int ret;
    
    if (result == 1)
            OnenetRet = ONENET_RESPONSE_OBSERVE;
        else if (result == 11)
            OnenetRet = ONENET_RESPONSE_BAD_REQUEST;
        else if (result == 12)
            OnenetRet = ONENET_RESPONSE_UNAUTHORIZED;
        else if (result == 13)
            OnenetRet = ONENET_RESPONSE_NOT_FOUND;
        else if (result == 14)
            OnenetRet = ONENET_RESPONSE_METHOD_NOT_ALLOWED;
        else if (result == 15)
            OnenetRet = ONENET_RESPONSE_NOT_ACCEPTABLE;
        else
        {
            SCI_TRACE_LOW( "MIPL# lwm2m result  failed  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        }

        ret = lwm2m_observe_rsp_onenet(msgid, OnenetRet, false, ref);
         if (ret == LWM2M_RET_OK)
        {        
            SCI_TRACE_LOW( "MIPL# lwm2m observe_rsp success  [%s  %d]\n",__FUNCTION__,__LINE__);
            return 0;
        }
        else if (ret == LWM2M_RET_ERROR)
        {
            SCI_TRACE_LOW( "MIPL# lwm2m observe_rsp failed  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        } 

}


int LWM2M_MIPLDISCOVERRSP(int ref,uint32_t msgid,uint8_t result,uint32_t length,char *valuestr)
{
    int ret;
    uint8_t OnenetRet;
    
    if (result == 1)
            OnenetRet = ONENET_RESPONSE_DISCOVER;
        else if (result == 11)
            OnenetRet = ONENET_RESPONSE_BAD_REQUEST;
        else if (result == 12)
            OnenetRet = ONENET_RESPONSE_UNAUTHORIZED;
        else if (result == 13)
            OnenetRet = ONENET_RESPONSE_NOT_FOUND;
        else if (result == 14)
            OnenetRet = ONENET_RESPONSE_METHOD_NOT_ALLOWED;
        else if (result == 15)
            OnenetRet = ONENET_RESPONSE_NOT_ACCEPTABLE;
        else
        {
            SCI_TRACE_LOW( "MIPL# lwm2m result  failed  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        }
    SCI_TRACE_LOW( " LWM2MDEBUG DISCOVERRSP   [%s  %d]\n",__FUNCTION__,__LINE__);
    ret = lwm2m_discover_rsp_onenet(msgid, OnenetRet, false, (const uint8_t *)valuestr, ref);
    if (ret == LWM2M_RET_OK)
    {        
        SCI_TRACE_LOW( "MIPL# lwm2m observe_rsp success  [%s  %d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else if (ret == LWM2M_RET_ERROR)
    {
        SCI_TRACE_LOW( "MIPL# lwm2m observe_rsp failed  [%s  %d]\n",__FUNCTION__,__LINE__);
        return -1;
    } 

}


int LWM2M_MIPLREADRSP(int ref,uint32_t msgid,uint8_t result,uint16_t objid,uint16_t instid,uint16_t resid,lwm2m_tpye_t valueType,uint32_t len,uint32_t value ,uint8_t index,uint8_t flag)
{
    uint8_t OnenetRet;
    //char *valuestr = NULL;
    //int64_t value = 0;
    //double dvalue = 0;
    //bool bvalue = false;
    Mipl_Cache_t *ReadRspCacheP = &gMiplCtx.ReadRspCache;
    int ret = 0;
    //char * valuestr ;
    SCI_TRACE_LOW( "LWM2MDEBUG  enter ------>  [%s  %d]\n",__FUNCTION__,__LINE__);


    if ((result != 1 && objid != 0 && instid!= 0 && resid!=0) ||
            (result == 1 && objid == 0 && instid== 0&&resid==0))
        {
            SCI_TRACE_LOW( "LWM2MDEBUG   PARAMETER ERROR  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        }

     if (result == 1)
            OnenetRet = ONENET_RESPONSE_READ;
        else if (result == 11)
            OnenetRet = ONENET_RESPONSE_BAD_REQUEST;
        else if (result == 12)
            OnenetRet = ONENET_RESPONSE_UNAUTHORIZED;
        else if (result == 13)
            OnenetRet = ONENET_RESPONSE_NOT_FOUND;
        else if (result == 14)
            OnenetRet = ONENET_RESPONSE_METHOD_NOT_ALLOWED;
        else if (result == 15)
            OnenetRet = ONENET_RESPONSE_NOT_ACCEPTABLE;
        else
        {
            SCI_TRACE_LOW( "MIPL# lwm2m result  failed  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        }
        
        
        if (OnenetRet == ONENET_RESPONSE_READ)
        {
             // check index
            if (ReadRspCacheP->LastIndex != 0 && (ReadRspCacheP->LastIndex - index) != 1)
            {
                SCI_TRACE_LOW( "LastIndex error [%s  %d]\n",__FUNCTION__,__LINE__);
                return -1;
            }
            else
                ReadRspCacheP->LastIndex = index; 
            
            if (valueType == lwm2m_string || valueType == lwm2m_opaque)
            {                
                
                if (flag == 1)
                {
                    if (ReadRspCacheP->SavedValue != NULL)
                    {
                        free(ReadRspCacheP->SavedValue);
                        ReadRspCacheP->SavedValue = NULL;
                        ReadRspCacheP->SavedLength = 0;
                    }

                    ReadRspCacheP->SavedValue = malloc(len + 1);
                    if (ReadRspCacheP->SavedValue == NULL)
                    {
                        SCI_TRACE_LOW( " malloc failed [%s  %d]\n",__FUNCTION__,__LINE__);
                        return -1;
                    }   
                    memset(ReadRspCacheP->SavedValue, 0, len + 1);
                    memcpy(ReadRspCacheP->SavedValue, value, len);
                    ReadRspCacheP->SavedLength = len;
                    //AT_CMD_RETURN(atCmdRespOK(pParam->engine));
                }
                else if (ReadRspCacheP->SavedValue != NULL)
                {
                    char *tmp = (char *)malloc(ReadRspCacheP->SavedLength + len + 1);
                    if (tmp == NULL)
                    {
                        SCI_TRACE_LOW( " malloc failed [%s  %d]\n",__FUNCTION__,__LINE__);
                        return -1;
                    }
                    memset(tmp, 0, ReadRspCacheP->SavedLength + len + 1);
                    memcpy(tmp, ReadRspCacheP->SavedValue, ReadRspCacheP->SavedLength);
                    memcpy(tmp + ReadRspCacheP->SavedLength, value, len);
                    free(ReadRspCacheP->SavedValue);
                    ReadRspCacheP->SavedValue = tmp;
                    ReadRspCacheP->SavedLength += len;
                    if (flag == 2)
                        SCI_TRACE_LOW( "  LWM2MDEBUG flag == 2 [%s  %d]\n",__FUNCTION__,__LINE__);
                }
                else if (flag == 0)
                {
                    ReadRspCacheP->SavedValue = malloc(len + 1);
                    if (ReadRspCacheP->SavedValue == NULL)
                    {
                        SCI_TRACE_LOW( " malloc failed [%s  %d]\n",__FUNCTION__,__LINE__);
                        return -1;
                    }
                    memset(ReadRspCacheP->SavedValue, 0, len + 1);
                    memcpy(ReadRspCacheP->SavedValue, value, len);
                    ReadRspCacheP->SavedLength = len;
                }
                else
                {
                    SCI_TRACE_LOW( "LWM2MDEBUG  error  [%s  %d]\n",__FUNCTION__,__LINE__);
                    ret = -1;
                }
                
                SCI_TRACE_LOW( "LWM2MDEBUG ReadRspCacheP->SavedValue :%s  [%s  %d]\n",ReadRspCacheP->SavedValue,__FUNCTION__,__LINE__);
            }
            else if (valueType == lwm2m_float ||valueType ==lwm2m_bool||valueType == lwm2m_integer )
            {
                if (flag != 0)
                    SCI_TRACE_LOW( "LWM2MDEBUG  error  [%s  %d]\n",__FUNCTION__,__LINE__);

                if (ReadRspCacheP->SavedValue != NULL)
                    SCI_TRACE_LOW( "LWM2MDEBUG  error  [%s  %d]\n",__FUNCTION__,__LINE__);

                ReadRspCacheP->SavedValue = malloc(len);
                if (ReadRspCacheP->SavedValue == NULL)
                {
                    SCI_TRACE_LOW( " malloc failed [%s  %d]\n",__FUNCTION__,__LINE__);
                    ret = -1;
                }
                ReadRspCacheP->SavedLength = len;                
                memcpy(ReadRspCacheP->SavedValue, &value, len);
            }
            else
            {
                 SCI_TRACE_LOW( "LWM2MDEBUG type error  [%s  %d]\n",__FUNCTION__,__LINE__);
                    return -1;
            }            
            
            //SCI_TRACE_LOW( "LWM2MDEBUG msgid :%d , objid:%d ,  instid:%d ,resid :%d , valueType :%d  ,[%s  %d]\n",msgid,objid,instid,resid,valueType,ReadRspCacheP->SavedValue,__FUNCTION__,__LINE__);
             ret = lwm2m_read_rsp_onenet(msgid, objid, instid, resid, valueType,
                                         (uint8_t *)ReadRspCacheP->SavedValue, ReadRspCacheP->SavedLength,
                                        (index != 0 ? ONENET_RESPONSE_CONTINUE : ONENET_RESPONSE_READ), ref);
            
        }
        else
        {
            ret = lwm2m_read_rsp_onenet(msgid, 0, 0, 0, 0, NULL, 0, OnenetRet, ref);            
        }    

        if (ret != LWM2M_RET_OK)
        {
            SCI_TRACE_LOW( "LWM2MDEBUG lwm2m_read_rsp_onenet fail  [%s  %d]\n",__FUNCTION__,__LINE__);
            ret =-1;
        }
        if (ReadRspCacheP->SavedValue)
            free(ReadRspCacheP->SavedValue);
        ReadRspCacheP->SavedValue = NULL;
        ReadRspCacheP->SavedLength = 0;
        SCI_TRACE_LOW( "LWM2MDEBUG  end  <------  [%s  %d]\n",__FUNCTION__,__LINE__);
        return ret;       

}


int  LWM2M_MIPLWRITERSP(int ref,uint32_t msgid,uint8_t result)
{
    uint8_t OnenetRet;
    lwm2m_ret_t ret;

     if (result == 2)
            OnenetRet = ONENET_RESPONSE_WRITE;
        else if (result == 11)
            OnenetRet = ONENET_RESPONSE_BAD_REQUEST;
        else if (result == 12)
            OnenetRet = ONENET_RESPONSE_UNAUTHORIZED;
        else if (result == 13)
            OnenetRet = ONENET_RESPONSE_NOT_FOUND;
        else if (result == 14)
            OnenetRet = ONENET_RESPONSE_METHOD_NOT_ALLOWED;
        else if (result == 15)
            OnenetRet = ONENET_RESPONSE_NOT_ACCEPTABLE;
        else
        {
            SCI_TRACE_LOW( "MIPL# lwm2m result  failed  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        }

        ret = lwm2m_write_rsp_onenet(msgid, OnenetRet, ref);
        if (ret == LWM2M_RET_OK)
        {        
            SCI_TRACE_LOW( "MIPL# lwm2m write_rsp success  [%s  %d]\n",__FUNCTION__,__LINE__);
            return 0;
        }
        else if (ret == LWM2M_RET_ERROR)
        {
            SCI_TRACE_LOW( "MIPL# lwm2m write_rsp failed  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        } 

}



int LWM2M_MIPLNOTIFY(int ref,uint32_t msgid,uint16_t objid,uint16_t instid,uint16_t resid,lwm2m_tpye_t valueType,uint32_t len,uint32_t value ,uint8_t index,uint8_t flag)
{
    
    Mipl_Cache_t *NotifyCacheP = &gMiplCtx.NotifyCache;
    int ret = 0;
    //char * valuestr ;
    SCI_TRACE_LOW( "LWM2MDEBUG  enter ------>  [%s  %d]\n",__FUNCTION__,__LINE__);
      
     // check index
    if (NotifyCacheP->LastIndex != 0 && (NotifyCacheP->LastIndex - index) != 1)
    {
        SCI_TRACE_LOW( "LastIndex error [%s  %d]\n",__FUNCTION__,__LINE__);
        return -1;
    }
    else
        NotifyCacheP->LastIndex = index; 
    
    if (valueType == lwm2m_string || valueType == lwm2m_opaque)
    {                
        
        if (flag == 1)
        {
            if (NotifyCacheP->SavedValue != NULL)
            {
                free(NotifyCacheP->SavedValue);
                NotifyCacheP->SavedValue = NULL;
                NotifyCacheP->SavedLength = 0;
            }

            NotifyCacheP->SavedValue = malloc(len + 1);
            if (NotifyCacheP->SavedValue == NULL)
            {
                SCI_TRACE_LOW( " malloc failed [%s  %d]\n",__FUNCTION__,__LINE__);
                return -1;
            }   
            memset(NotifyCacheP->SavedValue, 0, len + 1);
            memcpy(NotifyCacheP->SavedValue, value, len);
            NotifyCacheP->SavedLength = len;
            //AT_CMD_RETURN(atCmdRespOK(pParam->engine));
        }
        else if (NotifyCacheP->SavedValue != NULL)
        {
            char *tmp = (char *)malloc(NotifyCacheP->SavedLength + len + 1);
            if (tmp == NULL)
            {
                SCI_TRACE_LOW( " malloc failed [%s  %d]\n",__FUNCTION__,__LINE__);
                return -1;
            }
            memset(tmp, 0, NotifyCacheP->SavedLength + len + 1);
            memcpy(tmp, NotifyCacheP->SavedValue, NotifyCacheP->SavedLength);
            memcpy(tmp + NotifyCacheP->SavedLength, value, len);
            free(NotifyCacheP->SavedValue);
            NotifyCacheP->SavedValue = tmp;
            NotifyCacheP->SavedLength += len;
            if (flag == 2)
                SCI_TRACE_LOW( "  LWM2MDEBUG flag == 2 [%s  %d]\n",__FUNCTION__,__LINE__);
        }
        else if (flag == 0)
        {
            NotifyCacheP->SavedValue = malloc(len + 1);
            if (NotifyCacheP->SavedValue == NULL)
            {
                SCI_TRACE_LOW( " malloc failed [%s  %d]\n",__FUNCTION__,__LINE__);
                return -1;
            }
            memset(NotifyCacheP->SavedValue, 0, len + 1);
            memcpy(NotifyCacheP->SavedValue, value, len);
            NotifyCacheP->SavedLength = len;
        }
        else
        {
            SCI_TRACE_LOW( "LWM2MDEBUG  error  [%s  %d]\n",__FUNCTION__,__LINE__);
            ret = -1;
        }
        
        SCI_TRACE_LOW( "LWM2MDEBUG ReadRspCacheP->SavedValue :%s  [%s  %d]\n",NotifyCacheP->SavedValue,__FUNCTION__,__LINE__);
    }
    else if (valueType == lwm2m_float ||valueType ==lwm2m_bool||valueType == lwm2m_integer )
    {
        if (flag != 0)
            SCI_TRACE_LOW( "LWM2MDEBUG  error  [%s  %d]\n",__FUNCTION__,__LINE__);

        if (NotifyCacheP->SavedValue != NULL)
            SCI_TRACE_LOW( "LWM2MDEBUG  error  [%s  %d]\n",__FUNCTION__,__LINE__);

        NotifyCacheP->SavedValue = malloc(len);
        if (NotifyCacheP->SavedValue == NULL)
        {
            SCI_TRACE_LOW( " malloc failed [%s  %d]\n",__FUNCTION__,__LINE__);
            ret = -1;
        }
        NotifyCacheP->SavedLength = len;                
        memcpy(NotifyCacheP->SavedValue, &value, len);
    }
    else
    {
         SCI_TRACE_LOW( "LWM2MDEBUG type error  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
    }            
    
    ret = lwm2m_notify_onenet(msgid, objid, instid, resid, valueType,
                                  (uint8_t *)NotifyCacheP->SavedValue, NotifyCacheP->SavedLength,
                                  (index != 0 ? ONENET_NOTIFY_CONTINUE : ONENET_NOTIFY_CONTENT), 0, ref);


    if (ret != LWM2M_RET_OK)
    {
        SCI_TRACE_LOW( "LWM2MDEBUG lwm2m_read_rsp_onenet fail  [%s  %d]\n",__FUNCTION__,__LINE__);
        ret =-1;
    }
    if (NotifyCacheP->SavedValue)
        free(NotifyCacheP->SavedValue);
    NotifyCacheP->SavedValue = NULL;
    NotifyCacheP->SavedLength = 0;
    SCI_TRACE_LOW( "LWM2MDEBUG  end  <------  [%s  %d]\n",__FUNCTION__,__LINE__);
    return ret;       

}


int LWM2M_MIPLEXECUTERSP(int ref,uint32_t msgid,uint8_t result)
{
    lwm2m_ret_t ret;
    uint8_t OnenetRet;
    
     if (result == 2)
            OnenetRet = ONENET_RESPONSE_EXECUTE;
        else if (result == 11)
            OnenetRet = ONENET_RESPONSE_BAD_REQUEST;
        else if (result == 12)
            OnenetRet = ONENET_RESPONSE_UNAUTHORIZED;
        else if (result == 13)
            OnenetRet = ONENET_RESPONSE_NOT_FOUND;
        else if (result == 14)
            OnenetRet = ONENET_RESPONSE_METHOD_NOT_ALLOWED;
        else if (result == 15)
            OnenetRet = ONENET_RESPONSE_NOT_ACCEPTABLE;
        else
        {
            SCI_TRACE_LOW( "MIPL# lwm2m result  failed  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        }
        ret = lwm2m_execute_rsp_onenet(msgid, OnenetRet, ref);
        if (ret == LWM2M_RET_OK)
        {        
            SCI_TRACE_LOW( "MIPL# lwm2m EXE_rsp success  [%s  %d]\n",__FUNCTION__,__LINE__);
            return 0;
        }
        else if (ret == LWM2M_RET_ERROR)
        {
            SCI_TRACE_LOW( "MIPL# lwm2m EXE_rsp failed  [%s  %d]\n",__FUNCTION__,__LINE__);
            return -1;
        } 

}




int LWM2M_TEST_create(int is_dtls)
{
    int ref = 0;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    

    if(is_dtls ==1 )
    {        
        char * host = "183.230.40.40";    //DTLS for onenet 
        char * port = "5684";   //dtls
        char * imei = "867400021435205";
        char * imsi = "460022225441927";
        uint32_t lifetime = 3600;
        uint32_t timeout = 30;
        BOOLEAN bs_enable = FALSE;
        char * pskId = "1015638782";
        //char * pskId = "867400021435205";
        char * psk = "111222333444";
        char * authcode = "xiamengyang";

        if((ref =LWM2M_MIPLCREATE(host,port,imei,imsi,pskId,psk,authcode,bs_enable) )==-1)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG create error:[%s  %d]\n",__FUNCTION__,__LINE__);     
            return -1;

        }
        SCI_TRACE_LOW( " LWM2MDEBUG end <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
        return 0;
        
    }
    else
    {
        char * host = "183.230.40.39";    //bootstrap for onenet 
        char * port = "5683";
        char * imei = "867400021435205";
        char * imsi = "460022225441927";
        uint32_t lifetime = 3600;
        uint32_t timeout = 30;
        BOOLEAN bs_enable = TRUE;   
       
        if((ref =LWM2M_MIPLCREATE(host,port,imei,imsi,NULL,NULL,NULL,bs_enable) )==-1)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG create error:[%s  %d]\n",__FUNCTION__,__LINE__);     
            return -1;

        }
        SCI_TRACE_LOW( " LWM2MDEBUG end <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
        return 0;
    }
}
    



int LWM2M_TEST_register_bootstrap(void)
{
    int ref = 0;
    char * host = "183.230.40.39";    //bootstrap for onenet 
    char * port = "5683";
    char * imei = "867400021435205";
    char * imsi = "460022225441927";
    uint32_t lifetime = 3600;
    uint32_t timeout = 30;
    BOOLEAN bs_enable = TRUE;

   SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);   

   if(gMiplConnectRef ==-1)
    {
        if(ref =LWM2M_MIPLCREATE(host,port,imei,imsi,NULL,NULL,NULL,bs_enable)==-1)
        {
            SCI_TRACE_LOW( " LWM2MDEBUG create error:[%s  %d]\n",__FUNCTION__,__LINE__);     
            return 0;

        }
        else
        {            
            if(LWM2M_MIPLOPEN(ref,lifetime,timeout) == -1)
            {
                SCI_TRACE_LOW( " LWM2MDEBUG register error:[%s  %d]\n",__FUNCTION__,__LINE__);     
                return -1;
            }
            else
            {
                SCI_TRACE_LOW( " LWM2MDEBUG register is running:[%s  %d]\n",__FUNCTION__,__LINE__);     
            }
        }
    }
   else
    {        
        if(LWM2M_MIPLOPEN(ref,lifetime,timeout) == -1)
            {
                SCI_TRACE_LOW( " LWM2MDEBUG register error:[%s  %d]\n",__FUNCTION__,__LINE__);     
                return -1;
            }
            else
            {
                SCI_TRACE_LOW( " LWM2MDEBUG register is running:[%s  %d]\n",__FUNCTION__,__LINE__);     
            }
    }   
    
    SCI_TRACE_LOW( " LWM2MDEBUG end <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return 0;
}


int LWM2M_TEST_close(int ref)
{
    int ret;
    if(ref != -1)
    {
        ret =LWM2MW_MIPLCLOSE(ref);
    }
    return ret;
}

int LWM2M_TEST_add_object(int ref,uint16_t objid )
{
    //uint16_t objid = 3303;    
    uint32_t instCounts = 1;
    const char *instBitmap = "1";
    uint8_t attcount = 6;
    uint8_t actcount = 1;
    int ret;   
    
     SCI_TRACE_LOW( " LWM2MDEBUG enter  ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    
     SCI_TRACE_LOW( " LWM2MDEBUG objid :%d    [%s  %d]\n",objid,__FUNCTION__,__LINE__);    
    if(LWM2M_MIPLADD_OBJECT(ref,objid,instCounts,instBitmap,attcount,actcount)!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG   test add success:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret = 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  test add faild:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret= -1;
    }
    SCI_TRACE_LOW( " LWM2MDEBUG end   <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return ret;
}


int LWM2M_TEST_del_object(int ref,uint16_t objid)
{
    //uint16_t objid = 3303;  
    int ret;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    
    SCI_TRACE_LOW( " LWM2MDEBUG objid :%d    [%s  %d]\n",objid,__FUNCTION__,__LINE__);    
    if(LWM2M_MIPLDEL_OBJECT(ref,objid)!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG   test del success:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret = 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  test del faild:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret= -1;
    }
    SCI_TRACE_LOW( " LWM2MDEBUG end   <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return ret;
    
}

int LWM2M_TEST_observersp(int ref)
{
    //uint16_t objid = 3303;  
    int ret;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    
    
    g_test_observersp_enable =TRUE;
    
    if(LWM2M_TEST_register_bootstrap()!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG   register success:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret = 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  registerl faild:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret= -1;
    }
    
    SCI_TRACE_LOW( " LWM2MDEBUG end   <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return ret;
    
}

int LWM2M_TEST_discoverrsp(int ref)
{
    //uint16_t objid = 3303;  
    int ret;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    
    
    g_test_observersp_enable =TRUE;
    g_test_discoverrsp_enable = TRUE;

    
    
    if(LWM2M_TEST_register_bootstrap()!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG   register success:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret = 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  registerl faild:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret= -1;
    }
    
    SCI_TRACE_LOW( " LWM2MDEBUG end   <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return ret;
    
}


int LWM2M_TEST_readrsp(int ref)
{
    //uint16_t objid = 3303;  
    int ret;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    
    
    g_test_observersp_enable =TRUE;
    g_test_discoverrsp_enable = TRUE;
    g_test_readrsp_enable = TRUE;
    

    
    
    if(LWM2M_TEST_register_bootstrap()!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG   register success:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret = 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  registerl faild:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret= -1;
    }
    
    SCI_TRACE_LOW( " LWM2MDEBUG end   <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return ret;
    
}



int LWM2M_TEST_writersp(int ref)
{
    //uint16_t objid = 3303;  
    int ret;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    
    
    g_test_observersp_enable =TRUE;
    g_test_discoverrsp_enable = TRUE;
    g_test_readrsp_enable = TRUE;
    g_test_writersp_enable = TRUE;    

    
    
    if(LWM2M_TEST_register_bootstrap()!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG   register success:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret = 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  registerl faild:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret= -1;
    }
    
    SCI_TRACE_LOW( " LWM2MDEBUG end   <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return ret;
    
}



int LWM2M_TEST_notify(int ref)
{
    //uint16_t objid = 3303;  
    int ret;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    
    
    g_test_observersp_enable =TRUE;
    g_test_discoverrsp_enable = TRUE;
    g_test_readrsp_enable = TRUE;
    g_test_writersp_enable = TRUE; 
    g_test_notify_enable = TRUE;

    
    
    if(LWM2M_TEST_register_bootstrap()!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG   register success:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret = 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  registerl faild:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret= -1;
    }
    
    SCI_TRACE_LOW( " LWM2MDEBUG end   <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return ret;
    
}

int LWM2M_TEST_NOTIFY_POSITIVE(int ref)
{
    char  value[30] = "yxmtestNOTIFYpositive";  //string
    if(LWM2M_MIPLNOTIFY(ref,observeid,objid,instanceid,resid,1,21,value,0,0)!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG NOTIFY SEND OK  [%s  %d]\n",__FUNCTION__,__LINE__);
    }
    else
    {
        SCI_TRACE_LOW( "LWM2MDEBUG NOTIFY SEND error  [%s  %d]\n",__FUNCTION__,__LINE__);
    }
}




int LWM2M_TEST_executersp(int ref)
{
    //uint16_t objid = 3303;  
    int ret;
    SCI_TRACE_LOW( " LWM2MDEBUG enter ----------->:[%s  %d]\n",__FUNCTION__,__LINE__);    
    
    g_test_observersp_enable =TRUE;
    g_test_discoverrsp_enable = TRUE;
    g_test_readrsp_enable = TRUE;
    g_test_writersp_enable = TRUE;    
     g_test_notify_enable = TRUE;
    g_test_execute_enable = TRUE;

    
    
    if(LWM2M_TEST_register_bootstrap()!=-1)
    {
        SCI_TRACE_LOW( " LWM2MDEBUG   register success:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret = 0;
    }
    else
    {
        SCI_TRACE_LOW( " LWM2MDEBUG  registerl faild:[%s  %d]\n",__FUNCTION__,__LINE__);    
        ret= -1;
    }
    
    SCI_TRACE_LOW( " LWM2MDEBUG end   <-----------:[%s  %d]\n",__FUNCTION__,__LINE__);    
    return ret;
    
}

#endif



