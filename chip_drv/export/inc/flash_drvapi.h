/******************************************************************************
 ** File Name:      flash_drvapi.h                                                   *
 ** Author:         lin.liu                                                   *
 ** DATE:           27/11/2001                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 **                 Flash Driver                                              *
 **                                                                           *
 **                 used by flash.c                                           *
 **                                                                           *
 **                 user can modify and replace the file, but all functions   *
 **                 must be supported                                         *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 27/11/2001     Lin.liu          Create.                                   *
 ******************************************************************************/
#ifndef _FLASH_DRVAPI_H_
#define _FLASH_DRVAPI_H_

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#include "timer_drvapi.h"
#include "normcp_spec.h"
#include "umem_cfg.h"
#include "emc_drvapi.h"
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif


/**---------------------------------------------------------------------------*
 **                         DEFINE                                            *
 **---------------------------------------------------------------------------*/

//flash basic WR/RD
#define FLASH_WR(b,d)         *( (FLASH_PTR_TYPE)(b)) = (d)
#define FLASH_RD(b)           *( (FLASH_PTR_TYPE)(b))

//nor flash manufacture ID
#ifndef FLASH_AMD_ID
#define FLASH_SAMSUNG_ID        (0xEC)
#define FLASH_AMD_ID                (0x1)
#define FLASH_INTEL_ID              (0x89)
#define FLASH_ST_ID                 (0x20)
#define FLASH_SST_ID                (0xBF)
#define FLASH_TSB_ID            (0x98)
#define FLASH_CFEON_ID              (0x7F)
#endif

//nor flash manufacture ID

/* +++ bug 1350964 /1341705/1349442 */
extern void MMU_PHY_InvalideICACHEALL (void);
/* --- bug 1350964 /1341705/1349442 */

//reset Intel/AMD flash
#define RESET_FLASH   {    *(FLASH_PTR_TYPE)(0x0) = 0xF0;\
        *(FLASH_PTR_TYPE)(0x0) = 0xFF;\
    }

//disable irq/fiq
#define FLASH_IRQ_FIQ_DISABLE  0xC0

/* Below is the raw definitions for Threadx. */
#define FLASH_TX_DISABLE                  s_flash_interrupt_save =  _tx_thread_interrupt_control(FLASH_IRQ_FIQ_DISABLE);
#define FLASH_TX_RESTORE                  _tx_thread_interrupt_control(s_flash_interrupt_save);

//Disable irq/fiq and cache(M)
#define FLASH_DISABLE_IRQ   FLASH_TX_DISABLE    \
    DISABLE_CACHE
//Enable irq/fiq and cache(M)
#define FLASH_RESTORE_IRQ   {MMU_PHY_InvalideICACHEALL(); \
    ENABLE_CACHE  \
    FLASH_TX_RESTORE }

//Enable the system tick count
#ifdef PLATFORM_SC6530
//#define ENABLE_SYSTEM_TICK_COUNT  *(volatile uint32 *)(APB_EB0_SET) = SYST_RTC_EB_SET
#elif defined(PLATFORM_SC6530)
//#define ENABLE_SYSTEM_TICK_COUNT  *(volatile uint32 *)(APB_EB0_SET) = SYST_RTC_EB_SET
#else
//#define ENABLE_SYSTEM_TICK_COUNT  *(volatile uint32 *)(GR_GEN1) |= BIT_13;
#endif

//check any irq/fiq status be set.
#if defined(PLATFORM_UWS6121E)
#define   GICD_ISPEND_ADDR          (0x0800000+0x1200)  //bringup_tmp
#define   GICD_ISENABLE_ADDR        (0x0800000+0x1100)
#if 1
#define GET_INTERRUPT_FLAG   ((CHIP_REG_GET(GICD_ISPEND_ADDR + 0x4) & CHIP_REG_GET(GICD_ISENABLE_ADDR + 0x4))  \
                            ||(CHIP_REG_GET(GICD_ISPEND_ADDR + 0x8) & CHIP_REG_GET(GICD_ISENABLE_ADDR + 0x8)) \
                            ||(CHIP_REG_GET(GICD_ISPEND_ADDR + 0xC) & CHIP_REG_GET(GICD_ISENABLE_ADDR + 0xC)))
#else
#define GET_INTERRUPT_FLAG   (0)    /*there is still error when exit flash_status check,so setted to 0*/
#endif
#else
#define GET_INTERRUPT_FLAG (CHIP_REG_GET(INT_IRQ_STS) | CHIP_REG_GET(INT_FIQ_STS))
#endif
//invert the high/low byte of the given word.
#define INVERT_WORD( _w )           _w = ((_w) >> 8) + ((_w) << 8)

//ARM target, Big-Endian
#define FIRST_BYTE( _word )         (uint8)( (_word) >> 8 )
#define SECOND_BYTE( _word )        (uint8)( (_word) & 0xFF )

