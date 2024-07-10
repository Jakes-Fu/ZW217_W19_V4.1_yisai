/***************************************************************************************
** File Name:      lcd_if_hal.c                                                           *
** DATE:           01/19/2011                                                           *
** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.                    *
** Description:    this file defines LCM interfaces,                                   *
**                   these interfaces is visible for LCM's user                          *
****************************************************************************************

****************************************************************************************
**                         Edit History                                                  *
**-------------------------------------------------------------------------------------*
** DATE                    DESCRIPTION                                                  *
** 01/19/2010                Create.                                                       *
****************************************************************************************/

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                                  *
 **------------------------------------------------------------------------------------*/

#include "os_api.h"
#include "chip_plf_export.h"
#include "lcm_reg_v5.h"
#include "dal_lcd.h"
#include "lcm_drvapi.h"
#include "dma_drv_internal.h"
#include "ref_outport.h"
#include "lcd_cfg.h"

#include "lcm_cfg.h"
#include "lcd_hal_internal.h"

/**------------------------------------------------------------------------------------*
 **                        Compiler Flag                                                   *
 **------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                               Macro Define                                **
**----------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
**                               Data Prototypt                              **
**----------------------------------------------------------------------------*/
LCD_SPEC_INFO_T s_lcm_current_specinfo[LCD_SUPPORT_MAX];
LCM_ADDR_T  g_lcm_addr[CS_NUMBER]= {0, 0};
LCMCallBackFunc s_lcm_callbackfunc = NULL;  

//Only support the main&sub panel
LOCAL LCM_CONTROLLER_T s_lcm_controller[LCD_SUPPORT_MAX]={0};
LOCAL uint32 current_lcdid = MAIN_LCD_ID;

/**---------------------------------------------------------------------------*
**                               Function  Define                            **
**----------------------------------------------------------------------------*/

/******************************************************************************/
// Description:    init the lcm object and execute the _init();
// Global resource dependence:
// Author: lichd
// Note:
/******************************************************************************/
PUBLIC void LCM_Init ( uint32 lcd_id )
{
    //uint32 lcd_id = MAIN_LCD_ID;
    if( lcd_id == MAIN_LCD_ID )
	{
#ifdef MAINLCM_INTERFACE_LCM
	    {
	        s_lcm_controller[lcd_id]._init     = s_if_lcm._init;
	        s_lcm_controller[lcd_id]._reset    = s_if_lcm._reset;
	        s_lcm_controller[lcd_id]._setparam = s_if_lcm._setparam;
	        s_lcm_controller[lcd_id]._setaddr  = s_if_lcm._setaddr;
	        s_lcm_controller[lcd_id]._senddata = s_if_lcm._senddata;
	        s_lcm_controller[lcd_id]._sendcmd  = s_if_lcm._sendcmd;
	        s_lcm_controller[lcd_id]._dmawrite = s_if_lcm._dmawrite;
	        s_lcm_controller[lcd_id]._readid   = s_if_lcm._readid;
	    }
#endif
#ifdef MAINLCM_INTERFACE_SPI
	    {
	        s_lcm_controller[lcd_id]._init     = s_if_spi._init;
	        s_lcm_controller[lcd_id]._reset    = s_if_spi._reset;
	        s_lcm_controller[lcd_id]._setparam = s_if_spi._setparam;
	        s_lcm_controller[lcd_id]._setaddr  = s_if_spi._setaddr;    
	        s_lcm_controller[lcd_id]._senddata = s_if_spi._senddata;
	        s_lcm_controller[lcd_id]._sendcmd  = s_if_spi._sendcmd;
	        s_lcm_controller[lcd_id]._dmawrite = s_if_spi._dmawrite;
	        s_lcm_controller[lcd_id]._readid   = s_if_spi._readid;
	    }
#endif
#ifdef MAINLCM_INTERFACE_EMC
	    {
	        s_lcm_controller[lcd_id]._init     = s_if_emc._init;
	        s_lcm_controller[lcd_id]._reset    = s_if_emc._reset;
	        s_lcm_controller[lcd_id]._setparam = s_if_emc._setparam;
	        s_lcm_controller[lcd_id]._setaddr  = s_if_emc._setaddr;    
	        s_lcm_controller[lcd_id]._senddata = s_if_emc._senddata;
	        s_lcm_controller[lcd_id]._sendcmd  = s_if_emc._sendcmd;
	        s_lcm_controller[lcd_id]._dmawrite = s_if_emc._dmawrite;
	        s_lcm_controller[lcd_id]._readid   = s_if_emc._readid;
	    }
#endif
    }

    if( lcd_id == SUB_LCD_ID )
	{
#ifdef SUBLCM_INTERFACE_LCM
	    {
	        s_lcm_controller[lcd_id]._init     = s_if_lcm._init;
	        s_lcm_controller[lcd_id]._reset    = s_if_lcm._reset;
	        s_lcm_controller[lcd_id]._setparam = s_if_lcm._setparam;
	        s_lcm_controller[lcd_id]._setaddr  = s_if_lcm._setaddr;
	        s_lcm_controller[lcd_id]._senddata = s_if_lcm._senddata;
	        s_lcm_controller[lcd_id]._sendcmd  = s_if_lcm._sendcmd;
	        s_lcm_controller[lcd_id]._dmawrite = s_if_lcm._dmawrite;
	        s_lcm_controller[lcd_id]._readid   = s_if_lcm._readid;
	    }
#endif
#ifdef SUBLCM_INTERFACE_SPI
	    {
	        s_lcm_controller[lcd_id]._init     = s_if_spi._init;
	        s_lcm_controller[lcd_id]._reset    = s_if_spi._reset;
	        s_lcm_controller[lcd_id]._setparam = s_if_spi._setparam;
	        s_lcm_controller[lcd_id]._setaddr  = s_if_spi._setaddr;    
	        s_lcm_controller[lcd_id]._senddata = s_if_spi._senddata;
	        s_lcm_controller[lcd_id]._sendcmd  = s_if_spi._sendcmd;
	        s_lcm_controller[lcd_id]._dmawrite = s_if_spi._dmawrite;
	        s_lcm_controller[lcd_id]._readid   = s_if_spi._readid;
	    }
#endif
#ifdef SUBLCM_INTERFACE_EMC
	    {
	        s_lcm_controller[lcd_id]._init     = s_if_emc._init;
	        s_lcm_controller[lcd_id]._reset    = s_if_emc._reset;
	        s_lcm_controller[lcd_id]._setparam = s_if_emc._setparam;
	        s_lcm_controller[lcd_id]._setaddr  = s_if_emc._setaddr;    
	        s_lcm_controller[lcd_id]._senddata = s_if_emc._senddata;
	        s_lcm_controller[lcd_id]._sendcmd  = s_if_emc._sendcmd;
	        s_lcm_controller[lcd_id]._dmawrite = s_if_emc._dmawrite;
	        s_lcm_controller[lcd_id]._readid   = s_if_emc._readid;
	    }
#endif
    }
    // execute the _lcm_init()
    if( SCI_NULL != s_lcm_controller[lcd_id]._init )
    {
        s_lcm_controller[lcd_id]._init( lcd_id );
    }
                    
}

/******************************************************************************/
// Description:    LCM reset LCD
// Global resource dependence:
// Author: lichd 11.15
// Note:
//     reset sequence:1-0-delay-1
//       delay_ms - during reset,need to add wait time
/******************************************************************************/
PUBLIC void LCM_ResetLcd (uint32 delay_ms,uint8 rst_level)
{
    //do Panel reset
    if( SCI_NULL != s_lcm_controller[MAIN_LCD_ID]._reset )
        s_lcm_controller[MAIN_LCD_ID]._reset(delay_ms, rst_level);
}

/******************************************************************************/
// Description:configure the controller according the LCM timing spec
// Global resource dependence:
// Author: lichd 11.15
// Note:
//         input:
//            ahb_clk - AHB bus clock
//         return:
//            ERR_LCD_NONE,if success
//            other,if fail
//  Note: Not support this function, return  ERR_LCD_NONE derectly
/******************************************************************************/
PUBLIC ERR_LCD_E LCM_SetParam (LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 ahb_clk,uint32 lcd_id)
{
    /* record the current lcm connect info */
    s_lcm_current_specinfo[lcd_id] = *lcd_spec_ptr;
    
    if( SCI_NULL != s_lcm_controller[lcd_id]._setparam )
        s_lcm_controller[lcd_id]._setparam(lcd_spec_ptr, ahb_clk,lcd_id);
                    
    return ERR_LCD_NONE;
}

/******************************************************************************/
//  Description: Set data/cmd address according to cs&cd pin wrap
//  Author: lichd 09.03
//  Input:
//  Return: 0           successful
//          others      failed
//  Note:  you can use emc_mode or lcm_mode to control the lcm_panel.
//         pay attention to the pin_maped configuration.
/******************************************************************************/
PUBLIC void LCM_SetCSAddr (uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{  
    if( SCI_NULL != s_lcm_controller[lcd_id]._setaddr )
        // set cs address according to the cs&cd pin wrap
        s_lcm_controller[lcd_id]._setaddr(lcd_cs, lcd_cd, lcd_id);
}

uint32 LCM_GetDataAddr (uint32 lcd_id)
{
    return g_lcm_addr[lcd_id].data_addr;   
}

uint32 LCM_GetCmdAddr (uint32 lcd_id)
{
    return g_lcm_addr[lcd_id].cmd_addr;   
}

LCD_SPEC_INFO_T_PTR LCM_GetCurrentSpecinfo (uint32 lcd_id)
{
    return &(s_lcm_current_specinfo[lcd_id]);   
}

PUBLIC void LCM_GetSize (uint32 lcd_id, LCM_SIZE_T* lcm_size_ptr)
{
   lcm_size_ptr->width = LCD_WIDTH;
   lcm_size_ptr->height= LCD_HEIGHT;
}

/*
  Note: It should not be called befor LCM_DMA_DIR_Write().
        Because current_lcdid is recored in that function.
*/
uint32 LCM_GetCurrentLcdid ( void )
{
    return current_lcdid;   
}

/******************************************************************************/
// Description:    send command to LCD
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: lichd 11.13
// Note:
//         input:
//            cmd - LCD command
//            lcd_id - LCD cs
//         return:
//            ERR_LCD_NONE,if success
//            other,if fail
/******************************************************************************/
PUBLIC ERR_LCD_E LCM_SendCmd (uint32 cmd,uint32 lcd_id)
{
    if( SCI_NULL != s_lcm_controller[lcd_id]._sendcmd )
        s_lcm_controller[lcd_id]._sendcmd( cmd, lcd_id);

    return ERR_LCD_NONE;
}

/******************************************************************************/
// Description:    send data to LCD GRAM
// Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
// Author: lichd 11.13
// Note:
//         input:
//            data - display data
//            lcd_id - LCD cs
//         return:
//            ERR_LCD_NONE,if success
//            other, if fail
/******************************************************************************/
PUBLIC ERR_LCD_E LCM_SendData (uint32 data,uint32 lcd_id)
{
    if( SCI_NULL != s_lcm_controller[lcd_id]._senddata )
        s_lcm_controller[lcd_id]._senddata( data, lcd_id);
                    
    return ERR_LCD_NONE;
}

/******************************************************************************/
// Description:    send command with data to LCD
// Global resource dependence:
// Author: lichd 11.13
// Note:
//         input:
//             cmd - lcd command
//            data - lcd command data
//            lcd_id - LCD cs
//         return:
/******************************************************************************/
PUBLIC ERR_LCD_E LCM_SendCmdData (uint32 cmd,uint32 data,uint32 lcd_id)
{
    LCM_SendCmd(cmd, lcd_id);
    LCM_SendData(data,lcd_id);

    return ERR_LCD_NONE;
}

/******************************************************************************/
// Description: register handle function
// Global resource dependence: s_lcm_callbackfunc
// Author: Fei.zhang
// Note:
/******************************************************************************/
PUBLIC BOOLEAN LCM_RegisterCallBack (uint32 int_num, LCMCallBackFunc func)
{
    SCI_ASSERT (PNULL != func);/*assert verified*/

    s_lcm_callbackfunc = func;

    return SCI_TRUE;

}
/******************************************************************************/
//  Description:   LCM_DMA_DIR_Write for parallel lcm
//  Author:        lichd
//  Note: Dependence DMA_hal.h for SC6610
//  [in]--lcd_id,        //
//  [in]--src_addr,      //
//  [in]--data_width,    //   
//  [in]--data_height,   //  
//  [in]--buf_width,     //
//  [in]--dir_type,      //clockwise rotate_:0£¬90',£¬180'£¬ 270'£»mirror support also.
//  [in]--brush_mode,    //async or sync 
//  [out]--BOOLEAN
/******************************************************************************/
PUBLIC uint32 LCM_DmaWrite (
    LCD_ID_E lcd_id,                 
    LCD_BUS_WIDTH_E  bus_width,      
    uint32 src_addr,
    uint32 data_width,               
    uint32 data_height,
    uint32 buf_width,
    LCD_DIRECT_E    dir_type,
    LCD_BRUSH_MODE_E brush_mode    
)
{
    //LCD_BUS_WIDTH_T bus_width = s_lcd_spec_info_ptr[lcd_id]->spec_ptr->bus_width;
    uint32          des_addr  = LCM_GetDataAddr (lcd_id);

	current_lcdid = lcd_id;
		
    if(SCI_NULL != s_lcm_controller[lcd_id]._dmawrite)
    s_lcm_controller[lcd_id]._dmawrite(
                                            bus_width,
                                            src_addr,
                                            des_addr,
                                            data_width,
                                            data_height,
                                            buf_width,
                                            dir_type,                                            
                                            brush_mode
                                       );
    return ERR_LCD_NONE;
}

/*********************************************************************/
//  Description:   Read LCM ID
//  Global resource dependence:
//  Author: lichd
//  Note: Only suport read id after lcm init success
/*********************************************************************/
PUBLIC uint32 LCM_Read (uint32 lcd_id, uint32 cd_level)
{ 
    if( SCI_NULL != s_lcm_controller[lcd_id]._readid )
    {
        return s_lcm_controller[lcd_id]._readid( lcd_id, cd_level );
    }
    return 0x00;
}

#ifdef   __cplusplus
}
#endif
