/***************************************************************************
** File Name:      ctrlbasepicker_export.h                                 *
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
#ifndef _CTRLBASEPICKER_EXPORT_H_
#define _CTRLBASEPICKER_EXPORT_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/ 

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

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*! @enum CTRLBASEPICKER_TIPS_POS_E
@brief ������ʾ��λ��
*/
typedef enum
{
    CTRLBASEPICKER_TIPS_POS_TOP,           /*!<����λ��*/
    CTRLBASEPICKER_TIPS_POS_MIDDLE,        /*!<�м�λ��*/
    CTRLBASEPICKER_TIPS_POS_NONE,          /*!<û����ʾ*/
    CTRLBASEPICKER_TIPS_POS_MAX            /*!<����λ*/
} CTRLBASEPICKER_TIPS_POS_E;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ������ʾ���ֵ�λ��
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetTipsPos (
    MMI_CTRL_ID_T                 ctrl_id,
    CTRLBASEPICKER_TIPS_POS_E     tips_pos
);

/*****************************************************************************/
//  Description : �Ƿ�������ֵ��ʱ��������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_EnableAnim (
    MMI_CTRL_ID_T         ctrl_id,
    BOOLEAN               enable_anim
);

/*****************************************************************************/
//  Description : ����picker���ĸ߶�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetHeight (
    MMI_CTRL_ID_T        ctrl_id,
    uint16               height
);

/*****************************************************************************/
//  Description : ����picker�����и�
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetLineHeight (
    MMI_CTRL_ID_T    ctrl_id,
    uint16           line_height
);

/*****************************************************************************/
//  Description : ����picker������������
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLBASEPICKER_SetFont (
    MMI_CTRL_ID_T      ctrl_id,
    GUI_FONT_ALL_T     *normal_font_ptr,
    GUI_FONT_ALL_T     *active_font_ptr
);

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef _cplusplus
	}
#endif

#endif  // _CTRLBASEPICKER_EXPORT_H_
