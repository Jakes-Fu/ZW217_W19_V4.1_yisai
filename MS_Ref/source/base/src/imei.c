/******************************************************************************
 ** File Name:      imei.c                                                     					*
 ** Author:         Jiayong.Yang                                                   *
 ** DATE:           08/28/2009                                                *
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
                    implement the fixnv protection     *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 08/28/2009     Jiayong.Yang          Create.                                   *
 ******************************************************************************/
#include "run_mode.h" 
#include "os_api.h"
#include "nv_type.h"
#include "nvitem.h"
#include "arm_reg.h"
#include "cmddef.h"
#include "tb_comm.h"
#include "tb_hal.h"
#include "sio.h"
#include "vcom_usb.h"
#include "driver_export.h"
#include "power.h"
#include "sio.h"
#include "dal_chr.h"
#include "mn_type.h"        /* MN_MAX_IMEI_LENGTH     */
#include "bt_abs.h"         /* struct _BT_ADDRESS     */
#include "bt_cfg.h"         /* struct _BT_NV_PARAM    */
#include "gps_drv.h"        /* struct gps_nv_info_tag */
#include "production_test.h"
#include "tb_flash.h"
#include "imei.h"
#include "wifi_drv.h"
#include "ref_param.h"
#include "prod_param.h"
#include "layer1_engineering.h"
#ifdef IMEI_OTP_SUPPORT_EFUSE
    #include "efuse_drvapi.h"
