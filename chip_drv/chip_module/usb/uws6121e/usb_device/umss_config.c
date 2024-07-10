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
//#include "ms_ref_ubot_trc.h"
//#include "ubot_config.h"
#include "usb_drv.h"
#include "bsd.h"
#include "scm_api.h"
//#include "dal_lcd.h"
#include "prod_param.h"
#ifdef UDISK_PC_CONNECT
#include "flash.h"
#include "xsr_partition.h"
#endif
#include "umss_common.h"
#include "usb_utils.h"
#include "sfs.h"
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

/*--------------------------- Local Data ------------------------------------*/

LOCAL UMSS_DEV_T * 	s_umss_active_disk_info_ptr = PNULL;
LOCAL uint8 s_active_disk_num = 0;

LOCAL const unsigned char   s_ubot_hex_string[]  = "0123456789ABCDEF";

/*--------------------------- Global Data -----------------------------------*/

__align(4) LOCAL uint8 s_umss_VenderStr[UMSS_SCSI_VENDER_STR_LEN]={
    "Generic"
};

__align(4) LOCAL uint8 s_umss_ProductIDStrNand[UMSS_SCSI_PRODUCT_ID_STR_LEN]={
    "LOCAL DISK     "
};
__align(4) LOCAL uint8 s_umss_ProductIDStrSD[UMSS_SCSI_PRODUCT_ID_STR_LEN]={
    "SDCard DISK    "
};

__align(4) LOCAL uint8 s_umss_ProductIDStrSD_1[UMSS_SCSI_PRODUCT_ID_STR_LEN]={
    "SDCard_1 DISK  "
};

__align(4) LOCAL uint8 s_umss_ProductIDStrHiddenPartition[UMSS_SCSI_PRODUCT_ID_STR_LEN]={
    "System DISK    "
};


LOCAL UMSS_DEV_T s_umss_DiskInf[] = {
    //NAND Disk
    {
        (uint8*)STL_UDISK_FS_PART,	//
        s_umss_VenderStr,
        s_umss_ProductIDStrNand,
        NULL,					//dev_handle
        512,					//page_size
        NULL,					//page_number
#ifdef UDISK_PC_CONNECT
        300,   /*valid for usb nor disk only*/  /*cache number, 0: no cache*/
#else
        NULL,   /*not used in nandflash platform*/
#endif
		 0,/*lint !e651*/		//dev_state,0:UMSS_DEV_NO_PRESENT
		 0						//is_Hidden
    },

    //SDCard Disk
    {
        (uint8*)STL_SD_DEVICE_NAME,   //UBOT will base on this id to call BSD ;
        s_umss_VenderStr,
        s_umss_ProductIDStrSD,
        NULL,
        NULL,
        NULL,
        NULL,
         0,
         0
    },

    //SDCard_1 Disk
    {
        (uint8*)STL_SD_1_DEVICE_NAME,   //UBOT will base on this id to call BSD ;
        s_umss_VenderStr,
        s_umss_ProductIDStrSD_1,
        NULL,
        NULL,
        NULL,
        NULL,
         0,
         0
    },
    //Hidden Partition
    {
        (uint8*)STL_UDISK_HIDDEN_FS_PART,   //UBOT will base on this id to call BSD ;
        s_umss_VenderStr,
        s_umss_ProductIDStrHiddenPartition,
        NULL,
        NULL,
        NULL,
        NULL,
        0,		//dev_state:no_present
        1		//hidden
    },
};



/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/


/*****************************************************************************/
//  Description:    Get ubot serial number
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
LOCAL void _UMSS_GetSerialNumber( uint8 * serialNum )
{
    uint32  tick = SCI_GetTickCount();
    uint32  i;
    uint8   MobileSerialNumber[16];
    
    SCI_MEMSET ( MobileSerialNumber, NULL, 16 );
    PROD_GetMobileSerialNumber( MobileSerialNumber, 12 );
   
    // Init it.
    for( i = 0; i < (USB_STR_SERIALNUMBER_DESCRIPTOR_MIN /2); i++ )
    {
        *(serialNum+ i * 2 + 0) = '0';
        *(serialNum+ i * 2 + 1) = 0;
    }
    
    if( MobileSerialNumber[0] )
    {
        for( i = 0; i < (USB_STR_SERIALNUMBER_DESCRIPTOR_MIN /2); i++ )
        {
            *(serialNum+ i * 2 + 0) = MobileSerialNumber[i];
        }
    }
    else
    {
        for( i = 0; i < (USB_STR_SERIALNUMBER_DESCRIPTOR_MIN /2); i++ )
        {
            *(serialNum+ i * 2 + 0) = s_ubot_hex_string[ tick & 0xF ];
            tick >>= 1;
        }
    }
}

/*****************************************************************************/
//  Description:    Get ubot device id
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
LOCAL uint32 _UMSS_GetDevID (uint8 *dev_name)
{
    int i,cnt;

    cnt = umss_GetActiveDiskNum();
    for (i=0 ; i < cnt;i++){
        if (!strcmp((char *)s_umss_active_disk_info_ptr[i].dev_name,(char *)dev_name)){
            return i;   
        }
    }

    if(cnt == i)
    {
        SCI_ASSERT(FALSE); /*assert verified*/
    }
	
    return i;
}
/*----------------------------------------------------------------------------*
**                        Public Function Definitions                         *
**---------------------------------------------------------------------------*/
PUBLIC uint8 umss_GetActiveDiskNum (void)
{
	return s_active_disk_num;
}

PUBLIC UMSS_DEV_T * umss_GetActiveDiskDev (void)
{
	return s_umss_active_disk_info_ptr;
}

PUBLIC void  umss_CustomerDeconfig (void)
{
	uint8 i = 0;
	BSD_STATUS  status;
	UMSS_DEV_T *t_umss_disk_info_ptr = PNULL;
	uint32 disknum = sizeof(s_umss_DiskInf)/sizeof(UMSS_DEV_T);
	t_umss_disk_info_ptr = &s_umss_DiskInf[0];
	
	for (i = 0;i<disknum;i++){
        t_umss_disk_info_ptr[i].dev_state = UMSS_DEV_NO_PRESENT ;
    }

	for (i = 0; i< s_active_disk_num ; i++)
	{
		if(s_umss_active_disk_info_ptr[i].dev_state != UMSS_DEV_OPENED)
			continue;
		
		status = BSD_Close (s_umss_active_disk_info_ptr[i].dev_handle);
		if(status != BSD_SUCCESS)
		{
   			USB_ERR_TRACE("[%s]BSD_Close[%s] fail!\n",s_umss_active_disk_info_ptr[i].dev_name);
			continue;
		}
		s_umss_active_disk_info_ptr[i].dev_state = UMSS_DEV_CLOSE;
	}

	
	if(s_umss_active_disk_info_ptr){
		SCI_Free(s_umss_active_disk_info_ptr);
		s_umss_active_disk_info_ptr= PNULL;
	}
	
	#if 0 
	/*cache close*/
    Ubot_Cache_DeInit();
	#endif
}

/*****************************************************************************/
//  Description:    Customer configurate the device
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC UMSS_DEV_T *  umss_CustomerConfig (void)
{
    BSD_STATUS  status;
    int32 i,maxlun;
    int32 index;
    uint32 disknum = 0;
    BSD_DEVICE  *bsd_dev_ptr 	 = NULL ;    
	UMSS_DEV_T *t_umss_disk_info_ptr = PNULL;
	uint8 tryOpenCnt = 5;
	USB_LOG_TRACE("%s enter\n",__FUNCTION__);

    bsd_dev_ptr = SCI_ALLOC_APP(sizeof(BSD_DEVICE));
    if(bsd_dev_ptr == NULL)
    {
		USB_ERR_TRACE("%s alloc bsd_dev_ptr  fail\n",__FUNCTION__);
        return PNULL;
    }
    SCI_MEMSET(bsd_dev_ptr,0,sizeof(BSD_DEVICE));/*lint !e516 -e516*/
	
	s_umss_active_disk_info_ptr = (UMSS_DEV_T *)SCI_ALLOC_APP(sizeof(UMSS_DEV_T) * UMSS_MAX_DISK_NUM);
	if(s_umss_active_disk_info_ptr == NULL)
    {
    	
		USB_ERR_TRACE("%s alloc s_umss_active_disk_info_ptr fail\n",__FUNCTION__);
		SCI_Free(bsd_dev_ptr);
        return PNULL;
    }
	SCI_MEMSET(s_umss_active_disk_info_ptr,0,sizeof(UMSS_DEV_T) * UMSS_MAX_DISK_NUM);

    // Get config information for customer

    t_umss_disk_info_ptr = &s_umss_DiskInf[0];
    disknum = sizeof(s_umss_DiskInf)/sizeof(UMSS_DEV_T);

    //_UBOT_GetSerialNumber ((uint8 *)&(s_ubot_cfg_info_ptr->pusb_str_serialnum_desc->SerialNum));

    /*Init UBOT_manager;*/
    for (i = 0;i<disknum;i++){
        t_umss_disk_info_ptr[i].dev_state = UMSS_DEV_NO_PRESENT ;
    }
    
    s_active_disk_num = 0;
    index  = 0;
    
    for(i = 0; i< (int32)disknum; i++)
    {
        status = BSD_GetDevice((char *)t_umss_disk_info_ptr[i].dev_name ,bsd_dev_ptr);
        /*
        * pnp device is inited with some delay,because task exec is slowly
        * so we should add some addition handle here
        */
        /*add code here later*/
                
        if((BSD_SUCCESS == status)&&(!t_umss_disk_info_ptr[i].is_Hidden))
        {			
            t_umss_disk_info_ptr[i].dev_state = UMSS_DEV_PRESENT;
            t_umss_disk_info_ptr[i].page_size   = bsd_dev_ptr->SectorSize;
            t_umss_disk_info_ptr[i].page_number = bsd_dev_ptr->TotScts;

	     	//SCI_MEMCPY((uint8 *)s_umss_active_disk_info_ptr[index], (uint8 *)t_umss_disk_info_ptr[i],sizeof(UMSS_DEV_T));
			s_umss_active_disk_info_ptr[index].dev_name = t_umss_disk_info_ptr[i].dev_name;
			s_umss_active_disk_info_ptr[index].cache_number = t_umss_disk_info_ptr[i].cache_number;
			s_umss_active_disk_info_ptr[index].page_size = t_umss_disk_info_ptr[i].page_size;
			s_umss_active_disk_info_ptr[index].page_number = t_umss_disk_info_ptr[i].page_number;
			s_umss_active_disk_info_ptr[index].dev_state = UMSS_DEV_PRESENT;
			s_umss_active_disk_info_ptr[index].is_Hidden = t_umss_disk_info_ptr[i].is_Hidden;
			
			USB_LOG_TRACE("dev_name:%s, is present\n",t_umss_disk_info_ptr[i].dev_name);

            s_active_disk_num++;
                     
            index++;
        }
        else
        {
            t_umss_disk_info_ptr[i].dev_state   = UMSS_DEV_NO_PRESENT;
            t_umss_disk_info_ptr[i].page_size   = 0;
            t_umss_disk_info_ptr[i].page_number = 0;
        }
    }

        /*Sleep some time for shutdown ffs .*/
    SCI_Sleep (500);
	

    for (i = 0; i < index; i++)
    {
        do{
			SCI_Sleep (200);
        	status = BSD_Open ( (char *) s_umss_active_disk_info_ptr[i].dev_name, 
				BSD_GENERIC_READ | BSD_GENERIC_WRITE, &s_umss_active_disk_info_ptr[i].dev_handle);
    	}while(status!=BSD_SUCCESS && tryOpenCnt--);
		if(BSD_SUCCESS != status)
        {
       		USB_ERR_TRACE("[%d]BSD_Open[%s] fail!\n",i,s_umss_active_disk_info_ptr[i].dev_name);
			SCI_Free(bsd_dev_ptr);
			SCI_Free(s_umss_active_disk_info_ptr);
	   		return PNULL;
        }
	    s_umss_active_disk_info_ptr[i].dev_state = UMSS_DEV_OPENED;
    }
	
    SCI_Free(bsd_dev_ptr);
	return s_umss_active_disk_info_ptr;
}

/*****************************************************************************/
//  Description:    Function set the disk to be hidden state
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_SetDiskHidden (uint8 *dev_name, BOOLEAN state)
{
    uint32 index  = 0;
	if(PNULL == s_umss_active_disk_info_ptr)
	{
		s_umss_active_disk_info_ptr = umss_CustomerConfig();
	}
	
	index = _UMSS_GetDevID (dev_name);

    	s_umss_active_disk_info_ptr[index].is_Hidden = UMSS_DEV_HIDDEN ;

	
    return TRUE;
}

/*****************************************************************************/
//  Description:    Function get the disk hidden state
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC BOOLEAN UBOT_GetDiskHiddenState (uint8 *dev_name)
{
    uint32 index  = 0;
    if(PNULL == s_umss_active_disk_info_ptr)
	{
		s_umss_active_disk_info_ptr = umss_CustomerConfig();
	}
	index = _UMSS_GetDevID (dev_name);
	
    return (s_umss_active_disk_info_ptr[index].is_Hidden);

}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 

