#include "mmk_timer.h"
#include "mmk_app.h"
#include "od_adapter_log.h"
#include "od_adapter_timer.h"

#define OD_TIMER_COUNT_MAX  (30)
typedef struct
{
	Od_TimerCallbackFunc callback;
	void *data_ptr;
	OD_U8 id;
	OD_U8 flag;
	
}Od_TimerItem_t;

static Od_TimerItem_t sTimeItems[OD_TIMER_COUNT_MAX]={0};


static OD_U8 Od_CreateTimerInter(OD_U32  time_out,Od_TimerCallbackFunc callback,void *data_ptr,OD_U8 flag)
{
	    int i=0;
		
		if(callback==OD_NULL)
			return 0;
		
		for(i=0;i<OD_TIMER_COUNT_MAX;i++)
		{
			if(sTimeItems[i].id == 0)
			{
				sTimeItems[i].id = MMK_CreateWinTimer(VIRTUAL_WIN_ID,time_out,OD_FALSE);
				sTimeItems[i].callback = callback;
				sTimeItems[i].data_ptr = data_ptr;
				sTimeItems[i].flag = flag;
				//OD_LOG("Od_CreateTimerInter-id=%d i=%d fun=%d,flag=%d",sTimeItems[i].id,i,callback,flag);
				return sTimeItems[i].id;
			}
		}
		//OD_LOG("Od_CreateTimerInter-failure");
		return 0;
	
}
OD_U8 Od_CreateTimerEx(OD_U32  time_out,Od_TimerCallbackFunc callback,void *data_ptr)
{
	return Od_CreateTimerInter(time_out,(Od_TimerCallbackFunc)callback,data_ptr,OD_FALSE);	
}

OD_U8 Od_CreateTimer(OD_U32  time_out,OD_FUNC_PTR callback)
{
	
	return Od_CreateTimerInter(time_out,(Od_TimerCallbackFunc)callback,OD_NULL,OD_TRUE);
}

void Od_StopTimer(OD_U8 timer_id)
{
  OD_U16 i; 
  
  if(timer_id>0)
  {
  	for(i=0;i<OD_TIMER_COUNT_MAX;i++)
 	{
    	if(sTimeItems[i].id == timer_id)
    	{
    		//OD_LOG("Od_StopTimer-id=%d timer_id=%d",sTimeItems[i].id,timer_id);
  			MMK_StopTimer(sTimeItems[i].id );
			sTimeItems[i].id = 0;
			sTimeItems[i].callback = NULL;
			sTimeItems[i].data_ptr= NULL;
			sTimeItems[i].flag = OD_FALSE;
    	}
  	}
  }
}
OD_BOOL Od_OnTimerProcess(OD_U8 timer_id)
{
	int i=0;
	for(i=0;i<OD_TIMER_COUNT_MAX;i++)
	{
		if(sTimeItems[i].id == timer_id)
		{
			//OD_LOG("Od_OnTimerProcess-i=%d id=%d callback=%d,flag=%d",i,timer_id,sTimeItems[i].callback,sTimeItems[i].flag);
			MMK_StopTimer(timer_id );
			sTimeItems[i].id = 0;
			if(sTimeItems[i].callback)
			{
				if(sTimeItems[i].flag)
					((OD_FUNC_PTR)sTimeItems[i].callback)();
				else
					sTimeItems[i].callback(sTimeItems[i].data_ptr);
			}			
			return OD_TRUE;
		}
	}
	return OD_FALSE;

}

OD_U8 Od_CreateDaemonTimer(OD_U32  time_out,   //Specifies the expire value in milliseconds
                         OD_BOOL is_period)              //the period timer if is_period is TRUE
                                                          //the once timer if is_period is FALSE  
{
	return MMK_CreateWinTimer(VIRTUAL_WIN_ID,time_out,  is_period);                                                               
}

OD_BOOL Od_StopDaemonTimer(OD_U8  timer_id)
{
	return MMK_StopTimer(timer_id )== OD_TRUE? OD_TRUE:OD_FALSE;
                      
}

