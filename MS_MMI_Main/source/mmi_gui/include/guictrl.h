/*! @file guictrl.h
@brief 此文件介绍了抽象控件的对外接口和数据类型
@author Jassmine
@version 1.0
@date 04/26/2004  
@remarks Copyright: 2003 Spreadtrum, Incoporated. All Rights Reserved.
@note 11/2006      Jassmine              Creat
*******************************************************************************/

/*! @page page_Ctrl Ctrl

(暂无)

*******************************************************************************/

/*! @addtogroup ctrlGrp Ctrl
@brief 文件介绍了抽象控件的对外接口
@details ctrl模块
@{
*******************************************************************************/

#ifndef _GUICTRL_H_
#define _GUICTRL_H_ 

/*-------------------------------------------------------------------------
                         Include Files                                    
 --------------------------------------------------------------------------*/
#include "cafcontrol.h"
#include "mmk_type.h"
#include "ctrlinfo_export.h"

/*--------------------------------------------------------------------------
                          Compiler Flag                                   
--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                          MACRO DEFINITION                                
 --------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                         TYPE AND STRUCT                                 
--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
                         FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/

#define GUICTRL_QueryInterface                  VTLCTRL_QueryInterface            
#define GUICTRL_AddRef                          VTLCTRL_AddRef                    
#define GUICTRL_Release                         VTLCTRL_Release                   
#define GUICTRL_GetCtrlGuid                     VTLCTRL_GetCtrlGuid               
#define GUICTRL_SetCtrlHandle                   VTLCTRL_SetCtrlHandle             
#define GUICTRL_GetCtrlHandle                   VTLCTRL_GetCtrlHandle             
#define GUICTRL_SetState                        VTLCTRL_SetState                  
#define GUICTRL_GetState                        VTLCTRL_GetState                  
#define GUICTRL_SetRect                         VTLCTRL_SetRect                   
#define GUICTRL_GetRect                         VTLCTRL_GetRect                   
#define GUICTRL_SetLcdDevInfo                   VTLCTRL_SetLcdDevInfo             
#define GUICTRL_GetLcdDevInfo                   VTLCTRL_GetLcdDevInfo             
#define GUICTRL_SetProperties                   VTLCTRL_SetProperties             
#define GUICTRL_GetProperties                   VTLCTRL_GetProperties             
#define GUICTRL_TestProperties                  VTLCTRL_TestProperties            
#define GUICTRL_Redraw                          VTLCTRL_Redraw                    
#define GUICTRL_SetActive                       VTLCTRL_SetActive                 
#define GUICTRL_IsActive                        VTLCTRL_IsActive                  
#define GUICTRL_SetBorder                       VTLCTRL_SetBorder                 
#define GUICTRL_SetBg                           VTLCTRL_SetBg                     
#define GUICTRL_SetFont                         VTLCTRL_SetFont                   
#define GUICTRL_GetFont                         VTLCTRL_GetFont                   
#define GUICTRL_SetProgress                     VTLCTRL_SetProgress               
#define GUICTRL_SetCircularHandleUpDown         VTLCTRL_SetCircularHandleUpDown   
#define GUICTRL_SetCircularHandleLeftRight      VTLCTRL_SetCircularHandleLeftRight
#define GUICTRL_GetHeightByWidth                VTLCTRL_GetHeightByWidth          
#define GUICTRL_GetDisplayRect                  VTLCTRL_GetDisplayRect            
#define GUICTRL_SetDisplayRect                  VTLCTRL_SetDisplayRect            
#define GUICTRL_GetSelectRect                   VTLCTRL_GetSelectRect             
#define GUICTRL_GetSelectIndex                  VTLCTRL_GetSelectIndex
#define GUICTRL_SetSelectIndex                  VTLCTRL_SetSelectIndex
#define GUICTRL_GetBothRect                     VTLCTRL_GetBothRect               
#define GUICTRL_SetBothRect                     VTLCTRL_SetBothRect               

#define GUICTRL_SendNotify                      CTRLMSG_SendNotify                
#define GUICTRL_SendNotifyEx                    CTRLMSG_SendNotifyEx              
#define GUICTRL_PostNotify                      CTRLMSG_PostNotify                
#define GUICTRL_PostNotifyEx                    CTRLMSG_PostNotifyEx              




/*--------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
