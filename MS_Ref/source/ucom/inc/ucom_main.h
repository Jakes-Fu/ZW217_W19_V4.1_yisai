/******************************************************************************
 ** File Name:    UCOM_main.h                                                 *
 ** Author:       weihua.wang                                               *
 ** DATE:         2005-11-8                                                    *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 2005-11-8     weihua.wang     Create.                                    *
 ******************************************************************************/
#ifndef _UCOM_MAIN_H_
#define _UCOM_MAIN_H_
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
**                             Struct Define                                 **
**----------------------------------------------------------------------------*/
typedef struct _UCOM_PortTbl {
    volatile uint8 port_num;
    volatile uint8 rx_endp;
    volatile uint8 tx_endp;
}UCOM_PortTbl_T,*PUCOM_PortTbl_P;

typedef enum
{
    UCOM_WORK_AUTO_DWLD,    // auto download
    UCOM_WORK_WITH_UBOT,    // data and modem
    UCOM_WORK_COM_DATA,     // data port
    UCOM_WORK_COM_DEBUG,    // debug port
    UCOM_WORK_DATA_DEBUG,   // data and debug
    UCOM_WORK_DEBUG_DATA_MODEM,  // ap cp debug, data and ppp, 4 interface
    UCOM_WORK_DEBUG_DATA_UMS,  // at +diag+udisk
} ucomWorkMode_t;

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_init (void);

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_exit (void);
/*****************************************************************************/
//  Description:    get devptr according to port_num
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC UCOM_DEVICE_T * UCOM_GetDevfromPort(uint8 port_num);

/*****************************************************************************/
//  Description:    get dev point index from endp
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           called only in ucom project(internal)
/*****************************************************************************/
PUBLIC UCOM_DEVICE_T * UCOM_getDevfromEndp(uint8 endp_num);

/*****************************************************************************/
//  Description:    get assert mode 
//	Global resource dependence: 
//  Author:         weihua.wang
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UCOM_IsInAssert(void);

/*****************************************************************************/
//  Description:   get vcom mode as modem + ubot, to judge the device and 
//                  configure descriptor type for one interface
//	Global resource dependence: 
//  Author:        
//	Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UCOM_GetModemUbotEnable(void);

/*****************************************************************************/
//  Description:   set vcom mode as modem + ubot, to judge the device and 
//                  configure descriptor type for one interface
//	Global resource dependence: 
//  Author:         
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_SetModemUbotEnable(BOOLEAN b_enable);

/*@CR154954  begin modified by xinwei.zhang 2009-09-29*/
/*****************************************************************************/
//  Description:    when assert happen,init usb virtual com first
//	Global resource dependence: 
//  Author:         weihua.wang
//  Param   :       uint32 port_num: usb virtual com number   
//	Note:           to clear all usb interrtup sts,and init usb buffer,if current
//                  usb ldo is off, then build up usb connection 
/*****************************************************************************/
PUBLIC void UCOM_AssertInit(uint32 port_num);
/*@CR154954  end modified by xinwei.zhang 2009-09-29*/

PUBLIC ucomWorkMode_t UCOM_GetWorkMode();

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 
