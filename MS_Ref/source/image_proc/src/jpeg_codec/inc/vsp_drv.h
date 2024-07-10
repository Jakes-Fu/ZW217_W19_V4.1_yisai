#ifndef _VSP_DRV_H_
#define _VSP_DRV_H_

/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "arm_reg.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Macro Definitions                               *
**---------------------------------------------------------------------------*/
/* Global Register About VSP */
#define	AHB_GENERAL_CONTROL_REG_BASE	0x20900200

#define AHB_CTL0   		(AHB_GENERAL_CONTROL_REG_BASE + 0x0000)
#define AHB_CTL1		(AHB_GENERAL_CONTROL_REG_BASE + 0x0004)
#define AHB_CTL2		(AHB_GENERAL_CONTROL_REG_BASE + 0x0008)
#define AHB_SOFT_RST 	(AHB_GENERAL_CONTROL_REG_BASE + 0x0010)

/* Bit Position Define About VSP */
#define	MEA_EB_BIT	13
#define	DCT_EB_BIT	12

/* 0 is mean switch to MEA, 1 is mean switch to ARM */
#define	MEA_RBUF_SW_BIT	10

#define	GLB_MEA_ENDIAN_BIT	1

#define MEA_RST_BIT 15
#define DCT_RST_BIT 14

//#define	VSP_LITTLE_ENDIAN 0
//#define	VSP_BIG_ENDIAN	1


/* VSP_BUF for MEA */
#define	VSP_ADDRESS_BASE	0x20c00000

#define U_RAM16K_BASE	0x010000 + VSP_ADDRESS_BASE
#define	U0_RAM1K_BASE	0x001000 + VSP_ADDRESS_BASE
#define	U1_RAM1K_BASE	0x002000 + VSP_ADDRESS_BASE
#define	U2_RAM1K_BASE	0x003000 + VSP_ADDRESS_BASE

#define	MEA_REF_BFR_BASE	U_RAM16K_BASE
#define	MEA_SRC_BFR_BASE	U0_RAM1K_BASE
#define MEA_VIR_BFR_BASE    U1_RAM1K_BASE
#define	MCA_OUTPUT_BFR_BASE	U2_RAM1K_BASE

#define	MEA_CMD_BFR_BASE	0x20c00800

#define	MEA_CMD_BFR_SIZE	(64 * 4)
#define MEA_REF_BFR_SIZE	(4096 * 4)
#define	MEA_SRC_BFR_SIZE	(128 * 4)	
#define	MEA_OUT_BFR_SIZE	(128 * 4) 

/* VSP_BUF for DCT */
#define	DCT_IO_BFR_BASE		0x20400800
#define DCT_IO_BFR_SIZE		(192 * 4)


/* MEA register map */
#define	MEA_CTRL_BASE		0x20c00100

#define MEA_FM_CFG			(MEA_CTRL_BASE + 0x0000)
#define MEA_RBUF_CFG		(MEA_CTRL_BASE + 0x0004)
#define MEA_CBUF_CFG		(MEA_CTRL_BASE + 0x0008)
#define MEA_OP_CFG			(MEA_CTRL_BASE + 0x000C)
#define MEA_HP_SCAN_CFG		(MEA_CTRL_BASE + 0x0010)
#define MEA_SMB_CFG			(MEA_CTRL_BASE + 0x0014)
#define MEA_RESULT_MV		(MEA_CTRL_BASE + 0x0018)
#define MEA_RESULT_MV0		(MEA_CTRL_BASE + 0x001C)
#define MEA_RESULT_MV1		(MEA_CTRL_BASE + 0x0020)
#define MEA_RESULT_MV2		(MEA_CTRL_BASE + 0x0024)
#define MEA_RESULT_MV3		(MEA_CTRL_BASE + 0x0028)
#define MEA_OPERATE_CTRL    (MEA_CTRL_BASE + 0x002C)
#define MEA_CORE_CTRL		(MEA_CTRL_BASE + 0x0030)
#define MEA_INT_RAW			(MEA_CTRL_BASE + 0x0034)
#define MEA_INT_ENB			(MEA_CTRL_BASE + 0x0038)
#define MEA_INT_STATE		(MEA_CTRL_BASE + 0x003C)
#define MEA_INT_CLR			(MEA_CTRL_BASE + 0x0040)


/* MEA_FM_CFG register BIT Define */
#define MEA_Y_MAX_BIT		8
#define MEA_X_MAX_BIT		0

/* MEA_RBUF_CFG register BIT Define */
#define MEA_RBUF_CTRL_BIT		24
#define MEA_RBUF_MODE_BIT		16
#define MEA_RBUF_UV_BIT 		8
#define MEA_Y_MV_MAX_BIT		0

/* MEA_CBUF_CFG register BIT Define */
#define MEA_STP_VAL_BIT 		16
#define MEA_SB_CMD_NUM_BIT		8
#define MEA_MB_CMD_NUM_BIT		0

/* MEA_OP_CFG register BIT Define */
#define MEA_BLK_MODE_BIT 		8
#define MEA_YUV_TYPE_BIT		0

/* MEA_HPSCAN_CFG register BIT Define */
#define MEA_HP_SCAN_EN_BIT 		16
#define MEA_HP_CAL_BIT			8
#define MEA_HP_CMD_NUM_BIT		0

/* MEA_SMB_CFG register BIT Define */
#define MEA_Y_ID_BIT			8
#define MEA_X_ID_BIT			0

/* MEA_SAD_MV register BIT Define */
#define MEA_SAD_BIT				16
#define MEA_Y_MV_BIT			8
#define MEA_X_MV_BIT			0

/* MEA_OPERATE_CTRL register BIT Define */
#define MEA_WORK_MODE_BIT		8
#define MEA_BUSY_BIT			1
#define MEA_START_BIT			0

/* MEA_CORE_CTRL register BIT Define */
#define MEA_MODE_BIT			16
#define MEA_DATA_INV_BIT 		8

/* RBUF control under 2048 mode */
//#define	Y_REF_DATA			0x4
//#define	U_REF_DATA			0x2
//#define	V_REF_DATA			0x1


/* RBUF work mode */
//#define	MEA_REF_BFR_MODE_4096	0
//#define	MEA_REF_BFR_MODE_2048	1


/* MEA Blcok Mode */
//#define MODE_16_16		0
//#define MODE_8_8		1


/* 	YUV Type Selection */
//#define OUT_Y_DATA		4
//#define OUT_U_DATA 		2
//#define OUT_V_DATA		1


/* MEA Work Mode */
#define MEA_SAD_MODE		0
#define MEA_OUT_MODE		1

#define MEA_START			1

/* MEA Mode */
//#define MEA_INTER_MODE 		0
//#define MEA_INTRA_MODE		1


/* MEA DATA inverse */					
#define	MEA_DATA_INVER		1
#define	MEA_DATA_NOT_INVER	0



/* DCT register map */
#define	DCT_CTRL_BASE		0x20400000

#define DCT_CONFIG			(DCT_CTRL_BASE + 0x0000)
#define DCT_QUAN_PARA		(DCT_CTRL_BASE + 0x0004)
#define	Y_QUAN_PARA			(DCT_CTRL_BASE + 0x0008)
#define UV_QUAN_PARA		(DCT_CTRL_BASE + 0x000C)
#define DCT_OUTPUT_STATE	(DCT_CTRL_BASE + 0x0014)
#define DCT_START			(DCT_CTRL_BASE + 0x0018)
#define DCT_INT				(DCT_CTRL_BASE + 0x001C)
#define DCT_INT_MASK		(DCT_CTRL_BASE + 0x0020)
#define DCT_INT_RAW			(DCT_CTRL_BASE + 0x0024)
#define DCT_INT_CLR			(DCT_CTRL_BASE + 0x0028)	


/* DCT_CONFIG register BIT Define */
#define DCT_WORK_MODE_BIT			0
#define DCT_SCAN_MODE_BIT			1
#define	DCT_PIXEL_8BIT_EN_BIT		3
#define	DCT_PIXEL_8BIT_PACKET_BIT	4
#define	DCT_SIGN_EN_BIT				5
#define DCT_READINVERSE_BIT         6
#define	DCT_BLOCK_MASK_BIT			16
#define	DCT_IN_BUF_ARRANGE_BIT		24
#define	DCT_PROTECT_EN_BIT			25
#define	DCT_IN_ENDIAN_BIT			26
#define	DCT_OUT_ENDIAN_BIT			27

/* DCT_QUAN_PARA register BIT Define */
#define	DCT_QUAN_EN_BIT				0
#define	DCT_QUAN_MODE_BIT			1
#define	DCT_QUAN_SHIFT_BIT			2
#define	DCT_QAUN_VALUE_BIT			8
#define	DCT_QUAN_INV_SHIFT_BIT		16

/* Y_QUAN_PARA register BIT Define */
#define	DCT_Y_QUAN_SHIFT_BIT		2
#define	DCT_Y_QUAN_INV_SHIFT_BIT	16

/* UV_QUAN_PARA register BIT Define */
#define	DCT_UV_QUAN_SHIFT_BIT		2
#define	DCT_UV_QUAN_INV_SHIFT_BIT	16

/* DCT_OUTPUT_STATE register BIT Define */
#define	DCT_IDLE_BIT			0
#define	DCT_CBP_STATE_BIT		8
		
/* DCT_OUTPUT_STATE register BIT Define */
#define DCT_START_BIT   0		

/* DCT mode */
#define	VSP_DCT					0
#define	VSP_IDCT				1
					
/* Scan Mode */
//#define	ZIGZAG_SCAN				0
//#define	VERTICAL_SCAN			1
//#define	HORIZONTAL_SCAN			2
//#define	BYPASS_MODE				3

	
/* Pixel 8bit En */	
#define	PIXEL_8BIT				1
#define	PIXEL_9BIT				0
				
/* Pixel 8bit packet */
#define PIXEL_BYTE_SAVE         1 
#define PIXEL_WORD_SAVE 		0

/* source be sign or unsigned for 8 bit input */
#define	SIGN_PIXEL_8BIT			1
#define	UNSIGN_PIXEL_8BIT		0

/* read inverse */
#define	READ_INVERSE			1
#define	NOT_INVERSE				0

/* In_BUF_Arrange */
//#define INPUT_BUF_MB  			1
//#define INPUT_BUF_BLOCK			0


/* Protect Enable */
#define	DCT_PRCT_ENABLE			1
#define	DCT_PRCT_DISABLE		0

/* I/O Buffer Endian Setting */
//#define	DCT_BIG_ENDIAN			0
//#define	DCT_LITTLE_ENDIAN		1

/* Quan Enable */
#define	QUANT_EN				1
#define	QUANT_DIS				0

/* Quan Mode */
#define	INTER_DCT				0
#define	INTRA_DCT				1

#define	INTRA_QUANT				1
#define	INTER_QUANT				0

#define MB_SIZE					16
#define BLOCK_SIZE				8

/* Motion Estimation Method */
#define _THREE_STEP_SEARCH_
//#define _NEW_THREE_STEP_SEARCH_
//#define _PRED_CROSS_SEARCH_

#define MEA_STOP_VAL  			600
#define MEA_MB_CMD_NUM_TSM		33
#define MEA_MB_CMD_NUM_PCMS		34
#define MEA_SB_CMD_NUM			0
#define MEA_HP_CMD_NUM			8


/*-----------------------------------------------------------------------------
 Register Operate Interfaces MACRO Define
-----------------------------------------------------------------------------*/
#define VSP_REG_OR(reg_addr, value)    *(volatile uint32 *)(reg_addr) |= (value)
#define VSP_REG_AND(reg_addr, value)   *(volatile uint32 *)(reg_addr) &= (value)
#define VSP_REG_SET(reg_addr, value)   *(volatile uint32 *)(reg_addr)  = (value)
#define VSP_REG_GET(reg_addr)		   (*(volatile uint32 *)(reg_addr))


/* VSP---DCT Config Interface MACRO Define */
#define DISABLE_DCT_CLOCK   VSP_REG_AND(AHB_CTL0,~(1 << DCT_EB_BIT));
#define ENABLE_DCT_CLOCK 	VSP_REG_OR(AHB_CTL0,1 << DCT_EB_BIT);
/****************************************************
	Start DCT Engine
	1. Enable DCT Clock
	2. Start DCT Engine
*****************************************************/
#define START_DCT_ENGINE\
	{\
	ENABLE_DCT_CLOCK\
	VSP_REG_SET(DCT_START,0);\
	VSP_REG_SET(DCT_START,1);\
	}
#define CLOSE_DCT_ENGINE\
	{\
	VSP_REG_SET(DCT_INT_CLR,1);\
	}

#define START_DCT_ENGINE_OUT\
	{\
	ENABLE_DCT_CLOCK\
	}

#define CLOSE_DCT_ENGINE_OUT\
	{\
	DISABLE_DCT_CLOCK\
	}

#define VSP_WAIT_DCT_DONE\
	{\
	while(VSP_REG_GET(DCT_INT_RAW) != 1);\
	VSP_REG_SET(DCT_INT_CLR,1);\
	}

#define VSP_WAIT_DCT_OUTBUF_READY\
	{\
	while(((VSP_REG_GET(DCT_OUTPUT_STATE)) & BIT_0) != 1);\
	}


/* VSP---MEA Config Interface MACRO Define */
#define DISABLE_MEA_CLOCK 	VSP_REG_AND(AHB_CTL0,~(1 << MEA_EB_BIT));
#define ENABLE_MEA_CLOCK 	VSP_REG_OR(AHB_CTL0,1 << MEA_EB_BIT);

/* BUF Switch Between HW and SW */
/* SRCBUF need to be used because for 4096 mode */
#define SRC_OUT_REF_BUF_HW  		0x7
#define OUT_CMD_SRC_REF_BUF_HW  	0xB
#define ALL_BUF_FOR_SW				0xfffffff0	

#define MEA_START_OUT\
	{\
	VSP_REG_AND(MEA_CORE_CTRL,ALL_BUF_FOR_SW);\
	VSP_REG_OR(MEA_CORE_CTRL,SRC_OUT_REF_BUF_HW);\
	VSP_REG_SET(MEA_OPERATE_CTRL,(MEA_OUT_MODE << MEA_WORK_MODE_BIT) | MEA_START);\
	}

#define CLOSE_MEA_MC\
	{\
	VSP_REG_AND(MEA_CORE_CTRL,ALL_BUF_FOR_SW);\
	}	


#define START_MEA_ENGINE\
	{\
	VSP_REG_AND(MEA_CORE_CTRL,ALL_BUF_FOR_SW);\
	VSP_REG_OR(MEA_CORE_CTRL,OUT_CMD_SRC_REF_BUF_HW);\
	VSP_REG_SET(MEA_RESULT_MV,0xffff0000);\
	VSP_REG_AND(MEA_OP_CFG,0xfffffff8);\
	VSP_REG_OR(MEA_OP_CFG,MEA_OUT_Y_DATA);\
	VSP_REG_SET(MEA_OPERATE_CTRL,MEA_START);\
	}

#define CLOSE_MEA_ENGINE\
	{\
	VSP_REG_AND(MEA_CORE_CTRL,ALL_BUF_FOR_SW);\
	}

#define VSP_WAIT_MEA_DONE\
	{\
	while(VSP_REG_GET(MEA_INT_RAW) != 1);\
	VSP_REG_SET(MEA_INT_CLR,1);\
	}

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                  *
 **---------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                */
/*--------------------------------------------------------------------------*/
typedef enum
{
	VSP_LITTLE_ENDIAN = 0,
	VSP_BIG_ENDIAN
}VSP_ENDIAN_E;

typedef enum
{
	MEA_Y_REF_DATA = 0x4,
	MEA_U_REF_DATA = 0x2,
	MEA_V_REF_DATA = 0x1
}MEA_RBUF_DATYPE_E;

typedef enum
{
	MEA_REF_BFR_MODE_4096 = 0,
	MEA_REF_BFR_MODE_2048
}VSP_MEARBUF_MODE_E;

typedef enum
{
	MEA_MODE_16_16 = 0,
	MEA_MODE_8_8
}MEA_BLOCK_MODE_E;

typedef enum
{
	MEA_OUT_Y_DATA = 4,
	MEA_OUT_U_DATA = 2,
	MEA_OUT_V_DATA = 1
}MEA_OUT_TYPE_E;

typedef enum
{
	MEA_INTER_MODE = 0,
	MEA_INTRA_MODE
}MEA_VIR_MODE_E;

typedef enum
{
	MEA_LITTLE_ENDIAN = 0,
	MEA_BIG_ENDIAN
}MEA_ENDIAN_E;

typedef enum
{
	DCT_ZIGZAG_SCAN = 0,
	DCT_VERTICAL_SCAN,
	DCT_HORIZONTAL_SCAN,
	DCT_BYPASS_MODE
}DCT_SCAN_MODE_E;

typedef enum
{
	DCT_INPUT_BUF_BLOCK = 0,
	DCT_INPUT_BUF_MB
}DCT_INBUF_ARR_E;

typedef enum
{
	DCT_BIG_ENDIAN = 0,
	DCT_LITTLE_ENDIAN
}DCT_ENDIAN_E;

typedef struct global_control_register_tag
{
	volatile uint32 glb_clk_ena;  //[12]: dct_eb -- dct clock enable, active high
								  //[13]: mea_eb -- mea clock enable, active high
	volatile uint32 glb_buf_sw;   //[10]: mea_rbuf_sw -- mea reference buffer switch enable 
								  //	  0: arm use
								  //      1: mea use
	volatile uint32 glb_end_lan;  //[0]: ahb_endian   1: big endian   0: little endian
	volatile uint32 reserved;
	volatile uint32 reset;	      //[14]: dct reset
								  //[15]: mea reset

}GLOBAL_CTRL_REG_T;

