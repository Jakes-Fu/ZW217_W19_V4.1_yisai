/******************************************************************************
 ** File Name:      dl_api.h                                                  *
 ** Author:         fancier fan                                               *
 ** Date:           05/23/2002                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This header file contains general types and macros that   *
 **         		are of use to all modules.The values or definitions are   *
 ** 				dependent on the specified target.                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 05/23/2002     fancier fan      Create.                                   *
 ******************************************************************************/
#ifndef DL_API_H
#define DL_API_H

#ifdef _RTOS
	#include "sci_types.h"
#else
	#include "..\..\..\..\rtos\include\sci_types.h"
#endif

typedef enum 
{
	DL_DATA_MODE ,    // layer1 always can get data by read???data 
	DL_COMMON_MODE    // layer1 can get data or get PNULL by read???data
}DL_READ_MODE_E;


uint8* DL_ReadSdcchData(DL_READ_MODE_E);
uint8* DL_ReadFacchData(DL_READ_MODE_E);
uint8* DL_ReadSacchData(DL_READ_MODE_E);

#endif  /* DL_API_H */
