#include "abup_typedef.h"
#include "abup_bl_main.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#include "abup_define.h"
#include "abup_flash.h"
#include "abup_mcu.h"

#define ABUP_BL_MAX_DEBUG_BUF_LEN		  512
#define ABUP_NV_LOGIC_ADRESS              0x90000001
#define ABUP_PS_LOGIC_ADRESS              0x80000003
#define ABUP_CP_LOGIC_ADRESS              0x90000101
#define ABUP_DELTA_NV_LOGIC_ADRESS        0x90000100
#define ABUP_RES_LOGIC_ADRESS             0x90000004

#define UPI_WORKING_BUFFER_LEN         (3*1024*1024)

#pragma arm section zidata = "ZIDATA_AREA_5"
static abup_uint32 s_abup_working_buf[UPI_WORKING_BUFFER_LEN>>2];
#pragma arm section zidata 

abup_uint8 abup_delta_num=0;
abup_uint8 abup_curr_delta=1;
multi_head_info_patch PackageInfo;
abup_char debug_patch_session[50]={0};
abup_char debug_patch_session1[50]={0};
abup_char patch_mcu_result[50]={0};
extern const NOR_FLASH_CONFIG_T s_platform_patitiion_config;

abup_extern int fotapatch_procedure(void);
abup_extern ABUP_BOOL abup_IsPackgeFound(void);
abup_extern ABUP_BOOL abup_IsPartPackge(void);
abup_extern void abup_get_package_number(abup_uint8 *delta_num);
abup_extern abup_int32 abup_get_package_info(multi_head_info_patch *info,abup_uint8 curr_delta);

abup_extern void frame_memory_finalize(void);
abup_extern void init_pos_array(void);
abup_extern ABUP_BOOL abup_init_mem_pool(abup_uint8* buffer, abup_uint32 len);
abup_extern void abup_bl_erase_running_nv(void);

abup_bl_ratio_callback s_abup_ratio_cb;

static int32 s_abup_bl_debug_switch = 1;
char s_abup_out_buf[ABUP_BL_MAX_DEBUG_BUF_LEN] = {0};

ABUP_BOOL abup_malloc_work_buff(void)
{
    return ABUP_TRUE;
}

void abup_free_work_buff(void)
{
}

void abup_bl_delayms(uint32 ms)
{
	uint32 end = SCI_GetTickCount() + ms;
	
	while(SCI_GetTickCount() < end)
	{
	}

}

ABUP_BOOL AbupUseFlash(void)
{
#ifdef ABUP_FOTA_SUPPORT_TCARD
	return ABUP_FALSE;
#else
	return ABUP_TRUE;
#endif
}

int32 abup_bl_debug_switch(int32 nOn)
{
	s_abup_bl_debug_switch = nOn;

	return 0;
}

void abup_bl_debug_print(void* ctx, const abup_char* fmt, ...) 
{
	if(s_abup_bl_debug_switch) 
	{
		int32 len = 0;
		va_list ap;
		va_start(ap, fmt);

		memset(s_abup_out_buf, 0, ABUP_BL_MAX_DEBUG_BUF_LEN);
		len = vsnprintf(s_abup_out_buf,ABUP_BL_MAX_DEBUG_BUF_LEN, fmt, ap);

		rprintf("%s\n", s_abup_out_buf);

		va_end(ap);
		//abup_bl_delayms(20);
	}
}

void abup_patch_prepare(abup_uint16 ratio)
{
	abup_bl_debug_print((void *)ABUP_LOG_LOOP, "+FOTA: patch chk: %d",ratio);
}

void abup_patch_ratio(abup_uint16 ratio)
{
	abup_bl_debug_print((void *)ABUP_LOG_LOOP, "+FOTA: patch ratio: %d",ratio);
    if(s_abup_ratio_cb)
    {
        s_abup_ratio_cb(ratio / abup_delta_num + (abup_curr_delta - 1) * 100 / abup_delta_num);
    }
}

void abup_bl_print_bin(uint8 *content, uint16 size)
{
	uint16 idx=0;
	if(content)
	{
		for(idx=0;idx<size;idx++)
			abup_bl_debug_print(NULL,"A[%d] = %02X", idx, *(content+idx));
	}
}

abup_uint32 abup_bl_get_curr_write_address(void)
{
    return 0;	
}

abup_uint8* abup_bl_get_working_buffer(void)
{
	return (abup_uint8 *)s_abup_working_buf; 
}

abup_uint64 abup_bl_get_working_buffer_len(void)
{
	return (abup_uint64)UPI_WORKING_BUFFER_LEN;
}

