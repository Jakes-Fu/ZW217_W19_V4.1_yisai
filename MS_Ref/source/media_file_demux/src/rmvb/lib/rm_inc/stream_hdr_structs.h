

#ifndef STREAM_HDR_STRUCTS_H
#define STREAM_HDR_STRUCTS_H

#include "helix_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* map an ASM rule to some other property */
typedef struct rm_rule_map_struct
{
    UINT32  ulNumRules;
    UINT32* pulMap;
} rm_rule_map;

/* rm multistream header -- surestream */
typedef struct rm_multistream_hdr_struct
{
    UINT32             ulID;            /* unique identifier for this header */
    rm_rule_map rule2SubStream;  /* mapping of ASM rule number to substream */
    UINT32             ulNumSubStreams; /* number of substreams */
} rm_multistream_hdr;

#define RM_MULTIHEADER_OBJECT 0x4D4C5449 /* 'MLTI' */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef STREAM_HDR_STRUCTS_H */
