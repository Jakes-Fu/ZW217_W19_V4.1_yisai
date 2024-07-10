/*****************************************************************************
** File Name:      mmiidle_screensave_special.h                                      *
** Author:                                                                   *
** Date:           05/15/2008                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 05/15/2008     bin.ji              Create
******************************************************************************/

#ifndef _MMIIDLE_SCREENSAVER_SPECIAL_H_
#define _MMIIDLE_SCREENSAVER_SPECIAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif
#ifdef SCREENSAVER_SUPPORT

#include "mmiset_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
	SPESCR_STAR,		//���ǡ�����
	SPESCR_SNAKE,		// ̰��������
	SPESCR_SEED,		//������Ϸ����
	
	SPESCR_MAX	
}MMIIDLE_SPESCR_TYPE_E;

typedef struct 
{
    void (*init_proc)(void);        //��ʼ������
    void (*redraw_proc)(void);      //�ػ���Ļ
    void (*trigger_proc)(void);     //timer����
}MMIIDLE_SPESCR_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : ������Ԥ������
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIIDLE_OpenSpeScrSaverPreviewWin(MMIIDLE_SPESCR_TYPE_E   scr_type);

/*****************************************************************************/
// 	Description : ����������
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
void MMIAPIIDLESS_OpenSpecialScreenSaver(MMIIDLE_SPESCR_TYPE_E   scr_type);

/*****************************************************************************/
// 	Description : �˵�typeת��screentype
//	Global resource dependence : 
//  Author:bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC MMIIDLE_SPESCR_TYPE_E MMIAPIIDLESS_ConvertSCRSaverType(
                                                 MMISET_SCREENSAVER_TYPE_E menu_type
                                                 );

#endif
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
