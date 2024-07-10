/****************************************************************************
** File Name:      mmiemail_setting.h                                         *
** Author:                                                          			 *
** Date:           05/13/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2011                           create
** 
****************************************************************************/

#ifndef _MMIEMAIL_SETTING_H_
#define _MMIEMAIL_SETTING_H_

/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/

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
/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: start setting
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_SettingStart(void);

/*****************************************************************************/
// 	Description: entry email tip window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_EntryEmailTip(void);

/*****************************************************************************/
// 	Description: free g_email_opt_ptr;
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_FreeOptPtr(void);

/*****************************************************************************/
// 	Description: entry account setting window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_EntryAccountSetting(void);

/*****************************************************************************/
// 	Description: entry system setting window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_EntrySystemSetting(void);

/*****************************************************************************/
// 	Description: entry system setting window
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_SelectEmailAcc(void);

/*****************************************************************************/
// 	Description: exit setting 
//	Global resource dependence:
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_ExitSetting(void);
PUBLIC void MMIEMAIL_ExitEmailApp(void);
/*****************************************************************************/
// 	Description: register wifi notification
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_RegWifiNotify(void);

/*****************************************************************************/
// 	Description: deregister wifi notification
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_DeRegWifiNotify(void);

/*****************************************************************************/
// 	Description : email reset settings
//	Global resource dependence :
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIEMAIL_ResetSettings(void);

/*****************************************************************************/
// 	Description: 
//	Global resource dependence:
//  Author: tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_OpenNetSelectWin(void);
/*****************************************************************************/
// 	Description:
//	Global resource dependence:
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC void MMIEMAIL_InitSetting(uint32 act_id);

/*****************************************************************************/
// 	Description:
//	Global resource dependence:
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_IsNetAvailable(void);

/*****************************************************************************/
// 	Description:
//	Global resource dependence:
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_IsCurrentConfigNetAvailable(void);
/*****************************************************************************/
// 	Description: is device ok or not
//	Global resource dependence:
//  Author:tao.xue
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIEMAIL_IsDeviceReady(BOOLEAN is_pop_alert_win);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  /* _MMIEMAIL_SETTING_H_*/