/******************************************************************************
 ** File Name:      msk_type.h                                                *
 ** Author:         Liu Jun                                                   *
 ** Date:           25/02/2003                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This header file defines the common structures and        *
 **                 interface functions of MSKernel.                          *
 ******************************************************************************

 ******************************************************************************
 **                         Edit History                                      *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                                   *
 ** 25/03/2003      Liu Jun     Create.                                       *
 ** 24/09/2003      Liu Jun     Move the following functions from msk_api_cp  *
 **                             to this file. Thus this file can be treated   *
 **                             as an independant common file.                *
 **                             [                                             *
 **                                 MSK_ThreadContextSave()                   *
 **                                 MSK_ThreadContextRestore()                *
 **                             ]                                             *
 ******************************************************************************/

#ifndef __MSK_TYPE__
#define __MSK_TYPE__

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/**---------------------------------------------------------------------------*
 **                         DLL Import/Export Macro                           *
 **---------------------------------------------------------------------------*/
#ifdef MSKERNEL_EXPORTS
    #define MSKERNEL_API __declspec(dllexport)
#else
    #define MSKERNEL_API __declspec(dllimport)
#endif

/**---------------------------------------------------------------------------*
 **                         GUI Interface Functions                           *
 **---------------------------------------------------------------------------*/

typedef struct MSK_MSSIM_FUN_LIST_tag
{
    HANDLE (*fp_MSSIM_GetSIOLockEvent)();
    UINT   (*fp_MSSIM_sio_flush_tx)(void *, UINT);
    void   (*fp_MSSIM_sio_free)(void *);
    void   (*fp_MSSIM_bridge_power_off)(HANDLE, HANDLE);
    void   (*fp_MSSIM_Trace)(const char *);
    WORD   (*fp_MSSIM_KPD_ScanKey)();

    UINT   (*fp_MSSIM_LCD_GetPanelSize)();
    WORD * (*fp_MSSIM_LCD_GetBufferPointer)();
    void   (*fp_MSSIM_LCD_SetPixelColor)( WORD, WORD, WORD );
    WORD   (*fp_MSSIM_LCD_GetPixelColor)( WORD, WORD );
    void   (*fp_MSSIM_LCD_DrawPixel)( WORD, WORD, WORD );
    void   (*fp_MSSIM_LCD_InvalidateRect)( WORD, WORD, WORD, WORD );
    void   (*fp_MSSIM_LCD_Invalidate)();
    
    UINT   (*fp_MSSIM_SubLCD_GetPanelSize)();
    WORD * (*fp_MSSIM_SubLCD_GetBufferPointer)();
    void   (*fp_MSSIM_SubLCD_SetPixelColor)( WORD, WORD, WORD );
    WORD   (*fp_MSSIM_SubLCD_GetPixelColor)( WORD, WORD );
    void   (*fp_MSSIM_SubLCD_DrawPixel)( WORD, WORD, WORD );
    void   (*fp_MSSIM_SubLCD_InvalidateRect)( WORD, WORD, WORD, WORD );
    void   (*fp_MSSIM_SubLCD_Invalidate)();

    //@ Liu Kai 2004-7-1 CR10261
    BYTE   (*fp_MSSIM_LCD_GetInfo)( BYTE,void* );

	// Used to rotate screen
	BYTE   (*fp_MSSIM_Rotate_Screen)( BYTE );

	//apple 2009-04-14
	void   (*fp_MSSIM_SetLcdBackLight)( BOOL );

	//apple 2010-02-25 »¬¸ÇÄ£Äâ
	void   (*fp_MSSIM_SetSlideState)( BOOL );
	
	//hongliang 2010-02-25 flip on/off simulate
	void   (*fp_MSSIM_SetFlipState)( BOOL );
	
	//hongliang 2010-02-25 SetDCViewState
	//hongliang 2010-07-27 SetDCViewState add parameters
	void   (*fp_MSSIM_SetDCViewState)( BOOL,WORD,WORD,WORD,WORD);
	
	//hongliang 2010-05-25 do snap shot
	void   (*fp_MSSIM_DoSnapShot)(void**,DWORD*);
	
	//hongliang 2010-05-25 do record
	void   (*fp_MSSIM_DoRecord)(void**,DWORD*,BOOL);
    
    UINT   (*fp_MSSIM_sio_at_flush_tx)(void *, UINT);

	//hongliang 2010-05-25 do record
	BYTE   (*fp_MSSIM_GetLayerInfo)(BYTE,void**,DWORD*);	

	void   (*fp_MSSIM_SetVibrate)(BOOL);

	BOOL   (*fp_MSSIM_IsTraceOn)();

	void   (*fp_MSSIM_InvalidateLayerInfo)();
	void   (*fp_MSSIM_SendData2Com)(char *data);
}
MSK_MSSIM_FUN_LIST;

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Function:    MSK_ThreadContextSave
// Author:      Liu Jun
// Purpose:     Save the context of current thread
// Calls:       SCI_THREAD_CONTEXT_SAVE
// Called by:   The MS Control Panel, SIM:called_function.c
// Remarks:     1. Use with MSK_ThreadContextRestore() in pairs
//              2. Call MSK_ThreadContextSave() before invoking RTOS functions
//              3. Call MSK_ThreadContextRestore() after invoking RTOS functions
//-----------------------------------------------------------------------------
MSKERNEL_API void 
MSK_ThreadContextSave();

//-----------------------------------------------------------------------------
// Function:    MSK_ThreadContextRestore
// Author:      Liu Jun
// Purpose:     Restore the context of current thread
// Calls:       SCI_THREAD_CONTEXT_RESTORE
// Called by:   The MS Control Panel, SIM:called_function.c
// Remarks:     1. Use with in MSK_ThreadContextRestore pairs
//              2. Call MSK_ThreadContextSave() before invoking RTOS functions
//              3. Call MSK_ThreadContextRestore() after invoking RTOS functions
//-----------------------------------------------------------------------------
MSKERNEL_API void 
MSK_ThreadContextRestore();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif