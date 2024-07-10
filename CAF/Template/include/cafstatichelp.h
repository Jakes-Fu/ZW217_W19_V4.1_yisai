/****************************************************************************
** File Name:      cafstatichelp.h                                         *
** Author:         James.Zhang                                             *
** Date:           05/21/2008                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2008        James.Zhang       Create
** 
****************************************************************************/

#ifndef  _CAF_STATIC_HELP_H_    
#define  _CAF_STATIC_HELP_H_  

#ifdef CAF_DYANMIC_MODULE
#error this file is only for statice module !!!
#endif
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/*----------------------------------------------------------------------------*/
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/
#include "caf.h"
#include "os_api.h"
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/
#define CAF_ALLOC              SCI_ALLOC_APP
#define CAF_FREE               SCI_FREE
#define CAF_ASSERT             SCI_ASSERT
#define CAF_MEMSET             SCI_MEMSET
#define CAF_TRACE              SCI_TRACE_LOW
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
   }
#endif
#endif
