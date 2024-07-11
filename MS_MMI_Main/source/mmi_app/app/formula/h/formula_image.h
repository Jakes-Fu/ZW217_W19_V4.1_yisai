#ifndef FORMULA_MMI_IMAGE_H_
#define FORMULA_MMI_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Image Resource ID
///////////////////////////////////////////////////////////////////////////////

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

typedef enum FORMULA_MMI_IMAGE_ID_E
{
        FORMULA_IMAGE_NULL = ( MMI_MODULE_FORMULA_WIN_ID << 16 ) | MMI_SYS_RES_FLAG,
#include "formula_mdu_def.h"
        FORMULA_IMAGE_MAX_ID
} FORMULA_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //ZDT_MMI_IMAGE_H_