/*************************************************************************
 ** File Name:      Tpd_drv.c                                            *
 ** Author:           manliang.yang                                      *
 ** Date:             11/05/2005                                         *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.    *
 ** Description:    This file defines the basic interfaces of  touch panel on sp7360,the MPU is *
 **                            sc6600d     *
 ** Note:           1. This file isn't finished to add Compiler Flag for the OS changing between *
 **                         Nucleus and ThreadX             *
 **                 2. the GPIO interrupt mode is level             *
 **                 3. consider calibration function needn't get data from this driver. The    *
 **                         other way,this driver need a sign to tnow MMI is calibrating or not *
 **                 4. The touch pen moving message is send with a time space of     *
 **                         (TP_TIMER_INTERVAL)*(s_de_shading.ct),so ,moving speed is need   *
 **                         slowly  *
 **                 5. Even the pen no moving but is down ,timer interrupt happen always   *
 **                 6. this function to set GPIO1\2\26\27(YU\YD\XL\XR)   and  X_ADCI1(C6|bit10)/   *
 **	                   		Y_ADCI4(C7|bit 13)     GPIO26 to int                                     *  
 **                 7. For fast speed with touch panel ,TP_MEASURE_COUNT is shoud be 1,  *
 **                    		and TP_TIMER_INTERVAL shoud set three level:6/9/12
 *************************************************************************
 *************************************************************************
 **                        Edit History                                    *
 ** -------------------------------------------------------------------------------------------------------*
 ** DATE               NAME                  DESCRIPTION                    *
 ** 11/05/2005     manliang.Yang     Create.                                *
 ** 12/15/2005     David.Jia        Reimplement functions: IsGpioMessage,   *
 **     TP_GetADCValue, TP_Calculate, new TP_CALIBRATION_T and TP_HEIGHT.   *
 ** 12/21/2005	   David.Jia		Set YD to output low while idel.		*
*************************************************************************/

#include "ms_customize_trc.h"
#include "tp_srv.h"
#include "tp_api.h"
#include "tp_cfg.c"
#include "ref_outport.h"
#include "syscnt_drv.h"
#include "tx_port_thumb.h"
#include "diag.h"
#include "dal_lcd.h"
#include "dal_keypad.h"
#include "tb_hal.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
#undef __MODULE__ 
#define __MODULE__ "Tpd_drv.c"
#ifndef INLINE
    #define INLINE                      __inline
#endif /* INLINE */

/**---------------------------------------------------------------------------*/
//                              Macro Definition
/**---------------------------------------------------------------------------*/
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


#define TP_ADC_BETWEEN_MAX  	      20

#define TP_ADSAMPLENUM                5

#define TP_ADC_MOVE_MAX               60
#define TP_ADC_MOVE_MID               30 
#define TP_ADC_MOVE_MIN               0       //please don't change the value

#define OFFSET_X                     10
#define OFFSET_Y                     10
#define MAX_PT_NUM                   100       //define the number of sample point

#define BOUND_LIMIT                  3

#define GPIO_STATE_HIGH              1
#define GPIO_STATE_LOW               0
#define GPIO_DIRECTION_O             1
#define GPIO_DIRECTION_I             0

#if 0
#define TP_DOWN_MSG 		     (uint8)1
#define TP_MOVE_MSG		     (uint8)2
#define TP_UP_MSG		     (uint8)3
#endif //sunnyfei_tpm

#define TP_IGNORED_LATEST_COOR      3       //Ignored several latest coordinates, normally they are detected error. 
#define TP_APP_MAX_AVAIL_QUEUE_NUM  100

#define VALID_ADC(x,y)              (((x)<s_tp_cal.x_max_top)&&((x)>=s_tp_cal.x_min_up)\
                                    &&((y)<s_tp_cal.y_max_left)&&((y)>=s_tp_cal.y_min_up) || s_is_rawdata) 
#define VALID_ADC_DELTA(x)          ((x)<TP_ADC_MOVE_MAX)


#define GPIO_DIR_BASE       (0x8a000008)
#define GET_GPIO_DIR(x)     ((((x)>>4)<<7)+GPIO_DIR_BASE)
#define GPIO_SETBIT(x)      ((x)&0xf)

#define GPIO_MASK_BASE      (0x8a000004)
#define GET_GPIO_MASK(x)    ((((x)>>4)<<7)+ GPIO_MASK_BASE)

#define GPIO_DATA_BASE      (0x8a000000)
#define GET_GPIO_DATA(x)    ((((x)>>4)<<7)+GPIO_DATA_BASE) 


#define TP_GPIODIR_OUTPUT_XL  	do{*(volatile uint32 *)(GET_GPIO_DIR(TP_GPIO_XL)) |= (1<<(GPIO_SETBIT(TP_GPIO_XL)));}while(0);
#define TP_GPIODIR_OUTPUT_XR  	do{*(volatile uint32 *)(GET_GPIO_DIR(TP_GPIO_XR)) |= (1<<(GPIO_SETBIT(TP_GPIO_XR)));}while(0);
#define TP_GPIODIR_OUTPUT_YU  	do{*(volatile uint32 *)(GET_GPIO_DIR(TP_GPIO_YU)) |= (1<<(GPIO_SETBIT(TP_GPIO_YU)));}while(0);
#define TP_GPIODIR_OUTPUT_YD    do{*(volatile uint32 *)(GET_GPIO_DIR(TP_GPIO_YD)) |= (1<<(GPIO_SETBIT(TP_GPIO_YD)));}while(0);

#define TP_GPIODIR_INPUT_XL   	do{*(volatile uint32 *)(GET_GPIO_DIR(TP_GPIO_XL)) &= (~(1<<(GPIO_SETBIT(TP_GPIO_XL))));}while(0);
#define TP_GPIODIR_INPUT_XR   	do{*(volatile uint32 *)(GET_GPIO_DIR(TP_GPIO_XR)) &= (~(1<<(GPIO_SETBIT(TP_GPIO_XR))));}while(0);
#define TP_GPIODIR_INPUT_YU   	do{*(volatile uint32 *)(GET_GPIO_DIR(TP_GPIO_YU)) &= (~(1<<(GPIO_SETBIT(TP_GPIO_YU))));}while(0);
#define TP_GPIODIR_INPUT_YD   	do{*(volatile uint32 *)(GET_GPIO_DIR(TP_GPIO_YD)) &= (~(1<<(GPIO_SETBIT(TP_GPIO_YD))));}while(0);

#define TP_GPIO_HIGH_XL     	do{*(volatile uint32 *)(GET_GPIO_DATA(TP_GPIO_XL)) |= (1<<(GPIO_SETBIT(TP_GPIO_XL)));}while(0);
#define TP_GPIO_HIGH_YU     	do{*(volatile uint32 *)(GET_GPIO_DATA(TP_GPIO_YU)) |= (1<<(GPIO_SETBIT(TP_GPIO_YU)));}while(0);
#define TP_GPIO_LOW_XR      	do{*(volatile uint32 *)(GET_GPIO_DATA(TP_GPIO_XR)) &= (~(1<<(GPIO_SETBIT(TP_GPIO_XR))));}while(0);
#define TP_GPIO_LOW_YD      	do{*(volatile uint32 *)(GET_GPIO_DATA(TP_GPIO_YD)) &= (~(1<<(GPIO_SETBIT(TP_GPIO_YD))));}while(0);

#define TP_GPIO_ENABLE_XL		GPIO_Enable(TP_GPIO_XL)
#define TP_GPIO_ENABLE_XR		GPIO_Enable(TP_GPIO_XR)
#define TP_GPIO_ENABLE_YU		GPIO_Enable(TP_GPIO_YU)
#define TP_GPIO_ENABLE_YD		GPIO_Enable(TP_GPIO_YD)

#define FLOAT32_PRECISION       (1000)

//@David.Jia    2005.12.15  begin
typedef struct {        //present coordinate (x,y) or adc value (adc_x, adc_y)
    int16  x;
    int16  y;
} TP_PAIR_T;

/*message format:   
    [31:0] -- touch panel message type (DOWN/MOVE/UP)
    [31:16]-- hit coordinate x
    [15:0] -- hit coordinate y
*/   
typedef struct {                    
    //STRUCT_HEAD           //@David.Jia 2006.3.17
    TOUCHPANEL_MSG_SERVICE_E TP_type;
    uint16 TP_x;
    uint16 TP_y;        
} COMM_TP_T, *COMM_TP_PTR;  /*lint -esym(751, COMM_TP_T)*/

#if 0
typedef enum {
	TP_PEN_NONE = 0x00,			/*lint -esym(749, TP_PEN_NONE)*/
    TP_PEN_DOWN=1,    /*1*/  
    TP_PEN_UP,      /*2*/
    TP_PEN_DONE,    /*3*/		/*lint -esym(749, TP_PEN_DONE)*/
    TP_UNKNOWN_STATE/*4*/		/*lint -esym(749, TP_UNKNOWN_STATE)*/
} TP_STATE_E;
#endif //sunnyfei_tpm

/**---------------------------------------------------------------------------*/
//                               Global Variables
/**---------------------------------------------------------------------------*/
LOCAL TP_PAIR_T 		s_TP_SamplePnt[MAX_PT_NUM] 	= {0};
LOCAL uint32    		s_TP_rd_ptr 				= 0;
LOCAL BOOLEAN   		s_TP_is_up 					= TRUE;		
LOCAL BOOLEAN   		s_TP_is_sample 				= FALSE;
LOCAL BOOLEAN                   s_is_rawdata 				= FALSE;		              //
LOCAL TP_CALIBRATION_T          s_tp_cal					= {0};                       //
LOCAL TP_PAIR_T 		g_adc_lt[7]					= {0};
LOCAL TP_PAIR_T 		g_delta[7]					= {0};
LOCAL TPDSVR_SIG_T		tp_sig_ptr					= PNULL;
LOCAL uint16       		tp_old_state 				= TP_UP ;
LOCAL uint32       		s_TP_sleeptime 				= TP_MIN_SLEEPTIME;
LOCAL uint32       		get_adc_count				= 0;
//LOCAL uint32                    s_TP_calibration_mode       = 0;
LOCAL TP_INFO_T 		s_tp_info_t 				= {0};
//LOCAL TPDSVR_SIG_T              tp_sig_ptr;
LOCAL TPDSVR_SIG_T              s_tp_sig;       //touch panel signal send to client  

//extern uint32 get_queue_avilable(void);
/**---------------------------------------------------------------------------*/
//                               Global Variables
/**---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*/
//                      Local Function Prototypes 
/**---------------------------------------------------------------------------*/
LOCAL void TP_TimerCallback(uint32 param1);
LOCAL BOOLEAN TP_IsPenUp(void);
extern uint32 get_app_queue_avilable(void);
/*****************************************************************************/
//  FUNCTION:     TP_MountGPIOHandler
//  Description:   
//  Return:        TRUE,          success
//      	          FALSE,         unsuccessful     
//  Author:         
//  date:            2005.05.11
//	Note:           
/*****************************************************************************/
LOCAL BOOLEAN  TP_MountGPIOHandler  (void);

/*****************************************************************************/
//  FUNCTION:     TP_GPIOHandler
//  Description:  
//  Return:      
//  Author:         
//  date:            2005.05.11
//	Note:           
/*****************************************************************************/
LOCAL void  TP_GPIOHandler (
        uint32 pin_id,
        uint32 pin_state);

/*****************************************************************************/
//  Description:    This function is used to get touch panel ADC convertor valu
//  Author:           manliang.yang
//  Date:              2005.05.11 
//	Note:           
/*****************************************************************************/
static TP_PAIR_T TP_GetADCValue(void);      

/*****************************************************************************/
//  Description:    This function is used to Calculate touch panel screen posite from ADC convertor valu
//  Author:           manliang.yang
//  Date:              2005.05.11 
//	Note:             to be sure that the Calculate data is effective,so need't judge denominator is zreo or not
/*****************************************************************************/
//LOCAL INLINE void TP_Calculate(uint32 *px,uint32 *py,TP_DESHADING_T s_de_shading);
//@David.Jia    2005.12.15
/*
    Transfer ADC value to coordinate.
    return  0: detect pen up, adc value below minnium value;
            1: others.
*/
static int TP_Calculate(TP_PAIR_T adc, TP_PAIR_T * coor_ptr);

/*
    TP_IsMoving(TP_PAIR_T his, TP_PAIR_T coor)
        Decide current point is moving (!= history), not fly ( coor - his < TP_MOVE_MAX)
*/
static int TP_IsMoving(TP_PAIR_T his, TP_PAIR_T coor);


/*****************************************************************************/
//  FUNCTION:     TP_WeakupSetting
//  Description:   
//  Return: 
//  Author:         
//  date:            2005.05.11
//	Note:           
/*****************************************************************************/
void TP_WeakupSetting(void)
{
	//XL/XR/YL/YR input

	TP_GPIODIR_INPUT_XL;
	TP_GPIODIR_INPUT_XR;
	TP_GPIODIR_INPUT_YU;
	TP_GPIODIR_INPUT_YD;

	
	//XL/XR  NO wpu
	//*(volatile uint32 *)0x8C0000D4 &= ~(BIT_10|BIT_11);
	
	//YD NO wpu
	//*(volatile uint32 *)0x8C0000E4 &= ~BIT_12;
	
}

/*****************************************************************************/
//  FUNCTION:     TP_SleepSetting
//  Description:   
//  Return: 
//  Author:         
//  date:            2005.05.11
//	Note:           
/*****************************************************************************/
void TP_SleepSetting(void)
{
	//YU/XL/XR Input
	TP_GPIODIR_INPUT_XL;
	TP_GPIODIR_INPUT_XR;
	TP_GPIODIR_INPUT_YD;	

	GPIO_SetDirection(TP_GPIO_YU, GPIO_DIRECTION_O);   
	
        GPIO_SetValue(TP_GPIO_YU, 0);

}
/*****************************************************************************/
//  Description:   to get tp information
//                  
//                  
//  Author:         Liangwen.Zhen
//	Note:           
/*****************************************************************************/
PUBLIC TP_INFO_T* TP_GetInfo(void)
{
	LCD_INFO_T	lcd_info;	
	LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

	SCI_ASSERT(lcd_info.lcd_width&&lcd_info.lcd_height);/*assert verified*/
	
	s_tp_info_t.tp_width  	= lcd_info.lcd_width;//TP_WIDTH;  
	s_tp_info_t.tp_height 	= lcd_info.lcd_height+TP_HWICON_Y_LENGHT;//TP_HEIGHT;
	s_tp_info_t.tp_lcd_height = lcd_info.lcd_height;//TP_HEIGHT_LCD;  
	                       
	s_tp_info_t.adc_min_x 	  = TP_ADC_MIN_X; 
	s_tp_info_t.adc_min_y 	  = TP_ADC_MIN_Y; 
	s_tp_info_t.adc_max_x 	  = TP_ADC_MAX_X; 
	s_tp_info_t.adc_max_y 	  = TP_ADC_MAX_Y;	
	
	s_tp_info_t.exchange_x 	  = EXCHANGE_X;

    s_tp_info_t.exchange_y 	  = EXCHANGE_Y;


	return &s_tp_info_t;

}

/*****************************************************************************/
//  FUNCTION:     TP_InitCalValue
//  Description:  initialize touach panel calibration's value .
//  return:     
//  Author:       Pengyu.li
//  date:  2006.03.03
//	Note:        
/*****************************************************************************/
LOCAL void TP_InitCalValue(TP_CALIBRATION_T* tp_calibration)
{

	uint32 tp_xfactor,tp_yfactor;
	LCD_INFO_T	lcd_info;	
	LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

	SCI_ASSERT(lcd_info.lcd_width&&lcd_info.lcd_height);/*assert verified*/
	
	tp_xfactor = TP_ADC_MAX_X - TP_ADC_MIN_X;
	tp_xfactor = tp_xfactor*FLOAT32_PRECISION;
	tp_xfactor = tp_xfactor/lcd_info.lcd_width;//tp_xfactor/TP_WIDTH;
	tp_yfactor = TP_ADC_MAX_Y - TP_ADC_MIN_Y;
	tp_yfactor = tp_yfactor*FLOAT32_PRECISION;
	tp_yfactor = tp_yfactor/(lcd_info.lcd_height+TP_HWICON_Y_LENGHT);//tp_yfactor/TP_HEIGHT;
	
	tp_calibration->x_top_factor = tp_calibration->x_bottom_factor = tp_xfactor;
	tp_calibration->y_left_factor = tp_calibration->y_right_factor = tp_yfactor;
	tp_calibration->x_min_top = tp_calibration->x_min_bottom = TP_ADC_MIN_X;
	tp_calibration->x_max_top = tp_calibration->x_max_bottom = TP_ADC_MAX_X;
	tp_calibration->y_min_left = tp_calibration->y_min_right = TP_ADC_MIN_Y;
	tp_calibration->y_max_left = tp_calibration->y_max_right = TP_ADC_MAX_Y;
	tp_calibration->x_center = (TP_ADC_MIN_X + TP_ADC_MAX_X) / 2;
	tp_calibration->y_center = (TP_ADC_MIN_Y + TP_ADC_MAX_Y) / 2;
	tp_calibration->x_min_up = TP_ADC_MIN_X;
	tp_calibration->y_min_up = TP_ADC_MIN_Y;
}	

/*****************************************************************************/
//  FUNCTION:     TP_DInit
//  Description:    initialize touach panel driver ,include GPIO/NV/TIMER
//  return:     
//  Author:         
//  date:  2005.05.11
//	Note:        
/*****************************************************************************/
PUBLIC void  TP_DInit(void)
{//place it to mainapp.c/main_app_init()/before@switch (call_ring_type)|line3227
	
	s_tp_cal.x_top_factor    = 1;      
    s_tp_cal.x_bottom_factor = 1;  
    s_tp_cal.y_left_factor   = 1;		
    s_tp_cal.y_right_factor  =1;
    s_tp_cal.x_min_up = TP_ADC_MIN_X;
    s_tp_cal.y_min_up = TP_ADC_MIN_Y;   
		
    //TPDRV_PRINT:"TP_DRIVER: enter into TP_DInit\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_352_112_2_18_0_35_6_2094,(uint8*)"");
   
    
    //TPDRV_PRINT("TP_DRIVER: g_tp_xfactor Value is %d \n", g_tp_xfactor);
	//TPDRV_PRINT("TP_DRIVER: g_tp_yfactor Value is %d \n", g_tp_yfactor);
	//SCI_TRACE_LOW:"********TP_DInit***********\r\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_357_112_2_18_0_35_6_2095,(uint8*)"");
 

   	TP_GPIODIR_INPUT_XL;
   	TP_GPIODIR_INPUT_XR;
   	TP_GPIODIR_INPUT_YD;
   	TP_GPIODIR_INPUT_YU;

	TP_GPIO_ENABLE_XL;
	TP_GPIO_ENABLE_XR;
	TP_GPIO_ENABLE_YD;
	TP_GPIO_ENABLE_YU;
	
    //@David.Jia	2005.12.21	begin
	TP_SleepSetting();
    //@David.Jia	2005.12.21	end
	
	// for init calibration's value
 	TP_InitCalValue(&s_tp_cal);
    
    OS_TickDelay(10);  
   
	GPIO_SetInterruptSense(TP_GPIO_XL,GPIO_INT_LEVEL_LOW);
	GPIO_EnableInt();
	GPIO_ClearIntStatus(TP_GPIO_XL);    
	TP_MountGPIOHandler();
    
    OS_TickDelay(10);    
    
    //Initialize the s_TP_SamplePnt
    SCI_MEMSET(s_TP_SamplePnt,0,sizeof(s_TP_SamplePnt));
    
    //Initialize the s_TP_rd_ptr and s_TP_is_up
    s_TP_rd_ptr = 0;
    s_TP_is_up = TRUE;
    //SCI_TRACE_LOW:"*******TP_DInit finish********"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_392_112_2_18_0_35_7_2096,(uint8*)"");
    
}

/*****************************************************************************/
//  Description:    This function sets the TP task sleep time.
//  Author:         Nick.zhao
//	Note:           
/*****************************************************************************/
PUBLIC void TP_NeedSleepTime(BOOLEAN bNeed)
{
    if(bNeed)
    {
    	s_TP_sleeptime = TP_MAX_SLEEPTIME;
	}         
    else
    {
     	s_TP_sleeptime = TP_MIN_SLEEPTIME;  
    }
}

PUBLIC void TP_Calibration_Mode(BOOLEAN bMode)
{
#if 0
    if(bMode)
    {
    	s_TP_calibration_mode = SCI_TRUE;
	}         
    else
    {
     	s_TP_calibration_mode = SCI_FALSE;  
    }
#endif
}



/*****************************************************************************/
//  Description:    Main thread of TPSVR task. It manages the pen signal
//                  from driver, and send it to the tasks which registered to 
//                  it before. It also processes PowerOn/PowerOff operation.
//  Author:         Robert.Lu
//	Note:           
/*****************************************************************************/
LOCAL void send_tp_message(uint8 tp_message_type,uint16 tp_message_x,uint16 tp_message_y)
{
    uint32  app_queue_avilable;   
 
     tp_sig_ptr.x_key = tp_message_x;
    tp_sig_ptr.y_key = tp_message_y;
    switch(tp_message_type)
    {
    	case 1 :
    		tp_sig_ptr.SignalCode = TP_DOWN;         
            //TPDRV_PRINT("TP_DRIVER: TP_DOWN message is sended\n");
            break;
        case 2 :
    		tp_sig_ptr.SignalCode = TP_MOVE;            
            //TPDRV_PRINT("TP_DRIVER: TP_MOVE message is sended\n");
            break;
        case 3 :
    		tp_sig_ptr.SignalCode = TP_UP;
           // TPDRV_PRINT("TP_DRIVER: TP_UP message is sended\n");
            break;
        default : break;
    }
    app_queue_avilable = get_app_queue_avilable();
    if (app_queue_avilable > 10)
        {
        SCI_SendEventToClient(TP_SERVICE, tp_sig_ptr.SignalCode, (void *)&tp_sig_ptr);
        tp_old_state = tp_sig_ptr.SignalCode; 
        }
    // make sure the up signal must be sent out, meanwhile, repeated up signal should be discard;
    else if((tp_old_state != TP_UP) && (tp_sig_ptr.SignalCode == TP_UP))
        {
                SCI_SendEventToClient(TP_SERVICE, tp_sig_ptr.SignalCode, (void *)&tp_sig_ptr);
                tp_old_state = tp_sig_ptr.SignalCode; 
        }    
       
       while(get_app_queue_avilable() < 10){
            SCI_Sleep(50);    
       }
       

}


/*****************************************************************************/
//  Description:    This filter is used to bypass some bad point during the pen
//                  is moving.
//  Author:         Nick.zhao
//	Note:           
/*****************************************************************************/
LOCAL TP_PAIR_T TP_ADC_Filter(TP_PAIR_T adc1, TP_PAIR_T adc2, TP_PAIR_T adc3)
{
    uint32 delta1, delta2, delta3,delta1x, delta2x, delta3x,delta1y, delta2y, delta3y, delta;
    TP_PAIR_T adc_ret;
    
    adc_ret.x = 0;
    adc_ret.y = 0;
    
    delta1x = (adc1.x - adc2.x)>=0 ? (adc1.x-adc2.x):(adc2.x-adc1.x);
    delta2x = (adc2.x - adc3.x)>=0 ? (adc2.x-adc3.x):(adc3.x-adc2.x);
    delta3x = ((adc3.x - adc1.x)>=0 ? (adc3.x-adc1.x):(adc1.x-adc3.x))>>1;
    
    delta1y = (adc1.y - adc2.y)>=0 ? (adc1.y-adc2.y):(adc2.y-adc1.y);
    delta2y = (adc2.y - adc3.y)>=0 ? (adc2.y-adc3.y):(adc3.y-adc2.y);
    delta3y = ((adc3.y - adc1.y)>=0 ? (adc3.y-adc1.y):(adc1.y-adc3.y))>>1;
    
    delta1 = delta1x + delta1y;
    delta2 = delta2x + delta2y;
    delta3 = (delta3x + delta3y);   //Nick modified it.
    
    delta = MIN(MIN(delta1, delta2), delta3);
    
    if(delta == delta1)
    {
      adc_ret.x = (adc1.x+adc2.x)>>1;
      adc_ret.y = (adc1.y+adc2.y)>>1;
    }
     else if(delta == delta2)
    {
      adc_ret.x = (adc2.x+adc3.x)>>1;
      adc_ret.y = (adc2.y+adc3.y)>>1;  
    }
     else if(delta == delta3)
    {
      adc_ret.x = (adc3.x+adc1.x)>>1;
      adc_ret.y = (adc3.y+adc1.y)>>1;
    }

    return adc_ret; 
}
/*****************************************************************************/
//  Description:    This filter is used to bypass some bad point during the pen
//                  is up.
//  Author:         Nick.zhao
//	Note:           
/*****************************************************************************/
TP_PAIR_T TP_PenUp_Filter(uint32 tp_ptr)
{
    register volatile uint32 i, first_cnt, second_cnt, data1, data2;
    TP_PAIR_T adc_ret;

    adc_ret.x = 0;
    adc_ret.y = 0;
    
    //TPDRV_PRINT:"TP_DRV: TP UP filter begin, %d!\n\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_531_112_2_18_0_35_7_2097,(uint8*)"d",tp_ptr);
    //Firstly we must copy the latest seven point as pre-detect point
    if(tp_ptr<12)
    {
       data1 = tp_ptr;
       
       data2 = (data1==0)?(MAX_PT_NUM-1):(data1-1);    
       g_adc_lt[6] = s_TP_SamplePnt[data2];   
           
       data1 = (data2==0)?(MAX_PT_NUM-1):(data2-1);    
       g_adc_lt[5] = s_TP_SamplePnt[data1];
       
       data2 = (data1==0)?(MAX_PT_NUM-1):(data1-1);    
       g_adc_lt[4] = s_TP_SamplePnt[data2];
       
       data1 = (data2==0)?(MAX_PT_NUM-1):(data2-1);
       g_adc_lt[3] = s_TP_SamplePnt[data1];
       
       data2 = (data1==0)?(MAX_PT_NUM-1):(data1-1);    
       g_adc_lt[2] = s_TP_SamplePnt[data2];
       
       data1 = (data2==0)?(MAX_PT_NUM-1):(data2-1);
       g_adc_lt[1] = s_TP_SamplePnt[data1];
       
       data2 = (data1==0)?(MAX_PT_NUM-1):(data1-1);    
       g_adc_lt[0] = s_TP_SamplePnt[data2];
          
    }
    else
    {
       for(i=0;i<7;i++)
         g_adc_lt[6-i] = s_TP_SamplePnt[tp_ptr-1-i-5];
    }
    for(i=0;i<7;i++)
    {
       //TPDRV_PRINT:"g_adc_lt[%d].x=%d, g_adc_lt[%d].y=%d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_566_112_2_18_0_35_7_2098,(uint8*)"dddd",i,g_adc_lt[i].x,i,g_adc_lt[i].y);
    }
    first_cnt = 6;
    //Secondly, we must judge the valid ADC point by filter1.
    for(i=0;i<7;i++)
    {
       if(!(VALID_ADC(g_adc_lt[i].x,g_adc_lt[i].y)))
       {
         first_cnt = i;
         break;
       } 
    }
    
    //TPDRV_PRINT:"TP_DRV: TP UP filter middle %d!\n\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_579_112_2_18_0_35_7_2099,(uint8*)"d",first_cnt);
    
    if(first_cnt == 0)
    {
       return adc_ret;
    }
    //Thirdly, we must judge the valid ADC point by filter2
    second_cnt = first_cnt -1;

	
    for(i=0;i<first_cnt-1;i++)
    {
       register uint16 pre_data;
	   register uint16 next_data;

       //g_delta[i].x = (g_adc_lt[i].x - g_adc_lt[i+1].x >= 0) ? (g_adc_lt[i].x - g_adc_lt[i+1].x):(g_adc_lt[i+1].x - g_adc_lt[i].x);  /*lint !e564*/
	   pre_data = g_adc_lt[i].x;
	   next_data = g_adc_lt[i+1].x;
	   if(pre_data >= next_data)
	   {
	   	 g_delta[i].x = pre_data - next_data;
	   }
	   else
	   	{
	   		g_delta[i].x = next_data - pre_data;
	   	}
	  if(!VALID_ADC_DELTA(g_delta[i].x))
      {
         second_cnt = i;
         break;
      }   
      
      //g_delta[i].y = (g_adc_lt[i].y - g_adc_lt[i+1].y >= 0) ? (g_adc_lt[i].y - g_adc_lt[i+1].y):(g_adc_lt[i+1].y - g_adc_lt[i].y);/*lint !e564*/
	   pre_data = g_adc_lt[i].y;
	   next_data = g_adc_lt[i+1].y;
	   if(pre_data >= next_data)
	   {
	   	 g_delta[i].y = pre_data - next_data;
	   }
	   else
	   	{
	   		g_delta[i].y = next_data - pre_data;
	   	}
	  if(!VALID_ADC_DELTA(g_delta[i].y))
      {
         second_cnt = i;
         break;
      }
    } 
 
    //Fourth, we must judge the valid ADC point by filter3
    if(second_cnt == 0)
    {
       adc_ret = g_adc_lt[0];
    }   
    else if(second_cnt==1)
    {
       //adc_ret.x = (g_adc_lt[second_cnt-1].x + g_adc_lt[second_cnt].x)>>1;
       //adc_ret.y = (g_adc_lt[second_cnt-1].y + g_adc_lt[second_cnt].y)>>1;
       adc_ret = g_adc_lt[1];
    }
    else
    {
       //adc_ret = TP_ADC_Filter(g_adc_lt[second_cnt-2],g_adc_lt[second_cnt-1],g_adc_lt[second_cnt]);
       adc_ret = g_adc_lt[second_cnt];         
    }
    
    //TPDRV_PRINT:"TP_DRV: TP UP filter finish!\n\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_646_112_2_18_0_35_7_2100,(uint8*)"");
    return adc_ret; 
}
 
