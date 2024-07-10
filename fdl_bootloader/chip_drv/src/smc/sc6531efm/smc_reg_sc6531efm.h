/******************************************************************************
 ** File Name:    emc_reg_v5.h                                           *
 ** Author:       haiyang.hu                                                  *
 ** DATE:         05/31/2009                                                  *
 ** Copyright:    2009 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 ******************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------*
 ** DATE          NAME            DESCRIPTION                                 *
 ** 06/15/2007    Daniel.Ding     Create.                                     *
 ******************************************************************************/
#ifndef _EMC_REG_V5_H_
#define _EMC_REG_V5_H_
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
typedef struct
{
	volatile uint32 cfg0;                //0x0000
	volatile uint32 cfg1;                //0x0004
	volatile uint32 cfg2;                //0x0008
	volatile uint32 reserve0[4];
	volatile uint32 sts0;                //0x001C
	volatile uint32 dl[7];               //0x0020 ~ 0x0038
	volatile uint32 reserve1;
	volatile uint32 reserve2[4];
	volatile uint32 cfg0_cs1;            //0x0050
	volatile uint32 cfg1_cs1;            //0x0054
	volatile uint32 rsv_cfg2;
	volatile uint32 cfg3_cs1;            //0x005C
	volatile uint32 reserve3[4];
	volatile uint32 reserve4[4];
	volatile uint32 mem_sel;             //0x0080
	volatile uint32 opt_sel;             //0x0084
	volatile uint32 ce_len_cfg;          //0x0088
	volatile uint32 init2_start;         //0x008C
	volatile uint32 reserve5;
	volatile uint32 time_cfg_cs1;        //0x0094
	volatile uint32 reserve6[2];
	volatile uint32 mem_sel_preserve;    //0x00A0
	volatile uint32 ip_revision;         //0x00A4
}SMC_CTRL_REG_T,*SMC_CTRL_REG_PTR_T;

typedef enum {
	INTERFACE_ADP,
	INTERFACE_ADMUX,
	INTERFACE_AADMUX
}INTERFACE_E;

typedef struct
{
	uint32		vendorID;

	//smc_cfg0
	uint32		wtw;                         //write to write turn around time cfg0[30:28]
	uint32		wtr;                         //write to read turn around time  cfg0[26:24]
	uint32		rtw;                         //read to write turn around time  cfg0[22:20]
	uint32		rtr;                         //read to read turn around time   cfg0[18:16]
	uint32		clk_out_dly;                 //clock opening delay cycle number from synchronous burst end  cfg0[13:11]
	uint32		clk_out_mode;                //output clock mode               cfg0[7:6]

	//smc_cfg2
	BOOLEAN     dll_en;                      //dll enable
	uint32		dll_cpst_thr;
	uint32		dll_init_val;

	//smc_dl
	uint32		dly_dl_sel[7];
	
	//smc_cfg0_cs1
	INTERFACE_E				psram_if;       //adp/admux/aadmux
	CRAM_ROW_LENGTH_E		row_length;
	SMC_BURST_LENGTH_E		burst_length;
	SMC_WRAPMODE_E			wrap_mode;      //burst mode: wrap/increase
	SMC_WORKMODE_E			work_mode;      //asynchronous single/synchronous burst

	//smc_cfg1_cs1
	uint32		r_wait_dly;                 //delay time between wait input and input data     cs1_cfg1[25:24]

	//smc_cfg3_cs1
	uint32		buffer_read_sel;            //select between direct read and buffered read     cs1_cfg3[28:26]
	uint32		sample_dly;                 //delay cycle of input data sample logic(for ddr)  cs1_cfg3[25:24]

	//time_cfg_cs1
	uint32		sample_setup_time;			//sample setup time                                           time_cfg_cs1[20:16]
	uint32		next_write_dly;             //for AADMUX write, this field indicates the next data delay  time_cfg_cs1[15:14]
	uint32		first_write_dly;            //for AADMUX write, this field indicates the first data delay time_cfg_cs1[13:8]
	uint32		next_read_dly;              //for AADMUX read, this field indicates the next data delay   time_cfg_cs1[7:6]	
	uint32		first_read_dly;             //for AADMUX read, this field indicates the first data delay  time_cfg_cs1[5:0]

	//psram parameter
	uint32      drive_strength;             //psram drive strength
}PSRAM_SPEC;

/*****************************************************/
/*						  CFG0					     */
/*****************************************************/
#define SMC_WRITE_TO_WRITE_SHIFT        28
#define SMC_WRITE_TO_WRITE_MASK         (0x3 << SMC_WRITE_TO_WRITE_SHIFT)
#define SMC_WRITE_TO_READ_SHIFT         24
#define SMC_WRITE_TO_READ_MASK          (0x3 << SMC_WRITE_TO_READ_SHIFT)
#define SMC_READ_TO_WRITE_SHIFT         20
#define SMC_READ_TO_WRITE_MASK          (0x3 << SMC_READ_TO_WRITE_SHIFT)
#define SMC_READ_TO_READ_SHIFT          16
#define SMC_READ_TO_READ_MASK           (0x3 << SMC_READ_TO_READ_SHIFT)
#define SMC_SAMPLE_AUTO_RST_EN_SHIFT	15
#define SMC_SAMPLE_AUTO_RST_EN			(0x1 << SMC_SAMPLE_AUTO_RST_EN_SHIFT)
#define SMC_CLK_SAMPLE_SEL_SHIFT		14
#define SMC_CLK_SAMPLE_SEL_MASK			(0x1 << SMC_CLK_SAMPLE_SEL_SHIFT)
#define SMC_CLK_OUT_DLY_SHIFT			11
#define SMC_CLK_OUT_DLY_MASK			(0x7 << SMC_CLK_OUT_DLY_SHIFT)
#define SMC_SAMPLE_RST_SHIFT			8
#define SMC_SAMPLE_RST					(0x1 << SMC_SAMPLE_RST_SHIFT)
#define SMC_CLK_OUT_MODE_SHIFT			6
#define SMC_CLK_OUT_MODE_MASK			(0x3 << SMC_CLK_OUT_MODE_SHIFT)
#define SMC_CLK_MODE_SHIFT				4
#define SMC_CLK_MODE_MASK				(0x1 << SMC_CLK_MODE_SHIFT)
#define SMC_CS_POSITION_SHIFT			0
#define SMC_CS_POSITION_MASK			(0x3 << SMC_CS_POSITION_SHIFT)

/*****************************************************/
/*						  CFG1					     */
/*****************************************************/
#define SMC_CLK_SAMPLE_2X_PHASE_SHIFT	3
#define SMC_CLK_SAMPLE_2X_PHASE_MASK	(0x1 << SMC_CLK_SAMPLE_2X_PHASE_SHIFT)
#define SMC_CLK_SAMPLE_2X_EN_SHIFT		2
#define SMC_CLK_SAMPLE_2X_EN_MASK		(0x1 << SMC_CLK_SAMPLE_2X_EN_SHIFT)
#define SMC_CLK_OUT_2X_EN_SHIFT			1
#define SMC_CLK_OUT_2X_EN_MASK			(0x1 << SMC_CLK_OUT_2X_EN_SHIFT)
#define SMC_CLK_2X_EN_SHIFT				0
#define SMC_CLK_2X_EN_MASK				(0x1 << SMC_CLK_2X_EN_SHIFT)

/*****************************************************/
/*						  CFG2					     */
/*****************************************************/
#define SMC_DLL_CPST_THR_SHIFT			12
#define SMC_DLL_CPST_THR_MASK			(0xF << SMC_DLL_CPST_THR_SHIFT)
#define SMC_DLL_CPST_EN_SHIFT			10
#define SMC_DLL_CPST_EN					(0x1 << SMC_DLL_CPST_EN_SHIFT)
#define SMC_DLL_AUTO_CLR_EN_SHIFT		9
#define SMC_DLL_AUTO_CLR_EN_MASK		(0x1 << SMC_DLL_AUTO_CLR_EN_SHIFT)
#define SMC_DLL_EN_SHIFT				7
#define SMC_DLL_EN						(0x1 << SMC_DLL_EN_SHIFT)
#define SMC_DLL_INIT_SHIFT				0
#define SMC_DLL_INIT_MASK				(0x7F << SMC_DLL_INIT_SHIFT)

/*****************************************************/
/*						DLX					         */
/*****************************************************/
#define SMC_DLX_DLY_CPST_EN_SHIFT		8
#define SMC_DLX_DLY_CPST_EN				(0x1 << SMC_DLX_DLY_CPST_EN_SHIFT)
#define SMC_DLX_DLY_INV_SHIFT			7
#define SMC_DLX_DLY_INV_MASK			(0x1 << SMC_DLX_DLY_INV_SHIFT)
#define SMC_DLX_DLY_SEL_SHIFT			0
#define SMC_DLX_DLY_SEL_MASK			(0x7F << SMC_DLX_DLY_SEL_SHIFT)

/*****************************************************/
/*						CFG0_CS1					 */
/*****************************************************/
#define SMC_DDR_EN_SHIFT				27
#define SMC_DDR_EN						(0x1 << SMC_DDR_EN_SHIFT)
#define SMC_AADMUX_EN_SHIFT				25
#define SMC_AADMUX_EN					(0x1 << SMC_AADMUX_EN_SHIFT)
#define SMC_ADMUX_EN_SHIFT				23
#define SMC_ADMUX_EN					(0x1 << SMC_ADMUX_EN_SHIFT)
#define SMC_ROW_LENGTH_SHIFT			20
#define SMC_ROW_LENGTH_MASK				(0x7 << SMC_ROW_LENGTH_SHIFT)
#define SMC_ROW_DETECT_EN_W_SHIFT		19
#define SMC_ROW_DETECT_EN_W				(0x1 << SMC_ROW_DETECT_EN_W_SHIFT)
#define SMC_ROW_DETECT_EN_R_SHIFT		18
#define SMC_ROW_DETECT_EN_R				(0x1 << SMC_ROW_DETECT_EN_R_SHIFT)
#define SMC_BURST_MODE_W_SHIFT			11
#define SMC_BURST_MODE_W_MASK			(0x1 << SMC_BURST_MODE_W_SHIFT)
#define SMC_BURST_LENGTH_W_SHIFT		8
#define SMC_BURST_LENGTH_W_MASK			(0x7 << SMC_BURST_LENGTH_W_SHIFT)
#define SMC_BURST_MODE_R_SHIFT			7
#define SMC_BURST_MODE_R_MASK			(0x1 << SMC_BURST_MODE_R_SHIFT)
#define SMC_BURST_LENGTH_R_SHIFT		4
#define SMC_BURST_LENGTH_R_MASK			(0x7 << SMC_BURST_LENGTH_R_SHIFT)
#define SMC_MODE_W_SHIFT				2
#define SMC_MODE_W_MASK					(0x3 << SMC_MODE_W_SHIFT)
#define SMC_MODE_R_SHIFT				0
#define SMC_MODE_R_MASK					(0x3 << SMC_MODE_R_SHIFT)

/*****************************************************/
/*						CFG1_CS1					 */
/*****************************************************/
#define SMC_T_WAIT_DLY_R_SHIFT			24
#define SMC_T_WAIT_DLY_R_MASK			(0x3 << SMC_T_WAIT_DLY_R_SHIFT)

/*****************************************************/
/*						CFG3_CS1					 */
/*****************************************************/
#define SMC_T_BUFFER_READ_SEL_SHIFT		26
#define SMC_T_BUFFER_READ_SEL_MASK		(0x7 << SMC_T_BUFFER_READ_SEL_SHIFT)
#define SMC_T_SAMPLE_DLY_DDR_SHIFT		24
#define SMC_T_SAMPLE_DLY_DDR_MASK		(0x3 << SMC_T_SAMPLE_DLY_DDR_SHIFT)

/*****************************************************/
/*						MEM_SEL  					 */
/*****************************************************/
#define SMC_MEM_SEL_SHIFT				0
#define SMC_MEM_SEL						(0x1 << SMC_MEM_SEL_SHIFT)

/*****************************************************/
/*						OPT_SEL  					 */
/*****************************************************/
#define SMC_OPT_SEL_SHIFT				0
#define SMC_OPT_SEL						(0x1 << SMC_OPT_SEL_SHIFT)

/*****************************************************/
/*						CE_LEN_CFG					 */
/*****************************************************/
#define SMC_CE_LEN_CFG_SHIFT			0
#define SMC_CE_LEN_CFG_MASK				(0xF << SMC_CE_LEN_CFG_SHIFT)

/*****************************************************/
/*						INIT2_START					 */
/*****************************************************/
#define SMC_INIT2_START_SHIFT			0
#define SMC_INIT2_START					(0x1 << SMC_INIT2_START_SHIFT)

/*****************************************************/
/*						TIM_CFG_CS1					 */
/*****************************************************/
#define SMC_SAMPLE_STP_SHIFT			16
#define SMC_SAMPLE_STP_MASK				(0x1F << SMC_SAMPLE_STP_SHIFT)
#define SMC_2ND_DLY_W_SHIFT				14
#define SMC_2ND_DLY_W_MASK				(0x3 << SMC_2ND_DLY_W_SHIFT)
#define SMC_1ST_DLY_W_SHIFT				8
#define SMC_1ST_DLY_W_MASK				(0x3F << SMC_1ST_DLY_W_SHIFT)
#define SMC_2ND_DLY_R_SHIFT				6
#define SMC_2ND_DLY_R_MASK				(0x3 << SMC_2ND_DLY_R_SHIFT)
#define SMC_1ST_DLY_R_SHIFT				0
#define SMC_1ST_DLY_R_MASK				(0x3F << SMC_1ST_DLY_R_SHIFT)

/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/


/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
/**---------------------------------------------------------------------------*/
#endif

