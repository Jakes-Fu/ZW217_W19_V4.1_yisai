/******************************************************************************
 ** File Name:    UMOD_drv.h                                                  *
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
#ifndef _UMOD_DRV_H_
#define _UMOD_DRV_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */
#include "UMOD_common.h"
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
extern USB_FUNCTION g_umod_functions[USB_MAX_REQ_TYPE][USB_MAX_RECIPIENT][USB_MAX_REQUEST];



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
PUBLIC uint32 UMOD_EPxSendData  (uint8 ep_id ,uint32 * pBuf,uint8 len);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC uint32 UMOD_EPxReceiveData(uint8 ep_id ,uint32 * pBuf,uint16 len);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Jim.Lin
//	Note:           
/*****************************************************************************/
//PUBLIC uint32 UMOD_EPxReceiveData1  (uint8 ep_id ,uint32 * pBuf);


/*****************************************************************************/
//  Description:    usb endp0 register init
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_EP0Config  (void);

/*****************************************************************************/
//  Description:    DO with set ctrl line st
//	Global resource dependence: 
//  Author:         Jim.LIN
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_SET_CTRL_LINE_ST(void);

/*****************************************************************************/
//  Description:    DO with get line coding
//	Global resource dependence: 
//  Author:         Jim.LIN
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_GET_LINE_CODIING(void);

/*****************************************************************************/
//  Description:    DO with set buffer ready
//	Global resource dependence: 
//  Author:         Jim.LIN
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_SET_BUF_READY(void);

/*****************************************************************************/
//  Description:    DO with set line coding
//	Global resource dependence: 
//  Author:         Jim.LIN
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_SET_LINE_CODIING(void);


/*****************************************************************************/
//  Description:    usb endp1 register init
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_EP1Config  (void);

/*****************************************************************************/
//  Description:    usb endp2 register init
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_EP2Config  (void);

/*****************************************************************************/
//  Description:    usb endp3 register init
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UMOD_EP3Config  (void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_GetDevDescriptor (uint8 bDirection,uint16 wValue,\
                            uint16 wIndex ,uint16 wLen);



/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC void UMOD_DoNothing (uint8 bDirection,uint16 wValue,uint16 wIndex ,uint16 wLen);


/*****************************************************************************/
//  Description:    write chars to inendp without interrupt
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           param1: endp number
//                  param2: write data buffer 
//                  param3: write length
/*****************************************************************************/
PUBLIC void UMOD_WritetoEndp (uint8 ep_num,uint8 *write_buf,uint32 send_len);

//PUBLIC int UMOD_EPxSendData  (uint8 ep_id ,uint32 * pBuf,uint8 len);

					
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
