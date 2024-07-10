#include "os_api.h"
#include "sci_types.h"
#include "sfs.h"

#include "sprd_fts_type.h"
#include "sprd_fts_diag.h"
#include "sprd_fts_log.h"

#include "caliparam.h"
#include "phasecheck.h"


typedef struct
{
    uint32  cmd;        // 1: save ssid into DUT
    uint32 ssid_len;    // ssid length, max. characters is 32
    char ssid[32];      // ssid name
    uint32 psk_len;     // ssid password length
    char psk[64];       // password
}diag_cmd_ssid_info_req;

typedef struct
{
    uint32  cmd;        // same as cmd of diag_cmd_ssid_info_req, as for save SSID is 1
    uint32  status;     // 0: pass, otherwise fail, DUT can return the actual error code
}diag_cmd_ssid_info_ack;

typedef struct
{
    uint32  cmd;    // 1: save ssid into DUT   2: setup connect mode
    uint32  mode;   // 0: USB,  1: fix AP by WIFI
}diag_cmd_set_conn_mode_req;

typedef struct
{
    uint32  cmd;        // same as request cmd
    uint32  status;     // 0: pass, otherwise fail, DUT can return the actual error code
}diag_cmd_set_conn_mode_ack;

#define CONNECT_FIXAP_BASE      (0)
#define CONNECT_FIXAP_SIZE      (sizeof(char))
#define SSID_KEY_BASE           (CONNECT_FIXAP_BASE+CONNECT_FIXAP_SIZE)
#define SSID_KEY_SIZE           (128)

#define TESTMODE_BASE           (0)
#define TESTMODE_SIZE           (4)
#define UBOOT_TESTMOD_CHECKSUM  (0x53464d00)

#define CMDLINE_FIRST_MODE_FLAG "first_mode="
#define CMDLINE_CALI_MODE_FLAG  "calibration="

#define AT_CMD_GETTESTMODE      "AT+GETTESTMODE?"
#define AT_CMD_SETTESTMODE      "AT+SETTESTMODE="

#define MAX_CMDLIEN_LEN 128

#define bool  BOOLEAN
#define true  TRUE
#define false FALSE

typedef struct _MODEID_NAME{
    unsigned int id;
    char name[64];
}MODE_ID_NAME;

//id from pc tool with at like: at+settestmode=XX
MODE_ID_NAME id_first_mode[]= {
    {0x00, "NORMAL"},
    {0x01, "GSMCAL"},
    {0x02, "GSMFT"},
    {0x03, "WCDMACAL"},
    {0x04, "WCDMAFT"},
    {0x05, "TDSCAL"},
    {0x06, "TDSFT"},
    {0x07, "LTECAL"},
    {0x08, "LTEFT"},
    {0x0B, "NRCAL"},
    {0x0C, "NRFT"},
    {0x0D, "NRMMWCAL"},
    {0x0E, "NRMMWFT"},
    {0x0F, "C2KCAL"},
    {0x10, "C2KFT"},
    {0x11, "BBAT"},
    {0x12, "NATIVEMMI"},
    {0x13, "APKMMI"},
    {0x14, "NBIOTCAL"},
    {0x15, "NBIOTFT"},
    {0x16, "UPT"}
};

//id from pc tool send to UT with usb protocol
MODE_ID_NAME id_cali_mode[]= {
    {0, "NORMAL"},
    {1, "GSMCAL"},
    {5, "GSMFT"},
    {7, "TDSCAL"},
    {8, "TDSFT"},
    {11, "WCDMACAL"},
    {12, "WCDMAFT"},
    {16, "LTECAL"},
    {17, "LTEFT"},
    {18, "C2KCAL"},
    {19, "C2KFT"},
    {24, "NRCAL"},
    {25, "NRFT"},
};

SCI_MUTEX_PTR s_mutex_caliparam = NULL;

static char testmode_rsp[]={0x7E, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0xFE, 0xFF, 0x7E};

LOCAL void lock(){
    if (s_mutex_caliparam == NULL){
        s_mutex_caliparam = SCI_CreateMutex("engpc_caliparam_r", SCI_INHERIT);
    }

    SCI_GetMutex(s_mutex_caliparam, SCI_WAIT_FOREVER);
}

LOCAL void unlock(){
    if (s_mutex_caliparam == NULL){
        s_mutex_caliparam = SCI_CreateMutex("engpc_caliparam_r", SCI_INHERIT);
    }

    SCI_PutMutex(s_mutex_caliparam);        
}
char *strdup(const char* str)
{
    size_t len;
    char *newstr;

    if(!str)
        return (char*)NULL;

    len = strlen(str);
    if(len >= ((size_t)-1) / sizeof(char))
        return (char *)NULL;

    newstr = malloc((len+1)*sizeof(char));
    if (!newstr)
        return (char*)NULL;

    memcpy(newstr, str, (len+1)*sizeof(char));

    return newstr;
}

unsigned int findIdWithName(char* name){
    if (name == NULL) return 0xFF;
    ENG_LOG("name: %s", name);

    for(int i = 0; i < sizeof(id_first_mode)/sizeof(MODE_ID_NAME); i++){
        if (strcasecmp(id_first_mode[i].name, name) == 0){
            return id_first_mode[i].id;
        }
    }

    return 0xFF;
}

char* findNameWithId(int id, int modeFlag){
    ENG_LOG("id: 0x%x", id);
    int mode_num[] = {sizeof(id_first_mode)/sizeof(MODE_ID_NAME),
                      sizeof(id_cali_mode)/sizeof(MODE_ID_NAME)};
    MODE_ID_NAME* idName;
    if(modeFlag == 0){
      idName = id_first_mode;
    }else if(modeFlag == 1){
      idName = id_cali_mode;
    }

    for(int i = 0; i < mode_num[modeFlag]; i++){
        if (idName[i].id == id){
            return idName[i].name;
        }
    }

    return NULL;
}

int isspace(char c){
    if (c == '\r' || c == '\n' || c == ' '){
        return 1;
    }

    return 0;
}

char* trim(char *str){
    char *end,*sp,*ep;
    int len;
    sp = str;
    end = str + strlen(str) - 1;
    ep = end;

    while(sp<=end && isspace(*sp))
        sp++;
    while(ep>=sp && isspace(*ep))
        ep--;
    len = (ep < sp) ? 0:(ep-sp)+1;
    sp[len] = '\0';
    return sp;
}

int parse_key_value(char* buf, char* key, int flag_mode) {
    int len = 0;
    char value[8] = {0};
    char* str = NULL;
    char* gap = NULL;

    str = strstr(buf, key);
    if (str != NULL){
        str = strchr(str, '=');
        if (str != NULL){
            str++;
            if (str != NULL){
                if(flag_mode == 0){
                    gap=strchr(str, ' ');
                }else{
                    gap=strchr(str, ',');
                }
                if (gap != NULL){
                    len = gap - str;
                }else{
                    len = 2;
                }
                strncpy(value, str, len);
            }
        }
    }
   
    return strtol(value, NULL, flag_mode == 1 ? 10 : 16);
    
}

//const char* strTestMode = "first_mode=1, calibration=1";
extern char* RM_GetCaliCmdBuf(void);
extern char* RM_GetFirstModeBuf(void);

int ftmode_getTestMode(char *req, char *rsp){
    int fd = 0, ret = 0, len = 0;
    char cmdline[MAX_CMDLIEN_LEN] = {0};
    unsigned int value = 0x00;
    char* str = NULL;
    char* mode = 0;
    int mode_flag = 0;

    ENG_LOG("%s ", __FUNCTION__);

    //memcpy(cmdline, strTestMode, strlen(strTestMode));
    {
        //ENG_LOG("cmdline %s\n", cmdline);

        /*like this first_mode=11*/
        str = RM_GetFirstModeBuf();
        if(str != NULL){
            ENG_LOG("RM_GetFirstModeBuf return %s\n", str);
            if(strstr(str, CMDLINE_FIRST_MODE_FLAG)){
                value = parse_key_value(str, CMDLINE_FIRST_MODE_FLAG, mode_flag);
            }
        }else{
            str = RM_GetCaliCmdBuf();
            if(str != NULL){
                ENG_LOG("RM_GetCaliCmdBuf return %s\n", str);
                if(strstr(str, CMDLINE_CALI_MODE_FLAG)){
                    mode_flag=1;
                    value = parse_key_value(str, CMDLINE_CALI_MODE_FLAG, mode_flag);
                }
            }
        }
    }

    mode = findNameWithId(value, mode_flag);
    if (mode == NULL){
        sprintf(rsp, "+GETTESTMODE:0x%X", value);
    }else{
        sprintf(rsp, "+GETTESTMODE:%s", mode);
    }

    return strlen(rsp);
}

bool isdigitstr(char *str){
    return strspn(str, "0123456789AaBbCcDdEeFfXx")==strlen(str);
}

bool isHexstr(char* str){
    if (strlen(str) > 2 && strcasestr(str, "0x") != NULL){
        return true;
    }

    return false;
}

int ftmode_setTestMode(char *req, char *rsp){
    int ret = 0;
    char* ptr = NULL;
    char* ptrTmp = NULL;
    char* strNum = NULL;
    int len = 0;

    if(req[0] == 0x7e)
    {
        MSG_HEAD_T msg = {0};
        memcpy(&msg, req+1, sizeof(MSG_HEAD_T));
        int datalen = msg.len;
        ptr = req + 1 + sizeof(MSG_HEAD_T);
        *(req+datalen+1) = 0;
    }
    else
    {
        ptr = strdup(req);
    }

    ptrTmp = ptr;
    ENG_LOG("%s: ptr = %s", __FUNCTION__, ptr);
    ptrTmp = ptrTmp+strlen(AT_CMD_SETTESTMODE);
    if (ptrTmp != NULL && (strNum = trim(ptrTmp)) != NULL){
        unsigned int val = 0;
        if ( isdigitstr(strNum) ){
            if (isHexstr(strNum)){
                val = strtol(strNum, NULL, 16);
            }else{
                val = strtol(strNum, NULL, 10);
            }
        }else{
            val = findIdWithName(strNum);
        }

        ENG_LOG("%s: val = 0x%X", __FUNCTION__, val);
        val += UBOOT_TESTMOD_CHECKSUM;

        lock();

        if (CaliParam_Open()){
            if (!CaliParam_WriteOffset(PARAM_POWERON_MODE, TESTMODE_BASE, sizeof(val), &val)){
                ret = -3;
                ENG_LOG("write offset fail");
            }else{
                ret = 0;
                ENG_LOG("write offset ok.");
            }
            CaliParam_Close();
        }else{
            ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
            ret = -1;
        }

        unlock();
    }else{
        ret = -1;
    }

    if (ret == 0){
        sprintf(rsp, "+SETTESTMODE:OK");
    }else{
        sprintf(rsp, "+SETTESTMODE: ERROR %d", ret);
    }

    if (req[0] != 0x7e && ptr != NULL){
        free(ptr);
    }

    return strlen(rsp);
}

int ftmode_testmodeRsp(char *buf, int len, char *rsp, int rsplen){
    memcpy(rsp, testmode_rsp, sizeof(testmode_rsp));
    return sizeof(testmode_rsp);
}

int modeRsp_match(char* buff, int len){
    if (buff[7] == 0xFE && buff[8] >= 0x80 && buff[8] <= 0x9F){
        return 0;
    }else{
        return -1;
    }
}

int load(diag_cmd_ssid_info_req* req){
    bool ret = FALSE;
    int len = sizeof(diag_cmd_ssid_info_req)-sizeof(req->cmd);
    if (len > SSID_KEY_SIZE){
        len = SSID_KEY_SIZE;
    }

    lock();
    if (!CaliParam_Open()){
        unlock();
        ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
        return -1;
    }

    ret = CaliParam_ReadOffset(PARAM_WIFI_CONNECT, SSID_KEY_BASE, len, (char*)(&(req->ssid_len)));

    CaliParam_Close();
    len = (req->ssid_len >= sizeof(req->ssid))?(sizeof(req->ssid)-1):req->ssid_len;
    req->ssid_len = len;
    req->ssid[len] = 0;
    len = (req->psk_len >= sizeof(req->psk))?(sizeof(req->psk)-1):req->psk_len;
    req->psk_len = len;
    req->psk[len] = 0;

    unlock();

    return (ret==TRUE)?0:-1;
}

int save(diag_cmd_ssid_info_req* req){
    bool ret = FALSE;
    int len = sizeof(diag_cmd_ssid_info_req)-sizeof(req->cmd);
    if (len > SSID_KEY_SIZE){
        len = SSID_KEY_SIZE;
    }

    lock();

    if (!CaliParam_Open()){
        unlock();
        ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
        return -1;
    }

    ret = CaliParam_WriteOffset(PARAM_WIFI_CONNECT, SSID_KEY_BASE, len, (char*)&(req->ssid_len));

    CaliParam_Close();

    unlock();

    return (ret==TRUE)?0:-1;
}

