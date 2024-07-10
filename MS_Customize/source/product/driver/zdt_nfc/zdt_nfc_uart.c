#ifndef _ZDT_NFC_UART_C
#define _ZDT_NFC_UART_C

#include "zdt_nfc.h"

//ZDT_NFC_SUPPORT


#define ZDT_NFC_TX_WATER_MARK	    8
#define ZDT_NFC_RX_WATER_MARK   	48

#define ZDT_NFC_UART_PORT   1
#define ZDT_NFC_SEND_BUFFER_SIZE 2048        //default com send/receive buffer size

//¸ñÊ½  s/r  means   set/respone 
// 68              01               01                01            checksum  00 01        channel          10
//head(1byte)  cmd(1byte)   r/w(1byte)    s/r(1byte)    2byte     len(2byte)   data(nbyte)   tail(1byte) 

#define NFC_AT_START                                            0x68                                             
#define NFC_AT_END                                            0x10
#define NFC_AT_HEAD_LEN 8
#define NFC_AT_MAX_LEN 512

typedef  enum
{
    NFC_AT_PROTOCOL_NULL = 0,
    NFC_AT_PROTOCOL_CMD,    
    NFC_AT_PROTOCOL_MAX
}NFC_AT_PROTOCOL_E;


//Cycle Queue struct
typedef struct {
    uint8 *buf;
    uint32 size;        //queue volume
    uint32 head;
    uint32 tail;
    unsigned empty: 1;
    unsigned full:  1;
    unsigned overflow:  1;  //queue input after full, overwrite occur
} NFCCycleQueue;

__align (4) static uint8 s_nfc_send_buf[ZDT_NFC_SEND_BUFFER_SIZE];

//com send queue
static NFCCycleQueue Output_Q = {
    s_nfc_send_buf,
    ZDT_NFC_SEND_BUFFER_SIZE,
    0,
    0,
    1,              //empty
    0,         
    0,
};


static NFC_AT_PROTOCOL_E s_cur_at_protocol = NFC_AT_PROTOCOL_NULL;
static uint8 nfc_cmd_line_buffer[NFC_AT_MAX_LEN];     /*  buffer for AT cmd line  */
static uint16 nfc_cmd_line_len = 0;
static uint16 nfc_data_need_len = 0;
static uint8 *nfc_cmd_ptr;                     /* next loc in buffer       */
static uint8 nfc_cmd_status = 0;
static uint8 * nfc_at_rx_data_buf = NULL;
static uint8 nfc_uart_is_on = 0;

/*****************************************************************************/
//  FUNCTION:     QueueClean
//  Description:    clean cycle queue
//  INPUT:      pointer to a NFCCycleQueue
//  OUTPUT:     void
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:       
/*****************************************************************************/
static void QueueClean(NFCCycleQueue *Q_ptr)
{
    Q_ptr->head = Q_ptr->tail = 0;
    Q_ptr->empty = 1;
    Q_ptr->full = 0;
    Q_ptr->overflow = 0;    
}

/*****************************************************************************/
//  FUNCTION:     ZDT_NFC_QueueInsert(NFCCycleQueue *Q_ptr, uint8 *data, uint32 len)
//  Description:    insert a string in cycle queue
//  INPUT:          Q_ptr--pointer to a NFCCycleQueue, uint8 *data--string pointer, uint32 len--string length
//  OUTPUT:         >=0 actual insert number, <0 error
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:        when queue is full, new data wil overwrite olds.
/*****************************************************************************/
static int QueueInsert(NFCCycleQueue *Q_ptr, uint8 *data, uint32 len)
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
//  FUNCTION:     QueueDelete(NFCCycleQueue *Q_ptr, uint8 *data, uint32 len)
//  Description:    delete a string from cycle queue
//  INPUT:          Q_ptr--pointer to a NFCCycleQueue, uint8 *data--string pointer, uint32 len--string length
//  OUTPUT:         >=0 actual delete number, <0 error
//  Author:         David.Jia
//  date:           2007.6.23
//	Note:       
/*****************************************************************************/
static int QueueDelete(NFCCycleQueue *Q_ptr, uint8 *data, uint32 len)
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


static uint8 zdt_nfc_uart_process_cmd
(
  uint8 *cmd_line_ptr ,
  uint16  cmd_line_len
)
{
    NFC_TRACE("zdt_nfc_uart_process_cmd len=%d",cmd_line_len);
    APP_SendSigTo_NFC(SIG_ZDT_NFC_RCV,cmd_line_ptr,cmd_line_len);
    return 1;
}

