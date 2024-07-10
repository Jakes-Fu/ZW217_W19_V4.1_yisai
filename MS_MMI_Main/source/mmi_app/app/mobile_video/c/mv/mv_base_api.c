#define _MV_BASE_API_C_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmi_app_mobile_video_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "sfs.h"
#include "mv_debug.h"
#include "sci_api.h"
#include "mv_base_api.h"
#include "sig_code.h"
#include "tasks_id.h"
#include "mmi_signal.h"
#include "mmiidle_export.h"
#include "mv_timer_api.h"
#include "chng_freq.h"
#include "mmimv_id.h"
#include "mmi_default.h"
#include "_Utils.h"
#include "mv_mm_api.h"
#include "gpio_prod_api.h"
#include "LCD_backlight.h"
#include "mmi_autotest.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

                                                 
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get imei
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
// LOCAL const wchar *getIMEI(void);

LOCAL MMI_BACKLIGHT_STATUS_E g_backlight_status;
LOCAL CM_LCD_LIGHT_VALUE_ENUM g_lcd_light_value;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get disk id
//  Global resource dependence : 
//  Author:
//  Note: //strDiskPath = "/D/directory path/"
/*****************************************************************************/
void MV_GetDiskID(_CONST _WCHAR * strDiskPath, _WCHAR *newDiskPath)
{
    //SCI_TRACE_LOW:"[MV] MV_GetDiskID %x, %x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_BASE_API_63_112_2_18_2_39_9_7,(uint8*)"dd",strDiskPath[0],strDiskPath[1]);
    newDiskPath[0] = strDiskPath[1]; //取盘符
    newDiskPath[1] = 0;
}

/*****************************************************************************/
//  Description : CM_Concurrent
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_Concurrent(void)
{
    if(MMK_IsFocusWin(MMIMV_MAIN_WIN_ID))
    {    
        return _FALSE;        
    }
    else
    {         
         return _TRUE;
    }
    
}

/*****************************************************************************/
//  Description : get imei
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
//LOCAL const wchar *getIMEI(void)
//{
//    BOOLEAN            ret = TRUE;
//    char               temp_str[MN_MAX_IMEI_LENGTH<<1] = {0};
//    MN_IMEI_T          imei_arr = {0}; 
//    wchar              imei_wchar[MN_MAX_IMEI_LENGTH<<1] = {0};
//    
//#ifdef _USE_OLD_L4_API
//    ret = MNNV_GetIMEI(imei_arr);		
//#else
//    ret = MNNV_GetIMEIEx(MMIAPISET_GetActiveSim(), imei_arr);
//#endif
//    
//    if (!ret)
//    {
//        return PNULL;
//    }
//
//    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (uint8 *)imei_arr, MN_MAX_IMEI_LENGTH<<1,(char *)temp_str);  
//    MMIAPICOM_StrToWstr((uint8 *)temp_str,(wchar*)imei_wchar);
//
//    return (wchar *)imei_wchar;
//	
//  
//}
/*****************************************************************************/
//  Description : CM_GetMachineInfo
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_BOOLEAN CM_GetMachineInfo(MachineInfoEnum nIndex, _PVOID pBuffer, _INT nSize)
{    

    uint8 temp_value = 0;
    
    //MV_TRACE_LOW:"[MV] CM_GetMachineInfo pBuffer = %x, nSize = %d, nIndex = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_BASE_API_123_112_2_18_2_39_9_8,(uint8*)"ddd",pBuffer,nSize,nIndex);
    
    if (PNULL == pBuffer)
    {
        return FALSE;
    }

    switch(nIndex)
    {
    case EMachine_Serial:
        //MMIAPICOM_Wstrcpy((wchar *)pBuffer,getIMEI());      
        break;

    case EMachine_SignalStrength:
		//Jone 添加SIM 卡验证, 
		//当SIM 卡存在且有效的时候才给返回有效信号值,而不是单纯的取天线信号量
		if (MMIAPIPHONE_GetSimAvailableNum(PNULL, 0) > 0)
		{
	        temp_value = MMIMAIN_GetRxLevel(MN_DUAL_SYS_1)*PHONE_RX_BAT_LEVEL_SPAN;
		}
		else
		{
			temp_value = 0;
		}
        *((_INT*)pBuffer) = MIN(MAX_SIGNAL_VALUE, temp_value); 
        //MV_TRACE_LOW("[MV] [CM_GetMachineInfo] EMachineInfo_Signal = %d",MMIMAIN_GetRxLevel(MN_DUAL_SYS_1));
        break;
    
    case EMachine_BatteryStrength:  
        temp_value = PHONE_RX_BAT_LEVEL_SPAN * MMIAPIPHONE_GetBatCapacity();
        ((BATTERYINFO*)pBuffer)->m_nValue = MIN(MAX_BATTERY_VALUE, temp_value);
        ((BATTERYINFO*)pBuffer)->m_nStatus = CHGMNG_IsChargeConnect();           
        break;
    
    default:
        break;
    }

	return _TRUE;
}

