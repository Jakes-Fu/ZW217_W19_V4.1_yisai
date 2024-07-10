/******************************************************************************
�ļ�����:mmipim_wintab.h
�ļ�����:UI���ڹ���
�ļ�˵��:
������Ա:������
�״ο�������:2007-9.26
******************************************************************************/


/******************************************************************************
                                                            �޸ļ�¼
  ����                   ����                �޸�ԭ��
  -----------------------------------------------------------------------------
  2007-09-26         ������                ��һ�ο������롣
  -----------------------------------------------------------------------------
  *****************************************************************************/

#ifndef _MMIPIM_WINTAB_H_
#define _MMIPIM_WINTAB_H_ 
#include "mn_type.h"
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

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
 /*#ifdef MAINLCD_SIZE_176X220 
 #define PIM_NOTE_MSG_HEIGHT 21 // chenxiang 20080806
 #else
#define PIM_NOTE_MSG_HEIGHT 25 // chenxiang 20080806
#endif

#define PIM_NOTE_MSG_TOP MMI_TITLE_HEIGHT    
#define PIM_NOTE_MSG_BOTTOM (MMI_TITLE_HEIGHT +PIM_NOTE_MSG_HEIGHT)
#define PIM_NOTE_MSG_RIGHT (MMI_MAINSCREEN_WIDTH - 1)

#ifdef MAINLCD_SIZE_176X220   //add by wangxiaolin 2010.07.02
#define PIM_PRGBOX_LEFT 15
#define PIM_PRGBOX_TOP 175
#define PIM_PRGBOX_RIGHT MMI_MAINSCREEN_WIDTH-PIM_PRGBOX_LEFT
#define PIM_PRGBOX_BOTTOM (PIM_PRGBOX_TOP + 15)
#else
#define PIM_PRGBOX_LEFT 30
#define PIM_PRGBOX_TOP 240
#define PIM_PRGBOX_RIGHT MMI_MAINSCREEN_WIDTH-PIM_PRGBOX_LEFT
#define PIM_PRGBOX_BOTTOM (PIM_PRGBOX_TOP + 15)
#endif*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 
/*****************************************************************************/
//  Description :�򿪺Ų��ܼ����˵�����
//  Global resource dependence : none
//  Author: wangxiaolin
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIM_OpenMainMenuWin(PIM_ENTRY_TYPE entry_type);

/*****************************************************************************/
//  Description :�򿪺Ų��ܼҵ��Բ������ô���
//  Global resource dependence : none
//  Author: chenxiang
//  Note: 20080506
/*****************************************************************************/
PUBLIC void MMIPIM_OpenDebugMenuWin(void);
/*****************************************************************************/
//  Description :
//  Global resource dependence : none
//  Author: chenxiang
//  Note: 20080506
/*****************************************************************************/
PUBLIC void MMIPIM_OpenNetQueryWinFromPB(void);

/*****************************************************************************/
//  Description :���õ�ǰʹ�õ�sim ��
//  Global resource dependence : none
//  Author: renyi.hu
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIPIM_GetSim(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

