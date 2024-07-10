#ifndef _ZDT_IB_UART_C
#define _ZDT_IB_UART_C

#include "zdt_beacon_uart.h"
#include "zdt_beacon_parse_at.h"
#include "ldo_drvapi.h"
#include "dal_time.h"

#define ZDT_IB_TX_WATER_MARK	    8
#define ZDT_IB_RX_WATER_MARK   	13

#define ZDT_IB_UART_PORT   2               //uart port

#define ZDT_IB_SEND_BUFFER_SIZE 2048        //default com send/receive buffer size

#define IB_DATA_MAX_LEN 102

#define y_abs(x)  ((x) >= 0 ? (x) : (-(x)))

//Cycle Queue struct
typedef struct {
    uint8 *buf;
    uint32 size;        //queue volume
    uint32 head;
    uint32 tail;
    unsigned empty: 1;
    unsigned full:  1;
    unsigned overflow:  1;  //queue input after full, overwrite occur
} IBCycleQueue;

__align (4) static uint8 s_ib_send_buf[ZDT_IB_SEND_BUFFER_SIZE];

//com send queue
static IBCycleQueue Output_Q = {
    s_ib_send_buf,
    ZDT_IB_SEND_BUFFER_SIZE,
    0,
    0,
    1,              //empty
    0,         
    0,
};

static uint8 ib_cmd_line_buffer[IB_DATA_MAX_LEN];     /*  buffer for AT cmd line  */
static uint16 ib_cmd_line_len = 0;
static uint16 ib_hex_need_len = 0;
static uint8 *ib_cmd_ptr;                     /* next loc in buffer       */
static uint8 ib_cmd_status = 0;

static uint8 ib_uart_is_on = 0;
static BOOLEAN is_ib_ok = FALSE;
static BLOCK_ID zdtib_taskid=SCI_INVALID_BLOCK_ID;
BOOLEAN g_is_ib_hw_on = FALSE;
BOOLEAN g_is_ib_not_off = FALSE;
uint32 g_ib_hw_pwon_time = 0;

static SCI_TIMER_PTR      s_zdt_ib_check_timer = PNULL;
static uint8 s_is_ib_hw_check = 0;
static BOOLEAN s_ib_hw_check_laststatus = FALSE;

SCI_MUTEX_PTR g_ib_mutexPtr = NULL;
static SCI_TIMER_PTR      s_ib_out_timer = PNULL;

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

PUBLIC void ZDT_IBTraceExt(const char *string, int len)
{
#if 1
    if (len < 128)
    {
        char buff[128] = {0};
        SCI_MEMCPY(buff, string, len);
        SCI_TraceLow("[IB]:%s",buff);
    }
    else
    {
        char *p;
        char buff[128] = {0};

        p = string;
        SCI_MEMSET(buff,0,sizeof(buff));
        while (len > 0)
        {
           int l = len < 127 ? len : 127;
           SCI_MEMCPY(buff, p, l);
          buff[l] = 0;
        
           SCI_TraceLow("[IB]:%s",buff);
        
          p += l;
          len -= l;
        }
    }
#endif
}

void ZDT_IBTrace(const char *pszFmt, ...)
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
        ZDT_IBTraceExt(buf,tmp_len);
    }
    va_end(va_args);
}

PUBLIC void ZDT_IBTraceU8(const char *str, int len)
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
        SCI_TraceLow("[IB][U8]:%s",dest_buff);
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
           SCI_TraceLow("[IB][U8]:%s",dest_buff);
            p += l;
            len -= l;
        }
    }
#endif
}

static void ZDTIB_OutTimerOut(uint32 param)
{	    
    ZDT_IBTrace("ZDTIB_OutTimerOut");
    APP_SendSigTo_IB(SIG_ZDT_IB_POS_OUT,PNULL,0);
    return;
}

BOOLEAN  ZDTIB_OutTimer_Start(void)
{
    if(s_ib_out_timer == PNULL)
    {
        s_ib_out_timer = SCI_CreateTimer("IBOUT_Timer", ZDTIB_OutTimerOut,
                                    1, 10000, SCI_AUTO_ACTIVATE);
    }
    else
    {
        SCI_ChangeTimer(s_ib_out_timer, ZDTIB_OutTimerOut, 10000);
        SCI_ActiveTimer(s_ib_out_timer);
    }
    return TRUE;
}

BOOLEAN  ZDTIB_OutTimer_Stop(void)
{
    if(s_ib_out_timer != PNULL)
    {
        if(SCI_IsTimerActive(s_ib_out_timer))
        {
            SCI_DeactiveTimer(s_ib_out_timer);
        }
    }
    return TRUE;
}


/*****************************************************************************/
//  FUNCTION:     QueueClean
//  Description:    clean cycle queue
//  INPUT:      pointer to a IBCycleQueue
//  OUTPUT:     void
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:       
/*****************************************************************************/
static void QueueClean(IBCycleQueue *Q_ptr)
{
    Q_ptr->head = Q_ptr->tail = 0;
    Q_ptr->empty = 1;
    Q_ptr->full = 0;
    Q_ptr->overflow = 0;    
}

/*****************************************************************************/
//  FUNCTION:     ZDT_IB_QueueInsert(IBCycleQueue *Q_ptr, uint8 *data, uint32 len)
//  Description:    insert a string in cycle queue
//  INPUT:          Q_ptr--pointer to a IBCycleQueue, uint8 *data--string pointer, uint32 len--string length
//  OUTPUT:         >=0 actual insert number, <0 error
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:        when queue is full, new data wil overwrite olds.
/*****************************************************************************/
static int QueueInsert(IBCycleQueue *Q_ptr, uint8 *data, uint32 len)
{
    int ret = 0;
#if 1       //no overwrite    
    if (!Q_ptr->full)
    {
        while(ret < len)
        {
            *(Q_ptr->buf + Q_ptr->head) = *(data + ret);
            Q_ptr->head = ++Q_ptr->head % Q_ptr->size;
            ret++;
            
            if (Q_ptr->head == Q_ptr->tail)
            {
                Q_ptr->full = 1;
                break;
            }
        }
    }
    
    if ((1==Q_ptr->empty) && (ret > 0)) 
    {
        Q_ptr->empty = 0;
    }
#else       //overwrite
    for (; ret < len; ret++)
    {
        *(Q_ptr->buf + Q_ptr->head) = *(data + ret);
        
        if ((1==Q_ptr->full) && (Q_ptr->head==Q_ptr->tail))
        {
            Q_ptr->overflow = 1;               
        }
        
        Q_ptr->head = ++Q_ptr->head % Q_ptr->size;
        
        if (Q_ptr->head == Q_ptr->tail)
        {
            Q_ptr->full = 1;
        }
        
        if (1 == Q_ptr->empty)
        {
            Q_ptr->empty = 0;
        }
    }
    
    if (Q_ptr->overflow)
    {
        Q_ptr->tail = Q_ptr->head;
    }
#endif
   
    return ret;       
}


/*****************************************************************************/
//  FUNCTION:     QueueDelete(IBCycleQueue *Q_ptr, uint8 *data, uint32 len)
//  Description:    delete a string from cycle queue
//  INPUT:          Q_ptr--pointer to a IBCycleQueue, uint8 *data--string pointer, uint32 len--string length
//  OUTPUT:         >=0 actual delete number, <0 error
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:       
/*****************************************************************************/
static int QueueDelete(IBCycleQueue *Q_ptr, uint8 *data, uint32 len)
{
    int ret = 0;
    
    if (!Q_ptr->empty)
    {
        while(ret < len) 
        {
            *(data + ret) = *(Q_ptr->buf + Q_ptr->tail);
            Q_ptr->tail = ++Q_ptr->tail % Q_ptr->size;
            ret++;
            
            if (Q_ptr->tail == Q_ptr->head)
            {
                Q_ptr->empty = 1;
                break;
            }
        }
    }
   
    if ((ret>0) && (1==Q_ptr->full))   
    {
        Q_ptr->full = 0;
        Q_ptr->overflow = 0;
    }
    
    return ret;    
}

int zdt_ib_uart_write(uint8 *buf, uint32 len)
{
    int ret;
    if(ib_uart_is_on == 0)
    {
        return 0;
    }
    UART_Tx_Int_Enable(ZDT_IB_UART_PORT, FALSE);
    ret = QueueInsert(&Output_Q, buf, len);
    UART_Tx_Int_Enable(ZDT_IB_UART_PORT, TRUE);
    return ret;
}

