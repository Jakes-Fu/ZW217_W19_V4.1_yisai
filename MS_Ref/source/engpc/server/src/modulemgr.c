#include "os_api.h"

#include "sprd_fts_log.h"
#include "sprd_fts_type.h"
#include "reg_type.h"
#include "sprd_fts_table_reg.h"
#include "sio.h"

char *strcasestr(const char* s1, const char* s2)
{
    const char *s = s1;
    const char *p = s2;

    do {
        if (!*p)
            return (char *)s1;
        if ((*p == *s) || (tolower(*p) == tolower(*s))){
            ++p;
            ++s;
        }else {
            p = s2;
            if (!*s)
                return NULL;
            s = ++s1;
        }
    }while (1);

    return *p?NULL:(char*)s1;
}

int write_to_host_diag(char* buff, int len){
    ENG_LOG("write_to_host_diag: len = %d", len);

    return pc_write_diag(buff+1, len-2);
}

int chnl_send_diag_interface(char* buff, int len){
    return 0;
}

int chnl_send_at_interface(char* buff, int len){
    return 0;
}
DYMIC_WRITETOPC_FUNC write_interface[WRITE_TO_MAX] = {
    write_to_host_diag,/*pc lte diag*/
    NULL,NULL,NULL,NULL,NULL,
    chnl_send_diag_interface,
    NULL,
    chnl_send_at_interface,/*send cmd so to so*/
    NULL,NULL,NULL,NULL,NULL,NULL
};

LOCAL BOOLEAN isValidateAT(uint8 *src)
{
    if(src == NULL) return FALSE;
    if(strlen(src) < 2) return FALSE;
    if((src[0] == 'A' || src[0] == 'a') && (src[1] == 'T' || src[1] == 't')){
        return TRUE;
    }else {
        return FALSE;
    }
}

struct eng_callback* g_lpCurCB = NULL;
struct eng_callback* g_lpPendingCB = NULL;
const char *g_pending_mark = "\r\nPENDING\r\n";

