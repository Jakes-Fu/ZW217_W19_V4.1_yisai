/******************************************************************************
 ** File Name:    UPM_api.h                                                    *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005      Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _MMIUDISK_SIMU_H
#define _MMIUDISK_SIMU_H
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
//#include "USB_drv.h"
//#include "UPM_config.h"
#include "os_api.h"
#include "sci_service.h"
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
#define UPM_MANAGER            UPM_NO_SERVICE

//#define USB_WEBCAMERA_SUPPORT
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

// Definition for Keypad message
typedef enum
{
	USB_PLUGIN = (USB_SERVICE << 8) | 1 ,
	USB_PLUGOUT,
	USB_PLUG_OUT_START,
	USB_SERVICE_UDISK_STARTED,
	USB_SERVICE_UDISK_STOPPED,
	USB_SERVICE_UCOM_STARTED,
	USB_SERVICE_UCOM_STOPPED,
	USB_SERVICE_UPCC_STARTED,
	USB_SERVICE_UPCC_STOPPED,
	USB_SERVICE_LOG_STARTED,
	USB_SERVICE_LOG_STOPPED,
	USB_READY,
	USB_ERROR,
	USB_MSG_MAX
} USB_MSG_SERVICE_E;

typedef enum
{
	USB_SERVICE_NULL= 0,
	USB_SERVICE_UDISK ,
	USB_SERVICE_UCOM,
	USB_SERVICE_UPCC,
	USB_SERVICE_LOG,
	USB_SERVICE_MAX
}USB_SERVICE_E;


// ISR function pointer prototype.
typedef void (* TB_ISR_WIN) (uint32);
    
typedef struct  usb_isr_tag {
    TB_ISR_WIN     usb_irq;         
    TB_ISR_WIN     dma_epin;
    TB_ISR_WIN     dma_epout;
    TB_ISR_WIN     dma_ep1;
    TB_ISR_WIN     dma_ep2;
    TB_ISR_WIN     dma_ep3;
    TB_ISR_WIN     dma_ep4;
}USB_ISR_PTR_T;
    

typedef struct  upm_isr_tag {
    uint32        (*main)      (uint32 cmd,uint32 param,void *ptr);
    void          (*init)      (void);
    USB_ISR_PTR_T irq;
}UPM_FUNCTION_PTR_T;



//Product Driver: UCOM_api.h
#define UCOM_CREATE_COM_DEBUG      ( 0x00 ) /*data for debug port*//*same with 0x02*/
#define UCOM_CREATE_COM_DATA       ( 0x01 ) /*data for at port           */
#define UCOM_CREATE_COM_DEBUGDATA  ( 0x02 ) /*data for at and debug port */
#define UCOM_CREATE_COM_DATADISK   ( 0x03 ) /*data port + rom disk       */
#define UCOM_CREATE_MAX_TYPE       ( 0x04 ) 

PUBLIC int USB_Initialization(void);

PUBLIC void USB_VbusISR(BOOLEAN state);

PUBLIC int USB_GetNextService();

//PUBLIC BOOLEAN USB_StartService(int servicetype);

//PUBLIC BOOLEAN USB_StopService();

PUBLIC uint32 USB_GetServiceCount();
PUBLIC BOOLEAN USB_StartService(USB_SERVICE_E servicetype);

PUBLIC BOOLEAN USB_StopService(USB_SERVICE_E servicetype);

PUBLIC USB_SERVICE_E USB_GetCurService(void);

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Call this function to set current USB connect state ;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPM_SetConnectState (BOOLEAN state);
/*****************************************************************************/
//  Description:    Call this function to get current USB connect state ;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPM_GetConnectState (void);
/*****************************************************************************/
//  Description:    Call this function to get current service type;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
//PUBLIC UPM_SERVICETYPE_E UPM_GetService (void);
/*****************************************************************************/
//  Description:    Call this function to set current sevice type ;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
//PUBLIC BOOLEAN UPM_SetService (UPM_SERVICETYPE_E type);
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UPM_Init (void);
/*****************************************************************************/
//  Description:    Call this function to register protocol's handler to UPM;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
//BOOLEAN UPM_RegService (UPM_SERVICETYPE_E type,\
//                         const UPM_FUNCTION_PTR_T * handler_ptr);
/*****************************************************************************/
//  Description:    Call this function for sending commands to manager or 
//                  protoclos;
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
//PUBLIC BOOLEAN UPM_SendCommand (UPM_SERVICETYPE_E receiver,
 //                               uint32 command,
   //                             uint32 param,
  //                              void * ptr);

/*****************************************************************************/
//  Description: start the pc camera
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_OpenPCCamera(void);

/*****************************************************************************/
//  Description: stop the pc camera
//	Global resource dependence: 
//  Author: yuhua.shi
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UPCC_ClosePCCamera(void);

/*****************************************************************************/
//  Description: set Usb to COM
//	Global resource dependence: 
//  Author: liqing.peng
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_CreatForCOM(uint32 type);

PUBLIC BOOLEAN SIO_UserPortBusy(void);

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
