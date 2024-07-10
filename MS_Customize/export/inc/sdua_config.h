/******************************************************************************
 ** File Name:      sdua_config.h                                             *
 ** Author:         Zhang Hanbing                                             *
 ** DATE:           25/11/2009                                                *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    used for mbbms ca sd card rw function set                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 25/11/2009     Zhang Hanbing        Create                                *
 ******************************************************************************/

  
#ifndef _SDUA_CONFIG_H
#define _SDUA_CONFIG_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "card_sdio.h"
#include "scm_api.h"

#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/




/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//! \fn PUBLIC BOOLEAN SDUACFG_Is_Init(void)
//! \param void
//! \return TRUE init 
//! \brief  Description: This Function is used to get ca sd power init status
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC BOOLEAN SDUACFG_Is_Init(void);

/*****************************************************************************/
//! \fn PUBLIC void SDUACFG_Set_Init_Status(BOOLEAN is_init )
//! \param is_init
//! \return void
//! \brief  Description: This Function is used to set ca sd power init status
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC void SDUACFG_Set_Init_Status(BOOLEAN is_init );

/*****************************************************************************/
//! \fn BOOLEAN SDUACFG_IsMbbmsca(void)
//! \param void
//! \return TRUE is ca card
//! \brief  Description: This Function is used to judeg whether is mbbms ca card
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC BOOLEAN SDUACFG_IsMbbmsca(void);

/*****************************************************************************/
//! \fn PUBLIC void SDUACFG_GetPnm(uint8* pnm)
//! \param pnm
//! \return void
//! \brief  Description: This Function is used to get sd pnm
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC void SDUACFG_GetPnm(uint8* pnm);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN SDUACFG_IOCTL(SCM_SLOT_NAME_E slot_name,SCM_FUN_E cmd,SCM_PARAM_T* param)
//! \param is_init
//! \return void
//! \brief  Description: This Function is used to sd IO before init
//! \author Author: Zhang Hanbing
//! \note   Note:  
/*****************************************************************************/
PUBLIC BOOLEAN SDUACFG_IOCTL(SCM_SLOT_NAME_E slot_name,SCM_FUN_E cmd,SCM_PARAM_T* param);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif  
