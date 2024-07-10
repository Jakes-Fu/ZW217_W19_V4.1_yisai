/*************************************************************************
** File Name:      mmisrvvp.h                                             *
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

#ifndef _MMISRV_DRM_H_
#define _MMISRV_DRM_H_

/*****************************************************************************/
//  Description : convert mime type
//  Global resource dependence : 
//  Author: xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC MMISRVAUD_RING_FMT_E MMIAPIDRM_ConvertAudioMimeType(
                            MIME_TYPE_E mime_type
                            );

#endif