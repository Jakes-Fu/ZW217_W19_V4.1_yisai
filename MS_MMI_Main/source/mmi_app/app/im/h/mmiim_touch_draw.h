/*==============================================================================
File Name: mmiim_touch_draw.h
Author: Lianxiang.Zhou
Date: 2011/12/13
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/13   Lianxiang.Zhou  Create.
==============================================================================*/


#ifndef _MMIIM_TOUCH_DRAW_H_
#define _MMIIM_TOUCH_DRAW_H_





/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "guiim_base.h"

#if defined (MMI_IM_PDA_SUPPORT)

#include "mmitheme_im.h"
#include "mmiim_internal.h"

//ʹ��565�Ĳ�������뷨�������
//���ƺ�����DrawPoint�����������Ϊ565��ɫ�⣬��������������Ϊ888�������룬�ں����ڲ�ת��Ϊ565
//�����Ͳ���Ҫ����ͬ���޸�������ɫ������������
//#define MMIIM_LAYER_565     

//565͸��ɫֱ��ʹ�ã���Ҫ�ڳ������ٴ�ת��Ϊ565��ɫ
//��ǰ��������еĻ���ɫ����888ת��Ϊ565�����¶�
//ת����һ�Σ����������һ������Ĺ�������ԭ��  
//����������colorkey��ʱ�򣬲�Ҫʹ�������ɫ    yaoguang.chen
#ifdef MMIIM_LAYER_565
#define MMIIM_UILAYER_TRANSPARENT_COLOR (0xff801000)       //UILAYER_TRANSPARENT_COLOR 565 TO 888
#else
#define MMIIM_UILAYER_TRANSPARENT_COLOR UILAYER_TRANSPARENT_COLOR
#endif

#define DC_CHECK(dc_data_ptr, rect_ptr) \
    do {\
        SCI_ASSERT(PNULL != dc_data_ptr); /*assert verified*/ \
        SCI_ASSERT(PNULL != rect_ptr); /*assert verified*/ \
        SCI_ASSERT(rect_ptr->left >= 0); /*assert verified*/ \
        SCI_ASSERT(rect_ptr->top >= 0); /*assert verified*/ \
        SCI_ASSERT(rect_ptr->right < (int32)dc_data_ptr->width); /*assert verified*/ \
        SCI_ASSERT(rect_ptr->bottom < (int32)dc_data_ptr->height); /*assert verified*/ \
    } while (0)


/*
���ǵİ�ť��ö�ٵģ���Ϊ�ַ���ť�Ϳ��ư�ť
ÿ����ť��״̬��: ���������¡�ѡ����δ������ǻ��ÿ��״̬��������
��ť�ϵ��ַ�����֧�����С�
����Ĭ�ϣ�һ����ť�������3��Ԫ�أ�һ��ͼƬ���������֣����������������ŵ�
ͼƬ��ϯ�������һ�У���������
�������������������Ҫ������ɫ����С����״����Ϣ
1 ��ɫ
    1 ������ɫ
    2 �������͵İ�ť����ɫ��������ť���ַ�����ɫ
    3
*/
/*! @enum SHARP_TAG
@brief ��״����
*/
enum SHARP_TAG
{
    SHARP_RECTANGLE,        /*!< ֱ�Ǿ��� */
    SHARP_ROUND_RECTANGLE,  /*!< Բ�Ǿ��� */
};
typedef uint8 SHARP_T;

/*! @enum BORDER_RELIEF_TAG
@brief �߿��Ч������
*/
enum BORDER_RELIEF_TAG
{
    RELIEF_FLAT,            /*!< �ޱ߿� */
    RELIEF_RAISED,          /*!< 3Dͻ�� */
    RELIEF_SUNKEN,          /*!< 3D�³� */
    RELIEF_SOLID,           /*!< ʵ�߿� */
    RELIEF_RIDGE,           /*!< ɽ�� */
    RELIEF_GROOVE,          /*!< ���� */
    RELIEF_INSET_BRIGHT,    /*!< �ڱ߸��� */
    RELIEF_INSET_DARK,      /*!< �ڱ����� */
};
typedef uint8 BORDER_RELIEF_T;

/*! @enum KEY_STATE_TAG
@brief ���ⰴ����״̬
*/
enum KEY_STATE_TAG
{
    KEY_STATE_ACTIVE,       /*!< ����ģ�Ĭ�� */
    KEY_STATE_PRESS,         /*!< ������ */
    KEY_STATE_INACTIVE,     /*!< �Ǽ��� */

    KEY_STATE_COUNT,        /*!< ����״̬�� - ֮��Ĳ�Ҫ��ʾ*/
};
typedef uint8 KEY_STATE_T;