abup_uint32 abup_bl_get_app_base(void)
{
	if (PackageInfo.multi_bin_address == ABUP_PS_LOGIC_ADRESS)
	{
		return s_platform_patitiion_config.ps_addr;
	}
	else if (PackageInfo.multi_bin_address == ABUP_CP_LOGIC_ADRESS)
	{
		return s_platform_patitiion_config.cp_addr;
	}
	else if (PackageInfo.multi_bin_address == ABUP_RES_LOGIC_ADRESS)
	{
		return s_platform_patitiion_config.mmi_res;
	}
	else if (PackageInfo.multi_bin_address == ABUP_NV_LOGIC_ADRESS)
	{
		return s_platform_patitiion_config.fixnv_addr;
	}
	else if (PackageInfo.multi_bin_address == ABUP_DELTA_NV_LOGIC_ADRESS)
	{
		return s_platform_patitiion_config.operator_data_addr;
	}

	return 0;
}

abup_uint32 abup_bl_get_delta_base(void)
{
	return PackageInfo.multi_bin_offset;
}

abup_uint32 abup_bl_get_diff_param_size(void)
{
	 abup_uint32 block_size=abup_bl_flash_block_size();
	return block_size;
}

void abup_bl_DL_InitLCD()
{	
}

void abup_bl_ShowUpateSuccess(void)
{
}

void abup_bl_ShowUpdateError(abup_int16 status)
{
}

void abup_save_update_result(abup_char* result_str,abup_char* mcu_id,abup_uint8 result)
{
	abup_char temp_str[10]={0};
	sprintf(temp_str,"%s=%d",mcu_id,result);
	if(strlen(result_str)==0)
		strcpy(result_str,temp_str);
	else
		strcat(result_str,temp_str);
}

void abup_remove_all(void)
{
}

int abup_get_full_bin_size(void)
{
	char CharByte[4] = {0}, cycle = 0;
	int BinSize = 0, shift = 0, len = 0;
	abup_int32 temp_file, result;

    if(AbupUseFlash())
    {
        abup_bl_read_flash(6 + abup_bl_get_delta_base(), CharByte, 4);
    }
    else
    {         
        temp_file = abup_Open_File(abup_get_delta_file_path(), ABUP_READONLY);
        if(temp_file != NULL)
        {
            abup_Seek_File(temp_file, 6 + abup_bl_get_delta_base(), ABUP_SEEK_START);
            result=abup_Read_File(temp_file, CharByte, 4, &len);
            abup_Close_File(temp_file);
        }
    }

	for (cycle = 0;cycle < 4; cycle++)
	{
		shift = cycle*8;
		BinSize += (CharByte[cycle]&0xFF)<<shift;
	}

	abup_bl_debug_print(NULL, "abup_get_full_bin_size FirewareSize= %d", BinSize);
	return BinSize;
}

int abup_iot_full_patch(void)
{
	abup_uint32 physical_address = 0;
	abup_uint32 full_size = 0, len =0;
	abup_uint32 i = 0, block_num = 0;
	abup_uint8 *rbuf = NULL;
	abup_int32 temp_file, result;
    
	physical_address = abup_bl_get_app_base();
	full_size = abup_get_full_bin_size();
	rbuf = abup_bl_get_working_buffer();

	if (PackageInfo.multi_bin_address == ABUP_NV_LOGIC_ADRESS)
	{
        if(AbupUseFlash())
        {
    		abup_bl_read_flash(64 + abup_bl_get_delta_base(), rbuf, full_size);
        }
        else
        {         
            temp_file = abup_Open_File(abup_get_delta_file_path(), ABUP_READONLY);
            if(temp_file != NULL)
            {
                abup_Seek_File(temp_file, 64 + abup_bl_get_delta_base(), ABUP_SEEK_START);
                result=abup_Read_File(temp_file, rbuf, full_size, &len);
                abup_Close_File(temp_file);
            }
        }
		TF_Prepare_Slave_image((char *)rbuf, full_size);
		if (TF_Update_Fixnv((char *)rbuf, full_size) == 0)
		{
            abup_patch_ratio(100);
			return 0;
		}
		return ABUP_FUDIFFNET_ERROR_FULL_UPDATE_FAIL;
	}
	else if (PackageInfo.multi_bin_address == ABUP_DELTA_NV_LOGIC_ADRESS)
	{
	}
	else
	{
		return ABUP_FUDIFFNET_ERROR_FULL_UPDATE_FAIL;
	}

	if (full_size % abup_bl_flash_block_size() == 0)
	{
		block_num = full_size / abup_bl_flash_block_size();
	}
	else
	{
		block_num = full_size / abup_bl_flash_block_size() + 1;
	}

	for (i = 0; i < block_num; i++)
	{
		if (i == block_num - 1)
		{
			abup_bl_read_flash(64 + abup_bl_get_delta_base() + i * abup_bl_flash_block_size(), rbuf, full_size % abup_bl_flash_block_size());
			abup_bl_write_block(rbuf, physical_address + i * abup_bl_flash_block_size(), full_size % abup_bl_flash_block_size());
		}
		else
		{
			abup_bl_read_flash(64 + abup_bl_get_delta_base() + i * abup_bl_flash_block_size(), rbuf, abup_bl_flash_block_size());
			abup_bl_write_block(rbuf, physical_address + i * abup_bl_flash_block_size(), abup_bl_flash_block_size());
		}
        abup_patch_ratio((i + 1) * 100 / block_num);
	}

	return 0;
}

