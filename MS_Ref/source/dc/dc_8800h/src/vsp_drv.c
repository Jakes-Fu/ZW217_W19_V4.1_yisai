/******************************************************************************
 ** File Name:      vsp_drv.h                                                 *
 ** Author:         Binggo Zhou                                               *
 ** DATE:           4/23/2007                                                 *
 ** Copyright:      2007 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    VSP Driver for video codec.	                              *
 ** Notes:			Most Interfaces have been defined in VSP_drv.h            *
 **					used MACRO Define Method and Inline Function              *
 **					in order to improve codec efficiency. 	       	          *
 ******************************************************************************
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 4/23/2007    Binggo Zhou     Create.                                      *
 *****************************************************************************/
#include "vsp_drv.h"
//#include "arm_reg.h"
/**---------------------------------------------------------------------------*
**                        Compiler Flag                                       *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/*----------------------------------------------------------------------------*
**                            Mcaro Definitions                               *
**---------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                             Data Prototype                                 **
**----------------------------------------------------------------------------*/
LOCAL VSP_BASE_BFR_T s_vsp_base_bfr;
LOCAL MEA_BFR_T s_mea_bfr;
LOCAL  DCT_IO_BUFFER_T s_dct_bfr;
/**---------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                  *
 **---------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                */
/*--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **---------------------------------------------------------------------------*/

/**----------------------------------------------------------------------------*
**                         Local Function Prototype                           **
**----------------------------------------------------------------------------*/
/**----------------------------------------------------------------------------*
**                           Function Prototype                               **
**----------------------------------------------------------------------------*/
/*****************************************************************************
 **	Name : 			VSP_InitMEABuffer
 ** Description:	Initialize the VSP MEA buffer. 
 ** Author:			Binggo Zhou
 **	Note:
 *****************************************************************************/
PUBLIC void VSP_InitMEABuffer(MEA_BFR_T *pMea_bfr,VSP_MEARBUF_MODE_E ref_bfr_mode)
{
	SOURCE_BFR_T *pSource_bfr;
	MCA_BFR_T *pMca_output_bfr;
	VIRTUAL_BFR_T *pVirtual_buffer;
	VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;
	
	/*init mea buffer base address*/
	pMea_bfr->pRef_y = (uint8 *) MEA_REF_BFR_BASE;

	if(ref_bfr_mode == MEA_REF_BFR_MODE_2048)
	{
		pMea_bfr->pRef_u = (uint8 *)(pMea_bfr->pRef_y + 2048*4);     //2048 MODE
		pMea_bfr->pRef_v = (uint8 *)(pMea_bfr->pRef_u + 1024*4);
	}
		
	/*source buffer map*/
	pSource_bfr = &(pMea_bfr->source_bfr);
	pSource_bfr->pMB_y = (uint8 *) MEA_SRC_BFR_BASE;
	pSource_bfr->pMB_u = (uint8 *)(pSource_bfr->pMB_y + MB_SIZE * MB_SIZE);
	pSource_bfr->pMB_v = (uint8 *)(pSource_bfr->pMB_u + BLOCK_SIZE*BLOCK_SIZE);
	pSource_bfr->pMB_ref_uv = (uint8 *)(pSource_bfr->pMB_v + BLOCK_SIZE * BLOCK_SIZE); 

	if(ref_bfr_mode == MEA_REF_BFR_MODE_4096)
	{
		pMea_bfr->pRef_u = pSource_bfr->pMB_ref_uv;     //4096 MODE
		pMea_bfr->pRef_v = pSource_bfr->pMB_ref_uv;
	}

	/*mca output buffer map*/
	pMca_output_bfr = &(pMea_bfr->mca_output_bfr);
	pMca_output_bfr->pMB_y = (uint8 *) MCA_OUTPUT_BFR_BASE;
	pMca_output_bfr->pMB_u = (uint8 *)(pMca_output_bfr->pMB_y + MB_SIZE * MB_SIZE);
	pMca_output_bfr->pMB_v = (uint8 *)(pMca_output_bfr->pMB_u + BLOCK_SIZE*BLOCK_SIZE);

	/*mea virtual buffer*/
	pVirtual_buffer = &(pMea_bfr->virtual_buffer);
	pVirtual_buffer->pMB_y = (uint8 *)MEA_VIR_BFR_BASE;
	pVirtual_buffer->pMB_u = (uint8 *)pVirtual_buffer->pMB_y + MB_SIZE * MB_SIZE * 2;
	pVirtual_buffer->pMB_v = (uint8 *)pVirtual_buffer->pMB_u + BLOCK_SIZE * BLOCK_SIZE * 2;

}

/*****************************************************************************
 **	Name : 			VSP_InitDCTIOBuffer
 ** Description:	Initialize the VSP DCT buffer. 
 ** Author:			Binggo Zhou
 **	Note:
 *****************************************************************************/
