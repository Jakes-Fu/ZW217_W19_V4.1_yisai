#ifndef __LOAD_MODEM_H__
#define __LOAD_MODEM_H__

#include "sci_types.h"


#define MEM_NAME_MAX_SIZE              20
///////////////////////////////////////"01234567890123456789"//name max  size is 20 Bytes

#define MEM_MEM_INDEX_LIST_NAME	       "mem_index_list"
#define	MEM_CP_ASSERT_NAME                 "cp_assert_reg"
#define	MEM_IMEI_NAME                  "imei.bin"
#define	MEM_DYNAMIC_NV_NAME            "dynamic_nv.bin"
#define	MEM_STATIC_NV_NAME             "static_nv.bin"
#define	MEM_PHY_NV_NAME                "phy_nv.bin"
#define	MEM_PSM_NV_NAME                "psm_nv.bin"
#define	MEM_CFW_NV_NAME                "cfw_nv.bin"
#define	MEM_RF_NV_NAME                 "rf_nv.bin"
#define   MEM_PM2_DATA_SAVE_NAME	       "pm2_aon_data"
#define   MEM_SIMLOCK_NV_NAME	       "simlock_nv.bin"
#define   MEM_LTE_RF_CALIB_NAME	       "lte_rf_calib.bin"
#define   MEM_GOLDEN_BOARD_NAME	       "golden_board.bin"
#define	MEM_RISCV_BIN_NAME             "riscV.bin"
#define	MEM_AUDIO_SM_NAME              "audio_sm"
#define	MEM_AP_CP_SM_NAME              "ap_cp_sm"
#define	MEM_CP_BIN_NAME                "cp.bin"
#define	MEM_AP_BIN_NAME                "ap.bin"
#define	MEM_CP_JUMP_NAME              "cp_jump"
#define	MEM_CP_DEBUG_NAME              "cp_debug"
#define	MEM_CP_LOG_BUFF_NAME       "cp_log_buf"
#define	MEM_PHY_DEBUG_NAME              "phy_debug"


typedef struct T_MEM_RELAY_ADDRESS_AREA_Tag
{
uint32 cp_boot_sector_reload_struct_ptr;
uint32 cp_boot_sector_struct_ptr;
uint32 cp_retrieve_map_structure_ptr;
uint32 ap_dump_ip_register_save_base;
uint32 ap_dump_ip_register_base;
uint32 ap_dump_ip_register_end;
uint32 resered[58];
}T_MEM_RELAY_ADDRESS_AREA;

typedef struct ims_nv_info {
    uint32 ims_csm_addr;
    uint32 ims_csm_len;

    uint32 ims_sapp_addr;
    uint32 ims_sapp_len;

    uint32 ims_isim_addr;
    uint32 ims_isim_len;

    uint32 ims_sim1_delta_addr;
    uint32 ims_sim1_delta_len;

    uint32 ims_sim2_delta_addr;
    uint32 ims_sim2_delta_len;

    uint32 ims_plmn1_delta_addr;
    uint32 ims_plmn1_delta_len;

    uint32 ims_plmn2_delta_addr;
    uint32 ims_plmn2_delta_len;
} ims_nv_info_t;

/*****************************************************************************/
 //  Description:    Get the dump ip register save address
//	Param:
//  Return:
//  return dump save address
//	Note:
/*****************************************************************************/
T_MEM_RELAY_ADDRESS_AREA* GET_IPRegisterDumpAreaSaveAddr(void);

/*****************************************************************************/
 //  Description:    Get the memory address and length
 //              from memory index list
//	Param:
//  @name: memory name string,defined the Load_modem.h
//  @len: if @len isn,t equal NULL,memory len from memory index list
//  Return:
//  return memory start address
//  @len: if @len isn,t equal NULL,return memory len from memory index list
//	Note:
/*****************************************************************************/
extern void* GET_MemoryStartAddressAndLen(char *name, uint32 *len);

extern uint32 SPIFLASH_GetOperatorDataAddress(void);
extern uint32 SPIFLASH_GetOperatorDataSize(void);

extern uint32 v_cp_jump_addr;
extern uint32 v_zsp_jump_addr;
extern uint8 load_use_lzma(void);

#endif /*__LOAD_MODEM_H__*/

