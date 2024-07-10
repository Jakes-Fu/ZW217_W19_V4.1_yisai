/******************************************************************************
 ** File Name:      watchdog.c                                                *
 ** Author:         Richard Yang                                              *
 ** DATE:           23/01/2002                                                *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 23/01/2002     Richard.Yang     Create.                                   *
 ** 26/03/2003     Lin.liu          Added WatchDog Timer Feature              *
 ** 26/09/2009     Yi.Qiu           Added Soft WatchDog Feature               *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "watchdog.h"
#include "os_api.h"
#include "os_param.h"
#include "tb_hal.h"
#include "cmddef.h"
#include "dal_power.h"
#include "doidleonpage.h"
#include "syscnt_drvapi.h"
#include "threadx_os.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
#define _SWDG_DEBUG
//#define _WDG_RECORD_

#ifdef _SWDG_DEBUG
#include "sci_log.h"

#define SWDG_ASSERT(_x_)            SCI_ASSERT(_x_)
#define SWDG_VERIFY(_x_)            SCI_ASSERT(_x_)
#define SWDG_ERR_TRACE(_x_)         SCI_TraceLow _x_
#define SWDG_LOG_TRACE(_x_)         SCI_TraceLow _x_
#define SWDG_INF_TRACE(_x_)         SCI_TraceLow _x_
#else
#define SWDG_ASSERT(_x_)            ((void)0)
#define SWDG_VERIFY(_x_)            ((void)(_x_))
#define SWDG_ERR_TRACE(_x_)         ((void)0)
#define SWDG_LOG_TRACE(_x_)         ((void)0)
#define SWDG_INF_TRACE(_x_)         ((void)0)
#endif /* _SWDG_DEBUG */

#ifdef MODEM_PLATFORM
#define WDG_FEED_DOG_VAL            (15000) //uint : ms
#define SYSCNT_FEED_VAL (WDG_FEED_DOG_VAL - 2000)// 13s
#define SYSCNT_INIT_VAL (WDG_INIT_COUNT * 1000 / 32768  - 2000)// 18s
#else
#define WDG_FEED_DOG_VAL            (60000) //uint : ms 60s
#define SYSCNT_FEED_VAL (WDG_FEED_DOG_VAL - 2000)// 58s
#define SYSCNT_INIT_VAL (WDG_INIT_COUNT * 1000 / 16384  - 2000)// 58s
#endif

#ifdef _WDG_RECORD_
#define WDG_RECORD_POINT             8
#endif
/******************************************************************************
 * Static variables definitions
 ******************************************************************************/
LOCAL LIST_HEAD        g_SwdgList;           /* List of devices */

#ifdef _WDG_RECORD_
LOCAL uint32 s_wdg_record_buf[WDG_RECORD_POINT];
LOCAL uint32 s_wdg_record_idx;
#endif

LOCAL SCI_TIMER_PTR wdg_assert_timer;

/******************************************************************************
 * Static functions prototypes
 ******************************************************************************/
LOCAL void _SWDG_Init (void);
LOCAL void _WatchDogFeed (void);
LOCAL void _WatchDogTimerClose (void);
LOCAL void _CheckAllTask (void);
LOCAL SWDG_OBJECT *_LookupSwdgObject (const char *name);

/******************************************************************************
 * Implementations
 ******************************************************************************/
/******************************************************************************
 * SysTimer_AssertBeforeWDGReset
 ******************************************************************************/
LOCAL void SysTimer_AssertBeforeWDGReset(uint32 param)
{
    SCI_PASSERT ( SCI_FALSE, ("SysTimer Assert Before Watchdog Reset!"));/*assert verified*/
    //SWDG_LOG_TRACE(("%d: SysTimer_AssertBeforeWDGReset", SCI_GetTickCount()));
}
/******************************************************************************
 * _SysTimer_Load
 ******************************************************************************/
LOCAL void _SysTimer_Load(uint32 param)
{
    SCI_DeactiveTimer(wdg_assert_timer);
    SCI_ChangeTimer(wdg_assert_timer, SysTimer_AssertBeforeWDGReset, 
        SYSCNT_FEED_VAL);
    SCI_ActiveTimer(wdg_assert_timer);
}
/******************************************************************************
 * _SysTimer_Init
 ******************************************************************************/
