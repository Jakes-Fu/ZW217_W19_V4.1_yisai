#include "sci_types.h"
#include "os_api.h"
#include "socket_types.h"

#include "sprd_fts_type.h"
#include "sprd_fts_log.h"

#include "chnl.h"
#include "chnl_sipc.h"
#include "chnl_pc.h"
#include "chnl_wifi.h"

#define P_ENGPC_PC_SOCK_DIAG_STACK_SIZE     (1024*4)
#define P_ENGPC_PC_SOCK_DIAG_QUEUE_NUM      15    
#define P_ENGPC_PC_SOCK_DIAG_TASK_PRIORITY  78 //31


static BLOCK_ID s_engpc_pc_sock_diag_task = SCI_INVALID_BLOCK_ID;

#define PC_SOCK_DIAG_RSP_BUFF_SIZE (64*1024)

LOCAL uint8 s_pc_sock_diag_ptr[PC_SOCK_DIAG_RSP_BUFF_SIZE] = {0};
PUBLIC int cp_write_data(CHNL_DATA_TYPE type, char* buff, int len);
int chnl_wifi_isConnect();
extern void DisPoseMsgOut(uint8 **dest, uint16 *dest_len, const uint8 *src, uint16 src_len);

LOCAL int diag_decode7d7e(unsigned char *buf, int len) {
    int i, j, m = 0;
    unsigned char tmp;
    for (i = 0; i < len; i++) {
        if ((buf[i] == 0x7d) || (buf[i] == 0x7e)) {
        tmp = buf[i + 1] ^ 0x20;
        ENG_LOG("%s: tmp=%x, buf[%d]=%x", __FUNCTION__, tmp, i + 1, buf[i + 1]);
        buf[i] = tmp;
        for(j = i + 1; j < len; j ++){
            buf[j] = buf[j+1];
        }
        len--;
        m++;
        ENG_LOG("%s AFTER:", __FUNCTION__);
        }
    }

    ENG_LOG("%s: m=%d", __FUNCTION__, m);
    return m;
}

LOCAL int process(CHNL_DATA_TYPE data_type, uint8 *buff, int size)
{
    MSG_HEAD_T msg_head = {0};
    xSignalHeader	sig_ptr	= SCI_NULL;
    int len = 0;
    uint32  status = SCI_SUCCESS;

    memcpy(&msg_head, buff+1, sizeof(MSG_HEAD_T));
    len = msg_head.len;

    sig_ptr		= (xSignalHeader)SCI_ALLOC_APP(len + sizeof(xSignalHeaderRec)+4);
    if(sig_ptr)
    {
        sig_ptr->SignalSize = len + sizeof(xSignalHeaderRec);
        sig_ptr->Sender = SCI_IdentifyThread();

        SCI_MEMCPY((uint8 *)(sig_ptr + 1), buff+1, len);

        status = SCI_SendSignal(sig_ptr, DIAG);            
        if (status != SCI_SUCCESS)
        {
            ENG_LOG("!!!!!fatal error: send signal fail");
            return SCI_QUEUE_ERROR;
        }
    }

    return 0;
}

LOCAL void Engpc_PC_Sock_Diag( uint32 argc, void* argv )
{
    int ret = 0;
    int chnl_type = argc;
    ENG_LOG("%s: Engpc_PC_Sock_Diag...", findNameByChnlType(chnl_type));

    if(pc_sock_diag_open() != 0){
        ENG_LOG("open sock diag fail!!!");
        return ;
    }

    while(TRUE)
    {
        CHNL_DATA_TYPE data_type = findChnlDataTypeByChnlType(chnl_type);
        int nRd = 0;
        int status;

        SCI_MEMSET(s_pc_sock_diag_ptr, 0, sizeof(s_pc_sock_diag_ptr));

        nRd = pc_sock_read_data(data_type, s_pc_sock_diag_ptr, sizeof(s_pc_sock_diag_ptr));
        status = chnl_wifi_isConnect();
        if (nRd > 0)
        {
            int nRet = 0;
            activeCurConnectType(PC_CONNECT_WIFI);
            if(s_pc_sock_diag_ptr[0] != 0x7E){
                cp_write_data(data_type, s_pc_sock_diag_ptr, nRd);
            }else{
                nRet = diag_decode7d7e(s_pc_sock_diag_ptr+1, nRd-2);
                ENG_LOG("decode: nRet = %d", nRet);
                nRd -= nRet;
                if(0 != process(data_type, s_pc_sock_diag_ptr, nRd))
                {
                   cp_write_data(data_type, s_pc_sock_diag_ptr, nRd);
                }
                status = chnl_wifi_isConnect();
                if(status == WIFI_STATUS_DISCONNECTING || status == WIFI_STATUS_DISCONNECTED){
                    ENG_LOG("process after---wifi is disconnect!!!!break;");
                    break;
                }
            }
        }else if(status == WIFI_STATUS_DISCONNECTING || status == WIFI_STATUS_DISCONNECTED){
            ENG_LOG("read after---wifi is disconnect!!!!break;");
            break;
        }else{
            ENG_LOG("%s: cp read return %d, then reopen", findNameByChnlType(chnl_type), nRd);
            do{
                pc_sock_diag_close();
                if(pc_sock_diag_open() != 0){
                    ENG_LOG("reopen sock diag fail!!!");
                    break;
                }
            }while(0);
        }
    }

    pc_sock_diag_close();
}


PUBLIC void chnl_pc_sock_diag_init()
{
/*
    if( SCI_INVALID_BLOCK_ID == s_engpc_pc_sock_diag_task )
    {         
        s_engpc_pc_sock_diag_task = SCI_CreateThread(
            "ENGPC_PC_SOCK_DIAG_THREAD",
            "ENGPC_PC_SOCK_DIAG_THREAD",
            Engpc_PC_Sock_Diag,
            CHNL_DIAG_PC_CP,
            NULL,
            P_ENGPC_PC_SOCK_DIAG_STACK_SIZE,
            P_ENGPC_PC_SOCK_DIAG_QUEUE_NUM,
            P_ENGPC_PC_SOCK_DIAG_TASK_PRIORITY,
            SCI_PREEMPT,
            SCI_AUTO_START );
        ENG_LOG("%s: threadid = 0x%x...", findNameByChnlType(CHNL_DIAG_PC_CP), s_engpc_pc_sock_diag_task);
    }
*/

    Engpc_PC_Sock_Diag(CHNL_DIAG_PC_CP, NULL);
}
