/******************************************************************************
 ** File Name:    gea_reg_v0.h                                            *
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
#ifndef _GEA_REG_V0_H_
#define _GEA_REG_V0_H_
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
/*----------GEA Interface Control Registers----------*/
#define GEA_START_CMD                   (GEA_BASE + 0x0000)
#define GEA_COUNT                       (GEA_BASE + 0x0008)
#define GEA_FRESH                       (GEA_BASE + 0x000c)
#define GEA_KC0                         (GEA_BASE + 0x0010)
#define GEA_KC1                         (GEA_BASE + 0x0014)
#define GEA_KC2                         (GEA_BASE + 0x0018)
#define GEA_KC3                         (GEA_BASE + 0x001c)
#define GEA_POLL_REG                    (GEA_BASE + 0x0020)
#define GEA_OUT1                        (GEA_BASE + 0x0028)
#define GEA_OUT2                        (GEA_BASE + 0x002c)
#define GEA_12_OUT                      (GEA_BASE + 0x0034)

#define MODE_GEA1           5
#define MODE_GEA2           6
#define MODE_GEA3           3
#define MODE_KSMI           0
#define GEA_RESET           BIT_31
#define GEA_NEXT            BIT_5
#define GEA1_DIR_BIT        15
#define GEA2_DIR_BIT        15
#define GEA3_DIR_BIT        0
#define GEA_NOT_COMPLETE    0
#define GEA_COMPLETE        1

typedef struct GEA_REG_CFG_TAG  //8d00_0000
{
    volatile uint32 ctrl;       //- 0
    volatile uint32 reserve1;   //- 4
    volatile uint32 count;      //- 8
    volatile uint32 fresh;      //- c
    volatile uint32 kc0;        //-10
    volatile uint32 kc1;        //-14
    volatile uint32 kc2;        //-18
    volatile uint32 kc3;        //-1c
    volatile uint32 poll;       //-20
    volatile uint32 reserve2;   //-24
    volatile uint32 kasmi_out1; //-28
    volatile uint32 kasmi_out2; //-2c
    volatile uint32 reserve3;   //-30
    volatile uint32 gea_out;    //-34
}
GEA_REG_CFG_T;
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
