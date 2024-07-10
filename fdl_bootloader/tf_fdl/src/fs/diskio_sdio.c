/******************************************************************************
 ** File Name:      disk.c                                               *
 ** Author:           tao.feng                                              *
 ** DATE:            20/12/2010                                                *
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE               NAME             DESCRIPTION                               *
 ** 21/10/2010     tao.feng          Create.                                   *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
 
 /**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "diskio.h"
#include "scm_api.h"
#include "ff.h"
#include "card_sdio.h"

#define TF_UartTrace rprintf

LOCAL CARD_SDIO_HANDLE s_cardHandle;


/*****************************************************************************/
// 	Description : Initialize SD card.
//	Global resource dependence : 
//   Author: tao.feng
//	Note:
/*****************************************************************************/
LOCAL void diskio_sdpwrctrl(BOOLEAN on)
{
	if(TRUE == on)
	{
		CARD_SDIO_PwrCtl(s_cardHandle, TRUE);
//		TF_SDPwrCtrl(TRUE);
		SCI_Sleep(200);
	}
	else
	{
//		TF_SDPwrCtrl(FALSE);
		CARD_SDIO_PwrCtl(s_cardHandle, FALSE);
		SCI_Sleep(200);
	}
}

/*****************************************************************************/
// 	Description : Re Initialize SD card.
//	Global resource dependence : 
//   Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CARD_SDIO_Init(void)
{
	uint32 i;
  	static uint32 proc_count_HighMode = 0;
	if(CARD_SDIO_IsEnterHighSpeed(s_cardHandle))
	{
         if(proc_count_HighMode < 3 )
         {
		for(i = 0; i < 3; i++)
		{
                     proc_count_HighMode++;
			diskio_sdpwrctrl(FALSE);
			diskio_sdpwrctrl(TRUE);
			if(TRUE == CARD_SDIO_InitCard(s_cardHandle, NORMAL_SPEED_MODE))
			{
				return TRUE;
			}
		}
         }
	}	
	for(i = 0; i < 3; i++)
	{
		diskio_sdpwrctrl(FALSE);
		diskio_sdpwrctrl(TRUE);
		if(TRUE == CARD_SDIO_InitCard(s_cardHandle, NORMAL_SPEED_MODE))
		{
			return TRUE;
		}
	}	
	return FALSE;
}
// 	Description : Initialize SD card.
//	Global resource dependence : 
//   Author: tao.feng
//	Note:
/*****************************************************************************/
LOCAL BOOLEAN diskio_ErrProc(SCM_FUN_E cmd, SCM_PARAM_T* param)
{
	uint32 i;

	TF_UartTrace("diskio_ErrProc: cmd=0x%x", cmd);
	if(cmd == SCM_FUNC_REINIT)
	{
		for(i = 0; i < 3; i++)
		{
			diskio_sdpwrctrl(FALSE);
			diskio_sdpwrctrl(TRUE);
			if(TRUE == CARD_SDIO_Init())
			{
				return TRUE;
			}
		}
		return FALSE;
	}
	else if(cmd == SCM_FUNC_READ)
	{
		BOOLEAN result;
//---
		/*step1: normal read again*/
		if(1 == param->readParam.num)
		{
			result = CARD_SDIO_ReadSingleBlock(s_cardHandle, param->readParam.startBlock, param->readParam.buf);
		}
		else
		{
			result = CARD_SDIO_ReadMultiBlock(s_cardHandle, param->readParam.startBlock, param->readParam.num, param->readParam.buf);
		}
		if(TRUE == result)
		{
			return TRUE;
		}
		
		/*step2: buf read again*/
		result = CARD_SDIO_ReadMultiBlock(s_cardHandle, param->readParam.startBlock, param->readParam.num, param->readParam.buf);
		if(TRUE == result)
		{
			return TRUE;
		}
		
		/*step3: all singel read again*/
		for(i = 0; i < param->readParam.num; i++)
		{
			if(FALSE == CARD_SDIO_ReadSingleBlock(s_cardHandle, param->readParam.startBlock+i, 512*i+param->readParam.buf))
			{
				break;
			}
		}
		if(i == param->readParam.num)
		{
			return TRUE;
		}
		/*step4: reinit then normal read again*/
		diskio_sdpwrctrl(FALSE);
		diskio_sdpwrctrl(TRUE);
		if(TRUE == CARD_SDIO_Init())
		{
			if(1 == param->readParam.num)
			{
				result = CARD_SDIO_ReadSingleBlock(s_cardHandle, param->readParam.startBlock, param->readParam.buf);
			}
			else
			{
				result = CARD_SDIO_ReadMultiBlock(s_cardHandle, param->readParam.startBlock, param->readParam.num, param->readParam.buf);
			}
			if(TRUE == result)
			{
				return TRUE;
			}
		}
		/*step5: reinit then buf read again*/
		diskio_sdpwrctrl(FALSE);
		diskio_sdpwrctrl(TRUE);
		if(TRUE == CARD_SDIO_Init())
		{
			result = CARD_SDIO_ReadMultiBlock(s_cardHandle, param->readParam.startBlock, param->readParam.num, param->readParam.buf);
			if(TRUE == result)
			{
				return TRUE;
			}
		}
		/*step6: reinit then all single read again*/
		diskio_sdpwrctrl(FALSE);
		diskio_sdpwrctrl(TRUE);
		if(TRUE == CARD_SDIO_Init())
		{
			for(i = 0; i < param->readParam.num; i++)
			{
				if(FALSE == CARD_SDIO_ReadSingleBlock(s_cardHandle, param->readParam.startBlock+i, 512*i+param->readParam.buf))
				{
					return FALSE;
				}
			}
			return TRUE;
		}
		return FALSE;
//---
	}
	else if(cmd == SCM_FUNC_WRITE)
	{
		BOOLEAN result;
//---
		if(1 == param->writeParam.num)
		{
			result = CARD_SDIO_WriteSingleBlock(s_cardHandle, param->writeParam.startBlock, param->writeParam.buf);
		}
		else
		{
			result = CARD_SDIO_WriteMultiBlock(s_cardHandle, param->writeParam.startBlock, param->writeParam.num, param->writeParam.buf);
		}
		if(TRUE == result)
		{
			return TRUE;
		}

		for(i = 0; i < 3; i++)
		{
			diskio_sdpwrctrl(FALSE);
			diskio_sdpwrctrl(TRUE);
			if(TRUE == CARD_SDIO_Init())
			{
				if(1 == param->writeParam.num)
				{
					result = CARD_SDIO_WriteSingleBlock(s_cardHandle, param->writeParam.startBlock, param->writeParam.buf);
				}
				else
				{
					result = CARD_SDIO_WriteMultiBlock(s_cardHandle, param->writeParam.startBlock, param->writeParam.num, param->writeParam.buf);
				}
				if(TRUE == result)
				{
					return TRUE;
				}
			}
		}
		return FALSE;
//---
	}
	else if(cmd == SCM_FUNC_ERASE)
	{
		return FALSE;
	}
	else if(cmd == SCM_FUNC_GETCAPCITY)
	{
		return FALSE;
	}
	else
	{
		return FALSE;  /*lint !e527 */
	}
}

/*****************************************************************************/
// 	Description : Initialize SD card.
//	Global resource dependence : 
//   Author: tao.feng
//	Note:
/*****************************************************************************/
DSTATUS disk_initialize (
	BYTE drv		/* Physical drive nmuber (0) */
)
{
      DRESULT ret = RES_OK;

	if(0 == drv)
	{
		s_cardHandle = CARD_SDIO_Open(CARD_SDIO_SLOT_0);
	}
	else
	{
		s_cardHandle = CARD_SDIO_Open(CARD_SDIO_SLOT_1);		
	}
	TF_UartTrace("disk_initialize: s_cardHandle=0x%x\n", s_cardHandle);

	if(0 == s_cardHandle)
	{
		ret = RES_ERROR;
	}

	diskio_sdpwrctrl(TRUE);

	if(FALSE == CARD_SDIO_InitCard(s_cardHandle, NORMAL_SPEED_MODE))
	{
		if(!diskio_ErrProc(SCM_FUNC_REINIT,  NULL))
		{
			TF_UartTrace("disk_initialize: err process\n");
			ret = RES_ERROR;
		}
	}

      TF_UartTrace("disk_initialize: end: ret=%d\n", ret);

       return ret;
}



/*****************************************************************************/
// 	Description : disk status.
//	Global resource dependence : 
//   Author: tao.feng
//	Note:
/*****************************************************************************/
DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0) */
)
{    
    return RES_OK;    
}


