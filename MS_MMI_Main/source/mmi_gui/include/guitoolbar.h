/*! @file guitoolbar.h
@brief 此文件介绍了toolbar控件的对外接口和数据类型
@author James.Zhang
@version 1.0
@date 2011/02/18
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 9/2010         James.Zhang      Create
*******************************************************************************/

/*! @page page_ToolBar ToolBar

(暂无)

*******************************************************************************/

/*! @addtogroup ToolbarGrp ToolBar
@brief Toolbar控件
@details Toolbar控件有些类似menu。Toolbar使用一个长方形的小弹出窗口，排列一些命令选项。
@{
*******************************************************************************/

#ifndef _GUITOOLBAR_H_
#define _GUITOOLBAR_H_ 

/*--------------------------------------------------------------------------
                          Include Files                               
 --------------------------------------------------------------------------*/

#include "mmk_type.h"
#include "guifont.h"
#include "ctrltoolbar_export.h"

/*--------------------------------------------------------------------------
                         Compiler Flag                                   
 --------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                         Macro Declaration                                                                   
 --------------------------------------------------------------------------*/  

/*--------------------------------------------------------------------------
                         Data Structures                                  
 --------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                        Function Declare                             
 --------------------------------------------------------------------------*/
#define GUITOOLBAR_SetAllItem                   CTRLTOOLBAR_SetAllItem           
#define GUITOOLBAR_GetClickItemIndex            CTRLTOOLBAR_GetClickItemIndex    
#define GUITOOLBAR_GetItemPtr                   CTRLTOOLBAR_GetItemPtr           
#define GUITOOLBAR_GetItemUserDate              CTRLTOOLBAR_GetItemUserDate      
#define GUITOOLBAR_SetItemGray                  CTRLTOOLBAR_SetItemGray          
#define GUITOOLBAR_GetShowType                  CTRLTOOLBAR_GetShowType          
#define GUITOOLBAR_SetShowType                  CTRLTOOLBAR_SetShowType          
#define GUITOOLBAR_SetVisible                   CTRLTOOLBAR_SetVisible           
#define GUITOOLBAR_SetAutoShrink                CTRLTOOLBAR_SetAutoShrink        
#define GUITOOLBAR_SetAutoShrinkInterval        CTRLTOOLBAR_SetAutoShrinkInterval
#define GUITOOLBAR_SetPreExpandFunc             CTRLTOOLBAR_SetPreExpandFunc     
#define GUITOOLBAR_GetIconWidthHeight           CTRLTOOLBAR_GetIconWidthHeight   
#define GUITOOLBAR_SetIconRect                  CTRLTOOLBAR_SetIconRect          
#define GUITOOLBAR_SetPanelBasePt               CTRLTOOLBAR_SetPanelBasePt       
#define GUITOOLBAR_SetPanelPopForm              CTRLTOOLBAR_SetPanelPopForm      
#define GUITOOLBAR_CreateDynamic                CTRLTOOLBAR_CreateDynamic        

/*--------------------------------------------------------------------------
                         Compiler Flag                                   
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
