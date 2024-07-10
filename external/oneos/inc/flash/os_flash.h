#ifndef __OS_FLASH_H__
#define __OS_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

int nor_flash_read(unsigned int addr, unsigned char *buf, unsigned int size);
int nor_flash_write(unsigned int addr, unsigned char *buf, unsigned int size);
int nor_flash_erase(unsigned int addr, unsigned int size);

int onchip_flash_read(unsigned int addr, unsigned char *buf, unsigned int size);
int onchip_flash_write(unsigned int addr, unsigned char *buf, unsigned int size);
int onchip_flash_erase(unsigned int addr, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif