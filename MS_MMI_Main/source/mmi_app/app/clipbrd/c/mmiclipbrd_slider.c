/*****************************************************************************
** File Name:      mmiclipbrd_blockslider.c                                  *
** Author:         hua.fang                                                  *
** Date:           08/14/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:                                                              *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011        hua.fang          Create                                   *
******************************************************************************/

#include "mmiclipbrd_internal.h"

#ifdef GUIF_CLIPBOARD
/**--------------------------------------------------------------------------*/
/**                         Include Files                                    */
/**--------------------------------------------------------------------------*/
#include "window_parse.h"
#include "guictrl_api.h"
#include "mmiclipbrd_id.h"
#include "mmiclipbrd_text.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guiownerdraw.h"
#include "mmitheme_pos.h"
#include "mmitheme_clipbrd.h"
#include "mmiclipbrd_image.h"
#include "guires.h"
#include "guitext.h"
#include "guiedit.h"
#include "guirichtext.h"

/**--------------------------------------------------------------------------*/
/**                         MACRO DEFINITION                                 */
/**--------------------------------------------------------------------------*/
#define MMICLIPBRD_TP_OFFSET        10
#define MMICLIPBRD_SLIDER_OFFSET    10

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// ������Ϣ
typedef struct
{
    BOOLEAN                     is_visible;         // �Ƿ�ɼ�
    GUI_SIZE_T                  size;               // ����λ��
    MMITHEME_SLIDER_TYPE_E      slider_type;        // ��������
    MMI_IMAGE_ID_T              image_id;           // ����Ӧ��ͼƬ
    GUI_LCD_DEV_INFO            lcd_dev_info;       // lcd dev
    MMI_WIN_ID_T                win_id;             // ���ڴ���
    MMI_HANDLE_T                dst_ctrl_handle;    // Ŀ��ؼ�
    MMI_HANDLE_T                dummy_ctrl_handle;  // ���ڻ�ȡ�����Ϣ��ownerdraw�ؼ�
    int16                       x_offset;           // xƫ��
    int16                       y_offset;           // yƫ��
} GUI_SLIDER_DATA_T;

/*---------------------------------------------------------------------------*/
/*                          CONSTANT ARRAY                                   */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         EXTERNAL DECLARE                                 */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*/
/**                         STATIC DEFINITION                                */
/**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : �����ؼ�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T CreateDummyCtrl(
                                  MMITHEME_SLIDER_TYPE_E   slider_type,
                                  MMI_HANDLE_T   win_handle,
                                  MMI_HANDLE_T   ctrl_handle,
                                  ADD_DATA   add_data
                                  );

/*****************************************************************************/
//  Description : ���ƺ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void OwnerDrawFunc(
                         GUIOWNDRAW_INFO_T *owner_draw_ptr
                         );

/*****************************************************************************/
//  Description : APPEND��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateSliderLayer(
                                GUI_SLIDER_DATA_T   *slider_data_ptr
                                );

/*****************************************************************************/
//  Description : APPEND��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendSliderLayer(
                             GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                             );

/*****************************************************************************/
//  Description : ���»���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void UpdateSlider(
                         GUI_SLIDER_DATA_T   *slider_data_ptr
                         );

/*****************************************************************************/
//  Description : ����TP DOWN
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpDown(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                );

/*****************************************************************************/
//  Description : ����TP SHORT
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpShort(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                );

/*****************************************************************************/
//  Description : ����TP MOVE
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpMove(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                );

/*****************************************************************************/
//  Description : ����TP LONG
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpLong(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                );

/*****************************************************************************/
//  Description : ����TP UP
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpUp(
                              MMI_HANDLE_T ctrl_handle,
                              GUI_POINT_T tp_point
                              );

/*****************************************************************************/
//  Description : ����lose focus
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoseFocus(
                                   MMI_HANDLE_T ctrl_handle
                                   );

