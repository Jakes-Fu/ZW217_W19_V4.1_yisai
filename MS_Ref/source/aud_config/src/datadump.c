/******************************************************************************
 ** File Name:      datadump.c                                            *
 ** Author:         yaye.jiang                                                 *
 ** DATE:           03/21/2013                                                *
 ** Copyright:      2005 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 03/21/2013      yaye.jiang      Create                         *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "os_api.h"
#include "priority_system.h"
#include "base64.h"
#include "chng_freq_drvapi.h"
#include "task_monitor.h"
#include "sfs.h"
#include "datadump.h"
#include "audio_nv_arm.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define SAMPLES_PRINT_ONCE 30
#define SAMPLES_BASE64_PRINT_ONCE 64

#define    RECORD_DEBUG_DATA_THREAD_NAME            "RECORD_DEBUG_DATA_PROCESS"
#define    RECORD_DEBUG_DATA_QUEUE_NAME            "RECORD_DEBUG_DATA_QUEUE"

#define    DATADUMP_TASK_RECORD_DEBUG_DATA_SIZE    (1024*10)

#define DUMP_WRITE_DATA_TO_FILE_SIG 0

#define PRI_DATADUMP_RECORD_DEBUG_DATA_TASK   PRI_VOICE_RECORD_DEBUG_DATA_TASK//31
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef struct
{
uint32 uiFrameBytes; //unit:bytes; bytes once write to buffer
uint32 uiSampleOnce;//output to armlog size
uint32 uiCachCount; //cach number
DUMP_DATA_CALLBACK pDumpDataCallbackFunc;//encode funciton to armlog
}DATADUMP_CONFIG_PARAM_T;

typedef struct
{
DATADUMP_NV_INTO_T suiNvconfig;
DATADUMP_RECORD_DEBUG_DATA_INFO_T stRecordDebugDataInfo[DUMP_DATA_POSITION_MAX_COUNT];
DATADUMP_CONFIG_PARAM_T suiConfigPara[DUMP_DATA_POSITION_MAX_COUNT];
uint32 suiEnableDumpIndex;//times of dumping data to file
uint32 suiDumpCallbackTimes[DUMP_DATA_POSITION_MAX_COUNT];
}DATA_DUMP_PARA_T;

LOCAL BLOCK_ID stDataDumpThreadId = SCI_INVALID_BLOCK_ID;
LOCAL uint32 s_dump_record_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
LOCAL uint32 s_dump_record_freq_change_count = 0;//when is 1, higher in start to record; when is 0, lower in stop recording.

LOCAL DATA_DUMP_PARA_T spDataDumpInfo = {0};//all datadump info

BOOLEAN bBase64Enable = SCI_FALSE;

LOCAL char datadump_index_name[32][64]=
{
        {"nxp_dl_from_net"},//index:0
        {"nxp_dl_to_da"}, //index:1
        {"nxp_ul_to_net"}, //index:2
        {"nxp_ul_from_ad"}, //index:3
        {"nxp_dl_before_vb"}, //index:4
        {"nxp_dl_to_codec"}, //index:5
        {"nxp_dl_before_cutdown"}, //index:6
        {"nxp_ul_right_from_ad"}, //index:7
        {"nxp_ul_ref_from_ad"}, //index:8
        {"nxp_dl_loopback_delay_before"}, //index:9
        {"nxp_dl_before_fir65"}, //index:10
        {"nxp_dl_before_alc"}, //index:11
        {"nxp_dl_before_nxprx"}, //index:12
        {""},//index:13 reserve for call
        {""},//index:14 reserve for call
        {""},//index:15 reserve for call
        {""},//index:16 reserve for call
        {""},//index:17 reserve for call
        {""},//index:18 reserve for call
        {""},//index:19 reserve for call
        {""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""},{""}
        //reserve max:31
};
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description: thread
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _datadump_cstowcs(uint16 *p_dst, const char *p_src, int i_len);

/*****************************************************************************/
//  Description: thread
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_RecordDebugData_Thread_Entry(
                                                uint32 arg,
                                                void   *argv
                                                );

/*****************************************************************************/
//  Description:    This Function is used to construct data_dump.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_Construct(void);

/*****************************************************************************/
//  Description: init dump para config
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_initPosConfigPara(uint32 uiPosindex);

/*****************************************************************************/
//  Description: reset dump para config
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_resetPosConfigPara(uint32 uiPosindex);

/*****************************************************************************/
//  Description: callback option after buffer is full for armlog
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_Callback(
                        uint32 uiPositionIndex,
                        uint16 *psBuf,
                        uint32 uiLength //unit:byte.
                        );

/*****************************************************************************/
//  Description: callback option after buffer is full for armlog(BASE64)
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL  void _datadump_BASE64(
                        uint32 uiPositionIndex,
                        char*psBuf,
                        uint32 uiLength //unit:byte.
                        );

/*****************************************************************************/
//  Description: This Function is used to enable the pointed position.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL DUMP_RESULT_E _datadump_EnablePointedPos(
                                                BOOLEAN bOn, // true: start, false: stop
                                                uint32  uiPosIndex, //point the position to start/stop
                                                void*   para
                                                );

/*****************************************************************************/
//  Description: This Function is used to enable the pointed position dump to armlog.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL DUMP_RESULT_E _datadump_EnableDumptoArmlog(
                                                    BOOLEAN bOn, // true: start, false: stop
                                                    uint32  uiPosIndex,//point the position to start/stop
                                                    void*   para
                                                    );

/*****************************************************************************/
//  Description: This Function is used to enable the pointed position dump to file.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL DUMP_RESULT_E _datadump_EnableDumptoFile(
                                                BOOLEAN bOn, // true: start, false: stop
                                                uint32  uiPosIndex,//point the position to start/stop
                                                void*   para
                                                );

/*****************************************************************************/
//  Description: This Function is used to get output file name.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_GetFileName(
                                    uint32  uiPosIndex,//point the position to start/stop
                                    uint16 *uifilename
                                    );

/*****************************************************************************/
//  Description: This Function is used to get config from NV.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL DUMP_RESULT_E _datadump_GetConfigFromNV(void);