/*****************************************************************************/
//  Description : CM_Exit
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID CM_Exit(_INT nCode)
{   
    uint32 req_handler = NULL;
    
	if(1 == nCode)
    {
	    //MV_TRACE_LOW:"[MV] [CM_Exit] mobile video exit "
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_BASE_API_174_112_2_18_2_39_9_9,(uint8*)"");

        req_handler = MV_GetFreqHandler();

        if (req_handler != NULL)
        {
#ifndef WIN32
            CHNG_FREQ_RestoreARMClk(req_handler);
            CHNG_FREQ_DeleteReqHandler(req_handler); 
            MV_SetFreqHandler(CHNG_FREQ_REQ_HANDLER_NULL);
#endif
            //MV_TRACE_LOW:"[MV][MV_SetFreqHandler] "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_BASE_API_185_112_2_18_2_39_9_10,(uint8*)"");
        }

        CM_SendSignalToMMI(MVIDEO_EXIT_IND, PNULL);  
        MV_DeleteMemHandle();
    }	
}

/*****************************************************************************/
//  Description : set BackLight Level
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID MV_SetLCDLight(MMI_BACKLIGHT_STATUS_E backlight_status, CM_LCD_LIGHT_VALUE_ENUM lightLevel)
{
	if((backlight_status == g_backlight_status) && (lightLevel == g_lcd_light_value))
		return;
	
	if((MMI_BACKLIGHT_OFF == g_backlight_status) && (MMI_BACKLIGHT_OFF != backlight_status)) 
	{
		// 当点亮屏幕的时候需要Invalidate整个屏幕
        MMITHEME_InitMainLcdStortUpdateRect();
        MMITHEME_StoreUpdateRect(MMITHEME_GetDefaultLcdDev(), MMITHEME_GetFullScreenRect());
		GPIO_SetKeyPadBackLight(TRUE);
	}

	switch(backlight_status)
	{
		case MMI_BACKLIGHT_OFF:
			//关闭背光	
			GPIO_SetLcdBackLight(FALSE);
			GPIO_SetKeyPadBackLight(FALSE);
			//MMI_TraceAutoBackLightStatus(MMI_BACKLIGHT_OFF);
			break;
		case MMI_BACKLIGHT_ON:
			GPIO_SetLcdBackLight(TRUE);
			MMIAPISET_UpdateLCDContrast(GUI_MAIN_LCD_ID, MMIAPISET_GetCurrentContrast());
			//MMI_TraceAutoBackLightStatus(MMI_BACKLIGHT_ON);
			break;
		case MMI_BACKLIGHT_HALF_ON:
			GPIO_SetLcdBackLight(TRUE);
            LCD_SetBackLightBrightness(10 * lightLevel); 
            //MMI_TraceAutoBackLightStatus(MMI_BACKLIGHT_HALF_ON);
			break;
		case MMI_BACKLIGHT_STATUS_MAX:
			
			break;
		default:
			
			break;
	}
	g_backlight_status = backlight_status;
    MMI_TraceAutoBackLightStatus(g_backlight_status);
	g_lcd_light_value = lightLevel;
}

/*****************************************************************************/
//  Description : 当退出的时候点亮LCD
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID MV_TurnOnLCDLight()
{
	if(g_backlight_status != MMI_BACKLIGHT_ON)
	{
		MV_SetLCDLight(MMI_BACKLIGHT_ON, 0);
	}
}
/*****************************************************************************/
//  Description : set BackLight Level
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID CM_SetLCDLight(CM_LCD_LIGHT_VALUE_ENUM lightLevel)
{
	CM_WriteLogs(1, "CM_SetLCDLight()::lightLevel=%d", lightLevel);
	
		switch(lightLevel)
		{
			case WD_LCD_LIGHT_VALUE_0:	
			    MV_SetLCDLight(MMI_BACKLIGHT_OFF, 0);
				break;
			case WD_LCD_LIGHT_VALUE_1:
			case WD_LCD_LIGHT_VALUE_2:
			case WD_LCD_LIGHT_VALUE_3:
			case WD_LCD_LIGHT_VALUE_4:
			case WD_LCD_LIGHT_VALUE_5:
			case WD_LCD_LIGHT_VALUE_6:
			case WD_LCD_LIGHT_VALUE_7:
			case WD_LCD_LIGHT_VALUE_8:
			    MV_SetLCDLight(MMI_BACKLIGHT_HALF_ON, lightLevel);
				break;
			case WD_LCD_LIGHT_VALUE_9:
			    MV_SetLCDLight(MMI_BACKLIGHT_ON, 0);
				break;
		}
	 

	/////////////////////////////////////////////////////////////////////
}

