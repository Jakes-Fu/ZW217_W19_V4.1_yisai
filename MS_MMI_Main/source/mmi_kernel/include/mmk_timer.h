/******************************************************************************
** File Name:      mmk_timer.h                                                *
** Author:                                                                    *
** Date:           03/11/2003                                      	          *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe the data struct and          *
**                  function of timer                                         *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 11/2003       Louis.wei         Create		     		 				  *
******************************************************************************/

/*!
 *  \addtogroup mmk_timer_group_label
 *  @{
 */


/*! 
 *  \file 	mmk_timer.h
 *  \date	2006
 *  \author louis.wei
 *  \brief 	This file is used to describe mmk timer.
 */


#ifndef  _MMK_TIMER_H_    
#define _MMK_TIMER_H_   

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern "C"
    {
#endif

/*----------------------------------------------------------------------------*/ 
/*                         Include Files                                      */
/*----------------------------------------------------------------------------*/ 

#include "mmk_type.h"  
#include "mmi_signal.h"
#include "mmi_osbridge.h"
#include "mmk_app.h"

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

#define MMI_NONE_TIMER_ID           0x00
#define MMI_KBD_TIMER_ID_1          0x01  //按键长按的定时器1, 因为支持三个键同时按下, 所以有三个定时器
#define MMI_KBD_TIMER_ID_2          0x02  //按键长按的定时器2
#define MMI_KBD_TIMER_ID_3          0x03  //按键长按的定时器3

#define MMI_LCD_LIGHT_TIMER_ID      0x04  //lcd背光定时器
#define T9_EXPL_TIMER_ID            0x05  //输入法定时器
#define MMI_KEYBAD_LIGHT_TIMER_ID   0x06  //键盘背光定时器
#define MMI_TP_SHORT_TIMER_ID       0x07  //触笔短长按定时器
#define MMI_TP_LONG_TIMER_ID        0x08  //触笔长按定时器

#define MMI_KBD_REPEAT_TIMER_ID     0x09  //重复按键定时器
#define MMI_AUTO_KEYLOCK_TIMER_ID   0x0a  //自动锁键盘定时器

#define MMI_TP_DBLCLK_TIMER_ID		0x0b  //触笔双击定时器
#define MMI_KBD_DBCLK_TIMER_ID		0x0c  //按键双击定时器

#define MMI_SCREEN_SAVER_TIMER_ID   0x0d  //屏保定时器
/*! \brief this timer id must the last the timeID, so when user
 * define the other timer id , user must modify the value of 
 * this timer id( MMI_LAST_TIMER_ID) 
 */ 
#define MMI_LAST_TIMER_ID           0x0d
#define MMK_MAX_TIMER_NUM   	    32  /*!< the maximum number of timer */

/*----------------------------------------------------------------------------*/ 
/*                         Type Declaration                                   */
/*----------------------------------------------------------------------------*/ 
typedef void (*MMI_TIMER_FUNC)( 
                               uint8  timer_id,
                               uint32 param
                               );

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/


/******************************************************************************/
/*! \brief init the timer
 *  \author  Louis wei
 */
/******************************************************************************/
PUBLIC void MMK_InitTimer(void);

/******************************************************************************/
/*! \brief start and create the timer in a window that is processing message
*  \param[in]		timer_id			the identify of timer
*  \param[in]		time_out			Specifies the expire value in milliseconds
*  \param[in]		is_period			the period timer if is_period is TRUE,the once timer if is_period is FALSE 
*  \author  Louis wei
*	\return  BOOLEAN
*/
/******************************************************************************/
PUBLIC BOOLEAN MMK_StartTimerEx(
                              uint8   			timer_id,   	//the identify of timer
                              uint32  			time_out,   	//Specifies the expire value in milliseconds
                              BOOLEAN 			is_period,   	//the period timer if is_period is TRUE
                              //the once timer if is_period is FALSE 
                               const char*	file_name_ptr,
                               uint32		    file_line 
                               );


/******************************************************************************/
/*! \brief start and create the timer in a appointed window
*  \param[in]		win_id			the window id
*  \param[in]		timer_id		the identify of timer
*  \param[in]		time_out		Specifies the expire value in milliseconds
*  \param[in]		is_period		the period timer if is_period is TRUE
*  \author  Louis wei
*	\return  BOOLEAN
*/
/******************************************************************************/
PUBLIC BOOLEAN MMK_StartWinTimerEx(
                                 MMI_HANDLE_T owner_handle,
                                 uint8           	timer_id,      	//the identify of timer
                                 uint32          	time_out,      	//Specifies the expire value in milliseconds
                                 BOOLEAN         	is_period,      	//the period timer if is_period is TRUE
                                 //the once timer if is_period is FALSE 
                                  const char*	file_name_ptr,
                                  uint32		    file_line 
                                  );


/******************************************************************************/
/*! \brief create a timer for a window and return the timer id
*  \param[in]		win_id			the window id
*  \param[in]		time_out		Specifies the expire value in milliseconds
*  \param[in]		is_period		the period timer if is_period is TRUE
*  \author  Louis wei
*	\return  uint8
*/
/******************************************************************************/
PUBLIC uint8 MMK_CreateWinTimerEx(
                                MMI_HANDLE_T owner_handle,
                                uint32  			time_out,   	//Specifies the expire value in milliseconds
                                BOOLEAN 			is_period,   	//the period timer if is_period is TRUE
                                //the once timer if is_period is FALSE 
                                 const char*	file_name_ptr,
                                 uint32		    file_line 
                                 );

/******************************************************************************/
/*! \brief create a timer for a window and return the timer id
*  \param[in]		time_out		Specifies the expire value in milliseconds
*  \param[in]		is_period		the period timer if is_period is TRUE
*  \author  Louis wei
*	\return  uint8
*/
/******************************************************************************/
PUBLIC uint8 MMK_CreateTimerEx(
                             uint32  			time_out,   	//Specifies the expire value in milliseconds
                             BOOLEAN 			is_period,   	//the period timer if is_period is TRUE
                             //the once timer if is_period is FALSE 
                              const char*	file_name_ptr,
                              uint32		    file_line 
                              );

/******************************************************************************/
/*! \brief stop timer
 *  \param[in]		timer_id			timer id
 *  \author  Louis wei
 *	\return  BOOLEAN
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_StopTimer(uint8 timer_id);

/******************************************************************************/
/*! \brief pause the timer
 *  \param[in]		timer_id			timer id
 *  \author  Louis wei
 *	\return  BOOLEAN
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_PauseTimer(uint8 timer_id);

/******************************************************************************/
/*! \brief reset the timer
 *  \param[in]		timer_id			timer id
 *	\return  BOOLEAN
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_ResumeTimer(uint8 timer_id);

/******************************************************************************/
/*! \brief judge whether the timer is active
 *  \param[in]		timer_id			timer id
 *  \author  Louis wei
 *	\return  BOOLEAN
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsTimerActive(uint8 timer_id);

/******************************************************************************/
/*! \brief  dispatch the timer message 
 *  \param[in]		signal_ptr	 the pointer of message which need dispatch
 *  \author  Louis wei
 *	\return  BOOLEAN
 */
/******************************************************************************/
PUBLIC void MMK_DispatchMSGTimer(Signal* signal_ptr);

/******************************************************************************/
/*! \brief delete timer by window handle
 *  \param[in]		hwnd			window handle
 *  \author  Louis wei
 */
/******************************************************************************/
PUBLIC void MMK_DeleteTimerByHandle( MMI_HANDLE_T owner_handle );

/*****************************************************************************/
// 	Description : create timer callback
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC uint8 MMK_CreateTimerCallbackEx(
                                     uint32            time_out, //Specifies the expire value in milliseconds
                                     MMI_TIMER_FUNC    func,     //func
                                     uint32            param,     //param
                                     BOOLEAN 			is_period,   	//the period timer if is_period is TRUE
                                                                                           //the once timer if is_period is FALSE
                                     const char*	file_name_ptr,
                                     uint32		    file_line 
									 );

PUBLIC BOOLEAN MMK_StartTimerCallbackEx(
                              uint8   timer_id,   //the identify of timer
                              uint32  time_out,   //Specifies the expire value in milliseconds 
                              MMI_TIMER_FUNC    func,     //func
                              uint32            param,     //param
                              BOOLEAN   is_period, //the period timer if is_period is TRUE
                                                                //the once timer if is_period is FALSE
							  const char*	file_name_ptr,
							  uint32		file_line
                              );                                    

#define MMK_StartTimer(timer_id, time_out, is_period)	\
	MMK_StartTimerEx(timer_id, time_out, is_period, _D_FILE_NAME, _D_FILE_LINE)
#define MMK_StartWinTimer(handle, timer_id, time_out, is_period) \
	MMK_StartWinTimerEx(handle, timer_id, time_out, is_period, _D_FILE_NAME, _D_FILE_LINE)
#define MMK_CreateWinTimer(handle, time_out, is_period)		\
	MMK_CreateWinTimerEx(handle, time_out, is_period, _D_FILE_NAME, _D_FILE_LINE)
#define MMK_CreateTimer(time_out, is_period)	\
	MMK_CreateTimerEx(time_out, is_period, _D_FILE_NAME, _D_FILE_LINE)
#define MMK_CreateTimerCallback(time_out, func, param, is_period)	\
	MMK_CreateTimerCallbackEx(time_out, func, param, is_period, _D_FILE_NAME, _D_FILE_LINE)
#define MMK_StartTimerCallback(timer_id, time_out, func, param, is_period)	\
	MMK_StartTimerCallbackEx(timer_id, time_out, func, param, is_period, _D_FILE_NAME, _D_FILE_LINE)
                              
/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    }
#endif
#endif

/*! @} */ 

