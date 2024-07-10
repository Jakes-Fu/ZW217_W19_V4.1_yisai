#ifndef __LEBAO_IMAGE_H_
#define __LEBAO_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE   MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

typedef enum MMI_LEBAO_IMAGE_ID_E
{
	MMI_LEBAO_IMAGE_ID_START = ( MMI_MODULE_LEBAO << 16 ) | MMI_SYS_RES_FLAG,
#include "lebao_mdu_def.h"
	MMI_LEBAO_IMAGE_ID_MAX
} MMI_LEBAO_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //__LEBAO_IMAGE_H_