/*
云芯平台
*/
#include "ai_app.h"
#include "mmisrvrecord_export.h"
#include "ai_api.h"
#include "ai_voice.h"


#ifdef TULING_AI_SUPPORT

LOCAL void AI_Mp3BufList_FreeHead();
LOCAL void AI_Mp3BufList_FreeAll();
LOCAL void AI_Mp3FixBuf_FreeAll();

LOCAL void AI_Mp3DirectDataFree();
PUBLIC void AI_Voice_Rcv_Mp3_PlayBufThreadStop();

LOCAL void AI_StopNextMp3PlayTimer();
LOCAL void AI_StartNextMp3PlayTimer();

LOCAL void NotifyStartNextBufferDownload();



#define AI_VOICE_RECORD_TIME_OUT 10

static BOOLEAN s_voc_is_play_all = FALSE;
static uint8 s_voc_play_err_num = 0;
static BOOLEAN           s_ai_voc_is_playing = FALSE;
static BOOLEAN           s_ai_voc_is_recording = FALSE;
static uint16 s_ai_voc_play_vol = MMISRVAUD_VOLUME_LEVEL_MAX - 2;
static uint16 s_ai_voc_rcv_idx = 0;

Mp3BufList_T* ai_mp3_buf_list_ptr = NULL;
Mp3BufList_T* ai_mp3_buf_list_play_ptr = NULL;

static Mp3Data_T ai_mp3_data = {0}; // for save all buf
static uint8 ai_next_mp3_play_timer_id = 0;
static uint ai_next_mp3_check_count = 0; //check 3 time ,then release


static Mp3FixBufHolder_T ai_mp3_fix_buf_holder = {0};


static BLOCK_ID m_ai_voc_rcv_mp3_task_id = SCI_INVALID_BLOCK_ID;
static BOOLEAN ai_is_mp3_playing = FALSE;
static BOOLEAN ai_is_inserting_mp3 = FALSE;
static BOOLEAN ai_is_waiting_next_mp3 = FALSE;




//BOOLEAN AI_Voice_Play_Valid(void);
#ifdef TOUCH_PANEL_SUPPORT
//extern void  ZdtApp_AI_VocMenu_DrawStatus(AI_APP_T * pMe,AIVocRspTypeStatus type, uint16 err_id);
//extern void  ZdtApp_AI_VocMenu_RecordRsp(AI_APP_T * pMe,AIVocRspTypeStatus type, uint16 err_id);
#endif

static uint8 AI_Voice_GetMemStatus(AI_APP_T * pMe)
{
    BOOLEAN is_full = AI_File_MemFull(FALSE);
    if(is_full)
    {
        return 0;
    }
    return 1;
}
/*
BOOLEAN AI_Voice_Allow_Receive(AI_APP_T * pMe)
{
    BOOLEAN is_full = AI_File_MemFull(FALSE);
    if(is_full)
    {
        return FALSE;
    }
    return TRUE;
}

BOOLEAN AI_Voice_SendAlert(AI_APP_T * pMe,uint16 wParam)
{
    #if 0
    AI_LOG("AI__VOICE AI_Voice_SendAlert wParam=%d",wParam);
    if(wParam == 0) //成功
    {
        AECHAR	pszTitle[MAXSTRLEN] = {0x0};
        //发送成功!53D1 9001 6210 529F 0021 
        pszTitle[0] = 0x53D1;
        pszTitle[1] = 0x9001;
        pszTitle[2] = 0x6210;
        pszTitle[3] = 0x529F;
        pszTitle[4] = 0x0021;
        openBackLight(TRUE);
        
        pMe->m_nPuWndFld.handleId = NULL;
        pMe->m_nPuWndFld.iconType = HINT_ICON;		   
        pMe->m_nPuWndRef = ZDT_POP_TYPE_NOCARD;
        IPuWnd_DisplayStringWithTimeoutOk(pMe->m_pPuWnd, pszTitle,500, &pMe->m_nPuWndFld);
    }
    else
    {
        AECHAR	pszTitle[MAXSTRLEN] = {0x0};
        //发送失败!53D1 9001 5931 8D25 0021 
        pszTitle[0] = 0x53D1;
        pszTitle[1] = 0x9001;
        pszTitle[2] = 0x5931;
        pszTitle[3] = 0x8D25;
        pszTitle[4] = 0x0021;
        #if 0
        if(wParam == 2)
        {
            //网络错误7F51 7EDC 9519 8BEF
            pszTitle[5] = 0x7F51;
            pszTitle[6] = 0x7EDC;
            pszTitle[7] = 0x9519;
            pszTitle[8] = 0x8BEF;
        }
        else if(wParam == 3)
        {
            //文件出错6587 4EF6 51FA 9519 
            pszTitle[5] = 0x6587;
            pszTitle[6] = 0x4EF6;
            pszTitle[7] = 0x51FA;
            pszTitle[8] = 0x9519;
        }
        #endif
        openBackLight(TRUE);
        pMe->m_nPuWndFld.handleId = NULL;
        pMe->m_nPuWndFld.iconType = HINT_ICON;		   
        pMe->m_nPuWndRef = ZDT_POP_TYPE_NOCARD;
        IPuWnd_DisplayStringWithTimeoutOk(pMe->m_pPuWnd, pszTitle,500, &pMe->m_nPuWndFld);
    }
    #endif
}


static BOOLEAN AI_VOC_RcvFileNameBuild(AI_APP_T * pMe,AI_VOC_RCV_FILE_DATA_T * pRcvData)
{
    uint32 time_ms = AI_Get_TimeSecond();
    uint16 voc_idx = pRcvData->voc_idx;
    AI_LOG("AI__VOICE AI_VOC_RcvFileNameBuild voc_idx=%d,time=%ld",voc_idx,time_ms);

    SCI_MEMSET(pMe->m_ai_pFileRcvVocName, 0, MAX_AI_VOC_FILENAME_SIZE+1);

    sprintf( pMe->m_ai_pFileRcvVocName,
             "%s\\R%d_%ld.amr",
             AI_VOICE_RCV_DIR,voc_idx,time_ms);
    return TRUE;
}*/

//yangyu add begin
#define MAX_REMAIN_SIZE  (120*1024) //160K
LOCAL BOOLEAN AI_IsMemOverLimit(BOOLEAN is_tf)
{
    uint32          free_space_high     = 0;
    uint32          free_space_low      = MAX_REMAIN_SIZE;//102400;
    uint32          cur_free_space_high = 0;
    uint32          cur_free_space_low  = 0;
    MMIFILE_ERROR_E file_ret            = SFS_ERROR_NONE;
    MMIFILE_DEVICE_E sd_type             = MMI_DEVICE_UDISK;

    if(is_tf)
    {
       sd_type = MMI_DEVICE_SDCARD;
    }
    SCI_TRACE_LOW("yyu AI_IsMemOverLimit  sd_type=%d" ,sd_type);
    if(MMIAPIFMM_GetDeviceStatus (MMIAPIFMM_GetDevicePath (sd_type), MMIAPIFMM_GetDevicePathLen (sd_type)))
    {
        file_ret = SFS_GetDeviceFreeSpace ( (uint16 *) MMIAPIFMM_GetDevicePath (sd_type),
                                            &cur_free_space_high,
                                            &cur_free_space_low);
        SCI_TRACE_LOW("yyu AI_IsMemOverLimit cur_high=%ld,cur_low=%ld,high=%ld,low=%ld", cur_free_space_high, cur_free_space_low, free_space_high,free_space_low);
        if (file_ret == SFS_ERROR_NONE)
        {
            if (cur_free_space_high > free_space_high //高位大于肯定大于
                    || ( (cur_free_space_high == free_space_high) //高位相等，低位大于
                         && (cur_free_space_low > free_space_low))
               )
            {
                return FALSE;
            }
        }
    }
        
    return TRUE;
}

//yangyu end


static BOOLEAN AI_VOC_SendFileNameBuild(AI_APP_T * pMe)
{
    uint32 time_ms = AI_Get_TimeSecond();
    //yangyu add later
    if(AI_IsMemOverLimit(FALSE))
    {

        AI_File_RemoveDirFile(AI_VOICE_SEND_DIR);
    }
    //yangyu end    
    
    AI_File_Remove(pMe->m_ai_pFileSendVocName);//yangyu add for bug, delete old  record file
    SCI_MEMSET(pMe->m_ai_pFileSendVocName, 0, MAX_AI_VOC_FILENAME_SIZE+1);
    sprintf( pMe->m_ai_pFileSendVocName,
             "%s\\S%d_%ld.amr",
             AI_VOICE_SEND_DIR,pMe->m_ai_voc_data_idx+1,time_ms);
    AI_LOG("AI__VOICE AI_Voice_Send_BuildFileName %s",pMe->m_ai_pFileSendVocName);
    SCI_MEMSET(pMe->m_ai_pFileSendVocFullName, 0, (MAX_AI_VOC_FILENAME_SIZE+1)*2);
    pMe->m_ai_pFileSendVocFullName_len = GUI_GBToWstr(pMe->m_ai_pFileSendVocFullName, (const uint8*)pMe->m_ai_pFileSendVocName, SCI_STRLEN(pMe->m_ai_pFileSendVocName));
    
    AI_File_Remove(pMe->m_ai_pFileSendVocName);
    return TRUE;
}

BOOLEAN AI_Voice_FileSendAllRemove(AI_APP_T * pMe)
{
    if(pMe->m_ai_eRecorderStatus == AI_RECORDER_STOP
        && pMe->m_ai_voc_is_sending == FALSE
        )
    {
        AI_File_RemoveDirFile(AI_VOICE_SEND_DIR);
    }
    return TRUE;
}
/*
BOOLEAN AI_Voice_FileRcvAllRemove(AI_APP_T * pMe)
{
    if(pMe->m_ai_eRecorderStatus == AI_RECORDER_STOP
        && pMe->m_ai_voc_is_playing == FALSE
        )
    {
        AI_File_RemoveDirFile(AI_VOICE_RCV_DIR);
        pMe->m_ai_pDB_vocrcv_last_CurList.list_id = 0;
    }
    return TRUE;
}

static BOOLEAN AI_VOC_Send_Link_Add(AI_APP_T *pMe, uint8 * filename, uint32 name_len)
{
    AI_VOC_SEND_NODE_T  * p1 = NULL;
    AI_VOC_SEND_NODE_T  * p2 = NULL;
    uint32 len = name_len;

    if(len == 0)
    {
        return FALSE;
    }
    
    p1=(AI_VOC_SEND_NODE_T *)SCI_ALLOC_APPZ(sizeof(AI_VOC_SEND_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    
    p1->data.pFilename = (uint8 *)SCI_ALLOC_APPZ(len+1);
    if(p1->data.pFilename == NULL)
    {
        SCI_FREE(p1);
        return FALSE;
    }
    
    SCI_MEMCPY(p1->data.pFilename,filename,name_len);
    p1->data.pFilename[len] = 0;

    p1->data.len = len;
    p1->next_ptr = NULL;

    if(pMe->m_ai_voc_send_link_head == NULL)
    {
        pMe->m_ai_voc_send_link_head = p1;
    }
    else
    {
        p2 = pMe->m_ai_voc_send_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  AI_VOC_Send_Link_Get(AI_APP_T *pMe,AI_VOC_SEND_DATA_T * pVocSendData)
{
    AI_VOC_SEND_NODE_T  * p1 = NULL;
    
    p1 = pMe->m_ai_voc_send_link_head;
    
    if(p1 != NULL)
    {
      *pVocSendData = p1->data;
       return TRUE;
    }
        
    return FALSE;
}

static BOOLEAN  AI_VOC_Send_Link_DelOne(AI_APP_T *pMe)
{
    AI_VOC_SEND_NODE_T  * p1 = NULL;
    AI_VOC_SEND_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    
    p2 = p1 = pMe->m_ai_voc_send_link_head;
    
    while(p1 != NULL)
    {
        if(p1 == pMe->m_ai_voc_send_link_head)
        {
            pMe->m_ai_voc_send_link_head = p1->next_ptr;
        }
        else
        {
            p2->next_ptr = p1->next_ptr;
        }
        res = TRUE;
        if(p1->data.pFilename != NULL)
        {
            SCI_FREE(p1->data.pFilename);
        }
        SCI_FREE(p1);
        break;
    }
        
    return res;
}

BOOLEAN  AI_VOC_Send_Link_DelAll(AI_APP_T *pMe)
{
    AI_VOC_SEND_NODE_T  * p1 = NULL;
    AI_LOG("AI__VOICE AI_VOC_SendFile_Link_DelAll");
    while(pMe->m_ai_voc_send_link_head != NULL)
    {
        p1 = pMe->m_ai_voc_send_link_head;
        pMe->m_ai_voc_send_link_head = p1->next_ptr;
        if(p1->data.pFilename!= 0)
        {
            AI_File_Remove((char *)p1->data.pFilename);
            SCI_FREE(p1->data.pFilename);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

static BOOLEAN AI_VOC_RcvFile_Link_Add(AI_APP_T *pMe, AI_VOC_RCV_FILE_DATA_T * pRcvData)
{
    AI_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    AI_VOC_RCV_FILE_NODE_T  * p2 = NULL;
    
    p1=(AI_VOC_RCV_FILE_NODE_T *)SCI_ALLOC_APPZ(sizeof(AI_VOC_RCV_FILE_NODE_T));
    if(p1 == NULL)
    {
        return FALSE;
    }
    SCI_MEMSET(p1,0,sizeof(AI_VOC_RCV_FILE_NODE_T));

    p1->data = *pRcvData;
    p1->next_ptr = NULL;
    if(pMe->m_ai_voc_rcvfile_link_head == NULL)
    {
        pMe->m_ai_voc_rcvfile_link_head = p1;
    }
    else
    {
        p2 = pMe->m_ai_voc_rcvfile_link_head;
        while(p2->next_ptr != NULL)
        {
            p2 = p2->next_ptr;
        }
        p2->next_ptr = p1;
    }
    
    return TRUE;
}

static BOOLEAN  AI_VOC_RcvFile_Link_GetByIdx(AI_APP_T *pMe,AI_VOC_RCV_FILE_DATA_T * pData,uint16 voc_idx,uint8 pack_idx)
{
    AI_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    AI_VOC_RCV_FILE_NODE_T  * p2 = NULL;
    BOOLEAN res = FALSE;
    if(pData == NULL)
    {
        return FALSE;
    }
    
    p2 = p1 = pMe->m_ai_voc_rcvfile_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data.voc_idx == voc_idx && p1->data.pack_idx == pack_idx)
        {     
            * pData = p1->data;
            res = TRUE;
            break;
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
        
    return res;
}

static BOOLEAN  AI_VOC_RcvFile_Link_DelByVocIdx(AI_APP_T *pMe,uint16 voc_idx)
{
    AI_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    AI_VOC_RCV_FILE_NODE_T  * p2 = NULL;
    AI_VOC_RCV_FILE_NODE_T  * p3 = NULL;
    BOOLEAN res = FALSE;
    
    p3 = p2 = p1 = pMe->m_ai_voc_rcvfile_link_head;
    
    while(p1 != NULL)
    {
        p2 = p1;
        p1 = p2->next_ptr;
        if(p2->data.voc_idx == voc_idx)
        {     
            if(p2 == pMe->m_ai_voc_rcvfile_link_head)
            {
                pMe->m_ai_voc_rcvfile_link_head = p1;
                p3 = p1;
            }
            else
            {
                p3->next_ptr = p1;
            }
            
            if(p2->data.voc_data != NULL)
            {
                SCI_FREE(p2->data.voc_data);
            }
            SCI_FREE(p2);
            res = TRUE;
        }
        else
        {
            p3 = p2;
        }
    }
        
    return res;
}

BOOLEAN  AI_VOC_RcvFile_Link_DelAll(AI_APP_T *pMe)
{
    AI_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    AI_LOG("AI__VOICE AI_VOC_RcvFile_Link_DelAll");
    while(pMe->m_ai_voc_rcvfile_link_head != NULL)
    {
        p1 = pMe->m_ai_voc_rcvfile_link_head;
        pMe->m_ai_voc_rcvfile_link_head = p1->next_ptr;
        if(p1->data.voc_data != 0)
        {
            SCI_FREE(p1->data.voc_data);
        }
        SCI_FREE(p1);
    }
    return TRUE;
}

static uint8  AI_VOC_RcvFile_Link_isValid(AI_APP_T *pMe,uint16 voc_idx)
{
    AI_VOC_RCV_FILE_NODE_T  * p1 = NULL;
    AI_VOC_RCV_FILE_NODE_T  * p2 = NULL;
    uint8 valid_pack_idx[3];
    uint8 pack_sum = 0;
    
    p2 = p1 = pMe->m_ai_voc_rcvfile_link_head;
    
    while(p1 != NULL)
    {
        if(p1->data.voc_idx == voc_idx)
        {     
            pack_sum = p1->data.pack_sum;
            if(p1->data.pack_idx == 1)
            {
                valid_pack_idx[0] = 1;
            }
            else if(p1->data.pack_idx == 2)
            {
                valid_pack_idx[1] = 1;
            }

            if(p1->data.pack_sum == p1->data.pack_idx)
            {
                valid_pack_idx[2] = 1;
                if(p1->data.pack_sum == 1)
                {
                    valid_pack_idx[1] = 1;
                }
            }
        }
        p2 = p1;
        p1 = p2->next_ptr;
    }
    if(pack_sum > 0 && valid_pack_idx[0] == 1 && valid_pack_idx[1] == 1 && valid_pack_idx[2] == 1)
    {
        return pack_sum;
    }
    else
    {
        return 0;
    }
}

int  AI_VOC_RcvFile_Save(AI_APP_T *pMe,AI_VOC_RCV_FILE_DATA_T * pRcvData)
{
    int res = AEE_SUCCESS;
    MMIFILE_HANDLE file_handle;
    uint8 pack_sum = 0;
    uint16 i = 0;
    
    AI_VOC_RCV_FILE_DATA_T FileData = {0};
    
    pack_sum = AI_VOC_RcvFile_Link_isValid(pMe,pRcvData->voc_idx);
    if(pack_sum > 0)
    {
        AI_VOC_RcvFileNameBuild(pMe,pRcvData);
        AI_File_Remove(pMe->m_ai_pFileRcvVocName);
        file_handle = AI_File_Open(pMe->m_ai_pFileRcvVocName,FALSE);
        if(file_handle == SFS_INVALID_HANDLE)
        {
            AI_LOG("AI__VOICE AI Save VocRcvFile ERR Open voc_idx=%d",pRcvData->voc_idx);
            return AEE_EUNABLETOLOAD;
        }
        for(i = 0; i < pack_sum; i++)
        {
            if(AI_VOC_RcvFile_Link_GetByIdx(pMe,&FileData,pRcvData->voc_idx,i+1))
            {
                if(AI_File_Write(file_handle, FileData.voc_data, FileData.voc_data_len)==0)
                {
                    AI_LOG("AI__VOICE AI Save VocRcvFile Err voc_idx=%d,pack_idx=%d",pRcvData->voc_idx,i+1);
                    AI_File_Close(file_handle);
                    return AEE_ENOMEMORY;
                }
            }
        }
        
        AI_File_Close(file_handle);
    }
    else
    {
        res = AEE_EFAILED;
    }
    return res;
}
*/

