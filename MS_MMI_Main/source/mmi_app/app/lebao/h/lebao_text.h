#ifndef __LEBAO_TEXT_H
#define __LEBAO_TEXT_H

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
#include "lebao_mdu_def.h"
#endif

typedef enum MMI_LEBAO_TEXT_ID_E
{
	MMI_LEBAO_TEXT_ID_NULL = ( MMI_MODULE_LEBAO << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "lebao_mdu_def.h"
#endif
	MMI_LEBAO_TEXT_ID_MAX
} MMI_LEBAO_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // __LEBAO_TEXT_H
