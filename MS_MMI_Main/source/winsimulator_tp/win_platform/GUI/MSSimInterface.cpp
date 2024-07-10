#include "stdafx.h"
#include "MSSimInterface.h"
#include "MSDevDlg.h"

HANDLE MSSIM_GetSIOLockEvent()
{
    extern HANDLE hSIOLock;
    return hSIOLock;
}

UINT MSSIM_sio_flush_tx(void *src, UINT size)
{
    DWORD thread_id = ((CMSDevDlg *)AfxGetMainWnd())->GetSIOThreadID();
    
    if ( thread_id != 0 )
    {
	    PostThreadMessage( thread_id, SIO_SEND_DATA, (WPARAM)size, (LPARAM)src );
    }

    return 0;
}

UINT MSSIM_sio_at_flush_tx(void *src, UINT size)
{
	if (((CMSDevDlg *)AfxGetMainWnd())->GetAtCommandDlgPtr()->GetSafeHwnd() == NULL)
	{
		return 0;
	}

	LPBYTE pBuf = NULL;
	if(size != 0)
	{
		pBuf = new BYTE[size];
		memcpy(pBuf,src,size);
	}

    ((CMSDevDlg *)AfxGetMainWnd())->PostMessage(WM_RCV_ATRES, (WPARAM)size, (LPARAM)pBuf);

    return 0;
}

void MSSIM_sio_free(void *buffer_ptr)
{
    free(buffer_ptr);
}

void MSSIM_bridge_power_off(HANDLE hTimer, HANDLE hTimerInt)
{
    ((CMSDevDlg *)AfxGetMainWnd())->OS_TerminateRtos(hTimer, hTimerInt);
}

void MSSIM_Trace(const char *lpszString)
{
    TRACE(lpszString);
    TRACE("\n");
}

WORD MSSIM_KPD_ScanKey()
{
    return ((CMSDevDlg *)AfxGetMainWnd())->GetKeyCode();
}



UINT MSSIM_LCD_GetPanelSize()
{
    return ((CMSDevDlg *)AfxGetMainWnd())->GetLCDPanelSize();
}

WORD * MSSIM_LCD_GetBufferPointer()
{

    return ((CMSDevDlg *)AfxGetMainWnd())->GetLCDBufferPointer();
}

void MSSIM_LCD_SetPixelColor( WORD x, WORD y, WORD color )
{    
    ((CMSDevDlg *)AfxGetMainWnd())->SetLCDPixelColor( x, y, color );
}

WORD MSSIM_LCD_GetPixelColor( WORD x, WORD y )
{
    return ((CMSDevDlg *)AfxGetMainWnd())->GetLCDPixelColor( x, y );
}

void MSSIM_LCD_DrawPixel( WORD x, WORD y, WORD color )
{ 
    ((CMSDevDlg *)AfxGetMainWnd())->DrawLCDPixel(x, y, color);
}

void MSSIM_LCD_InvalidateRect( WORD left, WORD top, WORD right, WORD bottom )
{
    ((CMSDevDlg *)AfxGetMainWnd())->InvalidateLCDRect( left, top, right, bottom );
}

void MSSIM_LCD_Invalidate()
{ 
    ((CMSDevDlg *)AfxGetMainWnd())->InvalidateLCD();
}



UINT MSSIM_SubLCD_GetPanelSize()
{ 
    return ((CMSDevDlg *)AfxGetMainWnd())->GetSubLCDPanelSize();
}

WORD * MSSIM_SubLCD_GetBufferPointer()
{
    return ((CMSDevDlg *)AfxGetMainWnd())->GetSubLCDBufferPointer();
}

void MSSIM_SubLCD_SetPixelColor( WORD x, WORD y, WORD color )
{
    ((CMSDevDlg *)AfxGetMainWnd())->SetSubLCDPixelColor( x, y, color );
}

WORD MSSIM_SubLCD_GetPixelColor( WORD x, WORD y )
{
    return ((CMSDevDlg *)AfxGetMainWnd())->GetSubLCDPixelColor( x, y );
}