typedef struct base_bfr
{
	uint8 *pMe_cmd_bfr_base;
	uint8 *pDct_io_bfr_base;
	uint8 *pMea_ref_bfr_base;
	uint8 *pMea_src_bfr_base;
	uint8 *pMca_output_bfr_base;
	uint8 *pMea_virtual_bfr_base;
	uint8 *pMea_ctrl_reg_base;
	uint8 *pDct_ctrl_reg_base;
	uint8 *pGlobal_reg_base;
}VSP_BASE_BFR_T;

typedef struct source_bfr_tag
{
	uint8 *pMB_y;	
	uint8 *pMB_u;	
	uint8 *pMB_v;	
	uint8 *pMB_ref_uv;
}SOURCE_BFR_T;

typedef struct mca_bfr_tag
{
	uint8 *pMB_y;	
	uint8 *pMB_u;	
	uint8 *pMB_v;	
}MCA_BFR_T;

typedef struct bfr_str_tag
{
	uint8 *pMB_y;	
	uint8 *pMB_u;	
	uint8 *pMB_v;	
}VIRTUAL_BFR_T;

/*DCT IO buffer should give the address for six block*/
typedef struct dct_io_buffer_tag
{
	int16 *pBlk[6];
}DCT_IO_BUFFER_T;

/*DCT register description*/
typedef struct dct_control_register_tag
{
	volatile uint32 dct_config;	    //0: work_mode -- 0: DCT mode; 1: IDCT mode
									//[2:1]: scan mode -- 00  zigzag scan
									//					  01  alternate-vertical scan
									//					  10  alternate-horizontal scan
									//					  11  bypass mode
									//[3]: pixel_8bit_en: pixel data should be 8 bit or coef data should be 11 bits
									//[4]: pixel_8bit_packet -- 1: a pixel one byte  0: a pixel two byte
									//[5]: sign_en -- 1  pixel data is sign   0  pixel data is unsigned
									//[6]: read inverse   1: encoder mode   0: decoder mode
									//[21:16]: 6 bit that mask the enable of the 6 8x8 block to do dct/idct
									//[24]: in_buffer_arrange -- 1 MB mode, 0 block mode
									//[25]: protect_en -- protect enable bit
									//[26]: in endian -- 0 big endian   1  little endian
									//[27]: out_endian -- 0 big endian   1 little endian

	volatile uint32 dct_quan_para;  //[0] : quant_en -- 1 quant enable   0  quant disable
									//[1]: quant mode -- 1  intra mode   0 -- inter mode
									//[4:2]: quant shift -- the shift value of quan_inv_shift
									//[12:8]: quant_value
									//[27:16]: quan_inv_shift

	volatile uint32 y_quan_para;	//[4:2]: y_quan_shift -- the shift value of y_quan_inv_shift
									//[27:16]: Y_quan_inv_shift

	volatile uint32 uv_quan_para;	//[4:2] : uv_quan_shift
									//[27:16]: uv_quan_inv_shift

	volatile uint32 reserved;		//null

	volatile uint32 dct_output_state; //[0]: dct idle
									//[13:8]: dct_cbp_state

	volatile uint32 dct_start;		//[0]: dct_start

	volatile uint32 dct_int;		//[0]: dct_int

	volatile uint32 dct_int_mask;	//[0]: dct_int_mask
	volatile uint32 dct_int_raw;	//[0]: dct_int_raw
	volatile uint32 dct_int_clr;	//[0]: dct_int_clr
}DCT_CTRL_REG_T;

/**************hardware accelerator structure***************/
typedef struct motion_info_tag
{
	volatile uint32 mea_result_mv;	//0~7: mea_x_mv -- the initial search motion vector in X direction;
									//8~15: mea_y_mv -- the initial search motion vector in Y direction
									//16~31: mea_sad -- the initial SAD input and the output SAD
}MOTION_INFO_T;

/*mea control register for MB*/
typedef struct mea_control_tag
{
	volatile uint32 mea_fm_cfg;	 	// 0~4: mea_x_max
									// 8~12: mea_y_max
	
	volatile uint32 mea_rbuf_cfg;   //0~7: mea_y_mv_max --  maximum mv in Y direction
									//8:   mea_rbfr_uv_mode  -- reference frame UV is in reference buffer or not
									//16:  mea_rbfr_mode  -- reference buffer working mode
									//24~26: mea_rbuf_ctrl -- fill Y, U or V data into reference buffer

	volatile uint32 mea_cbuf_cfg;   //0~5: mea_mb_cmd_num -- the valid command number for 16x16 mode in command buffer
									//8~13: mea_sb_cmd_num -- the valid command number for 8x8 mode in command buffer
									//16~27:  mea_stp_val -- the threshold value for all the MBs
	
	volatile uint32 mea_op_cfg;	    //0~2: mea_yuv_type -- YUV type selection  bit[2] = 1: Y;  bit[1] = 1: U   bit[0] = 1: V  // for output reference
									//8: mea_blk_mode -- block mode    0: 16x16;   1: 8x8

	volatile uint32 mea_hp_scan_cfg;//0~5: mea_hp_cmd_num -- half pixel scan CMD number
									//8: mea_hp_cal -- only do half pixel calculation
									//16: mea_hp_scan_en -- half pixel scan enable following the integer pixel search

	volatile uint32 mea_smb_cfg;	//0~4: mea_x_id -- the ID of the current source MB in X direction
									//8~12: mea_y_id -- ID of the current source MB in Y direction

	/*mea sad and vector for 16x16 MB; initial value input and result output*/
	MOTION_INFO_T motion_info_16x16;
	MOTION_INFO_T motion_info_8x8[4];

	/*MEA core control register*/
	volatile uint32 mea_operate_ctr; //bit[8]: mea work mode  0-compute sad   1-out reference mb
									 //bit[1]: mea busy
									 //bit[0]: 1 start the MEA computation

	volatile uint32 mea_core_ctrl;   //0: MEA_RBUF_SW 0: reference buffer is used by hardware
									 //1: MEA_SBUF_SW 0: source buffer is used by hardware
									 //2: MEA_OBUF_SW 0: output buffer is used by hardware
									 //3: MEA_CBUF_SW 0: command buffer is used by hardware
									 //8: MEA_DATA_INV 0: invert the frame data byte sequence.
									 //16: mea_mode -- 0: inter mode; 1: intra mode

	volatile uint32 mea_int_raw;	

	volatile uint32 mea_int_eb;

	volatile uint32 mea_int_st;

	volatile uint32 mea_int_clr;
}MEA_CTRL_REG_T;

/****MEA command structure***/
typedef struct mea_command_tag
{
	volatile uint32 cmd;	//0~7: X_MV -- motion vector offset in x direction
							//8~15: Y_MV -- motion vector offset in y direction
							//16~19: level_ID
							//24: stp_ena -- stop rule enable(if the cental pixel 
							//has smallest SAD, stop full pixel search)
}MEA_CMD_T;

/*added for vsp application code*/
typedef struct mea_bfr_tag
{
	/*reference buffer*/
	uint8 *pRef_y;
	uint8 *pRef_u;	//if big frame mode, not exist
	uint8 *pRef_v;	//if big frame mode, not exist	
	
	SOURCE_BFR_T source_bfr;
	MCA_BFR_T mca_output_bfr;
	VIRTUAL_BFR_T virtual_buffer;
}MEA_BFR_T;
/**---------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **---------------------------------------------------------------------------*/ 
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************
 **	Name : 			VSP_InitMEABuffer
 ** Description:	Initialize the VSP MEA buffer. 
 ** Author:			Binggo Zhou
 **	Note:
 *****************************************************************************/
PUBLIC void VSP_InitMEABuffer(MEA_BFR_T *pMea_bfr,VSP_MEARBUF_MODE_E ref_bfr_mode);
/*****************************************************************************
 **	Name : 			VSP_InitDCTIOBuffer
 ** Description:	Initialize the VSP DCT buffer. 
 ** Author:			Binggo Zhou
 **	Note:
 *****************************************************************************/
PUBLIC void VSP_InitDCTIOBuffer(DCT_IO_BUFFER_T *pDct_io_bfr);
/*****************************************************************************
 **	Name : 			VSP_Get_DCTIOBUF_Addr
 ** Description:	Get The Address Of DCTIOBUF. 
 ** Author:			Binggo Zhou
 **	Note:
 *****************************************************************************/
PUBLIC int32 * VSP_Get_DCTIOBUF_Addr(void);
/*****************************************************************************
 **	Name : 			VSP_InitBaseBuffer
 ** Description:	Initialize the VSP Base buffer. 
 ** Author:			Binggo Zhou
 **	Note:
 *****************************************************************************/
PUBLIC void VSP_InitBaseBuffer(VSP_BASE_BFR_T * pBase_bfr);
/*****************************************************************************
 **	Name : 			VSP_ResetAllAccelerator
 ** Description:	Reset the hardware accelerator (VSP).
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of each module
					2. reset VSP register
 *****************************************************************************/
PUBLIC void VSP_ResetAllAccelerator(void);
/*****************************************************************************
 **	Name : 			VSP_ResetDCT
 ** Description:	Reset the DCT.
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of DCT module
					2. reset DCT register
 *****************************************************************************/
PUBLIC void VSP_ResetDCT(void);
/*****************************************************************************
 **	Name : 			VSP_ResetMEA
 ** Description:	Reset the MEA.
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of MEA
					2. reset MEA register
 *****************************************************************************/
PUBLIC void VSP_ResetMEA(void);
/*****************************************************************************
 **	Name : 			VSP_CloseDCT
 ** Description:	Close the DCT.
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of DCT module
 *****************************************************************************/
PUBLIC void VSP_CloseDCT(void);
/*****************************************************************************
 **	Name : 			VSP_CloseMEA
 ** Description:	Close the MEA.
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of MEA
 *****************************************************************************/
PUBLIC void VSP_CloseMEA(void);
/*****************************************************************************
 **	Name : 			VSP_InitDCTEngine
 ** Description:	Initialize the VSP DCT Engine.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_InitDCTEngine(void);
/*****************************************************************************
 **	Name : 			VSP_InitMEAEngine
 ** Description:	Initialize the VSP MEA Engine.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_InitMEAEngine(void);
/*****************************************************************************
 **	Name : 			VSP_InitVSPEndian
 ** Description:	Initialize the VSP endian mode.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_InitVSPEndian(VSP_ENDIAN_E endian);
/*****************************************************************************
 **	Name : 			VSP_MEA_Endian_Set
 ** Description:	Initialize the MEA endian mode.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_MEA_Endian_Set(MEA_ENDIAN_E endian);
/*****************************************************************************
 **	Name : 			Mp4_InitMEACtrlRegister
 ** Description:	Initialize MEA control register.
 ** Author:			Binggo Zhou
 **	Note:			it can be modified for each MB
 *****************************************************************************/
PUBLIC void VSP_InitMEACtrlRegister(uint32 frame_width,uint32 frame_height,
					VSP_MEARBUF_MODE_E ref_bfr_mode,int32 mv_ver_range);
