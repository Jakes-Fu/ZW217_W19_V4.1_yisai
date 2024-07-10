/*****************************************************************************
** File Name:      mmidcd_export.c                                           *
** Author:                                                                   *
** Date:           2008/10/21                                                *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2008/10/21     aoke.hu          Create
******************************************************************************/

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

#include "mmi_app_cs_trc.h"
#include "std_header.h"
#include "sci_types.h"
#include "mmk_type.h"
#include "mmk_app.h"

#ifdef CMCC_UI_STYLE
#include "mmics_main.h"
#include "mmics_export.h"
#include "mmics_nv.h"
#include "mmics_wintab.h"
#include "mmics_id.h"
#include "mmiphone_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
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
PUBLIC MMI_RESULT_E HandleCsProcessMsg(
								  PWND app_ptr,
								  uint16 msg_id,
								  DPARAM param
								  );
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

MMI_APPLICATION_T g_mmics_app={0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICS_InitModule(void)
{    
    MMICS_RegMenuGroup();
    MMICS_RegNv();
    MMICS_RegWinIdNameArr();	
}

/*****************************************************************************/
//  Description :  Father windows
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPICS_OpenMainWin(void)
{
    MMICS_OpenMainWin();
}


/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICS_AppInit(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    //SCI_TRACE_LOW:"[MMIDCD]: MMIAPICS_AppInit !!!! "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMICS_EXPORT_83_112_2_18_2_12_24_0,(uint8*)"");
    
    g_mmics_app.ProcessMsg		= HandleCsProcessMsg;
    g_mmics_app.component_type	= CT_APPLICATION;
    
    MMINV_READ(MMINV_CS_SET_INFO, MMICS_GeSettingInfo(), return_value);
    if (return_value != MN_RETURN_SUCCESS)
    {
        //get setting
        MMICS_SetSettingDefault();	
        
    }
    
    /*when dcd init ,parse xml file*/
    MMICS_InitStart();	   
    
}

#else	//!宏定义MMI_ENABLE_DCD


#endif	//!宏定义end MMI_ENABLE_DCD

