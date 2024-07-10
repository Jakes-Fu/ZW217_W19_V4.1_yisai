#ifndef _ABUP_MAIN_H
#define _ABUP_MAIN_H
 
#include "abup_typedef.h"

#ifdef __SERIAL_FLASH_EN__
#define NOR_PAGE_SIZE             (512)         //512 bytes
#else
#define NOR_PAGE_SIZE             (2048)        //2048 bytes
#endif

#define __MULTI_PACKAGE__

typedef void (*abup_bl_ratio_callback)(abup_uint16 ratio);

typedef struct multi_head_info{
	abup_uint32 multi_bin_offset;
	abup_uint32 multi_bin_address;
	abup_uint8 multi_bin_method;
	abup_uint8 multi_bin_resever[7];
}multi_head_info_patch;

typedef enum
{
   ABUP_LOG_DEBUG=1,
   ABUP_LOG_LOOP=2,
   ABUP_LOG_DELTA=3,
   ABUP_LOG_MCU=4,
   ABUP_LOG_NONE,
}ABUP_LOG_LEVEL;


//Error code

#define ABUP_FUDIFFNET_ERROR_NONE                                               (0)
#define ABUP_FUDIFFNET_ERROR_INSUFFICIENT_WORKING_BUF                           (-100)
#define ABUP_FUDIFFNET_ERROR_FULL_UPDATE_FAIL                                   (-101)

//Updating related
#define ABUP_FUDIFFNET_ERROR_UPDATE_ERROR_START                                 (-600)
#define ABUP_FUDIFFNET_ERROR_UPDATE_ERROR_END                                   (-699)

#define ABUP_MCU_PATCH_FAIL     (-700)

abup_extern abup_uint8 AbupProcedure(abup_bl_ratio_callback cb);
abup_extern abup_uint32 abup_bl_write_block(abup_uint8* src, abup_uint32 start, abup_uint32 size);
abup_extern abup_uint32 abup_bl_read_block(abup_uint8* dest, abup_uint32 start, abup_uint32 size);
abup_extern void abup_bl_debug_print(void* ctx, const abup_char* fmt, ...);
abup_extern abup_uint8* abup_bl_get_working_buffer(void);
abup_extern abup_uint64 abup_bl_get_working_buffer_len(void);
abup_extern abup_uint32 abup_bl_get_app_base(void);
abup_extern abup_uint32 abup_bl_get_diff_param_size(void);
abup_extern abup_uint32 abup_bl_flash_block_size() ;
abup_extern abup_uint32 abup_bl_get_delta_base(void);


#endif

