/******************************************************************************
 ** File Name:    ISP_service.c                                                     *
 ** Author:       Benny.zou                                                   *
 ** DATE:         6/25/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 6/25/2006     Benny.zou       Create.                                     *
 ** 8/30/2006	  patric.fang	  change									  *
 *****************************************************************************/
 
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_isp_service_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "tasks_id.h"
#include "dal_power.h" 
#include "deep_sleep.h"
#include "dal_chr.h"

#include "stdlib.h"
#include "string.h"
#include "dal_lcd.h"
#include "sensor_drv.h"
#include "isp_service_sc8800h.h"
#include "dal_dcamera.h"

#include "power_manager.h"
#include "chng_freq.h"

#include "threadx_os.h"
#include "lcd_cfg.h"
#include "dal_lcd.h"
#include "dc_cfg.h"
#include "display.h"
#include "ref_lcd.h"
#include "rotation_app.h"
#include "isp_service_cm.h"
#include "priority_system.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
#define __BD__ 							0

#define TASK_FMARK_PRI   PRI_FMARK_TASK   //28

#define max_of(a,b)  					       ((a)>(b)?(a):(b))
#define ISP_ALIGNED_BYTES				256
#define ISP_ADDRESS_ALIGNED_(a,n)		(((a) + (n) -1) / (n) * (n))
#define ISP_STRIDER_FOR_ONE_FRAME		1024


#ifndef NULL
#define NULL 							(void *)0
#endif

#define __pad(a) 						*(volatile uint32 *)(a)
#define _pard(a) 						(__pad(a))
#define _pawd(a,v) 						(__pad(a) = (v))
#define _paad(a,v) 						(__pad(a) &= (v))
#define _paod(a,v) 						(__pad(a) |= (v))
#define _pacd(a,v) 						(__pad(a) &= ~(v))
#define _pamd(a,m,v) 					do{uint32 _tmp = _pard(a); _tmp &= ~(m); _pawd((a), _tmp | ((m) & (v)));}while(0)

//#define DECLARE_SERVICE_ENTRY(s) 		ISP_SERVICE_T *s=&isp_service_state
/*lint -save -e773  */    
#define DECLARE_DRIVER_ENTRY(isp) 		ISP_DRIVER_T *isp = (ISP_DRIVER_T *)ISP_GetDriver(0,0)
/*lint -restore */

#define number_of(a)  					(sizeof(a)/sizeof(*a))
#define object_clear(a)  				do{memset((void *)(a),0,sizeof(*(a)));}while(0);
#define object_copy(a,b) 				do{memcpy((void *)(a),(void *)(b),sizeof(*(a)));}while(0)
#define PREDEFINED_PROFILES_NUMBER  	number_of(isp_predefined_profile)

#define ISP_STACK_SIZE  				(1024 * 4)
#define ISP_QUEUE_NUM   				50


//service ID
#define ISP_SERVICE_IDLE                0x00
#define ISP_SERVICE_MPEG                0x01
#define ISP_SERVICE_JPEG                0x02
#define ISP_SERVICE_PREVIEW				0x03
#define ISP_SERVICE_SCALE				0x04
#define ISP_SERVICE_REVIEW              0x07
#define ISP_SERVICE_VP					0x08
#define ISP_SERVICE_VP_ENCODE			0x09
#define ISP_SERVICE_VP_DECODE			0x10
#define ISP_SERVICE_VP_PLAYBACK         0x17

//machine state ID
//machine state ID
#define ISP_STATE_STOP          		1
#define ISP_STATE_PREVIEW       		2
#define ISP_STATE_CAPTURE 				3
#define ISP_STATE_PAUSE         		4
#define ISP_STATE_CAPTURE_STEP1 		5
#define ISP_STATE_CAPTURE_STEP2 		6
#define ISP_STATE_SCALE_DONE			7
#define ISP_STATE_MPEG					8
//isp next to when jpeg mode
#define ISP_STATE_CAPTURE_DONE	 		9
#define ISP_STATE_CAPTURE_SCALE			10
#define ISP_STATE_CAPTURE_CFA			11
#define ISP_STATE_VP					12
#define ISP_STATE_IDLETEMP				13
#define ISP_STATE_REVIEW_DONE			14
#define ISP_STATE_REVIEW_SLICE			15
#define ISP_STATE_SCALE_SLICE			16
#define ISP_STATE_SCALE 				17
#define ISP_STATE_REVIEW				18
#define ISP_STATE_CLOSED				0xFF

enum
{
   	ISP_SERVICE_PERFORMANCE_HIGH = 0,
	ISP_SERVICE_PERFORMANCE_MID,
	ISP_SERVICE_PERFORMANCE_LOW
};


typedef enum
{
	ISP_SERVICE_VT_PORTA_TIMEOUT = 0x00,
	ISP_SERVICE_TIMER_MODE_MAX
}ISP_SERVICE_TIMER_MODE_E;	

typedef enum
{
	ISP_SERVICE_PORTA = 0,
	ISP_SERVICE_PORTB,
	ISP_SERVICE_PORTC,
	ISP_SERVICE_PORT_NUM
}ISP_SERVICE_PORT_ID;


#define ISP_OPEN_EVENT						BIT_0
#define ISP_CLOSE_EVENT						BIT_1
#define ISP_DISPLAY_EVENT					BIT_2
#define ISP_MPEG_START_EVENT				BIT_3
#define ISP_VT_EVENT						BIT_4
#define ISP_REVIEW_EVENT					BIT_5
#define ISP_ROTATION_EVENT					BIT_6
#define ISP_VT_REMOTE_EVENT				    BIT_7
#define ISP_STOP_EVENT						BIT_8
#define ISP_SCALE_EVENT                     BIT_9



#define	ISP_SKIP_FRAMES						2

#define ISP_WAIT_TIME						5000 //ms
#define ISP_VT_PORTA_TIMEOUT				300 // ms
#define ISP_SERVICE_TIMEOUT				ISP_VT_PORTA_TIMEOUT

#define ISP_RESERVED_HEIGHT					32
#define JPEG_OFSSET_LENGTH					       (600*1024)
#define ISP_SCALING_WIDTH_MAX               1280

#define _ISP_SERVICE_DEBUG_

#ifdef  _ISP_SERVICE_DEBUG_
#define ISP_SERVICE_TRACE               SCI_TRACE_LOW
#else
#define ISP_SERVICE_TRACE               
#endif

typedef enum 
{
	ISP_SERVICE_OPEN_SIG 	= 1,
	ISP_SERVICE_CLOSE_SIG,
	ISP_SERVICE_PORTA_SIG,
	ISP_SERVICE_PORTB_SIG,
	ISP_SERVICE_AWBAE_SIG,
	ISP_SERVICE_SCALE_SIG,
	ISP_SERVICE_SET_PARAM_SIG,
	ISP_SERVICE_START_SIG,
	ISP_SERVICE_STOP_SIG,
	ISP_SERVICE_REMOTE_READY_SIG,	
	ISP_SERVICE_AF_OPEN_SIG,
	ISP_SERVICE_AF_CLOSE_SIG,
	ISP_SERVICE_AF_DONE_SIG = 0xafdd

}ISP_SERVICE_SIG_E;


typedef struct _display_block_t
{
    uint32                              lcd_id;
    uint32                              block_id;//block id
    ISP_RECT_T                          img_rect;
    ISP_RECT_T                          lcd_inv_rect;
    uint32                              img_fmt;
    uint32                              resolution;
    uint32                              is_enable;
}DISP_BLOCK_T;



typedef struct _isp_service_t
{
    uint32                              service;//service id;
    uint32                              state;//machine state

    uint32                              event_mask;//applied event mask

    //data format
    uint32                              input_image_format;
    uint32                              input_image_patten;
    uint32                              output_image_format;
    uint32                              output_yuv_shift;

    //the polarity of sync  signal
    uint32                              vsync_polarity;
    uint32                              hsync_polarity;
    uint32                              pclk_polarity;


    //control flag
    uint32                              is_first_frame;
    uint32                              is_display;
    uint32                              skip_frame_num;
    uint32                              display_from;
    uint32                              is_slice;
    uint32                              vp_deblock_enable;
    uint32                              vp_remote_ok;

    DISP_BLOCK_T                        display_block;//lcd block, display_block for normal use
    	
    ISP_RECT_T                          display_range;//display_range for port b normal use preview&review&mpeg&scale
    uint32                              local_review_enable;
    uint32                              local_display_enable;											 
    uint32                              display_addr[PB_NR_PINGPONG_FRAME];
    ISP_FRAME_T                         display_frame;//isp driver passed info

    ISP_SIZE_T                          encoder_size;	//for port a, jpeg&mpeg
    ISP_ADDRESS_T                       encoder_addr[PA_NR_PINGPONG_FRAME];

    ISP_SIZE_T                          input_size;   
    ISP_RECT_T                          input_range;	//for cap preview, for port c scale&review 
    ISP_ADDRESS_T                       input_addr;	//for port c
    ISP_ADDRESS_T                       output_addr;

    ISP_ROTATION_E                      display_rotation; // 0 -> 0;1 -> 90; 2 -> 180; 3 -> 270;
    uint32                              rotation_buf_addr[PB_NR_PINGPONG_FRAME];
    uint32                              *rotation_cur_buf_ptr;

    //vp remote data
    uint32                              remote_display_addr[PB_NR_PINGPONG_FRAME];
    uint32                              remote_rotation_display_addr[PB_NR_PINGPONG_FRAME];

    ISP_SIZE_T                          remote_size;
    ISP_RECT_T                          remote_range;
    DISP_BLOCK_T                        remote_display_block;//for display remote when vp mode
    	
    ISP_RECT_T                          remote_display_range;// for display remote when vp mode
    uint32                              remote_review_enable;	
    uint32                              remote_display_enable;
    ISP_ROTATION_E                      remote_display_rotation; // 0 -> 0;1 -> 90; 2 -> 180; 3 -> 270;

    ISP_FRAME_T                         *encoder_frame;
    ISP_FRAME_T                         *locked_frame;

    int                                 (*on_user_a)(void *);
    int                                 (*on_user_b)(void *);
    uint32                              total_slice_num;

}ISP_SERVICE_T;



static ISP_SERVICE_T  					isp_service_state;

/*lint -save -e773  */    
#define DECLARE_SERVICE_ENTRY(s) 		ISP_SERVICE_T *s=&isp_service_state
/*lint -restore */

LOCAL uint32 isp_busy 		= 0;
LOCAL uint32 encoder_done 	= 0;
LOCAL BOOLEAN s_is_composed_jpeg_capture = SCI_FALSE;
PUBLIC SCI_EVENT_GROUP_PTR  isp_event = NULL;
LOCAL SCI_SEMAPHORE_PTR s_isp_portc_in_sema_ptr = NULL;
LOCAL uint32 s_isp_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
LOCAL SCI_TIMER_PTR  s_vt_timer_ptr = PNULL;
LOCAL ISP_SERVICE_TIMER_MODE_E s_isp_timer_mode = ISP_SERVICE_VT_PORTA_TIMEOUT;
LOCAL volatile BOOLEAN s_b_vt_watchdog_feeded = 0;
LOCAL BOOLEAN s_b_wait_for_vt_event = 0;
LOCAL volatile BOOLEAN s_b_vt_reviewing = 0;
LOCAL volatile uint32 s_isp_performance = 0;
LOCAL BOOLEAN s_b_lost = 0,s_b_remote_lost = 0,s_b_vt_stop_arrived = 0;

LOCAL BOOLEAN s_b_frame_photo_review = 0;

LOCAL uint32 _ISP_ServiceStack[ISP_STACK_SIZE >> 2];
LOCAL uint32 _ISP_ServiceQueue[ISP_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];



LOCAL SCI_MUTEX_PTR  s_isp_port_callback_mutex[ISP_SERVICE_PORT_NUM] = {0};

LOCAL void _ISP_Display_End_Callback(uint32 address);
 
/*--------------------------- Local Data ------------------------------------*/

uint32 *s_lcdblockbuf_ptr[2] = {NULL,NULL};
uint32 g_lcdblock_enable  	= 0;
uint32 vp_remote_count = 0;
uint32 vp_local_count = 0;

/*--------------------------- External Function ---------------------------------*/
extern void PWRMNG_ISP_ClkSwtch(BOOLEAN b_clk_on);
extern ERR_LCD_E LCDC_GetBlockCtrl(LCD_ID_E lcd_id, uint32 block_id);
extern ERR_LCD_E LCD_CompoundPhoto(LCD_ID_E lcd_id,uint16 left,uint16 top,uint16 right,uint16 bottom);

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL int ISP_ServiceSendRequest(uint32 service,uint32 command,uint32 param,void * ptr);
LOCAL BOOLEAN _ISP_ServiceStartTimer(ISP_SERVICE_TIMER_MODE_E 	timer_mode, uint32 expire_value);
LOCAL void _ISP_ServiceCloseTimer(void);
LOCAL void _ISP_ServiceConfigBlock(DISP_BLOCK_T *p_disp_block,
                                   ISP_RECT_T   *p_range,
                                   ISP_RECT_T   *p_inv_range,
                                   uint32       rot_angle);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
LOCAL void  ISP_ServiceGetPortCallBackCtrl(uint32  port_id)//0,portA,1 portB,2,PortC
{
	if(port_id >= ISP_SERVICE_PORT_NUM)
		return ;

	if(s_isp_port_callback_mutex[port_id] == PNULL)
	{
		s_isp_port_callback_mutex[port_id] = SCI_CreateMutex("Callback controller",SCI_INHERIT);
		if(s_isp_port_callback_mutex[port_id] == PNULL)
			return ;
	}

	SCI_GetMutex(s_isp_port_callback_mutex[port_id], SCI_WAIT_FOREVER);

	return ;
	
}
LOCAL void  ISP_ServiceReleasePortCallBackCtrl(uint32  port_id)//0,portA,1 portB,2,PortC
{
	if(port_id >= ISP_SERVICE_PORT_NUM)
		return ;

	if(s_isp_port_callback_mutex[port_id] == PNULL)
		return ;

	SCI_PutMutex(s_isp_port_callback_mutex[port_id]);
	return ;
}



LOCAL void  ISP_LcdInvalidateRectDone(void)
{
    DECLARE_SERVICE_ENTRY(s);
    #ifndef USE_FMARK
    LCD_PutLcdCtrlSemaphore(s->display_block.lcd_id);
    #endif
    return ;
}


uint32	wait_for_sc_done = 200;

LOCAL  uint32 _ISP_EnterVTDisplay(void)
{
	uint32 status = 0;

	if(!s_isp_portc_in_sema_ptr)
		 s_isp_portc_in_sema_ptr = SCI_CreateSemaphore("Semaphore for PORTC in", 1);

	s_b_vt_reviewing = 1;
	status = SCI_GetSemaphore(s_isp_portc_in_sema_ptr, wait_for_sc_done);

	if(status)
	{
		s_b_vt_reviewing = 0;
		SCI_PutSemaphore(s_isp_portc_in_sema_ptr);
	}
	
	return status;		
	
		

}
LOCAL void _ISP_ExitVTDisplay(void)
{
	uint32 status = 0;

	if(s_isp_portc_in_sema_ptr)
	{
		status = SCI_PutSemaphore(s_isp_portc_in_sema_ptr);
	}
	s_b_vt_reviewing = 0;

}


/*****************************************************************************/
//  Description:    process port a
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
static void _ISP_ServiceOnPortA(void *p)
{
    DECLARE_SERVICE_ENTRY(s);

    if(ISP_SERVICE_IDLE != s->service)
    {
        if(s->state == ISP_STATE_VP)
        {
            s_b_vt_watchdog_feeded = 1;
            if(s_isp_performance == ISP_SERVICE_PERFORMANCE_LOW)
            {
                //SCI_TRACE_LOW:"ISP_Service: Low performance ,skip this frame"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_420_112_2_18_1_44_38_817,(uint8*)"");
                return ;
            }
            else if(s_isp_performance == ISP_SERVICE_PERFORMANCE_MID)
            {
                s_b_lost = !s_b_lost;
                if(s_b_lost) 
                {
                    //SCI_TRACE_LOW:"ISP_Service: middle performance ,skip this frame"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_428_112_2_18_1_44_38_818,(uint8*)"");
                    return ;
                }
            }
        }
        else
        {
            s->encoder_frame = (ISP_FRAME_T *)p;
        }
        ISP_ServiceSendRequest(0,ISP_SERVICE_PORTA_SIG,0,(void*)p);
    }
}

/*****************************************************************************/
//  Description:    process port a when overflow
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
static void _ISP_ServiceOnPortAOverflow(void *p)
{
    DECLARE_SERVICE_ENTRY(s);

    switch (s->state)
    {
        case ISP_STATE_CAPTURE_SCALE:
        case ISP_STATE_CAPTURE_DONE:
            
            ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_JPEG,0);
            
        break;

        case ISP_STATE_MPEG:
            
            ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_MPEG,0);
            
        break;
        
        case ISP_STATE_VP:
            
            s->is_first_frame = 1;  
            s_b_vt_watchdog_feeded = 1;
            _ISP_ServiceCloseTimer();

            ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_VP,0);
            
        break;
        
        default:

        break;
    }
           
}

/*****************************************************************************/
//  Description:    process port b
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
static void _ISP_ServiceOnPortB(void *p)
{
    DECLARE_SERVICE_ENTRY(s);
    
    if(ISP_SERVICE_IDLE != s->service)
    {
        SCI_MEMCPY(&s->display_frame,(ISP_FRAME_T *)p,sizeof(ISP_FRAME_T));    
        
        if(ISP_SERVICE_REVIEW == s->service)
        {
            SCI_SetEvent(isp_event, ISP_REVIEW_EVENT, SCI_OR);
        }
        if(s->state == ISP_STATE_VP)
        {
            _ISP_ExitVTDisplay();
        }

        //SCI_TRACE_LOW:"_ISP_ServiceOnPortB,flags = %d,p->rgbaddr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_504_112_2_18_1_44_39_819,(uint8*)"dd",((ISP_FRAME_T *)p)->flags,((ISP_FRAME_T *)p)->rgbaddr);
        ISP_ServiceSendRequest(0,ISP_SERVICE_PORTB_SIG,((ISP_FRAME_T *)p)->flags,p);

    }

}

/*****************************************************************************/
//  Description:    process port b when overflow
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
static void _ISP_ServiceOnPortBOverflow(void *p)
{
    DECLARE_SERVICE_ENTRY(s);
    switch (s->state)
    {
        case ISP_STATE_PREVIEW:
            
            ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_PREVIEW,0);
            
        break;

        case ISP_STATE_MPEG:
            
            ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_MPEG,0);
            
        break;

        case ISP_STATE_VP:
            
            s->is_first_frame = 1;  
            s_b_vt_watchdog_feeded = 1;
            _ISP_ServiceCloseTimer();

            ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_VP,0);

        break;
        
        default:
            
        break;
    }
           
}

/*****************************************************************************/
//  Description:    process port c
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
static void _ISP_ServiceOnPortC(void *p)
{
    return ;
}

/*****************************************************************************/
//  Description:    process AWB
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
static void _ISP_ServiceOnAWB(void *p)
{
   	ISP_ServiceSendRequest(0,ISP_SERVICE_AWBAE_SIG,0,0);
}



/*****************************************************************************/
//  Description:    process capture EOF
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
extern void MMU_InvalideDCACHE (void);

static void _ISP_ServiceOnCapEof(void *p)
{
    s_b_vt_watchdog_feeded = 1;
    MMU_InvalideDCACHE();
}


/*****************************************************************************/
//  Description:    get bytes per pixel of the block
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL uint32 _ISP_GetResolution(uint32 resolution)
{
	uint32 portb_fmt = 0;
	
	switch(resolution)
	{
		case 0:
			portb_fmt = ISP_FORMATB_RGB888;
			break;
	
		case 1:
			portb_fmt = ISP_FORMATB_RGB666;
			break;
			
		case 2:
			portb_fmt = ISP_FORMATB_RGB565;
			break;
			
		default:
			SCI_ASSERT(0);
			break;
	}
	
	return portb_fmt;
}
/*****************************************************************************/
//  Description:    get bytes per pixel of the block
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL uint32 _ISP_GetBytePerPixel(uint32 resolution)
{
	uint32 byte_per_pixel = 2;
	switch(resolution)
	{
		case ISP_FORMATB_RGB888:
		case ISP_FORMATB_RGB666:
			byte_per_pixel = 4;
			break;
			
		case ISP_FORMATB_RGB565:
			byte_per_pixel = 2;
			break;
		
		default:
			SCI_ASSERT(0);
			break;
	}
	
	return byte_per_pixel;
}



/*****************************************************************************/
//  Description:    get slice number when external mode
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_GetTotalSliceNum(uint32 max_width)
{
	DECLARE_SERVICE_ENTRY(s);
	uint32 i;
	
	s->total_slice_num = 1;
	
	for(i=max_width/ISP_SCALING_WIDTH_MAX; i>0; i=i/ISP_SCALING_WIDTH_MAX)
	{
		s->total_slice_num = s->total_slice_num<<1;
	}

}

/*****************************************************************************/
//  Description:    synchronize configuration
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
static void _ISP_ServiceSyncConfigure(void)
{
    DECLARE_SERVICE_ENTRY(s);
    DISP_BLOCK_T 			*block_ptr;
    ISP_RECT_T     			*rect_ptr;
    ISP_ADDRESS_T 			*porta_addr_ptr;//*portb_addr_ptr;
    uint32 					porta_frame_size;
    uint32 					portb_frame_size;
    uint32					portb_remote_frame_size;
    uint32 					pitch;
    uint32 					base_addr;
    uint32 					i;

    

    switch(s->service)
    {
        case ISP_SERVICE_PREVIEW:
            
            block_ptr = &s->display_block; 
            portb_frame_size = block_ptr->img_rect.w* block_ptr->img_rect.h*_ISP_GetBytePerPixel(block_ptr->resolution);
            
            base_addr = (uint32)DC_GetReservedBufferAddr();
            for(i=0; i< PB_NR_PINGPONG_FRAME; i++)
            {
                s->display_addr[i]= base_addr + portb_frame_size * i;
                s->rotation_buf_addr[i]= base_addr + portb_frame_size * (PB_NR_PINGPONG_FRAME + i);

            }

            if(s->rotation_buf_addr[PB_NR_PINGPONG_FRAME-1] + portb_frame_size > 
               (uint32)DC_GetReservedBufferEndAddr())
            {
                SCI_ASSERT(0);
            }

            SCI_MEMSET((void*)base_addr, 0, (uint32)(portb_frame_size*PB_NR_PINGPONG_FRAME*2));


        break;
		    
        case ISP_SERVICE_JPEG:

            porta_addr_ptr = &s->encoder_addr[0];

            base_addr = (uint32)DC_GetReservedBufferEndAddr() - ISP_STRIDER_FOR_ONE_FRAME;

            if(s->state == ISP_STATE_CAPTURE_SCALE || 
               s->state == ISP_STATE_SCALE_SLICE)
            {
                porta_frame_size = s->encoder_size.w*s->encoder_size.h + ISP_STRIDER_FOR_ONE_FRAME;
                porta_addr_ptr->yaddr = base_addr - (porta_frame_size >> 1)* 3;
                SCI_ASSERT(porta_addr_ptr->yaddr > 
                           s->input_addr.vaddr + (uint32)((s->input_range.w * s->input_range.h) >> 2));
            }
            else
            {
                porta_frame_size = s->input_range.w*s->input_range.h + ISP_STRIDER_FOR_ONE_FRAME;

                if(s->input_range.w != s->encoder_size.w && 
                   s->input_range.h != s->encoder_size.h) // need scaling up or down next time
                {
                    uint32  block_buf = (uint32)LCD_GetBlockBuffer(s->display_block.lcd_id,
                                                                   s->display_block.block_id);

                    portb_frame_size = LCD_WIDTH *LCD_HEIGHT * 2;
                    base_addr = (uint32)DC_GetReservedBufferAddr();

                    if(base_addr != block_buf)
                    {
                        SCI_MEMCPY((void*)base_addr, (void*)block_buf, portb_frame_size);
                        LCD_SetBlockBuffer(s->display_block.lcd_id, 
                                           s->display_block.block_id, 
                                           (uint32*)base_addr);
                    }
                    base_addr += portb_frame_size;


                    porta_addr_ptr->yaddr = (uint32)base_addr;
                    SCI_ASSERT(porta_addr_ptr->yaddr + (uint32)((porta_frame_size*3)>>1) 
                               <= (uint32)DC_GetReservedBufferEndAddr());
                }
                else
                {
                    porta_addr_ptr->yaddr = base_addr - (uint32)((porta_frame_size*3)>>1);     
                    SCI_ASSERT(porta_addr_ptr->yaddr >= (uint32)DC_GetReservedBufferAddr());
                }

            }

            porta_addr_ptr->yaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->yaddr,ISP_ALIGNED_BYTES);
            porta_addr_ptr->uaddr = porta_addr_ptr->yaddr + porta_frame_size;
            porta_addr_ptr->uaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->uaddr,ISP_ALIGNED_BYTES);
            porta_addr_ptr->vaddr = porta_addr_ptr->uaddr + (porta_frame_size>> 2);
            porta_addr_ptr->vaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->vaddr,ISP_ALIGNED_BYTES);					
              

        break;

			
        case ISP_SERVICE_MPEG:

            block_ptr = &s->display_block; 
            portb_frame_size = block_ptr->img_rect.w* block_ptr->img_rect.h*_ISP_GetBytePerPixel(block_ptr->resolution);

            porta_frame_size = s->encoder_size.w*s->encoder_size.h + ISP_STRIDER_FOR_ONE_FRAME;
            porta_addr_ptr = s->encoder_addr;

            base_addr = (uint32)DC_GetReservedBufferAddr();

            for(i=0; i<PB_NR_PINGPONG_FRAME; i++)
            {
                s->display_addr[i]= base_addr + portb_frame_size * i;
                s->rotation_buf_addr[i]= base_addr + portb_frame_size * (PB_NR_PINGPONG_FRAME + i);

            }
            
            base_addr += portb_frame_size * PB_NR_PINGPONG_FRAME * 2;
            
            for(i=0; i<PA_NR_PINGPONG_FRAME; i++)
            {
                porta_addr_ptr->yaddr = base_addr + (porta_frame_size >> 1) * 3 * i;
                porta_addr_ptr->yaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->yaddr,ISP_ALIGNED_BYTES);
                porta_addr_ptr->uaddr = porta_addr_ptr->yaddr + porta_frame_size;
                porta_addr_ptr->uaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->uaddr,ISP_ALIGNED_BYTES);
                porta_addr_ptr->vaddr = porta_addr_ptr->uaddr + (porta_frame_size >> 2);
                porta_addr_ptr->vaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->vaddr,ISP_ALIGNED_BYTES);					
                porta_addr_ptr++;

            }
            
            if(s->encoder_addr[PA_NR_PINGPONG_FRAME-1].vaddr+ (porta_frame_size>>2) > 
               (uint32)DC_GetReservedBufferEndAddr())
            {
                SCI_ASSERT(0);
            }

        break;    
			
        case ISP_SERVICE_SCALE:

            //port a or port b uses one frame			
            if(ISP_IMAGE_FORMAT_YUV420 == s->output_image_format)
            {
                //port a works

                porta_frame_size = s->encoder_size.w*s->encoder_size.h + ISP_STRIDER_FOR_ONE_FRAME;
                porta_addr_ptr = s->encoder_addr;

                if((s->output_addr.yaddr && (s->output_addr.yaddr % ISP_ALIGNED_BYTES == 0)) &&
                   (s->output_addr.uaddr && (s->output_addr.uaddr % ISP_ALIGNED_BYTES == 0)) && 
                   (s->output_addr.vaddr && (s->output_addr.vaddr % ISP_ALIGNED_BYTES == 0)))
                {
                    porta_addr_ptr->yaddr = s->output_addr.yaddr;
                    porta_addr_ptr->uaddr = s->output_addr.uaddr;
                    porta_addr_ptr->vaddr = s->output_addr.vaddr;                    
                }
                else
                {
                    if((s->input_size.w < s->encoder_size.w)&&(s->input_size.h < s->encoder_size.h))
                    {//do scale up

                        porta_addr_ptr->yaddr = (uint32)DC_GetReservedBufferAddr();//skip a frame to avoid overlap when capture 
                        porta_addr_ptr->yaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->yaddr,ISP_ALIGNED_BYTES);
                        porta_addr_ptr->uaddr = porta_addr_ptr->yaddr + porta_frame_size;
                        porta_addr_ptr->uaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->uaddr,ISP_ALIGNED_BYTES);
                        porta_addr_ptr->vaddr = porta_addr_ptr->uaddr + porta_frame_size/4;
                        porta_addr_ptr->vaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->vaddr,ISP_ALIGNED_BYTES);					

                    }
                    else
                    {//do scale down

                        //porta_addr_ptr->yaddr = (uint32)DC_GetReservedBufferEndAddr() - s->encoder_size.w*s->encoder_size.h*3/2;//skip a frame to avoid overlap when capture 
                        porta_addr_ptr->yaddr =s->input_addr.yaddr+ (s->input_range.w* s->input_range.h + ISP_STRIDER_FOR_ONE_FRAME)*2;
                        porta_addr_ptr->yaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->yaddr,ISP_ALIGNED_BYTES);
                        porta_addr_ptr->uaddr = porta_addr_ptr->yaddr + porta_frame_size;
                        porta_addr_ptr->uaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->uaddr,ISP_ALIGNED_BYTES);
                        porta_addr_ptr->vaddr = porta_addr_ptr->uaddr + porta_frame_size/4;
                        porta_addr_ptr->vaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->vaddr,ISP_ALIGNED_BYTES);					

                        if((porta_addr_ptr->vaddr + (porta_frame_size/4))>((uint32)DC_GetReservedBufferEndAddr()))
                        {
                            SCI_ASSERT(0);
                        }

                    }
                }

            }

        break;
			
        case ISP_SERVICE_VP:
            //port a uses three frame buffers and port b uses one frame buffer

            porta_frame_size = s->encoder_size.w*s->encoder_size.h + ISP_STRIDER_FOR_ONE_FRAME;
            porta_addr_ptr = s->encoder_addr;

            base_addr = (uint32)DC_GetReservedBufferAddr();
            for(i=0; i<PA_NR_PINGPONG_FRAME; i++)
            {
                porta_addr_ptr->yaddr = base_addr + (porta_frame_size >> 1) * 3 * i;;
                porta_addr_ptr->yaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->yaddr,ISP_ALIGNED_BYTES);
                porta_addr_ptr->uaddr = porta_addr_ptr->yaddr + porta_frame_size;
                porta_addr_ptr->uaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->uaddr,ISP_ALIGNED_BYTES);
                porta_addr_ptr->vaddr = porta_addr_ptr->uaddr + porta_frame_size/4;
                porta_addr_ptr->vaddr = ISP_ADDRESS_ALIGNED_(porta_addr_ptr->vaddr,ISP_ALIGNED_BYTES);					
                porta_addr_ptr++;

            }

            base_addr = s->encoder_addr[PA_NR_PINGPONG_FRAME-1].vaddr + porta_frame_size / 4;


            block_ptr = &s->display_block; 
            portb_frame_size = block_ptr->img_rect.w* block_ptr->img_rect.h*_ISP_GetBytePerPixel(block_ptr->resolution);

            for(i=0; i< PB_NR_PINGPONG_FRAME; i++)
            {
                s->display_addr[i] = base_addr + portb_frame_size*i;
            }
            
            s->rotation_buf_addr[0] = s->display_addr[PB_NR_PINGPONG_FRAME - 1] + portb_frame_size;
            SCI_MEMSET((void*)base_addr, 0, (uint32)(portb_frame_size * (PB_NR_PINGPONG_FRAME + 1)));


            block_ptr = &s->remote_display_block; 
            portb_remote_frame_size = block_ptr->img_rect.w*block_ptr->img_rect.h*_ISP_GetBytePerPixel(block_ptr->resolution);
            
            base_addr += portb_frame_size * (PB_NR_PINGPONG_FRAME + 1);						
            
            for(i=0; i< PB_NR_PINGPONG_FRAME; i++)
            {
                s->remote_display_addr[i] = base_addr + portb_remote_frame_size * i ;
            }
            
            s->remote_rotation_display_addr[0] = s->remote_display_addr[PB_NR_PINGPONG_FRAME - 1] + portb_remote_frame_size;
            
            SCI_MEMSET((void*)base_addr, 0, (uint32)(portb_remote_frame_size * (PB_NR_PINGPONG_FRAME + 1)));
           
            if(s->remote_rotation_display_addr[0] + portb_remote_frame_size > 
               (uint32)DC_GetReservedBufferEndAddr())
            {
                SCI_ASSERT(0);
            }


        break;
        
        case ISP_SERVICE_REVIEW:

            block_ptr = &s->display_block; 
            portb_frame_size = block_ptr->img_rect.w* block_ptr->img_rect.h*_ISP_GetBytePerPixel(block_ptr->resolution);

            base_addr = (uint32)DC_GetReservedBufferAddr();
            if(s->input_addr.yaddr >= base_addr&&
               s->input_addr.yaddr < (uint32)DC_GetReservedBufferEndAddr())
            {

                base_addr = (uint32)(s->input_addr.yaddr) -  portb_frame_size;

                s->display_addr[0]		   = base_addr;
                s->rotation_buf_addr[0]    = base_addr - portb_frame_size;     
                
                if(((uint32)DC_GetReservedBufferAddr()) > s->rotation_buf_addr[PB_NR_PINGPONG_FRAME-1]) 
                {
                    SCI_ASSERT(0);
                }
				

            }
            else
            {
                for(i = 0; i < PB_NR_PINGPONG_FRAME; i++)
                {
                    s->display_addr[i]= base_addr + portb_frame_size * i;
                }
                s->rotation_buf_addr[0]= s->display_addr[PB_NR_PINGPONG_FRAME-1] + portb_frame_size;
                
                if((uint32)DC_GetReservedBufferEndAddr() < 
                   s->rotation_buf_addr[0] + portb_frame_size) 
                {
                    SCI_ASSERT(0);
                }
            }
            
            SCI_MEMSET((void*)s->display_addr[0], 0, portb_frame_size);
            SCI_MEMSET((void*)s->rotation_buf_addr[0], 0,portb_frame_size);		
            
        break;
			
        default:

        break;
	}


}

/*****************************************************************************/
//  Description:    backup LCD block information
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_Backuplcdblockinfo(LCD_ID_E lcd_id,uint32 blk_num, uint32 index)
{
	DECLARE_SERVICE_ENTRY(s);
	
	//LCD_GetConfigBlock(lcd_id,blk_num, &g_lcdblockcfg);
	if(s_lcdblockbuf_ptr[index] == NULL)
	{	
    	s_lcdblockbuf_ptr[index] = (uint32 *)LCD_GetBlockBuffer(lcd_id, blk_num);
   	
    	//ISP_SERVICE_TRACE:"ISP: lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = 0x%x"
    	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_974_112_2_18_1_44_40_820,(uint8*)"ddd",lcd_id,blk_num,s_lcdblockbuf_ptr[index]);

    }
    else
    {
    	if(lcd_id != s->display_block.lcd_id || blk_num != s->display_block.block_id)
    	{
	    	LCD_SetBlockBuffer(s->display_block.lcd_id, s->display_block.block_id,s_lcdblockbuf_ptr[index]);
	
	    	s_lcdblockbuf_ptr[index] = (uint32 *)LCD_GetBlockBuffer(lcd_id, blk_num);
    	
			//ISP_SERVICE_TRACE:"ISP: lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = 0x%x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_985_112_2_18_1_44_40_821,(uint8*)"ddd",lcd_id,blk_num,s_lcdblockbuf_ptr[index]);
  		}
  		else
  		{
  			//ISP_SERVICE_TRACE:"_ISP_Backuplcdblockinfo same do nothing"
  			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_989_112_2_18_1_44_40_822,(uint8*)"");
  		}

    }
    
}

/*****************************************************************************/
//  Description:    restore LCD block information
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_Restorelcdblockinfo(LCD_ID_E lcd_id,uint32 blk_num,uint32 index)
{
	//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_Restorelcdblockinfo, lcd_id = %d, blk_num = %d, s_lcdblockbuf_ptr = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1002_112_2_18_1_44_40_823,(uint8*)"ddd",lcd_id,blk_num,s_lcdblockbuf_ptr[index]);

	if(s_lcdblockbuf_ptr[index] != NULL)
	{
		LCD_SetBlockBuffer(lcd_id,blk_num,s_lcdblockbuf_ptr[index]);
		s_lcdblockbuf_ptr[index] = NULL;
	}
}

/*****************************************************************************/
//  Description:    check preview paramter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL uint32 _ISP_CheckPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
	BLOCKCFG_T		block_cfg;
		
	//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_CheckPreviewParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1019_112_2_18_1_44_40_824,(uint8*)"");
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(((param_ptr->input_size.w%2)||(param_ptr->input_range.w%2)||(param_ptr->disp_range.w%2)))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: All width must be 2 pixel align"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1025_112_2_18_1_44_40_825,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if( param_ptr->disp_lcd_id >= MAX_LCD_ID)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: Invalid lcd id"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1031_112_2_18_1_44_40_826,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if(param_ptr->disp_block_id >5)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: Invalid lcd id or block id"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1037_112_2_18_1_44_40_827,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	
	if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
		(param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: Input range is out of input size"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1045_112_2_18_1_44_40_828,(uint8*)"");
		SCI_ASSERT(0);
	}
	
//	LCD_GetConfigBlock(param_ptr->disp_lcd_id, param_ptr->disp_block_id, &block_cfg);

	SCI_ASSERT(param_ptr->disp_rotation < ISP_ROTATION_MAX);

	return SCI_SUCCESS;

}


/*****************************************************************************/
//  Description:    check jpeg capture paramter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL uint32 _ISP_CheckJpegParam(ISP_JPEG_PARAM_T * param_ptr)
{
	//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_CheckJpegParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1064_112_2_18_1_44_40_829,(uint8*)"");
	
	SCI_ASSERT(SCI_NULL!=param_ptr);

	if((param_ptr->input_size.w%2)||(param_ptr->input_range.w%2)||(param_ptr->output_size.w%2))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: All width must be 2 pixel align"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1070_112_2_18_1_44_40_830,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
		(param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: Input range is out of input size"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1077_112_2_18_1_44_40_831,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	return SCI_SUCCESS;
}


/*****************************************************************************/
//  Description:    check mpeg record paramter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL uint32 _ISP_CheckMpegParam(ISP_MPEG_PARAM_T * param_ptr)
{
	//DECLARE_SERVICE_ENTRY(s);
	BLOCKCFG_T		block_cfg;
	
	//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_CheckMpegParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1094_112_2_18_1_44_40_832,(uint8*)"");
	
	SCI_ASSERT(SCI_NULL!=param_ptr);

	if((param_ptr->input_size.w%2)||(param_ptr->input_range.w%2)||(param_ptr->disp_range.w%2)||(param_ptr->encode_size.w%2))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: All width must be 2 pixel align"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1100_112_2_18_1_44_40_833,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if(ISP_DISPLAY_NONE != param_ptr->disp_lcd_id)
	{
		if(param_ptr->disp_lcd_id >= MAX_LCD_ID)
		{
			//SCI_TRACE_LOW:"ISP_SERVICE: Invalid lcd id"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1108_112_2_18_1_44_40_834,(uint8*)"");
			SCI_ASSERT(0);
		}
		
		if(param_ptr->disp_block_id >5)
		{
			//SCI_TRACE_LOW:"ISP_SERVICE: Invalid lcd id or block id"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1114_112_2_18_1_44_40_835,(uint8*)"");
			SCI_ASSERT(0);
		}
		
		if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
			(param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
		{
			//SCI_TRACE_LOW:"ISP_SERVICE: Input range is out of input size"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1121_112_2_18_1_44_40_836,(uint8*)"");
			SCI_ASSERT(0);
		}
		
//		LCD_GetConfigBlock(param_ptr->disp_lcd_id, param_ptr->disp_block_id, &block_cfg);

		
		SCI_ASSERT(param_ptr->disp_rotation < ISP_ROTATION_MAX);

	}
	
	return SCI_SUCCESS;

}

/*****************************************************************************/
//  Description:    check jpeg review paramter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL uint32 _ISP_CheckReviewParam(ISP_REVIEW_PARAM_T * param_ptr)
{
	BLOCKCFG_T		block_cfg;
	
	//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_CheckReviewParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1144_112_2_18_1_44_40_837,(uint8*)"");
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(param_ptr->input_format != ISP_IMAGE_FORMAT_YUV420 && param_ptr->input_format != ISP_IMAGE_FORMAT_RGB565 
	&& param_ptr->input_format != ISP_IMAGE_FORMAT_RGB666 &&  param_ptr->input_format != ISP_IMAGE_FORMAT_RGB888)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: input_format = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1151_112_2_18_1_44_40_838,(uint8*)"d",param_ptr->input_format);
		SCI_ASSERT(0);
	}
	
	if((param_ptr->input_size.w%2)||(param_ptr->input_range.w%2)||(param_ptr->disp_range.w%2))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: All width must be 2 pixel align"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1157_112_2_18_1_44_40_839,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if(param_ptr->input_range.x%4)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: input start x must be 4 pixel align when review"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1163_112_2_18_1_44_40_840,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if(param_ptr->disp_lcd_id >= MAX_LCD_ID)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: Invalid lcd id"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1169_112_2_18_1_44_40_841,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if(param_ptr->disp_block_id >5)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: Invalid lcd id or block id"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1175_112_2_18_1_44_40_842,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
		(param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: Input range is out of input size"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1182_112_2_18_1_44_40_843,(uint8*)"");
		SCI_ASSERT(0);
	}
	
//	LCD_GetConfigBlock(param_ptr->disp_lcd_id, param_ptr->disp_block_id, &block_cfg);
	
	SCI_ASSERT(param_ptr->disp_rotation < ISP_ROTATION_MAX);

	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    check scale paramter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL uint32 _ISP_CheckScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
	//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_CheckScaleParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1199_112_2_18_1_44_40_844,(uint8*)"");
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(param_ptr->input_format != ISP_IMAGE_FORMAT_YUV420 && param_ptr->input_format != ISP_IMAGE_FORMAT_RGB565 
	&& param_ptr->input_format != ISP_IMAGE_FORMAT_RGB666 &&  param_ptr->input_format != ISP_IMAGE_FORMAT_RGB888)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: input_format = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1206_112_2_18_1_44_40_845,(uint8*)"d",param_ptr->input_format);
		SCI_ASSERT(0);
	}
	
	if(param_ptr->output_format != ISP_IMAGE_FORMAT_YUV420 && param_ptr->output_format != ISP_IMAGE_FORMAT_RGB565 
	&& param_ptr->output_format != ISP_IMAGE_FORMAT_RGB666 &&  param_ptr->output_format != ISP_IMAGE_FORMAT_RGB888)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: input_format = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1213_112_2_18_1_44_40_846,(uint8*)"d",param_ptr->input_format);
		SCI_ASSERT(0);
	}
	
	if((param_ptr->input_size.w%2)||(param_ptr->input_range.w%2)||(param_ptr->output_size.w%2))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: All width must be 2 pixel align"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1219_112_2_18_1_44_40_847,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if(param_ptr->input_range.x%4)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: input start x must be 4 pixel align when scale"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1225_112_2_18_1_44_40_848,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	if((param_ptr->input_size.w < (param_ptr->input_range.x + param_ptr->input_range.w)) ||
		(param_ptr->input_size.h < (param_ptr->input_range.y + param_ptr->input_range.h)))
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: Input range is out of input size"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1232_112_2_18_1_44_40_849,(uint8*)"");
		SCI_ASSERT(0);
	}
	
	return SCI_SUCCESS; 
	
}

/*****************************************************************************/
//  Description:    copy preview paramter to isp service struct
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_SetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
	DECLARE_SERVICE_ENTRY(s);
	DISP_BLOCK_T *disp_block_ptr;
	BLOCKCFG_T		block_cfg;
	SENSOR_EXP_INFO_T	sensor_info;
	LCD_INFO_T 			lcd_info_ptr = {0};
    
	s->state = ISP_STATE_IDLETEMP;
    s->service = ISP_SERVICE_PREVIEW;
    	
	//SCI_TRACE_LOW:"_ISP_SetPreviewParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1255_112_2_18_1_44_40_850,(uint8*)"");
	
	sensor_info = *Sensor_GetInfo();
	
	s->input_image_format	= sensor_info.image_format;
	s->input_image_patten	= sensor_info.image_pattern;
	s->vsync_polarity 		= sensor_info.vsync_polarity;
	s->hsync_polarity 		= sensor_info.hsync_polarity;
	s->pclk_polarity  		= sensor_info.pclk_polarity;
	
	SCI_MEMCPY((void*)&(s->input_size), (void*)&(param_ptr->input_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY((void*)&(s->input_range), (void*)&(param_ptr->input_range), sizeof(ISP_RECT_T));
	SCI_MEMCPY((void*)&(s->display_range), (void*)&(param_ptr->disp_range), sizeof(ISP_RECT_T));
	if(param_ptr->disp_range.w % 8)
	{
		s->display_range.w = (param_ptr->disp_range.w + 7)&(~7);//8 pixels aligned
	}

	_ISP_Backuplcdblockinfo(param_ptr->disp_lcd_id,param_ptr->disp_block_id,0);

	s->display_rotation = param_ptr->disp_rotation;
	
	s->encoder_size.w = 0;
	s->encoder_size.h = 0;
	
	disp_block_ptr = &(s->display_block);
	disp_block_ptr->lcd_id  	= param_ptr->disp_lcd_id;
	disp_block_ptr->block_id 	= param_ptr->disp_block_id;

    _ISP_ServiceConfigBlock(disp_block_ptr,&s->display_range,&param_ptr->lcd_rect,s->display_rotation);

}

/*****************************************************************************/
//  Description:    start hw to preview
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_StartPreview()
{
	DECLARE_SERVICE_ENTRY(s);
	DECLARE_DRIVER_ENTRY(isp);
	ISP_RECT_T     			rect;
	ISP_SIZE_T     			size;
	uint32         				param;


	//SCI_TRACE_LOW:"ISP: _ISP_StartPreview"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1301_112_2_18_1_44_40_851,(uint8*)"");

	//配置PORT A和PORT B的地址
	_ISP_ServiceSyncConfigure();

	param = ISP_MODE_PREVIEW;
	isp->ioctl(ISP_CFG_MODE ,&param);

	if(s->input_image_format == ISP_IMAGE_FORMAT_YUV422)
	{
		param = ISP_FORMAT_YUV422;
		isp->ioctl(ISP_CFG_INPUT_FORMAT,&param);
		param = s->input_image_patten;
		isp->ioctl(ISP_CFG_YUV_SEQUENCE ,&param);

		param = 1;
		isp->ioctl(ISP_CFG_Y_SHIFT128,&param);
		isp->ioctl(ISP_CFG_UV_SHIFT128,&param);
	}
	else
	{

	}

	param = s->display_block.resolution;
	isp->ioctl(ISP_CFG_PORTB_FORMAT,&param);

	param = 30;
	isp->ioctl(ISP_CFG_INPUT_FPS ,&param);

	size.w = s->input_size.w;
	size.h = s->input_size.h;
	isp->ioctl(ISP_CFG_INPUT_SIZE  ,&size);

	param = ISP_SKIP_FRAMES;
	//	    param = 0;
	isp->ioctl(ISP_CFG_PRE_SKIP_FRAMES  ,&param);


	param = s->hsync_polarity;
	isp->ioctl(ISP_CFG_HSYNC_POLARITY ,&param);


	param = s->vsync_polarity;
	isp->ioctl(ISP_CFG_VSYNC_POLARITY ,&param);

	param = s->pclk_polarity;
	isp->ioctl(ISP_CFG_PCLK_POLARITY ,&param);

	//input shift info, this param may come from sensor spec
	param = 1;
	isp->ioctl(ISP_CFG_PORTA_Y_SHIFT128,&param);
	isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);


	param = ISP_TRAM_IN_CENTER;
	isp->ioctl(ISP_CFG_TRIM_POSITION,&param);

	rect.x = 0;
	rect.y = 0;
	rect.w = s->display_range.w;
	rect.h = s->display_range.h;
	isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);
    

    param = 0;
	isp->ioctl(ISP_CFG_DISP_BUFF_OFFSET, &param) ;  
	rect.x = s->input_range.x;
	rect.y = s->input_range.y;
	rect.w = s->input_range.w;
	rect.h = s->input_range.h;
	isp->ioctl(ISP_CFG_CAP_INPUT_RANGE,&rect);

	{		
		uint32 stream[2+PB_NR_PINGPONG_FRAME];
		uint32 i;

		stream[0] = ISP_PORTB;
		stream[1] = PB_NR_PINGPONG_FRAME;

		for(i=0; i<PB_NR_PINGPONG_FRAME; i++)
		{
			stream[2+i] = s->display_addr[i];
		}
		isp->ioctl(ISP_CFG_FRAME_MANUAL ,stream);
	}   

	isp->regevent(ISP_PORTA,_ISP_ServiceOnPortA);
	isp->regevent(ISP_PORTB,_ISP_ServiceOnPortB);
	isp->regevent(ISP_CAPSTART,_ISP_ServiceOnCapEof);
	isp->regevent(ISP_PORTBOVF,_ISP_ServiceOnPortBOverflow);


	param = ISP_SCALE1_ENABLE;
	isp->ioctl(ISP_CFG_SC1_ENABLE   ,&param);

	param = 0;
	isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);
	//LCDC_RegisterInterrupt(2,ISP_LcdInvalidateRectDone);
	isp->start();

	s->state = ISP_STATE_PREVIEW;
	s->is_display	=	1;			   
}

LOCAL void _ISP_ServiceConvertCoor( LCD_ID_E lcd_id, ISP_RECT_T *src_ptr, ISP_RECT_T *dst_ptr,ISP_ROTATION_E rotate_ang)
{
    LCD_INFO_T lcd_info={0x00};

    LCD_GetInfo(lcd_id, &lcd_info);


    //ISP_SERVICE_TRACE:"_ISP_ServiceConvertCoor,src x y w h {%d %d %d %d},rotate_ang %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1418_112_2_18_1_44_40_852,(uint8*)"ddddd", src_ptr->x, src_ptr->y, src_ptr->w, src_ptr->h,rotate_ang);

    switch(rotate_ang)
    {
        case ISP_ROTATION_0:

            SCI_MEMCPY(dst_ptr, src_ptr, sizeof(ISP_RECT_T));

        break;
        
        case ISP_ROTATION_90:

            dst_ptr->w = src_ptr->h;
            dst_ptr->h = src_ptr->w;
            dst_ptr->y = src_ptr->x;
            dst_ptr->x = lcd_info.lcd_width - src_ptr->h - src_ptr->y;


        break;

        case ISP_ROTATION_180:

            dst_ptr->x = lcd_info.lcd_width - src_ptr->w - src_ptr->x;
            dst_ptr->y = lcd_info.lcd_height - src_ptr->h - src_ptr->y;
            dst_ptr->w =  src_ptr->w;
            dst_ptr->h =  src_ptr->h;

        break;

        case ISP_ROTATION_270:

            dst_ptr->w = src_ptr->h;
            dst_ptr->h = src_ptr->w;
            dst_ptr->x = src_ptr->y;
            dst_ptr->y = lcd_info.lcd_height - src_ptr->w - src_ptr->x;

        break;

        default:

            SCI_ASSERT(0);

        break;

    }


    //ISP_SERVICE_TRACE:"_ISP_ServiceConvertCoor,dst x y w h {%d %d %d %d}"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1469_112_2_18_1_44_41_853,(uint8*)"dddd", dst_ptr->x, dst_ptr->y, dst_ptr->w, dst_ptr->h);

    return;

}

LOCAL void _ISP_ServiceConfigBlock(DISP_BLOCK_T *p_disp_block,
                                   ISP_RECT_T   *p_range,
                                   ISP_RECT_T   *p_inv_range,
                                   uint32       rot_angle)
{
	DECLARE_SERVICE_ENTRY(s);
    BLOCKCFG_T              blk_cfg = {0};
    LCD_INFO_T              lcd_info_t = {0};
    uint32                  blk_buffer = 0;


    SCI_MEMCPY((void*)&p_disp_block->lcd_inv_rect,(void*)p_inv_range,sizeof(ISP_RECT_T));
    SCI_MEMCPY((void*)&p_disp_block->img_rect,(void*)p_range,sizeof(ISP_RECT_T));  

    if(rot_angle == ISP_ROTATION_90 ||
       rot_angle == ISP_ROTATION_270) // to get the right size of target image
    {
       p_range->w = p_disp_block->img_rect.h;
       p_range->h = p_disp_block->img_rect.w;
    }
/*
    
    switch(s->service)
    {
        case ISP_SERVICE_PREVIEW:
        case ISP_SERVICE_MPEG:
            
            if(p_disp_block->block_id != 0) // come from vt preview
            {
                if(rot_angle == ISP_ROTATION_90 ||
                   rot_angle == ISP_ROTATION_270)
                {
                    p_disp_block->start_x = p_range->y;
                    p_disp_block->start_y = p_range->x;                
                    p_disp_block->width = p_range->h;
                    p_disp_block->height = p_range->w;
                }
                else
                {
                    p_disp_block->start_x = p_range->x;
                    p_disp_block->start_y = p_range->y;                
                    p_disp_block->width = p_range->w;
                    p_disp_block->height = p_range->h;
                }
                p_range->x = 0;
                p_range->y = 0;     
            }
            else 
            {
                p_disp_block->start_x = 0;
                p_disp_block->start_y = 0;       
                p_disp_block->width = LCD_WIDTH;
                p_disp_block->height = LCD_HEIGHT;
            }

        break;

        default:
            
            if(rot_angle == ISP_ROTATION_90 ||
               rot_angle == ISP_ROTATION_270)
            {
                p_disp_block->start_x = p_range->y;
                p_disp_block->start_y = p_range->x;                
                p_disp_block->width = p_range->h;
                p_disp_block->height = p_range->w;
            }
            else
            {
                p_disp_block->start_x = p_range->x;
                p_disp_block->start_y = p_range->y;                
                p_disp_block->width = p_range->w;
                p_disp_block->height = p_range->h;
            }
            
        break;
    }
*/   
	//ISP_SERVICE_TRACE:"ISP_SERVICE: block id %d p_disp_block->img_rect.x,y,w,h {%d,%d,%d,%d}"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1557_112_2_18_1_44_41_854,(uint8*)"ddddd",p_disp_block->block_id,p_disp_block->img_rect.x,p_disp_block->img_rect.y,p_disp_block->img_rect.w,p_disp_block->img_rect.h);
	
    blk_cfg.start_x = p_disp_block->img_rect.x;
    blk_cfg.start_y = p_disp_block->img_rect.y;
    blk_cfg.end_x = p_disp_block->img_rect.x + p_disp_block->img_rect.w - 1;
    blk_cfg.end_y = p_disp_block->img_rect.y + p_disp_block->img_rect.h - 1;       
    blk_cfg.width = p_disp_block->img_rect.w;
    blk_cfg.colorkey_en = 0;
    blk_cfg.alpha_sel = 1;
    blk_cfg.alpha = 0xFF;
    blk_cfg.resolution = LCD_RESOLUTION_RGB565;
    blk_cfg.type = 1;

    p_disp_block->resolution 	= _ISP_GetResolution(blk_cfg.resolution);
    
    LCD_ConfigBlock(p_disp_block->lcd_id,p_disp_block->block_id, blk_cfg);

    blk_buffer = (uint32)DC_GetJpegTmpBuffer();
    if(blk_buffer)
    {
        SCI_MEMSET((void*)blk_buffer, 0, (uint32)(p_disp_block->img_rect.w * p_disp_block->img_rect.h * 2));
        LCD_SetBlockBuffer(p_disp_block->lcd_id,p_disp_block->block_id,(uint32*)blk_buffer);
        LCD_EnableBlock(p_disp_block->lcd_id,p_disp_block->block_id);        
        p_disp_block->is_enable = 1;
    }
    else
    {
        LCD_DisableBlock(p_disp_block->lcd_id,p_disp_block->block_id);
        p_disp_block->is_enable = 0;
    }
}

LOCAL void _ISP_ServiceCloseBlock(void)
{
	DECLARE_SERVICE_ENTRY(s);


    if(s->display_block.is_enable == 1)
    {
        LCD_DisableBlock(s->display_block.lcd_id,s->display_block.block_id);
        s->display_block.is_enable = 0;
    }

    if(s->remote_display_block.is_enable == 1)
    {
        LCD_DisableBlock(s->remote_display_block.lcd_id,s->remote_display_block.block_id);
        s->remote_display_block.is_enable = 0;
    }

    
}


/*****************************************************************************/
//  Description:    copy capture paramter to isp service struct
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_SetJpegParam(ISP_JPEG_PARAM_T * param_ptr)
{
	DECLARE_SERVICE_ENTRY(s);
	SENSOR_EXP_INFO_T	sensor_info;
	
	//SCI_TRACE_LOW:"_ISP_SetJpegParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1619_112_2_18_1_44_41_855,(uint8*)"");
	
	s->state = ISP_STATE_IDLETEMP;
	
	SCI_MEMCPY((void*)&(s->input_size), (void*)&(param_ptr->input_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY((void*)&(s->input_range), (void*)&(param_ptr->input_range), sizeof(ISP_RECT_T));
	SCI_MEMCPY((void*)&(s->encoder_size), (void*)&(param_ptr->output_size), sizeof(ISP_SIZE_T));
	
	sensor_info = *Sensor_GetInfo();
	
	s->input_image_format	= sensor_info.image_format;
	s->input_image_patten	= sensor_info.image_pattern;
	s->vsync_polarity 		= sensor_info.vsync_polarity;
	s->hsync_polarity 		= sensor_info.hsync_polarity;
	s->pclk_polarity  		= sensor_info.pclk_polarity;
	
	s->output_yuv_shift = param_ptr->yuv_shift;
	s->skip_frame_num	= param_ptr->skip_frame_num;

	s->service = ISP_SERVICE_JPEG;
}

/*****************************************************************************/
//  Description:    start hw to capture one yuv420 frame
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_StartJpeg()
{
	DECLARE_SERVICE_ENTRY(s);
	DECLARE_DRIVER_ENTRY(isp);
	unsigned long         		param;
	ISP_RECT_T     			rect;
	ISP_SIZE_T     			size;

	//isp->stop();
	//SCI_TRACE_LOW:"_ISP_StartJpeg"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1654_112_2_18_1_44_41_856,(uint8*)"");
	_ISP_ServiceSyncConfigure();

	if(s->input_image_format == ISP_IMAGE_FORMAT_YUV422)
	{
		param = ISP_FORMAT_YUV422;
		isp->ioctl(ISP_CFG_INPUT_FORMAT,&param);
		param = s->input_image_patten;
		isp->ioctl(ISP_CFG_YUV_SEQUENCE ,&param);
	}
	else
	{

	}

	param = s->vsync_polarity;
	isp->ioctl(ISP_CFG_VSYNC_POLARITY ,&param);

	param = s->hsync_polarity;
	isp->ioctl(ISP_CFG_HSYNC_POLARITY ,&param);

	param = s->pclk_polarity;
	isp->ioctl(ISP_CFG_PCLK_POLARITY ,&param);

	//	param = s->skip_frame_num+2; ///baggiohe 20090702
	param = s->skip_frame_num;
	isp->ioctl(ISP_CFG_PRE_SKIP_FRAMES  ,&param);

	param = s->output_yuv_shift;
	isp->ioctl(ISP_CFG_PORTA_Y_SHIFT128,&param);
	isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);

	//this part may changed by different sensor 
	param = 1;
	isp->ioctl(ISP_CFG_Y_SHIFT128,&param);
	isp->ioctl(ISP_CFG_UV_SHIFT128,&param);


	{
		unsigned long stream[16];
		ISP_ADDRESS_T *a;

		stream[0] = ISP_PORTA;
		stream[1] = 3;

		a = s->encoder_addr;
		stream[2] = a->yaddr;
		stream[3] = a->uaddr;
		stream[4] = a->vaddr;a++;
		stream[5] = a->yaddr;
		stream[6] = a->uaddr;
		stream[7] = a->vaddr;a++;
		stream[8] = a->yaddr;
		stream[9] = a->uaddr;
		stream[10]= a->vaddr;
		isp->ioctl(ISP_CFG_FRAME_MANUAL ,stream);
	}


	param = 0;
	isp->ioctl(ISP_CFG_SC1_ENABLE   ,&param);

	param = 0;
	isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);


	param = ISP_TRAM_IN_CENTER;
	isp->ioctl(ISP_CFG_TRIM_POSITION,&param);

	if((ISP_IMAGE_FORMAT_RAW == s->input_image_format)&&(s->input_range.w > 1280))
	{
		param = 0;
		isp->ioctl(ISP_CFG_PORTA_Y_SHIFT128,&param);
		param = 0;
		isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);
		param = ISP_MODE_JPEG2;
	}
	else
	{
		param = 1;
		isp->ioctl(ISP_CFG_PORTA_Y_SHIFT128,&param);
		param = 1;
		isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);
		param = ISP_MODE_JPEG1;
	}
	isp->ioctl(ISP_CFG_MODE ,&param);//driver will do mode changes

	if((s->input_range.w == s->encoder_size.w && s->input_range.h == s->encoder_size.h)&&
		((ISP_IMAGE_FORMAT_YUV422 ==  s->input_image_format)||
		(ISP_IMAGE_FORMAT_RAW ==  s->input_image_format && s->input_range.w <= 1280)))
	{
		//two cases require only one step:jpeg 1//1  . yuv, no scale, //2 . raw, no scale and input width <= 1280
		size.w = s->input_size.w;
		size.h = s->input_size.h;
		isp->ioctl(ISP_CFG_INPUT_SIZE  ,&size);

		rect.x = 0;
		rect.y = 0;
		rect.w = s->encoder_size.w;
		rect.h = s->encoder_size.h;
		isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);


		rect.x = s->input_range.x;
		rect.y = s->input_range.y;
		rect.w = s->input_range.w;
		rect.h = s->input_range.h;
		isp->ioctl(ISP_CFG_CAP_INPUT_RANGE,&rect);

		s->state = ISP_STATE_CAPTURE_DONE;
	}
	else
	{
		//two case require two steps: jpeg1 + external mode 1
		size.w = s->input_size.w;
		size.h = s->input_size.h;
		isp->ioctl(ISP_CFG_INPUT_SIZE  ,&size);


		rect.x = 0;
		rect.y = 0;
		rect.w = s->input_range.w;
		rect.h = s->input_range.h;
		isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);

		rect.x = s->input_range.x;
		rect.y = s->input_range.y;
		rect.w = s->input_range.w;
		rect.h = s->input_range.h;
		isp->ioctl(ISP_CFG_CAP_INPUT_RANGE,&rect);
	}

	if(ISP_IMAGE_FORMAT_YUV422 ==  s->input_image_format)
	{
		if(s->input_range.w != s->encoder_size.w || s->input_range.h != s->encoder_size.h)
		{
			s->state = ISP_STATE_CAPTURE_SCALE;
		}
		else
		{
			s->state = ISP_STATE_CAPTURE_DONE;
		}
		}
		else
		{
		if(s->input_range.w > 1280)
		{
			s->state = ISP_STATE_CAPTURE_CFA;
		}
		else
		{
		if(s->input_range.w != s->encoder_size.w || s->input_range.h != s->encoder_size.h)
		{
			s->state = ISP_STATE_CAPTURE_SCALE;
		}
		else
		{
			s->state = ISP_STATE_CAPTURE_DONE;
		}
		}

	}

	isp->regevent(ISP_PORTAOVF,_ISP_ServiceOnPortAOverflow);
	isp->regevent(ISP_PORTA,_ISP_ServiceOnPortA);
	isp->start();
}

/*****************************************************************************/
//  Description:    copy scale paramter to isp service struct
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_SetScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
	DECLARE_SERVICE_ENTRY(s);
	
	//SCI_TRACE_LOW:"_ISP_SetScaleParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1830_112_2_18_1_44_41_857,(uint8*)"");
	
	if(s->state != ISP_STATE_CAPTURE_SCALE)
	{
		s->state = ISP_STATE_IDLETEMP;
	}
	s->input_image_format = param_ptr->input_format;
	
	if(param_ptr->output_format != ISP_IMAGE_FORMAT_YUV420 && param_ptr->output_format != ISP_IMAGE_FORMAT_RGB565 
	&& param_ptr->output_format != ISP_IMAGE_FORMAT_RGB666 &&  param_ptr->output_format != ISP_IMAGE_FORMAT_RGB888)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: output_format = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1841_112_2_18_1_44_41_858,(uint8*)"d",param_ptr->output_format);
		SCI_ASSERT(0);
	}
	else
	{
		s->output_image_format = param_ptr->output_format;
	}
	
	SCI_MEMCPY((void*)&(s->input_size), (void*)&(param_ptr->input_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY((void*)&(s->input_range), (void*)&(param_ptr->input_range), sizeof(ISP_RECT_T));
	SCI_MEMCPY((void*)&(s->input_addr),(void*)&(param_ptr->input_addr), sizeof(ISP_ADDRESS_T));
	SCI_MEMCPY((void*)&(s->output_addr),(void*)&(param_ptr->output_addr), sizeof(ISP_ADDRESS_T));
	
	if(param_ptr->output_format == ISP_IMAGE_FORMAT_YUV420)
	{
		SCI_MEMCPY(&(s->encoder_size), &(param_ptr->output_size), sizeof(ISP_SIZE_T));
	}
	else
	{
		s->display_range.x = 0;
		s->display_range.y = 0;
		s->display_range.w = param_ptr->output_size.w;
		s->display_range.h = param_ptr->output_size.h;
	}
	
	s->output_yuv_shift = param_ptr->yuv_shift;

	s->service = ISP_SERVICE_SCALE;
}

/*****************************************************************************/
//  Description:    start hw to scale one frame
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_StartScale()
{
	DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);

    ISP_RECT_T      rect;
    ISP_SIZE_T     	size;
    ISP_FRAME_T		addr = {0};
    uint32      param;
   	//SCI_TRACE_LOW:"_ISP_StartScale"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_1884_112_2_18_1_44_41_859,(uint8*)"");

    _ISP_ServiceSyncConfigure();

	//set input format
	switch(s->input_image_format)
	{
	case ISP_IMAGE_FORMAT_YUV420:
		param = ISP_FORMAT_YUV420;
		break;
	
	case ISP_IMAGE_FORMAT_RAW:
		param = ISP_FORMAT_RAWRGB;
		break;
	
	case ISP_IMAGE_FORMAT_RGB888:
	case ISP_IMAGE_FORMAT_RGB666:
		param = ISP_FORMAT_FULLRGB888;
		break;
		
	case ISP_IMAGE_FORMAT_RGB565:
		param = ISP_FORMAT_FULLRGB565;
		break;
	}
    isp->ioctl(ISP_CFG_INPUT_FORMAT,&param);
	

	
	if(s->output_image_format == ISP_IMAGE_FORMAT_YUV420)
	{
    	
	    	if(s->input_image_format == ISP_IMAGE_FORMAT_RAW)
	    	{
	    		
	    		
	    	}
	    	else
	    	{
	    		rect.x = 0;
		    	rect.y = 0;
		    	rect.w = s->encoder_size.w;
		    	rect.h = s->encoder_size.h;
		    	
		    	isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);
	    		    		
	    		param = ISP_MODE_EXTERNAL1;
	    	}
	    	isp->regevent(ISP_PORTA,_ISP_ServiceOnPortA);
    	

	    
	    rect.x = 0;
	    rect.y = 0;
	    rect.w = 0;
	    rect.h = 0;
	    isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);
        
        //set port a output addr
	    {   
	           uint32 stream[2+PA_NR_PINGPONG_FRAME*3];
	           uint32 i;
	           ISP_ADDRESS_T *a; 
	        	
	           stream[0] = ISP_PORTA;
	           stream[1] = 3;
	            
	            a = s->encoder_addr;
	            for(i=0; i<PA_NR_PINGPONG_FRAME; i++)
	            {
		            	stream[2+i*3] = a->yaddr;
		            	stream[3+i*3] = a->uaddr;
		            	stream[4+i*3] = a->vaddr;
		            	
		            	a++;
	            }

	            isp->ioctl(ISP_CFG_FRAME_MANUAL ,stream);	    
		}
    	
       }
       else
       {
    	
	    	isp->regevent(ISP_PORTB,_ISP_ServiceOnPortB);
	    	
	    	rect.x = 0;
	    	rect.y = 0;
	    	rect.w = 0;
	    	rect.h = 0;
	    	isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);
	    	rect.x = 0;
	    	rect.y = 0;
	    	rect.w = s->display_range.w;
	    	rect.h = s->display_range.h;
	    	isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);
	    	
	    	switch(s->output_image_format)
	    	{
	    		case ISP_IMAGE_FORMAT_RGB565:
	    		param = ISP_FORMATB_RGB565;
	    		break;
	    		
	    		case ISP_IMAGE_FORMAT_RGB666:
	    		param = ISP_FORMATB_RGB666;
	    		break;
	    	    
	    	    case ISP_IMAGE_FORMAT_RGB888:
	    		param = ISP_FORMATB_RGB565;
	    		break;
	    		
	    		default:
	    		SCI_ASSERT(0);
	    		break;
	    	}
	    	isp->ioctl(ISP_CFG_PORTB_FORMAT,&param);
		{
			ISP_FRAME_T video_frame,*f=&video_frame;
			if(s->output_addr.yaddr)
			{
				s->display_addr[0] = s->output_addr.yaddr;
			}
			f->rgbaddr=s->display_addr[0];

			isp->ioctl(ISP_CFG_PORTB_FRAME   ,f);
		}
	    	param = ISP_MODE_EXTERNAL2;
	 }

    	  isp->ioctl(ISP_CFG_MODE,&param);
		
	  param = ISP_TRAM_IN_CENTER;
	  isp->ioctl(ISP_CFG_TRIM_POSITION,&param);
		
	  if(((s->input_range.w == s->encoder_size.w)&&(s->input_range.h == s->encoder_size.h))
    		||((s->input_range.w == s->display_range.w)&&(s->input_range.h == s->display_range.h)))
	  {
		   param = 0;
	  }    
	  else
	  {
	    	   param = ISP_SCALE1_ENABLE;
	  }
	  isp->ioctl(ISP_CFG_SC1_ENABLE   ,&param);
	    
	  param = 0;
	  isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);

	   param = s->input_size.w-s->input_range.w;
	   isp->ioctl(ISP_CFG_PORTC_EOL_SKIP ,&param);
    
    param = 0;
    isp->ioctl(ISP_CFG_PRE_SKIP_FRAMES  ,&param);

    size.w = s->input_size.w;
    size.h = s->input_size.h;
    isp->ioctl(ISP_CFG_INPUT_SIZE  ,&size);

    rect.x = s->input_range.x;
    rect.y = s->input_range.y;
    rect.w = s->input_range.w;
    rect.h = s->input_range.h;
    isp->ioctl(ISP_CFG_SCALE1_IN   ,&rect);


    param = 0;
    isp->ioctl(ISP_CFG_YUV_SEQUENCE ,&param);
    //isp->ioctl(ISP_CFG_RAWRGB_BAYER ,&param); for Raw RGB Bayer sequence


    //input shift info

    param = 1;
    isp->ioctl(ISP_CFG_Y_SHIFT128,&param);
    param = 1;
    isp->ioctl(ISP_CFG_UV_SHIFT128,&param);

 //not used in port b mode
    //output shift info
	
    if(s->output_yuv_shift)
    {
    		param = 1;
    }
    else
    {
    		param = 0;
    }
    isp->ioctl(ISP_CFG_PORTA_Y_SHIFT128,&param);
    isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);

    
    param = 0;
    isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);
{
	unsigned long stream[16];
	ISP_ADDRESS_T *a;
	stream[0] = ISP_PORTA;
    stream[1] = 3;

            a = s->encoder_addr;
            stream[2] = a->yaddr;
            stream[3] = a->uaddr;
            stream[4] = a->vaddr;a++;
            stream[5] = a->yaddr;
            stream[6] = a->uaddr;
            stream[7] = a->vaddr;a++;
            stream[8] = a->yaddr;
            stream[9] = a->uaddr;
            stream[10]= a->vaddr;
            isp->ioctl(ISP_CFG_FRAME_MANUAL ,stream);
	
    }
    isp->regevent(ISP_PORTC,_ISP_ServiceOnPortC);
    isp->regevent(ISP_CAPSTART,_ISP_ServiceOnCapEof);

    isp->start();
        
	
	   if(ISP_SERVICE_JPEG == s->service)
	    {
	    		if(s->state == ISP_STATE_CAPTURE_SCALE)
	    		{
	    			s->state = ISP_STATE_CAPTURE_DONE;
	    		}
	    		else
	    		{
		    		if((s->input_range.w == s->encoder_size.w)&&(s->input_range.h == s->encoder_size.h))
		    		{
		    			s->state = ISP_STATE_CAPTURE_DONE;
		    		}
		    		else
		    		{
		    			s->state = ISP_STATE_CAPTURE_SCALE;
		    		}
	    		}
	    }
	    else
	    {
	    	if(ISP_SERVICE_SCALE == s->service)
	    	{
	    		s->state = ISP_STATE_SCALE_DONE;
	    	}
	    	else
	    	{
	    		SCI_ASSERT(0);
	    	}
	    }
	
	addr.yaddr = s->input_addr.yaddr;
	addr.uaddr = s->input_addr.uaddr;
	addr.vaddr = s->input_addr.vaddr;
/*
    addr.yaddr += s->input_size.w*s->input_range.y + s->input_range.x  ;
    addr.uaddr += s->input_size.w*s->input_range.y/4 + s->input_range.x/2;
    addr.vaddr += s->input_size.w*s->input_range.y/4 + s->input_range.x/2;
*/
    isp->ioctl(ISP_CFG_PORTC_FRAME,&addr);
}

/*****************************************************************************/
//  Description:    start hw to scale one frame
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_StartSliceScale(uint32 slice_id)
{
	DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);

    ISP_RECT_T      rect;
    ISP_SIZE_T     	size;
    ISP_FRAME_T		addr = {0};
    uint32      param;

    
   	//SCI_TraceLow:"_ISP_StartSliceScale, slice_id=%d"
   	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2157_112_2_18_1_44_42_860,(uint8*)"d",slice_id);
    if(slice_id == 0)	
    {
  	    _ISP_ServiceSyncConfigure();
    }
	
	//set input format
	switch(s->input_image_format)
	{
		case ISP_IMAGE_FORMAT_YUV420:
			param = ISP_FORMAT_YUV420;
			break;
		
		case ISP_IMAGE_FORMAT_RAW:
			param = ISP_FORMAT_RAWRGB;
			break;
		
		case ISP_IMAGE_FORMAT_RGB888:
		case ISP_IMAGE_FORMAT_RGB666:
			param = ISP_FORMAT_FULLRGB888;
			break;
			
		case ISP_IMAGE_FORMAT_RGB565:
			param = ISP_FORMAT_FULLRGB565;
			break;
	}
    isp->ioctl(ISP_CFG_INPUT_FORMAT,&param);
	
	//select isp work mode
	if(s->output_image_format == ISP_IMAGE_FORMAT_YUV420)
	{
    	
    	isp->regevent(ISP_PORTA,_ISP_ServiceOnPortA);
    	rect.x = 0;
	    rect.y = 0;
	    rect.w = s->encoder_size.w / s->total_slice_num;
	    rect.h = s->encoder_size.h;
	    isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);
	    
	    param = s->encoder_size.w/s->total_slice_num;
		isp->ioctl(ISP_CFG_PORTA_EOL_SKIP ,&param);
	    
	    rect.x = 0;
	    rect.y = 0;
	    rect.w = 0;
	    rect.h = 0;
	    isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);
        
        //set port a output addr
        {   
        	uint32 stream[2+PA_NR_PINGPONG_FRAME*3];
        	uint32 i;
        	ISP_ADDRESS_T *a; 
        	
            stream[0] = ISP_PORTA;
            stream[1] = 3;
            
            a = s->encoder_addr;
            for(i=0; i<PA_NR_PINGPONG_FRAME; i++)
            {
            	stream[2+i*3] = a->yaddr + s->encoder_size.w * slice_id / s->total_slice_num;
            	stream[3+i*3] = a->uaddr + s->encoder_size.w * slice_id / (s->total_slice_num*2);
            	stream[4+i*3] = a->vaddr + s->encoder_size.w * slice_id / (s->total_slice_num*2);
            	
            	a++;
            }

            isp->ioctl(ISP_CFG_FRAME_MANUAL ,stream);	    
		}

		if(s->input_image_format == ISP_IMAGE_FORMAT_RAW)
    	{
    		param = ISP_MODE_EXTERNAL0;
    	}
    	else
    	{
    		param = ISP_MODE_EXTERNAL1;
    	}
    	
    }
    else
    {
    	
    	isp->regevent(ISP_PORTB,_ISP_ServiceOnPortB);
    	
    	rect.x = 0;
    	rect.y = 0;
    	rect.w = 0;
    	rect.h = 0;
    	isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);
    	rect.x = 0;
    	rect.y = 0;
    	rect.w = s->display_range.w / s->total_slice_num;
    	rect.h = s->display_range.h;
    	isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);
    	
    	param = s->display_range.w/s->total_slice_num;
		isp->ioctl(ISP_CFG_PORTB_EOL_SKIP ,&param);
    	
    	switch(s->output_image_format)
    	{
    		case ISP_IMAGE_FORMAT_RGB565:
    		param = ISP_FORMATB_RGB565;
    		break;
    		
    		case ISP_IMAGE_FORMAT_RGB666:
    		param = ISP_FORMATB_RGB666;
    		break;
    	    
    	    case ISP_IMAGE_FORMAT_RGB888:
    		param = ISP_FORMATB_RGB565;
    		break;
    		
    		default:
    		SCI_ASSERT(0);
    		break;
    	}
    	isp->ioctl(ISP_CFG_PORTB_FORMAT,&param);
	
		{
		   	ISP_FRAME_T video_frame,*f=&video_frame;
			if(s->output_addr.yaddr)
			{
				s->display_addr[0] = s->output_addr.yaddr;
			}
			f->rgbaddr=s->display_addr[0];

			isp->ioctl(ISP_CFG_PORTB_FRAME   ,f);
		}
    	param = ISP_MODE_EXTERNAL2;
    }

    isp->ioctl(ISP_CFG_MODE,&param);

    param = 0;
    isp->ioctl(ISP_CFG_PRE_SKIP_FRAMES  ,&param);

    size.w = s->input_size.w;
    size.h = s->input_size.h;
    isp->ioctl(ISP_CFG_INPUT_SIZE  ,&size);

    rect.x = s->input_range.x;
    rect.y = s->input_range.y;
    rect.w = s->input_range.w / s->total_slice_num;
    rect.h = s->input_range.h;
    isp->ioctl(ISP_CFG_SCALE1_IN   ,&rect);


    param = 0;
    isp->ioctl(ISP_CFG_YUV_SEQUENCE ,&param);
    //isp->ioctl(ISP_CFG_RAWRGB_BAYER ,&param); for Raw RGB Bayer sequence


    //input shift info

    param = 1;
    isp->ioctl(ISP_CFG_Y_SHIFT128,&param);


    param = 1;
    isp->ioctl(ISP_CFG_UV_SHIFT128,&param);

 //not used in port b mode
    //output shift info
	
	if(s->output_yuv_shift)
	{
    	param = 1;
    }
    else
    {
    	param = 0;
    }
    isp->ioctl(ISP_CFG_PORTA_Y_SHIFT128,&param);
    isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);
    
    if(slice_id)
	{
		param = ISP_TRAM_LEFT_ALIGN;
	}
	else
	{
		param = ISP_TRAM_RIGHT_ALIGN;
	}
	isp->ioctl(ISP_CFG_TRIM_POSITION,&param);


    if(((s->input_range.w == s->encoder_size.w)&&(s->input_range.h == s->encoder_size.h))
    	||((s->input_range.w == s->display_range.w)&&(s->input_range.h == s->display_range.h)))
	{
		param = 0;
	}    
    else
    {
    	param = ISP_SCALE1_ENABLE;
    }
    isp->ioctl(ISP_CFG_SC1_ENABLE   ,&param);
    
    param = 0;
    isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);
	
	param = s->input_size.w-s->input_range.w/s->total_slice_num;	
	isp->ioctl(ISP_CFG_PORTC_EOL_SKIP ,&param);


    isp->regevent(ISP_PORTC,_ISP_ServiceOnPortC);

    isp->start();
    
    addr.yaddr = s->input_addr.yaddr;
	addr.uaddr = s->input_addr.uaddr;
	addr.vaddr = s->input_addr.vaddr;
	
	
	

	
    addr.yaddr += s->input_size.w*s->input_range.y + s->input_range.x + s->input_range.w*slice_id/s->total_slice_num  ;
    addr.uaddr += s->input_size.w*s->input_range.y/4 + s->input_range.x/2 + s->input_range.w*slice_id/(2*s->total_slice_num);
    addr.vaddr += s->input_size.w*s->input_range.y/4 + s->input_range.x/2 + s->input_range.w*slice_id/(2*s->total_slice_num);


    isp->ioctl(ISP_CFG_PORTC_FRAME,&addr);
    
    if(s->service == ISP_SERVICE_JPEG)
    {
    	if(slice_id)
    	{
    		s->state = ISP_STATE_CAPTURE_DONE;
    	}
    	else
    	{
    		s->state = ISP_STATE_SCALE_SLICE;
    	}	
    }
    else
    {
    	s->state = ISP_STATE_SCALE_DONE;
	}

}

LOCAL  uint32  s_from_dc = 0;
void	ISP_ServiceReviewFromDC(uint32 b_from_dc)
{
	s_from_dc = b_from_dc;
}

/*****************************************************************************/
//  Description:    copy review paramter to isp service struct
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/

LOCAL void _ISP_SetReviewParam(ISP_REVIEW_PARAM_T * param_ptr)
{
	DECLARE_SERVICE_ENTRY(s);
	DISP_BLOCK_T *disp_block_ptr;
	BLOCKCFG_T		block_cfg;
	LCD_INFO_T 			lcd_info_ptr = {0};
	
	//SCI_TRACE_LOW:"_ISP_SetReviewParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2417_112_2_18_1_44_42_861,(uint8*)"");
	
	s->state = ISP_STATE_IDLETEMP;
    s->service = ISP_SERVICE_REVIEW;
	
	s->input_image_format = param_ptr->input_format;
	
	SCI_MEMCPY((void*)&(s->input_size), (void*)&(param_ptr->input_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY((void*)&(s->input_range), (void*)&(param_ptr->input_range), sizeof(ISP_RECT_T));
	SCI_MEMCPY((void*)&(s->input_addr), (void*)&(param_ptr->input_addr), sizeof(ISP_ADDRESS_T));
	
	SCI_MEMCPY((void*)&(s->display_range), (void*)&(param_ptr->disp_range), sizeof(ISP_RECT_T));

	if(param_ptr->disp_range.w % 8)
	{
		s->display_range.w = (param_ptr->disp_range.w + 7)&(~7);//8 pixels aligned
	}
	s->encoder_size.w = 0;
	s->encoder_size.h = 0;
	s->display_rotation = param_ptr->disp_rotation;
    
	_ISP_Backuplcdblockinfo(param_ptr->disp_lcd_id,param_ptr->disp_block_id,0);
	
	disp_block_ptr = &(s->display_block);
	disp_block_ptr->lcd_id  	= param_ptr->disp_lcd_id;
	disp_block_ptr->block_id 	= param_ptr->disp_block_id;

    _ISP_ServiceConfigBlock(disp_block_ptr,&s->display_range,&param_ptr->lcd_rect,s->display_rotation);

	s->total_slice_num = 0;
		
}

/*****************************************************************************/
//  Description:    start hw to review one frame
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_StartReview()
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
    ISP_RECT_T      rect;
    ISP_SIZE_T      size;
    uint32          param;

    //ISP_SERVICE_TRACE:"_ISP_StartReview, cur time: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2462_112_2_18_1_44_43_862,(uint8*)"d",SCI_GetTickCount());

    _ISP_ServiceSyncConfigure();

    if(s->input_range.w <= 640 && s_b_frame_photo_review == 0)
    {
        //input width is less than 640, sc1 or sc2 can be used to de-block 
        param = ISP_MODE_EXTERNAL4;
        isp->ioctl(ISP_CFG_MODE         ,&param);

        if((s->input_range.w == s->display_range.w)&&(s->input_range.h == s->display_range.h))
        {
            param = 0;
        }    
        else
        {
            param = 1;
        }
        isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);
    }
    else
    {
        s_b_frame_photo_review = 0;
        //input width is larger than 640, sc2 must be bypass 
        param = ISP_MODE_EXTERNAL2;
        isp->ioctl(ISP_CFG_MODE         ,&param);

        if((s->input_range.w == s->display_range.w)&&(s->input_range.h == s->display_range.h))
        {
            param = 0;
        }    
        else
        {
            param = ISP_SCALE1_ENABLE;
        }
        isp->ioctl(ISP_CFG_SC1_ENABLE   ,&param);

        param = 0;
        isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);
    }

    param = s->input_size.w-s->input_range.w;
    isp->ioctl(ISP_CFG_PORTC_EOL_SKIP ,&param);

    param = ISP_TRAM_IN_CENTER;
    isp->ioctl(ISP_CFG_TRIM_POSITION,&param);

    
    param = ISP_FORMAT_YUV422;
    isp->ioctl(ISP_CFG_INPUT_FORMAT,&param);
    
    param = 0;
    isp->ioctl(ISP_CFG_PRE_SKIP_FRAMES  ,&param);

	param = s->display_block.resolution;	    	
	isp->ioctl(ISP_CFG_PORTB_FORMAT,&param);
		
    size.w = s->input_size.w;
    size.h = s->input_size.h;
    isp->ioctl(ISP_CFG_INPUT_SIZE  ,&size);

    rect.x = s->input_range.x;
    rect.y = s->input_range.y;
    rect.w = s->input_range.w;
    rect.h = s->input_range.h;
    isp->ioctl(ISP_CFG_SCALE1_IN   ,&rect);
    rect.x = 0;
    rect.y = 0;
    rect.w = s->encoder_size.w ;
    rect.h = s->encoder_size.h;
    isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);
    rect.x = 0;
    rect.y = 0;
    rect.w = s->display_range.w;
    rect.h = s->display_range.h;
    isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);

    param = 0;
    isp->ioctl(ISP_CFG_DISP_BUFF_OFFSET, &param) ;  

    param = 0;
    isp->ioctl(ISP_CFG_YUV_SEQUENCE ,&param);
    //isp->ioctl(ISP_CFG_RAWRGB_BAYER ,&param); for Raw RGB Bayer sequence

 	param = 1;
    isp->ioctl(ISP_CFG_Y_SHIFT128,&param);
    isp->ioctl(ISP_CFG_UV_SHIFT128,&param);

    param = 1;
    isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);

    isp->regevent(ISP_PORTB,_ISP_ServiceOnPortB);
    isp->regevent(ISP_PORTC,_ISP_ServiceOnPortC);
    isp->regevent(ISP_CAPSTART,_ISP_ServiceOnCapEof);

    isp->start();
    
	{
	    ISP_ADDRESS_T *a;
    	    ISP_RECT_T  *r;
	    
	    ISP_FRAME_T video_frame,*f=&video_frame;
		
		//此处只care rgbaddr,
	    f->rgbaddr = s->display_addr[0];
	    f->flags = 0;
	    f->width = s->display_range.w;
	    f->height = s->display_range.h;		
	    isp->ioctl(ISP_CFG_PORTB_FRAME   ,f);

	    //get dynamic window address
	    a = &s->input_addr;
	    r = &s->input_range;
	    f->yaddr = a->yaddr;
	    f->uaddr = a->uaddr;
	    f->vaddr = a->vaddr;

           f->yaddr += s->input_size.w*r->y   + r->x  ;
           f->uaddr += s->input_size.w*r->y/4 + r->x/2;
           f->vaddr += s->input_size.w*r->y/4 + r->x/2;

	    isp->ioctl(ISP_CFG_PORTC_FRAME,f);

	    s->is_display	=	1;	
	}
	
	s->state = ISP_STATE_REVIEW_DONE;
}

/*****************************************************************************/
//  Description:    copy mpeg  record paramter to isp service struct
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_SetMpegParam(ISP_MPEG_PARAM_T * param_ptr)
{
	
	DECLARE_SERVICE_ENTRY(s);
	DISP_BLOCK_T *disp_block_ptr;
	BLOCKCFG_T		block_cfg;
	SENSOR_EXP_INFO_T	sensor_info;
	LCD_INFO_T 			lcd_info_ptr = {0};
	
	sensor_info = *Sensor_GetInfo();
	
	//SCI_TRACE_LOW:"_ISP_SetMpegParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2606_112_2_18_1_44_43_863,(uint8*)"");
	s->state = ISP_STATE_IDLETEMP;
	s->service = ISP_SERVICE_MPEG;
	
	SCI_MEMCPY((void*)&(s->input_size), (void*)&(param_ptr->input_size), sizeof(ISP_SIZE_T));
    s->input_range.x = 0;
    s->input_range.y = 0;
    s->input_range.w = param_ptr->input_size.w;
    s->input_range.h = param_ptr->input_size.h;    
	SCI_MEMCPY((void*)&(s->encoder_size), (void*)&(param_ptr->encode_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY((void*)&(s->display_range),(void*)&(param_ptr->disp_range), sizeof(ISP_RECT_T));
	if(param_ptr->disp_range.w % 8)
	{
		s->display_range.w = (param_ptr->disp_range.w + 7)&(~7);//8 pixels aligned
	}
	s->display_rotation = param_ptr->disp_rotation;
	
	s->is_display		= param_ptr->is_display;
	disp_block_ptr = &(s->display_block);
		 
	disp_block_ptr->lcd_id  	= param_ptr->disp_lcd_id;
	disp_block_ptr->block_id 	= param_ptr->disp_block_id;
	
	
	if(ISP_DISPLAY_NONE != param_ptr->disp_lcd_id)
	{
        _ISP_ServiceConfigBlock(disp_block_ptr,&s->display_range,&param_ptr->lcd_rect,s->display_rotation);
	}
	else
	{
		disp_block_ptr->img_rect.w = s->display_range.w;
		disp_block_ptr->img_rect.h = s->display_range.h;
	}
	

	s->output_yuv_shift = param_ptr->yuv_shift;
	
	s->input_image_format	= sensor_info.image_format;
	s->input_image_patten	= sensor_info.image_pattern;
	s->vsync_polarity 		= sensor_info.vsync_polarity;
	s->hsync_polarity 		= sensor_info.hsync_polarity;
	s->pclk_polarity  		= sensor_info.pclk_polarity;
	
}

/*****************************************************************************/
//  Description:    start hw to get yuv420 frames  and full rgb frames continuously
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_StartMpeg()
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);

    uint32         param;
    ISP_RECT_T     rect;
    ISP_SIZE_T     size;
	
    //SCI_TRACE_LOW:"_ISP_StartMpeg"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2664_112_2_18_1_44_43_864,(uint8*)"");

	_ISP_ServiceSyncConfigure();
	
    if(s->input_image_format == ISP_IMAGE_FORMAT_YUV422)
    {
    	param = ISP_FORMAT_YUV422;
		isp->ioctl(ISP_CFG_INPUT_FORMAT,&param);
    	param = s->input_image_patten;
    	isp->ioctl(ISP_CFG_YUV_SEQUENCE ,&param);
    	
    	param = 1;
		isp->ioctl(ISP_CFG_Y_SHIFT128,&param);

		param = 1;
		isp->ioctl(ISP_CFG_UV_SHIFT128,&param);	
    	
    }
    else
    {

    	    	
    }
    
    param = s->vsync_polarity;
	isp->ioctl(ISP_CFG_VSYNC_POLARITY ,&param);
	
	param = s->hsync_polarity;
	isp->ioctl(ISP_CFG_HSYNC_POLARITY ,&param);
	
	param = s->pclk_polarity;
    isp->ioctl(ISP_CFG_PCLK_POLARITY ,&param);
    
	param = 1;
    isp->ioctl(ISP_CFG_PRE_SKIP_FRAMES  ,&param);
    
    param = s->output_yuv_shift;
	isp->ioctl(ISP_CFG_PORTA_Y_SHIFT128,&param);
    isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);
   
	param = s->display_block.resolution;
	isp->ioctl(ISP_CFG_PORTB_FORMAT,&param);
    
        {
            uint32 stream[2+PA_NR_PINGPONG_FRAME*3];
            ISP_ADDRESS_T *a;
			uint32 i;
			
            stream[0] = ISP_PORTB;
            stream[1] = PB_NR_PINGPONG_FRAME;
            
            for(i=0; i<PB_NR_PINGPONG_FRAME; i++)
			{
			   	stream[2+i] = s->display_addr[i];
			}
			
			isp->ioctl(ISP_CFG_FRAME_MANUAL ,stream);	

            stream[0] = ISP_PORTA;
            stream[1] = PA_NR_PINGPONG_FRAME;
            a = s->encoder_addr;
            for(i=0; i<PA_NR_PINGPONG_FRAME; i++)
            {
            	stream[2+i*3] = a->yaddr;
            	stream[3+i*3] = a->uaddr;
            	stream[4+i*3] = a->vaddr;
            	
            	a++;
            }
            isp->ioctl(ISP_CFG_FRAME_MANUAL ,stream);
        }
    
       
    	param = ISP_SCALE1_ENABLE;
        isp->ioctl(ISP_CFG_SC1_ENABLE   ,&param);
                       
        param = 1;
        isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);
    
    	param = ISP_MODE_MPEG;
    	isp->ioctl(ISP_CFG_MODE         ,&param);
	
	
	
		size.w = s->input_size.w;
	    size.h = s->input_size.h;
	    isp->ioctl(ISP_CFG_INPUT_SIZE  ,&size);
	    
	    rect.x = 0;
	    rect.y = 0;
	    rect.w = s->encoder_size.w;
	    rect.h = s->encoder_size.h;
	    isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);

	    rect.x = 0;
	    rect.y = 0;
	    rect.w = s->display_range.w;
	    rect.h = s->display_range.h;
	    isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);

        param = 0;
        isp->ioctl(ISP_CFG_DISP_BUFF_OFFSET, &param) ;  
 
	    rect.x = s->input_range.x;
	    rect.y = s->input_range.y;
	    rect.w = s->input_range.w;
	    rect.h = s->input_range.h;
	    isp->ioctl(ISP_CFG_CAP_INPUT_RANGE,&rect);
	  	
	  	#ifdef PLATFORM_SC8800H
		  	param = ISP_TRAM_IN_CENTER;
			isp->ioctl(ISP_CFG_TRIM_POSITION,&param);
	  	#endif
	  		
	  	isp->regevent(ISP_PORTA,_ISP_ServiceOnPortA);
		isp->regevent(ISP_PORTB,_ISP_ServiceOnPortB);
		isp->regevent(ISP_CAPSTART,_ISP_ServiceOnCapEof);
	  	isp->regevent(ISP_PORTAOVF,_ISP_ServiceOnPortAOverflow);
		isp->regevent(ISP_PORTBOVF,_ISP_ServiceOnPortBOverflow);
	  		
     //  LCDC_RegisterInterrupt(2,ISP_LcdInvalidateRectDone);
	  		
	isp->start();
	
	s->state = ISP_STATE_MPEG;
 
}


/*****************************************************************************/
//  Description:    check scale paramter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 _ISP_CheckVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    check scale paramter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 _ISP_CheckVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr)
{
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    copy video phone paramter to isp service struct
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/****************************************************************************/
LOCAL void _ISP_SetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
	
	DECLARE_SERVICE_ENTRY(s);
	DISP_BLOCK_T *disp_block_ptr;
	SENSOR_EXP_INFO_T	sensor_info;
	
	sensor_info = *Sensor_GetInfo();
	
	//SCI_TraceLow:"_ISP_SetVTEncodeParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2824_112_2_18_1_44_43_865,(uint8*)"");
	
	s->state = ISP_STATE_IDLETEMP;
	s->service = ISP_SERVICE_VP;

	SCI_MEMCPY(&(s->input_size), &(param_ptr->local_input_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY(&(s->input_range), &(param_ptr->local_input_range), sizeof(ISP_RECT_T));
	SCI_MEMCPY(&(s->encoder_size), &(param_ptr->encode_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY(&(s->display_range), &(param_ptr->local_disp_range), sizeof(ISP_RECT_T));
	
	s->output_yuv_shift = param_ptr->yuv_shift;

	s->display_rotation = param_ptr->disp_rotation;
	disp_block_ptr = &(s->display_block);
	 
	disp_block_ptr->lcd_id  	= param_ptr->local_disp_lcd_id;
	disp_block_ptr->block_id 	= param_ptr->local_disp_block_id;

    if(param_ptr->local_display_enable)
    {
        _ISP_ServiceConfigBlock(disp_block_ptr,&s->display_range,&param_ptr->lcd_rect,param_ptr->disp_rotation);
    }
    else
    {
        disp_block_ptr->img_rect.x = s->display_range.x;
        disp_block_ptr->img_rect.y = s->display_range.y;                

        if(param_ptr->disp_rotation == ISP_ROTATION_90 ||
           param_ptr->disp_rotation == ISP_ROTATION_270)
        {
            disp_block_ptr->img_rect.w = s->display_range.h;
            disp_block_ptr->img_rect.h = s->display_range.w;
        }
        else
        {
            disp_block_ptr->img_rect.w = s->display_range.w;
            disp_block_ptr->img_rect.h = s->display_range.h;
        }
    }

    s->input_image_format	= sensor_info.image_format;
	s->input_image_patten	= sensor_info.image_pattern;
	s->vsync_polarity 		= sensor_info.vsync_polarity;
	s->hsync_polarity 		= sensor_info.hsync_polarity;
	s->pclk_polarity  		= sensor_info.pclk_polarity;
  	s->local_display_enable		= param_ptr->local_display_enable;
  	s->local_review_enable		= param_ptr->local_review_enable;	
	//SCI_TraceLow:"_ISP_SetVTEncodeParam,s->local_display_enable = %d,s->local_review_enable = %d,s->display_rotation %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2874_112_2_18_1_44_44_866,(uint8*)"ddd",s->local_display_enable,s->local_review_enable,s->display_rotation);

}

/*****************************************************************************/
//  Description:    copy video phone paramter to isp service struct
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/****************************************************************************/
LOCAL void _ISP_SetVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr)
{
	DECLARE_SERVICE_ENTRY(s);
	DISP_BLOCK_T *disp_block_ptr;

	s->service = ISP_SERVICE_VP;
	
	SCI_MEMCPY(&(s->remote_size), &(param_ptr->remote_input_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY(&(s->remote_range), &(param_ptr->remote_input_range), sizeof(ISP_RECT_T));
	SCI_MEMCPY(&(s->remote_display_range), &(param_ptr->remote_disp_range), sizeof(ISP_RECT_T));
	
	disp_block_ptr = &(s->remote_display_block);
	 
	disp_block_ptr->lcd_id  	= param_ptr->remote_disp_lcd_id;
	disp_block_ptr->block_id 	= param_ptr->remote_disp_block_id;
    if(param_ptr->remote_display_enable)
    {
        _ISP_ServiceConfigBlock(disp_block_ptr,&s->remote_display_range,&param_ptr->lcd_rect,param_ptr->remote_disp_rotation);
    }
    else
    {
        disp_block_ptr->img_rect.x = s->remote_display_range.x;
        disp_block_ptr->img_rect.y = s->remote_display_range.y;                

        if(param_ptr->remote_disp_rotation == ISP_ROTATION_90 ||
           param_ptr->remote_disp_rotation == ISP_ROTATION_270)
        {
            disp_block_ptr->img_rect.w = s->remote_display_range.h;
            disp_block_ptr->img_rect.h= s->remote_display_range.w;
        }
        else
        {
            disp_block_ptr->img_rect.w = s->remote_display_range.w;
            disp_block_ptr->img_rect.h = s->remote_display_range.h;
        }
    }

	s->vp_deblock_enable			= param_ptr->deblock_enable;
	s->remote_display_rotation = param_ptr->remote_disp_rotation;
  	s->remote_display_enable		= param_ptr->remote_display_enable;
  	s->remote_review_enable		= param_ptr->remote_review_enable;	
	//SCI_TraceLow:"_ISP_SetVTDecodeParam,s->remote_display_enable = %d,s->remote_review_enable = %d,s->remote_display_rotation %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2926_112_2_18_1_44_44_867,(uint8*)"ddd",s->remote_display_enable,s->remote_review_enable,s->remote_display_rotation);

	
}
/*****************************************************************************/
//  Description:    start hw to get yuv420 frames 
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_StartVTEncode()
{
	DECLARE_SERVICE_ENTRY(s);
	DECLARE_DRIVER_ENTRY(isp);

	unsigned long   param;
	ISP_RECT_T     rect;
	ISP_SIZE_T      size;
   	uint32               timeout_count = 0;
	
	//SCI_TraceLow:"ISP_SERVICE: _ISP_StartVTEncode"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2944_112_2_18_1_44_44_868,(uint8*)"");

	while(s_b_vt_reviewing)
	{
		if(timeout_count++ < 2)
		{
			//SCI_TraceLow:"ISP_SERVICE: _ISP_StartVTEncode,Sleeping"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2950_112_2_18_1_44_44_869,(uint8*)"");
			s_b_vt_watchdog_feeded = 1;
			SCI_Sleep(10);
		}
		else
		{
			_ISP_ExitVTDisplay();
		}
	}

	//SCI_TraceLow:"ISP_SERVICE: _ISP_StartVTEncode,start VT capture"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_2960_112_2_18_1_44_44_870,(uint8*)"");
	
	vp_local_count = 0;
	vp_remote_count = 0;
	_ISP_ServiceSyncConfigure();

	if(s->input_image_format == ISP_IMAGE_FORMAT_YUV422)
	{
		param = ISP_FORMAT_YUV422;
		isp->ioctl(ISP_CFG_INPUT_FORMAT,&param);
		param = s->input_image_patten;
		isp->ioctl(ISP_CFG_YUV_SEQUENCE ,&param);

		param = 1;
		isp->ioctl(ISP_CFG_Y_SHIFT128,&param);

		param = 1;
		isp->ioctl(ISP_CFG_UV_SHIFT128,&param);	

	}
	else
	{

	}

	param = s->vsync_polarity;
	isp->ioctl(ISP_CFG_VSYNC_POLARITY ,&param);

	param = s->hsync_polarity;
	isp->ioctl(ISP_CFG_HSYNC_POLARITY ,&param);

	param = s->pclk_polarity;
	isp->ioctl(ISP_CFG_PCLK_POLARITY ,&param);

	param = 0;
	isp->ioctl(ISP_CFG_PRE_SKIP_FRAMES  ,&param);

	param = s->output_yuv_shift;
	isp->ioctl(ISP_CFG_PORTA_Y_SHIFT128,&param);
	isp->ioctl(ISP_CFG_PORTA_UV_SHIFT128,&param);

	{
		unsigned long stream[2+PA_NR_PINGPONG_FRAME*3];
		ISP_ADDRESS_T *a;
		uint32 i;

		stream[0] = ISP_PORTA;
		stream[1] = PA_NR_PINGPONG_FRAME;
		a = s->encoder_addr;
		for(i=0; i<PA_NR_PINGPONG_FRAME; i++)
		{
		stream[2+i*3] = a->yaddr;
		stream[3+i*3] = a->uaddr;
		stream[4+i*3] = a->vaddr;

		a++;
		}

		isp->ioctl(ISP_CFG_FRAME_MANUAL ,stream);
	}

	if(s->input_range.w == s->encoder_size.w &&  s->input_range.h == s->encoder_size.h)
	{
		param = 0;
	}
	else
	{
		param = ISP_SCALE1_ENABLE;
	}
	isp->ioctl(ISP_CFG_SC1_ENABLE   ,&param);

	if(s->display_range.w == s->encoder_size.w && s->display_range.h == s->encoder_size.h
	&& s->remote_range.w == s->remote_display_range.w && s->remote_range.h == s->remote_display_range.h)
	{
		param = 0;
	}  
	else
	{
		param = 1;
	}                   
	isp->ioctl(ISP_CFG_SC2_ENABLE   ,&param);

	param = ISP_MODE_VP;
	isp->ioctl(ISP_CFG_MODE         ,&param);

	size.w = s->input_size.w;
	size.h = s->input_size.h;
	isp->ioctl(ISP_CFG_INPUT_SIZE  ,&size);

	rect.x = 0;
	rect.y = 0;
	rect.w = s->encoder_size.w;
	rect.h = s->encoder_size.h;
	isp->ioctl(ISP_CFG_ENCODER_RANGE,&rect);

	rect.x = s->input_range.x;
	rect.y = s->input_range.y;
	rect.w = s->input_range.w;
	rect.h = s->input_range.h;
	isp->ioctl(ISP_CFG_CAP_INPUT_RANGE,&rect);

	param = ISP_TRAM_IN_CENTER;
	isp->ioctl(ISP_CFG_TRIM_POSITION,&param);

	isp->regevent(ISP_PORTA,_ISP_ServiceOnPortA);
	isp->regevent(ISP_PORTB,_ISP_ServiceOnPortB);
	isp->regevent(ISP_PORTAOVF,_ISP_ServiceOnPortAOverflow);
	isp->regevent(ISP_PORTBOVF,_ISP_ServiceOnPortBOverflow);
	isp->regevent(ISP_CAPSTART,_ISP_ServiceOnCapEof);	

	s->display_from = 0;

	s->state = ISP_STATE_VP;

	isp->start();
    
}


/*****************************************************************************/
//  Description:    start hw to get yuv420 frames 
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_StartVTDisplay()
{	
	DECLARE_SERVICE_ENTRY(s);
    	DECLARE_DRIVER_ENTRY(isp);
	
	uint32      	param = 0;
	ISP_RECT_T      rect;
	ISP_SIZE_T     	size;
	ISP_FRAME_T 	video_frame,*f=&video_frame;;
    
    
    //f->rgbaddr=s->display_addr[0];
	//isp->ioctl(ISP_CFG_PORTB_FRAME   ,f);
	
	param = 1;
	isp->ioctl(ISP_CFG_Y_SHIFT128,&param);
	isp->ioctl(ISP_CFG_UV_SHIFT128,&param);

	param = ISP_TRAM_IN_CENTER;
	isp->ioctl(ISP_CFG_TRIM_POSITION,&param);
	param = ISP_MODE_VP;
	isp->ioctl(ISP_CFG_MODE ,&param);

//	SCI_TRACE_LOW("_ISP_StartVTDisplay , s->display_from= %d",s->display_from);
	
	switch(s->display_from)
	{
		case 0:
			//display local
			param = s->display_block.resolution;
			isp->ioctl(ISP_CFG_PORTB_FORMAT,&param);

			rect.x = 0;
			rect.y = 0;
			rect.w = s->display_range.w;
			rect.h = s->display_range.h;
			isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);

            param = 0;
        	isp->ioctl(ISP_CFG_DISP_BUFF_OFFSET, &param) ;  

			size.w = s->encoder_size.w;
			size.h = s->encoder_size.h;
			isp->ioctl(ISP_CFG_VP_REVIEW_SIZE  ,&size);


			rect.x = 0;
			rect.y = 0;
			rect.w = s->encoder_size.w;
			rect.h = s->encoder_size.h;			
			isp->ioctl(ISP_CFG_SCALE2_IN   ,&rect);

			//get dynamic window address


			f->yaddr = s->encoder_frame->yaddr;
			f->uaddr = s->encoder_frame->uaddr;
			f->vaddr = s->encoder_frame->vaddr;


			f->rgbaddr=s->display_addr[vp_local_count%PB_NR_PINGPONG_FRAME];
			//  SCI_MEMSET(f->rgbaddr,0,(uint32)(s->display_range.w*s->display_range.h*2));
	/*		
			//SCI_TRACE_LOW:"_ISP_StartVTDisplay, f->yaddr = 0x%x, f->uaddr = 0x%x, f->vaddr  = 0x%x,f->rgbaddr = 0x%x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3147_112_2_18_1_44_44_871,(uint8*)"dddd",f->yaddr ,f->uaddr,f->vaddr, f->rgbaddr );
	*/		
			vp_local_count++;		    
			f->flags   =0; 
			isp->ioctl(ISP_CFG_PORTB_FRAME   ,f);	

			isp->ioctl(ISP_CFG_VP_PORTC,f);
			
		break;
		
		case 1:
			

			param = s->display_block.resolution;
			isp->ioctl(ISP_CFG_PORTB_FORMAT,&param);

			rect.x = 0;
			rect.y = 0;
			rect.w = s->remote_display_range.w;
			rect.h = s->remote_display_range.h;
			isp->ioctl(ISP_CFG_DISPLAY_RANGE,&rect);

            param = 0;
        	isp->ioctl(ISP_CFG_DISP_BUFF_OFFSET, &param) ;  

			size.w = s->remote_size.w;
			size.h = s->remote_size.h;
			isp->ioctl(ISP_CFG_VP_REVIEW_SIZE  ,&size);

			rect.x = s->remote_range.x;
			rect.y = s->remote_range.y;
			rect.w = s->remote_range.w;
			rect.h = s->remote_range.h;

			isp->ioctl(ISP_CFG_SCALE2_IN   ,&rect);

			//get dynamic window address

			f->yaddr = s->input_addr.yaddr;
			f->uaddr = s->input_addr.uaddr;
			f->vaddr = s->input_addr.vaddr;


			f->rgbaddr = s->remote_display_addr[vp_remote_count%PB_NR_PINGPONG_FRAME];
			vp_remote_count ++;
	/*		
			//SCI_TRACE_LOW:"_ISP_StartVTDisplay, f->yaddr = 0x%x, f->uaddr = 0x%x, f->vaddr  = 0x%x,f->rgbaddr = 0x%x"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3194_112_2_18_1_44_45_872,(uint8*)"dddd",f->yaddr ,f->uaddr,f->vaddr, f->rgbaddr );
	*/		
			f->flags   = 1;
			isp->ioctl(ISP_CFG_PORTB_FRAME   ,f);

			isp->ioctl(ISP_CFG_VP_PORTC,f);
			
			break;
		
		default:
			SCI_ASSERT(0);
			break;
	}


}

LOCAL void _ISP_SetVPFlag(uint32 flag)
{
    DECLARE_SERVICE_ENTRY(s);

	s->display_from = flag;
}

/*****************************************************************************/
//  Description:    stop isp
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
LOCAL void _ISP_Stop()
{
	DECLARE_SERVICE_ENTRY(s);
	DECLARE_DRIVER_ENTRY(isp);

    isp->stop();
    s->state = ISP_STATE_STOP;
}


/*****************************************************************************/
//  Description:    get the isp queue's avilable queue,
//  parameter:  none
//  return value: the avilable queue's num
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
extern SCI_THREAD_PTR SCI_GetThreadBlockFromId(BLOCK_ID thread_id);

LOCAL _ISP_GetQueueAvailable(void)
{
    uint32          status              = 0;
	uint32          count               = 0;
	uint32          enqueued            = 0;
	uint32          store               = 0;
	//char            *thread_name_ptr    = SCI_NULL;
	char            *queue_name_ptr     = SCI_NULL;
	TX_THREAD     	* suspended_thread  = SCI_NULL;	
	TX_QUEUE      	* next_queue        = SCI_NULL;
    SCI_THREAD_T    * thread_block  = SCI_NULL;
    
    //ISP_SERVICE_TRACE:"App_queue:\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3251_112_2_18_1_44_45_873,(uint8*)"");
    thread_block = (SCI_THREAD_T *)SCI_GetThreadBlockFromId(ISP);
    
    status = tx_queue_info_get((TX_QUEUE *)&(thread_block->queue),
		                       &queue_name_ptr,
							   (uint32 *)&enqueued,
							   (uint32 *)&store,
							   (TX_THREAD **)&suspended_thread,
							   (uint32 *)&count,
							   (TX_QUEUE **)&next_queue);
	
    //ISP_SERVICE_TRACE:"queue info:enqueued:%d, store: %d, count: %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3263_112_2_18_1_44_45_874,(uint8*)"ddd", enqueued, store, count);
    return store;
} 

int _ISP_ServiceRotation(uint32 param)
{
	
	SCI_SetEvent(isp_event, ISP_ROTATION_EVENT, SCI_OR);
	
	ISP_ServiceSendRequest(0,ISP_SERVICE_PORTB_SIG,1,(void*)(*(volatile uint32 *)0x20800204));

	return 0;
}

/*****************************************************************************/
//  Description:    get frame address
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceGetFrameAddress(void)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
    
    uint32 addr = 0;
    
    
    //todo: add fid support

    if(s->encoder_frame)
    {
        
        isp->lock(s->encoder_frame);
        s->locked_frame = s->encoder_frame;
        addr = s->encoder_frame->yaddr;
        //do not release this address for debug mode
        //s->encoder_frame = (ISP_FRAME_T *)0;
        
       	//DMA_memcpy((void *)addr, (void *)s->encoder_frame->yaddr,(s->encoder_size.w*s->encoder_size.h*3/2));
 
    }
    
    return addr;
}

/*****************************************************************************/
//  Description:    free frame address
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceFreeFrameAddress(ISP_FRAME_T *frame_ptr)
{
 	//DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);

    //SCI_TRACE_LOW:"ISP_ServiceFreeFrameAddress,yaddr=0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3316_112_2_18_1_44_45_875,(uint8*)"d",frame_ptr->yaddr);

	
    isp->unlock(frame_ptr);
}


/*****************************************************************************/
//  Description:    set review parameter
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
ISP_REVIEW_PARAM_T s_review_param_t;

uint32 ISP_ServiceSetReviewParam(ISP_REVIEW_PARAM_T *param_ptr)
{

	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceSetReviewParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3332_112_2_18_1_44_45_876,(uint8*)"");
	
	//ISP_SERVICE_TRACE:"ISP_SERVICE:input format = %d,input.x = %d, input.y = %d,input.w = %d, input.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3335_112_2_18_1_44_45_877,(uint8*)"ddddd",param_ptr->input_format, param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h);
	
	//ISP_SERVICE_TRACE:"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3338_112_2_18_1_44_45_878,(uint8*)"dddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h);
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(_ISP_CheckReviewParam(param_ptr))
	{
		return SCI_ERROR;
	}
	
	SCI_MEMCPY(&s_review_param_t,param_ptr,	sizeof(ISP_REVIEW_PARAM_T));

	ISP_ServiceSendRequest(0,ISP_SERVICE_SET_PARAM_SIG,ISP_SERVICE_REVIEW,&s_review_param_t);
		
	return SCI_SUCCESS;	
}

/*****************************************************************************/
//  Description:    start playback
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           JPEG/MPEG
/*****************************************************************************/
void ISP_ServiceStartReview(void)
{
    DECLARE_SERVICE_ENTRY(s);
    
    uint32 actual_event = 0;
    uint32 status = 0;
    
    if(ISP_STATE_CLOSED == s->state)
    {
    	  SCI_ASSERT(0);
    }
    
	//ISP_SERVICE_TRACE:"ISP_SERVICE:ISP_ServiceStartPlayback"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3370_112_2_18_1_44_45_879,(uint8*)"");

  	ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_REVIEW,0);
    
	       	  
	if(NULL == s->on_user_b)
    {
	    s->is_first_frame = 1;  
	   
		status = SCI_GetEvent(isp_event, 
		                      	  ISP_DISPLAY_EVENT, 
		                       	  SCI_AND_CLEAR, 
		                       	  &actual_event, 
		                       	  SCI_WAIT_FOREVER);
		                       	  
		SCI_ASSERT(!status);
	}
    /*
    LCD_DisableBlock(s->display_block.lcd_id, s->display_block.block_id);
    s->display_block.is_enable = 0;
	*/
}

uint32 ISP_ServiceReviewNext(ISP_ADDRESS_T *addr_ptr)
{
    DECLARE_SERVICE_ENTRY(s);
    uint32                 status = 0,actual_event;
    
    if(SCI_NULL == addr_ptr)
    {
        return SCI_ERROR;
    }
    if(SCI_NULL == addr_ptr->yaddr &&
       SCI_NULL == addr_ptr->uaddr && 
       SCI_NULL == addr_ptr->vaddr)
    {
        return SCI_ERROR;    
    }
    s_review_param_t.input_addr.yaddr = addr_ptr->yaddr;
    s_review_param_t.input_addr.uaddr = addr_ptr->uaddr;
    s_review_param_t.input_addr.vaddr = addr_ptr->vaddr;
    
	ISP_ServiceSendRequest(0,ISP_SERVICE_SET_PARAM_SIG,ISP_SERVICE_REVIEW,&s_review_param_t);
    
  	ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_REVIEW,0);
    
	       	  
	if(NULL == s->on_user_b)
    {
	    s->is_first_frame = 1;  
	   
		status = SCI_GetEvent(isp_event, 
	                      	  ISP_DISPLAY_EVENT, 
	                       	  SCI_AND_CLEAR, 
	                       	  &actual_event, 
	                       	  SCI_WAIT_FOREVER);
		                       	  
		SCI_ASSERT(!status);
	}
	
    return SCI_SUCCESS;
    
}

void ISP_ServiceStopReview(void)
{
    return ;
}

/*****************************************************************************/
//  Description:    set preview parameter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceSetPreviewParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3445_112_2_18_1_44_45_880,(uint8*)"");
	
	//SCI_TRACE_LOW:"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3448_112_2_18_1_44_45_881,(uint8*)"dddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3451_112_2_18_1_44_45_882,(uint8*)"dddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h);
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(_ISP_CheckPreviewParam(param_ptr))
	{
		return SCI_ERROR;
	}	

	
	ISP_ServiceSendRequest(0,ISP_SERVICE_SET_PARAM_SIG,ISP_SERVICE_PREVIEW,param_ptr);
	
			
	return SCI_SUCCESS;
}

uint32 ISP_ServiceGetPreviewParam(ISP_PREVIEW_PARAM_T * param_ptr)
{
    DECLARE_SERVICE_ENTRY(s);

    if(!param_ptr || s->service != ISP_SERVICE_PREVIEW)
        return SCI_ERROR;


    SCI_MEMCPY((void*)&(param_ptr->input_size), 
                (void*)&(s->input_size),
                sizeof(ISP_SIZE_T));

    SCI_MEMCPY((void*)&(param_ptr->input_range), 
                (void*)&(s->input_range),
                sizeof(ISP_RECT_T));

    SCI_MEMCPY((void*)&(param_ptr->disp_range), 
                (void*)&(s->display_range), 
                sizeof(ISP_RECT_T));

    param_ptr->disp_lcd_id = s->display_block.lcd_id;    
    param_ptr->disp_block_id = s->display_block.block_id;

    param_ptr->disp_rotation = s->display_rotation;
    return SCI_SUCCESS;    
}

/*****************************************************************************/
//  Description:    start preview
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           JPEG/MPEG
/*****************************************************************************/
uint32 ISP_ServiceStartPreview(void)
{
	
    DECLARE_SERVICE_ENTRY(s);
    uint32 actual_event = 0;
    uint32 status = 0;

    //SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceStartPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3505_112_2_18_1_44_45_883,(uint8*)"");

    ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_PREVIEW,0);
    
    s->is_first_frame = 1;  
        
    status = SCI_GetEvent(isp_event, 
                      	  ISP_DISPLAY_EVENT, 
                       	  SCI_AND_CLEAR, 
                       	  &actual_event, 
                       	  ISP_WAIT_TIME);
    LCD_SetInvalidateLimit(0,PERMIT_GUI_INVALIDATE);
                       	  

    return status;
}

/*****************************************************************************/
//  Description:    stop preview
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStopPreview(void)
{
    uint32 status = SCI_SUCCESS,actual_event;
    DECLARE_SERVICE_ENTRY(s);
    ISP_SIGNAL_T *sig_ptr;
	
    //SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceStopPreview"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3532_112_2_18_1_44_45_884,(uint8*)"");

    if(s->state != ISP_STATE_PREVIEW)
    {
    	return ;
    }

    SCI_SuspendThread(ISP_SERVICE_THREAD);
    while (1)
    {
        sig_ptr = (void*)SCI_PeekSignal(ISP_SERVICE_THREAD);
        if (sig_ptr != PNULL)
        {
            SCI_FREE (sig_ptr);
        }
        else
            break;
    }   
    SCI_ResumeThread(ISP_SERVICE_THREAD);

    _ISP_Stop();

    ISP_ServiceSendRequest(0,ISP_SERVICE_STOP_SIG,0,0);
    status = SCI_GetEvent(isp_event, 
                                 	  ISP_STOP_EVENT, 
                                 	  SCI_AND_CLEAR, 
                                 	  &actual_event, 
                                 	  SCI_WAIT_FOREVER);
                                 	  
                                 	  
    return ;
    
}

/*****************************************************************************/
//  Description:    set JPEG capture parameter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetJpegCaptureParam(ISP_JPEG_PARAM_T * param_ptr)
{
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceSetJpegCaptureJpegParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3572_112_2_18_1_44_45_885,(uint8*)"");
	
	//SCI_TRACE_LOW:"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3575_112_2_18_1_44_45_886,(uint8*)"dddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3578_112_2_18_1_44_45_887,(uint8*)"ddd",param_ptr->yuv_shift,param_ptr->output_size.w,param_ptr->output_size.h);
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(_ISP_CheckJpegParam(param_ptr))
	{
		return SCI_ERROR;
	}
	
	ISP_ServiceSendRequest(0,ISP_SERVICE_SET_PARAM_SIG,ISP_SERVICE_JPEG,param_ptr);

			
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    start capture JPEG 
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStartCaptureJpeg(void)
{
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceStartCaptureJpeg"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3599_112_2_18_1_44_45_888,(uint8*)"");
	
	ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_JPEG,0);
}

//@Liangwen.zhen 060811
/*****************************************************************************/
//  Description:    set compound JPEG capture parameter
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetCompoundJpegCaptureParam(ISP_COMPOUND_JPEG_CAPTURE_PARAM_T * param_ptr)
{
	DECLARE_SERVICE_ENTRY(s);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceSetCompoundJpegCaptureParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3613_112_2_18_1_44_45_889,(uint8*)"");
	
	
	
	//SCI_TRACE_LOW:"ISP_SERVICE:input format = %d,yuv_shift = %d,output.w = %d,output.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3618_112_2_18_1_44_46_890,(uint8*)"dddd",param_ptr->input_format, param_ptr->yuv_shift,param_ptr->output_size.w,param_ptr->output_size.h);
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	
	//SCI_MEMCPY(&(s->input_size), &(param_ptr->input_size), sizeof(ISP_SIZE_T));
	SCI_MEMCPY((void*)&(s->input_range), (void*)&(param_ptr->input_range), sizeof(ISP_RECT_T));
	SCI_MEMCPY((void*)&(s->encoder_size), (void*)&(param_ptr->output_size), sizeof(ISP_SIZE_T));
	
	s->output_yuv_shift = param_ptr->yuv_shift;
	
	s->input_image_format = param_ptr->input_format;
	s->service = ISP_SERVICE_JPEG;
		
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    set compound JPEG flag
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void _ISP_ServiceSetCompoundJpegFlag(BOOLEAN is_on)
{
	s_is_composed_jpeg_capture = is_on;
}

/*****************************************************************************/
//  Description:    get compound JPEG flag
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
BOOLEAN _ISP_ServiceGetCompoundJpegFlag(void)
{
	return s_is_composed_jpeg_capture;
}

/*****************************************************************************/
//  Description:    start capture compound JPEG
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStartCaptureCompoundJpeg(void)
{
	_ISP_ServiceSetCompoundJpegFlag(SCI_TRUE);
}

//Liangwen.zhen end


ISP_MPEG_PARAM_T mpeg_param = {0};
/*****************************************************************************/
//  Description:    set MPEG capture parameter
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetMpegCaptureParam(ISP_MPEG_PARAM_T * param_ptr)
{
    DECLARE_SERVICE_ENTRY(s);
		
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceSetMpegCaptureParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3674_112_2_18_1_44_46_891,(uint8*)"");
	
	//SCI_TRACE_LOW:"ISP_SERVICE: input.x = %d, input.y = %d,input.w = %d, input.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3677_112_2_18_1_44_46_892,(uint8*)"dddd",param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3680_112_2_18_1_44_46_893,(uint8*)"ddd",param_ptr->yuv_shift,param_ptr->encode_size.w,param_ptr->encode_size.h);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:lcd_id = %d,block_id = %d, disp_range.x = %d,disp_range.y = %d,disp_range.w = %d,disp_range.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3683_112_2_18_1_44_46_894,(uint8*)"dddddd",param_ptr->disp_lcd_id,param_ptr->disp_block_id,param_ptr->disp_range.x,param_ptr->disp_range.y,param_ptr->disp_range.w,param_ptr->disp_range.h);
	
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(_ISP_CheckMpegParam(param_ptr))
	{
		return SCI_ERROR;
	}
	
	SCI_MEMCPY(&mpeg_param,param_ptr,sizeof(ISP_MPEG_PARAM_T))
	ISP_ServiceSendRequest(0,ISP_SERVICE_SET_PARAM_SIG,ISP_SERVICE_MPEG,&mpeg_param);
		
	s->is_display = param_ptr->is_display;	
			
	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    enter MPEG mode
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceStartCaptureMpeg(void)
{
    DECLARE_SERVICE_ENTRY(s);
    
    uint32		status = 0;
    uint32		actual_event = 0;
    uint32		event = 0;
    LCD_BUF_INFO_T lcd_buf_t = {0};

	
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceStartCaptureMpeg"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3715_112_2_18_1_44_46_895,(uint8*)"");

   	ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_MPEG,0);

    
    event = ISP_DISPLAY_EVENT|ISP_MPEG_START_EVENT;
	s->is_first_frame = 1;
	encoder_done	  = 0; 
       
    status = SCI_GetEvent(isp_event, 
                      	  event, 
                       	  SCI_AND_CLEAR, 
                       	  &actual_event, 
                       	  ISP_WAIT_TIME);
                       	  
    LCD_SetInvalidateLimit(0,PERMIT_GUI_INVALIDATE);

	return status;
	//s->state = ISP_STATE_CAPTURE
}

void ISP_ServiceStopCapture(void)
{
    uint32 status = SCI_SUCCESS,actual_event;
    DECLARE_SERVICE_ENTRY(s);
    ISP_SIGNAL_T *sig_ptr;
	
    //SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceStopCapture"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3742_112_2_18_1_44_46_896,(uint8*)"");

    if(s->state != ISP_STATE_MPEG)
    {
        return ;
    }

    SCI_SuspendThread(ISP_SERVICE_THREAD);
    while (1)
    {
        sig_ptr = (void*)SCI_PeekSignal(ISP_SERVICE_THREAD);
        if (sig_ptr != PNULL)
        {
            SCI_FREE (sig_ptr);
        }
        else
            break;
    }   
    SCI_ResumeThread(ISP_SERVICE_THREAD);

    _ISP_Stop();

    ISP_ServiceSendRequest(0,ISP_SERVICE_STOP_SIG,0,0);
    status = SCI_GetEvent(isp_event, 
                     	  ISP_STOP_EVENT, 
                     	  SCI_AND_CLEAR, 
                     	  &actual_event, 
                     	  SCI_WAIT_FOREVER);
                                 	  
                                 	  
    return ;
}

/*****************************************************************************/
//  Description:    set scale parameter
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetScaleParam(ISP_SCLAE_PARAM_T * param_ptr)
{
	
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceSetScaleParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3782_112_2_18_1_44_46_897,(uint8*)"");
	
	//SCI_TRACE_LOW:"ISP_SERVICE:input_format = %d,input.x = %d, input.y = %d,input.w = %d, input.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3785_112_2_18_1_44_46_898,(uint8*)"ddddd",param_ptr->input_format, param_ptr->input_range.x,param_ptr->input_range.y,param_ptr->input_range.w,param_ptr->input_range.h);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:input_y_addr = 0x%x, input_u_addr = 0x%x, input_v_addr = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3788_112_2_18_1_44_46_899,(uint8*)"ddd", param_ptr->input_addr.yaddr, param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:output_format = %d, yuv_shift = %d,output.w = %d,output.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3791_112_2_18_1_44_46_900,(uint8*)"dddd",param_ptr->output_format,param_ptr->yuv_shift,param_ptr->output_size.w,param_ptr->output_size.h);
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(_ISP_CheckScaleParam(param_ptr))
	{
		return SCI_ERROR;
	}
	
	ISP_ServiceSendRequest(0,ISP_SERVICE_SET_PARAM_SIG,ISP_SERVICE_SCALE,param_ptr);

	return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    set vt encode parameter
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceSetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceSetVTEncodeParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3811_112_2_18_1_44_46_901,(uint8*)"");
	
	//SCI_TRACE_LOW:"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3814_112_2_18_1_44_46_902,(uint8*)"dddd",param_ptr->local_input_range.x,param_ptr->local_input_range.y,param_ptr->local_input_range.w,param_ptr->local_input_range.h);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:yuv_shift = %d,output.w = %d,output.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3817_112_2_18_1_44_46_903,(uint8*)"ddd",param_ptr->yuv_shift,param_ptr->encode_size.w,param_ptr->encode_size.h);
					
	//SCI_TRACE_LOW:"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x=%d, local_disp_range.y=%d, local_disp_range.w=%d, local_disp_range.h=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3821_112_2_18_1_44_46_904,(uint8*)"dddddd",param_ptr->local_disp_lcd_id,param_ptr->local_disp_block_id,param_ptr->local_disp_range.x, param_ptr->local_disp_range.y, param_ptr->local_disp_range.w, param_ptr->local_disp_range.h);
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(_ISP_CheckVTEncodeParam(param_ptr))
	{
		return SCI_ERROR;
	}
	
	ISP_ServiceSendRequest(0,ISP_SERVICE_SET_PARAM_SIG,ISP_SERVICE_VP_ENCODE,param_ptr);

	return SCI_SUCCESS;
}

uint32 ISP_ServiceGetVTEncodeParam(ISP_VT_ENCODE_PARAM_T * param_ptr)
{
	DECLARE_SERVICE_ENTRY(s);

    if(!param_ptr || s->service != ISP_SERVICE_VP)
        return SCI_ERROR;


    SCI_MEMCPY((void*)&(param_ptr->local_input_size), 
               (void*)&(s->input_size),
               sizeof(ISP_SIZE_T));
    
    SCI_MEMCPY((void*)&(param_ptr->local_input_range), 
               (void*)&(s->input_range),
               sizeof(ISP_RECT_T));
        
    SCI_MEMCPY((void*)&(param_ptr->encode_size), 
               (void*)&(s->encoder_size), 
               sizeof(ISP_SIZE_T));
    
    SCI_MEMCPY((void*)&(param_ptr->local_disp_range), 
               (void*)&(s->display_range), 
               sizeof(ISP_RECT_T));
    
    param_ptr->local_display_enable = s->local_display_enable;
    param_ptr->local_review_enable = s->local_review_enable;    
    
    param_ptr->local_disp_lcd_id = s->display_block.lcd_id;    
    param_ptr->local_disp_block_id = s->display_block.block_id;

	
    param_ptr->yuv_shift = s->output_yuv_shift;

    param_ptr->disp_rotation = s->display_rotation;

    return SCI_SUCCESS;
    
}

/*****************************************************************************/
//  Description:    set vt encode parameter
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/

uint32 ISP_ServiceSetVTDecodeParam(ISP_VT_DECODE_PARAM_T * param_ptr)
{
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceSetVTDecodeParam"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3881_112_2_18_1_44_46_905,(uint8*)"");
	
	//SCI_TRACE_LOW:"ISP_SERVICE:input.x = %d, input.y = %d,input.w = %d, input.h = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3884_112_2_18_1_44_46_906,(uint8*)"dddd",param_ptr->remote_input_range.x,param_ptr->remote_input_range.y,param_ptr->remote_input_range.w,param_ptr->remote_input_range.h);
						
	//SCI_TRACE_LOW:"ISP_SERVICE:local_disp_lcd_id=%d, local_disp_block_id=%d,local_disp_range.x=%d, local_disp_range.y=%d, local_disp_range.w=%d, local_disp_range.h=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3888_112_2_18_1_44_46_907,(uint8*)"dddddd",param_ptr->remote_disp_lcd_id,param_ptr->remote_disp_block_id,param_ptr->remote_disp_range.x, param_ptr->remote_disp_range.y, param_ptr->remote_disp_range.w, param_ptr->remote_disp_range.h);
	
	//SCI_TRACE_LOW:"ISP_SERVICE:deblock_enable = %d,y_input_addr = %d,u_input_addr = %d, v_input_addr = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3891_112_2_18_1_44_46_908,(uint8*)"dddd",param_ptr->deblock_enable,param_ptr->input_addr.yaddr,param_ptr->input_addr.uaddr,param_ptr->input_addr.vaddr);
	
	SCI_ASSERT(SCI_NULL!=param_ptr);
	
	if(_ISP_CheckVTDecodeParam(param_ptr))
	{
		return SCI_ERROR;
	}
	

	ISP_ServiceSendRequest(0,ISP_SERVICE_SET_PARAM_SIG,ISP_SERVICE_VP_DECODE,param_ptr);

	return SCI_SUCCESS;
}

LOCAL void _ISP_ServiceVTTimeout(void)
{
	DECLARE_SERVICE_ENTRY(s);
	s->is_first_frame = 1;  // stop review frame from remote ,see ISP_ServiceSetVTDecodeAddr
	//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceVTTimeout"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3910_112_2_18_1_44_46_909,(uint8*)"");
	ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_VP,0);
}

LOCAL void _ISP_ServiceTimerExpireHandle(uint32 param)
{
	uint32 curr_available	= 0;
	
	switch(s_isp_timer_mode)
	{
		case ISP_SERVICE_VT_PORTA_TIMEOUT:	

			curr_available = _ISP_GetQueueAvailable();
			if(curr_available < ISP_QUEUE_NUM -20)
			{
			    s_isp_performance = ISP_SERVICE_PERFORMANCE_LOW;
			}
			else if(curr_available < ISP_QUEUE_NUM - 10)
			{
			    s_isp_performance = ISP_SERVICE_PERFORMANCE_MID;
			}
			else if(curr_available > ISP_QUEUE_NUM - 5)
			{
			    s_isp_performance = ISP_SERVICE_PERFORMANCE_HIGH;
			}

			//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceTimerExpireHandle,s_isp_performance = %d, s_b_vt_watchdog_feeded = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3937_112_2_18_1_44_46_910,(uint8*)"dd",s_isp_performance,s_b_vt_watchdog_feeded);
			
			if(s_isp_performance != ISP_SERVICE_PERFORMANCE_LOW)
			{
				if(s_b_vt_watchdog_feeded)
				{
					s_b_vt_watchdog_feeded = 0;
				}
				else
				{
					_ISP_ServiceVTTimeout();
				}
			}
			break;
		default:
			//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceTimerExpireHandle,not supported timeout mode"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3952_112_2_18_1_44_46_911,(uint8*)"");
			break;
	}
}


LOCAL BOOLEAN _ISP_ServiceCreateTimer(void)
{
	BOOLEAN ret_val = SCI_FALSE;
	
	s_vt_timer_ptr = SCI_CreatePeriodTimer(
						"ISP_VT_TIMER",
					    _ISP_ServiceTimerExpireHandle,    			// Timer callback function
					    0,          							// Input value for timer callback function 
					    ISP_SERVICE_TIMEOUT,   	// Specifies the timer expire value in  milliseconds.                           
					    SCI_NO_ACTIVATE    						// Option to check if auto active the timer
                                								// after create.  
						);
						
	if(PNULL != s_vt_timer_ptr)
	{
		ret_val = SCI_TRUE;
	}
	
	//SCI_TRACE_LOW:"ISP_SERVICE : _ISP_VTCreateTimer, ret_val %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3976_112_2_18_1_44_46_912,(uint8*)"d", ret_val);
	
	return ret_val;
	
}
LOCAL void _ISP_ServiceCloseTimer(void)
{
	if(PNULL != s_vt_timer_ptr)
	{
		if( SCI_IsTimerActive(s_vt_timer_ptr))
		{
			SCI_DeactiveTimer(s_vt_timer_ptr);
			
			s_isp_timer_mode = ISP_SERVICE_TIMER_MODE_MAX;
			
			//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceCloseTimer"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_3991_112_2_18_1_44_46_913,(uint8*)"");
		}
	}
	
}
LOCAL void _ISP_ServiceDeleteTimer(void)
{
	if(PNULL != s_vt_timer_ptr)
	{
		SCI_DeleteTimer( s_vt_timer_ptr	);
		s_vt_timer_ptr = PNULL;
		
		//SCI_TRACE_LOW:"_DCAMERA_DeleteTimer"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4003_112_2_18_1_44_46_914,(uint8*)"");
	}
}

LOCAL BOOLEAN _ISP_ServiceStartTimer(
				ISP_SERVICE_TIMER_MODE_E 	timer_mode,
				uint32 					       expire_value
				)
{
	BOOLEAN ret_value = SCI_FALSE;
	
	if(PNULL != s_vt_timer_ptr)
	{
		if(SCI_SUCCESS == SCI_ChangeTimer(
						s_vt_timer_ptr,
						_ISP_ServiceTimerExpireHandle,
						expire_value							
						))
		{
			SCI_ActiveTimer(s_vt_timer_ptr);
			s_isp_timer_mode = timer_mode;
			ret_value = SCI_TRUE;
		}
		
	}
	
	//SCI_TRACE_LOW:"ISP_SERVICE: _ISP_ServiceStartTimer,: mode %d, expire value %d, ret_val, %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4029_112_2_18_1_44_46_915,(uint8*)"ddd", timer_mode, expire_value, ret_value);
	
	return ret_value;
}	


PUBLIC BOOLEAN ISP_ServiceIsVTOn(void)
{    
	DECLARE_SERVICE_ENTRY(s);

	return (s->service == ISP_SERVICE_VP);
}
void ISP_ServiceStartVT(void)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
    uint32	                status,actual_event;

    //SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceStartVT"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4047_112_2_18_1_44_46_916,(uint8*)"");
	
    if(ISP_STATE_PREVIEW == s->state)
    {
        isp->pause();
    }
    
    s->service = ISP_SERVICE_VP;
    s->is_first_frame = 1;
    s_b_vt_stop_arrived = SCI_FALSE;
   
    ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_VP,0);
    s_b_wait_for_vt_event = 1;							
    status = SCI_GetEvent(isp_event, 
    					ISP_VT_EVENT, 
    					SCI_AND_CLEAR, 
    					&actual_event, 
    					SCI_WAIT_FOREVER);

}

void ISP_ServiceStopVT(void)
{
	DECLARE_SERVICE_ENTRY(s);
    	uint32 status = SCI_SUCCESS,actual_event;
 	ISP_SIGNAL_T *sig_ptr;
	uint32 local_review_en, local_display_en;
	uint32 remote_review_en, remote_display_en;
	
	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceStopVT"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4076_112_2_18_1_44_46_917,(uint8*)"");
	if(s->state != ISP_STATE_VP)
	{
		return ;
	}
	s_b_vt_stop_arrived = SCI_TRUE;
	local_review_en = s->local_review_enable;
	local_display_en = s->local_display_enable;
	remote_review_en = s->remote_review_enable;
	remote_display_en = s->remote_display_enable;// reserve enable flags
	
	s->local_review_enable = 0;
	s->remote_review_enable = 0;
	s->local_display_enable = 0;
	s->remote_display_enable = 0;
	_ISP_ServiceCloseTimer();

	SCI_SuspendThread(ISP_SERVICE_THREAD);
	while (1)
	{
		sig_ptr = (void*)SCI_PeekSignal(ISP_SERVICE_THREAD);
		if (sig_ptr != PNULL)
		{
			SCI_FREE (sig_ptr);
		}
		else
			break;
	}   
	SCI_ResumeThread(ISP_SERVICE_THREAD);

	s_isp_performance = ISP_SERVICE_PERFORMANCE_HIGH; //the queue has been flushed ,it should be reset
	
	_ISP_EnterVTDisplay(); // if there being in review, wait untile review done
	
	_ISP_Stop();

	_ISP_ExitVTDisplay();

//    _ISP_ServiceCloseBlock();
	
   	ISP_ServiceSendRequest(0,ISP_SERVICE_STOP_SIG,0,0);
   	status = SCI_GetEvent(isp_event, 
                       	  ISP_STOP_EVENT, 
                       	  SCI_AND_CLEAR, 
                       	  &actual_event, 
                       	  SCI_WAIT_FOREVER);

	s->local_review_enable = local_review_en;
	s->local_display_enable = local_display_en;
	s->remote_review_enable = remote_review_en;
	s->remote_display_enable = remote_display_en;// recover enable flags
	
	return ;


}


/*****************************************************************************/
//  Description:    start scale
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceStartScale(void)
{
    uint32	                status,actual_event;

	//SCI_TRACE_LOW:"ISP_SERVICE:ISP_ServiceStartScale"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4142_112_2_18_1_44_47_918,(uint8*)"");

	ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_SCALE,0);
    status = SCI_GetEvent(isp_event, 
                          ISP_SCALE_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);
	
}

/*****************************************************************************/
//  Description:    set frame address
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/

ISP_ADDRESS_T s_addr_t;

uint32 ISP_ServiceSetVTDecodeAddr(ISP_ADDRESS_T *addr_ptr)
{
	DECLARE_SERVICE_ENTRY(s);
    uint32 status,actual_event;

	if(s->state != ISP_STATE_VP  || PNULL == addr_ptr)
		return 0xFFFFFFFF;		

	if(1 == s->is_first_frame)
	{
		//SCI_TRACE_LOW:"ISP_SERVICE: wait for first porta frame"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4170_112_2_18_1_44_47_919,(uint8*)"");
		return SCI_SUCCESS;
	}
	
	if(s_isp_performance == ISP_SERVICE_PERFORMANCE_LOW)
	{
                 //SCI_TRACE_LOW:"ISP_Service: Low performance ,skip the remote frame"
                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4176_112_2_18_1_44_47_920,(uint8*)"");
                 return SCI_SUCCESS;
	}
	else if(s_isp_performance == ISP_SERVICE_PERFORMANCE_MID)
	{
	    s_b_remote_lost = !s_b_remote_lost;
		
	    if(s_b_remote_lost) 
	    {
	        //SCI_TRACE_LOW:"ISP_Service: middle performance ,skip the remote frame"
	        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4185_112_2_18_1_44_47_921,(uint8*)"");
	        return SCI_SUCCESS;
	    }
	}
	
	//SCI_TRACE_LOW:"ISP_ServiceSetVTDecodeAddr , yaddr= 0x%x,uaddr = 0x%x,vaddr = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4191_112_2_18_1_44_47_922,(uint8*)"ddd",addr_ptr->yaddr,addr_ptr->uaddr,addr_ptr->vaddr);


	s->input_addr.yaddr = addr_ptr->yaddr;
	s->input_addr.uaddr = addr_ptr->uaddr;
	s->input_addr.vaddr = addr_ptr->vaddr;
//	SCI_TRACE_LOW("ISP_SERVICE: Remote, Set vp display request");
	ISP_ServiceSendRequest(0,ISP_SERVICE_START_SIG,ISP_SERVICE_VP_PLAYBACK,0);

    status = SCI_GetEvent(isp_event, 
                          ISP_VT_REMOTE_EVENT, 
                          SCI_AND_CLEAR, 
                          &actual_event, 
                          ISP_WAIT_TIME);

	return status;	
}

/*****************************************************************************/
//  Description:    register the event processing function to isp service
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceRegister(ISP_SERVICE_CALLBACK_ID_E eid, int (*func)(void *))
{
    DECLARE_SERVICE_ENTRY(s);

    switch(eid)
    {
        case ISP_SERVICE_CALLBACK_ENCODE:   
            
            ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTA);
            s->on_user_a = func; 
            ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTA);		
            
        break;

        case ISP_SERVICE_CALLBACK_DISPLAY:   
            
            ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTB);
            s->on_user_b = func; 
            ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTB);	
            
        break;
        
        default:
            
        SCI_ASSERT(0);
        
        break;
        
    }

    return SCI_SUCCESS;
}

/*****************************************************************************/
//  Description:    unregister the event processing function
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceUnRegister(ISP_SERVICE_CALLBACK_ID_E eid)
{
    uint32 status = SCI_SUCCESS;
    DECLARE_SERVICE_ENTRY(s);

    switch(eid)
    {
        case ISP_SERVICE_CALLBACK_ENCODE:   
            
            ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTA);				
            s->on_user_a = NULL; 
            ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTA);		
            
        break;

        case ISP_SERVICE_CALLBACK_DISPLAY:  
            
            ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTB);							
            s->on_user_b = NULL; 
            ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTB);	
            
        break;

        default:
            
            status = SCI_ERROR;
            SCI_ASSERT(0);
        break;
    }

    return status;
}


void ISP_ServiceSetClk(uint32 clk)
{
	 DECLARE_DRIVER_ENTRY(isp);
	 isp->ioctl(ISP_CFG_CLOCK, &clk);
}


void ISP_ServiceReviewBypass(void)
{
    DECLARE_SERVICE_ENTRY(s);
    LCD_INFO_T      lcd_info_ptr = {0};
    DISP_BLOCK_T    *disp_block_ptr = &(s->display_block);
    
    LCD_GetInfo(disp_block_ptr->lcd_id,&lcd_info_ptr);	
    LCD_InvalidateRect(s->display_block.lcd_id,
                       0,
                       0,
                       lcd_info_ptr.lcd_width- 1,
                       lcd_info_ptr.lcd_height- 1);		   

    return ;
	
}

LOCAL void _ISP_Display_End_Callback(uint32 address)
{
    DECLARE_DRIVER_ENTRY(isp);    
    ISP_FRAME_T *frame_ptr = (ISP_FRAME_T *)address;


    if(PNULL != frame_ptr)
    {
        isp->unlock(frame_ptr);
    }

    return ;
}

PUBLIC void _ISP_Set_LCD_BlockBuffer(LCD_ID_E lcd_id, 
                                     uint32 blk_num, 
                                     uint32 *buf_ptr,
                                     uint32 w,
                                     uint32 h)
{
#ifdef CHIP_ENDIAN_LITTLE

#define WORD_ENDIAN_SWAP(word) (((word & 0xFFFF)<<16) |((word & 0xFFFF0000) >>16))

    uint32 i, mid_data;
    uint32 *dst_buf_ptr = buf_ptr;

    for(i=0;i<w*h/2; i++)
    {
        mid_data = *dst_buf_ptr;
        *dst_buf_ptr++ =  WORD_ENDIAN_SWAP(mid_data);   
    }
      
#endif

    LCD_SetBlockBuffer(lcd_id,blk_num, buf_ptr);
	 	
}

LOCAL  void _ISP_ServicePortAJpeg(void* p_data,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);

    switch(s->state)
    {
        case ISP_STATE_CAPTURE_DONE:

            ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTA);				
            if(s->on_user_a)
            {
                s->on_user_a(&(s->encoder_addr[0]));
            }
            ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTA);				

        break;

        case ISP_STATE_CAPTURE_SCALE:

            s->input_addr.yaddr = s->encoder_frame->yaddr;
            s->input_addr.uaddr = s->encoder_frame->uaddr;
            s->input_addr.vaddr = s->encoder_frame->vaddr;

            if(s->encoder_size.w > ISP_SCALING_WIDTH_MAX)
            {
                _ISP_GetTotalSliceNum(s->encoder_size.w);
                s->input_size.w = s->input_range.w;
                s->input_size.h = s->input_range.h;
                s->input_range.x = 0;
                s->input_range.y = 0;
                s->input_image_format  = ISP_IMAGE_FORMAT_YUV420;
                s->output_image_format = ISP_IMAGE_FORMAT_YUV420;
                _ISP_StartSliceScale(0);
            }
            else
            {
                s->input_size.w = s->input_range.w;
                s->input_size.h = s->input_range.h;
                s->input_range.x = 0;
                s->input_range.y = 0;
                s->input_image_format  = ISP_IMAGE_FORMAT_YUV420;
                s->output_image_format = ISP_IMAGE_FORMAT_YUV420;																			
                _ISP_StartScale();
            }
            
        break;

        case ISP_STATE_CAPTURE_CFA:    	         

            s->input_addr.yaddr = s->encoder_frame->yaddr;
            s->input_addr.uaddr = s->encoder_frame->uaddr;
            s->input_addr.vaddr = s->encoder_frame->vaddr;
            s->input_size.w = s->input_range.w;
            s->input_size.h = s->input_range.h;
            s->input_range.x = 0;
            s->input_range.y = 0;
            s->output_image_format = ISP_IMAGE_FORMAT_YUV420;	
            _ISP_StartScale();

        break;

        case ISP_STATE_SCALE_SLICE:
            _ISP_StartSliceScale(1);
        break; 
    
    }
}

LOCAL  void _ISP_ServicePortAScaling(void* p_data,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);
    uint32                  status = 0;	

    switch(s->state)
    {
        case ISP_STATE_SCALE_DONE:
            
            ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTA);											
            if(s->on_user_a)
            {
                s->on_user_a(&(s->encoder_addr[0]));
            }
            ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTA);	
            status = SCI_SetEvent(isp_event, ISP_SCALE_EVENT, SCI_OR);

            
        break;

        case ISP_STATE_SCALE_SLICE:

            _ISP_StartSliceScale(1);

        break;

    }

}

LOCAL  void _ISP_ServicePortAMpeg(void* p_data,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);

    if(0 == encoder_done)
    {
        uint32 status = 0;
        //SCI_TRACE_LOW:"ISP: mpeg get two frame already"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4453_112_2_18_1_44_47_923,(uint8*)"");

        status = SCI_SetEvent(isp_event, ISP_MPEG_START_EVENT, SCI_OR);
        SCI_ASSERT(!status);
    }
    else
    {
        ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTA);
        if(s->on_user_a)
        {
            isp->lock(s->encoder_frame);
            s->on_user_a(s->encoder_frame);
        }
        ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTA);
    }

    encoder_done++;

}

LOCAL  void _ISP_ServicePortAVideoPhone(void* p_data,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);

    if(s->vp_deblock_enable)
    {
        ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTA);
        if(s->on_user_a)
        {
            isp->lock(s->encoder_frame);
            s->on_user_a(s->encoder_frame);
        }
        ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTA);														            					
    }
    else
    {
        if(s_b_vt_stop_arrived == SCI_FALSE)
        {
            s->encoder_frame = (ISP_FRAME_T *)p_data;
            isp->lock(s->encoder_frame);
            ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTA);
            if(s->on_user_a)
            {
                s->on_user_a(s->encoder_frame);
            }
            ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTA);
        }
        //SCI_TRACE_LOW:"ISP_SERVICE: To display local frame, s->local_review_enable = %d,s->is_first_frame = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4503_112_2_18_1_44_47_924,(uint8*)"dd",s->local_review_enable,s->is_first_frame);

        if( s->is_first_frame)
        {
            s->is_first_frame = 0;
            _ISP_ServiceStartTimer(ISP_SERVICE_VT_PORTA_TIMEOUT,ISP_VT_PORTA_TIMEOUT);

            if(s_b_wait_for_vt_event && !s->local_review_enable)
            {
                s_b_wait_for_vt_event = 0;
                SCI_SetEvent(isp_event, ISP_VT_EVENT, SCI_OR);		

            }
        }
        else
        {
            if(s->local_review_enable)
            {
                isp->ioctl(ISP_CFG_VP_DISP_REQ,&parameter);
                if(SCI_SUCCESS == _ISP_EnterVTDisplay())
                {
                    //SCI_TRACE_LOW:"ISP_SERVICE: display the local frame"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4524_112_2_18_1_44_47_925,(uint8*)"");
                    _ISP_SetVPFlag(0);
                    _ISP_StartVTDisplay();
                }
                else
                {
                    //SCI_TRACE_LOW:"ISP_SERVICE: timeout, don't display the local frame"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4530_112_2_18_1_44_47_926,(uint8*)"");
                }
            }
        }

    }
}


LOCAL  void _ISP_ServicePortAProcess(void* p_data,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);

    switch(s->service)
    {
        case ISP_SERVICE_JPEG:
            
            _ISP_ServicePortAJpeg(p_data,parameter);

            break;

        case ISP_SERVICE_SCALE:

            _ISP_ServicePortAScaling(p_data,parameter);
            
            break;

        case ISP_SERVICE_MPEG:

            _ISP_ServicePortAMpeg(p_data,parameter);

        break;

        case ISP_SERVICE_VP:

            _ISP_ServicePortAVideoPhone(p_data,parameter);
        break;

        default:
        	            			
        break;

    }
}

LOCAL uint32 _ISP_ServiceRotAngle2DrvAngle(ISP_ROTATION_E isp_angle)
{
    uint32 angle = ROTATION_DIR_MAX;
	
    switch(isp_angle)
    {
        case ISP_ROTATION_90:
            angle = ROTATION_90;
        	break;		
        case ISP_ROTATION_270:
            angle = ROTATION_270;
        	break;		
        case ISP_ROTATION_180:
            angle = ROTATION_180;
        	break;
		case ISP_ROTATION_MIRROR:
			angle = ROTATION_MIRROR;
			break;
        default:
        	break;
    }

    return angle;
}

LOCAL void _ISP_ServiceRotateRGBImage(uint32 src_addr, 
                                      uint32 dst_addr, 
                                      uint32 width,
                                      uint32 height,
                                      uint32 rot_angle)
{
	ROTATION_PARAM_T rotPara = {0};

    rotPara.data_format = ROTATION_RGB565;
    rotPara.rotation_mode = rot_angle;

    rotPara.src_addr.y_addr = src_addr;
    rotPara.dst_addr.y_addr = dst_addr;
	
    ispRotCvtWH(width, height, rot_angle, &rotPara);
	
	//ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceRotateRGBImage, input for Rotation_AppStart: {rotPara.rotation_mode=%d,rotPara.data_format=%d,rotPara.img_size.w=%d,rotPara.img_size.h=%d,rotPara.src_addr.y_addr=%d,rotPara.src_addr.uv_addr=%d,rotPara.src_addr.v_addr=%d}"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4623_112_2_18_1_44_47_927,(uint8*)"ddddddd",rotPara.rotation_mode,rotPara.data_format,rotPara.img_size.w,rotPara.img_size.h,rotPara.src_addr.y_addr,rotPara.src_addr.uv_addr,rotPara.src_addr.v_addr);
                      
    if(!Rotation_AppStart(&rotPara))
		//ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceRotateRGBImage, SUCCESS"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4626_112_2_18_1_44_47_928,(uint8*)"");
	else
		//ISP_SERVICE_TRACE:"ISP_SERVICE: _ISP_ServiceRotateRGBImage, FAIL"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4628_112_2_18_1_44_47_929,(uint8*)"");
		
	#if 0
    ROTATION_PARAMETERS_T   rotation_param = {0};
    
    rotation_param.desAddr = dst_addr;
    rotation_param.srcAddr = src_addr;
    rotation_param.width   = width;
    rotation_param.height  = height;
    rotation_param.mode = ROTATION_MOD_NORMAL;
    rotation_param.angel = rot_angle;
    
    rotation_param.pixFormat = ROTATION_PIXFORMAT_2BYTE;   
    
    ROTATION_Ratation(&rotation_param, SCI_NULL);
	#endif
}

// preview, review, mpeg preview,vt preview,vt review
LOCAL void _ISP_ServiceDisplayOneRGBFrame(uint32 block_id,ISP_FRAME_T *p_disp_frame,uint8 b_disp_en) 
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
    LCDC_RECT_T             lcd_rect = {0};
    uint32                  rot_addr = 0;
    uint16                  rot_tmp = 0;    
    uint32                  rot_angle = ISP_ROTATION_0;
    uint32                  lcd_id = 0;           
    DISP_BLOCK_T            *p_disp_block = SCI_NULL;
    uint32                  disp_addr = p_disp_frame->rgbaddr;

    if(block_id == s->display_block.block_id)
    {
        p_disp_block = &s->display_block;
        rot_addr = s->rotation_buf_addr[0];
        rot_tmp = s->display_rotation;
    }
    else
    {
        p_disp_block = &s->remote_display_block;
        rot_addr = s->remote_rotation_display_addr[0];  
        rot_tmp = s->remote_display_rotation;    
    }

    lcd_rect.x = (uint16)p_disp_block->img_rect.x;
    lcd_rect.y = (uint16)p_disp_block->img_rect.y;    
    lcd_rect.w = (uint16)p_disp_block->img_rect.w;
    lcd_rect.h = (uint16)p_disp_block->img_rect.h;	
    
    	
    if((uint16)ISP_ROTATION_0 != rot_tmp) //need rotation
    {
        rot_angle = _ISP_ServiceRotAngle2DrvAngle((ISP_ROTATION_E)rot_tmp);

        if(rot_angle != ROTATION_180)
        {
            rot_tmp = lcd_rect.w;
            lcd_rect.w = lcd_rect.h;
            lcd_rect.h = rot_tmp;  
        }
        //ISP_SERVICE_TRACE:"_ISP_ServiceDisplayOneRGBFrame ,rot_angle %d,rot_tmp %d,lcd_rect.w %d h %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4689_112_2_18_1_44_48_930,(uint8*)"dddd",rot_angle,rot_tmp,lcd_rect.w,lcd_rect.h);
        
        _ISP_ServiceRotateRGBImage(disp_addr, 
                                   rot_addr,
                                   lcd_rect.w, 
                                   lcd_rect.h,
                                   rot_angle);

        
        disp_addr = rot_addr;
		
    }


    if(b_disp_en)
    {
        //ISP_SERVICE_TRACE:"_ISP_ServiceDisplayOneRGBFrame ,lcd %d,block %d,addr 0x%x,p_disp_block->width %d,height %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4710_112_2_18_1_44_48_931,(uint8*)"ddddd",p_disp_block->lcd_id,block_id,disp_addr,p_disp_block->img_rect.w,p_disp_block->img_rect.h);
        
#ifndef USE_FMARK										  

        LCD_GetLcdCtrlSemaphore(0);
        _ISP_Set_LCD_BlockBuffer(p_disp_block->lcd_id,
                                 block_id,
                                 (uint32 *)disp_addr,
                                 lcd_rect.w,
                                 lcd_rect.h);
     
        if(p_disp_block->is_enable == 0)
        {
            LCD_EnableBlock(p_disp_block->lcd_id,block_id);
            p_disp_block->is_enable = 1;
        }


        LCD_InvalidateRect(p_disp_block->lcd_id,
                           p_disp_block->lcd_inv_rect.x, 
                           p_disp_block->lcd_inv_rect.y,
                           p_disp_block->lcd_inv_rect.x + p_disp_block->lcd_inv_rect.w - 1, 
                           p_disp_block->lcd_inv_rect.y + p_disp_block->lcd_inv_rect.h - 1); 
        
#else

        if(p_disp_block->is_enable == 0)
        {
            LCD_EnableBlock(p_disp_block->lcd_id,block_id);
            p_disp_block->is_enable = 1;
        }

        LCD_InvalidateRectWithFmark(p_disp_block->lcd_id,
                                    block_id,
                                    (uint32)disp_addr,
                                    (uint32)p_disp_frame,
                                     p_disp_block->lcd_inv_rect.x, 
                                     p_disp_block->lcd_inv_rect.y,
                                     p_disp_block->lcd_inv_rect.x + p_disp_block->lcd_inv_rect.w - 1, 
                                     p_disp_block->lcd_inv_rect.y + p_disp_block->lcd_inv_rect.h - 1); 
#endif
    }
	
}


LOCAL  void _ISP_ServicePortBPreview(ISP_FRAME_T* p_frame,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
    ISP_FRAME_T   frame_t = {0};
    
    uint32 status = 0;

    //ISP_SERVICE_TRACE:"ISP_Service, _ISP_ServicePortBPreview, 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4764_112_2_18_1_44_48_932,(uint8*)"d",p_frame->rgbaddr);
    
    if(ISP_DISPLAY_NONE != s->display_block.lcd_id)
    {
        isp->lock(p_frame);
        SCI_MEMCPY((void*)&frame_t, (void*)p_frame, sizeof(ISP_FRAME_T));
        _ISP_ServiceDisplayOneRGBFrame(s->display_block.block_id,&frame_t,SCI_TRUE);

        if(s->on_user_b)
        {
            s->on_user_b((void*)p_frame);
        }
        isp->unlock(p_frame);
    }
	
					
	if(s->is_first_frame)
	{	
		s->is_first_frame = 0;
		//ISP_SERVICE_TRACE:"ISP: display first frame"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4783_112_2_18_1_44_48_933,(uint8*)"");
		status = SCI_SetEvent(isp_event, ISP_DISPLAY_EVENT, SCI_OR);
		SCI_ASSERT(!status);
	}    
}

LOCAL  void _ISP_ServicePortBReview(ISP_FRAME_T* p_frame,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
    uint32        status = 0;
    DISP_BLOCK_T  *disp_block_ptr = &(s->display_block);
    ISP_FRAME_T   frame_t = {0};
    
    //ISP_SERVICE_TRACE:"ISP_SERVICE_PORTB_SIG before ,param = %d ,rgbaddr = 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4799_112_2_18_1_44_48_934,(uint8*)"dd",parameter,p_frame->rgbaddr);
    
    SCI_MEMCPY((void*)&frame_t, (void*)p_frame, sizeof(ISP_FRAME_T));
    _ISP_ServiceDisplayOneRGBFrame(s->display_block.block_id,&frame_t,SCI_TRUE);

    if(s->on_user_b)
    {
        s->on_user_b((void*)p_frame);
    }
    else
    {
        if(s->is_first_frame)
        {	
            s->is_first_frame = 0;
            //ISP_SERVICE_TRACE:"ISP: display first frame"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4813_112_2_18_1_44_48_935,(uint8*)"");
            status = SCI_SetEvent(isp_event, ISP_DISPLAY_EVENT, SCI_OR);
            SCI_ASSERT(!status);
        }
    }


}

LOCAL  void _ISP_ServicePortBJpeg(ISP_FRAME_T* p_frame,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
	ISP_SCLAE_PARAM_T	scale_param = {0};
    uint32              param_arr[5] = {0};
    LCD_INFO_T          lcd_info = {0};
    uint8*              buf_ptr = (uint8 *)((uint32)DC_GetReservedBufferEndAddr()-s->encoder_size.w*s->encoder_size.h*7*2);

    if((uint32)(DC_GetReservedBufferAddr())>(uint32)buf_ptr)
    {
        SCI_ASSERT(0);
    }
    
    if(_ISP_ServiceGetCompoundJpegFlag())
    {
        
        isp->stop();

        LCD_GetInfo(LCD_ID_0 , &lcd_info);

        param_arr[0] = (uint32)buf_ptr;
        param_arr[1] = s->input_range.x;
        param_arr[2] = s->input_range.y;
        param_arr[3] = s->input_range.w;
        param_arr[4] = s->input_range.h;

        ISP_BeforeGetdataFromLCDC(param_arr);	

        // right
        param_arr[3] = s->input_range.w + s->input_range.x - 1;
        // bottom
        param_arr[4] = s->input_range.h + s->input_range.y - 1;

        LCD_CompoundPhoto(LCD_ID_0,param_arr[1],param_arr[2],param_arr[3], param_arr[4]);

        ISP_AfterGetdataFromLCDC();					
        _ISP_ServiceSetCompoundJpegFlag(SCI_FALSE);			

        s->state = ISP_STATE_IDLETEMP;

        scale_param.input_size.w = s->input_range.w;
        scale_param.input_size.h = s->input_range.h;

        scale_param.input_range.x = s->input_range.x;
        scale_param.input_range.y = s->input_range.y;
        scale_param.input_range.w = s->input_range.w;
        scale_param.input_range.h = s->input_range.h;

        scale_param.input_format = ISP_IMAGE_FORMAT_RGB565;//s->input_image_format;

        scale_param.input_addr.yaddr = (uint32)buf_ptr;
        scale_param.input_addr.uaddr = (uint32)buf_ptr;
        scale_param.input_addr.vaddr = (uint32)buf_ptr;

        scale_param.output_size.w	= s->encoder_size.w;
        scale_param.output_size.h	= s->encoder_size.h;

        scale_param.output_format	= ISP_IMAGE_FORMAT_YUV420;

        scale_param.output_addr.yaddr	= 0;
        scale_param.output_addr.uaddr	= 0;
        scale_param.output_addr.vaddr	= 0;	

        scale_param.yuv_shift		= s->output_yuv_shift;

        _ISP_SetScaleParam(&scale_param);
        _ISP_StartScale();
        
        s_b_frame_photo_review = 1;
    }    
}

LOCAL  void _ISP_ServicePortBVideoPhone(ISP_FRAME_T* p_frame,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);
    ISP_FRAME_T   frame_t = {0};

    //SCI_TRACE_LOW:"ISP_Service: display %d frame, 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_4900_112_2_18_1_44_48_936,(uint8*)"dd", parameter, p_frame->rgbaddr);

    SCI_MEMCPY((void*)&frame_t, (void*)p_frame, sizeof(ISP_FRAME_T));

    if(parameter)
    {
         SCI_SetEvent(isp_event,ISP_VT_REMOTE_EVENT,SCI_OR);
        _ISP_ServiceDisplayOneRGBFrame(s->remote_display_block.block_id,&frame_t,s->remote_display_enable);
   
    }
    else
    {
        _ISP_ServiceDisplayOneRGBFrame(s->display_block.block_id,&frame_t,s->local_display_enable);
    }


    if(s->on_user_b)
    {
        (*s->on_user_b)((void*)p_frame);
    }


    if(s_b_wait_for_vt_event)
    {
        SCI_SetEvent(isp_event, ISP_VT_EVENT, SCI_OR);		
        s_b_wait_for_vt_event = 0;

    }


}


LOCAL  void _ISP_ServicePortBProcess(void* p_data,uint32 parameter)
{
    DECLARE_SERVICE_ENTRY(s);

  	switch(s->service)
	{
		case ISP_SERVICE_PREVIEW:
		case ISP_SERVICE_MPEG:
			      
            _ISP_ServicePortBPreview((ISP_FRAME_T*)p_data, parameter);
            
		break;
		
		case ISP_SERVICE_REVIEW:
         
            _ISP_ServicePortBReview((ISP_FRAME_T*)p_data, parameter);
				
		break;

	
        case ISP_SERVICE_JPEG:
            
            _ISP_ServicePortBJpeg((ISP_FRAME_T*)p_data, parameter);
            
        break;
          
		case ISP_SERVICE_SCALE:
         
            if(s->on_user_b)
            {
                s->display_frame.rgbaddr = s->display_addr[0];
                s->display_frame.width = s->display_range.w;
                s->display_frame.height = s->display_range.h;
                s->on_user_b(&s->display_frame);
            }
            
        break;
			
		case ISP_SERVICE_VP:

			_ISP_ServicePortBVideoPhone((ISP_FRAME_T*)p_data, parameter);

		break;
        
		default:

		break;
			
    }
}
static void _ISP_ServiceMainMessageLoop (uint32 argc, void *argv)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
    uint32 service,command ,param;
    void * ptr;
    ISP_SIGNAL_T *sig_ptr;
	ISP_SCLAE_PARAM_T	scale_param = {0};
	
	
    while(TRUE)
    {
        //Get signal from UPM's queue ;
        sig_ptr = (ISP_SIGNAL_T *)SCI_GetSignal(ISP_SERVICE_THREAD);
        
        SCI_ASSERT (sig_ptr);

        //Get variable from pointer ;
        service  = sig_ptr->sig.SignalCode;
        command  = sig_ptr->command ;
        param    = sig_ptr->param ;
        ptr      = sig_ptr->ptr ;
		
        //SCI_TRACE_LOW:"ISP: ISP_ServiceRecieveRequest,command=0x%x param=0x%x,ptr = 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5006_112_2_18_1_44_48_937,(uint8*)"ddd",command, param,ptr);

        if(ISP_STATE_CLOSED == s->state)
        {
            goto free_signal;
        }
		
        switch(command)
        {
            case ISP_SERVICE_OPEN_SIG:

                //SCI_TRACE_LOW:" ISP_ServiceOpen() !!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5017_112_2_18_1_44_48_938,(uint8*)"");

                //enable the isp clk.
                PWRMNG_ISP_ClkSwtch(TRUE);
                SCI_SetEvent(isp_event, ISP_OPEN_EVENT, SCI_OR);

            break;
			
            case ISP_SERVICE_CLOSE_SIG:

                if(ISP_DISPLAY_NONE != s->display_block.lcd_id)
                {
                    LCD_SetBrushMode(s->display_block.lcd_id, LCD_BRUSH_MODE_SYNC);
                    _ISP_Restorelcdblockinfo(s->display_block.lcd_id, s->display_block.block_id, 0);
                }				


                LCDC_RegisterInterrupt(2,NULL);
                SCI_SetEvent(isp_event, ISP_CLOSE_EVENT, SCI_OR);

            break;

            case ISP_SERVICE_PORTA_SIG:

                _ISP_ServicePortAProcess((void*)ptr, param);

            break;

			
			case ISP_SERVICE_PORTB_SIG:

                _ISP_ServicePortBProcess((void*)ptr, param);
                
				break;
    

            case ISP_SERVICE_SET_PARAM_SIG:
            {
                switch(param)
                {
                    case ISP_SERVICE_PREVIEW:
                    {
                        ISP_PREVIEW_PARAM_T * param_ptr = (ISP_PREVIEW_PARAM_T *)sig_ptr->ptr;
                        _ISP_SetPreviewParam(param_ptr);				
                        s->service = ISP_SERVICE_PREVIEW;
                        break;
                    }

                    case ISP_SERVICE_JPEG:
                    {

                        ISP_JPEG_PARAM_T * param_ptr = (ISP_JPEG_PARAM_T *)sig_ptr->ptr;
                        _ISP_SetJpegParam(param_ptr);				
                        s->service = ISP_SERVICE_JPEG;					
                        break;
                    }

                    case ISP_SERVICE_MPEG:
                    {
                        ISP_MPEG_PARAM_T * param_ptr = (ISP_MPEG_PARAM_T *)sig_ptr->ptr;
                        _ISP_SetMpegParam(param_ptr);	
                        s->service = ISP_SERVICE_MPEG;
                        break;
                    }

                    case ISP_SERVICE_REVIEW:
                    {
                        ISP_REVIEW_PARAM_T * param_ptr = (ISP_REVIEW_PARAM_T *)sig_ptr->ptr;
                        _ISP_SetReviewParam(param_ptr);	
                        s->service = ISP_SERVICE_REVIEW;
                        break;
                    }

                    case ISP_SERVICE_SCALE:
                    {
                        ISP_SCLAE_PARAM_T * param_ptr = (ISP_SCLAE_PARAM_T *)sig_ptr->ptr;
                        _ISP_SetScaleParam(param_ptr);				
                        s->service = ISP_SERVICE_SCALE;
                        break;
                    }

                    case ISP_SERVICE_VP_ENCODE:
                    {
                        ISP_VT_ENCODE_PARAM_T * param_ptr = (ISP_VT_ENCODE_PARAM_T *)sig_ptr->ptr;
                        _ISP_SetVTEncodeParam(param_ptr);
                        s->service = ISP_SERVICE_VP;				
                        break;
                    }

                    case ISP_SERVICE_VP_DECODE:
                    {
                        ISP_VT_DECODE_PARAM_T * param_ptr = (ISP_VT_DECODE_PARAM_T *)sig_ptr->ptr;
                        _ISP_SetVTDecodeParam(param_ptr);				
                        break;
                    }

                    default:
                    break;

                }
                break;
            }
			
            case ISP_SERVICE_START_SIG:
            {

                switch(param)
                {
                    case ISP_SERVICE_VP_PLAYBACK:

                        //SCI_TRACE_LOW:"ISP_SERVICE: To display remote frame,s->remote_review_enable = %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5128_112_2_18_1_44_48_939,(uint8*)"d",s->remote_review_enable);

                        if(s->remote_review_enable)
                        {
                            isp->ioctl(ISP_CFG_VP_DISP_REQ,&param);

                            if(SCI_SUCCESS == _ISP_EnterVTDisplay())
                            {
                                _ISP_SetVPFlag(1);
                                _ISP_StartVTDisplay();
                            }
                            else
                            {
                                //SCI_TRACE_LOW:"ISP_SERVICE: timeout, don't display the remote frame"
                                SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5141_112_2_18_1_44_48_940,(uint8*)"");

                            }
                        }

                    break;
                    
                    case ISP_SERVICE_PREVIEW:
                    {

                        LCD_SetBrushMode(s->display_block.lcd_id, LCD_BRUSH_MODE_ASYNC);
                        _ISP_StartPreview();

                        break;
                    }

                    case ISP_SERVICE_JPEG:
                    {
                        _ISP_StartJpeg();
                        break;
                    }

                    case ISP_SERVICE_MPEG:
                    {
                        _ISP_StartMpeg();
                        break;
                    }

                    case ISP_SERVICE_REVIEW:
                    {
                        uint32 actual_event = 0;

                        LCD_SetBrushMode(s->display_block.lcd_id, LCD_BRUSH_MODE_ASYNC);
                        _ISP_Reset();
                        _ISP_StartReview();

                        SCI_GetEvent(isp_event, 
                                     ISP_REVIEW_EVENT, 
                                     SCI_AND_CLEAR, 
                                     &actual_event, 
                                     500);
                        break;
                    }

                    case ISP_SERVICE_SCALE:
                    {

                        _ISP_StartScale();
                        break;
                    }

                    case ISP_SERVICE_VP:
                    {

                        LCD_SetBrushMode(s->display_block.lcd_id, LCD_BRUSH_MODE_ASYNC);

                        if(s->vp_deblock_enable)
                        {
                            _ISP_StartMpeg();
                        }
                        else
                        {
                            _ISP_StartVTEncode();
                        }
                        
                        break;
                    }

                    default:
                        
                    break;

                }
                break;
            }
			
            case ISP_SERVICE_STOP_SIG:
            {
                uint32 status = SCI_SUCCESS;

               // _ISP_ServiceCloseBlock();
                status = SCI_SetEvent(isp_event, ISP_STOP_EVENT, SCI_OR);
                break;
            }

            default:
            break;

        }


free_signal:

        //Release signal ;
        SCI_FREE (sig_ptr);
    }
}


/*****************************************************************************/
//  Description:    create a thread and its resource,register the thread to system and active it.
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
static void _ISP_ServiceCreate(void)
{
    //this function create a thread and its resource,
    //register the thread to system and active it.

    uint32 status;
	uint32 thread_pori =  SCI_PRIORITY_LOWEST + 1;
	
    //create 100ms timer, manual active
    /*
    ISP_ServiceTimerEntry = SCI_CreatePeriodTimer("ISP Service Timer",
        _ISP_ServiceOnTimer,0,100, SCI_NO_ACTIVATE);
    */
    //Create UPM Task ;
    status = SCI_CreateStaticThread(
        ISP_SERVICE_THREAD, //enum defined in "tasks_id.h"
        "TASK_ISP_SERVICE", 
        _ISP_ServiceMainMessageLoop, //local main message loop
        0, 0,
        _ISP_ServiceStack, ISP_STACK_SIZE,
        thread_pori, SCI_PREEMPT, "Q_ISP", 
        _ISP_ServiceQueue, ISP_QUEUE_NUM,
        SCI_AUTO_START);

    if (SCI_SUCCESS != status )
    {
        //error
    }

   
   // Create an isp event group     
   if (NULL == isp_event)
   {
		isp_event = SCI_CreateEvent("ISP_EVENT"); 
	}
}

/*****************************************************************************/
//  Description:    send request
//	Global resource dependence: 
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
int ISP_ServiceSendRequest(uint32 service,uint32 command,uint32 param,void * ptr)
{
	DECLARE_SERVICE_ENTRY(s);
	ISP_SIGNAL_T        *sig_ptr = NULL;
	xSignalHeaderRec    *sig;
	uint32 curr_queue_available	= 0;



	curr_queue_available = _ISP_GetQueueAvailable();

	if (curr_queue_available>=5)
	{
		sig_ptr = (ISP_SIGNAL_T *)SCI_ALLOC(sizeof(ISP_SIGNAL_T));
		SCI_ASSERT(SCI_NULL != sig_ptr);

		sig = &sig_ptr->sig;
		sig->SignalSize = sizeof(ISP_SIGNAL_T);
		sig->Sender     = ISP_SERVICE_THREAD;
		sig->SignalCode = service & 0x0FFFFUL;
		sig_ptr->command        = command ;
		sig_ptr->param          = param ;
		sig_ptr->ptr            = ptr ;

		if (SCI_SUCCESS == SCI_SendSignal((xSignalHeader)sig_ptr, ISP_SERVICE_THREAD))
		{
		//	return 0;
		}
		else
		{
			SCI_ASSERT (FALSE); 
		}
	}
	else
	{
		//SCI_TRACE_LOW:"ISP_ServiceSendRequest: curr_avliable = %d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5320_112_2_18_1_44_49_941,(uint8*)"d",curr_queue_available);

		if(((command == ISP_SERVICE_PORTA_SIG)&&((ISP_SERVICE_VP == s->service) || (ISP_SERVICE_MPEG == s->service)))
			||((command == ISP_SERVICE_PORTB_SIG)&&((ISP_SERVICE_VP == s->service) || (ISP_SERVICE_MPEG == s->service)||(ISP_SERVICE_PREVIEW == s->service)))
			||(command == ISP_SERVICE_AWBAE_SIG)||(command == ISP_SERVICE_AF_DONE_SIG))
    	{
    		//discard the message
    		//SCI_TRACE_LOW:"ISP_ServiceSendRequest: queue full not send the signal "
    		SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5327_112_2_18_1_44_49_942,(uint8*)"");
    		
		//	return 0;
    	}
    	else
    	{
    		//wait until queue avilbe
    		
    		while(curr_queue_available<5)
    		{
    			SCI_ASSERT (FALSE); 
    		//	curr_queue_available = _ISP_GetQueueAvailable();
    		}
    		
    		sig_ptr = (ISP_SIGNAL_T *)SCI_ALLOC(sizeof(ISP_SIGNAL_T));
		    SCI_ASSERT(SCI_NULL != sig_ptr);

		    sig = &sig_ptr->sig;
		    sig->SignalSize = sizeof(ISP_SIGNAL_T);
		    sig->Sender     = ISP_SERVICE_THREAD;
		    sig->SignalCode = service & 0x0FFFFUL;
		    sig_ptr->command        = command ;
		    sig_ptr->param          = param ;
		    sig_ptr->ptr            = ptr ;
		    
	    	if (SCI_SUCCESS == SCI_SendSignal((xSignalHeader)sig_ptr, ISP_SERVICE_THREAD))
		    {
		        return 0;
		    }
		    else
		    {
				SCI_ASSERT (FALSE); 
		    }
    	}
    }
	return 0;
}



/*****************************************************************************/
//  Description:    open isp service	
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceOpen()
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);
    uint32 param  = 0;

    if(s->state != ISP_STATE_CLOSED && s->service != ISP_SERVICE_IDLE)
    {
        //SCI_TRACE_LOW:"ISP_Service, Not closed since last opened"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5379_112_2_18_1_44_49_943,(uint8*)"");
        return SCI_ERROR ;
    }

    //SCI_TRACE_LOW:"ISP_ServiceOpen"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5383_112_2_18_1_44_49_944,(uint8*)"");

    s->service = ISP_SERVICE_IDLE;
    s->state = ISP_STATE_STOP;

    //review or playback
    s->input_addr.yaddr = 0;
    s->input_addr.uaddr = 0;
    s->input_addr.vaddr = 0;

    s->display_rotation = 0;
    s->rotation_cur_buf_ptr = PNULL;


    DPSLP_ISP_EnableAhbSleep(DISABLE_AHB_SLEEP);   

#ifdef USE_FMARK
    UpdateLcdCtrl_Init(TASK_FMARK_PRI);
    UpdateLcdCtrl_RegisterNoticeDone((lcddone_func_t)_ISP_Display_End_Callback);
#endif

    isp->poweron();

    ISP_ServiceSetClk(ISP_DEFAULT_CLK);

    if (CHNG_FREQ_REQ_HANDLER_NULL == s_isp_chng_freq_req_handler)
    { 
        s_isp_chng_freq_req_handler =  CHNG_FREQ_GetReqHandler("isp");
        //SCI_TRACE_LOW:"s_isp_chng_freq_req_handler = %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5411_112_2_18_1_44_49_945,(uint8*)"d",s_isp_chng_freq_req_handler);
    }


    _ISP_ServiceCreateTimer();	
    CHNG_FREQ_SetArmClk(s_isp_chng_freq_req_handler,FREQ_INDEX_ISP_HIGH);

    param = SCI_FALSE;
    isp->ioctl(ISP_SWITCH_BUFFER,&param),

    //enable isp_interrupt
    param = SCI_TRUE;
    isp->ioctl(ISP_CFG_INT_ENABLE,&param);

    DC_GetReservedBufferAddr();

    return 0;
}

/*****************************************************************************/
//  Description:    stop isp service
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
uint32 ISP_ServiceClose(void)
{
    DECLARE_SERVICE_ENTRY(s);
    DECLARE_DRIVER_ENTRY(isp);

    uint32 actual_event = 0;
    uint32 status = 0;
    uint32 param  = 0;
    ISP_SIGNAL_T *sig_ptr;

    if(ISP_STATE_CLOSED == s->state)
    {
        return SCI_SUCCESS;
    }
    //SCI_TRACE_LOW:"ISP_ServiceClose, service = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5448_112_2_18_1_44_49_946,(uint8*)"d", s->service);


    //_ISP_ServiceStopTimer();
    s->vp_remote_ok = 0;
    isp->stop();
    isp->poweroff();


    param = SCI_TRUE;
    isp->ioctl(ISP_SWITCH_BUFFER,&param),

    //enable isp_interrupt
    param = SCI_FALSE;
    isp->ioctl(ISP_CFG_INT_ENABLE,&param);
    
    SCI_SuspendThread(ISP_SERVICE_THREAD);
    while (1)
    {
        sig_ptr = (void*)SCI_PeekSignal(ISP_SERVICE_THREAD);
        if (sig_ptr != PNULL)
        {
            SCI_FREE (sig_ptr);
        }
        else
            break;
    }   
    SCI_ResumeThread(ISP_SERVICE_THREAD);

    ISP_ServiceSendRequest(0,ISP_SERVICE_CLOSE_SIG,0,0);
    status = SCI_GetEvent(isp_event, 
                         ISP_CLOSE_EVENT, 
                         SCI_AND_CLEAR, 
                         &actual_event, 
                         SCI_WAIT_FOREVER);
    SCI_ASSERT(!status);
    
    if(s_isp_chng_freq_req_handler != CHNG_FREQ_REQ_HANDLER_NULL)
    {
        //SCI_TRACE_LOW:"restore ARMCLK"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,ISP_SERVICE_SC8800H_5487_112_2_18_1_44_49_947,(uint8*)"");
        CHNG_FREQ_RestoreARMClk(s_isp_chng_freq_req_handler);
        CHNG_FREQ_DeleteReqHandler(s_isp_chng_freq_req_handler); 
        s_isp_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
    }

    if(s_isp_portc_in_sema_ptr)
    {
        SCI_PutSemaphore(s_isp_portc_in_sema_ptr);//if any task suspended on this object, resume it
        SCI_DeleteSemaphore	(s_isp_portc_in_sema_ptr);
        s_isp_portc_in_sema_ptr = NULL;
    }

    _ISP_ServiceCloseBlock();

#ifdef USE_FMARK
    UpdateLcdCtrl_Close();
#endif						
    _ISP_ServiceDeleteTimer();

    ISP_ServiceSetClk(ISP_DEFAULT_CLK);

    ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTA);
    s->on_user_a = NULL;
    ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTA);	

    ISP_ServiceGetPortCallBackCtrl(ISP_SERVICE_PORTB);
    s->on_user_b = NULL;
    ISP_ServiceReleasePortCallBackCtrl(ISP_SERVICE_PORTB);	

    s->state = ISP_STATE_CLOSED;
    s->service = ISP_SERVICE_IDLE;

    DC_MemFree();
    s->rotation_cur_buf_ptr = PNULL;

    DPSLP_ISP_EnableAhbSleep(ENABLE_AHB_SLEEP);


    return SCI_SUCCESS;

}

/*****************************************************************************/
//  Description:    initialize isp service , install isp driver module to system  
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
int ISP_ServiceInit(void)
{
	DECLARE_SERVICE_ENTRY(s);



	object_clear(s);
	s->service = ISP_SERVICE_IDLE;

	if(!isp_busy)
	{

		_ISP_ServiceCreate();

		if(ISP_ModuleInit() < 0)
			return -1;
	}

	isp_busy++;
    s->state = ISP_STATE_CLOSED;
	return 0;
}

/*****************************************************************************/
//  Description:    exit isp service, remove isp driver module from system 
//	Global resource dependence: none
//  Author:         Benny.Zou
//	Note:           
/*****************************************************************************/
void ISP_ServiceExit(void)
{
    DECLARE_SERVICE_ENTRY(s);

    if(isp_busy)
    {
        //remove isp driver module from system
        ISP_ModuleExit();//remove driver

        //todo: remove sensor driver from system

        //todo: remove i2c driver from system

        s->service = -1;
    }
}


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
