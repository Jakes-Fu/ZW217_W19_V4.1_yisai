#ifndef _STOPWATCH_IMAGE_H_
#define _STOPWATCH_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "zte_stopwatch_mdu_def.h"
#endif

typedef enum STOPWATCH_MMI_IMAGE_ID_E
{
	STOPWATCH_IMAGE_NULL = ( MMI_MODULE_ZTE_STOPWATCH << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "zte_stopwatch_mdu_def.h"
#endif
	STOPWATCH_IMAGE_MAX_ID
} STOPWATCH_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif