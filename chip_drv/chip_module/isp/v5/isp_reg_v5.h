/******************************************************************************
 ** File Name:    isp_reg_v5.h                                           *
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
#if 0//ndef _ISP_REG_V5_H_
#define _ISP_REG_V5_H_
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
#define DCAM_CFG            				(DCAM_ISP_REG_BASE + 0x0000)
#define CAP_SHADOW_CNTL     		(DCAM_ISP_REG_BASE + 0x0004)
#define SC_FORMAT_CFG         			(DCAM_ISP_REG_BASE + 0x0008)
#define DCAM_SRC_SIZE       			(DCAM_ISP_REG_BASE + 0x000C)
#define ISP_DIS_SIZE        			(DCAM_ISP_REG_BASE + 0x0010)
#define VSP_TIME_OUT        			(DCAM_ISP_REG_BASE + 0x0014)
#define DCAM_INT_STS        			(DCAM_ISP_REG_BASE + 0x0020)
#define DCAM_INT_MASK       			(DCAM_ISP_REG_BASE + 0x0024)
#define DCAM_INT_CLR        			(DCAM_ISP_REG_BASE + 0x0028)
#define DCAM_INT_RAW        			(DCAM_ISP_REG_BASE + 0x002C)

//ahb master
#define FRM_ADDR0					(DCAM_ISP_REG_BASE + 0x0040)
#define FRM_ADDR1					(DCAM_ISP_REG_BASE + 0x0044)
#define FRM_ADDR2					(DCAM_ISP_REG_BASE + 0x0048)
#define FRM_ADDR3					(DCAM_ISP_REG_BASE + 0x004C)
#define FRM_ADDR4					(DCAM_ISP_REG_BASE + 0x0050)
#define FRM_ADDR5 					(DCAM_ISP_REG_BASE + 0x0054)
#define FRM_ADDR6					(DCAM_ISP_REG_BASE + 0x0058)
#define BURST_GAP					(DCAM_ISP_REG_BASE + 0x0060)
#define ENDIAN_SEL					(DCAM_ISP_REG_BASE + 0x0064)
#define AHBM_STS					(DCAM_ISP_REG_BASE + 0x0068)
#define FRM_ADDR7					(DCAM_ISP_REG_BASE + 0x006C)
#define FRM_ADDR8					(DCAM_ISP_REG_BASE + 0x0070)
#define FRM_ADDR9					(DCAM_ISP_REG_BASE + 0x0074)
#define CMD_DBG0					(DCAM_ISP_REG_BASE + 0x0078)
#define CMD_DBG1					(DCAM_ISP_REG_BASE + 0x007C)

//cap
#define CAP_CTRL                			(DCAM_ISP_REG_BASE + 0x0100)
#define CAP_FRM_CNT             			(DCAM_ISP_REG_BASE + 0x0104)
#define CAP_START               			(DCAM_ISP_REG_BASE + 0x0108)
#define CAP_END                 			(DCAM_ISP_REG_BASE + 0x010C)
#define CAP_IMAGE_DECI         		(DCAM_ISP_REG_BASE + 0x0110)
#define CAP_FIX_MODE   				(DCAM_ISP_REG_BASE + 0x0114)
#define PACKAGE_IMG_INF    			(DCAM_ISP_REG_BASE + 0x0118)
#define CAP_JPEG_CTRL       			(DCAM_ISP_REG_BASE + 0x011C)
#define CAP_FRM_SIZE  				(DCAM_ISP_REG_BASE + 0x0120)
#define CAP_SPI_CFG	                   		(DCAM_ISP_REG_BASE + 0x0124)
#define PACK_SYNC_CFG	      			(DCAM_ISP_REG_BASE + 0x0128)

//trim&scale
#define SCALING_CFG					(DCAM_ISP_REG_BASE + 0x0130)
#define TRIM_START					(DCAM_ISP_REG_BASE + 0x0134)
#define TRIM_SIZE					(DCAM_ISP_REG_BASE + 0x0138)
#define SLICE_VER_SIZE				(DCAM_ISP_REG_BASE + 0x013C)			//SLICE_HEIGHT
#define DENOISE_CFG					(DCAM_ISP_REG_BASE + 0x0140)

#define VID_LUMA_HCOEFF01			(DCAM_ISP_REG_BASE + 0x0200)
#define VID_LUMA_HCOEFF02			(DCAM_ISP_REG_BASE + 0x0204)
#define VID_LUMA_HCOEFF03			(DCAM_ISP_REG_BASE + 0x0208)
#define VID_LUMA_HCOEFF04			(DCAM_ISP_REG_BASE + 0x020C)
#define VID_LUMA_HCOEFF11			(DCAM_ISP_REG_BASE + 0x0210)
#define VID_LUMA_HCOEFF12			(DCAM_ISP_REG_BASE + 0x0214)
#define VID_LUMA_HCOEFF13			(DCAM_ISP_REG_BASE + 0x0218)
#define VID_LUMA_HCOEFF14			(DCAM_ISP_REG_BASE + 0x021C)
#define VID_LUMA_HCOEFF21			(DCAM_ISP_REG_BASE + 0x0220)
#define VID_LUMA_HCOEFF22			(DCAM_ISP_REG_BASE + 0x0224)
#define VID_LUMA_HCOEFF23			(DCAM_ISP_REG_BASE + 0x0228)
#define VID_LUMA_HCOEFF24			(DCAM_ISP_REG_BASE + 0x022C)

#define VID_LUMA_HCOEFF31			(DCAM_ISP_REG_BASE + 0x0230)
#define VID_LUMA_HCOEFF32			(DCAM_ISP_REG_BASE + 0x0234)
#define VID_LUMA_HCOEFF33			(DCAM_ISP_REG_BASE + 0x0238)
#define VID_LUMA_HCOEFF34			(DCAM_ISP_REG_BASE + 0x023C)
#define VID_LUMA_HCOEFF41			(DCAM_ISP_REG_BASE + 0x0240)
#define VID_LUMA_HCOEFF42			(DCAM_ISP_REG_BASE + 0x0244)
#define VID_LUMA_HCOEFF43			(DCAM_ISP_REG_BASE + 0x0248)
#define VID_LUMA_HCOEFF44			(DCAM_ISP_REG_BASE + 0x024C)
#define VID_LUMA_HCOEFF51			(DCAM_ISP_REG_BASE + 0x0250)
#define VID_LUMA_HCOEFF52			(DCAM_ISP_REG_BASE + 0x0254)
#define VID_LUMA_HCOEFF53			(DCAM_ISP_REG_BASE + 0x0258)
#define VID_LUMA_HCOEFF54			(DCAM_ISP_REG_BASE + 0x025C)

#define VID_LUMA_HCOEFF61			(DCAM_ISP_REG_BASE + 0x0260)
#define VID_LUMA_HCOEFF62			(DCAM_ISP_REG_BASE + 0x0264)
#define VID_LUMA_HCOEFF63			(DCAM_ISP_REG_BASE + 0x0268)
#define VID_LUMA_HCOEFF64			(DCAM_ISP_REG_BASE + 0x026C)
#define VID_LUMA_HCOEFF71			(DCAM_ISP_REG_BASE + 0x0270)
#define VID_LUMA_HCOEFF72			(DCAM_ISP_REG_BASE + 0x0274)
#define VID_LUMA_HCOEFF73			(DCAM_ISP_REG_BASE + 0x0278)
#define VID_LUMA_HCOEFF74			(DCAM_ISP_REG_BASE + 0x027C)

#define VID_CHROMA_HCOEFF_LOW0		(DCAM_ISP_REG_BASE + 0x0280)
#define VID_CHROMA_HCOEFF_HIGH0		(DCAM_ISP_REG_BASE + 0x0284)
#define VID_CHROMA_HCOEFF_LOW1		(DCAM_ISP_REG_BASE + 0x0288)
#define VID_CHROMA_HCOEFF_HIGH1		(DCAM_ISP_REG_BASE + 0x028C)

#define VID_CHROMA_HCOEFF_LOW2		(DCAM_ISP_REG_BASE + 0x0290)
#define VID_CHROMA_HCOEFF_HIGH2		(DCAM_ISP_REG_BASE + 0x0294)
#define VID_CHROMA_HCOEFF_LOW3		(DCAM_ISP_REG_BASE + 0x0298)
#define VID_CHROMA_HCOEFF_HIGH3		(DCAM_ISP_REG_BASE + 0x029C)

#define VID_CHROMA_HCOEFF_LOW4		(DCAM_ISP_REG_BASE + 0x02A0)
#define VID_CHROMA_HCOEFF_HIGH4		(DCAM_ISP_REG_BASE + 0x02A4)
#define VID_CHROMA_HCOEFF_LOW5		(DCAM_ISP_REG_BASE + 0x02A8)
#define VID_CHROMA_HCOEFF_HIGH5		(DCAM_ISP_REG_BASE + 0x02AC)

#define VID_CHROMA_HCOEFF_LOW6		(DCAM_ISP_REG_BASE + 0x02B0)
#define VID_CHROMA_HCOEFF_HIGH6		(DCAM_ISP_REG_BASE + 0x02B4)
#define VID_CHROMA_HCOEFF_LOW7		(DCAM_ISP_REG_BASE + 0x02B8)
#define VID_CHROMA_HCOEFF_HIGH7		(DCAM_ISP_REG_BASE + 0x02BC)

#define VID_VCOEFF0					(DCAM_ISP_REG_BASE + 0x0300)
#define VID_VCOEFF1 				(DCAM_ISP_REG_BASE + 0x0304)
#define VID_VCOEFF2					(DCAM_ISP_REG_BASE + 0x0308)
#define VID_VCOEFF3 				(DCAM_ISP_REG_BASE + 0x030C)

#define VID_VCOEFF4					(DCAM_ISP_REG_BASE + 0x0310)
#define VID_VCOEFF5 				(DCAM_ISP_REG_BASE + 0x0314)
#define VID_VCOEFF6					(DCAM_ISP_REG_BASE + 0x0318)
#define VID_VCOEFF7 				(DCAM_ISP_REG_BASE + 0x031C)

#define VID_VCOEFF8					(DCAM_ISP_REG_BASE + 0x0320)
#define VID_VCOEFF9 				(DCAM_ISP_REG_BASE + 0x0324)
#define VID_VCOEFF10				(DCAM_ISP_REG_BASE + 0x0328)
#define VID_VCOEFF11 				(DCAM_ISP_REG_BASE + 0x032C)

#define VID_VCOEFF12				(DCAM_ISP_REG_BASE + 0x0330)
#define VID_VCOEFF13				(DCAM_ISP_REG_BASE + 0x0334)
#define VID_VCOEFF14				(DCAM_ISP_REG_BASE + 0x0338)
#define VID_VCOEFF15				(DCAM_ISP_REG_BASE + 0x033C)

#define VID_VCOEFF16				(DCAM_ISP_REG_BASE + 0x0340)
#define VID_VCOEFF17 				(DCAM_ISP_REG_BASE + 0x0344)
#define VID_VCOEFF18				(DCAM_ISP_REG_BASE + 0x0348)
#define VID_VCOEFF19 				(DCAM_ISP_REG_BASE + 0x034C)

#define VID_VCOEFF20				(DCAM_ISP_REG_BASE + 0x0350)
#define VID_VCOEFF21 				(DCAM_ISP_REG_BASE + 0x0354)
#define VID_VCOEFF22				(DCAM_ISP_REG_BASE + 0x0358)
#define VID_VCOEFF23 				(DCAM_ISP_REG_BASE + 0x035C)

#define VID_VCOEFF24				(DCAM_ISP_REG_BASE + 0x0360)
#define VID_VCOEFF25 				(DCAM_ISP_REG_BASE + 0x0364)
#define VID_VCOEFF26				(DCAM_ISP_REG_BASE + 0x0368)
#define VID_VCOEFF27 				(DCAM_ISP_REG_BASE + 0x036C)

#define VID_VCOEFF28				(DCAM_ISP_REG_BASE + 0x0370)
#define VID_VCOEFF29 				(DCAM_ISP_REG_BASE + 0x0374)
#define VID_VCOEFF30				(DCAM_ISP_REG_BASE + 0x0378)
#define VID_VCOEFF31 				(DCAM_ISP_REG_BASE + 0x037C)

#define VID_VCOEFF32				(DCAM_ISP_REG_BASE + 0x0380)
#define VID_VCOEFF33 				(DCAM_ISP_REG_BASE + 0x0384)
#define VID_VCOEFF34				(DCAM_ISP_REG_BASE + 0x0388)
#define VID_VCOEFF35 				(DCAM_ISP_REG_BASE + 0x038C)

#define VID_VCOEFF36				(DCAM_ISP_REG_BASE + 0x0390)
#define VID_VCOEFF37 				(DCAM_ISP_REG_BASE + 0x0394)
#define VID_VCOEFF38				(DCAM_ISP_REG_BASE + 0x0398)
#define VID_VCOEFF39 				(DCAM_ISP_REG_BASE + 0x039C)

#define VID_VCOEFF40				(DCAM_ISP_REG_BASE + 0x03A0)
#define VID_VCOEFF41 				(DCAM_ISP_REG_BASE + 0x03A4)
#define VID_VCOEFF42				(DCAM_ISP_REG_BASE + 0x03A8)
#define VID_VCOEFF43 				(DCAM_ISP_REG_BASE + 0x03AC)

#define VID_VCOEFF44				(DCAM_ISP_REG_BASE + 0x03B0)
#define VID_VCOEFF45 				(DCAM_ISP_REG_BASE + 0x03B4)
#define VID_VCOEFF46				(DCAM_ISP_REG_BASE + 0x03B8)
#define VID_VCOEFF47 				(DCAM_ISP_REG_BASE + 0x03BC)

#define VID_VCOEFF48				(DCAM_ISP_REG_BASE + 0x03C0)
#define VID_VCOEFF49				(DCAM_ISP_REG_BASE + 0x03C4)
#define VID_VCOEFF50				(DCAM_ISP_REG_BASE + 0x03C8)
#define VID_VCOEFF51 				(DCAM_ISP_REG_BASE + 0x03CC)

#define VID_VCOEFF52				(DCAM_ISP_REG_BASE + 0x03D0)
#define VID_VCOEFF53 				(DCAM_ISP_REG_BASE + 0x03D4)
#define VID_VCOEFF54				(DCAM_ISP_REG_BASE + 0x03D8)
#define VID_VCOEFF55 				(DCAM_ISP_REG_BASE + 0x03DC)

#define VID_VCOEFF56				(DCAM_ISP_REG_BASE + 0x03E0)
#define VID_VCOEFF57 				(DCAM_ISP_REG_BASE + 0x03E4)
#define VID_VCOEFF58				(DCAM_ISP_REG_BASE + 0x03E8)
#define VID_VCOEFF59 				(DCAM_ISP_REG_BASE + 0x03EC)

#define VID_VCOEFF60				(DCAM_ISP_REG_BASE + 0x03F0)
#define VID_VCOEFF61				(DCAM_ISP_REG_BASE + 0x03F4)
#define VID_VCOEFF62				(DCAM_ISP_REG_BASE + 0x03F8)
#define VID_VCOEFF63 				(DCAM_ISP_REG_BASE + 0x03FC)

#define VID_VCOEFF64				(DCAM_ISP_REG_BASE + 0x0400)
#define VID_VCOEFF65				(DCAM_ISP_REG_BASE + 0x0404)
#define VID_VCOEFF66				(DCAM_ISP_REG_BASE + 0x0408)
#define VID_VCOEFF67 				(DCAM_ISP_REG_BASE + 0x040C)

//Dcam int mask bits
#define DCAM_INT_MASK_VALUE         0x00007FFF

#define DCAM_ISP_MODE					0x0
#define DCAM_VSP_MODE					0x1
#define SCALE_COEFF_NUM					84
#define ISP_HCLK_DOMAIN					1
#define ISP_CLK_DCAM_DOMAIN				0
#define ISP_JPEG_MEM_SIZE_1             1//512K BYTE
#define ISP_JPEG_MEM_SIZE_2             2//640K BYTE
#define ISP_JPEG_MEM_SIZE_3             3//768K
#define ISP_JPEG_MEM_SIZE_4             4//1024 BYTE
#define ISP_JPEG_MEM_SIZE_5             5//1.25M
#define ISP_JPEG_MEM_SIZE_6             6//1.5M
#define ISP_JPEG_MEM_SIZE_7             7//2M BYTE


/**---------------------------------------------------------------------------*
**                               Data Prototype                              **
**----------------------------------------------------------------------------*/
//top module reg define being
#ifdef CHIP_ENDIAN_LITTLE

    typedef union _dcam_cfg_tag {
    	struct _dcam_cfg_map {
    		volatile unsigned int dcam_mode			:1; //0:ISP mode;1:VSP mode
    		volatile unsigned int isp_mode			:1; //0:capture;1:review
    		volatile unsigned int cap_mode			:1; //0:cap single; 1: capture multi
    		volatile unsigned int vsp_eb  			:1; //0:dcam clock domain;1: enbale VSP module
    		volatile unsigned int clk_switch        :1; //0:clk_dcam_domain;1:HCLK
    		volatile unsigned int cap_eb    		:1; //0:disable CAP module;1:enable CAP module
    		volatile unsigned int dummy	:           1;	
    		volatile unsigned int clk_status    	:1;	//0:enable clock decam domain; 1:HCLK domain
    		volatile unsigned int reserved       	:24;//Reserved ;
    	}mBits ;
    	volatile unsigned int dwValue ;
    }DCAM_CFG_U;


typedef union _cap_shadow_cntl_tag {
	struct _cap_shadow_cntl_map {
		volatile unsigned int frc_copy_cap	:1;
		volatile unsigned int auto_copy_cap :1;
		volatile unsigned int reserved		:30;
	}mBits;
	volatile unsigned int dwValue;
}CAP_SHADOW_CNTL_U;


typedef union _sc_format_cfg_tag {
	struct _sc_format_cfg_map {
		volatile unsigned int sc_out_frmt           :2;     //0: YUV422; 1: YUV420; 10: RGB565; 11: RGB888;
		volatile unsigned int dither_eb               :1;
		volatile unsigned int rgb_ext                    :1;    //rgb565 -> rgb888
		volatile unsigned int sc_in_frmt	           :3;      //0: YUV422; 1: YUV420;10: YUV400; 11: YUV420(3 frm); 100: RGB888; 111 RGB565
		volatile unsigned int rot_eb                    :1;    //only work in YUV
        	volatile unsigned int rot_mode                  :2;    //00: mirror; 01: 180; 10: flip; 11: service
		volatile unsigned int reserved                  :22;    
	}mBits;
	volatile unsigned int dwValue;
}SC_FORMAT_CFG_U;


typedef union _dcam_src_size_tag {
	struct _dcam_src_size_map {
		volatile unsigned int src_size_x	    :12;
		volatile unsigned int reserved0		:4;
		volatile unsigned int src_size_y	    :12;
		volatile unsigned int reserved1		:4;
	}mBits;
	volatile unsigned int dwValue;
}DCAM_SRC_SIZE_U;