ABUP_BOOL abup_IsPartPackge(void)
{
	abup_uint8 data[4];
	abup_bl_debug_print(ABUP_LOG_DEBUG, "abup_IsPartPackge");
	memset(data, 0, sizeof(data));
	abup_bl_read_backup_region(data, sizeof(data));
	abup_bl_debug_print(ABUP_LOG_DEBUG, "%c%c%c%c", data[0], data[1], data[2], data[3]);
	if (data[0] == 'A' && data[1] == 'B' && data[2] == 'U' && data[3] == 'P')
	{
		abup_bl_debug_print(ABUP_LOG_DEBUG, "abup_IsPartPackge: YES!");
		return ABUP_TRUE;
	}
	abup_bl_debug_print(ABUP_LOG_DEBUG, "abup_IsPartPackge: NO!");
	return ABUP_FALSE;
}

static void abup_bl_get_flash_partation(void)
{
	abup_uint8 rdest[8] = {0};

	abup_bl_debug_print(NULL, "bank_num=0x%x, sect_num=0x%x", s_platform_patitiion_config.bank_num, s_platform_patitiion_config.sect_num);
	abup_bl_debug_print(NULL, "file_sect_num=%d, sect_size=0x%x", s_platform_patitiion_config.file_sect_num, s_platform_patitiion_config.sect_size);
	abup_bl_debug_print(NULL, "start_addr=0x%x, efs_start_addr=0x%x", s_platform_patitiion_config.start_addr, s_platform_patitiion_config.efs_start_addr);
	abup_bl_debug_print(NULL, "flash_size=0x%x, fixnv_addr=0x%x", s_platform_patitiion_config.flash_size, s_platform_patitiion_config.fixnv_addr);
	abup_bl_debug_print(NULL, "prodinfo_addr=0x%x, mmi_res=0x%x", s_platform_patitiion_config.prodinfo_addr, s_platform_patitiion_config.mmi_res);
	abup_bl_debug_print(NULL, "mmi_res_size=0x%x, spload_addr=0x%x", s_platform_patitiion_config.mmi_res_size, s_platform_patitiion_config.umem_addr);
	abup_bl_debug_print(NULL, "umem_size=0x%x, sect_num=0x%x", s_platform_patitiion_config.umem_size, s_platform_patitiion_config.spload_addr);
	abup_bl_debug_print(NULL, "simlock_addr=0x%x, ps_addr=0x%x", s_platform_patitiion_config.simlock_addr, s_platform_patitiion_config.ps_addr);
	abup_bl_debug_print(NULL, "operator_data_addr=0x%x, operator_data_size=0x%x", s_platform_patitiion_config.operator_data_addr, s_platform_patitiion_config.operator_data_size);
	abup_bl_debug_print(NULL, "cp_addr=0x%x, cp_size=0x%x", s_platform_patitiion_config.cp_addr, s_platform_patitiion_config.cp_size);
	abup_bl_debug_print(NULL, "config_nv_addr=0x%x, config_nv_size=0x%x", s_platform_patitiion_config.config_nv_addr, s_platform_patitiion_config.config_nv_size);
	abup_bl_debug_print(NULL, "crystal_aging_addr=0x%x, crystal_aging_size=0x%x", s_platform_patitiion_config.crystal_aging_addr, s_platform_patitiion_config.crystal_aging_size);
	abup_bl_debug_print(NULL, "ims_toggel_cfg_addr=0x%x, ims_toggel_cfg_size=0x%x", s_platform_patitiion_config.ims_toggel_cfg_addr, s_platform_patitiion_config.ims_toggel_cfg_size);
	abup_bl_debug_print(NULL, "abup_fota_app_addr=0x%x, abup_fota_app_size=0x%x", s_platform_patitiion_config.abup_fota_app_addr, s_platform_patitiion_config.abup_fota_app_size);
}