/*****************************************************************************/
//  FUNCTION:     ib_uart1_callback(uint32 event)
//  Description:    callback function for uartcom_drv.
//  Author:         David.Jia
//  date:           2007.6.19
//	Note:           assume there is still data in when call UART_ReadData.
//      return value will large than called UART_GetRxFifoCnt.
//
/*****************************************************************************/
static uint8 zdt_ib_uart_process_at
(
  uint8 *cmd_line_ptr ,
  uint16  cmd_line_len
)
{
    APP_SendSigTo_IB(SIG_ZDT_IB_RCV,cmd_line_ptr,cmd_line_len);
    return 1;
}

static uint16 zdt_ib_uart_process_chars(uint8 cc)
{
    uint16 result = 0;
    switch ( ib_cmd_status )
    {
        case 0:
            /*-----------------------------------------------------------------
              In the HUNT state, the characters 'A' or 'a' are looked for.  
              Everything else is ignored. If 'A' or 'a' is detected, the   
              machine goes into the FOUND_A state.                              
            -----------------------------------------------------------------*/
            if(cc == 'O')
            {
                ib_cmd_ptr = &ib_cmd_line_buffer[0];
                *ib_cmd_ptr++ = cc;
                ib_cmd_line_len = 1;
                ib_cmd_status = 1;
                ib_hex_need_len = 0;
            }
            else if(cc == 'E')
            {
                ib_cmd_ptr = &ib_cmd_line_buffer[0];
                *ib_cmd_ptr++ = cc;
                ib_cmd_line_len = 1;
                ib_cmd_status = 2;
                ib_hex_need_len = 0;
            }
            else if(cc == '+')
            {
                ib_cmd_ptr = &ib_cmd_line_buffer[0];
                *ib_cmd_ptr++ = cc;
                ib_cmd_line_len = 1;
                ib_cmd_status = 4;
                ib_hex_need_len = 0;
            }
            else if(cc == 'N')
            {
                ib_cmd_ptr = &ib_cmd_line_buffer[0];
                *ib_cmd_ptr++ = cc;
                ib_cmd_line_len = 1;
                ib_cmd_status = 5;
                ib_hex_need_len = 0;
            }
            else if(cc == 'L')
            {
                ib_cmd_ptr = &ib_cmd_line_buffer[0];
                *ib_cmd_ptr++ = cc;
                ib_cmd_line_len = 1;
                ib_cmd_status = 7;
                ib_hex_need_len = 0;
            }
            break;
            
        case 1:
                if ( cc == 'K')
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 4;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;        
                }
            break;
            
        case 2:
                if ( cc == 'R')
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 3;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;        
                }
            break;
            
        case 3:
                if ( cc == 'R')
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 4;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;
                }
            break;
                        
         case 4:
            {
                if ( ib_cmd_ptr >= (ib_cmd_line_buffer + IB_DATA_MAX_LEN - 2) )
                {
                    ib_cmd_status = 0;
                    ib_cmd_ptr[0] = '\0';
                    break;
                }
                else
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                }
                
                if(cc == 0x0A && ib_cmd_line_buffer[ib_cmd_line_len-2] == 0x0D)
                {
                    result = ib_cmd_line_len;
                    ib_cmd_status = 0;
                }

            }
             break;
             
        case 5:
                if ( cc == 0x6F)
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 6;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;
                }
            break;
            
        case 6:
                if ( cc == 0x3A)
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 4;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;
                }
            break;
        case 7:
                if ( cc == 'i')
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 8;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;        
                }
            break;
            
        case 8:
                if ( cc == 'n')
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 9;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;
                }
            break;
        case 9:
                if ( cc == 'k')
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 10;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;
                }
            break;
        case 10:
                if ( cc == '_')
                {
                    *ib_cmd_ptr++ = cc;
                    ib_cmd_line_len++;
                    ib_cmd_status = 4;
                    ib_hex_need_len = 0;
                }
                else
                {
                    ib_cmd_status = 0;
                }
            break;
         default:
            break;
     }  /* end switch ( at_cmd_prep_state ) */

    return result;
}

