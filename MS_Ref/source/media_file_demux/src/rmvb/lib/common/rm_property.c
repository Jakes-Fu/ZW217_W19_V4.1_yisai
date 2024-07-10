

#include "helix_types.h"
#include "rm_property.h"

const char* rm_property_get_name(rm_property* prop)
{
    const char* pRet = HXNULL;

    if (prop)
    {
        pRet = (const char*) prop->pName;
    }

    return pRet;
}

UINT32 rm_property_get_type(rm_property* prop)
{
    UINT32 ulRet = 0;

    if (prop)
    {
        ulRet = prop->ulType;
    }

    return ulRet;
}

UINT32 rm_property_get_value_uint32(rm_property* prop)
{
    UINT32 ulRet = 0;

    if (prop)
    {
        ulRet = (UINT32) prop->pValue;
    }

    return ulRet;
}

const char* rm_property_get_value_cstring(rm_property* prop)
{
    const char* pRet = HXNULL;

    if (prop)
    {
        pRet = (const char*) prop->pValue;
    }

    return pRet;
}

UINT32 rm_property_get_value_buffer_length(rm_property* prop)
{
    UINT32 ulRet = 0;

    if (prop)
    {
        ulRet = prop->ulValueLen;
    }

    return ulRet;
}

BYTE* rm_property_get_value_buffer(rm_property* prop)
{
    BYTE* pRet = HXNULL;

    if (prop)
    {
        pRet = prop->pValue;
    }

    return pRet;
}
