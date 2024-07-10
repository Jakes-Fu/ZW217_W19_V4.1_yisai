/******************************************************************************
 ** File Name:        lcd_fota.c                                              *
 ** Description:                                                              *
 **    This file contains common driver for lcd modul                         *
 ** Author:         Jim zhang                                                 *
 ** DATE:           05/18/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/

/******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/18/2003     Jim.zhang        Create.                                   *
 ******************************************************************************/

#include "ms_ref_lcd_trc.h"
#include "sci_types.h"
#include "dal_lcd.h"
#include "os_api.h"
#include "chip.h"
#include "lcm_prod.h"
#include "tb_drv.h"
#include "ref_outport.h"
#include "lcdc_app.h"
#include "mmu.h"
#include "chng_freq.h"
#include "arm_mmu.h"
#include "lcd_cfg.h"
#include "sc_reg.h"
#include "display.h"
#include "ref_lcd.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define ENABLE_BLOCK		 1
#define DISABLE_BLOCK		 0
#define LCD_DONE_EVENT		 1
#define DISPLANE_WIDTH       640
#define DISPLANE_HEIGHT      640
#define DEFAULT_RESET_TIME   0x10
#define DEFAULT_RESET_LEVEL  0x0
#define LCDC_BLOCK_NUM 	     6
#define IS_USE_LCD_ROTATION  0
#define REG32(x)      (*((volatile uint32 *)(x)))

#define OUTPUT_LOG          0


extern void *MEM_GetMainLCDBuffer(void);
extern void *MEM_GetSubLCDBuffer(void);

LOCAL void LCD_GetSemaphore(SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag);
LOCAL void LCD_PutSemaphore(SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag);
LOCAL ERR_LCD_E LCD_SetParam(uint32 ahb_clk);
LOCAL void LCD_FreeSemaphore(void);
/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL   uint32              s_reg_flag[LCD_SUPPORT_MAX] ;
LOCAL   uint32              s_lcd_used_num = 0;     
LOCAL   BLOCK_T  			s_buffer_info[LCD_SUPPORT_MAX][BLOCK_SUM];
LOCAL   LCD_SPEC_INFO_T_PTR s_lcd_spec_info_ptr[LCD_SUPPORT_MAX];
LOCAL   LCD_PROPERTY_T      s_lcd_property_table[LCD_SUM];
LOCAL   LCD_DIRECT_E    	s_lcd_direct[LCD_SUPPORT_MAX] = {0};
LOCAL   SCI_SEMAPHORE_PTR   s_lcdc_sm_ptr;
LOCAL   SYS_STATE_E         s_sys_run_state = SYS_STATE_NORMAL;
LOCAL   BOOLEAN             s_is_cap_mode = SCI_FALSE;
LOCAL   uint32              s_debug_lcd_info[2][2] = {0};
LOCAL   uint32              s_save_ctrl_info[78]={0};
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
uint32                      s_lcd_index = 0;



/*****************************************************************************/
//Description: get system run state
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL SYS_STATE_E LCD_GetRunState()
{
	return s_sys_run_state;
} 
/*****************************************************************************/
//Description: data format convertion
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
//0:RGB888; 1:RGB666; 2:RGB565(RGB555 if OSD);3:RGB555;4:GREY,5:YUV422;6:YUV420;7:YUV400
__inline LCD_DATA_FORMAT_E _lcd_data_format_convert(uint32 format)
{
	LCD_DATA_FORMAT_E new_format = LCD_RGB565;
	switch(format)
	{
		case 0:
			new_format = LCD_RGB888;
			break;
		case 1:
			new_format = LCD_RGB666;
			break;
		case 2:
			new_format = LCD_RGB565;
			break;
		case 3:
			new_format = LCD_RGB555;
			break;
		case 4:
			new_format = LCD_GREY;
			break;
		case 5:
			new_format = LCD_YUV422;
			break;
		case 6:
			new_format = LCD_YUV420;
			break;
		case 7:
			new_format = LCD_YUV400;
			break;
		default:
			//SCI_TRACE_LOW:"_lcd_data_format_convert is error! format = %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_FOTA_123_112_2_18_1_45_10_93,(uint8*)"d",format);
			new_format = LCD_MAX;
			break;
	}
	return new_format;
}
/*****************************************************************************/
//Description:  LCD Reg Info
//Global resource dependence:
//Author: Younger.Yang
/*****************************************************************************/
LOCAL void LCD_RegInfo(LCD_ID_E lcd_id, LCD_SPEC_INFO_T_PTR lcd_spec_ptr)
{ 
    //support low device
    if(lcd_spec_ptr != s_lcd_spec_info_ptr[lcd_id])
	s_lcd_spec_info_ptr[lcd_id] = lcd_spec_ptr;

    s_reg_flag[lcd_id] = 1;
}
/*****************************************************************************/
//Description: brush lcd protection  
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL ERR_LCD_E ProtectionOperation(LCD_ID_E lcd_id)
{
	SCI_SEMAPHORE_PTR sm = NULL;
	uint32 			  ctrl_mode = 0;
	SYS_STATE_E       flag = 0;
	
	SCI_PASSERT((lcd_id < s_lcd_used_num), ("ProtectionOperation,lcd_id = %d", lcd_id));/*assert verified*/
		
	MMU_InvalideDCACHE ();		
    flag = LCD_GetRunState();

	ctrl_mode = s_lcd_property_table[lcd_id].lcd_spec.controller;	
	  	
	sm = s_lcd_property_table[lcd_id].semaphore;
	LCD_GetSemaphore(sm, flag);					
	LCD_GetSemaphore(s_lcdc_sm_ptr, flag);		
    
	if(s_lcd_property_table[lcd_id].sleep_state == SCI_TRUE)
	{					
		LCD_PutSemaphore(s_lcdc_sm_ptr, flag);			
		LCD_PutSemaphore(sm, flag);	
        //SCI_TRACE_LOW:"ProtectionOperation:err!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_FOTA_164_112_2_18_1_45_10_94,(uint8*)"");
		return ERR_LCD_OPERATE_FAIL;
	}
	if (PNULL == s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation)
	{		
		LCD_PutSemaphore(s_lcdc_sm_ptr, flag);
		LCD_PutSemaphore(sm, flag);	
		//SCI_TRACE_LOW:"---ProtectionOperation: lcd operation is NUL!---"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_FOTA_171_112_2_18_1_45_10_95,(uint8*)"");
		return ERR_LCD_POINTER_NULL;		
	}
	//s_lcd_property_table[lcd_id].state = LCD_BUSY;
	//SCI_LCDBacklightBrightless_EnableDeepSleep(DISABLE_DEEP_SLEEP);
	
//	DPSLP_Lcdc_EnableAhbSleep(DISABLE_AHB_SLEEP);
	CHNG_FREQ_Event_Clr(FREQ_CHNG_EVENT_LCDC,SYS_MODE_NORMAL);
	
	return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description: config block before update lcd
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
LOCAL void _ConfigBlock(
                               LCD_ID_E lcd_id, //ID of lcd to operate
                               uint16 left,     //left of the rectangle
                               uint16 top,      //top of the rectangle
                               uint16 right,    //right of the rectangle
                               uint16 bottom    //bottom of the rectangle.
					       )
{
    LCD_IMG_LAYER_PARAM_T layer_cfg = {0};
    LCD_OSD_LAYER_PARAM_T osd_cfg = {0};
    LCDC_RECT_T		          disp_rect = {0};
    int32 i = 0;
	
    LCDC_AppDisableAllLayer();
	
    for(i=0 ; i<LCD_LAYER_ID_MAX ; i++)
    {		
        if(i==0) //image layer
        {     
            if(s_buffer_info[lcd_id][i].is_enable)
            {
                layer_cfg.layer_en = TRUE;
                layer_cfg.src_base_addr.y_addr = (uint32)s_buffer_info[lcd_id][i].mem_ptr;
                layer_cfg.src_base_addr.uv_addr = (uint32)s_buffer_info[lcd_id][i].uv_addr;
                layer_cfg.format = _lcd_data_format_convert(s_buffer_info[lcd_id][i].cfg.resolution);
                layer_cfg.y_endian_switch = s_buffer_info[lcd_id][i].cfg.endian_switch;
                layer_cfg.src_size.w = s_buffer_info[lcd_id][i].cfg.width;
                layer_cfg.src_size.h = s_buffer_info[lcd_id][i].cfg.end_y-s_buffer_info[lcd_id][i].cfg.start_y+1;
                layer_cfg.src_trim_rect.x = 0;
                layer_cfg.src_trim_rect.y = 0;
                layer_cfg.src_trim_rect.w = s_buffer_info[lcd_id][i].cfg.end_x-s_buffer_info[lcd_id][i].cfg.start_x+1;
                layer_cfg.src_trim_rect.h = s_buffer_info[lcd_id][i].cfg.end_y - s_buffer_info[lcd_id][i].cfg.start_y+1;
                layer_cfg.disp_pos.x = s_buffer_info[lcd_id][i].cfg.start_x;
                layer_cfg.disp_pos.y = s_buffer_info[lcd_id][i].cfg.start_y;        		    
    	     }
            else
            {
                layer_cfg.layer_en = FALSE;
            }
    	    LCDC_AppConfigBlock(LCD_LAYER_IMAGE, (void*)&layer_cfg);
        
        }
        else//osd layer
        {
            if(s_buffer_info[lcd_id][i].is_enable)
            {
                osd_cfg.layer_en = TRUE;
                osd_cfg.alpha = (uint8)s_buffer_info[lcd_id][i].cfg.alpha;
                osd_cfg.alpha_base_addr = (uint32)s_buffer_info[lcd_id][i].alpha_addr;
                osd_cfg.alpha_mode = (LCD_OSD_ALPHA_E)s_buffer_info[lcd_id][i].cfg.alpha_sel;
                osd_cfg.colorkey = s_buffer_info[lcd_id][i].cfg.colorkey;
                osd_cfg.colorkey_en = s_buffer_info[lcd_id][i].cfg.colorkey_en;
                osd_cfg.disp_pos.x = s_buffer_info[lcd_id][i].cfg.start_x;
                osd_cfg.disp_pos.y = s_buffer_info[lcd_id][i].cfg.start_y;
                osd_cfg.format =  _lcd_data_format_convert(s_buffer_info[lcd_id][i].cfg.resolution);
                osd_cfg.grey_rgb = s_buffer_info[lcd_id][i].cfg.grey_rgb;
                osd_cfg.endian_switch = s_buffer_info[lcd_id][i].cfg.endian_switch;                
                osd_cfg.src_base_addr = (uint32)s_buffer_info[lcd_id][i].mem_ptr;               
                osd_cfg.src_size.w = s_buffer_info[lcd_id][i].cfg.width;
                osd_cfg.src_size.h = s_buffer_info[lcd_id][i].cfg.end_y-s_buffer_info[lcd_id][i].cfg.start_y+1;//config.end_y - config.start_y + 1;//bottom - top + 1;
                osd_cfg.src_trim_rect.x = 0;
                osd_cfg.src_trim_rect.y = 0;
                osd_cfg.src_trim_rect.w = s_buffer_info[lcd_id][i].cfg.end_x-s_buffer_info[lcd_id][i].cfg.start_x+1;
                osd_cfg.src_trim_rect.h = s_buffer_info[lcd_id][i].cfg.end_y - s_buffer_info[lcd_id][i].cfg.start_y+1;                        
    	   }
    	   else
    	   {
                osd_cfg.layer_en = FALSE;
    	   }
           LCDC_AppConfigBlock(i, (void*)&osd_cfg);            		  
        }
        disp_rect.x = left;
        disp_rect.y = top;
        disp_rect.w = right - left + 1;
        disp_rect.h = bottom - top + 1;
        LCDC_AppSetDisplayRect(&disp_rect);		
    }
}
/*****************************************************************************/
//Description:LCD Get Semaphore
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL void LCD_GetSemaphore(SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag)
{
	SCI_ASSERT( PNULL != sm );/*assert verified*/

    if((SCI_InThreadContext()) && (flag == SYS_STATE_NORMAL))
	{
	    if(sm == s_lcdc_sm_ptr)
	    {
	        s_debug_lcd_info[0][0] = SCI_IdentifyThread();
            s_debug_lcd_info[0][1]++;
	    }
        else
        {
            s_debug_lcd_info[1][0] = SCI_IdentifyThread();
            s_debug_lcd_info[1][1]++;
        }
		if( SCI_ERROR == SCI_GetSemaphore(sm, SCI_WAIT_FOREVER) )
		{
			//SCI_TRACE_LOW:"LCD:Get Semaphore Fail!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_FOTA_284_112_2_18_1_45_10_96,(uint8*)"");
		}        
	}
}
/*****************************************************************************/
//Description:LCD Put Semaphor
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL void LCD_PutSemaphore(SCI_SEMAPHORE_PTR sm, SYS_STATE_E flag)
{
	uint32 ret;
	
	SCI_ASSERT( PNULL != sm );	 /*assert verified*/   

	if((SCI_InThreadContext()) && (flag == SYS_STATE_NORMAL))
	{
	    if(sm == s_lcdc_sm_ptr)
	    {
	        s_debug_lcd_info[0][0] = 0;
            s_debug_lcd_info[0][1]--;
	    }
        else
        {
            s_debug_lcd_info[1][0] = 0;
            s_debug_lcd_info[1][1]--;
        }
		ret = SCI_PutSemaphore( sm );	
        SCI_ASSERT( ret == SCI_SUCCESS ); /*assert verified*/       
	}
	if(flag == SYS_STATE_INT)
	{
	    if(sm == s_lcdc_sm_ptr)
	    {
	        s_debug_lcd_info[0][0] = 0;
            s_debug_lcd_info[0][1]--;
	    }
        else
        {
            s_debug_lcd_info[1][0] = 0;
            s_debug_lcd_info[1][1]--;
        }
		ret = SCI_PutSemaphore( sm );	
	    SCI_ASSERT( ret == SCI_SUCCESS );   /*assert verified*/     
	}
}
/*****************************************************************************/
//Description: free semaphore when use LCDC by asyn fashion 
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
LOCAL void LCD_FreeSemaphore(void)
{				
    uint32 i = 0;	
	uint32 lcd_id;	
	LCD_OPERATIONS_T  *op_ptr;
    LCD_CAP_PARAM_T    cap_param = {0};

	lcd_id = s_lcd_index;   
    op_ptr = s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation;
       
	//s_lcd_property_table[lcd_id].state = LCD_READY;

	CHNG_FREQ_Event_Set(FREQ_CHNG_EVENT_LCDC,SYS_MODE_INT);
	op_ptr->lcd_SetBrushDirection((LCD_DIRECT_E)LCD_ANGLE_0);	
    cap_param.cap_en = FALSE;
    LCD_ConfigCapture(lcd_id,&cap_param);   

    
}
/*****************************************************************************/
//Description: LCD initialization
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Init(void)
{
	LCD_ID_E          lcd_id;
    uint16            lcd_width = 0;
	uint16            lcd_height = 0;
	LCD_INFO_T        lcd_info = {0};
	BLOCKCFG_T        cfg;
	LCDC_SIZE_T	      lcd_size = {0};
	LCDC_RECT_T	      lcd_disp_rect = {0};
	SCI_SEMAPHORE_PTR sm = SCI_NULL;	

    if(s_lcd_used_num != 0)
    {
        return ERR_LCD_NONE;
    }

	s_lcdc_sm_ptr   = SCI_CreateSemaphore("LCDC OPERATION SEMAPHORE", 1);
    s_sys_run_state = SYS_STATE_NORMAL;   

	//get lcd spec and reg the info
	for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
	{ 
		s_lcd_spec_info_ptr[lcd_id] = LCM_GetLcdSpecInfo(lcd_id);
		if(PNULL != s_lcd_spec_info_ptr[lcd_id])
		{
			SCI_ASSERT (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation);/*assert verified*/
			LCD_RegInfo   (lcd_id, s_lcd_spec_info_ptr[lcd_id]);

			LCDC_DrvSetCSAddr(s_lcd_spec_info_ptr[lcd_id]->cs_pin, s_lcd_spec_info_ptr[lcd_id]->cd_pin, lcd_id);
			LCDC_AppSetCSPin(lcd_id,s_lcd_spec_info_ptr[lcd_id]->cs_pin);
			s_lcd_used_num++;
		}
	}

	SCI_ASSERT( (s_lcd_used_num <= LCD_SUPPORT_MAX )&&(s_lcd_used_num != 0));/*assert verified*/

	LCDC_AppOpen();
	LCDC_AppSetFmark(0,LCD_FMARK_NONE, LCD_FMARK_HIGH);
	LCDC_AppSetBGColor(0);
	
    //begin by jianping.wang, below codes will be deleted.
	lcd_width  = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->width;
	lcd_height = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->height;
	
	lcd_size.w = DISPLANE_WIDTH;
	lcd_size.h = DISPLANE_HEIGHT;
	lcd_disp_rect.x = 0;
	lcd_disp_rect.y = 0;
	lcd_disp_rect.w = lcd_width;
	lcd_disp_rect.h = lcd_height;
	
	LCDC_AppSetLogicSize(&lcd_size);
	LCDC_AppSetDisplayRect(&lcd_disp_rect);
	LCDC_AppDisableDither();	

	for(lcd_id=0 ; lcd_id<s_lcd_used_num ; lcd_id++)
	{
		sm = SCI_CreateSemaphore("LCD ASYNC OPERATION SEMAPHORE", 1);
		
		SCI_ASSERT( PNULL != sm);/*assert verified*/
        s_lcd_property_table[lcd_id].semaphore = sm;
        s_lcd_property_table[lcd_id].brush_mode = LCD_BRUSH_MODE_SYNC;
   //     s_lcd_property_table[lcd_id].buffer = NULL;
        s_lcd_property_table[lcd_id].lcd_spec = *s_lcd_spec_info_ptr[lcd_id];
        s_lcd_property_table[lcd_id].operation = BLOCK_OPERATION;
   //     s_lcd_property_table[lcd_id].state = LCD_IDLE;
        s_lcd_property_table[lcd_id].sleep_state = SCI_FALSE;
	}

	//Reset LCD    
	LCD_Reset();
	//LCM RGBMODE
	LCD_SetRGBMode(MAIN_LCD_ID);
	//init LCM timing
	LCD_SetParam(CHIP_GetAhbClk());

	/* init lcd buffer */
	for(lcd_id=0;lcd_id<s_lcd_used_num;lcd_id++)
	{
		LCD_GetInfo(lcd_id,&lcd_info);
		cfg.colorkey   = 0;
		cfg.resolution = LCD_RESOLUTION_RGB565;
		cfg.start_x    = 0;
		cfg.start_y    = 0;
		cfg.end_x      = lcd_info.lcd_width -1;
		cfg.end_y      = lcd_info.lcd_height -1;
		cfg.width      = cfg.end_x - cfg.start_x +1;				
		cfg.colorkey_en = FALSE;
		cfg.alpha_sel = 1;
		cfg.alpha     = 0xff;
		cfg.priority  = 0;
		cfg.type      = 1;
		LCD_ConfigBlock(lcd_id, 0, cfg);
		LCD_SetBlockBuffer(lcd_id, 0, lcd_info.lcdbuff_ptr);
		LCD_EnableBlock(lcd_id, 0);

	}
    
	//init all the lcd support
	for (lcd_id = 0; lcd_id < s_lcd_used_num; lcd_id++)
	{
		if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
		{
			s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_Init();  
			s_lcd_property_table[MAIN_LCD_ID].sleep_state = FALSE;
		}
	}	    
    return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description: LCD initialization when assert
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InitInAssertRestMode(void)
{
	uint16		lcd_id;
    uint16      lcd_width = 0;
	uint16      lcd_height = 0;
	LCD_INFO_T	lcd_info = {0};
	BLOCKCFG_T  cfg;	
    LCDC_SIZE_T	lcd_size = {0};
	LCDC_RECT_T	lcd_disp_rect = {0};

    s_sys_run_state = SYS_STATE_ASSERT;
    s_lcd_used_num = 0;
    
	//get lcd spec and reg the info
	for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
	{ 
		s_lcd_spec_info_ptr[lcd_id] = LCM_GetLcdSpecInfo(lcd_id);
		if(PNULL != s_lcd_spec_info_ptr[lcd_id])
		{
			SCI_ASSERT (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation);/*assert verified*/
			LCD_RegInfo((LCD_ID_E)lcd_id, s_lcd_spec_info_ptr[lcd_id]);

			LCDC_DrvSetCSAddr(s_lcd_spec_info_ptr[lcd_id]->cs_pin, s_lcd_spec_info_ptr[lcd_id]->cd_pin, lcd_id);
			LCDC_AppSetCSPin(lcd_id,s_lcd_spec_info_ptr[lcd_id]->cs_pin);
			s_lcd_used_num++;
		}
	}	
    
    LCDC_AppClose();
	LCDC_AppOpen();
	LCDC_AppSetFmark(0,LCD_FMARK_NONE, LCD_FMARK_HIGH);
	LCDC_AppSetBGColor(0);
	      
	lcd_width  = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->width;
	lcd_height = s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->height;
	
	lcd_size.w = lcd_width;
	lcd_size.h = lcd_height;
	lcd_disp_rect.x = 0;
	lcd_disp_rect.y = 0;
	lcd_disp_rect.w = lcd_width;
	lcd_disp_rect.h = lcd_height;
	
	LCDC_AppSetLogicSize(&lcd_size);
	LCDC_AppSetDisplayRect(&lcd_disp_rect);
	LCDC_AppDisableDither();	

	for(lcd_id=0 ; lcd_id<s_lcd_used_num ; lcd_id++)
	{		
        s_lcd_property_table[lcd_id].brush_mode  = LCD_BRUSH_MODE_SYNC;
 //       s_lcd_property_table[lcd_id].buffer      = NULL;
        s_lcd_property_table[lcd_id].lcd_spec    = *s_lcd_spec_info_ptr[lcd_id];
        s_lcd_property_table[lcd_id].operation   = BLOCK_OPERATION;
  //      s_lcd_property_table[lcd_id].state       = LCD_IDLE;
        s_lcd_property_table[lcd_id].sleep_state = SCI_FALSE;
	}
	//Reset LCD    
	LCD_Reset();	
	//LCM RGBMODE
	LCD_SetRGBMode(MAIN_LCD_ID);
	//init LCM timing
	LCD_SetParam(CHIP_GetAhbClk());
	/* init lcd buffer */

	LCD_GetInfo(0,&lcd_info);
	cfg.colorkey   = 0;
	cfg.resolution = LCD_RESOLUTION_RGB565;
	cfg.priority   = 0;
	cfg.start_x    = 0;
	cfg.start_y    = 0;
	cfg.end_x      = lcd_info.lcd_width -1;
	cfg.end_y      = lcd_info.lcd_height -1 ;
	cfg.width      =  cfg.end_x - cfg.start_x +1;	
	cfg.colorkey_en = FALSE;
	cfg.alpha_sel   = 1;
	cfg.alpha       = 0xff;
	cfg.type        = 1;
    
	LCD_ConfigBlock(MAIN_LCD_ID, 0, cfg);
	LCD_SetBlockBuffer(MAIN_LCD_ID, 0, lcd_info.lcdbuff_ptr);
	LCD_EnableBlock(MAIN_LCD_ID, 0);
   
    LCD_DisableBlock(MAIN_LCD_ID, 1);
    LCD_DisableBlock(MAIN_LCD_ID, 2);
	LCD_DisableBlock(MAIN_LCD_ID, 3);
    LCD_DisableBlock(MAIN_LCD_ID, 4);
    LCD_DisableBlock(MAIN_LCD_ID, 5);

	//init all the lcd support
	if (PNULL != s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation)
	{			
		s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Init();
        s_lcd_property_table[MAIN_LCD_ID].sleep_state = SCI_FALSE;
	}    
	
	return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description: LCD initialization when assert
//Global resource dependence:
//Author: Tim.Zhu
/*****************************************************************************/
void LCD_Reset(void)
{
	if((PNULL == s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst)||
			(PNULL == s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation))
	{             
		LCDC_DrvResetLcd(DEFAULT_RESET_TIME,DEFAULT_RESET_LEVEL);
	}
	else if(PNULL != s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst)
	{
        s_lcd_spec_info_ptr[MAIN_LCD_ID]->spec_ptr->operation->lcd_Rst();
	}
}
/*****************************************************************************/
//Description:   get the lcd information.
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_GetInfo(
                                 LCD_ID_E lcd_id,            //ID of lcd to operate
                                 LCD_INFO_T *lcd_info_ptr    //lcd information struct pointer
                                )
{
    SCI_ASSERT(lcd_id < s_lcd_used_num);/*assert verified*/

	lcd_info_ptr->r_bitmask		  = 0xf800;
	lcd_info_ptr->g_bitmask		  = 0x07e0;
	lcd_info_ptr->b_bitmask		  = 0x001f;
	lcd_info_ptr->bits_per_pixel  = 16;
	lcd_info_ptr->contrast_min	  = 0x00;
	lcd_info_ptr->contrast_max    = 63;
	lcd_info_ptr->contrast_defaut = 0x0A;
	lcd_info_ptr->lcd_width		  = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->width;
	lcd_info_ptr->lcd_height	  = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->height;
	
	if(MAIN_LCD_ID == lcd_id)
	{
		lcd_info_ptr->lcdbuff_ptr = (void *)MEM_GetMainLCDBuffer();
	}
	else if(SUB_LCD_ID == lcd_id)
	{
		lcd_info_ptr->lcdbuff_ptr = (void *)MEM_GetSubLCDBuffer();
	}
	
	return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description:   invalidate a rectang with the internal lcd data buffer
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_InvalidateRect(
                                           LCD_ID_E lcd_id,//ID of lcd to operate
                                           uint16 left,     //left of the rectangle
                                           uint16 top,     //top of the rectangle
                                           uint16 right,     //right of the rectangle
                                           uint16 bottom    //bottom of the rectangle.
                                        )
{
	int32 ret = 0;
 	int32 i = 0;
	LCD_BRUSH_MODE_E brush_mode;
	uint32 lcm_data_num = 0;
      		
	s_lcd_index = lcd_id; 	
	    
    s_lcd_property_table[lcd_id].lcd_spec.spec_ptr->operation->lcd_InvalidateRect(left, top, right, bottom);
          
    _ConfigBlock(lcd_id,left,top,right,bottom);
 
    MMU_InvalideDCACHE();              
        	
    LCDC_AppSetCheckState(TRUE);
    LCDC_AppCloseInt(LCD_INT_LCDC_DONE);           
    //SCI_TRACE_LOW:"LCD_Invalidate:Check State"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_FOTA_634_112_2_18_1_45_11_97,(uint8*)"");
   	
	if (SCI_SUCCESS != LCDC_AppInvalidateRect(lcd_id,left, top, right, bottom))
	{
		SCI_PASSERT(0, ("lcd invalidate timeout"));/*assert verified*/
	}

	MMU_InvalideDCACHE();
			
    if(LCDC_AppGetCheckState())
    {
        LCD_FreeSemaphore();
    	LCDC_AppSetCheckState(FALSE);            
    }	    	
			
	return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description:   invalidate the whole lcd.
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_Invalidate(LCD_ID_E lcd_id)
{
	uint16 right  = 0;
	uint16 bottom = 0;
	LCD_INFO_T  lcd_info = {0};
	
	LCD_GetInfo(lcd_id,&lcd_info);
	right     = lcd_info.lcd_width-1;
	bottom = lcd_info.lcd_height-1;
	LCD_InvalidateRect(lcd_id, 0, 0, right, bottom);

	return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description:   entern/exit sleep mode
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC ERR_LCD_E  LCD_EnterSleep(
                                        LCD_ID_E lcd_id,    //ID of lcd to operate
                                        BOOLEAN is_sleep    //true:enter sleep mode; false:exit sleep mode
                                        )
{
 	ERR_LCD_E ret;
	SCI_SEMAPHORE_PTR sm = PNULL;
    SYS_STATE_E flag = LCD_GetRunState();
	SCI_ASSERT(lcd_id < s_lcd_used_num);/*assert verified*/

    if (PNULL != s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation)
    {
        if(s_lcd_property_table[lcd_id].sleep_state != (uint32)is_sleep)
        {             
            sm = s_lcd_property_table[lcd_id].semaphore;
            LCD_GetSemaphore(sm, flag);
            LCD_GetSemaphore(s_lcdc_sm_ptr, flag);
            
            s_lcd_property_table[lcd_id].sleep_state = (uint32)is_sleep;                
            
			if(!is_sleep)
			{				
				LCDC_DrvEnable(SCI_TRUE);				
			}
			ret =  s_lcd_spec_info_ptr[lcd_id]->spec_ptr->operation->lcd_EnterSleep(is_sleep) ;

            if(is_sleep)
            {				
				LCDC_DrvEnable(SCI_FALSE);	
            }
            
            LCD_PutSemaphore(s_lcdc_sm_ptr, flag);
            LCD_PutSemaphore(sm, flag);
        }
        else
        {
            ret =  ERR_LCD_NONE;
        }
        return  ret;
    }
    else
    {
        ret = ERR_LCD_POINTER_NULL;
    }
    return ret;
}    
/*****************************************************************************/
//Description:  clear the whole lcd to one color. 
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC void LCD_Clear(
                          LCD_ID_E lcd_id,     //id of lcd to operate.
                          uint32 color        //color to fill the whole lcd.
                         )
{
	LCD_INFO_T lcd_info;
	uint16 *	dest_ptr;
	uint32 i;
	BLOCKCFG_T cfg;
	
	SCI_ASSERT(lcd_id < s_lcd_used_num);/*assert verified*/
    
	LCD_GetInfo(lcd_id,&lcd_info);

	dest_ptr = lcd_info.lcdbuff_ptr;
	
	for (i = 0; i < lcd_info.lcd_height * lcd_info.lcd_width; i++)
	{
		*dest_ptr++ = color;
	}
    LCDC_AppSetBGColor(0);
	cfg.colorkey         = 0;
	cfg.resolution       = 2;
	cfg.type             = 1; 
	cfg.priority         = 0;
	cfg.start_x          = 0;
	cfg.start_y          = 0;
	cfg.end_x            = lcd_info.lcd_width -1;
	cfg.end_y            = lcd_info.lcd_height -1 ;
	cfg.width            =  cfg.end_x - cfg.start_x +1;
	cfg.alpha            = 0xff;
	cfg.alpha_sel        = 1;
	cfg.colorkey_en      = FALSE;    
	LCD_ConfigBlock(lcd_id, 0, cfg);
    
	LCD_EnableBlock(lcd_id,0);
	LCD_DisableBlock(lcd_id,1);
	LCD_DisableBlock(lcd_id,2);
	LCD_DisableBlock(lcd_id,3);	
#ifdef PLATFORM_SC8800G
    LCD_DisableBlock(lcd_id,4);	
    LCD_DisableBlock(lcd_id,5);	
#endif
	LCD_SetBlockBuffer(lcd_id, 0, lcd_info.lcdbuff_ptr);
		
	LCD_Invalidate(lcd_id);
   
}

/**--------------------------------------------------------------------------*
 **                     Font library used in sci_assert                      *
 **--------------------------------------------------------------------------*/

//
//  8 X 16 ( width * height )
//
static const uint8 ascii_8X16[ 96 * 16] =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 
0x00, 0x48, 0x6C, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x7F, 0x12, 0x12, 0x12, 0x7F, 0x12, 0x12, 0x12, 0x00, 0x00, 
0x00, 0x00, 0x08, 0x1C, 0x2A, 0x2A, 0x0A, 0x0C, 0x18, 0x28, 0x28, 0x2A, 0x2A, 0x1C, 0x08, 0x08, 
0x00, 0x00, 0x00, 0x22, 0x25, 0x15, 0x15, 0x15, 0x2A, 0x58, 0x54, 0x54, 0x54, 0x22, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0C, 0x12, 0x12, 0x12, 0x0A, 0x76, 0x25, 0x29, 0x11, 0x91, 0x6E, 0x00, 0x00, 
0x00, 0x06, 0x06, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x00, 
0x00, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x1C, 0x1C, 0x6B, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x04, 0x03, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 
0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x08, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x20, 0x20, 0x10, 0x08, 0x04, 0x42, 0x7E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x20, 0x18, 0x20, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x20, 0x30, 0x28, 0x24, 0x24, 0x22, 0x22, 0x7E, 0x20, 0x20, 0x78, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x38, 0x24, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7E, 0x22, 0x22, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0x40, 0x24, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x04, 
0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x46, 0x40, 0x20, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1C, 0x22, 0x5A, 0x55, 0x55, 0x55, 0x55, 0x2D, 0x42, 0x22, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x42, 0x42, 0xE7, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x1E, 0x22, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x01, 0x01, 0x01, 0x01, 0x01, 0x42, 0x22, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1F, 0x22, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x42, 0x42, 0x3F, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x01, 0x01, 0x01, 0x71, 0x21, 0x22, 0x22, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0F, 
0x00, 0x00, 0x00, 0x77, 0x22, 0x12, 0x0A, 0x0E, 0x0A, 0x12, 0x12, 0x22, 0x22, 0x77, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x77, 0x36, 0x36, 0x36, 0x36, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x6B, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xE3, 0x46, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x52, 0x62, 0x62, 0x47, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x4D, 0x53, 0x32, 0x1C, 0x60, 0x00, 
0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x3E, 0x12, 0x12, 0x22, 0x22, 0x42, 0xC7, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x02, 0x04, 0x18, 0x20, 0x40, 0x42, 0x42, 0x3E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7F, 0x49, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x22, 0x24, 0x24, 0x14, 0x14, 0x18, 0x08, 0x08, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x6B, 0x49, 0x49, 0x49, 0x49, 0x55, 0x55, 0x36, 0x22, 0x22, 0x22, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x77, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x7E, 0x21, 0x20, 0x10, 0x10, 0x08, 0x04, 0x04, 0x42, 0x42, 0x3F, 0x00, 0x00, 
0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00, 
0x00, 0x00, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40, 
0x00, 0x1E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x00, 
0x00, 0x38, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 
0x00, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x78, 0x44, 0x42, 0x42, 0xFC, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x02, 0x02, 0x02, 0x44, 0x38, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x40, 0x78, 0x44, 0x42, 0x42, 0x42, 0x64, 0xD8, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x7E, 0x02, 0x02, 0x42, 0x3C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0xF0, 0x88, 0x08, 0x08, 0x7E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x22, 0x22, 0x1C, 0x02, 0x3C, 0x42, 0x42, 0x3C, 
0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1E, 
0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x72, 0x12, 0x0A, 0x16, 0x12, 0x22, 0x77, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x92, 0x92, 0x92, 0x92, 0x92, 0xB7, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x26, 0x42, 0x42, 0x42, 0x22, 0x1E, 0x02, 0x07, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x44, 0x78, 0x40, 0xE0, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x4C, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x02, 0x3C, 0x40, 0x42, 0x3E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x30, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x42, 0x42, 0x42, 0x42, 0x62, 0xDC, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x14, 0x08, 0x08, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x49, 0x49, 0x55, 0x55, 0x22, 0x22, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x24, 0x18, 0x18, 0x18, 0x24, 0x6E, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x14, 0x18, 0x08, 0x08, 0x07, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x22, 0x10, 0x08, 0x08, 0x44, 0x7E, 0x00, 0x00, 
0x00, 0xC0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00, 
0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 
0x00, 0x06, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x06, 0x00, 
0x0C, 0x32, 0xC2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

};


LOCAL uint16 font_color = 0x00;

/*****************************************************************************/
//Description:  Set 8 pixels from(x,y) to(x+7,y). 
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC void LCD_Set8Pixels(uint16 cx, uint16 cy, uint8 bit_mask)
{
	uint16 *buffer_ptr = 0;
	uint16 i;
	LCD_INFO_T  lcd_info;
	BLOCKCFG_T cfg;
    uint16 lcd_width = 0;
       
	LCD_GetInfo(MAIN_LCD_ID,&lcd_info);
	buffer_ptr = lcd_info.lcdbuff_ptr;

    lcd_width = lcd_info.lcd_width;
	for (i=0; i<8; i++)
	{
		if ((bit_mask & (0x01<<i))!=0x00)
	    	*(buffer_ptr+cy*lcd_width+cx+i) = font_color; //( cx+i, cy, color);    
	}
}
/*****************************************************************************/
//Description:  display the text(assert information) when assert.(just english) 
//Global resource dependence:
//Author: Jim.zhang
/*****************************************************************************/
PUBLIC void LCD_DisplayText_Fota(uint16 x, uint16 y, char *text, uint16 len)
{
    uint16  pos_x = x, pos_y = y;
    uint16  offset, i;
    uint8  *data_ptr;
    char    ch;
    LCD_INFO_T  lcd_info;
    uint16 lcd_width = 0;
    uint16 lcd_height = 0;

    s_sys_run_state = SYS_STATE_ASSERT;
    s_is_cap_mode = SCI_FALSE;
    if(s_save_ctrl_info[77]==0)
    {
		SCI_MEMCPY((void*)&s_save_ctrl_info[0],(void*)0x20700000,304);
    }
    s_save_ctrl_info[76] = s_lcd_property_table[0].brush_mode;
    s_save_ctrl_info[77] += 1;
    
    //assert state
    LCD_InitInAssertRestMode();
           
    LCD_GetInfo(MAIN_LCD_ID,&lcd_info);
    lcd_width = lcd_info.lcd_width;
    lcd_height = lcd_info.lcd_height;
    
    SCI_MEMSET((void*)lcd_info.lcdbuff_ptr,0xff,lcd_width*lcd_height*2);
   
    if( (len==0) || (text == NULL) ) return;

    if ( x > (lcd_width-8) )
    {
           pos_x = 0;
           pos_y += 16;
    }
    
    if( pos_y > (lcd_height-16) ) return;
     
    s_lcd_property_table[MAIN_LCD_ID].lcd_spec.spec_ptr->operation->lcd_SetBrushDirection(LCD_DIRECT_NORMAL);
    while( len-- )
    {
        ch = *text++;
        if( (ch <= 0x20) || (ch & 0x80) )
        {
           offset = 0;  // invalid char, use the default char(' ')
        }
        else
        {
           offset = ((uint16)((uint8)ch - 0x20)) * 16;
        }

        data_ptr = (uint8 *)&ascii_8X16[offset];

        // show this char
        for( i = 0; i < 16; i++ )
        {
           LCD_Set8Pixels(pos_x, pos_y+i, *data_ptr++);
        }

        pos_x +=8;

        if( (pos_x + 8) > lcd_width ) // Next line.
        {
           pos_y += 16; 
           pos_x  = 0;
        }

        if ( pos_y > (lcd_height-16) ) break;
    }

    s_lcd_property_table[MAIN_LCD_ID].lcd_spec.spec_ptr->operation->lcd_InvalidateRect(0,0,lcd_width-1,lcd_height-1);
    LCD_EnableBlock(MAIN_LCD_ID, 0);
    LCD_InvalidateRect(MAIN_LCD_ID,0,0,lcd_width-1,lcd_height-1);
    GPIO_SetLcdBackLight (SCI_TRUE);/*lint !e718*/
}
/*****************************************************************************/
//Description:  set dest LCD's RGB mode to host
//Global resource dependence:
//Author: Younger.Yang
/*****************************************************************************/
PUBLIC void LCD_SetRGBMode(LCD_ID_E lcd_id)
{ 
	LCDC_DrvSetRGBMode(s_lcd_spec_info_ptr[lcd_id]->cs_pin,s_lcd_spec_info_ptr[lcd_id]->spec_ptr->bus_width);
}
/*****************************************************************************/
//Description:  set the timing of LCM
//Global resource dependence:
//Author: Younger.Yang
/*****************************************************************************/
LOCAL ERR_LCD_E LCD_SetParam(uint32 ahb_clk)
{
    uint32 lcd_id = 0;
    uint32 ret = 0;
     
	for (lcd_id = 0; lcd_id < LCD_SUPPORT_MAX; lcd_id++)
	{
	   if(s_reg_flag[lcd_id])
	   {	      
		   ret |= LCDC_DrvSetTiming(s_lcd_spec_info_ptr[lcd_id], ahb_clk);		   
	   }
	}

    return ret;
}
/**********************************************************************************************************/
//Description: send the command and data to LCD
//Global resource dependence: g_lcdc_mode
//Author: Jianping.wang
/**********************************************************************************************************/
PUBLIC int32 LCD_SendCmdData(uint32 cmd,uint32 lcd_data,uint32 lcd_id)
{
	LCDC_SendCmdData( cmd, lcd_data, lcd_id);
	
	return ERR_LCD_NONE;
}//end of LCDC_SendCmdData
/**********************************************************************************************************/
//Description: send command to lcd
//Global resource dependence: g_lcdc_mode
//Author: Jianping.wang
/**********************************************************************************************************/
PUBLIC int32 LCD_SendCmd(uint32 cmd,uint32 lcd_id)
{
	LCDC_SendCmd(cmd, lcd_id);
	return ERR_LCD_NONE;
}//end of LCDC_SendCmd
/**********************************************************************************************************/
//Description: send display data to LCD
//Global resource dependence: g_lcdc_mode
//Author: Jianping.wang
/**********************************************************************************************************/
PUBLIC int32 LCD_SendData(uint32 lcd_data,uint32 lcd_id)
{	
	LCDC_SendData( lcd_data, lcd_id);

	return ERR_LCD_NONE;
	
}//end of LCDC_SendData	
/*****************************************************************************/
//Description: configure block parameters  
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigBlock(
        								  LCD_ID_E lcd_id, 
        								  uint32 blk_num, 
        								  BLOCKCFG_T config
        							  )
{	
	SCI_SEMAPHORE_PTR sm = SCI_NULL;
	SYS_STATE_E       flag;

	SCI_ASSERT(lcd_id < s_lcd_used_num);	/*assert verified*/
    SCI_PASSERT((blk_num<BLOCK_SUM),("blk_num=%d",blk_num));/*assert verified*/
	SCI_PASSERT(((config.start_x< config.end_x) && (config.start_y< config.end_y)),    /*assert verified*/
	                    ("block position is error!s_x%d,e_x=%d,s_y=%d,e_y=%d",config.start_x,config.end_x,config.start_y,config.end_y));/*assert verified*/
      
	flag = LCD_GetRunState();	
	sm = s_lcd_property_table[lcd_id].semaphore;
	LCD_GetSemaphore(sm, flag);	

	s_buffer_info[lcd_id][blk_num].cfg.alpha = config.alpha;	
	s_buffer_info[lcd_id][blk_num].cfg.width = config.width;  
	s_buffer_info[lcd_id][blk_num].cfg.type  = config.type; 
	s_buffer_info[lcd_id][blk_num].cfg.start_x = config.start_x; 
	s_buffer_info[lcd_id][blk_num].cfg.start_y = config.start_y;
	s_buffer_info[lcd_id][blk_num].cfg.end_x = config.end_x;
	s_buffer_info[lcd_id][blk_num].cfg.end_y = config.end_y;	
	s_buffer_info[lcd_id][blk_num].cfg.colorkey  = config.colorkey;	   
	s_buffer_info[lcd_id][blk_num].cfg.resolution = config.resolution;	
	s_buffer_info[lcd_id][blk_num].cfg.colorkey_en = config.colorkey_en;
	s_buffer_info[lcd_id][blk_num].cfg.alpha_sel = config.alpha_sel;

	#ifdef CHIP_ENDIAN_LITTLE    
    switch(config.resolution)
    {
        case LCD_RESOLUTION_RGB565:
        case LCD_RESOLUTION_RGB555:
            s_buffer_info[lcd_id][blk_num].cfg.endian_switch = 2;
            break;
        default:
            s_buffer_info[lcd_id][blk_num].cfg.endian_switch = 0;
            break;
    }    
	#else
	s_buffer_info[lcd_id][blk_num].cfg.endian_switch = 0;
	#endif

	s_buffer_info[lcd_id][blk_num].cfg.grey_rgb = config.grey_rgb;
	
	LCD_PutSemaphore(sm, flag);
    	return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description: set display memory  
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBlockBuffer(
                                             LCD_ID_E lcd_id, 
                                             uint32 blk_num, 
                                             uint32 *buf_ptr
                                         )
{
	SCI_SEMAPHORE_PTR sm = SCI_NULL;
	SYS_STATE_E       flag;

	SCI_ASSERT(lcd_id < s_lcd_used_num);/*assert verified*/	
	SCI_ASSERT(blk_num < BLOCK_SUM);/*assert verified*/
	SCI_ASSERT(buf_ptr != PNULL);/*assert verified*/
       	      
	flag = LCD_GetRunState();	
	sm = s_lcd_property_table[lcd_id].semaphore;
	
	LCD_GetSemaphore(sm, flag);		
	s_buffer_info[lcd_id][blk_num].mem_ptr = buf_ptr;
	LCD_PutSemaphore(sm, flag);

	return ERR_LCD_NONE;
		
}
/*****************************************************************************/
//Description: Enable Block  
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_EnableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
	SCI_SEMAPHORE_PTR sm = SCI_NULL;
	SYS_STATE_E       flag;

	SCI_ASSERT(lcd_id < s_lcd_used_num);	/*assert verified*/
    SCI_ASSERT(blk_num<BLOCK_SUM);/*assert verified*/
	      
	flag = LCD_GetRunState();	
	sm = s_lcd_property_table[lcd_id].semaphore;
	
	LCD_GetSemaphore(sm, flag);		
	s_buffer_info[lcd_id][blk_num].is_enable = SCI_TRUE;
	LCD_PutSemaphore(sm, flag);
	
	return ERR_LCD_NONE;
}
/*****************************************************************************/
//  Description:disable block   
//	Global resource dependence:
//  Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_DisableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
	SCI_SEMAPHORE_PTR sm = SCI_NULL;
	SYS_STATE_E       flag;

	SCI_ASSERT(lcd_id < s_lcd_used_num);/*assert verified*/	
    SCI_ASSERT(blk_num<BLOCK_SUM);/*assert verified*/
	      
	flag = LCD_GetRunState();	
	sm = s_lcd_property_table[lcd_id].semaphore;

	LCD_GetSemaphore(sm, flag);	
	s_buffer_info[lcd_id][blk_num].is_enable = SCI_FALSE;
	LCD_PutSemaphore(sm, flag);
	
	return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description: set brush lcd fashion  
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SetBrushMode(LCD_ID_E lcd_id, LCD_BRUSH_MODE_E mode)
{       
	SCI_SEMAPHORE_PTR sm = SCI_NULL;
	SYS_STATE_E       flag;

	SCI_ASSERT(lcd_id < s_lcd_used_num);    /*assert verified*/
       	      
	flag = LCD_GetRunState();	
	sm = s_lcd_property_table[lcd_id].semaphore;
	
	LCD_GetSemaphore(sm, flag);	
	if(mode == s_lcd_property_table[lcd_id].brush_mode)
	{
		LCD_PutSemaphore(sm, flag);
		return ERR_LCD_NONE;
	}
    LCD_GetSemaphore(s_lcdc_sm_ptr,flag);

	s_lcd_property_table[lcd_id].brush_mode = mode;
    LCD_PutSemaphore(s_lcdc_sm_ptr,flag);
	LCD_PutSemaphore(sm, flag);
	
    return ERR_LCD_NONE;
	
}
/*****************************************************************************/
//Description: Update LCD Timing  
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_UpdateTiming(uint32 ahb_clk)
{
    uint32 i;
	uint32 cs_id = 0;
	BOOLEAN  lcd_sleep_flag = 0;
	SYS_STATE_E flag;	
	LCD_INFO_T 	lcd_info;
	SCI_SEMAPHORE_PTR   sm;	
	
	SCI_ASSERT((ahb_clk > 0) && (ahb_clk<100000000));/*assert verified*/

	//SCI_TRACE_LOW:"---LCD_UpdateTiming---: AHB Clock:%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,LCD_FOTA_1190_112_2_18_1_45_12_98,(uint8*)"d",ahb_clk);
	
	flag = LCD_GetRunState();		
	for( i=0 ; i<s_lcd_used_num ; i++)
	{
		if(s_lcd_property_table[i].sleep_state == SCI_FALSE)
		{
			lcd_sleep_flag = SCI_TRUE;
			break;
		}
	}
	if(lcd_sleep_flag == SCI_FALSE)
	{		
		LCD_GetSemaphore(s_lcdc_sm_ptr, flag);						
	}
	
	for( i=0 ; i<s_lcd_used_num ; i++)
	{				
		LCD_GetInfo(i, &lcd_info);
		cs_id = s_lcd_property_table[i].lcd_spec.cs_pin;	

		LCDC_DrvSetTiming(&s_lcd_property_table[i].lcd_spec,ahb_clk);
	}
    if(lcd_sleep_flag == SCI_FALSE)
	{
		LCD_PutSemaphore(s_lcdc_sm_ptr, flag);							
	}	

	return ERR_LCD_NONE;
}		
/*****************************************************************************/
//Description: config capture parameters of 6800h
//Global resource dependence:
//Author: Jianping.wang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_ConfigCapture (LCD_ID_E lcd_id, LCD_CAP_PARAM_T *param_ptr)
{
    SYS_STATE_E       flag;

    flag = LCD_GetRunState();
    LCD_GetSemaphore (s_lcdc_sm_ptr, flag);
    s_is_cap_mode = param_ptr->cap_en;
    LCDC_AppConfigCapture ( (LCDC_CAP_PARAM_T *) param_ptr);
    LCD_PutSemaphore (s_lcdc_sm_ptr, flag);
    return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description:Read data from lcm
//Global resource dependence: NONE
//Author: Younger.yang
/*****************************************************************************/
PUBLIC uint32 LCD_ReadRegVal(uint32 lcd_id, uint32 reg_addr)
{
	uint32 reg_val = 0;

    reg_val = LCDC_Read (lcd_id,reg_addr);
	return reg_val;
}
/*****************************************************************************/
//Description:
//Global resource dependence: NONE
//Author: Younger.yang
/*****************************************************************************/
PUBLIC ERR_LCD_E LCD_SimpleInit(LCD_SIMPLE_INIT_PARAM_T *param_ptr)
{
    ERR_LCD_E ret =ERR_LCD_NONE;

    LCD_SPEC_INFO_T mcu_spec = {0};
    LCDC_DrvSetCSAddr (param_ptr->cs_id,param_ptr->cd_pol,param_ptr->lcd_id);
    LCDC_AppSetCSPin (param_ptr->lcd_id,param_ptr->cs_id);

    LCDC_AppOpen();
    LCDC_AppSetFmark (param_ptr->lcd_id,LCD_FMARK_NONE, LCD_FMARK_HIGH);
    LCDC_DrvSetTimingInit();
    LCDC_DrvSetRGBMode (param_ptr->cs_id,param_ptr->lcd_bits);
    LCDC_DrvResetLcd (DEFAULT_RESET_TIME,DEFAULT_RESET_LEVEL);
	return ERR_LCD_NONE;
}
/*****************************************************************************/
//Description:    LCD delay
//Global resource dependence: 
//Author:         
/*****************************************************************************/
PUBLIC void LCD_Delayms (uint32 ms)	
{
	if (SCI_InThreadContext()) 
	{
		SCI_Sleep(ms);
	} 
	else 	
	{
		SCI_Sleep(ms);
	}
}
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

/* end of lcd.c*/


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif 

/* end of lcd.c*/

