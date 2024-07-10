/*****************************************************************************
** File Name:      sensor_text.h                                             *
** Author:         longwei.qiao                                              *
** Date:           04/15/2021                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe sensor UI text              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/2021     longwei.qiao         Create                                   *
******************************************************************************/


#ifndef _WATCH_SENSOR_TEXT_H_
#define _WATCH_SENSOR_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Text Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "sensor_mdu_def.h"
#endif

typedef enum WATCH_SENSOR_TEXT_ID_E
{
        SENSOR_TXT_NULL = ( WATCH_MODULE_SENSOR << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "sensor_mdu_def.h"
#endif
        SENSOR_TXT_MAX
} WATCH_SENSOR_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // _WATCH_SENSORTEXT_H_

