#include "ref_outport.h"
#include "ref_param.h"
#include "nvitem.h"
#include "nv_item_id.h"
#include "nv_hwversion_adpter.h"

/**---------------------------------------------------------------------------*
 **                             Compiler Flag                                 *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C"
    {
#endif

#define DELTA_NV_BUF_SIZE 1024*100
#define NV_MAGIC_LEN 32
#define NV_DIFF_FILE_NAME_MAX 20
#define NV_DIFF_EMPETY_FILE_LEN 2
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif
//#define MIN(x,y) (x>y) ? (y):(x)  //not used in this file

typedef struct _ITEM_STRUCT
{
	uint16 item_id;
	uint16 item_offset;
	uint16 value_len;
}ITEM_STRUCT;//__attribute__((__packed__))

typedef struct _NV_DIFF_FILE_STRUCT
{
	char file_name[NV_DIFF_FILE_NAME_MAX];
	uint16 nv_ver_flag;
	uint32 file_len;
}NV_DIFF_FILE_STRUCT;

typedef struct _DELTA_NV_BUF_STRUCT
{
	uint8  head[NV_MAGIC_LEN];
	uint8 nv_config[DELTA_NV_BUF_SIZE];
	uint8  tail[NV_MAGIC_LEN];
}__attribute__((__packed__))DELTA_NV_BUF_STRUCT;

__align(4)  static const DELTA_NV_BUF_STRUCT delta_nv_buf_info = {0};


extern uint32 SPIFLASH_GetOperatorDataAddress(void);
extern uint32 SPIFLASH_GetOperatorDataSize(void);

static char g_nvFileName[20] = {0};
static char g_paFileName[20] = {0};

static void bytes_to_u16(uint8* bytes, uint16* u16)
{
    *u16 = (uint16)(*bytes) + ((uint16)(*(bytes + 1)) << 8);
}

static void bytes_to_u32(uint8* bytes, uint32* u32)
{
    *u32 = (uint32)(*(bytes) ) +
               ((uint32)(*(bytes + 1)) << 8) +
               ((uint32)(*(bytes + 2)) << 16) + ((uint32)(*(bytes + 3))<< 24);
}

PUBLIC char* REFPARAM_GetFileName(void)
{
	uint32 hw_num = 0;
	hw_num = SCI_Get_RF_VERSION();
	if(hw_num > 99) {	//hw_num must less than 99 (%2d), otherwise the filename will be incorrect
		return NULL;
	}
	snprintf(g_nvFileName, sizeof(g_nvFileName), "hw_ver%02d", hw_num);

	return g_nvFileName;
}

PUBLIC char* REFPARAM_GetPAFileName(void)
{
	uint32 pa_num = 0;
	pa_num = SCI_Get_PA_VERSION();
	if(pa_num > 99) {	//pa_num must less than 99 (%2d), otherwise the filename will be incorrect
		return NULL;
	}
	snprintf(g_paFileName, sizeof(g_paFileName), "pa_ver%02d", pa_num);

	return g_paFileName;
}

PUBLIC void REFPARAM_GetFileVersion(uint8* rf_version, uint8* pa_version)
{
	REF_PARAM_T ref_param = {0};
	uint32 status = NVERR_NOT_OPENED;
	if(rf_version == NULL || pa_version == NULL) {
		return;
	}

	SCI_MEMSET( (uint8 *)(&ref_param), 0, sizeof(ref_param) );

	status = EFS_NvitemRead(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&ref_param));
	if (NVERR_NONE == status) {
		*rf_version = ref_param.nv_ver_flag;
		*pa_version = ref_param.pa_ver_flag;
	} else {
		*rf_version = 0;
		*pa_version = 0;
	}
}

PUBLIC BOOLEAN REFPARAM_SetFileVersion(uint8 rf_ver_flag, uint8 pa_ver_flag)
{
	REF_PARAM_T ref_param = {0};
	uint32 status = NVERR_NOT_OPENED;
	SCI_MEMSET( (uint8 *)(&ref_param), 0, sizeof(ref_param) );

	status = EFS_NvitemRead(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&ref_param));
	if (NVERR_NONE == status) {
		ref_param.nv_ver_flag = rf_ver_flag;
		ref_param.pa_ver_flag = pa_ver_flag;
		EFS_NvitemWrite(NV_REF_PARAMETER, REF_PARAM_T_SIZE, (uint8 *)(&ref_param), 0);
		return SCI_TRUE;
	} else {
		SCI_TRACE_LOW("REFPARAM_SetFileVersion ERR");
		return SCI_FALSE;
	}
}

PUBLIC BOOLEAN  get_deltanv_info(uint32* deltanv_addr, uint32* p_size)
{
	if (NULL == deltanv_addr) {
		SCI_TRACE_LOW("get_deltanv_info deltanv_addr is NULL");
		return FALSE;
	}

	if (NULL == p_size) {
		SCI_TRACE_LOW("get_deltanv_info size is NULL");
		return FALSE;
	}
#ifdef DELTA_NV_PATITION_SUPPORT
	*deltanv_addr = SPIFLASH_GetOperatorDataAddress();
	*p_size = SPIFLASH_GetOperatorDataSize();
#else
	*deltanv_addr = NULL;;
	*p_size = 0;
#endif
		return TRUE;
}

PUBLIC BOOLEAN get_rf_deltanv_info(uint32* deltanv_addr, uint32* p_size)
{
	if (NULL == deltanv_addr) {
		SCI_TRACE_LOW("get_rf_deltanv_info deltanv_addr is NULL");
		return FALSE;
	}

	if (NULL == p_size) {
		SCI_TRACE_LOW("get_rf_deltanv_info size is NULL");
		return FALSE;
	}

	if(delta_nv_buf_info.tail[0] != '#') {
		SCI_ASSERT(0);
	}

	*deltanv_addr = delta_nv_buf_info.nv_config;

	if ((NV_DIFF_EMPETY_FILE_LEN == sizeof(delta_nv_buf_info.nv_config) &&
			((uint16)(-1)) == (*((uint16*)deltanv_addr)))) {
		SCI_TRACE_LOW("get_rf_deltanv_info data ERROR");
		return FALSE;
	}

	*p_size = sizeof(delta_nv_buf_info.nv_config);
	return TRUE;
}


PUBLIC BOOLEAN NV_Force_Change(char* fileName, uint16 version)
{
	uint8* p;
	NV_DIFF_FILE_STRUCT file;
	ITEM_STRUCT item;
	uint32 itemLen;
	uint8* itemBuf;
	//volatile int i=1;
	uint8* offset = 0;
	uint16 file_end_flag = 0;
	BOOLEAN is_need_merge = FALSE;
	uint32 size;
	uint8 *p_delta;

	SCI_TRACE_LOW("NV_Force_Change enter");

	if (NULL == fileName)
	{
		SCI_TRACE_LOW("NV_Force_Change name ERROR");
		return FALSE;
	}

#ifdef DELTA_NV_PATITION_SUPPORT
	get_deltanv_info(&p_delta,&size);
	p = p_delta ;
	if ( ((uint16)(-1)) == (*((uint16*)p)))
	{
		SCI_TRACE_LOW("NV_Force_Change get delta nv mem ERROR");
		return FALSE;
	}
#else
	if(delta_nv_buf_info.tail[0] != '#')
	{
		SCI_ASSERT(0);
	}
	p = p_delta = delta_nv_buf_info.nv_config;
	if ((NV_DIFF_EMPETY_FILE_LEN == sizeof(delta_nv_buf_info.nv_config) && ((uint16)(-1)) == (*((uint16*)p))))
	{
		SCI_TRACE_LOW("NV_Force_Change get delta nv mem ERROR");
		return FALSE;
	}
	size = sizeof(delta_nv_buf_info.nv_config);
#endif

	while(p < (p_delta + size)){
		memset(&file, 0, sizeof(NV_DIFF_FILE_STRUCT));
		strncpy(file.file_name, p, NV_DIFF_FILE_NAME_MAX);
		offset = p+sizeof(char)*NV_DIFF_FILE_NAME_MAX;
		bytes_to_u16(offset, &file.nv_ver_flag);
		offset += sizeof(uint16);
		bytes_to_u32(offset, &file.file_len);

		if(file.file_len > size
			|| ((p + file.file_len) > (p_delta + size))) {
			break;
		}

		if(!strncmp(file.file_name,fileName,NV_DIFF_FILE_NAME_MAX) && file.file_len){
			if(file.nv_ver_flag  <= version){
				SCI_TRACE_LOW("NV_Force_Change no need to change");
				return TRUE;
			}
			p += sizeof(char)*NV_DIFF_FILE_NAME_MAX + sizeof(uint16) + sizeof(uint32);//sizeof(NV_DIFF_FILE_STRUCT)
			is_need_merge = TRUE;
			break;
		}
		else if(strncmp(file.file_name,fileName,NV_DIFF_FILE_NAME_MAX) && file.file_len){
			p+=file.file_len;
		}
		else if(!file.file_len){
			break;
		}
	}
	SCI_TRACE_LOW("NV_Force_Change is_need_merge = %d", is_need_merge);

	if(is_need_merge) {
		bytes_to_u16(p, &file_end_flag);
		while(p < (p_delta + size)
				&& ((uint16)(-1)) != file_end_flag)
		{
			memset(&item, 0, sizeof(ITEM_STRUCT));
			bytes_to_u16(p, &item.item_id);
			p+= sizeof(uint16);
			bytes_to_u16(p, &item.item_offset);
			p+= sizeof(uint16);
			bytes_to_u16(p, &item.value_len);

			if(item.value_len > size
				|| ((p + item.value_len) > (p_delta + size))) {
				SCI_TRACE_LOW("NV_Force_Change match ERROR");
				return FALSE;
			}

			p+= sizeof(uint16);//sizeof(ITEM_STRUCT)-2
			itemLen = NVITEM_GetLength(item.item_id);
			if(!itemLen) {
				p+=item.value_len;
				SCI_TRACE_LOW("NV_Force_Change itemLen==0");
				continue;
			}
			itemBuf = SCI_ALLOC_APP(itemLen);
			SCI_ASSERT(itemBuf);
			if(NVERR_NONE != EFS_NvitemRead(item.item_id, itemLen, (void*)itemBuf)){
				p+=item.value_len;
				SCI_TRACE_LOW("NV_Force_Change item%d read err",item.item_id);
				if(itemBuf != NULL) {
					SCI_FREE(itemBuf);
					itemBuf = NULL;
				}
				continue;
			}
			memcpy(itemBuf+item.item_offset,p,item.value_len);
			if(NVERR_NONE != EFS_NvitemWrite(item.item_id, itemLen, (void*)itemBuf, 0)) {
				SCI_ASSERT(0);
			}
			if(itemBuf != NULL) {
				SCI_FREE(itemBuf);
				itemBuf = NULL;
			}
			p+=item.value_len;
			bytes_to_u16(p, &file_end_flag);
		}
		REFPARAM_SetFileVersion(file.nv_ver_flag, 0);
	}

	return TRUE;
}


PUBLIC uint16 NV_Force_Change_Ex(char* fileName, uint8* deltanv_addr, uint32 deltanv_size, uint16 version)
{
	uint8* p;
	NV_DIFF_FILE_STRUCT file;
	ITEM_STRUCT item;
	uint32 itemLen;
	uint8* itemBuf;
	//volatile int i=1;
	uint8* offset = 0;
	uint16 file_end_flag = 0;
	BOOLEAN is_need_merge = FALSE;
	uint32 size;
	uint8 *p_delta;

	SCI_TRACE_LOW("NV_Force_Change_Ex enter");

	if (NULL == fileName) {
		SCI_TRACE_LOW("NV_Force_Change_Ex name ERROR");
		return 0;
	}

	if (NULL == deltanv_addr)
	{
		SCI_TRACE_LOW("NV_Force_Change_Ex deltanv_addr ERROR");
		return 0;
	}

	if (0 == deltanv_size) {
		SCI_TRACE_LOW("NV_Force_Change_Ex deltanv_size ERROR");
		return 0;
	}

	p = p_delta = deltanv_addr;
	size = deltanv_size;

	while(p < (p_delta + size)) {
		memset(&file, 0, sizeof(NV_DIFF_FILE_STRUCT));
		strncpy(file.file_name, p, NV_DIFF_FILE_NAME_MAX);
		offset = p+sizeof(char)*NV_DIFF_FILE_NAME_MAX;
		bytes_to_u16(offset, &file.nv_ver_flag);
		offset += sizeof(uint16);
		bytes_to_u32(offset, &file.file_len);

		if(file.file_len > size
			|| ((p + file.file_len) > (p_delta + size))) {
			break;
		}

		if(!strncmp(file.file_name,fileName,NV_DIFF_FILE_NAME_MAX) && file.file_len) {
			if(file.nv_ver_flag  <= version) {
				SCI_TRACE_LOW("NV_Force_Change no need to change");
				return 0;
			}
			p += sizeof(char)*NV_DIFF_FILE_NAME_MAX + sizeof(uint16) + sizeof(uint32);//sizeof(NV_DIFF_FILE_STRUCT)
			is_need_merge = TRUE;
			break;
		} else if(strncmp(file.file_name,fileName,NV_DIFF_FILE_NAME_MAX) && file.file_len) {
			p+=file.file_len;
		}
		else if(!file.file_len){
			break;
		}
	}
	SCI_TRACE_LOW("NV_Force_Change is_need_merge = %d", is_need_merge);

	if(is_need_merge) {
		bytes_to_u16(p, &file_end_flag);
		while(p < (p_delta + size)
				&& ((uint16)(-1)) != file_end_flag) {
			memset(&item, 0, sizeof(ITEM_STRUCT));
			bytes_to_u16(p, &item.item_id);
			p+= sizeof(uint16);
			bytes_to_u16(p, &item.item_offset);
			p+= sizeof(uint16);
			bytes_to_u16(p, &item.value_len);

			if(item.value_len > size
				|| ((p + item.value_len) > (p_delta + size))) {
				SCI_TRACE_LOW("NV_Force_Change match ERROR");
				break;
			}

			p+= sizeof(uint16);//sizeof(ITEM_STRUCT)-2
			itemLen = NVITEM_GetLength(item.item_id);
			if(!itemLen) {
				p+=item.value_len;
				SCI_TRACE_LOW("NV_Force_Change itemLen==0");
				continue;
			}
			itemBuf = SCI_ALLOC_APP(itemLen);
			SCI_ASSERT(itemBuf);
			if(NVERR_NONE != EFS_NvitemRead(item.item_id, itemLen, (void*)itemBuf)) {
				p+=item.value_len;
				SCI_TRACE_LOW("NV_Force_Change item%d read err",item.item_id);
				continue;
			}
			memcpy(itemBuf+item.item_offset,p,item.value_len);
			if(NVERR_NONE != EFS_NvitemWrite(item.item_id, itemLen, (void*)itemBuf, 0)) {
				SCI_ASSERT(0);
			}
			SCI_FREE(itemBuf);
			p+=item.value_len;
			bytes_to_u16(p, &file_end_flag);
		}
		return file.nv_ver_flag;
	}

	return 0;
}

PUBLIC void NV_Force_Change_Enter(void)
{
	uint8 rf_version = 0, pa_version = 0;
	uint8 rf_ver_flag = 0, pa_ver_flag = 0;
	char * hw_deltaNV_name = NULL;
	char * pa_deltaNV_name = NULL;
	uint32 deltanv_addr = 0;
	uint32 deltanv_size = 0;

#if 1
	REFPARAM_GetFileVersion(&rf_version, &pa_version);
	get_deltanv_info(&deltanv_addr, &deltanv_size);

	/* board id delta merge */
	hw_deltaNV_name = REFPARAM_GetFileName();
	SCI_TraceLow("hw_deltaNV_name:%s\n",hw_deltaNV_name);
	if (NULL != hw_deltaNV_name) {
		rf_ver_flag = NV_Force_Change_Ex(hw_deltaNV_name, (uint8*)deltanv_addr, deltanv_size, rf_version);
		rf_ver_flag = max(rf_ver_flag, rf_version);
	}

	/* update version flag */
	if (rf_ver_flag > rf_version || pa_ver_flag > pa_version)
	{
		REFPARAM_SetFileVersion(rf_ver_flag, pa_ver_flag);
	}
