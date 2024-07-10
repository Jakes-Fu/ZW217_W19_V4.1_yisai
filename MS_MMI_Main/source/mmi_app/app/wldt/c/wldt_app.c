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
#ifndef  _MMI_WLDT_APP_C_
#define _MMI_WLDT_APP_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "wldt_app.h"

MMI_APPLICATION_T   		g_wldt_app;
static BOOLEAN s_wldt_app_init = FALSE;
static uint8 g_wldt_check_timer = 0;
static uint16 s_wldt_app_cur_day = 0;

extern void WLDT_AppTrace(const char *pszFmt, ...);

LOCAL void MMIWLDT_CheckTimerOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    if(g_wldt_check_timer == timer_id)
    {
        MMIWLDT_SendSigTo_APP(WLDT_APP_SIGNAL_CHECK_TIME,NULL,0);
    }
    return ;
}

LOCAL void MMIWLDT_CheckTimerStart(void)
{
    if(0 != g_wldt_check_timer)
    {
        MMK_StopTimer(g_wldt_check_timer);
        g_wldt_check_timer = 0;
    }
    
    g_wldt_check_timer = MMK_CreateTimerCallback(60000, 
                                                                        MMIWLDT_CheckTimerOut, 
                                                                        PNULL, 
                                                                        TRUE);
}

PUBLIC void MMIWLDT_CheckTimerStop(void)
{
    if(0 != g_wldt_check_timer)
    {
        MMK_StopTimer(g_wldt_check_timer);
        g_wldt_check_timer = 0;
    }
}

static BOOLEAN MMIWLDT_HandleCheckTimer(void)
{
    SCI_TIME_T          time;
    SCI_DATE_T          date;

    TM_GetSysDate(&date);
    TM_GetSysTime(&time);

    if(s_wldt_app_cur_day == 0)
    {
        s_wldt_app_cur_day = date.mday;
    }
    if(s_wldt_app_cur_day != date.mday)
    {
        s_wldt_app_cur_day = date.mday;
    }

#ifdef WLDT_PLAT_CY_SUPPORT
        CY_Net_TimerCheck(date,time);
#endif
    return TRUE;
}

static MMI_RESULT_E  HandleWLDTAppMsg (PWND app_ptr, 
								 uint16 msg_id, 
								 DPARAM param)
{
    MMI_RESULT_E res = MMI_RESULT_TRUE;
    
    #ifdef WLDT_NET_SUPPORT
    if(MMI_RESULT_TRUE == MMIWLDT_Net_Handle_AppMsg(app_ptr,msg_id,param))
    {
        return MMI_RESULT_TRUE;
    }
    #endif
    switch(msg_id)
    {                       
        case WLDT_APP_SIGNAL_CHECK_TIME:
                MMIWLDT_HandleCheckTimer();
            break;
            
        default:
                res = MMI_RESULT_FALSE;
            break;
    }
    return res;
}

void MMIWLDT_AppInit(void)
{
    g_wldt_app.ProcessMsg     = HandleWLDTAppMsg;
    WLDT_DB_Init();

#ifdef WLDT_NET_SUPPORT
    MMIWLDT_Net_APP_Init();
#endif

    MMIWLDT_CheckTimerStart();
    s_wldt_app_init = TRUE;
}

BOOLEAN MMIWLDT_AppIsInit(void)
{
    return s_wldt_app_init;
}

void MMIWLDT_InitModule(void)
{
    MMIWLDT_RegMenuGroup();
    MMIWLDT_RegNv();
    MMIWLDT_RegWinIdNameArr();
}

BOOLEAN  MMIWLDT_SendSigTo_APP(WLDT_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len)
{
    uint8 * pstr = NULL;
    MMI_WLDT_SIG_T * psig = PNULL;
    if(s_wldt_app_init == FALSE)
    {
        return FALSE;
    }
    if(data_ptr != PNULL && data_len != 0)
    {
        pstr = SCI_ALLOCA(data_len);//free it in AT task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("MMIWLDT_SendSigTo_APP Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(MMI_WLDT_SIG_T),SCI_IdentifyThread());
    psig->data.len = data_len;
    psig->data.str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig,P_APP);
    return TRUE;
    
}

void MMIWLDT_ResetFactory(void)
{
#ifdef WLDT_PLAT_CY_SUPPORT
    CY_RestorFactory();
#endif
    return;
}


#if 1
static uint8 Nibble2HexChar(uint8 bNibble)
{
    uint8  bRtn = '0';

    if(bNibble <= 9)
        bRtn = bNibble + '0';
    else if(bNibble <= 0x0F)
    {
        switch(bNibble)
        {
            case 10:
                bRtn = 'A';
                break;
            case 11:
                bRtn = 'B';
                break;
            case 12:
                bRtn = 'C';
                break;
            case 13:
                bRtn = 'D';
                break;
            case 14:
                bRtn = 'E';
                break;
            case 15:
                bRtn = 'F';
                break;
        }
    }
    return(bRtn);
}

static uint32 U8_To_Hex(uint8 *buf,uint32 len,uint8 * dest_buf,uint32 dest_len)
{
    uint8 low = 0;
    uint8 high = 0;
    uint32 i = 0;
    uint32 j = 0;

    if((len*2) > dest_len)
    {
        len = dest_len/2;
    }
    for(i = 0; i < len; i++)
    {
        high = buf[i]>>4;
        dest_buf[j++] = (char)Nibble2HexChar(high);
        low = buf[i] & 0x0F;
        dest_buf[j++] = (char)Nibble2HexChar(low);
    }
    dest_buf[j] = 0;
    return j;
}

PUBLIC void WLDT_HexTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TraceLow("[U8]:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);        
           SCI_TraceLow("[U8]:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void WLDT_AppTraceExt(const char *string, int len)
{
#if 1
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TraceLow("[WLDT]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[WLDT]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

void WLDT_AppTrace(const char *pszFmt, ...)
{
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif
    if (tmp_len >= 0)
    {
        WLDT_AppTraceExt(buf,tmp_len);
    }
    va_end(va_args);
}


PUBLIC void WLDT_TCPTraceExt(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[WLDT][TCP]:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[WLDT][TCP]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void WLDT_TCPTrace(const char *pszFmt, ...)
{
    char buf[1024] = {0};
    int tmp_len = 0;
    va_list va_args;    
    va_start(va_args, pszFmt);
#ifdef WIN32
    tmp_len = vsprintf(buf,pszFmt, va_args);
#else
    tmp_len = vsnprintf(buf,1023,pszFmt, va_args);
#endif
    if (tmp_len >= 0)
    {
        WLDT_TCPTraceExt(buf,tmp_len);
    }
    va_end(va_args);
}

PUBLIC void WLDT_TCPSendTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[TCP] Send:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[WLDT][TCP] Send:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void WLDT_TCPSendTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    uint16 i = 0;
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TRACE_LOW("[WLDT][TCP][U8] Send:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);
           i++;
           SCI_TRACE_LOW("[WLDT][TCP][U8] Send_%d:%s",i,dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

PUBLIC void WLDT_TCPRcvTrace(const char *string, int len)
{
#ifndef RELEASE_INFO
    char tmp_str[128] = {0};
    if (len < 128)
    {
        SCI_MEMCPY(tmp_str,string,len);
        SCI_TRACE_LOW("[WLDT][TCP] Rcv:%s",tmp_str);
    }
    else
    {
        char *p;
        char buff[128];

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TRACE_LOW("[WLDT][TCP] Rcv:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

PUBLIC void WLDT_TCPRcvTraceU8(const char *str, int len)
{
#ifndef RELEASE_INFO
    uint16 i = 0;
    char dest_buff[130] = {0};
    if(str == PNULL || len == 0)
    {
        return;
    }
    if (len < 64)
    {
        U8_To_Hex(str,len,dest_buff,128);
        SCI_TRACE_LOW("[WLDT][TCP][U8] Rcv:%s",dest_buff);
    }
    else
    {
        char *p;

        p = str;
        while (len > 0)
        {
           int l = len < 63 ? len : 63;
           SCI_MEMSET(dest_buff,0,sizeof(dest_buff));
           U8_To_Hex(p,l,dest_buff,128);
           i++;
           SCI_TRACE_LOW("[WLDT][TCP][U8] Rcv_%d:%s",i,dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}
#endif // End Trace
#endif
