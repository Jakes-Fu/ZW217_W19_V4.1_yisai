#ifndef __MS_BRIDGE__
#define __MS_BRIDGE__

#include <windows.h>
#include "MSK_Type.h"
#include "sci_types.h"
#include "sci_api.h"
#include "dal_lcd.h"

#ifndef MSCODE_MOD
#ifdef __cplusplus
#include "tb_drv.h"
#include "tb_dal.h"
#endif
#endif

// SIO related MACROS
#define     ESCAPE_BYTE	        0x7D
#define     FLAG_BYTE		    0x7E
#define     COMPLEMENT_BYTE	    0x20
#define     MED_FRAME           0x0
#define     END_FRAME           0x1
#define     MAX_PACKET_SIZE     1000    //512
#define     MED_FRAME           0x0
#define     END_FRAME           0x1
#define     SIO_NO_END_FLAG     0

// Called by the MSSim GUI code via MSKernel
// [
void BL_RegisterMSSimFunList(const MSK_MSSIM_FUN_LIST *pFunList);
void BL_StartRTOS();
void BL_StopRTOS();
BOOL BL_IsPowerOn();
int  BL_GetKeyCodeByName(const char * lpszKeyName);
void BL_SendKeycodeToRTOS(WORD wKeyCode, UINT msg);
UINT BL_SendMsgToRTOS(BYTE *pBuffer, int nLength);
void BL_Free(void *pBuffer);
// ]

// Called by the MS Control Panel
// [
void BL_CPSetCallbackFunToPS(int (*fun_ptr)(xSignalHeader sig));
// ]

// Called by the low-level code
// [
#ifdef __cplusplus
extern "C"
{
#endif

uint32 bridge_queue_send(BLOCK_ID src_ID, VOID *source_ptr, uint16 size, ULONG wait_option);
uint16 bridge_encode_msg(uint8 * dest, uint8 * src, uint16 size, uint32 flag);
void   bridge_power_off (HANDLE hTimer, HANDLE hTimerInt);

DWORD WINAPI RestartRTOSThread( LPVOID lpParam );

// @Xueliang.Wang added : 2003-02-26
int SIO_SendTracePacket(void *src, int len);
int SIO_SendLogPacket  (uint32 *head_ptr, int head_len, uint32 *body_ptr, int body_len);
int SIO_SendPacket	   (void * src, int size);
int SIO_AT_SendPacket(void * src, int size);
void SIO_SendData2Com(char *data);
uint32 sio_flush_tx(unsigned char *src, uint32 size);
uint32 sio_at_flush_tx(unsigned char *src, uint32 size);

void sio_free(unsigned char *buffer_ptr);


uint32 KPD_Create(BLOCK_ID svr);
uint16 KPD_ScanKey();

void SCI_Trace(const char * x_format, ...);
void SCI_TraceEx(uint32 trc_class,const char *x_format,const uint8* arg_type, ...);
uint16 bridge_decode_msg(uint8 **dest, uint8 *src);

typedef void (* ATC_CALLBACK) (uint32);
void SIO_ATC_ReadCmdLine(uint8 * data_ptr, uint length, uint * read_length_ptr);
void SIO_ATC_WriteCmdRes(uint8 * res_data_ptr, uint length);
void SIO_ATC_SetCmdLineTerminateChar(uint8 terminate_char1, uint8 terminate_char2);
void SIO_ATC_SetCmdLineBackSpaceChar(uint8 back_space_char);

/* @jun.liu cr5120 2003/10/10 */
void SIO_ATC_SetDataMode( BOOLEAN mode );
/* cr5120 */

void SIO_ATC_SetCallback(ATC_CALLBACK callback);
void SIO_ATC_SetEcho(BOOLEAN IsEcho);

BOOLEAN SIO_SetBaudRate(uint32 port, uint32 baud_rate);
uint32 SIO_GetBaudRate(uint32 port);

 ERR_LCD_E LCD_SetUVBuffer (    LCD_ID_E lcd_id,    uint32 blk_num,    uint32 *buf_ptr);
 void DCAMERA_StartIspRecord(void);


 void DCAMERA_StopIspRecord(void);

 void DCAMERA_GetPclk(uint32 *pclk);

#ifdef __cplusplus
}
#endif
// ]

#endif // __MS_BRIDGE__
