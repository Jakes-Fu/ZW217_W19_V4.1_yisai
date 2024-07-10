/******************************************************************************
 ** File Name:    isp_reg_v0.h                                           *
 ** Author:       Jianping.Wang                                               *
 ** DATE:         06/10/2008                                                  *
 ** Copyright:    2008 Spreadtrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 08/8/2009    Tim.zhu   Create.                                     *10
 ******************************************************************************/
#ifndef _ISP_REG_V0_H_
#define _ISP_REG_V0_H_
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
/**---------------------------------------------------------------------------*
**                               Micro Define                                **
**----------------------------------------------------------------------------*/
#define DCAM_ISP_REG_BASE   0x20200000
//top module
#define DCAM_CFG            (DCAM_ISP_REG_BASE + 0x0000)
#define CAP_SHADOW_CNTL     (DCAM_ISP_REG_BASE + 0x0004)
#define YUV2RGB_CFG         (DCAM_ISP_REG_BASE + 0x0008)
#define DCAM_SRC_SIZE       (DCAM_ISP_REG_BASE + 0x000C)
#define ISP_DIS_SIZE        (DCAM_ISP_REG_BASE + 0x0010)
#define VSP_TIME_OUT        (DCAM_ISP_REG_BASE + 0x0014)
#define DCAM_INT_STS        (DCAM_ISP_REG_BASE + 0x0020)
#define DCAM_INT_MASK       (DCAM_ISP_REG_BASE + 0x0024)
#define DCAM_INT_CLR        (DCAM_ISP_REG_BASE + 0x0028)
#define DCAM_INT_RAW        (DCAM_ISP_REG_BASE + 0x002C)

//ahb master
#define FRM_ADDR0           (DCAM_ISP_REG_BASE + 0x0040)
#define FRM_ADDR1           (DCAM_ISP_REG_BASE + 0x0044)
#define FRM_ADDR2           (DCAM_ISP_REG_BASE + 0x0048)
#define FRM_ADDR3           (DCAM_ISP_REG_BASE + 0x004C)
#define FRM_ADDR4           (DCAM_ISP_REG_BASE + 0x0050)
#define FRM_ADDR5           (DCAM_ISP_REG_BASE + 0x0054)
#define FRM_ADDR6           (DCAM_ISP_REG_BASE + 0x0058)
#define FRM_ADDR_H          (DCAM_ISP_REG_BASE + 0x005c)
#define BURST_GAP           (DCAM_ISP_REG_BASE + 0x0060)
#define ENDAIN_SEL          (DCAM_ISP_REG_BASE + 0x0064)
#define AHBM_STS            (DCAM_ISP_REG_BASE + 0x0060)
#define FRM_ADDR7           (DCAM_ISP_REG_BASE + 0x006c)

//cap
#define CAP_CTRL                (DCAM_ISP_REG_BASE + 0x0100)
#define CAP_FRM_CNT             (DCAM_ISP_REG_BASE + 0x0104)
#define CAP_START               (DCAM_ISP_REG_BASE + 0x0108)
#define CAP_END                 (DCAM_ISP_REG_BASE + 0x010C)
#define CAP_IMAGE_DECI          (DCAM_ISP_REG_BASE + 0x0110)
#define CAP_FIX_MODE            (DCAM_ISP_REG_BASE + 0x0114)
#define CAP_OBSERVE             (DCAM_ISP_REG_BASE + 0x0118)
#define CAP_JPEG_FRM_CTRL       (DCAM_ISP_REG_BASE + 0x011C)
#define CAP_JPEG_LAST_FRM_SIZE  (DCAM_ISP_REG_BASE + 0x0120)

//trim&scale
#define SCALING_CFG                 (DCAM_ISP_REG_BASE + 0x0124)
#define TRIM_START                  (DCAM_ISP_REG_BASE + 0x0128)
#define TRIM_SIZE                   (DCAM_ISP_REG_BASE + 0x012C)
#define SLICE_HEIGHT                (DCAM_ISP_REG_BASE + 0x0130)

#define VID_LUMA_HCOEFF01           (DCAM_ISP_REG_BASE + 0x0200)
#define VID_VCOEFF0                 (DCAM_ISP_REG_BASE + 0x0300)

//Dcam int mask bits
#define DCAM_INT_MASK_VALUE         0x00003FFF

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
//top module reg define being
#ifdef CHIP_ENDIAN_LITTLE
typedef union _dcam_cfg_tag
{
    struct _dcam_cfg_map
    {
        volatile unsigned int dcam_mode         :1; //0:ISP mode;1:VSP mode
        volatile unsigned int isp_mode          :2; //00:idle;01:capture;10:preview;11:review
        volatile unsigned int vsp_eb            :1; //0:dcam clock domain;1: enbale VSP module
        volatile unsigned int clk_switch        :1; //0:clk_dcam_domain;1:HCLK
        volatile unsigned int cap_eb            :1; //0:disable CAP module;1:enable CAP module
        volatile unsigned int cap_input_format  :1; //0:YUV ; 1:jpeg
        volatile unsigned int clk_status        :1; //0:enable clock gating;1:disable clock gating
        volatile unsigned int cap_cfg           :1; //0:only capture one frame, 1: capture  data until cap_eb is low and eof is generated.
        volatile unsigned int reserved          :23;//Reserved ;
    } mBits ;
    volatile unsigned int dwValue ;
} DCAM_CFG_U;

typedef union _cap_shadow_cntl_tag
{
    struct _cap_shadow_cntl_map
    {
        volatile unsigned int frc_copy_cap  :1;
        volatile unsigned int auto_copy_cap :1;
        volatile unsigned int reserved      :30;
    } mBits;
    volatile unsigned int dwValue;
} CAP_SHADOW_CNTL_U;

typedef union _yuv2rgb_cfg_tag
{
    struct _yuv2rgb_cfg_map
    {
        volatile unsigned int yuv2rgb_eb        :1;
        volatile unsigned int dither_eb         :1;
        volatile unsigned int rgb_input_mode    :1;
        volatile unsigned int rgb_format        :1;
        volatile unsigned int reserved          :28;
    } mBits;
    volatile unsigned int dwValue;
} YUV2RGB_CFG_U;