/*****************************************************************************/
// 	Description : disk read.
//	Global resource dependence : 
//   Author: tao.feng
//	Note:
/*****************************************************************************/
DRESULT disk_read (
	BYTE drv,			/* Physical drive nmuber (0) */
	BYTE *buff,			/* Pointer to the data buffer to store read data */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{    
	DRESULT ret = RES_ERROR;
	SCM_PARAM_T  param;

	if(1 == count)
	{
		if(TRUE == CARD_SDIO_ReadSingleBlock(s_cardHandle, sector, buff))
		{
			ret = RES_OK;
		}
	}
	else
	{
		if(TRUE == CARD_SDIO_ReadMultiBlock(s_cardHandle, sector, count, buff))
		{
			ret = RES_OK;
		}
	}
      //TF_UartTrace("disk_read: : ret=%d\n", ret);

	if(ret != RES_OK)
	{
		param.readParam.startBlock = sector;
		param.readParam.buf = buff;
		param.readParam.num = count;
		if(!diskio_ErrProc(SCM_FUNC_READ,  &param))
		{
			ret = RES_ERROR;
		}
		else
		{
			ret = RES_OK;
		}
	}
      //TF_UartTrace("disk_read: 11 : ret=%d\n", ret);

	return ret;
}


/*****************************************************************************/
// 	Description : disk write.
//	Global resource dependence : 
//   Author: tao.feng
//	Note:
/*****************************************************************************/
#if	_READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0) */
	const BYTE *buff,	/* Pointer to the data to be written */
	DWORD sector,		/* Start sector number (LBA) */
	BYTE count			/* Sector count (1..255) */
)
{
	DRESULT ret = RES_ERROR;
	SCM_PARAM_T  param;

	if(1 == count)
	{
		if(TRUE == CARD_SDIO_WriteSingleBlock(s_cardHandle, sector, (uint8*)buff))
		{
			ret = RES_OK;
		}
	}
	else
	{
		if(TRUE == CARD_SDIO_WriteMultiBlock(s_cardHandle, sector, count, (uint8*)buff))
		{
			ret = RES_OK;
		}
	}

	if(ret != RES_OK)
	{
		param.readParam.startBlock = sector;
		param.readParam.buf = (BYTE *)buff;
		param.readParam.num = count;
		if(!diskio_ErrProc(SCM_FUNC_WRITE,  &param))
		{
			ret = RES_ERROR;
		}
		else
		{
			ret = RES_OK;
		}
	}

	return ret;
}
#endif

/*****************************************************************************/
// 	Description : disk ioctl.
//	Global resource dependence : 
//   Author: tao.feng
//	Note:
/*****************************************************************************/
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	if(NULL == buff)
	{
		return RES_PARERR;
	}

    switch (ctrl) 
    {
    case GET_CARD_CAPCITY:
        *(DWORD *)buff = CARD_SDIO_GetCapacity(s_cardHandle);
        if(0x0 == *(DWORD *)buff)
        {
            return RES_ERROR;
        }
        break;
        
	default:
	    break;
	}
        
    return RES_OK;    
}


