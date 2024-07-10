/******************************************************************************
 ** File Name:    dma_reg_v5.h(r3p0)                                                *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         11/13/2005                                                  *
 ** Copyright:    2005 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 11/13/2005    Daniel.Ding     Create.                                     *
 ** 01/29/2007    Aiguo.Miao      Port to SC8800H                             *
 ** 05/02/2007    Tao.Zhou        Modify it for SC8800H.                      *
 ** 12/06/2010    Jeff.Li         Modify for new chip driver architecture.    *
 ******************************************************************************/
#ifndef _DMA_REG_V5_H_
#define _DMA_REG_V5_H_
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

#define DMA_CTL_REG_BASE                 (0x20100000)      


//------------------------------------------------------------------------------
//Register Name       : DMA_PAUSE
//Register Offset     : 0x0000
//Register Description: 
//    DMA_PAUSE_STATUS           : [16   ] DMA Pause status. 
//    DMA_PAUSE                  : [0    ] Active High, pause all the channels.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DMA_PAUSE_STATUS                  (BIT_16)
#define DMA_PAUSE                         (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : DMA_FRAG_WAIT
//Register Offset     : 0x0000
//Register Description: 
//    DMA_FRAG_WAIT              : [15:0 ] Fragment Wait Time.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DMA_FRAG_WAIT_MSK                 (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : DMA_INT_RAW_STATUS
//Register Offset     : 0x0000
//Register Description: 
//    DMA_INT_RAW_STATUS         : [31:0 ] DMA 32 Channels Interrupt Raw Status.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DMA_INT_RAW_STATUS_MSK            (0xUL)

//------------------------------------------------------------------------------
//Register Name       : DMA_INT_MASK_STATUS
//Register Offset     : 0x0000
//Register Description: 
//    DMA_INT_MASK_STATUS        : [31:0 ] DMA 32 Channels Interrupt Mask Status
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DMA_INT_MASK_STATUS_MSK           (0xUL)

//------------------------------------------------------------------------------
//Register Name       : DMA_REQ_STATUS
//Register Offset     : 0x0000
//Register Description: 
//    DMA_REQ_STATUS             : [31:0 ] DMA 32 Channels Request Status.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DMA_REQ_STATUS_MSK                (0xUL)

//------------------------------------------------------------------------------
//Register Name       : DMA_EN_STATUS
//Register Offset     : 0x0000
//Register Description: 
//    DMA_EN_STATUS              : [31:0 ] DMA 32 Channels Enable Status.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DMA_EN_STATUS_MSK                 (0xUL)

//------------------------------------------------------------------------------
//Register Name       : DMA_ERR_STATUS
//Register Offset     : 0x0000
//Register Description: 
//    DMA_ERR_STATUS             : [31:4 ] Reserved
//    ERR_UNALIGN                : [3    ] Error Un-align Word access configure.
//    ERR_REQ_MODE               : [2    ] Error Request mode Configure.
//    ERR_LEN                    : [1    ] Error Length Configure of the        
//                                  following:
//    ERR_ADDR_WRAP              : [0    ] Error Address Wrap Configure.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DMA_ERR_STATUS_SHIFT              (4UL)
#define DMA_ERR_STATUS_MSK                (0xFFFFFFFUL << DMA_ERR_STATUS_SHIFT)
#define ERR_UNALIGN                       (BIT_3)
#define ERR_REQ_MODE                      (BIT_2)
#define ERR_LEN                           (BIT_1)
#define ERR_ADDR_WRAP                     (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : DMA_DEBUG_STATUS
//Register Offset     : 0x0000
//Register Description: 
//    DMA_BUSY                   : [20   ] DMA Busy Status.
//    SRC_HREADY                 : [17   ] Source Side HREADY status.
//    DEST_HREADY                : [16   ] Destination Side HREADY status.
//    MAIN_FSM_STATUS            : [12:8 ] MAIN FSM status.
//    SRC_FSM_STATUS             : [7 :4 ] Source Side FSM status.
//    DEST_FSM_STATUS            : [3 :0 ] Destination Side FSM status.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DMA_BUSY                          (BIT_20)
#define SRC_HREADY                        (BIT_17)
#define DEST_HREADY                       (BIT_16)
#define MAIN_FSM_STATUS_SHIFT             (8UL)
#define MAIN_FSM_STATUS_MSK               (0x1FUL << MAIN_FSM_STATUS_SHIFT)
#define SRC_FSM_STATUS_SHIFT              (4UL)
#define SRC_FSM_STATUS_MSK                (0xFUL << SRC_FSM_STATUS_SHIFT)
#define DEST_FSM_STATUS_MSK               (0xFUL)

//------------------------------------------------------------------------------
//Register Name       : STD_CHN_PAUSE
//Register Offset     : 0x0000
//Register Description: 
//    CHN_PAUSE_STATUS           : [16   ] Channel Pause status. 
//    CHN_PAUSE                  : [0    ] Channel Pause Enable.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_PAUSE_STATUS                  (BIT_16)
#define CHN_PAUSE                         (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : STD_CHN_REQ
//Register Offset     : 0x0000
//Register Description: 
//    CHN_REQ                    : [0    ] Channel Request.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_REQ                           (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : STD_CHN_CFG
//Register Offset     : 0x0000
//Register Description: 
//    CHN_PRIORITY               : [13:12] Channel Request Priority. Four level 
//                                  of Priority supported as list : 
//    CHN_EN                     : [0    ] Channel Enable:
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_PRIORITY_SHIFT                (12UL)
#define CHN_PRIORITY_MSK                  (0x3UL << CHN_PRIORITY_SHIFT)
#define CHN_EN                            (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : STD_CHN_INT
//Register Offset     : 0x0000
//Register Description: 
//    CHN_CFG_ERR_INT_CLR        : [28   ] Error interrupt clear.
//    CHN_BLK_INT_CLR            : [25   ] Block done interrupt clear.
//    CHN_FRAGMENT_INT_CLR       : [24   ] Fragment done interrupt clear.
//    CHN_CFG_ERR_INT_MASK_STATUS: [20   ] Configuration error masked interrupt 
//                                  status.
//    CHN_CFG_ERR_INT_RAW_STATUS : [12   ] Configuration error raw interrupts   
//                                  status.
//    CHN_CFG_ERR_INT_EN         : [4    ] Configuration error interrupt enable.
//    CHN_BLK_INT_EN             : [1    ] Block done interrupt enable.
//    CHN_FRAGMENT_INT_EN        : [0    ] Fragment done interrupt enable.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_CFG_ERR_INT_CLR               (BIT_28)
#define CHN_BLK_INT_CLR                   (BIT_25)
#define CHN_FRAGMENT_INT_CLR              (BIT_24)
#define CHN_CFG_ERR_INT_MASK_STATUS       (BIT_20)
#define CHN_CFG_ERR_INT_RAW_STATUS        (BIT_12)
#define CHN_CFG_ERR_INT_EN                (BIT_4)
#define CHN_BLK_INT_EN                    (BIT_1)
#define CHN_FRAGMENT_INT_EN               (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : STD_CHN_SRC_ADDR
//Register Offset     : 0x0000
//Register Description: 
//    CHN_SRC_ADDR               : [31:0 ] Channel Source Address
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_SRC_ADDR_MSK                  (0xUL)

//------------------------------------------------------------------------------
//Register Name       : STD_CHN_DEST_ADDR
//Register Offset     : 0x0000
//Register Description: 
//    CHN_DEST_ADDR              : [31:0 ] Channel Destination Address
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_DEST_ADDR_MSK                 (0xUL)

//------------------------------------------------------------------------------
//Register Name       : STD_CHN_FRAG_LEN
//Register Offset     : 0x0000
//Register Description: 
//    SRC_SIZE                   : [31:30] Source AHB Size.
//    DEST_SIZE                  : [29:28] Destination AHB Size.
//    SWT_MODE                   : [27:26] Data Switch mode selection.
//    REQ_MODE                   : [25:24] Request mode, For Standard DMA       
//                                  channel: 
//    ADDR_FIX_SEL               : [21   ] Selection of the side using Address  
//                                  Fixed mode.
//    ADDR_FIX_EN                : [20   ] Address Fixed Enable:
//    FRG_LEN                    : [16:0 ] Fragment Length, with byte unit. ¡ê¡§  
//                                  Max 64K bytes¡ê?
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------


//------------------------------------------------------------------------------
//Register Name       : STD_CHN_BLK_LEN
//Register Offset     : 0x0000
//Register Description: 
//    BLK_CNT                    : [31:16] Block counter, update after each     
//                                  fragment done if REQ_MODE = 2?¡¥b00 (One     
//                                  request for a fragment). For other REQ_MODE 
//                                  configure, BLK_CNT would be always equal to 
//                                  ?¡ã0?¡À.
//    BLK_LEN                    : [15:0 ] Block Length, with byte unit. ¡ê¡§Max  
//                                  64K bytes¡ê?
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_PAUSE
//Register Offset     : 0x0000
//Register Description: 
//    CHN_PAUSE_STATUS           : [16   ] Channel Pause status. 
//    CHN_PAUSE                  : [0    ] Channel Pause Enable.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_PAUSE_STATUS                  (BIT_16)
#define CHN_PAUSE                         (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_REQ
//Register Offset     : 0x0000
//Register Description: 
//    CHN_REQ                    : [0    ] Channel Request.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_REQ                           (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_CFG
//Register Offset     : 0x0000
//Register Description: 
//    CHN_PRIORITY               : [13:12] Channel Request Priority. Four level 
//                                  of Priority supported as list : 
//    LLIST_CFG_VALID            : [8    ] If ?¡ãLLIST_EN?¡À asserted, active high
//                                   indicates Link list configure valid.
//    LLIST_EN                   : [4    ] Link list Enable, only available for 
//                                  full DMA channel.
//    CHN_EN                     : [0    ] Channel Enable:
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_PRIORITY_SHIFT                (12UL)
#define CHN_PRIORITY_MSK                  (0x3UL << CHN_PRIORITY_SHIFT)
#define LLIST_CFG_VALID                   (BIT_8)
#define LLIST_EN                          (BIT_4)
#define CHN_EN                            (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_INT
//Register Offset     : 0x0000
//Register Description: 
//    CHN_CFG_ERR_INT_CLR        : [28   ] Configuration error interrupt clear.
//    CHN_CFG_ERR_INT_MASK_STATUS: [20   ] Configuration error masked interrupt 
//                                  status.
//    CHN_CFG_ERR_INT_RAW_STATUS : [12   ] Configuration error raw interrupts   
//                                  status.
//    CHN_FRAGMENT_INT_RAW_STATUS: [8    ] Fragment done Raw interrupt status.
//    CHN_CFG_ERR_INT_EN         : [4    ] Configuration error interrupt enable.
//    CHN_BLK_INT_EN             : [1    ] Block done interrupt enable.
//    CHN_FRAGMENT_INT_EN        : [0    ] Fragment done interrupt enable.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_CFG_ERR_INT_CLR               (BIT_28)
#define CHN_CFG_LLIST_INT_CLR             (BIT_27)
#define CHN_CFG_TRSC_INT_CLR              (BIT_26)
#define CHN_CFG_BLK_INT_CLR               (BIT_25)
#define CHN_CFG_FRAG_INT_CLR              (BIT_24)
#define CHN_CFG_ERR_INT_MASK_STATUS       (BIT_20)
#define CHN_CFG_ERR_INT_RAW_STATUS        (BIT_12)
#define CHN_CFG_LLIST_INT_RAW_STATUS      (BIT_11)
#define CHN_CFG_TRSC_INT_RAW_STATUS       (BIT_10)
#define CHN_CFG_BLK_INT_RAW_STATUS        (BIT_9)
#define CHN_CFG_FRAGMENT_INT_RAW_STATUS   (BIT_8)
#define CHN_CFG_ERR_INT_EN                (BIT_4)
#define CHN_LLIST_INT_EN                  (BIT_3)
#define CHN_TRSC_INT_EN                   (BIT_2)
#define CHN_BLK_INT_EN                    (BIT_1)
#define CHN_FRAGMENT_INT_EN               (BIT_0)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_SRC_ADDR
//Register Offset     : 0x0000
//Register Description: 
//    CHN_SRC_ADDR               : [31:0 ] Channel Source Address
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_SRC_ADDR_MSK                  (0xUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_DEST_ADDR
//Register Offset     : 0x0000
//Register Description: 
//    CHN_DEST_ADDR              : [31:0 ] Channel Destination Address
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_DEST_ADDR_MSK                 (0xUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_FRAG_LEN
//Register Offset     : 0x0000
//Register Description: 
//    SRC_SIZE                   : [31:30] Source AHB Size.
//    DEST_SIZE                  : [29:28] Destination AHB Size.
//    SWT_MODE                   : [27:26] Data Switch mode selection.
//    REQ_MODE                   : [25:24] Request mode, For Full DMA channel: 
//    ADDR_WRAP_SEL              : [23   ] Selection of the side using Address  
//                                  wrap mode.
//    ADDR_WRAP_EN               : [22   ] Active High enable Address-wrapping  
//                                  for ring buffer. The next address of DMA    
//                                  jumps to ?¡ãWRAP_START_ADDR?¡À address when   
//                                  the current address matches ?¡ãWRAP_END_ADDR?
//                                  ?address: 
//    ADDR_FIX_SEL               : [21   ] Selection of the side using Address  
//                                  Fix mode.
//    ADDR_FIX_EN                : [20   ] Address Fix Enable:
//    LLIST_END                  : [19   ] Link list end flag. Active only when 
//                                  ?¡ãLLIST_EN?¡À asserted.
//    FRG_LEN                    : [16:0 ] Fragment Length, with byte unit. ¡ê¡§  
//                                  Max 128K bytes¡ê?
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define SRC_SIZE_SHIFT                    (30UL)
#define SRC_SIZE_MSK                      (0x3UL << SRC_SIZE_SHIFT)
#define DEST_SIZE_SHIFT                   (28UL)
#define DEST_SIZE_MSK                     (0x3UL << DEST_SIZE_SHIFT)
#define SWT_MODE_SHIFT                    (26UL)
#define SWT_MODE_MSK                      (0x3UL << SWT_MODE_SHIFT)
#define REQ_MODE_SHIFT                    (24UL)
#define REQ_MODE_MSK                      (0x3UL << REQ_MODE_SHIFT)
#define ADDR_WRAP_SEL                     (BIT_23)
#define ADDR_WRAP_EN                      (BIT_22)
#define ADDR_FIX_SEL                      (BIT_21)
#define ADDR_FIX_EN                       (BIT_20)
#define LLIST_END                         (BIT_19)
#define FRG_LEN_MSK                       (0x1FFFFUL)

//only for sc6530
#define SC6530_LLIST_END                         (BIT_31)
#define SC6530_SRC_SIZE_SHIFT                    (26UL)
#define SC6530_SRC_SIZE_MSK                      (0x3UL << SRC_SIZE_SHIFT)
#define SC6530_DEST_SIZE_SHIFT                   (24UL)
#define SC6530_DEST_SIZE_MSK                     (0x3UL << DEST_SIZE_SHIFT)
#define SC6530_SWT_MODE_SHIFT                    (22UL)
#define SC6530_SWT_MODE_MSK                      (0x3UL << SWT_MODE_SHIFT)
#define SC6530_REQ_MODE_SHIFT                    (20UL)
#define SC6530_REQ_MODE_MSK                      (0x3UL << REQ_MODE_SHIFT)
#define SC6530_ADDR_WRAP_SEL                     (BIT_19)
#define SC6530_ADDR_WRAP_EN                      (BIT_18)
#define SC6530_ADDR_FIX_SEL                      (BIT_17)
#define SC6530_ADDR_FIX_EN                       (BIT_16)
#define SC6530_FRG_LEN_MSK                       (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_BLK_LEN
//Register Offset     : 0x0000
//Register Description: 
//    BLK_CNT                    : [31:16] Block counter, update after each     
//                                  fragment done if REQ_MODE = 2?¡¥b00 (One     
//                                  request for a fragment). For other REQ_MODE 
//                                  configure, BLK_CNT would be always equal to 
//                                  ?¡ã0?¡À.
//    BLK_LEN                    : [16:0 ] Block Length, with byte unit. ¡ê¡§Max  
//                                  64K bytes¡ê?
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define BLK_CNT_SHIFT                     (17UL)
#define BLK_CNT_MSK                       (0xFFFFUL << BLK_CNT_SHIFT)
#define BLK_LEN_MSK                       (0x1FFFFUL)

//only for sc6530
#define SC6530_BLK_CNT_SHIFT                     (16UL)
#define SC6530_BLK_CNT_MSK                       (0xFFFFUL << BLK_CNT_SHIFT)
#define SC6530_BLK_LEN_MSK                       (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_TRSC_LEN
//Register Offset     : 0x0000
//Register Description: 
//    TRSC_LEN                   : [27:0 ] Transaction Length, with byte unit. (
//                                  Max 256M bytes)
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define TRSC_LEN_MSK                      (0xFFFFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_TRSF_STEP
//Register Offset     : 0x0000
//Register Description: 
//    DEST_TRSF_STEP             : [31:16] Channel Source side transfer step.
//    SRC_TRSF_STEP              : [15:0 ] Channel destination side transfer    
//                                  step.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DEST_TRSF_STEP_SHIFT              (16UL)
#define DEST_TRSF_STEP_MSK                (0xFFFFUL << DEST_TRSF_STEP_SHIFT)
#define SRC_TRSF_STEP_SHIFT               (0UL)
#define SRC_TRSF_STEP_MSK                 (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_WRAP_PTR
//Register Offset     : 0x0000
//Register Description: 
//    WRAP_PTR                   : [27:0 ] Wrap jump pointer address.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define WRAP_PTR_MSK                      (0xFFFFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_WRAP_TO
//Register Offset     : 0x0000
//Register Description: 
//    WRAP_TO                    : [27:0 ] Wrap jump-to address.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define WRAP_TO_MSK                       (0xFFFFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_LLIST_PTR
//Register Offset     : 0x0000
//Register Description: 
//    LLIST_PTR                  : [31:0 ] Link List pointer.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define LLIST_PTR_MSK                     (0xUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_FRAG_STEP
//Register Offset     : 0x0000
//Register Description: 
//    DEST_FRAG_STEP             : [31:16] Channel Source side transfer step.
//    SRC_FRAG_STEP              : [15:0 ] Channel destination side transfer    
//                                  step.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define DEST_FRAG_STEP_SHIFT              (16UL)
#define DEST_FRAG_STEP_MSK                (0xFFFFUL << DEST_FRAG_STEP_SHIFT)
#define SRC_FRAG_STEP_MSK                 (0xFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_SRC_BLK_STEP
//Register Offset     : 0x0000
//Register Description: 
//    CHN_SRC_BLK_STEP           : [27:0 ] Channel Source block step
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_SRC_BLK_STEP_MSK              (0xFFFFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : FULL_CHN_DEST_BLK_STEP
//Register Offset     : 0x0000
//Register Description: 
//    CHN_DEST_BLK_STEP          : [27:0 ] Channel Destination block step
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define CHN_DEST_BLK_STEP_MSK             (0xFFFFFFFUL)

//------------------------------------------------------------------------------
//Register Name       : REQ0_CID
//Register Offset     : 0x0000
//Register Description: 
//    REQ1_CID                   : [4 :0 ] REQ1 Channel Identified Number.
//------------------------------------------------------------------------------

//------------------------- Fields Definition ----------------------------------
#define REQ1_CID_MSK                      (0x1FUL)

#define DMA_PAUSE_ENABLE_BIT               (0x00000001)
#define DMA_PAUSE_STAT_BIT               (0x00010000)

//Channel x dma contral regisers address ;
#define DMA_CHx_CTL_REG_BASE(x)          (DMA_REG_BASE + 0x0400 + 0x20 * x )

//LL End
#define DMA_CHx_LL_END_MASK              (0x80000000)
#define DMA_CHx_LL_END_YES               (0x80000000)
#define DMA_CHx_LL_END_NO                (0x00000000)

//endaian type
#define DMA_CHx_ENDIANSEL_MASK           (0x40000000)
#define DMA_CHx_ENDIANSEL_BIG            (0x00000000)
#define DMA_CHx_ENDIANSEL_LITTLE         (0x40000000)

//Modify for SC8501C 

//----------------------------------------------------------------------------//
//Register Name       : XXX_CHN_FRAG_LEN
//Register Offset     : 0x0000
//Register Description: 
//----------------------------------------------------------------------------//
//src data width[31:30]
/*
2'b00 : Byte;
2'b01 : Half Word
2'b10 : Word
2'b11 : Forbidden.
*/
#define DMA_CHx_SRC_DATAWIDTH_MASK       (0xC0000000)
#define DMA_CHx_SRC_DATAWIDTH_BYTE       (0x00000000)
#define DMA_CHx_SRC_DATAWIDTH_HALFWORD   (0x40000000)
#define DMA_CHx_SRC_DATAWIDTH_WORD       (0x80000000)
#define DMA_CHx_SRC_DATAWIDTH_FORBIDDEN  (0xC0000000)

//only for sc6530
#define SC6530_DMA_CHx_SRC_DATAWIDTH_MASK       (0x0C000000)
#define SC6530_DMA_CHx_SRC_DATAWIDTH_BYTE       (0x00000000)
#define SC6530_DMA_CHx_SRC_DATAWIDTH_HALFWORD   (0x04000000)
#define SC6530_DMA_CHx_SRC_DATAWIDTH_WORD       (0x08000000)

//des data width[29:28]
#define DMA_CHx_DEST_DATAWIDTH_MASK      (0x30000000)
#define DMA_CHx_DEST_DATAWIDTH_BYTE      (0x00000000)
#define DMA_CHx_DEST_DATAWIDTH_HALFWORD  (0x10000000)
#define DMA_CHx_DEST_DATAWIDTH_WORD      (0x20000000)

//only for sc6530
#define SC6530_DMA_CHx_DEST_DATAWIDTH_MASK      (0x03000000)
#define SC6530_DMA_CHx_DEST_DATAWIDTH_BYTE      (0x00000000)
#define SC6530_DMA_CHx_DEST_DATAWIDTH_HALFWORD  (0x01000000)
#define SC6530_DMA_CHx_DEST_DATAWIDTH_WORD      (0x02000000)

//endaian switch mode[27:26]
/*
2'b00 : ABCD => ABCD;
2'b01 : ABCD => DCBA
2'b10 : ABCD => BADC;
2'b11 : ABCD => CDAB
*/
#define DMA_CHx_ENDIANSWMODESEL_MASK     (0x0C000000)
#define DMA_CHx_ENDIANSWMODESEL_UN       (0x00000000)
#define DMA_CHx_ENDIANSWMODESEL_FULL     (0x04000000)
#define DMA_CHx_ENDIANSWMODESEL_MODE0    (0x08000000)
#define DMA_CHx_ENDIANSWMODESEL_MODE1    (0x0C000000)

//only for sc6530
#define SC6530_DMA_CHx_ENDIANSWMODESEL_MASK     (0x00C00000)
#define SC6530_DMA_CHx_ENDIANSWMODESEL_UN       (0x00000000)
#define SC6530_DMA_CHx_ENDIANSWMODESEL_FULL     (0x00400000)
#define SC6530_DMA_CHx_ENDIANSWMODESEL_MODE0    (0x00800000)
#define SC6530_DMA_CHx_ENDIANSWMODESEL_MODE1    (0x00C00000)

//request mode[25:24]
/*
2'b00 : Fragment;
2'b01 : Block;
2'b10 : Transaction;
2'b11 : Link List;
*/
#define DMA_CHx_REQMODE_MASK             (0x03000000)
#define DMA_CHx_REQMODE_FRAGMENT         (0x00000000)
#define DMA_CHx_REQMODE_BLOCK            (0x01000000)
#define DMA_CHx_REQMODE_TRANSACTION      (0x02000000)
#define DMA_CHx_REQMODE_LIST             (0x03000000)

//only for sc6530
#define SC6530_DMA_CHx_REQMODE_MASK             (0x00300000)
#define SC6530_DMA_CHx_REQMODE_FRAGMENT         (0x00000000)
#define SC6530_DMA_CHx_REQMODE_BLOCK            (0x00100000)
#define SC6530_DMA_CHx_REQMODE_TRANSACTION      (0x00200000)
#define SC6530_DMA_CHx_REQMODE_LIST             (0x00300000)

//src wrap enable[23]
/*
0 : Address wrap on Source.
1 : Address wrap on Destination
*/
#define DMA_CHx_WRAP_SEL_MASK             (0x00800000)
#define DMA_CHx_WRAP_SEL_DISABLE          (0x00000000)
#define DMA_CHx_WRAP_SEL_ENABLE           (0x00800000)

//only for sc6530
#define SC6530_DMA_CHx_WRAP_SEL_MASK             (0x00080000)
#define SC6530_DMA_CHx_WRAP_SEL_DISABLE          (0x00000000)
#define SC6530_DMA_CHx_WRAP_SEL_ENABLE           (0x00080000)

//dest wrap enable[22]
/*
0 : Disable address wrap mode. 
1 : Enable address wrap mode.
*/
#define DMA_CHx_WRAP_EN_MASK            (0x00400000)
#define DMA_CHx_WRAP_EN_DISABLE         (0x00000000)
#define DMA_CHx_WRAP_EN_ENABLE          (0x00400000)

//only for sc6530
#define SC6530_DMA_CHx_WRAP_EN_MASK            (0x00040000)
#define SC6530_DMA_CHx_WRAP_EN_DISABLE         (0x00000000)
#define SC6530_DMA_CHx_WRAP_EN_ENABLE          (0x00040000)

//address fix on select[21]
/*
0 : Address Fix on Source.
1 : Address Fix on Destination
*/
#define DMA_CHx_ADDFIXEDON_MASK          (0x00200000)
#define DMA_CHx_ADDFIXEDON_SRC           (0x00000000)
#define DMA_CHx_ADDFIXEDON_DEST          (0x00200000)

//only for sc6530
#define SC6530_DMA_CHx_ADDFIXEDON_MASK          (0x00020000)
#define SC6530_DMA_CHx_ADDFIXEDON_DEST          (0x00020000)
#define SC6530_DMA_CHx_ADDFIXEDON_SRC           (0x00000000)

//address fix enable[20]
#define DMA_CHx_ADDFIXED_MASK            (0x00100000)
#define DMA_CHx_ADDFIXED_DISABLE         (0x00000000)
#define DMA_CHx_ADDFIXED_ENABLE          (0x00100000)

//only for sc6530
#define SC6530_DMA_CHx_ADDFIXED_MASK            (0x00010000)
#define SC6530_DMA_CHx_ADDFIXED_DISABLE         (0x00000000)
#define SC6530_DMA_CHx_ADDFIXED_ENABLE          (0x00010000)

//fragment length bits[16:0]
#define DMA_CHx_FRAGLENGTH_MASK         (0x0001FFFF)
#define DMA_CHx_FRAGLENGTH_OFFSET        0

//only for sc6530
#define SC6530_DMA_CHx_FRAGLENGTH_MASK         (0x0000FFFF)
#define SC6530_DMA_CHx_FRAGLENGTH_OFFSET        0
//no auto close

#define DMA_CHx_NOAUTO_CLOSE_MASK        (0x00020000)
#define DMA_CHx_NOAUTO_CLOSE_NO          (0x00000000)
#define DMA_CHx_NOAUTO_CLOSE_YES         (0x00020000)

//blocke length bits
#define DMA_CHx_BLOCKLENGTH_MASK         (0x0000FFFF)
#define DMA_CHx_BLOCKLENGTH_OFFSET        0

//src/des step bits
#define DMA_CHx_SRC_STEP_MASK            (0x0000FFFF)
#define DMA_CHx_SRC_STEP_OFFSET          0
#define DMA_CHx_DEST_STEP_MASK           (0xFFFF0000)
#define DMA_CHx_DEST_STEP_OFFSET         16

#define DMA_CHx_SRC_STEP_POSITIVE        0
#define DMA_CHx_SRC_STEP_NEGATIVE        BIT_31
#define DMA_CHx_DEST_STEP_POSITIVE       0
#define DMA_CHx_DEST_STEP_NEGATIVE       BIT_16

//total length
#define DMA_CHx_TOTALLENGTH_MASK         (0x01FFFFFF)
#define DMA_CHx_TOTALLENGTH_OFFSET        0
//block mode
#define DMA_CHx_BLOCKMODE_MASK           (0x70000000)
#define DMA_CHx_BLOCKMODE_SIGNLE         (0x00000000)
#define DMA_CHx_BLOCKMODE_INCR           (0x10000000)
#define DMA_CHx_BLOCKMODE_INCR4          (0x30000000)
#define DMA_CHx_BLOCKMODE_INCR8          (0x50000000)
#define DMA_CHx_BLOCKMODE_INCR16         (0x70000000)

#define DMA_CHx_BLOCKSTEP_MASK           (0x03FFFFFF)
#define DMA_CHx_BLOCKSTEP_OFFSET         (0)
#define DMA_CHx_BLOCKSTEP_POSITIVE       0
#define DMA_CHx_BLOCKSTEP_NEGATIVE       BIT_25

#define DMA_CHx_CFG_PRIORITY0		     (0x00000000)
#define DMA_CHx_CFG_PRIORITY1		     (0x00001000)
#define DMA_CHx_CFG_PRIORITY2		     (0x00002000)
#define DMA_CHx_CFG_PRIORITY3            (0x00003000)


#define DMA_PRI_REG_COUNT                2
#define DMA_UID_REG_COUNT                8

typedef struct _DMA_GEN_CTL_REG
{
    volatile uint32 dma_pause;
    volatile uint32 dma_blk_wait;
    volatile uint32 dma_int_raw_status;    
    volatile uint32 dma_int_mask_status;
    volatile uint32 dma_req_status;
    volatile uint32 dma_en_status;
    volatile uint32 dma_err_status;
}DMA_GEN_CTL_REG_T;

typedef struct _DMA_STD_CHN_CFG_REG
{
    volatile uint32 std_chn_pause;
    volatile uint32 std_chn_req;
    volatile uint32 std_chn_cfg;    
    volatile uint32 std_chn_int;
    volatile uint32 std_chn_src_addr;
    volatile uint32 std_chn_dest_addr;
    volatile uint32 std_chn_frag_len;
    volatile uint32 std_chn_blk_len;
}DMA_STD_CHN_CFG_REG;

typedef struct _DMA_FULL_CHN_CFG_REG
{
    volatile uint32 full_chn_pause;
    volatile uint32 full_chn_req;    
    volatile uint32 full_chn_cfg;
    volatile uint32 full_chn_int;
    volatile uint32 full_chn_src_addr;
    volatile uint32 full_chn_dest_addr;    
    volatile uint32 full_chn_frag_len;    
    volatile uint32 full_chn_blk_len;
    volatile uint32 full_chn_trsc_len;
    volatile uint32 full_chn_trsf_step;
    volatile uint32 full_chn_wrap_ptr;    
    volatile uint32 full_chn_wrap_to;
    volatile uint32 full_chn_llist_ptr;        
    volatile uint32 full_chn_frag_step;        
    volatile uint32 full_chn_src_blk_step;
    volatile uint32 full_chn_dest_blk_step;    
}DMA_FULL_CHN_CFG_REG;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