/*! @struct BORDER_TAG
@brief �߿�Ķ���
*/
typedef struct BORDER_TAG
{
    BORDER_RELIEF_T relief; /*!< �߿����� */
    uint8 width;            /*!< ��� */
    uint32 color;           /*!< ��ɫ */
    uint32 color_bright;    /*!< Ч��������ɫ */
    uint32 color_dark;      /*!< Ч��������ɫ */
} BORDER_T;

/*! @enum FILL_TAG
@brief �������Ͷ���
*/
enum FILL_TAG
{
    FILL_NONE,              /*!< ����� */
    FILL_PURE,              /*!< ���ɴ�ɫ */
    FILL_GRADIENT,          /*!< ���ɽ���ɫ */
    FILL_GRADIENT_BLOCK,    /*!< ���ɷֿ�Ľ���ɫ */
    FILL_GRADIENT_PURE_BLOCK,    /*!< ����һ���ֽ���ɫ��һ���ִ�ɫ */
};
typedef uint8 FILL_T;

/*! @struct GRADIENT_DATA_TAG
@brief ����ɫ��������
*/
typedef struct GRADIENT_DATA_TAG
{
    uint32 c0;              /*!< ���ɫ */
    uint32 c1;              /*!< �յ�ɫ */
    int16 a;                /*!< �仯���ٶ� */
} GRADIENT_DATA_T;

/*! @struct GRADIENT_BLOCK_DATA_TAG
@brief �ֿ���佥��ɫ������
*/
typedef struct GRADIENT_BLOCK_DATA_TAG
{
    GRADIENT_DATA_T d0;     /*!< ��һ������ */
    GRADIENT_DATA_T d1;     /*!< �ڶ������� */
    GRADIENT_DATA_T d2;     /*!< ���������� */
} GRADIENT_BLOCK_DATA_T;

/*! @union FILL_DATA_TAG
@brief ��������
*/
typedef union FILL_DATA_TAG
{
    GRADIENT_BLOCK_DATA_T gradient_block;   /*!< �ֿ齥��ɫ���� */
    GRADIENT_DATA_T gradient;               /*!< ����ɫ���� */
    uint32 pure;                            /*!< ��ɫ���� */
} FILL_DATA_T;



#ifdef __cplusplus
extern "C"
{
#endif

    /* ��ͼ */
    PUBLIC BOOLEAN _DrawLine(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        int16 x0, int16 y0, int16 x1, int16 y1,
        int16 width,
        uint32 color
    );
    PUBLIC BOOLEAN _DrawRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color
    );
    PUBLIC BOOLEAN _FillRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color
    );
    PUBLIC BOOLEAN _FillGradientRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color_top,
        uint32 color_bottom,
        int16 acceleration
    );
    PUBLIC BOOLEAN _DrawBorderUneven(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color,
        uint32 color_bright,
        uint32 color_dark
    );
    PUBLIC BOOLEAN _DrawBorderSharp(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color,
        uint32 color_bright,
        uint32 color_dark
    );
    PUBLIC BOOLEAN _DrawRoundRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color
    );
    PUBLIC BOOLEAN _FillRoundRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color,
        BOOLEAN is_full
    );
    PUBLIC BOOLEAN _FillGradientRoundRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color_top,
        uint32 color_bottom,
        int16 acceleration,
        BOOLEAN is_full
    );
    PUBLIC BOOLEAN _FillGradientBlockRoundRect(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color_0_top,
        uint32 color_0_bottom,
        int16 acceleration_0,
        uint32 color_1_top,
        uint32 color_1_bottom,
        int16 acceleration_1,
        uint32 color_2_top,
        uint32 color_2_bottom,
        int16 acceleration_2,
        BOOLEAN is_full
    );
    PUBLIC BOOLEAN _FillGradientPureBlockRoundRect (
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint32 color_0_top,
        uint32 color_0_bottom,
        int16 acceleration_0,
        BOOLEAN is_full
    );
    PUBLIC BOOLEAN _DrawRoundBorderUneven(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color,
        uint32 color_bright,
        uint32 color_dark
    );
    PUBLIC BOOLEAN _DrawRoundBorderSharp(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color,
        uint32 color_bright,
        uint32 color_dark
    );
    PUBLIC BOOLEAN _DrawRoundBorderInset(
        GUILCD_DRAW_DC_T const *dc_data_ptr,
        GUI_RECT_T const *rect_ptr,
        uint8 width,
        BORDER_RELIEF_T relief,
        uint32 color_bright,
        uint32 color_dark
    );


#ifdef __cplusplus
}
#endif


#endif //defined (MMI_IM_PDA_SUPPORT) 

#endif /*_MMIIM_TOUCH_DRAW_H_*/

