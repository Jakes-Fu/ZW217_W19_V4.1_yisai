#include "os_api.h"
#include "sci_types.h"

#include "crc16.h"
#include "sprd_fts_diag.h"
#include "sprd_fts_log.h"

#include "ftmode.h"
#include "caliparam.h"
#include "phasecheck.h"
#include "translate_packet.h"


typedef uint32 uint32_t ;

#ifdef TEST_CALIPARAM
uint8 s_test_phasecheck_sp09[] = {
0x39,0x30,0x50,0x53,0x31,0x32,0x33,0x34,
0x35,0x36,0x37,0x38,0x39,0x30,0x31,0x32,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x31,0x32,0x32,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,
0x44,0x4F,0x57,0x4E,0x4C,0x4F,0x41,0x44,0x00,0x00,0x57,0x52,0x49,0x54,0x45,0x53,
0x4E,0x00,0x00,0x00,0x43,0x46,0x54,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x41,0x4E,
0x54,0x45,0x4E,0x4E,0x41,0x00,0x00,0x00,0x49,0x4D,0x45,0x49,0x00,0x00,0x00,0x00,
0x00,0x00,0x57,0x50,0x54,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF
};
#endif

int direct_phschk(char *buf, int len, char *rsp, int rsplen) {
    int crc = 0;
    int data_len = 0;
    unsigned short recv_crc = 0;
    int rsp_len = 0;
    unsigned char result;
    unsigned char *nvdata;
    ERR_IMEI_E error;
    uint32_t magic;
    int sn_len = 0;
    int ret = 0;

    MSG_HEAD_T msg_head = {0};
    memcpy(&msg_head, buf+1, sizeof(MSG_HEAD_T));

    if (!CaliParam_Open()){
      ENG_LOG("%s: open caliparam fail..", __FUNCTION__);
      return 0;
    }

    do {
		memcpy(&recv_crc, buf+1+msg_head.len-sizeof(unsigned short), sizeof(recv_crc));
        crc = crc16(0, (unsigned char *)(buf+1+sizeof(MSG_HEAD_T)),
                    msg_head.len - sizeof(MSG_HEAD_T) - sizeof(unsigned short));

        if (recv_crc != crc) {
            ENG_LOG("%s: CRC Error! recv_crc: %d, crc16: %d\n", __FUNCTION__,
                  recv_crc, crc);
            msg_head.len = sizeof(MSG_HEAD_T) + 2 * sizeof(unsigned short);
            //*(unsigned short *)(buf+1+sizeof(MSG_HEAD_T)) = IMEI_CRC_ERR;
            memcpy(buf+1+sizeof(MSG_HEAD_T), IMEI_CRC_ERR, sizeof(IMEI_CRC_ERR));
            break;
        }

        ENG_LOG("%s: Current oper: %d\n", __FUNCTION__,
            (msg_head.subtype & RW_MASK));

        if ((msg_head.subtype & RW_MASK) == WRITE_MODE) {
            ENG_LOG("%s: Write Mode ! \n", __FUNCTION__);
            if (0 != (msg_head.subtype & RM_VALID_CMD_MASK)) {
                data_len = msg_head.len - sizeof(MSG_HEAD_T) - sizeof(unsigned short);
                
                ENG_LOG("%s: data_len: %d\n", __FUNCTION__, data_len);

#ifdef TEST_CALIPARAM
                ret = TRUE;
#else
				ret = CaliParam_WriteOffset(PARAM_PHASHCHECK, 0, data_len, buf+1+sizeof(MSG_HEAD_T));
#endif
                if (!ret) {
                    ENG_LOG("%s:Write ERROR: %d\n", __FUNCTION__, ret);
                    error = IMEI_SAVE_ERR;
                    result = 0;
                } else {
                    error = IMEI_ERR_NONE;
                    result = 1;
                }
            } else {
                ENG_LOG("%s: Write error, subtype : %d\n", __FUNCTION__, msg_head.subtype);
                error = IMEI_CMD_ERR;
                result = 0;
            }

            msg_head.len = sizeof(MSG_HEAD_T) + sizeof(unsigned short);

            if (result) {
                unsigned short value = 0;
                msg_head.subtype = MSG_ACK;
                memcpy(buf+1, &msg_head, sizeof(MSG_HEAD_T));
                memcpy(buf + sizeof(MSG_HEAD_T) + 1, &value, sizeof(value));
            } else {
                unsigned short value = error;
                msg_head.subtype = MSG_NACK;
                msg_head.len = sizeof(MSG_HEAD_T) + 2 * sizeof(unsigned short);

                memcpy(buf + 1, &msg_head, sizeof(MSG_HEAD_T));
                memcpy(buf + 1 + sizeof(MSG_HEAD_T), &value, sizeof(value));
                value = 0;
                memcpy(buf + 1 + sizeof(MSG_HEAD_T) + sizeof(unsigned short), &value, sizeof(value));
            }

            rsp_len = translate_packet(rsp, (unsigned char *)(buf+1), msg_head.len);
        } else {  // Read Mode
            ENG_LOG("%s: Read Mode ! \n", __FUNCTION__);
            nvdata = (unsigned char *)malloc(MAXSIZE_CALIPARAM_PHASECHECK + sizeof(MSG_HEAD_T) + sizeof(unsigned short));
            memcpy(nvdata, &msg_head, sizeof(MSG_HEAD_T));

#ifdef TEST_CALIPARAM
            memcpy(nvdata+sizeof(MSG_HEAD_T), s_test_phasecheck_sp09, sizeof(s_test_phasecheck_sp09));
            ret = TRUE;
#else
            ret = CaliParam_ReadOffset(PARAM_PHASHCHECK, 0, MAXSIZE_CALIPARAM_PHASECHECK, nvdata + sizeof(MSG_HEAD_T));
#endif
            if (!ret) {
                unsigned short value = 0;
                ENG_LOG("%s:Read ERROR: %d\n", __FUNCTION__, ret);
                msg_head.len = sizeof(MSG_HEAD_T) + sizeof(unsigned short);
				msg_head.subtype = MSG_NACK;
                memcpy(nvdata, &msg_head, sizeof(MSG_HEAD_T));
                memcpy(nvdata + sizeof(MSG_HEAD_T), &value, sizeof(value));
            } else {
                unsigned short value = 0;
                //magic = *((uint32_t*)(nvdata + sizeof(MSG_HEAD_T)));
                memcpy(&magic, nvdata + sizeof(MSG_HEAD_T), sizeof(magic));
                ENG_LOG("%s:phase check magic: %x\n", __FUNCTION__,magic);
                if(magic == SP09_SPPH_MAGIC_NUMBER){
                    sn_len = sizeof(SP09_TEST_DATA_INFO_T);
                }else if(magic == SP15_SPPH_MAGIC_NUMBER){
                    sn_len = sizeof(SP15_TEST_DATA_INFO_T);
                }

                msg_head.len = sizeof(MSG_HEAD_T) + sn_len + sizeof(unsigned short);
                msg_head.subtype = MSG_ACK;

                value = crc16(0, ((unsigned char *)(nvdata + sizeof(MSG_HEAD_T))), sn_len);
                memcpy(nvdata, &msg_head, sizeof(MSG_HEAD_T));
                memcpy(nvdata+sizeof(MSG_HEAD_T)+sn_len, &value, sizeof(value));
            }

            rsp_len = translate_packet(rsp, (unsigned char *)(nvdata), msg_head.len);

            free(nvdata);
        }
    } while (0);

  CaliParam_Close();
  ENG_LOG("%s: rsp_len : %d\n", __FUNCTION__, rsp_len);

  return rsp_len;
}