/*****************************************************************************/
//  Description: This Function is used to get mode name by type
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL char * _datadump_ArmvbNv_GetName(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#if 1
#endif
/*****************************************************************************/
//  Description: thread
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _datadump_cstowcs(uint16 *p_dst, const char *p_src, int i_len)
{
    int i = 0;
    while(i<i_len && p_src[i])
    {
        p_dst[i] = (int16)(p_src[i]);//lint !e571
        i++;
    }
    if (i<i_len)
    {
        p_dst[i] = 0;
    }
}

/*****************************************************************************/
//  Description: thread
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void _datadump_RecordDebugData_Thread_Entry(
                                                uint32 arg,
                                                void   *argv
                                                )
{
    //@thread info
    BLOCK_ID    this_thread_id = SCI_IdentifyThread();

    DATADUMP_SIGNAL_T *sig_in_ptr = PNULL;

    //@msg drived body
    while (SCI_TRUE)//lint !e716
    {
        sig_in_ptr = (DATADUMP_SIGNAL_T*)SCI_GetSignal(this_thread_id);

        switch (sig_in_ptr->SignalCode)
        {
            case DUMP_WRITE_DATA_TO_FILE_SIG:
            {
                DATADUMP_RECORD_DEBUG_DATA_INFO_T *ptCurPosInfo = (DATADUMP_RECORD_DEBUG_DATA_INFO_T *)
                    &(spDataDumpInfo.stRecordDebugDataInfo[sig_in_ptr->uiPosIndex]);
                DATADUMP_CONFIG_PARAM_T *ptConfigPara = (DATADUMP_CONFIG_PARAM_T *)
                    &(spDataDumpInfo.suiConfigPara[sig_in_ptr->uiPosIndex]);

                if((PNULL == sig_in_ptr->spSrcData)||(0 == sig_in_ptr->uiDataLen))
                {
                /*    SCI_TRACE_LOW("[DATADUMP] _datadump_RecordDebugData_Thread_Entry sig param error, psrc=%d, datalen=%d",
                        sig_in_ptr->spSrcData, sig_in_ptr->uiDataLen);*/
                    break;
                }

                if(DATA_DUMP_TO_ARM_LOG == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
                {

                    /*if(!(ptCurPosInfo->bEnableRecording))
                    {
                        SCI_TRACE_LOW("[DATADUMP] _datadump_RecordDebugData_Thread_Entry not recording, index: %d.",
                            sig_in_ptr->uiPosIndex);
                        break ;
                    }*/
                    if(PNULL == ptConfigPara->pDumpDataCallbackFunc)
                    {
                        SCI_TRACE_LOW("[DATADUMP] _datadump_RecordDebugData_Thread_Entry config para error: callback:0x%x.",
                            ptConfigPara->pDumpDataCallbackFunc);
                        break ;
                    }
                    if(!bBase64Enable)
                    {
                        ptConfigPara->pDumpDataCallbackFunc(sig_in_ptr->uiPosIndex, sig_in_ptr->spSrcData, sig_in_ptr->uiDataLen);
                    }
                    else//base64
                    {
                        DATADUMP_RECORD_CACH_COMPRESS_INFO_T *ptComp = (DATADUMP_RECORD_CACH_COMPRESS_INFO_T *)&(ptCurPosInfo->tCachCompData);
                        int dlen = ptComp->uiCompressBufSize;
                        int base64Result =  0;
                        SCI_MEMSET(ptComp->pucCompressBuffer, 0, ptComp->uiCompressBufSize);

                        base64Result = base64_encode((unsigned char *)(ptComp->pucCompressBuffer), &dlen, (unsigned char *)(sig_in_ptr->spSrcData), sig_in_ptr->uiDataLen);
                        SCI_PASSERT((base64Result==0), ("%d", base64Result));
                        /*SCI_TRACE_LOW("[DATADUMP]  _datadump_RecordDebugData_Thread_Entry base64, pos_index:%d, dlen:%d, srcLen:%d.",
                            sig_in_ptr->uiPosIndex, dlen, sig_in_ptr->uiDataLen);*/

                        ptConfigPara->pDumpDataCallbackFunc(sig_in_ptr->uiPosIndex, (uint16 *)(ptComp->pucCompressBuffer), dlen);
                    }

                    ptCurPosInfo->ptCachData[sig_in_ptr->uiDataIndexOut].uiCurWriteCount = 0;
                    ptCurPosInfo->ptCachData[sig_in_ptr->uiDataIndexOut].uiFlag = 0;
                    ptCurPosInfo->uiAvailableCount += sig_in_ptr->uiDataLen;
                    SCI_TRACE_LOW("[DATADUMP] _datadump_RecordDebugData_Thread_Entry DUMP_TO_ARM_LOG index:%d, len:%d(bytes), cacheIndex:%d, send to ap over,time:%d(ms).",
                        sig_in_ptr->uiPosIndex, sig_in_ptr->uiDataLen, sig_in_ptr->uiDataIndexOut,
                        SCI_GetTickCount());
                }
                else if(DATA_DUMP_TO_FILE == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
                {
                    SFS_ERROR_E write_err_code = SFS_NO_ERROR;

                    uint32 fs_trans = 0;

                    if(SCI_NULL == ptCurPosInfo->hDebugFileHandle)
                    {
                        SCI_TRACE_LOW("[DATADUMP] _datadump_RecordDebugData_Thread_Entry fail to write file, file_handle is NULL!!");
                        break;
                    }
                    write_err_code = SFS_WriteFile(
                                        ptCurPosInfo->hDebugFileHandle,
                                        sig_in_ptr->spSrcData,
                                        sig_in_ptr->uiDataLen,
                                        &fs_trans,
                                        SCI_NULL);
                    if(SFS_NO_ERROR == write_err_code)
                    {
                        ptCurPosInfo->uiAvailableCount += sig_in_ptr->uiDataLen;
                    }
                    else
                    {
                        SCI_TRACE_LOW("[DATADUMP] _datadump_RecordDebugData_Thread_Entry fail to write file, dataSize:%d, index:%d,",
                            sig_in_ptr->uiDataLen, sig_in_ptr->uiPosIndex);
                    }

                    ptCurPosInfo->ptCachData[sig_in_ptr->uiDataIndexOut].uiCurWriteCount = 0;
                    ptCurPosInfo->ptCachData[sig_in_ptr->uiDataIndexOut].uiFlag = 0;
                    SCI_TRACE_LOW("[DATADUMP] _datadump_RecordDebugData_Thread_Entry DUMP_TO_FILE index:%d, len:%d(bytes), cacheIndex:%d, save to file over, result:%d, time:%d(ms).",
                        sig_in_ptr->uiPosIndex, sig_in_ptr->uiDataLen, sig_in_ptr->uiDataIndexOut,
                        write_err_code, SCI_GetTickCount());
                }
            }
            break;

            default:
            break;
        }
        SCI_FREE(sig_in_ptr);
    }
}

/*****************************************************************************/
//  Description:    This Function is used to construct data_dump.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_Construct(void)
{
    if(SCI_INVALID_BLOCK_ID == stDataDumpThreadId)
    {
        //SCI_TRACE_LOW("[DATADUMP]: datadump_createtask");
        stDataDumpThreadId = SCI_CreateAppThread(
                    RECORD_DEBUG_DATA_THREAD_NAME,
                    RECORD_DEBUG_DATA_QUEUE_NAME,
                    _datadump_RecordDebugData_Thread_Entry,// Entry function of the decode thread
                    0,                                    // First parameter for thread entry function,
                    NULL,                                  // Second parameter for thread entry function,
                    DATADUMP_TASK_RECORD_DEBUG_DATA_SIZE, // Size of the thread stack in bytes
                    100,                                  // Number of messages which can be enqueued
                    PRI_DATADUMP_RECORD_DEBUG_DATA_TASK,
                    SCI_PREEMPT,
                    SCI_AUTO_START
                    );
    }

    SCI_ASSERT( SCI_INVALID_BLOCK_ID != stDataDumpThreadId);/*assert verified*/
}

/*****************************************************************************/
//  Description:    This Function is used to destruct data_dump.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
/*
PUBLIC DUMP_RESULT_E Datadump_Destruct(void)
{
    DATADUMP_SIGNAL_T        *sig_ptr = NULL;

    SCI_TRACE_LOW("[DATADUMP]: datadump_closetask");
    if (SCI_INVALID_BLOCK_ID != stDataDumpThreadId)
    {
        SCI_SuspendThread(stDataDumpThreadId);

        //maybe there is signal in the queue
        sig_ptr = (DATADUMP_SIGNAL_T *)SCI_PeekSignal(stDataDumpThreadId);
        while (PNULL != sig_ptr)
        {
            SCI_FREE(sig_ptr);
            sig_ptr = PNULL;
            sig_ptr = (DATADUMP_SIGNAL_T *)SCI_PeekSignal(stDataDumpThreadId);
        }

        SCI_TerminateThread(stDataDumpThreadId);
        SCI_DeleteThread(stDataDumpThreadId);
        stDataDumpThreadId = SCI_INVALID_BLOCK_ID;
    }
    SCI_TRACE_LOW("[DATADUMP]: datadump_closetask");

    return DUMP_NO_ERROR;
}
*/

/*****************************************************************************/
//  Description: init dump para config
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_initPosConfigPara(uint32 uiPosindex)
{
    DATADUMP_CONFIG_PARAM_T *tPara = (DATADUMP_CONFIG_PARAM_T *)
        &spDataDumpInfo.suiConfigPara[uiPosindex];

    if(spDataDumpInfo.stRecordDebugDataInfo[uiPosindex].bEnableRecording)
    {
        SCI_TRACE_LOW("[DATADUMP] _datadump_initPosConfigPara init pos:%d failed, is already enabled", uiPosindex);
        return;
    }

    if(0 == tPara->uiFrameBytes)
    {
        tPara->uiFrameBytes = DUMP_FRAME_DATA_MAX_LEN;
    }

    tPara->uiSampleOnce = tPara->uiFrameBytes*3;

    if(bBase64Enable)
    {
        while(tPara->uiSampleOnce%8)
        {
            tPara->uiSampleOnce = tPara->uiSampleOnce*8;//for BASE64
        }
    }

    if(0 == tPara->uiCachCount)
    {
        tPara->uiCachCount = 3;
    }

    if(0 == tPara->pDumpDataCallbackFunc)
    {
        tPara->pDumpDataCallbackFunc = _datadump_Callback;
    }

    spDataDumpInfo.suiDumpCallbackTimes[uiPosindex] = 0;
    spDataDumpInfo.stRecordDebugDataInfo[uiPosindex].uiAvailableCount = 0;

    SCI_TRACE_LOW("[DATADUMP] _datadump_initPosConfigPara posindex:%d, frameBtyesOnce:%d, cachCount:%d, callback:0x%x.",
        uiPosindex, tPara->uiFrameBytes, tPara->uiCachCount, tPara->pDumpDataCallbackFunc);
}

/*****************************************************************************/
//  Description: reset dump para config
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_resetPosConfigPara(uint32 uiPosindex)
{
    DATADUMP_CONFIG_PARAM_T *tPara = (DATADUMP_CONFIG_PARAM_T *)
        &spDataDumpInfo.suiConfigPara[uiPosindex];

    if(spDataDumpInfo.stRecordDebugDataInfo[uiPosindex].bEnableRecording)
    {
        SCI_TRACE_LOW("[DATADUMP] _datadump_resetPosConfigPara reset pos:%d failed, is still enabled", uiPosindex);
        return;
    }

    tPara->uiFrameBytes = 0;
    tPara->uiSampleOnce = 0;
    tPara->uiCachCount = 0;
    tPara->pDumpDataCallbackFunc = PNULL;

    SCI_TRACE_LOW("[DATADUMP] _datadump_resetPosConfigPara posindex:%d", uiPosindex);

}

/*****************************************************************************/
//  Description: callback option after buffer is full for armlog
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_Callback(
                        uint32 uiPosIndex,
                        uint16 *psBuf,
                        uint32 uiLength //unit:byte.
                        )
{
    uint32 i=0, uiCount = (uiLength>>1)/SAMPLES_PRINT_ONCE;

    SCI_PASSERT((uiPosIndex<DUMP_DATA_POSITION_MAX_COUNT),
        ("Voice_Dump_Pcm_Data_Callback:%d", uiPosIndex));/*assert verified*/

    SCI_PASSERT((uiCount<=0xFF),
            ("Voice_Dump_Pcm_Data_Callback, index:%d, len: %d, %d, %d", uiPosIndex, uiLength, spDataDumpInfo.suiConfigPara[uiPosIndex].uiSampleOnce, uiCount)); /*assert verified*/

    if(bBase64Enable)
    {
        _datadump_BASE64(uiPosIndex, (char *)psBuf, uiLength);//Voice_Dump_Pcm_Compress((char *)psBuf, uiLength);//
        return;
    }

    SCI_PASSERT((uiLength<=spDataDumpInfo.suiConfigPara[uiPosIndex].uiSampleOnce),
            ("Voice_Dump_Pcm_Data_Callback, index:%d, len: %d, %d", uiPosIndex, uiLength, spDataDumpInfo.suiConfigPara[uiPosIndex].uiSampleOnce));    /*assert verified*/


    SCI_PASSERT( (((uiLength>>1)%SAMPLES_PRINT_ONCE)==0),
            ("Voice_Dump_Pcm_Data_Callback, index:%d, len: %d, %d, %d, %d", uiPosIndex, uiLength, spDataDumpInfo.suiConfigPara[uiPosIndex].uiSampleOnce, uiCount, SAMPLES_PRINT_ONCE)); /*assert verified*/

    spDataDumpInfo.suiDumpCallbackTimes[uiPosIndex]++;
#ifdef FILE_SAVE_COMPARE_ENABLE
    if((0==uiPosIndex) && (SCI_NULL!=sFileHandle))
    {
        uint32 fs_trans = 0;
        SFS_WriteFile(sFileHandle, psBuf, uiLength, &fs_trans, SCI_NULL);
    }
#endif
    SCI_TRACE_LOW("VoiceDumpPcmHex index:%d, %d", uiPosIndex, spDataDumpInfo.suiDumpCallbackTimes[uiPosIndex]);

    for(i=0; i<uiCount; i++)
    {
        uint32 uiBase = SAMPLES_PRINT_ONCE*i;
#if 1
        SCI_TRACE_LOW("VoiceDumpPcmExHex:%02x%02x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x%04x",
            uiPosIndex,
            i,
            psBuf[uiBase+0]&0xffff,
            psBuf[uiBase+1]&0xffff,
            psBuf[uiBase+2]&0xffff,
            psBuf[uiBase+3]&0xffff,
            psBuf[uiBase+4]&0xffff,
            psBuf[uiBase+5]&0xffff,
            psBuf[uiBase+6]&0xffff,
            psBuf[uiBase+7]&0xffff,
            psBuf[uiBase+8]&0xffff,
            psBuf[uiBase+9]&0xffff,
            psBuf[uiBase+10]&0xffff,
            psBuf[uiBase+11]&0xffff,
            psBuf[uiBase+12]&0xffff,
            psBuf[uiBase+13]&0xffff,
            psBuf[uiBase+14]&0xffff,
            psBuf[uiBase+15]&0xffff,
            psBuf[uiBase+16]&0xffff,
            psBuf[uiBase+17]&0xffff,
            psBuf[uiBase+18]&0xffff,
            psBuf[uiBase+19]&0xffff,
            psBuf[uiBase+20]&0xffff,
            psBuf[uiBase+21]&0xffff,
            psBuf[uiBase+22]&0xffff,
            psBuf[uiBase+23]&0xffff,
            psBuf[uiBase+24]&0xffff,
            psBuf[uiBase+25]&0xffff,
            psBuf[uiBase+26]&0xffff,
            psBuf[uiBase+27]&0xffff,
            psBuf[uiBase+28]&0xffff,
            psBuf[uiBase+29]&0xffff);
#endif
    }
}

/*****************************************************************************/
//  Description: callback option after buffer is full for armlog(BASE64)
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL  void _datadump_BASE64(
                        uint32 uiPosIndex,
                        char*psBuf,
                        uint32 uiLength //unit:byte.
                        )
{
    uint32 i=0, uiCount=uiLength/(SAMPLES_BASE64_PRINT_ONCE);
    SCI_PASSERT((SAMPLES_BASE64_PRINT_ONCE*uiCount == uiLength),
        ("Voice_Dump_Pcm_BASE64:%d, %d", uiCount, uiLength));/*assert verified*/
    spDataDumpInfo.suiDumpCallbackTimes[uiPosIndex]++;

    SCI_TRACE_LOW("Voice_Dump_Pcm_BASE64 index:%d, %d, %d.",
        uiPosIndex, spDataDumpInfo.suiDumpCallbackTimes[uiPosIndex], uiLength);
#if 1
    for(i=0; i<uiCount; i++)
    {
        uint32 uiBase = SAMPLES_BASE64_PRINT_ONCE*i;

        SCI_TRACE_LOW("VoiceDumpPcmExData:%02x%02x%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            uiPosIndex,
            i,
            psBuf[uiBase+0],
            psBuf[uiBase+1],
            psBuf[uiBase+2],
            psBuf[uiBase+3],
            psBuf[uiBase+4],
            psBuf[uiBase+5],
            psBuf[uiBase+6],
            psBuf[uiBase+7],
            psBuf[uiBase+8],
            psBuf[uiBase+9],
            psBuf[uiBase+10],
            psBuf[uiBase+11],
            psBuf[uiBase+12],
            psBuf[uiBase+13],
            psBuf[uiBase+14],
            psBuf[uiBase+15],
            psBuf[uiBase+16],
            psBuf[uiBase+17],
            psBuf[uiBase+18],
            psBuf[uiBase+19],
            psBuf[uiBase+20],
            psBuf[uiBase+21],
            psBuf[uiBase+22],
            psBuf[uiBase+23],
            psBuf[uiBase+24],
            psBuf[uiBase+25],
            psBuf[uiBase+26],
            psBuf[uiBase+27],
            psBuf[uiBase+28],
            psBuf[uiBase+29],
            psBuf[uiBase+30],
            psBuf[uiBase+31],
            psBuf[uiBase+32],
            psBuf[uiBase+33],
            psBuf[uiBase+34],
            psBuf[uiBase+35],
            psBuf[uiBase+36],
            psBuf[uiBase+37],
            psBuf[uiBase+38],
            psBuf[uiBase+39],
            psBuf[uiBase+40],
            psBuf[uiBase+41],
            psBuf[uiBase+42],
            psBuf[uiBase+43],
            psBuf[uiBase+44],
            psBuf[uiBase+45],
            psBuf[uiBase+46],
            psBuf[uiBase+47],
            psBuf[uiBase+48],
            psBuf[uiBase+49],
            psBuf[uiBase+50],
            psBuf[uiBase+51],
            psBuf[uiBase+52],
            psBuf[uiBase+53],
            psBuf[uiBase+54],
            psBuf[uiBase+55],
            psBuf[uiBase+56],
            psBuf[uiBase+57],
            psBuf[uiBase+58],
            psBuf[uiBase+59],
            psBuf[uiBase+60],
            psBuf[uiBase+61],
            psBuf[uiBase+62],
            psBuf[uiBase+63]);

    }
#else
    VOICE_DUMP_PCM_TRACE("VoiceDumpPcmData:%01x%02x%s",      3,  i,    psBuf);
#endif
}

/*****************************************************************************/
//  Description: This Function is used to enable the pointed position.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL DUMP_RESULT_E _datadump_EnablePointedPos(
                                                BOOLEAN bOn, // true: start, false: stop
                                                uint32  uiPosIndex, //point the position to start/stop
                                                void*   para
                                                )
{
    DUMP_RESULT_E result = DUMP_NO_ERROR;

    if(DATA_DUMP_TO_FILE == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
    {
        result = _datadump_EnableDumptoFile(bOn, uiPosIndex, para);
    }
    else if(DATA_DUMP_TO_ARM_LOG == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
    {
        result = _datadump_EnableDumptoArmlog(bOn, uiPosIndex, para);
    }
    SCI_TRACE_LOW("[DATADUMP] _datadump_EnablePointedPos bon=%d,uiPosIndex=0x%x, result=%d", bOn, uiPosIndex, result);
    return result;
}

/*****************************************************************************/
//  Description: This Function is used to enable the pointed position dump to armlog.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL DUMP_RESULT_E _datadump_EnableDumptoArmlog(
                                                    BOOLEAN bOn, // true: start, false: stop
                                                    uint32  uiPosIndex,//point the position to start/stop
                                                    void*   para
                                                    )
{
    DATADUMP_RECORD_DEBUG_DATA_INFO_T *ptCurPosInfo = (DATADUMP_RECORD_DEBUG_DATA_INFO_T *)
        &(spDataDumpInfo.stRecordDebugDataInfo[uiPosIndex]);
    DATADUMP_RECORD_CACH_INFO_T *ptCurCachInfo = (DATADUMP_RECORD_CACH_INFO_T *)
        &(ptCurPosInfo->ptCachData[ptCurPosInfo->uiCurIndex]);
    DATADUMP_CONFIG_PARAM_T *ptConfigPara = (DATADUMP_CONFIG_PARAM_T *)
        &(spDataDumpInfo.suiConfigPara[uiPosIndex]);

    if(uiPosIndex >=DUMP_DATA_POSITION_MAX_COUNT)
    {
        /*SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog input para error, uiRecordPos(%d, min:0, max:%d) is invalid.",
            uiPosIndex, DUMP_DATA_POSITION_MAX_COUNT - 1);*/
        return DUMP_PARAM_ERROR;
    }

    if(bOn)
    {
        if(ptCurPosInfo->bEnableRecording)
        {
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog the data of this pos:%d has been recording",
                uiPosIndex);
            return DUMP_NO_ERROR;
        }
        else
        {
            uint32 uiBufSize=0, i=0;
            uint32 uiCachByteCount = ptConfigPara->uiSampleOnce;
            if((PNULL == ptConfigPara->pDumpDataCallbackFunc)
                ||(ptConfigPara->uiCachCount<DUMP_CACH_FRAME_COUNT_MIN)
                || (uiCachByteCount<ptConfigPara->uiFrameBytes)
                ||  ((uiCachByteCount%ptConfigPara->uiFrameBytes) != 0))
            {
                SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog config para error: callback:0x%x, cachCount:%d, samplesOnce:%d.",
                    ptConfigPara->pDumpDataCallbackFunc, ptConfigPara->uiCachCount, ptConfigPara->uiSampleOnce);
                return DUMP_PARAM_ERROR;
            }

            ptCurPosInfo->bDumpToFile = SCI_FALSE;
            ptCurPosInfo->uiLostFramesCount = 0;
            ptCurPosInfo->uiCurIndex = 0;

            //alloc ptCachData
            if(PNULL != ptCurPosInfo->ptCachData)
            {
                SCI_FREE(ptCurPosInfo->ptCachData);
            }
            uiBufSize = ptConfigPara->uiCachCount * sizeof(DATADUMP_RECORD_CACH_INFO_T);
            ptCurPosInfo->ptCachData = (DATADUMP_RECORD_CACH_INFO_T *)SCI_ALLOCA(uiBufSize);
            if(PNULL == ptCurPosInfo->ptCachData)
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog alloc ptCachData failed, size: %d.", uiBufSize);
                return DUMP_NO_ENOUGH_MEMORY;
            }
            else
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog alloc ptCachData success, size: %d.", uiBufSize);
                SCI_MEMSET(ptCurPosInfo->ptCachData, 0, uiBufSize);
            }

            //alloc psTotalBuffer
            if(PNULL != ptCurPosInfo->psTotalBuffer)
            {
                SCI_FREE(ptCurPosInfo->psTotalBuffer);
            }
            uiBufSize = (uiCachByteCount * ptConfigPara->uiCachCount);
            ptCurPosInfo->psTotalBuffer = (uint8 *)SCI_ALLOCA(uiBufSize);
            if(PNULL == ptCurPosInfo->psTotalBuffer)
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog alloc psTotalBuffer failed, size: %d.", uiBufSize);
                SCI_FREE(ptCurPosInfo->ptCachData);
                ptCurPosInfo->ptCachData = PNULL;
                return DUMP_NO_ENOUGH_MEMORY;
            }
            else
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog alloc psTotalBuffer success, size: %d.", uiBufSize);
                SCI_MEMSET(ptCurPosInfo->psTotalBuffer, 0, uiBufSize);
            }

            //init ptCachData info
            for(i=0; i<ptConfigPara->uiCachCount; i++)
            {
                ptCurPosInfo->ptCachData[i].psBuffer = ptCurPosInfo->psTotalBuffer + (uiCachByteCount * i);
                ptCurPosInfo->ptCachData[i].uiCurWriteCount = 0;
            }

            //init tCachCompData info(for BASE64 encoder), only need in armlog
            if(PNULL != ptCurPosInfo->tCachCompData.pucCompressBuffer)
            {
                SCI_FREE(ptCurPosInfo->tCachCompData.pucCompressBuffer);
            }
            uiBufSize = (uiCachByteCount<<1); // unit: bytes  ->base64-> 2/3 bytes
