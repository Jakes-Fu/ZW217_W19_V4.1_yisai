

#ifndef RM_PACKET_H
#define RM_PACKET_H

#include "helix_types.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

/*
 * Packet struct
 *
 * Users are strongly encouraged to use the
 * accessor functions below to retrieve information
 * from the packet, since the definition of this
 * struct may change in the future.
 */
typedef struct rm_packet_struct
{
    UINT32 ulTime;  /* packet timestamp */
    UINT16 usStream; /*stream num*/
    UINT16 usASMFlags; /* ulASMFlags |= (hdr.flags & HX_KEYFRAME_FLAG ? HX_ASM_SWITCH_ON : 0); */
    BYTE   ucASMRule;
    BYTE   ucLost;
    UINT16 usDataLen;
    BYTE*  pData;
} rm_packet;

/*
 * Packet Accessor functions
 */
UINT32 rm_packet_get_timestamp(rm_packet* packet);
UINT32 rm_packet_get_stream_number(rm_packet* packet);
UINT32 rm_packet_get_asm_flags(rm_packet* packet);
UINT32 rm_packet_get_asm_rule_number(rm_packet* packet);
UINT32 rm_packet_get_data_length(rm_packet* packet);
BYTE*  rm_packet_get_data(rm_packet* packet);
HXBOOL rm_packet_is_lost(rm_packet* packet);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RM_PACKET_H */
