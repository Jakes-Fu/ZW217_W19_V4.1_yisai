/******************************************************************************
 ** File Name:      vbc_phy.h                                                 *
 ** Author:         ken.kuang                                                 *
 ** DATE:           12/12/2011                                                *
 ** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    Rigister definition for physical layer.                   *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 12/12/2011     ken.kuang          Create.                                 *
 *****************************************************************************/
#ifndef _VBC_PHY_H_
#define _VBC_PHY_H_

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro defination                                  *
 **---------------------------------------------------------------------------*/
#define IIS_PORT_NORMAL         (0)
#define IIS_PORT_FM             (1)

#define DAPATH_NO_MIX           (0)
#define DAPATH_ADD              (1)
#define DAPATH_SUB              (2)

#define ADPATH_NORMAL           (0)
#define ADPATH_FROM_ALC         (1)

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC void     __vbc_da_enable(uint32 en,uint32 chan);
PUBLIC void     __vbc_ad_enable(uint32 en,uint32 chan);
PUBLIC void     __vbc_dma_da_chn_en(uint32 en,uint32 chan);
PUBLIC void     __vbc_dma_ad_chn_en(uint32 en,uint32 chan);
PUBLIC void     __vbc_arm_acc_switch(uint32 is_arm);
PUBLIC int32    __vbc_set_buffer_size(uint32 da_buf_size,uint32 ad_buf_size);
PUBLIC void     __vbc_clear_da_buffer(void);
PUBLIC void     __vbc_iis_high_for_da0(uint32 en);
PUBLIC void     __vbc_init(void);
PUBLIC void     __vbc_reset(void);
PUBLIC void     __vbc_open(void);
PUBLIC void     __vbc_close(void);
PUBLIC void     __vbc_disable(void);
PUBLIC void     __vbc_enable(void);
PUBLIC void     __vbc_write(int16 *l_ptr, int16 *r_ptr,uint16 size);
PUBLIC void     __vbc_write_z(uint16 size);
PUBLIC uint32   __vbc_is_da_int(void);
PUBLIC uint32   __vbc_is_ad_int(void);
PUBLIC void     __vbc_clr_da_int(void);
PUBLIC void     __vbc_clr_ad_int(void);
PUBLIC void     __vbc_dac_sel_iis(uint32 port);
PUBLIC void     __vbc_adc_sel_iis(uint32 port);
PUBLIC void     __vbc_dac_fm_mix(uint32 mode,uint32 chan);
PUBLIC void     __vbc_adc_dgmux(uint32 mode,uint32 chan);
PUBLIC int32    __vbc_dma_da_chan_cfg(DMA_CHNCFG_T * pCfg,uint32 chan);
PUBLIC int32    __vbc_dma_ad_chan_cfg(DMA_CHNCFG_T * pCfg,uint32 chan);
PUBLIC int32    __vbc_dma_dac_chan_en(DMA_CHNCFG_T * pCfg,uint32 chan_id);
PUBLIC int32    __vbc_dma_adc_chan_en(DMA_CHNCFG_T * pCfg,uint32 chan_id);
PUBLIC void     _vbc_notch_enable(uint32 enable,uint32 fs);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_VBC_PHY_H_
