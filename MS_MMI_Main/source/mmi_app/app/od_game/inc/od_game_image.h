#ifndef _OD_GAME_IMAGE_H_
#define _OD_GAME_IMAGE_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

typedef enum
{
	OD_GAME_IMAGE_NULL = ( MMI_MODULE_OD_GAME << 16 ) | MMI_SYS_RES_FLAG,
       #include "od_game_mdu_def.h"
	OD_GAME_IMAGE_MAX_ID
} OD_GAME_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif
#endif

