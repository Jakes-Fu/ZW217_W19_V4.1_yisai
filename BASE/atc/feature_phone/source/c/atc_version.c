/******************************************************************************
 ** File Name:      atc_version.c                                             *
 ** Author:         Fancier.Fan                                               *
 ** Date:           08/24/2005                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file include version of AT                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 08/24/2005     Fancier.Fan      Create.                                   *
 ******************************************************************************/
#ifndef _ATC_VERSION_C_
#define _ATC_VERSION_C_
static const    char    s_atc_version_info[] = 
    {  "1406" };
    
/*****************************************************************************/
//  Description:    return the MMI version
//                  Global resource dependence:
//                      s_atc_version_info
//  Author:         Fancier.Fan
//                  Note: 2005 8 24
/*****************************************************************************/
const char * SCI_GetATVersionInfo(void)     // return atc version 
                                                    
{
    return s_atc_version_info;
}   
#endif  // _ATC_VERSION_C_
