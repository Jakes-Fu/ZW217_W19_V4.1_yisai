#ifndef _ZTE_MENU_IMAGE_H_
#define _ZTE_MENU_IMAGE_H_

#include "sci_types.h"
#include "mmi_imagefun.h"
#include "mmi_module.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MACRO_MDU_TYPE  MACRO_MDU_IMAGE
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "zte_menu_mdu_def.h"
#endif

typedef enum ZTE_MENU_MMI_IMAGE_ID_E
{
	ZTE_MENU_IMAGE_NULL = ( MMI_MODULE_ZTE_MENU << 16 ) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "zte_menu_mdu_def.h"
#endif
	ZTE_MENU_IMAGE_MAX_ID
} ZTE_MENU_MMI_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif