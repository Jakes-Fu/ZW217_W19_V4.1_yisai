/*******************************************************************************
 ** FileName:      dc_lcdc.c                                                   *
 ** Author:        jing.li                                                     *
 ** Date:          2008/11/29                                                  *
 ** CopyRight:     2008, Spreatrum, Incoporated, All right reserved            *
 ** Description:                                                               *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                                 *
 ** ------------------------------------------------------------------------------------ *
 ** DATE           NAME                   DESCRIPTION                                   *
 ** 2008/11/29     jing.li                 Created                                      *
 *******************************************************************************/

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ms_ref_dc_trc.h"
#include "dc_app_6x00.h"
#include "ref_lcd.h"


/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif


/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
/*
#define CHECK_LCD_BLOCK_VAILD(lcd_id, blk_num) \
do{\
    SCI_PASSERT(((lcd_id < LCD_GetUsedNum()) && (blk_num < BLOCK_SUM)), \
                           ("\nLCD:%d block:%d not supoort", lcd_id, blk_num));\
}while(0)

#define CHECK_LCD_BLOCK_OWNER(lcd_id, blk_num) \
do{\
    if(SCI_InThreadContext()){\
        if(SCI_IdentifyThread() != s_st_block_ctrl_array[lcd_id][blk_num].owner_id){\
            SCI_PASSERT(0, ("\n The caller thread must get mutex first")); \
        }\
    }\
}while(0)
*/
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/
typedef struct{
    SCI_MUTEX_PTR    block_mutex_ptr;
    char*                   block_mutex_string;
    uint32                  owner_id;
    BOOLEAN              is_enable;
    uint8                   u8_pack_1;
    uint16                  u16_pack_2;
}LCD_BLOCK_CTRL_T;


/*--------------------------- Local Data ------------------------------------*/
LOCAL   BLOCK_T  		s_buffer_info[LCD_SUPPORT_MAX][BLOCK_SUM];

/*lint -save -e785 */
LOCAL const   char*     s_block_mutex_string_array[LCD_SUPPORT_MAX][BLOCK_SUM] = {
                           {"LCD_0_BLOCK_0_MUTEX", "LCD_0_BLOCK_1_MUTEX", "LCD_0_BLOCK_2_MUTEX"}, 
                         {"LCD_1_BLOCK_0_MUTEX", "LCD_1_BLOCK_1_MUTEX", "LCD_1_BLOCK_2_MUTEX"}};
                        //   {"LCD_2_BLOCK_0_MUTEX", "LCD_2_BLOCK_1_MUTEX", "LCD_2_BLOCK_2_MUTEX"}};
 /*lint -restore */

LOCAL LCD_BLOCK_CTRL_T s_st_block_ctrl_array[LCD_SUPPORT_MAX][BLOCK_SUM];

LOCAL ISP_OSD_PARAM_T s_isp_osd_param={0x00};

/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
LOCAL uint32 CHECK_LCD_BLOCK_VAILD(uint32 lcd_id, uint32 blk_num) 
{
    if(!((lcd_id < LCD_GetUsedNum()) && (blk_num < BLOCK_SUM))) 
    { 
        //DC_LOG_TRACE:"\nLCD:%d block:%d not supoort"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_81_112_2_18_1_23_30_114,(uint8*)"dd", lcd_id, blk_num);
        return ERR_LCD_PARAMETER_WRONG;
    }
    return ERR_LCD_NONE;
}

