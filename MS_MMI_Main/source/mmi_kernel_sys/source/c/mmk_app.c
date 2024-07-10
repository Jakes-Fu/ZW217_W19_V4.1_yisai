/****************************************************************************
** File Name:      mmk_app.c                                               *
** Author:         Great.Tian                                              *
** Date:           06/11/2003                                              *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 11/2003        Great.Tian       Create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "mmi_kernel_sys_trc.h"
#include "mmk_app.h" 
#include "dal_lcd.h"
#include "freq_cfg.h"
#include "mmk_handle_internal.h"
#include "cafhelp.h"
#include "mmi_theme.h"

/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

 /**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#define AUTOTEST_TRACE_LENGTH       100


//the lcd width and height
#define MMI_WIN_ANIM_MOVE_HOR_FRAME_MAX			0x20
#define MMI_WIN_ANIM_MOVE_VER_FRAME_MAX			0x20
#define MMI_WIN_ANIM_MOVE_HOR_FRAME_TIMER		0x5
#define MMI_WIN_ANIM_MOVE_VER_FRAME_TIMER		0x5

#define MMI_WIN_ANIM_MOVE_FRAME_FORBIDDEN		1
#define MMI_WIN_ANIM_MOVE_INVALIDE_VALUE		0

#define TEST_LOW_MEM_PERCENT 20
#define TEST_NO_MEM_PERCENT  5

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         LOCAL Variables                                   *
 **---------------------------------------------------------------------------*/


/* setting and control variables for flip window */
LOCAL MMI_WIN_ANIM_MOVE_SETTING_TYPE_E	s_default_anim_move_type = MMI_WIN_ANIM_MOVE_SETTING_FORBID;
LOCAL uint32 s_slide_win_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;

#ifdef TOUCH_PANEL_SUPPORT
LOCAL MMK_SLIDE_RECOVER_INFO_T s_slide_recover_info = {0};
LOCAL VELOCITY_TRACKER_T s_velocity_tracker = {0};
#endif

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : get the window list pointer  
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_TYPE_E MMK_GetHandleType(
                                           MMI_HANDLE_T handle
                                           )
{
    MMI_BASE_NODE_T* base_node_ptr = MMK_FindBaseNode( handle );

    if( PNULL != base_node_ptr )
    {
        return base_node_ptr->handle_type;
    }
    else
    {
        return MMI_HANDLE_NONE;
    }
}

/*****************************************************************************/
// 	Description : init the system struct 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
LOCAL void InitChngFreqHandler(void)
{
	if (CHNG_FREQ_REQ_HANDLER_NULL == s_slide_win_chng_freq_req_handler)
	{
#ifndef WIN32
		s_slide_win_chng_freq_req_handler =  CHNG_FREQ_GetReqHandler("mmk_slide_win");
//		SCI_ASSERT (CHNG_FREQ_REQ_HANDLER_NULL != s_slide_win_chng_freq_req_handler);
#endif
	} 	
}

/*****************************************************************************/
// 	Description : init the system struct 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_InitSystem(
		     	              void
					          )
{
    InitChngFreqHandler();

    //init
    MMK_InitHandleList();    

    return TRUE;
}

/*****************************************************************************/
//     Description : set window move type
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC void MMK_SetWinDefaultAnimMoveStyle(MMI_WIN_ANIM_MOVE_SETTING_TYPE_E type)
{
	s_default_anim_move_type = type;
}

/*****************************************************************************/
//     Description : get window default move style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ANIM_MOVE_SETTING_TYPE_E MMK_GetWinDefaultAnimMoveStyle(void)
{
	return s_default_anim_move_type;
}

/*****************************************************************************/
// 	Description : 创建速度跟踪器
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void* MMK_CreateTracker( void )
{
    return SCI_ALLOC_APP(sizeof(VELOCITY_TRACKER_T));
}

/*****************************************************************************/
// 	Description : 释放速度跟踪器
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DestroyTracker(
                               void    *tracker_ptr
                               )
{
    if (PNULL != tracker_ptr)
    {
        SCI_FREE(tracker_ptr);
    }
}

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : 添加速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void AddVelocityItemByTracker(
									void    *tracker_ptr,
									int32   x,
									int32   y,
									MMI_TP_STATUS_E tp_action,
									uint32 tick_count
									)
{
	uint32 cur_tick = 0;
    VELOCITY_TRACKER_T *velocity_tracker_ptr = (VELOCITY_TRACKER_T*)tracker_ptr;

    if (PNULL == velocity_tracker_ptr)
    {
        return;
    }
#ifndef WIN32
    if(0 != tick_count)
    {
        cur_tick = tick_count;
    }
    else
#endif
    {
        cur_tick = SCI_GetTickCount(); // 统一取值，减小误差
    }

	// 当num 大于 0 时，判断
	if (velocity_tracker_ptr->item_num > 0)
	{
		if (abs(cur_tick - velocity_tracker_ptr->item[velocity_tracker_ptr->item_num - 1].time) > FLING_MIN_INVALID_TIME) // 说明无效的点，需要reset
		{
			MMK_ResetVelocityItem();
		}
	}

    if ( velocity_tracker_ptr->item_num < VELOCITY_TRACKER_ITEM_NUM )
    {
        velocity_tracker_ptr->item[velocity_tracker_ptr->item_num].x = x;
        velocity_tracker_ptr->item[velocity_tracker_ptr->item_num].y = y;
        velocity_tracker_ptr->item[velocity_tracker_ptr->item_num].time = cur_tick;
		velocity_tracker_ptr->item[velocity_tracker_ptr->item_num].tp_action = tp_action;
        velocity_tracker_ptr->item_num++;
    }
    else
    {
        memmove( velocity_tracker_ptr->item, velocity_tracker_ptr->item + 1, sizeof(VELOCITY_TRACKER_ITEM_T) * (VELOCITY_TRACKER_ITEM_NUM - 1) );

        velocity_tracker_ptr->item[VELOCITY_TRACKER_ITEM_NUM-1].x = x;
        velocity_tracker_ptr->item[VELOCITY_TRACKER_ITEM_NUM-1].y = y;
        velocity_tracker_ptr->item[VELOCITY_TRACKER_ITEM_NUM-1].time = cur_tick;
		velocity_tracker_ptr->item[VELOCITY_TRACKER_ITEM_NUM-1].tp_action = tp_action;

        velocity_tracker_ptr->item_num = VELOCITY_TRACKER_ITEM_NUM;
    }
}

/*****************************************************************************/
// 	Description : 添加速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_AddVelocityItemByTracker(
                                         void    *tracker_ptr,
                                         int32   x,
                                         int32   y
                                         )
{
	AddVelocityItemByTracker(tracker_ptr, x, y, MMI_TP_NONE,0);
}

/*****************************************************************************/
// 	Description : 重置速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_ResetVelocityItemByTracker(
                                           void    *tracker_ptr
                                           )
{
    VELOCITY_TRACKER_T *velocity_tracker_ptr = (VELOCITY_TRACKER_T*)tracker_ptr;

    if (PNULL == velocity_tracker_ptr)
    {
        return;
    }

    velocity_tracker_ptr->item_num = 0;
}

/*****************************************************************************/
// 	Description : 矫正最终速度
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL float AdjuestFinalVelocity(
								 float cur_velocity
								 )
{
#define FLING_DEF1_VELOCITY        (400)     // 默认的Fling速度
#define FLING_DEF2_VELOCITY        (800)     // 默认的Fling速度

	float final_vel = 0.0f;

	if (FLING_DEF2_VELOCITY <= cur_velocity)
	{
		final_vel = MIN(cur_velocity, FLING_DEF2_VELOCITY);
	}
	else
	{
		final_vel = MIN(cur_velocity, FLING_DEF1_VELOCITY);
	}

	return final_vel;
}

/*****************************************************************************/
// 	Description : 计算当前速度
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ComputeCurrentVelocityByTracker(
                                                   void    *tracker_ptr,
                                                   float   *vx_ptr, // x方向速度, in/out
                                                   float   *vy_ptr, // y方向速度, in/out
                                                   float   max_x_velocity,
												   float   max_y_velocity
                                                   )
{
    float  x_velocity = 0;
    float  y_velocity = 0;
    float  x_max_velocity = FLING_MAX_VELOCITY;
    float  y_max_velocity = FLING_MAX_VELOCITY;
    uint32 i = 0;
    uint8  item_num = 0;
    BOOLEAN result = FALSE;
    BOOLEAN is_last_point = FALSE; // 是否最后一个点的区间才有效，如果是，只适用默认速度
    BOOLEAN is_less_point = FALSE; // 是否是只有2、3个点
    VELOCITY_TRACKER_T *velocity_tracker_ptr = (VELOCITY_TRACKER_T*)tracker_ptr;

    if (PNULL == velocity_tracker_ptr)
    {
        return FALSE;
    }

    x_max_velocity = (float)abs((int)max_x_velocity);
    y_max_velocity = (float)abs((int)max_y_velocity);

    item_num = velocity_tracker_ptr->item_num;

    if ( item_num > 0)
    {
        int32 first_x = velocity_tracker_ptr->item[0].x;
        int32 first_y = velocity_tracker_ptr->item[0].y;
        uint32 first_time = velocity_tracker_ptr->item[0].time;
        uint32 start_index = 1;

        //超过两个点，丢弃第一个点
        if ( item_num > 2 )
        {
            first_x = velocity_tracker_ptr->item[1].x;
            first_y = velocity_tracker_ptr->item[1].y;
            first_time = velocity_tracker_ptr->item[1].time;
            start_index = 2;
        }
        
		// 只有两个点或者三个点的时候，适用default速度进行滑动
		if (2 == item_num || 3 == item_num)
		{
			is_less_point = TRUE;
		}

        for ( i = start_index; i < item_num; i++ ) 
        {
            int32 dur = velocity_tracker_ptr->item[i].time - first_time;

            if ( 0 != dur ) 
            {
                float dist = velocity_tracker_ptr->item[i].x - first_x;
                float vel = dist/dur * 1000;

                if ( (FLING_MIN_VELOCITY > abs((int32)x_velocity)) || (FLING_MIN_VELOCITY > abs((int32)vel)))
                {
                    x_velocity = vel;
					if (i == item_num - 2)
					{
						is_last_point = TRUE;
					}
                }
                else 
                {
                    x_velocity = (x_velocity + vel) * 0.5f;
                }


				if (is_last_point || is_less_point)
				{
                    x_velocity = AdjuestFinalVelocity(x_velocity);
				}
                
				is_last_point = FALSE;
                dist = velocity_tracker_ptr->item[i].y - first_y;
                vel = dist/dur * 1000;

                if(vel > abs(y_max_velocity))
                {
                    vel = abs(y_max_velocity);
                }
                else if(vel < -abs(y_max_velocity))
                {
                    vel = - abs(y_max_velocity);
                }

                if ( (FLING_MIN_VELOCITY > abs((int32)y_velocity)) || (FLING_MIN_VELOCITY > abs((int32)vel))) 
                {
                    y_velocity = vel;
					if (i == item_num - 2)
					{
						is_last_point = TRUE;
					}
                }
                else
                {
                    y_velocity = (y_velocity + vel) * 0.5f;
                }

				if (is_last_point || is_less_point)
				{
					y_velocity = AdjuestFinalVelocity(y_velocity);
						
				}
            }

        }
    }

    if ( PNULL != vx_ptr )
    {
        if ( x_velocity > FLING_MIN_VELOCITY )
        {
            if ( *vx_ptr > 0 )
            {
                x_velocity += *vx_ptr;
            }

            *vx_ptr = MIN( x_velocity, x_max_velocity );
        }
        else if ( x_velocity < -FLING_MIN_VELOCITY )
        {
            if ( *vx_ptr < 0 )
            {
                x_velocity += *vx_ptr;
            }

            *vx_ptr = MAX( x_velocity, -x_max_velocity );
        }
        else
        {
            *vx_ptr = 0;
        }

        result = TRUE;
    }

    if ( PNULL != vy_ptr )
    {
        if ( y_velocity > FLING_MIN_VELOCITY )
        {
            if ( *vy_ptr > 0 )
            {
                y_velocity += *vy_ptr;
            }

            *vy_ptr = MIN( y_velocity, y_max_velocity );
        }
        else if ( y_velocity < -FLING_MIN_VELOCITY )
        {
            if ( *vy_ptr < 0 )
            {
                y_velocity += *vy_ptr;
            }

            *vy_ptr = MAX( y_velocity, -y_max_velocity );
        }
        else
        {
            *vy_ptr = 0;
        }

        result = TRUE;
    }

    velocity_tracker_ptr->item_num = 0;

    return result;
}

/*****************************************************************************/
// 	Description : 添加速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
/*PUBLIC void MMK_AddVelocityItem(
                                int32 x,
                                int32 y
                                )
{
	// 暂时屏蔽，by xiaoqing
    MMK_AddVelocityItemByTracker(&s_velocity_tracker, x, y);
	
}*/

/*****************************************************************************/
// 	Description : 添加速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_AddVelocityItemExt(
								   int32 x,
								   int32 y,
								   MMI_TP_STATUS_E tp_action,
								   uint32 tick_count
								   )
{
    AddVelocityItemByTracker(&s_velocity_tracker, x, y, tp_action, tick_count);
}

/*****************************************************************************/
// 	Description : 重置速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_ResetVelocityItem( void )
{
    MMK_ResetVelocityItemByTracker(&s_velocity_tracker);
}

/*****************************************************************************/
// 	Description : 计算当前速度
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ComputeCurrentVelocity(
                                          float *vx_ptr, // x方向速度, in/out
                                          float *vy_ptr, // y方向速度, in/out
                                          float max_x_velocity,
										  float max_y_velocity
                                          )
{
    return MMK_ComputeCurrentVelocityByTracker(&s_velocity_tracker, vx_ptr, vy_ptr, max_x_velocity, max_y_velocity);
}
/*****************************************************************************/
//  Description : 获取当前速度追踪器的信息
//	Global resource dependence :
//  Author: longwei.qiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetCurrentVelocityTracker(VELOCITY_TRACKER_T* velocity_tracker_ptr)//in ,out
{
    if(velocity_tracker_ptr == PNULL)
    {
       SCI_TRACE_LOW("MMK_GetCurrentVelocityTracker velocity_tracker_ptr is PNULL");
       return FALSE;
    }
    SCI_MEMCPY(velocity_tracker_ptr, &s_velocity_tracker, sizeof(VELOCITY_TRACKER_T));
    return TRUE;
}

/*****************************************************************************/
// 	Description : 添加速度点
//	Global resource dependence :
//  Author:arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMK_AddVelocityItemByApp(
                                     VELOCITY_TRACKER_T *velocity_tracker_ptr,
                                     int32 x,
                                     int32 y,
                                     MMI_TP_STATUS_E tp_action
                                     )
{
    AddVelocityItemByTracker(velocity_tracker_ptr, x, y, tp_action, 0);
}
/*****************************************************************************/
// 	Description : 重置速度点
//	Global resource dependence :
//  Author: arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMK_ResetVelocityItemByApp( VELOCITY_TRACKER_T *velocity_tracker_ptr)
{
    MMK_ResetVelocityItemByTracker(velocity_tracker_ptr);
}

/*****************************************************************************/
// 	Description : 计算当前速度
//	Global resource dependence :
//  Author: arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ComputeCurrentVelocityByApp(
                                                        VELOCITY_TRACKER_T *velocity_tracker_ptr,
                                                        float *vx_ptr, // x方向速度, in/out
                                                        float *vy_ptr, // y方向速度, in/out
                                                        float max_x_velocity,
                                                        float max_y_velocity
                                                        )
{
    return MMK_ComputeCurrentVelocityByTracker(velocity_tracker_ptr, vx_ptr, vy_ptr, max_x_velocity, max_y_velocity);
}
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMK_GetFlingOffset( 
                                int32  cur_pos,
                                float  velocity,
                                float  time,
                                float  *out_velocity
                                )
{
    int32 result = cur_pos;
    float a = 0;

    a = MMITHEME_GetFlingFrictionExt();

    //if ( 0 != velocity || 0 == a)
    {
        float  v2 = a * time;
        float  temp_v = ( velocity < 0 ) ? -velocity : velocity;
        float  distance = temp_v * time - v2 * time * 0.5f;

        if ( distance < 0)
        {
            distance = 0;
        }

        if ( velocity > 0 )
        {
            velocity -= v2;

            if ( velocity < 0 )
            {
                velocity = 0;
            }

            result = cur_pos + (int32)distance;
        }
        else
        {
            velocity += v2;

            if ( velocity > 0 )
            {
                velocity = 0;
            }

            result = cur_pos - (int32)distance;
        }   
    }
    
    if ( PNULL != out_velocity )
    {
        *out_velocity = velocity;
    }

    return result;
}


/*****************************************************************************/
//  Description : 判断是否改变了方向
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWayChanged(
                                float            cur_velocity, // 当前的速度
                                GUI_POINT_T      *start_point_ptr, // 起始点
                                GUI_POINT_T      *cur_point_ptr, // 当前点
                                BOOLEAN          is_x_way  // 是否x方向
                                )
{
    BOOLEAN result = FALSE;
    int16 start_pos = 0;
    int16 cur_pos = 0;

    //SCI_ASSERT(PNULL != start_point_ptr);
    //SCI_ASSERT(PNULL != cur_point_ptr);

    if ( PNULL == start_point_ptr
        || PNULL == cur_point_ptr )
    {
        return FALSE;
    }

    if (is_x_way)
    {
        cur_pos = cur_point_ptr->x;
        start_pos = start_point_ptr->x;
    }
    else
    {
        cur_pos = cur_point_ptr->y;
        start_pos = start_point_ptr->y;
    }

    if (cur_velocity >= 0 && cur_pos - start_pos <= 0)
    {
        result = TRUE;
    }
    else if (cur_velocity <= 0 && cur_pos - start_pos >= 0)
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}

/*****************************************************************************/
//  Description : 重置信息
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: from tim.xia
/*****************************************************************************/
PUBLIC void MMK_ResetSlideRecoverInfo(void)
{
    s_slide_recover_info.orig_offset_y = 0;
    s_slide_recover_info.orig_offset_x = 0;
    s_slide_recover_info.odd_distance = 0;
    s_slide_recover_info.run_distance = 0;
}

/*****************************************************************************/
//  Description : 设置移动的y方向原始偏移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_SetRecoverOrigY(int32 orig_offset_y)
{
    s_slide_recover_info.orig_offset_y = orig_offset_y;
}

/*****************************************************************************/
//  Description : 设置移动的x方向原始偏移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_SetRecoverOrigX(int32 orig_offset_x)
{
    s_slide_recover_info.orig_offset_x = orig_offset_x;
}

/*****************************************************************************/
//  Description : 设置移动的剩余位移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_SetRecoverOddDistance(int32 odd_distance)
{
    s_slide_recover_info.odd_distance = odd_distance;
}

/*****************************************************************************/
//  Description : 设置移动的原始位移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_SetRecoverRunDistance(int32 run_distance)
{
    s_slide_recover_info.run_distance = run_distance;
}

/*****************************************************************************/
//  Description : 获得移动的y方向原始偏移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMK_GetRecoverOrigY(void)
{
    return s_slide_recover_info.orig_offset_y;
}

/*****************************************************************************/
//  Description : 获得移动的x方向原始偏移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMK_GetRecoverOrigX(void)
{
    return s_slide_recover_info.orig_offset_x;
}

/*****************************************************************************/
//  Description : 获得移动的剩余位移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMK_GetRecoverOddDistance(void)
{
    return s_slide_recover_info.odd_distance;
}

/*****************************************************************************/
//  Description : 获得移动的原始位移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMK_GetRecoverRunDistance(void)
{
    return s_slide_recover_info.run_distance;
}
#endif
/*****************************************************************************/
//  Description : 计算阻尼运动的位移偏移
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: from tim.xia
/*****************************************************************************/
PUBLIC int32 MMK_ClcltDampPlayOffset(
                                     MMK_DAMP_PLAY_TYPE_E   type,		//运动类型
                                     int16	                iLen,			//运动总位移
                                     int32                  iTotalFrame,	//总帧数
                                     int32                  iNowFrameNum	//当前帧
                                     )
{
	int32 iMidFrame_4 = 0;
	int32 offset = 0;

	if(iTotalFrame <= 0 || iTotalFrame >= 64 || iTotalFrame<iNowFrameNum || iTotalFrame*iTotalFrame*iLen>22369621)
	{
		return 0;
	}

	switch(type)
	{
	case DAMP_PLAY_TYPE_1:				//强加速弱减速
		iMidFrame_4 = iTotalFrame*4;
		if(iNowFrameNum <= iMidFrame_4/16)
		{
			offset = (iNowFrameNum*iNowFrameNum*iLen*64)/(iMidFrame_4*iMidFrame_4);
		}
		else
		{
			iNowFrameNum = iTotalFrame - iNowFrameNum;
			iMidFrame_4 = iTotalFrame*16 - iTotalFrame*4;
			offset = iLen - (iNowFrameNum*iNowFrameNum*iLen*3*64)/(iMidFrame_4*iMidFrame_4);
		}
		break;
	case DAMP_PLAY_TYPE_2:				//相同加速减速
		iMidFrame_4 = iTotalFrame*8;
		if(iNowFrameNum <= iMidFrame_4/16)
		{
			offset = (iNowFrameNum*iNowFrameNum*iLen*2*64)/(iMidFrame_4*iMidFrame_4);
		}
		else
		{
			iNowFrameNum = iTotalFrame - iNowFrameNum;
			iMidFrame_4 = iTotalFrame*16 - iTotalFrame*8;
			offset = iLen - (iNowFrameNum*iNowFrameNum*iLen*2*64)/(iMidFrame_4*iMidFrame_4);
		}
		break;
	case DAMP_PLAY_TYPE_3:				//弱加速强减速
		iMidFrame_4 = iTotalFrame*12;
		if(iNowFrameNum <= iMidFrame_4/16)
		{
			offset = (iNowFrameNum*iNowFrameNum*iLen*3*64)/(iMidFrame_4*iMidFrame_4);
		}
		else
		{
			iNowFrameNum = iTotalFrame - iNowFrameNum;
			iMidFrame_4 = iTotalFrame*16 - iTotalFrame*12;
			offset = iLen - (iNowFrameNum*iNowFrameNum*iLen*64)/(iMidFrame_4*iMidFrame_4);
		}
		break;
	case DAMP_PLAY_TYPE_4:				//阻尼振动
		if(iNowFrameNum&0x01)
		{
			offset = ((iLen*256/iTotalFrame)*(iTotalFrame-iNowFrameNum))/256;
		}
		else
		{
			offset = -((iLen*256/iTotalFrame)*(iTotalFrame-iNowFrameNum))/256;
		}
        break;
    case DAMP_PLAY_TYPE_5:			//匀减速运动
        offset = iLen * iNowFrameNum * ( 2 * iTotalFrame - iNowFrameNum)/ (iTotalFrame * iTotalFrame);
        break;
	default:
        //SCI_TRACE_LOW:"MMK_DAMP_PLAY_TYPE_E error type %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMK_APP_706_112_2_18_3_23_5_0,(uint8*)"d", type);
		offset = 0;
		break;
	}

    return offset;
}

/*****************************************************************************/
// 	Description : is tp single click
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsSingleClick( 
                                 MMI_MESSAGE_ID_E   msg_id, 
                                 DPARAM             param
                                 )
{
    BOOLEAN is_single_click = FALSE;

    if ( MSG_CTL_PENOK == msg_id && PNULL != param )
    {
        MMI_NOTIFY_T* notify_ptr = (MMI_NOTIFY_T*)param;
        
        if ( !notify_ptr->code )
        {
            is_single_click = TRUE;
        }
    }

    return is_single_click;
}

/*****************************************************************************/
//     Description : test low memory
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:测试再分配buf_size后，剩余内存是否是低内存
/*****************************************************************************/
PUBLIC BOOLEAN MMK_TestMemory(
                              MMK_TEST_MEMORY_E type,
                              uint32            buf_size
                              )
{
    uint64 test_size = SCI_GetHeapTotalAvailableSpace( DYNAMIC_HEAP_APP_MEMORY );
    uint64 max_size  = SCI_GetHeapTotalSpace( DYNAMIC_HEAP_APP_MEMORY );
    uint64 percent   = 0;
    BOOLEAN result   = FALSE;

    if ( type >= MMK_TEST_MEM_NUM )
    {
        return FALSE;
    }

    if ( test_size > buf_size )
    {
        test_size -= buf_size;
    }
    else
    {
        test_size = 0;
    }

    if ( max_size > 0 )
    {
        percent = test_size * 100 / max_size;
    }

    switch( type )
    {
    case MMK_TEST_LOW_MEM:
        if ( percent < TEST_LOW_MEM_PERCENT )
        {
            result = TRUE;
        }
        break;
    case MMK_TEST_NO_MEM:
        if ( percent < TEST_NO_MEM_PERCENT )
        {
            result = TRUE;
        }
        break;
    default:
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author: 		James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_CheckAllocatedMemInfo(
                                      const char* file_name_ptr,
                                      uint32 line
                                      )
{
#ifdef MMI_CHECK_MEMINFO_SUPPORT
    if(TRUE == MMIAPIENG_GetMemCheckFlag())
    {
        SCI_Mem_Check();
    }
#endif
}

