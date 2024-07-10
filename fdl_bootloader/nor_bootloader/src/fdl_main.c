/******************************************************************************
 ** File Name:      BB_main.c                                                 *
 ** Author:         Leo.Feng                                                  *
 ** DATE:           27/06/2002                                                *
 ** Copyright:      2001 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 27/06/2002     Leo.Feng         Create.                                   *
 ** 11/12/2003     Zhemin.Lin       Enhanced according to cr6198              *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "fdl_main.h"
#include "bdl_crc.h"

#include "bdl_customif.h"
#include "sio_drv.h"
#include "usb_boot.h"
#include "virtual_com.h"
#include "kpd_reg_v5.h"
#include "fdl_stdio.h"
#include "flash_drv.h"
#include "sc_fdl_reg.h"

#include "flash.h"
#include "detect_error.h"
#include "update_fixnv.h"
//#include "umem_main.h"
//#include "umem_cfg.h"
#include "sfc_drvapi.h"

#ifdef FLASH_TYPE_SPI
#include "spiflash.h"
#endif

#include "fdl_sys.h"

#include "timer_ap.h"
#include "timer_drvapi.h"
#include "tf_cfg.h"
#include "cmddef.h"
#include "mem_cfg.h"
#include "serial.h"
#ifdef SECURE_BOOT_SUPPORT
#include "hal_rom_api.h"
#endif

#ifdef RESET_DUMP_SUPPORT
#include "reset_dump.h"
#endif

#ifndef FOTA_SUPPORT_NONE
#include "spiflash_cfg.c"
#ifdef FOTA_SUPPORT_CMIOT
#define FOTA_BOOTLOADER_RUN_ADDR 0x80010000
#else
#define FOTA_BOOTLOADER_RUN_ADDR 0x80000000
#endif
#endif

#ifdef FOTA_SUPPORT_CMIOT
struct cmiot_update_state
{
    unsigned char  inited;
    unsigned short update_result;
};
#endif


#include "slv_fw_lps_ifc.h"
#include "slv_fw_ap_ifc.h"
#include "slv_fw_ap_ahb.h"
#include "slv_fw_aon_ifc.h"
#include "slv_fw_aon_ahb.h"
#include "mem_fw_spiflash1.h"
#include "mem_fw_spiflash2.h"
#include "mem_fw_pub_mem.h"
#include "mem_fw_ap_imem.h"
#include "mem_fw_aon_imem.h"
//#include "hwreg_access.h"

#define SLAVE_ACCESS_NO (0)
#define SLAVE_ACCESS_SECURE_ONLY (1)
#define SLAVE_ACCESS_NONSECURE_ONLY (2)
#define SLAVE_ACCESS_SECURE_NONESECURE (3)

//KP_CTRL
#define KEYPAD_KP_EN               (1<<0)
#define KEYPAD_KP_DBN_TIME(n)      (((n)&0xFF)<<2)
#define KEYPAD_KP_ITV_TIME(n)      (((n)&0x3F)<<10)
#define KEYPAD_KP_IN_MASK(n)       (((n)&0xFF)<<16)
#define KEYPAD_KP_OUT_MASK(n)      (((n)&0xFF)<<24)

//KP_IRQ_CLR
#define KEYPAD_KP_IRQ_CLR          (1<<0)

typedef volatile struct
{
    VOLATILE uint32                          KP_DATA_L;                    //0x00000000
    VOLATILE uint32                          KP_DATA_H;                    //0x00000004
    VOLATILE uint32                          KP_STATUS;                    //0x00000008
    VOLATILE uint32                          KP_CTRL;                      //0x0000000C
    VOLATILE uint32                          KP_IRQ_MASK;                  //0x00000010
    VOLATILE uint32                          KP_IRQ_CAUSE;                 //0x00000014
    VOLATILE uint32                          KP_IRQ_CLR;                   //0x00000018
} HWP_KEYPAD_T;

#define hwp_keypad      ((HWP_KEYPAD_T*) (0x51706000))

#define REG32(x)                                    (*((volatile uint32 *)(x)))
/**---------------------------------------------------------------------------*
 **                         Function Propertype                               *
 **---------------------------------------------------------------------------*/
extern void JumpToTarget(int Start_Addr);
extern void get_flash_id( uint32 *pManufacturerID, uint32 *pDeviceID, uint32 * );
extern NORMCP_ENTITY_PTR NORMCP_GetEntity(uint32, uint32, uint32, uint32* );
extern uint32 get_valid_nv_address(void);
extern void initialize_nv_status(NOR_FLASH_CONFIG_PTR, uint32 base_address);
extern uint32 update_productinfo(char * prod_address,uint32 write_size);
extern uint32 update_fixnv(char * fix_nv_address,uint32 fix_nv_size);
extern unsigned long Get_CheckSum(const unsigned char *src, int len);
extern uint32 CHIP_PHY_GetHwRstFlag(void);
extern uint32 CHIP_PHY_Get7sRstFlag (void);
extern void CHIP_PHY_Clean7sRstFlag (void);
extern void Time_DelayUS(unsigned int us);

NOR_FLASH_CONFIG_PTR get_flash_partition_info_InPS(void);
uint16 calculate_fixNV_length( uint32  start_addr, uint32 end_addr);

extern uint32    s_intel_m18_serial;
extern uint32    g_spansion_vs_serial;

/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
#define	MAX_FLASH_COUNT	500
#define IS_VALID_CMD(cmd)	    ((cmd >= BSL_CMD_TYPE_MIN) && (cmd < BSL_CMD_TYPE_MAX))
#define MAX_SECARCH_RANGE       0x1000

#define PIN_FUNC_SHIFT          5
#define PIN_FUNC_MSK            (0x07<<PIN_FUNC_SHIFT)
#define ADI_ARMREG_ADDR_MASK    (0xFFF)     //[11:0]
#define ADI_ARMREG_FLAG_MASK    (0<<12)
#define BOOT_FLAG_USB           (0x5A)

#define FORCE_DOWNLOAD_MODE     (0x10000)
#define TF_UPDATE_MODE          (0x1000000)

//#ifdef SECURE_BOOT_SUPPORT
#if 0
#pragma arm section zidata = "VLR_DATA"
uint8 signature_data[512];
#pragma arm section zidata
#endif

typedef struct
{
    uint32   start_addr;  // == 0xFFFFFFFF means reach tail
    uint32   size;        // == 0
}FLASH_INFOR_T;

#if defined(PLATFORM_UWS6121E)

extern void rprintf(const char *fmt, ...);
//uint32 stack_space[STACK_LIMIT>>2];
const unsigned long stack_bottom = (PHY_IRAM_BASE_ADDR + MEM_IRAM_SIZE - 16);

#else

#if defined(PLATFORM_UWS6121E)
static const FLASH_INFOR_T  flash_infor[]=
{
    {0x00800000, 0x00040000},  //IRAM
    {0x60000000, 0x01000000},  // FLASH
    {0x80000000, 0x0200000},  // DDR or PSRAM
    {0xFFFFFFFF, 0x0}  // last one, indicate tail
};
#else
static const FLASH_INFOR_T  flash_infor[]=
{
    {0x00000000, 0x4000000},  // the max we support is 64M
    {0x04000000, 0x4000000},  // the max we support is 64M
    {0x08000000, 0x4000000},  // the max we support is 64M
    {0x0c000000, 0x4000000},  // the max we support is 64M
    {0xFFFFFFFF, 0x0}  // last one, indicate tail
};
#endif

const NOR_FLASH_DRIVER_T  g_Spansion_driver =
{
    NULL
};

const NOR_FLASH_DRIVER_T  g_Intel_driver =
{
    NULL
};

sys_stage_s      global_sys_stage;
sio_handle       global_sio_handle;
unsigned int global_uart_base = 0;
const char       version_string[] = {"Spreadtrum Boot Block version 1.2"};

LOCAL FDL_BOOT_MODE_E s_fdl_boot_mode 		= BOOT_MODE_UART;
LOCAL uint32 		  s_fdl_uart_addr_base	= ARM_UART1_BASE;

#define NOR_BOOTLOADER_LOGIC_ADRESS   0x80000000
#define NV_LOGIC_ADRESS               0x90000001
#define PS_LOGIC_ADRESS               0x80000003
#define RES_LOGIC_ADRESS              0x90000004
#define ERASEFLASH_LOGIC_ADRESS       0x90000003
#define PHASECHECK_LOGIC_ADRESS       0x90000002
#define UMEM_LOGIC_ERASE_ADRESS       0x90000005
#define UMEM_LOGIC_LOAD_ADRESS        0x90000006
#define RUNNING_NV_LOGIC_LOCAD_ADRESS 0x90000007
#define SPLOAD_LOGIC_LOAD_ADRESS      0x90000008
#ifdef DSP_DOWNLOAD_SUPPORT
#define DSP_LOGIC_LOAD_ADRESS         0x90000009
#endif

#if defined(PLATFORM_SC6531EFM)
#define FLASH_START_ADRESS            0x00000000
#define FLASH_START_ADRESS_CS3        0x0C000000
#ifdef SECURE_BOOT_SUPPORT
#ifdef FOTA_SUPPORT_NONE
#define FLASH_CFG_OFFSET              (0x10000+0x400+0x64)//0x64+0x39C		//Header added in kernel for secure boot
#else
#define FLASH_CFG_OFFSET              (0x20000+0x400+0x64)//0x64+0x39C		//Header added in kernel for secure boot
#endif
#else
#define FLASH_CFG_OFFSET              0x64
#endif
#elif defined(PLATFORM_UWS6121E)

#define FLASH_START_ADRESS                0x00000000
#define FLASH_START_ADRESS_CS3        0x1C000000
#ifdef SECURE_BOOT_SUPPORT
#define FLASH_CFG_OFFSET              (0x100+0x64)
#else
#define FLASH_CFG_OFFSET              0x64
#endif

#else

#define FLASH_START_ADRESS            0x00000000
#define FLASH_START_ADRESS_CS3        0x0C000000
#ifdef SECURE_BOOT_SUPPORT
#ifdef FOTA_SUPPORT_NONE
#define FLASH_CFG_OFFSET              (0x10000+0x400+0x60)//0x60+0x3A0		//Header added in kernel for secure boot
#else
#define FLASH_CFG_OFFSET              (0x20000+0x400+0x60)//0x60+0x3A0		//Header added in kernel for secure boot
#endif
#else
#define FLASH_CFG_OFFSET              0x60
#endif
#endif

//#ifdef SECURE_BOOT_SUPPORT
#if 0
#define NOR_BOOTLOADER_START_ADDR		0x0
#endif

#define CHECKSUM_OTHER_DATA           0x5555aaaa
#define ERR_NV_LEN                    0xffff
#define ITEM_MASK_ID            	  0x1fff
#define EFS_INVALID_ITEM              0xffff
#define UMEM_ERASE_FLAG               0x66
#define MAX_PATITION_NUM	          16
#define UMEM_IMG_VALID_FLAG           0xAABB
#define UMEM_IMG_INFO_SIZE            0x6

#define M18_REGION_SIZE                 1024

//@hyy for WriteIMEI Protect for TY
#define FLAG_NV_DOWN                  (BIT_0)
#define FLAG_PS_DOWN                  (BIT_1)
#define FLAG_RES_DOWN                 (BIT_2)
#define FLAG_FLASH_DOWN               (BIT_3)
#define FLAG_FDL_DOWN                 (BIT_7)
#define FLAG_ALL_OK                   (FLAG_FDL_DOWN|FLAG_NV_DOWN|FLAG_PS_DOWN|FLAG_RES_DOWN|FLAG_FLASH_DOWN)
#define FIX_NV_IMEI_FLAG_OFFSET       (4)
#define FIX_NV_IMEI_FLAG_MAGIC 	      (0x13E1)// 1ME1

static uint8  erase_buf[M18_REGION_SIZE] = {0x0};

static NORMCP_ENTITY_PTR   s_normcp_entity_ptr    = PNULL;
FLASH_PARTITION_PTR    s_flash_partition_info_ptr = PNULL;
static FLASH_PARTITION_T  s_flash_partition_info  = {0};
static BOOLEAN           s_is_dload_udisk_img     = SCI_FALSE;

#ifdef FLASH_TYPE_SPI
static const SPIFLASH_SPEC_T *pSpiFlashSpec[SFC_MAX_CS] = {NULL};
uint8   nID_MID[SFC_MAX_CS]={0}, nID_DID1[SFC_MAX_CS]={0}, nID_DID2[SFC_MAX_CS]={0};
uint32 prj_total_size = 0;
uint32 flash_cs0_size = 0;
uint32 flash_cs1_size = 0;
#endif

static uint32 s_share_fixnv_sector_flag = 0;
static uint32 s_flash_partition_info_table[MAX_PATITION_NUM] = {0};
static uint32 s_flash_partition_info_num           = 0;
static uint32 s_flash_partition_info_num_in_cfg    = 0;
uint32         g_checksum;
static uint32 g_sram_addr;
static uint32 g_umem_image_size;
uint32 g_write_imei_protect_flag = 0;

#define SRAM_UMEM_START_ADDR   0x04180000

