/*****************************************************************************
** File Name:      appstore_bg_operation.h                                   *
** Author:         donglin.su                                                *
** Date:           11/1/2021                                                 *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to define app store bg operation api    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/1/2021      donglin.su       Create                                    *
******************************************************************************/

#ifndef _APPSTORE_BG_APERATION_H_
#define _APPSTORE_BG_APERATION_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
#include "ual_cms.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/
#define APPSTORE_CMD_LEN_MAX                (6)
#define APPSTORE_MSG_ID_LEN_MAX             (8)
#define APPSTORE_APP_ID_LEN_MAX             (50)
#define APPSTORE_URL_LEN_MAX                (128)
#define APPSTORE_SEND_MSG_HEAD_LEN          (6)


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DECLARATION                              *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : APPSTORE_BG_RegisterBLE
//  Parameter: [In]  none
//             [Out] none
//             [Return] Success:TRUE;Fail:FALSE
//  Author:donglin.su
//  Note:init watchface handle ble private message module
/*****************************************************************************/
PUBLIC BOOLEAN APPSTORE_BG_RegisterBLE(void);

#endif