/*****************************************************************************
** File Name:      mmiengtd_export.h                                                *
** Author:                                                                   *
** Date:           2007/10/26                                                *
** Copyright:      2007 Spreadrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe engtd module export function    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2007/10/26     yongwei.he      Create                                   *
******************************************************************************/

#ifndef _MMI_ENGTD_EXPORT_H
#define _MMI_ENGTD_EXPORT_H

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

typedef enum {
    MMIENG_VT_H263_PREFER=0,
    MMIENG_VT_MPEG4_PREFER,
    MMIENG_VT_H263_ONLY,
    MMIENG_VT_MPEG4_ONLY,
}MMIENG_VT_T;
/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENGTD_AppInit(void);

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENGTD_RegNv(void);

/********************************************************************************
 NAME:			MMIAPIENGTD_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
BOOLEAN MMIAPIENGTD_StartEngineerMenu(void);

/*****************************************************************************/
// 	Description : MMIAPIENGTD_InitModule
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENGTD_InitModule(void);

/********************************************************************************
 NAME:			MMIENGTD_GetIsDspParaLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jinju.ma
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIENGTD_GetIsDspParaLogOn(void);
/********************************************************************************
NAME:			MMIAPIENG_GetFtaFlag
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetFtaFlag(void);

/********************************************************************************
NAME:			MMIAPIENGTD_SetUart1UsedFlag
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetUart1UsedFlag(uint8 uart1flag);

/********************************************************************************
NAME:			MMIAPIENGTD_SetSMSSendPath
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetSMSSendPath(uint32 sms_service);

/********************************************************************************
NAME:			MMIAPIENGTD_SetServiceType
DESCRIPTION:	
AUTHOR:	sword		
DATE:			
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetServiceType(MN_PHONE_USER_SERVICE_SELECT_E service_type);

/********************************************************************************
 NAME:			MMIAPIENGTD_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENGTD_StartEngineerMenu(void);

PUBLIC void MMIENGTD_OpenUPLMNSettingWin(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif/* _MMI_ENGTD_EXPORT_H */