void MSSIM_SubLCD_DrawPixel( WORD x, WORD y, WORD color )
{
    ((CMSDevDlg *)AfxGetMainWnd())->DrawSubLCDPixel(x, y, color);
}

void MSSIM_SubLCD_InvalidateRect( WORD left, WORD top, WORD right, WORD bottom )
{
    ((CMSDevDlg *)AfxGetMainWnd())->InvalidateSubLCDRect( left, top, right, bottom );
	
}

void MSSIM_SubLCD_Invalidate()
{
    ((CMSDevDlg *)AfxGetMainWnd())->InvalidateSubLCD();
}

//@ Liu Kai 2004-6-30 CR10261
BYTE MSSIM_GetLCDInfo( BYTE lcd_id,void* lpInfo )
{  
    return ((CMSDevDlg *)AfxGetMainWnd())->GetLCDInfo( lcd_id,lpInfo );
}

BYTE MSSIM_RotateScreen( BYTE type )
{
    return (BYTE)((CMSDevDlg *)AfxGetMainWnd())->SendMessage( WM_ROTATE_SCREEN,type );
}

//apple 2009-04-14
void MSSIM_SetLcdBackLight( BOOL bOn )
{
	((CMSDevDlg *)AfxGetMainWnd())->SetLcdBackLight( bOn );	
}

//apple 2010-02-25 »¬¸ÇÄ£Äâ
void MSSIM_SetSlideState( BOOL bOn )
{
//	((CMSDevDlg *)AfxGetMainWnd())->SetSlideState( bOn );	
	((CMSDevDlg *)AfxGetMainWnd())->PostMessage( WM_SLIDE_PHONE,(WPARAM)bOn );
}

//apple 2010-04-01 flip on/off simulate
void MSSIM_SetFlipState( BOOL bOn )
{
	((CMSDevDlg *)AfxGetMainWnd())->SetFlipState( bOn );	
}

//hongliang 2010-05-21 Start/Stop DC View
void MSSIM_SetDCViewState( BOOL bOn ,WORD x,WORD y,WORD w,WORD h)
{
	DWORD offset = (DWORD)((((DWORD)x)<<16) | y);
	DWORD wh     = (DWORD)((((DWORD)w)<<16) | h);

	((CMSDevDlg *)AfxGetMainWnd())->PostMessage( bOn? WM_CAMERA_OPEN:WM_CAMERA_CLOSE,(WPARAM)offset, (LPARAM)wh );

}

//hongliang 2010-05-25 do snap shot
void MSSIM_DoSnapshot( void ** ppBuf, DWORD *pLength )
{
	((CMSDevDlg *)AfxGetMainWnd())->DoSnapshot(ppBuf,pLength);
}

//hongliang 2010-05-25 do record
void MSSIM_DoRecord( void ** ppBuf, DWORD *pLength,BOOL bEnd )
{
	((CMSDevDlg *)AfxGetMainWnd())->DoRecord(ppBuf,pLength,bEnd);
}

BYTE MSSIM_GetLayerInfo(BYTE lcd_id, void **lpInfo, DWORD *pMaxNum)
{
	return (BYTE)((CMSDevDlg *)AfxGetMainWnd())->GetLayerInfo(lcd_id,lpInfo,pMaxNum);
}

void MSSIM_SetVibrate(BOOL bOn)
{
	((CMSDevDlg *)AfxGetMainWnd())->SetVibrate(bOn);
}

BOOL MSSIM_IsTraceOn()
{
	return ((CMSDevDlg *)AfxGetMainWnd())->IsTraceOn();
}

void MSSIM_InvalidateLayerInfo()
{
	((CMSDevDlg *)AfxGetMainWnd())->InvalidateLayerInfo();
}

void MSSIM_SendData2Com(char *data)
{
	((CMSDevDlg *)AfxGetMainWnd())->sendData(data);
}