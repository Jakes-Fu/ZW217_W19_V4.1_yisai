/*****************************************************************************
** File Name:      tp_func.c                                               *
** Author:                                                                   *
** Date:           11/17/2005                                                   *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to handle input method                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2005       wancan.you     Creat
** 01/05/2010    vine.yuan      Add the definition of "TP_GetAdaptMode"
******************************************************************************/


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#if defined TOUCH_PANEL_SUPPORT
//#include "sci_types.h"
#include "mn_type.h"
#include "tp_nv.h"
//#include "tp_text.h"
#include "tp_export.h"
#include "tp_internal.h"
#include "mmi_modu_main.h"
#include "mmi_nv.h"

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
#ifndef _WIN32
extern BOOLEAN RM_GetCalibrationMode(void);     
extern BOOLEAN RM_GetCalibrationPostMode(void); 
#endif
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 是否时第一次开机
//	Global resource dependence : 
//  Author: Robert
//	Note:
/*****************************************************************************/
BOOLEAN TP_IsPowerOnFirst(void)
{

    BOOLEAN                 is_first = TRUE;
    MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
   
#ifndef _WIN32
     if (RM_GetCalibrationMode() || RM_GetCalibrationPostMode())
    {          
        return FALSE;
    }
#endif
    //read power on password from
    MMINV_READ(TPNV_SET_POWER_ON_IS_FIRST,&is_first, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        is_first = TRUE;
        MMINV_WRITE(TPNV_SET_POWER_ON_IS_FIRST,&is_first);
    }

    return (is_first);
}

/*****************************************************************************/
// 	Description : 清除已经设置的坐标value
//	Global resource dependence : 
//  Author:guopeng.zhang
//	Note: 
/*****************************************************************************/
void MMIAPITP_Coordinate_ResetValue(void)
{

    BOOLEAN          is_first = TRUE;

   

    MMINV_WRITE(TPNV_SET_POWER_ON_IS_FIRST, &is_first);
}

/*****************************************************************************/
// 	Description : 设置是否时第一次开机
//	Global resource dependence : 
//  Author: Robert
//	Note:
/*****************************************************************************/
void  TP_SetPowerOnFirst(
                                           BOOLEAN                 is_first   // 是否时第一次开机
                                       )
{
   MMINV_WRITE(TPNV_SET_POWER_ON_IS_FIRST, &is_first);   
}


#ifdef TP_SELFADAPTIVE_ENABLE
/*
* Here, "TP_MODE_MAX" means that mode hasn't 
* been initialized. vine.yuan 2010.6.3
*/
LOCAL TP_MODE_E g_adapt_mode = TP_MODE_MAX;

/*****************************************************************************/
// 	Description : To update the current self-adaptive mode,and save into nv
//	Global resource dependence : 
//  Author: vine.yuan
//	Note: To ensure that g_adapt_mode should be consist with the value of nv
/*****************************************************************************/
void TP_UpdateAdaptMode(TP_MODE_E adapt_mode)
{
	//To update current self-adaptive mode
	g_adapt_mode = adapt_mode;
	//To save the self-adaptive mode into nv
	MMINV_WRITE(TPNV_CAL_SELF_ADAPTIVE_MODE, &adapt_mode);
}

/*****************************************************************************/
// 	Description : To get the current self-adaptive mode.
//	Global resource dependence : 
//  Author: vine.yuan
//	Note:
/*****************************************************************************/
TP_MODE_E TP_GetCurAdaptMode(void)
{
	/*
	* if mode is "TP_MODE_MAX", this means that
	* it should be initialized first. vine.yuan 2010.6.3
	*/
	if(TP_MODE_MAX == g_adapt_mode)
	{
		//To initialize TP self-adaptive mode
		TP_InitAdaptMode();
	}
	return g_adapt_mode;
}

/*****************************************************************************/
// 	Description : To initialize the self-adaptive mode for TP.
//				If fail, return FALSE, or TRUE
//	Global resource dependence : 
//  Author: vine.yuan
//	Note:
/*****************************************************************************/
BOOLEAN TP_InitAdaptMode(void)
{
	MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;

	//To read the self-adaptive mode from nv
	MMINV_READ(TPNV_CAL_SELF_ADAPTIVE_MODE,  &g_adapt_mode,  return_value);

	if (MN_RETURN_SUCCESS != return_value)
	{
		/*
		* If fail, self-adaptive mode is TP_MODE_NORMAL 
		* as default. @vine.yuan 2010.1.5
		*/
		g_adapt_mode = TP_MODE_NORMAL;
		return FALSE;
	}
	return TRUE;
}
#endif //TP_SELFADAPTIVE_ENABLE
#endif //#if defined TOUCH_PANEL_SUPPORT