//ARM target, big-endian
#define MAKE_WORD( _fst, _snd ) ((uint16)(_fst) << 8) + ((uint16)(uint8)(_snd))

#define FLASH_INIT_MAGIC        (0xFAACCAAF)
#ifdef SECURITY_DATA_SUPPORT
#define FLASH_BSC_ADDR			0x0
#define FLASH_BSC_END_ADDR	(FLASH_BSC_ADDR+0x10000)  //unlocked the whole bootloader

#ifndef FOTA_SUPPORT_NONE
#define FLASH_ADDR_IS_VALID( _addr )  \
    ((((_addr) >= s_flash_efs_start_addr   ) && ((_addr) <= s_flash_efs_end_addr           ))\
     ||(((_addr) >= s_flash_fixed_nvitem_addr) && ((_addr) <= s_flash_fixed_nvitem_end_addr  ))\
     ||(((_addr) >= FLASH_BSC_ADDR) && ((_addr) <= FLASH_BSC_END_ADDR  ))\
     ||(((_addr) >= s_flash_product_info_addr) && ((_addr) <= (s_flash_product_info_addr + s_flash_sector_size)))\
     ||(((_addr) >= s_flash_umem_addr ) && ( (_addr) <= (s_flash_umem_addr+s_flash_umem_size)))\
     ||(((_addr) >= s_flash_simlock_addr ) && ( (_addr) <= s_flash_simlock_end_addr)) \
     ||(((_addr) >= s_flash_xfile_addr ) && ( (_addr) <= s_flash_xfile_end_addr)) \
     ||(((_addr) >= s_flash_auto_sms_addr ) && ( (_addr) <= s_flash_auto_sms_end_addr)) \
     ||(((_addr) >= s_flash_operator_data_addr) && ( (_addr) <= s_flash_operator_data_end_addr)) \
     ||(((_addr) >= s_flash_crystal_aging_addr) && ( (_addr) <= s_flash_crystal_aging_end_addr)) \
     ||(((_addr) >= s_flash_apn_addr) && ( (_addr) <= s_flash_apn_end_addr)) \
     ||(((_addr) >= s_flash_cfg_nv_addr) && ( (_addr) <= s_flash_cfg_nv_end_addr)) \
     ||(((_addr) >= s_flash_ims_toggle_cfg_addr) && ( (_addr) <= s_flash_ims_toggle_cfg_end_addr)) \
     ||(((_addr) >= s_flash_lcd_sensor_addr ) && ( (_addr) < s_flash_end_addr)))
#else
#define FLASH_ADDR_IS_VALID( _addr )  \
    ((((_addr) >= s_flash_efs_start_addr   ) && ((_addr) <= s_flash_efs_end_addr           ))\
     ||(((_addr) >= s_flash_fixed_nvitem_addr) && ((_addr) <= s_flash_fixed_nvitem_end_addr  ))\
     ||(((_addr) >= FLASH_BSC_ADDR) && ((_addr) <= FLASH_BSC_END_ADDR  ))\
     ||(((_addr) >= s_flash_product_info_addr) && ((_addr) <= (s_flash_product_info_addr + s_flash_sector_size)))\
     ||(((_addr) >= s_flash_umem_addr ) && ( (_addr) <= (s_flash_umem_addr+s_flash_umem_size)))\
     ||(((_addr) >= s_flash_xfile_addr ) && ( (_addr) <= s_flash_xfile_end_addr)) \
     ||(((_addr) >= s_flash_auto_sms_addr ) && ( (_addr) <= s_flash_auto_sms_end_addr)) \
     ||(((_addr) >= s_flash_operator_data_addr) && ( (_addr) <= s_flash_operator_data_end_addr)) \
     ||(((_addr) >= s_flash_crystal_aging_addr) && ( (_addr) <= s_flash_crystal_aging_end_addr)) \
     ||(((_addr) >= s_flash_apn_addr) && ( (_addr) <= s_flash_apn_end_addr)) \
     ||(((_addr) >= s_flash_cfg_nv_addr) && ( (_addr) <= s_flash_cfg_nv_end_addr)) \
     ||(((_addr) >= s_flash_ims_toggle_cfg_addr) && ( (_addr) <= s_flash_ims_toggle_cfg_end_addr)) \
     ||(((_addr) >= s_flash_simlock_addr ) && ( (_addr) <= s_flash_simlock_end_addr)))
#endif
#else
#ifndef FOTA_SUPPORT_NONE
#ifdef FOTA_SUPPORT_ABUP
#define FLASH_ADDR_IS_VALID( _addr )  \
    ((((_addr) >= s_flash_efs_start_addr   ) && ((_addr) <= s_flash_efs_end_addr           ))\
     ||(((_addr) >= s_flash_fixed_nvitem_addr) && ((_addr) <= s_flash_fixed_nvitem_end_addr  ))\
     ||(((_addr) >= s_flash_product_info_addr) && ((_addr) <= (s_flash_product_info_addr + s_flash_sector_size)))\
     ||(((_addr) >= s_flash_umem_addr ) && ( (_addr) <= (s_flash_umem_addr+s_flash_umem_size)))\
     ||(((_addr) >= s_flash_simlock_addr ) && ( (_addr) <= s_flash_simlock_end_addr)) \
     ||(((_addr) >= s_flash_xfile_addr ) && ( (_addr) <= s_flash_xfile_end_addr)) \
     ||(((_addr) >= s_flash_auto_sms_addr ) && ( (_addr) <= s_flash_auto_sms_end_addr)) \
     ||(((_addr) >= s_flash_operator_data_addr) && ( (_addr) <= s_flash_operator_data_end_addr)) \
     ||(((_addr) >= s_flash_crystal_aging_addr) && ( (_addr) <= s_flash_crystal_aging_end_addr)) \
     ||(((_addr) >= s_flash_apn_addr) && ( (_addr) <= s_flash_apn_end_addr)) \
     ||(((_addr) >= s_flash_cfg_nv_addr) && ( (_addr) <= s_flash_cfg_nv_end_addr)) \
     ||(((_addr) >= s_flash_ims_toggle_cfg_addr) && ( (_addr) <= s_flash_ims_toggle_cfg_end_addr)) \
     ||(((_addr) >= s_flash_lcd_sensor_addr ) && ( (_addr) < s_flash_end_addr)) \
     ||(((_addr) >= s_flash_abup_fota_app_addr ) && ( (_addr) < s_flash_abup_fota_app_addr_end)))
#else
#ifdef FOTA_SUPPORT_CMIOT
#define FLASH_ADDR_IS_VALID( _addr )  \
    ((((_addr) >= s_flash_efs_start_addr   ) && ((_addr) <= s_flash_efs_end_addr           ))\
     ||(((_addr) >= s_flash_fixed_nvitem_addr) && ((_addr) <= s_flash_fixed_nvitem_end_addr  ))\
     ||(((_addr) >= s_flash_product_info_addr) && ((_addr) <= (s_flash_product_info_addr + s_flash_sector_size)))\
     ||(((_addr) >= s_flash_umem_addr ) && ( (_addr) <= (s_flash_umem_addr+s_flash_umem_size)))\
     ||(((_addr) >= s_flash_simlock_addr ) && ( (_addr) <= s_flash_simlock_end_addr)) \
     ||(((_addr) >= s_flash_xfile_addr ) && ( (_addr) <= s_flash_xfile_end_addr)) \
     ||(((_addr) >= s_flash_auto_sms_addr ) && ( (_addr) <= s_flash_auto_sms_end_addr)) \
     ||(((_addr) >= s_flash_operator_data_addr) && ( (_addr) <= s_flash_operator_data_end_addr)) \
     ||(((_addr) >= s_flash_crystal_aging_addr) && ( (_addr) <= s_flash_crystal_aging_end_addr)) \
     ||(((_addr) >= s_flash_apn_addr) && ( (_addr) <= s_flash_apn_end_addr)) \
     ||(((_addr) >= s_flash_cfg_nv_addr) && ( (_addr) <= s_flash_cfg_nv_end_addr)) \
     ||(((_addr) >= s_flash_ims_toggle_cfg_addr) && ( (_addr) <= s_flash_ims_toggle_cfg_end_addr)) \
     ||(((_addr) >= s_flash_lcd_sensor_addr ) && ( (_addr) < s_flash_end_addr)) \
     ||(((_addr) >= s_flash_cmiot_fota_app_addr ) && ( (_addr) < s_flash_cmiot_fota_app_addr_end)))

#else
#define FLASH_ADDR_IS_VALID( _addr )  \
    ((((_addr) >= s_flash_efs_start_addr   ) && ((_addr) <= s_flash_efs_end_addr           ))\
     ||(((_addr) >= s_flash_fixed_nvitem_addr) && ((_addr) <= s_flash_fixed_nvitem_end_addr  ))\
     ||(((_addr) >= s_flash_product_info_addr) && ((_addr) <= (s_flash_product_info_addr + s_flash_sector_size)))\
     ||(((_addr) >= s_flash_umem_addr ) && ( (_addr) <= (s_flash_umem_addr+s_flash_umem_size)))\
     ||(((_addr) >= s_flash_simlock_addr ) && ( (_addr) <= s_flash_simlock_end_addr)) \
     ||(((_addr) >= s_flash_xfile_addr ) && ( (_addr) <= s_flash_xfile_end_addr)) \
     ||(((_addr) >= s_flash_auto_sms_addr ) && ( (_addr) <= s_flash_auto_sms_end_addr)) \
     ||(((_addr) >= s_flash_operator_data_addr) && ( (_addr) <= s_flash_operator_data_end_addr)) \
     ||(((_addr) >= s_flash_crystal_aging_addr) && ( (_addr) <= s_flash_crystal_aging_end_addr)) \
     ||(((_addr) >= s_flash_apn_addr) && ( (_addr) <= s_flash_apn_end_addr)) \
     ||(((_addr) >= s_flash_cfg_nv_addr) && ( (_addr) <= s_flash_cfg_nv_end_addr)) \
     ||(((_addr) >= s_flash_ims_toggle_cfg_addr) && ( (_addr) <= s_flash_ims_toggle_cfg_end_addr)) \
     ||(((_addr) >= s_flash_lcd_sensor_addr ) && ( (_addr) < s_flash_end_addr)))
#endif
#endif
#else
#define FLASH_ADDR_IS_VALID( _addr )  \
    ((((_addr) >= s_flash_efs_start_addr   ) && ((_addr) <= s_flash_efs_end_addr))\
     ||(((_addr) >= s_flash_fixed_nvitem_addr) && ((_addr) <= s_flash_fixed_nvitem_end_addr  ))\
     ||(((_addr) >= s_flash_product_info_addr) && ((_addr) <= (s_flash_product_info_addr + s_flash_sector_size)))\
     ||(((_addr) >= s_flash_umem_addr ) && ( (_addr) <= (s_flash_umem_addr+s_flash_umem_size)))\
     ||(((_addr) >= s_flash_xfile_addr ) && ( (_addr) <= s_flash_xfile_end_addr)) \
     ||(((_addr) >= s_flash_auto_sms_addr ) && ( (_addr) <= s_flash_auto_sms_end_addr)) \
     ||(((_addr) >= s_flash_operator_data_addr) && ( (_addr) <= s_flash_operator_data_end_addr)) \
     ||(((_addr) >= s_flash_crystal_aging_addr) && ( (_addr) <= s_flash_crystal_aging_end_addr)) \
     ||(((_addr) >= s_flash_apn_addr) && ( (_addr) <= s_flash_apn_end_addr)) \
     ||(((_addr) >= s_flash_cfg_nv_addr) && ( (_addr) <= s_flash_cfg_nv_end_addr)) \
     ||(((_addr) >= s_flash_ims_toggle_cfg_addr) && ( (_addr) <= s_flash_ims_toggle_cfg_end_addr)) \
     ||(((_addr) >= s_flash_simlock_addr ) && ( (_addr) <= s_flash_simlock_end_addr)))
#endif
#endif

//FLASH_OPTR_STATUS_FAILED    : the optr failed
//FLASH_OPTR_STATUS_COMPLETED : the optr success
//FLASH_OPTR_STATUS_RUNNING   : the optr is running, need check it again.

#define FLASH_OPTR_STATUS_FAILED        (0)
#define FLASH_OPTR_STATUS_COMPLETED (1)
#define FLASH_OPTR_STATUS_RUNNING       (2)

//running optr status
#define FLASH_STORE_OPTR_MAGIC          (0x4D534F46)    // FOSM
#define FLASH_STORE_OPTR_PROGRAMING (0x464F5350)    // FOSP
#define FLASH_STORE_OPTR_ERASING        (0x464F5345)    // FOSE
#define FLASH_STORE_OPTR_SUSPEND        (0x464F5353)    // FOSS
#define FLASH_STORE_OPTR_IDLE           (0x0)

//store optr status
#define STORE_FLASH_OPTR(optr , ptr)     *ptr = optr;\
    *(ptr + 1) = (optr ^ FLASH_STORE_OPTR_MAGIC);
//get optr status
#define GET_FLASH_STORE_OPTR(ptr)       ((FLASH_STORE_OPTR_MAGIC == ((*ptr) ^ (*(ptr + 1)))) ? (*ptr) : 0)

// add error trace information
#define FLASH_OPTR_ERASE                (0xB7)
#define FLASH_OPTR_PROGRAM          (0xCF)
#define FLASH_OPTR_RESUME               (0xDE)
#define FLASH_OPTR_SUSPEND              (0xE3)

#define FLASH_INVALID_ADDR              ((uint32)(-1))
#define FLASH_INVALID_SECTOR            ((uint16)(-1))



#define FLASH_SAVE_TRACE_INFOR_INIT     flash_trace_infor.flag = 0x46544946  // FTIF

#define FLASH_SAVE_TRACE_INFOR_ERASE    flash_trace_infor.optr = FLASH_OPTR_ERASE
#define FLASH_SAVE_TRACE_INFOR_PROGRAM( _data ) \
    flash_trace_infor.optr = FLASH_OPTR_PROGRAM; \
    flash_trace_infor.data = _data

#define FLASH_SAVE_TRACE_INFOR_RESUME   flash_trace_infor.optr = FLASH_OPTR_RESUME
#define FLASH_SAVE_TRACE_INFOR_SUSPEND  flash_trace_infor.optr = FLASH_OPTR_SUSPEND

#define FLASH_SAVE_TRACE_INFOR_CHECK_STATUS\
    flash_trace_infor.address  = (volatile uint32)(addr_ptr);\
    flash_trace_infor.tick_1   = old_tick;                    \
    flash_trace_infor.tick_2   = cur_tick;                    \
    flash_trace_infor.max_tick = tick_count;                  \
    flash_trace_infor.status_1 = d;                           \
    flash_trace_infor.status_2 = tmp;                         \
    flash_trace_infor.content  = *addr_ptr

#define FLASH_DELAY_US(_x_)   TIMER_DELAY_US(_x_)

#define DFILE_BZP_FLAG       (0x01UL)
#define DFILE_FLAGS_MASK     (0x03UL)
#define DFILE_MAGIC_FIRST    (0x50415254UL)
#define DFILE_MAGIC_SECOND   (0x494D4147UL)
#define DFILE_MAGIC_DSP      (0x44535000UL)     //"DSP."
#define DFILE_MAGIC_END      (0x5441494CUL)
#define DFILE_INVAILD_ADDR   (~0x0UL)

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

//Flash type define
typedef VOLATILE uint16 *FLASH_PTR_TYPE;

//Flash ID enum
typedef enum
{
    FLASH_AMD = 0,
    FLASH_SST,
    FLASH_INTEL,
    FLASH_ST,
    FLASH_SAMSUNG,
    FLASH_OTHER
} FLASH_TYPE_E;

//Flash erasing status enum
typedef enum FLASH_ERASE_STATUS_TAG
{
    FLASH_ERASE_COMPLETED_S    = 0,     /* erase has completed  */
    FLASH_ERASE_PROCESSING_S      ,     /* erase is processing  */
    FLASH_ERASE_SUSPEND_S         ,     /* erase suspend        */
    FLASH_ERASE_FAILED_S                /* erase failed         */
} FLASH_ERASE_STATUS_E;

//NOR_MCP config  for flash/sram connect to emc
typedef struct NORMCP_CONFIG_s
{
    EMC_CS_NUM_E                cs_num;
    EMC_MODE_CFG_T          cs_mode;
} NORMCP_CONFIG_T, *NORMCP_CONFIG_PTR;

#define SPIFLASH_VOLTAGE_1V8     0
#define SPIFLASH_VOLTAGE_3V     1

//#define SFC_ADDRESS_CYCLE_3    2
//#define SFC_ADDRESS_CYCLE_4    3
#define SPIFLASH_3_ADDR_CYCLE_MAX_REGION   0x1000000//address cycle only support max address 0x1000000
#define SPIFLASH_3_ADDR_CYCLE_MASK   0xFFFFFF
#define SPIFLASH_4_ADDR_CYCLE_MASK   0x1FFFFFF

typedef struct SPIFLASH_ExtCfg
{
    int voltage;
    uint32 desity;
    uint32 reserved1;
    uint32 reserved2;
    uint32 reserved3;
    uint32 reserved4;
    uint32 reserved5;
}SPIFLASH_ExtCfg_T, *Spiflash_ExtCfg_PRT;

//Flash Struct Configuration for platform application
typedef struct  nor_flash_config_s
{
    uint32     bank_num;
    uint32     sect_num;
    uint32     file_sect_num;
    uint32     sect_size;
    uint32     start_addr;
    uint32     efs_start_addr;
    uint32     flash_size;
    uint32     fixnv_addr;
    uint32     prodinfo_addr;
    uint32     mmi_res;
    uint32     mmi_res_size;
    uint32     umem_addr;
    uint32     umem_size;
    uint32     spload_addr;//sensor and lcd bin load base address
    uint32     simlock_addr;     //simlock Start Addr
#ifdef NOR_BOOTLOADER_SUPPORT
    uint32     ps_addr;     //AP Start Addr
#endif
     uint32     operator_data_addr;
     uint32     operator_data_size;
     uint32     cp_addr;
     uint32     cp_size;

     uint32     config_nv_addr;
     uint32     config_nv_size;
     uint32     crystal_aging_addr;
     uint32     crystal_aging_size;
     uint32     ims_toggel_cfg_addr;
     uint32     ims_toggel_cfg_size;
	//uint32	fota_bootloader_addr;
#if defined(FOTA_SUPPORT) && defined(FOTA_SUPPORT_CMIOT)
    uint32  fota_addr;
    uint32  fota_size;
    uint32  fota_bootloader_addr;
    uint32  fota_bootloader_size;
#endif
} NOR_FLASH_CONFIG_T,* NOR_FLASH_CONFIG_PTR;





//nor flash operate function
typedef struct nor_flash_operate_s
{
    void (*_init) (void);
    uint16 (*_readword) (uint32 addr);
    BOOLEAN (*_read) (uint32 addr, uint8 *buf, uint32 read_len);
    BOOLEAN (*_writeword) (uint32 addr, uint16 data);
    BOOLEAN (*_write) (uint32 addr, const uint8 *buf, uint32 len);
    BOOLEAN (*_erase) (uint32 addr);
    BOOLEAN (*_iserasecompleted) (void);
    void (*_suspend) (void);
    void (*_resume) (void);
    void (*_unlock) (uint32 addr);
    void (*_lock) (uint32 addr);
    void (*_enter_read) (void);
    void (*_exit_read) (void);
    void (*_close) (void);
    void (*_config) (NOR_FLASH_CONFIG_PTR,NORMCP_SPEC_PTR);
    /*start added by victorxu*/
    uint32 (*_addrconvert) (uint32 addr);
    /*end added by victorxu*/
} NOR_FLASH_DRIVER_T,* NOR_FLASH_DRIVER_PTR;

//NOR_MCP entity type contain some id and ptr
typedef struct normcp_entity_s
{
    NORMCP_SPEC_PTR        normcp_spec_ptr;
    NORMCP_CONFIG_PTR      normcp_config_ptr;

    NOR_FLASH_CONFIG_PTR   nor_flash_cfg_ptr;
} NORMCP_ENTITY_T,* NORMCP_ENTITY_PTR;

typedef struct
{
    volatile uint32  flag;      // So we can locate it if can't get map file.
    volatile uint32  address;   // The address that will be problem/erase.
    volatile uint32  tick_1;    // the start tick count
    volatile uint32  tick_2;    // the end tick count
    volatile uint32  max_tick;  // the max tick count
    volatile uint16  optr;      // see the FLASH_OPTR_????
    volatile uint16  data;      // the data will be write
    volatile uint16  content;   // the content of this address
    volatile uint16  status_1;  // the status read first time
    volatile uint16  status_2;  // the status read second time
} FLASH_TRACE_INFOR_T;

typedef struct DFILE_CONFIG_Tag
{
  uint32   magic_first;
  uint32   magic_second;
  uint32   image_addr;
  uint32   res_addr;
  uint32   nv_addr;
  uint32   dsp_addr;
  uint32   reserved2;
  uint32   ext[24];
  uint32   magic_end;
}DFILE_CONFIG_T;

/**---------------------------------------------------------------------------*
 **                    prototype                                              *
 **              flash initialization and infomation                          *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :
//     init flash config and get flash driver ptr.
//  Global resource dependence :
//      s_Normcp_spec_ptr s_Nor_Config_ptr s_Nor_Driver_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
BOOLEAN FLASH_Init (void); /* If Init success, return true, else return false */

/*****************************************************************************/
//  Description :
//      Close the flash device.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      This function should be called when power down.
/*****************************************************************************/
void FLASH_Close (void);

/*****************************************************************************/
//  Description :
//      Get sector size and sector number of the device.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_GetDeviceInfo (
    uint32   *sector_size_ptr,    /* output the sector size  */
    uint16   *sector_num_ptr,        /* output the sector number  */
    uint16   *file_system_sector_num_ptr);  // output the file system sector num.

/*****************************************************************************/
//  Description :
//      Convert address to sector number.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      sector no. 0 is the first sector we managered
/*****************************************************************************/
uint16  FLASH_AddrToSector (  /* the no. of the sector, If addr isn't in flash, return 0xFFFF  */
    uint32 addr);     /* physical address  that will be converted */

/*****************************************************************************/
//  Description :
//      Convert sector no. to physical address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
/*****************************************************************************/
uint32 FLASH_SectorToAddr (  /* start physical address of the sector */
    uint16 sector_no);   /* sector's no.                         */




/**---------------------------------------------------------------------------*
 **                    flash optr function                                    *
 **                                                                           *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :
//      Read the given count from device.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      the address MUST be word boundary.
/*****************************************************************************/
void FLASH_ReadWordByCount (uint32  addr, uint16 *w16_ptr, uint32  count);
/*****************************************************************************/
//  Description :
//      Read word(two byte) from the given address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//      the address MUST be word boundary.
/*****************************************************************************/
uint16 FLASH_ReadWord ( // The word that has read from the given address.
    uint32 addr);        // the address that will be read.
/*****************************************************************************/
//  Description :
//      The function reads up to size bytes from the device and stores them in buffer.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN FLASH_Read (     // If read successfully, return true, else false;
    uint32    addr,      // the start address that will be read
    uint8    *buf,       // store the data that being read to this buffer.
    uint32    read_len);  // the count of bytes that will be read.

/*****************************************************************************/
//  Description :
//    write a uint16 , the addr must be even.
//    we check first to avoid flash-reset,
//    if can't program( 0-->1 ), return false;
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/

BOOLEAN  FLASH_WriteWord (
    uint32      addr,
    uint16      data);

/*****************************************************************************/
//  Description :
//      The function write data to flash.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
//      We not verify the data that written.
//      If some bit can't convert from 0 to 1, the data that has written
//      is not correct.
/*****************************************************************************/
BOOLEAN FLASH_Write (   // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8       *buf,   // Pointer to the buffer containing the data to be written.
    uint32       len);   // number of bytes to write.

BOOLEAN mmi_FLASH_Write (   // If the data being written to flash, return TRUE.
    uint32       addr,  // the start address that will be written.
    const uint8       *buf,   // Pointer to the buffer containing the data to be written.
    uint32       len);   // number of bytes to write.
/*****************************************************************************/
//  Description :
//      Copy data from one area to another.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      Not check before write.
/*****************************************************************************/
BOOLEAN FLASH_Copy (
    uint32 dest,   /* the area we copy to   */
    uint32 sour,   /* the area we copy from */
    uint32  len);  /* the length we want copy*/

/*****************************************************************************/
//  Description :
//      Check can we write data to the area?
//      If some bits of the area from 0 to 1, the write wouldn't success,
//      and return false;
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      Not check before write.
//      This function called from EFS Task, so needn't  check erase status.
//
/*****************************************************************************/
BOOLEAN FLASH_CheckRevisable (
    uint32      addr,  // the start address of the area that will be checked.
    const uint8  *buf, // the buffer that contain the data.
    uint32      len);   // the data length.

/*****************************************************************************/
//  Description :
//      Is the last erase optr completed?
//
//  Global resource dependence :
//      None
//
//  Return :
//      If the erase has been completely, return true, other false
//  Author:
//      Lin.liu
//  Note:
//      This function MUST be called when system assert only,
/*****************************************************************************/
BOOLEAN FLASH_IsEraseCompleted (void);

/*****************************************************************************/
//  Description :
//      Erase the sector that contain the address.
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//      This will take a long time, typecal 1 second.
//      And this function must be called from a thread.
//
/*****************************************************************************/
BOOLEAN FLASH_Erase (
    uint32 addr);
/*****************************************************************************/
//  Description :
//      Suspend the program/erase operation
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Suspend (void);

/*****************************************************************************/
//  Description :
//      Resume the program/erase operation
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Resume (void);
/*****************************************************************************/
//  Description :
//      The function unlock the sector that contain the address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Unlock (uint32  addr);
/*****************************************************************************/
//  Description :
//      The function lock down the sector that contain the address.
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_Lock (uint32   addr);

/*****************************************************************************/
//  Description :
//      Get fixed nvitem address
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
void FLASH_GetFixedNvitemAddr (uint32 *start, uint32 *end);

/*****************************************************************************/
//  Description :
//      Check use SRAM for simulate ?
//
//  Global resource dependence :
//      None
//
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
BOOLEAN FLASH_IsSimulate (void);
/*****************************************************************************/
//  Description:    This function is used to get manufacture ID and device ID
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_ReadID (uint32 *pManu_id, uint32 *pDev_id, uint32 *pExtend_id);

/*****************************************************************************/
//  Description:    This function is used to get manufacture ID and device ID(already have been read)
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/
PUBLIC void FLASH_GetID (uint32 *pManu_id, uint32 *pDev_id, uint32 *pExtend_id);

/*****************************************************************************/
//  Description :
//  Get flash semaphore to make sure some flash operation be mutual exclusion
//
//  Global resource dependence :
//      s_flash_semaphore_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
void FLASH_GetSemaphore (void);
/*****************************************************************************/
//  Description :
//  Releas flash semaphore
//
//  Global resource dependence :
//      s_flash_semaphore_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
void FLASH_ReleaseSemaphore (void);
/*****************************************************************************/
//  Description:    This function is used to get flash info,
//                  and config flash struct.
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/
void FLASH_Config_struct (void);
/*****************************************************************************/
//  Description :
//      Set the flag if the erasing/programming sector be in code bank
//      .
//  Global resource dependence :
//      flash_bank_num
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
void FLASH_SetSuspendFlag (BOOLEAN flag);
/*****************************************************************************/
//  Description :
//      get the flag if the erasing/programming sector be in code bank
//      .
//  Global resource dependence :
//      flash_bank_num
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 FLASH_GetSuspendFlag (void);

/*****************************************************************************/
//  Description:    This function is used to get Flash end address.
//                  It equals FLASH_BASE_ADDR + FLASH_SIZE.
//  Global resource dependence:
//  Author:         Xueliang.Wang
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetEndAddr (void);

/*****************************************************************************/
//  Description :
//     init flash config and get flash driver ptr.
//  Global resource dependence :
//      s_Normcp_spec_ptr s_Nor_Config_ptr s_Nor_Driver_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 FLASH_SystemInit (void);
/*****************************************************************************/
//  Description :
//     init flash config and get flash driver ptr.
//  Global resource dependence :
//      s_Normcp_spec_ptr s_Nor_Config_ptr s_Nor_Driver_ptr
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
PUBLIC uint32 FLASH_InitHAL (NORMCP_ENTITY_PTR entity_ptr);

/*****************************************************************************/
//  Description :
//  get g_normcp_entity_table and entity count of the table
//  Global resource dependence :
//     None
//
//  Author:
//
//  Note:
//
/*****************************************************************************/
PUBLIC NORMCP_ENTITY_PTR NORMCP_GetEntityTab (uint32 *entity_cnt);

/*****************************************************************************/
//  Description :
//      Get the DFILE relative information of ps/res bin files.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the pointer for the relative information of ps/bin files
//  Author :
//  Note   : Only for the combination of the bin files
//
/*****************************************************************************/
PUBLIC DFILE_CONFIG_T*  SPIFLASH_GetDFileConfig(void);
/*****************************************************************************/
//  Description :
//      Get the DFILE relative information of cp/nv/UMEM/operator bin files.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the pointer for the relative information of cp/nv/UMEM/operator  files
//  Author :
//  Note   : Only for the combination of the bin files
//
/*****************************************************************************/
PUBLIC DFILE_CONFIG_T*  SPIFLASH_GetDFileConfigForFlashB(void);

/*****************************************************************************/
//  Description :
//      Get the XFILE flash address.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the XFILE flash address
//  Author :
//  Note   :
//
/*****************************************************************************/
PUBLIC uint32  SPIFLASH_GetXfileAddress(void);

/*****************************************************************************/
//  Description :
//      Get the XFILE flash area size.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the XFILE flash size
//  Author :
//  Note   :
//
/*****************************************************************************/
PUBLIC uint32  SPIFLASH_GetXfileSize(void);

/*****************************************************************************/
//  Description :
//      Get the AUTO SMS  flash area address.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the XFILE flash size
//  Author :
//  Note   :
//
/*****************************************************************************/
PUBLIC uint32  SPIFLASH_GetAutoSmsAddress(void);

/*****************************************************************************/
//  Description :
//      Get the AUTO SMS  flash area size.
//  Global resource dependence :
//  Input  : None
//  Output : None
//  Return : the XFILE flash size
//  Author :
//  Note   :
//
/*****************************************************************************/
PUBLIC uint32  SPIFLASH_GetAutoSmsSize(void);

/*****************************************************************************/
//  Description :
//  By manu_id,devc_id and extend_id,find correct g_normcp_entity_table item.
//  Global resource dependence :
//     None
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
NORMCP_ENTITY_PTR NORMCP_GetEntity (uint32 manu_id, uint32 devc_id, uint32 extend_id, uint32 *index);

/*****************************************************************************/
//  Description :
//  Get extend id to distinguish different flash with same manu_id and dev_id
//  Using CFI in autoselect mode.
//  Global resource dependence :
//     None
//
//  Author:
//      Younger.yang
//  Note:
//
/*****************************************************************************/
uint32 FLASH_GetExtID (uint32 manu_id,uint32 dev_id);

/*****************************************************************************/
//  Description:    This function is used to get
//                  flash product infomation address
//  Global resource dependence:
//  Author:         younger.yang
//  Note:
/*****************************************************************************/
uint32 FLASH_GetProductInfoAddr (void);

/*start added by victorxu*/
/*start added by feng tao*/
/*****************************************************************************/
//  Description :
//   get flash type, mainly determine  whether this flash is numonyx m18 flash or not
//
//  Global resource dependence :
//      None
//  Author:
//      Lin.liu
//  Note:
//
/*****************************************************************************/
PUBLIC BOOLEAN FLASH_M18_Type_Get (void);
/*end added by feng tao*/
/*end added by victorxu*/

/*****************************************************************************/
//  Description:    This function is used to get lcd and sensor load address
//	Global resource dependence:
//  s_flash_mmires_ptr
//  Author:         tao.feng
//	Note:
/*****************************************************************************/
uint32 FLASH_GetSperateLoadAddr(void);

/*****************************************************************************/
//  Description:    This function is used to get umem addree
//	Global resource dependence:
//  Author:         Zhou_Xiaobo
//	Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetUmemAddr(void);

/*****************************************************************************/
//  Description:    This function is used to get umem addree
//	Global resource dependence:
//  Author:         Zhou_Xiaobo
//	Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetUmemIdNum (void);
/*****************************************************************************/
//  Description:    This function is used to get umem addree
//	Global resource dependence:
//  Author:
//	Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetUmemVirSize (UMEM_TYPE_E index);

/*****************************************************************************/
//  Description:    This function is used to get umem size
//	Global resource dependence:
//  Author:         Zhou_Xiaobo
//	Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetUmemSize(void);

#ifdef DSP_DOWNLOAD_SUPPORT
/*****************************************************************************/
//  Description:    This function is used to get dsp bin address
//  Global resource dependence:
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint32 FLASH_GetDspCodeAddr (void);
#endif

unsigned int  _tx_thread_interrupt_control (unsigned int);

PUBLIC uint32 FLASH_GetProductInfoSize (void);

uint32 SPIFLASH_GetCrystalAgingAddress(void);
uint32 SPIFLASH_GetCrystalAgingSize(void);



/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //_FLASH_DRVAPI_H_
