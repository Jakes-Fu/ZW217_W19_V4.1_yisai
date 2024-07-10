/******************************************************************************
 ** File Name:      lcm_drvapi.h                                             *
 ** Author:         Livy liu                                                 *
 ** DATE:           05/23/2005                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file contains driver for LCM controller.
 **
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 05/23/2005     livy.liu             Create.
 ** 06/13/2005     livy.liu             Modify.
 ******************************************************************************/

#ifndef _LCM_DRVAPI_H_
#define _LCM_DRVAPI_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Definitions
 **---------------------------------------------------------------------------*/
typedef struct _LCM_CMD_DATA_ADDR_tag
{
    uint32 cmd_addr;
    uint32 data_addr;
} LCM_ADDR_T;

typedef struct lcd_dma_info_tag
{
    uint32  data_width;
    uint32  width;
    uint32  height;
    uint32  buf_width;
    uint32  buf_height;
} LCD_DMA_INFO_T, *LCD_DMA_INFO_T_PTR;

typedef struct _lcm_size_tag
{
    uint32 width;
    uint32 height;
} LCM_SIZE_T;

#define LCM_WAIT_AHBFIFO_NOTFULL {while((*(volatile uint32 *)LCM_STATUS) & BIT_1){};}
#define LCM_WAIT_AHBFIFO_EMPTY   {while(!((*(volatile uint32 *)LCM_STATUS) & BIT_2)){};}

typedef void (* LCMCallBackFunc) (void);

/*****************************************************************************************/
//LCM INTERFACE DECLARE
/*****************************************************************************************/


/*****************************************************************************************/
// Description: initialize LCM
// Global resource dependence:
// Author: Jianping.wang
// Note:
/*****************************************************************************************/
PUBLIC void LCM_Init ( uint32 lcd_id );

/*****************************************************************************************/
// Description: LCM reset LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      delay_ms - during reset,need to add wait time
//      reset sequence:1-0-delay-1
/*****************************************************************************************/
PUBLIC void LCM_ResetLcd (uint32 delay_ms,uint8 rst_level);

/*****************************************************************************************/
// Description: send command and data to LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          cmd - lcd command
//          data - lcd command data
//          lcd_id - LCD id
//      output:
//          none
//      return:
//          LCD_OK,if success
//          other,if fail
/*****************************************************************************************/
PUBLIC ERR_LCD_E LCM_SendCmdData (uint32 cmd,uint32 data,uint32 lcd_id);

/*****************************************************************************************/
// Description: send command to LCD
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          cmd - LCD command
//          lcd_id - LCD ID
//      output:
//          none
//      return:
//          LCD_OK,if success
//          other,if fail
/*****************************************************************************************/
PUBLIC ERR_LCD_E LCM_SendCmd (uint32 cmd,uint32 lcd_id);

/*****************************************************************************************/
// Description: send data to LCD GRAM
// Global resource dependence:
// Author: Jianping.wang
// Note:
//      input:
//          data - display data
//          lcd_id - LCD ID
//      output:
//          none
//      return:
//          LCD_OK,if success
//          other, if fail
/*****************************************************************************************/

PUBLIC ERR_LCD_E LCM_SendData (uint32 data,uint32 lcd_id);


/*********************************************************************/
//  Description: LCM Read Data
//  Input:
//            None
//  Return:
//            None
//  Note:
//            LCM Controller read lcd gram data
/*********************************************************************/
PUBLIC uint32  LCM_ReadData (uint32 lcd_cs, uint32 cmd);


/*********************************************************************/
//  Description: input lcd parameter struct to LCM Controller
//  Input:    lcd_spec_ptr        lcm spec struct pointer
//  Return: 0           successful
//          others      failed
//  Note:
//            None
/*********************************************************************/
PUBLIC ERR_LCD_E LCM_RegLCDInfo (LCD_SPEC_INFO_T_PTR lcd_spec_ptr);

/*********************************************************************/
//  Description: change bus priority for LCM controller
//  Input:    priority    bus priority
//  Return: 0           successful
//          others      failed
//  Note:
//      If WriteBusShare bit is true, control module will share the bus
//     to manipulate the LCM during the busy phase in NAND both program and erase operation.
//     Otherwise, the control module will never active LCM during either program or erase operation.
//        31                            30                29    0
//        WriteBusShare     Reserved     Priority
//        1: share;
//        0: unshare
/*********************************************************************/
PUBLIC ERR_LCD_E LCM_SetBusRatio (uint32 ratio);


PUBLIC void LCM_SetCSAddr (uint16 cs_pin,uint16 cd_pin, uint16 lcd_id);


/*********************************************************************/
//  Description: Set LCM parameter for LCM Device0 selected by lcm_cs[0].
//  Input:    None
//  Return: 0           successful
//          others      failed
//  Note:

//    Mode Selection Register:
//    31     6    5         4          3           2        1         0
//    Reserved    ModeSel5 ModeSel4 ModeSel3 ModeSel2 ModeSel1 ModeSel0
//                0: 8080     0: 8080  0: 8080  0: 8080  0: 8080     0: 8080
//                1: 6800     1: 6800  1: 6800  1: 6800    1: 6800     1: 6800
/*********************************************************************/
PUBLIC ERR_LCD_E LCM_SetMode (LCD_SPEC_INFO_T_PTR lcd_spec_ptr);

/**********************************************************************************************************/
// Description:change pulse width
// Global resource dependence:
// Author: Jianping.wang
// Note:
//         input:
//            ahb_clk - AHB bus clock
//         return:
//          ERR_LCD_NONE,if success
//            other,if fail
/**********************************************************************************************************/
PUBLIC ERR_LCD_E LCM_SetParam (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 ahb_clk, uint32 lcd_id);

/*****************************************************************************/
//  Description:  set dest LCD's RGB mode to host
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC void LCM_SetRGBMode (uint32 cs_id,LCD_BUS_WIDTH_E rgb_mode);

/*****************************************************************************/
//  Description:  set the number of pixes in the next frame from OSD
//    Global resource dependence:
//  Author: Younger.Yang
//    Note:
/*****************************************************************************/
PUBLIC void LCM_SetDataNumber (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 number);

PUBLIC void LCM_SetDataNumberConfirm (void);

/**********************************************************************************************************/
// Description:    send data to LCD GRAM use DMA
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: younger
// Note:
/**********************************************************************************************************/
uint32 LCM_DMA_Write (uint32 *pBuf,uint32 cs_addr,uint32 len,LCD_DATA_SIZE_E data_size,LCD_BRUSH_MODE_E brush_mode);
/**********************************************************************************************************/
// Description:    LCM LINK BUF NODE INIT
// Global resource dependence:
// Author: alvin.han
// Note:
/**********************************************************************************************************/

PUBLIC void LCM_DMA_LinkInit (void);
/**********************************************************************************************************/
// Description:    send data to LCD GRAM use DMA
// Global resource dependence:  pBuf    cs_addr  lcd_dmainfo_ptr
// Author: alvin.han
// Note:
/**********************************************************************************************************/
uint32 LCM_DMA_LinkWrite (uint32 *pBuf,uint32 cs_addr, LCD_DMA_INFO_T *lcd_dmainfo_ptr);

/**********************************************************************************************************/
// Description:    NLC DMA Callback
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: younger
/**********************************************************************************************************/
void LCM_DMA_Callback (uint32 param);

/*********************************************************************/
//  Description:   Read LCM
//  Global resource dependence:
//  Author: Younger.Yang
//  Note: You may read LCD ID before lcd init,otherwise
//  the read data may be incorrect.
/*********************************************************************/
PUBLIC uint32 LCM_Read (uint32 lcd_id,uint32 cd_value);


PUBLIC uint32 LCM_GetDataAddr (uint32 cs_pin);

uint32 LCM_GetCurrentLcdid ( void );
LCD_SPEC_INFO_T_PTR LCM_GetCurrentSpecinfo (uint32 lcd_id);

/**********************************************************************************************************/
// Description: register handle function
// Global resource dependence: s_lcm_callbackfunc
// Author: Jianping.wang
// Note:
/**********************************************************************************************************/
PUBLIC BOOLEAN LCM_RegisterCallBack (uint32 int_num, LCMCallBackFunc func);

PUBLIC BOOLEAN LCM_16BitDataWrite (
    LCD_DIRECT_E dir_type,
    uint32 src_addr,
    uint32 des_addr,
    uint16 data_width,
    uint16 data_height,
    uint16 buf_startx,
    uint16 buf_width,
    LCD_BRUSH_MODE_E brush_mode
);

/**********************************************************************************************************/
//  Description:   LCM_DmaWrite
//  Author:        lichd
//  Note: Dependence 
//  [in]--bus_width,     ////only support 8-bit£¬16bit
//  [in]--dir_type,      //clockwise rotate_:0,90',,180', 270';mirror support also.
//  [in]--src_addr,      //
//  [in]--des_addr,      //
//  [in]--data_width,    //if (320*320)picture_RGB565,src_width=320,src_height=240   
//  [in]--data_height,   //  
//  [in]--buf_width,     // 
//  [in]--brush_mode,    //async or sync 
//  [out]--BOOLEAN
/**********************************************************************************************************/
PUBLIC uint32 LCM_DmaWrite (
    LCD_ID_E lcd_id,                 
    LCD_BUS_WIDTH_E  bus_width,      
    uint32 src_addr,
    uint32 src_width,               
    uint32 src_height,
    uint32 buf_width,
    LCD_DIRECT_E    rotation_angle,
    LCD_BRUSH_MODE_E brush_mode    
);

PUBLIC uint32 SPI_ValidRect(uint32 data_num, uint32 lcdid);
PUBLIC uint32 SPI_ValidRect_End (uint32 lcdid);
PUBLIC void LCM_GetSize (uint32 lcd_id, LCM_SIZE_T* lcm_size_ptr);
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _LCM_DRVAPI_H_
