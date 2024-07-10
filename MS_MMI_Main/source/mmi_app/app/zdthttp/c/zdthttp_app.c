/****************************************************************************
** File Name:      mmicdma_app.c                                           *
** Author:          jianshengqi                                                       *
** Date:           03/22/2006                                              *
** Copyright:      2006 tlt, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the PHS                   *
/****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2006       Jianshq         Create
** 
****************************************************************************/
#ifndef  _MMI_ZDTHTTP_APP_C_
#define _MMI_ZDTHTTP_APP_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "zdthttp_app.h"

MMI_APPLICATION_T   		g_zdthttp_app;

static MMI_RESULT_E  HandleZDTHTTPAppMsg (PWND app_ptr, 
								 uint16 msg_id, 
								 DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    if (HTTP_SIGNAL_BEG <= msg_id && msg_id <= HTTP_SIGNAL_END)
    {
        res = Http_HandleHttpMsg(app_ptr,msg_id,param);
        return res;
    }
    
    if(MMI_RESULT_TRUE == MMIHTTP_Net_Handle_AppMsg(app_ptr,msg_id,param))
    {
        return MMI_RESULT_TRUE;
    }

    switch(msg_id)
    {                       
        case ZDTHTTP_APP_SIGNAL_HTTP_SEND:
                MMIZDT_HTTP_Handle_Send(param);
            break;
            
        case ZDTHTTP_APP_SIGNAL_HTTP_RESEND:
                MMIZDT_HTTP_Handle_ReSend(param);
            break;
            
        case ZDTHTTP_APP_SIGNAL_HTTP_RCV:
                MMIZDT_HTTP_Handle_Rcv(param);
            break;
            
        case ZDTHTTP_APP_SIGNAL_HTTP_REFRESH:
                MMIZDT_HTTP_Handle_RefreshInd(param);
            break;
            
         case ZDTHTTP_APP_SIGNAL_HTTP_SEND_OVER:
                MMIZDT_HTTP_Handle_SendOver(param);
            break;
           
        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}

void MMIZDTHTTP_AppInit(void)
{
    g_zdthttp_app.ProcessMsg     = HandleZDTHTTPAppMsg;
#ifdef FILE_LOG_SUPPORT
    TraceUser_Init();
    ZDTHTTP_LOG("MMIZDTHTTP_AppInit");
#endif
    MMIZDT_HTTP_Init();
    MMIHTTP_Net_APP_Init();

}

void MMIZDTHTTP_InitModule(void)
{
    MMIZDTHTTP_RegMenuGroup();
    MMIZDTHTTP_RegNv();
    MMIZDTHTTP_RegWinIdNameArr();
}

BOOLEAN  MMIZDTHTTP_SendSigTo_APP(ZDTHTTP_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_ZDTHTTP_SIG_T * psig = PNULL;
        
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOCA(data_len);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("MMIZDTHTTP_SendSigTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_ZDTHTTP_SIG_T),SCI_IdentifyThread());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
    
}

PUBLIC BOOLEAN  MMIZDTHTTP_SendSig_Refresh(ZDTHTTP_REFRESH_DATA_T * p_http_data)
{
    uint8 * pstr = PNULL;
    ZDTHTTP_REFRESH_SIG_T * psig = PNULL;
            
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,ZDTHTTP_APP_SIGNAL_HTTP_REFRESH,sizeof(ZDTHTTP_REFRESH_SIG_T),SCI_IdentifyThread());
    psig->p_http_data = p_http_data;
    
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
}
#endif
