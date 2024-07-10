/*
 *
 * ��Ȩ(c) 2014 ��ʯ���⣨�������Ƽ����޹�˾
 * ��Ȩ(c) 2007-2014 ��ʯ���⣨�������Ƽ����޹�˾��Ȩ����
 * 
 * ���ĵ�������Ϣ��˽�ܵ���Ϊ��ʯ����˽�У������񷨺��̷��ж���Ϊ��ҵ������Ϣ��
 * �κ���ʿ�������Ը�ӡ��ɨ����������κη�ʽ���д����������ʯ������Ȩ׷���������Ρ�
 * �Ķ���ʹ�ñ����ϱ�������Ӧ��������Ȩ���е��������κͽ�����Ӧ�ķ���Լ����
 *
 */

  
#ifndef __RS_UA_FOTA_PORTING_H__
#define __RS_UA_FOTA_PORTING_H__
#include "rsplatform.h"
#define PROGRESS_EXTEND_PRE  (20)
#define PROGRESS_NORMAL_PRE (100 - PROGRESS_EXTEND_PRE)

typedef enum 
{
	UPSTATE_IDLE , /*��״̬*/
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

