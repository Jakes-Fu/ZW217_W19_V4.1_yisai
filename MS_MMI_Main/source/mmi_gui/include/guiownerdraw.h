/*! @file guiownerdraw.h
@brief ���ļ�������ownerdraw�ؼ��Ķ���ӿں���������
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_OwnerDraw OwnerDraw

(����)

*******************************************************************************/

/*! @addtogroup ownerdrawGrp OwnerDraw
@brief ownerdrawģ��
@details
    - �Ի�ؼ�,�ؼ�����ֻ��ʾ�����ͱ߿�\n
    - ����������ʾ����Ӧ��ͨ���ص�����ʵ��
@{
*******************************************************************************/

#ifndef _GUIOWNERDRAW_H_
#define _GUIOWNERDRAW_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"

#include "ctrlownerdraw_export.h"
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*--------------------------------------------------------------------------
                          FUNCTION DEFINITION
--------------------------------------------------------------------------*/
#define GUIOWNDRAW_SetCallback          CTRLOWNERDRAW_SetCallback   
#define GUIOWNDRAW_SetAddData           CTRLOWNERDRAW_SetAddData    
#define GUIOWNDRAW_GetAddData           CTRLOWNERDRAW_GetAddData    
#define GUIOWNDRAW_SetBg                CTRLOWNERDRAW_SetBg         
#define GUIOWNDRAW_SetHeight            CTRLOWNERDRAW_SetHeight     
#define GUIOWNDRAW_Update               CTRLOWNERDRAW_Update        
#define GUIOWNDRAW_GetRect              CTRLOWNERDRAW_GetRect       
#define GUIOWNDRAW_SetRect              CTRLOWNERDRAW_SetRect       
#define GUIOWNDRAW_GetDisplayRect       CTRLOWNERDRAW_GetDisplayRect
#define GUIOWNDRAW_PermitBorder         CTRLOWNERDRAW_PermitBorder  
#define GUIOWNDRAW_SetVisible           CTRLOWNERDRAW_SetVisible    

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

/*@}*/
