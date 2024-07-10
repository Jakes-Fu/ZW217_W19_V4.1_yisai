/***************************************************************************************
** File Name:      lcdc_drv.c                                                          *
** DATE:           06/01/2009                                                          *
** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:                                                                        *
****************************************************************************************/

/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/01/2009    Tim.zhu   Create.                                           *
 ******************************************************************************/

/**------------------------------------------------------------------------------------*
 **                         Dependencies                                               *
 **------------------------------------------------------------------------------------*/

/**------------------------------------------------------------------------------------*
 **                         Compiler Flag                                              *
 **------------------------------------------------------------------------------------*/
#include "fdl_bootloader_trc.h"
#ifdef   __cplusplus
extern   "C"
{
#endif
/*lint -save -e728*/
/*lint -save -e808 -e26 -e63 -e49 -e502 -e601*/

#include "lcdc_drv.h"
#include "sc_reg.h"
#include "tb_isr.h"
#include "isr_drvapi.h "
#include "ref_outport.h"

/**------------------------------------------------------------------------------------*
 **                         Macro define                                               *
 **------------------------------------------------------------------------------------*/
#define LCDC_CLUT4_SHIFT        0x04
#define LCDC_IRQ_NUM            4
#define LCDC_INT_MASK_VALUE     0x0000000F
#ifdef PLATFORM_SC6800H
#define REG32(x)                (*((volatile uint32 *)(x)))
extern PUBLIC void LCM_SetDataNumberConfirm (void);
#endif
/**------------------------------------------------------------------------------------*
 **                         Local varaibles                                            *
 **------------------------------------------------------------------------------------*/
LOCAL LCD_IRQ_FUNC          s_lcdc_irq[LCDC_IRQ_NUM]    = {PNULL};
LOCAL LCDC_IRQ_STATUS_U     s_record_lcdc_int_status = 0;
LOCAL BOOLEAN               s_is_register = FALSE;
/**------------------------------------------------------------------------------------*
 **                         Public functions                                           *
 **------------------------------------------------------------------------------------*/


/********************************************************************************************/
//Description: save status of INT
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL void _DrvSaveIntStatus (void)
{
    LCDC_IRQ_STATUS_U  *status_ptr = (LCDC_IRQ_STATUS_U *) REG_LCDC_IRQ_STATUS;

    s_record_lcdc_int_status = *status_ptr;
    
}
/********************************************************************************************/
//Description:clear save status of INT
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL void _DrvClearSaveIntStatus (void)
{   
    s_record_lcdc_int_status.dwValue = 0;    
}
/********************************************************************************************/
//Description: get status of INT
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL BOOLEAN _GetIntStatus (LCD_INT_TYPE_E irq_num,LCDC_IRQ_STATUS_U *status_ptr)
{   
    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("_GetIntStatus: The interrupt num is invalid %d", irq_num));/*assert verified*/

    if (status_ptr->dwValue & (LCDC_ONE<<irq_num))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}
/********************************************************************************************/
//Description: open interrupt of LCDC
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL void _OpenInt(void)
{
    #ifdef PLATFORM_SC6800H       
    TB_EnableINT (TB_LCD_INT);
	#else		
    TB_EnableINT(TB_LCDC_INT);
	#endif
}
/********************************************************************************************/
//Description: close interrupt of LCDC
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL void _CloseInt(void)
{
    #ifdef PLATFORM_SC6800H       
    TB_DisableINT (TB_LCD_INT);
	#else		
    TB_DisableINT(TB_LCDC_INT);
	#endif
}
/********************************************************************************************/
//Description: lcdc module high irq handle func
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL void LCDC_DrvHisr(uint32 count, void *data)
{
    uint32 i = 0;   

    for (i=0; i<LCD_INT_MAX; i++)
    {
        if (_GetIntStatus ( (LCD_INT_TYPE_E) i,(LCDC_IRQ_STATUS_U*)data))       
        {
            if (PNULL != s_lcdc_irq[i])
            {
                s_lcdc_irq[i]();
            }
        }
    }
    _DrvClearSaveIntStatus();  
    _OpenInt();
}
/********************************************************************************************/
//Description: lcdc module low irq handle func
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL ISR_EXE_T LCDC_DrvLisr (uint32 param)
{
    uint32 i = 0;
    _CloseInt();
    
    _DrvSaveIntStatus();

    for (i=0; i<LCD_INT_MAX; i++)
    {
        if (LCDC_DrvGetIntStatus ( (LCD_INT_TYPE_E) i))
        {            
            LCDC_DrvCleanInt ( (LCD_INT_TYPE_E) i);
        }
    }   
    return CALL_HISR;
}
#if 0
/********************************************************************************************/
//Description: register lcdc module irq handle func
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC void LCDC_DrvIrq (void)
{
    uint32 i = 0;

    for (i=0; i<LCD_INT_MAX; i++)
    {
        if (LCDC_DrvGetIntStatus ( (LCD_INT_TYPE_E) i))
        {
            if (PNULL != s_lcdc_irq[i])
            {
                s_lcdc_irq[i]();
            }

            LCDC_DrvCleanInt ( (LCD_INT_TYPE_E) i);
        }
    }
}
#endif
/********************************************************************************************/
//Description: wait the interrupt
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC BOOLEAN LCDC_DrvWaitIntDone (LCD_INT_TYPE_E irq_num, uint32 wait_time)
{
    uint32      time_cnt = 0;
    BOOLEAN     done    = FALSE;
    LCDC_IRQ_STATUS_U *reg_ptr = (LCDC_IRQ_STATUS_U *) REG_LCDC_IRQ_RAW;

    while (time_cnt ++  < wait_time)
    {
        if (reg_ptr->dwValue & (1<<irq_num))
        {
            LCDC_DrvCleanRawState (irq_num);
            done = TRUE;
            break;
        }

        SCI_Sleep (10);
    }

    if (!done)
    { 
	    #ifdef PLATFORM_SC6800H
        SCI_TRACE_LOW ("LCDC: [0x20600100] = 0x%08X\r\n", REG32 (0x20600100));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20600104] = 0x%08X\r\n", REG32 (0x20600104));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20600108] = 0x%08X\r\n", REG32 (0x20600108));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x2060010c] = 0x%08X\r\n", REG32 (0x2060010c));/*lint !e718*/

        SCI_TRACE_LOW ("LCDC: [0x60001c50] = 0x%08X\r\n", REG32 (0x60001c50));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x60001c54] = 0x%08X\r\n", REG32 (0x60001c54));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x60001c58] = 0x%08X\r\n", REG32 (0x60001c58));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x60001c5c] = 0x%08X\r\n", REG32 (0x60001c5c));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x60001c68] = 0x%08X\r\n", REG32 (0x60001c68));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x60001400] = 0x%08X\r\n", REG32 (0x60001400));/*lint !e718*/
    	#else
        SCI_TRACE_LOW ("LCDC: [0x20700004] = 0x%08X\r\n", REG32 (0x20700004));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20700008] = 0x%08X\r\n", REG32 (0x20700008));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x2070000C] = 0x%08X\r\n", REG32 (0x2070000C));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20700020] = 0x%08X\r\n", REG32 (0x20700020));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x2070002C] = 0x%08X\r\n", REG32 (0x2070002C));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20700030] = 0x%08X\r\n", REG32 (0x20700030));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20700034] = 0x%08X\r\n", REG32 (0x20700034));/*lint !e718*/
		SCI_TRACE_LOW ("LCDC: [0x20700120] = 0x%08X\r\n", REG32 (0x20700120));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20700128] = 0x%08X\r\n", REG32 (0x20700128));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x2070012C] = 0x%08X\r\n", REG32 (0x2070012C));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20700158] = 0x%08X\r\n", REG32 (0x20700158));/*lint !e718*/
		#endif
	}

    return done;
}
/********************************************************************************************/
//Description: close lcdc irq
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvCloseInt (LCD_INT_TYPE_E irq_num)
{
    LCDC_IRQ_EN_U  *reg_ptr = (LCDC_IRQ_EN_U *) REG_LCDC_IRQ_EN;

    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("ISP_VSP_DRV: The interrupt num is invalid %d", irq_num));/*assert verified*/

    reg_ptr->dwValue&=~ (LCDC_ONE<< (uint32) irq_num);
    s_lcdc_irq[ (uint32) irq_num] = NULL;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: open lcdc irq
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvOpenInt (LCD_INT_TYPE_E irq_num, LCD_IRQ_FUNC irq_func)
{
    LCDC_IRQ_EN_U   *reg_ptr       = (LCDC_IRQ_EN_U *) REG_LCDC_IRQ_EN;
    LCDC_IRQ_CLR_U  *reg_clear_ptr = (LCDC_IRQ_CLR_U *)REG_LCDC_IRQ_CLR;

    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("ISP_VSP_DRV: The interrupt num is invalid %d", irq_num));/*assert verified*/
    reg_clear_ptr->dwValue|= (1<< (uint32) irq_num);
    
    s_lcdc_irq[ (uint32) irq_num] = irq_func;
    reg_ptr->dwValue|= (LCDC_ONE<< (uint32) irq_num);
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: clean lcdc irq
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvCleanInt (LCD_INT_TYPE_E irq_num)
{
    LCDC_IRQ_CLR_U  *reg_ptr = (LCDC_IRQ_CLR_U *) REG_LCDC_IRQ_CLR;
    LCDC_IRQ_CLR_U  *reg_state_ptr = (LCDC_IRQ_CLR_U *) REG_LCDC_IRQ_STATUS;

    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("ISP_VSP_DRV: The interrupt num is invalid %d", irq_num));/*assert verified*/

    if (reg_state_ptr->dwValue & (1<< (uint32) irq_num))
    {
        reg_ptr->dwValue|= (1<< (uint32) irq_num);
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: get status of INT
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC BOOLEAN LCDC_DrvGetIntStatus (LCD_INT_TYPE_E irq_num)
{
    LCDC_IRQ_STATUS_U  *status_ptr = (LCDC_IRQ_STATUS_U *) REG_LCDC_IRQ_STATUS;

    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("ISP_VSP_DRV: The interrupt num is invalid %d", irq_num));/*assert verified*/

    if (status_ptr->dwValue & (LCDC_ONE<<irq_num))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}
