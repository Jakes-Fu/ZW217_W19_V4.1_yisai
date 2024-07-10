#ifndef BSC_DEBUG_H
#define BSC_DEBUG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

#define BSC_LOG_TRACE_ON				0
#define BSC_INF_TRACE_ON				0

#ifdef _BSC_DEBUG
#define BSC_ASSERT(_x_)				SCI_ASSERT(_x_) 
#define BSC_VERIFY(_x_)				SCI_ASSERT(_x_)
#define BSC_ERR_TRACE(_x_)			SCI_TraceLow _x_
#else
#define BSC_ASSERT(_x_)				((void)0)
#define BSC_VERIFY(_x_)				((void)(_x_))
#define BSC_ERR_TRACE(_x_)			((void)0)
#endif /* _BSC_DEBUG */

#if	BSC_LOG_TRACE_ON
#define BSC_LOG_TRACE(_x_)			SCI_TraceLow _x_
#else
#define BSC_LOG_TRACE(_x_)			((void)0)
#endif /* BSC_LOG_TRACE_ON */

#if BSC_INF_TRACE_ON
#define BSC_INF_TRACE(_x_)			SCI_TraceLow _x_
#else
#define BSC_INF_TRACE(_x_)			((void)0)
#endif /* BSC_INF_TRACE_ON */

#ifdef __cplusplus
}
#endif /* __cplusplus */ 

	
#endif /* BSC_DEBUG_H */
