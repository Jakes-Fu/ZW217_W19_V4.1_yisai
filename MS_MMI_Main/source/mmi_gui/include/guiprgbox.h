/*! @file guiprgbox.h
@brief 此文件介绍了prgbox控件的对外接口和数据类型 
@author Jassmine
@version 1.0
@date 01/10/2007
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 01/2007      Jassmine              Creat
*******************************************************************************/

/*! @page page_PrgBox 滚动条和进度条(PrgBox)

-# @ref sec_prgbox_function
-# @ref sec_prgbox_create
-# @ref sec_prgbox_app

@section sec_prgbox_function 控件功能

- Prgbox即滚动条和进度条控件，其中prg是progress的缩写。在显示文本的时候，如果显示超过一页（即屏幕所能显示内容），通常使用滚动条控件提醒用户——本页没有显示完整，可以使用上下键切换上下页或下一行。
- 另外，当传输文件时，可以使用进度条显示文件传输的进度，例如蓝牙传输文件，web下载等。
- 滚动条和进度条控件作为滚动条使用时，一般内嵌在List、Menu、Dropdownlist控件等其他控件中，用户无需关心滚动条的使用。当然，用户也可以单独使用。
- 作为进度条控件，一般被单独应用在窗口中，例如表示下载文件的进度。
- 滚动条既可以是横向的，也可以是纵向的，它能够响应触笔消息，设计时，为滚动条设置了上下箭头（横向滚动条为左右箭头）。例如，当触笔点击上下箭头的时候，可以切换列表的上一条或下一条记录。当触笔点击在滚动条区域中滑块以外的区域时，可以切换上下页，还可以触笔拖动滑块。

@section sec_prgbox_create 控件的创建

滚动条和进度条控件（以下简称Prg）一般情况下用来嵌入其他控件中，例如Textbox、Editbox、menu、list等，不需要用户自己创建。如果用户需要单独使用该控件，创建时分为动态创建和静态创建，其创建方式和其他控件相同。
创建之后，根据需求设置相关的参数就可以了，这个控件的接口相对来说很少，也比较简单。

@section sec_prgbox_app 控件应用
下图为滚动条控件的使用：
@image html prgbox_display.jpg
图 2 42 控件显示例图
*******************************************************************************/

/*! @addtogroup groupGrp PrgBox
@brief group模块
@details group模块
@{
*******************************************************************************/

#ifndef _GUIPRGBOX_H_
#define _GUIPRGBOX_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmitheme_prgbox.h"
#include "guictrl.h"

#include "ctrlprogressbar_export.h"
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
        
/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
 typedef CTRLPRGBOX_NOTIFY_T GUIPRGBOX_NOTIFY_T;
/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUIPRGBOX_SetPosByCtrl              CTRLPROGRESSBAR_SetPos
#define GUIPRGBOX_UpdateByCtrl              CTRLPROGRESSBAR_Update
#define GUIPRGBOX_SetParamByCtrl            CTRLPROGRESSBAR_SetParam
#define GUIPRGBOX_SetBgColor                CTRLPROGRESSBAR_SetBgColor
#define GUIPRGBOX_SetFobidThumb             CTRLPROGRESSBAR_SetFobidThumb
#define GUIPRGBOX_SetNeedSlideBlockByCtrl   CTRLPROGRESSBAR_SetNeedSlideBlock
#define GUIPRGBOX_SetVerGrooveBgImg         CTRLPROGRESSBAR_SetVerGrooveBgImg
#define GUIPRGBOX_SetScrollBarImg           CTRLPROGRESSBAR_SetScrollBarImg
#define GUIPRGBOX_SetRect                   CTRLPROGRESSBAR_SetRect

/*****************************************************************************/
//  Description : set position by pointer
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetPosByPtr (
    BOOLEAN      is_update,      //is update
    uint32       cur_item_index,
    uint32       first_item_index,
    IGUICTRL_T   *prgbox_ctrl_ptr
);

