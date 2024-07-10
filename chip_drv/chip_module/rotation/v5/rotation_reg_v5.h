/******************************************************************************
 ** File Name:    rotation_reg_v0.h                                          *
 ** Author:       Tim.zhu                                                     *
 ** DATE:         07/09/2009                                                  *
 ** Copyright:    2009 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 07/09/2009    Tim.zhu         Create.                                     *
 ******************************************************************************/
#ifndef _ROTATION_REG_V5_H_
#define _ROTATION_REG_V5_H_
/*----------------------------------------------------------------------------*
 **                          Dependencies                                     *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                          Compiler Flag                                    *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C"
{
#endif
#define ROT_SRC_STAR_ADDR           (ROT_REG_BASE + 0x00)
#define ROT_DST_STAR_ADDR           (ROT_REG_BASE + 0x04)
#define ROT_IMAGE_SIZE              (ROT_REG_BASE + 0x08)
#define ROT_CONTROL                 (ROT_REG_BASE + 0x0c)
#define ROT_ORIWIDTH                (ROT_REG_BASE + 0x10)
#define ROT_ORICOORD                (ROT_REG_BASE + 0x14)
#define ROT_DMA_CFG0                (ROT_REG_BASE + 0x20)
#define ROT_DMA_CFG1                (ROT_REG_BASE + 0x24)
#define ROT_DMA_SRC_ADD             (ROT_REG_BASE + 0x28)
#define ROT_DMA_DST_ADD             (ROT_REG_BASE + 0x2c)
#define ROT_DMA_LLPTR               (ROT_REG_BASE + 0x30)
#define ROT_DMA_SDI                 (ROT_REG_BASE + 0x34)
#define ROT_DMA_SBI                 (ROT_REG_BASE + 0x38)
#define ROT_DMA_DBI                 (ROT_REG_BASE + 0x3c)
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
#define REG_ROTATION_SRC_ADDR           (REG_ROTATION_REG_BASE + 0x0200)
#define REG_ROTATION_DST_ADDR           (REG_ROTATION_REG_BASE + 0x0204)
#define REG_ROTATION_IMG_SIZE           (REG_ROTATION_REG_BASE + 0x0208)
#define REG_ROTATION_CTRL               (REG_ROTATION_REG_BASE + 0x020c)
#define REG_ROTATION_ORIWIDTH           (REG_ROTATION_REG_BASE + 0x210)
#define REG_ROTATION_ORICOORD           (REG_ROTATION_REG_BASE + 0x214)
#define REG_ROTATION_DMA_CHN_CFG0       (REG_ROTATION_REG_BASE + 0x0220)
#define REG_ROTATION_DMA_CHN_CFG1       (REG_ROTATION_REG_BASE + 0x0224)
#define REG_ROTATION_DMA_CHN_SRC_ADDR      (REG_ROTATION_REG_BASE + 0x0228)
#define REG_ROTATION_DMA_CHN_DST_ADDR      (REG_ROTATION_REG_BASE + 0x022c)
#define REG_ROTATION_DMA_CHN_LLPTR      (REG_ROTATION_REG_BASE + 0x0230)
#define REG_ROTATION_DMA_CHN_SDI        (REG_ROTATION_REG_BASE + 0x0234)
#define REG_ROTATION_DMA_CHN_SBI        (REG_ROTATION_REG_BASE + 0x0238)
#define REG_ROTATION_DMA_CHN_DBI        (REG_ROTATION_REG_BASE + 0x023c)

//define DMA req_id_addr for ROTATION module
#define ROTATION_DMA_REQ 		DMA_ROTATION
#if !defined(PLATFORM_UWS6121E)
#define ROTATION_DMA_REQ_ADDR  (DMA_CTL_REG_BASE+0x2000+(ROTATION_DMA_REQ-1)*0x04)
#else
#define ROTATION_DMA_REQ_ADDR  (REG_DMA_BASE+0x40+(ROTATION_DMA_REQ-1)*0x04)
#endif
#define ROTATION_DMA_CONNECT(req_addr, chan ) CHIP_REG_SET(req_addr, chan)

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
typedef union _rotation_src_addr_tag
{
    struct _rotation_src_addr_map
    {
        volatile unsigned int src_addr      :32;    // rotation src addr
    } mBits ;
    volatile unsigned int dwValue ;
} ROTATION_SRC_ADDR_U;

typedef union _rotation_dst_addr_tag
{
    struct _rotation_dst_addr_map
    {
        volatile unsigned int dst_addr      :32;    // rotation src addr
    } mBits ;
    volatile unsigned int dwValue ;
} ROTATION_DST_ADDR_U;

#ifdef CHIP_ENDIAN_LITTLE

typedef union _rotation_img_size_tag
{
    struct _rotation_img_size_map
    {
        volatile unsigned int height            :12; //[11:0]   image heighy
        volatile unsigned int width             :12; //[23:12]  image width
        volatile unsigned int pixel_format      :2;  //[25:24]  pixel format 1 bytes 2bytes 4bytes
        volatile unsigned int reserved_0        :6;  //[31:26]  Reserved
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_IMG_SIZE_U;

typedef union _rotation_ctrl_tag
{
    struct _rotation_ctrl_map
    {
        volatile unsigned int rot_mode          :1;  //[0]      0:normal mode 1:uv 422 mode
        volatile unsigned int rotation_mode     :2;  //[2:1]    rotation mode
        volatile unsigned int rotation_eb       :1;  //[3]      rotaton eb
        volatile unsigned int reserved_1        :28; //[31:4]   Reserved
    } mBits;
    volatile unsigned int dValue;
} ROTATION_CTRL_U;

#else   //#ifdef CHIP_ENDIAN_LITTLE

typedef union _rotation_img_size_tag
{
    struct _rotation_img_size_map
    {
        volatile unsigned int reserved_0        :6;  //[31:26]  Reserved
        volatile unsigned int pixel_format      :2;  //[25:24]  pixel format 1 bytes 2bytes 4bytes
        volatile unsigned int width             :12; //[23:12]  image width
        volatile unsigned int height            :12; //[11:0]   image heighy
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_IMG_SIZE_U;

typedef union _rotation_ctrl_tag
{
    struct _rotation_ctrl_map
    {
        volatile unsigned int reserved_1        :28; //[31:4]   Reserved
        volatile unsigned int rotation_eb       :1;  //[3]      rotaton eb
        volatile unsigned int rotation_mode     :2;  //[2:1]    rotation mode
        volatile unsigned int rot_mode          :1;  //[0]      0:normal mode 1:uv 422 mode
    } mBits;
    volatile unsigned int dValue;
} ROTATION_CTRL_U;

#endif  //#ifdef CHIP_ENDIAN_LITTLE

#ifdef CHIP_ENDIAN_LITTLE

typedef union _rotation_ori_coord_tag
{
    struct _rotation_ori_coord_map
    {
        volatile unsigned int ori_x             :12; //[11:0]   origin x start
        volatile unsigned int ori_y             :12; //[23:12]  origin y start
        volatile unsigned int reserved_0        :8;  //[31:26]  Reserved
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_ORI_COORD_U;

#else   //#ifdef CHIP_ENDIAN_LITTLE

typedef union _rotation_ori_coord_tag
{
    struct _rotation_ori_coord_map
    {
        volatile unsigned int reserved_0        :8;  //[31:26]  Reserved
        volatile unsigned int ori_y             :12; //[23:12]  origin y start
        volatile unsigned int ori_x             :12; //[11:0]   origin x start
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_ORI_COORD_U;

#endif  //#ifdef CHIP_ENDIAN_LITTLE

typedef union _rotation_dma_chn_cfg0_tag
{
    struct _rotation_dma_chn_cfg0_map
    {
        volatile unsigned int dma_chn_cfg0          :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_CFG0_U;

typedef union _rotation_dma_chn_cfg1_tag
{
    struct _rotation_dma_chn_cfg1_map
    {
        volatile unsigned int dma_chn_cfg1          :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_CFG1_U;

typedef union _rotation_dma_src_addr_tag
{
    struct _rotation_dma_src_addr_map
    {
        volatile unsigned int dma_src_addr          :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_SRC_ADDR_U;

typedef union _rotation_dma_dst_addr_tag
{
    struct _rotation_dma_dst_addr_map
    {
        volatile unsigned int dma_dst_addr          :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_DST_ADDR_U;

typedef union _rotation_dma_chn_llptr_tag
{
    struct _rotation_dma_chn_llptr_map
    {
        volatile unsigned int dma_chn_llptr         :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_LLPTR_U;

typedef union _rotation_dma_chn_sdi_tag
{
    struct _rotation_dma_chn_sdi_map
    {
        volatile unsigned int dma_chn_sdi           :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_SDI_U;

typedef union _rotation_dma_chn_sbi_tag
{
    struct _rotation_dma_chn_sbi_map
    {
        volatile unsigned int dma_chn_sbi           :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_SBI_U;

typedef union _rotation_dma_chn_dbi_tag
{
    struct _rotation_dma_chn_dbi_map
    {
        volatile unsigned int dma_chn_dbi           :32;
    } mBits;
    volatile unsigned int dwValue;
} ROTATION_DMA_CHN_DBI_U;

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End

