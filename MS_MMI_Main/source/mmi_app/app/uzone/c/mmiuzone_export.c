/*****************************************************************************
** File Name:      mmiuzone_export.c                                                *
** Author:         Jessica                                               *
** Date:           21/12/2011                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       Jessica              Creat
******************************************************************************/
#include "mmi_app_uzone_trc.h"
#ifdef MMIUZONE_SUPPORT

#define _MMIUZONE_EXPORT_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif

#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmiuzone_nv.h"
#include "mmiuzone_id.h"
#include "mmiuzone_internal.h"
#include "mmiuzone.h"
#include "mmiuzone_se.h"
#include "mmi_modu_main.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define UZONE_DEFAULT_ITEM 0

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get default nv value
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetDefaultNV(MMIUZONE_NV_INFO_T* nv_info_ptr);



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get default nv value
//  Global resource dependence :
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void GetDefaultNV(MMIUZONE_NV_INFO_T* nv_info_ptr)
{
    if(PNULL == nv_info_ptr)
    {
        return;
    }
    nv_info_ptr->cur_item_index = UZONE_DEFAULT_ITEM;
}



/*****************************************************************************/
//  Description : init uzone module
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUZONE_InitModule(void)
{
    //register nv
    MMIUZONE_RegNv();

    //register win id name
    MMIUZONE_RegWinIdNameArr();

    MMIUZONE_RegAppletInfo();
}

/*****************************************************************************/
//  Description : init uzone nv
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUZONE_Init(void)
{
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;
    MMIUZONE_NV_INFO_T nv_info = {0};
    
    MMINV_READ(MMINV_UZONE_INFO, &nv_info, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        GetDefaultNV(&nv_info);
        MMINV_WRITE(MMINV_UZONE_INFO, &nv_info);
    }
    MMIUZONE_SetNVInfo(nv_info);
    //SCI_TRACE_LOW:"[MMIUZONE] MMIAPIUZONE_Init - current itme = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_EXPORT_104_112_2_18_3_4_22_45,(uint8*)"d",nv_info.cur_item_index);
}

/*****************************************************************************/
//  Description : reset uzone factory settings
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUZONE_ResetFactorySettings(void)
{
    MMIUZONE_NV_INFO_T nv_info = {0};

    GetDefaultNV(&nv_info);
    MMINV_WRITE(MMINV_UZONE_INFO, &nv_info);
    MMIUZONE_SetNVInfo(nv_info);
    //SCI_TRACE_LOW:"[MMIUZONE] MMIAPIUZONE_ResetFactorySettings - current itme = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIUZONE_EXPORT_118_112_2_18_3_4_22_46,(uint8*)"d",nv_info.cur_item_index);
}


/*****************************************************************************/
//  Description : Entry function of uzone
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUZONE_Entry(void)
{
     MMIUZONE_Entry();
}

/*****************************************************************************/
//  Description : uzone is opened
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIUZONE_IsOpened(void)
{
    BOOLEAN ret = FALSE;

    if (MMK_IsOpenWin(MMIUZONE_MAIN_WIN_ID))
    {
        ret = TRUE;
    }

	SCI_TRACE_LOW("[MMIUZONE] MMIAPIUZONE_IsOpened ret=%d", ret);
    return ret;
}

/*****************************************************************************/
//  Description : exit uzone
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIUZONE_Exit(void)
{
    MMIUZONE_Exit();
}
#endif //MMIUZONE_SUPPORT
