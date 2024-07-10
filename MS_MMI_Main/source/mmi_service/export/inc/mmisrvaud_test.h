 /****************************************************************************
** File Name:      mmisrvaud_test.h                                                              *
** Author:          Yintang.ren                                                               *
** Date:             25/05/2011                                                              *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to declare the API of mmisrvaud test module.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE                NAME                 DESCRIPTION                                 *
** 25/05/2011       Yintang.ren         Create
** 
****************************************************************************/
#ifndef _MMISRVAUD_TEST_H_
#define _MMISRVAUD_TEST_H_


/*****************************************************************************/
//  Description:  Set test if is enable.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_TestSetEnable(BOOLEAN is_enable);

/*****************************************************************************/
//  Description:  Draw play test back ground.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC void MMISRVAUD_TestDrawBg(void);

/*****************************************************************************/
//  Description:  Handle key msg.
//  Global resource dependence: 
//  Author: Yintang.Ren
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMISRVAUD_TestHandleKey(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);


#endif


