/*****************************************************************************
** File Name:      mmiptscr_export.h                                             *
** Author:                                                                   *
** Date:           04/14/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mmi file manage             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 04/14/2011     bin.ji      Create                                    *
******************************************************************************/


#ifndef _MMI_PTSCR_EXPORT_H_
#define _MMI_PTSCR_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
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
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/

//ץ��ģʽ
typedef enum
{
    MMIPTSCR_RUNMODE_OFF,                   //�ر�
    MMIPTSCR_RUNMODE_PRESS_ONLYLCD,         //����������ֻץlcdͼ������
    MMIPTSCR_RUNMODE_PRESS_LCDANDLAYER,     //����������ͬʱץlcdͼ�����ݺͶ�ͼ������
    MMIPTSCR_RUNMODE_AUTO,            //�Զ���ʱץ����lcdͼ�����ݣ�

    MMIPTSCR_RUNMODE_MAX
}MMIPTSCR_RUNMODE_E;

/*,@CRMS00239652 renwei modify begin*/
typedef enum
{
    MMIPTSCR_ERROR_NONE,
    MMIPTSCR_SDCARD_INVALID,
    MMIPTSCR_SDCARD_NO_EXIST,
    MMIPTSCR_SDCARD_NO_SPACE,
    MMIPTSCR_ERROR_MAX
}MMIPTSCR_ERROR_STATUS_E;
/*,@CRMS00239652 renwei modify end*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : ���lcd_buffer�Լ�ͼ������
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPTSCR_Print(BOOLEAN is_includelayer);	

/*,@CRMS00239652 renwei modify begin*/					   
/*****************************************************************************/
//  Description : check sd card memory status
//  Global resource dependence : 
//  Author:   renwei
//  Note:
/*****************************************************************************/
PUBLIC MMIPTSCR_ERROR_STATUS_E MMIAPIPTSCR_PreCheck(void);

/*,@CRMS00239652 renwei modify end*/
/*****************************************************************************/
//     Description : �õ�ץ��������ģʽ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC MMIPTSCR_RUNMODE_E MMIAPIPTSCR_GetRunMode(void);


/*****************************************************************************/
//     Description : �ı�ץ��������ģʽ
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPIPTSCR_ChangeRunMode(
                                MMIPTSCR_RUNMODE_E runmode
                                );

/*****************************************************************************/
//     Description : extract lcd buffer
//    Global resource dependence : 
//  Author:   sheng.zhu
//    Note:
/****************************************************************************/
PUBLIC BOOLEAN MMIAPIPTSCR_GetLCDBuffer(unsigned char *lcd_buffer_ptr, \
													uint32 lcd_buffer_size_max, \
													uint32 *lcd_buffer_size_ptr, \
													uint32 *lcd_width_ptr, \
													uint32 *lcd_height_ptr);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif //_MMI_PTSCR_EXPORT_H_


