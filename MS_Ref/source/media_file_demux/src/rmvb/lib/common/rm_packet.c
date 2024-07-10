

#include "helix_types.h"
#include "rm_packet.h"

UINT32 rm_packet_get_timestamp(rm_packet* packet)
{
    UINT32 ulRet = 0;

    if (packet)
    {
        ulRet = packet->ulTime;
    }

    return ulRet;
}

UINT32 rm_packet_get_stream_number(rm_packet* packet)
{
    UINT32 ulRet = 0;

    if (packet)
    {
        ulRet = packet->usStream;
    }

    return ulRet;
}

UINT32 rm_packet_get_asm_flags(rm_packet* packet)
{
    UINT32 ulRet = 0;

    if (packet)
    {
        ulRet = packet->usASMFlags;
    }

    return ulRet;
}

UINT32 rm_packet_get_asm_rule_number(rm_packet* packet)
{
    UINT32 ulRet = 0;

    if (packet)
    {
        ulRet = packet->ucASMRule;
    }

    return ulRet;
}

UINT32 rm_packet_get_data_length(rm_packet* packet)
{
    UINT32 ulRet = 0;

    if (packet)
    {
        ulRet = packet->usDataLen;
    }

    return ulRet;
}

BYTE* rm_packet_get_data(rm_packet* packet)
{
    BYTE* pRet = HXNULL;

    if (packet)
    {
        pRet = packet->pData;
    }

    return pRet;
}

HXBOOL rm_packet_is_lost(rm_packet* packet)
{
    HXBOOL bRet = FALSE;

    if (packet)
    {
        bRet = (packet->ucLost ? TRUE : FALSE);
    }

    return bRet;
}