typedef union _isp_dis_size_tag {
	struct _isp_dis_size_map {
		volatile unsigned int dis_size_x    :12;
		volatile unsigned int reserved0		:4;
		volatile unsigned int dis_size_y	:12;
		volatile unsigned int reserved1		:4;
	}mBits;
	volatile unsigned int dwValue;
}ISP_DIS_SIZE_U;

typedef union _vsp_timeout_tag{
	struct _vsp_timeout_map {
		volatile unsigned int timeout_val    :16;
		volatile unsigned int timeout_eb		:1;
		volatile unsigned int timeout_clr	    :1;
		volatile unsigned int reserved1		:14;
	}mBits;
	volatile unsigned int dwValue;
}VSP_TIMEOUT_U;

typedef union _dcam_int_sts_tag {
	struct _dcam_int_sts_map {
		volatile unsigned int sensor_sof	        :1;
		volatile unsigned int sensor_eof         :1;
		volatile unsigned int cap_sof		 :1;
		volatile unsigned int cap_eof		 :1;
		volatile unsigned int isp_tx_done	 :1;
		volatile unsigned int cap_line_err		 :1;
		volatile unsigned int cap_buf_ovf	 :1;
		volatile unsigned int vsp_bsm_done   :1;
		volatile unsigned int vsp_vlc_done	 :1;
		volatile unsigned int vsp_mbio_done	 :1;
		volatile unsigned int cap_frm_err         :1;
		volatile unsigned int jpg_buf_ovf	 :1;
		volatile unsigned int vsp_time_out     :1;
		volatile unsigned int vsp_vld_err	 :1;
		volatile unsigned int vsp_cmd_done            :1;
		volatile unsigned int vsp_cmd_timeout         :1;
		volatile unsigned int reserved		 :16;
	}mBits;
	volatile unsigned int dwValue;
}DCAM_INT_STS_U;

