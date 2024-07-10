/******************************************************************************
 ** File Name:    ucom_api.h                                                  *
 ** Author:       weihua.wang                                                 *
 ** DATE:         2005-11-8                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 2005-11-8     weihua.wang     Create.                                     *
 ******************************************************************************/
#ifndef _UCOM_API_H_
#define _UCOM_API_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
//#include "upm_api.h"
#include "com_drv.h"
#include "usb_drv.h"
//#include "gadget.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
typedef enum
{
	UCOM_CREATE_COM_DEBUG=      ( 0x00 ), /*data for debug port*//*same with 0x02*/
	UCOM_CREATE_COM_DATA=       ( 0x01 ), /*data for at port           */
	UCOM_CREATE_COM_DEBUGDATA=  ( 0x02 ), /*data for at and debug port */
	UCOM_CREATE_COM_DATADISK=   ( 0x03 ), /*data port + rom disk       */
	UCOM_CREATE_COM_DEBUGDATA_MODEM= (0x04) ,/*data + debug + cp trace + modem */
	UCOM_CREATE_COM_NPI =( 0x05 ) , /*cali mode:diag and modemlog */
	UCOM_CREATE_COM_CALI =( 0x06 ) , /*enter cali mode only one port:U2S DIAG */
	UCOM_CREATE_COM_ASSERT =( 0x07 ) , /*enter assert mode , two ports: DIAG + CP log not used*/
	UCOM_CREATE_COM_DEBUGDATA_UMS,
	DRV_USB_UDISK,
	DRV_USB_MTP,
	DRV_USB_RNDIS,
	DRV_USB_CHARGER_ONLY,
	DRV_USB_MAX_TYPE
} drvUSBWorkMode_t;


/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
//#define UCOM_CMD_START  ((UPM_VIRTUAL_COM << 16) & 0xFFFFFFFFUL)
#define UCOM_CMD_START  ((2 << 16) & 0xFFFFFFFFUL)

typedef enum {
    UCOM_CMD_PLUG_IN,
    UCOM_CMD_PLUG_OUT,
    UCOM_CMD_EXECUTE,
    UCOM_CMD_TX_CONTINUE,
    UCOM_CMD_MAX
}UCOM_COMMAND_E;

#if 0
typedef struct  upm_isr_tag {
    uint32        (*main)      (uint32 cmd,uint32 param,void *ptr);
    void          (*init)      (void);
    void          (*exit)      (void);
    USB_ISR_PTR_T irq;
    uint32        state;
}UPM_FUNCTION_PTR_T;
#endif
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This is USLA's main body .
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
//const UPM_FUNCTION_PTR_T * UCOM_GetHandler(void);
//const USB_GADGET_DRIVER_T * UCOM_GetHandler(void);

/*****************************************************************************/
//  Description:    usb virtual com creat
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 UCOM_Creat(uint32 port_num, 
                               UART_INIT_PARA_T * para, 
                               UartCallback func);

/*****************************************************************************/
//  Description:    com port close
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 UCOM_Close(uint32 phy_port);

/*****************************************************************************/
//  Description:    call by sio to set usb in_endp enable/disable
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//                  BOOLEAN is_enable: TRUE/FALSE
//	Note:           this func is called after UPlayer callback
/*****************************************************************************/
PUBLIC void UCOM_TxEnable(uint32 phy_port,BOOLEAN is_enable);

/*****************************************************************************/
//  Description:    return the usb in endp fifo cnt
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//	Note:           here only return max fifo size for in endp
/*****************************************************************************/
PUBLIC uint32 UCOM_GetTxFIFOcnt(uint32 port_num);

/*****************************************************************************/
//  Description:    get char from fifo in loop mode
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//	Note:           when this func is called, it's indicated that current is assert
//                  mode, no interrupt can happen, 
/*****************************************************************************/
PUBLIC int32 UCOM_GetRxChar(uint32 phy_port);

/*****************************************************************************/
//  Description:    put char to fifo in loop mode
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//                  uint8 nchar: char to be sent
//	Note:           when this func is called, it's indicated that current is assert
//                  mode, no interrupt can happen, 
/*****************************************************************************/
PUBLIC void UCOM_PutTxChar(uint32 phy_port,uint8 nchar);

/*****************************************************************************/
//  Description:    put chars to fifo in loop mode, it will do '0x7e' convert in func
//                  to reduce the time
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//                  uint8* data: data to be send
//                  int32 len  : data size to be sent
//	Note:           function is called when dump memory, to reduce the time of repeat call
/*****************************************************************************/
PUBLIC void UCOM_PutTxChars(uint32 port_num,uint8* data, int32 len);

/*****************************************************************************/
//  Description:    indicating send char will be finished, remain char should be flushed
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//	Note:           uint32 phy_port: virtual com number
/*****************************************************************************/
PUBLIC void UCOM_Flush(uint32 phy_port);

/*****************************************************************************/
//  Description:    call by sio to set the usb out_endp enable/disable
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_RxEnable(uint32 phy_port,BOOLEAN is_enable);

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
PUBLIC uint32 UCOM_ReadData(uint32 port_num, uint8* buffer, uint32 length);

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
PUBLIC uint32 UCOM_WriteData(uint32 port_num, 
                          const uint8* buffer,
                           uint32 length);

#ifdef UCOM_UMS_SUPPORT

typedef void (*UCOMCallback) (uint32 event);
/*****************************************************************************/
//  Description:    UCOM_UMS_Register
//	Global resource dependence: 
//  Author:         weihua.wang
//	Param:          
//                  UartCallback func :uplayer callback
//  Note:
/*****************************************************************************/
PUBLIC uint32 UCOM_UMS_Register( UCOMCallback func);

/*****************************************************************************/
//  Description:   This function read data from receiving buffer.It is called *
//                 when up layfer receives the event of "EVENT_DATA_TO_READ". *    
//  Author:        weihua.wang                                                *
//  Param :                                                                   *
//                 uint8* buffer   : des buffer header address                *
//                 uint32 length   : size to be read                          *
//  Return:        if success,return size read in effect
//	Note  :                                                                   *
/*****************************************************************************/ 
PUBLIC uint32 UCOM_UMS_Read(uint8* buffer, uint32 length);

/*****************************************************************************/
//  Description:   This function record data which will be written to         *
//                 tx fifo and enable interrupt. The lengthof data should be  * 
//                 not larger than FIFO_SIZE(64B).                            *
//  Author:        weihua.wang                                                *
//  Param :                                                                   *
//                 uint8* buffer   : src buffer header address                *
//                 uint32 length   : size to be writed                        *
//  Return:        if success,return size writed in effect                    *
//	Note  :                                                                   *
/*****************************************************************************/ 
PUBLIC uint32 UCOM_UMS_Write( const uint8* buffer,uint32 length);

#endif

/*****************************************************************************/
//  Description:    creat usb virtual com type 
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:     type: UCOM_CREATE_COM_DEBUG    (only one virtualcom for COM_DEBUG)
//                  UCOM_CREATE_COM_DATA     (only one virtualcom for COM_DATA) 
//                  UCOM_CREATE_COM_DEBUGDATA(only tow virtualcom for BOTH) 
/*****************************************************************************/
PUBLIC void UCOM_CreatForCOM(uint32 type);

/******************************************************************************/
// Description:   Change rx_endp of vcom0 from 4 to 2
// Dependence: 
// Author:        Haifeng.Yang
// Note:          used for auto_download ONLY!
/******************************************************************************/
PUBLIC uint32 UCOM_SetDloaderEndpointMap(void);

/******************************************************************************/
// Description:   get related port device status
// Dependence: 
// Author:        
// Note:          port_num:  usb endpoint index
/******************************************************************************/
PUBLIC uint32 UCOM_IsInPortIdle (uint32 port_num);

extern PUBLIC void UCOM_CreatForCOM (uint32 type);
extern PUBLIC void UCOM_SetWorkMode(uint8 service_type, uint8 calibmode);
extern PUBLIC drvUSBWorkMode_t UCOM_GetUcomType(void);
PUBLIC BOOLEAN UCOM_PortIsEnabled (uint32 port_num);

/*****************************************************************************/
//  Description:   This function chek whether UCOM port is ready for tx  	  * 
//                                         									  *
//  Author:        lianlian.huang                                             *
//  Param :        uint32 port_num : usb virtual com number index             *
//																			  *
//  Return:        1 means ready for tx,0 means not ready                     *
//	Note  :                                                                   *
/*****************************************************************************/ 

extern PUBLIC BOOLEAN UCOM_PortIsOpenForWrite (uint32 port_num);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
