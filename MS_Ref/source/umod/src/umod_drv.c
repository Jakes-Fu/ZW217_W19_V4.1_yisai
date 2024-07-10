/******************************************************************************
 ** File Name:    UMOD_drv.c                                                  *
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
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "USB200.h"
#include "UPM_api.h"
#include "UMOD_common.h"
#include "UMOD_main.h"
#include "UMOD_drv.h"
#include "USB_drv.h"
#include "UMOD_common.h"

#define  NULL      0 

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
extern uint8  interrupt_array[];
extern char EP3_switch;

/*--------------------------- External Data ---------------------------------*/
extern const uint8 g_UMOD_DeviceDescr[];
extern const uint8 g_UMOD_ConfigDescr[];
extern const uint8 g_UMOD_LanguageStr[];
extern const uint8 g_UMOD_SerialNumStr[];
extern const uint8 g_UMOD_StrManufacturer[];
extern const uint8 g_UMOD_StrProduct[];
__align(4) uint8 LINE_CODING[]=
{
  0x00,0xc2,0x01,0x00,0x00,0x00,0x08
};
__align(4) uint8  SET_LINE_CODING[40]=
{
   NULL,
};


/*****************************************************************************/
//  Description:    Call this function to send data to special end point.
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           pBuf  must be aligned  4  ;
//                  len   must be less than 64 ;
//                  ep_id must be less than 6  ;
/*****************************************************************************/
PUBLIC uint32 UMOD_EPxSendData  (uint8 ep_id ,uint32 * pBuf,uint8 len)
{
	USB_EPx_REG_T * ctrl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;	
	USB_EP0_REG_T * ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
	USB_DEV_REG_T * ctrl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
	USB_EPx_REG_T * ctrl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
	int size = ((len + 3 )>>2) & 0x3FF ;

	switch (ep_id){  
    	case USB_EP1:
			ctrl_ep1->ctrl.mBits.pack_size   	    = len ;
			ctrl_ep1->transfer_size.mBits.num   	= len ;
			//Clear fifo 
            ctrl_usb->ep_rest.mBits.ep1_fifo_rst = TRUE;

        	USB_TxDWordToFIFO (pBuf ,size,USB_EP1);
    		USB_EPxDataInCtrl  (TRUE,USB_EP1);
    		break;
    	case USB_EP0_IN:
			ctrl_ep0->ctrl.mBits.pack_size	   	    = len ;
			ctrl_ep0->transfer_size_in.mBits.num	= len ;
			//Clear fifo 
            ctrl_usb->ep_rest.mBits.ep0_in_fifo_rst = TRUE;

           	USB_TxDWordToFIFO (pBuf ,size,USB_EP0_IN);
    		USB_EPxDataInCtrl  (TRUE,USB_EP0_IN);
    		break;
	case USB_EP3:
		ctrl_ep3->ctrl.mBits.pack_size   	    = len ;
			ctrl_ep3->transfer_size.mBits.num   	= len ;
			//Clear fifo 
            ctrl_usb->ep_rest.mBits.ep3_fifo_rst = TRUE;

        	USB_TxDWordToFIFO (pBuf ,size,USB_EP3);
    		USB_EPxDataInCtrl  (TRUE,USB_EP3);

		break;
		
    	default:
    	    break ;
	}

    return len ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jim.Lin
//	Note:           
/*****************************************************************************/
PUBLIC uint32 UMOD_EPxReceiveData(uint8 ep_id ,uint32 * pBuf,uint16 len)
{
	int32 size = NULL ;
	uint32 num   ;

    UMOD_ASSERT((ep_id == USB_EP0_OUT)||(ep_id == USB_EP2)||(ep_id == USB_EP4));
    
    if((ep_id == USB_EP0_OUT)||(ep_id == USB_EP2)||(ep_id == USB_EP4))
    {
        if (len){
        	size = ((len + 3 )>>2) & 0xFF ;
        	USB_RxDWordFromFIFO ((uint32 *)pBuf ,size,ep_id);
        }
    }

    num = len;
	switch (ep_id){
    	case USB_EP2:
    	{
    		USB_EPxDataOutCtrl  (TRUE,USB_EP2);
    		break;
    	}
    	case USB_EP4:
    	{
    		USB_EPxDataOutCtrl  (TRUE,USB_EP4);
    		break;
    	}
    	case USB_EP0_OUT:
    	{
    		USB_EPxDataOutCtrl  (TRUE,USB_EP0_OUT);
    		break;
    	}
    	default:
    	    num = NULL ;
    	    break ;	
	}
	return num ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
#if 0
PUBLIC uint32 UMOD_EPxReceiveData1  (uint8 ep_id ,uint32 * pBuf)
{
	USB_EPx_REG_T * ctrl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
	USB_EP0_REG_T * ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
	int size = NULL ;
	int num  = NULL ;

	switch (ep_id){
    	case USB_EP2:
   			num = ctrl_ep2->receive_num.mBits.num;
            if (num){
            	size = ((num + 3 )>>2) & 0xFF ;
            	USB_RxDWordFromFIFO ((uint32 *)pBuf ,size,ep_id);
            }
    		break;
    	case USB_EP0_OUT:
   			num = ctrl_ep0->receive_num.mBits.num;
        	if (num){
        		size = ((num + 3 )>>2) & 0xFF ;
        		USB_RxDWordFromFIFO ((uint32 *)pBuf ,size,ep_id);
        	}
    		break;
    	default:
    	    num = NULL ;
    	    break ;	
	}
	return num ;
}
#endif

/*****************************************************************************/
//  Description:    set usb device state
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:            
/*****************************************************************************/
LOCAL void Umod_SetDevState(UMOD_DEVICE_STATE_E state)
{
    int32 i;
    
    UMOD_ASSERT(state < UMOD_DEV_MAX);
    
    for(i = 0; i < UMOD_MAX_COM_NUM; i++)
    {
	    g_UMOD_manager.dev[i].state = state;
    }
}




/*****************************************************************************/
//  Description:    handle class specified command
//	Global resource dependence: 
//Argument:      None
//  Author:         Jim.Lin
//	Note:           
/*****************************************************************************/

PUBLIC void UMOD_ClassCommand (void)
{
	USB_EP0_REG_T * ep0_ctrl = (USB_EP0_REG_T *)USB_EP0_BASE ;
    
	
	
	//Data:21 22 00 00,it's indicated that the virtual com is close
	//Data:21 22 01 xx,it's indicated that the virtual com is open
	if(ep0_ctrl->setup_low.mBits.request == 0x22)
	{ 
		if((ep0_ctrl->setup_low.mBits.value&0xff) != 0) //21 22 01 xx
		{
		    Umod_SetDevState(UMOD_DEV_READY);
		}
		else         //21 22 00 00
		{
		    Umod_SetDevState(UMOD_DEV_PRESENT);
		}
	}
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Argument:     None
//  Author:         Jim.Lin
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_DoNothing (uint8 bDirection,uint16 wValue,uint16 wIndex ,uint16 wLen)
{
	//do nothing ;
}


/*****************************************************************************/
//  Description:    usb endp0 register init
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_EP0Config  (void)
{
    USB_EP0_REG_T * ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
	//config DMA's USB channel ;
	//Set max packet size is 8 Byte ;
	ctl_ep0->ctrl.mBits.pack_size   = 0x8 ;

	ctl_ep0->int_clr.dwValue = 0x3fff;
	ctl_ep0->int_ctrl.mBits.setup_end 		= 1 ;
	ctl_ep0->int_ctrl.mBits.in_timeout		= 1 ;
	ctl_ep0->int_ctrl.mBits.transaction_end	= 1 ;


	//Set buffer ready ;	
	USB_EPxDataOutCtrl  (TRUE,USB_EP0_OUT);
	return TRUE ;
}
/*****************************************************************************/
//  Description:    usb endp1 register init
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_EP1Config  (void)
{
	USB_EPx_REG_T * ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;

	ctl_ep1->ctrl.mBits.ep_en   			= 1 ;	//Enable ep1

    //Set endpoint type is bulk;
    ctl_ep1->ctrl.mBits.type                = 0;

	//Set max packet size is 64 Byte ;
	ctl_ep1->ctrl.mBits.pack_size   		= 0x40;
	ctl_ep1->transfer_size.mBits.num    	= 0x40;
	
	ctl_ep1->int_clr.dwValue	            = 0x3fff ;  //Clear
	ctl_ep1->int_ctrl.mBits.in_timeout		= 1 ;
	ctl_ep1->int_ctrl.mBits.transfer_end	= 1 ;

	USB_EPxDataInCtrl  (FALSE,USB_EP1);	 //Set data ready ;
	
	return TRUE ;
}
/*****************************************************************************/
//  Description:    usb endp2 register init
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_EP2Config  (void)
{
	USB_EPx_REG_T * ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;

	ctl_ep2->ctrl.mBits.ep_en   			= 1 ;	//Enable ep2

    //Set endpoint type is bulk;
    ctl_ep2->ctrl.mBits.type                = 0 ;

	//Set max packet size is 64 Byte ;
	ctl_ep2->ctrl.mBits.pack_size   		= 0x40;
	ctl_ep2->transfer_size.mBits.num    	= 0x40;
	
	ctl_ep2->int_clr.dwValue	            = 0x3fff ;  //Clear
	ctl_ep2->int_ctrl.mBits.transfer_end    = 1 ;	    //Enable buf not ready interrupt

	USB_EPxDataOutCtrl  (TRUE,USB_EP2);	 //Set data ready ;
	
	return TRUE ;
}

/*****************************************************************************/
//  Description:    usb endp3 register init
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_EP3Config  (void)
{
      //char *pbuf=0;
	USB_EPx_REG_T * ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;

	ctl_ep3->ctrl.mBits.ep_en   			= 1 ;	//Enable ep3

    //Set endpoint type is bulk;
    ctl_ep3->ctrl.mBits.type                = 0 ;

	//Set max packet size is 64 Byte ;
	ctl_ep3->ctrl.mBits.pack_size   		= 0x40;
	ctl_ep3->transfer_size.mBits.num    	= 0x40;
	
	ctl_ep3->int_clr.dwValue	            = 0x3fff ;  //Clear
	ctl_ep3->int_ctrl.mBits.in_timeout		= 1 ;
	ctl_ep3->int_ctrl.mBits.transfer_end	= 1 ;
	
	EP3_switch=1;

	USB_EPxDataInCtrl  (FALSE,USB_EP3);	 //Set data ready ;
	//pbuf=(char *)&interrupt_array,
	//UMOD_EPxSendData(USB_EP3,(uint32 *)pbuf,10);
	
	return TRUE ;
 
}



/*****************************************************************************/
//  Description:    Call this function to send data to special end point.
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           pBuf  must be aligned  4  ;
//                  len   must be less than 64 ;
//                  ep_id must be less than 6  ;
/*****************************************************************************/
LOCAL uint32 UMOD_EPxSendDataInLoop(uint8 ep_id ,uint32 * pBuf,uint8 len)
{
	USB_DEV_REG_T * ctrl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
	int size = ((len + 3 )>>2) & 0x3FF ;

	switch (ep_id){  
    	case USB_EP3:
    	{
        	USB_EPx_REG_T * ctrl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;	
		
			ctrl_ep3->ctrl.mBits.pack_size   	    = len ;
			ctrl_ep3->transfer_size.mBits.num   	= len ;
			//Clear fifo 
            ctrl_usb->ep_rest.mBits.ep3_fifo_rst = TRUE;

        	USB_TxDWordToFIFO (pBuf ,size,USB_EP3);
    		USB_EPxDataInCtrl  (TRUE,USB_EP3);

			while (!(ctrl_ep3->int_sts.mBits.transfer_end)){//wait until transfer end
			}
	        //clear the int status of ep3's tranfer end 
	        ctrl_ep3->int_clr.mBits.transfer_end = 1;  

    		break;
    	}
    	case USB_EP1:
    	{
        	USB_EPx_REG_T * ctrl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;	
		
			ctrl_ep1->ctrl.mBits.pack_size   	    = len ;
			ctrl_ep1->transfer_size.mBits.num   	= len ;
			//Clear fifo 
            ctrl_usb->ep_rest.mBits.ep1_fifo_rst = TRUE;

        	USB_TxDWordToFIFO (pBuf ,size,USB_EP1);
    		USB_EPxDataInCtrl  (TRUE,USB_EP1);

			while (!(ctrl_ep1->int_sts.mBits.transfer_end)){//wait until transfer end
			}
	        //clear the int status of ep3's tranfer end 
	        ctrl_ep1->int_clr.mBits.transfer_end = 1;  

    		break;
    	}
    	case USB_EP0_IN:
    	{
        	USB_EP0_REG_T * ctrl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
		
			ctrl_ep0->ctrl.mBits.pack_size	   	    = len ;
			ctrl_ep0->transfer_size_in.mBits.num	= len ;
			//Clear fifo 
            ctrl_usb->ep_rest.mBits.ep0_in_fifo_rst = TRUE;

           	USB_TxDWordToFIFO (pBuf ,size,USB_EP0_IN);
    		USB_EPxDataInCtrl  (TRUE,USB_EP0_IN);

    		break;
    	}
   
    	default:
    	    break ;
	}

    return len ;
}

/*****************************************************************************/
//  Description:    write to usb inendp fifo without interrupt
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           called by assert print
//	                param1: endp number
//                  param2: write data buffer 
//                  param3: write length
/*****************************************************************************/
PUBLIC void UMOD_WritetoEndp (uint8 ep_num,uint8 *write_buf,uint32 send_len)
{
    USB_EPx_REG_T * ctl_epx = NULL ;	
    uint32 i;
    
    if(send_len == 0)
        return;
     
    ctl_epx = (USB_EPx_REG_T *) (USB_EP0_BASE + ep_num * 0x40 );  
    
    //Clear transfer end int_en bit
	ctl_epx->int_clr.mBits.transfer_end = 1 ;  
	
	//Epx max packsize is 64 bytes .
    if ((send_len >> 6) > 0)
    {
    	for (i=0; i<(send_len >> 6); i++)
    	{
	    	UMOD_EPxSendDataInLoop(ep_num ,(uint32 * )(write_buf + (i<<6)),USB_FIFO_MAX_BYTE);
    	}

    	if(send_len%USB_FIFO_MAX_BYTE)
    	{
    	    UMOD_EPxSendDataInLoop(ep_num ,(uint32 * )(write_buf + (i<<6)),(send_len - (i<<6)));
    	}
    	
    }
    else
    {
	    UMOD_EPxSendDataInLoop(ep_num ,(uint32 * )write_buf,send_len);
    } 

}




/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jim.Lin
//	Note:           no
/*****************************************************************************/
PUBLIC void UMOD_GetDevDescriptor (uint8 bDirection,uint16 wValue,\
                            uint16 wIndex ,uint16 wLen)
{
	USB_HWORD_VALUE_U value;
	uint16 length = 0;
	char *pData = 0;

	value.wValue  = wValue;
	switch (value.bmDescriptor.type){
 	
 	case USB_DEVICE_DESCRIPTOR_TYPE:
		if (wLen >= 0x040){
			length = 18;			
		}
		else{
            length = wLen;
        }
        pData 	= (char *)&g_UMOD_DeviceDescr;
        break;
    
    case USB_CONFIGURATION_DESCRIPTOR_TYPE:
		/*
		if (wLen >= 0x0ff){
			length  = UBOT_CONFIG_DESCCIPTOR_LEN;			
		}
		else{
		    length  = wLen;
		}
		*/   //jim 1-15
		if (wLen >= 0x0ff){
			length  = UMOD_CDC_CONFIG_DESCCIPTOR_LEN;			
		}
		else if (wLen==UMOD_CDC_CONFIG_DESCCIPTOR_LEN)
			{
			  length  = UMOD_CDC_CONFIG_DESCCIPTOR_LEN;	
			}

		else{
		    length  = 9;
		}
        pData   = (char *)&g_UMOD_ConfigDescr;
        break;
    
    case USB_STRING_DESCRIPTOR_TYPE:
    
        switch (value.bmDescriptor.index){
            //USB 2.0 spec chapter 9.6.7 
            case USB_STR_INDEX_LANGUAGE :
                if (wLen > sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T)){
                    length  = sizeof(USB_STRING_LANGUAGE_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }
                pData   = (char *)&g_UMOD_LanguageStr;
                break;
            //USB 2.0 spec chapter 9.6.1
            case USB_STR_INDEX_SERIALNUMBER:
                if (wLen > sizeof(USB_STRING_SERIALNUMBER_DESCRIPTOR_T)){
                    length  = sizeof(USB_STRING_SERIALNUMBER_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }
                pData   = (char *)&g_UMOD_SerialNumStr;
                break;
    
            case USB_STR_INDEX_MANUFACTURER:
                if (wLen > sizeof(USB_STRING_MANUFACTURER_DESCRIPTOR_T)){
                    length  = sizeof(USB_STRING_MANUFACTURER_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }

                pData   = (char *)&g_UMOD_StrManufacturer;
                break;

            case USB_STR_INDEX_PRODUCT:
                if (wLen > sizeof(USB_STRING_PRODUCT_DESCRIPTOR_T)){
                    length  = sizeof(USB_STRING_PRODUCT_DESCRIPTOR_T);        
                }
                else{
                    length  = wLen;
                }

                pData   = (char *)&g_UMOD_StrProduct;
                break;

            default:
                break;
        }
        break;
    default:
        break;
	}
	//Write data to EP in & send data to host  ;
	//Jim 1-15
	//g_UBOT_manager.cache.lock = UBOT_READY_LOCK
	
	 UMOD_EPxSendData (USB_EP0_IN,(uint32 *)pData,length);
	
	
}


/*****************************************************************************/
//  Description:    DO with get line coding
//	Global resource dependence: 
//   Argument:   None
//  Author:         Jim.Lin
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_GET_LINE_CODIING()
{ 
  char *pbuf=0;
    pbuf=(char *)&LINE_CODING;
   UMOD_EPxSendData (USB_EP0_IN,(uint32 *)pbuf,7);
   return;
   
}


/*****************************************************************************/
//  Description:    DO with set buf ready
//	Global resource dependence: 
//  Author:         Jim.Lin
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_SET_BUF_READY()
{ 
    USB_EP0_REG_T * ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
	//Set buffer ready ;	
	ctl_ep0->ctrl.mBits.buf_ready = 1;
	
   return;
   
}


/*****************************************************************************/
//  Description:    Set Ep3 ready for transfer
//	Global resource dependence: 
//  Author:         Jim.Lin
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_SET_EP3_READY(void)
{ char *pbuf=0;
  pbuf=(char *)&interrupt_array;
  UMOD_EPxSendData(USB_EP3,(uint32 *)pbuf,10);
   return;
}
/*****************************************************************************/
//  Description:    DO with set line coding
//	Global resource dependence: 
//  Author:         Jim.Lin
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_SET_LINE_CODIING()
{ 
    USB_EP0_REG_T * ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
	USB_DEV_REG_T * ctrl_usb = (USB_DEV_REG_T *) USB_REG_BASE ;
	//Set buffer ready ;	
	
#if 0
  char *pbuf=0;
    pbuf=(char *)&SET_LINE_CODING;
  UMOD_EPxReceiveData1(USB_EP0_OUT,(uint32 *)pbuf);
 #endif

 ctrl_usb->ep_rest.mBits.ep0_out_fifo_rst = TRUE;
  

 #ifdef _UMOD_SC8800H_
    ctl_ep0->ctrl.mBits.buf_ready = 1;  //0;
 #else
    ctl_ep0->ctrl.mBits.buf_ready = 0;
 #endif
  
   return;
   
}



/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