abup_uint8 AbupProcedure(abup_bl_ratio_callback cb)
{
	abup_int16 status = ABUP_FUDIFFNET_ERROR_NONE;
	abup_uint8 i;
	abup_int8 mcu_flag=-1;
	abup_uint8 updata_flag[4] = "abup";

	abup_bl_write_block(updata_flag, abup_bl_flag_base(), 4);
	updata_flag[0] = 0;

	abup_bl_debug_switch(1);
	abup_bl_debug_print(NULL, "AUDPSProcedure 4.0");

	abup_bl_get_flash_partation();
    
	if(abup_fat_init() != E_ABUP_SUCCESS)
    {
		abup_bl_write_block(updata_flag, abup_bl_flag_base(), 4);
		abup_bl_debug_print(NULL, "abup_fat_init: failed!");
        return 0xFF;
    }   
    
	memset(patch_mcu_result,0,ADUPS_FOTA_SDK_MAX_MCU_NUM*7);

	if(abup_IsPartPackge())
	{
		abup_bl_write_block(updata_flag, abup_bl_flag_base(), 4);
		return 0xFF;
	}

	if (abup_IsPackgeFound() != 1)
	{
		status=ABUP_FUDIFFNET_ERROR_UPDATE_ERROR_END;
		abup_bl_debug_print(NULL, "abup_IsPackgeFound: not found");
		abup_bl_write_block(updata_flag, abup_bl_flag_base(), 4);
		return 0;
	}
	else
	{
		abup_bl_debug_print(NULL, "abup_IsPackgeFound: found");
	}

	abup_get_package_number(&abup_delta_num);

	abup_bl_debug_print(NULL, "package_number = %d",abup_delta_num);

	if ((abup_delta_num > ABUP_MAX_DELTA_NUM) || (abup_delta_num == 0))
	{
		abup_bl_write_block(updata_flag, abup_bl_flag_base(), 4);
		return 0;
	}

	/*Init memory buffer*/   
	if (NULL == abup_bl_get_working_buffer()) 
	{
		abup_bl_debug_print(NULL, "Can not alloc UPI working buffer");
		status=ABUP_FUDIFFNET_ERROR_INSUFFICIENT_WORKING_BUF;
	}

	//WacthDogRestart();
	abup_bl_debug_print(NULL, "working buf addr: 0x%x", abup_bl_get_working_buffer()); 
    
    s_abup_ratio_cb = cb;

	if(status == ABUP_FUDIFFNET_ERROR_NONE)
	{
		while(abup_curr_delta <= abup_delta_num)
		{
			abup_patch_ratio(0);
			memset(&PackageInfo,0,sizeof(multi_head_info_patch));
			abup_bl_debug_print((void *)ABUP_LOG_DELTA, "abup_curr_delta = %d",abup_curr_delta);
			status=abup_get_package_info(&PackageInfo,abup_curr_delta);
			if(status!=ABUP_FUDIFFNET_ERROR_NONE)
				break;
			abup_bl_debug_print(NULL, "PackageInfo add=%x,m=%d,offset=%d",PackageInfo.multi_bin_address,PackageInfo.multi_bin_method,PackageInfo.multi_bin_offset);

			if(PackageInfo.multi_bin_method==1)
			{
				abup_bl_debug_print(NULL, "full patch");
				//status = abup_iot_full_patch();
				if (status == 0)
				{
					//abup_bl_erase_running_nv();
				}
			}
			else
			{
				abup_bl_debug_print(NULL, "diff patch");
				status = fotapatch_procedure();
			}

			if (status != ABUP_FUDIFFNET_ERROR_NONE && status != ABUP_MCU_PATCH_FAIL)
			{
				abup_bl_debug_print(NULL, "patch fail");
				break;
			}
			else
			{
				if(status==ABUP_MCU_PATCH_FAIL)
				{
					abup_bl_debug_print(NULL, "mcu patch fail"); 
					//abup_save_update_result(patch_mcu_result,abup_patch_mcu[i].mcu_id,99);
					abup_curr_delta++;
					mcu_flag=-1;
				}
				else
				{
					abup_bl_debug_print(NULL, "patch sucess"); 
					abup_curr_delta++;
					mcu_flag=-1;
				}
			}
		}
	}

	if(status == ABUP_FUDIFFNET_ERROR_NONE)
	{
		abup_bl_ShowUpateSuccess();
        if(AbupUseFlash())
        {
            abup_bl_erase_delata();
        }
        else
        {
            abup_bl_remove_delata();
        }
		abup_bl_erase_backup_region();
		abup_bl_write_block(updata_flag, abup_bl_flag_base(), 4);

		abup_bl_debug_print(NULL, "[FUDIFFNET] Update done, reboot now ...");
		return 0;
	}
	else
	{
		abup_bl_ShowUpdateError(status);
        if(AbupUseFlash())
        {
            abup_bl_erase_delata();
        }
        else
        {
            abup_bl_remove_delata();
        }
		abup_bl_erase_backup_region();
		abup_bl_write_block(updata_flag, abup_bl_flag_base(), 4);
		if(strlen(patch_mcu_result)!=0)
			abup_bl_write_backup_region(patch_mcu_result,strlen(patch_mcu_result));

		abup_bl_debug_print(NULL, "***Something wrong during update, status=%d", status);
	}

	return 0xFF;
}

