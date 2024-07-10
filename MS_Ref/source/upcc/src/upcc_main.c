/******************************************************************************
 ** File Name:    UPCC_main.c                                                 *
 ** Author:       yuhua.shi                                                   *
 ** DATE:         3/22/2006                                                   *
 ** Copyright:    2006 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:  This file is USB video protocol main body for PC Camera.    *
 ******************************************************************************/
 /*****************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/22/2006     yuhua.shi       Create.                                     *
 ******************************************************************************/
#ifndef _UPCC_MAIN_C_
#define _UPCC_MAIN_C_
    
/*----------------------------------------------------------------------------*
 **                         DEPENDENCIES                                      *
 **-------------------------------------------------------------------------- */    
#include "ms_ref_upcc_trc.h"
#include "gadget.h"
#include "upcc_main.h"
#include "upcc_drv.h"
#include "upcc_api.h"
#include "upcc_request.h"
#include "upcc_camera.h"
#include "sc_reg.h"
#include "udc_api.h"
#include "usb_device.h"
#include "priority_system.h"
#ifdef USB_PROTOCOL_SUPPORT_USB20
#include "usb20_app_config.h"
#endif
/**---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                  *
 **---------------------------------------------------------------------------*/

#define  UPCC_STACK_SIZE     		   0x1000
#define  UPCC_QUEUE_NUM        	       0x20


/**---------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                  *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                 *
 **---------------------------------------------------------------------------*/

LOCAL USB_CTRLREQUEST_T *upccctrlreq;
uint32 upcc_interface=0xFFFFFFFF;
/*----------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                  *
 **---------------------------------------------------------------------------*/
extern uint8 *g_UPCC_jpeg_malloc_buf ;
extern uint8 *g_UPCC_jpeg_buf ;
extern SCI_SEMAPHORE_PTR g_UPCC_capture_semaphore;
extern SCI_SEMAPHORE_PTR g_UPCC_jpeg_buf_semaphore;
extern SCI_SEMAPHORE_PTR g_UPCC_setcur_semaphore;
extern BOOLEAN g_UPCC_is_video_streaming_allowed_transmit;

/**---------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                 *
 **---------------------------------------------------------------------------*/
static void UPCC_task_exit(void);
LOCAL uint8  *s_data_buffer=NULL;  
__align(4) UPCC_MANAGER_T g_UPCC_manager;
BLOCK_ID s_upcc_task_id = 0;//this task used to collect garbage generated in write operation.
BLOCK_ID upcc_main_id = 0;//this task used to collect garbage generated in write operation.
uint32 g_upcc_setcur_flag = FALSE;
uint32 g_upcc_test_flag = 0;

/*----------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                            *
 **---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 **                         LOCAL VARIABLE DECLARE                            *
 **---------------------------------------------------------------------------*/
LOCAL uint32 s_ep0_out_len = 0;//store the data length of endpoint 0

/**---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                               *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:				UPCC driver's thread
//	Global resource dependence: 
//  Author:						yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void UPCC_Drv_ThreadEntry(uint32 argc, void* argv)
{
	UPCC_SIGNAL_T *	sig_ptr;
	BLOCK_ID	tid = SCI_IdentifyThread();

	for(;;)
	{
		sig_ptr = (UPCC_SIGNAL_T*)SCI_GetSignal(tid);

		switch(sig_ptr->request)
		{
			case SET_CUR:
				g_upcc_setcur_flag = TRUE;
				UPCC_PU_SetCur(sig_ptr->command, sig_ptr->param);				// data is in, run command
                g_upcc_setcur_flag = FALSE;
				break;
			case GET_CUR:
				UPCC_PU_GetCur(sig_ptr->command, sig_ptr->param);
				break;
			case GET_MIN:
				UPCC_PU_GetMin(sig_ptr->command, sig_ptr->param);
				break;
			case GET_MAX:
				UPCC_PU_GetMax(sig_ptr->command, sig_ptr->param);
				break;
			case GET_RES:
				UPCC_PU_GetRes(sig_ptr->command, sig_ptr->param);
				break;
			case GET_LEN:
				UPCC_PU_GetLen(sig_ptr->command, sig_ptr->param);
				break;
			case GET_INF:
				UPCC_PU_GetInf(sig_ptr->command, sig_ptr->param);
				break;
			case GET_DEF:
				UPCC_PU_GetDef(sig_ptr->command, sig_ptr->param);
				break;
			default:
				break;
		}
		SCI_DisableIRQ();
		SCI_FREE(sig_ptr);
		SCI_RestoreIRQ();
	}
}

/*****************************************************************************/
//  Description: init the usb pc camera   
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_init (void)
{	
      UDC_PowerSet(FALSE);//BUS power;
      g_UPCC_is_video_streaming_allowed_transmit=FALSE;
      g_UPCC_manager.cache.malloc_buf = SCI_ALLOC_APP (UPCC_CACHE_SIZE + UPCC_DATA_ALIGN);
      //    g_UPCC_manager.cache.align_buf = (uint8 *)(((((uint32)g_UPCC_manager.cache.malloc_buf+3)>>2)<<2) + 32); 
      g_UPCC_manager.cache.align_buf = (uint8 *)(((((uint32)g_UPCC_manager.cache.malloc_buf+UPCC_DATA_ALIGN-1)/UPCC_DATA_ALIGN)*UPCC_DATA_ALIGN)); 
      
      g_UPCC_jpeg_malloc_buf = SCI_ALLOC_APP (UPCC_JPEG_BUF_MAX_SIZE + 64);
      g_UPCC_jpeg_buf = (uint8 *)(((((uint32)g_UPCC_jpeg_malloc_buf+3)>>2)<<2) + 32); 
      
      g_UPCC_manager.cache.cur_index = 0;
       
       UPCC_ResetCache();
       
       //init dc setting
       UPCC_SettingInit();
       
       //init the request delay timer when open the pc camera
       UPCC_InitRequestDelayTimer();
       
       UPCC_InitFeaValArray();
       
       g_UPCC_capture_semaphore = SCI_CreateSemaphore("UPCC capture semaphore", 0);
       
       g_UPCC_jpeg_buf_semaphore = SCI_CreateSemaphore("UPCC jpeg buf semaphore", 1);
       
       g_UPCC_setcur_semaphore = SCI_CreateSemaphore("UPCC set cur command semaphore", 1);
       
       
       s_upcc_task_id  = SCI_CreateAppThread("T_UPCC_DRV_THREAD",
       						"UPCC",
       						UPCC_Drv_ThreadEntry,
       						0,
       						0,
       						UPCC_STACK_SIZE,
       						UPCC_QUEUE_NUM,
       						PRI_UPCC_DRV_TASK,
       						SCI_PREEMPT,
       						SCI_AUTO_START);
       if(SCI_INVALID_BLOCK_ID == s_upcc_task_id)
       {
       	SCI_ASSERT(0); /*assert verified*/
       }
       
       return;
}

/*****************************************************************************/
//  Description: exit the pc camera   
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/

PUBLIC void UPCC_exit (void)
{
      //deinit the request delay timer when close the pc camera
      UPCC_DeInitRequestDelayTimer();
      g_UPCC_is_video_streaming_allowed_transmit=FALSE;
      SCI_FREE ((char *)g_UPCC_manager.cache.malloc_buf); /*lint !e63*/
      g_UPCC_manager.cache.buf = NULL; 
      
      if(PNULL != g_UPCC_jpeg_malloc_buf)
      {
           SCI_FREE(g_UPCC_jpeg_malloc_buf);
           g_UPCC_jpeg_buf = PNULL;
      }
      if(NULL != g_UPCC_capture_semaphore )
      {
           SCI_DeleteSemaphore(g_UPCC_capture_semaphore);
           g_UPCC_capture_semaphore = NULL;
      }
      if(NULL != g_UPCC_jpeg_buf_semaphore )
      {
           SCI_DeleteSemaphore(g_UPCC_jpeg_buf_semaphore);
           g_UPCC_jpeg_buf_semaphore = NULL;
      }
      if(NULL != g_UPCC_setcur_semaphore )
      {
           SCI_DeleteSemaphore(g_UPCC_setcur_semaphore);
           g_UPCC_setcur_semaphore = NULL;
      }
      SCI_DeleteThread(s_upcc_task_id);
}

/*****************************************************************************/
//  Description:    This is UPCC's main body .
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC void UPCC_main (uint32 argc, void *argv)
{
    
	xSignalHeader	signal;
	BLOCK_ID    tid = SCI_IdentifyThread();

	BOOLEAN is_upcc_main_task_exist = TRUE;

	while(is_upcc_main_task_exist)
	{
		signal = (xSignalHeader)SCI_GetSignal(tid);
		switch (signal->SignalCode)
		{
		     case UPCC_CMD_PLUG_IN:
                          UPCC_init();
                          g_upcc_test_flag = 0;
                          s_data_buffer = SCI_ALLOC_APP(32);
                          upccctrlreq = SCI_ALLOC_APP(sizeof(USB_CTRLREQUEST_T));
                          if(!upccctrlreq)
                          {
                                 //SCI_TRACE_LOW:"\ncom_bind() alloc ctrl req fail\n"
                                 SCI_TRACE_ID(TRACE_TOOL_CONVERT,UPCC_MAIN_241_112_2_18_1_55_22_42,(uint8*)"");
                           }
	            break;
		     case UPCC_CMD_PLUG_OUT:
                           UPCC_task_exit();
                           is_upcc_main_task_exist = FALSE;
		     break;
		     case UPCC_CMD_EXECUTE:
			     if (TRUE == UDC_GetConnect())
			     {
				     UPCC_HandleRequest(); 
			     }
		     break;
		     case UPCC_CMD_EXCEPTION:
			      UPCC_GetAndSendFrame();
		     break;
		     default :
		     break;	
		}
		SCI_FREE(signal);
	}
    // Empty the signal queue.
    while (1)    /*lint !e716*/
    {
        signal = (void*)SCI_PeekSignal(SCI_IdentifyThread());
        if (signal)
        {
            SCI_FREE(signal);
        }
        else
        {
            break;
        }
    }

     SCI_ThreadExit ();

}

/*****************************************************************************/
//  Description:				Function to set ep0 data out length
//	Global resource dependence: 
//  Author:						yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void SetEp0OutDataLen(uint32 len)
{
    s_ep0_out_len = len;
}

/*****************************************************************************/
//  Description:				Function to get ep0 data out length
//	Global resource dependence: 
//  Author:						yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL uint32 GetEp0OutDataLen(void)
{
    return s_ep0_out_len;
}
/*****************************************************************************/
//  Description:					Function is called when ep0 activity finished
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL void UPCC_CTRL_Complete(uint8 *buffer, uint32 length)
{
	xSignalHeader   signal;
	uint16 val = 0;

	if(0 != GetEp0OutDataLen())
	{//for out
		SetEp0OutDataLen(0);
		if((upccctrlreq->wValue==0x200) || (upccctrlreq->wValue==0x400))
		{
				signal = SCI_ALLOC_APP(sizeof(*signal));
				signal->SignalCode = UPCC_CMD_EXECUTE;
				signal->SignalSize = sizeof(*signal);
				signal->Sender = SCI_IdentifyThread();
				SCI_SendSignal(signal, upcc_main_id);
		}
		if((upccctrlreq->bRequest == 0x01)&& (upccctrlreq->wIndex==0x0300))			//For Set_Cur 
		{
			if(	((upccctrlreq->wValue==0x0500) && (upccctrlreq->wLength==0x01)) ||		//PowerLine
				((upccctrlreq->wValue==0x0B00) && (upccctrlreq->wLength==0x01)) ||		//WB temp auto
				((upccctrlreq->wValue==0x0D00) && (upccctrlreq->wLength==0x01)) ||		//WB com auto
				((upccctrlreq->wValue==0x1000) && (upccctrlreq->wLength==0x01)) ||		//Hue auto
				((upccctrlreq->wValue==0x1100) && (upccctrlreq->wLength==0x01)) ||		//Analog video
				((upccctrlreq->wValue==0x1200) && (upccctrlreq->wLength==0x01)) ) 		//Analog lock
			{
				val = buffer[0];
			}
			if( ((upccctrlreq->wValue==0x0100) && (upccctrlreq->wLength==0x02)) ||		//Backlight
				((upccctrlreq->wValue==0x0200) && (upccctrlreq->wLength==0x02)) ||		//Brightness
				((upccctrlreq->wValue==0x0300) && (upccctrlreq->wLength==0x02)) ||		//Contrast
				((upccctrlreq->wValue==0x0400) && (upccctrlreq->wLength==0x02)) ||		//Gain
				((upccctrlreq->wValue==0x0600) && (upccctrlreq->wLength==0x02)) ||		//HUE
				((upccctrlreq->wValue==0x0700) && (upccctrlreq->wLength==0x02)) ||		//Saturation
				((upccctrlreq->wValue==0x0800) && (upccctrlreq->wLength==0x02)) ||		//Sharp
				((upccctrlreq->wValue==0x0900) && (upccctrlreq->wLength==0x02)) ||		//Gamma
				((upccctrlreq->wValue==0x0A00) && (upccctrlreq->wLength==0x02)) ||		//WB temp
				((upccctrlreq->wValue==0x0E00) && (upccctrlreq->wLength==0x02)) ||		//Digital
				((upccctrlreq->wValue==0x0F00) && (upccctrlreq->wLength==0x02)) )		//Digital Lim
			{
				val = buffer[1];
				val = val<<8;
				val = val | buffer[0];
			}
			if((upccctrlreq->wValue==0x0C00) && (upccctrlreq->wLength==0x04))		//WB com
			{
				
			}
			UPCC_Fea_Control(upccctrlreq->bRequest,((upccctrlreq->wValue) >> 8),val);
		}
	}
	else
	{
	
	}
}

/*****************************************************************************/
//  Description:					Function is called when ep1 activity finished
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/

LOCAL void UPCC_Send_Complete(uint8 *buffer, uint32 length)
{
	UPCC_SetCacheLockFlag(FALSE);
}
LOCAL void UPCC_Transfer_Complete(uint8 event,uint8 *buffer,uint32 length)
{
	switch(event&0x0F)
	{
		case EVENT_CTRL:
			UPCC_CTRL_Complete(buffer,length);
			break;
		case EVENT_SEND:
			UPCC_Send_Complete(buffer,length);
			break;
		default:
			break;
	}
}
/*****************************************************************************/
//  Description:					Function bind upcc driver to udc driver
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/

static int UPCC_bind(USB_GADGET_T *gadget)
{
	xSignalHeader  signal;
	BOOLEAN        ret = FALSE;
       uint32 call_back = (uint32 )UPCC_Transfer_Complete;

       upcc_interface = UsbDevice_OpenInterface(USB_PROTOCOL_UPCC,call_back,0x03);
	upcc_main_id  = SCI_CreateAppThread("T_UPCC_MAIN_THREAD",
							"UPCC",
							UPCC_main,
							0,
							0,
							UPCC_STACK_SIZE,
							UPCC_QUEUE_NUM,
							PRI_UPCC_MAIN_TASK,
							SCI_PREEMPT,
							SCI_AUTO_START);
	if(SCI_INVALID_BLOCK_ID == upcc_main_id)
	{
		SCI_ASSERT(0); /*assert verified*/
	}

	signal = SCI_ALLOC_APP(sizeof(*signal));
	signal->SignalCode = UPCC_CMD_PLUG_IN;
	signal->SignalSize = sizeof(*signal);
	signal->Sender = SCI_IdentifyThread();
	SCI_SendSignal(signal, upcc_main_id);
#ifdef USB_PROTOCOL_SUPPORT_USB20	
	USB_SetService(USB_APP_UPCC,NULL);
#endif
	return ret;
}
static void UPCC_task_exit(void)
{
    UPCC_ClosePCCamera();
    UDC_PowerSet(TRUE);
    UPCC_exit();
    SCI_FREE(s_data_buffer);
    s_data_buffer = NULL;

    if (upccctrlreq)
    {
        SCI_FREE (upccctrlreq);
	 upccctrlreq = NULL;
    }
    UsbDevice_Delete(NULL);
    upcc_interface = 0xffffffff;
    #ifdef USB_PROTOCOL_SUPPORT_USB20
	USB_SetService(USB_APP_NONE,NULL);
    #endif
}
/*****************************************************************************/
//  Description:					Function unbind upcc driver from udc driver
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
static void UPCC_unbind(USB_GADGET_T *gadget)
{
    xSignalHeader signal;
	  
    signal = SCI_ALLOC_APP (sizeof (*signal));
    signal->SignalCode = UPCC_CMD_PLUG_OUT;
    signal->SignalSize = sizeof (*signal);
    signal->Sender = SCI_IdentifyThread();
    SCI_SendSignal (signal, upcc_main_id);
		 
}

/*****************************************************************************/
//  Description:					Function handle usb setup request
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
static int UPCC_setup(USB_GADGET_T *gadget,const USB_CTRLREQUEST_T *ctrl)
{
	uint32 ret = TRUE;
	
	switch(ctrl->bRequestType & USB_TYPE_MASK)
	{
		case USB_TYPE_STANDARD:
			switch(ctrl->bRequestType & USB_RECIP_MASK)
			{
				case USB_RECIP_DEVICE:
				{
					switch(ctrl->bRequest)
					{
						case USB_REQ_GET_DESCRIPTOR:
					                 UPCC_GetDevDescriptor (
										(ctrl->bRequestType & USB_DIR_IN),\
										(ctrl->wValue),\
										(ctrl->wIndex),\
										(ctrl->wLength));
						break;
						case USB_REQ_SET_CONFIGURATION:
		    					if(g_upcc_test_flag==0)
		    					{
		    						UsbDevice_Start_send(upcc_interface,NULL,0);
								g_upcc_test_flag = 1;
		    					}							
						break;
						default:
						break;
					}
				}
				break;
				default :
					break;                
			}
			break;
		case USB_TYPE_CLASS:
			if((ctrl->bRequestType & USB_RECIP_MASK) == USB_RECIP_INTERFACE)
			{
				switch (ctrl->wIndex)
				{
					case 0x0000: //vc interface
						break;
					case 0x0200: //output interface
						break;
					case 0x0300://Processing interface
						switch(ctrl->bRequest)
						{
							case 0x81://GET_CUR
							case 0x82://GET_MIN
							case 0x83://GET_MAX
							case 0x84://GET_RES
							case 0x85://GET_LEN
							case 0x86://GET_INFO
							case 0x87://GET_DEF
								UPCC_Fea_Control(ctrl->bRequest, (ctrl->wValue >> 8), (ctrl->wLength & 0xFF));
								break;
							case 0x01://SET_CUR
								memcpy(upccctrlreq, ctrl, sizeof(USB_CTRLREQUEST_T));
								SetEp0OutDataLen(ctrl->wLength);

								UPCC_EPxSendData (USB2011_EP0_IN,s_data_buffer,ctrl->wLength);
								break;
							default:
								break;
						}
						break;
					case 0x0100://CT 
						switch(ctrl->bRequest)
						{
							case 0x81://GET_CUR
								break;
							case 0x82://GET_MIN
								break;
							case 0x83://GET_MAX
								break;
							case 0x84://GET_RES
								break;
							case 0x85://GET_LEN
								break;
							case 0x86://GET_INFO
								break;
							case 0x87://GET_DEF
								break;
							case 0x01://SET_CUR
								break;
							default :
								break;                
						}
						break;
					case 0x0001://Streaming Interface
						switch(ctrl->bRequest)
						{
							case 0x81://GET_CUR
							case 0x82://GET_MIN
							case 0x83://GET_MAX
								UPCC_StreamingInterface_GetCur(ctrl->wValue,ctrl->wLength);
								break;
							case 0x84://GET_RES
								break;
							case 0x85://GET_LEN
								break;
							case 0x86://GET_INFO
								break;
							case 0x87://GET_DEF
								break;
							case 0x01://SET_CUR
								UPCC_StreamingInterface_SetCur(ctrl->wValue,ctrl->wLength);
								if((USB_DIR_OUT==(ctrl->bRequestType&USB_DIR_IN)) && (ctrl->wLength > 0))
								{
									memcpy(upccctrlreq, ctrl, sizeof(USB_CTRLREQUEST_T));
									SetEp0OutDataLen(ctrl->wLength);            	            
									UPCC_EPxSendData (USB2011_EP0_IN,s_data_buffer,ctrl->wLength);
								}
								break;
							default :
								break;                
						}
						break;
				default :
					break;                
				}
			}
			break;
		case  USB_TYPE_RESERVED:
			if(ctrl->bRequest == 1)
			{
				g_UPCC_is_video_streaming_allowed_transmit=TRUE;
				g_upcc_test_flag = 2;
			}
			break;
		default :
			break;
	}
	return ret;
}

/*****************************************************************************/
//  Description:					Function handle device disconnection
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
static void UPCC_disconnect(USB_GADGET_T *gadget)
{
}

/*****************************************************************************/
//  Description:					Function handle device suspend
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
static void UPCC_suspend(USB_GADGET_T *gadget)
{
}

/*****************************************************************************/
//  Description:					Function handle device resume
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
static void UPCC_resume(USB_GADGET_T *gadget)
{
}

/*****************************************************************************/
//  Description:					Function to get upcc gadget interface
//	Global resource dependence: 
//  Author:         yuhua.shi
//	Note:           
/*****************************************************************************/
LOCAL const USB_GADGET_DRIVER_T upcc_driver = {
    "UPCC",
#ifdef CONFIG_USB_GADGET_DUALSPEED
    USB_SPEED_HIGH,
#else
    USB_SPEED_FULL,
#endif    
    UPCC_bind,
    UPCC_unbind,
    UPCC_setup,
    UPCC_disconnect,
    UPCC_suspend,
    UPCC_resume,
    UPCC_GetCOnfigDescriptor
};


const USB_GADGET_DRIVER_T * UPCC_GetHandler(void)
{

	return &upcc_driver;
}

/**---------------------------------------------------------------------------*/
#endif //_UPCC_MAIN_C_