static uint32 zdt_ib_uart_callback(uint32 event)
{
    uint8 tmp_buf[128+1];
    uint32 cnt_old, cnt,i;
       
    switch (event)
    {
        case EVENT_DATA_TO_READ:
            {      
                cnt_old = UART_GetRxFifoCnt(ZDT_IB_UART_PORT);        
                cnt = UART_ReadData(ZDT_IB_UART_PORT, tmp_buf, cnt_old);
#if 0 //def FILE_LOG_SUPPORT
                Trace_Log_AT_Print("[IB] RCV len=%d",cnt);
                Trace_Need_Hex(TRUE);
                Trace_Log_AT_Data(tmp_buf, cnt);
                Trace_Need_Hex(FALSE);
                Trace_Log_AT_Data("\r\n", 2);
#endif
                ZDT_IBTrace("Read: len=%d",cnt);
                ZDT_IBTraceExt(tmp_buf,cnt);
                ZDT_IBTraceU8(tmp_buf,cnt);
                for(i = 0; i < cnt; i++)
                {
                    if(zdt_ib_uart_process_chars(tmp_buf[i]) > 0)
                    {
                        zdt_ib_uart_process_at( ib_cmd_line_buffer, ib_cmd_line_len);
                    }
                }

                break;
            }
        
        case EVENT_WRITE_COMPLETE:  {
                        
            cnt = UART_GetTxFifoCnt(ZDT_IB_UART_PORT);
            cnt = QueueDelete(&Output_Q, tmp_buf, cnt);
            UART_WriteData(ZDT_IB_UART_PORT, tmp_buf, cnt);
#if 0 //def FILE_LOG_SUPPORT
                Trace_Log_AT_Print("[IB] Send len=%d",cnt);
                Trace_Need_Hex(TRUE);
                Trace_Log_AT_Data(tmp_buf, cnt);
                Trace_Need_Hex(FALSE);
                Trace_Log_AT_Data("\r\n", 2);
#endif
            if (0 == cnt) UART_Tx_Int_Enable(ZDT_IB_UART_PORT, FALSE);
            
            break;    
        }
        
        case EVENT_INIT_COMPLETE:
            break;
        
        case EVENT_SHUTDOWN_COMPLETE:
            break;
        
        default:
            break;            
    }
    return UART_SUCCESS;
}

uint32 zdt_ib_uart_open(void)
{
    int ret; 
    
    UART_INIT_PARA_T    uart1_st;
    if(ib_uart_is_on)
    {
        return 0;
    }
    uart1_st.tx_watermark  = ZDT_IB_TX_WATER_MARK; // 0~127B
    uart1_st.rx_watermark  = ZDT_IB_RX_WATER_MARK; // 0~127B
    uart1_st.baud_rate     = BAUD_115200;
    uart1_st.parity        = FALSE; 
    uart1_st.parity_enable = PARITY_DISABLE;
    uart1_st.byte_size     = EIGHT_BITS;
    uart1_st.stop_bits     = ONE_STOP_BIT;
    uart1_st.flow_control  = NO_FLOW_CONTROL;
    uart1_st.ds_wakeup_en  = DS_WAKEUP_ENABLE;  //DS_WAKEUP_DISABLE
    uart1_st.ds_wakeup_type = DS_WAKEUP_BY_RXD;
    //assume u1rxd/u1txd have selected in pinmap    
    ret = UART_Initilize(ZDT_IB_UART_PORT, &uart1_st, zdt_ib_uart_callback);  //ZDT_IB_UART_PORT 1
    QueueClean(&Output_Q);
    UART_Tx_Int_Enable(ZDT_IB_UART_PORT, FALSE);
    
    if(ret == UART_SUCCESS)
    {
        ZDT_IBTrace("IB UART Open OK");
        ib_uart_is_on = 1;
    }
    else
    {
        ZDT_IBTrace("IB UART Open ERR");
    }
    return ret;
}

uint32 zdt_ib_uart_close(void)
{
    QueueClean(&Output_Q);
    UART_Close(ZDT_IB_UART_PORT);
    ib_uart_is_on = 0;
    ZDT_IBTrace("IB UART Close");
    return UART_SUCCESS;
}

void ZDTIB_HandleRcv(uint8 * rcv_buf, uint32 rcv_len)
{
    if(is_ib_ok == FALSE)
    {
        is_ib_ok = TRUE;
        ZDT_IB_CheckHW_End();
    }
#ifdef FILE_LOG_SUPPORT
    Trace_Log_AT_Print("[IB] RCV len=%d,%s",rcv_len,rcv_buf);
    Trace_Need_Hex(TRUE);
    Trace_Log_AT_Data(rcv_buf, rcv_len);
    Trace_Need_Hex(FALSE);
    Trace_Log_AT_Data("\r\n", 2);
#endif
    
    ZDT_IBTrace("RCV: len=%d",rcv_len);
    ZDT_IBTraceExt(rcv_buf,rcv_len);
    ZDT_IBTraceU8(rcv_buf,rcv_len);
    BEACON_API_ReceiveAT(rcv_buf,rcv_len);
    return;
}

