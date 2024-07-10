/***************************************************************************
** File Name:      hellosprd.h                                             *
** Author:                                                                 *
** Date:           4/01/2009                                               *
** Copyright:      2009 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the data struct of        *
**                 system, application, window and control                 *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 4/2009         Xiaoqing.Lu      Create                                  *
**																		   *
****************************************************************************/
#ifndef _HELLO_SPRD_H_
#define _HELLO_SPRD_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

#include "sci_types.h"
#include "mmk_type.h"

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

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ����list���Դ���
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateListWin(void);

/*****************************************************************************/
//  Description : ����Textbox���Դ���
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateTextboxWin(void);

/*****************************************************************************/
//  Description : ����Button���Դ���
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateButtonWin(void);

/*****************************************************************************/
//  Description : ����Editbox����������
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateEditboxMainWin(void);

/*****************************************************************************/
//  Description : ����dropdownlist���Դ���
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateDropdownlistWin(void);

/*****************************************************************************/
//  Description : ����richtext���Դ���
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMIHELLO_CreateRichtextWin(void);

/*****************************************************************************/
// 	Description : creat form win msg
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIHELLO_CreateFormWin(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIHELLO_CreateSetlistWin(void);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif//_HELLO_SPRD_H_