PUBLIC void VSP_InitDCTIOBuffer(DCT_IO_BUFFER_T *pDct_io_bfr)
{	
	pDct_io_bfr->pBlk[0] = s_dct_bfr.pBlk[0] = (int16 *)DCT_IO_BFR_BASE;
	pDct_io_bfr->pBlk[1] = s_dct_bfr.pBlk[1] = (int16 *)(pDct_io_bfr->pBlk[0] + 64);
	pDct_io_bfr->pBlk[2] = s_dct_bfr.pBlk[2] = (int16 *)(pDct_io_bfr->pBlk[1] + 64);
	pDct_io_bfr->pBlk[3] = s_dct_bfr.pBlk[3] = (int16 *)(pDct_io_bfr->pBlk[2] + 64);
	pDct_io_bfr->pBlk[4] = s_dct_bfr.pBlk[4] = (int16 *)(pDct_io_bfr->pBlk[3] + 64);
	pDct_io_bfr->pBlk[5] = s_dct_bfr.pBlk[5] = (int16 *)(pDct_io_bfr->pBlk[4] + 64);
}

/*****************************************************************************
 **	Name : 			VSP_Get_DCTIOBUF_Addr
 ** Description:	Get The Address Of DCTIOBUF. 
 ** Author:			Binggo Zhou
 **	Note:
 *****************************************************************************/
PUBLIC int32 * VSP_Get_DCTIOBUF_Addr(void)
{	
	return (int32 *)DCT_IO_BFR_BASE;
}

/*****************************************************************************
 **	Name : 			VSP_InitBaseBuffer
 ** Description:	Initialize the VSP Base buffer. 
 ** Author:			Binggo Zhou
 **	Note:
 *****************************************************************************/
PUBLIC void VSP_InitBaseBuffer(VSP_BASE_BFR_T * pBase_bfr)
{
	//ASSERT pBase_bfr NULL
	
	pBase_bfr->pDct_io_bfr_base	     = s_vsp_base_bfr.pDct_io_bfr_base = (uint8 *)DCT_IO_BFR_BASE;
	pBase_bfr->pMe_cmd_bfr_base	     = s_vsp_base_bfr.pMe_cmd_bfr_base = (uint8 *)MEA_CMD_BFR_BASE;
	pBase_bfr->pMea_ref_bfr_base	 = s_vsp_base_bfr.pMea_ref_bfr_base = (uint8 *)MEA_REF_BFR_BASE;
	pBase_bfr->pMea_src_bfr_base	 = s_vsp_base_bfr.pMea_src_bfr_base = (uint8 *)MEA_SRC_BFR_BASE;
	pBase_bfr->pMca_output_bfr_base	 = s_vsp_base_bfr.pMca_output_bfr_base = (uint8 *)MCA_OUTPUT_BFR_BASE;
	pBase_bfr->pMea_virtual_bfr_base = s_vsp_base_bfr.pMea_virtual_bfr_base = (uint8 *)MEA_VIR_BFR_BASE;
	
	pBase_bfr->pMea_ctrl_reg_base 	 = s_vsp_base_bfr.pMea_ctrl_reg_base = (uint8 *)MEA_CTRL_BASE;
	pBase_bfr->pDct_ctrl_reg_base    = s_vsp_base_bfr.pDct_ctrl_reg_base = (uint8 *)DCT_CTRL_BASE; 
	pBase_bfr->pGlobal_reg_base	     = s_vsp_base_bfr.pGlobal_reg_base = (uint8 *)AHB_GENERAL_CONTROL_REG_BASE;
}

/*****************************************************************************
 **	Name : 			VSP_ResetAllAccelerator
 ** Description:	Reset the hardware accelerator (VSP).
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of each module
					2. reset VSP register
 *****************************************************************************/
PUBLIC void VSP_ResetAllAccelerator(void)
{   
	//VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;
	//GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)pBase_bfr->pGlobal_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)AHB_GENERAL_CONTROL_REG_BASE;
    
    pGlobal_ctrl_reg->reset |= BIT_14;//DCT Reset
    pGlobal_ctrl_reg->reset |= BIT_15;//MEA Reset
    pGlobal_ctrl_reg->reset &= (~BIT_14);
    pGlobal_ctrl_reg->reset &= (~BIT_15);
    
    DISABLE_MEA_CLOCK
    DISABLE_DCT_CLOCK
}

/*****************************************************************************
 **	Name : 			VSP_ResetDCT
 ** Description:	Reset the DCT.
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of DCT module
					2. reset DCT register
 *****************************************************************************/
