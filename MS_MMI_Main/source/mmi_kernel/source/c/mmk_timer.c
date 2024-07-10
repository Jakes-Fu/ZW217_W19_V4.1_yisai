/****************************************************************************
** File Name:      mmk_timer.c                                             *
** Author:                                                                 *
** Date:           03/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the function of timer     *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003       Louis.wei         Create
** 
****************************************************************************/
#define MMK_TIMER_C


/**-------------------------------------------------------------------------*
 **                         Include Files                                   *
 **------------------------------------------------------------------------*/
#include "mmi_kernel_trc.h"
#include "mmk_timer.h"
#include "mmk_kbd.h"
#include "mmk_app.h"
#include "mmi_default.h"
#include "mmk_msg_internal.h"
#include "mmk_window_internal.h"
#include "mmk_kbd_internal.h"
/**-----------------------------------------------------------------------------*
 **                         MACRO Declaration                                   *
 **----------------------------------------------------------------------------*/
#define MMK_MAX_WAITING_TIMER       2   //每个timer消息允许在队列中等待的最大个数,2个
#define MMIMMK_TIME_NAME_MAX_SIZE   40  //SCI_MAX_NAME_SIZE

/*----------------------------------------------------------------------------*/ 
/*                         Type Declaration                                   */
/*----------------------------------------------------------------------------*/ 
/*! \brief mmi timer struct */
typedef struct Timer_tag
{
    MMI_HANDLE_T        owner;  		/*!< timer owner */
    MMI_TIMER_FUNC      func;           //timer callback
    uint32              param;          //timer param
    uint32              ref_num;	    /*!< timer ref num, 防止产生重复的id */
    uint8               wait_num;       //time msg在MMI队列中的个数
    uint8               timerId;        /*!< timer Id Issued by the Timer Task */
    SCI_TIMER_PTR       timer_ptr;      /*!< the timer control block */

#if !defined RELEASE_INFO
	const char*			file_name_ptr;
	uint32				file_line;
#endif
} MMI_TIMER_T;

/**-------------------------------------------------------------------------*
 **                         Global Variables                                *
 **------------------------------------------------------------------------*/
/*the global variable of timer*/
LOCAL MMI_TIMER_T  s_mmk_timer[MMK_MAX_TIMER_NUM];

LOCAL uint32      s_timer_status;

LOCAL uint16 s_ref_high = 0;
/*the global variable to keep the key code*/
#ifdef TOUCH_PANEL_SUPPORT
extern BOOLEAN  g_tp_short_timer_active;
extern BOOLEAN  g_tp_long_timer_active;
extern BOOLEAN  g_tp_dblclk_timer_active;
extern uint16   g_tp_x, g_tp_y;
#endif

/**---------------------------------------------------------------------------*
 **                         External Function                                 *
 **---------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------*/
/*                    LOCAL FUNCTION DECLARE                                */
/*--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : the callback function of timer, and it will send the signal 
//                to the MMI
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL  void TimerCallBack(
                          uint32 input
                          );

/*****************************************************************************/
//  Description : check timer signal,in mmi queue timer msg number
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckTimerSignal(
                               MMI_TIMER_T  *timer_ptr
                               );

/*****************************************************************************/
//  Description : reduce wait timer msg in mmi queue
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void ReduceWaitTimerNum(
                              MMI_TIMER_T  *timer_ptr
                              );

/*****************************************************************************/
// 	Description : search the timer
//                
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL uint8 SearchTimer(
                         uint8 timer_id 
                         );
/*****************************************************************************/
// 	Description : start and create the timer in a application
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN StartAppTimer(
                            MMI_HANDLE_T   owner_handle,
                            MMI_TIMER_FUNC func,       //timer callback
                            uint32         param,      //timer param
                            uint8          timer_id,   //the identify of timer
                            uint32         time_out,   //Specifies the expire value in milliseconds
                            BOOLEAN        is_period,  //the period timer if is_period is TRUE
                                                       //the once timer if is_period is FALSE     
                            uint8           index,
							const char*		file_name_ptr,
							uint32			file_line
                            );

/*****************************************************************************/
// 	Description : assign a timer index or stop the timer which point timer index
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL uint8 AssignTimerIndex(
                             uint8    index
                             );

/*****************************************************************************/
// 	Description : delete timer index
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void DeleteTimerIndex(
                            uint8    index
                            );

/*****************************************************************************/
// 	Description : create a timer for a window and return the timer id
//	Global resource dependence : 
//  Author:Louis wei
//	Note: this function will assign the timer id for user
/*****************************************************************************/
LOCAL uint8 CreateTimer(
                        MMI_HANDLE_T   owner_handle,
                        MMI_TIMER_FUNC func,       //timer callback
                        uint32         param,      //timer param
                        uint32         time_out,   //Specifies the expire value in milliseconds
                        BOOLEAN        is_period,   //the period timer if is_period is TRUE
                                                   //the once timer if is_period is FALSE
						const char*		file_name_ptr,
						uint32			file_line												   
                        );


/*-------------------------------------------------------------------------*/
/*                         FUNCTIONS                                       */
/*-------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : init the timer
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
PUBLIC void MMK_InitTimer(void)
{
	SCI_MEMSET((void*)s_mmk_timer, 0, (sizeof(MMI_TIMER_T)*MMK_MAX_TIMER_NUM));
    s_timer_status = 0;
}

/*****************************************************************************/
// 	Description : start and create the timer in a window
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_StartTimerEx(
                              uint8   timer_id,   //the identify of timer
                              uint32  time_out,   //Specifies the expire value in milliseconds
                              BOOLEAN is_period,   //the period timer if is_period is TRUE
                              //the once timer if is_period is FALSE                     
                               const char*	file_name_ptr,
                               uint32		 file_line 
                               )
{
    return MMK_StartWinTimerEx( MMK_GetProcMsgHandle(), timer_id, time_out, is_period, file_name_ptr, file_line);
}

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
                              uint32		 file_line 
                             )
{
	uint8	timer_id = 0;

    timer_id = CreateTimer( MMK_GetProcMsgHandle(), 0, 0, time_out, is_period, file_name_ptr, file_line);
	//SCI_TRACE_LOW:"MMK_CreateTimer: timer_id = %d, s_timer_status = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_192_112_2_18_3_22_43_13,(uint8*)"dd", timer_id, s_timer_status);

	return timer_id;
}

/*****************************************************************************/
// 	Description : start and create the timer in a window
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_StartTimerCallbackEx(
                              uint8   timer_id,   //the identify of timer
                              uint32  time_out,   //Specifies the expire value in milliseconds
                              MMI_TIMER_FUNC    func,     //func
                              uint32            param,     //param
                              BOOLEAN is_period,   //the period timer if is_period is TRUE
                                                                //the once timer if is_period is FALSE
                               const char*	file_name_ptr,                                  
                               uint32		 file_line 
                              )
{   
    //SCI_TRACE_LOW:"MMK_StartTimerCallback: timer_id = %d, time_out = %d, is_period = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_212_112_2_18_3_22_43_14,(uint8*)"ddd",  timer_id, time_out, is_period);
    //SCI_ASSERT( MMI_NONE_TIMER_ID < timer_id );
    //SCI_ASSERT( 0 != time_out );
    if ( MMI_NONE_TIMER_ID == timer_id
        || 0 == time_out )
    {
        return FALSE;
    }

    return StartAppTimer( 0, func, param, timer_id, time_out, is_period, SearchTimer( timer_id ), file_name_ptr, file_line);  
}



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
                                     )
{
    return CreateTimer( 0, func, param, time_out, is_period, file_name_ptr, file_line );
}