uint32   g_is_product_sector= 0;
BOOLEAN  g_is_dload_phasecheck = FALSE;
BOOLEAN  g_is_dload_spload = FALSE;

uint32 g_prod_info_offset_addr = 0;
uint32 g_fdl_file_end_addr = 0;
//#define SRAM_BASE_ADDR          (0x04000000) //move to common.h
//#define SRAM_START_ADDR         (0x04100000) //move to common.h
#define FIX_NV_LEN_OFFSET       (16)
#define PRD_TAIL_INFO_LEN     	(8)
#define PRD_INFO_CRC_OFFSET	    (6)
#define	SECTOR_STATUS_OFFSET    (2)
#define	PRD_STATUS_OFFSET		(SECTOR_STATUS_OFFSET)
#define PROD_INFO_MIN_SIZE_4KB        (4096)
#define PROD_INFO_MIN_SIZE_512        (512)

#define	PROD_INFO_SIZE	        (8*1024)

#define FDL_INVALID_ADDR       0xFFFFFFFF
#define FDL_INVALID_SIZE       0xFFFFFFFF
#define TIMER_GET_USTICK()      REG32(0x0480681C)

#ifdef STONE_IMAGE_SUPPORT
#define   SINGLE_CS_MAX_FLASH_SIZE        (32*1024*1024)
#else
#define   SINGLE_CS_MAX_FLASH_SIZE        (0x1000000)
#endif

DL_STATUS  g_dl_status;
uint32 g_is_last_packet = 0;

char *rep_ack_ptr;
char *recv_data_buffer_ptr;
unsigned int rep_ack_array[MAX_PKT_SIZE_B>> 2];
unsigned int recv_data_buffer_array[SIO_RECV_LIMIT>> 2];       //Initialize the global variable

uint32 FDL_InEfsSpace(uint32 addr)
{
	return ((addr >=s_flash_partition_info_ptr->EfsAdress) && (addr < s_flash_partition_info_ptr->EfsAdress + s_flash_partition_info_ptr->EfsSize));
}

uint32 FDL_EfsAddrConvert(uint32 addr)
{
    uint32 temp_addr;
    if((addr >=s_flash_partition_info_ptr->EfsAdress) && (addr < s_flash_partition_info_ptr->EfsAdress + s_flash_partition_info_ptr->EfsSize))
    {
        if(addr & 0x10)
        {
            temp_addr = 0x10;
            addr = (addr+ s_flash_partition_info_ptr->EfsSize) & (~temp_addr);
        }
    }
    return addr;
}

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
PUBLIC int UsbLdoEnable (unsigned char is_usb_ldo_enabled)
{
	uint32 ret = 0, ret1 = 0;
    uint16 usb_ldo_pd, usb_ldo_rst;

    /*Purpose:To read USB ldo register*/
    REG_READ_ADI(ANA_LDO_PD_SET1,ret);
    REG_READ_ADI(ANA_LDO_PD_RST1, ret1);

    if ((-1 == ret || -1 == ret1))
        return -1;

    usb_ldo_pd = ret & 0xffff;
    usb_ldo_rst = ret1 & 0xffff;

    if (is_usb_ldo_enabled)
    {
        usb_ldo_pd &= ~(LDO_USB_PD);
        usb_ldo_rst |= (LDO_USB_PD_RST);

        /*Purpose:open USB digital ldo*/
        REG_AND(APB_MISC_CTL0, ~LDO_USB_PD_GLB);
    }
    else
    {
        /*Purpose:close USB digital ldo*/
        REG_OR(APB_MISC_CTL0, LDO_USB_PD_GLB);

        usb_ldo_pd |= (LDO_USB_PD);
        usb_ldo_rst &= ~(LDO_USB_PD_RST);
    }

    /*Purpose:To write USB LDO register*/
    REG_WRITE_ADI (ANA_LDO_PD_SET1, usb_ldo_pd);
    REG_WRITE_ADI (ANA_LDO_PD_RST1, usb_ldo_rst);
    return 0;
}
#endif

int get_ver_len (void)
{
    return sizeof (version_string);
}

PUBLIC BOOLEAN IsUsbDownloadMode (void)
{
    uint32 Value = 0;
    uint32 keyRawIntSts = 0;
    uint32 keySts = 0;

#if !defined(PLATFORM_UWS6121E)
    /*Purpose:To get the bonding vaule of usb download mode*/
    REG_READ(AHB_SYS_CTL6,Value);

    /*If strappin is set, goto USB Boot */
    if(Value & ((uint32)ARM_BOOT_MODE))
    {
        /* Delay 20ms for keypad stable*/
        fdl_delay (20);
        /*If key in0_out0 is pressed, goto USB Boot */
        REG_READ(KPD_INT_RAW_STATUS,keyRawIntSts);
        REG_READ(KEY_STATUS,keySts);
#if defined(PLATFORM_SC6531EFM)
        if(SC6531EFM == CHIP_GetChipType())
        {
            if(( keyRawIntSts & BIT_0) && ((keySts & KEY_STATUS_MASK) == 0x10))
            {
                return TRUE;
            }
        }
        else
        {
            if(( keyRawIntSts & BIT_0) && ((keySts & KEY_STATUS_MASK) == 0x00))
            {
                return TRUE;
            }
        }
#else
        if(( keyRawIntSts & BIT_0) && ((keySts & KEY_STATUS_MASK) == 0x00))
        {
            return TRUE;
        }
#endif
    }
    else
    {
    	return TRUE;
    }
#endif

    return FALSE;
}

PUBLIC void UsbDownload(void)
{
     uint32 i = 0;

     REG32(APB_MISC_CTL2) &= ~HWRST1_REG_MSK;
     REG32(APB_MISC_CTL2) |= (BOOT_FLAG_USB << HWRST1_REG_SHIFT);
    /* open 1.5k pullup resister */
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
     REG_OR(APB_MISC_CTL0, USB_1K5_EN);
#endif
     usb_boot (1);
}

PUBLIC void KeypadDisable ()
{
    /*Purpose:Clear Keypad interrupt */
#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
    REG_WRITE(KPD_INT_CLR, 0xFFF);
    /*Purpose:Disable KPD */
    REG_WRITE(APB_EB0_CLR, KPD_EB_CLR);
#else
    REG_WRITE(KPD_INT_CLR, 0xFFF);
    REG_WRITE(APB_EB0_CLR, KPD_EB_CLR);
#endif
}

/*----------------------------------------------------------------------------*
**                         Function Definitions                               *
**---------------------------------------------------------------------------*/
// For 3204, from 0x3000_0000 to 0x3040_0000
// For 6408/6416  0x3000_0000 to 0x3080_0000
// For HTW, there are two flash. one is from 0x3000_0000 to 0x3080_0000
//                               the other is from 0x3400_0000 to 0x3480_0000
//
static int check_start_address (uint32 start_addr)
{
    uint32  index = 0, end_addr;

    while (0xFFFFFFFF != flash_infor[index].start_addr)
    {
        end_addr = flash_infor[index].start_addr + flash_infor[index].size;

        if ( (flash_infor[index].start_addr <= start_addr)
                && (end_addr > start_addr))
        {
            return 1;
        }

        index++;
    }

    return 0;
}

static int check_access_size (uint32 start_addr, uint32  access_size)
{
    uint32  index = 0, end_addr;

    while (0xFFFFFFFF != flash_infor[index].start_addr)
    {
        end_addr = flash_infor[index].start_addr + flash_infor[index].size;

        if ( (flash_infor[index].start_addr <= start_addr)
                && (flash_infor[index].size >= access_size)
                && (end_addr >= (start_addr + access_size)))
        {
            return 1;
        }

        index++;
    }

    return 0;
}

//
// Get the start address of the flash device
//
uint32 get_flash_base_addr (uint32 start_addr)
{
    uint32  index = 0, end_addr;

    while (0xFFFFFFFF != flash_infor[index].start_addr)
    {
        end_addr = flash_infor[index].start_addr + flash_infor[index].size;

        if ( (flash_infor[index].start_addr <= start_addr)
                && (end_addr > start_addr))
        {
            return flash_infor[index].start_addr;
        }

        index++;
    }

    return 0;
}


void handle_error (void)
{
    while (1)
    {
        SIO_TRACE( "some error" );
    }
}


LOCAL void _FDL_SetMcuClock (void)
{

}

LOCAL void reset_mcu_clock (void)
{
    _FDL_SetMcuClock();
}

/**---------------------------------------------------------------------------*
 ** The Follow defines the packet processed result table                      *
 ** packet_protocol:                                                          *
 ** HDLC_FLAG   PKT_TYPE     DATALENGHT    [DATA]    CRC       HDLC_FLAG      *
 **   0x7E      MAX:255      MAX:65536      ...      ...          0x7E        *
 **    1           2             2           0        2             1         *
 ** response_packet:                                                           *
 **   0x7E      MAX:255          0           --      ...          0x7E        *
 **    1           2             2           --       2             1         *
 ** response packet length: 8 bytes
 **---------------------------------------------------------------------------*/

#pragma arm section zidata = "ZIDATA_AREA_0"
uint32 stack_space[STACK_LIMIT>>2];
#pragma arm section zidata

 const unsigned long stack_bottom = ((unsigned long)stack_space + STACK_LIMIT);


/**---------------------------------------------------------------------------*
 ** The Follow defines the packet processed function entry table              *
 **---------------------------------------------------------------------------*/
DLSTATUS DL_Begin( PACKET_T * packet_ptr )
{
    sys_connect(packet_ptr);
    data_start(packet_ptr);
    return DL_SUCCESS;
}

DLSTATUS DL_Transfer( PACKET_T * packet_ptr )
{
    g_is_last_packet = 0;
    data_midst(packet_ptr);
   return DL_SUCCESS;
}

DLSTATUS DL_End( PACKET_T * packet_ptr )
{
   g_is_last_packet = 1;
   data_midst(packet_ptr);
   return DL_SUCCESS;
}

DLSTATUS DL_Exec( PACKET_T * packet_ptr )
{
    data_exec(packet_ptr);
	return DL_SUCCESS;
}

const  pkt_proc_tab proc_tab[] =
{
    {CMD_DL_BEGIN,      DL_Begin},
    {CMD_DL_DATA,       DL_Transfer},
    {CMD_DL_END,        DL_End},
    {CMD_RUN_GSMSW,     DL_Exec},
};

pkt_proc_fun  FDL_GetCmdFunc(cmd_pkt_type cmd)
{
    uint32 t = 0;
    pkt_proc_fun  proc_func = NULL;
    for(t = 0; t < sizeof(proc_tab)/sizeof(pkt_proc_tab); t++)
    {
       if(cmd == proc_tab[t].cmd_type)
       {
            proc_func = proc_tab[t].pkt_proc;
            break;
       }
    }

   return proc_func;
}
#pragma arm section code="CHIP_SETTING"

#define PSRAM_START_ADDRESS 0x04000000

void _fdl_psram_init(void)
{
#if defined (CHIP_VER_6531)
    CHIP_PHY_MCU_CLK_Init();

    // SMC burst config
    {
        MMU_DisableIDCM();

        EMC_Init();

        MMU_EnableIDCM();
    }
#else
	uint32 i = 0;
	uint32 tmp = 0;
	uint32 crI = 0;
	uint32 crII = 0;

    //MMU_DisableIDCM();

    //add for reconfig after ss-boot
    {
        REG32(0x20000000) = 0x22220000;
		REG32(0x20000004) = 0x00000000;

		REG32(0x20000020) = 0x00000000;
		REG32(0x20000024) = 0x00924FF0;
		REG32(0x20000028) = 0x0151FFFF;
		REG32(0x2000002C) = 0x00A0744F;

        for(i = 0; i < 100; i++){}
    }

    REG32(0x20000000) = 0x222211E0;
    REG32(0x20000004) = 0x00008080;

    for(i = 0; i < 100; i++){}

    // pSRAM burst config
    crI = 0x8191F;      // BCR: variable latency
    crII = 0x10;        // RCR

    // external memory config
    REG32(0x20000024) |= BIT_17;   //CRE eb
    *(volatile uint16 *)(PSRAM_START_ADDRESS + (crI << 1))  = 0;
    for(i = 0; i < 10; i++){}
    *(volatile uint16 *)(PSRAM_START_ADDRESS + (crII<<1))   = 0;
    for(i = 0; i < 10; i++){}
    REG32(0x20000024) &= (~BIT_17);

    for(i = 0; i < 10; i++){}

    // EMC config
    REG32(0x20000024) = 0x00AC1FFF;
    REG32(0x20000028) = 0x015115FF;
    REG32(0x2000002c) = 0x00501015;

    REG32(0x20000000) = 0x222210E0;
    REG32(0x20000004) = 0x00008080;

    for(i = 0; i < 100; i++)
    {
    	tmp = REG32(PSRAM_START_ADDRESS);
    }

    //MMU_EnableIDCM();
#endif
}

