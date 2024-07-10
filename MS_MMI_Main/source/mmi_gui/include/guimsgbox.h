/*! @file guimsgbox.h
@brief ���ļ�������msgbox�ؼ��Ķ���ӿں���������
@author panda.luo 
@version 1.0
@date 16/04/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note  04/2004       panda.luo          Create   
*******************************************************************************/

/*! @page page_MsgBox MsgBox

(����)

*******************************************************************************/

/*! @addtogroup  msgboxGrp MsgBox
@brief  ���ļ�������msgbox�ؼ��Ķ���ӿں���������
@details  ���ļ�������msgbox�ؼ��Ķ���ӿں���������
@{
*******************************************************************************/



#ifndef _GUIMSGBOX_H_
#define _GUIMSGBOX_H_

/*--------------------------------------------------------------------------*/
/*                         Include Files                                    */
/*--------------------------------------------------------------------------*/
#include    "guicommon.h"
#include    "mmk_type.h"
#include    "guianim.h"
#include    "guisoftkey.h"
#include 	"guilcd.h"

#include "ctrlmsgbox_export.h"

/*--------------------------------------------------------------------------*/
/*                         Compiler Flag                                    */
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/*--------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                 */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                         FUNCTION DEFINITION                              */
/*--------------------------------------------------------------------------*/
#define GUIMSGBOX_SetParam          CTRLMSGBOX_SetParam
#define GUIMSGBOX_SetSoftkey        CTRLMSGBOX_SetSoftkey
#define GUIMSGBOX_SetImageAnim      CTRLMSGBOX_SetImageAnim
#define GUIMSGBOX_SetText           CTRLMSGBOX_SetText
#define GUIMSGBOX_SetBackground     CTRLMSGBOX_SetBackground
#define GUIMSGBOX_SetTitleText      CTRLMSGBOX_SetTitleText
#define GUIMSGBOX_SetTitleParam     CTRLMSGBOX_SetTitleParam
#define GUIMSGBOX_SetFontParam      CTRLMSGBOX_SetFontParam
#define GUIMSGBOX_Layout            CTRLMSGBOX_Layout
#define GUIMSGBOX_Update            CTRLMSGBOX_Update

/*--------------------------------------------------------------------------------*/
/*                         Compiler Flag                                          */
/*--------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
