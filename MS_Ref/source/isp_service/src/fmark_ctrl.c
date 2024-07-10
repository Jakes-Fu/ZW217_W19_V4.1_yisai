/******************************************************************************
 ** File Name:                                                     *
 ** Author:                                           *
 ** DATE:         6/25/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 6/25/2006           Create.                                     *
 ** 8/30/2006	 	  change									  *
 *****************************************************************************/
 
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_isp_service_trc.h"
#include "sci_types.h"
#include "os_api.h"
#include "tasks_id.h"
#include "dal_lcd.h"
#include "threadx_os.h"
#include "ref_tasks_def.h"
#include "isp_service.h"
#include "gpio_drv.h"

/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*        data information saved */
/*  	     ____________________________________________________________________                             */

/*        | flag | lcd_id | block_id | buf_addr | left | top | right | bottom |callback_param |                           */
/*  	     ____________________________________________________________________                             */

#define INFO_FLAG_INDEX				0
#define INFO_LCD_ID_INDEX				1
#define INFO_BLOCK_ID_INDEX			2
#define INFO_BUF_ADDR_INDEX			3
#define INFO_LEFT_INDEX					4
#define INFO_TOP_INDEX					5
#define INFO_RIGHT_INDEX				6
#define INFO_BOTTOM_INDEX				7
#define INFO_CALLBACKPARAM			8
#define SAVE_DATA_INFO_ARRAY_NUM		4  
#define SAVE_DATA_INFO_NUM			8 

#define TEST_GPIO_PIN  					85

typedef struct _updatelcdctrl_signal_t{
    xSignalHeaderRec    				sig;
    uint32        						command;
    uint32        						param_num;
    void          						*ptr;
}UPDATELCDCTRL_SIGNAL_T;

typedef enum 
{
	UPDATELCDCTRL_DATA_INFO_SIG 	= 1,
	UPDATELCDCTRL_UPDATE_OPERATION_SIG,
	UPDATELCDCTRL_FMARK_SIG,
	UPDATELCDCTRL_UPDATE_DONE_SIG,
	UPDATELCDCTRL_MAX_SIG
}UPDATELCDCTRL_SIG_E;

//thread context
#define UPDATELCDCTRL_STACK_SIZE  				       (1024 * 4)
#define UPDATELCDCTRL_QUEUE_NUM   				30
LOCAL uint32 task_stack[UPDATELCDCTRL_STACK_SIZE >> 2];
LOCAL uint32 task_queue[UPDATELCDCTRL_QUEUE_NUM * SCI_QUEUE_ITEM_SIZE];
#define UPDATELLCDCTRL_SET_DISPLAY_DATAINFO   0x1

typedef enum
{
	IDLE = 0,
	READY,
	BUSY
}UPDATELCDCTRL_STATE_E;




typedef struct
{
    uint32 save_display_data_info[SAVE_DATA_INFO_ARRAY_NUM][SAVE_DATA_INFO_NUM+1];
    int32 result;
    UPDATELCDCTRL_STATE_E state;
    uint16 save_data_buf_pos;
    uint16 display_buf_pos;
    uint32 buf_addr;
    uint32 callback_param;
    lcddone_func_t callback;
    SCI_EVENT_GROUP_PTR  event;
    uint32 thread_id;
    BOOLEAN is_use_fmark;
    uint8  padding0;//reserved
    uint8  padding1;//reserved
    uint8  padding2;//reserved
    uint32 valid_data_num;
    UPDATELCDCTRL_SIGNAL_T *sig_ptr;
}UPDATELCDCTRL_CONTEX_T;

LOCAL UPDATELCDCTRL_CONTEX_T  		s_context = {0};
/*lint -save -e773  */    
#define DECLARE_ENTRY(s) 		UPDATELCDCTRL_CONTEX_T *s=&s_context

extern ERR_LCD_E LCDC_GetBlockCtrl(LCD_ID_E lcd_id, uint32 block_id);
extern ERR_LCD_E LCDC_ReleaseBlockCtrl(LCD_ID_E lcd_id, uint32 block_id);
extern void _ISP_Set_LCD_BlockBuffer(
					  LCD_ID_E lcd_id, 
                                     uint32 blk_num, 
                                     uint32 *buf_ptr,
                                     uint32 w,
                                     uint32 h);
/*****************************************************************************/
//  Description:    send request
//	Global resource dependence: 
//  Author:         Jianping.Wang
//	Note:           
/*****************************************************************************/
LOCAL void _UpdateLcd_SendSig(uint32 command,uint32 param_num,void * ptr)
{

	UPDATELCDCTRL_SIGNAL_T        *sig_ptr = NULL;
	xSignalHeaderRec                        *sig;
	DECLARE_ENTRY(s);

	sig_ptr = (UPDATELCDCTRL_SIGNAL_T *)SCI_ALLOC(sizeof(UPDATELCDCTRL_SIGNAL_T));
	SCI_PASSERT((SCI_NULL != sig_ptr),("malloc sig fail!"));/*assert verified*/

	sig = &sig_ptr->sig;
	sig->SignalSize = sizeof(UPDATELCDCTRL_SIGNAL_T);
	sig->Sender       = s->thread_id;
	sig->SignalCode = s->thread_id & 0x0FFFFUL;
	sig_ptr->command    = command ;
	sig_ptr->param_num = param_num ;
	sig_ptr->ptr               = ptr ;
       //SCI_TRACE_LOW:"_UpdateLcd_SendSig: command=%d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_134_112_2_18_1_43_34_0,(uint8*)"d",command);
       
	if (SCI_SUCCESS != SCI_SendSignal((xSignalHeader)sig_ptr, s->thread_id))
	{
		SCI_PASSERT ((FALSE),("Send Signal fail!")); /*assert verified*/
	}
	
}
/****************************************************************************************/
// Description: set event
// Author: Jianping.Wang
/*****************************************************************************************/
LOCAL void _SetEvent(uint32 set_flags)
{
    DECLARE_ENTRY(s);
    if(PNULL!=s->event)
    {   
        SCI_SetEvent(s->event, set_flags, SCI_OR);
    }
}
/****************************************************************************************/
// Description:  get event
// Author: Jianping.Wang
/*****************************************************************************************/
LOCAL uint32 _GetEvent(uint32 requested_flags)
{
    DECLARE_ENTRY(s);
    uint32 actual_event = 0;
    uint32 status = SCI_ERROR;

    if(PNULL!=s->event)
    {    
        status = SCI_GetEvent(s->event, 
                                            requested_flags, 
                                            SCI_OR_CLEAR, 
                                            &actual_event, 
                                            SCI_WAIT_FOREVER);
    }
    
    return status;
}
/*****************************************************************************/
//  Description:    notice  invalidate lcd end
//  Author:         Jianping.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void UpdateLcdCtrl_NoticeLcdDone(uint32 param);
/**********************************************************************************************************/
// Description: register callback function 
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC void UpdateLcdCtrl_RegisterNoticeDone(lcddone_func_t func)
{
	DECLARE_ENTRY(s);
	SCI_PASSERT((func != PNULL),("Notice Done fun is NULL!"));/*assert verified*/
	s->callback = func;
}
/*****************************************************************************/
//  Description:    invalidate lcd 
//  Author:         Jianping.Wang
//	Note:           return 0 is success,other is error!
/*****************************************************************************/
PUBLIC int32 LCD_InvalidateRectWithFmark(uint32 lcd_id,
											 uint32 block_id,
											 uint32 buf_adddr,
											 uint32 callback_param,
											 uint16 left,
						                                    uint16 top,
						                                    uint16 right, 	
                                     						 uint16 bottom)
{
	DECLARE_ENTRY(s);
       uint32 info[SAVE_DATA_INFO_NUM] = {0};
       int32 ret = 0;

       info[0] = lcd_id;
       info[1] = block_id;
       info[2] = buf_adddr;
       info[3] = left;
       info[4] = top;
       info[5] = right;
       info[6] = bottom;
       info[7] = callback_param;

       //SCI_TRACE_LOW:"LCD_InvalidateRectWithFmark:%d,%d,%d,%d,%d,%d,%d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_210_112_2_18_1_43_34_1,(uint8*)"ddddddd", lcd_id,block_id,left,top,right,bottom,ret);
       //SCI_TRACE_LOW:"handle LCD_InvalidateRectWithFmark time = %d"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_211_112_2_18_1_43_34_2,(uint8*)"d",SCI_GetTickCount());
       LCD_RegisterCallBack(UpdateLcdCtrl_NoticeLcdDone);
       
       _UpdateLcd_SendSig(UPDATELCDCTRL_DATA_INFO_SIG,SAVE_DATA_INFO_NUM,&info[0]);
       
       _GetEvent(UPDATELLCDCTRL_SET_DISPLAY_DATAINFO);
       
       if(s->result !=0)
       {
           ret = -1;
       }
              
       return ret;
}

/*****************************************************************************/
//  Description:    search next data info isn't invalidated.
//  Author:         Jianping.Wang
//	Note:           
/*****************************************************************************/
LOCAL int32 _UpdateLcdCtrl_SearchNextBuf(void)
{
	DECLARE_ENTRY(s);
	uint32 temp = 0;
	int32 ret = -1;
	int32 count = 0;

	//SCI_TRACE_LOW:"_UpdateLcdCtrl_SearchNextBuf:pos=%d,%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_236_112_2_18_1_43_34_3,(uint8*)"dd",s->display_buf_pos);

	temp = (s->display_buf_pos == (SAVE_DATA_INFO_ARRAY_NUM-1)) ? 0 : (temp  + 1);

       do
       {
          //SCI_TRACE_LOW:"s->save_display_data_info[temp][INFO_FLAG_INDEX] is %d"
          SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_242_112_2_18_1_43_34_4,(uint8*)"d",s->save_display_data_info[temp][INFO_FLAG_INDEX]);
          if(s->save_display_data_info[temp][INFO_FLAG_INDEX])
          {
          	ret = temp;
              break;
          }
          else
          {
          	 count++;
               temp ++;
               if( temp>=SAVE_DATA_INFO_ARRAY_NUM)
               {
               	temp = 0;
               }
          }                          
      }while(count<=(SAVE_DATA_INFO_ARRAY_NUM+1));
       
      return ret;
}
/*****************************************************************************/
//  Description:    operation when invalidate lcd end
//  Author:         Jianping.Wang
//	Note:           
/*****************************************************************************/
LOCAL void _UpdateLcdCtrl_Done(void)
{
	DECLARE_ENTRY(s);
	int32 temp = 0;
	LCD_ID_E lcd_id = 0;
	uint32 blk_num = 0;
	uint32 addr = 0;
	
	temp = _UpdateLcdCtrl_SearchNextBuf();
	//SCI_TRACE_LOW:"_UpdateLcdCtrl_Done:next buf index = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_273_112_2_18_1_43_34_5,(uint8*)"d",temp);

	if(temp>=0)
	{	              
              _UpdateLcd_SendSig(UPDATELCDCTRL_UPDATE_OPERATION_SIG,1,(void*)temp);
	}
	else
	{
		s->state = IDLE;
		s->buf_addr = 0;	
		s->callback_param = 0;
		s->is_use_fmark = FALSE;
		//SCI_TRACE_LOW:"not saved data!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_285_112_2_18_1_43_34_6,(uint8*)"");
	}
}
/*****************************************************************************/
//  Description:    handle task message.
//  Author:         Jianping.Wang
//	Note:           
/*****************************************************************************/
LOCAL void _UpdateLcdCtrl_MessageLoop (uint32 argc, void *argv)
{
	DECLARE_ENTRY(s);
	UPDATELCDCTRL_SIGNAL_T        *sig_ptr = NULL;
	uint32 command;
	uint32 param_num;
	void *ptr;
	uint32 temp = 0;
	
	while (TRUE)
       {     
            s->sig_ptr = (UPDATELCDCTRL_SIGNAL_T *)SCI_GetSignal(s->thread_id);

            if(NULL==s->sig_ptr)
            {
                continue ;
            }
            
            sig_ptr = s->sig_ptr;
            command  = sig_ptr->command ;
            param_num = sig_ptr->param_num ;
            ptr      = sig_ptr->ptr ;
            SCI_FREE (s->sig_ptr);	
            s->sig_ptr = 0;
	     //SCI_TRACE_LOW:"_UpdateLcd_MessageLoop:command=0x%x param_num=0x%x"
	     SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_315_112_2_18_1_43_34_7,(uint8*)"dd",command, param_num);
	     
	     switch(command)
	     {
		case UPDATELCDCTRL_DATA_INFO_SIG:
			
                     if(s->valid_data_num < SAVE_DATA_INFO_ARRAY_NUM)
                     {
				s->valid_data_num++;
				temp = s->save_data_buf_pos;
                     }
                     else
                     {
                     	if((s->save_data_buf_pos+1)>=SAVE_DATA_INFO_ARRAY_NUM)
                     	{
					temp = 0;
                     	}
				else
				{
					temp = s->save_data_buf_pos+1;									
				}
				s->save_display_data_info[temp][INFO_FLAG_INDEX]=0;
				//SCI_TRACE_LOW:"---discard a frame data!--"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_337_112_2_18_1_43_34_8,(uint8*)"");
                     }

	              SCI_MEMCPY((void*)&s->save_display_data_info[temp][INFO_LCD_ID_INDEX],ptr,SAVE_DATA_INFO_NUM<<2);
	              s->save_display_data_info[temp][INFO_FLAG_INDEX]=1;
	              s->result = 0;	         
	              s->save_data_buf_pos++;
	              
	              if(s->save_data_buf_pos>=SAVE_DATA_INFO_ARRAY_NUM)
	              {
				s->save_data_buf_pos = 0;
	              }
                     
                     _SetEvent(UPDATELLCDCTRL_SET_DISPLAY_DATAINFO);
                     
                     if((s->state == IDLE)&&(s->result == 0))
                     {
			   s->state = BUSY;			 
			   _UpdateLcd_SendSig(UPDATELCDCTRL_UPDATE_OPERATION_SIG,1,(void*)temp);
                     }                     
               
			break;
		case UPDATELCDCTRL_UPDATE_OPERATION_SIG:
		{
			LCD_ID_E lcd_id = 0;
			uint32 blk_num = 0;
			uint32 *addr_ptr = PNULL;			

			temp = (uint32)ptr;
			s->display_buf_pos = temp;  		
			lcd_id     = s->save_display_data_info[temp][INFO_LCD_ID_INDEX];
			blk_num = s->save_display_data_info[temp][INFO_BLOCK_ID_INDEX];
			addr_ptr = (uint32 *)(s->save_display_data_info[temp][INFO_BUF_ADDR_INDEX]);
			
			LCDC_GetBlockCtrl(lcd_id,blk_num);				
		//    	LCD_SetBlockBuffer(lcd_id, blk_num, addr_ptr);
		    _ISP_Set_LCD_BlockBuffer(lcd_id, blk_num,addr_ptr,\
		     						 (s->save_display_data_info[temp][INFO_RIGHT_INDEX]-s->save_display_data_info[temp][INFO_LEFT_INDEX]+1), \
		     						 (s->save_display_data_info[temp][INFO_BOTTOM_INDEX]-s->save_display_data_info[temp][INFO_TOP_INDEX]+1));
		    	
                     s->buf_addr = (uint32)addr_ptr;		
                     s->callback_param = s->save_display_data_info[temp][INFO_CALLBACKPARAM];                                    
                 
                     //SCI_TRACE_LOW:"set s->is_use_fmark 1"
                     SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_380_112_2_18_1_43_34_9,(uint8*)"",SCI_GetTickCount());
                 	s->is_use_fmark = TRUE;
                 	
		}
			break;		
		case UPDATELCDCTRL_FMARK_SIG:
              	{
				LCD_BRUSH_MODE_E brush_mode;
				LCD_ID_E lcd_id = 0;
				uint32 blk_num = 0;
				uint16 left      = 0;
				uint16 right    = 0;
				uint16 top       = 0;
				uint16 bottom = 0;
				
				//SCI_TRACE_LOW:"handle UPDATELCDCTRL_FMARK_SIG time = %d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_395_112_2_18_1_43_34_10,(uint8*)"d",SCI_GetTickCount());
				//close GPIO interrupt				
				temp   = s->display_buf_pos;
				lcd_id  = s->save_display_data_info[temp][INFO_LCD_ID_INDEX];
				left      = s->save_display_data_info[temp][INFO_LEFT_INDEX];
				top      = s->save_display_data_info[temp][INFO_TOP_INDEX];
				right    = s->save_display_data_info[temp][INFO_RIGHT_INDEX];
				bottom = s->save_display_data_info[temp][INFO_BOTTOM_INDEX];
				
				brush_mode = LCD_GetBrushMode(lcd_id);
				LCD_InvalidateRect(lcd_id, left, top, right, bottom);
				
				if(brush_mode == 	LCD_BRUSH_MODE_SYNC)
	  		       {	                	
	  		           blk_num = s->save_display_data_info[temp][INFO_BLOCK_ID_INDEX];
	  		           LCDC_ReleaseBlockCtrl(lcd_id,blk_num);
	  		           
	                	    //call callback of isp
	                	    s->save_display_data_info[temp][INFO_FLAG_INDEX]= 0;	                	    
	                	    s->callback(s->callback_param);
	                	    
	                	    _UpdateLcdCtrl_Done();
	  		       }	  		      
			}
			break;
		case UPDATELCDCTRL_UPDATE_DONE_SIG:
			{
				LCD_ID_E lcd_id = 0;
				uint32 blk_num = 0;
				lcd_id = s->save_display_data_info[s->display_buf_pos][INFO_LCD_ID_INDEX];
				blk_num = s->save_display_data_info[s->display_buf_pos][INFO_BLOCK_ID_INDEX];
				
				LCDC_ReleaseBlockCtrl(lcd_id,blk_num);
				s->save_display_data_info[s->display_buf_pos][INFO_FLAG_INDEX]=0;
				
				//SCI_TRACE_LOW:"UPDATELCDCTRL_UPDATE_DONE_SIG:callback_param is 0x%x,display_pos=%d,%d,%d"
				SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_430_112_2_18_1_43_34_11,(uint8*)"dddd",s->callback_param,s->display_buf_pos,lcd_id,blk_num);
				
				s->callback(s->callback_param);
				_UpdateLcdCtrl_Done();
			}
			break;
		default :
			//SCI_TRACE_LOW:"_UpdateLcd_MessageLoop command is error!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_437_112_2_18_1_43_34_12,(uint8*)"");
	   }
      }
}
/*****************************************************************************/
//  Description:    notice  invalidate lcd end
//  Author:         Jianping.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void UpdateLcdCtrl_NoticeLcdDone(uint32 param)
{     
     _UpdateLcd_SendSig(UPDATELCDCTRL_UPDATE_DONE_SIG, 0,0);    
}
/*****************************************************************************/
//  Description:    fmark callback
//  Author:         Jianping.Wang
//	Note:           
/*****************************************************************************/
PUBLIC void UpdateLcdCtrl_FmartTrigger(void)
{
	DECLARE_ENTRY(s);
	
	//SCI_TRACE_LOW:"FMARK Int: time = %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_455_112_2_18_1_43_34_13,(uint8*)"d",SCI_GetTickCount());
//	TM_SendTestPointRequest(2010,SCI_GetTickCount());
	if(s->is_use_fmark)
	{	
		s->is_use_fmark = FALSE;
     		_UpdateLcd_SendSig(UPDATELCDCTRL_FMARK_SIG, 0,0);
	}       
}
/*****************************************************************************/
//  Description:    create a thread and its resource,register the thread to system and active it.
//  Author:         Jianping.Wang
//	Note:           return 0 is success ,other is error
/*****************************************************************************/
PUBLIC int32 UpdateLcdCtrl_Init(uint32 thread_pori)
{
    DECLARE_ENTRY(s);
    uint32 status;	
    int32 ret = 0;

    //SCI_TRACE_LOW:"UpdateLcdCtrl_Init start!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_472_112_2_18_1_43_34_14,(uint8*)"");
    
    if(!s->thread_id)
    {    
	    s->thread_id = SCI_CreateAppThread(        
				       "T_FMARK_TASK",
                                    "T_FMARK_TASK_QUEUE",
                                    _UpdateLcdCtrl_MessageLoop,
                                    0,
                                    0,
                                    UPDATELCDCTRL_STACK_SIZE,
                                    UPDATELCDCTRL_QUEUE_NUM,
                                    thread_pori,
                                    SCI_PREEMPT,
                                    SCI_AUTO_START);

	    if (!s->thread_id )
	    {
	        ret = -1;
	    }	    
    }    
    
    s->state = IDLE ;
    s->display_buf_pos = 0;
    s->save_data_buf_pos = 0;
    s->buf_addr = 0;
    s->callback_param = 0;
    s->result = -1;
    s->is_use_fmark = FALSE;    
    s->valid_data_num = 0;

    GPIO_EnableIntCtl(TEST_GPIO_PIN); 

    // Create an isp event group     
   if (NULL == s->event)
   {
	s->event = SCI_CreateEvent("UPDATELCDCTRL_EVENT"); 
   }   
  SCI_MEMSET(&s->save_display_data_info[0],0,(SAVE_DATA_INFO_ARRAY_NUM*(SAVE_DATA_INFO_NUM+1))<<2);
   //SCI_TRACE_LOW:"UpdateLcdCtrl_Init end!"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,FMARK_CTRL_511_112_2_18_1_43_34_15,(uint8*)"");
   
   return ret;
}
/*****************************************************************************/
//  Description:    create a thread and its resource,register the thread to system and active it.
//  Author:         Jianping.Wang
//	Note:           return 0 is success ,other is error
/*****************************************************************************/
PUBLIC int32 UpdateLcdCtrl_Close(void)
{
	DECLARE_ENTRY(s);
	UPDATELCDCTRL_SIGNAL_T *sig_ptr = PNULL;
       
	if(s->thread_id)
	{
		SCI_SuspendThread(s->thread_id);
		LCD_UnRegisterCallBack();
		if(s->sig_ptr)
		{
			SCI_FREE(s->sig_ptr);
			s->sig_ptr = 0;
		}
		
		while (1)
		{
			sig_ptr = (void*)SCI_PeekSignal(s->thread_id);
			if (sig_ptr != PNULL)
			{
			    SCI_FREE(sig_ptr);
			    sig_ptr = PNULL;
			}
			else
			{
			    break;
			}
		}   
		SCI_ResumeThread(s->thread_id);
	}
	SCI_MEMSET(&s->save_display_data_info[0],0,(SAVE_DATA_INFO_ARRAY_NUM*(SAVE_DATA_INFO_NUM+1))<<2);
	GPIO_DisableIntCtl(TEST_GPIO_PIN);
	 s->state = IDLE ;
        s->display_buf_pos = 0;
        s->save_data_buf_pos = 0;
        s->buf_addr = 0;
        s->callback_param = 0;
        s->result = -1;
        s->is_use_fmark = FALSE;    
        s->valid_data_num = 0;

        return 0;
}

#ifdef   __cplusplus
}
#endif

