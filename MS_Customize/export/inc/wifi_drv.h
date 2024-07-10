/******************************************************************************
 ** File Name:      wifi_drv.h                                                  *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           06/05/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of WIFI.   *
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/05/2010     Liangwen.zhen    Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 

#ifndef _WIFI_DRV_H_
#define _WIFI_DRV_H_

#include "os_api.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/
// return value
#define WIFI_RET_OK                  0x00
#define WIFI_RET_ERR                 0xff
#define WIFI_RET_PARAM_ERR           0x01
#define WIFI_RET_STATUS_ERR          0x02
#define WIFI_RET_HW_ERR              0x03
#define WIFI_RET_PORT_ERR            0x04
#define WIFI_RET_NV_ERR              0x04

// WIFI TX FACTOR
#define WIFI_NV_TX_FACTOR_LEVEL_CNT  0x5
#define WIFI_NV_ADDR_LEN             0x6
#define WIFI_NV_RESERVED_LEN         0x9
/**---------------------------------------------------------------------------*
 **                         Data Structure Definition                         *
 **---------------------------------------------------------------------------*/
typedef uint32 (* WIFI_IRQ_CALLBACK_FUNC)(uint32 param);
typedef void (*WIFI_CMD_PRINTF_FUNC)(const char *x_format, ...);
typedef void (*WIFI_CMD_CALLBACK_FUNC)(WIFI_CMD_PRINTF_FUNC cprintf,        int argc, char *argv[]);
typedef int32 (*WIFI_Asyn_Callback)(uint32* param);

typedef enum
{
    WIFI_PORT_TYPE_SPI  = 0x00,
    WIFI_PORT_TYPE_SDIO,

    WIFI_PORT_TYPE_MAX
}WIFI_PORT_TYPE_E;

typedef enum
{
    WIFI_STATUS_NOT_INIT = 0x00,
    WIFI_STATUS_OPEN,
    WIFI_STATUS_CLOSE,
    WIFI_STATUS_MAX
}WIFI_STATUS_E;

typedef struct
{
    uint8 address[WIFI_NV_ADDR_LEN];
}WIFI_ADDR_T;

typedef struct
{
    uint8 tx_factor[WIFI_NV_TX_FACTOR_LEVEL_CNT];
}WIFI_TX_FACTOR_T;

typedef struct
{
    WIFI_ADDR_T       addr;
    WIFI_TX_FACTOR_T  tx_factor;
    uint8 reserved[WIFI_NV_RESERVED_LEN];
}WIFI_NV_PARAM_T;
/**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to power on and open transmit port 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_Open(
                WIFI_PORT_TYPE_E port_type,
                WIFI_IRQ_CALLBACK_FUNC callback_func
                );


/*****************************************************************************/
//  Description:    This function is used to power down and close transmit port 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_Close( void );


/*****************************************************************************/
//  Description:    This function is used to read data from wifi chip 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_Read( uint8 * buf_ptr, uint32 lenght);


/*****************************************************************************/
//  Description:    This function is used to write data to wifi chip 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_Write( uint8 * buf_ptr, uint32 lenght);

/*****************************************************************************/
//  Description:    ReadWrite function that lets us do short write+read combo fast 
//  Author:         
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_ReadWrite( uint8 * buf_ptr_in, uint8 *buf_ptr_out, uint32 lenght);

/*****************************************************************************/
//  Description:    This function is used to get the status of wifi chip 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC WIFI_STATUS_E WIFI_GetStatus( void );

/*****************************************************************************/
//  Description:    This function is used to callback wifi irq  
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_IrqCallback(uint32 param);

/*****************************************************************************/
//  Description:    This function is used to get the nv parameter for fixed nv 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_GetNvParam(
                    WIFI_ADDR_T*      addr_ptr,
                    WIFI_TX_FACTOR_T* tx_factor_ptr
                    );


/*****************************************************************************/
//  Description:    This function is used to save nv parameter to fixed nv 
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 WIFI_SaveNvParam(
                    WIFI_ADDR_T*      addr_ptr,
                    WIFI_TX_FACTOR_T* tx_factor_ptr
                    );

/******************************************************************************
// Description:   The function is used to get or release the mutex of cs
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.02.14                                                *
// Note:
******************************************************************************/
uint32 WIFI_Ctl_Signal(BOOLEAN mutex);

/******************************************************************************
// Description:  The function is used to set the cs of spi value
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.02.14                                                *
// Note:
******************************************************************************/
PUBLIC uint32 WIFI_Ctl_Bus(BOOLEAN cs_select);

/******************************************************************************
// Description:
// Dependence: 
// Author:
// DATE:
// Note:
******************************************************************************/
PUBLIC uint32 WIFI_Ctl_Bus_No_Mutex(BOOLEAN cs_select);

/******************************************************************************
// Description:
// Dependence: 
// Author:
// DATE:
// Note:
******************************************************************************/
PUBLIC void WIFI_interrupt_enable (BOOLEAN is_enable);

/******************************************************************************
// Description:
// Dependence: 
// Author:
// DATE:
// Note:
******************************************************************************/
PUBLIC void WIFI_Set_Clock_Frequency(uint32 hz);

/*****************************************************************************/
//  Description:    This function is used to callback wifi irq  
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC void WIFI_RegCallback(WIFI_IRQ_CALLBACK_FUNC  callback_func);


/*****************************************************************************/
//  Description:
//  Author:
//  Note:           
/*****************************************************************************/
PUBLIC void WIFI_CmdCallbackReg(WIFI_CMD_CALLBACK_FUNC func);

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.10.10                                                *
// Note:
******************************************************************************/
uint32 WIFI_Read_Asyn(uint8* buf_ptr, uint32 lenght,WIFI_Asyn_Callback call_back);

/******************************************************************************
// Description: 
// Dependence: 
// Author:        Paul.Luo
// DATE:            2011.10.10                                                *
// Note:
******************************************************************************/
uint32 WIFI_Write_Asyn( uint8 * buf_ptr, uint32 lenght,WIFI_Asyn_Callback call_back);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
    
#endif  // End of wifi_drv.h

#endif  // _WIFI_DRV_H_

