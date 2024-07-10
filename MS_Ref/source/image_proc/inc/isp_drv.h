#ifndef __ISP_DRV_COMM_H__
#define __ISP_DRV_COMM_H__


#if defined(PLATFORM_SC6600L)||defined(PLATFORM_SC6800H)
#include "isp_drv_6x00.h"
#endif

#if defined(PLATFORM_SC8800H)
#include "isp_drv_sc8800h.h"
#endif

#if defined(PLATFORM_SC8800G)
#include "isp_drv_sc8800g.h"
#endif

#if defined(PLATFORM_SC6530)
#include "isp_drv_sc6530.h"
#endif

#if defined(PLATFORM_SC6531EFM)
#include "isp_drv_sc6530.h"
#endif

#if defined(PLATFORM_UWS6121E)
#include "isp_drv_uws6121e.h"
#endif


#endif // for __IVSP_DRV_H__

