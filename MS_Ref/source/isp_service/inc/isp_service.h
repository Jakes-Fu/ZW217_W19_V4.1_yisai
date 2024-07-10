#ifndef __ISP_SERVICE_H_COMM__
#define __ISP_SERVICE_H_COMM__


#if (defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6600L))
#include "isp_service_sc6x00.h"
#elif defined(PLATFORM_SC8800H)
#include "isp_service_sc8800h.h"
#elif defined(PLATFORM_SC8800G)
#include "isp_service_sc8800g.h"
#elif defined(PLATFORM_SC6530)
#include "isp_service_sc6530.h"
#elif defined(PLATFORM_SC6531EFM)
#include "isp_service_sc6530.h"
#elif defined(PLATFORM_UWS6121E)
#include "isp_service_uws6121e.h"
#endif


#endif