#endif
#define	_ENABLE_CRC16_
/*lint -save -e762 */	

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    extern   "C" 
    {
#endif
/**---------------------------------------------------------------------------*
 **                         Struct Define
 **---------------------------------------------------------------------------*/
//

#ifdef NANDBOOT_SUPPORT
#define	IMEI_UpdateProductInfo_Protect	NVITEM_UpdateProductInfo
#define	IMEI_GetProductInfo_Protect		NVITEM_GetProductInfo
#else
#define	IMEI_UpdateProductInfo_Protect	NVITEM_UpdateProductInfo_Protect
#define	IMEI_GetProductInfo_Protect		NVITEM_GetProductInfo_Protect
#endif

PUBLIC uint32 DIAG_need_SCM_sleep = SCI_FALSE;

extern uint16 crc16(uint16 crc, uint8 const *buffer, uint32 len);
#ifndef NANDBOOT_SUPPORT
extern void *GetFixedNvitemAddr(uint identifier);
#endif
extern uint32 FLASH_GetProductInfoAddr(void);
extern const char * COMMON_GetProjectVersionInfo(void);

LOCAL uint8  *g_nv_org_ptr = NULL;
LOCAL uint32	need_update_finxv_sector=0;


LOCAL BOOLEAN update_nv_imei(uint8 *data,uint32 length);
LOCAL BOOLEAN update_nv_imei1(uint8 *data,uint32 length);
LOCAL BOOLEAN update_nv_GPS(uint8 *data,uint32 length);
LOCAL BOOLEAN update_nv_BT(uint8 *data,uint32 length);
LOCAL BOOLEAN update_nv_imei2(uint8 *data,uint32 length);
LOCAL BOOLEAN update_nv_imei3(uint8 *data,uint32 length);
LOCAL BOOLEAN update_nv_SN(uint8 *data,uint32 length);
LOCAL BOOLEAN update_nv_WIFI(uint8 *data,uint32 length);
LOCAL BOOLEAN update_product_info(uint8 *data,uint32 length);

LOCAL uint32 read_nv_imei(uint8 *data,uint32 max_data_len);
LOCAL uint32 read_nv_imei1(uint8 *data,uint32 max_data_len);
LOCAL uint32 read_nv_GPS(uint8 *data,uint32 max_data_len);
LOCAL uint32 read_nv_BT(uint8 *data,uint32 max_data_len);
LOCAL uint32 read_nv_imei2(uint8 *data,uint32 max_data_len);
LOCAL uint32 read_nv_imei3(uint8 *data,uint32 max_data_len);
LOCAL uint32 read_nv_SN(uint8 *data,uint32 max_data_len);
LOCAL uint32 read_nv_WIFI(uint8 *data,uint32 max_data_len);
LOCAL uint32 read_product_info(uint8 *data,uint32 length);
LOCAL uint32 read_software_version(uint8 *data,uint32 max_data_len);
LOCAL void send_request_ack(BOOLEAN result,MSG_HEAD_T *msg_ptr);
LOCAL uint32 read_nv_imei_num(uint8 *data,uint32 max_data_len);

LOCAL	IMEI_TOOL_E	call_back_list[]=
{
	{update_nv_imei,read_nv_imei},
	{update_nv_imei1,read_nv_imei1},
	{update_nv_BT,read_nv_BT},
	{update_nv_GPS,read_nv_GPS},	
	{update_nv_imei2,read_nv_imei2},
	{update_nv_imei3,read_nv_imei3}, // will be repaced to do IMEI4 
	{update_nv_WIFI,read_nv_WIFI},
	{update_product_info,read_product_info},
	{NULL,read_software_version},
	{NULL,read_nv_imei_num}
};
//uint32 Start_WriteFlash_Time = 0;
LOCAL uint32 End_WriteFlash_Time = 0;
LOCAL uint32 RecvCommand_Time=0,Send_CommandAck_Time=0;
#ifdef  __IMEI_DEBUG_ENABLE__
LOCAL MSG_HEAD_T	*g_msg_ptr = NULL;
LOCAL uint8	debug_msg_buffer[256];
LOCAL void Send_Debug_ACK(uint16 debug_value)
{

	if(g_msg_ptr == NULL)
	{
		debug_msg_buffer[0] = 0;
		debug_msg_buffer[1] = 0;
		debug_msg_buffer[2] = 0;
		debug_msg_buffer[3] = 0;
		debug_msg_buffer[4] = 0;
		debug_msg_buffer[5] = 0x0c;
		debug_msg_buffer[6] = 0x5f;
		debug_msg_buffer[7] = 0;
		g_msg_ptr = (MSG_HEAD_T *)debug_msg_buffer;
	}
	g_msg_ptr->len = sizeof(MSG_HEAD_T)+ sizeof(uint16)*2;
	*(uint16 *)(g_msg_ptr+1) =debug_value;
	g_msg_ptr->type = 0xAA;
	send_request_ack(SCI_FALSE,g_msg_ptr);
	SCI_Sleep(500);

}

#endif
LOCAL BOOLEAN update_nv_imei(uint8 *data,uint32 length)
{
	uint8              *nv_org_ptr          = NULL;
	BOOLEAN		ret=SCI_FALSE;
	
	nv_org_ptr = g_nv_org_ptr;

	SCI_ASSERT(length == MN_MAX_IMEI_LENGTH);/*assert verified*/
    #ifdef IMEI_OTP_SUPPORT_EFUSE
        IMEI_EfuseWriteTempBuf(NV_IMEI, data, MN_MAX_IMEI_LENGTH);
		need_update_finxv_sector = 1;
		ret = SCI_TRUE;
    #else    
    	
    	if(NVITEM_UpdateCaliBuf(NV_IMEI, MN_MAX_IMEI_LENGTH, data, nv_org_ptr)==NVERR_NONE)
    	{
    		need_update_finxv_sector = 1;
    		ret = SCI_TRUE;
    	}
    #endif
	return ret;
}
LOCAL uint32 read_nv_imei(uint8 *data,uint32 max_data_len)
{
	uint8               * nv_org_ptr          = NULL;
	uint32		ret=0;
	
	nv_org_ptr = g_nv_org_ptr;
	SCI_ASSERT(max_data_len >= MN_MAX_IMEI_LENGTH);/*assert verified*/
    #ifdef IMEI_OTP_SUPPORT_EFUSE
        IMEI_EfuseRead(NV_IMEI, data, MN_MAX_IMEI_LENGTH);
		ret = MN_MAX_IMEI_LENGTH;
    #else    
	
    	if(NVITEM_ReadCaliBuf(NV_IMEI, MN_MAX_IMEI_LENGTH, data, nv_org_ptr)==NVERR_NONE)
    	{
    		ret = MN_MAX_IMEI_LENGTH;
    	}
    #endif	
	return ret;
}
LOCAL BOOLEAN update_nv_imei1(uint8 *data,uint32 length)
{
	uint8              *nv_org_ptr          = NULL;
	BOOLEAN		ret=SCI_FALSE;
	
	nv_org_ptr = g_nv_org_ptr;
	SCI_ASSERT(length == MN_MAX_IMEI_LENGTH);/*assert verified*/
    #ifdef IMEI_OTP_SUPPORT_EFUSE
        IMEI_EfuseWriteTempBuf(NV_IMEI1, data, MN_MAX_IMEI_LENGTH);
		need_update_finxv_sector = 1;
		ret = SCI_TRUE;
    #else    
	
    	if(NVITEM_UpdateCaliBuf(NV_IMEI1, MN_MAX_IMEI_LENGTH, data, nv_org_ptr)==NVERR_NONE)
    	{
    		need_update_finxv_sector = 1;
    		ret = SCI_TRUE;
    	}
    #endif
	return ret;
}
LOCAL uint32 read_nv_imei1(uint8 *data,uint32 max_data_len)
{
	uint8              *nv_org_ptr          = NULL;
	uint32		ret=0;
	
	nv_org_ptr = g_nv_org_ptr;

	SCI_ASSERT(max_data_len >= MN_MAX_IMEI_LENGTH);/*assert verified*/
    #ifdef IMEI_OTP_SUPPORT_EFUSE
        IMEI_EfuseRead(NV_IMEI1, data, MN_MAX_IMEI_LENGTH);
		ret = MN_MAX_IMEI_LENGTH;
    #else    

    	if(NVITEM_ReadCaliBuf(NV_IMEI1, MN_MAX_IMEI_LENGTH, data, nv_org_ptr)==NVERR_NONE)
    	{
    		ret = MN_MAX_IMEI_LENGTH;
    	}
    #endif	
	return ret;
}
LOCAL BOOLEAN update_nv_imei2(uint8 *data,uint32 length)
{
	uint8              *nv_org_ptr          = NULL;
	BOOLEAN		ret=SCI_FALSE;
	
	nv_org_ptr = g_nv_org_ptr;
	SCI_ASSERT(length == MN_MAX_IMEI_LENGTH);/*assert verified*/
	if(NVITEM_UpdateCaliBuf(NV_IMEI2, MN_MAX_IMEI_LENGTH, data, nv_org_ptr)==NVERR_NONE)
	{
		need_update_finxv_sector = 1;
		ret = SCI_TRUE;
	}

	return ret;
}
LOCAL uint32 read_nv_imei2(uint8 *data,uint32 max_data_len)
{
	uint8              *nv_org_ptr          = NULL;
	uint32		ret=0;
	
	nv_org_ptr = g_nv_org_ptr;

	SCI_ASSERT(max_data_len >= MN_MAX_IMEI_LENGTH);/*assert verified*/
	if(NVITEM_ReadCaliBuf(NV_IMEI2, MN_MAX_IMEI_LENGTH, data, nv_org_ptr)==NVERR_NONE)
	{
		ret = MN_MAX_IMEI_LENGTH;
	}
	return ret;
}

LOCAL BOOLEAN update_nv_imei3(uint8 *data,uint32 length)
{
	uint8              *nv_org_ptr          = NULL;
	BOOLEAN		ret=SCI_FALSE;
	
	nv_org_ptr = g_nv_org_ptr;

	SCI_ASSERT(length == MN_MAX_IMEI_LENGTH);/*assert verified*/
	if(NVITEM_UpdateCaliBuf(NV_IMEI3, MN_MAX_IMEI_LENGTH, data, nv_org_ptr)==NVERR_NONE)
	{
		need_update_finxv_sector = 1;
		ret = SCI_TRUE;
	}

	return ret;
}
LOCAL uint32 read_nv_imei3(uint8 *data,uint32 max_data_len)
{
	uint8               * nv_org_ptr          = NULL;
	uint32		ret=0;
	
	nv_org_ptr = g_nv_org_ptr;
	SCI_ASSERT(max_data_len >= MN_MAX_IMEI_LENGTH);/*assert verified*/
	if(NVITEM_ReadCaliBuf(NV_IMEI3, MN_MAX_IMEI_LENGTH, data, nv_org_ptr)==NVERR_NONE)
	{
		ret = MN_MAX_IMEI_LENGTH;
	}
	return ret;
}

LOCAL uint32 read_nv_imei_num(uint8 *data,uint32 max_data_len)
{
#ifndef NANDBOOT_SUPPORT
	uint16 *item_addr_ptr = PNULL;
	volatile uint16 *flash_ptr = PNULL;
#endif
	uint16  item_len = 0;
	uint8   imei_cnt=0x0;
	
#ifdef NANDBOOT_SUPPORT
    item_len = NVITEM_FixedNV_GetLength(NV_IMEI);
    if(0 != item_len)
   	{
        imei_cnt++;
   	}
	item_len = NVITEM_FixedNV_GetLength(NV_IMEI1);
    if(0 != item_len)
   	{
        imei_cnt++;
   	}
    item_len = NVITEM_FixedNV_GetLength(NV_IMEI2);
    if(0 != item_len)
   	{
        imei_cnt++;
   	}
	item_len = NVITEM_FixedNV_GetLength(NV_IMEI3);
    if(0 != item_len)
   	{
        imei_cnt++;
   	}
#else
	item_addr_ptr = (uint16 *)((uint32)GetFixedNvitemAddr(NV_IMEI) - 4);
	if(item_addr_ptr != NULL)
	{
		flash_ptr = (volatile uint16 *)(item_addr_ptr);
		flash_ptr++;
		item_len = *flash_ptr;
		if(item_len == MN_MAX_IMEI_LENGTH)
		{
			imei_cnt++;
		}
	}
	item_addr_ptr = (uint16 *)((uint32)GetFixedNvitemAddr(NV_IMEI1) - 4);
	if(item_addr_ptr != NULL)
	{
		flash_ptr = (volatile uint16 *)(item_addr_ptr);
		flash_ptr++;
		item_len = *flash_ptr;
		if(item_len == MN_MAX_IMEI_LENGTH)
		{
			imei_cnt++;
		}
	}
	item_addr_ptr = (uint16 *)((uint32)GetFixedNvitemAddr(NV_IMEI2)-4);
	if(item_addr_ptr != NULL)
	{
		flash_ptr = (volatile uint16 *)(item_addr_ptr);
		flash_ptr++;
		item_len = *flash_ptr;
		if(item_len == MN_MAX_IMEI_LENGTH)
		{
			imei_cnt++;
		}
	}
	item_addr_ptr = (uint16 *)((uint32)GetFixedNvitemAddr(NV_IMEI3)-4);
	if(item_addr_ptr != NULL)
	{
		flash_ptr = (volatile uint16 *)(item_addr_ptr);
		flash_ptr++;
		item_len = *flash_ptr;
		if(item_len == MN_MAX_IMEI_LENGTH)
		{
			imei_cnt++;
		}
	}
#endif	
	*data++ = imei_cnt;
	SCI_MEMSET(data, 0x0,max_data_len-1);

	return 1;

}

LOCAL BOOLEAN update_nv_GPS(uint8 *data,uint32 length)
{
	uint8              *nv_org_ptr          = NULL;
	BOOLEAN		ret=SCI_FALSE;

	nv_org_ptr = g_nv_org_ptr;
	SCI_ASSERT(length == sizeof(GPS_NV_INFO_T));/*assert verified*/
	if(NVITEM_UpdateCaliBuf(NV_GPS_PARAM, sizeof(GPS_NV_INFO_T) ,data, nv_org_ptr) == NVERR_NONE)
	{
		need_update_finxv_sector = 1;
		ret = SCI_TRUE;
	}

	return ret;
}
LOCAL uint32 read_nv_GPS(uint8 *data,uint32 max_data_len)
{
	uint8              *nv_org_ptr          = NULL;
	uint32		ret=0;
	
	nv_org_ptr = g_nv_org_ptr;
	SCI_ASSERT(max_data_len >= sizeof(GPS_NV_INFO_T));/*assert verified*/
	if(NVITEM_ReadCaliBuf(NV_GPS_PARAM, sizeof(GPS_NV_INFO_T), data, nv_org_ptr) == NVERR_NONE)
	{
		ret = sizeof(GPS_NV_INFO_T);
	}

	return ret;
}
LOCAL BOOLEAN update_nv_BT(uint8 *data,uint32 length)
{
	uint8              *nv_org_ptr          = NULL;
	BT_NV_PARAM   nv_param;
	BOOLEAN		ret=SCI_FALSE;
	
	nv_org_ptr = g_nv_org_ptr;

	SCI_ASSERT(length == sizeof(BT_ADDRESS));/*assert verified*/
	EFS_NvitemRead(NV_BT_CONFIG, sizeof(BT_NV_PARAM), (uint8*)&nv_param);
	SCI_MEMCPY((uint8*)&(nv_param.bd_addr),  data,  sizeof(BT_ADDRESS));
    DIAG_need_SCM_sleep = SCI_TRUE;
	if(NVITEM_UpdateCaliBuf(NV_BT_CONFIG,  sizeof(BT_NV_PARAM),  &nv_param,  nv_org_ptr) == NVERR_NONE)
	{
		need_update_finxv_sector = 1;
		ret = SCI_TRUE;
	}

    DIAG_need_SCM_sleep = SCI_FALSE;
	return ret;
}
LOCAL uint32 read_nv_BT(uint8 *data,uint32 max_data_len)
{
	BT_NV_PARAM    nv_param = {0};
	uint8              *nv_org_ptr          = NULL;
	uint32		ret=0;
	
	nv_org_ptr = g_nv_org_ptr;
    SCI_ASSERT(max_data_len >= sizeof(BT_ADDRESS));/*assert verified*/
	if(NVITEM_ReadCaliBuf(NV_BT_CONFIG, sizeof(BT_NV_PARAM), (uint8*)&nv_param, nv_org_ptr)==NVERR_NONE)
	{
    		ret =sizeof(BT_ADDRESS);
	}
	SCI_MEMCPY(data,(uint8*)&(nv_param.bd_addr),  sizeof(BT_ADDRESS));
	return ret;
}
LOCAL BOOLEAN update_nv_SN(uint8 *data,uint32 length)
{
	TEST_DATA_INFO_T    *prod_info_ptr       = NULL;
	
	prod_info_ptr = (TEST_DATA_INFO_T *)SCI_ALLOC(sizeof(TEST_DATA_INFO_T) + 4);
	SCI_ASSERT(prod_info_ptr);/*assert verified*/
	IMEI_GetProductInfo_Protect((uint8*)prod_info_ptr,sizeof(TEST_DATA_INFO_T));
	SCI_MEMCPY((uint8*)prod_info_ptr->header.SN, data, MAX_SN_LEN);/*lint !e613*/
	IMEI_UpdateProductInfo_Protect(FLASH_GetProductInfoAddr(),(uint8*)prod_info_ptr, sizeof(TEST_DATA_INFO_T));
	need_update_finxv_sector = 0;
	SCI_FREE(prod_info_ptr);
	return SCI_TRUE;
}
LOCAL uint32 read_nv_SN(uint8 *data,uint32 max_data_len)
{
	TEST_DATA_INFO_T    *prod_info_ptr       = NULL;
	
	SCI_ASSERT(max_data_len >= MAX_SN_LEN);/*assert verified*/
	prod_info_ptr = (TEST_DATA_INFO_T *)SCI_ALLOC(sizeof(TEST_DATA_INFO_T) + 4);
	SCI_ASSERT(prod_info_ptr);/*assert verified*/
	IMEI_GetProductInfo_Protect((uint8*)prod_info_ptr,sizeof(TEST_DATA_INFO_T));
	SCI_MEMCPY(data,(uint8*)prod_info_ptr->header.SN,  MAX_SN_LEN);/*lint !e613*/
	SCI_FREE(prod_info_ptr);
	return SN_LEN;
}
LOCAL BOOLEAN update_nv_WIFI(uint8 *data,uint32 length)
{
	uint8              *nv_org_ptr          = NULL;
	WIFI_NV_PARAM_T   nv_param;
	BOOLEAN		ret=SCI_FALSE;
	
	nv_org_ptr = g_nv_org_ptr;

	SCI_ASSERT(length == sizeof(WIFI_ADDR_T));/*assert verified*/
	EFS_NvitemRead(NV_WIFI_CONFIG, sizeof(WIFI_NV_PARAM_T), (uint8*)&nv_param);

	SCI_MEMCPY((uint8*)&(nv_param.addr),  data,  sizeof(WIFI_ADDR_T));
	if(NVITEM_UpdateCaliBuf(NV_WIFI_CONFIG,  sizeof(WIFI_NV_PARAM_T),  &nv_param,  nv_org_ptr) == NVERR_NONE)
	{
		need_update_finxv_sector = 1;
		ret = SCI_TRUE;
	}

	return ret;
}
LOCAL uint32 read_nv_WIFI(uint8 *data,uint32 max_data_len)
{
	WIFI_NV_PARAM_T    nv_param = {0};
	uint8              *nv_org_ptr          = NULL;
	uint32		ret=0;
	
	nv_org_ptr = g_nv_org_ptr;
	SCI_ASSERT(max_data_len >= sizeof(WIFI_ADDR_T));/*assert verified*/
	
	if(NVITEM_ReadCaliBuf(NV_WIFI_CONFIG, sizeof(WIFI_NV_PARAM_T), (uint8*)&nv_param, nv_org_ptr)==NVERR_NONE)
	{
    		ret = sizeof(WIFI_ADDR_T);
	}
	SCI_MEMCPY(data,(uint8*)&(nv_param.addr),  sizeof(WIFI_ADDR_T));
	return ret;
}
LOCAL uint32 read_software_version(uint8 *data,uint32 max_data_len)
{
    char        *ver_str_ptr    = SCI_NULL;
    uint16      ver_str_len     = 0;

    ver_str_ptr = (char *)COMMON_GetProjectVersionInfo();
    //end CR25558
    ver_str_len = strlen(ver_str_ptr);
    SCI_MEMCPY(data,ver_str_ptr,  ver_str_len);//lint !e516
    return ver_str_len;
}
LOCAL BOOLEAN update_product_info(uint8 *data,uint32 length)
{
    uint32 pdt_info_addr,pdt_offset,fix_nv_addr_st, fix_nv_addr_end;
    uint8 *p_pdt_info;
    uint32 i;
    
    SCI_ASSERT(length == sizeof(TEST_DATA_INFO_T));/*assert verified*/
    IMEI_UpdateProductInfo_Protect(FLASH_GetProductInfoAddr(),data, sizeof(TEST_DATA_INFO_T));
    need_update_finxv_sector = 0;
    pdt_info_addr = FLASH_GetProductInfoAddr();
    FLASH_GetFixedNvitemAddr(&fix_nv_addr_st, &fix_nv_addr_end);
    pdt_offset = pdt_info_addr -  fix_nv_addr_st;
    p_pdt_info = g_nv_org_ptr + pdt_offset;
    for(i = 0; i < length ; i++)
    {
        p_pdt_info[i] = *data;
        data++;
    }
    return SCI_TRUE;
}

LOCAL uint32 read_product_info(uint8 *data,uint32 max_data_len)
{
	
	SCI_ASSERT(max_data_len >= sizeof(TEST_DATA_INFO_T));/*assert verified*/
	IMEI_GetProductInfo_Protect(data,sizeof(TEST_DATA_INFO_T));
	return sizeof(TEST_DATA_INFO_T);
}
LOCAL void get_command_packet(uint8 *cmd_mask,REF_NVWriteDirect_T *recv_para_ptr,ImeiCmdInf_T *cmd_inf_ptr)
{
	uint8	mask = BIT_0;
	uint8	cmd_no;

	SCI_ASSERT(cmd_mask!=NULL);/*assert verified*/
	SCI_ASSERT(recv_para_ptr!=NULL);/*assert verified*/	
	SCI_ASSERT(cmd_inf_ptr!=NULL);/*assert verified*/
	/*lint -esym(613, cmd_inf_ptr) */
	/*lint -esym(613, cmd_mask) */
	/*lint -esym(613, recv_para_ptr) */

	if(*cmd_mask == 0) /*lint !e613*/
	{
		cmd_inf_ptr->cmd_id = INVALID_IMEI_CMD;
		return;
	}
	
	for(cmd_no=0;cmd_no<7;cmd_no++)
	{
		mask = (BIT_0 <<cmd_no);
		if((*cmd_mask & mask)==mask)
			break;
	}

	switch(mask)
	{
		case   	RM_CALI_NV_IMEI1_MSK :
			cmd_inf_ptr->cmd_id =cmd_no;
			cmd_inf_ptr->data = recv_para_ptr->imei1;
			cmd_inf_ptr->data_len = sizeof(recv_para_ptr->imei1);
		break;
  		case 	RM_CALI_NV_IMEI2_MSK :
			cmd_inf_ptr->cmd_id = cmd_no;
			cmd_inf_ptr->data = recv_para_ptr->imei2;
			cmd_inf_ptr->data_len = sizeof(recv_para_ptr->imei2);
		break;
  		case 	RM_CALI_NV_BT_MSK      :
			cmd_inf_ptr->cmd_id =cmd_no;
			cmd_inf_ptr->data = (uint8 *)&recv_para_ptr->bt_add;
			cmd_inf_ptr->data_len = sizeof(recv_para_ptr->bt_add);
		break;
  		case 	RM_CALI_NV_GPS_MSK    :
			cmd_inf_ptr->cmd_id = cmd_no;
			cmd_inf_ptr->data = (uint8 *)&recv_para_ptr->gps_info;
			cmd_inf_ptr->data_len = sizeof(recv_para_ptr->gps_info);
		break;
  		case 	RM_CALI_NV_IMEI3_MSK  :
			cmd_inf_ptr->cmd_id = cmd_no;
			cmd_inf_ptr->data = recv_para_ptr->imei3;
			cmd_inf_ptr->data_len = sizeof(recv_para_ptr->imei3);
		break;
  		case 	RM_CALI_NV_IMEI4_MSK  :
			cmd_inf_ptr->cmd_id =cmd_no;
			cmd_inf_ptr->data = recv_para_ptr->imei4;
			cmd_inf_ptr->data_len = sizeof(recv_para_ptr->imei4);
		break;
  		case 	RM_CALI_NV_WIFI_MSK  :
			cmd_inf_ptr->cmd_id =cmd_no;
			cmd_inf_ptr->data = recv_para_ptr->wifi_add;
			cmd_inf_ptr->data_len = sizeof(recv_para_ptr->wifi_add);
		break;
		default:
			cmd_inf_ptr->cmd_id = INVALID_IMEI_CMD;
		break;
	}
	*cmd_mask &=~(mask);
}

LOCAL BOOLEAN	write_request_process(MSG_HEAD_T *msg_ptr)
{
	ImeiCmdInf_T		cmd_info = {0};
	uint8	cmd_mask = 0;
	REF_NVWriteDirect_T *recv_para_ptr = PNULL;
	BOOLEAN		result=SCI_TRUE;
	
	SCI_ASSERT(msg_ptr);/*assert verified*/
/*lint -esym(613, msg_ptr) */
	cmd_mask = msg_ptr->subtype & 0x7f;
	cmd_info.cmd_id = 0xff;
	recv_para_ptr = ((REF_NVWriteDirect_T *)(msg_ptr+1)) ;
	do{
		
		if(msg_ptr->type == DIAG_DIRECT_RDVER)
		{
			cmd_info.cmd_id  = 8;//write product info
			cmd_info.data = (uint8 *)(msg_ptr + 1);
			cmd_info.data_len = sizeof(TEST_DATA_INFO_T);
			cmd_mask = 0;			
		}
		else if(msg_ptr->type == DIAG_DIRECT_NV)
		{
			get_command_packet(&cmd_mask,recv_para_ptr,&cmd_info);
		       if(cmd_info.cmd_id == 0xff)
			   	break;
		}
		else if(msg_ptr->type == DIAG_DIRECT_PHSCHK)
		{
			cmd_info.cmd_id  = 7;//write product info
			cmd_info.data = (uint8 *)(msg_ptr + 1);
			cmd_info.data_len = msg_ptr->len -sizeof(*msg_ptr)-sizeof(uint16);
			cmd_mask = 0;
		}
		SCI_ASSERT(cmd_info.cmd_id  < sizeof(call_back_list)/sizeof(call_back_list[0]));/*assert verified*/

		
		//Send_Debug_ACK(0x5a00|cmd_info.cmd_id );
		if(call_back_list[cmd_info.cmd_id ].update!=NULL)
		{
			result = call_back_list[cmd_info.cmd_id ].update(cmd_info.data,cmd_info.data_len);
		}
	}while((cmd_mask!=0)&&(result==SCI_TRUE));
	
	return result;
}

LOCAL uint32	read_request_process(MSG_HEAD_T *msg_ptr,uint32 max_buffer_len)
{
	ImeiCmdInf_T		cmd_info = {0};
	uint8	cmd_mask = 0;
	REF_NVWriteDirect_T *recv_para_ptr = PNULL;
	uint32 	read_len =0;
	
	SCI_ASSERT(msg_ptr);/*assert verified*/

	cmd_mask = msg_ptr->subtype & 0x7f;
	SCI_MEMSET(msg_ptr+1,0,sizeof(REF_NVWriteDirect_T));
	cmd_info.cmd_id = 0xff;
	recv_para_ptr = ((REF_NVWriteDirect_T *)(msg_ptr+1)) ;
	do{
		if(msg_ptr->type == DIAG_DIRECT_RDVER)
		{
			cmd_info.cmd_id  = 8;//write product info
			cmd_info.data = (uint8 *)(msg_ptr + 1);
			cmd_info.data_len = sizeof(TEST_DATA_INFO_T);
			read_len = sizeof(TEST_DATA_INFO_T);
			cmd_mask = 0;			
		}
		else if(msg_ptr->type == DIAG_DIRECT_NV)
		{
			read_len = sizeof(REF_NVWriteDirect_T);
			get_command_packet(&cmd_mask,recv_para_ptr,&cmd_info);
		       if(cmd_info.cmd_id == 0xff)
			   	break;
		}
		else if(msg_ptr->type == DIAG_DIRECT_PHSCHK)
		{
			cmd_info.cmd_id  = 7;//write product info
			cmd_info.data = (uint8 *)(msg_ptr + 1);
			cmd_info.data_len = sizeof(TEST_DATA_INFO_T);
			read_len = sizeof(TEST_DATA_INFO_T);
			cmd_mask = 0;
		}
		else if(msg_ptr->type == DIAG_DIRECT_IMEINUM)
		{
			cmd_info.cmd_id  = 9;//read  nv  imei num
			cmd_info.data = (uint8 *)(msg_ptr + 1);
			cmd_info.data_len = MN_MAX_IMEI_LENGTH;
			read_len = MN_MAX_IMEI_LENGTH;
			cmd_mask = 0;		
		}
		SCI_ASSERT(cmd_info.cmd_id  < sizeof(call_back_list)/sizeof(call_back_list[0]));/*assert verified*/
		if(call_back_list[cmd_info.cmd_id ].read!=NULL)
		{
			call_back_list[cmd_info.cmd_id ].read(cmd_info.data,cmd_info.data_len);
		}
	}while(cmd_mask!=0);
	return read_len;
}

LOCAL void send_request_ack(BOOLEAN result,MSG_HEAD_T *msg_ptr)
{
	
	if(result == SCI_FALSE)
	{
		msg_ptr->subtype = MSG_NACK;
	}
	else
	{
		msg_ptr->subtype = MSG_ACK;
	}
	if(msg_ptr->len > sizeof(MSG_HEAD_T) + sizeof(uint16))
	{
		if(msg_ptr->type!=DIAG_NVITEM_F)
		{
			*((uint16 *)((uint8 *)msg_ptr+msg_ptr->len-sizeof(uint16))) = crc16(0,((uint8 *)(msg_ptr+1)),msg_ptr->len-sizeof(MSG_HEAD_T)-sizeof(uint16));
		}
	}
	else
	{
		*((uint16 *)(msg_ptr+1)) = 0;
	}

	RM_SendRes((uint8*)msg_ptr, (int32)msg_ptr->len);
}

LOCAL BOOLEAN	process_phase_check_large_mode(MSG_HEAD_T *msg_ptr)
{
  	uint8   status;
       uint16  len;
       uint32  offset;
       uint32  address;
	MSG_HEAD_T  *msg_head = (MSG_HEAD_T *)msg_ptr;
	
	if(msg_ptr->subtype == NVITEM_PRODUCT_CTRL_READ)
	{
    		/* The offset field is ignored */
	    	offset  = *(uint16 *)((uint32)msg_head + sizeof(MSG_HEAD_T));
	    	len     = *(uint16 *)((uint32)msg_head + sizeof(MSG_HEAD_T) + sizeof(uint16));

	       address = FLASH_GetProductInfoAddr() + offset;  /*lint !e737 */

        	msg_ptr->len = sizeof(MSG_HEAD_T) + 2 * sizeof(uint16) + len;       

        	NVITEM_GetProductInfo( (uint8 *)msg_ptr+ sizeof(MSG_HEAD_T) + 2 * sizeof(uint16),len);
		return SCI_TRUE;
	}
	 else if( msg_ptr->subtype == NVITEM_PRODUCT_CTRL_WRITE)  
	{
	    	offset  = *(uint16 *)((uint32)msg_head + sizeof(MSG_HEAD_T));
	    	len     = *(uint16 *)((uint32)msg_head + sizeof(MSG_HEAD_T) + sizeof(uint16));	    
	    	address = FLASH_GetProductInfoAddr() + offset;    /*lint !e737 */
	    
	    	/* Write product control information. */
              status = NVITEM_UpdateProductInfo(address, (uint8 *) ((uint32)msg_ptr + sizeof(MSG_HEAD_T) + 2 * sizeof(uint16)), len); 	
		msg_ptr->len = 8;
		return status;
    	}

	return SCI_FALSE;

}
/*****************************************************************************/
//  Description: _is_AccessIMEI 
//	Note:        
/*****************************************************************************/
LOCAL BOOLEAN _is_AccessIMEI(uint8 subtype)
{

    if( (subtype & RM_CALI_NV_IMEI1_MSK) ||
        (subtype & RM_CALI_NV_IMEI2_MSK) ||
        (subtype & RM_CALI_NV_IMEI3_MSK) ||
        (subtype & RM_CALI_NV_IMEI4_MSK))
        // only IMEI access to disabel Flag
        return SCI_TRUE;
    else
        return SCI_FALSE;
}



//uint32 my_test_flag = 0;
/*****************************************************************************/
//  Description: DisableUsbAutoModeFlag 
//	Note:        "g_nv_org_ptr" must be inited first
/*****************************************************************************/
LOCAL uint32 DisableUsbAutoModeFlag(void)
{
    // flag move to NV_L1_CALIBRATION_PARAMETER
    ADC_T adc_t = {0};  
    // read adc
    REF_GetCalibrationPara_Adc((uint32*)&adc_t, sizeof(ADC_T)); 
    //Send_Debug_ACK(0xFF00 | adc_t.reserved[6]);
    
    // only writing NV if flag set
    if( adc_t.reserved[6] & REF_CONFIG_USB_AUTO_MODE_BIT)
    {   
        uint8 * cali_param_ptr = 0;
        uint32  cali_param_len = 0;

        // disable
        adc_t.reserved[6] &= ~REF_CONFIG_USB_AUTO_MODE_BIT;
        // write adc
        cali_param_ptr = (uint8 *)REF_SetMemCalibrationPara_Adc( (uint32*)&adc_t, sizeof(ADC_T), &cali_param_len);
        //Send_Debug_ACK(cali_param_len); // debug

        // write cali
        NVITEM_UpdateCaliBuf(NV_L1_CALIBRATION_PARAMETER, cali_param_len, 
                             cali_param_ptr, (void *)g_nv_org_ptr);
        SCI_FREE(cali_param_ptr);
    }

    return 0;
}
void RM_Cali_NvAccess(void)
{
	uint8 		*recv_buffer;
	MSG_HEAD_T	*msg_ptr;
	BOOLEAN		result;
	uint32		recv_count;
	uint16		read_len;
	uint16		crc;
	uint16		recv_crc;
	uint32		*TimeTrace;
	ERR_IMEI_E	error = IMEI_ERR_NONE;
#ifndef NANDBOOT_SUPPORT
	uint32		slave_address=0;
#endif

	
	recv_buffer = (uint8*)SCI_ALLOC(MAX_MSG_SIZE);
	SCI_ASSERT(recv_buffer);/*assert verified*/
	g_nv_org_ptr = (uint8*)NVITEM_MallocCaliBuf();
	
#ifndef NANDBOOT_SUPPORT
	slave_address = HAL_SectorToAddr(0);
	if(*(uint32 *)slave_address != 0xFFFFFFFF)
		HAL_EraseSector(slave_address);	
#endif

	for(;;)
	{
		
		SCI_MEMSET(recv_buffer,0,MAX_MSG_SIZE);
		
		recv_count = IMEI_RM_RecvCMD(recv_buffer);
		//while(1){if(my_test_flag == 0) break;}
		SCI_ASSERT(recv_count >= sizeof(MSG_HEAD_T)+ sizeof(uint16));/*assert verified*/
/*lint -esym(613, recv_buffer) */
		msg_ptr = (MSG_HEAD_T *)recv_buffer;
#ifdef  __IMEI_DEBUG_ENABLE__        
        g_msg_ptr = (MSG_HEAD_T *)debug_msg_buffer;
		*g_msg_ptr = *msg_ptr;
#endif
		if(recv_count!=msg_ptr->len)
		{
			msg_ptr->len = sizeof(MSG_HEAD_T)+ sizeof(uint16)*2;
			*(uint16 *)(msg_ptr+1) = IMEI_CRC_ERR;
			send_request_ack(SCI_FALSE,msg_ptr);
			continue;
		}
		if(msg_ptr->type == DIAG_NVITEM_F)
		{
			result = process_phase_check_large_mode(msg_ptr);
			send_request_ack((BOOLEAN)!result,msg_ptr);	
			continue;
		}
#ifdef	_ENABLE_CRC16_
		recv_crc = *((uint16*)&recv_buffer[msg_ptr->len-sizeof(uint16)]);

		crc = crc16(0,recv_buffer+sizeof(MSG_HEAD_T),msg_ptr->len - sizeof(MSG_HEAD_T)-sizeof(uint16));
		if(crc!=recv_crc)
		{

			msg_ptr->len = sizeof(MSG_HEAD_T)+ sizeof(uint16)*2;
			*(uint16 *)(msg_ptr+1) = IMEI_CRC_ERR;
			send_request_ack(SCI_FALSE,msg_ptr);
			continue;
		}	
#endif
		need_update_finxv_sector = 0;

		if((msg_ptr->subtype & RW_MASK) == WRITE_MODE)
		{
			if((msg_ptr->subtype & RM_VALID_CMD_MSK)!=0)
			{
				result = write_request_process(msg_ptr);

				if((result == SCI_TRUE)&&(need_update_finxv_sector))
				{// disable USB auto up in "write IMEI" process 
                    if(_is_AccessIMEI(msg_ptr->subtype))
                    {
                        DisableUsbAutoModeFlag();
                    }
					//Start_WriteFlash_Time = SCI_GetTickCount();
					result = NVITEM_UpdateNvParam(g_nv_org_ptr);
                    #ifdef IMEI_OTP_SUPPORT_EFUSE
                      if(_is_AccessIMEI(msg_ptr->subtype))
                        result &= IMEI_EfuseFlushTempBuf();
                    #endif
					need_update_finxv_sector = 0;
				}
				else
				{
					error = IMEI_SAVE_ERR;
				}
			}
			else
			{
				error = IMEI_CMD_ERR;
				result = SCI_FALSE;
			}			
			TimeTrace = (uint32 *)(msg_ptr+1);
			if(result == SCI_TRUE)
			{
				TimeTrace = (uint32 *)(msg_ptr+1);
				msg_ptr->len = sizeof(MSG_HEAD_T) + sizeof(uint16);
			}
			else
			{
				
				*(uint16 *)(msg_ptr+1) = error;
				msg_ptr->len = sizeof(MSG_HEAD_T)  + sizeof(uint16) * 2;
			}
		}
		else
		{

			read_len = read_request_process(msg_ptr,MAX_MSG_SIZE-sizeof(MSG_HEAD_T)-sizeof(uint16));
			
			
			if(read_len == 0)
			{
				result = SCI_FALSE;
				msg_ptr->len = sizeof(MSG_HEAD_T) + sizeof(uint16);
			}
			else
			{
				result = SCI_TRUE;
				msg_ptr->len = sizeof(MSG_HEAD_T)+read_len + sizeof(uint16);
			}
		}
		send_request_ack(result,msg_ptr);	



	}
}	
void	RM_Cali_NvAccess_CAL_Mode(uint8 		**recv_buffer,uint16 *dest_len,MSG_HEAD_T	*recv_msg_ptr)
{
	MSG_HEAD_T	*msg_ptr;
	BOOLEAN		result;
	uint16		read_len;
	uint16		crc;
	uint16		recv_crc;
	uint16	  i;
	ERR_IMEI_E	error = IMEI_ERR_NONE;

	
	*recv_buffer = (uint8*)SCI_ALLOC(MAX_MSG_SIZE);
	SCI_ASSERT(*recv_buffer);/*assert verified*/
	SCI_MEMSET(*recv_buffer,0,MAX_MSG_SIZE);
	g_nv_org_ptr = (uint8*)NVITEM_MallocCaliBuf();
	msg_ptr = (MSG_HEAD_T *)(*recv_buffer);

	for(i=0;i<recv_msg_ptr->len;i++)
	{
		(*recv_buffer)[i]=((uint8 *)recv_msg_ptr)[i];
	}
	
	do{
#ifdef  __IMEI_DEBUG_ENABLE__   

		g_msg_ptr = (MSG_HEAD_T *)debug_msg_buffer;
		*g_msg_ptr = *msg_ptr;
#endif
#ifdef	_ENABLE_CRC16_
		recv_crc = *((uint16*)&((*recv_buffer)[recv_msg_ptr->len-sizeof(uint16)]));

		crc = crc16(0,(uint8 *)(recv_msg_ptr+1),recv_msg_ptr->len - sizeof(MSG_HEAD_T)-sizeof(uint16));
		if(crc!=recv_crc)
		{
			msg_ptr->len = sizeof(MSG_HEAD_T)+ sizeof(uint16)*2;
			*(uint16 *)(msg_ptr+1) = IMEI_CRC_ERR;
			*dest_len = msg_ptr->len;
			break;
		}	
#endif
		need_update_finxv_sector = 0;

		if((msg_ptr->subtype & RW_MASK) == WRITE_MODE)
		{
			if((msg_ptr->subtype & RM_VALID_CMD_MSK)!=0)
			{
				result = write_request_process(msg_ptr);
				if((result == SCI_TRUE)&&(need_update_finxv_sector))
				{
				    if(RM_GetCalibrationMode() && 
				       _is_AccessIMEI(msg_ptr->subtype))
				    {
                        // disable USB auto up in "write IMEI" + CAL mode
                        // else, reserve it until PC send "Clean" CMD,
                        // refe to HandleNvitemAccess() in diag.c
                        DisableUsbAutoModeFlag();
                    }
					result = NVITEM_UpdateNvParam(g_nv_org_ptr);
                    #ifdef IMEI_OTP_SUPPORT_EFUSE
                      if(_is_AccessIMEI(msg_ptr->subtype))
                        result &= IMEI_EfuseFlushTempBuf();
                    #endif
				}
				else
				{
					error = IMEI_SAVE_ERR;
				}
			}
			else
			{
				error = IMEI_CMD_ERR;
				result = SCI_FALSE;
			}			

			if(result == SCI_TRUE)
			{
				msg_ptr->len = sizeof(MSG_HEAD_T) + sizeof(uint16);
				msg_ptr->subtype = MSG_ACK;
				*((uint16 *)((uint8 *)(msg_ptr+1))) = 0;
			}
			else
			{
				
				*(uint16 *)(msg_ptr+1) = error;
				msg_ptr->subtype = MSG_NACK;
				*((uint16 *)((uint8 *)(msg_ptr+1)+2)) = 0;
				msg_ptr->len = sizeof(MSG_HEAD_T)  + sizeof(uint16) * 2;
			}
		}
		else
		{

			read_len = read_request_process(msg_ptr,MAX_MSG_SIZE-sizeof(MSG_HEAD_T)-sizeof(uint16));

			if(read_len == 0)
			{
				result = SCI_FALSE;
				msg_ptr->len = sizeof(MSG_HEAD_T) + sizeof(uint16);
				*((uint16 *)((uint8 *)(msg_ptr+1))) = 0;
				msg_ptr->subtype = MSG_NACK;
			}
			else
			{
				result = SCI_TRUE;
				msg_ptr->len = sizeof(MSG_HEAD_T)+read_len + sizeof(uint16);
				*((uint16 *)((uint8 *)(msg_ptr+1)+read_len)) =recv_crc= crc16(0,((uint8 *)(msg_ptr+1)),read_len);
				msg_ptr->subtype = MSG_ACK;
			}
		}
		*dest_len = msg_ptr->len;
		
	}while(0);

	if(g_nv_org_ptr)
	{
		SCI_Free(g_nv_org_ptr);
		g_nv_org_ptr = NULL;
	}
}

#ifdef IMEI_OTP_SUPPORT_EFUSE

//#define IMEI_OTP_TEMP_BUF_SELFTEST  // used for test

#define IMEI_ARRAY_SIZE(a)       (sizeof(a) / sizeof((a)[0]))

#define MAX_IMEI_BIT_LEN    60  // 7.5 byte 

#ifdef MMI_MULTI_SIM_SYS_SINGLE
    #define MAX_IMEI_IN_EFUSE_BIT_LEN    MAX_IMEI_BIT_LEN 
#elif defined(MMI_MULTI_SIM_SYS_DUAL)
    #define MAX_IMEI_IN_EFUSE_BIT_LEN    (MAX_IMEI_BIT_LEN<<1) 
#else
    #error  "Due to the max size of EUFSE, only support 2 IMEI right now"
#endif

LOCAL uint8 IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH<<1] = {0}; // only support 2 IMEI

