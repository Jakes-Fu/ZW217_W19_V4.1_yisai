#ifndef _MMIBT_FILETRANSFER_H_ /*perl robot add*/
#define _MMIBT_FILETRANSFER_H_ /*perl robot add*/
/****************************************************************************
** File Name:      mmibt_filetransfer.h                                        *
** Author:         Wenming feng                                            *
** Date:           7/20/2012                                              *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the paired device list     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 7/20/2012    yuantao.xu          Create                                  *
**                                                                         *
****************************************************************************/
/**----------------------------------------------------------------------*
 **                         Include Files                                *
 **----------------------------------------------------------------------*/
#include "window_parse.h"
#include "mmibt_app.h"
#include "mmifmm_export.h"
#include "mmibt_id.h"
#include "mmibt_nv.h"
#include "bt_abs.h"
#include "mmi_filemgr.h"
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: get receive file result ind(OPP/FTP)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_ReceiveFileResInd(BT_MSG_T* msg_body_ptr, BOOLEAN is_fts);
/*****************************************************************************/
//  Discription: get send file result ind(OPP/FTP)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SendFileResInd(BT_MSG_T* msg_body_ptr);
/*****************************************************************************/
//  Discription: get packet received indication
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPacketReceivedInd(BT_MSG_T* msg_body_ptr);
/*****************************************************************************/
//  Discription: get packet send indication
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_GetPacketSentInd(BT_MSG_T* msg_body_ptr);
/*****************************************************************************/
//  Discription: get receive file result ind(OPP/FTP)
//  Global resource dependence: None
//  Author: kelly.li 
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIBT_SetSendFileEndTimer(void);

#endif  /*perl robot add*/

