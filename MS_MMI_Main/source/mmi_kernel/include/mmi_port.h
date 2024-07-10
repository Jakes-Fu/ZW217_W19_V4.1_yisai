/*******************************************************************************
 ** File Name:      mmk_port.h                                                 *
 ** Author:         Great.Tian                                                 *
 ** Date:           14/01/2004                                                 *
 ** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.          *
 ** Description:    This file is used to describe the data struct of           *
 **                 system, applicaiton, window and control                    *
 *******************************************************************************
 **                         Important Edit History                             *
 ** ---------------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                                *
 ** 01/2003        Great.Tian       Create									   * 
 ******************************************************************************/
  
/*!
 *  \addtogroup mmi_port_group_label
 *  @{
 */
 
/*! 
 *  \file 	mmi_port.h
 *  \author	Great.Tian
 *  \date	Oct,2004
 *  \brief 	This file is used to describe the data struct of system, applicaiton, window and control
 */
 
#ifndef MMI_PORT_H
#define MMI_PORT_H

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                      	  */
/*----------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_app.h"

/*----------------------------------------------------------------------------*/
/*                         Functions                                          */
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/*! \brief  get the number of the reg application
 *  \author Great.Tian
 *  \return the number of the reg applicaion.
 */
/******************************************************************************/
uint32 MMI_GetRegAppNum(void);

/******************************************************************************/
/*! \brief  get the total number of the window id
 *  \author Great.Tian
 *  \return the total number of the window id.
 */
/******************************************************************************/
MMI_WIN_ID_T MMI_GetWinIdNumber(void);

/******************************************************************************/
/*! \brief  get the total number of the control id
 *  \author Great.Tian
 *  \return the total number of the control id.
 */
/******************************************************************************/
MMI_CTRL_ID_T MMI_GetCtrlIdNumber(void);

#endif /* MMI_PORT_H */
/*! @} */

