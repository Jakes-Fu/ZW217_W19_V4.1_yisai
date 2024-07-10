/*****************************************************************************
** File Name:      mmiset_multisim_internal.h                                           *
** Author:                                                                   *
** Date:           2006/09/18                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe setting definitions          *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/09/18    bruce.chi         Create
** 2012/02/12    xiyuan.ma         Edit
******************************************************************************/

#ifndef _MMISET_DUALSIM_INTERNAL_H_
#define _MMISET_DUALSIM_INTERNAL_H_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "mmiset.h"
#include "mmiset_internal.h"
#include "mmiset_multisim_export.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set sim name
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSimName(MN_DUAL_SYS_E dual_sys, MMISET_SIM_NAME_T * sim_name_ptr);

#ifdef MMI_MULTISIM_COLOR_SUPPORT

#define MAX_SIM_COLOR_NUM   8
#define SIM_COLOR_BTN_NUM_ONE_LINE   4


/*****************************************************************************/
// 	Description : set sim Color
//	Global resource dependence : 
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetSimColorByIndex(MN_DUAL_SYS_E dual_sys, uint16 index);

/*****************************************************************************/
// 	Description : get sim Color by index
//	Global resource dependence : 
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_GetSimColorByIndex(uint16 index);

/*****************************************************************************/
// 	Description : get index from sim Color 
//	Global resource dependence : 
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMISET_GetSimColorIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
// 	Description : set sim Color just for test
//	Global resource dependence : 
//  Author: sam.hua
//	Note:
/*****************************************************************************/
PUBLIC MMI_STRING_T MMISET_GetSimColorText(uint16   index);

#endif

/*****************************************************************************/
// 	Description : ���mmi ˫sim��ϵͳ�������
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
// PUBLIC void MMISET_SetDualSysSettging(MMISET_DUALSYS_TYPE_E	e_dual_sys);

/*****************************************************************************/
// 	Description :�����sim��nv����
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC MMISET_MAJOR_SIM_SEL_TYPE_E MMISET_GetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type);

/*****************************************************************************/
// 	Description :������sim��nv����
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetMajorSimSelType(MMISET_MAJOR_SIM_SEL_FUNC_TYPE_E e_func_type, MMISET_MAJOR_SIM_SEL_TYPE_E e_sim_sel_type);

/*****************************************************************************/
// 	Description :��ñ����ظ���nv����
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
//PUBLIC BOOLEAN MMISET_GetAnswerWithOriginalSimType(MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_TYPE_E e_func_type);

/*****************************************************************************/
// 	Description :���ñ����ظ���nv
//	Global resource dependence : 
//  Author: bruce.chi
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMISET_SetAnswerWithOriginalSimType(BOOLEAN is_answer_with_original_sim[MMISET_ANSWER_WITH_ORIGINAL_SIM_FUNC_MAX]);

/*****************************************************************************/
// 	Description : ���mmi �û����ö�sim��ϵͳ��������������ڿ����ж������
//	Global resource dependence : 
//  Author: bown.zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMISET_SetMultiSysSetting(MMISET_DUALSYS_TYPE_E	e_dual_sys);

#endif