void ZDTIB_HandleSend(uint8 * send_buf, uint32 send_len)
{
#ifdef FILE_LOG_SUPPORT
    Trace_Log_AT_Print("[IB] Send len=%d,%s",send_len,send_buf);
    Trace_Need_Hex(TRUE);
    Trace_Log_AT_Data(send_buf, send_len);
    Trace_Need_Hex(FALSE);
    Trace_Log_AT_Data("\r\n", 2);
#endif
    ZDT_IBTrace("SEND: len=%d",send_len);
    ZDT_IBTraceExt(send_buf,send_len);
    ZDT_IBTraceU8(send_buf,send_len);
    if(BEACON_API_IsSleep())
    {
        zdt_ib_uart_write("AT\r\n",4);
        SCI_Sleep(100);
    }
    zdt_ib_uart_write(send_buf,send_len);
    SCI_Sleep(100);
}

BOOLEAN  ZDTIB_HandlePosOK(void)
{
    ZDT_IBTrace("ZDTIB_HandlePosOK g_is_ib_hw_on =%d",g_is_ib_hw_on);
    return TRUE;
}

BOOLEAN  ZDTIB_HandlePosFail(void)
{
    ZDT_IBTrace("ZDTIB_HandlePosFail g_is_ib_hw_on =%d",g_is_ib_hw_on);
    #if 0
    if(g_ib_mutexPtr != NULL)
    {
        SCI_GetMutex(g_ib_mutexPtr, SCI_WAIT_FOREVER);
    }
    
     if(g_ib_mutexPtr != NULL)
    {
        SCI_PutMutex(g_ib_mutexPtr);
    }
     #endif
    return TRUE;
}

BOOLEAN  ZDTIB_HandlePowerOn(void)
{
    ZDT_IBTrace("ZDTIB Power ON g_is_ib_hw_on =%d",g_is_ib_hw_on);
    if(s_is_ib_hw_check)
    {
        s_ib_hw_check_laststatus = TRUE;
    }
    if(g_is_ib_hw_on == FALSE)
    {
        g_ib_hw_pwon_time = SCI_GetCurrentTime();
        #if 0
        if(g_ib_mutexPtr != NULL)
        {
            SCI_GetMutex(g_ib_mutexPtr, SCI_WAIT_FOREVER);
        }
         if(g_ib_mutexPtr != NULL)
        {
            SCI_PutMutex(g_ib_mutexPtr);
        }
         #endif
        LDO_TurnOnLDO (LDO_LDO_SDIO);
        zdt_ib_uart_open();
        SCI_Sleep(600);
        ZDT_IB_AT_Send_Wakeup();
        SCI_Sleep(100);
        ZDT_IB_AT_Send_GetVer();
        g_is_ib_hw_on = TRUE;
    }
    return TRUE;
}

BOOLEAN  ZDTIB_HandlePowerOff(void)
{
    uint8 send_buf[10] = {0xAB,0};
    ZDT_IBTrace("ZDTIB Power OFF");
    if(s_is_ib_hw_check)
    {
        s_ib_hw_check_laststatus = FALSE;
        return TRUE;
    }
    if(g_is_ib_hw_on)
    {
        if(g_is_ib_not_off)
        {
            return TRUE;
        }
        LDO_TurnOffLDO (LDO_LDO_SDIO);
        zdt_ib_uart_close();
        g_is_ib_hw_on = FALSE;
    }
    return TRUE;
}

LOCAL void ZDTIB_CHK_TimerCallback(uint32 param)
{	    
    ZDT_IB_CheckHW_End();
    return ;
}

BOOLEAN beacon_hw_is_ok(void) 
{
    return is_ib_ok;
}

BOOLEAN ZDTIB_HandleHwCheckStart(void)
{    
    ZDT_IBTrace("ZDTIB HWCheckStart ib_hw_is_ok = %d",is_ib_ok);
    if(is_ib_ok == FALSE && s_is_ib_hw_check == 0)
    {
        s_ib_hw_check_laststatus = g_is_ib_hw_on;
        ZDTIB_HandlePowerOn();
        s_is_ib_hw_check = 1;
        if(s_zdt_ib_check_timer == NULL)
        {
            s_zdt_ib_check_timer = SCI_CreateTimer("IBCHK_Timer", ZDTIB_CHK_TimerCallback,
                                        1, 2000, SCI_AUTO_ACTIVATE);
        }
        else
        {
            SCI_ChangeTimer(s_zdt_ib_check_timer, ZDTIB_CHK_TimerCallback, 2000);
            SCI_ActiveTimer(s_zdt_ib_check_timer);
        }
    }
    return TRUE;
}

BOOLEAN ZDTIB_HandleHwCheckEnd(void)
{
    ZDT_IBTrace("ZDTIB HWCheckEnd %d",beacon_hw_is_ok());
    if(s_is_ib_hw_check)
    {
        s_is_ib_hw_check = 0;
        if(s_ib_hw_check_laststatus == FALSE)
        {
            ZDTIB_HandlePowerOff();
        }
    }
    if(g_is_ib_hw_on)
    {
        ZDT_IB_AT_Send_Wakeup();
    }
    return TRUE;
}

