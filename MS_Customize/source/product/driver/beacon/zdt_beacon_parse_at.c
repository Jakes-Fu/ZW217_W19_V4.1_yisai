#ifndef _ZDT_BEACON_AT_C
#define _ZDT_BEACON_AT_C

#include "zdt_beacon_uart.h"
#include "zdt_beacon_parse_at.h"
#include "Dal_time.h"
#include "sig_code.h"


#ifdef BEACON_APP_SUPPORT
extern BOOLEAN  ZDTBEACON_SendTo_APP(BEACON_APP_SIG_E sig_id, uint8 * data_ptr, uint32 data_len);
#endif
extern SCI_MUTEX_PTR g_ib_mutexPtr;
uint32  g_beacon_hw_connect_ok_time = 0;
static   uint8 s_beacon_connnect_name[BEACON_NAME_MAX_LEN+1] = {0};
static   BOOLEAN s_beacon_is_in_sleep = FALSE;
static   uint8 s_beacon_connnect_id = 0;

const char* g_beacon_AtStr[ATRSP_IB_MAX_AT]=
{
    NULL,
    "OK",
    "ERR",             
    "+REBOOT",
    "+SLEEP:",
    "+CON_ROPE:",
    "+DISCONN:",
    "+DATA,",
    "+LINK:",
    "Link_ID:",
    "+SCAN:",
    "No:",
};

const BEACON_AT_MAP_TBL  g_beacon_cmdTbl[]=
{                                
    {ATRSP_IB_OK , HANDLE_BEACONAT_OK},
    {ATRSP_IB_ERR , HANDLE_BEACONAT_ERR},        
    {ATRSP_IB_REBOOT,     HANDLE_BEACONAT_REBOOT},         
    {ATRSP_IB_SLEEP,     HANDLE_BEACONAT_SLEEP},         
    {ATRSP_IB_CONN,     HANDLE_BEACONAT_CONN},         
    {ATRSP_IB_DISCONN,     HANDLE_BEACONAT_DISCONN},         
    {ATRSP_IB_DATA,     HANDLE_BEACONAT_DATA},         
    {ATRSP_IB_LINK , HANDLE_BEACONAT_LINK},
    {ATRSP_IB_LINK_ID , HANDLE_BEACONAT_LINK_ID},
    {ATRSP_IB_SCAN , HANDLE_BEACONAT_SCAN},
    {ATRSP_IB_NO, HANDLE_BEACONAT_NO},
    {   0,               0 }
};

int  BEACON_AT_GetNextPara(uint8** pCmd, uint16* pLen, char* pStr, uint16 wMaxStrLen)
{
    int  bRtn = 0, c, bHaveNextComma = FALSE;
    int    i, L = 0, EndIndex = 0;
    
    if(*pLen == 0)
    {
        return -1;
    }
    
    for(i=0; i < (*pLen); i++)
    {
        if(*(*pCmd + i) == ',')
        {
            bHaveNextComma = TRUE;
            break;
        }
    }
        
    EndIndex = i;

    if(EndIndex)
    {
        if(!wMaxStrLen)  // No limit if NULL
            wMaxStrLen = 100;
        for(i=0; i<EndIndex; i++)
        {
            if(wMaxStrLen)
            {
                *(pStr + L) = *(*pCmd + i);
                L ++;
                wMaxStrLen --;
            }
        }
        
        if(wMaxStrLen > 0)
        {
            *(pStr + L) = 0;
        }
        bRtn = EndIndex;
    }
    else
    {   // Nothing or empty
        bRtn = 0;
    }

    // Move command pointer to correct position and change the remainder length
    if(bRtn || bHaveNextComma)  // It does get something in command string
    {
        if(EndIndex == *pLen)   // Already to the end of string
        {
            *pCmd += EndIndex;
            *pLen = 0;
        }
        else                    // There is still something in string
        {
            *pCmd += (EndIndex + 1);
            *pLen -= (EndIndex + 1);
        }
    }
    return(bRtn);
}


void HANDLE_BEACONAT_OK(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    return;
}

void HANDLE_BEACONAT_ERR(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    return;
}

void HANDLE_BEACONAT_REBOOT(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_REBOOT,PNULL,1);
    return;
}

void HANDLE_BEACONAT_SLEEP(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    if(len >= 1 && 0 == strncmp((char *)str, "W", 1))
    {
        s_beacon_is_in_sleep = FALSE;
        ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_SLEEP,PNULL,1);
    }
    else if(len >= 4 && 0 == strncmp((char *)str, "AUTO", 4))
    {
        s_beacon_is_in_sleep = TRUE;  
        ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_SLEEP,PNULL,0);
    }
    else
    {
        s_beacon_is_in_sleep = TRUE;
        ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_SLEEP,PNULL,0);
    }
    
    return;
}

void HANDLE_BEACONAT_CONN(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    if(len >= 4 && 0 == strncmp((char *)str, "SCAN", 4))
    {
        //开始扫描
        ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_CONN,PNULL,1);
    }
    else if(len >= 6 && 0 == strncmp((char *)str, "CON...", 6))
    {
        //正在连接
        ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_CONN,PNULL,2);
    }
    else if(len >= 7 && 0 == strncmp((char *)str, "TIMEOUT", 7))
    {
        //连接超时
        ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_CONN,PNULL,3);
    }
    else if(len >= 7 && 0 == strncmp((char *)str, "NAME=", 5))
    {
        //连接成功返回名称
        SCI_MEMSET(s_beacon_connnect_name,0,sizeof(s_beacon_connnect_name))
        SCI_MEMCPY(s_beacon_connnect_name,str+5,len-7);
        ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_CONN,PNULL,4);
        ZDT_IBTrace("NAME=%s",s_beacon_connnect_name);
    }
    return;
}

void HANDLE_BEACONAT_DISCONN(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_DISCONN,str,len);
    return;
}

void HANDLE_BEACONAT_DATA(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    if(str == PNULL || len == 0)
    {
        return;
    }
    s_beacon_connnect_id = str[0];
    ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_DATA,str,len);
    return;
}

void HANDLE_BEACONAT_LINK(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_LINK,str,len);
    return;
}

void HANDLE_BEACONAT_LINK_ID(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_LINK_ID,str,len);
    return;
}

void HANDLE_BEACONAT_SCAN(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_SCAN,str,len);
    return;
}

void HANDLE_BEACONAT_NO(uint8 * str, uint16 len)
{
    ZDT_IBTrace("Entry %s len=%d",__func__,len);
    ZDTBEACON_SendTo_APP(BEACON_APP_SIGNAL_NO,str,len);
    return;
}

void BEACON_API_ReceiveAT(uint8* pCmd, uint16 len)
{
    uint16 cmd_Len = 0;
    uint16    i = 0;
    BOOLEAN  bFound = FALSE;
    BEACON_AT_MAP_TBL    * p_table = NULL;
    uint16   wLen = 0;
    uint8 *  pUint8 = NULL;
    
    if(pCmd == NULL || len < 2)
    {
        return;
    }
        
    wLen = len;
    pUint8 = pCmd;
    
    p_table = (BEACON_AT_MAP_TBL *)g_beacon_cmdTbl;
    while(p_table[i].wAtId)
    {
        cmd_Len = strlen(g_beacon_AtStr[p_table[i].wAtId]);
        if(0 == strncmp((char *)pUint8, g_beacon_AtStr[p_table[i].wAtId], cmd_Len))
        {
            bFound = TRUE;
            break;
        }
        i++;
    }
        
    if(bFound && p_table[i].CBFunc)
    {
        wLen -= cmd_Len;
        pUint8 += cmd_Len;
        p_table[i].CBFunc(pUint8, wLen);
    }

    return;
}

BOOLEAN BEACON_API_IsSleep(void)
{
    return s_beacon_is_in_sleep;
}

char * BEACON_API_GetBTName(void)
{
    return s_beacon_connnect_name;
}
#endif/*_ZDT_BEACON_AT_C*/
