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
#include "ucom_ubot_scsi_sbc.h"
#include "ucom_ubot_storage.h"
#include "ucom_drv.h"
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
uint8 *g_UCOM_UBOT_CBD_Response = NULL;// [UCOM_UBOT_MAX_ALLOC_LEN]; 

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
static  uint32 cpu_to_be32(uint32 dword)
{
    #ifndef _BIG_ENDIAN
	return ((((dword)<<24)&0xff000000L)|(((dword)<<8)&0x00ff0000L)|(((dword)>>8)&0x0000ff00L)|(((dword)>>24)&0x000000ffL));
    #else
	return dword;	
    #endif
}

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcRead10 (UCOM_UBOT_CBD_U *pCBD)
{
    uint32 data_len  = g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength;
    
    UCOM_UBOT_ResetCache(); 
    
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT !=   UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN))
    {
        UCOM_UBOT_ResetCache();

        return UCOM_UBOT_CBD_NO_DEVICE;
    }

    if(! UCOM_UBOT_ReadDataFromBSD (data_len))
    {

        return UCOM_UBOT_CBD_PARAMER_ERR;
     }
    return UCOM_UBOT_CBD_SUCESS;
}


/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcWrite10 (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT !=   UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }

    return UCOM_UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcVerify10 (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT !=  UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
	    UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
    }

    return UCOM_UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcStartStop (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
	    UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
    }

    return UCOM_UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcMediaRemove (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
	    UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
    }

    return UCOM_UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSbcReadCapacity (UCOM_UBOT_CBD_U *pCBD)
{
    uint32 page_num =   g_UCOM_manager.ubot_dev[g_UCOM_manager.cbw.mCBW.bCBWLUN].inf.page_number;
//    uint32 page_size =  g_UCOM_manager.ubot_dev[g_UCOM_manager.cbw.mCBW.bCBWLUN].inf.page_size;
    
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT !=   UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
	    UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
    }

#ifdef USB_VCOM_AUTORUN_SUPPORT   //CDROM, 1 block=2048B
    *(uint32 * ) g_UCOM_UBOT_CBD_Response      = cpu_to_be32(page_num - 1);
    *(uint32 * )(g_UCOM_UBOT_CBD_Response + 4) = cpu_to_be32(2048);//page_size*4 ;  //2048
    
#else
    *(uint32 * ) g_UCOM_UBOT_CBD_Response      = cpu_to_be32(page_num - 1);
    *(uint32 * )(g_UCOM_UBOT_CBD_Response + 4) = cpu_to_be32(page_size) ;
#endif

    //Send data to cache;
    UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,8);

    return UCOM_UBOT_CBD_SUCESS;
}

PUBLIC BOOLEAN    ucom_ubot_config_init()
{

    if(g_UCOM_UBOT_CBD_Response == NULL)
    {
        g_UCOM_UBOT_CBD_Response = SCI_ALLOC(UCOM_UBOT_MAX_ALLOC_LEN);
    }
    if(g_UCOM_UBOT_CBD_Response == NULL)
    {
        return SCI_FALSE;
    }
    SCI_MEMSET(g_UCOM_UBOT_CBD_Response,0,UCOM_UBOT_MAX_ALLOC_LEN);
    return SCI_TRUE;
}
PUBLIC void    ucom_ubot_config_exit()
{

    if(NULL != g_UCOM_UBOT_CBD_Response)
    {
        SCI_FREE(g_UCOM_UBOT_CBD_Response);
    }
    g_UCOM_UBOT_CBD_Response = NULL; 
}



/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