static void zdt_nfc_uart_process_chars(uint8 cc)
{
    switch ( nfc_cmd_status )
    {
        case 0:
            if (cc == NFC_AT_START)
            {
                nfc_cmd_ptr = &nfc_cmd_line_buffer[0];
                *nfc_cmd_ptr++ = cc;
                nfc_cmd_line_len = 1;
                nfc_data_need_len = 0;
                s_cur_at_protocol = NFC_AT_PROTOCOL_CMD;
                nfc_cmd_status = 1;
            }
            break;
            
        case 1:
            {
                if ( nfc_cmd_ptr >= (nfc_cmd_line_buffer + NFC_AT_MAX_LEN - 2) )
                {
                    nfc_cmd_status = 0;
                    nfc_cmd_ptr[0] = '\0';
                    break;
                }
                /*------------------------------------------------------------
                  If no overflow, fill buffer.  
                ------------------------------------------------------------*/
                else
                {
                    *nfc_cmd_ptr++ = cc;
                    nfc_cmd_line_len++;
                }
                
                if(nfc_cmd_line_len == NFC_AT_HEAD_LEN)
                {                                
                    uint16 data_len = 0;
                    data_len = nfc_cmd_line_buffer[NFC_AT_HEAD_LEN-1];
                    data_len = data_len << 8;
                    data_len += nfc_cmd_line_buffer[NFC_AT_HEAD_LEN-2];
                    
                    nfc_data_need_len = data_len + NFC_AT_HEAD_LEN+1;
                    if ( nfc_data_need_len > NFC_AT_MAX_LEN )
                    {
                        if(nfc_at_rx_data_buf != NULL)
                        {
                            SCI_FREE(nfc_at_rx_data_buf);
                            nfc_at_rx_data_buf = NULL;
                        }
                        nfc_at_rx_data_buf = (uint8 *)SCI_ALLOC_APPZ(nfc_data_need_len+1);
                        if(nfc_at_rx_data_buf == NULL)
                        {
                            NFC_TRACE("NFC RCV CMD ERR SCI_ALLOC_APPZ %d",nfc_data_need_len+1);
                            nfc_cmd_status = 0;
                            nfc_cmd_ptr[0] = '\0';
                        }
                        else
                        {
                            nfc_cmd_status = 2;
                            SCI_MEMCPY(nfc_at_rx_data_buf,nfc_cmd_line_buffer,NFC_AT_HEAD_LEN);
                            nfc_cmd_ptr = &nfc_at_rx_data_buf[NFC_AT_HEAD_LEN];
                        }
                        break;
                    }
                }
                else
                {
                    if(nfc_cmd_line_len >= nfc_data_need_len)
                    {
                        zdt_nfc_uart_process_cmd( nfc_cmd_line_buffer,nfc_cmd_line_len);
                        nfc_cmd_status = 0;
                    }
                }
            }
            break;
            
        case 2:
            {
                if ( nfc_cmd_ptr >= (nfc_at_rx_data_buf + nfc_data_need_len) )
                {
                    nfc_cmd_status = 0;
                    nfc_cmd_ptr[0] = '\0';
                    break;
                }
                /*------------------------------------------------------------
                  If no overflow, fill buffer.  
                ------------------------------------------------------------*/
                else
                {
                    *nfc_cmd_ptr++ = cc;
                    nfc_cmd_line_len++;
                }

                if(nfc_cmd_line_len >= nfc_data_need_len)
                {
                    if(nfc_at_rx_data_buf[nfc_cmd_line_len-1] == NFC_AT_END)
                    {
                        zdt_nfc_uart_process_cmd( nfc_at_rx_data_buf,nfc_cmd_line_len);
                    }
                    else
                    {
                        NFC_TRACE("TCP nfc CMD(%d) ERR End 0x%x",nfc_data_need_len,nfc_at_rx_data_buf[nfc_cmd_line_len-1]);
                        if(nfc_at_rx_data_buf != NULL)
                        {
                            SCI_FREE(nfc_at_rx_data_buf);
                            nfc_at_rx_data_buf = NULL;
                        }
                    }
                    nfc_cmd_status = 0;
                }
            }
            break;
    }  /* end switch ( at_cmd_prep_state ) */

    return;
}

