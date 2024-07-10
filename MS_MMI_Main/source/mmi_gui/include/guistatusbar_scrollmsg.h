/*! @file guistatusbar_data.h
@brief ���ļ�������statusbar_data�ؼ��Ķ���ӿں���������
@author bin.ji
@version 1.0
@date 8/5/2008
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 8/5/2008     bin.ji      Create
*******************************************************************************/

/*! @page page_StatusBar_data StatusBar_data

(����)

*******************************************************************************/

/*! @addtogroup statusbar_dataGrp StatusBar_data
@brief statusbar_dataģ��
@details ���ļ������⿪�ţ�����app��Ӧ�õ���guiwin.h�еķ�װ�ӿ�
@{
*******************************************************************************/

#ifndef _GUI_STATUSBAR_SCROLLMSG_H_
#define _GUI_STATUSBAR_SCROLLMSG_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "ctrlstatusbar_scrollmsg.h"
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
#ifdef GUIF_STATUSBAR_SCROLL
#define GUISTBSCROLLMSG_GetCurFrame CTRLSTBSCROLLMSG_GetCurFrame
#define GUISTBSCROLLMSG_GetCurScrollState CTRLSTBSCROLLMSG_GetCurScrollState
#define GUISTBSCROLLMSG_ConsumeOneLine CTRLSTBSCROLLMSG_ConsumeOneLine
#define GUISTBSCROLLMSG_ConsumeOneFrame CTRLSTBSCROLLMSG_ConsumeOneFrame
#define GUISTBSCROLLMSG_AppendScrollItem CTRLSTBSCROLLMSG_AppendScrollItem
#define GUISTBSCROLLMSG_GetCurLine CTRLSTBSCROLLMSG_GetCurLine
#define GUISTBSCROLLMSG_GetNextLine CTRLSTBSCROLLMSG_GetNextLine
#define GUISTBSCROLLMSG_SetIsValid CTRLSTBSCROLLMSG_SetIsValid
#endif

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _GUI_STATUSBAR_DATA_H_


/*@}*/
