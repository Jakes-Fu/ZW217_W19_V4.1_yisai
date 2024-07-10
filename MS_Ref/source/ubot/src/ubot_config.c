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
#include "ms_ref_ubot_trc.h"
#include "ubot_config.h"
#include "usb_drv.h"
#include "bsd.h"
#include "scm_api.h"
#include "dal_lcd.h"
#include "prod_param.h"
#ifdef UDISK_PC_CONNECT
#include "flash.h"
#include "xsr_partition.h"
#endif

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
extern uint8 *g_UBOT_CBD_Response ;
/*--------------------------- Local Data ------------------------------------*/
LOCAL PUBOT_CFG_INFO_T 		s_ubot_cfg_info_ptr  = PNULL;
LOCAL UBOT_DEVICE_INF_T* 	s_ubot_disk_info_ptr = PNULL;
LOCAL const unsigned char   s_ubot_hex_string[]  = "0123456789ABCDEF";

/*--------------------------- Global Data -----------------------------------*/

/*--------------------------- External Data ---------------------------------*/
extern uint8 g_UBOT_MaxLUN [];
extern UBOT_MANAGER_T g_UBOT_manager ;

/*--------------------------- External Func ---------------------------------*/

/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
PUBLIC BOOLEAN    ubot_config_init()
{

    if(g_UBOT_CBD_Response == NULL)
    {
        g_UBOT_CBD_Response = SCI_ALLOC(UBOT_MAX_ALLOC_LEN);
    }
    if(g_UBOT_CBD_Response == NULL)
    {
        return SCI_FALSE;
    }
    SCI_MEMSET(g_UBOT_CBD_Response,0,UBOT_MAX_ALLOC_LEN);/*lint !e718 !e746 -e718 -e746*/
    return SCI_TRUE;
}
PUBLIC void    ubot_config_exit()
{
    if(NULL != g_UBOT_CBD_Response)
    {
        SCI_FREE(g_UBOT_CBD_Response);
    }
    g_UBOT_CBD_Response = NULL; 
}

/*****************************************************************************/
//  Description:    Get ubot serial number
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
LOCAL void _UBOT_GetSerialNumber( uint8 * serialNum )
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
//  Description:    Get customer disk number
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
LOCAL uint8 _UBOT_GetCustomerDiskNum (void)
{
    uint32 num;
	
    num =  s_ubot_cfg_info_ptr->disk_count;

    UBOT_ASSERT ((num >= 1)&&(num <= UBOT_MAX_DISK_NUM)); /*assert verified*/

    return num ;
}

/*****************************************************************************/
//  Description:    Get customer disk information
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
LOCAL UBOT_DEVICE_INF_T * _UBOT_GetCustomerDiskInf (uint8 id)
{
	UBOT_ASSERT(id < s_ubot_cfg_info_ptr->disk_count); /*assert verified*/
	
    return (UBOT_DEVICE_INF_T *)&s_ubot_disk_info_ptr[id];
}

/*****************************************************************************/
//  Description:    Get ubot device id
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
LOCAL uint32 _UBOT_GetDevID (uint8 *dev_name)
{
    int i,cnt;

    cnt = _UBOT_GetCustomerDiskNum();
    for (i=0 ; i < cnt;i++){
        if (!strcmp((char *)s_ubot_disk_info_ptr[i].dev_name,(char *)dev_name)){
            return i;   
        }
    }

    if(cnt == i)
    {
        UBOT_ASSERT (FALSE); /*assert verified*/
    }
	
    return i;
}
/*----------------------------------------------------------------------------*
**                        Public Function Definitions                         *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Customer configurate the device
//  Global resource dependence: 
//  Author:         Daniel.Ding
//  Note:           
/*****************************************************************************/
PUBLIC void UBOT_CustomerConfig (void)
{
    BSD_STATUS  status;
    int32 i,maxlun;
    int32 index;
    uint32 num = 0;
    BSD_DEVICE  *ubot_dev_ptr 	 = NULL ;    

    ubot_dev_ptr = SCI_ALLOC_APP(sizeof(BSD_DEVICE));
    if(ubot_dev_ptr == NULL)
    {
        return ;
    }
    SCI_MEMSET(ubot_dev_ptr,0,sizeof(BSD_DEVICE));/*lint !e516 -e516*/

    // Get config information for customer
    s_ubot_cfg_info_ptr = UBOT_GetCfgInfo();
    UBOT_ASSERT(PNULL != s_ubot_cfg_info_ptr); /*assert verified*/

    s_ubot_disk_info_ptr = s_ubot_cfg_info_ptr->pdisk_info;
    num = _UBOT_GetCustomerDiskNum();

    _UBOT_GetSerialNumber ((uint8 *)&(s_ubot_cfg_info_ptr->pusb_str_serialnum_desc->SerialNum));

    /*Init UBOT_manager;*/
    for (i = 0;i<UBOT_MAX_DISK_NUM;i++){
        g_UBOT_manager.dev[i].state = UBOT_DEV_NO_PRESENT ;
    }
    
    maxlun = 0;
    index  = 0;
    
    for(i = 0; i< (int32)num; i++)
    {
        status = BSD_GetDevice((char *)s_ubot_disk_info_ptr[i].dev_name ,\
                                    ubot_dev_ptr);
        /*
        * pnp device is inited with some delay,because task exec is slowly
        * so we should add some addition handle here
        */
        /*add code here later*/
                
        if((BSD_SUCCESS == status)&&(!s_ubot_disk_info_ptr[i].dev_state.mBits.bHidden))
        {
            s_ubot_disk_info_ptr[i].dev_state.mBits.bState = TRUE;
            s_ubot_disk_info_ptr[i].page_size   = ubot_dev_ptr->SectorSize;
            s_ubot_disk_info_ptr[i].page_number = ubot_dev_ptr->TotScts;
	     	g_UBOT_manager.dev[index].cache_number = s_ubot_disk_info_ptr[i].cache_number;

            maxlun++;

            /*device exist*/
            g_UBOT_manager.dev[index].inf = *_UBOT_GetCustomerDiskInf((uint8)i);
            /* Set disk have been present ;*/
            g_UBOT_manager.dev[index].state = UBOT_DEV_PRESENT ;

           
#ifdef UDISK_PC_CONNECT
            if(!strcmp((char *)s_ubot_disk_info_ptr[i].dev_name, STL_UDISK_FS_PART))
            {
                if(g_UBOT_manager.dev[index].page_size == 512)
    			{
                    if(FLASH_M18_Type_Get())
                    {		
                        g_UBOT_manager.dev[index].cache_number = g_UBOT_manager.dev[index].cache_number/2;
                    }
    			}

           	    g_UBOT_manager.dev[index].page_size    = ubot_dev_ptr->SectorSize;
                g_UBOT_manager.dev[index].page_number  = ubot_dev_ptr->TotScts;
			
              	if(ubot_dev_ptr->SectorSize * ubot_dev_ptr->TotScts >  g_UBOT_manager.dev[index].cache_number*g_UBOT_manager.dev[index].page_size)
        		{
                    //SCI_TRACE_LOW:"udisk capatcity overflow, pls reduce udisk capacity below cache size if you want to use pc connect func!!!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_CONFIG_233_112_2_18_1_54_43_0,(uint8*)"");
                    //SCI_ASSERT(0);
              	}
                else
                {
                    g_UBOT_manager.dev[index].cache_number = \
                    ((s_ubot_disk_info_ptr[i].cache_number > ubot_dev_ptr->TotScts) ? ubot_dev_ptr->TotScts : s_ubot_disk_info_ptr[i].cache_number);
                }
            }
            else
            {
                g_UBOT_manager.dev[index].cache_number = 0;
            }
#endif
          
            index++;
        }
        else
        {
            s_ubot_disk_info_ptr[i].dev_state.mBits.bState   = FALSE;
            s_ubot_disk_info_ptr[i].page_size   = 0;
            s_ubot_disk_info_ptr[i].page_number = 0;
        }
    }

    if((maxlun >= 1)&&(maxlun <= UBOT_MAX_DISK_NUM))
    {
        /*cal maxLUN based on USB MSC BULKONLY protocol*/
        g_UBOT_MaxLUN[0] = maxlun-1;
    }
    else
    {
        /*no device exist*/
        g_UBOT_MaxLUN[0] = UBOT_MAX_DISK_NUM; 
    }
    SCI_Free(ubot_dev_ptr);
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
	if(PNULL == s_ubot_disk_info_ptr)
	{
		UBOT_CustomerConfig();
	}
	
	index = _UBOT_GetDevID (dev_name);
    s_ubot_disk_info_ptr[index].dev_state.mBits.bHidden = state ;
	
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
    if(PNULL == s_ubot_disk_info_ptr)
	  {
		    UBOT_CustomerConfig();
	  }
	  index = _UBOT_GetDevID (dev_name);
    return s_ubot_disk_info_ptr[index].dev_state.mBits.bHidden;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 

