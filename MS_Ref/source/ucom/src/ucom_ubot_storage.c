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
#include "ms_ref_ucom_trc.h"
#include "ucom_api.h"
#include "ucom_ubot_storage.h"
#include "ucom_ubot_scsi_spc.h"
#include "ucom_ubot_scsi_sbc.h"
#include "ucom_drv.h"
#include "bsd.h"
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

PUBLIC  uint32 cpu_to_le32 (uint32 dword)
{
#ifndef CHIP_ENDIAN_LITTLE
    return ( ( ( (dword) <<24) &0xff000000L) | ( ( (dword) <<8) &0x00ff0000L) | ( ( (dword) >>8) &0x0000ff00L) | ( ( (dword) >>24) &0x000000ffL));
#else
    return dword;
#endif
}
PUBLIC  uint32 le32_to_cpu (uint32 dword)
{
#ifndef CHIP_ENDIAN_LITTLE
    return ( ( ( (dword) <<24) &0xff000000L) | ( ( (dword) <<8) &0x00ff0000L) | ( ( (dword) >>8) &0x0000ff00L) | ( ( (dword) >>24) &0x000000ffL));
#else
    return dword;
#endif
}


const UCOM_UBOT_FUNCTION g_UCOM_UBOT_function[UCOM_UBOT_STATE_MAX] ={
    UCOM_UBOT_ProcReady,
    UCOM_UBOT_ProcCBW,
    UCOM_UBOT_ProcDataIn,
    UCOM_UBOT_ProcDataOut,
    UCOM_UBOT_ProcCSW,
    UCOM_UBOT_ProcDisconnect,
    UCOM_UBOT_ProcCmdError,
    UCOM_UBOT_ProcExeError,
    UCOM_UBOT_ProcCacheFull
};

LOCAL const UCOM_UBOT_CBD_PROC_T s_UCOM_UBOT_CBD_proc_table [UCOM_UBOT_INSTRUCTION_NUM]={

    {UCOM_UBOT_SBC_READ_10           ,NULL,NULL,UCOM_UBOT_ScsiSbcRead10},
    {UCOM_UBOT_SBC_VERIFY_10         ,NULL,NULL,UCOM_UBOT_ScsiSbcVerify10},
    
    {UCOM_UBOT_SPC_TEST_UNIT_READY   ,NULL,NULL,UCOM_UBOT_ScsiSpcTestUintReady},
    {UCOM_UBOT_SBC_READ_CAPACITY     ,NULL,NULL,UCOM_UBOT_ScsiSbcReadCapacity},
    {UCOM_UBOT_SPC_REQUEST_SENSE     ,NULL,NULL,UCOM_UBOT_ScsiSpcRequestSense},
    {UCOM_UBOT_SPC_INQUIRY           ,NULL,NULL,UCOM_UBOT_ScsiSpcInquiry},
    {UCOM_UBOT_SPC_MODE_SENSE_6      ,NULL,NULL,UCOM_UBOT_ScsiSpcModeSense6},
    {UCOM_UBOT_SBC_PREVENT_ALLOW_MEDIUM_REMOVAL
                                ,NULL,NULL,UCOM_UBOT_ScsiSbcMediaRemove},
    {UCOM_UBOT_SBC_START_STOP_UNIT   ,NULL,NULL,UCOM_UBOT_ScsiSbcStartStop},

    {UCOM_UBOT_SBC_READ_12           ,NULL,NULL,NULL},
    {UCOM_UBOT_SBC_WRITE_12          ,NULL,NULL,NULL},
    {UCOM_UBOT_SBC_MODE_SELECT_6     ,NULL,NULL,NULL},
    {UCOM_UBOT_SBC_MODE_SELECT_10    ,NULL,NULL,NULL},  
    {UCOM_UBOT_SPC_READ_TOC_PMA_ATIP ,NULL,NULL,UCOM_UBOT_ScsiSpcReadTpa},

#ifndef USB_VCOM_AUTORUN_SUPPORT 
    {UCOM_UBOT_SBC_WRITE_10          ,NULL,NULL,UCOM_UBOT_ScsiSbcWrite10},
    {UCOM_UBOT_SPC_READ_FORMAT_CAPACITIES,NULL,NULL,UCOM_UBOT_ScsiReadFormatCapacites},
     {UCOM_UBOT_SBC_MODE_SENSE_10     ,NULL,NULL,NULL},
#endif

#ifdef USB_VCOM_AUTORUN_SUPPORT 
//    {UCOM_UBOT_SPC_MODE_SENSE_10      ,NULL,NULL,UCOM_UBOT_ScsiSpcModeSense10},  
    {UCOM_UBOT_MMC_REPORT_KEY,		NULL,NULL,UCOM_UBOT_ScsiReportKey},
    {UCOM_UBOT_MMC_GET_CONFIGURATION,  NULL,  NULL,  UCOM_UBOT_ScsiGetConfiguration},
    {UCOM_UBOT_MMC_GET_EVENT_STATUS,  NULL,  NULL,  UCOM_UBOT_ScsiGetEventStatus},
    {UCOM_UBOT_MMC_READ_SUB_CHANNEL,  NULL, NULL,   UCOM_UBOT_ScsiReadSubChannel},
    {UCOM_UBOT_MMC_READ_DISK_INFORMATION,  NULL,  NULL, UCOM_UBOT_ScsiReadDiskInformation},
    {UCOM_UBOT_MMC_GET_PERFORMANCE,  NULL,  NULL,  UCOM_UBOT_ScsiGetPerformance},
    {UCOM_UBOT_MMC_SYNC_CACHE,  NULL,  NULL,  UCOM_UBOT_ScsiSyncCache},
#endif

    {UCOM_UBOT_SBC_MAX               ,NULL,NULL,NULL}
};/*lint !e785*/
/*--------------------------- External Data ---------------------------------*/

extern BLOCK_ID		ucom_main_id;


/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description:    Clear CBW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_UBOT_ClearCBW (void)
{
    memset ((char * )&g_UCOM_manager.cbw.bCBWBuf,NULL,UCOM_UBOT_CBW_LEN);
}

/*****************************************************************************/
//  Description:    Clear CSW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC void UCOM_UBOT_ClearCSW (void)
{
    memset ((char * )&g_UCOM_manager.csw.bCSWBuf,NULL,UCOM_UBOT_CSW_LEN);
}

/*****************************************************************************/
//  Description:    Process ready to run other function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
PUBLIC UCOM_UBOT_STATES_E UCOM_UBOT_ProcReady (uint32 param,void * ptr)
{
    if (!g_UCOM_manager.cache.num){
		if (g_UCOM_manager.cache.lock == UCOM_UBOT_UNLOCK){
	        //Start receive data from usb;
	        UCOM_UBOT_StartReceiveData(UCOM_UBOT_CBW_LOCK,\
	        					  UCOM_UBOT_CBW_LEN,\
	        					  (uint32 *)g_UCOM_manager.cache.buf);
        }
    	//If cache is empty ,return current state;
        return g_UCOM_manager.ubot_state ;        
    }
    else {
        return UCOM_UBOT_STATE_CBW;
    }
}

/*****************************************************************************/
//  Description:    Function to execute CBW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcCBW (uint32 param,void * ptr)
{
    UCOM_UBOT_CBD_ERR_E result = UCOM_UBOT_CBD_MAX_ERR;

    //read one page data and send it to scsi;
    if (UCOM_UBOT_CBW_LEN == UCOM_UBOT_ReadCache((uint8 * )&g_UCOM_manager.cbw.bCBWBuf,\
    									UCOM_UBOT_CBW_LEN)){
        //Swap little ending to big ending;
        g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength =
            le32_to_cpu(g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength);

	   if((g_UCOM_manager.cbw.mCBW.dwCBWSignature !=cpu_to_le32(UCOM_UBOT_CBW_SIGNATURE)))
		{
			//SCI_TRACE_LOW:"\nUBOT_ProcCBW() err\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_UBOT_STORAGE_168_112_2_18_1_55_0_5,(uint8*)"");
            return UCOM_UBOT_STATE_COMMAND_ERR;
		}
		UCOM_UBOT_ResetCache();

        //Execute scsi cmd then check result ;
        result = UCOM_UBOT_ProcCBD (\
                    (UCOM_UBOT_CBD_U *)&g_UCOM_manager.cbw.mCBW.uCBWCB);
        //Check execute result ;            
        if (UCOM_UBOT_CBD_SUCESS != result ){
           //SCI_TRACE_LOW:"UCOM_UBOT UPM CBD Err is 0x%x"
           SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_UBOT_STORAGE_178_112_2_18_1_55_0_6,(uint8*)"d",result);
           g_UCOM_manager.cbd_err = result ;
           return UCOM_UBOT_STATE_COMMAND_ERR;
        };
    }
    else{
        //printf("Read fail\r\n");
        return g_UCOM_manager.ubot_state ;
    }

    //Set state base on bDirection ;
    if (g_UCOM_manager.cbw.mCBW.bmCBWFlags.mBits.bDirection){
        return UCOM_UBOT_STATE_DATA_IN;
    }
    else{
        return UCOM_UBOT_STATE_DATA_OUT;
    }
}

/*****************************************************************************/
//  Description:    Function to execute CBD function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_CBD_ERR_E UCOM_UBOT_ProcCBD (UCOM_UBOT_CBD_U *pCBD)
{
    UCOM_UBOT_CBD_ERR_E tError = UCOM_UBOT_CBD_NO_EXECUTE;
    int i;
	uint8 Nullarry[1]={0};
    
    SCI_ASSERT (pCBD != SCI_NULL);/*assert verified*/
    
    //Dispatch command process because pCBD->cmd
    for (i=0;i<UCOM_UBOT_INSTRUCTION_NUM;i++){
        if (pCBD->cmd == s_UCOM_UBOT_CBD_proc_table[i].cmd){
            if (s_UCOM_UBOT_CBD_proc_table[i].proc != NULL){
                //SCI_TRACE_LOW ("UCOM_UBOT UPM Execute scsi cmd_0x%x\r\n",pCBD->cmd );
                tError = s_UCOM_UBOT_CBD_proc_table[i].proc (pCBD);
            }
            else {
                tError = UCOM_UBOT_CBD_NOREG_FUCTION;  
				memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
            		(char*)Nullarry,\
            		1);
	
        		//Send data to cache;
       			UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,1);
        		UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
     			UCOM_UBOT_SendCache(UCOM_UBOT_DATA_IN_LOCK);
            	//SCI_TRACE_LOW:"UCOM_UBOT CBD_0x%x_NOREG_FUCTION"
            	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_UBOT_STORAGE_226_112_2_18_1_55_0_7,(uint8*)"d",s_UCOM_UBOT_CBD_proc_table[i].cmd);
            }
            break;
        }
    }
    
    if (UCOM_UBOT_INSTRUCTION_NUM == i){
        tError = UCOM_UBOT_CBD_UNDEFINED_CMD;

		memcpy ((char *)g_UCOM_UBOT_CBD_Response,\
				(char*)Nullarry,\
				1);
	
		//Send data to cache;
		UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_UBOT_CBD_Response,1);
		UCOM_UBOT_DevCompaction(g_UCOM_manager.cbw.mCBW.bCBWLUN);
		UCOM_UBOT_SendCache(UCOM_UBOT_DATA_IN_LOCK);
        //SCI_TRACE_LOW:"UCOM_UBOT UBOT_CBD_UNDEFINED_CMD is 0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_UBOT_STORAGE_243_112_2_18_1_55_0_8,(uint8*)"d",pCBD->cmd);
    }
    
    return tError;
}

/*****************************************************************************/
//  Description:    Process to read data in
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcDataIn (uint32 param,void * ptr)
{
    uint32 data_len  = g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength;

    if (NULL == data_len){
        return UCOM_UBOT_STATE_CSW;
    }

    if (g_UCOM_manager.cache.num){
    	//If chache has data waiting for sent, send them ;
        UCOM_UBOT_SendCache(UCOM_UBOT_DATA_IN_LOCK);
    }

    return UCOM_UBOT_STATE_CSW;
}

/*****************************************************************************/
//  Description:    Process to deal with data out
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcDataOut (uint32 param,void * ptr)
{
    uint32 data_len  = g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength;

    if (NULL == data_len){
        return UCOM_UBOT_STATE_CSW;
    }
    /*
    *Only write(10) can send data to device;
    *Don't support write(12) command;
    *If other cmd send data to device ,assert it is invalid;
    */
    /*
    UBOT_ASSERT (g_UBOT_manager.cbw.mCBW.uCBWCB.cmd == UBOT_SBC_WRITE_10);
    */
    if(g_UCOM_manager.cbw.mCBW.uCBWCB.cmd != UCOM_UBOT_SBC_WRITE_10)
    {
        //SCI_TRACE_LOW:"UCOM_UBOT cbw cmd write error %x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_UBOT_STORAGE_290_112_2_18_1_55_0_9,(uint8*)"d",g_UCOM_manager.cbw.mCBW.uCBWCB.cmd);
        USB_ErrorHandle(USB_ERR_PARAM_ERR);
        return UCOM_UBOT_STATE_COMMAND_ERR;
    }
    
    UCOM_UBOT_ResetCache();

    if (UCOM_UBOT_WriteDataToBSD(data_len)){
        return UCOM_UBOT_STATE_CSW;
    }
    else {
        return UCOM_UBOT_STATE_COMMAND_ERR;
    }
}
/*****************************************************************************/
//  Description:    Function to check device status
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_DEVICE_STATE_E UCOM_UBOT_CheckDevState (uint8 id)
{
    return g_UCOM_manager.ubot_dev[g_UCOM_manager.cbw.mCBW.bCBWLUN].state ;
}