//            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog alloc psCompressBuffer success, uiCachByteCount:%d, uiBufSize: %d.", uiCachByteCount, uiBufSize);
            ptCurPosInfo->tCachCompData.pucCompressBuffer = (uint8 *)SCI_ALLOCA(uiBufSize);
            ptCurPosInfo->tCachCompData.uiCompressBufSize = uiBufSize;
            ptCurPosInfo->tCachCompData.puiCompressHandle = NULL;
            if(PNULL== ptCurPosInfo->tCachCompData.pucCompressBuffer)
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog alloc psCompressBuffer failed, size: %d.", uiBufSize);
                SCI_FREE(ptCurPosInfo->ptCachData);
                SCI_FREE(ptCurPosInfo->psTotalBuffer);
                ptCurPosInfo->psTotalBuffer = PNULL;
                ptCurPosInfo->ptCachData = PNULL;
                return DUMP_NO_ENOUGH_MEMORY;
            }
            else
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog alloc psCompressBuffer success, size: %d.", uiBufSize);
                SCI_MEMSET(ptCurPosInfo->tCachCompData.pucCompressBuffer, 0, uiBufSize);
            }
            ptCurPosInfo->bEnableRecording = SCI_TRUE;
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog modem start, index:%d.", uiPosIndex);
            return DUMP_NO_ERROR;
        }
    }
    else
    {
        if(!(ptCurPosInfo->bEnableRecording))
        {
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog the data of this pos:%d has not been recording",
                uiPosIndex);
            return DUMP_NO_ERROR;
        }
        else
        {
            uint32 i=0;
            uint32 uiWaitCount = 0;
            BOOLEAN bOutputFinishflag = SCI_FALSE;

            ptCurPosInfo->bEnableRecording = SCI_FALSE;
            //wait the data finish output in buffer.
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog stop, wait start time: %d.", SCI_GetTickCount());
            while(!bOutputFinishflag)
            {
                bOutputFinishflag = SCI_TRUE;
                for(i=0; i<ptConfigPara->uiCachCount; i++)
                {
                    ptCurCachInfo = (DATADUMP_RECORD_CACH_INFO_T *)&(ptCurPosInfo->ptCachData[i]);
                    if(1 == ptCurCachInfo->uiFlag)
                    {
                        //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog stop flag index:%d is wait for outputing.", i);
                        bOutputFinishflag = SCI_FALSE;
                        break;
                    }
                }
                if(!bOutputFinishflag)
                {
                    //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog stop flag wait output.");
                    SCI_Sleep(5);
                    uiWaitCount++;
                }

                if(uiWaitCount>100)
                {
                    /*SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog sleep to wait last write file over, time:%d(ms), force to break.",
                        uiWaitCount*5);*/
                    break;
                }
            }
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog stop, wait stop time: %d.", SCI_GetTickCount());

            ptCurCachInfo = (DATADUMP_RECORD_CACH_INFO_T *)&(ptCurPosInfo->ptCachData[ptCurPosInfo->uiCurIndex]);
            if((0 == ptCurCachInfo->uiFlag)&&(ptCurCachInfo->uiCurWriteCount>0))//output the last inputed info
            {
/*                SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog stop index:%d,the last unsend Data:%d((0x%x)bytes",
                uiPosIndex, ptCurCachInfo->uiCurWriteCount);*/

                if(PNULL == ptConfigPara->pDumpDataCallbackFunc)
                {
                    SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog config para error: callback:0x%x.",
                        ptConfigPara->pDumpDataCallbackFunc);
                    return DUMP_NO_ERROR;
                }
                if(!bBase64Enable)
                {
                    ptConfigPara->pDumpDataCallbackFunc(uiPosIndex, ptCurCachInfo->psBuffer, ptConfigPara->uiSampleOnce);
                }
                else//base64
                {
                    DATADUMP_RECORD_CACH_COMPRESS_INFO_T *ptComp = (DATADUMP_RECORD_CACH_COMPRESS_INFO_T *)&(ptCurPosInfo->tCachCompData);
                    int dlen = ptComp->uiCompressBufSize;
                    int base64Result =  0;
                    SCI_MEMSET(ptComp->pucCompressBuffer, 0, ptComp->uiCompressBufSize);

                    base64Result = base64_encode((unsigned char *)(ptComp->pucCompressBuffer), &dlen, (unsigned char *)(ptCurCachInfo->psBuffer), ptConfigPara->uiSampleOnce);
                    SCI_PASSERT((base64Result==0), ("%d", base64Result));
                    /*SCI_TRACE_LOW("[DATADUMP]  _datadump_EnableDumptoArmlog base64, pos_index:%d, dlen:%d, srcLen:%d.",
                        uiPosIndex, dlen, ptConfigPara->uiSampleOnce);*/

                    ptConfigPara->pDumpDataCallbackFunc(uiPosIndex, (uint16 *)(ptComp->pucCompressBuffer), dlen);
                }
                ptCurPosInfo->uiAvailableCount += ptConfigPara->uiSampleOnce;
            }

            //output the flag info.
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog stop write flag frames.");

            if (ptCurPosInfo->psTotalBuffer)
                SCI_FREE(ptCurPosInfo->psTotalBuffer);
            if (ptCurPosInfo->ptCachData)
                SCI_FREE(ptCurPosInfo->ptCachData);
            if (ptCurPosInfo->tCachCompData.pucCompressBuffer)
                SCI_FREE(ptCurPosInfo->tCachCompData.pucCompressBuffer);
            ptCurPosInfo->psTotalBuffer = PNULL;
            ptCurPosInfo->ptCachData = PNULL;
            ptCurPosInfo->tCachCompData.pucCompressBuffer = PNULL;
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoArmlog stop, index:%d,lostData:%d((0x%x)frames), sendData:%d((0x%x)bytes), logOutput:%d((0x%x)bytes).",
                uiPosIndex,
                ptCurPosInfo->uiLostFramesCount,ptCurPosInfo->uiLostFramesCount,
                ptCurPosInfo->uiAvailableCount,ptCurPosInfo->uiAvailableCount,
                (spDataDumpInfo.suiDumpCallbackTimes[uiPosIndex] * ptConfigPara->uiSampleOnce),
                (spDataDumpInfo.suiDumpCallbackTimes[uiPosIndex] * ptConfigPara->uiSampleOnce));

            return DUMP_NO_ERROR;
        }
    }
}

/*****************************************************************************/
//  Description: This Function is used to enable the pointed position dump to file.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL DUMP_RESULT_E _datadump_EnableDumptoFile(
                                                BOOLEAN bOn, // true: start, false: stop
                                                uint32  uiPosIndex,//point the position to start/stop
                                                void*   para
                                                )
{
    DATADUMP_RECORD_DEBUG_DATA_INFO_T *ptCurPosInfo = (DATADUMP_RECORD_DEBUG_DATA_INFO_T *)
        &(spDataDumpInfo.stRecordDebugDataInfo[uiPosIndex]);
    DATADUMP_RECORD_CACH_INFO_T *ptCurCachInfo = (DATADUMP_RECORD_CACH_INFO_T *)
        &(ptCurPosInfo->ptCachData[ptCurPosInfo->uiCurIndex]);
    DATADUMP_CONFIG_PARAM_T *ptConfigPara = (DATADUMP_CONFIG_PARAM_T *)
        &(spDataDumpInfo.suiConfigPara[uiPosIndex]);

    if(uiPosIndex >=DUMP_DATA_POSITION_MAX_COUNT)
    {
        /*SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile input para error, uiRecordPos(%d, min:0, max:%d) is invalid.",
            uiPosIndex, DUMP_DATA_POSITION_MAX_COUNT - 1);*/
        return DUMP_PARAM_ERROR;
    }

    if(bOn)
    {
        uint16 unicode_file_name[64] = {0};

        _datadump_GetFileName(uiPosIndex, unicode_file_name);

        if(ptCurPosInfo->bEnableRecording)
        {
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile the data of this pos:%d has been recording",
                uiPosIndex);
            return DUMP_NO_ERROR;
        }
        else
        {
            uint32 uiBufSize=0, i=0;
            uint32 uiCachByteCount = DUMP_DEBUG_DATA_FILE_BUF_SIZE - DUMP_DEBUG_DATA_FILE_BUF_SIZE%ptConfigPara->uiFrameBytes;

            ptCurPosInfo->bDumpToFile = SCI_TRUE;
            ptCurPosInfo->uiLostFramesCount = 0;
            ptCurPosInfo->uiCurIndex = 0;

            //create file
            if(SCI_NULL != ptCurPosInfo->hDebugFileHandle)
            {
                SFS_CloseFile(ptCurPosInfo->hDebugFileHandle);
                ptCurPosInfo->hDebugFileHandle = 0;
            }
            ptCurPosInfo->hDebugFileHandle =
                SFS_CreateFile( (const uint16 *)(unicode_file_name), SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE,NULL,NULL);
            if(SCI_NULL == ptCurPosInfo->hDebugFileHandle)
            {
                SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile hFileHanlde create failed.");
                return DUMP_ERROR;
            }

            //alloc ptCachData
            if(PNULL != ptCurPosInfo->ptCachData)
            {
                SCI_FREE(ptCurPosInfo->ptCachData);
            }
            uiBufSize = ptConfigPara->uiCachCount * sizeof(DATADUMP_RECORD_CACH_INFO_T);
            ptCurPosInfo->ptCachData = (DATADUMP_RECORD_CACH_INFO_T *)SCI_ALLOCA(uiBufSize);
            if(PNULL == ptCurPosInfo->ptCachData)
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile alloc ptCachData failed, size: %d.", uiBufSize);
                return DUMP_NO_ENOUGH_MEMORY;
            }
            else
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile alloc ptCachData success, size: %d.", uiBufSize);
                SCI_MEMSET(ptCurPosInfo->ptCachData, 0, uiBufSize);
            }

            //alloc psTotalBuffer
            if(PNULL != ptCurPosInfo->psTotalBuffer)
            {
                SCI_FREE(ptCurPosInfo->psTotalBuffer);
            }
            uiBufSize = (uiCachByteCount * ptConfigPara->uiCachCount);
            ptCurPosInfo->psTotalBuffer = (uint8 *)SCI_ALLOCA(uiBufSize);
            if(PNULL== ptCurPosInfo->psTotalBuffer)
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile alloc psTotalBuffer failed, size: %d.", uiBufSize);
                SCI_FREE(ptCurPosInfo->ptCachData);
                return DUMP_NO_ENOUGH_MEMORY;
            }
            else
            {
                //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile alloc psTotalBuffer success, size: %d.", uiBufSize);
                SCI_MEMSET(ptCurPosInfo->psTotalBuffer, 0, uiBufSize);
            }

            //init ptCachData info
            for(i=0; i<ptConfigPara->uiCachCount; i++)
            {
                ptCurPosInfo->ptCachData[i].psBuffer = ptCurPosInfo->psTotalBuffer + (uiCachByteCount * i);
                ptCurPosInfo->ptCachData[i].uiCurWriteCount = 0;
            }

            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile start positon_index:%d.", uiPosIndex);

            //init tCachCompData info(for BASE64 encoder)
            if(PNULL != ptCurPosInfo->tCachCompData.pucCompressBuffer)
            {
                SCI_FREE(ptCurPosInfo->tCachCompData.pucCompressBuffer);
            }
            ptCurPosInfo->tCachCompData.pucCompressBuffer = 0;
            ptCurPosInfo->tCachCompData.uiCompressBufSize = 0;
            ptCurPosInfo->tCachCompData.puiCompressHandle = NULL;

            //change freq
            s_dump_record_freq_change_count++;
            if(1==s_dump_record_freq_change_count)
            {
                if (CHNG_FREQ_REQ_HANDLER_NULL == s_dump_record_chng_freq_req_handler)
                {
                    s_dump_record_chng_freq_req_handler =  CHNG_FREQ_GetReqHandler("voice_record");

                    if(CHNG_FREQ_REQ_HANDLER_NULL == s_dump_record_chng_freq_req_handler)
                    {
                        SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile start to record; CHNG_FREQ_REQ_HANDLER_NULL == s_voice_record_chng_freq_req_handler");
                        return DUMP_ERROR;
                    }
                }
                CHNG_FREQ_SetArmClk(s_dump_record_chng_freq_req_handler,FREQ_INDEX_AAC);
            }

            ptCurPosInfo->bEnableRecording = SCI_TRUE;
            return DUMP_NO_ERROR;
        }
    }
    else
    {
        if(!(ptCurPosInfo->bEnableRecording))
        {
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile the data of this pos:%d has not been recording",
                uiPosIndex);
            return DUMP_NO_ERROR;
        }

        if(SCI_NULL == ptCurPosInfo->hDebugFileHandle)
        {
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile DebugFileHandle is null");
            return DUMP_NO_ERROR;
        }
        else
        {
            SFS_ERROR_E write_err_code = SFS_NO_ERROR;
            uint32 fs_trans = 0;
            uint32 endFlag[2]={0x55555555, 0x0};
            uint32 endFlag2[2]={0x66666666, 0x0};
            uint32 uiWaitCount = 0;
            BOOLEAN bOutputFinishflag = SCI_FALSE;

            ptCurPosInfo->bEnableRecording = SCI_FALSE;
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile stop, wait start time: %d.", SCI_GetTickCount());
            while(!bOutputFinishflag)//wait for output
            {
                uint32 i = 0;
                bOutputFinishflag = SCI_TRUE;
                for(i=0; i<ptConfigPara->uiCachCount; i++)
                {
                    ptCurCachInfo = (DATADUMP_RECORD_CACH_INFO_T *)&(ptCurPosInfo->ptCachData[i]);
                    if(1 == ptCurCachInfo->uiFlag)
                    {
                        //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile stop flag index:%d is wait for outputing.", i);
                        bOutputFinishflag = SCI_FALSE;
                        break;
                    }
                }
                if(!bOutputFinishflag)
                {
                    //SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile stop flag wait output.");
                    SCI_Sleep(5);
                    uiWaitCount++;
                }

                if(uiWaitCount>100)
                {
                    /*SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile sleep to wait last write file over, time:%d(ms), force to break.",
                        uiWaitCount*5);*/
                    break;
                }
            }
            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile stop, wait stop time: %d.", SCI_GetTickCount());

            ptCurCachInfo = (DATADUMP_RECORD_CACH_INFO_T *)&(ptCurPosInfo->ptCachData[ptCurPosInfo->uiCurIndex]);
            if((0 == ptCurCachInfo->uiFlag)&&(ptCurCachInfo->uiCurWriteCount>0))//output the last inputed info
            {
                ptCurCachInfo->uiFlag = 1;

                write_err_code = SFS_WriteFile(
                            ptCurPosInfo->hDebugFileHandle,
                            ptCurCachInfo->psBuffer,
                            ptCurCachInfo->uiCurWriteCount,
                            &fs_trans,
                            SCI_NULL);

                if(SFS_NO_ERROR == write_err_code)
                {
                    ptCurPosInfo->uiAvailableCount += ptCurCachInfo->uiCurWriteCount;
                }
                else
                {
                    SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile fail to write file, dataSize:%d, index:%d,",
                        ptCurCachInfo->uiCurWriteCount,
                        uiPosIndex);
                }

                ptCurCachInfo->uiFlag = 0;
            }

            SFS_CloseFile(ptCurPosInfo->hDebugFileHandle);

            SCI_FREE(ptCurPosInfo->psTotalBuffer);
            SCI_FREE(ptCurPosInfo->ptCachData);
            ptCurPosInfo->psTotalBuffer = PNULL;
            ptCurPosInfo->ptCachData = PNULL;

            SCI_TRACE_LOW("[DATADUMP] _datadump_EnableDumptoFile stop, index:%d, lost:%d((0x%x)frames), save:%d((0x%x)bytes).",
                uiPosIndex,
                ptCurPosInfo->uiLostFramesCount, ptCurPosInfo->uiLostFramesCount,
                ptCurPosInfo->uiAvailableCount, ptCurPosInfo->uiAvailableCount);

            //change freq
            s_dump_record_freq_change_count--;
            if(0 == s_dump_record_freq_change_count)
            {
                if (CHNG_FREQ_REQ_HANDLER_NULL != s_dump_record_chng_freq_req_handler)
                {
                    CHNG_FREQ_RestoreARMClk(s_dump_record_chng_freq_req_handler);
                }
            }

            return DUMP_NO_ERROR;
        }
    }
}

/*****************************************************************************/
//  Description: This Function is used to get output file name.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL void _datadump_GetFileName(
                                    uint32  uiPosIndex,//point the position to start/stop
                                    uint16 *uifilename
                                    )
{
    char char_file_name[64]= {0};

    if(0 == spDataDumpInfo.suiNvconfig.uicompanynum)//sprd's version
    {
        sprintf(char_file_name, "E:\\PCM\\testfile_%d_%d.wav", spDataDumpInfo.suiEnableDumpIndex, uiPosIndex);
    }
    else if(1 == spDataDumpInfo.suiNvconfig.uicompanynum)//custom's(samsung) mmi
    {
        sprintf(char_file_name, "/Mount/Mmc/PCM/testfile_%d_%d.wav", spDataDumpInfo.suiEnableDumpIndex, uiPosIndex);
    }

    SCI_TRACE_LOW("[DATADUMP] _datadump_GetFileName fileName:%s", char_file_name);
    _datadump_cstowcs(uifilename, char_file_name, (uint16)strlen(char_file_name));

}

/*****************************************************************************/
//  Description: This Function is used to get config from NV.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL DUMP_RESULT_E _datadump_GetConfigFromNV(void)
{
    AUDIO_DEVICE_MODE_TYPE_E    cur_dev_mode =  AUDIO_DEVICE_MODE_HANDFREE;
    AUDIO_NV_ARM_MODE_INFO_T    *ptAudMode = PNULL;
    char *mode_name = NULL;
    uint16 usNvConfigToFiles=0;

    _datadump_Construct();

    ptAudMode = (AUDIO_NV_ARM_MODE_INFO_T *)SCI_ALLOCA(sizeof(AUDIO_NV_ARM_MODE_INFO_T));
    if(PNULL==ptAudMode)
    {
        SCI_TRACE_LOW("[DATADUMP] _datadump_GetConfigFromNV, alloc fail, size: %d", sizeof(AUDIO_NV_ARM_MODE_INFO_T));

        return DUMP_NO_ENOUGH_MEMORY;
    }

    mode_name = _datadump_ArmvbNv_GetName(cur_dev_mode);
    if(PNULL==mode_name)
    {
        SCI_TRACE_LOW("[DATADUMP] _datadump_GetConfigFromNV get mode_name fail, mode:%d", cur_dev_mode);
        SCI_FREE(ptAudMode);
        return DUMP_ERROR;
    }

    if(AUDIO_NV_ARM_NO_ERROR != AUDIONVARM_GetModeParam((const char *) mode_name, (AUDIO_NV_ARM_MODE_INFO_T *)(ptAudMode)))
    {
        SCI_FREE(ptAudMode);
        SCI_TRACE_LOW("[DATADUMP] _datadump_GetConfigFromNV failed to get mode param.");
        return DUMP_ERROR;
    }

    usNvConfigToFiles = ptAudMode->tAudioNvArmModeStruct.reserve[63];
    if((usNvConfigToFiles&0x3000) == 0x0000)//bit13_bit12: 0b00, output to file*/
    {
        spDataDumpInfo.suiNvconfig.uiDumpOutFlag = 0; //file
        if(0x0==(usNvConfigToFiles&0xc000))//bit15_bit14: 0b00, spreadtrum
        {
            spDataDumpInfo.suiNvconfig.uicompanynum = 0;
        }
        else if(0x4000==(usNvConfigToFiles&0xc000))//bit15_bit14: 0b01, custom's(samsung) mmi
        {
            spDataDumpInfo.suiNvconfig.uicompanynum = 1;//spred_customer1:samsung
        }
    }
    else if((usNvConfigToFiles&0x3000) == 0x1000)//bit13_bit12: 0b01, output to arm log
    {
        spDataDumpInfo.suiNvconfig.uiDumpOutFlag = 1; //armlog
    }
    spDataDumpInfo.suiNvconfig.uiposflag = (usNvConfigToFiles&0x0FFF);//bit11_bit0: positon(11 - 0)

    usNvConfigToFiles = ptAudMode->tAudioNvArmModeStruct.reserve[61];
    spDataDumpInfo.suiNvconfig.uiposflag |= ((usNvConfigToFiles&0xFFFF)<<12);//bit15_bit0: positon(27 - 12)

    SCI_TRACE_LOW("[DATADUMP] _datadump_GetConfigFromNV flag:0x%x, company_num:0x%x, position_flag:0x%x",
        spDataDumpInfo.suiNvconfig.uiDumpOutFlag,
        spDataDumpInfo.suiNvconfig.uicompanynum,
        spDataDumpInfo.suiNvconfig.uiposflag);

    SCI_FREE(ptAudMode);

    return DUMP_NO_ERROR;
}

