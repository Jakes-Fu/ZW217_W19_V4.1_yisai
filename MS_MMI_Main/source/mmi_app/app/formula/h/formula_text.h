#ifndef FORMULA_MMI_TEXT_H_
#define FORMULA_MMI_TEXT_H_

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

typedef enum FORMULA_MMI_TEXT_ID_E
{
        FORMULA_TXT_NULL = ( MMI_MODULE_FORMULA_WIN_ID << 16 ) | MMI_SYS_RES_FLAG,
        #include "formula_mdu_def.h"
        FORMULA_TXT_MAX
} FORMULA_MMI_TEXT_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif // CC_MMI_TEXT_H_