void _IMEI_EfuseDumpTempBuf(void)
{
    SCI_TRACE_LOW("IMEI_EFUSE_BUF = 0x%02X%02X%02X%02X-%02X%02X%02X%02X-%02X%02X%02X%02X ",
        IMEI_EFUSE_BUF[0], IMEI_EFUSE_BUF[1], IMEI_EFUSE_BUF[2], IMEI_EFUSE_BUF[3],
        IMEI_EFUSE_BUF[4], IMEI_EFUSE_BUF[5], IMEI_EFUSE_BUF[6], IMEI_EFUSE_BUF[7],
        IMEI_EFUSE_BUF[8], IMEI_EFUSE_BUF[9], IMEI_EFUSE_BUF[10], IMEI_EFUSE_BUF[11]        
        );
}
/*****************************************************************************/
//  Description: flush the tem buf to EFUSE
//	Note:  return 0: success;  > 0: error num
/*****************************************************************************/

PUBLIC uint32 IMEI_EfuseFlushTempBuf(void)
{   uint32 ret = 0;
  #ifdef IMEI_OTP_TEMP_BUF_SELFTEST
    return SCI_TRUE;
  #else
    EFUSE_HAL_Open();
    ret = EFUSE_HAL_WriteImei(&IMEI_EFUSE_BUF[0], MAX_IMEI_IN_EFUSE_BIT_LEN);
    SCI_TRACE_LOW("IMEI_EfuseFlushTempBuf: ret = 0x%0X",ret);
    if( EFUSE_RESULT_SUCCESS == ret)
    {
       ret = SCI_TRUE;
    }
    else
    {
       ret = SCI_FALSE;
    }
    EFUSE_HAL_Close();
    return ret;
  #endif
}

/*****************************************************************************/
//  Description: write to a temp buf first 
//	Note:        NV_IMEI, NV_IMEI1, NV_IMEI2
/*****************************************************************************/
/* NOTE:  IMEI_EFUSE_BUF arragement
 please note EFUSE is bit arragement, we need change byte to bit

             <---  IMEI1 7.5 byte--->  <---  IMEI2 7.5 byte---> 
IMEI   ¡êo     1  52 27 30 17 38 63 40  2  52 27 30 17 38 63 40

             <---  IMEI1 8   byte--->  <---  IMEI2 8   byte---> 
IMEI NV¡êo     1A 25 72 03 71 83 36 04  2A 25 57 01 91 25 23 76

             <---  IMEI1 7.5 byte -->
                                   <---  IMEI2 7.5 byte---> 
EFUSE  :      25 72 03 71 83 36 04 21  25 57 01 91 25 23 76

NOTE: IMEI 1st digit num shift to the end with IMEI2 1st digit num.
*/

PUBLIC uint32 IMEI_EfuseWriteTempBuf(uint16 ItemID, uint8 *data,uint32 length)
{
    SCI_ASSERT(data);/* assert verified */

    if(NV_IMEI == ItemID)
    {
        SCI_MEMCPY(&IMEI_EFUSE_BUF[0], &data[1], MN_MAX_IMEI_LENGTH-1);
        // NOTE: IMEI 1st digit num shift to the end with IMEI2 1st digit num.
        IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH-1] &= 0xF0;        
        IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH-1] |= (data[0] & 0xF0) >> 4;
    }
 #ifdef MMI_MULTI_SIM_SYS_DUAL
    else if(NV_IMEI1 == ItemID)
    {   // NOTE: IMEI 1st digit num shift to the end with IMEI2 1st digit num.
        IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH-1] &= 0x0F;
        IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH-1] |= (data[0] & 0xF0);
        SCI_MEMCPY(&IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH], &data[1], MN_MAX_IMEI_LENGTH-1);
    }
 #endif     
    _IMEI_EfuseDumpTempBuf(); 
    return 0;
}

