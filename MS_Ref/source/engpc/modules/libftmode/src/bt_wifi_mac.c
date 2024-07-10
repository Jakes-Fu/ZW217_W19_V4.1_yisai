#include "sci_types.h"
#include "sfs.h"

#include "crc16.h"
#include "sprd_fts_diag.h"
#include "sprd_fts_log.h"

#include "caliparam.h"
#include "wifi_api.h"

#include "ftmode.h"
#include "bt_wifi_mac.h"
#include "translate_packet.h"
#include "bt_abs.h"


#ifdef TEST_CALIPARAM
uint8 s_REF_NVWriteDirect_T[] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x9F,0x11,0x36,0xDA,0x45,0x40,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x45,0xDA,0x3B,0x69,0xE1,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00
};
#endif



typedef uint32 uint32_t;
#define size_t uint32
#define off_t uint32

int btwifi_checkCRC(char* buff){
    MSG_HEAD_T msg_head = {0};
    int checksum = 0;
    unsigned short crc_sum = 0;
    memcpy(&msg_head, buff+1, sizeof(MSG_HEAD_T));

    memcpy(&crc_sum, buff+1+msg_head.len-sizeof(unsigned short), sizeof(unsigned short));
    checksum = crc16(0, (unsigned char *)(buff+sizeof(MSG_HEAD_T) + 1), msg_head.len - sizeof(MSG_HEAD_T) - sizeof(unsigned short));
    if (crc_sum != checksum){
        ENG_LOG("%s: CRC Error! crc_sum: %d, checksum: %d\n", __FUNCTION__, crc_sum, checksum);
        return -1;
    }

    return 0;
}

int btwifi_checkValid(char *buff){
    MSG_HEAD_T msg_head = {0};
    int cmd_mask;

    memcpy(&msg_head, buff+1, sizeof(MSG_HEAD_T));

    if (0 != btwifi_checkCRC(buff)){
        return 0;
    }

    if ((msg_head.subtype & RW_MASK) == WRITE_MODE && (msg_head.subtype & RM_VALID_CMD_MASK) == 0) {
        return 0;
    }

    if (0 != (cmd_mask = (msg_head.subtype & 0x7F))) {
        ENG_LOG("%s: cmd_mask: %d, subtype: %d\n", __FUNCTION__, cmd_mask, msg_head.subtype);

        if ((cmd_mask & DIAG_CMD_BTBIT) || (cmd_mask & DIAG_CMD_WIFIBIT)) {
            ENG_LOG("%s: Get BT/WIFI Mac addr req or IMEI req!\n", __FUNCTION__);
            if (cmd_mask = (cmd_mask & (~(DIAG_CMD_BTBIT | DIAG_CMD_WIFIBIT)))) {
                ENG_LOG("%s: Have other commands !\n", __FUNCTION__);
                return 2;
            } else {
                ENG_LOG("%s: No other commands !\n", __FUNCTION__);
                return 1;
            }
        }
    }

    return 0;
}