BOOLEAN AI_Voice_RecordStop_CallBack(AI_APP_T * pMe,AIVocRcdStopErrStatus err_id)
{
    if(AI_VOCRCDSTOP_SUCCESS == err_id)
    {
        MMIZDT_UpdateTalkWin(AI_RECORDER_DISP_RECORD_OK,PNULL);
    }
    else
    {
        if(AI_VOCRCDSTOP_ERR_LOWTIME == err_id)
        {
            MMIZDT_UpdateTalkWin(AI_RECORDER_DISP_RECORD_FAIL,"RStop Err LowTime");
        }
        else
        {
            char tmp_buf[50] = {0};
            sprintf(tmp_buf,"RStop Err %d",err_id);
            MMIZDT_UpdateTalkWin(AI_RECORDER_DISP_RECORD_FAIL,tmp_buf);
        }
    }
        
    return TRUE;
}

BOOLEAN AI_Voice_Record_CallBack(AI_APP_T * pMe,AIVocRecordErrStatus err_code)
{
    if(AI_VOCRECORD_SUCCESS == err_code)
    {
        MMIZDT_UpdateTalkWin(AI_RECORDER_DISP_RECORDING,PNULL);
    }
    else
    {
        MMIZDT_UpdateTalkWinErr(err_code);
    }
    return TRUE;
}
/*
BOOLEAN AI_Voice_Send_CallBack(AI_APP_T * pMe,AIVocSendErrStatus err_code)
{
    if(AI_VOCSEND_SUCCESS == err_code)
    {
        MMIZDT_UpdateTalkWin(AI_RECORDER_DISP_SEND_OK,PNULL);
    }
    else
    {
        char tmp_buf[50] = {0};
        sprintf(tmp_buf,"SendErr:%d",err_code);
        MMIZDT_UpdateTalkWin(AI_RECORDER_DISP_SEND_FAIL,tmp_buf);
    }
    return TRUE;
}
*/

void AI_Voice_RecordStop_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RECORD_TIMEOUT,NULL,0);
}

void AI_Voice_RecordStop_TimerStart(AI_APP_T * pMe)
{
    if(0 != pMe->m_ai_voc_record_stop_time_id)
    {
        MMK_StopTimer(pMe->m_ai_voc_record_stop_time_id);
        pMe->m_ai_voc_record_stop_time_id = 0;
    }
    
    pMe->m_ai_voc_record_stop_time_id = MMK_CreateTimerCallback(10000, 
                                                                        AI_Voice_RecordStop_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void AI_Voice_RecordStop_TimerStop(AI_APP_T * pMe)
{
    if(0 != pMe->m_ai_voc_record_stop_time_id)
    {
        MMK_StopTimer(pMe->m_ai_voc_record_stop_time_id);
        pMe->m_ai_voc_record_stop_time_id = 0;
    }
}

static BOOLEAN AI_Voice_File_Check(const char * file_name)
{
    uint32 buf_len = 0;
    
    if(file_name == NULL)
    {
        return FALSE;
    }

    buf_len = AI_File_GetSize(file_name);
    
    if(buf_len < 19)
    {
        return FALSE;
    }
    
    return TRUE;
}
PUBLIC BOOLEAN AI_Voice_Record_PlayCur(void)
{
    BOOLEAN ret = FALSE;
    MMIRECORD_SRV_RESULT_E srv_result = MMIRECORD_SRV_RESULT_SUCCESS;

    if(!MMIRECORDSRV_IsRecordFileExist(g_ai_app.m_ai_pRecordMedia))
    {
        ret = FALSE;
    }
    else
    {
        srv_result = MMIRECORDSRV_Play(g_ai_app.m_ai_pRecordMedia, 8);

        if(MMIRECORD_SRV_RESULT_SUCCESS == srv_result)
        {
            ret = TRUE;
        }
    }
    return ret;
}

PUBLIC void AI_Voice_Record_HandleEnd(BOOLEAN is_succeed)
{
    AI_APP_T *   pMe = (AI_APP_T *)&g_ai_app;
    AI_LOG("AI__VOICE AI_Voice_Record_HandleEnd is_succeed=%d", is_succeed);
    if(is_succeed)
    {
        AI_LOG("AI__VOICE AI_Voice_Record_HandleEnd Success");
        AI_Voice_RecordStop_TimerStop(pMe);
        if (PNULL != pMe->m_ai_pRecordMedia)
        {
            MMIRECORDSRV_StopRecord(pMe->m_ai_pRecordMedia);
            MMIRECORDSRV_FreeRecordHandle(pMe->m_ai_pRecordMedia);
            pMe->m_ai_pRecordMedia = 0;
        }
        s_ai_voc_is_recording = FALSE;
        pMe->m_ai_eRecorderStatus = AI_RECORDER_STOP;
        if(AI_Voice_File_Check(pMe->m_ai_pFileSendVocName))
        { 
            //if(AI_VOC_Send_Link_Add(pMe,(uint8 *)pMe->m_ai_pFileSendVocName,MAX_AI_VOC_FILENAME_SIZE))
            {
                AI_API_Record_Send();
            }
            AI_Voice_RecordStop_CallBack(pMe,AI_VOCRCDSTOP_SUCCESS);
        }
        else
        {
            AI_File_Remove(pMe->m_ai_pFileSendVocName);
            AI_Voice_RecordStop_CallBack(pMe,AI_VOCRCDSTOP_ERR_LOWTIME);
        }
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_Record_HandleEnd FAIL");
        AI_Voice_RecordStop_TimerStop(pMe);
        if (PNULL != pMe->m_ai_pRecordMedia)
        {
            MMIRECORDSRV_StopRecord(pMe->m_ai_pRecordMedia);
            MMIRECORDSRV_FreeRecordHandle(pMe->m_ai_pRecordMedia);
            pMe->m_ai_pRecordMedia = 0;
        }
        pMe->m_ai_eRecorderStatus = AI_RECORDER_STOP;
        s_ai_voc_is_recording = FALSE;
        AI_File_Remove(pMe->m_ai_pFileSendVocName);
        AI_Voice_RecordStop_CallBack(pMe,AI_VOCRCDSTOP_ERR);
    }
    return;
}

static void AI_Voice_RecordNotify(RECORD_SRV_HANDLE record_srv_handle, 
						  MMIRECORD_SRV_SUPPORT_TYPE_E support_type, 
						  MMIRECORD_SRV_RESULT_E result)
{
    
    AI_LOG("AI__VOICE AI_Voice_RecordNotify support_type=%d, result=%d", support_type, result);

	if (record_srv_handle == g_ai_app.m_ai_pRecordMedia
		&& MMIRECORD_SRV_SUPPORT_RECORD == support_type)
	{			
            AI_LOG("AI__VOICE AI_Voice_RecordNotify RECORD result=%d",result);
            if(MMIRECORD_SRV_RESULT_SUCCESS == result)
            {
                //MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RECORD_SUCCESS,NULL,0);
            }
            else
            {
                //MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RECORD_FAIL,NULL,0);
            }
	}
    
    if (record_srv_handle == g_ai_app.m_ai_pRecordMedia
    && MMIRECORD_SRV_SUPPORT_PLAY == support_type)
    {			
        AI_LOG("AI__VOICE AI_Voice_RecordNotify PLAY result=%d",result);
    }

}

void AI_Voice_Play_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    AI_APP_T *   pMe = (AI_APP_T *)param;
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_PLAYSTOP,NULL,0);
    //if(AI_Voice_Play_Valid())
    {
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_FILEPLAY,NULL,0);
    }
}

void AI_Voice_Play_TimerStart(AI_APP_T * pMe)
{
    if(0 != pMe->m_ai_voc_play_time_id)
    {
        MMK_StopTimer(pMe->m_ai_voc_play_time_id);
        pMe->m_ai_voc_play_time_id = 0;
    }
    
    pMe->m_ai_voc_play_time_id = MMK_CreateTimerCallback(1000, 
                                                                        AI_Voice_Play_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void AI_Voice_Play_TimerStop(AI_APP_T * pMe)
{
    if(0 != pMe->m_ai_voc_play_time_id)
    {
        MMK_StopTimer(pMe->m_ai_voc_play_time_id);
        pMe->m_ai_voc_play_time_id = 0;
    }
}

static BOOLEAN AI_Voice_PlayFileGetLastName(char *pInBuffer,char *pOutBuffer)
{
    uint16 len = 0;
    char* p;
    
    if(pInBuffer == NULL || pOutBuffer == NULL)
    {
        return FALSE;
    }
    
    len = strlen(pInBuffer);
    if(len == 0)
    {
        return FALSE;
    }

    p = strrchr(pInBuffer, '\\');
    if(p == NULL)
    {
        return FALSE;
    }
    p[1] = 0;

    strcpy(pOutBuffer,pInBuffer);
    strcat(pOutBuffer,AI_VOICE_RCV_LAST_FILENAME);
   
    AI_LOG("AI__VOICE AI_Voice_PlayFileGetLastName %s to %s",pInBuffer,pOutBuffer);

    return TRUE;
}
/*
static BOOLEAN AI_Voice_PlayFileRemove(AI_APP_T * pMe)
{
    BOOLEAN res = FALSE;
    uint32 cur_rcv_num = AI_DB_GetVocRcvNum();
    char szLastName[MAX_AI_VOC_FILENAME_SIZE] = {0};
    
    if(cur_rcv_num > 1)
    {
        AI_File_Remove((char *)pMe->m_ai_pDB_vocrcv_CurList.r_name);
        AI_DB_VocRcvDel(pMe,pMe->m_ai_pDB_vocrcv_CurList.list_id);
    }
    else if(cur_rcv_num == 1)
    {
        pMe->m_ai_pDB_vocrcv_last_CurList = pMe->m_ai_pDB_vocrcv_CurList;
        AI_Voice_PlayFileGetLastName(pMe->m_ai_pDB_vocrcv_last_CurList.r_name,szLastName);
        AI_File_Remove(szLastName);
        AI_File_Rename(pMe->m_ai_pDB_vocrcv_CurList.r_name,szLastName);
        SCI_MEMCPY(pMe->m_ai_pDB_vocrcv_last_CurList.r_name,szLastName,MAX_AI_VOC_FILENAME_SIZE);
        pMe->m_ai_pDB_vocrcv_last_CurList.list_id = 1;
        AI_DB_VocRcvDel(pMe,pMe->m_ai_pDB_vocrcv_CurList.list_id);
    }
    return TRUE;
}

static void AI_Voice_Play_HandleEnd(BOOLEAN is_end)
{
    AI_APP_T *   pMe = (AI_APP_T *)&g_ai_app;

    if (is_end )
    {
        s_voc_play_err_num = 0;
        AI_LOG("AI__VOICE AI_Voice_PlayNotify Play DONE");
        if(pMe->m_ai_pPlayMedia)
        {
            MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
            MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
            pMe->m_ai_pPlayMedia = NULL;
        }
        AI_Voice_PlayFileRemove(pMe);
        pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYSTOP;
        pMe->m_ai_voc_is_playing = FALSE;
        if(s_ai_voc_is_playing)
        {               
            if(s_voc_is_play_all)
            {
                 MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_PLAYSTOP,NULL,0);
                if(AI_Voice_Play_Valid() && AI_DB_GetVocRcvNum() > 0)
                {
                    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_FILEPLAY,NULL,0);
                }
            }
            else
            {
                MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_PLAYSTOP,NULL,0);
            }
        }
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_PlayNotify Play ABORT err_num=%d",s_voc_play_err_num);
        if(pMe->m_ai_pPlayMedia)
        {
            MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
            MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
            pMe->m_ai_pPlayMedia = NULL;
        }
        pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYSTOP;
        pMe->m_ai_voc_is_playing = FALSE;
        
        if(s_ai_voc_is_playing)
        {
            s_voc_play_err_num++;
            if(s_voc_play_err_num > 2)
            {
                s_voc_play_err_num = 0;
                if(AI_DB_GetVocRcvNum() > 0)
                {
                    AI_File_Remove((char *)pMe->m_ai_pDB_vocrcv_CurList.r_name);
                    AI_DB_VocRcvDel(pMe,pMe->m_ai_pDB_vocrcv_CurList.list_id);
                }
                MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_PLAYSTOP,NULL,0);
                if(s_voc_is_play_all)
                {
                    if(AI_Voice_Play_Valid() && AI_DB_GetVocRcvNum() > 0)
                    {
                        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_FILEPLAY,NULL,0);
                    }
                }
            }
            else
            {
                AI_Voice_Play_TimerStart(pMe);
            }
        }
    }
}

static void AI_Voice_PlayNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    AI_APP_T *   pMe = (AI_APP_T *)&g_ai_app;
    BOOLEAN       result = TRUE;
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == pMe->m_ai_pPlayMedia)
        {
            switch(report_ptr->report)
            {
            case MMISRVAUD_REPORT_END:                
                    MMISRVAUD_Stop(handle);
                    MMISRVMGR_Free(handle);
                    pMe->m_ai_pPlayMedia = 0;

                    if (MMISRVAUD_REPORT_RESULT_STOP != report_ptr->data1)
                    {
                        if (MMISRVAUD_REPORT_RESULT_SUCESS != report_ptr->data1)
                        {
                            result = FALSE;
                        }
                        AI_Voice_Play_HandleEnd(result);
                    }

                break;

            default:
                break;
            }
        }
    }
}

static int AI_Net_Send_TK_VocFile_CB(void *pUser,uint8 * pRcv,uint32 Rcv_len)
{
    uint8 * pContent;
    uint32 Content_len = 0;
    int cont_len = 0;
    char buf[21] = {0};
    char val_buf[11] = {0};
    int ret = 0;
    int ret_val = 0;
    uint16 rsp_val = 0;
    AI_APP_T *pMe = (AI_APP_T *)pUser;
    cont_len = AI_Net_GetContentLen_RCV(pMe,pRcv,Rcv_len);
    if(cont_len > 0)
    {
        pContent = pRcv+(g_ai_imei_len + 10);
        Content_len = cont_len;
        ret = AI_Func_GetNextPara(&pContent, &Content_len,buf,20);
        if(ret > 0)
        {
            AI_Func_strupr(buf);
        }
        if(ret == 2 &&  strncmp( (char *)buf, "TK", ret ) == 0)
        {
            ret_val = AI_Func_GetNextPara(&pContent, &Content_len,val_buf,10);
            if(ret_val > 0)
            {
                rsp_val = atoi(val_buf);
            }
            if(rsp_val == 1)
            {
                AI_LOG("AI__VOICE AI_Net_Send_TK_VocFile_CB Success");
                return AEE_SUCCESS;
            }
            else
            {
                AI_LOG("AI__VOICE AI_Net_Send_TK_VocFile_CB Fail");
                return AEE_EFAILED;
            }
        }
    }

    AI_LOG("AI__VOICE AI_Net_Send_TK_VocFile_CB  ERR cont_len=%d",cont_len);
    return AEE_EFAILED;
}*/

/*
语音包格式是MM_AMR_FORMAT_FIXED_4_75_AMR
AMR 头 6 个字节,每一帧13个字节发送时需要对齐
*/
/*int32 AI_Net_Send_TK_VocFile_Start(AI_APP_T *pMe)
{

    ZDT_TCP_LINK_DATA_T * cur_data = NULL;
    uint32 len = 0;
    uint32 msg_len = 0;
    uint32 i = 0;
    uint32 j = 0;
    uint32 content_len = 0;
    BOOLEAN bResult = TRUE;
    uint32 buf_len = 0;
    uint32 file_len = 0;
    MMIFILE_HANDLE file_handle;
    uint32 cur_len = 0;
    char * file_name = NULL;
    char len_str[5] = {0};
    uint8 * tmp_buf = NULL;
    AI_VOC_SEND_DATA_T cur_filedata = {0};
#if FEATURE_AI_SEND_MD5
    char md5_str[32] = {0};
#endif
    
    if(pMe->m_ai_voc_is_sending)
    {
        AI_LOG("AI__VOICE Voc is Sending");
        return 0;
    }
    
    if(FALSE == AI_VOC_Send_Link_Get(pMe,&cur_filedata))
    {
        return 0;
    }

    if(pMe->m_ai_is_land == FALSE)
    {
        return -10;
    }
    
    if(g_ai_imei_len == 0)
    {
        return -1;
    }
    #if 0
    if(pMe->m_zdt_is_in_call)
    {
    	AI_LOG("AI__VOICE Voc is InCall");
    	return 0;
    }
    #endif
    
    file_name = (char *)cur_filedata.pFilename;
    if(file_name == NULL)
    {
        AI_LOG("AI__VOICE AI_Send_FileStart: ERR Name");
        return -1;
    }
        
    file_len = AI_File_GetSize(file_name);
    if(file_len < 19)
    {
        AI_LOG("AI__VOICE AI_Send_FileStart: ERR File Len=%d",file_len);
        return -5;
    }
    
    tmp_buf = (uint8 * )SCI_ALLOC_APPZ(file_len+10);
    if(tmp_buf == NULL)
    {
        AI_LOG("AI__VOICE AI_Send_FileStart: Malloc File Len=%d",file_len+10);
        return -6;
    }

    AI_File_Read(file_name, tmp_buf, file_len,&cur_len);
    if(cur_len >  0)
    {
        AI_LOG("AI__VOICE AI_Send_FileStart: IFILE_Read cur_len=0x%x,file_len=0x%x",cur_len,file_len);
        pMe->m_ai_voc_file_seek = cur_len;
        content_len = cur_len;
        for(i = 0; i < cur_len; i++)
        {
            if(tmp_buf[i] == 0x7D)
            {
                content_len++; 
            }
            else if(tmp_buf[i] == 0x5B)
            {
                content_len++; 
            }
            else if(tmp_buf[i] == 0x5D)
            {
                content_len++; 
            }
            else if(tmp_buf[i] == 0x2C)
            {
                content_len++; 
            }
            else if(tmp_buf[i] == 0x2A)
            {
                content_len++; 
            }
        }
    }
    else
    {
        SCI_FREE(tmp_buf);
        return -7;
    }
        
    cur_data=(ZDT_TCP_LINK_DATA_T *)SCI_ALLOC_APPZ(sizeof(ZDT_TCP_LINK_DATA_T));
    if(cur_data == NULL)
    {
        AI_LOG("AI__VOICE AI_Send_FileStart: ERR Malloc 2");
        SCI_FREE(tmp_buf);
        return -8;
    }
    content_len = content_len + 3;
#if FEATURE_AI_SEND_MD5
    buf_len = content_len+g_ai_imei_len+43;
#else
    buf_len = content_len+g_ai_imei_len+11;
#endif
    cur_data->str= (uint8 *)SCI_ALLOC_APPZ(buf_len);
    if(cur_data->str == NULL)
    {
        AI_LOG("AI__VOICE AI_Send_FileStart: ERR Malloc buf_len=%d",buf_len);
        SCI_FREE(tmp_buf);
        SCI_FREE(cur_data);
        return -9;
    }
    
#if FEATURE_AI_SEND_MD5
    //[XY*
    cur_data->str[0] = AI_NET_MSG_START;
    cur_data->str[33] = AI_NET_MSG_SEND_CS_1;
    cur_data->str[34] = AI_NET_MSG_SEND_CS_2;
    cur_data->str[35] = '*';
    len = 36;
 #else
    //[XY*
    cur_data->str[0] = AI_NET_MSG_START;
    cur_data->str[1] = AI_NET_MSG_SEND_CS_1;
    cur_data->str[2] = AI_NET_MSG_SEND_CS_2;
    cur_data->str[3] = '*';
    len = 4;
#endif
    //MEID
    SCI_MEMCPY(cur_data->str+len,g_zdt_phone_imei,g_ai_imei_len);
    len += g_ai_imei_len;
    cur_data->str[len++] = '*';

    j = len+8;
    for(i = 0; i < cur_len; i++)
    {
        if(tmp_buf[i] == 0x7D)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x01; 
        }
        else if(tmp_buf[i] == 0x5B)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x02; 
        }
        else if(tmp_buf[i] == 0x5D)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x03; 
        }
        else if(tmp_buf[i] == 0x2C)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x04; 
        }
        else if(tmp_buf[i] == 0x2A)
        {
            cur_data->str[j++] = 0x7D; 
            cur_data->str[j++] = 0x05; 
        }
        else
        {
            cur_data->str[j++] = tmp_buf[i]; 
        }
    }
    
    SCI_FREE(tmp_buf);
    
    //包长度
    msg_len = j-len-5;
    sprintf(len_str,"%04X",msg_len);
    SCI_MEMCPY(cur_data->str+len,len_str,4);
    len += 4;
    cur_data->str[len++] = '*';
    cur_data->str[len++] = 'T';
    cur_data->str[len++] = 'K';
    cur_data->str[len++] = ',';
    
    //结束]
    len = j;
    
#if FEATURE_AI_SEND_MD5
    AI_Net_MD5(cur_data->str+33,len-33,md5_str);
    SCI_MEMCPY(cur_data->str+1,md5_str,32);
#endif

    cur_data->str[len++] = AI_NET_MSG_END;

    pMe->m_ai_voc_pack_num = 1;
    pMe->m_ai_voc_pack_idx = 1;
    pMe->m_ai_voc_data_idx++;

    cur_data->len = len;
    cur_data->times = 1;
    cur_data->timeout = 0;
    cur_data->priority = 150;
    cur_data->rcv_handle = AI_Net_Send_TK_VocFile_CB;
    ZDT_API_TCP_SendDataNow(pMe->m_ai_tcp_reg_interface,cur_data);
    pMe->m_ai_voc_is_sending = TRUE;
    MMIZDT_UpdateTalkWin(AI_RECORDER_DISP_SENDING,file_name);
    AI_LOG("AI__VOICE AI_Send_FileStart Start num=%d/%d,data_idx=%d,seek=%d,file_len=%d",\
        pMe->m_ai_voc_pack_idx,pMe->m_ai_voc_pack_num,pMe->m_ai_voc_data_idx,pMe->m_ai_voc_file_seek,file_len);
    return cur_len;
}

void AI_Net_Send_TK_VocFile_End(AI_APP_T *pMe,uint8 err_id)
{
    AI_VOC_SEND_DATA_T cur_filedata = {0};
    
    if(AI_VOC_Send_Link_Get(pMe,&cur_filedata))
    {
        AI_File_Remove((char *)cur_filedata.pFilename);
        AI_VOC_Send_Link_DelOne(pMe);
    }
    
    pMe->m_ai_voc_file_seek = 0;
    pMe->m_ai_voc_is_sending = FALSE;
    
    if(AI_VOCSEND_SUCCESS == err_id)
    {
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RECORD_SEND,NULL,0);
    }
    else
    {        
        if(err_id != AI_VOCSEND_ERR_FILE)
        {
            AI_VOC_Send_Link_DelAll(pMe);
        }
        else
        {
            MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RECORD_SEND,NULL,0);
        }
    }
    AI_Voice_Send_CallBack(pMe,err_id);
    AI_LOG("AI__VOICE AI_Net_Send_TK_VocFile_End err_id=%d",err_id);
    return;
}

BOOLEAN AI_Voice_Send_Start(AI_APP_T * pMe)
{
    if(0 > AI_Net_Send_TK_VocFile_Start(pMe))
    {
        //AI_Net_Send_TK_VocFile_End(pMe,AI_VOCSEND_ERR);
    }
}
/*
void AI_Voice_File_Rcv_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    AI_APP_T *   pMe = (AI_APP_T *)param;
    AI_VOC_RcvFile_Link_DelAll(pMe);
}

void AI_Voice_File_Rcv_TimerStart(AI_APP_T * pMe)
{
    
    if(0 != pMe->m_ai_voc_rcv_time_id)
    {
        MMK_StopTimer(pMe->m_ai_voc_rcv_time_id);
        pMe->m_ai_voc_rcv_time_id = 0;
    }
    
    pMe->m_ai_voc_rcv_time_id = MMK_CreateTimerCallback(30000, 
                                                                        AI_Voice_File_Rcv_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void AI_Voice_File_Rcv_TimerStop(AI_APP_T * pMe)
{
    if(0 != pMe->m_ai_voc_rcv_time_id)
    {
        MMK_StopTimer(pMe->m_ai_voc_rcv_time_id);
        pMe->m_ai_voc_rcv_time_id = 0;
    }
}

int AI_Net_Receive_TK_VocFile(AI_APP_T *pMe,uint8 * pFiledata,uint16 DataLen)
{
    int result = 0;
    
    if(pFiledata != NULL && DataLen > 18)
    {
        uint8 pack_num;
        uint8 pack_idx;
        uint16 voc_idx;
        uint8 mem_status = 0;
        BOOLEAN is_full = FALSE;
        int res = 0;
        uint16 voc_len = 0;
        uint16 i = 0;
        uint16 j = 0;
        
        AI_VOC_RCV_FILE_DATA_T voc_rcv_data = {0};
        s_ai_voc_rcv_idx ++;
        if(s_ai_voc_rcv_idx > 0xFFF0)
        {
            s_ai_voc_rcv_idx = 1;
        }
        pack_num = 1;
        voc_idx = s_ai_voc_rcv_idx;
        pack_idx = 1;
        
        AI_LOG("AI__VOICE AI_Net_Receive_TK_VocFile s_ai_voc_rcv_idx=%d",s_ai_voc_rcv_idx);
        
        voc_rcv_data.voc_idx = voc_idx;
        voc_rcv_data.pack_idx = pack_idx;
        voc_rcv_data.pack_sum= pack_num;
        voc_rcv_data.voc_data = (uint8 *)SCI_ALLOC_APPZ(DataLen);
        if(voc_rcv_data.voc_data == NULL)
        {
            AI_LOG("AI__VOICE AI_Net_Receive_TK_VocFile Err SCI_ALLOC_APPZ Len=%d",DataLen);
            return 0;
        }
        
        for(i = 0; i < DataLen; i++)
        {
            if(pFiledata[i] == 0x7D && i != (DataLen-1))
            {
                if(pFiledata[i+1] == 0x01)
                {
                    voc_rcv_data.voc_data[j] = 0x7D;
                    i++;
                }
                else if(pFiledata[i+1] == 0x02)
                {
                    voc_rcv_data.voc_data[j] = 0x5B;
                    i++;
                }
                else if(pFiledata[i+1] == 0x03)
                {
                    voc_rcv_data.voc_data[j] = 0x5D;
                    i++;
                }
                else if(pFiledata[i+1] == 0x04)
                {
                    voc_rcv_data.voc_data[j] = 0x2C;
                    i++;
                }
                else if(pFiledata[i+1] == 0x05)
                {
                    voc_rcv_data.voc_data[j] = 0x2A;
                    i++;
                }
                else
                {
                    voc_rcv_data.voc_data[j] = 0x7D;
                }
            }
            else
            {
                voc_rcv_data.voc_data[j] = pFiledata[i];
            }
            j++;
        }
        voc_rcv_data.voc_data_len = j;

        if(FALSE == AI_VOC_RcvFile_Link_Add(pMe,&voc_rcv_data))
        {
            AI_LOG("AI__VOICE AI_Net_Receive_TK_VocFile ERR LinkAdd");
            return 0;
        }
                
        if(pack_idx == pack_num)
        {            
            res = AI_VOC_RcvFile_Save(pMe,&voc_rcv_data);
            
            if(res == AEE_SUCCESS)
            {
                result = 1;
                AI_DB_VocRcvAdd(pMe,pMe->m_ai_pFileRcvVocName);
            }
            else
            {
                if(res == AEE_ENOMEMORY || res == AEE_EUNABLETOLOAD)
                {
                    if(is_full)
                    {
                        //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_VOC_FULL);
                    }
                    else
                    {
                        //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_VOC_NEW);
                    }
                    MMIZDT_OpenTalkTPWin();
                }
                AI_LOG("AI__VOICE AI_Net_Receive_TK_VocFile ERR FileSave res=%d,voc_idx=%d",res,voc_idx);
            }
            AI_VOC_RcvFile_Link_DelByVocIdx(pMe,voc_idx);
        }
        AI_Voice_File_Rcv_TimerStart(pMe);
    }
    AI_LOG("AI__VOICE AI_Net_Receive_TK_VocFile result=%d",result);
    return result;
}*/

void AI_Voice_Record_TimeOut(
                                uint8 timer_id,
                                uint32 param
                                )
{
    AI_APP_T *   pMe = (AI_APP_T *)param;
    if(pMe->m_ai_eRecorderStatus == AI_RECORDER_RECORD)
    {
        if(pMe->m_ai_voc_send_times > 0)
        {
            pMe->m_ai_voc_send_times--;
        }
        if(pMe->m_ai_voc_send_times == 0)
        {
            if(0 != pMe->m_ai_voc_record_time_id)
            {
                MMK_StopTimer(pMe->m_ai_voc_record_time_id);
                pMe->m_ai_voc_record_time_id = 0;
            }
            AI_API_Record_Stop();
        }
        else
        {
            char tmp_buf[50] = {0};
            sprintf(tmp_buf,"TimerOut %d",pMe->m_ai_voc_send_times);
            //MMIZDT_UpdateTalkStr(tmp_buf);
            if(0 != pMe->m_ai_voc_record_time_id)
            {
                MMK_StartTimerCallback(pMe->m_ai_voc_record_time_id, 1000, AI_Voice_Record_TimeOut, (uint32)pMe, FALSE);
            }
        }
    }
}

void AI_Voice_Record_TimerStart(AI_APP_T * pMe)
{
    
    if(0 != pMe->m_ai_voc_record_time_id)
    {
        MMK_StopTimer(pMe->m_ai_voc_record_time_id);
        pMe->m_ai_voc_record_time_id = 0;
    }
    
    pMe->m_ai_voc_record_time_id = MMK_CreateTimerCallback(1000, 
                                                                        AI_Voice_Record_TimeOut, 
                                                                        (uint32)pMe, 
                                                                        FALSE);
}

void AI_Voice_Record_TimerStop(AI_APP_T * pMe)
{
    if(0 != pMe->m_ai_voc_record_time_id)
    {
        MMK_StopTimer(pMe->m_ai_voc_record_time_id);
        pMe->m_ai_voc_record_time_id = 0;
    }
}

BOOLEAN AI_Voice_Record(AI_APP_T * pMe)
{
    BOOLEAN Ret = FALSE;
    MMIRECORD_SRV_RECORD_PARAM_T record_param = {0};
    MMIRECORD_SRV_RESULT_E  record_result = MMIRECORD_SRV_RESULT_SUCCESS;
    MMIFILE_DEVICE_E record_dev = MMI_DEVICE_UDISK;
    BOOLEAN build_file = FALSE;
    MMI_STRING_T record_name = {0};
    AI_LOG("AI__VOICE AI_Voice_Record status=%d", pMe->m_ai_eRecorderStatus);
    if(pMe->m_ai_eRecorderStatus == AI_RECORDER_RECORD)
    {
        return TRUE;
    }
    
    build_file = AI_VOC_SendFileNameBuild(pMe);
    if(FALSE == build_file)
    {
        AI_LOG("AI__VOICE AI_Voice_Record ERR FileName");
        return FALSE;
    }
    
    if (PNULL != pMe->m_ai_pRecordMedia)
    {
        MMIRECORDSRV_StopRecord(pMe->m_ai_pRecordMedia);
        MMIRECORDSRV_FreeRecordHandle(pMe->m_ai_pRecordMedia);
        pMe->m_ai_pRecordMedia = 0;
    }

    pMe->m_ai_pRecordMedia = MMIRECORDSRV_RequestRecordHandle(AI_Voice_RecordNotify);
    if (PNULL == pMe->m_ai_pRecordMedia)
    {
        AI_LOG("AI__VOICE AI_Voice_Record ERR Handle");
        return FALSE;
    }
    
    record_name.wstr_ptr = pMe->m_ai_pFileSendVocFullName;
    record_name.wstr_len = pMe->m_ai_pFileSendVocFullName_len;
    
    record_param.is_create_file_name = TRUE;
    record_param.prefix_ptr = PNULL;
    record_param.record_dev_ptr = &record_dev;
    record_param.record_file_id_ptr = PNULL;
    record_param.record_name_ptr = &record_name;
    record_param.source = MMISRVAUD_RECORD_SOURCE_NORMAL;
    record_param.fmt = MMISRVAUD_RECORD_FMT_AMR;
    record_param.record_param_ptr = PNULL;
    record_param.volume = MMIAPISET_GetMultimVolume();
    record_result = MMIRECORDSRV_StartRecordFullFile(pMe->m_ai_pRecordMedia, record_param);
    AI_LOG("AI__VOICE StartRecordFullFile record_result=%d",record_result);
    if (record_result == MMIRECORD_SRV_RESULT_SUCCESS)
    {
        Ret = TRUE;
        //AI_Voice_Record_TimerStart(pMe);// maybe have bug
        s_ai_voc_is_recording = TRUE;
        pMe->m_ai_eRecorderStatus = AI_RECORDER_RECORD;
        AI_LOG("AI__VOICE Voice_Record OK");
    }
    else
    {
        s_ai_voc_is_recording = FALSE;
        pMe->m_ai_eRecorderStatus = AI_RECORDER_STOP;

    }
    return Ret;
}


PUBLIC void AI_API_ForceToStopPlay()
{
    
    AI_APP_T  *pMe = &g_ai_app ;
    AI_LOG("AI__VOICE AI_API_ForceToStopPlay");  
    //if(AI_IsInAiChatWin())
    if(AI_IsChatWinOpen())
    {
        AI_LOG("AI__VOICE AI_API_ForceToStopPlay 2");  
        AI_HttpForceClose();//AI_HttpClose();// close http
        AI_Voice_Rcv_Mp3_StopPlayBuf(pMe);
        //AI_Voice_Rcv_Mp3_PlayBufThreadStop();
        AI_Mp3BufList_FreeAll();

    }
}


PUBLIC void AI_API_ForceToStopAI(MMISRV_HANDLE_T audio_handle)
{
    
    AI_APP_T  *pMe = &g_ai_app ;
    if(AI_IsInAiChatWin() && pMe->m_ai_pPlayMedia != audio_handle)
    {
         AI_API_ForceToStopPlay();
    }
    AI_Voice_Record_Stop(pMe);      //BUG 录音时来视频导致视频通话无声音
}







BOOLEAN AI_Voice_Record_Start(AI_APP_T * pMe)
{
    BOOLEAN ret = TRUE;
    AI_LOG("AI__VOICE ZDTAI_Voice_RecordStart");
    //sim net ok?

    //mem is full ?

    //is playing receive file ?, stop play , is receiving mp3

    //is in recording?

    //yangyu add for fix bug, can not record and play mp3 same time
    /*if(pMe->m_ai_pPlayMedia  )
    {
        AI_LOG("AI__VOICE AI_Voice_Record_Start is playing mp3 ,return");
        return FALSE;  //for fix playing bug
    }*/

    AI_HttpForceClose();//AI_HttpClose();// close http
    AI_Voice_Rcv_Mp3_StopPlayBuf(&g_ai_app);
    AI_Voice_Rcv_Mp3_PlayBufThreadStop();
    AI_Mp3BufList_FreeAll();
    //AI_Mp3FixBuf_FreeAll();
    //yangyu end    
    
    ret = AI_Voice_Record(pMe);
    AI_LOG("AI__VOICE AI_Voice_Record_Start ret=%d", ret);
    return ret;
}

BOOLEAN AI_Voice_Record_Stop(AI_APP_T * pMe)
{
    
    MMIRECORD_SRV_RESULT_E ret = MMIRECORD_SRV_RESULT_SUCCESS;
    AI_LOG("AI__VOICE AI_Voice_Record_Stop");
    if(pMe->m_ai_eRecorderStatus == AI_RECORDER_RECORD)
    {
        AI_Voice_Record_TimerStop(pMe);
        if(pMe->m_ai_pRecordMedia)
        {
            //AI_Voice_RecordStop_TimerStart(pMe);//maybe bug
            ret = MMIRECORDSRV_StopRecord(pMe->m_ai_pRecordMedia);
            AI_LOG("AI__VOICE AI_Voice_Record_Stop ret=%d", ret);
            if(ret == MMIRECORD_SRV_RESULT_SUCCESS)
            {
                pMe->m_ai_eRecorderStatus = AI_RECORDER_STOP;//yangyu add for bug ,not record next time
                pMe->m_ai_pRecordMedia = 0;// for fix bug
                s_ai_voc_is_recording = FALSE;
                AI_TCPSendAIRecordFile();//yangyu add

            }
        }
        else
        {
            s_ai_voc_is_recording = FALSE;
            pMe->m_ai_eRecorderStatus = AI_RECORDER_STOP;
            
            pMe->m_ai_pRecordMedia = 0;// for fix bug
            //AI_File_Remove(pMe->m_ai_pFileSendVocName);// not remove now , after send then remove
            //AI_Voice_RecordStop_CallBack(pMe,AI_VOCRCDSTOP_ERR_SPACE);
            return FALSE;
        }
    }
    //SCI_SLEEP(100);//yangyu ADD, bug
    //AI_TCPSendAIRecordFile();//yangyu add ,move to
    return TRUE;
}

BOOLEAN AI_Voice_Record_StopTimout(AI_APP_T * pMe)
{
    AI_LOG("AI__VOICE AI_Voice_Record_StopTimout");
    if(pMe->m_ai_eRecorderStatus == AI_RECORDER_RECORD)
    {
        if (PNULL != pMe->m_ai_pRecordMedia)
        {
            MMIRECORDSRV_StopRecord(pMe->m_ai_pRecordMedia);
            MMIRECORDSRV_FreeRecordHandle(pMe->m_ai_pRecordMedia);
            pMe->m_ai_pRecordMedia = 0;
        }
        s_ai_voc_is_recording = FALSE;
        pMe->m_ai_eRecorderStatus = AI_RECORDER_STOP;
        //AI_File_Remove(pMe->m_ai_pFileSendVocName);
        AI_Voice_RecordStop_CallBack(pMe,AI_VOCRCDSTOP_ERR_TIMEOUT);
        return TRUE;
    }
    return FALSE;
}


LOCAL BOOLEAN AI_Mp3_Play_RequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify,
						MMISRVAUD_VOLUME_T volume
                        )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    
    req.notify = notify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = route;
    audio_srv.volume =  volume;

    switch(audio_data->type)
    {
    case MMISRVAUD_TYPE_RING_FILE:
        audio_srv.info.ring_file.type = audio_data->type;
        audio_srv.info.ring_file.fmt  = audio_data->ring_file.fmt;
        audio_srv.info.ring_file.name = audio_data->ring_file.name;
        audio_srv.info.ring_file.name_len = audio_data->ring_file.name_len;

        if(audio_srv.volume > MMISRVAUD_VOLUME_LEVEL_MAX)
        {
            audio_srv.volume = MMISRVAUD_VOLUME_LEVEL_MAX;
        }
        break;

    case MMISRVAUD_TYPE_RECORD_FILE:
        audio_srv.info.record_file.type = audio_data->type;        
        audio_srv.info.record_file.fmt  = audio_data->record_file.fmt;
        audio_srv.info.record_file.name = audio_data->record_file.name;
        audio_srv.info.record_file.name_len = audio_data->record_file.name_len;    
        audio_srv.info.record_file.source   = audio_data->record_file.source;
        audio_srv.info.record_file.frame_len= audio_data->record_file.frame_len;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;

    case MMISRVAUD_TYPE_IQ://测试模式
        audio_srv.info.iq_file.type = audio_data->type;
        audio_srv.info.iq_file.name_ptr = audio_data->iq_file.name_ptr;
        audio_srv.info.iq_file.param    = audio_data->iq_file.param;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;
        
    default:
        break;
    }

    *audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
    AI_LOG("AI__VOICE AI_Mp3_Play_RequestHandle = %d", *audio_handle);    
    if(*audio_handle > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static void AI_Mp3_PlayNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    AI_APP_T *   pMe = (AI_APP_T *)&g_ai_app;
    BOOLEAN       result = TRUE;
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == pMe->m_ai_pPlayMedia)
        {
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    AI_LOG("AI__VOICE AI_Mp3_PlayNotify MMISRVAUD_REPORT_END data1=%d" ,report_ptr->data1);
                    AI_InitNextUrlRequest(); // play next url , one time have two url(nlp and music)
                    MMISRVAUD_Stop(handle);
                    MMISRVMGR_Free(handle);
                    pMe->m_ai_pPlayMedia = 0;
                    

                    if (MMISRVAUD_REPORT_RESULT_STOP != report_ptr->data1)
                    {
                        if (MMISRVAUD_REPORT_RESULT_SUCESS != report_ptr->data1)
                        {
                            result = FALSE;
                        }
                        //AI_Voice_Play_HandleEnd(result);
                    }

                    break;

                default:
                    break;
            }
        }
    }
}



int AI_Voice_Rcv_Mp3_Play(AI_APP_T *pMe)
{
    MMISRVAUD_TYPE_U    audio_data  = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;
    uint16      full_path[MAX_AI_VOC_FILENAME_SIZE+1] = {0};
    uint16      full_path_len = 0;
    int Ret = 0;
    char* file_name = AI_VOICE_RCV_FULL_PATH;
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_Play 1 ");
    if(file_name == PNULL)
    {
        return 1;
    }
    
    if(pMe->m_ai_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
        pMe->m_ai_pPlayMedia = NULL;
    }
    
    full_path_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    audio_data.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.name = full_path;
    audio_data.ring_file.name_len = full_path_len;
    audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)MMIAPICOM_GetMusicType(audio_data.ring_file.name, audio_data.ring_file.name_len);
    
    if(AI_Mp3_Play_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, AI_Mp3_PlayNotify,s_ai_voc_play_vol))
    {
        pMe->m_ai_pPlayMedia = audio_handle;
        if(!MMISRVAUD_Play(audio_handle, 0))
        {     
            AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_Play play failed");
            MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
            pMe->m_ai_pPlayMedia = 0;       
            Ret = 2;
        }
    }
    else
    {        
        Ret = 3;
    }
      
    if (Ret != 0)
    {
        pMe->m_ai_pPlayMedia = NULL;
        pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYSTOP;
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_Play Err Play Ret=%d",Ret);
        return Ret;
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_Play OK vol=%d",s_ai_voc_play_vol);
        pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYING;        
    }
    return Ret;



}
/*
int AI_Voice_Rcv_Mp3_PlayStream(AI_APP_T *pMe)
{
    MMISRVAUD_TYPE_U    audio_data  = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;
    uint16      full_path[MAX_AI_VOC_FILENAME_SIZE+1] = {0};
    uint16      full_path_len = 0;
    int Ret = 0;
    char* file_name = AI_VOICE_RCV_FULL_PATH;
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_Play 1 ");
    if(file_name == PNULL)
    {
        return 1;
    }
    
    if(pMe->m_ai_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
        pMe->m_ai_pPlayMedia = NULL;
    }
    
    full_path_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    audio_data.ring_file.type = MMISRVAUD_TYPE_STREAMING;//MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.name = full_path;
    audio_data.ring_file.name_len = full_path_len;
    audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)MMIAPICOM_GetMusicType(audio_data.ring_file.name, audio_data.ring_file.name_len);
    
    if(AI_Mp3_Play_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, AI_Mp3_PlayNotify,s_ai_voc_play_vol))
    {
        pMe->m_ai_pPlayMedia = audio_handle;
        if(!MMISRVAUD_Play(audio_handle, 0))
        {     
            AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_Play play failed");
            MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
            pMe->m_ai_pPlayMedia = 0;       
            Ret = 2;
        }
    }
    else
    {        
        Ret = 3;
    }
      
    if (Ret != 0)
    {
        pMe->m_ai_pPlayMedia = NULL;
        pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYSTOP;
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_Play Err Play Ret=%d",Ret);
        return Ret;
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_Play OK vol=%d",s_ai_voc_play_vol);
        pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYING;        
    }
    return Ret;



}*/

LOCAL BOOLEAN AI_Mp3PlayBufNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    AI_APP_T *   pMe = (AI_APP_T *)&g_ai_app;
    BOOLEAN       result = TRUE;
    BOOLEAN is_waiting_next_url = FALSE;
    BOOLEAN ret = FALSE;
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == pMe->m_ai_pPlayMedia)
        {
             AI_LOG("AI__VOICE AI_Mp3PlayBufNotify report=%d" , report_ptr->report);
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    AI_LOG("AI__VOICE AI_Mp3PlayBufNotify MMISRVAUD_REPORT_END data1=%d" ,report_ptr->data1);
                    is_waiting_next_url = AI_IsNextUrlAddrWaiting();
                    if(is_waiting_next_url) AI_InitNextUrlRequest(); // play next url , one time have two url(nlp and music)
                    ret = MMISRVAUD_Stop(handle);
                    if(ret) MMISRVMGR_Free(handle);//yangyu modify for bug die
                    ai_is_mp3_playing = FALSE;
                    pMe->m_ai_pPlayMedia = 0; 
                    //AI_Mp3BufList_FreeHead();
                    //AI_Voice_Rcv_Mp3_PlayBuf(pMe);//BUG,play next buffer
                    //AI_Voice_Rcv_Mp3_PlayBuf(&g_ai_app);//bug ,can not play here,not in same thread,play next buffer
                    
                    if(ai_mp3_buf_list_play_ptr != PNULL && ai_mp3_buf_list_play_ptr->mp3_data.data != PNULL)  //release data buffer
                    {
                        AI_LOG("AI__VOICE AI_Mp3PlayBufNotify free buf_index=%d, malloc_index=%d", ai_mp3_buf_list_play_ptr->mp3_data.buf_index,ai_mp3_buf_list_play_ptr->mp3_data.malloc_index);
                        //SCI_FREE(ai_mp3_buf_list_play_ptr->mp3_data.data);
                        //ai_mp3_buf_list_play_ptr->mp3_data.data = NULL;
                        
                        //SCI_MEMSET(ai_mp3_buf_list_play_ptr->mp3_data.data, 0, ai_mp3_buf_list_play_ptr->mp3_data.alloc_len);//solve bug
                        ai_mp3_buf_list_play_ptr->mp3_data.data_len = 0;
                        if(is_waiting_next_url)
                        {
                            ai_mp3_buf_list_play_ptr = NULL;

                        }
                        
                        
                    }

                    /*if(MMIAI_DataRcvIsBreak() && 
                            ai_mp3_buf_list_play_ptr != PNULL && 
                            ai_mp3_buf_list_play_ptr->next != PNULL && ai_mp3_buf_list_play_ptr->next->next == NULL)
                    {
                        AI_LOG("AI__VOICE AI_Mp3PlayBufNotify start next break continue download");
                        MMIAI_Http_Request_SendMsg(); //break continue downloadsss
                    }
                    /*else if(!MMIAI_DataRcvIsBreak() && !MMIAI_IsDownloadFinish() && ai_mp3_buf_list_play_ptr->next == NULL)
                    {
                        AI_LOG("AI__VOICE AI_Mp3PlayBufNotify start next break continue download again , exception stop");
                        MMIAI_SetContinueAgain();
                        MMIAI_Http_Request_SendMsg(); //break continue downloadsss
                    }*/
                    //if(!ai_is_inserting_mp3)
                    AI_LOG("AI__VOICE AI_Mp3PlayBufNotify is_inserting=%d",ai_is_inserting_mp3);
                    if(!ai_is_inserting_mp3 )
                    {
                        
                        if(ai_mp3_buf_list_play_ptr != PNULL && ai_mp3_buf_list_play_ptr->next != NULL)
                        {
                            ai_is_waiting_next_mp3 = FALSE;
                            ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;

                            AI_API_Rcv_Mp3_PlayStart();//no bug, but play discontinus
                            

                        }
                        else
                        {
                            ai_is_waiting_next_mp3 = TRUE;
                            //AI_StartNextMp3PlayTimer();
                        }
                    }
                    else
                    {
                        ai_is_waiting_next_mp3 = TRUE;

                    }
                    
                   
                    if (MMISRVAUD_REPORT_RESULT_STOP != report_ptr->data1)
                    {
                        if (MMISRVAUD_REPORT_RESULT_SUCESS != report_ptr->data1)
                        {
                            result = FALSE;
                        }
                        //AI_Voice_Play_HandleEnd(result);
                    }
                    
                   
                    break;

                default:
                    break;
            }
        }
    }
    return TRUE;
}


LOCAL BOOLEAN AI_Mp3PlayFixBufNotify(MMISRV_HANDLE_T handle, MMISRVMGR_NOTIFY_PARAM_T *param)
{
    MMISRVAUD_REPORT_T *report_ptr = PNULL;
    AI_APP_T *   pMe = (AI_APP_T *)&g_ai_app;
    BOOLEAN       result = TRUE;
    uint16 i = 0;
    
    if(param != PNULL && handle > 0)
    {
        report_ptr = (MMISRVAUD_REPORT_T *)param->data;
        if(report_ptr != PNULL && handle == pMe->m_ai_pPlayMedia)
        {
             AI_LOG("AI__VOICE AI_Mp3PlayBufNotify report=%d" , report_ptr->report);
            switch(report_ptr->report)
            {
                case MMISRVAUD_REPORT_END:  
                    AI_LOG("AI__VOICE AI_Mp3PlayBufNotify MMISRVAUD_REPORT_END data1=%d" ,report_ptr->data1);
                    AI_InitNextUrlRequest(); // play next url , one time have two url(nlp and music)
                    MMISRVAUD_Stop(handle);
                    MMISRVMGR_Free(handle);
                    ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.play_index] = 0;
                    ai_is_mp3_playing = FALSE;
                    pMe->m_ai_pPlayMedia = 0; 
                    
                    //AI_Mp3BufList_FreeHead();
                    //AI_Voice_Rcv_Mp3_PlayBuf(pMe);//BUG,play next buffer
                    //AI_Voice_Rcv_Mp3_PlayBuf(&g_ai_app);//bug ,can not play here,not in same thread,play next buffer
                    
                    ai_mp3_fix_buf_holder.play_index += 1;
                    if(ai_mp3_fix_buf_holder.play_index >= AI_FIX_BUF_COUNT)
                    {
                        ai_mp3_fix_buf_holder.play_index = 0;
                    }
                    
                    //if(ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.play_index] > 0)
                    //{
                        
                        AI_API_Rcv_Mp3_PlayStart();//no bug, but play discontinus
                    //}
                    //else
                    //{
                    //    AI_Mp3FixBuf_FreeAll(); // no data play ,free all
                    //}

                    
                   
                    if (MMISRVAUD_REPORT_RESULT_STOP != report_ptr->data1)
                    {
                        if (MMISRVAUD_REPORT_RESULT_SUCESS != report_ptr->data1)
                        {
                            result = FALSE;
                        }
                        //AI_Voice_Play_HandleEnd(result);
                    }

                    break;

                default:
                    break;
            }
        }
    }
    return TRUE;
}


