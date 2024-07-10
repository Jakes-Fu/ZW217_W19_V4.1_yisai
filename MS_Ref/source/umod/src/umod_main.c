/******************************************************************************
 ** File Name:    UMOD_main.c                                                  *
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
 ** 2007-6-20       Elly.Zhou  modified func.UMOD_init,UMOD_CreatForCOM
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "UPM_api.h"
#include "UMOD_main.h"
#include "UMOD_handler.h"
#include "UMOD_api.h"
#include "dal_chr.h"
#include "com_drv.h"
#include "UMOD_drv.h"
#include "sio.h"

#if defined(_UMOD_SC6800_)||defined(_UMOD_SC8800H_)
    #include "ref_outport.h"
#endif


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
#define UMOD_INVALID_NUM    (0xffff)
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
LOCAL const UPM_FUNCTION_PTR_T g_UMOD_handler = {
    //Basic function ;
    UMOD_main,
    UMOD_init,
    UMOD_exit,
    //Irq  
    UMOD_usb_irq,
    UMOD_dma_epin,
    UMOD_dma_epout,
    UMOD_dma_ep1,
    UMOD_dma_ep2,
    UMOD_dma_ep3,
    UMOD_dma_ep4,
    //State
    UPM_DISABLE
};

/*----------------------------------------------------------------------------*
**                         Local Data                                        *
**---------------------------------------------------------------------------*/
LOCAL const UMOD_PortTbl_T  s_umod_port_index[UMOD_MAX_COM_NUM] = {
//----dev_index--phyport---out endp---in endp----------//     
    {    0x0,   USB_COM0,  USB_EP4,  USB_EP3  },
    {    0x1,   USB_COM1,  USB_EP2,  USB_EP1  }
};

//to distinguish whether in assert mode 
LOCAL BOOLEAN s_umod_isassert;  


/*--------------------------- Global Data -----------------------------------*/
UMOD_MANAGER_T  g_UMOD_manager ;
USB_FUNCTION 	g_umod_functions [USB_MAX_REQ_TYPE][USB_MAX_RECIPIENT][USB_MAX_REQUEST];
/*--------------------------- External Data ---------------------------------*/
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
LOCAL void UMOD_AssertInit(uint32 port_num);

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_init (void)
{
    	int32 i;
	USB_FUNCTION 	* cpFuc;
	SIO_CONTROL_S   m_dcb;
    
	cpFuc = &g_umod_functions[0][0][0];
	for (i=0;i< (USB_MAX_REQ_TYPE * USB_MAX_RECIPIENT* USB_MAX_REQUEST);i++ ){
		* (cpFuc + i) = (USB_FUNCTION) UMOD_DoNothing ;
	}
	
        m_dcb.flow_control   = 0;
	m_dcb.baud_rate      = SIO_GetBaudRate(COM_DATA);     
	
	UMOD_EP0Config();
	UMOD_EP1Config();
	UMOD_EP2Config();
	UMOD_EP3Config();
	//UCOM_EP4Config();

    	USB_PowerSet(FALSE);//bus power;
	
	//current assert mode is null
	s_umod_isassert = FALSE;


}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_exit (void)
{
    int32 i;
    
	 SCI_DisableIRQ();
	 SIO_Close(COM_DATA);        
	 SCI_RestoreIRQ();     	
   
   //buffer init
    for(i = 0; i < UMOD_MAX_COM_NUM; i++)
    {
        g_UMOD_manager.dev[i].state         = UMOD_DEV_NO_PRESENT;
        g_UMOD_manager.dev[i].rx_buf.read  = 0;
        g_UMOD_manager.dev[i].rx_buf.write = 0;
        g_UMOD_manager.dev[i].tx_buf.read  = 0;
        g_UMOD_manager.dev[i].tx_buf.write = 0;
    }
}

/*****************************************************************************/
//  Description:    This is USLA's main body .
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
uint32 UMOD_main (uint32 command,uint32 param,void * ptr)
{
    int32 i;
	switch (command){

    	case UPM_CMD_PLUG_IN:
            //Start process masstorage .
    	    UPM_SendCommand (UPM_MODEM,UMOD_CMD_EXECUTE,NULL,NULL);
    	    break;

    	case UPM_CMD_PLUG_OUT:
	        USB_PowerSet(TRUE);//Self power;

		    for(i = 0; i < UMOD_MAX_COM_NUM; i++)
		    {
		        g_UMOD_manager.dev[i].state        = UMOD_DEV_NO_PRESENT;
		        g_UMOD_manager.dev[i].rx_buf.read  = 0;
		        g_UMOD_manager.dev[i].rx_buf.write = 0;
		        g_UMOD_manager.dev[i].tx_buf.read  = 0;
		        g_UMOD_manager.dev[i].tx_buf.write = 0;
		    }

    	    break;
        case UMOD_CMD_EXECUTE:
            //do null
            break;
        default :
            break;	
	}

	return TRUE ;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
const UPM_FUNCTION_PTR_T * UMOD_GetHandler(void)
{
    return &g_UMOD_handler;
}

/*****************************************************************************/
//  Description:    return virtual com number
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           in current condition,we can only enum one port
/*****************************************************************************/
PUBLIC uint32 UMOD_GetComNum(void)
{
    return UMOD_MAX_COM_NUM;
}

/*****************************************************************************/
//  Description:    creat usb virtual com type 
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:     type: UMOD_CREATE_COM_DEBUG    (only one virtualcom for COM_DEBUG)
//                  UMOD_CREATE_COM_DATA     (only one virtualcom for COM_DATA) 
//                  UMOD_CREATE_COM_DEBUGDATA(only tow virtualcom for BOTH) 
/*****************************************************************************/
PUBLIC void UMOD_CreatForCOM(uint32 type)
{
	SIO_CONTROL_S   m_dcb_data;

    UMOD_ASSERT(type < UMOD_CREATE_MAX_TYPE);

    switch(type){   
        case UMOD_CREATE_COM_DATA:
            {
	  		    m_dcb_data.flow_control = 0;
			    m_dcb_data.baud_rate    = SIO_GetBaudRate(COM_DATA);
				
			    SCI_DisableIRQ();
			    SIO_Create(COM_DATA,VIR_COM2,&m_dcb_data);
			    SCI_RestoreIRQ();
            }
            break;
			

        default:
            break;
    }
    
  UPM_SetService(UPM_MODEM);

}

/*****************************************************************************/
//  Description:    usb virtual com creat
//	Global resource dependence: 
//  Author:         weihua.wang
//	Param:          uint32 port_num: usb virtual com num USB_COM0/USB_COM1
//                  UART_INIT_PARA_T* :include param for uart, only useful for uart
//                  UartCallback func :uplayer callback
//  Note:
/*****************************************************************************/
PUBLIC uint32 UMOD_Creat(uint32 port_num, 
                               UART_INIT_PARA_T * para, 
                               UartCallback func)
{
    UMOD_DEVICE_T * dev_ptr = NULL;

    UMOD_ASSERT(func != NULL);
    UMOD_ASSERT(port_num < USB_MAX_NUM);

    dev_ptr = UMOD_GetDevfromPort(port_num);

    if (UMOD_ALREADY_OPEN == dev_ptr->open_flag)
    {
        return UART_PORT_REOPEN_ERR;
    }

    //buffer init
    dev_ptr->state        = UMOD_DEV_NO_PRESENT;
    dev_ptr->rx_buf.read  = 0;
    dev_ptr->rx_buf.write = 0;
    dev_ptr->tx_buf.read  = 0;
    dev_ptr->tx_buf.write = 0;
    dev_ptr->callback     = func;  //register callback
    dev_ptr->open_flag    = UMOD_ALREADY_OPEN;
    
    dev_ptr->callback(EVENT_INIT_COMPLETE);//inform uplayer
    
    return UART_SUCCESS;
}

/*****************************************************************************/
//  Description:  com port close
//	Global resource dependence: 
//  Author:       weihua.wang
//  Param :       uint32 port_num: usb virtual com number   
//	Note  :       
/*****************************************************************************/
PUBLIC uint32 UMOD_Close(uint32 port_num)
{
    UMOD_DEVICE_T * dev_ptr = NULL;

    UMOD_ASSERT(port_num < USB_MAX_NUM);

    //get dev ptr
    dev_ptr = UMOD_GetDevfromPort(port_num);
    
    if (UMOD_ALREADY_OPEN != dev_ptr->open_flag)
    {
        UMOD_ASSERT(0);
        return UART_PORT_RECLOSE_ERR;
    }

    dev_ptr->state = UMOD_DEV_NO_PRESENT;
	dev_ptr->rx_buf.read  = 0;
	dev_ptr->rx_buf.write = 0;
	dev_ptr->tx_buf.read  = 0;
	dev_ptr->tx_buf.write = 0;
    dev_ptr->open_flag    = 0;
    dev_ptr->callback(EVENT_SHUTDOWN_COMPLETE);
    
    //here, setting the value maybe hold a candle to the sun  
    s_umod_isassert = FALSE;   
    
    return UART_SUCCESS;
}

/*****************************************************************************/
//  Description:   This function read data from receiving buffer.It is called *
//                 when up layfer receives the event of "EVENT_DATA_TO_READ". *    
//  Author:        weihua.wang                                                *
//  Param :        uint32 port_num : usb virtual com number index             *
//                 uint8* buffer   : des buffer header address                *
//                 uint32 length   : size to be read                          *
//  Return:        if success,return size read in effect
//	Note  :                                                                   *
/*****************************************************************************/ 
PUBLIC uint32 UMOD_ReadData(uint32 port_num, uint8* buffer, uint32 length)
{
    UMOD_DEVICE_T * dev_ptr = NULL;
    uint32 count = 0;
    SCI_ASSERT(port_num < USB_MAX_NUM);
    SCI_ASSERT(buffer != NULL);
   
    dev_ptr = UMOD_GetDevfromPort(port_num); 

    if (UMOD_ALREADY_OPEN != dev_ptr->open_flag)
    {
        //this port is not initilized, so we can not shutdown
        UMOD_ASSERT(0);
        return UART_FAILURE;
    }
    
    //read data size musb less than max value
    if(length > USB_FIFO_MAX_BYTE)
    {
        length = USB_FIFO_MAX_BYTE;
    }
    
    count = (length > dev_ptr->rx_buf.write)? (dev_ptr->rx_buf.write):length ;

    SCI_MEMCPY(buffer,dev_ptr->rx_buf.usb_buf,count);

    //read buffer reset to zero
     dev_ptr->rx_buf.write = 0;
     
    //because read max size is less than max fifo size
    //the default conditon is start point < end_point
    //others, we don't care
    
    return count;
}

/*****************************************************************************/
//  Description:   This function record data which will be written to         *
//                 tx fifo and enable interrupt. The lengthof data should be  * 
//                 not larger than FIFO_SIZE(64B).                            *
//  Author:        weihua.wang                                                *
//  Param :        uint32 port_num : usb virtual com number index             *
//                 uint8* buffer   : src buffer header address                *
//                 uint32 length   : size to be writed                        *
//  Return:        if success,return size writed in effect                    *
//	Note  :                                                                   *
/*****************************************************************************/ 
PUBLIC uint32 UMOD_WriteData(uint32 port_num, 
                          const uint8* buffer,
                           uint32 length)
{
    UMOD_DEVICE_T * dev_ptr = NULL;
    uint8  endp_num;

    SCI_ASSERT(port_num < USB_MAX_NUM);
    UMOD_ASSERT(buffer != NULL);
   
    dev_ptr = UMOD_GetDevfromPort(port_num); 

    if (UMOD_ALREADY_OPEN != dev_ptr->open_flag)
    {
        //this port is not initilized, so we can not shutdown
        UMOD_ASSERT(0);
        return UART_FAILURE;
    }
    
    //read data size musb less than max value
    if(length > USB_FIFO_MAX_BYTE)
    {
        length = USB_FIFO_MAX_BYTE;
    }
    
    if(length == 0){//no data to be writed
        return 0;
    }
    
  
    SCI_MEMCPY(dev_ptr->tx_buf.usb_buf,buffer,length);

    //buffer ptr should be word align
    endp_num = s_umod_port_index[port_num].tx_endp;
    UMOD_EPxSendData(endp_num,(uint32*)dev_ptr->tx_buf.usb_buf,length);

    return length;
}                  

/*****************************************************************************/
//  Description:    call by sio to set usb in_endp enable/disable
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//                  BOOLEAN is_enable: TRUE/FALSE
//	Note:           this func is called after UPlayer callback
/*****************************************************************************/
PUBLIC void UMOD_TxEnable(uint32 port_num,BOOLEAN is_enable)
{
   	USB_EPx_REG_T * ctrl_epx = NULL; 
    UMOD_DEVICE_T * dev_ptr  = NULL;
    
    UMOD_ASSERT(port_num < USB_MAX_NUM);
    
    dev_ptr = UMOD_GetDevfromPort(port_num);
    //if current usb disconnect, return directly
    if(FALSE == UPM_GetConnectState()){
        return;
    }
    
    //if current virtual com is not ready, then do null
    if(dev_ptr->state != UMOD_DEV_READY){
        return;    
    }

    //in assert mode,no interrupt occur,return directly  
    if(UMOD_IsInAssert()){ 
        return;
    }
    
    //set bit of data not ready to produce the int
    ctrl_epx = (USB_EPx_REG_T *)(USB_EP0_BASE + 
                   (s_umod_port_index[port_num].tx_endp) * 0x40);

    if(is_enable)
    {
        //only set this bit when it's zero 
        if( ctrl_epx->int_ctrl.mBits.data_not_ready == 0)
        {
            ctrl_epx->int_ctrl.mBits.data_not_ready = 1;      
        }
    }
    else
    {
        ctrl_epx->int_clr.mBits.data_not_ready = 1; //int clear
        ctrl_epx->int_ctrl.mBits.data_not_ready = 0;//int disable 
    }
}

/*****************************************************************************/
//  Description:    call by sio to set the usb out_endp enable/disable
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//                  BOOLEAN is_enable: TRUE/FALSE
//	Note:           this func is called after UPlayer callback
/*****************************************************************************/
PUBLIC void UMOD_RxEnable(uint32 port_num,BOOLEAN is_enable)
{
   	USB_EPx_REG_T * ctrl_epx = NULL; 
    UMOD_DEVICE_T * dev_ptr  = NULL;
    
    UMOD_ASSERT(port_num < USB_MAX_NUM);
    
    //if current usb disconnect, return directly
    if(FALSE == UPM_GetConnectState()){
        return;
    }
    
    dev_ptr = UMOD_GetDevfromPort(port_num);
    //if current virtual com is not ready, then do null
    if(dev_ptr->state != UMOD_DEV_READY){
        return;    
    }
    
    //in assert mode,no interrupt occur,return directly  
    if(UMOD_IsInAssert()){ 
        return;
    }
    
    //set bit of data not ready to produce the int
    ctrl_epx = (USB_EPx_REG_T *)(USB_EP0_BASE +
                   (s_umod_port_index[port_num].rx_endp) * 0x40);

    if(is_enable)
    {
        ctrl_epx->int_ctrl.mBits.transfer_end = 1;      
    }
    else
    {
        ctrl_epx->int_ctrl.mBits.transfer_end = 0;//int disable 
              
    }
}

/*****************************************************************************/
//  Description:    return the usb in endp fifo cnt
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//	Note:           here only return max fifo size for in endp
/*****************************************************************************/
PUBLIC uint32 UMOD_GetTxFIFOcnt(uint32 port_num)
{
    UMOD_ASSERT(port_num < USB_MAX_NUM);
    
    return USB_FIFO_MAX_BYTE;
    
}

/*****************************************************************************/
//  Description:    get char from fifo in loop mode
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//	Note:           when this func is called, it's indicated that current is assert
//                  mode, no interrupt can happen, 
/*****************************************************************************/
PUBLIC int32 UMOD_GetRxChar(uint32 port_num)
{
    SCI_ASSERT(port_num < USB_MAX_NUM);
    
    return UMOD_GetChar(port_num);
}

/*****************************************************************************/
//  Description:    put char to fifo in loop mode
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//                  uint8 nchar: char to be sent
//	Note:           when this func is called, it's indicated that current is assert
//                  mode, no interrupt can happen, 
/*****************************************************************************/
PUBLIC void UMOD_PutTxChar(uint32 port_num,uint8 nchar)
{
    UMOD_DEVICE_T  * dev_ptr    = NULL;
    uint8 endp_num;
	
    SCI_ASSERT(port_num < USB_MAX_NUM);
    
	dev_ptr = UMOD_GetDevfromPort(port_num);
     
	if(!s_umod_isassert)
	{
	    s_umod_isassert = TRUE;
	    UMOD_AssertInit(port_num);
	}
	//if virtual is not ready,then exit with doing nothing
	if(dev_ptr->state != UMOD_DEV_READY)
	{
	    return;
	}   
    
    dev_ptr->tx_buf.usb_buf[dev_ptr->tx_buf.write++] = nchar;
     
    if(dev_ptr->tx_buf.write >= USB_RECV_LIMIT)
    {
        endp_num = s_umod_port_index[port_num ].tx_endp;
        UMOD_WritetoEndp(endp_num,dev_ptr->tx_buf.usb_buf,dev_ptr->tx_buf.write);
        dev_ptr->tx_buf.write = 0;
    }
}

/*****************************************************************************/
//  Description:    indicating send char will be finished, remain char should be flushed
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_Flush(uint32 port_num)
{
    UMOD_DEVICE_T  * dev_ptr    = NULL;
    uint8 endp_num;
	
    SCI_ASSERT(port_num < USB_MAX_NUM);

	dev_ptr = UMOD_GetDevfromPort(port_num);
     
	//if virtual is not ready,then exit with doing nothing
	if(dev_ptr->state != UMOD_DEV_READY)
	    return;

    if(dev_ptr->tx_buf.write)
    {
        endp_num = s_umod_port_index[port_num].tx_endp;
        UMOD_WritetoEndp(endp_num,dev_ptr->tx_buf.usb_buf,dev_ptr->tx_buf.write);
        dev_ptr->tx_buf.write = 0;
    }
}

/*****************************************************************************/
//  Description:    get dev point index from endp
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           called only in ucom project(internal)
/*****************************************************************************/
PUBLIC UMOD_DEVICE_T * UMOD_getDevfromEndp(uint8 endp_num)
{
    int32 i;
    uint8 index=0;
   
    UMOD_ASSERT((endp_num >= USB_EP1)&&(endp_num < USB_EPx_NUMBER));

    for(i = 0; i < UMOD_MAX_COM_NUM; i++)
    {
        if((s_umod_port_index[i].rx_endp == endp_num)||(s_umod_port_index[i].tx_endp == endp_num))
        {
            index = s_umod_port_index[i].dev_index;
            break;
        }
    }
    
    if(i >= UMOD_MAX_COM_NUM)
    {
        UMOD_ASSERT(0);
    }
    
    return &g_UMOD_manager.dev[index];
}

/*****************************************************************************/
//  Description:    get devptr according to port_num
//	Global resource dependence: 
//  Author:         weihua.wang
//  param :         uint8 port_num: physical usb protnum
//	Note:           
/*****************************************************************************/
PUBLIC UMOD_DEVICE_T * UMOD_GetDevfromPort(uint8 port_num)
{
    uint8 i;
    uint8 index=0;
    
    UMOD_ASSERT(port_num < USB_MAX_NUM);

    for(i = 0; i< UMOD_MAX_COM_NUM; i++)
    {
        if(s_umod_port_index[i].port_num == port_num)
        {
            index = s_umod_port_index[i].dev_index;
            break;
        }
    }
    
    if(i > UMOD_MAX_COM_NUM)
    {
        UMOD_ASSERT(0);
    }
    
    return &g_UMOD_manager.dev[index];
}

/*****************************************************************************/
//  Description:    get assert mode 
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_IsInAssert(void)
{
    return s_umod_isassert;
}

/*****************************************************************************/
//  Description:    get cur usbvirtual com work mode,to judge the device and 
//                  configure descriptor type (one interface or two interface)
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
#if 0
PUBLIC BOOLEAN UMOD_IsNeedTwoInterface(void)
{
    if(UMOD_CREATE_COM_DATA == s_umod_work_mode)
    {
        return FALSE;
    }
    
    return TRUE;
}
#endif

/*****************************************************************************/
//  Description:    when assert happen,init usb virtual com first
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//	Note:           to clear all usb interrtup sts,and init usb buffer,if current
//                  usb ldo is off, then build up usb connection 
/*****************************************************************************/
LOCAL void UMOD_AssertInit(uint32 port_num)
{
	GR_ANATST_CTL_U *pAnatst = (GR_ANATST_CTL_U *)GR_ANATST_CTL;
    UMOD_DEVICE_T  * dev_ptr    = NULL;
    
    if(port_num > USB_MAX_NUM)
        return;
    
    //disable usb int
    CHIPDRV_DisableIRQINT(TB_USB_INT);

    //clear the usb endp intbits
    {
	    USB_EP0_REG_T * ctl_ep0 = (USB_EP0_REG_T *) USB_EP0_BASE ;
		USB_EPx_REG_T * ctl_ep1 = (USB_EPx_REG_T *) USB_EP1_BASE ;
		USB_EPx_REG_T * ctl_ep2 = (USB_EPx_REG_T *) USB_EP2_BASE ;
		USB_EPx_REG_T * ctl_ep3 = (USB_EPx_REG_T *) USB_EP3_BASE ;
		USB_EPx_REG_T * ctl_ep4 = (USB_EPx_REG_T *) USB_EP4_BASE ;
		ctl_ep1->int_ctrl.mBits.data_not_ready = 0;
		ctl_ep3->int_ctrl.mBits.data_not_ready = 0;

		//when assert, we need to set transfer_end int
		ctl_ep3->int_ctrl.mBits.transfer_end	= 1 ;

		ctl_ep0->int_clr.dwValue = 0x3fff;
		ctl_ep1->int_clr.dwValue = 0x3fff ;
		ctl_ep2->int_clr.dwValue = 0x3fff ; 
		ctl_ep3->int_clr.dwValue = 0x3fff ; 
		ctl_ep4->int_clr.dwValue = 0x3fff ; 
	}

    //accordint with phy_port,find the device ptr
 	dev_ptr = UMOD_GetDevfromPort(port_num);
   
    //indicated current in assert debug mode
    s_umod_isassert = TRUE;

    if(dev_ptr->state == UMOD_DEV_READY)
    {
	    //Enable USB LDO
#if defined(_UMOD_SC6800_)||defined(_UMOD_SC8800H_)
	    pAnatst->mBits.Ldo_bpusb = 0x2;//BIT 13 is HIGH 12 is LOW;
#else
	    pAnatst->mBits.Ldo_bpusb = 0x1;//BIT 12 is HIGH 13 is LOW;
#endif
    }
    else{ //current upm service is others
	    AHB_CTL0_U      *pAhb     = (AHB_CTL0_U *)AHB_CTL0; 
	    USB_DEV_REG_T 	* ctl_usb = (USB_DEV_REG_T *) USB_REG_BASE;


#if defined(_UMOD_SC6800_)||defined(_UMOD_SC8800H_)
	    //Disable USB LDO
	    pAnatst->mBits.Ldo_bpusb = 0x1;//BIT 12 is HIGH 13 is LOW
		//Disable USB CLK ;
		pAhb->mBits.usbd_eb         = FALSE;
#else
	    //Disable USB LDO
	    pAnatst->mBits.Ldo_bpusb = 0x2;//BIT 13 is HIGH 12 is LOW
		//Disable USB CLK ;
		pAhb->mBits.Usbd_eb         = FALSE;
#endif
		//Disable USB device ;
		ctl_usb->ctrl.mBits.en 		= FALSE; 

		//Reset 48M and 12M clk ;
        USB_ClkReset();
        
		//Enable USB CLK ;
#if defined(_UMOD_SC6800_)||defined(_UMOD_SC8800H_)
		pAhb->mBits.usbd_eb       = TRUE;
#else
		pAhb->mBits.Usbd_eb       = TRUE;
#endif

	    OS_TickDelay(500);

		//Enable USB device ;
		ctl_usb->ctrl.mBits.en 	  = TRUE;
		//Set USB to self power;
		ctl_usb->ctrl.mBits.power = TRUE; 

	    UMOD_init();
		
        CHGMNG_SetChargeAdapter (CHGMNG_ADP_UNKNOW);//mingwei 2011
        
		//Enalbe USB LDO  ;
#if defined(_UMOD_SC6800_)||defined(_UMOD_SC8800H_)
	    pAnatst->mBits.Ldo_bpusb = 0x2;//BIT 13 is HIGH 12 is LOW;
#else
	    pAnatst->mBits.Ldo_bpusb = 0x1;//BIT 12 is HIGH 13 is LOW;
#endif
        s_umod_isassert = TRUE;
    }    
     
    //according with the phy port,find the devbuff and endp
    //and do the buffer init here first !!!
    dev_ptr->rx_buf.read  = 0;
    dev_ptr->rx_buf.write = 0;
    dev_ptr->tx_buf.read  = 0;
    dev_ptr->tx_buf.write = 0;

	//if virtual is not ready or busy,then exit with doing nothing
	if(dev_ptr->state != UMOD_DEV_READY)
	{
	    //current sts is busy, indicating that usb is transfering
    	if(dev_ptr->state == UMOD_DEV_BUSY)
    	{
    	    dev_ptr->state = UMOD_DEV_READY;
    	}
    }
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
