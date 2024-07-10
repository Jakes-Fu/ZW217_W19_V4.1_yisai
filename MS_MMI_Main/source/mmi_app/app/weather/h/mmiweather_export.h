/****************************************************************************
** File Name:      mmiweather_export.h                                     *
** Author:         Gaily.Wang                                              *
** Date:           April/04/2011                                           *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:    This file define the external APIs for weather app      *
****************************************************************************/

#ifndef _MMI_WEATHER_EXPORT_H_    
#define _MMI_WEATHER_EXPORT_H_   

#ifdef __cplusplus
extern "C" {
#endif


/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Discription: This fucntion is used to open the bluetooth main menu win  
//  Global resource dependence: None
//  Author: Wenming Feng 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_OpenMainWin(void);

/*****************************************************************************/
//  Discription: MMIAPIWEATHER_InitApp
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_InitApp(void);

/*****************************************************************************/
// 	Description : intial bt module  
//	Global resource dependence : none
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_InitModule(void);


/*****************************************************************************/
//  Discription: MMIAPIWEATHER_ResetFactory
//  Global resource dependence: None
//  Author: Gaily.Wang
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_ResetFactory(void);

#if  (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
/*****************************************************************************/
//    Description : �ָ�Ĭ������
//    Global resource dependence : 
//    Author:   xiaoming.ren
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIWEATHER_ResetDefaultSetting(void);

/****************************************************************************/
//  Description : �õ�X-Y�¶��ַ���
//  Global resource dependence : 
//  Author:juan.wu
//  Note: ���ⲿʹ��
/****************************************************************************/
PUBLIC void MMIAPIWEATHER_GetTemperatureRangeString(int32 low, 
                                                    int32 high, 
                                                    MMI_STRING_T * string_ptr,
                                                    BOOLEAN is_need_uint_str);

/****************************************************************************/
//  Description : �õ��¶��ִ�(eg.33oc)
//  Global resource dependence : 
//  Author:juan.wu
//  Note: ���ⲿʹ��
/****************************************************************************/
PUBLIC void MMIAPIWEATHER_GetTemperatureString(int32 temp, 
                                                MMI_STRING_T * string_ptr);


#ifdef PDA_WEATHER_TIME_SE
/*****************************************************************************/
//    Description :Handle time update msg
//    Global resource dependence : 
//    Author:  Gaily.Wang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWEATHER_HandleTimeUpdate(void);
#endif
#endif
#if defined BAR_PHONE_WIDGET || defined MMI_WIDGET_WEATHER2
PUBLIC void MMIAPIWEATHER_OpenAddCityWin(void);
#endif
/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  ���º궨������ʵ�ֶ���ӿڸ���ǰ��ļ���
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#if  (defined MMIWIDGET_SUPPORT) || (defined MMI_GRID_IDLE_SUPPORT)
#define MMIWIDGET_Weather_ResetDefaultSetting				     MMIAPIWEATHER_ResetDefaultSetting
#ifdef PDA_WEATHER_TIME_SE
#define MMIWIDGET_Weather_HandleTimeUpdate				         MMIAPIWEATHER_HandleTimeUpdate
#endif
#endif

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif
#endif

