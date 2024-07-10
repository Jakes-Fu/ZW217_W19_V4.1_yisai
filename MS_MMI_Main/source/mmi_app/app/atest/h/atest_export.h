/***************************************************************************************
** FileName    :  atest_export.h                                                        **
** Author      :  Mingxuan.Zhu                                                        **
** Date        :  14/05/2010                                                          **
** Copyright   :  2001 Spreadtrum, Incorporated. All Rights Reserved.                 **
** Description :  The definition of the message dispatch function in ATEST module.    **
****************************************************************************************
** Edit History                                                                       **
** ---------------------------------------------------------------------------------- **
**     DATE    |    EDITOR NAME    |    DESCRIPTION                                   **
** ---------------------------------------------------------------------------------- **
**  14/05/2010     Mingxuan.Zhu         Created                                       **
**  29/09/2010     Mingxuan.Zhu         Updated ( delete ota function )               **
**  10/02/2012     murphy.xie           Change file name ( interface clear  up )      **
** ---------------------------------------------------------------------------------- **
***************************************************************************************/
#ifndef _ATEST_EXPORT_H_
#define _ATEST_EXPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**-----------------------------------------------------------------------------------*
 **                                   Dependencies                                    *
 **-----------------------------------------------------------------------------------*/
#include "mmk_type.h"

/**************************************************************************************/
// Description      : process the signal from control panel.
//--------------------------------------------------------------------------------------
// Input Parameter  : 1.DPARAM  param
// Output Parameter : NONE
// Return Value     : NONE
//--------------------------------------------------------------------------------------
// Global Resource Dependence : NONE
// Author: Mingxuan.Zhu
// Note:
/**************************************************************************************/
PUBLIC MMI_RESULT_E MMIAPI_ATESTMAINHandleAtestMsg(DPARAM param);


/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define ATESTMAIN_HandleAtestMsg				         MMIAPI_ATESTMAINHandleAtestMsg

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
    }
#endif

#endif//_ATEST_EXPORT_H_
