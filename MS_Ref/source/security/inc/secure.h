#ifndef _SECURE_H_
#define _SECURE_H_

#include "sci_types.h"

#define IRAM_BASE   0x40000000

#define VLR_MAGIC	(0x524c56ff)
#define SHA1NUM_SIZ (20)

#define CODE_MAGIC	0x434F4445

#define TAG_NUM		(3)

//Bootloader signed image and others 
#define VECTOR_SIZE		(32)
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#define MAGIC_SIZE		(4)
#else
#define MAGIC_SIZE		(0)
#endif
#define HASH_OFF 		(VECTOR_SIZE + MAGIC_SIZE)
#define HASH_SIZE		(4)
#define FLASH_CONF_SIZ  (48)
#define CUS_DATA_OFF	(VECTOR_SIZE + MAGIC_SIZE + HASH_SIZE + FLASH_CONF_SIZ)
#define CUS_DATA_SIZ	(648 - MAGIC_SIZE)
#define RSAPUK_OFF		(CUS_DATA_OFF + CUS_DATA_SIZ)
#define RSAPUK_SIZ		(260)
#define CODE_OFF		(RSAPUK_OFF+RSAPUK_SIZ+VECTOR_SIZE)
#define CODE_SIZ		(10*1024)

//KERNEL signed image and others 
#define FILE_HEAD_SIZ	(512)
#define VLR_INFO_OFF	(FILE_HEAD_SIZ)
#define VLR_INFO_SIZ	(512)
#define SECURE_CODE_OFF	(FILE_HEAD_SIZ+VLR_INFO_SIZ)	

//FDL signed image and others 
#define FDL_CODE_SIZE	(40*1024)
#define KEY_OFF			(FDL_CODE_SIZE)
#define KEY_SIZE			(512)
#define VLR_OFF			(KEY_OFF + KEY_SIZE)
#define VLR_SIZE			(512)


#define HEADER_LEN		SECURE_CODE_OFF
#define HASH_LENGTH 	(1024*14) 
#define BOOT_OFFSET  	(32)


typedef struct {
    uint32 tag;
    uint32 offset;	//sectors
    uint32 size;	//sectors
    uint32 reserved;
}tag_t;

typedef struct {
    uint8   magic[16];
    uint32  version;
    uint32  tagsnum;
    uint8   reserved[8];
    tag_t   code;
    tag_t   vlr;
    tag_t   puk;
}file_header_t;

typedef struct {
	uint32 e;
	uint8 m[128];//NOTE: packed struct
	uint8 r2[128];//RSA public key 
}puk_info_t;   

typedef struct {
    uint32  magic;
    uint8  hash[128];
    uint32  setting;
    uint32  length;
    uint8   reserved[20];
}vlr_info_t;

typedef enum {
    SECURE_RESULT_SUCCESS = 0,    
    SECURE_RESULT_FAIL,
    SECURE_RESULT_DISABLE,
    SECURE_RESULT_ENABLE,    
}SECURE_RETURN_E;

typedef struct {
	uint32 *g_TraceValue;
	uint8 *efuse_harsh_data;
	uint8 *soft_harsh_data;
	BOOLEAN (*CheckSecureBootEnable)();
	int (*HarshVerify)(uint8 *, uint32);
	void (*RSA_ModPower)(unsigned int *p, unsigned int *m, unsigned int *r2, unsigned int e);
}harsh_func_t;


PUBLIC uint32* get_harsh_func(void);
PUBLIC BOOLEAN CheckSecureBootState(void);
PUBLIC int harshVerify(uint8 *data, uint32 data_len, uint8 *data_hash, uint8 *data_key);
PUBLIC int Verify_SecureHeader(const file_header_t *pHeader);
PUBLIC uint8 Security_Enable(void);
PUBLIC uint32 Anti_Clone_Enable();
PUBLIC void SecureMode_WCNuartEnable(void);
PUBLIC uint32 Write_Public_Key(void);
#endif
