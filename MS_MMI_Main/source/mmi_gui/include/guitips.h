/*! @file guitips.h
@brief ���ļ�������tips�ؼ��Ķ���ӿں���������
@author Xiaoqing.Lu
@version 1.0
@date 02/20/2009
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 2/2009         Xiaoqing.Lu      Create
*******************************************************************************/

/*! @page page_Tips Tips(Tips)

-# @ref sec_tips_function
-# @ref sec_tips_create
-# @ref sec_tips_app

@section sec_tips_function �ؼ�����

Tips�Ǵ���������Ϣ����ʾ�ؼ������ܹ�����������ʾһЩ������Ϣ��ʹ�ô��ڵ�ĳЩ���Ը�Ϊ��������ʾ���û�����һ�������У�ֻ����һ��Ψһ��tips�ؼ���Tips�ؼ�����Ӧ�κ���Ϣ��

@section sec_tips_create Tips�ؼ�����

��״̬�����Ƶģ�Tips�ؼ�Ҳ����Ϊ���ڵ�һ�����ԣ����Ĵ����ǵ���WIN_TIPS�꣬��״̬��һ���������ڴ��ڵ�Table�С���Tips��Ҫ��ʾ���ݵ�ʱ�����ᷢ��MSG_CTL_TIPS_NEED_DATA��Ϣ���û�����Я��һ���Ĳ��������統ǰlistbox���������ŵȣ��û����յ������Ϣ֮������������ݡ���Tips�ؼ���ʾ��

Tips�ؼ��Ƚϼ򵥣����������κ���Ϣ��Ҳ����Ӧ�û������룬�������þ�����ʾ��ʾ��Ϣ������û�ʹ������Ҳ�Ƚϼ򵥡�

��ȻTips�Ǵ��ڵ�һ�����ԣ���Ŀǰ���������б�˵����б�ؼ������Ž��ܵĹ�ϵ�����Ҳֻ�����б�˵����б�ؼ��Ĵ�����ʹ��Tips��

���û����յ�Tips��Ϣʱ������յ�һ����Ϣ���������������ʵ���������£�
- @ref GUITIPS_DATA_T

��Ϣ��ʱ���������Я������Ϣ�пؼ���ID����ǰitem�������ţ��û����Ծ����Ƿ���ʾTips���Լ���Tips����ʾ��icon id���ַ������û�ֻ��Ҫ�޸���������Ϳ����ˣ�������Ҫ�Լ����������ڴ档

@section sec_tips_app �ؼ�Ӧ��

���磬�ڵ绰���У�ʹ��tips�ؼ���ʾ����Ϊ���߼��𡱵ĵ绰����Ϊ��123456789����
@image html tips_display.jpg
ͼ 2 38 �ؼ���ʾ��ͼ
*******************************************************************************/

/*! @addtogroup tipsGrp Tips
@brief tips�ؼ�
@details tips�ؼ�
@{
*******************************************************************************/
#ifndef _GUITIPS_H_
#define _GUITIPS_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "sci_types.h"
#include "mmk_type.h"
#include "mmitheme_tips.h"
#include "guictrl.h"

#include "ctrltips_export.h"
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#ifdef __cplusplus
extern   "C"
{
#endif
/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define	GUITIPS_Update              CTRLTIPS_Update      
#define GUITIPS_SetEdgeRect         CTRLTIPS_SetEdgeRect 
#define	GUITIPS_SetTipsText         CTRLTIPS_SetTipsText 
#define	GUITIPS_SetTipsValid        CTRLTIPS_SetTipsValid
#define	GUITIPS_GetTipsValid        CTRLTIPS_GetTipsValid
#define	GUITIPS_SetTextColor        CTRLTIPS_SetTextColor
#define	GUITIPS_SetLcdInfo          CTRLTIPS_SetLcdInfo  

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif//_GUITIPS_H_
/*@}*/
