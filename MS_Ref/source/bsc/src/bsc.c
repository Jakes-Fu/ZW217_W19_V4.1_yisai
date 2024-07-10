/******************************************************************************
 ** File Name:      bsc.c                                                    *
 ** Author:                                                     *
 ** DATE:                                                          *
 ** Copyright:      2002 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file defines the basic operation interfaces of       *
 **                 bsc data protection. It manages important data verify,        *
 **			 write and read.                                *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           	NAME             	DESCRIPTION                               *
 ** 										                                     *
 ******************************************************************************/
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "bsc.h"
#include "bsc_device.h"
#include "bsc_debug.h"
#include "xsr_partition.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif

/******************************************************************************
 * Static Global Variables
 ******************************************************************************/
LOCAL const char *MUTEX_NAME = "BSC MANAGER MUTEX"; 
LOCAL SCI_MUTEX_PTR g_pMutex = SCI_NULL;
LOCAL BOOLEAN g_bOpenFlag = FALSE;

#define ACQUIRE_MUTEX(c) \
BSC_ASSERT(SCI_NULL != g_pMutex); /*assert verified*/\
if (SCI_SUCCESS != SCI_GetMutex(g_pMutex, \
		SCI_INVALID_BLOCK_ID != SCI_IdentifyThread() ? SCI_WAIT_FOREVER : 0)) { \
	BSC_ERR_TRACE(("[NV:%s] SCI_GetMutex failed\r\n", #c)); \
	{\
		SCI_ASSERT(0);\
	return BSCERR_SYSTEM; \
	}\
} 

#define RELEASE_MUTEX \
BSC_VERIFY(SCI_SUCCESS == SCI_PutMutex(g_pMutex));   /*assert verified*/

#define CHECK_OPENFLAG(c) \
if (!g_bOpenFlag) { \
	BSC_ERR_TRACE(("[NV:%s] NVM is not opened\r\n", #c)); \
	return BSCERR_NOT_OPENED; \
}

#define CHECK_ID(id) \
if (((id) < BSC_MIN_ID) || ((id) > BSC_MAX_ID)) { \
	return BSCERR_INVALID_PARAM; \
}
/**---------------------------------------------------------------------------*
 **                     Local Function Prototypes                             *
 **---------------------------------------------------------------------------*/
 
 /**---------------------------------------------------------------------------*
 **                         Function Definitions                              *
 **---------------------------------------------------------------------------*/
 /*****************************************************************************/
// Description :    BSC Init
// PUBLIC resource dependence : 
// Author :         
// Note :
/*****************************************************************************/
PUBLIC BSC_ERROR_E BSC_Init()
{
	BSC_ERROR_E 	err = BSCERR_NONE;

	BSC_ASSERT(SCI_NULL == g_pMutex);/*assert verified*/
	BSC_ASSERT(!g_bOpenFlag);/*assert verified*/
	
	do {
		g_pMutex = SCI_CreateMutex(MUTEX_NAME, SCI_INHERIT);
		if (SCI_NULL == g_pMutex) {
			BSC_ERR_TRACE(("[BSC:BSC_Init] SCI_CreateMutex failed\r\n"));
			err = BSCERR_SYSTEM;
			break;
		}
	
		err = BSCD_Open();
		if (BSCERR_NONE == err) {
			/* BSC device is opened successfully.*/
			g_bOpenFlag =TRUE;
			BSC_LOG_TRACE(("[BSC:BSC_Init] BSC initialized\r\n"));
			return BSCERR_NONE;

		} else {
			BSCD_Close();
			BSC_ERR_TRACE(("[BSC:BSC_Init] BSCD_Open failed(0x%x)\r\n", err));
			break;
		}
		
	} while (0);
	
	if (SCI_NULL != g_pMutex) {
		BSC_VERIFY(SCI_SUCCESS == SCI_DeleteMutex(g_pMutex));   /*assert verified*/
		g_pMutex = SCI_NULL;
	}

	return err;	
}

PUBLIC uint32 BSC_GetHashLength(void)	//Added this api to get Hash Length
{
	return BSCD_GetHashLength();
}
 /*****************************************************************************/
// Description :    Read data from Bsc.
// PUBLIC resource dependence : 
// Author :
// Note :
/*****************************************************************************/
PUBLIC BSC_ERROR_E BSC_Read(INPUT uint16 ItemID, INPUT uint16 cchSize, 
	OUTPUT void *pBuf)
{
	BSC_ERROR_E ret;
	BOOLEAN		   valid = FALSE;	
	
	BSC_ASSERT(NULL != pBuf);/*assert verified*/
	BSC_ASSERT(cchSize > 0);/*assert verified*/
	if ((NULL == pBuf) || (0 == cchSize))/*lint !e774*/
		return BSCERR_INVALID_PARAM;
		
	CHECK_OPENFLAG(BSC_Read);
	CHECK_ID(ItemID);
	ACQUIRE_MUTEX(BSC_Read);
	ret = BSCD_Read(ItemID,cchSize, pBuf);
	RELEASE_MUTEX;
	return ret;
}

 /*****************************************************************************/
// Description :    Write data to Bsc.
// PUBLIC resource dependence : 
// Author :
// Note :
/*****************************************************************************/
PUBLIC BSC_ERROR_E BSC_Write(INPUT uint16 ItemID, INPUT uint16 cchSize,  
	INPUT const void *pBuf)
{
	BSC_ERROR_E ret;
	
	BSC_ASSERT(NULL != pBuf);/*assert verified*/
	BSC_ASSERT(cchSize > 0);/*assert verified*/
	if ((NULL == pBuf) || (0 == cchSize))/*lint !e774*/
		return BSCERR_INVALID_PARAM;
		
	CHECK_OPENFLAG(BSC_Write);
	CHECK_ID(ItemID);
	
	ACQUIRE_MUTEX(BSC_Write);
	ret = BSCD_Write(ItemID,cchSize, pBuf);
	RELEASE_MUTEX;
	return ret;
}

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //

