

#ifndef RA_FORMAT_INFO_H
#define RA_FORMAT_INFO_H

#include "helix_types.h"

/*
 * ra_format_info struct
 *
 */
typedef struct ra_format_info_struct
{
    UINT32 ulSampleRate;
    UINT32 ulActualRate;
    UINT16 usBitsPerSample;
    UINT16 usNumChannels;
    UINT16 usAudioQuality;
    UINT16 usFlavorIndex;
    UINT32 ulBitsPerFrame;
    UINT32 ulGranularity;
    UINT32 ulOpaqueDataSize;
    BYTE*  pOpaqueData;
} ra_format_info;

#endif /* #ifndef RA_FORMAT_INFO_H */
