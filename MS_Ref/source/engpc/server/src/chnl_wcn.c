#include "sci_types.h"
#include "os_api.h"

#include "sprd_fts_type.h"
#include "sprd_fts_log.h"

#include "chnl.h"
#include "chnl_sipc.h"
#include "chnl_pc.h"

//engpc thread: wcn response to pc tool
#define P_ENGPC_WCN_RSP2PC_STACK_SIZE     (1024*4)
#define P_ENGPC_WCN_RSP2PC_QUEUE_NUM      15    
#define P_ENGPC_WCN_RSP2PC_TASK_PRIORITY  78 //31


static BLOCK_ID s_engpc_wcn_diag_task = SCI_INVALID_BLOCK_ID;
static BLOCK_ID s_engpc_wcn_at_task = SCI_INVALID_BLOCK_ID;


LOCAL void Engpc_WCN_Rsp2PC( uint32 argc, void* argv )
{
    int ret = 0;
    char *buff = (uint8 *)SCI_ALLOC_APP(DIAG_BUFF_SIZE);
    int chnl_type = argc;

    SCI_MEMSET(buff, 0, DIAG_BUFF_SIZE);
    ENG_LOG("%s: engpc_wcn_rsp2pc...", findNameByChnlType(chnl_type));

    while(TRUE)
    {
        int nRd = wcn_read_data(findChnlDataTypeByChnlType(chnl_type), buff, DIAG_BUFF_SIZE);
        if (nRd > 0)
        {
            pc_write_data(findChnlDataTypeByChnlType(chnl_type), buff, nRd);
        }else{
            ENG_LOG("%s: cp read return %d", findNameByChnlType(chnl_type), nRd);
        }
    }

    SCI_FREE(buff);
    buff = NULL;
}


PUBLIC void chnl_wcn_init()
{
/*
    if( SCI_INVALID_BLOCK_ID == s_engpc_cp_diag_task )
    {         
        s_engpc_wcn_diag_task = SCI_CreateThread(
            "ENGPC_WCN_DIAG_READ_THREAD",
            "ENGPC_WCN_DIAG_READ_THREAD",
            Engpc_WCN_Rsp2PC,
            WCN_DIAG,
            NULL,
            P_ENGPC_WCN_RSP2PC_STACK_SIZE,
            P_ENGPC_WCN_RSP2PC_QUEUE_NUM,
            P_ENGPC_WCN_RSP2PC_TASK_PRIORITY,
            SCI_PREEMPT,
            SCI_AUTO_START );
    }
*/
    if( SCI_INVALID_BLOCK_ID == s_engpc_wcn_at_task )
    {         
        s_engpc_wcn_at_task = SCI_CreateThread(
            "ENGPC_WCN_AT_READ_THREAD",
            "ENGPC_WCN_AT_READ_THREAD",
            Engpc_WCN_Rsp2PC,
            CHNL_AT_WCN_PC,
            NULL,
            P_ENGPC_WCN_RSP2PC_STACK_SIZE,
            P_ENGPC_WCN_RSP2PC_QUEUE_NUM,
            P_ENGPC_WCN_RSP2PC_TASK_PRIORITY,
            SCI_PREEMPT,
            SCI_AUTO_START );

        ENG_LOG("%s: threadid = 0x%x...", findNameByChnlType(CHNL_AT_WCN_PC), s_engpc_wcn_at_task);
    }

}

PUBLIC int wcn_read_data(CHNL_DATA_TYPE type, char* buff, int len)
{
    return chnl_sipc_read_data(CP_WCN, type, buff, len);
}

PUBLIC int wcn_write_data(CHNL_DATA_TYPE type, char* buff, int len)
{
    return chnl_sipc_write_data(CP_WCN, type, buff, len);
}
