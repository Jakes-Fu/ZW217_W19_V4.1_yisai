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
#include "ubot_api.h"
#include "ubot_storage.h"
#include "ubot_scsi_spc.h"
#include "ubot_scsi_sbc.h"
#include "ubot_drv.h"
#include "bsd.h"
#include "udc_api.h"
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
**                             External Function Prototype                    *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                             Data Structures                                *
**---------------------------------------------------------------------------*/
/*--------------------------- Local Data ------------------------------------*/
/*--------------------------- Global Data -----------------------------------*/
extern BLOCK_ID		ubot_main_id;
extern uint32 le32_to_cpu(uint32 dword);
extern uint32 cpu_to_le32(uint32 dword);

UBOT_MANAGER_T g_UBOT_manager ;

const UBOT_FUNCTION g_UBOT_function[UBOT_STATE_MAX] ={
    UBOT_ProcReady,
    UBOT_ProcCBW,
    UBOT_ProcDataIn,
    UBOT_ProcDataOut,
    UBOT_ProcCSW,
    UBOT_ProcDisconnect,
    UBOT_ProcCmdError,
    UBOT_ProcExeError,
    UBOT_ProcCacheFull
};

LOCAL const UBOT_CBD_PROC_T s_UBOT_CBD_proc_table []={

    {UBOT_SBC_READ_10           ,NULL,NULL,UBOT_ScsiSbcRead10},
    {UBOT_SBC_WRITE_10          ,NULL,NULL,UBOT_ScsiSbcWrite10},
    {UBOT_SBC_VERIFY_10         ,NULL,NULL,UBOT_ScsiSbcVerify10},

    {UBOT_SPC_TEST_UNIT_READY   ,NULL,NULL,UBOT_ScsiSpcTestUintReady},
    {UBOT_SBC_READ_CAPACITY     ,NULL,NULL,UBOT_ScsiSbcReadCapacity},
    {UBOT_SPC_REQUEST_SENSE     ,NULL,NULL,UBOT_ScsiSpcRequestSense},
    {UBOT_SPC_INQUIRY           ,NULL,NULL,UBOT_ScsiSpcInquiry},
    {UBOT_SPC_MODE_SENSE_6      ,NULL,NULL,UBOT_ScsiSpcModeSense6},
    {UBOT_SPC_READ_FORMAT_CAPACITIES
                                ,NULL,NULL,UBOT_ScsiReadFormatCapacites},
    {UBOT_SBC_PREVENT_ALLOW_MEDIUM_REMOVAL
                                ,NULL,NULL,UBOT_ScsiSbcMediaRemove},
    {UBOT_SBC_START_STOP_UNIT   ,NULL,NULL,UBOT_ScsiSbcStartStop},

    {UBOT_SBC_READ_12           ,NULL,NULL,NULL},
    {UBOT_SBC_WRITE_12          ,NULL,NULL,NULL},
    {UBOT_SBC_MODE_SELECT_6     ,NULL,NULL,NULL},
    {UBOT_SBC_MODE_SELECT_10    ,NULL,NULL,NULL},
    {UBOT_SBC_MODE_SENSE_10     ,NULL,NULL,UBOT_ScsiSbcModeSense10},
    {UBOT_SPC_READ_TOC_PMA_ATIP ,NULL,NULL,UBOT_ScsiSpcReadTpa},
    {UBOT_SBC_MAX               ,NULL,NULL,NULL}
};
/*--------------------------- External Data ---------------------------------*/
/*----------------------------------------------------------------------------*
**                         Local Function Prototype                           *
**---------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Function to clear CBW.
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void UBOT_ClearCBW (void)
{
    memset ((char * )&g_UBOT_manager.cbw.bCBWBuf,NULL,UBOT_CBW_LEN);
}

/*****************************************************************************/
//  Description:    Function to clear CSW.
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC void UBOT_ClearCSW (void)
{
    memset ((char * )&g_UBOT_manager.csw.bCSWBuf,NULL,UBOT_CSW_LEN);
}

/*****************************************************************************/
//  Description:    Function to process other call when it is ready
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
PUBLIC UBOT_STATES_E UBOT_ProcReady (uint32 param,void * ptr)
{
	//SCI_TRACE_LOW:"\nUBOT_ProcReady() enter\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_111_112_2_18_1_54_49_17,(uint8*)"");
    if (!g_UBOT_manager.cache.num){
        if (g_UBOT_manager.cache.lock == UBOT_UNLOCK){
			//SCI_TRACE_LOW:"\nUBOT_ProcReady() startreceivedata\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_114_112_2_18_1_54_49_18,(uint8*)"");
            //Start receive data from usb;
            UBOT_StartReceiveData(UBOT_CBW_LOCK,\
                                  UBOT_CBW_LEN,\
                                  (uint32 *)g_UBOT_manager.cache.buf);
        }
		//SCI_TRACE_LOW:"\nUBOT_ProcReady() exit\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_120_112_2_18_1_54_49_19,(uint8*)"");
        //If cache is empty ,return current state;
        return g_UBOT_manager.state ;
    }
    else {
		//SCI_TRACE_LOW:"\nUBOT_ProcReady() UBOT_STATE_CBW\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_125_112_2_18_1_54_49_20,(uint8*)"");
        return UBOT_STATE_CBW;
    }
}

/*****************************************************************************/
//  Description:    Function to process CBW
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_STATES_E UBOT_ProcCBW (uint32 param,void * ptr)
{
    UBOT_CBD_ERR_E result = UBOT_CBD_MAX_ERR;

	//SCI_TRACE_LOW:"\nUBOT_ProcCBW() enter\n"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_138_112_2_18_1_54_49_21,(uint8*)"");
    //read one page data and send it to scsi;
    if (UBOT_CBW_LEN == UBOT_ReadCache((uint8 * )&g_UBOT_manager.cbw.bCBWBuf,\
                                        UBOT_CBW_LEN)){
        //Swap little ending to big ending;
        g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength =le32_to_cpu(
            	g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength);
/*delete for rec uncorrect data, lead to assert*/
	if((g_UBOT_manager.cbw.mCBW.dwCBWSignature !=cpu_to_le32( UBOT_CBW_SIGNATURE)))
		{
			//SCI_TRACE_LOW:"\nUBOT_ProcCBW() err\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_147_112_2_18_1_54_49_22,(uint8*)"");
           return UBOT_STATE_COMMAND_ERR;
		}

        UBOT_ResetCache();

        //Execute scsi cmd then check result ;
        result = UBOT_ProcCBD (\
                    (UBOT_CBD_U *)&g_UBOT_manager.cbw.mCBW.uCBWCB);
        //Check execute result ;
        if (UBOT_CBD_SUCESS != result ){
//           SCI_TRACE_LOW ("UPM CBD Err is 0x%x",result);
           g_UBOT_manager.cbd_err = result ;
			//SCI_TRACE_LOW:"\nUBOT_ProcCBW() cmd err\n"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_160_112_2_18_1_54_49_23,(uint8*)"");
           return UBOT_STATE_COMMAND_ERR;
        };
    }
    else{
        //printf("Read fail\r\n");
		//SCI_TRACE_LOW:"\nUBOT_ProcCBW() read fail\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_166_112_2_18_1_54_49_24,(uint8*)"");
        return g_UBOT_manager.state ;
    }

    //Set state base on bDirection ;
    if (g_UBOT_manager.cbw.mCBW.bmCBWFlags.mBits.bDirection){
		//SCI_TRACE_LOW:"\nUBOT_ProcCBW() data in\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_172_112_2_18_1_54_49_25,(uint8*)"");
        return UBOT_STATE_DATA_IN;
    }
    else{
		//SCI_TRACE_LOW:"\nUBOT_ProcCBW() data out\n"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_176_112_2_18_1_54_49_26,(uint8*)"");
        return UBOT_STATE_DATA_OUT;
    }
}
/*****************************************************************************/
//  Description:    Function to process CBD
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_CBD_ERR_E UBOT_ProcCBD (UBOT_CBD_U *pCBD)
{
    UBOT_CBD_ERR_E tError = UBOT_CBD_NO_EXECUTE;
    int i, max_cnt;

    if (pCBD == SCI_NULL) /*lint !e774 */
    {
    	return tError;
    }

    max_cnt = sizeof(s_UBOT_CBD_proc_table)/sizeof(s_UBOT_CBD_proc_table[0]);
    //Dispatch command process because pCBD->cmd
    for (i = 0 ; i < max_cnt; i++){
        if (pCBD->cmd == s_UBOT_CBD_proc_table[i].cmd){
            if (s_UBOT_CBD_proc_table[i].proc != NULL){
               // SCI_TRACE_LOW ("UPM Execute scsi cmd_0x%x\r\n",pCBD->cmd );
                tError = s_UBOT_CBD_proc_table[i].proc (pCBD);
            }
            else {
                tError = UBOT_CBD_NOREG_FUCTION;
            	// UBOT_ERR_TRACE ("UBOT CBD_0x%x_NOREG_FUCTION",s_UBOT_CBD_proc_table[i].cmd);
            }
            break;
        }
    }

    if (max_cnt  == i){
        tError = UBOT_CBD_UNDEFINED_CMD;
        // UBOT_ERR_TRACE ("UBOT UBOT_CBD_UNDEFINED_CMD is 0x%x",pCBD->cmd);
    }

    return tError;
}

/*****************************************************************************/
//  Description:    Function to process data-in
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_STATES_E UBOT_ProcDataIn (uint32 param,void * ptr)
{
    uint32 data_len  = g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength;

    if (NULL == data_len){
		//SCI_TRACE_LOW:"UBOT_ProcDataIn() CSW 1"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_227_112_2_18_1_54_49_27,(uint8*)"");
        return UBOT_STATE_CSW;
    }

    if (g_UBOT_manager.cache.num){
        //If chache has data waiting for sent, send them ;
        UBOT_SendCache(UBOT_DATA_IN_LOCK);
    }

	//SCI_TRACE_LOW:"UBOT_ProcDataIn() CSW 2"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_236_112_2_18_1_54_49_28,(uint8*)"");
    return UBOT_STATE_CSW;
}

/*****************************************************************************/
//  Description:    Function to process data-out
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_STATES_E UBOT_ProcDataOut (uint32 param,void * ptr)
{
    uint32 data_len  = g_UBOT_manager.cbw.mCBW.dwCBWDataTransferLength;

    if (NULL == data_len){
        return UBOT_STATE_CSW;
    }
    /*
    *Only write(10) can send data to device;
    *Don't support write(12) command;
    *If other cmd send data to device ,assert it is invalid;
    */
    /*
    UBOT_ASSERT (g_UBOT_manager.cbw.mCBW.uCBWCB.cmd == UBOT_SBC_WRITE_10);
    */
    if(g_UBOT_manager.cbw.mCBW.uCBWCB.cmd != UBOT_SBC_WRITE_10)
    {
        return UBOT_STATE_COMMAND_ERR;
    }

    UBOT_ResetCache();

    if (UBOT_WriteDataToBSD(data_len)) {
        return UBOT_STATE_CSW;
    } else {
        return UBOT_STATE_COMMAND_ERR;
    }
}
/*****************************************************************************/
//  Description:    Function to check device status
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_DEVICE_STATE_E UBOT_CheckDevState (uint8 id)
{
    return g_UBOT_manager.dev[g_UBOT_manager.cbw.mCBW.bCBWLUN].state ;
}

