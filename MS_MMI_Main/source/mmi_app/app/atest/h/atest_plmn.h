/*****************************************************************************
** File Name:      atest_plmn.h                                             *
** Author:         lin.chen                                                  *
** Date:           18/04/2012                                               *
** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to get plmn data     		 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
**   DATE           NAME             DESCRIPTION                             *
**  18/04/2012     lin.chen          Created                                 **
******************************************************************************/

#ifndef _ATEST_PLMN_H
#define _ATEST_PLMN_H 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "stdio.h"

#ifdef WIN32
/**--------------------------------------------------------------------*
 **                         Compiler Flag                              *
 **--------------------------------------------------------------------*/

#define  ATEST_MAX_PATH		500

/**-----------------------------------------------------------------------*
 **                                Function Prototypes                    *
 **-----------------------------------------------------------------------*/
PUBLIC void Atest_GetPLMNDataFromMocor(char *PlmnTestingFilePath);

#endif//WIN32

#endif //_ATEST_PLMN_H

