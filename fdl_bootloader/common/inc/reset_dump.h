/******************************************************************************
 ** File Name:      reset_dump.h                                                *
 ** Author:         yang.li3                                                  *
 ** DATE:           10/11/2022                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contain API of reset dump.                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE                NAME             DESCRIPTION                          *
 ** 10/11/2022          yang.li3         Create.                              *
 ******************************************************************************/

#ifndef _RESET_DUMP_H__
#define _RESET_DUMP_H__

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "global_macros.h"
#include "sys_ctrl_6121e.h"
#include "serial.h"
#include "uart_reg_uws6121e.h"
#include "mem_cfg.h"
#include "mem_dump.h"
#include "cmddef.h"
#include "idle_lps.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern   "C"
{
#endif

#define ENABLE_RESETDUMP_DEBIG_LOG
#ifdef ENABLE_RESETDUMP_DEBIG_LOG
#define RESETDUMP_LOG rprintf
#else
#define RESETDUMP_LOG(...)
#endif

#define  RESET_DUMP_UNIT             (1024)
#define  RESET_DUMP_WAIT_TIME        15           //second
#define  RESET_DUMP_UART_REGBASE     0x51700000   //UART1
#define  hwp_idleLps ((HWP_IDLE_LPS_T *)REG_ACCESS_ADDRESS(REG_IDLE_LPS_BASE))

//define word endian swap
#define WORD_ENDIAN_SWAP(word) (((word & 0xFF)<<8) |((word & 0xFF00) >>8))

//define dword endian swap
#define DWORD_ENDIAN_SWAP(DWord) \
    (((DWord & 0x0FF00)<< 8)|((DWord & 0x0FF)<<24) |\
     ((DWord & 0xFF000000)>>24)|((DWord & 0xFF0000)>>8))

/*
#define REG_UART1_BASE (0x51700000)
#define REG_UART2_BASE (0x51400000)
#define REG_UART3_BASE (0x51401000)
#define REG_UART4_BASE (0x04400000)
#define REG_UART5_BASE (0x04401000)
#define REG_UART6_BASE (0x04402000)
*/

typedef enum
{
  CHANNEL_NO_RESET,                //RESET
  CHANNEL_NO_POWEROFF,             //POWEROFF
  CHANNEL_USB,                     //USB
  CHANNEL_UART,                    //UART
  CHANNEL_MAX,
}DumpChannel;

extern struct FDL_ChannelHandler *ResetDumpUsbChannelGet(void);


/*########################### UART ###########################*/

PUBLIC int RD_UartPutChar (const unsigned char ch);
PUBLIC int RD_UartGetSingleChar(void);
PUBLIC uint8 RD_UartPutUint8(uint8 data);
PUBLIC void RD_UartPutUint32(uint32 data);
PUBLIC uint32 RD_UartPutUint8Chars(const int8* dataptr, int32 len);
PUBLIC void RD_UartPrintHeaderInfo(void);
PUBLIC void RD_UartUpdateDumpHeader(uint32 seq_num, uint16 len);
PUBLIC uint32 RD_UartDumpMemSection(uint32 data, uint32 len);
PUBLIC uint32 RD_UartDumpRegisterGroup(void);
PUBLIC uint32  ResetUartDump2Logel(void);
PUBLIC uint32  ResetUartDumpBin(void*data, uint32 len);


/*########################### USB ###########################*/

PUBLIC int RD_UsbPutChar (const unsigned char ch);
PUBLIC uint8 RD_UsbPutUint8(uint8 data);
PUBLIC void RD_UsbPutUint32(uint32 data);
PUBLIC uint32 RD_UsbPutUint8Chars(const int8* dataptr, int32 len);
PUBLIC void RD_UsbPrintHeaderInfo(void);
PUBLIC void RD_UsbUpdateDumpHeader(uint32 seq_num, uint16 len);
PUBLIC uint32 RD_UsbDumpMemSection(uint32 data, uint32 len);
PUBLIC uint32 RD_UsbDumpRegisterGroup(void);
PUBLIC uint32  ResetUsbDump2Logel(void);


/*########################### COMMON ###########################*/

PUBLIC uint32 RD_GetCur32kNum(void);
PUBLIC uint32 RD_GetCur32kNum2Second(void);
PUBLIC void RD_DelaySecond(uint32 second);
PUBLIC void RD_ResetMCUSoft(void);
PUBLIC void RD_PowerOff(void);
PUBLIC uint32 After7sResetActionType(void);
PUBLIC void After7sResetActionStart(DumpChannel channel);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


#endif//_RESET_DUMP_H__
