#ifndef _BT_WIFI_MAC_H
#define _BT_WIFI_MAC_H


#define RW_MASK 0x80 
#define WRITE_MODE 0
#define RM_VALID_CMD_MASK 0x7F

#define MAX_IMEI_LENGTH 8
#define MAX_IMEI_STR_LENGTH 15
#define MAX_BTADDR_LENGTH 6
#define MAX_WIFIADDR_LENGTH 6
#define GPS_NVINFO_LENGTH 44
#define DIAG_HEADER_LENGTH 8

#define DIAG_CMD_IMEI1BIT 0x01
#define DIAG_CMD_IMEI2BIT 0x02
#define DIAG_CMD_IMEI3BIT 0x10
#define DIAG_CMD_IMEI4BIT 0x20
#define DIAG_CMD_BTBIT 0x04
#define DIAG_CMD_WIFIBIT 0x40

#define DIAG_CMD_READ 0x80 

#define IMEI_NUM 4

#define WCN_CALI_PARAM_BASE       (8*1024)

#define BT_MACADDR_BASE           (WCN_CALI_PARAM_BASE)
#define BT_MACADDR_MAX_SIZE       (MAX_BTADDR_LENGTH)

#define WIFI_MACADDR_BASE         (BT_MACADDR_BASE+BT_MACADDR_MAX_SIZE)
#define WIFI_MACADDR_MAX_SIZE     (MAX_WIFIADDR_LENGTH)


typedef struct {
    unsigned char imei1[MAX_IMEI_LENGTH];
    unsigned char imei2[MAX_IMEI_LENGTH];
    unsigned char btaddr[MAX_BTADDR_LENGTH];
    unsigned char gpsinfo[GPS_NVINFO_LENGTH];
    unsigned char wifiaddr[MAX_WIFIADDR_LENGTH];
    unsigned char reserved1[2];
    unsigned char imei3[MAX_IMEI_LENGTH];
    unsigned char imei4[MAX_IMEI_LENGTH];
    unsigned char reserved2[16];
} REF_NVWriteDirect_T;

typedef enum
{
    ENG_BT_MAC,
    ENG_WIFI_MAC
} MacType;

int eng_btwifimac_write(uint8* bt_mac, int len_bt, uint8 *wifi_mac, int len_wifi);
int eng_btwifimac_read(char* mac, MacType type);
int btwifi_checkCRC(char* buff);
int btwifi_checkValid(char *buff);
int btwifiimei(char *buf, int len, char *rsp, int rsplen, int cp_ap_proc, int ap_cali);

#endif