int zdt_nfc_uart_write(uint8 *buf, uint32 len)
{
    int ret;
    if(nfc_uart_is_on == 0)
    {
        return 0;
    }
    UART_Tx_Int_Enable(ZDT_NFC_UART_PORT, FALSE);
    ret = QueueInsert(&Output_Q, buf, len);
    UART_Tx_Int_Enable(ZDT_NFC_UART_PORT, TRUE);
    return ret;
}

/*****************************************************************************/
//  FUNCTION:     nfc_uart1_callback(uint32 event)
//  Description:    callback function for uartcom_drv.
//  Author:         David.Jia
//  date:           2007.6.19
//	Note:           assume there is still data in when call UART_ReadData.
//      return value will large than called UART_GetRxFifoCnt.
/*****************************************************************************/
static uint32 zdt_nfc_uart_callback(uint32 event)
{
    uint8 tmp_buf[128+1];
    uint32 cnt_old, cnt,i;

    //NFC_TRACE("zdtnfc zdt_nfc_uart_callback event=%x",event);	

    switch (event)
    {
        case EVENT_DATA_TO_READ:
            {      
                cnt_old = UART_GetRxFifoCnt(ZDT_NFC_UART_PORT);        
                cnt = UART_ReadData(ZDT_NFC_UART_PORT, tmp_buf, cnt_old);
                //ZDT_GPSRcvTraceCR(tmp_buf,cnt);
                //ZDT_HexTraceU8(tmp_buf,cnt);
                for(i = 0; i < cnt; i++)
                {
                    zdt_nfc_uart_process_chars(tmp_buf[i]);
                }
                break;
            }
        
        case EVENT_WRITE_COMPLETE:  {
                        
            cnt = UART_GetTxFifoCnt(ZDT_NFC_UART_PORT);
            cnt = QueueDelete(&Output_Q, tmp_buf, cnt);
            UART_WriteData(ZDT_NFC_UART_PORT, tmp_buf, cnt);
            
            if (0 == cnt) UART_Tx_Int_Enable(ZDT_NFC_UART_PORT, FALSE);
            
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

uint32 zdt_nfc_uart_open(void)
{
    int ret; 
    
    UART_INIT_PARA_T    uart1_st;
    NFC_TRACE("zdtnfc zdt_nfc_uart_open");

    if(nfc_uart_is_on)
    {
        NFC_TRACE("zdtnfc zdt_nfc_uart_open nfc_uart_is_on  then return");    
        return 0;
    }
    DPSLP_NFC_UART_EnableApbSleep(0);
    DPSLP_UART_EnableApbSleep(ZDT_NFC_UART_PORT, 0);
    uart1_st.tx_watermark  = ZDT_NFC_TX_WATER_MARK; // 0~127B
    uart1_st.rx_watermark  = ZDT_NFC_RX_WATER_MARK; // 0~127B
    uart1_st.baud_rate     = BAUD_115200;
    uart1_st.parity        = FALSE; 
    uart1_st.parity_enable = PARITY_DISABLE;
    uart1_st.byte_size     = EIGHT_BITS;
    uart1_st.stop_bits     = ONE_STOP_BIT;
    uart1_st.flow_control  = NO_FLOW_CONTROL;
    uart1_st.ds_wakeup_en  = DS_WAKEUP_DISABLE;
    uart1_st.ds_wakeup_type = DS_WAKEUP_BY_RXD;
      
    //assume u1rxd/u1txd have selected in pinmap    
    ret = UART_Initilize(ZDT_NFC_UART_PORT, &uart1_st, zdt_nfc_uart_callback);  //ZDT_NFC_UART_PORT 1
    QueueClean(&Output_Q);
    UART_Tx_Int_Enable(ZDT_NFC_UART_PORT, FALSE);
    
    if(ret == UART_SUCCESS)
    {
        NFC_TRACE("NFC UART Open OK");
        nfc_uart_is_on = 1;
    }
    else
    {
        NFC_TRACE("NFC UART Open ERR");
    }
    return ret;
}

uint32 zdt_nfc_uart_close(void)
{
    QueueClean(&Output_Q);
    UART_Close(ZDT_NFC_UART_PORT);
    DPSLP_NFC_UART_EnableApbSleep(1);
    DPSLP_UART_EnableApbSleep(ZDT_NFC_UART_PORT, 1);
    nfc_uart_is_on = 0;
    NFC_TRACE("NFC UART Close");
    return UART_SUCCESS;
}
#endif/*_ZDT_NFC_UART_C*/
