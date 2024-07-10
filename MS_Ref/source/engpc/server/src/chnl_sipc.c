
#include "sci_types.h"
#include "os_api.h"

#include "chnl.h"
#include "chnl_sipc.h"
#include "sprd_fts_log.h"

#ifdef SPRD_ENGPC_SIPC
#include "sipc_v2.h"

#define ENGPC_SIPC_ID_PSCP  SIPC_ID_PSCP
#define ENGPC_SMSG_CH_DIAG  SMSG_CH_DIAG
#define ENGPC_SMSG_CH_TTY   SMSG_CH_TTY
#define ENGPC_DIAG_BUF_ID   0
#define ENGPC_AT_BUF_ID     31


#define ENGPC_SIPC_ID_WCN       SIPC_ID_WCN
#define ENGPC_SMSG_CH_WCN_DIAG  SMSG_CH_PIPE
#define ENGPC_DIAG_WCN_BUF_ID   0
#endif

#ifdef SPRD_ENGPC_MODEMCONTROL
#include "modem_ctrl_api.h"
#endif

#define SIPC_RW_TIMEOUT  -1

#define MODEM_ALIVE_TIMEOUT     (60*5)

#ifdef SPRD_ENGPC_TEST
#define spipe_read      spipe_read_test
#define spipe_write     spipe_write_test
#define spipe_open      spipe_open_test
#define spipe_close     spipe_close_test
#endif

LOCAL void* s_hdl_chnl_cp_diag = NULL;
LOCAL void* s_hdl_chnl_cp_at = NULL;
LOCAL void* s_hdl_chnl_wcn_diag = NULL;
LOCAL void* s_hdl_chnl_wcn_at = NULL;

LOCAL BOOLEAN s_b_modem_assert = FALSE;

extern void CPDumpCmd();

#define MAX_ASSERT_BUF_SIZE (1024)
char s_assert_buf[MAX_ASSERT_BUF_SIZE] = {0};

char* engpc_findAssertInfo(){
    ENG_LOG("assertInfo: %s", s_assert_buf);
    return (char*)s_assert_buf;
}

uint32 engpc_callback(const uint8 *buf, uint32 size)
{
    ENG_LOG("engpc_callback: %s, size = %d", buf, size);

    if(strstr(buf, "Modem Alive")){
        ENG_LOG("Modem Alive");
        s_b_modem_assert = FALSE;
    }else if(strstr(buf, "Modem Assert")){
        ENG_LOG("Modem Assert");
        memset(s_assert_buf, 0, sizeof(s_assert_buf));
        memcpy(s_assert_buf, buf, size>=(MAX_ASSERT_BUF_SIZE-1)?(MAX_ASSERT_BUF_SIZE-1):size);
        s_b_modem_assert = TRUE;
        //SCI_PASSERT(0, ("%s",engpc_findAssertInfo() ));
        if (SCI_IsDebugBuild())
            CPDumpCmd();
    }else{
        //ENG_LOG("%s", buf);
    }

    return 0;
}

PUBLIC BOOLEAN chnl_sipc_isModemAssert()
{
    return s_b_modem_assert;
}

LOCAL int waitForModemAlive()
{
    void* hdl = NULL;
    int timeout = MODEM_ALIVE_TIMEOUT;

#ifdef SPRD_ENGPC_MODEMCONTROL
    while(timeout-- > 0){
        hdl = spipe_open(ENGPC_SIPC_ID_PSCP, ENGPC_SMSG_CH_DIAG, ENGPC_DIAG_BUF_ID);
        if (NULL != hdl){
            spipe_close(hdl);
            ENG_LOG("modem alive ok. continue.");
            return 0;
        }
        SCI_Sleep(1000);
    }
#endif

#ifdef SPRD_ENGPC_TEST
    ENG_LOG("modem alive ok, for test");
    return 0;
#else
    ENG_LOG("wait for modem alive fail!!!!!! please check modem.");
    return -1;
#endif
}

#define HDL_CP_DIAG     0x88888801
#define HDL_CP_AT       0x88888802
#define HDL_WCN_DIAG    0x88888803
#define HDL_WCN_AT      0x88888804


uint8 s_diag_rsp_test[] = {0x7E, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x01, 0x7E};
char s_at_rsp_test[] = "\r\nOK\r\n";
int s_pc_diag_flag = 0;
int s_pc_at_flag = 0;

void * spipe_open_test(uint8 dst, uint8 channel, uint8 bufid){
#ifdef SPRD_ENGPC_SIPC
    if (dst == ENGPC_SIPC_ID_PSCP){
        if(channel == ENGPC_SMSG_CH_DIAG){
            return HDL_CP_DIAG;
        }else{
            return HDL_CP_AT;
        }
    }else if (dst == ENGPC_SIPC_ID_WCN){
        return HDL_WCN_DIAG;
    }
#endif
    return 0x88888888;
}

int spipe_close_test(void *spipe_ptr){
    return 0;
}


int spipe_read_test(void *spipe_ptr, char*buf, size_t count, int timeout){
    int *flag = &s_pc_diag_flag;
    if(spipe_ptr == HDL_CP_DIAG){
        flag = &s_pc_diag_flag;
    }else if(spipe_ptr == HDL_CP_AT){
        flag = &s_pc_at_flag;
    }else{
        flag = &s_pc_diag_flag;
    }

    SCI_Sleep(1000);
    ENG_LOG("read test return: 0x%x", spipe_ptr);

    while(*flag == 0){
        SCI_Sleep(1000);
    }

    SCI_Sleep(1000);
    ENG_LOG("read test return: 0x%x, count = %d", spipe_ptr, count);

    *flag = 0;
    memset(buf, 0, count);
    if(spipe_ptr == HDL_CP_DIAG){
        memcpy(buf, s_diag_rsp_test, sizeof(s_diag_rsp_test));
        return sizeof(s_diag_rsp_test);
    }else if(spipe_ptr == HDL_CP_AT){
        memcpy(buf, s_at_rsp_test, sizeof(s_at_rsp_test));
        return sizeof(s_at_rsp_test);
    }else{
        memcpy(buf, s_diag_rsp_test, sizeof(s_diag_rsp_test));
        return sizeof(s_diag_rsp_test);
    }
}

int spipe_write_test(void *spipe_ptr, char*buf, size_t count, int timeout){
    int *flag = &s_pc_diag_flag;
    if(spipe_ptr == HDL_CP_DIAG){
        flag = &s_pc_diag_flag;
    }else if(spipe_ptr == HDL_CP_AT){
        flag = &s_pc_at_flag;
    }else{
        flag = &s_pc_diag_flag;
    }

    *flag = 1;
    return count;
}

PUBLIC int chnl_sipc_read_data(CP_TYPE cp, CHNL_DATA_TYPE type, char* buff, int len)
{
    void* hdl = NULL;
    int ret = 0;
    int chnl_type = CHNL_DIAG_CP_PC;

    if (cp == CP_CP){
        if(type == CP_DIAG){
            hdl = s_hdl_chnl_cp_diag;
            chnl_type = CHNL_DIAG_CP_PC;
        }else if(type == CP_AT){
            chnl_type = CHNL_AT_CP_PC;
            hdl = s_hdl_chnl_cp_at;
        }else{
            ENG_LOG("error: sipc read: cp = %d, type = %d", cp, type);
            return -1;
        }
    }else if (cp == CP_WCN){
        if(type == WCN_DATA){
            chnl_type = CHNL_DIAG_WCN_PC;
            hdl = s_hdl_chnl_wcn_diag;
        }else if(type == WCN_AT){
            chnl_type = CHNL_AT_WCN_PC;
            hdl = s_hdl_chnl_wcn_at;
        }else{
            ENG_LOG("error: sipc read: cp = %d, type = %d", cp, type);
            return -1;
        }
    }else{
        ENG_LOG("error: sipc read: cp = %d, type = %d", cp, type);
        return -1;
    }

    if(hdl == NULL){
        ENG_LOG("%s: hdl is NULL!", findNameByChnlType(chnl_type));
        return -1;
    }

    ENG_LOG("%s: sipc read", findNameByChnlType(chnl_type));

#ifdef SPRD_ENGPC_SIPC
    ret =  spipe_read(hdl, buff, len, SIPC_RW_TIMEOUT);
    ENG_LOG("%s: sipc read return %d", findNameByChnlType(chnl_type), ret);
    if (ret > 0){
        printData(findNameByChnlType(chnl_type), type, buff, ret, PRINT_NUM_MAX, PRINT_BRK_FLAG);
    }
    return ret;
#else
    return -1;
#endif


    return -1;
}

#define SIPC_WRITE_RETRY    60

