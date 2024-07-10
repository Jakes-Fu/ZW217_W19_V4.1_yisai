#ifndef __FTMODE_H__
#define __FTMODE_H__

#include "sprd_fts_type.h"
#include "sprd_fts_diag.h"

//#define TEST_CALIPARAM

typedef enum {
    IMEI_ERR_NONE = 0,
    IMEI_CRC_ERR,
    IMEI_CMD_ERR,
    IMEI_SAVE_ERR,
    IMEI_READ_ERR
} ERR_IMEI_E;

#define MSG_ACK 0
#define MSG_NACK 1


int ftmode_getTestMode(char *req, char *rsp);
int ftmode_setTestMode(char *req, char *rsp);
int modeRsp_match(char* buff, int len);
int ftmode_testmodeRsp(char *buf, int len, char *rsp, int rsplen);


int get_fw_ptr(DYMIC_WRITETOPC_FUNC * write_interface_ptr);

int eng_diag_product_ctrl(char *buf, int len, char *rsp, int rsplen);
int eng_diag_product_ctrl_ex(char *buf, int len, char *rsp, int rsplen);
int bt_wifi_mac_handler(char *buff, int len, char *rsp, int rsplen);
int bt_wifi_mac_match(char* buff, int len);
int phasecheck_handler(char *buff, int len, char *rsp, int rsplen);
int phasecheck_match(char* buff, int len);

int fixap_handle(char *buf, int len, char *rsp, int rsplen);
int ssid_save(char *buf, int len, char *rsp, int rsplen);

#endif