/********************************************************************************************/
//Description: top lcdc reset
//Global resource dependence:
//Author: Shan.he
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvReset (void)
{
    uint32              reg_value           = 0;

    //enable AHB_CTL0 lcdc enable first
    reg_value = REG32 (AHB_CTL0);
	#ifdef PLATFORM_SC6800H
    reg_value |= BIT_5;
	#else
	reg_value |= BIT_3;
	#endif
    REG32 (AHB_CTL0) = reg_value;

    // AHB_CTL0 lcdc soft reset, all lcdc registers are set to 0
    reg_value = REG32 (AHB_SOFT_RST);
	#ifdef PLATFORM_SC6800H
    reg_value |= BIT_5;
	#else
	reg_value |= BIT_3;
	#endif
    REG32 (AHB_SOFT_RST) = reg_value;

    SCI_Sleep (10);

    // clear AHB_CTL0 lcdc soft reset
    reg_value = REG32 (AHB_SOFT_RST);
	#ifdef PLATFORM_SC6800H
    reg_value &= ~BIT_5;
	#else
	reg_value &= ~BIT_3;
	#endif
    REG32 (AHB_SOFT_RST) = reg_value;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: lcdc enable
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvEnable (BOOLEAN enable)
{
    LCDC_CTRL_U *reg_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;
    uint32 reg_value = 0;

    if (enable)
    { 
	    #ifdef PLATFORM_SC6800H
        //enable AHB_CTL0 lcdc enable
        reg_value = REG32 (AHB_CTL0);
        reg_value |= BIT_5;
        REG32 (AHB_CTL0) = reg_value;
        reg_ptr->mBits.lcd_enable=LCDC_ONE;

        //set 64MHz
        reg_value = REG32(AHB_CLK_CFG1);
        reg_value |= 0x3;
        REG32(AHB_CLK_CFG1) = reg_value;

        //enable LCDC INT of top module  
        if(!s_is_register)
        {
  //          if(TB_SUCCESS != ISR_RegHandler_Ex(TB_LCD_INT, \
  //                                              LCDC_DrvLisr, \
  //                                              LCDC_DrvHisr, \
  //                                              CHIPDRV_HISR_PRIO_2, \
  //                                              (void*)&s_record_lcdc_int_status))
            {
                SCI_PASSERT(0,("Register Interrupt of the LCDC failed!"));/*assert verified*/
            }
            s_is_register = TRUE;
        }
        TB_EnableINT (TB_LCD_INT);
		#else
		//enable AHB_CTL0 lcdc enable
        reg_value = REG32 (AHB_CTL0);
        reg_value |= BIT_3;
        REG32 (AHB_CTL0) = reg_value;
        reg_ptr->mBits.lcd_enable=LCDC_ONE;

        //set 96MHz
        reg_value = REG32(GR_GEN4);
        reg_value &= 0xfffffff8;
        REG32(GR_GEN4) = reg_value;

        reg_value = REG32(GR_PLL_SCR);
        reg_value &= 0xffffff3f;
        REG32(GR_PLL_SCR) = reg_value;
        
        //enable LCDC INT of top module	
        if(!s_is_register)
        {
//    		if(TB_SUCCESS != ISR_RegHandler_Ex(TB_LCDC_INT, \
//    		                                    LCDC_DrvLisr, \
//    		                                    LCDC_DrvHisr, \
//    		                                    CHIPDRV_HISR_PRIO_2, \
//    		                                    (void*)&s_record_lcdc_int_status))
    		{
                SCI_PASSERT(0,("Register Interrupt of the LCDC failed!"));/*assert verified*/
    		}
            s_is_register = TRUE;
        }
	    TB_EnableINT(TB_LCDC_INT);
		#endif
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: lcdc start
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
//extern uint32                s_lcdc_run_time;
PUBLIC uint32 LCDC_DrvRun (void)
{
    LCDC_CTRL_U *reg_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;

	SCI_DisableIRQ();
//  s_lcdc_run_time = SCI_GetTickCount();	
#ifdef PLATFORM_SC6800H
	LCM_SetDataNumberConfirm();
#endif
  reg_ptr->mBits.lcdc_run=LCDC_ONE;
	SCI_RestoreIRQ();

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcd frame mark, mode: 0, fmark device; 1: non-fmark device, 
//              pol: 0, fmark valid at 0,1, fmark valid at 1
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetFmark (LCD_FMARK_MODE_E mode, LCD_FMARK_POL_E pol)
{
    LCDC_CTRL_U *reg_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;

    reg_ptr->mBits.fmark_mode = (LCD_FMARK_DEVICE == mode) ? 0 : 1;
    reg_ptr->mBits.fmark_pol  = (LCD_FMARK_HIGH == pol) ? 0 : 1;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: lcdc set back ground param
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetBGColor (uint32 bg_color)
{
    LCDC_BG_COLOR_U *reg_ptr = (LCDC_BG_COLOR_U *) REG_LCDC_BG_COLOR;

    reg_ptr->dwValue= (bg_color&0xffffff);

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: lcdc set back ground param
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetDitherEnable (BOOLEAN is_dither)
{
    LCDC_CTRL_U *reg_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;

    reg_ptr->mBits.dither_en=is_dither;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set ahb master writ data addr
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAhbMasterAddr (LCD_LAYER_ID_E layer_id, 
                                                  uint32 y_addr, 
                                                  uint32 uv_addr)
{
    IMG_Y_BASE_ADDR_U   *y_addr_ptr     = (IMG_Y_BASE_ADDR_U *) REG_IMG_Y_BASE_ADDR;
    IMG_UV_BASE_ADDR_U  *uv_addr_ptr    = (IMG_UV_BASE_ADDR_U *) REG_IMG_UV_BASE_ADDR;
    OSD_BASE_ADDR_U     *osd_addr_ptr   = NULL;
#ifdef PLATFORM_SC6800H
    uint32              y_base_addr  = ( (y_addr>>0x02)&0x00ffffff);
    uint32              uv_base_addr = ( (uv_addr>>0x02)&0x00ffffff);
#else 
	uint32 				y_base_addr	 = ((y_addr>>0x02)&0x3fffffff);
    uint32 				uv_base_addr = ((uv_addr>>0x02)&0x3fffffff);
#endif
    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            y_addr_ptr->dwValue=y_base_addr;
            uv_addr_ptr->dwValue=uv_base_addr;
            break;

        case LCD_LAYER_OSD1:
            osd_addr_ptr= (OSD_BASE_ADDR_U *) REG_OSD1_BASE_ADDR;
            osd_addr_ptr->dwValue=y_base_addr;
            break;

        case LCD_LAYER_OSD2:
            osd_addr_ptr= (OSD_BASE_ADDR_U *) REG_OSD2_BASE_ADDR;
            osd_addr_ptr->dwValue=y_base_addr;
            break;

        case LCD_LAYER_OSD3:
            osd_addr_ptr= (OSD_BASE_ADDR_U *) REG_OSD3_BASE_ADDR;
            osd_addr_ptr->dwValue=y_base_addr;
            break;
#ifdef PLATFORM_SC8800G		
        case LCD_LAYER_OSD4:			
            osd_addr_ptr=(OSD_BASE_ADDR_U*)REG_OSD4_BASE_ADDR;
    		osd_addr_ptr->dwValue=y_base_addr;
            break;
        case LCD_LAYER_OSD5:			
            osd_addr_ptr=(OSD_BASE_ADDR_U*)REG_OSD5_BASE_ADDR;
    		osd_addr_ptr->dwValue=y_base_addr;
            break;
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_470_112_2_18_1_1_42_29,(uint8*)"dd", layer_id, y_addr);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set ahb master writ data addr
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAlphaBaseAddr (LCD_LAYER_ID_E layer_id, uint32 base_addr)
{
    OSD_ALPHA_BASE_ADDR_U *alpha_addr_ptr=NULL;
#ifdef PLATFORM_SC6800H
    uint32 alpha_base_addr = ((base_addr>>0x02)&0x00ffffff);
#else
	uint32 alpha_base_addr = ((base_addr>>0x02)&0x3fffffff);
#endif	

    switch (layer_id)
    {
        case LCD_LAYER_OSD1:
            alpha_addr_ptr= (OSD_ALPHA_BASE_ADDR_U *) REG_OSD1_ALPHA_BASE_ADDR;
            break;
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_494_112_2_18_1_1_42_30,(uint8*)"dd", layer_id, base_addr);
            break;
    }
    alpha_addr_ptr->dwValue=alpha_base_addr;

    return SCI_SUCCESS;
}
/****************************************************************************************/
//Description: set lcdc layer data endian
//Global resource dependence:
//Author: Tim.zhu
/*****************************************************************************************/
PUBLIC uint32 LCDC_DrvSetEndian (LCD_LAYER_ID_E layer_id, uint32 y_endian_switch, uint32 uv_endian_switch)
{
    IMG_CTRL_U *img_ctrl_ptr= (IMG_CTRL_U *) REG_IMG_CTRL;
    OSD_CTRL_U *osd_ctrl_ptr = NULL;
    OSD1_CTRL_U *osd1_ctrl_ptr = NULL;
    uint32    endian_switch =   y_endian_switch;
    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
#ifdef PLATFORM_SC6800H
			img_ctrl_ptr->mBits.img_endian=endian_switch;
#else
			img_ctrl_ptr->mBits.img_y_endian=endian_switch;		
			img_ctrl_ptr->mBits.img_uv_endian=endian_switch;	
#endif            
            break;
        case LCD_LAYER_OSD1:
#ifdef PLATFORM_SC6800H
            osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD1_CTRL;
            osd_ctrl_ptr->mBits.blk_endian=endian_switch;
#else
        	osd1_ctrl_ptr=(OSD1_CTRL_U*)REG_OSD1_CTRL;
        	osd1_ctrl_ptr->mBits.blk_endian=endian_switch;
	 		osd1_ctrl_ptr->mBits.alpha_endian=endian_switch;
#endif
            break;			
		case LCD_LAYER_OSD2:	
			osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD2_CTRL;
			osd_ctrl_ptr->mBits.blk_endian=endian_switch;
			break;
        case LCD_LAYER_OSD3:
            osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD3_CTRL;
            osd_ctrl_ptr->mBits.blk_endian=endian_switch;
            break;
#ifdef PLATFORM_SC8800G
        case LCD_LAYER_OSD4:			
            osd_ctrl_ptr=(OSD_CTRL_U*)REG_OSD4_CTRL;
            osd_ctrl_ptr->mBits.blk_endian=endian_switch;
        break;
        case LCD_LAYER_OSD5:			
            osd_ctrl_ptr=(OSD_CTRL_U*)REG_OSD5_CTRL;
            osd_ctrl_ptr->mBits.blk_endian=endian_switch;
        break;
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetEndian ERROR],layer %d, endian %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_549_112_2_18_1_1_42_31,(uint8*)"dd", layer_id, endian_switch);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc layer data format
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetDataFormat (LCD_LAYER_ID_E layer_id,
                                              LCD_DATA_FORMAT_E data_format)
{
    IMG_CTRL_U *img_ctrl_ptr= (IMG_CTRL_U *) REG_IMG_CTRL;
    OSD_CTRL_U *osd_ctrl_ptr=NULL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_ctrl_ptr->mBits.img_format=data_format;
            break;

        case LCD_LAYER_OSD1:
            osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD1_CTRL;
            osd_ctrl_ptr->mBits.blk_format=data_format;
            break;

        case LCD_LAYER_OSD2:
            osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD2_CTRL;
            osd_ctrl_ptr->mBits.blk_format=data_format;
            break;

        case LCD_LAYER_OSD3:
            osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD3_CTRL;
            osd_ctrl_ptr->mBits.blk_format=data_format;
            break;
#ifdef PLATFORM_SC8800G
    case LCD_LAYER_OSD4:			
            osd_ctrl_ptr=(OSD_CTRL_U*)REG_OSD4_CTRL;
            osd_ctrl_ptr->mBits.blk_format=data_format;
            break;
    case LCD_LAYER_OSD5:			
            osd_ctrl_ptr=(OSD_CTRL_U*)REG_OSD5_CTRL;
            osd_ctrl_ptr->mBits.blk_format=data_format;
            break;
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetDataFormat ERROR],layer %d, format %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_595_112_2_18_1_1_42_32,(uint8*)"dd", layer_id, data_format);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc layer enavle
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLayerBlockEnable (LCD_LAYER_ID_E layer_id, BOOLEAN is_enable)
{
    IMG_CTRL_U *img_ctrl_ptr= (IMG_CTRL_U *) REG_IMG_CTRL;
    OSD_CTRL_U *osd_ctrl_ptr=NULL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_ctrl_ptr->mBits.img_en=is_enable;
            break;
        case LCD_LAYER_OSD1:
            osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD1_CTRL;
            osd_ctrl_ptr->mBits.blk_en=is_enable;
            break;
        case LCD_LAYER_OSD2:
            osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD2_CTRL;
            osd_ctrl_ptr->mBits.blk_en=is_enable;
            break;
        case LCD_LAYER_OSD3:
            osd_ctrl_ptr= (OSD_CTRL_U *) REG_OSD3_CTRL;
            osd_ctrl_ptr->mBits.blk_en=is_enable;
            break;
#ifdef PLATFORM_SC8800G
    case LCD_LAYER_OSD4:			
            osd_ctrl_ptr=(OSD_CTRL_U*)REG_OSD4_CTRL;
            osd_ctrl_ptr->mBits.blk_en=is_enable;
            break;
    case LCD_LAYER_OSD5:			
            osd_ctrl_ptr=(OSD_CTRL_U*)REG_OSD5_CTRL;
            osd_ctrl_ptr->mBits.blk_en=is_enable;
            break;		
#endif	
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_637_112_2_18_1_1_42_33,(uint8*)"dd", layer_id, is_enable);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
// Description: set lcdc logic size
// Global resource dependence:
// Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLCDCSize (LCDC_SIZE_T *size)
{
    LCDC_DISP_SIZE_U *lcdc_size_ptr= (LCDC_DISP_SIZE_U *) REG_LCDC_DISP_SIZE;

    lcdc_size_ptr->mBits.disp_size_x=size->w;
    lcdc_size_ptr->mBits.disp_size_y=size->h;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc corp pitch
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetPitch (LCD_LAYER_ID_E layer_id, uint32 pitch)
{
    IMG_PITCH_U *img_pitch_ptr= (IMG_PITCH_U *) REG_IMG_PITCH;
    OSD_PITCH_U *osd_pitch_prt=NULL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_pitch_ptr->mBits.img_pitch=pitch;
            break;
        case LCD_LAYER_OSD1:
            osd_pitch_prt= (OSD_PITCH_U *) REG_OSD1_PITCH;
            osd_pitch_prt->mBits.blk_pitch=pitch;
            break;
        case LCD_LAYER_OSD2:
            osd_pitch_prt= (OSD_PITCH_U *) REG_OSD2_PITCH;
            osd_pitch_prt->mBits.blk_pitch=pitch;
            break;
        case LCD_LAYER_OSD3:
            osd_pitch_prt= (OSD_PITCH_U *) REG_OSD3_PITCH;
            osd_pitch_prt->mBits.blk_pitch=pitch;
            break;
#ifdef PLATFORM_SC8800G
        case LCD_LAYER_OSD4:		
            osd_pitch_prt=(OSD_PITCH_U*)REG_OSD4_PITCH;
            osd_pitch_prt->mBits.blk_pitch=pitch;		
            break;	
        case LCD_LAYER_OSD5:		
            osd_pitch_prt=(OSD_PITCH_U*)REG_OSD5_PITCH;
            osd_pitch_prt->mBits.blk_pitch=pitch;		
            break;			
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetPitch ERROR],layer %d, pitch %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_691_112_2_18_1_1_42_34,(uint8*)"dd", layer_id, pitch);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc blk rect
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLayerRect (LCD_LAYER_ID_E layer_id, LCDC_RECT_T *disp_rect)
{
    IMG_DISP_XY_U *img_pos_ptr= (IMG_DISP_XY_U *) REG_IMG_DISP_XY;
    IMG_SIZE_XY_U *img_size_ptr= (IMG_SIZE_XY_U *) REG_IMG_SIZE_XY;
    OSD_DISP_XY_U *osd_pos_ptr=NULL;
    OSD_SIZE_XY_U *osd_size_ptr=NULL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_pos_ptr->mBits.img_disp_x = disp_rect->x;
            img_pos_ptr->mBits.img_disp_y = disp_rect->y;
            img_size_ptr->mBits.img_size_x = disp_rect->w;
            img_size_ptr->mBits.img_size_y = disp_rect->h;
            break;
        case LCD_LAYER_OSD1:
            osd_pos_ptr= (OSD_DISP_XY_U *) REG_OSD1_DISP_XY;
            osd_size_ptr = (OSD_SIZE_XY_U *) REG_OSD1_SIZE_XY;
            osd_pos_ptr->mBits.blk_disp_x = disp_rect->x;
            osd_pos_ptr->mBits.blk_disp_y = disp_rect->y;
            osd_size_ptr->mBits.blk_size_x = disp_rect->w;
            osd_size_ptr->mBits.blk_size_y = disp_rect->h;
            break;
        case LCD_LAYER_OSD2:
            osd_pos_ptr= (OSD_DISP_XY_U *) REG_OSD2_DISP_XY;
            osd_size_ptr = (OSD_SIZE_XY_U *) REG_OSD2_SIZE_XY;
            osd_pos_ptr->mBits.blk_disp_x = disp_rect->x;
            osd_pos_ptr->mBits.blk_disp_y = disp_rect->y;
            osd_size_ptr->mBits.blk_size_x = disp_rect->w;
            osd_size_ptr->mBits.blk_size_y = disp_rect->h;
            break;
        case LCD_LAYER_OSD3:
            osd_pos_ptr= (OSD_DISP_XY_U *) REG_OSD3_DISP_XY;
            osd_size_ptr = (OSD_SIZE_XY_U *) REG_OSD3_SIZE_XY;
            osd_pos_ptr->mBits.blk_disp_x = disp_rect->x;
            osd_pos_ptr->mBits.blk_disp_y = disp_rect->y;
            osd_size_ptr->mBits.blk_size_x = disp_rect->w;
            osd_size_ptr->mBits.blk_size_y = disp_rect->h;
            break;
#ifdef PLATFORM_SC8800G
        case LCD_LAYER_OSD4:			
           	osd_pos_ptr=(OSD_DISP_XY_U*)REG_OSD4_DISP_XY;
    		osd_size_ptr = (OSD_SIZE_XY_U*)REG_OSD4_SIZE_XY;
    		osd_pos_ptr->mBits.blk_disp_x = disp_rect->x;
    		osd_pos_ptr->mBits.blk_disp_y = disp_rect->y;
    		osd_size_ptr->mBits.blk_size_x = disp_rect->w;
    		osd_size_ptr->mBits.blk_size_y = disp_rect->h;
            break;
        case LCD_LAYER_OSD5:			
           	osd_pos_ptr=(OSD_DISP_XY_U*)REG_OSD5_DISP_XY;
    		osd_size_ptr = (OSD_SIZE_XY_U*)REG_OSD5_SIZE_XY;
    		osd_pos_ptr->mBits.blk_disp_x = disp_rect->x;
    		osd_pos_ptr->mBits.blk_disp_y = disp_rect->y;
    		osd_size_ptr->mBits.blk_size_x = disp_rect->w;
    		osd_size_ptr->mBits.blk_size_y = disp_rect->h;
            break;
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_759_112_2_18_1_1_43_35,(uint8*)"ddddd", layer_id,disp_rect->x, disp_rect->y, disp_rect->w, disp_rect->h);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAlphaMode (LCD_LAYER_ID_E layer_id, LCD_OSD_ALPHA_E alpha_mode)
{
    OSD1_CTRL_U *osd1_ctrl_ptr = NULL;
    OSD2_CTRL_U *osd2_ctrl_ptr = NULL;
    OSD3_CTRL_U *osd3_ctrl_ptr = NULL;
#ifdef PLATFORM_SC8800G
	OSD4_CTRL_U* osd4_ctrl_ptr = NULL;
	OSD5_CTRL_U* osd5_ctrl_ptr = NULL;
#endif

    switch (layer_id)
    {
        case LCD_LAYER_OSD1:
            osd1_ctrl_ptr= (OSD1_CTRL_U *) REG_OSD1_CTRL;
            osd1_ctrl_ptr->mBits.blk_alpha_sel=alpha_mode;
            break;

        case LCD_LAYER_OSD2:
            osd2_ctrl_ptr= (OSD2_CTRL_U *) REG_OSD2_CTRL;
            osd2_ctrl_ptr->mBits.blk_alpha_sel=alpha_mode;
            break;

        case LCD_LAYER_OSD3:
            osd3_ctrl_ptr= (OSD3_CTRL_U *) REG_OSD3_CTRL;
            osd3_ctrl_ptr->mBits.blk_alpha_sel=alpha_mode;
            break;
#ifdef PLATFORM_SC8800G
        case LCD_LAYER_OSD4:			
    	    osd4_ctrl_ptr=(OSD4_CTRL_U*)REG_OSD4_CTRL;
    	    osd4_ctrl_ptr->mBits.blk_alpha_sel=alpha_mode;  
            break; 
        case LCD_LAYER_OSD5:			
    	    osd5_ctrl_ptr=(OSD5_CTRL_U*)REG_OSD5_CTRL;
    	    osd5_ctrl_ptr->mBits.blk_alpha_sel=alpha_mode;  
            break; 	
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_805_112_2_18_1_1_43_36,(uint8*)"dd", layer_id, alpha_mode);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAlphaValue (LCD_LAYER_ID_E layer_id, uint8 alpha_value)
{
    OSD_ALPHA_U *osd_alpha_ptr=NULL;

    switch (layer_id)
    {
        case LCD_LAYER_OSD1:
            osd_alpha_ptr= (OSD_ALPHA_U *) REG_OSD1_ALPHA;
            osd_alpha_ptr->mBits.blk_alpha=alpha_value;
            break;

        case LCD_LAYER_OSD2:
            osd_alpha_ptr= (OSD_ALPHA_U *) REG_OSD2_ALPHA;
            osd_alpha_ptr->mBits.blk_alpha=alpha_value;
            break;

        case LCD_LAYER_OSD3:
            osd_alpha_ptr= (OSD_ALPHA_U *) REG_OSD3_ALPHA;
            osd_alpha_ptr->mBits.blk_alpha=alpha_value;
            break;
#ifdef PLATFORM_SC8800G
        case LCD_LAYER_OSD4:			
            osd_alpha_ptr=(OSD_ALPHA_U*)REG_OSD4_ALPHA;
            osd_alpha_ptr->mBits.blk_alpha=alpha_value; 
            break; 
        case LCD_LAYER_OSD5:			
            osd_alpha_ptr=(OSD_ALPHA_U*)REG_OSD5_ALPHA;
            osd_alpha_ptr->mBits.blk_alpha=alpha_value; 
            break; 		
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_845_112_2_18_1_1_43_37,(uint8*)"dd", layer_id, alpha_value);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetGreyRgb (LCD_LAYER_ID_E layer_id, uint32 grey_value)
{
    OSD_GREY_RGB_U *osd_grey_ptr=NULL;

    switch (layer_id)
    {
        case LCD_LAYER_OSD1:
            osd_grey_ptr= (OSD_GREY_RGB_U *) REG_OSD1_GREY_RGB;
            osd_grey_ptr->mBits.blk_grey_rgb=grey_value;
            break;

        case LCD_LAYER_OSD2:
            osd_grey_ptr= (OSD_GREY_RGB_U *) REG_OSD2_GREY_RGB;
            osd_grey_ptr->mBits.blk_grey_rgb=grey_value;
            break;

        case LCD_LAYER_OSD3:
            osd_grey_ptr= (OSD_GREY_RGB_U *) REG_OSD3_GREY_RGB;
            osd_grey_ptr->mBits.blk_grey_rgb=grey_value;
            break;
#ifdef PLATFORM_SC8800G
        case LCD_LAYER_OSD4:			
            osd_grey_ptr=(OSD_GREY_RGB_U*)REG_OSD4_GREY_RGB;
            osd_grey_ptr->mBits.blk_grey_rgb=grey_value;
            break; 
        case LCD_LAYER_OSD5:			
            osd_grey_ptr=(OSD_GREY_RGB_U*)REG_OSD5_GREY_RGB;
            osd_grey_ptr->mBits.blk_grey_rgb=grey_value;
            break; 		
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_885_112_2_18_1_1_43_38,(uint8*)"dd", layer_id, grey_value);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetColorKeyEnable (LCD_LAYER_ID_E layer_id, BOOLEAN is_enable)
{
    OSD1_CTRL_U *osd1_ctrl_ptr=NULL;
    OSD2_CTRL_U *osd2_ctrl_ptr=NULL;
    OSD3_CTRL_U *osd3_ctrl_ptr = NULL;
#ifdef PLATFORM_SC8800G
	OSD4_CTRL_U* osd4_ctrl_ptr = NULL;
	OSD5_CTRL_U* osd5_ctrl_ptr = NULL;
#endif

    switch (layer_id)
    {
        case LCD_LAYER_OSD1:
            osd1_ctrl_ptr= (OSD1_CTRL_U *) REG_OSD1_CTRL;
            osd1_ctrl_ptr->mBits.blk_ck_en=is_enable;
            break;
        case LCD_LAYER_OSD2:
            osd2_ctrl_ptr= (OSD2_CTRL_U *) REG_OSD2_CTRL;
            osd2_ctrl_ptr->mBits.blk_ck_en=is_enable;
            break;
        case LCD_LAYER_OSD3:
            osd3_ctrl_ptr= (OSD3_CTRL_U *) REG_OSD3_CTRL;
            osd3_ctrl_ptr->mBits.blk_ck_en=is_enable;
            break;
#ifdef PLATFORM_SC8800G
        case LCD_LAYER_OSD4:			
            osd4_ctrl_ptr=(OSD4_CTRL_U*)REG_OSD4_CTRL;
            osd4_ctrl_ptr->mBits.blk_ck_en=is_enable;
            break; 
        case LCD_LAYER_OSD5:			
            osd5_ctrl_ptr=(OSD5_CTRL_U*)REG_OSD5_CTRL;
            osd5_ctrl_ptr->mBits.blk_ck_en=is_enable;
            break; 		
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_929_112_2_18_1_1_43_39,(uint8*)"dd", layer_id, is_enable);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set lcdc layer blend postion
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetColorkey (LCD_LAYER_ID_E layer_id, uint32 color_key)
{
    OSD_COLORKEY_U *osd_color_key_ptr=NULL;

    switch (layer_id)
    {
        case LCD_LAYER_OSD1:
            osd_color_key_ptr= (OSD_COLORKEY_U *) REG_OSD1_CK;
            osd_color_key_ptr->mBits.osd_ck=color_key;
            break;
        case LCD_LAYER_OSD2:
            osd_color_key_ptr= (OSD_COLORKEY_U *) REG_OSD2_CK;
            osd_color_key_ptr->mBits.osd_ck=color_key;
            break;
        case LCD_LAYER_OSD3:
            osd_color_key_ptr= (OSD_COLORKEY_U *) REG_OSD3_CK;
            osd_color_key_ptr->mBits.osd_ck=color_key;
            break;
#ifdef PLATFORM_SC8800G
        case LCD_LAYER_OSD4:			
     		osd_color_key_ptr=(OSD_COLORKEY_U*)REG_OSD4_CK;
    		osd_color_key_ptr->mBits.osd_ck=color_key;
            break; 
        case LCD_LAYER_OSD5:			
     		osd_color_key_ptr=(OSD_COLORKEY_U*)REG_OSD5_CK;
    		osd_color_key_ptr->mBits.osd_ck=color_key;
            break; 		
#endif
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_967_112_2_18_1_1_43_40,(uint8*)"dd", layer_id, color_key);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: lcdc cap enable
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapEnable (BOOLEAN is_enable)
{
    LCDC_CAP_CTRL_U *reg_ptr = (LCDC_CAP_CTRL_U *) REG_CAP_CTRL;

    reg_ptr->mBits.cap_en=is_enable ? LCDC_ONE : LCDC_ZERO;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: lcdc cap disable
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapRect (LCDC_RECT_T *rect)
{
    CAP_START_XY_U *cap_start_ptr = (CAP_START_XY_U *) REG_CAP_START_XY;
    CAP_SIZE_XY_U *cap_size_ptr   = (CAP_SIZE_XY_U *) REG_CAP_SIZE_XY;

    cap_start_ptr->mBits.cap_start_x=rect->x;
    cap_start_ptr->mBits.cap_start_y=rect->y;
    cap_size_ptr->mBits.cap_size_x=rect->w;
    cap_size_ptr->mBits.cap_size_y=rect->h;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set address of capture
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapAddr (uint32 addr)
{
    CAP_BASE_ADDR_U *cap_addr_ptr = (CAP_BASE_ADDR_U *) REG_CAP_BASE_ADDR;
#ifdef PLATFORM_SC6800H
    cap_addr_ptr->dwValue = ((addr>>0x02) &0x00ffffff);;
#else
	cap_addr_ptr->dwValue = ((addr>>0x02)&0x3fffffff);
#endif	

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set data format of capture
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapDataFormat (LCD_DATA_FORMAT_E data_format)
{
    LCDC_CAP_CTRL_U *cap_ctrl_ptr = (LCDC_CAP_CTRL_U *) REG_CAP_CTRL;

    switch (data_format)
    {
        case LCD_RGB888:
            cap_ctrl_ptr->mBits.cap_format = 0x0;
            break;
        case LCD_RGB666:
            cap_ctrl_ptr->mBits.cap_format = 0x1;
            break;
        case LCD_RGB565:
            cap_ctrl_ptr->mBits.cap_format = 0x2;
            break;
        default:
            //SCI_TRACE_LOW:"LCDC_DrvSetCapDataFormat:data format is error! format is %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_1032_112_2_18_1_1_43_41,(uint8*)"d",data_format);
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set data endian of capture
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapDataEndian (uint32 endian_switch)
{
    LCDC_CAP_CTRL_U *cap_ctrl_ptr = (LCDC_CAP_CTRL_U *) REG_CAP_CTRL;

    cap_ctrl_ptr->mBits.cap_endian = endian_switch;

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set data format of capture
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetCapPitch (uint32 pitch)
{
    CAP_PITCH_U *cap_pitch_ptr= (CAP_PITCH_U *) REG_CAP_PITCH;
    cap_pitch_ptr->mBits.cap_pitch = pitch;
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set upsample mode of yuv to rgb, 0: duplicate, 1: average
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetY2RUpsampleMode (uint32 mode)
{
    Y2R_CTRL_U *y2r_ctrl_ptr = (Y2R_CTRL_U *) REG_Y2R_CTRL;
    y2r_ctrl_ptr->mBits.upsample_mode=mode;  // 0 duplicate 1 average
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set contrast of yuv to rgb, 0-255
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetY2RContrast (uint8 contrast)
{
    Y2R_CONTRAST_U *y2r_contrast_ptr = (Y2R_CONTRAST_U *) REG_Y2R_CONTRAST;
    y2r_contrast_ptr->mBits.contrast=contrast;
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set saturation of yuv to rgb, 0-255
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetY2RSaturation (uint8 saturation)
{
    Y2R_SATURATION_U *y2r_saturation_ptr = (Y2R_SATURATION_U *) REG_Y2R_SATURATION;
    y2r_saturation_ptr->mBits.saturation=saturation;
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set brightness of yuv to rgb, 0-255
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetY2RBrightness (int16 brightness)
{
    Y2R_BRIGHTNESS_U *y2r_brightness_ptr = (Y2R_BRIGHTNESS_U *) REG_Y2R_BRIGHTNESS;
    y2r_brightness_ptr->mBits.brightness=brightness;
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: lcdc cap disable
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvGetMaxWidth (void)
{
    return LCDC_MAX_WIDTH;
}
/********************************************************************************************/
//Description: lcdc cap disable
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvGetMaxHeight (void)
{
    return LCDC_MAX_HEIGHT;
}
/********************************************************************************************/
//Description: set lcdc trim rect
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLcmRect (LCDC_RECT_T *rect_ptr)
{
    LCDC_LCM_START_U *lcm_start_ptr = (LCDC_LCM_START_U *) REG_LCDC_LCM_START;
    LCDC_LCM_SIZE_U *lcm_size_ptr= (LCDC_LCM_SIZE_U *) REG_LCDC_LCM_SIZE;

    lcm_start_ptr->mBits.lcm_start_x = rect_ptr->x;
    lcm_start_ptr->mBits.lcm_start_y = rect_ptr->y;
    lcm_size_ptr->mBits.lcm_size_x = rect_ptr->w;
    lcm_size_ptr->mBits.lcm_size_y = rect_ptr->h;
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set the interval between 2 AHB master requests for each client.
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAhbReqestGap (uint8 cycles)
{
    LCDC_CTRL_U *lcdc_ctrl_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;

    lcdc_ctrl_ptr->mBits.req_gap = cycles;
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: set high address of block memory
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetAhbHiAddr (uint32 hi_addr)
{
    LCDC_CTRL_U *reg_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;
#ifdef PLATFORM_SC6800H
    reg_ptr->mBits.hi_addr = hi_addr & 0xf;
#endif
    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: disable idetifical layer
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL uint32 LCDC_DrvDisableLayer (uint32 layer_num)
{
    IMG_CTRL_U *img_reg_ptr = PNULL;
    OSD_CTRL_U *osd_reg_ptr = PNULL;
    
    switch (layer_num)
    {
        case 0:            
            img_reg_ptr = (IMG_CTRL_U *) REG_IMG_CTRL;
            img_reg_ptr->mBits.img_en  = FALSE;           
            break;
        case 1:            
            osd_reg_ptr = (OSD_CTRL_U *) REG_OSD1_CTRL;
            osd_reg_ptr->mBits.blk_en = FALSE;            
            break;
        case 2:
            osd_reg_ptr = (OSD_CTRL_U *) REG_OSD2_CTRL;
            osd_reg_ptr->mBits.blk_en = FALSE;            
            break;
        case 3:
            osd_reg_ptr = (OSD_CTRL_U *) REG_OSD3_CTRL;
            osd_reg_ptr->mBits.blk_en = FALSE;            
            break;
#ifdef PLATFORM_SC8800G
	    case 4: 
			osd_reg_ptr = (OSD_CTRL_U*)REG_OSD4_CTRL;
			osd_reg_ptr->mBits.blk_en = FALSE;      			
  			break;
	    case 5:      			
			osd_reg_ptr = (OSD_CTRL_U*)REG_OSD5_CTRL;
			osd_reg_ptr->mBits.blk_en = FALSE;      			
  			break;				
#endif
        default:
            //SCI_TRACE_LOW:"LCDC_DrvDisableLayer is error,layer_num=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_1179_112_2_18_1_1_43_42,(uint8*)"d",layer_num);
            break;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: disable all layer
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvDisableAllLayer (void)
{
    IMG_CTRL_U *reg_img_ptr = (IMG_CTRL_U *) REG_IMG_CTRL;
    OSD_CTRL_U *reg_osd_ptr = (OSD_CTRL_U *) REG_OSD1_CTRL;

    reg_img_ptr->mBits.img_en = 0;
    reg_osd_ptr->mBits.blk_en = 0;
    reg_osd_ptr = (OSD_CTRL_U *) REG_OSD2_CTRL;
    reg_osd_ptr->mBits.blk_en = 0;
    reg_osd_ptr = (OSD_CTRL_U *) REG_OSD3_CTRL;
    reg_osd_ptr->mBits.blk_en = 0;
	#ifdef PLATFORM_SC8800G
	reg_osd_ptr = (OSD_CTRL_U*)REG_OSD4_CTRL;
	reg_osd_ptr->mBits.blk_en = 0;	
	reg_osd_ptr = (OSD_CTRL_U*)REG_OSD5_CTRL;
	reg_osd_ptr->mBits.blk_en = 0;	
	#endif

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: clean lcdc raw state
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvCleanRawState (LCD_INT_TYPE_E irq_num)
{
    LCDC_IRQ_CLR_U  *reg_ptr = (LCDC_IRQ_CLR_U *) REG_LCDC_IRQ_CLR;
    LCDC_IRQ_CLR_U  *reg_state_ptr = (LCDC_IRQ_CLR_U *) REG_LCDC_IRQ_STATUS;

    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("ISP_VSP_DRV: The interrupt num is invalid %d", irq_num));/*assert verified*/

    // if(reg_state_ptr->dwValue & (1<<(uint32)irq_num))
    {
        reg_ptr->dwValue|= (1<< (uint32) irq_num);
    }

    return SCI_SUCCESS;
}

#ifdef PLATFORM_SC8800G 
typedef struct _LCM_CMD_DATA_ADDR_tag
{
	uint32 cmd_addr;
	uint32 data_addr;
}LCM_ADDR_T;

LOCAL LCM_ADDR_T  s_lcm_addr[2];
/********************************************************************************************/
//Description:    set cs address
//Global resource dependence:
//Author: Jianping.wang
/********************************************************************************************/
PUBLIC void LCDC_DrvSetCSAddr(uint16 lcd_cs, uint16 lcd_cd, uint16 lcd_id)
{
    uint32 reg_base = LCM_CD0;    

    SCI_ASSERT(lcd_id<2);/*assert verified*/
    
    //set cs address
    reg_base += lcd_cs * 0x10;   
    s_lcm_addr[lcd_id].cmd_addr  = reg_base+(lcd_cd<<2);      /*lint !e662 !e661*/
    s_lcm_addr[lcd_id].data_addr = reg_base + ((1-lcd_cd)<<2);/*lint !e662 !e661*/
    
    return ;
}
/********************************************************************************************/
//Description:    reset LCD
//Global resource dependence:
//Author: Jianping.wang
/********************************************************************************************/
PUBLIC void LCDC_DrvResetLcd(uint32 delay_ms,uint8 rst_level)
{    
    LCM_RSTN_U    *pLcm_rst = (LCM_RSTN_U *)LCM_RSTN;
    
    pLcm_rst->dwValue = ~rst_level;
    
    OS_TickDelay(delay_ms);

    pLcm_rst->dwValue = rst_level;    
    
    OS_TickDelay(delay_ms);

    pLcm_rst->dwValue = ~rst_level;
    
    OS_TickDelay(delay_ms);
    
    //SCI_TRACE_LOW("reset time = %dms,rst_level = %d",delay_ms,rst_level);

}
/********************************************************************************************/
//Description:  set dest LCD's RGB mode to host
//Global resource dependence:
//Author: Younger.Yang
/********************************************************************************************/
PUBLIC void LCDC_DrvSetRGBMode(uint32 cs_id, LCD_BUS_WIDTH_E rgb_mode)
{
    uint32 offset = cs_id<<1;
    //wait for until AHB FIFO empty(1)[BIT_2]
    while(!((*(volatile uint32 *)LCM_STATUS) & BIT_2))
    {}
    
    switch (rgb_mode)
    {
        case WIDTH_16:
            *(volatile uint32 *)LCM_RGBMODE |= (0x0<<offset);
            break;
        case WIDTH_18:
            *(volatile uint32 *)LCM_RGBMODE |= (0x1<<offset);
            break;
        case WIDTH_8:
            *(volatile uint32 *)LCM_RGBMODE |= (0x2<<offset);
            break;
        case WIDTH_9:
            *(volatile uint32 *)LCM_RGBMODE |= (0x3<<offset);
            break;
        default:
            break;
    }
}
/********************************************************************************************/
//Description:change pulse width    
//Global resource dependence:    
//Author: Jianping.wang
/********************************************************************************************/
PUBLIC ERR_LCD_E LCDC_DrvSetTiming(LCD_SPEC_INFO_T_PTR lcd_spec_ptr, uint32 ahb_clk)
{
    LCM_PARAMTER0_U     *pLcm_para0     = (LCM_PARAMTER0_U *)LCM_PARAMETER0;
    LCM_PARAMTER1_U     *pLcm_para1     = (LCM_PARAMTER1_U *)LCM_PARAMETER1;
    LCM_PARAMTER0_U     Lcm_para_value;
    uint32              temp_value  = 0; 
    
    temp_value = (100000000 >> 17) / (ahb_clk >> 20) ;//multiple 8/10
    //so lcm_cycle = 1.25*min_cycle(spec).
    
    SCI_ASSERT(NULL != temp_value);/*assert verified*/
    Lcm_para_value.mBits.rcss = MIN((((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rcss) / temp_value) + 1), 0x3);
    Lcm_para_value.mBits.rlpw = MIN((((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rlpw) / temp_value) + 1), 0xe);
    Lcm_para_value.mBits.rhpw = MIN((((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.rhpw) / temp_value) + 1), 0xe);
    Lcm_para_value.mBits.wcss = MIN((((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.wcss) / temp_value) + 1), 0x3);
    Lcm_para_value.mBits.wlpw = MIN((((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.wlpw) / temp_value) + 1), 0xe);
    Lcm_para_value.mBits.whpw = MIN((((lcd_spec_ptr->spec_ptr->timing->mcu_mode_timing.whpw) / temp_value) + 1), 0xe);
           
    //wait for until AHB FIFO empty(1)[BIT_2]
    while(!((*(volatile uint32 *)LCM_STATUS) & BIT_2))
    {}
    
    if (0 == (lcd_spec_ptr->cs_pin))
    {
        pLcm_para0->dwValue = Lcm_para_value.dwValue;
    }
    else 
    {        
        pLcm_para1->dwValue = Lcm_para_value.dwValue;
    }

    return ERR_LCD_NONE;
}
/********************************************************************************************/
//Description:set timing for read lcd id  
//Global resource dependence:    
//Author: Jianping.wang
/********************************************************************************************/
PUBLIC ERR_LCD_E LCDC_DrvSetTimingInit(void)
{
    LCM_PARAMTER0_U     *pLcm_para0     = (LCM_PARAMTER0_U *)LCM_PARAMETER0;
    LCM_PARAMTER1_U     *pLcm_para1     = (LCM_PARAMTER1_U *)LCM_PARAMETER1;
    LCM_PARAMTER0_U     Lcm_para_value;   
    
    Lcm_para_value.mBits.rcss = 3;
    Lcm_para_value.mBits.rlpw = 0xe;
    Lcm_para_value.mBits.rhpw = 0xe;
    Lcm_para_value.mBits.wcss = 3;
    Lcm_para_value.mBits.wlpw = 0xe;
    Lcm_para_value.mBits.whpw = 0xe;
           
    //wait for until AHB FIFO empty(1)[BIT_2]
    while(!((*(volatile uint32 *)LCM_STATUS) & BIT_2))
    {}
  
    pLcm_para0->dwValue = Lcm_para_value.dwValue;         
    pLcm_para1->dwValue = Lcm_para_value.dwValue;
 
    LCDC_DrvSetFmark(LCD_FMARK_NONE,LCD_FMARK_HIGH);

    return ERR_LCD_NONE;
}
/********************************************************************************************/
//Description: set lcm ctrl 
//Global resource dependence: 
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvSetLCMCtrl(uint32 setting)
{
	LCM_CTRL_U *plcdc_lcmctrl = (LCM_CTRL_U*)LCM_CTRL;

	setting &= 0xfc0fffff;
	setting |= 0x100000;
	plcdc_lcmctrl->dwValue = setting;

    return 0;
}
/********************************************************************************************/
//Description:    send command and data to LCD
//Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
//Author: Jianping.wang
/********************************************************************************************/
PUBLIC int32 LCDC_SendCmdData(uint32 cmd,uint32 data,uint32 lcd_id)
{    
    uint32 cmd_addr;
    uint32 data_addr;    

    cmd_addr = s_lcm_addr[lcd_id].cmd_addr;
    data_addr = s_lcm_addr[lcd_id].data_addr;
    
    //wait for if AHB FIFO full
    while((*(volatile uint32 *)LCM_STATUS) & BIT_1)
    {}
    
    TB_REG_SET(cmd_addr,cmd);
    
    while((*(volatile uint32 *)LCM_STATUS) & BIT_1)
    {}
    TB_REG_SET(data_addr,data);
    
    return ERR_LCD_NONE;
}
/********************************************************************************************/
//Description:    send command to LCD
//Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
//Author: Jianping.wang
/********************************************************************************************/
PUBLIC int32 LCDC_SendCmd(uint32 cmd,uint32 lcd_id)
{
    uint32 cmd_addr;    

    cmd_addr = s_lcm_addr[lcd_id].cmd_addr;    
    
    //wait for if AHB FIFO full
    while((*(volatile uint32 *)LCM_STATUS) & BIT_1)
    {}
    
    TB_REG_SET(cmd_addr,cmd);    
    
    return ERR_LCD_NONE;
}
/********************************************************************************************/
//Description:    send data to LCD GRAM
//Global resource dependence: g_lcm_lcd_sum,g_lcm_pin_use
//Author: Jianping.wang
/********************************************************************************************/
PUBLIC int32 LCDC_SendData(uint32 data,uint32 lcd_id)
{
    uint32 data_addr;
    
    data_addr = s_lcm_addr[lcd_id].data_addr;    
    
    //wait for if AHB FIFO full
    while((*(volatile uint32 *)LCM_STATUS) & BIT_1)
    {}
    
    TB_REG_SET(data_addr,data);
    
    return ERR_LCD_NONE;
}
/********************************************************************************************/
//Description:   Read LCM
//Global resource dependence:
//Author: Younger.Yang
//Note: You may read LCD ID before lcd init,otherwise
//  the read data may be incorrect.
/********************************************************************************************/
PUBLIC uint32 LCDC_Read(uint32 lcd_id, uint32 cd_value)
{
    uint32 value;
    uint32 data_addr;    

    data_addr = s_lcm_addr[lcd_id].data_addr;
    
    LCDC_SendCmd(cd_value,lcd_id);           //send lcd index    
    TB_REG_OR(data_addr, (1<<18));           //set to read mode
 
    SCI_Sleep(50);

    value = TB_REG_GET(LCM_RDDATA);

    return value;
}
#endif
/********************************************************************************************/
//Description:   set LCDC Clock
//Global resource dependence:
//Author: Younger.Yang
//Note: 
/********************************************************************************************/
PUBLIC void LCDC_SetClock(LCDC_CLOCK_E clock_type)
{
    uint32 reg_value = 0;   
    
    #ifdef PLATFORM_SC6800H
    if(clock_type > LCDC_CLOCK_64MHZ)
    {
        clock_type = LCDC_CLOCK_64MHZ;
    }
    //set 64MHz
    reg_value = REG32(AHB_CLK_CFG1);

    if(LCDC_CLOCK_64MHZ == clock_type)
    {
        reg_value |= 0x3;
    }
    else
    {
        reg_value &= 0xfffffffc;
        reg_value |= 0x2;
    }
    REG32(AHB_CLK_CFG1) = reg_value;
    
    #else
    if(clock_type > LCDC_CLOCK_96MHZ)
    {
        clock_type = LCDC_CLOCK_96MHZ;
    }

    switch(clock_type)
    {
        case LCDC_CLOCK_96MHZ:
            //set 96MHz
            reg_value = REG32(GR_GEN4);
            reg_value &= 0xfffffff8;
            REG32(GR_GEN4) = reg_value;

            reg_value = REG32(GR_PLL_SCR);
            reg_value &= 0xffffff3f;
            REG32(GR_PLL_SCR) = reg_value;
            break;
        case LCDC_CLOCK_64MHZ:
            //set 64MHz
            reg_value = REG32(GR_GEN4);
            reg_value &= 0xfffffff8;
            REG32(GR_GEN4) = reg_value;

            reg_value = REG32(GR_PLL_SCR);
            reg_value &= 0xffffff3f;
            reg_value |= 0x40;
            REG32(GR_PLL_SCR) = reg_value;
            break;
        case LCDC_CLOCK_48MHZ:
            //set 48MHz
            reg_value = REG32(GR_GEN4);
            reg_value &= 0xfffffff8;
            reg_value |= 0x1;
            REG32(GR_GEN4) = reg_value;

            reg_value = REG32(GR_PLL_SCR);
            reg_value &= 0xffffff3f;            
            REG32(GR_PLL_SCR) = reg_value;
            break;
        default:
            //SCI_TRACE_LOW:"LCDC_SetClock Fail,clock_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_1523_112_2_18_1_1_44_43,(uint8*)"d",clock_type);
            break;
     }         
    
    #endif
}
/*lint -restore*/
/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                             					        *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