typedef union _dcam_int_maks_tag {
	struct _dcam_int_mask_map {
		volatile unsigned int sensor_sof	        :1;
		volatile unsigned int sensor_eof         :1;
		volatile unsigned int cap_sof		 :1;
		volatile unsigned int cap_eof		 :1;
		volatile unsigned int isp_tx_done	 :1;
		volatile unsigned int cap_line_err		 :1;
		volatile unsigned int cap_buf_ovf	 :1;
		volatile unsigned int vsp_bsm_done   :1;
		volatile unsigned int vsp_vlc_done	 :1;
		volatile unsigned int vsp_mbio_done	 :1;
		volatile unsigned int cap_frm_err         :1;
		volatile unsigned int jpg_buf_ovf	 :1;
		volatile unsigned int vsp_time_out     :1;
		volatile unsigned int vsp_vld_err	 :1;
		volatile unsigned int vsp_cmd_done            :1;
		volatile unsigned int vsp_cmd_timeout         :1;
		volatile unsigned int reserved		 :16;
	}mBits;
	volatile unsigned int dwValue;
}DCAM_INT_MASK_U;

typedef union _dcam_int_clr_tag {
	struct _dcam_int_clr_map {
		volatile unsigned int sensor_sof	        :1;
		volatile unsigned int sensor_eof         :1;
		volatile unsigned int cap_sof		 :1;
		volatile unsigned int cap_eof		 :1;
		volatile unsigned int isp_tx_done	 :1;
		volatile unsigned int cap_line_err		 :1;
		volatile unsigned int cap_buf_ovf	 :1;
		volatile unsigned int vsp_bsm_done   :1;
		volatile unsigned int vsp_vlc_done	 :1;
		volatile unsigned int vsp_mbio_done	 :1;
		volatile unsigned int cap_frm_err         :1;
		volatile unsigned int jpg_buf_ovf	 :1;
		volatile unsigned int vsp_time_out     :1;
		volatile unsigned int vsp_vld_err	 :1;
		volatile unsigned int vsp_cmd_done            :1;
		volatile unsigned int vsp_cmd_timeout         :1;
		volatile unsigned int reserved		 :16;
	}mBits;
	volatile unsigned int dwValue;
}DCAM_INT_CLR_U;