/*****************************************************************************/
// 	Description : start and create the timer in a appointed window
//	Global resource dependence :
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_StartWinTimerEx(
                                 MMI_HANDLE_T    owner_handle,
                                 uint8           timer_id,      //the identify of timer
                                 uint32          time_out,      //Specifies the expire value in milliseconds
                                 BOOLEAN         is_period,      //the period timer if is_period is TRUE
                                 //the once timer if is_period is FALSE                     
                                 const char*	file_name_ptr,
                                 uint32		    file_line     
                                 )
{
    BOOLEAN result       = FALSE;
    uint8   search_index = 0; 
    
    //SCI_TRACE_LOW:"MMK_StartTimer: owner_handle = 0x%x, timer_id = %d, time_out = %d, is_period = %d!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_260_112_2_18_3_22_44_15,(uint8*)"dddd", owner_handle, timer_id, time_out, is_period);
    
    owner_handle = MMK_ConvertIdToHandle( owner_handle );
    
    //handle valid
    if( MMI_HANDLE_NONE != MMK_GetHandleType( owner_handle ) )
    {
        //SCI_ASSERT( MMI_NONE_TIMER_ID < timer_id );
        //SCI_ASSERT( 0 != time_out );

        if ( MMI_NONE_TIMER_ID == timer_id
            || 0 == time_out )
        {
            return FALSE;
        }
        
        search_index = SearchTimer( timer_id );    
        
        result = StartAppTimer( owner_handle, 0, 0, timer_id, time_out, is_period, search_index, file_name_ptr, file_line );
    }
    
    return result;
}

/*****************************************************************************/
// 	Description : create a timer for a window and return the timer id
//	Global resource dependence : 
//  Author: Great.Tian
//	Note: this function will assign the timer id for user
/*****************************************************************************/
PUBLIC uint8 MMK_CreateWinTimerEx(
                                MMI_HANDLE_T owner_handle,
                                uint32  time_out,   //Specifies the expire value in milliseconds
                                BOOLEAN is_period,   //the period timer if is_period is TRUE
                                //the once timer if is_period is FALSE                     
                                const char*	   file_name_ptr,
                                uint32		    file_line    
                                )
{
    uint8	timer_id = 0;
    
    // SCI_TRACE_LOW("MMK_CreateWinTimer: owner_handle = 0x%x, time_out = %d, is_period = %d!", owner_handle, time_out, is_period);
    
    owner_handle = MMK_ConvertIdToHandle( owner_handle );
    
    //handle valid
    if( MMI_HANDLE_NONE != MMK_GetHandleType( owner_handle ) )
    {
        if ( 0 == time_out )
        {
            return 0;
        }

        //SCI_ASSERT( 0 != time_out );
        
        timer_id = CreateTimer( owner_handle, 0, 0, time_out, is_period, file_name_ptr, file_line);
        // SCI_TRACE_LOW("MMK_CreateWinTimer: timer_id = %d, s_timer_status = %x", timer_id, s_timer_status);
    }
    
    return timer_id;
}

/*****************************************************************************/
// 	Description : create a timer for a window and return the timer id
//	Global resource dependence : 
//  Author:Louis wei
//	Note: this function will assign the timer id for user
/*****************************************************************************/
LOCAL uint8 CreateTimer(
                        MMI_HANDLE_T   owner_handle,
                        MMI_TIMER_FUNC func,       //timer callback
                        uint32         param,      //timer param
                        uint32         time_out,   //Specifies the expire value in milliseconds
                        BOOLEAN        is_period,   //the period timer if is_period is TRUE
                                                   //the once timer if is_period is FALSE
						const char*		file_name_ptr,
						uint32			file_line							
                        )
{
    BOOLEAN         id_exist = TRUE;
    static uint8    assign_id = MMI_LAST_TIMER_ID;
    uint8           i = 0;
    uint32          status = 0xFFFF;

    if ( 0 == time_out )
    {
        return 0;
    }

    //SCI_ASSERT( 0 != time_out );/*assert verified*/
    
    //assign the unused timerID
    if( 0xFFFFFFFF != s_timer_status )
    {
		//find an unused id
        do              
        {
            assign_id++;
            if( 0xFF == assign_id)
            {
                assign_id = MMI_LAST_TIMER_ID + 1;
            }  

			id_exist = FALSE;
            status = s_timer_status;
            for( i = 0; ( i < MMK_MAX_TIMER_NUM ) &&  ( 0 != status); i++ )
            {
                if( status & 0x00000001 )
                {
                    if( assign_id == s_mmk_timer[ i ].timerId )
                    {
						id_exist = TRUE;
                        break;
                    }
                }
                status = (uint32)(status >> 1);
            }
        }while(id_exist);

        //start the timer
        if( StartAppTimer( owner_handle, func, param, assign_id, time_out, is_period, MMK_MAX_TIMER_NUM, file_name_ptr, file_line) )
        {
            return assign_id;
        }
    }

    return 0;

}