/*****************************************************************************/
//  Description : ����get foucs
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGetFocus(
                                  MMI_HANDLE_T ctrl_handle
                                  );

LOCAL BOOLEAN GetPosition(
                          GUI_SLIDER_DATA_T *slider_data_ptr,
                          GUI_RECT_T        *rect_ptr
                          );

/**--------------------------------------------------------------------------*/
/**                         GLOBAL DEFINITION                                */
/**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : ��������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICLIPBRD_CreateSlider(
                                      MMITHEME_SLIDER_TYPE_E   slider_type,
                                      MMI_WIN_ID_T             win_id,
                                      MMI_HANDLE_T             ctrl_handle
                                      )
{
    BOOLEAN             result = FALSE;
    GUI_SLIDER_DATA_T   *slider_data_ptr = PNULL;
    MMI_IMAGE_ID_T      slider_image[MMITHEME_SLIDER_TYPE_MAX] = {      // ������������Ӧ��ͼƬ
        IMAGE_SLIDER_BLOCK,
        IMAGE_SLIDER_LEFT_SEL,
        IMAGE_SLIDER_RIGHT_SEL
        };

    // alloc memory to store data which will be used when show menu.
    slider_data_ptr = (GUI_SLIDER_DATA_T*)SCI_ALLOC_APP(sizeof(GUI_SLIDER_DATA_T));
    SCI_MEMSET(slider_data_ptr, 0, sizeof(GUI_SLIDER_DATA_T));

    slider_data_ptr->slider_type = slider_type;
    slider_data_ptr->image_id = slider_image[slider_type];
    slider_data_ptr->dst_ctrl_handle = ctrl_handle;
    slider_data_ptr->win_id = win_id;

    // �������ܿؼ���ֻ���ڽ���TP��Ϣ
    slider_data_ptr->dummy_ctrl_handle = CreateDummyCtrl(
            slider_data_ptr->slider_type,
            win_id, ctrl_handle, slider_data_ptr);
    if (0 == slider_data_ptr->dummy_ctrl_handle)
    {
        goto _Exit_;
    }

    result = GUIRES_GetImgWidthHeight(
            (uint16*)&slider_data_ptr->size.w,
            (uint16*)&slider_data_ptr->size.h,
            slider_data_ptr->image_id,
            win_id);
    if (!result)
    {
        goto _Exit_;
    }

    return (uint32)slider_data_ptr;

_Exit_:
    if (0 != slider_data_ptr->dummy_ctrl_handle)
    {
        MMK_DestroyControl(slider_data_ptr->dummy_ctrl_handle);
    }

    SCI_FREE(slider_data_ptr);

    return PNULL;
}

/*****************************************************************************/
//  Description : ���ٻ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_DestroySlider(
                                     uint32    slider_handle
                                     )
{
    GUI_SLIDER_DATA_T   *slider_data_ptr = (GUI_SLIDER_DATA_T*)slider_handle;

    if (PNULL != slider_data_ptr)
    {
        if (0 != slider_data_ptr->lcd_dev_info.block_id
            && UILAYER_NULL_HANDLE != slider_data_ptr->lcd_dev_info.block_id)
        {
            UILAYER_RELEASELAYER(&slider_data_ptr->lcd_dev_info);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(&slider_data_ptr->lcd_dev_info);
        }

        if (0 != slider_data_ptr->dummy_ctrl_handle)
        {
            MMK_DestroyControl(slider_data_ptr->dummy_ctrl_handle);
        }

        SCI_FREE(slider_data_ptr);
    }
}