int btwifiimei(char *buf, int len, char *rsp, int rsplen, int cp_ap_proc, int ap_cali) {
    int rlen = 0, i;
    int ret = -1;
    int cmd_mask = 0;
    unsigned short crc = 0;
    unsigned char crc1, crc2, crc3, crc4;
    char tmp;
    char btaddr[32] = {0};
    char wifiaddr[32] = {0};
    char tmprsp[512] = {0};
    int headlen = sizeof(MSG_HEAD_T);
    REF_NVWriteDirect_T direct = {0};
    MSG_HEAD_T head_ptr = {0};
	memcpy(&head_ptr, buf+1, sizeof(MSG_HEAD_T));
    cmd_mask = head_ptr.subtype & 0x7f;
    ENG_LOG("Call %s, subtype=%x\n", __FUNCTION__, head_ptr.subtype);

    if ((head_ptr.subtype & DIAG_CMD_READ) == 0) {  // write command

    	memcpy(&direct, buf + DIAG_HEADER_LENGTH + 1, sizeof(REF_NVWriteDirect_T));
		memcpy(&crc1, buf + DIAG_HEADER_LENGTH + sizeof(REF_NVWriteDirect_T) + 1, sizeof(char));
		memcpy(&crc2, buf + DIAG_HEADER_LENGTH + sizeof(REF_NVWriteDirect_T) + 2, sizeof(char));
        crc = crc16(crc, (const unsigned char *)(&direct), sizeof(REF_NVWriteDirect_T));
        crc3 = crc & 0xff;
        crc4 = (crc >> 8) & 0xff;
        ENG_LOG("%s: crc [%x,%x], [%x,%x]\n", __func__, crc3, crc4, crc1, crc2);

        if ((crc1 == crc3) && (crc2 == crc4)) {
            if ((cmd_mask & DIAG_CMD_BTBIT) || (cmd_mask & DIAG_CMD_WIFIBIT)) {
                // write bt address
                if ((head_ptr.subtype & DIAG_CMD_BTBIT) > 0) {
                    sprintf(btaddr, "%02x:%02x:%02x:%02x:%02x:%02x", direct.btaddr[5],
                            direct.btaddr[4], direct.btaddr[3], direct.btaddr[2],
                            direct.btaddr[1], direct.btaddr[0]);
                    ENG_LOG("%s: BTADDR:%s\n", __func__, btaddr);
                }

                // write wifi address
                if ((head_ptr.subtype & DIAG_CMD_WIFIBIT) > 0) {
                    sprintf(wifiaddr, "%02x:%02x:%02x:%02x:%02x:%02x",
                            direct.wifiaddr[0], direct.wifiaddr[1], direct.wifiaddr[2],
                            direct.wifiaddr[3], direct.wifiaddr[4],
                            direct.wifiaddr[5]);
                    ENG_LOG("%s: WIFIADDR:%s\n", __func__, wifiaddr);
                    if(direct.wifiaddr[0] & 0x03){
                        //wifi addr error break;
                        head_ptr.subtype = 0x00;

                        memcpy(tmprsp, (unsigned char *)(&head_ptr), headlen);                        
                        rlen = translate_packet(rsp, tmprsp, head_ptr.len+2);
                        return rlen;
                    }
                }

                ret = eng_btwifimac_write(&(direct.btaddr), sizeof(direct.btaddr), &(direct.wifiaddr), sizeof(direct.wifiaddr));
            }

            ENG_LOG("%s: WIFIADDR g_ap_cali_flag:%d,s_cp_ap_proc=%d\n", __func__, ap_cali,cp_ap_proc);
        }

        if (!cp_ap_proc) {
            if (ret <= 0) {  // fail
                head_ptr.subtype = 0x00;
            } else {
                head_ptr.subtype = 0x01;
            }

			head_ptr.len = headlen+2;
            memcpy(tmprsp, (unsigned char *)(&head_ptr), headlen);

            rlen = translate_packet(rsp, tmprsp, head_ptr.len);
        }
    } else {  // read command

        if ((cmd_mask & DIAG_CMD_BTBIT) || (cmd_mask & DIAG_CMD_WIFIBIT)) {
            // read btaddr
            if ((head_ptr.subtype & DIAG_CMD_BTBIT) > 0) {
                ret = eng_btwifimac_read(&(direct.btaddr), ENG_BT_MAC);
                sprintf(btaddr, "%02x:%02x:%02x:%02x:%02x:%02x", direct.btaddr[5],
                            direct.btaddr[4], direct.btaddr[3], direct.btaddr[2],
                            direct.btaddr[1], direct.btaddr[0]);
                ENG_LOG("%s: after BTADDR:%s\n", __func__, btaddr);

            }
            // read wifiaddr
            if ((head_ptr.subtype & DIAG_CMD_WIFIBIT) > 0) {
                ret = eng_btwifimac_read(&(direct.wifiaddr), ENG_WIFI_MAC);
                sprintf(wifiaddr, "%02x:%02x:%02x:%02x:%02x:%02x",
                        direct.wifiaddr[0], direct.wifiaddr[1], direct.wifiaddr[2],
                        direct.wifiaddr[3], direct.wifiaddr[4],
                        direct.wifiaddr[5]);
                ENG_LOG("%s: WIFIADDR:%s\n", __func__, wifiaddr);
            }
        }

        // response
        head_ptr.subtype = 0x01;
        memcpy(tmprsp, (unsigned char *)(&head_ptr), headlen);
		memcpy(tmprsp+headlen, &direct, sizeof(REF_NVWriteDirect_T));

        rlen = sizeof(REF_NVWriteDirect_T);
        crc = crc16(crc, (const unsigned char *)(&direct), sizeof(REF_NVWriteDirect_T));

        //*(tmprsp + headlen + rlen) = crc & 0xff;
        crc1 = crc&0xff;
        memcpy(tmprsp + headlen + rlen, &crc1, sizeof(crc1));
        //*(tmprsp + headlen + rlen + 1) = (crc >> 8) & 0xff;
        crc2 = (crc >> 8) & 0xff;
        memcpy(tmprsp + headlen + rlen + 1, &crc2, sizeof(crc2));
        ENG_LOG("%s: read crc = %d, [%x,%x]\n", __func__, crc, crc1, crc2);
        rlen += 2;

        head_ptr.len = headlen + rlen;
        memcpy(tmprsp, &head_ptr, sizeof(MSG_HEAD_T));
        rlen = translate_packet(rsp, tmprsp, headlen + rlen);
    }

    // clear BT/WIFI bit in this diag framer
    if (cp_ap_proc) {
        head_ptr.subtype &= ~(DIAG_CMD_BTBIT | DIAG_CMD_WIFIBIT);
    }

    ENG_LOG("%s: rlen=%d\n", __func__, rlen);
    return rlen;
}

