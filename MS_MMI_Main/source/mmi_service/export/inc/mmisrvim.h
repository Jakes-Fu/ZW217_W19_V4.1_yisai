/*************************************************************************
** File Name:      mmisrvim.h                                             *
** Author:         bin.ji                                          *
** Date:           03/14/2003                                           *
** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:     This file declare the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 03/14/2003     mmisrveng.h        Create.                              *
*************************************************************************/

#ifndef _MMISRV_IM_H_
#define _MMISRV_IM_H_

/*****************************************************************************/
//  Description : get text's letters
//  Global resource dependence : none
//  Author: 
//  Note:   
/*****************************************************************************/
PUBLIC void MMIAPIIM_GetTextLetters(uint16 text_unicode, uint16 *letters_ptr);

#endif