/*****************************************************************************/
// 	Description : start and create the timer in a application for other window
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN StartAppTimer(
                            MMI_HANDLE_T   owner_handle,
                            MMI_TIMER_FUNC func,       //timer callback
                            uint32         param,      //timer param
                            uint8          timer_id,   //the identify of timer
                            uint32         time_out,   //Specifies the expire value in milliseconds
                            BOOLEAN        is_period,  //the period timer if is_period is TRUE
                                                       //the once timer if is_period is FALSE     
                            uint8          index,
							const char*		file_name_ptr,
							uint32			file_line
                            )
{
    uint8   timerIndex              = 0;
    
    if ( 0 == time_out )
    {
        return FALSE;
    }

    //check whether the pointer of owner is null
    SCI_ASSERT( MMI_NONE_TIMER_ID < timer_id );/*assert verified*/
    //SCI_ASSERT( 0 != time_out );/*assert verified*/

    timerIndex = AssignTimerIndex( index );
    SCI_ASSERT( MMK_MAX_TIMER_NUM > timerIndex );/*assert verified*/

	//SCI_TRACE_LOW:"StartAppTimer, timer_id = %d, timerIndex = %d, s_timer_status = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_417_112_2_18_3_22_44_16,(uint8*)"ddd", timer_id, timerIndex, s_timer_status);

    if( FALSE == is_period )
    {
        //create and start the once timer
        s_mmk_timer[ timerIndex ].timer_ptr = SCI_CreateTimer(
                                                "MMK timer",
                                                TimerCallBack,
                                                (uint32)(&s_mmk_timer[ timerIndex ]),
                                                time_out,
                                                FALSE);
    }
    else
    {
        //create and start the period timer
        s_mmk_timer[ timerIndex ].timer_ptr = SCI_CreatePeriodTimer(
                                                "MMK period timer",
                                                TimerCallBack,
                                                (uint32)(&s_mmk_timer[ timerIndex ]),
                                                time_out,
                                                FALSE);
    }

    //check whether it is successful to create a new timer
    if( SCI_NULL != s_mmk_timer[ timerIndex ].timer_ptr )
    {
        s_timer_status = (uint32)(s_timer_status | (0x01 << timerIndex));

        //get the hwnd of window which start timer
        s_mmk_timer[ timerIndex ].owner = owner_handle;
        s_mmk_timer[ timerIndex ].func  = func;
        s_mmk_timer[ timerIndex ].param = param;
        s_mmk_timer[ timerIndex ].timerId = timer_id;
        //低位放下标, 高位放计数器
        s_mmk_timer[ timerIndex ].ref_num = ((s_ref_high++)<<16) | timerIndex;

        s_mmk_timer[ timerIndex ].wait_num = 0;

#if !defined RELEASE_INFO
		s_mmk_timer[ timerIndex ].file_name_ptr = file_name_ptr;
		s_mmk_timer[ timerIndex ].file_line     = file_line;
#endif

		SCI_ActiveTimer(s_mmk_timer[ timerIndex ].timer_ptr);
        return TRUE;    
    }

    return FALSE;

}

