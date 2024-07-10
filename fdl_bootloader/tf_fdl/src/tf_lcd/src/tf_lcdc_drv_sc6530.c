/***************************************************************************************
** File Name:      lcdc_drv_sc6530.c                                                          *
** DATE:           11/16/2011                                                          *
** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.                   *
** Description:                                                                        *
****************************************************************************************/

/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 11/16/2011    Eddy.Wei   Create.                                           *
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
#define LCDC_IRQ_NUM            2
#define LCDC_SUPPORT_LAYERNUM 4
#define REG32(x)                (*((volatile uint32 *)(x)))
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
LOCAL void _OpenInt (void)
{
    TB_EnableINT (TB_LCDC_INT);
}
/********************************************************************************************/
//Description: close interrupt of LCDC
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL void _CloseInt (void)
{
    TB_DisableINT (TB_LCDC_INT);
}
/********************************************************************************************/
//Description: lcdc module high irq handle func
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL void LCDC_DrvHisr (uint32 count, void *data)
{
    uint32 i = 0;

    for (i=0; i<LCD_INT_MAX; i++)
    {
        if (_GetIntStatus ( (LCD_INT_TYPE_E) i, (LCDC_IRQ_STATUS_U *) data))
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
        SCI_TRACE_LOW ("LCDC: [0x20d00110] = 0x%08X\r\n", REG32 (0x20d00110));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20d00114] = 0x%08X\r\n", REG32 (0x20d00114));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20d00118] = 0x%08X\r\n", REG32 (0x20d00118));/*lint !e718*/
        SCI_TRACE_LOW ("LCDC: [0x20d0011c] = 0x%08X\r\n", REG32 (0x20d0011c));/*lint !e718*/
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
    LCDC_IRQ_CLR_U  *reg_clear_ptr = (LCDC_IRQ_CLR_U *) REG_LCDC_IRQ_CLR;

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
    reg_value = REG32 (AHB_EB0_SET);
    reg_value |= (uint32) LCDC_EB_SET;
    REG32 (AHB_EB0_SET) = reg_value;

    // AHB lcdc soft reset, all lcdc registers are set to 0
    reg_value = REG32 (AHB_RST0_SET);
    reg_value |= (uint32) LCDC_SOFT_RST_SET;
    REG32 (AHB_RST0_SET) = reg_value;

    SCI_Sleep (10);

    // clear AHB lcdc soft reset
    reg_value = REG32 (AHB_RST0_CLR);
    reg_value |= (uint32) LCDC_SOFT_RST_CLR;
    REG32 (AHB_RST0_CLR) = reg_value;
    
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
        //enable AHB  lcdc enable
        reg_value = REG32 (AHB_EB0_SET);
        reg_value |= (uint32) LCDC_EB_SET;
        REG32 (AHB_EB0_SET) = reg_value;
        reg_ptr->mBits.lcd_enable=LCDC_ONE;

        //set clk
        //It is not nedded to setting AHB LCDC CLK for PLATFORM_SC6530

        //enable LCDC INT of top module
        if (!s_is_register)
        {
            if (TB_SUCCESS != ISR_RegHandler_Ex (TB_LCDC_INT, \
                                                 LCDC_DrvLisr, \
                                                 LCDC_DrvHisr, \
                                                 CHIPDRV_HISR_PRIO_2, \
                                                 (void *) &s_record_lcdc_int_status))
            {
                SCI_PASSERT (0, ("Register Interrupt of the LCDC failed!")); /*assert verified*/
            }

            s_is_register = TRUE;
        }

        TB_EnableINT (TB_LCDC_INT);
    }
    else
    {
        reg_ptr->mBits.lcd_enable = LCDC_ZERO;

        //disable AHB_CTL0 lcdc enable
        reg_value = REG32 (AHB_EB0_SET);
        reg_value &= ~ ( (uint32) LCDC_EB_SET);
        REG32 (AHB_EB0_SET) = reg_value;
    }

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description: lcdc start
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
PUBLIC uint32 LCDC_DrvRun (void)
{
    LCDC_CTRL_U *reg_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;

    SCI_DisableIRQ();
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
    uint32              y_base_addr  = ( (y_addr>>0x02) &0x3fffffff);
    uint32              uv_base_addr = ( (uv_addr>>0x02) &0x3fffffff);

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

        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetAhbMasterAddr ERROR],layer %d, y_addr 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_381_112_2_18_1_1_45_44,(uint8*)"dd", layer_id, y_addr);
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
    uint32 alpha_base_addr = ( (base_addr>>0x02) &0x3fffffff);

    switch (layer_id)
    {
        case LCD_LAYER_OSD1:
            alpha_addr_ptr= (OSD_ALPHA_BASE_ADDR_U *) REG_OSD1_ALPHA_BASE_ADDR;
            break;
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetAlphaBaseAddr ERROR],layer %d, base_addr 0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_402_112_2_18_1_1_45_45,(uint8*)"dd", layer_id, base_addr);
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
PUBLIC uint32 LCDC_DrvSetEndian (LCD_LAYER_ID_E layer_id, uint32 y_endian_switch,uint32 uv_endian_switch)
{
    IMG_CTRL_U *img_ctrl_ptr= (IMG_CTRL_U *) REG_IMG_CTRL;
    OSD1_CTRL_U *osd1_ctrl_ptr = NULL;
    OSD2_CTRL_U *osd2_ctrl_ptr = NULL;
    OSD3_CTRL_U *osd3_ctrl_ptr = NULL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_ctrl_ptr->mBits.img_y_switch = y_endian_switch;
            img_ctrl_ptr->mBits.img_uv_switch = uv_endian_switch;
            break;
            
        case LCD_LAYER_OSD1:
            osd1_ctrl_ptr= (OSD1_CTRL_U *) REG_OSD1_CTRL;
            osd1_ctrl_ptr->mBits.blk_switch = y_endian_switch;
            osd1_ctrl_ptr->mBits.blk_alpha_switch = y_endian_switch;
            break;
            
        case LCD_LAYER_OSD2:
            osd2_ctrl_ptr= (OSD2_CTRL_U *) REG_OSD2_CTRL;
            osd2_ctrl_ptr->mBits.blk_switch = y_endian_switch;
            break;
            
        case LCD_LAYER_OSD3:
            osd3_ctrl_ptr= (OSD3_CTRL_U *) REG_OSD3_CTRL;
            osd3_ctrl_ptr->mBits.blk_switch = y_endian_switch;
            break;
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetEndian ERROR],layer %d, endian1 %d endian2 %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_444_112_2_18_1_1_45_46,(uint8*)"ddd", layer_id, y_endian_switch,uv_endian_switch);
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
    OSD1_CTRL_U *osd1_ctrl_ptr = NULL;
    OSD2_CTRL_U *osd2_ctrl_ptr = NULL;
    OSD3_CTRL_U *osd3_ctrl_ptr = NULL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_ctrl_ptr->mBits.img_format=data_format;
            break;

        case LCD_LAYER_OSD1:
            osd1_ctrl_ptr= (OSD1_CTRL_U *) REG_OSD1_CTRL;
            osd1_ctrl_ptr->mBits.blk_format=data_format;
            break;

        case LCD_LAYER_OSD2:
            osd2_ctrl_ptr= (OSD2_CTRL_U *) REG_OSD2_CTRL;
            osd2_ctrl_ptr->mBits.blk_format=data_format;
            break;

        case LCD_LAYER_OSD3:
            osd3_ctrl_ptr= (OSD3_CTRL_U *) REG_OSD3_CTRL;
            osd3_ctrl_ptr->mBits.blk_format=data_format;
            break;
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetDataFormat ERROR],layer %d, format %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_483_112_2_18_1_1_45_47,(uint8*)"dd", layer_id, data_format);
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
    OSD1_CTRL_U *osd1_ctrl_ptr = NULL;
    OSD2_CTRL_U *osd2_ctrl_ptr = NULL;
    OSD3_CTRL_U *osd3_ctrl_ptr = NULL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_ctrl_ptr->mBits.img_en=is_enable;
            break;
            
        case LCD_LAYER_OSD1:
            osd1_ctrl_ptr= (OSD1_CTRL_U *) REG_OSD1_CTRL;
            osd1_ctrl_ptr->mBits.blk_en=is_enable;
            break;
            
        case LCD_LAYER_OSD2:
            osd2_ctrl_ptr= (OSD2_CTRL_U *) REG_OSD2_CTRL;
            osd2_ctrl_ptr->mBits.blk_en=is_enable;
            break;
            
        case LCD_LAYER_OSD3:
            osd3_ctrl_ptr= (OSD3_CTRL_U *) REG_OSD3_CTRL;
            osd3_ctrl_ptr->mBits.blk_en=is_enable;
            break;
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetLayerBlockEnable ERROR],layer %d, is_enable %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_521_112_2_18_1_1_45_48,(uint8*)"dd", layer_id, is_enable);
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
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetPitch ERROR],layer %d, pitch %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_569_112_2_18_1_1_46_49,(uint8*)"dd", layer_id, pitch);
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
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetLayerRect ERROR],layer %d, disp rect (%d, %d, %d, %d)"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_623_112_2_18_1_1_46_50,(uint8*)"ddddd", layer_id,disp_rect->x, disp_rect->y, disp_rect->w, disp_rect->h);
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
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetAlphaMode ERROR],layer %d, alpha_mode %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_656_112_2_18_1_1_46_51,(uint8*)"dd", layer_id, alpha_mode);
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
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetAlphaValue ERROR],layer %d, alpha value %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_687_112_2_18_1_1_46_52,(uint8*)"dd", layer_id, alpha_value);
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
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetGreyRgb ERROR],layer %d, grey_value %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_718_112_2_18_1_1_46_53,(uint8*)"dd", layer_id, grey_value);
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
            
        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetColorKeyEnable ERROR],layer %d, is_enable %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_751_112_2_18_1_1_46_54,(uint8*)"dd", layer_id, is_enable);
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

        default:
            //SCI_TRACE_LOW:"[LCDC_DrvSetColorkey ERROR],layer %d, color_key %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_782_112_2_18_1_1_46_55,(uint8*)"dd", layer_id, color_key);
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
    cap_addr_ptr->dwValue = ( (addr>>0x02) &0x3fffffff);

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
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_846_112_2_18_1_1_46_56,(uint8*)"d",data_format);
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

    cap_ctrl_ptr->mBits.cap_switch = endian_switch;

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
//Description: disable idetifical layer
//Global resource dependence:
//Author: Tim.zhu
/********************************************************************************************/
LOCAL uint32 LCDC_DrvDisableLayer (uint32 layer_num)
{
    IMG_CTRL_U *img_reg_ptr = PNULL;
    OSD1_CTRL_U *osd1_ctrl_ptr = NULL;
    OSD2_CTRL_U *osd2_ctrl_ptr = NULL;
    OSD3_CTRL_U *osd3_ctrl_ptr = NULL;

    switch (layer_num)
    {
        case 0:
            img_reg_ptr = (IMG_CTRL_U *) REG_IMG_CTRL;
            img_reg_ptr->mBits.img_en  = FALSE;
            break;
            
        case 1:
            osd1_ctrl_ptr = (OSD1_CTRL_U *) REG_OSD1_CTRL;
            osd1_ctrl_ptr->mBits.blk_en = FALSE;
            break;
            
        case 2:
            osd2_ctrl_ptr = (OSD2_CTRL_U *) REG_OSD2_CTRL;
            osd2_ctrl_ptr->mBits.blk_en = FALSE;
            break;
            
        case 3:
            osd3_ctrl_ptr = (OSD3_CTRL_U *) REG_OSD3_CTRL;
            osd3_ctrl_ptr->mBits.blk_en = FALSE;
            break;
            
        default:
            //SCI_TRACE_LOW:"LCDC_DrvDisableLayer is error,layer_num=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,TF_LCDC_DRV_SC6530_978_112_2_18_1_1_46_57,(uint8*)"d",layer_num);
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
    OSD1_CTRL_U *reg_osd1_ptr = (OSD1_CTRL_U *) REG_OSD1_CTRL;
    OSD2_CTRL_U *reg_osd2_ptr = (OSD2_CTRL_U *) REG_OSD2_CTRL;
    OSD3_CTRL_U *reg_osd3_ptr = (OSD3_CTRL_U *) REG_OSD3_CTRL;

    reg_img_ptr->mBits.img_en = 0;
    reg_osd1_ptr->mBits.blk_en = 0;
    reg_osd2_ptr->mBits.blk_en = 0;
    reg_osd3_ptr->mBits.blk_en = 0;

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

    SCI_PASSERT ( (uint32) LCD_INT_MAX > (uint32) irq_num, ("ISP_VSP_DRV: The interrupt num is invalid %d", irq_num));/*assert verified*/

    reg_ptr->dwValue|= (1<< (uint32) irq_num);

    return SCI_SUCCESS;
}
/********************************************************************************************/
//Description:   set LCDC Clock
//Global resource dependence:
//Author: Younger.Yang
//Note:
/********************************************************************************************/
PUBLIC void LCDC_SetClock (LCDC_CLOCK_E clock_type)
{
    //It is not nedded to setting AHB LCDC CLK for PLATFORM_SC6530
}

/********************************************************************************************/
//Description:   set LCDC CAP MASTER
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/********************************************************************************************/
PUBLIC void LCDC_DrvSetCapMaster (LCDC_CAP_DISP_E cap_disp_mode)
{
    LCDC_CAP_CTRL_U *reg_ptr = (LCDC_CAP_CTRL_U *) REG_CAP_CTRL;

    switch (cap_disp_mode)
    {
        case LCDC_CAP_MODE:
            reg_ptr->mBits.cap_master_en = LCDC_CAP_MASTER_1;
            reg_ptr->mBits.cap_master_mode = LCDC_INCRSE_ADDR;
            break;

        case LCDC_DISP_SPI_MODE:
            reg_ptr->mBits.cap_master_en = LCDC_CAP_MASTER_2;
            reg_ptr->mBits.cap_master_mode = LCDC_FIXD_ADDR;
            break;

        case LCDC_DISP_PARALLEL_MODE:
            reg_ptr->mBits.cap_master_en = LCDC_CAP_MASTER_2;
            reg_ptr->mBits.cap_master_mode = LCDC_INCRSE_WRAP_ADDR;
            break;

        default:
            break;
    }
}

/********************************************************************************************/
//Description:   set LCDC DISPLAY MODE
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/********************************************************************************************/
PUBLIC void LCDC_DrvSetDispRgbMode (LCDC_DISP_RGB_MODE_E disp_rgb_mode)
{
    LCDC_CTRL_U *lcdc_ctrl_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;

    lcdc_ctrl_ptr->mBits.rgb_mode = disp_rgb_mode;

}

/********************************************************************************************/
//Description:   set LCDC EXPAND MODE
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/********************************************************************************************/
PUBLIC void LCDC_DrvSetExpandMode (LCDC_EXPAND_MODE_E  expand_mode)
{
    LCDC_CTRL_U *lcdc_ctrl_ptr = (LCDC_CTRL_U *) REG_LCDC_CTRL;

    lcdc_ctrl_ptr->mBits.expand_mode = expand_mode;

}

/********************************************************************************************/
//Description:   set LCDC CAP ROTATION MODE
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/********************************************************************************************/
PUBLIC void LCDC_DrvSetCapRotation (LCDC_ROTATION_E rotation_mode)
{
    LCDC_CAP_CTRL_U *cap_ctrl_ptr = (LCDC_CAP_CTRL_U *) REG_CAP_CTRL;

    cap_ctrl_ptr->mBits.cap_rot = rotation_mode;

}

/********************************************************************************************/
//Description:   set LCDC LAYERS ROTATION MODE
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/*****************************************************************************************/
PUBLIC void  LCDC_DrvSetLayerRotation (LCD_LAYER_ID_E layer_id, LCDC_ROTATION_E rotation_mode)
{
    IMG_CTRL_U *img_ctrl_ptr= (IMG_CTRL_U *) REG_IMG_CTRL;
    OSD1_CTRL_U *osd1_ctrl_ptr= (OSD1_CTRL_U *) REG_OSD1_CTRL;
    OSD2_CTRL_U *osd2_ctrl_ptr= (OSD2_CTRL_U *) REG_OSD2_CTRL;
    OSD3_CTRL_U *osd3_ctrl_ptr= (OSD3_CTRL_U *) REG_OSD3_CTRL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_ctrl_ptr->mBits.img_rot = rotation_mode;
            break;

        case LCD_LAYER_OSD1:
            osd1_ctrl_ptr->mBits.blk_rot = rotation_mode;
            break;

        case LCD_LAYER_OSD2:
            osd2_ctrl_ptr->mBits.blk_rot = rotation_mode;
            break;

        case LCD_LAYER_OSD3:
            osd3_ctrl_ptr->mBits.blk_rot = rotation_mode;
            break;

        default:
            SCI_PASSERT (0, ("LCDC_DRV: LCDC_DrvSetLayerRotation layer %d, rot %d", layer_id, rotation_mode)); /*assert to do*/
            break;
    }

}

/********************************************************************************************/
//Description:   set LCDC CAP RB SWITCH MODE
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/*****************************************************************************************/
PUBLIC void LCDC_DrvSetCapRB_Switch (LCDC_RB_SWITCH_E rb_switch_mode)
{
    LCDC_CAP_CTRL_U *cap_ctrl_ptr = (LCDC_CAP_CTRL_U *) REG_CAP_CTRL;

    cap_ctrl_ptr->mBits.cap_rb_switch = rb_switch_mode;

}

/********************************************************************************************/
//Description:   set LCDC LAYERS RB SWITCH MODE
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/*****************************************************************************************/
PUBLIC void LCDC_DrvSetLayerRB_Switch (LCD_LAYER_ID_E layer_id, LCDC_RB_SWITCH_E rg_switch_mode)
{
    IMG_CTRL_U *img_ctrl_ptr= (IMG_CTRL_U *) REG_IMG_CTRL;
    OSD1_CTRL_U *osd1_ctrl_ptr= (OSD1_CTRL_U *) REG_OSD1_CTRL;
    OSD2_CTRL_U *osd2_ctrl_ptr= (OSD2_CTRL_U *) REG_OSD2_CTRL;
    OSD3_CTRL_U *osd3_ctrl_ptr= (OSD3_CTRL_U *) REG_OSD3_CTRL;

    switch (layer_id)
    {
        case LCD_LAYER_IMAGE:
            img_ctrl_ptr->mBits.img_rb_switch = rg_switch_mode;
            break;

        case LCD_LAYER_OSD1:
            osd1_ctrl_ptr->mBits.blk_rb_switch= rg_switch_mode;
            break;

        case LCD_LAYER_OSD2:
            osd2_ctrl_ptr->mBits.blk_rb_switch = rg_switch_mode;
            break;

        case LCD_LAYER_OSD3:
            osd3_ctrl_ptr->mBits.blk_rb_switch = rg_switch_mode;
            break;

        default:
            SCI_PASSERT (0, ("LCDC_DRV: LCDC_DrvSetLayerRB_Switch layer %d, mode %d", layer_id, rg_switch_mode)); /*assert to do*/
            break;
    }

}

/********************************************************************************************/
//Description:   set LCDC OSD BLEND MODE
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/*****************************************************************************************/
PUBLIC void LCDC_DrvSetOsdBlendMode (LCD_LAYER_ID_E layer_id, LCDC_OSD_BLEND_MODE_E blend_mode)
{
    OSD1_CTRL_U *osd1_ctrl_ptr= (OSD1_CTRL_U *) REG_OSD1_CTRL;
    OSD2_CTRL_U *osd2_ctrl_ptr= (OSD2_CTRL_U *) REG_OSD2_CTRL;
    OSD3_CTRL_U *osd3_ctrl_ptr= (OSD3_CTRL_U *) REG_OSD3_CTRL;

    switch (layer_id)
    {
        case LCD_LAYER_OSD1:
            osd1_ctrl_ptr->mBits.blk_blend_mode = blend_mode;
            break;

        case LCD_LAYER_OSD2:
            osd2_ctrl_ptr->mBits.blk_blend_mode = blend_mode;
            break;

        case LCD_LAYER_OSD3:
            osd3_ctrl_ptr->mBits.blk_blend_mode = blend_mode;
            break;

        default:
            SCI_PASSERT (0, ("LCDC_DRV: LCDC_DrvSetOsdBlendMode layer %d, mode %d", layer_id, blend_mode)); /*assert to do*/
            break;
    }

}

/********************************************************************************************/
//Description:   set LCDC CAP MODE
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/*****************************************************************************************/
PUBLIC void LCDC_DrvSetCapMode (LCDC_CAP_MODE_E cap_mode)
{
    LCDC_CAP_CTRL_U *cap_ctrl_ptr = (LCDC_CAP_CTRL_U *) REG_CAP_CTRL;

    cap_ctrl_ptr->mBits.cap_mode = cap_mode;

}

/********************************************************************************************/
//Description:   set LCDC FMark sync delay
//Global resource dependence:
//Author: Eddy.Wei
//Note:
/*****************************************************************************************/
PUBLIC void LCDC_DrvSetFMarkSyncDelay (uint32 delay_uint)
{
    LCDC_SYNC_DELAY_U *sync_delay_ptr = (LCDC_SYNC_DELAY_U *) REG_LCDC_SYNC_DELAY;

    sync_delay_ptr->mBits.sync_delay = delay_uint&0x000fffff;

}

/*lint -restore*/
/**---------------------------------------------------------------------------------------------*
 **                         Compiler Flag                                                       *
 **---------------------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
