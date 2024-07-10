/*****************************************************************************
** File Name:               mmiapwin_set.h                                   *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifndef _MMIAPWIN_SET_H_
#define _MMIAPWIN_SET_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
#define MMIMP3_MENU_INDEX_0             0
#define MMIMP3_MENU_INDEX_1             1
#define MMIMP3_MENU_INDEX_2             2
#define MMIMP3_MENU_INDEX_3             3
#define MMIMP3_MENU_INDEX_4             4
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
//for PDA style
#ifdef MMI_PDA_SUPPORT
typedef enum
{
    MMIMP3_SETTINGS_REPEAT = 1,
    MMIMP3_SETTINGS_SHUFFLE,
    MMIMP3_SETTINGS_EQ,
    MMIMP3_SETTINGS_BT,
    MMIMP3_SETTINGS_BT_HEADSET,
    MMIMP3_SETTINGS_CODE_TYPE,
    
    MMIMP3_SETTINGS_MAX
}MMIMP3_SETTINGS_TYPE_E;
#else
typedef enum
{
    MMIMP3_SETTINGS_DISPLAY = 1,
    MMIMP3_SETTINGS_EQ,
    MMIMP3_SETTINGS_BT,
    MMIMP3_SETTINGS_BT_HEADSET,
    MMIMP3_SETTINGS_CODE_TYPE,
    
    MMIMP3_SETTINGS_MAX
}MMIMP3_SETTINGS_TYPE_E;

#endif //MMI_PDA_SUPPORT

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenSetAlarmWin(
    wchar *file_full_path_ptr,
    uint16 file_full_path_len
);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenEQSetWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenDisplaySetWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenRepeatModeSetWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenShuffleModeSetWin(void);

#ifdef BLUETOOTH_SUPPORT  
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenBTSetWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenBTHeadSetWin(void);
#endif

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenTxtCodeTypeSetWin(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void  MMIAPSETWIN_OpenSettingWin(void);

/*****************************************************************************/
//  Description :
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_EQ_MODE_E MMIAPSETWIN_GetCurrentEqMode(void);
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif


