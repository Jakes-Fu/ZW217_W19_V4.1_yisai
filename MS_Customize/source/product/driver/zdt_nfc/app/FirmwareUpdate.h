#ifndef __FIRMWARE_UPDATE_H__
#define __FIRMWARE_UPDATE_H__

#include "MyInclude.h"

#define FRAME_LEN 1024
#define SOH 0x1
#define EOT 0x4
#define RSP_LEN 12

enum CMD_SET {
    CMD_DOWNLOAD,
    CMD_DOWN_TO_RAM,
    CMD_DUMP_MEMORY,
    CMD_WRITE_FLASH,
    CMD_ERASE_PAGE,
    CMD_SCAN_TEST,
    CMD_BIST_TEST,
    CMD_WRITE_WITH_ERASE,
    CMD_ERASE_FLASH,
    CMD_ERASE_REF_CELL,
    CMD_GOTO_FLASH,
    CMD_WRITE_REG,
    CMD_READ_REG,
    CMD_CONFIG_FLASH_WRITE,
    CMD_CONFIG_FLASH_READ,
    CMD_CALIBRATE,
    CMD_CHECK_INTEGRITY,
    CMD_WRITE_INFO,
    CMD_GOTO_SLEEP,
};

extern uint16_t calc_crc(uint8_t * dat, uint16_t len);
extern uint16_t recv_data(uint8_t * data);
extern void dump_memory(int handle, unsigned int addr, unsigned short len);
extern uint8_t write_flash(uint32_t addr, uint8_t * hexstring, uint8_t erase, uint16_t len);
extern uint8_t read_reg(uint32_t addr);
extern void write_reg(uint32_t addr, uint8_t val);
extern uint8_t write_info(uint32_t addr, uint8_t * hexstring, uint8_t len);
extern uint8_t erase_chip(unsigned char val);
extern void config_flash_read(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3);
extern t_RetStatus download_firmware_start(unsigned int addr, uint32_t filesize);
extern t_RetStatus download_firmware(uint8_t * firmware, uint8_t cur_seq, uint32_t len);
extern t_RetStatus download_firmware_end(uint8_t * firmware, uint8_t cur_seq, uint32_t len);
extern uint8_t NFCC_write_info(uint32_t addr, uint8_t * hexstring, uint16_t len);
extern void calibrate(void);

extern void write_info_init_recovery(uint8_t *info_buff,uint8_t len);
extern uint16_t update_proc_from_code(void);

extern void NFC_Firmware_Update(uint16_t flag,uint8_t *info_buff,uint8_t len);
extern void write_info_init(void);
#endif
