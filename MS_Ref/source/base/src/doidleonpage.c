/******************************************************************************
 ** File Name:      DoIdleOnPage.c                                            *
 ** Author:         Lin.liu                                                   *
 ** DATE:           05/29/2003                                                *
 ** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    Do Idle when layer1 receive new page.                     *
 **                 do something instead of create a timer to saving capacity *
 **                 the space between page :                                  *
 **                   (2--9) x 51 x 4.615ms == (2--9) x 235ms = 470ms--2118ms *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/29/2003     Lin.liu          Create.                                   *
 ******************************************************************************/
 
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "ms_ref_base_trc.h"
#include "dal_chr.h"    // call the charge do idle function.
#include "doidleonpage.h"
#include "tb_hal.h"
#include "watchdog.h"
#include "layer1_engineering.h"
#include "threadx_os.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Debug Flag                                        *
 **---------------------------------------------------------------------------*/
#define DEBUG_DO_IDLE  1

#ifdef  DEBUG_DO_IDLE
#define DEBUG_PRINT( _format_string )   SCI_TRACE_LOW _format_string 
#else
#define DEBUG_PRINT( _format_string )  
#endif


//
// When define this macro, call the system do idle function on page,
// we provide a funciton, and layer1 will call it after process one page.
// If not define it, we create a periodic timer to do it.
//

//#define DO_IDLE_ON_PAGE   1
/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
 
#ifdef WIN32
#undef DO_IDLE_ON_PAGE
#endif

#ifndef DO_IDLE_ON_PAGE
#ifdef MODEM_PLATFORM
#define DO_IDLE_TIME                10000                    // 10 second
#else
#define DO_IDLE_TIME                3000                    // 3 second
#endif
static  SCI_TIMER_PTR               do_idle_timer = NULL;   // timer handler
#endif

#define DOIDLE_CALLBACK_FUNC_NUM_MAX				20
LOCAL DOIDLE_CALLBACK_FUNC s_DoIdle_callback_func_tab[DOIDLE_CALLBACK_FUNC_NUM_MAX] = {PNULL};
LOCAL uint32 s_DoIdle_register_callback_num = 0;

// Charge Init-ed flag
//#define DO_IDLE_INIT_MAGIC          0x444F4954              // DOIT = do idle timer
//static uint32  do_idle_init_flag  = 0;

static void DoIdle_Callback( uint32 param );

/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to register DoIdle callback    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN DoIdle_RegisterCallback(DOIDLE_CALLBACK_FUNC func)
{
	uint32 i;
	BOOLEAN ret_value = SCI_FALSE;
	BOOLEAN func_register = SCI_FALSE;
	
	TX_INTERRUPT_SAVE_AREA
	
	// to check if the func has register
	for(i = 0; i < DOIDLE_CALLBACK_FUNC_NUM_MAX; i++)
	{
		if(func == s_DoIdle_callback_func_tab[i])
		{
			func_register = SCI_TRUE;	
		
			//SCI_TRACE_LOW:"DoIdle_RegisterCallback: func 0x%x has register !"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOIDLEONPAGE_91_112_2_18_1_4_1_16,(uint8*)"d");
			break;
		}	
	}
	
	if(!func_register)
	{
	    TX_DISABLE

		// if the func hasn't register, find a empty in the table for the func
		for(i = 0; i < DOIDLE_CALLBACK_FUNC_NUM_MAX; i++)
        {
            if(PNULL == s_DoIdle_callback_func_tab[i])
            {   
                s_DoIdle_callback_func_tab[i] = func;				
                s_DoIdle_register_callback_num++;			                    
                ret_value = SCI_TRUE;

                break;
            }		
		}

        TX_RESTORE
    }
	else
	{
		ret_value = SCI_TRUE;
	}	
	
	//SCI_TRACE_LOW:"DoIdle_RegisterCallback: return %d, func addr %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOIDLEONPAGE_117_112_2_18_1_4_1_17,(uint8*)"dd", ret_value, func);
	
	return ret_value;
}

/*****************************************************************************/
//  Description:    This function is used to unregister DoIdle callback    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN DoIdle_UnRegisterCallback(DOIDLE_CALLBACK_FUNC func)
{
	uint32 i;
	BOOLEAN ret_value = SCI_FALSE;

    TX_INTERRUPT_SAVE_AREA

    TX_DISABLE
        
	for(i = 0; i < DOIDLE_CALLBACK_FUNC_NUM_MAX; i++)
	{         
        if(s_DoIdle_callback_func_tab[i] == func)
        {            
            s_DoIdle_callback_func_tab[i] = PNULL;			
            s_DoIdle_register_callback_num--;            
            ret_value = SCI_TRUE;

            break;
        }        
	}	

    TX_RESTORE
	//SCI_TRACE_LOW:"DoIdle_RegisterCallback: return %d, func addr %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOIDLEONPAGE_143_112_2_18_1_4_1_18,(uint8*)"dd", ret_value, func);
	
	return ret_value;
}

/*****************************************************************************/
//  Description:    This function is used to do callback
//  Author:         Liangwen.Zhen
//  Note:
/*****************************************************************************/
LOCAL uint32 DoIdle_DoCallback(uint32 param)
{
    uint32 i;
    DOIDLE_CALLBACK_FUNC pFunc = PNULL;
    static uint32 first_flag = 0;

    TX_INTERRUPT_SAVE_AREA

    if (SCI_IsTimerActive (do_idle_timer))
    {
        SCI_DeactiveTimer(do_idle_timer);
        SCI_ChangeTimer(do_idle_timer, DoIdle_Callback, DO_IDLE_TIME);
        SCI_ActiveTimer(do_idle_timer);
    }

    for(i = 0; i < DOIDLE_CALLBACK_FUNC_NUM_MAX; i++)
    {
        TX_DISABLE
            
        if(PNULL != s_DoIdle_callback_func_tab[i])
        {
           pFunc = s_DoIdle_callback_func_tab[i];
        }
        else
        {
           pFunc = NULL;
        }

        TX_RESTORE

        if(pFunc)
        {
           (pFunc)(param);
        }
    }

#ifdef RELEASE_INFO
    if(!first_flag)
    {
        SIO_Set_cp_stop_log();
        first_flag = 1;
    }
#endif

	return s_DoIdle_register_callback_num;
}

//
// if defined DO_IDLE_ON_PAGE, this function be called on page,
// otherwise it is the timer callback.
//
// Maybe the function be called not from a thread, pleas CARE it.
//
static void DoIdle_Callback( uint32 param )
{
    DoIdle_DoCallback(param);
}

//
//  Init Do Idle system. alloc resource.
//
int DoIdle_Init( void )
{
#ifndef DO_IDLE_ON_PAGE
	// Clear function table
	//SCI_MEMSET(s_DoIdle_callback_func_tab, PNULL, sizeof(s_DoIdle_callback_func_tab));

#if defined(PLATFORM_SC6530) || defined(PLATFORM_SC6531EFM)
//|| defined(PLATFORM_UWS6121E)
    L1PagingTimer_RegisterCallback((L1PAGETIMER_CALLBACK_FUNC)DoIdle_Callback);
#endif

//    if(  DO_IDLE_INIT_MAGIC != do_idle_init_flag )
    {
        do_idle_timer = SCI_CreatePeriodTimer( "DoIdleOnPage",
    	    DoIdle_Callback,
    	    DO_IDLE_TIME, DO_IDLE_TIME, SCI_AUTO_ACTIVATE );
    	    
    	if( NULL == do_idle_timer )
    	{
    	    //DEBUG_PRINT:"%s(%d) : Create timer failed."
    	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DOIDLEONPAGE_214_112_2_18_1_4_1_19,(uint8*)"sd", __FILE__, __LINE__  );
    	    return 0;
    	}   	
    }
#endif   
//    do_idle_init_flag = DO_IDLE_INIT_MAGIC; 
    
    return 1;
}

#if 0
//
//  Release the resource.
//  
void DoIdle_Close( void )
{
//    if(  DO_IDLE_INIT_MAGIC == do_idle_init_flag )
    {
#ifndef DO_IDLE_ON_PAGE

        // deactive it first.
        if( SCI_IsTimerActive( do_idle_timer ) )
        {
            SCI_DeactiveTimer( do_idle_timer );
        }
        SCI_DeleteTimer( do_idle_timer );
#endif  
    }   
//    do_idle_init_flag = 0; 
}
#endif

//
// This function be called after layer1 process one page.
//
void DoIdle_OnPage( void )
{
#ifdef DO_IDLE_ON_PAGE
    DoIdle_Callback( 0 );
#endif    
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 


//  End of DoIdleOnPage.c
