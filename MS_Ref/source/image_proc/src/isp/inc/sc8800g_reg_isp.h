/******************************************************************************
 ** File Name:    sc8800g_reg_isp.h                                          *
 ** Author:        Baggio.he                                               		*
 ** DATE:          03/10/2010                                                 *
 ** Copyright:    2008 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
 
/******************************************************************************
 **                   Edit    History                                         *
 ****************************************************************************** 
 ** DATE           NAME            DESCRIPTION                       Version  *
 ** 03/10/2010     Baggio.he       Initial version for ISP driver.    1.0     * 
 ******************************************************************************/
 
#ifndef _SC8800G_REG_ISP_
#define _SC8800G_REG_ISP_
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
            volatile uint32 reserved              :25;
            volatile uint32 path2_clk_status      :1;
            volatile uint32 path1_clk_status      :1;
            volatile uint32 path2_clk_switch      :1;            	
            volatile uint32 path1_clk_switch      :1;
            volatile uint32 review_path_eb        :1;
            volatile uint32 cam_path2_eb          :1;
            volatile uint32 cam_path1_eb          :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_cfg_u;	
    
    union _DCAM_PATH_CFG_TAG
    {
        struct _DCAM_PATH_CFG_MAP
        {
            volatile uint32 reserved              :20;
            volatile uint32 auto_copy_cap         :1;
            volatile uint32 frc_copy_cap          :1;
            volatile uint32 cam2_deci_eb          :1;
            volatile uint32 cam1_trim_eb          :1;
            volatile uint32 ver_down_tap          :4;
            volatile uint32 scale_bypass          :1;
            volatile uint32 cap_input_format      :1;
            volatile uint32 cap_mode              :1;
            volatile uint32 cap_eb                :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_path_cfg_u;

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
            volatile uint32 reserved1             :6;
            volatile uint32 des_size_y            :10;
            volatile uint32 reserved0             :6;
            volatile uint32 des_size_x            :10;
        }mBits;
        volatile uint32 dwValue;
    }dcam_des_size_u;

    union _DCAM1_TRIM_START_TAG
    {
        struct _DCAM1_TRIM_START_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 start_y               :12;
            volatile uint32 reserved0             :4;
            volatile uint32 start_x               :12;
        }mBits;
        volatile uint32 dwValue;
    }dcam1_trim_start_u;

    union _DCAM1_TRIM_SIZE_TAG
    {
        struct _DCAM1_TRIM_SIZE_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 size_y                :12;
            volatile uint32 reserved0             :4;
            volatile uint32 size_x                :12;
        }mBits;
        volatile uint32 dwValue;
    }dcam1_trim_size_u;

    union _REVIEW_PATH_CFG_TAG
    {
        struct _review_path_cfg_map
        {
            volatile uint32 reserved              :14;
            volatile uint32 ver_down_tap          :4;
            volatile uint32 rgb_input_format      :1;
            volatile uint32 yuv_input_format      :2;
            volatile uint32 sub_sameple_mode      :2;
            volatile uint32 dither_eb             :1;
            volatile uint32 output_format         :2;
            volatile uint32 input_format          :1;
            volatile uint32 scale_mode            :1;
            volatile uint32 scale_bypass          :1;
            volatile uint32 sub_sample_eb         :1;
            volatile uint32 trim_eb               :1;
            volatile uint32 review_start          :1;
        }mBits;
        volatile uint32 dwValue;
    }review_path_cfg_u;

    union _REVIEW_SRC_SIZE_TAG
    {
        struct _REVIEW_SRC_SIZE_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 src_size_y            :12;
            volatile uint32 reserved0             :4;
            volatile uint32 src_size_x            :12;
        }mBits;
        volatile uint32 dwValue;
    }review_src_size_u;

    union _REVIEW_DES_SIZE_TAG
    {
        struct _REVIEW_DES_SIZE_MAP
        {
            volatile uint32 reserved1             :4;
            volatile uint32 des_size_y            :12;
            volatile uint32 reserved0             :4;
            volatile uint32 des_size_x            :12;
        }mBits;
        volatile uint32 dwValue;
    }review_des_size_u;

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

    union _DCAM_INT_STATUS_TAG
    {
        struct _DCAM_INT_STATUS_MAP
        {
            volatile uint32 reserved              :22;
            volatile uint32 review_done           :1;
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 sensor_frame_err      :1;
            volatile uint32 sensor_line_err       :1;
            volatile uint32 cap_buf_ovf           :1;
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
            volatile uint32 reserved              :22;
            volatile uint32 review_done           :1;
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 sensor_frame_err      :1;
            volatile uint32 sensor_line_err       :1;
            volatile uint32 cap_buf_ovf           :1;
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
            volatile uint32 reserved              :22;
            volatile uint32 review_done           :1;
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 sensor_frame_err      :1;
            volatile uint32 sensor_line_err       :1;
            volatile uint32 cap_buf_ovf           :1;
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
            volatile uint32 reserved              :22;
            volatile uint32 review_done           :1;
            volatile uint32 jpg_buf_ovf           :1;
            volatile uint32 sensor_frame_err      :1;
            volatile uint32 sensor_line_err       :1;
            volatile uint32 cap_buf_ovf           :1;
            volatile uint32 isp_tx_done           :1;
            volatile uint32 cap_eof               :1;
            volatile uint32 cap_sof               :1;
            volatile uint32 sensor_eof            :1;
            volatile uint32 sensor_sof            :1;
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_raw_u;

    union _DCAM_FRAME0_ADDR_TAG
    {
        struct _DCAM_FRAME0_ADDR_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_0            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame0_addr_u;

    union _DCAM_FRAME1_ADDR_TAG
    {
        struct _DCAM_FRAME1_ADDR_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_1            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame1_addr_u;

    union _DCAM_FRAME2_ADDR_TAG
    {
        struct _DCAM_FRAME2_ADDR_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_2            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame2_addr_u;

    union _DCAM_FRAME3_ADDR_TAG
    {
        struct _DCAM_FRAME3_ADDR_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_3            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame3_addr_u;

    union _DCAM_FRAME4_ADDR_TAG
    {
        struct _DCAM_FRAME4_ADDR_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_4            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame4_addr_u;

    union _DCAM_FRAME5_ADDR_TAG
    {
        struct _DCAM_FRAME5_ADDR_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_5            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame5_addr_u;

    union _DCAM_FRAME6_ADDR_TAG
    {
        struct _DCAM_FRAME6_ADDR_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_6            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame6_addr_u;

    union _DCAM_FRAME_ADDR_HIGH_TAG
    {
        struct _DCAM_FRAME_ADDR_HIGH_MAP
        {
            volatile uint32 reserved              :26;
            volatile uint32 frm_addr_h            :6;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame_addr_high_u;
    
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
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_7            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame7_addr_u;

    union _DCAM_FRAME8_ADDR_TAG
    {
        struct _DCAM_FRAME8_ADDR_MAP
        {
            volatile uint32 reserved              :14;
            volatile uint32 frm_addr_8            :18;
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame8_addr_u;

    uint32 reserved_70_100[(0x100-0x74)/4]; 

    union _CAP_CTRL_TAG
    {
        struct _CAP_CTRL_MAP
        {
            volatile uint32 reserved1             :19;
            volatile uint32 cap_ccir_pd           :1;
            volatile uint32 cap_ccir_rst          :1;
            volatile uint32 fifo_data_rate        :2;
            volatile uint32 yuv_type              :2;
            volatile uint32 cut_first_y1          :1;
            volatile uint32 reserved0             :1;
            volatile uint32 vsync_pol             :1;
            volatile uint32 hsync_pol             :1;
            volatile uint32 tv_frame_sel          :2;
            volatile uint32 ccir_656              :1;
        }mBits;
        volatile uint32 dwValue;
    }cap_ctrl_u;

    union _CAP_FRAME_COUNT_TAG
    {
        struct _CAP_FRAME_COUNT_MAP
        {
            volatile uint32 reserved2             :9;
            volatile uint32 cap_frm_clr           :1;
            volatile uint32 cap_frm_cnt           :6;
            volatile uint32 reserved1             :4;
            volatile uint32 cap_frm_deci          :4;
            volatile uint32 reserved0             :2;
            volatile uint32 pre_skip_cnt          :6;
        }mBits;
        volatile uint32 dwValue;
    }cap_frm_cnt_u;

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
            volatile uint32 reserved              :23;
            volatile uint32 cap_deci_mode         :1;
            volatile uint32 cap_deci_y            :4;
            volatile uint32 cap_deci_x            :4;
        }mBits;
        volatile uint32 dwValue;
    }cap_image_deci_u;

    uint32 reserved_114_32bits;
	
    union _CAP_OBSERVE_TAG
    {
        struct _CAP_OBSERVE_MAP
        {
            volatile uint32 reserved              :21;     
            volatile uint32 cap_fifo_count        :5;
            volatile uint32 cap_fifo_max          :5;
            volatile uint32 cap_obser_eb          :1;
        }mBits;
        volatile uint32 dwValue;
    }cap_observe_u;

    union _CAP_JPEG_FRM_CTRL_TAG
    {
        struct _CAP_JPEG_FRM_CTRL_MAP
        {
            volatile uint32 reserved1             :16;
            volatile uint32 jpg_mem_size          :8;
            volatile uint32 cap_jpeg_drop_num     :4;
            volatile uint32 reserved0             :1;
            volatile uint32 get_num               :3;
        }mBits;
        volatile uint32 dwValue;
    }cap_jpeg_frm_ctrl_u;

    union _CAP_JPEG_LAST_FRM_SIZE_TAG
    {
        struct _CAP_JPEG_LAST_FRM_SIZE_MAP
        {
            volatile uint32 reserved0             :8;
            volatile uint32 cap_jpg_last_frm_size :24;
        }mBits;
        volatile uint32 dwValue;
    }cap_jpeg_last_frm_size_u;
    
}ISP_REG_T;

#else  // Little endian

typedef struct _ISP_REG_T_
{
    union _DCAM_CFG_TAG 
    {
        struct _DCAM_CFG_MAP
        {
            volatile uint32 cam_path1_eb          :1;
            volatile uint32 cam_path2_eb          :1;            	
            volatile uint32 review_path_eb        :1;            	
            volatile uint32 path1_clk_switch      :1;
            volatile uint32 path2_clk_switch      :1;      
            volatile uint32 path1_clk_status      :1;   
            volatile uint32 path2_clk_status      :1;   
            volatile uint32 reserved              :25;            	      	            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_cfg_u;	
    
    union _DCAM_PATH_CFG_TAG
    {
        struct _DCAM_PATH_CFG_MAP
        {
            volatile uint32 cap_eb                :1;
            volatile uint32 cap_mode              :1;            	
            volatile uint32 cap_input_format      :1;            	
            volatile uint32 scale_bypass          :1;            	
            volatile uint32 ver_down_tap          :4;            	
            volatile uint32 cam1_trim_eb          :1;
            volatile uint32 cam2_deci_eb          :1;
            volatile uint32 frc_copy_cap          :1;
            volatile uint32 auto_copy_cap         :1;
            volatile uint32 reserved              :20;
        }mBits;
        volatile uint32 dwValue;
    }dcam_path_cfg_u;

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
            volatile uint32 des_size_x            :10;
            volatile uint32 reserved0             :6;            	
            volatile uint32 des_size_y            :10;            	
            volatile uint32 reserved1             :6;       
        }mBits;
        volatile uint32 dwValue;
    }dcam_des_size_u;

    union _DCAM1_TRIM_START_TAG
    {
        struct _DCAM1_TRIM_START_MAP
        {
            volatile uint32 start_x               :12;
            volatile uint32 reserved0             :4;            	
            volatile uint32 start_y               :12;            	
            volatile uint32 reserved1             :4;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam1_trim_start_u;

    union _DCAM1_TRIM_SIZE_TAG
    {
        struct _DCAM1_TRIM_SIZE_MAP
        {
            volatile uint32 size_x                :12;
            volatile uint32 reserved0             :4;            	
            volatile uint32 size_y                :12;            	
            volatile uint32 reserved1             :4;            	
            	
        }mBits;
        volatile uint32 dwValue;
    }dcam1_trim_size_u;

    union _REVIEW_PATH_CFG_TAG
    {
        struct _review_path_cfg_map
        {
            volatile uint32 review_start          :1;
            volatile uint32 trim_eb               :1;            	
            volatile uint32 sub_sample_eb         :1;            	
            volatile uint32 scale_bypass          :1;            	
            volatile uint32 scale_mode            :1;            	
            volatile uint32 input_format          :1;
            volatile uint32 output_format         :2;            	
            volatile uint32 dither_eb             :1;            	
            volatile uint32 sub_sameple_mode      :2;            	
            volatile uint32 yuv_input_format      :2;            	
            volatile uint32 rgb_input_format      :1;            	
            volatile uint32 ver_down_tap          :4;            	
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }review_path_cfg_u;

    union _REVIEW_SRC_SIZE_TAG
    {
        struct _REVIEW_SRC_SIZE_MAP
        {
            volatile uint32 src_size_x            :12;
            volatile uint32 reserved0             :4;            	
            volatile uint32 src_size_y            :12;            	
            volatile uint32 reserved1             :4;            	
        }mBits;
        volatile uint32 dwValue;
    }review_src_size_u;

    union _REVIEW_DES_SIZE_TAG
    {
        struct _REVIEW_DES_SIZE_MAP
        {
            volatile uint32 des_size_x            :12;
            volatile uint32 reserved0             :4;            	
            volatile uint32 des_size_y            :12;            	
            volatile uint32 reserved1             :4;            	
            	
        }mBits;
        volatile uint32 dwValue;
    }review_des_size_u;

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

    union _DCAM_INT_STATUS_TAG
    {
        struct _DCAM_INT_STATUS_MAP
        {
            volatile uint32 sensor_sof            :1;
            volatile uint32 sensor_eof            :1;            	
            volatile uint32 cap_sof               :1;            	
            volatile uint32 cap_eof               :1;            	
            volatile uint32 isp_tx_done           :1;            	
            volatile uint32 cap_buf_ovf           :1;            	
            volatile uint32 sensor_line_err       :1;            	
            volatile uint32 sensor_frame_err      :1;            	
            volatile uint32 jpg_buf_ovf           :1;            	
            volatile uint32 review_done           :1;            	
            volatile uint32 reserved              :22;            	
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
            volatile uint32 cap_buf_ovf           :1;            	
            volatile uint32 sensor_line_err       :1;            	
            volatile uint32 sensor_frame_err      :1;    
            volatile uint32 jpg_buf_ovf           :1;            	
            volatile uint32 review_done           :1;            	        	
            volatile uint32 reserved              :22;            	
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
            volatile uint32 cap_buf_ovf           :1;            	
            volatile uint32 sensor_line_err       :1;            	
            volatile uint32 sensor_frame_err      :1;            	
            volatile uint32 jpg_buf_ovf           :1;            	
            volatile uint32 review_done           :1;            	
            volatile uint32 reserved              :22;            	
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
            volatile uint32 cap_buf_ovf           :1;            	
            volatile uint32 sensor_line_err       :1;            	
            volatile uint32 sensor_frame_err      :1;            	
            volatile uint32 jpg_buf_ovf           :1;            	
            volatile uint32 review_done           :1;            	
            volatile uint32 reserved              :22;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_int_raw_u;

    union _DCAM_FRAME0_ADDR_TAG
    {
        struct _DCAM_FRAME0_ADDR_MAP
        {
            volatile uint32 frm_addr_0            :18;
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame0_addr_u;

    union _DCAM_FRAME1_ADDR_TAG
    {
        struct _DCAM_FRAME1_ADDR_MAP
        {
            volatile uint32 frm_addr_1            :18;
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame1_addr_u;

    union _DCAM_FRAME2_ADDR_TAG
    {
        struct _DCAM_FRAME2_ADDR_MAP
        {
            volatile uint32 frm_addr_2            :18;
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame2_addr_u;

    union _DCAM_FRAME3_ADDR_TAG
    {
        struct _DCAM_FRAME3_ADDR_MAP
        {
            volatile uint32 frm_addr_3            :18;
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame3_addr_u;

    union _DCAM_FRAME4_ADDR_TAG
    {
        struct _DCAM_FRAME4_ADDR_MAP
        {
            volatile uint32 frm_addr_4            :18;
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame4_addr_u;

    union _DCAM_FRAME5_ADDR_TAG
    {
        struct _DCAM_FRAME5_ADDR_MAP
        {
            volatile uint32 frm_addr_5            :18;
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame5_addr_u;

    union _DCAM_FRAME6_ADDR_TAG
    {
        struct _DCAM_FRAME6_ADDR_MAP
        {
            volatile uint32 frm_addr_6            :18;
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame6_addr_u;

    union _DCAM_FRAME_ADDR_HIGH_TAG
    {
        struct _DCAM_FRAME_ADDR_HIGH_MAP
        {
            volatile uint32 frm_addr_h            :6;
            volatile uint32 reserved              :26;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame_addr_high_u;
    
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
            volatile uint32 frm_addr_7            :18;
            volatile uint32 reserved              :14;            	
            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame7_addr_u;

    union _DCAM_FRAME8_ADDR_TAG
    {
        struct _DCAM_FRAME8_ADDR_MAP
        {
            volatile uint32 frm_addr_8            :18;
            volatile uint32 reserved              :14;            	
        }mBits;
        volatile uint32 dwValue;
    }dcam_frame8_addr_u;

    uint32 reserved_70_100[(0x100-0x74)/4]; 

    union _CAP_CTRL_TAG
    {
        struct _CAP_CTRL_MAP
        {
            volatile uint32 ccir_656              :1;
            volatile uint32 tv_frame_sel          :2;            	
            volatile uint32 hsync_pol             :1;            	
            volatile uint32 vsync_pol             :1;            	
            volatile uint32 reserved0             :1;            	
            volatile uint32 cut_first_y1          :1;            	
            volatile uint32 yuv_type              :2;            	
            volatile uint32 fifo_data_rate        :2;            	
            volatile uint32 cap_ccir_rst          :1;            	
            volatile uint32 cap_ccir_pd           :1;            	
            volatile uint32 reserved1             :19;            	
        }mBits;
        volatile uint32 dwValue;
    }cap_ctrl_u;

    union _CAP_FRAME_COUNT_TAG
    {
        struct _CAP_FRAME_COUNT_MAP
        {
            volatile uint32 pre_skip_cnt          :6;
            volatile uint32 reserved0             :2;            	
            volatile uint32 cap_frm_deci          :4;            	
            volatile uint32 reserved1             :4;            	
            volatile uint32 cap_frm_cnt           :6;            	
            volatile uint32 cap_frm_clr           :1;            	
            volatile uint32 reserved2             :9;            	
        }mBits;
        volatile uint32 dwValue;
    }cap_frm_cnt_u;

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
            volatile uint32 cap_deci_x            :4;
            volatile uint32 cap_deci_y            :4;            	
            volatile uint32 cap_deci_mode         :1;            	
            volatile uint32 reserved              :23;            	
        }mBits;
        volatile uint32 dwValue;
    }cap_image_deci_u;

    uint32 reserved_114_32bits;

    union _CAP_OBSERVE_TAG
    {
        struct _CAP_OBSERVE_MAP
        {
            volatile uint32 cap_obser_eb          :1;
            volatile uint32 cap_fifo_max          :5;            	
            volatile uint32 cap_fifo_count        :5;            	
            volatile uint32 reserved              :21;               	
        }mBits;
        volatile uint32 dwValue;
    }cap_observe_u;

    union _CAP_JPEG_FRM_CTRL_TAG
    {
        struct _CAP_JPEG_FRM_CTRL_MAP
        {
            volatile uint32 get_num               :3;
            volatile uint32 reserved0             :1;            	
            volatile uint32 cap_jpeg_drop_num     :4;            	
            volatile uint32 jpg_mem_size          :8;            	
            volatile uint32 reserved1             :16;            	
        }mBits;
        volatile uint32 dwValue;
    }cap_jpeg_frm_ctrl_u;

    union _CAP_JPEG_LAST_FRM_SIZE_TAG
    {
        struct _CAP_JPEG_LAST_FRM_SIZE_MAP
        {
            volatile uint32 cap_jpg_last_frm_size :24;
            volatile uint32 reserved0             :8;            	
        }mBits;
        volatile uint32 dwValue;
    }cap_jpeg_last_frm_size_u;
    
}ISP_REG_T;

#endif
/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/
#endif
// End 