void ZDTIB_TaskEntry(uint32 argc, void *argv)
{	
    zdtib_taskid=SCI_IdentifyThread();
    if(g_ib_mutexPtr == NULL) {
        g_ib_mutexPtr = SCI_CreateMutex("ib-mutex", SCI_INHERIT);
    }

    ZDT_IBTrace("ZDTIB_TaskEntry");
    
    while(1)
    {
    	xSignalHeaderRec*   sig_ptr = 0; 
    	sig_ptr = SCI_GetSignal(zdtib_taskid);
    	switch(sig_ptr->SignalCode)
    	{
                case SIG_ZDT_IB_HW_CHECK_START:
                    {
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        ZDTIB_HandleHwCheckStart();
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_IB_HW_CHECK_END:
                    {
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        ZDTIB_HandleHwCheckEnd();
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_IB_RCV:
                    {
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        ZDTIB_HandleRcv(atc_sig->str,atc_sig->len);
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_IB_RCVHEX:
                    {
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                    
                case SIG_ZDT_IB_SEND:
                    {
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        ZDTIB_HandleSend(atc_sig->str,atc_sig->len);
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_IB_POS_IN:
                    {
                        //定位成功
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        ZDTIB_HandlePosOK();
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_IB_POS_OUT:
                    {
                        //定位失败
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        ZDTIB_HandlePosFail();
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_IB_HW_PWON:
                    {
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        ZDTIB_HandlePowerOn();
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                case SIG_ZDT_IB_HW_PWOFF:
                    {
                        ZDT_IB_SIG_T *  atc_sig = (ZDT_IB_SIG_T*)sig_ptr;
                        
                        ZDTIB_HandlePowerOff();
                        
                        if(atc_sig->len >  0 && atc_sig->str!= NULL)
                        {
                            SCI_FREE(atc_sig->str);
                        }
                    }
                    break;
                    
                default:
                    break;

    	}
    	SCI_FREE(sig_ptr);
    }
}

void ZDTIB_TaskCreate(void)
{
	if(zdtib_taskid == SCI_INVALID_BLOCK_ID)
	{
		zdtib_taskid = SCI_CreateThread("ZDTIB_THREAD",
			"ZDTIB_QUEUE",
			ZDTIB_TaskEntry,
			NULL,
			NULL,
			1024*20,
			200,
			80,	// 比MMI 74要高
			SCI_PREEMPT,
			SCI_AUTO_START
			);
        
		ZDT_IBTrace("ZDTIB_TaskCreate, zdtib_taskid=0x%x", zdtib_taskid);
	}
}

void ZDTIB_TaskClose(void)
{

	if(SCI_INVALID_BLOCK_ID != zdtib_taskid)
	{
		SCI_TerminateThread(zdtib_taskid);
		SCI_DeleteThread(zdtib_taskid);
		zdtib_taskid = SCI_INVALID_BLOCK_ID;       
	}
    
	ZDT_IBTrace("ZDTIB_TaskClose");
}

BLOCK_ID ZDTIB_TaskGetID(void)
{
	return zdtib_taskid;
}

BOOLEAN  APP_SendSigTo_IB(ZDT_IB_TASK_SIG_E sig_id, const char * data_ptr,uint32 data_len)
{
    uint8 * pstr = NULL;
    ZDT_IB_SIG_T * psig = PNULL;
    BLOCK_ID cur_taskID=ZDTIB_TaskGetID();
    
    if(SCI_INVALID_BLOCK_ID == cur_taskID)
    {
        ZDT_IBTrace("APP_SendSigTo_IB Err Task sig_id=0x%x,data_len=%d",sig_id,data_len);
        return FALSE;
    }
    
    if(data_len != 0 && data_ptr != NULL)
    {
        pstr = SCI_ALLOCAZ(data_len+1);//free it in ib task
        if (pstr == PNULL)
        {
                SCI_PASSERT(0, ("APP_SendTo_ATC Alloc  %ld FAIL",data_len));
                return FALSE;
        }
        SCI_MEMCPY(pstr,data_ptr,data_len);
    }
    
    //send signal to AT to write uart
    SCI_CREATE_SIGNAL((xSignalHeaderRec*)psig,sig_id,sizeof(ZDT_IB_SIG_T), SCI_IdentifyThread());
    psig->len = data_len;
    psig->str = pstr;
		
    SCI_SEND_SIGNAL((xSignalHeaderRec*)psig, cur_taskID);
    return TRUE;
}

BOOLEAN ZDT_IB_Hw_IsOn(void)
{
    return g_is_ib_hw_on;
}

void ZDT_IB_Set_PowerNoOff(BOOLEAN no_off)
{
    g_is_ib_not_off = no_off;
}

int ZDT_IB_Init(void)
{    
    ZDTIB_TaskCreate();
    return 0;
}

int ZDT_IB_PowerOn(void)
{    
    APP_SendSigTo_IB(SIG_ZDT_IB_HW_PWON,PNULL,0);
    return 0;
}

int ZDT_IB_PowerOff(void)
{    
    APP_SendSigTo_IB(SIG_ZDT_IB_HW_PWOFF,PNULL,0);
    return 0;
}

int ZDT_IB_UartSend(uint8 * send_buf, uint32 send_len)
{    
    APP_SendSigTo_IB(SIG_ZDT_IB_SEND,send_buf,send_len);
    return 0;
}

int ZDT_IB_CheckHW_Start(void)
{    
    APP_SendSigTo_IB(SIG_ZDT_IB_HW_CHECK_START,PNULL,0);
    return 0;
}

int ZDT_IB_CheckHW_End(void)
{    
    APP_SendSigTo_IB(SIG_ZDT_IB_HW_CHECK_END,PNULL,0);
    return 0;
}


int ZDT_IB_AT_Send_GetVer(void)
{    
    //激活唤醒蓝牙模块
    ZDT_IB_UartSend("AT+CIVER?\r\n",11);
    return 0;
}

int ZDT_IB_AT_Send_Wakeup(void)
{    
    //激活唤醒蓝牙模块
    ZDT_IB_UartSend("AT\r\n",4);
    return 0;
}

int ZDT_IB_AT_Send_Connect(char * dev_mac_str)
{    
    //连接跳绳
    uint8 send_buf[100] = {0};
    uint16 send_len = 0;
    sprintf(send_buf,"AT+CON_ROPE=%s\r\n",dev_mac_str);
    send_len = strlen(send_buf);
    ZDT_IB_UartSend(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_DisConnect(void)
{    
    //断开连接
    uint8 send_buf[100] = {0};
    uint16 send_len = 0;
    sprintf(send_buf,"AT+DISCONN=A\r\n");
    send_len = strlen(send_buf);
    ZDT_IB_UartSend(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_EntryPassMode(void)
{    
    //进入透传模式
    uint8 send_buf[100] = {0};
    uint16 send_len = 0;
    sprintf(send_buf,"AT++++\r\n");
    send_len = strlen(send_buf);
    ZDT_IB_UartSend(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_ExitPassMode(void)
{    
    //退出透传模式
    uint8 send_buf[100] = {0};
    uint16 send_len = 0;
    sprintf(send_buf,"+++");
    send_len = strlen(send_buf);
    ZDT_IB_UartSend(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_Scan(void)
{    
    //蓝牙扫描
    uint8 send_buf[100] = {0};
    uint16 send_len = 0;
    sprintf(send_buf,"AT+SCAN=3\r\n");
    send_len = strlen(send_buf);
    ZDT_IB_UartSend(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_LinkData(uint8 * pData, uint16 DataLen)
{    
    uint8 send_buf[100] = {0};
    uint16 send_len = 0;
    sprintf(send_buf,"AT+SEND=0,%d\r\n",DataLen);
    send_len = strlen(send_buf);
    ZDT_IB_UartSend(send_buf,send_len);
    SCI_Sleep(100);
    ZDT_IB_UartSend(pData,DataLen);
    return 0;
}

uint8 ZDT_IB_AT_GetCRC(uint8 * pData, uint16 DataLen)
{
    uint8 crc = 0;
    uint16 i = 0;
    for(i = 0; i < DataLen; i++)
    {
        crc += pData[i];
    }
    return crc;
}

int ZDT_IB_AT_Send_41(void)
{    
    uint8 send_buf[18] = {0x41,0x00,0};
    uint16 send_len = 16;
    send_buf[15] = ZDT_IB_AT_GetCRC(send_buf,15);
    ZDT_IB_AT_Send_LinkData(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_57(void)
{    
    uint8 send_buf[18] = {0x57,0x00,0};
    uint16 send_len = 16;
    send_buf[15] = ZDT_IB_AT_GetCRC(send_buf,15);
    ZDT_IB_AT_Send_LinkData(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_PWD(void)
{    
    //下发密码
    uint8 send_buf[20] = {0xa5, 0x0a, 0x01, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00};
    uint16 send_len = 10;
    send_buf[9] = ZDT_IB_AT_GetCRC(send_buf,9);
    ZDT_IBTrace("ZDT_IB_AT_Send_PWD");
    ZDT_IB_AT_Send_LinkData(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_GetNum(void)
{    
    //查询记录
    uint8 send_buf[20] = {0xa5, 0x05, 0x07, 0x00, 0x00};
    uint16 send_len = 5;
    send_buf[4] = ZDT_IB_AT_GetCRC(send_buf,4);
    ZDT_IBTrace("ZDT_IB_AT_Send_GetNum");
    ZDT_IB_AT_Send_LinkData(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_DateTime(void)
{    
    uint8 send_buf[50] = {0xa5, 0x0b, 0x06, 0x00};
    uint16 send_len = 3;
    uint8 time_str[30] = {0};
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    ZDT_IBTrace("ZDT_IB_AT_Send_DateTime year=%d",date.year);
    if(date.year >= 2023)
    {
        send_buf[send_len++] = date.year >> 8;
        send_buf[send_len++] = date.year & 0x00FF;
        send_buf[send_len++] = date.mon;
        send_buf[send_len++] = date.mday;
        send_buf[send_len++] = time.hour;
        send_buf[send_len++] = time.min;
        send_buf[send_len++] = time.sec;
        send_buf[send_len] = ZDT_IB_AT_GetCRC(send_buf,send_len);
        send_len++;
        ZDT_IB_AT_Send_LinkData(send_buf,send_len);
        return 1;
    }
    return -1;
}

int ZDT_IB_AT_Send_58(void)
{    
    uint8 send_buf[18] = {0x58,0x00,0};
    uint16 send_len = 16;
    send_buf[15] = ZDT_IB_AT_GetCRC(send_buf,15);
    ZDT_IB_AT_Send_LinkData(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_LinkCheck(void)
{
    uint8 send_buf[100] = {0};
    uint16 send_len = 0;
    sprintf(send_buf,"AT+LINK?\r\n");
    send_len = strlen(send_buf);
    ZDT_IB_UartSend(send_buf,send_len);
    return 0;
}

int ZDT_IB_AT_Send_01(void)
{    
    uint8 send_buf[18] = {0x01,0x00,0};
    uint16 send_len = 16;
    uint8 time_str[30] = {0};
    SCI_TIME_T          time;
    SCI_DATE_T      date;
    TM_GetSysDate(&date);
    TM_GetSysTime(&time);
    if(date.year >= 2023)
    {
        sprintf(time_str,"%02d%02d%02d%02d%02d%02d",date.year-2000,date.mon,date.mday,time.hour,time.min,time.sec);
        MMIAPICOM_StrToBcd(1,time_str,send_buf+1);
        send_buf[15] = ZDT_IB_AT_GetCRC(send_buf,15);
        ZDT_IB_AT_Send_LinkData(send_buf,send_len);
    }
    return 0;
}
#endif/*_ZDT_IB_UART_C*/