void _fdl_flash_init(void)
{
    Spiflash_ExtCfg_PRT flash_ext_cfg;
    int cs = 0;

#if defined(PLATFORM_SC6531EFM)
    REG32(0x20A00204) |= 1;//enable cs0 interrupt
    REG32(0x20A0000C) = 1; //clear sfc int
#elif !defined(PLATFORM_UWS6121E)
    REG32(0x20A00204) |= 3;//enable cs0/cs1 interrupt
    REG32(0x20A0000C) = 3; //clear sfc int
#endif

    /* spiflash cs0/ initial */
    cs = SPIFLASH_CS0;
    SFCDRV_CSCfg(cs);
    SPIFLASH_ResetQPIFlash(cs);

    SPIFLASH_ReadID(cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

    if((nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
    {
        SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);//no device
        return;
    }
    else
    {
        SIO_TRACE("cs0 ID:0x%x 0x%x 0x%x", nID_MID[0], nID_DID1[0], nID_DID2[0]);
    }

    /* set cs1 start address register before spiflash_cs1 initial */
    pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get(nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);


    if( NULL ==  pSpiFlashSpec[cs])
    {
        SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
        return;
    }

#if defined(PLATFORM_SC6531EFM) || defined(PLATFORM_UWS6121E)
#ifdef FPGA_VERIFICATION
    REG32(0x205000E0) = 0x1;
    SPIFLASH_Init(cs);
    REG32(0x205000E0) = 0x0;
#else
    SPIFLASH_Init(cs);
#endif
#else
    SPIFLASH_Init(cs);
#endif

    prj_total_size = pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

    flash_ext_cfg = (Spiflash_ExtCfg_PRT)pSpiFlashSpec[cs]->pExtcfg;
    flash_cs0_size = flash_ext_cfg->desity;

    SFCDRV_SetCS1StartAddr(flash_cs0_size);
    SPIFLASH_SetCS0DesityFlag(nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs0_size);

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    if (SFC_MAX_CS - 1)
    // dual cs spiflash are used
    {
        /* spiflash cs1s initial */
        cs = SPIFLASH_CS1;
        SFCDRV_CSCfg(cs);
        SPIFLASH_ResetQPIFlash(cs);

        SPIFLASH_ReadID(cs, &nID_MID[cs], &nID_DID1[cs], &nID_DID2[cs]);

        if((nID_MID[cs] == 0) && (nID_DID1[cs] == 0) && (nID_DID2[cs] == 0))
        {
            //maybe cs1 has no device!
            return;
        }
        else
        {
            SIO_TRACE("cs1 ID:0x%x 0x%x 0x%x", nID_MID[1], nID_DID1[1], nID_DID2[1]);
        }

        pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get(nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);

        if(pSpiFlashSpec[cs] != NULL)
        {
            SPIFLASH_Init(cs);

            flash_ext_cfg = (Spiflash_ExtCfg_PRT)pSpiFlashSpec[cs]->pExtcfg;
            flash_cs1_size = flash_ext_cfg->desity;

            SPIFLASH_SetCS1DesityFlag(nID_MID[cs], nID_DID1[cs], nID_DID2[cs], flash_cs1_size);
        }
        else
        {
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
            return;
        }
    }
#endif

    SIO_TRACE("flash_cs0_size=0x%x, flash_cs1_size=0x%x, prj_total_size=0x%x",
        flash_cs0_size, flash_cs1_size, prj_total_size);
}

void KeypadConfig (void)
{
    int i;

    /*Purpose:Enable RTC and KPD */
    REG_WRITE(APB_EB0_SET, KPD_EB_SET|KPD_RTC_EB_SET);

    /*Purpose:To reset */
    REG_WRITE(APB_RST0_SET, KPD_SOFT_RST_SET);

    /* Delay some time */
    for (i = 0; i < 10; i++);

    /*Purpose:To reset */
    REG_WRITE(APB_RST0_CLR, KPD_SOFT_RST_CLR);

    /*Purpose:Clear Keypad interrupt  */
    REG_WRITE(KPD_INT_CLR, 0xFFF);

    /*Purpose:Set Keypad row colomn polarity */
    REG_WRITE(KPD_POLARITY, 0xFFFF);

    /*Purpose:Set Keypad clock div */
    REG_WRITE(KPD_CLK_DIVIDE_CNT, 0);

    /*Purpose:Set Keypad debounce time */
    REG_WRITE(KPD_DEBOUNCE_CNT, 0XF);

    /*Purpose:Enable Keypad module*/
    REG_OR(KPD_CTRL, BIT_0);
}

void _FDL_HWInit(void)
{
	uint32 i = 0;

#if defined(CHIP_VER_6531EFM)
    REG32(APB_EB0_SET)  = 0xFFFFFFDE;

    //add keypad config to clear key status and key interrupt.
    KeypadConfig();
#elif !defined(PLATFORM_UWS6121E)
    REG32(0x20000000)   = 0x22220000;
    REG32(APB_EB0_SET)  = 0xFFFFFFDE;

    /*sys clock set to 208Mhz*/
    i = REG32 (APB_CLK_CFG0);
    i &= ~0x3;
    i |= 0x01;     //sel clk_mcu 208MHz
    REG32(APB_CLK_CFG0) = i;
#endif
    for(i = 0; i < 500; i++);

#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
    /*Purpose:to select uart1 clock source 26M*/
    REG32(APB_CLK_CFG1) |= CLK_UART1_SEL_MCU;

    /* pSRAM burst config*/
    _fdl_psram_init();
#endif

#if defined(PLATFORM_SC6531EFM)
    CHIP_LDOSFPDEnAfterRst();
#endif

    /*spi flash init*/
    // TODO
#if !defined(PLATFORM_UWS6121E)
    _fdl_flash_init();
#endif
}
#pragma arm section code

#define  SYNC_WORD        0xB5
#define  SYNC_WORD_RSP    0x5B

#define MAX_SYNC_WAITING_TIME   300

//#ifdef SECURE_BOOT_SUPPORT
#if 0
uint32 download_secure_check(uint8* secure_code_ptr, uint8* fdl_vlr_ptr)
{
    uint8  *puuk_ptr = (uint8*)(NOR_BOOTLOADER_START_ADDR + RSAPUK_OFF);
    if(CheckSecureBootState())
    {
    	return harshVerify((uint8 *)(secure_code_ptr), 0, \
			(uint8 *)fdl_vlr_ptr, (uint8 *)(puuk_ptr));
    }
	else
	{
    	return 1;
	}
}
#endif

//#ifdef SECURE_BOOT_SUPPORT
#if 0
volatile int loop_var2 = 0;
uint32 tick_count_before_verify_kernel = 0;
uint32 tick_count_after_verify_kernel = 0;
#endif

void _fdl_nml_boot(void)
{

//#ifdef SECURE_BOOT_SUPPORT
#if 0
    uint8  *ptr	           = (uint8*)(NOR_BOOTLOADER_START_ADDR);
    uint8	*memAddr = (uint8*)(SYS_MODEM_PROGRAM_START_ADDR - SECURE_CODE_OFF);		//Secure Header added on kernel for secure boot feature
#endif
    uint32 i = 0;

//#ifdef SECURE_BOOT_SUPPORT
#if 0
    tick_count_before_verify_kernel = fdl_GetTickCount();
    SIO_TRACE("_fdl_nml_boot: verify start tick=%d\r\n", tick_count_before_verify_kernel);
    memcpy(signature_data, (uint8 *)(memAddr + VLR_INFO_OFF), VLR_INFO_SIZ);
    if(CheckSecureBootState())
    {
		if(0 == harshVerify((uint8 *)(memAddr + SECURE_CODE_OFF), 0, \
		(uint8 *)signature_data, (uint8 *)(ptr + RSAPUK_OFF)))//PUK + Signed part
		{
				loop_var2 = 1;
				while(loop_var2 == 1)
				{
					;//Do Nothing
				}
		}
    }
    tick_count_after_verify_kernel = fdl_GetTickCount();
    SIO_TRACE("_fdl_nml_boot: verify end tick=%d\r\n", tick_count_after_verify_kernel);
#endif

    SIO_TRACE("Execute Program: Addr=0x%x\r\n", global_sys_stage.data .start_addr);

#if defined(PLATFORM_SC6531EFM)
    REG32(APB_RST0_SET) = (MCU_SHM_SOFT_RST_SET);
    for (i = 0; i < 100; i++);
    REG32(APB_RST0_CLR) = (MCU_SHM_SOFT_RST_SET);
    REG32(APB_EB0_CLR)  = 0xFFFFFFFF;
#elif !defined(PLATFORM_UWS6121E)
    REG32(APB_RST0_SET) = (MCU_SOFT_RST_SET | MCU_SHM_SOFT_RST_SET);
    for (i = 0; i < 100; i++);
    REG32(APB_RST0_CLR) = (MCU_SOFT_RST_SET | MCU_SHM_SOFT_RST_SET);
    REG32(APB_EB0_CLR)  = 0xFFFFFFFF;
#endif

    MMU_DisableIDCM();

#if !defined(PLATFORM_UWS6121E)
    SPIFLASH_ResetQPIFlash(0);
#if !defined(PLATFORM_SC6531EFM)
    SPIFLASH_ResetQPIFlash(1);
#endif
#endif

#if defined(PLATFORM_UWS6121E)
    JumpToTarget(0x60010000);
#else
    JumpToTarget(SYS_MODEM_PROGRAM_START_ADDR);
#endif
}

#if !defined(PLATFORM_UWS6121E)
void _fdl_rsp_sync_word(void)
{
    while (!SIO_TX_READY (SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr()))){};

    SIO_PUT_CHAR(FDL_GetUARTBaseAddr(), SYNC_WORD_RSP);

    while (!SIO_TX_READY (SIO_GET_TX_STATUS(FDL_GetUARTBaseAddr()))){};
}
#endif

#ifdef FOTA_SUPPORT_REDSTONE
#include "rsplatform.h"
extern rs_u32 rs_trace(const char *format, ...);
extern void rs_ua_execute(uint32 upaddr, uint32 deltaAddr);

void rs_update_agent()
{
	FLASH_PARTITION_T  flash_partition_info = {0};

	rs_trace("Enter rsfota agent!\r\n");
	if (!get_flash_partition_info(&flash_partition_info))
	{
		send_ack(BSL_UNKNOWN_DEVICE);
		rs_trace("rsua init partition error!\r\n");
		return ;
	}

	rs_trace("start fota at TickCount:0x%x\r\n",SCI_GetTickCount());
	rs_trace("ps addr is 0x%x, delta addr is 0x%x\r\n", flash_partition_info.PsAdress, flash_partition_info.Spload_Addr);
	rs_ua_execute(flash_partition_info.PsAdress, flash_partition_info.Spload_Addr);
	rs_trace("exit fota at TickCount:0x%x\r\n",SCI_GetTickCount());
}
#endif
#endif

//#ifdef SECURE_BOOT_SUPPORT
#if 0
static uint32 secure_boot_check(uint8* secure_code_ptr, uint8* fdl_vlr_ptr)
{
#define CONFIG_SRAM_START               0x00800000
#define CONFIG_TRAP_SIZE                0xc0
#define CONFIG_LOAD_ADDRESS             (CONFIG_SRAM_START + CONFIG_TRAP_SIZE)
#define FDL_SIGNED_SIZE 	(0xbce0)
#define NOR_BOOTLOADER_PUBKEY_ADDR (CONFIG_LOAD_ADDRESS + FDL_SIGNED_SIZE - KEY_SIZE)
    uint8  *puuk_ptr = (uint8*)(NOR_BOOTLOADER_PUBKEY_ADDR);
    if(CheckSecureBootState())
    {
    	return harshVerify((uint8 *)(secure_code_ptr), 0, \
			(uint8 *)fdl_vlr_ptr, (uint8 *)(puuk_ptr));
    }
    else
    {
        return 1;
    }
}
#endif

void reset_to_force_download(void)
{
    uint32 val = REG32(0x50100114);
    rprintf("Reset Cause1: 0x%x\n", val);

    val |= 1 << 16; /* set force download mode */
    REG32(0x50100114) = val;
    rprintf("Reset Cause2: 0x%x\n", REG32(0x50100114));

    //while (1);

    REG32(0x5009029c) = 0x0;    /* SLOW_SYS_CLK_SEL_HWEN */
    REG32(0x500902A0) = 0x0;    /* SLOW_CLK_SEL_HWEN */
    REG32(0x50090034) = 0x0;    /* LTEM1_FRAME_INTEN */
    /* RESET MCU CMD */
    REG32(0x50100000) = 0x00A50001;
    REG32(0x501001D4) = 0x00667FEB;
    //REG32(0x50100114) = 0x00000001;
    REG32(0x50100044) = 0x00000010;
}

PUBLIC uint32 Scaning_Keypad(void)
{
    uint16 i = 0;

    rprintf("init keypad...\n");

    hwp_keypad->KP_CTRL = KEYPAD_KP_DBN_TIME(2) \
                          | KEYPAD_KP_ITV_TIME(2) \
                          | KEYPAD_KP_EN \
                          | KEYPAD_KP_IN_MASK(0x1F) \
                          | KEYPAD_KP_OUT_MASK(0x1F);
    hwp_keypad->KP_IRQ_CLR = KEYPAD_KP_IRQ_CLR;
    hwp_keypad->KP_IRQ_MASK = 0;

    for (i = 0; i < 20; i++) {
        Time_DelayUS(100);
        rprintf("wait keypad stable\n");
    }

    return hwp_keypad->KP_DATA_L;
}

void mdelay(uint32 ms)
{
    uint32 i = 0;

    for (i = 0; i < ms; i++)
        Time_DelayUS(1000);
}

void vector_set(void)
{
    ESAL_TS_RTE_CP_WRITE(p15, 0, 0x50800100, c12, c0, 0); //配置向量表起始地址
    ESAL_TS_RTE_CP_WRITE(p15, 0, 0x50800100, c12, c0, 1);

}

//ADI_API
#define ADI_FIFO_IS_EMPTY   (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_EMPTY)
#define ADI_FIFO_IS_FULL    (hwp_adiMst->arm_cmd_status & ADI_MST_WFIFO_FULL)

#define ANA_REG_GET(reg_addr)           ADI_Analogdie_reg_read(reg_addr)

#define ANA_REG_AND(reg_addr, value)  ADI_Analogdie_reg_and(reg_addr, value)

#define ANA_REG_OR(reg_addr, value)  ADI_Analogdie_reg_or(reg_addr, value)

PUBLIC BOOLEAN ADI_IS_Analogdie_reg (uint32 addr)
{
    if ( (addr >= ANA_REG_ADDR_START) && (addr <= ANA_REG_ADDR_END))
    {
        return SCI_TRUE;
    }
    else
    {
        return SCI_FALSE;
    }
}

PUBLIC uint16 ADI_Analogdie_reg_read (uint32 addr)
{
#if defined(PLATFORM_UWS6121E)
    uint32 adi_rd_data;
    uint32 rd_addr = 0;

    // Set read address
    hwp_adiMst->arm_rd_cmd = ADI_MST_ARM_RD_CMD(addr);

    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    do
    {
        adi_rd_data = hwp_adiMst->arm_rd_data;
    }
    while(adi_rd_data & ADI_MST_ARM_RD_CMD_BUSY);

    // rd_data high part should be the address of the last read operation
    rd_addr = (adi_rd_data >> 16) & 0x7FFF;
    if(rd_addr != addr >> 2)
    {
        rprintf("high 15bits rd_data (0x%x) != (0x%x)addr\n",adi_rd_data,addr);/*assert verified*/
    }

    return((uint16)(adi_rd_data & ADI_MST_ARM_RD_CMD_X1(0xFFFF)));
#else
    uint32 cur_tick, next_tick;
    uint32 adi_rd_data;
    // Set read address
    *(volatile uint32*)ADI_ARM_RD_CMD = ((addr)&ADI_ARMREG_ADDR_MASK) | ADI_ARMREG_FLAG_MASK;

    // wait read operation complete, RD_data[31] will be cleared after the read operation complete
    cur_tick = next_tick = SCI_GetTickCount();
    do
    {
        adi_rd_data = *(volatile uint32*)ADI_ARM_RD_DATA;

        next_tick = SCI_GetTickCount();
    }
    while(adi_rd_data & ADI_RD_CMD_BUSY_MASK);

    return((uint16)(adi_rd_data & ADI_ARM_RD_DATA_MASK));
#endif
}

PUBLIC  void ADI_Analogdie_reg_write (uint32 addr, uint16 data)
{
#if defined(PLATFORM_UWS6121E)
    for(;;)//wait until if these have one empty
    {
        if(ADI_FIFO_IS_FULL == 0)
        {
            break;
        }
    }

    REG32(REG_ADI_MST_BASE + 0x8000 + addr) = (uint32)data;

    // wait write fifo empty after adi analog write
    while(ADI_FIFO_IS_EMPTY == 0)//wait until all data have been sent
    {
    }
#else
    uint32 cur_tick, next_tick;

    cur_tick = next_tick = SCI_GetTickCount();

    for(;;)              ////ADI_wait_fifo_empty
    {
        if(ADI_FIFO_IS_FULL == 0)
        {
            break;
        }
    }

    *(volatile uint32*)addr = (uint32)data;
#endif
}

PUBLIC  void ADI_Analogdie_reg_and (uint32 addr, uint16 data)
{
    uint16 adi_tmp_val;
    uint32 adi_rd_data;


#if defined(FPGA_VERIFICATION)
    return ;
#endif
    /* ============read============== */
    adi_tmp_val = ADI_Analogdie_reg_read(addr);

    /* ===========Modify(and) value==== */
    adi_tmp_val &= data;

    /* ==============write============= */
    ADI_Analogdie_reg_write(addr, adi_tmp_val);
}

PUBLIC  void ADI_Analogdie_reg_mask_or (uint32 addr, uint16 data, uint16 msk)
{
    uint16 adi_tmp_val;
    uint32 adi_rd_data;


#if defined(FPGA_VERIFICATION)
    return ;
#endif
    /* ============read============== */
    adi_tmp_val = ADI_Analogdie_reg_read(addr);

    /* ===========Modify(mask or) value==== */
    adi_tmp_val &= (uint16)(~(msk)); \
    adi_tmp_val |= (uint16)((data)&(msk)); \

    /* ==============write============= */
    ADI_Analogdie_reg_write(addr, adi_tmp_val);
}

PUBLIC  void ADI_Analogdie_reg_or (uint32 addr, uint16 data)
{
    uint16 adi_tmp_val;
    uint32 adi_rd_data;


#if defined(FPGA_VERIFICATION)
    return ;
#endif
    /* ============read============== */
    adi_tmp_val = ADI_Analogdie_reg_read(addr);

    /* ===========Modify(or) value==== */
    adi_tmp_val |= data;

    /* ==============write============= */
    ADI_Analogdie_reg_write(addr, adi_tmp_val);
}


static void bootSetFirewall()
{

    // unset firewall set by sysrom
    hwp_memFwSpiflash1->seg_0_first_addr = 0xffffffff;
    hwp_memFwSpiflash1->seg_0_last_addr = 0;
    hwp_memFwAonImem->seg_0_first_addr = 0xffffffff;
    hwp_memFwAonImem->seg_0_last_addr = 0;
    hwp_memFwApImem->seg_0_first_addr = 0xffffffff;
    hwp_memFwApImem->seg_0_last_addr = 0;
    hwp_slvFwApAhb->id0_first_addr_0 = 0xffffffff;
    hwp_slvFwApAhb->id0_last_addr_0 = 0;
    hwp_slvFwLpsIfc->id0_first_addr_0 = 0xffffffff;
    hwp_slvFwLpsIfc->id0_last_addr_0 = 0;
    hwp_slvFwApIfc->id0_first_addr_0 = 0xffffffff;
    hwp_slvFwApIfc->id0_last_addr_0 = 0;


#if 0
    // unset flash,uart,keypad, usb firewall
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->rd_sec_1, REG_SLV_FW_AP_AHB_RD_SEC_1_T,
                      spiflash1_reg_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->wr_sec_1, REG_SLV_FW_AP_AHB_WR_SEC_1_T,
                      spiflash1_reg_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->rd_sec_1, REG_SLV_FW_AP_AHB_RD_SEC_1_T,
                      med_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->wr_sec_1, REG_SLV_FW_AP_AHB_WR_SEC_1_T,
                      med_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE1(hwp_slvFwLpsIfc->rd_sec_0, REG_SLV_FW_LPS_IFC_RD_SEC_0_T,
                      keypad_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwLpsIfc->wr_sec_0, REG_SLV_FW_LPS_IFC_WR_SEC_0_T,
                      keypad_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE1(hwp_slvFwApIfc->rd_sec_0, REG_SLV_FW_AP_IFC_RD_SEC_0_T,
                      uart4_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwApIfc->wr_sec_0, REG_SLV_FW_AP_IFC_WR_SEC_0_T,
                      uart4_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE1(hwp_slvFwLpsIfc->rd_sec_0, REG_SLV_FW_LPS_IFC_RD_SEC_0_T,
                      uart1_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwLpsIfc->wr_sec_0, REG_SLV_FW_LPS_IFC_WR_SEC_0_T,
                      uart1_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->rd_sec_1, REG_SLV_FW_AP_AHB_RD_SEC_1_T,
                      usb_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->wr_sec_1, REG_SLV_FW_AP_AHB_WR_SEC_1_T,
                      usb_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    // unset emmc firewall
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->rd_sec_0, REG_SLV_FW_AP_AHB_RD_SEC_0_T,
                      emmc_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->wr_sec_0, REG_SLV_FW_AP_AHB_WR_SEC_0_T,
                      emmc_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);

    REGT_FIELD_CHANGE1(hwp_mstCtrlAp->rd_sec_0, REG_MST_CTRL_AP_RD_SEC_0_T,
                      emmc_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_mstCtrlAp->wr_sec_0, REG_MST_CTRL_AP_WR_SEC_0_T,
                      emmc_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->rd_sec_1, REG_SLV_FW_AP_AHB_RD_SEC_1_T,
                      ce_sec_rd_sec, SLAVE_ACCESS_SECURE_NONESECURE);
    REGT_FIELD_CHANGE1(hwp_slvFwApAhb->wr_sec_1, REG_SLV_FW_AP_AHB_WR_SEC_1_T,
                      ce_sec_wr_sec, SLAVE_ACCESS_SECURE_NONESECURE);
#endif
}

/*typedef enum uart_instance
{
	uart1_instance = 1,
	uart2_instance,
	uart3_instance,
	uart4_instance,
}uart_instance_t;*/

int main(void)
{
    extern uint32  g_fdl_spiflash_disable_do_idle;
    extern uint32  g_fdl_spiflash_disable_qpi;
    extern uint32 _GPIO_GetValue(uint32);

    PACKET_T * packet_ptr = PNULL;
    FDL_BOOT_MODE_E boot_mode;
    uint32 cur_time = 0;
    uint32 key_val = 0;
    uint32 reset_mode = 0;

	serial_init(uart1_instance);

    rprintf("nor_bootloader\n");
   bootSetFirewall();
#if defined(PLATFORM_UWS6121E)
    ADI_Phy_init();
    ANA_REG_OR(ANA_POWER_PD_SW0, LDO_VIO33_PD);
    key_val = Scaning_Keypad();
    reset_mode = CHIP_PHY_GetHwRstFlag();
    rprintf("reset_mode =  0x%x\n", reset_mode);
#if !defined(CHIP_VER_UWS6121E)
    if (key_val == FORCE_DOWNLOAD_MODE)
    {
        reset_to_force_download();
    }
#endif

    rprintf("key_val: 0x%x\n", key_val);
    rprintf("reset_mode: %d\n", reset_mode);

    rprintf("boot end ANA_POR_7S_CTRL:0x%x\n",ANA_REG_GET(ANA_POR_7S_CTRL));

#ifdef RESET_DUMP_SUPPORT
    if (CHIP_PHY_Get7sRstFlag())
    {
        DumpChannel reset_action = CHANNEL_NO_RESET;

        rprintf("boot1 enter 7s reset dump mode\n");
        CHIP_PHY_Clean7sRstFlag(); //clera hw flag

        reset_action = After7sResetActionType();
        rprintf("after 7s reset_action:%d\n",reset_action);

        After7sResetActionStart(reset_action);

        while (1);
    }
#endif

#ifdef FOTA_SUPPORT_ABUP
	if (FOTA_RESET_MODE != reset_mode)
	{
		extern const NOR_FLASH_CONFIG_T s_platform_patitiion_config;
		uint8 dest_str[4] = {0};
		uint32 start_addr = s_platform_patitiion_config.abup_fota_app_addr + s_platform_patitiion_config.abup_fota_app_size - s_platform_patitiion_config.sect_size;

		memcpy((void *)dest_str, (void *)start_addr, 4);
		rprintf("abup update flag:0x%x0x%x0x%x0x%x\n", dest_str[0], dest_str[1], dest_str[2], dest_str[3]);
		if (dest_str[0] == 'a' && dest_str[1] == 'b' && dest_str[2] == 'u' && dest_str[3] == 'p')
		{
			reset_mode = FOTA_RESET_MODE;
		}
	}
#endif

#ifdef FOTA_SUPPORT_CMIOT
    if (FOTA_RESET_MODE != reset_mode)
    {
        extern const NOR_FLASH_CONFIG_T s_platform_patitiion_config;
        struct cmiot_update_state staus;
        uint32 start_addr = s_platform_patitiion_config.fota_addr + s_platform_patitiion_config.fota_size - s_platform_patitiion_config.sect_size;
        memcpy((void *)(&staus), (void *)start_addr, sizeof(struct cmiot_update_state));
        if (staus.update_result == 100)
        {
            reset_mode = FOTA_RESET_MODE;
            rprintf("cmiot fota: set FOTA_RESET_MODE \r\n");
        }
    }
#endif

    if ((key_val & TF_UPDATE_MODE) || (UPDATE_RESET_MODE == reset_mode))
    {
        rprintf("TF Update Mode, load tfboot.bin ...\n");
        TF_Load(TFLOAD_DEFAULT_DIR);
        while (1);
    }
#ifndef FOTA_SUPPORT_NONE
    else if (FOTA_RESET_MODE == reset_mode)
    {
        rprintf("FOTA Update Mode, load fota_bootloader.bin ...\n");
	 memcpy((uint8 *)FOTA_BOOTLOADER_RUN_ADDR, (uint8 *)FOTA_BOOTLOADER_ADDR, FOTA_BOOTLOADER_SIZE);
	 JumpToTarget(FOTA_BOOTLOADER_RUN_ADDR);
	 while (1);
    }
#endif
    else
    {

#define SYS_PROGRAM_START_ADDR (IMAGE_ENTRY_ADDR)
#define SECURE_CODE_OFF  0x100

#ifdef SECURE_BOOT_SUPPORT
       bootGetEfuseCtx();
    	if (!bootSimageCheckSign((const simageHeader_t *)SYS_PROGRAM_START_ADDR))
    	{
		rprintf("secure boot verify failed\n");
	}
	else
	{
            rprintf("Verify Success Bootloader Jump to 0x%08x ...\n",SYS_PROGRAM_START_ADDR + SECURE_CODE_OFF);
            JumpToTarget(SYS_PROGRAM_START_ADDR + SECURE_CODE_OFF);
	}
#else
      rprintf("Bootloader Jump to 0x%08x...\n", SYS_PROGRAM_START_ADDR);
      JumpToTarget(SYS_PROGRAM_START_ADDR + SECURE_CODE_OFF);
#endif
    }
#else //#if defined PLATFORM_UWS6121E

    g_fdl_spiflash_disable_do_idle = TRUE;
    g_fdl_spiflash_disable_qpi = TRUE;

#if defined(PLATFORM_SC6531EFM) && defined(MODEM_PLATFORM)
    ADI_init();
    ANA_REG_OR (ANA_POR_7S_CTRL,BIT_1); //disable 7s reset
#endif

    MMU_Init_ext();
    _FDL_HWInit();

    ClearUartFifo();
    if (IsUsbDownloadMode ())
    {
        FDL_memset(&global_sys_stage, 0, sizeof(sys_stage_s));
        global_sys_stage.data.stage = SYS_STAGE_NONE;
        rep_ack_ptr = NULL;
        recv_data_buffer_ptr = NULL;
        KeypadDisable ();
        UsbDownload();
        while (1);
    }
    else if (UartIdentify ())
    {
        FDL_memset(&global_sys_stage, 0, sizeof(sys_stage_s));
        global_sys_stage.data.stage = SYS_STAGE_NONE;
        rep_ack_ptr = NULL;
        recv_data_buffer_ptr = NULL;
        KeypadDisable ();
        UartDownload ();
        while (1);
    }

#if 1

    gUart0Channel.Open(&gUart0Channel, 115200);
    gUart1Channel.Open(&gUart1Channel, 115200);

#ifndef FOTA_SUPPORT_NONE
    FDL_memset(&global_sys_stage, 0, sizeof(sys_stage_s));
    global_sys_stage.data.stage = SYS_STAGE_NONE;
    rep_ack_ptr = NULL;
    recv_data_buffer_ptr = NULL;
    KeypadDisable ();

    REG32(APB_MISC_CTL2) &= ~HWRST1_REG_MSK;
    REG32(APB_MISC_CTL2) |= (BOOT_FLAG_USB << HWRST1_REG_SHIFT);
    /* open 1.5k pullup resister */
#if !defined(PLATFORM_SC6531EFM)
    REG_OR(APB_MISC_CTL0, USB_1K5_EN);
#endif
#endif

#ifdef FOTA_SUPPORT_REDSTONE
    rs_usb_init();
    rs_trace("rs_fdl_nml_boot run!\r\n");
    rs_update_agent();

#endif

    _fdl_nml_boot();

#else
    packet_init( );

    gUart0Channel.Open(&gUart0Channel, 115200);
    gUart1Channel.Open(&gUart1Channel, 115200);
    SIO_TRACE("SIO SUCCESS!\r\n");

    /*Check the Reset GPIO Pin Status is High or Low, if High go on, others loop here*/
#if defined(MODEM_TYPE_ZBX)
    GPIO_GetAPRstBBValueInit();
    do
    {
        SIO_TRACE("Waiting Sync..............\r\n");
    }while(SCI_FALSE == GPIO_GetAPRstBBValue());
#endif

    cur_time = fdl_GetTickCount();
    while(1)
    {

        while (!(*(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_STS1)&0xff))
        {
            if( fdl_GetTickCount() > (cur_time + MAX_SYNC_WAITING_TIME))
            {

               SIO_TRACE("Waiting Sync Timeout,Begin Run Program\r\n");
                //Wait Timeout,then enter Modem program
                _fdl_nml_boot();
            }
        }

        if(SYNC_WORD ==  (*(volatile uint32*)(FDL_GetUARTBaseAddr()+ARM_UART_RXD)&0xff))
        {
        	// clear uart tx fifo
            while((*(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_STS1)&0xff))
            {
                uint32 value;
                value = *(volatile uint32*)(FDL_GetUARTBaseAddr() +ARM_UART_RXD);
            }

            SIO_TRACE("Received Sync Word\r\n");
            _fdl_rsp_sync_word();
            break;
        }
    }

    // send true adress of nv,ps,res...to pc
    s_flash_partition_info_ptr = &s_flash_partition_info;

    get_flash_partition_info(s_flash_partition_info_ptr);

    //update_sector_by_slave();
    //g_write_imei_protect_flag = FLAG_FDL_DOWN;
    process_packets();
#endif
#endif

    return 1;
}

#if !defined(PLATFORM_UWS6121E)
ret_status process_packets()
{
    cmd_pkt_type cmd;
    PACKET_T    *packet_ptr;
    DLSTATUS  ret;
    pkt_proc_fun cmd_func = NULL;
    g_dl_status.stage = DL_STAGE_DL_BEGIN;
    g_dl_status.data_verify = 0;
    g_dl_status.dl_packet_id = 0;
    FDL_memset(&global_sys_stage, 0, sizeof(global_sys_stage));

    while( 1 )
    {
        //receive packet
        ret = receive_packet(&packet_ptr);

        if(DL_SUCCESS != ret)
        {
            Send_Respond(g_dl_status.stage, ret, 0);
            free_packet(packet_ptr);
            SIO_TRACE("receive_packet error:%d\r\n", ret);
            continue;
        }

        cmd = packet_ptr->packet_body.type;

        /* Check the validity of the command. */
        if (!IS_VALID_CMD(cmd))
        {
            Send_Respond(cmd, DL_PACKET_ERROR, 0);
            free_packet(packet_ptr);
            SIO_TRACE("cmd error:%d\r\n", cmd);
            continue;
        }


		SIO_TRACE("Received Command:cmd:%d, stage:%d\r\n", cmd, g_dl_status.stage);
		switch (g_dl_status.stage)
		{
    		case DL_STAGE_DL_BEGIN:
    		{
                if (CMD_DL_BEGIN != cmd)
                {
                    Send_Respond(CMD_DL_BEGIN_RSP,DL_PACKET_ERROR, 0);
                    break;
                }
                cmd_func = FDL_GetCmdFunc(cmd);
                ret =  cmd_func(packet_ptr );
                if (DL_SUCCESS == ret)
                {
                    g_dl_status.stage = DL_STAGE_DATA_TRANSFER;
                    g_dl_status.dl_packet_id = 0;
                    Send_Respond(CMD_DL_BEGIN_RSP, DL_SUCCESS, MAX_MTU_SIZE);
                }
                else
                {
                    SIO_TRACE("DL_STAGE_DL_BEGIN:Error:%d\r\n",ret);
                    Send_Respond(CMD_DL_BEGIN_RSP, ret, 0);
                }
                break;
        	}
        	case DL_STAGE_DATA_TRANSFER:
        	{
                if ((CMD_DL_DATA != cmd) && (CMD_DL_END != cmd))
                {
                    Send_Respond(CMD_DL_DATA_RSP, DL_PACKET_ERROR, 0);
                    break;
                }
                cmd_func = FDL_GetCmdFunc(cmd);
                ret =  cmd_func(packet_ptr);
                if (DL_SUCCESS  == ret)
                {
                	if (CMD_DL_END == cmd)
                	{
                        g_dl_status.stage = DL_STAGE_RUN_GSMSW;
                        g_dl_status.data_verify = 1;
                        Send_Respond(CMD_DL_END_RSP, DL_SUCCESS, 0);
                	}
                	else
                	{
                        g_dl_status.dl_packet_id++;
                        Send_Respond(CMD_DL_DATA_RSP, DL_SUCCESS, g_dl_status.dl_packet_id);
                	}
                }
                else
                {
                    SIO_TRACE("DL_STAGE_DATA_TRANSFER:Error:ret:%d, id=0x%X\r\n",ret, g_dl_status.dl_packet_id);
                    Send_Respond(CMD_DL_DATA_RSP, ret, g_dl_status.dl_packet_id);
                }
                break;
        	}
        	case DL_STAGE_RUN_GSMSW:
        	{
                cmd_func = FDL_GetCmdFunc(cmd);
                ret =  cmd_func(packet_ptr );
                Send_Respond(CMD_RUN_GSMSW_RSP, ret, 0);
                break;
        	}
        	default: /* DL_STAGE_CONNECTED */

            	break;
        } /* switch */

        free_packet( packet_ptr );
    }

    return BSL_PHONE_SUCCEED;
}

void  send_ack( cmd_pkt_type pkt_type )
{
    return;
    //send_ack_packet( pkt_type );
}

DLSTATUS sys_connect( PACKET_T * packet_ptr )
{
     if(SYS_STAGE_CONNECTED != global_sys_stage.data.stage)
     {
      //  Send_Respond(CMD_DL_BEGIN_RSP, DL_SUCCESS, MAX_PKT_SIZE);
        global_sys_stage.data.stage = SYS_STAGE_CONNECTED;
     }

     return  DL_SUCCESS;
}

#define MAX_IMAGE_SIZE      0x0400000

DLSTATUS data_start( PACKET_T * packet_ptr )
{
    unsigned long  start_addr;
    unsigned long  file_size;

    g_is_dload_phasecheck = FALSE;

    if( (global_sys_stage.data .stage != SYS_STAGE_CONNECTED) &&
        (global_sys_stage.data .stage != SYS_STAGE_END) )
    {
        SEND_ERROR_RSP(BSL_REP_DOWN_MULTI_START);
        return BSL_REP_DOWN_MULTI_START;
    }

#ifdef FLASH_TYPE_SPI
    {
        uint32 cs = SPIFLASH_CS0;
        if(NULL == pSpiFlashSpec[cs])
        {
            //unknown device
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
            return BSL_UNKNOWN_DEVICE;
        }

        cs = SPIFLASH_CS1;
        if((Is_FlashID_Valid(nID_MID[cs], nID_DID1[cs], nID_DID2[cs])) &&
            (NULL == pSpiFlashSpec[cs]))
        {
            //unknown device
            SEND_ERROR_RSP (BSL_UNKNOWN_DEVICE);
            return BSL_UNKNOWN_DEVICE;
        }

        if(prj_total_size > (flash_cs0_size + flash_cs1_size))
        {
            //total size error.
            SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
            return BSL_UNKNOWN_DEVICE;
        }
    }
#endif
    /* analyse the packet*/
    start_addr     = PS_LOGIC_ADRESS;
    file_size      = MAX_IMAGE_SIZE;

    if( ( NV_LOGIC_ADRESS == start_addr) || ( PS_LOGIC_ADRESS ==  start_addr)
        || ( RES_LOGIC_ADRESS == start_addr)
        || ( PHASECHECK_LOGIC_ADRESS ==  start_addr) )
    {
    //s_flash_partition_info_ptr = &s_flash_partition_info;

    	if (!get_flash_partition_info(s_flash_partition_info_ptr))
    	{
        	send_ack(BSL_UNKNOWN_DEVICE);
        	return  BSL_UNKNOWN_DEVICE;
    	}
    }

    if( NV_LOGIC_ADRESS == start_addr)
    	{
    		start_addr = s_flash_partition_info_ptr->NvAdress;
        if(file_size > s_flash_partition_info_ptr->NvSize)
        {
            SEND_ERROR_RSP(BSL_REP_DOWN_SIZE_ERROR);
            return BSL_REP_DOWN_SIZE_ERROR;
        }
    }
    else if( PS_LOGIC_ADRESS ==  start_addr)
    {
        start_addr = s_flash_partition_info_ptr->PsAdress;
#ifdef SECURE_BOOT_SUPPORT
        if(((SYS_MODEM_PROGRAM_START_ADDR - SECURE_CODE_OFF) != start_addr) ||
        	 (file_size < s_flash_partition_info_ptr->PSSize))
#else
        if((SYS_MODEM_PROGRAM_START_ADDR != start_addr) ||
        	 (file_size < s_flash_partition_info_ptr->PSSize))
#endif
        {
			SIO_TRACE("Program Start Addr Should Be 0x%X, PsAddr=0x%X\r\n",
                SYS_MODEM_PROGRAM_START_ADDR, start_addr);
            SEND_ERROR_RSP(BSL_REP_DOWN_SIZE_ERROR);
			handle_error();
            return BSL_REP_DOWN_SIZE_ERROR;
        }
    }
    else if( RES_LOGIC_ADRESS == start_addr)
	{
		start_addr = s_flash_partition_info_ptr->ResAdress;
        if(file_size > s_flash_partition_info_ptr->ResSize)
        {
            SEND_ERROR_RSP(BSL_REP_DOWN_SIZE_ERROR);
            return BSL_REP_DOWN_SIZE_ERROR;
        }
	}
    else if( PHASECHECK_LOGIC_ADRESS ==  start_addr)
	{
	    g_is_dload_phasecheck = TRUE;
		start_addr = s_flash_partition_info_ptr->ProdinfoAdress;
	}

    if( !check_start_address( start_addr ) )
    {
        SEND_ERROR_RSP(BSL_REP_DOWN_DEST_ERROR);
        return BSL_REP_DOWN_DEST_ERROR;
    }
    if( !check_access_size( start_addr, file_size ) )
    {
        SEND_ERROR_RSP(BSL_REP_DOWN_SIZE_ERROR);
        return BSL_REP_DOWN_SIZE_ERROR;
    }

    /* check the data download stage*/
    global_sys_stage.data .start_addr  = start_addr;
    global_sys_stage.data .write_addr  = global_sys_stage.data .start_addr ;
    global_sys_stage.data .file_size   = file_size;
    global_sys_stage.data .recv_size   = 0;
    global_sys_stage.data .stage       = SYS_STAGE_START;
    global_sys_stage.data_verify       = SYS_STAGE_NONE;
    g_fdl_file_end_addr = start_addr + file_size;

    //Enable Write Flash
	FDL_FlashWriteEnable( get_flash_base_addr( start_addr ), 1 );

    // send a ack immediatelly to allow PC to send next packet.
    if( !packet_ptr->ack_flag )
    {
        packet_ptr->ack_flag = 1;

    	send_ack(BSL_REP_ACK);
    }

    return DL_SUCCESS;
}
#define MAX_DOWNLOAD_CACHE_SIZE  1024*1024
//uint8*  s_download_cache = (uint8*)SRAM_START_ADDR;
#pragma arm section zidata = "ZIDATA_AREA_4"
uint8   s_download_cache[MAX_DOWNLOAD_CACHE_SIZE];
#pragma arm section zidata
uint32  s_cache_read_pos = 0;
uint32  s_cache_write_pos = 0;
uint32  s_cache_count = 0;

DLSTATUS data_midst( PACKET_T * packet_ptr )
{
	unsigned short     data_len;
	CMD_DL_DATA_T*     dl_cmd_ptr  = NULL;
    uint8*             dl_data_ptr = NULL;
	uint32  sector_size, j, slave_size;

    if( (global_sys_stage.data .stage != SYS_STAGE_START) &&
        (global_sys_stage.data .stage != SYS_STAGE_GATHER) )
    {
        SEND_ERROR_RSP(BSL_REP_DOWN_NOT_START);
        return BSL_REP_DOWN_NOT_START;
    }

    // analyse the packet
    dl_cmd_ptr =  (CMD_DL_DATA_T *)packet_ptr->packet_body.content;
    dl_data_ptr = (uint8*)packet_ptr->packet_body.content + sizeof(CMD_DL_DATA_T);
    data_len     = packet_ptr->packet_body.size - sizeof(CMD_DL_DATA_T);

    if(dl_cmd_ptr->packet_id != g_dl_status.dl_packet_id)
    {
        return DL_PACKET_ERROR;
    }

    if( (global_sys_stage.data .recv_size + data_len) > global_sys_stage.data .file_size )
    {
        SEND_ERROR_RSP(BSL_REP_DOWN_SIZE_ERROR);
        while(1);
    }

    // send a ack immediatelly to allow PC to send next packet.
    if( (!packet_ptr->ack_flag) && (g_is_dload_phasecheck == FALSE))
    {
        packet_ptr->ack_flag = 1;

    	send_ack(BSL_REP_ACK);
    }

	if( (s_cache_write_pos + data_len) <= MAX_DOWNLOAD_CACHE_SIZE)
	{
	    FDL_memcpy(&s_download_cache[s_cache_write_pos], dl_data_ptr, data_len);
	}
	else
	{
	    FDL_memcpy(&s_download_cache[s_cache_write_pos], dl_data_ptr,
            MAX_DOWNLOAD_CACHE_SIZE - s_cache_write_pos);

        FDL_memcpy(&s_download_cache[0],
            &dl_data_ptr[MAX_DOWNLOAD_CACHE_SIZE - s_cache_write_pos],
            s_cache_write_pos + data_len - MAX_DOWNLOAD_CACHE_SIZE);
	}

	s_cache_write_pos += data_len;

	if(s_cache_write_pos >= MAX_DOWNLOAD_CACHE_SIZE)
	{
	   s_cache_write_pos -= MAX_DOWNLOAD_CACHE_SIZE;
	}

    s_cache_count     += data_len;

    if( g_is_last_packet ||
        ((s_cache_count + data_len) >=
        (MAX_DOWNLOAD_CACHE_SIZE - (MAX_MTU_SIZE << 1))))
	{
        if((s_cache_read_pos + s_cache_count) < MAX_DOWNLOAD_CACHE_SIZE)
        {
            if(FDL_FlashWriteArea(global_sys_stage.data .write_addr,
                (char *)&s_download_cache[s_cache_read_pos], s_cache_count))
            {
                global_sys_stage.data .write_addr
                    = global_sys_stage.data .write_addr + s_cache_count;
                global_sys_stage.data .recv_size
                    = global_sys_stage.data .recv_size + s_cache_count;

                s_cache_read_pos += s_cache_count;

                if(s_cache_read_pos >= MAX_DOWNLOAD_CACHE_SIZE)
                {
                    s_cache_read_pos -= MAX_DOWNLOAD_CACHE_SIZE;
                }
            }
            else
            {
                 SEND_ERROR_RSP(BSL_REP_DOWN_DEST_ERROR);
                 handle_error();
            }

        }
        else
        {
            if(FDL_FlashWriteArea(global_sys_stage.data .write_addr,
                (char *)&s_download_cache[s_cache_read_pos],
                MAX_DOWNLOAD_CACHE_SIZE - s_cache_read_pos))
            {
                global_sys_stage.data .write_addr
                    = (global_sys_stage.data .write_addr
                        + MAX_DOWNLOAD_CACHE_SIZE - s_cache_read_pos);
                global_sys_stage.data .recv_size
                    = (global_sys_stage.data .recv_size
                        + MAX_DOWNLOAD_CACHE_SIZE - s_cache_read_pos);
            }
            else
            {
                 SEND_ERROR_RSP(BSL_REP_DOWN_DEST_ERROR);
                 handle_error();
            }

            s_cache_read_pos
                = s_cache_read_pos + s_cache_count - MAX_DOWNLOAD_CACHE_SIZE;

            if(FDL_FlashWriteArea(global_sys_stage.data .write_addr,
                (char *)&s_download_cache[0], s_cache_read_pos))
            {
                global_sys_stage.data .write_addr
                    = global_sys_stage.data .write_addr + s_cache_read_pos;
                global_sys_stage.data .recv_size
                    = global_sys_stage.data .recv_size + s_cache_read_pos;
            }
            else
            {
                 SEND_ERROR_RSP(BSL_REP_DOWN_DEST_ERROR);
                 handle_error();
            }
        }

        s_cache_count = 0;
	}

    return DL_SUCCESS;
}

#ifdef SECURE_BOOT_SUPPORT
volatile uint32 loop_var3=0;
#endif

DLSTATUS data_end( PACKET_T * packet_ptr )
{
    uint8    *image_state_ptr=PNULL;
    uint8     set_size_buf[2];
    uint16   byte_per_sec;
    uint8    udisk_img_flag[6];
    uint16  *image_mapper_ptr=PNULL,*image_erasenum_ptr=PNULL,*image_excption_state_ptr=PNULL;
    uint32 i, fix_nv_checksum,udisk_size,udisk_img_base_addr, udisk_addr;
    uint32 umem_vir_sec_num,umem_data_sec_num,umem_phy_sec_num,umem_ctrl_stru_size,umem_sec_size,image_vir_sec_num;
    //SEC_CTRL_T   *umem_info_ptr = PNULL;
    //UMEM_CUSTDEFINE_T umem_cust_info ;
#ifdef SECURE_BOOT_SUPPORT
    uint8	*memAddr;
#endif
    static uint32 fix_nv_size = 0xFFFFFFFF;

    if(!check_one_patition( global_sys_stage.data .start_addr,global_sys_stage.data .start_addr ))
    {
        SEND_ERROR_RSP (BSL_WRITE_ERROR);
        handle_error();
    }

    if (CHECKSUM_OTHER_DATA != g_checksum)
    {
        fix_nv_size = g_sram_addr - SRAM_START_ADDR;

        fix_nv_checksum = Get_CheckSum ( (uint8 *) SRAM_START_ADDR, fix_nv_size);

        if (fix_nv_checksum != g_checksum)
        {
            SEND_ERROR_RSP (BSL_CHECKSUM_DIFF);
        }

#if 0
        if(s_share_fixnv_sector_flag == 1)
        {
    		if(FDL_FlashCheckSectorSize(s_flash_partition_info_ptr->NvAdress) < fix_nv_size)
    		{
    			SEND_ERROR_RSP(BSL_WRITE_ERROR);
    			handle_error();
    		}

#ifndef FLASH_TYPE_SPI
        	memcpy((void *)(SRAM_START_ADDR + fix_nv_size),
        	    (void *)(s_flash_partition_info_ptr->NvAdress + fix_nv_size),
                FDL_FlashCheckSectorSize(s_flash_partition_info_ptr->NvAdress) - fix_nv_size);
#endif
        }
#endif

        for (i=0; i<3; i++)
        {
            uint32 ret = 0;
            ret = update_fixnv((char *)SRAM_START_ADDR, fix_nv_size);

    	    if(ret)
    			break;
    	    else
    	        FDL_SendTrace("ERROR! %s %d, ret=%d", __func__, __LINE__, ret);
        }

        if(i==3)
        {
           //Write error happened
           SEND_ERROR_RSP(BSL_WRITE_ERROR);
           handle_error();
        }

        g_write_imei_protect_flag |= FLAG_NV_DOWN;

    }
    else
    {
        if (g_is_dload_phasecheck == TRUE)
        {

            for (i=0; i<3; i++)
            {
                uint32 ret = 0;
                ret = update_fixnv((char *)SRAM_START_ADDR, fix_nv_size);

                if(ret)
                    break;
                else
                    FDL_SendTrace("ERROR! %s %d, ret=%d", __func__, __LINE__, ret);
            }

            if (i==3)
            {
               //Write error happened
               SEND_ERROR_RSP(BSL_WRITE_ERROR);
            }

            g_is_dload_phasecheck = FALSE;
        }

		//@hyy ps下结束了，判断FDL FLASH CFG与PS中的FLASH CFG是否一致
		if(global_sys_stage.data .start_addr == s_flash_partition_info_ptr->PsAdress)
		{
			if(0 != memcmp(pSpiFlashSpec[0]->spiflash_cfg, get_flash_partition_info_InPS(), sizeof(NOR_FLASH_CONFIG_T)))
            {
		        SEND_ERROR_RSP(BSL_FLASH_CFG_ERROR);

				handle_error();
			}

	 		g_write_imei_protect_flag |= FLAG_PS_DOWN;

#ifdef SECURE_BOOT_SUPPORT
		  memAddr = (uint8 *)s_flash_partition_info_ptr->PsAdress + SECURE_CODE_OFF;

		  memcpy(signature_data, (uint8 *)(memAddr - VLR_INFO_SIZ), VLR_INFO_SIZ);

		  if (0 == download_secure_check((uint8 *)memAddr, (uint8 *)signature_data))
		  {
			loop_var3 = 1;
			SEND_ERROR_RSP(BSL_REP_SECURITY_VERIFICATION_FAIL);
			while(loop_var3 == 1)
			{
			  ;//Do nothing.
			}
		  }
#endif

		}
#ifndef STONE_IMAGE_SUPPORT
		if(global_sys_stage.data .start_addr == s_flash_partition_info_ptr->ResAdress)
		{
	 		g_write_imei_protect_flag |= FLAG_RES_DOWN;
		}
#endif
        if(global_sys_stage.data .start_addr == s_flash_partition_info_ptr->PsAdress)
		{
            FDL_Mark_SPIFlash_Type();
        }
    }

    global_sys_stage.data .stage      = SYS_STAGE_END;
	global_sys_stage.data_verify      = BSL_PHONE_SUCCEED;

	// send a ack immediatelly to allow PC to send next packet.
	if( !packet_ptr->ack_flag )
	{
		packet_ptr->ack_flag = 1;

		send_ack(BSL_REP_ACK);
	}

    return DL_SUCCESS;
}

DLSTATUS data_exec( PACKET_T * packet_ptr )
{
#ifdef SECURE_BOOT_SUPPORT
    uint8  *ptr	           = (uint8*)(NOR_BOOTLOADER_START_ADDR);
    uint8	*memAddr = (uint8*)(SYS_MODEM_PROGRAM_START_ADDR - SECURE_CODE_OFF);		//Secure Header added on kernel for secure boot feature
#endif

    if(global_sys_stage.data_verify == BSL_REP_NOT_VERIFY)
    {
        send_ack(BSL_REP_NOT_VERIFY);
    }
    else
    {
#if 0//def SECURE_BOOT_SUPPORT
    memcpy(signature_data, (uint8 *)(memAddr + VLR_INFO_OFF), VLR_INFO_SIZ);
    if(CheckSecureBootState())
    {
		if(0 == harshVerify((uint8 *)(memAddr + SECURE_CODE_OFF), 0, \
		(uint8 *)signature_data, (uint8 *)(ptr + RSAPUK_OFF)))//PUK + Signed part
		{
				loop_var2 = 1;
				SEND_ERROR_RSP(BSL_REP_SECURITY_VERIFICATION_FAIL);
				while(loop_var2 == 1)
				{
					;//Do Nothing
				}
		}
    }
#endif

        send_ack(BSL_REP_ACK);

        SIO_TRACE("Execute Program: Addr=0x%x\r\n", global_sys_stage.data .start_addr);
        MMU_DisableIDCM();
        SPIFLASH_ResetQPIFlash(0);
#if !defined(PLATFORM_SC6531EFM) && !defined(PLATFORM_UWS6121E)
        SPIFLASH_ResetQPIFlash(1);
#endif
#ifdef SECURE_BOOT_SUPPORT
        JumpToTarget(SYS_MODEM_PROGRAM_START_ADDR);
#else
        JumpToTarget(global_sys_stage.data .start_addr );
#endif
    }

    return DL_SUCCESS;
}

static uint16 flash_get_checksum( uint32 addr, uint32 len )
{
    uint16  check_sum = 0xC50F + (uint16)addr;

    while( len-- )
	{
		check_sum += *(volatile uint16 *)(addr);
		addr      += sizeof(uint16);
	}
	return check_sum;
}

// Search the given id in the given sector contain the address.
static uint32 search_nvitem_in_a_sector( uint32 addr, uint16 id )
{
    uint32            sector_size, end_addr;
    uint16            item_len, item_id, check_sum;

    sector_size = FDL_FlashCheckSectorSize( addr );
    addr &= ~(sector_size - 1);  // sector boundary
    end_addr = addr + sector_size - 6; // The max address contain nvitem.

    while( addr < end_addr )
    {
        item_id = *(volatile uint16 *)(addr);
        if( 0xFFFF == item_id )
        {
            return FDL_INVALID_ADDR;  // reach tail, stop
        }
        else
        {
            // Check it is a valid item ?
            item_len  = *(volatile uint16 *)(addr + 6);
            check_sum = flash_get_checksum( addr + 4, (item_len + 4 + 1) >> 1 );
            if( check_sum != *(volatile uint16 *)(addr + 2) )
            {
                // Invalid item id
                return FDL_INVALID_ADDR;
            }
            else
            {
                // Valid
                if( item_id == id )
                {
                    return addr;
                }
                else
                {
                    // Skip this one
                    addr += 8 + item_len + (item_len & 0x1);
                }
            }
        }
    }

    return FDL_INVALID_ADDR;
}


//
// Read the given nvitem.
//
void lookup_nvitem( PACKET_T * packet_ptr );
void lookup_nvitem( PACKET_T * packet_ptr )
{
    uint32  start_addr, end_addr, nv_addr;
    uint16  nv_id, nv_len;
    unsigned long * ptr;

    /* analyse the packet*/
    ptr = (unsigned long *)(packet_ptr->packet_body.content);
    start_addr = *ptr++;
    end_addr   = *ptr++;
    nv_id = *((unsigned short *)ptr);


    //Init Flash
	FDL_FlashWriteEnable( get_flash_base_addr( start_addr ), 1 );

    nv_addr = FDL_INVALID_ADDR;
    nv_len  = 0xFFFF;
    while( start_addr < end_addr )
    {
        // search in a sector
        nv_addr = search_nvitem_in_a_sector( start_addr, nv_id );
        if( FDL_INVALID_ADDR == nv_addr )
        {
            // Not found, try find next sector
            start_addr += FDL_FlashCheckSectorSize( start_addr );
        }
        else
        {
            // Found, return the address/item length
            nv_len  = *(volatile uint16 *)(nv_addr + 6);
            nv_len += 8 + (nv_len & 0x1);
            break;
        }
    }

    //FDL_memcpy( &rep_ack[DATA_ADDR ], &nv_addr, sizeof(nv_addr) );
    //FDL_memcpy( &rep_ack[DATA_ADDR + sizeof(nv_addr) ], &nv_len, sizeof(nv_len) );
    //((pkt_header_ptr)&rep_ack)->pkt_type = BSL_REP_LOOKUP_NVITEM;

    //packet_size                          = sizeof(nv_addr) + sizeof(nv_len);
    //((pkt_header_ptr)&rep_ack)->pkt_size = packet_size;
    //sio_write((char *)rep_ack,  PKT_HEADER_SIZE + packet_size + PKT_CRC_SIZE);

    packet_ptr->packet_body.type = BSL_REP_LOOKUP_NVITEM;
    packet_ptr->packet_body.size = sizeof(nv_addr) + sizeof(nv_len);
    FDL_memcpy( packet_ptr->packet_body.content, &nv_addr, sizeof(nv_addr) );
    FDL_memcpy( &(packet_ptr->packet_body.content[sizeof(nv_addr) ]), &nv_len, sizeof(nv_len) );
    send_packet( packet_ptr );

    return;
}

//@Hyy->去FLASH CFG顺序的假设
//@含SORT功能，SORT when Insert复杂度低于SORT，本身就有INSERT功能即可用之
uint32 add_flash_partition_info(uint32 partition_addr)
{
	uint32 return_addr = partition_addr;
	uint32 i =0;
	uint32 j =0;

    if(partition_addr == 0xffffffff)
    {
        //invalid setting or default setting
        return 0xffffffff;
    }

	if(s_flash_partition_info_num == 0)
	{
		s_flash_partition_info_table[0] = partition_addr;
		s_flash_partition_info_num++;
		return return_addr;
	}

	if(s_flash_partition_info_num_in_cfg == s_flash_partition_info_num)
	{
		return return_addr;
	}

	//@快速通道
	if(partition_addr > s_flash_partition_info_table[s_flash_partition_info_num - 1])
	{
		s_flash_partition_info_table[s_flash_partition_info_num] = partition_addr;
		s_flash_partition_info_num++;
		return return_addr;
	}

	for(i=0; i< s_flash_partition_info_num; ++i)
	{
		if(partition_addr < s_flash_partition_info_table[i])
		{
			for(j = s_flash_partition_info_num; j > i; j--)
			{
				s_flash_partition_info_table[j] = s_flash_partition_info_table[j-1];
			}
			s_flash_partition_info_table[i] = partition_addr;
			s_flash_partition_info_num++;
			return return_addr;
		}
		else if(partition_addr == s_flash_partition_info_table[i])
		{
			SEND_ERROR_RSP(BSL_FLASH_CFG_ERROR);
        	handle_error();
			return return_addr;
		}
		else
		{
			//continue;
		}
	}

	return return_addr;
}
//@hyy
uint32 get_flash_partition_size(uint32 partition_addr)
{
	uint32 i =0;

    if(partition_addr == 0xffffffff)
    {
        //invalid setting or default setting, return size 0 directly
        return 0;
    }

	if(s_flash_partition_info_num== 0)
	{
		SEND_ERROR_RSP(BSL_FLASH_CFG_ERROR);
		return 0;
	}
	for(i=0; i< s_flash_partition_info_num; i++)
	{
		if(partition_addr == s_flash_partition_info_table[i])
		{
			if(i==(s_flash_partition_info_num - 1))
			{
				//末尾
				return  (SINGLE_CS_MAX_FLASH_SIZE - (partition_addr&(SINGLE_CS_MAX_FLASH_SIZE-1)));
			}
			else
			{
				if(s_flash_partition_info_table[i+1] <= s_flash_partition_info_table[i])
				{
					SEND_ERROR_RSP(BSL_FLASH_CFG_ERROR);
                        		handle_error();

					return 0;
				}
				//@跨CS末尾
				if((s_flash_partition_info_table[i+1] - s_flash_partition_info_table[i]) >= SINGLE_CS_MAX_FLASH_SIZE)
				{
					return (SINGLE_CS_MAX_FLASH_SIZE - (partition_addr&(SINGLE_CS_MAX_FLASH_SIZE-1)));
				}
				else
				{
					return (s_flash_partition_info_table[i+1] - s_flash_partition_info_table[i]);
				}
			}
		}
	}
	if(i==(s_flash_partition_info_num - 1))
	{
		SEND_ERROR_RSP(BSL_FLASH_CFG_ERROR);
		handle_error();
		return 0;
	}
	return 0;
}
//@hyy<-end

BOOLEAN  get_flash_partition_info(FLASH_PARTITION_PTR flash_partition_ptr)
{
    uint32  manufacturer_ID,device_ID,extend_ID;
    uint32 sect_size;
    uint32 cs = 0;
    if (flash_partition_ptr == 0)
    {
        return SCI_FALSE;
    }

    FDL_FlashWriteEnable( flash_infor[0].start_addr, 1 );

    cs = SPIFLASH_CS0;
    pSpiFlashSpec[cs] = SPIFLASH_SPEC_Get(nID_MID[cs], nID_DID1[cs], nID_DID2[cs]);

    if( NULL ==  pSpiFlashSpec[cs])
    {
        return SCI_FALSE;
    }

    //@hyy ->modify for free cfg
#ifdef NOR_BOOTLOADER_SUPPORT
    flash_partition_ptr->NorBootloaderAdress = FLASH_START_ADRESS + add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->start_addr);
#ifdef SECURE_BOOT_SUPPORT
    flash_partition_ptr->PsAdress  = FLASH_START_ADRESS + add_flash_partition_info (pSpiFlashSpec[cs]->spiflash_cfg->ps_addr - SECURE_CODE_OFF);  //Secure Header of size SECURE_CODE_OFF is substracted in secure boot
#else
    flash_partition_ptr->PsAdress  = FLASH_START_ADRESS + add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->ps_addr);
#endif
#else
    flash_partition_ptr->PsAdress  = FLASH_START_ADRESS + add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->start_addr);
#endif
    flash_partition_ptr->ResAdress = FLASH_START_ADRESS + add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->mmi_res);
    flash_partition_ptr->UmemAdress = add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->umem_addr); /*must not add FLASH_START_ADRESS !!!*/
    flash_partition_ptr->NvAdress  = FLASH_START_ADRESS + add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->fixnv_addr);
    flash_partition_ptr->ProdinfoAdress = FLASH_START_ADRESS + add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->prodinfo_addr);
    flash_partition_ptr->EfsAdress = FLASH_START_ADRESS + add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->efs_start_addr);
    flash_partition_ptr->Spload_Addr = FLASH_START_ADRESS + add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->spload_addr);
#ifdef SIM_LOCK_SUPPORT
    flash_partition_ptr->SimlockAddress=FLASH_START_ADRESS+ add_flash_partition_info(pSpiFlashSpec[cs]->spiflash_cfg->simlock_addr);
#endif
    flash_partition_ptr->FlashSize = pSpiFlashSpec[cs]->spiflash_cfg->flash_size;

    s_flash_partition_info_num_in_cfg = s_flash_partition_info_num;
    //@修改为排序方式，后进行SIZE分配
    //@主推荐FLASH配置方式修改为PS-- RES --NORUMEM -FIXNV+PRODUCTINFO -EFS后的修改
#ifdef NOR_BOOTLOADER_SUPPORT
    flash_partition_ptr->NorBootloaderSize = get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->start_addr);
#ifdef SECURE_BOOT_SUPPORT
    flash_partition_ptr->PSSize    = get_flash_partition_size (pSpiFlashSpec[cs]->spiflash_cfg->ps_addr - SECURE_CODE_OFF);  //Secure Header of size SECURE_CODE_OFF is substracted in secure boot
#else
    flash_partition_ptr->PSSize    = get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->ps_addr);
#endif
#else
    flash_partition_ptr->PSSize    = get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->start_addr);
#endif

    flash_partition_ptr->ResSize   = get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->mmi_res);

    flash_partition_ptr->NvSize    = get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->fixnv_addr);
    //@hyy<-End

    flash_partition_ptr->SploadSize   = get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->spload_addr);

