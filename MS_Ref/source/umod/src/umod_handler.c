/******************************************************************************
 ** File Name:    UMOD_handler.c                                                  *
 ** Author:       Jim.Lin                                                 *
 ** DATE:         2007-1-30                                                   *
 ** Copyright:    2007 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 2007-01-30     Jim.Lin     Create.                                     *
 ** 2007-02-06    Jim.Lin      modify the comment
 ** 2007-6-21     EllyZhou     modified func.UMOD_SET_LINE_CODIING for 8800H
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_umod_trc.h"
#include "UMOD_handler.h"
#include "UMOD_main.h"
#include "UMOD_common.h"
#include "USB200.h"
#include "com_drv.h"
#include "UMOD_drv.h"
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

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
extern PUBLIC void UMOD_ClassCommand (void);
extern PUBLIC void UMOD_SET_EP3_READY(void);
char EP3_switch=1;
//char EP1_switch=0;
LOCAL __align(4) uint32 s_umod_temp_buf[USB_FIFO_MAX_BYTE];

__align(4) uint8  interrupt_array[]=
{
   0XA1,0X20,0X00,0X00,0X00,0X00,0X02,0X00,0X02,0X00
};

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: UMOD int endp0 handler
//	dependence : none 
//  Author     : Jim.Lin
//	Note:        called by UMOD_usb_irq   
/*****************************************************************************/
LOCAL void UMOD_Ep0_handler (void)
{
	USB_EP0_REG_T * ep0_ctrl = (USB_EP0_REG_T *)USB_EP0_BASE ;
	USB_EPx_INT_U status ;
	
	
	status.dwValue = ep0_ctrl->int_sts.dwValue ;
	if (status.mBits.in_timeout){

	}
	if (status.mBits.setup_end){
        switch (ep0_ctrl->setup_low.mBits.type){
            case USB_REQ_STANDARD:
                switch (ep0_ctrl->setup_low.mBits.recipient){
                    case USB_REC_DEVICE:
                        UMOD_GetDevDescriptor (\
                                    (ep0_ctrl->setup_low.mBits.direction),\
                                    (ep0_ctrl->setup_low.mBits.value),\
                                    (ep0_ctrl->setup_high.mBits.index),\
                                    (ep0_ctrl->setup_high.mBits.length));
                        break;
                    case USB_REC_INTERFACE:
                        break;
                    default :
                        break;                
                }

                break;
            case USB_REQ_CLASS:
                
                switch (ep0_ctrl->setup_low.mBits.request){
                    case UMOD_CLASS_GET_MAX_LUN:
			
                        
                        break;
                    case UMOD_CLASS_RESET:
                        
			
                        break;
			case UMOD_CLASS_GET_LINE_CODING:
				UMOD_GET_LINE_CODIING();
				break;
			case UMOD_CLASS_SET_CTRL_LINE_ST:
				UMOD_ClassCommand ();
				if(EP3_switch==1)
				{
				   UMOD_SET_EP3_READY();
				   EP3_switch=0;
				}
				break;
			case UMOD_CLASS_SET_LINE_CODING:
				UMOD_SET_BUF_READY();
				break;
                    default :
                      //  USB_ErrorHandle(USB_ERR_INVALID_CLASSCMD);               
                        break;                
                }
                
                break;
            default :
                break;
        }
      
    }
	 if (status.mBits.transaction_end)
       {          
          UMOD_SET_LINE_CODIING();
       }
	ep0_ctrl->int_clr.dwValue =USB_INT_EPx_MASK;
}
/*****************************************************************************/
//  Description: UMOD int endp1 handler
//	dependence:  none
//  Author:      Jim.Lin
//	Note:        called by UMOD_usb_irq   
/*****************************************************************************/
LOCAL void UMOD_Ep1_handler(void)
{
	USB_EPx_REG_T* ep1_ctrl   = (USB_EPx_REG_T *)USB_EP1_BASE ;
    UMOD_DEVICE_T* dev_ptr    = NULL;
	USB_EPx_INT_U  status ;
    
    //cuurent in assert mode ,no interrupt occur
    if(UMOD_IsInAssert())
    {
        //clear the int directly
    	ep1_ctrl->int_clr.dwValue = USB_INT_EPx_MASK;
    	return;
    }

	status.dwValue = ep1_ctrl->int_sts.dwValue ;

    dev_ptr = UMOD_getDevfromEndp(USB_EP1);

    //int sts of data_not_ready, retreive some data from sio buffer
    //by means of callba1ck(WRITE COMPLETE) 
	if (status.mBits.in_timeout){
        //SCI_TRACE_LOW:"UMOD_handler.c ep1 intoken ack timeout"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UMOD_HANDLER_149_112_2_18_1_55_12_0,(uint8*)"");
        USB_ErrorHandle(USB_ERR_INACK_TIMEOUT);
	}

#if defined(_UMOD_SC6800_)||defined(_UMOD_SC8800H_)
    if(status.mBits.data_not_ready)
#else
    if(status.mBits.buf_not_ready)
#endif
    {
        dev_ptr->callback(EVENT_WRITE_COMPLETE);
    }
    
    ep1_ctrl->int_clr.dwValue = USB_INT_EPx_MASK;
}

/*****************************************************************************/
//  Description: UMOD int endp2 handler
//	dependence:  none
//  Author:      Jim.Lin
//	Note:        called by UMOD_usb_irq   
/*****************************************************************************/
LOCAL void UMOD_Ep2_handler  (void)
{
	USB_EPx_REG_T * ep2_ctrl = (USB_EPx_REG_T *)USB_EP2_BASE ;
	USB_EPx_INT_U status ;
    UMOD_DEVICE_T *dev_ptr = NULL;
    uint16 num;	
    
	status.dwValue = ep2_ctrl->int_sts.dwValue ;
	
    dev_ptr = UMOD_getDevfromEndp(USB_EP2);
    //judge endp has not been used yet
    //Jim  1-24
  
	if(dev_ptr->open_flag != UMOD_ALREADY_OPEN)
	{
	    //here, indacted that current endp is not using, endp index is should be 0xff
	    //only clear usb fifo and int sts
	    num = ep2_ctrl->receive_num.mBits.num;
        UMOD_EPxReceiveData(USB_EP2,s_umod_temp_buf,num);

    	ep2_ctrl->int_clr.dwValue = USB_INT_EPx_MASK;
    	
    	return ;
	}
	
	
	/*
	* below, received data is handled normaly
	*/
	if (status.mBits.in_timeout){
        //SCI_TRACE_LOW:"UMOD_handler.c ep2 intoken ack timeout"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UMOD_HANDLER_199_112_2_18_1_55_12_1,(uint8*)"");
        USB_ErrorHandle(USB_ERR_INACK_TIMEOUT);
	}

	if(status.mBits.transfer_end)
	{
	    //got the data from fifo
	    num = ep2_ctrl->receive_num.mBits.num;

        UMOD_EPxReceiveData(USB_EP2,(uint32 *)dev_ptr->rx_buf.usb_buf,num);
 
        //update the usb rx buf info
        //it's useful only when assert occured;
		dev_ptr->rx_buf.write = num;
        dev_ptr->rx_buf.read = 0;

        //only receive data when current comport is ready
        //Jim 1-24
       if((dev_ptr->state == UMOD_DEV_READY)&&(!UMOD_IsInAssert()))
        {
            dev_ptr->callback(EVENT_DATA_TO_READ);
		}
    }
    
	ep2_ctrl->int_clr.dwValue = USB_INT_EPx_MASK;
}

/*****************************************************************************/
//  Description: UMOD int endp3 handler
//	dependence:  none
//  Author:      Jim.Lin
//	Note:        called by UMOD_usb_irq   
/*****************************************************************************/
LOCAL void UMOD_Ep3_handler  (void)
{
	USB_EPx_REG_T * ctl = (USB_EPx_REG_T *)USB_EP3_BASE ;
	USB_EPx_INT_U status ;
	char *pbuf=0;
	 
	status.dwValue = ctl->int_sts.dwValue ;
	
	if (status.mBits.in_timeout){
	   // USB_ErrorHandle(USB_ERR_INACK_TIMEOUT);
		//ctl->int_clr.dwValue = USB_INT_EPx_MASK; 
		//return;
	}
	
	
	if (status.mBits.transaction_end)
	{
   		//Clear data ready ;
		USB_EPxDataInCtrl  (FALSE,USB_EP3);
		//DMA_ChannelEnable(DMA_USB_EP3,FALSE);
		
	}
	
	pbuf=(char *)&interrupt_array;

	if(EP3_switch==1)
	{
	UMOD_EPxSendData(USB_EP3,(uint32 *)pbuf,10);
       EP3_switch=0;
	}
	ctl->int_clr.dwValue = USB_INT_EPx_MASK; //Clear all interrupt ;
	return;
}

/*****************************************************************************/
//  Description: UMOD int endp4 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        called by UMOD_usb_irq   
/*****************************************************************************/
LOCAL void UMOD_Ep4_handler  (void)
{
	USB_EPx_REG_T * ep4_ctrl = (USB_EPx_REG_T *)USB_EP4_BASE ;
	USB_EPx_INT_U status ;
    UMOD_DEVICE_T *dev_ptr = NULL;
    uint16 num;	
    
	status.dwValue = ep4_ctrl->int_sts.dwValue ;
	
    dev_ptr = UMOD_getDevfromEndp(USB_EP4);
    //judge endp has not been used yet
	if(dev_ptr->open_flag != UMOD_ALREADY_OPEN)
	{
	    //here, indacted that current endp is not using, endp index is should be 0xff
	    //only clear usb fifo and int sts
	    num = ep4_ctrl->receive_num.mBits.num;
        UMOD_EPxReceiveData(USB_EP4,s_umod_temp_buf,num);

    	ep4_ctrl->int_clr.dwValue = USB_INT_EPx_MASK;
    	
    	return ;
	}
	
	/*
	* below, received data is handled normaly
	*/
	if (status.mBits.in_timeout){
       UMOD_ASSERT(0);
	}

	if(status.mBits.transfer_end)
	{
	    //got the data from fifo
	    num = ep4_ctrl->receive_num.mBits.num;
        UMOD_EPxReceiveData(USB_EP4,(uint32 *)dev_ptr->rx_buf.usb_buf,num);
 
        //update the usb rx buf info
        //it's useful only when assert occured;
		dev_ptr->rx_buf.write = num;
        dev_ptr->rx_buf.read = 0;

        //only receive data when current comport is ready
        if((dev_ptr->state == UMOD_DEV_READY)&&(!UMOD_IsInAssert()))
        {
            dev_ptr->callback(EVENT_DATA_TO_READ);
		}
    }
    
	ep4_ctrl->int_clr.dwValue = USB_INT_EPx_MASK;
}

/*****************************************************************************/
//  Description: usb global int handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        called in IRQ handler   
/*****************************************************************************/
PUBLIC void UMOD_usb_irq (uint32 param)
{
	USB_DEV_REG_T       *usb_ctrl_ptr = (USB_DEV_REG_T *)USB_REG_BASE;
	USB_DEV_INT_STS_U   status ;

	if( REG32(INT_IRQ_RAW_STS) & (1<<TB_USB_INT)){
		status.dwValue = usb_ctrl_ptr->int_sts.dwValue ;
		//dispatch handler message according to usb int status ;
		//dispatch EPx handler ;
		if (status.mBits.ep3 )
		{
			UMOD_Ep3_handler();
		}
		if (status.mBits.ep2 )
		{
			UMOD_Ep2_handler();
		}
		if (status.mBits.ep0 )
		{
			UMOD_Ep0_handler();
		}
		if (status.mBits.ep4 )
		{
			UMOD_Ep4_handler();
		}
		if (status.mBits.ep1 )
		{
			UMOD_Ep1_handler();
		}
		//clear usb device interrupt ;
		usb_ctrl_ptr->int_clr.dwValue = USB_INT_DEV_MASK ;
	}
}

/*****************************************************************************/
//  Description: got char from usb vcom
//	dependence:  none
//  Author:      weihua.wang
//	Note:        called in assert mode   
/*****************************************************************************/
PUBLIC int32 UMOD_GetChar(uint32 port_num)
{   
	USB_DEV_REG_T  *  ctl         = (USB_DEV_REG_T *)USB_REG_BASE;
    UMOD_DEVICE_T  *  dev_ptr     = NULL;
	USB_DEV_INT_STS_U status ;
	uint32            condition; 
	volatile int32    ch ;
    
    //get the dev buffer according to the endpnum
	dev_ptr = UMOD_GetDevfromPort(port_num);

	//if no any char, loop in here ;
	condition = FALSE ;
	ch        = -1; 
	do{
		if (dev_ptr->rx_buf.read != dev_ptr->rx_buf.write)
		{
        	ch = dev_ptr->rx_buf.usb_buf[dev_ptr->rx_buf.read];
			dev_ptr->rx_buf.read++;
			if (dev_ptr->rx_buf.read >= USB_RECV_LIMIT){
				dev_ptr->rx_buf.read = 0;
			}
				
			return ch ;
		}

		if( REG32(INT_IRQ_RAW_STS) & (1<<TB_USB_INT)){
			status.dwValue = ctl->int_sts.dwValue ;
			//dispatch handler message according to usb int status ;

			if (status.dwValue  & USB_INT_EPx_MASK )
			{
				//dispatch EPx handler ;
				if (status.mBits.ep2 )
				{
					UMOD_Ep2_handler();
				}
				if (status.mBits.ep4 )
				{
					UMOD_Ep4_handler();
				}	
				if(status.mBits.ep0)
				{
					UMOD_Ep0_handler();
				}
				if(status.mBits.ep1)
				{
				    UMOD_Ep1_handler();
				}
				if(status.mBits.ep3)
				{
    			    UMOD_Ep3_handler();
				}
			}
			//clear usb interrupt ;
			ctl->int_clr.dwValue = USB_INT_DEV_MASK ;
		}

	}while(condition);
	return ch;
}

/*****************************************************************************/
//  Description: UMOD dma ep0 in handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_epin (uint32 param)
{


}
/*****************************************************************************/
//  Description: UMOD dma ep0 out handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_epout (uint32 param)
{

}
/*****************************************************************************/
//  Description: UMOD dma ep1 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_ep1 (uint32 param)
{

}

/*****************************************************************************/
//  Description: UMOD dma ep2 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_ep2 (uint32 param)
{

}

/*****************************************************************************/
//  Description: UMOD dma ep3 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_ep3 (uint32 param)
{

}

/*****************************************************************************/
//  Description: UMOD dma ep4 handler
//	dependence:  none
//  Author:      weihua.wang
//	Note:        not used yet   
/*****************************************************************************/
PUBLIC void UMOD_dma_ep4 (uint32 param)
{

}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