/*****************************************************************************/
//  Description : set allows backlight on/off
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
_VOID CM_HighLight_Always(_BOOLEAN vOpen)
{	
	BOOLEAN allow_turn_off = FALSE;
	
	if(vOpen)
	{
        MMIDEFAULT_TurnOnBackLight(); 
	    allow_turn_off = FALSE;
	}
	else
	{
	    allow_turn_off = TRUE;
	}
	
	//MV_TRACE_LOW:"[CM_HighLight_Always]:this is CM_HighLight_Always \r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_BASE_API_301_112_2_18_2_39_9_11,(uint8*)"");
	MMIDEFAULT_AllowTurnOffBackLight(allow_turn_off);	
 
}


/*****************************************************************************/
//  Description : CM_GetDiskSpace
//  Global resource dependence : 
//  Author:
//  Note: 得到指定路径下存储空间剩余容量（单位KB）。注意是剩余容量
/*****************************************************************************/
_DWORD CM_GetDiskSpace(_CONST _WCHAR * strDiskPath)
{
    uint32                  free_space_low = 0;
    uint32                  free_space_high = 0;
    _WCHAR                  newDiskPath[2]={0};
    _DWORD                  result = 0;
    uint32                  kb_size = 1 << 10;
    uint64                  free_space = 0;

    MV_GetDiskID(strDiskPath,newDiskPath);

    if(!MMIAPIFMM_GetDeviceFreeSpace((wchar *)newDiskPath, (uint16)MMIAPICOM_Wstrlen((wchar *)newDiskPath), &free_space_high, &free_space_low))     
    {
        //SCI_TRACE_LOW:"[MV] CM_GetDiskSpace get error "
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_BASE_API_324_112_2_18_2_39_9_12,(uint8*)"");
        result = 0;
    }
    else if(0 == free_space_high)
    {
        result = free_space_low/kb_size;
    }
    else
    {        
        free_space = free_space_high;
        free_space = (free_space << 32) + free_space_low;
        free_space = free_space/kb_size;        
        result = (uint32)free_space;
    }
    //SCI_TRACE_LOW:"[MV] CM_GetDiskSpace result = %d KB"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MV_BASE_API_338_112_2_18_2_39_9_13,(uint8*)"d", result);
    return result;
}

/*****************************************************************************/
//  Description : to get the screen width                                   
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
_INT CM_GetScreenWidth(_VOID)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    
    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
 //   SCI_TRACE_LOW("[MV] CM_GetScreenWidth lcd_width = %d", lcd_width);

    return lcd_width;
}

/*****************************************************************************/
//  Description : to get screen height                                   
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
_INT CM_GetScreenHeight(_VOID)
{
    uint16          lcd_width = 0;
    uint16          lcd_height = 0;
    
    //get lcd width and height
    GUILCD_GetLogicWidthHeight(GUI_MAIN_LCD_ID,&lcd_width,&lcd_height);
//    SCI_TRACE_LOW("[MV] CM_GetScreenHeight lcd_height = %d", lcd_height);
    
    return lcd_height;
}

/*****************************************************************************/
//  Description : to send message to mmi                                   
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
PUBLIC void CM_SendSignalToMMI(uint16 signal, uint16 data)    
{                                                                              
    MmiAppCopyMsgS *sendSignal = PNULL;                                            
                                                                               
    MmiCreateSignal(signal, sizeof(MmiAppCopyMsgS), (MmiSignalS**)&sendSignal); //创建消息             
    sendSignal->Sender = MV_GetMvTaskID();                                                  
    sendSignal->data = data;  
    MmiSendSignal(P_APP, (MmiSignalS*)sendSignal);                                     
}


/*****************************************************************************/
//  Description : CM_SCI_SLEEP                         
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
_VOID CM_SCI_SLEEP(uint32 millsecs)
{
    SCI_SLEEP(millsecs);

    return;
}

/*****************************************************************************/
//  Description : WriteLogsL                         
//  Global resource dependence : none                                          
//  Author:                                                                    
//  Note:                                                                      
/*****************************************************************************/
_VOID WriteLogsL(_UINT uLevel, _CONST _CHAR* aFmt, ...)
{
    va_list arg = PNULL; 

    va_start(arg, aFmt);   /*lint -esym(628,__va_start)*/
    vprintf(aFmt, arg); 
    va_end(arg); 
}