LOCAL uint32 CHECK_LCD_BLOCK_OWNER(uint32 lcd_id, uint32 blk_num) 
{
    if(SCI_InThreadContext())
    {
        if(SCI_IdentifyThread() != s_st_block_ctrl_array[lcd_id][blk_num].owner_id)
        {
            //DC_LOG_TRACE:"\n The caller thread must get mutex first"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_93_112_2_18_1_23_30_115,(uint8*)"");
            return ERR_LCD_PARAMETER_WRONG;
        }
    }
    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_SetBlockBuffer(
                                     LCD_ID_E lcd_id, 
                                     uint32 blk_num, 
                                     uint32 *buf_ptr
                                   )
{
    //DC_LOG_TRACE:"LCDC_SetBlockBuffer, buf_ptr 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_106_112_2_18_1_23_30_116,(uint8*)"d", buf_ptr);

	CHECK_LCD_BLOCK_VAILD(lcd_id, blk_num);
    
    CHECK_LCD_BLOCK_OWNER(lcd_id, blk_num);

    s_buffer_info[lcd_id][blk_num].mem_ptr = buf_ptr;

    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_EnableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
    //DC_LOG_TRACE:"LCDC_EnableBlock"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_119_112_2_18_1_23_31_117,(uint8*)"");

	CHECK_LCD_BLOCK_VAILD(lcd_id, blk_num);

    s_st_block_ctrl_array[lcd_id][blk_num].is_enable = SCI_TRUE;
    
    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_DisableBlock(LCD_ID_E lcd_id, uint32 blk_num)
{
    //DC_LOG_TRACE:"LCDC_DisableBlock"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_130_112_2_18_1_23_31_118,(uint8*)"");

	CHECK_LCD_BLOCK_VAILD(lcd_id, blk_num);

    s_st_block_ctrl_array[lcd_id][blk_num].is_enable = SCI_FALSE;

    return ERR_LCD_NONE;
}

PUBLIC uint32 *LCDC_GetBlockBuffer(LCD_ID_E lcd_id, uint32 blk_num)
{
    uint32 *buf_ptr = PNULL;

    //DC_LOG_TRACE(("LCDC_GetBlockBuffer"));

	CHECK_LCD_BLOCK_VAILD(lcd_id, blk_num);

    buf_ptr = s_buffer_info[lcd_id][blk_num].mem_ptr;

    //DC_LOG_TRACE(("LCDC_GetBlockBuffer, buf_ptr 0x%x", buf_ptr));
    
    return buf_ptr;
}

PUBLIC ERR_LCD_E LCDC_ConfigBlock(
								  LCD_ID_E lcd_id, 
								  uint32 blk_num, 
								  BLOCKCFG_T config
								)
{
    //DC_LOG_TRACE:"LCDC_ConfigBlock"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_160_112_2_18_1_23_31_119,(uint8*)"");

	CHECK_LCD_BLOCK_VAILD(lcd_id, blk_num);

    CHECK_LCD_BLOCK_OWNER(lcd_id, blk_num);

    s_buffer_info[lcd_id][blk_num].cfg = config;

    return ERR_LCD_NONE;
}

PUBLIC ERR_LCD_E LCDC_GetConfigBlock(
									 LCD_ID_E lcd_id, 
									 uint32 blk_num, 
									 BLOCKCFG_T *cfg_ptr
								   )
{
    //DC_LOG_TRACE(("LCDC_GetConfigBlock"));
    CHECK_LCD_BLOCK_VAILD(lcd_id, blk_num);

    *cfg_ptr = s_buffer_info[lcd_id][blk_num].cfg;

	return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:    get the control of special block of special LCD
//	Global resource dependence: 
//  Author:         jing.li
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCDC_GetBlockCtrl(LCD_ID_E lcd_id, uint32 block_id)
{
    if(!SCI_InThreadContext())
        return ERR_LCD_NONE;

    CHECK_LCD_BLOCK_VAILD(lcd_id, block_id);

    /*
     * there is a hint danger, no protect for the mutex create
     * we can't disabel irq here, because the function is often called
     * by isp in preview or record.
     */
    if(!s_st_block_ctrl_array[lcd_id][block_id].block_mutex_ptr){
        if(PNULL != s_block_mutex_string_array[lcd_id][block_id]){
                s_block_mutex_string_array[lcd_id][block_id] = "LCD_BLOCK_MUTEX_DEFAULT";
        }
        s_st_block_ctrl_array[lcd_id][block_id].block_mutex_ptr = 
            SCI_CreateMutex(s_block_mutex_string_array[lcd_id][block_id], SCI_NO_INHERIT);
        s_st_block_ctrl_array[lcd_id][block_id].block_mutex_string = (char *)s_block_mutex_string_array[lcd_id][block_id];
    }

    SCI_GetMutex(s_st_block_ctrl_array[lcd_id][block_id].block_mutex_ptr, SCI_WAIT_FOREVER);
    s_st_block_ctrl_array[lcd_id][block_id].owner_id = SCI_IdentifyThread();

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:    release the control of special block of special LCD
//	Global resource dependence: 
//  Author:         jing.li
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCDC_ReleaseBlockCtrl(LCD_ID_E lcd_id, uint32 block_id)
{
    if(!SCI_InThreadContext())
        return ERR_LCD_NONE;

    CHECK_LCD_BLOCK_VAILD(lcd_id, block_id);

    CHECK_LCD_BLOCK_OWNER(lcd_id, block_id);
    
    if(!s_st_block_ctrl_array[lcd_id][block_id].block_mutex_ptr)
    {
        SCI_PASSERT(0, ("\nRelease LCD Block mutex is null"));/*assert verified*/
    }
    else{
        s_st_block_ctrl_array[lcd_id][block_id].owner_id = 0xffffffff;
        SCI_PutMutex(s_st_block_ctrl_array[lcd_id][block_id].block_mutex_ptr);
    }

    return ERR_LCD_NONE;
}

/*****************************************************************************/
//  Description:    get the state of special block of special LCD (whether it is enabled)
//	Global resource dependence: 
//  Author:         jing.li
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN LCDC_GetBlockState(LCD_ID_E lcd_id, uint32 block_id)
{
    CHECK_LCD_BLOCK_VAILD(lcd_id, block_id);

    return s_st_block_ctrl_array[lcd_id][block_id].is_enable;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Tim.zhu
//	Note:
/*****************************************************************************/
PUBLIC ISP_OSD_PARAM_T* LCDC_GetOsdBlockRect(void)
{
   // DC_LOG_TRACE(("LCDC_GetOsdBlockRect"));
    
    return &s_isp_osd_param; 
}



/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Tim.zhu
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCDC_SetOsdBlockRect(ISP_OSD_PARAM_T *  osd_param_ptr)
{
    //DC_LOG_TRACE:"LCDC_SetOsdBlockRect"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_271_112_2_18_1_23_31_120,(uint8*)"");

    SCI_MEMCPY((void*)(&s_isp_osd_param),(void*)(osd_param_ptr),sizeof(ISP_OSD_PARAM_T));

    s_isp_osd_param.enable=SCI_FALSE;    

    return ERR_LCD_NONE;
}
/*****************************************************************************/
//  Description:   
//	Global resource dependence: 
//  Author:         Tim.zhu
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCDC_OsdBlockRectEnable(void)
{
    //DC_LOG_TRACE:"LCDC_OsdBlockRectEnable"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_285_112_2_18_1_23_31_121,(uint8*)"");

    s_isp_osd_param.enable=SCI_TRUE;

    return ERR_LCD_NONE;
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Tim.zhu
//	Note:
/*****************************************************************************/
PUBLIC ERR_LCD_E LCDC_OsdBlockRectDisable(void)
{
    //DC_LOG_TRACE:"LCDC_OsdBlockRectDisable"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,DC_LCDC_297_112_2_18_1_23_31_122,(uint8*)"");

    s_isp_osd_param.enable=SCI_FALSE;       

    SCI_MEMSET((void*)(&s_isp_osd_param),0x00,sizeof(ISP_OSD_PARAM_T));

    s_isp_osd_param.enable=SCI_FALSE;    

    return ERR_LCD_NONE;
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif// End
