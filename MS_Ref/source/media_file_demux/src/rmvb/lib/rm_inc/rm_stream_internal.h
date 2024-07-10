

#ifndef RM_STREAM_INTERNAL_H
#define RM_STREAM_INTERNAL_H

#include "helix_types.h"
#include "rm_property.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

/*
 * Internal stream header struct
 *
 * The public definition of rm_stream_header
 * is void - opaque to the user. Properties of the
 * stream header are accessed via accessor functions
 * by the user.
 */
typedef struct rm_stream_header_internal_struct
{
    char*        pMimeType;
    char*        pStreamName;
    UINT32       ulStreamNumber;
    UINT32       ulMaxBitRate;
    UINT32       ulAvgBitRate;
    UINT32       ulMaxPacketSize;
    UINT32       ulAvgPacketSize;
    UINT32       ulDuration;
    UINT32       ulPreroll;
    UINT32       ulStartTime;
    UINT32       ulOpaqueDataLen;
    BYTE*        pOpaqueData;
    UINT32       ulNumProperties;
    rm_property* pProperty;
} rm_stream_header_internal;

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RM_STREAM_INTERNAL_H */
