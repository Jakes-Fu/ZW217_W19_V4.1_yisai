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
#include "ubot_scsi_spc.h"
#include "ubot_drv.h"
#include "ubot_storage.h"
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
#define USB_SPC_INQUIRE_HEAD_LEN  8
#define USB_SPC_REQUEST_SENSE_LEN 18
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/
LOCAL const uint8 s_UBOT_SPC_InquireHead [USB_SPC_INQUIRE_HEAD_LEN] =
{
    0x00,0x80,0x00,0x01,0x1f,0x00,0x00,0x00
};

LOCAL const uint8 s_UBOT_SPC_RequestSense [USB_SPC_REQUEST_SENSE_LEN] =
{
    0xf0,0x01,0x06,0x00,0x00,0x00,0x00,0x0a,
    0x00,0xaa,0x55,0x41,0x28,0x00,0x41,0x00,
    0x00,0x00
};

//LOCAL uint8 s_UBOT_FormatCapacityLock = 0;
/*--------------------------- Global Data -----------------------------------*/
/*--------------------------- External Data ---------------------------------*/
extern uint8 *g_UBOT_CBD_Response;
extern UBOT_MANAGER_T g_UBOT_manager ;
extern uint32 cpu_to_be32(uint32 dword);
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcInquiry (UBOT_CBD_U *pCBD)
{
    char * head   = (char *)&s_UBOT_SPC_InquireHead ;
    char * vendor = (char *)\
        g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.vendor;
    char * product= (char *)
        g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.product;
    uint32 len    =   USB_SPC_INQUIRE_HEAD_LEN +
                      UBOT_SCSI_VENDER_STR_LEN +
                      UBOT_SCSI_PRODUCT_ID_STR_LEN ;
                      
    SCI_MEMCPY ((char *)g_UBOT_CBD_Response,\
            head,\
            USB_SPC_INQUIRE_HEAD_LEN);

    SCI_MEMCPY ((char *)(g_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN),\
            vendor,\
            UBOT_SCSI_VENDER_STR_LEN);

    SCI_MEMCPY ((char *)(g_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN \
                                         + UBOT_SCSI_VENDER_STR_LEN),\
            product,\
            UBOT_SCSI_PRODUCT_ID_STR_LEN);

	*(char *)(g_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN\
                                  + UBOT_SCSI_VENDER_STR_LEN\
						          + UBOT_SCSI_PRODUCT_ID_STR_LEN) = NULL;
    //Send data to cache;
    UBOT_WriteCache ((uint8 *)g_UBOT_CBD_Response,len);
    
    return UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcTestUintReady (UBOT_CBD_U *pCBD)
{
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
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcRequestSense (UBOT_CBD_U *pCBD)
{
    char * response   = (char *)&s_UBOT_SPC_RequestSense ;
    
    SCI_MEMCPY ((char *)g_UBOT_CBD_Response,\
            response,\
            USB_SPC_REQUEST_SENSE_LEN);

    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        g_UBOT_CBD_Response[2]  = 0x6;
        g_UBOT_CBD_Response[12] = 0x28;
        g_UBOT_CBD_Response[13] = 0;
    }
    else{
        g_UBOT_CBD_Response[2]  = 0x2;
        g_UBOT_CBD_Response[12] = 0x3a;
        g_UBOT_CBD_Response[13] = 0;
    }
    
	if (0x08 == g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength ){
        g_UBOT_CBD_Response[2]  = 0x5;
        g_UBOT_CBD_Response[12] = 0x3a;
        g_UBOT_CBD_Response[13] = 0;
	}

    //Send data to cache;
    UBOT_WriteCache ((uint8 *)g_UBOT_CBD_Response,\
    				g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength);

    //s_UBOT_FormatCapacityLock = TRUE ;

    return UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcReadTpa (UBOT_CBD_U *pCBD)
{
    return UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiReadFormatCapacites (UBOT_CBD_U *pCBD)
{
    uint32 page_num = \
        g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_number;
    uint32 page_size = \
        g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].inf.page_size;

    
    if(UBOT_DEV_PRESENT != \
                  UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }
    else{
	    UBOT_DevCompaction(g_UBOT_manager.cbw.mCBW.bCBWLUN);
    }
    
    *(uint32 * ) g_UBOT_CBD_Response       =cpu_to_be32( 0x00000010);
    *(uint32 * )(g_UBOT_CBD_Response + 4)  =cpu_to_be32( page_num);
    *(uint32 * )(g_UBOT_CBD_Response + 8)  = cpu_to_be32(page_size | 0x02000000);
    *(uint32 * )(g_UBOT_CBD_Response + 12) =cpu_to_be32( page_num );
    *(uint32 * )(g_UBOT_CBD_Response + 16) =cpu_to_be32( page_size) ;  
 
    UBOT_WriteCache ((uint8 *)g_UBOT_CBD_Response,20);
    //s_UBOT_FormatCapacityLock = FALSE ;
    return UBOT_CBD_SUCESS;
}
/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSpcModeSense6 (UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }
    else{
        *(uint32 * ) g_UBOT_CBD_Response      =cpu_to_be32( 0x03000000) ;
        //Send data to cache;
        UBOT_WriteCache ((uint8 *)g_UBOT_CBD_Response,4);
        UBOT_DevCompaction(g_UBOT_manager.cbw.mCBW.bCBWLUN);
        return UBOT_CBD_SUCESS;
    }
}

/*****************************************************************************/
//  Description:    
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ScsiSbcModeSense10(UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UBOT_DEV_PRESENT != \
                    UBOT_CheckDevState(g_UBOT_manager.cbw.mCBW.bCBWLUN)){
        UBOT_ResetCache();
        return UBOT_CBD_NO_DEVICE;
    }
    else{
        *(uint32 * ) g_UBOT_CBD_Response           = cpu_to_be32(0x00060000) ;
        *(uint32 * ) (g_UBOT_CBD_Response  + 4 )   = cpu_to_be32(0x00000000);	 	
        //Send data to cache;
        UBOT_WriteCache ((uint8 *)g_UBOT_CBD_Response,8);
        UBOT_DevCompaction(g_UBOT_manager.cbw.mCBW.bCBWLUN);
        return UBOT_CBD_SUCESS;
    }
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
