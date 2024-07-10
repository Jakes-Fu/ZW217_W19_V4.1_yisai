#ifndef __IVSP_DRV_H__
#define __IVSP_DRV_H__

#if (defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6800H))
#include "ivsp_drv_6x00.h"
#elif (defined(PLATFORM_SC8800H) || defined(PLATFORM_SC8800G))
#include "ivsp_drv_8800h5.h"
#elif (defined(PLATFORM_SC6530))
#include "ivsp_drv_sc6530.h"
#elif (defined(PLATFORM_SC6531EFM))
#include "ivsp_drv_sc6530.h"
#elif (defined(PLATFORM_UWS6121E))
#include "ivsp_drv_sc6530.h"
#endif


#endif // for __IVSP_DRV_H__