PUBLIC int chnl_sipc_write_data(CP_TYPE cp, CHNL_DATA_TYPE type, char* buff, int len)
{
    void* hdl = NULL;
    int ret = 0;
    int offset = 0;
    int retry = SIPC_WRITE_RETRY*5;
    int chnl_type = CHNL_DIAG_PC_CP;

    if (cp == CP_CP){
        if(type == CP_DIAG){
            hdl = s_hdl_chnl_cp_diag;
            chnl_type = CHNL_DIAG_PC_CP;
        }else if(type == CP_AT){
             hdl = s_hdl_chnl_cp_at;
             chnl_type = CHNL_AT_PC_CP;
        }else{
            ENG_LOG("error: sipc write: cp = %d, type = %d", cp, type);
            return -1;
        }
    }else if (cp == CP_WCN){
        if(type == WCN_DATA){
            hdl = s_hdl_chnl_wcn_diag;
            chnl_type = CHNL_DIAG_PC_WCN;
        }else if(type == WCN_AT){
            hdl = s_hdl_chnl_wcn_at;
            chnl_type = CHNL_AT_PC_WCN;
        }else{
            ENG_LOG("error: sipc write: cp = %d, type = %d", cp, type);
            return -1;
        }
    }

    if(hdl == NULL){
        ENG_LOG("%s: hdl is NULL!", findNameByChnlType(chnl_type));
        return -1;
    }

    ENG_LOG("%s: sipc write count = %d", findNameByChnlType(chnl_type), len);

#ifdef SPRD_ENGPC_SIPC
    printData(findNameByChnlType(chnl_type), type, buff, len, PRINT_NUM_MAX, PRINT_BRK_FLAG);

    do{
        ret = spipe_write(hdl, buff+offset, len, SIPC_RW_TIMEOUT);
        if (ret < 0){
            if(retry-- <=0){
                ENG_LOG("error: spipe write fail! break");
                break;
            }else{
                SCI_Sleep(500);
                ENG_LOG("error: spipe write fail! retry, error = %d", ret);
            }
        }else{
            len -= ret;
            offset += ret;
        }

    }while(len > 0);
    ENG_LOG("%s: sipc write return %d", findNameByChnlType(chnl_type), offset);
    return offset;
#else
    return -1;
#endif
}

PUBLIC int chnl_sipc_init()
{
    void* hdl = NULL;

    ENG_LOG("register callback");
    modem_ctl_client_reg_handler(MC_CLIENT_ENGPC, engpc_callback);

    if (0 != waitForModemAlive())
    {
        return -1;
    }

#ifdef SPRD_ENGPC_SIPC
    s_hdl_chnl_cp_diag = spipe_open(ENGPC_SIPC_ID_PSCP, ENGPC_SMSG_CH_DIAG, ENGPC_DIAG_BUF_ID);
    //SCI_ASSERT(s_hdl_chnl_cp_diag);
    if (s_hdl_chnl_cp_diag == NULL){
        ENG_LOG("open sdiag_lte fail...");
    }

    s_hdl_chnl_cp_at = spipe_open(ENGPC_SIPC_ID_PSCP, ENGPC_SMSG_CH_TTY, ENGPC_AT_BUF_ID);
    //SCI_ASSERT(s_hdl_chnl_cp_at);
    if (s_hdl_chnl_cp_at == NULL){
        ENG_LOG("open stty_lte fail...");
    }
/*
    s_hdl_chnl_wcn_diag = spipe_open(ENGPC_SIPC_ID_WCN, ENGPC_SMSG_CH_WCN_DIAG, ENGPC_DIAG_WCN_BUF_ID);
    //SCI_ASSERT(s_hdl_chnl_wcn_diag);
    if(s_hdl_chnl_wcn_diag == NULL){
        ENG_LOG("open slog_wcn0 fail...");
    }
*/
#endif
    return 0;
}

PUBLIC void chnl_sipc_uninit()
{
    ENG_LOG("chnl sipc uninit");
#ifdef SPRD_ENGPC_SIPC
    if(s_hdl_chnl_cp_diag != NULL)
    {
        spipe_close(s_hdl_chnl_cp_diag);
        s_hdl_chnl_cp_diag = NULL;
    }

    if(s_hdl_chnl_cp_at != NULL)
    {
        spipe_close(s_hdl_chnl_cp_at);
        s_hdl_chnl_cp_at = NULL;
    }

    if(s_hdl_chnl_wcn_diag != NULL)
    {
        spipe_close(s_hdl_chnl_wcn_diag);
        s_hdl_chnl_wcn_diag = NULL;
    }

    if(s_hdl_chnl_wcn_at != NULL)
    {
        spipe_close(s_hdl_chnl_wcn_at);
        s_hdl_chnl_wcn_at = NULL;
    }

#endif
}
