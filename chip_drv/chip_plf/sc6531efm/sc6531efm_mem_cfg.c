/******************************************************************************
 ** File Name:    sc6531efm_mem_cfg.c                                            *
 ** Author:       steve.zhan                                                 *
 ** DATE:         07/27/2010                                                  *
 ** Copyright:    2010  Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **07/27/2010     Steve.zhan      Create.                                     *
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */
#include "sci_types.h"
#include "sc6531efm_reg_base.h"
#include "../../../MS_Ref/export/inc/iram_mgr.h"
#include "sc6531efm_mem_cfg.h"
#include "mem_dump.h"
/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                        Local variable definition                           **
**----------------------------------------------------------------------------*/
const MEMORY_ACCESS_INFO_T s_memory_acc_table[ ] =
{
    /*------------------------------------------------------------------------------------------------------------
    *start_address              end_address                 acc_condtion                                memory name
    ----------------------------------------------------------------------------------------------------------------*/
    {0x00000000,          0xFFFFFFFF, (READ_ACC|WRITE_ACC|BYTE_ACC|WORD_ACC|DWORD_ACC) },             //the RAM memory
};


LOCAL const IRAM_AREA g_init_iram_area[] = {0x40000000, 0x00002800, SCI_NULL, IRAM_ATTRIBUTE_NORMAL};

/*
    On Chip RAM for DSP 
*/
LOCAL const IRAM_AREA g_iram_dsp[] = {
                                        {0x4001B000, 0x00008000, SCI_NULL, IRAM_ATTRIBUTE_DSP},
                                        {0x40023000, 0x0000C000, SCI_NULL, IRAM_ATTRIBUTE_DSP}
                                     };


PUBLIC const CTL_REG_INFO_T g_CtlRegInfoTab[]=
{
    {EXTERNAL_MEM_CTL_BEGIN,EXTERNAL_MEM_CTL_END},
    {DMA_GEN_CTL_BEGIN,DMA_GEN_CTL_END},
    {DMA_CHA_CTL_BEGIN,DMA_CHA_CTL_END},
    {DCAM_CTL_BEGIN,DCAM_CTL_END},
    {USB_CTL_BEGIN,USB_CTL_END},
    {ROTATION_CTL_BEGIN,ROTATION_CTL_END},
    {BUS_MON0_CTL_BEGIN,BUS_MON0_CTL_END},
    {AHB_GEN_CTL_BEGIN,AHB_GEN_CTL_END},
    {CHIP_ID_BEGIN,CHIP_ID_END},
    {SDIO_SLOT0_CTL_BEGIN,SDIO_SLOT0_CTL_END},
    {SDIO_SLOT1_CTL_BEGIN,SDIO_SLOT1_CTL_END},
    {LCM_CTL_BEGIN,LCM_CTL_END},
    {SFC_CTL_BEGIN,SFC_CTL_END},
    {MIDI_CTL_BEGIN,MIDI_CTL_END},
    {LCDC_CTL_BEGIN,LCDC_CTL_END},
    {LZMA_CTL_BEGIN,LZMA_CTL_END},
    {SPI0_CTL_BEGIN,SPI0_CTL_END},
    
    {INT_CTL_BEGIN,INT_CTL_END},
    {INT1_CTL_BEGIN,INT1_CTL_END},
    {TIMER_CNT_BEGIN,TIMER_CNT_END},
    {BACKUP_TIMER_CNT_BEGIN,BACKUP_TIMER_CNT_END},
    {SYS_CNT_BEGIN,SYS_CNT_END},
    {ADI_CTL_BEGIN,ADI_CTL_END},
    {EFUSE_CTL_BEGIN,EFUSE_CTL_END},
    {VOICE_BAND_CODEC_BEGIN,VOICE_BAND_CODEC_END},
    {UART0_CTL_BEGIN,UART0_CTL_END},
    {UART1_CTL_BEGIN,UART1_CTL_END},
    {SIM0_CTL_BEGIN,SIM0_CTL_END},
    {I2C_CTL_BEGIN,I2C_CTL_END},
    {I2C1_CTL_BEGIN,I2C1_CTL_END},
    {KEYPAD_CTL_BEGIN,KEYPAD_CTL_END},
    {PWM_CTL_BEGIN,PWM_CTL_END},
    {GPIO_CTL_BEGIN,GPIO_CTL_END},
    {EIC_CTL_BEGIN,EIC_CTL_END},
    {AUD_CTL_BEGIN,AUD_CTL_END},
    {GLOBAL_CTL_BEGIN,GLOBAL_CTL_END},
    {AON_CTL_BEGIN,AON_CTL_END},
    {CHIPPIN_CTL_BEGIN,CHIPPIN_CTL_END},
    {GEA_CTL_BEGIN,GEA_CTL_END},
    {SPI1_CTL_BEGIN,SPI1_CTL_END},
    {IIS_CTL_BEGIN,IIS_CTL_END},

    //A-Die todo
    {ADIE_PWM_REG_START,ADIE_PWM_REG_END},
    {ADIE_INT_CTRL_START,ADIE_INT_CTRL_END},
    {WATDOG_CTL_BEGIN,WATDOG_CTL_END},
    {ADIE_CAL_REG_START,ADIE_CAL_REG_END},
    {ADIE_AUD_IF_REG_START,ADIE_AUD_IF_REG_END},
    {ADIE_PIN_REG_START,ADIE_PIN_REG_END},
    {ADIE_PTEST_REG_START,ADIE_PTEST_REG_END},
    {RTC_CTL_BEGIN,RTC_CTL_END},
    {AUXADC_CTL_BEGIN,AUXADC_CTL_END},
    {ADIE_AUDIO_REG_START,ADIE_AUDIO_REG_END},
    {ADIE_GLB_REG_START,ADIE_GLB_REG_END},

    {0xffffffff,0xffffffff}
};

/**----------------------------------------------------------------------------*
**                        Global variable definition                          **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
PUBLIC MEMORY_ACCESS_INFO_T *Mem_getAccTable (uint32 *pSize)
{
    *pSize = sizeof (s_memory_acc_table) /sizeof (s_memory_acc_table[0]);

    return (MEMORY_ACCESS_INFO_T *) s_memory_acc_table;
}

/*****************************************************************************/
//  Description:    Get IRAM information used by DSP
//  Global resource dependence:
//  Author:
//  Note:           
/*****************************************************************************/
PUBLIC IRAM_AREA* MEM_GetIRAMDsp (uint32 *count)
{
    // return registered area num by input param
    *count = sizeof(g_iram_dsp[0]) /sizeof (IRAM_AREA);

    return ( (IRAM_AREA *) g_iram_dsp);
}

/*****************************************************************************/
//  Description:    Get IRAM information used by ARM
//  Global resource dependence:
//  Author:
//  Note:           return num by input param and return the const array pointer
/*****************************************************************************/
PUBLIC IRAM_AREA *GetIRAMSet (uint32 *count)
{
    // return registered area num by input param
    *count = sizeof (g_init_iram_area) /sizeof (IRAM_AREA);

    return ( (IRAM_AREA *) g_init_iram_area);
}

/*****************************************************************************/
// Description :    This function is used to get the number of control registers group.
// Global resource dependence :
// Author :         kevin.wang
// Note :
/*****************************************************************************/
PUBLIC uint32 CHIP_GetNumCtlRegGrp (void)
{
    uint32 CtlRegInfoTabSize = sizeof (g_CtlRegInfoTab) / sizeof (g_CtlRegInfoTab[0]);
    return CtlRegInfoTabSize - 1;
}

/*****************************************************************************/
// Description :    This function is used to get the infomation of control
//                  registers,including the base address,the length and the
//                  name of External Memory Control Registers and AHB General
//                  Control Registers,DMA Control Register,Digital Camera Space,
//                  USB Device Space,NAND Flash Control and LCM Control Space,
//                  Peripheral Address Space.
// Global resource dependence :g_CtlRegInfoTab
// Author :         kevin.wang
// Note : the unit of length is a word(32bit).
/*****************************************************************************/
PUBLIC void const *CHIP_GetInfoOfCtlReg (void)
{
    return (void *) g_CtlRegInfoTab;
}
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