typedef union _dcam_src_size_tag
{
    struct _dcam_src_size_map
    {
        volatile unsigned int src_size_x    :12;
        volatile unsigned int reserved0     :4;
        volatile unsigned int src_size_y    :12;
        volatile unsigned int reserved1     :4;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_SRC_SIZE_U;

typedef union _isp_dis_size_tag
{
    struct _isp_dis_size_map
    {
        volatile unsigned int dis_size_x    :12;
        volatile unsigned int reserved0     :4;
        volatile unsigned int dis_size_y    :12;
        volatile unsigned int reserved1     :4;
    } mBits;
    volatile unsigned int dwValue;
} ISP_DIS_SIZE_U;

typedef union _vsp_time_out_tag
{
    struct _vsp_time_out_map
    {
        volatile unsigned int time_out_value    :16;
        volatile unsigned int time_out_en       :1;
        volatile unsigned int time_out_clr      :11;
        volatile unsigned int reserved          :14;
    } mBits;
    volatile unsigned int dwValue;
} VSP_TIME_OUT_U;

typedef union _dcam_int_sts_tag
{
    struct _dcam_int_sts_map
    {
        volatile unsigned int sensor_sof    :1;
        volatile unsigned int sensor_eof    :1;
        volatile unsigned int cap_sof       :1;
        volatile unsigned int cap_eof       :1;
        volatile unsigned int isp_tx_done   :1;
        volatile unsigned int cap_line_err  :1;
        volatile unsigned int cap_buf_ovf   :1;
        volatile unsigned int vsp_bsm_done  :1;
        volatile unsigned int vsp_vlc_done  :1;
        volatile unsigned int vsp_mbio_done :1;
        volatile unsigned int cap_frame_err :1;
        volatile unsigned int jpg_buf_ovf   :1;
        volatile unsigned int vsp_time_out  :1;
        volatile unsigned int vsp_vld_err   :1;
        volatile unsigned int dsp_hfull     :1;
        volatile unsigned int dsp_full      :1;
        volatile unsigned int reserved      :16;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_INT_STS_U;

typedef union _dcam_int_maks_tag
{
    struct _dcam_int_mask_map
    {
        volatile unsigned int sensor_sof    :1;
        volatile unsigned int sensor_eof    :1;
        volatile unsigned int cap_sof       :1;
        volatile unsigned int cap_eof       :1;
        volatile unsigned int isp_tx_done   :1;
        volatile unsigned int cap_line_err  :1;
        volatile unsigned int cap_buf_ovf   :1;
        volatile unsigned int vsp_bsm_done  :1;
        volatile unsigned int vsp_vlc_done  :1;
        volatile unsigned int vsp_mbio_done :1;
        volatile unsigned int cap_frame_err :1;
        volatile unsigned int jpg_buf_ovf   :1;
        volatile unsigned int vsp_time_out  :1;
        volatile unsigned int vsp_vld_err   :1;
        volatile unsigned int dsp_hfull     :1;
        volatile unsigned int dsp_full      :1;
        volatile unsigned int reserved      :16;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_INT_MASK_U;

typedef union _dcam_int_clr_tag
{
    struct _dcam_int_clr_map
    {
        volatile unsigned int sensor_sof    :1;
        volatile unsigned int sensor_eof    :1;
        volatile unsigned int cap_sof       :1;
        volatile unsigned int cap_eof       :1;
        volatile unsigned int isp_tx_done   :1;
        volatile unsigned int cap_line_err  :1;
        volatile unsigned int cap_buf_ovf   :1;
        volatile unsigned int vsp_bsm_done  :1;
        volatile unsigned int vsp_vlc_done  :1;
        volatile unsigned int vsp_mbio_done :1;
        volatile unsigned int cap_frame_err :1;
        volatile unsigned int jpg_buf_ovf   :1;
        volatile unsigned int vsp_time_out  :1;
        volatile unsigned int vsp_vld_err   :1;
        volatile unsigned int dsp_hfull     :1;
        volatile unsigned int dsp_full      :1;
        volatile unsigned int reserved      :16;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_INT_CLR_U;

typedef union _dcam_int_raw_tag
{
    struct _dcam_int_raw_map
    {
        volatile unsigned int sensor_sof    :1;
        volatile unsigned int sensor_eof    :1;
        volatile unsigned int cap_sof       :1;
        volatile unsigned int cap_eof       :1;
        volatile unsigned int isp_tx_done   :1;
        volatile unsigned int cap_line_err  :1;
        volatile unsigned int cap_buf_ovf   :1;
        volatile unsigned int vsp_bsm_done  :1;
        volatile unsigned int vsp_vlc_done  :1;
        volatile unsigned int vsp_mbio_done :1;
        volatile unsigned int cap_frame_err :1;
        volatile unsigned int jpg_buf_ovf   :1;
        volatile unsigned int vsp_time_out  :1;
        volatile unsigned int vsp_vld_err   :1;
        volatile unsigned int dsp_hfull     :1;
        volatile unsigned int dsp_full      :1;
        volatile unsigned int reserved      :16;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_INT_RAW_U;

//top module reg define end

//ahb master reg define being
typedef union _dcam_frm_addr0_tag
{
    struct _dcam_frm_addr0_map
    {
        volatile unsigned int frm_addr_0    :18;
        volatile unsigned int reserved      :14;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_0_U;

typedef union _dcam_frm_addr1_tag
{
    struct _dcam_frm_addr1_map
    {
        volatile unsigned int frm_addr_1    :18;
        volatile unsigned int reserved      :14;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_1_U;

typedef union _dcam_frm_addr2_tag
{
    struct _dcam_frm_addr2_map
    {
        volatile unsigned int frm_addr_2    :18;
        volatile unsigned int reserved      :14;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_2_U;

typedef union _dcam_frm_addr3_tag
{
    struct _dcam_frm_addr3_map
    {
        volatile unsigned int frm_addr_3    :18;
        volatile unsigned int reserved      :14;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_3_U;

typedef union _dcam_frm_addr4_tag
{
    struct _dcam_frm_addr4_map
    {
        volatile unsigned int frm_addr_4    :18;
        volatile unsigned int reserved      :14;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_4_U;

typedef union _dcam_frm_addr5_tag
{
    struct _dcam_frm_addr5_map
    {
        volatile unsigned int frm_addr_5    :18;
        volatile unsigned int reserved      :14;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_5_U;

typedef union _dcam_frm_addr6_tag
{
    struct _dcam_frm_addr6_map
    {
        volatile unsigned int frm_addr_6    :18;
        volatile unsigned int reserved      :14;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_6_U;

typedef union _dcam_frm_addr7_tag
{
    struct _dcam_frm_addr7_map
    {
        volatile unsigned int frm_addr_7    :18;
        volatile unsigned int reserved      :14;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_7_U;

typedef union _dcam_frm_high_tag
{
    struct _dcam_frm_high_map
    {
        volatile unsigned int frm_addr_h    :6;
        volatile unsigned int reserved      :26;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_HIGH_U;

typedef union _dcam_burst_cap_tag
{
    struct _dcam_burst_cap_map
    {
        volatile unsigned int burst_gap     :5;
        volatile unsigned int reserved      :27;
    } mBits;
    volatile unsigned int dwValue;
} BURST_GAP_U;

typedef union _dcam_endain_sel_tag
{
    struct dcam_endain_sel_map
    {
        volatile unsigned int endian_sel_wr :5;
        volatile unsigned int endian_sel_rd :5;
        volatile unsigned int reserved      :27;
    } mBits;
    volatile unsigned int dwValue;
} ENDAIN_SEL_U;

typedef union _dcam_ahbm_sts_tag
{
    struct _dcam_ahbm_sts_map
    {
        volatile unsigned int ahbm_busy     :1;
        volatile unsigned int reserved      :30;
    } mBits;
    volatile unsigned int dwValue;
} AHBM_STS_U;
//ahb master reg define end

//cap reg define being
typedef union _cap_ctrl_tag
{
    struct _cap_ctrl_map
    {
        volatile unsigned int ccir_656          :1;
        volatile unsigned int tv_frame_sel      :2;
        volatile unsigned int hsync_pol         :1;
        volatile unsigned int vsync_pol         :1;
        volatile unsigned int reserved0         :1;
        volatile unsigned int cut_first_y0      :1;
        volatile unsigned int yuv_type          :2;
        volatile unsigned int fifo_data_rate    :2;
        volatile unsigned int cap_ccir_rst      :1;
        volatile unsigned int cap_ccir_pd       :1;
        volatile unsigned int reserved1         :19;
    } mBits;
    volatile unsigned int dwValue;
} CAP_CTRL_U;

typedef union _cap_frm_cnt_tag
{
    struct _cap_frm_cnt_map
    {
        volatile unsigned int pre_skip_cnt  :6;
        volatile unsigned int reserved0     :2;
        volatile unsigned int cap_frm_deci  :4;
        volatile unsigned int reserved1     :4;
        volatile unsigned int cap_frm_cnt   :6;
        volatile unsigned int cap_frm_clr   :1;
        volatile unsigned int reserved2     :9;
    } mBits;
    volatile unsigned int dwValue;
} CAP_FRM_CNT_U;

typedef union _cap_start_tag
{
    struct _cap_start_map
    {
        volatile unsigned int start_x   :13;
        volatile unsigned int reserved0 :3;
        volatile unsigned int start_y   :13;
        volatile unsigned int reserved1 :3;
    } mBits;
    volatile unsigned int dwValue;
} CAP_START_U;

typedef union _cap_end_tag
{
    struct _cap_end_map
    {
        volatile unsigned int end_x     :13;
        volatile unsigned int reserved0 :3;
        volatile unsigned int end_y     :13;
        volatile unsigned int reserved1 :3;
    } mBits;
    volatile unsigned int dwValue;
} CAP_END_U;

typedef union _cap_image_deci_tag
{
    struct _cap_image_deci_map
    {
        volatile unsigned int cap_deci_x    :4;
        volatile unsigned int cap_deci_y    :4;
        volatile unsigned int cap_deci_mode :1;
        volatile unsigned int reserved      :23;
    } mBits;
    volatile unsigned int dwValue;
} CAP_IMAGE_DECI_U;

typedef union _cap_fix_mode_tag
{
    struct _cap_fix_mode_map
    {
        volatile unsigned int threshold_eb      :1;
        volatile unsigned int threshold_mode    :1;
        volatile unsigned int threshold_start   :9;
        volatile unsigned int threshold_end     :9;
        volatile unsigned int reserved          :12;
    } mBits;
    volatile unsigned int dwValue;
} CAP_FIX_MODE_U;

typedef union _cap_observe_tag
{
    struct _cap_observe_map
    {
        volatile unsigned int cap_observe_eb    :1;
        volatile unsigned int cap_fifo_max      :5;
        volatile unsigned int cap_fifo_cnt      :5;
        volatile unsigned int reserved          :16;
    } mBits;
    volatile unsigned int dwValue;
} CAP_OBSERVE_U;

typedef union _cap_jpeg_frm_ctrl_tag
{
    struct _cap_jpeg_frm_ctrl_map
    {
        volatile unsigned int get_num           :3;
        volatile unsigned int reserved0         :1;
        volatile unsigned int jpeg_drop_num     :4;
        volatile unsigned int mem_size          :8;
        volatile unsigned int dsp_log_mode      :1;
        volatile unsigned int dsp_log_buf_sel   :2;
        volatile unsigned int reserved1         :13;
    } mBits;
    volatile unsigned int dwValue;
} CAP_JPEG_FRM_CTRL_U;

typedef union _cap_jpeg_frm_size_tag
{
    struct _cap_jpeg_frm_size_map
    {
        volatile unsigned int jpeg_size         :24;
        volatile unsigned int reserved          :8;
    } mBits;
    volatile unsigned int dwValue;
} CAP_JPEG_FRM_SIZE_U;

//trim&scale
typedef union _scaling_cfg_tag
{
    struct _scaling_cfg_map
    {
        volatile unsigned int scale_bypass      :1;
        volatile unsigned int ver_down_tap      :3;
        volatile unsigned int trim_eb           :1;
        volatile unsigned int input_format      :2;
        volatile unsigned int scale_mode        :1;
        volatile unsigned int review_start      :1;
        volatile unsigned int sub_sample_eb     :1;
        volatile unsigned int sub_sample_mode   :2;
        volatile unsigned int reserved          :20;
    } mBits;
    volatile unsigned int dwValue;
} SCALING_CFG_U;

typedef union _trim_start_tag
{
    struct _trim_start_map
    {
        volatile unsigned int start_x   :12;
        volatile unsigned int reserved0 :4;
        volatile unsigned int start_y   :12;
        volatile unsigned int reserved1 :4;
    } mBits;
    volatile unsigned int dwValue;
} TRIM_START_U;

typedef union _trim_size_tag
{
    struct _trim_size_map
    {
        volatile unsigned int size_x    :12;
        volatile unsigned int reserved0 :4;
        volatile unsigned int size_y    :12;
        volatile unsigned int reserved1 :4;
    } mBits;
    volatile unsigned int dwValue;
} TRIM_SIZE_U;

typedef union _slice_ver_size_tag
{
    struct _slice_ver_size_map
    {
        volatile unsigned int slice_line_input  :12;
        volatile unsigned int last_slice        :1;
        volatile unsigned int reserved0         :3;
        volatile unsigned int slice_line_output :12;
        volatile unsigned int reserved1         :4;
    } mBits;
    volatile unsigned int dwValue;
} SLICE_VER_SIZE_U;

#else   //SC6800H
typedef union _dcam_cfg_tag
{
    struct _dcam_cfg_map
    {
        volatile unsigned int reserved          :23;//Reserved ;
        volatile unsigned int cap_cfg           :1; //0:only capture one frame, 1: capture  data until cap_eb is low and eof is generated.
        volatile unsigned int clk_status        :1; //0:enable clock gating;1:disable clock gating
        volatile unsigned int cap_input_format  :1; //0:YUV ; 1:jpeg
        volatile unsigned int cap_eb            :1; //0:disable CAP module;1:enable CAP module
        volatile unsigned int clk_switch        :1; //0:clk_dcam_domain;1:HCLK
        volatile unsigned int vsp_eb            :1; //0:dcam clock domain;1: enbale VSP module
        volatile unsigned int isp_mode          :2; //00:idle;01:capture;10:preview;11:review
        volatile unsigned int dcam_mode         :1; //0:ISP mode;1:VSP mode
    } mBits ;
    volatile unsigned int dwValue ;
} DCAM_CFG_U;

typedef union _cap_shadow_cntl_tag
{
    struct _cap_shadow_cntl_map
    {
        volatile unsigned int reserved      :30;
        volatile unsigned int auto_copy_cap :1;
        volatile unsigned int frc_copy_cap  :1;
    } mBits;
    volatile unsigned int dwValue;
} CAP_SHADOW_CNTL_U;

typedef union _yuv2rgb_cfg_tag
{
    struct _yuv2rgb_cfg_map
    {
        volatile unsigned int reserved          :28;
        volatile unsigned int rgb_format        :1;
        volatile unsigned int rgb_input_mode    :1;
        volatile unsigned int dither_eb         :1;
        volatile unsigned int yuv2rgb_eb        :1;
    } mBits;
    volatile unsigned int dwValue;
} YUV2RGB_CFG_U;

typedef union _dcam_src_size_tag
{
    struct _dcam_src_size_map
    {
        volatile unsigned int reserved1     :4;
        volatile unsigned int src_size_y    :12;
        volatile unsigned int reserved0     :4;
        volatile unsigned int src_size_x    :12;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_SRC_SIZE_U;

typedef union _isp_dis_size_tag
{
    struct _isp_dis_size_map
    {
        volatile unsigned int reserved1     :4;
        volatile unsigned int dis_size_y    :12;
        volatile unsigned int reserved0     :4;
        volatile unsigned int dis_size_x    :12;
    } mBits;
    volatile unsigned int dwValue;
} ISP_DIS_SIZE_U;

typedef union _vsp_time_out_tag
{
    struct _vsp_time_out_map
    {
        volatile unsigned int reserved          :4;
        volatile unsigned int time_out_clr      :12;
        volatile unsigned int time_out_en       :4;
        volatile unsigned int time_out_value    :16;
    } mBits;
    volatile unsigned int dwValue;
} VSP_TIME_OUT_U;

typedef union _dcam_int_sts_tag
{
    struct _dcam_int_sts_map
    {
        volatile unsigned int reserved      :16;
        volatile unsigned int dsp_full      :1;
        volatile unsigned int dsp_hfull     :1;
        volatile unsigned int vsp_vld_err   :1;
        volatile unsigned int vsp_time_out  :1;
        volatile unsigned int jpg_buf_ovf   :1;
        volatile unsigned int cap_frame_err :1;
        volatile unsigned int vsp_mbio_done :1;
        volatile unsigned int vsp_vlc_done  :1;
        volatile unsigned int vsp_bsm_done  :1;
        volatile unsigned int cap_buf_ovf   :1;
        volatile unsigned int cap_line_err  :1;
        volatile unsigned int isp_tx_done   :1;
        volatile unsigned int cap_eof       :1;
        volatile unsigned int cap_sof       :1;
        volatile unsigned int sensor_eof    :1;
        volatile unsigned int sensor_sof    :1;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_INT_STS_U;

typedef union _dcam_int_maks_tag
{
    struct _dcam_int_mask_map
    {
        volatile unsigned int reserved      :16;
        volatile unsigned int dsp_full      :1;
        volatile unsigned int dsp_hfull     :1;
        volatile unsigned int vsp_vld_err   :1;
        volatile unsigned int vsp_time_out  :1;
        volatile unsigned int jpg_buf_ovf   :1;
        volatile unsigned int cap_frame_err :1;
        volatile unsigned int vsp_mbio_done :1;
        volatile unsigned int vsp_vlc_done  :1;
        volatile unsigned int vsp_bsm_done  :1;
        volatile unsigned int cap_buf_ovf   :1;
        volatile unsigned int cap_line_err  :1;
        volatile unsigned int isp_tx_done   :1;
        volatile unsigned int cap_eof       :1;
        volatile unsigned int cap_sof       :1;
        volatile unsigned int sensor_eof    :1;
        volatile unsigned int sensor_sof    :1;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_INT_MASK_U;

typedef union _dcam_int_clr_tag
{
    struct _dcam_int_clr_map
    {
        volatile unsigned int reserved      :16;
        volatile unsigned int dsp_full      :1;
        volatile unsigned int dsp_hfull     :1;
        volatile unsigned int vsp_vld_err   :1;
        volatile unsigned int vsp_time_out  :1;
        volatile unsigned int jpg_buf_ovf   :1;
        volatile unsigned int cap_frame_err :1;
        volatile unsigned int vsp_mbio_done :1;
        volatile unsigned int vsp_vlc_done  :1;
        volatile unsigned int vsp_bsm_done  :1;
        volatile unsigned int cap_buf_ovf   :1;
        volatile unsigned int cap_line_err  :1;
        volatile unsigned int isp_tx_done   :1;
        volatile unsigned int cap_eof       :1;
        volatile unsigned int cap_sof       :1;
        volatile unsigned int sensor_eof    :1;
        volatile unsigned int sensor_sof    :1;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_INT_CLR_U;

typedef union _dcam_int_raw_tag
{
    struct _dcam_int_raw_map
    {
        volatile unsigned int reserved      :16;
        volatile unsigned int dsp_full      :1;
        volatile unsigned int dsp_hfull     :1;
        volatile unsigned int vsp_vld_err   :1;
        volatile unsigned int vsp_time_out  :1;
        volatile unsigned int jpg_buf_ovf   :1;
        volatile unsigned int cap_frame_err :1;
        volatile unsigned int vsp_mbio_done :1;
        volatile unsigned int vsp_vlc_done  :1;
        volatile unsigned int vsp_bsm_done  :1;
        volatile unsigned int cap_buf_ovf   :1;
        volatile unsigned int cap_line_err  :1;
        volatile unsigned int isp_tx_done   :1;
        volatile unsigned int cap_eof       :1;
        volatile unsigned int cap_sof       :1;
        volatile unsigned int sensor_eof    :1;
        volatile unsigned int sensor_sof    :1;
    } mBits;
    volatile unsigned int dwValue;
} DCAM_INT_RAW_U;

//top module reg define end

//ahb master reg define being
typedef union _dcam_frm_addr0_tag
{
    struct _dcam_frm_addr0_map
    {
        volatile unsigned int reserved      :14;
        volatile unsigned int frm_addr_0    :18;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_0_U;

typedef union _dcam_frm_addr1_tag
{
    struct _dcam_frm_addr1_map
    {
        volatile unsigned int reserved      :14;
        volatile unsigned int frm_addr_1    :18;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_1_U;

typedef union _dcam_frm_addr2_tag
{
    struct _dcam_frm_addr2_map
    {
        volatile unsigned int reserved      :14;
        volatile unsigned int frm_addr_2    :18;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_2_U;

typedef union _dcam_frm_addr3_tag
{
    struct _dcam_frm_addr3_map
    {
        volatile unsigned int reserved      :14;
        volatile unsigned int frm_addr_3    :18;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_3_U;

typedef union _dcam_frm_addr4_tag
{
    struct _dcam_frm_addr4_map
    {
        volatile unsigned int reserved      :14;
        volatile unsigned int frm_addr_4    :18;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_4_U;

typedef union _dcam_frm_addr5_tag
{
    struct _dcam_frm_addr5_map
    {
        volatile unsigned int reserved      :14;
        volatile unsigned int frm_addr_5    :18;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_5_U;

typedef union _dcam_frm_addr6_tag
{
    struct _dcam_frm_addr6_map
    {
        volatile unsigned int reserved      :14;
        volatile unsigned int frm_addr_6    :18;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_6_U;

typedef union _dcam_frm_addr7_tag
{
    struct _dcam_frm_addr7_map
    {
        volatile unsigned int reserved      :14;
        volatile unsigned int frm_addr_7    :18;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_7_U;

typedef union _dcam_frm_high_tag
{
    struct _dcam_frm_high_map
    {
        volatile unsigned int reserved      :26;
        volatile unsigned int frm_addr_h    :6;
    } mBits;
    volatile unsigned int dwValue;
} FRM_ADDR_HIGH_U;

typedef union _dcam_burst_cap_tag
{
    struct _dcam_burst_cap_map
    {
        volatile unsigned int reserved      :27;
        volatile unsigned int burst_gap     :5;
    } mBits;
    volatile unsigned int dwValue;
} BURST_GAP_U;

typedef union _dcam_endain_sel_tag
{
    struct dcam_endain_sel_map
    {
        volatile unsigned int reserved      :27;
        volatile unsigned int endian_sel_rd :5;
        volatile unsigned int endian_sel_wr :5;
    } mBits;
    volatile unsigned int dwValue;
} ENDAIN_SEL_U;

typedef union _dcam_ahbm_sts_tag
{
    struct _dcam_ahbm_sts_map
    {
        volatile unsigned int reserved      :30;
        volatile unsigned int ahbm_busy     :1;
    } mBits;
    volatile unsigned int dwValue;
} AHBM_STS_U;
//ahb master reg define end

//cap reg define being
typedef union _cap_ctrl_tag
{
    struct _cap_ctrl_map
    {
        volatile unsigned int reserved1         :19;
        volatile unsigned int cap_ccir_pd       :1;
        volatile unsigned int cap_ccir_rst      :1;
        volatile unsigned int fifo_data_rate    :2;
        volatile unsigned int yuv_type          :2;
        volatile unsigned int cut_first_y0      :1;
        volatile unsigned int reserved0         :1;
        volatile unsigned int vsync_pol         :1;
        volatile unsigned int hsync_pol         :1;
        volatile unsigned int tv_frame_sel      :2;
        volatile unsigned int ccir_656          :1;
    } mBits;
    volatile unsigned int dwValue;
} CAP_CTRL_U;

typedef union _cap_frm_cnt_tag
{
    struct _cap_frm_cnt_map
    {
        volatile unsigned int reserved2     :9;
        volatile unsigned int cap_frm_clr   :1;
        volatile unsigned int cap_frm_cnt   :6;
        volatile unsigned int reserved1     :4;
        volatile unsigned int cap_frm_deci  :4;
        volatile unsigned int reserved0     :2;
        volatile unsigned int pre_skip_cnt  :6;
    } mBits;
    volatile unsigned int dwValue;
} CAP_FRM_CNT_U;

typedef union _cap_start_tag
{
    struct _cap_start_map
    {
        volatile unsigned int reserved1 :3;
        volatile unsigned int start_y   :13;
        volatile unsigned int reserved0 :3;
        volatile unsigned int start_x   :13;
    } mBits;
    volatile unsigned int dwValue;
} CAP_START_U;

typedef union _cap_end_tag
{
    struct _cap_end_map
    {
        volatile unsigned int reserved1 :3;
        volatile unsigned int end_y     :13;
        volatile unsigned int reserved0 :3;
        volatile unsigned int end_x     :13;
    } mBits;
    volatile unsigned int dwValue;
} CAP_END_U;

typedef union _cap_image_deci_tag
{
    struct _cap_image_deci_map
    {
        volatile unsigned int reserved      :23;
        volatile unsigned int cap_deci_mode :1;
        volatile unsigned int cap_deci_y    :4;
        volatile unsigned int cap_deci_x    :4;
    } mBits;
    volatile unsigned int dwValue;
} CAP_IMAGE_DECI_U;

typedef union _cap_fix_mode_tag
{
    struct _cap_fix_mode_map
    {
        volatile unsigned int reserved          :12;
        volatile unsigned int threshold_end     :9;
        volatile unsigned int threshold_start   :9;
        volatile unsigned int threshold_mode    :1;
        volatile unsigned int threshold_eb      :1;
    } mBits;
    volatile unsigned int dwValue;
} CAP_FIX_MODE_U;

typedef union _cap_observe_tag
{
    struct _cap_observe_map
    {
        volatile unsigned int reserved          :16;
        volatile unsigned int cap_fifo_cnt      :5;
        volatile unsigned int cap_fifo_max      :5;
        volatile unsigned int cap_observe_eb    :1;
    } mBits;
    volatile unsigned int dwValue;
} CAP_OBSERVE_U;

typedef union _cap_jpeg_frm_ctrl_tag
{
    struct _cap_jpeg_frm_ctrl_map
    {
        volatile unsigned int reserved1         :13;
        volatile unsigned int dsp_log_buf_sel   :2;
        volatile unsigned int dsp_log_mode      :1;
        volatile unsigned int mem_size          :8;
        volatile unsigned int jpeg_drop_num     :4;
        volatile unsigned int reserved0         :1;
        volatile unsigned int get_num           :3;
    } mBits;
    volatile unsigned int dwValue;
} CAP_JPEG_FRM_CTRL_U;

typedef union _cap_jpeg_frm_size_tag
{
    struct _cap_jpeg_frm_size_map
    {
        volatile unsigned int reserved          :8;
        volatile unsigned int jpeg_size         :24;
    } mBits;
    volatile unsigned int dwValue;
} CAP_JPEG_FRM_SIZE_U;

//trim&scale
typedef union _scaling_cfg_tag
{
    struct _scaling_cfg_map
    {
        volatile unsigned int reserved          :20;
        volatile unsigned int sub_sample_mode   :2;
        volatile unsigned int sub_sample_eb     :1;
        volatile unsigned int review_start      :1;
        volatile unsigned int scale_mode        :1;
        volatile unsigned int input_format      :2;
        volatile unsigned int trim_eb           :1;
        volatile unsigned int ver_down_tap      :3;
        volatile unsigned int scale_bypass      :1;
    } mBits;
    volatile unsigned int dwValue;
} SCALING_CFG_U;

typedef union _trim_start_tag
{
    struct _trim_start_map
    {
        volatile unsigned int reserved1 :4;
        volatile unsigned int start_y   :12;
        volatile unsigned int reserved0 :4;
        volatile unsigned int start_x   :12;
    } mBits;
    volatile unsigned int dwValue;
} TRIM_START_U;

typedef union _trim_size_tag
{
    struct _trim_size_map
    {
        volatile unsigned int reserved1 :4;
        volatile unsigned int size_y    :12;
        volatile unsigned int reserved0 :4;
        volatile unsigned int size_x    :12;
    } mBits;
    volatile unsigned int dwValue;
} TRIM_SIZE_U;

typedef union _slice_ver_size_tag
{
    struct _slice_ver_size_map
    {
        volatile unsigned int reserved1         :4;
        volatile unsigned int slice_line_output :12;
        volatile unsigned int reserved0         :3;
        volatile unsigned int last_slice        :1;
        volatile unsigned int slice_line_input  :12;
    } mBits;
    volatile unsigned int dwValue;
} SLICE_VER_SIZE_U;
#endif   //CHIP_ENDIAN_LITTLE
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif
// End
