#ifndef FDL_NAND_H
#define FDL_NAND_H

#define NAND_SUCCESS                0
#define NAND_SYSTEM_ERROR           1
#define NAND_UNKNOWN_DEVICE         2
#define NAND_INVALID_DEVICE_SIZE    3
#define NAND_INCOMPATIBLE_PART      4
#define NAND_INVALID_ADDR           5
#define NAND_INVALID_SIZE           6
#define NAND_INVALID_STL_SIZE       7

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 * change the header of first bootloader page
 ******************************************************************************/
int nand_change_bootloader_header (unsigned int *bl_start_addr);

int nand_flash_init (void);
int nand_format (void);
int nand_erase (unsigned int addr, unsigned int size);
int nand_start_write (unsigned int addr, unsigned int size);
int nand_write (unsigned int size, unsigned char *buf);
int nand_end_write (void);
int nand_read (unsigned int addr, unsigned int off, unsigned int size, unsigned char *buf);
int nand_read_NBL (void *buf);

#ifdef POWER_OFF_PROTECT_SUPPORT
void BackupOrUpdateNvPart(void);
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FDL_NAND_H */