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
#include "ucom_ubot_scsi_spc.h"
#include "ucom_drv.h"
#include "ucom_ubot_storage.h"
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

#ifdef USB_VCOM_AUTORUN_SUPPORT 
#define USB_SPC_MODE_SENSE10_LEN		   32
#define USB_SPC_READ_TPA_LEN               12
#define USB_SPC_READ_TPA_ALL_LEN           20
#define USB_MMC_REPORT_KEY_LEN             8
#define USB_MMC_GET_CONFIGURATION_RT0_LEN  32
#define USB_MMC_GET_CONFIGURATION_RT1_LEN  16
#define USB_MMC_GET_CONFIGURATION_RT2_CDREAD_LEN       12
#define USB_MMC_GET_CONFIGURATION_RT2_PROFILELIST_LEN  16
#define USB_MMC_GET_CONFIGURATION_RT2_LEN  8
#define USB_MMC_READ_SUB_CHANNEL_LEN       16
#define USB_MMC_READ_DISK_INFORMATION_LEN  36
#define USB_MMC_GET_PERFORMANCE            24
#endif

static  uint32 cpu_to_be32(uint32 dword)
{
    #ifndef _BIG_ENDIAN
	return ((((dword)<<24)&0xff000000L)|(((dword)<<8)&0x00ff0000L)|(((dword)>>8)&0x0000ff00L)|(((dword)>>24)&0x000000ffL));
    #else
	return dword;	
    #endif
}
/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/
LOCAL uint8 s_UCOM_UBOT_SPC_InquireHead [USB_SPC_INQUIRE_HEAD_LEN] =
{
#ifdef USB_VCOM_AUTORUN_SUPPORT 
    0x05,0x80,0x02,0x02,0x1f,0x00,0x00,0x00
#else
    0x00,0x80,0x00,0x01,0x1f,0x00,0x00,0x00
#endif
};

LOCAL uint8 s_UCOM_UBOT_SPC_RequestSense [USB_SPC_REQUEST_SENSE_LEN] =
{
    0xf0,0x01,0x06,0x00,0x00,0x00,0x00,0x0a,
    0x00,0xaa,0x55,0x41,0x28,0x00,0x41,0x00,
    0x00,0x00
};

#ifdef USB_VCOM_AUTORUN_SUPPORT 
LOCAL uint8 s_UCOM_UBOT_SPC_Inquire_VPD [USB_SPC_INQUIRE_HEAD_LEN] =
{
    0x05,0x80,0x00,0x04,0x31,0x32,0x33,0x00
};

LOCAL uint8 s_UCOM_UBOT_SPC_ModeSense10 [USB_SPC_MODE_SENSE10_LEN] =
{
    0x00,0x20,0x02,0x00,0x00,0x00,0x00,0x00,
    0x2a,0x18,0x07,0x00,0x71,0x63,0x29,0x23,  
    0x21,0x13,0x00,0xff,0x00,0x78,0x21,0x13, 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

LOCAL uint8 s_UCOM_UBOT_SPC_ReadTpa_LBA [USB_SPC_READ_TPA_LEN] =
{
    0x00, 0x0a, 0x01, 0x01, 0x00, 0x14, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00
};

LOCAL uint8 s_UCOM_UBOT_SPC_ReadTpa_MSF [USB_SPC_READ_TPA_LEN] =
{
    0x00, 0x0a, 0x01, 0x01, 0x00, 0x14, 0x01, 0x00,
    0x00, 0x00, 0x02, 0x00
};

uint8 s_UCOM_UBOT_SPC_ReadTpa_LBA_ALL [USB_SPC_READ_TPA_ALL_LEN] =
{
    0x00, 0x12, 0x01, 0x01, 0x00, 0x14, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0xaa, 0x00,  
     0x00, 0x00, 0x00, 0x00 
};

uint8 s_UCOM_UBOT_SPC_ReadTpa_MSF_ALL [USB_SPC_READ_TPA_ALL_LEN] =
{
     0x00, 0x12, 0x01, 0x01, 0x00, 0x14, 0x01, 0x00, 
     0x00, 0x00, 0x02, 0x00, 0x00, 0x14, 0xaa, 0x00,  
     0x00, 0x00, 0x00, 0x00 
};

LOCAL uint8 s_UCOM_UBOT_MMC_ReportKey [USB_MMC_REPORT_KEY_LEN] =
{
    0x00, 0x06, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00
};//0x25

LOCAL uint8 s_UCOM_UBOT_MMC_GetConfiguration_RT0 [USB_MMC_GET_CONFIGURATION_RT0_LEN] =
{
    0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x03, 0x04, 0x00, 0x08, 0x01, 0x00,
    0x00, 0x01, 0x03, 0x04, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x1e, 0x03, 0x00, 0x01, 0x00, 0x03, 0x00
};

LOCAL uint8 s_UCOM_UBOT_MMC_GetConfiguration_RT1 [USB_MMC_GET_CONFIGURATION_RT1_LEN] =
{
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x03, 0x04, 0x00, 0x08, 0x01, 0x00 
};

LOCAL uint8 s_UCOM_UBOT_MMC_GetConfiguration_RT2_CDRead [USB_MMC_GET_CONFIGURATION_RT2_CDREAD_LEN] =
{
    0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x1e, 0x03, 0x00
}; 

LOCAL uint8 s_UCOM_UBOT_MMC_GetConfiguration_RT2_ProfileList [USB_MMC_GET_CONFIGURATION_RT2_PROFILELIST_LEN] =
{
    0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x03, 0x04, 0x00, 0x08, 0x01, 0x00 
}; 


LOCAL uint8 s_UCOM_UBOT_MMC_GetConfiguration_RT2 [USB_MMC_GET_CONFIGURATION_RT2_LEN] =
{
    0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08
}; 

LOCAL uint8 s_UCOM_UBOT_MMC_ReadSubChannel [USB_MMC_READ_SUB_CHANNEL_LEN] =
{
    0x00, 0x15, 0x00, 0x0c, 0x01, 0x14, 0x01, 0x01,
    0x00, 0x00, 0x02, 0x0f, 0x00, 0x00, 0x00, 0x0f
};   //address?

LOCAL uint8 s_UCOM_UBOT_MMC_ReadDiskInformation [USB_MMC_READ_DISK_INFORMATION_LEN] =
{
    0x00, 0x20, 0x0e, 0x01, 0x01, 0x01, 0x01, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};  

LOCAL uint8 s_UCOM_UBOT_MMC_GetPerformance_Type0 [USB_MMC_GET_PERFORMANCE] =
{
   0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x84,  
   0x00, 0x05, 0x14, 0x32, 0x00, 0x00, 0x10, 0x8c, 
};  

LOCAL uint8 s_UCOM_UBOT_MMC_GetPerformance_Type3 [USB_MMC_GET_PERFORMANCE] =
{
   0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x74,  
   0x00, 0x00, 0x10, 0x82, 0x00, 0x00, 0x10, 0x82, 
};  
#endif


//LOCAL uint8 s_UCOM_UBOT_FormatCapacityLock = 0;
/*--------------------------- Global Data -----------------------------------*/


/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Function to fun scsi inquiry
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcInquiry (UCOM_UBOT_CBD_U *pCBD)
{
    char * head   = (char *)&s_UCOM_UBOT_SPC_InquireHead ;
    char * vendor = (char *) g_UCOM_manager.ubot_dev[g_UCOM_manager.cbw.mCBW.bCBWLUN].inf.vendor;
    char * product= (char *) g_UCOM_manager.ubot_dev[g_UCOM_manager.cbw.mCBW.bCBWLUN].inf.product;
    
    uint32 len    =   USB_SPC_INQUIRE_HEAD_LEN +
                      UCOM_UBOT_SCSI_VENDER_STR_LEN +
                      UCOM_UBOT_SCSI_PRODUCT_ID_STR_LEN ;

#ifdef USB_VCOM_AUTORUN_SUPPORT 
      uint8 epvd = (pCBD->bCBD[1])&0x1;
      uint16 allocation_lenth = (pCBD->bCBD[3]<<8)|pCBD->bCBD[4];
      
     if(0x1 == epvd)
 	 {
	 	 memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
	            s_UCOM_UBOT_SPC_Inquire_VPD,\
	            USB_SPC_INQUIRE_HEAD_LEN);
	  
         UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_SPC_INQUIRE_HEAD_LEN);
	  	 UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
     }
     else
     {     
     	 memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
	            head,\
	            USB_SPC_INQUIRE_HEAD_LEN);

	     memcpy ((char *)(g_UCOM_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN),\
	            vendor,\
	            UCOM_UBOT_SCSI_VENDER_STR_LEN);

	     memcpy ((char *)(g_UCOM_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN \
	                                         + UCOM_UBOT_SCSI_VENDER_STR_LEN),\
	            product,\
	            UCOM_UBOT_SCSI_PRODUCT_ID_STR_LEN);

		*(char *)(g_UCOM_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN\
	                                  + UCOM_UBOT_SCSI_VENDER_STR_LEN\
							          + UCOM_UBOT_SCSI_PRODUCT_ID_STR_LEN) = NULL;
		if(allocation_lenth < len)
		{
			len = allocation_lenth;
		}
			
	    //Send data to cache;
	    UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,len);       	
     }

	 return UCOM_UBOT_CBD_SUCESS;
	 
#else      	                      
	    memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
	            head,\
	            USB_SPC_INQUIRE_HEAD_LEN);

	    memcpy ((char *)(g_UCOM_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN),\
	            vendor,\
	            UCOM_UBOT_SCSI_VENDER_STR_LEN);

	    memcpy ((char *)(g_UCOM_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN \
	                                         + UCOM_UBOT_SCSI_VENDER_STR_LEN),\
	            product,\
	            UCOM_UBOT_SCSI_PRODUCT_ID_STR_LEN);

		*(char *)(g_UCOM_UBOT_CBD_Response + USB_SPC_INQUIRE_HEAD_LEN\
	                                  + UCOM_UBOT_SCSI_VENDER_STR_LEN\
							          + UCOM_UBOT_SCSI_PRODUCT_ID_STR_LEN) = NULL;
	    //Send data to cache;
	    UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,len);      
    	return UCOM_UBOT_CBD_SUCESS;
#endif
}

/*****************************************************************************/
//  Description:    For unit test prepare 
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcTestUintReady (UCOM_UBOT_CBD_U *pCBD)
{
    if(UCOM_UBOT_DEV_PRESENT !=  UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN))
    {
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
	    UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
    }

    return UCOM_UBOT_CBD_SUCESS;
}

/*****************************************************************************/
//  Description:    For scsi request
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcRequestSense (UCOM_UBOT_CBD_U *pCBD)
{
    char * response   = (char *)&s_UCOM_UBOT_SPC_RequestSense ;
    
    memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
            response,\
            USB_SPC_REQUEST_SENSE_LEN);

    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        g_UCOM_UBOT_CBD_Response[2]  = 0x6;
        g_UCOM_UBOT_CBD_Response[12] = 0x28;
        g_UCOM_UBOT_CBD_Response[13] = 0;
    }
    else{
        g_UCOM_UBOT_CBD_Response[2]  = 0x2;
        g_UCOM_UBOT_CBD_Response[12] = 0x3a;
        g_UCOM_UBOT_CBD_Response[13] = 0;
    }
    
	if (0x08 == g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength ){
        g_UCOM_UBOT_CBD_Response[2]  = 0x5;
        g_UCOM_UBOT_CBD_Response[12] = 0x3a;
        g_UCOM_UBOT_CBD_Response[13] = 0;
	}

    //Send data to cache;
    UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,\
    				g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength);

    //s_UCOM_UBOT_FormatCapacityLock = TRUE ;

    return UCOM_UBOT_CBD_SUCESS;
}


/*****************************************************************************/
//  Description:    Scsi read tpa
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcReadTpa (UCOM_UBOT_CBD_U *pCBD)
{
#ifdef USB_VCOM_AUTORUN_SUPPORT 
   uint16  alloc_length=0;
   uint8*  buf_ptr = NULL;
   uint8  msf=0,format=0;
   uint32 len=0;
   
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
		msf = ((pCBD->bCBD[1])&0x02)>>1;
		format = (pCBD->bCBD[2])&0x0F;
		alloc_length = (pCBD->bCBD[7]<<8 )|pCBD->bCBD[8];
		
        if(1 == msf)  //msf
		{	
			if((1 == format)||(USB_SPC_READ_TPA_LEN == alloc_length))
			{
				buf_ptr = s_UCOM_UBOT_SPC_ReadTpa_MSF;
				len = USB_SPC_READ_TPA_LEN;
			}
			else
			{
				buf_ptr = s_UCOM_UBOT_SPC_ReadTpa_MSF_ALL;
				len = USB_SPC_READ_TPA_ALL_LEN;
			}		
		}
	    else //lba
	    {
			if((1 == format)||(USB_SPC_READ_TPA_LEN == alloc_length))
			{
				buf_ptr = s_UCOM_UBOT_SPC_ReadTpa_LBA;
				len = USB_SPC_READ_TPA_LEN;
			}
			else
			{
				buf_ptr = s_UCOM_UBOT_SPC_ReadTpa_LBA_ALL;
				len = USB_SPC_READ_TPA_ALL_LEN;
			}
	    }	   

		memcpy ((char *)g_UCOM_UBOT_CBD_Response,(char*)buf_ptr,	len);/*lint !e670*/
	
		//Send data to cache;
		UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,len);
			 
        UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
        return UCOM_UBOT_CBD_SUCESS;
    }

#else
	return UCOM_UBOT_CBD_SUCESS;
#endif
}


#ifdef USB_VCOM_AUTORUN_SUPPORT 
/*****************************************************************************/
//  Description:    Scsi report key
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiReportKey (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{   
		memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
	            		(char*)s_UCOM_UBOT_MMC_ReportKey,\
	            		USB_MMC_REPORT_KEY_LEN);
	
		//Send data to cache;
		UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_MMC_REPORT_KEY_LEN);
			 
   	 	UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
    	return UCOM_UBOT_CBD_SUCESS;
    }
}

/*****************************************************************************/
//  Description:    From scsi mode sense10 (refernce scsi spec)
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcModeSense10 (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
		memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
            	(char*)s_UCOM_UBOT_SPC_ModeSense10,\
            	USB_SPC_MODE_SENSE10_LEN);
	
        //Send data to cache;
        UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_SPC_MODE_SENSE10_LEN);
        UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
        return UCOM_UBOT_CBD_SUCESS;
    }
}

/*****************************************************************************/
//  Description:    Get scsi configuration
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiGetConfiguration (UCOM_UBOT_CBD_U *pCBD)
{
   uint8 RT=0;
   uint16 starting_feature_number=0, allocation_length=0;
   
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{   
		RT = (pCBD->bCBD[1])&0x03;
		starting_feature_number = (pCBD->bCBD[2]<<8 )|pCBD->bCBD[3];
		allocation_length =  (pCBD->bCBD[7]<<8 )|pCBD->bCBD[8];

		if(0 == RT )
		{
			if(allocation_length > USB_MMC_GET_CONFIGURATION_RT0_LEN)
			{
				allocation_length = USB_MMC_GET_CONFIGURATION_RT0_LEN;
			}
			
			memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
		            		(char*)s_UCOM_UBOT_MMC_GetConfiguration_RT0,\
		            		allocation_length);
			
			//Send data to cache;
		    UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,allocation_length);
		}
		else if(1 == RT )
		{			
			memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
		            		(char*)s_UCOM_UBOT_MMC_GetConfiguration_RT1,\
		            		USB_MMC_GET_CONFIGURATION_RT1_LEN);
			
			//Send data to cache;
			UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_MMC_GET_CONFIGURATION_RT1_LEN);
		}
			
		else if(2 == RT)
		{
			if(0x001E == starting_feature_number)  //cd read feature
			{
				memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
		            			(char*)s_UCOM_UBOT_MMC_GetConfiguration_RT2_CDRead,\
		            			USB_MMC_GET_CONFIGURATION_RT2_CDREAD_LEN);
				
				UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_MMC_GET_CONFIGURATION_RT2_CDREAD_LEN);
			}
			else if(0x0 == starting_feature_number)  //profile list
			{
				if(allocation_length > USB_MMC_GET_CONFIGURATION_RT2_PROFILELIST_LEN)
				{
					allocation_length = USB_MMC_GET_CONFIGURATION_RT2_PROFILELIST_LEN;
				}
				memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
		            			(char*)s_UCOM_UBOT_MMC_GetConfiguration_RT2_ProfileList,\
		            			allocation_length);
				
				UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,allocation_length);
			}
			else  //other feature
			{
				memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
		            			(char*)s_UCOM_UBOT_MMC_GetConfiguration_RT2,\
		            			USB_MMC_GET_CONFIGURATION_RT2_LEN);
				
				UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_MMC_GET_CONFIGURATION_RT2_LEN);
			}			
			
		}	
			 
        UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
        return UCOM_UBOT_CBD_SUCESS;
    }
}


/*****************************************************************************/
//  Description:    Get scsi envent status
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiGetEventStatus (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
        *(uint32 * ) g_UCOM_UBOT_CBD_Response      = 0x00028000 ;
        //Send data to cache;
        UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,4);
        UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
        return UCOM_UBOT_CBD_SUCESS;
    }
}

/*****************************************************************************/
//  Description:    Read from sub channel
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiReadSubChannel (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
		memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
            	(char*)s_UCOM_UBOT_MMC_ReadSubChannel,\
            	USB_MMC_READ_SUB_CHANNEL_LEN);
		
        //Send data to cache;
        UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_MMC_READ_SUB_CHANNEL_LEN);
        UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
        return UCOM_UBOT_CBD_SUCESS;
    }
}

/*****************************************************************************/
//  Description:    Get scsi disk information
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiReadDiskInformation (UCOM_UBOT_CBD_U *pCBD)
{
    uint32 write_length = g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength;
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{

		memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
            	(char*)s_UCOM_UBOT_MMC_ReadDiskInformation,\
            	write_length);
		
        //Send data to cache;
        UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,write_length);
        UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
        return UCOM_UBOT_CBD_SUCESS;
    }
}


/*****************************************************************************/
//  Description:    Get scsi performance
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiGetPerformance (UCOM_UBOT_CBD_U *pCBD)
{
   uint8  type=0;
   
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{	
	type = pCBD->bCBD[10];
	if(0x0 == type)
	{
			memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
	            	(char*)s_UCOM_UBOT_MMC_GetPerformance_Type0,\
	            	USB_MMC_GET_PERFORMANCE);
			
	        //Send data to cache;
	        UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_MMC_GET_PERFORMANCE);
	}
	else if(0x3 == type)
	{
			memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
	            	(char*)s_UCOM_UBOT_MMC_GetPerformance_Type3,\
	            	USB_MMC_GET_PERFORMANCE);
			
	        //Send data to cache;
	        UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,USB_MMC_GET_PERFORMANCE);
	}
		
        UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
        return UCOM_UBOT_CBD_SUCESS;
    }
}

UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSyncCache (UCOM_UBOT_CBD_U *pCBD)
{
if(UCOM_UBOT_DEV_PRESENT !=  UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN))
    {
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
	    UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
    }

    return UCOM_UBOT_CBD_SUCESS;
}
#endif


/*****************************************************************************/
//  Description:    Read scsi device format capacity
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiReadFormatCapacites (UCOM_UBOT_CBD_U *pCBD)
{
    uint32 page_num = \
        g_UCOM_manager.ubot_dev[g_UCOM_manager.cbw.mCBW.bCBWLUN].inf.page_number;
    uint32 page_size = \
        g_UCOM_manager.ubot_dev[g_UCOM_manager.cbw.mCBW.bCBWLUN].inf.page_size;

    
    if(UCOM_UBOT_DEV_PRESENT != \
                  UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
	    UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
    }
    
    *(uint32 * ) g_UCOM_UBOT_CBD_Response       = cpu_to_be32( 0x00000010) ;
    *(uint32 * )(g_UCOM_UBOT_CBD_Response + 4)  = cpu_to_be32( page_num)  ;
    *(uint32 * )(g_UCOM_UBOT_CBD_Response + 8)  = cpu_to_be32(page_size | 0x02000000);
    *(uint32 * )(g_UCOM_UBOT_CBD_Response + 12) = cpu_to_be32( page_num );
    *(uint32 * )(g_UCOM_UBOT_CBD_Response + 16) = cpu_to_be32( page_size) ;


    UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,20);
    //s_UBOT_FormatCapacityLock = FALSE ;
    return UCOM_UBOT_CBD_SUCESS;
}
/*****************************************************************************/
//  Description:    Run scsi mode sense6 command
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ScsiSpcModeSense6 (UCOM_UBOT_CBD_U *pCBD)
{
    //Check device is valid;
    if(UCOM_UBOT_DEV_PRESENT != \
                    UCOM_UBOT_CheckDevState(g_UCOM_manager.cbw.mCBW.bCBWLUN)){
        UCOM_UBOT_ResetCache();
        return UCOM_UBOT_CBD_NO_DEVICE;
    }
    else{
        *(uint32 * ) g_UCOM_UBOT_CBD_Response      = cpu_to_be32( 0x03000000) ;
        //Send data to cache;
        UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,4);
        UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
        return UCOM_UBOT_CBD_SUCESS;
    }
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
