/*****************************************************************************
** File Name:      mmisfr_main.h
** Author:
** Copyright (C) 2019 Unisoc Communications Inc.
** Description:
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE				NAME				DESCRIPTION
** 2020/07/23		mark.zhang1			Create
******************************************************************************/

#ifndef _MMISFR_MAIN_API_H_
#define _MMISFR_MAIN_API_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Retry when failed
//  Param : @is_in_register: current process, register or update
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_RetryRegister(BOOLEAN is_in_register);

/*****************************************************************************/
//  Description :Retry to update when successful
//  Param :@first_update: Time interval is different when first
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMISFR_RetryUpdate(BOOLEAN first_update);

#endif

