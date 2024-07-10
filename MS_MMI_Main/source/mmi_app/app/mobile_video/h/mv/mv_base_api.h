#ifndef  _MV_BASE_API_H
#define  _MV_BASE_API_H

#include "_Types.h"
#include "_utils.h"
#include "mv_debug.h"

/*
typedef enum
{
    CM_BACKLIGHT_OFF = 0,
    CM_BACKLIGHT_HALF_ON,
    CM_BACKLIGHT_ON,
    CM_BACKLIGHT_STATUS_MAX
}CM_BACKLIGHT_STATUS;
*/
typedef enum
{	
	WD_LCD_LIGHT_VALUE_0,	
	WD_LCD_LIGHT_VALUE_1,	
	WD_LCD_LIGHT_VALUE_2,	
	WD_LCD_LIGHT_VALUE_3,	
	WD_LCD_LIGHT_VALUE_4,	
	WD_LCD_LIGHT_VALUE_5,	
	WD_LCD_LIGHT_VALUE_6,	
	WD_LCD_LIGHT_VALUE_7,	
	WD_LCD_LIGHT_VALUE_8,	
	WD_LCD_LIGHT_VALUE_9,
}CM_LCD_LIGHT_VALUE_ENUM;


#ifndef WIN32
	#ifdef PLAYER_DEBUG
		extern _VOID _WriteLogs(_UINT uLevel, _CONST _CHAR* aFmt, ...);
		#define CM_WriteLogs (_WriteLogs)
	#else
	    #define CM_WriteLogs 
    #endif
#else
#define CM_WriteLogs(x, _format_string)
#endif

_BOOLEAN GetMachineInfo(MachineInfoEnum nIndex,_PVOID pBuffer, _INT nSize);

_VOID CM_Exit(_INT nCode);

_VOID CM_HighLight_Always(_BOOLEAN vOpen);

/*****************************************************************************/
//  Description : 当退出的时候点亮LCD
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID MV_TurnOnLCDLight();

/*****************************************************************************/
//  Description : set BackLight Level
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID CM_SetLCDLight(CM_LCD_LIGHT_VALUE_ENUM lightLevel);

_DWORD CM_GetDiskSpace(_CONST _WCHAR * strDiskPath);

_BOOLEAN  CM_MobileTV_Init(void);

_BOOLEAN  CM_MobileTV_Terminate(void);
/*****************************************************************************/
//  Description : CM_SCI_SLEEP                         
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
_VOID CM_SCI_SLEEP(uint32 millsecs);

/*****************************************************************************/
//  Description : to get the screen width                         
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
_INT CM_GetScreenWidth(_VOID);

/*****************************************************************************/
//  Description : to get the screen height                         
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
_INT CM_GetScreenHeight(_VOID);

/*****************************************************************************/
//  Description : to send message to mmi                                   
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
PUBLIC void CM_SendSignalToMMI(uint16 signal, uint16 data) ;
#endif