/*
 *
 * 版权(c) 2014 红石阳光（北京）科技有限公司
 * 版权(c) 2007-2014 红石阳光（北京）科技有限公司版权所有
 * 
 * 本文档包含信息是私密的且为红石阳光私有，且在民法和刑法中定义为商业秘密信息。
 * 任何人士不得擅自复印，扫描或以其他任何方式进行传播，否则红石阳光有权追究法律责任。
 * 阅读和使用本资料必须获得相应的书面授权，承担保密责任和接受相应的法律约束。
 *
 */

  
#ifndef __RS_UA_FOTA_PORTING_H__
#define __RS_UA_FOTA_PORTING_H__
#include "rsplatform.h"
#define PROGRESS_EXTEND_PRE  (20)
#define PROGRESS_NORMAL_PRE (100 - PROGRESS_EXTEND_PRE)

typedef enum 
{
	UPSTATE_IDLE , /*无状态*/
	UPSTATE_CHECK, /* check  */
	UPSTATE_UPDATE, /* update */
	UPSTATE_UPDATEED,/* update finish */
}e_updateState;

void rs_ua_postUpdateProgress(rs_u32 current , rs_u32 total);
void rs_ua_kick_watchdog(void);
rs_u32 rs_ua_sdk_heap_addr();
rs_u32 rs_ua_sdk_heap_size();
rs_u32 rs_ua_ram_block();
rs_u32 rs_ua_ram_block_size();
rs_u32 rs_ua_setDeltaPartitionAddr(rs_u32 addr);
rs_u32 rs_ua_getStartAddr(rs_u32 addr);
rs_u32 rs_ua_setStartAddr(rs_u32 addr);
rs_u32 rs_ua_getPackageAddr();
rs_u32 rs_ua_getUpdateInfo1Addr();
rs_u32 rs_ua_getUpdateInfo2Addr();

rs_u32 rs_ua_saveExtendData(rs_u8* buff,rs_u32 size, rs_u32 offset,rs_bool isFinal);
rs_u32 rs_ua_processExtendData();

rs_s32 rs_ua_flash_init();
rs_bool rs_ua_flash_read(rs_u8 *destination, volatile rs_u8 *address, rs_u32 size);
rs_bool rs_ua_flash_write(volatile rs_u8 *address, rs_u8 *source, rs_u32 size);
rs_bool rs_ua_flash_erase(volatile rs_u8 *address, rs_u32 size);

rs_u32 rs_trace(const char *format,...);
	

#endif