/*****************************************************************************
 **	Name : 			Mp4_InitMEACommandBuffer
 ** Description:	Initialize MEA command buffer.
 ** Author:			Xiaowei Luo
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_InitMEACommandBuffer(MEA_CMD_T **ppcmd_bfr);
/*****************************************************************************
 **	Name : 			VSP_Get_DCT_CBP_State
 ** Description:	Get the Current DCT Cbp State.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline uint8 VSP_Get_DCT_CBP_State(void)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	return (uint8)(pDct_ctrl_reg->dct_output_state >> DCT_CBP_STATE_BIT);   
}

/*****************************************************************************
 **	Name : 			VSP_Get_SrcMBX_ID
 ** Description:	Get the ID num of SRC MB in X direction.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline int32 VSP_Get_SrcMBX_ID(void)
{
	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
	return (pMea_ctrl_reg->mea_smb_cfg) & 0x1f;
}

/*****************************************************************************
 **	Name : 			VSP_Get_SrcMBY_ID
 ** Description:	Get the ID num of SRC MB in Y direction.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline int32 VSP_Get_SrcMBY_ID(void)
{
	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
	return (pMea_ctrl_reg->mea_smb_cfg >> 8) & 0x1f;
}

/*****************************************************************************
 **	Name : 			VSP_Get_MV_X_MB
 ** Description:	Get the MV in X direction of MB Method.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline int32 VSP_Get_MV_X_MB(void)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
 	uint32 mea_result_mv = pMea_ctrl_reg->motion_info_16x16.mea_result_mv;		
 	return ((int32)(mea_result_mv << 24)) >> 24;
}
 
 /*****************************************************************************
 **	Name : 			VSP_Get_MV_Y_MB
 ** Description:	Get the MV in Y direction of MB Method.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline int32 VSP_Get_MV_Y_MB(void)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;	
  	uint32 mea_result_mv = pMea_ctrl_reg->motion_info_16x16.mea_result_mv;
 	return ((int32)(mea_result_mv << 16)) >> 24;
}
 
 /*****************************************************************************
 **	Name : 			VSP_Get_MV_X_BLOCK
 ** Description:	Get the MV in X direction of BLOCK Method.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline int32 VSP_Get_MV_X_BLOCK(uint8 blk_num)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;	
 	return ((int32)(pMea_ctrl_reg->motion_info_8x8[blk_num].mea_result_mv << 24)) >> 24;
}

/*****************************************************************************
 **	Name : 			VSP_Get_MV_Y_BLOCK
 ** Description:	Get the MV in Y direction of BLOCK Method.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline int32 VSP_Get_MV_Y_BLOCK(uint8 blk_num)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;	
 	return ((int32)(pMea_ctrl_reg->motion_info_8x8[blk_num].mea_result_mv << 16)) >> 24;
}

/*****************************************************************************
 **	Name : 			VSP_Write_SrcPix_DCTIOBUF_CFG
 ** Description:	Config of Writing 8bit Pixel Date to DCT IOBUF Before DCT.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline void VSP_Write_SrcPix_DCTIOBUF_CFG(DCT_INBUF_ARR_E inbuf_method, DCT_ENDIAN_E input_endian,DCT_ENDIAN_E output_endian)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;	
	pDct_ctrl_reg->dct_config	= PIXEL_8BIT << DCT_PIXEL_8BIT_EN_BIT			|
								  PIXEL_BYTE_SAVE << DCT_PIXEL_8BIT_PACKET_BIT	|
								  UNSIGN_PIXEL_8BIT << DCT_SIGN_EN_BIT			|
								  inbuf_method << DCT_IN_BUF_ARRANGE_BIT		|
								  input_endian << DCT_IN_ENDIAN_BIT				|
								  output_endian << DCT_OUT_ENDIAN_BIT;
}	

/*****************************************************************************
 **	Name : 			VSP_Write_ErrData_DCTIOBUF_CFG
 ** Description:	Config of Writing Err Data to DCT IOBUF Before DCT.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline void VSP_Write_ErrData_DCTIOBUF_CFG(DCT_INBUF_ARR_E inbuf_method, DCT_ENDIAN_E input_endian,DCT_ENDIAN_E output_endian)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;	
	pDct_ctrl_reg->dct_config	= PIXEL_9BIT << DCT_PIXEL_8BIT_EN_BIT			|
								  PIXEL_WORD_SAVE << DCT_PIXEL_8BIT_PACKET_BIT	|
								  SIGN_PIXEL_8BIT << DCT_SIGN_EN_BIT			|
								  inbuf_method << DCT_IN_BUF_ARRANGE_BIT		|
								  input_endian << DCT_IN_ENDIAN_BIT				|
								  output_endian << DCT_OUT_ENDIAN_BIT;		
}

/********************************************************************************
 **	Name : 			VSP_Write_COF_DCTIOBUF_CFG
 ** Description:	Config of Writing DCT IOBUF Before IDCT.
 ** Author:			Binggo Zhou
 **	Note:			The Endian Setting under IDCT mode is different from DCT mode!
 ********************************************************************************/
PUBLIC __inline void VSP_Write_COF_DCTIOBUF_CFG(DCT_SCAN_MODE_E scan_mode,DCT_INBUF_ARR_E inbuf_method,DCT_ENDIAN_E input_endian,DCT_ENDIAN_E output_endian)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;	
	pDct_ctrl_reg->dct_config = (VSP_IDCT  << DCT_WORK_MODE_BIT)		|
						  scan_mode		   << DCT_SCAN_MODE_BIT			|
						  PIXEL_9BIT	   << DCT_PIXEL_8BIT_EN_BIT		|
						  PIXEL_WORD_SAVE  << DCT_PIXEL_8BIT_PACKET_BIT |
						  SIGN_PIXEL_8BIT  << DCT_SIGN_EN_BIT 			|
						  NOT_INVERSE	   << DCT_READINVERSE_BIT	 	|
						  inbuf_method  << DCT_IN_BUF_ARRANGE_BIT	|
						  DCT_PRCT_DISABLE << DCT_PROTECT_EN_BIT		|
						  input_endian	   << DCT_IN_ENDIAN_BIT			|
						  output_endian	   << DCT_OUT_ENDIAN_BIT;		
}

