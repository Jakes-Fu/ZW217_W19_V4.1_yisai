/*****************************************************************************
** File Name:      ctrlbasepicker.h                                               *
** Author:                                                                   *
** Date:           07/18/2012                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2009      hua.fang              Create
******************************************************************************/

#ifndef _CTRLBASEPICKER_H_
#define _CTRLBASEPICKER_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrlbase.h"
#include "mmitheme_picker.h"
#include "ctrlbasepicker_export.h"
#include "mmi_string.h"

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
#define CTRLBASEPICKER_MAX_STR_LEN          10              // �ı������ʾ����

// basepicker type info
#define CTRL_BASEPICKER_TYPE     (BASEPICKER_GetType())

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// field
typedef int32 CTRLBASEPICKER_FIELD_T;

// basepicker init param
typedef struct
{
    CTRLBASE_INIT_PARAM_T       base_ctrl_param;

    // self inif param ...
} CTRLBASEPICKER_INIT_PARAM_T;

// �������
typedef struct
{
    BOOLEAN                     is_loop;                // ��ֵ�Ƿ�Ҫѭ��
    MMK_TP_SLIDE_E              slide_state;            // ��ǰ��״̬
    int32                       cur_offset;             // ��ǰ��ƫ��ֵ��ͬʱ���ڼ��㵱ǰ��ʱ������ֵ
    void                        *tracker_ptr;           // ������
    float                       fling_velocity;         // fling״̬ʱ�ļ��ٶ�ֵ
    int32                       recover_move_step;      // recover״̬ʱ��ÿ��������, >0��ʾ�����ƶ���<0��ʾ�����ƶ���=0��ʾλ������
    int32                       recover_dst_offset;     // recover״̬ʱ������Ŀ��ƫ��ֵ.(��ֵ�п��ܻᳬ��offset����ֵ������SetOffset����������)
    int32                       recover_cur_offset;     // recover״̬ʱ�ĵ�ǰƫ��ֵ
} CTRLBASEPICKER_ITEM_T;

//basepicker control
typedef struct _basepicker_obj_tag
{
    CTRLBASE_OBJ_T              base_ctrl;

    CTRLBASEPICKER_FIELD_T      tp_down_field;          // ���ʰ��µ���
    GUI_POINT_T                 tp_start_point;         // TP DOWNʱ�ĵ�
    GUI_POINT_T                 tp_pre_point;           // ��һ��TP��
    uint8                       tpscroll_timer_id;      // �϶�ʱ�Ķ�ʱ��ID
    uint8                       fling_timer_id;         // ����ʱ�Ķ�ʱ��ID
    uint8                       recover_timer_id;       // �ָ�ԭλʱ�Ķ�ʱ��ID
    CTRLBASEPICKER_TIPS_POS_E   tips_pos;               // ��ʾ����λ�ã�Ĭ���ڶ���
    BOOLEAN                     enable_anim;            // ���ýӿ�����ֵʱ���Ƿ���Ҫ����

    CTRLBASEPICKER_ITEM_T       state[MMITHEME_BASEPICKER_MAX_FIELD_COUNT]; // �������״̬

    MMITHEME_BASEPICKER_T       theme;

    BOOLEAN                     is_tp_moved;
    // self member ...
} CTRLBASEPICKER_OBJ_T;

// basepicker class for public or protected method
typedef struct _basepicker_vtbl_tag
{
    CTRLBASE_VTBL_T            base_ctrl_vtbl;

    /**************************************************************************/
    //  Description : ��ȡ��ʼ���������(��������ֵ)
    /**************************************************************************/
    void (*GetFieldRange)(
                       CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr,   // [in]
                       int32                *start_field_ptr,       // [out]
                       int32                *end_field_ptr          // [out]
                       );

    /**************************************************************************/
    //  Description : ��ȡָ�������ֵ
    /**************************************************************************/
    void (*GetValueRange)(
                       CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
                       CTRLBASEPICKER_FIELD_T      field,               // [in]
                       uint16                      *min_value_ptr,      // [out]
                       uint16                      *max_value_ptr       // [out]
                       );

    /*****************************************************************************/
    //  Description : ��ȡָ����������ʾλ��
    /*****************************************************************************/
    GUI_RECT_T (*GetFieldRect)(
                            CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
                            int32                   field                   // [in]
                            );

    /**************************************************************************/
    //  Description : ��ȡ���е�����
    /**************************************************************************/
    void (*GetFieldText)(
                      CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,   // [in]
                      CTRLBASEPICKER_FIELD_T        field,                  // [in]
                      uint16                        cur_index,              // [in]
                      MMI_STRING_T                  *dis_string_ptr         // [out]
                      );

    /*****************************************************************************/
    //  Description : ��ȡ��ʾ�ı�
    /*****************************************************************************/
    MMI_TEXT_ID_T (*GetFieldTips)(
                               CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr,   // [in]
                               int32                field                   // [in]
                               );

    /*****************************************************************************/
    //  Description : ���ֵ�Ƿ�Ϸ���������Ϸ���������Ϊ�Ϸ�ֵ
    /*****************************************************************************/
    void (*CheckValue)(
                     CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
                     BOOLEAN                is_need_anim            // [in] �Ƿ���Ҫ���ƶ���
                     );

} CTRLBASEPICKER_VTBL_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get basepicker type
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T BASEPICKER_GetType(void);

/*****************************************************************************/
//  Description : init
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Init (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr    // [in]
);

/*****************************************************************************/
//  Description : destroy
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Destroy (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr    // [in]
);

/*****************************************************************************/
//  Description : ���������ֵ�����������ֵ���Զ�����ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEPICKER_SetValue (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      value                   // [in]
);

/*****************************************************************************/
//  Description : ��ȡ�����ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC uint16 BASEPICKER_GetValue (
    CTRLBASEPICKER_OBJ_T         *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T       field                   // [in]
);

/*****************************************************************************/
//  Description : ����Ŀ��ֵ��������recover״̬
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_SetRecoverValue (
    CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
    CTRLBASEPICKER_FIELD_T      field,                  // [in]
    uint16                      value,                  // [in],
    BOOLEAN                     is_up                   // [in] �����Ƿ�����
);

/*****************************************************************************/
//  Description : reset picker
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Reset (
    CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr
);

/*****************************************************************************/
//  Description : ����������ʾpicker����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void BASEPICKER_Display (
    CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr    // [in]
);

/*****************************************************************************/
//  Description : ������ʾ���ֵ�λ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN BASEPICKER_SetTipsPos (
    CTRLBASEPICKER_OBJ_T          *basepicker_ctrl_ptr,
    CTRLBASEPICKER_TIPS_POS_E     tips_pos
);

/*****************************************************************************/
//  Description : ��ȡ�����ı�������(�۳���ʾ����)
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC GUI_RECT_T BASEPICKER_GetTextRect (
    CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr
);

/*****************************************************************************/
//  Description : ��ȡ��ʼ���������(��������ֵ)
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEPICKER_GetFieldRange(
                       CTRLBASEPICKER_OBJ_T *basepicker_ctrl_ptr,   // [in]
                       int32                *start_field_ptr,       // [out]
                       int32                *end_field_ptr          // [out]
                       )
{
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetFieldRange)
    {
        basepicker_vtbl_ptr->GetFieldRange(basepicker_ctrl_ptr, start_field_ptr, end_field_ptr);
    }
}

/*****************************************************************************/
//  Description : ��ȡָ�������ֵ
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEPICKER_GetValueRange(
                       CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,// [in]
                       CTRLBASEPICKER_FIELD_T       field,               // [in]
                       uint16                      *min_value_ptr,      // [out]
                       uint16                      *max_value_ptr       // [out]
                       )
{
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetValueRange)
    {
        basepicker_vtbl_ptr->GetValueRange(basepicker_ctrl_ptr, field, min_value_ptr, max_value_ptr);
    }
}

/*****************************************************************************/
//  Description : ��ȡָ����������ʾλ��
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline GUI_RECT_T VTLBASEPICKER_GetFieldRect(
                            CTRLBASEPICKER_OBJ_T    *basepicker_ctrl_ptr,   // [in]
                            int32                   field                   // [in]
                            )
{
    GUI_RECT_T  rect = {0};
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetFieldRect)
    {
        rect = basepicker_vtbl_ptr->GetFieldRect(basepicker_ctrl_ptr, field);
    }

    return rect;
}

/*****************************************************************************/
//  Description : ��ȡ���е�����
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEPICKER_GetFieldText(
                        CTRLBASEPICKER_OBJ_T        *basepicker_ctrl_ptr,   // [in]
                        CTRLBASEPICKER_FIELD_T      field,                  // [in]
                        uint16                      cur_index,              // [in]
                        MMI_STRING_T                *dis_string_ptr         // [out]
                        )
{
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetFieldText)
    {
        basepicker_vtbl_ptr->GetFieldText(basepicker_ctrl_ptr, field, cur_index, dis_string_ptr);
    }
}

/*****************************************************************************/
//  Description : ��ȡ��ʾ�ı�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline MMI_TEXT_ID_T VTLBASEPICKER_GetFieldTips(
                                 CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
                                 int32                  field                   // [in]
                                 )
{
    MMI_TEXT_ID_T  text_id = {0};
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->GetFieldTips)
    {
        text_id = basepicker_vtbl_ptr->GetFieldTips(basepicker_ctrl_ptr, field);
    }

    return text_id;
}

/*****************************************************************************/
//  Description : ���ֵ�Ƿ�Ϸ���������Ϸ���������Ϊ�Ϸ�ֵ
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
static __inline void VTLBASEPICKER_CheckValue(
                 CTRLBASEPICKER_OBJ_T   *basepicker_ctrl_ptr,   // [in]
                 BOOLEAN                is_need_anim            // [in] �Ƿ���Ҫ���ƶ���
                 )
{
    CTRLBASEPICKER_VTBL_T  *basepicker_vtbl_ptr = (CTRLBASEPICKER_VTBL_T*)TYPEMNG_GetVtbl(CTRL_BASEPICKER_TYPE, basepicker_ctrl_ptr);

    if (PNULL != basepicker_vtbl_ptr->CheckValue)
    {
        basepicker_vtbl_ptr->CheckValue(basepicker_ctrl_ptr, is_need_anim);
    }
}

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif
