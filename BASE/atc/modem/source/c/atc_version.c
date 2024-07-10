/******************************************************************************
 ** File Name:      atc_version.c                                             *
 ** Author:         Fancier.Fan                                               *
 ** DATE:           08/24/2005                                                *
 ** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file include version of AT                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 08/24/2005      Fancier.Fan     Create.                                   *
 ******************************************************************************/
#ifndef _ATC_VERSION_C_
#define _ATC_VERSION_C_

static const char s_atc_version_info[] =
{"W14.06"};

/*****************************************************************************/
//  Description:    return the MMI version
//                  Global resource dependence:
//                      s_atc_version_info
//  Author:         Fancier.Fan
//                  Note: 2005 8 24
/*****************************************************************************/
const char *SCI_GetATVersionInfo(void)      // return atc version

{
    return s_atc_version_info;
}



#endif  // _ATC_VERSION_C_
