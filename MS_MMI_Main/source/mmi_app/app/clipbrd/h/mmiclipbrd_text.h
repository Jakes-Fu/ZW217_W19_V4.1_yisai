/***************************************************************************
** File Name:      mmiclipbrd_text.h                                       *
** Author:         hua.fang                                                *
** Date:           06/05/2010                                              *
** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 05/2010        hua.fang         Create                                  *
**																		   *
****************************************************************************/

#ifndef _MMICLIPBRD_TEXT_H_
#define _MMICLIPBRD_TEXT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "sci_types.h"
#include "mmi_module.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef _cplusplus
	extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
#define MACRO_MDU_TYPE  MACRO_MDU_TEXT
#include "macro_mdu_def.h"

#ifdef CODE_MATCH_RES_CHECK
#include "clipbrd_mdu_def.h"
#endif

typedef enum MMICLIPBRD_TEXT_ID_E
{
	MMICLIPBRD_TEXT_NULL = (MMI_MODULE_CLIPBRD << 16) | MMI_SYS_RES_FLAG,
#ifndef CODE_MATCH_RES_CHECK
#include "clipbrd_mdu_def.h"
#endif
	MMICLIPBRD_TEXT_MAX
} MMICLIPBRD_TEXT_ID_E;

#undef MACRO_MDU_TYPE

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMICLIPBRD_TEXT_H_