PUBLIC void VSP_ResetDCT(void)
{   
	//VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;
	//GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)pBase_bfr->pGlobal_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)AHB_GENERAL_CONTROL_REG_BASE;
    
    pGlobal_ctrl_reg->reset |= BIT_14;//DCT Reset
    pGlobal_ctrl_reg->reset &= (~BIT_14);
    
    DISABLE_DCT_CLOCK
}

/*****************************************************************************
 **	Name : 			VSP_ResetMEA
 ** Description:	Reset the MEA.
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of MEA
					2. reset MEA register
 *****************************************************************************/
PUBLIC void VSP_ResetMEA(void)
{   
	//VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;
	//GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)pBase_bfr->pGlobal_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)AHB_GENERAL_CONTROL_REG_BASE;
    
    pGlobal_ctrl_reg->reset |= BIT_15;//MEA Reset
    pGlobal_ctrl_reg->reset &= (~BIT_15);
    
    DISABLE_MEA_CLOCK
}

/*****************************************************************************
 **	Name : 			VSP_CloseDCT
 ** Description:	Close the DCT.
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of DCT module
 *****************************************************************************/
PUBLIC void VSP_CloseDCT(void)
{   
	//VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;
	//GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)pBase_bfr->pGlobal_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)AHB_GENERAL_CONTROL_REG_BASE;

    DISABLE_DCT_CLOCK
}

/*****************************************************************************
 **	Name : 			VSP_CloseMEA
 ** Description:	Close the MEA.
 ** Author:			Binggo Zhou
 **	Note:			1. disable clock of MEA
 *****************************************************************************/
PUBLIC void VSP_CloseMEA(void)
{   
	//VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;
	//GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)pBase_bfr->pGlobal_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)AHB_GENERAL_CONTROL_REG_BASE;
    
    DISABLE_MEA_CLOCK
}

/*****************************************************************************
 **	Name : 			VSP_InitDCTEngine
 ** Description:	Initialize the VSP DCT Engine.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_InitDCTEngine(void)
{
	//VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;	
	//GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)pBase_bfr->pGlobal_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)AHB_GENERAL_CONTROL_REG_BASE;
	
	ENABLE_DCT_CLOCK
}

/*****************************************************************************
 **	Name : 			VSP_InitMEAEngine
 ** Description:	Initialize the VSP MEA Engine.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_InitMEAEngine(void)
{
	//VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;	
	//GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)pBase_bfr->pGlobal_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)AHB_GENERAL_CONTROL_REG_BASE;
		
	pGlobal_ctrl_reg->glb_buf_sw &= (~BIT_10);
	
	ENABLE_MEA_CLOCK
}

/*****************************************************************************
 **	Name : 			VSP_InitVSPEndian
 ** Description:	Initialize the VSP endian mode.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_InitVSPEndian(VSP_ENDIAN_E endian)
{
	//VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;	
	//GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)pBase_bfr->pGlobal_reg_base;
	GLOBAL_CTRL_REG_T *pGlobal_ctrl_reg = (GLOBAL_CTRL_REG_T *)AHB_GENERAL_CONTROL_REG_BASE;
	
	if(endian)
		pGlobal_ctrl_reg->glb_end_lan |= BIT_1;//Big Endian
	else
		pGlobal_ctrl_reg->glb_end_lan &= (~BIT_1);//Little Endian
}

/*****************************************************************************
 **	Name : 			VSP_MEA_Endian_Set
 ** Description:	Initialize the MEA endian mode.
 ** Author:			Binggo Zhou
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_MEA_Endian_Set(MEA_ENDIAN_E endian)
{
	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T * )MEA_CTRL_BASE;
	
	if(endian)
		pMea_ctrl_reg->mea_core_ctrl |= (MEA_DATA_INVER << MEA_DATA_INV_BIT);//Big Endian
	else
		pMea_ctrl_reg->mea_core_ctrl &= (~(MEA_DATA_INVER << MEA_DATA_INV_BIT));//Little Endian
}

/*****************************************************************************
 **	Name : 			VSP_InitMEACtrlRegister
 ** Description:	Initialize MEA control register.
 ** Author:			Binggo Zhou
 **	Note:			Init MEA for ME and MC
 *****************************************************************************/
PUBLIC void VSP_InitMEACtrlRegister(uint32 frame_width,uint32 frame_height,
				VSP_MEARBUF_MODE_E ref_bfr_mode,int32 mv_ver_range)
{
	//MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T * ) (&s_vsp_base_bfr)->pMea_ctrl_reg_base;
	MEA_CTRL_REG_T *pMea_ctrl_reg = (MEA_CTRL_REG_T * )MEA_CTRL_BASE;

	pMea_ctrl_reg->mea_fm_cfg = (((frame_width + 15) / 16) - 1) |
							((((frame_height + 15) / 16) - 1) << MEA_Y_MAX_BIT);
	
	pMea_ctrl_reg->mea_rbuf_cfg	= mv_ver_range		|
					  ((ref_bfr_mode & 1) << MEA_RBUF_UV_BIT) |
					  (ref_bfr_mode << MEA_RBUF_MODE_BIT);

	#if defined( _THREE_STEP_SEARCH_)
	pMea_ctrl_reg->mea_cbuf_cfg = MEA_MB_CMD_NUM_TSM	|
							 (MEA_SB_CMD_NUM << MEA_SB_CMD_NUM_BIT)		|
							 (MEA_STOP_VAL << MEA_STP_VAL_BIT);	
	#elif defined(_PRED_CROSS_SEARCH_)
	pMea_ctrl_reg->mea_cbuf_cfg = MEA_MB_CMD_NUM_PCMS			|      
							 (MEA_SB_CMD_NUM << MEA_SB_CMD_NUM_BIT)		|
							 (MEA_STOP_VAL << MEA_STP_VAL_BIT);		
	#endif
	
	pMea_ctrl_reg->mea_op_cfg  = MEA_MODE_16_16 << MEA_BLK_MODE_BIT;
	
	pMea_ctrl_reg->mea_hp_scan_cfg = MEA_HP_CMD_NUM			|
								(0 << MEA_HP_CAL_BIT)	|       //not only half pixel
								(1 << MEA_HP_SCAN_EN_BIT);		//enable half pixel
	pMea_ctrl_reg->mea_core_ctrl |= (MEA_DATA_INVER << MEA_DATA_INV_BIT);//Big Endian
}

/*****************************************************************************
 **	Name : 			VSP_InitMEACommandBuffer
 ** Description:	Initialize MEA command buffer.
 ** Author:			Xiaowei Luo
 **	Note:			
 *****************************************************************************/
PUBLIC void VSP_InitMEACommandBuffer(MEA_CMD_T **ppcmd_bfr)
{
	int32 i = 0;
	int32 lev;
	int32 pos;
	int32 step = 8;
	VSP_BASE_BFR_T *pBase_bfr = &s_vsp_base_bfr;
	MEA_CMD_T **pCmd = ppcmd_bfr;
	int32 half_position[9][2] = {{-1, 1},{0, 1}, {1, 1}, {-1, 0}, {0, 0}, {1, 0}, {-1, -1}, {0, -1}, {1, -1}};

#if defined(_THREE_STEP_SEARCH_)
	int32 three_step_position[9][2] ={{0, 0}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
#elif defined(_PRED_CROSS_SEARCH_)
	int32 pcs_position[5][2] = {{0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
#endif

	/*init command buffer address*/
	for (i = 0; i < MEA_CMD_BFR_SIZE; i++)
	{
		//pCmd[i]  = (MEA_CMD_T *)(pBase_bfr->pMe_cmd_bfr_base + sizeof (MEA_CMD_T) * i);
		pCmd[i]  = (MEA_CMD_T *)(MEA_CMD_BFR_BASE + sizeof (MEA_CMD_T) * i);
	}

	/*configure full pixel command*/
#if defined(_THREE_STEP_SEARCH_) /*use three steps motion estimation*/
	/*zero point*/
	i=0;
	pCmd[i]->cmd = 0;
	
	i++;
	
	for (lev = 0; lev < 4; lev++)
	{
		for (pos = 1; pos < 9; pos++)
		{
			pCmd[i]->cmd = (0 << 24) | (lev << 16) | ( ((three_step_position[pos][1] * step * 2) << 8) & 0xff00 ) | ((three_step_position[pos][0] * step * 2)&0xff);
			
			i++;
		}
		
		step = step >> 1;
	}
#elif defined(_PRED_CROSS_SEARCH_)
	/*zero point*/
	i=0;
	pCmd[i]->cmd = 0;
	
	i++;
	pCmd[i]->cmd = 0;  //for prediction motion vector

	i++;

	for (lev = 1; lev < 11; lev++)
	{
		for (pos = 0; pos < 5; pos++)
		{
			pCmd[i]->cmd = (1 << 24) | (lev << 16) |( ((pcs_position[pos][1] * step * 2)& 0xff00) << 8 ) | ((pcs_position[pos][0] * step * 2)& 0xff);
			
			i++;
		}
		
		step = step >> 1;
	}
#endif

	/*configure half pixel command*/
	i = 56;
	for (pos = 0; pos < 9; pos++)
	{
		if (pos == 4)
			continue;

		pCmd[i]->cmd = (0 << 24) | (0xf << 16) | ((half_position[pos][1] << 8)& 0xff00) | (half_position[pos][0]& 0xff);

		i++;
	}
}


/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/**---------------------------------------------------------------------------*/