#ifdef SIM_LOCK_SUPPORT
    flash_partition_ptr->SimlockSize= get_flash_partition_size(pSpiFlashSpec[cs]->spiflash_cfg->simlock_addr);
#endif

    //@根据其配的，不进行通用计算
    flash_partition_ptr->UmemSize  = pSpiFlashSpec[cs]->spiflash_cfg->umem_size;

    // Calculate the efssize by sect_num multiply sect_size.
    flash_partition_ptr->EfsSize   = (pSpiFlashSpec[cs]->spiflash_cfg->sect_num) * pSpiFlashSpec[cs]->spiflash_cfg->sect_size;
    sect_size = pSpiFlashSpec[cs]->spiflash_cfg->sect_size;

#if 0
    if((flash_partition_ptr->ProdinfoAdress/sect_size)==(flash_partition_ptr->NvAdress/sect_size))
    {
        s_share_fixnv_sector_flag = 1;
    }
#endif
#ifdef FLASH_TYPE_SPI
#if 0
    else
    {
        FDL_SendTrace("ERROR! ProdinfoAdress: 0x%x", flash_partition_ptr->ProdinfoAdress);

        // productinfo 和fixnv 必须共sector 否则不好给productinfo 和fixnv 同时做断电保护
        SEND_ERROR_RSP (BSL_REP_DOWN_DEST_ERROR);
    }
