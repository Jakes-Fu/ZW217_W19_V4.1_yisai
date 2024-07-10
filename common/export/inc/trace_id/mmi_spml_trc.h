/******************************************************************************
 ** File Name:      mmi_spml_trc.h                                         *
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
//trace_id:260
#ifndef _MMI_SPML_TRC_H_
#define _MMI_SPML_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define SPML_OTF_GSUB_88_112_2_18_3_24_42_0 "invalid OTF_ReplaceGlyphs"
#define SPML_API_329_112_2_18_3_24_45_1 "SPMLAPI_LineBreak optlen failed, opt_len = %d, len = %d "
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_SPML_TRC)
TRACE_MSG(SPML_OTF_GSUB_88_112_2_18_3_24_42_0,"invalid OTF_ReplaceGlyphs")
TRACE_MSG(SPML_API_329_112_2_18_3_24_45_1,"SPMLAPI_LineBreak optlen failed, opt_len = %d, len = %d ")
END_TRACE_MAP(MMI_SPML_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_SPML_TRC_H_