/*****************************************************************************
 **	Name : 			VSP_Read_DCTIOBUF_DCT_CFG
 ** Description:	Config of Reading DCT IOBUF After Intra DCT.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline void VSP_Read_DCTIOBUF_DCT_CFG(DCT_SCAN_MODE_E scan_mode)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;	
	pDct_ctrl_reg->dct_config = (pDct_ctrl_reg->dct_config & 0xfffffff9) | (scan_mode << DCT_SCAN_MODE_BIT);
}

/*****************************************************************************
 **	Name : 			VSP_Read_DCTIOBUF_IDCT_CFG
 ** Description:	Config of Reading DCT IOBUF After IDCT.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/	
PUBLIC __inline void VSP_Read_DCTIOBUF_IDCT_CFG(void)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;	
	pDct_ctrl_reg->dct_config = (pDct_ctrl_reg->dct_config & 0xfffffff9) | (DCT_BYPASS_MODE << DCT_SCAN_MODE_BIT);
}

/*****************************************************************************
 **	Name : 			VSP_Disable_Quan
 ** Description:	Disable Quan.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline void VSP_Disable_Quan(void)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_quan_para = 	QUANT_DIS << DCT_QUAN_EN_BIT;
}

/*****************************************************************************
 **	Name : 			VSP_Quan_Intra_CFG
 ** Description:	Config Before Intra Quan.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_Quan_Intra_CFG(uint32 shift_bits,uint32 Qp,uint32 shift_val,uint32 Y_inv_shift,uint32 Y_shift,uint32 UV_inv_shift,uint32 UV_shift)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_quan_para =	QUANT_EN 		<<  DCT_QUAN_EN_BIT				|\
									INTRA_QUANT		<<  DCT_QUAN_MODE_BIT			|\
									shift_bits		<<  DCT_QUAN_SHIFT_BIT			|\
									Qp				<<  DCT_QAUN_VALUE_BIT			|\
									shift_val		<<  DCT_QUAN_INV_SHIFT_BIT; 	\
	pDct_ctrl_reg->y_quan_para 	 =  Y_inv_shift 	<< DCT_Y_QUAN_INV_SHIFT_BIT 	|\
									Y_shift			<< DCT_Y_QUAN_SHIFT_BIT;		\
	pDct_ctrl_reg->uv_quan_para  = 	UV_inv_shift 	<< DCT_UV_QUAN_INV_SHIFT_BIT 	|\
									UV_shift		<< DCT_UV_QUAN_SHIFT_BIT;		\
}

/*****************************************************************************
 **	Name : 			VSP_Quan_Inter_CFG
 ** Description:	Config Before Inter Quan.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_Quan_Inter_CFG(uint32 shift_bits,uint32 Qp,uint32 shift_val)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_quan_para =	QUANT_EN 		<<  DCT_QUAN_EN_BIT				|
									INTER_QUANT		<<  DCT_QUAN_MODE_BIT			|
									shift_bits		<<  DCT_QUAN_SHIFT_BIT			|
									Qp				<<  DCT_QAUN_VALUE_BIT			|
									shift_val		<<  DCT_QUAN_INV_SHIFT_BIT; 	
}

/*****************************************************************************
 **	Name : 			VSP_IQuan_Inter_CFG
 ** Description:	Config Before Inter IQuan.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline void VSP_IQuan_Inter_CFG(uint32 QP)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_quan_para = QUANT_EN	 	<< DCT_QUAN_EN_BIT			|
								   INTER_QUANT	<< DCT_QUAN_MODE_BIT		|
								   QP			<<  DCT_QAUN_VALUE_BIT;
}

/*****************************************************************************
 **	Name : 			VSP_IQuan_Intra_CFG
 ** Description:	Config Before Intra IQuan.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline void VSP_IQuan_Intra_CFG(uint32 QP,uint8 DcScalerY,uint8 DcScalerC)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_quan_para = QUANT_EN	 	 << DCT_QUAN_EN_BIT			|
								   INTRA_QUANT	 << DCT_QUAN_MODE_BIT		|
								   QP			 <<  DCT_QAUN_VALUE_BIT;

	pDct_ctrl_reg->y_quan_para =   DcScalerY     << DCT_Y_QUAN_INV_SHIFT_BIT;
	pDct_ctrl_reg->uv_quan_para =  DcScalerC     << DCT_UV_QUAN_INV_SHIFT_BIT;
}


/**********************************************************************************
 **	Name : 			VSP_ExternalData_IDCT_CFG
 ** Description:	Config IDCT When DCTBUF Data is from Ext.
 ** Author:			Binggo Zhou
 **	Note:			Read Inverse Bit Must not be Set if data in DCTBUF is from Ext
 *********************************************************************************/
PUBLIC __inline void VSP_ExternalData_IDCT_CFG(uint32 cbp,DCT_ENDIAN_E input_endian,DCT_ENDIAN_E output_endian)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_config = (VSP_IDCT  << DCT_WORK_MODE_BIT)		|
						  DCT_BYPASS_MODE	   << DCT_SCAN_MODE_BIT			|
						  PIXEL_9BIT	   << DCT_PIXEL_8BIT_EN_BIT		|
						  PIXEL_WORD_SAVE  << DCT_PIXEL_8BIT_PACKET_BIT |
						  SIGN_PIXEL_8BIT  << DCT_SIGN_EN_BIT 			|
						  NOT_INVERSE	   << DCT_READINVERSE_BIT	 	|
						  cbp			   << DCT_BLOCK_MASK_BIT		|
						  DCT_PRCT_DISABLE << DCT_PROTECT_EN_BIT		|
						  input_endian	   << DCT_IN_ENDIAN_BIT			|
						  output_endian	   << DCT_OUT_ENDIAN_BIT;		
}

/*********************************************************************************
 **	Name : 			VSP_InternalData_IDCT_CFG
 ** Description:	Config IDCT When DCTBUF Data is from Internal.
 ** Author:			Binggo Zhou
 **	Note:			Read Inverse Bit Must Be Set if data in DCTBUF is from Internal
 *********************************************************************************/
PUBLIC __inline VSP_InternalData_IDCT_CFG(uint32 cbp,DCT_ENDIAN_E input_endian,DCT_ENDIAN_E output_endian)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_config = (VSP_IDCT  << DCT_WORK_MODE_BIT)		|
						  DCT_BYPASS_MODE	   << DCT_SCAN_MODE_BIT			|
						  PIXEL_9BIT	   << DCT_PIXEL_8BIT_EN_BIT		|
						  PIXEL_WORD_SAVE  << DCT_PIXEL_8BIT_PACKET_BIT |
						  SIGN_PIXEL_8BIT  << DCT_SIGN_EN_BIT 			|
						  READ_INVERSE	   << DCT_READINVERSE_BIT	 	|
						  cbp			   << DCT_BLOCK_MASK_BIT		|
						  DCT_PRCT_DISABLE << DCT_PROTECT_EN_BIT		|
						  input_endian	   << DCT_IN_ENDIAN_BIT			|
						  output_endian	   << DCT_OUT_ENDIAN_BIT;	
}
	
