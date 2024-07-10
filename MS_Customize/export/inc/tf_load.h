#ifndef _TF_LOAD_H_
#define _TF_LOAD_H_
#include "os_api.h"
#include "dal_lcd.h"
#ifdef TF_FDL_BUILD
#include "ref_lcd.h"
#endif

/*tf update type magic number: normal update: 0x54464E53; protect update: other value*/
#define TF_UPDATE_TYPE_MAGIC        0x54464E53

typedef struct{
	uint32 	fixnv_addr;//valid fixnv address in ps
	uint32 	alloc_buf_addr;
	uint32 	manu_id;
	uint32 	dev_id;
	uint32 	ext_id;
} TF_FLASH_INFO;

#ifndef TF_FDL_BUILD
typedef enum
{
    LCD_ROT_MODE_LCD = 0x00,
    LCD_ROT_MODE_DMA,
    LCD_ROT_MODE_MAX
}LCD_ROT_MODE_E;
#endif

typedef struct{
	LCD_INFO_T    		  lcd_info;
	LCD_BUS_WIDTH_E        bus_width;
	LCD_DIRECT_E 		  dir_type;
	LCD_ROT_MODE_E     	  rotate_mode;
	uint32                            data_addr;
	uint16*				  block_buf_ptr;                           
} TF_LCD_INFO;

typedef struct
{	
	 BOOLEAN (*_flash_write)(uint32 addr, const uint8 * buf, uint32 len);
	 BOOLEAN (*_flash_writeword)(uint32 addr, uint16 data);
	 BOOLEAN (*_flash_read)(uint32 addr, uint8 *buf, uint32 read_len);
	 BOOLEAN (*_flash_erase)(uint32 addr);
	 uint16 (*_flash_readword)(uint32 addr);
}TF_FUNC_EXT;


typedef struct{
	TF_FLASH_INFO   tf_update_flash;
	TF_LCD_INFO      tf_update_lcd;
	uint32                 tf_update_magic; //normal update: 0x54464E53; protect update: other value
}TF_UPDATE_INFO, * TF_UPDATE_INFO_PTR;

typedef struct{
	TF_FLASH_INFO   tf_update_flash;
	TF_LCD_INFO     tf_update_lcd;
	uint32          tf_update_magic; //normal update: 0x54464E53; protect update: other value
}TF_UPDATE_INFO_T;

/*****************************************************************************/
//  Function name:	TF_GetBootExecAddress
//  Description:This function is used to get tf bootloader.bin start exec address.
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC uint32 TF_GetBootExecAddress(void);
               
/*****************************************************************************/
//  Function name:	TF_LoadInit
//  Description:This function is used to do tf update  init
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void  TF_LoadInit(void);

/*****************************************************************************/
//  Function name:	TF_Jump
//  Description:This function is used to jump to target
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC void  TF_Jump(void);

/*****************************************************************************/
//  Discription:TF  load string compare
//  Global resource dependence: none 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC int TF_LoadWstrCMP(
                             const unsigned short * string1,
                             const unsigned short * string2
                             );
                             
/*****************************************************************************/
//  Function name:	TF_EnterInit
//  Description:This function is used to exit system
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC  void  TF_EnterInit(void);


/*****************************************************************************/
//  Function name:	TF_GetDiabsleAssertFlag
//  Description:This function is used to get allow disbale assert flasg
//  Global resource dependence: 
//  Author: tao.feng
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN   TF_GetDiabsleAssertFlag(void);


#endif // _TF_LOAD_H_

