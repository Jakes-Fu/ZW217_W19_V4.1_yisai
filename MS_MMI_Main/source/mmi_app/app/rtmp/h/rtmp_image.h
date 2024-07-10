#ifndef __RTMP_DEMO_IMAGE_H_
#define __RTMP_DEMO_IMAGE_H_

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

typedef enum MMI_RTMP_IMAGE_ID_E
{
	MMI_RTMP_IMAGE_ID_START = ( MMI_MODULE_RTMPDEMO << 16 ) | MMI_SYS_RES_FLAG,
#include "rtmp_mdu_def.h"
	MMI_RTMP_IMAGE_ID_MAX
} MMI_RTMP_IMAGE_ID_E;

#undef MACRO_MDU_TYPE

#ifdef __cplusplus
}
#endif

#endif //__RTMP_DEMO_IMAGE_H_