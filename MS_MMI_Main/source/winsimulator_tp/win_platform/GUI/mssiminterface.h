#ifndef __MSSIM_INTERFACE__
#define __MSSIM_INTERFACE__


HANDLE MSSIM_GetSIOLockEvent();
UINT   MSSIM_sio_flush_tx( void *src, UINT size );
void   MSSIM_sio_free( void *buffer_ptr );
void   MSSIM_bridge_power_off( HANDLE hTimer, HANDLE hTimerInt );
void   MSSIM_Trace( const char *lpszString );
WORD   MSSIM_KPD_ScanKey();


UINT   MSSIM_LCD_GetPanelSize();
WORD * MSSIM_LCD_GetBufferPointer();
void   MSSIM_LCD_SetPixelColor( WORD x, WORD y, WORD color );
WORD   MSSIM_LCD_GetPixelColor( WORD x, WORD y );
void   MSSIM_LCD_DrawPixel( WORD x, WORD y, WORD color );
void   MSSIM_LCD_InvalidateRect( WORD left, WORD top, WORD right, WORD bottom );
void   MSSIM_LCD_Invalidate();


UINT   MSSIM_SubLCD_GetPanelSize();
WORD * MSSIM_SubLCD_GetBufferPointer();
void   MSSIM_SubLCD_SetPixelColor( WORD x, WORD y, WORD color );
WORD   MSSIM_SubLCD_GetPixelColor( WORD x, WORD y );
void   MSSIM_SubLCD_DrawPixel( WORD x, WORD y, WORD color );
void   MSSIM_SubLCD_InvalidateRect( WORD left, WORD top, WORD right, WORD bottom );
void   MSSIM_SubLCD_Invalidate();

//@ Liu Kai 2004-6-30 CR10261
BYTE   MSSIM_GetLCDInfo( BYTE lcd_id,void* lpInfo );

BYTE MSSIM_RotateScreen( BYTE type );

//apple 2009-04-14
void MSSIM_SetLcdBackLight( BOOL bOn );

//apple 2010-02-25 »¬¸ÇÄ£Äâ
void MSSIM_SetSlideState( BOOL bOn );

//hongliang 2010-04-01 flip on/off simulate
void MSSIM_SetFlipState( BOOL bOn );

//hongliang 2010-05-21 Start/Stop DC View
void MSSIM_SetDCViewState( BOOL bOn,WORD x,WORD y,WORD w,WORD h );

//hongliang 2010-05-21 do snap shot
void MSSIM_DoSnapshot( void ** ppBuf, DWORD *pLength );

//hongliang 2010-05-21 do record
void MSSIM_DoRecord( void ** ppBuf, DWORD *pLength, BOOL bEnd);

UINT MSSIM_sio_at_flush_tx( void *src, UINT size );

//hongliang 2011-2-27
BYTE MSSIM_GetLayerInfo(BYTE lcd_id, void **lpInfo, DWORD *pMaxNum);

void MSSIM_SetVibrate(BOOL bOn);

BOOL MSSIM_IsTraceOn();

void MSSIM_InvalidateLayerInfo();

void MSSIM_SendData2Com(char *data);
#endif  // __MSSIM_INTERFACE__