PUBLIC int processDiag(char *buf, int len, char *rsp, int rsp_len, int* cp_process)
{
    struct eng_callback *callback = NULL;
    MSG_HEAD_T msg_head_ptr = {0};
    int rlen = 0;
    unsigned int data_cmd;
    byte data;
    unsigned short apcmd;
    int bProcess= 0;
	int i = 0; 

    memcpy(&msg_head_ptr, buf+1, sizeof(MSG_HEAD_T));

    for(i = 0; i < sizeof(s_table_reg_diag_at)/sizeof(struct eng_callback); i++)
    {
        callback = &(s_table_reg_diag_at[i]);
        g_lpCurCB = callback;

        if ((buf[7] == 0x68) && (0 != strlen(callback->at_cmd)) && (strcasestr(buf+9, callback->at_cmd)) != NULL) { // at command
            ENG_LOG("%s: Dymic CMD=%s finded\n",__FUNCTION__,callback->at_cmd);
            if (NULL != callback->eng_linuxcmd_func) {
                rlen = callback->eng_linuxcmd_func(buf, rsp);

                //if lib*.so return not support, then continue
                if (rlen == ENG_DIAG_RET_UNSUPPORT){
                    ENG_LOG("%s eng_diag_func return %d, continue find next callback func", __FUNCTION__, ENG_DIAG_RET_UNSUPPORT);
                    continue;
                }
                //for case :need to ap & cp
                if (callback->also_need_to_cp) {
                    *cp_process = 1;
                }

                bProcess = 1;
                break;
            } else {
                ENG_LOG("%s: Dymic eng_linuxcmd_func == NULL\n",__FUNCTION__);
                break;
            }
        } else if ( (msg_head_ptr.type == callback->type && msg_head_ptr.subtype == callback->subtype) ||
                    (callback->eng_cmd_match != NULL && callback->eng_cmd_match(buf, len) == 0) )  {
            ENG_LOG("%s: type = %d, subtype = %d", __FUNCTION__, msg_head_ptr.type, msg_head_ptr.subtype);
            // diag command: type(unsigned char) + sub_type(unsigned char) + data_cmd(unsigned int)
            if (0x5D == msg_head_ptr.type){
                memcpy(&data_cmd, buf + 1 + sizeof(MSG_HEAD_T), sizeof(data_cmd));
                if (data_cmd != callback->diag_ap_cmd) {
                    ENG_LOG("%s data cmd is not matched!", __FUNCTION__);
                    continue;
                }
            }

            // diag command: type(unsigned char) + sub_type(unsigned char) + TOOLS_DIAG_AP_CMD_T
            if (0x62 == msg_head_ptr.type){//DIAG_CMD_APCALI
                memcpy(&apcmd, buf + 1 + sizeof(MSG_HEAD_T), sizeof(apcmd));
                if (apcmd != callback->diag_ap_cmd) {
                    ENG_LOG("%s apcmd->cmd is not matched!", __FUNCTION__);
                    continue;
                }
            }

            // diag command: type(unsigned char) + sub_type(unsigned char) + byte, for autotest test case which have same type+subtype
            if (0x38 == msg_head_ptr.type){
                memcpy(&data, buf + 1 + sizeof(MSG_HEAD_T), sizeof(data));
                if (callback->diag_ap_cmd != -1 && (int)(data) != callback->diag_ap_cmd)  {
                    ENG_LOG("%s data is not matched!", __FUNCTION__);
                    continue;
                }
            }

            if (NULL != callback->eng_diag_func) {
                if (NULL != callback->eng_set_writeinterface_func) {
                    callback->eng_set_writeinterface_func(write_interface);
                }
                rlen = callback->eng_diag_func(buf, len, rsp, rsp_len);
                if (rlen == ENG_DIAG_RET_UNSUPPORT){
                    ENG_LOG("%s eng_diag_func return %d, continue find next callback func", __FUNCTION__, ENG_DIAG_RET_UNSUPPORT);
                    continue;
                }

                //for case :need to ap & cp
                if (callback->also_need_to_cp) {
                    *cp_process = 1;
                }

                bProcess =1;
                break;
            } else {
                ENG_LOG("%s: Dymic eng_diag_func == NULL\n",__FUNCTION__);
                break;
            }
        } else {
            continue;
        }
    }

    //process pending at cmd
    if (bProcess == 0 && g_lpPendingCB != NULL && 0x68 == msg_head_ptr.type && strcasestr(buf + 9, "AT") == NULL){
        ENG_LOG("%s: Dymic next_data_callback", __FUNCTION__);
        rlen = g_lpPendingCB->eng_linuxcmd_func(buf, rsp);
    }

    g_lpPendingCB = NULL;
    if (bProcess==1 && 0x68 == msg_head_ptr.type){
        int at_ret_error = 0;
        int pending_len = strlen(g_pending_mark);
        at_ret_error = (strcasestr(rsp, "ERROR") != NULL)?1:0;
        if (!at_ret_error && strcasestr(rsp, g_pending_mark)  != NULL){
            g_lpPendingCB = callback;
            rlen -= pending_len;
        }
    }


    return rlen;
}

PUBLIC int processAT(char *buf, int len, char *rsp, int rsp_len, int* cp_process)
{
    int rlen = 0, ret = 0;
    struct eng_callback *callback = NULL;
	int i = 0;

    ENG_LOG("%s: buf:%s len:%d rsp_len:%d", __FUNCTION__, buf, len, rsp_len);

    for(i = 0; i < sizeof(s_table_reg_diag_at)/sizeof(struct eng_callback); i++)
    {
        callback = &(s_table_reg_diag_at[i]);

        ENG_LOG("callback->at_cmd = %s, buff = %s, len = %d", callback->at_cmd, buf, strlen(callback->at_cmd));
        if ((0 != strlen(callback->at_cmd)) &&
            (0 == strncasecmp((const char *) buf, (const char *)(callback->at_cmd),
            strlen(callback->at_cmd)))) { // at command
            ENG_LOG("%s: Dymic CMD=%s finded\n", __FUNCTION__, callback->at_cmd);
            if (NULL != callback->eng_linuxcmd_func) {
                rlen = callback->eng_linuxcmd_func(buf, rsp);

                //if lib*.so return not support, then continue
                if (rlen == ENG_DIAG_RET_UNSUPPORT){
                    ENG_LOG("%s: eng_at_dymic_hdlr return ENG_DIAG_RET_UNSUPPORT, continue", __FUNCTION__);
                    continue;
                }
                //for case :need to ap & cp
                if (callback->also_need_to_cp) {
                    *cp_process = 1;
                }

                break;
            } else {
              ENG_LOG("%s: Dymic eng_linuxcmd_func == NULL\n", __FUNCTION__);
              break;
            }
        }else {
          continue;
        }
    }

    return rlen;
}