/*****************************************************************************/
//  Description: read imei from imei
//	Note: return 0: error; return > 0: read valid data length
//        Please see above  IMEI_EFUSE_BUF arragement
/*****************************************************************************/

PUBLIC uint32 IMEI_EfuseRead(uint16 ItemID, uint8 *data,uint32 length)
{   uint32 ret = 0;

    SCI_ASSERT(data);/* assert verified */
  #ifndef IMEI_OTP_TEMP_BUF_SELFTEST
    // read the whole IMEI efuse zone
    SCI_MEMSET(&IMEI_EFUSE_BUF[0], 0, IMEI_ARRAY_SIZE(IMEI_EFUSE_BUF));
    EFUSE_HAL_Open();
    ret = EFUSE_HAL_ReadImei(&IMEI_EFUSE_BUF[0], MAX_IMEI_IN_EFUSE_BIT_LEN);
    SCI_TRACE_LOW("IMEI_EfuseRead: ret = 0x%0X",ret);
    EFUSE_HAL_Close();
  #endif
    _IMEI_EfuseDumpTempBuf();  
    if(NV_IMEI == ItemID)
    {   // NOTE: IMEI 1st digit num shift to the end with IMEI2 1st digit num.
        data[0] = ((IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH -1] & 0x0F) << 4) | 0x0A ;
        SCI_MEMCPY(&data[1], &IMEI_EFUSE_BUF[0], MN_MAX_IMEI_LENGTH-1);
    }
   #ifdef MMI_MULTI_SIM_SYS_DUAL
    else if(NV_IMEI1 == ItemID)
    {   // NOTE: IMEI 1st digit num shift to the end with IMEI2 1st digit num.
        data[0] = (IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH -1] & 0xF0) | 0x0A ; 
        SCI_MEMCPY(&data[1], &IMEI_EFUSE_BUF[MN_MAX_IMEI_LENGTH], MN_MAX_IMEI_LENGTH-1);
    }
  #endif
    return MN_MAX_IMEI_LENGTH;
}
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif //

