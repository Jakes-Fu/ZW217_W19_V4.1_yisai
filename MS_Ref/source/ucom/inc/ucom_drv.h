/******************************************************************************
 ** File Name:    UCOM_drv.h                                                    *
 ** Author:       weihua.wang                                                 *
 ** DATE:         3/25/2005                                                    *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 3/25/2005      weihua.wang     Create.                                     *
 ******************************************************************************/
#ifndef _UCOM_DRV_H_
#define _UCOM_DRV_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "ucom_common.h"
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

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
extern UCOM_FUNCTION_T 	g_ucom_functions [UCOM_MAX_FUNC_NUM];

extern uint8 g_ucom_device_desc[];
extern const uint8 g_UCOM_CfgDescDebug[];
extern const uint8 g_UCOM_CfgDescr_AutoDloader[];
extern const uint8 g_UCOM_CfgDescData[]; /*ep1,ep2 used*/
extern const uint8 g_UCOM_CfgDescr_twointerface[];
extern const uint8 g_UCOM_LanguageStr[];
extern const uint8 g_UCOM_SerialNumStr[];
extern const uint8 g_UCOM_StrManufacturer[];
extern const uint8 g_UCOM_StrProduct[];
extern const uint8 g_UCOM_APCP_DATA_MODEM_CfgDesc[];

#ifdef WINUSB_SUPPORT
extern const uint8 OS_StringDescriptor[18];
extern const uint8 WINUSB_ExtendedCompatId_Descritpor[64];
extern const uint8 WINUSB_ExtendedProperty_InterfaceGUID_Descritpor0[142];
extern const uint8 WINUSB_ExtendedProperty_InterfaceGUID_Descritpor1[142];
extern const uint8 g_UCOM_Device_Qualifier_Descr[10];

extern const uint8 g_UCOM_DeviceDescr_twointerfaceForWinUsb[];
extern const uint8 g_UCOM_DeviceDescr_threeinterfaceForWinUsb[];
extern const uint8 g_UCOM_CfgDescr_twointerfaceForWinUsb[];
extern const uint8 g_UCOM_CfgDescr_threeinterfaceForWinUsb[];
extern const uint8 g_UCOM20_CfgDescr_twointerfaceForWinUsb[];
extern const uint8 g_UCOM20_CfgDescr_threeinterfaceForWinUsb[];

#endif
/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Call this function to send data to special end point.
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:           pBuf  must be aligned  4  ;
//                  len   must be less than 64 ;
//                  ep_id must be less than 6  ;
/*****************************************************************************/
PUBLIC uint32 UCOM_EPxSendData  (uint8 ep_id ,uint32 * pBuf,uint32 len);

/*****************************************************************************/
//  Description:    usb get descriptor handler
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:           CTL:80 06 00 xx  00 00 xx 00
/*****************************************************************************/
PUBLIC void UCOM_GetDevDescriptor (uint16 req,uint16 wValue,\
                            uint16 wIndex ,uint16 wLen);

/*****************************************************************************/
//  Description:    handle vendor specified command
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC void UCOM_VendorCommand (uint16 req,uint16 wValue,
                                 uint16 wIndex ,uint16 wLen);

/*****************************************************************************/
//  Description:    handle class specified command
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC void UCOM_ClassCommand (uint16 req,uint16 wValue,
                                uint16 wIndex ,uint16 wLen);
/*****************************************************************************/
//  Description:    handle the standard configuration of set address
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC void UCOM_SetConfigure (uint16 req,uint16 wValue,uint16 wIndex ,uint16 wLen);

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC void DoNothing (uint16 req,uint16 wValue,uint16 wIndex ,uint16 wLen);

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:           param1: Request type
//                  param2: Request recipient
//                  param3: Specific request
//                  param4: Call back funcition
/*****************************************************************************/
PUBLIC BOOLEAN USB_RegCallback  (uint8 type, uint8 recipient,
                                	uint8 request,USB_FUNCTION callback );

/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:
/*****************************************************************************/
PUBLIC USB_FUNCTION USB_FindCallback  (uint8 type,			//Request type;
					uint8 recipient,			//Request recipient;
					uint8 request			//Specific request ;
					);

/*****************************************************************************/
//  Description:    write chars to inendp without interrupt
//	Global resource dependence:
//  Author:         weihua.wang
//	Note:           param1: endp number
//                  param2: write data buffer
//                  param3: write length
/*****************************************************************************/
PUBLIC void UCOM_WritetoEndp (uint8 ep_num,uint8 *write_buf,uint32 send_len);
extern const uint8 g_UCOM_UBOT_DeviceDescr[];
extern const uint8 g_UCOM_UBOT_CfgDescr[];
extern const uint8 g_UDEV_DeviceDescr_twointerface[];
extern const uint8 g_UDEV_CfgDescr_twointerface[];
extern const uint8 g_UCOM_CfgDescr_AutoDloader[];
/*****************************************************************************/
//  Description:
//	Global resource dependence:
//  Author:         Jack.Chen
//	Note:
/*****************************************************************************/

PUBLIC void VCOM_Open(BOOLEAN ready);


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
