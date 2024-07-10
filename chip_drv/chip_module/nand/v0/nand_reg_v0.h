/******************************************************************************
 ** File Name:    nand_reg_v0.h                                            *
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
#ifndef _NAND_REG_V0_H_
#define _NAND_REG_V0_H_
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

//Nand Controller register
#define NFC_MBUF_ADDR      (NAND_MEM_BASE + 0x0000)
#define NFC_SBUF_ADDR      (NAND_MEM_BASE + 0x0c00)
#define NFC_WPN            (NAND_CTL_BASE + 0x0038)
#define NFC_CMD            (NAND_CTL_BASE + 0x0000)
#define NFC_STR0           (NAND_CTL_BASE + 0x0004)
#define NFC_STR1           (NAND_CTL_BASE + 0x0008)
#define NFC_STR2           (NAND_CTL_BASE + 0x000C)
#define NFC_STR3           (NAND_CTL_BASE + 0x0010)
#define NFC_END0           (NAND_CTL_BASE + 0x0014)
#define NFC_END1           (NAND_CTL_BASE + 0x0018)
#define NFC_END2           (NAND_CTL_BASE + 0x001C)
#define NFC_END3           (NAND_CTL_BASE + 0x0020)
#define NFC_PARA           (NAND_CTL_BASE + 0x0024)
#define NFC_ID_STATUS      (NAND_CTL_BASE + 0x003C)
#define NFC_CMD_CLR        (NAND_CTL_BASE + 0x0048)
#define NFC_RB             (NAND_CTL_BASE + 0x0080)
#define NFC_ECC_EN         (NAND_CTL_BASE + 0x0100)
#define NFC_TIMEOUT        (NAND_CTL_BASE + 0x0118)
#define NFC_ENDIAN         (NAND_CTL_BASE + 0x0140)
#define NFC_CMD_SET        (NAND_CTL_BASE + 0x0144)

//INT CTL
#define NFC_INT_SRC        (NAND_CTL_BASE + 0x0084)
#define NFC_INT_EN         (NAND_CTL_BASE + 0x0088)
#define NFC_INT_MSK        (NAND_CTL_BASE + 0x008C)

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/


//Nand control register
typedef struct nand_tag
{
    VOLATILE uint32 cmd;
    VOLATILE uint32 start_addr0;
    VOLATILE uint32 start_addr1;
    VOLATILE uint32 start_addr2;
    VOLATILE uint32 start_addr3;
    VOLATILE uint32 end_addr0;
    VOLATILE uint32 end_addr1;
    VOLATILE uint32 end_addr2;
    VOLATILE uint32 end_addr3;
    VOLATILE uint32 para;
    VOLATILE uint32 ecc0;
    VOLATILE uint32 ecc1;
    VOLATILE uint32 ecc2;
    VOLATILE uint32 ecc3;
    VOLATILE uint32 wpn;
    VOLATILE uint32 idstatus;
} NAND_CTL_REG_T;
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
