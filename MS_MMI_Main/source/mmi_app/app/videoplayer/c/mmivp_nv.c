/*****************************************************************************
** File Name:                    mmivp_nv.c                                 *
** Author:                                                                   *
** Date:           2010/3/11                                                 *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe vp state machine            *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2010/3/11      apple.zhang      Create
******************************************************************************/
#define MMIVP_NV_C

/*---------------------------------------------------------------------------*/
/*                          Include Files                                    */
/*---------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmivp_export.h"
#include "mmivp_nv.h"
#include "mmivp_internal.h"
/*---------------------------------------------------------------------------*/
/*                         STATIC DEFINITION                                 */
/*---------------------------------------------------------------------------*/
const uint16 vp_nv_len[] =
{
    sizeof(VP_NV_SETTING_T),                                  //  VP_SETTING_INFO
};

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ×¢²ánv
//  Global resource dependence :  none
//  Author: apple.zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIVP_RegNv(void)
{
    MMI_RegModuleNv(MMI_MODULE_VP, vp_nv_len, sizeof(vp_nv_len) / sizeof(uint16));
}