#endif
    g_prod_info_offset_addr = flash_partition_ptr->ProdinfoAdress - flash_partition_ptr->NvAdress;

    if (g_prod_info_offset_addr >= sect_size)
    {
        SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
    }

    if((flash_partition_ptr->NvAdress + sect_size - flash_partition_ptr->ProdinfoAdress) <= PROD_INFO_MIN_SIZE_512)
    {
        FDL_SendTrace("ERROR! ProdinfoAdress: 0x%x", flash_partition_ptr->ProdinfoAdress);

        // NPI tool 要求 productinfo size 必须大于4 KB, 否则CFT 工具和工厂工具会有问题。
        // 如果要修改这里的限制必须和NPI 人沟通并同步改NPI tool 的限制。
        SEND_ERROR_RSP (BSL_REP_DOWN_SIZE_ERROR);
    }
#endif

    return TRUE;
}

uint32	Equal_1K_LeftShift(uint32 count)
{
    uint32 i;

    for(i = 0;i<22;i++)
    {
    	if((0x400<<i) == count)
    	{
    	    return 1;
    	}
    }
    return 0;
}

#ifdef FLASH_TYPE_SPI
#define NORFLASH_ENTITY_PTR SPIFLASH_SPEC_PTR
#else
#define NORFLASH_ENTITY_PTR NORMCP_ENTITY_PTR
#endif

PUBLIC NORFLASH_ENTITY_PTR NORMCP_GetEntityTabInPs(uint32 *entity_cnt, uint32 *entity_size, uint32 * cfg_offset)
{
	NORFLASH_ENTITY_PTR entity_tab_ptr = (NORFLASH_ENTITY_PTR)(FLASH_START_ADRESS + FLASH_CFG_OFFSET);

	NOR_FLASH_CONFIG_PTR flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR)(FLASH_START_ADRESS + FLASH_CFG_OFFSET);
	uint32	i;
	uint32	total_sector_count;
	uint32 entity_element_size = 0;
	uint32 entity_flash_cfg_offset = 0;

	i = 0;

	//@hyy First Check Entity Table In PS, Entity Element Size,because Entity Maybe Changed
	//@第一步 预查找，目的是找到FLASH_CFG的位置，其在ENTITY的结构位置不作假设，可以变
	//	找到到两个后认为OK
	{
		uint32 enter_addr = (uint32)entity_tab_ptr;
		uint32 flash_cfg_addr = enter_addr;
		VOLATILE NOR_FLASH_CONFIG_PTR _flash_cfg_ptr = 0;
		uint32 j =0;
		uint32 k=0;
		uint32 _flash_cfg_ptr_array[2] = {0};

		while((flash_cfg_addr -enter_addr)< MAX_SECARCH_RANGE)
		{
			flash_cfg_addr = enter_addr + (j<<2);
			_flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR)((*(uint32*)flash_cfg_addr)+FLASH_START_ADRESS);
			if((_flash_cfg_ptr->start_addr == 0) && Equal_1K_LeftShift(_flash_cfg_ptr->sect_size))
			{

				if((_flash_cfg_ptr->flash_size & (_flash_cfg_ptr->sect_size -1)) == 0)/*equal:flash_cfg_ptr->flash_size%flash_cfg_ptr->sect_size*/
				{
					total_sector_count = _flash_cfg_ptr->flash_size/_flash_cfg_ptr->sect_size;
					if((_flash_cfg_ptr->file_sect_num<total_sector_count)&&
				  		 (_flash_cfg_ptr->bank_num<=total_sector_count)&&
				   		(_flash_cfg_ptr->sect_num<total_sector_count))
					{

						if(((_flash_cfg_ptr->efs_start_addr&(_flash_cfg_ptr->sect_size-1))==0)&&
			     				((_flash_cfg_ptr->fixnv_addr&(_flash_cfg_ptr->sect_size-1))==0))
						{

							//send_ack(0x97);
							_flash_cfg_ptr_array[k] = flash_cfg_addr;
							++k;
							if(k==1)
							{
								entity_flash_cfg_offset = (uint32)flash_cfg_addr - enter_addr;
							}
							if(k==2)
							{
								entity_element_size = _flash_cfg_ptr_array[1]- _flash_cfg_ptr_array[0];
								break;
							}
						}
					}

				}
			}
			j++;
			if((k==1) && (j>100))
			{
				break;
			}
		}
		if(k == 0)
		{
			*entity_cnt = 0;
			return NULL;
		}
		if(k == 1)
		{
			*entity_cnt = 1;
			*entity_size = 12;
			*cfg_offset = entity_flash_cfg_offset;
			 return entity_tab_ptr;
		}
	}
	//@error
	if(entity_element_size < 12)
	{
		*entity_cnt = 0;
		return NULL;
	}
	if(entity_flash_cfg_offset>= entity_element_size)
	{
		*entity_cnt = 0;
		return NULL;
	}


	//@第二步: 找EntityTab里面Entity的数量
	{
		uint32 enter_addr = (uint32)entity_tab_ptr;
		uint32 flash_cfg_addr = enter_addr;
		i=0;

		while(1)
		{
			flash_cfg_addr = enter_addr + entity_element_size * i + entity_flash_cfg_offset;
			flash_cfg_ptr = (NOR_FLASH_CONFIG_PTR)((*(uint32 *)flash_cfg_addr) +FLASH_START_ADRESS);
			if((flash_cfg_ptr->start_addr == 0) && Equal_1K_LeftShift(flash_cfg_ptr->sect_size))
			{
				if((flash_cfg_ptr->flash_size & (flash_cfg_ptr->sect_size -1)) == 0)/*equal:flash_cfg_ptr->flash_size%flash_cfg_ptr->sect_size*/
				{
					total_sector_count = flash_cfg_ptr->flash_size/flash_cfg_ptr->sect_size;
					if((flash_cfg_ptr->file_sect_num<total_sector_count)&&
				  		 (flash_cfg_ptr->bank_num<=total_sector_count)&&
				   		(flash_cfg_ptr->sect_num<total_sector_count))
					{

						if(((flash_cfg_ptr->efs_start_addr&(flash_cfg_ptr->sect_size-1))==0)&&
			     				((flash_cfg_ptr->fixnv_addr&(flash_cfg_ptr->sect_size-1))==0))

						{
							i++;
							continue;
						}
					}
				}
			}
			break;
		}

	}
	*entity_cnt = i;
	*entity_size = entity_element_size;
	*cfg_offset = entity_flash_cfg_offset;
	#if 0
	SCI_BootUp_UartTrace("[Yiyue]: entity_tab_ptr=0x%x,%d,%d,%d",(uint32)entity_tab_ptr,i,entity_element_size,entity_flash_cfg_offset);
	#endif
	return entity_tab_ptr;
}