PUBLIC void AI_Voice_Rcv_Mp3_StopPlayBuf(AI_APP_T *pMe)
{
    BOOLEAN ret = FALSE;
    if(pMe->m_ai_pPlayMedia)
    {
        ret = MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_StopPlayBuf MMISRVAUD_Stop ret = %d ", ret);  
        
        if(ret) MMISRVMGR_Free(pMe->m_ai_pPlayMedia);//yangyu modify for bug die
        pMe->m_ai_pPlayMedia = NULL;
    }   
    ai_is_mp3_playing = FALSE;
    if(!AI_IsInAiChatWin())
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_StopPlayBuf not in ai win, clear list");  
        AI_Mp3BufList_FreeAll();
        //AI_Mp3FixBuf_FreeAll();
        //AI_Voice_Rcv_Mp3_PlayBufThreadStop();
    }

}


void AI_Mp3_Play_TaskEntry(uint32 argc, void *argv)
{
    TCP_LOG("AI_Mp3_Play_TaskEntry");
    //uint16 sleep_ms = 5;
    while(1)
    {
        if(!ai_is_mp3_playing /*&& !ai_is_inserting_mp3*/)
        {
            if(ai_mp3_buf_list_ptr && !ai_mp3_buf_list_play_ptr)
            {
                ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_ptr;
                AI_Voice_Rcv_Mp3_PlayBuf(&g_ai_app);
            }
            else if(ai_mp3_buf_list_play_ptr && ai_mp3_buf_list_play_ptr->next)
            {
                if(ai_mp3_buf_list_play_ptr->mp3_data.data)
                {
                    SCI_FREE(ai_mp3_buf_list_play_ptr->mp3_data.data);
                    ai_mp3_buf_list_play_ptr->mp3_data.data = NULL;
                }
                ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;
                //SCI_Sleep(20); //sleep 20 ms ,then play next buffer, useless
                AI_Voice_Rcv_Mp3_PlayBuf(&g_ai_app);
            }
            else if(ai_mp3_buf_list_play_ptr && !ai_mp3_buf_list_play_ptr->next)
            {
                AI_Mp3BufList_FreeAll();
            }

        }

        SCI_Sleep(5);
    }

}


PUBLIC void AI_Voice_Rcv_Mp3_PlayBufThreadStop()
{
    if(m_ai_voc_rcv_mp3_task_id != SCI_INVALID_BLOCK_ID)
    {
        SCI_TerminateThread(m_ai_voc_rcv_mp3_task_id);
        SCI_DeleteThread(m_ai_voc_rcv_mp3_task_id);
        m_ai_voc_rcv_mp3_task_id = SCI_INVALID_BLOCK_ID;
        ai_is_mp3_playing = FALSE;

    }

}


PUBLIC void AI_Voice_Rcv_Mp3_PlayBufThreadStart()
{
    //AI_APP_T *pMe = &g_ai_app;  
    TCP_LOG("AI_Voice_Rcv_Mp3_PlayBufThreadStart, m_ai_voc_rcv_mp3_task_id=0x%x", m_ai_voc_rcv_mp3_task_id);
    if(m_ai_voc_rcv_mp3_task_id == SCI_INVALID_BLOCK_ID)
    {
        m_ai_voc_rcv_mp3_task_id = SCI_CreateThread("AI_MP3_PLAY_THREAD_NAME",
                                                                "AI_MP3_PLAY_QUEUE",
                                                                AI_Mp3_Play_TaskEntry,
                                                                NULL,
                                                                NULL,
                                                                1024*10,
                                                                5,
                                                                TX_SYSTEM_NORMALE,  // TX_SYSTEM_HIGH,
                                                                SCI_PREEMPT,
                                                                SCI_AUTO_START
                                                                );
    TCP_LOG("AI_Voice_Rcv_Mp3_PlayBufThreadStart2, m_ai_voc_rcv_mp3_task_id=0x%x", m_ai_voc_rcv_mp3_task_id);                                                            
    }
    


}



PUBLIC int AI_Voice_Rcv_Mp3_PlayBuf(AI_APP_T *pMe)
{
    MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MP3, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    //MMISRV_HANDLE_T handle = 0;
    BOOLEAN result = FALSE;
    uint32        play_times = 1;
    //uint32          ring_id   =   0;
    uint32          ring_vol  =   0 ;

    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf");
    

    ai_is_mp3_playing = FALSE;
    
    if(!AI_IsInAiChatWin())
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf not in ai win, clear list");  
        AI_Mp3BufList_FreeAll();
        //AI_Voice_Rcv_Mp3_PlayBufThreadStop();
        return 1;
    }


    //ring_id = WNS_NOTIFICATION_RING_ID;
    ring_vol = s_ai_voc_play_vol;
    ring_vol = MMIAPISET_GetMultimVolume();//MMIAPISET_GetCallRingVolume();//s_ai_voc_play_vol;
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf volume=%d" ,ring_vol);
    ring_vol = 9; //MMIAPISET_GetCallRingVolume();//MMIAPISET_GetCallRingVolume();//s_ai_voc_play_vol;
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf volume=%d" ,ring_vol);
    //ring_vol = MMIAPIENVSET_GetCurModeOptValue(MN_DUAL_SYS_MAX, CALL_RING_VOL);
    //AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf volume=%d" ,ring_vol);

    //MMI_GetRingInfo(ring_id, &ring_data);
    /*if(ai_mp3_buf_list_play_ptr == NULL)
    {
        ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_ptr;

    }
    else
    {
        ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;
    }*/
    
    /*if(ai_mp3_buf_list_ptr)
    {
        ring_data.data_len = ai_mp3_buf_list_ptr->mp3_data.data_len;
        ring_data.data_ptr = ai_mp3_buf_list_ptr->mp3_data.data;
    }*/
    if(ai_mp3_buf_list_play_ptr != NULL && ai_mp3_buf_list_play_ptr->mp3_data.data_len > 0)
    {
        ring_data.data_len = ai_mp3_buf_list_play_ptr->mp3_data.data_len;
        ring_data.data_ptr = ai_mp3_buf_list_play_ptr->mp3_data.data;
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf, buf_index=%d, malloc_index=%d , data_len=%ld", ai_mp3_buf_list_play_ptr->mp3_data.buf_index, ai_mp3_buf_list_play_ptr->mp3_data.malloc_index,ring_data.data_len);
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf1, no list data, free all buf list");
        //AI_Mp3BufList_FreeAll();
        return 1;
    }
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf2");

    if(pMe->m_ai_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
        pMe->m_ai_pPlayMedia = NULL;
    }
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf3");

    if (PNULL != ring_data.data_ptr)
    {
        /*pMe->m_ai_pPlayMedia = MMISRVAUD_GetHandleByName("TONE SRV");
        if(pMe->m_ai_pPlayMedia > 0)
        {
            SCI_TRACE_LOW("Free tone handle when play key ring !!");
            MMISRVMGR_Free(handle);
        }*/
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf4");

        req.is_auto_free = TRUE;
        req.notify = AI_Mp3PlayBufNotify;
        req.pri = MMISRVAUD_PRI_NORMAL;

        audio_srv.play_times = play_times;
        //audio_srv.duation = WNS_NOTIFICATION_RING_DUATION;
        audio_srv.volume = ring_vol;
        audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
        audio_srv.info.ring_buf.fmt = ring_data.type;
        audio_srv.info.ring_buf.data = ring_data.data_ptr;
        audio_srv.info.ring_buf.data_len = ring_data.data_len;
        audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;//MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;

        pMe->m_ai_pPlayMedia = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(pMe->m_ai_pPlayMedia > 0)
        {
            result = MMISRVAUD_Play(pMe->m_ai_pPlayMedia, 0);       
            AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf play result=%d", result); 
            if(!result)
            {
                MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
                pMe->m_ai_pPlayMedia = 0;
                ai_is_mp3_playing = FALSE;
                
                //CallBackFunction(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
                AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf MMISRVAUD_REPORT_RESULT_ERROR "); 
            }
            if(result == MMISRVAUD_RET_OK)
            {

                AI_StopNextMp3PlayTimer();
                ai_is_mp3_playing = TRUE;
                ai_is_waiting_next_mp3 = FALSE;
                
                if(!AI_IsNextUrlAddrWaiting()&&AI_HTTP_IsMp3DownloadStopedLongTime())
                {
                    NotifyStartNextBufferDownload();
                }
                else
                {
                    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf waiting next url ,should start download after play end "); 
                }
                
                AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf play ok, buf_index=%d  malloc_index=%d",ai_mp3_buf_list_play_ptr->mp3_data.buf_index, ai_mp3_buf_list_play_ptr->mp3_data.malloc_index  ); 
                return 0;
            }
        }
        else
        {
            ai_is_mp3_playing = FALSE;
            //CallBackFunction(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
            AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf error MMISRVAUD_REPORT_RESULT_NOT_SUPPORT ");
        }
    }
    else
    {
        ai_is_mp3_playing = FALSE;
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf: ring file fomat error");
    } 
    return 1;
}

PUBLIC int AI_Voice_Rcv_Mp3_PlayFixBuf(AI_APP_T *pMe)
{
    MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MP3, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    //MMISRV_HANDLE_T handle = 0;
    BOOLEAN result = FALSE;
    uint32        play_times = 1;
    //uint32          ring_id   =   0;
    uint32          ring_vol  =   0 ;

    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf");
    if(!AI_IsInAiChatWin())
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf not in ai win, clear list");  
        //AI_Mp3BufList_FreeAll();
        //AI_Voice_Rcv_Mp3_PlayBufThreadStop();
        return 1;
    }


    //ring_id = WNS_NOTIFICATION_RING_ID;
    ring_vol = s_ai_voc_play_vol;

    //MMI_GetRingInfo(ring_id, &ring_data);
    
    if(ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.play_index] > 0)
    {
        ring_data.data_len = ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.play_index];
        ring_data.data_ptr = ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.play_index];
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf1, no list data, free all buf list");
        AI_Mp3FixBuf_FreeAll();
        return 1;
    }
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayBuf2");

    if(pMe->m_ai_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
        pMe->m_ai_pPlayMedia = NULL;
    }
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf3");

    if (PNULL != ring_data.data_ptr)
    {
        /*pMe->m_ai_pPlayMedia = MMISRVAUD_GetHandleByName("TONE SRV");
        if(pMe->m_ai_pPlayMedia > 0)
        {
            SCI_TRACE_LOW("Free tone handle when play key ring !!");
            MMISRVMGR_Free(handle);
        }*/
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf4");

        req.is_auto_free = TRUE;
        req.notify = AI_Mp3PlayFixBufNotify;
        req.pri = MMISRVAUD_PRI_NORMAL;

        audio_srv.play_times = play_times;
        //audio_srv.duation = WNS_NOTIFICATION_RING_DUATION;
        audio_srv.volume = ring_vol;
        audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
        audio_srv.info.ring_buf.fmt = ring_data.type;
        audio_srv.info.ring_buf.data = ring_data.data_ptr;
        audio_srv.info.ring_buf.data_len = ring_data.data_len;
        audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;//MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;
        //ai_is_mp3_playing = TRUE;

        pMe->m_ai_pPlayMedia = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(pMe->m_ai_pPlayMedia > 0)
        {
            result = MMISRVAUD_Play(pMe->m_ai_pPlayMedia, 0);       
            AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf play result=%d", result); 
            if(!result)
            {
                MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
                pMe->m_ai_pPlayMedia = 0;
                ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.play_index] = 0;
                ai_is_mp3_playing = FALSE;
                //CallBackFunction(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
                AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf MMISRVAUD_REPORT_RESULT_ERROR "); 
            }
            if(result == MMISRVAUD_RET_OK)
            {

                ai_is_mp3_playing = TRUE;
                AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf play ok"); 
                return 0;
            }
        }
        else
        {
            ai_is_mp3_playing = FALSE;
            //CallBackFunction(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
            AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf error MMISRVAUD_REPORT_RESULT_NOT_SUPPORT ");
        }
    }
    else
    {
        ai_is_mp3_playing = FALSE;
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayFixBuf: ring file fomat error");
    } 
    return 1;
}


PUBLIC int AI_Voice_Rcv_Mp3_PlayAllData(AI_APP_T *pMe)
{
    MMIAUD_RING_DATA_INFO_T     ring_data = {MMISRVAUD_RING_FMT_MP3, 0, NULL};/*lint !e64*/
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    //MMISRV_HANDLE_T handle = 0;
    BOOLEAN result = FALSE;
    uint32        play_times = 1;
    //uint32          ring_id   =   0;
    uint32          ring_vol  =   0 ;

    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData");
    if(!AI_IsInAiChatWin())
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData not in ai win, clear list");  
        AI_Mp3DirectDataFree();
        return 1;
    }


    
    if(ai_mp3_data.data)
    {
        ring_data.data_len = ai_mp3_data.data_len;
        ring_data.data_ptr = ai_mp3_data.data;
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData, no  data, free all buf list");
        //AI_Mp3BufList_FreeAll();
        return 1;
    }
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData2");

    if(pMe->m_ai_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
        pMe->m_ai_pPlayMedia = NULL;
    }
    AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData3");

    if (PNULL != ring_data.data_ptr)
    {
        /*pMe->m_ai_pPlayMedia = MMISRVAUD_GetHandleByName("TONE SRV");
        if(pMe->m_ai_pPlayMedia > 0)
        {
            SCI_TRACE_LOW("Free tone handle when play key ring !!");
            MMISRVMGR_Free(handle);
        }*/
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData4");

        req.is_auto_free = TRUE;
        req.notify = AI_Mp3PlayBufNotify;
        req.pri = MMISRVAUD_PRI_NORMAL;

        audio_srv.play_times = play_times;
        //audio_srv.duation = WNS_NOTIFICATION_RING_DUATION;
        audio_srv.volume = ring_vol;
        audio_srv.info.ring_buf.type = MMISRVAUD_TYPE_RING_BUF;
        audio_srv.info.ring_buf.fmt = ring_data.type;
        audio_srv.info.ring_buf.data = ring_data.data_ptr;
        audio_srv.info.ring_buf.data_len = ring_data.data_len;
        audio_srv.all_support_route = MMISRVAUD_ROUTE_NONE;//MMISRVAUD_ROUTE_SPEAKER | MMISRVAUD_ROUTE_EARPHONE;

        pMe->m_ai_pPlayMedia = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);
        if(pMe->m_ai_pPlayMedia > 0)
        {
            result = MMISRVAUD_Play(pMe->m_ai_pPlayMedia, 0);       
            AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData play result=%d", result); 
            if(!result)
            {
                MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
                pMe->m_ai_pPlayMedia = 0;
                //CallBackFunction(MMISRVAUD_REPORT_RESULT_ERROR, PNULL);
                AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData MMISRVAUD_REPORT_RESULT_ERROR "); 
            }
            if(result == MMISRVAUD_RET_OK)
            {
                AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData play ok"); 
                return 0;
            }
        }
        else
        {
            //CallBackFunction(MMISRVAUD_REPORT_RESULT_NOT_SUPPORT, PNULL);
            AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData error MMISRVAUD_REPORT_RESULT_NOT_SUPPORT ");
        }
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_Rcv_Mp3_PlayAllData: ring file fomat error");
    } 
    return 1;

}




/*
void AI_Mp3BufList_init(Mp3BufList_T *H)
{
      *H = (Mp3BufList_T)malloc(sizeof(Mp3BufList_T));
      if(NULL == *H)
      {
         AI_LOG("AI__VOICE AI_Mp3BufList_init malloc failed\n");
         //exit(1);
      }
      (*H)->next = NULL;
}

void AI_Mp3BufList_create(Mp3BufList_T h)
{
      int n,i;
      Mp3BufList_T new,p = h;
      printf("请输入链表长度:");
      scanf("%d",&n);
      for(i = 0; i < n; i++)
      {
         new = (Mp3BufList_T)malloc(sizeof(link_list));
         if(NULL == new){
        printf("malloc failed\n");
        exit(1);
     }
     scanf("%d",&new->data);
     AI_Mp3BufList_insert(p,new); //将new插入到p的后面
     p = p->next;
     AI_Mp3BufList_show(h);
      }
}*/

void AI_Mp3BufList_insert(Mp3BufList_T *p, Mp3BufList_T *new)
{
      new->next = p->next;
      p->next = new;
}

LOCAL void AI_Mp3BufList_FreeHead()
{
    Mp3BufList_T* tmp_ptr = NULL;
    if(ai_mp3_buf_list_ptr)
    {
        if(ai_mp3_buf_list_ptr->mp3_data.data)
        {
            SCI_FREE(ai_mp3_buf_list_ptr->mp3_data.data);
        }
        tmp_ptr = ai_mp3_buf_list_ptr;
        ai_mp3_buf_list_ptr = ai_mp3_buf_list_ptr->next;
        SCI_FREE(tmp_ptr);       
    }

}

LOCAL void AI_Mp3BufList_FreeAll()
{
    Mp3BufList_T* tmp_ptr = ai_mp3_buf_list_ptr;
    Mp3BufList_T* tmp_ptr2 = NULL;
    AI_LOG("AI__VOICE AI_Mp3BufList_FreeAll start");
    while(tmp_ptr != NULL)
    {
        tmp_ptr2 = tmp_ptr->next;
        if(tmp_ptr->mp3_data.data)
        {
            SCI_FREE(tmp_ptr->mp3_data.data);
        }
        //tmp_ptr = ai_mp3_buf_list_ptr;
        //ai_mp3_buf_list_ptr = ai_mp3_buf_list_ptr->next;
        SCI_FREE(tmp_ptr); 

        tmp_ptr = tmp_ptr2;
        //one loop, don't free again
        if(tmp_ptr != NULL && tmp_ptr->mp3_data.malloc_index == 0)
        {
            break;
        }
       
    }
    ai_mp3_buf_list_ptr = NULL;
    ai_mp3_buf_list_play_ptr = NULL;
    AI_LOG("AI__VOICE AI_Mp3BufList_FreeAll end");
    AI_StopNextMp3PlayTimer();

}

LOCAL void AI_Mp3FixBuf_FreeAll()
{
    
    //SCI_MEMSET(&ai_mp3_fix_buf_holder, 0 , sizeof(Mp3FixBufHolder_T));
    SCI_MEMSET(ai_mp3_fix_buf_holder.data_len, 0 , AI_FIX_BUF_COUNT*sizeof(uint32));
    ai_mp3_fix_buf_holder.inserting_index = 0;
    ai_mp3_fix_buf_holder.play_index = 0;
  
    ai_mp3_fix_buf_holder.is_init = FALSE;

}


LOCAL void AI_StartNextMp3Play(uint8 timer_id,
                                uint32 param)
{
    if(AI_IsInAiChatWin())
    {
        if(ai_mp3_buf_list_play_ptr && ai_mp3_buf_list_play_ptr->next != NULL)
        {
            ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;
            AI_API_Rcv_Mp3_PlayStart();
        }
        else
        {
            ai_next_mp3_check_count++;
            if(ai_next_mp3_check_count > 3)
            {
                AI_Mp3BufList_FreeAll();
                AI_StopNextMp3PlayTimer();
            }
        }
    }
    
}

LOCAL void AI_StopNextMp3PlayTimer()
{
    if(0 != ai_next_mp3_play_timer_id)
    {
        MMK_StopTimer(ai_next_mp3_play_timer_id);
        ai_next_mp3_play_timer_id = 0;
    }
}


LOCAL void AI_StartNextMp3PlayTimer()
{
    if(0 != ai_next_mp3_play_timer_id)
    {
        MMK_StopTimer(ai_next_mp3_play_timer_id);
        ai_next_mp3_play_timer_id = 0;
    } 
    ai_next_mp3_check_count = 0;
    ai_next_mp3_play_timer_id = MMK_CreateTimerCallback(100, AI_StartNextMp3Play, 0, TRUE); //500 ms later no http data receive ,then start play                                                                                                                                                                                                                   
}

LOCAL void AI_Mp3DirectDataFree()
{
    if(ai_mp3_data.data)
    {
        SCI_FREE(ai_mp3_data.data);
    }
    SCI_MEMSET(&ai_mp3_data, 0 , sizeof(Mp3Data_T));
    
}


PUBLIC void AI_Mp3InsertData(uint8* data, uint32 data_len)
{
    uint8* tmp_ptr = NULL;
    uint32 tmp_len = 0;
    if(ai_mp3_data.data && ai_mp3_data.data_len > 0)
    { 
        tmp_ptr = (uint8*)malloc(ai_mp3_data.data_len * sizeof(uint8));
        if(tmp_ptr)
        {
            SCI_MEMCPY(tmp_ptr, ai_mp3_data.data, ai_mp3_data.data_len);
            SCI_FREE(ai_mp3_data.data);
            ai_mp3_data.data = NULL;
            tmp_len = ai_mp3_data.data_len;
            ai_mp3_data.data_len = ai_mp3_data.data_len + data_len;
            ai_mp3_data.data = (uint8*)malloc(ai_mp3_data.data_len * sizeof(uint8));
            if(ai_mp3_data.data)
            {
                SCI_MEMCPY(ai_mp3_data.data, tmp_ptr, tmp_len);
                SCI_MEMCPY(ai_mp3_data.data+tmp_len, data, data_len);
                SCI_FREE(tmp_ptr);
                AI_StartNextMp3PlayTimer();
            }
            else
            {
                AI_LOG("AI__VOICE AI_Mp3InsertData malloc failed2 ");
            }
        }
        else
        {
            AI_LOG("AI__VOICE AI_Mp3InsertData malloc failed1 ");
        }

    }
    else
    {
        ai_mp3_data.data_len = data_len;
        ai_mp3_data.data = (uint8*)malloc(ai_mp3_data.data_len * sizeof(uint8));
        if(ai_mp3_data.data)
        {
           
           SCI_MEMCPY(ai_mp3_data.data, data, data_len);
           AI_StartNextMp3PlayTimer();
          
        }
        else
        {
           AI_LOG("AI__VOICE AI_Mp3InsertData malloc failed3 ");
        }

    }


}

// old use
/*PUBLIC uint8 AI_Mp3BufList_InsertData(uint8* data, uint32 data_len, BOOLEAN *result)
{
    
    Mp3BufList_T *new_item_ptr=NULL; 
    Mp3BufList_T *temp_ptr=NULL;
    uint8 list_size = 0;
    AI_LOG("AI__VOICE AI_Mp3BufList_insertData data_len=%d \n", data_len);
    if(data_len <= 0 || data == NULL)
    {
        return 0;
    } 
    if(!AI_IsInAiChatWin())
    {
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData not in ai win, clear list");  
        AI_Mp3BufList_FreeAll();
        return 0;
    }
    
    if(g_ai_app.m_ai_eRecorderStatus == AI_RECORDER_RECORD)
    {
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData is recording, can not add to play");
        return 0;
    }
    
    if(ai_mp3_buf_list_ptr == NULL)
    {
        ai_mp3_buf_list_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T));
        if(ai_mp3_buf_list_ptr)
        {
            ai_mp3_buf_list_ptr->next = NULL;
            ai_mp3_buf_list_ptr->mp3_data.data_len= data_len;
            ai_mp3_buf_list_ptr->mp3_data.data = (uint8*)malloc(data_len*sizeof(uint8));
            list_size = 1;
            if(ai_mp3_buf_list_ptr->mp3_data.data)
            {
            
                SCI_MEMCPY(ai_mp3_buf_list_ptr->mp3_data.data, data, data_len);
                *result = TRUE;
            }
            else
            {
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc failed1 ");
            }
           
        }
        else
        {
            AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc failed2 ");

        }
        
    }
    else
    {
        list_size = 1;
        new_item_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T));
        if(new_item_ptr)
        {
            new_item_ptr->next = NULL;
            new_item_ptr->mp3_data.data_len = data_len;
            new_item_ptr->mp3_data.data = (uint8*)malloc(data_len*sizeof(uint8));
            if(new_item_ptr->mp3_data.data)
            {

                SCI_MEMCPY(new_item_ptr->mp3_data.data, data, data_len);
                temp_ptr = ai_mp3_buf_list_ptr;
                
                while(temp_ptr->next != NULL)
                {
                    temp_ptr = temp_ptr->next;
                    list_size++;

                }
                temp_ptr->next = new_item_ptr;
                list_size = list_size + 1;
                *result = TRUE;
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData list_size=%d ",list_size);
            }
            else
            {
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc failed4 ");
            }
           

        }
        else 
        {
            AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc failed3 ");

        }
       

    }
    return list_size;
}
*/

LOCAL uint32 GetAlignmentLen(uint32 len)
{
    return len + (len%4 != 0) ? (4 - len%4) : 0;
}

#if 0 
PUBLIC uint8 AI_Mp3BufList_InsertData(uint8* data, uint32 data_len, BOOLEAN *result)
{
    
    Mp3BufList_T *new_item_ptr=NULL; 
    Mp3BufList_T *temp_ptr=NULL;
    uint8 list_size = 0;
    BOOLEAN allocNewItemOk = FALSE;
    BOOLEAN allocNewDataOk = FALSE;

    *result = FALSE;
    AI_LOG("AI__VOICE AI_Mp3BufList_insertData data_len=%d \n", data_len);
    if(data_len <= 0 || data == NULL)
    {
        return 0;
    } 
    if(!AI_IsInAiChatWin())
    {
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData not in ai win, clear list");  
        AI_Mp3BufList_FreeAll();
        return 0;
    }
    
    if(g_ai_app.m_ai_eRecorderStatus == AI_RECORDER_RECORD)
    {
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData is recording, can not add to play");
        return 0;
    }
    ai_is_inserting_mp3 = TRUE;
    if(ai_mp3_buf_list_ptr == NULL)
    {
        ai_mp3_buf_list_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T));
        if(ai_mp3_buf_list_ptr)
        {
            ai_mp3_buf_list_ptr->next = NULL;
            ai_mp3_buf_list_ptr->mp3_data.alloc_len= data_len;//data_len > (128*1024) ? data_len : (128*1024);//GetAlignmentLen(data_len);//data_len;
            ai_mp3_buf_list_ptr->mp3_data.data_len= data_len;
            //ai_mp3_buf_list_ptr->mp3_data.data = (uint8*)malloc(data_len*sizeof(uint8));
            ai_mp3_buf_list_ptr->mp3_data.data = (uint8*)malloc(ai_mp3_buf_list_ptr->mp3_data.alloc_len);//(uint8*)malloc(GetAlignmentLen(data_len));
            list_size = 1;
            if(ai_mp3_buf_list_ptr->mp3_data.data)
            {
                ai_mp3_buf_list_ptr->mp3_data.buf_index = list_size -1;
                SCI_MEMCPY(ai_mp3_buf_list_ptr->mp3_data.data, data, data_len);
                *result = TRUE;
                ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_ptr;
            }
            else
            {
                SCI_FREE(ai_mp3_buf_list_ptr);
                ai_mp3_buf_list_ptr = NULL;
                list_size = 0;
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc failed1 ");
            }
           
        }
        else
        {
            AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc failed2 ");

        }
        
    }
    else
    {
        list_size = 1;
        temp_ptr = ai_mp3_buf_list_ptr;
                
        while(temp_ptr->next != NULL)
        {
            temp_ptr = temp_ptr->next;
            list_size++;
        }
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData current list_size = %d", list_size);

        if(list_size == 1 )
        {
            new_item_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T)); 
            if(new_item_ptr)
            {
                allocNewItemOk = TRUE;
                new_item_ptr->next = NULL;
                new_item_ptr->mp3_data.alloc_len = data_len;//data_len > (128*1024) ? data_len : (128*1024);//data_len;
                //new_item_ptr->mp3_data.alloc_len = data_len;// > (128*1024) ? data_len : (128*1024);
                new_item_ptr->mp3_data.data_len = 0;
                new_item_ptr->mp3_data.data = (uint8*)malloc(new_item_ptr->mp3_data.alloc_len * sizeof(uint8));
                if(new_item_ptr->mp3_data.data)
                {
                    allocNewDataOk = TRUE;
                    SCI_MEMCPY(new_item_ptr->mp3_data.data, data, data_len);
                    new_item_ptr->mp3_data.data_len = data_len;
                    
                    temp_ptr->next = new_item_ptr;
                    list_size = list_size + 1;
                    new_item_ptr->mp3_data.buf_index = list_size -1;
                    *result = TRUE;
                    //return list_size;
                }
                else
                {
                    SCI_FREE(new_item_ptr);
                    new_item_ptr = NULL;
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc data failed1 ");

                }
            }
            else
            {
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc item failed1 ");
            }

        }
        else if(list_size >= 2)
        {
            
            if(temp_ptr && ai_mp3_buf_list_play_ptr && temp_ptr->mp3_data.buf_index != ai_mp3_buf_list_play_ptr->mp3_data.buf_index 
                && temp_ptr != ai_mp3_buf_list_play_ptr && temp_ptr->mp3_data.alloc_len >= (temp_ptr->mp3_data.data_len + data_len))
            {
                SCI_MEMCPY(temp_ptr->mp3_data.data + temp_ptr->mp3_data.data_len, data, data_len);
                temp_ptr->mp3_data.data_len += data_len;
                *result = TRUE;
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData %d , new data_len=%d ",list_size, temp_ptr->mp3_data.data_len);
            }
            else
            {
                new_item_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T)); 
                if(new_item_ptr)
                {
                    allocNewItemOk = TRUE;
                    new_item_ptr->next = NULL;

                    new_item_ptr->mp3_data.alloc_len = data_len > (128*1024) ? data_len : (128*1024);//if give too big and net speed slow ,will have bug
                    new_item_ptr->mp3_data.data_len = 0;
                    new_item_ptr->mp3_data.data = (uint8*)malloc(new_item_ptr->mp3_data.alloc_len * sizeof(uint8));
                    if(new_item_ptr->mp3_data.data)
                    {
                        allocNewDataOk = TRUE;
                        SCI_MEMCPY(new_item_ptr->mp3_data.data, data, data_len);
                        new_item_ptr->mp3_data.data_len = data_len;
                        temp_ptr->next = new_item_ptr;
                        list_size = list_size + 1;
                        new_item_ptr->mp3_data.buf_index = list_size -1;
                        *result = TRUE;

                    }
                    else
                    {
                        SCI_FREE(new_item_ptr);
                        new_item_ptr = NULL;
                        AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc data failed2 ");

                    }
                }
                else
                {
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc item failed2 ");

                }

            }
        }
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData after list_size = %d", list_size);
    }
    ai_is_inserting_mp3 = FALSE;
    if(ai_is_waiting_next_mp3 && *result && list_size > 1 && !ai_is_mp3_playing )
    {
        if(ai_mp3_buf_list_play_ptr && ai_mp3_buf_list_play_ptr->next != NULL)
        {
            ai_is_waiting_next_mp3 = FALSE;
            ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;
            AI_API_Rcv_Mp3_PlayStart();//no bug, but play discontinus
        }
    }
    /*if(*result && list_size > 1 && ai_mp3_buf_list_play_ptr && ai_mp3_buf_list_play_ptr->mp3_data.buf_index > 0 && ai_mp3_buf_list_play_ptr->next == NULL)
    { 
        ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;
        AI_StopNextMp3PlayTimer();
        AI_API_Rcv_Mp3_PlayStart();
    }*/
    return list_size;
}
#endif

LOCAL void NotifyStartNextBufferDownload()
{
    AI_LOG("AI__VOICE NotifyStartNextBufferDownload");

    MMIAI_Http_Request_SendMsg(); //break continue downloadsss
}

LOCAL void NotifyBufferFullOrDownloadFinish()
{
    AI_LOG("AI__VOICE NotifyBufferFullOrDownloadFinish ,is_playing = %d", ai_is_mp3_playing);
    if(ai_mp3_buf_list_play_ptr == NULL)
    {
        ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_ptr;
        AI_API_Rcv_Mp3_PlayStart();//no bug, but play discontinus
    }
    //else if(ai_is_waiting_next_mp3 && !ai_is_mp3_playing )
    else if( !ai_is_mp3_playing )
    {
        if(ai_mp3_buf_list_play_ptr != NULL && ai_mp3_buf_list_play_ptr->next != NULL)
        {
            ai_is_waiting_next_mp3 = FALSE;
            ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;
            AI_API_Rcv_Mp3_PlayStart();//no bug, but play discontinus
        }
    }
}

PUBLIC uint8 AI_Mp3BufList_InsertData(uint8* data, uint32 data_len, BOOLEAN *result,uint32 *inserted_len, BOOLEAN download_finish)
{
    
    Mp3BufList_T *new_item_ptr=NULL; 
    Mp3BufList_T *temp_ptr=NULL;
    uint8 list_size = 0;
    BOOLEAN allocNewItemOk = FALSE;
    BOOLEAN allocNewDataOk = FALSE;
    uint8 malloc_index = 0;
    *inserted_len = data_len;

    *result = FALSE;
    AI_LOG("AI__VOICE AI_Mp3BufList_insertData data_len=%d \n", data_len);

    ai_is_inserting_mp3 = FALSE;
    if(data_len <= 0 || data == NULL)
    {
        *inserted_len = 0;
        return 0;
    } 
    if(!AI_IsInAiChatWin())
    {
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData not in ai win, clear list"); 
        *inserted_len = 0;
        AI_Mp3BufList_FreeAll();
        return 0;
    }
    
    if(g_ai_app.m_ai_eRecorderStatus == AI_RECORDER_RECORD)
    {
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData is recording, can not add to play");
        *inserted_len = 0;
        return 0;
    }
    if(data_len > MP3_HTTP_BUFF_SIZE)
    {
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData data_len is overflow");
        *inserted_len = 0;
        return 0;
    }
    ai_is_inserting_mp3 = TRUE;
    if(ai_mp3_buf_list_ptr == NULL)
    {
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData , NULL ,start new ");
        ai_mp3_buf_list_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T));
        if(ai_mp3_buf_list_ptr)
        {
            ai_mp3_buf_list_ptr->next = NULL;
            ai_mp3_buf_list_ptr->mp3_data.alloc_len= MP3_HTTP_BUFF_SIZE;//data_len;//data_len > (128*1024) ? data_len : (128*1024);//GetAlignmentLen(data_len);//data_len;
            ai_mp3_buf_list_ptr->mp3_data.data_len= data_len;
            //ai_mp3_buf_list_ptr->mp3_data.data = (uint8*)malloc(data_len*sizeof(uint8));
            ai_mp3_buf_list_ptr->mp3_data.data = (uint8*)malloc(ai_mp3_buf_list_ptr->mp3_data.alloc_len);//(uint8*)malloc(GetAlignmentLen(data_len));
            //list_size = 1;
            if(ai_mp3_buf_list_ptr->mp3_data.data)
            {
                ai_mp3_buf_list_ptr->mp3_data.buf_index = 0;
                ai_mp3_buf_list_ptr->mp3_data.malloc_index= 0;
                SCI_MEMCPY(ai_mp3_buf_list_ptr->mp3_data.data, data, data_len);
                *result = TRUE;
                //ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_ptr;
                if(ai_mp3_buf_list_ptr->mp3_data.data_len == ai_mp3_buf_list_ptr->mp3_data.alloc_len || download_finish)
                {
                    NotifyBufferFullOrDownloadFinish();
                }
            }
            else
            {
                SCI_FREE(ai_mp3_buf_list_ptr);
                ai_mp3_buf_list_ptr = NULL;
                list_size = 0;
                *inserted_len = 0;
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc failed1 ");
            }
            
           
        }
        else
        {
            AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc failed2 ");
            *inserted_len = 0;

        }
        
    }
    else
    {
        uint32 remain_len = 0;
        list_size = 1;
        temp_ptr = ai_mp3_buf_list_ptr;
        
        //while(temp_ptr->next != NULL )        
        while(temp_ptr->next != NULL && temp_ptr->next->mp3_data.buf_index > temp_ptr->mp3_data.buf_index)
        {
            temp_ptr = temp_ptr->next;
            //list_size++;
        }
        malloc_index = temp_ptr->mp3_data.malloc_index;
        list_size = temp_ptr->mp3_data.buf_index;
        AI_LOG("AI__VOICE AI_Mp3BufList_insertData current list_size = %d malloc_index =%d", list_size , malloc_index);
        if(temp_ptr != NULL)
        {
            AI_LOG("AI__VOICE AI_Mp3BufList_insertData current buf_index = %d", temp_ptr->mp3_data.buf_index);
            if(temp_ptr->mp3_data.alloc_len > temp_ptr->mp3_data.data_len )
            {
                //for bug
                if(temp_ptr->mp3_data.data_len == 0)
                {
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData clear old data to 0");
                    SCI_MEMSET(temp_ptr->mp3_data.data, 0, temp_ptr->mp3_data.alloc_len);

                }
                //end
                if(temp_ptr->mp3_data.data_len + data_len > temp_ptr->mp3_data.alloc_len)
                {
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData problem, data over  MP3_HTTP_BUFF_SIZE, , buf_index=%d  malloc_index=%d",temp_ptr->mp3_data.buf_index, temp_ptr->mp3_data.malloc_index);
                    SCI_MEMCPY(temp_ptr->mp3_data.data + temp_ptr->mp3_data.data_len, data, temp_ptr->mp3_data.alloc_len - temp_ptr->mp3_data.data_len);
                    remain_len = data_len-(temp_ptr->mp3_data.alloc_len - temp_ptr->mp3_data.data_len);
                    temp_ptr->mp3_data.data_len = temp_ptr->mp3_data.alloc_len;
                    
                    *result = TRUE;
                    NotifyBufferFullOrDownloadFinish();
                }
                else if(temp_ptr->mp3_data.data_len + data_len == temp_ptr->mp3_data.alloc_len)
                {
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData ok, one MP3_HTTP_BUFF_SIZE download finish, , buf_index=%d  malloc_index=%d",temp_ptr->mp3_data.buf_index, temp_ptr->mp3_data.malloc_index);
                    SCI_MEMCPY(temp_ptr->mp3_data.data + temp_ptr->mp3_data.data_len, data, data_len);
                    temp_ptr->mp3_data.data_len += data_len;
                    *result = TRUE;
                    NotifyBufferFullOrDownloadFinish();
                }
                else
                {
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData ok, one MP3_HTTP_BUFF_SIZE not fill full,, buf_index=%d  malloc_index=%d",temp_ptr->mp3_data.buf_index, temp_ptr->mp3_data.malloc_index);
                    SCI_MEMCPY(temp_ptr->mp3_data.data + temp_ptr->mp3_data.data_len, data, data_len);
                    temp_ptr->mp3_data.data_len += data_len;
                    *result = TRUE;
                    if(download_finish)
                    {
                        NotifyBufferFullOrDownloadFinish();
                    }
                }
                if(remain_len > 0)
                {
                    BOOLEAN use_replace = FALSE;
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData have remain len, abnormal , remain_len = %d",remain_len);
                    if( temp_ptr->next == NULL && temp_ptr->mp3_data.malloc_index < MP3_HTTP_BUFF_COUNT - 1)
                    {
                        new_item_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T));
                    }
                    else 
                    {
                        if(temp_ptr->mp3_data.malloc_index == MP3_HTTP_BUFF_COUNT - 1)
                        {
                            temp_ptr->next = ai_mp3_buf_list_ptr;
                        }
 
                        if(!ai_is_mp3_playing || (ai_is_mp3_playing && ai_mp3_buf_list_play_ptr != temp_ptr->next))
                        {
                            
                            new_item_ptr = temp_ptr->next;
                            use_replace= TRUE;
                            SCI_MEMSET(new_item_ptr->mp3_data.data, 0, new_item_ptr->mp3_data.alloc_len);
                            new_item_ptr->mp3_data.data_len = 0;
                        }
                        else
                        {
                            AI_LOG("AI__VOICE AI_Mp3BufList_insertData next buff is playing, not insert");
                            *result = FALSE;
                            *inserted_len = data_len - remain_len;
                            ai_is_inserting_mp3 = FALSE;
                            return 0;
                        }
                    }
                     
                    if(new_item_ptr != NULL)
                    {
                        allocNewItemOk = TRUE;
                        if(!use_replace) 
                        {
                            new_item_ptr->next = NULL;
                            new_item_ptr->mp3_data.alloc_len = MP3_HTTP_BUFF_SIZE;//data_len;//data_len > (128*1024) ? data_len : (128*1024);//data_len;
                           
                            new_item_ptr->mp3_data.data_len = 0;
                            new_item_ptr->mp3_data.data = (uint8*)malloc(new_item_ptr->mp3_data.alloc_len * sizeof(uint8));
                        }
                        
                        if(new_item_ptr->mp3_data.data != NULL)
                        {
                            allocNewDataOk = TRUE;
                            
                            SCI_MEMCPY(new_item_ptr->mp3_data.data, data+(data_len-remain_len), remain_len);
                            new_item_ptr->mp3_data.data_len = remain_len;
                            
                            if(!use_replace) temp_ptr->next = new_item_ptr;
                            if(!use_replace) new_item_ptr->mp3_data.malloc_index = ++malloc_index;
                            new_item_ptr->mp3_data.buf_index = ++list_size;//list_size -1;
                            
                            AI_LOG("AI__VOICE AI_Mp3BufList_insertData remain insert, , buf_index=%d  malloc_index=%d",new_item_ptr->mp3_data.buf_index, new_item_ptr->mp3_data.malloc_index);
                            *result = TRUE;
                            //return list_size;
                        }
                        else
                        {
                            if(!use_replace) SCI_FREE(new_item_ptr);
                            new_item_ptr = NULL;
                            *inserted_len = data_len - remain_len;
                            AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc data failed1 ");
        
                        }
                    }
                    else
                    {
                        *inserted_len = data_len - remain_len;
                        AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc item failed1 ");
                    }

                }
            }
            else if(temp_ptr->mp3_data.alloc_len == temp_ptr->mp3_data.data_len)
            {
                BOOLEAN use_replace = FALSE;
                
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData buff full , next new or replace");
                /*if(ai_mp3_buf_list_play_ptr != NULL && ai_mp3_buf_list_play_ptr->mp3_data.malloc_index > 0 && temp_ptr->mp3_data.malloc_index == MP3_HTTP_BUFF_COUNT -1)
                {
                    new_item_ptr = ai_mp3_buf_list_ptr;
                    SCI_MEMSET(new_item_ptr->mp3_data.data, 0, new_item_ptr->mp3_data.alloc_len);
                    new_item_ptr->mp3_data.data_len = 0;
                    use_replace = TRUE;
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData replace  buf_index = %d ,malloc_index=%d " , new_item_ptr->mp3_data.buf_index, new_item_ptr->mp3_data.malloc_index);
                }
                else
                {
                    new_item_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T)); 
                }*/
                if( temp_ptr->next == NULL && temp_ptr->mp3_data.malloc_index < MP3_HTTP_BUFF_COUNT - 1)
                {
                    new_item_ptr = (Mp3BufList_T*)malloc(sizeof(Mp3BufList_T));
                }
                else 
                {
                    if(temp_ptr->mp3_data.malloc_index == MP3_HTTP_BUFF_COUNT - 1)
                    {
                        temp_ptr->next = ai_mp3_buf_list_ptr;
                    }

                    if(!ai_is_mp3_playing || (ai_is_mp3_playing && ai_mp3_buf_list_play_ptr != temp_ptr->next))
                    {
                        
                        new_item_ptr = temp_ptr->next;
                        use_replace= TRUE;
                        SCI_MEMSET(new_item_ptr->mp3_data.data, 0, new_item_ptr->mp3_data.alloc_len);
                        new_item_ptr->mp3_data.data_len = 0;
                    }
                    else
                    {
                        *inserted_len = 0;
                        AI_LOG("AI__VOICE AI_Mp3BufList_insertData next buff is playing 2, not insert, playing=%d", ai_is_mp3_playing);
                        *result = FALSE;
                        ai_is_inserting_mp3 = FALSE;
                        return 0;
                    }
                }
                if(new_item_ptr != NULL)
                {
                    allocNewItemOk = TRUE;
                    if(!use_replace) 
                    {
                        new_item_ptr->next = NULL;
                        new_item_ptr->mp3_data.alloc_len = MP3_HTTP_BUFF_SIZE;//data_len;//data_len > (128*1024) ? data_len : (128*1024);//data_len;
                       
                        new_item_ptr->mp3_data.data_len = 0;
                        new_item_ptr->mp3_data.data = (uint8*)malloc(new_item_ptr->mp3_data.alloc_len * sizeof(uint8));
                    }
                    if(new_item_ptr->mp3_data.data != NULL)
                    {
                        allocNewDataOk = TRUE;
                        AI_LOG("AI__VOICE AI_Mp3BufList_insertData NEW insert, use_replace=%d, buf_index=%d  malloc_index=%d",use_replace,new_item_ptr->mp3_data.buf_index, new_item_ptr->mp3_data.malloc_index);
                        SCI_MEMCPY(new_item_ptr->mp3_data.data, data, data_len);
                        new_item_ptr->mp3_data.data_len = data_len;
                        
                        if(!use_replace) temp_ptr->next = new_item_ptr;
                        
                        if(!use_replace) new_item_ptr->mp3_data.malloc_index = ++malloc_index;
                        new_item_ptr->mp3_data.buf_index = ++list_size;//list_size -1;
                        AI_LOG("AI__VOICE AI_Mp3BufList_insertData NEW insert end, use_replace=%d, buf_index=%d  malloc_index=%d",use_replace,new_item_ptr->mp3_data.buf_index, new_item_ptr->mp3_data.malloc_index);
                        *result = TRUE;
                        //return list_size;
                        if(download_finish)
                        {
                            NotifyBufferFullOrDownloadFinish();
                        }
                    }
                    else
                    {
                        if(!use_replace) SCI_FREE(new_item_ptr);
                        new_item_ptr = NULL;
                        *inserted_len = 0;
                        AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc data failed1 ");
    
                    }
                }
                else
                {
                    *inserted_len = 0;
                    AI_LOG("AI__VOICE AI_Mp3BufList_insertData malloc item failed1 ");
                }

            }
            else
            {
                *inserted_len = 0;
                AI_LOG("AI__VOICE AI_Mp3BufList_insertData error old data_len ");

            }

            

        }
    }   
    AI_LOG("AI__VOICE AI_Mp3BufList_insertData end, data_len = %ld, inserted_len=%ld", data_len ,*inserted_len);

      
    ai_is_inserting_mp3 = FALSE;
    
    /*if(ai_is_waiting_next_mp3 && *result && list_size > 1 && !ai_is_mp3_playing )
    {
        if(ai_mp3_buf_list_play_ptr && ai_mp3_buf_list_play_ptr->next != NULL)
        {
            ai_is_waiting_next_mp3 = FALSE;
            ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;
            AI_API_Rcv_Mp3_PlayStart();//no bug, but play discontinus
        }
    }*/
    /*if(*result && list_size > 1 && ai_mp3_buf_list_play_ptr && ai_mp3_buf_list_play_ptr->mp3_data.buf_index > 0 && ai_mp3_buf_list_play_ptr->next == NULL)
    { 
        ai_mp3_buf_list_play_ptr = ai_mp3_buf_list_play_ptr->next;
        AI_StopNextMp3PlayTimer();
        AI_API_Rcv_Mp3_PlayStart();
    }*/
    return list_size;
}




PUBLIC uint8 AI_Mp3FixBuf_InsertData(uint8* data, uint32 data_len)
{
    BOOLEAN isAlreadyInit = ai_mp3_fix_buf_holder.is_init;
    uint16 i = 0;
    AI_LOG("AI__VOICE AI_Mp3FixBuf_InsertData data_len=%d \n", data_len);
    if(data_len <= 0 || data == NULL)
    {
        return 0;
    } 
    
    if(!AI_IsInAiChatWin())
    {
        AI_LOG("AI__VOICE AI_Mp3FixBuf_InsertData not in ai win, clear list");  
        AI_Mp3FixBuf_FreeAll();
        return 0;
    }

    
    if(!ai_mp3_fix_buf_holder.is_init)
    {
        //SCI_MEMSET(&ai_mp3_fix_buf_holder, 0 , sizeof(Mp3FixBufHolder_T));
        SCI_MEMSET(ai_mp3_fix_buf_holder.data_len, 0 , AI_FIX_BUF_COUNT*sizeof(uint32));
        ai_mp3_fix_buf_holder.inserting_index = 0;
        ai_mp3_fix_buf_holder.play_index = 0;
        ai_mp3_fix_buf_holder.is_init = TRUE;
    }

    for(i = 0 ; i < AI_FIX_BUF_COUNT; i++)
    {
        
        if(ai_mp3_fix_buf_holder.data[i] == NULL)
        {
            ai_mp3_fix_buf_holder.data[i] = (uint8 *)malloc(AI_FIX_BUF_SIZE*sizeof(uint8));
            if(ai_mp3_fix_buf_holder.data[i] == NULL) 
            {
                AI_LOG("AI__VOICE AI_Mp3FixBuf_InsertData malloc 5 buff failed = %d", i); 

            }
        }
    }

    if(!ai_is_mp3_playing)
    {
        
        if(ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index] + data_len <= AI_FIX_BUF_SIZE)
        {
           

        }
        else
        {
            ai_mp3_fix_buf_holder.inserting_index += 1;
            if(ai_mp3_fix_buf_holder.inserting_index >= AI_FIX_BUF_COUNT)
            {
                ai_mp3_fix_buf_holder.inserting_index = 0;

            }
            
            if(ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index])
            {
                ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index] = 0;
                SCI_MEMSET(ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index], 0 , AI_FIX_BUF_SIZE);
            }
        }
        if(ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index])
        {
             
            SCI_MEMCPY(&ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index][ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index]]
                , data , data_len);
            ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index] += data_len; 
        }
          
    }
    else
    {
        if(ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index] + data_len <= AI_FIX_BUF_SIZE)
        {
            if(ai_mp3_fix_buf_holder.inserting_index == ai_mp3_fix_buf_holder.play_index)
            {
                ai_mp3_fix_buf_holder.inserting_index += 1;
                if(ai_mp3_fix_buf_holder.inserting_index >= AI_FIX_BUF_COUNT)
                {
                    ai_mp3_fix_buf_holder.inserting_index = 0;

                }
                if(ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index])
                {
                    ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index] = 0;
                    SCI_MEMSET(ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index], 0 , AI_FIX_BUF_SIZE);
                }

            }

        }
        else
        {
            ai_mp3_fix_buf_holder.inserting_index += 1;
            if(ai_mp3_fix_buf_holder.inserting_index >= AI_FIX_BUF_COUNT)
            {
                ai_mp3_fix_buf_holder.inserting_index = 0;

            }
            if(ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index])
            {
                ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index] = 0;
                SCI_MEMSET(ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index], 0 , AI_FIX_BUF_SIZE);
            }
            if(ai_mp3_fix_buf_holder.inserting_index == ai_mp3_fix_buf_holder.play_index)
            {
                AI_LOG("AI__VOICE AI_Mp3FixBuf_InsertData erro ,download too fast, over one circle, need more big size");
            }
        }
        if(ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index])
        {
             
            SCI_MEMCPY(&ai_mp3_fix_buf_holder.data[ai_mp3_fix_buf_holder.inserting_index][ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index]]
                , data , data_len);
            ai_mp3_fix_buf_holder.data_len[ai_mp3_fix_buf_holder.inserting_index] += data_len; 
        }
        

    }
    
    //if(!ai_is_mp3_playing && !isAlreadyInit && ai_mp3_fix_buf_holder.is_init)
    if(ai_mp3_fix_buf_holder.inserting_index == 0 && ai_mp3_fix_buf_holder.play_index == 0)
    {
        AI_API_Rcv_Mp3_PlayStart();
    }
    
    
    return ai_mp3_fix_buf_holder.inserting_index;
}




/*
BOOLEAN AI_Voice_Play_CallBack(AI_APP_T * pMe,AIFilePlayErrStatus err_code)
{
    if(AI_FILEPLAY_SUCCESS == err_code)
    {
        //MMIZDT_UpdateTalkTPWin();
    }
    else
    {
        char err_str[100] = {0};
        sprintf(err_str,"Err=%d",err_code);
        MMIZDT_UpdateTalkTPWinErr(0,err_str);
    }
    return TRUE;
}

LOCAL BOOLEAN AI_Voice_Play_RequestHandle( 
                        MMISRV_HANDLE_T *audio_handle,
                        MMISRVAUD_ROUTE_T route,
                        MMISRVAUD_TYPE_U *audio_data,
                        MMISRVMGR_NOTIFY_FUNC notify,
						MMISRVAUD_VOLUME_T volume
                        )
{
    MMISRVMGR_SERVICE_REQ_T req = {0};
    MMISRVAUD_TYPE_T audio_srv = {0};
    
    req.notify = notify;
    req.pri = MMISRVAUD_PRI_NORMAL;

    audio_srv.duation = 0;
    audio_srv.eq_mode = 0;
    audio_srv.is_mixing_enable = FALSE;
    audio_srv.play_times = 1;
    audio_srv.all_support_route = route;
    audio_srv.volume =  volume;

    switch(audio_data->type)
    {
    case MMISRVAUD_TYPE_RING_FILE:
        audio_srv.info.ring_file.type = audio_data->type;
        audio_srv.info.ring_file.fmt  = audio_data->ring_file.fmt;
        audio_srv.info.ring_file.name = audio_data->ring_file.name;
        audio_srv.info.ring_file.name_len = audio_data->ring_file.name_len;

        if(audio_srv.volume > MMISRVAUD_VOLUME_LEVEL_MAX)
        {
            audio_srv.volume = MMISRVAUD_VOLUME_LEVEL_MAX;
        }
        break;

    case MMISRVAUD_TYPE_RECORD_FILE:
        audio_srv.info.record_file.type = audio_data->type;        
        audio_srv.info.record_file.fmt  = audio_data->record_file.fmt;
        audio_srv.info.record_file.name = audio_data->record_file.name;
        audio_srv.info.record_file.name_len = audio_data->record_file.name_len;    
        audio_srv.info.record_file.source   = audio_data->record_file.source;
        audio_srv.info.record_file.frame_len= audio_data->record_file.frame_len;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;

    case MMISRVAUD_TYPE_IQ://测试模式
        audio_srv.info.iq_file.type = audio_data->type;
        audio_srv.info.iq_file.name_ptr = audio_data->iq_file.name_ptr;
        audio_srv.info.iq_file.param    = audio_data->iq_file.param;

        audio_srv.volume = AUD_MAX_SPEAKER_VOLUME;
        break;
        
    default:
        break;
    }

    *audio_handle = MMISRVMGR_Request(STR_SRV_AUD_NAME, &req, &audio_srv);

    if(*audio_handle > 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

int AI_Voice_FilePlay(AI_APP_T * pMe, const char * file_name)
{
    MMISRVAUD_TYPE_U    audio_data  = {0};
    MMISRV_HANDLE_T audio_handle = PNULL;
    uint16		full_path[MAX_AI_VOC_FILENAME_SIZE+1] = {0};
    uint16		full_path_len = 0;
    int Ret = 0;
    if(file_name == PNULL)
    {
        return 1;
    }
    
    if(pMe->m_ai_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
        pMe->m_ai_pPlayMedia = NULL;
    }

    full_path_len = GUI_GBToWstr(full_path, (const uint8*)file_name, SCI_STRLEN(file_name));
    audio_data.ring_file.type = MMISRVAUD_TYPE_RING_FILE;
    audio_data.ring_file.name = full_path;
    audio_data.ring_file.name_len = full_path_len;
    audio_data.ring_file.fmt  = (MMISRVAUD_RING_FMT_E)MMIAPICOM_GetMusicType(audio_data.ring_file.name, audio_data.ring_file.name_len);
    
    if(AI_Voice_Play_RequestHandle(&audio_handle, MMISRVAUD_ROUTE_NONE, &audio_data, AI_Voice_PlayNotify,s_ai_voc_play_vol))
    {
        pMe->m_ai_pPlayMedia = audio_handle;
        if(!MMISRVAUD_Play(audio_handle, 0))
        {			          
            MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
		pMe->m_ai_pPlayMedia = 0;		
            Ret = 2;
        }
    }
    else
    {        
        Ret = 3;
    }
	  
    if (Ret != 0)
    {
        pMe->m_ai_pPlayMedia = NULL;
        pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYSTOP;
        AI_LOG("AI__VOICE Voice_RecordPlay Err Play Ret=%d",Ret);
        return Ret;
    }
    else
    {
        AI_LOG("AI__VOICE Voice_RecordPlay OK vol=%d",s_ai_voc_play_vol);
        pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYING;        
    }
    return Ret;
}

BOOLEAN AI_Voice_FilePlay_Start(AI_APP_T * pMe)
{
    BOOLEAN Ret = FALSE;
    uint8 need_play = 0;
    
    AI_LOG("AI__VOICE AI_Voice_FilePlay_Start is_playing = %d,%d",pMe->m_ai_voc_is_playing,s_ai_voc_is_playing);

    if(pMe->m_ai_voc_is_playing)
    {
        AI_Voice_Play_CallBack(pMe,AI_FILEPLAY_ERR_PLAYING);
        return FALSE;
    }

#if 0
    if(CallUtil_Get_Calldraw_Status() != CALLDRAW_STATUS_NONE)
    {
        AI_Voice_Play_CallBack(pMe,AI_FILEPLAY_ERR_INCALL);
        return FALSE;
    }
#endif
    if(s_ai_voc_is_recording)
    {
        AI_Voice_Play_CallBack(pMe,AI_FILEPLAY_ERR_RECORRDING);
        return FALSE;
    }
    
#ifdef AI_DB_NEED_TIMER
    if(AI_DB_TIMER_IsValid_Slient())
    {
        AI_Voice_Play_CallBack(pMe,AI_FILEPLAY_ERR_HIDDEN);
        return FALSE;
    }
#endif
    AI_Voice_Play_TimerStop(pMe);
    if(AI_DB_VocRcv_ListGetFirstByTime(pMe,&pMe->m_ai_pDB_vocrcv_CurList))
    {
        need_play = 1;
    }
    else if(pMe->m_ai_pDB_vocrcv_last_CurList.list_id > 0)
    {
        pMe->m_ai_pDB_vocrcv_CurList = pMe->m_ai_pDB_vocrcv_last_CurList;
        need_play = 2;
    }
    if(need_play)
    {
        int play_res = 0;
        play_res = AI_Voice_FilePlay(pMe,pMe->m_ai_pDB_vocrcv_CurList.r_name);
        if(0 == play_res)
        {
            char file_name[100] = {0};
            pMe->m_ai_voc_is_playing = TRUE;
            s_ai_voc_is_playing = TRUE;
            Ret = TRUE;
            AI_Voice_Play_CallBack(pMe,AI_FILEPLAY_SUCCESS);
            AI_Func_GetFileName(pMe->m_ai_pDB_vocrcv_CurList.r_name,file_name);
            MMIZDT_UpdateTalkTPWinErr(0,file_name);
        }
        else
        {
            AI_File_Remove((char *)pMe->m_ai_pDB_vocrcv_CurList.r_name);
            if(need_play == 1)
            {
                AI_DB_VocRcvDel(pMe,pMe->m_ai_pDB_vocrcv_CurList.list_id);
            }
            else if(need_play == 2)
            {
                pMe->m_ai_pDB_vocrcv_last_CurList.list_id = 0;
            }
            if(s_voc_is_play_all)
            {
                if(AI_Voice_Play_Valid() && need_play == 1)
                {
                    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_FILEPLAY,NULL,0);
                }
                else
                {
                    AI_Voice_FilePlay_Stop(pMe);
                }
            }
            else
            {
                AI_Voice_FilePlay_Stop(pMe);
            }
            AI_Voice_Play_CallBack(pMe,AI_FILEPLAY_ERR);
        }
    }
    else
    {
        AI_LOG("AI__VOICE AI_Voice_FilePlay_Start OVER %d",AI_DB_GetVocRcvNum());
        AI_Voice_FilePlay_Stop(pMe);
    }
    return Ret;

}

BOOLEAN AI_Voice_FilePlay_Stop(AI_APP_T * pMe)
{
#ifdef TOUCH_PANEL_SUPPORT
    BOOLEAN is_play = s_ai_voc_is_playing;
#endif
    BOOLEAN               result = FALSE;
    AI_LOG("AI__VOICE AI_Voice_FilePlay_Stop");
    AI_Voice_Play_TimerStop(pMe);

    if(pMe->m_ai_pPlayMedia)
    {
        result = MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        if (result)
        {
            MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
            pMe->m_ai_pPlayMedia = NULL;
        }
    }
    pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYSTOP;
    pMe->m_ai_voc_is_playing = FALSE;
    s_ai_voc_is_playing = FALSE;
    if(AI_DB_GetVocRcvNum() > 0)
    {
        //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_VOC_NEW);
    }
    else
    {
        if(pMe->m_ai_pDB_vocrcv_last_CurList.list_id == 0)
        {
            AI_Voice_FileRcvAllRemove(pMe);
        }
        //IANNUNCIATOR_SetField((IAnnunciator*)CoreApp_GetAnnunciator(),ANNUN_FIELD_VOC,ANNUN_STATE_OFF);
    }
#ifdef TOUCH_PANEL_SUPPORT
    if(is_play)
    {
        MMIZDT_UpdateTalkTPWin();
        //ZdtApp_AI_VocMenu_RecordRsp(pMe,AI_VOCRSP_TYPE_PLAYSTOP,0);
    }
#endif
    return TRUE;
}

BOOLEAN AI_Voice_FilePlay_SetVolume(AI_APP_T * pMe,uint16 vol)
{
    AI_LOG("AI__VOICE AI_Voice_FilePlay_SetVolume vol = %d,is_playing=%d",vol,s_ai_voc_is_playing);
    if(s_ai_voc_is_playing)
    {
        if(pMe->m_ai_pPlayMedia)
        {
            //IMEDIA_SetVolume(pMe->m_ai_pPlayMedia, (uint16)MIN(vol * 25 , AEE_MAX_VOLUME));
        }
    }
    return TRUE;
}

BOOLEAN AI_Voice_Record_Valid(void)
{
    BOOLEAN res = FALSE;
#ifdef ZDT_PLAT_YX_SUPPORT
    if(yx_DB_Set_Rec.net_open)
    {
        res = TRUE;
    }        
#endif
    AI_LOG("AI__VOICE AI_Voice_Record_Valid res=%d",res);
    return res;
}

BOOLEAN AI_Voice_Play_Valid(void)
{
    BOOLEAN res = FALSE;
#ifdef ZDT_PLAT_YX_SUPPORT
    if(yx_DB_Set_Rec.net_open)
    {
        res = TRUE;
    }      
#endif
    AI_LOG("AI__VOICE AI_Voice_Play_Valid res=%d",res);
    return res;
}
*/
int AI_Voice_HandleStop(AI_APP_T * pMe)
{
    AI_Voice_Record_TimerStop(pMe);
    if (PNULL != pMe->m_ai_pRecordMedia)
    {
        MMIRECORDSRV_StopRecord(pMe->m_ai_pRecordMedia);
        MMIRECORDSRV_FreeRecordHandle(pMe->m_ai_pRecordMedia);
        pMe->m_ai_pRecordMedia = 0;
    }
    s_ai_voc_is_recording = FALSE;
    pMe->m_ai_eRecorderStatus = AI_RECORDER_STOP;

    AI_Voice_Play_TimerStop(pMe);

    if(pMe->m_ai_pPlayMedia)
    {
        MMISRVAUD_Stop(pMe->m_ai_pPlayMedia);
        MMISRVMGR_Free(pMe->m_ai_pPlayMedia);
        pMe->m_ai_pPlayMedia = NULL;
    }
    pMe->m_ai_eFilePlayrStatus = AI_FILE_PLAYSTOP;
    pMe->m_ai_voc_is_playing = FALSE;
    s_ai_voc_is_playing = FALSE;
    return AEE_SUCCESS;
}

int AI_API_Record_Start(void)
{
    AI_LOG("AI__VOICE AI_API_Record_Start");

    //if(AI_Voice_Record_Valid())
    {
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RECORD_START,NULL,0);
    }
    return AEE_SUCCESS;
}

int AI_API_Rcv_Mp3_PlayStart(void)
{
    AI_LOG("AI__VOICE AI_API_Rcv_Mp3_PlayStart");

    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RCV_MP3_PLAY, NULL, 0);
    
    return AEE_SUCCESS;
}

int AI_API_Rcv_Mp3_PlayStop(void)
{
    AI_LOG("AI__VOICE AI_API_Rcv_Mp3_PlayStart");

    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RCV_MP3_STOP, NULL, 0);
    
    return AEE_SUCCESS;
}




#ifdef TOUCH_PANEL_SUPPORT
int AI_API_Record_Start_TP(void)
{
    //ISHELL_StartAppletArgs(AEE_GetShell(), AEECLSID_ZDT, "O");
    return AEE_SUCCESS;
}
#endif

int AI_API_Record_Stop(void)
{
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RECORD_STOP,NULL,0);
    return AEE_SUCCESS;
}

int AI_API_Record_Send(void)
{
    MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_RECORD_SEND,NULL,0);
    return AEE_SUCCESS;
}

BOOLEAN AI_API_Record_IsFilePlay(void)
{
    return s_ai_voc_is_playing;
}

BOOLEAN AI_API_Record_IsRecording(void)
{
    return s_ai_voc_is_recording;
}
/*
int AI_API_Record_FilePlay(void)
{
    if(AI_Voice_Play_Valid())
    {
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_FILEPLAY,NULL,0);
    }
    return AEE_SUCCESS;
}*/

int AI_API_Record_PlayStop(void)
{
    if(s_ai_voc_is_playing)
    {
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_PLAYSTOP,NULL,0);
    }
    
    return AEE_SUCCESS;
}

int AI_API_SetVocPlayVolume(uint16 vol)
{
    s_ai_voc_play_vol = vol;
    if(s_ai_voc_is_playing)
    {
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_VOLUME,NULL,vol);
        return AEE_SUCCESS;
    }
    
    return AEE_EFAILED;
}

int AI_API_SetVocPlayVolumeUp(void)
{
    if(s_ai_voc_is_playing)
    {
        if(s_ai_voc_play_vol < 4)
        {
            s_ai_voc_play_vol++;
        }
        else
        {
            s_ai_voc_play_vol = 4;
        }
        //htNVMgr_SetAIVocVolume(s_ai_voc_play_vol);
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_VOLUME,NULL,s_ai_voc_play_vol);
        //ISHELL_SendEvent(AEE_GetShell(), AEECLSID_ZDT,AI_APP_SIGNAL_AI_VOC_VOLUME,s_ai_voc_play_vol,0);
        return AEE_SUCCESS;
    }
    
    return AEE_EFAILED;
}

int AI_API_SetVocPlayVolumeDown(void)
{
    if(s_ai_voc_is_playing)
    {
        if(s_ai_voc_play_vol > 1)
        {
            s_ai_voc_play_vol--;
        }
        else
        {
            s_ai_voc_play_vol = 1;
        }
        //htNVMgr_SetAIVocVolume(s_ai_voc_play_vol);
        MMIAI_SendSigTo_APP(AI_APP_SIGNAL_AI_VOC_VOLUME,NULL,s_ai_voc_play_vol);
        //ISHELL_SendEvent(AEE_GetShell(), AEECLSID_ZDT,AI_APP_SIGNAL_AI_VOC_VOLUME,s_ai_voc_play_vol,0);
        return AEE_SUCCESS;
    }
    
    return AEE_EFAILED;
}


void AI_API_Record_SetPlayAll(BOOLEAN is_play_all)
{
    s_voc_is_play_all = is_play_all;
    return;
}

BOOLEAN AI_API_Record_FileDelAll(AI_APP_T * pMe)
{
    AI_Voice_FileSendAllRemove(pMe);
    //AI_Voice_FileRcvAllRemove(pMe);
    return TRUE;
}

#endif

