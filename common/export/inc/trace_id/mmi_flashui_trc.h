/******************************************************************************
 ** File Name:      mmi_flashui_trc.h                                         *
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
//trace_id:266
#ifndef _MMI_FLASHUI_TRC_H_
#define _MMI_FLASHUI_TRC_H_
#include "sci_log.h" 
#include "sci_trace_mod.h"
#ifdef TRACE_TYPE_CLASSIC
#define AMUSE_FILE_DEVICE_85_112_2_18_3_27_24_0 "MMIEBOOK_FFS_fclose file_handle = PNULL"
#define AMUSE_FILE_DEVICE_141_112_2_18_3_27_24_1 "amuse_fwrite invalid parameter"
#define AMUSE_FILE_DEVICE_308_112_2_18_3_27_25_2 "flash_printf:***** %s"
#define MAL_PHONE_FUNC_41_112_2_18_3_27_30_3 "Mal_Phone_Entry_Screen id = %d error!"
#endif //End of TRACE_TYPE_CLASSIC

#ifdef TRACE_TYPE_ID 
BEGIN_TRACE_MAP(MMI_FLASHUI_TRC)
TRACE_MSG(AMUSE_FILE_DEVICE_85_112_2_18_3_27_24_0,"MMIEBOOK_FFS_fclose file_handle = PNULL")
TRACE_MSG(AMUSE_FILE_DEVICE_141_112_2_18_3_27_24_1,"amuse_fwrite invalid parameter")
TRACE_MSG(AMUSE_FILE_DEVICE_308_112_2_18_3_27_25_2,"flash_printf:***** %s")
TRACE_MSG(MAL_PHONE_FUNC_41_112_2_18_3_27_30_3,"Mal_Phone_Entry_Screen id = %d error!")
END_TRACE_MAP(MMI_FLASHUI_TRC)
#endif //End of TRACE_TYPE_ID
#endif  // End of _MMI_FLASHUI_TRC_H_