/*****************************************************************************/
//  Description : ��ʾ����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_ShowSlider(
                                  uint32   slider_handle,
                                  int16    x,
                                  int16    y
                                  )
{
    BOOLEAN             result = FALSE;
    //uint16              pos = 0;
    GUI_RECT_T          cursor_rect = {0};
    GUI_RECT_T          ctrl_rect = {0};
    GUI_RECT_T          slider_rect = {0};
    GUI_RECT_T          temp_rect = {0};
    GUI_SLIDER_DATA_T   *slider_data_ptr = (GUI_SLIDER_DATA_T*)slider_handle;

    if (PNULL != slider_data_ptr)
    {
        // �ж��Ƿ���Ҫ��ʾ
        if (GetPosition(slider_data_ptr, &cursor_rect))
        {
            if (!GUIAPICTRL_GetDisplayRect(slider_data_ptr->dst_ctrl_handle, &ctrl_rect))
            {
                GUIAPICTRL_GetRect(slider_data_ptr->dst_ctrl_handle, &ctrl_rect);
            }

            // �����λ��
            if (MMITHEME_SLIDER_TYPE_LEFT_SELECTOR == slider_data_ptr->slider_type)
            {
                slider_rect.left = cursor_rect.left - slider_data_ptr->size.w + MMICLIPBRD_SLIDER_OFFSET;
            }
            else if (MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR == slider_data_ptr->slider_type)
            {
                slider_rect.left = cursor_rect.left - MMICLIPBRD_SLIDER_OFFSET;
            }
            else
            {
                slider_rect.left = (cursor_rect.left + cursor_rect.right - slider_data_ptr->size.w) / 2;
            }
            slider_rect.top = cursor_rect.bottom;
            slider_rect.right = slider_rect.left + slider_data_ptr->size.w - 1;
            slider_rect.bottom = slider_rect.top + slider_data_ptr->size.h - 1;

            if (/*GUI_IntersectRect(&temp_rect, ctrl_rect, slider_rect)
                && */GUI_IntersectRect(&temp_rect, ctrl_rect, cursor_rect))
            {
                // ������ʼλ��
                slider_data_ptr->size.x = slider_rect.left;
                slider_data_ptr->size.y = slider_rect.top;

                // ����ͼ��
                result = CreateSliderLayer(slider_data_ptr);
                if (!result)
                {
                    return;
                }

                // ���ÿ��ܿؼ�����Ϣ
                ctrl_rect.left = slider_data_ptr->size.x;
                ctrl_rect.top = slider_data_ptr->size.y;
                ctrl_rect.right = ctrl_rect.left + slider_data_ptr->size.w - 1;
                ctrl_rect.bottom = ctrl_rect.top + slider_data_ptr->size.h - 1;

                GUIAPICTRL_SetLcdDevInfo(slider_data_ptr->dummy_ctrl_handle, &slider_data_ptr->lcd_dev_info);
                GUIAPICTRL_SetState(slider_data_ptr->dummy_ctrl_handle, GUICTRL_STATE_INVISIBLE, FALSE);
                GUIAPICTRL_SetRect(slider_data_ptr->dummy_ctrl_handle, &ctrl_rect);
                GUIAPICTRL_SetDisplayRect(slider_data_ptr->dummy_ctrl_handle, &ctrl_rect, FALSE);

                // ������ӽ�ȥ
                AppendSliderLayer(&slider_data_ptr->lcd_dev_info);

                UILAYER_SetLayerPosition(
                        &slider_data_ptr->lcd_dev_info,
                        ctrl_rect.left,
                        ctrl_rect.top);

                // ���ƻ���
                UpdateSlider(slider_data_ptr);

                slider_data_ptr->is_visible = TRUE;
            }
            else
            {
                MMITHEME_HideSlider(slider_handle);
            }
        }
        else
        {
            MMITHEME_HideSlider(slider_handle);
        }
    }
}

/*****************************************************************************/
//  Description : ���ػ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_HideSlider(
                                  uint32   slider_handle
                                  )
{
    GUI_SLIDER_DATA_T   *slider_data_ptr = (GUI_SLIDER_DATA_T*)slider_handle;

    if (PNULL != slider_data_ptr && slider_data_ptr->is_visible)
    {
        GUIAPICTRL_SetState(slider_data_ptr->dummy_ctrl_handle, GUICTRL_STATE_INVISIBLE, TRUE);

        if (0 != slider_data_ptr->lcd_dev_info.block_id
            && UILAYER_NULL_HANDLE != slider_data_ptr->lcd_dev_info.block_id)
        {
            UILAYER_RELEASELAYER(&slider_data_ptr->lcd_dev_info);   /*lint !e506 !e774*/
//            UILAYER_ReleaseLayer(&slider_data_ptr->lcd_dev_info);
//            slider_data_ptr->lcd_dev_info.block_id = UILAYER_NULL_HANDLE;
        }

        slider_data_ptr->is_visible = FALSE;
    }
}

/*****************************************************************************/
//  Description : ���»���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void UpdateSlider(
                         GUI_SLIDER_DATA_T   *slider_data_ptr
                         )
{
    GUI_POINT_T pt = {0};

    if (PNULL != slider_data_ptr)
    {
        UILAYER_Clear(&slider_data_ptr->lcd_dev_info);

        pt.x = slider_data_ptr->size.x;
        pt.y = slider_data_ptr->size.y;

        GUIRES_DisplayImg(
                &pt, PNULL, PNULL,
                slider_data_ptr->win_id,
                slider_data_ptr->image_id,
                &slider_data_ptr->lcd_dev_info);
    }
}

/*****************************************************************************/
//  Description : �����ؼ�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_HANDLE_T CreateDummyCtrl(
                                  MMITHEME_SLIDER_TYPE_E   slider_type,
                                  MMI_HANDLE_T   win_handle,
                                  MMI_HANDLE_T   ctrl_handle,
                                  ADD_DATA   add_data
                                  )
{
    MMI_CONTROL_CREATE_T        ctrl_create = {0};
    GUIOWNDRAW_INIT_DATA_T      init_data = {0};
    MMI_CTRL_ID_T               ctrl_id = 0;
    IGUICTRL_T                  *gui_ctrl = PNULL;
    GUI_BG_T                    bg = {0};
    GUIOWNDRAW_TP_HANDLER_T     tp_handler = {0};

    // ��ʼ������
    init_data.both_rect   = MMITHEME_GetWinClientBothRect(win_handle);
    init_data.OwnDrawFunc = OwnerDrawFunc;

    // ��������
    ctrl_create.ctrl_id           = 0;
    ctrl_create.guid              = SPRD_GUI_OWNDRAW_ID;
    ctrl_create.init_data_ptr     = &init_data;
    ctrl_create.parent_win_handle = win_handle;
    //ctrl_create.parent_ctrl_handle = ctrl_handle;
    ctrl_create.state               = GUICTRL_STATE_DISABLE_ACTIVE|GUICTRL_STATE_TOPMOST|GUICTRL_STATE_INVISIBLE;

    //creat control
    gui_ctrl =  MMK_CreateControl(&ctrl_create);
    if (PNULL != gui_ctrl)
    {
        tp_handler.tp_down  = HandleTpDown;
        tp_handler.tp_short = HandleTpShort;
        tp_handler.tp_move = HandleTpMove;
        tp_handler.tp_long = HandleTpLong;
        tp_handler.tp_up = HandleTpUp;
        tp_handler.lose_focus = HandleLoseFocus;
        tp_handler.get_focus = HandleGetFocus;

        ctrl_id = GUICTRL_GetCtrlHandle(gui_ctrl);

        GUIOWNDRAW_SetCallback(ctrl_id, &tp_handler);
        GUIOWNDRAW_SetAddData(ctrl_id, add_data);
        GUIOWNDRAW_PermitBorder(ctrl_id, FALSE);

        // Ϊ�˲��ÿؼ�ˢ�£��������⴦���Ǹ�BUG
        bg.bg_type = GUI_BG_COLOR;
        bg.shape = GUI_SHAPE_MAX;       // BUGBUG

        GUIOWNDRAW_SetBg(ctrl_id, &bg);
    }

    return ctrl_id;
}

/*****************************************************************************/
//  Description : ���ƺ���
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void OwnerDrawFunc(
                         GUIOWNDRAW_INFO_T *owner_draw_ptr
                         )
{
}

/*****************************************************************************/
//  Description : ����TP DOWN
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpDown(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_SLIDER_DATA_T   *slider_data_ptr = PNULL;
    GUI_RECT_T          ctrl_rect = {0};

    if (GUIOWNDRAW_GetAddData(ctrl_handle, (ADD_DATA*)&slider_data_ptr))
    {
        GUIAPICTRL_GetRect(slider_data_ptr->dummy_ctrl_handle, &ctrl_rect);

        if (MMITHEME_SLIDER_TYPE_LEFT_SELECTOR == slider_data_ptr->slider_type)
        {
            slider_data_ptr->x_offset = tp_point.x - ctrl_rect.right + MMICLIPBRD_SLIDER_OFFSET;
        }
        else if (MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR == slider_data_ptr->slider_type)
        {
            slider_data_ptr->x_offset = tp_point.x - ctrl_rect.left - MMICLIPBRD_SLIDER_OFFSET;
        }
        else
        {
            slider_data_ptr->x_offset = tp_point.x - (ctrl_rect.left + ctrl_rect.right) / 2;
        }
        slider_data_ptr->y_offset = tp_point.y - ctrl_rect.top;

        // ����Ϣ��Ŀ�꣬˵��ʼ������
        MMK_SendMsg(slider_data_ptr->dst_ctrl_handle, MSG_NOTIFY_BEGIN_SLIDER, (DPARAM)slider_data_ptr->slider_type);

//        _MMICLIPBRD_StartMagnify(
//                tp_point.x,
//                ctrl_rect.top,
//                slider_data_ptr->dst_ctrl_handle);
    }

    return result;
}

/*****************************************************************************/
//  Description : ����TP SHORT
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpShort(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    return result;
}

/*****************************************************************************/
//  Description : ����TP MOVE
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpMove(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_SLIDER_DATA_T   *slider_data_ptr = PNULL;
    //GUI_RECT_T          ctrl_rect = {0};
    MMITHEME_SLIDER_MSG_PARAM_T slider_msg_param = {0};

    if (GUIOWNDRAW_GetAddData(ctrl_handle, (ADD_DATA)&slider_data_ptr))
    {
        slider_msg_param.slider_type = slider_data_ptr->slider_type;
        slider_msg_param.tp_point.x = tp_point.x - slider_data_ptr->x_offset;
        slider_msg_param.tp_point.y = tp_point.y - slider_data_ptr->y_offset - MMICLIPBRD_TP_OFFSET;

        MMK_SendMsg(slider_data_ptr->dst_ctrl_handle, MSG_NOTIFY_MOVE_SLIDER, &slider_msg_param);

        MMIAPICLIPBRD_ShowSlider((uint32)slider_data_ptr, 0, 0);

//        _MMICLIPBRD_MoveMagnify(
//                tp_point.x,
//                tp_point.y - slider_data_ptr->y_offset,
//                slider_data_ptr->dst_ctrl_handle);
    }

    return result;
}

/*****************************************************************************/
//  Description : ����TP LONG
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpLong(
                                MMI_HANDLE_T ctrl_handle,
                                GUI_POINT_T tp_point
                                )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    return result;
}

/*****************************************************************************/
//  Description : ����TP UP
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleTpUp(
                              MMI_HANDLE_T ctrl_handle,
                              GUI_POINT_T tp_point
                              )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_SLIDER_DATA_T   *slider_data_ptr = PNULL;

    if (GUIOWNDRAW_GetAddData(ctrl_handle, (ADD_DATA)&slider_data_ptr))
    {
//        _MMICLIPBRD_StopMagnify();

        MMK_SendMsg(slider_data_ptr->dst_ctrl_handle, MSG_NOTIFY_END_SLIDER, (DPARAM)slider_data_ptr->slider_type);
    }

    return result;
}

