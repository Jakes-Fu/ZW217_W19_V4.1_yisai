/*************************************************************************
 ** File Name:      mmiim_touch_set.h                                    *
 ** Author:         Jassmine                                             *
 ** Date:           2011/12/14                                           *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.   *
 ** Description:    This file defines the function about set im          *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2011/12/14    Xinhe.Yan           Create.                            *
*************************************************************************/

#ifndef _MMIIM_SET_H_
#define _MMIIM_SET_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmiim_touch_common.h"

#if defined (MMI_IM_PDA_SUPPORT)

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//	Description: create layer for setting im 
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:istyle创建输入法选择界面，使用层来显示
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIM_CreateSettingIMLayer(
                                               IM_DATA_T *data_ptr,
                                               KEY_INFO_T *key_ptr
                                               );

/*****************************************************************************/
//	Description: release setting im layer
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:删除切换输入法创建的图层
/*****************************************************************************/
PUBLIC void MMIIM_ReleaseSettingIMLayer(IM_DATA_T *data_ptr);

/*****************************************************************************/
//	Description: destroy setting im data
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:release setting im layer and free s_setting_menu_index
/*****************************************************************************/
PUBLIC void MMIIM_DestroySettingIMData(IM_DATA_T *data_ptr);

/*****************************************************************************/
//	Description: handle setting im move
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:处理istyle切换输入法时的手势移动
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIM_HandleSettingIMMove(
                                              IM_DATA_T *data_ptr,
                                              GUIIM_EVENT_DATA_U const *event_data_ptr
                                              );

/*****************************************************************************/
//	Description: istyle handle set key up
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:处理输入法按键抬起
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIIM_HandleSettingIMUp(
                                            IM_DATA_T *data_ptr,
                                            GUIIM_EVENT_DATA_U const *event_data_ptr
                                            );

/*****************************************************************************/
//	Description: draw set im in istyle
//	Global resource dependence: NONE
//	Author: Xinhe.Yan
//	Note:if item height is 36(0-35),divide to(0-0)(1-34)(35-35) to display up border\item\down border
/*****************************************************************************/
PUBLIC void MMIIM_DrawSettingIm(
                                IM_DATA_T *    data_ptr,
                                KEY_INFO_T *   key_ptr,
                                BOOLEAN        is_highlight,  //weather heighlight current item
                                int16          highlight_index //index of item will highlight
                                );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif      //#if defined (MMI_IM_PDA_SUPPORT)

#endif      // (_MMIIM_SET_H_)

