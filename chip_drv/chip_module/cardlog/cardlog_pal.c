/******************************************************************************
 ** File Name:      cardlog_pal.c                                                 *
 ** Author:                                                             *
 ** DATE:                                                           *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/
#include "sci_types.h"
#include "os_api.h"
#include "scm_api.h"
#include "cardlog_pal.h"

#ifndef MODEM_PLATFORM
BOOLEAN Cardlog_Pal_init()
{
    if (SCM_STATUS_NORMAL == SCM_GetSlotStatus (SCM_SLOT_0))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void Cardlog_Pal_uninit()
{
    // Do nothing
}

BOOLEAN Cardlog_Pal_r (uint32 start, uint32 scts, void *buf)
{
    SCM_PARAM_T param;

    if (0 == scts)
    {
        return TRUE;
    }

    param.readParam.startBlock = start;
    param.readParam.num = scts;
    param.readParam.buf = (uint8 *) buf;
    return SCM_IOCTL (SCM_SLOT_0,SCM_FUNC_READ ,&param);
}

BOOLEAN Cardlog_Pal_w (uint32 start, uint32 scts, void *buf)
{

    SCM_PARAM_T param;

    if (0 == scts)
    {
        return TRUE;
    }

    param.writeParam.startBlock = start;
    param.writeParam.num = scts;
    param.writeParam.buf = (uint8 *) buf;
    return SCM_IOCTL (SCM_SLOT_0,SCM_FUNC_WRITE ,&param);
}

BOOLEAN Cardlog_Pal_e (uint32 start, uint32 scts)
{
    SCM_PARAM_T param;

    if (0 == scts)
    {
        return TRUE;
    }

    param.eraseParam.startBlock = start;
    param.eraseParam.endBlock = start + scts - 1;
    return SCM_IOCTL (SCM_SLOT_0,SCM_FUNC_ERASE,&param);
}

uint32  Cardlog_Pal_g (void)
{
    SCM_PARAM_T param = {0};
    SCM_IOCTL (SCM_SLOT_0,SCM_FUNC_GETCAPCITY,&param);
    return param.getCapcityParam.capcity;
}
#else
BOOLEAN Cardlog_Pal_init()
{
    return TRUE;
}

void Cardlog_Pal_uninit()
{
    // Do nothing
}

BOOLEAN Cardlog_Pal_r (uint32 start, uint32 scts, void *buf)
{
    return TRUE;
}

BOOLEAN Cardlog_Pal_w (uint32 start, uint32 scts, void *buf)
{
    return TRUE;
}

BOOLEAN Cardlog_Pal_e (uint32 start, uint32 scts)
{
    return TRUE;
}

uint32  Cardlog_Pal_g (void)
{
    return TRUE;
}
#endif


