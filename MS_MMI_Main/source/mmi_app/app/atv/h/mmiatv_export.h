/******************************************************************************
**  File Name:      mmiatv_export.h                                           *
**  Author:         xin.li                                                    *
**  Date:           2010/05                                                   *
**  Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
**  Description:     This file defines the function about atv                 *
*******************************************************************************
**  Edit History                                                              *
** -------------------------------------------------------------------------- *
**  DATE          NAME             DESCRIPTION                                *
**  05/2010       xin.li           Create.                                    *
******************************************************************************/
#ifndef _MMIATV_EXPORT_H
#define _MMIATV_EXPORT_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_appmsg.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
////ref callback msg  0x3120 - 0x312f
#ifdef ATV_SUPPORT
typedef enum
{
    MMIATV_CALLBACK_MSG_START = MSG_APP_ATV_CALLBACK_MSG_START,
    MMIATV_CALLBACK_MSG_REPORT,
	MMIATV_RECORD_END,
	MMIATV_CHN_CNF,
    //MMIATV_CALLBACK_MSG_PARAM_CTRL,
    MMIATV_CALLBACK_MSG_END
} MMIATV_CALLBACK_MSG_E;
#endif

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : ATV app init
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIATV_Init(void);

/*****************************************************************************/
// 	Description : ATV module init
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIATV_InitModule(void);

/*****************************************************************************/
// 	Description : open ATV player
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIATV_OpenATV(void);

/*****************************************************************************/
// 	Description : exit ATV
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIATV_Exit(void);

/*****************************************************************************/
// 	Description : whether atv is open
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIATV_IsATVOpen(void);

/*****************************************************************************/
// 	Description : get atv signal level value 
//	Global resource dependence : none
//  Author: xin.li
//	Note: level 0-5
//        ATV开启时获取TV信号强度，否则返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIATV_GetTVSignalLevel(uint8 *signal_level);

/*****************************************************************************/
// 	Description : get signal level image_id for status bar
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIATV_GetTVStatusImageID(uint8 signal_level);

/*****************************************************************************/
// 	Description : atv restore factory setting
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIATV_RestoreFactorySetting(void);

/*****************************************************************************/
// 	Description : clean user data
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIATV_ClearUserData(void);

/*****************************************************************************/
// 	Description : MMIAPIATV_IsRecording
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIATV_IsRecording(void);

/*****************************************************************************/
// 	Description : MMIAPIATV_GetSensorMSG
//	Global resource dependence : none
//  Author: chunyou.zhu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIATV_HandleSensorMSG(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
#endif  /* _MMIATV_EXPORT_H*/