typedef enum {
    BT_MAC_ADDR=0,
    WIFI_MAC_ADDR
}MAC_ADDR;

int eng_btwifimac_write(uint8* bt_mac, int len_bt, uint8 *wifi_mac, int len_wifi) {
  int retbt = 0;
  int retwifi = 0;
  int len = 0;
  BT_ADDRESS addr = {0};

  ENG_LOG("set BT/WIFI mac");

#ifdef TEST_CALIPARAM
  retbt = 1;
  retwifi = 1;
#else
  // If no bt_mac or wifi_mac, we can randomly generate them.

  len = (len_bt>BT_MACADDR_MAX_SIZE)?BT_MACADDR_MAX_SIZE:len_bt;
  memcpy(&(addr.addr), bt_mac, len);
  //retbt = CaliParam_WriteOffset(PARAM_WCN_CALI, BT_MACADDR_BASE, len, bt_mac);
  //retbt = wifi_set_mac(bt_mac);
  BTI_SaveBdAddr(&addr);
  retbt = 1;

  
  len = (len_wifi>WIFI_MACADDR_MAX_SIZE)?WIFI_MACADDR_MAX_SIZE:len_wifi;
  //retwifi = CaliParam_WriteOffset(PARAM_WCN_CALI, WIFI_MACADDR_BASE, len, wifi_mac);
  retwifi = wifi_set_mac(wifi_mac);
  retwifi = (retwifi == 0)?1:0;

#endif

  return (retbt==1&&retwifi==1)?1:0;
}

int eng_btwifimac_read(char *mac, MacType type) {
  int ret = 0;

  if (!mac) return -1;

#ifdef TEST_CALIPARAM
  {
      REF_NVWriteDirect_T drt = {0};
      memcpy(&drt, &s_REF_NVWriteDirect_T, sizeof(REF_NVWriteDirect_T));
      if (ENG_WIFI_MAC == type) {
        memcpy(mac, &(drt.wifiaddr), sizeof(drt.wifiaddr));
      }else{
        memcpy(mac, &(drt.btaddr), sizeof(drt.btaddr));
      }

      ret = 1;
  }
#else
  if (ENG_WIFI_MAC == type) {
    // read wifi mac
    ret = wifi_get_mac(mac);
    ret = (ret == 0)?1:0;
  } else {
    BT_CONFIG cfg = {0};
    // read bt mac
    BT_GetConfigInfo(&cfg);
    memcpy(mac, &(cfg.bd_addr), sizeof(cfg.bd_addr));

    ret = 1;
  }
#endif

  return ret;
}
