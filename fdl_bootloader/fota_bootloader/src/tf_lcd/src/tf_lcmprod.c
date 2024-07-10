/******************************************************************************
 ** File Name:      lcm_prod.c                                                 *
 ** Author:         Liangwen.Zhen                                             *
 ** DATE:           01/15/2008                                                *
 ** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    This file defines the basic operation interfaces of LCM   *
 **					about product
 **                                                                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/15/2008     Liangwen.Zhen    Create.                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/ 


/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/


#include "fdl_bootloader_trc.h"
#include "os_api.h"
#include "tb_hal.h"
#include "dal_lcd.h"
#include "nv_productionparam_type.h"
#include "lcm_prod.h"
#include "lcm_cfg.h"
#include "lcd_cfg.h"

#define DEBUG_LCM_PROD
#ifdef  DEBUG_LCM_PROD
#define LCM_TRACE   TF_UartTrace 
#else
#define LCM_TRACE( _format_string )  
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Definition                                   *
 **---------------------------------------------------------------------------*/

#define LCM_ID_INVALID		0xFFFF
#define LCM_ID_DEFAULT  	0x00

#define LCM_CNT_MAX			0x02
#define LCM_READ_ADC_CNT	10

#define LCM_ID_REG          0x0

/**---------------------------------------------------------------------------*
 **                         Data Structure Definition                         *
 **---------------------------------------------------------------------------*/
typedef enum 
{
    LCM_NV_INDEX_START 	= 0x00,		// Select LCM according to the index in [0x00-0x7f]

    LCM_NV_INDEX_END 	= 0x7F,

    LCM_NV_DEV_ID		= 0xFC,		// Select LCM according to the lcd device id read 

    LCM_NV_DEV_ID_WB	= 0xFD,		// Select LCM according to the lcd device id read 
    // and write the index value to nv
    LCM_NV_ADC	  	 	= 0xFE,		// Select LCM according to LCM ADC value

    LCM_NV_VAL_MAX 	 	= 0xFF
}LCM_NV_VAL_E;		/*lint -esym(751, LCM_NV_VAL_E)*/


/**---------------------------------------------------------------------------*
 **                         Local Variables                                   *
 **---------------------------------------------------------------------------*/
LOCAL uint16 	s_lcm_index[LCM_CNT_MAX] ;

LOCAL BOOLEAN 	s_lcm_is_identify 		 = SCI_FALSE;

/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to delay. UNIT: ms    
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL void _LCM_Delay(uint32 ms)
{
    if (SCI_InThreadContext()) 
    {
        SCI_Sleep(ms);
    } 
    else 
    {
        uint32 end = SCI_GetTickCount() + ms;
        while (SCI_GetTickCount() < end){};		
    }
}


/*****************************************************************************/
//  Description:    This function is used to read lcd device id
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint32 _LCM_ReadID(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 lcm_dev_id;

#ifdef PRODUCT_DRIVER_MTV_ENABLE
    uint32 lcd_id1 = 0; 
    uint32 lcd_id2 = 0;    
    uint32 sim_smd_ptr = 0;
    uint32 DTL_ReadLCDID(uint32 wcmd_cd_status, uint32 lcdid_addr);

    //DTL_IdentifyLCDId(0,&lcd_id1,&lcd_id2,&sim_smd_ptr); /*lint !e64*/
    lcd_id1 =  DTL_ReadLCDID(lcd_cd,0);    /*lint !e64*/
    lcm_dev_id = lcd_id1 & 0xffff;

#else

    lcm_dev_id = LCD_ReadRegVal(lcd_id, LCM_ID_REG);
#endif	

    //LCM_TRACE:"_LCM_ReadID: lcd_id=%d ,lcm_dev_id = 0x%x!"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCMPROD_124_112_2_18_1_1_48_58,(uint8*)"dd",lcd_id,lcm_dev_id);

    return lcm_dev_id;
}

/*****************************************************************************/
//  Description:    This function is used to identiy LCM by lcd device id
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL uint16 _LCM_DevIdIdentify(uint32 lcd_id)
{
    LCM_CFG_INFO_T_PTR cfg_info_ptr = PNULL;
    uint32 			   count		= 0;
    uint32			   i			= 0;
    uint16			   index		= LCM_ID_INVALID;
    uint32             lcm_dev_id   = 0;

    cfg_info_ptr = LCM_GetCfgInfo( lcd_id, &count );

    if(!((PNULL == cfg_info_ptr) || (0 == count)) )
    {
        static LCD_SIMPLE_INIT_PARAM_T lcd_param = {0};

        lcd_param.lcd_id         = lcd_id;
        lcd_param.cs_id          = cfg_info_ptr[0].spec_info.cs_pin;
        lcd_param.cd_pol         = cfg_info_ptr[0].spec_info.cd_pin;
        lcd_param.interface_type = cfg_info_ptr[0].spec_info.spec_ptr->bus_mode;
        lcd_param.lcd_bits       = cfg_info_ptr[0].spec_info.spec_ptr->bus_width;
        lcd_param.timing         = *(LCD_TIMING_U*)(cfg_info_ptr[0].spec_info.spec_ptr->timing);
        lcd_param.controller     = cfg_info_ptr[0].spec_info.controller; //lcdc+lcm0,lcdc+lcm1,lcm0

#if defined(PLATFORM_SC6530) //|| defined(PLATFORM_UWS6121E)
        LCM_ResetLcd (0x20, 0);
        LCM_SetParam( &(cfg_info_ptr->spec_info), CHIP_GetAhbClk(),lcd_id);//    LCD_SetParam (CHIP_GetAhbClk());
        LCM_SetCSAddr (cfg_info_ptr->spec_info.cs_pin, cfg_info_ptr->spec_info.cd_pin, lcd_id);
#else
        LCD_SimpleInit(&lcd_param);
#endif		
        //lcm_dev_id = _LCM_ReadID(cfg_info_ptr[0].spec_info.cs_pin,cfg_info_ptr[0].spec_info.cd_pin,lcd_id);
        for(i = 0; i < count; i++)
        {
            if(cfg_info_ptr[i].spec_info.spec_ptr->operation->lcd_readid)
            {
                lcm_dev_id = cfg_info_ptr[i].spec_info.spec_ptr->operation->lcd_readid(cfg_info_ptr[i].spec_info.cs_pin,cfg_info_ptr[i].spec_info.cd_pin,lcd_id);
            }
            else
            {
                lcm_dev_id = _LCM_ReadID(cfg_info_ptr[i].spec_info.cs_pin,cfg_info_ptr[i].spec_info.cd_pin,lcd_id);
            }

            if(lcm_dev_id == cfg_info_ptr[i].dev_id)
            {
                index = i;
                break;
            }
        }		
    }
    return index;
}


/*****************************************************************************/
//  Description:    This function is used to check if LCM is already identify
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL BOOLEAN _LCM_IsIdentify(void)
{
    return s_lcm_is_identify;
}

/*****************************************************************************/
//  Description:    This function is used to identify LCD
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
LOCAL BOOLEAN _LCM_Identify(void)
{
    uint16 i = 0;

    if(_LCM_IsIdentify())
    {
        //LCM_TRACE:"_LCM_Identify: already done!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCMPROD_200_112_2_18_1_1_48_59,(uint8*)"");
        return s_lcm_is_identify;
    }

    for( i = 0; i < LCM_CNT_MAX; i++)
    {
        s_lcm_index[i] = _LCM_DevIdIdentify( i );

        if(LCM_ID_INVALID != s_lcm_index[i] )
        {
            //LCM_TRACE:"TF_LcmIdentify:s_lcm_index[i=0x%x] =0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCMPROD_210_112_2_18_1_1_48_60,(uint8*)"dd", i, s_lcm_index[i]);
        }
        else
        {
            s_lcm_index[i] = LCM_ID_INVALID;
            //LCM_TRACE:"TF_LcmIdentify: %d LCM used default config info"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCMPROD_215_112_2_18_1_1_48_61,(uint8*)"d", i);
        }
    }

    if(LCM_ID_INVALID == s_lcm_index[MAIN_LCD_ID])
    {
        s_lcm_index[MAIN_LCD_ID] = LCM_ID_DEFAULT;
    }

#ifndef SUBLCD_SIZE_NONE
    if(LCM_ID_INVALID == s_lcm_index[SUB_LCD_ID])
    {
        s_lcm_index[SUB_LCD_ID] = LCM_ID_DEFAULT;
    }
#endif	

    s_lcm_is_identify = SCI_TRUE;

    return s_lcm_is_identify;

}

/**---------------------------------------------------------------------------*
 **                     PUBLIC Function Prototypes                            *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    This function is used to get LCD spec information
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC LCD_SPEC_INFO_T_PTR LCM_GetLcdSpecInfo(uint32 lcd_id)
{
    LCM_CFG_INFO_T_PTR cfg_info_ptr = PNULL;
    uint32 			   count		= 0;		

    if(!_LCM_IsIdentify())
    {
        if(!_LCM_Identify())
        {
            //Identiy fail
            return PNULL;
        }
    }

    cfg_info_ptr = LCM_GetCfgInfo( lcd_id, &count );

    if((s_lcm_index[lcd_id] >= count) || (PNULL == cfg_info_ptr))
    {
        return PNULL;
    }

    return &cfg_info_ptr[s_lcm_index[lcd_id]].spec_info;
}

/*****************************************************************************/
//  Description:    This function is used to get LCD device ID
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC uint32 LCM_GetLcdDeviceID(uint32 lcd_id)
{
    LCM_CFG_INFO_T_PTR cfg_info_ptr = PNULL;
    uint32 			   count		= 0;

    if(!_LCM_IsIdentify())
    {
        if(!_LCM_Identify())
        {
            //Identiy fail
            return LCM_ID_INVALID;
        }
    }

    cfg_info_ptr = LCM_GetCfgInfo( lcd_id, &count );

    if((s_lcm_index[lcd_id] >= count) || (PNULL == cfg_info_ptr))
    {
        return LCM_ID_INVALID;
    }

    return cfg_info_ptr[s_lcm_index[lcd_id]].dev_id;
}
/*****************************************************************************/
//  Description:    This function is used to get lcd back light information
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC LCD_BACKLIGHT_INFO_T_PTR LCM_GetBackLightInfo(uint32 lcd_id)
{
    LCM_CFG_INFO_T_PTR cfg_info_ptr = PNULL;
    uint32 			   count		= 0;	

    if(!_LCM_IsIdentify())
    {
        if(!_LCM_Identify())
        {
            //Identiy fail
            return PNULL;
        }
    }

    cfg_info_ptr = LCM_GetCfgInfo( lcd_id, &count );

    if((s_lcm_index[lcd_id] >= count) || (PNULL == cfg_info_ptr))
    {
        return PNULL;
    }

    return (LCD_BACKLIGHT_INFO_T_PTR)&cfg_info_ptr[s_lcm_index[lcd_id]].bl_control;

}

/*****************************************************************************/
//  Description:    This function is used to get touch panel information
//  Author:         Liangwen.Zhen
//  Note:           
/*****************************************************************************/
PUBLIC TP_CFG_INFO_T_PTR LCM_GetTouchPanelInfo( void )
{	
    LCM_CFG_INFO_T_PTR cfg_info_ptr = PNULL;
    uint32 			   count		= 0;	

    if(!_LCM_IsIdentify())
    {
        if(!_LCM_Identify())
        {
            //Identiy fail
            return PNULL;
        }
    }

    cfg_info_ptr = LCM_GetCfgInfo( MAIN_LCD_ID, &count );

    SCI_ASSERT(PNULL != cfg_info_ptr);/*assert verified*/
    SCI_ASSERT(s_lcm_index[MAIN_LCD_ID] < count);/*assert verified*/

    return cfg_info_ptr[s_lcm_index[MAIN_LCD_ID]].tp_info_ptr;
}


/******************************************************************************/
// Description:   LCM_GetLcdDevSize
// Dependence:    NONE
// Author:        Haifeng.Yang
// Note:          Only support MAIN_LCD and SUB_LCD.
//                This function will return SCI_ERROR if the lcdid is not 
//                supported, otherwise it will return SCI_SUCCESS.
/******************************************************************************/
PUBLIC uint32 LCM_GetLcdDevSize(LCD_ID_E lcd_id, uint16* lcd_width, 
        uint16* lcd_height)
{
    uint32 ret = SCI_SUCCESS;

    switch(lcd_id)
    {
        case LCD_ID_0:
            *lcd_width = LCD_WIDTH;
            *lcd_height = LCD_HEIGHT;	
            break;

        case LCD_ID_1:
            *lcd_width = SUBLCD_WIDTH;
            *lcd_height = SUBLCD_HEIGHT;	
            break;

        default:
            *lcd_width = 0;
            *lcd_height = 0;	
            ret = SCI_ERROR;
            break;
    }

    return ret;
}