/*****************************************************************************/
//  Description:    This functoion to handle  TP gpio message
//                  
//  Author:         
//	Note:           
/*****************************************************************************/
void HandleGpioMessage(void)
{
    LOCAL TP_STATE_E s_status = TP_PEN_UP;    
    LOCAL TP_PAIR_T coordinate[TP_IGNORED_LATEST_COOR+1] ;      //save latest several coordinate
    int i=0;
    TP_PAIR_T current_coordinate = {0};                     //current coordinate
    BOOLEAN gpio_state;
    
    //TPDRV_PRINT:"TP_DRV: IsGpioMessage\n\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_662_112_2_18_0_35_7_2101,(uint8*)"");

    for (i =0; i<TP_IGNORED_LATEST_COOR; i++)
        coordinate[i].x = coordinate[i].y = -1;
        
    get_adc_count = 0;    

    //Initialize the s_TP_SamplePnt
    SCI_MEMSET(s_TP_SamplePnt,0,sizeof(s_TP_SamplePnt));
    //liangwen TP_WeakupSetting();		//@David.Jia	2005.12.22
    
    GPIO_DisableIntCtl(TP_GPIO_XL);     
    
    SCI_Sleep(TP_BEFORE_DESHAKING_TIME);      //deshaking time
    
    gpio_state = GPIO_GetValue(TP_GPIO_XL);
    //TPDRV_PRINT:"TP_DRV: XL_state = %d\n\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_678_112_2_18_0_35_7_2102,(uint8*)"d", gpio_state);
      
    if (GPIO_STATE_LOW == GPIO_GetValue(TP_GPIO_XL))    //still down, is NOT shaking
    {
        //TPDRV_PRINT:"TP_DRV: Start to ADC\n\r"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_682_112_2_18_0_35_7_2103,(uint8*)"");
        
        //start ADC timer in order to get ADC value
        Syscnt_SetTimer(TP_TIMER_TIME,TP_TimerCallback,0);
        s_TP_is_up = FALSE;
        s_TP_is_sample = FALSE;
	    while( !TP_IsPenUp() )
        {
            
            if(s_TP_is_sample)
            {
                {
                   volatile TP_PAIR_T temp1,temp2,temp3,temp4,temp5;
                   register volatile uint32 data1,data2,delta_x1,delta_y1,delta_x2,delta_y2,delta_x3,delta_y3;

                   data1 = s_TP_rd_ptr;
                      
                   //temp1 = s_TP_SamplePnt[data1];
                   
                   data2 = (data1==0)?(MAX_PT_NUM-1):(data1-1);
                   
                   temp1 = s_TP_SamplePnt[data2];
                   
                   data1 = (data2==0)?(MAX_PT_NUM-1):(data2-1);
                   
                   temp2 = s_TP_SamplePnt[data1];
                   
                   data2 = (data1==0)?(MAX_PT_NUM-1):(data1-1);
                   
                   temp3 = s_TP_SamplePnt[data2];
                   
                   data1 = (data2==0)?(MAX_PT_NUM-1):(data2-1);
                   
                   temp4 = s_TP_SamplePnt[data1];
                  
                   delta_x1 = (temp1.x - temp2.x>= 0)?(temp1.x-temp2.x):(temp2.x-temp1.x);
                   delta_y1 = (temp1.y - temp2.y>= 0)?(temp1.y-temp2.y):(temp2.y-temp1.y);
                   delta_x2 = (temp2.x - temp3.x>= 0)?(temp2.x-temp3.x):(temp3.x-temp2.x);
                   delta_y2 = (temp2.y - temp3.y>= 0)?(temp2.y-temp3.y):(temp3.y-temp2.y);
                   delta_x3 = (temp3.x - temp4.x>= 0)?(temp3.x-temp4.x):(temp4.x-temp3.x);
                   delta_y3 = (temp3.y - temp4.y>= 0)?(temp3.y-temp4.y):(temp4.y-temp3.y);
                   
                
                   if((VALID_ADC_DELTA(delta_x1)&&VALID_ADC_DELTA(delta_y1))&&(VALID_ADC_DELTA(delta_x2)&&VALID_ADC_DELTA(delta_y2))&&(VALID_ADC_DELTA(delta_x3)&&VALID_ADC_DELTA(delta_y3))
                      &&VALID_ADC(temp1.x,temp1.y)&&VALID_ADC(temp2.x,temp2.y)&&VALID_ADC(temp3.x,temp3.y))
                  //if(VALID_ADC(temp1.x,temp1.y)&&VALID_ADC(temp2.x,temp2.y)&&VALID_ADC(temp3.x,temp3.y))
                   {
                      temp5 = TP_ADC_Filter(temp1, temp2, temp3);
                   }
                   else
                   {
                      //temp5 = temp3;
                      //TPDRV_PRINT:"temp1.x = %d, temp1.y = %d\n\r"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_734_112_2_18_0_35_7_2104,(uint8*)"dd",temp1.x,temp1.y);
                      //TPDRV_PRINT:"temp2.x = %d, temp2.y = %d\n\r"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_735_112_2_18_0_35_7_2105,(uint8*)"dd",temp2.x,temp2.y);
                      //TPDRV_PRINT:"temp3.x = %d, temp3.y = %d\n\r"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_736_112_2_18_0_35_7_2106,(uint8*)"dd",temp3.x,temp3.y);
                      //TPDRV_PRINT:"delta_x1 = %d, delta_y1 = %d\n\r"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_737_112_2_18_0_35_7_2107,(uint8*)"dd",delta_x1,delta_y1);
                      //TPDRV_PRINT:"delta_x2 = %d, delta_y2= %d\n\r"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_738_112_2_18_0_35_7_2108,(uint8*)"dd",delta_x2,delta_y2);
                      //TPDRV_PRINT:"delta_x3 = %d, delta_y3= %d\n\r"
                      SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_739_112_2_18_0_35_7_2109,(uint8*)"dd",delta_x3,delta_y3);
                      
                      //TPDRV_PRINT("temp5.x = %d, temp5.y = %d\n\r",temp5.x,temp5.y);

    			        SCI_Sleep(s_TP_sleeptime); 
                        continue;
                   }
                  
                  
    			   
    			   if ( 0 == TP_Calculate(temp5, &current_coordinate))   //temp5
    			   {
    			        SCI_Sleep(s_TP_sleeptime); 
                        continue;
                   }                
                }
    			
                //TPDRV_PRINT:"TP_DRV: ADC detected\n\r"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_756_112_2_18_0_35_7_2110,(uint8*)"");
                if (TP_PEN_UP== s_status)                //first time detect down
                {
                    s_status = TP_PEN_DOWN;
                    send_tp_message(TP_DOWN_MSG, current_coordinate.x, current_coordinate.y);
                    //send_tp_message(TP_MOVE_MSG, coor.x, coor.y);
                    //TPDRV_PRINT:"TP_DRV: DOWN message, x=%d, y=%d\n\r"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_762_112_2_18_0_35_7_2111,(uint8*)"dd", current_coordinate.x, current_coordinate.y);
                    //his[0] = coor;
                    coordinate[3]=coordinate[2]=coordinate[1]=coordinate[0] = current_coordinate;                
                }
                else                                    //following messages after down
                {
                    if (TP_IsMoving(coordinate[3]/*his[0]*/, current_coordinate))       //move detect
                    {
                        //his[0] = coor;
                        coordinate[3] = current_coordinate;  
  				        coordinate[2].x=(coordinate[2].x+coordinate[1].x+coordinate[0].x+current_coordinate.x)>>2;
  				        coordinate[2].y=(coordinate[2].y+coordinate[1].y+coordinate[0].y+current_coordinate.y)>>2;
  				        coordinate[1].x=(coordinate[1].x+coordinate[0].x+current_coordinate.x+current_coordinate.x)>>2;
  				        coordinate[1].y=(coordinate[1].y+coordinate[0].y+current_coordinate.y+current_coordinate.y)>>2;
  				        coordinate[0].x=(coordinate[0].x+current_coordinate.x)>>1;
  				        coordinate[0].y=(coordinate[0].y+current_coordinate.y)>>1;     
                        //send_tp_message(TP_MOVE_MSG, his[0].x, his[0].y);
                        send_tp_message(TP_MOVE_MSG, coordinate[2].x, coordinate[2].y);
                        //TPDRV_PRINT:"TP_DRV: MOVE message, x=%d, y=%d, time=%dms\n\r"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_780_112_2_18_0_35_7_2112,(uint8*)"ddd",coordinate[2].x, coordinate[2].y,*(volatile uint32 *)0x8700001c);
                        
                    }   
                }
                
                SCI_Sleep(s_TP_sleeptime); 
            }
        }
        s_TP_is_up = TRUE;
   
        //TPDRV_PRINT:"TPDRV: UP action happened!  \n\r"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_790_112_2_18_0_35_7_2113,(uint8*)"");
           
        if (s_status == TP_PEN_DOWN)          //have send down message
        {
         
            send_tp_message(TP_UP_MSG, coordinate[2].x, coordinate[2].y);
            //TPDRV_PRINT:"TPDRV: UP message, x=%d, y=%d\n\r"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_796_112_2_18_0_35_7_2114,(uint8*)"dd", coordinate[2].x,coordinate[2].y);
           
           s_status = TP_PEN_UP;
           
           for (i=TP_IGNORED_LATEST_COOR; i>=0; i--)
			  coordinate[i].x = coordinate[i].y = -1;
        }

        s_TP_rd_ptr = 0;
        s_TP_is_sample = FALSE;
    }
    
    SCI_Sleep(TP_AFTER_DESHAKING_TIME);      //deshaking time
        

    GPIO_EnableIntCtl(TP_GPIO_XL);   
    
    TP_SleepSetting();		
    
    gpio_state = GPIO_GetValue(TP_GPIO_XL);

    //TPDRV_PRINT:"TP_DRV: XL_state = %d\n\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_817_112_2_18_0_35_7_2115,(uint8*)"d", gpio_state);
   
}


/*****************************************************************************/
//  FUNCTION:     TP_SetRawDataMode
//  Description:   
//  return:  
//  Author:         
//  date:  2005.05.11
//	Note:        
/*****************************************************************************/
PUBLIC void TP_SetRawDataMode(BOOLEAN act_res)
{
     if (!act_res)
     {
     	s_is_rawdata = FALSE;
     }
     else
     {
     	s_is_rawdata = TRUE;
    	}
}

/*****************************************************************************/
//  FUNCTION:     TP_UpdateCalibration
//  Description:    refresh base data of touch panel ,set calibration flag TRUE¡£
//  input:  
//  return: TRUE, 
//      	  FALSE,
//  Author:         
//  date:   2005.05.11
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN  TP_UpdateCalibration(TP_CALIBRATION_T * tp_para)
{	
	if (	(tp_para->x_top_factor>6000) 
		|| 	(tp_para->y_left_factor < FLOAT32_PRECISION) 
		|| 	(tp_para->y_left_factor > 6000) ) 
	{	
    	return FALSE;
    }

    if (	(tp_para->x_max_top>1023) 
    	|| 	(tp_para->x_max_bottom>1023) 
    	|| 	(tp_para->y_max_left>1023) 
    	|| 	(tp_para->y_max_right>1023))
    {
	    return FALSE;   
	   }
    s_tp_cal = *tp_para;
    s_tp_cal.is_valid = CALIBRATION_FLAG;
    
    //SCI_TRACE_LOW:"TP_UpdateCalibration:x_top_factor = %d,x_bottom_factor=%d,y_left_factor = %d,y_right_factor=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_867_112_2_18_0_35_7_2116,(uint8*)"dddd",s_tp_cal.x_top_factor,s_tp_cal.x_bottom_factor,s_tp_cal.y_left_factor,s_tp_cal.y_right_factor);

    return TRUE;
}


/**---------------------------------------------------------------------------*/
//                               Local Function Definitions 
/**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  FUNCTION:     TP_MountGPIOHandler
//  Description:    mount TP_GPIOHandler to GPIO interrupt vector table
//  Return:          TRUE:
//              	     FALSE
//  Author:
//  date:             2005.05.11
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN  TP_MountGPIOHandler  (void)
{
	//TPDRV_PRINT:"TP_DRIVER: enter into TP_MountGPIOHandler\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_884_112_2_18_0_35_8_2117,(uint8*)"");
		
    if( SCI_SUCCESS ==
    	GPIO_AddCallbackToIntTable(
        				TP_GPIO_XL,          			// GPIO ID       		 			
        				SCI_FALSE,
        				GPIO_DEFAULT_SHAKING_TIME,   	// interval for de shaking 
        				(GPIO_CALLBACK)TP_GPIOHandler  	// Callback function
        			)
      )
    {
     	//TPDRV_PRINT:"TP_DRIVER: TP_MountGPIOHandler SUCCES\n"
     	SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_895_112_2_18_0_35_8_2118,(uint8*)"");
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************/
//  FUNCTION:     TP_GPIOHandler
//  Description:
//  input:    pin_id:
//              pin_state:
//  Return:
//  Author:
//  date:            2005.05.10
//  Note:
/*****************************************************************************/
LOCAL void TP_GPIOHandler (
        uint32 pin_id,
        uint32 pin_state)
{
    TPD_TO_TPSVR_MSG_T *tp_to_tpsvr_sig_ptr    =  SCI_NULL;
    
    //TPDRV_PRINT:"TP_DRIVER: enter into TP_GPIOHandler\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_918_112_2_18_0_35_8_2119,(uint8*)"");
    
     
    if( (TP_GPIO_XL == pin_id) && (pin_state == (uint32)GPIO_GetGPIOState(TP_GPIO_XL)) ) 
    {
    	// should disable int first for avoiding memory lose, yingchun
      	GPIO_DisableIntCtl(TP_GPIO_XL);
      	tp_to_tpsvr_sig_ptr = (TPD_TO_TPSVR_MSG_T *)SCI_ALLOC_APP(sizeof(TPD_TO_TPSVR_MSG_T));
  
      	tp_to_tpsvr_sig_ptr->SignalSize = sizeof(TPD_TO_TPSVR_MSG_T);
      	tp_to_tpsvr_sig_ptr->Sender = P_TPP;
      	
    	SCI_ASSERT(SCI_NULL != tp_to_tpsvr_sig_ptr);/*assert verified*/
    	tp_to_tpsvr_sig_ptr->SignalCode = TP_ENTER_INTO_GPIO_HANDLER;
    	tp_to_tpsvr_sig_ptr->tpd_msg_valu = TP_ENTER_INTO_GPIO_HANDLER;
    	SCI_SendSignal((xSignalHeader)tp_to_tpsvr_sig_ptr, P_TPP);    	
    }
    else //liangwen 
    {
        //TPDRV_PRINT:"TP_DRIVER: PIN detect error in INT\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_937_112_2_18_0_35_8_2120,(uint8*)"");
        SCI_Sleep(TP_AFTER_DESHAKING_TIME);      //deshaking time
        GPIO_EnableIntCtl(TP_GPIO_XL);
    }
    GPIO_ClearIntStatus(TP_GPIO_XL);
}

/*****************************************************************************/
//  FUNCTION:     TP_GetADCValue
//  Description:
//  input:    pin_id:
//              pin_state:
//  Return:
//  Author:
//  date:            2005.05.10
//  Note:
/*****************************************************************************/
LOCAL TP_PAIR_T TP_GetADCValue(void)
{
     TP_PAIR_T adc;
     
     TP_GPIODIR_OUTPUT_YU;
     TP_GPIODIR_OUTPUT_YD;
     TP_GPIO_HIGH_YU;
     TP_GPIO_LOW_YD;   
     	
    // adc.y = ADC_GetResultDirectly(TP_ADC_PIN_X, 1);
    
    //We read ADC two times in order to ensure the ADC value is correct in SC6800.
#ifdef PRODUCT_DRIVER_KUNLUN3
	adc.y = ADC_GetResultDirectly(TP_ADC_PIN_Y, 0);
#endif
    adc.y = ADC_GetResultDirectly(TP_ADC_PIN_Y, 0);

    
    TP_GPIODIR_INPUT_YU;
    TP_GPIODIR_INPUT_YD;    
    
        
    TP_GPIODIR_OUTPUT_XL;
    TP_GPIODIR_OUTPUT_XR;
    TP_GPIO_HIGH_XL;
    TP_GPIO_LOW_XR;    

     
    // adc.x = ADC_GetResultDirectly(TP_ADC_PIN_Y, 1);
    //We read ADC two times in order to ensure the ADC value is correct in SC6800.
#ifdef PRODUCT_DRIVER_KUNLUN3
    adc.x = ADC_GetResultDirectly(TP_ADC_PIN_X, 0);
#endif
    adc.x = ADC_GetResultDirectly(TP_ADC_PIN_X, 0);
   
     
    TP_GPIODIR_INPUT_XL;
    TP_GPIODIR_INPUT_XR;           
     
     return adc;
     

}

/*****************************************************************************/
//  Description:    This function is used to Calculate touch panel screen posite from ADC convertor valu
//  Author:           manliang.yang
//  Date:              2005.05.11 
//	Note:             to be sure that the Calculate data is effective,so need't judge denominator is zreo or not
/*****************************************************************************/
LOCAL int TP_Calculate(TP_PAIR_T adc, TP_PAIR_T *coor_ptr)
{
    LCD_INFO_T	lcd_info;
    int16 tp_width,tp_height;
    LCD_GetInfo(MAIN_LCD_ID, &lcd_info);

    SCI_ASSERT(lcd_info.lcd_width&&lcd_info.lcd_height);/*assert verified*/
    tp_width  = (int16)lcd_info.lcd_width;
    tp_height =	(int16)(lcd_info.lcd_height+TP_HWICON_Y_LENGHT);
    //TPDRV_PRINT:"TPDRV: ADC x=%d, y=%d.\n\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_1009_112_2_18_0_35_8_2121,(uint8*)"dd", adc.x, adc.y);
	

    if(s_is_rawdata)
    {
    	(*coor_ptr).x = adc.x;
        (*coor_ptr).y = adc.y;
                     
        //TPDRV_PRINT:"TPDRV: Coordinate: x=%d, y=%d.\n\r"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_1017_112_2_18_0_35_8_2122,(uint8*)"dd", (*coor_ptr).x, (*coor_ptr).y);
        return 1;
    }
    else if ((adc.x>=s_tp_cal.x_min_up) && (adc.y>=s_tp_cal.y_min_up))     //pen is down
    {  
    	int16 x, y;     //temp coordinate, below 0 set to 0, above TP_WIDTH/HEIGHT set to TP_WIDTH/HEIGHT

		if (adc.x > s_tp_cal.x_center)      //top
        {
            if(EXCHANGE_X)  /*lint !e506 !e774*/
            {
            	adc.x = s_tp_cal.x_center - (adc.x - s_tp_cal.x_center); 
            }
            x = tp_width - (int32)(adc.x - s_tp_cal.x_min_top) * FLOAT32_PRECISION / s_tp_cal.x_top_factor;
        }
        else        
        {
        	if(EXCHANGE_X)  /*lint !e506 !e774*/
            {
            	adc.x = s_tp_cal.x_center - adc.x + s_tp_cal.x_center; 
            }
            x = tp_width - (int32)(adc.x - s_tp_cal.x_min_bottom) * FLOAT32_PRECISION / s_tp_cal.x_bottom_factor;
        }
        
        x = x > tp_width ? tp_width : x;
        x = x < 0 ? 0 : x;
        
        if (adc.y > s_tp_cal.y_center)      //left
        {
            if(EXCHANGE_Y)  /*lint !e506 !e774*/
            {
            	adc.y = s_tp_cal.y_center - (adc.y - s_tp_cal.y_center) ;
            }
            y = tp_height - (int32)(adc.y - s_tp_cal.y_min_left) * FLOAT32_PRECISION / s_tp_cal.y_left_factor;
        }
        else
        {
        	if(EXCHANGE_Y)  /*lint !e506 !e774*/
            {
            	adc.y = s_tp_cal.y_center - adc.y + s_tp_cal.y_center;
            }
            y = tp_height - (int32)(adc.y - s_tp_cal.y_min_right) * FLOAT32_PRECISION / s_tp_cal.y_right_factor;
        }
    
        y = y > tp_height ? tp_height : y;
        y = y < 0 ? 0 : y;
            
        (*coor_ptr).x = x;  (*coor_ptr).y = y;
        
        //TPDRV_PRINT:"TPDRV: Coordinate: x=%d, y=%d.\n\r"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_1066_112_2_18_0_35_8_2123,(uint8*)"dd", (*coor_ptr).x, (*coor_ptr).y);
        return 1;
       }
    //TPDRV_PRINT:"TPDRV: TP_Calculate, pen is UP.\n\r"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_1069_112_2_18_0_35_8_2124,(uint8*)"");
    return 0;
}

/*****************************************************************************/
//  Description:    
//  Author:         
//  Date:           
//	Note:           
/*****************************************************************************/
LOCAL int TP_IsMoving(TP_PAIR_T his, TP_PAIR_T coor)
{
    int delta_x, delta_y;
    
    delta_x = coor.x - his.x;
    delta_x = (delta_x > 0) ? delta_x : (- delta_x);
    delta_y = coor.y - his.y;
    delta_y = (delta_y > 0) ? delta_y : (- delta_y);
        
    if ((TP_MOVE_MIN < (delta_x+delta_y)) && ((delta_x+delta_y) < TP_MOVE_MAX))
    {
        return 1;
    }
    
    return 0;
}

/*****************************************************************************/
//  Description:    
//  Author:         
//  Date:           
//	Note:           
/*****************************************************************************/
LOCAL BOOLEAN TP_IsPenUp(void)
{
    BOOLEAN gpio_val;
    
    TX_INTERRUPT_SAVE_AREA
    TX_DISABLE_FIQ_IRQ
    
    TP_GPIODIR_INPUT_XL;
    TP_GPIODIR_INPUT_XR;
    TP_GPIODIR_INPUT_YU;
    TP_GPIODIR_OUTPUT_YD;

    TP_GPIO_LOW_YD;
	    
    gpio_val = GPIO_GetValue(TP_GPIO_XL);
    
    TX_RESTORE
    
    return(gpio_val);
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
        GPIO_EnableIntCtl(TP_GPIO_XL);
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
        GPIO_DisableIntCtl(TP_GPIO_XL);
}

/*****************************************************************************/
//  Description:    
//  Author:         
//  Date:           
//	Note:           
/*****************************************************************************/
LOCAL void TP_TimerCallback(uint32 param1)
{
   TP_PAIR_T adc_val;
   static TP_PAIR_T temp;
   uint32 delta_x,delta_y;   
   //Start the timer according to the s_TP_is_up
   if(!s_TP_is_up)
   {
      
      //Catch the ADC value
      adc_val = TP_GetADCValue();
      
      if(get_adc_count>7)
      {
        delta_x = ((adc_val.x - temp.x) >= 0) ? (adc_val.x - temp.x) : (temp.x - adc_val.x);  /*lint !e727 */
        delta_y = ((adc_val.y - temp.y) >= 0) ? (adc_val.y - temp.y) : (temp.y - adc_val.y);  /*lint !e727 */

        if((TP_ADC_MOVE_MIN < delta_x) && ( delta_x <=TP_ADC_MOVE_MID) && (TP_ADC_MOVE_MIN < delta_y )&&  (delta_y <=TP_ADC_MOVE_MID)) 
          {
            temp = adc_val;
          }
        else if((TP_ADC_MOVE_MIN < delta_x) && ( delta_x <=TP_ADC_MOVE_MID) && (TP_ADC_MOVE_MID < delta_y )&&  (delta_y <=TP_ADC_MOVE_MAX)) 
          {
            temp.x = adc_val.x;
            temp.y = (adc_val.y + temp.y)>>1;
          }
        else if((TP_ADC_MOVE_MID < delta_x) && ( delta_x <=TP_ADC_MOVE_MAX) && (TP_ADC_MOVE_MIN < delta_y )&&  (delta_y <=TP_ADC_MOVE_MID)) 
          {
            temp.x = (adc_val.x + temp.x)>>1;
            temp.y = adc_val.y;
          }
        else if((TP_ADC_MOVE_MID < delta_x) && ( delta_x <=TP_ADC_MOVE_MAX) && (TP_ADC_MOVE_MID < delta_y )&&  (delta_y <=TP_ADC_MOVE_MAX)) 
          {
            temp.x = (adc_val.x + temp.x)>>1;
            temp.y = (adc_val.y + temp.y)>>1;
          }
        else
          {
            temp = adc_val;
          }   
   
      }
 
      //Store the ADC value into buffer
      s_TP_SamplePnt[s_TP_rd_ptr++] = adc_val;
      
      if(s_TP_rd_ptr>=TP_ADSAMPLENUM)
        s_TP_is_sample = TRUE;
        
      if(s_TP_rd_ptr == MAX_PT_NUM)
         s_TP_rd_ptr = 0;
       
      if (get_adc_count == 7)
        {
           temp = TP_ADC_Filter(s_TP_SamplePnt[6], s_TP_SamplePnt[5], s_TP_SamplePnt[4]);
        }   
         
      get_adc_count++;
      
      Syscnt_SetTimer(TP_TIMER_TIME,TP_TimerCallback,0);
   }
}

/*****************************************************************************/
//  Description:    Handle touch panel emulate message.
//	Global resource dependence:
//  Author:         David.Jia
//	Note:           For touch panel server task get ADC value by itself, this
//                  message will NOT be sent to server, but translated and 
//                  sent to client registered for it.
/*****************************************************************************/


LOCAL uint32 TouchPanelHit(
    uint8 **dest_ptr,       // Pointer of the response message.
    uint16 *dest_len_ptr,   // Pointer of size of the response message in uin8.
    const uint8 *src_ptr,   // Pointer of the input message. 
    uint16 src_len)         // Size of the source buffer in uint8. 
{
 
    COMM_TP_PTR     comm_ptr;
   // TPDSVR_SIG_T    *tp_sig_ptr = &s_tp_sig;
    
    comm_ptr = (COMM_TP_PTR)(src_ptr + sizeof(MSG_HEAD_T));

    // Build response to channel server
    *dest_ptr = SCI_ALLOC_APP(sizeof(MSG_HEAD_T));
    *dest_len_ptr = sizeof(MSG_HEAD_T);
    memcpy(*dest_ptr, src_ptr, sizeof(MSG_HEAD_T));
    ((MSG_HEAD_T *) (*dest_ptr))->len = *dest_len_ptr;
    
    //decode COMM_TP_T to TPDSVR_SIG_T
    s_tp_sig.SignalCode = comm_ptr->TP_type;
    s_tp_sig.x_key = comm_ptr->TP_x;
    s_tp_sig.y_key = comm_ptr->TP_y;
    
    SCI_ASSERT((TP_UP==s_tp_sig.SignalCode) || (TP_DOWN==s_tp_sig.SignalCode) || (TP_MOVE==s_tp_sig.SignalCode));/*assert verified*/
    
    //send to TP_SERVICE's client
    //SCI_TRACE_LOW:"SIMULATOR TP, type=%x(0x211:TP_DOWN, 0x212:TP_UP, 0x213:TP_MOVE), x=%d, y=%d."
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_1240_112_2_18_0_35_8_2125,(uint8*)"ddd", s_tp_sig.SignalCode, s_tp_sig.x_key, s_tp_sig.y_key);
    SCI_SendEventToClient(TP_SERVICE, s_tp_sig.SignalCode, (void *)&s_tp_sig);
    
    return SCI_TRUE;
}  

LOCAL void Register_TouchPanel_Hit_CmdRoutine( void )
{    
    //SCI_TRACE_LOW:"SC6800: Register_Sensor_Tool_CmdRoutine"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TPD_DRV_1248_112_2_18_0_35_8_2126,(uint8*)"" );
    DIAG_RegisterCmdRoutine( DIAG_TP_F,  TouchPanelHit );
}

PUBLIC void TP_InitProd(void)
{
	void TP_Init (void);
	
	Syscnt_Init();
	Register_TouchPanel_Hit_CmdRoutine();
	
	TP_Init();
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif  // End of 
 