int ssid_save(char *buf, int len, char *rsp, int rsplen) {
    diag_cmd_ssid_info_req lpReq = {0};
    diag_cmd_ssid_info_ack ack = {0};
    MSG_HEAD_T msg_head_ptr = {0};
    int lenReq = 0;
    int ret = 0;
    ENG_LOG("ssid_save...");
    if (buf == NULL || rsp == NULL){
        return 0;
    }

    lenReq = len-2-sizeof(MSG_HEAD_T);
    lenReq = lenReq>sizeof(diag_cmd_ssid_info_req)?sizeof(diag_cmd_ssid_info_req):lenReq;
    memcpy(&msg_head_ptr, buf+1, sizeof(MSG_HEAD_T));
    memcpy(&lpReq, buf+1+sizeof(MSG_HEAD_T), lenReq);
    ENG_LOG("%s: cmd = %d", __FUNCTION__, lpReq.cmd);
    if (lpReq.cmd == 1){
        ENG_LOG("%s: write ssid&&key", __FUNCTION__);
        ret = save(&lpReq);
        if (ret == 0) {
            ack.status = 0;    //action succ
        } else{
            ack.status = 1;    //action fail
        }

        ack.cmd = lpReq.cmd;

        msg_head_ptr.len = sizeof(MSG_HEAD_T)+sizeof(diag_cmd_ssid_info_ack);
        memcpy(rsp+1, &msg_head_ptr, sizeof(MSG_HEAD_T));
        memcpy(rsp+1+sizeof(MSG_HEAD_T), &ack, sizeof(diag_cmd_ssid_info_ack));

        rsp[0] = 0x7E;
        rsp[msg_head_ptr.len+1] = 0x7E;
    }else if (lpReq.cmd == 3){// add for test
        diag_cmd_ssid_info_req req = {0};
        ENG_LOG("%s: read ssid&&key", __FUNCTION__);
        if (0 == load(&req)){
            msg_head_ptr.subtype = 0x00;
        }else{
            msg_head_ptr.subtype = 0x01;
        }
        msg_head_ptr.len = sizeof(MSG_HEAD_T)+sizeof(diag_cmd_ssid_info_req);
        memcpy(rsp+1, &msg_head_ptr, sizeof(MSG_HEAD_T));
        memcpy(rsp+1+sizeof(MSG_HEAD_T), &req, sizeof(diag_cmd_ssid_info_req));
        
        rsp[0] = 0x7E;
        rsp[msg_head_ptr.len+1] = 0x7E;        
    }else{
        return ENG_DIAG_RET_UNSUPPORT;
    }

    return msg_head_ptr.len+2;
}

int isConnectAp(){
    char flag = 0;
    bool ret = FALSE;

    lock();
    if (!CaliParam_Open()){
        unlock();
        ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
        return 0;
    }
    ret = CaliParam_ReadOffset(PARAM_WIFI_CONNECT, CONNECT_FIXAP_BASE, sizeof(flag), &flag);
    ENG_LOG("ConnectAp flag: 0x%02x", flag);
    CaliParam_Close();
    unlock();

    if (flag == 1){
        return 1;
    }

    return 0;
}

int loadSsidKey(char* ssid, int ssid_len, char*key, int key_len){
    diag_cmd_ssid_info_req req = {0};
    if (0 == load(&req)){
        int len = (ssid_len-1 >= req.ssid_len)?req.ssid_len:(ssid_len-1);
        strncpy(ssid, req.ssid, len);
        len = (key_len-1>=req.psk_len)?req.psk_len:(key_len-1);
        strncpy(key, req.psk, len);
        return 0;
    }else{
        return -1;
    }
}

int fixap_handle(char *buf, int len, char *rsp, int rsplen){
    diag_cmd_set_conn_mode_req lpReq = {0};
    diag_cmd_set_conn_mode_ack ack = {0};
    MSG_HEAD_T msg_head_ptr = {0};
    int ret = 0;
    ENG_LOG("fixap_handle...");
    if (buf == NULL || rsp == NULL){
        return 0;
    }

    memcpy(&msg_head_ptr, buf+1, sizeof(MSG_HEAD_T));
    memcpy(&lpReq, buf+1+sizeof(MSG_HEAD_T), sizeof(diag_cmd_set_conn_mode_req));

    ENG_LOG("%s: cmd = %d", __FUNCTION__, lpReq.cmd);
    if (lpReq.cmd == 2){
        ENG_LOG("%s: write fix ap", __FUNCTION__);
        char cmd = (char)(lpReq.mode);

        lock();
        if (CaliParam_Open()){
            if (CaliParam_WriteOffset(PARAM_WIFI_CONNECT, CONNECT_FIXAP_BASE, sizeof(cmd), &cmd)){
                ret = 0;
            }else{
                ret = -1;
            }

            CaliParam_Close();
        }else{
            ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
            ret = -1;
        }
        unlock();

        ENG_LOG("%s: ret = %d", __FUNCTION__, ret);
        if (ret == 0) {
            ack.status = 0;    //action succ
        } else{
            ack.status = 1;    //action fail
        }
        ack.cmd = lpReq.cmd;

        msg_head_ptr.len = sizeof(MSG_HEAD_T)+sizeof(diag_cmd_set_conn_mode_ack);
        memcpy(rsp+1, &msg_head_ptr, sizeof(MSG_HEAD_T));
        memcpy(rsp+1+sizeof(MSG_HEAD_T), &ack, sizeof(diag_cmd_set_conn_mode_ack));

        rsp[0] = 0x7E;
        rsp[msg_head_ptr.len+1] = 0x7E;
    }else if (lpReq.cmd == 4){// add for test
        diag_cmd_set_conn_mode_req req = {0};
        char cmd = 0;
        ENG_LOG("%s: read fix ap", __FUNCTION__);
        lock();
        if (CaliParam_Open()){
            if (CaliParam_ReadOffset(PARAM_WIFI_CONNECT, CONNECT_FIXAP_BASE, sizeof(cmd), &cmd)){
                msg_head_ptr.subtype = 0x00;
                req.mode = cmd;
            }else{
                msg_head_ptr.subtype = 0x01;
                req.mode = 0xFF;
            }
            CaliParam_Close();
        }else{
            ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
            msg_head_ptr.subtype = 0x01;
            req.mode = 0xFF;
        }
        unlock();
        memcpy(rsp+1, &msg_head_ptr, sizeof(MSG_HEAD_T));
        memcpy(rsp+1+sizeof(MSG_HEAD_T), &req, sizeof(diag_cmd_set_conn_mode_req));

        rsp[0] = 0x7E;
        rsp[msg_head_ptr.len+1] = 0x7E;
    }else{
        return ENG_DIAG_RET_UNSUPPORT;
    }

    return msg_head_ptr.len+2;
}

#if 0
void register_this_module_ext(struct eng_callback *reg, int *num)
{
    int i = 0;
    int moudles_num = 0;
    ENG_LOG("register_this_module_ext :libftmode");

    sprintf(reg->at_cmd, "%s", AT_CMD_GETTESTMODE);
    reg->eng_linuxcmd_func = ftmode_getTestMode;
    moudles_num++;

    sprintf((reg+moudles_num)->at_cmd, "%s", AT_CMD_SETTESTMODE);
    (reg+moudles_num)->eng_linuxcmd_func = ftmode_setTestMode;
    moudles_num++;

    (reg+moudles_num)->eng_diag_func = ftmode_testmodeRsp;
    (reg+moudles_num)->eng_cmd_match = modeRsp_match;
     moudles_num++;

    //save ssid
    (reg+moudles_num)->type = 0x5D;
    (reg+moudles_num)->subtype = 0x05;
    (reg+moudles_num)->diag_ap_cmd = 0x01;
    (reg+moudles_num)->eng_diag_func = ssid_save;
    moudles_num++;

    //fix ap
    (reg+moudles_num)->type = 0x5D;
    (reg+moudles_num)->subtype = 0x05;
    (reg+moudles_num)->diag_ap_cmd = 0x02;
    (reg+moudles_num)->eng_diag_func = fixap_handle;
    moudles_num++;

    *num = moudles_num;
    ENG_LOG("register_this_module_ext: %d - %d",*num, moudles_num);
}
#endif