/*****************************************************************************/
//  Description: This Function is used to get mode name by type
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
LOCAL char * _datadump_ArmvbNv_GetName(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode)
{
    NV_AUDIO_ARM_E arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
    switch(aud_dev_mode)
    {
        case      AUDIO_DEVICE_MODE_HANDHOLD:
        arm_nv_id = NV_AUDIO_ARM_HANDSET_E;
        break;
        case      AUDIO_DEVICE_MODE_HANDFREE:
        arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
        break;
        case      AUDIO_DEVICE_MODE_EARPHONE:
        arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
        break;
        case      AUDIO_DEVICE_MODE_EARFREE:
        arm_nv_id = NV_AUDIO_ARM_EARFREE_E;
        break;
        case      AUDIO_DEVICE_MODE_TVOUT:
        arm_nv_id = NV_AUDIO_ARM_HEADSET_E;
        break;
        case      AUDIO_DEVICE_MODE_BLUEPHONE:
        arm_nv_id = NV_AUDIO_ARM_BTHS_E;
        break;
        default:
        arm_nv_id = NV_AUDIO_ARM_HANDSFREE_E;
        break;
    }
    return AUDIONVARM_GetAudioModeNameById(arm_nv_id);
}

/*****************************************************************************/
//  Description: This Function is used to set config data
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC DUMP_RESULT_E Datadump_SetConfigPara(
                                            uint32 uiPosindex,//bit_0:pos_0..... true:set,false:not
                                            void* puiParamStruct
                                            )
{
    DATADUMP_CONFIG_PARAM_T *tPospara = (DATADUMP_CONFIG_PARAM_T *)
        &spDataDumpInfo.suiConfigPara[uiPosindex];
    DATADUMP_CONFIG_DUMP_DATA_PARAM_T *param=
        (DATADUMP_CONFIG_DUMP_DATA_PARAM_T *)puiParamStruct;

    if(PNULL == puiParamStruct)
    {
        SCI_TRACE_LOW("[DATADUMP] _datadump_ConfigRecordDebugData input param error !!!");
        return DUMP_PARAM_ERROR;
    }
    else
    {
        //uint32 i = 0;

        if(tPospara->uiFrameBytes||tPospara->uiCachCount||tPospara->pDumpDataCallbackFunc)
        {
            SCI_TRACE_LOW("[DATADUMP] _datadump_ConfigRecordDebugData data is dumping, can't reConfig parameters");
            return DUMP_ERROR;
        }

        if(0 != param->uiFrameBytes)
        {
            tPospara->uiFrameBytes = param->uiFrameBytes;
        }
        if(0 != param->uiCachCount)
        {
            tPospara->uiCachCount = param->uiCachCount;
        }
        if(0 != param->pDumpDataCallbackFunc)
        {
            tPospara->pDumpDataCallbackFunc = param->pDumpDataCallbackFunc;
        }

        SCI_TRACE_LOW("[DATADUMP] _datadump_ConfigRecordDebugData exe successfully, frameBtyesOnce:%d, cachCount:%d, callback:0x%x.",
            param->uiFrameBytes, param->uiCachCount, param->pDumpDataCallbackFunc);

        return DUMP_NO_ERROR;
    }
}

/*****************************************************************************/
//  Description: set config from NV
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC DUMP_RESULT_E Datadump_ConfigFromNv(void)
{
    DUMP_RESULT_E result = DUMP_NO_ERROR;
    uint32 i=0;

    result = _datadump_GetConfigFromNV();
    if(DUMP_NO_ERROR != result)
    {
        SCI_TRACE_LOW("[DATADUMP] Datadump_ConfigFromNv fail to get config from NV!!!");
        return result;
    }

    if(DATA_DUMP_TO_FILE == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
    {
        spDataDumpInfo.suiEnableDumpIndex++;
    }

    for(i=0; i<DUMP_DATA_POSITION_MAX_COUNT; i++)
    {
        if((0x1<<i)==(spDataDumpInfo.suiNvconfig.uiposflag&(0x1<<i)))
        {
            _datadump_initPosConfigPara(i);
            _datadump_EnablePointedPos(TRUE, i, PNULL);
        }
        else
        {
            _datadump_EnablePointedPos(FALSE, i, PNULL);
            _datadump_resetPosConfigPara(i);
        }
    }

    return result;
}

/*****************************************************************************/
//  Description:    This Function is used to enable position.
//  Author:         yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC DUMP_RESULT_E Datadump_Enable(
                                        BOOLEAN bOn,
                                        uint32  uiposflag//pos flag(bit_0: pos_0 ....) 1:enable
                                        )
{
    DUMP_RESULT_E result = DUMP_NO_ERROR;
    uint32 i = 0;

    {
//        _datadump_Construct();
        result = _datadump_GetConfigFromNV();
        if(DUMP_NO_ERROR != result)
        {
            SCI_TRACE_LOW("[DATADUMP] Datadump_Enable fail to get config from NV!!!");
            return result;
        }
    }

    if(0 == uiposflag)
    {
        return DUMP_PARAM_ERROR;
    }

    if(bOn)
    {
        if(DATA_DUMP_TO_FILE == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
        {
            spDataDumpInfo.suiEnableDumpIndex++;
        }
        uiposflag &= spDataDumpInfo.suiNvconfig.uiposflag;
        for(i=0; i<DUMP_DATA_POSITION_MAX_COUNT; i++)
        {
            if((0x1<<i)==(uiposflag&(0x1<<i)))
            {
                _datadump_initPosConfigPara(i);
                result = _datadump_EnablePointedPos(bOn, i, PNULL);
            }
        }
    }
    else
    {
        for(i=0; i<DUMP_DATA_POSITION_MAX_COUNT; i++)
        {
            if((0x1<<i)==(uiposflag&(0x1<<i)))
            {
                result = _datadump_EnablePointedPos(bOn, i, PNULL);
                _datadump_resetPosConfigPara(i);
            }
        }
    }
    return result;
}

/*****************************************************************************/
//  Description: output data to file/armlog
//  Author:      yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void Datadump_Output(
                                uint32 uiPosIndex, //position index
                                uint16 *srcData,
                                uint32 uiDataLenByBytes //input bytes once(uint: bytes)
                                )
{
    DATADUMP_RECORD_DEBUG_DATA_INFO_T *ptCurPosInfo = (DATADUMP_RECORD_DEBUG_DATA_INFO_T *)
        &(spDataDumpInfo.stRecordDebugDataInfo[uiPosIndex]);
    DATADUMP_RECORD_CACH_INFO_T *ptCurCachInfo = (DATADUMP_RECORD_CACH_INFO_T *)
        &(ptCurPosInfo->ptCachData[ptCurPosInfo->uiCurIndex]);
    uint32 uiCachBytesCount = 0;
    uint32 uiFrameBytesMax = spDataDumpInfo.suiConfigPara[uiPosIndex].uiFrameBytes;

    if(uiPosIndex >=DUMP_DATA_POSITION_MAX_COUNT)
    {
        SCI_TRACE_LOW("[DATADUMP] Datadump_Output input para error, uiRecordPos(%d, min:0, max:%d) is invalid.",
            uiPosIndex, DUMP_DATA_POSITION_MAX_COUNT - 1);
        return;
    }
    if(PNULL == ptCurPosInfo->psTotalBuffer)
    {
        SCI_TRACE_LOW("[DATADUMP] Datadump_Output the index %d hasn't enable.", uiPosIndex);
        return;
    }

    if(!(ptCurPosInfo->bEnableRecording))
    {
        //SCI_TRACE_LOW("[DATADUMP] Datadump_Output the pos index:%d not recording", uiPosIndex);
        return;
    }

    if(DATA_DUMP_TO_FILE == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
    {
        uiCachBytesCount = DUMP_DEBUG_DATA_FILE_BUF_SIZE - DUMP_DEBUG_DATA_FILE_BUF_SIZE%uiFrameBytesMax;
        if(SCI_NULL == ptCurPosInfo->hDebugFileHandle)
        {
            SCI_TRACE_LOW("[DATADUMP] Datadump_Output fail Debugfilehandle is null");
            return;
        }
    }
    else if(DATA_DUMP_TO_ARM_LOG == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
    {
        uiCachBytesCount = spDataDumpInfo.suiConfigPara[uiPosIndex].uiSampleOnce;
    }

    if(ptCurCachInfo->uiFlag == 1)
    {
        ptCurPosInfo->uiLostFramesCount++;
        SCI_TRACE_LOW("[DATADUMP] Datadump_Output modem index:%d, lose one frame, total:%d frames until now, time:%d(ms).",
            uiPosIndex, ptCurPosInfo->uiLostFramesCount, SCI_GetTickCount());
    }
    else
    {
        BOOLEAN bcachfull = FALSE;
        if(ptCurCachInfo->uiCurWriteCount + uiDataLenByBytes > uiCachBytesCount)
        {
            uint32 uivalidsize = uiCachBytesCount - ptCurCachInfo->uiCurWriteCount;
            uint32 uinexbufindex = (ptCurPosInfo->uiCurIndex + 1)%(spDataDumpInfo.suiConfigPara[uiPosIndex].uiCachCount);
            //just copy the valid numer of bytes
            SCI_MEMCPY(
                (void *)(ptCurCachInfo->psBuffer + ptCurCachInfo->uiCurWriteCount),
                (void *)srcData,
                uivalidsize);
            ptCurCachInfo->uiCurWriteCount += uivalidsize;
            bcachfull = TRUE;
            if(1 != ptCurPosInfo->ptCachData[uinexbufindex].uiFlag)
            {
                DATADUMP_RECORD_CACH_INFO_T *ptNexCachInfo = (DATADUMP_RECORD_CACH_INFO_T *)
                    &(ptCurPosInfo->ptCachData[uinexbufindex]);
                uint32 uiremsize = uiDataLenByBytes - uivalidsize;

                uiremsize = (uiCachBytesCount>uiremsize)?uiremsize:uiCachBytesCount;
                SCI_MEMCPY(
                    (void *)(ptNexCachInfo->psBuffer),
                    (void *)(srcData+uivalidsize),
                    uiremsize);
                ptNexCachInfo->uiCurWriteCount = uiremsize;
            }
        }
        else
        {
            SCI_MEMCPY(
                (void *)(ptCurCachInfo->psBuffer + ptCurCachInfo->uiCurWriteCount),
                (void *)srcData,
                uiDataLenByBytes);

            ptCurCachInfo->uiCurWriteCount += uiDataLenByBytes;
            if(uiCachBytesCount == ptCurCachInfo->uiCurWriteCount)
            {
                bcachfull = TRUE;
            }
/*            if(DATA_DUMP_TO_FILE == spDataDumpInfo.suiNvconfig.uiDumpOutFlag)
            {
                if((ptCurCachInfo->uiCurWriteCount + uiFrameBytesMax) > uiCachBytesCount)//rest buffer is less than one frame
                {
                    bcachfull = TRUE;
                }
            }*/
        }

        if(bcachfull)
        {
            DATADUMP_SIGNAL_T *ptWriteFileSig = PNULL;

            ptCurCachInfo->uiFlag = 1;

/*            SCI_TRACE_LOW("[DATADUMP] Datadump_Output index:%d, start to save to file, time:%d(ms).",
                uiPosIndex, SCI_GetTickCount());*/

            ptWriteFileSig = (DATADUMP_SIGNAL_T *)SCI_ALLOC_APP(sizeof(DATADUMP_SIGNAL_T));
            SCI_ASSERT(PNULL != ptWriteFileSig);
            ptWriteFileSig->SignalCode = DUMP_WRITE_DATA_TO_FILE_SIG;
            ptWriteFileSig->SignalSize = sizeof(DATADUMP_SIGNAL_T);
            ptWriteFileSig->Sender = SCI_IdentifyThread();
            ptWriteFileSig->uiPosIndex = uiPosIndex;
            ptWriteFileSig->spSrcData = (uint16 *)(ptCurCachInfo->psBuffer);
            ptWriteFileSig->uiDataLen = ptCurCachInfo->uiCurWriteCount;
            ptWriteFileSig->uiDataIndexOut = ptCurPosInfo->uiCurIndex;

            //assert this singal send to decode task, other way must assert
            SCI_SendSignal((xSignalHeader)ptWriteFileSig, stDataDumpThreadId);
            ptWriteFileSig = PNULL;

            ptCurPosInfo->uiCurIndex++;
            if(spDataDumpInfo.suiConfigPara[uiPosIndex].uiCachCount == ptCurPosInfo->uiCurIndex)
            {
                ptCurPosInfo->uiCurIndex = 0;
            }
        }
    }
}


#ifdef CVSRCD_ENABLE
// only for cvs record debug
LOCAL DUMP_RESULT_E _datadump_Config_CVSRCD(
    uint32 is_dump_to_file,
    uint32 uiposflag)
{
    if (is_dump_to_file) {
        spDataDumpInfo.suiNvconfig.uiDumpOutFlag = 0; //file
        spDataDumpInfo.suiNvconfig.uicompanynum = 0;
    } else {
        spDataDumpInfo.suiNvconfig.uiDumpOutFlag = 1; //armlog
    }
    spDataDumpInfo.suiNvconfig.uiposflag = uiposflag;

    SCI_TRACE_LOW("[DATADUMP][_datadump_Config_CVSRCD] flag:0x%x, company_num:"
        "0x%x, position_flag:0x%x",
        spDataDumpInfo.suiNvconfig.uiDumpOutFlag,
        spDataDumpInfo.suiNvconfig.uicompanynum,
        spDataDumpInfo.suiNvconfig.uiposflag);

    return DUMP_NO_ERROR;
}

PUBLIC DUMP_RESULT_E Datadump_Enable_CVSRCD(
    BOOLEAN bOn,
    uint32 is_dump_to_file,
    uint32 uiposflag)
{
    DUMP_RESULT_E result = DUMP_NO_ERROR;
    uint32 i = 0;

    _datadump_Construct();
    result = _datadump_Config_CVSRCD(is_dump_to_file, uiposflag);
    if (DUMP_NO_ERROR != result) {
        SCI_TRACE_LOW("[DATADUMP][Datadump_Enable_CVSRCD] "
            "Datadump_Enable fail to get config!!!");
        return result;
    }

    SCI_TRACE_LOW("[DATADUMP][Datadump_Enable_CVSRCD] "
        "bOn=%d; is_dump_to_file=%d; uiposflag=%08x",
        bOn, is_dump_to_file, uiposflag);

    if (bOn) {
        if (DATA_DUMP_TO_FILE == spDataDumpInfo.suiNvconfig.uiDumpOutFlag) {
            spDataDumpInfo.suiEnableDumpIndex++;
        }
        uiposflag &= spDataDumpInfo.suiNvconfig.uiposflag;
        for (i=0; i<DUMP_DATA_POSITION_MAX_COUNT; i++) {
            if ((0x1<<i)==(uiposflag&(0x1<<i))) {
                _datadump_initPosConfigPara(i);
                result = _datadump_EnablePointedPos(bOn, i, PNULL);
            }
        }
    } else {
        for (i=0; i<DUMP_DATA_POSITION_MAX_COUNT; i++) {
            if ((0x1<<i)==(uiposflag&(0x1<<i))) {
                result = _datadump_EnablePointedPos(bOn, i, PNULL);
                _datadump_resetPosConfigPara(i);
            }
        }
    }
    return result;
}

PUBLIC void Datadump_Output_CVSRCD(
    uint32 uiPosIndex,          //position index
    uint16 *srcData,
    uint32 uiDataLenByBytes     //input bytes once(uint: bytes)
    )
{
    Datadump_Output(uiPosIndex, srcData, uiDataLenByBytes);
}
#endif

