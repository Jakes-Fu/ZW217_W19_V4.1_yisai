/******************************************************************************
 ** File Name:    eic_reg_uws6121e.h                                                   *
 ** Author:       mei                                                       *
 ** DATE:         8/27/2018                                                   *
 ** Copyright:    2008 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 **                                    *
 ******************************************************************************/
#ifndef _EIC_REG_H_
#define _EIC_REG_H_
/*----------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **-------------------------------------------------------------------------- */

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif

/**----------------------------------------------------------------------------*
**                               Micro Define                                 **
**----------------------------------------------------------------------------*/
/*  EIC_DBNC contral registers address map */
#define EIC_DBNC_DATA           ( 0x0000 )
#define EIC_DBNC_DMSK           ( 0x0004 )
#define EIC_DBNC_INT_EV         ( 0x0014 )
#define EIC_DBNC_INT_EN		    ( 0x0018 )
#define EIC_DBNC_INT_RAWSTS     ( 0x001C )
#define EIC_DBNC_INT_MSKSTS     ( 0x0020 )
#define EIC_DBNC_INT_CLR        ( 0x0024 )
#define EIC_DBNC_TRG_CTRL       ( 0x0028 )
/* EIC_DBNC channel control register:EIC0CTRL~EIC7CTRL */
#define EIC_CHANNEL_CTRL(x)     ( 0x0040 + (x)*0x04)


/*  EIC bits field define:
 *  EICCTRL:
 *        [31:16] : reserved
 *        [   15] : FORCE_CLK_DBNC
 *        [   14] : DBNC_EN
 *        [13:12] : reserved
 *        [11:00] : DBNC_CNT
 */
#define EICCTRL_FORCE_CLK_DBNC          BIT_15
#define EICCTRL_DBNC_EN                 BIT_14
#define EICCTRL_DBNC_CNT_BITMSK         ( 0x00FFF )
#define EICCTRL_DBNC_CNT_BITPOS         ( 0 )

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
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

