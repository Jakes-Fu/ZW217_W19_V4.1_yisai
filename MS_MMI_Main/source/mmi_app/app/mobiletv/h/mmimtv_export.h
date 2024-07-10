/****************************************************************************
** File Name:      mmimtv_export.h                                          *
** Author:         xin.li                                                   *
** Date:           2008-12                                                  *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the mtv application.       *
****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 12/2008        xin.li           Create
****************************************************************************/

#ifndef _MMIMTV_EXPORT_H
#define _MMIMTV_EXPORT_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"


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
#define MMIMTV_SNAPSHOT_JPGENCODE_SIZE  (80 * 1024)
        
/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMIMTV_NEWSMS = 0,
    MMIMTV_NEWMMS,
    MMIMTV_STATUS_UPDATE,
    MMIMTV_STATUS_MAX,
} MMIMTV_STATUS_TYPE_E;

/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : MTV app init
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_Init(void);

/*****************************************************************************/
// 	Description : MTV module init
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_InitModule(void);

/*****************************************************************************/
// 	Description : open MTV player
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_OpenMTVPlayer(void);

/*****************************************************************************/
// 	Description : whether mtv player is opened
//	Global resource dependence :
//  Author:xin.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsMTVOpen(void);

/*****************************************************************************/
// 	Description : whether mtv is running
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsMTVRunning(void);

/*****************************************************************************/
// 	Description : whether mtv is playing 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsPlaying(void);

/*****************************************************************************/
// 	Description : exit mobiel tv
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_Exit(void);

/*****************************************************************************/
// 	Description : close v-chip first when exit, called by CC when vt come in
//	Global resource dependence : none
//  Author: 
//	Note: 来视频电话时的快速退出
/*****************************************************************************/
PUBLIC void MMIAPIMTV_VTExit(void);

/*****************************************************************************/
// 	Description : 设置延时关机开关
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_SetDelayPowerOff(BOOLEAN is_delay);

/*****************************************************************************/
// 	Description : reset factory setting
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_SettingInfoSetFactory(void);

/*****************************************************************************/
// 	Description : is have unread Emergency Broadcast msg
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIMTV_IsHaveTobeReadEBMsg(void);

/*****************************************************************************/
// 	Description : get tv signal level value 
//	Global resource dependence : none
//  Author: xin.li
//	Note: level 0-5
//        只有V Chip工作时才能获得TV信号，否则返回FALSE
/*****************************************************************************/
PUBLIC BOOLEAN  MMIAPIMTV_GetTVSignalLevel(uint8 *signal_value);

/*****************************************************************************/
// 	Description : get signal icon id for status bar
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMIAPIMTV_GetTVStatusImageID(uint32 signal);

/*****************************************************************************/
// 	Description : open Emergency Broadcast msg list
//	Global resource dependence :
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_OpenEBList(void);

/*****************************************************************************/
// 	Description : MBBMS pre-exit callback function
//	Global resource dependence :
//  Author: xin.li
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMTV_PreExitCallback(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif


#endif  /* _MMIMTV_EXPORT_H*/