/*****************************************************************************
 **	Name : 			VSP_SrcPix_DCT_CFG
 ** Description:	Config DCT When DCTBUF Data is Src Pixel Dat.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/								
PUBLIC __inline VSP_SrcPix_DCT_CFG(DCT_ENDIAN_E input_endian,DCT_ENDIAN_E output_endian)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_config = (VSP_DCT	      << DCT_WORK_MODE_BIT)		|
						  		DCT_BYPASS_MODE		  << DCT_SCAN_MODE_BIT			|
						  		PIXEL_8BIT		  << DCT_PIXEL_8BIT_EN_BIT		|
						  		PIXEL_BYTE_SAVE	  << DCT_PIXEL_8BIT_PACKET_BIT	|
						  		UNSIGN_PIXEL_8BIT << DCT_SIGN_EN_BIT			|
						  		0x3f			  << DCT_BLOCK_MASK_BIT			|
						  		DCT_PRCT_DISABLE  << DCT_PROTECT_EN_BIT			|
						  		input_endian	  << DCT_IN_ENDIAN_BIT			|
						 		output_endian	  << DCT_OUT_ENDIAN_BIT;
}

/*****************************************************************************
 **	Name : 			VSP_ErrData_DCT_CFG
 ** Description:	Config DCT When DCTBUF Data is Err Dat.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_ErrData_DCT_CFG(DCT_ENDIAN_E input_endian,DCT_ENDIAN_E output_endian)
{
	DCT_CTRL_REG_T *pDct_ctrl_reg = (DCT_CTRL_REG_T *)DCT_CTRL_BASE;
	pDct_ctrl_reg->dct_config = (VSP_DCT	      << DCT_WORK_MODE_BIT)			|
						  		DCT_BYPASS_MODE		  << DCT_SCAN_MODE_BIT			|
						  		PIXEL_9BIT		  << DCT_PIXEL_8BIT_EN_BIT		|
						  		PIXEL_WORD_SAVE	  << DCT_PIXEL_8BIT_PACKET_BIT	|
						  		SIGN_PIXEL_8BIT << DCT_SIGN_EN_BIT				|
						  		0x3f			  << DCT_BLOCK_MASK_BIT			|
						  		DCT_PRCT_DISABLE  << DCT_PROTECT_EN_BIT			|
						  		input_endian	  << DCT_IN_ENDIAN_BIT			|
						 		output_endian	  << DCT_OUT_ENDIAN_BIT;
}

/*****************************************************************************
 **	Name : 			VSP_MEA_BLK_Mode_CFG
 ** Description:	Config MEA Block Mode.16*16 or 8*8
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_MEA_BLK_Mode_CFG(MEA_BLOCK_MODE_E blk_mode)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;	
	if(blk_mode)
		pMea_ctrl_reg->mea_op_cfg |= BIT_8;
	else
		pMea_ctrl_reg->mea_op_cfg &= (~BIT_8);
}

/*****************************************************************************
 **	Name : 			VSP_Write_RBUF_CFG
 ** Description:	Control the Data Type Before Writing RBUF.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_Write_RBUF_CFG(MEA_RBUF_DATYPE_E data_type)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
	pMea_ctrl_reg->mea_rbuf_cfg = (pMea_ctrl_reg->mea_rbuf_cfg & 0xf8ffffff) | data_type << MEA_RBUF_CTRL_BIT;
}

/*****************************************************************************
 **	Name : 			VSP_MEA_SRCMB_ID_CFG
 ** Description:	Setting Current SRC MB ID.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_MEA_SRCMB_ID_CFG(int32 mb_x,int32 mb_y)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
	pMea_ctrl_reg->mea_smb_cfg = (pMea_ctrl_reg->mea_smb_cfg & 0xffffe0e0) | mb_x | (mb_y << MEA_Y_ID_BIT);
}

/*****************************************************************************
 **	Name : 			VSP_MEA_MV_MB_CFG
 ** Description:	Setting MV under MB Mode.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_MEA_MV_MB_CFG(int32 mv_x,int32 mv_y)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
	pMea_ctrl_reg->motion_info_16x16.mea_result_mv = 0xffff0000 | ((mv_y << MEA_Y_MV_BIT) & 0xff00) | (mv_x & 0xff);
}	

/*****************************************************************************
 **	Name : 			VSP_MEA_MV_BLOCK_CONFIG
 ** Description:	Setting MV under Block Mode.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_MEA_MV_BLOCK_CFG(uint8 block_num,int32 mv_x,int32 mv_y)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
	pMea_ctrl_reg->motion_info_8x8[block_num].mea_result_mv = 0xffff0000 | ((mv_y << MEA_Y_MV_BIT) & 0xff00) | (mv_x & 0xff);
}

/*****************************************************************************
 **	Name : 			VSP_MEA_Output_Data
 ** Description:	Control Output Data Type.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_MEA_Output_Data(MEA_OUT_TYPE_E data_type)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
	pMea_ctrl_reg->mea_op_cfg = (pMea_ctrl_reg->mea_op_cfg & 0xfffffff8) | data_type;
} 

/*****************************************************************************
 **	Name : 			VSP_MEA_Mode_CFG
 ** Description:	Config MEA Mode.Inter Or Intra Mode,Virture Buffer.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC __inline VSP_MEA_Mode_CFG(MEA_VIR_MODE_E mea_mode)
{
 	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T *)MEA_CTRL_BASE;
	pMea_ctrl_reg->mea_core_ctrl = (pMea_ctrl_reg->mea_core_ctrl & 0xffff) | (mea_mode << MEA_MODE_BIT);
}
/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
