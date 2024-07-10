/*************************************************************************
 ** File Name:    tp_srv.c                                              *
 ** Author:       tao.feng                                              *
 ** Date:         6/10/2008                                           *
 ** Copyright:    2008 Spreadtrum, Incorporated. All Rights Reserved.    *
 ** Description:  This file  define TPC module task       *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE               NAME             DESCRIPTION                          *
 ** 06/11/2008         tao.feng          Create.                              *
*************************************************************************/
#include "ms_ref_base_trc.h"
#include "sci_types.h"
#include "tp_srv.h"
#include "threadx_os.h"
#include "os_apiext.h"
#include "tpc_drv.h"
#include "tp_api.h"
#include "watchdog.h"
#include "ref_outport.h"
#include "tx_port_thumb.h"
#include "diag.h"
#include "sci_api.h"
#include "dal_lcd.h"
#include "dal_keypad.h"
#include "tp_cfg.c"

#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/******************************************************************************
 * Macro definitions
 ******************************************************************************/
 #define TPDRV_DEBUG
#ifdef TPDRV_DEBUG
    #define TPDRV_PRINT	SCI_TRACE_LOW
#else
    #define TPDRV_PRINT	
#endif
#ifdef TOUCH_PANEL_HWICON_SUPPORT_NONE
#define TP_HWICON_Y_LENGHT 			 0
#else
#define TP_HWICON_Y_LENGHT 			 25
#endif

#define TP_FILTER_LIMIT                            4000     /* define the maximum range of noise window */
#define DX_OFFSET                                      0x03
#define TP_FILTER_DATA_COUNT                0x02     /* define the data count above which will start TP coord filter */
#define TP_FILTER_TBD_DATA_COUNT        0x20     /* define the data count above which will start TP coord filter */
#define TP_SMOOTH_DEPTH                         0x03     /* define the smooth filter depth */
#define TP_PRECISION                                 1000
#define TP_X_TOP_FACTOR_MAX    	             6000
#define TP_Y_LEFT_FACTOR_MAX   	      6000
#define TP_ADC_MAX			   	             1023 // 10 bit 

#define TP_MSG_MAX_NUM     				(TP_MSG_MAX & 0x00FF)
#define TP_ALLOW_SEND_QUE_SPACE 		10
#define TP_APP_BUF_FULL_SLEEP_TIME 		50
/******************************************************************************
 * Structure definitions
 ******************************************************************************/
 #if 0
typedef enum {
	TP_PEN_NONE = 0x00,
    TP_PEN_DOWN=1,    /*1*/  
    TP_PEN_UP,      /*2*/
    TP_PEN_DONE,    /*3*/
    TP_UNKNOWN_STATE/*4*/
} TP_STATE_E;
#endif//sunnyfei_tpm
typedef enum {
      TP_MOVE_EXCEED_LIMIT,
      TP_MOVE_SMOOTH,
      TP_MOVE_PRE
} TP_FILTER_RETURN_E;

#if 0
typedef enum {
	#ifdef WIN32
	TP_DOWN_MSG =  0x211 ,
	#else
	TP_DOWN_MSG =  (TP_SERVICE << 8) | 1 ,
	#endif
	TP_UP_MSG,
	TP_MOVE_MSG,
	TP_LONGTAP_MSG,
	TP_MAX_MSG
} TP_MSG_E;
#endif //sunnyfei_tpm

typedef struct
{   
    uint16 x;  /*x coordinate*/
    uint16 y;  /*y coordinate*/ 
}TP_COORD_T; 

typedef struct
{
	TP_COORD_T  data;
	TP_MSG_E    event;
}TP_MSG_T;

typedef struct {                   
	TP_MSG_E    TP_type;
	uint16 		TP_x;
	uint16 		TP_y;        
} TP_COMMON_T, *TP_COMMON_T_PTR;

typedef struct
{   
	uint16 x_adc;  /*x adc*/
	uint16 y_adc;  /*y adc*/ 
}TP_ADC_T; 

typedef void (*TP_MSG_FUNC)(TP_MSG_T tp_msg); 
typedef struct
{   
	TP_COORD_T 	   cur;             /*current point coord.*/
	TP_COORD_T 	   pre;             /*previous point coord.*/
	TP_MSG_FUNC    tp_event_cb; /*callback function*/
	TP_COORD_T 	   mov_buf[TP_FILTER_DATA_COUNT];/*used to store the temporary move filter data*/
	TP_COORD_T 	   movtbdbuf[TP_FILTER_TBD_DATA_COUNT];/*used to store the temporary move filter data*/
	uint8		   mtbdcnt;    
	BOOLEAN		   tbdState;
	uint8 		   mcnt;//move filter depth
	TP_COORD_T 	   smooth_buf[TP_SMOOTH_DEPTH];/*used to store the temporary smooth filter data*/
	uint8 		   smooth_cnt;             /*uesed to define the depth of the smooth filter.*/
	TP_STATE_E      state;  /*Down or UP*/  
	BOOLEAN  	   is_buff_full;//if use ring buf,this parameter indicate if the ring buf is full
	BOOLEAN 	   first_down_irq;//parameter indicate the firset enter done irq
	BOOLEAN 	   msg_could_send;//if use ring buf,this parameter indicate if driver layer could send message to uper layer
	BOOLEAN		   cal_en;
}TP_MAIN_INFO_T;


/******************************************************************************
 * Static functions prototypes
 ******************************************************************************/
LOCAL void  TP_Init(void);
LOCAL void TP_MsgHandler(uint8 msg_type);

LOCAL TP_MAIN_INFO_T 		    TP; 
/*LOCAL parameters and functions will be put below*/
LOCAL TP_DRV_FUNC_T	    *s_tp_drv_fun = NULL;
LOCAL TP_CALIBRATION_T   s_tp_cal     = {0};  
LOCAL TP_INFO_T		     s_tp_info_t  = {0};
LOCAL BOOLEAN		     s_is_rawdata = SCI_FALSE;	
//touch panel signal send to client  
LOCAL TPDSVR_SIG_T       s_tp_sig;       
LOCAL uint16			 s_screen_x_start = 0;
LOCAL uint16			 s_screen_y_start = 0;
LOCAL uint16			 s_screen_x_end   = 240;
LOCAL uint16			 s_screen_y_end   = 320;

/******************************************************************************
 * External functions prototypes
 ******************************************************************************/
#ifdef TP_SELFADAPTIVE_ENABLE
extern TP_MODE_E TP_GetCurAdaptMode(void);
#endif //TP_SELFADAPTIVE_ENABLE

/************************************************************************/
//  Function name:	Tp_Default
//  Description:   callback dummy function
//  Global resource depend
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void TP_Default( BLOCK_ID id, uint32 argc, void *argv )
{
    //Do nothing ;
}

/************************************************************************/
//  Function name:	tp_get_app_queue_avilable
//  Description:   this function will get the avilable app task queue space
//  Global resource depend
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 TP_Get_Task_Queue_Avilable(thread_id_type task_id)
{
	uint32         status   = 0;
	uint32         count    = 0;
	uint32         enqueued = 0;
	uint32         store    = 0;
	char           *queue_name_ptr    = SCI_NULL;
	TX_THREAD      * suspended_thread = SCI_NULL;	
	TX_QUEUE       * next_queue       = SCI_NULL;
	SCI_THREAD_T   *thread_block  	  = SCI_NULL;
	   
	thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(task_id);
	status = tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
		                       &queue_name_ptr,
							   (uint32 *)&enqueued,
							   (uint32 *)&store,
							   (TX_THREAD **)&suspended_thread,
							   (uint32 *)&count,
							   (TX_QUEUE **)&next_queue);
    return store;
}


/*****************************************************************************/
//  Function name:	THREAD_ENTRY
//  Description:  Main thread of TPSVR task. It manages the pen signal
//  from driver, and send it to the tasks which registered to it before.
//  Author: tao.feng
//  Note:
/*****************************************************************************/
THREAD_ENTRY(P_TP)
{
	#ifndef WIN32
	
	TPD_TO_TPSVR_MSG_T *sig_ptr = SCI_NULL;
	TP_Init();
	SCI_CreateClientList(TP_SERVICE,(TP_MSG_MAX_NUM & 0x0ff),(REG_CALLBACK)TP_Default);

	#endif
	
	while (1)
	{
		#ifndef WIN32
		// Receive signal.
		sig_ptr = (TPD_TO_TPSVR_MSG_T  *)SCI_GetSignal(P_TPP);

		if( (sig_ptr->SignalCode == DOWN_IRQ)
				||(sig_ptr->SignalCode == UP_IRQ) \
					||(sig_ptr->SignalCode == DONE_IRQ) )
		{
			TP_MsgHandler(sig_ptr->SignalCode);
		}
		SCI_FREE(sig_ptr);
		
		#else
		tp_sig_ptr = (TPDSVR_SIG_T *)SCI_GetSignal(P_TPP);
		SCI_SendEventToClient(TP_SERVICE, tp_sig_ptr->SignalCode, (void *)tp_sig_ptr);  //20050625
		SCI_FREE(tp_sig_ptr);
		#endif
	}//end while(1)  
}

/*****************************************************************************/
//  Function name:	TP_SendSignal
//  Description:  This function send the signal to tp task
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void TP_SendSignal(uint8 status)
{
	TPD_TO_TPSVR_MSG_T *tp_to_tpsvr_sig_ptr    =  SCI_NULL;
	
	//TPDRV_PRINT("TP_DRIVER: enter into tp_irq_handler\n");
	tp_to_tpsvr_sig_ptr = (TPD_TO_TPSVR_MSG_T *)SCI_ALLOC(sizeof(TPD_TO_TPSVR_MSG_T));
	tp_to_tpsvr_sig_ptr->SignalSize = sizeof(TPD_TO_TPSVR_MSG_T);
	tp_to_tpsvr_sig_ptr->Sender     = P_TPP;
	SCI_ASSERT(SCI_NULL != tp_to_tpsvr_sig_ptr);/*assert verified*/
	tp_to_tpsvr_sig_ptr->SignalCode = status;
	SCI_SendSignal((xSignalHeader)tp_to_tpsvr_sig_ptr, P_TPP);    	
}

/*****************************************************************************/
//  Function name:	Tp_SendMessage
//  Description:  This function send  message to  tp service
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void Tp_SendMessage(TP_MSG_T tp_msg)
{
	uint32  		app_queue_avilable;  
	TPDSVR_SIG_T 	tp_sig_ptr;
	
	#ifndef TP_RING_BUF_EN
	tp_sig_ptr.x_key = tp_msg.data.x;
	tp_sig_ptr.y_key = tp_msg.data.y;
	#endif
	switch(tp_msg.event)
	{
		case TP_DOWN_MSG :
			tp_sig_ptr.SignalCode = TP_DOWN;         
			//TPDRV_PRINT("TP_DRIVER: TP_DOWN message is sended\n");
			break;
		case TP_MOVE_MSG:
			tp_sig_ptr.SignalCode = TP_MOVE;            
			//TPDRV_PRINT("TP_DRIVER: TP_MOVE message is sended\n");
			break;
		case TP_UP_MSG :
			tp_sig_ptr.SignalCode = TP_UP;
			// TPDRV_PRINT("TP_DRIVER: TP_UP message is sended\n");
			break;
		default : 
			break;
	}
	app_queue_avilable = TP_Get_Task_Queue_Avilable(P_APP);
	if (app_queue_avilable > TP_ALLOW_SEND_QUE_SPACE)
	{
		SCI_SendEventToClient(TP_SERVICE, tp_sig_ptr.SignalCode, (void *)&tp_sig_ptr);
	}
	else
	{
		do
		{
			SCI_Sleep(TP_APP_BUF_FULL_SLEEP_TIME);    
		}while(TP_Get_Task_Queue_Avilable(P_APP) < TP_ALLOW_SEND_QUE_SPACE);
	}
}

/*****************************************************************************/
//  Function name:	_TP_CbRegistration
//  This function is to register touch event callback function.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_CbRegistration (TP_MSG_FUNC Callback)
{
	TP.tp_event_cb = Callback;
}

/*****************************************************************************/
//  Function name:	_Tp_MsgSend
//  Description:    This function is to send touch event indication to upper layer.
//  Author: tao.feng
//  Note:
/*****************************************************************************/
/*send touch event indication to upper layer*/
LOCAL void _Tp_MsgSend(TP_MSG_T msg)
{
#ifdef TP_RING_BUF_EN
	if (TP.msg_could_send)
	{
		TP.msg_could_send = SCI_FALSE;
		Tp_SendMessage(msg);
	}  
#else
	Tp_SendMessage(msg);
#endif
	
} 

/*****************************************************************************/
//  Function name:	TP_CfgInfo
//  Description:    This function will do the screen info init
//  global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_CfgInfo(void)
{
       LCD_INFO_T	lcd_info;	
	LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

	SCI_ASSERT(lcd_info.lcd_width&&lcd_info.lcd_height);/*assert verified*/
	
	s_tp_info_t.tp_width  	= lcd_info.lcd_width;//TP_WIDTH;  
	s_tp_info_t.tp_height 	= lcd_info.lcd_height+TP_HWICON_Y_LENGHT;//TP_HEIGHT;
	s_tp_info_t.tp_lcd_height = lcd_info.lcd_height;//TP_HEIGHT_LCD;                    
	s_tp_info_t.adc_min_x     = TP_ADC_MIN_X; 
	s_tp_info_t.adc_min_y     = TP_ADC_MIN_Y; 
	s_tp_info_t.adc_max_x     = TP_ADC_MAX_X; 
	s_tp_info_t.adc_max_y     = TP_ADC_MAX_Y;	
	s_tp_info_t.exchange_x	= EXCHANGE_X;
	s_tp_info_t.exchange_y	= EXCHANGE_Y;
	s_screen_x_start	      	= 0;
	s_screen_y_start	        = 0;
	s_screen_x_end		    = s_tp_info_t.tp_width;//TP_WIDTH;
	s_screen_y_end		    = s_tp_info_t.tp_height;//TP_HEIGHT;
}

/*****************************************************************************/
//  Function name:	TP_GetInfo
//  Description:    This function will get customer defined related screen infomation
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TP_INFO_T* TP_GetInfo(void)
{
	return &s_tp_info_t;
}

/*****************************************************************************/
//  Function name:	TP_GetCalInfo
//  Description:    This function will get calibration infomation
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC TP_CALIBRATION_T* TP_GetCalInfo(void)
{
	return &s_tp_cal;
}

/*****************************************************************************/
//  Function name:	TP_InitCalValue
//  Description:    This function set calibration parameters default values ,after calibrated this
//  value will be replaced by calibrated value.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_InitCalValue(TP_CALIBRATION_T* tp_calibration)
{
	uint32 tp_xfactor;
	uint32 tp_yfactor;
	LCD_INFO_T	lcd_info;	
	LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

	SCI_ASSERT(lcd_info.lcd_width&&lcd_info.lcd_height);/*assert verified*/
	
	tp_xfactor = TP_ADC_MAX_X - TP_ADC_MIN_X;
	tp_xfactor = tp_xfactor*TP_PRECISION;
	tp_xfactor = tp_xfactor/lcd_info.lcd_width;//TP_WIDTH;
	tp_yfactor = TP_ADC_MAX_Y - TP_ADC_MIN_Y;
	tp_yfactor = tp_yfactor*TP_PRECISION;
	tp_yfactor = tp_yfactor/(lcd_info.lcd_height+TP_HWICON_Y_LENGHT);//TP_HEIGHT;
	
	tp_calibration->x_top_factor  = tp_calibration->x_bottom_factor = tp_xfactor;
	tp_calibration->y_left_factor = tp_calibration->y_right_factor  = tp_yfactor;
	tp_calibration->x_min_top     = tp_calibration->x_min_bottom    = TP_ADC_MIN_X;
	tp_calibration->x_max_top     = tp_calibration->x_max_bottom    = TP_ADC_MAX_X;
	tp_calibration->y_min_left    = tp_calibration->y_min_right     = TP_ADC_MIN_Y;
	tp_calibration->y_max_left    = tp_calibration->y_max_right     = TP_ADC_MAX_Y;
	tp_calibration->x_center      = (TP_ADC_MIN_X + TP_ADC_MAX_X) / 2;
	tp_calibration->y_center      = (TP_ADC_MIN_Y + TP_ADC_MAX_Y) / 2;
	tp_calibration->x_min_up      =  TP_ADC_MIN_X;
	tp_calibration->y_min_up      =  TP_ADC_MIN_Y;
}	

/*****************************************************************************/
//  Function name:	TP_UpdateCalibration
//  Description:    refresh base data of touch panel ,set calibration flag SCI_TRUE.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  TP_UpdateCalibration(TP_CALIBRATION_T * tp_para)
{	
	if (	(tp_para->x_top_factor>TP_X_TOP_FACTOR_MAX) 
			|| (tp_para->y_left_factor < TP_PRECISION) 
				||(tp_para->y_left_factor > TP_Y_LEFT_FACTOR_MAX) ) 
	{	
		return SCI_FALSE;
	}

	if (	(tp_para->x_max_top > TP_ADC_MAX) 
			|| (tp_para->x_max_bottom > TP_ADC_MAX) 
				||(tp_para->y_max_left > TP_ADC_MAX) 
					||(tp_para->y_max_right > TP_ADC_MAX))
	{
		return SCI_FALSE;   
	}
	
	s_tp_cal = *tp_para;
	s_tp_cal.is_valid = CALIBRATION_FLAG;

	if(TP.cal_en) s_tp_drv_fun->tp_conf_cal_parameter();	
	
	return SCI_TRUE;
}

/*****************************************************************************/
//  Function name:	TP_SetRawDataMode
//  Description:    This function will set the flag whether in calibration mode or 
//  not
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void TP_SetRawDataMode(BOOLEAN act_res)
{
	if (!act_res)//if in calibration mode
	{
		s_is_rawdata = SCI_FALSE;
	}
	else
	{
		s_is_rawdata = SCI_TRUE;
	}
}

/*****************************************************************************/
//  Function name:	TP_GetRawDataMode
//  Description	 :  This function will get the flag whether in calibration mode or 
//  not 
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN  TP_GetRawDataMode(void)
{
	return s_is_rawdata ;
}

/*****************************************************************************/
//  Function name:	_TP_InitParam
//  Description:    This function init  tpc module parameter
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void  _TP_InitParam(void)
{
	TP.mcnt                = 0;
	TP.smooth_cnt      = 0;
	TP.state		       = TP_UNKNOWN_STATE;
	TP.cal_en	       = SCI_TRUE;
	TP.is_buff_full        = SCI_FALSE;
	TP.first_down_irq   = SCI_TRUE;
	TP.tbdState           = SCI_TRUE;
}

/*****************************************************************************/
//  Function name:	TP_Init
//  Description:    This function init  tpc module.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void  TP_Init(void)
{
	//TPDRV_PRINT:"TP_main: enter into TP_Init\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_485_112_2_18_1_5_52_178,(uint8*)"");
	/*TPC and Auxadc controller init*/
	s_tp_drv_fun = TP_GetDrvFunc();
	s_tp_drv_fun->tp_drv_init();
	/*register the callback function*/
	_TP_CbRegistration(_Tp_MsgSend);
	/*do the customer related info init*/
	_TP_CfgInfo();
	/*TPC module parameters init*/
	_TP_InitParam();

	#ifdef TP_RING_BUF_EN
	/*TPC ring buf init*/
	s_tp_buf_fun = TP_GetBufFunc();
	s_tp_buf_fun->tp_flush_data_buf();
	#endif
}

/*In order to speed filter arithmetic ,put move filter code into IRAM */
#pragma arm section code = "TP_Move_Filter"
/*****************************************************************************/
//  Function name:	_TP_MoveFilter
//  Description:    This function is pen move filter.This function will be used to eliminate large scale 
//  excursion.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:should carefully select threshold TP_FILTER_LIMIT value,if threshold too large will not 
// eliminate large scale excursion,if threshhold too small may wrongly discard the useful coord.
/*****************************************************************************/
PUBLIC TP_FILTER_RETURN_E _TP_MoveFilter(TP_COORD_T coord)
{
	uint16   tmpx = 0;
	uint16   tmpy = 0;
	uint16   dx   = 0;
	uint16   dy   = 0;
	uint16   dx_base =0;
	uint16   dy_base =0;
	static    uint16   out_count=0;
	
	uint16   count = 0;
	TP_FILTER_RETURN_E  retVal;

	if(TP.tbdState == TRUE)
	{
		TP.mtbdcnt++;
	} 
	  
	/*if count great than TP_FILTER_DATA_COUNT then will execute pen move filter*/
	if(++TP.mcnt > TP_FILTER_DATA_COUNT)
	{
		//TPDRV_PRINT("_TP_MoveF ilter: TP.mov_buf[0].x=%d, TP.mov_buf[0].y=%d.\n\r", TP.mov_buf[0].x, TP.mov_buf[0].y);
		//TPDRV_PRINT("_TP_MoveFilter: TP.mov_buf[1].x=%d, TP.mov_buf[1].y=%d.\n\r", TP.mov_buf[1].x, TP.mov_buf[1].y);
		//TPDRV_PRINT("_TP_MoveFilter: coord.x=%d, coord.y=%d.\n\r", coord.x, coord.y)

		TP.mcnt = TP_FILTER_DATA_COUNT;
		tmpx = (TP.mov_buf[0].x + coord.x)>>1;
		tmpy = (TP.mov_buf[0].y + coord.y)>>1;
		
		dx_base   = (TP.mov_buf[1].x > TP.mov_buf[0].x)? (TP.mov_buf[1].x - TP.mov_buf[0].x):(TP.mov_buf[0].x - TP.mov_buf[1].x);
		dy_base   = (TP.mov_buf[1].y > TP.mov_buf[0].y)? (TP.mov_buf[1].y - TP.mov_buf[0].y):(TP.mov_buf[0].y - TP.mov_buf[1].y);
		
		dx   = (TP.mov_buf[1].x > tmpx)? (TP.mov_buf[1].x - tmpx):(tmpx - TP.mov_buf[1].x);
		dy   = (TP.mov_buf[1].y > tmpy)? (TP.mov_buf[1].y - tmpy):(tmpy - TP.mov_buf[1].y);

		/*If dx or dy value greater then threshhold then current cord will be viewed as 
		invalid cord,this cord will be discarded, not stored*/
		if((dx*1000/(dx_base+DX_OFFSET) > TP_FILTER_LIMIT) ||(dy*1000/(dy_base+DX_OFFSET) > TP_FILTER_LIMIT))
		{
			//TPDRV_PRINT("_TP_MoveFilter: [x1,y1] = [%d,%d],[x2,y2] = [%d,%d],[x3,y3] = [%d,%d], ,count=%d\n\r", TP.mov_buf[0].x, TP.mov_buf[0].y, TP.mov_buf[1].x, TP.mov_buf[1].y,coord.x,coord.x,out_count++);
			TP.tbdState = SCI_TRUE;
			for(count=0;count<TP_FILTER_TBD_DATA_COUNT;count++)
			if(TP.mtbdcnt == TP_FILTER_TBD_DATA_COUNT)
			{
				TP.mcnt  = 0;
			}
			
			retVal    = TP_MOVE_EXCEED_LIMIT;
		} 

		/*If current cord withing threshold*/
		else
		{
			TP.mtbdcnt = 0;
			TP.tbdState = SCI_FALSE;
			TP.mov_buf[0].x = TP.mov_buf[1].x;
			TP.mov_buf[1].x = coord.x;
			TP.mov_buf[0].y = TP.mov_buf[1].y;
			TP.mov_buf[1].y = coord.y;
			retVal = TP_MOVE_SMOOTH;
		}
	}
	/*If count less than TP_FILTER_DATA_COUNT just store data.*/
	else
	{
		TP.tbdState = SCI_FALSE;
		TP.mtbdcnt =  0;
		TP.mov_buf[0].x = TP.mov_buf[1].x;
		TP.mov_buf[1].x = coord.x;
		TP.mov_buf[0].y = TP.mov_buf[1].y;
		TP.mov_buf[1].y = coord.y;
		retVal = TP_MOVE_PRE;
	}
	return retVal;
}
#pragma arm section code

/*****************************************************************************/
//  Function name:	_TP_FlushMovFilter
//  Description:    This function will flush the data of the mov filter buf
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_FlushMovFilter(void)
{
	uint16 i=0;
	
	for(i=0;i<TP_FILTER_DATA_COUNT;i++)
	{
		TP.mov_buf[i].x = 0;
		TP.mov_buf[i].y = 0;
	}
	TP.mcnt = 0;
	TP.tbdState = SCI_FALSE;
	TP.mtbdcnt =  0;
}

/*****************************************************************************/
//  Function name:	_TP_SmoothFilter
//  Description:    This function is pen smooth filter.This function will be used to eliminate small
//  scale excursion. 
//  Global resource dependence: 
//  Author: tao.feng
//  Note:should carefully select TP_SMOOTH_DEPTH,if TP_SMOOTH_DEPTH is too high will cause
//  line distorction,if TP_SMOOTH_DEPTH is too small will not eliminate small scale excursion
/*****************************************************************************/
LOCAL TP_COORD_T _TP_SmoothFilter(TP_COORD_T coord)
{
	uint16 	   i = 0;
	TP_COORD_T value ={0};
	
	/*if count small than TP_SMOOTH_DEPTH then just store data*/
	if(TP.smooth_cnt < TP_SMOOTH_DEPTH)
	{
		TP.smooth_buf[TP.smooth_cnt].x = coord.x;
		TP.smooth_buf[TP.smooth_cnt].y = coord.y;
		TP.smooth_cnt++;
		value.x = coord.x;
		value.y = coord.y;
	}
	/*if count great than TP_SMOOTH_DEPTH then will execute smooth filter*/
	else
	{
		value.x = TP.smooth_buf[0].x;
		value.y = TP.smooth_buf[0].y;
		for(i=1;i<TP_SMOOTH_DEPTH;i++)
		{
			value.x += TP.smooth_buf[i].x;
			value.y += TP.smooth_buf[i].y;
			TP.smooth_buf[i-1].x = TP.smooth_buf[i].x;
			TP.smooth_buf[i-1].y = TP.smooth_buf[i].y;
		}
		TP.smooth_buf[TP_SMOOTH_DEPTH-1].x = coord.x;
		TP.smooth_buf[TP_SMOOTH_DEPTH-1].y = coord.y;
		value.x = (value.x + coord.x) / (TP_SMOOTH_DEPTH+1);
		value.y = (value.y + coord.y) / (TP_SMOOTH_DEPTH+1);
		TP.smooth_cnt = TP_SMOOTH_DEPTH;
	}
	return value;
}

/*****************************************************************************/
//  Function name:	_TP_FlushSmoothFilter
//  Description:    This function will flush the data of the coordbuf
//  Global resource dependence: 
//  Author: tao.feng
//  Note:usually will be called when pen is up
/*****************************************************************************/
LOCAL void _TP_FlushSmoothFilter(void)
{
	uint16 i=0;
	
	for(i=0;i<TP_SMOOTH_DEPTH;i++)
	{
		TP.smooth_buf[i].x = 0;
		TP.smooth_buf[i].y = 0;
	}
	TP.smooth_cnt=0;
}

/*****************************************************************************/
//  Function name:	TP_IsMoving
//  Description:    This function will judge if the pen is moving
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN _TP_IsMoving(TP_COORD_T cur_coord, TP_COORD_T pre_coord)
{
	int delta_x;
	int delta_y;
	
	delta_x = cur_coord.x - pre_coord.x;
	delta_x = (delta_x > 0) ? delta_x : (- delta_x);
	delta_y = cur_coord.y - pre_coord.y;
	delta_y = (delta_y > 0) ? delta_y : (- delta_y);
    
	if ((TP_MOVE_MIN < (delta_x+delta_y)) && ((delta_x+delta_y) < TP_MOVE_MAX))
	{
		return SCI_TRUE;
	}

	//TPDRV_PRINT("_TP_IsMoving: delta_x+delta_y = %d.\n\r", delta_x+delta_y);

	return SCI_FALSE;
}
  
/*****************************************************************************/
//  Function name:	_TP_IsCoordValid
//  Description:    This function used to verify whether coord is valid
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN _TP_IsCoordValid(TP_COORD_T coord)
{
	if((s_screen_x_start <= coord.x)&&(coord.x <= s_screen_x_end)&&
		(s_screen_y_start <= coord.y)&&(coord.y <= s_screen_y_end))
	{
		return SCI_TRUE;  
	}
	else
	{   
		if(coord.x < s_screen_x_start)
		{
			coord.x = s_screen_x_start;
		}
		else if (coord.x > s_screen_x_end)
		{
			coord.x = s_screen_x_end;
		}
		if(coord.y < s_screen_y_start)
		{
			coord.y = s_screen_y_start;
		}
		else if (coord.y > s_screen_y_end)
		{
			coord.y = s_screen_y_end;     
		}
		return SCI_FALSE;  		
	}
}

/*****************************************************************************/
//  Function name:	_TP_IsADCValid
//  Description:    This function used to verify whether coord is valid
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN _TP_IsADCValid(TP_DATA_TYPE_T adc)
{
	
	if((adc.x < s_tp_cal.x_max_top) && (adc.x >= s_tp_cal.x_min_up)\
             && (adc.y < s_tp_cal.y_max_left) && (adc.y >= s_tp_cal.y_min_up)) 
	{
		return SCI_TRUE;
	}
	else
	{
		return SCI_FALSE;
	}
}

/*****************************************************************************/
//  Function name:	_TP_ADCToCoord
//  Description:    This function will convert adc value to coordinate value
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL __inline BOOLEAN _TP_ADCToCoord(TP_ADC_T *adc, TP_COORD_T *coor_ptr)
{
	LCD_INFO_T	lcd_info;
	int16 tp_width,tp_height;
	
	LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

	SCI_ASSERT(lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/

	tp_width  = lcd_info.lcd_width;
	tp_height =	(lcd_info.lcd_height + TP_HWICON_Y_LENGHT);

	/*Verify whether pen is down*/
	if ((adc->x_adc >= s_tp_cal.x_min_up) && (adc->y_adc  >= s_tp_cal.y_min_up))     
	{  
		int16 x, y;

		if (adc->x_adc> s_tp_cal.x_center)      //top
		{
			if(EXCHANGE_X)    /*lint !e506 !e774 */
			{
				adc->x_adc = s_tp_cal.x_center - (adc->x_adc - s_tp_cal.x_center); 
			}
			x = tp_width - (int32)(adc->x_adc - s_tp_cal.x_min_top) * TP_PRECISION / s_tp_cal.x_top_factor;
		}
		else        
		{
			if(EXCHANGE_X)    /*lint !e506 !e774 */
			{
				adc->x_adc = s_tp_cal.x_center - adc->x_adc + s_tp_cal.x_center; 
			}
			x = tp_width - (int32)(adc->x_adc - s_tp_cal.x_min_bottom) * TP_PRECISION / s_tp_cal.x_bottom_factor;
		}    
		x = x > tp_width ? tp_width : x;
		x = x < 0 ? 0 : x;

		if (adc->y_adc> s_tp_cal.y_center)      //left
		{
			if(EXCHANGE_Y)    /*lint !e506 !e774 */
			{
				adc->y_adc = s_tp_cal.y_center - (adc->y_adc - s_tp_cal.y_center) ;
			}
			y = tp_height - (int32)(adc->y_adc - s_tp_cal.y_min_left) * TP_PRECISION / s_tp_cal.y_left_factor;
		}
		else
		{
			if(EXCHANGE_Y)     /*lint !e506 !e774 */
			{
				adc->y_adc = s_tp_cal.y_center - adc->y_adc + s_tp_cal.y_center;
			}
			y = tp_height - (int32)(adc->y_adc- s_tp_cal.y_min_right) * TP_PRECISION / s_tp_cal.y_right_factor;
		}
		y = y > tp_height ? tp_height : y;
		y = y < 0 ? 0 : y;   
	    
	    	(*coor_ptr).x = x;  
		(*coor_ptr).y = y;
		
		return SCI_TRUE;
	}
	
	//TPDRV_PRINT:"TP_ADC_To_Coord: TP_Calculate, pen is UP.\n\r"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_804_112_2_18_1_5_52_179,(uint8*)"");
	return SCI_FALSE;
}

#ifdef TP_RING_BUF_EN
/*****************************************************************************/
//  Function name:	_TP_PopMsg
//  Description:   This function is to get event from ring buffer.
//  Author: tao.feng
//  Note:For mmi use
/*****************************************************************************/
PUBLIC BOOLEAN _TP_PopMsg(TP_MSG_T *touch_data)
{
	BOOLEAN 	      result;
	uint8 		      hign_data =0;
	uint8 		      low_data  =0;
	uint8  		      diff;
	uint16 		      avail	     =0;
	TP_BUF_FUNC_T     *buf_info =NULL;
	
	s_tp_drv_fun->tp_irq_mask(TP_IRQ_MASK_ALL);
	buf_info=s_tp_buf_fun->_tp_get_ring_buf_info();
	SCI_ASSERT(PNULL!=buf_info);/*assert verified*/
	/*If has data in ring buf,pop the msg from the buf*/
	if(buf_info.touch_buffer_rindex != buf_info.touch_buffer_windex)   	
	{            
		s_tp_buf_fun->tp_pop_data_from_buf(touch_data);     
		result = SCI_TRUE;
	}
	/*If buf is empty,set TP.msg_could_send=SCI_TRUE to enable msg send*/
	else
	{
		TP.msg_could_send = SCI_TRUE;
		result = SCI_FALSE;
	}      
	s_tp_drv_fun->tp_irq_en();
	return result;      
}   

/*****************************************************************************/
//  Function name:	_TP_PopMsg
//  Description:   This function is to push message to ring buffer.
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_PushMsg(TP_MSG_T *msg)
{
	uint16 roomleft=0;
	
	s_tp_drv_fun->tp_irq_mask(TP_IRQ_MASK_ALL);
	/*To get minimum roomleft*/
	roomleft = s_tp_buf_fun->tp_get_buf_roomleft();
	if(roomleft<=TP_BASIC_EVENT_UNIT)
	{
		TP.is_buff_full=SCI_TRUE;
	} 
	/*If ring bu is full,call sleep function until ring buf has enough available space*/
	if(TP.is_buff_full == SCI_TRUE)
	{
		do
		{
			SCI_Sleep(TP_BUF_FULL_SLEEP_TIME); 
		}
		while(s_tp_buf_fun->tp_get_buf_roomleft()  < TP_BASIC_EVENT_UNIT);
		TP.is_buff_full = SCI_FALSE;
	}
	/*Has enough buf space,puch msg to ring buf*/
	s_tp_buf_fun->tp_push_data_to_buf(msg);
	s_tp_drv_fun->tp_irq_en();
	
}  

/*****************************************************************************/
//  Function name:	_TP_FlushRingBuf
//  Description:   This function is to flush all data in ring buffer.
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_FlushRingBuf(void)
{
	s_tp_drv_fun->tp_irq_mask(TP_IRQ_MASK_ALL);
	s_tp_buf_fun->tp_flush_data_buf();
	TP.is_buff_full = SCI_FALSE;
	s_tp_drv_fun->tp_irq_en();
}
#endif

#ifdef TP_PENDOWN_ENHANCE
#ifndef TP_COMPATIBLE_EN
#error "TP_PENDOWN_ENHANCE's feature is based on TP_COMPATIBLE_EN"
#endif //TP_COMPATIBLE_EN
#endif //TP_PENDOWN_ENHANCE

#if defined(TP_COMPATIBLE_EN) && defined(TP_PENDOWN_ENHANCE)
#include "sc6600l_reg_tpc.h"
#include "deep_sleep.h"

LOCAL uint32 delay = 0x45;
LOCAL TP_REG_T * s_tp_regs = (TP_REG_T*)TPC_REG_BASE;
#endif //TP_COMPATIBLE_EN & TP_PENDOWN_ENHANCE

#ifdef TP_COMPATIBLE_EN
LOCAL BOOLEAN is_sample_x = SCI_TRUE;
#endif //TP_COMPATIBLE_EN
/*****************************************************************************/
//  Function name:	_TP_DownIrqHandler
//  Description:    This function is pen down interrupt service routine.
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_DownIrqHandler(void)
{
#if defined(TP_COMPATIBLE_EN) && defined(TP_PENDOWN_ENHANCE)
	TP_DATA_TYPE_T  *tp_data_buf = NULL;
	TP_INFO 			*tp_info     = NULL;
#endif //TP_COMPATIBLE_EN & TP_PENDOWN_ENHANCE
	
	if(TP_GetRawDataMode())
	{
		s_tp_drv_fun->_tp_drv_clac_disable();
	}

#if defined(TP_COMPATIBLE_EN) && defined(TP_PENDOWN_ENHANCE)
	tp_info    = s_tp_drv_fun->tp_drv_get_info();	
	do
	{
		SCI_Sleep(10);      //deshaking time
		s_tp_drv_fun->tp_drv_start_scan();
		s_tp_drv_fun->tp_irq_mask(TP_DONE_IRQ_MASK);
		//Wait the tpc sample x point ready.
		while(!(*(volatile uint32*)0x87003024 & 0x4));
		//while(!((INT_VAL & TPC_DONE_IRQ_STATUS) == TPC_DONE_IRQ_STATUS));
		s_tp_drv_fun->tp_drv_get_data(SCI_FALSE);
		tp_data_buf = s_tp_drv_fun->_tp_get_data_buf();
		s_tp_regs->TPC_INT_CLR |= TPC_DONE_IRQ_CLR;
		//TPDRV_PRINT:"_TP_DownIrqHandler: adc_x = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_931_112_2_18_1_5_53_180,(uint8*)"d", tp_data_buf[tp_info->buf_len-1].y);
	}while(tp_data_buf[tp_info->buf_len-1].y > delay && (0 == GPIO_GetValue(TP_GPIO_XL)));

	//SCI_Sleep(40);      //deshaking time

	if(1 == GPIO_GetValue(TP_GPIO_XL))		
	{
		//The pen is up
		s_tp_drv_fun->_tp_clr_irq_status();
		GPIO_EnableIntCtl(TP_GPIO_XL);
		SCI_TPC_EnableDeepSleep(ENABLE_APB_SLEEP);	
		return;
	}
#endif //TP_COMPATIBLE_EN & TP_PENDOWN_ENHANCE
	
#ifdef TP_COMPATIBLE_EN
	GPIO_SetDirection(TP_GPIO_XL, SCI_TRUE);
	GPIO_SetDirection(TP_GPIO_XR, SCI_TRUE);
	GPIO_SetDirection(TP_GPIO_YU, SCI_FALSE);
	GPIO_SetDirection(TP_GPIO_YD, SCI_FALSE);

	GPIO_SetValue(TP_GPIO_XL, SCI_TRUE);
	GPIO_SetValue(TP_GPIO_XR, SCI_FALSE);
	is_sample_x = SCI_TRUE;
#endif //TP_COMPATIBLE_EN
	
	s_tp_drv_fun->tp_irq_mask(TP_DOWN_IRQ_MASK);
	s_tp_drv_fun->tp_drv_start_scan();
	TP.state 		   = TP_PEN_DOWN;
	TP.first_down_irq = SCI_TRUE;
	//TPDRV_PRINT:"TP  DOWN IRQENTER :time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_961_112_2_18_1_5_53_181,(uint8*)"d",SCI_GetTickCount());
	s_tp_drv_fun->tp_irq_en();
}

/*****************************************************************************/
//  Function name:	_TP_UpIrqHandler
//  Description:    This function is pen up interrupt service routine.
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_UpIrqHandler(void)
{
	/*If pen up,stop the tpc and auxadc driver,flush the buf 
	and send the previous coord to upper layer*/
	TP_MSG_T tp_msg;

	if(TP.cal_en) 
	{
		s_tp_drv_fun->_tp_drv_clac_enable();
	}	
#ifdef TP_COMPATIBLE_EN
	GPIO_SetDirection(TP_GPIO_YD, SCI_TRUE);
	GPIO_SetValue(TP_GPIO_YD, SCI_FALSE);    
	GPIO_SetDirection(TP_GPIO_XL, SCI_FALSE);
	GPIO_SetDirection(TP_GPIO_XR, SCI_FALSE);
	GPIO_SetDirection(TP_GPIO_YU, SCI_FALSE);
#endif //TP_COMPATIBLE_EN
		
	s_tp_drv_fun->tp_irq_mask(TP_UP_IRQ_MASK);
	TP.state = TP_PEN_UP;
	//TPDRV_PRINT:"TP UP IRQ ENTER"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_989_112_2_18_1_5_53_182,(uint8*)"");
	s_tp_drv_fun->tp_drv_stop();
	_TP_FlushMovFilter();
	_TP_FlushSmoothFilter();
	tp_msg.data.x = TP.pre.x;
	tp_msg.data.y = TP.pre.y;
	tp_msg.event  = TP_UP_MSG;
	#ifdef TP_RING_BUF_EN
		_TP_PushMsg(tp_msg);
	#else
		TP.tp_event_cb(tp_msg);
	#endif
	s_tp_drv_fun->_tp_clr_irq_status();
	s_tp_drv_fun->tp_irq_en();
}

#ifdef TP_SELFADAPTIVE_ENABLE
//@vine.yuan 2010.1.5
typedef void (*TP_ADAPT_FUN_T)(TP_DATA_TYPE_T *, uint8, BOOLEAN);

/*****************************************************************************/
//  Function name:	_tp_mode_normal
//  Description: do nothing, reserved for uniformness
//  Global resource dependence:
//  Author: Vine.Yuan
//  Note:
/*****************************************************************************/
void _tp_mode_normal(
    TP_DATA_TYPE_T * tp_data_buf, //Pointer to TP's value buffer
    uint8 buf_len, //Length of TP's value buffer
    BOOLEAN cal_en //Indication of whether calculation is enabled.
    )
{
    //Here, do nothing
    return;
}

/*****************************************************************************/
//  Function name:	_tp_mode_xw
//  Description: inverse the value of x. 
//  Global resource dependence: s_tp_cal
//  Author: Vine.Yuan
//  Note:
/*****************************************************************************/
void _tp_mode_xw(
    TP_DATA_TYPE_T * tp_data_buf, //Pointer to TP's value buffer
    uint8 buf_len, //Length of TP's value buffer
    BOOLEAN cal_en //Indication of whether calculation is enabled.
    )
{
    LCD_INFO_T	lcd_info;
    int16 tp_width,tp_height;
    uint8 i;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    SCI_ASSERT(lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/
    tp_width  = lcd_info.lcd_width;
    tp_height =	(lcd_info.lcd_height+25);

    for (i=0; i<buf_len;i++)
    {
        //To inverse the value of x in coordinate
        if (cal_en)
        {
            tp_data_buf[i].x = tp_width - tp_data_buf[i].x;
        }
        else
        {
            tp_data_buf[i].x = s_tp_cal.x_max_top - tp_data_buf[i].x;
        }
    } 
}

/*****************************************************************************/
//  Function name:	_tp_mode_yw
//  Description: inverse the value of y. 
//  Global resource dependence: s_tp_cal
//  Author: Vine.Yuan
//  Note:
/*****************************************************************************/
void _tp_mode_yw(
    TP_DATA_TYPE_T * tp_data_buf, //Pointer to TP's value buffer
    uint8 buf_len, //Length of TP's value buffer
    BOOLEAN cal_en //Indication of whether calculation is enabled.
    )
{
    LCD_INFO_T	lcd_info;
    int16 tp_width,tp_height;
    uint8 i;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    SCI_ASSERT(lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/
    tp_width  = lcd_info.lcd_width;
    tp_height =	(lcd_info.lcd_height+25);

    for (i=0; i<buf_len;i++)
    {
        //To inverse the value of y in coordinate
        if (cal_en)
        {
            tp_data_buf[i].y = tp_height - tp_data_buf[i].y;
        }
        else
        {
            tp_data_buf[i].y = s_tp_cal.y_max_left - tp_data_buf[i].y;
        }
    } 
}

/*****************************************************************************/
//  Function name:	_tp_mode_xyw
//  Description: inverse x and y,respectively. 
//  Global resource dependence: s_tp_cal
//  Author: Vine.Yuan
//  Note:
/*****************************************************************************/
void _tp_mode_xyw(
    TP_DATA_TYPE_T * tp_data_buf, //Pointer to TP's value buffer
    uint8 buf_len, //Length of TP's value buffer
    BOOLEAN cal_en //Indication of whether calculation is enabled.
    )
{
    LCD_INFO_T	lcd_info;
    int16 tp_width,tp_height;
    uint8 i;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    SCI_ASSERT(lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/
    tp_width  = lcd_info.lcd_width;
    tp_height =	(lcd_info.lcd_height+25);

    for (i=0; i<buf_len;i++)
    {
        //To inverse the values of x and y, respectively in coordinate
        if (cal_en)
        {
            tp_data_buf[i].x = tp_width - tp_data_buf[i].x;
            tp_data_buf[i].y = tp_height - tp_data_buf[i].y;
        }
        else
        {
            tp_data_buf[i].x = s_tp_cal.x_max_top - tp_data_buf[i].x;
            tp_data_buf[i].y = s_tp_cal.y_max_left - tp_data_buf[i].y;
        }
    } 
}

/*****************************************************************************/
//  Function name:	_tp_mode_xye
//  Description:swap the values between x and y.
//  Global resource dependence: s_tp_cal
//  Author: Vine.Yuan
//  Note:
/*****************************************************************************/
void _tp_mode_xye(
    TP_DATA_TYPE_T * tp_data_buf, //Pointer to TP's value buffer
    uint8 buf_len, //Length of TP's value buffer
    BOOLEAN cal_en //Indication of whether calculation is enabled.
    )
{
    uint8 i;
    for (i=0; i<buf_len;i++)
    {
        uint16 temp;

        //To swap the value between x and y in coordinate
        temp = tp_data_buf[i].x;
        tp_data_buf[i].x = tp_data_buf[i].y;
        tp_data_buf[i].y = temp;
    }
}

/*****************************************************************************/
//  Function name:	_tp_mode_xye_xw
//  Description:swap the values between x and y, and then inverse y.
//  Global resource dependence: s_tp_cal
//  Author: Vine.Yuan
//  Note:
/*****************************************************************************/
void _tp_mode_xye_xw(
    TP_DATA_TYPE_T * tp_data_buf, //Pointer to TP's value buffer
    uint8 buf_len, //Length of TP's value buffer
    BOOLEAN cal_en //Indication of whether calculation is enabled.
    )
{
    LCD_INFO_T	lcd_info;
    int16 tp_width,tp_height;
    uint8 i;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    SCI_ASSERT(lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/
    tp_width  = lcd_info.lcd_width;
    tp_height =	(lcd_info.lcd_height+25);

    for (i=0; i<buf_len;i++)
    {
        uint16 temp;
        
        //To swap the value between x and y
        temp = tp_data_buf[i].x;
        tp_data_buf[i].x = tp_data_buf[i].y;
        tp_data_buf[i].y = temp;

        //To inverse the value of y in coordinate
        if (cal_en)
        {
            tp_data_buf[i].y = tp_height - tp_data_buf[i].y;
        }
        else
        {
            tp_data_buf[i].y = s_tp_cal.y_max_left - tp_data_buf[i].y;
        }
    } 
}

/*****************************************************************************/
//  Function name:	_tp_mode_xye_yw
//  Description:swap the values between x and y, and then inverse x.
//  Global resource dependence: s_tp_cal
//  Author: Vine.Yuan
//  Note:
/*****************************************************************************/
void _tp_mode_xye_yw(
    TP_DATA_TYPE_T * tp_data_buf, //Pointer to TP's value buffer
    uint8 buf_len, //Length of TP's value buffer
    BOOLEAN cal_en //Indication of whether calculation is enabled.
    )
{
    LCD_INFO_T	lcd_info;
    int16 tp_width,tp_height;
    uint8 i;

    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    SCI_ASSERT(lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/
    tp_width  = lcd_info.lcd_width;
    tp_height =	(lcd_info.lcd_height+25);

    for (i=0; i<buf_len;i++)
    {
        uint16 temp;
        
        //To swap the value between x and y
        temp = tp_data_buf[i].x;
        tp_data_buf[i].x = tp_data_buf[i].y;
        tp_data_buf[i].y = temp;

        //To inverse the value of x in coordinate
        if (cal_en)
        {
            tp_data_buf[i].x = tp_width - tp_data_buf[i].x;
        }
        else
        {
            tp_data_buf[i].x = s_tp_cal.x_max_top - tp_data_buf[i].x;
        }
    } 
}

/*****************************************************************************/
//  Function name:	_tp_mode_xye_xyw
//  Description:swap the values between x and y, and then inverse x and y, 
//              respectively. 
//  Global resource dependence: s_tp_cal
//  Author: Vine.Yuan
//  Note:
/*****************************************************************************/
void _tp_mode_xye_xyw(
    TP_DATA_TYPE_T * tp_data_buf, //Pointer to TP's value buffer
    uint8 buf_len, //Length of TP's value buffer
    BOOLEAN cal_en //Indication of whether calculation is enabled.
    )
{
	LCD_INFO_T	lcd_info;
	int16 tp_width,tp_height;
	uint8 i;
	
	LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

	SCI_ASSERT(lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/
	tp_width  = lcd_info.lcd_width;
	tp_height =	(lcd_info.lcd_height+25);
	
	for (i=0; i<buf_len;i++)
	{
		uint16 temp;
		//To swap the value between x and y
		temp = tp_data_buf[i].x;
		tp_data_buf[i].x = tp_data_buf[i].y;
		tp_data_buf[i].y = temp;

         //To inverse the values of x and y, respectively in coordinate
		if (cal_en)
		{
			tp_data_buf[i].x = tp_width - tp_data_buf[i].x;
			tp_data_buf[i].y = tp_height - tp_data_buf[i].y;
		}
		else
		{
			tp_data_buf[i].x = s_tp_cal.x_max_top - tp_data_buf[i].x;
			tp_data_buf[i].y = s_tp_cal.y_max_left - tp_data_buf[i].y;
		}
	} 
}

/*
* To initialize the pointer array to function for translation based on 
* the information of TP's self-adpative mode, @vine.yuan 2010.1.5
*/
TP_ADAPT_FUN_T tp_adapt_fun[TP_MODE_MAX]=
	{
		_tp_mode_normal,
		_tp_mode_xw,
		_tp_mode_yw,
		_tp_mode_xyw,
		_tp_mode_xye,
		_tp_mode_xye_xw,
		_tp_mode_xye_yw,
		_tp_mode_xye_xyw
	};
#endif //TP_SELFADAPTIVE_ENABLE

/*****************************************************************************/
//  Function name:	_TP_DoneIrqHandler
//  Description:    This function is done interrupt service routine.
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void _TP_DoneIrqHandler(void)
{
	LCD_INFO_T	lcd_info;
	int16 tp_width,tp_height;	
	BOOLEAN 		dmfv = SCI_FALSE;
	BOOLEAN 		is_moving = SCI_FALSE;
	TP_MSG_T 		tp_msg;
	TP_INFO 			*tp_info     = NULL;
	TP_DATA_TYPE_T  *tp_data_buf = NULL;
	uint32                    xl_state =0;

#ifdef TP_SELFADAPTIVE_ENABLE
    uint32          adapt_mode;
#endif //TP_SELFADAPTIVE_ENABLE

	LCD_GetInfo(MAIN_LCD_ID, &lcd_info);
    
	SCI_ASSERT(lcd_info.lcd_width && lcd_info.lcd_height);/*assert verified*/

	tp_width  = lcd_info.lcd_width;
	tp_height =	(lcd_info.lcd_height + TP_HWICON_Y_LENGHT);

	TP.state  = TP_PEN_DONE;
	tp_info    = s_tp_drv_fun->tp_drv_get_info();	
#ifdef TP_COMPATIBLE_EN
	/* sleep 1 ms */
	//SCI_SLEEP(1);

	if(s_tp_drv_fun->_tp_detect_up())
	{
		_TP_UpIrqHandler();
		is_sample_x = SCI_TRUE;
		return;
	}    

	if( is_sample_x)
	{        
		GPIO_SetDirection(TP_GPIO_XL, SCI_FALSE);
		GPIO_SetDirection(TP_GPIO_YD, SCI_TRUE);
		GPIO_SetValue(TP_GPIO_YD, SCI_FALSE);
		GPIO_SetDirection(TP_GPIO_XR, SCI_FALSE);
		GPIO_SetDirection(TP_GPIO_YU, SCI_TRUE);
		GPIO_SetValue(TP_GPIO_YU, SCI_TRUE);

		s_tp_drv_fun->tp_drv_get_data(is_sample_x);
		tp_data_buf = s_tp_drv_fun->_tp_get_data_buf();
		is_sample_x = SCI_FALSE;                

		return;            
	}
	else
	{
		GPIO_SetDirection(TP_GPIO_XL, SCI_TRUE);
		GPIO_SetDirection(TP_GPIO_XR, SCI_TRUE);
		GPIO_SetDirection(TP_GPIO_YU, SCI_FALSE);
		GPIO_SetDirection(TP_GPIO_YD, SCI_FALSE);

		GPIO_SetValue(TP_GPIO_XL, SCI_TRUE);
		GPIO_SetValue(TP_GPIO_XR, SCI_FALSE);

		s_tp_drv_fun->tp_drv_get_data(is_sample_x);
		tp_data_buf = s_tp_drv_fun->_tp_get_data_buf();            
		is_sample_x = SCI_TRUE;
	}
#else //TP_COMPATIBLE_EN


	s_tp_drv_fun->_tp_set_adc_range(ADC_SMALL_RANGE); 

	/* sleep 1 ms */
	SCI_SLEEP(1);

	if(s_tp_drv_fun->_tp_detect_up())
	{
		_TP_UpIrqHandler();
		return;
	}

	s_tp_drv_fun->_tp_set_adc_range(ADC_LARGE_RANGE); 
	s_tp_drv_fun->tp_drv_get_data();
	
	tp_data_buf = s_tp_drv_fun->_tp_get_data_buf();
#endif //TP_COMPATIBLE_EN

	if(TP_GetRawDataMode())
	{
		tp_msg.data.x  = tp_data_buf[tp_info->buf_len-1].x;
		tp_msg.data.y  = tp_data_buf[tp_info->buf_len-1].y;
		tp_msg.event   = TP_DOWN_MSG;
        /*
        * Here, fix a bug, only if previous point is located in shortcut bar,
        * ant then, when TP calibration is proceeding, it will show whether 
        * or not to exit.@vine.yuan 2010.1.7
        */
        TP.pre.x = tp_msg.data.x;
        TP.pre.y = tp_msg.data.y;

		TP.tp_event_cb(tp_msg);

		//TPDRV_PRINT:"Coordinate: x=%d, y=%d.\n\r"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_1401_112_2_18_1_5_54_183,(uint8*)"dd", tp_msg.data.x, tp_msg.data.y);

		s_tp_drv_fun->tp_irq_en();
		return;
	}	

#ifdef TP_SELFADAPTIVE_ENABLE
	/*
	* To get the current self-adaptive mode. @vine.yuan 2010.6.3
	*/
	adapt_mode = TP_GetCurAdaptMode();

    //TPDRV_PRINT:"Self-Adaptive Mode:%d\r\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_1413_112_2_18_1_5_54_184,(uint8*)"d",adapt_mode);
        
    /*
    * For the purpose of self-adaptive feature, add the following extra-code 
    * to translate TP's values. @vine.yuan 2010.1.5
    */
	tp_adapt_fun[adapt_mode](tp_data_buf, 
		tp_info->buf_len, tp_info->cal_info.cal_en);
#endif //TP_SELFADAPTIVE_ENABLE

	/*If read adc value through TPC_BUF_DATA reg*/
	if(!tp_info->cal_info.cal_en)
	{
		/*verify if the captured adc value valid*/
		if(_TP_IsADCValid(tp_data_buf[0])||\
			_TP_IsADCValid(tp_data_buf[tp_info->buf_len-1]))
		{
			if(!_TP_ADCToCoord((TP_ADC_T*)&tp_data_buf[0], &TP.pre)\
				||!_TP_ADCToCoord((TP_ADC_T*)&tp_data_buf[tp_info->buf_len-1], &TP.cur))
			{
				s_tp_drv_fun->tp_irq_en();
				return;
			}
		}
		/*captured adc value invalid*/
		else
		{		
			s_tp_drv_fun->tp_irq_en();
			return;
		}
	}
	/*If read coord value through TPC_BUF_DATA reg*/
	else
	{
		TP.cur.x = tp_width - tp_data_buf[tp_info->buf_len-1].x;
		TP.cur.y = tp_height - tp_data_buf[tp_info->buf_len-1].y;

#ifndef TP_SELFADAPTIVE_ENABLE
		if(EXCHANGE_X)/*lint !e506 !e774*/
		{
			TP.cur.x = tp_data_buf[tp_info->buf_len-1].x;
			TP.cur.x = TP.cur.x > tp_width ? tp_width : TP.cur.x;
			TP.cur.x = TP.cur.x < 0 ? 0 : TP.cur.x;
		}

		if(EXCHANGE_Y)/*lint !e506 !e774*/
		{
			TP.cur.y = tp_data_buf[tp_info->buf_len-1].y - TP_HWICON_Y_LENGHT;
			TP.cur.y = TP.cur.y > tp_height ? tp_height : TP.cur.y;
			TP.cur.y = TP.cur.y < 0 ? 0 : TP.cur.y;
		}
#endif //TP_SELFADAPTIVE_ENABLE		
	}
	if(!_TP_IsCoordValid(TP.cur))
	{
		//TPDRV_PRINT:"TP_COORD_INVALID: Coordinate: x=%d, y=%d.\n\r"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_1468_112_2_18_1_5_54_185,(uint8*)"dd", TP.cur.x, TP.cur.y);
		s_tp_drv_fun->tp_irq_en();
		return;
	}
	/*If captured data valid*/
	else
	{
		/*If first enter done irq*/
		if(TP.first_down_irq)
		{
			TP.first_down_irq = SCI_FALSE;
			TP.pre.x = TP.cur.x;
			TP.pre.y = TP.cur.y;
			tp_msg.data.x      = TP.pre.x;
			tp_msg.data.y      = TP.pre.y;
			TP.mov_buf[0].x =  TP.pre.x;
			TP.mov_buf[1].x =  TP.pre.x;
			TP.mov_buf[0].y =  TP.pre.y;
			TP.mov_buf[1].y =  TP.pre.y;
			TP.tbdState        =   SCI_FALSE;
			TP.mtbdcnt         =   0;
			TP.mcnt             =   TP_FILTER_DATA_COUNT;
			tp_msg.event      =   TP_DOWN_MSG; 
			
			#ifdef TP_RING_BUF_EN
				_TP_PushMsg(tp_msg);
			#else
				TP.tp_event_cb(tp_msg);
			#endif

			//TPDRV_PRINT:"TP_FIRSET_DOWN: Coordinate: TP.cur.x=%d, TP.cur.y=%d.\n\r"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_1498_112_2_18_1_5_54_186,(uint8*)"dd", TP.cur.x, TP.cur.y);
		}
		/*if  pen is moving then will call move filter to elimate large scale excursion,call smooth filter to elimanate small
		 scale excursion after do all of this will push data to ring buffer and send related msg to upper layer task.*/
		else
		{
			is_moving = _TP_IsMoving(TP.cur, TP.pre);
			if(is_moving)
			{
				dmfv = _TP_MoveFilter(TP.cur);
				if(TP_MOVE_SMOOTH == dmfv)
				{
					TP.cur = _TP_SmoothFilter(TP.cur);
				}
				else if(TP_MOVE_EXCEED_LIMIT == dmfv)
				{
					return;
				}
				else if(TP_MOVE_PRE == dmfv)
				{
					TP.cur.x = TP.mov_buf[0].x;
					TP.cur.y = TP.mov_buf[0].y;
				}
				tp_msg.data.x = TP.cur.x;
				tp_msg.data.y = TP.cur.y;
				tp_msg.event  = TP_MOVE_MSG;
				//TPDRV_PRINT("_TP_PushMsg:TP.cur.x=%d, TP.cur.y=%d.\n\r", TP.cur.x, TP.cur.y);		

				TP.pre.x = TP.cur.x;
				TP.pre.y = TP.cur.y;
				
				#ifdef TP_RING_BUF_EN
				_TP_PushMsg(tp_msg);
				#else
				TP.tp_event_cb(tp_msg);
				#endif
			}
			else
			{
				TP.pre.x = TP.cur.x;
				TP.pre.y = TP.cur.y;
				
				//TPDRV_PRINT:"TP_Not_Moving:TP.cur.x=%d, TP.cur.y=%d,TP.pre.x=%d,TP.pre.y=%d,"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,TP_SRV_V0_1541_112_2_18_1_5_54_187,(uint8*)"dddd", TP.cur.x, TP.cur.y, TP.pre.x, TP.pre.y);
				s_tp_drv_fun->tp_irq_en();

				return;
			}
		}
	}
}

/*****************************************************************************/
//  Function name:	TP_Enable
//  Description;
//  Global resource dependence: 
//  Author: yi.qiu
//  Note:
/*****************************************************************************/
PUBLIC void TP_Enable()
{
	s_tp_drv_fun->_tp_enable();
}

/*****************************************************************************/
//  Function name:	TP_Disable
//  Description:
//  Global resource dependence: 
//  Author: yi.qiu
//  Note:
/*****************************************************************************/
PUBLIC void TP_Disable()
{
	s_tp_drv_fun->_tp_disable();
}

/*****************************************************************************/
//  Function name:	TP_MsgHandler
//  Description: This function will receive the message received from TP driver 
//  layer
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
LOCAL void TP_MsgHandler(uint8 msg_type)
{
	switch(msg_type)
	{
		case DOWN_IRQ:
			_TP_DownIrqHandler();
			break;
		case UP_IRQ:
			SCI_PASSERT(0,("TP_MsgHandler, Exception UP IRQ"));/*assert verified*/
			break;
		case DONE_IRQ:
			_TP_DoneIrqHandler();
			break;
		default:
			SCI_PASSERT(0,("TP_MsgHandler, Exception IRQ"));/*assert verified*/
			break;
	}
}