/*****************************************************************************/
//  Description:    Process to run CSW function
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcCSW (uint32 param,void * ptr)
{
    xSignalHeader	signalout;
	UCOM_UBOT_ResetCache();
    //SCI_TRACE_LOW:"UCOM_UBOT_ProcCSW"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,UCOM_UBOT_STORAGE_321_112_2_18_1_55_0_10,(uint8*)"");
    g_UCOM_manager.csw.mCSW.dwCSWSignature  = \
                            cpu_to_le32(UCOM_UBOT_CSW_SIGNATURE);
    g_UCOM_manager.csw.mCSW.dwCSWTag       = g_UCOM_manager.cbw.mCBW.dwCBWTag;

    g_UCOM_manager.csw.mCSW.dwCSWDataResidue = NULL ;
    g_UCOM_manager.csw.mCSW.bwCSWStatus      = NULL ;

    //Write data to cache ;
    UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_manager.csw.bCSWBuf,UCOM_UBOT_CSW_LEN);
    UCOM_UBOT_SendCache(UCOM_UBOT_CSW_LOCK);

    UCOM_UBOT_ClearCBW();
    UCOM_UBOT_ClearCSW();
	
    signalout = SCI_ALLOC(sizeof(*signalout));
	signalout->SignalCode = UCOM_CMD_EXECUTE;
	signalout->SignalSize = sizeof(*signalout);
	signalout->Sender = SCI_IdentifyThread();
	SCI_SendSignal(signalout, ucom_main_id);
        
        
    return UCOM_UBOT_STATE_READY;    //return to ready state ;
}

/*****************************************************************************/
//  Description:    Process to handle command error
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcCmdError (uint32 param,void * ptr)
{

    xSignalHeader	signalout;
	UCOM_UBOT_ResetCache();
	
    g_UCOM_manager.csw.mCSW.dwCSWSignature   = \
                            USB_DWordEndianSwap(UCOM_UBOT_CSW_SIGNATURE);
    g_UCOM_manager.csw.mCSW.dwCSWTag         =g_UCOM_manager.cbw.mCBW.dwCBWTag;

    g_UCOM_manager.csw.mCSW.dwCSWDataResidue =\
           USB_DWordEndianSwap(g_UCOM_manager.cbw.mCBW.dwCBWDataTransferLength);

    g_UCOM_manager.csw.mCSW.bwCSWStatus      = TRUE ;

    //printf("0x%x Err 0x%x\r\n",g_UBOT_manager.cbw.mCBW.uCBWCB.cmd,
      //                         g_UBOT_manager.csw.mCSW.dwCSWDataResidue);

    //Write data to cache ;
    UCOM_UBOT_WriteCache ((uint8 *)g_UCOM_manager.csw.bCSWBuf,UCOM_UBOT_CSW_LEN);
    UCOM_UBOT_SendCache(UCOM_UBOT_CSW_LOCK);

    UCOM_UBOT_ClearCBW();
    UCOM_UBOT_ClearCSW();


   	signalout = SCI_ALLOC(sizeof(*signalout));
	signalout->SignalCode = UCOM_CMD_EXECUTE;
	signalout->SignalSize = sizeof(*signalout);
	signalout->Sender = SCI_IdentifyThread();
	SCI_SendSignal(signalout, ucom_main_id);   
    
    return UCOM_UBOT_STATE_READY;    //return to ready state ;
}

/*****************************************************************************/
//  Description:    Function to handle exe-error
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcExeError (uint32 param,void * ptr)
{
    return g_UCOM_manager.ubot_state ;
}

/*****************************************************************************/
//  Description:    Function to process device disconnect
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcDisconnect (uint32 param,void * ptr)
{
    return g_UCOM_manager.ubot_state ;
}

/*****************************************************************************/
//  Description:    Function to process cache full statue
//	Global resource dependence: 
//  Author:         Daniel.Ding
//	Note:           
/*****************************************************************************/
UCOM_UBOT_STATES_E UCOM_UBOT_ProcCacheFull (uint32 param,void * ptr)
{
    return g_UCOM_manager.ubot_state ;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End 
