/*******************************************************************************
 ** File Name:      sdslave_reg_v0.h                                           *
 ** Author:         andy.chen                                                  *
 ** Date:           12/30/2010                                                 *
 ** Copyright:      Copyright 2001-xxxx by Spreadtrum Communications,Inc.      *
 *                  All Rights Reserved.                                       *
 **                 This software is supplied under the terms of a license     *
 **                 agreement or non-disclosure agreement with Spreadtrum.     *
 **                 Passing on and copying of this document,and communication  *
 **                 of its contents is not permitted without prior written     *
 **                 authorization.                                             *
 ** Description:    This is the header file of defines signals of the sim      *
 **                 application layer                                          *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** ---------------------------------------------------------------------------*
 ** Date           Name             Description                                *
 **----------------------------------------------------------------------------*
 ** 2010.12        andy.chen         Create                                    *
 ** 20xx.xx        xxxx.xxxx         update                                    *
 ******************************************************************************/
#ifndef __SDSLAVE_REG_V0_H__
#define __SDSLAVE_REG_V0_H__

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*******************************************************************************
 **                        Mcaro Definitions                                   *
 ******************************************************************************/
// NOTE: WR/RD here means master wr/rd to/from slave
#define INT_WR_TRANS_CMP        BIT_0
#define INT_RD_TRANS_CMP        BIT_1
#define INT_DMA_INT             BIT_2
#define INT_BUF_RD_RDY          BIT_3
#define INT_BUF_WR_RDY          BIT_4
#define INT_WRITE_START         BIT_5
#define INT_READ_START          BIT_6
#define INT_OCR_SET             BIT_7
#define INT_TRANS_CMP           (INT_WR_TRANS_CMP|INT_RD_TRANS_CMP)
#define INT_TRANS_START         (INT_WRITE_START | INT_READ_START)
#define INT_DMA_RW              (INT_TRANS_CMP | INT_TRANS_START)

#define INT_ERR_CMDCRC          BIT_16
#define INT_ERR_CMDILL          BIT_17
#define INT_ERR_DATACRC         BIT_18
#define INT_ERR_AHB             BIT_19
#define INT_ERR                 (INT_ERR_CMDCRC|INT_ERR_CMDILL|INT_ERR_DATACRC|INT_ERR_AHB)

#define INT_MASK_ALL            0x000F00FF

// block len&cnt
#define BLK_LEN_MASK            0x00000FFF
#define BLK_CNT_MASK            0x01FF0000
#define BLK_MODE_MASK           BIT_0

// transfer mode
#define BLK_MODE_MASK           BIT_0           // blk mode:1, byte mode:0
#define TRANS_INC_MASK          BIT_1           // increasing:1, fix:0
#define DIR_WR_MASK             BIT_2           // WR:1, RD:0
#define BUSWIDTH_4_MASK         (2<<3)          // 4bit:2, 1bit:0
#define HIGH_SPEED_MASK         BIT_5           // high:1, normal:0
#define MULTI_BLOCK_MASK        BIT_6           // multi:1, single:0
#define FINITE_TRANS_MASK       BIT_7           // finite:1, infinite:0

/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/
typedef struct SDSLAVE_REGCFG_TAG
{
    volatile uint32 DMA_SYS_ADD;        //-0
    volatile uint32 SLAVE_CTL0;         //-4
    volatile uint32 REV0;               //-8
    volatile uint32 BLK_SIZE_COUNT;     //-C
    volatile uint32 CMD_TRANSMODE;      //-10
    volatile uint32 PRESENT_STAT;       //-14
    volatile uint32 CMD_ARGUMENT;       //-18
    volatile uint32 RSP_ARGUMENT;       //-1C
    volatile uint32 BUFFER_PORT;        //-20
    volatile uint32 REV1;
    volatile uint32 REV2;
    volatile uint32 REV3;
    volatile uint32 INT_STA_EN;         //-30
    volatile uint32 INT_STA_CLR;        //-34
    volatile uint32 INT_STA_RAW;        //-38
    volatile uint32 INT_STA_MSK;        //-3C
    volatile uint32 OCR_SUPPORT;        //-40
    volatile uint32 OCR_CURRENT;        //-44
}
SDSLAVE_REGCFG_T;

/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/

/*******************************************************************************
 **                        Function Definitions                                *
 ******************************************************************************/

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif

#endif