/*****************************************************************************/
// Description : this function only for progress bar
// Global resource dependence : none
// Author: allen
// Note: 2005.01.11
/*****************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_GetRectByPtr (
    IGUICTRL_T      *prgbox_ctrl_ptr,
    GUI_RECT_T      *rect
);

/*****************************************************************************/
// Description :
// Global resource dependence : none
// Author: allen
// Note: 2005.02.02
/*****************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_ResizeRectByPtr (
    IGUICTRL_T      *prgbox_ctrl_ptr,
    GUI_RECT_T      new_rect  // the new total rectangle
);

/***************************************************************************//*!
@brief 根据控件信息刷新
@author Jassmine
@param  prgbox_ctrl_ptr        [in] 滚动条控件id
@return 无
@note 
*******************************************************************************/
void GUIPRGBOX_UpdateByPtr(
                           IGUICTRL_T *prgbox_ctrl_ptr
                           );

 /***************************************************************************//*!
@brief 根据控件id设置item总数和每页item个数
@author guanyong.zhang
@param      prgbox_ctrl_ptr      [in]   控件数据信息
@param      total_item_count     [in]   item总数量                 
@param      items_per_page       [in]   每页item个数
@return 无
@note 
*******************************************************************************/
PUBLIC void GUIPRGBOX_SetParamByPtr(
                                    IGUICTRL_T          *prgbox_ctrl_ptr,
                                    uint32              total_item_count,
                                    uint32              items_per_page
                                    );

 /***************************************************************************//*!
@brief 根据控件数据信息设置是否有循环滚动条
@author guanyong.zhang
@param      prgbox_ctrl_ptr     [in]    控件数据信息
@param      is_loop_scroll      [in]    是否设置循环滚动条               
@return 无
@note 
*******************************************************************************/

PUBLIC void GUIPRGBOX_SetLoopScrollByPtr(
                                         IGUICTRL_T       *prgbox_ctrl_ptr,
                                         BOOLEAN          is_loop_scroll
                                         );

/***************************************************************************//*!
@brief 根据数据信息设置是否需要滑块
@author xiaoqing.lu
@param      prgbox_ctrl_ptr     [in]    控件数据信息
@param      is_true             [in]    是否需要设置滑块
@return 返回设置前的数据
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_SetNeedSlideBlock(
                                           IGUICTRL_T           *prgbox_ctrl_ptr,
                                           BOOLEAN              is_true
                                           );

/***************************************************************************//*!
@brief 根据控件数据信息判断是否需要设置背景
@author xiaoqing.lu
@param      prgbox_ctrl_ptr [in]    控件数据信息
@param      is_true         [in]    是否需要设置背景
@return 返回设置前的数据
@note 
*******************************************************************************/
PUBLIC BOOLEAN GUIPRGBOX_SetNeedBg(
                                   IGUICTRL_T           *prgbox_ctrl_ptr,
                                   BOOLEAN              is_true
                                   );

/***************************************************************************//*!
@brief 根据控件数据信息设置背景颜色
@author guanyong.zhang
@param      prgbox_ctrl_ptr [in]    控件数据信息
@param      bg_color        [in]    背景颜色
@return 无
@note 
*******************************************************************************/
PUBLIC void GUIPRGBOX_SetBgColorByPtr(
                                      IGUICTRL_T        *prgbox_ctrl_ptr,
                                      GUI_COLOR_T       bg_color
                                      );

/***************************************************************************//*!
@brief 获取当前滚动条的位置所对应字母的unicode
@author 
@param      prgbox_ctrl_ptr：［IN]滚动条的控件指针
@param      item_pos：滚动条的当前位置
@return 返回item_pos所对应的字母的unicode
@note 
*******************************************************************************/
PUBLIC wchar  GUIPRGBOX_GetWcharByPos (
                                       IGUICTRL_T * prgbox_ctrl_ptr,          
                                       uint16  item_pos
                                       );

/*****************************************************************************/
//  Description : set parameter: total item count and item count per page
//  Global resource dependence :
//  Author: guanyong.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetBothRectByPtr(
    IGUICTRL_T          *prgbox_ctrl_ptr,
    GUI_BOTH_RECT_T     *both_rect_ptr
);

/*****************************************************************************/
//  Description : 设置是否需要滑块
//  Global resource dependence :
//  Author: xiaoqing.lu
//  Note:返回设置前的数据
/*****************************************************************************/
PUBLIC void GUIPRGBOX_SetFobidThumbByPtr(
										 IGUICTRL_T *prgbox_ctrl_ptr
										 );

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
/*@}*/
