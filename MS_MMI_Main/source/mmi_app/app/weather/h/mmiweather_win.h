/*************************************************************************
** File Name:      mmiweather_win.h                                      *
** Author:         Gaily.Wang                                            *
** Date:           2011/04/18                                            *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.   *
** Description:    This file is used to describe weather app             *
**************************************************************************/

#ifndef MMIWEATHER_WIN_H_
#define MMIWEATHER_WIN_H_


/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
typedef enum
{
    MMIWEATHER_PUBWIN_NONE = 0,
    MMIWEATHER_PUBWIN_SUCCESS,
    MMIWEATHER_PUBWIN_FAIL,
    MMIWEATHER_PUBWIN_WAITING,
    MMIWEATHER_PUBWIN_QUERY,
    MMIWEATHER_PUBWIN_MAX
 } MMIWEATHER_PUBWIN_TYPE_E;

typedef enum
{
    WEATHER_WIN_CITYLIST = 0,
    WEATHER_WIN_DETAILS,
    WEATHER_WIN_TRENDS,

} WEATHER_WIN_PAGE;

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         EXTERN VARIABLE DECLARE                          *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         GLOBAL FUNCTION DECLARE                          *
**--------------------------------------------------------------------------*/


/*****************************************************************************/
// 	Description : Open weather main window
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/

PUBLIC void MMIWEATHER_OpenMainWin(void);

/*****************************************************************************/
// 	Description : Open Pub Win
//	Global resource dependence : 
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_OpenPubWin(MMIWEATHER_PUBWIN_TYPE_E win_type, MMI_TEXT_ID_T text_id, PROCESSMSG_FUNC handle_func);

/*****************************************************************************/
// 	Description : 更新天气，或者删减城市，重画三个page
//	Global resource dependence : none
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_UpdateWinAllPage(void);

/*****************************************************************************/
// 	Description : Open Pub Win
//	Global resource dependence : 
//  Author: Gaily.Wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWEATHER_ProcAddCityStatus(MMIWEATHER_ADD_CITY_STATE_E state);

/****************************************************************************/
//  Description : 得到未来天气数据，因为未来天气不是数组，所以需要提取
//  Global resource dependence : 
//  Author:violent.wei
//  Note: 
/****************************************************************************/
PUBLIC void MMIWEATHER_GetFCId(
                        MMIWEATHER_FC_CONDITIONS_T* fc_conditions_ptr, 
                        uint8 index, 
                        MMIWEATHER_WEEK_E* week_id_ptr,
                        MMIWEATHER_CONDITION_E* condition_id_ptr,
                        int32* low_ptr,
                        int32* high_ptr,
                        wchar** date_wstr_ptr
                        );

/*****************************************************************************/
//  Description : MMIWEATHER_OpenCountryList
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_OpenCountryList(void);

/*****************************************************************************/
//  Description : MMIWEATHER_FreeCountryInfoMem
//  Global resource dependence : 
//  Author: juan.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIWEATHER_FreeCountryInfoMem(void);

#endif //MMIWEATHER_WIN_H_
