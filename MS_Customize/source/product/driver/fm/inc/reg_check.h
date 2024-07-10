#ifndef _REG_CHECK_H_
#define _REG_CHECK_H_

#include "sci_types.h"

/******************************************************************************
 **  Important: register test process graph
 **  -------------------------------------------------------------------
 **  |     |          type                 |    READ     |    WRITE    |
 **  |  1  |    READ_DEFAULT_BEFORE_INIT   |     Y       |      Y      |
 **  |  2  |    NORMAL                     |     Y       |      Y      |
 **  |  3  |    SPECIAL_RW                 |     Y       |      Y      |
 **  |  4  |    NON_RW                     |     Y       |      N      |
 **  |  5  |    NON_READ_DEFAULT           |     N       |      N      |
 **  -------------------------------------------------------------------
 **  NOTE: "READ_DEFAULT_BEFORE_INIT": read test before bus init
 **                                    write test after bus init
 **        "NORMAL" : read and write test after bus init
 **        "SPECIAL_RW": read test after bus init
 **                      write test after some protect operation,such as MCU_PROT
 **        "NON_RW" : read test after bus init
 **                   write test is neglected
 **        "NON_READ_DEFAULT" : read and write test is neglected
 ******************************************************************************/
typedef enum
{
    READ_DEFAULT_BEFORE_INIT = 0,     //read before bus init
    NORMAL,                           //normal read and write
    SPECIAL_RW,                       //rw need some special operation or
    // may change other reg
    NON_RW,                           //not to rw
    NON_READ_DEFAULT                  //not to read default and rw
} REG_TYPE_E;

typedef struct
{
    uint32 reg_addr;         //address
    uint32 reg_dvalue_mask;  //bit mask
    uint32 reg_dvalue;       //default value
    uint32 reg_rw_mask;      //bit mask that can be both read and write
    uint32 reg_set_clr_mask; //set/clear mask bits
    REG_TYPE_E reg_type;         //such as r/w order
} REG_ELE_T;

/**************************************************************
    Function Propertype
***************************************************************/
//PUBLIC void REG_Read_Default_Before_Init (REG_ELE_T *reg_ele_ptr,uint32 len);
//PUBLIC void Print_Reg_Default_Before_Bus_Init (void);
PUBLIC void REG_Read_Default (REG_ELE_T *reg_ele_ptr,uint32 len);
PUBLIC void REG_Read_Write (REG_ELE_T *reg_list,uint32 list_len);
//PUBLIC void REG_SPECIAL_Read_Write (REG_ELE_T *reg_ele_ptr,uint32 len);
//PUBLIC void REG_Test_for_Module (REG_ELE_T *reg_ele_ptr,uint32 len,uint32 module_base_address);

#endif  //_REG_CHECK_H_ 
