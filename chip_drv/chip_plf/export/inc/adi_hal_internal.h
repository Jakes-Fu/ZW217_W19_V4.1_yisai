/******************************************************************************
 ** File Name:      adi_hal_internal.h                                                 *
 ** Author:         tim.luo                                             *
 ** DATE:           2/25/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                       *
 **                                                                                             *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 2/25/2010     Tim Luo      Create.                                   *
 **                                                                                                *
 ******************************************************************************/


#ifndef _ADI_HAL_INTERNAL_H_
#define  _ADI_HAL_INTERNAL_H_

PUBLIC  void ADI_Analogdie_reg_and (uint32 addr, uint16 data);

PUBLIC  void ADI_Analogdie_reg_mask_or (uint32 addr, uint16 data, uint16 msk);

PUBLIC  void ADI_Analogdie_reg_or (uint32 addr, uint16 data);

PUBLIC uint16 ADI_Analogdie_reg_read (uint32 addr);

PUBLIC void ADI_Analogdie_reg_write (uint32 addr, uint16 data);

PUBLIC BOOLEAN ADI_IS_Analogdie_reg (uint32 addr);

PUBLIC void ADI_init (void);

#ifdef FPGA_TEST

PUBLIC uint16 ADI_Analogdie_reg_read_AUDIO (uint32 addr);
PUBLIC void ADI_Analogdie_reg_write_AUDIO (uint32 addr, uint16 data);

#define ANA_REG_OR_AUDIO(reg_addr, value)     \
    do{\
        uint16 adi_tmp_val = ADI_Analogdie_reg_read(reg_addr); \
        adi_tmp_val |= (uint16)(value); \
        ADI_Analogdie_reg_write_AUDIO(reg_addr, adi_tmp_val); \
    }while(0)

#define ANA_REG_MSK_OR_AUDIO(reg_addr, value, msk)        \
    do{\
        uint16 adi_tmp_val = ADI_Analogdie_reg_read_AUDIO(reg_addr); \
        adi_tmp_val &= (uint16)(~(msk)); \
        adi_tmp_val |= (uint16)((value)&(msk)); \
        ADI_Analogdie_reg_write_AUDIO(reg_addr, adi_tmp_val); \
    }while(0)

#define ANA_REG_SET_AUDIO(reg_addr, value)    ADI_Analogdie_reg_write_AUDIO(reg_addr, (uint16)(value))
#define ANA_REG_GET_AUDIO(reg_addr)           ADI_Analogdie_reg_read_AUDIO(reg_addr)

#endif

///for analog die register operation
#define ANA_REG_OR(reg_addr, value)  ADI_Analogdie_reg_or(reg_addr, value)
#define ANA_REG_MSK_OR(reg_addr, value, msk) ADI_Analogdie_reg_mask_or(reg_addr, value, msk)
#define ANA_REG_AND(reg_addr, value)   ADI_Analogdie_reg_and(reg_addr, value)

#define ANA_REG_SET(reg_addr, value)    ADI_Analogdie_reg_write(reg_addr, (uint16)(value))

#define ANA_REG_GET(reg_addr)           ADI_Analogdie_reg_read(reg_addr)
#define ANA_IS_ANA_REG(reg_addr)      ADI_IS_Analogdie_reg(reg_addr)


#endif  //_ADI_HAL_INTERNAL_H_