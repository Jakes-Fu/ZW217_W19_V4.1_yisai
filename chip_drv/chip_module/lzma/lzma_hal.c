/******************************************************************************
 ** File Name:      lzma_hal.c                                                 *
 ** Author:         changde                                              *
 ** DATE:           09/19/2011                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 Analog to Digital Module.                                  *
 **                                                                            *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/19/2011     changde          Create.                                   *
 **                                                                           *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"

#include "lzma_hal.h"
#include "lzma_phy.h"
#include "mmu_drvapi.h"

/**---------------------------------------------------------------------------*
 **                         Macro Define                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Variable Define                                   *
 **---------------------------------------------------------------------------*/


/**---------------------------------------------------------------------------*
 **                         Local Define                                      *
 **---------------------------------------------------------------------------*/
PUBLIC __align(32) uint32 s_lzma_hal_src_buf[LZMA_UNZIP_BUF_LEN>>2] = {0};
/*lint -esym(552, g_lzma_decompress_err_cnt) */
PUBLIC uint32 g_lzma_decompress_err_cnt = 0;  

PUBLIC void LZMA_HAL_Init(void)
{
//#if defined(CHIP_DRV_SC6531) || defined(CHIP_DRV_SC6531EFM) || defined(CHIP_DRV_UIX8910)
#if 0
    LZMA_InitExtAll();
#else
	LZMA_Init();
#endif
}

PUBLIC void LZMA_HAL_RegCallback( uint32 chan_id, LZMA_CallBack callback )
{
	LZMA_RegCallback(chan_id, callback);
}

PUBLIC uint32 LZMA_HAL_GetUnZipStatus(void)
{
    return LZMA_GetUnZipRawStatus(); 
}

/***************************************************************************** 
 *  Description: LZMA_HAL_UnZip 
 *  Author:                                                                  * 
 *  Note:                                                                    * 
******************************************************************************/
uint32 LZMA_HAL_UnZip(LZMA_OUT_T* unzip_out_ptr, LZMA_CFG_T* lzma_cfg_ptr)
{
	SCI_ASSERT( unzip_out_ptr != PNULL); /*assert to do*/
	SCI_ASSERT( lzma_cfg_ptr  != PNULL); /*assert to do*/	

    LZMA_SetSrcCfg(lzma_cfg_ptr->src_addr, lzma_cfg_ptr->src_len);
    LZMA_SetDstCfg(lzma_cfg_ptr->des_addr, lzma_cfg_ptr->des_len); 
    LZMA_SetBufCfg(lzma_cfg_ptr->cache_buf_addr, lzma_cfg_ptr->cache_buf_len);
    LZMA_SetBufStart(lzma_cfg_ptr->cache_buf_offset);

    if(lzma_cfg_ptr->work_mode == LZMA_UNZIP_NORAML)
    {
		_LZMA_BufferEn(SCI_FALSE);
    }
	else
    {
		_LZMA_BufferEn(SCI_TRUE);
    	if(lzma_cfg_ptr->work_mode == LZMA_UNZIP_AUTO_STOP)
    	{
    		_LZMA_BufferFull(SCI_TRUE);
		}
		else if(lzma_cfg_ptr->work_mode == LZMA_UNZIP_FORCE_STOP)
    	{
    		_LZMA_BufferFull(SCI_FALSE);
		}
		else
			SCI_ASSERT(0); /*assert to do*/
	}

	// async or sync mode
	if(lzma_cfg_ptr->opt_mode == UNZIP_ASYNC_MODE)
    {
		//
        LZMA_RegCallback(LZMA_INT_FINISH, lzma_cfg_ptr->callback_func);
		
		LZMA_ClrAllIntsts();
		LZMA_IntEn(LZMA_INT_FINISH, SCI_TRUE);
    	LZMA_StartRun();
	}
	else
	{
        LZMA_ClrAllIntsts();
    	LZMA_StartRun();
		LZMA_WaitingFinish(0);
        unzip_out_ptr->real_size = LZMA_GetUnzipSize();
	}

    if(LZMA_GetUnZipRawStatus())
        return SCI_FALSE;
    else
        return SCI_TRUE;
}

uint32 LZMA_HAL_UnZip_U1(LZMA_OUT_T* unzip_out_ptr, LZMA_CFG_T* lzma_cfg_ptr)
{
    LZMA_CFG_T lzma_cfg = {0};
    
	SCI_ASSERT( unzip_out_ptr != PNULL); /*assert to do*/
	SCI_ASSERT( lzma_cfg_ptr  != PNULL); /*assert to do*/	

    SCI_MEMCPY((uint8*)&lzma_cfg, (uint8*)lzma_cfg_ptr, sizeof(LZMA_CFG_T)); /*lint !e718 !e746*/
/*lint -esym(628, SPIFLASH_IsOldUNID) */ 
    if(1)//(SPIFLASH_IsOldUNID()) && (0x1C == (lzma_cfg.src_addr & 0x1F))) /*lint !e718 !e746*/
    {
        g_lzma_decompress_err_cnt++;
        if(LZMA_UNZIP_BUF_LEN < lzma_cfg.src_len)
		{    
            LZMA_HAL_UnZip(unzip_out_ptr, &lzma_cfg);
            if((LZMA_GetUnZipRawStatus()) || (SCI_SUCCESS != LZMA_WaitingFinish(1)))
            {
                g_lzma_decompress_err_cnt = 0x1FFFFFFF;
                SCI_PASSERT(0, ("LZMA module Error1")); /*assert to do*/
    			return SCI_FALSE;    /*lint !e527*/
            }

		    return SCI_TRUE;
		}
		
		LZMA_ClrUnzipRawStatus();
		
        SCI_MEMCPY((uint8*)s_lzma_hal_src_buf, (uint8*)lzma_cfg.src_addr, lzma_cfg.src_len);
        lzma_cfg.src_addr = (uint32)s_lzma_hal_src_buf;

        MMU_DmaCacheSync((uint32)s_lzma_hal_src_buf, ((lzma_cfg.src_len+31)& 0xFFFFFFE0), 0);
        
        LZMA_HAL_UnZip(unzip_out_ptr, &lzma_cfg);

        if((LZMA_GetUnZipRawStatus()) || (SCI_SUCCESS != LZMA_WaitingFinish(1)))
        {
            g_lzma_decompress_err_cnt = 0xFFFFFFFF;
            SCI_PASSERT(0, ("LZMA module Error")); /*assert to do*/
			return SCI_FALSE;  /*lint !e527*/
        }
    }
    else
    {
        LZMA_HAL_UnZip(unzip_out_ptr, &lzma_cfg);
    }

    return SCI_TRUE;
}

//#if defined(CHIP_DRV_SC6531) || defined(CHIP_DRV_SC6531EFM) || defined(CHIP_DRV_UIX8910)
#if 0
uint32 LZMA_HAL_UnZipExt(uint32 id, LZMA_OUT_T* unzip_out_ptr, LZMA_CFG_T* lzma_cfg_ptr)
{
	SCI_ASSERT( unzip_out_ptr != PNULL); /*assert to do*/
	SCI_ASSERT( lzma_cfg_ptr  != PNULL); /*assert to do*/	

    LZMA_SetSrcCfgExt(id, lzma_cfg_ptr->src_addr, lzma_cfg_ptr->src_len);
    LZMA_SetDstCfgExt(id, lzma_cfg_ptr->des_addr, lzma_cfg_ptr->des_len); 
    LZMA_SetBufCfgExt(id, lzma_cfg_ptr->cache_buf_addr, lzma_cfg_ptr->cache_buf_len);
    LZMA_SetBufStartExt(id, lzma_cfg_ptr->cache_buf_offset);

    if(lzma_cfg_ptr->work_mode == LZMA_UNZIP_NORAML)
    {
		LZMA_BufferEnExt(id, SCI_FALSE);
    }
	else
    {
		LZMA_BufferEnExt(id, SCI_TRUE);
    	if(lzma_cfg_ptr->work_mode == LZMA_UNZIP_AUTO_STOP)
    	{
    		LZMA_BufferFullExt(id, SCI_TRUE);
		}
		else if(lzma_cfg_ptr->work_mode == LZMA_UNZIP_FORCE_STOP)
    	{
    		LZMA_BufferFullExt(id, SCI_FALSE);
		}
		else
			SCI_ASSERT(0); /*assert to do*/
	}

	// async or sync mode
	if(lzma_cfg_ptr->opt_mode == UNZIP_ASYNC_MODE)
    {
		//
        LZMA_RegCallbackExt(id, LZMA_INT_FINISH, lzma_cfg_ptr->callback_func);
		
		LZMA_ClrAllIntstsExt(id);
		LZMA_IntEnExt(id, LZMA_INT_FINISH, SCI_TRUE);
    	LZMA_StartRunExt(id);
	}
	else
	{
        LZMA_ClrAllIntstsExt(id);
    	LZMA_StartRunExt(id);
		LZMA_WaitingFinishExt(id, UNZIP_TIMEOUT_MAX);
        unzip_out_ptr->real_size = LZMA_GetUnzipSizeExt(id);
	}

    if(LZMA_GetUnzipStatusExt(id))
        return SCI_FALSE;
    else
        return SCI_TRUE;
}

#endif