/*****************************************************************************/
// 	Description : stop  timer
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_StopTimer(
                             uint8   timer_id   //the identify of timer
                             )
{

    uint8  index = MMK_MAX_TIMER_NUM;

    SCI_DisableIRQ();
    
    //find the pointer which timerid equal the timer_id
    index = SearchTimer( timer_id );

	// SCI_TRACE_LOW("MMK_StopTimer, timer_id = %d, index = %d", timer_id, index);

    if( MMK_MAX_TIMER_NUM > index )
    {
        DeleteTimerIndex( index );

        SCI_RestoreIRQ();
        return TRUE;
    }

    SCI_RestoreIRQ();
    return FALSE;
}

/*****************************************************************************/
// 	Description : the callback function of timer, and it will send the signal 
//                to the MMI
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL  void TimerCallBack(
                          uint32 input
                          )
{
    //* send the timer 
	MMI_TIMER_T        *timer ;
	MmiTimerExpiryS         *sendSignal;

    //check the pointer
    //SCI_TRACE_LOW("MMK_TIMER.C: in the function of TimerCallBack.");
    //SCI_ASSERT( PNULL != input );
    if ( PNULL == input )
    {
        return ;
    }

	timer = (MMI_TIMER_T *)input;
    sendSignal = PNULL;

    //check timer msg
    if (CheckTimerSignal(timer))
    {
	    // create the signal to send requesting task
	    MmiCreateSignal(APP_TIMER_EXPIRY,sizeof(MmiTimerExpiryS),(MmiSignalS**)&sendSignal);

	    sendSignal->timerId = timer->timerId;
        sendSignal->ref_num = timer->ref_num;
        sendSignal->Sender = P_APP;

	    MmiSendSignal(TASK_FL_ID, (MmiSignalS*)sendSignal);
    }
}

/*****************************************************************************/
//  Description : check timer signal,in mmi queue timer msg number
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CheckTimerSignal(
                               MMI_TIMER_T  *timer_ptr
                               )
{
    BOOLEAN     result = FALSE;

    SCI_ASSERT(PNULL != timer_ptr);/*assert verified*/

    if (MMK_MAX_WAITING_TIMER > timer_ptr->wait_num)
    {
        timer_ptr->wait_num++;
        result = TRUE;
    }
    else
    {
        result = FALSE;
        //SCI_TRACE_LOW:"CheckTimerSignal:the timer msg is too much! 0x%x,%d,%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_556_112_2_18_3_22_44_17,(uint8*)"ddd",timer_ptr->owner,timer_ptr->timerId,timer_ptr->wait_num);
    }

    return (result);
}

/*****************************************************************************/
//  Description : reduce wait timer msg in mmi queue
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
LOCAL void ReduceWaitTimerNum(
                              MMI_TIMER_T  *timer_ptr
                              )
{
    SCI_ASSERT(PNULL != timer_ptr);/*assert verified*/

    if (0 < timer_ptr->wait_num)
    {
        timer_ptr->wait_num--;
    }
}

