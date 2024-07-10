#ifndef _STOPWATCH_TEXT_H_
#define _STOPWATCH_TEXT_H_

#include "sci_types.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "zte_stopwatch_mdu_def.h"
#endif

typedef enum STOPWATCH_MMI_TEXT_ID_E
{
    STOPWATCH_TXT_NULL = ( MMI_MODULE_ZTE_STOPWATCH << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "zte_stopwatch_mdu_def.h"
#endif
	STOPWATCH_TXT_MAX
} STOPWATCH_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif 