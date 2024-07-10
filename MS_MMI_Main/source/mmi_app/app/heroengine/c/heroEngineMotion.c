#ifdef HERO_ENGINE_SUPPORT

#include "heroEngineExport.h"
#include "heroEngineId.h"
#include "mmk_gsensor.h"
#include "heroEngineMotion.h"

#ifdef __HERO_APP_MOTION_SUPPORT__

static T_MOTION_STATE gHeroMotionState = T_MOTION_PWROFF;
static void (*gDsmMotionCb)(void) = NULL;
static uint8   g_mr_motion_timer = 0;

static void hero_motion_read_acc_handler(void)
{
	if((gHeroMotionState == T_MOTION_LISTENING) &&gDsmMotionCb)
	{
		gDsmMotionCb();
	}
}

static void hero_motion_check_data(T_MOTION_ACC *p,T_MOTION_ACC *data)
{
	/*将平台的xyz坐标系和VM的坐标系对应*/	
	p->x = -data->x;
	p->y = data->y;
	p->z = data->z;

	if(p->x > HERO_MOTION_MAX_ACC)
		p->x = HERO_MOTION_MAX_ACC;
	else if(p->x <-HERO_MOTION_MAX_ACC)
		p->x = -HERO_MOTION_MAX_ACC;
	
	if(p->y > HERO_MOTION_MAX_ACC)
		p->y = HERO_MOTION_MAX_ACC;
	else if(p->y <-HERO_MOTION_MAX_ACC)
		p->y = -HERO_MOTION_MAX_ACC;

	if(p->z > HERO_MOTION_MAX_ACC)
		p->z = HERO_MOTION_MAX_ACC;
	else if(p->z <-HERO_MOTION_MAX_ACC)
		p->z = -HERO_MOTION_MAX_ACC;	
}

static void dsm_motion_shake_cb(void)
{
    T_MOTION_ACC p = {0};
    T_MOTION_ACC data = {0};
    MMK_GSENSOR_VECTOR_T vector = {0};
    
    if(gHeroMotionState == T_MOTION_LISTENING)
    {
    	if(TRUE == MMK_GetGsensorVector(&vector))
        {   
            data.x = vector.x;
            data.y = vector.y;
            data.z = vector.z;
    		hero_motion_check_data(&p, &data);
            if((p.x > (HERO_MOTION_MAX_ACC/2))||(p.x < -(HERO_MOTION_MAX_ACC/2))
            	||(p.y > (HERO_MOTION_MAX_ACC/2))||(p.y < -(HERO_MOTION_MAX_ACC/2))
            	||(p.z > (HERO_MOTION_MAX_ACC/2))||(p.z < -(HERO_MOTION_MAX_ACC/2))
              )
            {      
    			hr_event(HR_MOTION_EVENT, HR_MOTION_EVENT_SHAKE, (int32)&p);	
            }
        }
    }
}

static void dsm_motion_tilt_cb(void)
{
    T_MOTION_ACC p = {0};
    T_MOTION_ACC data = {0};
    MMK_GSENSOR_VECTOR_T vector = {0};
    if(gHeroMotionState == T_MOTION_LISTENING)
    {
    	if(TRUE == MMK_GetGsensorVector(&vector))
        {   
            data.x = vector.x;
            data.y = vector.y;
            data.z = vector.z;
    		hero_motion_check_data(&p,&data);
            if((p.x > (HERO_MOTION_MAX_ACC/2))||(p.x < -(HERO_MOTION_MAX_ACC/2))
            	||(p.y > (HERO_MOTION_MAX_ACC/2))||(p.y < -(HERO_MOTION_MAX_ACC/2))
            	||(p.z > (HERO_MOTION_MAX_ACC/2))||(p.z < -(HERO_MOTION_MAX_ACC/2))
              )
            {      
    			hr_event(HR_MOTION_EVENT, HR_MOTION_EVENT_TILT, (int32)&p);	
            }
        }
    }
}	

int32 hr_motion_stop_listen(int32 param)
{
	gDsmMotionCb = NULL;
	gHeroMotionState = T_MOTION_IDLE;
    MMK_CloseGsensor(MMI_HERO_APP_BASE_WIN_ID);
    MMK_StopTimer(g_mr_motion_timer);
    g_mr_motion_timer = 0;
	return HR_SUCCESS;
}

int32 hr_motion_power_on(int32 param)
{
	gDsmMotionCb = NULL;

    if(TRUE == MMK_OpenGsensor(MMI_HERO_APP_BASE_WIN_ID))
    {
    	gHeroMotionState = T_MOTION_PWERON;
    	return HR_SUCCESS;
    }
    else
    {
    	gHeroMotionState = T_MOTION_PWROFF;
    	return HR_FAILED;
    }
}

int32 hr_motion_power_off(int32 param)
{
	gDsmMotionCb = NULL;
	gHeroMotionState = T_MOTION_PWROFF;
    MMK_StopTimer(g_mr_motion_timer);
    g_mr_motion_timer = 0;
	return HR_SUCCESS;
}

int32 hr_motion_listen_shake(int32 param)
{
	if(param >=HR_MOTION_SENSITIVE_MAX)
		return HR_FAILED;

	if(gHeroMotionState == T_MOTION_PWROFF)
		return HR_FAILED;

	gHeroMotionState = T_MOTION_LISTENING;
	gDsmMotionCb = dsm_motion_shake_cb;
    if(g_mr_motion_timer != 0)
    {
        MMK_StopTimer(g_mr_motion_timer);
    }
    g_mr_motion_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, HERO_MOTION_FREQUENCY, TRUE);
	return HR_SUCCESS;
}

int32 hr_motion_listen_tilt(int32 param)
{
	if(param >=HR_MOTION_SENSITIVE_MAX)
		return HR_FAILED;

	if(gHeroMotionState == T_MOTION_PWROFF)
		return HR_FAILED;

	gHeroMotionState = T_MOTION_LISTENING;
	gDsmMotionCb = dsm_motion_shake_cb;
    if(g_mr_motion_timer != 0)
    {
        MMK_StopTimer(g_mr_motion_timer);
    }
    g_mr_motion_timer = MMK_CreateWinTimer(VIRTUAL_WIN_ID, HERO_MOTION_FREQUENCY, TRUE);
	return HR_SUCCESS;
}

int32 hr_motion_get_max_acc(int32 param)
{
	return (HR_PLAT_VALUE_BASE+HERO_MOTION_MAX_ACC);
}

int32 hr_motion_handle_timer(uint8 timer_id)
{
    int32 ret = 0;
    if(timer_id == g_mr_motion_timer)
    {
        hero_motion_read_acc_handler();
        ret = 1;
    }
    return ret;
}

#endif
#endif

