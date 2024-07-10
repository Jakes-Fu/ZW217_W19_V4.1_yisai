/******************************************************************************
 ** File Name:     sc6530_reg_isp.h                                           *
 ** Author:        Baggio.he                                                  *
 ** DATE:          10/18/2011                                                 *
 ** Copyright:     2011 Spreatrum, Incoporated. All Rights Reserved.          *
 ** Description:                                                              *
 ******************************************************************************/
 
/******************************************************************************
 **                   Edit    History                                         *
 ****************************************************************************** 
 ** DATE           NAME            DESCRIPTION                       Version  *
 ** 10/18/2011     Baggio.he       Initial version for ISP driver.    1.0     * 
 ** 05/01/2016     Eddy.he         Initial version for ISP driver.    SC6531EFM  *
 **                                the difference between SC6531EFM&SC6530/SC6531*
 **                                1. SC6531EFM delete the denoise hardware      *
 **                                   register: 0x2020_0140                   *
 **                                2. SC6531EFM add 2 spi sensor interface:      *
 **                                   a. ov spi mode/ ov packet mode          *
 **                                     (ov packet mode has no line sync code)*
 **                                   b. spi ddr mode(only for packet mode)   *
 ******************************************************************************/
 
#ifndef _SC6530_REG_ISP_
#define _SC6530_REG_ISP_

#include "sci_types.h"
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

/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
//top module reg define being
#ifdef __BIG_ENDIAN
typedef struct _ISP_REG_T_
{
    union _DCAM_CFG_TAG 
    {
        struct _DCAM_CFG_MAP
        {
            volatile uint32 reserved1             :24;
            volatile uint32 clk_status            :1;
            volatile uint32 reserved0             :1;
            volatile uint32 cap_eb                :1;
            volatile uint32 clk_switch            :1;            	
            volatile uint32 vsp_eb                :1;
            volatile uint32 cap_mode              :1;
            volatile uint32 isp_mode              :1;
            volatile uint32 dcam_mode             :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_cfg_u;	
    
    union _DCAM_CAP_SHADOW_CTL_TAG
    {
        struct _DCAM_CAP_SHADOW_CTL_MAP
        {
            volatile uint32 reserved              :30;
            volatile uint32 auto_copy_cap         :1;
            volatile uint32 frc_copy_cap          :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_cap_shadow_ctl_u;

    union _DCAM_SC_FMT_TAG
    {
        struct _DCAM_SC_FMT_MAP
        {
            volatile uint32 reserved              :22;
            volatile uint32 rot_mode              :2;
            volatile uint32 rot_eb                :1;
            volatile uint32 sc_input_fmt          :3;
            volatile uint32 rgb565_ext_mode       :1;
            volatile uint32 dithering_eb          :1;
            volatile uint32 sc_output_fmt         :2;
        }mBits;
        volatile uint32 dwValue;
    }dcam_sc_fmt_u;

    union _DCAM_SRC_SIZE_TAG
    {
        struct _DCAM_SRC_SIZE_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 src_size_y            :12;
            volatile uint32 reserved0             :4;
            volatile uint32 src_size_x            :12;
        }mBits;
        volatile uint32 dwValue;
    }dcam_src_size_u;
    
    union _DCAM_DES_SIZE_TAG
    {
        struct _DCAM_DES_SIZE_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 des_size_y            :12;
            volatile uint32 reserved0             :4;
            volatile uint32 des_size_x            :12;
        }mBits;
        volatile uint32 dwValue;
    }dcam_des_size_u;

    union _DCAM_VSP_TIMEOUT_TAG
    {
        struct _DDCAM_VSP_TIMEOUT_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 vsp_timeout_clr       :1;
            volatile uint32 vsp_timeout_eb        :1;
            volatile uint32 vsp_timeout_value     :16;
        }mBits;
        volatile uint32 dwValue;
    }dcam_vsp_timeout_u;

    uint32 reserved_18_20[(0x20-0x18)/4]; 

    union _DCAM_INT_STATUS_TAG
    {
        struct _DCAM_INT_STATUS_MAP
        {
            volatile uint32 reserved              :16;
            volatile uint32 vsp_cmd_timeout       :1;    
            volatile uint32 vsp_cmd_done          :1;            
            volatile uint32 vsp_vld_err           :1;            
            volatile uint32 vsp_timeout           :1;
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 cap_frame_err         :1;              
            volatile uint32 vsp_mbio_done         :1;
            volatile uint32 vsp_vlc_done          :1;
            volatile uint32 vsp_bsm_done          :1;
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 cap_line_err          :1;            
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 sensor_sof            :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_sts_u;

    union _DCAM_INT_MASK_TAG
    {
        struct _DCAM_INT_MASK_MAP
        {
            volatile uint32 reserved              :16;
            volatile uint32 vsp_cmd_timeout       :1;    
            volatile uint32 vsp_cmd_done          :1;            
            volatile uint32 vsp_vld_err           :1;            
            volatile uint32 vsp_timeout           :1;
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 cap_frame_err         :1;              
            volatile uint32 vsp_mbio_done         :1;
            volatile uint32 vsp_vlc_done          :1;
            volatile uint32 vsp_bsm_done          :1;
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 cap_line_err          :1;            
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 sensor_sof            :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_mask_u;

    union _DCAM_INT_CLR_TAG
    {
        struct _DCAM_INT_CLR_MAP
        {
            volatile uint32 reserved              :16;
            volatile uint32 vsp_cmd_timeout       :1;    
            volatile uint32 vsp_cmd_done          :1;            
            volatile uint32 vsp_vld_err           :1;            
            volatile uint32 vsp_timeout           :1;
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 cap_frame_err         :1;              
            volatile uint32 vsp_mbio_done         :1;
            volatile uint32 vsp_vlc_done          :1;
            volatile uint32 vsp_bsm_done          :1;
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 cap_line_err          :1;            
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 sensor_sof            :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_clr_u;

    union _DCAM_INT_RAW_TAG
    {
        struct _DCAM_INT_RAW_TAG_MAP
        {
            volatile uint32 reserved              :16;
            volatile uint32 vsp_cmd_timeout       :1;    
            volatile uint32 vsp_cmd_done          :1;            
            volatile uint32 vsp_vld_err           :1;            
            volatile uint32 vsp_timeout           :1;
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 cap_frame_err         :1;              
            volatile uint32 vsp_mbio_done         :1;
            volatile uint32 vsp_vlc_done          :1;
            volatile uint32 vsp_bsm_done          :1;
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 cap_line_err          :1;            
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 sensor_sof            :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_raw_u;

    uint32 reserved_30_40[(0x40-0x30)/4]; 

    union _DCAM_FRAME0_ADDR_TAG
    {
        struct _DCAM_FRAME0_ADDR_MAP
        {
            volatile uint32 frm_addr_0            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame0_addr_u;

    union _DCAM_FRAME1_ADDR_TAG
    {
        struct _DCAM_FRAME1_ADDR_MAP
        {
            volatile uint32 frm_addr_1            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame1_addr_u;

    union _DCAM_FRAME2_ADDR_TAG
    {
        struct _DCAM_FRAME2_ADDR_MAP
        {
            volatile uint32 frm_addr_2            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame2_addr_u;

    union _DCAM_FRAME3_ADDR_TAG
    {
        struct _DCAM_FRAME3_ADDR_MAP
        {
            volatile uint32 frm_addr_3            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame3_addr_u;

    union _DCAM_FRAME4_ADDR_TAG
    {
        struct _DCAM_FRAME4_ADDR_MAP
        {
            volatile uint32 frm_addr_4            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame4_addr_u;

    union _DCAM_FRAME5_ADDR_TAG
    {
        struct _DCAM_FRAME5_ADDR_MAP
        {
            volatile uint32 frm_addr_5            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame5_addr_u;

    union _DCAM_FRAME6_ADDR_TAG
    {
        struct _DCAM_FRAME6_ADDR_MAP
        {
            volatile uint32 frm_addr_6            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame6_addr_u;

    uint32 reserved_5C_60; 
    
    union _DCAM_BURST_GAP_TAG
    {
        struct _DCAM_BURST_GAP_MAP
        {
            volatile uint32 reserved              :27;
            volatile uint32 burst_gap             :5;
        }mBits;
        volatile uint32 dwValue;
    }dcam_burst_gap_u;

    union _DCAM_ENDIAN_SEL_TAG
    {
        struct _DCAM_ENDIAN_SEL_MAP
        {
            volatile uint32 reserved              :28;
            volatile uint32 endian_sel_wr         :2;
            volatile uint32 endian_sel_rd         :2;
        }mBits;
        volatile uint32 dwValue;
    }dcam_endian_sel_u;

    union _DCAM_AHBM_STATUS_TAG
    {
        struct _DCAM_AHBM_STATUS_MAP
        {
            volatile uint32 reserved              :31;
            volatile uint32 ahbm_busy             :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_ahbm_status_u;
    
    union _DCAM_FRAME7_ADDR_TAG
    {
        struct _DCAM_FRAME7_ADDR_MAP
        {
            volatile uint32 frm_addr_7            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame7_addr_u;

    union _DCAM_FRAME8_ADDR_TAG
    {
        struct _DCAM_FRAME8_ADDR_MAP
        {
            volatile uint32 frm_addr_8            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame8_addr_u;

    union _DCAM_FRAME9_ADDR_TAG
    {
        struct _DCAM_FRAME9_ADDR_MAP
        {
            volatile uint32 frm_addr_9            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame9_addr_u;
    
    union _DCAM_VSP_CMD_DBG0_TAG
    {
        struct _DCAM_VSP_CMD_DBG0_MAP
        {
            volatile uint32 vsp_cmd_dbg0          :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_vsp_cmd_dbg0_u;
    
    union _DCAM_VSP_CMD_DBG1_TAG
    {
        struct _DCAM_VSP_CMD_DBG1_MAP
        {
            volatile uint32 vsp_cmd_dbg1          :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_vsp_cmd_dbg1_u;

    uint32 reserved_80_100[(0x100-0x80)/4]; 
    
    union _CAP_CTRL_TAG
    {
        struct _CAP_CTRL_MAP
        {
            volatile uint32 reserved2             :13;
            volatile uint32 spi_ddr_mode          :1;
            volatile uint32 ov_spi_mode           :1;
            volatile uint32 package_mode_eb       :1;
            volatile uint32 reserved1             :3;
            volatile uint32 cap_ccir_pd           :1;
            volatile uint32 cap_ccir_rst          :1;
            volatile uint32 cap_if_mode           :2;
            volatile uint32 yuv_type              :2;
            volatile uint32 reserved0             :1;
            volatile uint32 cap_if_endian         :1;
            volatile uint32 vsync_pol             :1;
            volatile uint32 hsync_pol             :1;
            volatile uint32 sensor_mode           :2;
            volatile uint32 ccir_656              :1;
        }mBits;
        volatile uint32 dwValue;
    }cap_ctrl_u;

    union _CAP_FRAME_CTRL_TAG
    {
        struct _CAP_FRAME_CTRL_MAP
        {
            volatile uint32 reserved1             :9;
            volatile uint32 cap_frm_clr           :1;
            volatile uint32 cap_frm_cnt           :6;
            volatile uint32 reserved0             :10;
            volatile uint32 cap_frm_deci          :2;
            volatile uint32 pre_skip_cnt          :4;
        }mBits;
        volatile uint32 dwValue;
    }cap_frm_ctrl_u;

    union _CAP_START_TAG
    {
        struct _CAP_START_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 start_y               :12;
            volatile uint32 reserved0             :3;
            volatile uint32 start_x               :13;
        }mBits;
        volatile uint32 dwValue;
    }cap_start_u;

    union _CAP_END_TAG
    {
        struct _CAP_END_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 end_y                 :12;
            volatile uint32 reserved0             :3;
            volatile uint32 end_x                 :13;
        }mBits;
        volatile uint32 dwValue;
    }cap_end_u;

    union _CAP_IMAGE_DECI_TAG
    {
        struct _CAP_IMAGE_DECI_MAP
        {
            volatile uint32 reserved              :28;
            volatile uint32 cap_deci_y            :2;
            volatile uint32 cap_deci_x            :2;
        }mBits;
        volatile uint32 dwValue;
    }cap_image_deci_u;

    union _CAP_ATV_FIX_MODE_TAG
    {
        struct _CAP_ATV_FIX_MODE_MAP
        {
            volatile uint32 reserved              :7;
            volatile uint32 threshold_end         :12;
            volatile uint32 threshold_start       :12;            
            volatile uint32 threshold_eb          :1;
        }mBits;
        volatile uint32 dwValue;
    }cap_atv_fix_mode_u;
    
    union _CAP_PACKAGE_IMG_INF_TAG
    {
        struct _CAP_PACKAGE_IMG_INF_MAP
        {
            volatile uint32 package_img_width     :16;
            volatile uint32 package_img_height    :16;
        }mBits;
        volatile uint32 dwValue;
    }cap_package_ing_inf_u;

    union _CAP_JPEG_CTRL_TAG
    {
        struct _CAP_JPEG_CTRL_MAP
        {
            volatile uint32 reserved              :22;
            volatile uint32 jpg_mem_size          :10;
        }mBits;
        volatile uint32 dwValue;
    }cap_jpeg_ctrl_u;

    union _CAP_FRM_SIZE_TAG
    {
        struct _CAP_FRM_SIZE_MAP
        {
            volatile uint32 reserved              :8;
            volatile uint32 cap_frm_size          :24;
        }mBits;
        volatile uint32 dwValue;
    }cap_frm_size_u;

    union _CAP_SPI_CFG_TAG
    {
        struct _CAP_SPI_CFG_MAP
        {
            volatile uint32 reserved              :19;
            volatile uint32 spi_orig_width        :13;
        }mBits;
        volatile uint32 dwValue;
    }cap_spi_cfg_u;

    union _CAP_PACKAGE_SYNC_CFG_TAG
    {
        struct _CAP_PACKAGE_SYNC_CFG_MAP
        {
            volatile uint32 package_sod_id        :8;
            volatile uint32 package_sol_id        :8;
            volatile uint32 package_eof_id        :8;
            volatile uint32 package_sof_id        :8;
        }mBits;
        volatile uint32 dwValue;
    }cap_package_sync_cfg_u;

    uint32 reserved_12C_130; 

    union _SCALING_CFG_TAG
    {
        struct _scaling_cfg_map
        {
            volatile uint32 reserved              :21;
            volatile uint32 ver_down_tap          :4;
            volatile uint32 sub_sameple_mode      :2;
            volatile uint32 sub_sample_eb         :1;
            volatile uint32 trim_eb               :1;
            volatile uint32 scale_mode            :1;
            volatile uint32 scale_bypass          :1;
            volatile uint32 review_start          :1;
        }mBits;
        volatile uint32 dwValue;
    }scaling_cfg_u;

    union _TRIM_START_TAG
    {
        struct _TRIM_START_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 start_y               :12;
            volatile uint32 reserved0             :4;
            volatile uint32 start_x               :12;
        }mBits;
        volatile uint32 dwValue;
    }trim_start_u;

    union _TRIM_SIZE_TAG
    {
        struct _TRIM_SIZE_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 size_y                :12;
            volatile uint32 reserved0             :4;
            volatile uint32 size_x                :12;
        }mBits;
        volatile uint32 dwValue;
    }trim_size_u;

    union _SLICE_VER_SIZE_TAG
    {
        struct _SLICE_VER_SIZE_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 slice_line_output     :12;
            volatile uint32 reserved0             :3;
            volatile uint32 last_slice            :1;
            volatile uint32 slice_line_input      :12;
        }mBits;
        volatile uint32 dwValue;
    }slice_ver_size_u;    

    union _DENOISE_CFG_TAG
    {
        struct _DENOISE_CFG_MAP
        {
            volatile uint32 reserved0             :15;
            volatile uint32 denoise_eb            :1;
            volatile uint32 denoise_th_uv         :8;
            volatile uint32 denoise_th_y          :8;
        }mBits;
        volatile uint32 dwValue;
    }denoise_cfg_u; 
    
}ISP_REG_T;

#else  // Little endian

typedef struct _ISP_REG_T_
{
    union _DCAM_CFG_TAG 
    {
        struct _DCAM_CFG_MAP
        {
            volatile uint32 dcam_mode             :1;
            volatile uint32 isp_mode              :1;
            volatile uint32 cap_mode              :1;
            volatile uint32 vsp_eb                :1;
            volatile uint32 clk_switch            :1;            	
            volatile uint32 cap_eb                :1;
            volatile uint32 reserved0             :1;
            volatile uint32 clk_status            :1;
            volatile uint32 reserved1             :24;
        }mBits;
        volatile uint32 dwValue;
    }dcam_cfg_u;	
    
    union _DCAM_CAP_SHADOW_CTL_TAG
    {
        struct _DCAM_CAP_SHADOW_CTL_MAP
        {
            volatile uint32 frc_copy_cap          :1;
            volatile uint32 auto_copy_cap         :1;
            volatile uint32 reserved              :30;
        }mBits;
        volatile uint32 dwValue;
    }dcam_cap_shadow_ctl_u;

    union _DCAM_SC_FMT_TAG
    {
        struct _DCAM_SC_FMT_MAP
        {
            volatile uint32 sc_output_fmt         :2;
            volatile uint32 dithering_eb          :1;
            volatile uint32 rgb565_ext_mode       :1;
            volatile uint32 sc_input_fmt          :3;
            volatile uint32 rot_eb                :1;
            volatile uint32 rot_mode              :2;
            volatile uint32 reserved              :22;
        }mBits;
        volatile uint32 dwValue;
    }dcam_sc_fmt_u;

    union _DCAM_SRC_SIZE_TAG
    {
        struct _DCAM_SRC_SIZE_MAP
        {
            volatile uint32 src_size_x            :12;
            volatile uint32 reserved0             :4;
            volatile uint32 src_size_y            :12;
            volatile uint32 reserved1             :4;
        }mBits;
        volatile uint32 dwValue;
    }dcam_src_size_u;
    
    union _DCAM_DES_SIZE_TAG
    {
        struct _DCAM_DES_SIZE_MAP
        {
            volatile uint32 des_size_x            :12;
            volatile uint32 reserved0             :4;
            volatile uint32 des_size_y            :12;
            volatile uint32 reserved1             :4;
        }mBits;
        volatile uint32 dwValue;
    }dcam_des_size_u;

    union _DCAM_VSP_TIMEOUT_TAG
    {
        struct _DDCAM_VSP_TIMEOUT_MAP
        {
            volatile uint32 vsp_timeout_value     :16;
            volatile uint32 vsp_timeout_eb        :1;
            volatile uint32 vsp_timeout_clr       :1;
            volatile uint32 reserved              :14;
        }mBits;
        volatile uint32 dwValue;
    }dcam_vsp_timeout_u;

    uint32 reserved_18_20[(0x20-0x18)/4]; 

    union _DCAM_INT_STATUS_TAG
    {
        struct _DCAM_INT_STATUS_MAP
        {
            volatile uint32 sensor_sof            :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_line_err          :1;            
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 vsp_bsm_done          :1;
            volatile uint32 vsp_vlc_done          :1;
            volatile uint32 vsp_mbio_done         :1;
            volatile uint32 cap_frame_err         :1;              
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 vsp_timeout           :1;
            volatile uint32 vsp_vld_err           :1;            
            volatile uint32 vsp_cmd_done          :1;            
            volatile uint32 vsp_cmd_timeout       :1;    
            volatile uint32 reserved              :16;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_sts_u;

    union _DCAM_INT_MASK_TAG
    {
        struct _DCAM_INT_MASK_MAP
        {
            volatile uint32 sensor_sof            :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_line_err          :1;            
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 vsp_bsm_done          :1;
            volatile uint32 vsp_vlc_done          :1;
            volatile uint32 vsp_mbio_done         :1;
            volatile uint32 cap_frame_err         :1;              
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 vsp_timeout           :1;
            volatile uint32 vsp_vld_err           :1;            
            volatile uint32 vsp_cmd_done          :1;            
            volatile uint32 vsp_cmd_timeout       :1;    
            volatile uint32 reserved              :16;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_mask_u;

    union _DCAM_INT_CLR_TAG
    {
        struct _DCAM_INT_CLR_MAP
        {
            volatile uint32 sensor_sof            :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_line_err          :1;            
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 vsp_bsm_done          :1;
            volatile uint32 vsp_vlc_done          :1;
            volatile uint32 vsp_mbio_done         :1;
            volatile uint32 cap_frame_err         :1;              
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 vsp_timeout           :1;
            volatile uint32 vsp_vld_err           :1;            
            volatile uint32 vsp_cmd_done          :1;            
            volatile uint32 vsp_cmd_timeout       :1;    
            volatile uint32 reserved              :16;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_clr_u;

    union _DCAM_INT_RAW_TAG
    {
        struct _DCAM_INT_RAW_TAG_MAP
        {
            volatile uint32 sensor_sof            :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_line_err          :1;            
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 vsp_bsm_done          :1;
            volatile uint32 vsp_vlc_done          :1;
            volatile uint32 vsp_mbio_done         :1;
            volatile uint32 cap_frame_err         :1;              
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 vsp_timeout           :1;
            volatile uint32 vsp_vld_err           :1;            
            volatile uint32 vsp_cmd_done          :1;            
            volatile uint32 vsp_cmd_timeout       :1;    
            volatile uint32 reserved              :16;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_raw_u;

    uint32 reserved_30_40[(0x40-0x30)/4]; 

    union _DCAM_FRAME0_ADDR_TAG
    {
        struct _DCAM_FRAME0_ADDR_MAP
        {
            volatile uint32 frm_addr_0            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame0_addr_u;

    union _DCAM_FRAME1_ADDR_TAG
    {
        struct _DCAM_FRAME1_ADDR_MAP
        {
            volatile uint32 frm_addr_1            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame1_addr_u;

    union _DCAM_FRAME2_ADDR_TAG
    {
        struct _DCAM_FRAME2_ADDR_MAP
        {
            volatile uint32 frm_addr_2            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame2_addr_u;

    union _DCAM_FRAME3_ADDR_TAG
    {
        struct _DCAM_FRAME3_ADDR_MAP
        {
            volatile uint32 frm_addr_3            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame3_addr_u;

    union _DCAM_FRAME4_ADDR_TAG
    {
        struct _DCAM_FRAME4_ADDR_MAP
        {
            volatile uint32 frm_addr_4            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame4_addr_u;

    union _DCAM_FRAME5_ADDR_TAG
    {
        struct _DCAM_FRAME5_ADDR_MAP
        {
            volatile uint32 frm_addr_5            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame5_addr_u;

    union _DCAM_FRAME6_ADDR_TAG
    {
        struct _DCAM_FRAME6_ADDR_MAP
        {
            volatile uint32 frm_addr_6            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame6_addr_u;

    uint32 reserved_5C_60; 
    
    union _DCAM_BURST_GAP_TAG
    {
        struct _DCAM_BURST_GAP_MAP
        {
            volatile uint32 burst_gap             :5;
            volatile uint32 reserved              :27;
        }mBits;
        volatile uint32 dwValue;
    }dcam_burst_gap_u;

    union _DCAM_ENDIAN_SEL_TAG
    {
        struct _DCAM_ENDIAN_SEL_MAP
        {
            volatile uint32 endian_sel_rd         :2;
            volatile uint32 endian_sel_wr         :2;
            volatile uint32 reserved              :28;
        }mBits;
        volatile uint32 dwValue;
    }dcam_endian_sel_u;

    union _DCAM_AHBM_STATUS_TAG
    {
        struct _DCAM_AHBM_STATUS_MAP
        {
            volatile uint32 ahbm_busy             :1;
            volatile uint32 reserved              :31;
        }mBits;
        volatile uint32 dwValue;
    }dcam_ahbm_status_u;
    
    union _DCAM_FRAME7_ADDR_TAG
    {
        struct _DCAM_FRAME7_ADDR_MAP
        {
            volatile uint32 frm_addr_7            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame7_addr_u;

    union _DCAM_FRAME8_ADDR_TAG
    {
        struct _DCAM_FRAME8_ADDR_MAP
        {
            volatile uint32 frm_addr_8            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame8_addr_u;

    union _DCAM_FRAME9_ADDR_TAG
    {
        struct _DCAM_FRAME9_ADDR_MAP
        {
            volatile uint32 frm_addr_9            :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame9_addr_u;
    
    union _DCAM_VSP_CMD_DBG0_TAG
    {
        struct _DCAM_VSP_CMD_DBG0_MAP
        {
            volatile uint32 vsp_cmd_dbg0          :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_vsp_cmd_dbg0_u;
    
    union _DCAM_VSP_CMD_DBG1_TAG
    {
        struct _DCAM_VSP_CMD_DBG1_MAP
        {
            volatile uint32 vsp_cmd_dbg1          :32;
        }mBits;
        volatile uint32 dwValue;
    }dcam_vsp_cmd_dbg1_u;

    uint32 reserved_80_100[(0x100-0x80)/4]; 
    
    union _CAP_CTRL_TAG
    {
        struct _CAP_CTRL_MAP
        {
            volatile uint32 ccir_656              :1;
            volatile uint32 sensor_mode           :2;
            volatile uint32 hsync_pol             :1;
            volatile uint32 vsync_pol             :1;
            volatile uint32 cap_if_endian         :1;
            volatile uint32 reserved0             :1;
            volatile uint32 yuv_type              :2;
            volatile uint32 cap_if_mode           :2;
            volatile uint32 cap_ccir_rst          :1;
            volatile uint32 cap_ccir_pd           :1;
            volatile uint32 reserved1             :3;
            volatile uint32 package_mode_eb       :1;
            volatile uint32 ov_spi_mode           :1;
            volatile uint32 spi_ddr_mode          :1;
            volatile uint32 reserved2             :13;
        }mBits;
        volatile uint32 dwValue;
    }cap_ctrl_u;

    union _CAP_FRAME_CTRL_TAG
    {
        struct _CAP_FRAME_CTRL_MAP
        {
            volatile uint32 pre_skip_cnt          :4;
            volatile uint32 cap_frm_deci          :2;
            volatile uint32 reserved0             :10;
            volatile uint32 cap_frm_cnt           :6;
            volatile uint32 cap_frm_clr           :1;
            volatile uint32 reserved1             :9;
        }mBits;
        volatile uint32 dwValue;
    }cap_frm_ctrl_u;

    union _CAP_START_TAG
    {
        struct _CAP_START_MAP
        {
            volatile uint32 start_x               :13;
            volatile uint32 reserved0             :3;
            volatile uint32 start_y               :12;
            volatile uint32 reserved1             :4;
        }mBits;
        volatile uint32 dwValue;
    }cap_start_u;

    union _CAP_END_TAG
    {
        struct _CAP_END_MAP
        {
            volatile uint32 end_x                 :13;
            volatile uint32 reserved0             :3;
            volatile uint32 end_y                 :12;
            volatile uint32 reserved1             :4;
        }mBits;
        volatile uint32 dwValue;
    }cap_end_u;

    union _CAP_IMAGE_DECI_TAG
    {
        struct _CAP_IMAGE_DECI_MAP
        {
            volatile uint32 cap_deci_x            :2;
            volatile uint32 cap_deci_y            :2;
            volatile uint32 reserved              :28;
        }mBits;
        volatile uint32 dwValue;
    }cap_image_deci_u;

    union _CAP_ATV_FIX_MODE_TAG
    {
        struct _CAP_ATV_FIX_MODE_MAP
        {
            volatile uint32 threshold_eb          :1;
            volatile uint32 threshold_start       :12; 
            volatile uint32 threshold_end         :12;
            volatile uint32 reserved              :7;
        }mBits;
        volatile uint32 dwValue;
    }cap_atv_fix_mode_u;
    
    union _CAP_PACKAGE_IMG_INF_TAG
    {
        struct _CAP_PACKAGE_IMG_INF_MAP
        {
            volatile uint32 package_img_height    :16;
            volatile uint32 package_img_width     :16;
        }mBits;
        volatile uint32 dwValue;
    }cap_package_ing_inf_u;

    union _CAP_JPEG_CTRL_TAG
    {
        struct _CAP_JPEG_CTRL_MAP
        {
            volatile uint32 jpg_mem_size          :10;
            volatile uint32 reserved              :22;
        }mBits;
        volatile uint32 dwValue;
    }cap_jpeg_ctrl_u;

    union _CAP_FRM_SIZE_TAG
    {
        struct _CAP_FRM_SIZE_MAP
        {
            volatile uint32 cap_frm_size          :24;
            volatile uint32 reserved              :8;
        }mBits;
        volatile uint32 dwValue;
    }cap_frm_size_u;

    union _CAP_SPI_CFG_TAG
    {
        struct _CAP_SPI_CFG_MAP
        {
            volatile uint32 spi_orig_width        :13;
            volatile uint32 reserved              :19;
        }mBits;
        volatile uint32 dwValue;
    }cap_spi_cfg_u;

    union _CAP_PACKAGE_SYNC_CFG_TAG
    {
        struct _CAP_PACKAGE_SYNC_CFG_MAP
        {
            volatile uint32 package_sof_id        :8;
            volatile uint32 package_eof_id        :8;
            volatile uint32 package_sol_id        :8;
            volatile uint32 package_sod_id        :8;
        }mBits;
        volatile uint32 dwValue;
    }cap_package_sync_cfg_u;

    uint32 reserved_12C_130; 

    union _SCALING_CFG_TAG
    {
        struct _scaling_cfg_map
        {
            volatile uint32 review_start          :1;
            volatile uint32 scale_bypass          :1;
            volatile uint32 scale_mode            :1;
            volatile uint32 trim_eb               :1;
            volatile uint32 sub_sample_eb         :1;
            volatile uint32 sub_sameple_mode      :2;
            volatile uint32 ver_down_tap          :4;
            volatile uint32 reserved              :21;
        }mBits;
        volatile uint32 dwValue;
    }scaling_cfg_u;

    union _TRIM_START_TAG
    {
        struct _TRIM_START_MAP
        {
            volatile uint32 start_x               :12;
            volatile uint32 reserved0             :4;
            volatile uint32 start_y               :12;
            volatile uint32 reserved1             :4;
        }mBits;
        volatile uint32 dwValue;
    }trim_start_u;

    union _TRIM_SIZE_TAG
    {
        struct _TRIM_SIZE_MAP
        {
            volatile uint32 size_x                :12;
            volatile uint32 reserved0             :4;
            volatile uint32 size_y                :12;
            volatile uint32 reserved1             :4;
        }mBits;
        volatile uint32 dwValue;
    }trim_size_u;

    union _SLICE_VER_SIZE_TAG
    {
        struct _SLICE_VER_SIZE_MAP
        {
            volatile uint32 slice_line_input      :12;
            volatile uint32 last_slice            :1;
            volatile uint32 reserved0             :3;
            volatile uint32 slice_line_output     :12;
            volatile uint32 reserved1             :4;
        }mBits;
        volatile uint32 dwValue;
    }slice_ver_size_u;    

    union _DENOISE_CFG_TAG
    {
        struct _DENOISE_CFG_MAP
        {
            volatile uint32 denoise_th_y          :8;
            volatile uint32 denoise_th_uv         :8;
            volatile uint32 denoise_eb            :1;
            volatile uint32 reserved0             :15;
        }mBits;
        volatile uint32 dwValue;
    }denoise_cfg_u; 
    
}ISP_REG_T;

#endif
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif //for _SC6530_REG_ISP_


