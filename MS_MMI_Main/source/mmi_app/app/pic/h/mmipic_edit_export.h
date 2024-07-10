/*****************************************************************************
** File Name:      mmipic_edit_export.h                                                  *
** Author:                                                                   *
** Date:           01/10/2008                                                *
** Copyright:      2008 preadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmipic_edit.c  mmipic_edit_wintab.c                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2008       Vincent.shang           Create
******************************************************************************/
#ifndef _MMIPIC_H_
#define _MMIPIC_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmifmm_export.h"


#include "systypes.h"


/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
    **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPIC_InitModule(void);
/*****************************************************************************/
//  Description : open pic editor main win
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
#ifdef PIC_EDITOR_SUPPORT
PUBLIC BOOLEAN MMIAPIPIC_OpenPicEditorMainWin(
                                              const wchar     *full_path_ptr,     //in
                                              uint16         full_path_len      //in:字节数
                                              );
#else
PUBLIC BOOLEAN MMIAPIPIC_OpenPicEditorMainWin(uint16          *full_path_ptr,     //in
                                              uint16         full_path_len      //in:字节数
                                              );
#endif
/*****************************************************************************/
//  Description : exit from pic editor
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPIC_ExitFromPicEditor(void);

#endif