/*****************************************************************************/
//  Description:    Function to process CSW
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_STATES_E UBOT_ProcCSW (uint32 param,void * ptr)
{
	xSignalHeader	signalout;

	//SCI_TRACE_LOW:"UBOT_ProcCSW()"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,UBOT_STORAGE_290_112_2_18_1_54_49_29,(uint8*)"");
    UBOT_ResetCache();

    g_UBOT_manager.csw.mCSW.dwCSWSignature  = \
                            cpu_to_le32(UBOT_CSW_SIGNATURE);
    g_UBOT_manager.csw.mCSW.dwCSWTag       = g_UBOT_manager.cbw.mCBW.dwCBWTag;

    g_UBOT_manager.csw.mCSW.dwCSWDataResidue = NULL ;
    g_UBOT_manager.csw.mCSW.bwCSWStatus      = NULL ;

    //Write data to cache ;
    UBOT_WriteCache ((uint8 *)g_UBOT_manager.csw.bCSWBuf,UBOT_CSW_LEN);
    UBOT_SendCache(UBOT_CSW_LOCK);

    UBOT_ClearCBW();
    UBOT_ClearCSW();

	signalout = SCI_ALLOC(sizeof(*signalout));
	signalout->SignalCode = UBOT_CMD_EXECUTE;
	signalout->SignalSize = sizeof(*signalout);
	signalout->Sender = SCI_IdentifyThread();
	SCI_SendSignal(signalout, ubot_main_id);

    return UBOT_STATE_READY;    //return to ready state ;
}

/*****************************************************************************/
//  Description:    Function to process command error
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_STATES_E UBOT_ProcCmdError (uint32 param,void * ptr)
{
    UBOT_MANAGER_T *umass = &g_UBOT_manager;
	xSignalHeader signalout;

    UBOT_ResetCache();

    umass->csw.mCSW.dwCSWSignature = cpu_to_le32(UBOT_CSW_SIGNATURE);
    umass->csw.mCSW.dwCSWTag = umass->cbw.mCBW.dwCBWTag;

    umass->csw.mCSW.dwCSWDataResidue = cpu_to_le32(umass->cbw.mCBW.dwCBWDataTransferLength);
    umass->csw.mCSW.bwCSWStatus      = TRUE ;

    //Write data to cache ;
    UBOT_WriteCache ((uint8 *)g_UBOT_manager.csw.bCSWBuf,UBOT_CSW_LEN);
    UBOT_SendCache(UBOT_CSW_LOCK);

    UBOT_ClearCBW();
    UBOT_ClearCSW();

    // UPM_SendCommand (UPM_MASS_STORAGE,UBOT_CMD_EXECUTE,NULL,NULL);
	signalout = SCI_ALLOC(sizeof(*signalout));
	signalout->SignalCode = UBOT_CMD_EXECUTE;
	signalout->SignalSize = sizeof(*signalout);
	signalout->Sender = SCI_IdentifyThread();
	SCI_SendSignal(signalout, ubot_main_id);

    return UBOT_STATE_READY;
}

/*****************************************************************************/
//  Description:    Function to process exe-error
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_STATES_E UBOT_ProcExeError (uint32 param,void * ptr)
{
    return g_UBOT_manager.state ;
}

/*****************************************************************************/
//  Description:    Function to process device disconnect
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_STATES_E UBOT_ProcDisconnect (uint32 param,void * ptr)
{
    return g_UBOT_manager.state ;
}

/*****************************************************************************/
//  Description:    Function to process cache full status
//  Global resource dependence:
//  Author:         Daniel.Ding
//  Note:
/*****************************************************************************/
UBOT_STATES_E UBOT_ProcCacheFull (uint32 param,void * ptr)
{
    return g_UBOT_manager.state ;
}

/**---------------------------------------------------------------------------*
**                         Compiler Flag                                      *
**---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
// End
