/******************************************************************************
 ** File Name:    model.c                                                     *
 ** Author:       Daniel.Ding                                                 *
 ** DATE:         3/25/2005                                                   *
 ** Copyright:    2005 Spreatrum, Incoporated. All Rights Reserved.           *
 ** Description:                                                              *
 *****************************************************************************/
/******************************************************************************
 **                   Edit    History                                         *
 **---------------------------------------------------------------------------* 
 ** DATE          NAME            DESCRIPTION                                 * 
 ** 3/25/2005     Daniel.Ding     Create.                                     *
 *****************************************************************************/
/*----------------------------------------------------------------------------*
**                        Dependencies                                        *
**---------------------------------------------------------------------------*/
#include "ubot_scsi_sbc.h"
#include "ubot_storage.h"
#include "ubot_drv.h"
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

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/
/*--------------------------- Global Data -----------------------------------*/
uint8 *g_UBOT_CBD_Response = NULL; //[UBOT_MAX_ALLOC_LEN];
/*--------------------------- External Data ---------------------------------*/
extern UBOT_MANAGER_T g_UBOT_manager ;
extern uint32 cpu_to_be32(uint32 dword);
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Function to implete scsi read10 request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC UBOT_CBD_ERR_E UBOT_ScsiSbcRead10 (UBOT_CBD_U *pCBD)
{
    uint32 data_len  = g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength;
    
    UBOT_ResetCache(); 
    
    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }

    if(! UBOT_ReadDataFromBSD (data_len))
    {
        return UBOT_CBD_PARAMER_ERR;
     }
    return UBOT_CBD_SUCESS;
}


/*****************************************************************************/
//  Description:    Function to implete scsi sbcwrite10 request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC UBOT_CBD_ERR_E UBOT_ScsiSbcWrite10 (UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }

    return UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    Function to implete scsi sbcverify10 request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSbcVerify10 (UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }
    else{
	    UBOT_DevCompaction(g_UBOT_manager.cbw.mCBW.bCBWLUN);
    }

    return UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    Function to implete scsi sbc start/stop request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSbcStartStop (UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }
    else{
	    UBOT_DevCompaction(g_UBOT_manager.cbw.mCBW.bCBWLUN);
    }

    return UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    Function to implete scsi sbc media remove request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSbcMediaRemove (UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }
    else{
	    UBOT_DevCompaction(g_UBOT_manager.cbw.mCBW.bCBWLUN);
    }

    return UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    Function to implete scsi sbc read capacity request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSbcReadCapacity (UBOT_CBD_U *pCBD)
{
    uint32 page_num = \
        g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_number;
    uint32 page_size = \
        g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size;
    
    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }
    else{
	    UBOT_DevCompaction(g_UBOT_manager.cbw.mCBW.bCBWLUN);
    }
    
    *(uint32 * ) g_UBOT_CBD_Response      = cpu_to_be32(page_num - 1);
    *(uint32 * )(g_UBOT_CBD_Response + 4) = cpu_to_be32(page_size) ;
    //Send data to cache;
    UBOT_WriteCache ((uint8 *)g_UBOT_CBD_Response,8);

    return UBOT_CBD_SUCESS;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
