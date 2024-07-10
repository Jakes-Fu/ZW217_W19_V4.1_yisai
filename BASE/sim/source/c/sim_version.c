/******************************************************************************
 ** File Name:      sim_version.c                                             *
 ** Author:         Dan.Jiang                                                 *
 ** Date:           08/24/2005                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file include version of sim                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 08/24/2005     Dan.Jiang        Create                                    *
 ******************************************************************************/
#ifndef _sim_VERSION_C_
#define _sim_VERSION_C_

static const    char    s_sim_version_info[] =
{"1406"};

const char *SCI_GetSimVersionInfo(void)      // return sim version

{
    return s_sim_version_info;
}
#endif  // _sim_VERSION_C_

