#include "ai_app.h"
#ifdef TULING_AI_SUPPORT
#include "ai_app.h"

static MMI_RESULT_E  HandleAiAppMsg (PWND app_ptr, 
                                    uint16 msg_id, 
                                    DPARAM param);



AI_APP_T  g_ai_app = {0};
static BOOLEAN s_ai_app_init = FALSE;
//AI_TCP_INTERFACE_T  g_zdt_tcp_interface_array[ZDT_TCP_INTERFACE_MAX] = {0};
AI_TCP_INTERFACE_T  g_ai_tcp_interface;



MMI_APPLICATION_T   		mmi_ai_app;




void MMIAI_AppInit(void)
{
    mmi_ai_app.ProcessMsg     = HandleAiAppMsg;

#ifdef ZDT_TCP_SUPPORT
    AITCP_AppInit();
#endif
    AI_AppInit();
    
    s_ai_app_init = TRUE;

}






PUBLIC int AI_AppInit(void)
{
    AI_APP_T * pMe = &g_ai_app;
    pMe->m_ai_tcp_reg_interface = &g_ai_tcp_interface; //bug ,need first init

    
    AI_LOG("AI_AppInit");
       return 0;
}

extern int AI_AppFree(void)
{
    AI_APP_T * pMe = &g_ai_app;
    AI_LOG("HYHL_AppFree");
      return 0;
}

extern AI_APP_T * AI_AppGet(void)
{
    return &g_ai_app;
}

int AI_RestorFactory(void)
{
    AI_APP_T * pMe = &g_ai_app;
    AI_LOG("AI_RestorFactory");

    return 0;
}

extern MMI_RESULT_E AI_Handle_AppMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    AI_APP_T * pMe = &g_ai_app;
    AI_LOG("AI_Handle_AppMsg ");
    switch (msg_id)
    {
        case AI_APP_SIGNAL_AI_RECORD_SEND:
            //AI_Voice_Send_Start(pMe);
            break;
            
        case AI_APP_SIGNAL_AI_RECORD_START:
            AI_Voice_Record_Start(pMe);
            break;
            
        case AI_APP_SIGNAL_AI_RECORD_SUCCESS:
            AI_Voice_Record_HandleEnd(TRUE);
            break;
                
        case AI_APP_SIGNAL_AI_RECORD_FAIL:
            AI_Voice_Record_HandleEnd(FALSE);
            break;
                
        case AI_APP_SIGNAL_AI_RECORD_TIMEOUT:
            AI_Voice_Record_StopTimout(pMe);
            break;
            
        case AI_APP_SIGNAL_AI_RECORD_STOP:
            AI_Voice_Record_Stop(pMe);
            break;

        //http get receive mp3 file play
        case AI_APP_SIGNAL_AI_RCV_MP3_PLAY:
            //AI_Voice_Rcv_Mp3_Play(pMe);//only play one time, for small file 
            AI_Voice_Rcv_Mp3_PlayBuf(pMe);// play buffer list, problem ,discontinuous
            //AI_Voice_Rcv_Mp3_PlayAllData(pMe);// play all data ,need wait
            //AI_Voice_Rcv_Mp3_PlayBufThreadStart();//still have bug, dead
            //AI_Voice_Rcv_Mp3_PlayFixBuf(pMe);
            break;
        case AI_APP_SIGNAL_AI_RCV_MP3_STOP:
            //AI_Voice_Rcv_Mp3_Play(pMe);//only play one time, for small file 
            AI_Voice_Rcv_Mp3_StopPlayBuf(pMe);// play buffer list, 
            
            break;

        default:
            res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}





BOOLEAN  MMIAI_SendSigTo_APP(AI_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_AI_SIG_T * psig = PNULL;
    AI_LOG("MMIAI_SendSigTo_APP ");
    if(s_ai_app_init == FALSE)  // bug need to init s_ai_app_init fisrt ,otherwise will not send msg
    {
        return FALSE;
    }
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOCA(data_len);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("MMIAI_SendSigTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_AI_SIG_T),SCI_IdentifyThread());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    AI_LOG("MMIAI_SendSigTo_APP end");
    return TRUE;

   
    
}


static MMI_RESULT_E  HandleAiAppMsg (PWND app_ptr, 
                                    uint16 msg_id, 
                                    DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_FALSE;
    AI_LOG("HandleAiAppMsg  msg_id =%x", msg_id);

    
    if(msg_id >= AI_APP_SIGNAL_START && msg_id <= AI_APP_SIGNAL_END)
    {
        if(MMI_RESULT_TRUE == MMIAI_Net_Handle_AppMsg(app_ptr,msg_id,param))
        {
            return MMI_RESULT_TRUE;
        }
       
    
        if(MMI_RESULT_TRUE == AI_Handle_AppMsg(app_ptr,msg_id,param))
        {
           
            return MMI_RESULT_TRUE;
           
        }

    }
    
    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END) //handle HTTP msg
    {
        AI_LOG("HandleAiAppMsg HandleAiHttpMsgExt");
        res = HandleAiHttpMsgExt(app_ptr,msg_id,param);
    }
    
    
 
    return res;
}



#endif
