/******************************************************************************
 ** File Name:      modem_log.h                                                     *
 ** Author:         Guangqiao.she                                              *
 ** DATE:           28/11/2018                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 04/02/2002     Richard.Yang     Create.                                   *
 ** 05/13/2002     Richard.Yang     Add sio_send_packet, sio_send_log_packet  *
 ** 05/30/2003     Eric.zhou        Add AT through channel server associated  *
 ** functions.                                                                    *
 ** 08/25/2003     Zhemin.Lin     Add MIX_MODE for COM_USER port, according MS00004213 *
 ** 11/03/2010    fei.zhang     Add BT USB Log function        *
 ******************************************************************************/

#ifndef _MODEM_LOG_H_
#define _MODEM_LOG_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sio.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

#define SIO_MODEM_LOG_USB_TX_DISABLE 0
#define SIO_MODEM_LOG_USB_TX_ENABLE  1

#define SIO_MODEM_LOG_SDC_TX_DISABLE 0
#define SIO_MODEM_LOG_SDC_TX_ENABLE  1

#define MODEM_LOG_BUFF_OK 0x4F4B

PUBLIC void SIO_CreateModemLogThread (void);
PUBLIC void SIO_ModemLogUsbSendCallBack(void);
PUBLIC void SIO_ModemLogReset(uint8 status);
PUBLIC void SIO_SendModemLogNotify(void);
PUBLIC int SIO_SendModemTrace(uint32 type);
PUBLIC void SIO_CloseDskComNotifyModem(void);
PUBLIC void SIO_OpenDskComNotifyModem(void);
PUBLIC void SIO_CloseUsbComNotifyModem(void);
PUBLIC void SIO_OpenUsbComNotifyModem(void);
PUBLIC void SIO_SetModemTracePort(uint8 port);
PUBLIC uint8 SIO_ModemTracePortNotify(void);
PUBLIC uint8 SIO_GetModemTracePort(void);
PUBLIC void SIO_MDLogSendPutEvt();
PUBLIC uint16 GetModemLogCpPhyPort(void);

#ifdef CARD_LOG
//TURE: set to puase
//FALSE:not set
PUBLIC void SIO_SetCardLogPauseFlag(BOOLEAN flag);
PUBLIC BOOLEAN SIO_GetCardLogPauseFlag(void);
#endif

typedef struct T_TRA_MODEM_LOG_Tag
{
  uint32 head;  // log Buff base address;
  uint32 length; // log buff总长度2^n，单位:BYTES。
  uint32 rdIndex;  // Log buff Read offset;
  uint32 wrIndex;  // Log buff write offset;
  uint32 curWrIndex; //Log buff current write offset
  uint16 port;	 //0:CP UART 2：close(不输出) 4：IPC+UART(AP UART) 5:IPC+USB(AP USB) 6:IPC+FLASH(AP FLASH)
  uint16 status;  //buff available: 0K(0x4F4B)
  uint32 overflowIndex; //write index of protect area
  uint32 remain_send_len;      //remain the send len
  uint32 cur_send_len;  //current the send len;
  uint32 protect_len;   //log buffer protect len;
  uint32 cacheWrIndex;   // Log cache buff current write offset
  uint16 cacheFlushFlag; // Log flush Flag
  uint8 resered1;
  uint8  smpfirstlog_flag;
  uint32 smp_payloadNum_Cnt;
  uint32 smp_payloadLen_Cnt;
  uint32 smpframe_WrIndex;
  uint32 reserved[1];
}T_TRA_MODEM_LOG;

typedef enum
{
    SIO_MODEM_PORT_NULL,
    SIO_MODEM_UART,
    SIO_MODEM_AP_DSK,
    SIO_MODEM_AP_USB,
    SIO_MODEM_CLOSE,
} SIO_MODEM_PORT_E;


extern uint8  v_sio_modem_log_tx_status;
extern SCI_EVENT_GROUP_PTR modem_cp_log_evt;
extern uint8  v_sio_modem_log_sdc_tx_status;
extern T_TRA_MODEM_LOG * p_modem_cp_log_buff_info;

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif//_MODEM_LOG_H_

