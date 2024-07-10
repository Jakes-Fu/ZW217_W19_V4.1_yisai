/******************************************************************************
 ** File Name:    ldo_reg_v0.h                                            *
 ** Author:       Steve.Zhan                                                 *
 ** DATE:         06/05/2010                                                  *
 ** Copyright:    2010 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/05/2010    Steve.Zhan      Create.                                     *
 ******************************************************************************/
#ifndef _LDO_REG_V0_H_
#define _LDO_REG_V0_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **------------------------------------------------------------------------- */

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
/* save value for register */
#define LOCAL_VAR_DEF   uint32 reg_val;

/* set or clear the bit of register in reg_addr address */
#define REG_SETCLRBIT(_reg_addr, _set_bit, _clr_bit)    \
    do  \
    {   \
        reg_val = *(volatile uint32*)(_reg_addr);   \
        reg_val |= (_set_bit);  \
        reg_val &= ~(_clr_bit); \
        *(volatile uint32*)(_reg_addr) = reg_val;   \
    }while(0);

/* set the value of register bits corresponding with bit_mask */
#define REG_SETBIT(_reg_addr, _bit_mask, _bit_set) \
    do  \
    {   \
        reg_val = *(volatile uint32*)(_reg_addr);  \
        reg_val &= ~(_bit_mask);    \
        reg_val |= ((_bit_set) & (_bit_mask));  \
        *(volatile uint32*)(_reg_addr) = reg_val;   \
    }while(0);

/* macro used to get voltage level(one bit level) */
#define GET_LEVELBIT(_reg_addr, _bit_mask, _bit_set, _level_var)   \
    {   \
        (_level_var) |= \
                        ((*(volatile uint32*)(_reg_addr)&(_bit_mask))?(_bit_set):0);    \
    }

/* macro used to get voltage level */
#define GET_LEVEL(_reg_addr, _bit0_mask, _bit1_mask, _level_var)    \
    {   \
        (_level_var) |= \
                        (((*(volatile uint32*)(_reg_addr)&(_bit0_mask))?(BIT_0):0)|    \
                         ((*(volatile uint32*)(_reg_addr)&(_bit1_mask))?(BIT_1):0)); \
    }

/* macro used to set voltage level according to bit field */
#define SET_LEVELBIT(_reg_addr, _bit_mask, _set_bit, _rst_bit)  \
    {   \
        REG_SETBIT( \
                    _reg_addr,  \
                    (_set_bit) | (_rst_bit),    \
                    ((_set_bit)&(_bit_mask)) | ((_rst_bit)&(~_bit_mask))    \
                  )   \
    }

/* macro used to set voltage level according to two bits */
#define SET_LEVEL(_reg_addr, _bit0_mask, _bit1_mask, _set_bit0,     \
                  _rst_bit0, _set_bit1, _rst_bit1)    \
{   \
    REG_SETBIT( \
                (_reg_addr),    \
                ((_set_bit0)|(_rst_bit0) | (_set_bit1)|(_rst_bit1)),  \
                (((_set_bit0)&(_bit0_mask)) | ((_rst_bit0)&(~_bit0_mask))|  \
                 ((_set_bit1)&(_bit1_mask)) | ((_rst_bit1)&(~_bit1_mask)))   \
              )   \
}

#define LDO_REG_OR CHIP_REG_OR
#define LDO_REG_AND CHIP_REG_AND
#define LDO_REG_GET CHIP_REG_GET

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