#endif

#ifdef FORCECHANGE_SUPPORT
	REFPARAM_GetFileVersion(&rf_version, &pa_version);
	get_deltanv_info(&deltanv_addr, &deltanv_size);

	/* board id delta merge */
	hw_deltaNV_name = REFPARAM_GetFileName();
	SCI_TraceLow("hw_deltaNV_name:%s\n",hw_deltaNV_name);
	if (NULL != hw_deltaNV_name) {
		rf_ver_flag = NV_Force_Change_Ex(hw_deltaNV_name, (uint8*)deltanv_addr, deltanv_size, rf_version);
		rf_ver_flag = max(rf_ver_flag, rf_version);
	}
#if !defined(PLATFORM_UWS6121E)
	/* pa delta merge */
	pa_deltaNV_name = REFPARAM_GetPAFileName();
	if (NULL != pa_deltaNV_name) {
		pa_ver_flag = NV_Force_Change_Ex(pa_deltaNV_name, (uint8*)deltanv_addr, deltanv_size, pa_version);
		pa_ver_flag = max(pa_ver_flag, pa_version);
	}
#endif

	/* update version flag */
	if (rf_ver_flag > rf_version || pa_ver_flag > pa_version)
	{
		REFPARAM_SetFileVersion(rf_ver_flag, pa_ver_flag);
	}
#endif
}

/**----------------------------------------------------------------------------*
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
