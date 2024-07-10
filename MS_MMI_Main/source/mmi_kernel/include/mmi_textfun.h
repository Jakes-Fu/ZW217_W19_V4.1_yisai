/******************************************************************************
** File Name:      mmi_textfun.h                                              *
** Author:                                                                    *
** Date:           04/26/2004                                                 *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to process text of control               *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------* 
** DATE           NAME             DESCRIPTION                                *
** 04/26/2004     louis.wei          Creat									  *
******************************************************************************/
/*!
 *  \addtogroup mmk_textfun_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmi_textfun.h
 *  \author	louis.wei
 *  \date	April,2004
 *  \brief 	This file is used to describe the data struct of mmi text function
 */

#ifndef _MMI_TEXTFUN_H
#define _MMI_TEXTFUN_H

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h" 
#include "mmk_type.h"


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                    */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/*! \brief to get the text by label
 *  \author louis.wei
 *  \param[in]	label           the label defined in the mmi_text.def
 *  \return the pointer of text
 */ 
/******************************************************************************/
const wchar* MMI_GetLabelText(MMI_TEXT_ID_T label);

/******************************************************************************/
/*! \brief to get the text by label and language
 *  \author louis.wei
 *  \param[in]	label			the label defined in the mmi_text.def
 *  \param[out]	str_ptr         the data of text
 *  \return 
 */ 
/******************************************************************************/
void MMI_GetLabelTextByLang( 
                                MMI_TEXT_ID_T    	label,
                                MMI_STRING_T     	*str_ptr
                                );

/*****************************************************************************/
/*! \brief check the text whether is NULL
 *	\author wancan.you
 *  \param[in]	label			the label defined in the mmi_text.def
 *	\return 
 *		whether success
 */
/*****************************************************************************/
PUBLIC BOOLEAN MMI_IsTextNull(MMI_TEXT_ID_T label);
/*----------------------------------------------------------------------------*/
/*                         VARIABLES                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif /* KER_TEXT_H */

/*! @} */
