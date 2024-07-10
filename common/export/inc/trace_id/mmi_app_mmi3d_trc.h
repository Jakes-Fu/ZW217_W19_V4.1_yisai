/******************************************************************************
 ** File Name:      mmi_app_mmi3d_trc.h                                         *
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
//trace_id:194
#ifndef _MMI_APP_MMI3D_TRC_H_
#define _MMI_APP_MMI3D_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define MMI3D_150_112_2_18_2_37_19_0 "style_3d < MMISET_3D_STYLE_MAINMENU_MAX"
#define MMI3D_185_112_2_18_2_37_19_1 "MMIAPI3D_SaveMenuIcon PNULL == rect_ptr"
#define MMI3D_420_112_2_18_2_37_19_2 "icon_size * ICON_BUF_TOTAL_SIZE > BLOCK_MEM_POOL_MMI_3D_SIZE"
#define MMI3D_445_112_2_18_2_37_19_3 "icon_size * LIST_BUF_TOTAL_SIZE > BLOCK_MEM_POOL_MMI_3D_SIZE"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_APP_MMI3D_TRC)
TRACE_MSG(MMI3D_150_112_2_18_2_37_19_0,"style_3d < MMISET_3D_STYLE_MAINMENU_MAX")
TRACE_MSG(MMI3D_185_112_2_18_2_37_19_1,"MMIAPI3D_SaveMenuIcon PNULL == rect_ptr")
TRACE_MSG(MMI3D_420_112_2_18_2_37_19_2,"icon_size * ICON_BUF_TOTAL_SIZE > BLOCK_MEM_POOL_MMI_3D_SIZE")
TRACE_MSG(MMI3D_445_112_2_18_2_37_19_3,"icon_size * LIST_BUF_TOTAL_SIZE > BLOCK_MEM_POOL_MMI_3D_SIZE")
END_TRACE_MAP(MMI_APP_MMI3D_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_APP_MMI3D_TRC_H_

