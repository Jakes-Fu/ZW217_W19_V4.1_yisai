/*****************************************************************************
** File Name:      watch_qrcode_export.h                                     *
** Author:         bin.wang1                                                 *
** Date:           03/24/2020                                                *
** Copyright:      All Rights Reserved.                                      *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 03/2020      bin.wang1              Creat                                 *
******************************************************************************/

#ifndef _WATCH_QRCODE_H_
#define _WATCH_QRCODE_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 

#ifdef _cplusplus
    extern   "C"
    {
#endif

/*****************************************************************************/
//  Description : open qr code windows
//  Global resource dependence : 
//  Author:
//  Note: only support digit qrcode now
/*****************************************************************************/
PUBLIC void WatchQRCode_MainWin_Enter( uint8* input_str_ptr, int32 input_str_Len );

PUBLIC void WatchQRCode_Draw( uint8* input_str_ptr, int32 input_str_Len );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
    }
#endif

#endif//_SAMPLE_MAIN_H_