LOCAL void _SysTimer_Init()
{
    const char  *timer_name = "WDG_ASSERT_TIMER";
    wdg_assert_timer = SCI_CreatePeriodTimer(timer_name, 
        SysTimer_AssertBeforeWDGReset, 0, SYSCNT_INIT_VAL,TX_AUTO_ACTIVATE);
    DoIdle_RegisterCallback ( (DOIDLE_CALLBACK_FUNC) _SysTimer_Load);
}

/******************************************************************************
 * WatchDogTimerInit
 ******************************************************************************/
PUBLIC void WatchDogTimerInit (void)
{
#ifndef WIN32
    uint32 mode = POWER_GetResetMode();

    /* Stop it first */
    _WatchDogTimerClose();

    /* @binggo add :Init Watchdog under belowing mode */
    if ( (NORMAL_MODE == mode)
        || (ASSERT_BACK_MODE == mode) 
        || (WATCHDOG_RESET_MODE == mode)
        || (RESET_MODE == mode)
        || (SEVENSEC_RESET_MODE == mode))
    {
#ifndef  JTAG_SUPPORT
        HAL_WDG_INIT ();

        /* Register hardware watchdog callback function */
        DoIdle_RegisterCallback ( (DOIDLE_CALLBACK_FUNC) _WatchDogFeed);

#if 0
        /* Instead of reset by WDG, issue assert info */
        _SysTimer_Init();
#else 
        /* 使用硬件WATCHDOG 中断机制，不需要_SysTimer_Init */
        // Register ISR of WDG.
        //ISR_RegHandler (TB_WDG_INT, WDG_TimerHandler);

#endif
        
#endif
    }

    /* Init soft watchdog to monitor tasks */
    _SWDG_Init();

    /* Register software watchdog callback function */
#ifndef  JTAG_SUPPORT
    DoIdle_RegisterCallback ( (DOIDLE_CALLBACK_FUNC) _CheckAllTask);
#endif

#endif
}

/******************************************************************************
 * WatchDogTimerClose
 ******************************************************************************/
LOCAL void _WatchDogTimerClose (void)
{
    /* Now disable wathch dog timer */
    HAL_WDG_STOP();
}

/******************************************************************************
 * _WatchDogFeed
 ******************************************************************************/
LOCAL void _WatchDogFeed (void)
{
    WDG_TimerLoad (WDG_FEED_DOG_VAL);

#ifdef _WDG_RECORD_
    {
        s_wdg_record_buf[s_wdg_record_idx] = SCI_GetTickCount();

        s_wdg_record_idx++;

        if(s_wdg_record_idx >= WDG_RECORD_POINT)
        {
            s_wdg_record_idx = 0;
        }
    }
#endif
}

/******************************************************************************
 * SWDG_RegTask
 ******************************************************************************/
PUBLIC SWDG_OBJECT *SWDG_RegTask (const char *name, uint32 time_out)
{
    SWDG_OBJECT *p = SCI_NULL;

    SWDG_ASSERT (SCI_NULL != name);/*assert verified*/

    if (SCI_NULL != _LookupSwdgObject (name))
    {
        SWDG_ERR_TRACE ( (
                             "[SWDG:SWDG_RegTask] The [%s] object has existed\r\n",
                             name));

        return SCI_NULL;
    }

    if (SCI_NULL == (p = (SWDG_OBJECT *) SCI_ALLOC_APP (sizeof (*p))))
    {
        SWDG_ERR_TRACE ( (
                             "[SWDG:SWDG_RegTask] Fail to allocate %d bytes memory\r\n",
                             sizeof (*p)));

        return SCI_NULL;
    }

    SCI_MEMSET (p, 0, sizeof (*p));//lint !e516

    p->task_name = name;
    p->time_out = time_out;

    LIST_ADD (&p->next, &g_SwdgList);

    return p;
}

/******************************************************************************
 * SWDG_UnregTask
 ******************************************************************************/
PUBLIC void SWDG_UnregTask (SWDG_OBJECT *p)
{
    SWDG_ASSERT (SCI_NULL != p);/*assert verified*/

    /* Delete object and free memory */
    LIST_DEL (&p->next);
    SCI_FREE (p);

    SWDG_LOG_TRACE ( ("[SWDG:SWDG_UnregTask] IN (%s)\r\n",
                      p->task_name));
}

/******************************************************************************
 * SWDG_SetStatus
 ******************************************************************************/
