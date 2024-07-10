/******************************************************************************
 ** File Name:        Tpc_srv.c                                               *
 ** Author:           Yi.Qiu                                                  *
 ** DATE:             09/16/2009                                              *
 ** Copyright:        2009 Spreatrum, Incoporated. All Rights Reserved.       *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit      History                                       *
 **---------------------------------------------------------------------------*
 ** DATE              NAME      DESCRIPTION                                   *
 ** 06/10/2008        Tao.Feng  Create                                        *
 ** 09/16/2009        Yi.Qiu    Reconstruct TPC driver                        *
 ******************************************************************************/
#include "ms_ref_base_trc.h"
#include "tb_hal.h"
#include "sc_reg.h"

#include "tb_comm.h"
#include "tp_srv.h"
#include "tp_api.h"
#include "tpc_drv.h"
#include "tpc_drvapi.h"
#include "nvitem.h"
#include "nv_item_id.h"
#include "dal_lcd.h"
#include "dal_keypad.h"
#include "tp_cfg.c"
#include "diag.h"
#include "cmddef.h"
#ifdef TP_VK_SUPPORT
    #include "dal_keypad.h"
#endif
#include "tp_gesture_drv.h"
#ifdef   __cplusplus
extern   "C"
{
#endif

/******************************************************************************
 * Macro definitions
 ******************************************************************************/

#define _TPAPI_DEBUG

#ifdef _TPAPI_DEBUG
#include "sci_log.h"

#define TPAPI_ASSERT(_x_)                SCI_ASSERT(_x_)
#define TPAPI_VERIFY(_x_)                SCI_ASSERT(_x_)
#define TPAPI_ERR_TRACE                  SCI_TRACE_LOW
#define TPAPI_LOG_TRACE                  SCI_TRACE_LOW
#define TPAPI_INF_TRACE                  SCI_TRACE_LOW 
#else
#define TPAPI_ASSERT(_x_)                ((void)0)
#define TPAPI_VERIFY(_x_)                ((void)(_x_))
#define TPAPI_ERR_TRACE(_x_)            ((void)0)
#define TPAPI_LOG_TRACE(_x_)            ((void)0)
#define TPAPI_INF_TRACE(_x_)            ((void)0)
#endif /* _TPAPI_DEBUG */

#ifdef TOUCH_PANEL_HWICON_SUPPORT_NONE
#define TP_HWICON_Y_LENGHT           0
#else
#define TP_HWICON_Y_LENGHT           25
#endif

#define TPSRV_STATE_NONE          0 
#define TPSRV_STATE_DOWN          1
#define TPSRV_STATE_FIRST_DONE    2
#define TPSRV_STATE_DONES         3
#define TPSRV_STATE_UP            4

LOCAL uint32 s_tpsrv_state = TPSRV_STATE_NONE;

/******************************************************************************
 * Structure definitions
 ******************************************************************************/
typedef struct
{
    TP_INFO_T info;                  /* Screen information */
    TP_CALIBRATION_T calb;     /* Calibration information */
    BOOLEAN is_hw_calc;         /* Is hardware calculate cordinate */
    BOOLEAN is_raw_mode;     /* Is raw data mode */
} TP_PARAM_T;

typedef struct
{
#if defined CHIP_ENDIAN_LITTLE && !defined _BIG_ENDIAN
    int16 x;  /*x coordinate*/
    int16 y;  /*y coordinate*/
#else
    int16 y;  /*x coordinate*/
    int16 x;  /*y coordinate*/
#endif
} TP_COORD_T;

typedef struct
{
    TP_COORD_T     cur;                        /* current point coord */
    TP_COORD_T     pre;                        /* previous point coord */
    uint32         is_dirty;
   
    TP_COORD_T     trace_buf[TP_FILTER_TRACE_COUNT];  
    uint32         trace_cnt;
    uint32         trace_x_min;
    uint32         trace_x_max;
    uint32         trace_y_min;
    uint32         trace_y_max;
} TP_FILTER_T;

typedef struct
{
    TP_MSG_E    TP_type;
    uint16      TP_x;
    uint16      TP_y;
} TP_COMMON_T, *TP_COMMON_T_PTR;

typedef enum
{
    TP_MOVE_EXCEED_LIMIT,
    TP_MOVE_SMOOTH,
    TP_MOVE_PRE
} TP_FILTER_RETURN_E;

/******************************************************************************
 * Static functions prototypes
 ******************************************************************************/
LOCAL void _Init (void);
LOCAL void _Callback (TB_MSG *msg_ptr);
LOCAL BOOLEAN _IsCoordValid (TP_COORD_T coord);
LOCAL BOOLEAN _IsFlyingSpot (TP_FILTER_T* filter_ptr, TP_COORD_T cur_coord);

LOCAL void _AddTraceFilter (TP_FILTER_T* filter_ptr,TP_COORD_T cur_coord);
LOCAL void _FlushTraceFilter (TP_FILTER_T* filter_ptr);
LOCAL void _ResetPoints (TP_FILTER_T* filter_ptr, TP_COORD_T cur_coord);
LOCAL void _UpdatePoints (TP_FILTER_T* filter_ptr, TP_COORD_T cur_coord);
LOCAL uint32 _IsMoving (TP_FILTER_T* filter_ptr);
LOCAL void _GetLatestPoint (TP_FILTER_T* filter_ptr,TP_COORD_T* cur_coord_ptr);
LOCAL void _SmoothingWindow(TP_FILTER_T* filter_ptr, uint32 window_size, TP_COORD_T* coord_result_ptr);
LOCAL void _SmoothingFilter(uint32 data_buf[], uint32 new_data);

LOCAL uint32 _TP_Hit (uint8 **dest_ptr, uint16 *dest_len_ptr, const uint8 *src_ptr, uint16 src_len);

LOCAL TP_PARAM_T  tp_param  = {0};
LOCAL TP_FILTER_T tp_filter = {0};
LOCAL TP_FILTER_T tp_filter2= {0};  //DoublePoint for another position

LOCAL TPDSVR_SIG_T s_tp_sig;
LOCAL SCI_SEMAPHORE_PTR s_tp_sem = PNULL;

LOCAL uint32 is_first_doublepoint=SCI_FALSE;
LOCAL uint32 doublepoint_sample_cnt=0;


#define K_STEPS    TP_FILTER_TRACE_COUNT
uint32 k_filter_buf[K_STEPS]={0};
uint32 k_steps_cnt=0;

uint32 smooth_win_cnt=0;
uint32 win_data_min=0;
uint32 win_data_max=0;

/******************************************************************************
 * THREAD_ENTRY
 ******************************************************************************/

#ifdef TP_VK_SUPPORT

typedef enum
{
    TP_ZONE_POINT= 1,
    TP_ZONE_VK
}TP_ZONE_ENUM;

/*****************************************************************************/
//  Description:    _TP_PointIsInVKRect
//  Author:         
//  Note:       
/*****************************************************************************/

LOCAL BOOLEAN _TP_PointIsInVKRect(int32 x, int32 y, VIR_KEY_RECT_T rect)
{
    if (x >= rect.left && x <= rect.right && 
        y >= rect.top  && y <= rect.bottom)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*****************************************************************************/
//  Description:   _TP_CheckVirtualKey
//  Author:         
//  Note:       
/*****************************************************************************/

LOCAL uint32 _TP_CheckVirtualKey(int32 x, int32 y)
{
    uint32          virkey_size     = 0;
    VIR_KEY_T       *vk_keymap_ptr  = SCI_NULL;
    uint32          keycode         = SCI_VK_INVALID_KEY;
    uint32          i = 0;
    
    vk_keymap_ptr = (VIR_KEY_T *)KPD_GetVirKeyMap(&virkey_size);
    
    if(virkey_size)
    {
        for(i=0; i<virkey_size; i++)
        {
            if(_TP_PointIsInVKRect(x, y, vk_keymap_ptr[i].rect))
            {
                TPAPI_LOG_TRACE ("[TPSRV]_TP_CheckVirtualKey: (%d < x < %d) (%d < y <%d)",
                vk_keymap_ptr[i].rect.left, vk_keymap_ptr[i].rect.right, 
                vk_keymap_ptr[i].rect.top,  vk_keymap_ptr[i].rect.bottom);    
                keycode = vk_keymap_ptr[i].vir_key_code;
                break;
            }
        }
    }
    else
    {
        keycode = SCI_VK_INVALID_KEY;
    }
    TPAPI_LOG_TRACE ("[TPSRV]_TP_CheckVirtualKey: i = %d, keycode = 0x%X", i, keycode);    
    return keycode;
}
/*****************************************************************************/
//  Description:   _TP_VirtualKeyHandle
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint32 _TP_VirtualKeyHandle(int32 x, int32 y, uint32 tp_state)
{

    static uint32 s_keyevent =0, s_keycode = 0;
    static TP_ZONE_ENUM   s_tp_zone = TP_ZONE_POINT;

    //-----------------
    // it is a normal TP point: TP_UP without a valid TP_DOWN in VK_ZONE
    if( (TP_ZONE_POINT == s_tp_zone) && (TP_UP == tp_state) )
    {
         return SCI_VK_INVALID_KEY; // just break, remain code will handle it
    }
    // only check in tp_down, when tp_up just report the same keycode in tp_down
    if(TP_DOWN == tp_state)
    {
        s_keycode = _TP_CheckVirtualKey(x, y);
    }

    //---------------
    // it is a normal TP point: TP_DOWN in TP_ZONE, not in VK_ZONE 
    if(SCI_VK_INVALID_KEY == s_keycode)
    {
        return SCI_VK_INVALID_KEY;
    }
    TPAPI_LOG_TRACE ("[TPSRV]TP_VirtualKeyHandle: TP= 0x%X (%d, %d) -> key = 0x%X", 
        tp_state, x, y, s_keycode);

    //---------------
    // event convert
    if(TP_DOWN == tp_state)
    {
            s_keyevent = KPD_DOWN;
            s_tp_zone = TP_ZONE_VK;     // from dwon to up it is VK
    }
    else if (TP_UP == tp_state)
    {
            s_keyevent = KPD_UP;
            s_tp_zone = TP_ZONE_POINT;  // restore default: POINT
    }
    else
    {
        return SCI_VK_INVALID_KEY; // invalid TP for VK cause VK has no MOVING... etc. right now
    }

    SCI_SendEventToClient(KEYPAD_SERVICE, s_keyevent, (void *)s_keycode);

    return s_keycode;
 
}
#endif

/*****************************************************************************/
//  Description:    _TP_AdjustCoordinate
//  Author:         
//  Note:       
/*****************************************************************************/
LOCAL uint32 _TP_AdjustCoordinate(int32* x_ptr, int32* y_ptr)
{
    int32 x_key=*x_ptr, y_key=*y_ptr;

    x_key = x_key;
    y_key = y_key - TP_HWICON_Y_LENGHT;

    TPAPI_LOG_TRACE ("[TPSRV]AdjustCoordinate: from (%d, %d) to (%d, %d)",
        *x_ptr, *y_ptr, x_key, y_key);    

    *x_ptr = x_key;
    *y_ptr = y_key;

    return (SCI_TRUE);
}

/*****************************************************************************/
//  Description:    THREAD_ENTRY (P_TP)
//  Author:         
//  Note:       
/*****************************************************************************/
PUBLIC uint32       g_points_num=0;  

#ifdef WIN32
THREAD_ENTRY (P_TP)/*lint -esym(765,thread_entry_P_TP)*/
{
    while (SCI_TRUE)
    {
        tp_sig_ptr = (TPDSVR_SIG_T *) SCI_GetSignal (P_TPP);
        SCI_SendEventToClient (TP_SERVICE,
                               tp_sig_ptr->SignalCode, (void *) tp_sig_ptr);
        SCI_FREE (tp_sig_ptr);
    }
}
#else

THREAD_ENTRY (P_TP)/*lint -esym(765,thread_entry_P_TP)*/
{
    int32 x_data=0;
    int32 y_data=0;    
    TP_COORD_T   tmp_cur_coord={0};
    TP_COORD_T   tp_cur_coord={0}; 
    TP_COORD_T   tp_first_coord={0}; 
    TP_COORD_T   tp_second_coord={0};    
    
    TPDSVR_SIG_T *sig = SCI_NULL;
    BOOLEAN         is_moving = SCI_FALSE;
    
    /* Init TP Module */
    _Init();
    TP_Gesture_Init();

    /* Create TP_SERVICE */
    SCI_CreateClientList (
        TP_SERVICE, (TP_MSG_MAX_NUM & 0x0ff), SCI_NULL);
    DIAG_RegisterCmdRoutine (DIAG_TP_F,  _TP_Hit);

    // Create the semaphore for whether the tp has been intialized completely. 
    if (PNULL == s_tp_sem)
    {
        s_tp_sem = SCI_CreateSemaphore ("TP_READY SEMAPHORE", 0);
    }
    /* Wait for tp semaphore is ready */
    SCI_GetSemaphore (s_tp_sem, SCI_WAIT_FOREVER);

    TPDRV_Enable();

    for(;;)
    {
        /* Receive signal for TP handler */
        sig = (TPDSVR_SIG_T *) SCI_GetSignal (P_TPP);
        SCI_ASSERT (sig);/*assert verified*/


        tp_cur_coord.x = sig->x_key;
        tp_cur_coord.y = sig->y_key;      

        g_points_num     = sig->num_of_point;

    if (tp_param.is_raw_mode == SCI_FALSE) 
    {
        x_data = (int32)(sig->x_key);
        y_data = (int32)(sig->y_key);
        _TP_AdjustCoordinate(&x_data, &y_data); 
        tp_first_coord.x = sig->x_key = x_data; 
        tp_first_coord.y = sig->y_key = y_data;

        x_data = (int32)(sig->xy_position[0].x_position);
        y_data = (int32)(sig->xy_position[0].y_position);
        _TP_AdjustCoordinate(&x_data, &y_data);  
        tp_second_coord.x = sig->xy_position[0].x_position =x_data; 
        tp_second_coord.y = sig->xy_position[0].y_position =y_data; 
        
        #ifdef TP_VK_SUPPORT
        {
        int32  tp_x = 0, tp_y = 0;
        uint32 tp_state = 0;         
        //------
        tp_x        = (int32)(sig->x_key);
        tp_y        = (int32)(sig->y_key);
        tp_state    = (uint32)(sig->SignalCode);
        TPAPI_LOG_TRACE("[TPSRV] TP_VK_SUPPORT state = 0x%x , (%d, %d)",tp_state, tp_x, tp_y);
        // KEY only support UP/DOWN
        if(TP_UP == tp_state || TP_DOWN == tp_state)
        {
            if(SCI_VK_INVALID_KEY != _TP_VirtualKeyHandle(tp_x, tp_y, tp_state))
            {   // TODO, willjiang: right now let TP still hanle all sig 
                SCI_FREE(sig);
                continue; // it is A VK, not TP zone, so go to wait next TP sigal from TPDRV
            }
        }
        }
        #endif  
        

        if((sig->SignalCode) == TP_DOWN)
        {
            _FlushTraceFilter(&tp_filter);
            _ResetPoints(&tp_filter, tp_first_coord);  

            _FlushTraceFilter(&tp_filter2);                         
            _ResetPoints(&tp_filter2, tp_second_coord);  

            is_first_doublepoint=SCI_FALSE;  
            doublepoint_sample_cnt=0;
            s_tpsrv_state = TPSRV_STATE_NONE;

            //Avoid missing TP_DOWN.
            if(g_points_num == 2)
            {
                tp_filter.is_dirty =SCI_FALSE;  
                _UpdatePoints(&tp_filter, tp_first_coord);              
                _AddTraceFilter(&tp_filter, tp_first_coord );
                goto PROCESS;		                
            }
        }
		else if((sig->SignalCode) == TP_UP)
		{
		    //Avoid missing TP_UP.
            tp_filter.is_dirty =SCI_TRUE;
            goto PROCESS;		
		}
		else
		{}

		//SignalCode==TP_DONE && Double points case
        if(g_points_num == 2)
        {
            doublepoint_sample_cnt++;
            if(doublepoint_sample_cnt<= DPOINT_DITHER_LEN)
            {
            //Filter dithering point
            TPAPI_LOG_TRACE ("[TPSRV] double point dithering=%d", doublepoint_sample_cnt);                        
            tp_filter.is_dirty =SCI_TRUE;        
            goto PROCESS;
            }
            else if(doublepoint_sample_cnt == (DPOINT_DITHER_LEN+1))
            {
                _FlushTraceFilter(&tp_filter);
                _FlushTraceFilter(&tp_filter2);                        
                _ResetPoints(&tp_filter, tp_first_coord);
                _ResetPoints(&tp_filter2, tp_second_coord);            
            }
        }         
        
        //Filter points out of rectange and flying spot.
	    if( (!(_IsCoordValid(tp_first_coord))) || 
              (_IsFlyingSpot(&tp_filter, tp_first_coord)) )
        {
            SCI_TRACE_LOW("TPSRV: Invalid coordinate at (%d, %d)", \
                                           tp_first_coord.x,tp_first_coord.y);
            SCI_TRACE_LOW("TPSRV: flyingspot----: pre_(%d,%d). cur_(%d,%d)\n\r",\
                                           tp_filter.cur.x,tp_filter.cur.y,\
                                           tp_first_coord.x,tp_first_coord.y);             
            tp_filter.is_dirty =SCI_TRUE;
            goto PROCESS;
        }

		{
            tp_filter.is_dirty =SCI_FALSE;  
            //Update tp_filter.cur and tp_filter.pre
            _UpdatePoints(&tp_filter, tp_first_coord);              
            _AddTraceFilter(&tp_filter, tp_first_coord ); 
        }
        //Thirdly:Todo smoothing filter


        if(g_points_num == 2)
        {
        SCI_TRACE_LOW ("TPSRV: ----Start to process the Second points.=");        
        //Filter points out of rectange and flying spot.
	    if((!(_IsCoordValid(tp_second_coord))) ||
             (_IsFlyingSpot(&tp_filter2, tp_second_coord)) )
        {
            SCI_TRACE_LOW ("TPSRV: Invalid coordinate at secondP(%d, %d)",\
                                           tp_first_coord.x,tp_first_coord.y);
            SCI_TRACE_LOW("TPSRV: NOT flyingspot--secondP: pre_(%d,%d). cur_(%d,%d)\n\r",\
                                           tp_filter2.cur.x,tp_filter2.cur.y,\
                                           tp_second_coord.x,tp_second_coord.y);              
            tp_filter2.is_dirty =SCI_TRUE;               
            goto PROCESS;
        }
        {
            tp_filter2.is_dirty =SCI_FALSE; 
            //Update tp_filter.cur and tp_filter.pre
            _UpdatePoints(&tp_filter2, tp_second_coord);			 
            _AddTraceFilter(&tp_filter2, tp_second_coord);
        }
        //Thirdly:Todo smoothing filter
       }
    }                    

PROCESS:
        TPAPI_LOG_TRACE("[TPSRV:SignalCode 0x%x, F(%d,%d) S(%d,%d) points=%d is_dirty=%d",\
                                               sig->SignalCode,\
                                               tp_first_coord.x,tp_first_coord.y,\
                                               tp_second_coord.x,tp_second_coord.y,\
                                               g_points_num,tp_filter.is_dirty);    
        TPAPI_LOG_TRACE("[TPSRV:pre(x,y)=%d,%d cur=(%d,%d)",tp_filter.pre.x,tp_filter.pre.y,\
                                                            tp_filter.cur.x,tp_filter.cur.y); 
		TPAPI_LOG_TRACE("[TPSRV:pre(x,y)=%d,%d cur=(%d,%d)",tp_filter2.pre.x, tp_filter2.pre.y,\
															tp_filter2.cur.x, tp_filter2.pre.y);
        if( (tp_filter.is_dirty) && 
            (sig->SignalCode != TP_UP) &&
            (tp_param.is_raw_mode == SCI_FALSE) )
        {
            TPAPI_LOG_TRACE("[TPSRV:]:---- Filter a dirty point ----");
            goto OUT;
        }
        
        switch (sig->SignalCode)
        {
            case TP_DOWN:
                if(s_tpsrv_state == TPSRV_STATE_NONE)
                {
                    s_tpsrv_state = TPSRV_STATE_FIRST_DONE;
                    TPAPI_LOG_TRACE("[TPSRV:] :TP_DOWN change to s_tpsrv_state=0x%x", s_tpsrv_state); 
                }
                else
                {
                    //Note:VKey only process TP_DOWN&TP_UP,
                    //so,in some conditions it will meet a TP_MOVE before TP_DOWN.
                    s_tpsrv_state = TPSRV_STATE_NONE;
                    TPAPI_LOG_TRACE("[TPSRV:] Fetal Error:TP_DOWN but s_tpsrv_state=0x%x", s_tpsrv_state);
                }
                
                if(tp_param.is_raw_mode == SCI_TRUE)
                {
                    tmp_cur_coord.x = sig->x_key;
                    tmp_cur_coord.y = sig->y_key;
                    _ResetPoints(&tp_filter, tmp_cur_coord);
                }
                else
                {
                    _GetLatestPoint(&tp_filter, &tmp_cur_coord);
                    sig->x_key = tmp_cur_coord.x;
                    sig->y_key = tmp_cur_coord.y;
                }

                TP_SetGesture_Reset(sig);                
                /* Send message to TP client, such as MMI */
                SCI_SendEventToClient (TP_SERVICE,
                                       sig->SignalCode, (void *) sig);
                break;
                
            case TP_MOVE:
                if (tp_param.is_raw_mode == SCI_TRUE)
                {
                    tp_filter.pre.x = sig->x_key;
                    tp_filter.pre.y = sig->y_key;
                }
                else
                {                  
                    if((is_first_doublepoint==SCI_FALSE) && (g_points_num==2))
                    {
                        TPAPI_LOG_TRACE("[TPSRV:] changing 1->2. ");
                        //Reset sample_cnt for Dpoint
                        doublepoint_sample_cnt=0;
                        is_first_doublepoint=SCI_TRUE;

                        _FlushTraceFilter(&tp_filter);
                        _FlushTraceFilter(&tp_filter2);                        
                        _ResetPoints(&tp_filter, tp_first_coord);
                        _ResetPoints(&tp_filter2, tp_second_coord);
                    }
                    else if((is_first_doublepoint==SCI_TRUE) && (g_points_num==1))
                    {
                        TPAPI_LOG_TRACE("[TPSRV:] changing 2->1. ");                    
                        
                        _FlushTraceFilter(&tp_filter);
                        _ResetPoints(&tp_filter, tp_first_coord);
                        is_first_doublepoint=SCI_FALSE;  
                        doublepoint_sample_cnt=0;
                    }
                    else
                    {
                    }
                    
                    is_moving = _IsMoving(&tp_filter);

                    if (is_moving)
                    {                  
                        SCI_TRACE_LOW("[TPSRV]_TP_PushMsg:---->is moving\n\r");
                    }
                    else
                    {
                        SCI_TRACE_LOW ( "[TPSRV]TP_Not_Moving---->is not moving\n\r");
                        sig->SignalCode = TP_NOTMOVE;
                    }
                }
                
                if((s_tpsrv_state == TPSRV_STATE_FIRST_DONE) || (s_tpsrv_state == TPSRV_STATE_DONES))
                {
                    s_tpsrv_state = TPSRV_STATE_DONES;
                    TPAPI_LOG_TRACE("[TPSRV:] :TP_DONE change to s_tpsrv_state=0x%x", s_tpsrv_state);                     
                    /* Send message to TP client, such as MMI */
                    if (tp_param.is_raw_mode == SCI_FALSE)
                    {
                    _GetLatestPoint(&tp_filter, &tmp_cur_coord);
                    _SmoothingWindow(&tp_filter,TP_SMOOTH_WIN_SIZE,&tmp_cur_coord);                    
                    sig->x_key = tmp_cur_coord.x;
                    sig->y_key = tmp_cur_coord.y; 
                    }

                    if(g_points_num == 2)
                    {
                        _GetLatestPoint(&tp_filter2, &tmp_cur_coord);
                        _SmoothingWindow(&tp_filter2,TP_SMOOTH_WIN_SIZE,&tmp_cur_coord);
                        sig->xy_position[0].x_position= tmp_cur_coord.x;
                        sig->xy_position[0].y_position= tmp_cur_coord.y; 
                    }

 
                    TP_Gesture_Read(sig);
                    SCI_SendEventToClient (TP_SERVICE,
                                           sig->SignalCode, (void *) sig);                     
                }
                else
                {
                    TPAPI_LOG_TRACE("[TPSRV:] Fetal Error:TP_MOVE but s_tpsrv_state=0x%x", s_tpsrv_state);               
                    s_tpsrv_state = TPSRV_STATE_NONE;
					break;
                }
                break;
            case TP_UP:
                _FlushTraceFilter(&tp_filter);
                _FlushTraceFilter(&tp_filter2); 
                
                sig->x_key = tp_filter.pre.x;
                sig->y_key = tp_filter.pre.y;

                if((s_tpsrv_state == TPSRV_STATE_FIRST_DONE) || (s_tpsrv_state == TPSRV_STATE_DONES))
                {              
                    s_tpsrv_state = TPSRV_STATE_NONE;
                    TPAPI_LOG_TRACE("[TPSRV:] :TP_UP change to s_tpsrv_state=0x%x", s_tpsrv_state);   
                }
                else
                {
                    TPAPI_LOG_TRACE("[TPSRV:] Error:TP_UP but without validate TP_DOWN. s_tpsrv_state=0x%x", s_tpsrv_state);                
                    s_tpsrv_state = TPSRV_STATE_NONE;
                    break;
                }

                if (tp_param.is_raw_mode == SCI_FALSE) 
                {
                _GetLatestPoint(&tp_filter, &tmp_cur_coord);
                _SmoothingWindow(&tp_filter,TP_SMOOTH_WIN_SIZE,&tmp_cur_coord);                 
                sig->x_key = tmp_cur_coord.x;
                sig->y_key = tmp_cur_coord.y; 
                }

                TP_Gesture_Read(sig);                
                /* Send message to TP client, such as MMI */
                SCI_SendEventToClient (TP_SERVICE,
                                       sig->SignalCode, (void *) sig);
                break;
            default :
//                SCI_ASSERT (SCI_FALSE);  /*assert verified*/ 
                break;
        }

        OUT:
            /* Free memory */
            SCI_FREE (sig);
    }
}
#endif  //WIN32

/******************************************************************************
 * TP_Enable
 ******************************************************************************/
PUBLIC void TP_Enable (void)
{
    //Enable TPC
    TPDRV_Enable();

    TPAPI_LOG_TRACE ( ("[TPSRV:TP_Enable]"));
}

/******************************************************************************
 * TP_Disable
 ******************************************************************************/
PUBLIC void TP_Disable (void)
{
    //Disable TPC
    TPDRV_Disable();

    TPAPI_LOG_TRACE ( ("[TPSRV:TP_Disable]"));
}

/******************************************************************************
 * TP_GetInfo
 ******************************************************************************/
PUBLIC TP_INFO_T *TP_GetInfo (void)
{
    TPAPI_LOG_TRACE ( ("[TPSRV:TP_GetInfo]"));

    //Get TP information
    return &tp_param.info;
}

/******************************************************************************
 * TP_SetDataMode
 ******************************************************************************/
PUBLIC void TP_SetRawDataMode (BOOLEAN is_raw_mode)
{
    if (is_raw_mode)
    {
        //Disable coordinate convertion
        TPDRV_EnterCalibration();//lint !e746
    }
    else
    {
        //Enable coordinate convertion
        TPDRV_EnterMulti();//lint !e746
    }

    tp_param.is_raw_mode = is_raw_mode;

    TPAPI_LOG_TRACE ( ("[TPSRV:TP_SetDataMode]"));
}

/******************************************************************************
 * TP_UpdateCalibration
 ******************************************************************************/
PUBLIC BOOLEAN TP_UpdateCalibration (TP_CALIBRATION_T *param)
{
    int x_slop = 0, x_offset = 0;
    int y_slop = 0, y_offset = 0;
	BOOLEAN xy_exchange = SCI_FALSE;
	
    if(PNULL != s_tp_sem)
        SCI_PutSemaphore (s_tp_sem);  

    //Check calibration validation
    if (param->y_left_factor < 1000 || param->x_top_factor < 1000)
    {
        TPAPI_ERR_TRACE ( (
                              "[TPSRV:TP_UpdateCalibration] Factor error "));
        return SCI_FALSE;
    }

    //Set calibration parameter
    SCI_MEMCPY (&tp_param.calb, param,
                sizeof (TP_CALIBRATION_T));
    //Vaild calibration parameter*/
    tp_param.calb.is_valid = CALIBRATION_FLAG;

    //If coordinate calcuation is enable
    if (tp_param.is_hw_calc == SCI_TRUE)
    {
        //Enable coordinate calcuation
        TP_SetRawDataMode(SCI_FALSE);

        x_slop  = (tp_param.calb.x_slope*(1<<8))/1000;
        x_offset= (tp_param.calb.x_offset)/1000;
        y_slop  = (tp_param.calb.y_slope*(1<<8))/1000;
        y_offset= (tp_param.calb.y_offset)/1000;
        TPAPI_LOG_TRACE ( "[TPSRV:TP_UpdateCalibration] x_slop=%d, x_offset=%d,y_slop=%d,y_offset=%d",\
                                                           x_slop,x_offset,\
    		                                               y_slop,y_offset);	    

        //Config parameter, xy_exchange is a param neglected.
        TPDRV_ConfigCoord(x_slop, x_offset, y_slop, y_offset, xy_exchange);

#ifdef TPC_TYPE_DOUBLE
        {
            NV_TP_PARAM_T s_nv_tp_param={0};
            CALIBRATED_DATA_T* data_ptr = TPDRV_GetCalibratedData();
            s_nv_tp_param.Rx0 = (uint32)(data_ptr->Rx0);
            s_nv_tp_param.Ry0 = (uint32)(data_ptr->Ry0);
            s_nv_tp_param.Rt0 = (uint32)(data_ptr->Rt0);
            EFS_NvitemWrite(NV_TP_PARAM, sizeof(s_nv_tp_param), (uint8 *)(&s_nv_tp_param), SCI_TRUE);
        }
#endif
    }

    return SCI_TRUE;
}

/******************************************************************************
 * _Callback
 ******************************************************************************/
LOCAL void _Callback (TB_MSG *msg_ptr)
{
    TPDSVR_SIG_T *sig = (TPDSVR_SIG_T *)
                        SCI_ALLOC (sizeof (TPDSVR_SIG_T));
    SCI_MEMSET(sig, 0, sizeof(TPDSVR_SIG_T));
    
    sig->y_key = (msg_ptr->wparam) >> 16 ;
    sig->x_key = msg_ptr->wparam;

    /* Dispatch message to TP server */
    switch (msg_ptr->message)
    {
        case TB_TP_DOWN:
            sig->SignalCode = TP_DOWN;
            break;
        case TB_TP_MOVE:
            sig->SignalCode = TP_MOVE;
            break;
        case TB_TP_UP :
            sig->SignalCode = TP_UP;
            break;
        default :
            sig->SignalCode = TP_MAX_MSG;
//            SCI_ASSERT (SCI_FALSE);   /*assert verified*/
            break;
    }

    SCI_SendSignal ( (xSignalHeader) sig, P_TPP);
}

/******************************************************************************
 * _Callback for MultiTouch
 ******************************************************************************/
LOCAL void _MtouchCallback (TB_MSG *msg_ptr)
{
    uint32 points_number=0;
    uint32* points_ptr=SCI_NULL;	
    TPDSVR_SIG_T *sig = (TPDSVR_SIG_T *)
                        SCI_ALLOC (sizeof (TPDSVR_SIG_T));
    SCI_MEMSET(sig, 0, sizeof(TPDSVR_SIG_T));

    points_number = msg_ptr->wparam;
	points_ptr =(uint32*)(msg_ptr->lparam);
	sig->num_of_point =points_number;
    
	if(points_number == 1)
	{
    sig->y_key = (points_ptr[0]) >> 16 ;
    sig->x_key = points_ptr[0];
    }
	else if(points_number == 2)
	{
    sig->y_key = (points_ptr[0]) >> 16 ;
    sig->x_key = points_ptr[0];
	sig->xy_position[0].y_position = (points_ptr[1]) >> 16 ;
    sig->xy_position[0].x_position = points_ptr[1];
	}
	else
	{
	}

    /* Dispatch message to TP server */
    switch (msg_ptr->message)
    {
        case TB_TP_DOWN:
            sig->SignalCode = TP_DOWN;
            break;
        case TB_TP_MOVE:
            sig->SignalCode = TP_MOVE;
            break;
        case TB_TP_UP :
            sig->SignalCode = TP_UP;
            break;
        default :
            sig->SignalCode = TP_MAX_MSG;
//            SCI_ASSERT (SCI_FALSE);   /*assert verified*/
            break;
    }

    TPAPI_LOG_TRACE ( "[TPSRV:_callback] (%d,%d),(%d,%d),size=%d, Sigcode=0x%x",\
                                                        sig->x_key,sig->y_key,\
		                                                sig->xy_position[0].x_position,\
		                                                sig->xy_position[0].y_position,\
		                                                points_number,sig->SignalCode);	
    SCI_SendSignal ( (xSignalHeader) sig, P_TPP);
}

/******************************************************************************
 * _Init
 ******************************************************************************/
LOCAL void _Init (void)
{    
    LCD_INFO_T      lcd_info;
    //TP_CFG_INFO_PTR tp_cfginfo_ptr = TP_GetCfgInfo();
    	
    //Init TPC
    TPDRV_Init();

    //Register TPC ISR callback handler
    HAL_RegCallback(TB_TPC_INT, _MtouchCallback);

    //Set TP information
    LCD_GetInfo (MAIN_LCD_ID, &lcd_info);

    SCI_ASSERT (lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/

    //Set TP information
#ifndef TP_EXCHANGE_XY
    tp_param.info.tp_width = lcd_info.lcd_width;
    tp_param.info.tp_height = lcd_info.lcd_height + TP_HWICON_Y_LENGHT;
    tp_param.info.tp_lcd_height = lcd_info.lcd_height;
#else
    tp_param.info.tp_width = lcd_info.lcd_height;
    tp_param.info.tp_height = lcd_info.lcd_width + TP_HWICON_Y_LENGHT;
    tp_param.info.tp_lcd_height = lcd_info.lcd_width;	
#endif
    //Whether x,y reversed
/*
	tp_param.info.exchange_x  = tp_cfginfo_ptr->exchange_x;
    tp_param.info.exchange_y  = tp_cfginfo_ptr->exchange_y;
    tp_param.info.exchange_xy = tp_cfginfo_ptr->exchange_xy;
*/
	tp_param.info.exchange_x  = 0;
    tp_param.info.exchange_y  = 0;
    tp_param.info.exchange_xy = 0;
    
    tp_param.is_hw_calc = SCI_TRUE;

    //Not raw data mode
    tp_param.is_raw_mode = SCI_FALSE;

    if (SCI_TRUE == tp_param.is_hw_calc)
    {
        TP_SetRawDataMode(SCI_TRUE);
    }

#ifdef TPC_TYPE_DOUBLE
{
    CALIBRATED_DATA_T data={0};  
    NVITEM_ERROR_E  nv_err = NVERR_NONE;
    NV_TP_PARAM_T s_nv_tp_param={0};
    //Read NV if tpc has been calibrated.
    nv_err = EFS_NvitemRead(NV_TP_PARAM, sizeof(s_nv_tp_param), (uint8 *)(&s_nv_tp_param));
    {
        data.Rx0 = s_nv_tp_param.Rx0;
        data.Ry0 = s_nv_tp_param.Ry0;
        data.Rt0 = s_nv_tp_param.Rt0;
        TPDRV_SetCalibratedData(data);
    } 
}
#endif
    TPAPI_LOG_TRACE ( ("[TPSRV:TP_Init]"));
}

LOCAL void _ResetPoints (TP_FILTER_T* filter_ptr, TP_COORD_T cur_coord)
{
    SCI_TRACE_LOW("TPSRV: _ResetPoints.\n\r"); 
    filter_ptr->pre.x = filter_ptr->cur.x = cur_coord.x;
    filter_ptr->pre.y = filter_ptr->cur.y = cur_coord.y;
}

LOCAL void _UpdatePoints (TP_FILTER_T* filter_ptr, TP_COORD_T cur_coord)
{
    SCI_TRACE_LOW ("TPSRV: _UpdatePoints (%d, %d), trace_cnt=%d", \
                                cur_coord.x,cur_coord.y,filter_ptr->trace_cnt);   
    if(filter_ptr->trace_cnt == 0)
    {
    _ResetPoints(filter_ptr, cur_coord);
    }
    else
    {
    filter_ptr->pre.x = filter_ptr->cur.x;
    filter_ptr->pre.y = filter_ptr->cur.y;

    filter_ptr->cur.x = cur_coord.x;
    filter_ptr->cur.y = cur_coord.y;
    }    
}

/*****************************************************************************/
//  Function name:  _AddTraceFilter
//  Description:    Add validate data to trace filter buffer.
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void _AddTraceFilter (TP_FILTER_T* filter_ptr,TP_COORD_T cur_coord)
{
    uint32 i=0;
    SCI_TRACE_LOW("TPSRV: _AddTraceFilter: (%d,%d).\n\r", cur_coord.x,cur_coord.y); 

    SCI_ASSERT(filter_ptr != SCI_NULL);   /*assert verified*/
    //Get the min/max in trace buffer        
    filter_ptr->trace_x_min = cur_coord.x;  
    filter_ptr->trace_x_max = cur_coord.x;
    filter_ptr->trace_y_min = cur_coord.y;  
    filter_ptr->trace_y_max = cur_coord.y;
    
    for(i=0; i<MIN(filter_ptr->trace_cnt, TP_FILTER_TRACE_COUNT); i++)
    {
        uint32 cnt = TP_FILTER_TRACE_COUNT-1-i;
        if(filter_ptr->trace_buf[cnt].x <= (int16)(filter_ptr->trace_x_min)) 
            filter_ptr->trace_x_min = filter_ptr->trace_buf[cnt].x;
        if(filter_ptr->trace_buf[cnt].x >= (int16)(filter_ptr->trace_x_max)) 
            filter_ptr->trace_x_max = filter_ptr->trace_buf[cnt].x;
        
        if(filter_ptr->trace_buf[cnt].y <= (int16)(filter_ptr->trace_y_min)) 
            filter_ptr->trace_y_min=filter_ptr->trace_buf[cnt].y;
        if(filter_ptr->trace_buf[cnt].y >= (int16)(filter_ptr->trace_y_max)) 
            filter_ptr->trace_y_max=filter_ptr->trace_buf[cnt].y;
    }
        
    //Iterator 
    for(i=0; i<(TP_FILTER_TRACE_COUNT-1); i++)
    {
        filter_ptr->trace_buf[i].x = filter_ptr->trace_buf[i+1].x;
        filter_ptr->trace_buf[i].y = filter_ptr->trace_buf[i+1].y;
    }

    filter_ptr->trace_buf[TP_FILTER_TRACE_COUNT-1].x = cur_coord.x;
    filter_ptr->trace_buf[TP_FILTER_TRACE_COUNT-1].y = cur_coord.y;
    filter_ptr->trace_cnt ++;
    
    SCI_TRACE_LOW("TPSRV: _AddTraceFilter: X_min=%d,X_max=%d.Y_min=%d,Y_max=%d, cnt=%d\n\r",\
                                                              filter_ptr->trace_x_min,\
                                                              filter_ptr->trace_x_max,\
                                                              filter_ptr->trace_y_min,\
                                                              filter_ptr->trace_y_max,\
                                                              filter_ptr->trace_cnt); 
}

LOCAL void _FlushTraceFilter (TP_FILTER_T* filter_ptr)
{
    filter_ptr->is_dirty  = 0;
    filter_ptr->trace_cnt = 0;

    filter_ptr->trace_x_min = filter_ptr->trace_x_max =0;        
    filter_ptr->trace_y_min = filter_ptr->trace_y_max =0;    
}

/*****************************************************************************/
//  Function name:  _GetLatestPoint
//  Description:    Get the latest validate data in trace filter buffer.
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void _GetLatestPoint (TP_FILTER_T* filter_ptr,TP_COORD_T* cur_coord_ptr)
{
    cur_coord_ptr->x = filter_ptr->trace_buf[TP_FILTER_TRACE_COUNT-1].x;
    cur_coord_ptr->y = filter_ptr->trace_buf[TP_FILTER_TRACE_COUNT-1].y;
    TPAPI_LOG_TRACE("[TPSRV]:_GetLatestPoint x,y %d,%d",cur_coord_ptr->x,cur_coord_ptr->y);    
}

/*****************************************************************************/
//  Function name:  _IsMoving
//  Description:    Moving depends on the min/max data in trace filter buffer.
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL uint32 _IsMoving (TP_FILTER_T* filter_ptr)
{
    uint32 delta_x = filter_ptr->trace_x_max-filter_ptr->trace_x_min;
    uint32 delta_y = filter_ptr->trace_y_max-filter_ptr->trace_y_min;

    SCI_TRACE_LOW("TPSRV: _IsMoving: delta_x= %d delta_y=%d.\n\r", delta_x,delta_y);   
    SCI_TRACE_LOW("TPSRV: _IsMoving: limit_x= %d limit_y=%d.\n\r", MOVING_X_LIMIT,MOVING_Y_LIMIT);   
    
    if((delta_x >= MOVING_X_LIMIT) || (delta_y >= MOVING_Y_LIMIT))
        return SCI_TRUE;
    else
        return SCI_FALSE;   
}

/*****************************************************************************/
//  Function name:  _IsFlyingSpot
//  Description:    Filter flying spot depends on two points 
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _IsFlyingSpot (TP_FILTER_T* filter_ptr, TP_COORD_T cur_coord)
{
    int delta_x;
    int delta_y;
    TP_COORD_T latest_coord;

    latest_coord.x = filter_ptr->cur.x;
	latest_coord.y = filter_ptr->cur.y;

    delta_x = cur_coord.x - latest_coord.x;
    delta_x = (delta_x > 0) ? delta_x : (- delta_x);
    delta_y = cur_coord.y - latest_coord.y;
    delta_y = (delta_y > 0) ? delta_y : (- delta_y);

    if ( (FLYINGSPOT_X_LIMIT <= delta_x) || (FLYINGSPOT_Y_LIMIT <= delta_y))
    {  
        SCI_TRACE_LOW("TPSRV: _IsFlyingSpot: pre_(%d,%d). cur_(%d,%d)\n\r",\
                                                   latest_coord.x,latest_coord.y,\
                                                   cur_coord.x,cur_coord.y); 
        return SCI_TRUE;
    }

    return SCI_FALSE;
}

/*****************************************************************************/
//  Function name:    _TP_IsCoordValid
//  Description:    This function used to verify whether coord is valid
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN _IsCoordValid (TP_COORD_T coord)
{
    if ( (coord.x <= tp_param.info.tp_width) && 
         (coord.y <= tp_param.info.tp_height)&&
         (coord.x > 0)                      &&
         (coord.y > 0)                      )
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

void _SmoothingFilter(uint32 data_buf[], uint32 new_data)
{
    if(k_steps_cnt == 0)
	{
		data_buf[k_steps_cnt] = new_data;
		SCI_TRACE_LOW("---data, %d, cnt=%d\r\n", data_buf[k_steps_cnt],k_steps_cnt);
		k_steps_cnt++;
	}
	else if(k_steps_cnt<K_STEPS)
	{
	    data_buf[k_steps_cnt] = (new_data+data_buf[k_steps_cnt-1])/2;
		SCI_TRACE_LOW("---data, %d, cnt=%d\r\n", data_buf[k_steps_cnt],k_steps_cnt);
		k_steps_cnt++;
	}
	else
	{
		uint32 i=0;
	    for(i=0; i<K_STEPS-1; i++)
            data_buf[i] = data_buf[i+1];
		data_buf[i] = (data_buf[i-1]+new_data)/2;
		SCI_TRACE_LOW("---data, %d, cnt=%d\r\n", data_buf[i],k_steps_cnt);
		k_steps_cnt++;
	}
}

/*
    //Attention: don't access out of range.
*/
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
void _SmoothingWindow(TP_FILTER_T* filter_ptr, uint32 window_size, TP_COORD_T* coord_result_ptr)
{

    TP_COORD_T* coord_ptr= (TP_COORD_T*)(filter_ptr->trace_buf);
    
    uint32 smooth_cnt= filter_ptr->trace_cnt;
    uint32 smooth_window_size = window_size;    
	int16 xdata_min = filter_ptr->trace_buf[TP_FILTER_TRACE_COUNT-1].x;
	int16 xdata_max = filter_ptr->trace_buf[TP_FILTER_TRACE_COUNT-1].x;
	int16 ydata_min = filter_ptr->trace_buf[TP_FILTER_TRACE_COUNT-1].y;
	int16 ydata_max = filter_ptr->trace_buf[TP_FILTER_TRACE_COUNT-1].y;
    
    uint32 i=0;
	int16 x_sum=0;
	int16 y_sum=0;

    SCI_PASSERT((window_size <= TP_FILTER_TRACE_COUNT), \
                ("_SmoothingWindow: window_size=%d,TP_FILTER_TRACE_COUNT=%d",\
                window_size,TP_FILTER_TRACE_COUNT) );;
    if((smooth_cnt == 0) || (window_size<3))
    {
    SCI_TRACE_LOW("TPSRV [_SmoothingWindow]: cnt=%d.",smooth_cnt);  
    return;
    }

    //Get the min/max in SMOOTH_WINDOW    
    for(i=0; i<MIN(filter_ptr->trace_cnt, smooth_window_size); i++)
    {
        uint32 cnt = TP_FILTER_TRACE_COUNT-1-i;
        if(filter_ptr->trace_buf[cnt].x <= (int16)xdata_min) 
            xdata_min = filter_ptr->trace_buf[cnt].x;
        if(filter_ptr->trace_buf[cnt].x >= (int16)xdata_max) 
            xdata_max = filter_ptr->trace_buf[cnt].x;
        
        if(filter_ptr->trace_buf[cnt].y <= (int16)ydata_min) 
            ydata_min = filter_ptr->trace_buf[cnt].y;
        if(filter_ptr->trace_buf[cnt].y >= (int16)ydata_max) 
            ydata_max = filter_ptr->trace_buf[cnt].y;
    }

    SCI_TRACE_LOW("TPSRV [_SmoothingWindow]: xdata_min=%d, xdata_max=%d ydata_min=%d,ydata_max=%d.",\
                                   xdata_min, xdata_max, ydata_min, ydata_max);  
    

    //Sum the data in SMOOTH_WINDOW start from the latest data.
	for(i=0; i<min(smooth_window_size, smooth_cnt); i++)
	{
		x_sum += coord_ptr[TP_FILTER_TRACE_COUNT-1-i].x;
        y_sum += coord_ptr[TP_FILTER_TRACE_COUNT-1-i].y;
	}    
	if(smooth_cnt >= 3)
	{
        coord_result_ptr->x = (x_sum-xdata_min-xdata_max) / (int16)(min(smooth_window_size, smooth_cnt)-2);
        coord_result_ptr->y = (y_sum-ydata_min-ydata_max) / (int16)(min(smooth_window_size, smooth_cnt)-2);
	}
	else
	{
        coord_result_ptr->x = coord_ptr[TP_FILTER_TRACE_COUNT-1].x;
        coord_result_ptr->y = coord_ptr[TP_FILTER_TRACE_COUNT-1].y;
	}
    SCI_TRACE_LOW("TPSRV [_SmoothingWindow]: x=%d, y=%d cnt=%d.",\
                                   coord_result_ptr->x, coord_result_ptr->y,smooth_cnt);               

}

/*****************************************************************************/
//  Function name:  TP_Hit
//  Description:
//  Global resource dependence:
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL uint32 _TP_Hit (
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message.
    uint16 src_len)         // Size of the source buffer in uint8.
{
    TP_COMMON_T     *comm_ptr;

    do
    {
        if(!dest_ptr || !src_ptr)
        {
            break;
        }
        
        *dest_len_ptr = 0;

        comm_ptr = (TP_COMMON_T *) (src_ptr + sizeof (MSG_HEAD_T));
        // Build response to channel server
        *dest_ptr = SCI_ALLOC (sizeof (MSG_HEAD_T));
        if(NULL == *dest_ptr)
        {
            break;
        }
        
        *dest_len_ptr = sizeof (MSG_HEAD_T);
        memcpy (*dest_ptr, src_ptr, sizeof (MSG_HEAD_T));
        ( (MSG_HEAD_T *) (*dest_ptr))->len = sizeof (MSG_HEAD_T);
        // decode COMM_TP_T to TPDSVR_SIG_T
        s_tp_sig.SignalCode = comm_ptr->TP_type;
        s_tp_sig.x_key = comm_ptr->TP_x;
        s_tp_sig.y_key = comm_ptr->TP_y;

        if(!((TP_UP == s_tp_sig.SignalCode) || (TP_DOWN==s_tp_sig.SignalCode) || (TP_MOVE==s_tp_sig.SignalCode)))
        {
            SCI_FREE(*dest_ptr);
            *dest_ptr = NULL;
            *dest_len_ptr = 0;
            //TM_SendTestPointRequest(0x22222222, 22222222);
            SCI_TRACE_LOW("TPSRV [_TP_Hit]: type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d.",s_tp_sig.SignalCode, s_tp_sig.x_key, s_tp_sig.y_key);               
            break;    
        }

        // send to TP_SERVICE's client
        SCI_TRACE_LOW ("SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d.",s_tp_sig.SignalCode, s_tp_sig.x_key, s_tp_sig.y_key);

        SCI_SendEventToClient (TP_SERVICE, s_tp_sig.SignalCode, (void *) &s_tp_sig);

        break;

    }while(1);/*lint !e506*/

    return SCI_TRUE;
}


/*****************************************************************************/
//  Function name:    TPSRV_GetParamHandler( msg_id )
//  Description:    Get/Set parameter for debug mode
//  Global resource dependence:
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint32 TPSRV_GetParamHandler (uint32 msg_id)
{
    uint32 data=0;
    switch(msg_id)
    {
        case MSG_TP_WIDTH:        
        case MSG_TP_HEIGHT:        
        case MSG_TP_REF_RES:       
        case MSG_TP_REF_UOL:      
        case MSG_TP_DPOINT_XLIMIT: 
        case MSG_TP_DPOINT_YLIMIT: 
        case MSG_TP_SPOINT_RT_MAX:
        case MSG_TP_SPOINT_RT_MIN:            
        case MSG_TP_DPOINT_RT_MAX:
        case MSG_TP_DPOINT_RT_MIN:            
        case MSG_TP_DPOINT_ENABLE:
            data = TP_GetParam(msg_id);
            SCI_TRACE_LOW("TPSRV:_GetParamHandler: msg=0x%d,value=%d\n\r",\
                                                                msg_id,data);              
            break;
        default:
            break;            
    }

    return (data);
}

PUBLIC uint32 TPSRV_SetParamHandler (uint32 msg_id, uint32 data)
{
    switch(msg_id)
    {
        case MSG_TP_WIDTH:        
        case MSG_TP_HEIGHT:        
        case MSG_TP_REF_RES:       
        case MSG_TP_REF_UOL:      
        case MSG_TP_DPOINT_XLIMIT: 
        case MSG_TP_DPOINT_YLIMIT: 
        case MSG_TP_SPOINT_RT_MAX:
        case MSG_TP_SPOINT_RT_MIN:            
        case MSG_TP_DPOINT_RT_MAX:
        case MSG_TP_DPOINT_RT_MIN:            
        case MSG_TP_DPOINT_ENABLE:
            TP_SetParam(msg_id, data);
            SCI_TRACE_LOW("TPSRV:_SetParamHandler: msg=0x%d,value=%d\n\r",\
                                                                msg_id,data);             
            break;
        default:
            break;
    }
    return SCI_TRUE;
}


#define TPSRV_POSITION_TEST

#ifdef TPSRV_POSITION_TEST
uint32 g_points_buffer[10] = {0};
PUBLIC uint32 TPSRV_GetPoints (uint32* buf_addr, uint32* size)
{
    //Get original data address value and size from TPDRV
    *buf_addr = (uint32)(g_points_buffer);

#ifdef TPC_TYPE_SINGLE
        g_points_buffer[0] = (tp_filter.cur.x);
        g_points_buffer[1] = (tp_filter.cur.y);  
        g_points_buffer[2] = 0;
        g_points_buffer[3] = 0;  
        *size =4;
        SCI_TRACE_LOW("TPSRV: TPSRV_GetPoints: first(%d,%d). second(0,0) g_points_num=1\n\r",\
                                           g_points_buffer[0],\
                                           g_points_buffer[1]); 
        return SCI_TRUE;
#endif

    if(g_points_num == 1)/*lint !e527*/
    {
        g_points_buffer[0] = (tp_filter.cur.x);
        g_points_buffer[1] = (tp_filter.cur.y);  
        g_points_buffer[2] = 0;
        g_points_buffer[3] = 0;  
        *size =4;
    }
    else if((g_points_num == 2) && (doublepoint_sample_cnt > DPOINT_DITHER_LEN))
    {
        g_points_buffer[0] = (tp_filter.cur.x);
        g_points_buffer[1] = (tp_filter.cur.y);  
        g_points_buffer[2] = (tp_filter2.cur.x);
        g_points_buffer[3] = (tp_filter2.cur.y); 

        g_points_buffer[4] = (uint32)(g_watch_data[WATCH_DELTA_X]); 
        g_points_buffer[5] = (uint32)(g_watch_data[WATCH_DELTA_Y]);
        g_points_buffer[6] = (uint32)(g_watch_data[WATCH_RT]);
        *size =7;
    }
    else
    {
        //To do later:
        *size =0;
        return SCI_TRUE;
    }


    SCI_TRACE_LOW("TPSRV: TPSRV_GetPoints: first(%d,%d). second(%d,%d) g_points_num=%d\n\r",\
                                           tp_filter.cur.x,tp_filter.cur.y,\
                                           tp_filter2.cur.x,tp_filter2.cur.y,\
                                           g_points_num); 
    return SCI_TRUE;
}
#else  //TPDRV_POSITION_TEST
PUBLIC uint32 TPDRV_GetPoints (uint32* buf_addr, uint32* size);
PUBLIC uint32 TPSRV_GetPoints (uint32* buf_addr, uint32* size)
{
    uint32* data_ptr = 0;
    //Get original data address and size from TPDRV
    TPDRV_GetPoints(buf_addr, size);
    data_ptr =(uint32*)(*buf_addr);

    if(*size == 2)
    {
        _TP_AdjustCoordinate((int32*)&(data_ptr[0]), (int32*)&(data_ptr[1]));  
    }
    else if(*size == 4)
    {
        _TP_AdjustCoordinate((int32*)&(data_ptr[0]), (int32*)&(data_ptr[1]));   
        _TP_AdjustCoordinate((int32*)&(data_ptr[2]), (int32*)&(data_ptr[3]));   
    }
    else
    {
        //To do later:
    }
    return SCI_TRUE;
}
#endif
