/******************************************************************************
 ** File Name:      atc_port.h                                                *
 ** Author:         Shijun Cui                                                *
 ** Date:           02/08/2006                                                *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This file contains types and MICROS that are of use to all*
 **                 other ATC modules.                                        *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** DATE           NAME             Description:                              *
 **---------------------------------------------------------------------------*
 ** 02/08/2006     Shijun Cui       Create.                                   *
 ******************************************************************************/

#ifndef _ATC_PORT_H_
#define _ATC_PORT_H_
/*******************************************************************************
 **                        Common Header Files                                 *
 ******************************************************************************/
#include "sci_types.h"


/*******************************************************************************
 **                        Functon  declaration                                *
 ******************************************************************************/
/******************************************************************************/
// Description :export function interface for MMI to implement which need in ATC
// Global resource dependence : None
// Author : shijun
// Note :  the length of AT## content should not larger than 512 bytes
/******************************************************************************/
extern uint32 ATC_ExtCmd(
                   uint8 *cmd, // point to the AT## content
                   uint32 len  // length of the AT## content
                   );

/******************************************************************************/
// Description :export function interface for ref to create ATC task
// Global resource dependence : None
// Author : shijun
// Note :  the length of AT## content should not larger than 512 bytes
/******************************************************************************/
extern uint32 SCI_InitAtc(
                   void
                   );

#endif // _ATC_PORT_H_