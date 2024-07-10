/******************************************************************************
 ** File Name:      mmi_app_qbtheme_trc.h                                         *
 ** DATE:           2012.03.18                                                *
 ** Copyright:      Copyright 2012 by Spreadtrum Communications,Inc.          *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:                                                              *
 **                                                                           *
 **                                                                           *
 ******************************************************************************
 ******************************************************************************
 **                               Edit History                                *
 ** ------------------------------------------------------------------------- *
 **     DATE          NAME             DESCRIPTION                            *
 **---------------------------------------------------------------------------*
 **  2012.03.18                         Create.                               *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
//trace_id:215
#ifndef _MMI_APP_QBTHEME_TRC_H_
#define _MMI_APP_QBTHEME_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMIQBTHEME_APP_1498_112_2_18_2_48_51_0 "@@QBTHEME_GetResImgDat:offset = 0x%x, res_handle=0x%x, sfs_err = %d\n"
#define MMIQBTHEME_APP_1607_112_2_18_2_48_51_1 "@@QBTHEME UdiskMount:%d \n"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_QBTHEME_TRC)
TRACE_MSG(MMIQBTHEME_APP_1498_112_2_18_2_48_51_0,"@@QBTHEME_GetResImgDat:offset = 0x%x, res_handle=0x%x, sfs_err = %d\n")
TRACE_MSG(MMIQBTHEME_APP_1607_112_2_18_2_48_51_1,"@@QBTHEME UdiskMount:%d \n")
END_TRACE_MAP(MMI_APP_QBTHEME_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_QBTHEME_TRC_H_