/*****************************************************************************/
// 	Description : dispatch the timer message 
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DispatchMSGTimer(
                                 Signal* signal_ptr // the pointer of message which need dispatch
                                 )
{
    MmiTimerExpiryS*    timer = PNULL;
    uint8               timer_id        = 0;
    uint16              index         = 0;

    //SCI_ASSERT( PNULL != signal_ptr );
    if ( PNULL == signal_ptr )
    {
        return ;
    }

    timer = (MmiTimerExpiryS*)signal_ptr;

    //低位放下标, 高位放计数器
    index = timer->ref_num & 0xFFFF;//(uint16)(timer->ref_num % MMK_MAX_TIMER_NUM);

    if ( index >= MMK_MAX_TIMER_NUM )
    {
        //SCI_TRACE_LOW:"MMK_TIMER.C:The timer(timerID = %d, ref_num = %d) index is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_603_112_2_18_3_22_44_18,(uint8*)"dd", timer->timerId ,timer->ref_num);
        
        return;
    }

    if( ( timer->ref_num == s_mmk_timer[ index ].ref_num ) &&
        ( timer->timerId == s_mmk_timer[ index ].timerId ) )
    {
        //reduce wait timer number
        ReduceWaitTimerNum(&s_mmk_timer[index]);

        switch( timer->timerId )
        {
        case MMI_KBD_TIMER_ID_1:
		case MMI_KBD_TIMER_ID_2:
		case MMI_KBD_TIMER_ID_3:
			MMK_HandleKeyLongPressTimer(timer->timerId);
            break;

        case MMI_KBD_REPEAT_TIMER_ID:
            MMK_HandleKeyRepeatPressTimer();
            break;

        //the timer is backlight_timer
        case MMI_LCD_LIGHT_TIMER_ID:
			MMIDEFAULT_HandleBackLightOver();
            break;

        case MMI_AUTO_KEYLOCK_TIMER_ID:
            MMIDEFAULT_HandleAutoKeylockOver();
            break;

        case MMI_KEYBAD_LIGHT_TIMER_ID:
            MMIDEFAULT_StopKeybadBackLightTimer();
            MMIDEFAULT_SetKeybadBackLight(FALSE);
            break;
            
        case MMI_SCREEN_SAVER_TIMER_ID:
            MMIDEFAULT_HandleScreenSaverTimer();
            break;

        case MMI_TP_SHORT_TIMER_ID:
#ifdef TOUCH_PANEL_SUPPORT
            g_tp_short_timer_active = FALSE;
            MMK_DispatchMSGTpShort(g_tp_x, g_tp_y);
#endif
            break;

        case MMI_TP_LONG_TIMER_ID:
#ifdef TOUCH_PANEL_SUPPORT
            g_tp_long_timer_active = FALSE;
            MMK_DispatchMSGTpLong(g_tp_x, g_tp_y);
#endif
            break;
         
		case MMI_TP_DBLCLK_TIMER_ID:
#ifdef TOUCH_PANEL_SUPPORT
			g_tp_dblclk_timer_active = FALSE;
			MMK_ReInitDblClkState(FALSE);
#endif
			break;
        
        case MMI_KBD_DBCLK_TIMER_ID:
            MMK_ReInitKbdDblclkState(TRUE);
			break;

		default:  
            timer_id = timer->timerId ;

            // 加入对LcdSleepTimer的处理，因为背光和唤醒Lcd时间不同步，
            // 需要Timer进行同步
            if (MMIDEFAULT_IsLcdSleepTimer(timer_id))
            {
                // 点亮背光，关闭Timer
                MMIDEFAULT_SetLcdBackLight(TRUE);

                MMIDEFAULT_StopLcdSleepTimer();
            }
            else if( 0 != s_mmk_timer[ index ].owner )
            {
                MMK_DispatchToHandle(s_mmk_timer[ index ].owner, MSG_TIMER, (DPARAM)&timer_id );
            }
            else if ( PNULL != s_mmk_timer[ index ].func )
            {
                s_mmk_timer[ index ].func( timer_id, s_mmk_timer[ index ].param );
            }
            break;
        }

        //stop the one times timer
        // 如果在派发过程中,这个Timer被停掉了，并且应用又启动了一个新的Timer，
        // 并且这个新的Timer的Index刚好与派发的相同，那么程序接下来就会把这个Timer
        // 停掉，这样会导致比较严重的问题。
        if( timer->ref_num == s_mmk_timer[ index ].ref_num
           && timer->timerId == s_mmk_timer[ index ].timerId
           && (PNULL != s_mmk_timer[ index ].timer_ptr) 
           && (SCI_FALSE == SCI_IsTimerActive(s_mmk_timer[ index ].timer_ptr))  )
        {
            DeleteTimerIndex( index );
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMK_TIMER.C:The timer(timerID = %d, ref_num = %d) was stopped"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_696_112_2_18_3_22_44_19,(uint8*)"dd", timer->timerId ,timer->ref_num);
    } 
}

/*****************************************************************************/
// 	Description : pause the timer
//                
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_PauseTimer(
                              uint8    timer_id
                              )
{
    BOOLEAN     result = FALSE;
    uint16      index = MMK_MAX_TIMER_NUM;

    //find the pointer which timerid equal the timer_id
    index = SearchTimer( timer_id );

    if (MMK_MAX_TIMER_NUM > index)
    {
        if ((PNULL != s_mmk_timer[index].timer_ptr) &&
            (SCI_IsTimerActive( s_mmk_timer[ index ].timer_ptr)))
        {
            if( SCI_SUCCESS == SCI_DeactiveTimer(s_mmk_timer[index].timer_ptr) )
            {
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMK_PauseTimer:SCI_DeactiveTimer fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_726_112_2_18_3_22_45_20,(uint8*)"");
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMK_PauseTimer:index= %d,timer_ptr = 0x%x!"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_731_112_2_18_3_22_45_21,(uint8*)"dd",index,s_mmk_timer[ index ].timer_ptr);
        }
    }

    return (result);
}

/*****************************************************************************/
// 	Description : reset the timer
//                
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ResumeTimer(
                               uint8    timer_id
                               )
{
    BOOLEAN     result = FALSE;
    uint16      index = MMK_MAX_TIMER_NUM;
    uint32      remaining_time = 0;

    //find the pointer which timerid equal the timer_id
    index = SearchTimer( timer_id );

    if (MMK_MAX_TIMER_NUM > index)
    {
        if ((PNULL != s_mmk_timer[ index ].timer_ptr) && 
            (!SCI_IsTimerActive( s_mmk_timer[ index ].timer_ptr)))
        {
            char	    time_name[MMIMMK_TIME_NAME_MAX_SIZE] = {0};
    		BOOLEAN	    is_active = FALSE;
    		uint32	    reschedule_time = 0;

            SCI_GetTimerInfo(s_mmk_timer[index].timer_ptr,time_name,&is_active,&remaining_time,&reschedule_time);
            
            if (0 < remaining_time)
            {
                if (SCI_SUCCESS == SCI_ActiveTimer(s_mmk_timer[ index ].timer_ptr))
                {
                    result = TRUE;
                }
            }
            else
            {
                //SCI_TRACE_LOW:"MMK_ResumeTimer:remaining_time %d is error!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_774_112_2_18_3_22_45_22,(uint8*)"d",remaining_time);

            }
        }
    }

    return (result);
}


/*****************************************************************************/
// 	Description :  judge whether the timer is active
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsTimerActive(
                                 uint8   timer_id
                                 )
{
    uint16  timer_Index = 0;

    //check whether the timer_id is law
    //SCI_ASSERT( MMI_NONE_TIMER_ID < timer_id);
    if ( MMI_NONE_TIMER_ID == timer_id )
    {
        return FALSE;
    }

    if( 0 != s_timer_status )
    {
        timer_Index = SearchTimer( timer_id );

        if( ( MMK_MAX_TIMER_NUM > timer_Index ) && 
            ( PNULL != s_mmk_timer[ timer_Index ].timer_ptr ) )
        {
            //check whether the timer_id is still active
            if( SCI_TRUE == SCI_IsTimerActive( s_mmk_timer[ timer_Index ].timer_ptr ) )
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    
    return FALSE;
}

/*****************************************************************************/
// 	Description : search the timer
//                
//	Global resource dependence : 
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL uint8 SearchTimer(
                         uint8 timer_id 
                         )
{
    uint8   i = MMK_MAX_TIMER_NUM;
    uint32  status = s_timer_status;

    if( 0 != s_timer_status )
    {
        //find the pointer which timerid equal the timer_id
        for( i = 0;  i < MMK_MAX_TIMER_NUM ; i++ )
        {
            if( ( status & 0x00000001  ) && ( timer_id == s_mmk_timer[ i ].timerId ) )
            {
                break;
            }
            status = (uint32)(status >> 1);
            if( 0 == status )
            {
                return MMK_MAX_TIMER_NUM;
            }
        }
    }

    return i;
}

/*****************************************************************************/
// 	Description : start and create the timer in a application for other window
//	Global resource dependence :
//  Author:Louis wei
//	Note:
/*****************************************************************************/
LOCAL uint8 AssignTimerIndex(
                             uint8    index
                             )
{
    static uint8    s_timer_index = 0;
    uint8           timerIndex = 0;
    uint32          status = 0;

//	SCI_TRACE_LOW("AssignTimerIndex, index = %d, s_timer_status = %x, s_timer_index = %d", index, s_timer_status, s_timer_index);

    if( MMK_MAX_TIMER_NUM == index )
    {        
         status = (uint32)(~s_timer_status);

         if( 0 == status )
         {
			 //SCI_TRACE_LOW:"AssignTimerIndex, status fail = %d"
			 SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_875_112_2_18_3_22_45_23,(uint8*)"d", status);
             return MMK_MAX_TIMER_NUM;
         }

        if( ( status >> s_timer_index ) )
        {
            //the timer index will in the scope of s_timer_index to MMK_MAX_TIMER_NUM - 1
            status = (uint32)(status >> s_timer_index);

            for( ; (!( status & 0x00000001 )) && ( MMK_MAX_TIMER_NUM > s_timer_index ); status = (uint32)(status >> 1))
            {
                s_timer_index++;
            }

			if ( MMK_MAX_TIMER_NUM <= s_timer_index )
			{
				status = (uint32)(~s_timer_status);
				s_timer_index = 0;
				for (; (!( status & 0x00000001 )) && ( MMK_MAX_TIMER_NUM > s_timer_index ); status = (uint32)(status >> 1))
				{
					s_timer_index++;
				}
				SCI_ASSERT( MMK_MAX_TIMER_NUM > s_timer_index );/*assert verified*/
			}

        }
        else
        {
            //the timer index will in the scope of 0 to s_timer_index - 1
            status = (uint32)(~s_timer_status);
            s_timer_index = 0;
            for (; (!( status & 0x00000001 )) && ( MMK_MAX_TIMER_NUM > s_timer_index ); status = (uint32)(status >> 1))
            {
                s_timer_index++;
            }
			SCI_ASSERT( MMK_MAX_TIMER_NUM > s_timer_index );/*assert verified*/
        }

        timerIndex = s_timer_index;
//		SCI_TRACE_LOW("AssignTimerIndex, get timerIndex = %d", s_timer_index);
		
        s_timer_index++;
		if(MMK_MAX_TIMER_NUM <= s_timer_index)
		{
			s_timer_index = (uint8)(s_timer_index % MMK_MAX_TIMER_NUM);
		}
    }
    else
    {
        timerIndex = index;

        //stop the timer which saved in the index postion
        DeleteTimerIndex( index );
    }

    return timerIndex;
}

/*****************************************************************************/
// 	Description : delete timer index
//	Global resource dependence :
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void DeleteTimerIndex(
                            uint8    index
                            )
{
    if (PNULL != s_mmk_timer[index].timer_ptr)
    {
        SCI_DeleteTimer( s_mmk_timer[index].timer_ptr );

        SCI_MEMSET( &s_mmk_timer[ index ], 0, sizeof(MMI_TIMER_T));
        
        s_timer_status = (uint32)(s_timer_status^(0x01<<index));
    }
}

/*****************************************************************************/
// 	Description : 关闭窗口时将Timer列表中的对应的Timer关掉，
//                实际上这应该由应用来关闭的
//	Global resource dependence :
//  Author: Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DeleteTimerByHandle(
                                    MMI_HANDLE_T owner_handle
                                    )
{
    if( 0 != owner_handle )
    {
        uint8 i = 0;    
        
        for( i = 0; i < MMK_MAX_TIMER_NUM; i++ )
        {
            if ( owner_handle == s_mmk_timer[i].owner)
            {
                //SCI_TRACE_LOW:"MMK_DeleteTimerByHandle: timer_id=%d, owner=%d!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_TIMER_968_112_2_18_3_22_45_24,(uint8*)"dd", s_mmk_timer[i].timerId, s_mmk_timer[i].owner);
                
                DeleteTimerIndex( i );
            }
        }
    }
}
