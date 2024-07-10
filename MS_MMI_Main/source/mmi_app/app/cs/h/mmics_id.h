/*****************************************************************************
** File Name:      mmics_id.h                                               *
** Author:         haiyang.hu                                                *
** Date:           2006/09/22                                                *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ACC win and control id      *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/22     haiayng.hu        Create                                   *
******************************************************************************/

#ifndef _MMICS_ID_H_
#define _MMICS_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#define WIN_ID_DEF(win_id)          win_id

// window ID
typedef enum
{
    MMI_CS_WIN_ID_START = (MMI_MODULE_CMCS << 16),


    #include "mmics_id.def"    

    MMICS_MAX_WIN_ID

}MMICS_WINDOW_ID_E;

#undef WIN_ID_DEF
 // window ID

// control ID
typedef enum
{
    MMICS_CTRL_ID_START = MMICS_MAX_WIN_ID,
//CS
    MMICS_MAIN_MENU_ID,                  //dcd main tab ctrl
   
    MMICS_SERVICE_GUIDE_ID,                  //dcd main tab ctrl

    MMICS_E100_MENU_ID,                  //dcd main tab ctrl	

    MMICS_ABOUT_E100_ID,	

    MMICS_SERVICE_SETTING_MENU_ID,		
    MMICS_CM_NAME_TITLE_LABEL_CTRL_ID,	

    MMICS_CM_NAME_LABEL_CTRL_ID,		

    MMICS_CM_NUM_TITLE_LABEL_CTRL_ID,	

    MMICS_CM_NUM_LABEL_CTRL_ID,	

    MMICS_CM_NAME_TEXTEDIT_CTRL_ID,	

    MMICS_CM_PHONE_NUMEDIT_CTRL_ID,

    MMICS_SERVICE_HOTLINE_NAME_CTRL_ID,	

    MMICS_SERVICE_HOTLINE_NUM_CTRL_ID,	    

    MMICS_SERVICE_HOTLINE_NUMEDIT_CTRL_ID,	    	
    
    MMICS_MAX_CTRL_ID
    
}MMICS_CONTROL_ID_E;
/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DECLARE                          *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMICS_RegWinIdNameArr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMICS_ID_H_

