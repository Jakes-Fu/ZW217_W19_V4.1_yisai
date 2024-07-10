/***************************************************************************
** File Name:      mmitheme_picker.h                                       *
** Author:         hua.fang                                                *
** Date:           08/17/2012                                              *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.      *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2012        hua.fang         Create                                  *
**																		   *
****************************************************************************/
#ifndef _MMITHEME_PICKER_H_
#define _MMITHEME_PICKER_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 
#include "guifont.h"
#include "mmi_font.h"

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

#define MMITHEME_BASEPICKER_TPSCROLL_TIME               100     // picker��TP�ƶ�ʱ�ػ���
#define MMITHEME_BASEPICKER_FLING_TIME                  10      // ����״̬ʱ�Ķ�ʱ�����
#define MMITHEME_BASEPICKER_RECOVER_TIME                100     // �ָ�ԭλʱ�Ķ�ʱ�����
#define MMITHEME_BASEPICKER_RECOVER_DISTANCE            5       // �ָ�ԭλ�Ĳ���

#define MMITHEME_BASEPICKER_MAX_FIELD_COUNT             MMITHEME_TIMEPICKER_FIELD_COUNT

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
// ������
//  �����ֵ����XXX_START��XXX_END֮��

// field for picker
typedef enum
{
    MMITHEME_PICKER_FIELD_START  = 0,                                           // picker��ʼֵ
    MMITHEME_PICKER_FIELD        = MMITHEME_PICKER_FIELD_START,
    MMITHEME_PICKER_FIELD_END,

    MMITHEME_PICKER_FIELD_COUNT  = MMITHEME_PICKER_FIELD_END,                   // picker�ܸ���
} MMITHEME_PICKER_FIELD_E;

// field for time picker
//  ��Ϊ��12Сʱ��,24Сʱ�ƺͶ�ʱ��������Ǳ�����һ�����ά���Լ���ֵ
typedef enum
{
    // 24hour
    MMITHEME_TIMEPICKER_FIELD_24H_START     = 0,                                //  24Сʱ�ƵĿ�ʼֵ
    MMITHEME_TIMEPICKER_FIELD_24H_HOUR      = MMITHEME_TIMEPICKER_FIELD_24H_START,
    MMITHEME_TIMEPICKER_FIELD_24H_MINUTE,
    MMITHEME_TIMEPICKER_FIELD_24H_END,

    // 12hour
    MMITHEME_TIMEPICKER_FIELD_12H_START     = MMITHEME_TIMEPICKER_FIELD_24H_END,        //  12Сʱ�ƵĿ�ʼֵ
    MMITHEME_TIMEPICKER_FIELD_12H_HOUR      = MMITHEME_TIMEPICKER_FIELD_12H_START,
    MMITHEME_TIMEPICKER_FIELD_12H_MINUTE,
    MMITHEME_TIMEPICKER_FIELD_12H_AMPM,
    MMITHEME_TIMEPICKER_FIELD_12H_END,

    // timer for HM
    MMITHEME_TIMEPICKER_FIELD_TIMER_START   = MMITHEME_TIMEPICKER_FIELD_12H_END,        // ��ʱ����ʼֵ
    MMITHEME_TIMEPICKER_FIELD_TIMER_HOUR    = MMITHEME_TIMEPICKER_FIELD_TIMER_START,
    MMITHEME_TIMEPICKER_FIELD_TIMER_MINUTE,
    MMITHEME_TIMEPICKER_FIELD_TIMER_END,

    // timer for HMS
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_START   = MMITHEME_TIMEPICKER_FIELD_TIMER_END,  // ���붨ʱ����ʼֵ
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_HOUR    = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_START,
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_MINUTE,
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_SECOND,
    MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_END,

    MMITHEME_TIMEPICKER_FIELD_COUNT             = MMITHEME_TIMEPICKER_FIELD_TIMER_HMS_END,  // ʱ������ܸ���
} MMITHEME_TIMEPICKER_FIELD_E;

// field for date picker
typedef enum
{
    MMITHEME_DATEPICKER_FIELD_START         = 0,                                //  ������Ŀ�ʼֵ
    MMITHEME_DATEPICKER_FIELD_YEAR          = MMITHEME_DATEPICKER_FIELD_START,
    MMITHEME_DATEPICKER_FIELD_MONTH,
    MMITHEME_DATEPICKER_FIELD_DAY,
    MMITHEME_DATEPICKER_FIELD_END,

    MMITHEME_DATEPICKER_FIELD_COUNT         = MMITHEME_DATEPICKER_FIELD_END,    // ��������ܸ���
} MMITHEME_DATEPICKER_FIELD_E;

// base picker theme
typedef struct
{
    uint16              char_space;             //space between chars,must >=1,include cursor
    uint16              line_height;            // �и�
    uint16              border_width;           // ���߿��
    uint16              separator_width;        // �ָ��߿��
    uint16              client_width;           // �ͻ������
    uint16              client_height;          // �ͻ����߶�
    int16               top_margin;             // ���ݵĶ�λ��
    int16               bottom_margin;          // ���ݵĵ�λ��
    GUI_ALIGN_E         align;                  // ���뷽ʽ

    GUI_BG_T            bg;                     //background

    BOOLEAN             is_mask_on_top;         // �ɲ��Ƿ�������֮��
    GUI_BG_T            mask_bg;                //tips background

    GUI_FONT_ALL_T      tips_font;              // tips font
    GUI_FONT_ALL_T      tips_font_middle;       // tips font
    GUI_FONT_ALL_T      content_font;           // content font
    GUI_FONT_ALL_T      cur_content_font;       // content font

    MMI_IMAGE_ID_T      separator_image_id;     // �ָ���ͼID
    MMI_IMAGE_ID_T      border_image_id;        // ����ͼID
    MMI_IMAGE_ID_T      tips_image_id;          // �ָ���ͼID
} MMITHEME_BASEPICKER_T;

// picker theme
typedef struct
{
    uint16              field_width[MMITHEME_PICKER_FIELD_COUNT];      // ������Ŀ��
    MMI_TEXT_ID_T       field_tips[MMITHEME_PICKER_FIELD_COUNT];       // ��ʾ�ı�
} MMITHEME_PICKER_T;

// date picker theme
typedef struct
{
    uint32              date_style;                                             // GUIEDIT_DATE_STYLE_E
    uint32              year_range;                                             // ��ķ�Χ

    uint16              field_width[MMITHEME_DATEPICKER_FIELD_COUNT];           // ������Ŀ��
    MMI_TEXT_ID_T       field_tips[MMITHEME_DATEPICKER_FIELD_COUNT];            // ��ʾ�ı�
} MMITHEME_DATEPICKER_T;

// time picker theme
typedef struct
{
    uint32              time_style;                                             //GUIEDIT_TIME_STYLE_E
    uint32              display_style;                                          //GUIEDIT_TIME_DISPLAY_STYLE_E

    uint16              field_width[MMITHEME_TIMEPICKER_FIELD_COUNT];      // ������Ŀ��
    MMI_TEXT_ID_T       field_tips[MMITHEME_TIMEPICKER_FIELD_COUNT];       // ��ʾ�ı�
} MMITHEME_TIMEPICKER_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : get theme for base picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetBasePickerTheme(
    MMITHEME_BASEPICKER_T    *theme_ptr
    );

/*****************************************************************************/
//  Description : get theme for picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetPickerTheme(
    MMITHEME_PICKER_T    *theme_ptr
    );

/*****************************************************************************/
//  Description : get theme for time picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetTimePickerTheme(
    MMITHEME_TIMEPICKER_T    *theme_ptr
    );

/*****************************************************************************/
//  Description : get theme for date picker
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void MMITHEME_GetDatePickerTheme(
    MMITHEME_DATEPICKER_T    *theme_ptr
    );

/*****************************************************************************/
//  Description : get date min and max year
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
void MMITHEME_GetDatePickerYearRange(
    uint32    year_range_type,//in:CTRLDATEPICKER_YEAR_RANGE_E
    uint16    *min_year_ptr,  //in/out:
    uint16    *max_year_ptr   //in/out:
    );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _MMITHEME_PICKER_H_