PUBLIC void SWDG_SetStatus (SWDG_OBJECT *p, BOOLEAN on)
{
    SWDG_ASSERT (SCI_NULL != p);/*assert verified*/

    /* Task will set this status */
    p->task_status = on;

    SWDG_LOG_TRACE ( ("[SWDG:SWDG_SetStatus] IN (%s, %d)\r\n",
                      p->task_name, on));
}

/******************************************************************************
 * SWDG_GetStatus
 ******************************************************************************/
LOCAL BOOLEAN SWDG_GetStatus (SWDG_OBJECT *p)
{
    SWDG_ASSERT (SCI_NULL != p);/*assert verified*/

    SWDG_LOG_TRACE ( ("[SWDG:SWDG_GetStatus] IN (%s) OUT(%d)\r\n",
                      p->task_name, p->task_status));

    /* Get task status */
    return p->task_status;
}

/******************************************************************************
 * SWDG_FreshTick
 ******************************************************************************/
PUBLIC void SWDG_FreshTick (SWDG_OBJECT *p)
{
    uint32 curr_tick;

    SWDG_ASSERT (SCI_NULL != p);/*assert verified*/

    /* Get current tick */
    curr_tick = SCI_GetTickCount();

    /* Task will fresh this tick */
    p->fresh_tick = curr_tick;

    SWDG_LOG_TRACE ( ("[SWDG:SWDG_FreshTick] IN (%s, %d)\r\n",
                      p->task_name, curr_tick));
}

/******************************************************************************
 * SWDG_Initialize
 ******************************************************************************/
LOCAL void _SWDG_Init (void)
{
    SWDG_LOG_TRACE ( ("[SWDG:Initialize] IN()\r\n"));

    INIT_LIST_HEAD (&g_SwdgList);

#ifdef _WDG_RECORD_
    SCI_MEMSET(s_wdg_record_buf, 0x69, sizeof(s_wdg_record_buf));/*lint !e516 -e516*/
    s_wdg_record_idx = 0;
#endif

}

/******************************************************************************
 * SWDG_CheckAll
 ******************************************************************************/
LOCAL void _CheckAllTask (void)
{
    LIST_HEAD *list;
 //   uint8 assert_mode;
    uint32 curr_tick;

    /* Get current tick */
    curr_tick = SCI_GetTickCount();

    SWDG_LOG_TRACE ( ("[SWDG:SWDG_CheckAll] current tick: %d\r\n",
                      curr_tick));

    /* Check all tasks */
    list = g_SwdgList.next;

    while (&g_SwdgList != list)
    {
        SWDG_OBJECT *p;

        p = LIST_ENTRY (list, SWDG_OBJECT, next);       /*lint !e413*/

        if (SCI_NULL == p)			/*lint !e774*/
        {
            return;    /*lint !e774*/
        }

        SWDG_LOG_TRACE ( ("[SWDG:SWDG_CheckAll] %s, fresh_tick:%d, %d \r\n",
                          p->task_name, p->fresh_tick, p->task_status));

        /* System count will come back to 0 after about 40 days */
        if (curr_tick < p->fresh_tick)
        {
            return;
        }

        /* Watch task timer out */
        if ((p->task_status == SCI_TRUE) 
         && (curr_tick - p->fresh_tick > p->time_out))
        {
            SWDG_LOG_TRACE ( (
                                 "[SWDG:SWDG_CheckAll] Time out, %s, fresh_tick:%d \r\n",
                                 p->task_name, p->fresh_tick));

            SCI_PASSERT (SCI_FALSE, ("Task %s timeout", p->task_name));/*assert verified*/
        }

        list = list->next;
    }
}

/******************************************************************************
 * _LookupSwdgObject
 ******************************************************************************/
LOCAL SWDG_OBJECT *_LookupSwdgObject (const char *name)
{
    LIST_HEAD *list;

    SWDG_ASSERT (SCI_NULL != name);/*assert verified*/

    list = g_SwdgList.next;

    while (&g_SwdgList != list)
    {
        SWDG_OBJECT *p;

        p = LIST_ENTRY (list, SWDG_OBJECT, next);           /*lint !e413*/

        if (SCI_NULL == p)		/*lint !e774*/
        {
            return NULL;    /*lint !e774*/
        }

        if (0 == strcmp (p->task_name, name))/*lint !e746 -e746*/
        {
            return p;
        }

        list = list->next;
    }

    return SCI_NULL;
}


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

