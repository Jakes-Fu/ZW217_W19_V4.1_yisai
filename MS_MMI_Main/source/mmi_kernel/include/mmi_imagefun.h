/******************************************************************************
** File Name:      mmi_imagefun.h                                             *
** Author:                                                                    *
** Date:           13/05/2004                                                 *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe image                        *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 13/05/2004       jibin          Create									  *
******************************************************************************/

/*!
 *  \addtogroup mmk_image_group_label
 *  @{
 */


/*! 
 *  \file 	mmi_imagefun.h
 *  \author	bin.ji
 *  \date	13/05/2004
 *  \brief 	This file is used to describe image
 */


#ifndef _MMI_IMAGEFUN_H
#define _MMI_IMAGEFUN_H

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
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/*! \brief get the image data
 *  \param[in]		label	image label id
 *  \author  bin.ji
 *  \return  uint8 const pointer,the image data pointer
 */
/******************************************************************************/
const uint8* MMI_GetLabelImage(MMI_IMAGE_ID_T label,MMI_WIN_ID_T win_id, uint32 *size);

/******************************************************************************/
/*! \brief get the anim data
 *  \param[in]		label	anim label id
 *  \author  bin.ji
 *  \return  uint8 const pointer,the anim data pointer
 */
/******************************************************************************/
PUBLIC const uint8* MMI_GetLabelAnim(
                                     MMI_ANIM_ID_T  label,      //in:
                                     MMI_WIN_ID_T   win_id,     //in:
                                     uint32         *size_ptr   //in:may PNULL
                                     );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus   
    }
#endif

#endif /* MMI_IMAGE_H */
/*! @} */ 