typedef union _dcam_int_raw_tag {
	struct _dcam_int_raw_map {
		volatile unsigned int sensor_sof	        :1;
		volatile unsigned int sensor_eof         :1;
		volatile unsigned int cap_sof		 :1;
		volatile unsigned int cap_eof		 :1;
		volatile unsigned int isp_tx_done	 :1;
		volatile unsigned int cap_line_err		 :1;
		volatile unsigned int cap_buf_ovf	 :1;
		volatile unsigned int vsp_bsm_done   :1;
		volatile unsigned int vsp_vlc_done	 :1;
		volatile unsigned int vsp_mbio_done	 :1;
		volatile unsigned int cap_frm_err         :1;
		volatile unsigned int jpg_buf_ovf	 :1;
		volatile unsigned int vsp_time_out     :1;
		volatile unsigned int vsp_vld_err	 :1;
		volatile unsigned int vsp_cmd_done            :1;
		volatile unsigned int vsp_cmd_timeout         :1;
		volatile unsigned int reserved		 :16;
	}mBits;
	volatile unsigned int dwValue;
}DCAM_INT_RAW_U;

//top module reg define end
//ahb master reg define being
typedef union _dcam_frm_addr0_tag {
	struct _dcam_frm_addr0_map {
		volatile unsigned int frm_addr_0    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_0_U;

typedef union _dcam_frm_addr1_tag {
	struct _dcam_frm_addr1_map {
		volatile unsigned int frm_addr_1    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_1_U;

typedef union _dcam_frm_addr2_tag {
	struct _dcam_frm_addr2_map {
		volatile unsigned int frm_addr_2    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_2_U;

typedef union _dcam_frm_addr3_tag {
	struct _dcam_frm_addr3_map {
		volatile unsigned int frm_addr_3    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_3_U;

typedef union _dcam_frm_addr4_tag {
	struct _dcam_frm_addr4_map {
		volatile unsigned int frm_addr_4    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_4_U;

typedef union _dcam_frm_addr5_tag {
	struct _dcam_frm_addr5_map {
		volatile unsigned int frm_addr_5    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_5_U;

typedef union _dcam_frm_addr6_tag {
	struct _dcam_frm_addr6_map {
		volatile unsigned int frm_addr_6    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_6_U;

typedef union _dcam_burst_cap_tag {
	struct _dcam_burst_cap_map {
		volatile unsigned int burst_gap     :5;
		volatile unsigned int reserved       :27;
	}mBits;
	volatile unsigned int dwValue;
}BURST_GAP_U;

typedef union _dcam_endian_sel_tag {
	struct dcam_endian_sel_map {
		volatile unsigned int endian_sel_rd     :2;     //00: 0,1,2,3; 01: 3,2,1,0; 10: 2,3,0,1
		volatile unsigned int endian_sel_wr     :2;      //00: 0,1,2,3; 01: 3,2,1,0; 10: 2,3,0,1
		volatile unsigned int reserved       :28;
	}mBits;
	volatile unsigned int dwValue;
}ENDIAN_SEL_U;


typedef union _dcam_ahbm_sts_tag {
	struct _dcam_ahbm_sts_map {
		volatile unsigned int ahbm_busy     :1;
		volatile unsigned int reserved       :30;
	}mBits;
	volatile unsigned int dwValue;
}AHBM_STS_U;

typedef union _dcam_frm_addr7_tag {
	struct _dcam_frm_addr7_map {
		volatile unsigned int frm_addr_7    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_7_U;

typedef union _dcam_frm_addr8_tag {
	struct _dcam_frm_addr8_map {
		volatile unsigned int frm_addr_8    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_8_U;

typedef union _dcam_frm_addr9_tag {
	struct _dcam_frm_addr9_map {
		volatile unsigned int frm_addr_8    :32;
	}mBits;
	volatile unsigned int dwValue;
}FRM_ADDR_9_U;

typedef union _cmd_dgb0_tag {
	struct _cmd_dgb0_map {
		volatile unsigned int data_vdb_saddr     :20;
		volatile unsigned int exec_state       :3;
    		volatile unsigned int  fetch_state     :2;
		volatile unsigned int dbuf_end_flag       :1;
    		volatile unsigned int cbuf_end_flag     :1;
		volatile unsigned int rfifo_cmd_emp       :1;
    		volatile unsigned int cmd_vdb_req     :1;
		volatile unsigned int cmd_vdb_sel       :1;
    		volatile unsigned int cmd_bus_wr     :1;
		volatile unsigned int cmd_bus_req       :1;
	}mBits;
	volatile unsigned int dwValue;
}CMD_DGB0_U;

typedef union _cmd_dgb1_tag {
	struct _cmd_dgb1_map {
		volatile unsigned int info_vdb_saddr     :20;
		volatile unsigned int cmd_vbus_addr       :12;
	}mBits;
	volatile unsigned int dwValue;
}CMD_DGB1_U;

//ahb master reg define end

//cap reg define being
typedef union _cap_ctrl_tag {
	struct _cap_ctrl_map {
		volatile unsigned int ccir_656		 :1;
		volatile unsigned int sensor_mode	 :2;
		volatile unsigned int hsync_pol		 :1;
		volatile unsigned int vsync_pol		 :1;
		volatile unsigned int cap_if_endian   :1;
		volatile unsigned int reserved0		 :1;
		volatile unsigned int yuv_type		 :2;        //0: big endian; 1: little endian
		volatile unsigned int cap_if_mode     :2;
		volatile unsigned int cap_ccir_rst   :1;
		volatile unsigned int cap_ccir_pd	 :1;
		volatile unsigned int reserved1		 :3;
		volatile unsigned int pack_mode	 :1;        //0: 8; 1: 4; 10: 2; 11: 1;
		volatile unsigned int reserved2		 :15;
	}mBits;
	volatile unsigned int dwValue;
}CAP_CTRL_U;

typedef union _cap_frm_cnt_tag {
	struct _cap_frm_cnt_map {
		volatile unsigned int pre_skip_cnt	:4;
		volatile unsigned int cap_frm_deci	:2;
		volatile unsigned int reserved0		:10;
		volatile unsigned int cap_frm_cnt	:6;
		volatile unsigned int cap_frm_clr	:1;
		volatile unsigned int reserved2		:9;
	}mBits;
	volatile unsigned int dwValue;
}CAP_FRM_CNT_U;

typedef union _cap_start_tag {
	struct _cap_start_map {
		volatile unsigned int start_x		:12;
		volatile unsigned int reserved0		:4;
		volatile unsigned int start_y       :12;
		volatile unsigned int reserved1		:4;
	}mBits;
	volatile unsigned int dwValue;
}CAP_START_U;

typedef union _cap_end_tag {
	struct _cap_end_map {
		volatile unsigned int end_x			:12;
		volatile unsigned int reserved0		:4;
		volatile unsigned int end_y			:12;
		volatile unsigned int reserved1     :4;
	}mBits;
	volatile unsigned int dwValue;
}CAP_END_U;

typedef union _cap_image_deci_tag {
	struct _cap_image_deci_map {
		volatile unsigned int cap_deci_y	:2;
		volatile unsigned int cap_deci_x	:2;
		volatile unsigned int reserved		:28;
	}mBits;
	volatile unsigned int dwValue;
}CAP_IMAGE_DECI_U;

typedef union _atv_fix_mode_tag {
	struct _atv_fix_mode_map {
		volatile unsigned int threshold_eb	:1;
		volatile unsigned int threshold_start :12;
		volatile unsigned int threshold_end	:12;
		volatile unsigned int reserved		:7;     
	}mBits;
	volatile unsigned int dwValue;
}ATV_FIX_MODE_U;

typedef union _package_img_inf_tag {
	struct _package_img_inf_map {
		volatile unsigned int pack_img_height	:16;
		volatile unsigned int pack_img_width		:16;
	}mBits;
	volatile unsigned int dwValue;
}PACKAGE_IMG_INF_U;

typedef union _cap_jpeg_ctrl_tag {
	struct _cap_jpeg_ctrl_map {
		volatile unsigned int jpeg_mem_size :10;    //n*32k
		volatile unsigned int reserved1		:22;
	}mBits;
	volatile unsigned int dwValue;
}CAP_JPEG_CTRL_U;

typedef union _cap_frm_size_tag {
	struct _cap_frm_size_map {
		volatile unsigned int cap_frm_size :24;    
		volatile unsigned int reserved1		:8;
	}mBits;
	volatile unsigned int dwValue;
}CAP_FRM_SIZE_U;

typedef union _cap_spi_cfg_tag {
	struct _cap_spi_cfg_map {
		volatile unsigned int spi_org_width :13;    
		volatile unsigned int reserved1		:19;
	}mBits;
	volatile unsigned int dwValue;
}CAP_SPI_CFG_U;

typedef union _pack_sync_cfg_tag {
	struct _pack_sync_cfg_map {
		volatile unsigned int package_sof_id        :8;   
		volatile unsigned int package_eof_id		:8;
		volatile unsigned int package_sol_id        :8;   
		volatile unsigned int package_sod_id		:8;
	}mBits;
	volatile unsigned int dwValue;
}PACK_SYNC_CFG_U;



//**triming & scaling control register**///
//trim&scale
typedef union _scaling_cfg_tag {
	struct _scaling_cfg_map {
		volatile unsigned int review_start  :1;
		volatile unsigned int scale_bypass  :1;
		volatile unsigned int slice_mode	:1;
		volatile unsigned int trim_eb		:1;
		volatile unsigned int subsample_eb		:1;
		volatile unsigned int subsample_mode	:2;     //0: 1/2; 01: 1/4; 10: 1/8; 11: 1/16;
		volatile unsigned int ver_down_tap  :4;
		volatile unsigned int reserved		:21;
	}mBits;
	volatile unsigned int dwValue;
}SCALING_CFG_U;

typedef union _trim_start_tag {
	struct _trim_start_map {
		volatile unsigned int start_x       :12;
		volatile unsigned int reserved0		:4;
		volatile unsigned int start_y		:12;
		volatile unsigned int reserved1		:4;
	}mBits;
	volatile unsigned int dwValue;
}TRIM_START_U;

typedef union _trim_size_tag {
	struct _trim_size_map {
		volatile unsigned int size_x		:12;
		volatile unsigned int reserved0		:4;
		volatile unsigned int size_y 		:12;
		volatile unsigned int reserved1		:4;
	}mBits;
	volatile unsigned int dwValue;
}TRIM_SIZE_U;

typedef union _slice_ver_size_tag {
	struct _slice_ver_size_map {
		volatile unsigned int slice_line_input   	:12;
              volatile unsigned int last_slice				:1;
		volatile unsigned int reserved0				:3;
		volatile unsigned int slice_line_output   	:12;
		volatile unsigned int reserved1				:4;
	}mBits;
	volatile unsigned int dwValue;
}SLICE_VER_SIZE_U;

typedef union _denoise_cfg_tag {
	struct _denoise_cfg_map {
		volatile unsigned int de_threshold_y   	:8;
              volatile unsigned int de_threshold_uv	:8;
		volatile unsigned int denoise_eb			:1;
		volatile unsigned int reserved1			:15;
	}mBits;
	volatile unsigned int dwValue;
}DENOISE_CFG_U;
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

