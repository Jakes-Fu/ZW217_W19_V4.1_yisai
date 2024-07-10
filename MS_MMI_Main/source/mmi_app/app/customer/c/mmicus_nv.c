/*****************************************************************************
** File Name:      mmicus_nv.c                                                 *
** Author:                                                                   *
** Date:           12/2011                                                   *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe Customer NV                     *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 
******************************************************************************/
#ifdef CSC_SUPPORT

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmi_modu_main.h"
#include "mmicus_nv.h"
#include "mmi_nv.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
/*the length of Customer NV*/
const uint16 cus_nv_len[] =
{
    sizeof(BOOLEAN)//MMICUS_CSC_IS_INIT
};


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//     Description : register Customer module NV len and max item
//    Global resource dependence : none
//  Author: 
//    Note:
/*****************************************************************************/
PUBLIC void MMICUS_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_CUS, cus_nv_len, ARR_SIZE(cus_nv_len));
}

/*****************************************************************************/
// 	Description : Get the if the csc xml nv set is init  
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMICUS_GetCscIsInit(void)
{
    BOOLEAN is_csc_init = FALSE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMICUS_CSC_IS_INIT, &is_csc_init, return_value);

    if (MN_RETURN_SUCCESS != return_value)
    {
        is_csc_init = FALSE;
        MMINV_WRITE(MMICUS_CSC_IS_INIT, &is_csc_init);
    }

    return is_csc_init;
}

/*****************************************************************************/
// 	Description : Set the if the csc xml nv set is init  
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMICUS_SetCscIsInit(BOOLEAN is_csc_init)
{
    MMINV_WRITE(MMICUS_CSC_IS_INIT, &is_csc_init);
}

#endif