PUBLIC SPIFLASH_SPEC_PTR NORMCP_GetEntityInPs(
									uint32 manu_id,
									uint32 devc_id,
									uint32 extend_id,
									uint32 *index,
									uint32 *cfg_offset)
{
	uint32 table_index 				 = 0;
	uint32 table_num 				 = 0;
	uint32 entity_size = 0;
	SPIFLASH_SPEC_PTR entity_ptr 	 = PNULL;
	SPIFLASH_SPEC_PTR entity_tab_ptr = NORMCP_GetEntityTabInPs(&table_num,&entity_size,cfg_offset);
	SPIFLASH_SPEC_PTR	flash_spec_ptr;

	for(table_index=0; table_index < table_num; ++table_index)
	{
		flash_spec_ptr = (SPIFLASH_SPEC_PTR)(((uint32)((SPIFLASH_SPEC_PTR)(((uint32)entity_tab_ptr)+(table_index*entity_size)))));

		if( (  manu_id   == flash_spec_ptr->nMID)
			&&(devc_id   == flash_spec_ptr->nDID1)
			&&(extend_id == flash_spec_ptr->nDID2))
		{
			//Find Nor Type
			entity_ptr = ((SPIFLASH_SPEC_PTR)(((uint32)entity_tab_ptr)+(table_index*entity_size)));
			break;
		}
	}

    if (PNULL != index)
    {
        if (table_index == table_num)
        {
            *index = 0xFFFFFFFF;
        }
        else
        {
            *index = table_index;
        }
    }

    return entity_ptr;
}

NOR_FLASH_CONFIG_PTR get_flash_partition_info_InPS(void)
{
    uint32 index;
    SPIFLASH_SPEC_PTR entity_ptr = PNULL;
    NOR_FLASH_CONFIG_PTR flash_cfg = PNULL;
    uint32 cfg_offset =0;
    uint32 cs = 0;

    FDL_FlashWriteEnable( flash_infor[0].start_addr, 1 );
    cs= SPIFLASH_CS0;
    entity_ptr = NORMCP_GetEntityInPs(nID_MID[cs],nID_DID1[cs],nID_DID2[cs],&index,&cfg_offset);
    if(entity_ptr==NULL) return NULL;

    flash_cfg  = (NOR_FLASH_CONFIG_PTR)((*(uint32 *)(((uint32)entity_ptr)+cfg_offset))+FLASH_START_ADRESS);

    return flash_cfg;
}

uint16 calculate_fixNV_length( uint32  start_addr, uint32 end_addr)
{
	uint32  cur_addr;
	uint16  id, fix_dedicate_len, len;
	BOOLEAN is_sram_fixnv = SCI_FALSE;

	if((start_addr >= SRAM_START_ADDR) && (start_addr <= (SRAM_START_ADDR + SINGLE_CS_MAX_FLASH_SIZE)))
	{
		is_sram_fixnv = SCI_TRUE;
	}
	if(is_sram_fixnv)
	{
	    fix_dedicate_len = *(uint16 *)(end_addr - FIX_NV_LEN_OFFSET);
	}
	else
	{
	    fix_dedicate_len = FDL_FlashReadWord(end_addr - FIX_NV_LEN_OFFSET);
    }

	cur_addr = start_addr + sizeof(uint32); // skip update flag.
	while( cur_addr < end_addr )
	{
		if(is_sram_fixnv)
		{
			id   = *(uint16 *)(cur_addr);
			len = *(uint16 *)(cur_addr+2);
		}
		else
		{
			id   = FDL_FlashReadWord(cur_addr);
			len = FDL_FlashReadWord(cur_addr+2);
		}

        //FDL_SendTrace("%s %d id=0x%x, len=0x%x 0x%x %d", __func__, __LINE__, id, len, cur_addr, is_sram_fixnv);

		cur_addr += 4;
		if( EFS_INVALID_ITEM == id ) // Reach tail
		{
			cur_addr -= 4;
			break; // Stop loop
		}
		else if(id > ITEM_MASK_ID)
		{
		    FDL_SendTrace("ERROR! %s %d id=0x%x, start_addr=0x%x", __func__, __LINE__, id, start_addr);
			return ERR_NV_LEN;
		}
		if((len == 0x0) || (id == 0))
		{
		    FDL_SendTrace("ERROR! %s %d id=0x%x, len=0x%x, start_addr=0x%x", __func__, __LINE__, id, len, start_addr);
			return ERR_NV_LEN;
		}
		cur_addr += len + (len & 0x1); // WORD boundary
		cur_addr +=3;
		cur_addr &=0xfffffffc;
	}

	if(fix_dedicate_len != (uint16)(cur_addr - start_addr))
	{
	    FDL_SendTrace("ERROR! %s %d fix_dedicate_len=0x%x, cur_addr=0x%x, start_addr=0x%x",
	        __func__, __LINE__, fix_dedicate_len, cur_addr, start_addr);
		return ERR_NV_LEN;
    }

    //FDL_SendTrace("%s start_addr=0x%x fix_dedicate_len=0x%x", __func__, start_addr, fix_dedicate_len);
    return fix_dedicate_len;
}



data_download*  FDL_GetCurDownloadInfo(void)
{
    return &global_sys_stage.data;
}

uint32 Get_fdl_file_end_addr(void)
{
    return 0;
}

uint32 Get_productinfo_offset(void)
{
    return g_prod_info_offset_addr;
}

BOOLEAN Get_phasecheck_flag(void)
{
    return g_is_dload_phasecheck;
}
/*add dummy func here*/
void SCI_Assert(void* exp,  void* file, uint32  line){}





			//flash_info->PsAdress = s_flash_partition_info_ptr->PsAdress;
			//flash_info->PSSize = s_flash_partition_info_ptr->PSSize;










#endif

