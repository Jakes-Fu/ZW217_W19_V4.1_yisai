/******************************************************************************
 ** File Name:    eic_reg.h                                                   *
 ** Author:       lichd                                                       *
 ** DATE:         6/14/2011                                                   *
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
// EIC_DBNC contral registers address map : D-Die
#define EIC_DBNC_DATA                   ( 0x0000 )
#define EIC_DBNC_DMSK                   ( 0x0004 )
#define EIC_DBNC_INT_POL                ( 0x0014 )
#define EIC_DBNC_INT_EN                 ( 0x0018 )
#define EIC_DBNC_INT_RAWSTS             ( 0x001C )
#define EIC_DBNC_INT_MSKSTS             ( 0x0020 )
#define EIC_DBNC_INT_CLR                ( 0x0024 )
#define EIC_DBNC_TRG_CTRL               ( 0x0028 )
// EIC_DBNC channel control register:EIC0CTRL~EIC7CTRL
#define EIC_CHANNEL_CTRL(x)             ( 0x0040 + (x)*0x04)

// EIC_LATCH contral registers address map
#define EIC_LATCH_INT_EN                ( 0x0000 )
#define EIC_LATCH_INT_RAWSTS            ( 0x0004 )
#define EIC_LATCH_INT_MSKSTS            ( 0x0008 )
#define EIC_LATCH_INT_CLR               ( 0x000C )
#define EIC_LATCH_INT_POL               ( 0x0010 )
#define EIC_LATCH_INT_MODE              ( 0x0014 )

// EIC_ASYNC contral registers address map
#define EIC_ASYNC_INT_EN                ( 0x0000 )
#define EIC_ASYNC_INT_RAWSTS            ( 0x0004 )
#define EIC_ASYNC_INT_MSKSTS            ( 0x0008 )
#define EIC_ASYNC_INT_CLR               ( 0x000C )
#define EIC_ASYNC_INT_MODE              ( 0x0010 )
#define EIC_ASYNC_INT_BOTH              ( 0x0014 )
#define EIC_ASYNC_INT_POL               ( 0x0018 )
#define EIC_ASYNC_INT_DATA              ( 0x001C )

// EIC_SYNC contral registers address map
#define EIC_SYNC_INT_EN                 ( 0x0000 )
#define EIC_SYNC_INT_RAWSTS             ( 0x0004 )
#define EIC_SYNC_INT_MSKSTS             ( 0x0008 )
#define EIC_SYNC_INT_CLR                ( 0x000C )
#define EIC_SYNC_INT_MODE               ( 0x0010 )
#define EIC_SYNC_INT_BOTH               ( 0x0014 )
#define EIC_SYNC_INT_POL                ( 0x0018 )
#define EIC_SYNC_INT_DATA               ( 0x001C )

// EICA contral registers address map : A-Die
#define EICA_DBNC_DATA                   ( 0x0000 )
#define EICA_DBNC_DMSK                   ( 0x0004 )
#define EICA_DBNC_INT_POL                ( 0x0014 )
#define EICA_DBNC_INT_EN                 ( 0x0018 )
#define EICA_DBNC_INT_RSTS               ( 0x001C )
#define EICA_DBNC_INT_MSTS               ( 0x0020 )
#define EICA_DBNC_INT_CLR                ( 0x0024 )
#define EICA_DBNC_TRG_CTRL               ( 0x0028 )
// EICA channel control register:EICA0CTRL~EICA7CTRL
#define EICA_CHANNEL_CTRL(x)            ( 0x0040 + (x)*0x04)

// EIC bits field define:
// EICCTRL:
//        [31:16] : reserved 
//        [   15] : FORCE_CLK_DBNC
//        [   14] : DBNC_EN 
//        [13:12] : reserved
//        [11:00] : DBNC_CNT
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