/*****************************************************************************/
//  Description : ����lose focus
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleLoseFocus(
                                   MMI_HANDLE_T ctrl_handle
                                   )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_SLIDER_DATA_T   *slider_data_ptr = PNULL;

    if (GUIOWNDRAW_GetAddData(ctrl_handle, (ADD_DATA)&slider_data_ptr))
    {
        if (0 != slider_data_ptr->lcd_dev_info.block_id
            && UILAYER_NULL_HANDLE != slider_data_ptr->lcd_dev_info.block_id)
        {
            UILAYER_RemoveBltLayer(&slider_data_ptr->lcd_dev_info);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : ����get foucs
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleGetFocus(
                                  MMI_HANDLE_T ctrl_handle
                                  )
{
    MMI_RESULT_E        result = MMI_RESULT_TRUE;
    GUI_SLIDER_DATA_T   *slider_data_ptr = PNULL;

    if (GUIOWNDRAW_GetAddData(ctrl_handle, (ADD_DATA)&slider_data_ptr))
    {
        if (slider_data_ptr->is_visible
            && 0 != slider_data_ptr->lcd_dev_info.block_id
            && UILAYER_NULL_HANDLE != slider_data_ptr->lcd_dev_info.block_id)
        {
            AppendSliderLayer(&slider_data_ptr->lcd_dev_info);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : APPEND��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateSliderLayer(
                                GUI_SLIDER_DATA_T   *slider_data_ptr
                                )
{
    BOOLEAN             result = FALSE;
    UILAYER_CREATE_T    create_info = {0};
    UILAYER_RESULT_E    layer_result = UILAYER_RESULT_SUCCESS;

    if (PNULL == slider_data_ptr)
    {
        return FALSE;
    }

    if (0 != slider_data_ptr->lcd_dev_info.block_id
        && UILAYER_NULL_HANDLE != slider_data_ptr->lcd_dev_info.block_id)
    {
        return TRUE;
    }

    // ������
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = 0;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = slider_data_ptr->size.w;;
    create_info.height = slider_data_ptr->size.h;
    create_info.is_bg_layer = FALSE;
    create_info.is_static_layer = TRUE;             // ������static������Ϊ���������崰�ں����˽��㣬��ᱻ�ͷ�
    create_info.format = UILAYER_MEM_FORMAT_DOUBLE;

    layer_result = UILAYER_CreateLayer(&create_info, &slider_data_ptr->lcd_dev_info);
    if (UILAYER_RESULT_SUCCESS == layer_result)
    {
        result = TRUE;
    }

    return result;
}

/*****************************************************************************/
//  Description : APPEND��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void AppendSliderLayer(
                             GUI_LCD_DEV_INFO    *lcd_dev_info_ptr
                             )
{
    UILAYER_APPEND_BLT_T append_layer = {0};

    if (PNULL != lcd_dev_info_ptr
        && 0 != lcd_dev_info_ptr->block_id
        && UILAYER_NULL_HANDLE != lcd_dev_info_ptr->block_id)
    {
        UILAYER_RemoveBltLayer(lcd_dev_info_ptr);

        append_layer.lcd_dev_info = *lcd_dev_info_ptr;
        append_layer.layer_level = UILAYER_LEVEL_NORMAL;

        UILAYER_AppendBltLayer(&append_layer);
    }
}

LOCAL BOOLEAN GetPosition(
                          GUI_SLIDER_DATA_T *slider_data_ptr,
                          GUI_RECT_T        *rect_ptr
                          )
{
    BOOLEAN     result = FALSE;
    uint16      pos = 0;
    IGUICTRL_T  *ctrl_ptr = PNULL;
    CAF_GUID_T  ctrl_guid = 0;

    ctrl_ptr = MMK_GetCtrlPtr(slider_data_ptr->dst_ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        ctrl_guid = IGUICTRL_GetCtrlGuid(ctrl_ptr);

        switch (ctrl_guid)
        {
        case SPRD_GUI_TEXTBOX_ID:
            // ��ȡλ��
            if (MMITHEME_SLIDER_TYPE_LEFT_SELECTOR == slider_data_ptr->slider_type)
            {
                GUITEXT_GetHighlightInfo(slider_data_ptr->dst_ctrl_handle, &pos, PNULL);
            }
            else if (MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR == slider_data_ptr->slider_type)
            {
                GUITEXT_GetHighlightInfo(slider_data_ptr->dst_ctrl_handle, PNULL, &pos);
            }

            // �ж��Ƿ���Ҫ��ʾ
            result = GUITEXT_GetPosInfo(slider_data_ptr->dst_ctrl_handle, pos, rect_ptr);
            break;

        case SPRD_GUI_RICHTEXT_ID:
            // ��ȡλ��
            if (MMITHEME_SLIDER_TYPE_LEFT_SELECTOR == slider_data_ptr->slider_type)
            {
                // �ж��Ƿ���Ҫ��ʾ
                result = GUIRICHTEXT_GetHighlightPos(slider_data_ptr->dst_ctrl_handle, rect_ptr, PNULL);
            }
            else if (MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR == slider_data_ptr->slider_type)
            {
                // �ж��Ƿ���Ҫ��ʾ
                result = GUIRICHTEXT_GetHighlightPos(slider_data_ptr->dst_ctrl_handle, PNULL, rect_ptr);
            }
            break;

        case SPRD_GUI_TEXTEDIT_ID:
        case SPRD_GUI_PHONENUMEDIT_ID:
        case SPRD_GUI_DIGITALEDIT_ID:
        case SPRD_GUI_PSWEDIT_ID:
        case SPRD_GUI_LISTEDIT_ID:
            // ��ȡλ��
            if (MMITHEME_SLIDER_TYPE_BLOCK == slider_data_ptr->slider_type)
            {
                pos = GUIEDIT_GetCursorCurPos(slider_data_ptr->dst_ctrl_handle);
            }
            else if (MMITHEME_SLIDER_TYPE_LEFT_SELECTOR == slider_data_ptr->slider_type)
            {
                GUIEDIT_GetHighlightInfo(slider_data_ptr->dst_ctrl_handle, &pos, PNULL);
            }
            else if (MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR == slider_data_ptr->slider_type)
            {
                GUIEDIT_GetHighlightInfo(slider_data_ptr->dst_ctrl_handle, PNULL, &pos);
            }

            // �ж��Ƿ���Ҫ��ʾ
            result = GUIEDIT_GetPosInfo(slider_data_ptr->dst_ctrl_handle, pos, rect_ptr);
            break;

        default:
            break;
        }
    }

    return result;
}
#else

/*****************************************************************************/
//  Description : ��������
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPICLIPBRD_CreateSlider(
                                      MMITHEME_SLIDER_TYPE_E   slider_type,
                                      MMI_WIN_ID_T             win_id,
                                      MMI_HANDLE_T             ctrl_handle
                                      )
{
    return 0;
}

/*****************************************************************************/
//  Description : ���ٻ���
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_DestroySlider(
                                     uint32    slider_handle
                                     )
{
}

/*****************************************************************************/
//  Description : ��ʾ����
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_ShowSlider(
                                  uint32   slider_handle,
                                  int16    x,
                                  int16    y
                                  )
{
}

/*****************************************************************************/
//  Description : ���ػ���
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICLIPBRD_HideSlider(
                                  uint32   slider_handle
                                  )
{
}

#endif

