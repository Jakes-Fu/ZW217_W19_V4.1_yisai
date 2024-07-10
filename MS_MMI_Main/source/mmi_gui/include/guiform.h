/*****************************************************************************
** File Name:      guiform.h                                                 *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      Jassmine              Creat
******************************************************************************/

#ifndef _GUIFORM_H_
#define _GUIFORM_H_ 

/*---------------------------------------------------------------------------
                          Include Files                                    
 --------------------------------------------------------------------------*/
#include "ctrlform_export.h"

/*---------------------------------------------------------------------------
                         Compiler Flag                                    *
 --------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------
                          MACRO DEFINITION                                 
 --------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                          TYPE AND STRUCT                                  
--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/
#define GUIFORM_ParseChild                  CTRLFORM_ParseChild
#define GUIFORM_SetActiveChild              CTRLFORM_SetActiveChild
#define GUIFORM_SetResetSlideFrame          CTRLFORM_SetResetSlideFrame
#define GUIFORM_SetRect                     CTRLFORM_SetRect
#define GUIFORM_SetVisible                  CTRLFORM_SetVisible
#define GUIFORM_GetType                     CTRLFORM_GetType
#define GUIFORM_SetType                     CTRLFORM_SetType
#define GUIFORM_GetStyle                    CTRLFORM_GetStyle
#define GUIFORM_SetStyle                    CTRLFORM_SetStyle
#define GUIFORM_SetBg                       CTRLFORM_SetBg
#define GUIFORM_SetSaveBg                   CTRLFORM_SetSaveBg
#define GUIFORM_SetMargin                   CTRLFORM_SetPaddingEx
#define GUIFORM_GetChildCtrlId              CTRLFORM_GetChildCtrlId
#define GUIFORM_SetDividingLine             CTRLFORM_SetDividingLine
#define GUIFORM_SetSpace                    CTRLFORM_SetSpace
#define GUIFORM_SetChildSpace               CTRLFORM_SetChildSpace
#define GUIFORM_SetChildWidth               CTRLFORM_SetChildWidth
#define GUIFORM_SetChildHeight              CTRLFORM_SetChildHeight
#define GUIFORM_SetChildTheme               CTRLFORM_SetChildTheme
#define GUIFORM_SetActiveChildTheme         CTRLFORM_SetActiveChildTheme
#define GUIFORM_SetGrayChildTheme           CTRLFORM_SetGrayChildTheme
#define GUIFORM_SetAlign                    CTRLFORM_SetAlign
#define GUIFORM_SetChildAlign               CTRLFORM_SetChildAlign
#define GUIFORM_GetChildDisplay             CTRLFORM_GetChildDisplay
#define GUIFORM_SetChildDisplay             CTRLFORM_SetChildDisplay
#define GUIFORM_SetChildDisplayOnly         CTRLFORM_SetChildDisplayOnly
#define GUIFORM_PermitChildBg               CTRLFORM_PermitChildBg
#define GUIFORM_PermitChildBorder           CTRLFORM_PermitChildBorder
#define GUIFORM_PermitChildFont             CTRLFORM_PermitChildFont
#define GUIFORM_PermitChildFontColor    CTRLFORM_PermitChildFontColor
#define GUIFORM_IsSlide                     CTRLFORM_IsSlide
#define GUIFORM_IsAutoDir                   CTRLFORM_IsAutoDir
#define GUIFORM_SetCircularHandleUpDown     CTRLFORM_SetCircularHandleUpDown
#define GUIFORM_SetCircularHandleLeftRight  CTRLFORM_SetCircularHandleLeftRight
#define GUIFORM_GetHeight                   CTRLFORM_GetHeight
#define GUIFORM_CreatDynaCtrl               CTRLFORM_CreatDynaCtrl
#define GUIFORM_CreatDynaChildCtrl          CTRLFORM_CreatDynaChildCtrl
#define GUIFORM_DestroyDynaChildCtrl        CTRLFORM_DestroyDynaChildCtrl
#define GUIFORM_SetChildDock                CTRLFORM_SetChildDock
#define GUIFORM_SetSizeRange                CTRLFORM_SetSizeRange
#define GUIFORM_ForceLayout                 CTRLFORM_ForceLayout
//zmt add start
#define GUIFORM_SetDisplayScrollBar         CTRLFORM_SetDisplayScrollBar
//zmt add end
#ifdef GUI_DUMP_CTRL_TREE
#define GUIFORM_DumpTree                    CTRLFORM_DumpTree
#endif

/**--------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
