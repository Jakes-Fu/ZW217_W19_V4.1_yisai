#include "sci_types.h"
#include "os_api.h"

#include "sprd_fts_type.h"
#include "sprd_fts_log.h"

#include "chnl.h"
#include "chnl_sipc.h"
#include "chnl_pc.h"
#include "chnl_sd.h"

//#include "ulog.h"

//engpc thread: cp response to pc tool
#define P_ENGPC_CP_RSP2PC_STACK_SIZE     (1024*4)
#define P_ENGPC_CP_RSP2PC_QUEUE_NUM      15    
#define P_ENGPC_CP_RSP2PC_TASK_PRIORITY  78 //31


static BLOCK_ID s_engpc_cp_diag_task = SCI_INVALID_BLOCK_ID;
static BLOCK_ID s_engpc_cp_at_task = SCI_INVALID_BLOCK_ID;

#define CP_DIAG_RSP_BUFF_SIZE (64*1024)
//#define CP_DIAG_RSP_BUFF_SIZE (4)
#define CP_AT_RSP_BUFF_SIZE     (4*1024)

LOCAL uint8 s_rsp_cp_diag_ptr[CP_DIAG_RSP_BUFF_SIZE] = {0};
LOCAL uint8 s_rsp_cp_at_ptr[CP_AT_RSP_BUFF_SIZE] = {0};

#ifdef UAL_AT_SUPPORT
typedef void(*func_ptr)( char*, int);
func_ptr s_at_call_back = 0;
#endif

#define FLAG_BYTE         0x7E

typedef enum{
    DIAG_STATUS_NONE,
    DIAG_STATUS_HEAD,
    DIAG_STATUS_TAIL
}DIAG_FRAME_SATATUS;

DIAG_FRAME_SATATUS s_frame_status_cur = DIAG_STATUS_NONE;
DIAG_FRAME_SATATUS s_frame_status_last = DIAG_STATUS_NONE;

int findDiagFlag(uint8 *src, int len){
    int i = 0;
    while(len-- > 0){
        if(*(src+i) == FLAG_BYTE){
            break;
        }
        i++;
    }

    return i;
}

int countDiagFlag(uint8 *src, int len){
    int cnt = 0, i = 0;
    while(len-- > 0){
        if(*(src+i) == FLAG_BYTE){
            cnt++;
        }
        i++;
    }

    return cnt;
}

extern void SIO_SetCmdReq(BOOLEAN req);
extern int s_cpdump_flag;

#if 0
uint8 s_cpdump_cmd[] = {0x74, 0x0A};
PUBLIC void CPDumpCmd()
{
    SCI_Sleep(500);
    if(ulog_get_log2pc(ULOG_MODEM) != 1) {
        ENG_LOG("log to sdcard, so send CPDumpCmd.....");
        cp_write_data(CP_DIAG, s_cpdump_cmd, sizeof(s_cpdump_cmd));
    }
}

LOCAL BOOLEAN isDumpToSD(){
    int log2pc = ulog_get_log2pc(ULOG_MODEM);
    BOOLEAN isAssert = chnl_sipc_isModemAssert() ;

    //return (s_cpdump_flag || (log2pc==0 && isAssert));
    return (log2pc==0 && isAssert);
}
#endif

uint32 s_frame_size = 0;
int DiagFlagCnt = 0;
void trans_diag(CHNL_DATA_TYPE type, uint8 *buff, int size){
    uint32 nRd = 0;
    uint8* src = buff;
    uint32 nlen = 0;
    uint32 offset = 0;
    
    nRd = size;
    nlen = nRd;
    ENG_LOG("%s: nRd = %d", __FUNCTION__, nRd);
    #if 0
        DiagFlagCnt += countDiagFlag(src, size);
        if (DiagFlagCnt > 0) {
            SIO_SetCmdReq(TRUE);
        }
        //SIO_SendCommData(buff, nlen);
        pc_write_data(type, buff, nlen);
        if ( DiagFlagCnt % 2 == 0) {
            SIO_SetCmdReq(FALSE);
        }
        ENG_LOG("%s: DiagFlagCnt = %d", __FUNCTION__, DiagFlagCnt);
        DiagFlagCnt %= 2;
    #else
    while(TRUE){
        switch (s_frame_status_cur)
        {
            case DIAG_STATUS_NONE:
            {
                int step = findDiagFlag(src, nlen);
                if(step < nlen){
                    //find flag
                    s_frame_status_last = s_frame_status_cur;
                    s_frame_status_cur = DIAG_STATUS_HEAD;
                    s_frame_size = 0;
                }

                if(isDumpToSD()){
                    chnl_sd_dump2File(CP_DUMP_MEMORY, src, step);
                }else{
#ifdef FT_WIFI_SUPPORT_CUSTOM_XTC
                    SIO_SendCommData(src, step);
#else
                    pc_write_data(type, src, step);
#endif
                }

                src += step;
                nlen -= step;
                offset += step;

                //ENG_LOG("%s: DIAG_STATUS_NONE: step = %d, nlen = %d, offset = %d", __FUNCTION__, step, nlen, offset);
            }
            break;
            case DIAG_STATUS_HEAD:
            {
                int step = 0;
                if (nlen == nRd && s_frame_status_last == DIAG_STATUS_HEAD){
                    //new frame find, so find with header
                    step = findDiagFlag(src, nlen);
                    s_frame_status_last = s_frame_status_cur;
                    if(step < nlen){
                        //find flag
                        step += 1;
                        s_frame_status_cur = DIAG_STATUS_TAIL;
                    }
                }else{
                    //cur frame find
                    step = findDiagFlag(src+1, nlen-1);
                    s_frame_status_last = s_frame_status_cur;
                    if(step < nlen-1){
                        //find flag
                        step += 2;                        
                        s_frame_status_cur = DIAG_STATUS_TAIL;
                    }else{
                        step += 1;
                    }
                }

                if(isDumpToSD()){
                    chnl_sd_dump2File(CP_DUMP_MEMORY, src, step);
                }else{
                    SIO_SetCmdReq(TRUE);
#ifdef FT_WIFI_SUPPORT_CUSTOM_XTC
                    SIO_SendCommData(src, step);
#else
                    pc_write_data(type, src, step);
#endif
                }

                src += step;
                nlen -= step;
                offset += step;
                s_frame_size += step;

                //ENG_LOG("%s: DIAG_STATUS_HEAD:  step = %d, nlen = %d, offset = %d", __FUNCTION__, step, nlen, offset);
            }
            break;
            case DIAG_STATUS_TAIL:
            {
                //ENG_LOG("%s: DIAG_STATUS_TAIL: step = 0, nlen = %d, offset = %d, s_frame_size = %d", __FUNCTION__, nlen, offset, s_frame_size);
                SIO_SetCmdReq(FALSE);
                s_frame_status_last = s_frame_status_cur;
                s_frame_status_cur = DIAG_STATUS_NONE;
            }
            break;
            default:
            break;
        }

        if(offset >= nRd && s_frame_status_cur != DIAG_STATUS_TAIL){
            ENG_LOG("%s: continue to read next frame", __FUNCTION__);
            break;
        }
    }
    #endif
        
}

extern BOOLEAN POWER_IsCalibrationResetMode(void);
LOCAL void Engpc_CP_Rsp2PC( uint32 argc, void* argv )
{
    int ret = 0;
    int chnl_type = argc;
    BOOLEAN isCaliMode = POWER_IsCalibrationResetMode();
    ENG_LOG("%s: engpc_cp_rsp2pc...", findNameByChnlType(chnl_type));

    while(TRUE)
    {
        CHNL_DATA_TYPE data_type = findChnlDataTypeByChnlType(chnl_type);
        int nRd = 0;
        uint8* buff_ptr = NULL;
        int size = 0;

        if (data_type == CP_DIAG || data_type == WCN_DATA){
            SCI_MEMSET(s_rsp_cp_diag_ptr, 0, sizeof(s_rsp_cp_diag_ptr));
            buff_ptr = s_rsp_cp_diag_ptr;
            size = sizeof(s_rsp_cp_diag_ptr);
        } else {
            SCI_MEMSET(s_rsp_cp_at_ptr, 0, sizeof(s_rsp_cp_at_ptr));
            buff_ptr = s_rsp_cp_at_ptr;
            size = sizeof(s_rsp_cp_at_ptr);
        }

        nRd = cp_read_data(data_type, buff_ptr, size);
        if (nRd > 0) {
            if (SCI_IsDebugBuild() && isDumpToSD()) {
                chnl_sd_dump2File(CP_DUMP_MEMORY, buff_ptr, nRd);
            } else {
                if (data_type == CP_DIAG && (chnl_sipc_isModemAssert()||isCaliMode)) {
                    trans_diag(data_type, buff_ptr, nRd);
                } else {
                    pc_write_data(data_type, buff_ptr, nRd);
#ifdef UAL_AT_SUPPORT
                    if(s_at_call_back != 0)
                    {
                        s_at_call_back(buff_ptr,nRd);
                    }
#endif
                }
            }
        } else {
            ENG_LOG("%s: cp read return %d", findNameByChnlType(chnl_type), nRd);
        }
    }
}


PUBLIC void chnl_cp_init()
{
    if( SCI_INVALID_BLOCK_ID == s_engpc_cp_diag_task )
    {         
        s_engpc_cp_diag_task = SCI_CreateThread(
            "ENGPC_CP_DIAG_READ_THREAD",
            "ENGPC_CP_DIAG_READ_THREAD",
            Engpc_CP_Rsp2PC,
            CHNL_DIAG_CP_PC,
            NULL,
            P_ENGPC_CP_RSP2PC_STACK_SIZE,
            P_ENGPC_CP_RSP2PC_QUEUE_NUM,
            P_ENGPC_CP_RSP2PC_TASK_PRIORITY,
            SCI_PREEMPT,
            SCI_AUTO_START );
        ENG_LOG("%s: threadid = 0x%x...", findNameByChnlType(CHNL_DIAG_CP_PC), s_engpc_cp_diag_task);
    }

    if( SCI_INVALID_BLOCK_ID == s_engpc_cp_at_task )
    {         
        s_engpc_cp_at_task = SCI_CreateThread(
            "ENGPC_CP_AT_READ_THREAD",
            "ENGPC_CP_AT_READ_THREAD",
            Engpc_CP_Rsp2PC,
            CHNL_AT_CP_PC,
            NULL,
            P_ENGPC_CP_RSP2PC_STACK_SIZE,
            P_ENGPC_CP_RSP2PC_QUEUE_NUM,
            P_ENGPC_CP_RSP2PC_TASK_PRIORITY,
            SCI_PREEMPT,
            SCI_AUTO_START );
        ENG_LOG("%s: threadid = 0x%x...", findNameByChnlType(CHNL_AT_CP_PC), s_engpc_cp_at_task);
    }    

}   

PUBLIC int cp_read_data(CHNL_DATA_TYPE type, char* buff, int len)
{
    return chnl_sipc_read_data(CP_CP, type, buff, len);
}

PUBLIC int cp_write_data(CHNL_DATA_TYPE type, char* buff, int len)
{
    return chnl_sipc_write_data(CP_CP, type, buff, len);
}
#ifdef UAL_AT_SUPPORT
PUBLIC int at_send_data_to_cp(CHNL_DATA_TYPE type, char* buff, int len, func_ptr callBack)
{
    ENG_LOG("at_send_data_to_cp enter ");
    s_at_call_back = callBack;
    return cp_write_data(type, buff, len);
}
PUBLIC void at_remove_callback(void)
{
    s_at_call_back = 0;
}
#endif

